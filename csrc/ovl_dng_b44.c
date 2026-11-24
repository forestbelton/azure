#include "common.h"

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b44_80172040(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80174198[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
void func_80047784(u8 *p, s32 b, s32 c);

extern s16 D_80083228;
extern u8 D_80175F18[];
extern u8 D_80175F28[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b44_801718F8(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b44_80170E94();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

extern Unk80083462 D_80083460;
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
u16 func_800A6D30(void);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b44_80172050(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u8 D_80082E80[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

s32 func_800AB464();

extern u8 D_80175F10[];
UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_b44_80170A84(u8 *p, UnkVec *b, u8 *c);

/* The banked room script's SPAWNER for the state-9 actor: allocate a 0x112
   node off D_80083498, install func_b44_80170A84 as the node's +0x10 driver,
   dress the slot from the bank-local D_80175F10 facing table, pick the
   +0x14/+0x1C flag pair from the low two bits of the caller's `a', and -- only
   for a==0 -- give one actor in two a random 0x20|rand palette out of
   func_800A48F0 and the OTHER bank-local table D_80175F28. Then hand the
   node to func_800A9C18 and arm the +0x8C continuation func_b44_80170E94.

   Not a twin of anything landed in this block: no carved sibling calls
   func_8003FD64 yet and the resident overlay has no func_800A9C18 caller in C.
   The structural template is the resident dungeon overlay's func_800B23F8
   (csrc/ovl_dungeon_800A4B88.c), which is the same allocate/dress/flag-arm/
   +0x9A-tail skeleton with a different driver, a different +0x13 state and
   without the a==0 arm; all three of ITS named facts hold here and are
   re-measured below on this body.
   114 words, 0 differing (0 aligned, and all 21 raw differences are the
   7 `jal's, 2 `j's and 6 `lui'/`addiu' pairs the verifier masks).

   THE FIRST PARAMETER MUST BE A SIGNED 16-BIT PARAMETER and that is the one
   fact that is not free. As `s32 a' with `(s16)a' written at the
   func_800A9C18 call the body is 111 words / 30 aligned differences -- the
   second copy of `a' ($s7, the one the sll/sra at the call reads) disappears
   entirely, because it only exists as the schedulable parameter copy a
   sub-word parameter type creates (see func_800B23F8's comment and
   docs/matching-levers.md, "A SUB-WORD parameter type is how a parameter copy
   becomes schedulable"). As `u16 a' it is 112 / 4: the a==0 gate folds to one
   `andi $v0, $s3, 0xFFFC'.

   `b' AND `c' MUST BE 16-BIT TOO, though they are only ever stored as the
   bytes at r+0x24 and r+0x25. Signedness is free (s16/s16 is also 0), the
   width is not: `u8 b, u8 c' and `u32 b, u32 c' are both 11 differing /
   13 aligned, `u8 b, u16 c' is 5 and `u16 b, u8 c' is 4 -- all of it the
   interleaved prologue copy order, which the target emits a0, a1, a3, a2.
   `d' is free at either width (s32 also 0).

   `w' IS A SECOND PAYLOAD POINTER and is load-bearing, exactly as in
   func_800B23F8: the three late stores go through the register the `c'
   parameter dies in ($s5), while $s0 stays live as func_800AA36C's FOURTH
   argument. Written through `p' with no `w' at all the body is 113 words /
   8 aligned differences. `w = p;' and `w = (u8 *)n + 0x20;' are
   byte-identical, and so is writing `w = p;' lower down -- the `move' lands
   between the +0x25 and +0x2C stores by scheduling, not by source order.

   `func_800AA36C(w, m, r, p)' and NOT `(w, m, r, w)': the same address twice
   costs the last word ($a3 comes from $s5), 1 aligned difference.
   `(p, m, r, p)' is 4.

   `*(s8 *)(w + 0x9C) = -1;' and NOT `w[0x9C] = 0xFF;' -- through a u8 lvalue
   gcc folds -1 to 0xFF, cse shares it with the neighbouring 0x9A store and
   the `addiu $v0, $zero, -0x1' disappears: 113 words, 2 aligned differences.

   NO DEVICES, and these are free (all measured at 114/0): the a==0 gate as
   `(a & ~3) == 0', `(s16)(a & ~3) == 0', `(u16)(a & ~3) == 0' or
   `((a & ~3) << 16) == 0' -- the truncation comes from `a' being s16, not
   from the cast; `k' inlined as `(a & 3)' at both tests; `k > 1' for
   `k >= 2'; and the two +0x2C stores spelled `*(u32 *)(...) = (u32)D_...'.
   Only `a & 0xFFFC' in the gate is wrong, at 112 / 4.

   func_b44_80170A84 and func_b44_80170E94 are address-taken here and are
   still INCLUDE_ASM stubs below, so both need a declaration above.
   THIS COPY'S OWN CONSTANT: 0x9 -> 0x2c.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
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

void func_800478B8(u8 *);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b44_80174698(u8 *p, u8 *b, u8 *c, u8 *d);
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
extern u8 D_80175F58[];
UnkNode *func_8003FC64(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);

s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
void func_800BB044(u8 *);
s32 func_800A56E0(s32);
extern s32 D_8008346C;
UnkNode *func_b44_80174BC8(u8 *p, UnkVec *b, u8 *c);
extern u8 D_80175F48[];
void func_b44_80174D24(u8 *, UnkVec *, u8 *, u8 *);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
extern u8 D_8006CCF8[];
extern void (*D_80175F70[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

extern u8 D_80175F30[];
extern u8 D_800DDC40[];
extern u8 D_80175F68[];
u8 *func_8009C12C(u8 *, u8 *, s16, s16);
s32 func_800C77D0(void *, u8 *, s16, s32);
void func_b44_80175A90(UnkVec *, UnkVec *);

u8 *func_b44_801708BC(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b44_80170A84;
        p[0x13] = 44;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80175F10;
        k = a & 3;
        *((u8 *)r + 0x24) = b;
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
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80175F28;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b44_80170E94;
        func_800AA36C(w, m, r, p);
    }
    return p;
}

/* The banked room script's per-frame actor driver: past the engine's 0x2000
   gate it runs func_800A9E70, then the record's own +0x8C continuation, then
   the +0x9A entry of this bank's D_80175F70 handler table, re-aims the tile
   when the +0x6D facing moved under it, re-skins the slot for the eight-way
   facing, steps the mover by its velocity, runs the +0x9D bob counter and
   leashes +0x92/+0x88 to the ground height func_800BCB04 reports.

   The relinked twin of the landed func_b28_80170A58 (csrc/ovl_dng_b28.c);
   transcribed from it with this bank's symbols -- D_80175F70 for D_80174568,
   func_b44_80170E94 for func_b28_80170E5C -- and ONE structural change.
   260 words, 0 differing, on the first compile.

   THE `p + 0x92 + q + 0x88 < r' ARM IS THE SIBLINGS' 0xF7FFFFFF MASK HERE,
   NOT b28's BARE VOLATILE READ, and that is the whole +3 words over b28.
   b28's own note records the mask spelling as 10 aligned differences on ITS
   body; measured here the swap runs the other way and b28's
   `*(volatile u16 *)(p + 0x98);' is 257 words with 10 aligned differences --
   the same price, the same ten rows, the opposite sign.

   Everything below is b28's note re-measured on this body.

   b28's THREE SEPARATE LOCALS FOR THE `& 7' FACING INDEX ARE LOAD-BEARING.
   `v' for the +0x94 store, `k1' for the +0x94 compare and the +0x2C
   subscript, `k2' for the D_8006CCF8 test: collapsed to one value with an
   `(s16)' cast at the compare the body is 265 words and 252 differing.

   `dir' MUST BE `s16'. As `s32' it is 257 words and 221 differing, the two
   `sll'/`sra' pairs gone. And the dir test READS `q + 0x6D', not `p + 0x6D':
   spelled `p' it is exactly 1 differing, an `lb' off $s2 where the target
   has $s1.

   func_800BCB04 comes through the plain `s32' declaration, not the
   `func_800BCB04h' s16 alias this unit also carries for
   func_b44_801718F8: through the alias it is 3 differing.

   Measured and FREE at 260 / 0, so these are NOT why it lands: the +0x8C
   handler compare written `(u32)h == (u32)func_b44_80170E94' instead of
   through the function-pointer cast. */
void func_b44_80170A84(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b44_80170E94)
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
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, q);
    }
    D_80175F70[p[0x9A]](p, b, c, q);
    if (dir != *(s8 *)(q + 0x6D))
    {
        func_800AA36C(p, b, c, q);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80170E94);

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_80175F18 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b44_80172040.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b44_80172040 is called only on
   `(s16)t == 3 && !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)`, and 0xF is
   the else arm.  The listing shows it: `bne $v0, $v1, .Lb07_8017172C` lands
   on an `addiu $v0, 0xF` of its own, the copy reorg left behind when it
   stole the else arm's `addiu` into the two `bnez` delay slots.  Written the
   twin's way round the function is 143 words and 46 differing: the two arms
   swap, and jump.c then cross-jumps the `sw $zero, 0x8C($s4)` that the
   target keeps in both.

   DEVICE: `x + ((x + x) - x - x)` on func_800A0818's first argument.  It is
   the phantom zero of docs/matching-levers.md :11693 -- cse keeps the three
   insns, flow counts them as references on `x`, and combine deletes them, so
   it emits nothing and ties nothing.  Its job here is allocation only:
   without it `d` outranks the `x`/`t` quantity in global-alloc and the two
   swap callee-saved registers, `d` taking $s0 and `x` $s1 where the target
   has $s1 and $s0 -- 145 words, 28 differing, every one of them that rename
   (and `t`, which shares `x`'s register, renames with it).  What the
   function does without it: the same 145 instructions with two callee-saved
   registers exchanged.  It does NOT warn under -Wall: nothing is read
   uninitialised.

   Measured and rejected, all 145 words unless stated:
     the phantom on func_8009A3D0's first argument instead          7
     the phantom as its own statement, `x = x + ((x + x) - x - x);` 2
     the phantom on `y` as well as `x`                              28
     `__asm__("" ::"r"(x));` before the func_8009A3D0 call          2
     no device at all                                               28
     no device, declaring `x` and `y` first                         28
     no device, `s16 x, y`                                          28
     no device, `d` copied into a local before the first test       28
     merging `t` into `x`                                           53
     merging `r` into `y`                                146 words, 112
     a local for the 0x300/0x3000 select                 146 words, 118
     `x` and `y` inline, no locals                       144 words, 120
     `x` and `y` computed before the table swap          146 words, 134 */
void func_b44_801716B4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80175F18)
        {
            *(u8 **)(c + 0x2C) = D_80175F18;
            func_80047784(c, D_80175F18[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        if ((s16)t == 3 && (D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            func_b44_80172040(p, b, c, d);
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
            n = d[0x71];
            if (n > 0)
            {
                *(s16 *)(p + 0x96) = 8 / n;
            }
        }
    }
}

/* The banked room script's walk step: the relinked twin of
   func_b01_80171C34 in csrc/ovl_dng_b01.c, which is itself the resident
   overlay's func_800D92C0 with three extra pre-loop arms.  Transcribed from
   that landed body with this bank's symbols -- func_8009A66C for
   func_8009A8C0, func_b44_80172050 for func_b01_8017237C -- and ONE
   structural change.  466 words, 0 differing, 0 aligned differences.

   THE 0x8000 GATE WRAPS ONLY THE ACTOR LOOKUP HERE, NOT THE 0x20000 ARM.
   b01 writes `else if ((d[0x46] & 0x8000) == 0)' around both the
   func_800A04F0 lookup and the 0x20000 arm, so a set 0x8000 skips the pair
   and jump.c sends the branch straight to the loop prologue with the
   `i = 0' hoisted into its delay slot.  This copy's gate closes after the
   lookup and the 0x20000 arm runs either way: the branch goes to the
   0x20000 test and its delay slot is a `nop'.  Written b01's way round it is
   466 words and 2 aligned differences, the branch offset and that
   `addu $s1, $zero, $zero'.  That one word is the only normalised
   instruction between the two listings -- `dngfam.py --diff', word 202.

   Everything below is b01's note re-measured on this bank; the numbers are
   mine, not transcribed.

   func_800BCB04 HAS TO COME THROUGH THE s16 ALIAS, AND IT DECIDES THE WHOLE
   FUNCTION'S REGISTER ASSIGNMENT.  `i = func_800BCB04h(...)' is a halfword
   assignment into `i' itself; with a plain `s32 func_800BCB04' and an
   `(s16)' cast the result needs a temporary instead, which drops `i's
   allocno priority -- global.c's allocno_compare ranks on
   floor_log2(refs)*refs/live_length -- below `d's, so `i' no longer takes
   $s1 first and `i', `d' and `c' come out rotated: 466 words, 125 aligned
   differences.

   AND THE LOOP'S HEADING VALUE SHARES `e' WITH THE TWO ACTOR LOOKUPS.  In
   its own local it is 3 aligned differences: the `lh' of d + 0x2A lands in
   $a0 where the target has $v1, because expand_preferences gives it `s's
   hard-register preference and `s' prefers $a0.  Sharing `e', whose other
   live range is in the pre-loop arms, gives the allocno the conflicts that
   put $a0 out of reach and find_reg keeps $v1.

   Both of b01's plain-C devices are still load-bearing here and both are
   kept; each is 4 aligned differences if dropped, and the details are in
   the two DEVICE comments below.  The D_8006CCD8 lookup through a local
   that the index add writes back into is 8 aligned differences written
   inline.  The tail's `t80' measures 0 either way -- D_80082E80 written
   inline in that arm is also 466 words and 0 -- and is kept only to leave
   this body a literal transcription of b01's.

   b01's remaining facts are carried over untested, because this body landed
   on the first compile: D_800E2970 indexed as an array of a 0x14-byte
   record so the +0xC read keeps its own displacement, the second
   D_8006CCE8 lookup left inline because cse folds its `%hi'/`%lo' away, the
   tail's own `g2' for D_80083460, and the `(s16)' casts on the three
   s32-returning callees.  THE SECOND PARAMETER IS NEVER READ AND MUST STILL
   BE DECLARED AND PASSED ON: func_b44_80172050's call sets only $a0, $a2
   and $a3, so its $a1 is this function's own incoming $a1 reaching it
   untouched. */
void func_b44_801718F8(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b44_80172050(p, b, c, d) == 0)
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
               wrapped in a do-while(0).  Measured here: a plain
               `*(s32 *)(d + 0x14) |= 0x80000000;' is 466 words and 4
               aligned differences, the loaded word and the `lui 0x8000'
               swapping $v0 and $v1.  No uninitialised read, so nothing
               warns under -Wall.  Same construct, same place, as
               func_b01_80171C34 and the resident func_800D92C0; the other
               spellings they rejected are in b01's note and are not
               re-measured here. */
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
               insn's destination then has reg_n_sets != 1.  Written inline
               as `*(u8 *)(kk + (s32)D_8006CCD8)' it is 466 words and 8
               aligned differences here.  The second lookup must stay
               inline -- cse knows D_8006CCE8 is D_8006CCD8 + 0x10 and folds
               its `%hi'/`%lo' away. */
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
                /* DEVICE: the return wrapped in a do-while(0).  Measured
                   here: a plain `return;' is 466 words and 4 aligned
                   differences.  No uninitialised read, so nothing warns
                   under -Wall.  Same construct, same place, as
                   func_b01_80171C34 and the resident func_800D92C0, whose
                   note has the reorg mechanism. */
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80172040);

/* The banked room script's "same slot" branch: with the D_80083460 gates
   clear, ask func_800A04F0 what sits under the cursor, and if the two
   headings are within 0x40 of each other put the actor into state 0x11, swap
   in the bank-local D_80175F48 facing table and re-aim through
   func_8009C93C.

   The relinked twin of the landed func_b45_80171EBC (csrc/ovl_dng_b45.c),
   which is itself the resident overlay's func_800CA788. Transcribed from
   that body with three deltas -- the table, `d[0x85] = 4;' where b45 has
   `= 0;', and the heading window's constants -- and landed on the first
   compile. 114 words, 0 differing.

   THE HEADING WINDOW IS `0x3F + -f > 0x7E' AND THE UNARY MINUS IS NOT
   COSMETIC. b45's constants are 0x40/0x80; this copy's are 0x3F/0x7E, which
   is the target's `negu' / `addiu 0x3F' / `andi 0xFFFF' / `sltiu 0x7F'.
   Written `0x3F - func_800A0134(t, d)' it is 5 differing -- the subtraction
   reassociates and the `negu' goes. `>= 0x7F' for `> 0x7E' is the same word
   either way (114 / 0).

   Both of b45's named facts hold here and both were re-measured.

   THE 0x40 FRAME NEEDS b45's UNREFERENCED 8-BYTE AGGREGATE LOCAL. Seven
   register saves and func_8009C93C's 0x18 outgoing area add up to 0x38;
   gcc allocates the slot for an aggregate even when flow deletes every
   reference to it. Without `s32 unused[2];' the body is 114 words and 16
   differing, every $sp-relative offset and both ends of the frame. Only the
   size is witnessed, never the type.

   AND THE SHARED `-1' EXIT BELONGS TO THE SECOND func_800A2B5C TEST, as in
   b45: four plain `return -1;' with no label is 114 words and 36 differing.

   Measured and FREE at 114 / 0, so it is NOT the reason this lands: the
   facing table used inline at both sites with no `e' local. */
s32 func_b44_80172050(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    if ((u16)(0x3F + -func_800A0134(t, d)) > 0x7E)
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
    d[0x85] = 4;
    e = D_80175F48;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The banked room script's unconditional "same slot" entry, the 71-word
   sibling of func_b44_80172050 above with the cursor pick and the heading
   window cut out: past the single D_80083462 0x2000 gate and the two
   func_800A2B5C tests, put the actor into state 0x11, swap the bank-local
   D_80175F48 facing table into the slot's +0x2C and re-aim through
   func_8009C93C.

   The relinked twin of the landed func_b45_80172080 (csrc/ovl_dng_b45.c).
   `d[0x85] = 4;' where b45 has `= 0;' is the whole +1 word over b45's 70.
   THIS ONE DID NOT LAND ON THE FIRST COMPILE: b45's statement order is 12
   differing, and the fix is one move, not two. 71 words, 0 differing.

   THE +0x84/+0x85 PAIR SITS BETWEEN func_80047784 AND `d[0x6D] -= 1;'.
   b45 puts it at the very tail, after func_8009C93C; there it is 10
   differing, the target's `addiu 0x7C' / `sb 0x84' / `addiu 0x4' /
   `sb 0x85' block emitted after the call's argument set-up instead of
   before it. func_b44_80172050 above puts the same pair before the table
   swap, which is a third position and a third body -- this is priced per
   body.

   `d[0x6D] -= 1;' STAYS BEFORE func_8009C93C: after the call it is 72 words
   and 22 differing, because the store can no longer sink past the `lh' of
   d + 0x2A.

   The `(s16)' on both func_800A2B5C tests is the target's `sll $v0, $v0, 16'
   with no `sra': plain `if (func_800A2B5C(d) != 0)' is 69 words and 52
   differing.

   NO LEFTOVER AGGREGATE HERE, unlike func_b44_80172050: all three gates fall
   straight into the epilogue with no $v0 set and the 0x30 frame is exactly
   what five saves and func_8009C93C's outgoing area need. Adding
   `s32 unused[2];' back is 12 differing.

   Measured and FREE at 71 / 0, so neither is why this lands: the p stores in
   b45's +0x9A, +0x8C, +0x9B order -- ON ITS OWN it is 0, and it only showed
   up in the first compile's 12 because it costs 2 more on top of the WRONG
   +0x84/+0x85 position; and the facing table used inline with no `e'
   local. */
void func_b44_80172218(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    e = D_80175F48;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x84] = 0x7C;
    d[0x85] = 4;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80172334);

/* State 0x18's entry: clear d + 0x71's top bit, refuse (returning -1) if the
   engine is in its 0x2000 mode or -- when d + 0x46's 0x8000 bit is clear --
   if D_80083462's 8 bit is set, then push the record 0x300 through
   func_800C7930 with func_800A2B5C refusing on either side of it, reset the
   actor to state 0x18 with both counters cleared, publish the bank-local
   D_80175F30 facing table at c + 0x2C, dress through func_80047784, and
   spend one of d + 0x6D.  Returns 0 only on the path that dresses.
   75 words, 0 differing, FIRST COMPILE.

   The twin is the landed func_b42_801756E0 (csrc/ovl_dng_b42.c, 64 words,
   dngtwin 0.849; func_b28_8017236C, func_b10_80174798 and func_b09_80174928
   are the same body at the same ratio).

   ITS ONE NAMED FACT INVERTED, AND THE RESIDUE IS ITS OWN REJECTED PRICE.
   b42 keeps `*(s32 *)(p + 0x8C) = 0' BETWEEN the p + 0x9A and p + 0x9B byte
   stores and records the order csrc/ovl_dng_b08.c uses -- the word store
   after BOTH byte stores -- at 64 words / 2 differing.  Here the b08 order is
   what lands and b42's kept order is 75 words / 2 aligned differences: the
   same number in the same two transposed stores, with the sign flipped.
   This is a STATEMENT-ORDER fact, the class the taxonomy says follows
   neither the frame nor the shape, and nothing about the twin predicted it.

   | spelling                                        | words | aligned |
   | as written below                                |   75  |    0    |
   | 0x8C store between 0x9A and 0x9B (b42's KEPT)   |   75  |    2    |
   | the two `(s16)' casts on func_800A2B5C dropped  |   73  |    5    |

   TWO MEASURED FREES.  The `&&' refusal gate written as nested `if's is
   75 / 0 -- fold_truthop merges tests on ONE local and these are two
   different symbols, so there is nothing to prevent -- and dropping the `t'
   local to write D_80175F30 inline at both sites is also 75 / 0.

   WHAT IS THIS COPY'S OWN, over b42: it RETURNS s32, -1 at each refusal and
   0 at the end (b42 is void); it has the extra
   `(d + 0x46) & 0x8000 == 0 && (D_80083462 & 8) != 0' refusal, which is a
   second test of the SAME D_80083462 halfword that cse unifies with the
   0x2000 one; the state is 0x18 where b42's is 0x17; and the table is the
   bank-local D_80175F30. */
s32 func_b44_801723D8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return -1;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 8) != 0)
    {
        return -1;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    p[0x9A] = 0x18;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    t = D_80175F30;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    d[0x6D] -= 1;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80172504);

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
s32 func_b44_801727D8(u8 *a, void *b, void *c, s32 d)
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
        func_b44_801718F8(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b44_801718F8(p, b, c, p);
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
   installed handler is the bank-local func_b44_80170E94. 31 words,
   0 differing, first compile.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x5.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b44_8017291C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 5);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b44_80170E94;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's fall-and-land state machine, the p[0x9B] three-case
   fallthrough: state 0 arms the +0xA4 countdown, state 1 interpolates
   b->unk_0C/unk_10 as `(num << 16) / step' and accumulates the
   `-func_800644B8(step * 292) << 9' drift, state 2 waits on d + 0x1C's
   0x8000000 and re-seats the slot through func_800A2B04.  Then the shared
   +0x96 teardown: func_800A2B04 / func_800AD594 / func_800A4ACC, one off
   D_80083460 + 0x8, the three-way d + 0x1C gate and func_800AD9B4 installing
   the bank-local func_b44_80170E94.  No rodata of its own.
   195 words, 0 differing, FIRST COMPILE.

   The twin is the landed func_b22_8017406C (csrc/ovl_dng_b22.c, 189 words,
   dngtwin 0.911); its own twin chain is func_b27_80173210 and the resident
   original is func_800D991C (csrc/ovl_dungeon_800D50FC.c), which uses p+0x96
   as the counter and p+0xA4 as the accumulator and is NOT the body to copy.

   ALL FIVE OF b22'S NAMED FACTS HOLD, AND ALL FIVE REPRODUCE ITS FIGURES TO
   THE DIGIT.  Every row re-measured on THIS body with aligndiff, one change
   at a time:

   | spelling                                        | words | aligned |
   | as written below                                |  195  |    0    |
   | tail gate's 0x2000 arm as the `else'            |  195  |   20    |
   | the 0x410 and 0x20000 tests folded into one &&  |  195  |   10    |
   | the first numerator as one expression           |  195  |    1    |
   | the second numerator split like the first       |  196  |   12    |
   | no `(s16)' on the func_800AD9B4 result          |  194  |    2    |
   | `g' dropped, &D_80083460 inline at both sites   |  194  |    5    |
   | state 1's extra d + 0x1C clear REMOVED          |  188  |   46    |
   | the drift shifted 10, which is b22's own        |  195  |    1    |
   | state 2 ending `p[0x9B] = 3'                    |  193  |    4    |

   TWO MEASURED FREES, so neither is why this lands: the countdown written as
   a separate store and then a test -- `*(s16 *)(p + 0xA4) -= 1;' followed by
   `if (*(s16 *)(p + 0xA4) < 0)' -- is 195 / 0, and b22's
   `*(u32 *)(p + 0x8C) = (u32)f' spelling of the handler store is 195 / 0
   against the `*(void **)' this unit's func_b44_8017291C already uses.

   THIS COPY'S OWN CONSTANTS AND ITS ONE EXTRA STATEMENT.  The counter is
   p + 0xA4 where b22 uses p + 0x9E, state 0 seeds 7 where b22 seeds 5, the
   drift shifts 9 where b22 shifts 10, the sine argument is `step * 292'
   where b22's is `* 409', and func_800AD594 is handed 5 where b22 hands 4.
   The real shape difference is one statement: state 1 clears d + 0x1C's
   0x8000000 bit AGAIN after the accumulate, which b22 does not, and that is
   5 of the 6 words between the two bodies -- removing it is 188 / 46. */
void func_b44_80172998(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0xA4) = 7;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0xA4) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0xA4);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0xA4);
            *(s32 *)(p + 0xA0) = -func_800644B8(*(s16 *)(p + 0xA4) * 292) << 9;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        if ((*(s16 *)(p + 0xA4) -= 1) < 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            p[0x9B] += 1;
        }
    case 2:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            p[0x9B] += 1;
        }
        break;
    }

    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_800AD594(d, 5);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
    }
    else if ((v & 0x410) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(void **)(p + 0x8C) = func_b44_80170E94;
        func_800A9A04(d);
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
void func_b44_80172CA4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80172D08);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_8017346C);

