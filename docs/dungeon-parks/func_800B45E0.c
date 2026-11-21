/* PARK: func_800B45E0 -- 203 of 203 words, **9 ALIGNED DIFFERENCES**.

   ================= 2026-11-20, SECOND PASS: 30 rows -> 9, and (A) IS CLOSED

   Two more devices, and the first came out of decomp-permuter (base 900 ->
   290 over 141737 candidates):

   1. **`i = t >> 1;` HOISTED INTO ITS OWN LOCAL, BEFORE the `>> 0x11`**, with
      the store then reading `i`:

          t = *(s32 *)(b + 0x14);
          i = t >> 1;
          v = t >> 0x11;
          if (v < 0) { v = -v; }
          *(s32 *)(b + 0x14) = i;

      That is cluster (C) below, and it is exactly what the target's word
      order says -- `sra $v1, $v0, 1` at 154 BEFORE `sra $v0, $v0, 17` at 155.
      Written as `*(s32 *)(b + 0x14) = t >> 1;` at its natural place the shift
      is emitted after the test and the store lands outside the `beqz`'s delay
      slot.  Worth 14 rows on its own: 30 -> 16, at 202 words.
      Note it REUSES `i`, the ramp's numerator, so `i` has two sets.  That is
      the permuter's own choice and a fresh local measures the same.

   2. **A `do { } while (0);` AROUND CASE 4's DECREMENT AS WELL**, which is
      the other half of cluster (A) -- the counter store sinking past its own
      sign extension -- and which this park had recorded as costing a word.
      It does, and after device 1 that word is exactly the one missing: 202
      -> 203, and 16 rows -> 9.
      **So (A) is closed, and the lesson is that the two halves had to be
      bought in the right order.** The park's own note said the barrier "fixes
      the order and the problem is paying for it inside the word count"; the
      answer was that something else had to give the word back first.

   WHAT IS LEFT, 9 rows, ONE REGISTER: the loaded byte `j = c[0xE]` is in $a3
   where the target has $v1, and the ramp's result is in $v0 where the target
   reuses $v1 -- `lbu $v1, 0xE($s2)` / `negu $v0, $v1` / `addu $v1, $v1, $v0`
   / `sb $v1, 0xE($s2)`.  Making the result share `j`'s variable does NOT do
   it: `j = j + i / n;`, `j += i / n;` and `k = j; k += i / n;` are all 9 and
   all leave `j` in $a3.  So it is `j`'s own allocation priority, not the
   sharing, and that is the next thing to price off `.lreg`.
   2026-11-20.  The body below carries the two devices a beam search found:
   a `do { } while (0);` around case 1's counter decrement (37 -> 33) and a
   duplicated-arm `if (j)` around `*(u8 **)p = NULL;` (33 -> 30).  Neither is
   guessable and the ladder went 37 -> 36 -> 34 -> 33 -> 32 -> 30 over 2904
   measurements; the sweep is every statement position times {do-while,
   duplicated-arm `if` for each of 10 locals}, two deep, on a
   `tools/decomp/onefunc.py` file.

   One of the seven 4-mod-8 jump-table functions: it owns `jtbl_800892CC` at
   rodata offset 0xB6C.  **It cannot be landed until the unit is carved**
   whatever its residue -- see docs/dungeon-parks/func_800D50FC.c and
   docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8".

   THE WORD COUNT IS EXACT AND THE STRUCTURE IS RIGHT.  Everything left is
   instruction order and register names, in three clusters.

   SHAPE, as read off the listing rather than guessed:
     * five cases (`sltiu $v0, $v1, 0x5`), case 0 FALLING THROUGH into case 1;
     * cases 1 and 4 share the colour-ramp block -- the target jumps into it
       from case 1 at `j .L800B47D0` with the numerator already in $v0 -- so
       the body reaches it with a `goto ramp;` and `break`s out of case 4;
     * the ramp's two inputs are the LOADED byte `j = c[0xE]` (kept in $v1)
       and the numerator `i` (0x80 - j for case 1, -j for case 4), and the
       result is `j + i / n` with j FIRST: `addu $v1, $v1, $v0`;
     * cases 1 and 4 test the counter through a SIGN EXTENSION
       (`sll`/`sra $a0` then `blez $a0`), while CASE 3 TESTS THE SHIFTED
       VALUE DIRECTLY (`sll` then `bgtz`, no `sra`) -- so case 3 is written
       `if ((s32)(m << 16) <= 0)` and the other two `n = (s16)m; if (n > 0)`.
       That asymmetry is in the target, not a mistake;
     * `p + 0xE` is read with `lh`, a HALFWORD, not a byte;
     * cases 1-else and 3-then share the `sh $v0, 0x8($s0)` store, which
       jump2 cross-jumps -- both are written `*(u16 *)(p + 0x8) += 1;`.

   WHAT IS LEFT, 37 rows in three clusters
   ---------------------------------------
   (A) THE COUNTER STORE SINKS PAST ITS OWN SIGN EXTENSION, twice (cases 1
       and 4).  The target is `addiu $v0, -1` / `sh $v0, 0xA($s0)` /
       `sll $v0, $v0, 16` / `sra $a0, $v0, 16`, and note that the `sll`
       OVERWRITES $v0 -- which is only possible because the store has already
       happened.  Ours shifts first into fresh registers ($v1, $a1) and
       stores after, so the stored value stays live and everything downstream
       shifts a register.  That is sched1 doing what it always does with a
       store: `schedule_select` promotes the insn with the largest
       `potential_hazard`, which is nonzero only for the "memory" unit, so a
       ready store is taken earliest in the BACKWARD pass and therefore
       placed LATEST (the rule stated in docs/dungeon-parks/func_800B23F8.c).
       Six spellings of the decrement trio were measured and ALL are 37:
       `m` u16 + `(s16)m`; the compound `n = (s16)(*(u16 *)(p+0xA) -= 1)`;
       decrement then RELOAD `n = *(s16 *)(p + 0xA)` (cse forwards it);
       load-then-decrement 41; a second local between them; `s16` for the
       temp; `(s32)(m << 16) >> 16` for the extension.
       A `do { } while (0);` around case 1's decrement IS worth 4 rows (33)
       and keeps 203 words -- it is in the body below.  Around case 4's it
       costs a word (204/31), and around BOTH it costs a word (204/27 for the
       store, 204/23 for the extension).  So the barrier fixes the order and
       the problem is paying for it inside the word count; case 4's half of
       (A) is still open and is the next thing to attack.
   (B) `abs()` IS NOT THE ANSWER HERE, which is worth recording because the
       target LOOKS like it: both absolute values come out as
       `bgez` / `nop` / `negu` with an empty delay slot, which is exactly
       `abssi2`'s own pattern (the lever written against func_800A08A0).
       Spelled with `abs()` this function measures 39, against 37 for the
       hand-written `if (v < 0) { v = -v; }`.  Do not "fix" it to abs().
   (C) the `sra $v1, $v0, 1` that stores b+0x14 is computed late and lands
       outside the `beqz`'s delay slot, where the target has it at word 154
       and the store in the slot at 161.

   DECLARATIONS
   extern u8 D_80044C54[];
   END DECLARATIONS
   (func_8004491C and D_800814A0 are already declared.)

   Measured with
     python3 tools/decomp/parkresidue.py func_800B45E0
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

    q = *(u8 **)p;
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        if (j)
        {
            *(u8 **)p = NULL;
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
            i = 0x80 - j;
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
            i = -j;
        ramp:
            k = j + i / n;
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
