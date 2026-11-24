#include "common.h"

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80170E5C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_801728B4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80172ED0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80173A20);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80173CE0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80174084);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_80174254);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b28", func_b28_801742AC);

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
