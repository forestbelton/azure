/* func_800B1768 (ovl_dungeon) -- PARK, best candidate so far.
   target 234 words, candidate 234 words, 38 aligned differences
   (ovlverify masked: 48 differing -- read aligndiff, not that number).
   Previous park was 234 words / 55 aligned; this is the same word count with
   17 fewer rows, and the residue is now ONE named mechanism (below).

   SIGNATURE, for whoever takes func_800B6AC4 (which is the only `jal` to it
   anywhere and is itself parked at docs/dungeon-parks/func_800B6AC4.c):
       void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f);
   The two landed callers already pass six arguments
   (`func_800B1768(0x21, 0xE0, 0x84, 0, 0, 0)` at csrc/ovl_dungeon.c:3178 and
   `func_800B1768(0, 0x27, 0x40, 0x209, 0, 0)` at :25179), all constants, so
   adding this prototype leaves both at 0 differing -- checked by a whole-unit
   sweep of all 803 landed names.
   The fifth parameter is the stack word at 0x58($sp) copied to the local
   0x10($sp) and tested twice as a halfword; the sixth is the halfword at
   0x5C($sp) copied to 0x18($sp) and stored to q+0x26 at the end.

   NEEDS THESE DECLARATIONS at the top of the unit:
       s32 func_80069E98(u8 *);
       s8 *func_8004E298(u8 *, u8 *, s32);
       void func_800B1768(s16, s16, s16, s16, s16, s16);
       extern u8 *D_80073618[];
       extern u8 *D_800DF03C[];

   TEMPLATE: the landed func_800B1B10 (csrc/ovl_dungeon.c, "The dungeon's
   message panel") is this function one layer over -- same func_8003FD64(0x12,
   D_80083498) node, same func_800B14FC step, same func_800B06F0 tag, the same
   three func_800B12F4 chains parked in the +0x50 array off the +0x20 view,
   and the same `func_800B1320(t, e != 0 ? A : B, (s16)(-*(s8 *)(t+3) - K))`
   tail.  Its four recorded spellings all transferred.  ITS NOTE (3) IS THE
   ONE THAT MATTERS HERE and it is now measured on this function too: "ONE
   cursor local for the second and third func_800B12F4 chains."

   WHAT CHANGED ON 2026-11-16 (j90), three measured mechanisms:

   (1) ONE LOCAL FOR THE SECOND AND THIRD CHAINS, as the template says.
       Writing the func_800B1434 result into a temp and assigning it back to
       `u` -- `tmp = func_800B1434(u, u - 0x60); func_800B1320(u, 1, 1);
       u = tmp;` -- takes the body from 55 aligned rows to 40 and fixes the
       ENTIRE $s0/$s1 tail in one step.  It costs one word (the `u = tmp`
       copy does not coalesce; see the residue).  The park's old note called
       this "235 words but only 41 aligned differences ... the shape to aim a
       permuter at"; it is 235/40 now and it is not a permuter hint, it is
       the structure.
   (2) `v2`/`z2` -- the park's own cluster-(2) fix, in legal C -- is correct
       and is now adopted: +2 words, and every row from the first `-2` to the
       end becomes register-only.
   (3) `base` MUST HAVE MORE THAN ONE SET, and that is what kills the park's
       cluster (1).  The mechanism is combine's `nonzero_bits`, REG case
       (combine.c, the `reg_last_set_value` test): it returns
       `reg_last_set_nonzero_bits` -- the value computed at the `base =
       has * 10` insn, which is 0xA -- when `reg_n_sets[base] == 1`, EVEN
       ACROSS A BASIC BLOCK BOUNDARY.  With bit 7 of `base` provably clear,
       simplify_binary_operation rewrites `(plus base 128)` as `(ior base
       128)` (combine.c, "If we are adding two things that have no bits in
       common, convert the addition into an IOR"), and the IOR form costs a
       `move`+`ori` pair plus a `j` instead of the target's single
       `addiu $a1, $v1, 0x80` in the branch delay slot.
       Give `base` a SECOND SET and combine must fall back to
       `reg_nonzero_bits[base]`, which was accumulated with
       `nonzero_sign_valid == 0` and is therefore all-ones -- the addition
       stays an addition.  The second set used here is the `z` computation:
       `z` and `base` are never live at the same time, so ONE local serves
       both roles (`base = has * 24 + w;` ... `t[0xA] = z2;`).  That is -2
       words and it is the same "one local, two roles" move as (1).
       MEASURED: with the merge and `v2`/`z2` in place, `base` reused as `z`
       is 234 words / 38 aligned; without it, 237 words / 30 aligned.
       The park's old `__asm__("" : "+r"(base))` device was measuring exactly
       this mechanism by hand; it is not needed.

   RESIDUE, 38 rows, and it is ONE cause plus its shadow:

   (A) THE ONLY REAL DIFFERENCE IS TWO INSNS IN THE WRONG ORDER.
       target  160 addu $a0, $s1, $zero     <- a0 = u, for func_800B1320
               161 addu $s1, $v0, $zero     <- $s1 = func_800B1434's result
       candidate emits the result copy FIRST, so the two pseudos CONFLICT and
       cannot share $s1; the candidate then needs the third `move` that makes
       the merge cost a word.  In the target they are disjoint (u is dead the
       instant it is copied to $a0) so `u` and `t` get the SAME register and
       the copy is free.
       This is a pre-sched RTL ORDER fact, not a scheduler tie: at sched1
       every insn in that block has priority 1, `rank_for_schedule`'s class
       test ties (every MIPS move has insn_cost 1, so both are class 3), and
       the LUID tie-break therefore reproduces the source order -- which puts
       `(set t v0)` first because expand_call emits the return copy
       immediately after the call.  Read the trace with
       `python3 tools/decomp/sched.py <file> func_800B1768 --sched1`: the
       relevant block is "basic block number 12", insns 380 (`(set 97 v0)`)
       and 383 (`(set a0 (reg 96))`), chosen at T-16 and T-15.
   (B) the rest is the shadow: because the extra temp occupies $s0, the
       sign-extended `e` is pushed out of $s0 into $s5, and `col`/`j`/`d`
       rotate ($s5/$s6/$s7 against the target's $s6/$s7/$s5).  Target packing
       is $s0={&D_800DF03C[j], (s16)e}, $s1={e, u, t}, $s5={y, col},
       $s6={d, w}, $s7={j}; the candidate's is
       $s0={&D_800DF03C[j], tmp}, $s1={e, u}, $s5={y, j, (s16)e}, $s6={col},
       $s7={d, w}.  The quantity multiset is identical -- only the grouping
       differs -- and every row of (B) follows from (A).
   (C) `base`'s final `sll` has moved into the branch arm (candidate word
       105, target word 102) and the target's `nop` at word 104 is gone.
       That is the price of the second set in (3) and it is 4 of the 38 rows.

   SO: a permuter run on this body only has to find the order in (A).

   MEASURED AND REJECTED (j90's numbers; the previous park's are below)
   * DECLARATION ORDER IS INERT.  All 183 single-move permutations of the 14
     locals measure 234 words / 55 aligned -- IDENTICAL, not "56 or worse" as
     the old header said.  Do not sweep it again.
   * A SEPARATE LOCAL for `t + 0x18` (so `t` has reg_n_sets == 1) DOES move
     the result copy -- but two slots too far, into the func_800B1320 delay
     slot, because `birthing_insn_p` (sched.c) then gives it `max_priority`
     and it is taken the first cycle it is ready.  234 words / 57 aligned.
     With the merge: 235/48.  This is the right mechanism aimed one notch
     wrong; a lever that moves the copy exactly ONE slot closes the park.
   * A SEPARATE LOCAL for `u + 0x60`: 234/113 alone, 234/63 with the t+0x18
     local.  Do not.
   * A SEPARATE LOCAL for `&D_800DF03C[j]` (`pp = &D_800DF03C[j]`): 234/56.
     The current spelling (the address CSEd, loaded twice) is right.
   * THE TEMP VARIABLE'S IDENTITY does not matter: `tmp`, `u2`, a reused `q2`
     all give 235/40; reusing the dead `r` gives 235/85.  Assigning the temp
     back to `t` instead of `u` lets cse propagate it away and returns the
     body to BASE (234/57).
   * ALL 25 LEGAL ORDERINGS of the six statements from `u += 0x60` to the
     second `i++` were swept with `v2`/`z2` in place.  Two tie for best:
     the source order at 236/49 and `*col`-first at 235/49; the rest are
     51-70.  Keep the source order -- it is the one with the right word
     count once the merge is in.
   * TERNARY SPELLINGS, all with the merge and `v2`/`z2`: an `if/else` that
     writes `base` in both arms 235/39; `+=` in both arms 235/38; a separate
     `arg` local 233/35; `base = e != 0 ? ... : ...;` as its own statement
     235/39; swapping the arms' order 235/39; `base = w;` after the call
     (a dead second set) 235/31 -- it does NOT fix the IOR, it deletes the
     target's `nop` at word 104 instead.
   * `base` reused as `v` instead of as `z`: 235/36.  As BOTH: 235/37.
     As `z` written `has * 24 + w` instead of `w + has * 24`: ALSO 234/38,
     and NOT the same stream -- it emits `addu $v1, $v0, $s7` where the
     target (and `w + has * 24`) emit `addu $a0, $s6, $v0`, operands the
     other way round.  Checked by diffing the two .s files, because the
     scores are equal; keep `w + has * 24`.
   * THE OLD HEADER'S REJECTIONS STILL STAND and were not re-measured:
     `s32 j` 234/56 (`s16 j` is what puts the `andi` at word 52), `u8 j`
     238/102, `s16 base` 232/59, `s16 has` 232/114, `s16 w` 237/62,
     `u32 has` identical to `s32 has`, `has = k != 0;` 234/90, `has = i;`
     identical (cse folds it back), `base = has * 5 * 2` identical,
     `0x80 + base` identical, dropping the else arm's cast identical,
     the argument as a statement 230/75, `base - w + 0x76` 234/57,
     a `b2 = base + 0x76` local 234/57, `j = d & 0xFF` at five source
     positions identical at four and 236/61 hoisted above func_8003FD64.
     The `__asm__("" : "+r"(base))` device is superseded by (3) above. */
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *tmp;
    u32 *col;
    s32 i;
    s16 j;
    s32 has;
    s32 w;
    s32 v;
    s32 base;
    s32 v2;
    s32 z2;

    i = 0;
    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    if (e != 0)
    {
        *(u16 *)(r + 0x14) |= 1;
    }
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0xC) = y;
    *(s16 *)((u8 *)m + 0xA) = 0;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    j = d & 0xFF;
    *(s16 *)(q + 0x24) = d >> 8;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    col = (u32 *)((u8 *)n + 0x58);
    u = func_800B12F4();
    *(u8 **)(q + 0x28) = u;
    if (u != NULL)
    {
        if (k != 0)
        {
            i = 1;
            col = (u32 *)((u8 *)n + 0x5C);
            *(u32 *)((u8 *)n + 0x70) = (u32)u;
            u = func_800B1434(u, D_80073618[k]);
            has = 1;
            *(u32 *)((u8 *)n + 0x58) = 0x808080;
        }
        else
        {
            has = 0;
        }
        *(s32 *)(q + i * 4 + 0x50) = (s32)func_8004E298(u, D_800DF03C[j], 0);
        i++;
        w = func_80069E98(D_800DF03C[j]) * 8;
        base = has * 10;
        func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : (s16)(base + 0x80),
                      (s16)(-*(s8 *)(u + 0x3) - 0x9));
        func_800B135C(u, e != 0 ? 4 : 8);
        func_800B13CC(u, 0x20);
        func_800B1400(u, 2);
        *col++ = 0x808080;
        u += 0x60;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        tmp = func_800B1434(u, u - 0x60);
        func_800B1320(u, 1, 1);
        u = tmp;
        *col = 0x80808;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        u[0x1] = 0x38;
        v = has == 0 ? -0x2 : -0xA;
        v2 = v;
        if (e != 0)
        {
            v2 = v - 2;
        }
        *(s8 *)(u + 0x2) = v2;
        *(s8 *)(u + 0x3) = -0xE;
        u[0x9] = 0;
        u[0x8] = 0;
        base = w + has * 24;
        if (has == 0)
        {
            base += 4;
        }
        z2 = base;
        if (e != 0)
        {
            z2 = base - 2;
        }
        u[0xA] = z2;
        u[0xB] = 0x10;
        u[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(u + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(u + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(u + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(u + 0x14) = *(s32 *)(q + 0x1C);
        u[0x1] |= 2;
        u += 0x18;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        u[0x1] = 0x81;
        *(s16 *)(u + 0x4) = 0x20;
        *(s16 *)(u + 0x6) = 0;
        u[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}
