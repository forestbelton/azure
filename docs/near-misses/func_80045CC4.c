/* func_80045CC4 (unk_14934) -- 429 of 437 words, 364 DIFF. Not compiled; see
 * docs/open-matching-problems.md. Eight words short, and the structure -- all
 * basic blocks, all call sequences, all addressing -- is right.
 *
 * FOUR OF THE EIGHT are the `(s8)` byte conversion, and the mechanism is
 * known: load the byte ONCE above the `if` and sign-extend it in BOTH arms, so
 * combine cannot absorb the zero_extend into either sign_extend and each arm
 * gets its own `sll 24` / `sra 24` off one shared `lbu`. See
 * docs/matching-levers.md section 7. The source below still emits `lb`; the
 * fix is to hoist the byte read above the `if` at both sites (2 sites x 2 arms
 * x 1 word). One variable assigned in BOTH arms does not work -- each set dies
 * at its own use and folds locally -- but that spelling does reproduce the
 * whole arm structure, including the shared sh/sh tail and the `j` with the
 * subtraction in the delay slot, so build on it.
 *
 * Ruled out for the conversion: `-(s8) e->b2`; via s8, u8 and s32 locals; the
 * bare pointer `e[2]`; the hand-written `(x << 24) >> 24`; and signed 8-bit
 * bitfields, both `signed int b : 8` and `s8 b : 8`.
 *
 * THE OTHER FOUR are a global register shift with a specific hypothesis.
 * This source has the scratchpad in $s1, `v` in $s6 and 0xFF000000 hoisted
 * into $s7; the target has the scratchpad in $s0, `v` in $s7, and 0xFFFFFF
 * hoisted into $s6 with 0xFF000000 materialised inside the loop as a single
 * `lui`. The target's choice is the natural one -- 0xFFFFFF costs lui+ori so
 * loop.c hoists it, while 0xFF000000 is one lui so it does not -- so the
 * spelling of the two masks here is making gcc hoist the wrong one. Not
 * isolated, and there is no greg dump for this version.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - one scratchpad struct through one pointer local at 0x1F800000, but the
 *    libgte arguments as literal constants: `RotMatrix((void *) 0x1F800100,
 *    (void *) 0x1F8000D0)` gives the target's `addu $a0,$s0,$zero` / `ori`
 *    pairs, and twelve argument setups came out byte-exact first try. The
 *    RotAverage4 arguments are the opposite -- `&p->unk_70` etc., giving
 *    `addiu`. Same mixing as func_8003D0F0 and func_800463EC.
 *  - the loop tests before advancing: `for (;;) { body; if ((s8) e->unk_00 < 0)
 *    break; e++; }`. With `e++` first and `e[-1].unk_00` as the condition,
 *    combine_givs picks `e-0xC` as the representative and the second induction
 *    pointer comes out `addiu $s2,$s5,-12` where the target has `+0x8`. Worth
 *    about 40 differing words, and exactly the trap recorded for
 *    func_8003D0F0.
 *  - the `& 0x20` dispatch must be `if ((e->unk_00 & 0x20) == 0) { main }
 *    else { callback }` so the main body is the fall-through and the callback
 *    goes out of line. The other polarity inverts the branch: about 20 words.
 *  - unk_100, then unk_104, then unk_102 -- the same order func_8003DBD0
 *    needs.
 *  - the clip-test vertex fields are u16, giving `lhu`/`addiu`/`andi`/`sltiu`
 *    for `(u16) (q->unk_08 + 0x20) < 0x181`, while the SAME fields need
 *    `(s16)` casts in the later min tests to get `lh`. Declaring them s16 is
 *    wrong.
 *  - the OT address is recomputed per statement, not cached: two
 *    `lw 0xC0`/`lw 0x20` pairs, because the intervening `sw` invalidates cse.
 *  - unk_08/0C/10/14 are word/halfword unions, stored with `sw` and read back
 *    with `lhu`, like func_8003DBD0's Unk3DBD0W.
 *  - both clamp pairs are store-then-overwrite, not if/else.
 *
 * Writing the sign-arm expressions out four times instead of through a local
 * costs 80 words (509 against 429). A field-by-field OT update without the
 * second statement is 10 words short.
 *
 * structs.h, when this lands: Unk3DBD0SP needs extending with unk_08/0C/10/14
 * as Unk3DBD0W unions, `u8 *unk_20`, `u16 unk_50`, `u16 unk_58`, the
 * unk_78..unk_8C SVECTORs, `s32 unk_90`, `s32 unk_C0` and `u8 unk_11C[0x14]`,
 * all inside existing pads -- and it, Unk463ECSP and func_80044D24's
 * Unk44D24SP all describe the same region and want merging. UnkSlot's unk_0C
 * range wants resolving the same way func_80044D24 needs. New types:
 * Unk45CC4E (0xC bytes: four bytes, two halfwords, then a word that is a
 * function pointer in one arm and four bytes in the other) and Unk45CC4P
 * (another POLY_FT4, with unions of `u16 h` / `u8 b[2]` at +0x0C/14/1C/24).
 * Prototypes: CompMatrix (now added by func_800463EC) and
 * `s32 RotAverage4(void *, void *, void *, void *, u32 *, u32 *, u32 *, u32 *,
 * s32 *, s32 *)` -- note the u32* argument types differ from the s32* form
 * func_800463EC landed, so one of the two is imprecise.
 */

