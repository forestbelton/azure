/* func_lo16_8001C06C -- ovl_dng_lo16, 350 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-c06c; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 350 words, target 350 words, 21 aligned differences
       func_lo16_8001C06C: target 350 words, candidate 350 words, 59 differing (masked)

   Last thing the agent reported before it died: nothing; it was testing whether three 8-byte slots are addressable locals rather than reload spills.

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

     RESOLVED FURTHER by agent h181 from the listings: func_lo16_8001C06C
     NEEDS NO ALIAS AT ALL.  It has exactly FOUR call sites, all four inside
     func_lo16_8001AC4C (at 0x8001AF90, 0x8001B048, 0x8001B0C8, 0x8001B17C),
     and at none of them is $v0 READ after the jal or its delay slot -- the
     next insn is `sll $t1, $s7, 16', a `j', `sll $t0, $s7, 16' and
     `lhu $v0, 0x28($sp)' respectively, the last of which WRITES $v0.  So no
     caller witnesses its return type, its own `sll'/`sra' tail is the only
     evidence, and `s16' with ONE declaration is correct.  The arity also
     agrees at every site: four register arguments plus one stack argument at
     0x10($sp), i.e. FIVE parameters, which is what both parks already say.

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

       X  159  159 8001C2E8  lhu t4,32(sp)                            sll $v1, $s7, 16
       +  162                sll v1,t4,0x10
       X  203  202 8001C394  sll v0,s8,0x10                           lui $v1, %hi(D_8006CCD8)
       -       207 8001C3A8                                           lui $v1, %hi(D_8006CCD8)
       -       208 8001C3AC                                           lhu $t4, 0x20($sp)
       -       209 8001C3B0                                           addiu $v1, $v1, %lo(D_8006CCD8)
       +  209                lui v1,0x0
       +  210                addiu v1,v1,0

   h181's OWN WORK ON IT, after the recovery.  The residue is a CLEAN
   TWO-REGISTER EXCHANGE and `-da' gives the arithmetic exactly:

       Register 87  13 refs / 528 insns, pointer   -> got $s7  (hard 23)
       Register 90   5 refs / 149 insns, 2 bytes   -> got $fp  (hard 30)

   87 is `g', the grid descriptor; 90 is `dir'.  global.c ranks on
   `floor_log2(R) * R / L', so g scores 3 * 13 / 528 = 0.0739 and dir
   2 * 5 / 149 = 0.0671; g is allocated first (position 35 of 45 in the
   `;; 45 regs to allocate' list, dir at 36) and takes the lower register.
   THE TARGET HAS THEM THE OTHER WAY ROUND -- `dir' in $s7 and `g' in $fp --
   and every one of the 21 rows is that exchange or its wake.
   ONE more reference to `dir' is arithmetically enough: 2 * 6 / 149 = 0.0805
   beats 0.0739, and dir cannot reach 8 refs (3 * 8 / 149 = 0.161) without
   inventing statements.  Equivalently, g dropping to 11 refs gives
   3 * 11 / 528 = 0.0625, below dir.

   AND EVERY SOURCE ROUTE TO THAT ONE REFERENCE IS PRICED AND CLOSED.  All at
   350 words / 21 rows unless noted, i.e. FLAT, which by the batch rule rules
   out statement order AND allocation priority and sends the next agent to the
   local set -- which is priced below too:
       `do { } while (0)' around `dir2[0] = dir'                     21
       `do { } while (0)' around `dir = (u16) p[3]'            351w / 20
       `do { } while (0)' around the whole `D_8006CCD8[dir]' gate    45
       `g' assigned as the FIRST statement of the body               21
       `g' assigned after the two q reads                            21
       `b' and `g' interleaved into the early reads                  21
       `dir' declared `s32'                                    349w / 22
       `dir' declared `u16'                                    349w / 22
       `dir' declared `u8'                                     347w / 30
       `dir2' declared `u16[1]'                                347w / 26
       MERGED -- `dir' dropped, `dir2[0]' used everywhere      348w / 58
       the late `D_8006CCD8[dir]' test read as `dir2[0]'       348w / 58
       that plus the first `dir2[0]' pair read as `dir'        348w / 58
       that plus the loop pair read as `dir'                   350w / 31
       that plus the else pair read as `dir'                   348w / 58
   So `s16 dir' AND `s16 dir2[1]' must BOTH exist, the late gate must read the
   REGISTER `dir' and every other use the ADDRESSABLE `dir2[0]', and the
   `do { } while (0)' bump -- the one device that is arithmetically right --
   does not move the allocation, which means flow is not counting the extra
   reference where global.c would see it.

   THE DISPATCHER'S STRUCT RECONCILIATION IS FREE HERE, measured: rewriting
   `p' and `q' as `UnkDngLo16Rect *' with the witnessed field signedness
   (`s16 unk_00; u16 unk_02; u8 pad_04[2]; u16 unk_06') and dropping the
   casts is 350 words / 21 rows -- byte-identical.  So the struct form costs
   nothing and should be adopted for consistency with func_lo16_8001AC4C, but
   it is NOT the lever for these 21 rows.

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
   s16 func_800A07D0(s16, s16, s16, s16);
   END DECLARATIONS */

