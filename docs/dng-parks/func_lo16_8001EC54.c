/* func_lo16_8001EC54 -- ovl_dng_lo16, 174 target words.
   PARKED by agent h176, batch 35, 2026-11-25.  174 words / 32 ALIGNED
   DIFFERENCES, of which FOUR are the one real fault and the other
   twenty-eight are an `n'-in-$s2-against-$s3 rotation behind it.

   WHAT IT IS: the chest/item budget spender.  It computes a budget from
   D_8008146C (x200 when *(s32 *)0x80012090 == 2, else a 0x4B / 0xC8 / 0x2BC
   band), jitters it by +/- rand(0, budget/2), then repeatedly claims a free
   D_800E3548 slot, picks a kind and a value from one of three bands, draws a
   free cell, rejects it against the 0x40 existing spots and publishes it,
   until the budget runs out.

   THE ONE REMAINING FAULT IS THE DIVIDE BY TEN:

       candidate   lui 0xCCCC / ori 0xCCCD / multu $s2 / mfhi / srl $t0, 3
       target      lui 0x6666 / ori 0x6667 / mult  $s2 / mfhi / srl $t0, 2

   The target's is `choose_multiplier (10, 32, 31)' -- THIRTY-ONE bits of
   precision -- which gives ml = 0x66666667 and post_shift 2 with mh == 0,
   and `expand_mult_highpart' then uses the SIGNED `mult' because both
   operands' high bits are clear.  The candidate's is
   `choose_multiplier (10, 32, 32)': ml = 0xCCCCCCDD, post_shift 3, `multu'.
   So the target divides a value gcc knows fits in 31 bits and the candidate
   does not.  SIX spellings measured and none reaches it:
     `s32 cost', `cost / 10'      -> the full SIGNED sequence,
                                     `sra 0x1F' / `sra 2' / `subu',
                                     176 words / 17
     `u32 cost', `cost / 10'      KEPT                        174 / 32
     `u16 cost', `cost / 10'                                  178 / 42
     `s32 cost', `(u32)cost / 10'                             174 / 32
     `s32 cost', `(u16)cost / 10'                             175 / 35
     `s32 cost', `(cost & 0xFFFF) / 10'                       176 / 17
     `s32 cost', `(u32)(cost & 0xFFFF) / 10'                  174 / 32
   NOTE WHAT THE FIRST AND SECOND ROWS TOGETHER SAY: this compiler promotes
   `u16' UNSIGNED, not value-preserving -- `u16 x; x / 10' is a `multu' --
   so the usual ANSI reasoning about where a division's signedness comes
   from does not apply in this tree.  The same thing shows up in
   func_lo16_8001EAA4's `(x & 0x3000) / 0x1000', which comes out `srl' from
   a `u16' load and needs an `s32' local to go signed.

   THE SECOND FAULT, present in the 176-word spellings and gone in the
   174-word ones: `D_800E36C8[slot].unk_04 = func_800BCA68(...)' must come
   BEFORE `D_800E36C8[slot].unk_07 = 0' in the source, and reorg then sinks
   the `sh $v0, 0x4($s0)' into the NEXT call's (func_8009A21C's) delay slot
   while the `sb $zero, 0x7($s0)' stays where it is.  Written the other way
   round it is 175 words / 19.  Do not read the store order off the listing
   here -- that is the brief's "a store's position inside a dress block is
   not readable".

   EVERYTHING ELSE IS SETTLED AND AGREES:
   - the budget band is an if/else-if chain and `D_8008146C * 200' comes out
     as the target's `sll 1'/`addu'/`sll 3'/`addu'/`sll 3' by itself.
   - the +/- jitter is a two-arm `if/else' on `(rand() & 1) != 0', with the
     shared `addu $a0, $zero, $zero' in the gate's own delay slot, and
     `n / 2' as a SIGNED divide by two (`srl 31'/`addu'/`sra 1'), so `n' is
     `s32'.
   - `slot = (s16)func_800A71F4()' -- the cast is the `sll 16'/`sra 16'.
   - the spot scan is `for (j = 0; j < 0x40; j++) { if (...) break; }' and
     then `if (j == 0x40)', which is what the target's `addiu $v0, $zero,
     0x40' in the hit arm's delay slot plus `bne $a1, $v0' is.
   - UnkDungeonSpotA needs TWO MORE FIELDS witnessed here: `u8 unk_07' and
     `u32 unk_08' carved out of its `pad_06[0x6]'.  The layout is unchanged,
     so func_lo16_8001D328, which uses unk_00/unk_01, re-verifies at 0.
   - THIS BODY ADDRESSES A CONSTANT BASE: `*(s32 *)0x80012090', same as
     func_lo16_8001E660 and func_lo16_8001784C in this unit.

   DECLARATIONS
   s16 func_800A4E2C(u8 *, u8 *);
   s16 func_800A71F4(void);
   s16 func_800BCA68(u16, u16);
   u32 func_800A7A38(u8 *);
   extern UnkDungeonSlot D_800E3548_s[] __asm__("D_800E3548");
   END DECLARATIONS
*/
void func_lo16_8001EC54(void)
{
    UnkDungeonSlot *q;
    u8 buf[2];
    s32 n;
    s32 slot;
    u32 cost;
    s32 a;
    s32 b;
    s32 kind;
    s32 j;

    if (*(s32 *)0x80012090 == 2)
    {
        n = D_8008146C * 200;
    }
    else if (D_8008146C < 0xC)
    {
        n = 0x4B;
    }
    else if (D_8008146C < 0x16)
    {
        n = 0xC8;
    }
    else
    {
        n = 0x2BC;
    }
    if ((func_800A6D30() & 1) != 0)
    {
        n += func_800A6DA4(0, (u16)(n / 2)) & 0xFFFF;
    }
    else
    {
        n -= func_800A6DA4(0, (u16)(n / 2)) & 0xFFFF;
    }
    while (n >= 0)
    {
        slot = (s16)func_800A71F4();
        if (slot < 0)
        {
            return;
        }
        if (n >= 0x12D)
        {
            cost = func_800A6DA4(0xA0, 0xF0) & 0xFFFF;
            b = cost / 10;
            a = 1;
            kind = 3;
        }
        else if (n >= 0x97)
        {
            cost = func_800A6DA4(0x50, 0x78) & 0xFFFF;
            b = cost;
            a = 0;
            kind = 2;
        }
        else
        {
            cost = func_800A6DA4(0x10, 0x18) & 0xFFFF;
            b = cost;
            a = 0;
            kind = 1;
        }
        while ((s16)func_800A4E2C(&buf[0], &buf[1]) < 0)
        {
        }
        for (j = 0; j < 0x40; j++)
        {
            if (D_800E3548_s[j].unk_01 != 0 && D_800E36C8[j].unk_00 == buf[0] &&
                D_800E36C8[j].unk_01 == buf[1])
            {
                break;
            }
        }
        if (j == 0x40)
        {
            q = &D_800E3548_s[slot];
            q->unk_00 = kind;
            q->unk_01 = 0xE;
            q->unk_03 = a;
            q->unk_02 = b;
            D_800E36C8[slot].unk_00 = buf[0];
            D_800E36C8[slot].unk_01 = buf[1];
            D_800E36C8[slot].unk_04 = func_800BCA68((buf[0] << 6) | 0x20, (buf[1] << 6) | 0x20);
            D_800E36C8[slot].unk_07 = 0;
            func_8009A21C(buf[0], buf[1], 0x800);
            D_800E36C8[slot].unk_08 = func_800A7A38((u8 *)q);
        }
        n -= cost;
    }
}
