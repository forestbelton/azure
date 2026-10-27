/* func_8003931C (unk_72A8) -- 85 of 85 words, 10 DIFF. NOT compiled.
 *
 * A script opcode handler, and the same shape as func_80039470 (landed): it
 * reads a byte from the cursor at p+0x1C, indexes D_80082660 by it, and either
 * rewinds two bytes and installs func_800383D4 / func_80038408, or fills the
 * slot and writes two halfwords into the object func_800392A4 returns -- at
 * +0x36/+0x38 when the slot is 1 and the +0x10 field is neither D_800C3174 nor
 * D_800C321C, and at +0x88/+0x8A otherwise.
 *
 * THREE SHAPES WERE NEEDED to reach the exact word count, and all three are
 * already documented, which is why this is a cheap resume:
 *   - the cursor reads are ONE statement, `n = *(*(u8 **) (p + 0x1C))++;`.
 *     Split into two they cost a register copy (carved-sweep, and
 *     matching-levers section 8).
 *   - both error blocks have to be `else` arms, not guard clauses with an
 *     early return: as guards gcc leaves them INLINE and the first `bne`
 *     spans 7 words instead of the target's 51.
 *   - the table base goes through a pointer local, `Unk80082660 *t =
 *     D_80082660;` then `&t[k]`, so its lui/addiu precedes the index. Exactly
 *     what func_80039470 needed.
 *
 * WHAT IS LEFT is a two-register swap, and --greg gives the margin: the target
 * puts the opcode byte in $s3 and the +0x80 pointer in $s4, this source the
 * other way round, and global.c's priority has them at
 *   pseudo 81 (the byte)    4 refs / 23 insns -> 0.348
 *   pseudo 84 (the pointer) 6 refs / 32 insns -> 0.375
 * so the pointer wins the lower register here and did not in the original.
 * floor_log2(4) and floor_log2(6) are both 2, so ONE more reference on the
 * byte flips it (2*5/23 = 0.435). Nothing found adds one at zero instruction
 * cost: `k = n` is already its own statement, the index must be the masked
 * `k` rather than `n` (the target has `andi` then `sll`), and the two error
 * blocks already store `n` rather than `k`.
 *
 * MEASURED AND NOT WORKING, all 85 words / 10 DIFF: the +0x80 read hoisted
 * above the table index, `r` declared first, and `k = n = *(...)++;` as one
 * chained statement.
 */

#include "common.h"
void func_800383D4(u8 *);
void func_80038408(u8 *);

void func_8003931C(u8 *p) {
    u8 n;
    s32 k;
    u8 *e;
    u8 *r;
    Unk80082660 *t;

    n = *(*(u8 **) (p + 0x1C))++;
    k = n;
    t = D_80082660;
    e = (u8 *) &t[k];
    r = *(u8 **) (p + 0x80);
    if (*(s8 *) (e + 1) == 0) {
        if (k != 0) {
            u8 *q = func_800392A4(k);

            if (q != 0) {
                u8 *f = q - 0x20;

                e[0] = *(*(u8 **) (p + 0x1C))++;
                e[1] = 1;
                e[2] = r[0x48];
                if (k == 1 && *(u32 **) (f + 0x10) != D_800C3174 && *(u32 **) (f + 0x10) != D_800C321C) {
                    *(s16 *) (q + 0x36) = *(u16 *) (r + 0x4C);
                    *(s16 *) (q + 0x38) = *(u16 *) (r + 0x50);
                } else {
                    *(s16 *) (q + 0x88) = *(u16 *) (r + 0x4C);
                    *(s16 *) (q + 0x8A) = *(u16 *) (r + 0x50);
                }
            } else {
                *(u32 *) (p + 0x10) = (u32) func_80038408;
                p[0x2F] = n;
                p[0x89] = 0x10;
                *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) - 2;
            }
        }
    } else {
        *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) - 2;
        p[0x2F] = n;
        *(u32 *) (p + 0x10) = (u32) func_800383D4;
    }
}
