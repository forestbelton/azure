/* ===========================================================================
   DECODE (not a measured candidate) for func_800ADE74, ovl_dungeon.
   1817 words (header `nonmatching func_800ADE74, 0x1C64`), plus
   jtbl_80089088 (36 entries) at 0x80089088.  2026-11-15, agent j34.

   NOTHING HERE HAS BEEN COMPILED.  The body below is a reading of the
   listing, written out as C so the next attempt starts from it instead of
   re-deriving it.  Treat every line as a claim to check, not as a candidate.
   (This file was lost with /tmp on 2026-11-16 and recovered from the agent's
   transcript -- see docs/dungeon-sweep.md, "2026-11-16: /tmp IS NOT
   DURABLE".)

   NOT one of the seven forbidden functions: 0x80089088 is 0 mod 8, so the
   `.align 3` cc1psx writes before a compiled jump table costs nothing here.
   (docs/dungeon-sweep.md "SEVEN of the 28 jump tables are at 4 mod 8".)

   ---------------------------------------------------------------- SIGNATURE
   SIX parameters, and the FIRST ONE IS NEVER READ:

       s32 func_800ADE74(u8 *unused, u8 *b, u8 *c, u16 d, u16 e, u8 *f)

     $a0  never read before it is clobbered (first use is `addu $a0,$s5` in
          func_800A9A0C's delay slot), so parameter 0 is dead in the body.
     $a1 -> $s7   = b   the actor/view record (b[0x24], b[0x25] tile coords,
                        b[0x26] a signed room index)
     $a2 -> $s5   = c   the record this function drives: +0x12 kind, +0x13
                        the switch selector, +0x1C flags, +0x24/+0x25 a
                        halfword/byte pair, +0x2A facing, +0x46 a status
                        halfword, +0x60 an owner pointer, +0x66, +0x88,
                        +0x98 flags, +0xA6..+0xAE, +0xB5
     $a3 -> $s1   = d   read ONLY as `(s16)d` for the func_800A19E4 call
     0x60($sp) -> $s0 = e   read with `lhu`, so the parameter is u16, then
                        `(s16)e` for the same call
     0x64($sp)      = f   read with `lw` and handed to func_800A19E4's
                        fifth argument, whose declared type is `u8 *`

   The frame is 0x50: s0-s7/fp/ra at 0x28..0x4C, the outgoing argument area
   0x00..0x17 (two calls take five arguments), and five halfword locals at
   0x18, 0x1A, 0x1C, 0x1E, 0x20.  $fp holds the `u16 ret` below, 2 from the
   prologue; the function returns `ret` zero-extended (sll 16/srl 16) on the
   two fallthrough paths and a literal on every other path, so the return
   type is signed (one path returns -1) and `ret` is a u16 LOCAL.

   ----------------------------------------------------------------- DISPATCH
       v0 = c[0x13]; v1 = v0 - 9;
       if ((u32)v1 >= 0x24) -> default;  jr jtbl_80089088[v1];

   so `switch (c[0x13])` over the range 9..0x2C.  36 entries: cases 0x1E,
   0x1F and 0x21 point at the default label, and so does the whole 11..20
   run, so THOSE EMPTY CASES MUST BE WRITTEN OUT or the table comes out
   short.

   case value -> arm label, and the arms appear in the listing in this order,
   which is the SOURCE order a future attempt has to reproduce (switch arms
   are emitted in source order):

        0x19 .L800AE900     0x16 .L800AE9A0     0x2C .L800AEA3C
        0x2B .L800AEB54     0x22 .L800AEBC0     0x29 .L800AEC50
        0x28 .L800AED18     0x1A .L800AED4C     0x23 .L800AEE10
        0x26 .L800AEE70     0x27 .L800AEF20     0x20 .L800AEFEC
        0x24 .L800AF130     0x25 .L800AF210     0x18 .L800AF238
        0x2A .L800AF304     0x1B .L800AF4AC   9,0xA .L800AF57C
        0x17 .L800AF5C0     0x1C .L800AF66C     0x1D .L800AF738
        0x15 .L800AF7D8     default .L800AF89C

   ------------------------------------------------------- THE SHARED TAILS
   Every arm ends by falling into one of six shared tails.  They are a real
   source structure (one `goto`-ladder, exactly like func_8008B9FC's), not
   only cross-jumping: .L800AF89C/.L800AF8A0/.L800AF8A4 are three entries to
   ONE call, differing in how many of its arguments the arm has already set.

     t1 = .L800AF89C   a0 = b;  fall into t2
     t2 = .L800AF8A0   a1 = c;  fall into t3
     t3 = .L800AF8A4   q = func_800A3D18(b, c, 2);
                       *(u8 **)(c + 0x60) = q;
                       if (q == NULL) goto t6;                 (.L800AF900)
                       r = func_800A384C(c, q, &sp1E, 1);
                       sp20 = r;  *(s16 *)(c + 0x2A) = sp1E;
                       if ((s16)r < 0) goto t5;
                       *(u16 *)(c + 0x46) = r | 0x8000;  return 1;
     t4 = .L800AF8E4   *(u16 *)(c + 0x46) = v0;          return 1;
     t5 = .L800AF8F0   v0 = 0x800B; fall into
          .L800AF8F4   *(u16 *)(c + 0x46) = v0;          return 2;
     t6 = .L800AF900   the "no slot" fallback, decoded below
     t7 = .L800AFA88   *(s16 *)(c + 0x2A) =
                           func_800A0818(b[0x24], b[0x25], ?, ?, &sp20);
                       return 5;
     ret-paths         .L800AFA9C `return ret;` (sll 16 then the shared srl
                       at .L800AFAA0, which two `(u16)` returns jump into),
                       .L800AFAA4 the epilogue with $v0 already set.

   t6 in full (.L800AF900):
        k = c[0x13];
        if (k == 0x1E && (*(s32 *)(c + 0x14) & 0x20000000) == 0)
        {
            u = (*(u16 *)(c + 0x2A) >> 9) & 7;   sp1E = u;
            sp18 = b[0x24] + D_8006CCD8[u];
            sp1A = b[0x25] + D_8006CCE8[u];
            g = NULL;
            if (c[0xAC] == 0)
            {
                g = func_8009B25C(c, sp18, sp1A, *(s16 *)(c + 0x88));
                if (g != NULL &&
                    ((u32)(g[0x13] - 1) >= 0x2D || g[0x13] == k))
                    g = NULL;    /* out of the 1..0x2D range, or same kind */
            }
            if (c[0xAC] < 2)
            {
                if (g != NULL) { v0 = 0x8009; goto set46_ret5; }
                /* $s0 is reused as an int here: the +1 only makes the
                   truthiness test below mean "func_800A70E4 != -1" */
                gi = (s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88)) + 1;
            }
            if (gi != 0) { *(u16 *)(c + 0x46) = 0x8009; return 5; }
        }
        if (*(s8 *)(b + 0x26) < 0) return ret;
        return 2;

   ------------------------------------------------------------------ CAVEATS
   * The halfword locals are shared between unrelated uses: 0x18/0x1A are
     func_800A6E8C's two outputs AND a scratch coordinate pair in cases 0x24
     and 0x2A; 0x1C is func_800A3544's result; 0x1E is a facing index AND
     func_800A384C's output; 0x20 is func_800A384C/func_800A40AC's result and
     func_8009A350's buffer.  Which of those are one local and which are
     several is NOT witnessed by the listing.
   * `*(u16 *)(c + 0x24) > 0x10000` in the head block is DEAD as written
     (the value is an `lhu`), and the listing keeps it: `lui $v0, 1` /
     `slt $v0,$v0,$v1`.  It must be written out or the block is short.
   * The `.L800AF738` arm (case 0x1D) ENDS with `j .L800AEEFC`, i.e. into the
     middle of case 0x26's arm.  That is gcc cross-jumping two identical
     tails, not a source goto: the shared text is
     `if (X) { if ((s16)func_80042900(g,4) != 0) goto t2; v0=0x8009; goto A; }`
     with X = `(s16)func_800A2CB8(c,g)` in 0x26 and `g[0x13]` in 0x1D.
   * The nine-arm facing preamble
         u = (*(u16 *)(c + 0x2A) >> 9) & 7;  sp1E = u;
         g = func_8009B25C(c, (u16)(b[0x24] + D_8006CCD8[u]),
                              (u16)(b[0x25] + D_8006CCE8[u]),
                              *(s16 *)(c + 0x88));
     is byte-identical in cases 0x19, 0x1A, 0x26, 0x27, 0x20, 0x17, 0x1D,
     0x15 and in t6 -- nine copies, NOT cross-jumped, so it is written out
     nine times in the source.  The `andi $a1,$a1,0xFFFF` /
     `andi $a2,$a2,0xFFFF` pair says both coordinate arguments are (u16).
   =========================================================================== */

