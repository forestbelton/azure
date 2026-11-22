"""partsweep.py FUNC [--body PATH] [--max N] -- sweep PARTITIONS of the locals
that hold the same expression, which is the one space no other sweep varies.

    python3 tools/decomp/partsweep.py func_800C2824

WHY THIS EXISTS. On 2026-11-22 three dungeon parks were moved a long way and
TWO OF THE THREE BIGGEST SINGLE CHANGES WERE PARTITION MOVES -- a decision
about WHICH LOCALS SHARE ONE NAME, not about any local's type, position,
reference count or device:

    func_800C2824  27 -> 25  `r16' deleted, merged into the shared `r'
    func_800C2824  13 -> 10  case 3's `*(u8 **)p' split out of `e3'
    func_800B51C8  24 -> 16  one `UnkDungeonSpotB *r' deleted, subscript
                             written at every site

and in EVERY case the park already held a NEGATIVE for that move, measured on
an older body. refsweep varies cancelled pairs, devsweep wrappers, stmtsweep
statement positions, widthsweep declared widths. None of them touches this,
so the only way it has ever been searched is by hand, which is why the stale
negatives survived three sessions.

WHAT THE SPACE IS. A C variable is one pseudo. A pseudo live in more than one
basic block is a GLOBAL allocno (flow.c:2561) that `find_reg' allocates from
the conflicts of its WHOLE range; one confined to a single block goes to
local-alloc's `find_free_reg' and, because MIPS defines no REG_ALLOC_ORDER,
takes the LOWEST FREE register. So merging two sites and splitting them are
not cosmetic -- they move a quantity between two different allocators. That is
the whole of func_800C2824's $v0-against-$a1 and func_800B6AC4's $t0-against-
$v1.

HOW THE SITES ARE FOUND. A site is (region, local): a region is a `case N:'
arm, or the whole body when there is no switch, and a local is one assigned
there. Locals are grouped by the EXPRESSION assigned to them, normalised for
whitespace, so only sites that genuinely hold the same value are ever merged.
Sites holding different expressions are never put in one group, because that
would change what the program computes.

WHAT THIS TOOL DELIBERATELY DOES NOT DO, AND WHY. It only ever merges sites
that hold the SAME EXPRESSION. That is a conservative subspace, and it is NOT
where func_800C2824's two big partition wins came from: `r16' held
`*(u8 **)(e3 - 0x14)' and was merged into `r', which holds something else
entirely in cases 2 and 17. Merging locals that hold DIFFERENT values is legal
only when their live ranges are provably disjoint, and this tool cannot prove
that -- so it does not try.

That line is drawn on purpose. On this very function five permuter finds and
one sweep find have been rejected for reading a local that was never assigned
on the path reaching it, twice with the read passed to a callee as a pointer.
A tool that silently proposes a semantically wrong body is worse than no tool,
because its candidates measure well and have to be caught by hand. Cross-value
merges therefore stay with the permuter, where gate 2 ("read what it MEANS")
is already part of the protocol.

What this tool IS for: making the same-expression subspace CHEAP TO RE-CHECK
after every structural change, which is the actual failure mode -- the parks
did not lack a search of this space, they held results from an older body. A
run is seconds, so re-run it rather than trusting a recorded line.

A safe extension, not implemented: a site whose region ASSIGNS the local
before any use in that region and whose uses never leave it is self-contained,
and two such sites can be merged whatever they hold. That criterion is
checkable and would reach the cross-value merges. It is left undone rather
than done approximately.

THE OBJECTIVE IS (WORD GAP, ROWS), as in every other sweep -- never a gate on
`ww == tw'. See refsweep.py for what that gate broke and, just as important,
what it did not.

DANGER, MEASURED: A PARTITION'S VALUE IS SCOPED TO THE STATEMENT ORDER IT WAS
MEASURED IN. On func_800B6AC4 "share these two locals" was correct in one
statement-order basin and worth -2 rows in the other, and on func_800C2824 a
separate local was neutral at 9 rows and cost 2 rows at 10. So re-run this
after any structural change; a partition result is not a fact about the
function. docs/matching-levers.md, "A STALE POSITIVE IS AS DANGEROUS AS A
STALE NEGATIVE".

The number of partitions of n sites is the Bell number: 1, 2, 5, 15, 52, 203,
877, 4140, 21147. --max caps it (default 1000) and the tool says how many it
skipped rather than running for an hour silently.
"""
import os
import re
import subprocess
import sys

