/* ELEVEN VERIFIED FUNCTIONS WAITING ON ONE STRUCTURAL CHANGE.
 *
 * Every function below is a clean verify.py MATCH with an exact word count,
 * relocations spot-checked by hand (R_MIPS_GPREL16 on the defined .sdata
 * symbol beside R_MIPS_HI16/LO16 on the undefined ones). They are NOT in
 * csrc/ because none of them can be landed until the translation-unit split
 * described at the end of this header happens. Nothing here needs
 * re-deriving; when the split lands, these go straight in.
 *
 * WHY THEY WERE BLOCKED, AND WHAT CHANGED. Each one reaches a global
 * $gp-relatively while reaching others absolutely. The two modes looked
 * mutually exclusive because they are chosen per assembler invocation. They
 * are not: maspsx rewrites a bare-symbol load or store to %gp_rel when it has
 * seen a definition of that symbol in a .sdata/.sbss section of the same
 * unit, and leaves undefined symbols alone for gas to expand absolutely. So
 * defining the object in the unit selects the mode per symbol. Full mechanism,
 * including why it is maspsx and not gas, in
 * docs/addressing-and-translation-units.md.
 *
 * The recipe each file uses, and the `.size` line is the load-bearing one:
 *
 *     __asm__(".sdata\n\t.globl D_800814E0\n\t.type D_800814E0,@object\n"
 *             "\t.size D_800814E0,4\nD_800814E0:\n\t.word 0\n\t.text");
 *
 * WHAT STILL BLOCKS THEM. The definition has to be in the same object file as
 * the code, so the window cannot live in one shared C file -- each unit must
 * define the objects its own functions reach. That is fine in principle,
 * because the objects cluster by unit exactly as the addressing analysis
 * predicts (D_800814D0 only in unk_11940, D_800814E8 and D_80081500 only in
 * unk_14934, and so on -- more evidence that $gp-relative access means shared
 * original translation unit). But the objects' addresses are scattered through
 * the window while a unit's .sdata is one contiguous block, and the two
 * gp-only functions that would otherwise be a cheap -G8 carve-out
 * (func_80044618 and func_80044698) sit in the MIDDLE of csrc/unk_14934.c, so
 * carving them means splitting that file in three and renaming the tail.
 *
 * So the remaining blocker is not addressing any more -- it is the same
 * csrc/unk_14934.c split that the jump-table work needs (there at
 * func_80041588). Those two items should be done as one deliberate
 * restructuring. See docs/open-matching-problems.md.
 *
 * Word counts, all exact: func_8003F5E0 3, func_8003F240 12, func_80041B98 19,
 * func_80043DB8 19, func_80041C64 22, func_80043E60 22, func_80043E04 23,
 * func_800448BC 24, func_8003F624 25, func_80041134 30, func_800410FC 5.
 *
 * TYPES THE ACCESSES PROVE, three of which disagree with include/structs.h.
 * All three are reachable from the existing declaration with a cast, which is
 * what these sources do, so retyping is optional:
 *   D_800814E8  s16, not u16 -- `lh` in two functions, used as an array index
 *   D_80081500  s16, not u16 -- `lh` in two, `sh` in a third
 *   D_80081508  a pointer, not u32 -- `lw`, then its value bases `lbu 0x0`,
 *               `lbu 0x1` and `addiu 0x4`, so it points at a 4-byte record
 * And one flagged as a guess: D_800814D4 is proved to be at least one byte (an
 * `sb` of 0x84, address handed to CdControlB as a CdlSetmode parameter block),
 * but it is a libcd command buffer and may originally have been wider. Window
 * B is all zeros, so sizing it 1 and padding with .space is byte-safe either
 * way.
 *
 * DECLARATIONS NEEDED WHEN THESE LAND:
 *   s32 CdControlB(u8, u8 *, u8 *);     beside the existing CdControl
 *   extern u8 D_80082E6E;               byte +0x0E of Unk80082E60 reached as
 *                                       its own symbol, exactly like the
 *                                       existing `extern u8 D_80082E6A;`
 *   void func_8003D92C(void);           still an INCLUDE_ASM stub, declared
 *                                       nowhere
 *   u8 func_8003F240(void); s32 func_800448BC(u16, u16);
 *   void func_80041134(void (*)(void));
 * No new typedef is needed: every struct these touch already exists, which is
 * the point -- they were blocked purely on addressing.
 *
 * Per-unit forward declarations for intra-unit callees defined later in the
 * file: in unk_14934, func_80041CBC, func_80043EB8, func_80041BE4,
 * func_80043E04, func_80043E60; in unk_12DE0, func_8003E70C (which lives in
 * unk_11940).
 */

