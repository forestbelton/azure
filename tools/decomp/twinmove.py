#!/usr/bin/env python3
"""Move a live function's body into the 0x80400000 twin, renaming its symbols.

The twin is a debug build of the live card overlay's source (see
docs/overlays.md and the header of csrc/ovl_main_80400000.c), so almost
nothing in it needs decompiling: the body is already in
csrc/ovl_main_80020000.c and what differs is which symbols it names. This
copies the body across, renames every reference through
`twinalign.py --symmap`, and drops it in place of its INCLUDE_ASM.

ALWAYS MEASURE WHAT THIS TOOL WROTE:

    python3 tools/decomp/ovlverify.py --G 8 csrc/ovl_main_80400000.c <fn>

and note the `--G 8`, which is the opposite of every other overlay unit. The
measurement is not a formality, because this tool has a SILENT failure mode
that cost 19 words of func_8040122C once: a NAMED engine function missing
from the symbol map (`_card_load`, not a `func_`/`D_` symbol, so it does not
look unresolved) renames to nothing and links against the RESIDENT engine
instead of this block's own static copy. The link succeeds and the container
is wrong. A `func_`/`D_` symbol that is missing is reported as UNMAPPED; a
named one cannot be, so the only guard is the measurement.

A symbol whose map vote is AMBIGUOUS is refused rather than guessed at, and
reported, so it can be renamed by hand.

WHAT IT CANNOT DO. It carries the body over verbatim. Where the twin's source
genuinely differs -- a table with one more entry, a different word index into
its data, a local the live source does not have -- the move lands with
differing immediates and the adjustment is yours to make, with --patch. Every
such adjustment so far has been an immediate, never a different spelling of
the body; the four families found in the first 86 functions are written up in
the unit's header.

Usage:
    python3 tools/decomp/twinmove.py func_804008A0              # move (live fn from the pairing)
    python3 tools/decomp/twinmove.py func_804008A0 set_card_slot
    python3 tools/decomp/twinmove.py func_804008A0 --dry        # print, do not write
    python3 tools/decomp/twinmove.py func_80402BB8 --patch 'p[0x238]=>p[0x1A7]'
    python3 tools/decomp/twinmove.py func_804038C8 --back       # back out to INCLUDE_ASM
"""

import argparse
import json
import pathlib
import re
import sys

sys.path.insert(0, str(pathlib.Path(__file__).resolve().parent))
import twinalign  # noqa: E402

ROOT = twinalign.ROOT
# THE LIVE SIDE IS NINE FILES, NOT ONE. This was `csrc/ovl_main_80020000.c`
# alone, which the 2026-11-10 carve emptied of every function body -- the same
# staleness that made twinalign pair nothing. The list comes from
# twinalign.live_srcs(), which derives it from the yaml, so a future carve
# cannot break it either.
LIVE_SRCS = list(twinalign.live_srcs())
TWIN = ROOT / "csrc/ovl_main_80400000.c"
HDR = ROOT / "csrc/main_twin.h"
CACHE = twinalign.BUILD / "symmap.json"
STUB = 'INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", %s);'
# every function the LIVE units define, so a name that survives renaming can be
# reported instead of linking silently against the resident engine
LIVE_NAMES = set()
for _src in LIVE_SRCS:
    LIVE_NAMES |= set(re.findall(r"^[\w \*]+?(\w+)\([^;)]*\)\s*\n\{",
                                 _src.read_text(), re.MULTILINE))


def live_aliases():
    """{name used in live source: the symbol it really resolves to}.

    TWO SPELLINGS, and missing the second one cost six functions on
    2026-11-11. A live global can be named through a `#define NAME D_XXXXXXXX`
    OR through an `__asm__` alias -- `extern s32 card_file_count[]
    __asm__("D_80084128");`. Both have to be resolved to the real symbol
    BEFORE the live -> twin rename, or the name survives, main.h's own
    declaration resolves it at compile time, and the function links against
    the RESIDENT engine. It compiles, it links, and ovlverify reports 0
    differing, because the relocation is masked. Only the container link says
    anything.
    """
    out = {}
    for f in ("csrc/main.h", "include/structs.h"):
        txt = (ROOT / f).read_text()
        out.update(re.findall(r"^#define (\w+) (D_[0-9A-F]{8})$", txt, re.MULTILINE))
        out.update((n, t) for n, t in re.findall(
            r"\b(\w+)\s*(?:\[[^\]]*\])?\s*__asm__\(\"((?:D_|func_)[0-9A-F]{8})\"\)", txt))
    return out


