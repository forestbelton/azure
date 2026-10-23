/* A carved-out translation unit: these two consecutive functions reach
 * D_80081510 $gp-relatively and nothing else does, so they were one original
 * translation unit. gas is left at -G8 here rather than maspsx's forced -G0
 * (MASPSXFLAGS_G8 in the Makefile); no window definition and no linker
 * placement needed -- see docs/addressing-and-translation-units.md.
 *
 * Audited before carving: the only symbol reached absolutely is D_80083160,
 * already `extern Unk80083160` at 0x1F2 bytes and therefore compiler-split, so
 * -G8 does not touch it. (At -G8 gas makes any small UNDEFINED symbol
 * $gp-relative, so every absolutely-reached symbol in a carved unit must be
 * checked.) */
/* Another piece of the original unk_14934 chain, carved off when the
 * gprel_17B7C $gp cluster was taken out of the middle of unk_176B8. Named for
 * its start offset. See docs/addressing-and-translation-units.md.
 */
/* The tail of the original unk_14934 split, carved off when gprel_17504 was
 * taken out of the middle of it: a $gp cluster cannot be compiled with the
 * same -G as its neighbours, so the unit had to be cut in three. Named for its
 * start offset, like the other provisional units. See
 * docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u32, u32);
void func_8003F6D4(s32, s32, s32 *, s32);
void func_80040B88(void);
void func_80042984(u8 *);
void func_80041AB0(void);
void func_80041AE4(void);
void func_80043CD0(void);
void func_80043D04(void);
s32  func_80042518(s8 *, s8);
UnkNode *func_8003FC64(s32);
void func_80043FD0(Unk44144 *);
s32  func_8003F794(s16, s16);
void func_80040454(void);

/* Links the node into one of the 0x20 tag slots at D_80083360/D_800833E0,
 * pushing any existing head down, or claims a free slot. Returns 0 only when
 * every slot is taken.
 *
 * Three shapes are load-bearing:
 *  - `D_800833E0[i] = (u32) n;` is duplicated into both arms of the null
 *    check. That gives the store its own basic block instead of putting it in
 *    the join block, so sched1 never has it and the `return 1` constant in one
 *    ready list, where the hazard heuristic always hoists the constant past
 *    it. jump2's cross-jumping then merges the two copies back and plants the
 *    join label in front of the store, which is why .L80044964 sits before it
 *    and the word count is unchanged.
 *  - the free-slot bookkeeping is `if (first == -1) { first = i; } last = i;`
 *    in that order. With `last = i` first, cse folds the second assignment to
 *    a copy of last's register; in this order the `last = i` insn is instead
 *    stolen into the bne's delay slot. The test has to be against -1, not
 *    `< 0`: the target compares against a loop-hoisted -1 in a register.
 *  - `return 0` is the last statement, so its block lands just before the
 *    epilogue and the test is a single bltz. An early return costs a bgez, a
 *    `j` and a move.
 *
 * The two tails of the D_8006E7B4 walk are genuinely duplicated in the target
 * -- they differ only in which index they use -- so they are written out
 * twice here as well. */

/* Walks the node list, drawing each slot through func_800453E0 and stopping
 * early if func_80045310 reports the OT is full.
 *
 * This is the fifth member of a family: func_80044BB0, func_80044C54,
 * func_80045C34 and func_80046398 are the same node walk with a different
 * per-slot callee, and func_80045C34 is the exact skeleton. **Start there if
 * you are writing anything else in this unit** -- that is worth more than any
 * lever.
 *
 * Two things in the listing that look like source and are not: there is only
 * ONE `return 0` worth of `addu $v0,$zero,$zero` in the source -- jump2
 * cross-jumped the two return tails and reorg then stole the survivor into the
 * bnez's delay slot and retargeted the branch past it, which is why the branch
 * appears to skip the `sw $zero, %gp_rel(D_80081510)` while still returning 0.
 * And `*(u32 *) 0x1F80013C = 0;` is a single-use constant address, so it
 * collapses to the two-word absolute form (matching-levers section 6). */
