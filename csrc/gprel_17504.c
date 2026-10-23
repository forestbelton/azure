/* A carved-out translation unit: these four functions reach D_80081500
 * $gp-relatively, so gas is left at -G8 here rather than maspsx's forced -G0
 * (see MASPSXFLAGS_G8 in the Makefile).
 *
 * They are one original translation unit on the evidence: of the 38
 * $gp-relative symbols in the game, 36 are referenced from exactly one of the
 * project's units, and within unk_14934 they cluster into runs of CONSECUTIVE
 * functions -- D_80081500 is reached by exactly these four and by nothing
 * else, and they sit between two decompiled functions that touch no $gp
 * symbol at all. A $gp-relative reference means the function was compiled in
 * the unit that defined the global, so this run was its own file.
 *
 * Carving it is what makes these matchable. The unit needs BOTH addressing
 * modes only because unk_14934 lumps several original units together; split
 * out, this one needs only $gp-relative and the existing -G8 carve-out
 * mechanism covers it, with no window definition and no linker placement.
 */
#include "common.h"

void func_80043EB8(void);
void func_80043E60(void);
void func_80043E04(void);
void func_80040A88(void (*)(void));
void func_80040B88(void);
s32  func_8003F794(s16, s16);
void func_8003D92C(void);

INCLUDE_ASM("asm/nonmatchings/gprel_17504", func_80043D04);

void func_80043DB8(void) {
    func_80043EB8();
    if (D_80083120[(s16) D_80081500].unk_00 == 0) {
        func_80040A88(func_80043E04);
    }
}

/* D_80082E6E is reached as its own symbol rather than through Unk80082E60, and
 * is declared an INCOMPLETE ARRAY in structs.h so cc1psx compiler-splits the
 * address itself: this unit is built at -G8, where gas would otherwise make it
 * $gp-relative, since it sits inside the $gp window even though the target
 * reaches it absolutely. That costs a word. The same hazard applies to any
 * absolute-reached symbol within gp +/- 0x8000 in a carved unit. */
void func_80043E04(void) {
    func_80043EB8();
    if (D_80082E6E[0] != 0) {
        D_80081500 = func_8003F794(6, 8);
        func_80040A88(func_80043E60);
    } else {
        func_8003D92C();
    }
}

void func_80043E60(void) {
    func_80043EB8();
    if (D_80083120[(s16) D_80081500].unk_00 == 0) {
        D_80082E60.unk_00 |= 1;
        func_80040B88();
    }
}
