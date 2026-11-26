/* PARK: func_b31f_8016F78C (ovl_dng_b31f) -- 190 words / 190 words /
   22 ALIGNED DIFFERENCES.  Agent h186, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   THE WORD COUNT IS EXACT, EVERY INSTRUCTION IS IN POSITION, AND ALL 22
   ROWS ARE THE FRAME SIZE.  The target is `addiu $sp, $sp, -0x30' with the
   six saves ($s0..$s4, $ra) at 0x18..0x2C; this body is `-0x28' with them
   at 0x10..0x24.  Eight bytes of stack I cannot source.

   THE DIAGNOSTIC IS DECISIVE: forcing the outgoing-argument area to 24
   bytes by giving ONE call a fifth argument through a unit-local alias
   (`func_800AA6B4_5(p, b, c, D_b31f_80174A64, 0)') gives 191 words / 3
   ALIGNED DIFFERENCES -- the extra `sw' and two displacements.  So the
   target's frame carries 24 bytes of outgoing-argument space (a 5- or
   6-argument call, 20 rounded up to the 8-byte boundary) or 8 bytes of
   addressed local, and NOTHING ELSE about this body is wrong.

   WHAT I COULD NOT FIND: the listing has NO store to any `($sp)' offset
   except the six register saves, so no fifth argument is ever written, and
   no `addiu $aX, $sp, N' anywhere, so no local's address is ever passed.
   Every call's arity is readable and all are 4 or fewer: func_b31f_8016FA84
   (0 -- nothing is set up at all), func_800AA258 (4), func_80047784 (3,
   twice), func_800AA6B4 (4), func_800AA888 (4), func_b31f_8017092C (4),
   func_8009FB34 (2), func_800A9A0C (1, three times), func_8009A180 (2),
   func_b31f_80174320 (3), func_b31f_80170224 (4), func_b31f_8016FCE4 (4).
   The RESIDENT TWIN func_800D8C64 (csrc/ovl_dungeon_800D50FC.c, 250 words)
   HAS BOTH CANDIDATES: a `u16 x' whose address goes to func_800A0818 as a
   fifth argument.  That whole tail -- the D_800E2970 / 0x430 /
   func_8009FD7C / func_800A0818 block -- is ABSENT from this relink's
   listing, so if x survived in the source it survived as dead code, which
   is not something this body can show.  WHAT I WOULD TRY NEXT, and it is a
   strong lead: func_b31f_8016B778, LANDED in this same unit by the same
   agent at 359/0, is the fuller relink of the SAME resident twin and it DOES
   carry both constructs -- a `u16 x' whose address goes to func_800A0818 as
   a fifth argument at two sites, and a five-argument func_800AAF00 call --
   and its frame is -0x38 with the same six saves.  Read its body beside
   this listing and look for the arm this relink kept in a form I read as
   something else; a func_800AAF00(p, b, c, table, (u32)handler) call is
   five arguments and would reserve exactly the missing 24 bytes.

   WHAT IT DOES: the bank's pre-walk gate, a 190-word relink of the resident
   func_800D8C64 with its d[0x25] arm, its 0x200/func_800AA924 arm, its
   func_800A1C58/func_800AAB10 arm and its whole 0x2000-clear tail removed,
   and with three new arms: the 0x80000 arm calls func_b31f_8017092C and
   re-dresses unconditionally from D_b31f_80174A2C before zeroing +0x90, the
   `(*(u16 *)0x80013714 & 8)' arm hands off to func_b31f_80174320 and then
   clears the record's 0x7F/0x7FFF pair round a func_800A9A0C, and the
   fall-out calls func_b31f_80170224(p, b, c, 0) and raises 0x4000.  The
   shared tail clears +0x46's 0x8000 and +0x71's top bit, calls
   func_800A9A0C(d) and hands on to func_b31f_8016FCE4.

   THREE SPELLINGS FORCED BY THE LISTING AND KEPT:
   1. THE 0x80013714 TEST IS THE BARE CONSTANT `*(u16 *)0x80013714', not the
      declared symbol.  The preceding `bnez $v0, .Lb31f_8016FA60' leaves a
      `nop' in its delay slot, and on a declared symbol reorg lifts
      `lui %hi' into it -- this unit's func_b31f_8016D5B4 comment records
      the same measurement on the same symbol.
   2. func_b31f_8016FA84 IS CALLED WITH NO ARGUMENTS AT ALL.  The call site
      sets nothing and leaves `sb $v0, 0x9A($s2)' in the delay slot, so it
      goes through a unit-local alias
      `void func_b31f_8016FA84_0(void) __asm__("func_b31f_8016FA84")' --
      its real landed definition takes four parameters and its listing is
      gone, so the prototype was NOT changed.
   3. THE 0xE STATE CONSTANT IS A LOCAL (`k = 0xE') in the second arm: the
      target puts it in $s4 (`addiu $s4, $zero, 0xE', saved unconditionally)
      and uses it for both the `beq' and the `sb'.  The first arm's 0xE is a
      separate `addiu $v0, $zero, 0xE' and stays inline.

   MEASURED AND FREE, so neither is the reason the figure is what it is:
   | spelling                                            | words | aligned |
   | as written below                                    |  190  |   22    |
   | `s16 n = func_8009FB34(...); c[0x26] = n;' (the
     resident twin's stated fact, which it prices at
     251/106 the other way)                              |  190  |   22    |
   | one call given a fifth argument (the diagnostic)     |  191  |    3    |

   DECLARATIONS
   extern u16 D_80083462;
   extern s16 D_80083228;
   extern u8 D_b31f_80174A2C[];
   extern u8 D_b31f_80174A64[];
   extern u8 D_800814A8[];
   void func_80047784(u8 *, s32, s32);
   void func_800AA258(u8 *, void *, u8 *, u8 *);
   s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
   void func_800AA888(u8 *, void *, u8 *, u8 *);
   s16 func_8009FB34(u16 x, u16 y);
   s32 func_8009A180(u8 *a, u8 *b);
   void func_800A9A0C(u8 *);
   void func_b31f_8016FA84_0(void) __asm__("func_b31f_8016FA84");
   void func_b31f_8017092C(u8 *p, UnkVec *b, u8 *c, u8 *d);
   s32 func_b31f_80170224(u8 *a, void *b, void *c, s32 d);
   s32 func_b31f_80174320(u8 *p, UnkVec *b, u8 *c);
   void func_b31f_8016FCE4(u8 *p, void *b, void *c, u8 *d);
   END DECLARATIONS */
