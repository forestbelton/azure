/* The D_80080AA4..D_80080AB0 $gp cluster, carved out of unk_102A8: these two
 * consecutive functions reach that run $gp-relatively and nothing else in
 * unk_102A8 does, so they were their own original translation unit. Built with
 * MASPSXFLAGS_G8; see docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_8003D7DC(void);
void func_8003D0F0(void);
void func_8003CCB0(s32);
s8 *func_8004E298(u8 *, u8 *, s32);

/* The per-frame tick for the overworld compass trail: advances the VSync
 * stamp, flips the display buffer, shifts the 0x3F-entry x/y history down by
 * one, writes this frame's position into slot 0 from the heading angle, and
 * draws the four trail segments.
 *
 * TWO POINTER-LOCAL SHAPES carry most of the function, and both are additions
 * to matching-levers section 6 rather than instances of what it already said.
 *
 *  - `Unk80083160 *g = &D_80083160;` is a ONE-set pointer local, and that
 *    collapses to a bare `lui %hi` which is then relocated into the function's
 *    ENTRY block. expand emits the object's `la` (`lui %hi` + `addiu %lo`)
 *    there; cse rewrites every `g->field` MEM as a lo_sum off the `(high)`
 *    pseudo, the `addiu %lo` goes dead and flow deletes it, leaving
 *    `lui $s0,%hi(D_80083160)` in the entry block and `lw %lo(D_80083160)($s0)`
 *    at each of the four uses. It is the only way found to get a large
 *    in-image object's `%hi` materialised BEFORE the first conditional branch,
 *    and that one insn is worth a word and 104 differing words here: it
 *    creates an anti-dependence that forces `sw $s0` early in the prologue,
 *    which leaves `sw $ra` for the first branch's delay slot and empties the
 *    second `beq`'s slot (the target's nop). Using `D_80083160.unk_00`
 *    directly: 105 words, 104 DIFF.
 *  - the history arrays are reached through a BASE pointer plus a constant,
 *    not through `&ARRAY[K]`. `&D_80083858[0x3F]` folds the constant into the
 *    address and emits `lui %hi(SYM+0x7E)` / `addiu %lo(SYM+0x7E)` -- one pair
 *    per pointer -- whereas `b1 = D_80083858; d1 = b1 + 0x3F; s1 = b1 + 0x3E;`
 *    materialises the symbol once and adds 0x7E/0x7C with their own `addiu`s,
 *    which is the target: 2 words and 62 differing. This is the mirror image
 *    of section 6's "nothing folds a constant into a lo_sum", which states only
 *    the `p += CONST` direction.
 *
 * Also load-bearing, each measured by reverting it: four pointer bivs in the
 * copy loop rather than `b1[i] = b1[i - 1]` indexing (2 words, 62 DIFF);
 * `i = 0x3F;` before the pointer setup rather than in the `for` init (9 DIFF);
 * `d1`'s decrement as its own statement after the two copies (3 DIFF);
 * `p = D_801C9E40;` then a conditional `p += 0x108D4;` rather than an if/else
 * (2 words, 108 DIFF); `(u32) VSync(1) >> 8` for the `srl` (1 DIFF); the
 * `(s16)` casts on both multiplies (2 DIFF); and the operand order of the
 * D_80080AA6 test (1 DIFF).
 *
 * `u = (t >> 7);` as its own statement -- the SAME local at both call sites, so
 * it has two sets -- closed the last 11 words, and the mechanism is worth
 * carrying: expand_assignment evaluates a store's lvalue address before its
 * RHS, so `D_80083858[0] = (t >> 7) + ((t >> 10) + 0x100);` puts the store's
 * `lui %hi` ahead of the first `sra` and sched1 keeps it there on the LUID
 * tie-break. Giving the RHS's first subexpression its own statement moves the
 * `lui` after the `sra`. When a `lui %hi` sits one slot too early, that is the
 * lever -- not moving the store. The `u + ((t >> 10) + K)` grouping is separate
 * and worth 4 more words. */
