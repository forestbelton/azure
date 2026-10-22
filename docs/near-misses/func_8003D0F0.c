/* func_8003D0F0 (unk_102A8) -- 212 of 222 words, 200 DIFF. Not compiled; see
 * docs/open-matching-problems.md.
 *
 * Per-frame billboard emitter. It sets up a translate+rotate matrix in the
 * scratchpad, then walks the D_800777AC table -- 0xC-byte entries, terminated
 * when the sign bit of unk_00 is set -- building one 0x28-byte textured quad
 * per entry into the OT buffer at D_80083160.unk_00 + 0x8D0.
 *
 * TWO REAL GAPS, everything else cascades from them.
 *
 * (a) 8 of the 10 missing words: the four `(s8)` byte conversions. The target
 * has lbu + sll 24 + sra 24 in each of the four flag arms; every spelling
 * tried collapses to a single `lb`. Tried and all folded: `-(s8) e->unk_02`
 * directly; via an s32 local in two statements; with an `& 0xFF` in between;
 * with explicit `a = e->unk_02 << 24; a = -(a >> 24);`. combine reduces
 * (sign_extend (subreg (zero_extend (mem:QI)))) to (sign_extend (mem:QI)) in
 * all of them. For the target's form the zero-extended value would need a
 * second use or an intervening memory STORE, and the target has neither --
 * between its lbu and sll sits only an independent lhu. This is the byte-load
 * counterpart of the lh fold recorded for func_8004425C.
 *
 * (b) 2 words: the post-call block's copy. The target is addu/addu/sw 0x10/
 * `addu $v0,$a1,$zero`/sll/sll/sw 0x14/sw 0xC -- it copies the 0x0C value into
 * another register before shifting it in place, i.e. the (set p2 p1) +
 * (set p2 (ashift p2 8)) pattern that combine could not fold back. Reading all
 * four words into locals before the three stores (in the target's load order
 * 0x10, 0x8, 0x14, 0xC) gets the load order and the two addus right but still
 * emits `sll $v0,$a1,8` with no copy.
 *
 * The rest of the 200 is a global $s0/$s1 swap: this source puts the
 * scratchpad base in $s1 and the p+4 giv in $s0, the target the other way
 * round. One allocation-priority decision, expected to fall out once (a) is
 * fixed, since (a) changes the ref counts in four basic blocks.
 *
 * ESTABLISHED AND LOAD-BEARING, each measured by reverting it:
 *  - `u8 *sp = (u8 *) 0x1F800000;` as a pointer local, with every scratchpad
 *    access as `*(T *)(sp + off)` -- 262 words otherwise. As literal constant
 *    addresses gcc rebuilds `lui $at, 0x1F80` for every access and the four
 *    scalars at 0x08..0x14 get their own callee-saved registers. The single
 *    `lui $s0, 0x1F80` with displacements only comes from the pointer local
 *    (its low half is zero, so the ori folds away).
 *  - BUT the matrix/vector arguments stay literal constants:
 *    `TransMatrix((void *) 0x1F800050, (void *) 0x1F800040)`. That is what
 *    gives the target's `addu $a0,$s0,$zero` / `ori $a0,$a0,0x50` pairs -- gcc
 *    materialises the constant as high part + ori, cse replaces the high part
 *    with a copy from sp's register, and the ori stays. Through sp each would
 *    be a single addiu. Same deliberate mixing as func_8003D5A4 and
 *    func_80044724.
 *  - `s32 *sz = (s32 *) 0x1F800094;` as its own local -- inline, the constant
 *    is materialised four times inside the loop because cse's extended basic
 *    block does not reach into the loop body. The target really does have TWO
 *    `lui 0x1F80` in the prologue, one for sp and one for sz; they are not
 *    shared.
 *  - `Unk80083160 *s = &D_80083160;` declared, but the FIRST read spelled
 *    directly as `D_80083160.unk_00`. Without the local gcc rebuilds `lui %hi`
 *    at the end of the function; with it, the direct read gets its %lo riding
 *    on the load and the addiu into $s6 shares the same lui.
 *  - the four `vz = 0` stores are four separate statements in descending order,
 *    not a chain. The chain gets the order right but makes the three inner
 *    stores use plain constant addresses instead of $s0 displacements.
 *  - store-then-overwrite, not if/else, for the two clipped dimensions. The
 *    if/else form costs a `j` per arm and re-reads the byte.
 *  - those two dimensions go through locals read in interleaved order;
 *    declaring all four with initialisers at the top of the block hoists all
 *    four lbus together where the target interleaves load/store/load.
 *  - the flag arms reuse ONE variable for both the corner value and the offset
 *    sum. A separate variable lets gcc schedule the subu before the two sh
 *    stores; reusing it creates the anti-dependence that pins the subtraction
 *    into the j's delay slot. The corner value must be s32, not s16.
 *  - `*(u16 *) ((u8 *) p + 0xC)` as bare pointer dereferences for the four UV
 *    halfword stores, not struct fields: as struct fields they are
 *    MEM_IN_STRUCT_P while the scratchpad reads are not, so
 *    true_dependence's differing-flags exception lets cse keep the scratchpad
 *    values across them and the four sums share two loads, where the target
 *    re-reads for every statement. Worth 3 words plus the block's ordering.
 *  - the loop tests before advancing e: `p++; if ((s8) e->unk_00 < 0) break;
 *    e++;`. With e++ first gcc computes the e+4 giv as `s4 - 12`.
 *  - p->unk_1C's companion byte is at +0x1D, so `((u8 *) &p->unk_1C)[1] = t;`.
 *
 * structs.h would want Unk3D0F0V (an 8-byte SVECTOR) and Unk3D0F0P (the
 * 0x28-byte POLY_FT4), plus TransMatrix/RotMatrix/RotTransSV/DrawPrim
 * prototypes. Unk800777AC's field types are all already right, and
 * `extern Unk800777AC D_800777AC;` is the spelling that gives the lui/addiu
 * pair kept in $s4.
 */

