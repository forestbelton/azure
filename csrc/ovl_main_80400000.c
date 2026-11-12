#include "common.h"
#include "main_twin.h"

/* main.bin's DEBUG TWIN of the 0x80020000 card overlay: the same source built
 * at -G8 with its asserts compiled in, linked against a different engine
 * build, and dead on retail (0x80400000 is 8 MB dev-board RAM). It is in the
 * container, so it still has to be reproduced. See docs/overlays.md.
 *
 * THIS UNIT IS THE ONE THE PROJECT BUILDS WITH A DIFFERENT COMPILER. The
 * Makefile points it at tools/psx272gcc -- native FSF gcc 2.7.2 for
 * mips-sony-psx -- because this block is a PSY-Q 4.0 build and everything
 * else is 4.1. Two passes separate them, reload_cse_regs and split
 * addresses, and both are Cygnus/SN additions over FSF 2.7.2. The triage
 * below has the evidence; tools/psx272gcc's header has the provenance.
 * EVERY MEASUREMENT OF THIS UNIT MUST USE THAT COMPILER -- asmlib resolves
 * it from the Makefile, so ovlverify/verify/advise/permimport/twinalign all
 * do it automatically; a hand-rolled `tools/cdkgcc` command does not.
 *
 * THE PLAN IS NOT A SECOND DECOMPILATION. The live overlay's own source
 * rebuilt at -G8 already reproduces 86 of this block's 263 functions
 * instruction-for-instruction, 231 of 263 pair 1:1 with a function it
 * defines, and the 32 that do not are engine code this build linked
 * statically (memory.c, the PSY-Q stubs) which is decompiled in the resident
 * units. tools/assertmap.py reads the missing asserts straight out of here.
 *
 * WHY THE SYMBOLS ARE SPELLED 0x8040xxxx RATHER THAN SHARED. Both blocks sit
 * in ONE container, so a name cannot resolve to two addresses, and the two
 * builds do not agree on any constant offset: card_slot is D_800287CC live
 * and D_804094EC here, a delta of 0x3E0D20, while the data bases differ by
 * 0x3E1320. The external calls are worse than renamed -- this block was
 * linked against a different engine build, where printf is at 0x8007C040,
 * which in the shipped executable is inside libgte's rcossin_tbl.
 *
 * THE LAST 23 FUNCTIONS ARE NOT DECOMPILATION WORK. Everything above
 * func_804084DC -- 23 functions, 289 words, 3.5% of the block -- is PSY-Q
 * library code, and this project keeps PSY-Q library code as ASSEMBLY
 * everywhere, including for the resident executable. Two kinds of evidence,
 * and they agree:
 *
 *   * Nine of them are BIOS call stubs: `addiu $t2, $zero, 0xB0; jr $t2;
 *     addiu $t1, $zero, <call>`, a jump to the BIOS entry point with the
 *     call number in $t1. That is not C in any revision.
 *   * func_804087C8 matches `_ExitCard` (0x80069D78) at 0.90 masked, and
 *     `_ExitCard`'s own listing is `asm/psyq/libcard/END.s` -- the resident
 *     copy of this same code is assembly in this tree too. The `jalr $t2`
 *     BIOS calls are right there in both. The eleven neighbours below it are
 *     named per module in `docs/twin-block-libcard.md`.
 *
 * So they stay on INCLUDE_ASM, as the asm/psyq listings do, and the block's
 * real remaining work is the 239 functions below them.
 *
 * HOW A FUNCTION GETS HERE. `tools/decomp/twinalign.py --todo` pairs each of
 * this block's functions with the live function it came from and sorts by how
 * much they differ; --symmap gives the live -> twin rename. A body is moved
 * over with its references renamed, then measured with
 * `tools/decomp/ovlverify.py --G 8` -- note the --G 8, which is the opposite
 * of every other overlay unit.
 *
 * WHAT THE FIRST 86 FUNCTIONS COST, AND WHAT THEY TAUGHT. Every function
 * whose masked body was already identical to the live source at -G8 was moved.
 * 71 landed at 0 differing words with nothing changed but their symbol names.
 * The other 15 differed ONLY in immediates, and all 15 now land too, after
 * adjustments that fall into four families -- none of which is a different
 * SPELLING of the body, and all of which say something about this build:
 *
 *   * THE WORD INDICES THROUGH ITS STRUCTURES DIFFER, by a constant per
 *     structure: -0x91 words in func_80402BB8, -0x90 in func_80402EBC and
 *     func_804033EC, -0x21 in func_80404314/func_804044D8/func_8040454C,
 *     -0xD in func_8040553C/func_80405900/func_804057B0, +6 in
 *     func_804038C8/func_80403738/func_80403B84/func_80403CD4. These bodies
 *     index a plain `s32 *p` by word, not an UNK_CARD_GROUP struct, so the
 *     fix is the index and the deltas are a direct read-out of how this
 *     build's data is laid out against the live one's.
 *
 *   * FOUR OF ITS TABLES HAVE ONE MORE ENTRY. func_804038C8, func_80403738,
 *     func_80403B84 and func_80403CD4 each need a loop bound of 4 where the
 *     live source has 3, and the +6-word index shift beside it is 24 bytes --
 *     one UnkStrSet record. func_804038C8 also wants 0x22 where the live
 *     source writes 0x1A. A TABLE WITH ONE MORE ENTRY IS NOT SOMETHING AN
 *     ASSERT DOES: this is the first hard evidence for the open question in
 *     docs/where-we-are.md, that part of this block is a DIFFERENT REVISION
 *     of the source rather than the same source with asserts compiled in.
 *
 *   * ONE FUNCTION LOOKED LIKE IT HAD A LOCAL THE LIVE SOURCE DOES NOT
 *     (func_804025F8, 8 bytes, never read). IT DOES NOT, and the device is
 *     gone: the 8 bytes were the COMPILER, and FSF 2.7.2 reserves them by
 *     itself. Kept in this list because a frame 8 bytes too small still
 *     looks like a missing local, and now there is a second thing to check
 *     first.
 *
 *   * ONE CONSTANT IS LOWER BY ONE (func_80402DC8 passes 6 where the live
 *     source passes 7).
 *
 * AND THEN THE SECOND COMPILER RESTARTED IT, 2026-11-11. With
 * tools/psx272gcc in place, `twinalign --todo` went from 1 mask-identical
 * pair to 33, and 32 of those 33 landed in one pass: 21 with nothing but
 * their symbol names changed, 11 after an adjustment. The adjustments were
 * all old families plus TWO NEW ONES:
 *
 *   * A WHOLE STRUCTURE 0x84 BYTES EARLIER. func_80404668, func_804045C8,
 *     func_80404618 and func_80403E54 reach UnkCardGrp240's fields at
 *     0x1C0/0x21C/0x1DC/0x1EC/0x1F4 where the live struct has
 *     0x244/0x2A0/0x260/0x270/0x278 -- a flat -0x84 on every field. Written
 *     as raw word indices off `s32 *p`, as func_804040C8 already was. This
 *     is the first-bullet family at a larger scale, and the cleanest
 *     evidence yet that the two builds' structures genuinely differ.
 *
 *   * FOUR REVISION DIFFERENCES IN ONE FUNCTION. func_80403698 needs the
 *     ptrs array 0x18 later, a loop bound of 4 for 3, func_80056F14 called
 *     with 4 for 3, and a 0x3C stride for 0x48. Nothing in it is a spelling.
 *
 *   * AND THE MIRROR OF func_804065CC's ONE-HANDLER-FOR-TWO:
 *     func_80405F24 has TWO handlers where the live source has one
 *     (func_80406368 and func_80405AB4 for the single live func_80024FFC),
 *     which is why twinmove refuses that symbol -- a real tie, not a weak
 *     vote.
 *
 * THE ONE TRAP IN THAT BATCH IS WORTH MORE THAN THE BATCH. Six of the 32
 * linked against the RESIDENT engine and every one of them measured 0
 * differing. The live source names three objects through `__asm__` ALIASES
 * rather than their symbols -- `card_file_count`, `card_free_blocks`,
 * `D_800814A0_a` -- twinmove only resolved `#define` aliases, so the names
 * survived the rename, csrc/main.h's own declarations resolved them at
 * compile time, and the relocations pointed at the resident executable
 * (D_80084128 where this build wants D_80136140, a different engine build
 * entirely). ovlverify masks relocation targets, so it saw nothing. ONLY THE
 * CONTAINER LINK CAUGHT IT, 33 bytes in 2.5 MB. twinmove now resolves
 * `__asm__` aliases too, carries an alias SUFFIX across the rename when the
 * twin declares one, and reports any alias name that survives. The general
 * rule stands: a clean ovlverify sweep is a reason to build, never a
 * substitute for building.
 *
 * THE SECOND PASS, same day: the 24 `near` rows. 14 more landed, 141 -> 155.
 *
 *   * TWELVE WERE THE reload_cse_regs WRAPPERS and needed no thought at all
 *     once the compiler was right -- see their own comment. Their IDs, not
 *     twinalign, are what pairs them: every wrapper masks to the same ten
 *     words, so the DP had the whole run shifted by one.
 *   * func_804067BC moved over untouched.
 *   * func_80401ED4 NEEDED THE LIVE SOURCE'S OWN DEVICE REVERSED, which is
 *     the most interesting thing in this batch -- see its comment. Without
 *     split addresses a MEM address FOLDS, so the spelling that costs four
 *     words under 4.1 costs three here, and the pointer-local goes on the
 *     OTHER access.
 *
 * THE OTHER TEN ARE REAL MATCHING WORK and are back on INCLUDE_ASM, but their
 * SYMBOL MAPS are not: those are in csrc/main_twin.h now, and they are the
 * expensive part to re-derive. Three of them are findings rather than renames:
 *
 *   * func_80405DE0 (49 differing). func_80024FFC maps to TWO twin functions,
 *     func_80405AB4 after the loop and func_80406368 in the fail arm.
 *     func_80024298's symmap vote is func_80404C84 and is WRONG (a 0.26
 *     pairing); the listing says func_80404688. AND func_80025358 IS NOT
 *     CALLED AT ALL -- the live source calls it between the read and the
 *     write and it is two calls deep (set_save_title + set_save_icon), where
 *     this build has one store, `sw $zero, %lo(0x80010208)`. An earlier
 *     revision of the function, not an assert difference.
 *   * func_804012B0 and func_804010F0, the two state machines, LANDED
 *     2026-11-12. Their two listing-settled renames are below; the devices
 *     that closed them are on func_804010F0 itself.
 *   * D_80084118 -> D_8009E390 for BOTH of those, but a different twin
 *     function genuinely uses the resident D_80084118. Check the listing per
 *     function; the name is not enough.
 *
 * WHERE THE MECHANICAL PHASE ENDED THE FIRST TIME. Everything above the
 * second-compiler batch was a move plus an immediate. The 33 functions of
 * twinalign's `near` and `inserts-only` classes that carried no assert were
 * moved the same way and measured, and only ONE of them (func_80407E08)
 * landed; the rest went back on INCLUDE_ASM. EVERY DIFFERING-WORD COUNT IN
 * THE TRIAGE BELOW WAS TAKEN UNDER THE 4.1 COMPILER AND IS STALE. It is kept
 * because the reasoning is still worth reading, and because two of its
 * verdicts -- the reload_cse_regs cluster and the folded indexed load --
 * are what identified the compiler in the first place.
 *
 *   14 fn, 1 word   NOT A SPELLING -- THIS BLOCK'S COMPILER LACKS
 *   each            reload_cse_regs. RESOLVED 2026-11-11; the park below was
 *                   left open as "one lever here is worth 14 functions" and
 *                   the answer is that there is no source lever at all.
 *
 *                   func_804030F4, func_8040311C, func_80403144,
 *                   func_8040320C, func_80403234, func_8040325C,
 *                   func_80403284, func_804032AC, func_804032D4,
 *                   func_804032FC, func_8040334C, func_80403374,
 *                   func_804033C4 -- the card_mes.c wrappers main.h
 *                   documents -- plus func_804013F8. The live build copies a
 *                   hard register that already holds the value; this block
 *                   materialises it again:
 *
 *                     live  addu $a3,$a2,$zero   twin  addu  $a3,$zero,$zero
 *                     live  addu $a2,$a1,$zero   twin  addiu $a2,$zero,0x2
 *                     live  addu $v1,$v0,$zero   twin  addiu $v1,$zero,imm
 *
 *                   14 for 14, one word each, same position each time, and
 *                   NO function differs the other way round. The pass is
 *                   reload_cse_simplify_set (reload1.c:8142) -- not cse: the
 *                   operand is (const_int 0) in every dump through .lreg and
 *                   (reg a2) in .greg. It has no cost test, takes the
 *                   lowest-numbered hard register holding the value, is
 *                   gated only on `optimize > 0`, and IS ABSENT FROM VANILLA
 *                   gcc-2.7.2.3 -- an SN addition that cdk-gcc-b18 has. So
 *                   this block was built by a cc1psx WITHOUT it, which is
 *                   also why the footprint is so narrow: the pass only bites
 *                   where a hard register already holds a wanted constant.
 *                   docs/matching-levers.md has the full heading and the
 *                   swept-and-failed spellings; do not retry them.
 *
 *                   RESOLVED 2026-11-11 BY A SECOND COMPILER, and it needed
 *                   no wine. THIS BLOCK IS A 4.0 BUILD: two unrelated passes
 *                   4.1 has are missing from it -- reload_cse_regs, above,
 *                   and split addresses, which is the very discriminator
 *                   docs/compiler-version.md uses to prove the GAME code is
 *                   4.1. SN never released 4.0's source (decompals/old-gcc
 *                   has one CDK tarball, b18, and that IS 4.1 -- its
 *                   version.c is `cygnus-2.7.2-970404 SN32.3.7.0004`, the
 *                   string 4.1's cc1psx.exe reports). But 4.0's reports
 *                   `2.7.2.SN32.3.7 Build 0001`, so 4.0 is SN's build of FSF
 *                   gcc 2.7.2 where 4.1 is SN's build of a Cygnus snapshot
 *                   -- and reload_cse, split_addresses, -mfix4300 and
 *                   -ffunction-sections are all in b18 and all absent from
 *                   FSF 2.7.2 AND from 4.0's binary. So FSF 2.7.2 stands in,
 *                   this unit builds with tools/psx272gcc, and both classes
 *                   closed at once.
 *
 *                   IT IS A STAND-IN, NOT THE COMPILER. Where it and the
 *                   real Build 0001 differ the bytes will say so. What is
 *                   measured: 105 of the 106 functions landed under 4.1 are
 *                   byte-identical under it untouched, the 106th
 *                   (func_804025F8) matches once its frame-padding device is
 *                   deleted, and FOUR devices came out of this unit and
 *                   csrc/main_twin.h altogether.
 *
 *   LANDED 2026-11-12  func_80406720 and func_804070C4, which were a
 *                   register race plus a delay slot and stood at 2 words
 *                   differing for three sessions. BOTH halves had a lever.
 *                   The race (the candidate gives $s0 to the counter and
 *                   $s1 to the pointer, the target the reverse, and the four
 *                   prologue/epilogue saves follow) is a priority race: cse
 *                   folds every `q[n]` outside the loop to p-relative, so
 *                   `q` has NO reference outside the loop and loses to `i`.
 *                   A degenerate `do { } while (0)` over the body buys it
 *                   one -- flow.c:440 weights a reference by loop_depth.
 *                   The delay slot is reorg scanning BACKWARD out of a
 *                   CALL_INSN; an empty `asm` between `q++` and the call
 *                   stops it (stop_search_p) and the forward scan takes
 *                   `i++`. Both are in docs/matching-levers.md.
 *
 *   LANDED 2026-11-11, all three by the SAME lever -- a declared size of 8
 *   or less takes the -G8 small-data path, which costs the address ONE
 *   register and leaves the %hi/%lo pair ADJACENT so nothing schedules
 *   between them. See docs/matching-levers.md, "A global's DECLARED SIZE
 *   also decides how many registers its address costs".
 *
 *     func_804078D8   was "two words transposed, a lui/addiu scheduling
 *                     order". D_80408D6C's real size is 8 and it was
 *                     declared `[]`; declaring `[8]` is both correct and the
 *                     fix. Four more twin objects had the same wrong
 *                     declaration (D_804005F0, D_80408C84, D_80408CE8,
 *                     D_80408CF0, D_80408D64); all now carry their real
 *                     size, with the container byte-identical either way.
 *     func_804037F4   needed the lever on a table whose real size is 16, so
 *                     under 4.1 it used a D_80408C8C_a[1] alias. That alias
 *                     is gone -- tools/psx272gcc needs no help here. Its two
 *                     revision differences stand: stride 0x3C for 0x48,
 *                     bound 4 for 3.
 *     func_804040C8   needed no declaration change once D_804005F0 was
 *                     sized -- only the -0x21 word indices and four revision
 *                     constants.
 *
 *   AND ONE ON A SECOND LEVER, THE INCREMENT ORDER. func_80405B98 walks a
 *   5-element loop with a moving pointer `off` and a counter `i`. Written the
 *   live way -- `off++;` then `while (++i < 5)` -- the POINTER increment
 *   fills the two `j` delay slots and the `bnez` takes a nop: 18 differing.
 *   Incrementing the COUNTER first, with a plain `while (i < 5)`, puts `i++`
 *   in those slots and pushes `off++` into the `bnez`'s, which is this
 *   build's shape: 0 differing. Its other difference is an arity one --
 *   func_80405A00 takes TWO arguments here where the live func_80024F3C
 *   takes three, and its listing never reads $a2.
 *
 *   func_80405AE8 was THE SAME TWO LEVERS AND THEN ONE reload_cse_regs
 *   WORD, which made the cluster 15 rather than 14. It was parked at exactly
 *   one differing word with a note saying it would land unchanged the day a
 *   cc1psx without that pass existed. LANDED 2026-11-11 on tools/psx272gcc,
 *   body unchanged, park deleted.
 *
 *   LANDED 2026-11-11 ON A THIRD LEVER, THEN UN-LEVERED THE SAME DAY.
 *   func_80406BEC (12 -> 0), func_804065CC and func_80406F70 (11 -> 0 each)
 *   were landed with a sized DATA ALIAS of a FUNCTION symbol -- `extern u8
 *   F_a[8] __asm__("F");` gives a function a declared size, which puts its
 *   address on the -G8 small-data path and makes it ONE insn, so reorg has
 *   no `lui` to steal into the preceding branch's delay slot where this
 *   build keeps a nop. All three were EMULATING a compiler without split
 *   addresses, one site at a time. This unit now has one, so the plain
 *   spellings match and the three aliases are deleted. The revision
 *   difference they also carry is real and stays: one handler where the live
 *   source has two.
 *
 *   The lever itself is still true and still documented in
 *   docs/matching-levers.md -- it is the right tool for a 4.1-built unit.
 *
 *   func_80406274 and func_80404DEC were THE FOLDED INDEXED LOAD, and both
 *   LANDED 2026-11-11 by moving the live body over with its symbols renamed
 *   and nothing else. Between them they had 33 swept spellings recorded
 *   against them. See their own comments.
 *
 *   others          THE WHOLE LIST IS STALE as of the compiler switch and
 *                   must be re-measured, not read. Every count in it was
 *                   taken under 4.1's cc1. `twinalign.py --todo` on
 *                   tools/psx272gcc now reports 33 of the 156 remaining
 *                   stubs as MASK-IDENTICAL to their live counterparts
 *                   (against 1 before) and 24 more as `near`, so the
 *                   remaining work is mostly moving live bodies over with
 *                   the symbol map rather than finding levers.
 *
 *                   What is known to still be real: func_804018FC is 42
 *                   words against a live counterpart of 2, so that pairing
 *                   is wrong or the live source lost the body entirely.
 *                   func_804012B0's block layout was characterised under
 *                   4.1 and the characterisation was RIGHT: its last arm
 *                   falls through to the shared return in the live build and
 *                   jumps to it here. It landed 2026-11-12 by making that
 *                   jump real -- see the note on the function.
 *
 *                   RE-MEASURE BEFORE BELIEVING ANY OF IT. `rm -rf
 *                   build/twinalign` first: that cache keys on the source
 *                   mtime only and does not notice the compiler changing.
 */