def symmap(refresh=False):
    """The live -> twin map, plus the symbols whose vote was ambiguous."""
    if CACHE.exists() and not refresh:
        d = json.loads(CACHE.read_text())
        return d["map"], set(d["ambig"])
    live = twinalign.load_live(twinalign.build_live_g8())
    twin = twinalign.load_twin()
    votes, _ = twinalign.symbol_map(twinalign.align(live, twin))
    m, ambig = {}, set()
    for sa, counter in votes.items():
        best = counter.most_common()
        m[sa] = best[0][0]
        if len(best) > 1 and best[1][1] == best[0][1]:
            ambig.add(sa)          # a tie is not a mapping
    twinalign.BUILD.mkdir(parents=True, exist_ok=True)
    CACHE.write_text(json.dumps({"map": m, "ambig": sorted(ambig)}, indent=1))
    return m, ambig


def pairs():
    live = twinalign.load_live(twinalign.build_live_g8())
    twin = twinalign.load_twin()
    return twinalign.align(live, twin)


def pairing_map():
    """{live function name: twin function name} from the ALIGNMENT.

    A second source of renames, and a weaker one than a --symmap vote: the map
    is derived from reference lists that have to agree in length, which skips
    every assert-bearing pair, so a function only ever CALLED from such a pair
    is missing from it. The alignment still pairs it. HandleCardError,
    HandleCardTimeout and HandleCardNew are the case that found this: all
    three are passed to OpenEvent in func_80400A10, whose reference lists
    differ in length because of its sixteen asserts, so the map had only
    HandleCardEOF and the other three silently stayed live names.
    """
    return {a["name"]: b["name"] for a, b, _ in pairs() if a and b}


def pairing(twin_fn):
    """The live function this one was compiled from, per twinalign."""
    for a, b, s in pairs():
        if b is not None and b["name"] == twin_fn:
            if a is None:
                sys.exit("%s has no live counterpart (twin-only); it is engine code "
                         "this build linked statically -- see the unit header" % twin_fn)
            return a["name"], s
    sys.exit("%s is not in the twin's listings (already landed?)" % twin_fn)


def span(src, fn):
    """(start, end) of fn's definition in src, leading comment included."""
    m = re.search(r"^[\w \*]+?\b" + fn + r"\([^;)]*\)\s*\n\{", src, re.MULTILINE)
    if not m:
        return None
    start = m.start()
    # a /* ... */ block immediately above belongs to the function
    head = src[:start].rstrip("\n")
    if head.endswith("*/"):
        o = head.rfind("/*")
        if o != -1 and "\n\n" not in head[o:]:
            start = o
    depth, i = 0, src.index("{", m.end() - 1)
    for k in range(i, len(src)):
        if src[k] == "{":
            depth += 1
        elif src[k] == "}":
            depth -= 1
            if depth == 0:
                return start, k + 1
    sys.exit("unbalanced braces in " + fn)


def extract(fn):
    """The live definition of FN, from whichever of the nine live units has it."""
    for path in LIVE_SRCS:
        src = path.read_text()
        m = re.search(r"^[\w \*]+?\b" + fn + r"\([^;)]*\)\s*\n\{",
                      src, re.MULTILINE)
        if not m:
            continue
        s, e = span(src, fn)
        # the live body's own comment is about the LIVE function; leave it behind
        return src[m.start():e], path
    sys.exit("no definition of %s in any of %s"
             % (fn, ", ".join(p.name for p in LIVE_SRCS)))


