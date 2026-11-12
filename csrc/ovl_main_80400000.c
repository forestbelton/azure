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
 *   2 fn, 2 words   A DIFFERENT POINTER SPELLING OF THE SAME ADDRESS.
 *                   func_80406720 and func_804070C4 hold `q = p + 8` and
 *                   reach two fields as `q[6]`/`q[7]`; this block reaches
 *                   the same bytes as `0x38($s2)`/`0x3C($s2)` with $s2
 *                   holding p, i.e. p[14]/p[15].
 *
 *                   REFINED 2026-11-11, and the earlier note that writing it
 *                   that way "is NOT the fix" was half right. p[14]/p[15]
 *                   DOES fix both loads; what is left at 8 differing is one
 *                   REGISTER SWAP and nothing else -- the candidate gives
 *                   $s0 to the counter and $s1 to the pointer, the target
 *                   the reverse, and the four prologue/epilogue saves follow.
 *                   The cause is the priority race: `q[0]` folds to `p[8]`,
 *                   so with q[6]/q[7] gone `q` has NO reference outside the
 *                   loop and loses to `i`. Swept without moving it off 8:
 *                   declaring i first, i inside the if, q inside the if (36),
 *                   a for-loop, `i = 0` at declaration (37), i++ as its own
 *                   statement, a down-counter (26), `register`, `*q` for
 *                   q[0], the final `|=` written off q to buy a reference
 *                   (38), moving only one of the two reads (9 each). The
 *                   live spelling and a `u8 *q` with byte offsets both sit
 *                   at 2. So it is a register race needing a lever that is
 *                   not in this file yet, and the residue is exactly one.
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
 *                   func_80406720/func_804070C4's register race (above) and
 *                   func_804012B0's block layout were characterised under
 *                   4.1 and have not been re-opened -- func_804012B0's last
 *                   arm falls through to the shared return in the live build
 *                   and jumps to it here, and that build reuses a register
 *                   already holding %lo(D_8009E390) where the live one
 *                   re-materialises the pair.
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

