#!/usr/bin/env python3
"""devmeasure.py FUNC... -- verify.py's measurement for a LANDED function.

`verify.py` scores a candidate against `asm/nonmatchings/<unit>/<func>.s`, and
a landed function has no such listing. That is why the device contract in
`docs/landing-a-function.md` used to say a device's word value could only be
measured while its function was a park -- and it is why four DEVICE comments
carried figures that had drifted with their unit's context for a month.

The measurement for a landed function is the built container against the
reference, localised to the one function through the linked elf's symbol
table. Edit `csrc/`, run this, and read the same `N of M words, D DIFF` that
`advise.py` prints for a park:

    python3 tools/decomp/devmeasure.py func_8003E758
    python3 tools/decomp/devmeasure.py --dis func_8004A574   # side by side

Three details, each of which gets a wrong answer rather than an error:

* `objcopy -O binary` lays sections out by **LMA**. The output offset of a
  vram address is `lma + (vram - vma)` for its section, NOT objdump's
  "File off", which is the ELF's own and is 0x10000 out on every container.
* `make`'s own recipe runs `tools/check.py` and **fails the target** when the
  bytes differ, so a nonzero exit code says nothing about whether the compile
  succeeded. We pass `-k` and treat the build as failed only when the binary
  came out OLDER than the sources.
* `nm -n` lists the `__asm__("...")` aliases (`func_8004437Cw`) at the same
  address as the function they rename, so a function's end has to be the next
  STRICTLY GREATER address. Taking the next row instead gives size 0, which
  scores as `0 of 0 words, 0 DIFF` -- a false MATCH.

The target's own word count comes from a **layout cache** under `build/`,
because once a device is removed the function's length is exactly what changes
and the current elf can no longer say what the length should have been. The
cache is refreshed on every run in which a container comes out byte-identical,
so a clean tree populates it and an edited tree scores against it. If it is
missing, build the container clean first (the tool says so).

WHAT THIS CANNOT SEE: it compares one function's words. As with
`ovlverify.py`, a clean run here is a reason to run `make check` and
`make containers`, not a substitute for them -- a C change can move a unit's
.rodata without touching a single instruction in the function.
"""
import argparse
import json
import os
import re
import subprocess
import sys
import tempfile

HERE = os.path.dirname(os.path.abspath(__file__))
PROJ = os.environ.get("AZURE_ROOT", os.path.dirname(os.path.dirname(HERE)))

CONTAINERS = [
    ("slus", "build/slus_006.14", "data/slus_006.14"),
    ("main", "build/main.bin", "data/main/main.bin"),
    ("town", "build/town.bin", "data/town/town.bin"),
    ("dungeon", "build/dungeon.bin", "data/dungeon/dungeon.bin"),
]
SECTION = re.compile(r"\s*\d+\s+(\S+)\s+([0-9a-f]+)\s+([0-9a-f]+)\s+"
                     r"([0-9a-f]+)\s+([0-9a-f]+)")


def run(args):
    return subprocess.run(args, capture_output=True, text=True, cwd=PROJ)


def text_symbols(elf):
    """[(vram, name)] for every text symbol, ascending."""
    out = run(["mips64-linux-gnu-nm", "-n", elf]).stdout
    return [(int(m.group(1), 16), m.group(2))
            for m in (re.match(r"^([0-9a-f]{8}) [tT] (\S+)", line)
                      for line in out.split("\n")) if m]


def loaded_sections(elf):
    """[(lma, lma_end, vma)] for every loaded section, so a vram maps to an
    output offset. LMA, not "File off" -- see the module docstring."""
    out = run(["mips64-linux-gnu-objdump", "-h", elf]).stdout
    segs = []
    for line in out.split("\n"):
        m = SECTION.match(line)
        if not m:
            continue
        size, vma, lma = (int(m.group(2), 16), int(m.group(3), 16),
                          int(m.group(4), 16))
        if size and vma >= 0x80000000:
            segs.append((lma, lma + size, vma))
    return segs


def offset_of(vram, segs):
    for lma, lma_end, vma in segs:
        if vma <= vram < vma + (lma_end - lma):
            return lma + (vram - vma)
    return None


def extents(elf):
    """{name: (start, end)}, end being the next STRICTLY GREATER address."""
    sy = text_symbols(elf)
    out = {}
    for i, (addr, name) in enumerate(sy):
        nxt = next((a for a, _ in sy[i + 1:] if a > addr), None)
        out[name] = (addr, nxt)
    return out


def cache_path(key):
    return os.path.join(PROJ, "build", "%s.symlayout.json" % key)


def build(targets):
    """Build with -k; report which targets did not come out."""
    csrc = os.path.join(PROJ, "csrc")
    newest = max(os.path.getmtime(os.path.join(csrc, f))
                 for f in os.listdir(csrc) if f.endswith(".c"))
    r = run(["make", "-k"] + targets)
    stale = []
    for t in targets:
        p = os.path.join(PROJ, t)
        if not os.path.exists(p) or os.path.getmtime(p) < newest:
            stale.append(t)
    return stale, r.stdout + r.stderr