#define Unk3DBD0SP Unk3DBD0SP_orig
#define UnkSlot UnkSlot_orig
#define func_80045CC4 func_80045CC4_decl
#include "common.h"
#undef Unk3DBD0SP
#undef UnkSlot
#undef func_80045CC4

typedef struct UnkSlot {
    UnkElem *unk_00;
    s8       unk_04;
    u8       unk_05;
    s16      unk_06;
    u32      unk_08;
    u32      unk_0C;
    u16      unk_10;
    u16      unk_12;
    u16      unk_14;
    u16      unk_16;
    u16      unk_18;
    u16      unk_1A;
    u16      unk_1C;
    u16      unk_1E;
    u16      unk_20;
    u16      unk_22;
    u8       pad_24[0x4];
    u32      unk_28;
} UnkSlot;

typedef struct Unk3DBD0SP {
    s16         unk_00;
    s16         unk_02;
    u8          pad_04[0x4];
    Unk3DBD0W   unk_08;
    Unk3DBD0W   unk_0C;
    Unk3DBD0W   unk_10;
    Unk3DBD0W   unk_14;
    u8          pad_18[0x8];
    u8         *unk_20;
    u16         unk_24;
    u8          pad_26[0xA];
    Unk3DBD0W   unk_30;
    Unk3DBD0W   unk_34;
    s32         unk_38;
    u8          pad_3C[0x14];
    u16         unk_50;
    u8          pad_52[0x6];
    u16         unk_58;
    u8          pad_5A[0x16];
    s16         unk_70;
    s16         unk_72;
    s16         unk_74;
    u8          pad_76[0x2];
    s16         unk_78;
    s16         unk_7A;
    s16         unk_7C;
    u8          pad_7E[0x2];
    s16         unk_80;
    s16         unk_82;
    s16         unk_84;
    u8          pad_86[0x2];
    s16         unk_88;
    s16         unk_8A;
    s16         unk_8C;
    u8          pad_8E[0x2];
    s32         unk_90;
    s32         unk_94;
    u8          pad_98[0x28];
    s32         unk_C0;
    u8          pad_C4[0xC];
    u8          unk_D0[0x14];
    s32         unk_E4;
    s32         unk_E8;
    s32         unk_EC;
    u8          pad_F0[0x10];
    s16         unk_100;
    s16         unk_102;
    s16         unk_104;
    u8          pad_106[0x2];
    u16         unk_108;
    u16         unk_10A;
    u8          pad_10C[0x10];
    u8          unk_11C[0x14];
} Unk3DBD0SP;

/* The 0xC-byte element func_80045CC4 walks. */
typedef struct Unk45CC4E {
    u8  unk_00;
    u8  unk_01;
    u8  unk_02;
    u8  unk_03;
    u16 unk_04;
    u16 unk_06;
    u8  unk_08;
    u8  unk_09;
    u8  unk_0A;
    u8  unk_0B;
} Unk45CC4E;

/* The 0x28-byte primitive it emits. */
typedef union Unk45CC4UV {
    u16 h;
    u8  b[2];
} Unk45CC4UV;

typedef struct Unk45CC4P {
    u32         unk_00;
    u32         unk_04;
    u16         unk_08;
    u16         unk_0A;
    Unk45CC4UV  unk_0C;
    u16         unk_0E;
    u16         unk_10;
    u16         unk_12;
    Unk45CC4UV  unk_14;
    u16         unk_16;
    u16         unk_18;
    u16         unk_1A;
    Unk45CC4UV  unk_1C;
    u16         unk_1E;
    u16         unk_20;
    u16         unk_22;
    Unk45CC4UV  unk_24;
    u16         unk_26;
} Unk45CC4P;

void CompMatrix(void *, void *, void *);
s32  RotAverage4(void *, void *, void *, void *, u32 *, u32 *, u32 *, u32 *,
                 s32 *, s32 *);