s16 func_lo16_8001C06C(s16 *p, s16 *q, s16 f, s16 *ox, s16 *oy)
{
    u8 *b;
    UnkDngLo16Grid *g;
    s16 y;
    s16 x;
    s16 dir;
    s16 tx[1];
    s16 ty[1];
    s16 dir2[1];
    s16 rx;
    s16 ry;
    s16 sx;
    s16 sy;
    s16 n;
    s32 t;
    s32 d;

    n = 0;
    y = (u16)p[1];
    dir = (u16)p[3];
    dir2[0] = dir;
    x = p[0];
    rx = abs(q[0] - x) - 4;
    b = (u8 *)&D_80083160;
    g = (UnkDngLo16Grid *)(b + 0x1DC);
    ty[0] = (u16)q[1];
    tx[0] = (u16)q[0];
    if (rx <= 0)
    {
        rx = 1;
    }
    sx = rx;
    if (sx > 4)
    {
        sx = 4;
    }
    rx = rx - sx;
    if (rx <= 0)
    {
        rx = 1;
    }
    ry = abs(ty[0] - y) - 4;
    if (ry <= 0)
    {
        ry = 1;
    }
    sy = ry;
    if (sy > 4)
    {
        sy = 4;
    }
    ry = ry - sy;
    if (ry <= 0)
    {
        ry = 1;
    }
    if (f != 0)
    {
        t = func_800A6D30() % rx;
        *ox = t;
        sx = x + t * (u16)D_8006CCD8[dir2[0]] + (u16)D_8006CCD8[dir2[0]] * (u16)sx;
        t = func_800A6D30() % ry;
        *oy = t;
        sy = y + t * (u16)D_8006CCE8[dir2[0]] + (u16)D_8006CCE8[dir2[0]] * (u16)sy;
    }
    else
    {
        sx = x + (u16)*ox * (u16)D_8006CCD8[dir2[0]] + (u16)D_8006CCD8[dir2[0]] * (u16)sx;
        sy = y + (u16)*oy * (u16)D_8006CCE8[dir2[0]] + (u16)D_8006CCE8[dir2[0]] * (u16)sy;
    }
    do
    {
        D_800EA000_s[(y << g->unk_14) + x].unk_00 = 0x68;
        D_800EA000_s[(y << g->unk_14) + x].unk_04 = 1;
        if (x != sx || y != sy)
        {
            n++;
            x += (u16)D_8006CCD8[dir2[0]];
            y += (u16)D_8006CCE8[dir2[0]];
        }
        else
        {
            break;
        }
    } while (1);
    if (sx != tx[0] || sy != ty[0])
    {
        if (D_8006CCD8[dir] != 0)
        {
            sy = ty[0];
        }
        else
        {
            sx = tx[0];
        }
        d = func_800A07D0(x, y, sx, sy);
        do
        {
            D_800EA000_s[(y << g->unk_14) + x].unk_00 = 0x68;
            D_800EA000_s[(y << g->unk_14) + x].unk_04 = 1;
            if (x != sx || y != sy)
            {
                n++;
                x += (u16)D_8006CCD8[(d >> 9) & 6];
                y += (u16)D_8006CCE8[(d >> 9) & 6];
            }
            else
            {
                break;
            }
        } while (1);
    }
    if (sx != tx[0] || sy != ty[0])
    {
        d = func_800A07D0(x, y, tx[0], ty[0]);
        do
        {
            D_800EA000_s[(y << g->unk_14) + x].unk_00 = 0x68;
            D_800EA000_s[(y << g->unk_14) + x].unk_04 = 1;
            if (x != tx[0] || y != ty[0])
            {
                n++;
                x += (u16)D_8006CCD8[(d >> 9) & 6];
                y += (u16)D_8006CCE8[(d >> 9) & 6];
            }
            else
            {
                break;
            }
        } while (1);
    }
    return n;
}
