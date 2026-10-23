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
 * declares that one as an incomplete array. That check on func_80041AE4's D_80081480 and
 * D_8008148C is now DONE, and the answer changed this unit's build: they are
 * u32 reached absolutely in the bare-symbol form, which no declaration can
 * give at -G8, so the unit is built at **-G2** instead. gas emits GPREL16 only
 * for a symbol whose `.extern SYM,size` is in (0, G], and cc1psx emits that
 * size from the declared type -- so -G2 keeps the u16 $gp objects
 * $gp-relative and sends the u32s absolute. Plain scalar declarations, nothing
 * added to structs.h, and no incomplete-array shim: that shim gives the
 * compiler-split form and is 9 words wrong here.
 */
#include "common.h"

void func_80041CBC(void);
void func_80041BE4(void);
void func_80041C64(void);
void func_80040A88(void (*)(void));
void func_80040B88(void);
void func_80041B98(void);
void func_8003E2D8(void);
void func_8003D92C(void);
void func_80040FDC(u16);
void func_80041038(u16);
void func_800411FC(u16);
s32  func_80040F2C(u16);
s32  func_8003F794(s16, s16);

/* Enters the mode whose callback is func_80041B98: snapshot the frame counter,
 * run the three per-mode setup calls on the same byte, and either hand off to
 * func_80040B88 or arm the callback with a fresh timer.
 *
 * Load-bearing:
 *  - `D_80082E60.unk_0B` is re-read at all four uses. Caching it in a local
 *    costs 33 words (levers section 4).
 *  - `D_80082E60.unk_00 &= ~1;` comes BEFORE the func_8003F794 call. After it
 *    costs 9 words -- and note func_80043D04, this function's twin, has the
 *    two in the OTHER order, which is worth 9 words there in reverse.
 *
 * The `$a0 = 5` in the beqz's delay slot is not an argument to
 * func_80040B88: it is reorg stealing the first insn of the taken thread
 * (func_8003F794's first argument) and redirecting the branch past it. */
void func_80041AE4(void) {
    D_80081480 = D_8008148C;
    func_8003E2D8();
    func_80040FDC(D_80082E60.unk_0B);
    func_80041038(D_80082E60.unk_0B);
    func_800411FC(D_80082E60.unk_0B);
    if (func_80040F2C(D_80082E60.unk_0B) != 0) {
        func_80040B88();
    } else {
        D_80082E60.unk_00 &= ~1;
        D_800814E8 = func_8003F794(5, 8);
        func_80040A88(func_80041B98);
    }
}

void func_80041B98(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        func_80040A88(func_80041BE4);
    }
}

/* The mode's per-frame tick: if the slot is free, restart the timer, then
 * re-arm the callback -- or bail out through func_8003D92C.
 *
 * Load-bearing: the `(s16)` cast on D_800814E8, which is declared u16. Without
 * it you get `lhu` where the target has `lh`, one differing word (levers
 * section 7). The duplicated `lui $a0,%hi(func_80041C64)` in the target is
 * reorg stealing the join-point insn, not a source duplication, so the single
 * call after the inner `if` is right. */
void func_80041BE4(void) {
    func_80041CBC();
    if (D_80082E6E[0] != 0) {
        if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
            D_800814E8 = func_8003F794(6, 8);
        }
        func_80040A88(func_80041C64);
    } else {
        func_8003D92C();
    }
}

void func_80041C64(void) {
    func_80041CBC();
    if (D_80083120[(s16) D_800814E8].unk_00 == 0) {
        D_80082E60.unk_00 |= 1;
        func_80040B88();
    }
}
