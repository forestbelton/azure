#include "common.h"

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b42_801725C8(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
s32 func_b42_8017263C(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_b42_8017609C[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_b42_80176094[];
extern u8 D_801760EC[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b42_80171E80(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b42_801714B8();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

extern u8 D_801760F4[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b42_8017412C(u8 *p, u8 *b, u8 *c, u8 *d);

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
void func_b42_8017103C(u8 *p, UnkVec *b, u8 *c);

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

extern u8 D_801760DC[];

void func_800A2B04(UnkVec *, s32, s32);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
s32 func_800A56E0(s32);
extern s32 D_8008346C;
extern u8 D_801760CC[];

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
typedef struct UnkDngB20Name12
{
    u8 b[0xC];
} UnkDngB20Name12;

void func_800478B8(u8 *);
extern s8 D_800DCECC[];

/* The thrown slot's project-and-settle step: push the mover at b and the
   slot's own vector at p+0xA8 through RotTransPers, set c+0x6 from the
   DIFFERENCE of the two depths less twice the D_800DCECC facing byte, send
   the view through func_800478B8, and raise 0x8000 in the node's -0x2 and
   in D_800814A0 when the +0x96 countdown runs out. 78 words, 0 differing,
   first compile. 2 members (dngfam.py: this and func_b42_801708D4).

   IT IS THE ~45-WORD TAIL OF THE LANDED func_b23_80170874
   (csrc/ovl_dng_b23.c, 151 words) with that body's velocity decay, its
   c+0x1C/+0x1E wind and its `m < 0xA' colour ease all absent, and with
   three parameters instead of four. Two of b23's facts were re-measured
   here and one of them does NOT hold:

     * THE FRAME SHAPE IS LOAD-BEARING. RotTransPers's second out-parameter
       is EIGHT bytes, `s32 sxy[2]', which is what puts the third and fourth
       at sp+0x20 and sp+0x24. A plain `s32 sxy' with `&sxy' at both calls
       is 78 words, 2 aligned differences.
     * THE D_800DCECC BYTE IS `* 2', NOT `<< 1' -- 79 words, 3 aligned
       differences written as a shift.
     * MEASURED AND FREE, so NOT the reason this lands: the countdown test.
       b23 records `(s16)(--*(u16 *)(p + 0x96)) <= 0' as a fact; here
       `*(s16 *)(p + 0x96) -= 1;' followed by a separate reload-and-test is
       ALSO 78 words / 0 differing, so on this body it is not a lever.

   `--raw' is 10 differing and all 10 are relocations -- 3 `jal's and 7
   `%hi'/`%lo' halves. */
extern u8 D_801760B4[];
extern u8 D_8006CCF8[];
extern void (*D_80176138[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_801760D4[];
extern u8 D_801760BC[];
extern u8 D_801760C4[];

s16 func_800A41F0(u8 *);
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
s16 func_800A45D8(u16, u16, s32);
extern u8 D_800DEC70[];
extern u8 D_800DECF8[];
void func_b42_80174300(u8 *p, UnkVec *b, u8 *c);
void func_b42_80171E80(u8 *p, void *b, void *c, u8 *d);
typedef struct UnkDngB42Cell
{
    u8 unk_00;
    u8 pad_01;
    u8 unk_02;
    u8 pad_03;
} UnkDngB42Cell;
typedef struct UnkDngB42Word48
{
    u32 w[0xC];
} UnkDngB42Word48;
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
extern u8 D_801760E4[];
extern u8 D_801760AC[];
extern u8 D_801760A4[];
extern u8 D_800DEA68[];
void func_b42_80171C3C(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b42_80172800(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b42_80172918(u8 *a, void *b, void *c, s32 d);
void func_b42_801756E0(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
s32 func_8003F270(void);
u8 *func_8003DE58(u8 *, u8 *, void *, s16);
extern u8 D_801760FC[];
extern UnkDngB09A28 D_8017610C[];
extern u8 D_8017612C[];
s32 func_800C77D0(void *, u8 *, s16, s32);
void func_b42_80174AA4(u8 *p, UnkVec *b, u8 *c);

void func_b42_801708D4(u8 *p, UnkVec *b, u8 *c)
{
    SVECTOR sv;
    s32 sxy[2];
    s32 pz;
    s32 flag;
    UnkVec *q;
    s32 r;

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
    func_800478B8(c);
    if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The lift slot's project-and-settle step: push the mover at b and the
   record's own vector at p+0x28 through RotTransPers, set c+0x6 from the
   difference of the two depths PLUS twice the D_800DCECC facing byte, clear
   0x80 in c+0x14, send the view through func_800478B8, and raise 0x8000 in
   the node's -0x2 and in D_800814A0 when c+0x14 comes back with 0x6000 set.
   79 words, 0 differing.

   THE TWIN IS func_b42_801708D4 TWO LISTINGS UP IN THIS FILE (78 words,
   dngtwin 0.904) and the statements are its.  Four deltas: the second vector
   is at p+0x28 not p+0xA8 and the facing at p+0x1C not p+0x94; the
   D_800DCECC term is ADDED, not subtracted (`addu' where b42_801708D4 has
   `subu'); there is an extra `*(u16 *)(c + 0x14) &= 0xFF7F;'; and the tail
   gate is a fresh read of c+0x14 against 0x6000 rather than the +0x96
   countdown.

   THE 0xFF7F MASK GOES AFTER THE +0x6 STORE IN THE SOURCE, which is the one
   thing on this body that had to be measured.  The target's `lhu'/`andi'/`sh'
   on c+0x14 are interleaved into the D_800DCECC computation and its `sh'
   lands one word BEFORE the +0x6 store, but written ahead of the +0x6 store
   the mask is scheduled up past the second RotTransPers call instead:
   79 words, 21 aligned differences.  Written after it, sched moves it where
   the target has it: 0 differing.  Splitting the expression to put the mask
   between the call and the store (`r -= RotTransPers(...);' then the mask
   then `c+0x6 = r + ...') is 81 words / 52.

   b42_801708D4's FRAME-SHAPE fact holds here and is worth more than it is
   there: `s32 sxy[2]' is what puts the third and fourth out-parameters at
   sp+0x20 and sp+0x24.  A plain `s32 sxy' with `&sxy' at both calls is
   76 words / 16 aligned differences -- three words short.
   Its D_800DCECC fact holds too, with a bigger number than b42_801708D4
   measured: the byte is `* 2', and `<< 1' is 81 words / 13 aligned
   differences. */
void func_b42_80170A0C(u8 *p, UnkVec *b, u8 *c)
{
    SVECTOR sv;
    s32 sxy[2];
    s32 pz;
    s32 flag;
    UnkVec *q;
    s32 r;

    sv.vx = *(u16 *)((u8 *)b + 0x2);
    sv.vy = *(u16 *)((u8 *)b + 0x6);
    sv.vz = *(u16 *)((u8 *)b + 0xA);
    r = RotTransPers(&sv, sxy, &pz, &flag);
    q = *(UnkVec **)(p + 0x28);
    sv.vx = *(u16 *)((u8 *)q + 0x2);
    sv.vy = *(u16 *)((u8 *)q + 0x6);
    sv.vz = *(u16 *)((u8 *)q + 0xA);
    *(s16 *)(c + 0x6) = r - RotTransPers(&sv, sxy, &pz, &flag) +
                        D_800DCECC[((D_80083228 + *(s16 *)(p + 0x1C) + 0x100) >> 9) & 7] * 2;
    *(u16 *)(c + 0x14) &= 0xFF7F;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 particle's three-axis integrator, and the fuller of the two this
   unit carries: add the payload's +0x50/+0x54/+0x58 velocity words into the
   mover's +0x0/+0x4/+0x8, accelerate each by +0x5C/+0x60/+0x64, back the x
   and y steps out again when func_800A45D8 reports the new cell blocked,
   clamp +0xA to func_800BCB04's ground height (and on hitting it arm +0x14
   and clear +0x1E), re-send the view through func_800478B8 every fourth
   frame on the +0x20 counter, ease the slot's +0x1C and +0x1E scale up to
   0x1000 by 0x64 a frame, and raise 0x8000 in the node's -0x2 and in
   D_800814A0 when the +0x1E life runs out or the slot reports 0x8000.
   162 words, 0 differing, FIRST COMPILE.

   NO TWIN ANYWHERE.  dngtwin finds no carved sibling at ratio >= 0.60 and
   xsibs finds nothing for this unit at any ratio.  What made it a first
   compile is func_b42_80174300 below -- the SMALLER driver of the same
   family, landed first in this same pass.  Its head is this one's head, so
   the whole integrator transcribed and its one priced fact carried over.

   THE GROUND-HEIGHT LOCAL MUST BE `s32 h', NOT `s16 h': 163 words and
   5 aligned differences as `s16'.  A 16-bit local is a HImode pseudo, so its
   load is emitted `lhu' and cse then derives `h' from the `lhu' the
   func_800BCB04 argument already needs, paying a `sll'/`sra' to sign-extend
   it again.  The target has an independent `lh $s0, 0xA($s1)' beside the
   argument's `lhu $a2, 0xA($s1)'.  Re-measured here; func_b42_80174300
   prices the same fact at 120 / 4.

   `--raw' is 11 differing and every one is a relocation: the 5 `jal's and
   the two D_800814A0 gates' 3 words each. */
void func_b42_80170B48(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;

    b->unk_00 += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x50) += *(s32 *)(p + 0x5C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x50);
        *(s32 *)(p + 0x50) = 0;
        *(s32 *)(p + 0x5C) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x54);
    *(s32 *)(p + 0x54) += *(s32 *)(p + 0x60);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x54);
        *(s32 *)(p + 0x54) = 0;
        *(s32 *)(p + 0x60) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x58);
    *(s32 *)(p + 0x58) += *(s32 *)(p + 0x64);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x58) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        if (*(s16 *)(p + 0x14) == 0)
        {
            *(s16 *)(p + 0x14) = 1;
            *(s16 *)(p + 0x1E) = 0;
        }
    }
    *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) + 1;
    if ((*(u16 *)(p + 0x20) & 3) == 0)
    {
        func_800478B8(c);
    }
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x64;
    if (*(u16 *)(c + 0x1C) > 0x1000)
    {
        *(s16 *)(c + 0x1C) = 0x1000;
    }
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x64;
    if (*(u16 *)(c + 0x1E) > 0x1000)
    {
        *(s16 *)(c + 0x1E) = 0x1000;
    }
    *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) - 1;
    if (*(s16 *)(p + 0x1E) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Wire a newly spawned node to its owner: park the owner at the node's
   +0x60, re-send it through func_8004491C/D_80045340, clear the renderable
   (0x20 at the slot's +0x10, the 0xC bits in +0x14), seed the mover's three
   halfwords from the caller's record and then, through a RE-READ of
   n->unk_0C, set the 0x1000 scale pair and a 0x80 grey.  41 words,
   0 differing, first compile.  2 copies (dngfam.py).

   IT IS THE RESIDENT func_800D4BD4 (csrc/ovl_dungeon_800CABA8.c) WITHOUT ITS
   TAIL: the +0xC view copies, the 0x7DCF at +0x12, the 0x100 bit and the
   func_8003DB94 call are all absent here, +0x1E/+0x1C carry 0x1000 instead
   of the twin's 0x800, the owner lands at n+0x60 rather than at p+0x24, and
   the three 0x80 bytes at +0xE/+0xD/+0xC are func_800D4AB0's, not
   func_800D4BD4's.  The twin's one documented fact DOES hold: n->unk_0C is
   READ TWICE, because the halfword stores through the mover pointer wipe
   cse's memory table.

   THREE PARAMETERS, NOT THE TWIN'S FOUR.  The sole caller
   func_b20_80172BCC sets only $a0..$a2 -- `addu $a2, $s4, $zero' is the
   `jal's delay slot and nothing writes $a3 -- so the fourth parameter the
   resident's call sites witness has no witness here.

   `--raw' is 3 differing: one `jal', one `lui' and one `addiu', all
   relocations.
   THIS COPY'S OWN CONSTANT: 0x60 -> 0x48.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b42_80170DD0(UnkNode *n, u8 *b, u8 *c)
{
    u8 *r;
    UnkVec *m;

    *(u8 **)((u8 *)n + 0x48) = b;
    func_8004491C(n, (u32)D_80045340);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x14) |= 0xC;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(c + 0x2);
    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(c + 0x6);
    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(c + 0xA);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x1E) = 0x1000;
    *(s16 *)(r + 0x1C) = 0x1000;
    r[0xE] = 0x80;
    r[0xD] = 0x80;
    r[0xC] = 0x80;
}

/* The banked room script's SPAWNER for the state-9 actor: allocate a 0x112
   node off D_80083498, install func_b42_8017103C as the node's +0x10 driver,
   dress the slot from the bank-local D_b42_8017609C facing table, pick the
   +0x14/+0x1C flag pair from the low two bits of the caller's `a', and -- only
   for a==0 -- give one actor in two a random 0x20|rand palette out of
   func_800A48F0 and the OTHER bank-local table D_801760EC. Then hand the
   node to func_800A9C18 and arm the +0x8C continuation func_b42_801714B8.

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

   func_b42_8017103C and func_b42_801714B8 are address-taken here and are
   still INCLUDE_ASM stubs below, so both need a declaration above.
   THIS COPY'S OWN CONSTANT: 0x9 -> 0x2a.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_b42_80170E74(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b42_8017103C;
        p[0x13] = 42;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_b42_8017609C;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_801760EC;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b42_801714B8;
        func_800AA36C(w, m, r, p);
    }
    return p;
}

/* The banked room script's per-frame actor driver, the relinked twin of the
   resident overlay's func_800D8728: past the engine's 0x2000 gate it runs
   func_800A9E70, then the record's own +0x8C continuation, then the +0x9A
   entry of this bank's D_80176138 handler table, re-aims the tile when the
   +0x6D facing moved under it, re-skins the slot for the eight-way facing,
   steps the mover by its velocity, runs the +0x9D bob counter and leashes
   +0x92/+0x88 to the ground height func_800BCB04 reports.  287 words,
   0 differing, on the first compile.

   THE TWIN IS THE LANDED func_b34_801718E0 (csrc/ovl_dng_b34.c, 258 words,
   dngtwin 0.947).  Bank-local deltas: the +0x8C gate compares against
   func_b42_801714B8 and the handler table is D_80176138.

   THE 29 EXTRA WORDS ARE ONE ARM NO LANDED SIBLING HAS, where b34 calls
   func_800A020C unconditionally: a 0x10000000 gate on d+0x1C, and inside it a
   func_80042900 gate whose TRUE side is a four-way chain on `*(s8 *)(c + 0x4)'
   writing one byte to c+0xE, c+0xD and c+0xC -- 0 -> 0x80, 1 -> 0x64,
   2 -> 0x50, 3 -> 0x64, anything else nothing -- and whose FALSE side is
   b34's func_800A020C call.  The chain is written as four if/else-if arms with
   the stores DUPLICATED in each; jump2 cross-jumps the three store triples
   into the one block the target has, and cases 1 and 3 merge all the way back
   through their shared `li 0x64'.  Written as a `switch' with 1 and 3 sharing
   an arm it is 291 words / 196 aligned differences -- expand_case builds a
   balanced tree that compares the midpoint first, where the target compares
   0, 1, 2, 3 in order.

   ALL THREE OF b34's NAMED FACTS HOLD HERE, re-measured in this body:

   | spelling                                      | words | aligned diffs |
   | as written below                              |  287  |      0        |
   | one handler local instead of `h' and `h2'     |  287  |      3        |
   | `s32 k1'/`s32 k2' instead of `s16'            |  284  |    272        |
   | no `q' alias, `p' at every use                |  284  |    279        |
   | the siblings' duplicated three-way tail       |  296  |    139        |

   THIS FUNCTION'S FORWARD DECLARATION HAD THE WRONG ARITY and is changed in
   the block at the top of the unit: `void func_b42_8017103C(u8 *, void *,
   u8 *, u8 *)' is now three parameters, because the 0x2000 arm's `jalr'
   copies the INCOMING $a0 into $a3 -- the fourth argument is p, not a fourth
   parameter.  The only other use in the unit is the address-take in
   func_b42_80170DD0, which does not depend on the signature;
   func_b42_80170DD0 and func_b42_801708D4 were re-measured after the change
   and are still 41 and 78 words / 0 differing. */
void func_b42_8017103C(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b42_801714B8)
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
    D_80176138[p[0x9A]](p, b, c, p);
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
        if ((*(s32 *)(q + 0x1C) & 0x10000000) == 0)
        {
            if ((s16)func_80042900((s8 *)q, 1) != 0)
            {
                if (*(s8 *)(c + 0x4) == 0)
                {
                    c[0xE] = 0x80;
                    c[0xD] = 0x80;
                    c[0xC] = 0x80;
                }
                else if (*(s8 *)(c + 0x4) == 1)
                {
                    c[0xE] = 0x64;
                    c[0xD] = 0x64;
                    c[0xC] = 0x64;
                }
                else if (*(s8 *)(c + 0x4) == 2)
                {
                    c[0xE] = 0x50;
                    c[0xD] = 0x50;
                    c[0xC] = 0x50;
                }
                else if (*(s8 *)(c + 0x4) == 3)
                {
                    c[0xE] = 0x64;
                    c[0xD] = 0x64;
                    c[0xC] = 0x64;
                }
            }
            else
            {
                func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
            }
        }
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

/* The +0x8C handler at the head of this bank's actor machine: four early
   exits on D_80083462's flag word and d+0x1C's bits, the +0x9A state byte
   set to 0xE with a fresh random +0xA0 timeout, the facing re-dress from
   the bank-local byte tables, then either the inlined d+0x46 dispatch or
   the walk; and after all of that a two-table idle animation -- swap
   D_b42_8017609C in, count +0x9E up to +0xA0, and on expiry swap in
   D_801760A4 and spawn one 0x212 node driven by func_b42_80170A0C.
   481 words, 0 differing, FIRST COMPILE.

   TWIN: the landed func_b09_80171728 in csrc/ovl_dng_b09.c (363 words,
   dngtwin 0.758 -- its FOURTH row, and the first three were the wrong body).
   The right one was picked by ROLE, not by ratio: func_b42_801730E4 above
   installs THIS function at p+0x8C, and b09's twin of func_b42_801730E4
   (func_b09_80173230) installs func_b09_80171728 at p+0x8C, so the two are
   the same slot of the same machine.  Their jump tables are even at the same
   address, 0x80170808, with the same case map.  dngtwin's three higher rows
   (func_b43_80170F68 0.792, func_b15_801719DC 0.765, func_b31_801716F4
   0.764) were not opened.

   IT OWNS jtbl_b42_80170808: 12 words = 12 cases + 0 pad, dispatch bound 12,
   only SIX distinct targets (jtcheck).  MINVAL IS 1, so entry k is case k+1.
   Cases 1, 2, 3 -> the func_800AAF00 arm; 5, 6, 7 -> the func_800A0818 /
   D_800814A8 arm; 8 -> func_b42_8017263C / func_b42_80172800;
   9 -> func_b42_801756E0; 12 -> func_800A9A0C, written as a `case' label
   INSIDE the `w[0x9A] != 0x11' arm.  CASES 4, 10 AND 11 ARE NOT WRITTEN:
   their three entries hold the `default' arm's label, which is also the
   range check's `beqz' target, so they are gcc's pad to maxval and NOT arms.
   Twelve entries, nine real cases, six distinct labels.

   THE RELINK DELTA FROM b09, read off the listing:
     * b09's D_80174E4C / D_80174E44 / D_b09_80174DEC / D_80174DFC /
       D_80174E2C become D_801760EC / D_801760E4 / D_b42_8017609C /
       D_801760AC / D_801760DC, and func_b09_80171CD4 / func_b09_80171F40 /
       func_b09_80172710+func_b09_801728D4 / func_b09_801729EC /
       func_b09_80174928 become func_b42_80171C3C / func_b42_80171E80 /
       func_b42_8017263C+func_b42_80172800 / func_b42_80172918 /
       func_b42_801756E0;
     * the state-0xE arm's +0xA0 is `(rand() & 0x1F) + 0xF' where b09 stores
       the constant 0x14;
     * the 0x80000 arm is b18's SHORT shape -- func_800AA888 then
       func_b42_8017412C and return, with no dress and no `p + 0x90' clear,
       so b09's 13-word delta and its 23-row price do not apply here;
     * THE 118 EXTRA WORDS ARE ALL TAIL, and they are STATEMENTS b09 has not
       got, which is what dngtwin's shorter/longer flag predicts: one more
       early return on `d+0x1C & 0x20', then the D_b42_8017609C /
       D_801760A4 two-table idle with the +0x9E counter, the func_8003FC64
       spawn, the func_b42_80170DD0 wire-up, the func_8003DE58 projection
       added into the new node's mover and the func_8003DB94 dress from
       D_800DEA68.

   b09'S TWO SPELLING FACTS, BOTH RE-PRICED HERE AND BOTH HELD -- and both
   cost far more on this body than on b09's:

   | spelling                                              | words | aligned |
   | as written below                                       |  481  |    0    |
   | one shared `e' across all five dress sites             |  483  |   25    |
   | `w = (u8 *)D_800814A8' AFTER the `d + 0x2A' store       |  482  |   16    |

   ONE TABLE LOCAL PER DRESS SITE: b09 measured 4 aligned differences for the
   shared local and b05 measured 2; here it is 25 AND an eighth callee-saved
   register ($s5 appears), because this body has FIVE dress sites where b09
   has four and one of them is live across two calls.
   AND `w' MUST BE ASSIGNED BEFORE THE `d + 0x2A' STORE, so the `lw's load
   delay is filled by that `sh'.  b09 states this fact with no price because
   its body was 0 differing with it in place; 16 rows here is the first
   number anyone has put on it.

   MEASURED AND FREE, so NOT the reason this lands: the +0x9E counter as an
   explicit `t = *(u16 *)(p + 0x9E); *(s16 *)(p + 0x9E) = t + 1;
   if ((s16)t >= ...)' instead of the post-increment written below
   (481 / 0 either way).

   `--raw' is 110 differing and all 110 are relocations -- 76 %hi/%lo halves,
   24 `jal's and 10 `j's -- so no masked word hides a constant address.  The
   11 D_80083462 halves are 7 `lui's against 4 `lhu's, because reorg
   duplicates the `lui' into three branch delay slots that all reach one
   shared `lhu'. */
void func_b42_801714B8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR out;
    u16 x;
    s16 n;
    s32 v;
    s32 m;
    u8 *e;
    u8 *e2;
    u8 *e3;
    u8 *e4;
    u8 *e5;
    u8 *w;
    UnkNode *n2;
    UnkVec *m2;
    u8 *w2;
    u8 *r0;
    u8 *r2;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b42_80171C3C(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_801760EC)
        {
            e = D_801760E4;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_801760EC)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801760E4) != 0)
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
            e2 = D_b42_8017609C;
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
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_801760AC) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b42_8017412C(p, (u8 *)b, c, d);
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
            func_b42_80171E80(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b42_80172918(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b42_80171E80(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b42_8017263C(p, b, c, d) != 0)
            {
                return;
            }
            func_b42_80172800(p, b, c, d);
            return;
        case 9:
            func_b42_801756E0(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_801760DC, func_b42_801714B8);
            return;
        default:
            func_b42_80171E80(p, b, c, d);
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
    if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
    {
        return;
    }
    e3 = D_b42_8017609C;
    if (*(u8 **)(c + 0x2C) != e3 && *(u8 **)(c + 0x2C) != D_801760A4)
    {
        *(u8 **)(c + 0x2C) = e3;
        func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x9E) = 0;
        *(s16 *)(p + 0xA0) = (rand() & 0x1F) + 0xF;
    }
    if (*(u8 **)(c + 0x2C) == e3)
    {
        if ((s16)(*(u16 *)(p + 0x9E))++ >= *(s16 *)(p + 0xA0))
        {
            e4 = D_801760A4;
            *(u8 **)(c + 0x2C) = e4;
            func_80047784(c, e4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            func_800478B8(c);
            n2 = func_8003FC64(0x212);
            if (n2 != NULL)
            {
                func_b42_80170DD0(n2, p, (u8 *)b);
                n2->unk_10 = (u32)func_b42_80170A0C;
                w2 = (u8 *)n2 + 0x20;
                *(u8 **)(w2 + 0x28) = (u8 *)b;
                *(s16 *)(w2 + 0x1C) = *(u16 *)(d + 0x2A);
                r0 = *(u8 **)(p - 0x14);
                if (func_8003DE58(*(u8 **)(r0 + 0x8), r0, &out, 0xF) != NULL)
                {
                    m2 = n2->unk_08;
                    *(s16 *)((u8 *)m2 + 0x2) = *(u16 *)((u8 *)m2 + 0x2) + (u16)out.vx;
                    *(s16 *)((u8 *)m2 + 0x6) = *(u16 *)((u8 *)m2 + 0x6) + (u16)out.vy;
                    *(s16 *)((u8 *)m2 + 0xA) = *(u16 *)((u8 *)m2 + 0xA) + (u16)out.vz;
                }
                r2 = (u8 *)n2->unk_0C;
                *(u16 *)(r2 + 0x14) |= 0x80;
                func_8003DB94(r2, D_800DEA68, 0);
            }
        }
    }
    if (*(u8 **)(c + 0x2C) == D_801760A4 && (*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        e5 = D_b42_8017609C;
        *(u8 **)(c + 0x2C) = e5;
        func_80047784(c, e5[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x9E) = 0;
        *(s16 *)(p + 0xA0) = (rand() & 0x1F) + 0xF;
    }
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_b42_80176094 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b42_801725C8.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b42_801725C8 is called only on
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
void func_b42_80171C3C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b42_80176094)
        {
            *(u8 **)(c + 0x2C) = D_b42_80176094;
            func_80047784(c, D_b42_80176094[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b42_801725C8(p, b, c, d);
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
   AND PASSED ON. func_b42_8017263C's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A66C 2 words, func_b42_8017263C 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b42_80171E80(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b42_8017263C(p, b, c, d) == 0)
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

/* State 0x10: arm the slot's +0x1C/+0x1E wind at 0x1400/0xC00 and hand c the
   bank-local D_801760B4 byte the +0x2A facing selects, through func_80047784.
   29 words, 0 differing, on the first compile.

   THE TWIN IS THE LANDED func_b34_80172BD8 (csrc/ovl_dng_b34.c, 27 words,
   dngtwin 0.964): the same body with `*(s16 *)(p + 0x9E) = 2' where this one
   has the two wind stores, and D_80174EC8 for D_801760B4.

   ITS ONE NAMED FACT IS FREE HERE.  b34 records the table base as a POINTER
   LOCAL "because it is used twice"; spelled out as D_801760B4 at both uses --
   the c+0x2C store and the subscript -- this body is also 29 words /
   0 differing, so on this allocation cse shares the `%hi'/`%lo' by itself.
   The local is kept below only because it reads better.

   MEASURED AND NOT FREE: the wind stores are +0x1C then +0x1E.  The other way
   round is 29 words / 7 aligned differences. */
void func_b42_801725C8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    *(s16 *)(c + 0x1C) = 0x1400;
    *(s16 *)(c + 0x1E) = 0xC00;
    t = D_801760B4;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_b42_8017609C facing table and re-aim through func_8009C93C.

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
s32 func_b42_8017263C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_b42_8017609C;
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
   tests, put the actor into state 0x11, swap the D_b42_8017609C facing table
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
   D_b42_8017609C used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b42_80172800(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_b42_8017609C;
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
s32 func_b42_80172918(u8 *a, void *b, void *c, s32 d)
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
        func_b42_80171E80(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b42_80171E80(p, b, c, p);
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
   installed handler is the bank-local func_b42_801714B8. 31 words,
   0 differing, first compile. */
void func_b42_80172A5C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b42_801714B8;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's four-state lift, switching on p[0x9B]: state 0
   swaps in the D_801760BC facing table and arms the +0x9E counter, state 1
   drives the two-numerator divide and the func_800644B8 sub-step at +0xA4,
   state 2 settles onto the tile and swaps in D_801760C4, state 3 only parks
   the wind at 0x1000, and the shared tail re-centres, runs func_800AD594 /
   func_800A4ACC, decrements D_80083460+0x8 and installs func_b42_801714B8.
   242 words, 0 differing, on the first compile.

   THE TWIN IS THE LANDED func_b40_8017240C (csrc/ovl_dng_b40.c, 223 words,
   dngtwin 0.942), itself a copy of func_b36_801723D4.  Operand deltas: the
   facing tables are D_801760BC and D_801760C4; the counter is p + 0x9E and
   the accumulator p + 0xA4 (b40 has 0xA4 and 0xA0); the sub-step shifts by 9,
   b36's shift and not b40's 10; the installed handler is func_b42_801714B8.

   THE 19 EXTRA WORDS ARE FOUR PAIRS OF +0x1C/+0x1E WIND STORES THE TWIN HAS
   NOT GOT, and a fourth case that is nothing but one of them: state 1 opens
   with 0xC00/0x1400 after the +0x90 debit, state 2 writes 0x1400/0xC00
   between func_800A2B04 and the table swap, state 3 is `0x1000' into both,
   and the tail writes 0x1000 into both between func_800A2B04 and
   func_800AD594.  All four are read literally off the listing.

   ALL THREE OF b40's NAMED FACTS HOLD HERE, re-measured in this body:

   | spelling                                      | words | aligned diffs |
   | as written below                              |  242  |      0        |
   | the tail gate's two arms swapped              |  242  |     17        |
   | 0x410 and 0x20000 folded into one `&&'        |  242  |     10        |
   | the first numerator as one expression         |  242  |      1        | */
void func_b42_80172AD8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801760BC;
        func_80047784(c, D_801760BC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 5;
        *(s32 *)(p + 0xA4) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA4);
        *(s16 *)(c + 0x1C) = 0xC00;
        *(s16 *)(c + 0x1E) = 0x1400;
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
            *(s16 *)(c + 0x1C) = 0x1400;
            *(s16 *)(c + 0x1E) = 0xC00;
            *(u8 **)(c + 0x2C) = D_801760C4;
            func_80047784(c, D_801760C4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
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
void func_b42_80172EA0(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        }
    }
}

/* A four-state tail switched on the actor's +0x9B: state 0 either flags the
   actor done and hands off to func_8009C12C or zeroes the UnkVec's three
   high fields, states 1 and 2 count the +0x96 timer UP to 4 (or bail early
   on any of c + 0x14's 0xE000 bits) and then arm the D_801760CC facing
   table / re-fire func_8009C12C, and state 3 parks func_b42_801714B8 in
   +0x8C and clears +0x46's top bit. 120 words, 0 differing. Landed on the
   first compile. 2 copies (dngfam.py). No rodata of its own -- its listing
   has no `.section .rodata' block.

   IT LANDED BECAUSE func_b42_801714B8 WAS ALREADY IN THIS UNIT: its address
   is what state 3 stores into +0x8C, and the only declarations this body
   needed beyond what the unit already carried were func_8009C12C,
   func_800A56E0, D_8008346C and D_801760CC -- D_80083228, func_80047784,
   func_800AD594 and func_800A4ACC were all brought in by earlier bodies.

   Nearest relative is the landed func_b37_80172650 in csrc/ovl_dng_b37.c
   (124 words), which shares the +0x9B switch, the
   `*(u8 **)(c + 0x2C) = T; func_80047784(c, T[...], 0)' pair, the
   func_800A56E0(0x808) sting and the terminal +0x8C handoff. It is NOT the
   same body: this one has four numbered states instead of 0/1/0xFF, no
   leading `--*(s16 *)(p + 0x96)', a counter that counts up rather than down,
   state 0's three UnkVec stores, and a terminal arm with no func_800A2B04
   and an unconditional 0x7FFF clear in place of b37's +0x6D two-arm gate.

   The three `p[0x9B]++; *(s16 *)(p + 0x96) = 0;' copies are written out once
   per arm as below; jump.c cross-jumps them into the single shared tail at
   .Lb27_80173754 that the target has.

   `--raw' is 20 differing and all 20 are relocations -- 6 `jal's, 6 `j's and
   8 `%hi'/`%lo' halves (D_801760CC, D_80083228, func_b42_801714B8 and
   D_8008346C) -- so no masked word is hiding a constant address.
   THIS COPY'S OWN CONSTANT: 0xe000 -> 0x8000 at 2 of 3 sites, 0x808 -> 0x809, 0x100 -> 0x140 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b42_80172F04(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (p[0x9B])
    {
    case 0:
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
        p[0x9B]++;
        *(s16 *)(p + 0x96) = 0;
        return;
    case 1:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) == 4 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u8 **)(c + 0x2C) = D_801760CC;
            func_80047784(c, D_801760CC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            func_800A56E0(0x809);
            p[0x9B]++;
            *(s16 *)(p + 0x96) = 0;
        }
        return;
    case 2:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) == 4 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            p[0x9B]++;
            *(s16 *)(p + 0x96) = 0;
        }
        return;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_800AD594(d, 0x140);
        *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        D_8008346C = 0;
        func_800A4ACC(d);
        *(u16 *)(d + 0x46) &= 0x7FFF;
        return;
    }
}

/* The actor's +0x9B four-state throw-and-handoff sequence.  State 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF -- from a 7-entry jump table when
   d+0x1C has 0x2000 and from a three-way compare chain otherwise -- and
   hands it to func_800A94A0, re-arming +0x96 at 0xA and cueing 0x703; on an
   empty triple it tears the actor down instead.  State 1 waits on
   func_8003F270, then spawns one 0x112 node driven by func_b42_801708D4,
   copies the caller's 0x30-byte renderable template over the node's slot,
   dresses it from D_801760FC and seeds the node's mover from b plus
   func_8003DE58's projection of the ACTOR's own slot, and falls into state
   2's +0x96 countdown.  State 3 waits for the +0x14 facing bits, then
   re-dresses c from the bank-local D_b42_8017609C, drops D_80083460's live
   count and installs func_b42_801714B8.  403 words, 0 differing.

   IT OWNS ONE TABLE.  jtbl_b42_80170838: 8 words = 7 cases + 1 pad, dispatch
   bound 7, 7 distinct targets (jtcheck).  Its MINVAL IS 1, so entry k is
   case k+1 -- the listing subtracts 1 before the `sltiu 0x7'.  Entries:
     1 -> d + 0x8   2 -> d + 0xB   3 -> d + 0xE
     4 -> the `default' arm's own label (case 4 is NOT written; gcc pads to
          maxval with the default label, which is also the range check's
          `beqz' target)
     5 -> k = 1, d + 0x8   6 -> k = 1, d + 0xB   7 -> k = 1, d + 0xE
   Seven cases is odd and a pad word is present, so another 8-aligned rodata
   object follows it in this unit's .rodata.  The STATE switch itself owns no
   table: four cases compile to a compare chain.

   TWIN: the landed func_b09_80173230 in csrc/ovl_dng_b09.c (461 words,
   dngtwin 0.824, flagged "yours is 58 words shorter").  The flag was right
   and it was STATEMENTS that were missing, not references -- b09 has five
   states where this has four, and its state 3 arms a 0x7E80/+0xAC swap and a
   0x100 bit this copy simply does not have.  b09's -0x40 frame and its seven
   saves match this listing's, so even its budget facts were safe to take.

   THREE OF b09'S FACTS RE-PRICED HERE, all three HELD:

   | spelling                                              | words | aligned |
   | as written below                                       |  403  |    0    |
   | the D_801760FC dress site store-first, the byte read   |  403  |    8    |
   |   inline in the func_80047784 call                     |       |         |
   | the `else' compare chain written ascending 1, 2, 3     |  405  |   11    |

   The dress site's price is b09's number to the digit (8 on a 461-word body,
   8 here): the target computes `%hi(D_801760FC)' once, loads the byte off
   the high half, and only THEN forms the `%lo' pointer for the `r + 0x2C'
   store, so `j' must be read into its own local BEFORE the store.  The
   compare chain must be descending; b09 priced the ascending order at 9 and
   it is 11 here.  b09's remaining facts -- `s16 k', the `n0' local, the `m'
   local, __builtin_abs over two `if's, the D_8008346C-before-0xA6 order and
   the 0x30 blob as `u32 w[0xC]' -- were taken as written and NOT re-measured,
   except that the blob's type is witnessed by this listing directly: a
   four-register aligned block-move loop is emitted, which alignment 1 would
   have turned into the runtime `andi 3' test and an lwl/lwr path.

   THE ONE FACT OF ITS OWN IS STATE 3'S CONSTANT, AND IT WAS READ OUT OF A
   REGISTER RATHER THAN GUESSED.  `*(s8 *)(c + 0x4) == 2': the target's
   `bne $v0, $a0' reuses the $a0 that the state dispatch loaded with 2 four
   blocks earlier, so the constant IS 2 and gcc shares one pseudo between the
   switch's `case 2:' compare and this one.  Written `== 1' -- the value $a0
   holds at the TOP of the dispatch, which is what the listing looks like it
   says -- the body is still 403 words and 2 aligned differences, both of
   them just the constant 2 moving from $a0 to $v0, because gcc then has to
   keep the 1 alive.  A listing's register REUSE is the only witness for this
   and the word count cannot see it at all.

   `--raw' is 58 differing and every one is a relocation: 43 %hi/%lo and
   `jal' words plus the 15 in-function `j's reorg emits. */
void func_b42_801730E4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    UnkNode *n0;
    UnkNode *n;
    UnkVec *m;
    SVECTOR out;
    u8 *g;
    u8 *e;
    u8 *q;
    u8 *r;
    u8 *r0;
    u8 *w;
    u8 *h;
    s16 k;
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    k = 0;
    n0 = (UnkNode *)(p - 0x20);
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
                e = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)e;
                q = *(u8 **)(e - 0x14);
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
                    e = *(u8 **)(d + 0x60);
                    if (e != NULL)
                    {
                        q = *(u8 **)(e - 0x14);
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
                *(s16 *)(p + 0x96) = 0xA;
                p[0x9B]++;
                func_800A56E0(0x703);
            }
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
            *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
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
        *(s16 *)(p + 0x96) = 0xA;
        p[0x9B]++;
        n = func_8003FD64(0x112, D_80083498);
        if (n != NULL)
        {
            w = (u8 *)n + 0x20;
            *(s16 *)(w + 0x96) = 0xF;
            n->unk_10 = (u32)func_b42_801708D4;
            *(u8 **)(w + 0xA8) = (u8 *)b;
            *(s16 *)(w + 0x94) = *(u16 *)(d + 0x2A);
            r = (u8 *)n->unk_0C;
            *(UnkDngB42Word48 *)r = *(UnkDngB42Word48 *)c;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            r[0xE] = 0x80;
            r[0xD] = 0x80;
            r[0xC] = 0x80;
            func_8004491C(n, (u32)D_80045340);
            j = D_801760FC[0];
            *(u8 **)(r + 0x2C) = D_801760FC;
            func_80047784(r, j, 0);
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x12) = 0xFF80;
            *(u16 *)(r + 0x14) |= 0xC;
            r0 = (u8 *)n0->unk_0C;
            m = n->unk_08;
            if (func_8003DE58(*(u8 **)(r0 + 0x8), r0, &out, 0) != NULL)
            {
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + (u16)out.vx;
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + (u16)out.vy;
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + (u16)out.vz;
            }
        }
    case 2:
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0 || (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
            p[0x9B]++;
        }
        break;
    case 3:
        if ((*(s8 *)(c + 0x4) == 2 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0x14;
        }
        h = (u8 *)&D_80083460;
        if (*(s32 *)(h + 0xC) == 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
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
        if (*(u8 **)(c + 0x2C) != D_b42_8017609C)
        {
            *(u8 **)(c + 0x2C) = D_b42_8017609C;
            func_80047784(c, D_b42_8017609C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if (*(s32 *)(h + 0xC) == 0)
        {
            *(u16 *)(h + 0xA) -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
            func_800A4ACC(d);
            d[0x73] = 0;
            d[0x72] = 0;
            d[0x6D] -= 1;
            *(u16 *)(d + 0x46) &= 0x7FFF;
            func_800A56E0(0xB4);
        }
        break;
    }
}

/* The banked room script's four-state glide, switching on p[0x9B]: state 0
   arms the +0x96 counter and calls func_800AD4D0, state 1 seeds the velocity
   from the D_8006CCD8/D_8006CCE8 facing pair while the counter runs, state 2
   steers onto the tile centre with the two-numerator divide, and state 3
   stops, re-centres and installs func_b42_801714B8.  178 words, 0 differing.

   THE TWIN IS THE LANDED func_b23_80173B4C (csrc/ovl_dng_b23.c, 198 words,
   dngtwin 0.947) and every statement below is its, with three deltas read off
   the listing: state 1's tick-to-zero arm arms the counter to 0xC and not 6;
   func_800AAA54's fourth argument is the bank-local D_801760D4; and state 3
   has NO facing-table swap -- b23's `if (c+0x2C == D_80174E98)' arm with its
   func_80047784 re-send is absent, which is the whole 20-word difference.

   DROPPING THAT ARM IS WHAT MAKES THIS BODY NEED A SPELLING OF ITS OWN, AND
   THE FIX IS TO WRITE THE STOP BLOCK TWICE.  b23 reaches its func_800AAA54
   bail-out from state 0's `else' by `goto stop' into a label inside state 2.
   Spelled that way here the body is 178 words with 19 ALIGNED DIFFERENCES,
   every one of them c and d holding each other's register: the target has
   c in $s2 and d in $s3, the `goto' spelling has them the other way round.
   The cause is measured, not guessed.  b23's missing arm carried three c
   references; without it flow counts c at 7 references over 84 insns and d at
   9 over 114, and global.c's allocno_compare ranks on
   floor_log2(n_refs) * n_refs / live_length: c scores 2*7/84 = 0.1667 against
   d's 3*9/114 = 0.2368, loses, and is allocated second.  Writing the whole
   three-store-plus-func_800AAA54 block out in BOTH state 0's `else' and
   state 2 -- which jump2 then cross-jumps back into the single block at
   .Lb42_801738C4 that the target has -- puts c at 8 references over 90 insns,
   and 8 is the first count whose floor_log2 is 3: 3*8/90 = 0.2667 wins.  ONE
   extra reference was the whole job, and it had to be the one that crossed
   the power-of-two boundary.  (The duplicate also adds four b references.
   That does NOT disturb p and b: p goes to 21/216 = 0.3889 against b's
   18/214 = 0.3364 and keeps $s0.)  It is probably macro residue -- the block
   is a coherent unit of work, three mover stores and one call, which is what
   a `STOP(p, b, c)' macro body would look like -- so it is written out rather
   than wrapped.

   b23's other named facts were carried over unchanged and not separately
   re-measured here, because the body landed at 0 with them: the D_8006CCD8
   index is NOT a local (all four lookups recompute it), state 1's counter
   read wants the compound assignment's VALUE in an `s32' while state 2's tail
   wants a POST-decrement, and the first numerator is the three-statement form
   with the second left as two. */
void func_b42_80173730(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
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
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_801760D4);
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
            *(s16 *)(p + 0x96) = 0xC;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_801760D4);
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
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b42_801739F8(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b42_80173A18(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b42_80173A38(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b42_80173A58(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}

/* The banked room script's three-state tail, switching on the state index at
   p + 0x9B: the relinked twin of the resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c), which is this body statement for statement
   with four local changes -- two separate bank-local facing tables
   (D_801760EC in state 0, D_801760F4 in state 1's re-dress) where the
   resident uses D_800E262C for both, func_80047784 for func_8003DB94,
   func_b42_8017412C/func_b42_801714B8 for func_800DA660/func_800D8C64, and
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
void func_b42_80173A90(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801760EC;
        func_80047784(c, D_801760EC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
                func_b42_8017412C(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801760F4;
        func_80047784(c, D_801760F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
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
        *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_801760EC facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b42_801714B8. The resident
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
void func_b42_80173E20(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801760EC;
        func_80047784(c, D_801760EC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_801760F4;
            func_80047784(c, D_801760F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b42_8017412C(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801760F4;
        func_80047784(c, D_801760F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
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
   call emits the bare `jal` either way); hoisting D_b42_80176094 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b42_8017412C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_801760EC && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b42_80176094;
        func_80047784(c, D_b42_80176094[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b42_801741F4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b42_801714B8;
        }
    }
}

/* 14 words, 0 differing. */
void func_b42_80174258(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}

/* 14 words, 0 differing. */
void func_b42_80174290(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}

/* 14 words, 0 differing. */
void func_b42_801742C8(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}

/* The same integrator as func_b42_80170B48 above with a shorter tail: the
   func_800478B8 send is unconditional and at the top, hitting the ground
   zeroes all THREE velocity words rather than only +0x58, there is no +0x20
   frame counter and no scale ease, and the 0x8000 retire pair hangs off the
   +0x1E life alone.  It is the +0x10 driver that func_b42_801744DC and
   func_b42_801746EC install on the nodes they spawn.  119 words, 0 differing.

   NO TWIN: no carved sibling at ratio >= 0.60, nothing in xsibs at any
   ratio.  Written off the listing.  func_800A45D8's (u16, u16, s32)
   signature is csrc/ovl_dng_s15.c's.

   THE ONE FACT IS THE GROUND-HEIGHT LOCAL, and it is two facts at once --
   its TYPE and its POSITION:

   | spelling                                          | words | aligned |
   | as written below: `s32 h' after the +0x58 ease    |  119  |    0    |
   | `s32 h' one statement earlier                     |  118  |    2    |
   | `s16 h'                                           |  120  |    4    |
   | `s16 h' one statement earlier                     |  120  |    5    |
   | no local -- the compare reads *(s16 *)(b + 0xA)   |  117  |   63    |

   `s16' is a HImode pseudo, loaded `lhu', and cse then derives `h' from the
   `lhu' the func_800BCB04 argument needs, paying `sll'/`sra'.  `s32' forces
   the independent `lh $s0, 0xA($s2)' the target has beside it.  Dropping the
   local entirely loses a callee-saved register outright -- three saves
   against the target's four, with `p' and `b' sliding down to $s0/$s1 -- so
   the last row is a PSEUDO-COUNT fault, not an idiom, and a reference bump
   is the wrong tool for it.

   `--raw' is 8 differing: the 5 `jal's and the 3 D_800814A0 relocations. */
void func_b42_80174300(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;

    *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) - 1;
    func_800478B8(c);
    b->unk_00 += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x50) += *(s32 *)(p + 0x5C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x50);
        *(s32 *)(p + 0x50) = 0;
        *(s32 *)(p + 0x5C) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x54);
    *(s32 *)(p + 0x54) += *(s32 *)(p + 0x60);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x54);
        *(s32 *)(p + 0x54) = 0;
        *(s32 *)(p + 0x60) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x58);
    *(s32 *)(p + 0x58) += *(s32 *)(p + 0x64);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x58) = 0;
        *(s32 *)(p + 0x54) = 0;
        *(s32 *)(p + 0x50) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        *(s16 *)(p + 0x1E) = 0;
    }
    if (*(s16 *)(p + 0x1E) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The bank's 0x212 burst spawner: allocate a node with func_b42_80174300 as
   its +0x10 driver and 0xF frames of life at the payload's +0x1E, dress the
   slot from D_80045340 with a random 3-bit spin at +0x1A, seed the mover
   from the caller's three halfwords plus a +-0x20/-0x40 jitter, set the
   +0x50/+0x54 velocity pair from the bank-local D_8017610C record the
   owner's +0x1C selects and the +0x58/+0x64 fall from constants, jitter all
   three by 14 bits, and tint the renderable 0x80 grey at 0x1000 scale out of
   the D_800DEC70 template.  132 words, 0 differing.

   TWIN: the landed func_s58_80024F80 / func_s58_80024CA4 pair in
   csrc/ovl_dng_s58.c (183 words).  dngtwin's best row for this body is 0.649
   against a different function and would not have found them; CALLEE.txt's
   6-of-7 symbol overlap did, on D_80045340 + D_800DEC70 + D_8017610C +
   func_8003DB94 + func_8003FC64 + func_8004491C + rand.  s58's body is this
   one plus a stack record table and three extra mover adds; strip those and
   the remainder is word-for-word this.

   BOTH OF s58'S FACTS HOLD, and each is re-priced on this body:

   | spelling                                              | words | aligned |
   | as written below                                       |  132  |    0    |
   | the jitter as `= *(u16 *)(m + 0x2) + (rand & 0x3F)     |  132  |    6    |
   |   - 0x20', i.e. the constant associated OUTSIDE        |       |         |
   | D_8017610C as `s16 D_8017610C[]' indexed [k*2]/[k*2+1] |  132  |    2    |

   The jitter must be a `+=' with the constant INSIDE the addend: written the
   other way the sum comes out `addu' before `addiu' where the target has
   `addiu $v1, $v1, -0x20' first.  s58 priced that at 8 on its body, 6 here.
   And the velocity table must be an array of a RECORD read through FIELDS:
   as a flat `s16' array the SECOND access alone comes out
   `addu $v1, $v1, $v0' against the target's `addu $v0, $v0, $v1'.  The
   record is UnkDngB09A28, which this unit already declared and did not use.

   `--raw' is 18 differing, all relocations: 10 `jal's (7 of them `rand') and
   the 4 %hi/%lo pairs for D_80045340, func_b42_80174300, D_8017610C and
   D_800DEC70. */
void func_b42_801744DC(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x1E) = 0xF;
        n->unk_10 = (u32)func_b42_80174300;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_1A = (rand() & 7) << 9;
        r->unk_06 = 0;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0x3F) + -0x40;
        *(s32 *)(q + 0x58) = 0xFFFA0000;
        *(s32 *)(q + 0x64) = 0x20000;
        *(s32 *)(q + 0x50) = D_8017610C[*(s16 *)(p + 0x1C)].unk_00 << 18;
        *(s32 *)(q + 0x54) = D_8017610C[*(s16 *)(p + 0x1C)].unk_02 << 18;
        *(s32 *)(q + 0x50) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x54) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x58) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DEC70, 0);
    }
}

