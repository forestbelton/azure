#include "common.h"
#include "main_twin.h"

/* THE 0x80400000 DEBUG TWIN -- card_win.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x804005F8..0x8040062C   .text from 0x80403DA0

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main_twin/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- card_win.c's assert pair, and nothing else in this unit's rodata. --- */
/* 0x804005F8 */
const char D_804005F8[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400620 */
const char D_80400620[12] __attribute__((section(".rodata"))) = "card_win.c\0\xAE";


void func_80403DA0(UnkCardPair *p, u32 *a, u32 *b, s32 n)
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
    p->unk_04->unk_08 = -0xA0;
    p->unk_04->unk_0A = n * 24 - 0x78;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* Live func_80023B18. Four differences, all ONE revision constant: this
 * build's UnkCardGrp240 is 0x84 bytes smaller, so `ptrs[1]`, `ptrs[5]` and
 * `ptrs[7]` sit at 0x1DC, 0x1EC and 0x1F4 where the live struct has 0x260,
 * 0x270 and 0x278, and the embedded buffer the last pair points at is at
 * +0xB0 rather than +0x134. Raw word indices off p, as func_804040C8 and
 * func_80404668 use. */
void func_80403E54(s32 *p)
{
    UnkCardPair *q = (UnkCardPair *)p[0x77];
    s32 k;

    k = func_80402268() >= 3;
    q->unk_00 = (s32)func_80051B50((u8 *)p + 8, D_80408CB0[k], 1);
    q->unk_04->unk_08 = 0xF0;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x7B];
    q->unk_04->unk_08 = 0x70;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_8008432C;
    q = (UnkCardPair *)p[0x7D];
    q->unk_00 = (s32)((u8 *)p + 0xB0);
    q->unk_04->unk_08 = 0xAE;
    q->unk_04->unk_0A = 0x18;
}

s32 func_80403F1C(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

/* Live func_80023C44, with FIVE groups rather than four and the save-slot
 * row reached directly -- `&D_8009DDD8[p[0]][3]` where the live one loads a
 * pointer out of D_80083EA4. */
void func_80403F7C(s32 *p)
{
    u8 buf[8];
    UnkCardPair *e;

    e = (UnkCardPair *)p[0x7B];
    e->unk_04->unk_08 = 0x70;
    e->unk_04->unk_0A = 0x10;
    e->unk_00 = (s32)D_80084158;

    e = (UnkCardPair *)p[0x77];
    e->unk_00 = (s32)func_80051B50((u8 *)(p + 2),
                                   (u8 *)&D_8009DDD8[p[0]][3], 1);
    e->unk_04->unk_08 = 0xF0;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x78];
    e->unk_00 = (s32)func_80051B50((u8 *)p + 0x50, D_804005EC, 1);
    e->unk_04->unk_08 = 0x160;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x7A];
    func_80051804(D_8009DDD8[p[0]][0xB], 4, buf);
    func_80051900(buf);
    e->unk_00 = (s32)func_80051520((u8 *)p + 0x80, buf, 1);
    e->unk_04->unk_08 = 0x13F;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[0x7D];
    e->unk_00 = (s32)((u8 *)p + 0xB0);
    e->unk_04->unk_08 = 0xAE;
    e->unk_04->unk_0A = 0x18;
}

/* Live func_80023D64. Its UnkCardGrp240 `c->ptrs[N]` reads are spelled as raw
 * word indices here because this build's ptrs array sits 0x21 WORDS lower --
 * ptrs[6] is p[0x7C] against the live 0x9D -- which is the same -0x21 delta
 * func_80404314 and func_804044D8 carry. Four of the constants are revision
 * differences too: 0x100 for 0xFA, 0x181 for 0x178, 0x91 for 0x90, 0x112 for
 * 0x10C; 0x10 and 0x18 are shared. The byte offsets differ independently --
 * 0x5C for 0xA4, 0x128 for 0x1AC. */
void func_804040C8(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x7C];
    q->unk_04->unk_08 = 0x100;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_80084110;
    q = (UnkCardPair *)p[0x79];
    q->unk_00 = (s32)func_80051B50((u8 *)p + 0x5C, D_804005F0, 1);
    q->unk_04->unk_08 = 0x181;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x7E];
    q->unk_00 = (s32)((u8 *)p + 0x128);
    q->unk_04->unk_08 = 0x112;
    q->unk_04->unk_0A = 0x18;
}

