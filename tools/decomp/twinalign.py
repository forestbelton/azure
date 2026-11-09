#!/usr/bin/env python3
"""Pair main.bin's debug twin at 0x80400000 against the live overlay's source.

THE TWO BLOCKS. `main.bin+0x25d000` (vram 0x80020000) is the live card overlay,
241 functions, decompiled and byte-identical. `main.bin+0x269000` (vram
0x80400000) is a DEBUG BUILD OF THE SAME SOURCE at -G8, with more of its asserts
compiled in, linked against a different engine build, and dead on retail
(0x80400000 is 8 MB dev-board RAM). It is in the container, so it still has to
be reproduced. See docs/overlays.md.

WHAT THIS TOOL IS FOR. The twin is not a second decompilation: most of its code
is code we already have. This measures exactly how much, pairs each of its
functions with the live function it came from, and -- the part that makes the
work mechanical -- derives the LIVE -> TWIN SYMBOL MAP, so a live function's
body can be moved over with its references renamed rather than re-read out of a
listing.

HOW. The live unit is compiled at -G8 (not the -G0 the live BLOCK is built at;
-G is the whole difference in shape, and -G8 is what the twin used), and the two
function sequences are aligned by an order-preserving global DP over masked
instruction words. Masking drops immediates, relocation targets and branch
labels, since those necessarily differ between the two links; register names are
kept, because allocation differing is real signal.

THE SYMBOL MAP falls out of the aligned pairs. The -G8 object carries a
relocation per symbol reference (`objdump -dr` prints `R_MIPS_26 reset_sw_events`
under the `jal`), and the twin's listing names its own symbols in the
disassembly (`jal func_804014E4`, `%hi(D_804094EC)`). Within a pair, the Nth
symbol reference on one side is the Nth on the other, so zipping the two ordered
lists votes one mapping per pair. A pair whose two lists differ in LENGTH is
skipped rather than guessed at -- that is the signature of an assert the retail
build compiled out, which adds references to ASSERT_FMT and a filename.

FOUR CAUTIONS, three of them paid for.

  * The twin's external `jal` targets are NOT retail engine addresses. It calls
    printf at 0x8007C040 and exit at 0x8007BEF0, which in the shipped
    executable land inside libgte's `rcossin_tbl`. The symbol map's whole point
    is that these are DIFFERENT symbols; never carry a live address across.
  * There is no constant offset between the two blocks' data. `card_slot` is
    D_800287CC live and D_804094EC here (delta 0x3E0D20) while the data bases
    differ by 0x3E1320. Use the map, not arithmetic.
  * `make splat` regenerates the nonmatchings this tool reads, and make has no
    dependency from a C unit's .o on them. Run `rm -f build/csrc/<unit>.o`
    after a re-split or you will compare against a stale object.
  * A function pairing is a MEASUREMENT, not a fact about the source. 45
    functions sit at >40% difference and 37 assert sites do not obviously
    explain them, so part of the twin may be an EARLIER REVISION of the source.
    Treat a low-similarity pair as a question, not an instruction.

Usage:
    python3 tools/decomp/twinalign.py                 # the coverage table
    python3 tools/decomp/twinalign.py --pairs         # every function pairing
    python3 tools/decomp/twinalign.py --symmap        # live -> twin symbols
    python3 tools/decomp/twinalign.py --todo          # what is worth moving next
    python3 tools/decomp/twinalign.py --fn func_804008A0   # one pair, side by side
    python3 tools/decomp/twinalign.py --json out.json
"""

import argparse
import collections
import difflib
import json
import pathlib
import re
import subprocess
import sys

ROOT = pathlib.Path(__file__).resolve().parent.parent.parent

LIVE_SRC = ROOT / "csrc/ovl_main_80020000.c"
TWIN_SRC = ROOT / "csrc/ovl_main_80400000.c"
TWIN_ASM = ROOT / "asm/nonmatchings/ovl_main_80400000"
BUILD = ROOT / "build/twinalign"

CC = ROOT / "tools/cdkgcc"
MASPSX = ROOT / "vendor/maspsx/maspsx.py"
AS = "mips64-linux-gnu-as"
OBJDUMP = "mips64-linux-gnu-objdump"

REGS = r"(zero|at|v[01]|a[0-3]|t\d|s[0-7]|k[01]|gp|sp|fp|ra)"


# --------------------------------------------------------------------------
# normalising one instruction so the two printers can be compared


