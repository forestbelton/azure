/* func_80057D20 (unk_1C540) -- 463 of 463 words, 54 DIFF. NOT compiled.
 *
 * THE THIRD AND LAST FUNCTION OF THE 0x80032E14 JUMP-TABLE RUN. func_80054F9C
 * and func_80057A94 both MATCH (see their files); when this one does, the run
 * can be placed and all three land at once -- see the plan in
 * docs/open-matching-problems.md.
 *
 * The controller-change applier proper: 126 controller numbers through
 * jtbl_80032F04, of which 25 have bodies. Case 1 (modulation) is 135 of the
 * 463 words and case 64 (sustain pedal) another 95.
 *
 * THE WORD COUNT IS EXACT AND EVERY OPCODE MATCHES. All 54 differing words
 * are REGISTER NAMES: about 32 of them are `d` sitting in $s5 where the
 * listing has $s4, and the rest are the loop counters permuted with it.
 * Nothing structural is left.
 *
 * THE CASE VALUES AND THEIR SOURCE ORDER come out of the table (file 0x6704,
 * 126 entries, index = cc - 1): 1, 2, 3, 4, 5, 7, 10, 11, 12, 20, 21, 22, 23,
 * 25, 26, 27, 28, 30, 64, 91, then 6, then 98, 99, 120|121|123, 126. **Case 6
 * is out of order in the source**, between 91 and 98; the body addresses say
 * so and nothing else does.
 *
 * WHAT IS PAID FOR
 *
 * * `ch` is a `u8` parameter and ends up in memory at 0x10($sp) -- `sb` once
 *   at entry, `lbu` at each of its nine uses -- which is the register
 *   pressure of a function that holds s0-s7 and fp. `d` is `u8` too, which is
 *   what puts an `andi 0xFF` at every use rather than one at entry.
 *
 * * `func_80055E7C` takes THREE arguments everywhere, and declaring it
 *   unprototyped (`void func_80055E7C();`) is what lets case 1's tail call it
 *   with three. The `addiu $a3, $zero, 1` in cases 7, 10, 11 and 12 is NOT a
 *   fourth argument -- case 12 has no call at all and still has it -- it is
 *   reload's register for the `f1 = 1` store, and $a3 is free precisely
 *   because the calls stop at $a2. Reading it as an argument costs 58 DIFF:
 *   the constant then needs its own pseudo, f1's value gets $t0, and a
 *   `move $a3, $t0` appears that changes what cross-jumping merges in cases
 *   7, 10 and 11.
 *
 * * Case 1's first arm READS BACK `e->unk_40` and its second arm reads back
 *   `e->unk_44`, both just stored. cse forwards each load to the register
 *   holding the stored value, which is why there is no second load -- and the
 *   read-back is what makes the else arm's divide `divu`: `e->unk_44` is
 *   `u32`, where `d` is `u8` and would promote to signed `int` and give
 *   `div`. advise.py's SIGNEDNESS row names exactly this.
 *
 * * `d << 1`, never `d * 2`, in all three places -- case 1's first arm and
 *   cases 20 and 25. With a `u8 d` the multiply materialises the
 *   zero-extension as its own `andi` and the shift lets combine fold it away;
 *   on the byte destinations of cases 20 and 25 that is the difference
 *   between the listing's bare `sll $v0, $s4, 1` and an extra mask, and in
 *   case 1 it is the difference between sharing `e->unk_08 = d`'s mask and
 *   getting the second `andi` the listing has. One word each, and the first
 *   reading of this had it backwards -- with `* 2` in cases 20/25 the two
 *   errors cancelled and the function still read 463 of 463.
 *
 * * The five D_80085458 loops take a BLOCK-SCOPED `u8 *q = D_80085458 + i *
 *   0x78;` inside the body -- func_80057948's recipe. A function-scope
 *   pointer costs a `move` off the giv every iteration, and a walked pointer
 *   costs loop.c a second biased giv.
 *
 * * Case 1's four loops are `n = D_80073734s; i = 0; e->unk_3A = 0;
 *   if (n > 0) { do ... while (i < D_80073734s); }`. All three parts matter:
 *   the bound read FIRST, so it is not pinned behind the store to +0x3A;
 *   `i = 0` SECOND, so it fills the load-delay slot; and the store THIRD, so
 *   reorg has it for the `blez`'s delay slot. Written as a plain `for` with
 *   the store ahead of it, two words of nops appear.
 *
 * * `D_80073734` takes the INCOMPLETE-ARRAY `__asm__` alias, which is the
 *   opposite of func_80057948's choice and is what the listing's delay-slot
 *   `lui $v0, %hi(D_80073734)` requires: the scalar form is one insn of
 *   length 2 and `eligible_for_delay` refuses it, so every loop keeps a nop
 *   instead. Measured on the finished shape, the scalar costs three words
 *   (460) and 272 DIFF. The two luis per loop are reorg copying the join's
 *   one into the `bne`'s slot, not two source expressions.
 *
 * * `f1 = 1` sits BEFORE the call in all three of cases 7, 10 and 11. The
 *   asymmetry the earlier draft needed here was an artefact of reading $a3 as
 *   a fourth argument; with three-argument calls the plain order is right.
 *
 * ABOUT 32 OF THE 54 WORDS ARE ONE REGISTER. `d` is in
 * $s5 here and $s4 in the listing, and every `andi $v0, $d, 0xFF` reads as a
 * differing word because of it. advise.py --greg prices it exactly: `d` is
 * pseudo 84, 34 refs over 177 live insns, priority 0.960; the allocno that
 * takes $s4 ahead of it is pseudo 288 -- case 64's INNER loop counter, 13
 * refs over 32, priority 1.219. To flip them, 288's live length has to pass
 * 40 insns (3 * 13 / 40.6 = 0.960) or `d` needs 44 references, and neither is
 * available at zero cost. MEASURED: repartitioning the counters so the inner
 * loop shares `i` with case 1's loops and a second variable serves case 64's
 * outer loop and the final one -- which is what the listing's register REUSE
 * suggests, $s2 in both places and $s5 in the other two -- is 468 words and
 * 414 DIFF. Register reuse across disjoint live ranges does not mean one
 * variable, and that reading cost a round.
 *
 * The rest is the loop counters permuted around it: `i` is $s2 here and the
 * listing uses $s2 for case 1's loops but $s5 for case 64's outer loop and
 * the final one. That is NOT a third variable -- splitting them three ways
 * (i for case 1, j for case 64's inner, a third for the outer and the final)
 * is 464 words and 90 DIFF, and giving the inner loop `i` with the outer and
 * final sharing a second is 464 and 123. Register reuse across disjoint live
 * ranges does not mean one variable, and that reading has now cost two
 * rounds.
 *
 * MEASURED AND NOT WORKING, besides the counter partitions: `f1` passed as an
 * argument (case 10 then LOADS it back, so f1 really is spilled); `f1` and
 * `f2` declared `volatile` -- the listing's load/or/store of f2 inside case
 * 64's loop looks like volatile but is not, 467 words; every before/after
 * placement of `f1 = 1`; the field store moved after the call; `j` declared
 * at function scope rather than in case 64's block (identical); the inner
 * loop's `D_80073740[j]` walked as a pointer (462/250) or read into a local
 * used three times (462/186) -- both drop one of `j`'s references, which is
 * the arithmetic that would flip it, and both cost a word; and
 * `e->unk_0C`/`e->unk_14` read back as the third argument instead of `d`.
 */

