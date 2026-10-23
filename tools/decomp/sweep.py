#!/usr/bin/env python3
"""Batch-verify candidate decompilations for the carved units, then land them.

The carved units (unk_72A8, unk_1C540, unk_1AF38, unk_301D4) hold hundreds of
small functions, so they are worked in batches rather than one at a time. See
docs/carved-sweep.md for the source patterns that keep paying and the traps
that have cost time.

A batch file defines CANDS as a list of (name, unit, body) triples:

    CANDS = [
      ("func_80033C84", "unk_72A8", "void func_80033C84(u32 *p) {\\n    *p = 0;\\n}"),
    ]

    python3 tools/decomp/sweep.py verify batch.py   # -> prints each result,
                                                    #    writes batch_ok.py
    python3 tools/decomp/sweep.py land  batch_ok.py # -> edits csrc/<unit>.c
    make check                                      # the only authority

`verify` writes ONE candidate per scratch file, because verify.py treats the
whole .text of the file as the function: a file holding several candidates
reports nonsense (a two-word function came back "8 DIFF").

`land` refuses to touch a unit unless every stub it needs is present exactly
once, AND refuses to land a definition when the unit already carries an inline
prototype for that name. That second check exists because the collision broke
the build three times: declaring a callee the sweep had already defined,
landing a definition for a callee already declared inline, and both at once.
"""
import io, os, re, runpy, subprocess, sys

# Units whose objects are assembled at -G8 (Makefile's MASPSXFLAGS_G8 list).
G8 = {"gprel_10308", "gprel_11CFC", "gprel_12A40", "gprel_12DAC", "gprel_12DD4",
      "gprel_148FC", "gprel_17B7C", "gprel_18B40", "gprel_1AB38"}

PROTO = r'^[A-Za-z_][\w \*]*?\b%s\s*\([^;{]*\);\s*$'


def verify(path):
    os.environ.setdefault("AZURE_WINEPREFIX", "/tmp/azure-wine-reuse")
    cands = runpy.run_path(path)["CANDS"]
    ok = []
    for name, unit, body in cands:
        io.open("zzone.c", "w").write('#include "common.h"\n\n%s\n' % body)
        cmd = ["python3", "tools/decomp/verify.py", "zzone.c", unit, name]
        if unit in G8:
            cmd.append("--g8")
        r = subprocess.run(cmd, capture_output=True, text=True)
        m = re.search(r"RESULT: (MATCH|\d+ DIFF)", r.stdout + r.stderr)
        res = m.group(1) if m else "ERROR"
        if res == "MATCH":
            ok.append((name, unit, body))
        print("%-16s %-10s %s" % (name, unit, res))
    print("\n%d/%d MATCH" % (len(ok), len(cands)))
    out = path.replace(".py", "_ok.py")
    io.open(out, "w").write("CANDS = %r\n" % (ok,))
    print("wrote %s" % out)


def land(path):
    byunit = {}
    for name, unit, body in runpy.run_path(path)["CANDS"]:
        byunit.setdefault(unit, []).append((name, body))
    for unit, items in byunit.items():
        p = "csrc/%s.c" % unit
        t = io.open(p, encoding="utf-8").read()
        for name, _ in items:
            stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (unit, name)
            if t.count(stub) != 1:
                sys.exit("ABORT %s: %s stub appears %d times" % (p, name, t.count(stub)))
            m = re.search(PROTO % name, t, re.M)
            if m:
                sys.exit("ABORT %s: %s already has an inline prototype (%r). "
                         "Reconcile it with the definition before landing."
                         % (p, name, m.group(0).strip()))
        for name, body in items:
            stub = 'INCLUDE_ASM("asm/nonmatchings/%s", %s);' % (unit, name)
            t = t.replace(stub, body)
        io.open(p, "w", encoding="utf-8").write(t)
        print("landed %d in %s" % (len(items), p))
    print("now run: make check")


if __name__ == "__main__":
    if len(sys.argv) != 3 or sys.argv[1] not in ("verify", "land"):
        sys.exit(__doc__)
    (verify if sys.argv[1] == "verify" else land)(sys.argv[2])
