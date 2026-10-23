/* func_80041E70 (unk_154BC) -- 270 of 270 words, 4 DIFF. Not compiled; see
 * docs/open-matching-problems.md.
 *
 * It rescales a record's size/speed pair by flag bits and by func_80042900's
 * sign, then clamps-or-copies four byte fields and six flag-bit pairs.
 * Everything is byte-exact except four words, and they are ONE problem:
 *
 *     X  li   $v1,32          |  sll  $v1, $v1, 1
 *     X  sra  $v0,$v0,0x10    |  sra  $v1, $v0, 16
 *     X  blez $v0,...         |  blez $v1, .L80041EE0
 *     X  bgez $v0,...         |  bgez $v1, .L80041F04
 *
 * The root is the first. cse folds `p->unk_87 * 2` to the constant 32: the
 * store `p->unk_87 = 0x10;` sits immediately before the branch and the if-body
 * is the fall-through, so mem(0x87)'s equivalence class carries (const_int 16)
 * into the body. Confirmed in the .cse dump -- insn 30 is already
 * `(set (reg:SI 89) (const_int 32))`.
 *
 * And it is not a coincidence that the fold wins: cse weighs its candidates by
 * rtx_cost, a CONST_INT costs ~0 and `(ashift reg 1)` costs an insn, so the
 * constant is always cheaper. For the target's shape cse would have to not
 * know the register holds 16 at all -- and MIPS cannot store an immediate, so
 * expand always emits `(set (reg) (const_int 16))` before the `sb`, which is
 * exactly what gives cse the equivalence.
 *
 * CLOSED WITH A PROOF 2026-10-22, and the wall is not the cost. Cost decides
 * which equivalent wins once cse knows the constant; what decides whether it
 * knows it is the RUN BOUNDARY. A cse run is a maximal linear insn range that
 * ends only at a CODE_LABEL with LABEL_NUSES != 1 -- it follows a conditional
 * jump's fall-through AND its target, and crosses barriers and single-use
 * labels. Four probes: body as fall-through, body as a `bne` target, body
 * behind an unconditional jump plus barrier -- all fold; a body label with TWO
 * uses -- the `sll` survives. No `-fno-cse-follow-jumps`,
 * `-fno-cse-skip-blocks`, `-fno-thread-jumps` or `-fno-rerun-cse-after-loop`
 * changes any of it, so this was never a flag question.
 *
 * Three things block buying that two-use label, each worth knowing:
 *   - `thread_jumps` runs in jump1, BEFORE cse, so a provably redundant second
 *     predecessor is already gone: three equivalent OR-ed tests compile to one
 *     `beqz` with the body as fall-through, 270 words, still `li 32`.
 *   - jump1 also does dead-pseudo-set DCE, so a dead assignment cannot hold an
 *     arm open -- the set, the jump and the label are all in `.rtl` and all
 *     gone in `.jump`.
 *   - cse deletes a redundant store of the same value to the same address, so a
 *     duplicated store cannot hold one open either.
 *
 * And the target's first block provably has no label in it: its prologue is
 * interleaved with a store, and the constant materialisation fills the
 * load-delay slot between a load and its consumer. Only a scheduler does
 * either, and schedulers do not cross labels. So the only boundary available
 * is the body's own entry label, which is the case that folds.
 *
 * The one zero-word way to make a run boundary is an `if`/`else` with IDENTICAL
 * arms (now matching-levers section 1): jump2 merges them, the branch becomes a
 * jump-to-next and is deleted, and delete_computation removes the condition's
 * insns too -- even a load. Used here it gives 270 words with the whole body
 * byte-exact and two words lost in the first block instead, because it is also
 * a sched1/sched2 boundary. Placement sweep, all worse: arms holding the `sb`,
 * or the boundary before the `lw`, give 271 and an unfillable load-delay nop;
 * arms holding `n = 0x10` give 270 but the fold returns, because identical
 * single-insn arms are collapsed before cse runs.
 *
 * A shape with an opaque third test (`|| p->unk_11 || p->unk_25`) reproduces
 * the body byte-for-byte at +2 words, which is the proof that the rest of this
 * function is right.
 *
 * ALSO CORRECTED: the claim below that the three register words are DOWNSTREAM
 * of the fold. They are not. With the fold defeated and the body byte-exact,
 * the call result still comes out `sll`/`sra` coalesced onto $v0, so that is an
 * independent allocation problem -- and in every shape where the 16 became a
 * global allocno it took $a0, never the target's $v1. Worth its own small
 * dispatch.
 *
 * One more: a narrow local is still an SImode pseudo (PROMOTE_MODE), so `u16`,
 * `s16` and `u16 n <<= 1` all fold to `li 32` exactly as `s32` does -- there is
 * no paradoxical-subreg escape from cse's equiv_constant.
 *
 * The three register fields are downstream of the fold, not independent: with
 * it, the 0x10 pseudo dies at the `sb` before the branch and is block-local, so
 * LOCAL-alloc takes it; without it the pseudo would be live across the branch
 * and become a global allocno, which is almost certainly what pushes the
 * sign-extended call result off $v0 onto $v1 in the target.
 *
 * Note the asymmetry that is already right: the `sb` at +0x87 DOES invalidate
 * mem(0x6E), which is why the target reloads unk_6E with `lhu` inside the if,
 * and this source reproduces that. So cse's memory invalidation is coarse
 * enough to cross field offsets -- which also rules out reordering the two
 * entry stores, because then mem(0x87) would be killed too and the body would
 * need an `lbu` the target does not have.
 *
 * FIVE MORE PROBES (2026-10-22, second batch), all still `li 32`, run against
 * a minimal reproduction that otherwise matches the target instruction for
 * instruction (`li 16` / `andi` / `beq` / `sb` in the delay slot / `lhu` / ...
 * / `sll` / `sh`): the literal with the field read back; an `s32` local doubled
 * IN PLACE (`n *= 2`); the same written `n = n * 2`; the local given a SECOND
 * SET later in the function, so it is genuinely multi-set; and the local with
 * the field read back for the store. The multi-set shape is worth noting
 * because it is what defeats combine's `nonzero_bits` elsewhere (see
 * docs/matching-levers.md section 7) -- it does nothing here, because cse's
 * quantity still carries the constant at the point of the shift.
 *
 * What the target's instructions say about the shape, which narrows it: the
 * doubling is `sll $v1,$v1,1` on the REGISTER with no reload, while unk_6E in
 * the same block IS reloaded (`lhu`) because the `sb` at +0x87 invalidated it.
 * So the original cse knew `mem(0x87) == reg` and did NOT know `reg == 16`,
 * and that split is the thing to reproduce. Every spelling measured so far
 * gives cse both facts or neither.
 *
 * Five further probes (2026-10-22, first batch) against a minimal
 * reproduction, all still `li 32`: the field read back in the body; an `s32` local doubled in place;
 * the same local given a second use after the if; the stored value derived as
 * `p->unk_6E >> 4`; and the local also used to compute the halfword's value.
 * Note what those probes also show -- the halfword's doubling stays a real
 * `sll` in every one of them, because its reload is genuine, so the byte and
 * halfword halves of this statement pair behave differently and only the byte
 * one folds.
 *
 * Ruled out for the fold: `p->unk_87 <<= 1` and `* 2` (identical); the 0x10 in
 * an s32 local, a u8 local, or read back from the field; `p->unk_87 = n << 1`
 * from the local; the field as s8 (271 words, 242 DIFF -- so u8 is confirmed);
 * both accesses through `((u8 *) p)[0x87]`; unk_6E through a byte-pointer cast
 * so its store would not kill mem(0x87); `(u8)` cast on the product; the
 * if inverted so the body is the else arm; the two entry stores swapped (265
 * DIFF).
 *
 * Ruled out for the sign-extend, independently: `s16 r` with the comparisons
 * on r; `s32 r` with `(s16)` casts at both comparisons; `s32 r = (s16) call()`;
 * `u16 r` with casts; all four orderings of the r/i declarations. All give
 * `sra $v0,$v0,16` in place. In the `s32 r = (s16) call()` form the RTL does
 * carry two pseudos, so the two-register form is reachable in principle -- the
 * allocator just coalesces them onto $v0.
 *
 * Everything else was byte-exact first time and is worth not re-deriving: the
 * 0x200000 arm writes unk_1C BEFORE unk_14 where the 0x4000000 arm writes them
 * the other way round (that asymmetry is real); the five unk_14/unk_1C pairs
 * have an else that masks unk_14 while the first and last have none; the
 * `(u32) (p->unk_13 - 3) < 0x2B` range check compiles to addiu/sltiu directly;
 * and the final `p->unk_54 &= 0xFF7FFFFF` reuses the value already loaded for
 * the 0x800000 test.
 *
 * structs.h is already updated with what this function taught: Unk42710A's
 * middle byte is a real field (this function writes it), unk_29 is a field not
 * padding, and the record runs to at least +0x87. The forward declaration in
 * csrc/unk_14934.c should become `void func_80041E70(Unk42710 *);` when this
 * lands.
 *
 * UNIT RENAMED: this function was in unk_14934 until the D_800814E8 $gp
 * cluster was carved out of the middle of that unit; everything after the
 * carve became unk_154BC. Pass that as verify.py's UNIT argument.
 */

