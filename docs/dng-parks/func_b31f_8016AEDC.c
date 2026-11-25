/* PARK: func_b31f_8016AEDC (ovl_dng_b31f) -- 131 words / 131 words /
   49 ALIGNED DIFFERENCES.  Agent h173, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   WHAT IT DOES: this bank's PRIMARY actor spawner.  It is
   func_b31f_8016F160 (landed, 96 words, in this same file -- READ THAT
   COMMENT FIRST) with four deltas: it publishes into D_b31f_80174704 rather
   than D_b31f_80174CD8, installs func_b31f_8016B0E8 as the driver and
   func_b31f_8016B778 as the +0x8C continuation, dresses from
   D_b31f_8017467C, puts 0x31 in the payload's +0x13, and its tail stores
   +0xAE/+0xB4/+0xB0 and the +0x48..+0x4B quad with a
   func_800A6DA4(0x10, 0x18) result in the middle.  THEN, on every path, it
   calls func_b31f_8016F160 itself for the second actor, cues it through
   func_800A152C and func_80042640, sets that payload's +0x43 to 0xFF and
   +0x12 to 0, and returns its OWN payload.

   THE WORD COUNT IS EXACT AND NOTHING IS MISPLACED.  All 49 rows are a
   UNIFORM ROTATION of the nine callee-saved registers, with every
   instruction in position and every displacement right:

     target  s0=n  s1=p  s2=d then r  s3=m  s4=a  s5=w  s6=a-alias  s7=c  fp=b
     mine    s1=n  s2=p  s0=d then w  s4=m  s5=a  s3=r  s6=a-alias  s7=c  fp=b

   So `d' (which reload SPILLS to 0x10($sp) with `sh' and reloads with `lhu')
   shares its register with `r' in the target and with `w' in the candidate,
   and everything below $s6 slides by one.

   TWO FACTS THIS BODY ESTABLISHES, both kept below:
   1. THERE IS AN ALIAS LOCAL FOR THE FIRST PARAMETER AND IT IS WORTH TWO
      WORDS.  The target copies $a0 into a SECOND callee-saved register
      ($s6) and uses that at the two sites that need a sign-extended
      halfword -- func_800A9C18's fourth argument and func_b31f_8016F160's
      first -- while `a & 3' reads the parameter register itself.  Naming
      `a' at both call sites instead is 133 words / 83 rows.
   2. THE func_b31f_8016F160 CALL NEEDS ITS SECOND AND THIRD ARGUMENTS
      SIGN-EXTENDED, SO THAT FUNCTION'S `u16 b, u16 c' WIDTHS ARE WRONG.
      The target emits `sll'/`sra' pairs for `b + 1' and `c'; against the
      landed declaration `u8 *func_b31f_8016F160(s16 a, u16 b, u16 c, s16 d)'
      the candidate emits `andi 0xFFFF' for each and comes out TWO WORDS
      SHORT (129/53).  func_b31f_8016F160's own comment says its b/c widths
      were "CARRIED UNMEASURED" from csrc/ovl_dng_b44.c, and this is the
      first measurement of them.  I did NOT change its prototype, because
      make has deleted its listing and I cannot re-verify its 96 words;
      instead this body calls it through a unit-local alias
      `func_b31f_8016F160_s(s16, s16, s16, s16) __asm__("func_b31f_8016F160")'.
      THE DISPATCHER SHOULD DECIDE whether to widen the real prototype to
      `(s16, s16, s16, s16)' once that listing exists again.

   MEASURED AND REJECTED -- 20 spellings, and the sweep is FLAT AT 49 on
   every axis, which is the signature of a fault none of the axes contains:
   | spelling                                               | words | aligned |
   | as written below                                       |  131  |   49    |
   | `a' at both call sites, no alias                        |  133  |   83    |
   | func_b31f_8016F160 called with its declared u16/u16     |  129  |   53    |
   | `u32 d'                                                 |  131  |   73    |
   TWELVE PERMUTATIONS OF THE LOCAL DECLARATION ORDER (n/m/r/p/w/e/g/k,
   including p, r and w first and g moved) all measure 131/49 EXACTLY, which
   is the brief's test for ruling out allocation priority.
   SIX PARAMETER-WIDTH COMBINATIONS all measure 131/49 as well: (s16,u16,
   u16,s16), (u16,u16,u16,u16), (s16,u16,u16,u16), (u16,u16,u16,s16),
   (s16,s16,s16,s16), (s32,u16,u16,s16).  Only `u32 d' is distinguishable,
   at 73, so `d' is 16 bits wide and its signedness is not measurable.
   THREE POSITIONS FOR `w = p' (before `k = a & 3', after it, after the
   +0x2C dress, and immediately after `p') all measure 131/49, and `g = a'
   before the spawn call or reading `g & 3' instead of `a & 3' are also
   131/49.  `g = a' INSIDE the `if' is 51.

   WHAT I WOULD TRY NEXT: the rotation's handle is which pseudo shares the
   spilled `d's register.  The target's `r' takes it (r is born one insn
   after d's last use); the candidate's `w' does.  That is a question about
   where `w' and `r' are BORN relative to the `*(s16 *)(m + 0xA) = d' store,
   and the three `w' positions I tried did not move it -- so try moving the
   `r' load instead, and price `-da' `.greg's allocation order line rather
   than its reference counts (the counts are equal by construction here).

   DECLARATIONS
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   extern u8 D_b31f_8017467C[];
   extern u8 *D_b31f_80174704;
   void func_b31f_8016B0E8();
   void func_b31f_8016B778();
   u8 *func_b31f_8016F160_s(s16, s16, s16, s16) __asm__("func_b31f_8016F160");
   s32 func_800A6DA4(u32 lo, u32 hi);
   void func_80042640(Unk42710 *, s16);
   s32 func_800A152C(s16 a, s16 b);
   s32 func_800AA36C(u8 *, void *, void *, u8 *);
   s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
   UnkNode *func_8003FD64(s32, void *);
   END DECLARATIONS */
u8 *func_b31f_8016AEDC(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    u8 *e;
    s16 g;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    g = a;
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        D_b31f_80174704 = (u8 *)n;
        n->unk_10 = (u32)func_b31f_8016B0E8;
        p[0x13] = 0x31;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        w = p;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        k = a & 3;
        *(u8 **)((u8 *)r + 0x2C) = D_b31f_8017467C;
        *((u8 *)r + 0x24) = b;
        *((u8 *)r + 0x25) = c;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_800A9C18(n, m, r, g);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b31f_8016B778;
        *(s16 *)(w + 0xAE) = 0;
        w[0xB4] = 0;
        w[0xB0] = 0;
        r->unk_14 |= 0x80;
        func_800AA36C(w, m, r, p);
        w[0x49] = 0xE;
        w[0x48] = 3;
        w[0x4A] = func_800A6DA4(0x10, 0x18);
        w[0x4B] = 1;
    }
    e = func_b31f_8016F160_s(g, b + 1, c, d);
    func_800A152C(e[0x13], 1);
    func_80042640((Unk42710 *)e, e[0x13]);
    e[0x43] = 0xFF;
    e[0x12] = 0;
    return p;
}
