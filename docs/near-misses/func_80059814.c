/* func_80059814 (unk_2CDC0) -- 103 of 103 words, 0 DIFF. NOT compiled. MATCH.
 *
 * Advances the +0x10 accumulator by the +0x40 step at a rate set by the
 * current video mode in D_800869A8, then splits the result into a remainder
 * stored back at +0x40 and a quotient stored at +0x10. Ten case values in
 * six bodies.
 *
 * verify.py: 103 words, MATCH (was 94 of 103, 69 DIFF). Not integrated into
 * csrc/ -- this file is the matched source, waiting to be moved.
 *
 * WHAT CLOSED IT -- one shape, worth all nine words and all 25 register
 * differences at once:
 *
 *   each body stores the mask STRAIGHT INTO THE FIELD and then RE-READS the
 *   field for the shift, i.e.
 *
 *       *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 3;
 *       *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 2;
 *
 *   instead of the park's
 *
 *       m = *(u32 *)(p + 0x10) & 3;
 *       t = *(u32 *)(p + 0x10) >> 2;
 *       ... p->unk_40 = m; p->unk_10 = t;
 *
 *   The second read of +0x10 sits AFTER a store through p (the +0x40 store),
 *   and that is the whole lever -- see CANDIDATE LEVER below. Reverting to
 *   the local-variable form costs 9 words and 69-70 DIFF whether the two
 *   final stores are shared in a tail (94/69) or written per body (94/70).
 *
 * Everything else the old header listed as paid for still holds (ten labels
 * in six bodies, unsigned subject via `extern u32 D_800869A8[]` read as
 * `[0]`, two accumulate statements per body for the two `sw 0x10`). The old
 * header's diagnosis -- "ONE sched1 decision, potential_hazard inside a
 * priority group" -- was wrong: sched1 was behaving the same way in both
 * sources; it was the dependence graph that differed, because the matching
 * source has an extra load in it.
 *
 * MEASURED, all with verify.py (words / DIFF):
 *
 *   * field-direct `& 3` / `>> 2` per body, `break`           103 / 0 MATCH
 *   * same with `% 4` / `/ 4`                                  103 / 0 MATCH
 *   * `% 4` with `>> 2`, and `& 3` with `/ 4`             both 103 / 0 MATCH
 *     (the operator spelling is irrelevant; expand gives the same RTL)
 *   * same with `>>=` / `*=` / `+=` / `/=` compound forms     103 / 0 MATCH
 *   * same with an explicit `default: return;`                103 / 0 MATCH
 *   * same with the /3 body ending `return` instead of `break` 103 / 0 MATCH
 *   * same through a typed struct pointer (`p->acc`, `p->step`) 103 / 0 MATCH
 *   * locals m, t BUT m stored to the field BEFORE the field is
 *     re-read for t (`m = f & 3; f40 = m; t = f >> 2; f = t;`) 103 / 0 MATCH
 *   * the park's shared `store:` tail kept, with only t in it and the
 *     +0x40 store moved up into each body                     103 / 0 MATCH
 *     -- so neither the locals nor the shared tail were the defect; only
 *     the ORDER "store to +0x40, then read +0x10 again" matters.
 *   * the park as it was (locals m,t; shared tail)              94 / 69
 *   * locals m,t with the two stores written per body           94 / 70
 *   * explicit sum local x, stores per body, m/t from x          94 / 69
 *   * `t = x; x &= 3; t >>= 2;` with a shared tail               98 / 94
 *     (buys 4 of the copies by a different route, wrong shape)
 *   * The old header's "no locals at all, 105 words / 73 DIFF" could NOT
 *     be reproduced: every no-locals, per-body spelling tried today is
 *     103 / MATCH. Whatever that variant was, it was not this one.
 *
 * CANDIDATE LEVER -- "re-read the field after a store through the pointer"
 *
 * SYMPTOM in the target listing (what to grep an instruction stream for):
 *   a value is computed into $rA and stored (`sw $rA, OFF(base)`), and then,
 *   INSTEAD of the store sitting at the end of the block,
 *     (1) the store sits BEFORE the ALU ops that consume the value, and
 *     (2) the next insn is a register copy of it, `addu $rB, $rA, $zero`
 *         (`move $rB, $rA`), after which BOTH registers are modified in
 *         place (`andi $rA, $rA, k` / `srl $rB, $rB, n`).
 *   The copy is the fossil of a second `lw OFF(base)` of the field the
 *   compiler could not fold away. If your source holds the value in a local
 *   across an intervening store, you get `andi $rB, $rA, k` with no copy,
 *   the store sinks to the end of the block (sched1 has nothing after it to
 *   keep it in place), and if the block is a switch arm jump2 cross-jumps
 *   the sunk store into the shared tail: one word short per arm plus the
 *   missing copy per arm, and the arm's registers all shuffle.
 *
 * SHAPES. Minimal pair, measured in isolation (tools/decomp/dump.sh out.s):
 *
 *   DOES produce it (103 / MATCH here):
 *       p[4] = p[4] + p[16];
 *       p[16] = p[4] & 3;        <- store through p
 *       p[4] = p[4] >> 2;        <- field read AGAIN after that store
 *     -> addu $2,$2,$3 ; sw $2,16($4) ; move $3,$2 ; andi $2,$2,3 ;
 *        srl $3,$3,2 ; sw $2,64($4) ; sw $3,16($4)
 *
 *   Does NOT (94 / 69 here):
 *       x = p[4] + p[16];
 *       p[4] = x;
 *       p[16] = x & 3;
 *       p[4] = x >> 2;
 *     -> addu $2,$2,$3 ; andi $3,$2,3 ; sw $2,16($4) ; srl $2,$2,2 ;
 *        sw $3,64($4) ; sw $2,16($4)
 *
 *   Also does NOT: re-reading the field with NO store in between
 *   (`p[4] = ...; p[16] = p[4] & 3;` is folded to the register by cse, as
 *   the first use in each body here is).
 *
 *   The intervening store need not be through the same pointer: a store
 *   through another pointer (`q[0] = 5`) and a store to a fixed-address
 *   global (`D_800869A8[0] = 5`) both make the re-read survive as a real
 *   `lw` (probed in the cse dump and out.s). In those cases the re-read
 *   stays a load rather than a copy, because sched1 cannot hoist it over a
 *   possibly-aliasing store to sit beside the store of its own address.
 *
 * MECHANISM (gcc 2.7.2 source in scratch/gcc-src/gcc-2.7.2.3):
 *   1. cse.c `note_mem_written`: a store whose address varies (any base
 *      that is a pseudo, i.e. any pointer) sets `writes_ptr->var = 1`
 *      (and `nonscalar`, not `all`, because the address is a PLUS).
 *      cse.c `invalidate_memory` then removes EVERY hash-table entry with
 *      `in_memory` whose address `cse_rtx_addr_varies_p` -- it does not
 *      look at offsets at all (the offset-aware `refers_to_mem_p` path in
 *      `invalidate` is only used for the stored MEM's own entry). So after
 *      any pointer store, cse has forgotten every pointer-based load, and
 *      the C-level re-read is emitted as a real `(set rN (mem))`. Seen in
 *      the .cse dump: insn 35 `and` already uses the summed register, but
 *      insn 40, after the +0x40 store, is still `(set r91 (mem +0x10))`.
 *   2. sched.c (sched1): the reload is a real load with a true memory
 *      dependence on the earlier `sw +0x10` and a consumer (`srl`) after
 *      it, so the store can no longer sink to the end of the block; and
 *      `memrefs_conflict_p` knows +0x10 and +0x40 are disjoint, so the
 *      load is hoisted above the +0x40 store to sit right after the +0x10
 *      store (trace: 31 then 40 then 35 37 41 43).
 *   3. Between the .lreg and .greg dumps the load is rewritten IN PLACE
 *      (same UID 40, same LOG_LINKS, REG_EQUIV note kept) from
 *      `(set v1 (mem +0x10))` to `(set v1 v0)`, v0 being the register the
 *      adjacent store wrote. FSF 2.7.2.3's reload has no path that touches
 *      an insn needing no reload (movsi_internal2 alternative `d <- R`
 *      wins with zero losers; find_equiv_reg's callers all require a
 *      reload), so this is a post-reload hard-register CSE of the gcc 2.8
 *      `reload_cse_regs`/`reload_cse_simplify_set` shape, which this
 *      cygnus-2.7.2-970404 binary plausibly carries alongside its
 *      backported `regmove` pass (the .regmove dump exists; 2.7.2 has
 *      none). Not confirmed by reading source: the binary is all we have.
 *   4. jump2 then has nothing to cross-jump but the two final stores,
 *      which is the listing's two-insn tail.
 *
 * TO APPLY ELSEWHERE: when a target shows `sw $rA; addu $rB,$rA,$zero` with
 * the store ahead of the arithmetic, write the source so the field is read
 * again after a store through the pointer, not held in a local. Conversely,
 * if the target has the store at the END of a block with no copy, keep the
 * value in a local (or do the re-read before any store).
 */

#include "common.h"

extern u32 D_800869A8[];

void func_80059814(u8 *p)
{
    switch (D_800869A8[0])
    {
    case 0x30:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) * 10;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 3;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 2;
        break;
    case 0x60:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) * 5;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 3;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 2;
        break;
    case 0xC0:
    case 0xF0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 1;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 1;
        break;
    case 0x120:
    case 0x168:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) / 3;
        break;
    case 0x180:
    case 0x1E0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 3;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 2;
        break;
    case 0x300:
    case 0x3C0:
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) + *(u32 *)(p + 0x40);
        *(u32 *)(p + 0x40) = *(u32 *)(p + 0x10) & 7;
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x10) >> 3;
        break;
    }
}
