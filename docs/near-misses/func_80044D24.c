/* func_80044D24 (unk_14934) -- 372 of 379 words. Not compiled; see
 * docs/open-matching-problems.md. All seven missing words are in one place.
 *
 * FIRST: this is the only stub marked /* Handwritten function *\/ in its
 * listing, and that marker is only about the coprocessor instructions. GTE
 * inline asm works through this toolchain: `__asm__ volatile` survives cc1psx,
 * maspsx and gas, and `c2 0x480012` / `lwc2 $0,0(%0)` / `mfc2 $12,$9`
 * assemble to the target's exact bytes. (`c2 0x480012` is how gas spells
 * MVMVA 1,0,0,0,0.) The macros below belong in something like
 * include/gte_inline.h when this lands -- include/gte.inc is assembler-side
 * only.
 *
 * THE SEVEN WORDS. The X and Y blocks each convert a byte to signed in three
 * arms, and the target emits three separate single-use loads, none folded:
 *
 *     lbu $v0,0x1($s2) ; nop ; sll $v0,$v0,24 ; sra $v0,$v0,24
 *
 * (or `sra 23` in the doubled arm). The shift shapes ARE reachable -- a shared
 * load blocks combine from folding into `lb` -- but a shared load means ONE
 * lbu hoisted above the outer branch where the target has three. The probe
 * that reproduced all three arms' shift patterns exactly, including arm 2's
 * `sll 24` before the branch with `sra 24` in the delay slot, still had the
 * one hoisted load. That is the seven words.
 *
 * Ruled out for it: `(s8) e->b` per arm; `s32 t = (s8) e->b;` then `t * 2`;
 * the local at function scope assigned in each arm; the local at block scope
 * inside each arm; the negated form; and, on a u16 field, `(s8)(e->bc & 0xFF)`
 * (still `lb`) and `(s8)(e->bc >> 8)` (`lhu`/`sll 16`/`sra 24` -- the right
 * count in the right places but the wrong opcodes), and `u32 h = e->bc;` then
 * `(s8) h` (the shared-load shape again).
 *
 * UNTRIED, and the two best remaining ideas: a `volatile` lvalue, and a memory
 * STORE between the load and the cast in the same block. The store is the
 * mechanism now measured in docs/matching-levers.md section 7 and it is clean
 * -- it costs no extra register -- so it is the one to try first. Note the
 * two-sign-extension trick from that same section is what func_80045CC4 needs
 * and is NOT this: it gives one shared load, which is exactly what is wrong
 * here.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - `e = (Unk44D24E *) s->unk_08;` must sit between `s->unk_14 |= 0x8000;`
 *    and `p->unk_24 = s->unk_14;`. Moving it after the unk_24 store costs the
 *    entire prologue's register assignment (arg1 to $s4 and the scratchpad to
 *    $s1, where the target has $s3 and $s0). In place, the prologue is
 *    word-exact.
 *  - one scratchpad struct through one pointer local, but the matrix/vector
 *    arguments stay literal constants, giving the `addu`/`ori` pairs. Same
 *    mixing as func_8003D0F0 and func_800463EC.
 *  - D_80083160 reached both ways: a pointer local declared first, but the
 *    first read spelled as the plain symbol so its %lo rides the load and the
 *    addiu shares the same lui. The epilogue's write-back goes through the
 *    local. Identical to func_8003D5A4.
 *  - the clamp pairs are store-then-overwrite with interleaved loads, not
 *    if/else. Both matched first try.
 *  - arg0 is UNUSED -- $a0's incoming value is dead -- and arg2 is s16, which
 *    is what gives `arg2 * 4` its `sll 16` / `sra 14`.
 *  - the loop is one `Unk44D24E *e` with `e++`, not two pointers: `e` becomes
 *    the biv and combine_givs puts the one surviving giv at +1 because
 *    `e->unk_01` is the last `e->` access in the body. Same for `prim`, whose
 *    giv lands at +4 because `prim->unk_04` is written last.
 *
 * Writing the sign-arm expressions out four times instead of through a local
 * costs 80 words (509 against 429).
 *
 * structs.h, when this lands: UnkSlot's unk_0C is read here as a whole WORD
 * (stored into prim->unk_04) and the byte at +0x0F written on its own, where
 * func_800463EC needed the first four bytes as a u8[4] -- so that range wants
 * resolving, and the `((u8 *) &s->unk_0C)[3]` spelling below is a guess, not
 * measured against an alternative. Unk44D24SP should be MERGED into
 * Unk3DBD0SP/Unk463ECSP rather than added beside them -- all three describe
 * the 0x1F800000 scratchpad, and this function reads 0x00/0x02 which
 * func_8003DBD0 writes. Unk44D24P is the same POLY_FT4 as Unk463ECP and
 * func_8003D0F0's Unk3D0F0P; this one adds the measured detail that +0x04 is a
 * whole u32 and that +0x14/+0x1C/+0x24 are written both as halfwords and as
 * individual bytes. Prototypes still missing: TransMatrix, ScaleMatrix,
 * AddPrim.
 */

