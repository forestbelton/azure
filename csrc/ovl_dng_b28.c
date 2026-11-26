#include "common.h"

s32 rand(void);

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b28_80171DAC(u8 *p, UnkVec *b, u8 *c, u8 *d);
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

extern Unk80083462 D_80083460;
extern u8 D_80174540[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80174518[];
extern u8 D_80174538[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b28_80171420(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b28_80170E5C();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
u16 func_800A6D30(void);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b28_80171E10(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u8 D_80082E80[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

s32 func_800AB464();

extern u8 D_80174528[];

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

extern u8 D_80174558[];

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
s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
void func_800BB044(u8 *);
s32 func_800A56E0(s32);
void func_800A2B04(UnkVec *, s32, s32);
extern s32 D_8008346C;
extern u8 D_80174520[];

UnkNode *func_8003FD64(s32, void *);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b28_80170A58(u8 *p, UnkVec *b, u8 *c);
extern u8 D_80083498[];
extern u8 D_80045340[];
extern u8 D_80174510[];

/* Spawn the 0x112 node this bank's actors live in: allocate it off
   D_80083498, dress it with D_80045340, put the caller's facing in the
   mover's +0xA and the tile pair in the slot's +0x24/+0x25, then pick the
   record's flag set from the low two bits of the first argument -- 1 raises
   0x6000 on +0x14/+0x1C, >=2 raises 0x2000, and 0 with no high bits left is
   the only arm that may roll func_800A6D30 for the D_80174538 skin -- and
   hand the result to func_800A9C18 and func_800AA36C. The relinked twin of
   func_b03_80170854 (csrc/ovl_dng_b03.c), with this bank's symbols, p[0x13]
   = 0xE in place of 3, no `*(u32 *)(p + 0x1C) |= 0x200' in the roll arm, and
   the +0x8C handler written per arm rather than once in the tail. 129 words,
   0 differing, 0 aligned differences.

   THE +0x2C FACING TABLE IS STORED IN ALL FOUR ARMS, NOT ONCE AFTER THE
   CHAIN -- which makes it a dead store, since the roll arm's D_80174538 is
   overwritten by it. Written once after the chain there is ONE
   `lui'/`addiu' pair for D_80174510 and the store sinks past the
   func_800A9C18 argument moves: 123 words, 41 aligned differences. Per arm,
   cross-jumping merges the bare `sw $v0, 0x2C($s1)' across all four and the
   address pair across the last two only, which is the target's three
   materialisations.

   THE 0x200 ROLL ARM IS `(a & ~3) == 0' WITH THE BARE HANDLER STORE AS THE
   FINAL `else', and its handler goes through `w', not `p'. The two arms
   swapped round is 129 words / 8 aligned differences (the `bnez'/`beqz' and
   the arm that stays inline). Through `p' in both, cross-jumping merges the
   two arms whole and three words go: 126 words, 5 aligned differences. The
   two locals are the same value -- `w = p' -- and only the register the
   store uses tells them apart.

   `*((u8 *)r + 0x24) = b' GOES BEFORE THE +0x25 STORE, which is the reverse
   of the order the two `sb's come out in. The other way round, `b' and `m'
   swap $s5 and $s6 -- 129 words, 8 aligned differences. It is global-alloc's
   closest pair: `m' is 4 refs over 80 insns (priority 0.100) and `b' 2 refs
   over 21 (0.095), a 4.8% gap, and putting the +0x24 store first takes one
   insn off `b's live range so the tie falls to the lower allocno number.

   This function owns NO rodata: its listing has no `.section .rodata'
   block and the two-bit dispatch is a compare chain, not a table.
   THIS COPY'S OWN CONSTANT: 0xe -> 0x1c.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b28_801737DC(u8 *p, u8 *b, u8 *c, u8 *d);

void *func_800990FC(void);
void *func_80099734(u8 *, u8 *);
void *func_80099194(u8 *, u8 *);
u8 *func_80099290(u8 *);
s32 func_800A5720(void *);
extern u8 D_801745CC[];
extern u8 D_801745F8[];
extern u8 D_8006CCF8[];
extern void (*D_80174568[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800478B8(u8 *);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);
s32 func_800644B8(s32);

void func_b28_80171420(u8 *p, void *b, void *c, u8 *d);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80174550[];
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
s16 func_8009FB34(u16 x, u16 y);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b28_80171B68(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b28_80171FD4(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b28_801720EC(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b28_8017236C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b28_80173E48(u8 *a, void *b, void *c, s32 d);
extern u8 D_80174530[];
extern u8 D_80174548[];
extern u8 D_80174560[];

void func_b28_80174460(u8 *p);
void func_b28_801744B8(u8 *p);
UnkNode *func_8003FC64(s32);
u8 *func_8003DE58(u8 *, u8 *, void *, s16);
void func_8003DB94(void *, void *, s32);
s32 func_80064584(s32);
void func_b28_80173F8C();
extern u8 D_800DEA68[];
extern u8 D_800DEEC0[];
void func_b28_80174254();
u8 *func_800D3F74(s16, s16, s16, s16);
void func_80042640(Unk42710 *, s16);
void func_b28_80174084(u8 *a, UnkVec *b, u8 *c);
u8 *func_b28_801742AC(u8 *p, UnkVec *b, u8 *c);

u8 *func_b28_80170854(s16 a, u16 b, u16 c, s16 d)
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
        p[0x13] = 0x1c;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = b;
        *((u8 *)r + 0x25) = c;
        w = p;
        k = a & 3;
        if (k == 1)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
            *(u8 **)((u8 *)r + 0x2C) = D_80174510;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
            *(u8 **)((u8 *)r + 0x2C) = D_80174510;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80174538;
                }
            }
            *(u32 *)(w + 0x8C) = (u32)func_b28_80170E5C;
            *(u8 **)((u8 *)r + 0x2C) = D_80174510;
        }
        else
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
            *(u8 **)((u8 *)r + 0x2C) = D_80174510;
        }
        n->unk_10 = (u32)func_b28_80170A58;
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

/* The banked room script's per-frame actor driver, the relinked twin of the
   resident overlay's func_800D8728 / func_800B274C: past the engine's 0x2000
   gate it runs func_800A9E70, then the record's own +0x8C continuation, then
   the +0x9A entry of this bank's D_80174568 handler table, re-aims the tile
   when the +0x6D facing moved under it, re-skins the slot for the eight-way
   facing, steps the mover by its velocity, runs the +0x9D bob counter and
   leashes +0x92/+0x88 to the ground height func_800BCB04 reports.

   THE NEAREST TWIN IS THE LANDED func_b09_80171320 (csrc/ovl_dng_b09.c), not
   the resident original: like this copy it has the +0x8C gate arm, no
   +0x98 0xFBFF mask and no +0x14 0x100000 test, and it writes the
   ground-height arm's common tail ONCE rather than duplicating it. Its three
   locals for the `& 7` facing index (`v` for the +0x94 store, `k1` for the
   +0x94 compare and the +0x2C subscript, `k2` for the D_8006CCF8 test, the
   last live across func_80047738 and the reason a sixth callee-saved
   register is saved) and its two separate +0x8C handler locals carry over
   unchanged. Four bank-local deltas from b09, each measured here:

   * THE 0x8000-CLEAR BLOCK COMES BEFORE THE HANDLER CALLS, not after them.
   * THE +0x8C HANDLER CALL, THE D_80174568 TABLE CALL AND func_800AA36C
     PASS `q`, NOT `p`, as the fourth argument, and the facing test reads
     `q + 0x6D`. Spelled `p` the dir test alone is one difference, an `lb`
     off $s2 where the target has $s1.
   * The gate compares the +0x8C handler against func_b28_80170E5C.
   * THE `p + 0x92 + q + 0x88 < r` ARM IS A BARE VOLATILE READ OF p + 0x98,
     not the siblings' `*(u32 *)(q + 0x1C) &= 0xF7FFFFFF`. That spelling is
     260 words with 10 aligned differences; the arm in the target is the
     single `lhu $v0, 0x98($s2)` whose destination is dead at the `j`, and a
     non-volatile `*(u16 *)(p + 0x98);` is deleted by flow -- 254 words with
     7 differences, the arm gone and the branch inverted.
     (docs/matching-levers.md, "A FOUR-WORD `if` ARM WHOSE ONLY CONTENT IS A
     LOAD IS A BARE `volatile` READ".)

   func_800BCB04 is called through the plain `s32` declaration here, not the
   `func_800BCB04h` s16 alias that func_b28_80173E48 in this unit needs: the
   result is kept raw and sign-extended at each test. 257 words, 0 differing. */
void func_b28_80170A58(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b28_80170E5C)
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
    D_80174568[p[0x9A]](p, b, c, q);
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
                *(volatile u16 *)(p + 0x98);
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

/* The bank's main actor handler and the bank's default +0x8C continuation:
   three early bails over the facing-table swap, then the twelve-way dispatch
   on `*(u16 *)(d + 0x46) & 0x3FFF'. 369 words, 0 differing, 0 aligned
   differences.

   IT OWNS A JUMP TABLE: jtbl_b28_80170808, 12 entries, at offset 0x0 of this
   unit's .rodata, 8-aligned. Index = (that mask) - 1, so the entries are
   cases 1 2 3 -> the func_800AAF00 arm (.Lb28_801712A0), 4 -> default
   (.Lb28_801712CC), 5 6 7 -> the func_800A0818 re-aim arm (.Lb28_8017124C),
   8 -> the func_b28_80171E10/func_b28_80171FD4 pair (.Lb28_80171198),
   9 -> the (d + 0x1C) 0x400 arm (.Lb28_801711D0), 10 -> default, 11 ->
   default, 12 -> the func_800A9A0C arm (.Lb28_80171290). Cases 4, 10 and 11
   carry no label of their own. The entry count is EVEN, so there is no
   trailing pad word to mistake for a thirteenth case.

   TWIN: the landed carved sibling func_b32_801710F4 (csrc/ovl_dng_b32.c),
   356 words. THIS IS THAT BODY PLUS THIRTEEN WORDS, and the thirteen are two
   whole arms rather than an allocation delta: b32 has a `case 10' of its own
   (-7 here, case 10 falls to `default') and its case 9 is a D_800DCF5B gate
   over two calls where this one is the (d + 0x1C) 0x400 / (d + 0x14) sign
   block followed by func_b28_801720EC and func_b28_8017236C (+20).
   Everything else is a rename -- D_80174F48 -> D_80174538, D_80174F40 ->
   D_80174530, D_80174F00 -> D_80174520, D_80174F08 -> D_80174548,
   D_80174F38 -> D_80174560, func_b32_80171684 -> func_b28_80171B68,
   func_b32_80174060 -> func_b28_801737DC, func_b32_80172364 ->
   func_b28_80173E48, func_b32_801718DC -> func_b28_80171420,
   func_b32_80172088 -> func_b28_80171E10, func_b32_8017224C ->
   func_b28_80171FD4.

   THE FRAME AND SAVE LIST ARE b32's EXACTLY (-0x38, s0 s1 s2 s3 s4 ra), BUT
   THE SAVE *ORDER* IS NOT: b32 copies $a2 into $s0 and $a3 into $s2, this
   body copies $a2 into $s2 and $a3 into $s0, so `c' and `d' hold each
   other's callee-saved register throughout. Nothing was needed for that --
   it falls out of case 9's extra references on `d' -- which is one more
   confirmation that a save-ORDER mismatch is not work to do.

   THE ONE LOAD-BEARING SPELLING IS case 9's READ-MODIFY-WRITE, AND IT NEEDS
   BOTH HALVES OF THE DEVICE AT ONCE. The landed func_b28_80171420 in this
   unit carries the same idiom and prices only the fully-plain spelling; the
   two halves are separated here for the first time:

   | spelling                                        | words | aligned diffs |
   | `cc' local + `do { } while (0)' (as below)      |  369  |      0        |
   | plain `*(s32 *)(d + 0x14) |= 0x80000000;'       |  369  |      4        |
   | `cc' local, NO do-while                         |  369  |      4        |
   | `do { } while (0)', literal constant inside     |  369  |      4        |
   | `if (1)' + `cc' local instead of the do-while   |  369  |      4        |
   | the loaded word hoisted, `v = ...; v | 0x8...'  |  369  |      4        |

   All four failing rows are the same four: the `lw' of d + 0x14 and the
   `lui 0x8000' exchanging $v0 and $v1, plus the `bltz' and the `or' that
   name them. MACRO RESIDUE, not a device: flow.c:2122 counts a reference
   once per ENCLOSING LOOP, so the wrapper is what bumps `cc' -- and with the
   literal there is no pseudo to bump, which is why neither half alone moves
   a single row. `if (1)' is not a loop and scores exactly as no device,
   confirming the rule in docs/matching-levers.md.

   b32's OTHER FOUR PRICED FACTS ALL HOLD HERE and three reproduce its
   figures to the digit (its word counts are 356-based, mine 369-based):

   | spelling                                        | words | aligned diffs |
   | `m'/`w' locals dropped: func_800A0818's result  |  370  |     17        |
   |   stored straight to +0x2A, D_800814A8 indexed  |       |               |
   |   inline at the 0x11 test          (b32: 17)    |       |               |
   | `case 12:' as its own top-level arm instead of  |  369  |     10        |
   |   nested in the `w[0x9A] != 0x11' body (b32: 10)|       |               |
   | `s32 n' instead of `s16 n'          (b32: 5)    |  370  |      5        |
   | all three facing-table locals collapsed into    |  369  |      2        |
   |   one `u8 *e'                       (b32: 2)    |       |               |
   | case 9 written BEFORE case 8                    |  369  |     28        |

   That last row is this body's version of b32's "case 9 before case 10"
   (which is NOT APPLICABLE here -- there is no case 10 arm). Arms emit in
   SOURCE order, so the source must list case 8, case 9, cases 5/6/7,
   case 12 nested, cases 1/2/3, default -- the address order .Lb28_80171198,
   _1D0, _24C, _290, _2A0, _2CC.

   b32's `e3 merged into e2' row is FREE HERE TOO, re-measured: 369 words,
   0 aligned differences. Those two name the same symbol, D_80174520, in
   disjoint arms. It is the THREE-way collapse in the table above that costs
   2, because `e' names a different table (D_80174530). b32's other three
   free rows were not re-priced.

   The d[0x25] == 0 arm's dress block is written out in full and jump2
   cross-joins it into the tail's by itself (`j .Lb28_801713C8' with $a1
   preloaded), exactly as in b32. Do not share it in the source.

   `--raw' is 83 differing and all 83 are relocations, so no masked word
   hides a constant address. */
void func_b28_80170E5C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;
    s32 cc;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b28_80171B68(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174538)
        {
            e = D_80174530;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174538)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80174530) != 0)
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
            e2 = D_80174520;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174548) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b28_801737DC(p, (u8 *)b, c, d);
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
            func_b28_80171420(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b28_80173E48(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b28_80171420(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b28_80171E10(p, b, c, d) != 0)
            {
                return;
            }
            func_b28_80171FD4(p, b, c, d);
            return;
        case 9:
            if ((*(s32 *)(d + 0x1C) & 0x400) != 0)
            {
                cc = 0x80000000;
                if (*(s32 *)(d + 0x14) >= 0)
                {
                    do
                    {
                        *(s32 *)(d + 0x14) |= cc;
                    } while (0);
                    *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
                }
            }
            if ((s16)func_b28_801720EC(p, b, c, d) != 0)
            {
                return;
            }
            func_b28_8017236C(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80174560, func_b28_80170E5C);
            return;
        default:
            func_b28_80171420(p, b, c, d);
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
    e3 = D_80174520;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's walk step: the relinked twin of
   func_b01_80171C34 in csrc/ovl_dng_b01.c, which is itself the resident
   overlay's func_800D92C0 with three extra pre-loop arms.  Transcribed from
   that landed body with this bank's symbols -- func_8009A66C for
   func_8009A8C0, func_b28_80171E10 for func_b01_8017237C -- and ONE
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
   BE DECLARED AND PASSED ON: func_b28_80171E10's call sets only $a0, $a2
   and $a3, so its $a1 is this function's own incoming $a1 reaching it
   untouched. */
void func_b28_80171420(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b28_80171E10(p, b, c, d) == 0)
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

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_80174518 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b28_80171DAC.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b28_80171DAC is called only on
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
void func_b28_80171B68(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80174518)
        {
            *(u8 **)(c + 0x2C) = D_80174518;
            func_80047784(c, D_80174518[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b28_80171DAC(p, b, c, d);
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

/* State 0x10, and hand c the facing cell that the heading at d+0x2A selects
   out of the bank-local byte table D_80174528: the relinked twin of the
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
     the local assigned inside the store, `= t = D_80174528'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80174528 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b28_80171DAC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80174528;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174540 facing table and re-aim through func_8009C93C.

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
s32 func_b28_80171E10(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    d[0x85] = 0;
    e = D_80174540;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The 0x11 hand-off that func_b14_80171E00 (above) also ends with, behind a
   three-gate chain instead of that one's five: D_80083462's 0x2000 bit, then
   func_800A2B5C both before and after the func_800C7930 push. On the far side
   it installs state 0x11 with a null handler, points the renderable at this
   bank's D_80174540 facing table and calls func_8009C93C. 70 words,
   0 differing, first compile.

   `*(s32 *)(p + 0x8C) = 0' GOES BETWEEN THE TWO +0x9A/+0x9B BYTES, which is
   not the order func_b14_80171E00 uses. Written 0x9A, 0x9B, 0x8C the two `sb's
   pair up and the `sw' sinks past them -- 70 words, 2 aligned differences.

   Two spellings that do NOT matter here, both 70/0: the gate through
   `g = (u8 *)&D_80083460' + 0x2 (the unit's usual idiom) rather than
   D_80083462 as its own symbol, and the three gates as one `&&' chain with an
   `else return' rather than three early returns. */
void func_b28_80171FD4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174540;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_801720EC);

/* State 0x17, built out of the banked room scripts' shared idioms rather
   than from one twin: the D_80083462 gate, then func_800A2B5C either
   side of func_800C7930(d - 0x20, b, 8, 0x300), then the pointer-local
   D_80174558 facing table published at c+0x2C and one func_80047784.

   THE ONE SPELLING FACT IS STATEMENT ORDER: `*(s32 *)(p + 0x8C) = 0' sits
   BETWEEN the p+0x9A and p+0x9B byte stores. Written after both of them --
   the order csrc/ovl_dng_b08.c uses -- the 0x8C word store and the 0x9B byte
   store come out transposed: 64 words, 2 differing.

   64 words, 0 differing.
   THIS COPY'S OWN CONSTANT: 0x17 -> 0x18.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b28_8017236C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    p[0x9A] = 0x18;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    t = D_80174558;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    d[0x6D] -= 1;
}

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b28_80170E5C. 31 words,
   0 differing, first compile. */
void func_b28_8017246C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b28_80170E5C;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's fall-and-land state machine, the relinked twin of
   the resident overlay's func_800D991C (csrc/ovl_dungeon_800D50FC.c). THE
   NEARER TWIN IS THE LANDED func_b10_80172C54 (csrc/ovl_dng_b10.c), which
   has already absorbed the relink: the interpolation's step counter at
   p + 0x9E with p + 0x96 left as the outer countdown the shared tail
   decrements, `-func_800644B8(step * 409)` as case 1's drift in place of the
   resident's b->unk_14 accumulator, func_80047784 against a bank-local table
   in place of func_8003DB94 against D_800E262C, and the d + 0x46 0x8000 arm
   as the `(v & 0x2000) != 0` side of what is three nested ifs in the
   resident. Four deltas from b10, all read off this listing: the drift
   accumulator is p + 0xA0, not p + 0xA4, and its shift is 9, not 10; case 0
   carries NO dress at all and its gate is the c + 0x4 / 0x1000 pair falling
   back on 0xE000 rather than b10's flat 0x6000 test; case 2's table is
   D_80174520; and there is no case 3. 218 words, 0 differing, first compile.

   The c + 0x4 comparison reuses the 2 the switch dispatch already has in
   $v0, which is why the gate reads `!= 2` against a register and not an
   immediate. func_800D991C's own header carries the two spelling facts
   reproduced here and not re-measured, since the first compile landed: the
   first numerator is THREE statements (`u = c[0x24] << 6;
   w = b + 0x2 - 0x20; u -= w;`) while the second stays inline, and the tail
   guard is nested ifs rather than an `&&` chain. */
void func_b28_801724E8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if (*(s8 *)(c + 0x4) != 2 || (*(u16 *)(c + 0x14) & 0x1000) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
            {
                break;
            }
        }
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 5;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0x9E) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x9E);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x9E);
            *(s32 *)(p + 0xA0) = -func_800644B8(*(s16 *)(p + 0x9E) * 409) << 9;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
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
            *(u8 **)(c + 0x2C) = D_80174520;
            func_80047784(c, D_80174520[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
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
void func_b28_80172850(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        }
    }
}

/* The actor's +0x9B three-state hand-off for the 0x6000 push: state 0 either
   arms c + 0x14's 0x6000 and jumps straight to state 2, or zeroes the mover
   and steps; state 1 rings func_800A56E0(0x808) on the c[4] == 7 / 0x1000
   pair and pushes through func_8009C12C on the c[4] == 9 / 0x1000 pair or any
   0xE000; state 2 is the teardown that re-installs func_b28_80170E5C.
   105 words, 0 differing, 0 aligned differences, first compile.

   NO LANDED WHOLE-BODY TWIN. dngtwin's two best carved siblings
   (func_b21_801752A0 and func_b21_801725B8 at 0.933) are BOTH STILL STUBS,
   and the 0.769 func_b28_80173CE0 in this unit is a stub as well. The body
   was read off the listing with the landed func_b45_801726C4
   (csrc/ovl_dng_b45.c, 0.716) open beside it for the idioms only: its state 0
   has the same `p[0x9B] = <terminal>; c + 0x14 |= 0x6000;
   func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);' arm and its state 1 the same
   `(c[4] == K && 0x1000) || 0x...' gate. b45 states no priced fact that
   applies here -- its one lever is a +0x6D arm order this body has not got,
   so that fact is NOT APPLICABLE.

   STATE 1 FALLS THROUGH INTO STATE 2 AND THE FALSE SIDE OF ITS GATE GOES TO
   THE FUNCTION END, NOT TO STATE 2's OWN 0xE000 TEST -- and a plain
   `if (gate) { ... }` with the fallthrough gets that for free: cc1psx's
   jump.c threads the gate's false branch past state 2's identical 0xE000
   test to the epilogue by itself. Spelling it as the compiler's own control
   flow instead -- the gate inverted with an explicit `break;' and the two
   statements unguarded -- also measures 105 words / 0 aligned differences,
   so the target does NOT constrain that choice; the plain `if' is kept
   because it is the readable one.

   MEASURED AND LOAD-BEARING (aligndiff, both spellings priced):

   | spelling                                        | words | aligned diffs |
   | as written below                                |  105  |      0        |
   | `*(s8 *)(c + 0x4)' hoisted into one local and   |  106  |     30        |
   |   named by both gates                           |       |               |
   | `*(u16 *)(d + 0x46) &= 0x7FFF' AFTER the        |  106  |      7        |
   |   func_800A4ACC call instead of before it       |       |               |
   | state 0's three mover stores ascending          |  105  |      4        |
   |   (0xC, 0x10, 0x14) instead of 0x14, 0x10, 0xC  |       |               |
   | state 0's `c + 0x14 |= 0x6000' BEFORE the       |  104  |     11        |
   |   `p[0x9B] = 2' store                           |       |               |

   THE TWO READS OF c + 0x4 ARE REAL, NOT A MISSING HOIST. func_800A56E0
   sits between them and a call is a cse barrier, so the second gate reloads;
   hoisting is the 30-row row above and a word long. (brief: "cse UNIFIES
   ONLY WITHIN A BASIC BLOCK AND ACROSS NO BARRIER".)

   MEASURED AND FREE at 105 words / 0 aligned differences, so it is NOT why
   this lands: the c[4] == 7 gate written as two nested `if's instead of
   `&&'. That is the narrowed fold_truthop rule -- it merges tests across
   `&&' only for two BIT TESTS ON ONE LOCAL, and these operands are a byte
   load and a halfword load of two different fields.

   The `*(u16 *)(d + 0x46) &= 0x7FFF' store lands in the func_800A4ACC call's
   delay slot and is written through $a0, which is `d': that is sched, not a
   second pointer in the source. */
void func_b28_801728B4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            p[0x9B] = 2;
            *(u16 *)(c + 0x14) |= 0x6000;
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        p[0x9B]++;
        return;
    case 1:
        if (*(s8 *)(c + 0x4) == 7 && (*(u16 *)(c + 0x14) & 0x1000) != 0)
        {
            func_800A56E0(0x808);
        }
        if ((*(s8 *)(c + 0x4) == 9 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            p[0x9B]++;
        }
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_800AD594(d, 0x100);
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A4ACC(d);
        return;
    }
}

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state
   1 waits on func_8003F270, state 2 runs the +0x96 countdown and the
   bank-local D_80174520 facing table, then re-installs func_b28_80170E5C.
   The relinked twin of func_b03_80172A24 (csrc/ovl_dng_b03.c), which is the
   same machine with 30 more words of tail; transcribed from that landed body
   with this bank's symbols and THIS COPY'S OWN CONSTANTS -- c[0x4] == 3 and
   a +0x96 reload of 1, where b03 has 4 and 3 -- and without b03's SVECTOR
   copy, its second c + 0x14 clear and its d + 0x6D guard. 286 words,
   0 differing, 0 aligned differences.

   THE TWO SIGN FIXES ARE `__builtin_abs', NOT `if (x < 0) x = -x;'. The
   opcodes are the same either way, but written as the two ifs the
   `sw $v0, 0x60($s1)' that stores the func_800A05A4 result loses its own word
   to the first `bgez's delay slot, and x then takes $a0 where the target has
   $v0: 285 words, 14 aligned differences. Nothing reaches it from the source
   order -- the store and the two `lb's are reg+const off the same base, so
   memrefs_conflict_p proves them independent and sched1 is free to sink the
   store; a pointer local for the store's base, the result in a local, a
   volatile store (286 words, 11) and reading d + 0x72 through a struct all
   measured 285/14 or worse.

   `k' MUST BE `s16', as b03 says: `s32' is 285 words and the missing word is
   the target's `addu $v0, $s5, $zero' ahead of the `beqz' (7 aligned
   differences here).

   THIS FUNCTION OWNS jtbl_b28_80170838, the 7-entry table at 0x80170850 for
   state 0's FIRST switch -- cases 1,2,3,5,6,7 over the 0x3FFF field with case
   4 falling to `default'. The second switch (the (d + 0x1C) & 0x2000 `else')
   has three cases and compiles to a compare chain. Its arms must be written
   3, 2, 1: ascending is 288 words / 10, because cross-jumping merges the
   table switch's 1/2/3 arms into these three and their emission order is what
   the table's `j's point at.

   The D_8006DE24 record is fine EITHER WAY here, unlike b03: the pointer
   local plus index local (`tbl'/`i'/`e') and a plain
   `D_8006DE24[g[0]].pad_11[1] == 2' in an `else if' both land at 286/0. */
void func_b28_80172A58(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            *(void **)(p + 0x8C) = func_b28_80170E5C;
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
            f = D_80174520;
            if (*(u8 **)(c + 0x2C) != f)
            {
                *(u8 **)(c + 0x2C) = f;
                func_80047784(c, f[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            g2 = (u8 *)&D_80083460;
            if (*(u32 *)(g2 + 0xC) == 0)
            {
                *(u16 *)(g2 + 0xA) -= 1;
                *(void **)(p + 0x8C) = func_b28_80170E5C;
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
   eases onto the tile centre and installs func_b28_80170E5C. The relinked
   twin of the landed func_b44_80173D88 (csrc/ovl_dng_b44.c), 188 words;
   transcribed from that body with this bank's D_80174550 and ONE statement
   REMOVED. 187 words, 0 differing, 0 aligned differences.

   THE ONE-WORD DELTA FROM THE TWIN IS A MISSING STATEMENT, NOT A COMPARE
   CONSTANT. dngtwin flags a 1-word shortfall as "most likely a COMPARE
   CONSTANT ... bgez/bltz/blez/bgtz/beqz/bnez are one word"; here it is
   state 0's c + 0x14 0x8000 arm, which in b44 is
   `*(s16 *)(p + 0x96) = 0; p[0x9B] = 2; return;' and here is just
   `p[0x9B] = 2; return;'. With b44's +0x96 store kept it is 188 words and
   5 ALIGNED differences, not the 1 the word count suggests -- the extra
   `sh $zero' displaces the `addiu $v0, 2' out of the `beqz' delay slot and
   both `j' offsets with it. So the flag's heuristic was wrong on this body
   and the brief's other rule ("A SHORTER BODY USUALLY MEANS MISSING
   STATEMENTS") was right.

   b44's four stated deltas-from-b19, all RE-PRICED here, all HELD:

   | b19's spelling instead                     | words | aligned diffs |
   | the seeds negated and scaled by 15         |  189  |      6        |
   |   (b44 measured 190 / 149 on its own body) |       |               |
   | state 2's two eases scaled by 15, not 16   |  187  |      2        |
   |   (b44: 2)                                 |       |               |
   | state 2's gate `!= 0', not `> 0' (`blez')  |  187  |      1        |
   |   (b44: 1)                                 |       |               |

   The fourth -- "the shared `stop:' has THREE reaching edges here, not two"
   -- is a SHAPE statement with no second spelling to price, and it is true
   of this body too: state 0's `d[0x28] == 0', state 1's `d[0x28] == 0' and
   state 2's `(c[0x14] & 0x6000) && d[0x28] == 0' all reach .Lb28_80173120.
   Note that b44's seed figure does NOT reproduce: 149 differences there,
   6 here, because its body has an arm past the seeds that this one lacks.

   THE FIRST NUMERATOR IS THREE STATEMENTS AND THE SECOND IS INLINE, which is
   b44's and b19's fact and HOLDS here, with both wrong spellings a word long:
     both written out as three statements      188 words,  14
     both inline                               188 words,   8

   MEASURED AND FREE at 187 words / 0 aligned differences, so neither is why
   this lands: the facing index written as a byte offset,
   `*(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(d + 0x6A) >> 8) & 0xE))', instead
   of `D_8006CCD8[(x >> 9) & 7]' -- combine produces the same `srl 8' /
   `andi 0xE' either way, so the asm is no evidence for the byte form; and
   `s16 t' instead of `s32 t'. */
void func_b28_80172ED0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            p[0x9B] = 2;
            return;
        }
        t = -1;
        if ((*(s32 *)(d + 0x1C) & 0x228) != 0)
        {
            t = 8;
        }
        *(s16 *)(p + 0x96) = t;
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
            func_800AAA54(p, b, c, D_80174550);
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
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b28_801731BC(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b28_801731DC(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b28_801731FC(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b28_8017321C(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* The banked room script's TWO-state tail, state index at p + 0x9B: state 0
   re-dresses the renderable from the bank-local D_80174538 facing table and
   steps the state, state 1 is the long refusal chain, and the shared
   D_80174520 re-dress hands the actor back to func_b28_80170E5C. The landed
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
void func_b28_80173254(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174538;
        func_80047784(c, D_80174538[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b28_801737DC(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174520;
        func_80047784(c, D_80174520[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        return;
    }
}

/* The banked room script's other two-state tail, state index at p + 0x9B.
   The landed func_b03_801736F4 (csrc/ovl_dng_b03.c) is the same machine with
   a third state; its comment carries the spellings that hold here unchanged
   -- the facing table named DIRECTLY at both dress sites rather than through
   a `u8 *` local, `*(s8 *)(d + 0x6D)` for the `lb`, `*(s16 *)(d + 0x64)` for
   the `lh`, and the `(s16)` cast on the first func_800A2C34 result.

   THE D_80174520 DRESS TAIL MUST BE WRITTEN OUT TWICE EVEN THOUGH THE TARGET
   CROSS-JUMPS IT INTO ONE. State 1 opens with the same d[0x25] test that
   closes it, and spelling the opening arm as `goto` to the single copy is
   152 words with 15 ALIGNED DIFFERENCES, every one of them `p` and `b`
   exchanged into $s2/$s3 -- this body is two references short of what
   allocno_compare needs to give `p` the lower number, and the duplicated
   `sw $v0, 0x8C(p)` supplies them. jump2 merges the copies back, so the
   duplication costs no word. Rejected with its score: a
   `do { func_800AA258(p, b, c, d); } while (0)` free-reference bump also
   measures 152 / 0, and so do the same wrapper on the func_800AA888 and
   func_b28_801737DC calls, but the duplicated tail is plain C that the twin
   already uses, so no device is recorded here. `do { }` on p[0x9B] += 1,
   on the func_800A9A0C / func_800A9A04 calls or on the p + 0x8C store is
   15; on `h = (u8 *)&D_80083460` 26; on either `c + 0x2C` store 136 and a
   word long.

   Three deltas from b03: the 0x1000 gate reads the D_80083462 SYMBOL
   directly, not `*(u16 *)(g + 0x2)` off `&D_80083460`; the tail installs
   p + 0x8C before clearing d + 0x1C's 0x200; and state 0 decrements
   D_80083460 + 0xA before the dress, not after. 152 words, 0 differing. */
void func_b28_8017357C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174538;
        func_80047784(c, D_80174538[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174520;
            func_80047784(c, D_80174520[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
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
            func_b28_801737DC(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174520;
        func_80047784(c, D_80174520[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
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
   call emits the bare `jal` either way); hoisting D_80174518 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b28_801737DC(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174538 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80174518;
        func_80047784(c, D_80174518[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b28_801738A4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        }
    }
}

/* 14 words, 0 differing. */
void func_b28_80173908(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* 14 words, 0 differing. */
void func_b28_80173940(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* 14 words, 0 differing. */
void func_b28_80173978(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* 14 words, 0 differing. */
void func_b28_801739B0(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* 14 words, 0 differing. */
void func_b28_801739E8(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b28_80170E5C;
    }
}

/* The actor's +0x9B three-state hand-off for the 0x80C cue. State 0 either
   arms c + 0x14's 0x6000 and jumps to state 2 or steps to state 1; state 1,
   on the `(c[4] == 8 && 0x1000) || 0xE000' gate, shows the bank-local
   message, rings func_800A56E0(0x80C), runs func_b28_80174084 and parks
   func_b28_801742AC's result in p + 0xA4; state 2 raises 0x8000 on that
   record's +0x1E and in D_800814A0, spawns a func_800D3F74 node on the cell
   the D_8006CCD8/D_8006CCE8 facing pair picks, dresses it through
   func_80042640 and a clamped +0x28/+0x29 pair, and hands the actor back to
   func_b28_80170E5C. 176 words, 0 differing, 0 aligned differences. Owns no
   jump table: the three-state dispatch is a compare chain and the listing has
   no `.section .rodata'. No twin at any ratio; this body came from the park
   docs/dng-parks/func_b28_80173A20.c, which reached 2 aligned differences.

   THE `u1' BASE HOIST IS THE WHOLE LANDING, AND IT IS A sched LUID FACT, NOT
   AN ALLOCATION ONE. Written the natural way, `t1 = &D_8006CCD8[k]', the body
   is 176 words with ONE row wrong: `andi $v1, $v1, 0xE' comes out three words
   early, before the D_800814A0 store instead of after the table's
   `lui'/`addiu' (2 aligned differences, measured on this body). The cause is
   visible in a `-da' dump pair. In sched2's block 14 the ready list reaches
   `ready list at T-26: 379 (1) 170 (4) 172 (4)' -- the `andi' and the table's
   `lo_sum' tied at priority 4 -- and `rank_for_schedule' (sched.c) then falls
   past the priority test and past the class test (both are class 3: the
   `addsi3' that consumes them costs 1) to `INSN_LUID (tmp) - INSN_LUID
   (tmp2)', which is source order. A SYMBOL_REF costs no insn at expand time:
   `expand_expr's PLUS arm takes `goto binop', `op0' is the array's ADDR_EXPR
   and comes back as a bare `symbol_ref' with nothing emitted, so it is
   `expand_binop' that forces it into a register AFTER the index operand is
   evaluated. The `andi' therefore always gets the LOWER LUID and always wins
   the tie. Assigning the base to a pointer local first makes the `high'/
   `lo_sum' pair real insns at the assignment, the `andi' becomes the later
   LUID, and the tie goes the other way: 0 rows.

   MEASURED ON THIS BODY, all with aligndiff and all from the landing (so
   these are prices, not improvements):

   | spelling                                        | words | aligned diffs |
   | as written below                                |  176  |      0        |
   | `t1 = &D_8006CCD8[k]' with no `u1' at all       |  176  |      2        |
   | `u1 = D_8006CCD8' moved ABOVE the split store   |  176  |      0        |
   |   -- so the hoist's slot between the two        |       |               |
   |   statements is NOT MEASURABLE; what is         |       |               |
   |   measurable is that it must come AFTER `k'.    |       |               |
   | `u1 = D_8006CCD8' moved ABOVE `k'              |  174  |     24        |
   |   (then the `srl' moves below the store too)    |       |               |
   | `s32 r' for the clamp instead of `u8 r'         |  175  |     13        |
   | `D_800814A0 |= 0x8000' unsplit                  |  176  |      2        |
   | func_800D3F74's first argument `!= 0' not `> 0' |  176  |      2        |
   | the two func_800BCB04 arguments inline instead  |  176  |     18        |
   |   of through `x1'/`x2'                          |       |               |
   | no `hx', the d + 0x88 read inside the `h =' arm |  176  |      4        |
   | array-ref derefs (`t1[0]'/`t2[0]') at all four  |  176  |      2        |
   |   table reads, which is the MEM_IN_STRUCT_P     |       |               |
   |   route the park named as its next idea: dead.  |       |               |

   AND ONE OF THE PARK'S FACTS IS NOW FREE. It priced `s16 *t1' DECLARED
   BEFORE `s16 *t2' at 177 words / 39-42 aligned differences and called it the
   reason t1 gets $s4. From the landing the two declaration orders both
   measure 176 / 0: the park's figure was taken with the sched fault live and
   the two faults were adding. The declaration order below is the park's and
   is kept only because it is what landed.
*/
void func_b28_80173A20(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *q;
    s16 *t2;
    s16 *t1;
    s16 *u1;
    s32 h;
    s32 t;
    s32 k;
    s32 hx;
    s32 w;
    u8 r;
    s32 x3;
    s32 x1;
    s32 x2;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x6000;
            p[0x9B] = 2;
            return;
        }
        p[0x9B] = 1;
    case 1:
        if ((*(s8 *)(c + 0x4) == 8 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_b28_80174460(d);
            func_800A56E0(0x80C);
            func_b28_80174084(p, b, c);
            *(u8 **)(p + 0xA4) = func_b28_801742AC(p, b, c);
            p[0x9B] += 1;
        }
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        e = *(u8 **)(p + 0xA4);
        *(u16 *)(e + 0x1E) |= 0x8000;
        w = D_800814A0;
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        x3 = *(u16 *)(d + 0x88);
        D_800814A0 = w | 0x8000;
        u1 = D_8006CCD8;
        t1 = &u1[k];
        x1 = ((c[0x24] + *t1) << 6) & 0xFFC0;
        t2 = &D_8006CCE8[k];
        x2 = ((c[0x25] + *t2) << 6) & 0xFFC0;
        h = func_800BCB04(x1, x2, x3 - 0x20);
        hx = *(u16 *)(d + 0x88);
        if (*(s16 *)(d + 0x88) < (s16)h)
        {
            h = hx;
        }
        q = func_800D3F74((*(s32 *)(d + 0x1C) & 0x2000) > 0, c[0x24] + *t1, c[0x25] + *t2, h);
        func_80042640((Unk42710 *)q, 0x36);
        t = q[0x29] + (d[0x11] * 2 + (func_800A6D30() & 3));
        r = t;
        if (t >= 0x100)
        {
            r = 0xFF;
        }
        q[0x28] = r;
        q[0x29] = r;
        func_800AD594(d, 0x400);
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        return;
    }
}

/* The same +0x9B three-state hand-off as func_b28_80173A20 above, with state
   2's whole spawn block deleted: no e = p + 0xA4 record, no D_800814A0 raise,
   no facing-table lookup, no func_800D3F74 node and no clamp -- just the
   shared teardown. State 1 shows this bank's other message through
   func_b28_801744B8 rather than func_b28_80174460 and does not park a
   func_b28_801742AC result. Written off func_b28_80173A20 in this same file;
   dngtwin's 0.708 func_b28_801728B4 was not needed and was not read.
   90 words, 0 differing, 0 aligned differences, FIRST COMPILE.

   Three spellings priced from the landing, since a first-compile body has
   measured nothing:
     `p[0x9B] += 1' in state 1 is load-bearing -- `p[0x9B] = 2' is 86 words
       and 10 aligned differences, because the constant store loses the
       `lbu'/`addiu'/`sb' and lets jump2 fold the arm's tail.
     `*(s8 *)(c + 0x4) == 8' is worth 1 row: `c[0x4] == 8' gives `lbu' where
       the target has `lb', and nothing else moves.
     FREE: writing the two flag tests bare (`& 0x1000' instead of
       `(& 0x1000) != 0') measures 90 / 0 either way. Both operands here are
       different symbols, so fold_truthop has nothing to merge.
*/
void func_b28_80173CE0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x6000;
            p[0x9B] = 2;
            return;
        }
        p[0x9B] = 1;
    case 1:
        if ((*(s8 *)(c + 0x4) == 8 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_b28_801744B8(d);
            func_800A56E0(0x80C);
            func_b28_80174084(p, b, c);
            p[0x9B] += 1;
        }
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        func_800AD594(d, 0x400);
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        return;
    }
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
s32 func_b28_80173E48(u8 *a, void *b, void *c, s32 d)
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
        func_b28_80171420(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b28_80171420(p, b, c, p);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80173F8C);

/* The bank's 0x212 spark burst, called out of state 1 of both +0x9B
   hand-offs (func_b28_80173A20 and func_b28_80173CE0): project the caller's
   own slot once through func_8003DE58 into a zeroed SVECTOR, then spawn FOUR
   nodes driven by func_b28_80173F8C, each with a 0x60/0x60/0x60 colour, a
   random 0..0xFFF spin in +0x1A, a 0x1000/0x1000 scale, dressed from
   D_800DEA68 and D_80045340, and each node's mover seeded with a whole copy
   of the caller's UnkVec plus the projection's three deltas and a fresh
   +0xC/+0x10 velocity off p + 0x2A's angle with a 0x9C40 +0x14.
   116 words, 0 differing, 0 aligned differences.

   Written off csrc/ovl_dng_s13.c's func_s13_80024628, which dngcallee names
   at 7 of 10 symbols and which is the same 0x212 skeleton (same
   func_8003FC64(0x212), same `u8 *r = (u8 *)n->unk_0C' plain-cast dress
   block, same func_8004491C(n, (u32)D_80045340) tail), plus the two facts in
   csrc/ovl_dng_b16.c's own 0x212 spawner comment. Both of b16's facts HELD
   here. Priced on this body, every figure taken from the landing:

   | spelling                                              | words | aligned |
   | as written below                                       |  116  |    0    |
   | func_8003DE58's fourth argument as `i' with an         |  117  |    2    |
   |   `i = 0;' statement before the call instead of the    |       |         |
   |   literal 0 with the `for' owning the init             |       |         |
   | no `p = a' alias -- parameter 0 used directly          |  116  |    5    |
   | the three SVECTOR zero stores ASCENDING vx, vy, vz     |  116  |    4    |
   | the UnkVec seeded with six word assignments instead    |  122  |   36    |
   |   of `*m = *b'                                         |       |         |
   | `rand() & 0xFFF' instead of `rand() % 0x1000'          |  111  |    9    |

   THE FOURTH ARGUMENT IS THE LITERAL 0 AND THE LISTING STILL SHOWS
   `addu $a3, $s2, $zero'. That copy is NOT evidence the source passed the
   counter: the `for' init sets a pseudo to 0 in the same basic block, sched
   lifts it above the `jal', and cse then forwards that known-zero register
   into the constant argument. Writing the counter there really does cost a
   word (the `i = 0' becomes its own insn), so a register copy from a
   loop counter in an argument slot is worth checking against a literal.

   `p = a' IS b16's PARAMETER-0 FACT and it reproduces: the target's four
   leading copies run $s5 (b), $a1 (c), $s2, $s3 (p) -- parameter 0 LAST --
   and only a body-level copy moves it there, because combine folds the
   prologue copy into the user insn, which sched then ranks after the three
   argument moves.
*/
void func_b28_80174084(u8 *a, UnkVec *b, u8 *c)
{
    u8 *p;
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    s32 i;
    SVECTOR out;

    p = a;
    out.vz = 0;
    out.vy = 0;
    out.vx = 0;
    func_8003DE58(*(u8 **)(c + 0x8), c, &out, 0);
    for (i = 0; i < 4; i++)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            r = (u8 *)n->unk_0C;
            n->unk_10 = (u32)func_b28_80173F8C;
            r[0xE] = 0x60;
            r[0xD] = 0x60;
            r[0xC] = 0x60;
            *(u16 *)(r + 0x14) |= 0xC;
            *(u16 *)(r + 0x10) |= 0x20;
            func_8003DB94(r, D_800DEA68, 0);
            func_800478B8(r);
            *(s16 *)(r + 0x1A) = rand() % 0x1000;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *m = *b;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + (u16)out.vx;
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + (u16)out.vy;
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + (u16)out.vz;
            m->unk_0C = (func_80064584(*(s16 *)(p + 0x2A)) >> 4) << 11;
            m->unk_10 = (func_800644B8(*(s16 *)(p + 0x2A)) >> 4) << 11;
            m->unk_14 = 0x9C40;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80174254);

/* The 0x212 node func_b28_80173A20's state 1 parks in p + 0xA4: allocate it,
   install func_b28_80174254 as its +0x10 step, seed the payload's +0x00 with
   0 and its +0x02 with the actor's facing index out of p + 0x2A, dress the
   slot 0x80/0x80/0x80 with a 0x7E40 +0x12, the 0x100 flag, D_800DEEC0 and
   D_80045340 and a 0x800/0x800 scale, then copy the caller's whole UnkVec
   into the node's mover and overwrite its +0x2/+0x6 with the facing pair's
   cell centre plus 0x20 and clamp its +0x8 down to func_800BCB04's floor
   shifted up 16. Returns the node, or NULL. 109 words, 0 differing, 0
   aligned differences.

   Same donor as func_b28_80174084 above -- csrc/ovl_dng_s13.c's
   func_s13_80024628, which dngcallee names at 7 of 9 symbols -- with the
   facing-table geometry taken off func_b28_80173A20 in this same file.
   Priced on this body, all figures from the landing:

   | spelling                                              | words | aligned |
   | as written below                                       |  109  |    0    |
   | BOTH stores before BOTH table reads (x, store, y,      |  110  |   16    |
   |   store) instead of x, y, store, store                 |       |         |
   | `if (n == NULL) return NULL;' as an early exit rather  |  109  |    6    |
   |   than the `if (n != NULL) { ... return n; } return    |       |         |
   |   NULL;' nesting                                       |       |         |
   | no `(s16)' cast on the two c[]+table sums              |  107  |    7    |
   | the three colour bytes ASCENDING 0xC, 0xD, 0xE         |  109  |    4    |
   | the UnkVec seeded with six word assignments instead    |  115  |   26    |
   |   of `*m = *b'                                         |       |         |
   | FREE: the `k' local dropped and `*(s16 *)(v + 0x2)'    |  109  |    0    |
   |   written inline in both array refs. With both reads   |       |         |
   |   above both stores cse unifies the reload either way;  |       |         |
   |   it is only load-bearing in the interleaved order.    |       |         |

   THE ORDER FACT IS A MEMORY-DEPENDENCE FACT, NOT AN ALLOCATION ONE. The
   target loads `lbu $a1, 0x25($s3)' at word 68, ABOVE the `sh $v1, 0x2($s0)'
   at 78. A c[] load cannot be hoisted over a store through `m', so the only
   source that can emit it there is one that reads c[0x25] before the store.
   Interleaving the stores costs the extra word as well -- our word 68 comes
   out a `nop' the target fills with that load.

   `(s16)' ON THE SUM IS WHAT MAKES `sll 16' / `sra 10' OUT OF A `<< 6', and
   it is also why both table loads are `lhu' where the signed `s16
   D_8006CCD8[]' would give `lh': the 16-bit truncation makes the high bits
   dead and combine takes the unsigned load. Without the cast the two
   `sll 16' go away, which is the two missing words.
*/
u8 *func_b28_801742AC(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *v;
    s32 x;
    s32 y;
    s32 h;
    s32 k;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_b28_80174254;
        *(s16 *)v = 0;
        *(s16 *)(v + 0x2) = (*(u16 *)(p + 0x2A) >> 9) & 7;
        r = (u8 *)n->unk_0C;
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x12) = 0x7E40;
        *(u16 *)(r + 0x14) |= 0x100;
        func_8003DB94(r, D_800DEEC0, 0);
        func_800478B8(r);
        *(s16 *)(r + 0x1E) = 0x800;
        *(s16 *)(r + 0x1C) = 0x800;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *m = *b;
        k = *(s16 *)(v + 0x2);
        x = (s16)(c[0x24] + D_8006CCD8[k]) << 6;
        y = (s16)(c[0x25] + D_8006CCE8[k]) << 6;
        *(s16 *)((u8 *)m + 0x2) = x + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y + 0x20;
        h = func_800BCB04(x & 0xFFC0, y & 0xFFC0, *(u16 *)(p + 0x88) - 0x20) << 16;
        if (h < m->unk_08)
        {
            m->unk_08 = h;
        }
        return (u8 *)n;
    }
    return NULL;
}

/* The resident overlay's func_80099844 (csrc/ovl_dungeon.c) with the message
   string a bank-local constant instead of a parameter, so the second saved
   register it needs is gone: 22 words where the twin is 24. 0 differing. */
void func_b28_80174460(u8 *p)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099734(p, h);
    h = func_80099194(D_801745CC, h);
    func_80099290(h);
    func_800A5720(m);
}

/* func_b28_80174460 (above) again with its own string. 22 words, 0 differing. */
void func_b28_801744B8(u8 *p)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099734(p, h);
    h = func_80099194(D_801745F8, h);
    func_80099290(h);
    func_800A5720(m);
}
