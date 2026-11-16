/* PARK: func_800BB2E4, 158 of 158 words, 7 aligned differences (ovlverify
   reports 7 masked too).  ovl_dungeon.  Re-measured 2026-11-16 by agent j54;
   THIS FILE WAS CORRUPTED (the whole header was duplicated, half of it
   outside the comment, so the file did not compile) and is rewritten here.
   j48's allocation arithmetic is kept and EXTENDED: the window j48 called
   unreachable IS reachable, and the exact register pair flips.

   The three-way overlay dispatcher: with a fifth argument it brackets
   func_800B8FC8's output in a mask primitive; without one, and only when the
   fourth argument's low halfword is zero, it emits a single 0x10-byte
   primitive from the two words at *a and *b; and whenever that halfword is
   NOT zero it falls through to the 0x60000000 tint rectangle, a second mask
   primitive and func_80067E2C's draw-area primitive.

   The prototype `void func_800BB2E4(void *, void *, s32 *, s32, void *);' at
   line 315 is already pinned by four landed call sites -- do not change it.
   `d' cannot be narrowed to `s16' in the definition for that reason; the
   `(s16)d' narrowings are written out in the body instead.

   NEW DECLARATIONS NEEDED:

       void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
       void func_80067E2C(void *, void *);

   func_800B8FC8 is still INCLUDE_ASM; the prototype above is the one
   docs/dungeon-parks/func_800B8FC8.c's body defines, so the two agree.  The
   call site casts rather than changing it.

   ------------------------------------------------------------------ residue

   ONE REGISTER PAIR, SWAPPED, AND NOTHING ELSE.  `d' is in $s7 here and $s6
   in the target; `lean' is in $s6 here and $s7 there.  That is all 7 rows --
   the two prologue saves, the parameter copy, the `lean' copy in the guard's
   delay slot, the two `(s16)d' tests and the rect block's `lean' read.
   Every other word in the function, including the whole frame and both
   cross-jumped tails, matches.

   ------------------------------------------- THE ALLOCATION, MEASURED (j48)

   Read out of cc1's own `-da' dumps (`bb.c.lreg' flow info and `bb.c.greg'),
   by compiling this body plus the unit's declaration block as a standalone
   file with tools/cdkgcc.  That loop is worth reusing: it is one compile, it
   needs no splice and no verifier, and `;; N regs to allocate:' IS the
   allocation order.  Re-confirmed by j54, numbers identical:

       pseudo 83 = `d'     3 refs, live 178 insns, crosses 8 calls
       pseudo 89 = `lean'  2 refs, live  93 insns, crosses 8 calls, 2 bytes

   global.c's `allocno_compare' sorts DESCENDING by

       (int) ((double)(floor_log2 (n_refs) * n_refs) / live_length * 10000)

   (allocno_size is 1 for both), and the dump's order line is exactly that
   sort -- all fourteen allocnos check out against the formula:

       86 20000 | 88 18888 | 90 16000 | 94 6666 | 100 2142 | 87 1875 |
       82 1309  | 84 1071  | 91  746  | 80  588 | 81  526  | 85  327 |
       89  215  | 83  168

   `;; 14 regs to allocate: 86 88 90 94 100 87 82 84 91 80 81 85 89 83', and
   the dispositions walk $s0..$s7 in exactly that order (86 s0, 87 s1, 82 s2,
   84 s3, 91 s4, 85 s5, 89 s6, 83 s7), so THE ALLOCNO THAT SORTS FIRST GETS
   THE LOWER REGISTER.  `lean' (215) sorts before `d' (168), `lean' takes $s6
   and `d' takes $s7.  The target wants the other order.

   ------------------------- THE WINDOW IS REACHABLE: `d' AT FOUR REFERENCES

   j48 ruled out the ref-count route by holding `d's live length at 178:

     * d at 2 refs  -> 20000/178 = 112, FURTHER below lean.
     * d at 4 refs  -> 80000/178 = 449, which overtakes `g' at 327 and steals
       $s5.  (Measured consequence if it did happen: d $s5, g $s6, lean $s7 --
       g and d both move, so this really is the wrong side of 327.)
     * lean at 3 refs -> 30000/93 = 322, still under `g' and still above `d'.

   BUT A FOURTH REFERENCE PLACED AT THE END OF THE FUNCTION LENGTHENS `d' AS
   WELL AS WIDENING IT, and 8 * 10000 / L lands inside (215, 327) for any L
   between 245 and 371.  The block is long enough: allocno 82 spans 252 insns
   and 85 spans 244, so the function is about 255 insns and `d' is born in the
   prologue.  MEASURED, by wrapping the LAST statement of the rect block in a
   test that is always true there (`(s16)d != 0' holds on both paths into
   `rect'):

       if ((s16)d != 0)
       {
           func_8006658C(r, q);
       }

   gives `Register 83 used 4 times across 262 insns' -> 8 * 10000 / 262 = 305,
   the order line becomes `... 80 81 85 83 89' instead of `... 80 81 85 89 83',
   and `;; Register dispositions' becomes **83 in 22 ($s6) and 89 in 23 ($s7)
   -- the target's pair, exactly**.  The whole function is then 160 words and
   FOUR aligned rows: two of them are the probe's own `sll'/`beqz', one is the
   branch displacement that moves with them, and the fourth is word 56.
   Saved as the alternative body at the bottom of this file.

   SO WHAT IS LEFT IS A FOURTH REFERENCE TO `d' THAT COSTS NO WORDS and sits
   late enough to carry the live length past 244.  Measured and rejected:

     * the probe above (guarding the final func_8006658C) ...... 160 words / 4
     * `if ((s16)d == 0) { return; }' as the LAST statement of the
       function ................................................ 158 / 7 --
       jump.c deletes it outright (both arms return), so `d' is back to
       3 refs and the body is byte-identical to this one
     * the same test added at the end of the `e != NULL' arm .... 171 / 26
     * the same test added at the end of the `else' arm ......... 166 / 25

   AND ONE ROW IS NOT THE ALLOCATION AT ALL.  Target word 56 is
   `sll $v0, $a3, 16' -- the else-arm's `(s16)d' test reads the INCOMING
   ARGUMENT REGISTER, which is still live on that path because nothing on it
   has called yet.  This body (and the 4-ref probe) read the callee-saved copy
   instead.  So the target's `d' pseudo is referenced only twice (the
   parameter copy and the word-95 test), which by the formula alone would put
   it at 112 -- BELOW lean.  Either the target's `lean' is longer-lived than
   ours (2 refs needs live length > 178 to fall under 112) or its `d' copy is
   placed differently.  Whichever it is, the 4-ref probe reaches the same
   dispositions by another road, and word 56 is the row that road does not
   fix.

   ---------------------------------------- what paid, in the order it paid

   55 -> 21 -> 11 -> 7 differing, and 153 -> 154 -> 158 of 158 words.

   * THE SCISSOR-OFF BLOCK IS WRITTEN OUT IN BOTH ARMS, not once after the
     join with a flag variable.  55 -> 21.  The target reloads
     `D_80083160.unk_00' at the END of EACH arm (`lw $v1, %lo(D_80083160)
     ($s4)' at 0x800BB3B8 and at 0x800BB434) and `.L800BB43C' opens with
     `lw $s0, 0x8D0($v1)'.  That is cross-jumping: the two arms are identical
     from `.L800BB43C' down, and the merge stops at the differing
     `addiu $a1, $zero, 0x1' / `addu $a1, $zero, $zero', which leaves each
     arm's own reload outside the merged tail.  Written once with
     `func_80067EF4(q, f, 0)' and an `s32 f' there is ONE reload at the join,
     the `%hi(D_80083160)' pseudo never has a use that crosses a call, it
     stays in a caller-saved register, the two later reads rebuild their own
     `lui', and the function is 153 words with a 0x40 frame against the
     target's 0x48 -- one callee-saved register short.
   * `s16 lean;' AND NOT `s32 lean;'.  21 -> 11, and the last FOUR words.
     As an `s32' the boolean is tested directly out of its callee-saved
     register in both arms (`beqz $s6'); the target copies it to $v0 first
     in each (`addu $v0, $a2, $zero' / `addu $v0, $s7, $zero'), which is two
     words, and the copy is also what stops the first arm's
     `sw $v0, 0xC($a1)' being stolen into the `beqz' delay slot, which is the
     third and fourth.  `u8 lean' is 158/13, so it is the `s32' that is wrong
     rather than the `s16' that is right.  This is the same lever the sibling
     func_800B835C needed, read from the other side: there the target has NO
     copy and here it has two.
   * `lean' COMPUTED AFTER `r', not before it.  11 -> 7.  All six orderings
     of {g = &D_80083160; r = ...+0x8B0; lean = ...} were measured: g,r,l and
     r,g,l are 7; g,l,r and r,l,g are 11; l,g,r and l,r,g are 15 -- and the
     two 15s also split `%hi(D_80083160)'s materialisation across two
     registers and sign-extend `lean' at its use.
   * The first argument's halfword arithmetic is `((s16) * ((u16 *)c + 2) /
     2)' -- a SIGNED DIVIDE, `sll 16 / sra 16 / srl 31 / addu / sra 1', not
     the `>> 1' the sibling func_800B835C uses at the same slot.  The `(s16)'
     cast of an unsigned load is what produces `lhu' + `sll`/`sra' where
     `*(s16 *)' alone would be a one-word `lh'; the listing has a real `lh'
     at 0x800BB4A4 for the rect block's y, so both spellings are present in
     this one function and they are not interchangeable.

   -------------------------------------------- measured and rejected, no better

   All 158 words unless noted, against the 7-aligned base:

   * local declaration order: `lean' first and `lean' last are both 7.  The
     allocation does not move with declaration order here, so the two
     allocnos are not tied.
   * `dd = (s16)d;' hoisted to the top with both tests reading `dd':
     157 words / 12.  `dd = d;' as a plain copy first: 9.  After `lean': 11.
   * a second boolean `lean2 = lean;' for the rect block's test: 7.
   * `lean = g->unk_00 != D_801C9E40;': 7.  `r = g->unk_00 + 0x8B0;': 7.
   * the rect test as `if (lean != 0)': 7.  `lean = (s16)(... != ...)': 7.
   * the tail test inverted (`if ((s16)d != 0) goto rect; return;'): 7.
   * `s16 d' in the definition does not compile -- line 315's prototype.
   * `s32 dd = d;' at the top with ONLY the else-arm test reading `dd': 7,
     and byte-identical -- cse canonicalises `dd' onto `d' because `d' lives
     longer (cse.c `make_regs_eqv'), so the copy never reaches allocation.
     Assigned just before the `if (e != NULL)' instead: also 7.  With only
     the LATE test on `dd': 9.
   * `lean2 = lean;' with EVERY use renamed onto `lean2' (the rename lever,
     not a copy alongside): 7, byte-identical -- same cse cure.  `s32 lean2'
     that way is 154 words / 20.
   * the `goto rect' replaced by `else if ((s16)d == 0) { ... }' with no
     label at all: 7, byte-identical.  So the goto is not load-bearing.
   * `(u16)d' instead of `(s16)d' in the else-arm test: 7.
   * the two arms exchanged (`if (e == NULL) {...} else {...}'): 69.  The
     e != NULL arm must be the fall-through.
   * the rect block's 0xC and 0xE stores hoisted above the `if (lean)':
     lean 97 -- six insns short of the 119 a 2-ref `lean' needs.

   The `goto rect' is load-bearing in shape if not in spelling: the `e == 0,
   (s16)d != 0' path enters the tint rectangle WITHOUT the scissor-off block,
   and the listing's `bnez $v0, .L800BB46C' jumps to the same label the
   `if ((s16)d == 0) return;' falls through to.

   `.greg' work, not the permuter. */

