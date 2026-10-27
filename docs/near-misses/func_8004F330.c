/* func_8004F330 (unk_1C540) -- 40 of 40 words, 9 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep. Fills n 0x10-byte slots from the four-word D_8002E5D8 template,
 * cross-links each slot's +0x240/+0x244 pair, and copies three words from
 * D_8002E5E8 into +0x204 after the loop.
 *
 * WHAT IS PAID FOR
 *
 * * `u32 *s = D_8002E5D8;` assigned INSIDE the loop body, so its
 *   materialisation lands in the preheader AFTER the `blez` (func_8004CCBC's
 *   lever), with the FIRST word read through the symbol
 *   (`lw %lo(D_8002E5D8)($t5)`) and the other three through `s`.
 * * `u32 *t = D_8002E5E8;` assigned AFTER the loop, or its base is hoisted
 *   ahead of the guard.
 * * The 0x2B8 store written BEFORE the 0x240/0x244 pair, even though the
 *   listing emits it after: four other orderings measured at 30-41 DIFF.
 * * THE p/q SPLIT IS THE REVERSE OF THE OBVIOUS ONE: `u8 *q = p;` as a
 *   declaration initialiser, the LOOP's stores through `p`, and the three
 *   TRAILING stores through `q`. The other way round -- loop through `q`,
 *   trailing through `p` -- is 40 words and 15 DIFF, and it gets the
 *   registers RIGHT (the loop addresses off $t3, as the listing has them)
 *   while losing the insn order inside the body; with everything through one
 *   of them the copy coalesces and the function is a word short (39).
 *
 * WHAT IS LEFT: NINE WORDS, and they are one allocation decision with its
 * knock-ons. The target copies the parameter to $t3 BEFORE the guard and uses
 * $t3 for everything, which frees $a0 as the scratch for the in-loop address
 * value `p + (0x2AC + i * 0x10)`:
 *
 *     addu $t3, $a0, $zero ; blez $a1 ; addu $a3, $zero, $zero
 *     ...
 *     addu $a0, $t3, $t2   <- the 0x2AC address, in $a0
 *     addu $v0, $t3, $t1   <- the 0x23C address, in $v0
 *
 * This source keeps `p` in $a0 and puts the copy in $t3, so both address
 * values go to $v0/$v1 and the copy is emitted after the guard. For the
 * target's assignment the 0x2AC address value has to be allocated BEFORE the
 * parameter -- read `advise.py --greg` and compare the two quantities, and
 * note that the 15-DIFF variant above already has the registers, so the
 * answer may be that variant plus whatever fixes its ORDER rather than this
 * one plus whatever fixes its registers.
 */

#include "common.h"

void func_8004F330(u8 *p, s32 n) {
    s32 i;
    u32 *t;
    u8 *q = p;

    for (i = 0; i < n; i++) {
        u32 *s = D_8002E5D8;

        *(u32 *) (p + 0x220 + i * 4) = (u32) (p + 0x23C + i * 0x10);
        *(u32 *) (p + 0x2AC + i * 0x10) = D_8002E5D8[0];
        *(u32 *) (p + 0x2B0 + i * 0x10) = s[1];
        *(u32 *) (p + 0x2B4 + i * 0x10) = s[2];
        *(u32 *) (p + 0x2B8 + i * 0x10) = s[3];
        *(u32 *) (p + 0x240 + i * 0x10) = (u32) (p + 0x2AC + i * 0x10);
        *(u32 *) (p + 0x244 + i * 0x10) = (u32) (p + 0x204);
    }
    t = D_8002E5E8;
    *(u32 *) (q + 0x204) = t[0];
    *(u32 *) (q + 0x208) = t[1];
    *(u32 *) (q + 0x20C) = t[2];
}
