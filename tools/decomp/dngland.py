#!/usr/bin/env python3
"""Integrate one agent's landed DUNGEON.BIN body, then clone it to its family.

The sweep's agents work in a PRIVATE COPY of their unit and never touch
`csrc/` or run a build -- `make` re-runs splat, which deletes the listings
every other agent is measuring against. So a landing arrives as a modified
copy, and this is the dispatcher's side of that handover:

  1. lift the definition (with its comment) and the new declarations out of
     the agent's copy,
  2. splice them into the real `csrc/<unit>.c` and verify THERE, because the
     agent's copy can carry other edits and the verifier's answer is only
     about the file it was given,
  3. hand the result to `dngclone.py`, which writes the same body into every
     other member of the function's family and verifies each one.

Nothing is written unless step 2 comes back `0 differing`. The build gate --
`make check` and `make containers`, four `OK  byte-identical` lines -- is
still the caller's and is still the only thing that proves a unit's .rodata
and ordering survived. A clean run here is a reason to build, not a
substitute for building.

Usage:
    python3 tools/decomp/dngland.py FUNC --from AGENT_COPY.c [--dry-run]
                                         [--no-clone] [-j N]
"""
import argparse
import os
import re
import subprocess
import sys
import tempfile

sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import dngclone  # noqa: E402

PROJ = dngclone.PROJ


def verify(path, func):
    env = dict(os.environ, AZURE_SCRATCH=os.path.join(
        tempfile.gettempdir(), "dngland-sp-%d" % os.getpid()))
    r = subprocess.run(
        [sys.executable, os.path.join(PROJ, "tools", "decomp", "ovlverify.py"),
         path, func], capture_output=True, text=True, env=env, cwd=PROJ)
    # The WHOLE stderr on failure: a truncated traceback shows only `main()`,
    # and the useful line of a Python traceback is its last one.
    line = (r.stdout.splitlines()
            or [r.stderr.strip().replace("\n", " | ")])[0]
    ok = " 0 differing " in line and re.search(
        r"target (\d+) words, candidate \1 words", line) is not None
    return ok, line.strip()


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("func")
    ap.add_argument("--from", dest="src", required=True,
                    help="the agent's copy of the unit")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--no-clone", action="store_true")
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    func = args.func
    agent_text = open(args.src).read()
    body, _ = dngclone.definition(agent_text, func)
    if body is None:
        raise SystemExit(f"{func} is not defined in {args.src}")

    dest = dngclone.unit_path(func)
    text = open(dest).read()
    have = set(dngclone.decl_block(text))
    # needed_decls: the agent's copy carries the whole unit's declaration
    # block, most of it from earlier families. reconcile_decls: a declaration
    # of something this unit already DEFINES takes its prototype from that
    # definition, so two agents' different parameter types for one callee
    # cannot collide. Both are dngclone's, for the same reasons.
    want = dngclone.reconcile_decls(
        dngclone.needed_decls(dngclone.decl_block(agent_text), body), text)
    add = [d for d in want if d not in have]
    new = text
    if add:
        m = re.search(r'#include "common\.h"\n', new)
        at = m.end() if m else 0
        new = new[:at] + "\n" + "\n".join(add) + "\n" + new[at:]
    new = dngclone.splice(new, func, body)

    with tempfile.TemporaryDirectory(prefix="dngland-") as td:
        probe = os.path.join(td, os.path.basename(dest))
        open(probe, "w").write(new)
        ok, line = verify(probe, func)
    print(f"{'OK  ' if ok else 'FAIL'} {line}")
    if not ok:
        raise SystemExit(f"{func} does not verify against {dest} -- not written")
    print(f"     {len(add)} new declaration(s): {'; '.join(add) or 'none'}")
    if args.dry_run:
        print("     (dry run, nothing written)")
        return
    open(dest, "w").write(new)
    print(f"     wrote {os.path.relpath(dest, PROJ)}")

    if args.no_clone:
        return
    r = subprocess.run(
        [sys.executable, os.path.join(PROJ, "tools", "decomp", "dngclone.py"),
         func, "-j", str(args.j)], cwd=PROJ)
    sys.exit(r.returncode)


if __name__ == "__main__":
    main()
