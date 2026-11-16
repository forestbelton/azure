/* PARK: func_800A08A0, 189 words against a target of 189, 73 differing
   (masked), 34 aligned differences.  ovl_dungeon, 2026-11-16, agent j41.
   The word count is exact and the shape is exact; every remaining row is a
   register assignment or a delay slot that follows from one.

   THREE NEW DECLARATIONS:

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

   ------------------------------------------- THE FOUR LEVERS THAT MATTERED

   1. **THE DISTANCE IS `abs()`, NOT `if (d < 0) d = -d;`.**  This is
      docs/matching-levers.md's "AN abs() RESULT THAT SHARES A VARIABLE
      ACROSS BLOCKS IS WORTH 8 WORDS" in its word-count form: `abssi2`
      (mips.md) emits its OWN delay slot, so it expands to `bgez / nop /
      negu` and the two `nop`s at 800A0958 and 800A0968 are part of the
      pattern.  Hand-written conditional negation lets the scheduler fill
      both slots with the second subtraction, and that also drags the two
      `lbu`s of D_80082E80+0x24/+0x25 and the stack bytes into the wrong
      blocks.  58 aligned rows -> 37, and it is what flipped $s4/$s5 into
      the target's assignment across the whole function.

   2. **THE PACING LOOP NEEDS A SECOND POINTER LOCAL FOR ITS OWN TEST.**
      Written as one `tb` for both the `D_800835E4[c]` index and the loop
      test, jump2 cross-jumps the duplicated `while` guard away and the
      function is 183 words with a bare `j` to the bottom test where the
      target has the guard spelled out (800A0AE4..800A0AF8).  A separate
      `tp = tb;` inside the guarded arm -- plus a local `g` holding
      `tb[c]`, so the guard compares a REGISTER where the loop test reloads
      `*(u32 *)(q + 0x18)` -- makes the two copies differ, which is what
      find_cross_jump needs to leave them alone.  183 -> 189 words.

   3. **`off` MUST BE `s32` WITH THE `(s16)` WRITTEN AT THE USE.**  As `s16`
      the function is 190 words: the `(n - 2) * 2` arm is hoisted whole into
      $s7, `off` lands in $v0 -- which is the register the `slti` test
      result lives in -- and reorg then cannot put the else arm's `move`
      into the branch's delay slot, so it costs a `nop` and a `j`.  As
      `s32 off` with `D_80083478 + (s16)off` only the `addiu $s7, $s2, -2`
      is hoisted and the count is exact.  189 words.

   4. **`c` IS `(u8)t[1]`, read from the table and not back from
      D_800E2968.**  Reading the global back is a real `lbu` reload here
      (cse does not forward it), where the target has
      `andi $s3, $a0, 0xFF`.  2 rows.

   --------------------------------------------- WHAT IS LEFT: 34 ROWS, 0 WORDS

     * the `off` arms (rows 59-65): the target computes `(n - 2) * 2` WHOLE
       in the preheader ($s7) and still keeps `off` in $v1, so the else arm
       is `addu $v1, $s7, $zero` in the `beqz`'s delay slot and no `j` is
       needed.  We hoist only the `addiu` and emit the `sll` in both arms.
       One word moved, not gained -- the counts match either way.
     * `andi $s3, $a0, 0xFF` vs our `lbu $s3, 0x1($v1)` (2 rows).
     * the pacing tail (rows 135-161): `tb` is $v1 in the target and $a0
       here, which costs the guard's delay slot a `nop` and moves the loop
       test's reload one slot.

   MEASURED AND REJECTED:
     * `if (d < 0) d = -d;` in either statement order:   183-186 words
     * `d1`/`d2` loaded into `u8` locals first:          181-184 words
     * `off = (n-2)*2;` then `if (n < 2) off = ...;`:    186 words / 43 rows
     * the same with `(n-2)*2` hoisted into a local
       outside the loop:                                 190 / 69
     * `off` as a ternary, either operand order:         189 / 34, 187 / 33
     * `(func_800A6D30() & 0xF) << 1`:                   191 / 62
     * `while (...)` instead of `if (...) do {} while`:  183 / 66
     * `g` local without `tp`:                           183 / 66
     * `c = D_800E2968;` / `u8 c` / `s32 c`:             35-37 rows */

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
    s32 off;
    s16 h;
    s16 c;

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
        t = D_80083478 + (s16)off;
        D_800E2968 = t[1];
        c = (u8)t[1];
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
        tb = D_800835E4;
        g = tb[c];
        *(u32 *)(q + 0x18) = g;
        tb++;
        if (g >= tb[q[0x11]])
        {
            tp = tb;
            do
            {
                func_800A1D4C(q, 0);
            } while (*(u32 *)(q + 0x18) >= tp[q[0x11]]);
        }
        if (q[0x49] != 0)
        {
            return;
        }
        *(s32 *)(q + 0x48) = func_800A9230(q);
        return;
    }
}
