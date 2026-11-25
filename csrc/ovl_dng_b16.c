#include "common.h"

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b16_80171ECC(u8 *p, UnkVec *b, u8 *c, u8 *d);
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

extern s16 D_80083228;
extern u8 D_b16_8017420C[];
extern u8 D_8017425C[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b16_80171784(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC7EC();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b16_80171014();

s32 func_800AB778();

s32 func_800AD058();

s32 func_800AB464();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

s32 func_800AB538();

extern u8 D_80174264[];
s32 func_800A2C34(u8 *);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b16_80173468(u8 *p, u8 *b, u8 *c, u8 *d);

extern u8 D_80174244[];
void func_800A2B04(UnkVec *, s32, s32);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD4D0(u8 *);

extern Unk80083462 D_80083460;
extern s16 D_800DCF5E;

extern s16 D_801742E0;
s32 func_800453E0(u8 *, UnkVec *, u8 *, s16);

s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);

extern u8 D_8017423C[];
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

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
void func_80044A50(UnkNode *);
extern s32 D_801742E8;

UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
typedef union UnkDungeonSPXY
{
    s32 w;
    u16 h[2];
} UnkDungeonSPXY;
typedef struct UnkDungeonSPA
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x1A];
    u8 *unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[2];
    u8 pad_80[0x40];
    s32 unk_C0;
} UnkDungeonSPA;
typedef struct UnkDungeonG4
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    u32 unk_0C;
    UnkDungeonSPXY unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 pad_17[0x1];
    s32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 pad_1F[0x1];
    UnkDungeonSPXY unk_20;
} UnkDungeonG4;
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b16_80170A58(u8 *p, UnkVec *v, u8 *c);