void func_80404178(s32 *p)
{
    func_80403F1C((s32)(p + 0x2C), 0x80, 0x18);
    func_80403F1C((s32)(p + 0x4A), 0x2C, 0x14);
    if (D_8009DDD8[p[0]][0] != 0)
    {
        func_80403F7C(p);
        if (D_8009DDD8[p[0]][2] != 0)
        {
            func_804040C8(p);
        }
    }
    else
    {
        func_80403E54(p);
    }
}

void func_80404224(s32 *p)
{
    p[-4] = 0;
}

s32 func_8040422C(s32 a)
{
    return func_80047358(a, D_80408CB8, 0xA);
}

/* Live func_80023EF4, the UnkTwinGrp1BC instance of the five-way template builder. The
 * group is 0x1BC bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80404254(UnkTwinGrp1BC *c, s32 n)
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

void func_80404308(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

void func_80404314(s32 *p, s32 a, s32 b)
{
    func_80404308(p, a, b);
    func_80403DA0((UnkCardPair *)(p + 0x72), (u32 *)(p + 0x68), (u32 *)(p + 0x6C), b);
    func_80404178(p);
}

/* One of five 0x360-byte groups, picked by index, with THREE asserts the
 * live func_80023FF0 has no counterpart for: the index is bounds-checked at
 * both ends before the group is touched. Raw word indices off r: root at
 * r[0x72], its unk_0C at r[0x75], ptrs at r[0x76].
 *
 * It needed func_8007BEF0 declared NORETURN. Without it `n` outranks `r` in
 * global.c's allocno order and the two take $s1 and $s2 the other way round
 * -- 16 differing with every other word already right -- because the three
 * assert arms look like ordinary calls that come back, which lengthens every
 * live range that spans them. */
s32 *func_80404364(s32 a, s32 n)
{
    s32 *q = (s32 *)D_8040A0F0[n];
    s32 *r = q + 8;

    if (n >= 5)
    {
        __asm__ volatile("");
        func_8007C040(D_804005F8, D_80400620, 0x12B);
        func_8007BEF0(1);
    }
    if (n < 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804005F8, D_80400620, 0x12C);
        func_8007BEF0(1);
    }
    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, (u8 *)q, 0xD8);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804005F8, D_80400620, 0x12F);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80404570(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x340);
    }
    func_80404254(r, 0xA);
    q[3] = (s32)(r + 0x72);
    r[0x75] = func_8040422C((s32)(r + 0x76));
    func_80404314(r, a, n);
    q[4] = (s32)func_80404224;
    return q;
}

void func_804044D8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        func_80051FBC(*(s32 *)p[off + 0x77]);
        *(s32 *)p[off + 0x77] = 0;
        off++;
    } while (++i < 3);
    *(s32 *)p[0x7A] = 0;
}

void func_8040454C(s32 **p)
{
    s32 i = 0;

    do
    {
        *p[0x76] = 0;
        p++;
    } while (++i < 10);
}

void func_80404570(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_804044D8((UnkCardGrp240 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

/* Live func_800241D4, with func_80404668's 0x84 struct shift. */
void func_804045C8(s32 *p, s32 flag)
{
    UnkCardPair *r;

    if (p != 0)
    {
        r = (UnkCardPair *)p[0x87];
        p[0x70] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_80083840;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x60;
        r->unk_04->unk_0A = 0x18;
    }
}

/* Live func_80024224, func_804045C8's twin one message over, with the same
 * 0x84 struct shift. */
void func_80404618(s32 *p, s32 flag)
{
    UnkCardPair *r;

    if (p != 0)
    {
        r = (UnkCardPair *)p[0x87];
        p[0x70] = 0x808080;
        if (flag != 0)
        {
            r->unk_00 = (s32)D_80083834;
        }
        else
        {
            r->unk_00 = 0;
        }
        r->unk_04->unk_08 = 0x66;
        r->unk_04->unk_0A = 0x18;
    }
}

/* Live func_80024274. REVISION: this build's UnkCardGrp240 is 0x84 BYTES
 * SMALLER, so `c->root_prim[0]` and `c->ptrs[9]` are at 0x1C0 and 0x21C off
 * `p` where the live struct puts them at 0x244 and 0x2A0. Written as raw word
 * indices off p, as func_804040C8 is, rather than inventing a second struct
 * for a build that only this unit sees. The same 0x84 appears in
 * func_804045C8 and func_80404618. */
void func_80404668(s32 *p)
{
    if (p != 0)
    {
        p[0x70] = 0x606868;
        ((UnkCardPair *)p[0x87])->unk_00 = 0;
    }
}

void func_80404688(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_804044D8(q);
        func_8040454C((s32 **)q);
        func_80404178(q);
    }
}
