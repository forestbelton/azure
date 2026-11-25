#include "common.h"

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
s32 func_b41_80171FF4(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80174888[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80174880[];
extern u8 D_801748E0[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b41_80171848(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b41_80171058(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AD058();

extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
void func_b41_80171F90(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB464();

s32 func_800AC7EC();

extern u8 D_80174898[];

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

s32 func_800AB538();

extern u8 D_801748E8[];
s32 func_800A2C34(u8 *);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b41_80173834(u8 *p, u8 *b, u8 *c, u8 *d);

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
extern u8 D_801748B8[];

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
extern s32 D_8008346C;
void func_800A2B04(UnkVec *, s32, s32);
s32 func_800A56E0(s32);
extern u8 *D_800E3DE8;
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);

void func_b41_80171848(u8 *p, void *b, void *c, u8 *d);
extern u8 D_801748C8[];
extern u8 D_801748D8[];
extern u8 D_80174890[];
void func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, u8 *t, void (*h)());
s16 func_800A1C58(u8 *p);
s16 func_8009FB34(u16 x, u16 y);
s32 func_800C7F68(u8 *p);
void func_b41_801715F0();
void func_b41_801721B8(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b41_80172414(u8 *a, void *b, void *c, s32 d);
s32 func_8003F270(void);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
void func_800DA840(SVECTOR *src, s16 a);
UnkNode *func_8003FD64(s32, void *);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b41_80170A78(u8 *p, UnkVec *v, u8 *c, u8 *t);
extern u8 D_80083498[];
extern u8 D_80045340[];
void func_b41_801722E0(u8 *p, UnkVec *b, u8 *c, u8 *d);

/* Spawn the kind-0x112 node this bank's scripts use for a thrown or placed
   actor: arm it with the bank-local func_b41_80170A78 handler and the
   D_80174880 facing table, dress its slot from the b/c/d arguments, pick one
   of three flag pairs off `a & 3', and hand it to func_800A9C18 and
   func_800AA36C. 123 words, 0 differing, ON THE FIRST COMPILE.

   The twin is the landed 120-word func_b16_80170878 (csrc/ovl_dng_b16.c),
   whose own comment states that nothing in it was fought for, so there were
   no inherited facts to test -- the ones below are this body's own. Deltas
   from it: p[0x13] is 0x29, the handler and the two facing tables are
   func_b41_80170A78 / D_80174880 / D_801748E0, the +0x92 slide is -0x18
   rather than -0x20, the installed step is func_b41_80171058, and -- this is
   the three words that make 123 against b16's 120 -- the func_800A6D30 arm
   DOES set `*(u32 *)(p + 0x1C) |= 0x200', which b16's does not.

   TWO SPELLINGS MEASURED HERE, because a first-compile landing proves
   nothing about which of its choices mattered:
     `w = p' collapsed into plain `p' at the five tail sites
                                                  122 words / 86 differing
     the `|= 0x200' written AFTER the func_800A48F0 call
                                                  124 words / 54 differing
   AND ONE MEASURED FREE, so it is NOT why this lands: `k = a & 3' hoisted
   into a local. Written inline as `(a & 3) == 1' / `(a & 3) >= 2' it is also
   123 words / 0 differing. 1 copy (dngfam.py). */
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_801748C0[];
extern u8 D_801748A0[];
extern u8 D_801748A8[];
extern u8 D_801748B0[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void *func_800990FC(void);
void *func_80099194(u8 *, u8 *);
u8 *func_80099290(u8 *);
void *func_80099734(u8 *, u8 *);
s32 func_800A5720(void *);
extern const u8 D_b41_80170838[];
extern const u8 D_b41_80170848[];

s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
void func_800478B8(u8 *);
s32 func_800644B8(s32);
s32 func_800BCB04(s32, s32, s16);
extern u8 D_8006CCF8[];
extern void (*D_80174900[])(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA53C(u8 *);
s32 func_800A4E2C(u8 *, u8 *);
s32 func_8009A2B8(s16, s16, s32);
extern u8 D_800DDC40[];
extern s16 D_8008146E;
extern u8 D_801748F0[];
extern u8 D_801748F8[];

u8 *func_b41_8017088C(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b41_80170A78;
        p[0x13] = 0x29;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80174880;
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
                    *(u32 *)(p + 0x1C) |= 0x200;
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_801748E0;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b41_80171058;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x18;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

/* The UnkNode +0x10 PER-FRAME DRIVER for this bank's thrown/placed actor --
   the body func_b41_8017088C arms the node with. Past the engine's 0x2000
   gate (where it runs the +0x8C continuation only when that continuation is
   this object's own func_b41_80171058) it runs func_800A9E70, the record's
   +0x8C step, the +0x9A entry of this bank's D_80174900 handler table,
   re-aims through func_800AA36C when the +0x6D facing moved under it, steps
   the mover, runs the +0x9D bob, re-skins the slot for the eight-way facing
   and leashes +0x92/+0x88 to the ground height func_800BCB04 reports.
   376 words, 0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED func_b25_80171964 (csrc/ovl_dng_b25.c), 366 words,
   dngtwin 0.916, and the two prologues are BYTE-IDENTICAL -- same -0x30
   frame, same $s0..$s5 + $ra save list in the same order at the same
   offsets -- so all three of its fact classes were in scope here. Nine
   operand deltas and two structural ones account for the whole 376:
   func_b41_80171058 for func_b25_80171F1C, D_80174900 for D_b25_8017664C,
   D_80174880 for D_800E2348, +0xB2/+0xA4/+0xA6 for +0x9E/+0xA0/+0xA2, the
   wobble multiply is 170 (sll 2/addu/sll 4/addu/sll 1) not 195, +0xA4 is
   ACCUMULATED with `+= func_800644B8(...) << 5' where b25 ASSIGNS
   `-func_800644B8(...) << 7'; b25's `*(u16 *)(c + 0x4) == 0x400' reset of
   the counter is ABSENT, and so is b25's `p[0x9A] == 0x18' arm on the +0xA
   publish. The one real shape delta: b25's 0x40000-TRUE clamp is against
   the constants -0x20/-0x28 through a `lim' local, where this body calls
   func_800BCB04 in that arm too and clamps against `e - 0x18'/`e - 0x20',
   so there is no constant to hoist and no `lim' local at all.

   FOUR OF b25's SPELLINGS REPRICED HERE, AND TWO OF THEM MOVED:

   | spelling                                        | words | aligned |
   | as written below                                |  376  |    0    |
   | no `d = p' alias, `p' at every site             |  373  |   149   |
   | `s32 dir' instead of `s16 dir'                  |  373  |    9    |
   | ONE `w' local shared by both copies of the arm  |  376  |    4    |
   | one facing local instead of `k' and `i'         |  376  |    0    |
   | ONE `e' local shared by both copies of the arm  |  376  |    0    |
   | three-parameter signature, no unused 4th        |  376  |    0    |

   The alias (b25: 146) and the `dir' width (b25: 9) reproduce b25's prices.
   THE `local-alloc.c:1759' DUPLICATED-ARM FACT HOLDS AT b25's EXACT FIGURE,
   4 rows, and the residue is b25's: the +0xB2 increment lands in $v0 at both
   duplicated sites where the target has `addiu $v1, $v1, 1', with the word
   count unchanged. BUT IT IS NARROWER THAN b25's WORDING. The 0x40000-TRUE
   arm is written out in BOTH arms of the +0x14 0x8000 gate and it now
   contains TWO locals, and only ONE of them wants its own copy: sharing `e'
   -- the func_800BCB04 height the clamp compares against, in the same
   duplicated block -- is 376 words / 0 aligned differences. So the rule is
   not "a duplicated block's locals each need their own scope"; it is about
   the local whose SET is tied to a dying input, which is what
   `combine_regs' refuses when `reg_qty' is -1. `e' is only read.

   AND b25's FACING-LOCAL FACT IS FREE HERE, which contradicts its comment.
   b25 prices one facing local instead of `k' and `i' at 363 words / 63
   aligned differences; on this relink `s16 k' alone with no `i' at all is
   376 words / 0, the same word count, so it is free and not merely equal.
   Both bodies have the identical frame and save list, so nothing about the
   frame predicts the difference -- treat that row of b25's table as priced
   on b25 only. `k' and `i' are kept below because that is the form the
   whole-unit re-verification ran against, not because they were earned.

   The unused fourth parameter is kept only to agree with the declaration
   this unit already had; the body does not read it and 3 params measure the
   same 376 / 0, so the arity is NOT witnessed by this body. Every call it
   makes passes `p' as the fourth argument, which is why the listing has
   `addu $a3, $a0, $zero' rather than a deleted copy.

   No jump table and no rodata of its own -- the listing opens at `glabel'.
   relcand: ok, 5 data symbols and 11 calls. 1 copy (dngfam.py). */
void func_b41_80170A78(u8 *p, UnkVec *v, u8 *c, u8 *t)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *d;
    s16 dir;
    s32 i;
    s16 k;
    s16 e;
    s16 e2;
    s16 f;
    s16 g;
    u16 w;
    u16 w2;

    d = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b41_80171058)
        {
            h(p, v, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)p[0x6D];
    if (func_800A9E70(p, v, c, p) != 0)
    {
        return;
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, v, c, p);
    }
    D_80174900[p[0x9A]](p, v, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, v, c, p);
    }
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    if ((*(s32 *)(p + 0x1C) & 0x40000) == 0 && (*(u16 *)(p + 0x98) & 0x8) == 0)
    {
        v->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    else
    {
        p[0x9D] = 0;
    }
    *(s32 *)(p + 0x90) += v->unk_14;
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        i = k;
        if (*(s16 *)(p + 0x94) != i)
        {
            func_80047738(c, (*(u8 **)(c + 0x2C))[i], *(s8 *)(c + 0x4));
            *(s16 *)(p + 0x94) = k;
        }
        if (D_8006CCF8[i] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        func_800A020C(*(s32 *)(d + 0x1C), c + 0xC);
        if ((*(s32 *)(d + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
            *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        }
        *(s32 *)(d + 0x1C) &= 0xF7FFFFFF;
        if ((*(s32 *)(d + 0x1C) & 0x40000) != 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_80174880)
            {
                w = *(u16 *)(p + 0xB2);
                *(s16 *)(p + 0xB2) = w + 1;
                *(s32 *)(p + 0xA4) += func_800644B8((s16)w * 170) << 5;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (*(s16 *)(p + 0x92) > e - 0x18)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < e - 0x20)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
        else
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA4);
            *(s16 *)(p + 0xB2) = 0;
            *(s32 *)(p + 0xA4) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                f = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (f < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = f;
                    p[0x9D] = 0;
                    v->unk_14 = 0;
                    *(s32 *)(d + 0x1C) |= 0x8000000;
                }
            }
        }
    }
    else
    {
        if ((*(u16 *)(c + 0x14) & 0x800) != 0)
        {
            *(u16 *)(c + 0x14) &= 0x8FFF;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        *(s32 *)(d + 0x1C) &= 0xF7FFFFFF;
        if ((*(s32 *)(d + 0x1C) & 0x40000) == 0)
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA4);
            *(s16 *)(p + 0xB2) = 0;
            *(s32 *)(p + 0xA4) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                f = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (f < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = f;
                    p[0x9D] = 0;
                    v->unk_14 = 0;
                    *(s32 *)(d + 0x1C) |= 0x8000000;
                }
            }
        }
        else
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_80174880)
            {
                w2 = *(u16 *)(p + 0xB2);
                *(s16 *)(p + 0xB2) = w2 + 1;
                *(s32 *)(p + 0xA4) += func_800644B8((s16)w2 * 170) << 5;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e2 = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                     *(u16 *)(d + 0x88);
                if (*(s16 *)(p + 0x92) > e2 - 0x18)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < e2 - 0x20)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
    }
    if ((*(s32 *)(d + 0x1C) & 0x40000000) != 0)
    {
        *(s32 *)(d + 0x1C) &= 0xBFFFFFFF;
        g = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (g < 0x200)
        {
            *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + (*(u16 *)(d + 0x88) - g);
            *(s16 *)(d + 0x88) = g;
        }
    }
    *(s16 *)((u8 *)v + 0xA) = (*(u16 *)(d + 0x88) + *(u16 *)(p + 0x92)) - *(u16 *)(p + 0xA6);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The banked room script's IDLE handler: the body parked at the actor's
   +0x8C, and the one its own switch re-installs through func_800AAF00.
   OWNS A JUMP TABLE: jtbl_b41_80170808, twelve entries for the +0x46 state
   1..12, with 4, 10 and 11 carrying the default label rather than arms of
   their own. 358 words, 0 differing.

   THE TWIN IS THE LANDED func_b01_801714D4 (csrc/ovl_dng_b01.c), 337 words.
   Of its three named facts, (1) the shared tail written out at every site
   and (2) one table local per dress site both HOLD -- one shared `e' across
   this copy's two dress sites is 2 differing -- and its measured delay-slot
   fact holds in a stronger form: the +0x9A dress test must load c + 0x2C
   into `f' AHEAD of the table local, or the table's `lui' fills the `beq'
   delay slot and the body is 357 words / 5 aligned differences. Its fact (3)
   does not arise, because this copy's dress is a ONE-WAY test against
   D_80174880 and not b01's two-way D_801740E0/D_801740E8 pair.

   Deltas from that twin, all read off the listing. The 0x200 escape compares
   against D_801748E0 and re-dresses from D_801748D8, and func_800AA924 is
   handed D_801748D8 rather than a table of its own. The cleared pair is
   p + 0xA6 / p + 0xB2, not b01's p + 0xA2 / p + 0x9E, and func_800AA6B4's
   table is D_80174890. The switch splits b01's single 8/9/10 arm into case 8
   (func_b41_80171FF4 then func_b41_801721B8) and a NEW case 9 that parks
   func_800A04F0's hit at p + 0xA8 and gates on that record's +0x13 byte
   being below 0x33 before func_b41_801722E0; cases 1, 2 and 3 pass
   D_801748C8 to func_800AAF00 as its fourth argument where b01 passes 0.
   The shared tail is the bank-local func_b41_80171848, written out at SIX
   sites.

   TWO REFERENCE BUMPS, AND THEY ONLY WORK AS A PAIR. Without them the body
   is still 358 words with 70 aligned differences, every row `p' and `d'
   exchanging $s1/$s2 -- a WRONG PRIORITY and not a wrong pseudo count, since
   no `move' is missing. global.c ranks on
   floor_log2(n_refs) * n_refs / live_length, and measured off `-da' this
   body's p has 31 references scoring 4*31 against d's 41 at 5*41, so p is
   allocated fourth of the four parameters instead of second. flow.c weights
   REG_N_REFS by loop depth, so a `do { } while (0)' DOUBLES the references
   inside it: wrapping the 0x1000 escape takes p across the 32-reference
   power of two to 5*32 -- but that alone OVERSHOOTS, lifting p past `c' to
   $s0 for 69 differing. Wrapping the func_8009FB34 publish as well lifts c
   by three references and restores c, p, d, b = $s0, $s1, $s2, $s3. Priced
   both ways: p's bump alone 69 differing, c's bump alone 70, neither 70,
   both 0. Macro residue rather than device -- each wrapped block is one
   coherent unit of work.

   THE `m'/`w' PAIR IN CASES 5/6/7 IS ALSO LOAD-BEARING: the D_800814A8 load
   has to be written between the func_800A0818 call and the d + 0x2A store,
   or that store takes the slot the `lw' needs -- 359 words / 19 aligned
   differences with the two statements collapsed. 1 copy (dngfam.py). */
void func_b41_80171058(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *f;
    u8 *t;
    u8 *w;
    s32 m;

    if ((D_80083462 & 0x1000) != 0)
    {
        do
        {
            p[0x9A] = 0xE;
            func_b41_801715F0();
        } while (0);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_801748E0)
        {
            e = D_801748D8;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_801748E0)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801748D8) != 0)
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
            p[0x9A] = 0xE;
        }
        f = *(u8 **)(c + 0x2C);
        e2 = D_80174880;
        if (f != e2)
        {
            *(u8 **)(c + 0x2C) = e2;
            func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            c[5] = 1;
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB2) = 0;
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174890) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB2) = 0;
            func_b41_80173834(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    do
    {
        n = func_8009FB34(c[0x24], c[0x25]);
        c[0x26] = n;
    } while (0);
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b41_80171848(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b41_80172414(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b41_80171848(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 9:
            t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
            *(u8 **)(p + 0xA8) = t;
            if (t == NULL)
            {
                func_b41_80171848(p, b, c, d);
                return;
            }
            if (func_800C7F68(t) != 0)
            {
                func_b41_80171848(p, b, c, d);
                return;
            }
            if ((*(u8 **)(p + 0xA8))[0x13] >= 0x33)
            {
                func_b41_80171848(p, b, c, d);
                return;
            }
            func_b41_801722E0(p, b, c, d);
            return;
        case 8:
            if ((s16)func_b41_80171FF4(p, b, c, d) != 0)
            {
                return;
            }
            func_b41_801721B8(p, b, c, d);
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
            func_800AAF00(p, b, c, D_801748C8, func_b41_80171058);
            return;
        default:
            func_b41_80171848(p, b, c, d);
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
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   landed func_b07_80171570 in csrc/ovl_dng_b07.c (itself the resident
   overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c): advance the actor
   one cell of its queued path, swap in the D_80174880 facing table,
   re-aim the sprite, and either stop at state 0xF or hand on to
   func_b41_80171F90.  150 words, 0 differing.  12 copies (dngfam.py).

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
   the three `j's, and the `%hi'/`%lo' pairs for D_80174880, D_80083228
   and the two D_80083462 loads -- so no masked word is hiding a constant
   address.  Hoisting D_80174880 into a local and writing the 0x8A bump
   as `*(s16 *)(d + 0x8A) += 1' are each 0 differing too; the forms kept here
   are b07's, for the replicator's sake. */
void func_b41_801715F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80174880)
        {
            *(u8 **)(c + 0x2C) = D_80174880;
            func_80047784(c, D_80174880[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
                func_b41_80171F90(p, b, c, d);
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
   AND PASSED ON. func_b41_80171FF4's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A66C 2 words, func_b41_80171FF4 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b41_80171848(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b41_80171FF4(p, b, c, d) == 0)
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
   out of the bank-local byte table D_80174898: the relinked twin of the
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
     the local assigned inside the store, `= t = D_80174898'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80174898 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b41_80171F90(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80174898;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174888 facing table and re-aim through func_8009C93C.

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
s32 func_b41_80171FF4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174888;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* State 0x11's entry: clear the record's +0x71 top bit, and unless the
   D_80083462 0x2000 gate is up or func_800A2B5C reports the record busy,
   step it forward through func_800C7930, then -- if it is still not busy --
   mark state 0x11/0, clear the +0x8C word, hang the bank-local D_801748B8
   facing table off the renderable's +0x2C, re-key the sprite through
   func_80047784, spend one of the record's +0x6D charges and re-aim with
   func_8009C93C, raise the owner's +0x98 0x8 bit and set the +0x84 timer to
   0x7C.  74 words, 0 differing.

   The twin is csrc/ovl_dng_b33.c's func_b33_80171FCC (113 words), which is
   this function with a func_800A04F0/func_800A2CB8 reach test, a second
   D_80083460 gate and a `return -1` chain in front of it; the whole of this
   function's gate chain, the func_80047784 byte-table index and the
   `d[0x6D] -= 1;` BEFORE the func_8009C93C call are taken from it unchanged
   and re-measured here.  This one is `void` -- the target leaves $v0 dead at
   every exit -- so the three gates are plain `return;`s sharing the
   epilogue and none of the twin's return-value plumbing appears.

   ONE SPELLING FACT, and it is a store order: `*(s32 *)(p + 0x8C) = 0;`
   comes BETWEEN `p[0x9A] = 0x11;` and `p[0x9B] = 0;`.  Written the twin's
   way, with the +0x8C word last of the three, the word count and every
   opcode are already right and the `sw $zero, 0x8C($s1)` sits one slot late
   -- 74 words, 2 aligned differences. */
void func_b41_801721B8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_801748B8;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    *(u16 *)(p + 0x98) |= 0x8;
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

/* State 0x17 of the +0x9A machine: clear the record's +0x71 top bit, refuse
   on the D_80083460 + 0x2 0x2000 flag and on func_800A2BDC, then park the
   actor at state 0x17/0, dress the renderable from D_80174880, drop
   d + 0x6D, bump D_80083460 + 0xA and print the two-line message through
   the func_800990FC / func_80099734 / func_80099194 / func_80099290 /
   func_800A5720 chain. 77 words, 0 differing, ON THE FIRST COMPILE. 1
   member (dngfam.py). It owns no jump table.

   IT IS WHAT READS D_b41_80170838 AND D_b41_80170848, the two const arrays
   already defined further down this file. Their definitions sit AFTER this
   function, so the declaration block now carries
   `extern const u8 D_b41_80170838[];' / `...80170848[];' forward
   declarations; the definitions and their position are untouched.

   TWO TWINS STITCHED. The head is dngtwin's func_b29_8017294C
   (csrc/ovl_dng_b29.c, 68 words, 0.731): the same +0x71 mask, the same
   refusal pair, the same p + 0x9A / p + 0x8C / p + 0x9B park and the same
   dress-plus-`d[0x6D] -= 1'. The tail is the message chain of the landed
   func_b40_80173C20 (csrc/ovl_dng_b40.c), which is where `m' holding the
   first result comes from. Deltas off the listing: the state constant is
   0x17, the facing table is D_80174880, b29's `func_800A4ACC(d)' and
   `*(u16 *)(p + 0x98) |= 8' are absent, the refusal is func_800A2BDC and
   not func_800A2B5C, the 0x2000 test goes through the pointer local rather
   than b29's `D_80083462' scalar, and the second func_80099734 is handed
   `*(u8 **)(p + 0xA8)'.

   PRICED HERE, BOTH SPELLINGS:

     as written below                                      77 words /  0
     `g' dropped: the 0x2000 test on the `D_80083462'
       scalar b29 uses and the +0xA site written inline     74 words / 19
     `d[0x6D] -= 1' moved after the g + 0xA bump            77 words / 10
     `m = h' dropped, `m' taken straight from
       func_800990FC and `h' from the first func_80099734   77 words /  2
     the func_800A2BDC test without its (s16) cast          76 words /  2

   The `g' row is the one to keep: b29's body reaches +0x2 through the
   D_80083462 scalar because it never touches +0xA, and this body does, so
   the single `u8 *' local holding &D_80083460 across both sites is what
   wins it $s1. That is the same lever as func_b41_801730B4's five-locals
   row above, pointing the other way, because there the arms are disjoint.

   MEASURED AND FREE: the facing table held in a `u8 *' local rather than
   named directly (77 / 0) -- the third body in this unit on which that
   fact costs nothing. */
void func_b41_801722E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    void *m;
    void *h;

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2BDC(d) != 0)
    {
        return;
    }
    p[0x9A] = 0x17;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    *(u8 **)(c + 0x2C) = D_80174880;
    func_80047784(c, D_80174880[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    *(u16 *)(g + 0xA) += 1;
    h = func_800990FC();
    m = h;
    h = func_80099734(d, h);
    h = func_80099194((u8 *)D_b41_80170838, h);
    h = func_80099734(*(u8 **)(p + 0xA8), h);
    h = func_80099194((u8 *)D_b41_80170848, h);
    func_80099290(h);
    func_800A5720(m);
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
s32 func_b41_80172414(u8 *a, void *b, void *c, s32 d)
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
        func_b41_80171848(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b41_80171848(p, b, c, p);
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

/* The banked room script's two-gate installer, the relinked twin of the
   resident overlay's func_800D9884 (csrc/ovl_dungeon_800D50FC.c): the same
   func_800AB1C0 gate, the same func_800AD594 / func_800A4ACC pair, the same
   `blez'-to-the-epilogue inner gate, and the same D_80083462 & 0x80 tail.
   Two deltas, both read off the listing: this copy does NOT clear p + 0x90
   after installing the handler, and its flag arm writes -0x20 to p + 0x92
   where the resident writes 0. The handler is the bank-local
   func_b41_80171058. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b41_80172558(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b41_80171058;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = -0x20;
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
void func_b41_801725EC(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        }
    }
}

/* The same four-call tail the landed func_b45_801726C4
   (csrc/ovl_dng_b45.c) and func_b37_80172A7C run -- func_800A2B04,
   func_800AD594, the +0x8C handler park, D_8008346C = 0, func_800A4ACC and
   the +0x6D two-arm -- in front of a FOUR-state +0x9B machine instead of
   their three. 121 words, 0 differing on the first compile. 2 copies
   (dngfam.py).

   What this copy has that func_b45_801726C4 does not: no leading
   `--*(s16 *)(p + 0x96)'; a state 0 that is only `p[0x9B] = 1' and falls
   through; this bank's D_801748B8 facing table published through
   func_80047784; three b zero-stores and a `*(s16 *)(p + 0x96) = 0' on the
   state-1 else path; a state 2 whose gate is the countdown itself; and
   `p[0x9B] += 1' in place of the `0xFF' terminal state -- which is why
   states 1 and 2 share one `addiu'/`sb' pair by cross-jumping.

   STATE 2's COMPARE IS AGAINST A LITERAL 2 EVEN THOUGH THE TARGET SPELLS IT
   `bne $v0, $v1'. $v1 still holds the dispatch's `lbu 0x9B($s1)' and cse
   knows it equals 2 inside the arm, so the literal is folded onto the live
   register. The increment is the value of the test --
   `if ((*(s16 *)(p + 0x96) += 1) == 2)' -- which is what produces the
   `sll 16'/`sra 16' sign-extension of the stored halfword.

   The +0x6D arm order is func_b45_801726C4's documented fact and it holds
   here: `== 0' with the +0x46 mask as the FIRST arm.
   THIS COPY'S OWN CONSTANT: 0x804 -> 0x808.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b41_80172650(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (p[0x9B])
    {
    case 0:
        p[0x9B] = 1;
    case 1:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            p[0x9B] = 3;
            *(u16 *)(c + 0x14) |= 0x6000;
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(u8 **)(c + 0x2C) = D_801748B8;
        func_80047784(c, D_801748B8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(s16 *)(p + 0x96) += 1) == 2)
        {
            func_800A56E0(0x808);
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            p[0x9B] += 1;
        }
        return;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_800A2B04(b, c[0x24], c[0x25]);
        func_800AD594(d, 0x100);
        *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
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
        return;
    }
}

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b41_801722E0. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b41_80170838[] = {
    0x81,
    0x40,
    0x82,
    0x94,
    0x82,
    0x8F,
    0x82,
    0x8F,
    0x82,
    0x8B,
    0x81,
    0x40,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b41_801722E0. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b41_80170848[] = {
    0x81,
    0x40,
    0x82,
    0x81,
    0x82,
    0x97,
    0x82,
    0x81,
    0x82,
    0x99,
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks
   the slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0,
   state 1 waits on func_8003F270, state 2 waits for the c + 0x14 window and
   then re-installs func_b41_80171058. OWNS A JUMP TABLE: jtbl_b41_80170858,
   seven entries for state 0's FIRST switch -- cases 1,2,3,5,6,7 over the
   0x3FFF field with case 4 falling to `default' -- followed by one .word 0
   of .align 3 padding that the ROM carries. The second switch (the
   (d + 0x1C) & 0x2000 `else') has three cases and compiles to a compare
   chain, which is why the unit gets one new table here and not two.
   299 words, 0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED func_b35_80172AA8 (csrc/ovl_dng_b35.c), 316 words,
   and ALL FOUR of its named facts HOLD here. Re-measured on this body, with
   the rejected spelling's price beside each:
     `k' as `s32' instead of `s16'                298 words /  7 aligned
     the inner three-case switch written 1,2,3    301 words / 10 aligned
     `D_8008346C = 0' after the +0xA6 decrement   300 words /  9 aligned
     the D_8006DE24 record without its pointer
       and index locals, inline as
       &D_8006DE24[g[0]]                          299 words /  1 aligned
   The last of those is all but FREE here where b35 priced it at 317 words /
   10 aligned, so it is the one of the four a later copy should re-measure
   rather than inherit; the other three reproduce b35's aligned counts
   exactly.

   The 17 words this copy is SHORTER than b35 are all in state 2: the
   c + 0x14 gate's body is only `*(u16 *)(p + 0x98) |= 0x80', with no
   `|= 0x800' and no `*(s16 *)(p + 0x96) = 32', and the whole p + 0x96
   countdown that follows the gate in b35 is absent. That gate also tests
   `*(s8 *)(c + 0x4) == 3' where b35 tests 5, its facing table is
   D_80174888, and its handler is func_b41_80171058.

   ONE DELTA IS AN ORDER, NOT A CONSTANT: this copy calls func_800DA840
   BEFORE func_800A56E0(0x703) where b35 calls them the other way round.
   Written b35's way it is 299 words / 7 aligned differences, so the swap is
   real and cheap to get wrong. 1 copy (dngfam.py). */
void func_b41_80172834(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800DA840(&t, (g[0] - 1) % 3);
            func_800A56E0(0x703);
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
            *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
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
        if ((*(s8 *)(c + 0x4) == 3 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) != D_80174888)
        {
            *(u8 **)(c + 0x2C) = D_80174888;
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
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
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

/* The banked room script's three-state glide on the actor's +0x9B: state 0
   seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair, state 1
   steers and runs the +0x96 countdown, state 2 divides onto the tile centre
   and re-installs func_b41_80171058. 204 words, 0 differing, ON THE FIRST
   COMPILE. 1 member (dngfam.py). It owns no jump table -- its listing has no
   .section .rodata.

   The twin is the landed func_b40_80172FE4 (csrc/ovl_dng_b40.c), also 204
   words, dngtwin 0.956, same frame (-0x28, {s0,s1,s2,s3,ra}). THREE OPERAND
   DELTAS, all read off the listing: state 1's step shifts by 10 and not
   b40's 14 (`sll $v0, $v0, 10'); func_800AAA54's fourth argument is this
   bank's D_801748C0; and the installed handler is func_b41_80171058.

   ALL SIX OF b40'S PRICED FACTS HELD AND EVERY ONE REPRODUCED ITS RECORDED
   NUMBER TO THE DIGIT (aligndiff.py on this body, both spellings each time):

     as written below                                     204 words /  0
     the quarter decay at the HEAD OF CASE 1 (b55's
       position) instead of the tail of case 0            204 words /  3
     `g = (u8 *)&D_80083460' dropped, the symbol written
       inline at both tail sites                          203 words /  5
     the `? 8 : -1' ternary written as an if/else          206 words /  9
     case 1's countdown as `if (--x >= 0) { }'            204 words / 10
     case 2's SECOND numerator split three ways like
       the first                                          205 words / 12

   MEASURED AND FREE, so none of these is why this lands: the `goto stop'
   into case 1's shared block (the stop block written out in case 0 as well
   is also 204 / 0, so b40's inherited device costs nothing here either);
   b->unk_0C / unk_10 as plain `*(s32 *)((u8 *)b + 0xC)' casts (204 / 0);
   the seeds as `* 0x8000' rather than `<< 15' (204 / 0); and state 1's step
   as `* 0x400' rather than `<< 10' (204 / 0). The last two matter because
   the same multiply/shift choice is NOT free in func_b41_80173A08 below --
   see its comment. */
void func_b41_80172CE0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
            b->unk_0C -= b->unk_0C / 4;
            b->unk_10 -= b->unk_10 / 4;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C += D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 10;
        b->unk_10 += D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 10;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) -= 1;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_801748C0);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 15) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 15) / *(s16 *)(p + 0x96);
        }
        if (--*(s16 *)(p + 0x96) > 0)
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
        *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b41_80173010(void)
{
    func_800AD058();
}

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile.
   THIS COPY'S OWN CONSTANT: 0xa2 -> 0xa6.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b41_80173030(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xa6) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b41_8017305C(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b41_8017307C(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C: state 0 re-dresses the renderable from D_801748E0, state 1 is
   the long refusal chain with the D_801748E8 dress/flag/bump tail written
   out TWICE, state 2 hands the actor back to func_b41_80171058. 267 words,
   0 differing, ON THE FIRST COMPILE. 1 member (dngfam.py). It owns no jump
   table.

   THE BRIEF'S NAMED TWIN WAS THE WRONG ONE. dngtwin offered
   func_b38_8017357C at 0.848; the body that lands this is the landed
   func_b29_80173DD4 (csrc/ovl_dng_b29.c, 254 words), which dngtwin did not
   rank at all. It was found off the 0x80000 arm: that arm has a six-word
   block between func_800AA888 and the bank-local hand-over -- p + 0x92
   reduced by p + 0xA6, then two halfword clears -- and grepping
   `0x92) -=' over the landed side names b29 at once. b38's body has a
   single `*(s16 *)(p + 0xA0) = 0' there, a `*(s16 *)(p + 0x96) = 3' this
   listing has not got, and a much longer state 2. Frames agree with both
   (-0x28, {s0,s1,s2,s3,s4,ra}); the alias split is a third one again
   (a0->$s2, a1->$s3, a2->$s1, a3->$s0 against b29's and b38's), and as
   batch 14 predicts that cost nothing.

   Deltas from b29, read off the listing: the facing tables are D_801748E0
   and D_801748E8; the 0x80000 arm's second clear is p + 0xB2 for b29's
   p + 0xAC; the gate and the state-2 tail test `*(u16 *)(g + 0x2) & 0x2000'
   through the pointer local rather than b29's `D_80083462' scalar; all
   three park sites carry `*(s32 *)(d + 0x1C) &= ~0x200' which b29 has not
   got; and b29's four func_b29_80175060 calls in state 0's and state 2's
   refusal arms are absent -- both arms are a bare `return'.

   PRICED HERE, BOTH SPELLINGS, aligndiff.py:

     as written below                                     267 words /  0
     all five &D_80083460 pointer locals collapsed
       into one hoisted above the switch                  260 words / 35
     `p[0x9B] += 1' reached by ONE `goto bump' instead
       of written out at all three sites                  265 words / 32
     the 0x80000 arm's p + 0xA6 clear BEFORE the
       p + 0x92 step rather than after it                 264 words / 12
     the first func_80042900 test without its (s16) cast  266 words /  3

   The five-locals row holds b05_80173BEC's and b38_8017357C's direction
   (they record 160 and 91) at this body's own price, and the goto-bump row
   holds b05's and b38's (177 and 17) likewise.

   THE DRESS-SITE FACT IS FREE ON THIS BODY, IN BOTH OF ITS USUAL FORMS, AND
   ONLY THE THIRD SPELLING COSTS. b05_80173BEC records 6 differing for the
   facing table held in a `u8 *' local rather than named directly. Here:
   named directly at all three sites is 267 / 0, ONE LOCAL PER SITE is also
   267 / 0 -- and ONE SHARED local for the two D_801748E8 sites is 267 / 28.
   So on a three-site body the two options the root dispatchers argue about
   are indistinguishable and the sharing is what you must not do. Do not
   inherit the 6.

   MEASURED AND FREE, so none of these is why this lands: the
   `*(s16 *)(d + 0x64) != 0 && func_800AA6B4(...)' pair split into two
   nested `if's (267 / 0, reproducing b38's free); the `v' local dropped and
   both d + 0x1C tests written inline (267 / 0, also b38's free); the
   p + 0x92 step through (s16) loads instead of (u16) (267 / 0); and state
   2's gate arms the other way round (267 / 0 -- unlike b29, whose `== 0'
   arm carries four calls and which prices the same inversion at 57, this
   body's `== 0' arm is a bare `return', so the two spellings are one
   program and the 0-versus-0 means NOT MEASURABLE, not free). */
void func_b41_801730B4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801748E0;
        func_80047784(c, D_801748E0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_801748E8;
            func_80047784(c, D_801748E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
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
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA6);
            *(s16 *)(p + 0xA6) = 0;
            *(s16 *)(p + 0xB2) = 0;
            func_b41_80173834(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801748E8;
        func_80047784(c, D_801748E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
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
        *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_801748E0 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b41_80171058. The landed
   func_b05_80173BEC (csrc/ovl_dng_b05.c) is the twin -- like this copy it
   opens state 1 with the same d[0x25] test that closes it, so the
   dress/flag/bump tail is written out twice, and it carries the same
   `*(s32 *)(d + 0x1C) |= 0x40000' at both dress sites. Read its comment for
   the mechanism: the facing table named directly at all three sites rather
   than held in a `u8 *' local (6 differing there), `&D_80083460' as separate
   pointer locals per arm (160 differing shared there), and the `p[0x9B] += 1'
   bump as a `goto' label inside the switch (177 differing after the switch
   there).

   Two deltas from that twin, both read off the listing: state 2 clears only
   bit 3 of d + 0x1C (`addiu $v1, $zero, -0x9') where func_b05_80173BEC
   clears 0x208, and the 0x80000 arm has a six-word block between
   func_800AA888 and the bank-local func_b41_80173834 -- p + 0x92 reduced by
   p + 0xA2 as halfwords (both reads `lhu', so both are unsigned), then
   p + 0xA2 and p + 0x9E cleared. p + 0x92 is READ BEFORE p + 0xA2 IS
   CLEARED in the listing, which is the order the three statements are
   written in. 213 words, 0 differing, on the first compile; the six-word
   block and the `~8' account for the 213 against the twin's 207.
   THIS COPY'S OWN CONSTANT: 0xa2 -> 0xa6 at 2 sites, 0x9e -> 0xb2, -0x9 -> -0x209 (as ~0x8 -> ~0x208).
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b41_801734E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801748E0;
        func_80047784(c, D_801748E0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_801748E8;
            func_80047784(c, D_801748E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xa6);
            *(s16 *)(p + 0xa6) = 0;
            *(s16 *)(p + 0xb2) = 0;
            func_b41_80173834(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801748E8;
        func_80047784(c, D_801748E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(s32 *)(d + 0x1C) &= ~520;
        *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
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
   call emits the bare `jal` either way); hoisting D_80174880 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b41_80173834(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_801748E0 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80174880;
        func_80047784(c, D_80174880[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b41_801738FC(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        }
    }
}

/* 14 words, 0 differing. */
void func_b41_80173960(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}

/* 14 words, 0 differing. */
void func_b41_80173998(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}

/* 14 words, 0 differing. */
void func_b41_801739D0(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}

/* The same glide one state longer: a FOUR-state +0x9B machine. State 0
   dresses from D_801748A0 and breaks, state 1 ramps p + 0x90 and dresses
   from D_801748A8 before falling through, state 2 divides onto the tile
   centre and runs the p + 0xB0 sub-counter, state 3 waits on the d + 0x1C
   0x8000000 flag, and the shared tail runs the p + 0x96 countdown and
   retires the actor onto func_b41_80171058. 279 words, 0 differing, ON THE
   FIRST COMPILE. 1 member (dngfam.py). It owns no jump table.

   The twin is the landed func_b40_8017240C (csrc/ovl_dng_b40.c, 223 words,
   dngtwin 0.821), itself func_b36_801723D4's copy of the resident
   func_800D991C. THE 56-WORD GAP IS STATEMENTS, NOT REFERENCES: a whole
   extra state. Deltas, all read off the listing -- case 0 ends in `break'
   where b40 falls through; case 1 gains `p + 0x90 += 0xC0000' at its head
   and `p + 0x90 -= 0x400000' at its tail; the sub-counter is p + 0xB0 for
   b40's p + 0xA4; the p + 0xA0 step gains its `y + (y >> 1)' half; the
   facing tables are D_801748A0 / D_801748A8 / D_801748B0; and the tail's
   `if ((s16)func_800AD9B4(c, d) > 0)' gate is GONE -- the park and the
   func_800A9A04 call are unconditional.

   ALL THREE OF b40'S PRICED FACTS HELD, again at their recorded numbers,
   across the state-count change:

     as written below                                     279 words /  0
     the tail gate's 0x2000 arm as the `else'             279 words / 17
     the 0x410 and 0x20000 tests folded into one `&&'     279 words / 10
     case 2's FIRST numerator as one expression           279 words /  1

   THIS BODY'S OWN FACTS, both spellings priced:

     `* 409' spelled as the sll/addu/sll decomposition
       the listing shows (pure shifts and adds, no
       MULT_EXPR anywhere)                                279 words / 11
     `u = c[0x24] * 64' instead of `u = c[0x24] << 6'
       at case 2's FIRST numerator                        279 words /  3
     the two D_801748B0 dress sites sharing ONE `u8 *'
       local                                              279 words / 20
     case 0 falling through into case 1                   278 words /  5
     the p + 0xA0 step as `(y * 3) >> 1'                  280 words /  7
     the p + 0xA0 step as `y - (-y >> 1)'                 280 words /  7

   THE MULTIPLY ROW IS A REAL REFINEMENT OF THE POWER-OF-TWO RULE. `* 64'
   and `<< 6' select the same `sll', but at the FIRST numerator -- where the
   shift is a statement of its own assigning the s32 local `u' -- the
   MULT_EXPR form displaces the `lbu'/`sll' pair one slot in sched1 and
   costs 3 rows; at the SECOND numerator, inline in a larger expression,
   `c[0x25] * 64' is 279 / 0. Same constant, same function, free at one site
   and not the other, so price it per site. No narrowing context occurs
   anywhere in this body and convert.c never bit: there is no `lhu' where
   the target has `lh' and no constant materialised into a register.

   MEASURED AND FREE: the p + 0xA0 step as `y + y / 2' (279 / 0 -- the
   `<< 10' leaves the dividend provably even, so the signed-divide bias
   correction is dead and both spellings emit the bare `sra'); one `u8 *'
   local PER dress site at all four sites (279 / 0); case 1's p + 0x90 step
   before the p + 0xA0 clear (279 / 0); the tail countdown split into
   `-= 1;' plus a separate test (279 / 0); case 2's countdown as
   `if (--x < 0)' (279 / 0); and `&= 0xF7FFFFFF' for `&= ~0x8000000'
   (279 / 0). */
void func_b41_80173A08(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    s32 y;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801748A0;
        func_80047784(c, D_801748A0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0xB0) = 5;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
        break;
    case 1:
        *(s32 *)(p + 0x90) += 0xC0000;
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801748A8;
        func_80047784(c, D_801748A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0xB0) = 5;
        *(s32 *)(p + 0xA0) = 0;
        *(s32 *)(p + 0x90) -= 0x400000;
        p[0x9B] += 1;
    case 2:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0xB0) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0xB0);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0xB0);
            y = -func_800644B8(*(s16 *)(p + 0xB0) * 409) << 10;
            *(s32 *)(p + 0xA0) = y + (y >> 1);
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
        if ((*(s16 *)(p + 0xB0) -= 1) < 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            *(u8 **)(c + 0x2C) = D_801748B0;
            func_80047784(c, D_801748B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
    case 3:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(u8 **)(c + 0x2C) = D_801748B0;
            func_80047784(c, D_801748B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        if ((*(u16 *)(d + 0x46) & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
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
    *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
    func_800A9A04(d);
}

/* The five-state SEQUENCE DRIVER for this bank's pick-up-and-throw: the body
   that walks p[0x9B] 0..4 while the carried actor at *(u8 **)(p + 0xA8) is
   lifted, searched to a free adjacent cell, slid there and handed back to
   func_b41_80171058. State 0 mirrors the carried record's flags into p+0x98
   and dresses the slot from D_801748F0; state 1 runs the p+0x96 timer down,
   zeroes the mover at 8 and re-arms it from D_801748F8; state 2 drops the
   mover, then retries func_800A4E2C up to 0x3F times for a cell that
   func_8009A2B8 accepts and whose func_800BCB04 height matches the carrier's,
   restoring the saved +0x24/+0x25 if it runs out; state 3 slides +0x92 and
   re-aims the mover off D_8006CCD8/D_8006CCE8 negated; state 4 republishes,
   drops D_80083460's +0xA counter, re-installs func_b41_80171058 at p+0x8C
   and re-dresses from D_80174888. 619 words, 0 differing.

   OWNS A JUMP TABLE: jtbl_b41_80170878 at rodata offset 0x0, five entries for
   p[0x9B] 0..4, ALL FIVE DISTINCT and none of them the default label -- the
   dispatch bound is a bare `sltiu $v0, $v1, 0x5' with no minval subtraction,
   so the set starts at 0 and nothing above 4 reaches the table. jtcheck:
   5 words = 5 cases + 0 pad, bound 5, 5 distinct targets, which is the pad
   rule stated correctly - an odd case count that is last in .rodata has none.
   The entries, in source order, are .Lb41_80173EE0 / .Lb41_801740F8 /
   .Lb41_80174204 / .Lb41_801744F8 / .Lb41_80174668. CASE 0 FALLS THROUGH into
   case 1 after its own `p[0x9B]++', which is why that increment is written
   inside the arm as well as after the switch, and the default arm is a bare
   `return' so an out-of-range state does NOT get the increment.

   THERE IS NO TWIN AT ANY RATIO. dngtwin at a 0.30 floor and xsibs at 0.70
   over 3,366 landed functions both return nothing, and the callee-set
   intersection over func_800AA53C / func_800A4E2C / func_8009A2B8 /
   D_8008146E / D_800DDC40 names only the resident definitions. Everything
   below was read off the listing and priced on this body; nothing is
   inherited.

   THE FRAME IS THE FIRST FACT: `addiu $sp, -0x48' with $s0..$s7, $fp and $ra
   saved, and `sw $a3, 0x54($sp)' -- THE FOURTH PARAMETER LIVES IN ITS
   INCOMING SLOT and is reloaded fifteen times. That spill is sixteen of the
   619 words, so a body that keeps `d' in a register comes out thirteen short
   with a whole-body register rotation, and it is the thing to get right
   first. Nine callee-saved registers hold exactly nine values -- hit, r, p,
   k, q, qa, n, b, c -- and `m', `sx', `sy' and the two loop-invariant table
   addresses take the four local words at 0x10..0x1F.

   | spelling                                          | words | aligned |
   | as written below                                  |  619  |    0    |
   | no `qa' alias, `q' at every site                  |  595  |   229   |
   | three separate facing locals instead of one `k'   |  599  |   213   |
   | `s16 k' instead of `s32 k'                        |  600  |   209   |
   | no reference bump on `p'                          |  619  |   55    |
   | `while (--n > 0)' instead of the `while (1)' form |  604  |   42    |
   | `s16 hit' with no `(s16)' cast on the call        |  620  |   37    |
   | the four divisions by the literal, not p + 0x96   |  623  |   23    |
   | the restore block after the loop, no `timeout'    |  621  |   19    |
   | the +0x24/+0x25 publish written out twice         |  629  |   13    |
   | one table local shared by all four dress sites    |  619  |   11    |
   | the D_8008146E gate spelled `< 2'                 |  619  |    1    |

   EVERY DIVISION DIVIDES BY `*(s16 *)(p + 0x96)', NOT BY THE CONSTANT IT WAS
   JUST SET TO, and the listing says so outright: `addiu $v1, $zero, 0xC'
   followed by `div' is cse CONSTANT-PROPAGATING a known store into a
   VARIABLE division. A literal divisor never reaches `div' at all --
   expand_divmod strength-reduces it at expand time, four passes before cse,
   and the diagnostic is `lui $a1, 0x2AAA' for the magic multiply. The second
   division of each pair reloads the halfword instead because the intervening
   `sw' through `b' invalidates cse's entry for it. Worth 4 words and 23 rows.

   THE LOOP MUST NOT BE A `while' WITH ITS TEST IN THE CONDITION POSITION.
   jump.c rotates a loop only when the insn after NOTE_INSN_LOOP_BEG is a
   conditional jump, so `while (--n > 0)' is rotated -- `0x40' folds to
   `li 0x3F', the test moves to the bottom, and the five delay-slot
   decrements never appear: 604 words, 42 rows. Written as
   `while (1) { n--; if (n <= 0) goto timeout; ... }' the loop top is a SET,
   the guard fails, and reorg.c:3728 then steals that decrement into every
   back branch's delay slot with a compensating `addiu +1' on the
   fall-through -- five of them, and the sixth has none because `n' is dead
   where the loop falls out. THE COUNTER IS DEAD AT THE EXIT, so none of the
   six adjustments is source; writing one is wrong.

   THE RESTORE BLOCK IS NOT AFTER THE LOOP. It sits at 0x801741EC, between
   case 1's `j' and case 2's own jump-table entry at 0x80174204, so it is
   reached only by `goto' and its label is written between the two arms. gcc
   emits a loop's exit code after the loop, so written there it is 621 words
   and 19 rows. The loop's `if (n <= 0) goto timeout' then needs no threading.

   `s32 hit' WITH AN EXPLICIT `(s16)' CAST, NOT `s16 hit'. With the narrow
   declaration gcc keeps TWO values -- the raw call result for the two byte
   stores and the sign-extended one for the three comparisons -- and pays a
   `move' to carry the raw across the func_8009A2B8 call: 620 words, 37 rows,
   with the extra word being that `move'. The single SImode pseudo holding the
   sign-extension is what the target has, and it is also what lets `hit' win
   $s0 outright at 3*10/16.

   THE REFERENCE BUMP ON `p' IS THE ONE DEVICE, AND ITS SCOPE IS NOT
   MEASURABLE. Without it the body is 619 words with 55 aligned differences,
   every row a uniform `p'/`k' exchange of $s2 and $s3 with the instruction
   order identical -- a WRONG PRIORITY, not a wrong pseudo count, since no
   `move' is missing and the register mention counts are the target's exactly
   with those two swapped. Measured off `-da': global.c's allocno_compare
   ranks on floor_log2(refs)*refs/live_length, and this body's `p' scores
   5*50/988 = 2530 against the shared facing local `k' at 3*8/89 = 2696, so
   `k' is allocated one position too early and takes $s2. flow.c:2122 counts
   a reference once per ENCLOSING LOOP, so a `do { } while (0)' doubles what
   it wraps; `p' needs four more references to clear 2696 and the +0x96 timer
   tick carries two. THREE DIFFERENT SCOPES ALL MEASURE 0 -- all four ticks
   wrapped, ticks two and three only, and tick one plus the tail `p[0x9B]++'
   -- so the target does not constrain the choice and the kept scope is a
   guess. All four are wrapped below because that statement appears four
   times unchanged, which reads as macro residue rather than a device; two
   p-only wrappers worth +2 each are what the arithmetic actually asks for.
   A single wrapper is 55, i.e. +2 buys nothing, which is the power-of-two
   cliff doing what it always does.

   ONE TABLE LOCAL PER DRESS SITE. The four `*(u8 **)(c + 0x2C) = e' sites
   each need their own pointer local: shared, the one local becomes a global
   allocno spanning the whole switch and costs 11 rows. D_80082E80 and
   D_80083460 likewise come through pointer locals, because the listing's
   `lui'/`addiu' then offset is the pointer-value shape and a plain
   `*(s8 *)(D_80082E80 + 0x26)' would emit the two-insn symbolic form.

   MEASURED AND FREE, so neither is why this lands: `s16 sx'/`s16 sy' in
   place of `u16' (619/0 -- the `sh' and the `lbu' are the same either way),
   and the loop's last test written `if (!=) continue; goto found;' rather
   than `if (==) goto found;' (619/0). The second one is worth recording
   because it WAS worth 8 rows mid-way, while two other faults were still
   live; once they were fixed both spellings reached 0. An improvement is not
   evidence. 1 copy (dngfam.py). relcand: ok, 10 data symbols and 17 calls. */
void func_b41_80173E64(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkVec *m;
    u8 *q;
    u8 *qa;
    u8 *r;
    u8 *e;
    u8 *e2;
    u8 *e3;
    u8 *e4;
    u8 *g;
    u8 *g2;
    s32 k;
    s32 n;
    s32 hit;
    u16 sx;
    u16 sy;

    q = *(u8 **)(p + 0xA8);
    qa = q;
    r = *(u8 **)(q - 0x14);
    m = *(UnkVec **)(q - 0x18);
    switch (p[0x9B])
    {
    case 0:
        *(u32 *)(p + 0xAC) = *(u32 *)(qa + 0x8C);
        if ((*(s32 *)(q + 0x1C) & 0x40000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x1000;
        }
        else
        {
            *(u16 *)(p + 0x98) &= 0xEFFF;
        }
        if ((*(u16 *)(qa + 0x98) & 0x8) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x4000;
        }
        else
        {
            *(u16 *)(p + 0x98) &= 0xBFFF;
        }
        if ((*(u16 *)(qa + 0x98) & 0x4) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x2000;
        }
        else
        {
            *(u16 *)(p + 0x98) &= 0xDFFF;
        }
        *(u32 *)(qa + 0x8C) = 0;
        r = *(u8 **)(q - 0x14);
        func_8009A3D0(r[0x24], r[0x25], ((*(s32 *)(q + 0x1C) & 0x2000) != 0) ? 0x300 : 0x3000);
        func_8009A3D0(c[0x24], c[0x25], ((*(s32 *)(d + 0x1C) & 0x2000) != 0) ? 0x300 : 0x3000);
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        *(u16 *)(p + 0x98) |= 0xC;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0 && (*(u16 *)(r + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 2;
            return;
        }
        *(s16 *)(p + 0x96) = 0xC;
        b->unk_0C = (D_8006CCD8[k] << 22) / *(s16 *)(p + 0x96);
        b->unk_10 = (D_8006CCE8[k] << 22) / *(s16 *)(p + 0x96);
        b->unk_14 = 0;
        *(s32 *)(q + 0x1C) &= 0xFFFBFFFF;
        *(u16 *)(qa + 0x98) |= 0xC;
        e = D_801748F0;
        *(u8 **)(c + 0x2C) = e;
        func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        /* fallthrough */
    case 1:
        do
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        } while (0);
        if (*(s16 *)(p + 0x96) == 8)
        {
            b->unk_14 = ((*(s16 *)((u8 *)m + 0xA) - *(s16 *)(d + 0x88) - D_800DDC40[q[0x13]] + 0x10) << 16) /
                        *(s16 *)(p + 0x96);
            *(s16 *)(qa + 0x92) = *(u16 *)(qa + 0x92) + *(u16 *)(q + 0x88);
            *(s16 *)(q + 0x88) = 0;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_10 = 0;
        b->unk_0C = 0;
        b->unk_14 = -0x40000;
        func_800A56E0(0x802);
        *(s16 *)(p + 0x96) = 0x10;
        e2 = D_801748F8;
        *(u8 **)(c + 0x2C) = e2;
        func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        break;
    timeout:
        r[0x24] = sx;
        r[0x25] = sy;
        goto found;
    case 2:
        b->unk_14 += -0x20000;
        *(s16 *)(qa + 0x92) = *(u16 *)((u8 *)b + 0xA) + D_800DDC40[q[0x13]] - 0x10;
        do
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        } while (0);
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(r + 0x14) & 0x8000) == 0)
        {
            return;
        }
        sx = r[0x24];
        sy = r[0x25];
        k = ((*(s16 *)(d + 0x2A) + 0x800) & 0xFFF) >> 9;
        n = 0x40;
        while (1)
        {
            n--;
            if (n <= 0)
            {
                goto timeout;
            }
            hit = (s16)func_800A4E2C(&r[0x24], &r[0x25]);
            if (hit < 0)
            {
                continue;
            }
            g2 = D_80082E80;
            if (hit == *(s8 *)(g2 + 0x26) && D_8008146E >= 2)
            {
                continue;
            }
            if ((s16)func_8009A2B8(r[0x24], r[0x25], k) != 0)
            {
                continue;
            }
            r[0x26] = hit;
            c[0x26] = hit;
            c[0x24] = r[0x24] + D_8006CCD8[k];
            c[0x25] = r[0x25] + D_8006CCE8[k];
            *(s16 *)(d + 0x88) = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                                               (s16)(*(u16 *)((u8 *)b + 0xA) - 0x80));
            if (*(s16 *)(d + 0x88) > 0x200)
            {
                continue;
            }
            *(s16 *)(q + 0x88) = func_800BCB04((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20,
                                               (s16)(*(u16 *)((u8 *)b + 0xA) - 0x80));
            if (*(s16 *)(q + 0x88) > 0x200)
            {
                continue;
            }
            if (*(s16 *)(d + 0x88) == *(s16 *)(q + 0x88))
            {
                goto found;
            }
        }
    found:
        b->unk_00 = m->unk_00 = ((r[0x24] << 6) + 0x20) << 16;
        b->unk_04 = m->unk_04 = ((r[0x25] << 6) + 0x20) << 16;
        b->unk_08 = (*(s16 *)(q + 0x88) - 0x100) << 16;
        *(s32 *)(qa + 0x90) = *(s32 *)(p + 0x90);
        *(s16 *)(p + 0x96) = 8;
        b->unk_14 = ((*(s16 *)(d + 0x88) << 16) - b->unk_08) / *(s16 *)(p + 0x96);
        e3 = D_801748F8;
        *(u8 **)(c + 0x2C) = e3;
        func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        func_800AA53C(d);
        func_800AA53C(q);
        break;
    case 3:
        *(s16 *)(qa + 0x92) = *(u16 *)(p + 0x92) + 0x40;
        if (*(s16 *)(p + 0x92) < 0)
        {
            do
            {
                *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
            } while (0);
            if (*(s16 *)(p + 0x96) > 0)
            {
                return;
            }
            if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
            {
                return;
            }
        }
        *(s32 *)(p + 0x90) = 0;
        *(s32 *)(qa + 0x90) = 0;
        if ((*(u16 *)(p + 0x98) & 0x1000) != 0)
        {
            *(s32 *)(q + 0x1C) |= 0x40000;
        }
        if ((*(u16 *)(p + 0x98) & 0x4000) == 0)
        {
            *(u16 *)(qa + 0x98) &= 0xFFF7;
        }
        if ((*(u16 *)(p + 0x98) & 0x2000) == 0)
        {
            *(u16 *)(qa + 0x98) &= 0xFFFB;
        }
        *(s32 *)(d + 0x1C) |= 0x40000000;
        *(s32 *)(q + 0x1C) |= 0x40000000;
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_14 = 0;
        *(s16 *)(p + 0x96) = 8;
        b->unk_0C = -(D_8006CCD8[k] << 22) / *(s16 *)(p + 0x96);
        b->unk_10 = -(D_8006CCE8[k] << 22) / *(s16 *)(p + 0x96);
        m->unk_14 = 0;
        b->unk_14 = 0;
        break;
    case 4:
        do
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        } while (0);
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(r + 0x14) & 0x8000) == 0)
        {
            return;
        }
        b->unk_00 = ((c[0x24] << 6) + 0x20) << 16;
        b->unk_04 = ((c[0x25] << 6) + 0x20) << 16;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(u32 *)(qa + 0x8C) = *(u32 *)(p + 0xAC);
        func_800AD594(d, 0x100);
        *(u32 *)(p + 0x8C) = (u32)func_b41_80171058;
        g = (u8 *)&D_80083460;
        *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
        func_800A4ACC(d);
        *(u16 *)(p + 0x98) &= 0xFFF3;
        *(s32 *)(d + 0x1C) |= 0x40000;
        func_8009A21C(c[0x24], c[0x25], ((*(s32 *)(d + 0x1C) & 0x2000) != 0) ? 0x300 : 0x3000);
        func_8009A21C(r[0x24], r[0x25], ((*(s32 *)(q + 0x1C) & 0x2000) != 0) ? 0x300 : 0x3000);
        e4 = D_80174888;
        *(u8 **)(c + 0x2C) = e4;
        func_80047784(c, e4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        d[0x6D] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        break;
    default:
        return;
    }
    p[0x9B]++;
}

/* 14 words, 0 differing. */
void func_b41_80174810(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}

/* 14 words, 0 differing. */
void func_b41_80174848(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b41_80171058;
    }
}
