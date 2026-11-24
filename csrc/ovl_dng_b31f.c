#include "common.h"

s32 func_800AC010();

s32 func_800AC82C();
s32 func_800AD9B4();

void func_b31f_8016F78C();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b31f_8016B778();

s32 func_b31f_8016ADB8(s32 n);

s32 func_800AB778();

s32 func_800AB538();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;

void func_80047784(u8 *p, s32 b, s32 c);
extern s16 D_80083228;
void func_80047784(u8 *, s32, s32);
extern u8 D_b31f_801746D4[];

extern u8 D_b31f_80174A7C[];

u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s32 func_b31f_8016C720(u8 *p, UnkVec *b, u8 *c, u8 *d);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

void func_800478B8(u8 *);

s32 func_800AB378();
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800A4ACC(u8 *p);
extern u16 D_80083462;
extern u8 D_b31f_80174674[];
void func_b31f_8016C698(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern u8 D_80082E80[];
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s32 func_8009A180(u8 *a, u8 *b);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
s32 func_800BCB04(s32, s32, s16);
u16 func_800A6D30(void);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
void func_8009A21C(s32 a, s32 b, s32 c);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_800A9A0C(u8 *);

extern u8 D_b31f_80174A24[];
void func_b31f_80170418(u8 *p, UnkVec *b, u8 *c, u8 *d);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A908);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A928);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A9FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016ABC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AC30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AD00);

/* The bank's "random value below n": six masked draws from rand, picked by
   the size of n, each retried until it falls under n. Exact dngfam clone of
   the landed func_s72_8002458C (csrc/ovl_dng_s72.c) at ratio 1.000; the masks
   and shifts (0xC>>2, 0xF0>>4, 0xFC>>2, 0xFF0>>4, 0xFFF, 0xFFFF) and the five
   `slti' bounds 5, 0x11, 0x41, 0x101, 0x1001 were re-read off this unit's own
   listing and all agree. 73 words, 0 differing, first compile. No jump table
   and no rodata of its own -- the listing opens at `glabel'. */
s32 func_b31f_8016ADB8(s32 n)
{
    s32 r;

    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AEDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016B0E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016B778);

/* Advance the record one step along its stored 0x74/0x7C trail: the 0.956
   relink of the landed func_b25_801724B0 (csrc/ovl_dng_b25.c, 143 words).
   Read b25's comment for the shape -- the inverted func_b31f_8016C698 gate,
   `t = (s16)func_8009A66C(...)' cast AT THE ASSIGNMENT, and THE DIVISOR IN
   THE TAIL BEING `t' REUSED rather than a separate local. The merged divisor
   was RE-MEASURED on this body and holds harder here than on b25's: a
   separate `s32 n' for it is 153 words and 41 aligned differences here
   against b25's 144 / 18. The inverted gate and the cast-at-the-assignment
   were carried over unmeasured; b25 priced the cast on its own body at 8.

   FOUR DELTAS FROM b25, AND THE LAST ONE IS THE ONLY ONE THAT TOOK WORK.

   1. The facing table is the bank-local D_b31f_80174674, not the resident
      D_800E2348.
   2. b25's trailing `*(s16 *)(p + 0x9E) = 0;' after func_80047784 IS ABSENT
      here -- this listing has no `sh $zero, 0x9E($s4)'.
   3. A NEW PRE-TEST BLOCK between the two halves of b25's `&&', so the
      0x8A test is NESTED rather than `&&'-joined:
      `if (p[0x9A] != 0xF) { *(s32 *)(p + 0x90) = 0; b->unk_14 = 0;
      p[0x9D] = 0; }'. That is the `lbu $v0, 0x9A($s4)' / `addiu $s5, $zero,
      0xF' / `beq $v0, $s5' head and the three stores behind it, 7 words.
   4. THE `p[0x9A] = 0xF' ARM IS WRITTEN TWICE, under a SPLIT gate -- `if
      (t == 3) { if (gate) call; else p[0x9A] = 0xF; } else p[0x9A] = 0xF;'
      -- not once under b25's three-term `&&'. MEASURED: b25's single-arm
      spelling is 149 words and 19 aligned differences here, and every one of
      those 19 rows is this one cause. Duplicating the arm gives 0xF two
      references far apart, so allocno_compare hands the constant a
      callee-saved register of its own: the target keeps 0xF in $s5 for the
      whole body (shared with the new pre-test's compare), pushes `b' out to
      $s6, and pays the extra `sw $s5, 0x2C($sp)' / `lw $s6, 0x30($sp)'
      frame pair. Those two words plus the duplicated `sb' are exactly the
      three this body has over the single-arm spelling, and the `t != 3' arm
      is the one that gets $s5 while the inner gate's arm rematerialises 0xF
      into $v0 in its two branch delay slots. Probably macro residue: the
      three-term gate reads like a configuration macro collapsing to two
      nested tests.

   152 words, 0 differing, 0 aligned differences. No jump table and no rodata
   of its own -- the listing opens at `glabel'. The second parameter is
   written through once (b->unk_14) and otherwise only passed on. */