void func_b31f_8016F78C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 k;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b31f_8016FA84_0();
        return;
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        k = 0xE;
        if (p[0x9A] != k)
        {
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A2C)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A2C;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A2C), 0);
            }
            p[0x9A] = k;
        }
        *(u16 *)(p + 0x98) &= 0xFFF3;
        if (*(s16 *)(d + 0x64) != 0)
        {
            if (func_800AA6B4(p, b, c, D_b31f_80174A64) != 0)
            {
                return;
            }
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b31f_8017092C(p, b, c, d);
            *(u8 **)(c + 0x2C) = D_b31f_80174A2C;
            func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A2C), 0);
            *(s32 *)(p + 0x90) = 0;
            return;
        }
    }
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    if (*(s8 *)(d + 0x6D) <= 0)
    {
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
    {
        if ((D_80083462 & 0x2000) != 0)
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
        }
        if ((*(u16 *)0x80013714 & 8) != 0)
        {
            func_b31f_80174320(p, b, c);
            d[0x71] &= 0x7F;
            func_800A9A0C(d);
            *(u16 *)(d + 0x46) &= 0x7FFF;
            return;
        }
        if ((s16)func_b31f_80170224(p, b, c, 0) == 0)
        {
            return;
        }
        *(u16 *)(d + 0x46) |= 0x4000;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    d[0x71] &= 0x7F;
    func_800A9A0C(d);
    func_b31f_8016FCE4(p, b, c, d);
}