/* The twin of set_card_slot, and the pair that measures the -G difference:
 * 24 words at -G0 in the live block, 26 here at -G8, where a global access
 * comes out as the assembler-expanded `lui $at` + `%lo` pair instead of the
 * hoisted-lui split. Same body as the live one, including the pointer local,
 * which is the in-place flag update device. */
void func_804008A0(s32 use_second_slot)
{
    s32 *p;

    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    p = &dbg_card_slot;
    if (use_second_slot)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    func_804013F8(dbg_card_slot);
}

void func_80400908(void)
{
    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    func_804013F8(D_804094EC);
}

void func_80400948(void)
{
    long next_card_slot;

    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    next_card_slot = 0x10 - D_804094EC;
    D_804094EC = next_card_slot;
    func_804013F8(next_card_slot);
}

void func_80400998(void)
{
    s32 *p = &D_804094E8;

    *p = 0;
    func_804014E4();
}

long func_804009C0(void)
{
    D_80409504 = CARD_STATE_EOF;
    return 0;
}

long func_804009D4(void)
{
    D_80409504 = CARD_STATE_ERROR;
    return 0;
}

long func_804009E8(void)
{
    D_80409504 = CARD_STATE_TIMEOUT;
    return 0;
}

long func_804009FC(void)
{
    D_80409504 = CARD_STATE_NEW;
    return 0;
}

/* open_card_events, sixteen assert sites. Two things beyond the renames.
 *
 * NORETURN on func_8007BEF0 is what finally landed this. The seven delay
 * slots that used to come out empty are the ones where the target steals the
 * NEXT call's $a1 event constant into the preceding `bnez`: reorg will only
 * do that if the assert arm cannot fall through, and without the attribute
 * gcc treats it as an ordinary pair of calls that return.
 *
 * The LINE NUMBERS are this block's, not the live source's -- an offset of
 * -5 from the live open_card_events, per function -- and they come from
 * `tools/assertmap.py --sites`, which reads them out of the block. */
void func_80400A10(void)
{
    func_8007CA38();
    D_80409270 = func_8007C998(SwCARD, EvSpIOE, EvMdNOINTR, NULL);
    if (D_80409270 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 180);
        func_8007BEF0(1);
    }
    D_80409274 = func_8007C998(SwCARD, EvSpERROR, EvMdNOINTR, NULL);
    if (D_80409274 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 182);
        func_8007BEF0(1);
    }
    D_80409278 = func_8007C998(SwCARD, EvSpTIMOUT, EvMdNOINTR, NULL);
    if (D_80409278 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 184);
        func_8007BEF0(1);
    }
    D_8040927C = func_8007C998(SwCARD, EvSpNEW, EvMdNOINTR, NULL);
    if (D_8040927C == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 186);
        func_8007BEF0(1);
    }
    D_80409280 = func_8007C998(HwCARD, EvSpIOE, EvMdINTR, func_804009C0);
    if (D_80409280 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 189);
        func_8007BEF0(1);
    }
    D_80409284 = func_8007C998(HwCARD, EvSpERROR, EvMdINTR, func_804009D4);
    if (D_80409284 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 191);
        func_8007BEF0(1);
    }
    D_80409288 = func_8007C998(HwCARD, EvSpTIMOUT, EvMdINTR, func_804009E8);
    if (D_80409288 == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 193);
        func_8007BEF0(1);
    }
    D_8040928C = func_8007C998(HwCARD, EvSpNEW, EvMdINTR, func_804009FC);
    if (D_8040928C == -1)
    {
        func_8007C040(D_80400004, D_8040002C, 195);
        func_8007BEF0(1);
    }
    func_8007CA48();
    if (func_8007C9D8(D_80409270) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 207);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_80409274) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 209);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_80409278) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 211);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_8040927C) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 213);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_80409280) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 215);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_80409284) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 217);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_80409288) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 219);
        func_8007BEF0(1);
    }
    if (func_8007C9D8(D_8040928C) == 0)
    {
        func_8007C040(D_80400004, D_8040002C, 221);
        func_8007BEF0(1);
    }
}

/* The `card_first_flg` site. This opens with
   `printf("%s = %d\n", "card_first_flg", D_8013BB6C)` and then tests and sets
   that same word, so D_8013BB6C is this link's `card_first_flg` and the live
   start_card_server's D_80084130 is the retail link's -- which is why that one is
   now spelled `card_first_flg[0]` (include/structs.h). Nothing else in either
   block names a global. */
void func_80400E34(void)
{
    func_8007C040(D_80400038, D_80400044, D_8013BB6C);
    if (D_8013BB6C == 0)
    {
        D_8013BB6C = 1;
        func_804086DC(1);
        func_80408730();
        func_8007C958();
        func_8007CB08(0);
        D_8009E390 = 0;
        D_8009E394 = 0;
        func_8007BFE0((u8 *)D_8009DDD8, 0x280);
    }
    func_80400A10();
    D_804094F0[0] = 0;
    D_804094F0[1] = 0;
    D_804094F8[0] = 0;
    D_804094F8[1] = 0;
    D_804094E8 = 0;
}

/* Eight assert sites, and two things had to change beyond the symbol renames.
 *
 * DEVICE: the empty volatile asm at the top of each assert body, worth one
 * word per site -- eight here, 114 -> 122. It emits nothing. What it does is
 * stop reorg stealing the body's first instruction into the `bnez`'s delay
 * slot: this block leaves that slot empty and the live build fills it, and an
 * asm_operands insn is not something reorg will move. The underlying cause is
 * NOT established -- both builds call an abort the compiler knows nothing
 * about, and declaring func_8007BEF0 `noreturn` changes neither one -- so this
 * reproduces an observed scheduling difference rather than explaining it. It
 * is also not general: func_80400A10 above fills these slots from the other
 * side at seven of its sixteen sites, where an unconditional barrier is wrong.
 *
 * The LINE NUMBERS are this block's, not the live source's: 253..267 against
 * 263..277, an offset of -10 that is per function (func_80400A10's is -5), so
 * they come from `tools/assertmap.py --sites`, which reads them out of the
 * block. A line number the live source happens to share would be luck. */
void func_80400EF8(void)
{
    func_8007CA38();
    if (func_8007C9A8(D_80409270) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 253);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_80409274) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 255);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_80409278) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 257);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_8040927C) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 259);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_80409280) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 261);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_80409284) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 263);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_80409288) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 265);
        func_8007BEF0(1);
    }
    if (func_8007C9A8(D_8040928C) == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_80400004, D_8040002C, 267);
        func_8007BEF0(1);
    }
    func_8007CA48();
}

s32 func_804010E0(void)
{
    return D_804094E8;
}

/* BOTH of these state machines turn on the SAME two devices, and the second
 * is the one that took them off 2 differing after three sessions.
 *
 * ONE: `r = 5;` ahead of the `if (st == 3)`. The target has it in the beq's
 * delay slot, and reorg can only put it there from BEFORE the branch --
 * `fill_simple_delay_slots`'s forward "insns from beyond" block takes nothing
 * for a conditional jump (the resource test it guards is `target == 0`,
 * reorg.c:3055) and its target-steal needs `JUMP_LABEL (trial) == target`
 * (reorg.c:3104), which the following `j` is not.
 *
 * TWO: an asm-only `def:` block BETWEEN the three arm and `out:`, which the
 * chain's fall-through jumps to instead of `out:`. It settles the other two
 * jumps at once. Without it the three arm sits directly above `out:`, its
 * `goto out` is deleted as a jump to the next insn, and the arm falls through
 * -- so `out:`'s `addu $v0,$s1,$zero` is never duplicated and the function is
 * two words short. With it the arm really does end in a `j`, and reorg takes
 * `out:`'s insn into its slot and retargets past it, which is the target's
 * `j .L80401390` / `addu $v0,$s1,$zero` beside a separate one-insn `out:`.
 * The default's own `j` then lands on the `def:` label -- same address, since
 * the asm emits nothing -- where the insn at the label is an asm and so is
 * not eligible for the slot, leaving the nop the target has. It also breaks
 * the cross-jump that used to merge the three arm's `D_804094E8 = 0` tail
 * onto an identical one, so the separate barrier that was doing that job is
 * not needed.
 *
 * `D_8009E390_a` is the third device, in csrc/main_twin.h: read through the
 * INCOMPLETE array alias the symbol has no size record, costs two insns, and
 * the `t++` arms come out as the target's bare `addiu $v1,$v1,4` instead of a
 * folded `D_8009E390+4`. */
s32 func_804010F0(void)
{
    s32 st = D_804094E8;
    s32 r = 0;
    s32 *t;
    s32 *u;

    if (st == 0)
    {
        goto out;
    }
    if (st == 2)
    {
        goto out;
    }
    if ((u32)st >= 3)
    {
        goto high;
    }
    if (st == 1)
    {
        goto one;
    }
    r = 5;
    goto out;
high:
    r = 5;
    if (st == 3)
    {
        goto three;
    }
    goto def;
one:
    t = D_8009E390_a;
    if (D_804094EC != 0)
    {
        t++;
    }
    if (t[0] != 0)
    {
        r = 1;
        D_804094E8 = 0;
    }
    else if ((func_80408644(0) & 1) != 0 || (func_80408644(1) & 1) != 0)
    {
        func_804014E4();
        while (func_80408674(D_804094EC) == 0)
        {
        }
        D_80409500 = 0;
        D_804094E8 = 3;
    }
    goto out;
three:
    r = 2;
    u = D_8009E390_a;
    if (D_804094EC != 0)
    {
        u++;
    }
    u[0] = 1;
    D_804094E8 = 0;
    goto out;
def:
    __asm__("");
out:
    return r;
}

s32 func_8040122C(void)
{
    s32 r = 0;

    if (D_804094E8 == 2)
    {
        func_804014E4();
        if (func_80408674(D_804094EC) == 0)
        {
            D_804094E8 = 0;
            r = 5;
        }
        else
        {
            D_80409500 = 0;
            D_804094E8 = D_804094E8 + 1;
        }
    }
    return r;
}

/* The same two devices as func_804010F0 above. */
s32 func_804012B0(void)
{
    s32 st = D_804094E8;
    s32 r = 0;
    s32 *t;

    if (st == 0)
    {
        goto out;
    }
    if (st == 1)
    {
        goto one;
    }
    r = 5;
    if (st == 3)
    {
        goto three;
    }
    goto def;
one:
    func_80401578();
    if (func_80408684(D_804094EC) == 0)
    {
        goto fail;
    }
    if ((func_80408644(D_804094EC != 0) & 4) == 0)
    {
        goto fail;
    }
    D_80409500 = st;
    D_804094E8 = D_804094E8 + 1;
    goto out;
fail:
    D_804094E8 = 0;
    r = 5;
    goto out;
three:
    r = 3;
    t = D_8009E390_a;
    if (D_804094EC != 0)
    {
        t++;
    }
    t[0] = 0;
    D_804094E8 = 0;
    goto out;
def:
    __asm__("");
out:
    return r;
}

s32 func_804013A8(void)
{
    s32 *p = &D_804094E8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 4;
        *p = 0;
    }
    return r;
}

s32 func_804013D0(void)
{
    s32 *p = &D_804094E8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 5;
        *p = 0;
    }
    return r;
}

s32 func_804013F8(s32 D_804094EC)
{
    func_804014E4();
    do
    {
    } while (func_80408664(D_804094EC) == 0);
    D_80409500 = 0;
    D_804094E8 = 1;
    return 1;
}

s32 func_80401444(void)
{
    s32 r = 0;

    if (func_8007C9C8(D_80409270) != 0)
    {
        r = 1;
    }
    else if (func_8007C9C8(D_80409274) != 0)
    {
        r = 2;
    }
    else if (func_8007C9C8(D_80409278) != 0)
    {
        r = 3;
    }
    else if (func_8007C9C8(D_8040927C) != 0)
    {
        r = 4;
    }
    return r;
}

void func_804014E4(void)
{
    func_8007C9C8(D_80409270);
    func_8007C9C8(D_80409274);
    func_8007C9C8(D_80409278);
    func_8007C9C8(D_8040927C);
}

CARD_STATE func_8040153C(void)
{
    CARD_STATE state = D_80409504;
    CARD_STATE old_state = CARD_STATE_OK;

    if (state != CARD_STATE_OK)
    {
        old_state = state;
        func_80401578();
    }
    return old_state;
}

void func_80401578(void)
{
    D_80409504 = CARD_STATE_OK;
    func_8007C9C8(D_80409280);
    func_8007C9C8(D_80409284);
    func_8007C9C8(D_80409288);
    func_8007C9C8(D_8040928C);
}

s32 func_804015D8(void)
{
    UnkFnTab5 a = D_80400054;
    UnkFnTab5 b = D_80400068;
    s32 r = 0;
    s32 st = D_80409500;

    if (st == 0)
    {
        r = func_80401444();
        if (r != 0)
        {
            r = a.f[r]();
        }
    }
    else if (st == 1)
    {
        r = func_8040153C();
        if (r != 0)
        {
            r = b.f[r]();
        }
    }
    return r;
}

/* Live func_8002168C. Three revision differences: case 3's threshold is 4
 * here and 0xB there, none of the three cases zeroes D_80409250/D_80409258
 * after setting the state, and cases 4/5 have no counterpart to the live
 * `(&D_80084118)[i] = 0;`. */