void func_b31f_8016BD14(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0)
    {
        if (p[0x9A] != 0xF)
        {
            *(s32 *)(p + 0x90) = 0;
            b->unk_14 = 0;
            p[0x9D] = 0;
        }
        if (d[0x71] > *(s16 *)(d + 0x8A))
        {
            if (*(u8 **)(c + 0x2C) != D_b31f_80174674)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174674;
                func_80047784(c, D_b31f_80174674[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            x = c[0x24];
            y = c[0x25];
            func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            r = func_800A0818(x, y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
            t = (s16)func_8009A66C((s16)r, c, d, 0x20);
            c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
            c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            *(s16 *)(d + 0x2A) = r;
            if (t == 3)
            {
                if ((D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
                {
                    func_b31f_8016C698(p, b, c, d);
                }
                else
                {
                    p[0x9A] = 0xF;
                }
            }
            else
            {
                p[0x9A] = 0xF;
            }
            *(s32 *)(p + 0x8C) = 0;
            *(u32 *)(d + 0x1C) |= 0x40000000;
            if ((D_80083462 & 0x80) != 0)
            {
                *(s16 *)(p + 0x96) = 0;
            }
            else
            {
                *(s16 *)(p + 0x96) = 8;
                t = d[0x71];
                if (t > 0)
                {
                    *(s16 *)(p + 0x96) = 8 / t;
                }
            }
        }
    }
}

/* The banked room script's walk step: a 457-word relink of the same source as
   the landed func_b25_801726EC (csrc/ovl_dng_b25.c, 456 words, ratio 0.997).
   READ THAT COMMENT FIRST -- the four source deltas it documents from the
   466-word variant (s32 `i', D_8006CD00[i] named inline with no `s16 *tbl'
   local, D_8006CCD8 hoisted into `tb' and indexed `*(u8 *)(kk + (s32)tb)'
   with kk first, and `i = (s16)func_800BCB04(...)') all hold here verbatim
   and are not repeated.

   LISTING-AGAINST-LISTING, THIS COPY DIFFERS FROM b25'S IN EXACTLY THREE
   PLACES and they account for the one extra word:

   1. the bank-local callee is func_b31f_8016C720, not func_b25_80172E80;
   2. func_8009A66C, not func_8009A8C0, at BOTH walk-step sites;
   3. THE ONE WORD. b25's second top-level arm tests the already-loaded
      `v' -- `andi $v0, $v1, 0x2000' -- where this copy reloads a byte from
      `p': `lbu $v0, 0xB2($s5)' plus the load-delay `nop'. So the gate is
      `p[0xB2] != 0', not `(v & 0x2000) != 0'. $v1 still carries `v' into the
      arm's `(v & 0x20000)' test below, which is why only the one word moves.

   457 words, 0 differing, on the first compile with those three changes.

   THREE OF b25'S FACTS RE-MEASURED ON THIS BODY, ONE CHANGE AT A TIME, AND
   ALL THREE HOLD: `s16 i' in place of `s32 i' is 469 words / 144 aligned
   differences; `tb[kk]' in place of `*(u8 *)(kk + (s32)tb)' is 457 words / 1
   aligned difference, the `addu $v1, $a0, $s7' coming out `addu $v1, $s7,
   $a0'; and dropping the `cc2' do-while(0) reference bump for a plain
   `*(s32 *)(d + 0x14) |= 0x80000000;' is 457 words / 4 aligned differences.

   No jump table and no rodata of its own -- the listing opens at `glabel'.
   The second parameter is never read and still has to be declared and passed
   on. */
void func_b31f_8016BF74(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 f;
    s32 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 x;
    s32 y;
    u8 *tb;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b31f_8016C720(p, (UnkVec *)b, (u8 *)c, d) == 0)
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
    else if (p[0xB2] != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
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
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                              (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                   (u16 *)(p + 0x98));
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tb = (u8 *)D_8006CCD8;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - D_8006CD00[i];
        }
        else
        {
            s = e + D_8006CD00[i];
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
            *(u8 *)(c + 0x24) += *(u8 *)(kk + (s32)tb);
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
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
    i = (s16)func_800BCB04((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* csrc/ovl_dng_b05.c's func_b05_80171D6C facing-table installer on the
   bank-local D_b31f_801746D4, with a *(s32 *)(p + 0x90) = 0 / b->unk_14 = 0 /
   p[0x9D] = 0 tail. The table base is a pointer local because it is used
   twice -- stored at c+0x2C and indexed. 34 words, 0 differing, first
   compile. */
void func_b31f_8016C698(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_b31f_801746D4;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(s32 *)(p + 0x90) = 0;
    b->unk_14 = 0;
    p[0x9D] = 0;
}

/* Can the record step onto the slot it is facing, and if so close on it:
   func_b04_80172270 (csrc/ovl_dng_b04.c) with its D_80175140 re-dress and
   func_80047784 call removed and d[0x85] set to 4 rather than 0 -- so the
   whole func_800A04F0 / func_800A2CB8 / func_800A0134 / func_800A2B5C
   interrogation, the func_800C7930 push, state 0x11 and the func_8009C93C
   re-aim are that function's verbatim. 99 words, 0 differing, first compile.
   Three of the twin's facts were carried over unmeasured because of that:
   the 8-byte `unused' local that makes the frame 0x40, the shared `-1' exit
   living in the SECOND func_800A2B5C test, and func_800A04F0 coming through
   the unit's `_r' alias with an `s16' fourth parameter.

   THIS UNIT'S FORWARD DECLARATION OF THIS FUNCTION HAD TO BE WIDENED from
   `s32 func_b31f_8016C720(u8 *p, void *b, void *c, u8 *d)' to the real
   `(u8 *p, UnkVec *b, u8 *c, u8 *d)'. Its only caller, func_b15_801721D8
   below, passes `void *' arguments that convert implicitly, so no call site
   changed; every landed body in this unit was re-verified afterwards and all
   are still 0 differing, func_b15_801721D8's 466 words included.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x6.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_b31f_8016C720(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 unused[2];

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
    {
        goto fail;
    }
    if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
    {
        return 0;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    d[0x85] = 6;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* State 8..10's second half, the 0.982 relink of the landed func_b01_80172548
   (csrc/ovl_dng_b01.c, 56 words) -- read that comment for the shape: the
   two func_800A2B5C tests on either side of the func_800C7930 push, both
   compared as halfwords, and the actor stores in listing order 0x8C, 0x9A,
   0x9B. Two deltas, both read off this unit's own listing:

   1. THE `*(s16 *)(p + 0x96) = 0' STORE IS ABSENT HERE. b01 emits
      `sh $zero, 0x96($s1)' after the 0x9B store; this listing goes straight
      from `sb $zero, 0x9B($s1)' to the 0x6D reload.
   2. `d[0x85] = 6', not b01's 0. The listing has
      `addiu $v0, $zero, 0x6' / `sb $v0, 0x85($s0)' where b01 has
      `sb $zero, 0x85($s0)'. 6 is this object's constant throughout --
      func_b31f_8016C720 above carries the same 0x4 -> 0x6 re-dress.

   56 words, 0 differing, on the first compile with those two changes.

   MEASURED AND FREE, so it is NOT the reason this lands: the D_80083462
   gate. Written through func_b31f_8016C720's pointer form instead,
   `*(u16 *)((u8 *)&D_80083460 + 0x2) & 0x2000', this body is also 56 words
   and 0 aligned differences -- gcc folds the two to the same `lui %hi' /
   `lhu %lo' pair, so the listing cannot tell them apart and the symbol
   spelling below is a readability choice only.

   MEASURED AND LOAD-BEARING: the two tail stores in the order 0x84 then
   0x85. Swapped they are 56 words and 4 aligned differences.

   No jump table and no rodata of its own -- the listing opens at `glabel'. */
void func_b31f_8016C8AC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, NULL);
    d[0x84] = 0x7C;
    d[0x85] = 6;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016C98C);

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test, in the 29-word form: the 0.933 relink of the landed
   func_b44_8017291C (csrc/ovl_dng_b44.c, 31 words) WITH ITS
   `func_800AD594(d, 5)' CALL DROPPED. That is the whole 31-to-29 delta -- the
   listing goes straight from `beqz $v0' (with a bare `nop' in its delay slot
   where b44 sets up $a0) to `jal func_800A4ACC', so there is no 0x4/0x5
   per-object constant to re-dress in this copy at all. The outer gate's
   failure falls through to the epilogue and the inner gate's failure returns,
   so both branches land on the same word; the inner result is read as a
   halfword (`sll 16' with no `sra', then `blez').

   29 words, 0 differing, first compile.

   MEASURED AND FREE, so it is NOT the reason this lands: the handler store's
   cast. `*(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;', the spelling the
   14-word installers further down this unit use, is also 29 words and 0
   aligned differences.

   No jump table and no rodata of its own -- the listing opens at `glabel'.
   func_b31f_801703A4 below is the 1.000 dngfam clone of this body with
   func_b31f_8016F78C as the installed handler. */
void func_b31f_8016CB0C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b31f_8016B778;
        *(s32 *)(p + 0x90) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CB80);

/* The same two-gate handler installer as func_b31f_8016DB08 above and as the
   landed func_b57_80175140 (csrc/ovl_dng_b57.c, 25 words, ratio 0.958), with
   TWO listing-measured deltas. The outer gate is func_800AB378, called with
   no arguments at all so the four incoming registers reach it untouched; and
   THE INNER RESULT IS NOT TESTED -- where b57 and 8016DB08 have
   `sll $v0, $v0, 16' / `blez' after the func_800AD9B4 call, this listing goes
   straight from the `jal' to the `lui %hi(func_b31f_8016B778)', so the two
   words of difference ARE the dropped `(s16)x > 0' gate and func_800AD9B4 is
   called for its effect only. The second parameter is never mentioned.
   23 words, 0 differing, first compile. No jump table and no rodata of its
   own -- the listing opens at `glabel'. */
void func_b31f_8016CED4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        func_800AD9B4(c, d);
        *(u32 *)(p + 0x8C) = (u32)func_b31f_8016B778;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CF30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D1CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D204);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D5B4);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b31f_8016D678(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b31f_8016D698(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b31f_8016D6B8(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D6F0);

void func_b31f_8016DA9C(void)
{
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b31f_8016DAA4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b31f_8016B778;
        }
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b31f_8016DB08(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b31f_8016B778;
        }
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DB6C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DBA4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DBDC(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DC14(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DC4C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

void func_b31f_8016DC84(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DC8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DCC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DDB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DE1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DECC);

/* A bare forwarder onto func_b31f_8016ADB8: one call, a bare nop in the
   delay slot, no argument set up and no `move $v0' after the call. That is
   the PASS-THROUGH -- $a0 already holds n and $v0 already holds the result,
   so the compiler emits nothing for either. 8 words, 0 differing. */
s32 func_b31f_8016DEE8(s32 n)
{
    return func_b31f_8016ADB8(n);
}

void func_b31f_8016DF08(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF50);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E120);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E138);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E264);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E300);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E434);

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_8016E138. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A858[] = {
    0x01,
    0xF4,
    0x08,
    0xF8,
    0x08,
    0xF9,
    0x06,
    0x07,
    0xF8,
    0xF8,
    0x01,
    0xF2,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E528);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EB14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016ECE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EEBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EF10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F140);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F160);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F2E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F78C);