void func_800BB2E4(void *a, void *b, s32 *c, s32 d, void *e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;

    g = &D_80083160;
    r = D_80083160.unk_00 + 0x8B0;
    lean = D_80083160.unk_00 != D_801C9E40;
    if (e != NULL)
    {
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        func_8006658C(r, q);
        w[0] = *(u16 *)c + ((s16) * ((u16 *)c + 2) / 2);
        w[1] = *((u16 *)c + 1) + *((u16 *)c + 3);
        func_800B8FC8((u8 *)e, c, (s32 *)w, 0, 0);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 1, 0);
        func_8006658C(r, q);
    }
    else
    {
        if ((s16)d != 0)
        {
            goto rect;
        }
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x8) = *(u32 *)a;
        *(u32 *)(u + 0xC) = *(u32 *)b;
        if (lean)
        {
            *(u16 *)(u + 0xA) -= 0xE0;
            *(u16 *)(u + 0xE) -= 0xE0;
        }
        u[0x3] = 3;
        *(u32 *)(u + 0x4) = 0;
        u[0x7] = 0x40;
        func_8006658C(r, u);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        func_8006658C(r, q);
    }
    if ((s16)d == 0)
    {
        return;
    }
rect:
    u = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(u + 0x10);
    *(u32 *)(u + 0x4) = 0x60000000;
    u[0x3] = 3;
    *(u16 *)(u + 0x8) = *(u16 *)c;
    t = *((s16 *)c + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    *(s16 *)(u + 0xA) = t;
    *(u16 *)(u + 0xC) = *((u16 *)c + 2);
    *(u16 *)(u + 0xE) = *((u16 *)c + 3);
    func_8006658C(r, u);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067EF4(q, 0, 1);
    func_8006658C(r, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067E2C(q, c);
    func_8006658C(r, q);
}

#if 0
/* ALTERNATIVE (agent j54, 2026-11-16): the 4-reference probe -- 160 words,
   4 aligned rows, and the greg dispositions are the TARGET pair (d $s6,
   lean $s7).  Two of its four rows are the probe branch itself.  Replace the
   probe with a fourth reference to `d' that costs no words and this lands. */
void func_800BB2E4(void *a, void *b, s32 *c, s32 d, void *e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;

    g = &D_80083160;
    r = D_80083160.unk_00 + 0x8B0;
    lean = D_80083160.unk_00 != D_801C9E40;
    if (e != NULL)
    {
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        func_8006658C(r, q);
        w[0] = *(u16 *)c + ((s16) * ((u16 *)c + 2) / 2);
        w[1] = *((u16 *)c + 1) + *((u16 *)c + 3);
        func_800B8FC8((u8 *)e, c, (s32 *)w, 0, 0);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 1, 0);
        func_8006658C(r, q);
    }
    else
    {
        if ((s16)d != 0)
        {
            goto rect;
        }
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x8) = *(u32 *)a;
        *(u32 *)(u + 0xC) = *(u32 *)b;
        if (lean)
        {
            *(u16 *)(u + 0xA) -= 0xE0;
            *(u16 *)(u + 0xE) -= 0xE0;
        }
        u[0x3] = 3;
        *(u32 *)(u + 0x4) = 0;
        u[0x7] = 0x40;
        func_8006658C(r, u);
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
        func_80067EF4(q, 0, 0);
        func_8006658C(r, q);
    }
    if ((s16)d == 0)
    {
        return;
    }
rect:
    u = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(u + 0x10);
    *(u32 *)(u + 0x4) = 0x60000000;
    u[0x3] = 3;
    *(u16 *)(u + 0x8) = *(u16 *)c;
    t = *((s16 *)c + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    *(s16 *)(u + 0xA) = t;
    *(u16 *)(u + 0xC) = *((u16 *)c + 2);
    *(u16 *)(u + 0xE) = *((u16 *)c + 3);
    func_8006658C(r, u);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067EF4(q, 0, 1);
    func_8006658C(r, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067E2C(q, c);
    if ((s16)d != 0)
    {
        func_8006658C(r, q);
    }
}
#endif
