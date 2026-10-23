/* func_800453E0 (gprel_18B40) -- 533 of 533 words, 10 DIFF. NOT compiled.
 *
 * The fourth member of the "emit one slot's primitives" family, and its
 * closest sibling is func_80045CC4, not func_80044D24. It projects the slot
 * origin with RotTransPers, turns the averaged z into an OT index, bails out
 * if that index is out of range, builds the matrix in the 0x1F800000
 * scratchpad, then walks the slot's 0xC-byte element list: per element either
 * call the element's own callback (bit 0x20) or clip/project it with
 * RotTransPers4, clamp the UVs and link one 0x28-byte primitive into the OT.
 *
 * Two things are new against the three siblings:
 *  - the OT index is `p->unk_C0 + p->unk_CC`, where unk_CC is a per-element
 *    depth bias `-((D_80081510 * (s8) e->unk_03) >> 14)` clamped to <= 0, so
 *    the OT address carries TWO scaled terms. That is the only
 *    %gp_rel(D_80081510) reference in the function and the reason the unit is
 *    carved.
 *  - the callback returns s32 and takes FIVE arguments (the fifth on the stack
 *    at 0x10($sp)), and its result is dispatched on sign: a positive return is
 *    a physical address to continue the element walk at, in KSEG0; a negative
 *    one is already a KSEG0 primitive pointer. p->unk_CC is used to save and
 *    restore p->unk_C0 across that call.
 *
 * WHAT IS LEFT is 10 words, all in the FIRST OT statement, and the pass is
 * local-alloc. One clean register exchange ($v1/$a0 and $a1/$s4), with every
 * instruction, immediate, displacement, delay slot and block boundary
 * matching, and the SECOND OT statement -- the same expression mirrored --
 * matching byte for byte.
 *
 * This is func_80045CC4's documented "mask tension", unresolved, and here it
 * costs 10 words rather than 4. The target wants `q->unk_00 & 0xFF000000` as
 * the `or`'s operand 0 AND `0xFFFFFF` hoisted by loop.c, and the two are in
 * direct conflict because loop.c hoists whichever invariant is SET FIRST in
 * the body. The swapped order used below does hoist 0xFFFFFF (`lui $s4` in
 * PushMatrix's delay slot, callee-saved) and leaves 0xFF000000 inline as a
 * single `lui $a1`, which is the target; the natural order loses the hoist
 * outright, 532 words and 475 DIFF.
 *
 * From the dumps: the pseudos are p->unk_CC (a global allocno, spanning the
 * guard block and the OT block), unk_CC * 4, p->unk_20, the address, and *ot
 * -- the last four block-local, so block_alloc assigns them by live length
 * before global_alloc runs. The only thing that moves those lengths is which
 * of the two masked loads is emitted first, which follows the `|`'s operand
 * order; ours makes the address pseudo die one insn earlier than the target's,
 * which flips p->unk_20 and unk_CC * 4, and p->unk_CC then follows through
 * global_alloc. Promoting p->unk_20 to a global allocno (section 3's escape
 * from local-alloc) would put it in a callee-saved register, which the target
 * does not have -- the same door that was closed on func_80041344.
 *
 * RULED OUT for those 10 words, do not retry: the natural operand order (532 /
 * 475); the two ANDs' operands reversed (533 / 10); q->unk_00 pre-loaded into
 * a local (533 / 10); `q->unk_00 & 0xFF000000` pre-masked into a local (532 /
 * 477); `*ot & 0xFFFFFF` hoisted one statement earlier (533 / 10, the same
 * non-result func_80045CC4 recorded); *ot pre-loaded and masked in place (533 /
 * 10); giving 0xFFFFFF an earlier first set in the body so the natural order
 * could be used -- `ov = (u32) q & 0xFFFFFF;` lifted above statement 1 and
 * above statement 2, with statement 1 natural or swapped (532 / 475, 532 / 469,
 * 532 / 475; the extra local costs the word and the hoist wherever it goes, and
 * this was the only genuinely new idea for the tension); four per-site
 * MEM_IN_STRUCT_P spellings in statement 1 (533 / 10 each); three address
 * re-associations (533 / 10, 533 / 11, 533 / 13) and a fourth that loses two
 * words (531 / 74); three guard variants (533 / 11, 533 / 15, and 535 / 81 for
 * the clamp as if/else rather than store-then-overwrite); and
 * `search.py --g8 --only=width,cast`, 57 candidates over 3 families, 58
 * variants, none better.
 *
 * NEW LEVERS MEASURED HERE, all now in docs/matching-levers.md:
 *  - `s32 w = p->unk_C0 - 0xA;` as its own statement, worth 1 word. As one
 *    expression, tree-level `fold` reassociates the constant onto the other
 *    operand and emits `addiu $s0,$s0,0xA` / `subu` where the target has
 *    `addiu $v0,$v0,-0xA` / `subu`.
 *  - `s32 h = s->unk_18 - 0x100;` as its own statement, worth 1 word, and it
 *    is section 7's byte-decrement entry generalised to HImode with the costs
 *    INVERTED, because 0xFF is a valid addiu immediate and 0xFF00 is not.
 *  - `p->unk_108 = p->unk_E4 = s->unk_20;` chained, worth 1 word each: two
 *    statements give two `lhu 0x20($s3)` because the intervening store kills
 *    cse's memory entry even though the bases differ.
 *  - the scale-vector store order 30/34/38, worth 7 words. All six
 *    permutations swept (10, 14, 17, 17, 18, 18 DIFF). Note the EMITTED order
 *    is 30, 38, 34 -- reading the source order off the listing would have cost
 *    those 7 words, and the sweep was faster than reasoning about it.
 *  - a compiler-split symbol used twice needs no pointer local: D_8006CD30 is
 *    reached by plain symbol at two sites separated by three calls and cse
 *    keeps the single lui/%lo pair in $s5 by itself.
 *
 * DECLARATIONS NEEDED WHEN THIS LANDS. The two `#define` shims below are
 * scratch-only scaffolding for exactly these:
 *  1. **D_8006CD30 must be retyped from `extern u32` to a 0x20-byte object**
 *     (include/structs.h). This is load-bearing twice: at -G8 a 4-byte
 *     undefined symbol becomes $gp-relative, which is WRONG here -- and this
 *     is a real correction to this unit's own -G8 audit comment, which claims
 *     D_80083160 is the only absolutely-reached symbol. `tools/decomp/gaudit.py`
 *     now catches it and reports exactly this one problem across all eleven
 *     carved units. Nothing else in csrc/ or include/ references the symbol, so
 *     the retype is free.
 *  2. RotTransPers must return s32, not void. Its only other call site,
 *     csrc/unk_1811C.c, discards the result, so the change is codegen-neutral
 *     there -- but re-verify that function.
 *  3. New prototype: `s32 RotTransPers4(void *, void *, void *, void *, void *,
 *     void *, void *, void *, void *, void *);` -- four input SVECTORs, four
 *     sxy outputs, p and flag.
 *  4. The scratchpad struct below. It agrees with Unk3DBD0SP / Unk44D24SP /
 *     Unk463ECSP where they overlap but needs `Unk3DBD0W unk_38` (the existing
 *     `s32 unk_38` would not give the `lhu` read-back), u16 unk_B8/unk_BA,
 *     u16 unk_F0..unk_FE, s32 unk_CC and u16 unk_13C. unk_20 is
 *     func_80045CC4's byte-scaled `u8 *`, not Unk44D24SP's s32.
 *  5. The primitive struct below. Unk44D24P does NOT work: its vertices are
 *     s16 and the clip tests need `lhu` while the min tests need `(s16)` casts
 *     for `lh`. This is Unk45CC4P with `u16 unk_0C` in place of the union.
 *  6. No new element or slot type -- the existing UnkSlot and Unk44D24E are
 *     byte-for-byte right, measured.
 *
 * nmcheck: 533 of 533 words, 10 DIFF
 */