/* Advance the record one step along its stored 0x74/0x7C trail: the 0.956
   relink of the landed func_b25_801724B0 (csrc/ovl_dng_b25.c, 143 words).
   Read b25's comment for the shape -- the inverted func_b31f_80170418 gate,
   `t = (s16)func_8009A66C(...)' cast AT THE ASSIGNMENT, and THE DIVISOR IN
   THE TAIL BEING `t' REUSED rather than a separate local. The merged divisor
   was RE-MEASURED on this body and holds harder here than on b25's: a
   separate `s32 n' for it is 153 words and 41 aligned differences here
   against b25's 144 / 18. The inverted gate and the cast-at-the-assignment
   were carried over unmeasured; b25 priced the cast on its own body at 8.

   FOUR DELTAS FROM b25, AND THE LAST ONE IS THE ONLY ONE THAT TOOK WORK.

   1. The facing table is the bank-local D_b31f_80174A24, not the resident
      D_800E2348.
   2. b25's trailing `*(s16 *)(p + 0x9E) = 0;' after func_80047784 IS ABSENT
      here -- this listing has no `sh $zero, 0x9E($s4)'.
   3. A NEW PRE-TEST BLOCK between the two halves of b25's `&&', so the
      0x8A test is NESTED rather than `&&'-joined:
      `if (p[0x9A] != 0xF) { *(s32 *)(p + 0x90) = 0; b->unk_14 = 0;
      p[0x9D] = 0; }'. That is the `lbu $v0, 0x9A($s4)' / `addiu $s5, $zero,
      0xF' / `beq $v0, $s5' head and the three stores behind it, 7 words.
   4. THE `p[0x9A] = 0xF' ARM IS WRITTEN TWICE, under a SPLIT gate -- `if
      (t == 3) { if (gate) call; else p[0x9A] = 0xF; } else p[0x9A] = 0xF;'
      -- not once under b25's three-term `&&'. MEASURED: b25's single-arm
      spelling is 149 words and 19 aligned differences here, and every one of
      those 19 rows is this one cause. Duplicating the arm gives 0xF two
      references far apart, so allocno_compare hands the constant a
      callee-saved register of its own: the target keeps 0xF in $s5 for the
      whole body (shared with the new pre-test's compare), pushes `b' out to
      $s6, and pays the extra `sw $s5, 0x2C($sp)' / `lw $s6, 0x30($sp)'
      frame pair. Those two words plus the duplicated `sb' are exactly the
      three this body has over the single-arm spelling, and the `t != 3' arm
      is the one that gets $s5 while the inner gate's arm rematerialises 0xF
      into $v0 in its two branch delay slots. Probably macro residue: the
      three-term gate reads like a configuration macro collapsing to two
      nested tests.

   152 words, 0 differing, 0 aligned differences. No jump table and no rodata
   of its own -- the listing opens at `glabel'. The second parameter is
   written through once (b->unk_14) and otherwise only passed on. */