def do_move(twin_fn, live_fn, dry):
    m, ambig = symmap()
    fnmap = pairing_map()
    body, live_path = extract(live_fn)
    macros = live_aliases()
    used, refused, byalign = set(), set(), set()

    hdr = HDR.read_text()

    def keep_suffix(tok, real, twin):
        """Carry an alias SUFFIX across the rename when the twin has one too.

        The live source reaches one object under two names on purpose -- the
        per-site declared-size lever -- so `D_800814A0_a` must become
        `D_8008DAB4_a` and not the bare `D_8008DAB4`, which is declared as a
        scalar and would not even compile. Only used when main_twin.h actually
        declares the suffixed spelling; otherwise the bare name goes in and
        the compiler says so, which is the right kind of failure.
        """
        if tok != real and tok.startswith(real):
            cand = twin + tok[len(real):]
            if re.search(r"\b" + cand + r"\b", hdr):
                return cand
        return twin

    def sub(mo):
        tok = mo.group(0)
        real = macros.get(tok, tok)
        if tok == live_fn:
            return twin_fn
        if real in ambig:
            refused.add(real)
            return tok
        if real in m:
            t = keep_suffix(tok, real, m[real])
            used.add((real, t))
            return t
        if real in fnmap:
            byalign.add(real)
            t = keep_suffix(tok, real, fnmap[real])
            used.add((real, t))
            return t
        return tok

    new = re.sub(r"\b[A-Za-z_]\w*\b", sub, body)
    unmapped = sorted(set(re.findall(r"\b(?:func_[0-9A-F]{8}|D_[0-9A-F]{8})\b", new))
                      - {twin_fn} - {t for _, t in used})
    # a live NAME that is neither mapped nor paired: the silent failure mode,
    # since it compiles and links against the resident engine
    unmapped += sorted(n for n in re.findall(r"\b[A-Za-z_]\w*\b", new)
                       if n in LIVE_NAMES and n not in m and n not in fnmap)
    # AND AN ALIAS THAT SURVIVED RENAMING, which is the same silent failure one
    # level down: `card_file_count` is not a func_/D_ name so it is not caught
    # above, and main.h's __asm__ alias quietly resolves it to the RESIDENT
    # D_80084128. Six functions linked that way on 2026-11-11 and every one of
    # them measured 0 differing; only the container caught it.
    unmapped += sorted(n for n in set(re.findall(r"\b[A-Za-z_]\w*\b", new))
                       if n in macros)
    # AND EVERY NAMED CALLEE THAT SURVIVED, which is the widest form of the
    # same failure. This block linked its OWN copy of the C library and the
    # BIOS directory calls at addresses that are not the resident ones, so a
    # surviving `strcpy` or `firstfile2` compiles, links against the resident
    # engine and measures 0 differing -- func_804019B4 did exactly that on
    # 2026-11-11 and only `make build/main.bin` caught it, as 8 bytes.
    # LIVE_NAMES cannot see these: they are not defined in the live units.
    kw = {"if", "while", "for", "switch", "return", "sizeof", "do", "else"}
    named = sorted({c for c in re.findall(r"\b([A-Za-z_]\w*)\s*\(", new)
                    if not re.match(r"^(?:func|D)_[0-9A-F]{8}$", c)
                    and c not in kw and c != twin_fn})
    if named:
        print("    NAMED CALLEES, resolve each against the LISTING before you "
              "trust a measurement: %s" % ", ".join(named))

    print("%s  <-  %s  (%s)" % (twin_fn, live_fn, live_path.name))
    for a, b in sorted(used):
        print("    %-22s -> %s" % (a, b))
    if byalign:
        print("    from the ALIGNMENT, not a symmap vote: %s"
              % ", ".join(sorted(byalign)))
    if refused:
        print("    REFUSED, the map's vote is a tie -- rename by hand: %s"
              % ", ".join(sorted(refused)))
    if unmapped:
        print("    UNMAPPED, left as-is: %s" % ", ".join(unmapped))
    if dry:
        print(new)
        return
    src = TWIN.read_text()
    if STUB % twin_fn not in src:
        sys.exit("no INCLUDE_ASM for %s in %s" % (twin_fn, TWIN.name))
    TWIN.write_text(src.replace(STUB % twin_fn, new))
    missing = [b for _, b in sorted(used) if not re.search(r"\b" + b + r"\b", HDR.read_text())
               and not re.search(r"^[\w \*]+?\b" + b + r"\(", src, re.MULTILINE)]
    if missing:
        print("    DECLARE in %s: %s" % (HDR.name, ", ".join(missing)))
    print("    moved. NOW MEASURE: tools/decomp/ovlverify.py --G 8 %s %s"
          % (TWIN.relative_to(ROOT), twin_fn))


def do_patch(twin_fn, pairs):
    src = TWIN.read_text()
    sp = span(src, twin_fn)
    if sp is None:
        sys.exit("%s is not defined in %s (move it first)" % (twin_fn, TWIN.name))
    s, e = sp
    body = src[s:e]
    for spec in pairs:
        if "=>" not in spec:
            sys.exit("--patch wants OLD=>NEW, got %r" % spec)
        old, new = spec.split("=>", 1)
        if old not in body:
            sys.exit("%s: %r is not in the body" % (twin_fn, old))
        body = body.replace(old, new)
        print("    %s -> %s" % (old, new))
    TWIN.write_text(src[:s] + body + src[e:])
    print("    patched. NOW MEASURE: tools/decomp/ovlverify.py --G 8 %s %s"
          % (TWIN.relative_to(ROOT), twin_fn))


def do_back(twin_fn):
    src = TWIN.read_text()
    sp = span(src, twin_fn)
    if sp is None:
        sys.exit("%s is not defined in %s" % (twin_fn, TWIN.name))
    s, e = sp
    TWIN.write_text(src[:s] + STUB % twin_fn + src[e:])
    print("%s: backed out to INCLUDE_ASM" % twin_fn)


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("twin_fn")
    ap.add_argument("live_fn", nargs="?")
    ap.add_argument("--dry", action="store_true", help="print the renamed body, write nothing")
    ap.add_argument("--patch", nargs="+", metavar="OLD=>NEW",
                    help="replace literals inside an already-moved function")
    ap.add_argument("--back", action="store_true", help="restore its INCLUDE_ASM")
    ap.add_argument("--refresh", action="store_true", help="rebuild the cached symbol map")
    args = ap.parse_args()

    if args.refresh:
        symmap(refresh=True)
    if args.back:
        return do_back(args.twin_fn)
    if args.patch:
        return do_patch(args.twin_fn, args.patch)
    live_fn = args.live_fn
    if live_fn is None:
        live_fn, score = pairing(args.twin_fn)
        print("(pairing: similarity %.2f)" % score)
    do_move(args.twin_fn, live_fn, args.dry)


if __name__ == "__main__":
    main()
