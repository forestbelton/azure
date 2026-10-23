/* func_80041588 (unk_14934) -- 164 of 164 words, 21 DIFF. Every opcode,
 * immediate, offset, delay slot and branch target matches; the 21 differences
 * are one coupled register permutation. NOT compiled, and NOT landable on its
 * own: it dispatches through jtbl_8002D658, so it needs the splat/linker change
 * in docs/open-matching-problems.md, and all three unk_14934 switch functions
 * have to land together. verify.py also masks the two relocated halfwords that
 * reference the table, so 164/164 here is necessary and not sufficient.
 *
 * It walks a command stream asynchronously: for each record, either DMA a
 * texture, patch palette bits, memcpy a payload, or relocate a pointer table,
 * then advance by the record's own byte size until a zero size ends it.
 *
 * CASE ORDER RECOVERED (table words at file offset 0xE58 of
 * data/slus_006.14, little-endian, 9 entries, index = op - 1):
 *
 *   8004164c 80041688 8004177c 80041794 80041670 800416a0 800416bc 80041700 800417b0
 *
 * In address order the arms are 0x164C(1), 0x1670(5), 0x1688(2), 0x16A0(6),
 * 0x16BC(7), 0x1700(8), 0x177C(3), 0x1794(4), 0x17B0(9), so the SOURCE ORDER IS
 * 1, 5, 2, 6, 7, 8, 3, 4, 9 -- byte-for-byte the same order func_80041344
 * needs, which is more evidence the two are siblings over one format.
 *
 * LOAD-BEARING, each measured by reverting it:
 *
 *  1. `flags` is a function-scope `s32`, loaded and then OR'd IN PLACE
 *     (`flags = *(u16 *) (p + 0xC); flags |= 2;`). This was the biggest find in
 *     the function, and it is a general lever: with more than one set, combine
 *     can no longer reduce the `zero_extend` to a `subreg` nor substitute the
 *     `ior`, so the whole load/OR chain becomes ONE GLOBAL ALLOCNO -- and
 *     sched1 then orders the arm the way the target does. An arm-local inline
 *     expression costs 163 words / 102 DIFF (case 6 loses its own `ori`,
 *     because jump2 then cross-jumps one insn deeper and case 6 jumps to the
 *     `ori` instead of the `sll`, and the `lhu 0xC` is scheduled third instead
 *     of first in both case 6 and the shared tail). Writing the OR as one
 *     statement still costs 3 DIFF.
 *  2. `n`, `q` and `w` at function scope, one variable each reassigned in
 *     several arms. Arm-locals cost 22 DIFF.
 *  3. ONE counter `i` shared by case 7, case 8's OUTER loop and case 9, and a
 *     second `j` for case 8's inner loop. Per-arm locals cost 10 DIFF. All 54
 *     assignments of the four counter sites to 2 or 3 shared variables were
 *     swept; (A, A, B, A) is the unique best and is what the target shows --
 *     $a3 for case 7's counter, case 8's `j` and case 9's counter, $a2 for case
 *     8's inner counter.
 *  4. `base` has TWO sets: the masked value, then the KSEG0 pointer copied
 *     back. Folding them into one expression costs 3 DIFF, because the
 *     target's `and $s1,$v0,$v1` puts the masked value in a callee-saved
 *     register, which only happens if it is `base`'s own pseudo -- a
 *     block-local temp always gets $v0.
 *  5. Case 8's SINGLE walking pointer, `w = q;` with `w++` as the outer body's
 *     first statement and `*w |= 0x8000; w++;` in the inner body. That
 *     reproduces the arm's 19 words exactly, including the
 *     `addu $a1,$a0,$zero` / `addiu $a1,$a1,2` pair: the copy is the biv init
 *     emitted before the loop guard and the `+2` is loop.c's giv adjustment in
 *     the preheader, created after cse, so cse cannot propagate the copy away.
 *     `q[j][i]` costs 165 words / 81 DIFF; `u16 *w = q + 1;` with `w++` last
 *     gives 162 words (the init folds to one `addiu`); `w[1] |= 0x8000; w++;`
 *     gives 164 words but two induction registers.
 *  6. `dst = D_80081480 = D_8008148C;` as a CHAINED store. Two statements give
 *     163 words / 132 DIFF -- the same store_expr mechanism commented on
 *     func_80041284 in csrc/unk_14934.c.
 *  7. A separate `Unk41344 *e` inside the loop body. Using
 *     `((Unk41344 *) p)->unk_04` gives 163 words / 123 DIFF, losing the
 *     `addu $t0,$s0,$zero` copy.
 *  8. Case 9 DOES advance its pointer, unlike func_80041344's case 9, which
 *     does not (the sibling's note calls that a bug in the original). The
 *     explicit `| 0x80000000` on an already-KSEG0 `base` is real: it is the
 *     target's `lui $v0,0x8000` / `or $a0,$s1,$v0` hoisted into the preheader,
 *     and cse will not fold it because the cheapest equivalent of `base` is
 *     the register itself.
 *  9. Case 9's `blez` guard falls out of `for (i = *(u16 *) (p + 8); i > 0;
 *     i--)` for the documented reason -- two sets of `i` defeat combine's
 *     `nonzero_bits`. Case 8's entry guard is `beqz` because its `n` has a
 *     single set from the `lhu`.
 * 10. `default: return;`. The out-of-range path and the `size == 0` exit both
 *     go straight to the epilogue and nothing follows the loop, so `break` and
 *     `return` are interchangeable here -- unlike func_80041344, which has a
 *     `D_80080A8C = 0` tail.
 * 11. func_8003F80C's fourth argument must be `s16`: case 2 passes
 *     `*(s16 *) (p + 0xC)` and gets a bare `lh` (the u16-field/(s16)-cast
 *     fold), cases 6/7/8 pass `(s16) flags` and get `sll 16`/`sra 16`.
 *
 * WHAT IS LEFT is one coupled permutation, with no instruction, immediate or
 * ordering difference:
 *
 *   value                    mine   target   words
 *   switch index (op - 1)    $a0    $v1      3   (addiu, sltiu, sll)
 *   e (record pointer)       $v1    $t0      13  (the addu copy + 12 lw)
 *   flags                    $a3    $v1      5   (2x lhu, 2x ori, 1x sll)
 *
 * From the dumps, with global.c's floor_log2(refs) * refs / live_length, all
 * post-sched1: `e` 26 refs (13 at loop depth 2) / 29 insns = 3.586; the switch
 * index 6 / 4 = 3.0; `flags` 24 / 17 = 5.65 with a hard-reg PREFERENCE for $a3;
 * the shared counter `i` 39 / 45 = 4.33, which takes $a3. `e`'s hard-reg
 * conflicts are {$v0, $a0, $a1, $a2} and $a3 is additionally blocked by
 * flags/i, so find_reg hands it $v1 and the index, allocated two places later,
 * falls to $a0.
 *
 * For `e` to land on $t0, both $v1 and $a3 must be blocked at its turn. In the
 * target $a3 is blocked by the counter and $v1 BY `flags` -- which genuinely
 * conflicts with `e` (in case 6 the `lhu 0xC` is the arm's first insn and `e`
 * dies at its fourth) -- and the sign extension there is a separate $a3 pseudo
 * (`sll $a3,$v1,16`). Here `flags` takes $a3 itself, because the copy into the
 * fourth argument register gives it a preference for $a3 and the `sll`/`sra`
 * pseudos coalesce with it. SO THE ONE REMAINING LEVER is to stop `flags`
 * taking $a3 -- and of the two ways named here, **the first is now refuted and
 * the second is the one to use.** Breaking the argument-register preference is
 * not reachable: measured on func_80041344, such a preference propagates along
 * the dataflow chain including through arithmetic insns, and splitting the
 * chain through an intermediate variable leaves BOTH allocnos carrying it,
 * while a separate `s32 f = (s16) x;` is eliminated by combine outright.
 * Getting a conflicting higher-priority allocno in first is the live route,
 * because `regs_someone_prefers` collects only the preferences of
 * LOWER-priority conflicting allocnos -- so allocation order decides whose
 * preference wins, and order is reachable through the section 3 levers.
 * func_80041344 went from 24 differing words to 2 on exactly that comparison.
 * Also worth trying here, from the same work: making the switch index a real
 * source variable by writing `op = *(u16 *) p - 1;` with the arms renumbered
 * from `case 0`, so expand_end_case skips its own MINUS and the index carries
 * the flag word's references and conflicts. Ruled out, all still 21 DIFF unless
 * noted: `flags` as s16 or u16 (163 words), as u32, a separate
 * `s32 farg = (s16) flags;` or `s16 farg = flags;`, a flag variable used only
 * by case 6, the OR inside the argument expression (163 words), and passing
 * `flags` with no cast.
 *
 * THE SWITCH INDEX CAN NEVER BE ALLOCATED BEFORE `e`, and this is now numeric
 * rather than assumed. It also CORRECTS func_80041344's note, which has the
 * index as block-local and assigned by local-alloc: it is not. `sltiu`/`beqz`
 * end its block and the `sll` is in the next one, so it is a global allocno.
 * Its 3 references at loop depth 2 and its live length of 4 are both floors
 * forced by the target's own code, giving 3.0; `e`'s 13 references are forced
 * by the target's 12 `lw ...($t0)` plus the copy, and lowering 3.586 below 3.0
 * would need a live length of 35 or more.
 *
 * Also ruled out, all at exactly 21 DIFF, so do not retry: `switch (e->unk_00)`;
 * the opcode in a u16 or s32 local; `e` hoisted above the loop or at function
 * scope; `e` as `u8 *` with `*(u32 *) (e + 4)` (MEM_IN_STRUCT_P off); the
 * halfword accesses through a struct cast (MEM_IN_STRUCT_P on); `dst` as
 * `u8 *`; `r` at function scope; declaration-order swaps of p/base;
 * `0x80000000 | (u32) base`; and every spelling of the func_8003F80C argument
 * list -- all four arguments hoisted into locals in forward or reverse order,
 * two of them, one of them. STATEMENT ORDER DOES NOT MOVE SCHED1 INSIDE THESE
 * BLOCKS AT ALL; what moved it was the number of SETS of `flags`, i.e. the
 * dependency graph, not the order.
 *
 * Inert, worth knowing: the parameter type does not matter here. `u32 *arg0`
 * with `*arg0 & 0x7FFFFF` gives the same 21 DIFF as `Unk41284 *arg0`, because
 * the load sits before the DrawSync call and the D_80081480 store after it, so
 * no sched1 alias question arises -- unlike func_80041284, where the struct
 * pointer was worth four words. `Unk41284 *` is kept for consistency with the
 * caller.
 *
 * include/structs.h, when this lands: `Unk41344` as written below fits this
 * function exactly -- same five fields, same dual word/halfword reading, same
 * reason for two pointers to one record -- so it should be added once and used
 * by both functions. Also needed: `void *memcpy(void *, const void *, u32);`
 * beside bzero; `void func_8003E140(s8 *);` (defined in csrc/unk_11940.c and
 * declared nowhere -- this function takes its address); and
 * `void func_8003F80C(u32, s32, s32, s16);`, which currently has only a local
 * prototype in csrc/unk_12DE0.c. Nothing calls func_80041588 from C yet, so
 * its signature comes from the listing: arg0 is the Unk41284 * whose word is
 * masked and is passed on to func_8003E4FC, arg1 is an s8 * completion flag
 * (`*arg1 = 0`, then registered with func_8003E140, which is
 * `void func_8003E140(s8 *a) { *a = 1; }`), and arg2 is only tested against
 * zero and selects the asynchronous path.
 *
 * SEARCHER SWEEP (2026-10-22): tools/decomp/search.py found 54 candidate
 * changes inside this function's body and compiled 55 variants; none improved
 * on 21 DIFF. Read that as "the eight transform families in search.py found
 * nothing at the sites they recognise" -- NOT as "every source shape has been
 * tried", which is a much stronger claim than 55 variants can support. The
 * families are declared widths, adding and dropping an (s16) cast, splitting
 * and joining a statement, `<` against `>`, the `& 1` mask, and duplicating
 * the statement after an if/else into both arms.
 */