/* func_b42_801744DC above with TWO constants changed and nothing else: this
   bank has two of the burst spawner.  132 words, 0 differing.  The two
   listings' opcode streams are identical and differ in exactly two words,
   the 0xF -> 0x4 life at the payload's +0x1E and the D_800DEC70 -> D_800DECF8
   slot template.  Every fact in func_b42_801744DC's comment was measured on
   that body and none was re-measured here, because there is nothing between
   the two for one to bite on. */
void func_b42_801746EC(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x1E) = 0x4;
        n->unk_10 = (u32)func_b42_80174300;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        r->unk_1A = (rand() & 7) << 9;
        r->unk_06 = 0;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        *(s16 *)((u8 *)m + 0x2) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0x6) += (rand() & 0x3F) + -0x20;
        *(s16 *)((u8 *)m + 0xA) += (rand() & 0x3F) + -0x40;
        *(s32 *)(q + 0x58) = 0xFFFA0000;
        *(s32 *)(q + 0x64) = 0x20000;
        *(s32 *)(q + 0x50) = D_8017610C[*(s16 *)(p + 0x1C)].unk_00 << 18;
        *(s32 *)(q + 0x54) = D_8017610C[*(s16 *)(p + 0x1C)].unk_02 << 18;
        *(s32 *)(q + 0x50) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x54) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        *(s32 *)(q + 0x58) += 0xFFF80000 + ((rand() & 0x3FFF) << 6);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
    }
}