s32 func_804016D0(void)
{
    s32 st = func_804015D8();
    s32 i = func_804019A4();

    if (st != 0)
    {
        if (D_80409258[i] == st)
        {
            D_80409250[i]++;
        }
        else
        {
            D_80409250[i] = 0;
        }
        D_80409258[i] = st;
        switch (st)
        {
        case 1:
        case 2:
            D_804094F0[i] = st;
            break;
        case 3:
            if (D_80409250[i] >= 4)
            {
                D_804094F0[i] = st;
            }
            break;
        case 4:
        case 5:
            if (D_80409250[i] >= 0xB)
            {
                D_804094F0[i] = st;
            }
            break;
        }
        func_804008A0(i);
    }
    return st;
}

/* Live func_800217D4, with the same two revision differences as
 * func_804016D0 above: neither arm zeroes D_80409260/D_80409268 after
 * setting the state, and there is no counterpart to the live
 * `(&D_80084118)[i] = 0;`. */
s32 *func_804017EC(void)
{
    s32 v = func_80401444();
    s32 i;

    if (v != 0)
    {
        i = func_804019A4();
        if (D_80409268[i] == v)
        {
            D_80409260[i]++;
        }
        else
        {
            D_80409260[i] = 0;
        }
        D_80409268[i] = v;
        if (v == 1 || v == 4)
        {
            if (D_80409260[i] >= 4)
            {
                D_804094F8[i] = 1;
            }
        }
        else
        {
            if (D_80409260[i] >= 4)
            {
                D_804094F8[i] = 0;
            }
        }
        func_80400948();
    }
    return D_804094F8;
}

/* print_card_status, which is EMPTY in the live block and prints six lines
 * here. That is the clearest single piece of evidence that this block is a
 * debug build of the same source. */
void func_804018FC(void)
{
    func_8003FA78(D_80400090, D_804094EC);
    func_8003FA78(D_8040009C, D_804094E8);
    func_8003FA78(D_804000A4, D_80409500);
    func_8003FA78(D_804000B0, D_80409504);
    func_8003FA78(D_804000BC, func_80408644(0));
    func_8003FA78(D_804000D0, func_80408644(1));
}

int func_804019A4(void)
{
    return D_804094EC != 0;
}

/* NEAR MISS: one insertion: the conditional +4 is re-materialised as SYM+4 instead of added to the live register
   Source in docs/near-misses/func_804019B4.c. */
/* Live count_card_files. TWO RENAMES THE SYMMAP DID NOT HAVE --
 * MEM_CARD_NAMES -> D_80408AA0 and D_80020084 -> D_804000F4 -- AND THEN
 * ONE DECLARED SIZE. See csrc/main_twin.h on D_80408AA0: at [2] the array
 * is 8 bytes, takes the -G8 small-data path, and the conditional `+4` gets
 * folded into a fresh SYM+4 address where the target adds 4 to the live
 * register -- one inserted word and 28 positional differences behind it.
 * An incomplete declaration, or any size over 8, matches exactly.
 *
 * REJECTED on the way: a byte-cast increment, `+ 1`, and dropping the
 * `&...[0]` (all 40 words); and an index local, which gets the length
 * right but goes BRANCHLESS where the target branches. */
s32 func_804019B4(void)
{
    u8 name[0x20];
    s32 n = 0;
    struct DIRENTRY *e = D_80409290;
    const char **mem_card_name = &D_80408AA0[0];

    if (D_804094EC != 0)
    {
        mem_card_name++;
    }
    func_8007BF80(name, *mem_card_name);
    func_8007BF50(name, (u8 *)D_804000F4);
    if (func_8007CAB8(name, e) != NULL)
    {
        do
        {
            n++;
            e = (struct DIRENTRY *)((u8 *)e + 0x28);
        } while (func_8007CAC8(e) == e);
    }
    return n;
}

s32 func_80401A50(s32 n)
{
    s32(*p)[10] = (s32(*)[10])D_80409290;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < n; i++)
    {
        sum += p[i][6];
    }
    sum = sum / 0x2000;
    return 0xF - sum;
}

s32 func_80401AA4(s32 n, u8 *s)
{
    s32 i = 0;
    s32 ret = 0;
    u8 *tbl = (u8 *)D_80409290;
    u8 *row;
    /* DEVICE: 8 bytes of unread local. The target builds a 0x8 frame where
       this body is frameless, and every other word matches once it shifts --
       no extra calls, no extra registers saved, just the prologue, the
       epilogue adjust and the displacement. AND THIS ONE IS NOT THE COMPILER:
       func_804025F8 looked identical and turned out to be FSF 2.7.2 reserving
       8 bytes that the Cygnus snapshot does not, but BOTH sides here are FSF
       2.7.2, so the twin's source really does have a local this one lost. A
       scalar is eliminated; it has to be an aggregate, and 8 bytes of one.
       `s32 unused[1]` is 32 words, `u8 buf[8]` also matches. */
    s32 unused[2];

    if (n > 0)
    {
        row = tbl;
        do
        {
            s32 bad = 0;
            s32 j = 0;

            do
            {
                u8 c = s[j];

                if (c == 0)
                {
                    break;
                }
                if (c != row[j])
                {
                    bad++;
                    break;
                }
                j++;
            } while (j < 0x15);
            if (bad == 0)
            {
                ret = 1;
                break;
            }
            row += 0x28;
        } while (++i < n);
    }
    return ret;
}

/* A `cnt` site: the $s1 the VSync poll loop increments is printed as
   `printf("%s = %d\n", "cnt", cnt)` once the loop ends, so name it `cnt` when
   this lands. `cnt` is TWIN-ONLY -- the live format_card has no counter at
   all, because the print was its only use -- so there is nothing to rename in
   the live unit. Same story in func_804022C8. */
/* Live format_card, with a VSync COUNTER the live one does not keep and a
 * printf of it after the wait loop. */
s32 func_80401B30(s32 card_slot)
{
    const char **mem_card;
    s32 n = 0;
    CARD_STATE r;

    func_80408654(0);
    func_80408654(1);
    func_80401578();
    mem_card = D_80408AA0;
    if (card_slot != 0)
    {
        mem_card++;
    }
    func_8007CAA8(*mem_card);
    while ((r = func_8040153C()) == 0)
    {
        n++;
        func_800702F0(0);
    }
    func_8007C040(D_80400038, D_804000F8, n);
    if (r != 1)
    {
        if (r == 3)
        {
            r = 4;
        }
        else
        {
            r = 5;
        }
    }
    return r;
}

void func_80401BF4(char *out, s32 file_id)
{
    const char **mem_card_name = &D_80408AA0[0];

    if (D_804094EC != 0)
    {
        mem_card_name++;
    }
    func_8007BF80(out, *mem_card_name);
    func_8007BF50(out, D_80408AA8);
    func_8007BF50(out, D_80408AB8[file_id]);
}

/* Live read_card_file, and NOT its two devices. The shifted length is its
 * own local here -- assigning back into `n` as the live source does swaps
 * $s1 and $s2 between `n` and `off` -- and the result is a branch, not the
 * live `r = read(...) == n;`, which comes out branchless and a word short.
 * The live source's two nested degenerate loops around `return r` are wrong
 * here: they cost 17 differing. */
s32 func_80401C70(u8 *name, u8 *buf, s32 n, s32 off)
{
    s32 r = 0;
    s32 fd;

    fd = func_8007CA58(name, 1);
    if (fd != -1)
    {
        s32 len = n << 7;

        if (off == 0 || func_8007CA68(fd, off << 7, 0) != -1)
        {
            if (func_8007CA78(fd, buf, len) == len)
            {
                r = 1;
            }
        }
        func_8007CA98(fd);
    }
    return r;
}

/* Live write_card_file. SINGLE EXIT, not the live source's two early
 * `return ok;` -- with those, gcc proves `ok` is still 0 at each and
 * materialises the constant, where the target reads it back out of $s3. A
 * `goto out` to one `return ok;` makes both exits share the tail and it
 * matches exactly. (Writing `return 0;` at both sites is the same 3
 * differing, because the fold is what the source cannot reach.) */
s32 func_80401D28(u8 *name, u8 *buf, s32 n, s32 off, s32 skip)
{
    s32 ok = 0;
    s32 fd;

    if (skip == 0)
    {
        fd = func_8007CA58(name, 0x30200);
        if (fd == -1)
        {
            goto out;
        }
        func_8007CA98(fd);
    }
    fd = func_8007CA58(name, 2);
    if (fd == -1)
    {
        goto out;
    }
    if (off == 0 || func_8007CA68(fd, off << 7, 0) != -1)
    {
        if (func_8007CA88(fd, buf, n << 7) == (n << 7))
        {
            ok = 1;
        }
    }
    func_8007CA98(fd);
out:
    return ok;
}

s32 func_80401E14(s32 i)
{
    return D_8009DDD8[i][0];
}

/* Live load_save_slot_info, with the row pointer NOT kept in a local: every
 * one of the three D_8009DDD8 references re-materialises the base, which is
 * what the two assembler-expanded stores and the re-loaded $a1 say. */
/* Live load_save_slot_info. The row is addressed by a BYTE OFFSET, not a
 * pointer local: the target's two stores are `sw $v0, D_8009DDD8($s0)` with
 * $s0 holding idx << 7, and a `s32 *e = D_8009DDD8[idx];` local costs the add
 * and comes out at 40 words. */
s32 func_80401E2C(s32 idx)
{
    char name[0x20];
    s32 r = 1;
    s32 **t;
    s32 off;
    s32 v;
    s32 slot;

    func_80401BF4(name, idx);
    off = idx * 0x80;
    slot = D_804094EC;
    /* DEVICE: one marker, worth a word (43 -> 42, 34 differing -> 0). The
       block before the branch is three insns -- `la $4,D_80136140`,
       `lw $2,D_804094EC` and `sll $16,$16,7`. Written plainly gcc sinks the
       load to its comparison, so the `la` is first in LUID order, sched2 ties
       on priority and keeps that order, the load's delay has nothing in it
       and gcc emits its own `#nop`. The target is the same four insns with
       the first two swapped, the la's two assembler-expanded words covering
       the delay. Reading the flag into a local does not move it on its own --
       gcc sinks it again -- so the marker is what pins the load ahead of the
       `la`, and then the scheduler fills the delay by itself.
       Legal spellings ruled out: no marker 43 words, 34 differing; the marker
       AFTER the `t =` 43/34 (too late to change the order); `volatile` 43/34
       (it is a scheduling barrier, so the la cannot move into the delay
       either); `"=r"(slot)`/`"0"(slot)` also matches, and so does moving the
       `off` multiply after the marker.
       Warns under -Wall? No. */
    __asm__("" : : "r"(slot));
    t = (s32 **)D_80136140;
    if (slot != 0)
    {
        t++;
    }
    v = func_80401AA4((s32)*t, (u8 *)(name + 5));
    *(s32 *)((u8 *)D_8009DDD8 + off) = v;
    if (v != 0)
    {
        r = func_80401C70((u8 *)name, (u8 *)D_8009DDD8 + off, 1, 4);
        *(s32 *)((u8 *)D_8009DDD8 + off) = 1;
    }
    return r;
}

void func_80401ED4(s32 idx, u8 *src)
{
    s32 *row;

    /* THE LIVE SOURCE'S DEVICE IS THE OTHER WAY ROUND HERE, and the reason
       is the compiler. Live store_save_slot_info computes the row pointer
       for the SECOND access in its own statement, because as a MEM address
       4.1's memory_address forces the symbol into a register before the
       index shift (lui/addiu/sll) and only a pointer assignment gets the
       target's sll/lui/addiu order. Without split addresses there is a third
       option that build did not have: a MEM address FOLDS, and the second
       access comes out as `lui $at` / `addu $at,$at,$v1` /
       `sw %lo(SYM)($at)` -- three words, not four. So here it is the FIRST
       access that needs the pointer local and the second that must be
       written as the plain 2-D index. Swapping the two spellings takes it
       from 4 differing to 0. */
    row = D_8009DDD8[idx];
    *(UnkRow *)row = *(UnkRow *)(src + 0x200);
    D_8009DDD8[idx][0] = 1;
}

/* Prints `(int)gsw->check_sum` after storing it, so the parameter is `gsw` and
   the word at +0x204 is its `check_sum` member -- GSW_CHECK_SUM in csrc/main.h,
   now used by the live set_gsw_checksum/check_gsw_checksum. */
/* Live set_gsw_checksum, with FOUR more words in the run and a printf after. */
void func_80401F98(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 n = 0x1781;

    do
    {
        sum ^= *q++;
        n--;
    } while (n >= 0);
    gsw[0x81] = sum;
    func_8007C040(D_80400114, D_80400120);
}

/* Prints both halves of the comparison: `c` (the xor accumulator) and then
   `(int)gsw->check_sum`. The live check_gsw_checksum's accumulator is now `c`. */
/* Live check_gsw_checksum, with the run four words longer and both the
 * computed and the stored sum printed before the comparison. */
s32 func_80401FEC(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 i = 0;

    for (i = 0; i < 0x1782; i++)
    {
        sum ^= *q;
        q++;
    }
    func_8007C040(D_80400114, D_80400134, sum);
    func_8007C040(D_80400114, D_80400120, gsw[0x81]);
    return gsw[0x81] == sum;
}

/* Live write_save_file, without its `*(s32 *)(buf + 0x204) = 0;` and without
 * the refresh_card_info call. */
s32 func_80402084(s32 idx, u8 *buf)
{
    char nameA[0x20];
    char nameB[0x20];
    s32 r;

    func_80401BF4(nameA, idx);
    func_80401BF4(nameB, 5);
    if (D_8009DDD8[idx][0] != 0)
    {
        func_8007CAD8(nameA, nameB);
    }
    func_80401F98((s32 *)buf);
    r = func_80401D28((u8 *)nameB, buf, 0xC0, 0, D_8009DDD8[idx][0]);
    if (r != 0)
    {
        func_80401ED4(idx, buf);
    }
    func_8007CAD8(nameB, nameA);
    return r;
}

/* Live read_save_file. TWO arguments here, no `flag` arm at all, and a
 * RETURN: the live one returns nothing. The checksum failure prints. */
s32 func_80402154(s32 idx, u8 *buf)
{
    char name[0x20];
    s32 r;

    func_80401BF4(name, idx);
    r = func_80401C70((u8 *)name, buf, 0xC0, 0);
    if (r != 0)
    {
        r = func_80401FEC((s32 *)buf);
        if (r != 0)
        {
            r = func_80401D28((u8 *)name, (u8 *)D_8009DDD8[idx], 1, 4,
                              D_8009DDD8[idx][0]);
        }
        else
        {
            func_8007BF18(D_80400138);
        }
    }
    return r;
}

void func_80402214(void)
{
    s32 i = D_804094EC != 0;
    s32 n;

    n = func_804019B4();
    D_80136140[i] = n;
    D_8009EB58[i] = func_80401A50(n);
}

s32 func_80402268(void)
{
    s32 *p = D_8009EB58;

    if (D_804094EC != 0)
    {
        p++;
    }
    return *p;
}

/* Live count_used_save_slots. TWO SPELLING CHOICES, both forced by the
 * listing. The loop variable is a BYTE OFFSET stepping 0x80 against a limit
 * of 0x280, not an index with a `sll 7` -- the load is the folded
 * `lui $at` / `addu $at,$at,$v1` / `lw %lo(SYM)($at)`. And the value is read
 * into a local with the increment BEFORE the test, which is what frees the
 * `beqz`'s delay slot for that increment; with the increment after the `if`,
 * reorg finds `count++` in the fall-through first, cannot use it, and leaves
 * a nop. Worth one word. */
s32 func_80402290(void)
{
    s32 count = 0;
    s32 off = 0;

    do
    {
        s32 v = *(s32 *)((u8 *)D_8009DDD8 + off);

        off += 0x80;
        if (v != 0)
        {
            count++;
        }
    } while (off < 0x280);
    return count;
}

/* The other `cnt` site; see func_80401B30. The live erase_save_file has no
   counter either. */
/* NEAR MISS, 63 of 63 words, 7 differing -- and the park was REBUILT on a
 * worse-counting, better-shaped candidate on 2026-11-12. Every one of the 7
 * is a register NAME: `q` gets $a0 and the flag $a1 where the target has $v0
 * and $a0, because the INDEX pseudo of `D_8009DDD8[idx][0] = 0` holds $v0
 * and gcc emits the `q = w` copy with the declarations, ahead of it. Write
 * that store with a constant index and the whole fixup is byte-exact.
 * Source in docs/near-misses/func_804022C8.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804022C8);

void func_804023C4(void)
{
    func_804022C8(5);
}

/* The flag has to be read into a local and `p` decremented IN PLACE: the
 * target reuses $s0 for both and puts the `addiu $s0,$s0,-0x20` in the
 * beqz's delay slot. A separate `q = p - 8` costs the extra register and
 * leaves the slot empty. */