#include "common.h"

typedef struct Unk41344 {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
} Unk41344;

void *memcpy(void *, const void *, u32);
void func_8003E140(s8 *);
void func_8003F80C(u32, s32, s32, s16);

void func_80041588(Unk41284 *arg0, s8 *arg1, s32 arg2) {
    u8 *p;
    u8 *base;
    u32 dst;
    u16 size;
    s32 flags;
    s32 n;
    s32 i;
    s32 j;
    u16 *q;
    u16 *w;

    if (arg2 == 0) {
        func_8003E4FC(6, (u8 *) arg0, 0);
        *arg1 = 0;
        func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) arg1);
        return;
    }

    base = (u8 *) (arg0->unk_00 & 0x7FFFFF);
    p = (u8 *) ((u32) base | 0x80000000);
    base = p;
    DrawSync(0);
    dst = D_80081480 = D_8008148C;

    for (;;) {
        Unk41344 *e = (Unk41344 *) p;

        switch (*(u16 *) p) {
        case 1:
            func_8004068C(base + e->unk_04, (u8 *) dst);
            LoadImage(p + 8, (u32 *) dst);
            DrawSync(0);
            break;
        case 5:
            LoadImage(p + 8, (u32 *) (base + e->unk_04));
            DrawSync(0);
            break;
        case 2:
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), *(s16 *) (p + 0xC));
            DrawSync(0);
            break;
        case 6:
            flags = *(u16 *) (p + 0xC);
            flags |= 2;
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), (s16) flags);
            DrawSync(0);
            break;
        case 7: {
            q = (u16 *) (base + e->unk_04);
            n = *(u16 *) (p + 0xA) << 4;
            for (i = 1; i < n; i++) {
                q[i] |= 0x8000;
            }
            flags = *(u16 *) (p + 0xC);
            flags |= 2;
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) flags);
            DrawSync(0);
            break;
        }
        case 8: {
            q = (u16 *) (base + e->unk_04);
            w = q;
            n = *(u16 *) (p + 0xA);
            for (i = 0; i < n; i++) {
                w++;
                for (j = 1; j < 16; j++) {
                    *w |= 0x8000;
                    w++;
                }
            }
            flags = *(u16 *) (p + 0xC);
            flags |= 2;
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) flags);
            DrawSync(0);
            break;
        }
        case 3:
            func_8004068C(base + e->unk_04, (u8 *) e->unk_08);
            break;
        case 4:
            memcpy((void *) e->unk_08, base + e->unk_04, e->unk_0C);
            break;
        case 9: {
            u32 *r = (u32 *) (base + e->unk_04);

            for (i = *(u16 *) (p + 8); i > 0; i--) {
                r[1] = r[1] + ((u32) base | 0x80000000);
                r += 2;
            }
            break;
        }
        default:
            return;
        }
        size = *(u16 *) (p + 2);
        if (size == 0) {
            break;
        }
        p += size;
    }
}
