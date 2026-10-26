/* func_8003AB44 (unk_72A8) -- 54 of 54 words, 9 DIFF. NOT compiled.
 *
 * Moves a VRAM rectangle: x = arg5 + arg2 * D_8006A8C8[arg4], and if that is
 * in [0x140, 0x400) it fills a RECT from func_8004D880/func_8004D91C and
 * MoveImages it to (x, arg6 + arg3 * D_8006A8DC[arg4]).
 *
 * It was 52 of 54 until 2026-10-26, two words short of the target's extra
 * callee-saved register. **That was argument 6 declared `s32` with an
 * `(s16)` cast at the use** -- see the narrow-parameter entry in
 * matching-levers section 7, which this function and func_8004A24C are the
 * two measurements behind. Declaring it `s16` restores the `lw $s3` at entry
 * and the `sll`/`sra` at the use, and the word count is now exact.
 *
 * NOTE THE COMMITTED PROTOTYPE IS STILL WRONG. csrc/unk_72A8.c declares
 * `void func_8003AB44(u8 *, s16, s32, u8, s16, s32, s32);` and func_8003AAC4
 * (landed, matching) calls it. Landing this source means narrowing argument 6
 * to `s16` there, which changes the CALLER's argument setup -- check
 * func_8003AAC4 still matches, or give this function its own declaration
 * through an `__asm__` alias (carved-sweep has the idiom for data; it works
 * on a function declarator too).
 *
 * WHAT IS LEFT is a pure 3-cycle register rotation, which advise.py reports as
 * ALLOC with every opcode, immediate, displacement and branch offset already
 * matching:
 *     built s1 = arg4*4   s2 = arg6      s3 = arg3
 *     target s2 = arg4*4  s3 = arg6      s1 = arg3
 * so the two `sw $sN,off($sp)` offsets differ as a consequence, not a cause.
 * Read matching-levers section 3 and `advise.py --greg` before sweeping
 * shapes; `search.py`'s families do not reach a rotation.
 */

#include "common.h"
extern s32 D_8006A8C8[];
extern s32 D_8006A8DC[];

s32 func_8004D880(s32);
void func_8004D91C(u8, s16 *);

void func_8003AB44(u8 *p, s16 b, s32 c, u8 d, s16 e, s16 f, s32 g) {
    RECT r;
    s32 x;

    x = e + b * D_8006A8C8[d];
    if (x >= 0x140 && x < 0x400) {
        func_8004D91C(func_8004D880(p[1] | (p[0] << 8)), (s16 *) &r);
        MoveImage(&r, x, f + (s16) c * D_8006A8DC[d]);
    }
}
