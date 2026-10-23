/* func_80041E70 (unk_154BC) -- 270 of 270 words, 1 DIFF. NOT compiled.
 * Was 4 DIFF; three of the four were fixed at zero cost and the fourth is a
 * wall for a reason I believe is complete.
 *
 * Applies the entity's modifier flags at unk_54 to its derived stats: doubles
 * or halves the two rate fields by the sign of func_80042900's result, clamps
 * four doubled byte stats at 0xFF and three more at 0x63, then sets and clears
 * a dozen bits across unk_14 and unk_1C.
 *
 * THE THREE WORDS THAT ARE FIXED -- `sra $v1,$v0,16`, `blez $v1`, `bgez $v1` --
 * were a `global-alloc` preference with no rival to beat it. The sra's dest
 * allocno had `preferences: 2` (from `set_preference` stripping one 'e' level
 * off `(set 94 (ashiftrt (reg 95) 16))`, where reg 95 was already coalesced
 * onto hard $v0 as the call's return value) and conflicted with nothing
 * caller-saved, so `regs_someone_prefers` was empty and `find_reg` simply
 * handed it the $v0 it preferred.
 *
 * What fixed it is the CONFLICT side rather than the reference count: `r` now
 * holds BOTH the doubled unk_87 value computed in the `unk_54 & 1` arm and the
 * sign-extended call result. The arm emits the identical two insns (cse still
 * folds the product to `li $v1,32`), but the allocno gains a second live range
 * there, where a local-allocated quantity already holds $v0 -- so the union of
 * the two ranges' conflicts contains hard reg 2, find_reg skips $v0 for the
 * whole allocno, and both ranges land in $v1. **That is why the target holds
 * the unk_87 value and the call result in one register: they are one
 * variable.** Zero words. See docs/matching-levers.md section 3.
 *
 * Measured on the way, all at 270 words: the `s16` return declaration on
 * func_80042900, a separate `raw` local, `s32 r` with one cast, the call result
 * into the loop counter's variable (dead in between, so no conflict is
 * created), no `r` at all with `(s16) i` at both tests, and the cast split into
 * two statements -- every one of them still 4 DIFF. `r` also holding the 0x10
 * is 11 DIFF and lands on $a0, for a SCHEDULE reason rather than an allocation
 * one: making the 0x10 its own statement lets sched1 hoist its `li` above the
 * `lw 0x54`, both block-0 quantities then need $v0 and $v1, and the union
 * conflicts with both. `r` reused as the first clamp temp is 231 DIFF and as
 * the loop counter 252.
 *
 * THE REMAINING WORD -- `li $v1,32` where the target has `sll $v1,$v1,1` -- is
 * a WALL, owned by cse. mem(0x87)'s equivalence class contains both the pseudo
 * holding 16 and `(const_int 16)`; `fold_rtx` substitutes the cheapest class
 * member, and `rtx_cost` of a CONST_INT is ~0 against `COSTS_N_INSNS(1)` for
 * `(ashift reg 1)`, so the constant always wins and the insn becomes
 * `(set q (const_int 32))`. Keeping the register needs the class NOT to contain
 * the constant, which needs the `li 16` to lie in a different cse run from the
 * body -- and MIPS cannot store an immediate, so expand always emits
 * `(set reg (const_int 16))` ahead of the `sb`. The constant is unavoidably in
 * RTL and a run boundary is the only escape. Four run-boundary probes were
 * measured and all three ways of buying a two-use label are blocked
 * (thread_jumps in jump1 runs before cse; jump1's dead-set DCE; cse deleting a
 * redundant duplicate store), and the one free boundary -- identical if/else
 * arms -- costs 1 to 2 words in that block, i.e. more than the word it saves.
 *
 * DECLARATIONS: none. include/structs.h's Unk42710/Unk42710A already carry
 * every field, verified by compiling against the header. The source needs only
 * `#include "common.h"` plus `s32 func_80042900(s8 *, s8);`, and when it lands
 * csrc/unk_14934.c's forward declaration should become
 * `void func_80041E70(Unk42710 *);`.
 *
 * nmcheck: 270 of 270 words, 1 DIFF
 */
#include "common.h"

s32 func_80042900(s8 *, s8);

void func_80041E70(Unk42710 *p) {
    s32 r;
    s32 i;

    p->unk_6E = 0x100;
    p->unk_87 = 0x10;
    if (p->unk_54 & 1) {
        r = p->unk_87 * 2;
        p->unk_87 = r;
        p->unk_6E = p->unk_6E * 2;
    }
    r = (s16) func_80042900((s8 *) p, 7);
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
