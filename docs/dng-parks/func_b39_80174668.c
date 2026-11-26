/* PARK: func_b39_80174668, csrc/ovl_dng_b39.c, agent-h198, 2026-11-25.

   TARGET 302 words.  BEST BUILT 302 words, 66 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py).  1 member, 302 words (dngfam.py).
   VERDICT: ALLOC, and the cause is EXACTLY ONE CALLEE-SAVED REGISTER.
   The target's frame is -0x50 with s0..s7 + ra (nine slots); this body's is
   -0x48 with s0..s6 + ra (eight).  Every row follows from that: the target
   holds p/b/c/d in s6/s7/s5/s3 and this body holds them in s5/s6/s2/s3, so
   the whole assignment is rotated by one position.

   WHICH QUANTITY IS MISSING, read off both listings.  The call-crossing
   values are p, b, c, d, q (= node + 0x20), n, &rect, mm (the
   func_800990FC result held across the print chain), r (= n->unk_0C) and the
   func_800BCB04 result.  The target packs them into EIGHT registers:
   s1 = &rect then n, s0 = the constant 1 then mm then the func_800BCB04
   result, s2 = `s' in case 0 and `r' in case 1.  This body packs them into
   SEVEN by merging `c' with `r' -- they do not conflict, because `c' is used
   only in cases 0 and 2 -- and spilling `s' to $v1.  So the question is what
   makes `c' and `r' conflict in the target, i.e. what keeps `c' live across
   case 1.  Nothing in case 1's listing reads c.  UNRESOLVED.

   DONOR.  dngcallee.py named csrc/ovl_dng_b26.c's func_b26_80173614
   (106 words, landed) at 13 of 25 symbols with a clean gap, and it is the
   right donor: its case 0 is this body's print chain and its case 1 is this
   body's case 2.  ALL FOUR of b26's recorded facts HOLD here and were
   re-measured as part of reaching 302 words:
     * `h = func_800990FC(); mm = h; h = func_80099734(d, h);' -- the alias
       split, not `mm = func_800990FC(); h = func_80099734(d, mm);'
     * the facing table materialized into `e' AFTER func_800A05A4 returns
     * `p[0x9B] = 1' BEFORE the three b stores
     * `*(u8 **)(d + 0x60)' RE-READ at the -0x14 deref, not hoisted into a
       local (S1 below: 301 words, 69 rows, and still only seven saves)
   b26's `== 0'-as-the-then rule for the d + 0x6D arms also holds.

   RESOLVED NAMES.  func_80069EF8 is rand and func_800673A0 is MoveImage
   (splat/common_syms.txt).  func_8004491C's second argument here is a
   FUNCTION pointer, `(u32)func_b39_80174B98', not the usual
   `(u32)D_80045340' -- that is case 0's 0x201 node; case 1's 0x312 node uses
   D_80045340.  The 0xAC769185 magic with `sra 6' and the
   `(x*3)<<5 - x' multiply-back is `rand() % 95'.
   The 0x8 bytes of D_b39_8017086C already in the unit are a RECT
   { 0x340, 0x100, 0x40, 0x80 } and the body's first statement is
   `rect = *(RECT *)D_b39_8017086C' -- the `const u8 []' declaration is what
   makes the copy alignment-1 and emits the target's lwl/lwr + swl/swr pair.

   MEASURED SPELLINGS -- words / aligned rows.

     | spelling                                                | words | rows |
     |---------------------------------------------------------|-------|------|
     | THE BODY BELOW                                          |  302  |  66  |
     | `*(s8 *)(d + 0x72) << 6' at both 0x72/0x73 sites         |  304  |  72  |
     | ... `(s32)*(s8 *)(d + 0x72) << 6'                        |  304  |  72  |
     | ... through an `s32' comma-local                         |  301  |  80  |
     | ... `rand() % 0x40 + (x << 6)' (addend order swapped)    |  304  |  90  |
     | ... `*(s8 *)(d + 0x72) * 0x40' (ADOPTED)                 |  302  |  66  |
     | `RECT *rp = &rect' at the top, call through rp           |  302  |  66  |
     | ... rp assigned before the struct copy                   |  302  |  66  |
     | ... `s32 *rp = (s32 *)&rect'                             |  302  |  66  |
     | ... and the w.vx/w.vy reads through rp as well           |  302  |  76  |
     | `g0 = *(u8 **)(d + 0x60)' local for the test and deref   |  301  |  69  |
     | the `s' read moved above func_800A56E0 (so it spans it)  |  303  |  70  |
     | `mv = n->unk_08' local instead of four re-reads          |  302  |  72  |

   THE `* 0x40' LEVER IS THE ONE GENERAL RESULT AND IT IS WORTH A ROW IN
   docs/matching-levers.md.  `*(s8 *)x << 6' compiles to THREE words --
   `lbu' then `sll 24' then `sra 18' -- because the sign extension is done by
   a shift pair that combine then merges with the user's shift.  The same
   value as `*(s8 *)x * 0x40' compiles to TWO -- `lb' then `sll 6' -- because
   expand_mult works on the already-extended value.  Two words and four rows
   on this body, at two sites.  A byte field multiplied by a power of two is
   therefore NOT interchangeable with the shift spelling.

   UNMEASURED, and where to look next:
     * anything that makes `c' live across case 1, which is the whole
       residue.  Nothing was tried: the three locals above all leave seven
       saves, so the extra quantity is not one of `g0', `mv' or a `&rect'
       pointer.  Worth trying: case 1 written as a nested `if' chain rather
       than a switch arm; the three cases in a different source order; and an
       eighth call-crossing value invented in case 2.
     * the dress-store schedule at the end of case 0.  This body emits the
       +0xAC and the node's +0x96 stores between the +0xA2 and +0xA4 stores
       and pays a `nop' after `lhu 0xA(b)'; the target emits +0xA2, +0xA4,
       then +0xAC/+0x96/+0xA6.  Only the source order written below was
       tried.
     * `w.vx = rect.x + (rect.w >> 1)' emits `lhu' + `sll 16' + `sra 17',
       which MATCHES the target, so RECT's `short' fields are right here --
       but the same shape at func_b39_8017382C's +0xA2 store needed a `u16'
       read plus an `(s16)' cast.  The two are not the same question and the
       difference is unexplained.

   DECLARATIONS
   void *func_800990FC(void);
   void *func_80099194(u8 *, u8 *);
   u8 *func_80099290(u8 *);
   void *func_80099734(u8 *, u8 *);
   s32 func_800A5720(void *);
   s32 func_800C8CD8(u8 *, s16, s16);
   void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
   extern u8 D_800DEC50[];
   extern u8 D_80045340[];
   s32 rand(void);
   UnkNode *func_8003FD64(s32, void *);
   extern u8 D_80083498[];
   void func_b39_80174B98(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_b39_80174B20(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_b39_80175174(u8 *p, UnkVec *b, u8 *c, u8 *d);
   END DECLARATIONS
*/
void func_b39_80174668(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    RECT rect;
    DVECTOR w;
    UnkNode *n;
    UnkSlot *r;
    u8 *e;
    u8 *g;
    u8 *q;
    u8 *s;
    void *mm;
    void *h;

    rect = *(RECT *)D_b39_8017086C;
    switch (p[0x9B])
    {
    case 0:
        *(s32 *)(d + 0x60) = func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
        e = D_80175258;
        *(u8 **)(c + 0x2C) = e;
        func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            p[0x9B] = 2;
            return;
        }
        p[0x9B] = 1;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        if (*(u8 **)(d + 0x60) == NULL)
        {
            return;
        }
        h = func_800990FC();
        mm = h;
        h = func_80099734(d, h);
        h = func_80099194((u8 *)D_b39_80170874, h);
        func_80099290(h);
        func_800A5720(mm);
        func_800A56E0(0x60A);
        s = *(u8 **)(*(u8 **)(d + 0x60) - 0x14);
        d[0x72] = s[0x24];
        d[0x73] = s[0x25];
        w.vx = rect.x + (rect.w >> 1);
        w.vy = rect.y + (rect.h >> 1);
        func_800B8FC8(p, (s32 *)&rect, (s32 *)&w, 1, 0);
        n = func_8003FD64(0x201, D_80083498);
        if (n == NULL)
        {
            return;
        }
        func_8004491C(n, (u32)func_b39_80174B98);
        q = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_b39_80174B20;
        *(s16 *)(q + 0xA2) = (c[0x24] << 6) + 0x20;
        *(s16 *)(q + 0xA4) = (c[0x25] << 6) + 0x20;
        *(u32 *)(q + 0xAC) = 0x808080;
        *(s16 *)(q + 0x96) = 0x3C;
        *(s16 *)(q + 0xA6) = *(u16 *)((u8 *)b + 0xA);
        *(s16 *)(p + 0x96) = 0x3C;
        return;
    case 1:
        g = *(u8 **)(d + 0x60);
        if (g != NULL && *(s16 *)(p + 0x96) < 0x34)
        {
            *(s16 *)(g + 0x2A) = (*(u16 *)(g + 0x2A) + 0x200) & 0x3FFF;
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n + 0x20;
                r = (UnkSlot *)n->unk_0C;
                n->unk_10 = (u32)func_b39_80175174;
                *(s16 *)((u8 *)n->unk_08 + 0x2) = (*(s8 *)(d + 0x72) * 0x40) + rand() % 0x40;
                *(s16 *)((u8 *)n->unk_08 + 0x6) = (*(s8 *)(d + 0x73) * 0x40) + rand() % 0x40;
                *(s16 *)((u8 *)n->unk_08 + 0xA) =
                    func_800BCB04h(*(u16 *)((u8 *)n->unk_08 + 0x2),
                                   *(u16 *)((u8 *)n->unk_08 + 0x6), -0x400) -
                    rand() % 95 - 0x20;
                *(s16 *)((u8 *)r + 0x1E) = 0xC04;
                *(s16 *)((u8 *)r + 0x1C) = 0xC04;
                *(s16 *)((u8 *)r + 0x10) = 0x60;
                *(u8 **)r = D_800DEC50;
                *(u16 *)((u8 *)r + 0x14) |= 0xC;
                *((u8 *)r + 0x4) = 0;
                *((u8 *)r + 0x5) = 0;
                *(u32 *)((u8 *)r + 0xC) = 0x208020;
                *(u32 *)((u8 *)r + 0x8) = *(u32 *)(D_800DEC50 + 0x4);
                q[0x9A] = 0;
            }
        }
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            p[0x9B] += 1;
        }
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        d[0x73] = 0;
        d[0x72] = 0;
        func_800AD594(d, 0x800);
        *(s32 *)(p + 0x8C) = (s32)func_b39_80170F6C;
        D_8008346C = 0;
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) == 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        else
        {
            D_800E3DE8 = d - 0x20;
        }
        if (*(u8 **)(d + 0x60) != NULL)
        {
            func_800C8CD8(*(u8 **)(d + 0x60), 0x28, 0x10);
        }
        return;
    }
}
