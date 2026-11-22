/* LANDED: func_800AFBFC, 238 of 238 words, 0 differing (masked).  Verified with
   tools/decomp/ovlverify.py three times serially against a real copy of
   csrc/ovl_dungeon_800A4B88.c, and with tools/decomp/aligndiff.py on an
   onefunc.py standalone (0 aligned differences, four serial re-measurements,
   the baseline canary reading 2 rows each time).  clang-format leaves the body
   byte-identical.

   ====================================== 2026-11-22: 2 rows -> 0

   THE WHOLE RESIDUE WAS THAT THE TWO LOOP ACCUMULATORS ARE 16 BITS WIDE.
   The park residue was

     800AFE54  lhu   s0,24(sp)        vs  lhu   $t0, 0x18($sp)
     800AFE5C  addiu s1,s0,-128       vs  addiu $s1, $t0, -0x80

   and the park read it as an allocation question, about which register a
   short-lived temp gets.  It is not.  In the target there is NO temp at that
   point at all.  `lhu $t0 is a RELOAD of the spilled HImode local `m, and
   `addiu $s1, $t0, -0x80 is the whole statement, one single insn

     (set (reg:HI k) (plus:HI (reg:HI m) (const_int -128)))

   which can only exist if `k is 16 bits wide.  With `s32 k the C expression
   `m - 0x80 is int arithmetic, so cc1psx has to widen `m first, and
   `(zero_extend:SI (reg:HI m)) needs a destination register of its own --
   which is the park body s `ang, hence `lhu $s0 and then `addiu $s1,$s0.
   With `u16 k, convert.c s convert_to_integer distributes the narrowing
   conversion down into the MINUS_EXPR, the subtraction happens in HImode,
   `m is read directly as an operand, and reload spills $t0 for it exactly as
   it already does at 800AFE14 for the `(s16)m seed.

   PROOF IN THE DUMPS rather than inference -- .greg of the standalone, on the
   PARK body.  The 800AFE14 seed is already in the target s shape:
     insn 714  (set (reg:HI 8 t0) (mem:HI (plus sp 24)))     <- code -1, a reload
     insn 375  (set (reg:SI 16 s0) (ashift (subreg:SI (reg:HI 8 t0)) 16))
   The park s loop top, 64 insns later, is not:
     insn 434  (set (reg/v:SI 16 s0) (zero_extend:SI (mem:HI (plus sp 24))))
     insn 447  (set (reg/v:SI 17 s1) (plus:SI (reg 16 s0) (const_int -128)))
   Two insns, and the target has only the second.  $t0 is never a pseudo s
   hard register here -- global_alloc prints "Spilling reg 8" and uses it for
   reloads -- so no respelling of a temp could ever have produced it, which is
   why all fourteen entries in the park s rejected list measured 2 or worse.

   ---------------------------------------------- the three changes, in order

   Rows by aligndiff on an onefunc.py standalone, each change priced against
   the body that existed when it was measured.

   1) `u16 k and `u16 bs TOGETHER, with the forward loop s second half
      written `(s16)k again in place of the park s `k <<= 16 / `k >>= 16
      pair -- `k <<= 16 on a u16 is identically zero, so that pair cannot
      survive a narrow k.  Result: 238 words, 2 rows, EXACTLY the park s
      residue with nothing else moved.  This is the step the park could not
      see, because on its own it buys nothing.
      Priced apart on the park body:
        s32 k with the `(s16)k form (control)    3 rows
        u16 k alone                              3 rows  (andi s6,s1,0xffff for bs = k)
        s16 k alone                             26 rows
        u16 k and u16 bs together                2 rows  = the park
      Every single-change price is a loss or a draw, so widthsweep s clean
      verdict on this park was honest rather than stale.

   2) The loop top becomes `k = m - 0x80 with no temp and NO DEVICE, in place
      of the park s `do { ang = m } while (0) plus `k = ang - 0x80.  The
      park s last device is gone: the target s `nop at 800AFE58 is the
      load-delay nop after a RELOAD insn, which reload emits long after sched1
      has run, so nothing has to be held back to protect it.

   3) The backward loop s cse-association breaker becomes a write to `m.
      THIS IS WHY THE PARK COULD NOT GET THERE on its own: its item (d)
      records that a dead store to `m does NOT break the association,
      "because the loop top is Z = zero_extend(m), k = Z - 0x80 and k s class
      names the UNNAMED pseudo Z, not m".  That is true for s32 k -- and
      change 1 deletes Z, after which writing `m is exactly the right
      breaker.  The two are a pair and neither works without the other.

   On the u16 k / u16 bs body, all nine combinations of loop-top spelling and
   breaker, 238 words except where noted:
                                        ang = k    m = k .. m = k   no breaker
     k = m - 0x80                         17             0             17
     do { k = m - 0x80 } while (0)        10 at 239      2             10 at 239
     do { k = m, k -= 0x80 } while (0)    10 at 239      2             10 at 239
   So the do-while is now a one-word regression and the park s own breaker is
   a 17-row one.

   ------------------------------ 4) and then the redundant stores disappear

   The park closed with two stores it was explicitly unhappy about -- `bs = k
   before `k += 0x80 and `ang = k before `k -= 0x80, both deleted by flow,
   both pure cse-association breakers.  Once k and bs are narrow the honest
   shape measures the same: UPDATE THE ACCUMULATOR IN PLACE AND COPY IT INTO
   k.  Priced on the landed body, all at 238 words:
     bs = k, bs += 0x80, k = bs     +   m = k, m -= 0x80, k = m      0  <- kept
     bs = k, k = bs + 0x80, bs = k  +   m = k, m -= 0x80, k = m      0
     bs = k, k += 0x80, bs = k      +   m = k, k -= 0x80, m = k      0
     bs = k, k += 0x80, bs = k      +   m = k, k = m - 0x80, m = k   0
     k += 0x80, bs = k              (forward breaker dropped)       19
     bs += 0x80, k = bs             (no `bs = k first)             118 at 244
     m -= 0x80, k = m               (backward breaker dropped)      78 at 240
     m = k - 0x80, k = m                                            17
   The kept spelling is the only one of the zero-row four with no dead store
   in it, and the two loops are then written identically up to sign.

   ------------------------------------------------- what the park had right

   Everything else is the park s, unchanged: `s16 x, y, x2, y2 and the
   store_expr mechanism behind the four spare `addu copies, the separate
   forward accumulator seeded from `ang, `ang <<= 16 / `ang >>= 16 in the
   head, `ang = a[6] and `ang += 0xC00 as two statements, `s32 cx and
   `s32 cy, `u16 m with `(s16)m at the backward seed, the separate `q for
   the call result with `p = q after the test, `u8 *g as a pointer local,
   the `(u16) read of unk_C8, the texture byte read once into `v, and the
   two-two-segment loop shape.

   `ang needs no reference after the forward loop any more: the park s item
   (h) was a consequence of `ang being the loop-top temp, and change 2 ends
   that.

   DECLARATIONS: none.  `func_800AFFB4 is defined below this function in the
   unit and the call rides its implicit `int declaration.  No prototype
   change. */