void func_b44_801735B4(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_801735BC);

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0,
   state 1 waits on func_8003F270, state 2 fires one 0x312 spark through
   func_b44_80174BC8 and falls through into state 3, which runs the +0x96
   countdown and the bank-local D_80175F10 facing table and then re-installs
   func_b44_80170E94.

   The relinked twin of the landed func_b28_80172A58 (csrc/ovl_dng_b28.c),
   transcribed from it with this bank's symbols -- D_80175F10 for D_80174520,
   func_b44_80170E94 for func_b28_80170E5C -- and ONE WHOLE EXTRA STATE.
   309 words, 0 differing, on the first compile.

   THIS COPY HAS FOUR STATES WHERE b28 HAS THREE. The new state 2 is the same
   `c[0x4] == N && +0x14 & 0x1000, or +0x14 & 0xE000' gate as b28's state 2,
   but with N == 3, and all it does is call func_b44_80174BC8(p, b, c) and
   bump +0x9B -- then FALL THROUGH into b28's state 2, which is state 3 here
   with N == 4. That fall-through is what makes the two gates share their
   0xE000 arm in the emitted code.

   THIS FUNCTION OWNS jtbl_b44_80170868, the 7-entry table at rodata offset
   0x60 for state 0's FIRST switch -- cases 1,2,3,5,6,7 over the 0x3FFF field
   with case 4 falling to `default'. Verified against the compiler's own
   output: `.rdata' / `.align 3' / 7 words, in the order g = d+0x8,
   g = d+0xB, g = d+0xE, default, then the three k=1 arms jumping into the
   first three. It is the only compiler-emitted rodata in this unit.

   The SECOND switch -- the (d + 0x1C) & 0x2000 `else' -- has three cases and
   compiles to a compare chain. ITS ARMS MUST BE WRITTEN 3, 2, 1: ascending
   is 311 words and 12 aligned differences, because cross-jumping merges the
   table switch's 1/2/3 arms into these three and their emission order is
   what the table's `j's point at.

   b28's other two named facts hold here and were re-measured.

   THE TWO SIGN FIXES ARE `__builtin_abs', NOT `if (x < 0) x = -x;'. As the
   two ifs it is 308 words and 16 aligned differences: the
   `sw $v0, 0x60($s1)' that stores the func_800A05A4 result loses its word to
   the first `bgez's delay slot and x then takes $a0 where the target has
   $v0.

   `k' MUST BE `s16': `s32' is 308 words and 9 aligned differences, the
   missing word being the target's `addu $v0, $s5, $zero' ahead of the
   `beqz'.

   Measured and FREE at 309 / 0, so neither is why this lands: the
   D_8006DE24 record read inline as `D_8006DE24[g[0]].pad_11[1] == 2' with no
   tbl/i/e locals (b28 reports the same freedom), and both +0x8C stores
   spelled `*(u32 *)(p + 0x8C) = (u32)func_b44_80170E94'. */
void func_b44_801738B4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    u8 *f;
    s16 k;
    s32 i;
    s32 x;
    s32 y;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                m = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)m;
                q = *(u8 **)(m - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                tbl = D_8006DE24;
                i = g[0];
                e = (u8 *)&tbl[i];
                if (e[0x12] == 2)
                {
                    m = *(u8 **)(d + 0x60);
                    if (m != NULL)
                    {
                        q = *(u8 **)(m - 0x14);
                        d[0x72] = q[0x24];
                        d[0x73] = q[0x25];
                    }
                }
                else
                {
                    *(s32 *)(d + 0x60) = func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            if (func_800A94A0(d, g, k, p + 0x98) != 0)
            {
                func_800BB044(d);
                p[0x9B] += 1;
            }
        }
        else
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(void **)(p + 0x8C) = func_b44_80170E94;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B] += 1;
    case 2:
        if ((*(s8 *)(c + 0x4) == 3 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_b44_80174BC8(p, b, c);
            p[0x9B] += 1;
        }
    case 3:
        if ((*(s8 *)(c + 0x4) == 4 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 1;
            *(u16 *)(p + 0x98) |= 0x80;
        }
        *(s16 *)(p + 0x96) -= 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            *(s16 *)(p + 0x96) = 0;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            f = D_80175F10;
            if (*(u8 **)(c + 0x2C) != f)
            {
                *(u8 **)(c + 0x2C) = f;
                func_80047784(c, f[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            g2 = (u8 *)&D_80083460;
            if (*(u32 *)(g2 + 0xC) == 0)
            {
                *(u16 *)(g2 + 0xA) -= 1;
                *(void **)(p + 0x8C) = func_b44_80170E94;
                func_800A4ACC(d);
                d[0x73] = 0;
                d[0x72] = 0;
                d[0x6D] -= 1;
                *(u16 *)(d + 0x46) &= 0x7FFF;
                func_800A56E0(0xB4);
            }
        }
        break;
    }
}

/* The banked room script's three-state glide, state index at p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it a quarter per frame and runs the +0x96 countdown, state 2
   eases onto the tile centre and installs func_b44_80170E94. The landed
   func_b19_80173674 (csrc/ovl_dng_b19.c) is the same body and carries the
   measured spellings this one reuses -- the three-statement first numerator,
   the two-statement second one, `if (x > 0) x--;` in state 1 against
   `if (--x > 0)` in state 2. xsibs named func_800B2CA0 at 0.841; b19's copy
   is the closer twin. 188 words, 0 differing, first compile.

   Four deltas, all read off this listing rather than assumed, and each
   measured against b19's spelling: the seeds are POSITIVE and scaled by 19
   (`sll 19`, no `negu`) where b19 negates and scales by 15, which here is
   190 words and 149 differing; state 2's two eases scale by 16, not 15,
   worth 2 differing; state 2's gate is `> 0` (`blez`) and not `!= 0`, worth
   1 differing, because the `lh` the test leaves in $a0 is the first divisor;
   and state 2 has an arm b19 has not -- `(c[0x14] & 0x6000) && d[0x28] == 0`
   routes into the shared `stop:` label, so the func_800AAA54 teardown has
   THREE reaching edges here, not two. */
void func_b44_80173D88(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 t;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 19;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 19;
        p[0x9B] += 1;
        if (d[0x28] == 0)
        {
            goto stop;
        }
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 2;
            return;
        }
        t = -1;
        if ((*(s32 *)(d + 0x1C) & 0x228) != 0)
        {
            t = 8;
        }
        *(s16 *)(p + 0x96) = t;
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
            goto stop;
        }
        *(s16 *)(p + 0x96) = 8;
        p[0x9B] += 1;
        return;
    case 2:
        if (*(s16 *)(p + 0x96) > 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0 && d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80175F58);
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b44_80174078(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b44_80174098(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b44_801740B8(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b44_801740D8(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

/* The banked room script's TWO-state tail, state index at p + 0x9B: state 0
   re-dresses the renderable from the bank-local D_80175F28 facing table and
   steps the state, state 1 is the long refusal chain, and the shared
   D_80175F10 re-dress hands the actor back to func_b44_80170E94. The landed
   func_b03_80173358 (csrc/ovl_dng_b03.c) is the same machine with a third
   state, and the resident func_800DA014 is the original; read b03's comment
   for the chain's measured spellings, all of which hold here.

   THE RE-DRESS TAIL IS WRITTEN ONCE AND REACHED BY `goto`, WHICH IS WHAT THE
   TARGET CROSS-JUMPS TO, and unlike b03's copy it does not have to be
   duplicated: b03 needed two copies to buy `p` an allocno_compare reference,
   and here BOTH spellings measure 202 words / 0 differing, so the single copy
   is kept because it is what the listing shows. 202 words, 0 differing,
   first compile.

   Two other deltas from b03: there is no 0x8000 gate in front of the tail,
   and the tail clears d + 0x1C's 0x200 instead of bumping D_80083460 + 0xA.
   The 0xA decrement is in state 0 only, and it runs AFTER the dress. */
void func_b44_80174110(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80175F28;
        func_80047784(c, D_80175F28[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            goto tail;
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
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C(p, b, c, d);
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
            func_b44_80174698(p, (u8 *)b, c, d);
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
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
    tail:
        *(u8 **)(c + 0x2C) = D_80175F10;
        func_80047784(c, D_80175F10[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        return;
    }
}

/* The banked room script's other two-state tail, state index at p + 0x9B.
   The landed func_b03_801736F4 (csrc/ovl_dng_b03.c) is the same machine with
   a third state; its comment carries the spellings that hold here unchanged
   -- the facing table named DIRECTLY at both dress sites rather than through
   a `u8 *` local, `*(s8 *)(d + 0x6D)` for the `lb`, `*(s16 *)(d + 0x64)` for
   the `lh`, and the `(s16)` cast on the first func_800A2C34 result.

   THE D_80175F10 DRESS TAIL MUST BE WRITTEN OUT TWICE EVEN THOUGH THE TARGET
   CROSS-JUMPS IT INTO ONE. State 1 opens with the same d[0x25] test that
   closes it, and spelling the opening arm as `goto` to the single copy is
   152 words with 15 ALIGNED DIFFERENCES, every one of them `p` and `b`
   exchanged into $s2/$s3 -- this body is two references short of what
   allocno_compare needs to give `p` the lower number, and the duplicated
   `sw $v0, 0x8C(p)` supplies them. jump2 merges the copies back, so the
   duplication costs no word. Rejected with its score: a
   `do { func_800AA258(p, b, c, d); } while (0)` free-reference bump also
   measures 152 / 0, and so do the same wrapper on the func_800AA888 and
   func_b44_80174698 calls, but the duplicated tail is plain C that the twin
   already uses, so no device is recorded here. `do { }` on p[0x9B] += 1,
   on the func_800A9A0C / func_800A9A04 calls or on the p + 0x8C store is
   15; on `h = (u8 *)&D_80083460` 26; on either `c + 0x2C` store 136 and a
   word long.

   Three deltas from b03: the 0x1000 gate reads the D_80083462 SYMBOL
   directly, not `*(u16 *)(g + 0x2)` off `&D_80083460`; the tail installs
   p + 0x8C before clearing d + 0x1C's 0x200; and state 0 decrements
   D_80083460 + 0xA before the dress, not after. 152 words, 0 differing. */
void func_b44_80174438(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *h;
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
        *(u8 **)(c + 0x2C) = D_80175F28;
        func_80047784(c, D_80175F28[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80175F10;
            func_80047784(c, D_80175F10[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
            *(s32 *)(d + 0x1C) &= ~0x200;
            return;
        }
        if ((D_80083462 & 0x1000) != 0)
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
            func_b44_80174698(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175F10;
        func_80047784(c, D_80175F10[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        *(s32 *)(d + 0x1C) &= ~0x200;
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
   call emits the bare `jal` either way); hoisting D_80175F18 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b44_80174698(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80175F28 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80175F18;
        func_80047784(c, D_80175F18[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b44_80174760(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b44_80170E94;
        }
    }
}

/* 14 words, 0 differing. */
void func_b44_801747C4(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

/* 14 words, 0 differing. */
void func_b44_801747FC(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

/* 14 words, 0 differing. */
void func_b44_80174834(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

/* 14 words, 0 differing. */
void func_b44_8017486C(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

/* 14 words, 0 differing. */
void func_b44_801748A4(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b44_80170E94;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_801748DC);

/* 27 words, 0 differing. */
/* The 0x312 spark's own step function: hand the renderable to func_800478B8,
   and when its +0x14 reports 0x6000 mark the owning actor retired and set the
   same bit in D_800814A0. No xsibs twin at ratio 0.40; written from the
   listing. D_800814A0 is the PLAIN scalar here, not ovl_main_8008A288's
   `D_800814A0_a[0]` array alias -- the `lui`/`lw %lo` pair is what the
   non-array spelling emits. 27 words, 0 differing, first compile. */
void func_b44_80174A00(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_b44_80174BC8's twin, two constants apart: the func_80047784 sprite is
   0x39 rather than 0x37 and the +0x1E / +0x1C scale is 0x2000 rather than
   0x1000. Everything else -- the unused first parameter, the success path
   inside the `if` with `return NULL` after it, the descending out[] zeroing
   -- is that function's comment, and it landed here with no change.
   87 words, 0 differing. */
UnkNode *func_b44_80174A6C(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    s16 out[4];

    n = func_8003FC64(0x312);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b44_80174A00;
        func_8004491C(n, (u32)D_80045340);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x2000;
        *(s16 *)(r + 0x1C) = 0x2000;
        *(s16 *)(r + 0x6) = 1;
        *(u16 *)(r + 0x14) |= 0xC;
        *(u16 *)(r + 0x10) |= 0x20;
        func_80047784(r, 0x39, 0);
        m = n->unk_08;
        *m = *b;
        out[2] = 0;
        out[1] = 0;
        out[0] = 0;
        if (func_8003DE58(*(u8 **)(c + 0x8), (UnkSlot *)c, out, 0) != NULL)
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + out[2];
        }
        return n;
    }
    return NULL;
}

/* Spawn one 0x312 node for the 0x37 sprite, give it func_b44_80174A00 as its
   step, copy the caller's vector into it and offset that by the slot
   projection func_8003DE58 computes. The first parameter is UNUSED -- $a0 is
   overwritten with 0x312 before anything reads it, and func_b44_801738B4
   passes three arguments. No xsibs twin; found through the callee set
   (func_8003FC64 / func_8004491C / func_8003DE58), which is the resident
   func_800D58D8's spawn block in csrc/ovl_dungeon_800D50FC.c.

   THE SUCCESS PATH MUST BE INSIDE THE `if`, WITH `return NULL` AFTER IT.
   The early-return spelling `if (n == NULL) { return NULL; }` is 87 words
   with 69 differing: it emits `bnez`/`j`/`move $v0, $zero` where the target
   branches once with `beqz $s1`, and everything after is shifted two words.
   `if (n != NULL) { ...; return n; } else { return NULL; }` also measures 0.
   The out[] zeroing runs DESCENDING, out[2] then out[1] then out[0];
   ascending is 2 differing. 87 words, 0 differing. */
UnkNode *func_b44_80174BC8(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    s16 out[4];

    n = func_8003FC64(0x312);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b44_80174A00;
        func_8004491C(n, (u32)D_80045340);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x6) = 1;
        *(u16 *)(r + 0x14) |= 0xC;
        *(u16 *)(r + 0x10) |= 0x20;
        func_80047784(r, 0x37, 0);
        m = n->unk_08;
        *m = *b;
        out[2] = 0;
        out[1] = 0;
        out[0] = 0;
        if (func_8003DE58(*(u8 **)(c + 0x8), (UnkSlot *)c, out, 0) != NULL)
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + out[2];
        }
        return n;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80174D24);

/* Spawn one 0x312 node for the 0x38 sprite, give it func_b44_80174D24 as its
   step, stash the caller's b and c in the record's +0x24 / +0x28, copy the
   caller's vector into the node and offset that by the slot projection
   func_8003DE58 computes.

   func_b44_80174BC8's twin two listings up, four deltas apart, and it landed
   on the first compile. 89 words, 0 differing.

   THE ALLOCATOR IS THE TWO-ARGUMENT func_8003FD64(0x312, p - 0x20), SO THE
   FIRST PARAMETER IS LIVE HERE. In func_b44_80174BC8 and func_b44_80174A6C
   $a0 is overwritten with 0x312 before anything reads it and `p' is dead;
   here `p - 0x20' is the allocator's second argument, which is why this one
   allocates off the caller's record rather than D_80083498.

   THE +0x24 / +0x28 STORES GO THROUGH A `w = (u8 *)n + 0x20' BASE. Written
   straight off `n' as `*(u32 *)((u8 *)n + 0x24)' the pair costs the target's
   `addiu $v0, $s1, 0x20' and the body is 88 words with 71 differing. It is
   the same +0x20 record base func_b44_801708BC names `p'.

   Also: the sprite is 0x38, the +0x1E / +0x1C scale is 0x1000 as in BC8, and
   BC8's `*(s16 *)(r + 0x6) = 1;' is ABSENT.

   BC8's two named facts, re-measured here: the success path inside the `if'
   with `return NULL' after it is load-bearing -- the early-return spelling
   `if (n == NULL) { return NULL; }' is 89 words with 70 differing. But the
   DESCENDING out[] ZEROING IS FREE ON THIS BODY: ascending out[0], out[1],
   out[2] also measures 89 / 0, where BC8's comment records it as 2 differing
   on ITS body. Do not carry that one forward as a rule. */
