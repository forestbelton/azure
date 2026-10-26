/* func_8004DE88 (unk_1C540) -- 64 of 65 words, one register copy. NOT compiled.
 *
 * Builds a command buffer in D_80083E18: a leading 0x08, the argument string
 * via strcpy, then 0x19, 0x02, 0x1A, then the ADDRESS of D_80083D88 and that
 * address plus four, each written out as four separate bytes (an unaligned
 * 32-bit store done by hand). It then dispatches through
 * D_800714A4[D_80081550] -- the idiom func_8004DCF4 above it already uses --
 * and initialises D_80083D88[0] and [1].
 *
 * `strrchr(buf, 0)` is how it finds the end of the string; strcpy and strrchr
 * are not declared anywhere in include/, so they need hand prototypes beside
 * the function (structs.h declares memcpy the same way).
 *
 * THE MISSING WORD is `addu $a0,$v0,$zero`, a copy of strrchr's return value.
 * The target keeps the cursor in $a0 and the three byte constants plus the
 * D_80083D88 address value in $v0; this source keeps the cursor in $v0 (the
 * return register, so the copy is deleted) and the constants in $v1. So the
 * question is only which quantity wins $v0, and the candidate is local-alloc's
 * SUGGESTION pass: a quantity born from a copy of a hard register is assigned
 * that register before the priority loop runs, and the cursor is born from a
 * copy of $v0. For the target's allocation, that suggestion has to have been
 * blocked -- i.e. the cursor conflicts with $v0 -- which would be the case if
 * the address value in $v0 is live across the cursor's whole range. Reading
 * local-alloc.c's suggestion phase against the .lreg dump is the next step;
 * `advise.py --greg` prints the conflicts but not the suggestions.
 *
 * MEASURED AND NOT WORKING: a separate `e` variable with `p = e;` (62 words --
 * gcc coalesces both and loses two more); the three stores as `p[0] = c; p++;`
 * instead of `*p++ = c;` (64, unchanged); and computing the end pointer as
 * `&D_80083E18[1] + strlen(s)` instead of with strrchr (68).
 */

#include "common.h"
extern u8 D_80083E18[];
extern u32 D_80083D88[];

char *strcpy(char *, const char *);
char *strrchr(const char *, s32);

void func_8004DE88(u8 *s) {
    u8 *p;
    u32 a;

    D_80083E18[0] = 8;
    strcpy((char *) &D_80083E18[1], (char *) s);
    p = (u8 *) strrchr((char *) D_80083E18, 0);
    *p++ = 0x19;
    *p++ = 2;
    *p++ = 0x1A;
    a = (u32) D_80083D88;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    p += 4;
    a += 4;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80083E18);
    D_80083D88[0] = 0x2300030C;
    D_80083D88[1] = 0x2300040C;
}
