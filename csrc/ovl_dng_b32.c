#include "common.h"

extern u32 D_800814A8;
extern u8 D_80174F40[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
s32 func_8009A180(u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b32_80174060(u8 *p, u8 *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_b03_80171DA0(u8 *p, UnkVec *b, u8 *c, u8 *d);

extern u8 D_8006CCF8[];
extern void (*D_80174F60[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800478B8(u8 *);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b32_80172088(u8 *p, UnkVec *b, u8 *c, u8 *d);
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

extern Unk80083462 D_80083460;
extern u8 D_80174F00[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_b32_80174EF8[];
extern u8 D_80174F48[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b32_801718DC(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b32_801710F4();

s32 func_800AB778();

s32 func_800AB538();

void func_b32_80172024(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB464();

s32 func_800AC7EC();

extern u8 D_80174F18[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80170908);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80170AF4);

/* The banked room script's per-frame actor driver, the relinked twin of the
   resident overlay's func_800D8728 (csrc/ovl_dungeon_800D50FC.c): past the
   engine's 0x2000 gate it runs func_800A9E70, then the record's own +0x8C
   continuation, then the +0x9A entry of the D_80174F60 table, re-aims the
   tile when the +0x6D facing moved under it, re-skins the slot for the
   eight-way facing, steps the mover by its velocity, runs the +0x9D bob
   counter and leashes +0x92/+0x88 to the ground height func_800BCB04
   reports. It is func_800D8728 without that one's `*(s16 *)(c + 0x12) = 0`
   opener and without its 0xE-state +0xA0 countdown block, and its facing
   table is subscripted through a `u8 *` -- `addu`/`lbu`, not the twin's
   `void **` word index. 267 words, 0 differing on the first compile.

   THE THREE SPELLINGS func_800D8728's COMMENT NAMES ARE ALL LOAD-BEARING
   HERE TOO, re-measured in this unit:

   | spelling                                   | words | aligned diffs |
   | as written below                           |  267  |      0        |
   | one handler local instead of `h` and `h2`  |  267  |      3        |
   | `s32 k1`/`s32 k2` instead of `s16`         |  264  |     15        |
   | no `q` alias, `p` at every use             |  262  |    132        |

   The `& 7` facing index is read four times and the target holds it in three
   registers -- $s0 for the +0x94 store, $v1 for the +0x94 compare and the
   +0x2C subscript, $s5 for the D_8006CCF8 test, the last live across
   func_80047738 and the reason a sixth callee-saved register is saved. A
   same-width copy is copy-propagated away; the NARROWER copies survive
   because cse's make_regs_eqv never equates a subreg set (see
   docs/matching-levers.md, "A `u8` destination makes a register copy
   INVISIBLE to cse"). The two +0x8C handler reads need their own locals for
   the same reason the twin does: one local is a single global-alloc quantity
   and takes $v1 at both sites, where the target's second site is $v0. */
void func_b32_80170CC8(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b32_801710F4)
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
    D_80174F60[p[0x9A]](p, b, c, p);
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
            else if ((s16)r >= *(s16 *)(q + 0x88))
            {
                *(s32 *)(p + 0x90) = 0;
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
            }
            else
            {
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
                *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_801710F4);

/* The banked room script's step-the-walk handler, the relinked twin of the
   landed func_b07_80171570 in csrc/ovl_dng_b07.c (itself the resident
   overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c): advance the actor
   one cell of its queued path, swap in the D_b32_80174EF8 facing table,
   re-aim the sprite, and either stop at state 0xF or hand on to
   func_b32_80172024.  150 words, 0 differing.  12 copies (dngfam.py).

   IT IS func_b07_80171570 PLUS FIVE WORDS: the `t != 3' arm also clears bit 3
   of the halfword at p + 0x98, so the two 0xF stores cannot be one else arm
   and the gate has to be NESTED.  b07's flat
   `(s16)t == 3 && !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)' with 0xF as
   the single else is 145 words / 42 differing here.  Spelling the inner gate
   the other way round -- `||' with the 0xF store as the taken arm -- is 10
   differing: jump.c then cross-jumps one of the two `sw $zero, 0x8C($s4)'
   stores the target keeps in both.  Hoisting the 0xFFF7 clear into its own
   `if ((s16)t != 3)' ahead of the flat `&&' gate is 24, and leading with
   `if ((s16)t != 3)' and trailing the call as the final else is 149 words /
   60.

   DEVICE: `x + ((x + x) - x - x)' on func_800A0818's first argument.  It is
   the phantom zero of docs/matching-levers.md :11693 -- cse keeps the three
   insns, flow counts them as references on `x', and combine deletes them
   (simplify_binary_operation's `X * C - X' and `X - X' cases), so it emits
   nothing and ties nothing.  Its job here is allocation only: without it `d'
   outranks the `x'/`t' quantity in global-alloc and the two swap
   callee-saved registers, `d' taking $s0 and `x' $s1 where the target has
   $s1 and $s0 -- 150 words, 28 differing, every one of them that rename
   (and `t', which shares `x's register, renames with it).  What the function
   does without it: the same 150 instructions with two callee-saved registers
   exchanged.  It does NOT warn under -Wall: nothing is read uninitialised.

   Measured and rejected, all 150 words unless stated:
     no device at all                                              28
     no device, declaring `x' and `y' first                         28
     no device, `s16 x, y'                                          28
     no device, `d' copied into a local before the first test       28
     the phantom on `y' as well as `x'                              28
     the phantom on func_8009A3D0's first argument instead           7
     the phantom as its own statement, `x = x + ((x + x) - x - x);'  2
     `__asm__("" ::"r"(x));' after the two assignments               2
     `__asm__("" ::"r"(x));' before the func_8009A3D0 call           2
     the entry gate as `*(s16 *)(d + 0x8A) < d[0x71]'                2
     the entry gate's first test as `d[0x71] > 0', no `(s8)' cast    3
     merging `t' into `x'                                           37
     no device, `u8 x, y'                                151 words, 123
     merging `r' into `y'                                151 words, 107
     a local for the 0x300/0x3000 select                 151 words,  75
     `x' and `y' inline, no locals                       149 words, 125

   `--raw' is 17 differing and every one is a relocation -- the six `jal's,
   the three `j's, and the `%hi'/`%lo' pairs for D_b32_80174EF8, D_80083228
   and the two D_80083462 loads -- so no masked word is hiding a constant
   address.  Hoisting D_b32_80174EF8 into a local and writing the 0x8A bump
   as `*(s16 *)(d + 0x8A) += 1' are each 0 differing too; the forms kept here
   are b07's, for the replicator's sake. */
void func_b32_80171684(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b32_80174EF8)
        {
            *(u8 **)(c + 0x2C) = D_b32_80174EF8;
            func_80047784(c, D_b32_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(/* DEVICE: the phantom zero.  See the header. */
                          x + ((x + x) - x - x), y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
        t = func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if ((s16)t == 3)
        {
            if ((D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
            {
                func_b32_80172024(p, b, c, d);
            }
            else
            {
                p[0x9A] = 0xF;
            }
        }
        else
        {
            p[0x9A] = 0xF;
            *(u16 *)(p + 0x98) &= 0xFFF7;
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
            n = d[0x71];
            if (n > 0)
            {
                *(s16 *)(p + 0x96) = 8 / n;
            }
        }
    }
}

/* The banked room script's walk step, the relinked twin of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c) with three extra
   pre-loop arms: the +0x400/+0x10 pair that re-aims through func_800A02AC or
   func_800A04F0, and the +0x2000/+0x20000 arm that compares the cursor
   against D_80082E80's. Everything after that is func_800D92C0's eight-step
   D_8006CD00 sweep. 466 words, 0 differing.

   THE SECOND PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED
   AND PASSED ON. func_b32_80172088's call sets only $a0, $a2 and $a3, so its
   $a1 is this function's own incoming $a1, reaching it untouched; the
   parameter is there to carry it.

   func_800BCB04 HAS TO COME THROUGH THE s16 ALIAS, AND IT DECIDES THE WHOLE
   FUNCTION'S REGISTER ASSIGNMENT. `i = func_800BCB04h(...)' is a halfword
   assignment into `i' itself; with the landed `s32 func_800BCB04' and an
   `(s16)' cast the result needs a temporary instead, which takes `i's
   allocno from 17 references over 113 insns down to 14 over 107. The
   priority global.c's allocno_compare computes is
   floor_log2(refs)*refs/live_length, so 4*17/113 beats `d's 6*85/860 and `i'
   is allocated first and takes $s1, while 3*14/107 does not and `i', `d' and
   `c' come out rotated: 466 words, 126 differing.

   AND THE LOOP'S HEADING VALUE SHARES `e' WITH THE TWO ACTOR LOOKUPS. In its
   own local of any type or spelling it is 3 differing -- the `lh' of d + 0x2A
   lands in $a0 where the target has $v1 -- because global.c's
   expand_preferences gives it `s's hard-register preference (it dies in the
   insn that sets `s') and `s' prefers $a0, where the `sll'/`sra' feeding
   func_8009A66C's first argument is local-allocated. Sharing `e', whose other
   live range is in the pre-loop arms, gives the allocno the hard-register
   conflicts that put $a0 out of reach, and find_reg keeps $v1.

   Measured, on this body, by deletion: writing the 0x20000 arm's
   D_80082E80 through a local is 465 words / 23 differing, and the resident
   twin's own note explains why the D_8006CCE8 lookup must stay inline. The
   first gate with the func_800A9A0C call as the *taken* arm rather than the
   if-body is 11 differing. The tail's D_80083460 through `g' instead of its
   own `g2' is 3 differing. The `(s16)' casts on the three s32-returning
   callees are load-bearing -- func_8009A66C 2 words, func_b32_80172088 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b32_801718DC(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 t1;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b32_80172088(p, b, c, d) == 0)
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
            /* DEVICE: the constant in a local, and the read-modify-write
               wrapped in a do-while(0). Worth 4 differing: without it the
               function still builds at 466 words but the loaded word and
               the `lui 0x8000' swap $v0 and $v1. Plain
               `*(s32 *)(d + 0x14) |= 0x80000000;' is 4 differing, as are
               `= *(s32 *)(d + 0x14) | 0x80000000', `|= (s32)0x80000000' and
               hoisting the loaded value into a local of its own. No
               uninitialised read, so nothing warns under -Wall. Same
               construct, same place, as the landed func_800D92C0's `cc2'. */
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
            /* The table address is a local that the index add writes back
               into, exactly as the landed func_800D92C0 needs it: the `%lo'
               insn's destination then has reg_n_sets != 1. Written inline as
               `*(u8 *)(kk + (s32)D_8006CCD8)' it is 8 differing. The second
               lookup must stay inline -- cse knows D_8006CCE8 is
               D_8006CCD8 + 0x10 and folds its `%hi'/`%lo' away. */
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
                /* DEVICE: the return wrapped in a do-while(0). Worth 4
                   differing. A plain `return;' here leaves reorg filling
                   only one of the three branches into the `i++' join with a
                   copy of `addiu $v0, $s1, 0x1', where the target has all
                   three, and puts a `nop' in the third branch's delay slot
                   instead. No uninitialised read, so nothing warns under
                   -Wall. Same construct, same place, as the landed
                   func_800D92C0. */
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

/* State 0x10, and hand c the facing cell that the heading at d+0x2A selects
   out of the bank-local byte table D_80174F18: the relinked twin of the
   resident overlay's func_800D9820 in csrc/ovl_dungeon_800D50FC.c.  The
   relink is the one the rest of this block shows -- a BYTE table pushed
   through func_80047784 (`>> 9 & 7', one `lbu') where the resident has a
   POINTER table pushed through func_8003DB94 (`>> 7 & 0x1C', a `lw').  The
   base is a local because it is used twice, stored at c+0x2C and indexed,
   exactly as in the twin.  25 words, 0 differing, first compile.
   24 copies (dngfam.py).

   THE ONE FACT THAT MATTERS IS STATEMENT ORDER, not the table spelling:
   the two p stores have to come before the c+0x2C store, because 0x10 is
   materialised into $v0 and spent on p+0x9A before the table address is ever
   loaded.  Writing them after the table store is 24 words / 17 differing.
   Hoisting the d+0x2A halfword into an `s16' local is 26 words / 19 -- the
   reload is re-sign-extended instead of feeding the `addu' straight.

   Measured and rejected, all 0 differing at 25 words, so the form kept here
   is the twin's for the replicator's sake:
     no local, the symbol written out twice                         0
     `*(t + idx)' instead of `t[idx]'                               0
     the local assigned inside the store, `= t = D_80174F18'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80174F18 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b32_80172024(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80174F18;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174F00 facing table and re-aim through func_8009C93C.

   TWO THINGS THAT ARE NOT VISIBLE IN THE LOGIC.

   THE FRAME IS 0x40 WHERE THE CALLS NEED 0x38, SO THE ORIGINAL DECLARED AN
   UNREFERENCED 8-BYTE AGGREGATE LOCAL. The widest call here is
   func_8009C93C's five arguments, which is a 0x18 outgoing-argument area and
   a 0x38 frame -- which is exactly what func_800CA788 gets for the same call
   set. gcc allocates the slot for an aggregate even when flow deletes every
   reference to it, so the extra 8 bytes are a leftover declaration. Only the
   size is witnessed, never the type. No local at all, and `s32 unused;`
   instead of the array, both give 113 words and 16 differing -- every
   $sp-relative offset and both ends of the frame.

   AND THE SHARED `-1` EXIT BELONGS TO THE *LAST* GATE, NOT THE FIRST ONE THAT
   REACHES IT. All four failure exits are one `goto fail`, and the label sits
   in the second func_800A2B5C test, which is where the merged
   `j .Lb03_80171F3C` / `addiu $v0, $zero, -0x1` block is emitted -- as the
   fall-through of that test's `beqz`, with reorg then copying the constant
   into the other three branches' delay slots. func_800CA788's note puts its
   own label in its SECOND test; that is its layout, not a rule.

   | spelling                                  | words | differing |
   | four plain `return -1;`, no label         |  113  |    35     |
   | label in the second D_80083460 gate       |  113  |    36     |
   | label in the `& 0x8` arm                  |  112  |    73     |
   | label in the second func_800A2B5C test    |  113  |     0     |

   func_800A04F0 is reached through a unit-local alias because the landed
   definition in csrc/ovl_dungeon_8009F644.c is `void` while this call site
   uses $v0, and its fourth parameter has to be declared `s16` here rather
   than the definition's `u16`: `u16` makes the `lh` of d + 0x2A an
   `lh`/`andi $a3, $a3, 0xFFFF` pair -- 113 words, 1 differing.

   `d[0x6D] -= 1;` is also BEFORE the func_8009C93C call, not after it as in
   func_800CA788: after the call is 113 words, 8 differing, because the store
   can no longer sink past the `lh` of d + 0x2A. 113 words, 0 differing. */
s32 func_b32_80172088(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    u8 *e;
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
    d[0x85] = 0;
    e = D_80174F00;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The banked room script's unconditional "same slot" entry, the 70-word
   sibling of func_b03_80171DA0 above (the relinked twin of the resident
   overlay's func_800CA788) with the cursor pick and the heading window cut
   out: past the single D_80083462 0x2000 gate and the two func_800A2B5C
   tests, put the actor into state 0x11, swap the D_80174F00 facing table
   into the slot's +0x2C and re-aim through func_8009C93C.

   THIS ONE IS `void` AND IT CARRIES NO LEFTOVER LOCAL. All three gates fall
   straight into the epilogue with no $v0 set, and the calls here need exactly
   the 0x30 frame the five register saves and func_8009C93C's 0x18 outgoing
   area add up to -- so the 8-byte aggregate func_b03_80171DA0's comment had
   to invent is absent. Adding `s32 unused[2];` back is 12 differing, every
   $sp-relative offset and both ends of the frame.

   AND THE +0x84/+0x85 PAIR MOVED TO THE TAIL. func_b03_80171DA0 stores them
   before the table swap; here they are AFTER the func_8009C93C call, which is
   the only place the scheduler cannot hoist them from. In the sibling's
   position they are 28 differing. `d[0x6D] -= 1;` stays BEFORE that call as
   in the sibling: after it is 10 differing.

   | spelling                                   | words | differing |
   | as written below                            |  70   |     0     |
   | +0x84/+0x85 before the table swap           |  70   |    28     |
   | `s32 unused[2];` restored                   |  70   |    12     |
   | `d[0x6D] -= 1;` after func_8009C93C         |  70   |    10     |
   | p stores in +0x9A, +0x9B, +0x8C order       |  70   |     2     |
   | plain `if (func_800A2B5C(d) != 0)`          |  68   |    51     |

   The `(s16)` on both func_800A2B5C tests is the target's `sll $v0, $v0, 16`
   / `bnez` halfword test; dropping it costs the two `sll`s and 51 differing.
   Three equal spellings, all 0: the gate read as
   `g = (u8 *)&D_80083460; *(u16 *)(g + 0x2)` the way the sibling spells it,
   D_80174F00 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b32_8017224C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;

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
    p[0x9A] = 0x11;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    e = D_80174F00;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

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
s32 func_b32_80172364(u8 *a, void *b, void *c, s32 d)
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
        func_b32_801718DC(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b32_801718DC(p, b, c, p);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_801724A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80172540);

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
void func_b32_801728A4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b32_801710F4;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80172908);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80172C90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80173420);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_801736EC);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b32_80173808(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b32_80173828(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b32_80173848(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80173880);

/* The banked twin of the resident overlay's func_800DA398
   (csrc/ovl_dungeon_800D50FC.c): the three-state tail of the handler
   installed at p + 0x8C, switching on the state index at p + 0x9B. State 0
   re-dresses the renderable from the D_80174F48 facing table, state 1 is the
   long refusal chain, state 2 hands the actor back to func_b32_801710F4.
   This copy adds an arm the resident twin does not have -- state 1 opens with
   the same d[0x25] test that closes it, so the dress/flag/bump tail is
   written out twice.

   THE FACING TABLE MUST BE NAMED DIRECTLY AT ALL THREE SITES, NOT HELD IN A
   `u8 *` LOCAL. One shared local for the three dressings is 207 words, 6
   differing: it is live in more than one basic block, so it becomes a global
   allocno and global.c breaks the tie between the `%hi` and the `%lo` -- the
   target's `lui $a1, %hi` / `addiu $a1, $a1, %lo` comes out as `lui $a0` /
   `addiu $a1, $a0` at each of the three sites, 2 words apiece. Three
   SEPARATE locals, one per site, also measures 0; the bare symbol is kept
   because it needs no declaration. The local is what the landed
   func_b03_80171DA0 above uses, and it is correct there because that
   function dresses only once.

   `&D_80083460` is FOUR separate pointer locals, one per arm. Sharing one
   across the arms is 179 words, 160 differing -- the shared gate load sinks
   and the arms merge.

   Other spellings measured and rejected: `d[0x6D]` read as `u8` rather than
   `*(s8 *)(d + 0x6D)` is 1 differing (`lbu` for the target's `lb`);
   `*(s32 *)(d + 0x64)` for the `lh` of d + 0x64 is 1 differing; dropping the
   `(s16)` cast on the first func_800A2C34 result is 206 words, 111
   differing; writing `p[0x9B] += 1;` after the switch with `break` in the
   reaching arms instead of as a `goto` label inside it is 207 words, 177
   differing. Measured and equal at 0 differing, so neither is the reason it
   lands: re-reading d + 0x1C at both flag tests instead of through the `v`
   local, and the three separate table locals above.

   207 words, 0 differing. */
void func_b32_80173D24(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = D_80174F48;
        func_80047784(c, D_80174F48[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174F40;
            func_80047784(c, D_80174F40[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            goto bump;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b32_80174060(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80174F40;
        func_80047784(c, D_80174F40[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x208;
        *(u32 *)(p + 0x8C) = (u32)func_b32_801710F4;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_b32_80174EF8 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b32_80174060(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b32_801710F4;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174F48 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b32_80174EF8;
        func_80047784(c, D_b32_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b32_80174128(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b32_801710F4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b32_8017418C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}

/* 14 words, 0 differing. */
void func_b32_801741C4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}

/* 14 words, 0 differing. */
void func_b32_801741FC(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174234);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174428);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174978);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174B14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174BF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b32", func_b32_80174D48);

/* 14 words, 0 differing. */
void func_b32_80174E88(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}

/* 14 words, 0 differing. */
void func_b32_80174EC0(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b32_801710F4;
    }
}
