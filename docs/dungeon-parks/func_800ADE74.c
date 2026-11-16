/* ===========================================================================
   BEST MEASURED CANDIDATE for func_800ADE74, ovl_dungeon (1817 words).
   2026-11-16, agent j36.  Supersedes j34's uncompiled decode (the structural
   reading in that file was correct and is what this was built from; the
   per-instruction claims below are what compiling it established).

   MEASURED: built 1820 words, target 1817 words, 322 aligned differences
   (tools/decomp/aligndiff.py).  ovlverify's masked count is meaningless
   while the word counts differ.  Most of the 322 are branch-offset rows
   that aligndiff cannot mask; the structural residue is the ~25 rows listed
   under RESIDUE.

   The three alias declarations at the top of this file are required and are
   NOT yet in csrc/ovl_dungeon.c's declaration block.
   ===========================================================================

   WHAT THE COMPILER CONFIRMED (none of this was in the decode)

   * The 36-entry jump table needs NO written-out empty cases.  gcc emits a
     dense table from the lowest case label to the highest, so `case 9` and
     `case 0x2C` alone give all 36 entries with the gaps pointing at the
     default label.  j34's note that 11..20/0x1E/0x1F/0x21 "must be written
     out or the table comes out short" is wrong.
   * The six shared tails are NOT a goto ladder and are not written out per
     arm either.  `.L800AF89C` is simply THE CODE AFTER THE SWITCH, every arm
     reaches it with a plain `break`, and the three entry points
     .L800AF89C/.L800AF8A0/.L800AF8A4 are the delay-slot scheduler stealing
     one or two instructions from the head of that block (`addu $a0,$s7` /
     `addu $a1,$s5`) and bumping the branch target.  Written this way the
     whole tail structure falls out with no gotos anywhere in the function.
   * The facing preamble has NO separate index variable: it is
     `sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;` and then `D_8006CCD8[sp1E]`.
     With a separate `s32 u` gcc puts the index in a second register and
     defers the `sh`, costing a word of shuffling in each of the nine copies.
   * `ret` is an `s32` returned as `(u16)ret`, not a `u16` local.  MIPS
     PROMOTE_MODE keeps a `u16` local in SImode already zero-extended, so
     `return ret;` compiles to a bare `move v0,s8` and the target's
     `sll $v0,$fp,16` / `srl $v0,$v0,16` pair never appears.
   * `(s16)func_8009FD40(...) < (s16)func_800A35A4(...)` does NOT give the
     target's `sll`/`sll`/`slt`.  func_800A35A4's landed return type is `u8`,
     so the cast leaves an `andi $v0,$v0,0xFF` and the compare degrades to
     `sra`/`slt` -- 5 extra words across the three copies.  C's
     shorten_compare gives the `sll`/`sll` form only when BOTH operands are
     short-typed rvalues, so the call needs an s16-returning alias and the
     casts have to come OFF.
   * `(u16)(r - 2) < 7` in cases 0x2C and 0x2A needs the func_8009FD40 result
     UNEXTENDED.  Assigned to an `s32` through the landed `s16` return it
     arrives as `sll`/`sra` and gcc then drops the `andi $v0,$v0,0xFFFF`; an
     s32-returning alias gives the target's bare `addu $s0,$v0,$zero`.
   * func_800A40AC's landed `s16` return costs a `sra` in case 2's throw
     path for the same reason; an s32-returning alias removes it.
   * `D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2` is csrc/ovl_dungeon.c's own
     spelling (line ~32172) and matched first time, 0x14 stride and all.
   * func_800A0E6C is called WITHOUT a prototype, exactly as the unit's
     later call site does; adding one would change that landed call.

   RESIDUE (+3 words, and the rows that are not branch offsets)

   1. case 0x1D's tail does not cross-jump into case 0x26's at .L800AEEFC.
      The nine instructions are textually identical in both arms in my build
      and gcc still emitted both copies: +8 words.  Biggest single item.
   2. case 0x18's last `*(u16 *)(c+0x46) = 0x800B; return 2;` does not
      cross-jump into .L800AF8F0 either: gcc puts the 0x800B in $v1 and the
      return value in $v0 in the wrong order, so the texts differ: +3 words.
   3. .L800AE010's `func_800A0818(...); c[0x2A]=; c[0x46]=0x800B; return 2;`
      cross-jumps in MY build and does NOT in the target: -5 words.  The
      target blocks the merge by holding 0x800B in $v1 there and in $v0 at
      .L800AF8F0; the same $v0/$v1 question as (2), opposite sign.
   4. The three `nd = *(u8 **)(q - 0x14)` loads in cases 0/1/2 land in the
      `beqz` delay slot in my build and sit after an unfilled one in the
      target: -3 words.
   5. The claim block reloads D_800814A8 once more than the target
      (`lw v0,0(s6)` / `nop` / `addu v0,s4,v0` against a bare
      `addu $v1,$s4,$v1`): +2 words.
   6. case 0x24's Chebyshev test: the `m`/dx/dy shape below is close but the
      target's `addu $s0,$v1,$zero` lands before the `sh $v1,0x18($sp)` and
      mine after.  Net 0 words, 2 rows.
   7. case 0x2A has one extra `sra` around the func_800BCB04 result (+1).
   8. Register rows: `r` is one pseudo used at six sites, so gcc gives it
      $s0 where the target has $v1/$a0 (case 0, case 1, case 4, the
      fallback).  Splitting it into one variable per site DOES move it to a
      caller-saved register -- but it also reshuffles everything else and
      moved `c` out of $s5 into $s3, taking the count from 322 to 598.  Do
      not split variables without re-checking which register `c` lands in.

   SPELLINGS MEASURED AND REJECTED
   * `u16 ret` + `return ret;`            -- loses the sll/srl pair, 1 word.
   * separate `s32 u` for the facing index -- +1 word x 9 arms of shuffling.
   * `(s16)func_800A35A4(c, sp1C)` with the landed u8 return -- +5 words.
   * `s32 r = func_8009FD40(...)` with the landed s16 return -- loses the
     `andi 0xFFFF` in cases 0x2C and 0x2A, 2 words.
   * one variable per use site for q/nd/i/j/r/t/g/h -- 1829 words, 598 rows.
   =========================================================================== */