#include "common.h"

typedef struct Unk3D0F0V {
    s16 vx;
    s16 vy;
    s16 vz;
    s16 pad_06;
} Unk3D0F0V;

typedef struct Unk3D0F0P {
    u8  pad_00[0x3];
    u8  unk_03;
    u8  unk_04;
    u8  unk_05;
    u8  unk_06;
    u8  unk_07;
    s16 unk_08;
    s16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    s16 unk_10;
    s16 unk_12;
    u8  unk_14;
    u8  unk_15;
    u16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    u16 unk_1C;
    u16 unk_1E;
    s16 unk_20;
    s16 unk_22;
    u8  unk_24;
    u8  unk_25;
    u16 unk_26;
} Unk3D0F0P;

void TransMatrix(void *, void *);
void RotMatrix(void *, void *);
void RotTransSV(void *, void *, s32 *);
void DrawPrim(void *);

void zztest(void) {
    s32 *sz = (s32 *) 0x1F800094;
    Unk80083160 *s = &D_80083160;
    Unk3D0F0P *p = *(Unk3D0F0P **) (D_80083160.unk_00 + 0x8D0);
    u8 *sp = (u8 *) 0x1F800000;
    Unk800777AC *e = &D_800777AC;

    *(s32 *) (sp + 0x48) = 0;
    ((Unk3D0F0V *) (sp + 0x70))[3].vz = 0;
    ((Unk3D0F0V *) (sp + 0x70))[2].vz = 0;
    ((Unk3D0F0V *) (sp + 0x70))[1].vz = 0;
    ((Unk3D0F0V *) (sp + 0x70))[0].vz = 0;
    PushMatrix();
    *(s32 *) (sp + 0x40) = 0x100;
    *(s32 *) (sp + 0x44) = 0xB0;
    ((Unk3D0F0V *) (sp + 0x28))->vx = 0;
    ((Unk3D0F0V *) (sp + 0x28))->vy = 0;
    ((Unk3D0F0V *) (sp + 0x28))->vz = 0;
    TransMatrix((void *) 0x1F800050, (void *) 0x1F800040);
    RotMatrix((void *) 0x1F800028, (void *) 0x1F800050);
    SetRotMatrix((void *) 0x1F800050);
    SetTransMatrix((void *) 0x1F800050);
    for (;;) {
        s32 a;
        s32 b;
        s32 u;
        s32 w;
        s32 v;
        s32 h;

        u = e->unk_08;
        *(s32 *) (sp + 0x8) = u;
        w = e->unk_0A;
        *(s32 *) (sp + 0x10) = w;
        if (u + w >= 0x100) {
            *(s32 *) (sp + 0x10) = w - 1;
        }
        u = e->unk_09;
        *(s32 *) (sp + 0xC) = u;
        w = e->unk_0B;
        *(s32 *) (sp + 0x14) = w;
        if (u + w >= 0x100) {
            *(s32 *) (sp + 0x14) = w - 1;
        }
        if (e->unk_00 & 1) {
            a = e->unk_02;
            a = -(s8) a;
            ((Unk3D0F0V *) (sp + 0x70))[2].vx = a;
            ((Unk3D0F0V *) (sp + 0x70))[0].vx = a;
            a = a - *(u16 *) (sp + 0x10);
        } else {
            a = e->unk_02;
            a = (s8) a;
            ((Unk3D0F0V *) (sp + 0x70))[2].vx = a;
            ((Unk3D0F0V *) (sp + 0x70))[0].vx = a;
            a = a + *(u16 *) (sp + 0x10);
        }
        ((Unk3D0F0V *) (sp + 0x70))[3].vx = a;
        ((Unk3D0F0V *) (sp + 0x70))[1].vx = a;
        if (e->unk_00 & 2) {
            b = e->unk_03;
            b = -(s8) b;
            ((Unk3D0F0V *) (sp + 0x70))[1].vy = b;
            ((Unk3D0F0V *) (sp + 0x70))[0].vy = b;
            b = b - *(u16 *) (sp + 0x14);
        } else {
            b = e->unk_03;
            b = (s8) b;
            ((Unk3D0F0V *) (sp + 0x70))[1].vy = b;
            ((Unk3D0F0V *) (sp + 0x70))[0].vy = b;
            b = b + *(u16 *) (sp + 0x14);
        }
        ((Unk3D0F0V *) (sp + 0x70))[3].vy = b;
        ((Unk3D0F0V *) (sp + 0x70))[2].vy = b;
        RotTransSV((void *) (sp + 0x70), &p->unk_08, sz);
        RotTransSV((void *) (sp + 0x78), &p->unk_10, sz);
        RotTransSV((void *) (sp + 0x80), &p->unk_18, sz);
        RotTransSV((void *) (sp + 0x88), &p->unk_20, sz);
        w = *(s32 *) (sp + 0x10);
        u = *(s32 *) (sp + 0x8);
        h = *(s32 *) (sp + 0x14);
        v = *(s32 *) (sp + 0xC);
        *(s32 *) (sp + 0x10) = w + u;
        *(s32 *) (sp + 0x14) = (h + v) << 8;
        *(s32 *) (sp + 0xC) = v << 8;
        *(u16 *) ((u8 *) p + 0xE) = e->unk_06;
        *(u16 *) ((u8 *) p + 0xC) = *(u16 *) (sp + 0xC) + *(u16 *) (sp + 0x8);
        *(u16 *) ((u8 *) p + 0x14) = *(u16 *) (sp + 0xC) + *(u16 *) (sp + 0x10);
        *(u16 *) ((u8 *) p + 0x16) = e->unk_04;
        *(u16 *) ((u8 *) p + 0x1C) = *(u16 *) (sp + 0x14) + *(u16 *) (sp + 0x8);
        *(u16 *) ((u8 *) p + 0x24) = *(u16 *) (sp + 0x14) + *(u16 *) (sp + 0x10);
        if (p->unk_20 < p->unk_08) {
            u8 t = p->unk_24;

            p->unk_24 = t - 1;
            p->unk_14 = t;
        }
        if (p->unk_22 < p->unk_0A) {
            u8 t = p->unk_25;

            p->unk_25 = t - 1;
            ((u8 *) &p->unk_1C)[1] = t;
        }
        p->unk_03 = 9;
        p->unk_07 = 0x2C;
        p->unk_06 = 0x80;
        p->unk_05 = 0x80;
        p->unk_04 = 0x80;
        DrawPrim(p);
        p++;
        if ((s8) e->unk_00 < 0) {
            break;
        }
        e++;
    }
    PopMatrix();
    *(Unk3D0F0P **) (s->unk_00 + 0x8D0) = p;
}
