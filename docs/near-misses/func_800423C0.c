/* func_800423C0 (unk_14934) -- 72 of 72 words. Every INSTRUCTION now matches;
 * what is left is one register pair swapped. Not compiled; see
 * docs/open-matching-problems.md.
 *
 * SOLVED 2026-10-22: the question this was parked on -- "how do the three
 * 0x8001xxxx constants get into registers", where gcc shared one `lui 0x8001`
 * that the target materialises three times -- is answered, and the answer is
 * two statement-level facts:
 *
 *  1. The base pointer is built by an IN-PLACE update, `p = (u8 *) 0x80010000;`
 *     then `p += r * 2;`, not `p = (u8 *) 0x80010000 + r * 2;`. In place, the
 *     add's destination ties to the constant's own pseudo and consumes it, so
 *     nothing holds 0x80010000 afterwards and the 0x80012096 arm needs a fresh
 *     `lui`. Written as one expression the constant survives in its own
 *     register and cse reuses it with a `move`. Same "update in place to block
 *     a reuse" mechanism as func_8003FE78's `flags |= 0x80`.
 *  2. `cmp` must be declared and initialised BEFORE `p`. Otherwise p's `lui`
 *     comes first and cse shares it with the 0x80012094 constant instead --
 *     the sharing just moves. With `cmp` first, its lui/ori consumes its own
 *     register, p's lui is fresh, the in-place add consumes it, and the arm's
 *     lui is fresh: three independent `lui 0x8001`s in the target's order.
 *  3. `p += r * 2;` must come BEFORE the `if (r == 0)`, not after. After it,
 *     the sharing comes back as a `move` (8 DIFF).
 *
 * Measured: this source 12 differing words, all register fields; the one
 * expression form 4 differing words but with the wrong instructions; `cmp`
 * declared second 14; `p += ` after the if 8. Also inert, all 4: writing the
 * pointer arithmetic as `(u8 *) (r * 2 + 0x80010000)`, via an `off` local, or
 * as `&((u8 *) 0x80010000)[r * 2]`.
 *
 * WHAT IS LEFT is a two-register swap: `r` wants $a0 and the base pointer $v1,
 * and this source has them the other way round. From the greg/lreg dumps the
 * allocation order is `95 113 82 114 80 112` with `r` (allocno 95, 4 refs / 8
 * insns, priority 2*4/8 = 1.00) just ahead of the base pointer (allocno 113,
 * 7 refs / 15 insns, 2*7/15 = 0.93). A 7% gap -- the closest of any of the
 * parked allocation cases. It would flip if the base pointer reached 8
 * references (crossing the floor_log2 boundary takes it to 1.6) or if its live
 * length dropped to 13, or if `r`'s rose to 9. All three numbers look fixed by
 * the target's own code, but the margin is small enough to be worth one more
 * look before giving up. See docs/matching-levers.md section 3.
 *
 * Settled earlier, and not to be re-derived:
 *
 * - The indexed base is the plain constant 0x80010000 with 0x2094/0x2098 as
 *   load displacements, NOT the symbol D_80012094. splat renders a bare
 *   `lui reg, 0x8001` as `%hi(D_80012094)` whenever it can pair the register
 *   with a later `%lo`, which hides the difference. Measured: a symbol array
 *   with a variable index always materialises the whole address with
 *   `lui %hi` + `addiu %lo` and then indexes off that, with a plain `0(reg)`
 *   on the load -- for every spelling tried, including a complete array
 *   `u16 B[0x100]`, two accesses at different indices, and a
 *   `union { u16 h; u8 b; }` element type. Only the constant base pointer
 *   gives the target's form, so declaring D_80012094 is a dead end; that
 *   declaration was added and then reverted.
 * - The branch sense is `if (t == *cmp)`, not `!=`: the target's
 *   `bne $a0, $v0, .L800424A0` jumps *past* the store.
 * - The counter has to be read into `t` and reused for `t + 1`; the target
 *   has `addiu $v0, $a0, 0x1` off the already-loaded value.
 * - arg1 is genuinely unused -- $a1 is overwritten with the -8 mask before
 *   any use -- and that mask constant is what ends up in $a1 at the
 *   func_800A1BD0 call, which takes one argument.
 * - D_800E3E48's element type is Unk42710, not the fieldscan-inferred
 *   Unk800E3E48: this function indexes it with the 0x8C stride and hands the
 *   element straight to func_80042710, whose both parameters are Unk42710.
 *   That is also what pins the record's size at 0x8C.
 */

void func_800423C0(u8 *arg0, s32 arg1, u8 *arg2) {
    if (arg2 != NULL) {
        Unk42710 *e = (Unk42710 *) &D_800E3E48[arg2[3] & 0x1F];
        s32 r;

        func_80042710((Unk42710 *) arg0, e);
        *(u32 *) (arg0 + 0x14) &= ~7;
        *(u32 *) (arg0 + 0x1C) &= ~7;
        *(u32 *) (arg0 + 0x14) |= e->unk_14 & 7;
        *(u32 *) (arg0 + 0x1C) |= e->unk_1C & 7;
        r = (s16) func_800A1BD0(arg0);
        if (r >= 0) {
            u16 *cmp = (u16 *) 0x80012094;
            u8 *p = (u8 *) 0x80010000;
            u16 t;

            p += r * 2;
            if (r == 0) {
                cmp = (u16 *) 0x80012096;
            }
            t = *(u16 *) (p + 0x2094);
            if (t == *cmp) {
                *(u16 *) (p + 0x2094) = (t + 1) & 7;
            }
            arg0[0x45] = p[0x2094];
            arg0[0x12] = p[0x2098];
        }
    }
    *(u32 *) (arg0 + 0x1C) |= 0x20000;
}
