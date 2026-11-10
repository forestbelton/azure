#include "common.h"
#include "main.h"
#include "strings.h"

/* card_win.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 103..121. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine". */

void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n)
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
    p->unk_04->unk_08 = -0xA0;
    p->unk_04->unk_0A = n * 24 - 0x78;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

void func_80023B18(UnkCardGrp240 *c)
{
    UnkCardPair *q = c->ptrs[1];
    s32 k;

    k = get_free_blocks() >= 3;
    q->unk_00 = (s32)func_8004DA74((u8 *)c + 8, D_800282A4[k], 1);
    q->unk_04->unk_08 = 0xF0;
    q->unk_04->unk_0A = 0x91;
    q = c->ptrs[5];
    q->unk_04->unk_08 = 0x70;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_800791A4;
    q = c->ptrs[7];
    q->unk_00 = (s32)((u8 *)c + 0x134);
    q->unk_04->unk_08 = 0xAE;
    q->unk_04->unk_0A = 0x18;
}

s32 func_80023BE4(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

/* Four UnkCardPair slots set up in a row, each `e->unk_00` plus the x and y
   in its UnkCardPrim. Two things are load-bearing:

   `e = c->ptrs[4]` (word 155, when this read `p[155]`) must be assigned
   BEFORE the func_8004E5A0 / func_8004E69C pair, not after: the target loads
   $s3 at word 39, ahead of both calls, and with the load after them the
   function is 71 words instead of 72 (gcc fills the p[0] load-delay slot with
   the `li $a1, 4` that the target has already issued back at word 27).

   The second table access has to be spelled as a BYTE offset. The target
   advances the D_80083EA4 base register in place at word 37
   (`addu $s0, $s0, $v0`), which gives that pseudo a fourth reference and so
   wins it $s0 over `p`; `D_80083EA4[p[0]][8]` computes the sum into a
   scratch instead and leaves $s0/$s1 swapped through the whole function (18
   DIFF). A named `s32 (*tbl)[32]` local with `tbl += p[0]` gets the in-place
   update but hoists the `lui`/`addiu` into the prologue, which is worse
   (23). */
void func_80023C44(UnkCardGrp240 *c)
{
    u8 buf[8];
    UnkCardPair *e;
    s32 *p = (s32 *)c;

    e = c->ptrs[5];
    e->unk_04->unk_08 = 0x70;
    e->unk_04->unk_0A = 0x10;
    e->unk_00 = (s32)D_80078FD0;

    e = c->ptrs[1];
    e->unk_00 = (s32)func_8004DA74((u8 *)(p + 2), (u8 *)D_80083EA4[p[0]], 1);
    e->unk_04->unk_08 = 0xF0;
    e->unk_04->unk_0A = 0x91;

    e = c->ptrs[4];
    func_8004E5A0(*(s32 *)((u8 *)D_80083EA4[p[0]] + 0x20), 4, buf);
    func_8004E69C(buf);
    e->unk_00 = (s32)func_8004E298((u8 *)(p + 0x41), buf, 1);
    e->unk_04->unk_08 = 0x147;
    e->unk_04->unk_0A = 0x91;

    e = c->ptrs[7];
    e->unk_00 = (s32)(p + 77);
    e->unk_04->unk_08 = 0xAE;
    e->unk_04->unk_0A = 0x18;
}

void func_80023D64(UnkCardGrp240 *c)
{
    UnkCardPair *q;

    q = c->ptrs[6];
    q->unk_04->unk_08 = 0xFA;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_80027F7C;
    q = c->ptrs[3];
    q->unk_00 = (s32)func_8004DA74((u8 *)c + 0xA4, D_800282B0, 1);
    q->unk_04->unk_08 = 0x178;
    q->unk_04->unk_0A = 0x90;
    q = c->ptrs[8];
    q->unk_00 = (s32)((u8 *)c + 0x1AC);
    q->unk_04->unk_08 = 0x10C;
    q->unk_04->unk_0A = 0x18;
}

void func_80023E14(UnkCardGrp240 *c)
{
    s32(*tbl)[32];
    s32 *p = (s32 *)c;

    func_80023BE4((s32)(p + 0x4D), 0x80, 0x18);
    func_80023BE4((s32)(p + 0x6B), 0x2A, 0x14);
    tbl = D_80083E98;
    if (tbl[p[0]][0] != 0)
    {
        func_80023C44(c);
        if (((s16 *)tbl[p[0]])[4] != 0)
        {
            func_80023D64(c);
        }
    }
    else
    {
        func_80023B18(c);
    }
}

void func_80023EC4(s32 *p)
{
    p[-4] = 0;
}

s32 func_80023ECC(s32 a)
{
    return func_800491F4(a, D_80027FC4, 0xA);
}

void func_80023EF4(UnkCardGrp240 *c, s32 n)
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

void func_80023F94(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

void func_80023FA0(UnkCardGrp240 *c, s32 a, s32 b)
{
    func_80023F94((s32 *)c, a, b);
    func_80023A78(&c->root, c->root_prim, c->root_shared, b);
    func_80023E14(c);
}

void func_80023EF4(UnkCardGrp240 *c, s32 n);

/* The func_800238D4 constructor idiom at an INDEXED row: D_80029618 is an
   array of 0x3E4-byte rows, and gcc synthesises the 996 multiply as
   `(((b << 5) - b) << 3) + b) << 2`. func_80023ECC had to become `s32` with
   a `return` -- its value lands in r[150] here -- which leaves its own ten
   words unchanged. `q[3] = ...` must come BEFORE the func_80023ECC call:
   reversed, the function grows to 64 words. */
s32 func_80023FF0(s32 a, s32 b)
{
    s32 *q = (s32 *)D_80029618[b];
    s32 *r = q + 8;
    UnkCardGrp240 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, (u8 *)q, 0xF9);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024184(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x3C4);
    }
    c = (UnkCardGrp240 *)r;
    func_80023EF4(c, 0xA);
    q[3] = (s32)&c->root;
    c->root.unk_0C = func_80023ECC((s32)c->ptrs);
    func_80023FA0(c, a, b);
    q[4] = (s32)func_80023EC4;
    return (s32)q;
}

