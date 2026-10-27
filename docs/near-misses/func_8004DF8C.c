/* func_8004DF8C (unk_1C540) -- 70 of 72 words, two words short. NOT compiled.
 *
 * The sibling of func_8004DE88 (also parked): the same command buffer in
 * D_80083E18, but with a strcat of D_80071404 after the strcpy, and the two
 * addresses it writes out byte-wise are D_80081548 and D_8008154C instead of
 * D_80083D88 and D_80083D88+4.
 *
 * WHAT THIS FUNCTION IS WORTH READING FOR is the $gp-relative half, which
 * refines docs/addressing-and-translation-units.md's bare-size-record trick.
 * The target wants BOTH forms of the same two symbols at once: the address
 * materialised ABSOLUTELY (lui/addiu, two words, because the address is
 * written into the buffer as four bytes) and the final store in ONE word.
 * Getting that needs two things, and the second is new:
 *
 *   1. the symbols in maspsx's table, via
 *      `__asm__(".sdata\n\t.size D_80081548,4\n\t.size D_8008154C,4\n"
 *               "\t.size D_80081550,4\n\t.text");`
 *      -- D_80081550 too, because the dispatch through D_800714A4 reads it
 *      $gp-relatively. Nothing else in the unit touches 0x1548 or 0x154C, so
 *      adding them is safe; grep before adding any others, as the header of
 *      csrc/unk_1C540.c says.
 *   2. the symbols declared as INCOMPLETE ARRAYS, `extern u32 D_80081548[];`,
 *      and written `D_80081548[0] = ...`. Declared `extern u32 D_80081548;`
 *      instead, cc1psx emits its own `.extern D_80081548, 4` and `&D_80081548`
 *      becomes a single `la`, which maspsx then makes $gp-relative as well --
 *      the address materialisation collapses from two words to one and the
 *      function comes out FOUR words short rather than two. The incomplete
 *      array emits no size for cc1psx to act on, so the address stays
 *      lui/addiu and only the store is rewritten. That is the combination the
 *      docs describe but it is NOT what the obvious declaration gives.
 *
 * MEASURED: the two symbols are NOT one array. `extern u32 D_80081548[]` with
 * `&D_80081548[0]` and `&D_80081548[1]` lets gcc share one base and the second
 * materialisation drops to a single word -- 68 words. The target has two full
 * lui/addiu pairs with different %lo, so they are separate symbols, even
 * though its D_80081548 store is `sw $v1,-0x4($s2)` off the OTHER symbol's
 * materialised address, which is the one thing here still unexplained.
 *
 * THE REMAINING TWO WORDS are in the same family as func_8004DE88's one: the
 * byte-writing cursor and the address value competing for registers. Read that
 * file's header first -- it has the local-alloc numbers and `advise.py --greg`
 * now prints them.
 */

#include "common.h"
extern u8 D_80083E18[];
extern u8 D_80071404[];
__asm__(".sdata\n\t.size D_80081548,4\n\t.size D_8008154C,4\n"
        "\t.size D_80081550,4\n\t.text");

extern u32 D_80081548[];
extern u32 D_8008154C[];

char *strcpy(char *, const char *);
char *strcat(char *, const char *);
char *strrchr(const char *, s32);

void func_8004DF8C(u8 *s) {
    u8 *b;
    u8 *p;
    u32 a;

    D_80083E18[0] = 8;
    b = &D_80083E18[1];
    strcpy((char *) b, (char *) s);
    strcat((char *) b, (char *) D_80071404);
    p = (u8 *) strrchr((char *) D_80083E18, 0);
    *p++ = 0x19;
    *p++ = 2;
    *p++ = 0x1A;
    a = (u32) D_80081548;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    p += 4;
    a = (u32) D_8008154C;
    p[0] = a;
    p[1] = a >> 8;
    p[2] = a >> 16;
    p[3] = a >> 24;
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80083E18);
    D_80081548[0] = 0x2300030C;
    D_8008154C[0] = 0x2300040C;
}