HERE = os.path.dirname(os.path.abspath(__file__))
sys.path.insert(0, HERE)

# LINE-BUFFER STDOUT so a run killed by `timeout' still leaves its finds.
try:
    sys.stdout.reconfigure(line_buffering=True)
except Exception:
    pass

from onefunc import find_definition
import parkresidue as pr

TAG = "partsweep"


def regions(fn):
    """[(name, start, end)] -- the `case N:' arms, or the whole body."""
    hits = [(m.group(1), m.start()) for m in
            re.finditer(r'^\s*case\s+(\w+):', fn, re.M)]
    if not hits:
        return [("body", 0, len(fn))]
    out = []
    for i, (name, s) in enumerate(hits):
        e = hits[i + 1][1] if i + 1 < len(hits) else len(fn)
        out.append(("case " + name, s, e))
    return out


def sites(fn):
    """{normalised RHS: [(region, local, n_uses)]} for locals assigned it."""
    # The NAMES of the locals. (The first version built this with
    # dict(re.findall(...)) on a two-group pattern, which maps TYPE -> NAME and
    # made every lookup below fail, so the tool reported "nothing to partition"
    # on a body with six partitionable sites. A negative from a broken detector
    # is exactly the stale negative this tool exists to kill.)
    #
    # The type is any identifier, not just the six integer widths: this block's
    # parks declare `UnkDungeonSpotB *r;' and `Unk80083160 *g;' and those are
    # the pointers most worth partitioning.
    decls = set(m.group(2) for m in re.finditer(
        r'^\s*[A-Za-z_]\w*\s*\**\s*()([A-Za-z_]\w*)\s*;\s*$', fn, re.M))
    groups = {}
    for rname, rs, re_ in regions(fn):
        seg = fn[rs:re_]
        for m in re.finditer(r'^\s*([A-Za-z_]\w*)\s*=\s*([^;]+);\s*$', seg, re.M):
            name, rhs = m.group(1), " ".join(m.group(2).split())
            if name not in decls:
                continue
            uses = len(re.findall(r'\b%s\b' % re.escape(name), seg))
            groups.setdefault(rhs, []).append((rname, name, uses))
    return {k: v for k, v in groups.items() if len(v) > 1}


def partitions(items):
    if not items:
        yield []
        return
    first, rest = items[0], items[1:]
    for p in partitions(rest):
        for i in range(len(p)):
            yield p[:i] + [[first] + p[i]] + p[i + 1:]
        yield [[first]] + p


