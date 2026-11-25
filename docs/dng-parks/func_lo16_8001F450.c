/* func_lo16_8001F450 -- ovl_dng_lo16, 77 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 77 candidate words, 2 ALIGNED differences, and they are the SAME TWO
   INSTRUCTIONS in a different register:

       candidate   lui $v0, %hi(D_80012090) / lw $v1, %lo(...)($v0)
       target      lui $v1, %hi(D_80012090) / lw $v1, %lo(...)($v1)

   The target ties the `lui's output to the `lw's output (one register); the
   candidate does not.  Everything else in the body -- 75 of 77 words -- is
   byte-identical, including all five call sites, the two-arm flag/table gate,
   the delay-slot `$s2 = 0', the `s16 x' cursor stepped by 0x12 and the
   `s16 y = 0x78' whose `0x78 << 16' loop.c hoists into $s4 so only the
   `sra $a1, $s4, 16' stays in the loop.

   This is the FIRST insn pair in the function, so nothing earlier can be
   blamed; it is local-alloc deciding not to reuse the register for a pseudo
   that dies at its single use.

   SPELLINGS MEASURED (candidate words / ovlverify differing):

     `t = NULL' first, `f = 0' before the 0x28 test,
       `if (D_80012090 == 2) f = 1; else {...}'            76w / 13
     the same with the arms INVERTED (`!= 2' first)        77w / 3
       -- this is the shape fact: with `== 2' as the `then' gcc hoists
          `f = 1' into the entry block and overwrites it, losing a word
     + `f = 0' moved AFTER the inner 0x28 `if'             77w / 2   <- kept
       -- this is what turns `move $s2, $a3' (cse substituting the register
          that already holds 0, from `t = NULL') into `addu $s2, $zero, $zero'
     + `s16 f' instead of `s32 f'                          77w / 2   FREE
     + `u8 *t' declared last instead of third              77w / 2   FREE
     the buf[1] arms as `if (buf[1] != 0) x = 0xC8; else
       x = 0xDA;'                                          77w / 5
       -- inverted: `if (buf[1] == 0) x = 0xDA; else x = 0xC8;' is what
          lands the `bnez' with `$s1 = 0xC8' in its delay slot

   WHAT TO TRY NEXT: this is the brief's "wrong register WITHIN a class"
   rung.  The cheapest reading is ONE LOCAL TOO MANY -- `b' and `t' are both
   `u8 *', `b' is only ever `buf' and `t' is only ever NULL or D_80077E00, and
   they never overlap except at the `if (t != NULL)' gate.  Try merging them,
   and try removing `b' entirely (pass `buf' to func_8004E634 and subscript
   `buf' in the loop).  Declaration-order permutation is already measured FREE
   so allocation PRIORITY is out; the remaining levers are the pseudo COUNT
   and `reg_n_sets' (note `t' has TWO sets, which by the unit's own
   `reg_n_sets' rule is exactly the condition for `lui $v0 / addiu $aN, $v0'
   rather than a tied pair).

   DECLARATIONS
   extern u8 D_80077E00[];
   extern u8 D_80077DE8[];
   extern u32 D_8001F5B4[];
   u8 *func_8004E634(u32, u8 *);
   void func_lo16_8001F354(s16 x, s16 y, s16 f, u32 v);
   END DECLARATIONS

   func_lo16_8001F354 is LANDED in this unit (63/63, 0 differing) and this
   body is its only caller; D_80012090 and D_8008146C come from structs.h.
*/
void func_lo16_8001F450(void)
{
    u8 buf[8];
    u8 *b;
    s16 x;
    s16 y;
    s32 f;
    u8 *t;

    t = NULL;
    if (D_80012090 != 2)
    {
        if (D_8008146C == 0x28)
        {
            t = D_80077E00;
        }
        f = 0;
    }
    else
    {
        f = 1;
    }
    if (t != NULL)
    {
        func_lo16_8001F354(0xAA, 0x78, 0, (u32)t);
        return;
    }
    func_lo16_8001F354(0xAA, 0x78, f, (u32)D_80077DE8);
    b = buf;
    func_8004E634(D_8008146C, b);
    if (buf[1] == 0)
    {
        x = 0xDA;
    }
    else
    {
        x = 0xC8;
    }
    if (buf[0] == 0)
    {
        return;
    }
    y = 0x78;
    while (*b != 0)
    {
        if (*b != 0x20)
        {
            func_lo16_8001F354(x, y, f, D_8001F5B4[*b]);
        }
        b++;
        x += 0x12;
    }
}
