/* PARK: func_800B490C -- 219 words against a target of 220, 28 aligned
   differences.  FIRST DRAFT, 2026-11-20.

   The last of the seven 4-mod-8 jump-table functions to get a body.  It owns
   `jtbl_800892E4' at rodata offset 0xb84, which is still in the HEAD unit --
   so unlike func_800C2824/func_800C4F20 this one needs ANOTHER CARVE before
   it can land, a split at rodata 0xb6c taking func_800B45E0 and func_800B490C
   into their own unit.  See splat/dungeon.bin.yaml and j96.

   IT IS func_800B45E0's SIBLING and most of the body transferred directly,
   but NOT all of it -- the three differences are worth stating because the
   obvious move is to copy the sibling wholesale:

     * an extra early-out before the +0x14 block:
           if (*(s16 *)(p + 0x10) != 0) { *(u16 *)(p + 0x10) -= 1; return; }
     * `*(u16 *)(b + 0xA) -= 0x80;' in case 0, not 0x40;
     * **cases 1 and 4 do NOT share a colour-ramp block here.**  In the
       sibling they do, and reaching it with a `goto' is load-bearing there.
       Here case 1 ramps c[0xC..0xE] toward 0x80 and case 4 ramps the
       HALFWORDS c+0x1C/0x1E toward zero and adds 6 to b+0xA, so they are
       different code and each is written out.  Case 4's `else' also carries
       the `|= 0x8000' tail, which in the sibling sits after the switch.

   WHAT TRANSFERRED, and it is most of the residue:
     * `i = t >> 1;' HOISTED into its own local before the `>> 0x11', with
       the store reading `i'.  That is the sibling's permuter find.
     * a `do { } while (0);' around EACH of the two counter decrements, which
       stops the `sh 0xA($s0)' sinking past its own sign extension.  Each is
       worth a word and some rows here: 217/36 plain, 218/32 with case 1's
       alone, 218/33 with case 4's alone, **219/28 with both**.

   ONE WORD SHORT and 28 rows.  The sibling needed exactly this pattern --
   two devices that each cost a word, with the word coming back from the
   hoist -- so the missing word here is likely a third device of the same
   kind rather than a shape error.  A beam sweep over every statement
   position is the cheap next step; it has not been run on this body yet.

   DECLARATIONS
   extern u8 D_80044C54[];
   END DECLARATIONS
   (func_8004491C and D_800814A0 are already in csrc/ovl_dungeon_decls.h.)

   Measured with
     python3 tools/decomp/parkresidue.py func_800B490C
*/
void func_800B490C(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    s32 i;
    s32 n;
    s32 t;
    s32 v;
    s32 j;
    s16 k;
    u16 m;
    u16 u;

    q = *(u8 **)p;
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        *(u8 **)p = NULL;
        *(u8 **)(p + 0x14) = NULL;
    }
    if (*(s16 *)(p + 0x10) != 0)
    {
        *(u16 *)(p + 0x10) -= 1;
        return;
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
        *(u16 *)(b + 0xA) -= 0x80;
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
            u = c[0xE];
            k = u + (0x80 - u) / n;
            c[0xE] = k;
            c[0xD] = k;
            c[0xC] = k;
            break;
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
            u = *(u16 *)(c + 0x1E);
            m = u + -(s32)u / n;
            *(u16 *)(c + 0x1E) = m;
            *(u16 *)(c + 0x1C) = m;
            *(u16 *)(b + 0xA) += 6;
        }
        else
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
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
    if (p[0xE] != 0)
    {
        j = 0x2CF0F0F0;
        if ((*(u16 *)(p + 0xC) & 3) != 0)
        {
            j = 0x2C404040;
        }
        *(s32 *)(c + 0xC) = j;
    }
}
