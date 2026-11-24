#include "common.h"

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b14_80171D9C(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80174F28[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80174EE8[];
extern u8 D_80174F00[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b14_80171410(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b14_80170E9C();

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
s32 func_b14_80171E00(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u8 D_80082E80[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

s32 func_800AB464();

extern u8 D_80174EF0[];

extern s16 D_800DCF5E;

extern s32 D_80174FD0;
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
void func_b14_80173B4C(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

UnkNode *func_8003FD64(s32, void *);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b14_80170A98(u8 *p, UnkVec *b, u8 *c);
extern u8 D_80083498[];
extern u8 D_80045340[];
extern u8 D_b14_80174ED8[];
s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
void func_800BB044(u8 *);
s32 func_800A56E0(s32);
void func_800A2B04(UnkVec *, s32, s32);
extern s32 D_8008346C;
extern u8 D_80174EE0[];

/* Spawn the 0x112 node this bank's actors live in: allocate it off
   D_80083498, dress it with D_80045340, put the caller's facing in the
   mover's +0xA and the tile pair in the slot's +0x24/+0x25, then pick the
   record's flag set from the low two bits of the first argument -- 1 raises
   0x6000 on +0x14/+0x1C, >=2 raises 0x2000, and 0 with no high bits left is
   the only arm that may roll func_800A6D30 for the D_80174F00 skin -- and
   hand the result to func_800A9C18 and func_800AA36C. The relinked twin of
   func_b03_80170854 (csrc/ovl_dng_b03.c), with this bank's symbols, p[0x13]
   = 0xE in place of 3, no `*(u32 *)(p + 0x1C) |= 0x200' in the roll arm, and
   the +0x8C handler written per arm rather than once in the tail. 129 words,
   0 differing, 0 aligned differences.

   THE +0x2C FACING TABLE IS STORED IN ALL FOUR ARMS, NOT ONCE AFTER THE
   CHAIN -- which makes it a dead store, since the roll arm's D_80174F00 is
   overwritten by it. Written once after the chain there is ONE
   `lui'/`addiu' pair for D_b14_80174ED8 and the store sinks past the
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
   block and the two-bit dispatch is a compare chain, not a table. */
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80174F18[];

extern u8 D_8006CCF8[];
extern void (*D_80174F4C[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

void func_b14_80171410(u8 *p, void *b, void *c, u8 *d);
s32 func_800644B8(s32);
void func_800A9A04(u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b14_801737C4(u8 *p, u8 *b, u8 *c, u8 *d);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
extern u8 D_80174EF8[];
extern u8 D_80174F10[];
extern u8 D_80174F08[];
void func_b14_80171B58(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b14_80171FC4(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_b14_8017414C(u8 *, UnkVec *, u8 *);

extern Unk41284 D_80174FAC[];
extern u8 D_80174FCC[];
extern u8 D_80174FCD[];
extern s16 D_80174F48;
extern s32 D_80174FB4[];
extern s32 D_8006CD58;
extern u8 *D_800DCEEC[];
extern u8 D_8003E140[];
extern u8 D_800DCF5C;
extern u8 D_8014A000[];
void func_8003DB94(void *, void *, s32);
void func_80041588(Unk41284 *, s8 *, s32);
u32 func_800445E0(void);
s32 func_800498A0(u8 *);
s32 func_800C77D0(void *, u8 *, s16, s32);
void func_8009A028(u8 *);
void func_800A18E8(s16, s16);
s32 func_800A504C(u8 *, u8 *);
UnkNode *func_800ADC4C(u8 *, s32, s16, u8 *);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void func_b14_801745E0(u8 *p, UnkVec *b, u8 *c);
void func_b14_8017487C(u8 *p, UnkVec *b, u8 *c, u8 *d);

u8 *func_b14_80170894(s16 a, u16 b, u16 c, s16 d)
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
        p[0x13] = 0xE;
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
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
            *(u8 **)((u8 *)r + 0x2C) = D_b14_80174ED8;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
            *(u8 **)((u8 *)r + 0x2C) = D_b14_80174ED8;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80174F00;
                }
            }
            *(u32 *)(w + 0x8C) = (u32)func_b14_80170E9C;
            *(u8 **)((u8 *)r + 0x2C) = D_b14_80174ED8;
        }
        else
        {
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
            *(u8 **)((u8 *)r + 0x2C) = D_b14_80174ED8;
        }
        n->unk_10 = (u32)func_b14_80170A98;
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
   the +0x9A entry of this bank's D_80174F4C handler table, re-aims the tile
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
   * THE +0x8C HANDLER CALL, THE D_80174F4C TABLE CALL AND func_800AA36C
     PASS `q`, NOT `p`, as the fourth argument, and the facing test reads
     `q + 0x6D`. Spelled `p` the dir test alone is one difference, an `lb`
     off $s2 where the target has $s1.
   * The gate compares the +0x8C handler against func_b14_80170E9C.
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
void func_b14_80170A98(u8 *p, UnkVec *b, u8 *c)
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
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b14_80170E9C)
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
    D_80174F4C[p[0x9A]](p, b, c, q);
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

/* The +0x8C handler at the head of this bank's actor machine, and the
   function the other three bodies in this unit re-install at p + 0x8C -- the
   relinked twin of the landed func_b12_80170E70 (csrc/ovl_dng_b12.c), itself
   the bank-local form of the resident overlay's func_800D8C64
   (csrc/ovl_dungeon_800D50FC.c). 349 words, 0 differing, first compile.

   WHICH TWIN: b12, b18 and b20 all score 0.963 at 345 words and b45 0.952 at
   338, and ALL FIVE have this one's exact prologue (-0x38, s1/s3/s0/s2/ra/s4
   in that order), so the prologue does not choose here -- it only rules
   nothing out. b12 and b20's listings are identical to each other; b12 was
   taken because its comment carries the priced fact table. Normalised
   listing diffs: b20 40 rows, b12 40, b18 48, b45 55.

   THIS FUNCTION OWNS jtbl_b14_80170808, the 12-entry table at rodata offset
   0x0 (8-aligned), for the switch over (d + 0x46) & 0x3FFF. Its entries, in
   order, are the labels for cases 1..12:
     1, 2, 3   -> .Lb14_80171290, the func_800AAF00 arm
     4         -> .Lb14_801712BC, which IS the `default' block (the walk)
     5, 6, 7   -> .Lb14_8017123C, the func_800A0818 / D_800814A8 arm
     8, 9      -> .Lb14_80171204, the func_b14_80171E00 arm
     10, 11    -> .Lb14_801712BC, `default' again
     12        -> .Lb14_80171280, func_800A9A0C, written as a `case' label
                  inside the `w[0x9A] != 0x11' arm, where cross-jumping also
                  merges it with the d + 0x1C & 0x20 early exit
   So cases 4, 10 and 11 need no label of their own, exactly as in b12.
   Nothing here can measure where the table LANDS -- asmlib.compile_asm does
   not expand INCLUDE_ASM, so the still-stubbed tables that interleave are
   absent from a standalone compile; that is the build gate's question.

   THE RELINK DELTA FROM b12, read off the listing. The eight bank-local
   table sites were checked ONE AT A TIME against the listing addresses,
   because `ovlverify' masks a relocated field and CANNOT see which symbol it
   names: spelling D_80174F10 as D_80174EE0 measures 349 words / 0 differing
   here, so a clean run proves nothing about table identity.
     80170F14  D_80174F00  the d[0x25]==0 arm's comparand
     80170F20  D_80174EF8  what that arm stores and dresses from
     80170F3C  D_80174F00  the 0x200 arm's comparand
     80170F94  D_80174EF8  func_800AA924's fourth argument
     80170FF4  D_80174EE0  the state-0xE dress
     80171064  D_80174F10  func_800AA6B4's fourth argument
     801712A8  D_80174F08  func_800AAF00's fourth argument
     801713A4  D_80174EE0  the final dress
   and the eight callees b12 spells for its own bank become
   func_b14_80171B58, func_b14_801737C4, func_b14_801747D0,
   func_b14_80173A08, func_b14_80171E00, func_b14_80171FC4 and
   func_b14_80171410, with func_b14_80170E9C itself passed to func_800AAF00.

   TWO STATEMENTS THIS COPY HAS AND b12 DOES NOT, and both are load-bearing:
   | spelling                                      | words | aligned diff |
   | 0x200 arm ends `c[0xC]=c[0xD]=c[0xE]=0x40;'   |  349  |      0       |
   | the same three as separate 0xE, 0xD, 0xC      |  349  |      0       |
   | the same three as separate 0xC, 0xD, 0xE      |  349  |      2       |
   | the func_800AA924 arm sets d+0x1C's 0x10000000 |      |              |
   |   and then returns UNCONDITIONALLY            |  349  |      0       |
   | ... returning only on the nonzero, as b12     |  350  |     21       |
   The chained assignment and the explicit 0xE/0xD/0xC order are the same
   thing to the compiler; 0xC-first is not.

   B12'S THREE STATED FACTS: one held, two are FREE here. All three are
   SELECTION facts, which is the class that never inverts -- and indeed none
   inverted; two simply stopped mattering.
   | spelling                                      | words | aligned diff |
   | `w' (D_800814A8) assigned BEFORE the d+0x2A   |      |              |
   |   store, so the `lw' delay is filled by it    |  349  |      0       |
   | ... assigned after it (b12's rejected form)   |  350  |     16       |
   | ONE table local per dress site (e/e2/e3)      |  349  |      0       |
   | one local SHARED by all three                 |  349  |      2       |
   | no local at all, symbol named directly        |  349  |      0       |
   | the walk tail written out at all three sites  |  349  |      0       |
   | the three sites `goto'-ed to one copy         |  349  |      0       |

   AND THE TWO HOISTS THE LISTING LOOKS LIKE IT ASKS FOR ARE FREE, so do not
   spend anything on them. The target reads D_80083462 once into $a0 and
   d + 0x1C once into $v1 and reuses each for its second test, where b12's
   listing reloads both; written either way this body is 349 words and
   0 aligned differences, because cse does it. The locals `f' and `u' are
   kept only because they say what the asm does.

   `--raw' is 79 differing and all 79 are relocations -- 23 `jal's, 10 `j's,
   25 `%hi' and 21 `%lo' halves -- so no masked word hides a constant
   address. (The target's `%hi' count exceeds its `%lo' count because
   D_80083462, D_80083228, D_80082EA4 and D_80082E80 share the 0x8008 page,
   so a cross-jumped `lui' serves more than one `%lo' and splat names it
   after whichever it reached.) */
void func_b14_80170E9C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    u16 f;
    s32 u;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;

    f = D_80083462;
    if ((f & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b14_80171B58(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174F00)
        {
            e = D_80174EF8;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    u = *(s32 *)(d + 0x1C);
    if ((u & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174F00)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            c[0xC] = c[0xD] = c[0xE] = 0x40;
            return;
        }
        if (func_800AA924(p, b, c, D_80174EF8) != 0)
        {
            *(s32 *)(d + 0x1C) |= 0x10000000;
        }
        return;
    }
    if ((f & 0x2000) == 0)
    {
        if ((u & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            e2 = D_80174EE0;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174F10) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b14_801737C4(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0 && (s16)func_800AAB10(p, b, c, d) != 0)
        {
            func_b14_801747D0(p, b, c, d);
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
            func_b14_80171410(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b14_80173A08(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b14_80171410(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b14_80171E00(p, b, c, d) != 0)
            {
                return;
            }
            func_b14_80171FC4(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80174F08, func_b14_80170E9C);
            return;
        default:
            func_b14_80171410(p, b, c, d);
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
    e3 = D_80174EE0;
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
   func_8009A8C0, func_b14_80171E00 for func_b01_8017237C -- and ONE
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
   BE DECLARED AND PASSED ON: func_b14_80171E00's call sets only $a0, $a2
   and $a3, so its $a1 is this function's own incoming $a1 reaching it
   untouched. */
void func_b14_80171410(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b14_80171E00(p, b, c, d) == 0)
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
   the actor one cell of its queued path, swap in the D_80174EE8 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b14_80171D9C.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b14_80171D9C is called only on
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
void func_b14_80171B58(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80174EE8)
        {
            *(u8 **)(c + 0x2C) = D_80174EE8;
            func_80047784(c, D_80174EE8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b14_80171D9C(p, b, c, d);
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
   out of the bank-local byte table D_80174EF0: the relinked twin of the
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
     the local assigned inside the store, `= t = D_80174EF0'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80174EF0 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b14_80171D9C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80174EF0;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174F28 facing table and re-aim through func_8009C93C.

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
s32 func_b14_80171E00(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174F28;
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
   bank's D_80174F28 facing table and calls func_8009C93C. 70 words,
   0 differing, first compile.

   `*(s32 *)(p + 0x8C) = 0' GOES BETWEEN THE TWO +0x9A/+0x9B BYTES, which is
   not the order func_b14_80171E00 uses. Written 0x9A, 0x9B, 0x8C the two `sb's
   pair up and the `sw' sinks past them -- 70 words, 2 aligned differences.

   Two spellings that do NOT matter here, both 70/0: the gate through
   `g = (u8 *)&D_80083460' + 0x2 (the unit's usual idiom) rather than
   D_80083462 as its own symbol, and the three gates as one `&&' chain with an
   `else return' rather than three early returns. */
void func_b14_80171FC4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80174F28;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
}

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b14_80170E9C. 31 words,
   0 differing, first compile.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x3.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b14_801720DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 3);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b14_80170E9C;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's fall-and-land state machine, the relinked twin of
   the landed func_b28_801724E8 (csrc/ovl_dng_b28.c), which is itself the
   bank-local form of the resident overlay's func_800D991C
   (csrc/ovl_dungeon_800D50FC.c). b28 is the right twin and not b35/b36/b40:
   all four score 0.943-1.000, but b28's prologue is byte-identical to this
   one -- -0x38 with s0, s1, s2, s3, ra saved IN THAT ORDER -- while the
   other three save s2 and s3 BEFORE s1. Against b28 the whole delta is six
   operands, read off this listing: case 0's gate is `c[0x4] != 1' not
   `!= 2', the step counter at p + 0x9E loads 6 not 5, case 1's drift angle
   is `step * 341' not `step * 409' (the shift chain is 2/+1/4/+/2/+1, so
   5 * 17 * 4 + 1 = 341), case 2's facing table is D_80174EE0, func_800AD594
   is handed 3 not 4, and the re-installed handler is func_b14_80170E9C.
   218 words, 0 differing, first compile.

   `c[0x4] != 1' IS WHAT PRODUCES THE REGISTER SHUFFLE the diff against b28
   shows, and it is not a free choice. b28 compares c[0x4] against the 2 the
   switch dispatch leaves in $v0, so its gate reads against a register; here
   the constant the gate wants is the 1 the `case 1' test already needed, so
   gcc keeps that 1 in $a0 across the dispatch and the case-0 load lands in
   $v0 instead of $v1. Spelled `!= 2' this body is 218 words and 4 aligned
   differences -- the two dispatch registers and the two gate registers --
   with every other word already correct.

   b28's two inherited spelling facts both HELD, which is what the taxonomy
   predicts: both are SELECTION facts and no inversion has been seen in that
   class. b28 priced neither, so these numbers are measured here.
   | spelling                                   | words | aligned diff |
   | first numerator in three statements        |  218  |      0       |
   | both numerators inline                     |  219  |      7       |
   | tail guard as nested ifs                   |  218  |      0       |
   | tail guard's inner two as an `&&' chain    |  218  |     10       |

   AND TWO SPELLINGS I EXPECTED TO BE FREE ARE NOT, so do not tidy them:
   | spelling                                   | words | aligned diff |
   | clears 0x14, 0x10, 0x0C (both sites)       |  218  |      0       |
   | case 2's three clears 0x0C, 0x10, 0x14     |  218  |      4       |
   | tail's three clears 0x0C, 0x10, 0x14       |  218  |      4       |
   | `g = (u8 *)&D_80083460' hoisted            |  218  |      0       |
   | two `(u8 *)&D_80083460' references instead |  217  |      5       | */
void func_b14_80172158(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        if (*(s8 *)(c + 0x4) != 1 || (*(u16 *)(c + 0x14) & 0x1000) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
            {
                break;
            }
        }
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 6;
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
            *(s32 *)(p + 0xA0) = -func_800644B8(*(s16 *)(p + 0x9E) * 341) << 9;
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
            *(u8 **)(c + 0x2C) = D_80174EE0;
            func_80047784(c, D_80174EE0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
    func_800AD594(d, 3);
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
        *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
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
void func_b14_801724C0(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b14", func_b14_80172524);

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0, state
   1 waits on func_8003F270, state 2 runs the +0x96 countdown and the
   bank-local D_80174EE0 facing table, then re-installs func_b14_80170E9C.
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

   THIS FUNCTION OWNS jtbl_b14_80170850, the 7-entry table at 0x80170850 for
   state 0's FIRST switch -- cases 1,2,3,5,6,7 over the 0x3FFF field with case
   4 falling to `default'. The second switch (the (d + 0x1C) & 0x2000 `else')
   has three cases and compiles to a compare chain. Its arms must be written
   3, 2, 1: ascending is 288 words / 10, because cross-jumping merges the
   table switch's 1/2/3 arms into these three and their emission order is what
   the table's `j's point at.

   The D_8006DE24 record is fine EITHER WAY here, unlike b03: the pointer
   local plus index local (`tbl'/`i'/`e') and a plain
   `D_8006DE24[g[0]].pad_11[1] == 2' in an `else if' both land at 286/0. */
void func_b14_8017293C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            *(void **)(p + 0x8C) = func_b14_80170E9C;
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
            f = D_80174EE0;
            if (*(u8 **)(c + 0x2C) != f)
            {
                *(u8 **)(c + 0x2C) = f;
                func_80047784(c, f[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            g2 = (u8 *)&D_80083460;
            if (*(u32 *)(g2 + 0xC) == 0)
            {
                *(u16 *)(g2 + 0xA) -= 1;
                *(void **)(p + 0x8C) = func_b14_80170E9C;
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
   eases onto the tile centre and installs func_b14_80170E9C. The landed
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
void func_b14_80172DB4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800AAA54(p, b, c, D_80174F18);
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
        *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b14_801730A4(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b14_801730C4(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b14_801730E4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b14_80173104(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
    }
}

/* The banked room script's three-state fade-out tail, state index at
   p + 0x9B. State 0 darkens the renderable's three +0xC..+0xE colour bytes by
   0xC until its +0x14 reports one of 0xE000, then re-dresses it off the
   bank-local D_80174F00 facing table at a full 0x40, drops D_80083460's +0xA
   counter and steps. State 1 is the long refusal chain -- the same one this
   unit's landed func_b14_80173564 below runs -- and on the way out either
   brightens the three bytes to 0x80, clears d + 0x1C's 0x10000000 and 0x200
   and hands the actor back to func_b14_80170E9C, or bumps +0xA, arms a
   6-frame countdown in +0x96 and steps. State 2 brightens by 0xC a frame
   until that countdown runs out and then does the same hand-back.
   266 words, 0 differing, first compile.

   TWIN: the landed func_b43_80173228 (csrc/ovl_dng_b43.c, 231 words, ratio
   0.757) for the three-state skeleton and the refusal chain, plus THIS UNIT'S
   OWN landed func_b14_80173564 (152 words, ~150 lines below), which is the
   two-state sibling of the same chain and carries the fact table for it --
   read that one first. dngtwin's own four leads for this stub (b05, b27, b41,
   b06, 0.794..0.859) are ALL STILL STUBS, so the ratio ranking was no help
   and the twin came from the brief's sibling list instead.
   The deltas off b43: the three c[0xC..0xE] colour sets (b43 has none); the
   dress index read BACK through c + 0x2C instead of off the table symbol; a
   second `&= ~0x10000000' ahead of b43's `&= ~0x200'; a
   `*(s16 *)(p + 0x96) = 6' on both bump paths; no re-dress in the `again'
   tail; and the 0x1000/0x2008 gate reading
   `*(u16 *)((u8 *)&D_80083460 + 0x2)' rather than the D_80083462 symbol.

   The `switch' is a COMPARE CHAIN, not a jump table (`beq 1', `slti 2',
   `beqz 0', `beq 2'), so its arms emit in SOURCE order -- and 0, 1, 2 is the
   order the target wants.

   THE DRESS INDEX IS READ BACK OUT OF c + 0x2C, NOT OFF THE SYMBOL. This is
   the one delta that is cheap to get wrong and invisible in b43, whose own
   comment states the opposite for its body. The three `sb's to c[0xC..0xE]
   sit between the `sw' of the table pointer and the index load, and each `sb'
   through a `u8 *' wipes cse's memory table, so the reload survives as a real
   `lw 0x2C($a0)'. All three forms priced:
   | spelling                                      | words | aligned diff |
   | `(*(u8 **)(c + 0x2C))[idx]'                   |  266  |      0       |
   | `D_80174F00[idx]' (b43's form)                |  265  |     10       |
   | the table held in a `u8 *' local              |  265  |     10       |

   THE TWO `&=' ON d + 0x1C MUST BE TWO STATEMENTS, NOT ONE CONSTANT.
   0xEFFFFFFF & 0xFFFFFDFF is 0xEFFFFDFF, which is not an `andi' immediate,
   so combine declines to merge the pair and the target keeps both `and's over
   a single `lw'/`sw' -- the second load is cse'd and the first store deleted
   as dead. The vehicle does not matter, only the pair:
   | two `*(s32 *)(d + 0x1C) &= ' statements       |  266  |      0       |
   | the same two through one `s32' local           |  266  |      0       |
   | one folded `&= ~0x10000200'                   |  264  |      7       |

   THE FIVE `&D_80083460' BASES ARE FIVE SEPARATE LOCALS -- the 0x1000/0x2008
   gate, state 0's decrement, the two +0xA increments and state 2's decrement.
   That is b43's fact and it HELD, though at 20 rows rather than its 160:
   | five separate locals                          |  266  |      0       |
   | the gate's local shared with both increments   |  265  |     20       |

   OTHER MEASURED, LOAD-BEARING SPELLINGS, both ways priced:
   | spelling                                      | words | aligned diff |
   | `c[0xC] = c[0xD] = c[0xE] -= 0xC', chained,   |       |              |
   |   HIGH offset first                            |  266  |      0       |
   | `c[0xE] = c[0xD] = c[0xC] -= 0xC' (0xC first)  |  266  |      3       |
   | the three written out, each re-reading c[0xE]   |  267  |      6       |
   | `*(u16 *)(k + 0xA) += 1' BEFORE                |       |              |
   |   `*(s16 *)(p + 0x96) = 6'                     |  266  |      0       |
   | ... the two in the other order                 |  277  |     15       |
   | `(s16)' cast on the first func_800A2C34        |  266  |      0       |
   | no cast on it (b43 priced its own at 206/111)  |  265  |      7       |
   | `*(s8 *)(d + 0x6D)' for the `lb'               |  266  |      0       |
   | `d[0x6D]' (unsigned)                           |  266  |      1       |
   | `*(s16 *)(d + 0x64)' for the `lh'              |  266  |      0       |
   | `*(s32 *)(d + 0x64)'                           |  266  |      1       |

   MEASURED AND FREE at 266 words / 0 differing, so none of these is why it
   lands: the three colour bytes as three statements through ONE `u8' local
   instead of the chain (identical, as the chain's right-to-left order
   predicts); re-reading d + 0x1C at both flag tests instead of through `v';
   nesting the d + 0x64 guard instead of `&&'; and the `goto again' replaced
   by the func_800A6D30/func_80042B68 block written out in both arms -- jump2
   cross-joins it either way, so the `goto' is kept only as the shorter C.

   `--raw' is 44 differing and all 44 are relocations: 17 `jal's, 9 `j's and
   9 `%hi' + 9 `%lo' halves, four of each pair being the separate D_80083460
   bases (five locals, but jump2 merges the two +0xA-increment blocks into
   one). No masked word hides a constant address. */
void func_b14_8017313C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        c[0xC] = c[0xD] = c[0xE] -= 0xC;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80174F00;
        c[0xC] = c[0xD] = c[0xE] = 0x40;
        func_80047784(c,
                      (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                      0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                c[0xC] = c[0xD] = c[0xE] = 0x80;
                *(s32 *)(d + 0x1C) &= ~0x10000000;
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
                return;
            }
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
            *(s16 *)(p + 0x96) = 6;
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
            func_b14_801737C4(p, (u8 *)b, c, d);
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
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            c[0xC] = c[0xD] = c[0xE] = 0x80;
            *(s32 *)(d + 0x1C) &= ~0x10000000;
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
            return;
        }
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        *(s16 *)(p + 0x96) = 6;
        p[0x9B] += 1;
        return;
    case 2:
        c[0xC] = c[0xD] = c[0xE] += 0xC;
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            m = (u8 *)&D_80083460;
            *(u16 *)(m + 0xA) -= 1;
            c[0xC] = c[0xD] = c[0xE] = 0x80;
            *(s32 *)(d + 0x1C) &= ~0x10000000;
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
        }
        return;
    }
}

/* The banked room script's other two-state tail, state index at p + 0x9B,
   the relinked twin of the landed func_b28_8017357C (csrc/ovl_dng_b28.c).
   b28 and func_b44_80174438 both score 0.980 and both have this one's exact
   prologue -- -0x28 with s3, s2, s1, ra, s0 in that order -- so the prologue
   does not choose between them; b28 was taken because its body is the one
   with the priced fact table. Four operand deltas and one order delta, all
   read off this listing: state 0's dress table is D_80174F00, the shared
   tail's is D_80174EE0, the 0x80000 arm's second call is the bank-local
   func_b14_801737C4, and the installed handler is func_b14_80170E9C.
   152 words, 0 differing, first compile.

   THE TAIL'S TWO STATEMENTS ARE IN THE OPPOSITE ORDER TO THE TWIN'S, and
   that INVERTS one of the three deltas b28's comment states against b03
   ("the tail installs p + 0x8C before clearing d + 0x1C's 0x200"). Here
   d + 0x1C is cleared FIRST. This is a STATEMENT-ORDER fact, the class the
   taxonomy says follows neither the shape nor the frame, and it inverted
   even though the prologue is byte-identical to the twin's and the ratio is
   0.980. It is also the only fact on this body that is expensive:
   | spelling                                    | words | aligned diff |
   | clear d + 0x1C, then store p + 0x8C         |  152  |      0       |
   | store p + 0x8C, then clear d + 0x1C (b28)   |  152  |     21       |

   B28'S DUPLICATED-TAIL FACT DOES NOT APPLY HERE AND THE TARGET CANNOT TELL
   THE TWO FORMS APART. b28 writes its dress tail out TWICE because its body
   is two references short of giving `p' the lower allocno number, and
   measures the `goto'-to-one-copy form at 15 aligned differences. Here both
   forms measure 0, so b28's reference argument is dead on this body; the
   single copy is kept only because it is the shorter C.
   | spelling                                    | words | aligned diff |
   | opening arm `goto tail', one copy written   |  152  |      0       |
   | dress tail written out twice (b28's form)   |  152  |      0       |

   b28's SELECTION facts held, as that class predicts. Measured here, not
   assumed, with both spellings priced:
   | spelling                                    | words | aligned diff |
   | both facing tables named DIRECTLY           |  152  |      0       |
   | both hoisted into one `u8 *' local          |  152  |      5       |
   | only state 0's hoisted                      |  153  |     10       |
   | only the tail's hoisted                     |  153  |     13       |
   | `*(s8 *)(d + 0x6D)' for the `lb'            |  152  |      0       |
   | `d[0x6D]' (unsigned) instead                |  152  |      1       |
   | `*(s16 *)(d + 0x64)' for the `lh'           |  152  |      0       |
   | `*(u16 *)(d + 0x64)' instead                |  152  |      1       |
   | `(s16)' cast on BOTH func_800A2C34 results  |  152  |      0       |
   | no cast on the first                        |  151  |      5       |
   | no cast on the second                       |  151  |      7       |

   AND TWO OF B28'S STATED DELTAS ARE FREE HERE, so do not spend budget
   preserving them: the 0x1000 gate reads the D_80083462 SYMBOL in this copy
   as it does in b28, but off `*(u16 *)((u8 *)&D_80083460 + 0x2)' it is also
   152 / 0; and the d + 0x64 guard measures 152 / 0 both as an `&&' chain and
   as two nested ifs. */
void func_b14_80173564(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174F00;
        func_80047784(c, D_80174F00[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            goto tail;
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
            func_b14_801737C4(p, (u8 *)b, c, d);
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
    tail:
        *(u8 **)(c + 0x2C) = D_80174EE0;
        func_80047784(c, D_80174EE0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
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
   call emits the bare `jal` either way); hoisting D_80174EE8 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b14_801737C4(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174F00 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80174EE8;
        func_80047784(c, D_80174EE8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b14_8017388C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b14_80170E9C;
        }
    }
}

/* 14 words, 0 differing. */
void func_b14_801738F0(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
    }
}

/* 14 words, 0 differing. */
void func_b14_80173928(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
    }
}

/* 14 words, 0 differing. */
void func_b14_80173960(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
    }
}

/* 14 words, 0 differing. */
void func_b14_80173998(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
    }
}

/* 14 words, 0 differing. */
void func_b14_801739D0(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b14_80170E9C;
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
s32 func_b14_80173A08(u8 *a, void *b, void *c, s32 d)
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
        func_b14_80171410(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b14_80171410(p, b, c, p);
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

/* One gouraud-textured billboard quad per tag-chain node: compose the slot's
   rotation with the camera matrix in the 0x1F800000 scratchpad, RotAverage4
   the four SVECTORs taken off the payload's +0x64..+0x7B into the POLY_GT4 at
   the +0x8D0 cursor, and -- when the OT index is in range and at least one
   corner is on screen -- build its UV/CLUT/TPAGE words out of the slot's own
   +0x10/+0x12 overrides and hang the quad on the OT row.  0x34 bytes of
   cursor, which is what a POLY_GT4 costs.  346 words, 0 differing.

   The twin is csrc/ovl_dungeon_800D50FC.c's func_800D6170 (400 words, the
   ladder strip): same signature, same scratchpad layout, same
   PushMatrix/RotMatrix/ReadRotMatrix/CompMatrix/ScaleMatrix/SetTransMatrix/
   SetRotMatrix chain, same `- d - 6`, and the same four on-screen tests.
   Two of its three spelling facts hold here and were re-measured:

   * THE SCALE VECTOR IS ONE CHAINED ASSIGNMENT, which emits +0x3C, +0x38,
     +0x34 in that order.
   * THE FOUR ON-SCREEN TESTS ARE `|`, NOT `||` -- each `or` sits in the next
     term's branch delay slot.

   AND ONE OF THEM IS INVERTED: `&m` DOES *NOT* GO THROUGH A POINTER LOCAL
   here.  func_800D6170 needs `MATRIX *mp = &m;` for its 400th word; this
   function has no loop, the two `addiu $a0, $sp, 0x28` are what the target
   has, and a pointer local would make them a single $s register copy.  Write
   `ReadRotMatrix(&m); CompMatrix(&m, ...)` directly.  (The twin's `q[4]`
   declaration order does not arise -- `m` is the only stack local, so it
   sits at $sp+0x28 above RotAverage4's six-word outgoing argument area.)

   THE UV SCALARS ARE SET ARM B'S WAY AND ARITHMETIC'D ARM A'S WAY.  The four
   `sw` of e[8..0xB] into +0x0C/+0x10/+0x14/+0x18 are
   csrc/ovl_dungeon_800B45E0.c func_800B51C8's SECOND arm (straight stores,
   no pre-correction), while the `+=` / `& 0x100` / `<<= 8` block inside the
   on-screen test is its FIRST arm; the +0x100 correction moved from before
   RotAverage4 to after it.  Both halves relink +4 from func_800B51C8's
   offsets, which is the same +4 the whole scratchpad carries.

   THE ONE THING FOUGHT FOR: `e = (u8 *)s->unk_08;` COMES BEFORE
   `sp->unk_28 = s->unk_14;`.  Written the other way round the body is 348
   words and 3 aligned differences -- both MEMs are MEM_IN_STRUCT_P and
   varying, so sched1 keeps the dependence, the `lw $s3, 0x8($s5)` cannot
   reach the `lhu 0x14($s5)`'s load-delay slot and a `nop` is emitted on each
   side of the store.  Reading `e` first puts the load where the target has
   it with no alias question to answer.  (docs/matching-levers.md, "A LOAD'S
   DELAY SLOT CROSSES A STORE ONLY WHEN *BOTH* POINTER-SIDE MEMs CARRY
   MEM_IN_STRUCT_P" -- here the exemption is unreachable, so the cure is
   source order rather than a spelling.)

   `u16 unk_28` AND NOT THE TWIN'S `s16`: this function reads the field back
   three times (`& 0x100`, `& 0x8`, `& 0x4`) and the target's loads are
   `lhu`.  The scratchpad is therefore this unit's own typedef.

   The two OT links are `addPrim` -- P_TAG bitfield stores, which a
   hand-written `|` cannot produce -- exactly as in the twin.

   raw 16 against the masked 0: 1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on
   the ten library calls, and three on .text for the clut/tpage blocks' own
   forward `j`s.  The ten 0x1F8000xx addresses carry no relocation on either
   side and come out byte-identical: cse replaces each one's `lui` with a
   copy of the $s2 that already holds 0x1F800000, which is the `move`/`ori`
   pair the listing shows. */
void func_b14_80173B4C(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 *e;
    u8 f;

    g = &D_80083160;
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
    e = (u8 *)s->unk_08;
    sp->unk_28 = s->unk_14;
    sp->unk_0C.w = e[8];
    sp->unk_10.w = e[9];
    sp->unk_14.w = e[10];
    sp->unk_18.w = e[11];
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x34;
    sp->unk_B0[0].vx = *(u16 *)(p + 0x64);
    sp->unk_B0[1].vx = *(u16 *)(p + 0x6A);
    sp->unk_B0[2].vx = *(u16 *)(p + 0x70);
    sp->unk_B0[3].vx = *(u16 *)(p + 0x76);
    sp->unk_B0[0].vy = *(u16 *)(p + 0x66);
    sp->unk_B0[1].vy = *(u16 *)(p + 0x6C);
    sp->unk_B0[2].vy = *(u16 *)(p + 0x72);
    sp->unk_B0[3].vy = *(u16 *)(p + 0x78);
    sp->unk_B0[0].vz = *(u16 *)(p + 0x68);
    sp->unk_B0[1].vz = *(u16 *)(p + 0x6E);
    sp->unk_B0[2].vz = *(u16 *)(p + 0x74);
    sp->unk_B0[3].vz = *(u16 *)(p + 0x7A);
    sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                              (s32 *)(q + 0x8), (s32 *)(q + 0x14), (s32 *)(q + 0x20),
                              (s32 *)(q + 0x2C), &sp->unk_D0, &sp->unk_D4) -
                  d - 6;
    if ((u32)sp->unk_100 < 0x1E0)
    {
        if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x14) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x16) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x2C) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x2E) + 0x20) < 0x121))
        {
            s->unk_14 &= 0x7FFF;
            sp->unk_14.w += sp->unk_0C.w;
            if ((sp->unk_14.w & 0x100) != 0)
            {
                sp->unk_14.w -= 1;
            }
            sp->unk_18.w += sp->unk_10.w;
            if ((sp->unk_18.w & 0x100) != 0)
            {
                sp->unk_18.w -= 1;
            }
            sp->unk_10.w <<= 8;
            sp->unk_18.w <<= 8;
            if (s->unk_12 != 0)
            {
                if ((sp->unk_28 & 0x100) != 0)
                {
                    *(u16 *)(q + 0xE) = s->unk_12;
                }
                else
                {
                    *(u16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
                }
            }
            else
            {
                *(u16 *)(q + 0xE) = *(u16 *)(e + 0x6);
            }
            *(u16 *)(q + 0xC) = sp->unk_10.h + sp->unk_0C.h;
            *(u16 *)(q + 0x18) = sp->unk_10.h + sp->unk_14.h;
            if (s->unk_10 != 0)
            {
                *(u16 *)(q + 0x1A) = s->unk_10 + (*(u16 *)(e + 0x4) & 0xFF9F);
            }
            else
            {
                *(u16 *)(q + 0x1A) = *(u16 *)(e + 0x4);
            }
            *(u16 *)(q + 0x24) = sp->unk_18.h + sp->unk_0C.h;
            *(u16 *)(q + 0x30) = sp->unk_18.h + sp->unk_14.h;
            if (*(s16 *)(q + 0x8) > *(s16 *)(q + 0x2C))
            {
                q[0x18] -= 1;
                q[0x30] -= 1;
            }
            if (*(s16 *)(q + 0xA) > *(s16 *)(q + 0x2E))
            {
                q[0x25] -= 1;
                q[0x31] -= 1;
            }
            *(s32 *)(q + 0x4) = *(s32 *)(p + 0x4);
            *(s32 *)(q + 0x10) = *(s32 *)(p + 0x8);
            *(s32 *)(q + 0x1C) = *(s32 *)(p + 0xC);
            *(s32 *)(q + 0x28) = *(s32 *)(p + 0x10);
            SetPolyGT4((POLY_GT4 *)q);
            f = e[1];
            s->unk_0C[3] = f;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] = f | 0x2;
                }
                else
                {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            q[0x7] |= s->unk_0C[3];
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
        }
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_b14_80173B4C with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b14_801740B4(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b14_80173B4C(p, v, s, 0);
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
void func_b14_80174108(u8 *p)
{
    if (--*(s16 *)(p + 0x1A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b14", func_b14_8017414C);

/* Spawn this bank's sixteen-segment ring: for each of 16 steps allocate a
   0x12 node, dress it with D_80045340, arm it with func_b14_8017414C, copy
   the caller's position vector into the node's mover, and seed the payload's
   four-corner SVECTOR quad at +0x64..+0x7A from the step's two angles --
   `i << 8' and `(i + 1) << 8' -- so each node is one wall of a cylinder.
   124 words, 0 differing.

   NO TWIN, RESIDENT OR CARVED. dngtwin's only lead is func_b06_80174E78 at
   0.992, which is STILL A STUB (and b06 is held by another agent this run);
   xsibs at --min-words 18 --ratio 0.70 does not list this function at all,
   so there is no resident sibling either. The callee-set grep
   (func_8003FC64 + func_8004491C + D_80045340 + func_80064584 +
   func_800644B8) intersects to the seven resident dungeon units and
   ovl_dng_s20/s21/s69, and none of them has this shape: the 0x12 spawns
   there are single, not looped. What DID identify the payload was a grep for
   the two offsets 0x76 and 0x6A together, which hits only ovl_dng_b06,
   ovl_dng_s67 and THIS UNIT -- func_b14_80173B4C, 150 lines below, reads
   p + 0x64/0x6A/0x70/0x76 as `sp->unk_B0[0..3].vx' and p + 0x68/0x6E/0x74/
   0x7A as `.vz'. That is what says the eight `sh's are a quad and not eight
   scalars. Written from the listing against that reading.

   THE ONE THING THAT HAD TO BE FOUND: `r->unk_10 = 0x20' SITS BETWEEN THE
   TWO `r->unk_14 |= ' STATEMENTS. Two consecutive ors on one field collapse
   to a single `ori $v0, $v0, 0x8C' with one `sh' -- the first store is
   deleted as dead, since nothing between them touches memory -- and the
   target has both stores. Any MEM store between them saves the first one,
   and the scheduler then lifts it back above both, which is why the emitted
   order (`sh 0x10', `ori 0xC', `sh 0x14', `ori 0x80', `sh 0x14') does not
   look like the source order. The 0x10 store is the only candidate that also
   lands at 0:
   | spelling                                     | words | aligned diff |
   | `unk_10 = 0x20' BETWEEN the two ors          |  124  |      0       |
   | ... before both (the natural order)          |  122  |      5       |
   | ... after both                               |  122  |      5       |
   | `a = a | C' written out instead of `|='      |  122  |      5       |
   | `do { } while (0)' round each or             |  124  |      8       |
   | the three b->unk_0x copies between the ors   |  126  |     11       |
   | the -0x40 quad pair between the ors          |  126  |     32       |
   I call this MACRO RESIDUE rather than a device: a "set slot flags" macro
   used twice with an unrelated store written between the calls is the
   natural way to get this, and the plain-C spelling kept here is ordinary C.

   OTHER MEASURED, LOAD-BEARING SPELLINGS, both ways priced:
   | spelling                                     | words | aligned diff |
   | `s16 i' for the loop counter                 |  124  |      0       |
   | `s32 i'                                      |  120  |     26       |
   | `u16 i'                                      |  121  |     17       |
   | n->unk_0C RE-READ into `r' for the second    |      |              |
   |   block (0xC/0xD/0xE, 0x1C, 0x1E)            |  124  |      0       |
   | one `r' kept across the four trig calls      |  124  |     29       |
   | `q[0x4] = r->unk_0C[0]' (read back)          |  124  |      0       |
   | `q[0x4] = 0x20' (literal)                    |  123  |     18       |
   | `q[0x5]/q[0x6] = 0x80/0x20' as literals too  |  119  |     29       |
   | `(f(a) * 24) >> 12'                          |  124  |      0       |
   | `(f(a) * 24) / 4096'                         |  130  |     46       |
   | the quad pairs chained, high offset first    |  124  |      0       |
   | the same as separate low-offset-first stores |  124  |      5       |
   | `i < 0x10'                                   |  124  |      0       |
   | `i != 0x10'                                  |  124  |      2       |
   | a `while' loop with `i++' at the body's end  |  123  |      3       |

   MEASURED AND FREE at 124 words / 0 differing, so none of these is why it
   lands: `n->payload' in place of `(u8 *)n + 0x20'; the quad pairs as
   separate statements with the HIGH offset written first (identical to the
   chained form, as the chain's right-to-left order predicts);
   `if (n == NULL) continue;' in place of wrapping the body in
   `if (n != NULL)' -- jump2 cross-joins the two `addiu $v0, $s4, 1'
   increments either way; and dropping the unused third parameter.
   The third parameter is KEPT because the only caller,
   func_b14_801749A8 at 0x80174D10, sets up $a0, $a1 AND $a2.

   `--raw' is 10 differing and all 10 are relocations -- 6 `jal's and the
   `%hi'/`%lo' pairs for D_80045340 and func_b14_8017414C -- so no masked
   word hides a constant address. */
void func_b14_801745E0(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *v;
    UnkSlot *r;
    u8 *q;
    s16 i;

    for (i = 0; i < 0x10; i++)
    {
        n = func_8003FC64(0x12);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1A) = 0x32;
            *(s16 *)(q + 0x1C) = *(u16 *)(p + 0x96);
            n->unk_10 = (u32)func_b14_8017414C;
            func_8004491C(n, (u32)D_80045340);
            r = (UnkSlot *)n->unk_0C;
            r->unk_14 |= 0xC;
            r->unk_10 = 0x20;
            r->unk_14 |= 0x80;
            v = n->unk_08;
            v->unk_00 = b->unk_00;
            v->unk_04 = b->unk_04;
            v->unk_08 = b->unk_08;
            *(s16 *)(q + 0x68) = *(s16 *)(q + 0x6E) = -0x40;
            *(s16 *)(q + 0x74) = *(s16 *)(q + 0x7A) = 0;
            *(s16 *)(q + 0x64) = *(s16 *)(q + 0x70) = (func_80064584((i + 1) << 8) * 24) >> 12;
            *(s16 *)(q + 0x6A) = *(s16 *)(q + 0x76) = (func_80064584(i << 8) * 24) >> 12;
            *(s16 *)(q + 0x66) = *(s16 *)(q + 0x72) = (func_800644B8((i + 1) << 8) * 24) >> 12;
            *(s16 *)(q + 0x6C) = *(s16 *)(q + 0x78) = (func_800644B8(i << 8) * 24) >> 12;
            r = (UnkSlot *)n->unk_0C;
            r->unk_0C[0] = 0x20;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            r->unk_0C[1] = 0x80;
            r->unk_0C[2] = 0x20;
            q[0x4] = r->unk_0C[0];
            q[0x5] = r->unk_0C[1];
            q[0x6] = r->unk_0C[2];
        }
    }
}

/* State 0x17's entry: clear the node's +0x71 top bit and +0x6D, install
   ITSELF in the slot's +0x8C handler slot and mark state 0x17/0; then, unless
   D_80083460's +0x2 0x2000 bit is up, if func_800A2BDC says the engine is
   idle, drop the handler again, re-mark 0x17/0, reset D_800DCF5E and bump
   D_80083460's +0xA counter. 43 words, 0 differing, first compile.

   THE +0x8C STORE IS THE FUNCTION'S OWN ADDRESS -- `lui'/`addiu' of
   %hi/%lo(func_b14_801747D0) -- so the self-reference is written as the
   plain identifier, not as a table lookup.

   D_80083460 goes through the unit's `g = (u8 *)&D_80083460' idiom: one
   `addiu' base in $s1 serves both the +0x2 gate and the +0xA bump. */
void func_b14_801747D0(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    *(void **)(p + 0x8C) = func_b14_801747D0;
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

/* The two-phase white-out. With the slot's +0x9A still zero, park the view's
   +0x8 in the bank-local D_80174FD0, re-send the view through func_800478B8
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
void func_b14_8017487C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (p[0x9A] == 0)
    {
        D_80174FD0 = *(s32 *)(c + 0x8);
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
   the counter, hand func_80041588 the D_80174FAC command stream and its
   D_80174FCC handshake byte, turn the actor to face 2, swap the HUD pair
   through func_8003F540/func_8003E4FC, open with func_800C77D0, ease
   D_80083160's three colour bytes while the D_80174FCD handshake is pending,
   spawn the 0x12 node whose driver is func_b14_8017487C and then kick
   func_b14_801745E0's sixteen-segment ring, wait for the node's +0x14 to
   carry one of 0xE000, then func_800ADC4C the replacement in, re-aim it and
   finally raise 0x8000 in the node's -0x2 and in D_800814A0 and drop
   D_80083460's +0xA counter. 332 words, 0 differing, first compile.

   TWIN: the landed func_b20_80174D14 (csrc/ovl_dng_b20.c, 330 words, ratio
   0.955). The landed func_b08_80174648 (csrc/ovl_dng_b08.c, 349 words, 0.934)
   is the same body with state 5's arm written out a second time, which this
   one does NOT do, so b20 is the body to read and b08 only for its own
   re-measurement of b20's facts. Same frame as both -- addiu $sp,-0x28, six
   saves s0..s4/ra at 0x10..0x24 -- so b20's two allocation facts carry and
   BOTH HELD. The deltas off b20: state 1 clears +0xA8 where b20 clears +0xA4;
   state 2 DROPS b20's trailing func_b20_80174724 call and state 4 GAINS a
   func_b14_801745E0(p, b, c) as its last statement; the node pointer is
   parked in +0xA4 where b20 uses +0xAC; the node driver is
   func_b14_8017487C; and the tables are the bank-local D_80174FAC /
   D_80174FB4 / D_80174FCC / D_80174FCD / D_80174FD0 / D_80174F48 for b20's
   D_8017530C / D_80175318 / D_801753A8 / D_801753A9 / D_801753AC /
   D_80175314.

   THIS FUNCTION OWNS jtbl_b14_80170870, the 9-entry table at rodata offset
   0x68 (ROM 0xBB7070) for states 0..8. Every state has a label of its own, no
   two share an arm and there is no `default': the `sltiu $v1, 0x9' gate sends
   everything else to the epilogue. States 1, 2, 5 and 7 fall through to their
   successors. The table is the LAST object in this unit's .rodata -- splat
   gives the subsegment as 0xbb7008..0xbb7094 and 0x68 + 9*4 = 0x8C is exactly
   the segment end -- so its odd word count takes no trailing pad and all nine
   words are real cases, not eight plus alignment.

   B20'S TWO ALLOCATION FACTS, RE-MEASURED HERE. BOTH HELD:
   | spelling                                      | words | aligned diff |
   | state 3's D_80174FCD flag read into `f'       |       |              |
   |   BEFORE the +0x96 restore store               |  332  |      0       |
   | ... the restore written first (b20: 330/18)    |  333  |      8       |
   | state 6's `d - 0x20' IS states 4/5/7's `r'     |  332  |      0       |
   | ... given a local of its own (b20: 330/3)      |  332  |      3       |

   MY OWN DELTA IS THE EXPENSIVE ONE: func_b14_801745E0 MUST BE THE LAST
   STATEMENT OF STATE 4, after the +0x9B bump, not before it.
   | `func_b14_801745E0(p, b, c)' after the bump    |  332  |      0       |
   | ... before the bump                            |  327  |     18       |

   OTHER MEASURED, LOAD-BEARING SPELLINGS, both ways priced:
   | spelling                                      | words | aligned diff |
   | `s16 k' for the D_80174FB4 index               |  332  |      0       |
   | `s32 k'                                        |  329  |     15       |
   | `s32 func_800498A0(u8 *)' (b20's fact)         |  332  |      0       |
   | `u8', as its resident definition declares it   |  332  |      7       |
   | `g = (u8 *)&D_80083160' BEFORE the switch      |  332  |      0       |
   | ... taken inside case 3, its only user         |  333  |     11       |
   | three separate `n->unk_08->unk_0x' stores      |  332  |      0       |
   | n->unk_08 hoisted into one local               |  330  |      6       |
   | `*(u32 *)(r + 0xC) = 0x808080' BEFORE          |       |              |
   |   `k = func_800498A0(d) - 1'                   |  332  |      0       |
   | ... after it                                   |  333  |     15       |
   | state 1's `*(s16 *)(p + 0xA8) = 0' FIRST of    |       |              |
   |   the three post-call statements               |  332  |      0       |
   | ... last of the three                          |  333  |      3       |
   | `D_80174FCD[0] = 0' BEFORE func_8003E4FC       |  332  |      0       |
   | ... after it                                   |  333  |     40       |
   | state 7's +0x2A store reading d + 0x60 again,  |       |              |
   |   with `w' assigned after it                   |  332  |      0       |
   | `w' hoisted ahead of that store                |  330  |     15       |
   | state 5's `r = (u8 *)n->unk_0C' local           |  332  |      0       |
   | n->unk_0C inlined at its one use               |  332  |      2       |

   MEASURED AND FREE at 332 words / 0 differing, so none of these is why it
   lands: `s16 t2' in place of b20's `s32 t2' for state 2's facing index;
   state 6's `p[0x9B] == 6 && func_800ADC4C(...) != NULL' written as two
   nested ifs; state 2's `D_80174FCC[0] == 0 || t2 != 2' written as the
   inverted `!= 0 && == 2' with an empty then-arm; and `func_80069EF8' spelled
   as the address instead of `rand' -- same symbol, so that one is a naming
   choice only.

   `--raw' is 74 differing and all 74 are relocations: 21 `jal's, 6 `j's, 23
   `%hi' and 24 `%lo' halves. The halves do not pair evenly because state 2's
   single `lui %hi(D_80174FCD)' serves both the `addiu $a2' and the
   `sb $zero, %lo(D_80174FCD)($v0)' in the next delay slot. No masked word
   hides a constant address. */
void func_b14_801749A8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_80041588(D_80174FAC, (s8 *)D_80174FCC, 0);
        *(s16 *)(p + 0xA8) = 0;
        p[0x9B] += 1;
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x2A);
        /* fallthrough */
    case 2:
        t2 = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (D_80174FCC[0] == 0 || t2 != 2)
        {
            if (t2 != 2)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            return;
        }
        func_80041588(D_80174FAC, (s8 *)D_80174FCC, 1);
        func_8003F540(0, D_8006CD58, 0x4000AD4, 0x5000CC4);
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        D_80174FCD[0] = 0;
        func_8003E4FC(0xFF, D_8003E140, (s32)D_80174FCD);
        func_800C77D0(d - 0x20, (u8 *)b, 8, 0x300);
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        t = *(u16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            f = D_80174FCD[0];
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
        n->unk_10 = (u32)func_b14_8017487C;
        func_8004491C(n, (u32)D_80045340);
        *(UnkNode **)(p + 0xA4) = n;
        n->unk_08->unk_00 = b->unk_00;
        n->unk_08->unk_04 = b->unk_04;
        n->unk_08->unk_08 = b->unk_08;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        k = func_800498A0(d) - 1;
        if (k < 0)
        {
            k = rand() % 3;
        }
        func_8003DB94(r, D_8014A000 + D_80174FB4[k], 0);
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        func_b14_801745E0(p, b, c);
        return;
    case 5:
        n = *(UnkNode **)(p + 0xA4);
        r = (u8 *)n->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0xE000) != 0)
        {
            p[0x9B] += 1;
        }
        /* fallthrough */
    case 6:
        if (p[0x9B] == 6 &&
            func_800ADC4C((u8 *)b, D_80174FD0, D_80174F48, &D_800DCF5C) != NULL)
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