#define Unk42710  Unk42710_orig
#define Unk42710A Unk42710A_orig
#define func_80041E70 func_80041E70_decl_clash
#include "common.h"
#undef Unk42710
#undef Unk42710A
#undef func_80041E70

typedef struct Unk42710A {
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
} Unk42710A;

typedef struct Unk42710 {
    u8          unk_00;
    u8          unk_01;
    u8          unk_02;
    u8          unk_03;
    u8          unk_04;
    u8          unk_05;
    u16         unk_06;
    Unk42710A   unk_08[3];
    u8          unk_11;
    u8          unk_12;
    u8          unk_13;
    u32         unk_14;
    u32         unk_18;
    u32         unk_1C;
    u16         unk_20;
    u16         unk_22;
    u8          unk_24;
    u8          unk_25;
    u8          unk_26;
    u8          unk_27;
    u8          unk_28;
    u8          unk_29;
    u16         unk_2A;
    Unk42710B   unk_2C[4];
    u8          unk_34[0xD];
    u8          unk_41;
    u8          unk_42;
    u8          unk_43;
    u8          unk_44;
    u8          unk_45;
    u8          pad_46[0x2];
    u32         unk_48;
    u32         unk_4C;
    u8          pad_50[0x4];
    u32         unk_54;
    u8          pad_58[0xE];
    u8          unk_66;
    u8          pad_67[0x1];
    u8          unk_68;
    u8          unk_69;
    u8          pad_6A[0x4];
    u16         unk_6E;
    u8          pad_70[0x17];
    u8          unk_87;
} Unk42710;

