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
import io, os, re, sys

PROJ = os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__))))
sys.path.insert(0, os.path.dirname(os.path.abspath(__file__)))
import leverpick

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
3. **RE-RUN `provision.py` NEAR THE END, not only at step 0.** It has no cache --
   it reads `csrc/%(unit)s.c` fresh every time -- but in a parallel campaign a
   sibling may land your line-for-line template WHILE you work. That happened:
   a target's exact template was absent from the relatives table at step 0 and
   ranked 0.499 an hour later, and the agent found it by idiom grep instead.
4. **`tools/decomp/sched.py FILE FUNC [--sched1]` prints cc1psx's OWN scheduler dump** -- every insn's priority, the ready list at each cycle, every hazard decision, the scheduled RTL, and `--insn N` reads a ready-list number back to an instruction. Use it instead of modelling the scheduler in your head: one agent hand-rolled this with `-dS` and said the dump is what told it the priorities were 4-8 rather than the long chains it had assumed, and that an arm it thought was several blocks is one. **`tools/decomp/relocdump.py FILE [FUNC...]`** dumps your candidate's relocations at `-G0` and flags how many sit on a FUNCTION symbol -- six agents hand-rolled that one before it was promoted, and two of the six hit traps baked into it now.
5. **`advise.py` needs literally ONE function per file**; invocation `advise.py FILE UNIT FUNC`. On a merged unit it FABRICATES `CALLEE-SAVED`/`DELAY-NOPS` findings and a wrong `--greg` table. Its ALLOC verdict is the useful one: it means stop rewriting the logic.
6. **Sweep the whole unit in your copy**, from the **C definitions** (`awk` recipe in `docs/town-sweep.md`), never `nm` -- every stub verifies against itself. Expect **`progress.py`'s landed count, plus yours, plus one benign off-by-one**; absolute numbers go stale within the hour.
7. `ovlverify` **MASKS every relocated word**. Decode the `jal`s yourself and read every `%hi`/`%lo` off each target's **own** listing. A `%hi` on a FUNCTION has no `jal` to decode and is invisible to both standard checks; that class has already broken one build. **`ovlverify --raw` is the other half of the check, and the arithmetic has to close:** raw differing must equal exactly the relocation words plus intra-function `j` words in that listing. Worked examples that closed: 41 against 18+14+9; 38 against 9+29+0; 28/7/0/26 against 18+5+5, 6+0+1, 0+0+0 and 19+5+2. If it does not close, a REAL difference is hiding behind the mask. Also: `*((u8 *)&SYM + N)` folds the offset into the RELOCATION and the mask hides the addend, so check the reloc list rather than the printed displacement.
8. Word counts off the listing header.
"""

TRAPS = """## LEVERS AND TRAPS -- try these before anything exotic
Read `docs/matching-levers.md` via `python3 tools/decomp/leverindex.py` (%LEVERS% levers; GENERATED, never hand-edit, `--check` is read-only).

### ALWAYS -- the ones that have paid most often across this sweep
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

MATCHED_HEAD = """### MATCHED BY THIS TARGET'S SHAPE
Picked by `tools/decomp/leverpick.py` from each target's OWN listing: the
signal in brackets is what triggered the suggestion, so when the signal is a
false positive for your function, dismiss the lever. These are CANDIDATES, not
findings -- measured recall is about one brief in five carrying the lever that
eventually paid, so the ALWAYS list above and your own reading still come
first. Headings and line numbers only; read a section with the `sed` below.
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
    av = [a for a in sys.argv[1:] if a != '--no-levers']
    # The shape-matched block is additive and capped, but its measured recall
    # of the lever that ACTUALLY paid is 18%, not the 50% that would justify
    # it outright. `--no-levers` drops it and leaves the ALWAYS list alone.
    want_levers = '--no-levers' not in sys.argv
    unit, pre = av[0], av[1]
    fns = av[2:]
    D = os.path.join(PROJ, 'asm/nonmatchings', unit)
    rows, tot, haz, listings = [], 0, [], []
    for f in fns:
        t = open(os.path.join(D, f + '.s')).read()
        listings.append((f, t))
        hx = re.search(r'nonmatching \S+, (0x[0-9A-F]+)', t).group(1)
        w = int(hx, 16) // 4; tot += w
        jt = len(re.findall(r'jr\s+\$(?!ra)', t))
        # OWN RODATA MEANS A MIGRATED `dlabel D_`, NOT ANY `.section .rodata`.
        # Every jump-table listing has a .rodata block for the table itself,
        # so the old test said YES for a function whose only rodata IS its
        # table -- and jumptable + own-rodata is exactly the conjunction the
        # func_800BF1C8 park was blocked on, so the brief reported the
        # park-blocking shape for functions that cannot have it. Said so for
        # func_800ABA1C on 2026-11-03, which landed byte-identical with its
        # table; provision.py was fixed for the same conflation in 6da6df0.
        # Anchored, because `enddlabel` contains `dlabel`.
        rod = 1 if re.search(r'^dlabel D_', t, re.M) else 0
        jtr = 1 if re.search(r'^dlabel jtbl_', t, re.M) else 0
        if jt or rod or jtr: haz.append((f, jt, rod, jtr, t))
        # THE ADDRESS-TAKE WORD COUNT, COMPUTED RATHER THAN PREDICTED.
        # The masked build-breaking class is %hi/%lo OCCURRENCES on a function
        # symbol, not (distinct install targets) x 2 -- a handler address can
        # be materialised more than once in one function, and on 2026-11-03
        # func_800BBCA0 materialised one target twice, so my hand-written
        # "expect 2" would have made a correct relocdump reading of 4 look
        # wrong. Across 40 installing stubs three disagree with the doubling
        # formula (1 target/4 words, 3/8 and 3/12). relocdump prints the right
        # number; this puts the same number in the brief so nobody has to take
        # mine on trust.
        at = len(re.findall(r'%(?:hi|lo)\(func_[0-9A-Fa-f]+\)', t))
        rows.append((f, hx, w, at))
    print("Land %d MIPS R3000 PSX function(s) in /home/case/projects/azure, overlay `%s`, "
          "to BYTE-IDENTICAL output. %d words.\n" % (len(fns), unit, tot))
    print("## STEP 0 -- MANDATORY FIRST COMMAND\n")
    print("    python3 tools/decomp/provision.py %s %s\n" % (unit, ' '.join(fns)))
    print("""Read all of it. It computes, fresh: per-function callee lists (never a union), every callee classified LANDED-ABOVE / LANDED-BELOW / STUB-HERE / NOT-IN-THIS-UNIT with its prototype (searching `include/` too) and a loud marker when there is none, your targets' own existing declarations, the `%hi`-on-a-function installs, relatives by rarity-and-conjunction score, landed callers, ASM callers, who takes your targets' addresses, and arity leads.

