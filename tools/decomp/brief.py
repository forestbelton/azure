#!/usr/bin/env python3
"""brief.py UNIT PREFIX FUNC... -- emit an agent brief for one group.

Written because hand-writing briefs is what produced an error in EVERY brief
across three batches, while the one batch whose briefs carried only
machine-verified facts produced none across six agents. This generator can only
emit facts it reads off the listings, so the failure mode is structurally gone.

It deliberately does NOT inline provision.py's output: the brief tells the agent
to run it, so the provisioning is fresh when the agent reads it rather than as
of whenever the brief was written. csrc/ovl_town.c moves under agents several
times per batch.
"""
import os, re, sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))

HARD = """## HARD RULES -- violating any destroys shared state
- Do **NOT** touch `csrc/`, `include/` or `docs/`. You produce a merge script; I own the merge, the build gate and the commit.
- Do **NOT** run `make`, `make check` or `make splat`. **NEVER** `rm -rf build`. **NEVER** `splat split --modes ld`.
- Do **NOT** commit.
- Everything to `scratch/` with the UNIQUE prefix `%(pre)s`.
- Overlays compile **`-G0`, not `-G8`**. Any helper you write or COPY that compiles must pass `cflags=('-G0',)` -- a helper copied out of `scratch/` was found silently dumping a `-G8` compilation the build never produces.
- **ANOTHER SESSION WORKS IN THIS REPO** and I commit other groups while you run, so `csrc/ovl_town.c` WILL move under you, several times. Anchor-based, re-runnable merge script; assert every anchor BEFORE writing anything; re-run it at the end. Unexpected modified files are not yours and **must not be reverted**.

## HOW TO VERIFY
1. `scratch/%(pre)smerge.py`: read `csrc/%(unit)s.c` **fresh**, `assert text.count(old) == 1` per anchor, write `scratch/%(pre)sunit.c`.
2. `python3 tools/decomp/ovlverify.py scratch/%(pre)sunit.c <names>` -- it compiles once for all of them.
3. **`advise.py` needs literally ONE function per file**; invocation `advise.py FILE UNIT FUNC`. On a merged unit it FABRICATES `CALLEE-SAVED`/`DELAY-NOPS` findings and a wrong `--greg` table. Its ALLOC verdict is the useful one: it means stop rewriting the logic.
4. **Sweep the whole unit in your copy**, from the **C definitions** (`awk` recipe in `docs/town-sweep.md`), never `nm` -- every stub verifies against itself. Expect **`progress.py`'s landed count, plus yours, plus one benign off-by-one**; absolute numbers go stale within the hour.
5. `ovlverify` **MASKS every relocated word**. Decode the `jal`s yourself and read every `%hi`/`%lo` off each target's **own** listing. A `%hi` on a FUNCTION has no `jal` to decode and is invisible to both standard checks; that class has already broken one build. Also: `*((u8 *)&SYM + N)` folds the offset into the RELOCATION and the mask hides the addend, so check the reloc list rather than the printed displacement.
6. Word counts off the listing header.
"""

TRAPS = """## LEVERS AND TRAPS -- try these before anything exotic
Read `docs/matching-levers.md` via `python3 tools/decomp/leverindex.py` (390 levers; GENERATED, never hand-edit, `--check` is read-only). The ones that have paid most often in this sweep:
- **Statement order is a lead, not a rule.** Across two families, three of thirteen and four of four members needed the *opposite* order from their template. Permute mechanically when a few words off at the right length.
- **`sched1`/`sched2` are strictly per-basic-block** (`sched.c:3220-3221`, `:3389`), so an insn in a different block than your candidate puts it can only have been moved by **`reorg`, which steals from two insns back** (`reorg.c:3067-3077`).
- **Where a variable is DECLARED decides which allocator sees it** (`:2263`), and **a pointer-to-global local must be assigned as a STATEMENT, not a declaration initialiser** (`c-decl.c:3947-3948`) and **before any intervening call** (`local-alloc.c:1142`). Fingerprint for the allocator ones: every differing word is a register rename.
- **A store of zero to a halfword field is one word cheaper than any constant.** Check first when two members' headers differ by exactly 4.
- **A dead MIDDLE parameter is witnessed** by the gap it leaves in the `$aN` sequence; only a dead TRAILING one is invisible. An unread parameter costs no words in the CALLEE and can cost a word in the CALLER.
- **A chained assignment emits its stores RIGHT-TO-LEFT**, so a descending chain equals separate descending statements -- a third spelling when adjacent stores look mis-ordered.
- **A cast decides `MEM_IN_STRUCT_P`** and so whether sched1 may hoist a global load over a struct store: `*((s16 *)a + N)` is in-struct (`expr.c:5343-5351`), `*(s16 *)((u8 *)a + N)` is not.
- **A `goto` back-edge is INVISIBLE to `loop.c`** (`stmt.c:2214` is the only `NOTE_INSN_LOOP_BEG` emitter), so nothing in a `goto` loop is strength-reduced. If a candidate is 2 words long with a duplicated induction register, respell the loop with `goto`.
- **`expand_abs`'s copy survives only when a ternary writes a SEPARATE variable** (`optabs.c:2152`/`:2162`) -- this unparked two functions.
- **A duplicated tail CALL STATEMENT is usually free** (cross-jumping unifies it); the duplicated-tail lever bites only when the arms carry live pseudos the allocator must price.
- **Do not start with** where a truncating cast is written, or a predicate-preserving comparison flip -- but DO measure the flip once you are at the right length with a `nop`-versus-insn discrepancy in a delay slot.
- A differing count coinciding with another function's recorded error is **not** evidence, and a nearby function's recorded diagnosis does **not** transfer -- both have cost this sweep real time. A referrer is not a caller.
- `grep -rl func_XXXX asm/` can hit `asm/ovl_dungeon.s` -- unrelated functions at the same vram.
"""

