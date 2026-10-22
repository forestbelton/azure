#include "common.h"

void func_8003DBD0(UnkSlot *, u8 *, s16 *);

/* Transforms a sprite's position, rotation and scale into a rotation matrix in
 * the PSX scratchpad, projects a hotspot through RotTransSV, then rotates the
 * result by the camera yaw.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - the scratchpad is ONE struct reached through ONE pointer local. Bare
 *    constant addresses cost 194 words: each repeated address becomes its own
 *    CSE'd full constant in a callee-saved register, where the target keeps
 *    only 0x1F800000 in $s1 and uses displacements. A `u8 *` base with
 *    `*(T *)(b + off)` gives the right word count but 123 differ: those MEMs
 *    are not MEM_IN_STRUCT_P, so true_dependence lets sched1 hoist every
 *    s->unk_NN load above the scratchpad stores and shuffle the stores among
 *    themselves. With struct fields on both sides memrefs_conflict_p cannot
 *    separate $s0 from $s1 and the prologue stays in source order.
 *  - unk_30/unk_34 are unions of a word and a halfword, because the function
 *    stores a word and reads the low halfword back at the same address; the
 *    mode difference is what makes the read a real lhu.
 *  - s->unk_20 and s->unk_22 each go through an s32 local used twice. Without
 *    the locals each is loaded twice (165 words), because the scratchpad store
 *    between the two uses invalidates cse's entry. With u16 locals, an SImode
 *    store of a HImode pseudo adds an andi per value (164 words).
 *  - the four statements after RotTransSV are in the order unk_00, unk_02,
 *    unk_30, unk_38. The obvious order (unk_00, unk_30, unk_02, unk_38) is 52
 *    DIFF: it loses both the empty delay slot after the first lhu and the
 *    genuine reload of the angle before func_80064584. Found by sweeping all
 *    24 permutations, which is worth doing whenever a cluster is close but
 *    mis-ordered.
 *  - g->unk_C8 is hoisted into a local before the `(s16) g->unk_B8 - c8`
 *    subtraction: gcc expands the operands left to right, so the local is the
 *    only way to get the target's lh of +0xC8 before the lh of +0xB8.
 *  - the three ScaleMatrix stores are written 0x30, 0x34, 0x38 even though the
 *    target emits 0x30, 0x38, 0x34 -- sched1 moves the 0x2000 store up.
 *  - each sign arm is a complete store duplicated in both branches, not a
 *    conditional value feeding one store. A ternary is 154 words and a
 *    negate-in-place local 153; the duplicated form is what cross-jumping
 *    merges back into the shared subu/sh tail. */
void func_8003DBD0(UnkSlot *s, u8 *r, s16 *out) {
    Unk80083160 *g = &D_80083160;
    Unk3DBD0SP *p = (Unk3DBD0SP *) 0x1F800000;
    s32 t20;
    s32 t22;
    s32 c8;

    PushMatrix();
    p->unk_30.w = g->unk_C4;
    p->unk_34.w = g->unk_C6;
    p->unk_38 = g->unk_C8;
    p->unk_100 = (s->unk_16 - 0x400) - p->unk_30.h;
    p->unk_104 = g->unk_B8 + (s->unk_1A - p->unk_34.h);
    p->unk_102 = s->unk_18;
    t20 = s->unk_20;
    p->unk_E4 = t20;
    p->unk_108 = t20;
    t22 = s->unk_22;
    p->unk_EC = 0;
    p->unk_E8 = t22;
    p->unk_10A = t22;
    RotMatrix(&p->unk_100, p->unk_D0);
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38 = 0x2000;
    ScaleMatrix(p->unk_D0, &p->unk_30);
    SetTransMatrix(p->unk_D0);
    SetRotMatrix(p->unk_D0);
    p->unk_24 = s->unk_14;
    if ((r[0] ^ p->unk_24) & 1) {
        p->unk_70 = -(s8) r[2] - p->unk_108;
    } else {
        p->unk_70 = (s8) r[2] - p->unk_108;
    }
    if ((r[0] ^ p->unk_24) & 2) {
        p->unk_74 = -(s8) r[3] - p->unk_10A;
    } else {
        p->unk_74 = (s8) r[3] - p->unk_10A;
    }
    p->unk_72 = 0;
    RotTransSV(&p->unk_70, out, &p->unk_94);
    p->unk_00 = out[0];
    p->unk_02 = out[1];
    p->unk_30.w = -g->unk_C4;
    c8 = g->unk_C8;
    p->unk_38 = (s16) g->unk_B8 - c8;
    out[2] = out[2] + ((p->unk_02 * func_80064584(p->unk_30.w)) >> 12);
    out[0] = (p->unk_00 * func_80064584(p->unk_38)) >> 12;
    out[1] = (p->unk_00 * func_800644B8(p->unk_38)) >> 12;
    PopMatrix();
}

u8 *func_8003DE58(u8 *arg0, UnkSlot *s, s16 *out, s16 arg3) {
    s16 tmp[4];
    u8 *r = func_8004CAE8(arg0, arg3);

    if (r != NULL) {
        func_8003DBD0(s, r, tmp);
        out[0] = tmp[0];
        out[1] = tmp[1];
        out[2] = tmp[2];
    } else if (s->unk_14 & 0x6000) {
        r = arg0;
        out[1] = 0;
        out[0] = 0;
        out[2] = -0x40;
    }
    return r;
}

Unk800777AC *func_8003DF0C(Unk800777AC *p, s32 arg1, s16 *arg2) {
    Unk800777AC *found = NULL;
    u32 i;

    *arg2 = 0;
    i = 0;
    do {
        if ((p->unk_00 & 0x20) && p->unk_06 == arg1) {
            found = p;
            *arg2 = i;
        }
        i++;
    } while ((p++->unk_00 & 0x80) == 0);
    *arg2 = *arg2 - (i >> 1);
    return found;
}

u8 *func_8003DF74(u8 *arg0, UnkSlot *s, s16 *out, s16 arg3) {
    s16 tmp[4];
    s16 idx[4];
    u8 *r = (u8 *) func_8003DF0C((Unk800777AC *) arg0, arg3, idx);

    if (r != NULL) {
        func_8003DBD0(s, r, tmp);
        out[0] = tmp[0];
        out[1] = tmp[1];
        out[2] = tmp[2];
    } else if (s->unk_14 & 0x6000) {
        r = arg0;
        out[1] = 0;
        out[0] = 0;
        out[2] = -0x40;
    }
    return r;
}

void func_8003E02C(s16 *arg0, s16 *out) {
    out[0] = (func_80064584(D_80083160.unk_C8) * arg0[0]) >> 12;
    out[1] = -(func_800644B8(D_80083160.unk_C8) * arg0[0]) >> 12;
    out[2] = arg0[1] * 2;
}

void func_8003E0C4(void *m, void *arg1, void *arg2) {
    s32 flag;

    PushMatrix();
    SetTransMatrix(m);
    SetRotMatrix(m);
    RotTrans(arg1, arg2, &flag);
    PopMatrix();
}