s32 func_800AFBFC(u8 *a, void *b, u8 *c)
{
    UnkDungeonSP5 *sp;
    u8 *g;
    POLY_GT4 *p;
    POLY_GT4 *q;
    u8 *t;
    s32 cx;
    s32 cy;
    s32 f;
    s32 v;
    s32 ang;
    u16 k;
    u16 bs;
    u16 m;
    s16 x;
    s16 y;
    s16 x2;
    s16 y2;

    g = (u8 *)&D_80083160;
    sp = (UnkDungeonSP5 *)0x1F800000;
    p = *(POLY_GT4 **)(*(u8 **)g + 0x8D0);
    sp->unk_20 = (u32)(*(u8 **)g + 0x8B0);
    t = *(u8 **)(c + 0x8);
    sp->unk_118 = (u32)t;
    cx = *(u16 *)(g + 0xAC);
    cy = *(u16 *)(g + 0xAE);
    v = t[1];
    c[0xF] = v;
    f = *(u16 *)(c + 0x14);
    if (f & 8)
    {
        c[0xF] = (f & 4) ? (v | 2) : (v & 0xFD);
    }
    ang = a[6];
    ang += 0xC00;
    ang -= (*(u16 *)(g + 0xC8) + 0x80) & 0xF00;
    m = ang;
    bs = ang;
    ang <<= 16;
    ang >>= 16;
    x = func_80064584(ang) * 6 - cx;
    y = func_800644B8(ang) * 6 - cy;
    while (1)
    {
        k = bs + 0x80;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
        bs = k;
        bs += 0x80;
        k = bs;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        x = func_80064584((s16)k) * 6 - cx;
        y = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        q = func_800AFFB4(a, c, sp, p, 1);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    x = func_80064584((s16)m) * 6 - cx;
    y = func_800644B8((s16)m) * 6 - cy;
    while (1)
    {
        k = m - 0x80;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        q = func_800AFFB4(a, c, sp, p, 1);
        if (q == NULL)
        {
            break;
        }
        p = q;
        m = k;
        m -= 0x80;
        k = m;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        x = func_80064584((s16)k) * 6 - cx;
        y = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)p;
    return 0;
}
