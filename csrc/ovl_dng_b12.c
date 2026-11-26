#include "common.h"

s32 rand(void);

extern u8 D_80175AA4[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
u16 func_800A6D30();
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);

extern u32 D_800814A8;
extern u8 D_80175A94[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
s32 func_8009A180(u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b12_80173D64(u8 *p, u8 *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);

extern u8 D_8006CCF8[];
extern void (*D_80175AAC[])(u8 *, UnkVec *, u8 *, u8 *);
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
s32 func_b12_80171DD8(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80175A54[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_b12_80175A4C[];
extern u8 D_80175A9C[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b12_8017162C(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b12_80170E70(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AB538();

void func_b12_80171D74(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB464();

s32 func_800AC7EC();

extern u8 D_80175A64[];

UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
void func_b12_80170A44(u8 *p, UnkVec *b, u8 *c);

/* The banked room script's SPAWNER for the state-9 actor: allocate a 0x112
   node off D_80083498, install func_b12_80170A44 as the node's +0x10 driver,
   dress the slot from the bank-local D_80175A54 facing table, pick the
   +0x14/+0x1C flag pair from the low two bits of the caller's `a', and -- only
   for a==0 -- give one actor in two a random 0x20|rand palette out of
   func_800A48F0 and the OTHER bank-local table D_80175A9C. Then hand the
   node to func_800A9C18 and arm the +0x8C continuation func_b12_80170E70.

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

   func_b12_80170A44 and func_b12_80170E70 are address-taken here and are
   still INCLUDE_ASM stubs below, so both need a declaration above.
   THIS COPY'S OWN CONSTANT: 0x9 -> 0xc.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
extern s16 D_800DCF5E;

extern s16 D_80175B20;
s32 func_800453E0(u8 *, UnkVec *, u8 *, s16);

s32 func_800AB1C0();
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);

void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);

extern s32 D_8008346C;
s32 func_8003F270(void);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
void func_800DA840(SVECTOR *src, s16 a);
s32 func_800A56E0(s32);

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
void func_b12_80174618(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

void func_b12_8017162C(u8 *p, void *b, void *c, u8 *d);
extern u8 D_80175A5C[];
extern u8 D_80175A8C[];
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b12_801713D4(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b12_80171F9C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b12_801720B4(u8 *a, void *b, void *c, s32 d);
void func_b12_8017526C(u8 *p, void *b, void *c, u8 *d);

void func_80044A50(UnkNode *);
extern s32 D_80175B28;

extern Unk41284 D_80175B0C[];
extern s32 D_80175B14[];
extern u8 D_80175B24[];
extern u8 D_80175B25[];
extern u8 *D_800DCEEC[];
extern u8 D_8014A000[];
extern u8 D_800DCF5C;
extern s32 D_8006CD58;
extern u8 D_8003E140[];
void func_80041588(Unk41284 *, s8 *, s32);
u32 func_800445E0(void);
s32 func_800C77D0(void *, u8 *, s16, s32);
s32 func_800498A0(u8 *);
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
UnkNode *func_800ADC4C(u8 *, s32, s16, u8 *);
void func_800A18E8(s16, s16);
void func_8009A028(u8 *);
s32 func_800A504C(u8 *, u8 *);
void func_b12_80175180(u8 *p, UnkVec *b, u8 *c);
extern u8 D_80175A6C[];
extern u8 D_80175A74[];
void func_80067F20(u32 *, s32, s32, s32, s32);
void func_b12_801749EC(u8 *p, UnkVec *b, u8 *c);

u8 *func_b12_8017087C(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b12_80170A44;
        p[0x13] = 12;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80175A54;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_80175A9C;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b12_80170E70;
        func_800AA36C(w, m, r, p);
    }
    return p;
}

/* The banked room script's per-frame actor driver, the relinked twin of the
   resident overlay's func_800D8728 (csrc/ovl_dungeon_800D50FC.c): past the
   engine's 0x2000 gate it runs func_800A9E70, then the record's own +0x8C
   continuation, then the +0x9A entry of the D_80175AAC table, re-aims the
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
void func_b12_80170A44(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b12_80170E70)
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
    D_80175AAC[p[0x9A]](p, b, c, p);
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

/* The +0x8C handler at the head of this bank's actor machine -- the relinked
   twin of the resident overlay's func_800D8C64
   (csrc/ovl_dungeon_800D50FC.c). Four early exits on the D_80083462 flag word
   and d+0x1C's bits, the +0x9A state byte set to 0xE, the facing re-dress from
   the bank-local byte tables, and then the walk. 345 words, 0 differing, first
   compile. 3 copies (dngfam.py).

   IT IS THE LANDED func_b05_80170E84 (csrc/ovl_dng_b05.c, 338 words) PLUS ONE
   ARM, and b05's three load-bearing spellings were all that was needed:
     * the walk tail func_b12_8017162C is WRITTEN OUT AT ALL THREE SITES, not
       goto-ed -- jump2 folds the three copies back to one block but flow has
       already counted their references, which is what gives `c' $s0;
     * ONE table local per dress site (`e'/`e2'/`e3'), not one shared;
     * the case 5/6/7 arm's D_800814A8 is a POINTER local `w' assigned BEFORE
       the d+0x2A store, so the `lw's load delay gets filled by that `sh'.
   Those three facts are measured on b05's body, not re-measured here: this
   body was 0 differing on its first compile with them in place.

   THE RELINK DELTA FROM b05, read off the listing:
     * b05's five tables become D_80175A9C (the d[0x25]==0 and 0x200 arms'
       comparand), D_80175A94 (what those two arms store and pass),
       D_80175A54 (the state-0xE dress and the final dress),
       D_80175A5C (func_800AA6B4) and D_80175A8C (func_800AAF00);
     * func_b05_801713CC / func_b05_80171624 / func_b05_80171DD0 /
       func_b05_80171F94 / func_b05_801720AC / func_b05_80173F28 become
       func_b12_801713D4 / func_b12_8017162C / func_b12_80171DD8 /
       func_b12_80171F9C / func_b12_801720B4 / func_b12_80173D64;
     * THE WHOLE 345-vs-338 WORD DELTA IS ONE EXTRA NESTED ARM: b05 calls
       func_800AAB10 and discards the result, this one tests it and calls
       func_b12_8017526C on a nonzero -- 7 words.

   THIS FUNCTION OWNS jtbl_b12_80170808, the 12-entry table at 0x80170808 for
   the switch over (d + 0x46) & 0x3FFF. Entries: cases 1,2,3 -> the
   func_800AAF00 arm; 4, 10 and 11 -> `default' (the walk), so they need no
   label of their own; 5,6,7 -> the func_800A0818 / D_800814A8 arm; 8,9 -> the
   func_b12_80171DD8 arm; 12 -> func_800A9A0C, written as a `case' label inside
   the `w[0x9A] != 0x11' arm, which is also where cross-jumping merges it with
   the d+0x1C & 0x20 early exit.

   `--raw' is 80 differing and all 80 are relocations -- 23 `jal's, 9 `j's and
   48 `%hi'/`%lo' halves (b05's 79 plus this body's one extra `jal') -- so no
   masked word hides a constant address. */
void func_b12_80170E70(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b12_801713D4(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80175A9C)
        {
            e = D_80175A94;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80175A9C)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80175A94) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            e2 = D_80175A54;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80175A5C) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b12_80173D64(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0 && (s16)func_800AAB10(p, b, c, d) != 0)
        {
            func_b12_8017526C(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b12_8017162C(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b12_801720B4(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b12_8017162C(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b12_80171DD8(p, b, c, d) != 0)
            {
                return;
            }
            func_b12_80171F9C(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
            case 12:
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_800AAF00(p, b, c, (s32)D_80175A8C, func_b12_80170E70);
            return;
        default:
            func_b12_8017162C(p, b, c, d);
            return;
        }
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
        {
            if ((v & 0x430) == 0)
            {
                if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
                }
            }
        }
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    e3 = D_80175A54;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   landed func_b07_80171570 in csrc/ovl_dng_b07.c (itself the resident
   overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c): advance the actor
   one cell of its queued path, swap in the D_b12_80175A4C facing table,
   re-aim the sprite, and either stop at state 0xF or hand on to
   func_b12_80171D74.  150 words, 0 differing.  12 copies (dngfam.py).

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
   the three `j's, and the `%hi'/`%lo' pairs for D_b12_80175A4C, D_80083228
   and the two D_80083462 loads -- so no masked word is hiding a constant
   address.  Hoisting D_b12_80175A4C into a local and writing the 0x8A bump
   as `*(s16 *)(d + 0x8A) += 1' are each 0 differing too; the forms kept here
   are b07's, for the replicator's sake. */
void func_b12_801713D4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b12_80175A4C)
        {
            *(u8 **)(c + 0x2C) = D_b12_80175A4C;
            func_80047784(c, D_b12_80175A4C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
                func_b12_80171D74(p, b, c, d);
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
   AND PASSED ON. func_b12_80171DD8's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A66C 2 words, func_b12_80171DD8 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b12_8017162C(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b12_80171DD8(p, b, c, d) == 0)
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
   out of the bank-local byte table D_80175A64: the relinked twin of the
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
     the local assigned inside the store, `= t = D_80175A64'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80175A64 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b12_80171D74(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80175A64;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80175A54 facing table and re-aim through func_8009C93C.

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
s32 func_b12_80171DD8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80175A54;
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
   tests, put the actor into state 0x11, swap the D_80175A54 facing table
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
   D_80175A54 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b12_80171F9C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80175A54;
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
s32 func_b12_801720B4(u8 *a, void *b, void *c, s32 d)
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
        func_b12_8017162C(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b12_8017162C(p, b, c, p);
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

/* The two-gate handler installer of this unit's own landed func_b05_80172670,
   with func_800AD594(d, 4) and func_800A4ACC(d) run between the gates, a
   second store at +0x90, and a D_80083462 0x80 tail. 38 words, 0 differing
   on the first compile.

   The outer gate is called with no arguments at all -- no instruction sets up
   $a0..$a3 before it -- so func_800AB1C0 needs the empty-paren declaration
   this unit already uses for the same shape; its landed definition
   (csrc/ovl_dungeon_800A4B88.c:4277) takes four, and spelled that way the
   call site does not compile ("too few arguments"). The inner gate's result
   is read as a halfword, `sll $v0, 16` with no `sra` and then `blez`, which
   is `(s16)x > 0`.

   THE `blez` BRANCHES PAST THE D_80083462 TAIL, not back to the gate's join:
   its target is the epilogue, so the failing inner gate is an early `return`
   and not a nested `if` that falls into the tail. The second parameter is
   never mentioned. */
void func_b12_801721F8(u8 *p, void *b, u8 *c, u8 *d)
{
    if (func_800AB1C0() != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        *(s32 *)(p + 0x90) = 0;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = 0;
    }
}

/* The three-state "knocked back and falling" handler whose shared tail runs
   once the +0x96 countdown expires: state 0 dresses the slot from the
   bank-local byte table D_80175A6C, raises +0x98's bit 3, seeds the fall at
   0xFFF00000 and clears the +0xA0 accumulator; state 1 integrates the fall
   into +0x90 and drives the vector's +0xC/+0x10 towards the slot's +0x24/
   +0x25 cell; state 2 waits for d+0x1C's 0x8000000 and re-dresses from
   D_80175A74.  The tail zeroes the vector, func_800A2B04s it back to the
   cell, runs func_800AD594(d, 4) / func_800A4ACC(d), drops D_80083460's +0x8
   counter and re-installs func_b12_80170E70.  216 words, 0 differing, on the
   FIRST COMPILE.  1 member (dngfam.py).

   TWIN: the landed func_b43_80172404 (csrc/ovl_dng_b43.c, 217 words, ratio
   0.947; its own twin is the resident func_800D991C in
   csrc/ovl_dungeon_800D50FC.c).  BOTH of its stated spellings HOLD here, and
   both are re-priced because its own numbers are from a first compile too:

   (1) THE FIRST NUMERATOR MUST BE THREE STATEMENTS -- `u = c[0x24] << 6;
   w = *(s16 *)((u8 *)b + 0x2) - 0x20; u -= w;' -- or combine folds the
   subtract into the shift and the `lbu' sinks below the `lh'.  As one
   expression it is 216 words / 1 aligned difference.  The twin states this
   fact as the reason it compiled clean; here it is worth exactly ONE row, so
   it is TRUE BUT MUCH WEAKER THAN THE TWIN IMPLIES.

   (2) THE SECOND NUMERATOR MUST STAY TWO STATEMENTS, because there the `lh'
   has the `mflo' bubble to fall into.  Split the same three ways as the
   first, it is 217 words / 13 aligned differences.  This one is the real
   fact of the pair.

   THE ONE WORD THIS BODY IS SHORTER THAN THE TWIN IS STATE 1'S EXIT GATE,
   NOT A MISSING STATEMENT.  The twin's `if (*(s16 *)(p + 0x96) < 2)' is
   `slti'/`beqz'; this listing has a bare `bgez $a0', which is
   `if (*(s16 *)(p + 0x96) < 0)'.  Spelled the twin's way, 217 words / 7
   aligned differences -- so `dngtwin's "YOURS IS 1 WORDS SHORTER -- find the
   MISSING STATEMENTS first" is a false lead on this body: 216 = 217 - 1
   comparison instruction.

   THE TAIL'S THREE GATES ARE `if / else if / if' AND THE `else' IS LOAD-
   BEARING, as the twin says: it is what puts the `andi $v0, $v1, 0x410' in
   the `beqz' delay slot.  Written as three nested/sequential `if's it is 215
   words / 11 aligned differences, and folded to
   `(v & 0x410) == 0 && (v & 0x20000) != 0' it is 216 / 10 -- fold_truthop
   merges the two tests into one `(v & 0x22410) != 0x20000'.

   STATE 0'S FAILING GATE `break's OUT OF THE SWITCH to the countdown; it does
   not fall into state 1.  Written as a positive `if' wrapping the rest of the
   arm, 216 words / 1 aligned difference (the one row is the branch target).

   THE RELINK OFF THE TWIN: D_80173FD8 -> D_80175A6C, D_80173FE0 ->
   D_80175A74, the handler is func_b12_80170E70, the fall seed is 0xFFF00000
   (twin 0xFFE80000) and the per-frame step 0x40000 (twin 0x60000); the
   accumulator is +0xA0 at all four sites, as in the twin.

   MEASURED AND FREE, so NOT the reason this lands: the countdown spelled
   `(s16)(--*(u16 *)(p + 0x96)) > 0' instead of
   `(*(s16 *)(p + 0x96) -= 1) > 0' (216/0 either way).

   No `.section .rodata' block in the listing, so this function owns no jump
   table -- the switch is a compare chain (`beq', `slti', `beq'). */
void func_b12_80172290(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;
    u16 y;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_80175A6C;
        func_80047784(c, D_80175A6C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        b->unk_14 = 0xFFF00000;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
            *(s32 *)(p + 0xA0) += b->unk_14;
            b->unk_14 += 0x40000;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0x96) < 0)
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
            *(u8 **)(c + 0x2C) = D_80175A74;
            func_80047784(c, D_80175A74[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
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
    func_800AD594(d, 4);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) != 0)
    {
        y = *(u16 *)(d + 0x46);
        if ((y & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) = y & 0x7FFF;
        }
    }
    else if ((v & 0x410) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            *(s16 *)(d + 0x2A) =
                func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
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
void func_b12_801725F0(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b12", func_b12_80172654);

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state
   1 waits on func_8003F270, state 2 runs the +0x96 countdown and the
   D_80175A54 facing table, then re-installs func_b12_80170E70.
   316 words, 0 differing.

   THIS FUNCTION OWNS jtbl_b12_80170838, the 7-entry table at 0x80170838 for
   state 0's FIRST switch -- cases 1,2,3,5,6,7 over the 0x3FFF field, with
   case 4 falling to `default'. The second switch (the (d + 0x1C) & 0x2000
   `else') has only three cases and compiles to a compare chain, which is why
   the unit gets one new table and not two.

   `k' MUST BE `s16'. As `s32' the function is 315 words and the one missing
   word is the target's `addu $v0, $s5, $zero' ahead of the `beqz' -- without
   that copy reorg steals the `sh 0x98($s3)' into the branch's delay slot
   instead of the then-arm's `lui'. `u32' is 315 words / 7 aligned differences
   as well, `u8' 316/1, `k > 0' 315/6, and an `x = k' copy before the test
   315/7 (cse propagates it straight back); `s8' also lands at 316/0. This is
   docs/matching-levers.md's "`s16' for a boolean local, when `s32' costs a
   `move'" running the OTHER way -- here the narrow local is what BUYS the copy.

   THE INNER (three-case) SWITCH'S ARMS MUST BE WRITTEN 3, 2, 1. Ascending is
   318 words / 10: cross-jumping merges the table switch's 1/2/3 arms into
   these three, so their emission order is what the table's `j's point at. The
   table switch's own arm order does NOT matter -- written 5/1, 6/2, 7/3 it is
   also 316/0.

   `D_8008346C = 0' GOES BEFORE THE D_800814A8 DECREMENT. Written after it the
   `lw' of D_800814A8 has nothing to fill its load delay and both halves of
   D_8008346C move into the func_800A4ACC delay slot: 317 words / 9. A
   `w = (u8 *)D_800814A8' pointer local with the two statements the other way
   round reaches the same 316/0, so it is the order and not the local.

   THE D_8006DE24 RECORD NEEDS BOTH THE POINTER LOCAL AND THE INDEX LOCAL,
   exactly as the resident func_800AAF00 (csrc/ovl_dungeon_800A4B88.c) says:
   inline as `&D_8006DE24[g[0]]' the symbol is materialised AFTER the
   stride-0x14 multiply and the index load takes a `nop', 317 words / 10.
   THIS COPY'S OWN CONSTANT: 0x3 -> 0x20 at 1 of 4 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b12_80172AF8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR t;
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    s16 k;
    s32 i;
    s32 x;
    s32 y;
    s16 n;

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
                    *(s32 *)(d + 0x60) =
                        func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            t.vx = *(u16 *)((u8 *)b + 0x2);
            t.vy = *(u16 *)((u8 *)b + 0x6);
            t.vz = *(u16 *)((u8 *)b + 0xA);
            if (func_800A94A0(d, g, k, p + 0x98) == 0)
            {
                break;
            }
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x703);
            func_800DA840(&t, (g[0] - 1) % 3);
            p[0x9B]++;
        }
        else
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
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
        p[0x9B]++;
    case 2:
        if ((*(s8 *)(c + 0x4) == 4 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 32;
            *(u16 *)(p + 0x98) |= 0x80;
        }
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            *(s16 *)(p + 0x96) = 0;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) != D_80175A54)
        {
            *(u8 **)(c + 0x2C) = D_80175A54;
            func_80047784(c,
                          (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
        }
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) > 0)
        {
            d[0x6D] = d[0x6D] - 1;
        }
        d[0x73] = 0;
        d[0x72] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}

/* The banked twin of the resident overlay's func_800B2CA0
   (csrc/ovl_dungeon_800A4B88.c:9260): the same three-state lunge switched on
   the actor's +0x9B, with the same cross-jumped shared exit, the same
   `t = -1; if (flags) t = 8;` shape for +0x96, and the same +0x8C handoff.
   193 words, 0 differing. It differs from the twin in three measured ways:
   the facing index is computed ONCE into a local before the switch (the
   target keeps it in $s4 across both arms and reuses the `sll 1` byte offset,
   where the twin re-reads d + 0x6A four times); case 0 seeds the velocity
   from the tables POSITIVE and shifted 18 rather than negated and shifted 15;
   and the shared exit is func_800AAA54 with this bank's D_80175A94 table in
   $a3 rather than the twin's D_80083460 teardown.

   THE ONE SPELLING THAT HAD TO BE FOUGHT FOR IS CASE 2's TWO EASES. The
   target computes `c[0x24] << 6` FIRST, then the cell centre, then
   `sll 15`/`sra 1` -- two shifts, not one:

   | spelling of the two case-2 eases                     | words | aligned |
   | as written below: `(x = ...)` inside, then `>> 1`    |  193  |    0    |
   | `((...) * 0x8000) >> 1`, x assigned beforehand       |  193  |    7    |
   | `x = ...;` as its own statement, then `>> 1`         |  193  |    7    |
   | shift into x, then `x >> 1` as a second statement    |  193  |   11    |
   | `/ 2` in place of `>> 1`, assignment either way      |  193  |   31    |
   | no local at all, `>> 1`                              |  191  |    8    |
   | no local at all, `* 0x4000`                          |  191  |   12    |

   Two separate facts are in that table. The EMBEDDED assignment is what
   stops fold-const turning `- (*(s16 *)(b + 2) - 0x20)` into `+ 32`, worth
   two words; written as a preceding statement the fold is also stopped but
   the `lh` is then evaluated before the `lbu`, which is the 7. And `>> 1` is
   NOT interchangeable with `/ 2` here even though both emit `sll 15`/`sra 1`
   -- `/ 2` adds an eight-byte unreferenced stack local (`vars= 8`, frame 0x30
   against the target's 0x28), so all 31 of its differences are the prologue,
   the epilogue and the register renumbering they force. */
void func_b12_80172FE8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 k;
    s32 t;
    s32 x;
    s32 y;

    k = (*(u16 *)(d + 0x6A) >> 9) & 7;
    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[k] << 18;
        b->unk_10 = D_8006CCE8[k] << 18;
        p[0x9B]++;
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80175A94);
            return;
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
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        /* fallthrough */
    case 1:
        b->unk_0C -= D_8006CCD8[k] << 14;
        b->unk_10 -= D_8006CCE8[k] << 14;
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
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80175A94);
            return;
        }
        *(s16 *)(p + 0x96) = 8;
        p[0x9B]++;
        break;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) >> 1;
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) >> 1;
        }
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0)
        {
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
        *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b12", func_b12_801732EC);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b12_80173614(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b12_80173634(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b12_80173654(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}

/* The banked room script's three-state tail, state index at +0x9B: state 0
   re-dresses the renderable from the D_80175A9C facing table, state 1 is the
   long refusal chain, and state 2 hands the actor back to
   func_b12_80170E70. The resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c) is the same machine over the same record, and
   its comment covers the shape; read that first. 231 words, 0 differing.

   THE D_80175AA4 RE-DRESS TAIL IS WRITTEN TWICE AND CROSS-JUMPED BACK INTO
   ONE, and that duplication is the only thing that makes this function land:
   it is worth two references on `p`. The single-copy body -- the obvious one,
   with the chain inside `if (func_80042900(d, 1) != 0) { ... }` and the tail
   after it -- is 231 words with 18 ALIGNED DIFFERENCES, every one of them a
   register: `p` takes $s3 and `b` takes $s2, the other way round from the
   target. The cause is measured, not guessed. At flow time the single-copy
   body gives `p` 13 references over 215 insns and `b` 6 over 60, and
   global.c's allocno_compare ranks on floor_log2(n_refs) * n_refs /
   live_length: p scores 3*13/215 = 0.1814 against b's 2*6/60 = 0.200, loses,
   and is allocated second. Duplicating the tail puts p at 16 references
   (4*16/254 = 0.252) and it wins. 15 references would have been enough;
   14 is 1953 and still loses, so one extra store would not have done it.

   THE TWO COPIES NEED TWO DIFFERENT `&D_80083460` LOCALS. Sharing one
   (`k` in both) is 229 words with 10 differences: one pseudo spanning both
   ranges is allocated to $v1 in each copy, which makes state 1's increment
   `lhu $v0, 0xA($v1)` / `sh $v0, 0xA($v1)` -- identical to state 0's
   decrement, so jump2 merges those two `sh`s as well and takes the
   load-delay `nop` with them. Two locals get $v0 as the state 1 base and
   $v1 as the state 0 one, the `sh`s differ, and only the intended tail
   merges. (Same trap as func_800DA014's header records from the other side.)

   Measured and equal at 231 words / 0 differing, so none of these is the
   reason it lands: nesting the `d + 0x64` test instead of `&&`; reading
   `d + 0x1C` twice instead of into `v`; the chain reached by `goto` past an
   inverted gate instead of nested inside it; `d[0x25] == 0` written with an
   inner `else`; the duplicated tail placed inside the gate rather than
   ahead of the chain. Rejected: `goto` to a shared `p[0x9B] += 1` label
   (229 words, 32 differing -- the bump must be written out in both states
   and cross-jumped); a dead `z = (u32)p;` pair as a cheap reference bump
   (231 words, 18 differing -- cse deletes it before flow counts refs, so
   `p` stays at 13); using D_80082E80 directly instead of through `e`
   (231 words, 25 differing). */
void func_b12_8017368C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80175A9C;
        func_80047784(c, D_80175A9C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80175AA4;
            func_80047784(c, D_80175AA4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
                return;
            }
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
            p[0x9B] += 1;
            return;
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
            func_b12_80173D64(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175AA4;
        func_80047784(c, D_80175AA4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
            return;
        }
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        return;
    }
}

/* The banked twin of the resident overlay's func_800DA398
   (csrc/ovl_dungeon_800D50FC.c): the three-state tail of the handler
   installed at p + 0x8C, switching on the state index at p + 0x9B. State 0
   re-dresses the renderable from the D_80175A9C facing table, state 1 is the
   long refusal chain, state 2 hands the actor back to func_b12_80170E70.
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
void func_b12_80173A28(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80175A9C;
        func_80047784(c, D_80175A9C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80175A94;
            func_80047784(c, D_80175A94[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b12_80173D64(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175A94;
        func_80047784(c, D_80175A94[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
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
   call emits the bare `jal` either way); hoisting D_b12_80175A4C into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b12_80173D64(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80175A9C && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b12_80175A4C;
        func_80047784(c, D_b12_80175A4C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b12_80173E2C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b12_80170E70;
        }
    }
}

/* 14 words, 0 differing. */
void func_b12_80173E90(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}

/* 14 words, 0 differing. */
void func_b12_80173EC8(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}

/* 14 words, 0 differing. */
void func_b12_80173F00(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}

/* The 0x212 node's two-state fade: while its +0xC phase is 0, spend the
   +0xE countdown and step the phase when it runs out; in phase 1, walk the
   packed colour word at +0x8 down by 0x101010 a frame and kill the node --
   0x8000 into its -0x2 flags and into D_800814A0 -- once the byte at +0x8
   has dropped below 0x11. 41 words, 0 differing, first compile.

   NO TWIN: xsibs.py's best offer is town.bin:func_800C907C at ratio 0.635,
   which is a different function, and the callee set is empty (this body
   calls nothing). It is plain decompilation, and the one thing that made it
   immediate is that THE KILL PAIR IS ALREADY IN THIS UNIT: the
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;' couple and its
   `lhu'/`addiu -1'/`sh'/`sll 16'/`bgtz' countdown are both verbatim out of
   the landed func_b12_801749A8 below, so no new declaration was needed at
   all.

   WRITTEN AS A `switch', NOT AN if/else CHAIN, and there is no jump table:
   two cases compile to the `beqz'/`addiu $v0, 1'/`beq' compare chain the
   target has, and the listing opens straight at `.section .text'. The
   subtraction is spelled as the ADDITION of 0xFFEFEFF0 -- one `lui'/`ori'
   pair and an `addu', which is what `+= 0xFFEFEFF0' gives and what
   `-= 0x101010' would not. */
void func_b12_80173F38(u8 *p)
{
    switch (*(s16 *)(p + 0xC))
    {
    case 0:
        if ((s16)(--*(u16 *)(p + 0xE)) <= 0)
        {
            *(u16 *)(p + 0xC) += 1;
        }
        break;
    case 1:
        if (p[0x8] < 0x11)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(s32 *)(p + 0x8) += 0xFFEFEFF0;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b12", func_b12_80173FDC);

/* The node walk, the relinked twin of the resident overlay's func_800ADBCC
   (csrc/ovl_dungeon_800A4B88.c): walk the second list off p - 8, park a
   halfword in the scratchpad each turn and call func_800453E0 for every slot
   whose +0x14 does not carry 0x80. 35 words, 0 differing, first compile.

   THE RELINK IS THE SCRATCHPAD VALUE. func_800ADBCC parks the record's own
   `*(s16 *)(p + 0x8)'; this copy parks the bank-local s16 D_80175B20, whose
   `lui' is hoisted out of the loop while the `lh' stays inside it -- that is
   the whole of the three-word difference from the twin.

   Both of the twin's load-bearing spellings were kept and still are: the
   scratchpad goes through a POINTER LOCAL (as an absolute `*(s32 *)0x1F80013C'
   it is the assembler's `lui $at'/`sw' macro, which cannot leave the loop),
   and the slot stays a `u8 *' rather than a struct pointer so its +0x14 read
   is not MEM_IN_STRUCT_P and sched1 leaves the load-delay `nop's alone. */
s32 func_b12_8017458C(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = D_80175B20;
        if (!(*(u16 *)(s + 0x14) & 0x80))
        {
            func_800453E0(p, v, s, *(s16 *)(s + 0x6));
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (u8 *)n->unk_0C;
    }
}

/* One semi-transparent LINE_F2 segment per node of the tag chain, projected
   through the 0x1F800000 scratchpad: park the OT base in the scratchpad, load
   the node view's +0x2/+0x6/+0xA into unk_74's translation row, raise 0x8000
   in the slot's flags, then PushMatrix / RotMatrix / ReadRotMatrix /
   CompMatrix / ScaleMatrix / SetTransMatrix / SetRotMatrix the 0x2000-scaled
   local frame.  Allocate a 0x10-byte primitive off D_80083160.unk_00 + 0x8D0,
   fill a DEGENERATE quad (two endpoints, each written into two of the four
   SVECTORs), RotAverage4 it, propagate the slot flags' 0x8 / 0x4 pair into the
   embedded primitive's code byte and into the new LINE_F2's, addPrim it, then
   a DR_MODE behind it and PopMatrix.  207 words, 0 differing.  1 member
   (dngfam.py).

   NO TWIN: dngtwin finds no carved sibling at 0.70 and xsibs none either.
   The route was the CALLEE-SET one, two listings up IN THIS UNIT:
   func_b12_80174954 (landed, 21 words) names its resident original
   func_800D67B0, and func_800D67B0's per-node call func_800D6170
   (csrc/ovl_dungeon_800D50FC.c) is this function line for line -- same
   scratchpad layout, same matrix pipeline, same `sp->unk_34.vx = vy = vz =
   0x2000' chain.  The deltas are that func_800D6170 draws a POLY_F4 inside
   two loops over a +0x44..+0x52 grid where this draws ONE LINE_F2 off
   +0x64..+0x6E, and that this one carries the flag-propagation pair
   func_800D6170 has not got.  The DR_MODE tail and the addPrim spellings come
   from csrc/ovl_town_800BF3F0.c's func_800BFFF4 and from
   docs/dng-parks/func_b12_80173FDC.c, which priced the same block in this
   unit.

   FOUR SPELLINGS ARE LOAD-BEARING, measured here with everything else as
   below:

   (1) `Unk80083160 *g = &D_80083160' AS A POINTER LOCAL.  Written as the
   direct `D_80083160.unk_00' at all three uses it is 207 words / 6 aligned
   differences: the target materialises the full address into $s0 in the
   gate's `beqz' delay slot (`addiu $s0, $s2, %lo') and reads `lw $v1,
   0x0($s0)' for the DR_MODE allocation, where the direct form spends a fresh
   `lui'/`lw 0x0' pair there.  The pointer is needed only for THAT use: `g'
   declared and used only for the DR_MODE allocation, with the first two uses
   left as `D_80083160.unk_00', is also 207 / 0, so the uniform spelling below
   is a choice and not a measurement.

   (2) NO `MATRIX *mp' LOCAL -- `ReadRotMatrix(&m)' and `CompMatrix(&m, ...)'
   take the stack matrix's address twice.  func_800D6170 uses `mp' and that
   device does NOT carry: with it the body is 209 words / 92 aligned
   differences, and 90 of the 92 are the whole callee-saved assignment
   shifting one place down the $s0..$s4 order, because `mp' takes a register
   of its own.  THIS IS THE TWIN FACT THAT INVERTED.

   (3) THE TWELVE SVECTOR STORES ARE SIX CHAINED ASSIGNMENTS, high index
   first: `sp->unk_B0[0].vx = sp->unk_B0[2].vx = *(u16 *)(p + 0x64)'.  As
   twelve separate statements it is 213 words / 33 aligned differences -- cse
   keeps one `lhu' per pair either way, but the second store then needs its
   own address computation.  The chain's right-to-left order is what puts the
   +0xC0 store ahead of the +0xB0 one, which is how the target has them.

   (4) THE COLOUR WORD IS COPIED BEFORE `setlen'/`setcode'.  With the tag
   setup first it is 213 words / 14 aligned differences -- the same direction
   func_800D5AC0's note records (`$v0' would carry both the constant and the
   loaded word).  The `lbu`-free `ori 0x2'/`andi 0xFD' constants in the second
   gate are cse folding `getcode' back to the 0x40 just stored.

   (5) RotAverage4'S THIRD AND FOURTH sxy OUT-POINTERS ARE `t + 0x8' AND
   `t + 0xC' AGAIN, not `t + 0x10'/`t + 0x14'.  The primitive is a LINE_F2
   with only two xy pairs and the allocator steps 0x10, so the quad is
   degenerate in its outputs as well as its inputs; func_800D6170's
   +0x10/+0x14 spelling is 208 words / 18 aligned differences.

   MEASURED AND FREE, so NOT the reason this lands: `s32 d' with `(s16)d' at
   the subtraction (207/0 -- the sub-word parameter is NOT load-bearing here,
   which is the opposite of what the recorded lever predicts, because `d' has
   exactly one use and no parameter copy to keep schedulable); `s16 unk_28' in
   the scratchpad typedef instead of `u16' (207/0 -- the `lhu' comes from the
   two `andi' masks, not from the declared signedness); the `sp->unk_34'
   triple written as three descending statements rather than a chain (207/0);
   and `setSemiTrans' expanded by hand into `if/else' with `setcode(t,
   getcode(t) | 0x02)' arms (207/0, which is the macro's own expansion).

   THE SCRATCHPAD TYPE IS THE UNIT'S EXISTING `UnkDng74334SP', which was
   already declared here and unused; it is UnkDungeonSP6
   (csrc/ovl_dungeon_decls.h) with four extra Unk3DBD0W fields at +0xC..+0x18
   that this function does not touch.  No new typedef was invented.

   The listing has no `.section .rodata' block, so this function owns no jump
   table. */
void func_b12_80174618(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    MATRIX m;
    u8 *t;
    u8 *dm;

    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    ReadRotMatrix(&m);
    CompMatrix(&m, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    sp->unk_28 = s->unk_14;
    t = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x10;
    sp->unk_B0[0].vx = sp->unk_B0[2].vx = *(u16 *)(p + 0x64);
    sp->unk_B0[1].vx = sp->unk_B0[3].vx = *(u16 *)(p + 0x6A);
    sp->unk_B0[0].vy = sp->unk_B0[2].vy = *(u16 *)(p + 0x66);
    sp->unk_B0[1].vy = sp->unk_B0[3].vy = *(u16 *)(p + 0x6C);
    sp->unk_B0[0].vz = sp->unk_B0[2].vz = *(u16 *)(p + 0x68);
    sp->unk_B0[1].vz = sp->unk_B0[3].vz = *(u16 *)(p + 0x6E);
    sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                              (s32 *)(t + 0x8), (s32 *)(t + 0xC), (s32 *)(t + 0x8),
                              (s32 *)(t + 0xC), &sp->unk_D0, &sp->unk_D4) -
                  d - 6;
    if ((sp->unk_28 & 8) != 0)
    {
        setSemiTrans((u8 *)s + 0x8, sp->unk_28 & 4);
    }
    *(s32 *)(t + 0x4) = *(s32 *)((u8 *)s + 0xC);
    setlen(t, 3);
    setcode(t, 0x40);
    if ((sp->unk_28 & 8) != 0)
    {
        setSemiTrans(t, sp->unk_28 & 4);
    }
    addPrim(sp->unk_24 + sp->unk_100 * 4, t);
    dm = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)dm + 0xC;
    func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
    addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_b12_80174618 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b12_80174954(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b12_80174618(p, v, s, 0);
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

/* Spend one frame of the payload's +0x1A life counter and, when it runs out,
   raise the 0x8000 kill bit both in the node's -0x2 flags and in
   D_800814A0.  17 words, 0 differing, first compile.

   The twin is csrc/ovl_dungeon_800D50FC.c's func_800D5CA0, whose tail is
   this function verbatim with the counter at +0x32; the relink moves it to
   +0x1A and nothing else changes.  The decrement stays folded into the test
   (`--*(s16 *)(p + 0x1A) <= 0`), which is what keeps the one `lhu`/`addiu`/
   `sh` run and the `sll`/`bgtz` pair instead of a reload. */
void func_b12_801749A8(u8 *p)
{
    if (--*(s16 *)(p + 0x1A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b12", func_b12_801749EC);

/* The auxiliary 0x12 node func_b12_80175470's state 2 spawns for the
   white-out: allocate it, seed the view's +0x8/+0xA/+0xC with 0x4A, the
   slot's +0x96 countdown and 0, install func_b12_801749EC as the node's +0x10
   driver and D_80045340 as its tag list, raise 0xC then 0x80 in the slot
   flags with the +0x10 depth bias between them, copy the caller's vector down
   by 0xFFE80000 and finish with the 0x1000 scale pair and the three 0x80
   colour bytes.  59 words, 0 differing.  1 member (dngfam.py).

   TWIN: dngtwin's best carved rows are 0.775 and below and all of them are
   still stubs.  The body came from the landed func_b14_801745E0
   (csrc/ovl_dng_b14.c, 71 words) instead, found by grepping the callee set
   `func_8003FC64' + `func_8004491C' + `D_80045340': it is the same spawner
   with a 0x10-iteration loop around it, and its flag/scale/colour tail is
   this function's verbatim.  Four of its spellings are load-bearing here and
   were each re-measured:

   (1) `r->unk_10 = 0x20' GOES BETWEEN THE TWO `r->unk_14 |=' STATEMENTS.
   With the two ORs adjacent the body is 57 words / 13 aligned differences:
   combine merges them into one `ori $v0, $v0, 0x8C' and the first `sh'
   disappears, where the target has `ori 0xC'/`sh'/`ori 0x80'/`sh' off ONE
   `lhu'.  The intervening store is what keeps the first OR's result live.

   (2) `r = (UnkSlot *)n->unk_0C' IS READ A SECOND TIME before the scale/
   colour block.  With one `r' spanning both blocks it is 58 words / 6 aligned
   differences; the target reloads because the stores through `v' wipe cse's
   memory table.

   (3) `v = n->unk_08' IS A LOCAL.  Written out as `n->unk_08->unk_00' three
   times it is 60 words / 9 aligned differences -- the target has ONE
   `lw $a0, 0x8($s0)' for all three stores.  (This is the opposite of
   func_b12_80175470's state 4, which reloads it per store; same unit, same
   callee, different spelling.)

   (4) THE THREE 0x80 COLOUR BYTES GO +0xE, +0xC, +0xD.  Ascending is 59
   words / 2 aligned differences.

   AND ONE FACT IS THIS BODY'S OWN, FOUND OFF THE RESIDUE RATHER THAN THE
   TWIN: THE VIEW'S THREE HALFWORD SEEDS GO +0x8, +0xA, +0xC, i.e. the +0x96
   READ IS THE MIDDLE STATEMENT.  With the +0xC store in the middle (the
   order the target's `sh's come out in -- 0x8, 0xC, 0xA) it is 59 words / 11
   aligned differences, and the residue is NOT the two transposed stores: 9 of
   the 11 rows are `p' and `b' holding each other's callee-saved register.
   Reading +0x96 earlier shortens `p's live range, so
   floor_log2(2)*2/live_length overtakes `b's 2*4/live_length and `p' takes
   $s1.  Partitioning the rows is what showed this was one allocation fault
   and not a scheduling one; no device was needed.

   MEASURED AND FREE: `if (n == NULL) return;' in place of wrapping the body
   in `if (n != NULL)' (59/0).

   func_b12_801749EC is address-taken here and is still an INCLUDE_ASM stub,
   so it needs the declaration above; three parameters, which is
   func_80040190's cast for a +0x10 driver and all its listing reads
   ($a0, $a1, $a2). */
void func_b12_80175180(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *v;
    UnkSlot *r;
    u8 *q;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x8) = 0x4A;
        *(s16 *)(q + 0xA) = *(u16 *)(p + 0x96);
        *(s16 *)(q + 0xC) = 0;
        n->unk_10 = (u32)func_b12_801749EC;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0xC;
        r->unk_10 = 0x20;
        r->unk_14 |= 0x80;
        v = n->unk_08;
        v->unk_00 = b->unk_00;
        v->unk_04 = b->unk_04;
        v->unk_08 = b->unk_08 + 0xFFE80000;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_0C[2] = 0x80;
        r->unk_0C[0] = 0x80;
        r->unk_0C[1] = 0x80;
    }
}

/* State 0x17's entry: clear the node's +0x71 top bit and +0x6D, install
   ITSELF in the slot's +0x8C handler slot and mark state 0x17/0; then, unless
   D_80083460's +0x2 0x2000 bit is up, if func_800A2BDC says the engine is
   idle, drop the handler again, re-mark 0x17/0, reset D_800DCF5E and bump
   D_80083460's +0xA counter. 43 words, 0 differing, first compile.

   THE +0x8C STORE IS THE FUNCTION'S OWN ADDRESS -- `lui'/`addiu' of
   %hi/%lo(func_b12_8017526C) -- so the self-reference is written as the
   plain identifier, not as a table lookup.

   D_80083460 goes through the unit's `g = (u8 *)&D_80083460' idiom: one
   `addiu' base in $s1 serves both the +0x2 gate and the +0xA bump. */
void func_b12_8017526C(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    *(void **)(p + 0x8C) = func_b12_8017526C;
    p[0x9A] = 0x17;
    p[0x9B] = 0;
    d[0x6D] = 0;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        if ((s16)func_800A2BDC(d) == 0)
        {
            *(s32 *)(p + 0x8C) = 0;
            p[0x9A] = 0x17;
            p[0x9B] = 0;
            D_800DCF5E = 0;
            *(u16 *)(g + 0xA) += 1;
        }
    }
}

/* The two-phase white-out: with the slot's +0x9A still zero, park the view's
   +0x8 in the bank-local D_80175B28, re-send the view through func_800478B8
   and -- only if its +0x14 carries one of 0xE000 -- unlink the node, set the
   0x10-frame countdown in +0x96 and step +0x9A; every eighth frame of that
   phase also bumps D_80175B20. Afterwards, ease D_80083160's three
   +0xA8..+0xAA colour bytes towards 0x80 by (0x80 - v) / +0x96 and, when the
   countdown runs out, raise 0x8000 in the node's -0x2 and in D_800814A0.
   86 words, 0 differing, first compile. 2 copies (dngfam.py:
   func_b12_80175318, func_b16_80173B94).

   xsibs.py FOUND NOTHING for this stub (best 0.633 to
   dungeon.bin:func_800C4C00) -- the twin came out of the CALLEE-SET GREP the
   brief describes: `grep -ln func_80044A50' intersected with
   `grep -ln func_800478B8' leaves six carved units, and all six call
   func_80044A50 as `func_80044A50((UnkNode *)(p - 0x20))' inside exactly
   this function. The template is the landed func_b04_801749F4 in
   csrc/ovl_dng_b04.c, 75 words, and both of its named facts hold here: the
   `lh' of +0x96 is re-read for each of the three colour bytes (each `sb'
   through the `u8 *' wipes cse's memory table), and D_80083160 is reached
   through a `u8 *' base taken BEFORE the gate, which is why its `addiu'
   sits in the `bnez's delay slot although the only use is in the else arm.
   The countdown test still reads the decremented value rather than a
   reload, `(s16)(--*(u16 *)(p + 0x96)) <= 0'.

   THE RELINK IS D_80175220 -> D_80175B28 AND ELEVEN EXTRA WORDS: the
   state-0 arm ends with `if (((*(u16 *)(p + 0x96))++ & 7) == 0)
   D_80175B20 += 1;'. The post-increment is load-bearing -- the target stores
   the BUMPED halfword (`lhu', `addiu 1', `andi 7' on the OLD value, `sh' the
   new) -- and the arm's exit `j' is reused by the new `bnez', so 75 + 12 - 1
   = 86. D_80175B20 is the same bank-local halfword func_b12_8017458C below
   parks in the scratchpad, and its existing `extern s16' declaration serves
   both uses unchanged.

   `--raw' is 13 differing and all 13 are relocations -- the two `jal's, the
   one `j' and the ten `%hi'/`%lo' halves for D_80083160, D_80175B28,
   D_80175B20 and D_800814A0. func_80044A50 and the bank-local D_80175B28
   are the only new declarations this body needed; D_80083160 and D_800814A0
   come from include/structs.h. */
void func_b12_80175318(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (p[0x9A] == 0)
    {
        D_80175B28 = *(s32 *)(c + 0x8);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9A] += 1;
        }
        if (((*(u16 *)(p + 0x96))++ & 7) == 0)
        {
            D_80175B20 += 1;
        }
    }
    else
    {
        g[0xA8] += (0x80 - g[0xA8]) / *(s16 *)(p + 0x96);
        g[0xA9] += (0x80 - g[0xA9]) / *(s16 *)(p + 0x96);
        g[0xAA] += (0x80 - g[0xAA]) / *(s16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* This bank's nine-state white-out cutscene, run off the slot's +0x9B: step
   the counter, hand func_80041588 the bank-local D_80175B0C command stream
   and its D_80175B24 handshake byte, turn the actor to face 2, swap the HUD
   pair through func_8003F540/func_8003E4FC, open with func_800C77D0, spawn
   the auxiliary 0x12 node through func_b12_80175180, ease D_80083160's three
   colour bytes while the D_80175B25 handshake is pending, spawn the 0x12 node
   whose driver is func_b12_80175318, wait for its +0x14 to carry one of
   0xE000, then func_800ADC4C the replacement in, re-aim it and finally raise
   0x8000 in the node's -0x2 and in D_800814A0 and drop D_80083460's +0xA
   counter.  347 words, 0 differing.  1 member (dngfam.py).

   TWIN: the landed func_b08_80174648 (csrc/ovl_dng_b08.c, 349 words, ratio
   0.954).  Same frame -- addiu $sp,-0x28, six saves s0..s4/ra at 0x10..0x24,
   same order -- so its allocation facts carry, and ALL THREE HOLD, re-priced
   on this body:

   (1) STATE 3'S HANDSHAKE BYTE MUST BE READ INTO A LOCAL BEFORE THE +0x96
   RESTORE STORE.  With the restore written ahead of the test, 348 words / 8
   aligned differences (the twin's own price was 350/248).

   (2) STATE 6'S `d - 0x20' MUST BE THE SAME LOCAL AS STATES 4/5'S SLOT
   POINTER.  Given its own local it is 346 words / 14 aligned differences
   (the twin's price was 349/6).

   (3) STATE 5'S ARM IS THE WHOLE OF STATE 6'S BODY WRITTEN OUT A SECOND
   TIME.  Spelled as the bare `p[0x9B] += 1' bump it is 324 words / 45
   aligned differences.  As in the twin there is no `goto' that reaches 347:
   jump2 will not cross-join the two copies because state 5's falls through
   into state 6's gate.

   THE RELINK IS D_80174CF4/D_80174CFC/D_80174D18/D_80174D19/D_80174D14/
   D_80174D1C -> D_80175B0C/D_80175B14/D_80175B24/D_80175B25/D_80175B20/
   D_80175B28, the node slot moves from p+0xAC to p+0xA8, state 1's cleared
   halfword from p+0xB0 to p+0xAC, the driver pair becomes
   func_b12_80175318 / func_b12_8017458C, and the parked halfword is
   `D_80175B20 = -0xC' where the twin writes 5.

   TWO SHAPE DELTAS OVER THE TWIN, both measured:

   STATE 1 RETURNS, IT DOES NOT FALL THROUGH INTO STATE 2 -- the target's
   `j' to the epilogue sits where the fallthrough needs a load-delay `nop'.
   Written the twin's way (fallthrough) it is 347 words / 1 aligned
   difference, which is this body's whole residue off the first compile.

   THE func_b12_80175180 SPAWN IS THE LAST STATEMENT OF STATE 2, after the
   +0x96 and +0x9B stores, where the twin's equivalent call closes state 4.
   Written ahead of those two stores, 347 words / 2 aligned differences.

   THIS FUNCTION OWNS jtbl_b12_80170858, the 9-entry table at 0x80170858
   (rodata offset 0x50, `.align 3') for states 0..8.  Every state has a label
   of its own, no two share an arm and there is no `default': the
   `sltiu $v1, 0x9' gate sends everything else to the epilogue.  States 2, 5
   and 7 fall through to their successors.

   MEASURED AND FREE, so NOT the reason this lands: `D_80175B20 = -0xC'
   written after `r = (u8 *)n->unk_0C' instead of before it (347/0, as in the
   twin), and state 1's `*(s16 *)(p + 0xAC) = 0' written after the p[0x9B]
   bump instead of before it (347/0).

   `--raw' is 80 differing and all 80 are relocations: 24 `jal's, 7 `j's,
   23 `lui's, 15 `%lo' `addiu's and 11 `%lo'-based loads/stores. */
void func_b12_80175470(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *q;
    u8 *r;
    u8 *w;
    UnkNode *n;
    s32 t2;
    s32 v;
    s32 f;
    s16 t;
    s16 k;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        p[0x9B] += 1;
        return;
    case 1:
        func_80041588(D_80175B0C, (s8 *)D_80175B24, 0);
        *(s16 *)(p + 0xAC) = 0;
        p[0x9B] += 1;
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x2A);
        return;
    case 2:
        t2 = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (D_80175B24[0] == 0 || t2 != 2)
        {
            if (t2 != 2)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            return;
        }
        func_80041588(D_80175B0C, (s8 *)D_80175B24, 1);
        func_8003F540(0, D_8006CD58, 0x4000AD4, 0x5000CC4);
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        D_80175B25[0] = 0;
        func_8003E4FC(0xFF, D_8003E140, (s32)D_80175B25);
        func_800C77D0(d - 0x20, (u8 *)b, 8, 0x300);
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        func_b12_80175180(p, b, c);
        /* fallthrough */
    case 3:
        t = *(u16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            f = D_80175B25[0];
            *(s16 *)(p + 0x96) = t;
            if (f == 0)
            {
                return;
            }
            p[0x9B] += 1;
            func_800A56E0(0x300);
            return;
        }
        q = D_800DCEEC[func_800498A0(d)];
        g[0xA8] += (q[0] - g[0xA8]) / *(s16 *)(p + 0x96);
        g[0xA9] += (q[1] - g[0xA9]) / *(s16 *)(p + 0x96);
        g[0xAA] += (q[2] - g[0xAA]) / *(s16 *)(p + 0x96);
        return;
    case 4:
        n = func_8003FC64(0x12);
        if (n == NULL)
        {
            return;
        }
        n->unk_10 = (u32)func_b12_80175318;
        func_8004491C(n, (u32)func_b12_8017458C);
        *(UnkNode **)(p + 0xA8) = n;
        n->unk_08->unk_00 = b->unk_00;
        n->unk_08->unk_04 = b->unk_04;
        n->unk_08->unk_08 = b->unk_08;
        D_80175B20 = -0xC;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        k = func_800498A0(d) - 1;
        if (k < 0)
        {
            k = rand() % 3;
        }
        func_8003DB94(r, D_8014A000 + D_80175B14[k], 0);
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        return;
    case 5:
        n = *(UnkNode **)(p + 0xA8);
        r = (u8 *)n->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0xE000) != 0)
        {
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9B] += 1;
            func_800A18E8(d[0x13], 3);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            func_8009A028(d);
            r = d - 0x20;
            *(u32 *)(r + 0x10) |= 0x80000000;
        }
        /* fallthrough */
    case 6:
        if (p[0x9B] == 6 &&
            func_800ADC4C((u8 *)b, D_80175B28, D_80175B20, &D_800DCF5C) != NULL)
        {
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9B] += 1;
            func_800A18E8(d[0x13], 3);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            func_8009A028(d);
            r = d - 0x20;
            *(u32 *)(r + 0x10) |= 0x80000000;
        }
        return;
    case 7:
        v = func_800A504C(c, d);
        *(s32 *)(d + 0x60) = v;
        if (v == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        *(s16 *)(*(u8 **)(d + 0x60) + 0x2A) = *(u16 *)(d + 0x2A);
        w = *(u8 **)(d + 0x60);
        r = *(u8 **)(w - 0x14);
        func_80047738(r,
                      (*(u8 **)(r + 0x2C))[((D_80083228 + *(s16 *)(w + 0x2A) + 0x100) >> 9) & 7],
                      *(s8 *)(r + 0x4));
        *(u16 *)(r + 0x14) &= ~1;
        /* fallthrough */
    case 8:
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(s16 *)(*(u8 **)(d + 0x60) + 0x2A) = *(u16 *)(d + 0x8A);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            d[0x6D] = 0;
        }
        return;
    }
}

/* 14 words, 0 differing. */
void func_b12_801759DC(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}

/* 14 words, 0 differing. */
void func_b12_80175A14(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b12_80170E70;
    }
}
