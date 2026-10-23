/* func_8003F368 (unk_12A70) -- 113 of 113 words, 4 DIFF. NOT compiled.
 *
 * Waits for the drive to settle, retrying CdControl(0x10) and CdSync sixteen
 * times each; on error 0x1B -- media changed -- it marks D_800814D3, rewinds
 * the ring-queue read index and starts the whole body over; then validates the
 * BCD minute/second/sector and returns the sector position.
 *
 * NOTE ON ITS DECLARATIONS: the structs.h changes it needs are ALREADY IN the
 * tree, applied when func_8003F2A4 landed -- `extern u8 D_80081450[4]` with
 * separate `D_80081451`/`D_80081452` bytes in place of the old Unk80081450
 * typedef, and the unit's file-scope size record. So this source should compile
 * and verify as-is; the `#define`/`#undef` shims and local externs it may still
 * carry are now redundant.
 *
 * WHAT IS LEFT is 4 words in two independent two-word gaps, both in the tail's
 * BCD validation chain:
 *
 * ONE UNEXPLORED LEVER, added 2026-10-23 from reading local-alloc.c and NOT
 * measured. `block_alloc` allocates in two phases: every quantity carrying a
 * SUGGESTION (`qty_phys_copy_sugg`, else `qty_phys_sugg`) gets its suggested
 * register first, jumping the queue, and only the unsuggested remainder is
 * sorted by decreasing life and given the first register `find_free_reg`
 * finds. Gap (A) below assumes the second path. This function has 12 calls, so
 * hard-register copies -- the thing `combine_regs` turns into suggestions --
 * do exist here, unlike in func_80047054 where there are none. Whether a
 * suggestion can be put on THIS quantity is the open question; $v1 is hard to
 * name from C, so check what the .lreg dump already shows for the quantities
 * in that block before spending runs. See matching-levers section 3.
 *
 *  (A) local-alloc. The third func_8003F688 comparison must land in $v1. Its
 *      RTL is `(set (reg 124) (le:SI (reg:SI 2 v0) (const_int 74)))`, a
 *      one-block quantity born where $v0 dies, so find_free_reg takes the first
 *      entry of REG_ALLOC_ORDER. For it to take $v1, something must occupy $v0
 *      across `[slti, beqz]` -- and nothing can, because that block contains
 *      the call.
 *  (B) reorg, and the exact pass check is now named. The second comparison's
 *      branch must NOT fill its delay slot from the shared return-0 block,
 *      where the other three do. Two facts narrow it to one test.
 *
 *      The target's test-2 branch is NOT redirected -- it reads
 *      `beqz $v0, .L8003F50C` (the shared block) where the other three read
 *      `.L8003F510`. In reorg, `new_thread` starts equal to `thread` and
 *      advances only when a trial is accepted or deleted as redundant, and the
 *      closing `reorg_redirect_jump` runs only `if (new_thread != thread)`. So
 *      the first trial was REJECTED OUTRIGHT -- not consumed, and not found
 *      redundant by `redundant_insn`, which would have redirected without
 *      filling and given `.L8003F510` plus a nop. And on a first trial `set`
 *      and `needs` are still empty and MIPS slots are not annulled, so the
 *      condition register is unprotected -- test 1 is `beqz $v0` with
 *      `set $v0,0` in its slot and is accepted. That leaves exactly one test
 *      that can reject it:
 *
 *          ! insn_sets_resource_p (trial, &opposite_needs, 1)
 *
 *      with `opposite_needs = mark_target_live_regs (fallthrough_insn)`. So the
 *      original had $v0 REPORTED LIVE AT THE HEAD OF THE THIRD COMPARISON'S
 *      BLOCK -- test 2's fall-through -- and nowhere else. No dataflow can do
 *      that: that block's first real event is `jal func_8003F688`, which
 *      clobbers $v0. The only route is `mark_target_live_regs`'s conservative
 *      fallback -- `find_basic_block` scans back to the previous BARRIER then
 *      forward over CODE_LABELs for a `basic_block_head`, returns -1 on
 *      failure, and every register is then marked live, so any trial that
 *      writes one is rejected. **So gap (B) is a question about block-entry
 *      layout in the tail, and producing it from C costs a word.** Three
 *      zero-word ways to put a label at that block's head were probed -- a
 *      `goto third;` above its own label, a `for (;;) { ... break; }` wrapper,
 *      and inverting test 2 so the block is a branch target rather than a
 *      fall-through -- and all three are 113 words / 4 DIFF, because jump2
 *      deletes the redundant jump and the label's last use goes with it.
 *
 * **(A) and (B) are independent**, measured: writing the third comparison into
 * `saved` puts its `slti` in $s2, so $v0 IS free at that point, and branch 2
 * still steals -- the same two branch words wrong.
 *
 * THE `own_thread_p` LEVER POINTS THE WRONG WAY HERE, and the target proves it,
 * so do not try it: giving the shared `(set v0 0)` its own thread would make
 * reorg STEAL AND DELETE it, and the target KEEPS it -- `addu $v0,$zero,$zero`
 * survives at 0x8003F50C, physically last before the epilogue, with three
 * further copies of it in delay slots. **A surviving original plus copies
 * elsewhere is the signature of an UN-OWNED thread**, which is the state this
 * build is already in. Measured: the func_80042900 shape, with success falling
 * off the end and failure owning the `return 0`, gives 109 words and 17 DIFF.
 *
 * And no measured shape keeps the barrier `own_thread_p` needs anyway. The final
 * conditional always falls through into the return-0 block, because
 * `set v0, pos` is harmless on both paths and gets hoisted above the `pos > 0`
 * branch; the branch is then inverted to jump straight to the epilogue, the
 * explicit `j epilogue` after `return pos` becomes a jump-to-next and is
 * deleted, and its barrier goes with it. Confirmed in `.sched2`: the
 * `goto zero;` form with an explicit jump on the success path collapses to a
 * layout identical to the base's. The one shape that kept a barrier -- the
 * return-0 block first with success behind `ok:` -- puts it AFTER the block,
 * which is no use, since `own_thread_p` scans backwards.
 *
 * EXIT-BLOCK SHAPES MEASURED, 11 variants, on top of the 23 condition-chain
 * ones above (34 spellings now, all one RTL shape): three `goto zero;` forms,
 * the return-0 block first with success behind a label, and two-return-0-block
 * forms with test 1 or test 2 returning directly -- all 113 words / 4 DIFF.
 * Failure falling off the end with no `(set v0 0)` anywhere: 113 / 8, and that
 * one is EVIDENCE -- all four delay slots then come out `nop`, which proves the
 * target's three filled slots are copies of that one block's insn, and that the
 * fourth really is one of four identical opportunities being declined.
 *
 * RULED OUT for the last 4 words, 23 compiled variants, all 113 words / 4 DIFF
 * unless noted. Everything in the condition chain canonicalises to one RTL
 * shape, so none of these moved it: a flat `&&` chain; four
 * `if (x >= k) return 0;` early returns; a `goto fail` chain;
 * `do { ... break; } while (0)`; two-, three- and four-deep nesting; an extra
 * `return 0` reachable only from test 2; `!(x >= 0x4B)`; a ternary tail (7);
 * `r = 0; if (...) r = pos; return r;` (114 words, 42); the if/else form of
 * that (7); the bound 0x4B hoisted into a variable; `pos` and `i` as one
 * variable; the third call's result assigned to `i` (7) or to `saved` (4, with
 * `slti $s2`); `volatile s32 pos` (116, 38); the loc/res declaration order
 * swapped (9); a dummy `loc[3] = 0` (114, 36). search.py's own sweep, 17
 * candidates over 8 families, found nothing.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - the two size records (122 words, 115 DIFF).
 *  - D_80081451/D_80081452 as their own byte symbols rather than
 *    D_80081450[1]/[2] (109 words, 39 DIFF), and D_800814D3 likewise rather
 *    than D_800814D0[3] (114 words, 69 DIFF). This kills the standing
 *    "+0/+1/+2/+3 through one materialised base" hypothesis for a second
 *    function, and for a second reason: these absolute accesses are gas's
 *    bare-symbol macro expansions (`lui $at` + `%lo`), not one base with
 *    displacements, so the array spelling gives the wrong OPCODES before
 *    maspsx's rewrite even enters into it.
 *  - the `q` pointer local, assigned in the INNER LOOP'S PREHEADER. Removing it
 *    and reaching D_800814D0 by symbol throughout costs 7 words (106);
 *    assigning it inside the 0x1B handler alongside its uses keeps the word
 *    count but lands it in caller-saved $a0 and materialises it in the wrong
 *    block (22 DIFF).
 *  - `restart:` plus `goto restart` for the outer retry, NOT a `for (;;)`
 *    wrapper: 115 words, 112 DIFF. See the matching-levers entry -- a backward
 *    goto is not a loop to loop.c, and that is what keeps `li 1` out of the
 *    outer preheader and the function inside three callee-saved registers.
 *
 * NOTE ON THE SPELLING: D_800814D3 is now declared `extern u8 D_800814D3[1]`
 * in include/structs.h -- func_8003E758 needs the ARRAY_REF to be
 * MEM_IN_STRUCT_P -- so this source reaches it as `D_800814D3[0]` and
 * `&D_800814D3[0]`. Re-verified at 113 of 113 / 4 DIFF after that change.
 *
 * nmcheck: 113 of 113 words, 4 DIFF
 */
#include "common.h"

void func_8003E70C(void);
void func_8003F624(void);
s32 func_8003F688(s32);
u8 func_8003F240(void);

__asm__(".sdata\n\t.size D_80081450,4\n\t.size D_800814D0,1\n\t.text");

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
    pos = CdPosToInt(loc);
    if (func_8003F688(D_80081450[0]) < 0x4B && func_8003F688(D_80081451) < 0x3C &&
        func_8003F688(D_80081452) < 0x4B && pos > 0) {
        return pos;
    }
    return 0;
}