/* PARK: 265 words, and the only one of the 86+2 moved so far that is not a
 * mechanical move. The body is right -- the live counterpart open_card_events
 * renamed, with this block's 16 assert line numbers (180..221, -5 from the
 * live source's) -- and comes out 256 words, nine short. The nine are branch
 * delay slots, and the mechanism is NOT the one func_80400EF8 below needs:
 * there the twin leaves the slot empty, here BOTH builds fill it, from
 * opposite sides. At the first site this block has
 *
 *     bne   $v0, $s0, .L80400A6C
 *      ori  $a1, $zero, 0x8000      <- stolen from the branch TARGET
 *
 * against our
 *
 *     bne   v0, s0, ...
 *      lui  a0, %hi(ASSERT_FMT)     <- stolen from the FALL-THROUGH
 *
 * where that `ori` is the NEXT OpenEvent call's second argument, bit-identical
 * in both (34058000) and merely placed differently. So reorg is choosing
 * steal_delay_list_from_target where ours chooses the fall-through, and what
 * decides it is the order sched1 put the next call's argument setup in --
 * $a1's constant first in the target, $a0's `lui` first in ours. Seven of the
 * sixteen sites already agree, so it is per-site and an unconditional barrier
 * (which is what func_80400EF8 wants) overshoots by exactly those seven.
 *
 * Worth knowing before picking this up: docs/overlays.md records that the LIVE
 * open_card_events keeps all sixteen %hi values live in callee-saved registers,
 * which is why it saves $s0..$s7 and $fp. This one saves only $s0, so its
 * register allocation is genuinely different, not just its scheduling. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400A10);

/* The `card_first_flg` site. This opens with
   `printf("%s = %d\n", "card_first_flg", D_8013BB6C)` and then tests and sets
   that same word, so D_8013BB6C is this link's `card_first_flg` and the live
   start_card_server's D_80084130 is the retail link's -- which is why that one is
   now spelled `card_first_flg[0]` (include/structs.h). Nothing else in either
   block names a global. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400E34);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804010F0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804012B0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804016D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804017EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804018FC);

int func_804019A4(void)
{
    return D_804094EC != 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804019B4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401AA4);

/* A `cnt` site: the $s1 the VSync poll loop increments is printed as
   `printf("%s = %d\n", "cnt", cnt)` once the loop ends, so name it `cnt` when
   this lands. `cnt` is TWIN-ONLY -- the live format_card has no counter at
   all, because the print was its only use -- so there is nothing to rename in
   the live unit. Same story in func_804022C8. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401B30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401C70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401D28);

s32 func_80401E14(s32 i)
{
    return D_8009DDD8[i][0];
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401ED4);

/* Prints `(int)gsw->check_sum` after storing it, so the parameter is `gsw` and
   the word at +0x204 is its `check_sum` member -- GSW_CHECK_SUM in csrc/main.h,
   now used by the live set_gsw_checksum/check_gsw_checksum. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401F98);

/* Prints both halves of the comparison: `c` (the xor accumulator) and then
   `(int)gsw->check_sum`. The live check_gsw_checksum's accumulator is now `c`. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402084);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402154);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402290);

/* The other `cnt` site; see func_80401B30. The live erase_save_file has no
   counter either. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804022C8);

void func_804023C4(void)
{
    func_804022C8(5);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804023E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402428);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804024A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402508);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804025B8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402670);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040274C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804027F8);

void func_8040293C(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_8040343C(0);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402974);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402A1C);

void func_80402BB8(s32 *p)
{
    s32 **q = (s32 **)p[0x1A7];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402CD4);

void func_80402D90(s32 *p)
{
    func_80402CD4((u8 *)(p + 9), p);
    func_80402BB8(p);
}

s32 func_80402DC8(s32 a)
{
    return func_80047358(a, D_80408C84, 6);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402EA4);

void func_80402EBC(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    func_80402EA4(p, a, b, c, d);
    func_80402974((UnkCardPair *)(p + 0x19F), (u32 *)(p + 0x195), (u32 *)(p + 0x199));
    func_80402D90(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F04);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F84);

void func_804030CC(s32 a, s32 p)
{
    func_80402F84(a, 0, 1, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804030F4);

/* Live msg_please_wait, NOT msg_loading -- twinalign paired it with
 * msg_loading, which is the arbitrary-pairing trap this block's wrapper run
 * is famous for, and func_804031E4 already holds message 14. Message 2 with
 * kind 2 is msg_please_wait, and it is the only live wrapper with that pair. */
void func_8040311C(s32 a, s32 p)
{
    func_80402F84(a, 2, 2, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403144);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040320C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403234);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040325C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403284);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032FC);

void func_80403324(s32 a, s32 p)
{
    func_80402F84(a, 15, 2, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040334C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403374);

void func_8040339C(s32 a, s32 p)
{
    func_80402F84(a, 18, 1, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804033C4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040343C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403AC8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403BC4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403F7C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404178);

void func_80404224(s32 *p)
{
    p[-4] = 0;
}

s32 func_8040422C(s32 a)
{
    return func_80047358(a, D_80408CB8, 0xA);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404254);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404364);

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
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404708);

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
void func_80404B5C(s32 a)
{
    func_80047358(a, D_80408CE8, 0x7);
}

