/* The remainder of unk_11940 after the gprel_12A40 carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);
void func_8003E70C(void);
void func_8003F624(void);
s32 func_8003F688(s32);
u8 func_8003F240(void);

/* File-scope definitions for the whole unit: func_8003F2A4 and
 * func_8003F368 reach these four objects $gp-relatively while reaching others
 * absolutely, which no -G value can express. These are the definitions the
 * original translation unit had; cc1psx emits a `.comm` for each and maspsx
 * rewrites this unit's loads and stores of them to %gp_rel. Nothing is
 * allocated -- the objects are already defined in asm/data/gp_data.data.s and
 * a tentative definition is a COMMON symbol that merges with that definition
 * -- so there is no linker placement to arrange. See the longer note on
 * func_8003F624 in csrc/unk_12DEC.c.
 *
 * A definition is FILE-SCOPE, so it was checked against every function in the
 * unit: func_8003F270 reaches D_800814D0's second byte and is therefore spelled
 * through the D_800814D1 scalar below, func_8003F320 touches none of the four,
 * func_8003F55C uses D_800814B8 and func_8003F534/func_8003F540 use D_8006CE34.
 * `tools/decomp/gaudit.py` reports nothing for this unit while func_8003F368 is
 * still assembly, since it reads the compiler's output and only func_8003F2A4
 * is in it. When func_8003F368 lands, D_800814D0 will be mentioned by two
 * functions and gaudit will ask for it to be checked; both want it
 * $gp-relative, so that report will be expected rather than a problem.
 *
 * D_800814D0 is also defined in csrc/gprel_11B4C.c and csrc/unk_11F0C.c, which
 * reach it $gp-relatively too. Several commons for one symbol is fine: they
 * merge with each other and with the real definition in the data file.
 *
 * This unit stays at the project default -G0 and must NOT join the
 * MASPSXFLAGS_G8 list: at -G8 the one-byte D_800814D1 and D_800814D3 would go
 * $gp-relative too, the same reason csrc/gprel_11B4C.c gives. */
u8 D_800814D0[4];
u8 D_80081450[4];
u32 D_800814B0[1];
u32 D_800814B4[1];

s32 func_8003F270(void) {
    /* The target materialises the address of element 1 and reaches element 0
     * through it with a -1 displacement, and reloads element 0 for the
     * subtraction rather than keeping it live; the separate `t` is what stops
     * the second read being held in a register.
     *
     * Spelled through the D_800814D1 scalar rather than D_800814D0[1], for two
     * reasons. It is what the listing names, so verify.py no longer flags the
     * two relocated words whose addend differed (`%lo(D_800814D0)+1` against
     * `%lo(D_800814D1)`). And it is REQUIRED now that this unit carries a
     * file-scope `.size D_800814D0` record for its two $gp functions: maspsx's
     * rewrite keys on the symbol and ignores the displacement, so through the
     * array spelling this function's `lbu D_800814D0+1` is rewritten to
     * `%gp_rel` as well and the lui/%lo pair collapses -- 12 words against 13,
     * 11 differing. This is the func_8003E34C trap, and here it is avoidable
     * without a carve because the two spellings name different symbols. */
    u8 *p = &D_800814D1;
    u32 v = D_800814D1;
    u8 t = p[-1];

    if (v < t) {
        v += 0x20;
    }
    return v - p[-1];
}

/* Validates one queue entry: if it is not a type-6 entry, or not the one the
 * read index points at, it is cancelled; otherwise its two payload words are
 * copied into D_800814B0/D_800814B4 and the entry is repointed at them.
 *
 * Two shapes carry it. The test is written as `if (!c1 || !c2) { fail } else
 * { body }`, NOT as `if (c1 && c2) { body } else { fail }`: the listing's block
 * order is `bne` falling through, `beq` to the else label, then the fail block,
 * then the body, which is do_jump's TRUTH_ORIF_EXPR layout with
 * `if_true_label == 0`. The `&&` form puts the body first -- 15 differing
 * words.
 *
 * And `D_800814B0`/`D_800814B4` are reached as ARRAY elements, which is what
 * makes the target re-read `arg0->unk_04` for the second word instead of
 * caching it. Through plain `u32` scalars the stores are non-struct references
 * at non-varying addresses, true_dependence's differing-flags exception
 * applies, cse's `invalidate` leaves the cached base load in place, the second
 * read becomes a `move` and sched1 hoists both loads above both stores: 29
 * words against 31, all 31 differing. The declaration is doing the work here,
 * not the source -- see the note in include/structs.h and
 * docs/matching-levers.md section 5. */