void func_804023E4(s32 *p)
{
    s32 v;

    func_80047938(p);
    v = ((u16 *)p)[-1];
    p -= 8;
    if (v & 0x2000)
    {
        func_804024A4(p);
    }
}

/* The FIRST argument is never read -- the allocator is called with a literal
 * zero -- and the constructor comes out of D_80408AD0 by index. */
s32 *func_80402428(s32 unused, s32 a, s32 n)
{
    s32 *p = func_8003C480(0);

    if (p != 0)
    {
        p[8] = a;
        p[9] = n;
        D_80408AD0[n](p);
        p[4] = (s32)func_804023E4;
    }
    return p;
}

void func_804024A4(s32 *p)
{
    if (p != 0)
    {
        *(s32 *)p[8] = func_800484A4(p[9] + 6, (s32 *)p[8]);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

/* NEAR MISS, 44 of 44 words, 16 differing: gcc duplicates the inner scan's
   zero test at the bottom of the loop and then undoes the pointer increment,
   where the target keeps one test at each end.
   Source in docs/near-misses/func_80402508.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402508);

/* Live func_80022300, with three constants changed: n * 12 where it has
 * n << 3, 0x15 where it has 0x13, and 0x800000 where it has 0xA04010. */
u8 *func_804025B8(s32 *a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xA] = (n * 12) + 4;
    p[0xB] = 0x15;
    *(s32 *)(p + 4) = 0x800000;
    p[0] |= 0x80;
    return p + 0xC;
}

void func_804025F8(u8 *p, s32 idx, s32 n)
{
    s32 i;

    /* The frame is 0x28 here against the live build's 0x20, with the same
       four saves 8 bytes higher and every instruction otherwise identical.
       That used to need a DEVICE -- `s32 unused[2]`, eight bytes of locals
       the function never reads -- on the theory that the debug source had an
       unused local. It did not: FSF 2.7.2 simply reserves those 8 bytes and
       the Cygnus snapshot does not, so the device came out when this unit
       moved to tools/psx272gcc. See the header. */
    func_80051DE4(p, 1);
    p += idx * 12;
    for (i = 0; i < n; i++)
    {
        func_800514A8(p, 0);
        p += 12;
    }
}

/* Live func_800223B4, in raw word indices because every offset moved: ptrs
 * at c[0x1A3] against the live group's 563, the pair at c[0x1A8] (ptrs[5],
 * not ptrs[6]), the scratch at c[0x4B] against p[57], and x * 6 where the
 * live one has x * 4. `sx` still has to be the FIRST local: the target
 * computes it at word 3, ahead of the $s4 save. */
void func_80402670(s32 *c, s32 x, s32 y, s32 n, s32 z)
{
    s32 sx = x * 6;
    UnkCardPair *e = (UnkCardPair *)c[0x1A8];
    u8 *q = (u8 *)&c[0x4B];

    e->unk_04->unk_08 = sx - (c[6] / 2) + 0xA2;
    e->unk_04->unk_0A = (n * 17) - (c[7] / 2) + 0x7A;
    func_804025B8(c, q, y);
    e->unk_00 = (s32)q;
    func_804025F8((u8 *)((UnkCardPair *)c[n + 0x1A3])->unk_00, z, y);
}

/* Live func_80022488. The strings are a SEPARATE table here, D_80408B2C,
 * four per record (the index is `sll 4`), where the live build has them in
 * the same 24-byte UnkStrSet as the four bytes. */
void func_8040274C(s32 *p)
{
    s32 i = p[2];
    s32 n = D_80408ADC[i].unk_01 - 1;
    s32 x;
    s32 y;
    s32 z;

    if (D_80408ADC[i].unk_02 != 0)
    {
        func_80402508(D_80408B2C[i][n], p[0], &x, &y, &z);
        func_80402670(p, x, y, n, z);
    }
}

/* The third of the input-word handlers, after func_80405FC4 and
 * func_80406DA0. Its masks are 0xA000/0x8000/0x2000 where theirs are
 * 0x5000/0x1000/0x4000, its 0x40 arm does nothing at all, its counter is
 * p[4] with a threshold of 3 and NO decrement in the over-threshold arm,
 * and the step comes out of the message table. */
void func_804027F8(s32 *p)
{
    s32 dir = 0;

    if (D_801379A8.unk_00 != 0)
    {
        if ((D_801379B0.unk_00 & 0x40) == 0)
        {
            if (D_801379B0.unk_00 & 0x20)
            {
                func_80063FF8(0x514);
                if (p[8] != 0)
                {
                    *(s32 *)p[8] = func_8040343C(1);
                }
                else
                {
                    func_8040343C(1);
                }
            }
            else if (D_801379A8.unk_00 & 0xA000)
            {
                if (D_801379B0.unk_00 & 0xA000)
                {
                    p[4] = 0;
                    if (D_801379B0.unk_00 & 0x8000)
                    {
                        dir = -1;
                    }
                    else if (D_801379B0.unk_00 & 0x2000)
                    {
                        dir = 1;
                    }
                }
                else if (p[4] >= 3)
                {
                    if (D_801379A8.unk_00 & 0x8000)
                    {
                        dir = -1;
                    }
                    else if (D_801379A8.unk_00 & 0x2000)
                    {
                        dir = 1;
                    }
                }
                else
                {
                    p[4]++;
                }
            }
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            p[0] = func_80056450(p[0], dir, D_80408ADC[p[2]].unk_02);
            func_8040274C(p);
        }
    }
}

void func_8040293C(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_8040343C(0);
    }
}

void func_80402974(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -72;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

u8 *func_80402A1C(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    u32 hh;
    u32 bh;
    s32 g;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_80400544;
    i = 0;
    w = D_80408ADC[q[2]].unk_00 * 12;
    hh = D_80408ADC[q[2]].unk_01;
    h = hh * 17;
    q[6] = w + 8;
    q[7] = h + 8;
    for (; i < 4; i++)
    {
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x101010;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = (s32)(w + 0xA) >> 1;
        bh = h + 0xA;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x101010;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = D_80408C6C;
        g = 0x30BFC0;
        /* DEVICE: one marker, worth 68 words (68 -> 0). `"0"(g)` is a second
           set of g inside the loop, and `"r"(p)` is a VARYING input, so the
           asm's set is not invariant and `consec_sets_invariant_p` (loop.c)
           refuses the pair: the lui/ori stays in the loop, where the target
           has it. This is the twin of the same device in func_80402A1C's live
           counterpart func_80022774, with one difference that matters -- there
           the varying input is `i`, here it is `p`. `i` cannot do the job in
           this build: it has no non-giv use, so loop.c's strength reduction
           eliminates it and rewrites `i < 4` as the buf pointer against
           `buf + 4` (`slt $t1, $t8`), which is what the target does. Naming
           `i` in the marker is the one spelling that keeps it alive, and that
           costs the elimination -- 104 words, 74 differing.
           `"r"(bh)` is the live device's (b)/(c) jobs and does both here too:
           it is the last use of `h + 0xA`, so force_movables does not link the
           `srl` to the `addiu` and the shift stays in the loop while the addiu
           hoists; and it is bh's third reference, which fixes its register.
           Legal spellings ruled out: no marker 68; `"0"(g), "r"(bh)` 67 (bh is
           loop-invariant, so the asm is invariant and both insns hoist);
           `"0"(g), "r"(p)` 24, with or without `volatile`, and the same 24 with
           the shift written inline as `p[0xB] = (h + 0xA) >> 1;`.
           Operand order does not matter. `h = hh * 17;` is free here -- the
           live source needs `(hh << 4) + hh` only because expand_mult's copy
           would lose its hard-register pin, and this build has no pin.
           Warns under -Wall? No -- g is initialised. */
        __asm__("" : "=r"(g) : "0"(g), "r"(p), "r"(bh));
        *(s32 *)(p + 4) = g;
        *(s32 *)(p + 0xC) = g;
        *(s32 *)(p + 0x10) = g;
        *(s32 *)(p + 0x14) = g;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80402BB8(s32 *p)
{
    s32 **q = (s32 **)p[0x1A7];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

/* Live func_80022934. FOUR entries here, not five; the buffer stride is
 * 0x108 from a base of 0x234 where the live one is 0x180 from 0x114; the
 * strings come from the separate D_80408B2C table. */
void func_80402BE0(s32 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[i + 0x1A3];

        e->unk_04->unk_08 = 0x124 - (p[6] / 2);
        e->unk_04->unk_0A = (i * 17) - (p[7] / 2) + 0xFC;
        e->unk_00 = (s32)func_80051B50((u8 *)p + 0x234 + i * 0x108,
                                       D_80408B2C[p[2]][i], 1);
    }
}

/* Live func_80022A30, whose seven-statement preheader device carries over --
 * see that function's note in csrc/main/card_mes.c for why the `h1 = w1` copy
 * survives. Two differences here: the shared word is 0x101010 where the live
 * one has 0x104058, and the SECOND height is not a copy but a shift the
 * target recomputes every iteration. The marker in the loop is what keeps it
 * there. */
u8 *func_80402CD4(u8 *p, s32 *q)
{
    u8 buf[4];
    u8 w1;
    u8 h1;
    u8 c;
    s32 d;
    u8 w2;
    u32 e;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_80400544;
    i = 0;
    w1 = 5;
    h1 = w1;
    c = 0x48;
    d = 0x101010;
    w2 = 6;
    e = 0xC;
    for (; i < 4; i++)
    {
        p[0xA] = w1;
        p[0xB] = h1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = w2;
        /* DEVICE: one marker, worth 24 words (24 -> 0) and the last word of
           length. `"0"(e)` is a SECOND set of e inside the loop and `"r"(p)`
           is a varying input, so the asm is not invariant, e is not invariant,
           and loop.c leaves `e >> 1` in the loop -- where the target computes
           `srl $v0,$a3,1` every iteration. Written without the marker the
           shift is a movable, loop.c hoists it into the preheader, e is a
           known 0xC in that block and the shift FOLDS to `li 6`: one word
           short and every register after it renamed. Keeping the constant out
           of the loop is the point, so this is the mirror of func_80402A1C's
           marker, which keeps one in.
           `"r"(d)` TWICE is the second job. One set and one use leave e with
           three references against d's two, so e takes $a1 and d takes $a3;
           the target has them the other way round. Two more references for d
           restore the order. Three also match.
           Legal spellings ruled out: no marker 46 words, 24 differing;
           `"0"(e)` alone 48/29; `"r"(e)` alone 48/36; `volatile` with the
           same operands 47/2; dropping `"r"(p)` 48/25 (the asm goes back to
           being invariant); dropping one `"r"(d)`, or replacing it with
           `"r"(c)`, 47/6; and declaring d after e, or e before d, 47/6.
           Warns under -Wall? No. */
        __asm__("" : "=r"(e) : "0"(e), "r"(p), "r"(d), "r"(d));
        p[0xB] = e >> 1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80402D90(s32 *p)
{
    func_80402CD4((u8 *)(p + 9), p);
    func_80402BB8(p);
}

s32 func_80402DC8(s32 a)
{
    return func_80047358(a, D_80408C84, 6);
}

/* Live func_80022B48, the UnkTwinGrp670 instance of the five-way template builder. The
 * group is 0x670 bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80402DF0(UnkTwinGrp670 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80402EA4(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    p[5] = a;
    p[2] = b;
    p[3] = c;
    p[8] = d;
}

void func_80402EBC(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    func_80402EA4(p, a, b, c, d);
    func_80402974((UnkCardPair *)(p + 0x19F), (u32 *)(p + 0x195), (u32 *)(p + 0x199));
    func_80402D90(p);
}

/* The two handlers are the other way round from the live func_80022CD8, and
 * the pointer goes through a LOCAL: the target materialises func_804027F8
 * before the test and overwrites it, where two stores would cost a second
 * %hi pair. Same in-place device as func_804008A0's. */
void func_80402F04(s32 *p)
{
    void (*f)(s32 *);

    func_80402A1C((u8 *)(p + 9), p);
    func_80402BE0(p);
    p[0] = D_80408ADC[p[2]].unk_03;
    func_8040274C(p);
    f = func_804027F8;
    if (p[3] == 2)
    {
        f = func_8040293C;
    }
    p[-4] = (s32)f;
}

/* Live open_card_message, the third of the group-allocating family after
 * func_804057F0 and func_804076C8 -- same assert this build carries and the
 * live one does not. func_80402DF0 takes 6 here, not 7; raw word indices for
 * the group: root at r[0x19F], its unk_0C at r[0x1A2], ptrs at r[0x1A3]. */
s32 func_80402F84(s32 a, s32 id, s32 kind, s32 p)
{
    s32 *q = (s32 *)D_80409508;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_80409508, 0x1E1);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_80400548, D_80400570, 0x299);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_8040343C(r[3] != 2);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x764);
    }
    func_80402DF0((UnkTwinGrp670 *)r, 6);
    q[3] = (s32)(r + 0x19F);
    r[0x1A2] = func_80402DC8((s32)(r + 0x1A3));
    func_80402EBC(r, a, id, kind, p);
    q[4] = (s32)func_80402F04;
    return (s32)q;
}

void func_804030CC(s32 a, s32 p)
{
    func_80402F84(a, 0, 1, p);
}

/* THE ELEVEN ZERO-KIND MESSAGE WRAPPERS, and the twelfth below them, were
 * the reload_cse_regs cluster: one word each, `addu $a3, $a2, $zero` in the
 * live build against `addu $a3, $zero, $zero` here, because 4.1's
 * reload_cse_simplify_set rewrites the second zero argument into a copy of
 * the first. tools/psx272gcc has no such pass and they land untouched.
 *
 * THE MESSAGE ID IS THE ONLY THING THAT PAIRS THEM, and twinalign cannot:
 * every wrapper masks to the same ten words, so its order-preserving DP had
 * the whole run shifted by one. Each id below is read off that function's
 * own listing and agrees with csrc/main_twin.h's map. */
/* Live msg_no_card. */
void func_804030F4(s32 a)
{
    func_80402F84(a, 1, 0, 0);
}

/* Live msg_please_wait, NOT msg_loading -- twinalign paired it with
 * msg_loading, which is the arbitrary-pairing trap this block's wrapper run
 * is famous for, and func_804031E4 already holds message 14. Message 2 with
 * kind 2 is msg_please_wait, and it is the only live wrapper with that pair. */
void func_8040311C(s32 a, s32 p)
{
    func_80402F84(a, 2, 2, p);
}

/* Live msg_overwrite_failed. */
void func_80403144(s32 a)
{
    func_80402F84(a, 8, 0, 0);
}

void func_8040316C(s32 a, s32 p)
{
    func_80402F84(a, 4, 2, p);
}

void func_80403194(s32 a, s32 p)
{
    func_80402F84(a, 5, 1, p);
}

void func_804031BC(s32 a, s32 p)
{
    func_80402F84(a, 6, 2, p);
}

void func_804031E4(s32 a, s32 p)
{
    func_80402F84(a, 14, 2, p);
}

/* Live msg_not_enough_blocks_overwrite. */
void func_8040320C(s32 a)
{
    func_80402F84(a, 9, 0, 0);
}

/* Live msg_not_enough_blocks. */
void func_80403234(s32 a)
{
    func_80402F84(a, 10, 0, 0);
}

/* Live msg_load_failed. */
void func_8040325C(s32 a)
{
    func_80402F84(a, 11, 0, 0);
}

/* Live msg_format_failed. */
void func_80403284(s32 a)
{
    func_80402F84(a, 12, 0, 0);
}

/* Live msg_format_done. */
void func_804032AC(s32 a)
{
    func_80402F84(a, 13, 0, 0);
}

/* Live msg_card_damaged. */
void func_804032D4(s32 a)
{
    func_80402F84(a, 3, 0, 0);
}

/* Live msg_no_saved_game. */
void func_804032FC(s32 a)
{
    func_80402F84(a, 7, 0, 0);
}

void func_80403324(s32 a, s32 p)
{
    func_80402F84(a, 15, 2, p);
}

/* Live msg_copy_failed. */
void func_8040334C(s32 a)
{
    func_80402F84(a, 16, 0, 0);
}

/* Live msg_cannot_overwrite. */
void func_80403374(s32 a)
{
    func_80402F84(a, 17, 0, 0);
}

void func_8040339C(s32 a, s32 p)
{
    func_80402F84(a, 18, 1, p);
}

/* Live msg_delete_failed. */
void func_804033C4(s32 a)
{
    func_80402F84(a, 19, 0, 0);
}

void func_804033EC(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80051FBC(*p[0x1A3]);
        p++;
    } while (++i < 4);
}