def norm(ins):
    ins = re.sub(r"<[^>]*>", "", ins).strip().replace("\t", " ")
    ins = re.sub(r"\s+", " ", ins)
    mn, _, rest = ins.partition(" ")
    mn = mn.lower()
    ops = [o.strip().replace("$", "") for o in rest.split(",") if o.strip()]

    def o(i):
        return ops[i] if i < len(ops) else ""

    # the pseudo-ops objdump and spimdisasm spell differently
    if mn == "move":
        mn, ops = "addu", [o(0), o(1), "zero"]
    elif mn == "li":
        mn, ops = "addiu", [o(0), "zero", "I"]
    elif mn == "b":
        mn, ops = "beq", ["zero", "zero", "I"]
    elif mn == "beqz":
        mn, ops = "beq", [o(0), "zero", "I"]
    elif mn == "bnez":
        mn, ops = "bne", [o(0), "zero", "I"]
    elif mn == "negu":
        mn, ops = "subu", [o(0), "zero", o(1)]
    elif mn == "not":
        mn, ops = "nor", [o(0), o(1), "zero"]
    elif mn == "bal":
        mn, ops = "jal", ["I"]

    out = []
    for x in ops:
        m = re.match(r"^(?:-?(?:0x[0-9a-fA-F]+|\d+)|%lo\(.*\)|%hi\(.*\))\((\w+)\)$", x)
        if m:
            out.append("I(%s)" % m.group(1))
        elif re.match(r"^-?(0x[0-9a-fA-F]+|\d+)$", x):
            out.append("I")
        elif re.match(r"^" + REGS + r"$", x):
            out.append(x)
        else:
            out.append("I")
    return mn + " " + ",".join(out)


# --------------------------------------------------------------------------
# the two sides


