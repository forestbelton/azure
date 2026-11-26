/* func_lo16_8001AC4C -- ovl_dng_lo16, 1288 target words.
   RECOVERED by agent h181, batch 35, 2026-11-25.

   PROVENANCE -- READ THIS BEFORE TRUSTING ANYTHING BELOW.  This body was
   written by a dedicated sub-agent that was KILLED MID-RUN by a weekly API
   limit before it could write a park of its own.  Its working copy survived in
   sub-ac4c; I have re-measured it and preserved it here so the work is not
   lost.  **THE AGENT'S REASONING AND ITS REJECTED SPELLINGS DIED WITH ITS
   TRANSCRIPT.**  Nothing in this header is a priced fact except the two
   figures I measured myself.  Every spelling in the body is therefore
   UNMEASURED -- do not preserve any of it believing it was fought for, and do
   not write any of it up as a lever.  Splice markers have been stripped.

   MEASURED BY ME, in that copy, just now:

       built 1326 words, target 1288 words, 716 aligned differences
       func_lo16_8001AC4C: target 1288 words, candidate 1326 words, 1306 differing (masked)

   Last thing the agent reported before it died: nothing beyond an early figure; this is the block's largest stub and was briefed as a park-not-land job.

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

       X    0    0 8001AC4C  addiu sp,sp,-192                         addiu $sp, $sp, -0xE0
       X    1    1 8001AC50  sw s3,164(sp)                            sw $s2, 0xC0($sp)
       X    3    3 8001AC58  sw s0,152(sp)                            sw $s3, 0xC4($sp)
       X    8    8 8001AC6C  sw s6,176(sp)                            sw $s4, 0xC8($sp)
       X   14   14 8001AC84  sw s2,160(sp)                            sw $s1, 0xBC($sp)
       X   20   20 8001AC9C  sw s8,184(sp)                            sw $s5, 0xCC($sp)
       X   24   24 8001ACAC  sh t0,104(sp)                            sh $t0, 0x60($sp)
       X   26   26 8001ACB4  sw s1,156(sp)                            sw $s0, 0xB8($sp)
       X   28   28 8001ACBC  sw v0,112(sp)                            sw $v0, 0x78($sp)
       -        32 8001ACCC                                           sw $fp, 0xD8($sp)
       X   33   34 8001ACD4  sw ra,188(sp)                            sw $ra, 0xDC($sp)
       X   34   35 8001ACD8  sw s7,180(sp)                            sw $s7, 0xD4($sp)
       X   35   36 8001ACDC  sw s5,172(sp)                            sw $s6, 0xD0($sp)
       X   36   37 8001ACE0  sw s4,168(sp)                            sh $zero, 0x80($sp)
       X   37                sh zero,120(sp)
       X   38                sh t1,96(sp)
       X   40   39 8001ACE8  sw v0,116(sp)                            sh $t1, 0x58($sp)
       X   41                sll v0,s0,0x10
       X   42                sra v0,v0,0x10
       +   46                lw a3,116(sp)
       X   49   45 8001AD00  sw v0,136(sp)                            addu $a3, $fp, $zero
       X   50   46 8001AD04  lw t4,112(sp)                            lw $t2, 0x78($sp)
       X   53   49 8001AD10  nop                                      addu $s7, $s2, $zero
       X   55   51 8001AD18  sh s0,88(sp)                             sh $s2, 0x88($sp)
       +   69                lw t5,116(sp)
       +   70                nop
          ... and 690 further rows, 352 of them register-name-only

   NEXT AGENT: re-derive the residue yourself and price with aligndiff, never
   with ovlverify's masked count -- on a body this size the masked count
   reports every word downstream of a single fault.  Then work the FIRST
   non-register row; the rest are usually its wake.

   DECLARATIONS
       s8 unk_02;
   typedef struct UnkDngLo16Walk
   {
       s16 unk_00;
       u16 unk_02;
       u8 pad_04[0x2];
       u16 unk_06;
   } UnkDngLo16Walk;
   RECONSTRUCTED 2026-11-25: this typedef had been truncated to its first
      field, so the body's `.unk_02' and `.unk_06' would not compile and the
      park could not be measured at all. The field set and the SIGNEDNESS come
      from the usage-witness recorded in docs/dng-parks/func_lo16_8001C06C.c,
      which is the callee both `&a' and `&b' are passed to: it reads $a0 as
      `lh 0x0', `lhu 0x2', `lhu 0x6' -- a signed/unsigned MIX at distinct
      offsets, which is why that park concludes the struct is witnessed and
      says to reconcile toward it rather than toward `s16 *'. The truncated
      version had `u16 unk_00' where the witness says `lh', i.e. signed.
   void func_lo16_800177D8_h(s16, s16, s16, s16) __asm__("func_lo16_800177D8");
   void func_800A6D60(s32);
   u32 func_800A07D0u(s32, s32, s32, s32) __asm__("func_800A07D0");
   s32 func_lo16_8001C06C(UnkDngLo16Walk *, UnkDngLo16Walk *, s32, s16 *, s16 *);
   s16 func_lo16_8001C5E4(UnkDngLo16Walk *, UnkDngLo16Walk *, s32, s16 *, s16 *, s32 *, s32 *);
   void func_lo16_8001CCEC(s16 d, s16 *ox, s16 *oy, s16 *p);
   s32 func_lo16_8001CE14(s16 a, s16 b, s16 c);
   END DECLARATIONS */