def refresh_cache(key, built, ref, elf):
    """Record the layout whenever this container is byte-identical, which is
    the only state in which the elf's own symbol table IS the target's."""
    if open(built, "rb").read() != open(ref, "rb").read():
        return False
    layout = {n: [s, e] for n, (s, e) in extents(elf).items() if e}
    with open(cache_path(key), "w") as fh:
        json.dump(layout, fh)
    return True


def locate(func):
    """The container whose elf defines `func`."""
    for key, built, ref in CONTAINERS:
        elf = os.path.join(PROJ, built + ".elf")
        if os.path.exists(elf) and func in extents(elf):
            return key, built, ref
    return None, None, None


def words(path, off, size):
    data = open(os.path.join(PROJ, path), "rb").read()[off:off + size]
    return [data[i:i + 4] for i in range(0, len(data) - 3, 4)]


def score(func, verbose=False):
    key, built, ref = locate(func)
    if key is None or built is None or ref is None:
        print("%s: in no container's elf -- build first, and check the name"
              % func)
        return 2
    stale, log = build([built])
    if stale:
        print("%s: BUILD FAILED" % func)
        print("\n".join(line for line in log.split("\n")
                        if "error" in line.lower())[:2000])
        return 2
    elf = os.path.join(PROJ, built + ".elf")
    refresh_cache(key, os.path.join(PROJ, built), os.path.join(PROJ, ref), elf)
    cache = cache_path(key)
    if not os.path.exists(cache):
        print("%s: no layout cache for %s. Build it byte-identical once "
              "(stash your edit, `make %s`) and re-run." % (func, key, built))
        return 2
    layout = json.load(open(cache))
    if func not in layout:
        print("%s: not in the %s layout cache; it is newer than the cache, so "
              "rebuild the cache clean." % (func, key))
        return 2
    segs = loaded_sections(elf)
    t_start, t_end = layout[func]
    b_start, b_end = extents(elf)[func]
    off_t = offset_of(t_start, segs)
    off_b = offset_of(b_start, segs)
    if off_t is None or off_b is None:
        print("%s: outside every loaded section" % func)
        return 2
    tgt = words(ref, off_t, t_end - t_start)
    got = words(built, off_b, (b_end or b_start) - b_start)
    diff = sum(1 for a, b in zip(got, tgt) if a != b) + abs(len(got) - len(tgt))
    print("%-16s %3d of %3d words, %3d DIFF%s"
          % (func, len(got), len(tgt), diff, "   MATCH" if diff == 0 else ""))
    return 0 if diff == 0 else 1


def disassemble(data, vram):
    fd, tmp = tempfile.mkstemp()
    os.write(fd, data)
    os.close(fd)
    out = run(["mips64-linux-gnu-objdump", "-D", "-b", "binary", "-m",
               "mips:3000", "-EL", "--adjust-vma", hex(vram), tmp]).stdout
    os.unlink(tmp)
    rows = []
    for line in out.split("\n"):
        m = re.match(r"\s*([0-9a-f]+):\t[0-9a-f ]+\t(.*)", line)
        if m:
            rows.append(re.sub(r"\s+", " ", m.group(2).strip()))
    return rows


def show(func):
    if score(func) == 2:
        return 2
    key, built, ref = locate(func)
    if key is None or built is None or ref is None:
        return 2
    elf = os.path.join(PROJ, built + ".elf")
    layout = json.load(open(cache_path(key)))
    segs = loaded_sections(elf)
    t_start, t_end = layout[func]
    b_start, b_end = extents(elf)[func]
    off_b = offset_of(b_start, segs) or 0
    off_t = offset_of(t_start, segs) or 0
    a = disassemble(open(os.path.join(PROJ, built), "rb").read()
                    [off_b:][:(b_end or b_start) - b_start], b_start)
    b = disassemble(open(os.path.join(PROJ, ref), "rb").read()
                    [off_t:][:t_end - t_start], t_start)
    print("    %-34s   %s" % ("built", "reference"))
    for i in range(max(len(a), len(b))):
        x = a[i] if i < len(a) else ""
        y = b[i] if i < len(b) else ""
        print("%4d %-34s %s %s" % (i, x, "|" if x != y else " ", y))
    return 0


def main():
    ap = argparse.ArgumentParser(description=__doc__.split("\n")[0])
    ap.add_argument("funcs", nargs="+")
    ap.add_argument("--dis", action="store_true",
                    help="side-by-side disassembly as well as the score")
    args = ap.parse_args()
    rc = 0
    for func in args.funcs:
        rc = max(rc, show(func) if args.dis else score(func))
    return rc


if __name__ == "__main__":
    sys.exit(main())