s32 func_8040343C(s32 flag)
{
    s32 **q = (s32 **)D_80409528;

    if (func_80047FD8((s32 *)(q - 8)) != 0)
    {
        if (flag != 0)
        {
            D_8040953C[15] |= 0x2000;
        }
        func_804033EC(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
    return (s32)q[0];
}

/* NEAR MISS, 112 of 112 words, 6 differing -- ONE register pair swapped.
 * The body is established (live func_80023258, with the ptrs array 0x18
 * further along and FOUR slots in the loop, not three), and this function is
 * what found --expand-div: see the Makefile. What is left is that the target
 * puts the division's quotient in $v1 and the ptrs pointer in $v0, and gcc
 * does it the other way round, which is a local-alloc tie.
 * Source in docs/near-misses/func_804034D8.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804034D8);

/* Live func_800233D0, with FOUR revision differences and nothing else -- the
 * families in this file's header, all four at once. UnkCardGrp184's `ptrs`
 * array is 0x18 LATER here (ptrs[0] at 0x1B8 against the live 0x1A0), so the
 * indices are written raw off p; the loop runs to 4 where the live source
 * runs to 3, which makes the four groups of four tile exactly where the live
 * ones leave gaps; func_80056F14 is called with 4 rather than 3; and the
 * embedded stride is 0x3C against 0x48, the same 12-bytes-smaller record that
 * func_804037F4 sees. */
void func_80403698(s32 *p)
{
    s32 i;

    ((UnkCardPair *)p[0x6E])->unk_00 = func_80056F14(4);
    ((UnkCardPair *)p[0x6F])->unk_00 = (s32)D_80082FF4;
    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[0x70 + i])->unk_00 = (s32)D_80083060;
        ((UnkCardPair *)p[0x74 + i])->unk_00 = (s32)D_8008306C;
        ((UnkCardPair *)p[0x78 + i])->unk_00 = (s32)(p + 1);
        ((UnkCardPair *)p[0x7C + i])->unk_00 = (s32)((u8 *)p + 0x90 + i * 0x3C);
    }
}

void func_80403738(s32 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[0x78 + i])->unk_04->unk_08 = 0x24;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[0] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[1] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[2] = 0x60;
        ((UnkCardPair *)p[0x70 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[0x74 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[0x7C + i])->unk_04->unk_08 = 0x8F;
    }
}

/* Live func_8002352C, with TWO revision differences and one addressing lever.
 * The stride is 0x3C where the live source steps 0x48, and the bound is 4
 * where it is 3 -- both measured off the listing, and both of the families
 * the header describes: a structure 12 bytes smaller, and a table walked to
 * one more entry. It also needed a sized `D_80408C8C_a[1]` alias under 4.1's
 * compiler, to make the table's address one insn; tools/psx272gcc makes it
 * one insn anyway, so the plain `D_80408C8C` is back. */
void func_804037F4(u8 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        func_80051B50(p + 0x90 + i * 0x3C, D_80408C8C[i], 1);
    }
}

s32 func_80403868(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

void func_804038C8(s32 *p)
{
    s32 i;

    p[32] = 1;
    p[33] = 1;
    func_804037F4((u8 *)p);
    func_80403868((s32)(p + 1), 0x36, 0x10);
    func_80403698(p);
    ((UnkCardPair *)p[110])->unk_04->unk_0A = 0x22;
    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[124 + i])->unk_04->unk_08 = 0x8E;
        ((UnkCardPair *)p[124 + i])->unk_04->unk_0A = (i * 16) + 0x82;
        ((UnkCardPair *)p[112 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[112 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[116 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[116 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[120 + i])->unk_04->unk_08 = 0x24;
        ((UnkCardPair *)p[120 + i])->unk_04->unk_0A = (i * 16) + 0xA;
    }
    func_80403738(p);
}

void func_804039F8(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_80403AA0(s32 a)
{
    return func_80047358(a, D_80408C9C, 0x12);
}

/* Live func_800237EC, the UnkTwinGrp19C instance of the five-way template builder. The
 * group is 0x19C bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80403AC8(UnkTwinGrp19C *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80403B7C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80403B84(s32 *p, s32 v)
{
    func_80403B7C(p, v);
    func_804039F8((UnkCardPair *)(p + 0x6A), (u32 *)(p + 0x60), (u32 *)(p + 0x64));
    func_804038C8(p);
}

/* Live func_800238D4, the same allocator over the UnkTwinGrp19C: root at
 * r[0x6A], unk_0C at r[0x6D], ptrs at r[0x6E]. */
s32 *func_80403BC4(s32 *p)
{
    s32 *q = (s32 *)D_80409C90;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_80409C90, 0x118);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804005A0, D_804005C8, 0x151);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80403D24(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x440);
    }
    func_80403AC8((UnkTwinGrp19C *)r, 0x12);
    q[3] = (s32)(r + 0x6A);
    r[0x6D] = func_80403AA0((s32)(r + 0x6E));
    func_80403B84(r, (s32)p);
    q[4] = (s32)func_804034D8;
    func_804034D8(r);
    return q;
}

void func_80403CD4(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80051FBC(*p[0x7C]);
        p++;
    } while (++i < 4);
}

void func_80403D24(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80403CD4((UnkCardGrp184 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80403D7C(s32 *p, s32 v)
{
    s32 *q;
    s32 t;

    if (p != 0)
    {
        q = p + 8;
        t = q[34];
        q[33] = 0;
        q[32] = 3;
        q[34] = v;
        q[35] = t;
    }
}

void func_80403DA0(UnkCardPair *p, u32 *a, u32 *b, s32 n)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -0xA0;
    p->unk_04->unk_0A = n * 24 - 0x78;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* Live func_80023B18. Four differences, all ONE revision constant: this
 * build's UnkCardGrp240 is 0x84 bytes smaller, so `ptrs[1]`, `ptrs[5]` and
 * `ptrs[7]` sit at 0x1DC, 0x1EC and 0x1F4 where the live struct has 0x260,
 * 0x270 and 0x278, and the embedded buffer the last pair points at is at
 * +0xB0 rather than +0x134. Raw word indices off p, as func_804040C8 and
 * func_80404668 use. */
void func_80403E54(s32 *p)
{
    UnkCardPair *q = (UnkCardPair *)p[0x77];
    s32 k;

    k = func_80402268() >= 3;
    q->unk_00 = (s32)func_80051B50((u8 *)p + 8, D_80408CB0[k], 1);
    q->unk_04->unk_08 = 0xF0;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x7B];
    q->unk_04->unk_08 = 0x70;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_8008432C;
    q = (UnkCardPair *)p[0x7D];
    q->unk_00 = (s32)((u8 *)p + 0xB0);
    q->unk_04->unk_08 = 0xAE;
    q->unk_04->unk_0A = 0x18;
}

s32 func_80403F1C(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

/* Live func_80023C44, with FIVE groups rather than four and the save-slot
 * row reached directly -- `&D_8009DDD8[p[0]][3]` where the live one loads a
 * pointer out of D_80083EA4. */
void func_80403F7C(s32 *p)
{
    u8 buf[8];
    UnkCardPair *e;

    e = (UnkCardPair *)p[0x7B];
    e->unk_04->unk_08 = 0x70;
    e->unk_04->unk_0A = 0x10;
    e->unk_00 = (s32)D_80084158;

    e = (UnkCardPair *)p[0x77];
    e->unk_00 = (s32)func_80051B50((u8 *)(p + 2),
                                   (u8 *)&D_8009DDD8[p[0]][3], 1);
    e->unk_04->unk_08 = 0xF0;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x78];
    e->unk_00 = (s32)func_80051B50((u8 *)p + 0x50, D_804005EC, 1);
    e->unk_04->unk_08 = 0x160;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x7A];
    func_80051804(D_8009DDD8[p[0]][0xB], 4, buf);
    func_80051900(buf);
    e->unk_00 = (s32)func_80051520((u8 *)p + 0x80, buf, 1);
    e->unk_04->unk_08 = 0x13F;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x7D];
    e->unk_00 = (s32)((u8 *)p + 0xB0);
    e->unk_04->unk_08 = 0xAE;
    e->unk_04->unk_0A = 0x18;
}

/* Live func_80023D64. Its UnkCardGrp240 `c->ptrs[N]` reads are spelled as raw
 * word indices here because this build's ptrs array sits 0x21 WORDS lower --
 * ptrs[6] is p[0x7C] against the live 0x9D -- which is the same -0x21 delta
 * func_80404314 and func_804044D8 carry. Four of the constants are revision
 * differences too: 0x100 for 0xFA, 0x181 for 0x178, 0x91 for 0x90, 0x112 for
 * 0x10C; 0x10 and 0x18 are shared. The byte offsets differ independently --
 * 0x5C for 0xA4, 0x128 for 0x1AC. */
void func_804040C8(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x7C];
    q->unk_04->unk_08 = 0x100;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_80084110;
    q = (UnkCardPair *)p[0x79];
    q->unk_00 = (s32)func_80051B50((u8 *)p + 0x5C, D_804005F0, 1);
    q->unk_04->unk_08 = 0x181;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x7E];
    q->unk_00 = (s32)((u8 *)p + 0x128);
    q->unk_04->unk_08 = 0x112;
    q->unk_04->unk_0A = 0x18;
}

void func_80404178(s32 *p)
{
    func_80403F1C((s32)(p + 0x2C), 0x80, 0x18);
    func_80403F1C((s32)(p + 0x4A), 0x2C, 0x14);
    if (D_8009DDD8[p[0]][0] != 0)
    {
        func_80403F7C(p);
        if (D_8009DDD8[p[0]][2] != 0)
        {
            func_804040C8(p);
        }
    }
    else
    {
        func_80403E54(p);
    }
}

void func_80404224(s32 *p)
{
    p[-4] = 0;
}

s32 func_8040422C(s32 a)
{
    return func_80047358(a, D_80408CB8, 0xA);
}

/* Live func_80023EF4, the UnkTwinGrp1BC instance of the five-way template builder. The
 * group is 0x1BC bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80404254(UnkTwinGrp1BC *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80404308(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

void func_80404314(s32 *p, s32 a, s32 b)
{
    func_80404308(p, a, b);
    func_80403DA0((UnkCardPair *)(p + 0x72), (u32 *)(p + 0x68), (u32 *)(p + 0x6C), b);
    func_80404178(p);
}

/* One of five 0x360-byte groups, picked by index, with THREE asserts the
 * live func_80023FF0 has no counterpart for: the index is bounds-checked at
 * both ends before the group is touched. Raw word indices off r: root at
 * r[0x72], its unk_0C at r[0x75], ptrs at r[0x76].
 *
 * It needed func_8007BEF0 declared NORETURN. Without it `n` outranks `r` in
 * global.c's allocno order and the two take $s1 and $s2 the other way round
 * -- 16 differing with every other word already right -- because the three
 * assert arms look like ordinary calls that come back, which lengthens every
 * live range that spans them. */
s32 *func_80404364(s32 a, s32 n)
{
    s32 *q = (s32 *)D_8040A0F0[n];
    s32 *r = q + 8;

    if (n >= 5)
    {
        __asm__ volatile("");
        func_8007C040(D_804005F8, D_80400620, 0x12B);
        func_8007BEF0(1);
    }
    if (n < 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804005F8, D_80400620, 0x12C);
        func_8007BEF0(1);
    }
    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, (u8 *)q, 0xD8);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804005F8, D_80400620, 0x12F);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80404570(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x340);
    }
    func_80404254(r, 0xA);
    q[3] = (s32)(r + 0x72);
    r[0x75] = func_8040422C((s32)(r + 0x76));
    func_80404314(r, a, n);
    q[4] = (s32)func_80404224;
    return q;
}

void func_804044D8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        func_80051FBC(*(s32 *)p[off + 0x77]);
        *(s32 *)p[off + 0x77] = 0;
        off++;
    } while (++i < 3);
    *(s32 *)p[0x7A] = 0;
}

void func_8040454C(s32 **p)
{
    s32 i = 0;

    do
    {
        *p[0x76] = 0;
        p++;
    } while (++i < 10);
}