BACK = """## WHAT TO HAND BACK
1. The merge script path.
2. **A table of EVERY spelling measured**, with word and differing counts -- not just winners.
3. **Every `%hi`/`%lo` symbol you read, per function, with its source listing.**
4. Anything that did not land: a quantified near-miss -- exact words, exact differing, which words, what you ruled out -- **plus what your candidate held FIXED**. A park I retired this session had 49 spellings and a permuter run on a plateau, and the answer was a second lever interacting with a variable the park held constant. A park is a statement about a search space, not about a function. **Parking with good numbers is a perfectly good outcome; I would rather have that than a forced landing.**
5. Any new mechanism, with its CDK citation (`scratch/gcc-src/cdk-gcc-b18`) and the line quoted. **Never cite upstream gcc.** If you draft a mechanism and your own measurement falsifies it, report that -- two agents did this batch and it is worth more than a confident guess.
6. Your whole-unit sweep result.
7. **Errors in this brief, and in `provision.py`'s output.** The tool has had eighteen defects found by the agents using it; it is better now but not trustworthy by default. Its relative RANKING has been reported inverted or empty four times -- treat it as a lead and ALSO read each target's immediate C-file neighbours and try an idiom grep (grep a distinctive *statement* from the listing, not a callee name).

Do not stop to ask permission. If you run long, land what you can and report the rest with numbers rather than abandoning the run.
"""

def main():
    unit, pre = sys.argv[1], sys.argv[2]
    fns = sys.argv[3:]
    D = os.path.join(PROJ, 'asm/nonmatchings', unit)
    rows, tot, haz = [], 0, []
    for f in fns:
        t = open(os.path.join(D, f + '.s')).read()
        hx = re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t).group(1)
        w = int(hx, 16) // 4; tot += w
        jt = len(re.findall(r'jr\s+\$(?!ra)', t))
        rod = 1 if ('.section .rodata' in t or re.search(r'^dlabel D_', t, re.M)) else 0
        if jt or rod: haz.append((f, jt, rod))
        rows.append((f, hx, w))
    print("Land %d MIPS R3000 PSX function(s) in /home/case/projects/azure, overlay `%s`, "
          "to BYTE-IDENTICAL output. %d words.\n" % (len(fns), unit, tot))
    print("## STEP 0 -- MANDATORY FIRST COMMAND\n")
    print("    python3 tools/decomp/provision.py %s %s\n" % (unit, ' '.join(fns)))
    print("""Read all of it. It computes, fresh: per-function callee lists (never a union), every callee classified LANDED-ABOVE / LANDED-BELOW / STUB-HERE / NOT-IN-THIS-UNIT with its prototype (searching `include/` too) and a loud marker when there is none, your targets' own existing declarations, the `%hi`-on-a-function installs, relatives by rarity-and-conjunction score, landed callers, ASM callers, who takes your targets' addresses, and arity leads.

**This brief contains NO provisioning prose, deliberately, and it is machine-generated so that it cannot.** Across three batches where I hand-wrote "worth knowing in advance" paragraphs on top of that tool, my prose was wrong in EVERY brief while the tool was right every time -- a wrong callee in a "shared head" (which measures **0 differing**, because `ovlverify` masks the `jal`), installed handlers carried forward from the previous batch's pair, swapped word counts. In the batch whose briefs carried only machine-verified facts, all six agents reported no brief errors. Take your provisioning from the tool, not from me.

LANDED-ABOVE versus LANDED-BELOW matters: an arity mismatch on a callee defined ABOVE its call site is a hard "too many arguments", while one defined BELOW rides an implicit declaration, which this unit does 18 times.
""")
    print("## WHAT I VERIFIED MYSELF (read off the listings; the only facts I assert)\n")
    print("| function | header | words |")
    print("|---|---|---|")
    for f, hx, w in rows:
        print("| `%s` | `%s` | %d |" % (f, hx, w))
    if haz:
        print("\n**HAZARD -- these own rodata or have a jump table, so the build gate is the")
        print("only authority and a clean whole-unit sweep will NOT show a rodata shift:**")
        for f, jt, rod in haz:
            print("- `%s`: jump table %d, own rodata %s" % (f, jt, 'YES' if rod else 'no'))
        print("\nIf a local array initialiser or a jump table is involved, report the exact")
        print("bytes and alignment your C emits versus the listing, and hand it back")
        print("unlanded if you cannot make them agree -- I adjudicate with the build.")
    else:
        print("\nAll of them: **no jump table** (no `jr` on a non-`$ra` register) and **no own")
        print("rodata** (no `dlabel`, no `.section .rodata`). Re-confirm off each header anyway.")
    print("\nThese were grouped by `tools/decomp/batcher.py` on shared callees/globals weighted")
    print("by rarity, slot-chain edges, and address locality. **The grouping may be wrong** --")
    print("one of my clusters last batch was a shape coincidence, not a family. Diff within")
    print("the group first, and say so with numbers if a member does not belong.\n")
    print("## REQUIRED READING")
    print("`docs/town-sweep.md` -- method, traps, and the pre-send checklist, which is a list")
    print("of defects *my own briefs* have had. `docs/where-we-are.md`.\n")
    print(HARD % dict(pre=pre, unit=unit))
    print(TRAPS)
    print(BACK)

if __name__ == '__main__':
    sys.exit(main())
