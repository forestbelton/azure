#!/usr/bin/env python3
r"""gaudit.py -- two addressing checks that `verify.py` structurally cannot do.

CHECK 1: the carved -G8/-G2 units, for symbols that would come out
$gp-relative where the target reaches them absolutely.

**THIS CHECK IS VACUOUS AS OF 2026-11-03** and is kept against the day a unit
needs a nonzero -G again. MASPSXFLAGS_G8 and MASPSXFLAGS_G2 were deleted from
the Makefile when all eleven carved gprel_* units were converted to file-scope
definitions of their $gp objects; at -G0 gas makes no small-data decision of
its own, so there is nothing for the declared size to get wrong. The two units
the check still reports are the overlays, built with MASPSXFLAGS_G0, where
(0, G] is empty. CHECK 2 below is the live one.

The mechanism, from docs/addressing-and-translation-units.md: gas emits a
GPREL16 relocation for any symbol whose `.extern SYM,size` record gives a size
in (0, G], and cc1psx emits that record from the C declaration. **gas never
sees the symbol's address** -- the symbol is undefined at assembly time -- so
the declared SIZE is the whole of the decision. A unit built at -G8 therefore
turned every <=8-byte extern it named into a $gp-relative access, including
ones the original reached absolutely -- which is what this check looked for
while the MASPSXFLAGS_G8 escape hatch existed.

So for each function still left as assembly in a carved unit, this finds the
symbols it reaches through a %hi/%lo pair (i.e. absolutely, in the original)
and reports any that include/structs.h declares as a scalar of <= G bytes. The
fix is always to widen the declaration past -G -- a real struct, or an
incomplete array -- so cc1psx compiler-splits the address itself.

A symbol OUTSIDE gp +/- 0x8000 is the worse case, not the safer one: it cannot
have been $gp-relative in the original, and the GPREL16 field cannot even hold
its offset. An earlier version of this rule had that backwards.

This only looks at functions still in assembly. A landed function's addressing
is already proven by `make check`, which is the real authority.

CHECK 2: the file-scope DEFINITIONS of small globals -- `u32 D_80080ADC;` and
the like, plus the two surviving `__asm__(".sdata\n\t.size SYM,N\n\t.text")`
records, in csrc/unk_264E0.c and csrc/gprel_17B7C.c (each needed because its
symbol has an ADDEND access, which maspsx refuses to make $gp-relative for a
`.comm`). Either one puts the symbol in a maspsx table, and
that is FILE-SCOPE: maspsx then rewrites every access to that symbol in the
translation unit, so a unit cannot have the symbol $gp-relative in one function
and absolute in another -- and if a second function in the unit wanted it
absolute, its accesses are silently rewritten and that function loses a word
per site. That is how func_8003E34C and func_8003E39C were briefly left in one
unit: func_8003E39C lost 5 words, the whole image shifted by -0x14 and the
jump-table contents changed. `make check` caught it; `verify.py` could not,
because it compiles one function at a time.

So for each such symbol, this reports every OTHER function in the same unit
that mentions it. Each one is a site to check by hand -- some will
genuinely want $gp-relative addressing too, so the report is a prompt rather
than a verdict. It reads the pre-maspsx compiler output under build/csrc/, so
it needs a build; it says so rather than guessing if one is missing.
"""
import glob
import os
import re
import sys

PROJ = os.environ.get(
    "AZURE_ROOT",
    os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))),
)

SIZES = {"u8": 1, "s8": 1, "u16": 2, "s16": 2, "u32": 4, "s32": 4, "f32": 4}
GP = 0x80080994

SDATA_RE = re.compile(r'__asm__\s*\(\s*"\.sdata(.*?)\.text"\s*\)', re.S)
SIZE_RE = re.compile(r"\.size\s+(D_[0-9A-Fa-f]+)\s*,\s*(\d+)")
# A file-scope tentative definition: `u32 D_80080ADC;`, `u8 D_800814D0[4];`,
# `u8 *D_80081540;`. Anchored at column 0, and `extern` excluded, so a local
# or a header-style declaration cannot match.
DEF_RE = re.compile(
    r"^(?!extern\b)([A-Za-z_]\w*(?:\s+[A-Za-z_]\w*)*\s*\**)\s*"
    r"(D_[0-9A-Fa-f]+)\s*(\[[^\]]*\])?\s*;", re.M)


def carved_units(makefile):
    """{unit: G} for every unit given a nonzero -G in the Makefile."""
    out = {}
    pat = r"((?:build/csrc/\S+\.o[ \\\n]*)+):\s*MASPSXFLAGS\s*:=\s*\$\((MASPSXFLAGS_G[0-9]+)\)"
    for block, flag in re.findall(pat, makefile):
        g = int(flag.rsplit("_G", 1)[1])
        for m in re.finditer(r"build/csrc/(\w+)\.o", block):
            # the Makefile's own usage example names a placeholder unit
            if m.group(1) != "some_unit":
                out[m.group(1)] = g
    return out