void func_80404570(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_804044D8((UnkCardGrp240 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

/* Live func_800241D4, with func_80404668's 0x84 struct shift. */
void func_804045C8(s32 *p, s32 flag)
{
    UnkCardPair *r;

    if (p != 0)
    {
        r = (UnkCardPair *)p[0x87];
        p[0x70] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_80083840;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x60;
        r->unk_04->unk_0A = 0x18;
    }
}

/* Live func_80024224, func_804045C8's twin one message over, with the same
 * 0x84 struct shift. */
void func_80404618(s32 *p, s32 flag)
{
    UnkCardPair *r;

    if (p != 0)
    {
        r = (UnkCardPair *)p[0x87];
        p[0x70] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_80083834;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x66;
        r->unk_04->unk_0A = 0x18;
    }
}

/* Live func_80024274. REVISION: this build's UnkCardGrp240 is 0x84 BYTES
 * SMALLER, so `c->root_prim[0]` and `c->ptrs[9]` are at 0x1C0 and 0x21C off
 * `p` where the live struct puts them at 0x244 and 0x2A0. Written as raw word
 * indices off p, as func_804040C8 is, rather than inventing a second struct
 * for a build that only this unit sees. The same 0x84 appears in
 * func_804045C8 and func_80404618. */
void func_80404668(s32 *p)
{
    if (p != 0)
    {
        p[0x70] = 0x606868;
        ((UnkCardPair *)p[0x87])->unk_00 = 0;
    }
}

void func_80404688(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_804044D8(q);
        func_8040454C((s32 **)q);
        func_80404178(q);
    }
}

/* func_8004EEBC, and it references nothing outside its own argument. */
void func_804046C8(u8 *p)
{
    u32 *q = *(u32 **) (p + 4);

    if (q != 0)
    {
        if (q[0] != 0)
        {
            *(u32 *) (p + 0x274) = 0;
            *(u32 *) (p + 0x284) = (u32) (p + 0xF8);
        }
        else
        {
            *(u32 *) (p + 0x274) = (u32) (p + 0x80);
            *(u32 *) (p + 0x284) = 0;
        }
    }
}

/* PARK: 42 words, and the closest spelling reaches 38. The body is
 * func_8004EEFC from csrc/unk_1C540.c, which copies the two GPU-primitive
 * templates into the caller's two primitives.
 *
 * What is settled: this block reads the templates as SEVEN SEPARATE SCALARS,
 * not through two array pointers. A pointer into an array keeps one base
 * register and displaces off it (37 words); seven scalars materialise a
 * `lui`/`%lo` pair each, which is what the listing shows, and that is worth
 * one word. splat named these words individually for the same reason, and the
 * shipped build's D_8002E5D8/D_8002E5E8 are this engine build's D_8002D690..
 * and D_8002D6A0..
 *
 * What is not: the remaining four words are LOAD-DELAY NOPS. This block emits
 * `lui`, `lw`, `nop`, `sw` per word; ours interleaves the next word's `lui`
 * into the delay slot. Tried and did not reach it -- `volatile` on the seven
 * sources (no change at 38, since it constrains the loads and not the stores)
 * and `volatile` on the destinations through a cast (overshoots to 44). What
 * is wanted is a spelling where the next load cannot be hoisted across the
 * previous store. */
void func_80404708(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* func_8004EF90. Its three globals are D_80408CC4, D_80408CCC and D_80408CC8,
 * in the order the body reads them -- the listing's first-use order is the
 * rename. */
u8 *func_804047B0(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CC4;
    a = D_80408CCC;
    b = D_80408CC8;
    p[1] |= 2;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004EFF4, the same shape one template along. */
u8 *func_80404818(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CD0;
    a = D_80408CD8;
    b = D_80408CD4;
    p[1] |= 2;
    p[0] |= 0x40;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004F068, the third of the template builders, with its three globals
 * as scalars for the same reason as func_804047B0's. */
u8 *func_80404888(u8 *p)
{
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80408CDC;
    a = D_80408CE4;
    b = D_80408CE0;
    p[1] |= 2;
    p[0] |= 0x41;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004F0DC, with ONE MORE INDIRECTION. Where the live structure has the
 * seven primitive pointers inline at p+0x220.., this build holds a POINTER
 * there to a table of them: it loads p[0x220], then reads the entry at 0x0,
 * 0x14, 0x18, 0x4, 0x8, 0xC, 0x10 off that -- the same seven fields, one
 * level down. It also RELOADS p[0x220] for every entry, which is not a
 * spelling choice: each block ends with a store through q, which may alias
 * p[0x220], so the load cannot be cached. Writing the dereference out each
 * time is what reproduces that; a local would not.
 */
void func_804048F8(u8 *p)
{
    u8 *q;

    q = (*(u8 ***) (p + 0x220))[0];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 6;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x10;
    *(u8 **) q = p + 8;
    q = (*(u8 ***) (p + 0x220))[5];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x80;
    q = (*(u8 ***) (p + 0x220))[6];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0x68;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0xF8;
    q = (*(u8 ***) (p + 0x220))[1];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[2];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[3];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
    q = (*(u8 ***) (p + 0x220))[4];
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
}

/* func_8004F1E8, with its four primitive blocks ROTATED. The live body runs
 * 0x80/0xFF, then 0x2C with the GetTPage and GetClut pair, then the 0x70/0xE0
 * one, then 0x81 with p[0] = 0x80 inside it. This build runs the 0x81 block
 * FIRST -- without the p[0] = 0x80 -- then the other three in order, and sets
 * p[0] = 0x80 at the end. Reading it off the listing is unambiguous: the
 * entry sets 0x81/0x20/0 before it has touched anything else, and the final
 * `sb $s1, 0x0($s0)` comes after the 0x70/0xE0 stores. Its two calls are
 * GetTPage and GetClut in this engine build, func_80078570 and
 * func_80078640. */
u8 *func_80404A3C(u8 *p)
{
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p += 0xC;
    p[1] = 0x80;
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xFF;
    p[0xB] = 0xFF;
    p += 0xC;
    p[1] = 0x2C;
    *(s16 *) (p + 4) = func_80078570(1, 0, 0x380, 0x100);
    *(s16 *) (p + 6) = func_80078640(0, 0x1F5);
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xA0;
    p[0xB] = 0x70;
    p += 0xC;
    p[8] = 0x70;
    p[9] = 0xE0;
    p[1] = 0x80;
    p[0xA] = 8;
    p[0xB] = 8;
    p[0] = 0x80;
    return p + 0xC;
}

/* func_8004F2BC. Its five calls are this block's func_804047B0, func_80404818,
 * func_80404888, func_80404A3C and func_804048F8, in that order. */
void func_80404B10(u8 *p)
{
    func_804047B0(p + 0x8);
    func_80404818(p + 0x80);
    func_80404888(p + 0xF8);
    func_80404A3C(p + 0x170);
    func_804048F8(p);
}

/* func_80050B7C. */
/* RETURNS: func_80404C84 stores $v0 straight after the call. */
s32 func_80404B5C(s32 a)
{
    return func_80047358(a, D_80408CE8, 0x7);
}

/* It was parked here at 5 differing for a long time. What landed it was the
 * same thing that landed the other six builders: NO `u32 v` local for the
 * first template word, and the three trailing words read through the
 * one-member-struct spelling so that the store and the load conflict. The
 * park's structural findings stand: this block loads the ptrs POINTER from
 * c[0x88] where the live body has a seven-entry inline table, and every
 * field above it is 0x18 lower, which is exactly what a seven-entry table
 * costs when it becomes one pointer. */
/* The seventh of the template-builder family, and the second (after
 * func_804075D0) whose ptrs is a POINTER at c[0x88] rather than an inline
 * array. shared at c[0x81], ents at c[0x89], prims at c[0xA5]. */
void func_80404B84(s32 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        ((UnkCardPair **)c[0x88])[i] = (UnkCardPair *)&c[0x89 + i * 4];
        c[0xA5 + i * 4] = D_8002D690_a[0];
        c[0xA6 + i * 4] = D_8002D690_a[1];
        c[0xA7 + i * 4] = D_8002D690_a[2];
        c[0xA8 + i * 4] = D_8002D690_a[3];
        c[0x8A + i * 4] = (s32)&c[0xA5 + i * 4];
        c[0x8B + i * 4] = (s32)&c[0x81];
    }
    c[0x81] = D_8002D6A0.unk_00;
    c[0x82] = D_8002D6A4.unk_00;
    c[0x83] = D_8002D6A8.unk_00;
}

/* From the RESIDENT engine, not the live overlay: this debug build linked
 * libcard's callers statically where retail calls them in the executable, so
 * these bodies come from csrc/unk_1C540.c. Every rename below was read off
 * this block's own listing positionally -- the Nth call in the resident body
 * is the Nth jal here -- which is firmer than the similarity match that found
 * the counterpart. func_8004F3D0. */
void func_80404C3C(u32 *p, u32 v)
{
    *p = v;
}

/* func_8004F3D8. */
void func_80404C44(u8 *p, u32 v)
{
    func_80404C3C((u32 *) p, v);
    func_80404708(p + 0x210, (u32 *) (p + 0x1E8), (u32 *) (p + 0x1F8));
    func_80404B10(p);
}

s32 *func_80404C84(s32 *a, s32 *b)
{
    s32 *q = (s32 *)D_8040B1D0;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040B1D0, 0xC9);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_8040062C, D_80400654, 0x17A);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80404DBC((u8 *)q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x304);
    }
    D_804F281B = 8;
    D_805030EF = 8;
    func_80404B84(r, 7);
    q[3] = (s32)(r + 0x84);
    r[0x87] = func_80404B5C(r[0x88]);
    r[1] = (s32)b;
    func_80404C44((u8 *)r, (u32)a);
    q[4] = (s32)func_804046C8;
    func_804046C8((u8 *)r);
    return q;
}

/* func_8004B500. D_800814A0 is D_8008DAB4 here, by a 24-vote symmap entry,
 * and the local `g` is the live body's -- see main.h's note on the two
 * spellings of that object. */
void func_80404DBC(u8 *p)
{
    u32 g;

    if (p != 0)
    {
        g = D_8008DAB4;
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_8008DAB4 = g | 0x8000;
    }
}

/* Live func_800242D8, MOVED OVER UNCHANGED except for the symbol
 * (D_80027FD0 -> D_80408CF0). It was the headline blocked function -- 28
 * differing, and the triage above records five spellings of the indexed load
 * swept against it, because under 4.1's compiler the load could not be made
 * to fold: GO_IF_LEGITIMATE_ADDRESS rejects a constant address plus a
 * register whenever mips_split_addresses is on, and override_options turns it
 * on unconditionally. tools/psx272gcc has no such pass, so
 * `D_80408CF0[i++]` emits the one insn the assembler expands into
 * `lui $at` / `addu $at,$at,$a3` / `lbu %lo(...)($at)`, and the live body
 * matches with nothing added. Every lever in the live source's own long
 * comment -- the empty third for-clause, `i++` before `p += 0x18` -- still
 * does its work; see csrc/main/cardbk.c. */
u8 *func_80404DEC(u8 *p, s32 w, s32 h)
{
    s32 i;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 0;
    p += 0xC;
    for (i = 0; i < 4;)
    {
        p[1] = 0x38;
        *(s32 *)(p + 4) = 0x9CBCBC;
        *(s32 *)(p + 0xC) = 0x9CBCBC;
        *(s32 *)(p + 0x10) = 0x9CBCBC;
        *(s32 *)(p + 0x14) = 0x8C9C9C;
        p[0xA] = w / 2;
        p[0xB] = h / 2;
        p[0] = D_80408CF0[i++];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}

/* NEAR MISS: two words: an unsigned division's magic constant is a CONST_DOUBLE when its top bit is set, so mips.md's large-constant split never fires and the lui cannot fill the load delay
   Source in docs/near-misses/func_80404EA0.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404EA0);

/* NEAR MISS, 82 of 82 words, 10 differing -- the LENGTH now matches. Rebuilt
 * twice on 2026-11-12: 81/68, then 84/46, then this. The last device was the
 * loop bound: written `while (n < 3 ? i < 2 : i < 1)` rather than as a `for`
 * over the conditional bound `(n < 3 ? 2 : 1)`, there is ONE induction
 * variable instead of two and the whole loop comes out byte-exact. What is
 * left is six words of scheduling before the first jal (gcc hoists
 * `addiu $s1,$sp,0x50` above the two seed stores), the three-word `mfhi`
 * reload-register gap it shares with func_80404EA0, and one commuted `addu`.
 * Source in docs/near-misses/func_804053A0.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804053A0);

/* The live func_80024A50 has a second test on p[0x20] and a third callee in
 * the else; this build has neither. */
void func_804054E8(s32 *p)
{
    s32 n = p[0x1F];

    if (D_8009DDD8[n][0] != 0)
    {
        func_80404EA0(p);
    }
    else
    {
        func_804053A0(p);
    }
}

void func_8040553C(s32 *p)
{
    UnkCardPair *q;

    func_80404DEC((u8 *)(p + 1), 0xF8, 0x48);
    q = (UnkCardPair *)p[0x2C5];
    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0xB0;
    q->unk_00 = (s32)(p + 1);
    q = (UnkCardPair *)p[0x2C2];
    q->unk_04->unk_08 = 0x60;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_800841DC;
    q = (UnkCardPair *)p[0x2C3];
    q->unk_04->unk_08 = 0xD0;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_8008423C;
    q = (UnkCardPair *)p[0x2C4];
    q->unk_04->unk_08 = 0x20;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_8008429C;
}

void func_80405624(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* RETURNS: func_804057F0 stores $v0 straight after the call. */
s32 func_804056CC(s32 a)
{
    return func_80047358(a, D_80408D28, 0x15);
}

/* Live func_80024C70, the UnkTwinGrpAEC instance of the five-way template builder. The
 * group is 0xAEC bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_804056F4(UnkTwinGrpAEC *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_804057A8(s32 *p, s32 v)
{
    p[0] = v;
}

void func_804057B0(s32 *p, s32 v)
{
    func_804057A8(p, v);
    func_80405624((UnkCardPair *)(p + 0x2BE), (u32 *)(p + 0x2B4), (u32 *)(p + 0x2B8));
    func_8040553C(p);
}

/* Live func_80024D58. TWO arguments here, not three -- there is no r[0x20]
 * store -- and the allocation failure is an ASSERT this build carries and the
 * live one does not (D_80400738/D_80400760 are this translation unit's own
 * copies of the format and the filename, not c_server.c's D_80400004 pair).
 * The group is spelled in raw word indices, as func_804057B0 and
 * func_80405900 beside it are: root at r[0x2BE], its unk_0C at r[0x2C1],
 * ptrs at r[0x2C2]. */
s32 *func_804057F0(s32 *p, s32 a)
{
    s32 *q = (s32 *)D_8040B4F8;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040B4F8, 0x387);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_80400738, D_80400760, 0x1A0);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_804059A8(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0xDFC);
    }
    r[0x1F] = a;
    func_804056F4((UnkTwinGrpAEC *)r, 0x15);
    q[3] = (s32)(r + 0x2BE);
    r[0x2C1] = func_804056CC((s32)(r + 0x2C2));
    func_804057B0(r, (s32)p);
    q[4] = 0;
    return q;
}

void func_80405900(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_80051FBC(*(s32 *)p[0x2C6 + i]);
        *(s32 *)p[0x2C6 + i] = 0;
    }
    for (i = 0; i < 4; i++)
    {
        *(s32 *)p[0x2C9 + i] = 0;
    }
    for (i = 0; i < 10; i++)
    {
        *(s32 *)p[0x2CD + i] = 0;
    }
}

void func_804059A8(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80405900((UnkCardGrpB20 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80405A00(s32 *p, s32 a)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x1F] = a;
        func_80405900((UnkCardGrpB20 *)q);
        func_804054E8(q);
    }
}

void func_80405A3C(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80405A64(s32 *p)
{
    func_80405A3C(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406368;
    }
}

void func_80405AB4(s32 *p)
{
    func_80405AE8(p);
    p[-4] = (s32)func_80406368;
}

/* main.bin's 0x80400000 twin, live counterpart func_80025030.
 *
 * THE FUNCTION THE SECOND COMPILER WAS WORTH. Under the 4.1 cc1 it sat at
 * exactly one differing word for a week, parked, with its own note saying it
 * would land unchanged the day a cc1psx without reload_cse_regs existed. It
 * did, and it did -- not one character of the body changed.
 *
 * Two levers took it from 20 differing to 1, and both are already landed on
 * its sibling func_80405B98:
 *
 *   1. func_80405A00 TAKES TWO ARGUMENTS in this build where the live
 *      func_80024F3C takes three. Its listing reads $a0 and $a1 and never
 *      $a2.
 *   2. THE INCREMENT ORDER IS THE DELAY SLOTS. `i++` before `off++`, with a
 *      plain `while (i < 5)`, puts the counter increment in the two `j`
 *      delay slots and pushes the pointer increment into the `bnez`'s. The
 *      live spelling -- `off++;` then `while (++i < 5)` -- is 19 differing.
 *
 * What is left is word 10:
 *
 *     candidate  move  $s3, $v0          target  addiu $s3, $zero, 0x1
 *
 * `flag = 1` in the arm whose test already put 1 in $v0. 4.1's
 * reload_cse_simplify_set rewrites the constant to a copy of that register;
 * FSF 2.7.2 has no such pass, so it materialises, which is what the target
 * does. It could never have been spelled around -- the TARGET also carries
 * `addiu $v0, $zero, 0x1` for the comparison, so the register genuinely
 * holds 1 there and 4.1's substitution is unconditional.
 *
 * MEASURE IT WITH `ovlverify.py --G 8`, never the default -G0.
 */
void func_80405AE8(s32 *p)
{
    s32 flag;
    s32 off;
    s32 i;

    if (p[0x12] == 1)
    {
        flag = 1;
    }
    else
    {
        func_80403D7C((s32 *)p[6], p[0x11]);
        flag = 0;
    }
    off = 0;
    i = 0;
    do
    {
        if (i == p[10])
        {
            func_804045C8((s32 *)p[off + 1], flag);
        }
        else
        {
            func_80404668((s32 *)p[off + 1]);
        }
        i++;
        off++;
    } while (i < 5);
    func_80405A00((s32 *)p[7], p[10]);
}


/* Live func_800250E8, with two differences and neither of them an index: the
 * p[10]/p[11]/p[7] reads are at the same offsets as the live ones.
 *
 * 1. func_80405A00 TAKES TWO ARGUMENTS HERE where the live func_80024F3C
 *    takes three -- its listing reads $a0 and $a1 and never $a2, which is
 *    also the 2-word length difference between the two bodies.
 * 2. THE INCREMENT ORDER IS THE DELAY SLOTS. Written the live way --
 *    `off++;` then `while (++i < 5)` -- the pointer increment fills the two
 *    `j` delay slots and the `bnez` gets a nop: 18 differing. Incrementing
 *    the counter FIRST puts `i++` in those slots and pushes `off++` into the
 *    `bnez`'s, which is what this build does. 0 differing. */
void func_80405B98(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_80404618((s32 *)p[off + 1], 1);
        }
        else if (i == p[10])
        {
            func_804045C8((s32 *)p[off + 1], 1);
        }
        else
        {
            func_80404668((s32 *)p[off + 1]);
        }
        i++;
        off++;
    } while (i < 5);
    func_80405A00((s32 *)p[7], p[10]);
}

/* splat seeded this as an empty `(void)` function, which is a correct
 * spelling of its two words (`jr $ra`, `nop`) but not the source's: the
 * live counterpart func_8002519C takes a u8 *, and an empty body emits the
 * same two words either way. */
void func_80405C44(u8 *p)
{
}

/* Live func_800251A4. Three differences: D_8008CA34 and func_80400EF8 stand in
 * for its func_80027BF4, the D_80082E60 flag update and two of its calls are
 * absent, and the two absolute addresses are this link's -- 0x80011F48 for the
 * halfword store and 0x80010208 for the word read, where the live pair is
 * 0x80013714 and 0x80010208 and the read is a HALFWORD. */
void func_80405C4C(s32 *p)
{
    s32 v;

    D_8008CA34 = 2;
    func_80400EF8();
    v = 1;
    if (p[0x11] == 0)
    {
        v = 3;
    }
    D_80011F48 = v;
    func_8003D548(D_80010208.unk_00 != 0 ? 5 : 4);
}

void func_80405CB8(s32 *p)
{
    if (func_80402154(p[10], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_80406368;
        func_8040325C((s32)(p - 8));
        p[-4] = (s32)func_80405A3C;
    }
    else
    {
        func_80405C44((u8 *)0x80010000);
        p[-4] = (s32)func_80405C4C;
    }
    func_80400908();
    p[16] = 0;
}

void func_80405D2C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405CB8;
    }
}

