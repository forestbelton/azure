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
 *  (A) local-alloc. The third func_8003F688 comparison must land in $v1. Its
 *      RTL is `(set (reg 124) (le:SI (reg:SI 2 v0) (const_int 74)))`, a
 *      one-block quantity born where $v0 dies, so find_free_reg takes the first
 *      entry of REG_ALLOC_ORDER. For it to take $v1, something must occupy $v0
 *      across `[slti, beqz]` -- and nothing can, because that block contains
 *      the call.
 *  (B) reorg. The second comparison's branch must NOT steal
 *      `(set (reg/i:SI 2 v0) (const_int 0))` out of the shared return-0 block.
 *      All four branches to that block target one label with one `(set v0 0)`
 *      behind it; this build's `fill_eager_delay_slots` steals and retargets
 *      for all four, the target does it for three. `own_thread_p` is 0 for that
 *      thread in both builds, so copy-and-redirect is the only path either can
 *      take, and the three sites are identical in shape, target and condition
 *      register. The asymmetry was not found.
 *
 * **(A) and (B) are independent**, measured: writing the third comparison into
 * `saved` puts its `slti` in $s2, so $v0 IS free at that point, and branch 2
 * still steals -- the same two branch words wrong.
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
    q = &D_800814D3;
    for (;;) {
        if (CdSync(1, D_80081450) == 2) {
            break;
        }
        if (--i == 0) {
            func_8003E70C();
            if (func_8003F240() == 0x1B) {
                D_800814D3 = 0xFF;
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