s32 func_80042900(s8 *, s8);

void func_80041E70(Unk42710 *p) {
    s16 r;
    s32 i;

    p->unk_6E = 0x100;
    p->unk_87 = 0x10;
    if (p->unk_54 & 1) {
        p->unk_87 = p->unk_87 * 2;
        p->unk_6E = p->unk_6E * 2;
    }
    r = func_80042900((s8 *) p, 7);
    if (r > 0) {
        p->unk_6E = p->unk_6E * 2;
        p->unk_87 = p->unk_87 * 2;
    } else if (r < 0) {
        p->unk_6E = (s16) p->unk_6E >> 1;
        p->unk_87 = p->unk_87 >> 1;
    }
    if (p->unk_54 & 2) {
        s32 t = p->unk_05 * 2;

        if (t >= 0x100) {
            t = 0xFF;
        }
        p->unk_29 = t;
    } else {
        p->unk_29 = p->unk_05;
    }
    if (p->unk_54 & 4) {
        s32 t = p->unk_04 * 2;

        if (t >= 0x100) {
            t = 0xFF;
        }
        p->unk_66 = t;
    } else {
        p->unk_66 = p->unk_04;
    }
    if (p->unk_54 & 8) {
        s32 t = p->unk_00 * 2;

        if (t >= 0x100) {
            t = 0xFF;
        }
        p->unk_68 = t;
    } else {
        p->unk_68 = p->unk_00;
    }
    if (p->unk_54 & 0x10) {
        s32 t = p->unk_01 * 2;

        if (t >= 0x100) {
            t = 0xFF;
        }
        p->unk_69 = t;
    } else {
        p->unk_69 = p->unk_01;
    }
    for (i = 2; i >= 0; i--) {
        if (p->unk_08[i].unk_00 != 0) {
            if (p->unk_54 & 0x80) {
                s32 t = p->unk_08[i].unk_02 * 2;

                if (t >= 0x64) {
                    t = 0x63;
                }
                p->unk_08[i].unk_01 = t;
            } else {
                p->unk_08[i].unk_01 = p->unk_08[i].unk_02;
            }
        }
    }
    if (p->unk_54 & 0x4000000) {
        p->unk_14 |= 0x400;
        p->unk_1C &= ~0x400;
    }
    if (p->unk_54 & 0x2000000) {
        p->unk_14 |= 0x200;
        p->unk_1C &= ~0x200;
    } else {
        p->unk_14 &= ~0x200;
    }
    if (p->unk_54 & 0x8000000) {
        p->unk_14 |= 0x10;
        p->unk_1C &= ~0x10;
    } else {
        p->unk_14 &= ~0x10;
    }
    if (p->unk_54 & 0x10000000) {
        p->unk_14 |= 0x20;
        p->unk_1C &= ~0x20;
    } else {
        p->unk_14 &= ~0x20;
    }
    if (p->unk_54 & 0x20000000) {
        p->unk_14 |= 0x40;
        p->unk_1C &= ~0x40;
    } else {
        p->unk_14 &= ~0x40;
    }
    if (p->unk_54 & 0x40000000) {
        p->unk_14 |= 0x80;
        p->unk_1C &= ~0x80;
    } else {
        p->unk_14 &= ~0x80;
    }
    if (p->unk_54 & 0x200000) {
        p->unk_1C |= 0x1000;
        p->unk_14 &= ~0x1000;
    }
    if (p->unk_54 & 0x800000) {
        if ((u32) (p->unk_13 - 3) < 0x2B) {
            if (((u8 *) &p->unk_48)[0] != 4) {
                p->unk_54 &= 0xFF7FFFFF;
            }
        }
    }
}
