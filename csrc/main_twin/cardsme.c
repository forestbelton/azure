#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_80400908(void);
extern void func_80400948(void);
extern void func_804030CC(s32 a, s32 p);
extern void func_8040316C(s32 a, s32 p);

/* THE 0x80400000 DEBUG TWIN -- cardsme.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x804007D8..0x8040086C   .text from 0x8040789C

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main_twin/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- cardsme.c's assert pair. --- */
/* 0x804007D8 */
const char D_804007D8[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400800 */
const char D_80400800[12] __attribute__((section(".rodata"))) = "cardsme.c\0\x03";

/* 0x8040080C  the six slot-state names, D_804090C8[5]..[0] and D_804090F8's copy */
const char D_8040080C[8] __attribute__((section(".rodata"))) = "ERROR";

/* 0x80400814 */
const char D_80400814[8] __attribute__((section(".rodata"))) = "NOTHING";

/* 0x8040081C */
const char D_8040081C[12] __attribute__((section(".rodata"))) = "UN FORMAT";

/* 0x80400828 */
const char D_80400828[12] __attribute__((section(".rodata"))) = "NEW READY";

/* 0x80400834 */
const char D_80400834[8] __attribute__((section(".rodata"))) = "READY";

/* 0x8040083C */
const char D_8040083C[12] __attribute__((section(".rodata"))) = "NOW CHECK";

/* 0x80400848 */
const char D_80400848[12] __attribute__((section(".rodata"))) = "CARD%d %s\n";

/* 0x80400854 */
const char D_80400854[12] __attribute__((section(".rodata"))) = "CARD1 %s\n";

/* 0x80400860 */
const char D_80400860[12] __attribute__((section(".rodata"))) = "CARD2 %s\n";

/* --- memory.c's assert pair, and the section ends inside the filename's
   object: see the block header. --- */


void func_8040789C(UnkCardGrp0D4 *c)
{
    c->ptrs[0]->unk_00 = (s32)D_80083060;
    c->ptrs[1]->unk_00 = (s32)D_8008306C;
    c->ptrs[2]->unk_00 = (s32)((s32 *)c + 1);
    c->ptrs[3]->unk_00 = (s32)((s32 *)c + 0x1F);
}

void func_804078D8(u8 *p)
{
    func_80051B50(p + 0x7C, D_80408D6C, 1);
}

s32 func_80407904(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

void func_80407964(s32 *p)
{
    func_804078D8((u8 *)p);
    func_80407904((s32)(p + 1), 0x36, 0x10);
    func_8040789C(p);
    ((UnkCardPair *)p[0x3F])->unk_04->unk_08 = 0x8E;
    ((UnkCardPair *)p[0x3F])->unk_04->unk_0A = 0x82;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_08 = 9;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_08 = 0x42;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_08 = 0x24;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_0A = 0xA;
}

void func_80407A44(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_80407AEC(s32 a)
{
    return func_80047358(a, D_80408D74, 4);
}

/* Live func_8002727C, the UnkTwinGrp0D4 instance of the five-way template builder. The
 * group is 0xD4 bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80407B14(UnkTwinGrp0D4 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80407BC8(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80407BD0(s32 *p, s32 v)
{
    func_80407BC8(p, v);
    func_80407A44((UnkCardPair *)(p + 0x38), (u32 *)(p + 0x2E), (u32 *)(p + 0x32));
    func_80407964(p);
}

/* Live func_80027364, a group allocator of the same family as
 * func_804057F0 and the rest, with the assert this build carries. The
 * UnkTwinGrp0D4's root is at r[0x38], its unk_0C at r[0x3B], ptrs at
 * r[0x3C]. */
s32 *func_80407C10(s32 *p)
{
    s32 *q = (s32 *)D_8040C420;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C420, 0x68);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804007D8, D_80400800, 0xEE);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80403D24(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x180);
    }
    func_80407B14((UnkTwinGrp0D4 *)r, 4);
    q[3] = (s32)(r + 0x38);
    r[0x3B] = func_80407AEC((s32)(r + 0x3C));
    func_80407BD0(r, (s32)p);
    return q;
}

void func_80407D0C(s32 *p)
{
    func_80051FBC(*(s32 *)p[0x3F]);
}

void func_80407D38(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80407D0C((UnkCardGrp0D4 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80407D90(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[11];
    }
}

void func_80407DB8(s32 *p)
{
    func_80407D90(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_804081AC;
    }
}

void func_80407E08(s32 *p)
{
    s32 v;

    func_804084DC(p - 8);
    v = p[0];
    if (v == 0)
    {
        func_80406678w((s32 *)p[5]);
    }
    else
    {
        func_8040701C((s32 *)p[5], v);
    }
}

void func_80407E64(s32 *p)
{
    s32 i = 0;
    s32 r;

    do
    {
        r = func_80401B30(p[3]);
        if (r != 5)
        {
            break;
        }
    } while (i++ < 6);
    p[14] = 0;
    func_80400908();
    if (r == 1)
    {
        p[11] = (s32)func_804081AC;
        func_804032AC((s32)(p - 8));
        p[-4] = (s32)func_80407DB8;
    }
    else if (r == 4)
    {
        p[11] = (s32)func_804081AC;
        func_804030F4((s32)(p - 8));
        p[-4] = (s32)func_80407D90;
    }
    else
    {
        p[11] = (s32)func_804081AC;
        func_80403284((s32)(p - 8));
        p[-4] = (s32)func_80407DB8;
    }
}

void func_80407F40(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80407E64;
    }
}

void func_80407F68(s32 *p)
{
    s32 h;

    if (p[15] != 0)
    {
        h = (s32)func_804081AC;
    }
    else
    {
        p[16] = 1;
        func_8040316C((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
        h = (s32)func_80407F40;
    }
    p[-4] = h;
}

void func_80407FD4(s32 *p)
{
    p[11] = (s32)func_80407F68;
    func_804030CC((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_80407DB8;
}

void func_8040801C(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804030F4((s32)(p - 8));
    p[-4] = (s32)func_80407D90;
}

void func_80408060(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804032D4((s32)(p - 8));
    p[-4] = (s32)func_80407DB8;
}

void func_804080A4(s32 *p)
{
    s32 st;

    if (D_801379A8.unk_00 != 0 && (D_801379B0.unk_00 & 0x40) != 0)
    {
        func_80400948();
        p[14] = 0;
        p[-4] = (s32)func_804081AC;
    }
    else
    {
        st = func_804016D0();
        func_8003FA78(D_80400848, p[3], D_804090C8[st]);
        func_804018FC();
        if (st != 0)
        {
            p[14] = 0;
            p[12] = st;
            p[-4] = D_804090E0[st];
            p[13] = st;
        }
        else if (func_804010E0() == 3 && p[14] == 0)
        {
            func_8040311C((s32)(p - 8), (s32)(p + 14));
            p[14] = 1;
        }
    }
}
