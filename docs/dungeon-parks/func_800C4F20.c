/* PARK: func_800C4F20 -- **323 of 323 words**, 207 aligned differences.
   FIRST DRAFT, 2026-11-20.

   The sixth of the seven 4-mod-8 jump-table functions, and since j96's carve
   it and func_800C2824 are the ONLY two stubs left in
   csrc/ovl_dungeon_800C2824.c.  Its table jtbl_800893E4 is at offset 0x58 of
   that unit's rodata -- 0 mod 8 -- so **it lands the moment the body is
   right**, with no further splat work.

   THE WORD COUNT IS ALREADY EXACT.  Every one of the 207 rows is a register
   name or a scheduling slot; there is no missing or extra instruction.
   `ovlverify` says 284 differing masked, which for a permutation of this size
   is the LCS charging each moved insn twice -- quote 323/323 and the row
   count, not the masked number.

   WHAT IT IS.  A six-arm state machine on `*(s16 *)(p + 0x28)' (cases 0, 1,
   2, 3, 17 plus a default) that eases a camera toward D_80083780's three
   halfwords, fades c+0x1C/0x1E/0x20 and the +0x20 sub-node's pair, and on the
   last arm clears 0x100000 out of the actor's +0x14 and raises 0x8000.

   STRUCTURE THAT IS ESTABLISHED (read off the listing):
     * ALL FIVE ARMS SHARE THE TAIL (m2c's block_24): the `+0x100000' test,
       the three copies into the +0x1C sub-node, `*(u16 *)(p + 0x2C) += 1;'
       and `*(u16 *)(c + 0x1A) -= 0x80;'.  It is the `default:' arm and the
       others reach it with a `goto', which is what the target's five
       `j .L800C539C' edges are.  Writing it out per arm is 320 words -- THREE
       SHORT -- so the sharing is not optional.
     * cases 1 and 2 also share the `*(u16 *)(p + 0x2A) = 0x10;' +
       `*(u16 *)(p + 0x28) += 1;' pair with case 0's 0x20 version
       (m2c's block_16/block_17), and case 3 and case 17 share the fade tail
       (block_22).  Sharing the state-set pair through an `st' variable and a
       label is 318 words -- FIVE SHORT -- so the target evidently does NOT
       share that one even though cross-jumping could: keep it written out.
     * `(s32)(w << 16) <= 0' for every counter test, with a u16 temp holding
       the decremented value -- the `sll 16' + `blez' shape.

   WHAT IS LEFT, AND WHERE IT STARTS.  The first difference is word 22:

       22  lh $a1, 0x2A($s1)      target      lh  $a0, 0x2A($s1)   ours
       26  lhu $a0, 0x1E($s4)                 lhu $a1, 0x1E($s3)

   i.e. the divisor `n' and the faded halfword have $a0 and $a1 exchanged,
   and `c' is in $s3 where the target has $s4.  Everything after is that
   rotation propagating.  Priced off `.lreg':

       reg  80  p      57 refs / 510 insns  pri 5588  -> $s1  ok
       reg  81  b      20 refs / 484 insns  pri 1652  -> $s2  ok
       reg  82  c      16 refs / 498 insns  pri 1285  -> $s3  WANTED $s4
       reg 164  &D_80083780 + 2   3 /  42   pri  714  -> $s4  WANTED $s3

   so `c' and the shared D_80083780 base have to exchange places, which needs
   `pri(164) > 1285'.  At three references that is 1*3/42; FOUR references is
   2*4/42 = 1904 and clears it -- the `floor_log2' step at 4 is what does it.

   MEASURED:
     * `g = (u8 *)&D_80083780;` as a POINTER LOCAL inside case 1, with the
       three reads at `g + 0x2/0x6/0xA`, DOES move `c' to $s4 -- and costs a
       word (322) because the base assignment becomes a real insn.  That is
       the right register for the wrong price.
     * a `do { } while (0);` around each of the three reads in turn, to buy
       the fourth reference for nothing: 322/211, 323/254, 324/217.  None
       helps, and the middle one is the only one that keeps the count.
     * splitting the read-modify pairs into separate load and result locals
       (m2c's `temp_a0' / `temp_a0_2'), which is what the target looks like:
       no change at all, 323/207.
   So the 4th reference has to come from somewhere that costs no instruction
   AND does not lengthen reg 164's 42-insn range much -- at 4 refs it stays
   above `c' until the range reaches 62.

   **THIS IS A PERMUTER TARGET.** The word count is exact, the stream aligns,
   and the residue is one named allocation inversion -- the shape that closed
   func_8008E700's last two rows after 143000 candidates and func_8009DAD8's
   nine from base=45.

   DECLARATIONS
   NONE.  func_80064584, func_800644B8, func_80069EF8 and func_800A56E0 ride
   implicit declarations as the surrounding landed code does, and D_80083780
   and D_800814A0 are already declared.
   END DECLARATIONS

   Measured with
     python3 tools/decomp/parkresidue.py func_800C4F20
*/
void func_800C4F20(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    u8 *r;
    s32 i;
    s32 j;
    s16 n;
    s16 m;
    u16 w;
    u16 v;

    switch (*(s16 *)(p + 0x28))
    {
    case 0:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            w = *(u16 *)(c + 0x1E);
            w = w + (0x1000 - w) / n;
            v = *(u16 *)(c + 0x20);
            *(u16 *)(c + 0x1E) = w;
            *(u16 *)(c + 0x1C) = w;
            *(u16 *)(c + 0x20) = v + (0x1000 - v) / *(s16 *)(p + 0x2A);
            q = *(u8 **)(p + 0x20);
            w = *(u16 *)(q + 0x1E);
            w = w + (0x400 - w) / *(s16 *)(p + 0x2A);
            *(u16 *)(q + 0x1E) = w;
            *(u16 *)(q + 0x1C) = w;
            *(u16 *)(p + 0x32) -= *(s16 *)(p + 0x32) / *(s16 *)(p + 0x2A);
        }
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(c + 0x1E) = 0x1000;
            *(u16 *)(c + 0x1C) = 0x1000;
            *(u16 *)(c + 0x20) = 0x1000;
            q = *(u8 **)(p + 0x20);
            *(u16 *)(q + 0x1E) = 0x800;
            *(u16 *)(q + 0x1C) = 0x800;
            *(s16 *)(p + 0x32) = 0;
            *(u16 *)(p + 0x2A) = 0x20;
            *(u16 *)(p + 0x28) += 1;
        }
    default:
    join:
        if ((*(u32 *)(*(u8 **)(p + 0x24) + 0x14) & 0x100000) != 0)
        {
            q = *(u8 **)(p + 0x1C);
            *(u16 *)(q + 0x2) = *(u16 *)(b + 0x2);
            *(u16 *)(q + 0x6) = *(u16 *)(b + 0x6);
            *(u16 *)(q + 0xA) = *(u16 *)(b + 0xA);
        }
        *(u16 *)(p + 0x2C) += 1;
        *(u16 *)(c + 0x1A) -= 0x80;
        return;
    case 1:
        i = *(s16 *)(D_80083780 + 0x2) + (func_80064584(*(s16 *)(p + 0x2E)) >> 6);
        i -= *(s16 *)(b + 0x2);
        j = *(s16 *)(D_80083780 + 0x6) + (func_800644B8(*(s16 *)(p + 0x2E)) >> 6);
        if (i < 0)
        {
            i += 3;
        }
        *(u16 *)(b + 0x2) += i >> 2;
        *(u16 *)(b + 0x6) += (j - *(s16 *)(b + 0x6)) / 4;
        i = *(s16 *)(D_80083780 + 0xA) - *(s16 *)(b + 0xA);
        *(u16 *)(b + 0xA) += (i - ((func_800644B8(*(s16 *)(p + 0x2C) << 5) >> 6) + 0x40)) / 4;
        n = *(s16 *)(p + 0x36);
        *(u16 *)(p + 0x2E) += 0x40 + (func_80069EF8(i) & 0x1F);
        if (n != 0)
        {
            w = *(u16 *)(p + 0x2A) - 1;
            *(u16 *)(p + 0x2A) = w;
            if ((s32)(w << 16) <= 0)
            {
                if (n != 1 && n != 3)
                {
                    *(u16 *)(p + 0x2A) = 0x20;
                    *(u16 *)(p + 0x28) += 0x10;
                    func_800A56E0(0x518);
                }
                else
                {
                    *(u16 *)(p + 0x2A) = 0x10;
                    *(u16 *)(p + 0x28) += 1;
                }
            }
        }
        goto join;
    case 2:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            q = *(u8 **)(p + 0x20);
            *(u16 *)(b + 0x2) += ((q[0x24] << 6) - (*(s16 *)(b + 0x2) - 0x20)) / n;
            *(u16 *)(b + 0x6) += ((q[0x25] << 6) - (*(s16 *)(b + 0x6) - 0x20)) / *(s16 *)(p + 0x2A);
            *(u16 *)(b + 0xA) +=
                (*(s16 *)(*(u8 **)(p + 0x24) + 0x88) - *(s16 *)(b + 0xA)) / *(s16 *)(p + 0x2A);
        }
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(p + 0x2A) = 0x10;
            *(u16 *)(p + 0x28) += 1;
        }
        goto join;
    case 3:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            w = *(u16 *)(c + 0x1E);
            w = w - (s32)w / n;
            *(u16 *)(c + 0x1E) = w;
            *(u16 *)(c + 0x1C) = w;
            v = *(u16 *)(c + 0x20);
            *(u16 *)(c + 0x20) = v + (0x2000 - v) / *(s16 *)(p + 0x2A);
            q = *(u8 **)(p + 0x20);
            w = *(u16 *)(q + 0x1E);
            w = w + (0x1000 - w) / *(s16 *)(p + 0x2A);
            *(u16 *)(q + 0x1E) = w;
            *(u16 *)(q + 0x1C) = w;
            *(u16 *)(p + 0x32) += (0x400 - *(s16 *)(p + 0x32)) / *(s16 *)(p + 0x2A);
        }
    fade:
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            q = *(u8 **)(p + 0x24);
            *(u32 *)(q + 0x14) &= 0xFFEFFFFF;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        goto join;
    case 17:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            *(u16 *)(b + 0xA) += (-0x400 - *(s16 *)(b + 0xA)) / n;
        }
        goto fade;
    }
}
