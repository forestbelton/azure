/* A carved-out translation unit: these four consecutive functions reach
 * D_800814E8 $gp-relatively and nothing else does, so they were one original
 * translation unit -- a $gp-relative reference means the unit DEFINED the
 * object, which is the tentative definition below. cc1psx emits `.comm`,
 * maspsx's `--use-comm-section` files it under `sbss_entries`, and `_uses_gp`
 * rewrites this unit's accesses to %gp_rel at the project default gas -G0; no
 * window definition and no linker placement are needed, because the object is
 * already a GLOBAL dlabel in asm/data/gp_data.data.s and a COMMON merges with
 * it. Same reasoning as gprel_17504 -- see that file and
 * docs/addressing-and-translation-units.md.
 *
 * THIS UNIT USED TO BE BUILT AT -G2, and the history is worth keeping because
 * it is the clearest case against the old mechanism. At a nonzero -G, gas
 * makes any small UNDEFINED symbol $gp-relative, so every symbol these
 * functions reached absolutely had to be audited: D_80083120 and D_80082E60
 * (both already compiler-split -- an incomplete array of structs and a
 * 0x19-byte struct), D_80082E6E in func_80041BE4 (which is why structs.h
 * declares that one as an incomplete array), and func_80041AE4's D_80081480
 * and D_8008148C, which are u32 reached absolutely in the BARE-SYMBOL form.
 * No declaration can give that form at -G8, so the unit was built at -G2:
 * gas emits GPREL16 only for a symbol whose `.extern SYM,size` is in (0, G],
 * and cc1psx emits that size from the declared type, so -G2 kept the u16
 * $gp-relative and sent the u32s absolute. The definition below asks for the
 * one symbol directly, so the -G value stops mattering: at -G0 gas makes no
 * small-data decision at all and the u32s get the bare-symbol absolute form
 * unconditionally. What survives from the audit is the declaration advice --
 * plain scalar declarations, nothing added to structs.h, and no
 * incomplete-array shim, which gives the compiler-split form and is 9 words
 * wrong here.
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

u16 D_800814E8;

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
