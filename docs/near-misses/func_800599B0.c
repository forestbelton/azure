/* func_800599B0 (unk_2CDC0) -- 133 of 133 words, 1 DIFF. NOT compiled.
 *
 * The sequencer start: clears D_800737C4, picks the format from the first byte
 * of the data at D_80085FA4 (0x70 -> 0, 0x4B -> 2, otherwise 1), runs
 * func_80058B70, and then initialises all D_800869B4 of the D_80085FA8 track
 * entries -- one way for formats 0 and 1, another for format 2, where each
 * track also gets a running offset advanced by its own length halfword.
 *
 * ONE word differs: the `sw` to +0x38 reads `$v0` here and `$v1` in the
 * listing. Everything else -- both loops, the switch tree, the shared tail --
 * is identical.
 *
 * Six things paid for, 126 DIFF down to 1:
 *
 *   * `switch ((s32) D_800737C8[0])` with the CAST. The symbol is `u32` in
 *     this unit (func_80059BC4 needs that), and unsigned gives one `sltiu`
 *     range check where the listing has `bltz` then `slti 0x2` then
 *     `beq 0x2` -- gcc's signed balanced tree over cases 0, 1 and 2.
 *
 *   * Each entry pointer is INDEXED inside the loop body
 *     (`e = D_80085FA8 + i * 0x50`). Walked as `e += 0x50` loop.c adds a
 *     SECOND giv biased to `e + 0x40` and rewrites half the fields as
 *     negative displacements off it -- this unit's standing recipe, and here
 *     it was 40 words.
 *
 *   * Format 2's length halfword is also INDEXED, off a base read before the
 *     loop: `b = (u8 *) D_80085FA4[0];` then `*(u16 *) (b + i * 2 + 0x10)`.
 *     The base has to be a local because the loop calls func_80058A04, so the
 *     load is not loop-invariant; and the index has to be an index because
 *     that is what makes loop.c put the giv's initialisation in the preheader
 *     -- the listing's `addu $s2, $a0, $zero`, with the `lw` itself up in the
 *     pre-guard block.
 *
 *   * `f = D_800737C4[0];` is read into a LOCAL between the three byte stores
 *     and the three word stores of the shared tail. A store through `e` and a
 *     load of a global are a real dependence to sched, so left inside
 *     `if (D_800737C4[0] != 0)` after the stores the load cannot reach a
 *     delay slot and both loops end with stalls. This is advise.py's
 *     DELAY-NOPS lever (and it is what landed func_8005313C outright).
 *
 *   * Format 2's +0x04, +0x38 and +0x1C are READ-BACKS, not re-spellings of
 *     `n` and `len`. That is where the listing's four reg-to-reg moves come
 *     from: cse replaces each load with the register already holding the
 *     value, which is a move insn (see matching-levers section 8). Spelled
 *     as `n` and `len` directly the function loses four words.
 *
 *   * The +0x18 store sits BETWEEN the +0x04 and +0x38 stores. That is worth
 *     one word and is the whole reason this reaches 133: it invalidates
 *     cse's entry for +0x00, so the second read-back is a fresh pseudo and a
 *     real move. With the three stores in address order the second read-back
 *     is free and the function is a word short.
 *
 * WHAT IS LEFT. The listing stores +0x04 and +0x38 from ONE read-back ($v1)
 * and keeps the SECOND ($v0) only for the +0x1C sum; here the second
 * read-back serves both the +0x38 store and the sum, so the store names $v0.
 * advise.py reports it as one register-only difference and `--greg` puts the
 * two candidate quantities 33% apart on qty_compare_1, which needs +2 or +3
 * references to flip -- more than any no-cost split found. Measured and not
 * working: the read-back in a local reused for both stores (the move then
 * disappears and the function is a word short); reading +0x38 back from
 * +0x04; assigning the read-back to `v`, `f`, `i`, `len` or `n` instead of a
 * fresh local; assigning the same local twice; both operand orders of the
 * sum; and every position of the +0x18 store.
 *
 * Also measured 2026-10-27: giving the format-2 arm its OWN copy of each
 * shared local in turn -- `e`, `n`, `v`, `f`, `i` -- which is the
 * one-variable-per-block lever that landed func_800517CC. It does not reach
 * this: `e`, `v` and `f` are still 1 DIFF, `n` is 9 and `i` is 19. All six
 * orderings of the +0x04 / +0x18 / +0x38 stores are 1, 8 or 44, with the
 * order below the best.
 *
 * perm.py over all nine statements of the format-2 body (400 of 362880
 * orderings sampled) returns the order below as the best, at the same 1 DIFF
 * -- so this is NOT a statement-order problem and the remaining lever is in
 * local-alloc's refusal to tie the second read-back, not in the source
 * sequence.
 *
 * MEASURED 2026-10-28 (second batch), five more shapes and none of them moves
 * the +0x38 store onto read-back #1:
 *
 *   * the +0x1C sum's two operands read into their OWN LOCALS just above it,
 *     one local or two -- 133 words, still 1 DIFF, the same word. So the sum's
 *     two surviving copies are not what the store's register depends on.
 *   * ONE SHARED LOCAL for both stores (`y = *(u32 *) (e + 0x00);` with both
 *     stores writing `y`), in all three positions of the +0x18 store relative
 *     to them, and with the sum reading +0x38 or +0x04: 132 words every time,
 *     44-46 DIFF. The shared local is exactly the listing's "both stores from
 *     read-back #1", and it always costs the third copy -- which is the word.
 *   * the +0x38 store's value read back from +0x04 instead of +0x00: 132
 *     words alone (46 DIFF), and 133 words / 11 DIFF with the sum's first
 *     operand also moved to +0x04.
 *
 * AND THE COMBINE-BLOCKING ROUTE IS CLOSED TOO, 2026-10-28. `try_combine`
 * cannot fold a copy whose SOURCE is modified between the copy and its use,
 * and this function has such a modification available for nothing: `n += len`
 * already happens, just below the +0x1C store. Moved up between the +0x38
 * store and the sum, and also above the +0x38 store, it is 133 words and the
 * SAME single differing word both times.
 *
 * The reason is worth keeping: read-back #2 is a copy of READ-BACK #1's
 * pseudo, not of `n`. cse copies `n` into a fresh pseudo for #1 (the listing's
 * `addu $v1, $s3, $zero`) and then copies THAT for #2, so modifying `n` leaves
 * #2's source untouched. Blocking combine would need #1 itself to be
 * reassigned between, and #1 is a cse artefact with no source-level name --
 * giving it one (a local read of +0x00 stored to both fields) is the shared
 * local that costs the third copy, measured above at 132 words.
 *
 * So the listing has BOTH things this source can only have one of: the two
 * stores sharing a register AND a third copy. Every spelling that shares gives
 * up the copy and every spelling that keeps the copy makes the +0x38 store its
 * consumer. That is the shape of the remaining word, stated as precisely as
 * these measurements can.
 *
 * MEASURED 2026-10-28, and it closes the reordering idea rather than opening
 * it. The listing's shape is `sw $v1,0x4 ; sw $v1,0x38` -- BOTH stores from
 * read-back #1 -- with read-back #2 a surviving copy of it that only the
 * +0x1C sum reads. The obvious way to get the two stores onto one pseudo is to
 * put the +0x18 store BELOW the +0x38 one (order 0x04, 0x38, 0x18, sum) so
 * nothing intervenes between them. That is 132 words, one SHORT, 44 DIFF:
 *
 *   * the two stores do share read-back #1, as intended; but
 *   * the sum's `*(u32 *) (e + 0x38)` then reuses it as well, because a store
 *     to `e + 0x18` does NOT invalidate cse's entry for `e + 0x38` -- two
 *     constant displacements off the SAME base register are disambiguated, so
 *     the +0x18 store cannot be used as a cse barrier for +0x38 at all; and
 *   * with only one consumer left, combine folds the substituted copy into the
 *     `addu` and the move disappears -- hence the missing word.
 *
 * Swapping the sum's operands in that order is 46. So the missing ingredient is
 * NOT an ordering: it is a SECOND read-back of the same value whose copy
 * SURVIVES combine, and in this source the copy survives only because it has
 * two consumers (the +0x38 store and the sum), which is exactly what makes the
 * store name $v0. Anything that reaches the listing has to give read-back #2 a
 * single consumer AND keep its copy -- i.e. defeat combine, not cse.
 *
 * MEASURED 2026-10-27, reading the listing's four moves rather than the diff.
 * The listing is `sw $s3,0x0 ; addu $v1,$s3 ; lhu $a2,0x10 ; addu $v0,$v1 ;
 * ... sw $v1,0x4 ; sw $v1,0x38 ; sw $a2,0x18 ; addu $a1,$a2 ; addu $v0,$v0,$a1
 * ; sw $v0,0x1C`, i.e. BOTH operands of the +0x1C sum are their own
 * read-backs (a copy of the +0x38 value and a copy of len) while +0x04 and
 * +0x38 share the first one. Giving the sum's first operand an explicit
 * temporary read before the +0x18 store, with the +0x38 store moved up, does
 * not reproduce that: from +0x38 it is 132 words / 39 DIFF, from +0x04 132 /
 * 43, and from +0x00 133 / 11. All three are worse than the one register this
 * file is on, so the shape below stands; the temporary is not the answer.
 */

