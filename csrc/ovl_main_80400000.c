#include "common.h"
#include "main_twin.h"

/* main.bin's DEBUG TWIN of the 0x80020000 card overlay: the same source built
 * at -G8 with its asserts compiled in, linked against a different engine
 * build, and dead on retail (0x80400000 is 8 MB dev-board RAM). It is in the
 * container, so it still has to be reproduced. See docs/overlays.md.
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
 *   * ONE FUNCTION HAS A LOCAL THE LIVE SOURCE DOES NOT (func_804025F8, 8
 *     bytes, never read). See the DEVICE comment on it.
 *
 *   * ONE CONSTANT IS LOWER BY ONE (func_80402DC8 passes 6 where the live
 *     source passes 7).
 *
 * WHERE THE MECHANICAL PHASE ENDS. Everything above was a move plus an
 * immediate. The 33 functions of twinalign's `near` and `inserts-only`
 * classes that carry no assert were moved the same way and measured, and
 * only ONE of them (func_80407E08) landed; the rest are per-function
 * matching work and are back on INCLUDE_ASM. The triage is worth more than
 * the attempt, so here it is, with the differing-word count:
 *
 *   14 fn, 1 word   THE EQUAL-VALUE REGISTER REUSE DOES NOT HAPPEN HERE.
 *   each            func_804030F4, func_8040311C, func_80403144,
 *                   func_8040320C, func_80403234, func_8040325C,
 *                   func_80403284, func_804032AC, func_804032D4,
 *                   func_804032FC, func_8040334C, func_80403374,
 *                   func_804033C4 -- the card_mes.c wrappers main.h
 *                   documents -- plus func_804013F8. The live build spells
 *                   an argument that equals one already in a register as a
 *                   copy (`addu $a2, $a1, $zero` where the kind equals the
 *                   id, `move $a3, $a2` where both are 0); this block
 *                   re-materialises the constant (`addiu $a2, $zero, 0x2`).
 *                   The SOURCE IS THE SAME -- func_80022EC4 is
 *                   `func_80022D60(a, 2, 2, p)` and this block's
 *                   func_8040311C sets $a1 and $a2 from two separate
 *                   `addiu`s -- so cse chose differently. Tried and did not
 *                   move it: a `void *` fourth parameter, an unprototyped
 *                   callee, a `u8` parameter to force a mode difference.
 *                   One lever here is worth 14 functions.
 *
 *   2 fn, 2 words   A DIFFERENT POINTER SPELLING OF THE SAME ADDRESS.
 *                   func_80406720 and func_804070C4 hold `q = p + 8` and
 *                   reach two fields as `q[6]`/`q[7]`; this block reaches
 *                   the same bytes as `0x38($s2)`/`0x3C($s2)` with $s2
 *                   holding p, i.e. p[14]/p[15]. Writing it that way is NOT
 *                   the fix -- it takes func_80406720 from 2 differing to 8,
 *                   because the allocation cascades.
 *
 *   1 fn, 2 words   func_804078D8: two words transposed, a `lui`/`addiu`
 *                   scheduling order.
 *
 *   others          func_804037F4 (5), func_804012B0 (9), func_804065CC and
 *                   func_80406F70 (11 each), func_804040C8 and
 *                   func_80406BEC (12), func_804083A0 (15), func_80405B98
 *                   and func_80407F68 (19), func_80405AE8 (20),
 *                   func_80404DEC (28), func_804018FC and func_80403E54
 *                   (42), func_804010F0 and func_80405DE0 (49).
 *                   func_804018FC is the extreme case: 42 words against a
 *                   live counterpart of 2, so that pairing is wrong or the
 *                   live source lost the body entirely.
 */

/* The twin of func_80020924, and the pair that measures the -G difference:
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
 * mechanical move. The body is right -- the live counterpart func_80020A80
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
 * func_80020A80 keeps all sixteen %hi values live in callee-saved registers,
 * which is why it saves $s0..$s7 and $fp. This one saves only $s0, so its
 * register allocation is genuinely different, not just its scheduling. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400A10);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804013F8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804015D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804016D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804017EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804018FC);

int func_804019A4(void)
{
    return D_804094EC != 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804019B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401A50);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401AA4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401B30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401C70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401D28);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401E14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401ED4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401F98);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402084);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402154);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402214);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402268);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402290);

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
    /* DEVICE: eight bytes of unread locals, worth nothing in
       instructions and everything in displacements. The live frame is 0x20 --
       0x10 of outgoing-argument area plus the four saved registers, no locals
       -- and this block's is 0x28, with the SAME four saves 8 bytes higher and
       every instruction otherwise identical. So the debug source has a local
       here that it never reads, and gcc 2.7.2 allocates an unused local
       AGGREGATE rather than eliminating it. A scalar would be dropped. */
    s32 unused[2];

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040311C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403698);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804037F4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804039F8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403E54);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804040C8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404570);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804045C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404618);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404668);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404708);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804047B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404818);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404888);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804048F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404A3C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404DEC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405624);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804059A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A00);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A3C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405B98);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405D54);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405F24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405FC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406150);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804061C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406274);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804065CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406678);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406720);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804067BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406844);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804069FC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406BEC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406CFC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040701C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804070C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407160);

void func_80407298(s32 *p)
{
    func_8004DCCC(p + 1);
    func_8004DCCC(p + 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804072CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804073B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407458);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804074C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407558);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040789C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804078D8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407A44);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D90);

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407E64);

void func_80407F40(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80407E64;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407F68);

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