void func_8003CB08(void) {
    s32 i;
    u32 v;
    s32 t;
    s32 u;
    u16 *b1;
    u16 *b2;
    u16 *s1;
    u16 *d1;
    u16 *s2;
    u16 *d2;
    u8 *p;
    Unk80083160 *g = &D_80083160;

    if ((D_80082E60.unk_00 & 3) != 1) {
        return;
    }
    v = (u32) VSync(1) >> 8;
    if ((u8) v == D_80080AAA) {
        return;
    }
    D_80080AAA = v;
    PutDispEnv(g->unk_00 + 0x5C);
    PutDrawEnv(g->unk_00);
    p = D_801C9E40;
    if (g->unk_00 == p) {
        p += 0x108D4;
    }
    g->unk_00 = p;
    *(u32 *) (p + 0x8D0) = (u32) (p + 0x8D4);
    if (D_80080AA6 != D_80083960[0]) {
        D_80080AA6 = D_80083960[0];
        D_80080AA8 = 8;
    }
    i = 0x3F;
    b2 = D_800838D8;
    d2 = b2 + 0x3F;
    s2 = b2 + 0x3E;
    b1 = D_80083858;
    d1 = b1 + 0x3F;
    s1 = b1 + 0x3E;
    for (; i > 0; i--) {
        *d1 = *s1--;
        *d2-- = *s2--;
        d1--;
    }
    t = func_80064584((s16) D_80080AA4 * 80);
    u = (t >> 7);
    D_80083858[0] = u + ((t >> 10) + 0x100);
    t = func_800644B8((s16) D_80080AA4 * 80);
    u = (t >> 7);
    D_800838D8[0] = u + ((t >> 10) + 0xB0);
    func_8003D0F0();
    for (i = 3; i >= 0; i--) {
        func_8003CCB0(i);
    }
    if (D_80080AA8 != 0) {
        D_80080AA8--;
    }
    DrawSync(0);
    D_80080AA4++;
}

/* Draws one interpolated frame of the 14-segment compass trail. On the first
 * call it builds the 14 glyph records through func_8004E298; then per segment
 * it lerps the x/y history by arg0/8, builds the matrix in the 0x1F800000
 * scratchpad, projects the four corners with RotTransSV, clamps the UVs and
 * hands one 0x28-byte primitive to DrawPrim.
 *
 * TWO SHAPES CLOSED THIS, and both are corrections to what the levers doc said
 * rather than applications of it.
 *
 * `r` IS REUSED FOR THE D_80080AB0 LOAD after the entry test, and that is what
 * keeps the return-value copy `addu $v1,$v0,$zero` alive in the init loop. One
 * allocno with two ranges imports the SECOND range's hard-register conflicts:
 * in the join block the target's schedule is `lui $v0,%hi(D_800838D8)` /
 * `lw $v1,D_80080AB0` / `addiu $fp,$v0,%lo(...)`, so a block-local quantity
 * holds $v0 across `r`'s range there, `global_conflicts` ORs reg 2 into
 * `hard_reg_conflicts[r]`, `prune_preferences` can no longer honour `r`'s
 * `preferences: 2`, and the copy becomes a real instruction instead of
 * coalescing away. 29 DIFF to 6, no extra word. Two facts make this the ONLY
 * route, both measured: a pseudo set from a hard register does not conflict
 * with it, because `global_conflicts` processes an insn's REG_DEAD notes before
 * its stores -- so nothing written inside the init loop can reach it, and the
 * live thing has to be in another block; and a plain pointer copy is not a
 * second pseudo, because cse's `canon_reg` rewrites the uses to the quantity's
 * first register and the copy dies (all eight placements of the three uses
 * across `r` and a copy gave byte-identical output).
 *
 * `g` IS A SINGLE-SET POINTER LOCAL WITH TWO USES IN DIFFERENT BLOCKS, which
 * is what gives the compiler-split `lui %hi` / `addiu %lo` / `lw 0x0(reg)` form
 * at both sites -- and the register is reload's spill register, not an
 * allocated one. The old shape here was two sets wrapped in identical
 * `if`/`else` arms, which does select the split form but makes the pseudo a
 * genuine allocno; it then inherits `preferences: 2` from the block-local
 * `(high)` quantity and takes $v0, which is the 6-DIFF state. Deleting all
 * four assignments and both wrappers in favour of one initialised declaration
 * is the match. See the corrected section 6 entry: the split form turns on the
 * USE count, not the set count, and one set with one use in the same block
 * folds back to `lui` + `lw %lo` (measured four ways, 2 words each).
 *
 * verify.py flags the two `%lo(D_80083858)+4` / `%lo(D_800838D8)+4` words,
 * which is the expected output for an interior address spelled through its
 * container. */