def build_live_g8():
    """Compile the live unit at -G8 and return the object's path."""
    BUILD.mkdir(parents=True, exist_ok=True)
    s = BUILD / "live_g8.s"
    o = BUILD / "live_g8.o"
    if o.exists() and o.stat().st_mtime > LIVE_SRC.stat().st_mtime:
        return o
    subprocess.run([str(CC), "-O2", "-Iinclude", "-Ipsyq/include", "-G8", "-S",
                    "-o", str(s), str(LIVE_SRC)], cwd=ROOT, check=True,
                   stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    with open(s) as fh:
        subprocess.run(["python3", str(MASPSX), "--aspsx-version=2.56",
                        "--force-stdin", "--run-assembler", "--gnu-as-path=" + AS,
                        "--use-comm-section", "-G8", "-EL", "-32", "-Iinclude",
                        "-march=r3000", "-mtune=r3000", "-no-pad-sections", "-O1",
                        "-o", str(o)], cwd=ROOT, check=True, stdin=fh,
                       stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
    return o


def load_live(obj):
    """Functions of the -G8 object, with the symbol each instruction relocates."""
    out = subprocess.run([OBJDUMP, "-dr", "--no-show-raw-insn", str(obj)],
                         cwd=ROOT, capture_output=True, text=True).stdout
    funcs, cur = [], None
    for line in out.split("\n"):
        m = re.match(r"^[0-9a-f]+ <(\w+)>:", line)
        if m:
            cur = {"name": m.group(1), "ins": [], "raw": [], "syms": []}
            funcs.append(cur)
            continue
        if cur is None:
            continue
        # relocations FIRST: "\t\t\t2c: R_MIPS_HI16\tD_800287CC" also matches
        # the instruction pattern below, and counting them as instructions
        # inflated the live unit from 7,529 words to 9,119.
        m = re.match(r"^\s+[0-9a-f]+:\s+(R_MIPS_\w+)\s+(\S+)", line)
        if m:
            if cur["ins"]:
                kind, sym = m.group(1), m.group(2).split("+")[0]
                # A reloc against a SECTION NAME (`.text`, `.rodata`) is how
                # gas encodes a branch to a local label or a local constant.
                # The twin's listing spells those `.L80401298`, which is
                # filtered on that side -- so they have to be filtered here
                # too, or the two reference lists differ in LENGTH and the
                # pair is skipped. That alone lost _card_load, and with it
                # 19 words of func_8040122C to a call into the resident
                # engine instead of this block's own static copy.
                if kind in ("R_MIPS_26", "R_MIPS_HI16", "R_MIPS_LO16") \
                   and not sym.startswith("."):
                    cur["syms"].append((len(cur["ins"]) - 1, kind, sym))
            continue
        m = re.match(r"^\s+[0-9a-f]+:\s+(.*)$", line)
        if m:
            t = m.group(1).strip()
            if t:
                cur["ins"].append(norm(t))
                cur["raw"].append(t)
    return [f for f in funcs if f["ins"]]


def load_twin():
    """Functions of the twin, read from the per-function nonmatchings listings."""
    funcs = []
    for path in sorted(TWIN_ASM.glob("*.s")):
        cur = None
        in_text = True   # a listing with no `.section` at all is all text
        for line in open(path):
            s = line.strip()
            if s.startswith(".section"):
                in_text = ".text" in s
                continue
            if not in_text:
                continue
            m = re.match(r"(?:glabel|alabel)\s+(\w+)", s)
            if m:
                # an alabel is an alternate entry INSIDE the preceding function
                if s.startswith("glabel"):
                    cur = {"name": m.group(1), "ins": [], "raw": [], "syms": [],
                           "vram": None, "alts": []}
                    funcs.append(cur)
                elif cur is not None:
                    cur["alts"].append(m.group(1))
                continue
            if cur is None:
                continue
            m = re.match(r"/\* [0-9A-F]+ ([0-9A-F]{8}) [0-9A-F]{8} \*/\s+(.*)", s)
            if not m:
                continue
            vram, ins = int(m.group(1), 16), m.group(2).strip()
            if cur["vram"] is None:
                cur["vram"] = vram
            cur["ins"].append(norm(ins))
            cur["raw"].append(ins)
            for kind, pat in (("R_MIPS_26", r"^(?:jal|j)\s+(\w+)"),
                              ("R_MIPS_HI16", r"%hi\((\w+)\)"),
                              ("R_MIPS_LO16", r"%lo\((\w+)\)")):
                mm = re.search(pat, ins)
                if mm and not mm.group(1).startswith(".L"):
                    cur["syms"].append((len(cur["ins"]) - 1, kind, mm.group(1)))
    funcs = [f for f in funcs if f["ins"]]
    funcs.sort(key=lambda f: f["vram"])
    return funcs


# --------------------------------------------------------------------------
# alignment


def sim(a, b):
    return difflib.SequenceMatcher(a=a["ins"], b=b["ins"], autojunk=False).ratio()


def align(live, twin, gap=-0.25):
    L, T = len(live), len(twin)
    s = [[sim(live[i], twin[j]) for j in range(T)] for i in range(L)]
    NEG = float("-inf")
    dp = [[NEG] * (T + 1) for _ in range(L + 1)]
    bt = [[None] * (T + 1) for _ in range(L + 1)]
    dp[0][0] = 0.0
    for i in range(L + 1):
        for j in range(T + 1):
            v = dp[i][j]
            if v == NEG:
                continue
            if i < L and j < T and v + s[i][j] > dp[i + 1][j + 1]:
                dp[i + 1][j + 1] = v + s[i][j]
                bt[i + 1][j + 1] = ("M", i, j)
            if i < L and v + gap > dp[i + 1][j]:
                dp[i + 1][j] = v + gap
                bt[i + 1][j] = ("L", i, j)
            if j < T and v + gap > dp[i][j + 1]:
                dp[i][j + 1] = v + gap
                bt[i][j + 1] = ("T", i, j)
    pairs, i, j = [], L, T
    while (i, j) != (0, 0):
        kind, pi, pj = bt[i][j]
        if kind == "M":
            pairs.append((live[pi], twin[pj], s[pi][pj]))
        elif kind == "L":
            pairs.append((live[pi], None, 0.0))
        else:
            pairs.append((None, twin[pj], 0.0))
        i, j = pi, pj
    pairs.reverse()
    return pairs


def classify(a, b, score):
    if a is None:
        return "twin-only"
    if b is None:
        return "live-only"
    if a["ins"] == b["ins"]:
        return "identical"
    ops = difflib.SequenceMatcher(a=a["ins"], b=b["ins"], autojunk=False).get_opcodes()
    if set(t for t, *_ in ops) <= {"equal", "insert"}:
        return "inserts-only"
    if score >= 0.90:
        return "near"
    if score >= 0.60:
        return "drift"
    return "far"


ORDER = ["identical", "inserts-only", "near", "drift", "far", "twin-only", "live-only"]
BLURB = {
    "identical": "already identical at -G8",
    "inserts-only": "live body is a strict subsequence (asserts inserted)",
    "near": ">=90% same",
    "drift": "60-90% same",
    "far": "<60% same -- may be an earlier revision",
    "twin-only": "no live counterpart (statically linked engine code)",
    "live-only": "live function the twin lacks",
}


# --------------------------------------------------------------------------
# the symbol map


def symbol_map(pairs):
    votes = collections.defaultdict(collections.Counter)
    skipped = 0
    for a, b, _ in pairs:
        if a is None or b is None:
            continue
        la = [s for s in a["syms"]]
        lb = [s for s in b["syms"]]
        if len(la) != len(lb) or not la:
            skipped += 1
            continue
        for (_, ka, sa), (_, kb, sb) in zip(la, lb):
            if ka != kb:
                skipped += 1
                break
            votes[sa][sb] += 1
    return votes, skipped


def landed_in_twin():
    if not TWIN_SRC.exists():
        return set()
    src = TWIN_SRC.read_text()
    stubbed = set(re.findall(r'INCLUDE_ASM\("[^"]+",\s*(\w+)\)', src))
    defined = set(re.findall(r"^[\w \*]+?(\bfunc_\w+)\([^;)]*\)\s*\n\{",
                             src, re.MULTILINE))
    return defined - stubbed


# --------------------------------------------------------------------------


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("--pairs", action="store_true", help="every function pairing")
    ap.add_argument("--symmap", action="store_true", help="live -> twin symbol map")
    ap.add_argument("--todo", action="store_true", help="unlanded pairs, easiest first")
    ap.add_argument("--fn", metavar="NAME", help="one pair, side by side")
    ap.add_argument("--json", metavar="FILE", help="write the whole pairing as JSON")
    ap.add_argument("--class", dest="klass", choices=ORDER, help="restrict --pairs/--todo")
    args = ap.parse_args()

    if not TWIN_ASM.is_dir():
        sys.exit("no %s -- run `make splat` first" % TWIN_ASM.relative_to(ROOT))
    live = load_live(build_live_g8())
    twin = load_twin()
    pairs = align(live, twin)
    landed = landed_in_twin()
    tw = sum(len(f["ins"]) for f in twin)

    rows = [(a, b, s, classify(a, b, s)) for a, b, s in pairs]

    if args.fn:
        for a, b, s, k in rows:
            if (b and b["name"] == args.fn) or (a and a["name"] == args.fn):
                print("%s  <->  %s   similarity %.2f   %s"
                      % (a["name"] if a else "-", b["name"] if b else "-", s, k))
                ra = a["raw"] if a else []
                rb = b["raw"] if b else []
                for x, y in zip(ra + [""] * len(rb), rb + [""] * len(ra)):
                    if not x and not y:
                        break
                    flag = " " if norm(x or "") == norm(y or "") else "|"
                    print("  %-38s %s %s" % (x, flag, y))
                return
        sys.exit("no pair for %s" % args.fn)

    if args.symmap:
        votes, skipped = symbol_map(pairs)
        print("LIVE -> TWIN symbol map: %d live symbols, from %d pairs (%d skipped)\n"
              % (len(votes), len(pairs) - skipped, skipped))
        for sa in sorted(votes):
            best = votes[sa].most_common()
            flag = "" if len(best) == 1 else "   AMBIGUOUS: " + ", ".join(
                "%s x%d" % (s, n) for s, n in best[1:])
            print("  %-22s -> %-22s x%-3d%s" % (sa, best[0][0], best[0][1], flag))
        return

    if args.pairs or args.todo:
        sel = [r for r in rows if not args.klass or r[3] == args.klass]
        if args.todo:
            sel = [r for r in sel if r[1] and r[1]["name"] not in landed]
            sel.sort(key=lambda r: (ORDER.index(r[3]), len(r[1]["ins"])))
        for a, b, s, k in sel:
            mark = "LANDED" if b and b["name"] in landed else ""
            print("  %-16s %4dw  <->  %-16s %4dw  %.2f  %-12s %s"
                  % (b["name"] if b else "-", len(b["ins"]) if b else 0,
                     a["name"] if a else "-", len(a["ins"]) if a else 0,
                     s, k, mark))
        print("\n  %d rows" % len(sel))
        return

    if args.json:
        blob = [{"twin": b["name"] if b else None,
                 "twin_words": len(b["ins"]) if b else 0,
                 "live": a["name"] if a else None,
                 "live_words": len(a["ins"]) if a else 0,
                 "similarity": round(s, 4), "class": k,
                 "landed": bool(b and b["name"] in landed)}
                for a, b, s, k in rows]
        votes, _ = symbol_map(pairs)
        pathlib.Path(args.json).write_text(json.dumps(
            {"pairs": blob,
             "symmap": {k: v.most_common(1)[0][0] for k, v in votes.items()}},
            indent=2))
        print("wrote %s" % args.json)
        return

    # default: the coverage table
    print("twin at 0x80400000: %d functions, %d words.  live unit at -G8: %d functions, %d words\n"
          % (len(twin), tw, len(live), sum(len(f["ins"]) for f in live)))
    by = collections.defaultdict(list)
    for a, b, s, k in rows:
        by[k].append((a, b))
    for k in ORDER:
        if k not in by:
            continue
        n = len(by[k])
        w = sum(len(b["ins"]) for _, b in by[k] if b)
        print("  %-13s %3d fn  %5d words  %5.1f%%   %s"
              % (k, n, w, 100.0 * w / tw, BLURB[k]))
    paired = sum(len(b["ins"]) for a, b, s, k in rows if a and b)
    print("\n  paired with a live function: %d of %d words (%.1f%%)"
          % (paired, tw, 100.0 * paired / tw))
    print("  landed in %s: %d of %d functions"
          % (TWIN_SRC.relative_to(ROOT), len(landed), len(twin)))


if __name__ == "__main__":
    main()