/* Walk the room's +0x5C actor ring for the first OTHER actor standing on
   cell (x, y) within 0x20 of height z whose +0x13 state is OUTSIDE
   0x33..0x36 and which func_800A41F0 admits, and return its base.  65 words,
   0 differing.

   TWIN: the landed func_b09_80174800 in csrc/ovl_dng_b09.c (74 words,
   dngtwin 0.705 via its b10 clone).  Its ring idiom carried verbatim -- the
   `h = p' head copy with the PARAMETER reused as the cursor, the
   `*(u8 **)(p + 0x5C) + 0x20' step, the `*(u8 **)(p - 0x14)' cell record and
   the `(u32)(p[0x13] - 0x33)' band test.  The deltas off this listing: the
   height field is the payload's own +0x88 rather than the twin's
   `*(u8 **)(p - 0x18)' plus 0xA, the band is +-0x20 not +-0x38, the lower
   bound is `>=' where the twin has `>', and this one RETURNS a pointer where
   the twin does work in place.

   TWO FACTS, both mine and both priced:

   | spelling                                              | words | aligned |
   | as written below                                       |   65  |    0    |
   | `return p - 0x20' in place of the `g' local            |   63  |   62    |
   | the +0x13 band test as `< 4' (the twin's sense)        |   65  |    1    |

   The return value needs its OWN local assigned at the TOP of the loop body:
   the target computes `addiu $s1, $s0, -0x20' in the first `bne's delay slot
   and returns `$s1', which it can only do if the subtraction is a pseudo
   that exists before the gate.  Written inline at the `return' the body is
   two words short and loses a callee-saved register.

   AND THE BAND TEST'S SENSE INVERTS BETWEEN THIS BODY AND
   func_b42_80174A00 BELOW, which is otherwise the same walk: that one
   returns when +0x13 IS in 0x33..0x36, this one proceeds when it is NOT.
   Copying one into the other is 1 aligned difference -- the `bnez' alone --
   which is exactly the cheap-looking kind a word count cannot catch.

   `--raw' is 1 differing, the func_800A41F0 `jal'. */