#define func_80044D24 func_80044D24_decl_clash
#define UnkSlot UnkSlot_orig
#include "common.h"
#undef func_80044D24
#undef UnkSlot

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

typedef union Unk44D24UV {
    u16 h;
    u8  b[2];
} Unk44D24UV;

typedef struct Unk44D24SP {
    s16        unk_00;
    u16        unk_02;
    u8         pad_04[0x4];
    Unk3DBD0W  unk_08;
    Unk3DBD0W  unk_0C;
    Unk3DBD0W  unk_10;
    Unk3DBD0W  unk_14;
    u8         pad_18[0x8];
    s32        unk_20;
    u16        unk_24;
    u8         pad_26[0xA];
    s32        unk_30;
    s32        unk_34;
    s32        unk_38;
    u8         pad_3C[0x4];
    s32        unk_40;
    s32        unk_44;
    s32        unk_48;
    u8         pad_4C[0x4];
    u16        unk_50;
    u8         pad_52[0x6];
    u16        unk_58;
    u8         pad_5A[0x16];
    s16        unk_70;
    s16        unk_72;
    s16        unk_74;
    u8         pad_76[0x2];
    s16        unk_78;
    s16        unk_7A;
    s16        unk_7C;
    u8         pad_7E[0x2];
    s16        unk_80;
    s16        unk_82;
    s16        unk_84;
    u8         pad_86[0x2];
    s16        unk_88;
    s16        unk_8A;
    s16        unk_8C;
} Unk44D24SP;

typedef struct Unk44D24E {
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
} Unk44D24E;

typedef struct Unk44D24P {
    u8         pad_00[0x3];
    u8         unk_03;
    u32        unk_04;
    s16        unk_08;
    s16        unk_0A;
    u16        unk_0C;
    u16        unk_0E;
    s16        unk_10;
    s16        unk_12;
    Unk44D24UV unk_14;
    u16        unk_16;
    s16        unk_18;
    s16        unk_1A;
    Unk44D24UV unk_1C;
    u16        unk_1E;
    s16        unk_20;
    s16        unk_22;
    Unk44D24UV unk_24;
    u16        unk_26;
} Unk44D24P;

void TransMatrix(void *, void *);
void RotMatrix(void *, void *);
void ScaleMatrix(void *, void *);
void AddPrim(void *, void *);

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" :: "r"(r0))
#define gte_rtv0()   __asm__ volatile ("nop\n\tnop\n\tc2 0x480012")
#define gte_stsv(r0) __asm__ volatile ("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\t" \
                                       "sh $12, 0(%0)\n\tsh $13, 2(%0)\n\tsh $14, 4(%0)" \
                                       :: "r"(r0) : "$12", "$13", "$14")

