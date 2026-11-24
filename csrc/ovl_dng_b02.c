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
s32 func_b02_80171F9C(u8 *p, void *b, u8 *c, u8 *d);
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
extern u8 D_80175E40[];
extern u8 D_80175EA0[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b02_80171854(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC7EC();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b02_801710EC();

s32 func_800AB778();

s32 func_800AD058();

s32 func_800AB464();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

s32 func_800AB538();

extern u8 D_80175EC0[];
extern u8 D_80175E70[];
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800A9A04(u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b02_80174574(u8 *p, u8 *b, u8 *c, u8 *d);

s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
void func_800BB044(u8 *);
s32 func_800A56E0(s32);
void func_800A2B04(UnkVec *, s32, s32);
s16 func_800A9400(s16);
u32 func_800445E0(void);
void func_b02_80175814(u8 *);
void func_b02_80175D7C(u8 *p);
extern u8 D_80175F40[];
extern u16 D_80175F6E;
extern u8 D_800DCF50;
extern u8 D_800DDAB8[];
extern s32 D_8006CD58;
s32 func_800CEEFC();
void func_b02_801750E4(s32 a0, s32 a1, s32 a2, s32 a3, s32 ox, s32 oy, u8 *cu, u8 *cv, u8 *ot, s32 idx, s32 n);
void func_b02_80174FC0(u8 *, s32, s32);
UnkNode *func_8003FD64(s32, void *);
extern u8 D_80175F90[];

s16 func_800A2CB8(u8 *a, u8 *b);
extern u8 D_80175E50[];
extern u8 D_80175EA8[];
extern u8 D_80083498[];
extern u8 D_80175F70[];
extern u8 D_80175FA8[];
extern u8 D_80175FB4[];
void func_800B835C(u8 *, s32 *, s32, s32);
void func_b02_80175B1C(u8 *, UnkVec *, u8 *);
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
void func_b02_801747B8(u8 *p, u8 *b, u8 *c);
extern u8 D_80045340[];
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b02_80170A94(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A0134_w(u8 *, u8 *) __asm__("func_800A0134");

/* Spawn the kind-0x112 node this bank's scripts use for a thrown or placed
   actor: arm it with the bank-local func_b02_80170A94 handler and the
   D_80175E40 facing table, dress its slot from the b/c/d arguments, pick one
   of three flag pairs off `a & 3', and hand it to func_800A9C18 and
   func_800AA36C. 120 words, 0 differing, ON THE FIRST COMPILE.

   The twin is the 113-word func_b03_80170854 (csrc/ovl_dng_b03.c) and every
   one of its spellings held unchanged, so nothing here was fought for; read
   its comment for the shape. Five deltas, all read off the listing:
   `p[0x13]' is 2 rather than 3, the handler and the facing table are this
   bank's func_b02_80170A94 and D_80175E40 / D_80175EA0, the func_800A6D30 arm
   does NOT set `*(u32 *)(p + 0x1C) |= 0x200' the way the twin's does, and the
   tail gains `*(u32 *)(p + 0x1C) |= 0x40000;' and
   `*(s16 *)(w + 0x92) = -0x20;' before the func_800AA36C call. */
u8 *func_b02_801708B4(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b02_80170A94;
        p[0x13] = 2;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80175E40;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_80175EA0;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b02_801710EC;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x20;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80170A94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_801710EC);

/* The banked room script's path-step advance, the twin of func_b35_801714D8
   (csrc/ovl_dng_b35.c) and of the resident overlay's func_800D904C: past the
   d + 0x71 / d + 0x8A step gates, dress the renderable from D_80175E40,
   retire the current waypoint through func_8009A3D0, take the next pair out
   of d + 0x74 / d + 0x7C, and re-aim with func_8009A21C and func_800A0818.
   110 words, 0 differing, on the first compile, and WITH NO NEW DECLARATIONS
   -- D_80175E40, D_80083228, D_80083462, func_80047784, func_8009A3D0,
   func_8009A21C and func_800A0818 were all already in this unit.

   Three deltas from those twins, all read off the listing. There is no
   func_8009A66C call and no `(s16)t == 3' arm, so `p[0x9A] = 0xF' is
   unconditional. func_800A0818 is called AFTER func_8009A21C, not before it,
   and its first two arguments are the `x'/`y' locals straight -- which is why
   this copy needs none of the phantom-zero device func_b35_801714D8 and
   func_800D904C both carry. And the D_80175E40 dress has only the `!=' arm;
   func_800D904C's `else if' on the c + 0x14 0x6000 bits is absent. */
void func_b02_8017169C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80175E40)
        {
            *(u8 **)(c + 0x2C) = D_80175E40;
            func_80047784(c, D_80175E40[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
   AND PASSED ON. func_b02_80171F9C's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A8C0 2 words, func_b02_80171F9C 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b02_80171854(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b02_80171F9C(p, b, c, d) == 0)
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

/* The banked room script's "nudge into a neighbouring slot" branch: with the
   D_80083460 gates clear ask func_800A04F0 what sits under the cursor, then
   classify the heading delta into one of three windows and put the actor into
   state 0x11, 0x17 or 0x18 with the matching bank-local facing table.
   166 words, 0 differing.

   The twin is the 113-word func_b03_80171DA0 (csrc/ovl_dng_b03.c), itself the
   relinked twin of the resident overlay's func_800CA788; this copy is that
   body with the single 0x40-window test replaced by the three-arm
   classification and with a tail that also drains p + 0xA2 into p + 0x92.
   Four of that comment's facts were re-measured here and all held: the
   func_800A04F0 alias with its `s16' fourth parameter, the shared `-1' exit
   label sitting in the SECOND func_800A2B5C test, `d[0x6D] -= 1' BEFORE the
   func_8009C93C call, and `g = (u8 *)&D_80083460' as one pointer local reused
   by all three gates. Its 8-byte leftover aggregate local is absent here --
   the 0x38 frame is exactly the eight register saves plus func_8009C93C's
   0x18 outgoing area -- and adding `s32 unused[2];' back is 166 words,
   16 differing.

   THE FACING TABLE IS READ BACK OUT OF c + 0x2C FOR THE func_80047784 CALL.
   Every sibling in this unit keeps it in a local (`e = D_80175E70;
   *(u8 **)(c + 0x2C) = e; func_80047784(c, e[...], 0);') because there is one
   table; here the three arms store three different ones and the target
   re-loads `lw $v1, 0x2C($s3)' at .Lb02_801721B0 instead of holding a
   register across the merge. Carried in a local assigned in each arm it is
   166 words, 2 differing -- the `lw' becomes the arm's register and the
   arm-3 constant moves.

   AND THE STATE BYTE IS A VARIABLE WHILE THE TABLE IS NOT. .Lb02_80172144 is
   a four-insn tail shared by the r == 1 and r == 2 arms whose first insn is
   `sb $v1, 0x9A($s1)' -- a register, not a constant -- so the 0x9A store,
   the p + 0x98 `|= 8' and the +0x84/+0x85 pair are written ONCE after the
   chain and the r == 3 arm jumps past them. Writing all three arms out in
   full, which is what cross-jumping would have to merge, is 170 words and
   24 differing: gcc 2.7.2's cross_jump compares constants exactly, so it
   stops at the differing 0x11/0x17 and the merge point lands one insn late.

   `return 0;' after func_800A2CB8 is the literal and the one at the end of
   the classification chain is `return r;' -- the target's `addu $v0, $zero,
   $zero' against `addu $v0, $s4, $zero' at .Lb02_801720AC, even though r is
   provably 0 at both. Spelling both `return r;' is 165 words, 3 differing. */
/* The banked room script's "nudge into a neighbouring slot" branch: with the
   D_80083460 gates clear ask func_800A04F0 what sits under the cursor,
   classify the heading delta into one of three 0x21/0x31/0x31-wide windows and
   put the actor into state 0x11, 0x17 or 0x18 with the matching bank-local
   facing table, then drain p + 0xA2 into p + 0x92 and return the window index.
   166 words, 0 differing.

   The twin is the 113-word func_b03_80171DA0 (csrc/ovl_dng_b03.c), itself the
   relinked twin of the resident overlay's func_800CA788; this copy is that
   body with the single 0x40 heading window replaced by the three-arm
   classification and with the p + 0x92 tail added. Three of that comment's
   facts carried: the func_800A04F0 alias with its `s16' fourth parameter,
   `g = (u8 *)&D_80083460' as one pointer local reused by all three gates, and
   no 8-byte leftover aggregate (the 0x38 frame is exactly the eight register
   saves plus func_8009C93C's 0x18 outgoing area).

   `r' IS `s16' AND THAT IS THE WHOLE RESIDUE. As `s32' the literal
   `return 0;' after func_800A2CB8 comes out `addu $v0, $s4, $zero' -- cse has
   r's pseudo in the zero equivalence class and spends it on the SImode
   constant -- and the dispatch's `addu $v1, $s4, $zero' copy disappears:
   165 words, 24 aligned differences. That is matching-levers.md's "A NARROW
   RETURN-FLAG LOCAL STOPS cse SUBSTITUTING ITS REGISTER FOR A LITERAL
   return 0". With `r' narrowed, `return 0;' and `return r;' at that gate are
   both 0 differing; the literal is kept.

   AND THE TWIN'S SHARED `-1' LABEL IS WRONG HERE: FOUR PLAIN `return -1;'.
   The twin proves its own body needs the label inside the SECOND
   func_800A2B5C test; spelled that way this body is 162 words,
   32 differences, because the label then splits the classification's
   `return r' off into a block where cse no longer knows `r' and splits the
   0x92/0xA2 drain off from its own return-value move. reorg rebuilds the
   shared `j .Lb02_80172208' / `addiu $v0, $zero, -0x1' block at
   .Lb02_801720F0 from the four plain returns by itself.

   THE THREE ARMS ARE WRITTEN OUT IN FULL. The four-insn tail they share at
   .Lb02_80172144, starting at a REGISTER `sb $v1, 0x9A($s1)', is what
   cross-jumping makes of three duplicated arms -- it stops at the differing
   0x11/0x17 constant and leaves the `lhu' of p + 0x98 in each arm. Written
   once through a state variable with a `goto' past it: 163 words,
   57 differences.

   func_800A0134 HAS TO COME THROUGH AN s32-RETURNING ALIAS. The landed
   `s16 func_800A0134' makes `h = -func_800A0134(t, d)' a `sll'/`sra'
   extension pair ahead of the target's bare `negu' -- 168 words,
   6 differences -- and the landed declaration is used twice more in this unit
   so it cannot simply be widened. And the facing table is READ BACK OUT OF
   c + 0x2C for the func_80047784 call (`lw $v1, 0x2C($s3)' at
   .Lb02_801721B0), not carried in a local the way every one-table sibling in
   this unit does it. */
s32 func_b02_80171F9C(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 h;
    s16 r;

    r = 0;
    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        return -1;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        return -1;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
    {
        return -1;
    }
    h = -func_800A0134_w(t, d);
    if ((u16)(h - 0x20) < 0x21)
    {
        r = 1;
    }
    else if ((u16)(h + 0x10) < 0x31)
    {
        r = 2;
    }
    else if ((u16)(h + 0x40) < 0x31)
    {
        r = 3;
    }
    else
    {
        return r;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    p[0x9B] = 0;
    if (r == 1)
    {
        *(s32 *)(p + 0x8C) = 0;
        *(u8 **)(c + 0x2C) = D_80175E50;
        p[0x9A] = 0x11;
        *(u16 *)(p + 0x98) |= 8;
        d[0x84] = 0x7C;
        d[0x85] = 0;
    }
    else if (r == 2)
    {
        *(s32 *)(p + 0x8C) = 0;
        *(u8 **)(c + 0x2C) = D_80175E70;
        p[0x9A] = 0x17;
        *(u16 *)(p + 0x98) |= 8;
        d[0x84] = 0x7C;
        d[0x85] = 0;
    }
    else
    {
        *(s32 *)(p + 0x8C) = 0;
        *(u8 **)(c + 0x2C) = D_80175EA8;
        p[0x9A] = 0x18;
        d[0x84] = 0x7C;
        d[0x85] = 0;
        *(s32 *)(d + 0x1C) &= ~0x40000;
    }
    func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
    *(s16 *)(p + 0xA2) = 0;
    *(s16 *)(p + 0x9E) = 0;
    return r;
}

/* The banked room script's unconditional "same slot" entry: past the single
   D_80083462 0x2000 gate and the two func_800A2B5C tests, put the actor into
   state 0x17, swap the D_80175E70 facing table into the slot's +0x2C and
   re-aim through func_8009C93C. 73 words, 0 differing, on the first compile.

   The twin is the 70-word func_b03_80171F64 (csrc/ovl_dng_b03.c); read its
   comment for the mechanism, which all held here -- the `(s16)' on both
   func_800A2B5C tests, no leftover aggregate local (the 0x30 frame is exactly
   the five saves plus func_8009C93C's 0x18 outgoing area), `d[0x6D] -= 1'
   BEFORE the func_8009C93C call and the +0x84/+0x85 pair AFTER it, and the
   p + 0x9A, p + 0x8C, p + 0x9B store order.

   Three deltas from that twin, all read off the listing: the state byte is
   0x17 rather than 0x11, the facing table is this bank's D_80175E70, and
   there is an extra `*(u16 *)(p + 0x98) |= 8;' between `d[0x6D] -= 1;' and
   the func_8009C93C call -- the three words at .Lb02_8017230C. */
void func_b02_80172234(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    p[0x9A] = 0x17;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    e = D_80175E70;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    *(u16 *)(p + 0x98) |= 8;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80172358);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80172480);

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
s32 func_b02_8017266C(u8 *a, void *b, void *c, s32 d)
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
        func_b02_80171854(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b02_80171854(p, b, c, p);
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
   func_b02_801710EC. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b02_801727B0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b02_801710EC;
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
void func_b02_80172844(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_801728A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80172DDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_8017328C);

/* The banked room script's five-state carry machine on p + 0x9B. State 0
   picks the slot triple out of (d + 0x46) & 0x3FFF, publishes it into
   D_80175F6E with 0x80 set when the triple is one of the three high arms, and
   hands it to func_800A94A0; state 1 waits on func_8003F270 and loads the
   +0x96 countdown out of the bank-local D_80175F40 facing table; state 2 runs
   that countdown down by D_80080A84 and re-uploads the HUD pair from
   D_800DDAB8; state 3 waits on func_8003F270 again; state 4 is the hand-back
   that re-installs func_b02_801710EC. State 0's head is the same machine as
   the landed func_b14_8017293C / func_b03_80172A24 (so `s16 k', the inner
   switch written 7/3, 6/2, 5/1, default, and `__builtin_abs' for the two sign
   fixes), but the four states after it are this function's own and no landed
   function in the project is a near-sibling of the whole body
   (tools/decomp/xsibs.py finds nothing for it at ratio 0.40). 350 words,
   0 differing, 0 aligned differences.

   THE D_800DDAB8 BASE MUST BE TAKEN INTO ITS LOCAL BEFORE STATE 2's
   func_800A9400 CALL, not between func_800A56E0 and func_8003F540. Hoisted,
   `f' is live across two calls, becomes a global allocno and gets a
   callee-saved register -- which is the target's `lui $s1' / `addiu $s1' pair
   ahead of the first `jal'. Written after the calls it is a block-local temp
   instead: still 350 words, but 51 aligned differences, because with $s1 no
   longer needed for `f' global_alloc hands $s1 to `d' and $s2 to `k' where
   the target has them the other way round -- 49 of the 51 rows are that one
   swap and the remaining 2 are the address pair's own slots.

   THIS FUNCTION OWNS TWO JUMP TABLES: jtbl_b02_80170880, the 5-entry table at
   0x80170880 for the switch on p[0x9B] (cases 0..4, with a sixth zero word
   from the .align 3), and jtbl_b02_80170898, the 7-entry table at 0x80170898
   for state 0's switch over (d + 0x46) & 0x3FFF (cases 1,2,3,5,6,7 with case
   4 falling to `default'). Both are written as `switch'es. */
void func_b02_80173560(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    u8 *f;
    u8 *w;
    s16 k;
    s16 j;
    s32 i;
    s32 t;
    s32 x;
    s32 y;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
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
        *(u16 *)(p + 0x98) &= 0xFF7F;
        t = g[0];
        if (k != 0)
        {
            t |= 0x80;
        }
        D_80175F6E = t;
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
        if (func_800A94A0(d, g, k, p + 0x98) == 0)
        {
            break;
        }
        if (k != 0)
        {
            D_800DCF50 = g[0];
            d[0x13] |= 0x80;
        }
        func_80047784(
            c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 2);
        *(u16 *)(c + 0x14) |= 0x800;
        if (k != 0 && (d[0x13] & 0x80) == 0)
        {
            func_b02_80175814(d);
            func_b02_80175D7C(d);
        }
        else
        {
            d[0x13] &= 0x7F;
            D_80175F6E &= 0xFF7F;
            func_800BB044(d);
        }
        p[0x9B] += 1;
    case 1:
        if ((D_80175F6E & 0x80) != 0)
        {
            if (func_8003F270() != 0)
            {
                break;
            }
            func_800A56E0(0x300);
            *(s16 *)(p + 0x96) = D_80175F40[func_800A9400(D_80175F6E & 0x7F)] - 0x10;
            p[0x9B] += 1;
        }
        else
        {
            p[0x9B] += 2;
        }
        break;
    case 2:
        *(s16 *)(p + 0x96) -= D_80080A84;
        if (*(s16 *)(p + 0x96) < 3)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        f = D_800DDAB8;
        j = func_800A9400(D_80175F6E & 0x7F) * 2;
        func_800A56E0(0x1300);
        func_8003F540(0, D_8006CD58, *(u32 *)(f + j * 4), *(u32 *)(f + j * 4 + 4));
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        p[0x9B] += 1;
    case 3:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B] += 1;
    case 4:
        if ((*(u16 *)(p + 0x98) & 0x80) == 0)
        {
            c[0x5] -= 1;
        }
        if ((*(s8 *)(c + 0x4) == 4 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
            if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
            {
                *(s32 *)((u8 *)b + 0x14) = 0;
                *(s32 *)((u8 *)b + 0x10) = 0;
                *(s32 *)((u8 *)b + 0xC) = 0;
                func_800A2B04(b, c[0x24], c[0x25]);
                w = D_80175E40;
                if (*(u8 **)(c + 0x2C) != w)
                {
                    *(u8 **)(c + 0x2C) = w;
                    func_80047784(
                        c, w[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
                g2 = (u8 *)&D_80083460;
                if (*(u32 *)(g2 + 0xC) == 0)
                {
                    *(u16 *)(g2 + 0xA) -= 1;
                    *(void **)(p + 0x8C) = func_b02_801710EC;
                    func_800A4ACC(d);
                    d[0x6D] -= 1;
                    *(u16 *)(d + 0x46) &= 0x7FFF;
                    *(u16 *)(p + 0x98) &= 0xFFBF;
                    d[0x73] = 0;
                    d[0x72] = 0;
                    func_800A56E0(0xB4);
                }
            }
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80173AD8);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b02_80173DAC(void)
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
void func_b02_80173DCC(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b02_80173DF8(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b02_80173E18(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_80175EA0 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b02_801710EC. 241 words,
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
   cleared, and the tail call is this unit's already-landed func_b02_80174574.
   State 2 changes no flag word at all. And in the tail the two bumps are in
   the opposite order to the twin's -- `p[0x9B] += 1' first, then
   `*(u16 *)(k + 0xA) += 1' -- which is the order the `sb 0x9B' / `lhu 0xA'
   pair appears in at .Lb02_80174194. */
void func_b02_80173E50(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80175EA0;
        func_80047784(c, D_80175EA0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80175EC0;
            func_80047784(c, D_80175EC0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
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
            func_b02_80174574(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175EC0;
        func_80047784(c, D_80175EC0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
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
        *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
        return;
    }
}

/* The three-state tail of the handler installed at p + 0x8C, switching on
   p + 0x9B: state 0 re-dresses the renderable from the bank-local D_80175EA0
   facing table, state 1 is the long refusal chain, state 2 hands the actor
   back to func_b02_801710EC. Transcribed from the landed func_b03_801736F4
   (csrc/ovl_dng_b03.c), itself the banked twin of the resident
   func_800DA398, with this bank's symbols and four deltas of its own:
   state 0's dress is guarded by `*(u8 **)(c + 0x2C) != D_80175EA0' where b03
   dresses unconditionally, both of state 1's dress sites use D_80175EC0, the
   0x80000 arm clears p + 0xA2 / p + 0x9E into p + 0x92 before calling
   func_b02_80174574, and state 2 clears only 0x8 of d + 0x1C where b03
   clears 0x208. Landed on the first compile: 216 words, 0 differing,
   0 aligned differences, so none of b03's measured spellings had to be
   re-measured here -- but they are all carried over, and the two that its
   comment calls load-bearing are the bare facing-table symbol at every dress
   site (not a shared `u8 *' local) and a separate `&D_80083460' pointer
   local per arm. */
void func_b02_80174214(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        if (*(u8 **)(c + 0x2C) != D_80175EA0)
        {
            *(u8 **)(c + 0x2C) = D_80175EA0;
            func_80047784(c, D_80175EA0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
        }
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80175EC0;
            func_80047784(c, D_80175EC0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
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
            *(u16 *)(p + 0xA2) = 0;
            *(u16 *)(p + 0x9E) = 0;
            func_b02_80174574(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175EC0;
        func_80047784(c, D_80175EC0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(s32 *)(d + 0x1C) &= ~0x8;
        *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
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
   call emits the bare `jal` either way); hoisting D_80175E40 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b02_80174574(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80175EA0 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80175E40;
        func_80047784(c, D_80175E40[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b02_8017463C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b02_801710EC;
        }
    }
}

/* 14 words, 0 differing. */
void func_b02_801746A0(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* 14 words, 0 differing. */
void func_b02_801746D8(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* 14 words, 0 differing. */
void func_b02_80174710(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* 14 words, 0 differing. */
void func_b02_80174748(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* 14 words, 0 differing. */
void func_b02_80174780(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b02_801710EC;
    }
}

/* The hazard's per-frame tick: refresh the slot from the +0x18 record, run
   the +0xC colour up by 0x101010 until it passes 0x80 >> p[0x34] and then
   back down by 0xFFF7F7F8 until it reaches zero, publish the head of the
   trail into the mover's three integer halves, then shift the whole trail one
   step along and write the +0x1C record's position into the new head.
   86 words, 0 differing. No twin: the family has two members and neither
   sibling was landed, so this is m2c's draft worked down from 3 differing.

   THE p + 0x17 BUMP MUST NOT GO THROUGH AN `s32' LOCAL. The byte is tested
   against zero and incremented in the branch where that test succeeded, so
   with `t = p[0x17]; if (t == 0) ... p[0x17] = t + 1;' cse knows `t' is zero
   on the path it follows and folds the store value to `li $v0, 1', where the
   target keeps `addiu $v0, $t0, 0x1' off the register the compare left live.
   Written as `p[0x17] += 1;' against a direct `if (p[0x17] == 0)' the load is
   the available expression rather than the constant and the register survives.

   | spelling                                       | words | differing |
   | as written below                                |  86   |     0     |
   | `u8 t' local, `p[0x17] = t + 1'                 |  86   |     0     |
   | `s32 t' local, `p[0x17] = t + 1'                |  86   |     3     |
   | `s32 t' local, `p[0x17] = p[0x17] + 1'          |  86   |     3     |
   | 0x80 >> gate inverted, bump as the `if' body    |  86   |     7     |
   | p + 0x17 gate inverted, bump in the `else if'   |  86   |    30     |

   The `u8 t' spelling also measures 0; the one with no local at all is kept.
   The trail shift is subscripted `p + n * 2 + 0x20' rather than walked with a
   pointer local: loop.c strength-reduces the body to the target's `addiu $a1,
   $a1, 0x2' walk by itself, and the post-loop head store then recomputes the
   address from `n' the way the target's `sll $v1, $a2, 1' / `addu' pair does.
   The +0x18 and +0x1C records are re-read at every use, never hoisted. */
void func_b02_801747B8(u8 *p, u8 *b, u8 *c)
{
    s32 n;

    *(s32 *)(c + 0x8) = *(s32 *)(*(u8 **)(p + 0x18) + 0x8);
    *(s16 *)(c + 0x14) = *(u16 *)(*(u8 **)(p + 0x18) + 0x14) | 0xC;
    if (p[0x17] == 0)
    {
        if (c[0xC] < (0x80 >> *(s16 *)(p + 0x34)))
        {
            *(s32 *)(c + 0xC) += 0x101010;
        }
        else
        {
            p[0x17] += 1;
        }
    }
    else if (c[0xC] != 0)
    {
        *(s32 *)(c + 0xC) += 0xFFF7F7F8;
    }
    else
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    *(s16 *)(b + 0x2) = *(u16 *)(p + 0x20);
    *(s16 *)(b + 0x6) = *(u16 *)(p + 0x26);
    *(s16 *)(b + 0xA) = *(u16 *)(p + 0x2C);
    for (n = 0; n < *(s16 *)(p + 0x34); n++)
    {
        *(s16 *)(p + n * 2 + 0x20) = *(u16 *)(p + n * 2 + 0x22);
        *(s16 *)(p + n * 2 + 0x26) = *(u16 *)(p + n * 2 + 0x28);
        *(s16 *)(p + n * 2 + 0x2C) = *(u16 *)(p + n * 2 + 0x2E);
    }
    *(s16 *)(p + n * 2 + 0x20) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2);
    *(s16 *)(p + n * 2 + 0x26) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x6);
    *(s16 *)(p + n * 2 + 0x2C) = *(u16 *)(*(u8 **)(p + 0x1C) + 0xA);
}

/* Spawn three kind-0x12 nodes on the actor at `a', each carrying a copy of
   the parent's +0x8/+0xC pointers and three copies of the sprite's 0x2/0x6/0xA
   extents, and on the first of the three walk the tag chain at
   (*(a + 0xC))[8] to the first entry whose +0x0 bit 0x20 is clear and
   LoadImage its 0x10 x 1 strip across the whole 0x100-wide page.
   113 words, 0 differing.

   No twin: xsibs found nothing, and the func_8003FD64 + LoadImage callee pair
   intersects no landed body. LoadImage is 0x800672D8 in
   splat/common_syms.txt, which is what makes the sp + 0x10 block a RECT.

   THE RECT IS FILLED y, h, w, x. The `lhu' of k + 0x6 has to issue ahead of
   all three constant stores to cover its own load delay, which is what the
   target's `lhu' at .Lb02_80174A1C followed by 1 / 0x10 / 0 and only then the
   `andi'/`addiu'/`andi'/`srl' chain is. In the natural x, y, w, h order it is
   5 aligned differences; in h, w, x, y it is 114 words and 10, because the
   load then lands after the stores and needs a `nop'. */
void func_b02_80174910(u8 *a, u8 *b)
{
    UnkNode *n;
    u8 *e;
    u8 *r;
    u8 *k;
    s32 i;
    s32 t;
    RECT rect;

    for (i = 0; i < 3; i += 1)
    {
        n = func_8003FD64(0x12, a);
        if (n != NULL)
        {
            e = (u8 *)n + 0x20;
            n->unk_10 = (u32)func_b02_801747B8;
            *(u32 *)(e + 0x18) = *(u32 *)(a + 0xC);
            *(u32 *)(e + 0x1C) = *(u32 *)(a + 0x8);
            t = *(u16 *)(*(u8 **)(a + 0x8) + 0x2);
            *(s16 *)(e + 0x24) = t;
            *(s16 *)(e + 0x22) = t;
            *(s16 *)(e + 0x20) = t;
            t = *(u16 *)(*(u8 **)(a + 0x8) + 0x6);
            *(s16 *)(e + 0x2A) = t;
            *(s16 *)(e + 0x28) = t;
            *(s16 *)(e + 0x26) = t;
            t = *(u16 *)(*(u8 **)(a + 0x8) + 0xA);
            *(u8 **)(e + 0xC) = b;
            *(s16 *)(e + 0x34) = i;
            *(s16 *)(e + 0x30) = t;
            *(s16 *)(e + 0x2E) = t;
            *(s16 *)(e + 0x2C) = t;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            func_8004491C(n, (u32)D_80045340);
            if (i == 0)
            {
                k = *(u8 **)(*(u8 **)(a + 0xC) + 0x8);
                while ((k[0] & 0x20) != 0)
                {
                    k += 0xC;
                }
                rect.y = (u16)((*(u16 *)(k + 0x6) & 0xFFC0) - 0x40) >> 6;
                rect.h = 1;
                rect.w = 0x10;
                rect.x = 0;
                do
                {
                    LoadImage(&rect, (u32 *)D_80175F70);
                    rect.x += 0x10;
                } while ((s16)rect.x < 0x100);
            }
            *(s16 *)((u8 *)n->unk_0C + 0x12) -= 0x40;
            *(s16 *)((u8 *)n->unk_0C + 0x10) = 0x20;
        }
    }
}

/* The 0x12 node's per-frame step for the bank's screen-wipe: state 0 emits
   the draw-area / 0x60000000 tint pair into D_80083160's 0x8D0 primitive ring
   and runs func_b02_801750E4 over the 0x340 x 0x100 rectangle at sp + 0x30,
   state 1 installs func_800CEEFC as the node's tag, state 3 fades c + 0xC out
   by 8 a frame and retires the node at zero, and every other state fades it
   in by 0x20; the shared tail copies the +0x1C record's three position
   halfwords into the mover and runs func_b02_80174FC0 four times. The
   D_80083160 / 0x8D0 ring prologue, the `lean' flag against D_801C9E40 and
   the tint primitive are func_800B8FC8's (csrc/ovl_dungeon_800B6AC4.c)
   line-for-line, with the ordering table at unk_00 + 0xB0 for its + 0x8B0;
   the state machine around them is this function's own. The dispatch is
   written as an if / else-if chain in the listing's own 0, 1, 3, default
   order. 179 words, 0 differing, 0 aligned differences.

   THE 0x350 HAS TO REACH func_b02_801750E4 THROUGH THE SAME LOCAL THAT
   STORES w[4]. As two separate literals the constant is materialised twice --
   the second `li $v0, 0x350' lands after the `lean' branch, where the target
   still has the first one live -- and the body is 180 words of 179 with 4
   aligned differences. The two uses straddle a basic-block boundary, so cse
   cannot share them; an s32 local does.

   AND `*(u32 *)(u + 0xC)' IS STORED BEFORE `*(s16 *)(u + 0xA)'. The source
   order u + 0xA then u + 0xC is 179 words / 2 aligned differences: the `lw'
   of the w[2] word no longer has the 0xA store to schedule past, so
   `sh $v1, 0xA($a1)' comes out one slot early. `s16 lean' is kept from
   func_800B8FC8 (where `s32' is 95/56); it is what makes the read of the flag
   a mode conversion whose extension combine drops, leaving the target's bare
   `addu $s0, $s3, $zero' copy ahead of the first test. */
void func_b02_80174AD4(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[6];
    s16 lean;
    s16 i;
    s32 t;
    s32 n;

    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    r = D_80083160.unk_00 + 0xB0;
    if (p[0x17] == 0)
    {
        q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        ((u32 *)D_80083160.unk_00)[0x234] = (u32)(q + 0xC);
        SetDrawArea(q, D_80083160.unk_00);
        AddPrim(r, q);
        w[0] = 0x340;
        w[1] = 0x100;
        w[2] = 0x20;
        w[3] = 0x20;
        n = 0x350;
        w[4] = n;
        t = 0x110;
        if (lean)
        {
            t = 0x30;
        }
        w[5] = t;
        func_b02_801750E4(0x10, 0x10, 0, 0, n, w[5], p, p + 4, r, 0, 0x20);
        u = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = w[0];
        t = w[1];
        if (lean)
        {
            t -= 0xE0;
        }
        *(u32 *)(u + 0xC) = *(u32 *)&w[2];
        *(s16 *)(u + 0xA) = t;
        AddPrim(r, u);
        q = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
        SetDrawArea(q, w);
        AddPrim(r, q);
        goto bump;
    }
    else if (p[0x17] == 1)
    {
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800CEEFC);
        goto bump;
    }
    else if (p[0x17] == 3)
    {
        if (c[0xC] != 0)
        {
            c[0xC] -= 8;
            if (c[0xC] == 0)
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
    }
    else
    {
        if (c[0xC] < 0xC0)
        {
            c[0xC] += 0x20;
        }
        if (p[0x16] == (*(u8 **)(p + 0x10))[0])
        {
            goto tail;
        }
    bump:
        p[0x17] += 1;
    }
tail:
    if (p[0x17] == 3)
    {
        return;
    }
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(p + 0x1C) + 0xA) - 0xC;
    i = 4;
    do
    {
        func_b02_80174FC0(p - 0x20, (*(u16 *)*(u8 **)(p + 0xC) >> 9) & 7, 0xFF);
    } while (--i > 0);
}

/* The spawner for the node func_b02_80174AD4 above steps: allocate the 0x12
   node off the first argument, give it that function as its +0x10 handler,
   seed the mover's three position pairs from the caller's +0x8 record, dress
   the renderable from the bank-local D_80175F90 at 0x1000 scale, and park the
   two handler arguments plus the record in the payload. func_800B8EA8
   (csrc/ovl_dungeon_800B6AC4.c) is the same shape at the same offsets.
   NO DECLARATION WAS NEEDED FOR ITS HANDLER: func_b02_80174AD4 is a
   definition above it in this unit now. 62 words, 0 differing, 0 aligned
   differences.

   `*(u8 **)(q + 0x1C)' IS THE FIRST OF THE PAYLOAD STORES. Written third,
   after q + 0xC and q + 0x10, the `lw $v0, 0x8($s0)' it needs lands
   immediately before its own use and takes a load-delay `nop': 63 words of
   62, 4 aligned differences (the `nop', the displaced `lw', and the entry
   `beqz's displacement, which the extra word moves). Put first, the two
   pointer stores fill the load delay, which is what the target does. */
void func_b02_80174DA0(u8 *a, void *b, u8 *c)
{
    UnkNode *n;
    u8 *v;
    u8 *s;
    u8 *q;
    s32 t;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b02_80174AD4;
        v = (u8 *)n->unk_08;
        t = *(u16 *)(*(u8 **)(a + 0x8) + 0x2);
        *(s16 *)(v + 0xE) = t;
        *(s16 *)(v + 0x2) = t;
        t = *(u16 *)(*(u8 **)(a + 0x8) + 0x6);
        *(s16 *)(v + 0x12) = t;
        *(s16 *)(v + 0x6) = t;
        t = *(u16 *)(*(u8 **)(a + 0x8) + 0xA);
        *(s16 *)(v + 0x16) = t;
        *(s16 *)(v + 0xA) = t;
        s = (u8 *)n->unk_0C;
        *(u8 **)(s + 0x8) = D_80175F90;
        *(s16 *)(s + 0x1E) = 0x1000;
        *(s16 *)(s + 0x1C) = 0x1000;
        *(s32 *)(s + 0xC) = 0;
        *(s16 *)(s + 0x6) = 8;
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0x1C) = *(u8 **)(a + 0x8);
        *(void **)(q + 0xC) = b;
        *(u8 **)(q + 0x10) = c;
        q[0x16] = c[0];
        *(s32 *)n->payload = 0x40;
        *(s32 *)(q + 0x4) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80174E98);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80174FC0);

/* The 0x34-byte POLY_GT4 ribbon: step an angle by 0x80 a time from
   `(s16)idx << 7', and for each step emit one Gouraud-textured quad whose four
   corners are the previous and current sin/cos scaled by the four radius
   arguments and offset by ox/oy, with the two colour words read out of *cu and
   *cv. The prims come out of D_80083160's +0x8D0 ring and the advanced cursor
   is written back. 165 words, 0 differing.

   No twin: xsibs found nothing at ratio 0.70 and the callee-set intersection
   found no landed body that calls both func_80064584 and LoadImage-era
   func_8006671C. SetPolyGT4 (0x8006671C) and AddPrim (0x8006658C) are
   splat/common_syms.txt names, which is also how the 0x34 stride reads as
   POLY_GT4.

   THE FIRST SIX PARAMETERS ARE 16-BIT, AND THEY ARE LOCALS RATHER THAN A
   NARROWED PROTOTYPE. Every one of them is spilled with `sh' and reloaded
   with `lhu', so the pseudos are HImode and unsigned. Declaring the
   parameters `u16' instead is byte-identical -- 0 differing either way, and
   the already-landed caller func_b02_80174DA0 re-verifies at 0 differing with
   either -- so the unit's existing s32 prototype is kept and the narrowing
   happens in six locals. The caller pushes its two stack arguments as words,
   which is what an s32 prototype gives. ALL SIX NARROWING ASSIGNMENTS GO
   TOGETHER, AHEAD OF THE +0x8D0 READ: with `wx' through `w3' moved below it
   the five parameter loads come out of sched2 in a different order and the
   prologue's register saves follow them -- 6 differing.

   TWO TEMPORARIES FOR *cu AND *cv, NOT ONE REUSED. With one, the two colour
   words get the same pseudo and so the same register where the target has
   $v1 and $v0, and the whole loop's callee-saved assignment rotates with it:
   14 aligned differences -- `ps'/`pc' and `q'/`co' each swap their pair.

   AND THE FOUR 0xFB V-BYTES GO AFTER THE EIGHT COORDINATE STORES, NOT WITH
   THE U-BYTES. Before them the 0xFB quantity's live range is short enough for
   local-alloc to rank it above the 1 and the 2 and hand it $t3 where the
   target chains all three constants through $v0: 5 aligned differences. The
   scheduler still emits the eight byte stores as one 1,1,2,2,FB,FB,FB,FB run
   either way.

   `*(s32 *)(q + 0x10)' IS STORED BEFORE `*(s32 *)(q + 0x4)' -- the other way
   round is 2 differences. gcc's own strength reduction supplies the second
   base register at q + 0x1A; writing that as an explicit `e = q + 0x1A' local
   is also 0 differing. */
void func_b02_801750E4(s32 a0, s32 a1, s32 a2, s32 a3, s32 ox, s32 oy, u8 *cu, u8 *cv, u8 *ot, s32 idx,
                       s32 n)
{
    Unk80083160 *g;
    u8 *q;
    s32 s;
    s32 co;
    s32 ps;
    s32 pc;
    s32 ang;
    s32 t;
    s32 t2;
    u16 w0;
    u16 w1;
    u16 w2;
    u16 w3;
    u16 wx;
    u16 wy;
    u16 m;
    u16 i;

    g = &D_80083160;
    w0 = a0;
    wx = ox;
    wy = oy;
    w1 = a1;
    w2 = a2;
    w3 = a3;
    ang = (s16)idx << 7;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    s = func_80064584(ang) >> 4;
    co = func_800644B8(ang) >> 4;
    m = n;
    for (i = idx; (s16)i < (s16)m; i += 1)
    {
        ang += 0x80;
        ps = s;
        pc = co;
        s = func_80064584(ang) >> 4;
        co = func_800644B8(ang) >> 4;
        t = *(s32 *)cu;
        *(s32 *)(q + 0x10) = t;
        *(s32 *)(q + 0x4) = t;
        t2 = *(s32 *)cv;
        *(s32 *)(q + 0x28) = t2;
        *(s32 *)(q + 0x1C) = t2;
        SetPolyGT4((POLY_GT4 *)q);
        q[0x24] = 1;
        q[0xC] = 1;
        q[0x30] = 2;
        q[0x18] = 2;
        *(s16 *)(q + 0x8) = wx + ((w2 * ps) >> 8);
        *(s16 *)(q + 0xA) = wy + ((w3 * pc) >> 8);
        *(s16 *)(q + 0x14) = wx + ((w2 * s) >> 8);
        *(s16 *)(q + 0x16) = wy + ((w3 * co) >> 8);
        *(s16 *)(q + 0x20) = wx + ((w0 * ps) >> 8);
        *(s16 *)(q + 0x22) = wy + ((w1 * pc) >> 8);
        *(s16 *)(q + 0x2C) = wx + ((w0 * s) >> 8);
        *(s16 *)(q + 0x2E) = wy + ((w1 * co) >> 8);
        q[0x19] = 0xFB;
        q[0xD] = 0xFB;
        q[0x31] = 0xFB;
        q[0x25] = 0xFB;
        *(u16 *)(q + 0x1A) = 0x133;
        AddPrim(ot, q);
        q += 0x34;
    }
    *(u8 **)(g->unk_00 + 0x8D0) = q;
}

/* The 32-element fan of semi-transparent Gouraud quads this bank's
   kind-0x202-style node carries at +0x10, the relinked twin of the resident
   overlay's func_800BABA8 (csrc/ovl_dungeon_800B6AC4.c) -- xsibs scores it
   1.000 and it is the same body with EVERY p OFFSET TWO LOWER: the element
   count is p + 0xF, the phase array p + i + 0x10, the "stick" flag p + 0xE
   and the fan's angle p + 0xC, against the twin's 0x11, 0x12, 0x10 and 0xE.
   215 words, 0 differing.

   LANDED ON THE FIRST COMPILE WITH ALL THREE OF THAT TWIN'S MEASURED
   SPELLINGS IN PLACE; none of their alternatives was re-measured on this body,
   so the numbers in the twin's own comment are the only ones. The three are:
   `s16 i = 0;' DECLARED AND INITIALISED IMMEDIATELY AFTER `g' with the loop
   written `for (; i < 0x20; i++)' -- which is what puts `addu $s5, $zero,
   $zero' at word 8, ahead of all three constant pointer locals, and so what
   makes `sp' own the shared `lui 0x1F80' with `fb' an `addu'/`ori' off it;
   `*(p + i + 0x10)' and NOT `p[i + 0x10]', for the target's index-second
   `addu $v1, $s4, $v0'; and the two RotTransPers out-parameters kept as
   CONSTANT ADDRESSES IN POINTER LOCALS rather than members of the scratchpad
   struct, which is what gives the one-word `addu $a2, $s7, $zero' per call.
   The +0x10 phase byte keeps both of its faces, as it does there: `u8' for
   the increment and `(s8)' at every test and use.

   The three scratchpad types are copied verbatim from
   csrc/ovl_dungeon_decls.h, which this unit does not include. */
s32 func_b02_80175378(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g = &D_80083160;
    s16 i = 0;
    UnkDungeonSPA *sp = (UnkDungeonSPA *)0x1F800000;
    s32 *fa = (s32 *)0x1F800090;
    s32 *fb = (s32 *)0x1F800094;
    UnkDungeonG4 *q;
    s32 n;
    s32 cc;
    s32 z;
    s32 r;
    s32 gg;
    s32 bb;

    q = *(UnkDungeonG4 **)(D_80083160.unk_00 + 0x8D0);
    p[0xF] = 0;
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    sp->unk_04 = *(u16 *)((u8 *)b + 0xA);
    for (; i < 0x20; i++)
    {
        *(p + i + 0x10) += 4;
        if ((s8) * (p + i + 0x10) >= 0x30)
        {
            if (p[0xE] != 0)
            {
                *(p + i + 0x10) = 0x30;
                continue;
            }
            *(p + i + 0x10) = 0;
        }
        q->unk_04 = 0;
        q->unk_0C = 0;
        sp->unk_70[1].vx = sp->unk_70[0].vx =
            sp->unk_00 + ((func_80064584((i + (*(s16 *)(p + 0xC) << 6)) << 7) << 4) >> 11);
        sp->unk_70[1].vy = sp->unk_70[0].vy =
            sp->unk_02 + ((func_800644B8((i + (*(s16 *)(p + 0xC) << 6)) << 7) << 4) >> 11);
        z = sp->unk_04 - (*(s8 *)(p + i + 0x10) << 1);
        sp->unk_70[0].vz = z;
        sp->unk_70[1].vz = z + 0x40;
        n = rand();
        cc = *(s8 *)(p + i + 0x10);
        if (cc > 0x20)
        {
            n = (s16)n / (cc - 0x20);
        }
        else if (cc < 0x10)
        {
            n = (s16)n / (0x10 - cc);
        }
        r = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x1) != 0)
        {
            r = n;
        }
        q->unk_14 = r;
        q->unk_1C = r;
        gg = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4) != 0)
        {
            gg = n;
        }
        q->unk_15 = gg;
        q->unk_1D = gg;
        bb = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x2) != 0)
        {
            bb = n;
        }
        q->unk_16 = bb;
        q->unk_1E = bb;
        sp->unk_C0 = RotTransPers(&sp->unk_70[0], &q->unk_08, fa, fb);
        q->unk_10.w = q->unk_08;
        q->unk_10.h[0] += 4;
        sp->unk_C0 += RotTransPers(&sp->unk_70[1], &q->unk_18, fa, fb);
        q->unk_20.w = q->unk_18;
        q->unk_20.h[0] += 2;
        sp->unk_C0 >>= 1;
        *((u8 *)q + 0x3) = 8;
        *((u8 *)q + 0x7) = 0x3A;
        AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
        q++;
        func_80067F20((u32 *)q, 0, 0, 0x60, 0);
        AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
        q = (UnkDungeonG4 *)((u8 *)q + 0xC);
        p[0xF]++;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_801756D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80175814);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80175878);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_801759CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b02", func_b02_80175B1C);

/* Spawn this bank's kind-0x202 node under the slot itself and arm it with the
   bank-local func_b02_80175B1C handler and the D_80175FA8 sprite table.
   49 words, 0 differing.

   The twin is the 54-word func_800A5A18 (csrc/ovl_dungeon_800A4B88.c) at
   xsibs ratio 0.796, and its own comment's fact -- TWO POINTER LOCALS, NOT
   ONE REUSED, one for the +0x0C renderable and one for the payload base --
   held here. Four deltas from it, all read off the listing: the kind is 0x202
   and the pool argument is `p - 0x20' rather than D_80083498, the renderer's
   +0x1E is 0x800 against its +0x1C's 0x1000 (the twin writes 0x1000 to both),
   the payload stores are the slot pointer at +0x00 plus +0xA/+0xC where the
   twin has +0x04/+0x66, and there is no func_800A56E0 tail call.

   `*(s16 *)(q + 0xA)' MUST BE WRITTEN BEFORE `*(s16 *)(q + 0xC)' EVEN THOUGH
   THE TARGET EMITS THEM THE OTHER WAY ROUND. The `lhu' of p + 0x2A has to be
   hoisted ahead of the 0x30 constant to cover its own load delay, and the
   reading is that this is what makes the two values' live ranges overlap and
   so what puts p + 0x2A in $a2 and leaves $v0 to the 0x30. What is MEASURED is
   the cost: with +0xC written first both take $v0 and the 0x1000340/0x200020
   pair comes out in $a3/$a2 instead of the target's $t0/$a3 -- 49 words,
   18 differing. `v[1]' assigned before `v[0]' is 14 differing on its own. */
void func_b02_80175D7C(u8 *p)
{
    UnkNode *n;
    u8 *s;
    u8 *q;
    s32 v[2];

    n = func_8003FD64(0x202, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b02_80175B1C;
        func_8004491C(n, (u32)D_80045340);
        s = (u8 *)n->unk_0C;
        *(u8 **)(s + 0x8) = D_80175FA8;
        *(s16 *)(s + 0x1C) = 0x1000;
        *(s16 *)(s + 0x1E) = 0x800;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0xA) = *(u16 *)(p + 0x2A);
        *(s16 *)(q + 0xC) = 0x30;
        v[0] = 0x1000340;
        v[1] = 0x200020;
        func_800B835C(D_80175FB4, v, 1, 0);
    }
}