void func_800240EC(UnkCardGrp240 *c)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        func_8004E994(c->ptrs[off + 1]->unk_00);
        c->ptrs[off + 1]->unk_00 = 0;
        off++;
    } while (++i < 3);
    c->ptrs[4]->unk_00 = 0;
}

void func_80024160(UnkCardGrp240 *c)
{
    s32 i = 0;

    do
    {
        c->ptrs[i]->unk_00 = 0;
    } while (++i < 10);
}

void func_80024184(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC((UnkCardGrp240 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_800241D4(s32 *p, s32 flag)
{
    UnkCardGrp240 *c;
    UnkCardPair *r;

    if (p != 0)
    {
        c = (UnkCardGrp240 *)(p + 8);
        r = c->ptrs[9];
        c->root_prim[0] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_800786E8;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x60;
        r->unk_04->unk_0A = 0x18;
    }
}

void func_80024224(s32 *p, s32 flag)
{
    UnkCardGrp240 *c;
    UnkCardPair *r;

    if (p != 0)
    {
        c = (UnkCardGrp240 *)(p + 8);
        r = c->ptrs[9];
        c->root_prim[0] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_800786DC;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x66;
        r->unk_04->unk_0A = 0x18;
    }
}

void func_80024274(s32 *p)
{
    UnkCardGrp240 *c;

    if (p != 0)
    {
        c = (UnkCardGrp240 *)(p + 8);
        c->root_prim[0] = 0x606868;
        c->ptrs[9]->unk_00 = 0;
    }
}

void func_80024298(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC((UnkCardGrp240 *)q);
        func_80024160((UnkCardGrp240 *)q);
        func_80023E14((UnkCardGrp240 *)q);
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

/* 0x80027F7C */
/* Six 12-byte cel records, stored by address into a primitive by
   func_80023D64 (`q->unk_00 = (s32)D_80027F7C`). */
u8 D_80027F7C[0x48] = {0x41, 0x2C, 0x02, 0xFC, 0x17, 0x00, 0xC0, 0x7C, 0x10, 0x00, 0x08, 0x10, 0x41, 0x2C, 0x02, 0x04,
                       0x17, 0x00, 0xC0, 0x7C, 0x10, 0x10, 0x08, 0x10, 0x40, 0x2C, 0x26, 0xFC, 0x17, 0x00, 0xC0, 0x7C,
                       0x10, 0x00, 0x08, 0x10, 0x40, 0x2C, 0x26, 0x04, 0x17, 0x00, 0xC0, 0x7C, 0x10, 0x10, 0x08, 0x10,
                       0x40, 0x2C, 0xF1, 0x00, 0x17, 0x00, 0xC0, 0x7C, 0x68, 0x20, 0x08, 0x10, 0xC0, 0x2C, 0x2A, 0xFC,
                       0x17, 0x00, 0xC1, 0x7C, 0x20, 0x00, 0x08, 0x08};

/* 0x80027FC4 */
/* func_800491F4 index list for func_80023ECC, 9 entries then the 0x0A
   terminator. */
u8 D_80027FC4[0xC] = {0x07, 0x08, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x09, 0x0A, 0x00, 0x94};
