/* func_lo16_8001C5E4 -- ovl_dng_lo16, 380 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-c5e4; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 379 words, target 380 words, 83 aligned differences
       func_lo16_8001C5E4: target 380 words, candidate 379 words, 223 differing (masked)

   Last thing the agent reported before it died: nothing beyond an early-stage figure.

   CROSS-PARK PROTOTYPE CONFLICT -- SETTLED off the listings by the batch
   dispatcher, 2026-11-25.  The three agents on this cluster were NOT in
   disagreement; they were each right about a different translation unit, and
   the reconciliation is an __asm__ ALIAS ON THE RETURN TYPE, the same device
   as the arity alias:

     RETURN TYPES.  func_lo16_8001C06C and func_lo16_8001C5E4 both END with
     `sll $v0, $s6, 16' / `sra $v0, $v0, 16', so both were compiled with an
     `s16' return type and both DEFINITIONS are `s16'.  But func_lo16_8001AC4C
     does `sll $v0, $v0, 16' then `bltz' after its 8001C5E4 call, which tests
     the sign of the LOW 16 BITS -- gcc emits that only when the declaration
     it compiled against is WIDER than `s16' (with an `s16' prototype the
     value is already extended and you get a bare `bltz', and an explicit
     `(s16)' cast in the source would be a no-op).  So:

         s16 func_lo16_8001C5E4(...);                  the definition
         s32 func_lo16_8001C5E4_i(...)
             __asm__("func_lo16_8001C5E4");            what 8001AC4C calls

     CAVEAT on func_lo16_8001C06C: its result is NOT USED at its first call
     site -- $v0 is never read after that jal or its delay slot -- so the
     caller witnesses nothing there.  Its own sign-extension still gives
     `s16'.  Check the other call sites before adding an alias for it; it may
     need none.

     PARAMETERS.  Both callees read $a0 with a SIGNED/UNSIGNED MIX at distinct
     offsets -- `lh 0x0($a0)', `lhu 0x2($a0)', `lhu 0x6($a0)' -- which is the
     usage-witness for a struct with differently-signed fields.  A plain
     `s16 *' gives `lh' throughout and needs casts to produce the mix, so
     UnkDngLo16Walk IS witnessed and the two callee parks' `s16 *' is the
     weaker reading.  RECONCILE TOWARD THE STRUCT.

     func_800A07D0's three identifiers collapse to ONE alias, its type chosen
     the same way: read whether each call site's use of $v0 implies a
     narrowing.

   THE RESIDUE as aligndiff gives it (`R' = register names only, `X' = a
   differing instruction, `+'/`-' = present on one side only):

       X   96   96 8001C764  andi v0,s5,0xffff                        nop
       X  117  117 8001C7B8  andi v0,s4,0xffff                        nop
       X  130  130 8001C7EC  andi v0,s5,0xffff                        nop
       -       134 8001C7FC                                           nop
       -       135 8001C800                                           lhu $a0, 0x0($t5)
       +  137                lhu a0,0(v1)
       X  143  144 8001C824  andi v0,s4,0xffff                        nop
       X  157  158 8001C85C  lhu t5,32(sp)                            sll $v1, $fp, 16
       +  160                sll v1,t5,0x10
       X  212  212 8001C934  sll v0,s8,0x10                           lui $v1, %hi(D_8006CCD8)
       -       217 8001C948                                           lui $v1, %hi(D_8006CCD8)
       -       218 8001C94C                                           lhu $t5, 0x20($sp)
       -       219 8001C950                                           addiu $v1, $v1, %lo(D_8006CCD8)
       +  218                lui v1,0x0
       +  219                addiu v1,v1,0
          ... and 57 further rows, 68 of them register-name-only

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
   u32 func_800A07D0_u(s16, s16, s16, s16) __asm__("func_800A07D0");
   END DECLARATIONS */

