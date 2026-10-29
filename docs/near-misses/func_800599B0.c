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
 * READ THIS BEFORE THE SECTIONS BELOW IT, 2026-10-28. Several of them weigh a
 * trade-off THIS SOURCE DOES NOT FACE, and one of them tells the next session
 * to "defeat combine, not cse". That is wrong, and the whole-diff print says
 * so: **all three copies are already here and byte-identical to the
 * listing's.** Side by side, the format-2 body, with the one differing word
 * marked:
 *
 *       sw   $s3, 0x0($s0)        sw   $s3, 0x0($s0)
 *       addu $v1, $s3, $zero      addu $v1, $s3, $zero    <- read-back #1
 *       lhu  $a2, 0x10($s2)       lhu  $a2, 0x10($s2)
 *       addu $v0, $v1, $zero      addu $v0, $v1, $zero    <- read-back #2
 *       sw   $s4, 0x24($s0)       sw   $s4, 0x24($s0)
 *       sw   $s4, 0x20($s0)       sw   $s4, 0x20($s0)
 *       sw   $v1, 0x4($s0)        sw   $v1, 0x4($s0)
 *   X   sw   $v0, 0x38($s0)       sw   $v1, 0x38($s0)     <- THE WORD
 *       sw   $a2, 0x18($s0)       sw   $a2, 0x18($s0)
 *       addu $a1, $a2, $zero      addu $a1, $a2, $zero    <- len's read-back
 *       addu $s3, $s3, $a2        addu $s3, $s3, $a2
 *       addu $v0, $v0, $a1        addu $v0, $v0, $a1
 *
 * Nothing has to be kept, defeated or traded: read-back #2's copy is present,
 * len's copy is present, both are unfolded in BOTH builds, and the function
 * is 133 words on both sides. The sections below that talk about "giving up
 * the copy", "keeping the copy", "a third copy" and "defeating combine" were
 * written from advise.py's register-difference summary rather than from this
 * print, and they overstate the problem.
 *
 * WHAT IS ACTUALLY WRONG is one register on one store, and the reason is
 * narrow. Read-back #2's copy insn has TWO consumers here -- the +0x38 store
 * and the +0x1C sum -- and ONE in the listing, the sum. The +0x38 store's own
 * `*(u32 *) (e + 0x00)` read becomes a copy because the +0x18 store between it
 * and the +0x04 store invalidates cse's entry for +0x00, and that copy is then
 * never folded into the store. For the listing's word the store has to read
 * read-back #1 DIRECTLY, which needs its +0x00 read to hit a VALID cse entry
 * -- i.e. no invalidation before it -- while the sum still needs one so that
 * ITS read becomes a copy. The two want the invalidation in two different
 * places and the +0x18 store is the only thing available to provide it. That
 * is the whole of the remaining word.
 *
 * MEASURED 2026-10-28 on that reading, and it moves the sum but not the store:
 *
 *     sum's first operand from +0x04 (store still +0x00)   133 words,  8 DIFF
 *     sum's first operand from +0x00                       134 words, 45 DIFF
 *     that, operands swapped                               134 words, 46 DIFF
 *
 * The 8-DIFF one does what it was meant to -- the sum reads read-back #1 and
 * #2's copy is left with a single consumer -- and the +0x38 store STILL takes
 * a copy of its own rather than #1, which is the proof that the store's copy
 * is not folded even with one use. So the question to answer is not about the
 * sum at all: it is why `(set P2 P1)` is not folded into `(set (mem e+0x38))
 * P2)`, or how the store's +0x00 read avoids being invalidated.
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
 *
 * 2026-10-28: "DEFEAT COMBINE" IS NOT REACHABLE BY NAMING, checked in the
 * compiler rather than by another run. The paragraph above ends on "anything
 * that reaches the listing has to give read-back #2 a single consumer AND keep
 * its copy -- i.e. defeat combine, not cse", and the obvious handle would be
 * REG_USERVAR_P, which is exactly what decided func_80059DAC's giv that same
 * day. combine.c consults it in five places -- :954, :1134, :1279, :1765,
 * :1831 -- and EVERY one of them is inside `#ifdef SMALL_REGISTER_CLASSES`,
 * which config/mips never defines. So combine treats a named local and a cse
 * temp identically here and no spelling of read-back #2 changes whether its
 * copy is folded. The full map of what REG_USERVAR_P does reach on this target
 * is in matching-levers, under its own heading.
 *
 * 2026-10-28, THE ONE FORM OF "DEFEAT COMBINE" THAT IS STILL OPEN. The note
 * above closes naming (combine.c's REG_USERVAR_P uses are all
 * SMALL_REGISTER_CLASSES-only, which config/mips never defines) and the file
 * already closes modifying the copy's source. There is a third way, and it is
 * structural rather than a spelling: `try_combine` only runs on a pair joined
 * by a LOG_LINK, and life analysis builds one only when the using insn is in
 * the SAME basic block --
 *
 *     if (y && (BLOCK_NUM (y) == blocknum) ...)
 *       LOG_LINKS (y) = gen_rtx (INSN_LIST, VOIDmode, insn, LOG_LINKS (y));
 *                                                -- 2.7.2 flow.c:2080-2091
 *
 * -- so a copy whose single use is in another block is never a candidate, and
 * read-back #2's copy would survive with its one consumer intact, which is
 * exactly what this file asks for.
 *
 * WHAT IS NOT KNOWN is whether a boundary can be had here for nothing. This
 * function needs no extra word -- it is 133 of 133 with one register wrong --
 * so a boundary that costs a word is no better than the shapes already
 * measured. func_80041E70's park prices all five ways of buying one in a
 * straight-line body and every one costs at least a word, so the only version
 * worth trying is a boundary this function ALREADY has.
 *
 * CHECKED THE SAME DAY, and it does not: the format-2 body runs
 *
 *     e = D_80085FA8 + i * 0x50;  *(u32 *)(e + 0x00) = n;
 *     len = ...;  +0x24 = 0x68;  +0x20 = 0x68;
 *     +0x04 = *(u32 *)(e + 0x00);  +0x18 = len;
 *     +0x38 = *(u32 *)(e + 0x00);
 *     +0x1C = *(u32 *)(e + 0x38) + *(u32 *)(e + 0x18);
 *
 * with no branch anywhere between the +0x00 store and the sum -- the nearest
 * one is the `if (f != 0)` five statements later. So the boundary would have to
 * be BOUGHT, every way of buying one costs at least a word (func_80041E70
 * prices all five), and this function has no word to spend: it is already 133
 * of 133 with a single register wrong. The combine route is therefore closed
 * in all three of its forms here -- naming, modifying the source, and a block
 * boundary -- and the remaining word is local-alloc's refusal to tie the
 * second read-back, exactly as the body of this file says.
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
