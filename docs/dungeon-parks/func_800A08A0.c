/* PARK: func_800A08A0, 189 words against a target of 189, 5 differing
   (masked), 6 aligned differences -- ALL SIX IN ONE FOUR-INSN WINDOW
   (800A0990..800A09B0).  ovl_dungeon, 2026-11-16, agent j81; was 189/73
   masked, 34 rows (agent j41).

   THREE DECLARATIONS (unchanged from j41):

       extern s16 D_8008347C;
       extern u8 *D_80083478;
       extern u32 D_800835E4[];

   NO prototypes are added: func_800A4E2C, func_800A0B94, func_800A152C,
   func_800A1D4C and func_800A9230 are all defined LOWER in the unit and the
   calls ride implicit `int` declarations, exactly as the landed
   func_80095A10 (:7169) and csrc/ovl_dungeon.c:3210 already do.  The
   existing `void func_800A08A0(s32);` at the top of the file is the
   signature.

   ------------------------------------------------------- WHAT IT IS

   Up to 16 attempts to place a spawn.  Each attempt asks func_800A4E2C for a
   tile, rejects it if func_8009FB34 says no; when the argument is 0 it also
   rejects any tile within a Chebyshev-ish 0x20 of D_80082E80+0x24/+0x25
   (for a nonzero argument the distance stays at its initial 0x100 and the
   test always passes).  It then picks a row of the D_80083478 table -- the
   argument's own row for n >= 2, a random one for n < 2 -- publishes its
   second byte in D_800E2968, loads the resource with func_800A1618, and
   builds the entity by CALLING func_800A0B94's RESULT with (0, x, y, and the
   `(s16)` of func_800BCB04's answer), the same idiom the landed
   func_80095A10 uses.  n >= 2 then re-centres the camera and raises
   D_800814A0's 0x8000; n < 2 paces the spawn against D_800835E4 with
   func_800A1D4C and parks func_800A9230's answer at +0x48.

   =============================================== j81's FOUR LEVERS (34 rows -> 6)

   5. **`c` IS A `u8` LOCAL THE STORE GOES THROUGH, NOT A SECOND `t[1]`.**
      `cb = t[1]; D_800E2968 = cb; c = cb;` with `u8 cb` keeps the loaded
      byte in a QImode pseudo, so the store is `sb $a0` and the widening to
      `c` is zero_extendqisi2 ON THAT REGISTER -- the target's
      `andi $s3, $a0, 0xFF`.  j41's `D_800E2968 = t[1]; c = (u8)t[1];`
      reloads instead (`lbu $s3, 0x1($v1)`): cse does not forward the second
      read across the store to the global.  2 rows, +1 word (paid for by 6).

   6. **`off` IS `s16`, WHICH IS WHAT MAKES loop.c HOIST THE WHOLE
      `(n - 2) * 2`.**  With `s32 off` the shift writes `off` directly, `off`
      has two sets, and only `addiu $v0, $s2, -0x2` hoists -- the `sll` is
      then emitted in both arms plus a `j`.  With `s16 off` the assignment
      expands as `temp = (n-2)*2; off = (HI)temp`: `temp` is a non-user
      pseudo with ONE set, and loop.c moves it to the preheader as the
      target's `addiu $v0, $s2, -0x2 / sll $s7, $v0, 1`.  Measured, not
      inferred -- the -dL dump prints the decision.

      WHY it hoists is worth keeping, because it decides where the
      statement may go.  It is NOT the desirability test: the temp's life is
      2 and savings 2, and `threshold * savings * lifetime` = 132 against
      the loop's 158 insns, which loop.c prints as `not desirable`.  It
      moves because force_movables (loop.c:1320) hands the movable that is
      the LAST USE of `(s32)n` the free move at loop.c:1731
      (`m->forces->done && n_times_used[forces->regno] == 1`, and
      n_times_used is a COPY of n_times_SET, so the sign extension's 1 set
      satisfies it).  In the if/else form the else arm's `n - 2` IS that
      last use.  Write it as `off = (n-2)*2;` followed by
      `if (n < 2) off = ...;` and the `slti` becomes the last use instead,
      so the slti hoists and the mult does not -- AND cse then merges that
      slti with the `n >= 2` test 0xD8 bytes later (cse_end_of_basic_block's
      skip-blocks arm, cse.c:8197, extends the path AROUND the call arm
      because the arm has no label and the join label has one use), giving
      it life 68 and a callee-saved register.  187 words / 40 rows.

   7. **THE TABLE POINTER IS LOADED INTO `t` BEFORE THE INDEX IS ADDED:
      `t = D_80083478; t += (s16)off;` NOT `t = D_80083478 + (s16)off;`.**
      Written as one expression, `t` is born at the `addu` and the %hi/%lo
      pair and the sign extension are both block-local temps; the sign
      extension wins the local-alloc tie for $v0, the pointer takes $v1, and
      `off` -- which is GLOBAL, set in both arms and used in the join block
      -- conflicts with the pointer and so lands in $v0, THE REGISTER THE
      `slti` RESULT IS IN.  reorg may not put an insn that writes a
      resource the branch READS into its delay slot, so it cannot steal the
      else arm's `move`: the arm then costs a `nop`, a `j` and a duplicated
      `sll`.  191 words / 14 rows.  Splitting the statement moves `off` off
      $v0, the steal fires, the `j` goes away, and the count is exact.

   8. **THE PACING BLOCK INDEXES THE ARRAY DIRECTLY AND STARTS THE POINTER
      AT [1], AND THE DO-WHILE TEST IS WRITTEN WITH THE TABLE ON THE LEFT.**

          g = D_800835E4[c];
          *(u32 *)(q + 0x18) = g;
          tb = &D_800835E4[1];
          if (g >= tb[q[0x11]])
          {
              tp = tb;
              do { func_800A1D4C(q, 0); } while (tp[q[0x11]] <= *(u32 *)(q + 0x18));
          }

      This is worth FIFTEEN rows and it is two separate facts.

      (a) `tb = D_800835E4; g = tb[c]; ... tb++;` makes `tb` the live
      pointer across the whole block, and because `tp = tb` sits in the
      guarded arm `tb` is referenced in two blocks, so flow marks it
      REG_BLOCK_GLOBAL and global-alloc gets it only after local-alloc has
      given `g` $v1 (the greg dump prints `86 conflicts: ... 2 3 29`), so
      `tb` lands in $a0.  $a0 is then not free for the guard branch's delay
      slot, which the target fills with the call's own `addu $a0, $s0,
      $zero`, so the branch gets a `nop` and the loop test's reload slides a
      slot.  Reading the first element off the ARRAY and giving the variable
      the +1 address makes the base a block-local temp, `g` loses the tie
      and takes $a0, and `tb` gets $v1 -- the target's assignment exactly.

      (b) `while (tp[q[0x11]] <= *(u32 *)(q + 0x18))` instead of
      `while (*(u32 *)(q + 0x18) >= tp[q[0x11]])` puts the TABLE load first
      in the RTL, which is the order the target emits, and the `q + 0x18`
      reload second -- where it writes $v0, the register the index chain
      uses, so sched2 cannot float it up into the `lbu`'s load-delay slot.
      With the operands the other way round the reload is independent, moves
      into that slot, and the function loses the `nop` the target has
      (-1 word).  The `sltu $v0, $v0, $v1` operand order is identical both
      ways.

      Note that `g` is NOT load-bearing: `*(u32 *)(q + 0x18) = D_800835E4[c];`
      with the guard reading the global back measures the same 189 / 6,
      because cse forwards the stored register into the guard while the call
      inside the loop forces the loop test to reload.  j41's lever 2 is
      right about WHY the guard and the loop test must differ; the variable
      is one way of getting there, not the only one.

   ========================================= j41's LEVERS, STILL LOAD-BEARING

   1. **THE DISTANCE IS `abs()`, NOT `if (d < 0) d = -d;`.**  `abssi2`
      (mips.md) emits its OWN delay slot, so it expands to `bgez / nop /
      negu` and the two `nop`s at 800A0958 and 800A0968 are part of the
      pattern.  Hand-written conditional negation lets the scheduler fill
      both slots and drags the two `lbu`s into the wrong blocks: 58 rows
      -> 37, and it is what put $s4/$s5 in the target's assignment.

   2. **THE PACING LOOP NEEDS ITS OWN POINTER LOCAL FOR THE INNER TEST.**
      One pointer for both the guard and the loop test lets jump2
      cross-jump the duplicated guard away: 183 words with a bare `j` where
      the target spells the guard out (800A0AE4..800A0AF8).  `tp = tb;`
      inside the guarded arm is what makes the two copies differ -- and it
      must stay INSIDE the arm: moved above the `if` it is in the same basic
      block as its source, optimize_reg_copy_1 (local-alloc.c:704) rewrites
      the guard's `tb` to `tp`, `tb` dies at the copy, local-alloc coalesces
      the two and the copy evaporates.  187 words.

   3. **`off` CARRIES THE `(s16)` AT THE USE** -- j41 measured it with
      `s32 off`; j81's lever 6 keeps the cast and narrows the variable.

   4. **`c` IS READ FROM THE TABLE, NOT BACK FROM D_800E2968** -- see 5.

   ========================================== WHAT IS LEFT: 6 ROWS, 0 WORDS

   Every one is in 800A0990..800A09B0, and they are one fact:

       target                          candidate
       addu $v1, $s7, $zero            move a0,s7          <- off is $a0
       sll  $v1, $v0, 1                sll  a0,v0,0x1
       lui  $v0, %hi(D_80083478)       lui  v0,0x0
       sll  $v1, $v1, 16               lw   v1,0(v0)
       lw   $v0, %lo(D_80083478)($v0)  sll  v0,a0,0x10
       sra  $v1, $v1, 16               sra  v0,v0,0x10
       addu $v1, $v1, $v0              addu v1,v1,v0

   In the target `off`, the sign-extension temp and `t` all share $v1 and
   the %hi/%lo temp has $v0 to itself: the `sll` of the sign extension is
   scheduled BEFORE the `lw`, so `off` dies before the pointer is loaded and
   conflicts only with the %hi temp.  Here the split `t = D_80083478;`
   gives the `lw` a higher sched1 priority than the `sll` (its dest is the
   user variable `t`, and the load's cost beats the shift's), so the
   pointer is in $v1 from the `lw` onwards, `off` conflicts with both $v0
   and $v1, and global-alloc pushes it to $a0.  It is one scheduling
   choice, worth zero words.  The five masked words are 60, 64, 66, 67 and
   68; word 65 (`lui ... %hi(D_80083478)`) already agrees, and the only
   relocated word among the five is 67's `lw`, whose difference is its
   destination register and not its %lo.

   This is the shape to permute: the window is four insns wide and the word
   count is already exact.

   MEASURED AND REJECTED (j41, do not repeat):
     * `if (d < 0) d = -d;` in either statement order:   183-186 words
     * `d1`/`d2` loaded into `u8` locals first:          181-184 words
     * `off` as a ternary, either operand order:         189 / 34, 187 / 33
     * `(func_800A6D30() & 0xF) << 1`:                   191 / 62
     * `c = D_800E2968;` / `u8 c` / `s32 c`:             35-37 rows

   MEASURED AND REJECTED (j81, all on top of levers 5-8 unless noted):
     off:
     * `s16 off` with the if/else arms, one-expression
       table access (i.e. lever 6 without lever 7):      191 / 14
     * `off = (n-2)*2;` then `if (n < 2) off = ...;`:     190 / 40
     * the same with `((s16)a - 2) * 2`:                  194 / 60
     * if/else with `((s16)a - 2) * 2`:                   195 / 62
     * a pre-loop `s32 m = (n - 2) * 2;` with `off = m`
       in the else arm (j41's "hoisted into a local"):    194 / 65
       -- the slti takes $s7, `m` is pushed to $s8 and
          the prologue grows by the extra save/restore
     * `m = (n - 2) * 2;` as the first statement INSIDE
       the loop with `off = m` in the else arm:           187 / 40
       -- an in-loop user variable used in another block
          fails all three arms of scan_loop's gate
          (loop.c:786-799), so nothing hoists
     table access (all with levers 5, 6, 8 in place):
     * `t = D_80083478 + (s16)off;`                       191 / 14
     * `t = (s16)off + D_80083478;`                       191 / 14
     * `t = &D_80083478[(s16)off];`                       191 / 14
     * `so = (s16)off; t = D_80083478 + so;`              191 / 18
     * `u8 *bp = D_80083478; t = bp + (s16)off;`          191 / 31 (pre-8)
     * no `t` at all, `D_80083478[(s16)off + 1]` and
       `D_80083478[(s16)off]` indexed separately:         191 / 12
     * `t = D_80083478; t = t + (s16)off;`                189 / 6  (identical)
     * `so = (s16)off;` before or after the load,
       `s32 so` or plain `off`:                           189 / 6  (identical)
     pacing block:
     * j41's `tb = D_800835E4; g = tb[c]; tb++;` with
       `*(q+0x18) >= tp[...]`:                            189 / 23
     * the same with the loop test reversed:              190 / 26
     * `tp = tb;` moved above the `if`:                   187 / 23
     * `g` assigned in the loop body and tested there:    188 / 23
     * no `tp`, the loop using `tb`:                      188 / 24
     * a plain `while` with no `g`/`tp`:                  188 / 24
     * `g = 0;` at the end of the arm (an attempt to make
        `g` global for flow): no effect at all, 189 / 23
        -- a dead store cannot do it, as the catalogue says */