UnkNode *func_b44_80174EB4(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *w;
    s16 out[4];

    n = func_8003FD64(0x312, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b44_80174D24;
        func_8004491C(n, (u32)D_80045340);
        w = (u8 *)n + 0x20;
        *(u32 *)(w + 0x4) = (u32)b;
        *(u32 *)(w + 0x8) = (u32)c;
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x28) = *(s32 *)(c + 0x28);
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u16 *)(r + 0x14) |= 0xC;
        *(u16 *)(r + 0x10) |= 0x20;
        func_80047784(r, 0x38, 0);
        m = n->unk_08;
        *m = *b;
        out[2] = 0;
        out[1] = 0;
        out[0] = 0;
        if (func_8003DE58(*(u8 **)(c + 0x8), (UnkSlot *)c, out, 0) != NULL)
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + out[2];
        }
        return n;
    }
    return NULL;
}

/* The banked room script's five-state camera mover, driven by the s16 at
   p + 0x0 and clocked by p + 0x2.  The head copies the record at
   *(p + 0x44) into a local UnkVec, asks func_8003DE58 for a three-halfword
   shake offset (unless p + 0x48's slot has any of 0x9FFF set), and folds it
   into the copy's three integer halves, remembering it at p + 0xA/0xC/0xE for
   the frames where the slot refuses.  State 0 seeds the target from either
   the D_8006CCD8/D_8006CCE8 facing pair (kind 1) or the leader's position
   less D_800DDC40's footprint (kind 2) and derives the glide length in
   p + 0x4; state 1 halves the velocity and lands; state 2 jitters the two
   angles by `rand() % 0x800' and orbits; state 3 doubles the velocity and,
   when it expires, clears the slot's 0x800 and FALLS THROUGH into state 4,
   which refuses unless the slot still carries the bank-local D_80175F68
   table.  The tail pushes through func_800C77D0 whenever the state is not 0.
   545 words, 0 differing.

   NO TWIN ANYWHERE, and all three searches were run: dngtwin reports no
   carved sibling at 0.70 or better, xsibs finds no landed body in the whole
   project at 0.70 (its only hit in this unit is func_b44_80172998's), and the
   callee-set intersection over func_800478B8 / func_8003DE58 / func_80064584 /
   func_8009C12C / D_800DDC40 names no single landed file.  Written off the
   listing.

   OWNS jtbl_b44_80170888 (rodata +0x80, .align 3) -- AND IT HAS FIVE CASES,
   NOT SIX.  The listing's sixth `.word' is literally 0x00000000, not a label:
   five entries are 20 bytes, which is 4 mod 8, and the NEXT rodata object is
   func_b44_80175A90's jtbl_b44_801708A0 at 0x801708A0, whose own `.align 3'
   forces exactly one pad word at 0x8017089C.  So this is the ordinary
   odd-count pad, and an agent that writes the zero as a sixth case gets a
   phantom arm.  Entries in order:
     +0x00  .Lb44_80175174   case 0
     +0x04  .Lb44_8017544C   case 1
     +0x08  .Lb44_80175574   case 2
     +0x0C  .Lb44_80175720   case 3
     +0x10  .Lb44_801757F0   case 4 (also state 3's fallthrough target)
     +0x14  0x00000000       alignment, NOT a case
   The arms are written ASCENDING below because the entry addresses ascend,
   and that was confirmed the only way it can be -- `cdkgcc -S' on this copy
   emits the table as five `.word's in the order $L351, $L368, $L372, $L377,
   $L380, which are the five blocks in text order.  ovlverify, aligndiff and
   relcand are all blind to arm order on a table owner, so their 0 is not
   evidence about it.

   THE INNER SWITCH NEEDS AN EMPTY `case 0:'.  State 0 switches on the kind
   byte at *(p + 0x40) + 0xAE with real arms for 1 and 2, and the target's
   decision tree is `beq 1' / `slti 2 -> default' / `beq 2' / `j default' --
   three tests, which is the balanced tree for the case set {0, 1, 2}, not
   {1, 2}.  Without the empty arm the body is 543 words / 4 aligned
   differences and the `slti'/`bnez' pair is simply absent.

   THE FOUR `lh' LOADS IN STATE 0'S KIND-1 ARM ARE A `* 64' / `* 32', NOT A
   SHIFT.  `(x << 6) + ((y + 1) << 5)' assigned to an `s16' lvalue lets
   convert.c pass the truncation down through the PLUS and then through each
   LSHIFT, and the loads come out `lhu': 545 words / 4 aligned differences,
   all four of them the one opcode.  Written as `x * 64' and `(y + 1) * 32'
   the truncation stops at the MULT and the loads stay `lh'.  convert.c's
   LSHIFT_EXPR arm is guarded on `TREE_UNSIGNED (type)' and MULT_EXPR's is
   not, which is why the two spellings of the same value differ at all --
   docs/matching-levers.md's "the plain-constant multiply rule is about
   NON-powers of two" is about a plain `int' context and does not cover a
   narrowing one.

   Every other spelling measured on this body, one change at a time:

   | spelling                                        | words | aligned |
   | as written below                                |  545  |    0    |
   | the two seeds as `<< 6' / `<< 5'                |  545  |    4    |
   | no empty `case 0:' in the inner switch          |  543  |    4    |
   | the loop test as `p + 0x4 < sp28[i]'            |  545  |    2    |
   | sp28 zeroed ascending                           |  545  |    4    |
   | the six p + 0x28..0x3C zeros ascending          |  545  |    6    |
   | `if (x < 0) x = -x;' for __builtin_abs          |  552  |   41    |
   | *(p + 0x40) hoisted into one local for state 0  |  544  |   15    |
   | p + 0x48's `q' local used at states 3 and 4 too |  544  |   11    |
   | state 1's `= 3' / `+= 1' arms exchanged         |  545  |    7    |
   | the UnkVec copy written field by field          |  560  |   29    |
   | the `>> 5' clamp as one test, no store-back     |  545  |    2    |
   | state 2's `>= 8' gate inverted with early call  |  552  |   26    |

   THE THREE STORE ORDERS ARE LOAD-BEARING AND ONE IS FREE.  sp28 must be
   zeroed 2, 1, 0 (ascending is 4) and state 0's six-word clear must be
   written 0x30, 0x2C, 0x28, 0x3C, 0x38, 0x34 (ascending is 6) -- both
   descending within their group, which is how the target emits them.  But
   state 0's tail is free: `*(s16 *)(p + 0x2) = 0' before or after
   `*(s16 *)(p + 0x0) += 1' is 545 / 0 either way.

   AND THE "ONE LOCAL PER DRESS SITE" QUESTION HAS A THIRD ANSWER HERE.
   p + 0x48 is read at three places.  The `q' local as written (head only,
   inline at states 3 and 4) is 0; `q' used at all three is 544 / 11; and NO
   local at all, the deref written inline at the head too, is ALSO 545 / 0.
   So the head is vacuous and the later sites reject the local -- compare
   this with the root dispatchers' sentence before paying for it. */
