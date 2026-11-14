#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern long func_80408684(long chan);
extern void func_804086DC(long pad_enable);

/* THE 0x80400000 DEBUG TWIN -- c_server.c, part 1 of 2: the objects below jtbl_8040007C and
   every function up to and including func_804016D0, whose switch emits it.

   .rodata 0x80400000..0x80400090   .text from 0x804008A0

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main_twin/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */

/* ---------------------------------------------------------------------------
 * THE UNIT'S FIRST RODATA, 0x80400000..0x8040007C -- 0x7C bytes, 7 objects,
 * followed immediately by jtbl_8040007C, which func_804016D0's switch emits.
 *
 * c_server.c's rodata: the assert pair, the debug-print pair, and the two card
 * event handler tables.  These lived in a file of their own until the jump
 * table's `.align 3` was dealt with -- gcc 8-aligns every jump table
 * (final.c:1518), the table is at offset 0x7C here, 4 mod 8, and gas paid for
 * it with four bytes of pad until tools/decomp/jtbl_align.py started
 * rewriting that one directive for this unit.  See the Makefile and
 * docs/owning-data.md SS9.3; the live 0x80020000 overlay is the reason the
 * rewrite is scoped to this unit and not applied to the 4.1 ones.
 *
 * ORDER MATTERS AND IS FILE ORDER: these seven are the only file-scope
 * `.rodata` objects the unit defines, and at -G8 all file-scope data precedes
 * everything the text stream emits, so the table lands behind them.  Anything
 * added here lands between 0x80400000 and the table.
 *
 * Nothing references D_80400000 -- not this unit, not csrc/ovl_main_80400000_
 * rodata.c, not any remaining asm stub -- so it must be non-static or gcc
 * discards it.  It is one word holding a function address and the live overlay
 * has no counterpart for it: that overlay's rodata starts at ASSERT_FMT.  What
 * registers it, if anything does, is not known.  It needs the explicit
 * `section(".rodata")` because it is four bytes and this unit is the one built
 * at -G8, which would otherwise put it in `.sdata` and drop it -- measured, as
 * main.bin four bytes short with the image starting at ASSERT_FMT.
 * ------------------------------------------------------------------------- */
/* 0x80400000 */
void (*const D_80400000)(s32 *) __attribute__((section(".rodata"))) =
    func_8040855C;

/* 0x80400004 -- ASSERT_FMT.  c_server.c's own copy; each of the nine original
   translation units has one -- that is how the carve found the unit
   boundaries -- and the other eight open the other eight units. */
const char D_80400004[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x8040002C -- FILENAME_C_SERVER. */
const char D_8040002C[12] __attribute__((section(".rodata"))) = "c_server.c";

/* 0x80400038 -- the debug-print format, used from three sites. */
const char D_80400038[12] __attribute__((section(".rodata"))) = "%s = %d\n";

/* 0x80400044 -- the variable name the line above prints, which is how we know
   the original called this flag `card_first_flg`. */
const char D_80400044[16] __attribute__((section(".rodata"))) = "card_first_flg";

/* 0x80400054 -- gSwCardEventHandlers, and 0x80400068 gHwCardEventHandlers.
   The live csrc/main/c_server.c spells these the same way; only the first
   handler differs between the two tables. */
const UnkFnTab5 D_80400054 __attribute__((section(".rodata"))) = {{
    NULL,
    func_804010F0,
    func_804013D0,
    func_804013A8,
    func_804012B0,
}};

/* 0x80400068 */
const UnkFnTab5 D_80400068 __attribute__((section(".rodata"))) = {{
    NULL,
    func_8040122C,
    func_804013D0,
    func_804013A8,
    func_804012B0,
}};


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
