/* A carved-out translation unit: these four consecutive functions reach
 * D_800814E8 $gp-relatively and nothing else does, so they were one original
 * translation unit. gas is left at -G8 here rather than maspsx's forced -G0
 * (see MASPSXFLAGS_G8 in the Makefile); no window definition and no linker
 * placement are needed. Same reasoning as gprel_17504 -- see that file and
 * docs/addressing-and-translation-units.md.
 *
 * Audited before carving, which is the step to repeat: every symbol these
 * functions reach ABSOLUTELY must either sit outside gp +/- 0x8000 or be
 * declared so cc1psx compiler-splits the address itself, because at -G8 gas
 * makes any small undefined symbol $gp-relative. Here that is D_80083120 and
 * D_80082E60 (both already split -- an incomplete array of structs and a
 * 0x19-byte struct), and D_80082E6E in func_80041BE4, which is why structs.h
 * declares that one as an incomplete array. func_80041AE4's D_80081480 and
 * D_8008148C need no attention while it is still an INCLUDE_ASM stub: the
 * asm carries its own %hi/%lo and -G does not touch it. They WILL need
 * checking when it is decompiled.
 */
#include "common.h"

void func_80041CBC(void);
void func_80041BE4(void);
void func_80041C64(void);
void func_80040A88(void (*)(void));
void func_80040B88(void);

INCLUDE_ASM("asm/nonmatchings/gprel_152E4", func_80041AE4);

void func_80041B98(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        func_80040A88(func_80041BE4);
    }
}

INCLUDE_ASM("asm/nonmatchings/gprel_152E4", func_80041BE4);

void func_80041C64(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        D_80082E60.unk_00 |= 1;
        func_80040B88();
    }
}
