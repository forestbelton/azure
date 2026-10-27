/* func_8005914C (unk_1C540) -- 205 of 204 words, 131 DIFF. NOT compiled.
 *
 * The MIDI channel-message dispatcher: `switch (st & 0xF0)` over the seven
 * status nibbles 0x80..0xE0, with the channel in the low nibble. 0x80, 0x90,
 * 0xA0, 0xC0, 0xD0 and 0xE0 are one call each; 0xB0, the controller change,
 * is 128 of the 204 words and holds all three loops.
 *
 * EVERY WORD OUTSIDE THE FIRST TWO LOOPS MATCHES, including the dispatch
 * tree, all seven arms, the 0x63/0x14 and 0x63/0x1E controller bodies, the
 * `D_80084960[ch].unk_4D == 0x14` test with its 0x9C-byte index chain, the
 * third loop, and the early `return` out of the 0x06 controller's else arm.
 * The gap is ONE WORD: a load-delay `nop` in each of the first two loops
 * against one insn fewer somewhere else in this source.
 *
 * WHAT IS PAID FOR
 *
 * * The case bodies are in SOURCE order 0x80, 0x90, 0xA0, 0xB0, 0xE0, 0xC0,
 *   0xD0 -- read off the body addresses, since the dispatch is a balanced
 *   comparison tree (seven cases spanning 0x60, and `range > 10 * count` is
 *   false, so no jump table) and the tree order says nothing about the
 *   source. 0xE0 before 0xC0 and 0xD0 is not a typo.
 *
 * * `st`, `d1` and `d2` are `u8` PARAMETERS and `ch` is a `u8` local. That is
 *   what puts an `andi 0xFF` at every use rather than once at entry, which is
 *   what the listing has -- fourteen of them.
 *
 * * The two save/restore loops are WHILE loops, not guarded do-whiles. This
 *   is worth two words and it is not a cosmetic choice: loop.c's
 *   `duplicate_loop_exit_test` copies a top-tested loop's exit test to the
 *   bottom, which is where the listing's guard-plus-bottom-test shape comes
 *   from, and because both tests then come from ONE source expression they
 *   share the address pseudo for D_800869B4 -- the listing's single
 *   `lui $t4` serving the guard's load and the in-loop load. Written as
 *   `if (D_800869B4n[0] != 0) { do { ... } while (i < D_800869B4n[0]); }` the
 *   two reads are two expressions, cse rewrites the second `(high)` into a
 *   reg-reg copy of the first, and each loop pays an extra `move`.
 *
 * * The entry pointer is the INLINE indexed form, `D_80085FA8s[i].unk_04`,
 *   not a pointer local. A `Unk80085FA8 *e` assigned from the index inside
 *   the body is a separate pseudo that loop.c loads from the giv register
 *   every iteration (`move $t0, $t2` plus `addiu $t2, $t0, 0x50` where the
 *   listing has one `addiu $t0, $t0, 0x50`), and a WALKED pointer is worse
 *   still: loop.c biases a second giv to `e + 0x30` and rewrites half the
 *   fields as negative displacements off it, which is four words.
 *
 * * The third loop IS the guarded do-while with its bound in a local
 *   (`n = D_800869B4n[0]; if (n != 0) ...`), because the listing keeps that
 *   bound in $a0 across the loop instead of re-reading it. Only the first two
 *   loops re-read.
 *
 * * The third loop also gets its OWN counter, `ic`, rather than sharing `i`
 *   with the first two. One word, and the lever is the one func_80057D20
 *   measured seven times over: a counter reused across disjoint loops is a
 *   single allocno whose reference count SUMS over all of them, and the
 *   inflated priority steals a low register. Splitting the first two as well
 *   changes nothing (they are the pair that re-reads the bound, so they
 *   behave alike), and splitting one of those two instead is 206 again.
 *
 * WHAT IS LEFT -- one word, behind the two load-delay nops.
 *
 * The listing puts each of the first two loops' `lw %lo(D_800869B4)($t4)` in
 * the MIDDLE of the body, after the first two stores and before the last
 * four, with `addiu $t1, $t1, 1` behind it covering the load delay. Here the
 * load stays at the end, where the source's `while` condition puts it, and
 * the delay needs a `nop`. sched cannot move it up because the load's base is
 * a symbol and the stores go through the loop's giv, so `true_dependence` has
 * to assume they may alias; the listing's placement means its load was
 * already mid-body before sched ran.
 *
 * Reading the bound into a local in the middle of the body DOES move it
 * (`m = D_800869B4n[0];` after the second store kills both nops) -- but then
 * the bound is a second source expression and the `move` for the address
 * pseudo comes back, so the word comes straight back with it.
 *
 * ALSO MEASURED 2026-10-27, after func_80057D20 taught the guard-plus-do-while
 * ordering that killed the same two nops there: it does NOT transfer. The
 * guarded form (`n = D_800869B4n[0]; i = 0; if (n > 0) { do ... while (i <
 * D_800869B4n[0]); }`) is 208 words here, because the guard's read and the
 * loop's read become two expressions and each loop pays a second `lui`; the
 * `while` form's single `lui $t4` is what the listing has, and
 * `duplicate_loop_exit_test` is what gives it both tests. The difference
 * between the two functions is real: func_80057D20's listing materialises
 * %hi(D_80073734) twice per loop, this one's materialises %hi(D_800869B4)
 * once.
 *
 * And the five loaded values as explicit LOCALS, read in the listing's load
 * order (0x00, 0x10, 0x40, 0x2C, then the byte at 0x4A) and stored after, is
 * 205 words -- it kills ONE nop -- but 148 DIFF against 131, because the
 * listing's five live values in $v0/$v1/$a0/$a1/$a2 come out of sched
 * hoisting the loads rather than out of five source temps. Both loops
 * separately and the byte read first were measured too, as was that form
 * combined with the third loop's own counter (still 148). Measured besides:
 * every position 0..6 for the bound's read, with and without a pre-loop
 * `m = D_800869B4n[0];`, both loop forms, all three entry-pointer forms,
 * `i++` at three positions, the bound through a `u32 *b` pointer local, and
 * the third loop as a `while`, a guarded do-while, with a `u8` count and with
 * the count sharing `m`. The best is the 205/131 below; the best DIFF with
 * the nops gone is 139.
 *
 * So the lever wanted is one that moves the load mid-body WITHOUT making it a
 * second expression -- or an alias fact that lets sched hoist it past the
 * stores. See docs/matching-levers.md on sched1's memory dependence. */