/* Spawn the kind-0x112 node this bank's scripts use for a thrown or placed
   actor: arm it with the bank-local func_b16_80170A58 handler and the
   D_b16_8017420C facing table, dress its slot from the b/c/d arguments, pick one
   of three flag pairs off `a & 3', and hand it to func_800A9C18 and
   func_800AA36C. 120 words, 0 differing, ON THE FIRST COMPILE.

   The twin is the 113-word func_b03_80170854 (csrc/ovl_dng_b03.c) and every
   one of its spellings held unchanged, so nothing here was fought for; read
   its comment for the shape. Five deltas, all read off the listing:
   `p[0x13]' is 2 rather than 3, the handler and the facing table are this
   bank's func_b16_80170A58 and D_b16_8017420C / D_8017425C, the func_800A6D30 arm
   does NOT set `*(u32 *)(p + 0x1C) |= 0x200' the way the twin's does, and the
   tail gains `*(u32 *)(p + 0x1C) |= 0x40000;' and
   `*(s16 *)(w + 0x92) = -0x20;' before the func_800AA36C call.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x10 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b16_80171784(u8 *p, void *b, void *c, u8 *d);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, u8 *t, void (*h)());
s16 func_800A1C58(u8 *p);
s16 func_8009FB34(u16 x, u16 y);
void func_b16_80171574_0(void) __asm__("func_b16_80171574");
void func_b16_80172090(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b16_801721B4(u8 *a, void *b, void *c, s32 d);
void func_b16_80173AE8(u8 *p, void *b, void *c, u8 *d);
extern u8 D_8017421C[];
extern u8 D_8017424C[];
extern u8 D_80174254[];
s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
void func_800BB044(u8 *);
s32 func_800A56E0(s32);
extern s32 D_8008346C;
s16 func_800A2CB8(u8 *a, u8 *b);
extern u8 D_80174214[];
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);

s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
s32 func_800644B8(s32);
extern u8 D_8006CCF8[];
extern void (*D_8017426C[])(u8 *, UnkVec *, u8 *, u8 *);
void func_80041588(Unk41284 *, s8 *, s32);
void func_8003F540(u32, u32, u32, u32);
u32 func_800445E0(void);
s32 func_800C77D0(void *, u8 *, s16, s32);
s32 func_800498A0(u8 *);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_8003DB94(void *, void *, s32);
UnkNode *func_800ADC4C(u8 *, s32, s16, u8 *);
void func_800A18E8(s16, s16);
void func_8009A028(u8 *);
s32 func_800A504C(u8 *, u8 *);
void func_b16_80173904(u8 *p, UnkVec *b, u8 *c, u8 *d, s32 e, u16 f);
extern Unk41284 D_801742CC[];
extern u8 D_801742E4[];
extern u8 D_801742E5[];
extern s32 D_801742D4[];
extern u8 *D_800DCEEC[];
extern u8 D_800DCF5C;
extern u8 D_8014A000[];
extern u8 D_8003E140[];
extern s32 D_8006CD58;
s32 func_80064584(s32);
extern u8 D_800DEC70[];

u8 *func_b16_80170878(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b16_80170A58;
        p[0x13] = 16;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_b16_8017420C;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_8017425C;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b16_80171014;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x20;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

/* The UnkNode +0x10 PER-FRAME DRIVER for this bank's actor: the relink of the
   landed func_b25_80171964 (csrc/ovl_dng_b25.c, 366 words, dngtwin 0.941).
   Past the engine's D_80083462 0x2000 gate -- where it runs the +0x8C
   continuation only when that continuation is this object's own
   func_b16_80171014 -- it runs func_800A9E70, the record's +0x8C step, the
   +0x9A entry of this bank's D_8017426C handler table, re-aims through
   func_800AA36C when the +0x6D facing moved under it, steps the mover, runs
   the +0x9D bob, re-skins the slot for the eight-way facing, and leashes
   +0x92/+0x88 to the ground height func_800BCB04 reports.
   367 words, 0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED func_b25_80171964 (csrc/ovl_dng_b25.c, 366 words,
   dngtwin 0.941), and the FRAME IS BYTE-IDENTICAL: `addiu $sp, $sp, -0x30'
   with $s0..$s5 and $ra at the same offsets in the same order. FIVE OF ITS
   SIX LIVE FACTS REPRODUCE AT OR WITHIN ONE ROW OF ITS OWN FIGURES, which is
   the strongest agreement recorded in this family:

   | spelling                                       | words | aligned | b25
   | as written below                               |  367  |    0    |  0
   | no `d = p' alias, `p' at every site            |  364  |  147    | 146
   | `lim' inlined as `> -0x20' at both clamp sites |  366  |   15    |  16
   | ONE `w' local shared by both copies of the arm |  367  |    4    |  4
   | one handler local for both +0x8C reads         |  367  |    3    |  3
   | `s32 dir' instead of `s16 dir'                 |  364  |    9    |  9
   | the discarded func_800BCB04h call deleted      |  351  |   24    |  --

   b25's `local-alloc.c:1759' DUPLICATED-ARM LEVER HOLDS HERE AT 4 ROWS, the
   same figure b25 and func_b24_801711B0 record: the 0x40000 arm is written
   out in BOTH arms of the +0x14 0x8000 gate, and the two copies need their
   OWN `w' and `w2'. With one shared `w' the +0x9E increment lands in $v0 at
   both sites where the target has `addiu $v1, $v1, 1'. Note the asymmetry:
   the `lim' local IS shared across the same two copies and is correct that
   way -- the lever is about a pseudo whose value crosses the copies, not
   about every local mentioned inside them.

   ONE OF b25's FACTS IS WEAKER HERE THAN IT STATES. b25 prices `one facing
   local instead of `k' and `i'' at 363 words / 63 aligned differences; on
   this body dropping `i' and indexing off `k' alone is 367 words / 0 aligned
   differences -- FREE. The two locals are kept so the family reads alike,
   but they are not why this lands.

   ALSO MEASURED AND FREE, so NOT reasons this lands: the 0x9D gate's
   `p + 0x1C' read spelled `d + 0x1C' instead (367 / 0 at that one site,
   although dropping the alias everywhere is the 147 above), and
   `*(s32 *)(p + 0xA0) = *(s32 *)(p + 0xA0) + (... << 5)' written out
   longhand instead of `+=' (367 / 0).

   DELTA FROM b25, all read off the listing: the handler is this object's
   func_b16_80171014 and the +0x9A table is D_8017426C; the facing table
   compared in the 0x9E arm is the bank-local D_b16_8017420C where b25 has
   the resident D_800E2348; b25's `*(u16 *)(c + 0x4) == 0x400' reset inside
   the 0x9E arm is ABSENT from both copies; the bob is
   `*(s32 *)(p + 0xA0) += func_800644B8((s16)w * 170) << 5' where b25 has
   `= -func_800644B8((s16)w * 195) << 7' -- a different constant, a different
   shift and no negation; each 0x40000-true clamp arm GAINS a
   func_800BCB04h call whose result is discarded (deleting it is the 24 rows
   above); and b25's `p[0x9A] != 0x18' tail gate is ABSENT -- the +0xA store
   is unconditional here.

   func_800BCB04 COMES THROUGH THE UNIT'S EXISTING `s16' ALIAS
   func_800BCB04h, as func_b16_80171784 above requires; no second alias was
   introduced. No jump table and no rodata of its own -- the listing opens at
   `glabel'.
*/
void func_b16_80170A58(u8 *p, UnkVec *v, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *d;
    s16 dir;
    s32 i;
    s16 k;
    s16 e;
    s16 g;
    u16 w;
    u16 w2;
    s32 lim;

    d = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b16_80171014)
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
    D_8017426C[p[0x9A]](p, v, c, p);
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
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_b16_8017420C)
            {
                w = *(u16 *)(p + 0x9E);
                *(s16 *)(p + 0x9E) = w + 1;
                *(s32 *)(p + 0xA0) += func_800644B8((s16)w * 170) << 5;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                func_800BCB04h(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               (s16)(*(u16 *)(d + 0x88) - 0x20));
                lim = -0x20;
                if (*(s16 *)(p + 0x92) > lim)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < -0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
        else
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
            *(s16 *)(p + 0x9E) = 0;
            *(s32 *)(p + 0xA0) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e = func_800BCB04h(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (e < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = e;
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
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
            *(s16 *)(p + 0x9E) = 0;
            *(s32 *)(p + 0xA0) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e = func_800BCB04h(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (e < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = e;
                    p[0x9D] = 0;
                    v->unk_14 = 0;
                    *(s32 *)(d + 0x1C) |= 0x8000000;
                }
            }
        }
        else
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_b16_8017420C)
            {
                w2 = *(u16 *)(p + 0x9E);
                *(s16 *)(p + 0x9E) = w2 + 1;
                *(s32 *)(p + 0xA0) += func_800644B8((s16)w2 * 170) << 5;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                func_800BCB04h(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               (s16)(*(u16 *)(d + 0x88) - 0x20));
                lim = -0x20;
                if (*(s16 *)(p + 0x92) > lim)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < -0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
    }
    if ((*(s32 *)(d + 0x1C) & 0x40000000) != 0)
    {
        *(s32 *)(d + 0x1C) &= 0xBFFFFFFF;
        g = func_800BCB04h((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                           (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (g < 0x200)
        {
            *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + (*(u16 *)(d + 0x88) - g);
            *(s16 *)(d + 0x88) = g;
        }
    }
    *(s16 *)((u8 *)v + 0xA) = (*(u16 *)(d + 0x88) + *(u16 *)(p + 0x92)) - *(u16 *)(p + 0xA2);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The banked room script's IDLE handler -- the body this unit's other states
   park at the actor's +0x8C, and the one the switch below re-installs through
   func_800AAF00. OWNS A JUMP TABLE: jtbl_b16_80170808, twelve entries for the
   +0x46 state 1..12, with 4, 10 and 11 carrying the default label rather than
   arms of their own. 344 words, 0 differing.

   THE TWIN IS THE LANDED func_b06_8017140C (csrc/ovl_dng_b06.c), 342 words,
   SAME FRAME (addiu $sp, -0x38; $s0..$s3 + $ra, saved in the same order), and
   after the relink the two bodies differ in ONE STATEMENT: here the
   func_800AAB10 arm RETURNS after func_b16_80173AE8 instead of falling into
   the func_8009FB34 tail. Written b06's way the body is 342 words / 8 aligned
   differences, all of them the exit label two words early.

   AND THAT ONE `return' INVERTS b06's FIRST FACT. b06 prices its shared
   default tail both ways and says both are free; here the three written-out
   `func_b16_80171784(p, b, c, d); return;' copies are 344 words / 65 ALIGNED
   DIFFERENCES -- every one a $s0/$s1 exchange between `p' and `c' -- and the
   one shared block the three gates `goto' is 0. Measured cause, from
   `tools/cdkgcc -da': the `return' shortens `c's live length from 480 insns
   to 476, which takes `c' to 170/476 in allocno_compare against the
   D_80082E80 tail pointer's 10/28 -- the same value to four figures, so the
   tie breaks by allocno number and `c' wins $s0. Sharing the tail drops two
   of `c's references and ends the tie. A `do { } while (0)' bump on the
   dress, on its store, on its func_80047784 call and on the whole publish all
   measure 65 as well, because each one lifts `c' and the table local
   together; so does nesting any of the three `&&' gates, dropping `e2' and
   swapping the two +0xA2/+0x9E stores. Three shared-tail shapes measure 0:
   all three gates sharing, and either of the two outer gates sharing with
   `default' alone.

   b06's other two priced facts HOLD on this body as well, re-measured here:
   inlining the D_800814A8 pointer local into the d + 0x2A store is 345 words
   / 16 aligned differences (b06's own figure is 16 too), and dropping `f' so
   the gate reads c + 0x2C directly is 343 / 5 -- again b06's own 5. Its
   one-table-local-per-dress-site fact is carried unmeasured.

   Deltas from the twin, all read off the listing. The relink: D_80175894 ->
   D_8017425C, D_8017588C -> D_80174254 at both the d[0x25] dress and
   func_800AA924's refusal table, D_b06_8017586C -> D_b16_8017420C,
   D_80175874 -> D_8017421C, D_80175884 -> D_8017424C, and p + 0xB6 / p + 0xB8
   -> p + 0xA2 / p + 0x9E. The callees relink to this bank's: func_b16_80171574
   (zero-arity, through the alias, since it is defined in this unit with four
   parameters), func_b16_80171784, func_b16_80173468, func_b16_80173AE8,
   func_b16_80171ECC, func_b16_80172090 and func_b16_801721B4.

   func_b16_80171574 is called WITH NO ARGUMENTS -- nothing writes $a0..$a3
   before the `jal' -- but it is defined in this unit with four parameters,
   so the zero-arity call goes through the alias func_b16_80171574_0. */
void func_b16_80171014(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *f;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b16_80171574_0();
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_8017425C)
        {
            e = D_80174254;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_8017425C)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80174254) != 0)
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
        e2 = D_b16_8017420C;
        if (f != e2)
        {
            *(u8 **)(c + 0x2C) = e2;
            func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            c[5] = 1;
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_8017421C) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b16_80173468(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0 && (s16)func_800AAB10(p, b, c, d) != 0)
        {
            func_b16_80173AE8(p, b, c, d);
            return;
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
            goto deflt;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b16_801721B4(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                goto deflt;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b16_80171ECC(p, b, c, d) != 0)
            {
                return;
            }
            func_b16_80172090(p, b, c, d);
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
            func_800AAF00(p, b, c, D_8017424C, func_b16_80171014);
            return;
        default:
            goto deflt;
        }
    deflt:
        func_b16_80171784(p, b, c, d);
        return;
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

/* The banked room script's walk step with a same-tile short circuit, the
   relinked twin of func_b02_8017169C (csrc/ovl_dng_b02.c) -- itself the
   trimmed form of the resident func_800D904C (csrc/ovl_dungeon_800D50FC.c):
   no func_8009A66C call, no `(s16)t == 3' arm, func_800A0818 called AFTER
   func_8009A21C with the `x'/`y' locals straight, and the D_b16_8017420C
   dress carrying only the `!=' arm. The one thing this copy adds over that
   twin is an arm that is in none of its five relinked siblings: when the
   cursor's path byte pair already equals c + 0x24/0x25 the step is skipped
   and p + 0x92 / d + 0x88 are slid by 0x20 instead. 132 words, 0 differing.

   THE d + 0x88 READ MUST BE HOISTED INTO `w' AHEAD OF THE p + 0x92 STORE.
   Written as three read-modify-writes in listing order the target's
   `lhu 0x88($s0)' cannot move up into the delay slot of `lhu 0x92($s2)':
   $s2 is `p' and $s0 is `d', so the scheduler will not lift a load across a
   store through the other pointer, and the slot takes a nop -- 133 words,
   87 differing (every word after it shifted). With `w' the two loads are
   adjacent in the RTL before either store and the slot fills, which is the
   whole one-word delta. */
void func_b16_80171574(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 w;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b16_8017420C)
        {
            *(u8 **)(c + 0x2C) = D_b16_8017420C;
            func_80047784(c, D_b16_8017420C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        if (c[0x24] == *(d + *(s16 *)(d + 0x8A) + 0x74) && c[0x25] == *(d + *(s16 *)(d + 0x8A) + 0x7C))
        {
            w = *(u16 *)(d + 0x88) - 0x20;
            *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 0x20;
            *(s16 *)(d + 0x88) = w;
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        }
        else
        {
            x = c[0x24];
            y = c[0x25];
            func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
            c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (u16 *)(p + 0x98));
        }
        p[0x9A] = 0xF;
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
   AND PASSED ON. func_b16_80171ECC's call sets only $a0, $a2 and $a3, so its
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
   func_8009A8C0's first argument is local-allocated. Sharing `e', whose other
   live range is in the pre-loop arms, gives the allocno the hard-register
   conflicts that put $a0 out of reach, and find_reg keeps $v1.

   Measured, on this body, by deletion: writing the 0x20000 arm's
   D_80082E80 through a local is 465 words / 23 differing, and the resident
   twin's own note explains why the D_8006CCE8 lookup must stay inline. The
   first gate with the func_800A9A0C call as the *taken* arm rather than the
   if-body is 11 differing. The tail's D_80083460 through `g' instead of its
   own `g2' is 3 differing. The `(s16)' casts on the three s32-returning
   callees are load-bearing -- func_8009A8C0 2 words, func_b16_80171ECC 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b16_80171784(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b16_80171ECC(p, b, c, d) == 0)
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
                if ((s16)func_8009A8C0(m, c, d, 0x20) <= 0)
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
        if ((s16)func_8009A8C0((s16)s, (u8 *)c, d, 0x20) > 0)
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

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174214 facing table and re-aim through func_8009C93C. 113 words,
   0 differing.

   The twin is the landed func_b45_80171EBC (csrc/ovl_dng_b45.c), identical
   in csrc/ovl_dng_b42.c and csrc/ovl_dng_b41.c, SAME FRAME (addiu $sp, -0x40
   with $s0..$s5 + $ra in the same order). Two of its three facts hold:

   THE FRAME IS 0x40 WHERE THE CALLS NEED 0x38, so the original declared an
   unreferenced 8-byte aggregate local. HOLDS: no local at all is 113 words /
   27 aligned differences, and `s32 unused;' instead of the array is also
   113 / 27 -- every $sp-relative offset and both ends of the frame.

   THE SHARED `-1' EXIT BELONGS TO THE SECOND func_800A2B5C TEST. HOLDS:
   four plain `return -1;' with no label is 114 words / 12, and the label put
   in the second D_80083460 gate instead is also 114 / 12. The twins' own
   figures for these two are 113 / 35 and 113 / 36; they do not reproduce
   here because of the inversion below, so take these numbers, not theirs.

   AND ITS THIRD FACT IS INVERTED. All three twins put `d[0x6D] -= 1;' BEFORE
   the func_8009C93C call and price "after the call" at 113 / 8. Here AFTER
   the call is what lands, and the twins' spelling is 113 words / 10 aligned
   differences -- in the target the decrement is $s0-based with $v1 after the
   call, where the twins' is $a0-based with $v0 before it.

   The `g = (u8 *)&D_80083460' pointer local HOLDS: writing the gates off the
   D_80083462 symbol directly is 112 words / 17.

   THIS BODY'S PROTOTYPE WAS RECONCILED. The declaration block carried
   `s32 func_b16_80171ECC(u8 *p, void *b, void *c, u8 *d);' for the landed
   func_b16_80171784's call; it is now `(u8 *p, UnkVec *b, u8 *c, u8 *d)' so
   the body can index `c' and hand `b' to func_800C7930. Every landed body in
   the unit was re-verified after the change and all stayed at 0 differing.

   func_800A04F0 comes through the unit's existing alias, whose fourth
   parameter is `s16'. */
s32 func_b16_80171ECC(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174214;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x6D] -= 1;
    return 1;
}

/* The banked room script's unconditional "same slot" entry: past the single
   D_80083462 0x2000 gate and the two func_800A2B5C tests, put the actor into
   state 0x17, swap the D_8017423C facing table into the slot's +0x2C and
   re-aim through func_8009C93C. 73 words, 0 differing, on the first compile.

   The twin is the 70-word func_b03_80171F64 (csrc/ovl_dng_b03.c); read its
   comment for the mechanism, which all held here -- the `(s16)' on both
   func_800A2B5C tests, no leftover aggregate local (the 0x30 frame is exactly
   the five saves plus func_8009C93C's 0x18 outgoing area), `d[0x6D] -= 1'
   BEFORE the func_8009C93C call and the +0x84/+0x85 pair AFTER it, and the
   p + 0x9A, p + 0x8C, p + 0x9B store order.

   Three deltas from that twin, all read off the listing: the state byte is
   0x17 rather than 0x11, the facing table is this bank's D_8017423C, and
   there is an extra `*(u16 *)(p + 0x98) |= 8;' between `d[0x6D] -= 1;' and
   the func_8009C93C call -- the three words at .Lb02_8017230C.
   THIS COPY'S OWN CONSTANT: 0x17 -> 0x11.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b16_80172090(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_8017423C;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    *(u16 *)(p + 0x98) |= 8;
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
s32 func_b16_801721B4(u8 *a, void *b, void *c, s32 d)
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
        func_b16_80171784(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b16_80171784(p, b, c, p);
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
   func_b16_80171014. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b16_801722F8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b16_80171014;
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
void func_b16_8017238C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        }
    }
}

/* A four-state tail switched on the actor's +0x9B: state 0 bumps itself to 1
   and falls into state 1, which either flags the actor done and hands off to
   func_8009C12C or zeroes the UnkVec's three high fields, arms the
   D_8017423C facing table and fires the func_800A56E0(0x804) sting; state 2
   counts the +0x96 timer up to 8 and falls into state 3, which parks
   func_b16_80171014 in +0x8C and clears +0x46's top bit. 109 words,
   0 differing.

   IT LANDED BECAUSE func_b16_80171014 WAS ALREADY IN THIS UNIT -- its
   address is what state 3 stores into +0x8C -- and the only new declaration
   it needed was func_8009C12C; func_800A56E0 and D_8008346C had been brought
   in by func_b16_801725A4 above.

   The twin is the landed func_b42_80172F04 (csrc/ovl_dng_b42.c), 120 words,
   which is the same machine with its states split differently: there state 0
   owns the 0x8000 gate, state 1 the dress and state 2 the func_8009C12C,
   each ending in its own `p[0x9B]++; *(s16 *)(p + 0x96) = 0;'. Here states 0
   and 1 are one arm, state 2 has neither the increment nor the +0x96 reset
   and falls straight into state 3. b42 also supplies this copy's constants:
   the c + 0x14 masks 0x8000 / 0x8000 / 0xE000 (b27's three are all 0xE000),
   against b42's own counter 4 and 0x809 this one counts to 8 and stings with
   0x804, and func_800AD594's second argument is 0x100.

   TWO OF THE TWIN'S STATEMENT ORDERS ARE INVERTED, and together they were
   worth 30 aligned differences and a $s1/$s2 exchange between `p' and `d'
   that looked like a separate allocation problem:

   | spelling                                        | words | aligned |
   | 0x46 clear before func_800A4ACC, sting last     |  109  |    0    |
   | both as b42 writes them                         |  109  |   30    |
   | b42's 4ACC-then-0x46 only                       |  110  |    6    |
   | b42's sting-first only                          |  108  |    7    |
   | sting between the +0x9B bump and the +0x96 zero |  108  |    4    |
   | +0x96 zero, sting, then the +0x9B bump          |  109  |    8    |

   The +0x96 zero and the +0x9B bump are free in either order (0 differing
   both ways), so only the sting's position is a fact. Four more plausible
   spellings measured FREE at 109 / 0 and are NOT why this lands:
   `*(s16 *)(p + 0x96) += 1;' in place of the `*(u16 *)' read, `*(void **)'
   in place of `*(u32 *)' for the +0x8C park, the three UnkVec stores written
   as `*(s32 *)((u8 *)b + N) = 0;', and a table local for D_8017423C instead
   of naming the symbol twice. */
void func_b16_801723F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_8017423C;
        func_80047784(c, D_8017423C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        *(s16 *)(p + 0x96) = 0;
        func_800A56E0(0x804);
        return;
    case 2:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) == 8 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
        }
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_800AD594(d, 0x100);
        *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A4ACC(d);
        return;
    }
}

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state
   1 waits on func_8003F270, state 2 runs the c + 0x14 gates and the
   bank-local D_b16_8017420C facing table, then re-installs
   func_b16_80171014. 269 words, 0 differing, ON THE FIRST COMPILE off the
   landed func_b28_80172A58 (csrc/ovl_dng_b28.c), 286 words.

   OWNS jtbl_b16_80170838: seven live entries plus one 0x00000000 pad word,
   for state 0's FIRST switch over the 0x3FFF field -- cases 1, 2, 3, 5, 6, 7
   with case 4 falling to `default'. The second switch (the (d + 0x1C) & 0x2000
   `else') has three cases and compiles to a compare chain.

   IT LANDED BECAUSE func_b16_80171014 WAS ALREADY IN THIS UNIT: its address
   is what both +0x8C stores park, and the only new declarations this body
   needed were func_8003F270, func_800A05A4, func_800A94A0, func_800BB044,
   func_800A56E0 and D_8008346C.

   ALL THREE OF b28's PRICED FACTS HOLD, at these aligned prices:
   `k' as `s32' instead of `s16' is 268 words / 51; the two sign fixes written
   as `if (x < 0) x = -x;' instead of `__builtin_abs' is 268 / 14, the same
   14 b28 records; and the compare-chain switch's arms written ascending
   instead of 3, 2, 1 is 271 / 10, again b28's own 10. Its free fact holds
   too: `D_8006DE24[g[0]].pad_11[1] == 2' in an `else if', without the
   tbl/i/e locals, is 0 differing. ONE MORE MEASURED AS FREE, which b28 did
   not price: the TABLE switch's arms are free in either order -- 7,3 / 6,2 /
   5,1 as below and 5,1 / 6,2 / 7,3 ascending both land at 269 / 0. The `g2'
   pointer local for D_80083460 in the state 2 tail HOLDS: the direct
   `(u8 *)&D_80083460 + 0xC' / `+ 0xA' form is 268 words / 11.

   b16's own deltas from b28, read off the listing: c[0x4] == 4 where b28 has
   3, and state 2 is SEVENTEEN WORDS SHORTER -- the `(c + 0x14) |= 0x800'
   publish, the `*(s16 *)(p + 0x96) = 1' arm and the whole +0x96 countdown
   that follows it are absent, leaving only the +0x98 0x80 set inside the
   gate. Facing table D_80174520 -> D_b16_8017420C. */