void func_8003CCB0(s32 arg0) {
    Unk3CCB0SP *sp = (Unk3CCB0SP *) 0x1F800000;
    Unk80083160 *g = &D_80083160;
    s8 *r;
    u8 *c;
    u8 *cc;
    u8 name[2];
    u8 *pb;
    u8 *m;
    s32 *flag;
    s32 i;
    s32 cur;
    s32 cur2;
    s32 a;
    s32 b;
    s16 t;
    s32 h;
    s32 x;
    s32 y;

    if (*(s16 *) &D_80080AAC == 0) {
        D_80080AAC = (u16) D_80080AAC + 1;
        name[1] = 0;
        for (i = 0; i < 0xE; i++) {
            name[0] = D_8006CDB8[i];
            r = func_8004E298(&D_80083798[i * 0xC], name, 0);
            r[3] = -4;
            r[2] = -4;
            if (i == 0) {
                D_80080AB0 = (u32) r;
            }
        }
    }
    pb = *(u8 **) (g->unk_00 + 0x8D0);
    sp->unk_48 = 0;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    i = 0;
    PushMatrix();
    sp->unk_28 = 0;
    sp->unk_2A = 0;
    sp->unk_2C = 0;
    r = (s8 *) D_80080AB0;
    c = (u8 *) r + 4;
    flag = &sp->unk_94;
    for (; i < 0xE; i++) {
        cc = c + i * 0xC;
        cur = (s16) D_80083858[i * 2];
        sp->unk_40 = cur + ((s16) D_80083858[i * 2 + 2] - cur) * arg0 / 8;
        cur2 = (s16) D_800838D8[i * 2];
        m = sp->unk_50;
        sp->unk_44 = cur2 + ((s16) D_800838D8[i * 2 + 2] - cur2) * arg0 / 8;
        TransMatrix(m, &sp->unk_40);
        RotMatrix(&sp->unk_28, m);
        SetRotMatrix(m);
        SetTransMatrix(m);
        a = cc[4];
        sp->unk_08 = a;
        b = cc[6];
        sp->unk_10 = b;
        if (a + b >= 0x100) {
            sp->unk_10 = b - 1;
        }
        a = cc[5];
        sp->unk_0C = a;
        b = cc[7];
        sp->unk_14 = b;
        if (a + b >= 0x100) {
            sp->unk_14 = b - 1;
        }
        h = (u16) sp->unk_10;
        t = (s8) cc[-2];
        sp->unk_80 = t;
        sp->unk_70 = t;
        t = t + h;
        sp->unk_88 = t;
        sp->unk_78 = t;
        h = (u16) sp->unk_14;
        t = (s8) cc[-1];
        sp->unk_7A = t;
        sp->unk_72 = t;
        t = t + h;
        sp->unk_8A = t;
        sp->unk_82 = t;
        RotTransSV(&sp->unk_70, pb + 8, flag);
        RotTransSV(&sp->unk_78, pb + 0x10, flag);
        RotTransSV(&sp->unk_80, pb + 0x18, flag);
        RotTransSV(&sp->unk_88, pb + 0x20, flag);
        sp->unk_10 = sp->unk_10 + sp->unk_08;
        sp->unk_14 = (sp->unk_14 + sp->unk_0C) << 8;
        sp->unk_0C = sp->unk_0C << 8;
        *(u16 *) (pb + 0xE) = *(u16 *) (cc + 2);
        *(u16 *) (pb + 0xC) = (u16) sp->unk_0C + (u16) sp->unk_08;
        *(u16 *) (pb + 0x14) = (u16) sp->unk_0C + (u16) sp->unk_10;
        *(u16 *) (pb + 0x16) = *(u16 *) cc;
        *(u16 *) (pb + 0x1C) = (u16) sp->unk_14 + (u16) sp->unk_08;
        *(u16 *) (pb + 0x24) = (u16) sp->unk_14 + (u16) sp->unk_10;
        if (*(s16 *) (pb + 8) > *(s16 *) (pb + 0x20)) {
            pb[0x14] = pb[0x24]--;
        }
        if (*(s16 *) (pb + 0xA) > *(s16 *) (pb + 0x22)) {
            pb[0x1D] = pb[0x25]--;
        }
        pb[3] = 9;
        pb[7] = 0x2C;
        x = ((func_800644B8(((s16) D_80080AA4 << 8) + i * 64) >> 6) + 0x80) >> (arg0 / 2);
        pb[4] = x;
        y = 0;
        if (D_80080AA8 == 0) {
            y = x;
        }
        pb[6] = y;
        pb[5] = y;
        pb[7] |= 2;
        DrawPrim(pb);
        pb += 0x28;
    }
    PopMatrix();
    *(u8 **) (g->unk_00 + 0x8D0) = pb;
}