def scalar_decls(header):
    """{symbol: type} for every extern declared as a scalar (<= 4 bytes)."""
    return {
        m.group(2): m.group(1)
        for m in re.finditer(
            r"^extern\s+(u8|s8|u16|s16|u32|s32|f32)\s+(D_[0-9A-Fa-f]+)\s*;",
            header,
            re.M,
        )
    }


def check_carved_units():
    """CHECK 1 -- see the module docstring."""
    mk = open(os.path.join(PROJ, "Makefile")).read()
    hdr = open(os.path.join(PROJ, "include", "structs.h")).read()
    units = carved_units(mk)
    scal = scalar_decls(hdr)

    problems = 0
    for unit, g in sorted(units.items()):
        listings = sorted(glob.glob(os.path.join(PROJ, "asm/nonmatchings", unit, "*.s")))
        rows = []
        for path in listings:
            text = open(path).read()
            for sym in sorted(set(re.findall(r"%(?:hi|lo)\((D_[0-9A-Fa-f]+)\)", text))):
                typ = scal.get(sym)
                if typ is None or SIZES[typ] > g:
                    continue
                off = int(sym[2:], 16) - GP
                where = (
                    "outside gp window (%+#x): GPREL16 cannot hold it either" % off
                    if abs(off) >= 0x8000
                    else "inside gp window (%+#x)" % off
                )
                rows.append((sym, typ, os.path.basename(path)[:-2], where))
        if rows:
            problems += len(rows)
            print("%s (-G%d), %d function(s) still asm:" % (unit, g, len(listings)))
            for sym, typ, fn, where in rows:
                print("  %-14s declared %-3s (%d byte%s) -- reached absolutely by %s"
                      % (sym, typ, SIZES[typ], "" if SIZES[typ] == 1 else "s", fn))
                print("  %-14s %s" % ("", where))
                print("  %-14s FIX: declare it past -G%d so cc1psx splits the address"
                      % ("", g))
            print()

    print("%d carved unit(s), %d scalar extern(s) declared, %d problem(s)"
          % (len(units), len(scal), problems))
    return problems


def gp_definitions():
    """{unit: {symbol: size}} for every file-scope small-global definition."""
    out = {}
    for path in sorted(glob.glob(os.path.join(PROJ, "csrc", "*.c"))):
        txt = open(path).read()
        syms = {}
        for ty, sym, arr in DEF_RE.findall(txt):
            n = SIZES.get(ty.strip().rstrip("*").strip(), 4)
            if arr.strip("[]").isdigit():
                n *= int(arr.strip("[]"))
            syms[sym] = n
        for block in SDATA_RE.findall(txt):
            for sym, size in SIZE_RE.findall(block):
                syms[sym] = int(size)
        if syms:
            out[os.path.basename(path)[:-2]] = syms
    return out


def functions_mentioning(unit, syms):
    """{symbol: [function, ...]} from the unit's pre-maspsx compiler output."""
    path = os.path.join(PROJ, "build", "csrc", unit + ".s")
    if not os.path.exists(path):
        return None
    hits = {s: [] for s in syms}
    current = None
    for line in open(path, errors="replace"):
        m = re.match(r"\s*\.ent\s+(\S+)", line)
        if m:
            current = m.group(1)
            continue
        if re.match(r"\s*\.end\b", line):
            current = None
            continue
        if current is None:
            continue
        for s in syms:
            # whole-word, so D_800814D0 does not match D_800814D00
            if re.search(r"\b%s\b" % re.escape(s), line) and current not in hits[s]:
                hits[s].append(current)
    return hits


def check_gp_definitions():
    """CHECK 2 -- see the module docstring."""
    records = gp_definitions()
    if not records:
        return 0
    print()
    problems = 0
    missing = []
    for unit, syms in sorted(records.items()):
        hits = functions_mentioning(unit, syms)
        if hits is None:
            missing.append(unit)
            continue
        for sym, fns in sorted(hits.items()):
            if len(fns) > 1:
                problems += 1
                print("%s: defines %s, but %d functions mention it:"
                      % (unit, sym, len(fns)))
                for fn in fns:
                    print("    %s" % fn)
                print("    a definition is FILE-SCOPE -- check that every one of those")
                print("    wants %s $gp-relative, or carve them apart" % sym)
                print()
    if missing:
        print("no build/csrc/*.s for %s -- run make, then re-run this check"
              % ", ".join(missing))
    nsyms = sum(len(s) for s in records.values())
    print("%d small-global definition(s) over %d unit(s), %d to check by hand"
          % (nsyms, len(records), problems))
    return problems


def main():
    return 1 if (check_carved_units() + check_gp_definitions()) else 0


if __name__ == "__main__":
    sys.exit(main())