def main():
    args = [a for a in sys.argv[1:] if not a.startswith("--")]
    if not args:
        raise SystemExit((__doc__ or "").strip().split("\n\n")[0])
    func = args[0]
    cap = 1000
    for i, a in enumerate(sys.argv):
        if a == "--max":
            cap = int(sys.argv[i + 1])

    scratch = os.environ.get("AZURE_SCRATCH") or "/tmp"
    sp = os.path.join(scratch, "partsweep", func)
    os.makedirs(sp, exist_ok=True)
    unit_path, unit = pr.unit_of(func)
    dst = os.path.join(sp, os.path.basename(unit_path))
    subprocess.run(["cp", unit_path, dst], check=True)
    r = subprocess.run([sys.executable, os.path.join(HERE, "parksplice.py"),
                        func, dst], capture_output=True, text=True)
    if "spliced" not in r.stdout:
        raise SystemExit("%s: parksplice failed:\n%s%s" % (TAG, r.stdout, r.stderr))

    base_txt = open(dst).read()
    st, en = find_definition(base_txt, func)
    head, fn, tail = base_txt[:st], base_txt[st:en], base_txt[en:]

    for i, a in enumerate(sys.argv):
        if a == "--body":
            t2 = open(sys.argv[i + 1]).read()
            s2 = find_definition(t2, func)
            if not s2:
                raise SystemExit("%s: no definition of %s in %s"
                                 % (TAG, func, sys.argv[i + 1]))
            fn = t2[s2[0]:s2[1]]
            base_txt = head + fn + tail
            open(dst, "w").write(base_txt)

    def measure_on(path, pre, post):
        def go(body):
            open(path, "w").write(pre + body + post)
            rr = subprocess.run([sys.executable, os.path.join(HERE, "aligndiff.py"),
                                 path, func], capture_output=True, text=True)
            m = re.match(r"built (\d+) words, target (\d+) words, (\d+)",
                         (rr.stdout.strip() or "").split("\n")[0])
            return (int(m.group(3)), int(m.group(1)), int(m.group(2))) if m else (999, 0, 1)
        return go

    measure_unit = measure_on(dst, head, tail)
    rows, w, tw = measure_unit(fn)

    # See refsweep.py for why the threshold is 800 and not 8000.
    measure = measure_unit
    if len(base_txt.split("\n")) > 800:
        subprocess.run([sys.executable, os.path.join(HERE, "onefunc.py"), func,
                        "--src", dst, "--out", os.path.join(sp, "one")],
                       capture_output=True, text=True)
        one = os.path.join(sp, "one", os.path.basename(unit_path))
        if os.path.exists(one):
            otxt = open(one).read()
            ost, oen = find_definition(otxt, func)
            m1 = measure_on(one, otxt[:ost], otxt[oen:])
            orows, ow, _ = m1(fn)
            if (orows, ow) == (rows, w):
                print("%s: standalone agrees (%d rows, %d words) -- sweeping there"
                      % (func, orows, ow))
                measure = m1
            else:
                print("%s: standalone measures %d/%d against the unit's %d/%d"
                      " -- NOT using it" % (func, orows, ow, rows, w))
    print("%s: base %d rows, %d/%d words" % (func, rows, w, tw))

    def key(r, ww):
        return (abs(ww - tw), r)

    best_key, best, found = key(rows, w), (rows, w), None
    grouped = sites(fn)
    if not grouped:
        print("%s: no two locals hold the same expression -- nothing to partition" % TAG)
        return
    for rhs, members in sorted(grouped.items(), key=lambda kv: -len(kv[1])):
        names = sorted({m[1] for m in members})
        if len(names) < 2:
            continue
        parts = list(partitions(names))
        if len(parts) > cap:
            print("%s: `%s' has %d names -> %d partitions, over --max %d; SKIPPED"
                  % (TAG, rhs[:40], len(names), len(parts), cap))
            continue
        print("%s: `%s' -- %d names %s, %d partitions"
              % (TAG, rhs[:50], len(names), names, len(parts)))
        for part in parts:
            cand = fn
            dropped = []
            for group in part:
                keep = group[0]
                for other in group[1:]:
                    cand = re.sub(r'\b%s\b' % re.escape(other), keep, cand)
                    dropped.append(other)
            # remove the declarations of the names that no longer appear
            for d in dropped:
                cand = re.sub(r'^\s*(?:u8|s8|u16|s16|u32|s32|void)\s*\**\s*'
                              + re.escape(d) + r';\s*\n', '', cand, flags=re.M)
            if cand == fn:
                continue
            rr, ww, _ = measure(cand)
            if key(rr, ww) < best_key:
                best_key, best = key(rr, ww), (rr, ww)
                found = (cand, " | ".join("+".join(g) for g in part))
                print("  %d rows, %d words  %s" % (rr, ww, found[1]))
    if found:
        out = os.path.join(sp, "best.c")
        open(out, "w").write(found[0])
        print("final %d rows, %d/%d words -- %s" % (best[0], best[1], tw, found[1]))
        print("saved: " + out)
    else:
        print("final %d rows, %d/%d words" % (best[0], best[1], tw))
        print("%s: no partition helps" % TAG)


if __name__ == "__main__":
    main()