void func_800A08A0(s32 a)
{
    u8 x;
    u8 y;
    u8 *t;
    u8 *q;
    u8 *w;
    u32 *tb;
    u32 *tp;
    u32 g;
    s32 e;
    s32 v;
    s32 d1;
    s32 d2;
    s16 dist;
    s16 n;
    s16 k;
    s16 off;
    s16 h;
    s16 c;
    u8 cb;

    dist = 0x100;
    if ((D_800E296C & 0x10000000) != 0)
    {
        return;
    }
    if (D_8008347C >= 0x20)
    {
        return;
    }
    n = a;
    for (k = 0; k < 0x10; k++)
    {
        func_800A4E2C(&x, &y);
        if (func_8009FB34(x, y) < 0)
        {
            continue;
        }
        if (n == 0)
        {
            dist = abs(D_80082E80[0x24] - x) + abs(D_80082E80[0x25] - y);
        }
        if (dist < 0x21)
        {
            continue;
        }
        if (n < 2)
        {
            off = (func_800A6D30() & 0xF) * 2;
        }
        else
        {
            off = (n - 2) * 2;
        }
        t = D_80083478;
        t += (s16)off;
        cb = t[1];
        D_800E2968 = cb;
        c = cb;
        h = t[0];
        if (h == 0)
        {
            return;
        }
        e = (s32)func_800A1618(h, 1);
        if (e == 0)
        {
            continue;
        }
        v = func_800BCB04((x << 6) | 0x20, (y << 6) | 0x20, -0x400);
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(h, (u8 *)e, 1))(0, x, y, (s16)v);
        if (q == NULL)
        {
            return;
        }
        func_800A152C(h, 1);
        func_80042640((Unk42710 *)q, h);
        q[0x43] = func_800A6D30();
        q[0x12] = 0;
        if (n >= 2)
        {
            w = *(u8 **)(q - 0x14);
            func_8009A3D0(w[0x24], w[0x25], 0x3000);
            func_800A32A4(q);
            func_8009A028(q);
            *(u16 *)(q - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        g = D_800835E4[c];
        *(u32 *)(q + 0x18) = g;
        tb = &D_800835E4[1];
        if (g >= tb[q[0x11]])
        {
            tp = tb;
            do
            {
                func_800A1D4C(q, 0);
            } while (tp[q[0x11]] <= *(u32 *)(q + 0x18));
        }
        if (q[0x49] != 0)
        {
            return;
        }
        *(s32 *)(q + 0x48) = func_800A9230(q);
        return;
    }
}
