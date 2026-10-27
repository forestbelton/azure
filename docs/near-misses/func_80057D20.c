/* func_80057D20 (unk_1C540) -- 463 of 463 words, 6 DIFF. NOT compiled.
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
 * THE WORD COUNT IS EXACT, EVERY OPCODE MATCHES AND SO DOES EVERY REGISTER.
 * The six differing words are the ORDER of two giv initialisations and their
 * two increments inside case 64's inner loop -- see the end of this comment.
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
 * THE SEVEN LOOP COUNTERS ARE SEVEN VARIABLES, and that is what fixed 48 of
 * the 54 words this was stuck on. Sharing one `i` across the four case-1
 * loops, case 64's outer loop and the final loop makes one allocno with 46
 * references over 155 live insns (priority 1.484), which outranks the entry
 * pointer (56/206, 1.359) and `d` (34/177, 0.960) and takes their registers;
 * the listing's own register REUSE says the opposite -- $s2 serves case 1's
 * loops AND case 64's INNER loop, $s5 serves case 64's outer loop and the
 * final one -- which only works if they are separate variables with disjoint
 * live ranges. Four separate counters for case 1's loops (7 refs / 17 live
 * each, 0.824), one shared with case 64's inner loop, and one more for the
 * outer and final loops puts `e` back in $s1 and `d` in $s4 and every other
 * register where the listing has it. Register reuse across disjoint ranges
 * does not mean one variable -- but splitting on it is still the right move,
 * for the priorities rather than the reuse.
 *
 * WHAT IS LEFT -- six words, and they are ONE ORDERING. In case 64's inner
 * loop the listing initialises the D_80085458 giv before the D_80073740 one
 * and increments them in that order; this source does both the other way
 * round. The registers are already right ($s1 for D_80085458, $s3 for
 * D_80073740): only the order of the two lui/addiu pairs and of the two
 * increments differs.
 *
 * The mechanism, read out of the -dl dump rather than guessed (the loop pass
 * prints every giv it records, every combination it makes and every reduction,
 * so this is not inference): loop.c's `record_giv` PREPENDS to the biv's giv
 * list, the reduction loop at loop.c:3856 walks that list, and both the
 * initialisations before `loop_start` and the increments before the biv's own
 * increment are emitted in list order -- so EMISSION IS THE REVERSE OF
 * CREATION, and creation is insn order. `u8 *r = D_80085458 + j * 0x78;` is
 * the body's first insn, so r's giv is created first and emitted second.
 *
 * And the D_80073740 giv cannot be created before it, because of
 * `combine_givs` (loop.c:5494). Every D_80073740 reference in the loop -- the
 * `f2 |=` load, the two loads inside the do/while, and the copy that the
 * do/while's own invariant hoist leaves in its preheader -- has the same
 * mult (4) and the same add (the %lo pseudo), so they are all one combinable
 * group, and the representative is the FIRST eligible `g1` in the giv list,
 * i.e. the LAST created member of the group. The dump says it plainly:
 *
 *     giv at 890 combined with giv at 1316
 *     giv at 888 combined with giv at 1316
 *     giv at 886 combined with giv at 1315
 *     giv at 1316 reduced to (reg:SI 416)
 *     giv at 860 reduced to (reg:SI 418)
 *
 * -- 1315/1316 being the hoisted copies move_movables made for the do/while,
 * which the loop pass creates LAST because loops are processed innermost
 * first. So the surviving D_80073740 giv is always the latest one, and
 * putting a reference ahead of `r` in the source does not help: the early giv
 * is simply combined INTO the late one and loses its place in the list. That
 * is what `u32 *g = &D_80073740[j];` ahead of `r` does -- the dump shows
 * `giv at 857 combined with giv at 906` -- and it also costs the
 * `addu $s0, $s3, $zero` copy, because `*g` in the do/while is then already a
 * pseudo and has nothing left to hoist: 462 words.
 *
 * For the listing's order, r's giv would have to be the one created last.
 * Everything that gets a D_80085458 giv created after the do/while also
 * changes which member represents THAT group, and the representative's
 * add_val becomes the base: a source-level walked pointer (`u8 *r =
 * D_80085458;` with `r += 0x78`) makes r a biv, every field access a giv on
 * it, and the group's representative the last one -- the `+0x1A` store -- so
 * the pointer comes out BIASED to +0x1A with displacements -20, 3, 2, 3, 0.
 * That is 463 words and 13 DIFF, and it is the closest any reordering gets.
 * Spelling the two stores after the do/while as fresh `D_80085458[j * 0x78 +
 * 0x1D]` index expressions is the same trap at +0x1D (472/215).
 *
 * MEASURED AND NOT WORKING for those six: a `u32 *t = &D_80073740[j];`
 * declared ahead of `r` (unused, identical; used for all three accesses,
 * 459/363); the same pointer declared inside the `if` (462/262); the
 * `f2 |= D_80073740[j]` moved after the do/while (463/45); the three
 * `r`-based conditions written inline off D_80085458 so that `r` is not
 * computed first (477 words -- cse does not share the index chain); the
 * inner counter as `u32`; `j` at function scope rather than in case 64's
 * block; both source-level WALKED pointers for the two arrays instead of the
 * indexed form, which is where the order WOULD be the declaration's (466
 * words, 331 DIFF); `0x78 * i` instead of `i * 0x78`; `&D_80085458[i * 0x78]`;
 * `*(D_80073740 + i)`; and the three conditions as nested `if`s. The last
 * five are all 463 and 6, so the ordering is robust to how either reference
 * is spelled -- it is the giv list, not the expression.
 *
 * MEASURED AND NOT WORKING earlier, kept so they are not retried: `f1`
 * passed as an argument (case 10 then LOADS it back, so f1 really is
 * spilled); `f1` and `f2` declared `volatile` -- the listing's load/or/store
 * of f2 inside case 64's loop looks like volatile but is not, 467 words;
 * every before/after placement of `f1 = 1`; the field store moved after the
 * call; the inner loop's `D_80073740[j]` walked as a pointer (462/250) or
 * read into a local used three times (462/186); and `e->unk_0C`/`e->unk_14`
 * read back as the third argument instead of `d`.
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
    s32 i1;
    s32 i2;
    s32 i3;
    s32 i4;
    s32 f1 = 0;
    s32 f2 = 0;
    s32 k;
    s32 kk;
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
                i1 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i1 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i1++;
                    } while (i1 < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i2 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i2 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i2++;
                    } while (i2 < D_80073734a[0]);
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
                i3 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i3 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i3++;
                    } while (i3 < D_80073734a[0]);
                }
            }
            if (d == 0 || e->unk_38 == 0)
            {
                n = D_80073734a[0];
                i4 = 0;
                e->unk_3A = 0;
                if (n > 0)
                {
                    do
                    {
                        u8 *q = D_80085458 + i4 * 0x78;

                        if (*(u16 *) (q + 6) == ch)
                        {
                            func_80056654(q, 1);
                        }
                        i4++;
                    } while (i4 < D_80073734a[0]);
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
        for (kk = 0; kk < D_80073734a[0]; kk++)
        {
            u8 *q = D_80085458 + kk * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (d < 0x40)
                {
                    for (i1 = 0; i1 < D_80073734a[0]; i1++)
                    {
                        u8 *r = D_80085458 + i1 * 0x78;

                        if (*(u16 *) (r + 6) == ch && r[0x1D] != 0 && (r[0x1C] & 0x80) != 0)
                        {
                            f2 |= D_80073740[i1];
                            do
                            {
                                func_8005E97C(0, D_80073740[i1]);
                                k = func_8005EB78(D_80073740[i1]);
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
        for (kk = 0; kk < D_80073734a[0]; kk++)
        {
            u8 *q = D_80085458 + kk * 0x78;

            if (ch == *(u16 *) (q + 6))
            {
                if (*(u16 *) (q + 0x1A) != 0)
                {
                    func_800561D8(q, &D_80084960[ch]);
                    func_800563B0(kk, *(u16 *) (q + 0x10), *(u16 *) (q + 0x12));
                }
            }
        }
    }
}