void func_8003F2A4(Unk80083968 *arg0) {
    if (arg0->unk_00 != 6 || arg0 != &D_80083968[D_800814D0[0]]) {
        arg0->unk_00 = 0;
    } else {
        D_800814B0[0] = ((u32 *) arg0->unk_04)[0];
        D_800814B4[0] = ((u32 *) arg0->unk_04)[1];
        arg0->unk_04 = (s32) D_800814B0;
    }
}

void func_8003F320(void) {
    while (func_8003F270() != 0) {
        func_8003CB08();
        func_80044618(2);
        func_8003E758();
    }
}

/* Reads the drive's current MSF position into loc[] and returns it as a
 * sector number, or 0 if the drive did not settle or the position is not a
 * valid MSF (minutes < 75, seconds < 60, frames < 75, sector > 0).
 *
 * The validation tail is NOT written as one `&&` chain. Nineteen spellings of
 * the chain all measured 4 DIFF: the target leaves the SECOND guard's delay
 * slot empty and puts the THIRD guard's compare in $v1, and both come from a
 * `return 0` that sits between the third `slti` and its `beqz` before reorg.
 * That is what `if (!(C)) return 0;` produces: cc1psx's jump1 hoists the
 * fall-through arm's `v0 = 0` above the conditional jump and inverts it to
 * the return label (upstream 2.7.2 hoists the OTHER arm; this is measured,
 * not read). With `v0` reborn between the compare's birth and death the
 * compare cannot take $v0 (local-alloc.c:1416, :2108) and falls to $v1;
 * reorg's backward scan then steals that `v0 = 0` into the third guard's slot
 * and leaves a `(use (insn))` marker where it was (reorg.c:2943, :2244),
 * which mark_target_live_regs counts as a set of $v0 live at the second
 * guard's fall-through (:2698-2704), so the second guard may not copy the
 * `return 0` from its target (:3375-3376) and keeps a nop. See
 * docs/matching-levers.md, "A `return 0` BETWEEN A COMPARE AND ITS BRANCH". */
s32 func_8003F368(void) {
    u8 loc[4];
    u8 res[8];
    u8 *q;
    u32 saved;
    s32 i;
    s32 pos;

    saved = *(u32 *) D_80081450;
    *(u32 *) D_80081450 = 0;
    if (CdSync(1, res) != 2) {
        return 0;
    }
restart:
    i = 0x10;
    for (;;) {
        if (CdControl(0x10, NULL, D_80081450) == 1) {
            break;
        }
        if (--i == 0) {
            func_8003E70C();
            func_8003F624();
            i = 0x10;
        }
    }
    i = 0x10;
    q = &D_800814D3[0];
    for (;;) {
        if (CdSync(1, D_80081450) == 2) {
            break;
        }
        if (--i == 0) {
            func_8003E70C();
            if (func_8003F240() == 0x1B) {
                D_800814D3[0] = 0xFF;
                if (q[-3] != 0) {
                    D_800814D0[0]--;
                } else {
                    q[-3] = 0x1F;
                }
                D_800814D0[0] &= 0x1F;
                func_8003F624();
                goto restart;
            }
            *(u32 *) D_80081450 = saved;
            i = 0x10;
        }
    }
    loc[0] = D_80081450[0];
    loc[1] = D_80081451;
    loc[2] = D_80081452;
    pos = CdPosToInt((CdlLOC *)loc);
    if (func_8003F688(D_80081450[0]) < 0x4B && func_8003F688(D_80081451) < 0x3C) {
        if (!(func_8003F688(D_80081452) < 0x4B)) {
            return 0;
        }
        if (pos > 0) {
            return pos;
        }
    }
    return 0;
}

s32 func_8003F52C(s32 arg0) {
    return arg0;
}

Unk8006CE34 *func_8003F534(void) {
    return &D_8006CE34;
}

void func_8003F540(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    D_8006CE34.unk_00 = arg0;
    D_8006CE34.unk_04 = arg1;
    D_8006CE34.unk_08 = arg2;
    D_8006CE34.unk_0C = arg3;
}

s32 func_8003F55C(void) {
    if (CdSync(1, NULL) == 2) {
        CdControl(0x10, NULL, (u8 *)&D_800814B8);
    }
    return CdPosToInt(&D_800814B8);
}
