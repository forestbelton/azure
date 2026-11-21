/* PARK: func_800B45E0 -- 203 of 203 words, **2 ALIGNED DIFFERENCES**.

   Previous park: 203/203 words, 6 rows.  This run: 2 rows, and the residue
   now has a named mechanism and a stated open question.

   =================== 2026-11-21: 6 rows -> 2, AND THE MECHANISM IS NAMED

   The six rows were one allocation conflict and the previous park said so.
   It also said the conflict was not a priority problem, and that was right.
   What it did not have is the CAUSE.  The cause is the DEAD MOD OUTPUT of
   `divmodsi4` -- an instruction that is never emitted, holding a hard
   register.

   THE MECHANISM, read off local-alloc.c, global.c and mips.md and confirmed
   in `.lreg`/`.greg`:

     * `u / n` at -O2 always expands to `divmodsi4` (mips.md:1505).  `divsi3`
       (mips.md:1605) is guarded `!optimize`, genopinit emits
       `if (HAVE_divsi3)`, so `sdiv_optab` is CODE_FOR_nothing at -O2 and
       expmed.c:3671 falls through to the twoval form.  The pattern has TWO
       register outputs; with `REG_UNUSED` on the MOD the template prints
       only `div\t%0,%1,%2`, but the MOD is a real pseudo with a real
       register.  (flow.c:2156 gives it `reg_n_deaths++` == 1, and its one
       reference is in one block, so local-alloc.c:472's gate accepts it:
       it is LOCAL-allocated, and `.lreg` shows it, e.g. "Register 155 in 2".)
     * `qty_compare` (local-alloc.c:1698) ranks a quantity by
       `floor_log2(n_refs)*n_refs*size / (death-birth)`, which is 0 for the
       1-reference MOD, so it is allocated AFTER everything else local in its
       block; `find_free_reg` (:2181) ORs `regs_live_at[]` over its range and
       takes the LOWEST free hard reg (MIPS defines no `REG_ALLOC_ORDER`).
       `reg_is_set` gives it birth `2*N` and `wipe_dead_reg(reg, 1)` death
       `2*N+1`, so its range is the single slot `2*N` at the div.
     * `global_conflicts` processes REG_DEAD notes BEFORE `note_stores`
       (global.c:724 then :731).  So the numerator and the divisor, which die
       at the div, do NOT conflict with either output -- that is why the
       numerator can share the quotient's register, as the target does.  But
       anything LIVE ACROSS the div does conflict: when a locally-allocated
       pseudo becomes live, `mark_reg_store` -> `record_one_conflict`
       (global.c:1246) sets that hard bit in EVERY live allocno.

   THREE SPELLINGS, THREE DIFFERENT RESIDUES, all 203 words
   --------------------------------------------------------
     (1) quotient is a BLOCK-LOCAL temp -- `k = j + i / n;` (the old park).
         quotient -> $v0, MOD -> $v1.  `j` is live across the div, so it
         conflicts with BOTH and lands in $a3.                     6 rows
         `88 conflicts: 80 81 82 83 84 85 88 89 2 3 4 5 6 29 64 65 66`
     (2) quotient written into the GLOBAL pseudo that is ALSO the numerator --
         `u = u / n; k = j + u;`.  The MOD is then the only local in the
         block and takes $v0, so `j` gets $v1, `k` gets $v1 and `n` gets
         $a0 -- but `u` is live when the MOD becomes live, so it inherits
         the MOD's hard-2 conflict and lands in $a3.                5 rows
     (3) quotient written into a SEPARATE global pseudo -- `w = u / n;
         k = j + w;` with `u` the numerator.  Now the numerator DIES at the
         div and does not inherit the conflict, so it takes $v0 as well.
         `j` -> $v1, `k` -> $v1, `n` -> $a0, numerator -> $v0.      2 rows
         `88 conflicts: 80 81 82 85 88 91 92 2 29 64 65 66`   (no hard 3)
         `91 conflicts: 80 81 82 85 88 91 29 64 65 66` + `preferences: 2`

   THE BODY BELOW IS (3).  The two rows left are the quotient `w`:

     ours    mflo a3      addu v1,v1,a3
     target  mflo $v0     addu $v1, $v1, $v0

   and the reason is one line of the dump:

     92 conflicts: 80 81 82 83 88 89 92 **2** 4 5 6 29 64 65 66

   hard 2 is the MOD's.  `note_stores` walks a PARALLEL BACKWARDS, so the
   MOD's set is processed before the quotient's; by the time
   `record_one_conflict` runs for the quotient, the MOD's hard reg is
   already in `hard_regs_live`.  **A global quotient can therefore never
   share the MOD's register, whichever order they are written in.**

   THE OPEN QUESTION, STATED PRECISELY
   -----------------------------------
   The target has the quotient in $v0 AND `j` in $v1, so the MOD must be in
   a register >= $a0.  For that, `find_free_reg` has to see hard 2 AND hard 3
   busy over the MOD's single slot `2*N`.  The only things that can be in
   `regs_live_at[2*N]` are (a) real hard registers live there -- none is, the
   div's own clobbers are 64/65/66 -- or (b) other LOCAL quantities whose
   `[birth, death)` covers slot `2*N`.  A local that DIES at the div has
   death `2*N` and so does NOT cover the slot; a local born at the div is the
   quotient itself; and the div is the FIRST insn of the ramp block (the
   `goto ramp;` label is on it), so nothing local can be born before it.
   Any local that did cover the slot would itself put its hard register into
   `j`'s conflicts.  **So within this reading the target's assignment is
   unreachable, and since the target was built by this compiler the reading
   must be incomplete somewhere.  Finding where is worth the next run's
   first hour.**  Three concrete places to look:
     * a way to give the MOD pseudo a SECOND death, which would fail
       local-alloc.c:472's `reg_n_deaths == 1` and send it to global-alloc,
       where its priority is 0 (`allocno_compare`, 1 ref) so it would be
       allocated LAST and take a leftover register.  That is the same gate the
       func_800A94A0 asm-device lever turns (docs/matching-levers.md, "the
       dead set of `g` ... dies in 2 places").  A dead `t = u % n;` does NOT
       do it -- see the negatives below -- because the store is deleted
       outright.
     * a way to give the MOD's quantity `qty_n_calls_crossed > 0`, which
       switches `find_free_reg` from `fixed_reg_set` to `call_used_reg_set`
       and forces it into $s*.
     * a second local quantity in the ramp block whose range covers the div
       and which does not extend over `j` -- i.e. the premise above that no
       such range exists is the one to attack.

   =================== WHAT WAS MEASURED THIS RUN (203 words unless noted)

   RAMP SPELLINGS, with the old park's `if (j) { if (k) ... }` device:
     `k = j + i / n;`  (the old park)                               6
     `k = j; k += i / n;`                                           6
     `j = j + i/n;` with the three stores from `j`                  9
     `i = i / n; k = j + i;`                                        7
     `n = i / n; k = j + n;`                                       20
     `t = i / n; k = j + t;`                              202 words 27
     `v = i / n; k = j + v;`                              202 words 31
     separate numerator `u`, quotient still local                  12
     separate numerator `u` declared first                         12
     separate numerator + merge j,k                                15
     `u = u / n; k = j + u;`                                        7
     `u /= n;  k = j + u;`                                          7
     `u = u / n; j = j + u;` (stores from j)                       10
     `i = u / n; k = j + i;`                                       13
     `t = u / n; k = j + t;`                              202 words 27
     `v = u / n; k = j + v;`                              202 words 31
   (NB: the old park recorded `i = i / n; k = j + i;` as 6.  On the park body
   as it stood it measures 7 here.  Reported as measured.)

   THE DUPLICATED-ARM DEVICE IS LOAD-BEARING AND ITS CONDITION VARIABLES
   CHANGE WITH THE RAMP SPELLING.  Every single- and nested-arm condition
   over every local was swept on each shape (1 + N + N*N candidates each):
     on `k = j + i/n;`   (shape 1)  best `if (j){if (k)}`            6
     on `u = u / n; ...` (shape 2)  best `if (k){if (u)}`            5
                                         `if (j){if (k)}`            7
     on `w = u / n; ...` (shape 3)  best `if (k){if (w)}`            2
                                         `if (w){if (k)}`            2
                                         `if (i|m|q){if (w)}`        5
                                         `if (w){if (w)}`, `if (w)`  5
                                         `if (j){if (k)}`           12
     no device at all, on every shape                            >= 10
   The device is what makes `k` a global pseudo (so global-alloc can give it
   $v1), and the second arm's condition is what makes the QUOTIENT variable
   global (shape 3).  That is the whole reason the old park's `if (j)` arm
   had to go: `j` and `k` both read at the head of the function made them
   conflict, and `j` has to be free to share $v1 with `k`.

   CLEAN NEGATIVES ON THIS 2-ROW BODY -- do not repeat:
     * widthsweep.py   -- every declared width of every local: no change helps.
     * devsweep.py     -- 63 statements x 13 names = 882 wrappers: none helps.
     * refsweep.py     -- 13 names x 1440 insertion points of `v++; v--;`:
                          none helps.
     * stmtsweep.py    -- 10 movable assignments: no safe move helps.
     * DECLARATION ORDER IS INERT: `u` and `w` moved through every position
       of the declaration block x four device pairs, 360 candidates, all 2.
     * A DEAD `%` IS DELETED AND BUYS NOTHING: `t = u % n;`, `v = u % n;`,
       `i`, `m`, `n`, `u` and `w` as the dead remainder's destination, before
       and after the division and after the add, 7 x 7 device pairs -- every
       one is 203 words and 2 rows, byte-identical to the body below.  So the
       remainder store is removed before local-alloc and does NOT give the MOD
       pseudo a second death.
     * also 2 and inert: `k = w + j;` (operand order), `k = j; k += w;`,
       `k = j;` before the division, `w = u; w /= n;`, `k = (s16)(j + w);`,
       `j += w;` with the stores from `j`, `u = w;` or `t = w;` after the add.

   =================== EARLIER PARK TEXT, STILL VALID

   One of the seven 4-mod-8 jump-table functions: it owns `jtbl_800892CC` at
   rodata offset 0xB6C.  The unit HAS been carved (this function lives in
   csrc/ovl_dungeon_800B45E0.c), so that is no longer a bar to landing it.

   SHAPE, as read off the listing rather than guessed:
     * five cases (`sltiu $v0, $v1, 0x5`), case 0 FALLING THROUGH into case 1;
     * cases 1 and 4 share the colour-ramp block -- the target jumps into it
       from case 1 at `j .L800B47D0` with the numerator already in $v0 -- so
       the body reaches it with a `goto ramp;` and `break`s out of case 4;
     * cases 1 and 4 test the counter through a SIGN EXTENSION
       (`sll`/`sra $a0` then `blez $a0`), while CASE 3 TESTS THE SHIFTED
       VALUE DIRECTLY (`sll` then `bgtz`, no `sra`) -- so case 3 is written
       `if ((s32)(m << 16) <= 0)` and the other two `n = (s16)m; if (n > 0)`.
       That asymmetry is in the target, not a mistake;
     * `p + 0xE` is read with `lh`, a HALFWORD, not a byte;
     * cases 1-else and 3-then share the `sh $v0, 0x8($s0)` store, which
       jump2 cross-jumps -- both are written `*(u16 *)(p + 0x8) += 1;`.

   DEVICES IN THE BODY BELOW, all measured and all still needed:
     * `do { m = *(u16 *)(p + 0xA) - 1; } while (0);` in case 1 AND case 4 --
       the counter store sinking past its own sign extension.  Buying only
       one of the two costs a word; both together are 203 words.
     * `i = t >> 1;` hoisted into its own statement BEFORE the `>> 0x11`,
       with the store reading `i`.  Worth 14 rows when it was found.
     * the duplicated-arm `if (k) { if (w) ... }`, which is what makes `k`
       and `w` global pseudos.
     * `abs()` IS NOT THE ANSWER: both absolute values come out as
       `bgez`/`nop`/`negu`, which is `abssi2`'s own pattern, but spelled with
       `abs()` this function measured 2 rows WORSE.  Do not "fix" it.
     * THE OLD PARK'S "THE SHARING IS CORRECT, DO NOT CLEAN IT UP" NOTE ABOUT
       `i` IS NOW WRONG, and that matters: it was true of the old ramp
       spelling, where splitting `i` cost 12 and 32 rows.  In THIS body the
       ramp numerator is `u`, the quotient is `w`, and `i` keeps only the
       `t >> 1` and colour-constant duty.  Splitting is what buys the
       numerator its $v0.

   EXHAUSTED, DO NOT REPEAT
     * decomp-permuter on the SIX-row body: 128,048 candidates, 45 minutes,
       5 jobs, found=none.  **It has NOT been run on this 2-row body, and
       this is a different basin -- it is now the obvious next step.**
     * the previous run's beam over free-reference devices: 11,316
       candidates, depth 3, plateaued at 6 (on the old ramp spelling).
     * raising `j`'s priority: `do { j = c[0xE]; } while (0);` takes it from
       6 refs to 8 across the same 14 insns, third of eleven globals,
       CONFIRMED in `.lreg`, and moves no row.  Priority was never the lever;
       it was a hard-register conflict, and the conflict came from the MOD.

   DECLARATIONS
   extern u8 D_80044C54[];
   END DECLARATIONS
   (func_8004491C and D_800814A0 are already declared.)

   Measured with
     python3 tools/decomp/parkresidue.py func_800B45E0      -> 2 rows, 203/203
     python3 tools/decomp/ovlverify.py <unit copy> func_800B45E0
       -> func_800B45E0: target 203 words, candidate 203 words, 2 differing
*/
void func_800B45E0(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    s32 i;
    s32 n;
    s32 t;
    s32 v;
    s32 j;
    s16 k;
    u16 m;
    s32 u;
    s32 w;

    q = *(u8 **)p;
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        if (k)
        {
            if (w)
            {
                *(u8 **)p = NULL;
            }
            else
            {
                *(u8 **)p = NULL;
            }
        }
        else
        {
            *(u8 **)p = NULL;
        }
        *(u8 **)(p + 0x14) = NULL;
    }
    if (*(u8 **)(p + 0x14) != NULL)
    {
        *(u16 *)(b + 0x2) -= *(u16 *)(p + 0x18) - *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(p + 0x18) = *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(b + 0x6) -= *(u16 *)(p + 0x1A) - *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(p + 0x1A) = *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(b + 0xA) -= *(u16 *)(p + 0x1C) - *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
        *(u16 *)(p + 0x1C) = *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
    }
    switch (*(s16 *)(p + 0x8))
    {
    case 0:
        *(s32 *)(b + 0x14) = 0x200000;
        *(u16 *)(b + 0xA) -= 0x40;
        *(s32 *)(p + 0x4) = 0x200000;
        *(u16 *)(c + 0x1E) = 0x1000;
        *(u16 *)(c + 0x1C) = 0x1000;
        func_8004491C(p - 0x20, (u32)&D_80044C54);
        *(u16 *)(p + 0xA) = 0x10;
        *(u16 *)(p + 0x8) += 1;
    case 1:
        do
        {
            m = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = m;
        n = (s16)m;
        if (n > 0)
        {
            j = c[0xE];
            u = 0x80 - j;
            goto ramp;
        }
        *(u16 *)(p + 0x8) += 1;
        break;
    case 2:
        break;
    case 3:
        m = *(u16 *)(p + 0xA) - 1;
        *(u16 *)(p + 0xA) = m;
        if ((s32)(m << 16) <= 0)
        {
            *(u16 *)(p + 0xA) = 8;
            *(u16 *)(p + 0x8) += 1;
        }
        break;
    case 4:
        do
        {
            m = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = m;
        n = (s16)m;
        if (n > 0)
        {
            j = c[0xE];
            u = -j;
        ramp:
            w = u / n;
            k = j + w;
            c[0xE] = k;
            c[0xD] = k;
            c[0xC] = k;
            break;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
    *(u16 *)(p + 0xC) += 1;
    if ((*(u16 *)(p + 0xC) & 1) != 0)
    {
        t = *(s32 *)(b + 0x14);
        i = t >> 1;
        v = t >> 0x11;
        if (v < 0)
        {
            v = -v;
        }
        *(s32 *)(b + 0x14) = i;
        if (v < 2)
        {
            t = -(*(s32 *)(p + 0x4) >> 1);
            *(s32 *)(p + 0x4) = t;
            *(s32 *)(b + 0x14) = t;
            v = *(s16 *)(p + 0x6);
            if (v < 0)
            {
                v = -v;
            }
            if (v < 2 && *(s16 *)(p + 0x8) < 3)
            {
                *(s16 *)(p + 0x8) = 3;
                *(u16 *)(p + 0xA) = 8;
            }
        }
    }
    if (*(s16 *)(p + 0xE) != 0)
    {
        i = 0x2CF0F0F0;
        if ((*(u16 *)(p + 0xC) & 3) != 0)
        {
            i = 0x2C404040;
        }
        *(s32 *)(c + 0xC) = i;
    }
}