#include "common.h"

extern u32 D_800737C4[];
extern u32 D_800737C8[];
extern u32 D_800869B4n[] __asm__("D_800869B4");

void func_80058B70(void);
void func_800597A8(u8 *);
u32 func_80058A04(u8 *);
void func_80059814(u8 *);

s32 func_800599B0(void)
{
    u8 *e;
    u8 *b;
    u32 i;
    u32 n;
    u32 v;
    u32 len;
    u32 f;

    D_800737C4[0] = 0;
    D_800737C8[0] = 1;
    n = 0;
    if (*(u8 *) D_80085FA4[0] == 0x70)
    {
        D_800737C8[0] = 0;
    }
    else if (*(u8 *) D_80085FA4[0] == 0x4B)
    {
        D_800737C8[0] = 2;
    }
    func_80058B70();
    switch ((s32) D_800737C8[0])
    {
    case 0:
    case 1:
        i = 0;
        if (D_800869B4n[0] != 0)
        {
            do
            {
                e = D_80085FA8 + i * 0x50;
                if (D_800737C8[0] == 1)
                {
                    *(u32 *) (e + 0x00) = n + 1;
                    *(u32 *) (e + 0x24) = 0x68;
                    *(u32 *) (e + 0x20) = 0x68;
                    func_800597A8(e);
                    n = *(u32 *) (e + 0x00);
                }
                *(u32 *) (e + 0x04) = *(u32 *) (e + 0x00);
                v = func_80058A04(e);
                e[0x48] = 0;
                e[0x4A] = 0;
                e[0x49] = 0;
                f = D_800737C4[0];
                *(u32 *) (e + 0x10) = v;
                *(u32 *) (e + 0x2C) = 0;
                *(u32 *) (e + 0x40) = 0;
                if (f != 0)
                {
                    func_80059814(e);
                }
                i++;
            } while (i < D_800869B4n[0]);
        }
        break;
    case 2:
        n = 0x50;
        b = (u8 *) D_80085FA4[0];
        i = 0;
        if (D_800869B4n[0] != 0)
        {
            do
            {
                e = D_80085FA8 + i * 0x50;
                *(u32 *) (e + 0x00) = n;
                len = *(u16 *) (b + i * 2 + 0x10);
                *(u32 *) (e + 0x24) = 0x68;
                *(u32 *) (e + 0x20) = 0x68;
                *(u32 *) (e + 0x04) = *(u32 *) (e + 0x00);
                *(u32 *) (e + 0x18) = len;
                *(u32 *) (e + 0x38) = *(u32 *) (e + 0x00);
                *(u32 *) (e + 0x1C) = *(u32 *) (e + 0x38) + *(u32 *) (e + 0x18);
                n += len;
                v = func_80058A04(e);
                e[0x48] = 0;
                e[0x4A] = 0;
                e[0x49] = 0;
                f = D_800737C4[0];
                *(u32 *) (e + 0x10) = v;
                *(u32 *) (e + 0x2C) = 0;
                *(u32 *) (e + 0x40) = 0;
                if (f != 0)
                {
                    func_80059814(e);
                }
                i++;
            } while (i < D_800869B4n[0]);
        }
        break;
    }
    return 0;
}
