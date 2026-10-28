/* func_8005A778 (unk_2CDC0) -- 101 of 101 words, 20 DIFF. NOT compiled.
 * (A cast-form variant reaches 18, but it shifts into $v0 first and is the
 * wrong structure -- see the 2026-10-27 note below. This file carries the
 * in-place form on purpose.)
 *
 * func_8005A608 with a third argument and a bound check: claims a D_80086A40
 * slot for `p`, stores `h` at +0x10 twice (once as given, once as
 * func_80059F8C's answer), and returns -1 when the caller's index is out of
 * range as well as when there is no free slot or no handle.
 *
 * Everything structural is paid for, including:
 *
 *   * `j = id; id = (s16) id;` -- the lever written up in
 *     docs/near-misses/func_8005A608.c and in matching-levers section 7. It
 *     buys the raw parameter's copy AND stops cse forwarding the entry's
 *     sign-extension into the else arm, which is three words.
 *   * TWO variables hold `p`: the parameter for the `e->unk_04 = p` store and
 *     a second local `q` for the four field reads. That is the listing's
 *     `addu $s3, $s4, $zero`, a second callee-saved register for the same
 *     pointer.
 *   * `e->unk_10 = h` comes AFTER `e->unk_08 = ...` in the source, so sched2
 *     can lift the store into the `lhu 0x12($s3)` load-delay slot. Written
 *     before it, the slot takes a nop.
 *   * The bound check is on `(s16) i` AFTER `i = j`, which is what gives the
 *     else arm its own `sll`/`sra` pair ahead of the `slti 0x10`.
 *
 * SINCE PARKED: writing the sign-extension as two statements --
 * `id = id << 16; id = id >> 16;` instead of `id = (s16) id` -- reproduces
 * the listing's IN-PLACE `sll $a1, $a1, 16` / `sra $a1, $a1, 16` here too
 * (23 differing words rather than 21, but structurally closer; see the lever
 * in matching-levers section 7). What is then left is exactly
 * func_8005A608's single scheduling fact: the listing emits
 * `sw $zero, %lo(D_8007382C)($v0)` AFTER the shifts and this source emits it
 * before, which decides whether $v0 is still holding the symbol's high half
 * when `j = id` is allocated.
 *
 * THE OLDER ANALYSIS: 21 words, all of them the one register chain func_8005A608's
 * park describes -- the extension's intermediate takes $v0 here instead of
 * being in place in $a1, which pushes the entry's -1 to $a2, which lets
 * reload's find_equiv_reg reuse it in the then-arm and in the else instead of
 * materialising fresh, which in turn moves the else's index registers. Fix
 * that one allocation and this function, func_8005A608 and func_8005A90C all
 * fall. *
 * 2026-10-27, 21 DIFF -> 18, AND THE CONTESTED ALLOCATION IS NAMED.
 *
 * Moving `D_8007382C[0] = 0;` OFF the front -- to just after `i = 0;` or just
 * after `j = id;` -- is worth three words with the cast form (18 DIFF) and one
 * with the in-place form (20). All ten positions were swept against both
 * extension spellings:
 *
 *     cast    store first / after i=0 / after j=id / last   21 / 18 / 18 / 23
 *     inplace store first / i=0 / j=id / mid-shifts / last  23 / 20 / 20 / 23 / 23
 *
 * WORK FROM THE 20, NOT THE 18. The listing's extension is IN PLACE on $a1
 * (`sll $a1, $a1, 16` / `sra $a1, $a1, 16`), which only the two-statement form
 * produces; the cast form scores better while shifting into $v0 first, which
 * is the wrong structure. Same trap as func_800565D8's park.
 *
 * WHAT THE REMAINING WORDS ARE. The listing's head is
 *
 *     lui $v0,%hi(D_8007382C) ; move $v1,$a1 ; sw $s3 ; move $s3,$s4 ;
 *     sll $a1,$a1,16 ; sra $a1,$a1,16 ; sw $zero,%lo(D_8007382C)($v0) ;
 *     li $v0,-1
 *
 * so `j = id` is in $v1 and the symbol's HIGH half stays in $v0 across four
 * insns until the store consumes it. Two separate facts follow:
 *
 *   * WITH THE STORE EARLY (slot 1 or 2) `j` takes $v0, because the high is
 *     dead by then -- and the store has to precede `j = id` for that to work,
 *     which is the 20-DIFF shape.
 *   * WITH THE STORE LAST `j` correctly takes $v1 and the two shifts land
 *     exactly right, but the `lui` goes with its store: it lands at word 11
 *     and the `sw` at 19, where the listing has 8 and 14. That is sched1's
 *     birthing bump on the high pseudo -- single-set, so it is raised and
 *     placed as late as its use allows (section 8's first entry).
 *
 * So what is needed is the HIGH BORN EARLY AND USED LATE, with the store after
 * the shifts. A pointer local for the base does not do it: that gives
 * `lui`+`addiu` and the listing has the symbolic `sw ...%lo(...)($v0)` form,
 * one word fewer. Measured and NOT working: declaring the symbol as a SCALAR
 * under an `__asm__` alias instead of an incomplete array -- 24 DIFF at every
 * one of the five store positions, so the array spelling is load-bearing here
 * (section 6).
 *
 * The -1 in $v0 at word 15 and the fresh `li $a0,-1` at 22 are downstream of
 * this: once the high is out of $v0, the constant lands there instead of $a2
 * and reload's find_equiv_reg stops reusing it. Still the same single
 * allocation that the older analysis below identified, and still the one that
 * would also settle func_8005A608 and func_8005A90C.
 *
 * 2026-10-28: THE BUMP IS CONFIRMED FROM THE DUMP, so "sched1's birthing bump"
 * above is a measurement now rather than an inference, and that names the
 * requirement exactly. In the store-after-the-shifts shape, `.sched`'s first
 * block reads
 *
 *     ;; ready list at T-5: 24 (1) 17 (1) 26 (7f000001), now 26 24 17
 *
 * and insn 26 is `(set (reg:SI 90) (high:SI (symbol_ref:SI ("D_8007382C"))))`.
 * So the high pseudo is raised to LAUNCH_PRIORITY, is picked as soon as it is
 * ready, and -- the pass running backward -- is PLACED as late as its single
 * use allows, which is just before its own store. The listing has the `lui` at
 * word 8 and the store at 14, six insns apart, so THE LISTING'S HIGH IS NOT
 * BUMPED. An un-bumped insn with no producers sits at priority 1, loses every
 * tie in its block and drifts to the front -- which is "born early, used late",
 * word for word what the paragraph above asks for.
 *
 * AND THE csrc PROTOTYPE IS WRONG FOR PARAMETER 2, settled 2026-10-28 so the
 * standing signature-mismatch list can stop pointing here. `csrc/unk_2CDC0.c`
 * declares `(u8 *, s16, u32)` against this park's `(u8 *, s32, s32)`.
 * Parameter 2 as `s16` is 77 words -- TWENTY-FOUR SHORT -- and 94 DIFF: a
 * narrow parameter is assumed already extended by the caller, so the whole
 * `id = id << 16; id = id >> 16;` pair disappears, and that pair is the
 * listing's own `sll $a1, $a1, 16` / `sra $a1, $a1, 16`. The explicit
 * narrowing IS the evidence that the parameter is wide, and it is a register
 * parameter, so the prototype is not a witness either way. Parameter 3 as
 * `u32` is inert (101 words, the same 20 DIFF), so take the `u32` if the
 * prototype is ever reconciled and leave parameter 2 as `s32`.
 *
 * SO THIS PARK IS THE SAME MECHANISM AS THE func_8003A060 LANDING, and the
 * requirement reduces to one sentence: give the `(high (symbol_ref))` pseudo a
 * SECOND SET so `birthing_insn_p` stops firing (matching-levers section 8).
 * That also explains the -1 for free. With the high born at 11 and consumed in
 * the branch's delay slot at 19, $v0 is busy across the whole head and the
 * comparison's -1 is pushed to $a2; with the high at 8 and its store at 14,
 * $v0 falls free and `addiu $v0, $zero, -1` lands there at 15, exactly as the
 * listing has it.
 *
 * WHAT IS NOT KNOWN is how to get that second set. A `(high)` pseudo is
 * written once by construction: a second reference to the same symbol is cse'd
 * onto the same pseudo (still one set) or materialised in another block (a
 * different pseudo, also single-set and also bumped), and the two constructs
 * that do produce an uncollapsible second set -- the arms of an `if` and a
 * loop-carried variable -- are not available to a straight-line head without
 * spending a word. That is the open question, and it is a narrow one: it has a
 * definite answer in loop.c/local-alloc rather than in another source sweep.
 *
 * MEASURED 2026-10-28 and NOT working -- the DECLARED SIZE lever (section 6),
 * which this file had tried only as a scalar alias. All 101 words:
 *
 *     extern s32 D_8007382C[];    store after i=0 / after j=id / last   20/20/23
 *     extern s32 D_8007382C[1];   same three positions                  24/24/24
 *     extern s32 D_8007382C[2];   same three positions                  24/24/24
 *     extern s32 D_8007382C[4];   same three positions                  20/20/23
 *
 * So `[1]` and `[2]` behave like the scalar alias already recorded above (24 at
 * every position) and `[4]` behaves like the incomplete array. The threshold is
 * between 8 and 16 bytes, i.e. the -G8 small-data limit -- worth knowing that
 * THIS unit is a -G8 unit for addressing purposes even though it is not in the
 * Makefile's MASPSXFLAGS_G8 list, since the declared size still decides the
 * addressing shape. Neither size reaches the listing.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059F8C(s32, u32);

s32 func_8005A778(u8 *p, s32 id, s32 h)
{
    u8 *q = p;
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 r;

    i = 0;
    D_8007382C[0] = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
    if (id == -1)
    {
        do
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
        } while (i < 0x10);
        return -1;
    }
    i = j;
    if (i >= 0x10)
    {
        return -1;
    }
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (q + 0x12) << 9) + 0xA20;
    e->unk_10 = h;
    e->unk_14 = *(u32 *) (q + 0xC) - e->unk_08;
    e->unk_18 = q[0x18];
    e->unk_1B = q[0x19];
    r = func_80059F8C(h, e->unk_14);
    e->unk_10 = r;
    if (r == -1)
    {
        return -1;
    }
    return k;
}