s32 func_lo16_8001C5E4(s16 *p0, s16 *p1, s16 flag, s16 *ox, s16 *oy, s32 *acc, s32 *delta)
{
    u8 *b;
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s32 ang;
    s16 ex;
    s16 ey;
    s16 d;
    s16 d2;
    s16 n;
    s16 nx;
    s16 ny;
    s16 tx;
    s16 ty;
    s16 x;
    s16 y;
    s32 dx;
    s32 dy;
    s32 r;
    u32 a;

    b = (u8 *)&D_80083160;
    g = (UnkDngLo16Grid *)(b + 0x1DC);
    n = 0;
    y = (u16)p0[1];
    x = p0[0];
    d = (u16)p0[3];
    d2 = d;
    ex = (u16)p1[0];
    ey = (u16)p1[1];
    nx = abs(p1[0] - p0[0]) - 4;
    if (nx <= 0)
    {
        nx = 1;
    }
    tx = nx;
    if (tx >= 5)
    {
        tx = 4;
    }
    nx -= tx;
    if (nx <= 0)
    {
        nx = 1;
    }
    ny = abs(ey - y) - 4;
    if (ny <= 0)
    {
        ny = 1;
    }
    ty = ny;
    if (ty >= 5)
    {
        ty = 4;
    }
    ny -= ty;
    if (ny <= 0)
    {
        ny = 1;
    }
    if (flag != 0)
    {
        r = func_800A6D30() % nx;
        *ox = r;
        dx = *(u16 *)(d * 2 + (s32)D_8006CCD8);
        tx = x + r * dx + dx * (u16)tx;
        r = func_800A6D30() % ny;
        *oy = r;
        dy = *(u16 *)(d * 2 + (s32)D_8006CCE8);
        ty = y + r * dy + dy * (u16)ty;
    }
    else
    {
        dx = *(u16 *)(d * 2 + (s32)D_8006CCD8);
        tx = x + (u16)*ox * dx + dx * (u16)tx;
        dy = *(u16 *)(d * 2 + (s32)D_8006CCE8);
        ty = y + (u16)*oy * dy + dy * (u16)ty;
    }
    for (;;)
    {
        ang = ((u16 *)acc)[1] & 0xFFE0;
        c = (UnkDungeonCell *)(((y << g->unk_14) + x) * 6 + (s32)D_800EA000_s);
        if (*(s16 *)&c->unk_02 != 0x4000 && abs(*(s16 *)&c->unk_02 - (s16)ang) >= 0x21)
        {
            return -1;
        }
        *(s16 *)&c->unk_02 = ang;
        if (x == tx && y == ty)
        {
            break;
        }
        *acc += *delta;
        n++;
        x += *(u16 *)(d * 2 + (s32)D_8006CCD8);
        y += *(u16 *)(d * 2 + (s32)D_8006CCE8);
    }
    if (tx != ex || ty != ey)
    {
        if (*(s16 *)(d2 * 2 + (s32)D_8006CCD8) != 0)
        {
            ty = ey;
        }
        else
        {
            tx = ex;
        }
        a = func_800A07D0_u(x, y, tx, ty);
        for (;;)
        {
            ang = ((u16 *)acc)[1] & 0xFFE0;
            c = (UnkDungeonCell *)(((y << g->unk_14) + x) * 6 + (s32)D_800EA000_s);
            if (*(s16 *)&c->unk_02 != 0x4000 && abs(*(s16 *)&c->unk_02 - (s16)ang) >= 0x21)
            {
                return -1;
            }
            *(s16 *)&c->unk_02 = ang;
            if (x == tx && y == ty)
            {
                break;
            }
            *acc += *delta;
            n++;
            x += *(u16 *)(((a >> 10) & 3) * 4 + (s32)D_8006CCD8);
            y += *(u16 *)(((a >> 10) & 3) * 4 + (s32)D_8006CCE8);
        }
    }
    if (tx != ex || ty != ey)
    {
        a = func_800A07D0_u(x, y, ex, ey);
        for (;;)
        {
            ang = ((u16 *)acc)[1] & 0xFFE0;
            c = (UnkDungeonCell *)(((y << g->unk_14) + x) * 6 + (s32)D_800EA000_s);
            if (*(s16 *)&c->unk_02 != 0x4000 && abs(*(s16 *)&c->unk_02 - (s16)ang) >= 0x21)
            {
                return -1;
            }
            *(s16 *)&c->unk_02 = ang;
            if (x == ex && y == ey)
            {
                break;
            }
            *acc += *delta;
            n++;
            x += *(u16 *)(((a >> 10) & 3) * 4 + (s32)D_8006CCD8);
            y += *(u16 *)(((a >> 10) & 3) * 4 + (s32)D_8006CCE8);
        }
    }
    return n;
}
