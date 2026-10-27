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
 * perm.py over all nine statements of the format-2 body (400 of 362880
 * orderings sampled) returns the order below as the best, at the same 1 DIFF
 * -- so this is NOT a statement-order problem and the remaining lever is in
 * local-alloc's refusal to tie the second read-back, not in the source
 * sequence.
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
