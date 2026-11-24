#include "common.h"

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b22_80172F58(u8 *p, void *b, void *c, u8 *d);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b22_801724BC();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

extern s16 D_80083228;
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_b22_801720B4(u8 *p, UnkVec *b, u8 *c);
extern u8 D_8006CCF8[];
extern void (*D_80175EF4[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800478B8(u8 *);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
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

typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_b22_8017086C(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

s32 func_8009A180(u8 *a, u8 *b);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_b22_80173734(u8 *p, void *b, void *c, u8 *d);
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_8017086C);

/* Walk the tag chain from `p` and hand every node to func_b22_8017086C with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b22_80170CEC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b22_8017086C(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80170D40);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80170DC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80170E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80170EF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80170F94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_8017121C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801714FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171790);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171928);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171A38);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171BEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171D64);

/* The banked room script's per-frame actor driver, the same relinked twin of
   the resident overlay's func_800D8728 (csrc/ovl_dungeon_800D50FC.c) that
   landed as func_b03_80170A28 and its thirteen siblings: past the engine's
   0x2000 gate it runs func_800A9E70, then the record's own +0x8C
   continuation, then the +0x9A entry of this bank's D_80175EF4 facing table,
   re-aims the tile when the +0x6D facing moved under it, re-skins the slot
   for the eight-way facing, steps the mover by its velocity, runs the +0x9D
   bob counter and leashes +0x92/+0x88 to the ground height func_800BCB04
   reports. Bank-local deltas from func_b03_80170A28: the +0x8C gate compares
   against func_b22_801724BC and the facing table is D_80175EF4.

   THE ONE SHAPE DELTA FROM THE FOURTEEN LANDED SIBLINGS, and the reason this
   body is 258 words where theirs are 267: the three-way ground-height arm's
   common tail -- b->unk_14 = 0, the +0x1C 0x8000000 set and p[0x9D] = 0 --
   is written ONCE here, with the +0x90 / +0x92 stores as a nested if/else
   inside the else arm, not duplicated into both arms the way every sibling
   writes it. The siblings' duplicated spelling is not merely longer: the
   extra arm carries a second `q + 0x1C` reference, and dropping it moves q
   below p in global.c's allocno rank, so p takes $s1 and q takes $s2 here
   where the siblings have them the other way round. That is not a choice --
   writing the tail twice puts q back in $s1 and misses everywhere.

   The three spellings func_800D8728's and func_b03_80170A28's comments name
   are all load-bearing here too, re-measured in this unit:

   | spelling                                      | words | aligned diffs |
   | as written below                              |  258  |      0        |
   | the siblings' duplicated tail                 |  267  |     74        |
   | one handler local instead of `h` and `h2`     |  258  |      3        |
   | `s32 k1`/`s32 k2` instead of `s16`            |  255  |     15        |
   | no `q` alias, `p` at every use                |  255  |    120        |

   The `& 7` facing index is read four times and the target holds it in three
   registers -- $s0 for the +0x94 store, $v1 for the +0x94 compare and the
   +0x2C subscript, $s5 for the D_8006CCF8 test, the last live across
   func_80047738 and the reason a sixth callee-saved register is saved. A
   same-width copy is copy-propagated away; the NARROWER copies survive
   because cse's make_regs_eqv never equates a subreg set. The two +0x8C
   handler reads need their own locals for the same reason: one local is a
   single global-alloc quantity and takes $v1 at both sites, where the
   target's second site is $v0. 258 words, 0 differing, first compile. */
void func_b22_801720B4(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    u8 *e;
    s16 dir;
    s32 r;
    s32 v;
    s16 k1;
    s16 k2;

    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b22_801724BC)
        {
            h(p, b, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)p[0x6D];
    if (func_800A9E70(p, b, c, p) != 0)
    {
        return;
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, p);
    }
    D_80175EF4[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        k1 = v;
        k2 = v;
        if (*(s16 *)(p + 0x94) != k1)
        {
            e = *(u8 **)(c + 0x2C);
            if (e != NULL)
            {
                func_80047738(c, e[k1], *(s8 *)(c + 0x4));
            }
            *(s16 *)(p + 0x94) = v;
        }
        if (D_8006CCF8[k2] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        if ((*(s32 *)(q + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
    }
    else if ((*(u16 *)(c + 0x14) & 0x800) != 0)
    {
        *(u16 *)(c + 0x14) &= 0x8FFF;
    }
    else
    {
        *(u16 *)(c + 0x14) |= 0x7000;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(u16 *)(p + 0x98) & 0x8) != 0)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(p + 0x98) & 0x4) == 0)
    {
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
            }
            else
            {
                if ((s16)r >= *(s16 *)(q + 0x88))
                {
                    *(s32 *)(p + 0x90) = 0;
                }
                else
                {
                    *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
                }
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(q + 0x88) - 0x20);
                *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - r;
                *(u16 *)(q + 0x88) = r;
            }
        }
        else
        {
            *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
        }
    }
    else
    {
        *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801724BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80172CC0);

