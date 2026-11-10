#include "common.h"
#include "main.h"
#include "strings.h"

/* cardsme.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 206..227. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine". */

void func_80027018(UnkCardGrp0D4 *c)
{
    c->ptrs[0]->unk_00 = (s32)D_80077EF0;
    c->ptrs[1]->unk_00 = (s32)D_80077EFC;
    c->ptrs[2]->unk_00 = (s32)((s32 *)c + 1);
    c->ptrs[3]->unk_00 = (s32)((s32 *)c + 0x1F);
}

void func_80027054(u8 *p)
{
    func_8004DA74(p + 0x7C, D_800282CC, 1);
}

s32 func_80027080(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

void func_800270E0(UnkCardGrp0D4 *c)
{
    func_80027054((u8 *)c);
    func_80027080((s32)((s32 *)c + 1), 0x36, 0x10);
    func_80027018(c);
    c->ptrs[3]->unk_04->unk_08 = 0x8E;
    c->ptrs[3]->unk_04->unk_0A = 0x82;
    c->ptrs[0]->unk_04->unk_08 = 9;
    c->ptrs[0]->unk_04->unk_0A = 0xA;
    c->ptrs[1]->unk_04->unk_08 = 0x42;
    c->ptrs[1]->unk_04->unk_0A = 0xA;
    c->ptrs[2]->unk_04->unk_08 = 0x24;
    c->ptrs[2]->unk_04->unk_0A = 0xA;
}

void func_800271C0(UnkCardPair *p, u32 *a, u32 *b)
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

s32 func_80027254(s32 a)
{
    return func_800491F4(a, D_80028094, 4);
}

void func_8002727C(UnkCardGrp0D4 *c, s32 n)
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

void func_8002731C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80027324(UnkCardGrp0D4 *c, s32 v)
{
    func_8002731C((s32 *)c, v);
    func_800271C0(&c->root, c->root_prim, c->root_shared);
    func_800270E0(c);
}

s32 *func_80027364(s32 *p)
{
    s32 *q = (s32 *)D_8002B968;
    s32 *r = q + 8;
    UnkCardGrp0D4 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B968, 0x68);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x180);
    }
    c = (UnkCardGrp0D4 *)r;
    func_8002727C(c, 4);
    q[3] = (s32)&c->root;
    c->root.unk_0C = func_80027254((s32)c->ptrs);
    func_80027324(c, (s32)p);
    return q;
}

void func_8002742C(UnkCardGrp0D4 *c)
{
    func_8004E994(c->ptrs[3]->unk_00);
}

void func_80027458(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_8002742C((UnkCardGrp0D4 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_800274A8(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[11];
    }
}

void func_800274D8(s32 *p)
{
    func_800274A8(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002789C;
    }
}

/* The two calls take the SAME local, which is the whole function: gcc knows
   `v` is 0 in the true arm and materialises `addu $a1, $zero, $zero` there,
   and in the false arm `v` is already in $a1 from the test, so nothing is
   emitted. func_80025C7C needs a second parameter declared for this -- it
   ignores it, so its own 46 words are unchanged, and it had no other
   caller. */
void func_80027528(s32 *p)
{
    s32 v;

    func_80027C90(p - 8);
    v = p[0];
    if (v == 0)
    {
        func_80025C7C((s32 *)p[5], v);
    }
    else
    {
        func_80026868((s32 *)p[5], v);
    }
}

void func_80027584(s32 *p)
{
    s32 i = 0;
    s32 r;

    do
    {
        r = format_card(p[3]);
        if (r != 5)
        {
            break;
        }
    } while (i++ < 6);
    p[14] = 0;
    recheck_card_slot();
    if (r == 1)
    {
        p[11] = (s32)func_8002789C;
        msg_format_done((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
    else if (r == 4)
    {
        p[11] = (s32)func_8002789C;
        msg_no_card((s32)(p - 8));
        p[-4] = (s32)func_800274A8;
    }
    else
    {
        p[11] = (s32)func_8002789C;
        msg_format_failed((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
}

void func_8002765C(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80027584;
    }
}

void func_80027684(s32 *p)
{
    s32 h;

    if (p[15] != 0)
    {
        h = (s32)func_8002789C;
    }
    else
    {
        p[16] = 1;
        msg_formatting((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
        h = (s32)func_8002765C;
    }
    p[-4] = h;
}

void func_800276EC(s32 *p)
{
    p[11] = (s32)func_80027684;
    msg_format_confirm((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_800274D8;
}

void func_80027734(s32 *p)
{
    p[11] = (s32)func_8002789C;
    msg_no_card((s32)(p - 8));
    p[-4] = (s32)func_800274A8;
}

void func_80027778(s32 *p)
{
    p[11] = (s32)func_8002789C;
    msg_card_damaged((s32)(p - 8));
    p[-4] = (s32)func_800274D8;
}

void func_800277BC(s32 *p)
{
    s32 n;
    void (*h)(s32 *);

    if (D_80083160.unk_08 != 0 && (D_80083160.unk_10 & 0x20) != 0)
    {
        switch_card_slot();
        p[14] = 0;
        p[-4] = (s32)func_8002789C;
        return;
    }
    n = func_8002168C();
    print_card_status();
    if (n != 0)
    {
        p[14] = 0;
        p[12] = n;
        h = D_8002809C[n];
        p[13] = n;
        p[-4] = (s32)h;
        return;
    }
    if (get_card_st() == 3 && p[14] == 0)
    {
        msg_please_wait((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
    }
}