void func_b31f_8016FA84(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0)
    {
        if (p[0x9A] != 0xF)
        {
            *(s32 *)(p + 0x90) = 0;
            b->unk_14 = 0;
            p[0x9D] = 0;
        }
        if (d[0x71] > *(s16 *)(d + 0x8A))
        {
            if (*(u8 **)(c + 0x2C) != D_b31f_80174A24)
            {
                *(u8 **)(c + 0x2C) = D_b31f_80174A24;
                func_80047784(c, D_b31f_80174A24[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            x = c[0x24];
            y = c[0x25];
            func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            r = func_800A0818(x, y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
            t = (s16)func_8009A66C((s16)r, c, d, 0x20);
            c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
            c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            *(s16 *)(d + 0x2A) = r;
            if (t == 3)
            {
                if ((D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
                {
                    func_b31f_80170418(p, b, c, d);
                }
                else
                {
                    p[0x9A] = 0xF;
                }
            }
            else
            {
                p[0x9A] = 0xF;
            }
            *(s32 *)(p + 0x8C) = 0;
            *(u32 *)(d + 0x1C) |= 0x40000000;
            if ((D_80083462 & 0x80) != 0)
            {
                *(s16 *)(p + 0x96) = 0;
            }
            else
            {
                *(s16 *)(p + 0x96) = 8;
                t = d[0x71];
                if (t > 0)
                {
                    *(s16 *)(p + 0x96) = 8 / t;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016FCE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170214);

void func_b31f_8017021C(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170224);

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test, in the 29-word form: the 0.933 relink of the landed
   func_b44_8017291C (csrc/ovl_dng_b44.c, 31 words) WITH ITS
   `func_800AD594(d, 5)' CALL DROPPED. That is the whole 31-to-29 delta -- the
   listing goes straight from `beqz $v0' (with a bare `nop' in its delay slot
   where b44 sets up $a0) to `jal func_800A4ACC', so there is no 0x4/0x5
   per-object constant to re-dress in this copy at all. The outer gate's
   failure falls through to the epilogue and the inner gate's failure returns,
   so both branches land on the same word; the inner result is read as a
   halfword (`sll 16' with no `sra', then `blez').

   29 words, 0 differing, first compile. No jump table and no rodata of its
   own -- the listing opens at `glabel'.
   The 1.000 dngfam clone of func_b31f_8016CB0C above -- listing against
   listing the only difference is the installed handler, func_b31f_8016F78C
   here against func_b31f_8016B778 there. */
void func_b31f_801703A4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b31f_8016F78C;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* csrc/ovl_dng_b05.c's func_b05_80171D6C facing-table installer on the
   bank-local D_b31f_80174A7C, with a *(s32 *)(p + 0x90) = 0 / b->unk_14 = 0 /
   p[0x9D] = 0 tail. The table base is a pointer local because it is used
   twice -- stored at c+0x2C and indexed. 34 words, 0 differing, first
   compile. */
void func_b31f_80170418(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_b31f_80174A7C;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(s32 *)(p + 0x90) = 0;
    b->unk_14 = 0;
    p[0x9D] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801704A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801707D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801707F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170814);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170834);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170854);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170874);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170894);

/* 14 words, 0 differing. */
void func_b31f_801708B4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801708EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017090C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017092C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017094C);

/* 14 words, 0 differing. */
void func_b31f_8017096C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801709A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801709C4);

/* 14 words, 0 differing. */
void func_b31f_801709E4(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

/* 14 words, 0 differing. */
void func_b31f_80170A1C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170A54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170BAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170C24);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170D44);

/* 27 words, 0 differing. */
/* The 0x312 spark's own step function: hand the renderable to func_800478B8,
   and when its +0x14 reports 0x6000 mark the owning actor retired and set the
   same bit in D_800814A0. No xsibs twin at ratio 0.40; written from the
   listing. D_800814A0 is the PLAIN scalar here, not ovl_main_8008A288's
   `D_800814A0_a[0]` array alias -- the `lui`/`lw %lo` pair is what the
   non-array spelling emits. 27 words, 0 differing, first compile. */
void func_b31f_80170D60(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170DCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017105C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017110C);

void func_b31f_80171128(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171130);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171168);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801712C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017140C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171470);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171488);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801714B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171594);