void func_b44_80175018(u8 *p, UnkVec *b, u8 *c)
{
    UnkVec v;
    s16 sp28[3];
    UnkSlot *q;
    s32 i;
    s32 k;
    u8 *h;

    *(s16 *)(p + 0x2) += 1;
    func_800478B8(c);
    v = **(UnkVec **)(p + 0x44);
    sp28[2] = 0;
    sp28[1] = 0;
    sp28[0] = 0;
    q = *(UnkSlot **)(p + 0x48);
    if ((q->unk_14 & 0x9FFF) == 0 && func_8003DE58((u8 *)q->unk_08, q, sp28, 3) != NULL)
    {
        *(s16 *)((u8 *)&v + 0x2) += sp28[0];
        *(s16 *)((u8 *)&v + 0x6) += sp28[1];
        *(s16 *)((u8 *)&v + 0xA) += sp28[2];
        *(s16 *)(p + 0xA) = sp28[0];
        *(s16 *)(p + 0xC) = sp28[1];
        *(s16 *)(p + 0xE) = sp28[2];
    }
    else
    {
        *(s16 *)((u8 *)&v + 0x2) += *(s16 *)(p + 0xA);
        *(s16 *)((u8 *)&v + 0x6) += *(s16 *)(p + 0xC);
        *(s16 *)((u8 *)&v + 0xA) += *(s16 *)(p + 0xE);
    }

    switch (*(s16 *)(p + 0x0))
    {
    case 0:
        *(s32 *)(p + 0x30) = 0;
        *(s32 *)(p + 0x2C) = 0;
        *(s32 *)(p + 0x28) = 0;
        *(s32 *)(p + 0x3C) = 0;
        *(s32 *)(p + 0x38) = 0;
        *(s32 *)(p + 0x34) = 0;
        switch ((*(u8 **)(p + 0x40))[0xAE])
        {
        case 0:
            break;
        case 1:
            k = (*(u16 *)(*(u8 **)(p + 0x4C) + 0x2A) >> 8) & 0xE;
            *(s16 *)(p + 0x2A) =
                (*(s16 *)(*(u8 **)(p + 0x40) + 0xA8) * 64) + ((*(s16 *)((s32)D_8006CCD8 + k) + 1) * 32);
            *(s16 *)(p + 0x2E) =
                (*(s16 *)(*(u8 **)(p + 0x40) + 0xAA) * 64) + ((*(s16 *)((s32)D_8006CCE8 + k) + 1) * 32);
            *(s16 *)(p + 0x32) = *(s16 *)((u8 *)&v + 0xA);
            sp28[0] = __builtin_abs(*(s16 *)(p + 0x2A) - *(s16 *)((u8 *)&v + 0x2));
            sp28[1] = __builtin_abs(*(s16 *)(p + 0x2E) - *(s16 *)((u8 *)&v + 0x6));
            *(s16 *)(p + 0x4) = sp28[0];
            if (*(s16 *)(p + 0x4) < sp28[1])
            {
                *(s16 *)(p + 0x4) = sp28[1];
            }
            *(s16 *)(p + 0x4) = *(s16 *)(p + 0x4) >> 5;
            if (*(s16 *)(p + 0x4) == 0)
            {
                *(s16 *)(p + 0x4) = 1;
            }
            b->unk_0C = *(s32 *)(p + 0x28) - v.unk_00;
            b->unk_10 = *(s32 *)(p + 0x2C) - v.unk_04;
            b->unk_14 = 0;
            break;
        case 2:
            h = *(u8 **)(*(u8 **)(*(u8 **)(p + 0x4C) + 0x60) - 0x18);
            *(s32 *)(p + 0x28) = *(s32 *)(h + 0x0);
            *(s32 *)(p + 0x2C) = *(s32 *)(h + 0x4);
            *(s32 *)(p + 0x30) =
                *(s32 *)(h + 0x8) - (D_800DDC40[(*(u8 **)(*(u8 **)(p + 0x4C) + 0x60))[0x13]] << 15);
            sp28[0] = __builtin_abs(*(s16 *)(p + 0x2A) - *(s16 *)((u8 *)&v + 0x2));
            sp28[1] = __builtin_abs(*(s16 *)(p + 0x2E) - *(s16 *)((u8 *)&v + 0x6));
            sp28[2] = __builtin_abs(*(s16 *)(p + 0x32) - *(s16 *)((u8 *)&v + 0xA));
            *(s16 *)(p + 0x4) = sp28[0];
            for (i = 1; i < 3; i++)
            {
                if (sp28[i] > *(s16 *)(p + 0x4))
                {
                    *(s16 *)(p + 0x4) = sp28[i];
                }
            }
            *(s16 *)(p + 0x4) = *(s16 *)(p + 0x4) >> 5;
            if (*(s16 *)(p + 0x4) == 0)
            {
                *(s16 *)(p + 0x4) = 1;
            }
            b->unk_0C = *(s32 *)(p + 0x28) - v.unk_00;
            b->unk_10 = *(s32 *)(p + 0x2C) - v.unk_04;
            b->unk_14 = *(s32 *)(p + 0x30) - v.unk_08;
            break;
        }
        b->unk_00 = v.unk_00;
        b->unk_04 = v.unk_04;
        b->unk_08 = v.unk_08;
        *(s16 *)(p + 0x2) = 0;
        *(s16 *)(p + 0x0) += 1;
        break;
    case 1:
        b->unk_0C /= 2;
        b->unk_10 /= 2;
        b->unk_14 /= 2;
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x2) >= *(s16 *)(p + 0x4))
        {
            b->unk_00 = *(s32 *)(p + 0x28);
            b->unk_04 = *(s32 *)(p + 0x2C);
            b->unk_08 = *(s32 *)(p + 0x30);
            *(s16 *)(p + 0x2) = 0;
            if ((*(u8 **)(p + 0x40))[0xAE] != 2)
            {
                b->unk_0C = (v.unk_00 - b->unk_00) >> (*(s16 *)(p + 0x4) + 1);
                b->unk_10 = (v.unk_04 - b->unk_04) >> (*(s16 *)(p + 0x4) + 1);
                b->unk_14 = (v.unk_08 - b->unk_08) >> (*(s16 *)(p + 0x4) + 1);
                *(s16 *)(p + 0x0) = 3;
            }
            else
            {
                *(s16 *)(p + 0x0) += 1;
            }
        }
        func_b44_80175A90(&v, b);
        break;
    case 2:
        *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 0x400 + rand() % 0x800;
        *(s16 *)(p + 0x8) = *(u16 *)(p + 0x8) + 0x400 + rand() % 0x800;
        b->unk_00 = *(s32 *)(p + 0x28) +
                    (((func_800644B8(*(s16 *)(p + 0x6)) >> 4) * (func_800644B8(*(s16 *)(p + 0x8)) >> 4)) << 5);
        b->unk_04 = *(s32 *)(p + 0x2C) +
                    (((func_800644B8(*(s16 *)(p + 0x6)) >> 4) * (func_80064584(*(s16 *)(p + 0x8)) >> 4)) << 5);
        b->unk_08 = *(s32 *)(p + 0x30) + ((func_80064584(*(s16 *)(p + 0x6)) >> 4) << 13);
        if (*(s16 *)(p + 0x2) >= 8)
        {
            if ((*(u8 **)(p + 0x40))[0xAE] == 2)
            {
                func_8009C12C(*(u8 **)(p + 0x4C), *(u8 **)(p + 0x48), *(s16 *)(*(u8 **)(p + 0x4C) + 0x2A),
                              *(s16 *)(*(u8 **)(p + 0x40) + 0xAC));
            }
            b->unk_0C = (v.unk_00 - b->unk_00) >> (*(s16 *)(p + 0x4) + 1);
            b->unk_10 = (v.unk_04 - b->unk_04) >> (*(s16 *)(p + 0x4) + 1);
            b->unk_14 = (v.unk_08 - b->unk_08) >> (*(s16 *)(p + 0x4) + 1);
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)(p + 0x0) += 1;
        }
        func_b44_80175A90(&v, b);
        break;
    case 3:
        if (*(s16 *)(p + 0x2) < *(s16 *)(p + 0x4))
        {
            b->unk_0C *= 2;
            b->unk_10 *= 2;
            b->unk_14 *= 2;
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
            b->unk_08 += b->unk_14;
            func_b44_80175A90(&v, b);
            break;
        }
        b->unk_00 = v.unk_00;
        b->unk_04 = v.unk_04;
        b->unk_08 = v.unk_08;
        if (((*(UnkSlot **)(p + 0x48))->unk_14 & 0x800) == 0)
        {
            break;
        }
        (*(UnkSlot **)(p + 0x48))->unk_14 &= ~0x800;
        *(s16 *)(p + 0x2) = 0;
        *(s16 *)(p + 0x0) += 1;
    case 4:
        if (*(u8 **)((u8 *)*(UnkSlot **)(p + 0x48) + 0x2C) != D_80175F68)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        (*(UnkSlot **)(p + 0x48))->unk_14 &= ~0x800;
        b->unk_00 = v.unk_00;
        b->unk_04 = v.unk_04;
        b->unk_08 = v.unk_08;
        break;
    }

    if (*(s16 *)(p + 0x0) != 0)
    {
        func_800C77D0(p - 0x20, (u8 *)b, 8, 0x300);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_8017589C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80175A90);