u8 *func_b42_801748FC(u8 *p, u16 x, u16 y, s16 z)
{
    u8 *h;
    u8 *q;
    u8 *g;

    h = p;
    p = *(u8 **)(h + 0x5C) + 0x20;
    while (p != h)
    {
        q = *(u8 **)(p - 0x14);
        g = p - 0x20;
        if (q[0x24] == x && q[0x25] == y && *(s16 *)(p + 0x88) <= z + 0x20 &&
            *(s16 *)(p + 0x88) >= z - 0x20 && (u32)(p[0x13] - 0x33) >= 4 &&
            (s16)func_800A41F0(p) != 0)
        {
            return g;
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
    return NULL;
}

/* The same +0x5C ring walk as func_b42_801748FC above, without the
   func_800A41F0 test and with the band test the other way round: return the
   base of the first other actor on cell (x, y) within 0x20 of height z whose
   +0x13 state IS in 0x33..0x36.  41 words, 0 differing -- a leaf with no
   frame at all, so every pseudo is a temp.

   Same donor (func_b09_80174800, csrc/ovl_dng_b09.c) and the same one fact:
   the returned pointer needs its own local `g = p - 0x20' at the top of the
   loop body, which is where the target's `addiu $a3, $a0, -0x20' in the
   first `bne's delay slot comes from.  Written inline at the `return' it is
   41 words / 6 aligned differences -- the right length, so the count cannot
   see it; the rows are the missing pseudo and the two loop-invariant height
   bounds swapping $t1/$t2.

   `--raw' is 0 differing: this body has no relocations whatever. */
u8 *func_b42_80174A00(u8 *p, u16 x, u16 y, s16 z)
{
    u8 *h;
    u8 *q;
    u8 *g;

    h = p;
    p = *(u8 **)(h + 0x5C) + 0x20;
    while (p != h)
    {
        q = *(u8 **)(p - 0x14);
        g = p - 0x20;
        if (q[0x24] == x && q[0x25] == y && *(s16 *)(p + 0x88) <= z + 0x20 &&
            *(s16 *)(p + 0x88) >= z - 0x20 && (u32)(p[0x13] - 0x33) < 4)
        {
            return g;
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b42", func_b42_80174AA4);

/* The state-spawner for this bank's thrown object: allocate a 0x212 node
   driven by func_b42_80174AA4, park the owner at the payload's +0x28 and
   clear its +0x2C/+0x30, step the target cell one square along the owner's
   +0x2A facing through the bank-local D_8017610C delta table, put the mover
   on that cell's centre at func_800BCB04's ground height, copy the owner's
   facing and its 3-bit octant to +0x1A/+0x1C, dress the slot 0x2400 by
   0x1C00 and black out of the 12-byte D_8017612C template published at the
   slot's +0x8, and then -- if func_b42_801748FC finds an actor standing on
   the target cell -- record it at +0x30, raise 0x100000 in its +0x34 and cue
   func_8009A3D0 on its cell.  147 words, 0 differing.

   NO TWIN: no carved sibling at ratio >= 0.60, nothing in xsibs.  What made
   it cheap is that it is surrounded by its own family -- it SPAWNS the driver
   whose spawners func_b42_801744DC and func_b42_801746EC I had just landed
   (so the n+0x20 payload layout, the n->unk_0C re-read and the
   func_8004491C/D_80045340 dress were already written) and it CALLS
   func_b42_801748FC (so the ring walk's u16/u16/s16 signature was settled).
   Both typedefs came off this unit's own declaration block, both flagged by
   dngtypes as declared-but-unused here: UnkDngB09A28 for the D_8017610C
   record and UnkDngB20Name12 for the 12-byte block copy, whose alignment of
   1 is what forces the `lwl'/`lwr' + `swl'/`swr' pairs instead of a word
   loop.

   THE ONE FACT IS THE DELTA TABLE'S TYPE, the same lever as
   func_b42_801744DC's with a different record:

   | spelling                                              | words | aligned |
   | as written below, both reads through FIELDS            |  147  |    0    |
   | `((u8 *)D_8017610C)[k]' and `[k + 2]'                  |  148  |    9    |

   D_8017610C is read as a {s16; s16} record by the two spawners and as a
   {u8; pad; u8; pad} record here -- `lbu' at +0 and +2 off a byte offset the
   `(... >> 7) & 0x1C' mask produces directly, so the index is NOT scaled and
   the access must go through a cast pointer.  Through a plain `u8 *' index
   the FIRST access is byte-identical and the SECOND comes out
   `addu $a0, $a0, $v0' against the target's `addu $v0, $v0, $a0', which is
   why UnkDngB42Cell exists: identical field expressions get the same
   canonical operand order.

   MEASURED AND FREE, so NOT the reason this lands: the two cell bytes passed
   to func_b42_801748FC as `(s8)q[0x4C]' instead of `*(s8 *)(q + 0x4C)'
   (147 / 0 either way).  The `lbu' + `sll 24'/`sra 24' + `andi 0xFFFF' at
   those two argument sites comes from the u16 PARAMETER truncating a signed
   byte, not from how the byte is spelled.

   `--raw' is 14 differing, all relocations: 6 `jal's and the 4 %hi/%lo pairs
   for D_80045340, func_b42_80174AA4, D_8017610C and D_8017612C. */
void func_b42_80175494(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;
    u8 *t;
    u8 *u;
    u8 *w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0x28) = p;
        *(s32 *)(q + 0x30) = 0;
        *(s32 *)(q + 0x2C) = 0;
        n->unk_10 = (u32)func_b42_80174AA4;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        q[0x4C] = c[0x24] +
                  ((UnkDngB42Cell *)((u8 *)D_8017610C + ((*(u16 *)(p + 0x2A) >> 7) & 0x1C)))
                      ->unk_00;
        q[0x4D] = c[0x25] +
                  ((UnkDngB42Cell *)((u8 *)D_8017610C + ((*(u16 *)(p + 0x2A) >> 7) & 0x1C)))
                      ->unk_02;
        r->unk_06 = 6;
        m = n->unk_08;
        m->unk_00 = (*(s8 *)(q + 0x4C) * 64 + 0x20) << 16;
        m->unk_04 = (*(s8 *)(q + 0x4D) * 64 + 0x20) << 16;
        m->unk_08 = func_800BCB04h(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                   *(u16 *)((u8 *)b + 0xA) - 0x20)
                    << 16;
        *(s16 *)(q + 0x1A) = *(u16 *)(p + 0x2A);
        *(s16 *)(q + 0x1C) = (*(u16 *)(p + 0x2A) >> 9) & 7;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1C = 0x2400;
        r->unk_1E = 0x1C00;
        *((u8 *)r + 0xE) = 0;
        *((u8 *)r + 0xD) = 0;
        *((u8 *)r + 0xC) = 0;
        *(UnkDngB20Name12 *)((u8 *)n + 0x54) = *(UnkDngB20Name12 *)D_8017612C;
        r->unk_08 = (u32)((u8 *)n + 0x54);
        t = func_b42_801748FC(p, *(s8 *)(q + 0x4C), *(s8 *)(q + 0x4D), *(s16 *)((u8 *)m + 0xA));
        *(u8 **)(q + 0x30) = t;
        if (t != NULL)
        {
            u = t + 0x20;
            w = *(u8 **)(t + 0xC);
            *(u32 *)(u + 0x14) |= 0x100000;
            func_8009A3D0(w[0x24], w[0x25], (*(u32 *)(u + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_800C77D0(n, (u8 *)m, 8, 0x300);
    }
}

/* State 0x17, built out of the banked room scripts' shared idioms rather
   than from one twin: the D_80083462 gate, then func_800A2B5C either
   side of func_800C7930(d - 0x20, b, 8, 0x300), then the pointer-local
   D_801760DC facing table published at c+0x2C and one func_80047784.

   THE ONE SPELLING FACT IS STATEMENT ORDER: `*(s32 *)(p + 0x8C) = 0' sits
   BETWEEN the p+0x9A and p+0x9B byte stores. Written after both of them --
   the order csrc/ovl_dng_b08.c uses -- the 0x8C word store and the 0x9B byte
   store come out transposed: 64 words, 2 differing.

   64 words, 0 differing. */
void func_b42_801756E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    t = D_801760DC;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    d[0x6D] -= 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b42", func_b42_801757E0);

/* 14 words, 0 differing. */
void func_b42_80176024(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}

/* 14 words, 0 differing. */
void func_b42_8017605C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
    }
}