void func_80045CC4(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3) {
    Unk3DBD0SP *p = (Unk3DBD0SP *) 0x1F800000;
    Unk45CC4E *e;
    Unk45CC4P *q;

    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_E4 = *(s16 *) ((u8 *) v + 2);
    p->unk_E8 = *(s16 *) ((u8 *) v + 6);
    p->unk_EC = *(s16 *) ((u8 *) v + 0xA);
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    q = *(Unk45CC4P **) (D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    PushMatrix();
    p->unk_108 = s->unk_20;
    p->unk_10A = s->unk_22;
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38 = 0x2000;
    p->unk_100 = s->unk_16;
    p->unk_104 = s->unk_1A;
    p->unk_102 = s->unk_18;
    RotMatrix((void *) 0x1F800100, (void *) 0x1F8000D0);
    ReadRotMatrix((void *) 0x1F80011C);
    CompMatrix((void *) 0x1F80011C, (void *) 0x1F8000D0, (void *) 0x1F800050);
    ScaleMatrix((void *) 0x1F800050, (void *) 0x1F800030);
    SetTransMatrix((void *) 0x1F800050);
    SetRotMatrix((void *) 0x1F800050);
    e = (Unk45CC4E *) s->unk_08;
    p->unk_24 = s->unk_14;
    for (;;) {
        if ((e->unk_00 & 0x20) == 0) {
            s32 u;
            s32 w;
            s32 a;
            s32 otz;

            u = e->unk_08;
            p->unk_08.w = u;
            w = e->unk_0A;
            p->unk_10.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0) {
                p->unk_10.w = w - 1;
            }
            u = e->unk_09;
            p->unk_0C.w = u;
            w = e->unk_0B;
            p->unk_14.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0) {
                p->unk_14.w = w - 1;
            }
            if ((e->unk_00 ^ p->unk_24) & 1) {
                a = -(s8) ((u8 *) e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a - p->unk_10.h;
            } else {
                a = (s8) ((u8 *) e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a + p->unk_10.h;
            }
            p->unk_88 = a;
            p->unk_78 = a;
            if ((e->unk_00 ^ p->unk_24) & 2) {
                a = -(s8) ((u8 *) e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a - p->unk_14.h;
            } else {
                a = (s8) ((u8 *) e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a + p->unk_14.h;
            }
            p->unk_8A = a;
            p->unk_82 = a;
            otz = RotAverage4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                              (u32 *) &q->unk_08, (u32 *) &q->unk_10,
                              (u32 *) &q->unk_18, (u32 *) &q->unk_20,
                              &p->unk_90, &p->unk_94) - arg3 - 8;
            p->unk_C0 = otz;
            if ((u32) otz < 0x1E0) {
                s32 c0;
                s32 c1;
                s32 c2;
                s32 c3;
                s32 t;

                c0 = (u16) (q->unk_08 + 0x20) < 0x181 &&
                     (u16) (q->unk_0A + 0x20) < 0x121;
                c1 = (u16) (q->unk_10 + 0x20) < 0x181 &&
                     (u16) (q->unk_12 + 0x20) < 0x121;
                c2 = (u16) (q->unk_18 + 0x20) < 0x181 &&
                     (u16) (q->unk_1A + 0x20) < 0x121;
                c3 = (u16) (q->unk_20 + 0x20) < 0x181 &&
                     (u16) (q->unk_22 + 0x20) < 0x121;
                if ((c0 | c1 | c2 | c3) != 0) {
                    ((u8 *) &q->unk_00)[3] = 9;
                    s->unk_14 &= 0x7FFF;
                    p->unk_10.w = p->unk_10.w + p->unk_08.w;
                    p->unk_14.w = p->unk_14.w + p->unk_0C.w;
                    p->unk_0C.w = p->unk_0C.w << 8;
                    p->unk_14.w = p->unk_14.w << 8;
                    if (p->unk_24 & 0x100) {
                        q->unk_0E = s->unk_12;
                    } else {
                        q->unk_0E = s->unk_12 + e->unk_06;
                    }
                    q->unk_0C.h = p->unk_0C.h + p->unk_08.h;
                    q->unk_14.h = p->unk_0C.h + p->unk_10.h;
                    if (s->unk_10 != 0) {
                        q->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
                    } else {
                        q->unk_16 = e->unk_04;
                    }
                    q->unk_1C.h = p->unk_14.h + p->unk_08.h;
                    q->unk_24.h = p->unk_14.h + p->unk_10.h;
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
                    if ((s16) q->unk_20 < (s16) q->unk_08) {
                        q->unk_14.b[0] = q->unk_14.b[0] - 1;
                        q->unk_24.b[0] = q->unk_24.b[0] - 1;
                    }
                    if ((s16) q->unk_22 < (s16) q->unk_0A) {
                        q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                        q->unk_24.b[1] = q->unk_24.b[1] - 1;
                    }
                    t = e->unk_01;
                    ((u8 *) &s->unk_0C)[3] = t;
                    if (p->unk_24 & 8) {
                        if (p->unk_24 & 4) {
                            ((u8 *) &s->unk_0C)[3] = t | 2;
                        } else {
                            ((u8 *) &s->unk_0C)[3] = t & 0xFD;
                        }
                    }
                    q->unk_04 = s->unk_0C;
                    q->unk_00 = (q->unk_00 & 0xFF000000) |
                        (*(u32 *) (p->unk_20 + p->unk_C0 * 4) & 0xFFFFFF);
                    *(u32 *) (p->unk_20 + p->unk_C0 * 4) =
                        (*(u32 *) (p->unk_20 + p->unk_C0 * 4) & 0xFF000000) |
                        ((u32) q & 0xFFFFFF);
                    q++;
                }
            }
        } else {
            void (*f)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *);

            f = *(void (**)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *)) &e->unk_08;
            if (f != NULL) {
                f(arg0, v, s, e);
            }
        }
        if ((s8) e->unk_00 < 0) {
            break;
        }
        e++;
    }
    PopMatrix();
    *(u32 *) (D_80083160.unk_00 + 0x8D0) = (u32) q;
}
