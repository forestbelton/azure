#include "common.h"

s32 rand(void);

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b33_80171FCC(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_8017418C[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_b33_80174174[];
extern u8 D_801741CC[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b33_80171884(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b33_801711A4();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AB464();

s32 func_800AC7EC();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

extern u8 D_801741C4[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, UnkVec *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b33_80173D10(u8 *p, u8 *b, u8 *c, u8 *d);

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
extern u8 D_8017419C[];

void func_800478B8(u8 *);
void func_800DBA90(u8 *);

void func_b33_80171884(u8 *p, void *b, void *c, u8 *d);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
s16 func_8009FB34(u16 x, u16 y);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, u8 *t, void (*h)());
void func_b33_801716CC();
void func_b33_80172190(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b33_801722B8(u8 *a, void *b, void *c, s32 d);
extern u8 D_80174194[];
extern u8 D_801741AC[];
s32 func_800A56E0(s32);
void func_800ACF88(u8 *);
void func_800A2FE0(u8 *);
u8 *func_800A32A4(u8 *);
void func_8009A028(u8 *);
extern s16 D_8008346A;
void func_800A2B04(UnkVec *, s32, s32);
s32 func_8003F270(void);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
void func_800DB2DC(u8 *, u8 *, u8 *, s16);
extern s32 D_8008346C;
extern u8 D_801741B4[];
extern u8 D_801741BC[];

s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
extern u8 D_80174184[];
extern u8 D_8006CCF8[];
extern void (*D_801741D4[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b33", func_b33_8017086C);

/* The UnkNode +0x10 PER-FRAME DRIVER for this bank's thrown/placed actor.
   Past the D_80083462 0x2000 gate -- where it runs the +0x8C continuation
   only when that continuation is this object's own func_b33_801711A4 -- it
   runs func_800A9E70, the record's +0x8C step, the +0x9A entry of this bank's
   D_801741D4 handler table, re-aims through func_800AA36C when the +0x6D
   facing moved under it, steps the mover, runs the +0x9D bob, re-skins the
   slot for the eight-way facing and leashes +0x92/+0x88 to the ground height
   func_800BCB04 reports. 400 words, 0 differing, ON THE FIRST COMPILE.
   No jump table and no rodata of its own -- the listing opens at `glabel'.
   relcand: ok, 6 data symbols and 9 calls. 1 member (dngfam.py).

   THE TWIN IS THE LANDED func_b41_80170A78 (csrc/ovl_dng_b41.c), 376 words,
   dngtwin 0.920, and the two prologues are BYTE-IDENTICAL -- -0x30 frame,
   $s0..$s5 + $ra in the same order at the same offsets -- so all three of its
   fact classes were in scope and its alias split carried unchanged ($s1 p,
   $s5 v, $s4 c, $s2 the `d' alias, with the ONE p + 0x1C site that reads
   through `p' rather than `d' exactly where b41 has it).

   DELTAS FROM b41, all read off the listing. The handler is
   func_b33_801711A4 for func_b41_80171058 and the table D_801741D4 for
   D_80174900. The +0x92 clamp is against `e - 0x20'/`e - 0x28' where b41 has
   `e - 0x18'/`e - 0x20' -- b25's constants on b41's e-relative shape. The
   0x40000-FALSE arm clears p + 0xA8 and p + 0xAC where b41 clears p + 0xB2
   and p + 0xA4. The +0xA publish ADDS p + 0xAE where b41 SUBTRACTS p + 0xA6.
   AND THE WOBBLE ARM IS A DIFFERENT BLOCK, which is where 74 of the 400 go:
   b41's one-way D_80174880 test with a func_800644B8 multiply is replaced by
   a TWO-WAY dress test -- D_8017418C clears p + 0xAC outright, while
   D_b33_80174174 steps a counter at p + 0xA0 (reset, with p + 0xAC, when
   *(u16 *)(c + 0x4) == 0x100) and then adds -0x33333 or 0x12492 to p + 0xAC
   on whether that counter is below 5. That 37-word block is written out in
   BOTH arms of the c + 0x14 0x8000 gate.

   ELEVEN SPELLINGS MEASURED HERE (aligndiff; b41's own figure in brackets),
   because a first-compile landing proves nothing about its own choices:

   | as written below                                   | 400 |   0 |     |
   | no `d = p' alias, `p' at every site                | 397 | 161 | [149]
   | `s32 dir' instead of `s16 dir'                     | 397 |   9 | [9]
   | the 0xAC step as one `+= cond ? A : B'             | 396 |  29 | own
   | ONE shared `e' for both duplicated clamp arms      | 400 |   0 | [0]
   | ONE shared `f' for both duplicated reset arms      | 400 |   0 | own
   | one facing local, `k' only and no `i' at all       | 400 |   0 | [0]
   | the 0xA0 counter through a `w' local (b41's form)  | 400 |   0 | own
   | c + 0x2C through a `tp' local at both wobble sites | 400 |   0 | own
   | the tail publish as (0x88 + 0x92) + 0xAE          | 400 |   0 | own
   | the 0x40000-FALSE block ONCE, reached by a `goto' | 400 |   0 | own

   SO THE ONLY LOAD-BEARING SPELLINGS ARE THE ALIAS, THE `dir' WIDTH AND THE
   0xAC STEP. The `dir' width reproduces b41's 9 AND b25's 9 to the digit; the
   alias holds in direction at 161 against b41's 149 and b25's 146. b25's
   facing-local row, which it prices at 63, is FREE here for the SECOND
   independent time (b41 was the first), so that row of b25's table is priced
   on b25 only and should not be paid for again.

   THE DUPLICATED-ARM LOCAL RULE IS VACUOUS ON THIS BODY, and both halves were
   measured, not assumed: neither the clamp's `e' nor the reset's `f' needs
   its own copy. Both are assigned once and thereafter only read, which is
   exactly the `local-alloc.c:1759' refinement -- `combine_regs' only refuses
   to tie an output to a dying INPUT, so only a read-modify-write local wants
   per-copy scope. The p + 0xA0 counter here IS a read-modify-write, and it
   measures 0 with no local at all, so the rule never gets a chance to fire.
   The separate `e'/`e2' and `f'/`f2' pairs below are kept because that is the
   form the whole-unit re-verification ran against, not because they earned it.

   THE 0xAC STEP IS THIS BODY'S OWN FACT AND IT IS NOT AN ALLOCATION ONE. Two
   separate `+=' statements in two arms is what matches; the ternary is FOUR
   WORDS SHORT, because the target reloads p + 0xAC inside each arm (the `lw'
   is scheduled in between the constant's `lui' and `ori', so jump2 can only
   cross-join the trailing `addu'/`sw') and a ternary gives it one reload in
   the merge block.

   AND ONE 2x2 CROSS EACH WAY FOUND NO CONJUNCTION, which is worth recording
   because the method was asked for: alias x `dir' width is 0 / 9 / 161 / 165,
   and the 0xAC form x the 0xA0 counter local is 0 / 29 / 0 / 29. Both
   products are the sum of the parts, so on this body one-at-a-time pricing
   was adequate and the facts are independent.

   THE FOURTH PARAMETER IS NOT WITNESSED. The body never reads it, no caller
   of it has landed in this unit yet (the spawner is still the
   func_b33_8017086C stub) and a three-parameter signature measures the same
   400 / 0, so the arity below is b41's family convention and nothing more.
   The listing's `addu $a3, $a0, $zero' at every call site is this function
   passing its OWN `p' on as the callee's fourth argument, not evidence of an
   incoming fourth. */
void func_b33_80170B64(u8 *p, UnkVec *v, u8 *c, u8 *t)
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
    s16 f2;
    s16 g;

    d = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b33_801711A4)
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
    D_801741D4[p[0x9A]](p, v, c, p);
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
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                if (*(u8 **)(c + 0x2C) == D_8017418C)
                {
                    *(s32 *)(p + 0xAC) = 0;
                }
                else if (*(u8 **)(c + 0x2C) == D_b33_80174174)
                {
                    *(s16 *)(p + 0xA0) = *(u16 *)(p + 0xA0) + 1;
                    if (*(u16 *)(c + 0x4) == 0x100)
                    {
                        *(s16 *)(p + 0xA0) = 0;
                        *(s32 *)(p + 0xAC) = 0;
                    }
                    if (*(s16 *)(p + 0xA0) < 5)
                    {
                        *(s32 *)(p + 0xAC) += -0x33333;
                    }
                    else
                    {
                        *(s32 *)(p + 0xAC) += 0x12492;
                    }
                }
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (*(s16 *)(p + 0x92) > e - 0x20)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < e - 0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
        else
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xAC);
            *(s16 *)(p + 0xA8) = 0;
            *(s32 *)(p + 0xAC) = 0;
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
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xAC);
            *(s16 *)(p + 0xA8) = 0;
            *(s32 *)(p + 0xAC) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                f2 = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                     *(u16 *)(d + 0x88);
                if (f2 < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = f2;
                    p[0x9D] = 0;
                    v->unk_14 = 0;
                    *(s32 *)(d + 0x1C) |= 0x8000000;
                }
            }
        }
        else
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                if (*(u8 **)(c + 0x2C) == D_8017418C)
                {
                    *(s32 *)(p + 0xAC) = 0;
                }
                else if (*(u8 **)(c + 0x2C) == D_b33_80174174)
                {
                    *(s16 *)(p + 0xA0) = *(u16 *)(p + 0xA0) + 1;
                    if (*(u16 *)(c + 0x4) == 0x100)
                    {
                        *(s16 *)(p + 0xA0) = 0;
                        *(s32 *)(p + 0xAC) = 0;
                    }
                    if (*(s16 *)(p + 0xA0) < 5)
                    {
                        *(s32 *)(p + 0xAC) += -0x33333;
                    }
                    else
                    {
                        *(s32 *)(p + 0xAC) += 0x12492;
                    }
                }
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                e2 = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                   (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                     *(u16 *)(d + 0x88);
                if (*(s16 *)(p + 0x92) > e2 - 0x20)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < e2 - 0x28)
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
    *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0xAE) + (*(u16 *)(d + 0x88) + *(u16 *)(p + 0x92));
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The banked room script's IDLE handler -- the body this bank parks at the
   actor's +0x8C and the one the switch below re-installs through
   func_800AAF00. OWNS A JUMP TABLE: jtbl_b33_80170808, rodata offset 0x0,
   `.align 3', TWELVE entries for the +0x46 state 1..12, an even count so
   there is no pad word. 330 words, 0 differing, ON THE FIRST COMPILE.
   1 member (dngfam.py).

   THE TABLE'S TWELVE ENTRIES, states 1..12 in order: 1, 2, 3 -> the
   func_800AAF00 re-install arm; 4 -> default; 5, 6, 7 -> the func_800A0818 /
   D_800814A8[0x9A] arm; 8, 9 -> the func_b33_80171FCC arm; 10, 11 ->
   default; 12 -> the func_800A9A0C arm, which jump2 cross-joins with the
   d + 0x1C 0x20 gate's copy of the same two statements.

   THE TWIN IS THE LANDED 330-word func_b17_801713A8 (csrc/ovl_dng_b17.c),
   dngtwin 0.985 and the same word count. Same frame, `addiu $sp, $sp, -0x38'
   with $s0..$s3 and $ra; the register map is rotated, p $s0 / b $s3 / c $s1 /
   d $s2 against b17's p $s0 / d $s1 / c $s2 / b $s3, which per the save-order
   rule predicted exactly what happened: every SELECTION fact of b17's
   reproduced and both of its ALLOCATION facts inverted. Measured here, b17's
   own figure last:

     as written below                            330 /  0  |   0
     D_800814A8 inline, not held in `w'          331 / 16  |  16
     the d + 0x2A store below the w[0x9A] test   331 / 14  |  14
     the case 8/9 arm written AFTER case 5/6/7   330 / 31  |  31
     the second dress table through `e' too,
       shared with the first                     331 /  8  |  11
     b38's `do { } while (0)' on the 0x40000 set 330 / 98  |  65
     the three shared-tail sites `goto'-ed to
       one label instead of written out          330 / 66  |   0

   THE TWO ALLOCATION FACTS INVERT AND THE THREE SELECTION FACTS REPRODUCE TO
   THE DIGIT. b17 records the `do'-wrapper as needed by b38 and 65 here; this
   body is 98 without it being wanted at all, i.e. further still from the
   target than b17 was. And b17's one FREE fact is the expensive one here: the
   shared func_b33_80171884 tail must be written out at all three sites, where
   b17 could collapse it to a `goto' for nothing -- 66 aligned differences,
   with the whole epilogue region displaced.

   MEASURED AND FREE, so NOT the reason this lands:
     the first dress table as the bare symbol
       instead of through the `e' local          330 /  0
     func_b33_801716CC called with its four
       arguments instead of none                 330 /  0
     the `(s16)' cast dropped on func_800A1C58    330 /  0
   The 0x1000 escape calls func_b33_801716CC with nothing written to
   $a0..$a3, and b17 keeps a second empty-paren declaration of its own
   equivalent for that reason -- but the four moves are between hard registers
   that still hold p, b, c and d, so flow deletes them and the two spellings
   are byte-identical. The empty-paren form is kept because it is what the
   listing shows; it is not load-bearing.

   LOAD-BEARING AND THIS BODY'S OWN: the `(s16)' cast on func_b33_801722B8 --
   without it 329 words / 9 aligned differences, because the `sll 16' the
   target uses to test the low half goes missing and the branch inverts. The
   casts on the s16-returning func_800A1C58 and func_8009FD7C cost nothing.

   DELTAS FROM b17, all read off the listing: p[0x9B] = 1 where b17 sets 2;
   the section-4 dress is a SINGLE-table test against D_8017418C that installs
   the table it tested, and adds c[0x5] = 1, p + 0xAE = 0 and p + 0xA8 = 0
   after the func_80047784 call, whose third argument is a literal 0 at both
   dress sites; the tables are D_801741CC, D_801741C4, D_8017418C, D_80174194
   and D_801741AC; the 0x80000 arm ends in func_b33_80173D10; cases 8 and 9
   share one arm (func_b33_80171FCC, then func_b33_80172190 if it returns
   zero) and that arm is FIRST in source order; and the shared tail is
   func_b33_80171884. */
void func_b33_801711A4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b33_801716CC();
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_801741CC)
        {
            e = D_801741C4;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_801741CC)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801741C4) != 0)
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
        if (*(u8 **)(c + 0x2C) != D_8017418C)
        {
            *(u8 **)(c + 0x2C) = D_8017418C;
            func_80047784(c, D_8017418C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            c[0x5] = 1;
            *(s16 *)(p + 0xAE) = 0;
            *(s16 *)(p + 0xA8) = 0;
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174194) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(s16 *)(p + 0xA8) = 0;
            func_b33_80173D10(p, (u8 *)b, c, d);
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
            func_b33_80171884(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b33_801722B8(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b33_80171884(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b33_80171FCC(p, b, c, d) != 0)
            {
                return;
            }
            func_b33_80172190(p, b, c, d);
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
            func_800AAF00(p, b, c, D_801741AC, func_b33_801711A4);
            return;
        default:
            func_b33_80171884(p, b, c, d);
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

/* The banked room script's path-step advance, the twin of func_b35_801714D8
   (csrc/ovl_dng_b35.c) and of the resident overlay's func_800D904C: past the
   d + 0x71 / d + 0x8A step gates, dress the renderable from D_b33_80174174,
   retire the current waypoint through func_8009A3D0, take the next pair out
   of d + 0x74 / d + 0x7C, and re-aim with func_8009A21C and func_800A0818.
   110 words, 0 differing, on the first compile, and WITH NO NEW DECLARATIONS
   -- D_b33_80174174, D_80083228, D_80083462, func_80047784, func_8009A3D0,
   func_8009A21C and func_800A0818 were all already in this unit.

   Three deltas from those twins, all read off the listing. There is no
   func_8009A66C call and no `(s16)t == 3' arm, so `p[0x9A] = 0xF' is
   unconditional. func_800A0818 is called AFTER func_8009A21C, not before it,
   and its first two arguments are the `x'/`y' locals straight -- which is why
   this copy needs none of the phantom-zero device func_b35_801714D8 and
   func_800D904C both carry. And the D_b33_80174174 dress has only the `!=' arm;
   func_800D904C's `else if' on the c + 0x14 0x6000 bits is absent. */
void func_b33_801716CC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b33_80174174)
        {
            *(u8 **)(c + 0x2C) = D_b33_80174174;
            func_80047784(c, D_b33_80174174[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (u16 *)(p + 0x98));
        *(u32 *)(d + 0x1C) |= 0x40000000;
        p[0x9A] = 0xF;
        *(s32 *)(p + 0x8C) = 0;
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
   AND PASSED ON. func_b33_80171FCC's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A8C0 2 words, func_b33_80171FCC 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b33_80171884(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b33_80171FCC(p, b, c, d) == 0)
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
   D_8017418C facing table and re-aim through func_8009C93C.

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
s32 func_b33_80171FCC(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_8017418C;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* State 0x11's entry: clear the record's +0x71 top bit, and unless the
   D_80083462 0x2000 gate is up or func_800A2B5C reports the record busy,
   step it forward through func_800C7930, then -- if it is still not busy --
   mark state 0x11/0, clear the +0x8C word, hang the bank-local D_8017419C
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
void func_b33_80172190(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_8017419C;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    *(u16 *)(p + 0x98) |= 0x8;
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
s32 func_b33_801722B8(u8 *a, void *b, void *c, s32 d)
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
        func_b33_80171884(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b33_80171884(p, b, c, p);
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
   func_b33_801711A4. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b33_801723FC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b33_801711A4;
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
void func_b33_80172490(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b33", func_b33_801724F4);

/* The banked room script's four-state grab-and-throw. State 0 picks the limb
   slot out of (d + 0x46) & 0x3FFF -- d + 0x8, d + 0xB or d + 0xE, with the
   d + 0x1C 0x2000 arm's cases 5, 6 and 7 raising `k' on the way to the same
   three -- aims the target from D_800814A8 or func_800A05A4, re-dresses the
   +0xA4 node's renderable from D_801741B4 and, if func_800A94A0 accepts,
   hands off through func_800DB2DC; state 1 waits on func_8003F270, state 2
   counts p + 0x96 down and re-dresses from D_801741BC, and state 3 clears the
   vector, re-dresses from D_8017418C and tears the actor down back onto
   func_b33_801711A4. 341 words, 0 differing, ON THE FIRST COMPILE.
   1 member (dngfam.py).

   OWNS A JUMP TABLE: jtbl_b33_80170838, rodata offset 0x30, `.align 3', for
   state 0's FIRST switch -- cases 1..7 over (d + 0x46) & 0x3FFF. SEVEN live
   entries, in state order 1..7: .Lb33_80172900, .Lb33_801728F8,
   .Lb33_801728F0, .Lb33_80172908 (case 4 carries the default arm's label),
   .Lb33_801728B0, .Lb33_801728A8, .Lb33_801728A0. THE EIGHTH `.word
   0x00000000' IS NOT A CASE -- it is the `.align 3' pad between this table
   and jtbl_b33_80170858 at 0x80170858, which func_b33_801731FC owns. The
   d + 0x1C 0x2000 `else' switch has three cases and compiles to a compare
   chain whose arms jump2 cross-joins with this one's, so the unit gains ONE
   table here and not two. The outer four-state switch on p + 0x9B is a
   compare chain as well.

   THE TWIN IS THE LANDED 304-word func_b38_80172A40 (csrc/ovl_dng_b38.c),
   dngtwin 0.797, taken over b17's 0.807 and b04's 0.845 on the SHAPE: b38 is
   the only candidate with the same twin-switch state 0. Its frame does NOT
   match -- b38 is -0x38 with seven callee-saved registers, this body is -0x40
   with eight, $s0..$s7, because it carries two extra prologue locals -- so by
   the frame/shape split only b38's SELECTION facts were expected to carry,
   and that is what happened. Measured here, with b38's figure last:

     as written below                               341 /  0  |  0
     `s32 k'                                        340 /  8  |  7
     `u8 k'                                         341 /  1  |  1
     `s8 k'                                         341 /  0  |  0
     `k > 0' instead of `k != 0'                    341 /  1  |  1
     the inner three-case switch written 1, 2, 3    343 / 11  | 10
     `D_8008346C = 0' after the 0xA6 decrement      342 / 10  |  9
     the D_8006DE24 record indexed inline, with no
       tbl / i / e locals                           341 /  1  |  1
     func_800DB2DC called BEFORE the c + 0x14 clear 341 /  6  | 50

   ALL SEVEN OF b38's PRICED SPELLINGS HELD, three to the digit and the rest
   within one, across +37 words and an extra callee-saved register. The only
   one that moved far is the statement-order fact, 6 here against b38's 50 for
   the same transposition on its own hand-off call -- a REFERENCE-COUNT AND
   STATEMENT-ORDER fact, the class the brief says nothing predicts. `s16 k'
   and `s8 k' both measure 0, so the target does not choose between them;
   `s16' is kept because that is what the sibling records.

   MEASURED AND FREE, so NOT the reason this lands:
     the state 2 facing table read through
       *(u8 **)(c + 0x2C) rather than the symbol    341 /  0  | free in b38
     state 3's two dress gates as one `&&' gate     341 /  0
   The second is the corrected `fold_truthop' rule: the two tests are against
   DIFFERENT objects (c + 0x2C and c + 0x14), so nesting them buys nothing.
   They are left nested only because that is how the listing reads.

   LOAD-BEARING AND THIS BODY'S OWN, neither of which b38 has a site for:

   (1) THE +0xA4 NODE'S TWO SUB-OBJECTS ARE PROLOGUE LOCALS. `h' is
   *(u8 **)(p + 0xA4) + 0x20 and `h2' is the same load + 0x28, both assigned
   before the dispatch, which is why the listing computes $s6 and $s7 off one
   `lw 0xA4' in the prologue and why this frame saves two registers more than
   b38's. Read inline at their three use sites instead it is 340 words / 47
   aligned differences.

   (2) THE TWO +0x72 / +0x73 SIGN FLIPS ARE `__builtin_abs'. Written as two
   `if (x < 0) x = -x;' it is 340 words / 15 aligned differences; the target's
   `bgez' / `negu' pair with no store in between is the builtin's expansion.

   DELTAS FROM b38, all read off the listing: this body has FOUR states, not
   three, and b38's whole state 2 splits in two here -- the p + 0x96 countdown
   and the D_801741BC dress become state 2, the vector clear and the
   D_80083460 teardown become state 3. State 0's success path raises no
   +0x98 0x8 bit and clears no d + 0x1C bits, calls
   func_800DB2DC((u8 *)b, c, d, 0xA) where b38 calls func_800DAE44((u8 *)b, 4),
   and sets p + 0x96 to 6; it also adds the `*(u16 *)(h + 0x4) &= 0x7FFF' and
   the unconditional D_801741B4 dress on `h2' before func_800A94A0. State 2's
   dress is UNGUARDED -- no `!= D_801741BC' test -- and state 3's is guarded on
   both c + 0x2C and the c + 0x14 0x6000 bits and raises `h + 0x4's 0x8000 and
   zeroes p + 0xA8. func_80047784's third argument is a literal 0 at all three
   dress sites, where b38 passes *(s16 *)(p + 0xA0) at one of them. */
void func_b33_801727C8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    u8 *h;
    u8 *h2;
    s16 k;
    s32 i;
    s32 x;
    s32 y;

    k = 0;
    h = *(u8 **)(p + 0xA4) + 0x20;
    h2 = *(u8 **)(p + 0xA4) + 0x28;
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
            *(u16 *)(h + 0x4) &= 0x7FFF;
            *(u8 **)(h2 + 0x2C) = D_801741B4;
            func_80047784(h2, D_801741B4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            if (func_800A94A0(d, g, k, p + 0x98) == 0)
            {
                break;
            }
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800DB2DC((u8 *)b, c, d, 0xA);
            func_800A56E0(0x703);
            *(s16 *)(p + 0x96) = 6;
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
            *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
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
        if ((s16)(--*(u16 *)(p + 0x96)) > 0 && (*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u16 *)(p + 0x98) |= 0x80;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801741BC;
        func_80047784(c, D_801741BC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) != D_8017418C)
        {
            if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
            {
                *(u16 *)(h + 0x4) |= 0x8000;
                *(s16 *)(p + 0xA8) = 0;
                *(u8 **)(c + 0x2C) = D_8017418C;
                func_80047784(c, D_8017418C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
        }
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b33", func_b33_80172D1C);

/* The banked room script's five-state death throw, switched on p + 0x9B and
   driven by the D_8006CCD8 / D_8006CCE8 facing pair read ONCE before the
   dispatch: state 0 seeds the velocity, state 1 drifts it by 0x1C000 and
   zeroes the whole vector when func_800BCB04 reports 0x200 or more, state 2
   re-dresses the renderable through the func_800ACF88 arm and kicks
   func_800A56E0(0x805), state 3 drifts again behind the c + 0x14 gate, and
   state 4 fades c + 0xC..0xE out of p + 0x96 and, when the fade is under
   0x18, releases D_80083460's +0x10 claim and tears the actor down.
   174 words, 0 differing, ON THE FIRST COMPILE. 1 member (dngfam.py).

   OWNS A JUMP TABLE: jtbl_b33_80170858, rodata offset 0x50, `.align 3', FIVE
   entries for states 0..4 at .Lb33_8017327C, .Lb33_801732D0, .Lb33_80173334,
   .Lb33_80173394 and .Lb33_801733D0, matching the `sltiu $v0, $a1, 0x5' gate;
   no two states share a label and everything else goes to the epilogue. The
   odd count takes NO pad word, because this table is LAST in the unit's
   .rodata -- the segment ends at 0x8017086C, which is 0x858 + 5 * 4.
   States 0, 1 and 2 fall through to their successors; state 3 ends in its own
   `j' to the epilogue.

   THE TWIN IS THE LANDED 164-word func_b04_80173724 (csrc/ovl_dng_b04.c),
   dngtwin 0.911. Frame `addiu $sp, $sp, -0x28' with $s0..$s3 and $ra,
   p $s0 / b $s1 / c $s3 / d $s2. Re-measured here, with b04's own figure last:

     as written below                            174 /  0  |   0
     the two `&= ~' written as one `&= ~0x8040000'
                                                 171 /  9  |   5
     state 2's two d + 0x14 tests as one `&&'    173 / 11  |  11
     state 4's fade byte read three times
       instead of through a local                178 /  7  |   7
     the b + 0x14 drift as the struct field
       `b->unk_14 += 0x1C000', both sites        174 /  0  |  12

   FOUR OF b04's FIVE FACTS HOLD, TWO TO THE DIGIT, AND ITS MEM_IN_STRUCT_P
   FACT GOES FREE. b04 needs the plain cast at every drift site because gcc's
   fixed-scalar-versus-varying-struct rule lets sched1 hoist the fixed scalar
   D_8008346A's `lh' above a struct store; here it costs nothing, because
   state 1's drift store is scheduled into the func_800BCB04 call's delay slot
   and a call is a barrier the hoist cannot cross. The plain cast is kept
   anyway -- it is free and it is what the sibling records.

   MEASURED AND FREE, so NOT the reason this lands:
     the facing index as the byte offset the listing shows,
       `*(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(d + 0x6A) >> 8) & 0xE))',
       instead of the array ref                  174 /  0  |  free in b04
     func_800BCB04's result through an `s16' local
       instead of tested in place                174 /  0

   LOAD-BEARING AND THIS BODY'S OWN: THE FACING PAIR MUST BE READ BEFORE THE
   SWITCH. Moved inside state 0, which is the only state that uses it, it is
   175 words / 45 aligned differences -- the two `lh's leave the prologue and
   every later block shifts, so the table's five entries point past moved
   code. That is what the hoisted `lh $a2' / `lh $a3' ahead of the dispatch in
   the listing is telling you, and it is the same shape b04 records.

   DELTAS FROM b04, all read off the listing: func_800A56E0(0x805) is in state
   2 after the func_800ACF88 arm, not in state 0; state 1 gains the
   func_800BCB04 gate over b + 0x2, b + 0x6 and b + 0xA that zeroes b + 0x14,
   b + 0x10 and b + 0xC; state 2 sets c + 0x10 to 0x20 rather than b04's 0x60
   and adds `*(s16 *)(c + 0x12) -= 0x80'; and state 4 does NOT drift
   b + 0x14 by 0x20000. */
void func_b33_801731FC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 x;
    s32 y;
    s32 v;

    x = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    y = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        b->unk_0C = x << 16;
        b->unk_10 = y << 16;
        b->unk_14 = 0xFFF40000;
        p[0x9B] += 1;
        /* fallthrough */
    case 1:
        *(s32 *)((u8 *)b + 0x14) += 0x1C000;
        if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) >=
            0x200)
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
        }
        if (D_8008346A != 0)
        {
            return;
        }
        p[0x9B] += 1;
        /* fallthrough */
    case 2:
        if (*(s32 *)(d + 0x14) & 0x4000)
        {
            if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
            {
                func_800ACF88(d);
            }
        }
        func_800A56E0(0x805);
        *(s16 *)(c + 0x10) = 0x20;
        *(u16 *)(c + 0x14) |= 0xC;
        *(s16 *)(c + 0x12) -= 0x80;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        *(s32 *)((u8 *)b + 0x14) += 0x1C000;
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0x80;
        p[0x9B] += 1;
        return;
    case 4:
        *(s32 *)(d + 0x1C) |= 0x10000000;
        v = p[0x96];
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        if ((s16)(*(u16 *)(p + 0x96) -= 0x18) < 0x18)
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            func_800A2FE0(d);
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b33_801734B4(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b33_801734D4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b33_801734F4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 pushes p + 0x90
   out by 0x80000 and re-dresses the renderable from the bank-local D_801741CC
   facing table, state 1 is the long refusal chain with the D_80174184
   dress/flag/bump tail written out TWICE, state 2 runs the p + 0x96 countdown
   out, parks the mover and hands the actor back to func_b33_801711A4.
   297 words, 0 differing, ON THE FIRST COMPILE. It owns no jump table -- the
   three-case switch compiles to a compare chain. relcand: ok, 6 data symbols
   and 20 calls. 1 member (dngfam.py).

   THE TWIN IS THE LANDED func_b06_801734B0 (csrc/ovl_dng_b06.c), 268 words,
   dngtwin 0.814 -- AND ITS FRAME DOES NOT MATCH. This body is -0x30 with
   $s0..$s6 + $ra where b06 is -0x28 with $s0..$s4 + $ra, so by the taxonomy
   every budget fact was out of scope; in the event b06 has none and ALL FIVE
   of its priced rows reproduced here in direction, four of them in magnitude.
   The extra two callee-saved registers are the `q'/`r' pair below.

   DELTAS FROM b06, read off the listing: state 0 opens with
   `*(s32 *)(p + 0x90) += 0x80000'; the facing tables are D_801741CC in state
   0 and D_80174184 at both state-1 dress sites, where b06 has one table at
   all three; the 0x80000 arm clears only p + 0xA8 (b06 steps p + 0x92 by
   p + 0xB6 first and clears two fields); the hand-over is
   func_b33_801711A4; both dress copies carry an extra
   `*(u16 *)(q + 0x4) |= 0x8000' and set p + 0x96 to 3 and clear 0x4000 in
   p + 0x98; state 2 gains the countdown, the p + 0x98 0x4000 raise, the
   b->unk_14 = 0xFFF80000 park and a 0 write-back with a p + 0xA8 clear.
   AND THE WHOLE 27-WORD IDLE-TIMER BLOCK IS NEW: between the 0x80000 arm and
   the d + 0x6D test this copy runs the p + 0x96 countdown down, and on expiry
   drops 0x8000 in q + 0x4, calls func_80047784(r, 0x27, 0) and re-seeds
   p + 0x96 from `(rand() & 0xF) + 0x20'. That is the bulk of the 297 - 268.
   `rand' is the real name of the 0x80069EF8 the listing shows (checked in
   splat/common_syms.txt) and needs no declaration.

   NINE SPELLINGS MEASURED HERE (aligndiff; b06's own figure in brackets):

   | as written below                                   | 297 |  0 |     |
   | q and r computed INSIDE case 1, their only user    | 295 |  9 | own
   | `p[0x9B] += 1' by ONE `goto bump', not 3 sites     | 297 | 52 | [33]
   | ONE shared &D_80083460 local for the 2 dress sites | 333 | 62 | [30]
   | all five &D_80083460 locals collapsed into one     | 293 | 33 | [35]
   | case 0's `p + 0x96 = 0' AFTER the 0xA decrement    | 300 |  9 | [8]
   | the first func_80042900 test without its `(s16)'   | 296 |  3 | [3]
   | the 0x80000 arm's p + 0xA8 clear BEFORE AA888      | 297 |  2 | own
   | the two p + 0x96 countdowns split from their tests | 297 |  0 | own
   | the `v' local dropped, both d + 0x1C tests inline  | 297 |  0 | [0]

   THE `q'/`r' HOIST IS THIS BODY'S OWN AND THE LARGEST THING b06 CANNOT TELL
   YOU. Both pointers are derived from *(u8 **)(p + 0xA4) and both are read
   ONLY inside case 1, yet the listing computes them in the PROLOGUE, ahead of
   the dispatch, and holds them in $s2 and $s6 for the whole function. Moved
   into the one case that uses them the body is TWO WORDS SHORT with 9 rows.
   This is the same shape func_b33_801731FC's header records for its facing
   pair forty words up in this unit, measured there at 45 rows -- so in this
   unit, a value the listing materialises before a switch was hoisted in the
   SOURCE, and that is now two for two.

   TWO OF b06's ROWS CAME IN MUCH LARGER HERE AND THE REASON IS STRUCTURAL,
   NOT ALLOCATION. Sharing one &D_80083460 local across the two dress copies
   is 333 words, THIRTY-SIX LONG: the shared local's live range runs through
   both copies, which stops jump2 cross-joining them, so the second copy stays
   in the output. b06's 30 rows are the allocation cost alone on a body where
   the join survived. The same reading explains the `goto bump' row coming in
   at 52 against b06's 33 while the word count does NOT move: with three
   written-out bumps jump2 folds them back into the one block the target has,
   so the length is identical either way and the only thing the three sites
   buy is `p's reference count -- which, with seven callee-saved registers in
   play here instead of five, is worth more than it was on b06.

   AND ONE 2x2 CROSS: the `q'/`r' hoist x the three-site bump is
   0 / 52 / 9 / 60. 9 + 52 = 61 against 60 measured, so the two are
   independent to within one row and there is no conjunction to find.

   NOT MEASURABLE RATHER THAN FREE: the d[0x25] gate written the other way
   round, with the func_800A2C34 guard as the `!= 0' arm's body, is 297 / 0 --
   but that rewrite leaves the control-flow graph identical, so it is one
   program under two spellings and the tools have nothing to compare. */
void func_b33_8017352C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;
    u8 *q;
    u8 *r;
    u8 *e;
    s32 v;

    q = *(u8 **)(p + 0xA4) + 0x20;
    r = *(u8 **)(p + 0xA4) + 0x28;
    switch (p[0x9B])
    {
    case 0:
        *(s32 *)(p + 0x90) += 0x80000;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801741CC;
        func_80047784(c, D_801741CC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 0;
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80174184;
            func_80047784(c, D_80174184[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            *(u16 *)(q + 0x4) |= 0x8000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
                return;
            }
            *(s16 *)(p + 0x96) = 3;
            *(u16 *)(p + 0x98) &= 0xBFFF;
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
            *(s16 *)(p + 0xA8) = 0;
            func_b33_80173D10(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(q + 0x4) &= 0x7FFF;
            func_80047784(r, 0x27, 0);
            *(s16 *)(p + 0x96) = (rand() & 0xF) + 0x20;
        }
        if ((*(u16 *)(r + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(q + 0x4) |= 0x8000;
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
        *(u8 **)(c + 0x2C) = D_80174184;
        func_80047784(c, D_80174184[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(q + 0x4) |= 0x8000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
            return;
        }
        *(s16 *)(p + 0x96) = 3;
        *(u16 *)(p + 0x98) &= 0xBFFF;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(p + 0x98) |= 0x4000;
            b->unk_14 = 0xFFF80000;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_14 = 0;
        *(s16 *)(p + 0xA8) = 0;
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
        return;
    }
}

/* The banked room script's three-state tail, state index at +0x9B, and the
   near-twin of the landed func_b03_801736F4 (csrc/ovl_dng_b03.c), itself the
   banked copy of the resident overlay's func_800DA398: state 0 re-dresses the
   renderable from the bank-local D_801741CC facing table, state 1 is the long
   refusal chain with the D_801741C4 dress/flag/bump tail written out twice,
   state 2 hands the actor back to func_b33_801711A4. 208 words, 0 differing.

   THE DELTA FROM func_b03_801736F4 IS ONE WORD: the +0x80000 arm clears the
   slot's halfword at +0xA8 BETWEEN the func_800AA888 call and the
   func_b33_80173D10 hand-over. Written before the call instead it is 19
   aligned differences.

   `p[0x9B] += 1' IS WRITTEN OUT AT ALL THREE REACHING SITES, NOT REACHED BY
   `goto'. jump2 cross-jumps the three copies back into the one block the
   target has, so the length is the same either way -- but the three extra
   references on `p' are what win it $s2. With the single `goto bump' label
   func_b03_801736F4 uses, this body is 208 words with 17 ALIGNED DIFFERENCES,
   every one a register: `p' takes $s3 and `b' takes $s2, the other way round
   from the target, exactly the allocno_compare loss func_b03_80173358's
   header measures from the other side. The extra +0xA8 store on `p' is not
   enough on its own.

   Measured and rejected: `b' declared `void *' with `(UnkVec *)' casts at the
   three calls -- 208 words, 16 differing, so the parameter's type is not what
   decides the swap.

   The rest is func_b03_801736F4's shape verbatim, including its four
   SEPARATE `&D_80083460' pointer locals (one per arm, with the gate's shared
   with the state 1 tail) and the facing tables named directly at all three
   dress sites rather than held in a local. */
void func_b33_801739D0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801741CC;
        func_80047784(c, D_801741CC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_801741C4;
            func_80047784(c, D_801741C4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
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
            *(s16 *)(p + 0xA8) = 0;
            func_b33_80173D10(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801741C4;
        func_80047784(c, D_801741C4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
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
        *(s32 *)(d + 0x1C) &= ~0x208;
        *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
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
   call emits the bare `jal` either way); hoisting D_b33_80174174 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b33_80173D10(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_801741CC && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b33_80174174;
        func_80047784(c, D_b33_80174174[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b33_80173DD8(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b33_801711A4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b33_80173E3C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}

/* 14 words, 0 differing. */
void func_b33_80173E74(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}

/* 14 words, 0 differing. */
void func_b33_80173EAC(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}

/* The two-state fade step of the node at p, switched on the s16 at p + 0x4C:
   state 0 re-sends the view, drops its +0x4/+0x5 pair once the +0x14 flags
   carry one of 0x6000, steps the +0xC colour down by 0x181818 and advances
   the state when its low byte falls under 0x30; then it swings p + 0x8 by
   0xA, parks `-(((s16)(p + 0x8) >> 2) ** 2) >> 4' in p + 0x4 and builds
   func_800DBA90's 0x20-byte matrix record out of p + 0x4, the output vector
   and two eight-byte copies of p + 0x14 and p + 0xC, then writes the
   rotated vector back into b + 0x2 / +0x6 / +0xA. State 1 is the standard
   teardown: raise 0x8000 in the node's -0x2 and in D_800814A0.
   98 words, 0 differing.

   No twin: no landed body in the block calls func_800DBA90, and the resident
   overlay's two callers (func_800DB3F8 and func_800DB660,
   csrc/ovl_dungeon_800D50FC.c) build the record for a different primitive.
   What came from func_800DB660 is the RECORD'S TYPE: `u8 rec[0x20]', which
   is both why it sits at 0x10($sp) with the output vector at 0x30($sp) and
   why the two field copies are `lwl'/`lwr' pairs with no runtime alignment
   test -- a `u8' array is alignment 1, so cc1psx expands an 8-byte copy
   straight into one unaligned load pair and one unaligned store pair per
   word. The copies are `memcpy(rec + k, p + n, 8)' with both pointers u8 *,
   which is the only spelling that keeps that alignment (see
   docs/matching-levers.md, "`memcpy's ALIGNMENT COMES OFF THE POINTER
   EXPRESSION").

   THE p + 0x8 STORE IS WRITTEN AFTER THE SHIFT THAT FEEDS THE SQUARE, AND
   THE SHIFTED VALUE NEEDS ITS OWN LOCAL. The target's `sh $v1, 0x8($s1)'
   sits between the `mult' and its `mflo', in the multiply's latency, which
   sched can only do if the store comes after the `sll'/`sra' in the insn
   stream. Written `*(s16 *)(p + 0x8) = k; k = (s16)k >> 2;' -- one local,
   store first -- the body is still 98 words but the store is emitted
   immediately and it is 5 aligned differences. */
void func_b33_80173EE4(u8 *p, u8 *b, u8 *c)
{
    u8 rec[0x20];
    SVECTOR vec;
    s32 j;
    s32 k;

    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            c[0x4] = 0;
            c[0x5] = 0;
        }
        *(s32 *)(c + 0xC) += 0xFFE7E7E8;
        if (c[0xC] < 0x30)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        k = *(u16 *)(p + 0x8) + 0xA;
        j = (s16)k >> 2;
        *(s16 *)(p + 0x8) = k;
        *(s16 *)(p + 0x4) = -(j * j) >> 4;
        *(u8 **)rec = p + 0x4;
        *(u8 **)(rec + 0x4) = (u8 *)&vec;
        memcpy(rec + 0x8, p + 0x14, 8);
        memcpy(rec + 0x10, p + 0xC, 8);
        *(s16 *)(rec + 0x18) = 1;
        *(s16 *)(rec + 0x1A) = 1;
        func_800DBA90(rec);
        *(s16 *)(b + 0x2) = vec.vx;
        *(s16 *)(b + 0x6) = vec.vy;
        *(s16 *)(b + 0xA) = vec.vz;
        break;
    case 1:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* The per-frame step a node of this bank parks at its +0x10: advance the
   mover by its own three velocity words, re-send the renderable through
   func_800478B8, and when the +0x48 countdown runs out raise 0x8000 in the
   node's -0x2 and in D_800814A0 so the engine reaps it. 38 words, 0
   differing, ON THE FIRST COMPILE. No jump table. relcand: ok, 1 data symbol
   and 1 call. 1 member (dngfam.py).

   THE TWIN IS THE LANDED 30-word func_b18_80173FF4 (csrc/ovl_dng_b18.c),
   dngtwin 0.853, and it is worth knowing that ITS comment names THIS stub as
   its own best row and records `NO TWIN READ' -- it was written from its
   listing. So the three facts it states are its own measurements, not
   inherited ones, and all three reproduce here:

   | as written below                                   | 38 |  0 |     |
   | the three b bumps AFTER the func_800478B8 call     | 41 | 26 | [21]
   | the countdown split into a store and a test        | 38 |  0 | [0]
   | the three bumps as plain casts, not UnkVec fields  | 38 |  0 | own

   THE ONE LOAD-BEARING SPELLING IS THAT THE MOVER BUMPS COME BEFORE THE CALL,
   and the mechanism b18 gives holds at the larger size: written after it, the
   three `lw'/`addu'/`sw' triples can no longer be scheduled around the call,
   the body is THREE WORDS LONG and 26 rows out. b18 pays 21 for one bump;
   this body pays 26 for three.

   DELTAS FROM b18: the countdown is at p + 0x48 rather than p + 0x1A, and
   where b18 bumps only b->unk_08 by a literal 0x10000 this body advances all
   three of the mover's words by its own +0xC/+0x10/+0x14 velocity.
   MEM_IN_STRUCT_P does not bite at any of the three sites -- the UnkVec field
   form and the plain cast both measure 38 / 0 -- so the field form is kept
   because it reads better, not because it was earned. */
void func_b33_8017406C(u8 *p, UnkVec *b, u8 *c)
{
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    func_800478B8(c);
    if ((s16)(--*(u16 *)(p + 0x48)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 14 words, 0 differing. */
void func_b33_80174104(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}

/* 14 words, 0 differing. */
void func_b33_8017413C(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b33_801711A4;
    }
}
