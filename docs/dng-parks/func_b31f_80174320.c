/* PARK: func_b31f_80174320 (ovl_dng_b31f) -- 213 words / 213 words /
   7 ALIGNED DIFFERENCES.  Agent h186, batch 35.  One-member family.
   OWNS NO JUMP TABLE -- the ten-case dispatch is a COMPARE CHAIN with three
   `slti' splits at 0xD1, 0x11 and 0xE9, which is what gcc 2.7.2 emits for a
   sparse set, and the listing opens at `glabel'.

   WHAT IT DOES: this bank's SECOND cutscene-script stepper, on the script
   pointer D_b31f_80174AB4 with the frame counter D_b31f_80174AB8.  Bail out
   when the slot's +0x14 carries 0x40 or the script pointer is NULL; when the
   command byte is zero clear the actor's +0xB2 and fall to the shared
   func_800A9A0C tail; otherwise count the frame, advance the script by two
   and reset the state byte when the count passes the repeat byte, publish
   the command's low three bits as the actor's +0x2A heading, raise 0x8000 on
   +0x46, and dispatch on the command's 0xF8 field: 8 hands to
   func_b31f_8016FCE4 and returns, 0xC0 clears +0x9B and FALLS INTO 0xC8
   which calls func_b31f_801740F8, 0x10/0xF0/0xF8 are three dress arms
   (D_b31f_80174A2C / D_b31f_80174AAC / D_b31f_80174AA4) that cross-join
   their dress block, 0xE8 dresses from D_b31f_80174A9C then calls
   func_b31f_80171594 and func_800A152C(0x15, 1), 0xD8 dresses from the same
   table then raises 0x800 on the slot's +0x14, 0xD0 masks +0x14 down to
   0x800 and zeroes it if anything survives, and 0xE0 dresses from
   D_b31f_80174A7C then calls func_b31f_801724D4 and func_8009A3D0.

   THE TWIN IS func_b55_80173D6C (csrc/ovl_dng_b55.c, 72 words) -- the same
   skeleton, and FOUR OF ITS FOUR STATED FACTS WERE RE-PRICED HERE:
   * `u8 k' PLUS a separate `s32 v = k' is right: the listing's redundant
     `andi $t0, $a3, 0xFF' needs both, and `u16 k' gives `andi 0xFFFF'
     (29 rows against 29 -- NOT MEASURABLE on this body's figure because the
     register rotation below masks it, but the andi's immediate is visibly
     0xFF in the target).
   * THE REPEAT BYTE IS READ BEFORE THE COUNTER IS STORED, so the increment
     goes inside the comparison: `s[0] < ++D_b31f_80174AB8'.  Kept.
   * THE ALIAS LOCAL FOR THE FIRST PARAMETER IS WORTH 22 ROWS AND ONE WORD
     HERE, and it is the single largest fact on this body:
     | spelling                                          | words | aligned |
     | `q = p', q at the field sites AND both call sites,
       `p' at the final func_800A9A0C                    |  213  |    7    |
     | `q = p', q at the field sites and the tail call   |  212  |    7    |
     | `q = p', q EVERYWHERE including the tail          |  213  |   27    |
     | `q = p', q at the field sites only                |  214  |   29    |
     | no alias at all                                   |  214  |   29    |
     The mechanism is b55's: with the alias the parameter stays live in $a0
     past the D_b31f_80174AB4 load, so that load takes $a1 (which is what
     this listing has) and the two bank-local calls reach their first
     argument without an extra `move'.
   * b55's k == 0 arm has its own tail; HERE IT FALLS INTO THE SHARED ONE
     (the listing's `j .Lb31f_8017464C' with `sb $zero, 0xB2($s0)' in the
     delay slot), which is b55's rejected spelling, so that fact INVERTS.

   THE RESIDUE IS THE PROLOGUE'S PARAMETER-COPY ORDER AND ITS WAKE.  The
   target copies $a1 into $s2 at word 2 and defers $a0 into $s1 to the
   `bnez' delay slot at word 11; this body does the opposite.  All 7 rows
   are that:  two `sw' positions, the two `move's, the `beqz' delay slot
   (target `addu $v0, $zero, $zero', mine `move a0,s1'), and
   func_b31f_801740F8's fourth argument coming off $a0 instead of $s1.

   FLAT AT 7 ACROSS EVERYTHING ELSE TRIED, so do not re-sweep these:
   | `q = p' before the first gate                       |  223  |   58    |
   | `q = p' after the second gate                       |  214  |    9    |
   | `q' declared first in the block                     |  213  |    7    |
   | a `b2 = b' alias as well                            |  213  |   31    |
   | a fourth parameter `u8 *d' declared and unused      |  213  |    7    |
   | `void *b' instead of `UnkVec *b'                    |  213  |    7    |
   | an `s' local for the script pointer                 |  213  |    7    |
   | that plus the store as `(s32)(s + 2)'               |  213  |    7    |
   | `s' declared first                                  |  213  |    7    |
   | the tail call on `q'                                |  213  |   27    |
   WHAT I WOULD TRY NEXT: the permuter.  The fault is a sched1 tie on two
   independent parameter copies in the entry block, which no source
   permutation I found moves; `-da' on `.sched' would say which of priority
   or INSN_LUID decides it, and that is a question about the compiler's
   input costs rather than about the C.

   SAID LOUDLY, A NEW PROTOTYPE THIS UNIT NEEDS: this function is THREE
   parameters, `s32 func_b31f_80174320(u8 *p, UnkVec *b, u8 *c)' -- $a3 is
   never touched in the body and func_b31f_8016F78C's call site sets only
   $a0, $a1 and $a2.  The unit's block had no declaration for it at all
   (func_b31f_8016F78C calls it from above its definition), so this adds one
   rather than changing one.

   DECLARATIONS
   extern s32 D_b31f_80174AB4;
   extern s16 D_b31f_80174AB8;
   extern s16 D_80083228;
   extern u8 D_b31f_80174A2C[];
   extern u8 D_b31f_80174AA4[];
   extern u8 D_b31f_80174AAC[];
   extern u8 D_b31f_80174A9C[];
   extern u8 D_b31f_80174A7C[];
   void func_80047784(u8 *, s32, s32);
   void func_b31f_8016FCE4(u8 *p, void *b, void *c, u8 *d);
   void func_b31f_801740F8(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_b31f_80171594(s16 a, s16 b, s32 c);
   void func_b31f_801724D4(void);
   s32 func_800A152C(s16 a, s16 b);
   void func_8009A3D0(s32 x, s32 y, s32 f);
   void func_800A9A0C(u8 *);
   END DECLARATIONS */