/* func_800D5AC0 (csrc/ovl_dungeon_800D50FC.c), byte for byte: one
   semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, whose layout here is the
   twin's exactly (+0x1C cursor, +0x24 OT base, +0xD0/+0xD4 out-parameters,
   +0x100 index). The three dead lbu's at +0x4/+0x5/+0x6 are the
   setRGB0(t, t->r0, t->g0, t->b0) echo of the colour word just copied in;
   the twin's own comment carries the measurements for that and for its three
   load-bearing orderings, which were not re-measured here because the first
   compile of the twin's body landed. 120 words, 0 differing. */
s32 func_b31f_80171704(u8 *p, UnkVec *v)
{
    UnkDng708A4SP *sp = (UnkDng708A4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = RotTransPers((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

/* One frame of a fading burst, and a leaf: func_800D5CA0
   (csrc/ovl_dungeon_800D50FC.c) byte for byte, with nothing relinked -- the
   only symbol it touches outside itself is D_800814A0, which is resident.
   Integrate the mover, halve each of its three velocities, scale the three
   colour bytes by the remaining life over the original life, copy the colour
   word forward, and when the life runs out raise the node's and the engine's
   0x8000 flags. The three `srl 31'/`addu'/`sra 1' triples are the signed
   `/= 2'; `>>= 1' would drop the correction. 75 words, 0 differing, first
   compile. func_b15_80170F2C below installs it as its step function. */
void func_b31f_801718E4(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171A10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171CC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171D90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171E60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171F30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801720D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172160);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172274);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801722F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801723D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801724D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801725F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172610);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172874);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172A14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172B00);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172CC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172DFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172F44);

/* 0x18 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_80171168. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A87C[] = {
    0x00,
    0xF5,
    0x00,
    0xF5,
    0x01,
    0xF5,
    0x0B,
    0xF5,
    0xF5,
    0xFD,
    0x08,
    0xF8,
    0x00,
    0xF2,
    0x08,
    0xF2,
    0xF8,
    0xF5,
    0xFD,
    0xEB,
    0xFF,
    0xF4,
    0xFA,
    0x08,
};

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_80171A10, func_b31f_801730AC. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A894[] = {
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801730AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E38);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173F6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801740F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80174320);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80174674);