/* The banked room script's walk step: the 456-WORD RELINK of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c), and a DIFFERENT
   SOURCE from the 466-word relink that 46 other stubs in this block share
   (func_b01_80171C34, func_b14_80171410, ...). Read func_b01_80171C34's
   comment first: everything about the three extra pre-loop arms, the
   func_800BCB04 register-assignment fact, the `e'-sharing of the loop's
   heading value, and the D_800E2970 0x14-byte indexing is its, holds here
   verbatim, and is not repeated.

   FOUR SOURCE DELTAS FROM THE 466-WORD BODY, AND THEY ACCOUNT FOR ALL TEN
   WORDS. Every number below was measured on THIS body, one change at a time.

   1. `s32 i', not `s16 i'. That is the whole 456-vs-466 story: an s32 index
      lets loop.c strength-reduce the D_8006CD00 lookup to a pointer walk, so
      the two arms become a bare `lh $v0, 0x0($s1)' with an `addiu $s1, $s1,
      0x2' in the tail, and the `i >= 3', `i == 0' and `i < 8' tests lose
      their `sll 16'/`sra 16' pairs. `s16 i' is 468 words and 144 aligned
      differences.

   2. THE SWEEP TABLE IS NAMED INLINE AS `D_8006CD00[i]', WITH NO `s16 *tbl'
      LOCAL. This is the one spelling fact that is not visible in the
      listing's arithmetic, and it is why the 466-word body cannot simply be
      retyped. b01's `tbl = D_8006CD00;' local is 457 words and 21 aligned
      differences HERE: naming the table through a local takes four RTL insns
      out of the loop, the loop's insn_count drops below gcc 2.7.2's
      invariant-motion threshold (loop.c:532 computes
      `(loop_has_call ? 1 : 2) * (1 + n_non_fixed_regs)'; loop.c:1631 moves a
      movable when `threshold * savings * lifetime >= insn_count'), and the
      halfword extension of `f' in `i >= 3 && f != 0' is then hoisted into
      the loop preheader as an extra `move' -- costing the one word and
      rotating $s1/$s5/$s6. Written inline the index arithmetic is
      strength-reduced away AFTER that count is taken, so the final code is
      two words SHORTER while the count stays above the threshold. The
      preheader's single `lui'/`addiu' pair for D_8006CD00 is the giv's
      initial value, not a source assignment.

   3. D_8006CCD8 IS HOISTED, THROUGH A `u8 *tb' SET BETWEEN `i = 0;' AND THE
      LOOP, AND INDEXED `*(u8 *)(kk + (s32)tb)' WITH kk FIRST. This is the
      exact inverse of b01, whose `t1 = (s32)D_8006CCD8; t1 += kk;' device
      exists to give the `%lo' destination reg_n_sets != 1 and PIN the
      address inside the loop. Here the target carries it in $s7 across the
      whole loop, which is also why this copy saves nine registers' worth of
      frame plus $s7 (sp -0x40, $ra at 0x38) where b01 saves eight (sp -0x38).
      Measured: inline `*(u8 *)(kk + (s32)D_8006CCD8)' with no local is 454
      words / 20 differences; `tb[kk]' instead of `kk + (s32)tb' is 456 words
      / 1 difference, the `addu $v1, $a0, $s7' coming out `addu $v1, $s7,
      $a0'; putting `tb = ...' BEFORE `i = 0' is 456 words / 8 differences,
      because `i = 0' must be the first insn of the pre-loop join block for
      reorg to copy it into the seven branch delay slots that reach it. The
      SECOND lookup stays inline as `*(u8 *)(kk + (s32)D_8006CCE8)' -- it
      must, for the reason b01 gives.

   4. `i = (s16)func_800BCB04(...)' INTO THE s32 `i', not b01's s16-returning
      func_800BCB04h alias. The target's `sll $v0, $v0, 16' / `sra $s3, $v0,
      16' sign-extends straight into `i'; dropping the `(s16)' is 455 words /
      5 differences.

   TWO OF b01'S DEVICES DIVERGE. Its `do { return; } while (0)' in the
   `i == 0' arm must be REMOVED -- 459 words / 14 differences with it, because
   with an s32 index reorg does not want `i++' in those delay slots and fills
   them with the `lui %hi(D_80082EA4)' and a `nop' instead. Its `cc2 =
   0x80000000' do-while(0) reference bump is KEPT verbatim: spelled plainly as
   `*(s32 *)(d + 0x14) |= 0x80000000;' this body is 456 words / 4 differences,
   the same swap of $v0 and $v1 b01 measures.

   456 words, 0 differing, and 0 aligned differences. No jump table and no
   rodata of its own -- the listing opens at `glabel'. The second parameter is
   never read and still has to be declared and passed on, as in b01. */
void func_b22_80172F58(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b22_80173734(p, b, c, d) == 0)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80173678);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80173734);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80173B48);

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b22_80173EAC(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b22_80172F58(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b22_80172F58(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b22_801724BC. 31 words,
   0 differing, first compile. */
void func_b22_80173FF0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b22_801724BC;
        *(s32 *)(p + 0x90) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_8017406C);

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
void func_b22_80174360(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801743C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_8017472C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80174C44);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FB8(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FD8(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FF8(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b22_80175018(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80175050);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80175574);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801759A0);

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
void func_b22_80175C48(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        }
    }
}

/* 14 words, 0 differing. */
void func_b22_80175CAC(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175CE4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D1C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D54(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D8C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}
