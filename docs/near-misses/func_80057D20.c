/* func_80057D20 (unk_1C540) -- 463 of 463 words, 112 DIFF. NOT compiled.
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
 * THE WORD COUNT IS EXACT and the 112 differing words are ONE shifted region,
 * lines 194..253 of the side-by-side -- cases 7 through 25. Everything else
 * matches instruction for instruction, including both case-1 arms, all four
 * of its loops, case 64's nested loops and its inner do/while, the two
 * portamento-shaped groups at +0x64 and +0x80, and the tail's two flag tests.
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
 * * `func_80055E7C` is called with FOUR arguments in cases 7, 10 and 11 and
 *   with THREE in case 1's tail, so it has no prototype in the original:
 *   declare it `void func_80055E7C();`. With a 4-argument prototype case 1
 *   spends a word setting $a3 that the listing does not have.
 *
 * * Case 1's first arm READS BACK `e->unk_40` and its second arm reads back
 *   `e->unk_44`, both just stored. cse forwards each load to the register
 *   holding the stored value, which is why there is no second load -- and the
 *   read-back is what makes the else arm's divide `divu`: `e->unk_44` is
 *   `u32`, where `d` is `u8` and would promote to signed `int` and give
 *   `div`. advise.py's SIGNEDNESS row names exactly this.
 *
 * * `d << 1` rather than `d * 2` in case 1's first arm. `* 2` is narrowed by
 *   `build_binary_op`'s `shorten` and shares the store's mask of `d`; the
 *   shift keeps its own, which is the listing's second `andi $a0, $s4, 0xFF`.
 *   The same choice goes the OTHER way in cases 20 and 25, where the
 *   destination is a byte: there `* 2` narrows to the listing's bare
 *   `sll $v0, $s4, 1` and `<< 1` adds a mask the listing does not have.
 *   Two words each way, and the two cases want opposite spellings.
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
 * * `f1 = 1` sits BEFORE the call in cases 7 and 11 and AFTER it in case 10.
 *   That asymmetry is worth 155 DIFF and is not cosmetic: with it before in
 *   case 10 as well, cross-jumping merges case 10's call into the shared site
 *   at .L8005807C and the function needs a `move $a3, $t0` it does not have.
 *
 * WHAT IS LEFT -- one word misplaced inside cases 7..25, and its knock-on.
 *
 * The listing DUPLICATES `lbu $a1, 0x10($sp)` in cases 7 and 11 and shares
 * only `jal` + `sw $a3, 0x14($sp)`; here the `lbu` is in the shared tail too,
 * so case 7 is a word short, and case 25 spends a word the listing does not
 * (its `andi` -- see the `* 2` note above; fixing that one alone leaves the
 * function two words short at 461).
 *
 * Underneath both is the constant 1: the listing materialises it into $a3
 * separately in each of cases 7, 10, 11 and 12, so the value that reaches
 * `sw $a3, 0x14($sp)` IS the call's fourth argument. Here it is one global
 * allocno in $t0 with a `move $a3, $t0` at the call, which changes what
 * cross-jumping can merge and what sched2 leaves for the delay slot. Measured
 * and NOT working: `f1` passed as the fourth argument, `f1` and `f2` declared
 * `volatile` (the listing's load/or/store of f2 inside case 64's loop looks
 * like volatile but is not -- 467 words), every before/after placement of
 * `f1 = 1` in the three cases, and the field store moved after the call.
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
        func_80055E7C(3, ch, d, 1);
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
        func_80055E7C(2, ch, e->unk_04, 1);
        f1 = 1;
        break;
    case 11:
        e->unk_14 = d;
        f1 = 1;
        func_80055E7C(5, ch, d, 1);
        break;
    case 12:
        e->unk_2C = d;
        f1 = 1;
        break;
    case 20:
        e->unk_6C = d * 2;
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
        e->unk_88 = d * 2;
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
