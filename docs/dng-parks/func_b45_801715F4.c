#include "common.h"

/* PARK: func_b45_801715F4 (csrc/ovl_dng_b45.c), the banked room script per-frame
   walk/aim driver.  537 target words, 537 candidate words, 25 ALIGNED
   DIFFERENCES (aligndiff.py); 94 differing with --raw, all of them the same
   rows plus their relocations.  1 member only (dngfam.py), so no replication
   rides on it.  agent-h60, 2026-11-23.

   TWIN: func_b44_801718F8 (csrc/ovl_dng_b44.c), 466 words, dngtwin 0.907.
   func_b29_801718B4, func_b28_80171420 and func_b21_80171468 are the same
   466-word body with the same prologue.  This copy is 71 words LONGER: the
   extra statements are all in the THIRD arm (the one b44 opens with
   `j = *(s8 *)(c + 0x26)').  They are, in order:

     q = func_800A6E8C(c, 0x12, &x, &y);   at the head of the arm, so x and y
                                           become ADDRESS-TAKEN s16/u16 stack
                                           locals at 0x18/0x1A($sp) -- which
                                           is why this frame is -0x40 where
                                           every twin is -0x38
     and, in BOTH the 0x20000 arm and its else, a re-aim guarded by q:
       if (q != 0) and abs(c[0x24] - x) >= 2 or abs(c[0x25] - y) >= 2
          -> *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], x, y, w)
          else *(s16 *)(d + 0x2A) = (func_800A6D30() & 7) << 9

   THE FRAME RULE HELD AND NAMED BOTH INVERSIONS.  My prologue is
   `addiu $sp, -0x40' with seven saves in the order s5, s2, s1, s0, ra, s6,
   s4, s3; all four twins are `-0x38' with seven saves in the order s5, s3,
   s2, s0, ra, s6, s4, s1.  Same register SET, different frame and different
   save order -- so by batch 14/15s rule the twins' SHAPE facts carry and
   their ALLOCATION facts do not.  Measured: every shape fact held, and the
   two facts that are really about `i's allocno priority both inverted.

   INVERSION 1 -- func_800BCB04 MUST NOT COME THROUGH THE s16 ALIAS ALONE.
   b44s headline fact is `i = func_800BCB04h(...)' through
   `s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04")', and it says
   the plain `s32' declaration with an `(s16)' cast rotates `i', `d' and `c'
   for 125 aligned differences.  HERE THE ROTATION IS WHAT THE TARGET HAS:
   c is $s2, d is $s1, i is $s3, where b44 has c $s3, d $s2, i $s1.  With a
   separate `s16 q' for the func_800A6E8C result the alias measured 190
   aligned differences and the plain cast 63.

   INVERSION 2, AND IT IS THE REAL FIND -- THERE IS NO `q'.  THE
   func_800A6E8C RESULT, THE LOOP COUNTER AND THE func_800BCB04 RESULT ARE
   ONE s16 VARIABLE.  $s3 holds all three in the target: `addu $s3, $v0,
   $zero' at 801718F0 (func_800A6E8C), the `i = 0' / `i++' / `(s16)i < 8'
   loop, and `addu $s3, $v0, $zero' at 80171E10 (func_800BCB04).  Writing
   them as one `s16 i' fed by s16-returning aliases for BOTH calls is 41
   aligned differences; a separate `s16 q' plus `(s16)func_800BCB04(...)' is
   63, and the difference is exactly the $s3/$s4 swap between `q' and the
   0x20000 arms `w2 = (u16 *)(p + 0x98)' local plus the three tail rows.
   The arithmetic is in global.cs allocno_compare: with a separate q,
   `-da' gives q 3 refs across 92 insns (3/92 = 0.033) against w2s 3 refs
   across 62 (3/62 = 0.048), so w2 is allocated first and takes $s3.  Merged
   into `i' the allocno has 17 refs across 107 insns and outranks everything
   but `c' and `d'.  So the alias IS needed -- for func_800A6E8C as well --
   but only once `i' is the variable it feeds.  A plain `s32 i' with `(s16)i'
   at every use instead grows the frame to -0x48: 541 words, 210 differences.

   WHAT IS LEFT, 25 rows in three clusters, all register-naming or
   scheduling, no missing or extra instruction:

   (1) 11 rows, 801717B8..80171824.  The two hoisted symbol addresses are
       swapped: the target has $a1 = D_80082E80 and $a0 = D_8006CCD8, this
       body has them the other way round, and the four loads and two `sh'
       that use them follow.  Instruction-for-instruction identical
       otherwise.
   (2) 1 row, 80171A38.  The target leaves the func_800A0134 branchs delay
       slot a `nop'; reorg here fills it with the `sll $v0, $s3, 16' of the
       `i != 0' test, which the target emits only after func_8009A540.
   (3) 6 rows, 80171AA4/AA8/AB0 and 80171B1C/B20/B28.  The SECOND abs
       (`c[0x25] - y') has its two operand temps exchanged: the target is
       `lbu $v0, 0x25($s2)' / `lhu $v1, 0x1A($sp)' / `subu $v0, $v0, $v1',
       this body is `lbu $v1' / `lhu $v0' / `subu $v0, $v1, $v0'.  The FIRST
       abs matches exactly, because its x operand survives the subtraction.

   MEASURED AND REJECTED, every one on top of the body below:

     the abs pair as `if (ABS >= 2 || ABS >= 2)' with ternary abs
       expressions                                  547 words,  83
     the same as `if (ABS < 2 && ABS < 2)'           552 words,  99
       -- in both, the ternary in the SECOND operand of the connective gets
       its `slti' and branch duplicated into both arms for 5 extra words;
       which operand pays depends on which way round the connective is
     plain nested ifs with the A0818 call duplicated in each arm
                                                    537 words,  74
       -- the two random blocks then fail to cross-join and the surviving
       copy sits in the middle of arm 2 instead of after both A0818 blocks
     x and y read fresh at the A0818 call instead of through `xx'
                                                    537 words,  50
       -- the target reuses the abs `lhu' with `sll 16 / sra 16'
     `xx = x;' as its own statement rather than `(xx = x)' inside the
       subtraction                                  539 words,  50
     `s32 q' with `(s16)q != 0' tests                539 words,  50
     x and y declared `s16' with `(u16)' casts at every use
                                                    537 words,  34
     k1/k2 holding the two table ADDRESSES rather than the two loaded
       halfwords                                    539 words,  46
     both halfwords loaded through k1 and k2         537 words,  30
     `t80 = D_80082E80' local in this arm            537 words,  42
     D_8006CCD8/D_8006CCE8 as `u16' arrays indexed by the 0..7 value
                                                    537 words,  25  (free)
     `dy' replaced by a second use of `dx'           537 words,  25  (free)
     `dy = c[0x25] - (s32)y'                         537 words,  25  (free)
     `dy = c[0x25]; dy -= y;' as two statements      537 words,  33
     `yy = y' local used for the abs and the call    541 words, 109

   b44 FACTS THAT HELD HERE, re-measured where the row was reachable:
     the `cc2 = 0x80000000' local plus the do-while(0) read-modify-write in
       the 0x400 arm -- kept, and the whole arm is byte-identical
     the loop tails `t1 = (s32)D_8006CCD8; t1 += kk;' with the SECOND lookup
       left inline -- kept, loop is byte-identical
     the do-while(0) around the shared `return;' in the loop -- kept
     D_800E2970 indexed as an array of a 0x14-byte record -- kept
     the `(s16)' casts on the s32-returning callees -- kept
     THE SECOND PARAMETER IS NEVER READ and must still be declared and
       passed on to func_b45_80171EBC

   NOT APPLICABLE: b44s note that "THE 0x8000 GATE WRAPS ONLY THE ACTOR
   LOOKUP HERE, NOT THE 0x20000 ARM" is about ITS delta from b01.  This
   listing has b44s arrangement in the 0x2000 arm already, so the fact is
   neither held nor inverted -- there is nothing to choose.

   NO JUMP TABLE.  The listing has no `.section .rodata' and no `jtbl_'
   label.  All 16 %hi/%lo pairs name resident symbols -- D_8006CCD8,
   D_8006CCE8, D_8006CD00, D_800814A8, D_80082E80, D_80082EA4, D_80083460,
   D_800E2970 -- and no bank-local D_b45_* table, so there is no relocheck
   question in this body.

   DECLARATIONS
   extern u32 D_800814A8;
   extern Unk80083462 D_80083460;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s16 D_8006CD00[];
   extern u8 D_80082E80[];
   extern u16 D_80082EA4;
   typedef struct UnkDngRoom2970
   {
       u8 pad_00[0xC];
       u16 unk_0C;
       u8 pad_0E[0x6];
   } UnkDngRoom2970;
   extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
   s32 func_b45_80171EBC(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_800A9A0C(u8 *);
   s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
   u8 *func_800A02AC(u8 *p, u16 x, u16 y);
   s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
   u16 func_800A6D30(void);
   u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
   s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
   s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
   s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
   s16 func_800A0134(u8 *a, u8 *b);
   s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
   void func_8009A3D0(s32 x, s32 y, s32 f);
   void func_8009A21C(s32 a, s32 b, s32 c);
   s32 func_8009A180(u8 *a, u8 *b);
   s16 func_800A6E8Ch(u8 *p, s32 key, s16 *ox, s16 *oy) __asm__("func_800A6E8C");
   s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
   END DECLARATIONS */