#include "common.h"

extern s32 D_80073734a[] __asm__("D_80073734");
extern s16 D_80073738[];

void func_80055E7C();
void func_80056654(u8 *, s32);
void func_800561D8(u8 *, Unk80084960 *);
void func_800563B0(s32, u16, u16);
void func_800564A8(void);
void func_80056A08(void);
void func_80057A94(u8 *);
s32 func_8005E97C(s32, s32);
s32 func_8005EB78(s32);

void func_80057D20(u8 ch, u8 cc, u8 d)
{
    Unk80084960 *e = &D_80084960[ch];
    s32 i;
    s32 f1 = 0;
    s32 f2 = 0;
    s32 k;
    s32 n;

    switch (cc)
    {
    case 1:
        if (e->unk_48 < 0x40)
        {
            e->unk_08 = d;
            e->unk_40 = d << 1;
            if (e->unk_44 != 0 && e->unk_40 != 0)
            {
                e->unk_38 = e->unk_40 * 4 / e->unk_44;
            }
            else
            {
                n = D_80073734a[0];
                i = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i++;
                    } while (i < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i++;
                    } while (i < D_80073734a[0]);
                }
            }
        }
        else
        {
            e->unk_08 = d;
            e->unk_44 = d;
            if (e->unk_44 != 0 && e->unk_40 != 0)
            {
                e->unk_38 = e->unk_40 / e->unk_44 * 2;
            }
            else
            {
                n = D_80073734a[0];
                i = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i++;
                    } while (i < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i++;
                    } while (i < D_80073734a[0]);
                }
            }
        }
        func_80055E7C(4, ch, e->unk_08);
        break;
    case 2:
        if (e->unk_48 < 0x40)
        {
            e->unk_44 = d;
        }
        else
        {
            e->unk_40 = d << 1;
        }
        break;
    case 3:
        e->unk_48 = d;
        break;
    case 4:
        e->unk_28 = d;
        break;
    case 5:
        e->unk_50 = d;
        break;
    case 7:
        e->unk_0C = d;
        f1 = 1;
        func_80055E7C(3, ch, d);
        break;
    case 10:
        if (d == 0)
        {
            e->unk_04 = 1;
        }
        else
        {
            e->unk_04 = d;
        }
        f1 = 1;
        func_80055E7C(2, ch, e->unk_04);
        break;
    case 11:
        e->unk_14 = d;
        f1 = 1;
        func_80055E7C(5, ch, d);
        break;
    case 12:
        e->unk_2C = d;
        f1 = 1;
        break;
    case 20:
        e->unk_6C = d << 1;
        break;
    case 21:
        e->unk_6E = d;
        break;
    case 22:
        e->unk_68 = d << 4;
        e->unk_64 = d << 4;
        e->unk_6D = 0;
        e->unk_5D = 1;
        break;
    case 23:
        e->unk_6D = d;
        if (d != 0)
        {
            if (e->unk_68 != 0)
            {
                e->unk_70 = e->unk_68 / d;
                if (e->unk_70 == 0)
                {
                    e->unk_70 = 1;
                }
            }
        }
        break;
    case 25:
        e->unk_88 = d << 1;
        break;
    case 26:
        e->unk_8A = d;
        break;
    case 27:
        e->unk_84 = d << 7;
        e->unk_80 = d << 7;
        e->unk_89 = 0;
        e->unk_78 = 1;
        break;
    case 28:
        e->unk_89 = d;
        if (d != 0)
        {
            if (e->unk_84 != 0)
            {
                e->unk_8C = e->unk_84 / d;
                if (e->unk_8C == 0)
                {
                    e->unk_8C = 1;
                }
            }
        }
        break;
    case 30:
        D_80073738[0] = d;
        func_800564A8();
        break;
    case 64:
        for (i = 0; i < D_80073734a[0]; i++)
        {
            u8 *q = D_80085458 + i * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (d < 0x40)
                {
                    s32 j;

                    for (j = 0; j < D_80073734a[0]; j++)
                    {
                        u8 *r = D_80085458 + j * 0x78;

                        if (*(u16 *) (r + 6) == ch && r[0x1D] != 0 && (r[0x1C] & 0x80) != 0)
                        {
                            f2 |= D_80073740[j];
                            do
                            {
                                func_8005E97C(0, D_80073740[j]);
                                k = func_8005EB78(D_80073740[j]);
                            } while (k != 2 && k != 0);
                            r[0x1D] = 0;
                            *(u16 *) (r + 0x1A) = 0;
                        }
                    }
                    q[0x1D] = 0;
                }
                else
                {
                    q[0x1D] = 1;
                }
            }
        }
        if (d < 0x40)
        {
            D_80084960[ch].unk_18 = 0;
        }
        else
        {
            D_80084960[ch].unk_18 = 1;
        }
        break;
    case 91:
        e->unk_30 = d & 0x7F;
        break;
    case 6:
        e->unk_4E = d;
        if (e->unk_4D != 0x14 && e->unk_4D != 0x1E)
        {
            func_80057A94((u8 *) e);
        }
        break;
    case 98:
        e->unk_4C = d;
        break;
    case 99:
        e->unk_4D = d;
        break;
    case 120:
    case 121:
    case 123:
        func_80056A08();
        break;
    case 126:
        e->unk_98 = d;
        break;
    }
    if (f2 != 0)
    {
        func_8005E97C(0, f2);
    }
    if (f1 != 0)
    {
        for (i = 0; i < D_80073734a[0]; i++)
        {
            u8 *q = D_80085458 + i * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (*(u16 *) (q + 0x1A) != 0)
                {
                    func_800561D8(q, &D_80084960[ch]);
                    func_800563B0(i, *(u16 *) (q + 0x10), *(u16 *) (q + 0x12));
                }
            }
        }
    }
}