/* Declarations this body needs that the unit does not already carry above
   line 1893 (every one of these is DEFINED later in csrc/ovl_dungeon.c, so
   they are forward declarations, and the signatures are copied from those
   definitions):

      s32  func_800A2CAC(u8 *);
      s32  func_800A6E8C(u8 *, s32, s16 *, s16 *);
      s16  func_800A70E4(s16, s16, s16);
      s32  func_800A0818(s16, s16, s16, s16, u16 *);
      s16  func_800A3544(u8 *, u8);
      s16  func_8009FD40(u8 *, u8 *);
      u8   func_800A35A4(u8 *, s32);
      s32  func_800A3518(u8 *);
      u8  *func_800A3D18(u8 *, u8 *, s16);
      s32  func_800A384C(u8 *, u8 *, u8 *, s32);
      s32  func_800A404C(void);
      s32  func_8009CD58(u8 *, s32, s16);
      s16  func_800A40AC(u8 *, u16);
      u8  *func_800A03C4(u8 *, u16, u16);
      s16  func_800A0134(u8 *, u8 *);
      s32  func_800A36B4(u8 *, u8 *);
      s32  func_800A365C(u8 *, u8 *);
      s32  func_800C7F68(u8 *);
      void func_800A0E6C(u8 *, s8, u8 *, u8 *);
      s16  func_800A45D8(u16, u16, s32);
      u8  *func_800A02AC(u8 *, u16, u16);
      void func_800ADD20(u8 *, s32);
      s32  func_800A2C34(u8 *);
      s32  func_80042900(s8 *, s8);
      s32  func_800BCB04(s32, s32, s16);
      u8  *func_8009B25C(u8 *, u16, u16, s16);
   and the globals, all already declared in the unit:
      D_80013714 D_8006CCD8 D_8006CCE8 D_800814A8 D_80082E80 D_8008347E
      D_800E296C D_800E2970 D_800E3548 D_800E3D7C
*/

