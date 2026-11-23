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

An agent now lands SEVERAL functions in one copy of one unit, so this takes
several names. They are spliced together and verified in ONE compile -- the
verifier compiles the unit once for any number of names -- and the unit's
other landed functions are re-verified in the same pass. If one of the
requested functions fails, it is DROPPED and the rest are retried, because a
partial landing of four good bodies is worth banking and the fifth's residue
is a separate problem. Every function actually written is reported by name.

Usage:
    python3 tools/decomp/dngland.py FUNC... --from AGENT_COPY.c [--dry-run]
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


def verify_many(path, funcs):
    """(all ok, [result line], stderr) for several functions in one compile."""
    env = dict(os.environ, AZURE_SCRATCH=os.path.join(
        tempfile.gettempdir(), "dngland-sp-%d" % os.getpid()))
    r = subprocess.run(
        [sys.executable, os.path.join(PROJ, "tools", "decomp", "ovlverify.py"),
         path] + funcs, capture_output=True, text=True, env=env, cwd=PROJ)
    lines = [l.strip() for l in r.stdout.splitlines()
             if re.match(r"^func_\S+: ", l)]
    ok = all(" 0 differing " in l for l in lines) and len(lines) == len(funcs)
    return ok, lines, r.stderr


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


def build(agent_text, funcs, text):
    """(candidate text, [new decls]) with every func in `funcs` spliced in."""
    bodies = {}
    for f in funcs:
        body, _ = dngclone.definition(agent_text, f)
        if body is None:
            raise SystemExit(f"{f} is not defined in the agent copy")
        bodies[f] = body
    have = set(dngclone.decl_block(text))
    # needed_decls is per-body and the agent's copy carries the whole unit's
    # block, most of it from earlier families; scanning the CONCATENATED
    # bodies is the union over them. reconcile_decls: a declaration of
    # something this unit already DEFINES takes its prototype from that
    # definition, so two agents' different parameter types for one callee
    # cannot collide. Both are dngclone's, for the same reasons.
    want = dngclone.reconcile_decls(
        dngclone.needed_decls(dngclone.decl_block(agent_text),
                              "\n".join(bodies.values())), text)
    add = [d for d in want if d not in have]
    new = dngclone.insert_decls(text, want)
    for f in funcs:
        new = dngclone.splice(new, f, bodies[f])
        # A stale forward declaration of this very function, left by another
        # agent that only took its address, is a `conflicting types' error
        # once the definition arrives. See fix_forward_decl.
        new = dngclone.fix_forward_decl(new, f)
    return new, add


def probe(new, dest, funcs):
    """(candidate text's verdict lines) for funcs + every other definition."""
    others = [f for f in dngclone.defined_funcs(new) if f not in funcs]
    with tempfile.TemporaryDirectory(prefix="dngland-") as td:
        path = os.path.join(td, os.path.basename(dest))
        open(path, "w").write(new)
        _, lines, err = verify_many(path, list(funcs) + others)
    want = {}
    for l in lines:
        want[l.split(":")[0]] = l
    return want, others, err


def main():
    ap = argparse.ArgumentParser()
    ap.add_argument("funcs", nargs="+")
    ap.add_argument("--from", dest="src", required=True,
                    help="the agent's copy of the unit")
    ap.add_argument("--dry-run", action="store_true")
    ap.add_argument("--no-clone", action="store_true")
    ap.add_argument("-j", type=int, default=8)
    args = ap.parse_args()

    agent_text = open(args.src).read()
    units = {dngclone.unit_path(f) for f in args.funcs}
    if len(units) != 1:
        raise SystemExit("all the functions must belong to one unit: "
                         + ", ".join(sorted(os.path.basename(u) for u in units)))
    dest = units.pop()
    text = open(dest).read()

    want = list(args.funcs)
    dropped = []
    while want:
        new, add = build(agent_text, want, text)
        lines, others, err = probe(new, dest, want)
        if not lines:
            # NOT ONE VERDICT MEANS THE UNIT DID NOT COMPILE, which is a
            # different thing from a function being wrong and must not be
            # reported as every function failing. Dropping names cannot fix
            # it. The useful line of a Python traceback is its LAST one, but
            # a CompileError's text is the compiler's and belongs whole.
            print("FAIL the unit did not compile with these bodies spliced "
                  "in -- no function was measured:")
            for l in (err or "no stderr").strip().splitlines():
                print(f"     {l}")
            raise SystemExit("nothing written")
        bad = [f for f in want
               if " 0 differing " not in lines.get(f, "")
               or not re.search(r"target (\d+) words, candidate \1 words",
                                lines.get(f, ""))]
        reg = [f for f in others if " 0 differing " not in lines.get(f, "")]
        for f in want:
            print(f"{'OK  ' if f not in bad else 'FAIL'} "
                  f"{lines.get(f, f + ': no verdict')}")
        if reg:
            # A regression is NOT attributable to one of the requested
            # functions, so dropping them one at a time cannot fix it. Stop
            # and let a human read it: this is the failure this shape of work
            # adds, and guessing at it is how a landed body gets lost.
            print(f"     {len(others)} other function(s) re-verified, "
                  f"{len(reg)} now failing")
            for f in reg:
                print(f"     REGRESSED {lines.get(f, f + ': no verdict')}")
            raise SystemExit("an already-landed function in this unit "
                             "regressed -- nothing written")
        if not bad:
            break
        print(f"     dropping {', '.join(bad)} and retrying the rest")
        dropped += bad
        want = [f for f in want if f not in bad]

    if not want:
        raise SystemExit("none of the requested functions verify -- "
                         "nothing written")
    print(f"     {len(others)} other function(s) in the unit re-verified, "
          f"0 now failing")
    print(f"     {len(add)} new declaration(s): {'; '.join(add) or 'none'}")
    if dropped:
        print(f"     NOT LANDED: {', '.join(dropped)}")
    if args.dry_run:
        print("     (dry run, nothing written)")
        return
    open(dest, "w").write(new)
    print(f"     wrote {os.path.relpath(dest, PROJ)}: {', '.join(want)}")

    if args.no_clone:
        return
    rc = 0
    for f in want:
        r = subprocess.run(
            [sys.executable, os.path.join(PROJ, "tools", "decomp",
                                          "dngclone.py"),
             f, "-j", str(args.j)], cwd=PROJ)
        rc = rc or r.returncode
    sys.exit(rc)


if __name__ == "__main__":
    main()