/* PARK: 46 words against 46, and FIVE differing -- all of them register
 * names, with every displacement and every opcode already right. The
 * spelling that gets here is recorded because it took four tries:
 *
 *   void func_80404B84(u8 *p, s32 n)
 *   {
 *       s32 i;
 *       s32 unused[2];          <- the 8-byte frame this block has
 *
 *       for (i = 0; i < n; i++)
 *       {
 *           (*(u32 **) (p + 0x220))[i] = (u32) (p + 0x224 + i * 0x10);
 *           *(u32 *) (p + 0x294 + i * 0x10) = D_8002D690_a[0];
 *           ... [1] [2] [3] at 0x298 0x29C 0x2A0 ...
 *           *(u32 *) (p + 0x228 + i * 0x10) = (u32) (p + 0x294 + i * 0x10);
 *           *(u32 *) (p + 0x22C + i * 0x10) = (u32) (p + 0x204);
 *       }
 *       *(u32 *) (p + 0x204) = D_8002D6A0;   <- scalars, not an array
 *       *(u32 *) (p + 0x208) = D_8002D6A4;
 *       *(u32 *) (p + 0x20C) = D_8002D6A8;
 *   }
 *
 * WHAT IS SETTLED, and it is the useful part:
 *
 *   * It CONFIRMS func_804048F8's structural finding from a second function.
 *     The live body writes seven primitive pointers inline at p+0x220+i*4;
 *     this one loads p[0x220] and indexes off THAT, and every field above it
 *     is 0x18 lower (0x23C -> 0x224, 0x2AC -> 0x294, 0x240 -> 0x228,
 *     0x244 -> 0x22C). 0x18 is exactly what a seven-entry inline table
 *     (0x1C) costs when it becomes one pointer (0x4).
 *   * The two templates want DIFFERENT spellings in the same function: the
 *     4-word one through a base register (the D_8002D690_a alias) and the
 *     3-word one as three scalars. That is the listing's four %hi values.
 *   * The resident body's `u32 v` local must NOT be carried over, and that
 *     is the opposite of csrc/unk_1C540.c's comment on it, which exists
 *     because the live build needs it. Here it costs 3 words: with it 43 and
 *     25 differing, without it 42, and 42 + the 4-word frame is 46.
 *
 * WHAT IS NOT: ours materialises the template address as `lui $t3` then
 * `addiu $t2, $t3, %lo` -- two registers -- where this block does it in
 * place, `lui $t2` then `addiu $t2, $t2, %lo`. Tried and did not move it:
 * reading [0] through the `s` local (47 words), a sized `[4]` declaration.
 */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404B84);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404C84);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804053A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804054E8);

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

void func_804056CC(s32 a)
{
    func_80047358(a, D_80408D28, 0x15);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804056F4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057F0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A00);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405C4C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405CB8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405DE0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405FC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406150);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804061C4);

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

/* func_804062F4 IS BLOCKED BY THE RODATA SPLIT, NOT BY MATCHING. Live
 * func_800258C8, and its body moves over cleanly -- but it is
 * `void (*fns[4])(s32 *) = {...}`, so gcc emits a 16-byte template into the
 * unit's .rodata and block-copies it to the stack. The target does exactly
 * that, from D_8040076C at 0x26976C.
 *
 * This unit's dotted `.rodata` subsegment is 0x26907C..0x269090, twenty bytes
 * that are an already-landed function's SWITCH TABLE, and a unit can have
 * only one. 0x26976C is 0x6DC further on, inside the hand-owned
 * `ovl_main_80400000_rodata_post` blob, so landing this function emits the
 * template a SECOND time: the object's .rodata goes 20 -> 36 bytes, every
 * rodata byte and the whole text after it shift 0x10, and main.bin comes out
 * 0x10 long. Measured -- and note that ovlverify reported 0 differing for
 * this function throughout, exactly as its own header warns. Only the link
 * catches it.
 *
 * WHAT IT ACTUALLY PROVES: the twin block is several translation units, like
 * the live side's nine, and our single `c` subsegment is an approximation
 * that works only while the asm rodata blobs fill the gaps. Two ways out, in
 * order of honesty: carve this function (and whatever else shares its unit)
 * into its own translation unit with its own `.rodata` at 0x26976C; or leave
 * it here. The live counterpart is cardinfo.c, so the carve has a name to
 * aim at. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804062F4);

void func_80406358(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406368);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406678);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406720);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804067BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406844);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406A74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406AA8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406DA0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040701C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804070C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407160);

void func_80407298(s32 *p)
{
    func_8004DCCC(p + 1);
    func_8004DCCC(p + 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804072CC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407458);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804074C4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804075D0);

void func_80407688(s32 *p, s32 v)
{
    p[0] = v;
    func_804073B0((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80407558(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804076C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804077D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407830);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407B14);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407C10);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804080A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804081AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408394);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804083A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804083FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804084DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040855C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040859C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804085DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040861C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408644);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408654);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408664);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408674);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408684);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408730);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408768);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408798);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804088A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408928);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408958);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804089C4);
