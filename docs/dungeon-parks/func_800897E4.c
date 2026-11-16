/* PARK: func_800897E4, 173 of 175 words, 50 aligned differences.
   ovl_dungeon.  2026-11-16, agent j41, improving the 165/103 park.

   TWO NEW DECLARATIONS (beside the existing `extern u32 D_800E296C;`):

       extern u32 D_800DCF80[];
       extern u32 D_800DCFA4[];

   `D_80083360[0x20]` and `D_800833E0[0x20]` are already declared with their
   sizes in include/structs.h:1814-1815.  **DO NOT ADD A PROTOTYPE** for
   func_800897E4: csrc/ovl_dungeon.c:2075 calls it seven lines above the
   INCLUDE_ASM and the call rides an implicit `int` declaration.

   See the previous park header (git history of this file) for the decode of
   what the function does.  Three things were measured since:

   1. **`*f = 0` BELONGS IN AN `else`, NOT THE `if`.**  The old park wrote
      `if (s == NULL) { *f = 0; } else if (flags) {...}`, which puts the
      store inline and costs a `j` over it; the target writes
      `if (s != NULL) { if (!flags) {...} } else { *f = 0; }`, which drops
      the store at the bottom, where it falls through into the shared
      `addiu $s0, $s0, 0x4`.  That ALSO buys the ten missing words: with the
      store out of the way, the handler-call path can no longer reach the
      shared increment by fall-through, so it needs `j .L800898B8`, and
      reorg steals the increment into the delay slot -- which is exactly the
      target's "two copies of the increment" that the old header described as
      the remaining job.  It is not a cross-jumping question at all.
      165 words / 103 rows -> 173 words / 67 rows.

   2. **`cam` AND `s` ARE ONE VARIABLE.**  In loop 2 the target overwrites
      $s1 (the object) with `&D_80083498` on the match path, so `s` is live
      across a call somewhere and the single pseudo gets a callee-saved
      register in all three loops.  With a separate `cam` local, `s` lands in
      $a0 everywhere.  67 rows -> 51 rows, no word change.

   3. **THE LOOP BOUND IS A SIGNED COMPARE.**  The target ends loops 1 and 2
      with `addiu $v0, $s3, 0x80` / `slt $v0, $s0, $v0`.  A pointer compare
      (`f < base + 0x20`) is `sltu`.  `(s32)f < (s32)base + 0x80` gives `slt`.
      51 rows -> 50 rows.

   WHAT IS LEFT: 2 words and 50 rows, and all of it is ONE unresolved
   question -- where cc1psx puts three `lui %hi(...)`:

     * loop 1, `a = D_800DCF80`: target keeps `lui $v0, %hi(D_800DCF80)` /
       `addiu $a0, $v0, %lo(D_800DCF80)` INSIDE the loop; we hoist the `lui`
       to the preheader (it takes $16).
     * loop 3, `D_80083160.unk_00`: target keeps `lui $v0, %hi(D_80083160)`
       inside; we hoist it to the preheader (it takes $20).
     * loop 2, `D_800DCF80[0]`: target HOISTS it -- `lui $s5` in the
       preheader, shared with `addiu $s6, $s5, %lo(D_800DCF80)` for `tbl`,
       and the load inside is `lw $v0, %lo(D_800DCF80)($s5)`; we do NOT
       hoist and emit `lui $v0` / `lw $v0, %lo($v0)` inside.

   The two words are exactly that third one: the hoisted $s5 is the target's
   seventh callee-saved register ($16-$22 + $ra, `sw $ra, 0x2C($sp)`), and we
   save only six ($16-$21, `sw $ra, 0x28($sp)`).  The other ~47 rows are
   register numbering that follows from the two spurious hoists stealing $16
   and $20: our loop 1 is f=$18 o=$19 base=$20 tbl=$21 against the target's
   f=$16 o=$18 base=$19 tbl=$20, while our loop 2 already agrees with the
   target except that tbl is $21 and not $22.  Get the three hoists right and
   the register numbering should follow.

   MEASURED AND REJECTED:
     * loop 2 with `tbl` moved inside the loop (`a = D_800DCF80; b = a + 1;`)
       so loop.c has to hoist it: 171 words, 53 rows.  Worse.
     * (from the previous park) a `for` loop with the literal bound in its
       condition, 166/128; `!=` instead of `<`, 162/125. */

void func_800897E4(void)
{
    u32 *f;
    u32 *base;
    u32 *o;
    u32 *a;
    u32 *b;
    u32 *tbl;
    u8 *s;
    void (*h)(u8 *, s32, s32);
    s32 save;
    s32 i;

    if ((D_800E296C & 0x2000000) != 0)
    {
        tbl = D_800DCFA4;
        base = D_80083360;
        f = base;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        a = D_800DCF80;
                        b = tbl;
                        for (;;)
                        {
                            if (*b == (u32)h)
                            {
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                break;
                            }
                            if (*a == 0)
                            {
                                break;
                            }
                            a++;
                            b++;
                        }
                    }
                }
                else
                {
                    *f = 0;
                }
            }
            f++;
            o++;
        } while ((s32)f < (s32)base + 0x80);
    }
    else if ((*(s32 *)(D_800814A8 + 0x1C) & 0x10) != 0)
    {
        tbl = D_800DCF80;
        base = D_80083360;
        f = base;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        b = tbl + 1;
                        if (D_800DCF80[0] == (u32)h)
                        {
                            s = D_80083498;
                            if (*(s32 *)(s + 0x14) != 0)
                            {
                                save = *(s32 *)(s + 0x18);
                                *(s32 *)(s + 0x18) = 0;
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                *(s32 *)(s + 0x18) = save;
                            }
                        }
                        else
                        {
                            for (;;)
                            {
                                if (*b == (u32)h)
                                {
                                    h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                    break;
                                }
                                if (*b == 0)
                                {
                                    break;
                                }
                                b++;
                            }
                        }
                    }
                }
                else
                {
                    *f = 0;
                }
            }
            f++;
            o++;
        } while ((s32)f < (s32)base + 0x80);
    }
    else
    {
        i = 0;
        f = D_80083360;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                        if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
                        {
                            return;
                        }
                    }
                }
                else
                {
                    *f = 0;
                }
            }
            f++;
            i++;
            o++;
        } while (i < 0x20);
    }
}
