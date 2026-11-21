/* PARK: func_800B490C -- 219 words against a target of 220, 28 aligned

   ====================================== 2026-11-20: 28 rows at 219/220 words -> 18 at 220/220

   The body below carries one device: a duplicated-arm `if (n)` around `i = t >> 1;` -- which is also the missing word, exactly as the sibling predicted.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same free-reference lever from two directions -- flow.c:2128 weights
   each reference by `loop_depth`, jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  Found by sweeping every
   statement position times both devices over every local on a
   `tools/decomp/onefunc.py` file, where a candidate is 0.09 s.  Nothing in
   this park's own diagnosis points at the winning statement.
   differences.  FIRST DRAFT, 2026-11-20.

   The last of the seven 4-mod-8 jump-table functions to get a body.  It owns
   `jtbl_800892E4' at rodata offset 0xb84.

   THE CARVE THIS NOTE USED TO ASK FOR HAS HAPPENED.  splat/dungeon.bin.yaml
   splits .rodata at 0xa3b6c and .text at 0xcee80 into ovl_dungeon_800B45E0,
   which holds this function and its sibling, so the `.align 3' cc1psx emits
   before the table is a no-op and NOTHING BLOCKS THIS LANDING but the rows.
   The only park still waiting on a carve is func_8009F644 (rodata 0x61c).

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
   ====================================== 2026-11-20: 18 rows -> 7 at 220/220

   A PERMUTER FIND, and both halves of it are the same idea:

     * `u16 m' and `s16 k' MERGED into the one `s16 k'.  They never overlap --
       `m' holds the decremented 0xA counter and `k' the ramp result -- and
       merging them is worth most of the 11 rows.  The type that survives is
       `s16', not the counter's `u16'.
     * the ramp split through a self-assignment:

           k = u;
           k = u + (0x80 - k) / n;

       instead of `k = u + (0x80 - u) / n;'.  That gives the DIVIDEND the
       result variable's register, which is what the target does -- its `subu'
       writes the same register the `mflo' later reuses.

   THE SIBLING func_800B45E0 HAS THE SAME RAMP and this does NOT transfer to
   it, because there the two arms compute DIFFERENT numerators (0x80 - j and
   -j) and jump into a SHARED div, so the numerator has to be a variable and
   cannot be an inline sub-expression of the result.  That is the structural
   difference between the two parks and it is why one is at 7 and the other
   at 6 by completely different routes.
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
            k = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = k;
        n = (s16)k;
        if (n > 0)
        {
            u = c[0xE];
            k = u;
            k = u + (0x80 - k) / n;
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
        k = *(u16 *)(p + 0xA) - 1;
        *(u16 *)(p + 0xA) = k;
        if ((s32)(k << 16) <= 0)
        {
            *(u16 *)(p + 0xA) = 8;
            *(u16 *)(p + 0x8) += 1;
        }
        break;
    case 4:
        do
        {
            k = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = k;
        n = (s16)k;
        if (n > 0)
        {
            u = *(u16 *)(c + 0x1E);
            k = u + -(s32)u / n;
            *(u16 *)(c + 0x1E) = k;
            *(u16 *)(c + 0x1C) = k;
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
        if (n)
        {
            i = t >> 1;
        }
        else
        {
            i = t >> 1;
        }
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