s32 func_b31f_80174320(u8 *p, UnkVec *b, u8 *c)
{
    u8 k;
    s32 v;
    u8 *q;

    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return 0;
    }
    q = p;
    if (D_b31f_80174AB4 == 0)
    {
        return 0;
    }
    k = ((u8 *)D_b31f_80174AB4)[1];
    v = k;
    if (v == 0)
    {
        *(s16 *)(q + 0xB2) = 0;
    }
    else
    {
        if (((u8 *)D_b31f_80174AB4)[0] < ++D_b31f_80174AB8)
        {
            D_b31f_80174AB4 += 2;
            D_b31f_80174AB8 = 0;
            q[0x9B] = 0;
        }
        *(s16 *)(q + 0x2A) = (k & 7) << 9;
        *(u16 *)(q + 0x46) |= 0x8000;
        switch (v & 0xF8)
        {
        case 8:
            func_b31f_8016FCE4(q, b, c, q);
            return 0;
        case 0xC0:
            q[0x9B] = 0;
        case 0xC8:
            func_b31f_801740F8(q, b, c, q);
            break;
        case 0x10:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A2C)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A2C;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A2C), 0);
            }
            break;
        case 0xF8:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174AA4)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174AA4;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174AA4), 0);
            }
            break;
        case 0xF0:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174AAC)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174AAC;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174AAC), 0);
            }
            break;
        case 0xE8:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A9C)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A9C;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A9C), 0);
                func_b31f_80171594(c[0x24], c[0x25], *(s16 *)((u8 *)b + 0xA));
                func_800A152C(0x15, 1);
            }
            break;
        case 0xD8:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A9C)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A9C;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A9C), 0);
                *(u16 *)(c + 0x14) |= 0x800;
            }
            break;
        case 0xD0:
            *(u16 *)(c + 0x14) &= 0x800;
            if (*(u16 *)(c + 0x14) != 0)
            {
                *(u16 *)(c + 0x14) = 0;
            }
            break;
        case 0xE0:
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A7C)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A7C;
                func_80047784(c, *(u8 *)((((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7) + (s32)D_b31f_80174A7C), 0);
                func_b31f_801724D4();
                func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(q + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            }
            break;
        }
    }
    func_800A9A0C(p);
    return 0;
}