void func_b16_801725A4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
                    *(s32 *)(d + 0x60) =
                        func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
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
            *(void **)(p + 0x8C) = func_b16_80171014;
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
        if ((*(s8 *)(c + 0x4) == 4 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            f = D_b16_8017420C;
            if (*(u8 **)(c + 0x2C) != f)
            {
                *(u8 **)(c + 0x2C) = f;
                func_80047784(c, f[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            g2 = (u8 *)&D_80083460;
            if (*(u32 *)(g2 + 0xC) == 0)
            {
                *(u16 *)(g2 + 0xA) -= 1;
                *(void **)(p + 0x8C) = func_b16_80171014;
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

/* The banked room script's three-state glide, switching on p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it, state 2 steers onto the tile centre and installs
   func_b16_80171014. The resident overlay's func_800D9C7C
   (csrc/ovl_dungeon_800D50FC.c) is the same machine -- the same three cases
   FALLING THROUGH into one another, the same `stop:' label inside case 1
   that case 0's `else' reaches by `goto', the same func_800AAA54 call -- and
   func_800D991C in the same unit supplies the divide form of case 2. Both
   of those comments carry the measured spelling facts this body relies on:
   the three-statement numerator `u = c[0x24] << 6; w = b2 - 0x20; u -= w;'
   (nine other spellings 1 to 4 differing on func_800D991C), the second
   numerator left two-statement (1 word and 145 differing when split), and
   `if (x > 0) x--;' in case 1 against `if (--x > 0)' in case 2, which
   func_800D9C7C measures as not interchangeable.

   Four deltas from func_800D9C7C, all read off the listing: the
   D_8006CCD8 index is NOT a local here -- state 0 recomputes
   `(*(u16 *)(d + 0x6A) >> 9) & 7' for each of the two tables and state 1
   never uses it, so there is no $s4 to hold it; the seeds are negated and
   scaled by 15 (`negu' then `sll 15') rather than shifted by 18; state 1
   decays by a quarter (`b->unk_0C -= b->unk_0C / 4', the
   `bgez'/`addiu 3'/`sra 2' triple) where func_800D9C7C subtracts
   `D_8006CCD8[k] << 14'; and func_800AAA54's fourth argument is the
   bank-local D_80174244 rather than 0. 181 words, 0 differing, on the first
   compile. */
void func_b16_801729D8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
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
            func_800AAA54(p, b, c, D_80174244);
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
        *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b16_80172CAC(void)
{
    func_800AD058();
}

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile. */
void func_b16_80172CCC(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b16_80172CF8(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b16_80172D18(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_8017425C facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b16_80171014. 241 words,
   0 differing, on the first compile.

   THE TWIN IS func_b03_80173358 (csrc/ovl_dng_b03.c), not the 213-word
   func_b01_80173AAC / func_b16_80173114 pair: like func_b03_80173358 this
   copy opens state 1 with a func_80042900 test rather than the d[0x25] one,
   carries the func_800AA79C arm behind the g + 0x2 0x2008 gate, closes state
   1 with the D_80082E80 / func_8009FD40 / func_800A6D30 window and the second
   func_80042900 test, and writes the dress/flag/bump tail out TWICE -- once
   in the first test's `== 0' arm and once after `again:' -- which gcc
   cross-jumps into the single block at the target's .Lb02_80174134. Read that
   comment for the mechanism: `&D_80083460' as separate pointer locals per
   arm, and the facing table named directly at every site.

   Four deltas from that twin, all read off the listing. The tail's flag
   update is `*(s32 *)(d + 0x1C) |= 0x40000' and it sits BEFORE the c + 0x14
   0x8000 gate, unconditionally, where func_b03_80173358 has `&= ~0x200'
   inside the gate -- that is func_b01_80173AAC's constant in
   func_b03_80173358's position. The 0x80000 arm has the same six-word block
   func_b01_80173AAC has between func_800AA888 and the bank-local tail call,
   p + 0x92 reduced by p + 0xA2 as halfwords then p + 0xA2 and p + 0x9E
   cleared, and the tail call is this unit's already-landed func_b16_80173468.
   State 2 changes no flag word at all. And in the tail the two bumps are in
   the opposite order to the twin's -- `p[0x9B] += 1' first, then
   `*(u16 *)(k + 0xA) += 1' -- which is the order the `sb 0x9B' / `lhu 0xA'
   pair appears in at .Lb02_80174194. */
void func_b16_80172D50(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_8017425C;
        func_80047784(c, D_8017425C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80174264;
            func_80047784(c, D_80174264[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
                return;
            }
            p[0x9B] += 1;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
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
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b16_80173468(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174264;
        func_80047784(c, D_80174264[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
            return;
        }
        p[0x9B] += 1;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_8017425C facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b16_80171014. The landed
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
   func_800AA888 and the bank-local func_b16_80173468 -- p + 0x92 reduced by
   p + 0xA2 as halfwords (both reads `lhu', so both are unsigned), then
   p + 0xA2 and p + 0x9E cleared. p + 0x92 is READ BEFORE p + 0xA2 IS
   CLEARED in the listing, which is the order the three statements are
   written in. 213 words, 0 differing, on the first compile; the six-word
   block and the `~8' account for the 213 against the twin's 207. */
void func_b16_80173114(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_8017425C;
        func_80047784(c, D_8017425C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174264;
            func_80047784(c, D_80174264[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b16_80173468(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174264;
        func_80047784(c, D_80174264[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(s32 *)(d + 0x1C) &= ~8;
        *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
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
   call emits the bare `jal` either way); hoisting D_b16_8017420C into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b16_80173468(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_8017425C && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b16_8017420C;
        func_80047784(c, D_b16_8017420C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b16_80173530(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b16_80171014;
        }
    }
}

/* 14 words, 0 differing. */
void func_b16_80173594(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* 14 words, 0 differing. */
void func_b16_801735CC(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* 14 words, 0 differing. */
void func_b16_80173604(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* 14 words, 0 differing. */
void func_b16_8017363C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* 14 words, 0 differing. */
void func_b16_80173674(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b16_80171014;
    }
}

/* The node walk, the relinked twin of the resident overlay's func_800ADBCC
   (csrc/ovl_dungeon_800A4B88.c): walk the second list off p - 8, park a
   halfword in the scratchpad each turn and call func_800453E0 for every slot
   whose +0x14 does not carry 0x80. 35 words, 0 differing, first compile.

   THE RELINK IS THE SCRATCHPAD VALUE. func_800ADBCC parks the record's own
   `*(s16 *)(p + 0x8)'; this copy parks the bank-local s16 D_801742E0, whose
   `lui' is hoisted out of the loop while the `lh' stays inside it -- that is
   the whole of the three-word difference from the twin.

   Both of the twin's load-bearing spellings were kept and still are: the
   scratchpad goes through a POINTER LOCAL (as an absolute `*(s32 *)0x1F80013C'
   it is the assembler's `lui $at'/`sw' macro, which cannot leave the loop),
   and the slot stays a `u8 *' rather than a struct pointer so its +0x14 read
   is not MEM_IN_STRUCT_P and sched1 leaves the load-delay `nop's alone. */
s32 func_b16_801736AC(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = D_801742E0;
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

/* The driver func_b16_80173904 parks at its node's +0x10: swing the
   renderable around the slot's +0x40/+0x44 centre on a 0x12C-per-frame angle
   at a radius that GROWS by 2 each frame, pull it 0xC0000 closer, re-skin it
   and, while the slot still carries 0x4000, re-dress it from D_800DEC70;
   under a +0x1A countdown of 0xA fade the three +0xC..+0xE colour bytes out
   proportionally, and at zero raise 0x8000 in the node's -0x2 and in
   D_800814A0. 115 words, 0 differing.

   THE TWIN IS THE LANDED func_b18_801741A0 (csrc/ovl_dng_b18.c, 113 words,
   dngtwin 0.798) -- the sibling driver of the same spawner shape. Both of
   its priced facts are live here and both HELD:

     * THE THREE /9 EASES EACH RE-READ `*(s16 *)(p + 0x1A)'. Through one
       local it is 113 words / 25 aligned differences here -- b18's figure to
       the digit. The gate's own `lh' is reused by the FIRST ease only (cse
       inside one basic block); sites two and three reload.
     * `b->unk_08 += 0xFFF40000' and `-= 0xC0000' are FREE either way (both
       115 / 0), as on b18.

   THE RADIUS MULTIPLY IS NOT b18's `* 640', AND ITS POWER-OF-TWO SCALE IS
   STILL NOT FREE AS A SHIFT. b18's radius is the constant 640; here it is
   the slot's own `*(s16 *)(p + 0x20)' times 0x10. The scale is a power of
   two, so a widely stated rule says `* 0x10' and `<< 4' are the same tree --
   ON THIS BODY THEY ARE NOT. `* 0x10' is 0 and `<< 4' is 3 aligned
   differences at the same 115 words. The rule holds for a shift of a single
   operand; here the scaled value is itself a `MULT_EXPR' of two variables,
   and `MULT_EXPR(MULT_EXPR(a,b),16)' fuses with the following `addu' where
   `LSHIFT_EXPR(MULT_EXPR(a,b),4)' does not. Measured, not inferred.

   Priced on this body:

   | spelling                                              | words | aligned |
   | as written below                                       |  115  |    0    |
   | the three /9 eases through one +0x1A local              |  113  |   25    |
   | the +0x20 bump moved after the two centre stores        |  113  |    5    |
   | `(... * 0x10)' written `(... << 4)'                     |  115  |    3    |
   | the call result as the FIRST multiply operand           |  115  |    1    |

   MEASURED AND FREE: `b->unk_08 -= 0xC0000' for `+= 0xFFF40000' (115 / 0, as
   on b18), and the 0x4000 re-dress gate written as an empty `then' with the
   call in the `else' (115 / 0 -- the arms are textually distinguishable, so
   this is FREE and not merely unmeasurable).

   DELTA FROM b18: the +0x20 radius bump and the 0x4000/D_800DEC70 re-dress
   are this copy's, and b18's three-test func_b18_8017406C companion spawn is
   absent. */
void func_b16_80173738(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) - 0x12C;
    *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) + 2;
    b->unk_00 = *(s32 *)(p + 0x40) + *(s16 *)(p + 0x20) * func_80064584(*(s16 *)(p + 0x1E)) * 0x10;
    b->unk_04 = *(s32 *)(p + 0x44) + *(s16 *)(p + 0x20) * func_800644B8(*(s16 *)(p + 0x1E)) * 0x10;
    b->unk_08 += 0xFFF40000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x4000) != 0)
    {
        func_8003DB94(c, D_800DEC70, 0);
    }
    if (*(s16 *)(p + 0x1A) < 0xA)
    {
        c[0xC] = p[0] * *(s16 *)(p + 0x1A) / 9;
        c[0xD] = p[1] * *(s16 *)(p + 0x1A) / 9;
        c[0xE] = p[2] * *(s16 *)(p + 0x1A) / 9;
    }
    if ((s16)(--*(u16 *)(p + 0x1A)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The bank's 0x212 particle spawner, called eight times a frame out of
   func_b16_80173CEC's states 4 and 5: allocate a node, install
   func_b16_80173738 as its +0x10 step, give the payload a 0x14 countdown and
   a 0x14 radius and the caller's angle, dress the slot from D_80045340 and
   D_800DEC70 with a 0x1000/0x1000 scale and a 0x40/0xC0/0x40 colour, place
   the node's mover on the +0x40/+0x44 centre at that angle, and light the
   renderable up with +0x12 = 0x7DCF. 121 words, 0 differing.

   THE BRIEF'S TWIN IS func_s59_80024DD4 AT 0.609 AND I DID NOT USE IT. The
   right body is the landed func_b18_8017406C (csrc/ovl_dng_b18.c, 77 words),
   which `dngtwin' scores below its floor because it is 44 words shorter:
   it is the SPAWNER OF THE SAME DRIVER FAMILY -- same func_8003FC64(0x212),
   same func_8004491C(n, D_80045340), same func_8003DB94(r, D_800DEC70, ...)
   skeleton -- and its sibling driver func_b18_801741A0 is the twin of
   func_b16_80173738 just above. Found by taking func_b16_80173738's twin and
   reading ITS caller.

   b18's TWO READ-OFF-THE-LISTING SPELLINGS, one HELD and one INVERTED:

     * `r' IS ASSIGNED TWICE, not once -- HELD, and expensively. The target
       reloads `lw $s0, 0xC($s2)' after the stores through `m' and
       `v + 0x40'. One `r' across the whole body is 120 words / 39 aligned
       differences (b18 states the fact without a price).
     * THE PAYLOAD'S +0x00 STORE IS SPELLED OFF `n' -- INVERTED. b18's target
       uses the node base at that one site; THIS one uses `v': the listing has
       `sb $v1, 0x0($s1)' with $s1 = n + 0x20. Written b18's way as
       `*((u8 *)n + 0x20) = ...' it is 121 words / 1 aligned difference. A
       one-site naming fact, read off the register, not inherited.

   TWO FACTS b18 DOES NOT HAVE, and both were the fight:

     * `k = e' MUST SIT BEFORE THE NULL TEST, not at the `while' loop that
       uses it. The target fills the `beqz $s2' delay slot with
       `addu $s3, $s5, $zero', which reorg can only do by moving the copy
       DOWN from before the branch -- and with the copy early the body gets
       an eighth callee-saved register ($s0..$s7 against $s0..$s6). Written
       inside the `if', beside the loop, it is 118 words / 23 aligned
       differences and ONE REGISTER SHORT, with the loop variable and the
       sign-extended angle sharing $s0. An `s32' angle local for the two call
       arguments does NOT recover it (still 117 / 43); only the copy's
       POSITION does, which is why this is a statement-order fact and not an
       allocation one.
     * `v[0]' READS `r->unk_0C[0]' AND THE COLOUR BYTES GO 0xC, 0xE, 0xD.
       Written as the literal `v[0] = 0x40' it is 120 words / 9: cse unifies
       the three 0x40 constants into one pseudo that simply stays live, where
       the target has `addiu $v0, $zero, 0x40' and a separate
       `addu $v1, $v0, $zero' -- the register copy cse leaves behind when it
       forwards a stored value into a LOAD. And the two 0x40 stores must come
       FIRST: ascending 0xC/0xD/0xE is 121 / 11, because then the 0xC0
       constant lands between them and the forwarded copy has nothing to
       preserve. The two together are the only spelling that reaches 0: with
       the loads but ascending order it is 121 / 11, with the literal in
       either order 120 / 9.

   THE SIXTH PARAMETER IS A `u16' AND THE FIFTH AN `s32'. The prologue loads
   them `lw $s5, 0x48($sp)' and `lhu $s7, 0x4C($sp)'. `s32 f' is 119 words /
   12 differences HERE, and 328 / 0 in the caller func_b16_80173CEC -- so the
   narrow sixth parameter is load-bearing on the callee and NOT MEASURABLE on
   the caller, which emits `sw $zero, 0x14($sp)' either way. Parameters 1, 3
   and 4 are never read -- $a0 is overwritten by the 0x212 constant -- and are
   there to carry the caller's four.

   Priced on this body:

   | spelling                                              | words | aligned |
   | as written below                                       |  121  |    0    |
   | `s32 k' instead of `s16 k'                              |  115  |   60    |
   | one `r' for the whole body                             |  120  |   39    |
   | `m = n->unk_08' after the two centre stores             |  124  |   35    |
   | `k = e' inside the `if', at the loop                    |  118  |   23    |
   | the colour bytes ascending 0xC/0xD/0xE                  |  121  |   11    |
   | `s32 f' sixth parameter                                 |  119  |   12    |
   | `v[0] = 0x40' literal instead of `r->unk_0C[0]'         |  120  |    9    |
   | `(... * 0x10)' written `(... << 4)'                     |  121  |    3    |
   | the +0x00 store off `n' instead of `v'                  |  121  |    1    |

   MEASURED AND FREE: the whole +0xC..+0xE block spelled with plain
   `((u8 *)r)[0xC]' casts instead of `r->unk_0C[k]' (121 / 0), so UnkSlot is
   kept for readability and not for the codegen; and an `s32' angle local
   between the loop and the two calls (no change in either direction once
   `k = e' is positioned right). */
void func_b16_80173904(u8 *p, UnkVec *b, u8 *c, u8 *d, s32 e, u16 f)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *v;
    s16 k;

    n = func_8003FC64(0x212);
    k = e;
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        *(s16 *)(v + 0x1A) = 0x14;
        *(s16 *)(v + 0x20) = 0x14;
        *(s16 *)(v + 0x1E) = e;
        n->unk_10 = (u32)func_b16_80173738;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        if ((rand() & 1) != 0)
        {
            r->unk_14 |= 1;
        }
        m = n->unk_08;
        *(s32 *)(v + 0x40) = b->unk_00;
        *(s32 *)(v + 0x44) = b->unk_04;
        while (k > 0x1000)
        {
            k -= 0x1000;
        }
        m->unk_00 = *(s32 *)(v + 0x40) + *(s16 *)(v + 0x20) * func_80064584(k) * 0x10;
        m->unk_04 = *(s32 *)(v + 0x44) + *(s16 *)(v + 0x20) * func_800644B8(k) * 0x10;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) + f;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_0C[0] = 0x40;
        r->unk_0C[2] = 0x40;
        r->unk_0C[1] = 0xC0;
        v[0] = r->unk_0C[0];
        v[1] = r->unk_0C[1];
        v[2] = r->unk_0C[2];
        func_8003DB94(r, D_800DEC70, 0);
        r->unk_12 = 0x7DCF;
        r->unk_14 |= 0x100;
    }
}

/* State 0x17's entry: clear the node's +0x71 top bit and +0x6D, install
   ITSELF in the slot's +0x8C handler slot and mark state 0x17/0; then, unless
   D_80083460's +0x2 0x2000 bit is up, if func_800A2BDC says the engine is
   idle, drop the handler again, re-mark 0x17/0, reset D_800DCF5E and bump
   D_80083460's +0xA counter. 43 words, 0 differing, first compile.

   THE +0x8C STORE IS THE FUNCTION'S OWN ADDRESS -- `lui'/`addiu' of
   %hi/%lo(func_b16_80173AE8) -- so the self-reference is written as the
   plain identifier, not as a table lookup.

   D_80083460 goes through the unit's `g = (u8 *)&D_80083460' idiom: one
   `addiu' base in $s1 serves both the +0x2 gate and the +0xA bump. */
void func_b16_80173AE8(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    *(void **)(p + 0x8C) = func_b16_80173AE8;
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
   +0x8 in the bank-local D_801742E8, re-send the view through func_800478B8
   and -- only if its +0x14 carries one of 0xE000 -- unlink the node, set the
   0x10-frame countdown in +0x96 and step +0x9A; every eighth frame of that
   phase also bumps D_801742E0. Afterwards, ease D_80083160's three
   +0xA8..+0xAA colour bytes towards 0x80 by (0x80 - v) / +0x96 and, when the
   countdown runs out, raise 0x8000 in the node's -0x2 and in D_800814A0.
   86 words, 0 differing, first compile. 2 copies (dngfam.py:
   func_b16_80173B94, func_b16_80173B94).

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

   THE RELINK IS D_80175220 -> D_801742E8 AND ELEVEN EXTRA WORDS: the
   state-0 arm ends with `if (((*(u16 *)(p + 0x96))++ & 7) == 0)
   D_801742E0 += 1;'. The post-increment is load-bearing -- the target stores
   the BUMPED halfword (`lhu', `addiu 1', `andi 7' on the OLD value, `sh' the
   new) -- and the arm's exit `j' is reused by the new `bnez', so 75 + 12 - 1
   = 86. D_801742E0 is the same bank-local halfword func_b12_8017458C below
   parks in the scratchpad, and its existing `extern s16' declaration serves
   both uses unchanged.

   `--raw' is 13 differing and all 13 are relocations -- the two `jal's, the
   one `j' and the ten `%hi'/`%lo' halves for D_80083160, D_801742E8,
   D_801742E0 and D_800814A0. func_80044A50 and the bank-local D_801742E8
   are the only new declarations this body needed; D_80083160 and D_800814A0
   come from include/structs.h.
   THIS COPY'S OWN CONSTANT: 0x7 -> 0x3, 0x1 -> -0x1 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b16_80173B94(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (p[0x9A] == 0)
    {
        D_801742E8 = *(s32 *)(c + 0x8);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9A] += 1;
        }
        if (((*(u16 *)(p + 0x96))++ & 3) == 0)
        {
            D_801742E0 += -1;
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

/* The bank's eight-state white-out cutscene, run off the slot's +0x9B: step
   the counter, hand func_80041588 the D_801742CC command stream and its
   D_801742E4 handshake byte, turn the actor to face 2, swap the HUD pair
   through func_8003F540/func_8003E4FC, open with func_800C77D0, ease
   D_80083160's three colour bytes while the D_801742E5 handshake is pending,
   spawn the 0x12 node whose driver is func_b16_80173B94, poll
   func_b16_80173904 eight times at the end of state 4 and again for the first
   0x1E frames of state 5, wait for the node's +0x14 to carry one of 0xE000,
   then func_800ADC4C the replacement in and re-aim it. 328 words, 0 differing.

   THIS FUNCTION OWNS jtbl_b16_80170858, the 8-entry table at rodata offset
   0x0, indexed by p[0x9B] for states 0..7. The dispatch bound is
   `sltiu $v0, $v1, 0x8' -- 8 cases -- and the listing has 8 `.word's, so
   there are 8 cases and NO PAD WORD. Every state has a label of its own, no
   two share an arm and there is no `default': everything >= 8 goes to the
   epilogue. States 1, 2 and 5 fall through to their successors.

   THE TWIN IS THE LANDED func_b10_80175398 (csrc/ovl_dng_b10.c, 350 words,
   dngtwin 0.841). Both of its carried allocation facts hold here:

     * STATE 3'S D_801742E5 FLAG MUST BE READ INTO A LOCAL BEFORE THE +0x96
       RESTORE STORE -- the restore `sh' sits in the flag test's delay slot.
     * STATE 6'S `d - 0x20' IS THE SAME LOCAL `r' STATES 4 AND 5 USE.

   DELTA FROM THAT TWIN, all read off the listing: eight states, not nine --
   this copy's state 7 is the bare `*(s32 *)(d + 0x60) = func_800A504C(c, d);'
   and b10's states 7 and 8 are absent; state 1 GAINS
   `*(s32 *)(d + 0x1C) &= 0xFFFBFFFF;'; the slot's s16 is at `p + 0xA6' where
   b10 has `p + 0xAC', and the node pointer is at `p + 0xAC' where b10 has
   `p + 0xA8'; state 4 GAINS its own copy of the eight-call
   func_b16_80173904 poll loop, so the loop is written TWICE; the +0x10
   driver is func_b16_80173B94 and func_8004491C's second argument is the
   FUNCTION func_b16_801736AC; `D_801742E0 = -8' where b10 has -2; and the
   relink is D_801759F8 -> D_801742CC, D_80175A80 -> D_801742E4,
   D_80175A81 -> D_801742E5, D_80175A00 -> D_801742D4,
   D_80175A84 -> D_801742E8, D_80175A18 -> D_801742E0.

   PRICED ON THIS BODY, all through aligndiff:

   | spelling                                        | words | aligned |
   | as written below                                |  328  |    0    |
   | state 1's 0x1C mask BEFORE the +0x8A store      |  328  |    2    |
   | state 3's flag read AFTER the +0x96 restore     |  329  |    8    |
   | state 4's `D_801742E0 = -8' after the r stores  |  328  |   16    |
   | state 4's `p + 0xAC' store after the unk_08 set |  328  |   16    |
   | state 6's `d - 0x20' in its own local, not `r'  |  328  |    3    |
   | `s32 t3' instead of `s16 t3'                    |  326  |    7    |
   | `s32 i' instead of `s16 i'                      |  320  |   19    |

   MEASURED AND FREE, so NOT reasons this lands: `s32 t' rather than `s16 t'
   in state 3 (0), state 5's gate written as the post-increment
   `if ((s16)((*(u16 *)(p + 0x96))++) < 0x1E)' instead of the `t3' pair (0),
   and -- the one that contradicts a brief -- A SHARED LOOP COUNTER ACROSS THE
   TWO COPIES OF THE POLL LOOP. b25's `local-alloc.c:1759' duplicated-arm
   lever says each copy of a duplicated block needs its own local, and it was
   priced at 4 rows there and on func_b24_801711B0. HERE IT IS FREE: one
   function-scope `i' set in both loops and a block-scope `i2' in state 4's
   copy both measure 328 words / 0 aligned differences. The difference from
   b25 is that these two copies are not a cross-join of one source block --
   they are two separate statements in two switch arms, each already its own
   basic block, so `combine_regs' has nothing to refuse. The lever is about
   cross-joined copies, not about textual duplication.

   D_80083160 IS REACHED THROUGH A `u8 *' BASE TAKEN BEFORE THE DISPATCH, as
   in b10: the `addiu $s0, $v0, %lo(D_80083160)' sits in the prologue ahead of
   the `sltiu' range check although the only use is in state 3, which is what
   a VAR_DECL gives and an inline cast does not.

   func_80069EF8 is spelled `rand' (splat/common_syms.txt) and needs no
   declaration. func_8003FC64 takes ONE argument here -- only $a0 is written
   before the `jal' -- and is unrelated to the unit's func_8003FD64. */
void func_b16_80173CEC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *q;
    u8 *r;
    UnkNode *n;
    s32 t2;
    s32 f;
    s16 t;
    s16 t3;
    s16 i;
    s16 k;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        p[0x9B] += 1;
        return;
    case 1:
        func_80041588(D_801742CC, (s8 *)D_801742E4, 0);
        *(s16 *)(p + 0xA6) = 0;
        p[0x9B] += 1;
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x2A);
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        /* fallthrough */
    case 2:
        t2 = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (D_801742E4[0] == 0 || t2 != 2)
        {
            if (t2 != 2)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            return;
        }
        func_80041588(D_801742CC, (s8 *)D_801742E4, 1);
        func_8003F540(0, D_8006CD58, 0x4000AD4, 0x5000CC4);
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        D_801742E5[0] = 0;
        func_8003E4FC(0xFF, D_8003E140, (s32)D_801742E5);
        func_800C77D0(d - 0x20, (u8 *)b, 8, 0x300);
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        t = *(u16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            f = D_801742E5[0];
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
        n->unk_10 = (u32)func_b16_80173B94;
        func_8004491C(n, (u32)func_b16_801736AC);
        *(UnkNode **)(p + 0xAC) = n;
        n->unk_08->unk_00 = b->unk_00;
        n->unk_08->unk_04 = b->unk_04;
        n->unk_08->unk_08 = b->unk_08;
        D_801742E0 = -8;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        k = func_800498A0(d) - 1;
        if (k < 0)
        {
            k = rand() % 3;
        }
        func_8003DB94(r, D_8014A000 + D_801742D4[k], 0);
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        i = 0;
        do
        {
            func_b16_80173904(p, b, c, d, rand() & 0xFFF, 0);
            i++;
        } while (i < 8);
        return;
    case 5:
        t3 = *(u16 *)(p + 0x96);
        *(s16 *)(p + 0x96) = t3 + 1;
        if (t3 < 0x1E)
        {
            i = 0;
            do
            {
                func_b16_80173904(p, b, c, d, rand() & 0xFFF, 0);
                i++;
            } while (i < 8);
        }
        n = *(UnkNode **)(p + 0xAC);
        r = (u8 *)n->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0xE000) != 0)
        {
            p[0x9B] += 1;
        }
        /* fallthrough */
    case 6:
        if (p[0x9B] == 6 &&
            func_800ADC4C((u8 *)b, D_801742E8, D_801742E0, &D_800DCF5C) != NULL)
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
        *(s32 *)(d + 0x60) = func_800A504C(c, d);
        return;
    }
}
