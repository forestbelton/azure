#include "common.h"
#include "main.h"
#include "strings.h"

/* card_opt.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 88..102. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine". */

void func_80023258(s32 *p)
{
    s32 w = p[35];
    s32 d = ((p[34] - w) * (p[33] << 4)) / p[32] + 0xA;
    s32 i;

    ((UnkCardGrp184 *)p)->ptrs[1]->unk_04->unk_0A = (w << 4) + d;
    for (i = 0; i < 3; i++)
    {
        s32 v = ((u8 *)p)[0x7C + i];
        s32 u;
        s32 n;
        u8 *q = (u8 *)p + i + 0x7C;

        if (i == p[34])
        {
            n = v + (8 - v) / (p[32] - p[33] + 1);
        }
        else
        {
            n = v;
            if (n != 0)
            {
                n -= 1;
            }
        }
        *q = n;
        u = ((u8 *)p)[0x7C + i];
        ((UnkCardGrp184 *)p)->ptrs[10 + i]->unk_04->unk_08 = u + 0x24;
        ((UnkCardGrp184 *)p)->ptrs[14 + i]->unk_04->unk_08 = u + 0x8F;
        ((UnkCardGrp184 *)p)->ptrs[2 + i]->unk_04->unk_08 = u + 9;
        ((UnkCardGrp184 *)p)->ptrs[6 + i]->unk_04->unk_08 = u + 0x42;
        ((u8 *)((UnkCardGrp184 *)p)->ptrs[10 + i]->unk_04)[0] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardGrp184 *)p)->ptrs[10 + i]->unk_04)[1] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkCardGrp184 *)p)->ptrs[10 + i]->unk_04)[2] = ((u8 *)p)[0x7C + i] * 4 + 0x60;
    }
    if (p[33] < p[32])
    {
        p[33]++;
    }
}

void func_800233D0(UnkCardGrp184 *c)
{
    s32 *p = (s32 *)c;
    s32 i;

    c->ptrs[0]->unk_00 = func_80049E6C(3);
    c->ptrs[1]->unk_00 = (s32)D_80077E84;
    for (i = 0; i < 3; i++)
    {
        c->ptrs[2 + i]->unk_00 = (s32)D_80077EF0;
        c->ptrs[6 + i]->unk_00 = (s32)D_80077EFC;
        c->ptrs[10 + i]->unk_00 = (s32)(p + 1);
        c->ptrs[14 + i]->unk_00 = (s32)((u8 *)p + 0x90 + i * 0x48);
    }
}

void func_80023470(UnkCardGrp184 *c)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        c->ptrs[10 + i]->unk_04->unk_08 = 0x24;
        ((u8 *)c->ptrs[10 + i]->unk_04)[0] = 0x68;
        ((u8 *)c->ptrs[10 + i]->unk_04)[1] = 0x68;
        ((u8 *)c->ptrs[10 + i]->unk_04)[2] = 0x60;
        c->ptrs[2 + i]->unk_04->unk_08 = 9;
        c->ptrs[6 + i]->unk_04->unk_08 = 0x42;
        c->ptrs[14 + i]->unk_04->unk_08 = 0x8F;
    }
}

void func_8002352C(u8 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004DA74(p + 0x90 + i * 0x48, D_800282BC[i], 1);
    }
}

s32 func_800235A0(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

/* Four sprite slots per row, three rows, each x and y written through a
   freshly reloaded `p[k + i]` -- gcc cannot CSE the second store's base
   because the first store may alias it, so spelling each store as its own
   full expression is what produces the doubled `lw` pairs. A `do`/`while`
   here is 78 words against 76. */
void func_80023600(UnkCardGrp184 *c)
{
    s32 *p = (s32 *)c;
    s32 i;

    p[32] = 1;
    p[33] = 1;
    func_8002352C((u8 *)p);
    func_800235A0((s32)(p + 1), 0x36, 0x10);
    func_800233D0(c);
    c->ptrs[0]->unk_04->unk_0A = 0x1A;
    for (i = 0; i < 3; i++)
    {
        c->ptrs[14 + i]->unk_04->unk_08 = 0x8E;
        c->ptrs[14 + i]->unk_04->unk_0A = (i * 16) + 0x82;
        c->ptrs[2 + i]->unk_04->unk_08 = 9;
        c->ptrs[2 + i]->unk_04->unk_0A = (i * 16) + 0xA;
        c->ptrs[6 + i]->unk_04->unk_08 = 0x42;
        c->ptrs[6 + i]->unk_04->unk_0A = (i * 16) + 0xA;
        c->ptrs[10 + i]->unk_04->unk_08 = 0x24;
        c->ptrs[10 + i]->unk_04->unk_0A = (i * 16) + 0xA;
    }
    func_80023470(c);
}

void func_80023730(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

s32 func_800237C4(s32 a)
{
    return func_800491F4(a, D_80027F68, 0x12);
}

void func_800237EC(UnkCardGrp184 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_8002388C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80023894(UnkCardGrp184 *c, s32 v)
{
    func_8002388C((s32 *)c, v);
    func_80023730(&c->root, c->root_prim, c->root_shared);
    func_80023600(c);
}

s32 *func_800238D4(s32 *p)
{
    s32 *q = (s32 *)D_800291D0;
    s32 *r = q + 8;
    UnkCardGrp184 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800291D0, 0x112);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x428);
    }
    c = (UnkCardGrp184 *)r;
    func_800237EC(c, 0x12);
    q[3] = (s32)&c->root;
    c->root.unk_0C = func_800237C4((s32)c->ptrs);
    func_80023894(c, (s32)p);
    q[4] = (s32)func_80023258;
    func_80023258(r);
    return q;
}

void func_800239B0(UnkCardGrp184 *c)
{
    s32 i = 0;

    do
    {
        func_8004E994(c->ptrs[14 + i]->unk_00);
    } while (++i < 3);
}

void func_80023A00(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800239B0((UnkCardGrp184 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80023A50(s32 *p, s32 v)
{
    s32 *q;
    s32 t;

    if (p != 0)
    {
        q = p + 8;
        t = q[34];
        q[33] = 0;
        q[32] = 3;
        q[34] = v;
        q[35] = t;
    }
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S .data, moved here from csrc/ovl_main_80020000.c on 2026-11-10.
   The overlay's first data run, 0x80027E20..0x800280B4, partitions over the
   nine units with zero order violations; docs/overlays.md, "Rodata is per
   unit; so is data", has the argument. Definition order is address order, and
   the block sits at the END of the file on purpose: every function above it
   reaches these through main.h's declaration, and a definition earlier in the
   file would complete the type at those sites and could move codegen.
   --------------------------------------------------------------------------- */

/* 0x80027F68 */
/* func_800491F4 index list for func_800237C4, 18 entries then the 0x12
   terminator. */
u8 D_80027F68[0x14] = {0x00, 0x01, 0x0A, 0x0B, 0x0C, 0x0D, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0E, 0x0F,
                       0x10, 0x11, 0x12, 0x00};