void func_b45_801715F4(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    u16 *w2;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    u16 x;
    u16 y;
    u16 xx;
    s32 dx;
    s32 dy;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 k1;
    s32 k2;
    s32 kk;
    s32 t1;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b45_80171EBC(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                k2 = *(u16 *)(k + (s32)D_8006CCE8);
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + k2;
                if (*(u8 *)(c + 0x24) == x && *(u8 *)(c + 0x25) == y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        i = func_800A6E8Ch((u8 *)c, 0x12, (s16 *)&x, (s16 *)&y);
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else
        {
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
                if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                    func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                w2 = (u16 *)(p + 0x98);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25], w2);
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                    if (i != 0)
                    {
                        dx = *(u8 *)(c + 0x24) - (xx = x);
                        if (dx < 0)
                        {
                            dx = -dx;
                        }
                        if (dx < 2)
                        {
                            dy = *(u8 *)(c + 0x25) - y;
                            if (dy < 0)
                            {
                                dy = -dy;
                            }
                            if (dy < 2)
                            {
                                goto spin1;
                            }
                        }
                        *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), xx, y, w2);
                        goto aimed;
spin1:
                        *(s16 *)(d + 0x2A) = (func_800A6D30() & 7) << 9;
                    }
                }
            }
            else
            {
                if (i != 0)
                {
                    dx = *(u8 *)(c + 0x24) - (xx = x);
                    if (dx < 0)
                    {
                        dx = -dx;
                    }
                    if (dx < 2)
                    {
                        dy = *(u8 *)(c + 0x25) - y;
                        if (dy < 0)
                        {
                            dy = -dy;
                        }
                        if (dy < 2)
                        {
                            goto spin2;
                        }
                    }
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), xx, y, (u16 *)(p + 0x98));
                    goto aimed;
spin2:
                    *(s16 *)(d + 0x2A) = (func_800A6D30() & 7) << 9;
                }
                else
                {
                    func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
                }
            }
        }
    }
aimed:
    i = 0;
    tbl = D_8006CD00;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - tbl[i];
        }
        else
        {
            s = e + tbl[i];
        }
        if ((s16)func_8009A66C((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}