#include "common.h"

extern u32 D_800869B4n[] __asm__("D_800869B4");
extern u32 D_800869B8[];

void func_80056E10(u8, u8, u8);
void func_80057948(u8, u8, u8);
void func_80057A48(u8, u8, u8);
void func_80057D20(u8, u8, u8);
void func_8005845C(u8, u8);
void func_8005848C(u8, u8);
void func_80058494(u8, u8, u8);

void func_8005914C(Unk80085FA8 *p, u8 st, u8 d1, u8 d2)
{
    u8 ch = st & 0xF;
    u32 i;
    u32 ic;
    u32 n;

    switch (st & 0xF0)
    {
    case 0x80:
        func_80057948(ch, d1, d2);
        break;
    case 0x90:
        if (d2 != 0)
        {
            func_80056E10(ch, d1, d2);
        }
        else
        {
            func_80057948(ch, d1, 0);
        }
        break;
    case 0xA0:
        func_80057A48(ch, d1, d2);
        break;
    case 0xB0:
        if (d1 == 0x63)
        {
            if (d2 == 0x14)
            {
                if (D_800869B8[0] != 0)
                {
                    i = 0;
                    while (i < D_800869B4n[0])
                    {
                        D_80085FA8s[i].unk_04 = D_80085FA8s[i].unk_00;
                        D_80085FA8s[i].unk_4B = D_80085FA8s[i].unk_4A;
                        D_80085FA8s[i].unk_08 = 0x14;
                        D_80085FA8s[i].unk_14 = D_80085FA8s[i].unk_10;
                        D_80085FA8s[i].unk_44 = D_80085FA8s[i].unk_40;
                        D_80085FA8s[i].unk_30 = D_80085FA8s[i].unk_2C;
                        i++;
                    }
                }
                else
                {
                    p->unk_08 = 0x7F;
                    p->unk_04 = p->unk_00;
                }
            }
            else if (d2 == 0x1E)
            {
                if (p->unk_08 != 0)
                {
                    if (p->unk_08 < 0x7F)
                    {
                        p->unk_08 = p->unk_08 - 1;
                    }
                    if (D_800869B8[0] != 0)
                    {
                        i = 0;
                        while (i < D_800869B4n[0])
                        {
                            D_80085FA8s[i].unk_00 = D_80085FA8s[i].unk_04;
                            D_80085FA8s[i].unk_4A = D_80085FA8s[i].unk_4B;
                            D_80085FA8s[i].unk_10 = D_80085FA8s[i].unk_14;
                            D_80085FA8s[i].unk_40 = D_80085FA8s[i].unk_44;
                            D_80085FA8s[i].unk_2C = D_80085FA8s[i].unk_30;
                            i++;
                        }
                    }
                    else
                    {
                        p->unk_00 = p->unk_04;
                    }
                }
            }
        }
        if (d1 == 6)
        {
            if (D_80084960[ch].unk_4D == 0x14)
            {
                if (D_800869B8[0] != 0)
                {
                    n = D_800869B4n[0];
                    if (n != 0)
                    {
                        ic = 0;
                        do
                        {
                            D_80085FA8s[ic].unk_08 = d2;
                            ic++;
                        } while (ic < n);
                    }
                }
                else
                {
                    p->unk_08 = d2;
                    return;
                }
            }
        }
        func_80057D20(ch, d1, d2);
        break;
    case 0xE0:
        func_80058494(ch, d1, d2);
        break;
    case 0xC0:
        func_8005845C(ch, d1);
        break;
    case 0xD0:
        func_8005848C(ch, d1);
        break;
    }
}