/* DRAFT */
s32 func_lo16_8001AC4C(s16 d, s16 e, s16 r1, s16 r2, u8 *flags)
{
    UnkDngLo16Walk a;
    UnkDngLo16Walk b;
    UnkDngLo16Walk cc;
    s16 ox1;
    s16 oy1;
    s16 ox2;
    s16 oy2;
    s16 dx;
    s16 dy;
    s32 h0;
    s32 hs;
    s32 h1;
    UnkDngRoom2970 *p1;
    UnkDngRoom2970 *p2;
    UnkDungeonCell *c1;
    UnkDungeonCell *c2;
    s16 cnt;
    s16 dir;
    s16 k;
    s16 w;
    s32 bx;
    s32 by;
    s32 nx;
    s32 ny;
    s32 ex;
    s32 ey;
    s32 cx;
    s32 cy;
    s32 tx;
    s32 ty;
    s32 i;
    s32 ha;
    s32 hb;
    s32 t;

    p1 = &D_800E2970_s[r1];
    p2 = &D_800E2970_s[r2];
    cnt = 0;
    func_lo16_8001CCEC(d, &ox1, &oy1, (s16 *)p1);
    func_lo16_8001CCEC((e + 4) & 6, &ox2, &oy2, (s16 *)p2);

    if (p1->unk_0A != 0)
    {
        func_lo16_800177D8_h(r1, p1->unk_00 + (u16)ox1, p1->unk_02 + (u16)oy1, 0);
    }
    if (p2->unk_0A != 0)
    {
        func_lo16_800177D8_h(r2, p2->unk_00 + (u16)ox2, p2->unk_02 + (u16)oy2, 0);
    }
    func_800A6D60(1);

    dir = d;
    bx = p1->unk_00 + (u16)ox1;
    by = p1->unk_02 + (u16)oy1;
    a.unk_00 = bx;
    a.unk_06 = dir;
    a.unk_02 = by;
    if (p1->unk_0A != 0)
    {
        a.unk_00 = bx + (u16)D_8006CCD8[dir];
        a.unk_02 = by + (u16)D_8006CCE8[dir];
    }

    cx = (s16)(r1 % *(s32 *)D_8001F660) + D_8006CCD8[dir];
    cy = (s16)(r1 / *(s32 *)D_8001F660) + D_8006CCE8[dir];
    tx = (s16)(r2 % *(s32 *)D_8001F660);
    ty = (s16)(r2 / *(s32 *)D_8001F660);
    while (1)
    {
        if (cx == tx && cy == ty)
        {
            if (D_800E2970_s[r2].unk_0A != 0)
            {
                b.unk_00 = p2->unk_00 + (u16)ox2;
                b.unk_02 = p2->unk_02 + (u16)oy2;
            }
            else
            {
                b.unk_00 = p2->unk_00;
                b.unk_02 = p2->unk_02;
            }
            b.unk_00 -= (u16)D_8006CCD8[e];
            b.unk_02 -= (u16)D_8006CCE8[e];
            cnt += func_lo16_8001C06C(&a, &b, 1, &dx, &dy);
            a.unk_00 += (u16)D_8006CCD8[(dir + 2) & 7];
            a.unk_02 += (u16)D_8006CCE8[(dir + 2) & 7];
            b.unk_00 += (u16)D_8006CCD8[(e + 2) & 7];
            b.unk_02 += (u16)D_8006CCE8[(e + 2) & 7];
            dx -= (u16)D_8006CCD8[dir];
            dy -= (u16)D_8006CCE8[dir];
            func_lo16_8001C06C(&a, &b, 0, &dx, &dy);
            break;
        }
        i = (s16)(cy * *(s32 *)D_8001F660 + cx);
        flags[i] = 2;
        b.unk_00 = D_800E2970_s[i].unk_00;
        b.unk_02 = D_800E2970_s[i].unk_02;
        cnt += func_lo16_8001C06C(&a, &b, 1, &dx, &dy);
        cc = a;
        cc.unk_00 += (u16)D_8006CCD8[(dir + 2) & 7];
        cc.unk_02 += (u16)D_8006CCE8[(dir + 2) & 7];
        dx -= (u16)D_8006CCD8[dir];
        dy -= (u16)D_8006CCE8[dir];
        func_lo16_8001C06C(&cc, &b, 0, &dx, &dy);
        a.unk_00 = b.unk_00;
        a.unk_02 = b.unk_02;
        if (D_8006CCD8[dir] != 0)
        {
            if (cx == tx)
            {
                a.unk_06 = (func_800A07D0u((s16)cx, (s16)cy, cx, (s16)(r2 / *(s32 *)D_8001F660)) >> 9) & 6;
            }
        }
        else if (cy == ty)
        {
            a.unk_06 = (func_800A07D0u((s16)cx, (s16)cy, (s16)(r2 % *(s32 *)D_8001F660), cy) >> 9) & 6;
        }
        dir = a.unk_06;
        cx += D_8006CCD8[a.unk_06];
        cy += D_8006CCE8[a.unk_06];
    }

    k = (d + 4) & 7;
    bx = p1->unk_00 + ox1;
    by = p1->unk_02 + oy1;
    c1 = (UnkDungeonCell *)(((by << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + bx) * 6 + (s32)D_800EA000_s);
    c2 = (UnkDungeonCell *)((((by + D_8006CCE8[k]) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + bx + D_8006CCD8[k]) * 6 +
                            (s32)D_800EA000_s);
    ex = p2->unk_00 + ox2;
    ey = p2->unk_02 + oy2;

    if (func_lo16_8001CE14(c1->unk_00, 0xF, 0x12) != 0)
    {
        c1->unk_00 = (c2->unk_04 >> 1) + d / 2 * 5 + 0x4F;
        c1->unk_02 = c2->unk_02 - ((c2->unk_04 & 1) << 5);
        if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
        {
            c1->unk_02 += 0x60;
        }
    }
    if (func_lo16_8001CE14(c2->unk_00, 0x13, 0x1C) != 0)
    {
        c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
    }
    else if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
    {
        c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
        c2->unk_02 += 0x60;
    }

    nx = bx + D_8006CCD8[(d + 2) & 7];
    ny = by + D_8006CCE8[(d + 2) & 7];
    c1 = (UnkDungeonCell *)(((ny << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + nx) * 6 + (s32)D_800EA000_s);
    c2 = (UnkDungeonCell *)((((ny + D_8006CCE8[k]) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + nx + D_8006CCD8[k]) * 6 +
                            (s32)D_800EA000_s);
    if (func_lo16_8001CE14(c1->unk_00, 0xF, 0x12) != 0)
    {
        c1->unk_00 = (c2->unk_04 >> 1) + d / 2 * 5 + 0x4F;
        c1->unk_02 = c2->unk_02 - ((c2->unk_04 & 1) << 5);
        if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
        {
            c1->unk_02 += 0x60;
        }
    }
    if (func_lo16_8001CE14(c2->unk_00, 0x13, 0x1C) != 0)
    {
        c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
    }
    else if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
    {
        c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
        c2->unk_02 += 0x60;
    }

    if (D_800E2970_s[r2].unk_0A != 0)
    {
        k = e;
        c1 = (UnkDungeonCell *)(((ey << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + ex) * 6 + (s32)D_800EA000_s);
        c2 = (UnkDungeonCell *)((((ey + D_8006CCE8[k]) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) +
                                 (ex + D_8006CCD8[k])) *
                                    6 +
                                (s32)D_800EA000_s);
        w = (e + 4) & 6;
        if (func_lo16_8001CE14(c1->unk_00, 0xF, 0x12) != 0)
        {
            c1->unk_00 = (c2->unk_04 >> 1) + w / 2 * 5 + 0x4F;
            c1->unk_02 = c2->unk_02 - ((c2->unk_04 & 1) << 5);
            if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
            {
                c1->unk_02 += 0x60;
            }
        }
        if (func_lo16_8001CE14(c2->unk_00, 0x13, 0x1C) != 0)
        {
            c2->unk_00 = c2->unk_04 + w / 2 * 10 + 0x27;
        }
        else if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
        {
            c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
            c2->unk_02 += 0x60;
        }

        nx = ex + D_8006CCD8[(w - 2) & 7];
        ny = ey + D_8006CCE8[(w - 2) & 7];
        c1 = (UnkDungeonCell *)(((ny << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + nx) * 6 + (s32)D_800EA000_s);
        c2 = (UnkDungeonCell *)((((ny + D_8006CCE8[k]) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + nx +
                                 D_8006CCD8[k]) *
                                    6 +
                                (s32)D_800EA000_s);
        if (func_lo16_8001CE14(c1->unk_00, 0xF, 0x12) != 0)
        {
            c1->unk_00 = (c2->unk_04 >> 1) + w / 2 * 5 + 0x4F;
            c1->unk_02 = c2->unk_02 - ((c2->unk_04 & 1) << 5);
            if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
            {
                c1->unk_02 += 0x60;
            }
        }
        if (func_lo16_8001CE14(c2->unk_00, 0x13, 0x1C) != 0)
        {
            c2->unk_00 = c2->unk_04 + w / 2 * 10 + 0x27;
        }
        else if (func_lo16_8001CE14(c2->unk_00, 0x63, 0x67) != 0)
        {
            c2->unk_00 = c2->unk_04 + d / 2 * 10 + 0x27;
            c2->unk_02 += 0x60;
        }
    }

    func_800A6D60(0);
    dir = d;
    if (D_800E2970_s[r1].unk_0A != 0)
    {
        ha = (s16)D_800EA000_s[((p1->unk_02 + oy1) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + p1->unk_00 + ox1].unk_02;
    }
    else
    {
        ha = (s16)D_800EA000_s[(p1->unk_02 << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + p1->unk_00].unk_02;
    }
    if (D_800E2970_s[r2].unk_0A != 0)
    {
        hb = (s16)D_800EA000_s[((p2->unk_02 + oy2) << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + p2->unk_00 + ox2].unk_02;
    }
    else
    {
        hb = (s16)D_800EA000_s[(p2->unk_02 << ((UnkDngLo16Grid *)D_8008333C)->unk_14) + p2->unk_00].unk_02;
    }
    if (cnt == 0)
    {
        cnt = 1;
    }
    t = hb - ha;
    t = t < 0 ? -t : t;
    if (cnt < (t + 31) / 32)
    {
        return 1;
    }
    hs = (t << 16) / cnt;
    if (hb < ha)
    {
        hs = -hs;
    }
    h1 = ha << 16;
    h0 = ha << 16;

    a.unk_00 = p1->unk_00 + (u16)ox1;
    a.unk_06 = dir;
    a.unk_02 = p1->unk_02 + (u16)oy1;
    if (D_800E2970_s[r1].unk_0A != 0)
    {
        a.unk_00 = p1->unk_00 + (u16)ox1 + (u16)D_8006CCD8[dir];
        a.unk_02 = p1->unk_02 + (u16)oy1 + (u16)D_8006CCE8[dir];
    }

    cx = (s16)(r1 % *(s32 *)D_8001F660) + D_8006CCD8[dir];
    cy = (s16)(r1 / *(s32 *)D_8001F660) + D_8006CCE8[dir];
    tx = (s16)(r2 % *(s32 *)D_8001F660);
    ty = (s16)(r2 / *(s32 *)D_8001F660);
    while (1)
    {
        if (cx == tx && cy == ty)
        {
            if (D_800E2970_s[r2].unk_0A != 0)
            {
                b.unk_00 = p2->unk_00 + (u16)ox2;
                b.unk_02 = p2->unk_02 + (u16)oy2;
            }
            else
            {
                b.unk_00 = p2->unk_00;
                b.unk_02 = p2->unk_02;
            }
            b.unk_00 -= (u16)D_8006CCD8[e];
            b.unk_02 -= (u16)D_8006CCE8[e];
            if (func_lo16_8001C5E4(&a, &b, 1, &dx, &dy, &h0, &hs) < 0)
            {
                return 1;
            }
            a.unk_00 += (u16)D_8006CCD8[(dir + 2) & 7];
            a.unk_02 += (u16)D_8006CCE8[(dir + 2) & 7];
            b.unk_00 += (u16)D_8006CCD8[(e + 2) & 7];
            b.unk_02 += (u16)D_8006CCE8[(e + 2) & 7];
            dx -= (u16)D_8006CCD8[dir];
            dy -= (u16)D_8006CCE8[dir];
            if (func_lo16_8001C5E4(&a, &b, 0, &dx, &dy, &h1, &hs) < 0)
            {
                return 1;
            }
            return 0;
        }
        i = (s16)(cy * *(s32 *)D_8001F660 + cx);
        flags[i] = 2;
        b.unk_00 = D_800E2970_s[i].unk_00;
        b.unk_02 = D_800E2970_s[i].unk_02;
        if (func_lo16_8001C5E4(&a, &b, 1, &dx, &dy, &h0, &hs) < 0)
        {
            return 1;
        }
        cc = a;
        cc.unk_00 += (u16)D_8006CCD8[(dir + 2) & 7];
        cc.unk_02 += (u16)D_8006CCE8[(dir + 2) & 7];
        dx -= (u16)D_8006CCD8[dir];
        dy -= (u16)D_8006CCE8[dir];
        if (func_lo16_8001C5E4(&cc, &b, 0, &dx, &dy, &h1, &hs) < 0)
        {
            return 1;
        }
        a.unk_00 = b.unk_00;
        a.unk_02 = b.unk_02;
        if (D_8006CCD8[dir] != 0)
        {
            if (cx == tx)
            {
                a.unk_06 = (func_800A07D0u((s16)cx, (s16)cy, cx, (s16)(r2 / *(s32 *)D_8001F660)) >> 9) & 6;
            }
        }
        else if (cy == ty)
        {
            a.unk_06 = (func_800A07D0u((s16)cx, (s16)cy, (s16)(r2 % *(s32 *)D_8001F660), cy) >> 9) & 6;
        }
        dir = a.unk_06;
        cx += D_8006CCD8[a.unk_06];
        cy += D_8006CCE8[a.unk_06];
    }
}