/* ---- zz_a_3F5E0.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814C8\n\t.type D_800814C8,@object\n"
        "\t.size D_800814C8,4\nD_800814C8:\n\t.word 0\n\t.text");

void func_8003F5E0(u32 arg0) {
    D_800814C8 = arg0;
}

/* ---- zz_b_3F240.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814D0\n\t.type D_800814D0,@object\n"
        "\t.size D_800814D0,4\nD_800814D0:\n\t.word 0\n\t.text");

u8 func_8003F240(void) {
    return D_80083968[(D_800814D0[0] + 0x1F) & 0x1F].unk_00;
}

/* ---- zz_c_41B98.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814E8\n\t.type D_800814E8,@object\n"
        "\t.size D_800814E8,2\nD_800814E8:\n\t.half 0\n\t.text");

void func_80041CBC(void);
void func_80041BE4(void);

void func_80041B98(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        func_80040A88(func_80041BE4);
    }
}

/* ---- zz_d_43DB8.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_80081500\n\t.type D_80081500,@object\n"
        "\t.size D_80081500,2\nD_80081500:\n\t.half 0\n\t.text");

void func_80043EB8(void);
void func_80043E04(void);

void func_80043DB8(void) {
    func_80043EB8();
    if (D_80083120[(s16) D_80081500].unk_00 == 0) {
        func_80040A88(func_80043E04);
    }
}

/* ---- zz_e_41C64.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814E8\n\t.type D_800814E8,@object\n"
        "\t.size D_800814E8,2\nD_800814E8:\n\t.half 0\n\t.text");

void func_80041CBC(void);
void func_80040B88(void);

void func_80041C64(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        D_80082E60.unk_00 |= 1;
        func_80040B88();
    }
}

/* ---- zz_f_43E60.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_80081500\n\t.type D_80081500,@object\n"
        "\t.size D_80081500,2\nD_80081500:\n\t.half 0\n\t.text");

void func_80043EB8(void);
void func_80040B88(void);

void func_80043E60(void) {
    func_80043EB8();
    if (D_80083120[(s16) D_80081500].unk_00 == 0) {
        D_80082E60.unk_00 |= 1;
        func_80040B88();
    }
}

/* ---- zz_g3_43E04scalar.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_80081500\n\t.type D_80081500,@object\n"
        "\t.size D_80081500,2\nD_80081500:\n\t.half 0\n\t.text");

extern u8 D_80082E6E;
void func_80043EB8(void);
void func_80043E60(void);
s32  func_8003F794(s16, s16);
void func_8003D92C(void);

void func_80043E04(void) {
    func_80043EB8();
    if (D_80082E6E != 0) {
        D_80081500 = func_8003F794(6, 8);
        func_80040A88(func_80043E60);
    } else {
        func_8003D92C();
    }
}

/* ---- zz_h_448BC.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_80081508\n\t.type D_80081508,@object\n"
        "\t.size D_80081508,4\nD_80081508:\n\t.word 0\n\t.text");

s32 func_800448BC(u16 arg0, u16 arg1) {
    u8 *p = (u8 *) D_80081508;

    while (p[0] != 0) {
        if (p[0] == arg0 && p[1] == arg1) {
            return 1;
        }
        p += 4;
    }
    return 0;
}

/* ---- zz_i_3F624.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814D4\n\t.type D_800814D4,@object\n"
        "\t.size D_800814D4,1\nD_800814D4:\n\t.byte 0\n\t.text");

s32  CdControlB(u8, u8 *, u8 *);
void func_8003E70C(void);
void func_8003F5EC(void);

void func_8003F624(void) {
    s32 i;

    D_800814D4 = 0x84;
    for (i = 0x10; i != 0; i--) {
        if (CdControlB(0xE, &D_800814D4, NULL) != 0) {
            goto done;
        }
    }
    func_8003E70C();
done:
    func_8003F5EC();
}

/* ---- zz_j_41134.c ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814E0\n\t.type D_800814E0,@object\n"
        "\t.size D_800814E0,4\nD_800814E0:\n\t.word 0\n\t.text");

void func_80041134(void (*arg0)(void)) {
    void (**p)(void) = D_80083C68;

    while (*p != NULL) {
        if (*p == arg0) {
            D_800814E0--;
            do {
                p[0] = p[1];
                p++;
            } while (*p != NULL);
            return;
        }
        p++;
    }
}

/* ---- func_800410FC, verified separately; it is the proof case in
   docs/addressing-and-translation-units.md ---- */
#include "common.h"

__asm__(".sdata\n\t.globl D_800814E0\n\t.type D_800814E0,@object\n"
        "\t.size D_800814E0,4\nD_800814E0:\n\t.word 0\n\t.text");

void func_800410FC(void) {
    D_800814E0 = (s32 *) D_80083C68;
    D_80083C68[0] = NULL;
}
