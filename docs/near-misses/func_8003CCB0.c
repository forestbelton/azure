/* func_8003CCB0 (gprel_10308) -- 272 of 272 words, 29 DIFF. NOT compiled.
 *
 * Draws one interpolated frame of the 14-segment compass trail: on the first
 * call it builds the 14 glyph records through func_8004E298, then per segment
 * it lerps the x/y history by arg0/8, builds the matrix in the 0x1F800000
 * scratchpad, projects the four corners with RotTransSV, clamps the UVs and
 * hands one 0x28-byte primitive to DrawPrim.
 *
 * WHAT IS LEFT is two spots, and together they currently cancel in the word
 * count, so 272 of 272 is exact while 29 words differ:
 *
 *  (a) the init loop lacks the target's `addu $v1,$v0,$zero` copy of
 *      func_8004E298's return value -- 1 word, which shifts 8 more. From
 *      `.greg` the pseudo prints `preferences: 2` and conflicts with no
 *      caller-saved hard register, so it takes $v0 and the copy coalesces
 *      away. The target's $v1 needs something live in hard $v0 across that
 *      pseudo's range, and no spelling produces that without extra words.
 *  (b) the join block puts the `g` address pseudo in $v0 where the target has
 *      $a3, and leaves the `lw`'s load-delay slot empty where the target
 *      schedules `i = 0` into it -- 1 word, which offsets (a).
 *
 * RULED OUT for (a), all at 272 words with no improvement unless noted:
 * reusing the variable for `cc`; reusing it for `pb` (273 words); swapping the
 * two byte stores; `*(s8 *) (r + 3) = -4` casts; moving the `if` ahead of the
 * stores (273 words) or between them; `(u32) &r[0]`; adding an `else` arm;
 * hoisting -4 into a local (33 DIFF); passing `name` through a pointer local
 * (273 words); the init loop as a backward `goto` loop (276 words).
 *
 * RULED OUT for (b): statement order does not reach it. `i = 0` before `pb`,
 * before the zero stores, as the `for` init, in its own variable, with
 * `g->unk_00` split into its own local, and `flag`'s assignment moved above
 * PushMatrix all give exactly 29 DIFF. Keeping one `g` live across the loop
 * instead of re-materialising it costs 2 words (270, 68 DIFF).
 * `search.py --g8` compiled 76 variants over 8 families on the 37-DIFF
 * predecessor and improved nothing.
 *
 * THE TWO `?` WORDS ARE BENIGN and expected: `%hi`/`%lo(D_80083858)` with
 * addend 4 where the listing prints `D_8008385C`, and the same for
 * `D_800838D8`/`D_800838DC`. Both resolve to identical half-words. That is the
 * price of spelling the interior addresses through the container, which is
 * what keeps them out of the -G8 hazard -- the same trade-off the D_80083164
 * note in include/structs.h records.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - `*(s16 *) &D_80080AAC == 0` for the entry test, because a plain `(s16)`
 *    cast compared against zero is folded away and the target's first insn is
 *    `lh`: 271 words, 271 DIFF. The declaration-side alternative (retype to
 *    s16, put the `(u16)` cast at the one arithmetic site) is section 7's
 *    documented mirror image and would be safe globally -- only this function
 *    references the symbol -- but is untested.
 *  - the record pointer INDEXED, `c = (u8 *) D_80080AB0 + 4;` with
 *    `cc = c + i * 0xC;` and constant displacements, rather than a walking
 *    `c += 0xC`: 2 words, 246 DIFF. Likewise the prim fields off the `pb` biv
 *    with no `q = pb + 7` variable (2 words, 246 DIFF) and
 *    `&D_80083798[i * 0xC]` as the call argument (1 word, 241 DIFF). The
 *    mechanism is section 9's one-register-per-base rule.
 *  - identical `if`/`else` arms around `g = &D_80083160` at BOTH sites, which
 *    is how a two-set pointer local survives cse: dropping the first costs 31
 *    differing words at an unchanged 272, dropping the last costs a word.
 *  - `h = (u16) sp->unk_1X;` hoisted BEFORE the two `sh` stores, so sched1
 *    leaves the `addu` where the target has it: 39 DIFF.
 *  - `s16 t`, the HImode consumer that blocks combine's byte-to-signed refold:
 *    4 words, 162 DIFF.
 *  - the `(s16)` casts on the history reads (2 words, 233 DIFF); the `(u16)`
 *    casts reading the u32 scratchpad words (31 DIFF); `pb[0x14] =
 *    pb[0x24]--;` as one statement (1 word, 95 DIFF); the clamp comparison's
 *    operand order (37 DIFF); `s8 *` as func_8004E298's return type, which
 *    makes `-4` build as `addiu rd,$zero,-0x4` (30 DIFF); and the loop notes
 *    on both loops (4 words / 250 DIFF and 6 words / 263 DIFF).
 *
 * MEASURED INERT, so do not spend time on them: `sp->unk_0C <<= 8` with or
 * without a `(u32)` cast; `y = 0; if (...) y = x;` against the ternary; the
 * `flag` and `m` pointer locals (inlining them is byte-identical); the
 * `r[3]`/`r[2]` store order.
 *
 * DECLARATIONS NEEDED WHEN THIS LANDS, beyond the Unk3CCB0SP below:
 *  - `extern u8 D_80083798[0xA8];` (0xE records of 0xC bytes; incomplete
 *    measures identically). It sits at gp + 0x2E04, INSIDE the window, so the
 *    declared size must exceed -G8.
 *  - `extern u8 D_8006CDB8[0xE];`. Below the window, so no -G8 hazard either
 *    way.
 *  - `s8 *func_8004E298(u8 *, u8 *, s32);`.
 *  Both objects are referenced only by this function, so both are safe to add.
 *
 * nmcheck: 272 of 272 words, 29 DIFF
 */
#include "common.h"

typedef struct Unk3CCB0SP {
    u8  pad_00[0x8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u8  pad_18[0x10];
    s16 unk_28;
    s16 unk_2A;
    s16 unk_2C;
    u8  pad_2E[0x12];
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    u8  pad_4C[0x4];
    u8  unk_50[0x20];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8  pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8  pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8  pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8  pad_8E[0x6];
    s32 unk_94;
} Unk3CCB0SP;

extern u8 D_80083798[];
extern u8 D_8006CDB8[];

s8 *func_8004E298(u8 *, u8 *, s32);

void func_8003CCB0(s32 arg0) {
    Unk3CCB0SP *sp = (Unk3CCB0SP *) 0x1F800000;
    Unk80083160 *g;
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
        g = &D_80083160;
    } else {
        g = &D_80083160;
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
    c = (u8 *) D_80080AB0 + 4;
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
    if (arg0) {
        g = &D_80083160;
    } else {
        g = &D_80083160;
    }
    *(u8 **) (g->unk_00 + 0x8D0) = pb;
}