#define D_8006CD30 D_8006CD30_orig
#define RotTransPers RotTransPers_orig
#include "common.h"
#undef D_8006CD30
#undef RotTransPers


/* 0x20-byte MATRIX global. Must be declared larger than -G8 so cc1psx
   compiler-splits its address; as the existing `extern u32 D_8006CD30` it is a
   small symbol and gas would make it $gp-relative in this -G8 unit. */
typedef struct Unk6CD30 {
    u8  pad_00[0x1C];
    s32 unk_1C;
} Unk6CD30;
extern Unk6CD30 D_8006CD30;

s32 RotTransPers(void *, void *, void *, void *);
s32 RotTransPers4(void *, void *, void *, void *, void *, void *, void *,
                  void *, void *, void *);

typedef struct Unk453E0SP {
    s16       unk_00;
    s16       unk_02;
    s16       unk_04;
    u8        pad_06[0x2];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8        pad_18[0x8];
    u8       *unk_20;
    u16       unk_24;
    u8        pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8        pad_3C[0x14];
    u16       unk_50;
    u8        pad_52[0x6];
    u16       unk_58;
    u8        pad_5A[0x16];
    s16       unk_70;
    s16       unk_72;
    s16       unk_74;
    u8        pad_76[0x2];
    s16       unk_78;
    s16       unk_7A;
    s16       unk_7C;
    u8        pad_7E[0x2];
    s16       unk_80;
    s16       unk_82;
    s16       unk_84;
    u8        pad_86[0x2];
    s16       unk_88;
    s16       unk_8A;
    s16       unk_8C;
    u8        pad_8E[0x2];
    s32       unk_90;
    s32       unk_94;
    u8        pad_98[0x20];
    u16       unk_B8;
    u16       unk_BA;
    u8        pad_BC[0x4];
    s32       unk_C0;
    u8        pad_C4[0x8];
    s32       unk_CC;
    u8        pad_D0[0x14];
    s32       unk_E4;
    s32       unk_E8;
    s32       unk_EC;
    u16       unk_F0;
    u16       unk_F2;
    u16       unk_F4;
    u16       unk_F6;
    u16       unk_F8;
    u16       unk_FA;
    u16       unk_FC;
    u16       unk_FE;
    s16       unk_100;
    s16       unk_102;
    s16       unk_104;
    u8        pad_106[0x2];
    u16       unk_108;
    u16       unk_10A;
    u8        pad_10C[0x30];
    u16       unk_13C;
} Unk453E0SP;


typedef union Unk453E0UV {
    u16 h;
    u8  b[2];
} Unk453E0UV;

typedef struct Unk453E0P {
    u32        unk_00;
    u32        unk_04;
    u16        unk_08;
    u16        unk_0A;
    u16        unk_0C;
    u16        unk_0E;
    u16        unk_10;
    u16        unk_12;
    Unk453E0UV unk_14;
    u16        unk_16;
    u16        unk_18;
    u16        unk_1A;
    Unk453E0UV unk_1C;
    u16        unk_1E;
    u16        unk_20;
    u16        unk_22;
    Unk453E0UV unk_24;
    u16        unk_26;
} Unk453E0P;

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
                    u8 t;
                    u8 f;

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
                        q->unk_00 =
                            (*(u32 *) (p->unk_20 + p->unk_C0 * 4 +
                                       p->unk_CC * 4) & 0xFFFFFF) |
                            (q->unk_00 & 0xFF000000);
                        *(u32 *) (p->unk_20 + p->unk_C0 * 4 + p->unk_CC * 4) =
                            (*(u32 *) (p->unk_20 + p->unk_C0 * 4 +
                                       p->unk_CC * 4) & 0xFF000000) |
                            ((u32) q & 0xFFFFFF);
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