void func_80405D54(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[15] = 1;
        func_804031E4((s32)(p - 8), (s32)(p + 16));
        p[16] = 1;
        p[-4] = (s32)func_80405D2C;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025398. Four differences: func_80402154 takes two arguments
 * here, the live func_80025358 call is a plain `D_80010208.unk_00 = 0;`, the loop
 * calls func_80404688 rather than func_80404C84, and the fail arm's two
 * handlers are func_80406368 and func_80405A3C where the live one has
 * func_80024FFC twice over. */
void func_80405DE0(s32 *p)
{
    u8 *b = (u8 *)0x80010000;
    s32 i;
    s32 off;

    if (func_80402154(p[11], b) == 0)
    {
        goto fail;
    }
    D_80010208.unk_00 = 0;
    if (func_80402084(p[10], b) == 0)
    {
        goto fail;
    }
    func_80405C44(b);
    func_80402214();
    off = 0;
    for (i = 0; i < 5; i++)
    {
        func_80404688((s32 *)p[off + 1]);
        off++;
    }
    func_80405AB4(p);
    goto done;
fail:
    p[13] = (s32)func_80406368;
    func_8040334C((s32)(p - 8));
    p[-4] = (s32)func_80405A3C;
done:
    func_80400908();
    p[16] = 0;
}

void func_80405EAC(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405DE0;
    }
}