s32 func_80045340(u8 *p, UnkVec *v, UnkSlot *s) {
    D_80081510 = 0;
    *(u32 *) 0x1F80013C = 0;
    for (;;) {
        UnkNode *n;

        if (!(s->unk_14 & 0x80)) {
            func_800453E0(p, v, s, s->unk_06);
            if (func_80045310(*(u32 *) (D_80083160.unk_00 + 0x8D0)) != 0) {
                return 0;
            }
        }
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            D_80081510 = 0;
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

/* Emits one slot's primitives: projects the slot origin, derives an OT index
 * from the averaged z, builds the matrix in the 0x1F800000 scratchpad, then per
 * 0xC-byte element either calls the element's own callback or clips, projects
 * and links one 0x28-byte primitive into the OT.
 *
 * LOAD-BEARING, and the one that took the longest to find: the two OT-linking
 * statements are BITFIELD stores through Unk453E0T (`u32 addr:24; u32 len:8`),
 * which is libgpu's P_TAG -- i.e. the original wrote `addPrim(ot, q)`, whose
 * expansion is `setaddr(q, getaddr(ot)), setaddr(ot, q)`. They are NOT
 * hand-written masks, and no spelling of a hand-written `|` can match here.
 *
 * Why the bitfield is required, from gcc 2.7.2 source plus the -da loop dump
 * rather than inference. The target needs two things at once from one
 * statement: `q->unk_00 & 0xFF000000` as the `or`'s operand 0, and 0xFFFFFF
 * hoisted by loop.c. A written `|` cannot give both, because `expand_binop`
 * materialises operand 0's chain first, so whichever constant is set first wins
 * the hoist and that is the one that must also be operand 0. The bitfield
 * expansion (expmed.c `store_fixed_bit_field`) emits the VALUE first -- load
 * `*ot`, set 0xFFFFFF, mask -- then the lvalue's clear-mask -- load
 * `q->unk_00`, set 0xFF000000, mask -- and then `ior` with the q side as
 * operand 0. So 0xFFFFFF gets the earlier set and the longer lifetime while the
 * q term is still operand 0.
 *
 * And the hoist itself is a cost test, not a rule: `move_movables`
 * (loop.c:1631) hoists iff `threshold * savings * lifetime >= insn_count`, with
 * savings = n_times_SET (1 for a single-set pseudo), threshold starting at 29
 * here, and `threshold -= 3` after every successful move (loop.c:1718). The
 * first-set constant is therefore tested at 29 with the longer lifetime and the
 * second at 26 with the shorter -- 0xFFFFFF at life 15 gives 435 >= 369 and
 * moves, 0xFF000000 at life 13 gives 338 < 369 and stays inline. See
 * docs/matching-levers.md.
 *
 * Also load-bearing: `D_8006CD30` must be declared as a struct larger than -G8
 * (include/structs.h) or gas makes it $gp-relative in this -G8 unit; and the
 * address in both statements is the plain three-term sum -- the `cc4 =
 * p->unk_CC * 4` hoist that an earlier near-miss needed is not wanted here and
 * costs 2 words. */
void func_800453E0(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3) {
    Unk80083160 *g = &D_80083160;
    Unk453E0SP *p = (Unk453E0SP *) 0x1F800000;
    u8 *base = D_80083160.unk_00;
    Unk453E0P *q;
    Unk44D24E *e;
    s32 w;
    s32 h;

    p->unk_EC = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    p->unk_20 = base + 0xB0;
    p->unk_00 = ((u16 *) v)[1];
    p->unk_02 = ((u16 *) v)[3];
    p->unk_04 = ((u16 *) v)[5];
    q = *(Unk453E0P **) (base + 0x8D0);
    p->unk_C0 = RotTransPers((void *) 0x1F800000, (void *) 0x1F8000B8,
                             (void *) 0x1F800090, (void *) 0x1F800094);
    s->unk_14 |= 0x8000;
    w = p->unk_C0 - 0xA;
    D_8006CD30.unk_1C = p->unk_C0 * 4;
    p->unk_C0 = w - arg3;
    if ((u32) p->unk_C0 < 0x1D6) {
        PushMatrix();
        p->unk_B8 = p->unk_B8 - 0xA0;
        p->unk_BA = p->unk_BA - 0x78;
        p->unk_30.w = g->unk_C4;
        p->unk_34.w = g->unk_C6;
        p->unk_38.w = g->unk_C8;
        p->unk_100 = s->unk_16;
        p->unk_104 = g->unk_B8 + (s->unk_1A - p->unk_34.h);
        h = s->unk_18 - 0x100;
        p->unk_102 = h + ((p->unk_38.h + 0x100) & 0x1FF);
        p->unk_108 = p->unk_E4 = s->unk_20;
        p->unk_10A = p->unk_E8 = s->unk_22;
        RotMatrix((void *) 0x1F800100, (void *) 0x1F8000D0);
        p->unk_30.w = s->unk_1C;
        p->unk_34.w = s->unk_1E;
        p->unk_38.w = 0x1000;
        ScaleMatrix((void *) 0x1F8000D0, (void *) 0x1F800030);
        CompMatrix(&D_8006CD30, (void *) 0x1F8000D0, (void *) 0x1F800050);
        SetTransMatrix((void *) 0x1F800050);
        SetRotMatrix((void *) 0x1F800050);
        e = (Unk44D24E *) s->unk_08;
        p->unk_24 = s->unk_14;
        for (;;) {
            if ((e->unk_00 & 0x20) == 0) {
                s32 x;
                s32 y;
                s16 a;
                s32 n;

                x = e->unk_08;
                p->unk_08.w = x;
                y = e->unk_0A;
                p->unk_10.w = y;
                if (x + y >= 0x100 || s->unk_1A != 0) {
                    p->unk_10.w = y - 1;
                }
                x = e->unk_09;
                p->unk_0C.w = x;
                y = e->unk_0B;
                p->unk_14.w = y;
                if (x + y >= 0x100 || s->unk_1A != 0) {
                    p->unk_14.w = y - 1;
                }
                if ((e->unk_00 ^ p->unk_24) & 1) {
                    a = -(s8) ((u8 *) e)[2] - p->unk_108;
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a - p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                } else {
                    a = (s8) ((u8 *) e)[2] - p->unk_108;
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                }
                if ((e->unk_00 ^ p->unk_24) & 2) {
                    a = -(s8) ((u8 *) e)[3] - p->unk_10A + p->unk_13C;
                    p->unk_7A = a;
                    p->unk_72 = a;
                    a = a - p->unk_14.h;
                    p->unk_8A = a;
                    p->unk_82 = a;
                } else {
                    a = (s8) ((u8 *) e)[3] - p->unk_10A + p->unk_13C;
                    p->unk_7A = a;
                    p->unk_72 = a;
                    a = a + p->unk_14.h;
                    p->unk_8A = a;
                    p->unk_82 = a;
                }
                RotTransPers4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                              &p->unk_F0, &p->unk_F4, &p->unk_F8, &p->unk_FC,
                              &p->unk_90, &p->unk_94);
                q->unk_08 = p->unk_F0 + p->unk_B8;
                q->unk_0A = p->unk_F2 + p->unk_BA;
                q->unk_10 = p->unk_F4 + p->unk_B8;
                q->unk_12 = p->unk_F6 + p->unk_BA;
                q->unk_18 = p->unk_F8 + p->unk_B8;
                q->unk_1A = p->unk_FA + p->unk_BA;
                q->unk_20 = p->unk_FC + p->unk_B8;
                q->unk_22 = p->unk_FE + p->unk_BA;
                if ((((u16) (q->unk_08 + 0x20) < 0x181 &&
                      (u16) (q->unk_0A + 0x20) < 0x121) |
                     ((u16) (q->unk_10 + 0x20) < 0x181 &&
                      (u16) (q->unk_12 + 0x20) < 0x121) |
                     ((u16) (q->unk_18 + 0x20) < 0x181 &&
                      (u16) (q->unk_1A + 0x20) < 0x121) |
                     ((u16) (q->unk_20 + 0x20) < 0x181 &&
                      (u16) (q->unk_22 + 0x20) < 0x121)) != 0) {
                    u8 t; u8 f;

                    ((u8 *) &q->unk_00)[3] = 9;
                    s->unk_14 &= 0x7FFF;
                    p->unk_10.w = p->unk_10.w + p->unk_08.w;
                    p->unk_14.w = (p->unk_14.w + p->unk_0C.w) << 8;
                    p->unk_0C.w = p->unk_0C.w << 8;
                    if (p->unk_24 & 0x100) {
                        q->unk_0E = s->unk_12;
                    } else {
                        q->unk_0E = s->unk_12 + e->unk_06;
                    }
                    q->unk_0C = p->unk_0C.h + p->unk_08.h;
                    q->unk_14.h = p->unk_0C.h + p->unk_10.h;
                    if (s->unk_10 != 0) {
                        q->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
                    } else {
                        q->unk_16 = e->unk_04;
                    }
                    q->unk_1C.h = p->unk_14.h | p->unk_08.h;
                    q->unk_24.h = p->unk_14.h | p->unk_10.h;
                    if ((s16) p->unk_50 >= 0x1800) {
                        t = q->unk_24.b[0];
                        q->unk_24.b[0] = t + 0xFF;
                        q->unk_14.b[0] = t;
                    }
                    if ((s16) p->unk_58 >= 0x1800) {
                        t = q->unk_24.b[1];
                        q->unk_24.b[1] = t + 0xFF;
                        q->unk_1C.b[1] = t;
                    }
                    if ((s16) q->unk_08 > (s16) q->unk_20) {
                        t = q->unk_24.b[0];
                        q->unk_24.b[0] = t + 0xFF;
                        q->unk_14.b[0] = t;
                    }
                    if ((s16) q->unk_0A > (s16) q->unk_22) {
                        t = q->unk_24.b[1];
                        q->unk_24.b[1] = t + 0xFF;
                        q->unk_1C.b[1] = t;
                    }
                    f = e->unk_01;
                    s->unk_0C[3] = f;
                    if (p->unk_24 & 8) {
                        if (p->unk_24 & 4) {
                            s->unk_0C[3] = f | 2;
                        } else {
                            s->unk_0C[3] = f & 0xFD;
                        }
                    }
                    q->unk_04 = *(u32 *) s->unk_0C;
                    n = -((D_80081510 * (s8) ((u8 *) e)[3]) >> 14);
                    p->unk_CC = n;
                    if (n > 0) {
                        p->unk_CC = 0;
                    }
                    if ((u32) (p->unk_C0 + p->unk_CC) < 0x1D6) {
                        ((Unk453E0T *) q)->addr = ((Unk453E0T *) (p->unk_20 + p->unk_C0 * 4 + p->unk_CC * 4))->addr;
                        ((Unk453E0T *) (p->unk_20 + p->unk_C0 * 4 + p->unk_CC * 4))->addr = (u32) q;
                    }
                    q++;
                }
            } else {
                s32 (*f)(u8 *, UnkVec *, UnkSlot *, Unk44D24E *, Unk453E0P *);

                f = *(s32 (**)(u8 *, UnkVec *, UnkSlot *, Unk44D24E *,
                               Unk453E0P *)) &e->unk_08;
                if (f != NULL) {
                    s32 r;

                    p->unk_CC = p->unk_C0;
                    r = f(arg0, v, s, e, q);
                    if (r > 0) {
                        e = (Unk44D24E *) (r | 0x80000000);
                    } else {
                        q = (Unk453E0P *) r;
                    }
                    p->unk_C0 = p->unk_CC;
                }
            }
            if ((s8) e->unk_00 < 0) {
                break;
            }
            e++;
        }
        PopMatrix();
    }
    *(u32 *) (g->unk_00 + 0x8D0) = (u32) q;
}