**This brief contains NO provisioning prose, deliberately, and it is machine-generated so that it cannot.** Across three batches where I hand-wrote "worth knowing in advance" paragraphs on top of that tool, my prose was wrong in EVERY brief while the tool was right every time -- a wrong callee in a "shared head" (which measures **0 differing**, because `ovlverify` masks the `jal`), installed handlers carried forward from the previous batch's pair, swapped word counts. In the batch whose briefs carried only machine-verified facts, all six agents reported no brief errors. Take your provisioning from the tool, not from me.

LANDED-ABOVE versus LANDED-BELOW matters: an arity mismatch on a callee defined ABOVE its call site is a hard "too many arguments", while one defined BELOW rides an implicit declaration, which this unit does 18 times.
""")
    print("## WHAT I VERIFIED MYSELF (read off the listings; the only facts I assert)\n")
    print("| function | header | words | address-take words |")
    print("|---|---|---|---|")
    for f, hx, w, at in rows:
        print("| `%s` | `%s` | %d | %d |" % (f, hx, w, at))
    tot_at = sum(at for *_, at in rows)
    print("\nThe last column is the MASKED, build-breaking class: `%hi`/`%lo`")
    print("OCCURRENCES on a FUNCTION symbol, counted off each listing. **%d words"
          % tot_at)
    print("across the group.** `relocdump.py` must report exactly this, and it is not")
    print("(install targets x 2) -- one handler address can be materialised more than")
    print("once in one function. Report what relocdump prints, not what you expect.")
    if haz:
        print("\n**HAZARD -- these own rodata or have a jump table, so the build gate is the")
        print("only authority and a clean whole-unit sweep will NOT show a rodata shift:**")
        for f, jt, rod, jtr, lt in haz:
            print("- `%s`: jump table %d, own rodata %s" % (
                f, jt, 'YES' if rod else ('jump table only' if jtr else 'no')))
            if jtr:
                # The one computation that decides whether a jump-table
                # function is safe to write naturally. Doing it by hand for
                # two briefs is what prompted putting it here.
                a = re.search(r'^dlabel (jtbl_[0-9A-Fa-f]+)', lt, re.M).group(1)
                off = int(a.split('_')[1], 16) - 0x80088760
                ent = len(re.findall(r'^\s*/\* [0-9A-F]+ [0-9A-F]+ [0-9A-F]{8} \*/\s+\.word',
                                     lt, re.M)) or len(re.findall(r'\.word', lt))
                print("    %s: %d entries, rodata offset 0x%X, %d mod 8 -- %s"
                      % (a, ent, off, off % 8,
                         "`.align 3` is a NO-OP, the table costs nothing"
                         if off % 8 == 0 else
                         "*** 4 mod 8: `.align 3` WILL PAD it and the container "
                         "comes out long -- the class that forced the carve ***"))
                print("    The ENTRY COUNT is load-bearing, not cosmetic: with no `addiu`")
                print("    subtraction before the `sltiu` range check it pins minval 0, which")
                print("    is what tells you whether `case 0:` is in the source.")
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
    print(HARD.replace('%(pre)s', pre).replace('%(unit)s', unit))
    # The count was hardcoded at 390 and was stale within the day -- the same
    # line-number drift leverpick.py avoids by calling leverindex.build() every
    # time. Take it from the tool.
    try:
        import leverindex
        n = str(len(leverindex.build(
            io.open(leverindex.DOC, encoding='utf-8').read().split("\n"))))
    except Exception:
        n = "390+"
    print(TRAPS.replace('%LEVERS%', n))
    # NEVER %-format a lever heading: they are full of literal `%hi`/`%lo`,
    # and `%hi` parses as an `h` length modifier plus an `i` conversion. That
    # crashed this file on every invocation for eight agent runs.
    block = ""
    if want_levers:
        try:
            per = max(1, min(8, 14 // max(1, len(fns))))
            block = leverpick.render(listings, limit=per)
        except Exception as e:
            block = ("leverpick.py failed (" + type(e).__name__ + ": "
                     + str(e) + ") -- read the index with `leverindex.py`.")
    if block.strip():
        print(MATCHED_HEAD)
        print(block)
        print("")
    print(BACK)

if __name__ == '__main__':
    sys.exit(main())