void func_80044D24(u8 *arg0, UnkSlot *s, s16 arg2) {
    Unk80083160 *g = &D_80083160;
    Unk44D24SP *p = (Unk44D24SP *) 0x1F800000;
    u8 *base = D_80083160.unk_00;
    Unk44D24P *prim = *(Unk44D24P **) (base + 0x8D0);
    Unk44D24E *e;
    s32 t;
    s32 x;
    s32 y;
    u8 c;

    p->unk_20 = (s32) (base + 0x70);
    p->unk_38 = 0x1000;
    p->unk_48 = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    PushMatrix();
    s->unk_14 |= 0x8000;
    e = (Unk44D24E *) s->unk_08;
    p->unk_24 = s->unk_14;
    p->unk_30 = s->unk_1C;
    p->unk_34 = s->unk_1E;
    p->unk_40 = p->unk_00 + s->unk_20;
    p->unk_44 = (s16) p->unk_02 + s->unk_22;
    TransMatrix((void *) 0x1F800050, (void *) 0x1F800040);
    RotMatrix(&s->unk_16, (void *) 0x1F800050);
    ScaleMatrix((void *) 0x1F800050, (void *) 0x1F800030);
    SetRotMatrix((void *) 0x1F800050);
    SetTransMatrix((void *) 0x1F800050);
    for (;;) {
        if ((e->unk_00 & 0x20) == 0) {
            x = e->unk_08;
            p->unk_08.w = x;
            y = e->unk_0A;
            p->unk_10.w = y;
            if (x + y >= 0x100) {
                p->unk_10.w = y - 1;
            }
            x = e->unk_09;
            p->unk_0C.w = x;
            y = e->unk_0B;
            p->unk_14.w = y;
            if (x + y >= 0x100) {
                p->unk_14.w = y - 1;
            }
            x = e->unk_02;
            if (((e->unk_00 ^ p->unk_24) & 1) != 0) {
                if (s->unk_14 & 0x400) {
                    t = (s8) x * 2;
                } else {
                    t = (s8) x;
                }
                t = -t;
                p->unk_80 = t;
                p->unk_70 = t;
                t = t - p->unk_10.h;
            } else {
                t = (s8) x;
                if (s->unk_14 & 0x400) {
                    t = t * 2;
                }
                p->unk_80 = t;
                p->unk_70 = t;
                t = t + p->unk_10.h;
            }
            p->unk_88 = t;
            p->unk_78 = t;
            y = e->unk_03;
            if (((e->unk_00 ^ p->unk_24) & 2) != 0) {
                if (s->unk_14 & 0x400) {
                    t = (s8) y * 2;
                } else {
                    t = (s8) y;
                }
                t = -t;
                p->unk_7A = t;
                p->unk_72 = t;
                t = t - p->unk_14.h;
            } else {
                t = (s8) y;
                if (s->unk_14 & 0x400) {
                    t = t * 2;
                }
                p->unk_7A = t;
                p->unk_72 = t;
                t = t + p->unk_14.h;
            }
            p->unk_8A = t;
            p->unk_82 = t;
            gte_ldv0(&p->unk_70);
            gte_rtv0();
            gte_stsv(&prim->unk_08);
            gte_ldv0(&p->unk_78);
            gte_rtv0();
            gte_stsv(&prim->unk_10);
            gte_ldv0(&p->unk_80);
            gte_rtv0();
            gte_stsv(&prim->unk_18);
            gte_ldv0(&p->unk_88);
            gte_rtv0();
            gte_stsv(&prim->unk_20);
            prim->unk_03 = 9;
            s->unk_14 &= 0x7FFF;
            p->unk_10.w = p->unk_10.w + p->unk_08.w;
            p->unk_14.w = (p->unk_14.w + p->unk_0C.w) << 8;
            p->unk_0C.w = p->unk_0C.w << 8;
            if (p->unk_24 & 0x100) {
                prim->unk_0E = s->unk_12;
            } else {
                prim->unk_0E = s->unk_12 + e->unk_06;
            }
            prim->unk_0C = p->unk_0C.h + p->unk_08.h;
            prim->unk_14.h = p->unk_0C.h + p->unk_10.h;
            if (s->unk_10 != 0) {
                prim->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
            } else {
                prim->unk_16 = e->unk_04;
            }
            prim->unk_1C.h = p->unk_14.h + p->unk_08.h;
            prim->unk_24.h = p->unk_14.h + p->unk_10.h;
            if ((s16) p->unk_50 >= 0x1800) {
                c = prim->unk_24.b[0];
                prim->unk_24.b[0] = c + 0xFF;
                prim->unk_14.b[0] = c;
            }
            if ((s16) p->unk_58 >= 0x1800) {
                c = prim->unk_24.b[1];
                prim->unk_24.b[1] = c + 0xFF;
                prim->unk_1C.b[1] = c;
            }
            if (prim->unk_20 < prim->unk_08) {
                c = prim->unk_24.b[0];
                prim->unk_24.b[0] = c + 0xFF;
                prim->unk_14.b[0] = c;
            }
            if (prim->unk_22 < prim->unk_0A) {
                c = prim->unk_24.b[1];
                prim->unk_24.b[1] = c + 0xFF;
                prim->unk_1C.b[1] = c;
            }
            ((u8 *) &s->unk_0C)[3] = e->unk_01;
            if (p->unk_24 & 8) {
                if (p->unk_24 & 4) {
                    ((u8 *) &s->unk_0C)[3] = e->unk_01 | 2;
                } else {
                    ((u8 *) &s->unk_0C)[3] = e->unk_01 & 0xFD;
                }
            }
            prim->unk_04 = s->unk_0C;
            AddPrim((void *) (p->unk_20 + arg2 * 4), prim);
            prim++;
        }
        if ((s8) e->unk_00 < 0) {
            break;
        }
        e++;
    }
    PopMatrix();
    *(Unk44D24P **) ((u8 *) g->unk_00 + 0x8D0) = prim;
}
