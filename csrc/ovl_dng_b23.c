#include "common.h"

s32 rand(void);

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b23_80172664(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80174198[])(u8 *, UnkVec *, u8 *, u8 *);

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
s32 func_b23_801726D0(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80174E88[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80174E80[];
extern u8 D_80174EF0[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b23_80171F1C(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b23_80171760();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

extern u8 D_80174EF8[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b23_80174598(u8 *p, u8 *b, u8 *c, u8 *d);

s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);

s32 func_800AB464();

UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_b23_80171358(u8 *p, UnkVec *b, u8 *c);

extern u8 D_8006CCF8[];
extern void (*D_80174F00[])(u8 *, UnkVec *, u8 *, u8 *);
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

extern u8 D_80174EA0[];
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
extern u8 D_80174ED8[];
extern u8 D_80174E98[];
extern s8 D_800DCECC[];
extern u32 D_800814A0;
extern u32 D_800814A8;
extern s32 D_8008346C;
s32 func_8003F270(void);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
s32 func_800A56E0(s32);
void func_b23_80171020(u8 *a, s16 b, s32 c, s32 f, s16 dx, s16 dy, s16 dz);

/* The tumble-and-settle step of the thrown-slot machine: decay b's three
   velocity components by 9/10 (the third by 8/10), wind c+0x1C/+0x1E on by
   0x32, project both b and the slot's own vector at p+0xA8 through
   RotTransPers (RotTransPers) and set c+0x6 from the DIFFERENCE of the two
   depths less twice the D_800DCECC facing byte, then, once p+0x96 is under
   10, square the sprite up and ease its three colour bytes, re-send the view
   through func_800478B8 and raise 0x8000 in the node's -0x2 and in
   D_800814A0 when the countdown runs out.

   No xsibs twin at ratio 0.70 and none in the resident overlay; written from
   the listing. Two facts carried in from landed code, both re-measured here:
   the countdown test is `(s16)(--*(u16 *)(p + 0x96)) <= 0'
   (func_b08_8017451C's form -- `lhu'/`addiu -1'/`sh'/`sll 16'/`bgtz', no
   `sra' and no reload), and the D_800DCECC byte is `* 2', not `<< 1'.

   THE SPELLING FACT, worth one word of the 151: c+0x12 takes its 0xFF80
   through a `u16' destination. Through `*(s16 *)' the constant folds signed
   and gcc emits `addiu $v1, $zero, -0x80' where the target has
   `ori $v1, $zero, 0xFF80' -- 151 words, 1 differing
   (docs/matching-levers.md :24832).

   THE FRAME SHAPE IS LOAD-BEARING: RotTransPers's second out-parameter is
   EIGHT bytes (`s32 sxy[2]'), which is what puts the third and fourth at
   sp+0x20 and sp+0x24 rather than sp+0x1C and sp+0x20. A plain `s32 sxy'
   leaves a hole where the target has none and moves two `addiu's.

   151 words, 0 differing. */
void func_b23_80171F1C(u8 *p, void *b, void *c, u8 *d);
extern u8 D_80174EE8[];
extern u8 D_80174EE0[];
extern u8 D_80174EA8[];
extern u8 D_80174EB0[];
extern u8 D_80174EC8[];
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
u8 *func_8009B25C(u8 *p, u16 x, u16 y, s16 ang);
void func_b23_80170F0C(u8 *, UnkVec *, u8 *, u8 *);
s32 func_b23_80170D2C(u8 *p, UnkVec *v);
void func_b23_80171CD8(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b23_80172894(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b23_8017476C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b23_801729AC(u8 *a, void *b, void *c, s32 d);

void func_b23_80170874(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR sv;
    s32 sxy[2];
    s32 pz;
    s32 flag;
    UnkVec *q;
    s32 r;
    s32 v;
    s32 m;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C = b->unk_0C * 9 / 10;
    b->unk_10 = b->unk_10 * 9 / 10;
    b->unk_14 = b->unk_14 * 8 / 10;
    if (*(u16 *)(c + 0x1C) == 0)
    {
        *(s16 *)(c + 0x1E) = 0x400;
        *(s16 *)(c + 0x1C) = 0x400;
    }
    v = *(u16 *)(c + 0x1E) + 0x32;
    *(s16 *)(c + 0x1E) = v;
    *(s16 *)(c + 0x1C) = v;
    sv.vx = *(u16 *)((u8 *)b + 0x2);
    sv.vy = *(u16 *)((u8 *)b + 0x6);
    sv.vz = *(u16 *)((u8 *)b + 0xA);
    r = RotTransPers(&sv, sxy, &pz, &flag);
    q = *(UnkVec **)(p + 0xA8);
    sv.vx = *(u16 *)((u8 *)q + 0x2);
    sv.vy = *(u16 *)((u8 *)q + 0x6);
    sv.vz = *(u16 *)((u8 *)q + 0xA);
    *(s16 *)(c + 0x6) = r - RotTransPers(&sv, sxy, &pz, &flag) -
                        D_800DCECC[((D_80083228 + *(s16 *)(p + 0x94) + 0x100) >> 9) & 7] * 2;
    m = *(s16 *)(p + 0x96);
    if (m < 0xA)
    {
        *(s16 *)(c + 0x10) = 0x20;
        *(u16 *)(c + 0x12) = 0xFF80;
        *(u16 *)(c + 0x14) |= 0xC;
        v = (m << 7) / 10;
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
    }
    func_800478B8(c);
    if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The slot's OTHER tumble step, and NOT the same function as
   func_b23_80170874 above despite both being 151 words: they share only the
   ~45-word tail (the two RotTransPers projections, the D_80083228 /
   D_800DCECC facing fix-up into c+0x6, func_800478B8 and the countdown into
   the node's -0x2 and D_800814A0). The heads are different work -- this one
   has no velocity decay at all, squares the sprite up under a count of 0x15
   rather than 10 and dividing by 20 rather than 10, winds c+0x1C/+0x1E on by
   a FIVE-STEP ladder off p+0x96, and derives c+0x22 from the sprite data at
   c+0x8. No xsibs twin at ratio 0.70; written from the listing.

   Three things read off the listing rather than carried over:
   (1) The two reads of p+0x96 are SEPARATE locals -- the ladder's `lh' is a
   second load, not the compare's register, because the three `sb's to c
   between them wipe cse's memory table.
   (2) The ladder's five arms each reload `lhu 0x1E' for themselves and the
   two `sh's are written ONCE after the chain, which is why the four taken
   arms are `j' to a common join and the fifth falls into it.
   (3) c+0x22 is `-(x / 0x1000) / 2' in that order: `bgez'/`addiu 0xFFF'/
   `sra 12', then `negu', then the `srl 31'/`addu'/`sra 1' halve.

   The frame is func_b23_80170874's, for the same reason: RotTransPers's
   second out-parameter is eight bytes (`s32 sxy[2]'), so the third and
   fourth sit at sp+0x20 and sp+0x24.

   151 words, 0 differing, first compile -- the tail cost nothing because
   func_b23_80170874 had already brought RotTransPers, D_800DCECC and
   D_800814A0 into the unit. */
void func_b23_80170AD0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR sv;
    s32 sxy[2];
    s32 pz;
    s32 flag;
    UnkVec *q;
    u8 *t;
    s32 m;
    s32 n;
    s32 w;
    s32 x;
    s32 r;

    m = *(s16 *)(p + 0x96);
    if (m < 0x15)
    {
        x = (m << 7) / 20;
        c[0xE] = x;
        c[0xD] = x;
        c[0xC] = x;
    }
    n = *(s16 *)(p + 0x96);
    if (n >= 0x2B)
    {
        w = *(u16 *)(c + 0x1E) + 0x2BC;
    }
    else if (n >= 0x28)
    {
        w = *(u16 *)(c + 0x1E) + 0x226;
    }
    else if (n >= 0x26)
    {
        w = *(u16 *)(c + 0x1E) + 0x190;
    }
    else if (n >= 0x24)
    {
        w = *(u16 *)(c + 0x1E) + 0xC8;
    }
    else if (n >= 0x1A)
    {
        w = *(u16 *)(c + 0x1E) + 0x64;
    }
    else
    {
        goto wound;
    }
    *(s16 *)(c + 0x1E) = w;
    *(s16 *)(c + 0x1C) = w;
wound:
    t = *(u8 **)(c + 0x8);
    x = (t[0xB] + *(s8 *)(t + 0x3)) * *(u16 *)(c + 0x1E);
    *(s16 *)(c + 0x22) = -(x / 0x1000) / 2;
    if (*(s16 *)(p + 0xA2) == 0)
    {
        sv.vx = *(u16 *)((u8 *)b + 0x2);
        sv.vy = *(u16 *)((u8 *)b + 0x6);
        sv.vz = *(u16 *)((u8 *)b + 0xA);
        r = RotTransPers(&sv, sxy, &pz, &flag);
        q = *(UnkVec **)(p + 0xA8);
        sv.vx = *(u16 *)((u8 *)q + 0x2);
        sv.vy = *(u16 *)((u8 *)q + 0x6);
        sv.vz = *(u16 *)((u8 *)q + 0xA);
        *(s16 *)(c + 0x6) = r - RotTransPers(&sv, sxy, &pz, &flag) -
                            D_800DCECC[((D_80083228 + *(s16 *)(p + 0x94) + 0x100) >> 9) & 7] * 2;
    }
    else
    {
        *(s16 *)(c + 0x6) = 4;
    }
    func_800478B8(c);
    if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D5AC0 (csrc/ovl_dungeon_800D50FC.c), byte for byte: one
   semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, whose layout here is the
   twin's exactly (+0x1C cursor, +0x24 OT base, +0xD0/+0xD4 out-parameters,
   +0x100 index). The three dead lbu's at +0x4/+0x5/+0x6 are the
   setRGB0(t, t->r0, t->g0, t->b0) echo of the colour word just copied in;
   the twin's own comment carries the measurements for that and for its three
   load-bearing orderings, which were not re-measured here because the first
   compile of the twin's body landed. 120 words, 0 differing. */
s32 func_b23_80170D2C(u8 *p, UnkVec *v)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b23", func_b23_80170F0C);

/* The bank's spark SPAWNER, called ten at a time from func_b23_8017364C: a
   0x211 node off the caller's own, driven by func_b23_80170F0C, placed at
   the caller's UnkVec plus the dx/dy/dz the caller passes, given a random
   velocity triple at +0x40/+0x44/+0x48 of the slot, nudged along the first
   two axes by twice that velocity, and handed to func_8004491C with
   func_b23_80170D2C as its step. 92 words, 0 differing.

   THE TWIN IS THE LANDED func_b19_80170F2C (csrc/ovl_dng_b19.c, 85 words,
   dngtwin 0.870; func_b15_80170F2C is the same body). Deltas, all read off
   the listing: no `- 0x64' on the z placement and no `n->unk_0C + 0x6' life;
   the velocity goes into the SLOT at +0x40/+0x44/+0x48 rather than into
   n->unk_08 + 0xC/0x10/0x14, is `<< 4' on all three axes rather than `<< 7',
   and the third axis takes `& 0x3FFF' and `- 0x6000' where the first two
   take `& 0x7FFF' and `- 0x4000'; the two `n->unk_08' nudges and the
   `*(s32 *)(p + 0x54) = 0x8000' are new; the +0x32/+0x34 life is 0x14 and
   the step is func_b23_80170D2C.

   THE PROTOTYPE CHANGED AND THE CALLER WAS RE-MEASURED -- see item 3 of the
   report. The unit already carried
   `void func_b23_80171020(u8 *, s32, s32, s32, s32, s32, s32);' and TWO of
   those widths are load-bearing:

   | spelling                                          | words | aligned |
   | as written below                                  |   92  |    0    |
   | `s32 dx, s32 dy, s32 dz'                          |   83  |   37    |
   | `s32 b'                                           |   92  |    8    |
   | `*(u8 **)(a + 0x8)' hoisted into one local        |   88  |   16    |
   | the +0x32 / +0x34 life stores in the other order  |   92  |    4    |
   | `<< 1' instead of `* 2' on both nudges            |   92  |    0    |
   | `*(s32 *)(p + 0x54) = 0x8000' after the +0x14     |   92  |    0    |
   |   store instead of before it                      |       |         |

   `s32 dx/dy/dz' is b19's own measured fact and it HOLDS: the three stack
   parameters keep their incoming MEMs and are reloaded at each use instead
   of being held in $s2/$s3/$s4, which costs NINE WORDS. `s16 b' is this
   body's own: as `s32' it is 2 references against `c's 3, `c' takes $s5 and
   the two exchange for 8 rows; the sub-word type buys `b' the extra
   reference that ties them, and the tie breaks on declaration order. The
   caller passes constants in every one of those slots, so the widened
   prototype emits no conversion -- VERIFIED by compiling the whole unit both
   ways: func_b23_8017364C's assembly is byte-identical and this function is
   the ONLY one in the unit whose bytes change.

   The hoist row is the shorter-body test from the brief: hoisting makes it 4
   words SHORT, so the target really does re-dereference `a->unk_08' at all
   three placements -- a call (rand) does not even separate them, two of the
   three are in one basic block and cse still does not unify them because the
   intervening `sh' through `n->unk_08' may alias.

   `--raw' is 9 differing: 5 `jal'/`j' and 4 `%hi'/`%lo' halves -- the
   func_b23_80170F0C and func_b23_80170D2C addresses -- so no masked word
   hides a constant. rand() is func_80069EF8 from common_syms.txt. */
void func_b23_80171020(u8 *a, s16 b, s32 c, s32 f, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b23_80170F0C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(*(u8 **)(a + 0x8) + 0x2) + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(*(u8 **)(a + 0x8) + 0x6) + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(*(u8 **)(a + 0x8) + 0xA) + dz;
        p = (u8 *)n + 0x20;
        *(s32 *)(p + 0x40) = ((rand() & 0x7FFF) - 0x4000) << 4;
        *(s32 *)(p + 0x44) = ((rand() & 0x7FFF) - 0x4000) << 4;
        *(s32 *)(p + 0x48) = ((rand() & 0x3FFF) - 0x6000) << 4;
        n->unk_08->unk_00 += *(s32 *)(p + 0x40) * 2;
        n->unk_08->unk_04 += *(s32 *)(p + 0x44) * 2;
        *(s32 *)(p + 0x54) = 0x8000;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = 0x14;
        *(s16 *)(p + 0x34) = 0x14;
        func_8004491C(n, (u32)func_b23_80170D2C);
        *(u32 *)p = c;
        *(u32 *)(p + 0x8) = c;
    }
}

/* The banked room script's SPAWNER for the state-9 actor: allocate a 0x112
   node off D_80083498, install func_b23_80171358 as the node's +0x10 driver,
   dress the slot from the bank-local D_80174E88 facing table, pick the
   +0x14/+0x1C flag pair from the low two bits of the caller's `a', and -- only
   for a==0 -- give one actor in two a random 0x20|rand palette out of
   func_800A48F0 and the OTHER bank-local table D_80174EF0. Then hand the
   node to func_800A9C18 and arm the +0x8C continuation func_b23_80171760.

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

   func_b23_80171358 and func_b23_80171760 are address-taken here and are
   still INCLUDE_ASM stubs below, so both need a declaration above.
   THIS COPY'S OWN CONSTANT: 0x9 -> 0x17.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_b23_80171190(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b23_80171358;
        p[0x13] = 23;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80174E88;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_80174EF0;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b23_80171760;
        func_800AA36C(w, m, r, p);
    }
    return p;
}

/* The banked room script's per-frame actor driver, the same relinked twin of
   the resident overlay's func_800D8728 (csrc/ovl_dungeon_800D50FC.c) that
   landed as func_b03_80170A28 and its thirteen siblings: past the engine's
   0x2000 gate it runs func_800A9E70, then the record's own +0x8C
   continuation, then the +0x9A entry of this bank's D_80174F00 facing table,
   re-aims the tile when the +0x6D facing moved under it, re-skins the slot
   for the eight-way facing, steps the mover by its velocity, runs the +0x9D
   bob counter and leashes +0x92/+0x88 to the ground height func_800BCB04
   reports. Bank-local deltas from func_b03_80170A28: the +0x8C gate compares
   against func_b23_80171760 and the facing table is D_80174F00.

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
void func_b23_80171358(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b23_80171760)
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
    D_80174F00[p[0x9A]](p, b, c, p);
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

/* The banked room script's IDLE handler and this bank's default +0x8C
   continuation: the three early bails, the D_80174E88 facing-table dress,
   and then the twelve-way dispatch on `*(u16 *)(d + 0x46) & 0x3FFF'.
   350 words, 0 differing.

   IT OWNS A JUMP TABLE: jtbl_b23_80170808, 12 entries at rodata offset 0x0,
   8-aligned, no pad (0x30 bytes, and D_b23_80170838 follows it). Read off
   the listing, index = (that mask) - 1: cases 1 2 3 -> the func_800AAF00
   arm, 4 10 11 -> `default' (so they carry no label of their own), 5 6 7 ->
   the func_800A0818 / D_800814A8 arm, 8 -> func_b23_801726D0 then
   func_b23_80172894, 9 -> func_b23_8017476C, 12 -> func_800A9A0C, written
   as a `case' label inside the `w[0x9A] != 0x11' arm.

   TWO LANDED CARVED TWINS, and the body is a BLEND of them. The statements
   are func_b31_801716F4's (csrc/ovl_dng_b31.c, 365 words) MINUS its 0x80000
   arm's second dress and `*(s32 *)(p + 0x90) = 0' -- which is exactly the
   -15 words -- i.e. that arm is func_b37_80170E68's (csrc/ovl_dng_b37.c,
   345 words) two calls and nothing else. b31 is the one that carries the
   state-0xE `(rand() & 0x1F) + 0xF' seed, which is the +5 over b37.
   The relink is a rename, read off the listing: b31's D_801755A4 ->
   D_80174EF0, D_8017559C -> D_80174EE8, D_80175554 -> D_80174E88,
   D_80175564 -> D_80174E98, D_80175594 -> D_80174EE0, func_b31_80171CA8 ->
   func_b23_80171CD8, func_b31_80171EEC -> func_b23_80171F1C,
   func_b31_8017430C -> func_b23_80174598, func_b31_8017298C ->
   func_b23_801729AC, func_b31_801726B0 -> func_b23_801726D0,
   func_b31_80172874 -> func_b23_80172894, func_b31_80174FE4 ->
   func_b23_8017476C. rand() is func_80069EF8 and needs no declaration.

   THE ONE SPELLING FACT, and it is NEITHER twin's: the three gates that end
   in func_b23_80171F1C must `goto' ONE shared block after the switch.
   Written out as three `func_b23_80171F1C(p, b, c, d); return;' copies the
   body is 350 words and 66 ALIGNED DIFFERENCES, every one a $s0/$s1
   exchange between `p' and `c'. This is the fault func_b16_80171014
   (csrc/ovl_dng_b16.c) documents at 65 differences for the same cause, and
   its fix is the one that works here: sharing the block drops two of `c's
   references, which takes `c' from 38 refs / 528 insns to below the 32-ref
   `floor_log2' cliff in allocno_compare and hands `p' $s0.

   Measured on this body with `tools/cdkgcc -da' and priced both ways:

   | spelling                                          | words | aligned |
   | as written below (one shared `deflt' block)       |  350  |    0    |
   | three written-out func_b23_80171F1C copies        |  350  |   66    |
   | those three plus `h = p' at the top, `h' used     |  350  |    5    |
   |   everywhere (fixes the rank, moves the prologue  |       |         |
   |   copy to the END of the four parameter moves)    |       |         |
   | those three plus `p = D_80082E80' reused as the   |  349  |   12    |
   |   tail pointer (fixes the rank; p then carries a  |       |         |
   |   symbolic reg_known_value, canon_rtx kills the   |       |         |
   |   sp-vs-pointer dependence and the outgoing       |       |         |
   |   `sw $v0, 0x10($sp)' sinks into the jal's delay  |       |         |
   |   slot the target leaves as a `nop')              |       |         |
   | the same with the tail kept as D_80082E80[...]    |  349  |   12    |

   So the residue DOES NOT constrain how the rank is won -- three spellings
   win it -- but only the shared block wins it for free. `--raw' is 82
   differing and that is exactly the relocation count (24 `jal', 13 `j', 25
   `%hi', 20 `%lo'), so no masked word hides a constant address. Every
   `%hi'/`%lo' symbol was read back out of the listing per site: D_80174EF0
   is the compare table at BOTH the d[0x25] bail and the 0x200 arm,
   D_80174EE8 the table stored at the d[0x25] dress and passed to
   func_800AA924, D_80174E88 the state-0xE dress AND the final dress,
   D_80174E98 func_800AA6B4's and D_80174EE0 func_800AAF00's. */
void func_b23_80171760(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_b23_80171CD8(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174EF0)
        {
            e = D_80174EE8;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174EF0)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80174EE8) != 0)
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
            e2 = D_80174E88;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            *(s16 *)(p + 0x9E) = 0;
            *(s16 *)(p + 0xA0) = (rand() & 0x1F) + 0xF;
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174E98) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b23_80174598(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
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
            if ((s16)func_b23_801729AC(p, b, c, 0) == 0)
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
            if ((s16)func_b23_801726D0(p, b, c, d) != 0)
            {
                return;
            }
            func_b23_80172894(p, b, c, d);
            return;
        case 9:
            func_b23_8017476C(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80174EE0, func_b23_80171760);
            return;
        default:
            goto deflt;
        }
    deflt:
        func_b23_80171F1C(p, b, c, d);
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
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    e3 = D_80174E88;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_80174E80 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b23_80172664.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b23_80172664 is called only on
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
void func_b23_80171CD8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80174E80)
        {
            *(u8 **)(c + 0x2C) = D_80174E80;
            func_80047784(c, D_80174E80[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b23_80172664(p, b, c, d);
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

/* The banked room script's walk step, the relinked twin of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c) with three extra
   pre-loop arms: the +0x400/+0x10 pair that re-aims through func_800A02AC or
   func_800A04F0, and the +0x2000/+0x20000 arm that compares the cursor
   against D_80082E80's. Everything after that is func_800D92C0's eight-step
   D_8006CD00 sweep. 466 words, 0 differing.

   THE SECOND PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED
   AND PASSED ON. func_b23_801726D0's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A66C 2 words, func_b23_801726D0 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b23_80171F1C(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b23_801726D0(p, b, c, d) == 0)
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

/* State 0x10 with a 0x9E countdown of 2, and hand c the bank-local
   D_80174EA0 byte the facing at d+0x2A selects, through func_80047784 rather
   than the resident twin's func_8003DB94. 27 words, 0 differing.
   Shape is func_800D9820's (ratio 0.923): the table base is a POINTER LOCAL
   because it is used twice -- stored at c+0x2C and indexed -- but here the
   table is bytes and the index a plain subscript (`sra 9`, `andi 7`), not the
   resident's byte offset into a pointer array. */
void func_b23_80172664(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    *(s16 *)(p + 0x9E) = 2;
    t = D_80174EA0;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174E88 facing table and re-aim through func_8009C93C.

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
s32 func_b23_801726D0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174E88;
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
   tests, put the actor into state 0x11, swap the D_80174E88 facing table
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
   D_80174E88 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b23_80172894(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174E88;
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
s32 func_b23_801729AC(u8 *a, void *b, void *c, s32 d)
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
        func_b23_80171F1C(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b23_80171F1C(p, b, c, p);
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
   installed handler is the bank-local func_b23_80171760. 31 words,
   0 differing, first compile. */
void func_b23_80172AF0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b23_80171760;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's four-state lift, switching on p[0x9B]: state 0
   counts the +0x9E timer down, then swaps in the D_80174EA8 facing table and
   re-arms +0x9E at 5, state 1 drives the two-numerator divide into the
   UnkVec and the func_800644B8 sub-step at +0xA4 and leans the sprite by an
   inner switch on the remaining +0x9E, state 2 settles onto the tile and
   swaps in D_80174EB0, state 3 only parks the wind at 0x1000, and the shared
   tail re-centres, runs func_800AD594 / func_800A4ACC, decrements
   D_80083460 + 0x8 and installs func_b23_80171760. 267 words, 0 differing.

   THE TWIN IS THE LANDED func_b42_80172AD8 (csrc/ovl_dng_b42.c, 242 words,
   dngtwin 0.853), SAME FRAME -- addiu $sp, -0x38, $s0/$s3/$s1/$s2 + $ra
   saved in that order, x at sp+0x18. Two shape deltas, both read off the
   listing: b42's case-0 gate is `(*(u16 *)(c + 0x14) & 0x6000) == 0' where
   this one counts +0x9E down, and b42's four loose pairs of +0x1C/+0x1E wind
   stores become ONE INNER SWITCH on `*(s16 *)(p + 0x9E)' here -- cases 5, 4,
   3 and 2 writing 0xC00/0x1400, 0xA00/0x1600, 0xE00/0x1200 and
   0x1000/0x1000. jump.c cross-jumps the four +0x1E stores into the one
   shared `sh $v0, 0x1E($s1)' the target has at .Lb23_80172D78; do not write
   that sharing by hand.

   THE SPELLING FACT, worth 2 words: THE INNER SWITCH'S ARMS GO IN DESCENDING
   ORDER, 5 4 3 2. gcc lays the arm BODIES out in source order while the
   decision tree is balanced on value, so with the natural 2 3 4 5 the {4,5}
   node reaches case 4 by `beq' and case 5 by a second `beq' -- 269 words /
   15 aligned differences -- where the target reaches case 4 by `beq' and
   FALLS INTO case 5 past a `bne' to `default'. Only case 5 first buys that
   fallthrough.

   ALL THREE OF b42's NAMED FACTS HOLD, re-measured here, and two reproduce
   its figures to the digit:

   | spelling                                          | words | aligned |
   | as written below                                  |  267  |    0    |
   | the inner switch written 2 3 4 5                  |  269  |   15    |
   | the tail gate's two arms swapped                  |  267  |   17    |  (b42: 17)
   | 0x410 and 0x20000 folded into one `&&'            |  267  |   10    |  (b42: 10)
   | the first numerator as ONE expression, no u / w   |  267  |    1    |  (b42: 1)
   | case 2's two wind stores as +0x1C then +0x1E      |  267  |    2    |
   | case 0's countdown as a test then a separate      |  269  |   12    |
   |   decrement in both arms                          |       |         |

   The case-2 store order is this body's own fact and runs the OTHER way from
   the tail's: inside case 2 the target writes +0x1E first and in the shared
   tail it writes +0x1C first, with the same 0x1000 in both.

   `--raw' is 32 differing against a relocation count of 33 read off the
   listing (11 `jal', 8 `j', 7 `%hi', 7 `%lo'), so no masked word hides a
   constant address. Per-site symbols from the listing: D_80174EA8 dresses in
   state 0, D_80174EB0 in state 2, and func_b23_80171760 -- landed above in
   this unit -- is what the tail parks in +0x8C. */
void func_b23_80172B6C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if ((*(s16 *)(p + 0x9E) -= 1) > 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_80174EA8;
        func_80047784(c, D_80174EA8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 5;
        *(s32 *)(p + 0xA4) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA4);
        if (*(s16 *)(p + 0x9E) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x9E);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x9E);
            *(s32 *)(p + 0xA4) = -func_800644B8(*(s16 *)(p + 0x9E) * 409) << 9;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA4);
        switch (*(s16 *)(p + 0x9E))
        {
        case 5:
            *(s16 *)(c + 0x1C) = 0xC00;
            *(s16 *)(c + 0x1E) = 0x1400;
            break;
        case 4:
            *(s16 *)(c + 0x1C) = 0xA00;
            *(s16 *)(c + 0x1E) = 0x1600;
            break;
        case 3:
            *(s16 *)(c + 0x1C) = 0xE00;
            *(s16 *)(c + 0x1E) = 0x1200;
            break;
        case 2:
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(c + 0x1E) = 0x1000;
            break;
        }
        if ((*(s16 *)(p + 0x9E) -= 1) < 0)
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
            *(s16 *)(c + 0x1E) = 0x1000;
            *(s16 *)(c + 0x1C) = 0x1000;
            *(u8 **)(c + 0x2C) = D_80174EB0;
            func_80047784(c, D_80174EB0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
        break;
    case 3:
        *(s16 *)(c + 0x1C) = 0x1000;
        *(s16 *)(c + 0x1E) = 0x1000;
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
    *(s16 *)(c + 0x1C) = 0x1000;
    *(s16 *)(c + 0x1E) = 0x1000;
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
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
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
void func_b23_80172F98(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b23", func_b23_80172FFC);

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_b23_80172FFC. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b23_80170838[] = {
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

/* The actor's +0x9B carry machine, FOUR states where the landed
   func_b03_80172A24 / func_b14_8017293C have three: state 0 picks the slot
   triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state 1
   waits on func_8003F270 and plays 0x703, state 2 runs the 0x13-frame
   countdown and, while +0x96 is in 9..14, sprays ten func_b23_80171020
   particles, state 3 is the hand-back that re-installs func_b23_80171760.
   320 words, 0 differing, first compile. No xsibs twin at ratio 0.70; found
   by grepping the callee set (func_800A05A4 / func_800A94A0 / D_8006DE24 /
   D_800814A8 / func_8003F270 / D_8008346C), which is a nine-member family.

   THIS FUNCTION OWNS jtbl_b23_80170858, the 7-entry table at 0x80170858 for
   state 0's FIRST switch -- cases 1,2,3,5,6,7 over the 0x3FFF field with
   case 4 falling to `default'. The switch on p[0x9B] itself is a compare
   chain, and so is the three-case `else' switch, so the unit gets ONE new
   table. It lands immediately after const D_b23_80170838 (0x80170838,
   0x20 bytes), whose position in this file is what puts it there, and its
   `.align 3' costs no padding because 0x80170838 + 0x20 is already
   8-aligned.

   Four facts inherited from func_b03_80172A24's comment, all re-measured
   here as part of the first clean compile, not assumed:
     `k' is `s16'. That is what buys the target's `addu $v0, $s4, $zero'
       ahead of the `beqz'.
     THE INNER THREE-CASE SWITCH IS WRITTEN 3, 2, 1. Cross-jumping merges
       the table switch's 1/2/3 arms into these, so their emission order is
       what the table's `j's point at.
     `D_8008346C = 0' GOES BEFORE THE D_800814A8 DECREMENT, so the `lw' of
       D_800814A8 has the `sw' of D_8008346C to fill its load delay.
     THE D_8006DE24 RECORD NEEDS BOTH THE POINTER LOCAL AND THE INDEX LOCAL,
       so the symbol is materialised before the stride-0x14 multiply.

   Five deltas from func_b03_80172A24, all read off the listing:
   (1) There is NO SVECTOR and no func_800DA840 publish -- state 0 goes
   straight from the sign fixes to func_800A94A0, and on success arms
   `*(s16 *)(p + 0x96) = 0x13' instead.
   (2) func_800A56E0(0x703) is state 1's, after `p[0x9B]++', not state 0's.
   (3) State 2's gate is `n <= 0 || (c+0x14 & 0xE000) != 0' -- one `blez'
   INTO the arm and one `beqz' past it -- and the arm bumps p[0x9B], sets
   0x800 and reloads +0x96 with 0xF.
   (4) The particle burst's range test is `+0x96 >= 9 && +0x96 <= 14', which
   fold turns into the target's single `addiu -9' / `sltiu 6', and the loop
   counter is `s16' -- the `sll 16'/`sra 16'/`slti 0xA' triple with the
   `addu $s0, $v0, $zero' copy is what a narrow counter in a callee-saved
   register costs.
   (5) State 3 is func_b03_80172A24's state 2 with a `g2+0xC == 0' head that
   zeroes the countdown, the facing table swapped for the bank-local
   D_80174E88 through a POINTER LOCAL (the target indexes the held base, it
   does not re-read c+0x2C), the `c+0x14 &= 0xF7FF' moved INSIDE that arm,
   and d[0x6D] decremented unguarded. */
void func_b23_8017364C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    u8 *t;
    s16 k;
    s16 n;
    s16 i;
    s32 ix;
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
                ix = g[0];
                e = (u8 *)&tbl[ix];
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
            if (func_800A94A0(d, g, k, p + 0x98) == 0)
            {
                break;
            }
            *(s16 *)(p + 0x96) = 0x13;
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
            *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
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
        func_800A56E0(0x703);
    case 2:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0 || (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
            p[0x9B]++;
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0xF;
        }
        if (*(u16 *)(p + 0x96) >= 9 && *(u16 *)(p + 0x96) <= 14)
        {
            i = 0;
            do
            {
                func_b23_80171020(p - 0x20, 0, 0xC0C0, (rand() & 0xFF) | 0x80, 0, 0, 0);
                i++;
            } while (i < 0xA);
        }
        break;
    case 3:
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) == 0)
        {
            *(s16 *)(p + 0x96) = 0;
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
        t = D_80174E88;
        if (*(u8 **)(c + 0x2C) != t)
        {
            *(u8 **)(c + 0x2C) = t;
            func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
        func_800A4ACC(d);
        d[0x6D] -= 1;
        d[0x73] = 0;
        d[0x72] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}

/* The 0x800D1C70..0x800D4634 state machine with a FOURTH state. Same base as
   the resident func_800D9C7C (csrc/ovl_dungeon_800D50FC.c) and the carved
   func_b01_80173294: the same switch on p[0x9B], the same `stop:' label
   reached from state 0's `else' by `goto', the same func_800AAA54 bail-out,
   the same divide-by-step-count state and the same D_80083460+0x10 /
   handler-install tail. Five deltas, all read off the listing:

   (1) The seeds have MOVED OUT of state 0 into state 1 and are driven by the
   countdown instead: state 0 only arms `0x96 = 4', state 1 decrements and
   scales by 19 while the counter is still positive and by 18 on the tick it
   reaches zero. There is no `-= D_8006CCD8[k] << 14' decay and no quarter
   decay anywhere, so state 1 does not fall through from state 0 -- it breaks.
   (2) The D_8006CCD8 index is NOT a local: all four lookups recompute
   `(*(u16 *)(d + 0x6A) >> 9) & 7' and each gets its own `%hi'/`%lo'.
   (3) The counter's two reads are NOT the same form and are not
   interchangeable. State 1 wants the compound assignment's VALUE in an s32
   (`sll 16' AND `sra 16', because `n' is tested twice -- `blez' then
   `bnez'); state 2's tail wants a POST-decrement, which shifts the OLD
   `lhu' (`sll $v0' with the new value already in $v1) where a pre-decrement
   shifts the new one.
   (4) state 2's divide is `<< 16', not func_b01_80173294's `<< 15' -- and it
   keeps that body's split numerators: the first three-statement
   (`goal = c[0x24] << 6; now = b2 - 0x20; goal -= now;'), the second left
   two-statement.
   (5) func_800AAA54's fourth argument is the bank-local D_80174ED8, and the
   tail carries an extra arm the resident has not got: if c+0x2C still holds
   D_80174E98 it is swapped for D_80174E88 and re-sent through
   func_80047784, the same pointer-local shape as func_b23_80172664 above.
   The installed handler is the bank-local func_b23_80171760.

   198 words, 0 differing. */
void func_b23_80173B4C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 n;
    s32 goal;
    s32 now;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        *(s16 *)(p + 0x96) = 4;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 3;
            }
        }
        else
        {
            goto stop;
        }
        return;
    case 1:
        n = (*(s16 *)(p + 0x96) -= 1);
        if (n > 0)
        {
            b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 19;
            b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 19;
        }
        else if (n == 0)
        {
            b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
            b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
            *(s16 *)(p + 0x96) = 6;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80174ED8);
            return;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 16) / *(s16 *)(p + 0x96);
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 16) / *(s16 *)(p + 0x96);
        }
        if ((*(s16 *)(p + 0x96))-- > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        p[0x9B] += 1;
        return;
    case 3:
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) == D_80174E98)
        {
            t = D_80174E88;
            *(u8 **)(c + 0x2C) = t;
            func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b23_80173E64(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b23_80173E84(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b23_80173EA4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b23_80173EC4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}

/* The banked room script's three-state tail, switching on the state index at
   p + 0x9B: the relinked twin of the resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c), which is this body statement for statement
   with four local changes -- two separate bank-local facing tables
   (D_80174EF0 in state 0, D_80174EF8 in state 1's re-dress) where the
   resident uses D_800E262C for both, func_80047784 for func_8003DB94,
   func_b23_80174598/func_b23_80171760 for func_800DA660/func_800D8C64, and
   NEITHER of the twin's two `*(s32 *)(d + 0x1C) &= ~0x200' clears: state 1's
   0x8000 arm and state 2 install the handler and nothing else.
   228 words, 0 differing.

   STATE 1 OPENS WITH A func_80042900 GATE THE RESIDENT TWIN DOES NOT HAVE,
   and its false arm is the re-dress block the long refusal chain also falls
   into -- so the whole chain is the body of
   `if ((s16)func_80042900((s8 *)d, 1) != 0)' and the re-dress follows it.

   THE `p[0x9B] += 1;' TAIL IS WRITTEN OUT PER ARM, NOT `goto'-ed, and
   jump2's cross-jumping puts the single copy back: as `goto bump' it is
   226 words / 29 aligned differences, because with one bump block the two
   `sh' stores either side of it come out in the same registers and
   cross-jumping merges them too, costing state 0's `sh $v0, 0xA($v1)' and
   the load-delay `nop' the target has. Written per arm the two `sh's differ
   ($v1 address/$v0 value in state 0, $v0 address/$v1 value in state 1), the
   merge stops at the bump, and the word count is the target's.

   DEVICE: state 0's bump is wrapped in a `do { } while (0)'. Without it the
   body is 228 words / 18 aligned differences and every one of them is `p'
   and `b' holding each other's callee-saved register -- the target has
   p = $s2, b = $s3 and the plain body gives b = $s2, p = $s3. flow.c counts
   a reference once per enclosing loop depth, so the zero-trip loop makes
   state 0's two `p' references count twice, and that is enough to lift `p'
   past `b' in allocno_compare without lifting it past `c' and `d'. The
   cancelled pointer pair `p++; p--;' after the declarations overshoots --
   it also gives `p' a second set, which turns off update_equiv_regs'
   live-length doubling, and `p' comes out in $s0 ahead of `d': 49 aligned
   differences. Measured and equal at 0 differing, so the exact spelling of
   the wrapped statement is not the lever: `++p[0x9B];' inside the
   do-while, and widening it to wrap the `*(u16 *)(h + 0xA) -= 1;' as well.
   Measured and rejected: the do-while around state 1's bump instead is
   226 words / 11 differences and around both bumps 226 / 37;
   `p[0x9B] = p[0x9B] + 1;' long-hand in state 0 and declaring `b' as
   `void *' are both 18, i.e. no change at all.

   `&D_80083460' is FOUR separate pointer locals, one per arm, as in both
   resident twins; sharing state 0's with state 1's re-dress is 226 words /
   30 differences. */
void func_b23_80173EFC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
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
        *(u8 **)(c + 0x2C) = D_80174EF0;
        func_80047784(c, D_80174EF0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        do
        {
            p[0x9B] += 1;
        } while (0);
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
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
                func_b23_80174598(p, (u8 *)b, c, d);
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
        }
        *(u8 **)(c + 0x2C) = D_80174EF8;
        func_80047784(c, D_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_80174EF0 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b23_80171760. The resident
   overlay's func_800DA398 (csrc/ovl_dungeon_800D50FC.c) is the same machine
   over the same record and its comment covers the shape; the landed
   func_b05_80173BEC is the nearer twin, since like this copy it opens state 1
   with the same d[0x25] test that closes it, so the dress/bump tail is
   written out twice. Representative of a 10-member family.

   THIS COPY CARRIES NO FLAG UPDATES. The resident twin's
   `*(s32 *)(d + 0x1C) |= 0x40000` at both state-1 dress sites and its
   `&= ~0x208` in state 2 are absent here: writing them in, which is exactly
   func_b05_80173BEC's body, is 207 words with 142 differing.

   THE `p[0x9B] += 1;` BUMP MUST BE WRITTEN OUT IN ALL THREE REACHING ARMS,
   NOT REACHED BY A SHARED `goto` LABEL, and that is the only thing that makes
   this function land. jump2 cross-jumps the three copies back into the one
   the target has, so they cost no code -- but flow counts them first, and
   they are worth four references on `p`. One shared label is 195 words with
   15 differing, EVERY ONE OF THEM A REGISTER: `p` takes $s3 and `b` takes
   $s2, the other way round from the target. Two copies -- the opening arm
   reaching the tail's label -- is the same 195 words and the same 15.

   The cause is read off the compiler, not guessed. global.c's
   allocno_compare ranks on floor_log2 (n_refs) * n_refs / live_length, and
   from `.flow` one bump copy gives `p` 9 references over 173 insns against
   `b`'s 5 over 47:

       p = 3 *  9 / 173 = 0.156     loses to
       b = 2 *  5 /  47 = 0.213

   so `p` is allocated second. Three copies put `p` at 13 references over 177
   insns -- 3 * 13 / 177 = 0.220 -- and it wins. The threshold either side was
   solved before compiling: 12 references scores 3 * 12 / 177 = 0.203 and
   still loses, which is why two copies cannot work and three must.
   (docs/matching-levers.md, "HOW MANY REFERENCES YOU NEED IS AN ARITHMETIC
   PROBLEM, NOT A SWEEP".)

   `&D_80083460` IS FOUR SEPARATE POINTER LOCALS, one per arm. Sharing one
   across the arms is 191 words with 146 differing -- the gate load sinks and
   the arms merge.

   The facing tables are NAMED DIRECTLY at all three dress sites. One shared
   `u8 *` local for the three is 195 words with 6 differing, the `%hi`/`%lo`
   pair splitting across registers at each site; three separate locals, one
   per site, also measure 0 and the bare symbols are kept because they need no
   extra declaration.

   Other spellings measured and rejected: `d[0x6D]` read as `u8` rather than
   `*(s8 *)(d + 0x6D)` is 1 differing, an `lbu` for the target's `lb`;
   `*(s32 *)(d + 0x64)` for the `lh` of d + 0x64 is 1 differing; dropping the
   `(s16)` cast on the first func_800A2C34 result is 194 words with 103
   differing. Measured and equal at 195 words / 0 differing, so none of these
   is the reason it lands: re-reading d + 0x1C at both flag tests instead of
   through the `v` local, writing state 2 as a positive `if` arm instead of an
   early return, nesting the d + 0x64 test instead of `&&`, and the three
   separate table locals above.

   195 words, 0 differing. */
void func_b23_8017428C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174EF0;
        func_80047784(c, D_80174EF0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174EF8;
            func_80047784(c, D_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
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
            func_b23_80174598(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174EF8;
        func_80047784(c, D_80174EF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
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
   call emits the bare `jal` either way); hoisting D_80174E80 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b23_80174598(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174EF0 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80174E80;
        func_80047784(c, D_80174E80[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b23_80174660(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b23_80171760;
        }
    }
}

/* 14 words, 0 differing. */
void func_b23_801746C4(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}

/* 14 words, 0 differing. */
void func_b23_801746FC(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}

/* 14 words, 0 differing. */
void func_b23_80174734(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}

/* The 0x17 hand-off: clear d+0x71's top bit, bail on D_80083462's 0x2000 and
   on func_800A2B5C either side of the func_800C7930 push, then install state
   0x17 with a null +0x8C handler and a 0xF countdown at +0x96, dress the
   renderable from this bank's D_80174EC8, raise 0x800 in c+0x14, run
   func_800A4ACC, spend one of d+0x6D and park in d+0x60 whatever
   func_8009B25C finds on the tile one step along d+0x2A's facing.
   89 words, 0 differing. Reached from case 9 of func_b23_80171760's jump
   table, landed above in this unit.

   THE TWIN IS THE LANDED func_b28_8017236C (csrc/ovl_dng_b28.c, 64 words) --
   NOT dngtwin's top row, which is a stub, nor its func_b28_80171FD4 at
   0.792; 8017236C sits four listings further down the same file and is the
   same state-0x17 body. The whole 89 - 64 delta is additive and read off
   this listing: the 0xF store at p + 0x96, the `*(u16 *)(c + 0x14) |= 0x800'
   after the dress, and the func_8009B25C tail. func_800A4ACC and the
   `d[0x6D] -= 1' are b28's; only the ORDER differs (b28 runs func_800A4ACC
   first).

   THE SPELLING FACT, worth 2 words of search: THE INDEX IS
   `(*(u16 *)(d + 0x2A) >> 9) & 7' WITH `(u16)D_8006CCD8[k]', the resident
   func_8008C8BC spelling (csrc/ovl_dungeon.c). The listing's `srl 8' /
   `andi 0xE' LOOKS like the byte-offset idiom csrc/ovl_dng_b12.c records --
   combine folds `((x >> 9) & 7) << 1' into exactly those two insns -- but
   writing that idiom out leaves the `andi' scheduled three insns early,
   before the `sb $v0, 0x6D($s0)' instead of after the table's
   `addiu %lo', because it then belongs to its own statement rather than to
   the array index.

   Measured on this body, both ways:

   | spelling                                          | words | aligned |
   | as written below                                  |   89  |    0    |
   | `k = (d+0x2A >> 8) & 0xE' + *(u16 *)(k + (s32)T)  |   89  |    2    |
   |   (the b12 byte-offset idiom; symbol-first and    |       |         |
   |    mask-inline variants are 2 as well)            |       |         |
   | b12's write-back local `t1 = (s32)T; t1 += k;'    |   89  |   18    |
   | `*(s32 *)(p + 0x8C) = 0' after BOTH byte stores   |   89  |    2    |
   | func_800A4ACC before the c + 0x14 flag            |   89  |    4    |
   | `*(s16 *)(p + 0x96) = 0xF' after p[0x9B] = 0      |   89  |    0    |
   | no `t' pointer local, D_80174EC8 at both sites    |   89  |    0    |
   | the three gates as one `&&' chain + `else return' |   89  |    0    |

   b28's ONE named fact HOLDS and reproduces its figure: the 0x8C word store
   must sit between the p+0x9A and p+0x9B byte stores, and after both of them
   the `sw' and the second `sb' come out transposed -- b28's own 2, and 2
   here. Its two free spellings are free here too, and `t' is free on this
   body where b28 keeps it, so that one is NOT the reason this lands.

   `--raw' is 16 differing: 6 `jal' and 10 `%hi'/`%lo' halves, no `j' at all
   (every bail branches straight to the epilogue), so no masked word hides a
   constant address. Per-site symbols from the listing: D_80174EC8 is the
   only facing table, and D_8006CCD8 / D_8006CCE8 are the resident
   step-vector pair, not bank-local. */
void func_b23_8017476C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 k;

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
    p[0x9A] = 0x17;
    *(s16 *)(p + 0x96) = 0xF;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    t = D_80174EC8;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(u16 *)(c + 0x14) |= 0x800;
    func_800A4ACC(d);
    d[0x6D] -= 1;
    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    *(u8 **)(d + 0x60) = func_8009B25C(d, c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k],
                                       *(s16 *)(d + 0x88));
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b23", func_b23_801748D0);

/* 14 words, 0 differing. */
void func_b23_80174E10(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}

/* 14 words, 0 differing. */
void func_b23_80174E48(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b23_80171760;
    }
}