/* A signed jitter: twice the argument, plus `rand() % 9', with its sign
   flipped on a coin toss.  `rand' is func_80069EF8 out of
   splat/common_syms.txt and needs no declaration.  No twin and none needed.
   33 words, 0 differing, FIRST COMPILE.

   `r' MUST BE `s16'.  As `s32' the body is 30 words / 13 aligned
   differences: the two `sll 16' copies that feed the shared `sra 16' at the
   return disappear, because the truncation then happens only once, at the
   return type.

   THE ADDENDS ARE IN THE TARGET'S ORDER.  `a * 2 + rand() % 9' is
   33 words / 3 aligned differences -- the `sll'/`sra' pair that widens the
   parameter moves ahead of the call instead of filling the slots after it.

   `a * 2' AND NOT `a << 1', which is 32 words / 2.  The target's
   `sll $s0, 16' / `sra $s0, 15' is one combined sign-extend-and-double, and
   MULT_EXPR is what folds into it; the LSHIFT keeps the `sra 16' separate.
   This is the narrowing-context exception to
   docs/matching-levers.md's "`i * 0x400' and `i << 10' are the same tree".

   TWO MEASURED FREES, so neither is why this lands: `s32 a' with an `(s16)'
   cast written at the one use is 33 / 0 (the parameter is copied ONCE in the
   prologue, which is the documented precondition for a sub-word parameter
   type being free), and giving the negation its own second local instead of
   `r = -r' is also 33 / 0 -- the target does hold the two values in $s1 and
   $s0, but reload picks that up from `r = -r' by itself.

   The parity gate inverted, returning early, is 31 words / 16. */
s16 func_b44_80175E50(s16 a)
{
    s16 r;

    r = rand() % 9 + a * 2;
    if ((rand() & 1) != 0)
    {
        r = -r;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b44", func_b44_80175ED4);
