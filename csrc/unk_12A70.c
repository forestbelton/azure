/* The remainder of unk_11940 after the gprel_12A40 carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

/* One file-scope size record for the whole unit: func_8003F2A4 and
 * func_8003F368 reach these four objects $gp-relatively while reaching others
 * absolutely, which no -G value can express. It defines nothing -- .sdata stays
 * 0 bytes and every symbol stays UND -- so there is no linker placement to
 * arrange. See the longer note on func_8003F624 in csrc/unk_12DEC.c.
 *
 * The record is FILE-SCOPE, so it was checked against every function in the
 * unit: func_8003F270 reaches D_800814D0's second byte and is therefore spelled
 * through the D_800814D1 scalar below, func_8003F320 touches none of the four,
 * func_8003F55C uses D_800814B8 and func_8003F534/func_8003F540 use D_8006CE34.
 * `tools/decomp/gaudit.py` reports nothing for this unit while func_8003F368 is
 * still assembly, since it reads the compiler's output and only func_8003F2A4
 * is in it. When func_8003F368 lands, D_800814D0 will be mentioned by two
 * functions and gaudit will ask for it to be checked; both want it
 * $gp-relative, so that report will be expected rather than a problem.
 *
 * This unit stays at the project default -G0 and must NOT join the
 * MASPSXFLAGS_G8 list: at -G8 the one-byte D_800814D1 and D_800814D3 would go
 * $gp-relative too, the same reason csrc/gprel_11B4C.c records. */
__asm__(".sdata\n\t.size D_800814D0,1\n\t.size D_80081450,4\n"
        "\t.size D_800814B0,4\n\t.size D_800814B4,4\n\t.text");

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

INCLUDE_ASM("asm/nonmatchings/unk_12A70", func_8003F368);

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