s32 func_800ADE74(u8 *unused, u8 *b, u8 *c, u16 d, u16 e, u8 *f)
{
    u16 ret;
    s16 sp18, sp1A, sp1C;
    u16 sp1E, sp20;
    u8 *g;
    u8 *nd;
    u8 *q;
    u8 k;
    s16 r;
    s32 i, j, u, gi;

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
        return ret;                                     /* .L800AFA9C */
    }
    if ((s16)func_800A2CAC(c) != 0 &&
        (s16)func_800A6E8C(b, 0x60C, &sp18, &sp1A) != 0 && c[0x13] != 0x22)
    {
        if ((s16)func_800A2C34(c) != 0)
        {
            return -1;
        }
        if (sp18 == b[0x24] && sp1A == b[0x25])
        {
            /* same tile: top up the carried count and play the sound */
            r = (s16)func_800A70E4(sp18, sp1A, *(s16 *)(c + 0x88));
            if (r >= 0)
            {
                u16 t = *(u16 *)(c + 0x24);
                if (t > 0x10000)            /* dead as written; keep it */
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
        *(s16 *)(c + 0x2A) =
            func_800A0818(b[0x24], b[0x25], sp18, sp1A, (u16 *)&sp1C);
        *(u16 *)(c + 0x46) = 0x800B;
        return 2;
    }
    func_800A19E4((s32)b, c, (s16)d, (s16)e, f);        /* .L800AE044 */

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
        /* a four-case compare tree, bodies in source order 0,1,2,4,default */
        switch (k)
        {
        case 0:                                          /* .L800AE108 */
            if ((s16)func_800A2C34(c) != 0) return -1;
            if ((s16)func_80042900((s8 *)c, 6) == 0)
            {
                sp1C = func_800A3544(c, 0x1E);
                if (sp1C >= 0)
                {
                    if ((*(s32 *)(c + 0x1C) & 0x20000) != 0 &&
                        func_8009FD40(b, D_80082E80) <
                            (s16)func_800A35A4(c, sp1C) &&
                        (s16)func_800A3518(D_800E3D7C) != 0)
                    {
                        *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
                        *(s16 *)(c + 0x2A) =
                            func_800A0818(b[0x24], b[0x25], D_80082E80[0x24],
                                          D_80082E80[0x25], (u16 *)&sp1C);
                        *(u32 *)(c + 0x60) = D_800814A8;
                        return 4;
                    }
                    /* two tries over D_800814A8's +0xAC slot pair,
                       starting with the parity in D_8008347E bit 0 */
                    i = 0;
                    j = D_8008347E & 1;
                    do
                    {
                        q = *(u8 **)((u8 *)D_800814A8 + j * 4 + 0xAC);
                        if (q != NULL)
                        {
                            nd = *(u8 **)(q - 0x14);
                            if (func_8009FD40(b, nd) <
                                    (s16)func_800A35A4(c, sp1C) &&
                                (s16)func_800A3518(
                                    *(u8 **)((u8 *)D_800814A8 + j * 4 + 0xAC))
                                    != 0)
                            {
                                goto claim_slot;         /* .L800AE638 */
                            }
                        }
                        i++;
                        j ^= 1;
                    } while (i < 2);
                }
            }
            /* .L800AE290 / .L800AE294 */
            q = func_800A3D18(b, c, 4);
            *(u8 **)(c + 0x60) = q;
            if (q == NULL) goto no_claim;                /* .L800AE7F4 */
            r = func_800A384C(c, q, (u8 *)&sp1E, 0);
            sp20 = r;
            *(s16 *)(c + 0x2A) = sp1E;
            if (r < 0) goto fail_800B;                   /* .L800AF8F0 */
            *(u16 *)(c + 0x46) = r | 0x8000;
            return 1;

        case 1:                                          /* .L800AE2E0 */
            /* func_80042900 key 6 / func_800A3544 key 0x1E, then the SAME
               two-slot walk as case 0, differing only in that its first
               probe reads D_800814A8 directly (`lui $s0,%hi; lw %lo($s0)`)
               and in its tail (.L800AE460/.L800AE464):
                   q = func_800A3D18(b, c, -3);     <-- -3, not 4
                   *(u8 **)(c + 0x60) = q;
                   if (q == NULL) goto no_claim;
                   r = func_800A384C(c, q, &sp1E, 0);
                   sp20 = r;  *(s16 *)(c + 0x2A) = sp1E;
                   if ((s16)r < 4) goto fail_800B;  <-- < 4, not < 0
                   *(u16 *)(c + 0x46) = r | 0x8000;  return 1;             */
            break;

        case 2:                                          /* .L800AE4B8 */
            /* like case 1, but after the walk it also tries the "throw"
               path at .L800AE6A8: (c+0x1C & 0x20000) && !func_800A404C()
               && D_800814A8->unk_9A == 0x11 && its +0x60 owner's
               +0x14 lacks 0x2000, then func_8009CD58(owner, 7, 0),
               func_800A40AC(c, (u16)that) and the t7 tail with
               D_80082E80's coords; returns 3.  Its own refusal path
               .L800AE76C is a SECOND `if ((s16)func_800A2C34(c) != 0)
               return -1;` before it falls into no_claim, which calls
               func_800A2C34 a third time -- nine calls in the function. */
            break;

        case 4:                                          /* .L800AE788 */
            if ((s16)func_800A2C34(c) != 0) return -1;
            ret = 0;
            q = func_800A03C4(c, b[0x24], b[0x25]);
            *(u8 **)(c + 0x60) = q;
            if (q != NULL)
            {
                r = func_800A384C(c, q, (u8 *)&sp1E, 0);
                sp20 = r;
                if (r >= 0)
                {
                    *(u16 *)(c + 0x46) = r | 0x8000;
                    *(s16 *)(c + 0x2A) = sp1E;
                    return 1;
                }
            }
            /* falls into the default tail */
            break;

        default:
            break;
        }
    no_claim:                                            /* .L800AE7F4 */
        if ((s16)func_800A2C34(c) != 0) return -1;
        return ret;
    }

    /* .L800AE810 -- the main body: the 0x2410/0x2000 flag combination above
       is the "being carried" case; everything below is the normal step. */
    if ((s16)func_800A2C34(c) != 0) return -1;
    if ((*(s32 *)(c + 0x1C) & 0x10) != 0) return ret;
    if (*(s8 *)(b + 0x26) >= 0 &&
        (D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2) != 0)
    {
        return ret;
    }
    /* .L800AE870: one more "already holding something" shortcut */
    if ((s16)func_80042900((s8 *)c, 6) == 0)
    {
        sp1C = func_800A3544(c, 0x1E);
        if (sp1C >= 0 && (s16)func_800A3518(c) != 0)
        {
            *(u32 *)(c + 0x60) = (u32)c;
            *(u16 *)(c + 0x46) = (sp1C + 1) | 0x8000;
            return 4;
        }
    }

    /* .L800AE8CC: switch (c[0x13]) over 9..0x2C, 36 jump-table entries.
       Arms in SOURCE order -- see the header.  Each arm's own shape:

       case 0x19 (.L800AE900): func_800ADD20(c, 0x10); needs c+0x98 bit 0x100;
            the nine-arm facing preamble; refuses when the probed record's
            +0x13 is non-zero; func_800A3518(c); c+0x46 = 0x800A; returns 6.
       case 0x16 (.L800AE9A0): c[0x48] must be 0xF and c+0x1C must carry
            0x20000; func_8009FD40(D_80082E80, b) < 0xB; func_800A365C(b,
            D_80082E80); func_800A36B4(c, D_800E3D7C); c+0x46 = 0x8008; the
            t7 tail with D_80082E80's coords; returns 1.
       case 0x2C (.L800AEA3C): func_800ADD20(c, 4); c+0x98 0x100 and c+0x1C
            0x20000; r = func_8009FD40(D_80082E80, b) must satisfy
            (u16)(r - 2) < 7; func_800A365C; |func_800A0134(c,D_800E3D7C)|
            < 0x21; g = func_800A05A4(c, b[0x24], b[0x25],
            *(s16*)(c+0x2A), (s16)r); func_800A2CB8(c, g);
            func_800A36B4(c, g); c[0xAE] must be 0; c+0x46 = 0x8009; t7.
       case 0x2B (.L800AEB54): func_800ADD20(c, 0x20); c+0x98 0x100,
            c+0x1C 0x20000, c+0x98 must LACK 0x8000; c+0x46 = 0x8009; t7
            with D_80082E80's coords.
       case 0x22 (.L800AEBC0): func_800ADD20(c, 0x20); c+0x98 0x100,
            c+0x1C 0x20000, b[0x26] >= 0; then for i = 0,1 over
            D_800E3D7C[+0xAC + 4i]: if func_800A2CAC(that) != 0 ->
            c+0x46 = 0x8009 and the t7 tail with D_80082E80's coords;
            otherwise t3.
       case 0x29 (.L800AEC50): func_800ADD20(c, 0x10); c+0x98 0x100; for
            i = 0,1 over D_800E3D7C[+0xAC + 4i]: |func_800A0134(c,that)|
            < 0x21, func_800A41F0(that), func_8009FD40(*(u8**)(that-0x14),
            b) == 1, func_800C7F68(that) == 0 -> c+0x46 = 0x8009,
            c+0xA8 = that, t7 with that record's coords; else t2.
       case 0x28 (.L800AED18): func_800ADD20(c, 0x40); then c+0x98 gets
            0x8000 set when it carried 0x100 and cleared when it did not;
            t1 either way.
       case 0x1A (.L800AED4C): func_800ADD20(c, 2); c+0x98 0x100, and
            c+0x98 loses 0x8000; facing preamble; func_800A2CB8(c, g);
            func_80042900(g, 1); c+0x98 |= 0x8000; t2.
       case 0x23 (.L800AEE10): func_800ADD20(c, 4); c+0x98 0x100,
            c+0x1C 0x20000, *(s16*)(c+0xA6) == 0, D_800E296C lacks 4;
            c+0x46 = 0x8009; returns 5.
       case 0x26 (.L800AEE70): func_800ADD20(c, 2); c+0x98 0x100; facing
            preamble; func_800A2CB8(c, g); (shared tail .L800AEEFC)
            func_80042900(g, 4); c+0x46 = 0x8009; returns 5.
       case 0x27 (.L800AEF20): func_800ADD20(c, 0x10); c+0x98 0x100 and
            loses 0x8000; facing preamble; func_800A2CB8; func_80042900(g,2);
            c+0x46 = 0x8009 and c+0x98 |= 0x8000; returns 5.
       case 0x20 (.L800AEFEC): if c[0x49] == 0: func_800ADD20(c,2),
            c+0x98 0x100, facing preamble, the probed record must BE
            D_800E3D7C, c+0x46 = 0x8009, returns 5.  Else (.L800AF088)
            c+0x1C must lack 0x410; func_800AA53C(c); if c+0x1C lacks
            0x20000 return 2; if b[0x26] >= 0 then
            func_800A0E6C(b, *(s8*)(c+0x9C), c, c+0x98) and c+0x46 = 0x800B,
            return 2; else func_8009FD40(b, D_80082E80) == 1 and the t7-shaped
            func_800A0818 into c+0x98 with +0x800 added to c+0x2A.
       case 0x24 (.L800AF130): c+0x98 loses 0x8000; c[0x49] must be 0;
            func_800A6E8C(b, 0x12, &sp18, &sp1A); func_800A0818 into c+0x98;
            c+0xA8/c+0xAA get the two coordinates, c+0x98 |= 0x8000; then
            the Chebyshev distance max(|b[0x24]-x|, |b[0x25]-y|) must be 1,
            else c+0x46 = 0x800B and return 2.
       case 0x25 (.L800AF210): func_800ADD20(c, 0x20); c+0x98 0x100;
            c+0x46 = 0x8009; returns 5.
       case 0x18 (.L800AF238): func_8009FD40(b, D_80082E80) >= 2 (else
            .L800AF294) and c[0xB5] == 0 -> func_800ADD20(c, 8), c+0x98
            0x100, c+0x46 = 0x800A, return 6.  Otherwise c[0xB5] must be
            non-zero and then func_8009B25C(c, b[0x24], b[0x25],
            *(s16*)(c+0x88)) must find something, c+0x1C must carry
            0x20000, and func_800A0818 writes c+0x98 before t5.
       case 0x2A (.L800AF304): func_800ADD20(c, 4); c+0x98 0x100 and
            c+0x1C 0x20000; func_800A365C(b, D_80082E80);
            r = func_8009FD40(D_80082E80, b) with (u16)(r-2) < 7;
            func_800A0818 -> sp1E = (that >> 9) & 7; then a do-while r times
            stepping one tile per iteration with func_800A45D8 and
            func_800BCB04, requiring the func_800BCB04 result to stay within
            0x20 of the running angle; on completion (i == r) it falls into
            .L800AF720 (c+0x46 = 0x8009, c+0x2A = sp1E << 9, return 5).
       case 0x1B (.L800AF4AC): func_800ADD20(c, 4); c+0x98 0x100; for
            i = 0,1 over D_800E3D7C[+0xAC + 4i]: |func_800A0134| < 0x21,
            func_800A41F0, func_80042900(that, 0xC) == 0,
            func_8009FD40(*(u8**)(that-0x14), b) == 1 -> c+0x46 = 0x8009
            and t7 with that record's coords.
       cases 9 and 0xA (.L800AF57C): g = func_800A02AC(c, b[0x24], b[0x25]);
            g must exist and (u32)(g[0x13] - 0x33) < 4; c+0x46 = 0x8009;
            t7 with *(u8 **)(g - 0x14)'s coords.
       case 0x17 (.L800AF5C0): func_800ADD20(c, 0x10); c+0x98 0x100; facing
            preamble; the probed record's +0x14 must carry 0x4000 and it must
            NOT be D_800E3D7C; c+0x46 = 0x8009; returns 5.
       case 0x1C (.L800AF66C): c+0x1C 0x20000; func_800ADD20(c, 0x20);
            c+0x98 0x100; func_800A0818 -> sp1E = (that >> 9) & 7;
            func_8009A350(b[0x24], b[0x25], sp1E, &sp20); sp20 must lack
            0xB712; func_8009FD40(D_80082E80, b) == 2; falls into .L800AF720.
       case 0x1D (.L800AF738): func_800ADD20(c, 8); c+0x98 0x100; facing
            preamble; the probed record's +0x14 must carry 0x4000; then it
            CROSS-JUMPS into case 0x26's tail at .L800AEEFC with
            X = g[0x13].
       case 0x15 (.L800AF7D8): c[0xA7] must be non-zero and c[0xA8] zero;
            facing preamble; the probed record must BE D_800E3D7C;
            c+0x46 = 0x8009; t7 with D_80082E80's coords.
       default: t1.
    */
    return ret;
}