void func_80405ED4(s32 *p)
{
    p[15] = 1;
    func_80403324((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80405EAC;
}

/* Live func_800254E4. REVISION, and the MIRROR of the one func_804065CC
 * carries: here the twin has TWO handlers where the live source has one.
 * func_80024FFC is assigned at both `p[13]` sites live; this build assigns
 * func_80406368 in the not-enough-blocks arm and func_80405AB4 in the
 * cannot-overwrite arm. That is why twinmove refuses the symbol -- the map's
 * vote is a genuine tie, one live symbol against two twin ones, and the
 * listing is the only thing that can break it. */
void func_80405F24(s32 *p)
{
    s32(*tbl)[32] = D_8009DDD8;

    if (tbl[p[10]][0] == 0)
    {
        if (func_80402268() < 3)
        {
            p[13] = (s32)func_80406368;
            func_80403234((s32)(p - 8));
            p[-4] = (s32)func_80405A64;
        }
        else
        {
            func_80405ED4(p);
        }
    }
    else
    {
        p[13] = (s32)func_80405AB4;
        func_80403374((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025584. D_801379A8/D_801379B0 are this link's two input words,
 * read as plain globals where the live source has D_80083160.unk_08/unk_10;
 * the 0x40 and 0x20 arms are the other way round; and the tail calls
 * func_804083FC with a literal 0 where the live one passes p[9]. */
void func_80405FC4(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            p[-4] = (s32)func_80405AB4;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_80405F24(p);
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 2;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            p[10] = func_80058FF0(p[10], dir, 5);
            func_80405B98(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_80406720(p - 8);
    func_804083FC(p[8], 0);
}

/* Live func_8002570C. REVISION: the live true-arm calls func_80024F3C's
 * sibling between the two stores; this build has no call there at all, just
 * `p[11] = n` and the handler. */
void func_80406150(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[11] = n;
        p[-4] = (s32)func_80405FC4;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_80025788, with func_80404688 in the loop where it has
 * func_80404C84, func_80405A64 as the else arm's handler where it has
 * func_80405A3C, and no `func_80400908(); p[16] = 0;` tail. */
void func_804061C4(s32 *p)
{
    s32 i;
    s32 off;

    if (p[14] != 0)
    {
        p[-4] = (s32)func_80406368;
    }
    else
    {
        if (func_804022C8(p[10]) != 0)
        {
            p[-4] = (s32)func_80406368;
            off = 0;
            for (i = 0; i < 5; i++)
            {
                func_80404688((s32 *)p[off + 1]);
                off++;
            }
        }
        else
        {
            p[13] = (s32)func_80406368;
            func_804033C4((s32)(p - 8));
            p[-4] = (s32)func_80405A64;
        }
    }
}

/* Live func_80025848, MOVED OVER UNCHANGED except for the four symbols. The
 * other half of the folded-indexed-load pair with func_80404DEC: under 4.1's
 * compiler two levers took it 32 -> 5 and the five were entirely
 * `D_8009DDD8[n][0]`, which came out as a split address plus an `addu` plus a
 * `lw 0(...)` where the target has the assembler's three-word expansion of
 * one insn. tools/psx272gcc folds it, and the live body matches with no
 * device. */
void func_80406274(s32 *p)
{
    s32 n = p[10];

    if (D_8009DDD8[n][0] != 0)
    {
        p[13] = (s32)func_804061C4;
        func_8040339C((s32)(p - 8), (s32)(p + 14));
        p[-4] = (s32)func_80405A64;
    }
    else
    {
        p[13] = (s32)func_80406368;
        func_804032FC((s32)(p - 8));
        p[-4] = (s32)func_80405A64;
    }
}

/* Live func_800258C8. The source is `void (*fns[4])(s32 *) = {...}` -- written
 * that way gcc emits the template into this unit's .rodata a SECOND time and
 * the container comes out 0x10 long, because the unit has one dotted
 * `.rodata` subsegment and this template is 0x6DC further on, inside a
 * hand-owned blob. Copying the blob's own object instead produces the same
 * four lw/sw pairs and emits nothing; the declaration in main_twin.h carries
 * the rest of the note.
 *
 * It also remains true that the twin block is several translation units like
 * the live side's nine, and that our single `c` subsegment is an
 * approximation that works while the asm rodata blobs fill the gaps. This
 * function was the one piece of evidence for that and it no longer forces
 * the carve. */
void func_804062F4(s32 *p)
{
    UnkTwinDispatch4 tbl = D_8040076C;

    tbl.f[p[0x11]](p);
}

void func_80406358(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

/* Live func_8002593C. The 0x40 and 0x20 arms are the other way round, the
 * counter loses 4 rather than 2, and the second func_80058FF0 call takes 4
 * where the live one takes 3. */
void func_80406368(s32 *p)
{
    s32 dir = 0;
    s32 upd = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_80406720(p - 8);
            func_804083FC(p[8], 0);
            return;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            if (p[18] == 0)
            {
                func_80406358(p);
                upd = 1;
            }
            else
            {
                func_804062F4(p);
            }
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        else if (D_801379A8.unk_00 & 0xA000)
        {
            if (D_801379A8.unk_00 & 0x2000)
            {
                p[18] = 1;
            }
            else
            {
                p[18] = 0;
            }
            upd = 1;
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            if (p[18] == 1)
            {
                p[10] = func_80058FF0(p[10], dir, 5);
            }
            else
            {
                p[17] = func_80058FF0(p[17], dir, 4);
            }
            func_80405AE8(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r != 0)
    {
        if (r != 1)
        {
            func_80406720(p - 8);
            func_804083FC(p[8], 0);
        }
    }
    if (upd != 0)
    {
        func_80405AE8(p);
    }
}

void func_80406568(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406570(s32 *p, s32 v)
{
    func_80406568(p, v);
}

void func_80406590(s32 *p)
{
    func_80405AE8(p);
    func_80400908();
    p[-4] = (s32)func_80406368;
}

/* Live func_80025BD4, with one revision difference and one delay slot.
 *
 * THE REVISION DIFFERENCE: this build assigns func_80406590 at BOTH sites
 * where the live source has two separate handlers, func_80025B9C for p[13]
 * and func_80025B60 for the trailing p[-4]. Its listing names the same
 * symbol twice, so this is not a symmap miscount.
 *
 * THE DELAY SLOT is the last p[-4] only, and it was what the second compiler
 * fixed. Under 4.1's, `(s32)func_80406590` let reorg steal that address's
 * `lui` into the preceding `bne`'s slot where this build keeps a nop (11
 * differing), and a sized data alias at that one site bought it back. Under
 * tools/psx272gcc the address is one insn and the plain spelling is 0. */
void func_804065CC(s32 *p)
{
    s32 n = p[9];

    if (n < 5)
    {
        if (func_80401E2C(n) != 0)
        {
            p[n + 1] = func_80404364((s32)(p - 8), n);
            p[9] = p[9] + 1;
        }
        else
        {
            p[13] = (s32)func_80406590;
            func_8040325C((s32)(p - 8));
            p[-4] = (s32)func_80405A3C;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80406590;
    }
}

s32 *func_80406678(s32 *p, s32 a)
{
    s32 *q = func_8003C714(0, D_8040C628, 0x1C);
    s32 *r = q + 8;

    func_80406570(r, (s32)p);
    func_804023C4();
    func_80402214();
    r[0x12] = 1;
    q[8] = func_80404C84(q, q + 0x1A);
    r[6] = (s32)func_80403BC4(q);
    r[7] = (s32)func_804057F0(q, r[10]);
    r[9] = 0;
    q[4] = (s32)func_804065CC;
    return q;
}

/* TWO devices, and func_804070C4 below is the same function with three
 * different callees and the same two.
 *
 * The degenerate `do { } while (0)` sets the register assignment. cse rewrites
 * all three of `q[0]`, `q[6]` and `q[7]` as p-relative, so `q` loses the
 * references that would outrank `i`; flow.c:440 weights a reference by
 * loop_depth, and one more level over the body is enough to put them back in
 * the target's order (p in $s2, q in $s0, i in $s1).
 *
 * The empty asm sets the call's delay slot. reorg fills a CALL_INSN's slot
 * from BEHIND as well as from in front, so with `q++` written before the call
 * it takes `q++` either way; `stop_search_p` ends the backward scan at an
 * asm, and the forward scan then takes `i++`, which is the target's
 * `addiu $s0,$s0,4` / `jal` / `addiu $s1,$s1,1`. The hoisted `v` is what lets
 * `q++` precede the call at all. */
void func_80406720(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_80404DBC((u8 *)q[0]);
        func_80403D24((s32 *)q[6]);
        func_804059A8((s32 *)q[7]);
        i = 0;
        do
        {
            do
            {
                s32 *v = (s32 *)q[1];

                q++;
                __asm__("");
                func_80404570(v);
            } while (0);
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

void func_804067BC(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_804045C8((s32 *)p[off + 3], 1);
        }
        else
        {
            func_80404668((s32 *)p[off + 3]);
        }
        off++;
    } while (++i < 5);
    func_80405A00((s32 *)p[1], p[11]);
}

/* The second block copy's seven-byte tail is `lwl/lwr` + `lh` + `lb`, which
 * is expand_block_move with align 2 and a size of 0x27. No C TYPE can ask for
 * that pair -- a 0x27-byte struct has align 1, and raising its alignment
 * rounds the size to 0x28 -- but the alignment memcpy is handed does not come
 * from a type: get_pointer_alignment (expr.c) reads it off the pointer
 * EXPRESSION, and a cast is undone on the way (the NOP_EXPR arm re-reads the
 * inner pointer's target type), so only a pointer VARIABLE of u16 type keeps
 * it. Hence q, which every use goes through: route one of them through p and
 * the two pseudos cost a `move`. */
void func_80406844(u8 *p, s32 a, s32 mode)
{
    u8 buf[0x10];
    u16 *q = (u16 *)p;

    *(UnkTwinBlk200 *)q = D_80408EC8;
    memcpy(q + 2, D_8040077C, 0x27);
    func_8003830C(a + 1, buf);
    func_8007BF50((u8 *)q + 4, buf);
    D_80010208.unk_00 = mode == 2;
}

void func_804069FC(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80406A24(s32 *p)
{
    func_804069FC(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406DA0;
    }
}

/* No live counterpart: D_8008CA34 is written by six functions in this block
 * and by nothing in the live one. It is a BYTE -- the store is `sb`. */
void func_80406A74(void)
{
    D_8008CA34 = 2;
    func_80400EF8();
    func_8003D548(3);
}

/* Live func_800262BC, with func_80403144 where it has func_804030F4, two of
 * its calls absent, func_80404688 taking one argument, and the 0x80010000
 * read a WORD here where the live one reads ((s16 *)0x80010000)[0x104]. The
 * handler goes through a LOCAL: three stores would cost three %hi pairs. */
void func_80406AA8(s32 *p)
{
    func_80406844((u8 *)0x80010000, p[11], p[9]);
    if (func_80402084(p[11], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_80406DA0;
        func_80403144((s32)(p - 8));
        p[-4] = (s32)func_804069FC;
    }
    else
    {
        func_80405A00((s32 *)p[1], p[11]);
        func_80404688((s32 *)p[p[11] + 3]);
        if (D_80010208.unk_00 != 0)
        {
            p[-4] = (s32)func_80406A74;
        }
        else
        {
            p[-4] = (s32)func_80406DA0;
        }
    }
    func_80400908();
    p[16] = 0;
}

void func_80406B74(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80406AA8;
    }
}

void func_80406B9C(s32 *p)
{
    p[15] = 1;
    func_804031BC((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80406B74;
}

/* Live func_80026410, same body and the same p[14]. The ONE difference is a
 * delay slot: the live build steals the function address's `lui` into the
 * `beqz`'s slot, this build leaves a nop there. Under 4.1's compiler that
 * needed a sized DATA alias of the function symbol to make the address one
 * insn, so reorg had nothing to take -- 12 differing plain, 0 with the alias.
 * Under tools/psx272gcc the address is one insn to begin with and the plain
 * spelling matches, so the alias is gone. */
void func_80406BEC(s32 *p)
{
    if (p[14] != 0)
    {
        p[-4] = (s32)func_80406DA0;
    }
    else
    {
        func_80406B9C(p);
    }
}

void func_80406C2C(s32 *p)
{
    p[13] = (s32)func_80406BEC;
    func_80403194((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_80406A24;
}

void func_80406C74(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_8040320C((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

void func_80406CB8(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_80403234((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

void func_80406CFC(s32 *p)
{
    if (D_8009DDD8[p[11]][0] != 0)
    {
        p[-4] = (s32)func_80406C2C;
    }
    else if (func_80402268() >= 3)
    {
        p[-4] = (s32)func_80406B9C;
    }
    else if (func_80402290() != 0)
    {
        func_80406C74(p);
    }
    else
    {
        func_80406CB8(p);
    }
}

/* Live func_800265B8, the sibling of func_80405FC4 above: the 0x40 arm goes
 * straight to the close tail, p[12] loses 4 here rather than 2, and the
 * state word is p[11].
 *
 * DEVICE: that first arm REPEATS the close tail instead of a `goto` to it,
 * and gcc's cross-jumping merges the two into the one `j`. The four extra
 * references to `p` are what the device is for -- with a `goto`, `dir`
 * outranks `p` in global.c's allocno order and the two take $s0 and $s1 the
 * other way round, 22 differing with every other word already right. The
 * live func_800265B8 uses the goto, and at -G0 either spelling matches. */
void func_80406DA0(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_804070C4(p - 8);
            func_804083FC(p[8], p[9]);
            return;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_80406CFC(p);
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            p[11] = func_80058FF0(p[11], dir, 5);
            func_804067BC(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_804070C4(p - 8);
    func_804083FC(p[8], p[9]);
}

void func_80406F0C(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406F14(s32 *p, s32 v)
{
    func_80406F0C(p, v);
}

void func_80406F34(s32 *p)
{
    func_804067BC(p);
    func_80400908();
    p[-4] = (s32)func_80406DA0;
}

/* Live func_800267C0, and func_804065CC's twin in every sense: the same body
 * one slot over (p[10] and p[n+3] where that one has p[9] and p[n+1]), the
 * same revision collapse -- func_80406F34 at both the p[13] and the trailing
 * p[-4] site, where the live source has func_80026784 and func_80026748 --
 * and the same delay slot, which was 11 differing under 4.1's compiler and
 * needed a sized alias at the last site; it is 0 plain under
 * tools/psx272gcc. */
void func_80406F70(s32 *p)
{
    s32 n = p[10];

    if (n < 5)
    {
        if (func_80401E2C(n) != 0)
        {
            p[n + 3] = func_80404364((s32)(p - 8), n);
            p[10] = p[10] + 1;
        }
        else
        {
            p[13] = (s32)func_80406F34;
            func_8040325C((s32)(p - 8));
            p[-4] = (s32)func_804069FC;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80406F34;
    }
}

s32 *func_8040701C(s32 *p, s32 a)
{
    s32 *q = func_8003C480(0);
    s32 *r;

    if (q != 0)
    {
        r = q + 8;
        r[9] = a;
        func_80406F14(r, (s32)p);
        func_804023C4();
        func_80402214();
        q[8] = func_80404C84(q, 0);
        r[1] = (s32)func_804057F0(q, r[11]);
        r[2] = (s32)func_80407C10(q);
        r[10] = 0;
        q[4] = (s32)func_80406F70;
    }
    return q;
}

/* The same two devices as func_80406720 above. */
void func_804070C4(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_80404DBC((u8 *)q[0]);
        func_804059A8((s32 *)q[1]);
        func_80407D38((s32 *)q[2]);
        i = 0;
        do
        {
            do
            {
                s32 *v = (s32 *)q[3];

                q++;
                __asm__("");
                func_80404570(v);
            } while (0);
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

/* Live func_800269B4, with the group's ptrs reached through p[0x18] -- the
 * same extra indirection as func_804072CC and func_80407458. */
void func_80407160(s32 *p)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        s32 k = D_80408D40[p[9]][i];
        u32 *e = ((u32 **)p[0x18])[k + 2];

        if (e[0] != 0)
        {
            func_8004DDBC((u32 *)((u8 *)p + (k * 12 + 4)), D_80408D4C[i], e);
        }
        ((UnkCardPair **)p[0x18])[i]->unk_04->unk_00 =
            D_80408D44[D_80408D40[p[9]][i]];
        ((UnkCardPair **)p[0x18])[i + 2]->unk_04->unk_00 =
            D_80408D44[D_80408D40[p[9]][i]];
    }
}

void func_80407298(s32 *p)
{
    func_8004DCCC(p + 1);
    func_8004DCCC(p + 4);
}

/* Live func_80026B18, with ONE MORE INDIRECTION: the pair comes through
 * p[0x18] here, where the live one reads the group's own ptrs array. Same
 * difference as func_80407458 and func_804074C4. */
void func_804072CC(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            UnkCardPair *q = ((UnkCardPair **)((s32 *)p)[0x18])[2 + i];

            any = 1;
            if (b[i] != 0)
            {
                func_8004DDBC((u32 *)(p + (i * 0xC + 4)), (u8 *)D_80408A30, (u32 *)q);
            }
            else
            {
                q->unk_00 = 0;
                *(s32 *)(p + i * 0xC + 8) = 0;
            }
        }
    }
    if (any != 0)
    {
        func_80407160((s32 *)p);
    }
}

void func_804073B0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* Live func_80026C94. REVISION: one more INDIRECTION. The live struct has
 * `ptrs` inline at +0x60; here +0x60 holds a POINTER to the pair array, so
 * each access is `lw 0x60($a0)` then `lw N($v0)`. And it is re-read for the
 * second pair rather than held in a local, because the `sw` through the
 * first pair invalidates cse's entry for it. */
void func_80407458(s32 *p)
{
    UnkCardPair *q;

    q = ((UnkCardPair **)p[0x18])[0];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80409110;
    q = ((UnkCardPair **)p[0x18])[1];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80409128;
}

/* Live func_80026CF0, func_80407458's sibling and the same extra
 * indirection through +0x60. */
void func_804074C4(s32 *p)
{
    UnkCardPair *q;

    q = ((UnkCardPair **)p[0x18])[2];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    func_8004DDBC((u32 *)(p + 1), (u8 *)D_80408A30, (u32 *)q);
    q = ((UnkCardPair **)p[0x18])[3];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    func_8004DDBC((u32 *)(p + 4), (u8 *)D_80408A30, (u32 *)q);
}

void func_80407558(UnkCardGrp044 *c)
{
    func_80407458(c);
    func_804074C4(c);
    func_804072CC((u8 *)c, (s32 *)D_80408D54, (s32 *)D_80408D5C);
    func_80407160((s32 *)c);
}

s32 func_804075A8(s32 a)
{
    return func_80047358(a, D_80408D64, 4);
}

/* The sixth of the family, and the one whose ptrs is a POINTER at c[0x18]
 * rather than an inline array -- same revision difference as func_804072CC,
 * func_80407458 and func_804074C4. Raw word indices: shared at c[0x11],
 * ents at c[0x19], prims at c[0x29]. */
void func_804075D0(s32 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        ((UnkCardPair **)c[0x18])[i] = (UnkCardPair *)&c[0x19 + i * 4];
        c[0x29 + i * 4] = D_8002D690_a[0];
        c[0x2A + i * 4] = D_8002D690_a[1];
        c[0x2B + i * 4] = D_8002D690_a[2];
        c[0x2C + i * 4] = D_8002D690_a[3];
        c[0x1A + i * 4] = (s32)&c[0x29 + i * 4];
        c[0x1B + i * 4] = (s32)&c[0x11];
    }
    c[0x11] = D_8002D6A0.unk_00;
    c[0x12] = D_8002D6A4.unk_00;
    c[0x13] = D_8002D6A8.unk_00;
}

void func_80407688(s32 *p, s32 v)
{
    p[0] = v;
    func_804073B0((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80407558(p);
}

/* Live func_80026ED0. ONE argument, not two -- the live `r[9] = a;` has no
 * counterpart -- and, as in func_804057F0, the allocation failure is an
 * assert this build carries and the live one does not.  Raw word indices for
 * the UnkCardGrp044: root at r[0x14], its unk_0C at r[0x17], ptrs at r[0x18]. */
s32 *func_804076C8(s32 *p)
{
    s32 *q = (s32 *)D_8040C318;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C318, 0x42);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804007A4, D_804007CC, 0x136);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_804077D4(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0xE4);
    }
    func_804075D0(r, 4);
    q[3] = (s32)(r + 0x14);
    r[0x17] = func_804075A8(r[0x18]);
    func_80407688(r, (s32)p);
    q[4] = (s32)func_80407298;
    return q;
}

/* Live func_80026FB4 plus an assert on the pointer. */
void func_804077D4(s32 *p)
{
    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804007A4, D_804007CC, 0x14B);
        func_8007BEF0(1);
    }
    ((u16 *)p)[15] |= 0x8000;
    D_8008DAB4 |= 0x8000;
}

void func_80407830(s32 *p, s32 a)
{
    s32 *q = p + 8;

    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804007A4, D_804007CC, 0x153);
        func_8007BEF0(1);
    }
    p[0x11] = a;
    func_80407160(q);
}

void func_8040787C(s32 *p, s32 *b, s32 *c)
{
    func_804072CC((u8 *)(p + 8), b, c);
}

void func_8040789C(UnkCardGrp0D4 *c)
{
    c->ptrs[0]->unk_00 = (s32)D_80083060;
    c->ptrs[1]->unk_00 = (s32)D_8008306C;
    c->ptrs[2]->unk_00 = (s32)((s32 *)c + 1);
    c->ptrs[3]->unk_00 = (s32)((s32 *)c + 0x1F);
}

void func_804078D8(u8 *p)
{
    func_80051B50(p + 0x7C, D_80408D6C, 1);
}

s32 func_80407904(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

void func_80407964(s32 *p)
{
    func_804078D8((u8 *)p);
    func_80407904((s32)(p + 1), 0x36, 0x10);
    func_8040789C(p);
    ((UnkCardPair *)p[0x3F])->unk_04->unk_08 = 0x8E;
    ((UnkCardPair *)p[0x3F])->unk_04->unk_0A = 0x82;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_08 = 9;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_08 = 0x42;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_08 = 0x24;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_0A = 0xA;
}

void func_80407A44(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_80407AEC(s32 a)
{
    return func_80047358(a, D_80408D74, 4);
}

/* Live func_8002727C, the UnkTwinGrp0D4 instance of the five-way template builder. The
 * group is 0xD4 bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80407B14(UnkTwinGrp0D4 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80407BC8(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80407BD0(s32 *p, s32 v)
{
    func_80407BC8(p, v);
    func_80407A44((UnkCardPair *)(p + 0x38), (u32 *)(p + 0x2E), (u32 *)(p + 0x32));
    func_80407964(p);
}

/* Live func_80027364, a group allocator of the same family as
 * func_804057F0 and the rest, with the assert this build carries. The
 * UnkTwinGrp0D4's root is at r[0x38], its unk_0C at r[0x3B], ptrs at
 * r[0x3C]. */
s32 *func_80407C10(s32 *p)
{
    s32 *q = (s32 *)D_8040C420;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C420, 0x68);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804007D8, D_80400800, 0xEE);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80403D24(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x180);
    }
    func_80407B14((UnkTwinGrp0D4 *)r, 4);
    q[3] = (s32)(r + 0x38);
    r[0x3B] = func_80407AEC((s32)(r + 0x3C));
    func_80407BD0(r, (s32)p);
    return q;
}

void func_80407D0C(s32 *p)
{
    func_80051FBC(*(s32 *)p[0x3F]);
}

void func_80407D38(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80407D0C((UnkCardGrp0D4 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80407D90(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[11];
    }
}

void func_80407DB8(s32 *p)
{
    func_80407D90(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_804081AC;
    }
}

void func_80407E08(s32 *p)
{
    s32 v;

    func_804084DC(p - 8);
    v = p[0];
    if (v == 0)
    {
        func_80406678w((s32 *)p[5]);
    }
    else
    {
        func_8040701C((s32 *)p[5], v);
    }
}

void func_80407E64(s32 *p)
{
    s32 i = 0;
    s32 r;

    do
    {
        r = func_80401B30(p[3]);
        if (r != 5)
        {
            break;
        }
    } while (i++ < 6);
    p[14] = 0;
    func_80400908();
    if (r == 1)
    {
        p[11] = (s32)func_804081AC;
        func_804032AC((s32)(p - 8));
        p[-4] = (s32)func_80407DB8;
    }
    else if (r == 4)
    {
        p[11] = (s32)func_804081AC;
        func_804030F4((s32)(p - 8));
        p[-4] = (s32)func_80407D90;
    }
    else
    {
        p[11] = (s32)func_804081AC;
        func_80403284((s32)(p - 8));
        p[-4] = (s32)func_80407DB8;
    }
}

void func_80407F40(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80407E64;
    }
}

void func_80407F68(s32 *p)
{
    s32 h;

    if (p[15] != 0)
    {
        h = (s32)func_804081AC;
    }
    else
    {
        p[16] = 1;
        func_8040316C((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
        h = (s32)func_80407F40;
    }
    p[-4] = h;
}

void func_80407FD4(s32 *p)
{
    p[11] = (s32)func_80407F68;
    func_804030CC((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_80407DB8;
}

void func_8040801C(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804030F4((s32)(p - 8));
    p[-4] = (s32)func_80407D90;
}

void func_80408060(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804032D4((s32)(p - 8));
    p[-4] = (s32)func_80407DB8;
}

void func_804080A4(s32 *p)
{
    s32 st;

    if (D_801379A8.unk_00 != 0 && (D_801379B0.unk_00 & 0x40) != 0)
    {
        func_80400948();
        p[14] = 0;
        p[-4] = (s32)func_804081AC;
    }
    else
    {
        st = func_804016D0();
        func_8003FA78(D_80400848, p[3], D_804090C8[st]);
        func_804018FC();
        if (st != 0)
        {
            p[14] = 0;
            p[12] = st;
            p[-4] = D_804090E0[st];
            p[13] = st;
        }
        else if (func_804010E0() == 3 && p[14] == 0)
        {
            func_8040311C((s32)(p - 8), (s32)(p + 14));
            p[14] = 1;
        }
    }
}

/* Live func_8002789C, with four more calls in the 0x40 arm and two printfs
 * after. The 8-byte copy of t into p[6]/p[7] has to be a STRUCT assignment
 * here for the same reason as there -- see that function's note. */
void func_804081AC(s32 *p)
{
    s32 flag = 0;
    s32 *t;
    s32 diff[2];

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_804084DC(p - 8);
            D_8008CA34 = 2;
            func_80400EF8();
            func_8003FD58(0, 0xF, 0, 0);
            ((u16 *)p[5])[15] |= 0x2000;
        }
        else if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_804008A0(p[3]);
            p[13] = 0;
            p[-4] = (s32)func_804080A4;
            return;
        }
        else if (D_801379A8.unk_00 & 0xA000)
        {
            if (D_801379A8.unk_00 & 0x8000)
            {
                s32 v = p[3];

                p[3] = 0;
                if (v != 0)
                {
                    flag = 1;
                }
            }
            else
            {
                if (p[3] == 0)
                {
                    flag = 1;
                }
                p[3] = 1;
            }
        }
        if (flag != 0)
        {
            func_80063FF8(0x502);
            func_80407830((s32 *)p[9], p[3]);
        }
    }
    t = func_804017EC();
    diff[0] = t[0] != p[6];
    diff[1] = t[1] != p[7];
    *(UnkTwoWords *)&p[6] = *(UnkTwoWords *)t;
    func_8040787C((s32 *)p[9], t, diff);
    func_8003FA78(D_80400854, D_804090F8[t[0]]);
    func_8003FA78(D_80400860, D_804090F8[t[1]]);
    func_804018FC();
}

void func_80408394(s32 *p, s32 a, s32 b)
{
    p[5] = a;
    p[0] = b;
}

/* Live func_80027A9C. func_804076C8 takes ONE argument at this site -- the
 * live call passes p[3] second and there is no load of it here. */
void func_804083A0(s32 *p)
{
    s32 *q = p - 8;

    p[8] = func_80404C84(q, p + 3);
    p[9] = (s32)func_804076C8(q);
    func_80400948();
    p[-4] = (s32)func_804081AC;
}

/* A group allocator with NO func_80040560 call and the two arms the other
 * way round: the allocation path's assert never returns, so gcc jumps past
 * the else arm instead of over the then arm. */
s32 *func_804083FC(s32 a, s32 b)
{
    s32 *q = (s32 *)D_8040C5C0;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C5C0, 0x19);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_8040086C, D_80400894, 0x173);
            func_8007BEF0(1);
        }
    }
    else
    {
        func_804084DC(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x44);
    }
    func_80408394(r, a, b);
    q[4] = (s32)func_804083A0;
    return q;
}

void func_804084DC(s32 *p)
{
    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_8040086C, D_80400894, 0x183);
        func_8007BEF0(1);
    }
    func_80404DBC((u8 *)p[0x10]);
    func_804077D4((s32 *)p[0x11]);
    ((u16 *)p)[15] |= 0x8000;
    D_8008DAB4 |= 0x8000;
}

void func_8040855C(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 1);
}

void func_8040859C(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 2);
}

void func_804085DC(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 0);
}

/* The body is just `return func_8004DBDC();` with a `short` return type. The
 * marker is a DEVICE, worth one word and five differing (5 -> 0).
 *
 * mips.h's `mips_epilogue_delay_slots` gives the epilogue ONE delay slot when
 * the only saved register is $ra, and reorg's backward scan at the end of
 * `fill_simple_delay_slots` then walks back from the last insn looking for
 * something to put in it. Here it finds the `sra` and the epilogue comes out
 * interleaved and noreorder --
 *     lw $31,16($sp) / sra $2,$2,16 / j $31 / addu $sp,$sp,24
 * -- four words where the target has the plain reorder-mode five:
 *     sra $2,$2,16 / lw $ra / addiu $sp / jr $ra / nop
 *
 * `stop_search_p` (reorg.c) ends that scan at an `asm`, so a marker AFTER the
 * truncation blocks it. The cast is the whole device: without it gcc sinks the
 * sign-extension past the asm to the return and the scan reaches it anyway --
 * `short v = g(); __asm__ volatile(""); return v;` is still 9 words. `"r"((s32)v)`
 * forces the sll/sra before the marker, and then there is nothing behind it.
 *
 * THIS IS A COMPILER-VERSION DIFFERENCE AND THE PROOF IS IN THE TREE.
 * csrc/main/memory.c's `s16 func_80027DD0(void) { return func_8004CAA0(); }`
 * is the same two lines in the live block, it is landed byte-identical under
 * the 4.1 compiler, and its epilogue IS interleaved -- 9 words, exactly what
 * gcc gives here. 4.1 fills that slot, this block's 4.0 build does not, and
 * FSF 2.7.2 and the Cygnus b18 tree carry an identical
 * `mips_epilogue_delay_slots`, so the clause is not where they differ.
 * Across the twin's 49 framed functions that save only $ra, not one fills the
 * jr slot; across the live overlay's 48, two do, and both of those are
 * compiled C. Measured 2026-11-12.
 *
 * Tried and rejected, all still 9 words: `short`/`s32` return, a short
 * callee, a short local, an explicit (short) cast, and an empty volatile asm
 * before the return, between the call and the return, and at the top. */
short func_8040861C(void)
{
    short v = func_8004DBDC();

    __asm__("" :: "r"((s32)v));
    return v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408644);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408654);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408664);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408674);

/* Live func_80027D0C. TWO REVISION DIFFERENCES: func_804086BC takes THREE
 * arguments here and the second is 0x3F, not 1. It is a BIOS 0xB0:0x4E stub,
 * so the listing is the only authority on its arity. */
long func_80408684(long chan)
{
    func_804086CC();
    return func_804086BC(chan, 0x3F, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086CC);

/* libcard's InitCARD, not game code: ChangeClearPAD, then init_card /
 * start_card / B0:0x55 inside a critical section. The live func_80027D40 the
 * alignment pairs it with is something else entirely. */
void func_804086DC(long pad_enable)
{
    func_8007CB08(0);
    func_8007CA38();
    func_80408798(pad_enable);
    func_804088A0();
    func_80408958();
    func_8007CA48();
}

void func_80408730(void)
{
    func_8007CA38();
    func_804087A8();
    func_8007CB08(0);
    func_8007CA48();
}

void func_80408768(void)
{
    func_804087B8();
    func_80408958();
    func_804087C8();
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408798);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804088A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408928);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408958);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804089C4);
