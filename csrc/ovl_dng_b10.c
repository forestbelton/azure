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
s32 func_b10_801727B8(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80175988[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80175980[];
extern u8 D_801759E8[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b10_80171FE8(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b10_801717F4();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);

extern u8 D_801759F0[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b10_80174258(u8 *p, u8 *b, u8 *c, u8 *d);

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
void func_b10_80171104(u8 *, void *, u8 *, u8 *);

extern s16 D_80175A18;
s32 func_800453E0(u8 *, UnkVec *, u8 *, s16);

extern s32 D_80175A84;
void func_800478B8(u8 *);

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

/* func_800D5AC0 (csrc/ovl_dungeon_800D50FC.c), byte for byte: one
   semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, whose layout here is the
   twin's exactly (+0x1C cursor, +0x24 OT base, +0xD0/+0xD4 out-parameters,
   +0x100 index). The three dead lbu's at +0x4/+0x5/+0x6 are the
   setRGB0(t, t->r0, t->g0, t->b0) echo of the colour word just copied in;
   the twin's own comment carries the measurements for that and for its three
   load-bearing orderings, which were not re-measured here because the first
   compile of the twin's body landed. 120 words, 0 differing. */
extern u8 D_801759D8[];

extern u8 D_801759A0[];

extern const u8 D_b10_80170854[];
extern s32 D_8008346C;
void func_800A2B04(UnkVec *, s32, s32);
s32 func_800A56E0(s32);
void func_b10_80174670(u8 *p, u16 x, u16 y, s16 z);
void func_b10_8017442C(u8 *p, u16 x, u16 y, s16 z);

s32 func_800644B8(s32);
extern u8 D_801759A8[];
extern u8 D_801759B8[];

void func_b10_80172730(u8 *p, UnkVec *b, u8 *c, u8 *d);

UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_8003DB94(void *, void *, s32);
extern u8 D_800DEC70[];
void func_b10_80170B0C(u8 *p, UnkVec *b, u8 *c);

void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_801759C8[];
extern u8 D_80175998[];

void func_800419EC(u16, u32);
void func_80099C58(s16, s16, s16, s16, s16);
u8 *func_8009C12C(u8 *, u8 *, s16, s16);

void func_b10_80171FE8(u8 *p, void *b, void *c, u8 *d);
extern s16 D_800DCF5E;
void func_b10_801751C0(u8 *p, void *b, void *c, u8 *d);
extern u8 D_801759E0[];
extern u8 D_801759D0[];
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
s16 func_8009FB34(u16 x, u16 y);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b10_80171D7C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b10_80172A94(u8 *a, void *b, void *c, s32 d);
void func_b10_8017297C(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b10_80174798(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern Unk41284 D_801759F8[];
extern u8 D_80175A80[];
extern u8 D_80175A81[];
extern s32 D_80175A00[];
extern s32 D_8006CD58;
extern u8 D_8003E140[];
extern u8 *D_800DCEEC[];
extern u8 D_800DCF5C;
extern u8 D_8014A000[];
void func_80041588(Unk41284 *, s8 *, s32);
u32 func_800445E0(void);
s32 func_800C77D0(void *, u8 *, s16, s32);
s32 func_800498A0(u8 *);
UnkNode *func_800ADC4C(u8 *, s32, s16, u8 *);
void func_800A18E8(s16, s16);
void func_8009A028(u8 *);
s32 func_800A504C(u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_b10_80174F24(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_b10_8017092C(u8 *p, UnkVec *v)
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

/* The +0x10 step of the 0x212 particle node that func_b09_80170DDC spawns:
   integrate the payload's velocity at +0x40..+0x48 into the mover and its
   acceleration at +0x4C..+0x54 into the velocity; kill the node once the
   mover's two integer halves have come within 0x10 of the spawn point at
   +0x36/+0x38; fade the three colour bytes by p[i] * p+0x32 / p+0x34 into
   the slot's +0xC..+0xE, wind the slot's +0x1C/+0x1E down by 0xC8 with a
   floor of 0, drop the +0x32 life by 8, re-register through func_800478B8
   and kill the node when the life runs out. 123 words, 0 differing.
   2 members (dngfam.py: this and func_b10_80170B0C).

   THIS WAS PARKED AT 6 ALIGNED DIFFERENCES, taken to 3 by a 45-minute
   permuter run and to 2 by two duplicated-arm devices
   (docs/dng-parks/, agent-h11). ALL OF THAT RESIDUE IS THE PROXIMITY TEST
   BEING WRITTEN AS TWO MANUAL BLOCKS. The park's shape --
   `u = b+0x2; t = p+0x36 - u; if (t < 0) t = -t; if (t < 0x10) { ... }'
   nested -- is 123 words and 6 aligned differences, RE-MEASURED HERE. One
   `&&' chain with an inline `abs()' on each half is 0:

     if (abs(*(s16 *)(p + 0x36) - *(s16 *)((u8 *)b + 0x2)) < 0x10 &&
         abs(*(s16 *)(p + 0x38) - *(s16 *)((u8 *)b + 0x6)) < 0x10)

   With no `t' and no `u' there is no local for the b + 0x2 load to be
   alias-dependent through, so the whole scheduling window the park was
   fighting -- and the allocno competition on `k' the permuter was fighting
   -- does not exist. `abs' expands to mips.md's two-word in-place `abssi2'
   arm, `bgez'/real-`nop'/`negu' on one register, which is what the target
   has. The lead was dngtwin.py's 0.773 func_b19_80170A64
   (csrc/ovl_dng_b19.c, 102 words, landed), whose comment names that arm.

   THE ONE OTHER LOAD-BEARING SPELLING, and the park's measurement of it
   survives the rewrite at a lower price: `k = *(u16 *)(c + 0x1C);
   k -= 0xC8;' as TWO statements gives the loaded value and `k' one
   quantity, the target's `lhu $a0' / `addiu $v0, $a0' / `addu $a0, $v0'.
   The single statement `k = *(u16 *)(c + 0x1C) - 0xC8;' is 123 words, 2
   aligned differences (the park measured 15 against 13 on its own body).
   `k' must stay `s16'. */
void func_b10_80170B0C(u8 *p, UnkVec *b, u8 *c)
{
    s16 k;

    b->unk_00 += *(s32 *)(p + 0x40);
    b->unk_04 += *(s32 *)(p + 0x44);
    b->unk_08 += *(s32 *)(p + 0x48);
    *(s32 *)(p + 0x40) += *(s32 *)(p + 0x4C);
    *(s32 *)(p + 0x44) += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x48) += *(s32 *)(p + 0x54);
    if (abs(*(s16 *)(p + 0x36) - *(s16 *)((u8 *)b + 0x2)) < 0x10 &&
        abs(*(s16 *)(p + 0x38) - *(s16 *)((u8 *)b + 0x6)) < 0x10)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    c[0xC] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    c[0xD] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    c[0xE] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    k = *(u16 *)(c + 0x1C);
    k -= 0xC8;
    if (k < 0)
    {
        k = 0;
    }
    *(s16 *)(c + 0x1C) = k;
    *(s16 *)(c + 0x1E) = k;
    *(s16 *)(p + 0x32) = *(u16 *)(p + 0x32) - 8;
    func_800478B8(c);
    if (*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn one of this bank's particle nodes from an existing node's mover: a
   0x212 node whose +0x10 step is func_b10_80170B0C, placed at the parent
   mover's position plus (dx, dy, dz - 0x14), dressed from D_80045340 and
   D_800DEC70, with `col' in the payload's +0x00 and +0x08 and a per-frame
   drift and acceleration laid in +0x40..+0x54.  145 words, 0 differing.

   NOT A RELINKED TWIN -- xsibs.py finds no sibling for this one at any
   ratio.  It is func_800B2230 (csrc/ovl_dungeon_800A4B88.c) grown a payload:
   same func_8003FC64(0x212) / +0x10 install / func_8004491C(n, D_80045340) /
   func_8003DB94(r, D_800DEC70, 0) skeleton, with the mover copy replaced by
   the three parent-relative stores and the six quotients.

   THE DIVISOR IS WRITTEN OUT AT EACH OF THE SIX DIVISIONS AND MUST NOT BE A
   LOCAL.  `(life / 8)' inline six times is 145 words, 0 differing: cse
   shares the one `sra'/`addiu 7'/`sra 3' chain and the three `div's, and the
   scheduler then has nothing to fill the three `mflo' delays with, which is
   where three of the target's `nop's come from.  A `k = life / 8;' local is
   139 words and 49 aligned differences -- the three divide chains batch, the
   six `nop's vanish, and the first quotient coalesces with its own dividend.
   Measured and rejected, all 49 aligned differences at 139 words unless
   stated: a `k' local with the three quotients inline, 49; a `k' local with
   explicit `qx'/`qy'/`qz' quotient locals, 49 (byte-identical to the
   previous -- cse makes them the same RTL); `k = (s16)life / 8' with an
   `s32 life' parameter, 49; a `t = life; k = t / 8;' pair, 49; `k' computed
   right after `v', 140 words / 54; `k' computed as the block's first
   statement, 139 / 82. */
void func_b10_80170CF8(UnkNode *p, s16 a, u32 col, s16 life, s32 dx, s32 dy, s32 dz)
{
    UnkNode *n;
    UnkSlot *r;
    u8 *v;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b10_80170B0C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)p->unk_08 + 0x2) + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)p->unk_08 + 0x6) + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)p->unk_08 + 0xA) + dz - 0x14;
        v = (u8 *)n + 0x20;
        *(s16 *)(v + 0x36) = *(u16 *)((u8 *)p->unk_08 + 0x2);
        *(s16 *)(v + 0x38) = *(u16 *)((u8 *)p->unk_08 + 0x6);
        *(s16 *)(v + 0x3A) = *(u16 *)((u8 *)p->unk_08 + 0xA);
        *(s32 *)(v + 0x40) = -(dx << 16) / (life / 8) / 2;
        *(s32 *)(v + 0x44) = -(dy << 16) / (life / 8) / 2;
        *(s32 *)(v + 0x48) = -(dz << 16) / (life / 8) / 2;
        *(s32 *)(v + 0x4C) = -(dx << 16) / (life / 8) / 4;
        *(s32 *)(v + 0x50) = -(dy << 16) / (life / 8) / 4;
        *(s32 *)(v + 0x54) = -(dz << 16) / (life / 8) / 4;
        *(s16 *)(v + 0x14) = a;
        *(s16 *)(v + 0x32) = life;
        *(s16 *)(v + 0x34) = life;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_0C[2] = 0;
        r->unk_0C[1] = 0;
        r->unk_0C[0] = 0;
        r->unk_14 |= 0xC;
        *(u32 *)v = col;
        *(u32 *)(v + 0x8) = col;
        func_8003DB94(r, D_800DEC70, 0);
    }
}

/* The banked room script's SPAWNER for the state-9 actor: allocate a 0x112
   node off D_80083498, install func_b10_80171104 as the node's +0x10 driver,
   dress the slot from the bank-local D_80175988 facing table, pick the
   +0x14/+0x1C flag pair from the low two bits of the caller's `a', and -- only
   for a==0 -- give one actor in two a random 0x20|rand palette out of
   func_800A48F0 and the OTHER bank-local table D_801759E8. Then hand the
   node to func_800A9C18 and arm the +0x8C continuation func_b10_801717F4.

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

   func_b10_80171104 and func_b10_801717F4 are address-taken here and are
   still INCLUDE_ASM stubs below, so both need a declaration above.
   THIS COPY'S OWN CONSTANT: 0x9 -> 0xa.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_b10_80170F3C(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b10_80171104;
        p[0x13] = 10;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80175988;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_801759E8;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b10_801717F4;
        func_800AA36C(w, m, r, p);
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b10", func_b10_80171104);

/* The +0x8C handler at the head of this bank's actor machine -- the relinked
   twin of the resident overlay's func_800D8C64 -- which the other states of
   this unit park at the actor's +0x8C and which func_800AAF00 re-installs
   from the switch below. Four early exits on the D_80083462 flag word and
   d+0x1C's bits, the +0x9A state byte set to 0xE, the facing re-dress from
   the bank-local byte tables, then either the inlined d+0x46 dispatch or the
   walk. 354 words, 0 differing, FIRST COMPILE. 1 member (dngfam.py).

   THE TWIN IS THE LANDED func_b09_80171728 (csrc/ovl_dng_b09.c, 363 words)
   AND IT WAS PICKED ON THE FRAME, NOT ON THE RATIO. dngtwin ranks
   func_b06_8017140C higher (0.937 against b09's 0.923), but b06's prologue is
   `-0x38' with FIVE saves ($s0..$s3 + $ra) and $s0 = $a0, while b09's is
   `-0x38' with SIX ($s0..$s4 + $ra) at the same slots IN THE SAME ORDER and
   the same $s1/$s3/$s0/$s2 = $a0/$a1/$a2/$a3 binding as this listing. Every
   one of b09's facts then held, with no inversion:

     * ONE TABLE LOCAL PER DRESS SITE -- e/e2/e3, not one shared. One shared
       local across the three sites is 354 words / 4 aligned differences,
       which is b09's own re-measured figure to the row.
     * THE case 5/6/7 ARM'S D_800814A8 IS A POINTER LOCAL `w' ASSIGNED BEFORE
       THE d+0x2A STORE, so the `lw's load delay is filled by that `sh'.
       Inlining the call into the store and writing
       `((u8 *)D_800814A8)[0x9A]' directly is 355 words / 89 aligned
       differences -- far worse here than the 16 b06 priced, because this
       body owns a jump table and the displaced block moves every table `j'.
     * THE STATE-0xE ARM'S TWO STORES COME BEFORE `p[0x9A] = 0xE', in the
       order +0x9E then +0xA0. The other order is 354 words / 2 aligned
       differences, again b09's exact figure.
     * THE WALK TAIL IS CALLED AND WRITTEN OUT AT ALL FOUR SITES. One shared
       `goto walk' label is 354 words / 69 aligned differences; reorg rebuilds
       the shared block from the four copies by itself.

   MEASURED AND FREE, so NOT a reason this lands: nesting the func_800AAB10
   test inside the func_800A1C58 one instead of `&&' -- 354 words, 0
   differing either way, which is b06's result on its own body too.

   THE RELINK AND SHAPE DELTA FROM b09, read off the listing:
     * the tables are D_801759E8 (the d[0x25] and 0x200 arms' comparand),
       D_801759E0 (what the d[0x25] arm stores and func_800AA924 is passed),
       D_80175988 (the state-0xE dress and the 0x80000 arm's dress),
       D_80175998 (func_800AA6B4) and D_801759D0 (func_800AAF00);
     * func_b09_80171CD4 / func_b09_80171F40 / func_b09_801729EC /
       func_b09_80172710 / func_b09_801728D4 / func_b09_80174928 /
       func_b09_801743E8 become func_b10_80171D7C / func_b10_80171FE8 /
       func_b10_80172A94 / func_b10_801727B8 / func_b10_8017297C /
       func_b10_80174798 / func_b10_80174258;
     * b09 DISCARDS func_800AAB10's result; this copy TESTS it and the arm
       calls func_b10_801751C0, the state-0x18 entry landed above in this
       same unit -- which is b18's/b06's shape, not b09's;
     * b09's FINAL DRESS IS ABSENT. Where b09 ends with a D_80083462 gate, a
       c+0x14 0x40 gate and a fourth dress off e4, this listing's epilogue
       follows the `sh $v0, 0x2A($s2)' directly. With the extra func_800AAB10
       arm that is the whole -9 word delta, and it is MISSING STATEMENTS, not
       missing references.

   THIS FUNCTION OWNS jtbl_b10_80170808, the 12-entry table at rodata offset
   0x0, for the switch over (d + 0x46) & 0x3FFF. Cases 1..12: 1,2,3 -> the
   func_800AAF00 arm; 4, 10 and 11 -> `default' (the walk), so they need no
   label of their own; 5,6,7 -> the func_800A0818 / D_800814A8 arm; 8 ->
   func_b10_801727B8 / func_b10_8017297C; 9 -> func_b10_80174798; 12 ->
   func_800A9A0C, written as a `case' label inside the `w[0x9A] != 0x11' arm.

   `--raw' is 80 differing and all 80 are relocations, so no masked word hides
   a constant address; every `%hi'/`%lo' symbol was read back off the listing
   and the five tables above are five DISTINCT symbols at five distinct
   sites. */
void func_b10_801717F4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_b10_80171D7C(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_801759E8)
        {
            e = D_801759E0;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_801759E8)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801759E0) != 0)
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
            e2 = D_80175988;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            *(s16 *)(p + 0x9E) = 0;
            *(s16 *)(p + 0xA0) = 0x14;
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80175998) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b10_80174258(p, (u8 *)b, c, d);
            e3 = D_80175988;
            *(u8 **)(c + 0x2C) = e3;
            func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(p + 0x90) = 0;
            return;
        }
        if ((s16)func_800A1C58(d) != 0 && (s16)func_800AAB10(p, b, c, d) != 0)
        {
            func_b10_801751C0(p, b, c, d);
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
            func_b10_80171FE8(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b10_80172A94(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b10_80171FE8(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b10_801727B8(p, b, c, d) != 0)
            {
                return;
            }
            func_b10_8017297C(p, b, c, d);
            return;
        case 9:
            func_b10_80174798(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_801759D0, func_b10_801717F4);
            return;
        default:
            func_b10_80171FE8(p, b, c, d);
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

/* The banked room script's step-the-walk handler: advance the actor one cell
   of its queued path, swap in the bank-local D_80175980 facing table,
   re-aim the sprite, and either stop at state 0xF or hand on to
   func_b10_80172730.  155 words, 0 differing.

   THE TWIN IS THE LANDED func_b07_80171570 (csrc/ovl_dng_b07.c), not the
   resident func_800D904C: b07 has already absorbed the relink and the
   inverted gate, so the only delta is ONE EXTRA ARM at the top -- the
   p[0x9A] != 0xF reset of p + 0x90, b->unk_14 and p[0x9D], which also
   splits b07's single `(s8)d[0x71] > 0 && d[0x71] > *(s16 *)(d + 0x8A)'
   gate into two nested ifs with the arm between them.

   TWO THINGS HAD TO BE RE-MEASURED AND BOTH MOVED THE NUMBER:

   * b07's PHANTOM ZERO IS STILL NEEDED.  `x + ((x + x) - x - x)' on
     func_800A0818's first argument (docs/matching-levers.md :11693 -- cse
     keeps the three insns, flow counts them as references on `x', combine
     deletes them, so it emits nothing).  Without it `d' outranks the `x'/`t'
     quantity in global-alloc and the two swap callee-saved registers:
     152 words, 50 aligned differences.  With it, 152 words and 19.
   * THE GATE MUST BE NESTED, func_b03_8017139C's WAY, NOT b07's FLAT
     `&&' CHAIN.  b07's `(s16)t == 3 && !(D_80083462 & 0x80) &&
     !(c[0x14] & 0x8000)' with 0xF as the single else is 152 words / 19 here:
     jump.c cross-joins the two `p[0x9A] = 0xF' stores, where the target
     keeps them apart (`sb $v0, 0x9A' off the inner gate's `addiu $v0, 0xF'
     and `sb $s5, 0x9A' off the CSE'd 0xF in $s5) and duplicates the
     `sw $zero, 0x8C($s4)' as well.  Nesting the inner two tests inside
     `if ((s16)t == 3)' with 0xF in both elses is 155 words, 0 differing. */
void func_b10_80171D7C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
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
            if (*(u8 **)(c + 0x2C) != D_80175980)
            {
                *(u8 **)(c + 0x2C) = D_80175980;
                func_80047784(c, D_80175980[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
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
                    func_b10_80172730(p, b, c, d);
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
                n = d[0x71];
                if (n > 0)
                {
                    *(s16 *)(p + 0x96) = 8 / n;
                }
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
   AND PASSED ON. func_b10_801727B8's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A66C 2 words, func_b10_801727B8 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b10_80171FE8(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b10_801727B8(p, b, c, d) == 0)
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

/* csrc/ovl_dng_b05.c's func_b05_80171D6C facing-table installer on the
   bank-local D_801759A0, with a *(s32 *)(p + 0x90) = 0 / b->unk_14 = 0 /
   p[0x9D] = 0 tail. The table base is a pointer local because it is used
   twice -- stored at c+0x2C and indexed. 34 words, 0 differing, first
   compile. */
void func_b10_80172730(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_801759A0;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(s32 *)(p + 0x90) = 0;
    b->unk_14 = 0;
    p[0x9D] = 0;
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80175988 facing table and re-aim through func_8009C93C.

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
s32 func_b10_801727B8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80175988;
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
   tests, put the actor into state 0x11, swap the D_80175988 facing table
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
   D_80175988 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b10_8017297C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80175988;
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
s32 func_b10_80172A94(u8 *a, void *b, void *c, s32 d)
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
        func_b10_80171FE8(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b10_80171FE8(p, b, c, p);
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
   installed handler is the bank-local func_b10_801717F4. 31 words,
   0 differing, first compile. */
void func_b10_80172BD8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b10_801717F4;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's fall-and-land state machine, the relinked twin of
   the resident overlay's func_800D991C in csrc/ovl_dungeon_800D50FC.c: the
   same p[0x9B] fallthrough switch, the same `(num << 16) / step'
   interpolation into b->unk_0C/unk_10, and the same
   func_800A2B04 / func_800AD594 / func_800A4ACC / func_800AD9B4 tail.
   242 words, 0 differing, FIRST COMPILE.

   THE RELINK, written out because it is the whole delta: func_8003DB94
   against the resident D_800E262C becomes func_80047784 against the
   bank-local D_801759A8 (case 0), D_801759B8 (case 2) and D_80175988
   (case 3) tables; the interpolation's step counter moves from p + 0x96 to
   p + 0x9E, leaving p + 0x96 as the outer countdown the shared tail
   decrements; case 1's drift is `-func_800644B8(step * 409) << 10' in place
   of the twin's b->unk_14 accumulator; case 0 gains the c + 0x14 0x6000
   gate; case 3 is new; and the tail gains the d + 0x46 0x8000 arm as the
   `(v & 0x2000) != 0' side of what is three nested ifs in the twin.

   Two of func_800D991C's measured spelling facts are reproduced here
   verbatim and were NOT re-measured, because the first compile landed: the
   first numerator is THREE statements (`u = c[0x24] << 6;
   w = b + 0x2 - 0x20; u -= w;') while the second stays inline, and the tail
   guard is nested ifs rather than an `&&' chain.  func_800D991C's own header
   carries what the alternatives cost there. */
void func_b10_80172C54(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801759A8;
        func_80047784(c, D_801759A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            *(s32 *)(p + 0xA4) = -func_800644B8(*(s16 *)(p + 0x9E) * 409) << 10;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA4);
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
            *(u8 **)(c + 0x2C) = D_801759B8;
            func_80047784(c, D_801759B8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
        break;
    case 3:
        if (*(u8 **)(c + 0x2C) != D_80175988)
        {
            *(u8 **)(c + 0x2C) = D_80175988;
            func_80047784(c, D_80175988[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
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
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
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
void func_b10_8017301C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b10", func_b10_80173080);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b10", func_b10_80173280);

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
   (5) func_800AAA54's fourth argument is the bank-local D_801759C8, and the
   tail carries an extra arm the resident has not got: if c+0x2C still holds
   D_80175998 it is swapped for D_80175988 and re-sent through
   func_80047784, the same pointer-local shape as func_b23_80172664 above.
   The installed handler is the bank-local func_b10_801717F4.

   198 words, 0 differing. */
void func_b10_8017380C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800AAA54(p, b, c, D_801759C8);
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
        if (*(u8 **)(c + 0x2C) == D_80175998)
        {
            t = D_80175988;
            *(u8 **)(c + 0x2C) = t;
            func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b10_80173B24(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b10_80173B44(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b10_80173B64(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b10_80173B84(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}

/* The banked room script's three-state tail, switching on the state index at
   p + 0x9B: the relinked twin of the resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c), which is this body statement for statement
   with four local changes -- two separate bank-local facing tables
   (D_801759E8 in state 0, D_801759F0 in state 1's re-dress) where the
   resident uses D_800E262C for both, func_80047784 for func_8003DB94,
   func_b10_80174258/func_b10_801717F4 for func_800DA660/func_800D8C64, and
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
void func_b10_80173BBC(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801759E8;
        func_80047784(c, D_801759E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
                func_b10_80174258(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801759F0;
        func_80047784(c, D_801759F0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
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
        *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_801759E8 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b10_801717F4. The resident
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
void func_b10_80173F4C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801759E8;
        func_80047784(c, D_801759E8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_801759F0;
            func_80047784(c, D_801759F0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b10_80174258(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801759F0;
        func_80047784(c, D_801759F0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
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
   call emits the bare `jal` either way); hoisting D_80175980 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b10_80174258(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_801759E8 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80175980;
        func_80047784(c, D_80175980[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b10_80174320(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b10_801717F4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b10_80174384(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}

/* 14 words, 0 differing. */
void func_b10_801743BC(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}

/* 14 words, 0 differing. */
void func_b10_801743F4(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}

/* Walk the actor's own circular room list (the h + 0x5C chain, payloads at
   node + 0x20) and, for every other actor standing on cell (x, y) within
   0x40 of height z, either knock it over -- clear its +0x28, lay -1 in its
   +0x64, func_800419EC(6, 0xC), the 0x80B/0x601 cue and a func_80099C58
   flash whose x/y come from the eight {dx, dy} pairs at D_b10_80170854
   indexed by (g + 0x2A >> 7) & 0x1C -- or, when its +0x13 is outside
   0x33..0x36, re-aim it through func_8009C12C. 145 words, 0 differing.
   2 members (dngfam.py: this and func_b10_8017442C). No twin: dngtwin.py's
   only offer is that clone, which is still a stub.

   THIS WAS PARKED AT 19 ALIGNED DIFFERENCES (docs/dng-parks/, agent-h11).
   Every spelling the park's table credits was right; the one row it could
   not reach was a missing register-to-register copy, and the answer is that
   THE PARAMETER `p' IS REUSED AS THE LOOP CURSOR.

   `h = p' then `g = h' alone is two pseudos and one copy, because `p' dies
   at `h = p' and regmove folds it into the parameter's own
   `addu <p>, $a0, $zero'. Letting `p' carry the cursor keeps it live, so
   the fold cannot happen and the chain is the target's three pseudos:
   p ($s0, shared with the cursor) -> h ($s2, the pre-loop loads and both
   loop compares) -> g ($s1, the four spawn-site reads). Measured on the
   smaller func_b09_80174800 below, which has the identical chain: a
   separate `e' cursor is 73 words and 15 aligned differences, this form 74
   and 0.

   MEASURED AND FREE, so NOT the reason this lands: `g = p' in place of
   `g = h' (145 words, 0 -- cse makes `h' the canonical register of the
   quantity either way, because it outlives `p'). The park's own table
   carries the rest: the height tests must put m + 0xA on the LEFT, the
   func_80099C58 index and its fifth argument share one sign extension, and
   the 0x80B/0x601 cue is a ternary. */
void func_b10_8017442C(u8 *p, u16 x, u16 y, s16 z)
{
    UnkDngB09A28Tbl t;
    u8 *q;
    u8 *m;
    u8 *v;
    u8 *w;
    u8 *h;
    u8 *g;

    t = *(UnkDngB09A28Tbl *)D_b10_80170854;
    h = p;
    g = h;
    v = *(u8 **)(h - 0x14);
    w = *(u8 **)(h - 0x18);
    p = *(u8 **)(h + 0x5C) + 0x20;
    while (p != h)
    {
        q = *(u8 **)(p - 0x14);
        m = *(u8 **)(p - 0x18);
        if (q[0x24] == x && q[0x25] == y && *(s16 *)(m + 0xA) <= z + 0x40 &&
            *(s16 *)(m + 0xA) > z - 0x40)
        {
            if ((u32)(p[0x13] - 0x33) < 4)
            {
                p[0x28] = 0;
                *(s16 *)(p + 0x64) = -1;
                func_800419EC(6, 0xC);
                func_800A56E0(p[0x13] == 0x34 ? 0x80B : 0x601);
                func_80099C58((s16)(*(u16 *)(w + 0x2) +
                                    ((UnkDngB09A28 *)((u8 *)&t +
                                                      (((s16) * (u16 *)(g + 0x2A) >> 7) & 0x1C)))
                                            ->unk_00 *
                                        0x20),
                              (s16)(*(u16 *)(w + 0x6) +
                                    ((UnkDngB09A28 *)((u8 *)&t +
                                                      (((s16) * (u16 *)(g + 0x2A) >> 7) & 0x1C)))
                                            ->unk_02 *
                                        0x20),
                              (s16)(*(u16 *)(w + 0xA) - 0x20), *(s16 *)(g + 0x14),
                              (s16) * (u16 *)(g + 0x2A));
            }
            else
            {
                func_8009C12C(g, v, *(s16 *)(g + 0x2A), 1);
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* The same room-list walk as func_b09_801745BC above and the smaller of the
   pair: for every other actor on cell (x, y) within 0x38 of height z whose
   +0x13 is OUTSIDE 0x33..0x36, set g+0x84 to 0x7C, g+0x85 to g[0x26] * 3 / 2
   and re-aim it through func_8009C93C. 74 words, 0 differing.
   2 members (dngfam.py: this and func_b10_80174670).

   THIS IS WHERE THE func_b09_801745BC PARK'S MISSING COPY WAS FOUND, because
   it is the same p -> h -> g pseudo chain in a quarter of the words. THE
   PARAMETER `p' CARRIES THE LOOP CURSOR. With a separate `u8 *e' cursor and
   the same aliases the body is 73 words and 15 aligned differences: `p' dies
   at `h = p', regmove folds that copy into the parameter's own
   `addu <p>, $a0, $zero', and the eight callee-saved registers all land one
   place over. Keeping `p' live as the cursor is 74 words and 0.

   MEASURED AND FREE, all 74 words / 0 aligned differences, so NONE of them
   is the reason this lands:
     * `g = p' in place of `g = h';
     * `g[0x26] * 3 / 2' in place of `(g[0x26] * 3) >> 1' -- gcc proves the
       `lbu' * 3 non-negative and drops the rounding correction either way;
     * the +0x13 gate written `if ((u32)(p[0x13] - 0x33) < 4) { } else { }'
       with the work in the `else' instead of `>= 4' with one arm.

   The height tests put m + 0xA on the LEFT, which is the func_b09_801745BC
   park's measured fact and was not re-measured here. `--raw' is 1 differing,
   the one `jal'. */
void func_b10_80174670(u8 *p, u16 x, u16 y, s16 z)
{
    u8 *q;
    u8 *m;
    u8 *v;
    u8 *h;
    u8 *g;

    h = p;
    g = h;
    v = *(u8 **)(h - 0x14);
    p = *(u8 **)(h + 0x5C) + 0x20;
    while (p != h)
    {
        q = *(u8 **)(p - 0x14);
        m = *(u8 **)(p - 0x18);
        if (q[0x24] == x && q[0x25] == y && *(s16 *)(m + 0xA) <= z + 0x38 &&
            *(s16 *)(m + 0xA) > z - 0x38)
        {
            if ((u32)(p[0x13] - 0x33) >= 4)
            {
                g[0x84] = 0x7C;
                g[0x85] = (g[0x26] * 3) >> 1;
                func_8009C93C(g, v, *(s16 *)(g + 0x2A), 1, NULL);
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* State 0x17, built out of the banked room scripts' shared idioms rather
   than from one twin: the D_80083462 gate, then func_800A2B5C either
   side of func_800C7930(d - 0x20, b, 8, 0x300), then the pointer-local
   D_801759D8 facing table published at c+0x2C and one func_80047784.

   THE ONE SPELLING FACT IS STATEMENT ORDER: `*(s32 *)(p + 0x8C) = 0' sits
   BETWEEN the p+0x9A and p+0x9B byte stores. Written after both of them --
   the order csrc/ovl_dng_b08.c uses -- the 0x8C word store and the 0x9B byte
   store come out transposed: 64 words, 2 differing.

   64 words, 0 differing. */
void func_b10_80174798(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

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
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    t = D_801759D8;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    d[0x6D] -= 1;
}

/* The p[0x9B] state machine that drives func_b10_80174670 and
   func_b10_8017442C: the eight {dx, dy} pairs at D_b10_80170854 are copied
   into a local before the dispatch, and cases 0 and 2 share the p[0x9B]++
   tail that the cross-jump merges. The local's alignment of 2 is what makes
   that copy the four lwl/lwr + swl/swr pairs rather than word moves.

   TWO SPELLING FACTS, both worth words.

   THE TWO CALL ARGUMENTS GO THROUGH `s32' LOCALS and the callees' x/y
   parameters are `u16'. Written straight into the call, fold narrows the
   whole argument to 16 bits and distributes the conversion into the
   operands, which drops the +0x0 field's `lh' to an `lhu' and deletes the
   `sll'/`sra' sign extension of the +0x2 field at both sites: 206 words,
   29 aligned differences. `s16' locals for the sums are 206 words, 33. An
   `s32' local for the +0x2 field alone keeps the extension but not the `lh':
   210 words, 26. Forty combinations of field type, operand cast and
   parameter type were swept with the sums inline first, and every one
   measured 202-207 words and 31-35 aligned differences.

   NO LOCAL FOR THE PAIR POINTER: the `(UnkDngB09A28 *)((u8 *)&t + idx)' cast
   is written out at all four uses. Hoisting it into an `e' local leaves the
   second site's `addiu $v0, $sp, 0x10' and `lhu $v1, 0x2A($a0)' with their
   two registers swapped -- 211 words, 5 aligned differences -- and no
   declaration order, no operand order in the addition, no index local and no
   `u8 *' spelling of `e' moved it off 5.

   211 words, 0 differing. `--raw' is 21 differing and every one is a
   relocation: the %hi/%lo pairs for D_b10_80170854, D_80175988,
   D_80083228, D_8008346C and func_b10_801717F4, six `jal's and four
   intra-function `j's.
   THIS COPY'S OWN CONSTANT: 0x12 -> 0x11, 0x605 -> 0x606.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b10_80174898(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkDngB09A28Tbl t;
    s32 x;
    s32 y;

    t = *(UnkDngB09A28Tbl *)D_b10_80170854;
    switch (p[0x9B])
    {
    case 0:
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s16 *)(p + 0x96) = 0x14;
        p[0x9B]++;
        break;
    case 1:
        if ((s16)(*(u16 *)(p + 0x96) -= 1) == 0x11 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            x = c[0x24] + ((UnkDngB09A28 *)((u8 *)&t + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_00;
            y = c[0x25] + ((UnkDngB09A28 *)((u8 *)&t + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_02;
            func_b10_80174670(d, x, y, *(s16 *)((u8 *)b + 0xA));
        }
        if (*(s16 *)(p + 0x96) == 0xA || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x606);
        }
        if (*(s16 *)(p + 0x96) == 0x9 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            x = c[0x24] + ((UnkDngB09A28 *)((u8 *)&t + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_00;
            y = c[0x25] + ((UnkDngB09A28 *)((u8 *)&t + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_02;
            func_b10_8017442C(d, x, y, *(s16 *)((u8 *)b + 0xA));
        }
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        *(s16 *)(p + 0x96) = 0;
        p[0x9B]++;
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) == D_80175988)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_80175988;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        break;
    case 3:
        func_800AD594(d, 0x400);
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        break;
    }
}

/* The node walk, the relinked twin of the resident overlay's func_800ADBCC
   (csrc/ovl_dungeon_800A4B88.c): walk the second list off p - 8, park a
   halfword in the scratchpad each turn and call func_800453E0 for every slot
   whose +0x14 does not carry 0x80. 35 words, 0 differing, first compile.

   THE RELINK IS THE SCRATCHPAD VALUE. func_800ADBCC parks the record's own
   `*(s16 *)(p + 0x8)'; this copy parks the bank-local s16 D_80175A18, whose
   `lui' is hoisted out of the loop while the `lh' stays inside it -- that is
   the whole of the three-word difference from the twin.

   Both of the twin's load-bearing spellings were kept and still are: the
   scratchpad goes through a POINTER LOCAL (as an absolute `*(s32 *)0x1F80013C'
   it is the assembler's `lui $at'/`sw' macro, which cannot leave the loop),
   and the slot stays a `u8 *' rather than a struct pointer so its +0x14 read
   is not MEM_IN_STRUCT_P and sched1 leaves the load-delay `nop's alone. */
s32 func_b10_80174BE4(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = D_80175A18;
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

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_b10_8017442C, func_b10_80174898. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b10_80170854[] = {
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

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b10_80174C70, func_b10_80174F24. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b10_80170874[] = {
    0x10,
    0x00,
    0x00,
    0x00,
    0x0D,
    0x00,
    0x0D,
    0x00,
    0x00,
    0x00,
    0x10,
    0x00,
    0xF3,
    0xFF,
    0x0D,
    0x00,
    0xF0,
    0xFF,
    0x00,
    0x00,
    0xF3,
    0xFF,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0xF0,
    0xFF,
    0x0D,
    0x00,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b10", func_b10_80174C70);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b10", func_b10_80174F24);

/* State 0x18's entry: clear the node's +0x71 top bit and +0x6D, install
   ITSELF in the slot's +0x8C handler slot and mark state 0x18/0; then, unless
   D_80083460's +0x2 0x2000 bit is up, if func_800A2BDC says the engine is
   idle, drop the handler again, re-mark 0x18/0, reset D_800DCF5E and bump
   D_80083460's +0xA counter. 43 words, 0 differing, first compile.

   This is func_b20_80174B3C (csrc/ovl_dng_b20.c) at ratio 1.000, same frame
   (-0x20, $s0/$ra/$s2/$s1 saved in that order), with the ONE delta being the
   state constant 0x17 -> 0x18. Both of its stated facts held:
   the +0x8C store is the function's OWN address, written as the plain
   identifier (`lui'/`addiu' of %hi/%lo(func_b10_801751C0)), and D_80083460
   goes through the unit's `g = (u8 *)&D_80083460' pointer-local idiom so one
   `addiu' base in $s1 serves both the +0x2 gate and the +0xA bump.

   BOTH SPELLINGS PRICED HERE, since a first compile proves nothing about
   which choice mattered:
     * the `g' POINTER LOCAL is load-bearing -- the direct `D_80083462'
       symbol for the gate with `&D_80083460 + 0xA' for the bump is 41 words
       (two SHORT) and 38 aligned differences;
     * `(s16)func_800A2BDC(d) == 0' needs the cast: without it the `sll 16'
       goes and the body is 42 words / 18 aligned differences;
     * the first group's `p[0x9A] = 0x18' BEFORE `p[0x9B] = 0' is
       load-bearing but cheap -- the other order is 43 words / 2 aligned
       differences, both rows the two `sb's swapped.
   1 member (dngfam.py): the four 1.000 rows dngtwin prints for this stub are
   LANDED bodies in other banks, not family members, because their state
   constant is 0x17 and this one's is 0x18. */
void func_b10_801751C0(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    *(void **)(p + 0x8C) = func_b10_801751C0;
    p[0x9A] = 0x18;
    p[0x9B] = 0;
    d[0x6D] = 0;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        if ((s16)func_800A2BDC(d) == 0)
        {
            *(s32 *)(p + 0x8C) = 0;
            p[0x9A] = 0x18;
            p[0x9B] = 0;
            D_800DCF5E = 0;
            *(u16 *)(g + 0xA) += 1;
        }
    }
}

/* The two-phase white-out. With the slot's +0x9A still zero, park the view's
   +0x8 in the bank-local D_80175A84, re-send the view through func_800478B8
   and -- only if its +0x14 carries one of 0xE000 -- unlink the node, set the
   0x10-frame countdown in +0x96 and step +0x9A. Afterwards, ease
   D_80083160's three +0xA8..+0xAA colour bytes towards 0x80 by
   (0x80 - v) / +0x96 and, when the countdown runs out, raise 0x8000 in the
   node's -0x2 and in D_800814A0. 75 words, 0 differing, first compile.

   THE EASE IS func_800ACE34's (csrc/ovl_dungeon_800A4B88.c), with the
   divisor counting DOWN in the slot's +0x96 instead of up in the view's
   +0xF: the `lh' of +0x96 is re-read for each of the three bytes for the
   same reason its note gives -- each `sb' through the `u8 *' wipes cse's
   memory table.

   D_80083160 IS REACHED THROUGH A `u8 *' BASE TAKEN BEFORE THE GATE. Its
   `addiu' sits in the `bnez's delay slot although the only use is in the
   else arm, which is the materialise-up-front shape unk_108F0.c records for
   this object; the bytes at +0xA8 also have their own splat label
   (D_80083208) and are NOT spelled that way here.

   THE COUNTDOWN TEST READS THE DECREMENTED VALUE, not a reload: `lhu',
   `addiu -1', `sh', `sll 16', `bgtz' with no `sra' and no second load, which
   is `(s16)(--*(u16 *)(p + 0x96)) <= 0'. */
void func_b10_8017526C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (p[0x9A] == 0)
    {
        D_80175A84 = *(s32 *)(c + 0x8);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9A] += 1;
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

/* The bank's nine-state white-out cutscene, run off the slot's +0x9B: step
   the counter, hand func_80041588 the D_801759F8 command stream and its
   D_80175A80 handshake byte, turn the actor to face 2, swap the HUD pair
   through func_8003F540/func_8003E4FC, open with func_800C77D0, ease
   D_80083160's three colour bytes while the D_80175A81 handshake is pending,
   spawn the 0x12 node whose driver is func_b10_8017526C, poll
   func_b10_80174F24 eight times a frame for the first 0x1E frames, wait for
   the node's +0x14 to carry one of 0xE000, then func_800ADC4C the
   replacement in, re-aim it and finally raise 0x8000 in the node's -0x2 and
   in D_800814A0 and drop D_80083460's +0xA counter. 350 words, 0 differing,
   FIRST COMPILE. 1 member (dngfam.py).

   THE TWIN IS THE LANDED func_b20_80174D14 (csrc/ovl_dng_b20.c, 330 words),
   PICKED ON THE FRAME: four twins sit between 0.935 and 0.944, all at
   `-0x28' or `-0x30', and only b20 has this listing's save order
   ($s1 0x14, $s3 0x1C, $s4 0x20, $s2 0x18, $ra 0x24, $s0 0x10) AND its
   $s1/$s3/$s4/$s2 = $a0/$a1/$a2/$a3 binding. b08, b14 and b04 all bind
   $s4 = $a1 and $s3 = $a2 instead. Both of b20's allocation facts held:

     * STATE 3'S D_80175A81 FLAG MUST BE READ INTO A LOCAL BEFORE THE +0x96
       RESTORE STORE. Written the other way round the body is 351 words and
       251 aligned differences -- b20 priced the same revert at 18 on its own
       body, and the difference is this one: the block moves, and on a
       table-owning body every table `j' moves with it. A three-digit residue
       here is not structural.
     * STATE 6'S `d - 0x20' MUST BE THE SAME LOCAL `r' AS STATES 4/5/7 USE.
       Given its own local it is 350 words / 3 aligned differences -- b20's
       figure exactly -- because local-alloc then hands it a call-clobbered
       $a0 instead of global-alloc's $s0.

   TWO NEW FACTS, both on pseudos b20 DOES NOT HAVE, since state 5's frame
   counter and its polling loop are this copy's own delta:

     * STATE 5'S `t3' MUST BE `s16'. As `s32' the body is 348 words / 127
       aligned differences. The declared width is what makes `flow' record
       the `sll 16'/`sra 16' pair and the live length that goes with it.
     * THE LOOP COUNTER `i' MUST BE `s16' TOO: as `s32' it is 347 words / 121
       aligned differences. The loop's SHAPE is free -- `for (i = 0; i < 8;
       i++)' and the `do/while' written here both measure 0 differing -- so
       only the width is a fact.
     * AND STATE 4'S `D_80175A18 = -2' MUST SIT BEFORE THE THREE `r' STORES.
       Moved after them it is 350 words / 21 aligned differences; sched1
       interleaves that `sh' into the `n->unk_08' copies and will not lift it
       back over the stores through `r'.

   MEASURED AND FREE, so NOT reasons this lands: state 5's gate written as
   the post-increment `if ((s16)((*(u16 *)(p + 0x96))++) < 0x1E)' instead of
   the `t3' pair (0 differing), case 1's `p + 0xAC' store on either side of
   the +0x9B bump (0 differing), and state 3's `t' as `s32' rather than `s16'
   (0 differing).

   THE RELINK AND SHAPE DELTA FROM b20, read off the listing:
     * D_8017530C -> D_801759F8, D_801753A8 -> D_80175A80, D_801753A9 ->
       D_80175A81, D_801753AC -> D_80175A84, D_80175314 -> D_80175A18,
       D_80175318 -> D_80175A00;
     * the two slot fields SWAP ROLES: the s16 b20 zeroes at `p + 0xA4' is at
       `p + 0xAC' here, and the node pointer b20 keeps at `p + 0xAC' is at
       `p + 0xA8';
     * the node's +0x10 driver is func_b10_8017526C (landed above in this
       unit), and func_8004491C's second argument is the FUNCTION
       func_b10_80174BE4 where b20 passes the data symbol D_80045340;
     * b20's `func_b20_80174724(p, b, c)' call at the end of state 2 is
       ABSENT;
     * state 4 GAINS `D_80175A18 = -2';
     * state 5 GAINS the whole +0x96 frame gate and the eight-call
       func_b10_80174F24 loop in front of b20's node poll. Those three
       together are the +20 words.

   THIS FUNCTION OWNS jtbl_b10_80170908, the 9-entry table at rodata offset
   0x100, indexed by p[0x9B] for states 0..8. Every state has a label of its
   own, no two share an arm and there is no `default': the `sltiu $v0, 0x9'
   gate sends everything else to the epilogue. States 1, 2, 5 and 7 fall
   through to their successors.

   func_80069EF8 is spelled `rand' (splat/common_syms.txt) and needs no
   declaration. Every `%hi'/`%lo' symbol was read back off the listing: the
   six bank-local symbols above are six DISTINCT names with distinct widths
   (D_80175A80/D_80175A81 bytes, D_80175A18 an s16, D_80175A84 an s32,
   D_80175A00 an s32 table, D_801759F8 an Unk41284 stream). */
void func_b10_80175398(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_80041588(D_801759F8, (s8 *)D_80175A80, 0);
        *(s16 *)(p + 0xAC) = 0;
        p[0x9B] += 1;
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x2A);
        /* fallthrough */
    case 2:
        t2 = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (D_80175A80[0] == 0 || t2 != 2)
        {
            if (t2 != 2)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            return;
        }
        func_80041588(D_801759F8, (s8 *)D_80175A80, 1);
        func_8003F540(0, D_8006CD58, 0x4000AD4, 0x5000CC4);
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        D_80175A81[0] = 0;
        func_8003E4FC(0xFF, D_8003E140, (s32)D_80175A81);
        func_800C77D0(d - 0x20, (u8 *)b, 8, 0x300);
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        t = *(u16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            f = D_80175A81[0];
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
        n->unk_10 = (u32)func_b10_8017526C;
        func_8004491C(n, (u32)func_b10_80174BE4);
        *(UnkNode **)(p + 0xA8) = n;
        n->unk_08->unk_00 = b->unk_00;
        n->unk_08->unk_04 = b->unk_04;
        n->unk_08->unk_08 = b->unk_08;
        D_80175A18 = -2;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        k = func_800498A0(d) - 1;
        if (k < 0)
        {
            k = rand() % 3;
        }
        func_8003DB94(r, D_8014A000 + D_80175A00[k], 0);
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        return;
    case 5:
        t3 = *(u16 *)(p + 0x96);
        *(s16 *)(p + 0x96) = t3 + 1;
        if (t3 < 0x1E)
        {
            i = 0;
            do
            {
                func_b10_80174F24(p, b, c, d);
                i++;
            } while (i < 8);
        }
        n = *(UnkNode **)(p + 0xA8);
        r = (u8 *)n->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0xE000) != 0)
        {
            p[0x9B] += 1;
        }
        /* fallthrough */
    case 6:
        if (p[0x9B] == 6 &&
            func_800ADC4C((u8 *)b, D_80175A84, D_80175A18, &D_800DCF5C) != NULL)
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
void func_b10_80175910(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}

/* 14 words, 0 differing. */
void func_b10_80175948(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b10_801717F4;
    }
}
