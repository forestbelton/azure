/* The D_80080AA4..D_80080AB0 $gp cluster, carved out of unk_102A8: these two
 * consecutive functions reach that run $gp-relatively and nothing else in
 * unk_102A8 does, so they were their own original translation unit. Built with
 * MASPSXFLAGS_G8; see docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_8003D7DC(void);
void func_8003D0F0(void);
void func_8003CCB0(s32);

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

INCLUDE_ASM("asm/nonmatchings/gprel_10308", func_8003CCB0);