s16 func_800A35A4h(u8 *, s32) __asm__("func_800A35A4");
s32 func_8009FD40w(u8 *, u8 *) __asm__("func_8009FD40");
s32 func_800A40ACw(u8 *, u16) __asm__("func_800A40AC");

s32 func_800ADE74(u8 *a, u8 *b, u8 *c, u16 d, u16 e, u8 *f)
{
    s32 ret;
    s16 sp18;
    s16 sp1A;
    s16 sp1C;
    s16 sp1E;
    s16 sp20;
    u8 *g;
    u8 *q;
    u8 *nd;
    u8 *o;
    u8 k;
    u8 k2;
    u16 h;
    s32 i;
    s32 j;
    s32 r;
    s32 t;
    s32 n;
    s32 nn;
    s32 aa;
    s32 ang;
    s32 dx;
    s32 dy;
    s16 mx;

    ret = 2;
    *(u16 *)(c + 0x46) |= 0x4000;
    if ((D_80013714 & 8) != 0)
    {
        func_800A9A0C(c);
        return -1;
    }
    if (c[0x25] == 0)
    {
        ret = 0;
        return (u16)ret;
    }
    if ((s16)func_800A2CAC(c) != 0 && (s16)func_800A6E8C(b, 0x60C, &sp18, &sp1A) != 0 &&
        c[0x13] != 0x22)
    {
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        if (sp18 == b[0x24] && sp1A == b[0x25])
        {
            r = (s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88));
            if (r >= 0)
            {
                t = *(u16 *)(c + 0x24);
                if (t > 0x10000)
                {
                    t = 0xFFFF;
                }
                *(u16 *)(c + 0x24) = t;
                if (c[0x25] != 0)
                {
                    *(s32 *)(c + 0x1C) &= ~0x8;
                }
                if (c[0x66] < c[0x25])
                {
                    c[0x25] = c[0x66];
                    c[0x24] = 0xFF;
                }
                func_80098B38(D_800E3548 + r * 4);
            }
            return 0;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)&sp1C);
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;
    }
    func_800A19E4((s32)b, c, (s16)d, (s16)e, f);
    if ((*(s32 *)(c + 0x1C) & 0x2410) == 0x2000)
    {
        k = c[0x12];
        if ((*(s32 *)(D_800E3D7C + 0x1C) & 0x220) != 0 ||
            (s16)func_80042900((s8 *)D_800E3D7C, 0xA) != 0)
        {
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && k != 0)
            {
                k = 1;
            }
        }
        switch (k)
        {
        case 0:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518(D_800E3D7C) != 0)
                    {
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q != NULL)
                        {
                            nd = *(u8 **)(q - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q = func_800A3D18(b, c, 4);
            *(u8 **)(c + 0x60) = q;
            if (q == NULL)
            {
                break;
            }
            r = func_800A384C(c, q, (u8 *)&sp1E, 0);
            sp20 = r;
            *(s16 *)(c + 0x2A) = sp1E;
            if ((s16)r < 0)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r | 0x8000;
            return 1;

        case 1:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q != NULL)
                        {
                            nd = *(u8 **)(q - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            q = func_800A3D18(b, c, -3);
            *(u8 **)(c + 0x60) = q;
            if (q == NULL)
            {
                break;
            }
            r = func_800A384C(c, q, (u8 *)&sp1E, 0);
            sp20 = r;
            *(s16 *)(c + 0x2A) = sp1E;
            if ((s16)r < 4)
            {
                *(u16 *)(c + 0x46) = 0x800B;
                return 2;
            }
            *(u16 *)(c + 0x46) = r | 0x8000;
            return 1;

        case 2:
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) < func_800A35A4h(c, sp1C) &&
                        (s16)func_800A3518((u8 *)D_800814A8) != 0)
                    {
                        if ((s16)func_800A2C34(c) != 0)
                        {
                            return -1;
                        }
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q = *(u8 **)(D_800814A8 + j * 4 + 0xAC);
                        if (q != NULL)
                        {
                            nd = *(u8 **)(q - 0x14);
                            if (func_8009FD40(b, nd) < func_800A35A4h(c, sp1C) &&
                                (s16)func_800A3518(*(u8 **)(D_800814A8 + j * 4 + 0xAC)) != 0)
                            {
                                if ((s16)func_800A2C34(c) != 0)
                                {
                                    return -1;
                                }
                                *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                                if (*(u8 **)(D_800814A8 + j * 4 + 0xAC) != c)
                                {
                                    *(s16 *)(c + 0x2A) =
                                        func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25],
                                                      (u16 *)&sp1C);
                                }
                                *(u32 *)(c + 0x60) = *(u32 *)(D_800814A8 + j * 4 + 0xAC);
                                return 4;
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 && (s16)func_800A404C() == 0 &&
                *(u8 *)(D_800814A8 + 0x9A) == 0x11)
            {
                o = *(u8 **)(D_800814A8 + 0x60);
                if (o != NULL && (*(s32 *)(o + 0x14) & 0x2000) == 0)
                {
                    r = func_800A40ACw(c, func_8009CD58(o, 7, 0));
                    sp20 = r;
                    if ((s16)r >= 0)
                    {
                        *(u16 *)(c + 0x46) = r | 0x8000;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], (u16 *)&sp20);
                        return 3;
                    }
                }
            }
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            break;

        case 4:
            if ((s16)func_800A2C34(c) != 0)
            {
                return -1;
            }
            ret = 0;
            q = func_800A03C4(c, b[0x24], b[0x25]);
            *(u8 **)(c + 0x60) = q;
            if (q != NULL)
            {
                r = func_800A384C(c, q, (u8 *)&sp1E, 0);
                sp20 = r;
                if ((s16)r >= 0)
                {
                    *(u16 *)(c + 0x46) = r | 0x8000;
                    *(s16 *)(c + 0x2A) = sp1E;
                    return 1;
                }
            }
            break;

        default:
            break;
        }
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        return (u16)ret;
    }

    if ((s16)func_800A2C34(c) != 0)
    {
        return -1;
    }
    if ((*(s32 *)(c + 0x1C) & 0x10) != 0)
    {
        return (u16)ret;
    }
    if (*(s8 *)(b + 0x26) >= 0 && (D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2) != 0)
    {
        return (u16)ret;
    }
    if ((s16)func_80042900((s8 *)c, 6) == 0)
    {
        sp1C = func_800A3544(c, 0x1E);
        if (sp1C >= 0 && (s16)func_800A3518(c) != 0)
        {
            *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
            *(u8 **)(c + 0x60) = c;
            return 4;
        }
    }
    switch (c[0x13])
    {
    case 0x19:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g[0x13] != 0)
        {
            break;
        }
        if ((s16)func_800A3518(c) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x800A;
        return 6;

    case 0x16:
        if (c[0x48] != 0xF)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) >= 0xB)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        if (func_800A36B4(c, D_800E3D7C) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8008;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 1;

    case 0x2C:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        r = func_8009FD40w(D_80082E80, b);
        if ((u16)(r - 2) >= 7)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        t = (s16)func_800A0134(c, D_800E3D7C);
        if (t < 0)
        {
            t = -t;
        }
        if (t >= 0x21)
        {
            break;
        }
        g = (u8 *)func_800A05A4(c, b[0x24], b[0x25], *(s16 *)(c + 0x2A), (s16)r);
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if (func_800A36B4(c, g) == 0)
        {
            break;
        }
        if (c[0xAE] != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x2B:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if ((*(u16 *)(c + 0x98) & 0x8000) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    case 0x22:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s8 *)(b + 0x26) < 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL && (s16)func_800A2CAC(g) != 0)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                                   D_80082E80[0x25], (u16 *)&sp20);
                return 5;
            }
        }
        break;

    case 0x29:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t = (s16)func_800A0134(c, g);
                if (t < 0)
                {
                    t = -t;
                }
                if (t < 0x21 && (s16)func_800A41F0(g) != 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1 && func_800C7F68(g) == 0)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(u8 **)(c + 0xA8) = g;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 0x28:
        func_800ADD20(c, 0x40);
        h = *(u16 *)(c + 0x98);
        if ((h & 0x100) != 0)
        {
            *(u16 *)(c + 0x98) = h | 0x8000;
            break;
        }
        *(u16 *)(c + 0x98) = h & 0x7FFF;
        break;

    case 0x1A:
        func_800ADD20(c, 2);
        h = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h & 0x7FFF;
        if ((h & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 1) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x98) |= 0x8000;
        break;

    case 0x23:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (*(s16 *)(c + 0xA6) != 0)
        {
            break;
        }
        if ((D_800E296C & 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x26:
        func_800ADD20(c, 2);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x27:
        func_800ADD20(c, 0x10);
        h = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h & 0x7FFF;
        if ((h & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((s16)func_800A2CB8(c, g) == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 2) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(u16 *)(c + 0x98) |= 0x8000;
        return 5;

    case 0x20:
        if (c[0x49] == 0)
        {
            func_800ADD20(c, 2);
            if ((*(u16 *)(c + 0x98) & 0x100) == 0)
            {
                break;
            }
            sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
            if (func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                              *(s16 *)(c + 0x88)) != D_800E3D7C)
            {
                break;
            }
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        if ((*(s32 *)(c + 0x1C) & 0x410) != 0)
        {
            break;
        }
        func_800AA53C(c);
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            return 2;
        }
        if (*(s8 *)(b + 0x26) >= 0)
        {
            func_800A0E6C(b, *(s8 *)(c + 0x9C), c, c + 0x98);
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        if ((s16)func_8009FD40(b, D_80082E80) != 1)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98)) +
                             0x800;
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x24:
        h = *(u16 *)(c + 0x98);
        *(u16 *)(c + 0x98) = h & 0x7FFF;
        if (c[0x49] != 0)
        {
            break;
        }
        if ((s16)func_800A6E8C(b, 0x12, &sp18, &sp1A) == 0)
        {
            break;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)(c + 0x98));
        *(u16 *)(c + 0xA8) = sp18;
        *(u16 *)(c + 0xAA) = sp1A;
        *(u16 *)(c + 0x98) |= 0x8000;
        dx = b[0x24] - sp18;
        if (dx < 0)
        {
            dx = -dx;
        }
        mx = dx;
        sp18 = dx;
        dy = b[0x25] - sp1A;
        if (dy < 0)
        {
            dy = -dy;
        }
        sp1A = dy;
        if (sp18 < sp1A)
        {
            mx = sp1A;
        }
        if (mx == 1)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x25:
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x18:
        if ((s16)func_8009FD40(b, D_80082E80) >= 2 && c[0xB5] == 0)
        {
            func_800ADD20(c, 8);
            if ((*(u16 *)(c + 0x98) & 0x100) != 0)
            {
                *(u16 *)(c + 0x46) = 0x800A;
                return 6;
            }
        }
        if (c[0xB5] == 0)
        {
            break;
        }
        g = func_8009B25C(c, b[0x24], b[0x25], *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)(c + 0x98));
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;

    case 0x2A:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        if (func_800A365C(b, D_80082E80) == 0)
        {
            break;
        }
        r = func_8009FD40w(D_80082E80, b);
        if ((u16)(r - 2) >= 7)
        {
            break;
        }
        ang = *(u16 *)(c + 0x88);
        sp1E = (func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                              (u16 *)&sp20) >>
                9) &
               7;
        i = 0;
        sp18 = b[0x24];
        sp1A = b[0x25];
        for (; i < (s16)r; i++)
        {
            sp18 = sp18 + (u16)D_8006CCD8[sp1E];
            sp1A = sp1A + (u16)D_8006CCE8[sp1E];
            if ((s16)func_800A45D8(sp18 << 6, sp1A << 6, (s16)(ang + 0x20)) != 0)
            {
                break;
            }
            n = func_800BCB04(((sp18 << 6) + 0x20) & 0xFFE0, ((sp1A << 6) + 0x20) & 0xFFE0,
                                (s16)(ang - 0x20));
            nn = (s16)n;
            aa = (s16)ang;
            if (nn < aa)
            {
                break;
            }
            if (nn - aa >= 0x21)
            {
                break;
            }
            ang = n;
        }
        if (i != (s16)r)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1B:
        func_800ADD20(c, 4);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        for (i = 0; i < 2; i++)
        {
            g = *(u8 **)(D_800E3D7C + i * 4 + 0xAC);
            if (g != NULL)
            {
                t = (s16)func_800A0134(c, g);
                if (t < 0)
                {
                    t = -t;
                }
                if (t < 0x21 && (s16)func_800A41F0(g) != 0 &&
                    (s16)func_80042900((s8 *)g, 0xC) == 0)
                {
                    nd = *(u8 **)(g - 0x14);
                    if ((s16)func_8009FD40(nd, b) == 1)
                    {
                        *(u16 *)(c + 0x46) = 0x8009;
                        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24],
                                                           nd[0x25], (u16 *)&sp20);
                        return 5;
                    }
                }
            }
        }
        break;

    case 9:
    case 0xA:
        g = func_800A02AC(c, b[0x24], b[0x25]);
        if (g == NULL)
        {
            break;
        }
        if ((u32)(g[0x13] - 0x33) >= 4)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        nd = *(u8 **)(g - 0x14);
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], nd[0x24], nd[0x25], (u16 *)&sp20);
        return 5;

    case 0x17:
        func_800ADD20(c, 0x10);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g == D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x1C:
        if ((*(s32 *)(c + 0x1C) & 0x20000) == 0)
        {
            break;
        }
        func_800ADD20(c, 0x20);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                              (u16 *)&sp20) >>
                9) &
               7;
        if ((s16)func_8009A350(b[0x24], b[0x25], sp1E, (u16 *)&sp20) == 0)
        {
            break;
        }
        if ((sp20 & 0xB712) != 0)
        {
            break;
        }
        if ((s16)func_8009FD40(D_80082E80, b) != 2)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = sp1E << 9;
        return 5;

    case 0x1D:
        func_800ADD20(c, 8);
        if ((*(u16 *)(c + 0x98) & 0x100) == 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if ((*(s32 *)(g + 0x14) & 0x4000) == 0)
        {
            break;
        }
        if (g[0x13] == 0)
        {
            break;
        }
        if ((s16)func_80042900((s8 *)g, 4) != 0)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        return 5;

    case 0x15:
        if (c[0xA7] == 0)
        {
            break;
        }
        if (c[0xA8] != 0)
        {
            break;
        }
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        g = func_8009B25C(c, b[0x24] + (u16)D_8006CCD8[sp1E], b[0x25] + (u16)D_8006CCE8[sp1E],
                            *(s16 *)(c + 0x88));
        if (g == NULL)
        {
            break;
        }
        if (g != D_800E3D7C)
        {
            break;
        }
        *(u16 *)(c + 0x46) = 0x8009;
        *(s16 *)(c + 0x2A) = func_800A0818(b[0x24], b[0x25], D_80082E80[0x24], D_80082E80[0x25],
                                           (u16 *)&sp20);
        return 5;

    default:
        break;
    }

    q = func_800A3D18(b, c, 2);
    *(u8 **)(c + 0x60) = q;
    if (q != NULL)
    {
        r = func_800A384C(c, q, (u8 *)&sp1E, 1);
        sp20 = r;
        *(s16 *)(c + 0x2A) = sp1E;
        if ((s16)r < 0)
        {
            *(u16 *)(c + 0x46) = 0x800B;
            return 2;
        }
        *(u16 *)(c + 0x46) = r | 0x8000;
        return 1;
    }
    k2 = c[0x13];
    if (k2 == 0x1E && (*(s32 *)(c + 0x14) & 0x20000000) == 0)
    {
        sp1E = (*(u16 *)(c + 0x2A) >> 9) & 7;
        sp18 = b[0x24] + (u16)D_8006CCD8[sp1E];
        sp1A = b[0x25] + (u16)D_8006CCE8[sp1E];
        g = NULL;
        if (c[0xAC] == 0)
        {
            g = func_8009B25C(c, sp18, sp1A, *(s16 *)(c + 0x88));
            if (g != NULL && ((u32)(g[0x13] - 1) >= 0x2D || g[0x13] == k2))
            {
                g = NULL;
            }
        }
        if (c[0xAC] < 2)
        {
            if (g != NULL)
            {
                *(u16 *)(c + 0x46) = 0x8009;
                return 5;
            }
            g = (u8 *)((s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1);
        }
        if (g != NULL)
        {
            *(u16 *)(c + 0x46) = 0x8009;
            return 5;
        }
    }
    if (*(s8 *)(b + 0x26) < 0)
    {
        return (u16)ret;
    }
    return 2;
}
