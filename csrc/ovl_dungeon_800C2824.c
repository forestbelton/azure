#include "common.h"
#include "ovl_dungeon_decls.h"

/* dungeon.bin's overlay, translation unit 2 of 4: func_800C2824 to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

void func_8008D330(UnkDungeonActor *p, void *b, void *c, void *d);
u8 *func_8009929C(s32 c, u8 *p);
u8 *func_800999B0(u8 *p);
s32 func_8009A180(u8 *a, u8 *b);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
u8 *func_8009B4B0(u8 *p, u16 x, u16 y);
void func_8009BF7C(s16 a, s8 b);
s16 func_800A0134(u8 *a, u8 *b);
void func_800A020C(s32 f, u8 *d);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_800A1BD0(u8 *key);
s32 func_800A2B5C(void *d);
u8 *func_800A3D18(u8 *a, u8 *b, s16 c);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
UnkNode *func_800A5A18(UnkVec *a, u8 *b);
s32 func_800A9E70(UnkDungeonActor *p, void *b, u8 *c, u8 *d);
void func_800BB0A8(u8 *p, void *b, u8 *c);
void func_800BB14C(u8 *p, UnkVec *b, u8 *c);
void func_800BD184(s16 i);
s32 func_800BF7FC(u8 *p, u8 *q, s16 k, s32 r);
s32 func_800C22EC(u8 *p, u8 *q, s16 k, s32 r);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800C2824", func_800C2824);

/* THE NODE IS A `u8 *`, NOT AN `UnkNode *`. With `UnkNode *n` and
   `n->unk_10 = (u32)func_800C2824;` the node pseudo loses $a0 to the
   handler's `(high)`, the two address pairs interleave, the `addiu` of the
   second fills the `lhu`'s load-delay slot and the function is 23 words
   against 24 with 18 differing -- in either store order. 24 words,
   0 differing. */
u8 *func_800C2C7C(u8 *p)
{
    u8 *n;
    u8 *g;

    n = (u8 *)func_8003FC64(2);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800C2824;
        *(u8 **)(n + 0x20) = p;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

/* THE PAYLOAD POINTER IS ITS OWN STATEMENT, AND IT HAS TO BE ON THE OTHER
   SIDE OF A CALL. `d = n + 0x20;` written next to its use is folded into the
   store (`sh $v0, 0x40($s0)`, 147 words) -- combine turns
   (plus (plus n 32) 32) into one displacement whether the intermediate is a
   `u8 *`, an `s16 *` indexed [0x10], or written inline. Put the `d = n +
   0x20` BEFORE func_800A56E0 and the `addiu $v1, $s0, 0x20` survives as its
   own word: 148 and 0 differing.
   The D_800E39C8 clear needs the STRUCT field too -- `*(s32 *)((u8
   *)&D_800E39C8[i] + 8)` folds the +8 into the table address
   (`addiu $a0, $a0, 8`) and loses the store's displacement.
   148 words, 0 differing. */
s32 func_800C2CDC(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *e;
    u8 *n;
    u8 *d;
    s16 i;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    e = *(u8 **)(p - 0x14);
    n = func_800C7380(e[0x24], e[0x25], *(s16 *)(p + 0x88), 7, q[0]);
    if (n == NULL)
    {
        return 0;
    }
    d = n + 0x20;
    func_800A56E0(0x816);
    i = func_800B60B8(e[0x24], e[0x25], *(s16 *)(p + 0x88), 6, n);
    *(s16 *)(d + 0x20) = i;
    D_800E39C8[i].unk_08 = 0;
    h = func_800990FC();
    m = h;
    h = func_8009929C(8, h);
    if (k == 0xD)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E1684, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893D4, h);
        h = func_800999B0(h);
        if (!(*(u32 *)(p + 0x1C) & 0x2000))
        {
            if (p[0x13] < 0x31)
            {
                func_8009A3D0(e[0x24], e[0x25], 0x3000);
                *(u32 *)(p + 0x1C) |= 0x2000;
                func_8009A21C(e[0x24], e[0x25], 0x300);
            }
        }
        func_80099290(h);
        func_800A5720(m);
    }
    h = func_800990FC();
    m = h;
    h = func_80099194(D_800E169A, h);
    func_80099290(h);
    func_800A5720(m);
    if ((s16)func_80042900((s8 *)p, 2) != 0)
    {
        func_80042B68((s8 *)p, 2);
    }
    func_8009A21C(e[0x24], e[0x25], 2);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The kill skeleton WITHOUT the cached-RAM gate: past the D_800E3D7C
   identity test it spawns through func_800C7380 and returns 0 if that
   fails, then dresses the spawn, clears the spot record's +0x08 word and
   speaks only when k == 0xD -- the handle is taken unconditionally, ahead
   of that test.

   THE SPOT INDEX IS NOT A LOCAL: `D_800E39C8[*(s16 *)(c + 0x20)].unk_08 = 0`
   with the subscript written at the site. Read into an `s32 i` first the
   `lui`/`addiu` for D_800E39C8 lands AFTER the `sh`/`sll`/`sra` instead of
   before it -- 5 differing at the right length. `c` is the spawn's record
   at `n + 0x20`, which is where the `addiu $v1, $s0, 0x20` comes from.
   106 words, 0 differing. */
s32 func_800C2F2C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *e;
    u8 *n;
    u8 *c;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    e = *(u8 **)(p - 0x14);
    n = func_800C7380(e[0x24], e[0x25], *(s16 *)(p + 0x88), -1, q[0]);
    if (n == NULL)
    {
        return 0;
    }
    func_800A56E0(0x60D);
    c = n + 0x20;
    *(s16 *)(c + 0x20) = func_800B60B8(e[0x24], e[0x25], *(s16 *)(p + 0x88), 5, n);
    D_800E39C8[*(s16 *)(c + 0x20)].unk_08 = 0;
    h = func_800990FC();
    m = h;
    if (k == 0xD)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E1713, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893D8, h);
        h = func_800999B0(h);
        func_80099290(h);
        func_800A5720(m);
    }
    func_800997FC(D_800E16E3);
    func_800C8CD8(p, 0x100, 0x10);
    func_8009A21C(e[0x24], e[0x25], 0x10);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

s32 func_800C30D4(u8 *p, u8 *q, s16 k)
{
    u8 *e;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    e = *(u8 **)(p - 0x14);
    func_800C5BBC((e[0x24] << 6) | 0x20, (e[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802080, 0x20,
                  1);
    if ((s16)func_800A48F0((s8 *)p, 0x5, 0x8) != 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1729);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton again, with a charge roll through func_800C8900 and
   sound 0x612 on a hit. 65 words, 0 differing. */
s32 func_800C3238(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800C8900(p, 0x400, 0x8) != 0)
    {
        func_800A56E0(0x612);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800C3238 with a func_800A2424 gate and a message instead of a
   sound. 65 words, 0 differing. */
s32 func_800C333C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800A2424(p, 1) == 0)
    {
        func_80099844(p, D_800E173A);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The second kill skeleton with an out-of-range arm that sweeps the two
   engine slots at D_800E3D7C's +0x4C and +0x50, clearing each one's 0x40
   bit and announcing it, and falls back to D_800E1788 when neither fired.
   The flag is SET LAST IN EACH ARM, as in func_800C22EC. 148 words,
   0 differing. */
s32 func_800C3440(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    s32 f;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800C4D78((u8 *)0xC0C0C0, 0);
        func_800A56E0(0x702);
        func_80042B68((s8 *)p, 0x8);
        f = 0;
        e = *(u8 **)(D_800E3D7C + 0x4C);
        if (e[1] != 0 && (e[3] & 0x40))
        {
            e[3] &= ~0x40;
            h = func_800990FC();
            m = h;
            h = func_80099368(e, h);
            h = func_80099194(D_800E1756, h);
            func_80099290(h);
            func_800A5720(m);
            f = 1;
        }
        e = *(u8 **)(D_800E3D7C + 0x50);
        if (e[1] != 0 && (e[3] & 0x40))
        {
            e[3] &= ~0x40;
            h = func_800990FC();
            m = h;
            h = func_80099368(e, h);
            h = func_80099194(D_800E176F, h);
            func_80099290(h);
            func_800A5720(m);
            f = 1;
        }
        if (f == 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E1788, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The same skeleton with NO func_800C4AFC tint at all: this one spends a
   +0x26 charge instead, with sound 0x615, and only when at least two are
   left. func_800C37C8 is it again on +0x27 with D_800E2082.
   78 words, 0 differing. */
s32 func_800C3690(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x26] >= 2)
    {
        func_800A56E0(0x615);
        p[0x26]--;
        func_80041E70(p);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E17A1);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800C3690 on the record's +0x27 charge with D_800E2082.
   78 words, 0 differing. */
s32 func_800C37C8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x27] >= 2)
    {
        func_800A56E0(0x615);
        p[0x27]--;
        func_80041E70(p);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E2082);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 120 words, 0 differing. */
s32 func_800C3900(u8 *p, u8 *q, s16 k)
{
    s32 f;
    u8 t;
    u8 *gi;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if (p[0x13] >= 1 && p[0x13] < 0x2F && (u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        f = (*(u32 *)(p + 0x1C) >> 3) & 1;
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        t = f;
        if (t != 0)
        {
            *(u32 *)(p + 0x1C) |= 8;
        }
        *(s16 *)(p + 0x24) = 0;
        if ((*(u32 *)(p + 0x1C) & 8) == 0)
        {
            if (*(u32 *)(p + 0x1C) & 0x80000)
            {
                func_80094E34();
            }
            gi = (u8 *)&D_80083460;
            *(u16 *)(gi + 0xA) += 1;
        }
        func_800A56E0(0x51E);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E17C6);
        }
        func_80098B38(q);
    }
    else if (p[0x13] == 0)
    {
        func_800997FC(D_800E17EF);
        func_800A56E0(0x506);
    }
    else
    {
        func_80098B38(q);
        func_800997FC(D_800E180E);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton with an effect applied through func_800A48F0 -- the
   engine entry whose return type include/structs.h had as void until this
   block was opened; the `sll $v0, 16` before the `beqz` is the halfword
   test that corrected it. 74 words, 0 differing. */
s32 func_800C3AE0(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if ((s16)func_800A48F0((s8 *)p, 0x7, -0xA) != 0)
    {
        func_800A56E0(0x615);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1827);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with a +0x03 counter bumped and the node handed
   to func_800D4FC8 for its burst. 77 words, 0 differing. */
s32 func_800C3C08(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x3] < 0xFF)
    {
        p[0x3]++;
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1843);
        }
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x202020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The floor-wide reveal: behind D_800E296C's 0x20000000 it raises
   0x40000000 on every record of the engine's +0x5C ring, clears the two spot
   tables' live halfwords and walks the 0x3E x 0x3E cell grid, flooring every
   cell whose first halfword is 0xB or more and clearing its +0x02.

   THE D_80083160 BASE IS A LOCAL OF ITS OWN. Written `w = (u8 *)&D_80083160
   + 0x1DC` in one go gcc folds the displacement into the symbol
   (`%hi(D_80083160+476)`) and the pair that materialises the base disappears
   -- 188 words of 189. Assigning `v = (u8 *)&D_80083160;` first and taking
   both `v + 0x1DC` and `*(s32 *)(v + 0x1DC)` off it keeps the
   `lui`/`addiu`/`addiu` the target has.

   THE SWEEP COUNTER IS NOT THE NESTED LOOPS' COUNTER: with one `i` for all
   three loops the pseudo gets one register and the two table sweeps come out
   with the counter and the narrow table's giv in each other's registers,
   13 differing. The ring walk also loads into the CURSOR and copies to the
   sentinel (`n = D_800E3D7C; b = n;`), not the other way round -- reversed
   it is 2 differing.

   THE INNER COUNTER IS BUMPED BETWEEN THE INDEX AND THE ADDRESS. The target
   puts `addiu $a0, $a0, 1` immediately after `addu $v0, $a0, $v0` and before
   the multiply by 6; a plain `for (l = 1; l < 0x3F; l++)` emits it after the
   store and the last five words shift. Splitting the third subscript into
   `x = l + (j << shift); l++;` is the only spelling measured that places it
   there. 189 words, 0 differing. */
s32 func_800C3D3C(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *v;
    u8 *w;
    s32 t;
    u8 *b;
    u8 *n;
    u8 *c;
    s32 i;
    s32 j;
    s32 l;
    s32 x;
    u8 *g;

    v = (u8 *)&D_80083160;
    w = v + 0x1DC;
    t = *(s32 *)(v + 0x1DC);
    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else if (D_800E296C & 0x20000000)
    {
        func_8009BF7C(1, 8);
        func_800A56E0(0x80F);
        n = D_800E3D7C;
        b = n;
        do
        {
            *(u32 *)(n + 0x1C) |= 0x40000000;
            *(s16 *)(n + 0x88) = 0;
            n = *(u8 **)(n + 0x5C) + 0x20;
        } while (n != b);
        for (i = 0; i < 0x40; i++)
        {
            if (D_800E3548_s[i].unk_01 != 0)
            {
                D_800E36C8[i].unk_04 = 0;
            }
        }
        for (i = 0; i < 0x20; i++)
        {
            if (D_800E3648_s[i].unk_01 != 0)
            {
                D_800E39C8[i].unk_12 = 0;
            }
        }
        for (j = 1; j < 0x3F; j++)
        {
            for (l = 1; l < 0x3F;)
            {
                c = (u8 *)((l + (j << *(s16 *)(w + 0x14))) * 6 + t);
                if (*(u16 *)c >= 0xB)
                {
                    *(s16 *)c = 0x68;
                    c = (u8 *)((l + (j << *(s16 *)(w + 0x14))) * 6 + t);
                    *(u16 *)(c + 0x4) &= 0x7B32;
                }
                x = l + (j << *(s16 *)(w + 0x14));
                l++;
                c = (u8 *)(x * 6 + t);
                *(s16 *)(c + 0x2) = 0;
            }
        }
        func_8009D6F4();
        D_800E296C &= 0x3FF7FFFF;
        func_800403BC((s32)func_800CE028);
    }
    else
    {
        func_800997FC(D_800E1863);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The second kill skeleton with the chain in the IN-RANGE arm and a 64-slot
   sweep out of range. The sweep indexes both tables off the counter through
   UnkDungeonSlot4 / UnkDungeonSpotC: see those types for the eight words the
   `u8 *` cursors cost. 124 words, 0 differing. */
s32 func_800C4030(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        h = func_800990FC();
        m = h;
        h = func_80099368(q, h);
        h = func_80099194(D_800E187C, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893DC, h);
        func_80099290(func_800999B0(h));
        func_800A5720(m);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_8009BF7C(1, 8);
        func_800A56E0(0x80F);
        for (i = 0; i < 0x40; i++)
        {
            if (D_800E3548_t[i].unk_01 != 0)
            {
                D_800E3548_t[i].unk_01 = 0xE;
                D_800E3548_t[i].unk_00 = 3;
                D_800E36C8_t[i].unk_08 = func_8004A658(0xE, 3);
                D_800E3548_t[i].unk_02 = func_800A6DA4(0x10, 0x18);
                D_800E3548_t[i].unk_03 = 1;
            }
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800C3238 with func_800C8A3C and sound 0x520.
   65 words, 0 differing. */
s32 func_800C4220(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800C8A3C(p, 0x400, 0x8) != 0)
    {
        func_800A56E0(0x520);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

s32 func_800C4324(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0xC02020, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        if (!(*(u32 *)(p + 0x1C) & 0x10))
        {
            h = func_80099194(D_800E188B, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80042B68((s8 *)p, 0x3);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The heal: 7 plus two random bits, clamped to 0xFF and then to the gap
   between +0x28 and +0x29, parked in +0x64 and read back out of the field
   for func_8003AD08 to spell. The func_800AD6FC gate is tested as a HALFWORD
   here (`sll 16` with no `sra`), which is the (s16) cast. 108 words,
   0 differing. */
s32 func_800C4490(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 n;
    s32 d;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if ((s16)func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    n = (func_800A6D30() & 3) + 7;
    if (n > 0xFF)
    {
        n = 0xFF;
    }
    d = p[0x29] - p[0x28];
    if (d < n)
    {
        n = d;
    }
    *(s16 *)(p + 0x64) = n;
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        h = func_80099734(p, h);
        h = func_80099194(D_800E18A4, h);
        h = func_8003AD08(*(s16 *)(p + 0x64), h);
        h = func_80099194(D_800893E0, h);
        func_80099290(h);
        func_800A5720(m);
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20E020, p);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The +0x10 handler of the 0x202 node func_800C4944 spawns: a three-state
   fade driven off the view's +0x00, then a shared tail that swings the
   mover's two integer halves by rcos/rsin of +0x16 scaled by +0x14 around the
   parent record at +0x1C. State 0 eases the renderable's colour triple and
   its +0x1C/+0x1E scale toward one of two targets chosen by +0x18 over +0x02
   ticks; state 1 flashes the triple on a 2-bit roll; state 2 does the same
   with a decaying value and kills the view when +0x02 runs out. The switch is
   a real `switch` -- cases 0/1/2 give gcc's balanced tree, `beq 1` first then
   `slti 2`, which is what the target has. 193 words, 0 differing.

   THE COLOUR TRIPLE IS ONE CHAINED ASSIGNMENT, `r->unk_0C[0] = r->unk_0C[1] =
   r->unk_0C[2] = v;`, NOT THREE STATEMENTS OFF A LOCAL. The chain stores
   0xE, 0xD, 0xC (innermost first) either way, but through a local the value
   pseudo is a separate quantity and local-alloc hands it $v1 where the target
   reuses $v0: 12 words differing across states 0 and 2, 15 for the function.
   The same holds for the `r->unk_1C = r->unk_1E = v;` pair.

   STATE 1'S PAIR OF CONSTANTS IS `t = 0xF0; if (...) t = 0x10;` AND THE CALL
   RESULT MUST GO THROUGH ITS OWN LOCAL FIRST. As a ternary -- either
   `(x & 3) == 0 ? 0xF0 : 0x10` or its mirror -- fold canonicalizes EQ to NE
   with the arms swapped, so the delay slot carries 0x10 behind a `bnez` where
   the target has 0xF0 behind a `beqz`: 3 words. Written with the call inline
   in the `if`, `t = 0xF0` lands before the `jal` and needs a callee-saved
   register: 195 words, two more, exactly the save/restore pair. */
void func_800C4640(u8 *p, UnkVec *v, UnkSlot *r)
{
    s32 k;
    s32 t;

    switch (*(s16 *)(p + 0x0))
    {
    case 0:
        r->unk_0C[0] = r->unk_0C[1] = r->unk_0C[2] =
            r->unk_0C[2] +
            (((*(s16 *)(p + 0x18) != 0 ? 0xF0 : 0x40) - r->unk_0C[2]) / *(s16 *)(p + 0x2));
        r->unk_1C = r->unk_1E =
            r->unk_1E + (((*(s16 *)(p + 0x18) != 0 ? 0x100 : 0x400) - r->unk_1E) / *(s16 *)(p + 0x2));
        v->unk_14 -= 0x200;
        *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
        if (*(s16 *)(p + 0x2) <= 0)
        {
            *(s16 *)(p + 0x2) = 8;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 1:
        v->unk_14 += 0x1800;
        if (*(s16 *)(p + 0x18) != 0)
        {
            k = rand();
            t = 0xF0;
            if ((k & 3) != 0)
            {
                t = 0x10;
            }
            r->unk_0C[0] = r->unk_0C[1] = r->unk_0C[2] = t;
        }
        *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
        if (*(s16 *)(p + 0x2) <= 0)
        {
            *(s16 *)(p + 0x2) = 0x20;
            *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
        }
        break;
    case 2:
        v->unk_14 += 0x2000;
        if (*(s16 *)(p + 0x18) != 0)
        {
            r->unk_0C[0] = r->unk_0C[1] = r->unk_0C[2] =
                (rand() & 3) != 0 ? 0x10 : (u32)(*(s16 *)(p + 0x2) * 15) >> 1;
        }
        *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
        if (*(s16 *)(p + 0x2) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x14) < *(s16 *)(p + 0x12))
    {
        *(s16 *)(p + 0x14) = *(u16 *)(p + 0x14) + 2;
        if (*(s16 *)(p + 0x12) < *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0x14) = *(u16 *)(p + 0x12);
        }
    }
    *(s16 *)(p + 0x16) = *(u16 *)(p + 0x16) + (func_800644B8((++*(s16 *)(p + 0x10)) << 8) >> 6);
    *(s16 *)((u8 *)v + 0x2) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2) +
                              ((func_80064584(*(s16 *)(p + 0x16)) * *(s16 *)(p + 0x14)) >> 12);
    *(s16 *)((u8 *)v + 0x6) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x6) +
                              ((func_800644B8(*(s16 *)(p + 0x16)) * *(s16 *)(p + 0x14)) >> 12);
    v->unk_08 += v->unk_14;
}

/* The +0x10 handler of func_800C4AFC's tint node: twice per frame it spawns a
   0x202 node parked on func_800C4640, seeds its mover from the dying mover's
   +0x1C record and three random draws, points its renderable at D_800DF564,
   and when its own +0x02 countdown expires kills itself and decrements the
   engine's +0x0A counter. 110 words, 0 differing.

   THE THREE RANDOM DRAWS ARE AN `s32 t[3]` ARRAY, NOT THREE LOCALS. As three
   scalars they take callee-saved registers and `t[0] & 0xF` is hoisted out of
   the loop: 109 words / 107 differing. An array always lives in memory, so
   the three `sw`s land at 0x10/0x14/0x18($sp), the reloads narrow to `lhu`,
   and nothing is hoisted.

   AND `q = (u8 *)n + 0x20;` MUST BE WRITTEN BEFORE THE func_8004491C CALL.
   Written after it, `q` does not cross a call, global-alloc gives it $a2
   instead of $s0 and the function is 108 words -- two short, exactly the
   missing $s6 save/restore pair. */
void func_800C4944(u8 *p)
{
    UnkNode *n;
    UnkVec *v;
    u8 *q;
    u8 *r;
    u8 *g;
    s32 i;
    s32 t[3];

    t[0] = rand();
    t[1] = rand();
    t[2] = rand();
    for (i = 1; i >= 0; i--)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            n->unk_10 = (u32)func_800C4640;
            func_8004491C(n, (u32)D_80045340);
            *(s16 *)(q + 0x12) = (t[0] & 0xF) + 0x10;
            *(s16 *)(q + 0x16) = t[1];
            v = n->unk_08;
            *(s16 *)(q + 0xC) = *(s16 *)((u8 *)v + 0x2) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x2);
            *(s16 *)(q + 0xE) = *(s16 *)((u8 *)v + 0x6) = *(u16 *)(*(u8 **)(p + 0x1C) + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(*(u8 **)(p + 0x1C) + 0xA) -
                                      D_800DDC40[(*(u8 **)(p + 0x28))[0x13]] - (t[2] & 0xF);
            *(u8 **)(q + 0x1C) = *(u8 **)(p + 0x1C);
            r = *(u8 **)((u8 *)n + 0xC);
            *(s16 *)(r + 0x14) = 0xC;
            *(u32 *)(r + 0x8) = (u32)D_800DF564;
            *(s16 *)(r + 0x10) = i != 0 ? 0x20 : 0x60;
            *(s16 *)(q + 0x2) = 8;
            *(s16 *)(q + 0x18) = i;
        }
    }
    *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g = (u8 *)&D_80083460;
        *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
    }
}

/* Spawn the 0x202 tint node the kill handlers close on: stash the colour in
   D_800DCF78, run func_800B835C over the pair {0x1000340, 0x200020}, copy the
   dying mover's three coordinates into both the new node's mover and its own
   +0x40 record, and bump the engine's +0x0A counter. 65 words, 0 differing.

   THE FIRST ARGUMENT IS `(u8 *)&D_800DCF78 - 0x10`, and that spelling is what
   produces the target's THREE-instruction address: one `lui %hi`, one
   `addiu %lo` and a separate `addiu -0x10`, with the colour's own store
   reusing the same `%hi` register. A symbol of its own at 0x800DCF68 would
   fold into two.

   ONE TEMPORARY PER COORDINATE, NOT ONE REUSED. With a single `u16 t` for all
   three the three live quantities rotate -- the copy takes $a1 where the
   target has $v0, the record pointer $v1 where the target has $a1 and the
   mover $v0 where the target has $v1 -- 18 differing with every opcode right.
   Three locals is 0. The screen that proved it was the mode: `u8 t` alone
   reproduces the target's allocation exactly and leaves only the three `lbu`
   against `lhu`, 5 differing, which says the residue was never about the
   stores.

   AND THE COLOUR'S STORE COMES FIRST, above both stack-array stores: written
   last it swaps with the `sw 0x14($sp)` that fills the call's delay slot
   (2 differing), and written between them it also swaps the two constants'
   registers (7). */
void func_800C4AFC(u8 *m, s32 col, u8 *act)
{
    UnkNode *n;
    u8 *q;
    UnkVec *v;
    s32 arr[2];
    u16 t1;
    u16 t2;
    u16 t3;
    u8 *g;

    n = func_8003FD64(0x202, D_80083498);
    if (n != NULL)
    {
        D_800DCF78 = col;
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C((u8 *)&D_800DCF78 - 0x10, arr, 1, 0);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2) = 0x40;
        v = n->unk_08;
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800C4944;
        t1 = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x2) = t1;
        *(s16 *)(q + 0x20) = t1;
        t2 = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0x6) = t2;
        *(s16 *)(q + 0x22) = t2;
        t3 = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = t3;
        *(s16 *)(q + 0x24) = t3;
        *(u8 **)(q + 0x1C) = m;
        *(u8 **)(q + 0x28) = act;
        func_800A56E0(0x702);
        g = (u8 *)&D_80083460;
        *(s16 *)(g + 0xA) += 1;
    }
}

/* The +0x10 handler of the tint node func_800C4D78 spawns: ease
   D_80083178's three +0x90 colour bytes toward the record's own triple, or
   toward 0x80 once the record's +0x04 is non-zero, one 1/+0x06th step per
   frame. 94 words, 0 differing on the first compile.

   `(u8 *)&D_80083178` as a POINTER LOCAL is the unit's own spelling for this
   symbol (csrc/ovl_dungeon.c:13497), and the same for D_80083460. */
void func_800C4C00(u8 *p)
{
    u8 *g;
    u8 *h;

    g = (u8 *)&D_80083178;
    if (*(s16 *)(p + 0x4) == 0)
    {
        g[0x90] += (p[0x0] - g[0x90]) / *(s16 *)(p + 0x6);
        g[0x91] += (p[0x1] - g[0x91]) / *(s16 *)(p + 0x6);
        g[0x92] += (p[0x2] - g[0x92]) / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x10;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        }
    }
    else
    {
        g[0x90] += (0x80 - g[0x90]) / *(s16 *)(p + 0x6);
        g[0x91] += (0x80 - g[0x91]) / *(s16 *)(p + 0x6);
        g[0x92] += (0x80 - g[0x92]) / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            g[0x90] = 0x80;
            g[0x91] = 0x80;
            g[0x92] = 0x80;
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Spawn a kind-0x200 node carrying the caller's record, park func_800C4C00
   at its +0x10, and make a noise when the flag is set. 34 words,
   0 differing. */
void func_800C4D78(u8 *p, s16 noise)
{
    UnkNode *n;
    u8 *g;

    n = func_8003FD64(0x200, D_80083498);
    if (n != NULL)
    {
        *(s16 *)((u8 *)n + 0x26) = 0x20;
        n->unk_10 = (u32)func_800C4C00;
        *(u8 **)((u8 *)n + 0x20) = p;
        if (noise != 0)
        {
            func_800A56E0(0x501);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}

/* A node's +0x10 step, gated on the engine's 0x10 flag. The record is
   reached as an ARRAY OF 0x14-BYTE STRUCTS, which is what puts the
   `lui`/`addiu` ahead of the index chain and makes the final `addu`
   index-first. 45 words, 0 differing. */
void func_800C4E00(u8 *p)
{
    if ((D_80083462 & 0x10) != 0)
    {
        if (--*(s16 *)(p + 0x8) <= 0)
        {
            if (*(s16 *)(p + 0xA) == 1)
            {
                func_800997FC(D_800E18C8);
                D_800E2970_s[*(s16 *)(p + 0xC)].unk_0C &= ~2;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Allocate a kind-0 node under D_80083498, park func_800C4E00 at its +0x10
   and fill three halfwords of its payload. The first three parameters are
   unread here; the fourth and the stack fifth are the two halfwords, and
   func_800BF7FC's call site passes them with `lbu`/`sw` so neither width is
   witnessed -- s16 is this unit's convention for a parameter whose only use
   is a halfword store. 27 words, 0 differing. */
UnkNode *func_800C4EB4(s32 a, s32 b, s32 c, s16 d, s16 e)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800C4E00;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x8) = 0x40;
        *(s16 *)(q + 0xA) = d;
        *(s16 *)(q + 0xC) = e;
    }
    return n;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_800893D4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_800893D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_800893DC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_800893E0);

void func_800C4F20(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    u8 *q0;
    u8 *r;
    s32 i;
    s32 ii;
    s32 t;
    s32 j;
    s32 jj;
    s32 cs;
    s16 n;
    s32 m;
    u16 w;

    switch (*(s16 *)(p + 0x28))
    {
    case 0:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            *(u16 *)(c + 0x1E) += (0x1000 - *(u16 *)(c + 0x1E)) / n;
            *(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
            *(u16 *)(c + 0x20) += (0x1000 - *(u16 *)(c + 0x20)) / *(s16 *)(p + 0x2A);
            q = *(u8 **)(p + 0x20);
            *(u16 *)(q + 0x1E) += (0x400 - *(u16 *)(q + 0x1E)) / *(s16 *)(p + 0x2A);
            *(u16 *)(q + 0x1C) = *(u16 *)(q + 0x1E);
            *(u16 *)(p + 0x32) -= *(s16 *)(p + 0x32) / *(s16 *)(p + 0x2A);
        }
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(c + 0x1E) = 0x1000;
            *(u16 *)(c + 0x1C) = 0x1000;
            *(u16 *)(c + 0x20) = 0x1000;
            q0 = *(u8 **)(p + 0x20);
            *(u16 *)(q0 + 0x1E) = 0x800;
            *(u16 *)(q0 + 0x1C) = 0x800;
            *(s16 *)(p + 0x32) = 0;
            *(u16 *)(p + 0x2A) = 0x20;
            *(u16 *)(p + 0x28) += 1;
        }
        goto join;
    case 1:
        t = func_80064584(*(s16 *)(p + 0x2E)) >> 6;
        r = D_80083780;
        i = *(s16 *)(r + 0x2);
        i += t;
        cs = func_800644B8(*(s16 *)(p + 0x2E)) >> 6;
        i -= *(s16 *)(b + 0x2);
        jj = *(s16 *)(r + 0x6) + cs;
        if (i < 0)
        {
            i += 3;
        }
        *(u16 *)(b + 0x2) += i >> 2;
        *(u16 *)(b + 0x6) += (jj - *(s16 *)(b + 0x6)) / 4;
        j = (func_800644B8(*(s16 *)(p + 0x2C) << 5) >> 6) + 0x40;
        ii = *(s16 *)(r + 0xA) - *(s16 *)(b + 0xA);
        *(u16 *)(b + 0xA) += (ii - j) / 4;
        *(u16 *)(p + 0x2E) = *(s16 *)(p + 0x2E) + 0x40 + (func_80069EF8(ii) & 0x1F);
        m = *(s16 *)(p + 0x36);
        if (m != 0)
        {
            if (i)
            {
                w = *(u16 *)(p + 0x2A) - 1;
            }
            else
            {
                w = *(u16 *)(p + 0x2A) - 1;
            }
            *(u16 *)(p + 0x2A) = w;
            if ((s32)(w << 16) <= 0)
            {
                n = m;
                if (n != 1 && n != 3)
                {
                    *(u16 *)(p + 0x2A) = 0x20;
                    *(u16 *)(p + 0x28) += 0x10;
                    func_800A56E0(0x518);
                }
                else
                {
                    *(u16 *)(p + 0x2A) = 0x10;
                    *(u16 *)(p + 0x28) += 1;
                }
            }
        }
        goto join;
    case 2:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            *(u16 *)(b + 0x2) += (((*(u8 **)(p + 0x20))[0x24] << 6) + 0x20 - *(s16 *)(b + 0x2)) / n;
            *(u16 *)(b + 0x6) +=
                (((*(u8 **)(p + 0x20))[0x25] << 6) + 0x20 - *(s16 *)(b + 0x6)) / *(s16 *)(p + 0x2A);
            *(u16 *)(b + 0xA) +=
                (*(s16 *)(*(u8 **)(p + 0x24) + 0x88) - *(s16 *)(b + 0xA)) / *(s16 *)(p + 0x2A);
        }
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(p + 0x2A) = 0x10;
            *(u16 *)(p + 0x28) += 1;
        }
        goto join;
    case 3:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            *(u16 *)(c + 0x1E) -= *(u16 *)(c + 0x1E) / n;
            *(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
            *(u16 *)(c + 0x20) += (0x2000 - *(u16 *)(c + 0x20)) / *(s16 *)(p + 0x2A);
            q = *(u8 **)(p + 0x20);
            *(u16 *)(q + 0x1E) += (0x1000 - *(u16 *)(q + 0x1E)) / *(s16 *)(p + 0x2A);
            *(u16 *)(q + 0x1C) = *(u16 *)(q + 0x1E);
            *(u16 *)(p + 0x32) += (0x400 - *(s16 *)(p + 0x32)) / *(s16 *)(p + 0x2A);
        }
        goto fade;
    case 17:
        n = *(s16 *)(p + 0x2A);
        if (n != 0)
        {
            *(u16 *)(b + 0xA) += (-0x400 - *(s16 *)(b + 0xA)) / n;
        }
    fade:
        w = *(u16 *)(p + 0x2A) - 1;
        *(u16 *)(p + 0x2A) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u32 *)(*(u8 **)(p + 0x24) + 0x14) &= 0xFFEFFFFF;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        goto join;
    default:
    join:
        if ((*(u32 *)(*(u8 **)(p + 0x24) + 0x14) & 0x100000) != 0)
        {
            *(u16 *)(*(u8 **)(p + 0x1C) + 0x2) = *(u16 *)(b + 0x2);
            *(u16 *)(*(u8 **)(p + 0x1C) + 0x6) = *(u16 *)(b + 0x6);
            *(u16 *)(*(u8 **)(p + 0x1C) + 0xA) = *(u16 *)(b + 0xA);
        }
        *(u16 *)(p + 0x2C) += 1;
        *(u16 *)(c + 0x1A) -= 0x80;
        return;
    }
}

/* Spawn the kind-0x12 node that carries the actor's beam: install
   func_800C4F20 at its +0x10, hand func_800C55A0 to func_8004491C as the
   tag, fill the +0x0C renderable from D_800DF630 and copy the mover's three
   coordinates into the node's own mover.

   THREE SPELLINGS, each measured:
   (1) `b` IS `s16` AND THE +0x2E VALUE IS `(s32)b * 0x800`. As `s32 b` with
   `(s16)b << 11` -- or as `s16 b` with a plain `b << 11` -- gcc drops the
   sign-extension, because the destination is a halfword store, and the
   target's `sll 16` / `sra 5` pair becomes one `sll 11`: 93 words, 7
   differing. `(b << 16) >> 5` keeps the pair but is one word long at 94,
   and `(s32)b << 11` is 92 / 49.
   (2) `d` IS `s16`. That is what produces the raw `addu $s6, $s3, $zero`
   copy the +0x36 store uses while the two tests sign-extend $s3 separately;
   as `s32` with `(s16)` casts the copy goes and the function is 90 / 89,
   as `u16` 89 / 88.
   (3) THE +0x20 STORE IS WRITTEN BEFORE THE +0x2E AND +0x24 ONES. Written
   last its `lw -0x14($s2)` is scheduled after the +0x2E store and its load
   delay stands as a `nop` where the target covers it with the `sra`: 94
   words. Between the other two it is 93 / 14, and hoisting the load into
   its own local is 93 / 22. 93 words, 0 differing. */
UnkNode *func_800C542C(u8 *p, s16 a, s16 b, s16 d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    UnkVec *v;
    u8 *m;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800C4F20;
        func_8004491C(n, (u32)func_800C55A0);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0xC) = 0x808080;
        *(u8 **)(r + 0x8) = D_800DF630;
        *(s16 *)(r + 0x6) = 0xC;
        m = *(u8 **)(p - 0x18);
        v = n->unk_08;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x30) = a;
        *(u8 **)(q + 0x1C) = m;
        *(u8 **)(q + 0x20) = *(u8 **)(p - 0x14);
        *(s16 *)(q + 0x2E) = (s32)b * 0x800;
        *(u8 **)(q + 0x24) = p;
        *(s16 *)(r + 0x20) = 0x2000;
        *(s16 *)(q + 0x32) = 0x400;
        *(s16 *)(q + 0x2C) = rand();
        if (d == 1)
        {
            *(s16 *)(q + 0x2A) = 0;
        }
        else if (d == 0)
        {
            func_800A56E0(0x802);
            *(s16 *)(q + 0x2A) = 0x10;
        }
        else
        {
            func_800A56E0(0x802);
            *(s16 *)(q + 0x2A) = 0x20;
        }
        *(s16 *)(q + 0x36) = d;
        *(s32 *)(p + 0x14) |= 0x100000;
    }
    return n;
}

s32 func_800C55A0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C55F4(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* The slot's shadow/overlay triangle fan: push the matrix stack, project the
   mover's position through the view to get the depth reference at +0xCC,
   compose the slot's +0x16 rotation and +0x1C scale into the scratchpad
   matrix at +0x50 and load it, then walk the element list at
   `s->unk_08`'s +0x8 -- one 0xC-byte record per triangle, three byte indices
   into the 8-byte vertex table at +0x0 and a sign-bit terminator at +0xB --
   projecting each triangle into a POLY_G3 taken from D_80083160.unk_00's
   +0x8D0 cursor, depth-fading each corner's colour through func_8004CECC and
   chaining the primitive plus a following 0xC-byte command into the OT.
   284 words, 0 differing.

   FOUR SPELLINGS ARE LOAD-BEARING, each measured on this function:

   (1) THE VERTEX TABLE BASE IS AN INTEGER, NOT A POINTER. `u8 *tbl` puts the
   base in the `addu`'s FIRST operand at all six table reads, because
   c-typeck.c's `pointer_int_sum` builds `ptr + idx` whatever order the source
   writes, and the target has the index first. Writing the sum as
   `(c[0] << 3) + tbl` does not move it; declaring `tbl` as `u32` does, and
   that is 6 of the 36 differing words.

   (2) THE `0x100 - p[0x32]` PARTIAL IS ITS OWN LOCAL PER CALL SITE -- u0, u1,
   u2 -- and that is worth 30 differing. Shared in one local it lives in three
   basic blocks, so global-alloc owns it; `set_preference` (global.c:1545)
   takes XEXP(src,0) of each `subu`, which is the 0x100 constant local-alloc
   has already put in $a0 at one site and $a1 at another, so the partial
   carries preferences {4,5}; `expand_preferences` (global.c:782) then merges
   them into the clamped value it dies into, and `find_reg`'s preference pass
   overrides the $v1 it had picked with $a0. One local per site keeps all
   three out of global-alloc, the clamped value is left with no hard-register
   preference, and find_reg's plain scan gives it the target's $v1.

   (3) AND THE PARTIAL CANNOT BE A PARENTHESISED SUBEXPRESSION.
   `t = (0x100 - p[0x32]) - ((sp->unk_CC - sp->unk_C0) << 5);` is 285 words /
   125 differing -- fold reassociates it anyway -- against 284 / 30 for the
   same thing as two statements. The unparenthesised three-term form is
   285 / 131 and puts the shifted term first.

   (4) THE TWO SCRATCHPAD ADDRESS SPELLINGS ARE BOTH HERE, and the
   `ori`/`addiu` distinction is what picks them. `&sp->unk_50`, `&sp->unk_30`,
   `&sp->unk_D0`, `&sp->unk_90` and `&sp->unk_94` are plain members and come
   out as `addu $aN, $s3, $zero` + `ori $aN, $aN, 0xNN` -- a 32-bit constant
   whose `lui` cse replaced with a copy of the held base -- while
   `&sp->unk_70[i]` is a SUBSCRIPT and comes out as one `addiu $a0, $s3, 0xNN`
   off the same register. Mixing them is not a slip; it is what the target
   has, and docs/matching-levers.md's "`ori` WHERE THE TARGET HAS `addiu`" is
   the diagnostic.

   func_8004CECC's `s16` second parameter is also load-bearing: the
   `sll`/`sra 16` pair at each of the three call sites is the CALLER's, and
   without the prototype there is no such pair.

   The primitive cursor and the `&prim->code` cursor at +7 are two source
   pointers, each bumped by 0x1C and then 0xC: the three NormalClip arguments
   are read off the code cursor at +1/+9/+0x11 and the three RotTransPers
   out-parameters off the primitive at +8/+0x10/+0x18, so neither base can
   serve both. The element cursor is ONE pointer: `c + 0xB` is loop.c's giv
   and combine_givs folds c[1] and c[2] onto it as -0xA and -0x9. */
void func_800C55F4(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    MATRIX m;
    UnkDungeonSP3 *sp;
    u8 *g;
    u8 *e;
    u8 *c;
    u32 tbl;
    u8 *prim;
    u8 *code;
    s32 u0;
    s32 u1;
    s32 u2;
    s32 t;

    sp = (UnkDungeonSP3 *)0x1F800000;
    g = (u8 *)&D_80083160;
    e = (u8 *)s->unk_08;
    c = *(u8 **)(e + 0x8);
    tbl = *(u32 *)e;
    PushMatrix();
    sp->unk_30.vx = s->unk_1C;
    sp->unk_30.vy = s->unk_1E;
    sp->unk_30.vz = s->unk_20;
    sp->unk_28.vx = sp->unk_50.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_28.vy = sp->unk_50.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_28.vz = sp->unk_50.t[2] = *(s16 *)((u8 *)v + 0xA);
    sp->unk_CC = RotTransPers(&sp->unk_28, &sp->unk_F0, &sp->unk_90, &sp->unk_94) - d;
    ReadRotMatrix(&m);
    RotMatrix((SVECTOR *)&s->unk_16, &sp->unk_50);
    ScaleMatrix(&sp->unk_50, &sp->unk_30);
    CompMatrix(&m, &sp->unk_50, &sp->unk_D0);
    SetTransMatrix(&sp->unk_D0);
    SetRotMatrix(&sp->unk_D0);
    sp->unk_20 = *(u8 **)&D_80083160 + 0xB0;
    prim = *(u8 **)(*(u8 **)&D_80083160 + 0x8D0);
    code = prim + 0x7;
    s->unk_14 |= 0x8000;
    for (;;)
    {
        *(s32 *)&sp->unk_70[0] = *(s32 *)((c[0] << 3) + tbl);
        *(s32 *)&sp->unk_70[1] = *(s32 *)((c[1] << 3) + tbl);
        *(s32 *)&sp->unk_70[2] = *(s32 *)((c[2] << 3) + tbl);
        sp->unk_70[0].vz = *(u16 *)((c[0] << 3) + tbl + 0x4);
        sp->unk_70[1].vz = *(u16 *)((c[1] << 3) + tbl + 0x4);
        sp->unk_70[2].vz = *(u16 *)((c[2] << 3) + tbl + 0x4);
        sp->unk_C0 = RotTransPers(&sp->unk_70[0], (s32 *)(prim + 0x8), &sp->unk_90, &sp->unk_94);
        sp->unk_C4 = RotTransPers(&sp->unk_70[1], (s32 *)(prim + 0x10), &sp->unk_90, &sp->unk_94);
        sp->unk_C8 = RotTransPers(&sp->unk_70[2], (s32 *)(prim + 0x18), &sp->unk_90, &sp->unk_94);
        sp->unk_114 = NormalClip(*(s32 *)(code + 0x1), *(s32 *)(code + 0x9), *(s32 *)(code + 0x11));
        if (sp->unk_114 > 0)
        {
            sp->unk_C0 -= d;
            sp->unk_C4 -= d;
            sp->unk_C8 -= d;
            if ((u32)sp->unk_C0 < 0x1E0)
            {
                u0 = 0x100 - *(s16 *)(p + 0x32);
                t = u0 - ((sp->unk_CC - sp->unk_C0) << 5);
                if (t < 0x20)
                {
                    t = 0x20;
                }
                else if (t >= 0x100)
                {
                    t = 0xFF;
                }
                func_8004CECC(*(s16 *)(p + 0x30), t, 0xFF, prim + 0x4);
                u1 = 0x100 - *(s16 *)(p + 0x32);
                t = u1 - ((sp->unk_CC - sp->unk_C4) << 5);
                if (t < 0x20)
                {
                    t = 0x20;
                }
                else if (t >= 0x100)
                {
                    t = 0xFF;
                }
                func_8004CECC(*(s16 *)(p + 0x30), t, 0xFF, prim + 0xC);
                u2 = 0x100 - *(s16 *)(p + 0x32);
                t = u2 - ((sp->unk_CC - sp->unk_C8) << 5);
                if (t < 0x20)
                {
                    t = 0x20;
                }
                else if (t >= 0x100)
                {
                    t = 0xFF;
                }
                func_8004CECC(*(s16 *)(p + 0x30), t, 0xFF, prim + 0x14);
                SetPolyG3(prim);
                *code |= 0x2;
                AddPrim(sp->unk_20 + sp->unk_C0 * 4, prim);
                prim += 0x1C;
                code += 0x1C;
                s->unk_14 &= 0x7FFF;
                func_80067F20((u32 *)prim, 0, 0, 0, 0);
                AddPrim(sp->unk_20 + sp->unk_C0 * 4, prim);
                prim += 0xC;
                code += 0xC;
            }
        }
        if (*(s8 *)(c + 0xB) < 0)
        {
            break;
        }
        c += 0xC;
    }
    PopMatrix();
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)prim;
}

/* Per-frame step: advance the mover by its own three deltas, damp the first
   two by a 1/8th subtract, fade the renderable's three colour bytes towards 0
   by 1/(remaining frames), and on the last frame drop the engine's +0xA
   counter and raise the node's 0x8000 flags. `*(s16 *)(p + 0x10)` is re-read
   for every division because each `sb` invalidates it, while `b->unk_10`
   survives from the `b->unk_04 +=` above it -- both fall out of the plain
   spelling. 86 words, 0 differing on the first compile. */
void func_800C5A64(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    s16 t;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= b->unk_0C >> 3;
    b->unk_10 -= b->unk_10 >> 3;
    func_800478B8(c);
    c[0xC] -= c[0xC] / *(s16 *)(p + 0x10);
    c[0xD] -= c[0xD] / *(s16 *)(p + 0x10);
    c[0xE] -= c[0xE] / *(s16 *)(p + 0x10);
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        if (*(s16 *)(p + 0xC) == 0 && *(s16 *)(p + 0x1C) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The eight-or-more particle spawner: a random 0x11..0x20 nodes on
   func_800C5A64, the first of them 0x12 and the rest 0x212, each with a
   random position jitter and a `-((rand & 0xF) + 0x18) << 13' lift. Returns
   the FIRST node, so it is not void.

   `g = (u8 *)&D_80083460;' BELONGS AT ITS USE, inside the `i == 0' arm.
   Hoisted above the loop its `lui'/`addiu' pair is emitted whole before the
   `bltz' guard and the target's split -- `lui' in the guard's delay slot,
   `addiu' in the preheader -- is gone: 113 words, 2 differing.

   THE COLOUR STORE IS THE FIRST OF THE SIX RENDERABLE STORES. Written in
   offset order (0x1E, 0x1C, 0x10, 0x12, 0xC, 0x14) the `lw' that reloads the
   fourth parameter from its home slot is scheduled seven words late:
   113 words, 4 differing. Four other orders measure 4, 6 and 8.

   THE FIRST PARAMETER IS `s16' AND HAS NO REG_EQUIV. A narrow REGISTER
   parameter is promoted, so assign_parms does not hang the REG_EQUIV note on
   its entry copy, and reload spills it to a fresh 16-bit slot -- which is the
   target's `sh $a0, 0x10($sp)' / `lhu $t0, 0x10($sp)' pair rather than a word
   spill to its own 0x40($sp) home. With a separate `s16 w = x;' local gcc
   spills `y' instead and the three register parameters rotate: 113 words,
   21 differing. The ternary is `i != 0 ? 0x212 : 0x12', not the other way
   round -- inverted it is 17 differing. 113 words, 0 differing. */
s32 func_800C5BBC(s16 x, s16 y, s16 ang, s32 col, u16 e, s16 snd)
{
    UnkNode *n;
    UnkNode *first;
    UnkVec *m;
    u8 *r;
    u8 *p;
    u8 *g;
    s32 i;
    s32 k;

    first = NULL;
    if (snd != 0)
    {
        func_800A56E0(0x608);
    }
    i = 0;
    k = (func_80069EF8() & 0xF) | 0x10;
    for (; k >= 0; k--)
    {
        n = func_8003FC64(i != 0 ? 0x212 : 0x12);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800C5A64;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = ang;
            *(s32 *)((u8 *)m + 0xC) += ((func_80069EF8() & 0x1F) - 0x10) << 14;
            *(s32 *)((u8 *)m + 0x10) += ((func_80069EF8() & 0x1F) - 0x10) << 14;
            *(s32 *)((u8 *)m + 0x14) = -((func_80069EF8() & 0xF) + 0x18) << 13;
            r = (u8 *)n->unk_0C;
            func_8003DB94(r, D_800DEA68, 0);
            *(u32 *)(r + 0xC) = col;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(r + 0x10) = e;
            *(s16 *)(r + 0x12) = 3;
            *(u16 *)(r + 0x14) |= 0xC;
            p = (u8 *)n + 0x20;
            *(s16 *)(p + 0x10) = (func_80069EF8() & 0xF) | 0x10;
            *(s16 *)(p + 0xC) = i;
            if (i == 0)
            {
                first = n;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
            }
        }
        i++;
    }
    return (s32)first;
}

/* The +0x10 countdown is the usual `lhu` / `addiu -1` / `sh` / `sll 16`+
   `bgtz` pair of statements through one s16 local; the colour fade is one s32
   local, `v -= v / n`. 55 words, 0 differing. */
void func_800C5D80(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    s16 t;
    s32 v;

    b->unk_08 += b->unk_14;
    b->unk_14 += 0x2000;
    func_800478B8(c);
    v = c[0xC];
    v -= v / *(s16 *)(p + 0x10);
    c[0xC] = v;
    c[0xD] = v;
    c[0xE] = v;
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        if (*(s16 *)(p + 0xC) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Twelve nodes in a ring around (x, y): eleven kind-0x212 and a kind-0x12
   last, each with func_800C5D80 at its +0x10 and the facing stepped a full
   0x8000 per node.

   FOUR SPELLINGS ARE LOAD-BEARING, and each is a REGISTER-PRESSURE fact --
   nine values want the nine callee-saved registers and the wrong spelling
   loses one of them to a loop-invariant the target rematerialises.
   (1) THE THREE REGISTER PARAMETERS ARE COPIED INTO LOCALS AHEAD OF THE
       func_800A56E0 CALL. Used directly they are 83 words / 14 differing
       with $a0's and $a2's values in each other's registers; copied AFTER
       the call instead, the parameter pseudos have to survive it too and it
       is 86 words / 79. The declaration order of the three locals does not
       matter (measured both ways, 0 differing).
   (2) `m->unk_14` IS A GIV, `(i + 1) * -0x8000`, not an accumulator. A
       `w` local stepped by `w += 0x8000` from 0xFFFA0000 emits the same
       `ori`/`addu` pair inside the loop but puts its `lui` BEFORE loop.c's
       hoists in the preheader instead of after them: 83 words / 7 differing,
       or 8 with `w` initialised ahead of the `for`. loop.c emits a giv's
       initialisation after `move_movables` has inserted the invariants,
       which is the order the target has.
   (3) `g = (u8 *)&D_80083460;` -- the unit's own pointer-local idiom for
       this global. Reaching it as `*(u16 *)((u8 *)&D_80083460 + 0xA)` keeps
       only the `lui` in the callee-saved register and is 82 words of 83 and
       62 differing.
   (4) The node kind is `i != 0 ? 0x212 : 0x12`. Written `i == 0 ? 0x12 :
       0x212` gcc keeps the source's test and emits `bnez` with 0x212 in the
       delay slot where the target has `beqz` with 0x12: 3 differing.
   Also measured and NOT needed, each 0 differing either way: the order of
   the three mover stores (all six permutations), where the install sits
   relative to the func_8004491C call, and a local for the 0x1000 pair.
   83 words, 0 differing. */
void func_800C5E5C(s32 x, s32 y, s32 z, void *d, s16 e)
{
    s32 cz;
    s32 cy;
    s32 cx;
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *v;
    s32 i;
    u8 *g;

    cx = x;
    cy = y;
    cz = z;
    if (e != 0)
    {
        func_800A56E0(0x816);
    }
    for (i = 0xB; i >= 0; i--)
    {
        n = func_8003FC64(i != 0 ? 0x212 : 0x12);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800C5D80;
            func_8004491C(n, (u32)func_80045C34);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = cx;
            *(s16 *)((u8 *)m + 0x6) = cy;
            *(s16 *)((u8 *)m + 0xA) = cz;
            m->unk_14 = (i + 1) * -0x8000;
            r = (u8 *)n->unk_0C;
            func_8003DB94(r, d, 0);
            *(s32 *)(r + 0xC) = 0x808080;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            v = (u8 *)n + 0x20;
            *(s16 *)(v + 0x10) = 0x20;
            *(s16 *)(v + 0xC) = i;
            if (i == 0)
            {
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800C2824", func_800C5FA8);

/* 15 words, 0 differing. */
void func_800C6AB0(void)
{
    UnkNode *n;

    n = func_8003FF2C(0, (UnkNode *)D_800E58B8, 0x10, (UnkNode **)D_80083498);
    n->unk_10 = (u32)func_800C5FA8;
}

/* Variant B of the node walk, func_800C6AEC and func_800D67B0: the visitor's
   fourth argument is a constant 0 rather than the slot's unk_06, which is
   what moves the `addu $a0, $s0, $zero` above the loop head -- with no `lh`
   to schedule, reorg has a free slot at the bottom of the loop and hoists the
   copy into it. 21 words each, 0 differing. */
s32 func_800C6AEC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C6B40(p, v, s, 0);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

void func_800C6B40(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3)
{
    Unk80083160 *g = &D_80083160;
    Unk453E0SP *p = (Unk453E0SP *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    UnkDungeonFT4 *q;
    Unk44D24E *e;
    s32 w;

    p->unk_EC = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    p->unk_20 = base + 0xB0;
    p->unk_00 = ((u16 *)v)[1];
    p->unk_02 = ((u16 *)v)[3];
    p->unk_04 = ((u16 *)v)[5];
    q = *(UnkDungeonFT4 **)(base + 0x8D0);
    p->unk_C0 = RotTransPers((void *)0x1F800000, (void *)0x1F8000B8, (void *)0x1F800090,
                             (void *)0x1F800094);
    s->unk_14 |= 0x8000;
    w = p->unk_C0 - 0x8;
    D_8006CD30.unk_1C = p->unk_C0 * 4;
    p->unk_C0 = w - arg3;
    if ((u32)p->unk_C0 < 0x1E0)
    {
        PushMatrix();
        p->unk_B8 = p->unk_B8 - 0xA0;
        p->unk_BA = p->unk_BA - 0x78;
        p->unk_30.w = g->unk_C4;
        p->unk_34.w = g->unk_C6;
        p->unk_38.w = g->unk_C8;
        p->unk_100 = s->unk_16;
        p->unk_104 = s->unk_1A - p->unk_34.h;
        p->unk_102 = ((p->unk_38.h + 0x100) & 0x1FF) - 0x100 + s->unk_18;
        p->unk_108 = p->unk_E4 = s->unk_20;
        p->unk_10A = p->unk_E8 = s->unk_22;
        RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
        p->unk_30.w = s->unk_1C;
        p->unk_34.w = s->unk_1E;
        p->unk_38.w = 0;
        ScaleMatrix((void *)0x1F8000D0, (void *)0x1F800030);
        CompMatrix(&D_8006CD30, (void *)0x1F8000D0, (void *)0x1F800050);
        SetTransMatrix((void *)0x1F800050);
        SetRotMatrix((void *)0x1F800050);
        e = (Unk44D24E *)s->unk_08;
        p->unk_24 = s->unk_14;
        for (;;)
        {
            if ((e->unk_00 & 0x20) == 0)
            {
                s16 a;
                s32 du;
                s32 dv;
                s32 dxt;
                s32 dxb;
                s32 dyl;
                s32 dyr;
                s32 yl;
                s32 yr;
                s32 yn;
                s32 yt;
                s32 tv;
                s32 i;
                s32 j;

                p->unk_08.w = e->unk_08;
                p->unk_0C.w = e->unk_09;
                p->unk_10.w = e->unk_0A;
                p->unk_14.w = e->unk_0B;
                if ((e->unk_00 ^ p->unk_24) & 1)
                {
                    a = -(s8)e->unk_02 - p->unk_108;
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a - p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                }
                else
                {
                    a = (s8)e->unk_02 - p->unk_108;
                    p->unk_80 = a;
                    p->unk_70 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
                    p->unk_78 = a;
                }
                if ((e->unk_00 ^ p->unk_24) & 2)
                {
                    a = -(s8)e->unk_03 - p->unk_10A;
                    p->unk_7A = a;
                    p->unk_72 = a;
                    a = a - p->unk_14.h;
                    p->unk_8A = a;
                    p->unk_82 = a;
                }
                else
                {
                    a = (s8)e->unk_03 - p->unk_10A;
                    p->unk_7A = a;
                    p->unk_72 = a;
                    a = a + p->unk_14.h;
                    p->unk_8A = a;
                    p->unk_82 = a;
                }
                RotTransPers4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &p->unk_F0,
                              &p->unk_F4, &p->unk_F8, &p->unk_FC, &p->unk_90, &p->unk_94);
                du = p->unk_10.w / 4;
                dv = p->unk_14.w / 4;
                dxt = (((s16)p->unk_F4 - (s16)p->unk_F0) << 16) / 4;
                dyl = (((s16)p->unk_FA - (s16)p->unk_F2) << 16) / 4;
                dxb = (((s16)p->unk_FC - (s16)p->unk_F8) << 16) / 4;
                dyr = (((s16)p->unk_FE - (s16)p->unk_F6) << 16) / 4;
                tv = p->unk_0C.w;
                yl = ((s16)p->unk_BA + (s16)p->unk_F2) << 16;
                yr = ((s16)p->unk_BA + (s16)p->unk_F6) << 16;
                yn = yr + dyr;
                for (j = 0; j < 4; j++, tv += dv, yl += dyl, yt = yn, yn += dyr, yr = yt)
                {
                    s32 xt;
                    s32 xb;
                    s32 tu;

                    xt = ((s16)p->unk_B8 + (s16)p->unk_F0) << 16;
                    xb = ((s16)p->unk_B8 + (s16)p->unk_F8) << 16;
                    tu = p->unk_08.w;
                    for (i = 0; i < 4; i++, tu += du, xt += dxt, xb += dxb)
                    {
                        q->unk_08 = xt >> 16;
                        q->unk_10 = (xt + dxt) >> 16;
                        q->unk_18 = xb >> 16;
                        q->unk_20 = (xb + dxb) >> 16;
                        q->unk_0A = yl >> 16;
                        q->unk_12 = yr >> 16;
                        q->unk_1A = (yl + dyl) >> 16;
                        q->unk_22 = yn >> 16;
                        if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                             ((u16)(q->unk_10 + 0x20) < 0x181 && (u16)(q->unk_12 + 0x20) < 0x121) |
                             ((u16)(q->unk_18 + 0x20) < 0x181 && (u16)(q->unk_1A + 0x20) < 0x121) |
                             ((u16)(q->unk_20 + 0x20) < 0x181 &&
                              (u16)(q->unk_22 + 0x20) < 0x121)) != 0)
                        {
                            s32 ru;
                            s32 rv;

                            ru = tu + du - s->unk_0C[3];
                            if (tu >= ru)
                            {
                                ru = tu + 1;
                            }
                            rv = tv + dv - s->unk_0C[3];
                            if (tv >= rv)
                            {
                                rv = tv + 1;
                            }
                            if (ru >= 0x100)
                            {
                                ru = 0xFF;
                            }
                            if (rv >= 0x100)
                            {
                                rv = 0xFF;
                            }
                            q->unk_0C.b[0] = tu;
                            q->unk_0C.b[1] = tv;
                            q->unk_14.b[0] = ru;
                            q->unk_14.b[1] = tv;
                            q->unk_1C.b[0] = tu;
                            q->unk_1C.b[1] = rv;
                            q->unk_24.b[0] = ru;
                            q->unk_24.b[1] = rv;
                            setlen(q, 9);
                            s->unk_14 &= 0x7FFF;
                            q->unk_0E = s->unk_12 + e->unk_06;
                            if (s->unk_10 != 0)
                            {
                                q->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
                            }
                            else
                            {
                                q->unk_16 = e->unk_04;
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            setcode(q, 0x2C);
                            if (p->unk_24 & 8)
                            {
                                setSemiTrans(q, p->unk_24 & 4);
                            }
                            AddPrim((void *)(p->unk_20 + p->unk_C0 * 4), q);
                            q++;
                        }
                    }
                }
            }
            else
            {
                s32 (*f)(u8 *, UnkVec *, UnkSlot *, Unk44D24E *, UnkDungeonFT4 *);

                f = *(s32(**)(u8 *, UnkVec *, UnkSlot *, Unk44D24E *, UnkDungeonFT4 *)) & e->unk_08;
                if (f != NULL)
                {
                    s32 r;

                    r = f(arg0, v, s, e, q);
                    if (r > 0)
                    {
                        e = (Unk44D24E *)PHYS_TO_K0(r);
                    }
                    else
                    {
                        q = (UnkDungeonFT4 *)r;
                    }
                }
            }
            if (*(s8 *)e < 0)
            {
                break;
            }
            e++;
        }
        PopMatrix();
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The eleven-spoke fan: allocate the 0x12 parent under D_80083498, dress its
   mover from the (x, z) tile and its slot from D_800777E8, then spawn eleven
   children under it, each one a func_800BB14C step whose mover is the
   parent's offset by (cos, sin) of a 0x155-per-child sweep and whose view
   carries a 5-per-child colour cursor. 148 words, 0 differing.

   ALL FIVE PARAMETERS ARE `s16`, AND THAT IS WHAT PUTS THE SPILLS IN THEIR
   OWN FRAME SLOTS. A narrow parameter is converted in assign_parms, so
   function.c's REG_EQUIV test (`nominal_mode == passed_mode && !
   did_conversion`) fails and the pseudo has no memory equivalence: reload
   gives it a fresh slot at 0x10/0x18/0x1C instead of reusing the incoming
   argument home slot above the frame. Declared `(s32, s32, s16, s32, s32)`
   -- the shape the two landed callers were written against -- the frame is
   0x40 instead of 0x48, `x` is spilled to its home slot and `z` takes $s1,
   and the function is 144 words against 148. Both callers (func_800C2CDC,
   func_800C2F2C) stay at 0 differing under the narrowed declaration.
   THE THIRD PARAMETER IS REASSIGNED `y = d;` BEFORE THE ALLOCATION, and a
   local would not do: stack slots are handed out in ascending PSEUDO number
   (reload1.c's `for (i = LAST_VIRTUAL_REGISTER + 1; i < max_regno; i++)
   alter_reg (i, -1)`), so a local always lands AFTER the parameters and the
   copy of `d` comes out at 0x1C where the target has it at 0x14, between
   `z` and `d`. Only a parameter's own pseudo can sort there. The incoming
   `y` is read into `g` first, which is the `addu $s0, $a2, $zero` the
   target opens with.
   THE +0x10 HANDLER INSTALL GOES BEFORE ITS func_8004491C CALL, in both the
   parent and the child: written after, reorg fills the `jal`'s delay slot
   with the second half of the func_80045C34 address instead of the store
   (3 differing in the child alone).
   `(x << 6) + 0x20` MUST BE A MULTIPLY. As a shift, combine proves only the
   low 16 bits reach the `sh` and drops the sign extension -- `sll $v0, $s1,
   6` where the target has `sll 16` / `sra 10`, one word per axis.
   The view's five halfword stores are written 0x10, 0x12, 0x18, 0x1E, 0x1C:
   that order fills the `lhu 0x18($sp)` load-delay slot with the 0x1E store,
   where 0x10, 0x12, 0x1E, 0x18, 0x1C leaves a `nop` (149 words). */
u8 *func_800C7380(s16 x, s16 z, s16 y, s16 d, s16 e)
{
    UnkNode *n;
    UnkNode *k;
    UnkVec *m;
    UnkVec *w;
    UnkSlot *r;
    u8 *q;
    s16 g;
    s32 i;
    s32 s;
    s32 t;
    s32 u;

    g = y;
    y = d;
    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800BB0A8;
        func_8004491C(n, (u32)func_80045C34);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = z * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = g;
        r = (UnkSlot *)n->unk_0C;
        r->unk_08 = (u32)D_800777E8;
        r->unk_1E = 0x1800;
        r->unk_1C = 0x1800;
        r->unk_10 = 0x20;
        r->unk_22 = 0xFFFB;
        *(u32 *)((u8 *)r + 0xC) = 0x404040;
        r->unk_20 = 0xFFFF;
        r->unk_14 |= 0xC;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x10) = x;
        *(s16 *)(q + 0x12) = z;
        *(s16 *)(q + 0x18) = d;
        *(s16 *)(q + 0x1E) = 0x14;
        *(s16 *)(q + 0x1C) = e;
        i = 0xB;
        s = -0xAA7;
        t = 0xEA7;
        u = 0x37;
        do
        {
            k = func_8003FD64(0x12, n);
            if (k != NULL)
            {
                *(u32 *)((u8 *)k + 0x10) = (u32)func_800BB14C;
                func_8004491C(k, (u32)func_800D0774);
                w = k->unk_08;
                *(s16 *)((u8 *)w + 0x2) = *(u16 *)((u8 *)m + 0x2) + (func_80064584(t) >> 7);
                *(s16 *)((u8 *)w + 0x6) = *(u16 *)((u8 *)m + 0x6) + (func_800644B8(t) >> 7);
                *(s16 *)((u8 *)w + 0xA) = *(u16 *)((u8 *)m + 0xA);
                r = (UnkSlot *)k->unk_0C;
                r->unk_16 = 0xFC00;
                r->unk_18 = s;
                r->unk_1C = 0x410;
                if (*(s16 *)(q + 0x18) < 0)
                {
                    r->unk_10 = 0x40;
                }
                q = (u8 *)k + 0x20;
                *(s16 *)(q + 0x18) = y;
                r->unk_08 = (u32)((u8 *)k + 0x24);
                func_8003DB6C((int *)((u8 *)k + 0x24), (int *)D_800DF3B4, 3);
                q[0xD] = u & 0xBF;
                *(s16 *)(q + 0x1A) = u;
                *(u8 **)q = (u8 *)n;
            }
            s += 0x155;
            t -= 0x155;
            i -= 1;
            u -= 5;
        } while (i > 0);
    }
    return (u8 *)n;
}

/* The camera step func_800C77D0 installs at D_80083178's +0xB4 slot. `p` is
   the record it fills, D_80083178 + 0xB8; D_80083178 itself is reached
   through a `u8 *` local the same way func_800C77D0 reaches it. Retire the
   pending request once its +0x1E turns over (re-seeding D_800E58F8 from the
   mover and pointing +0x0C at it), ease the engine's +0x98 bearing towards
   the request's over the frames left at +0x24, run that counter down with a
   -0x80 floor, then ease the three +0xA4/+0xA6/+0xA8 axes a quarter of the
   way towards the mover's own (x, z, bearing) and halve the two +0x94/+0x96
   rates.

   EVERY READ OF +0x0C IS RE-DEREFERENCED, never cached in a local: each
   halfword store may alias it, so the target reloads `lw 0xC($s0)` after
   every one and then uses ONE load for the three reads in the func_800BCB04
   call, which is the only statement with no store before its reads. A
   cached `m = *(u8 **)(p + 0xC);` is 124 words against 128 with 109
   differing.

   The two trailing halvings read `lhu` and sign-extend at the shift
   (`sll 16`/`sra 18`); `*(s16 *)` and `(s16)*(u16 *)` are byte-identical
   there, because the `sh` truncates either way.

   128 words, 0 differing on the first compile. */
void func_800C75D0(u8 *p)
{
    u8 *g;
    u8 *q;
    u8 *r;

    g = (u8 *)&D_80083178;
    q = *(u8 **)(p + 0x34);
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        r = D_800E58F8;
        *(u32 *)(p + 0x34) = 0;
        *(s16 *)(r + 0x2) = *(u16 *)(*(u8 **)(p + 0xC) + 0x2);
        *(s16 *)(r + 0x6) = *(u16 *)(*(u8 **)(p + 0xC) + 0x6);
        *(s16 *)(r + 0xA) = *(u16 *)(*(u8 **)(p + 0xC) + 0xA);
        *(u8 **)(p + 0xC) = r;
    }
    if (*(s16 *)(p + 0x24) > 0)
    {
        *(s16 *)(g + 0x98) += (*(s16 *)(p + 0x26) - *(s16 *)(g + 0x98)) / *(s16 *)(p + 0x24);
    }
    else
    {
        D_800E3D20 = 0;
    }
    *(s16 *)(p + 0x24) = *(u16 *)(p + 0x24) - 1;
    if (*(s16 *)(p + 0x24) < -0x80)
    {
        *(s16 *)(p + 0x24) = -0x80;
    }
    *(s16 *)(p + 0x4) = *(u16 *)(*(u8 **)(p + 0xC) + 0x2);
    *(s16 *)(p + 0x6) = *(u16 *)(*(u8 **)(p + 0xC) + 0x6);
    *(s16 *)(p + 0x8) = func_800BCB04(*(u16 *)(*(u8 **)(p + 0xC) + 0x2), *(u16 *)(*(u8 **)(p + 0xC) + 0x6),
                                      *(u16 *)(*(u8 **)(p + 0xC) + 0xA) - 0x20);
    if (*(s16 *)(p + 0x8) > 0x200)
    {
        *(s16 *)(p + 0x8) = *(u16 *)(*(u8 **)(p + 0xC) + 0xA);
    }
    *(s16 *)(g + 0xA4) += (*(s16 *)(p + 0x4) - *(s16 *)(g + 0xA4)) >> 2;
    *(s16 *)(g + 0xA6) += (*(s16 *)(p + 0x6) - *(s16 *)(g + 0xA6)) >> 2;
    if (D_800DF63C != 0)
    {
        *(s16 *)(g + 0xA8) += (*(s16 *)(p + 0x8) - *(s16 *)(g + 0xA8)) >> 2;
    }
    D_800DF63C = *(s16 *)(g + 0xA8) != *(s16 *)(p + 0x8);
    *(s16 *)(g + 0x94) = *(s16 *)(g + 0x94) >> 2;
    *(s16 *)(g + 0x96) = *(s16 *)(g + 0x96) >> 2;
}

/* THE TWO RANGE TESTS ARE abs TERNARIES, not `if (d < 0) d = -d;`: only
   `d >= 0 ? d : -d` reaches fold's ABS_EXPR, and only an ABS_EXPR's
   `bgez`/nop/`negu` carries its own delay slot. Written as an `if` the three
   words are a real branch, reorg steals the $a2 copy into its slot, and the
   function is 9 differing. D_80083780 needs a POINTER LOCAL -- read as
   `*(s16 *)(D_80083780 + 2)` the `%lo` folds into the load displacement and
   the second read walks the pointer, 53 words.

   THE THIRD PARAMETER IS s16 AND THE FOURTH IS s32, and only the CALLER says
   so: func_800C78A0 sign-extends $a2 across the `jal` and passes $a3 straight
   out of a `lw` with no extension. Both are stored with `sh` here, so the
   callee is 0 differing either way; `s16 d` costs func_800C78A0 two words.
   52 words, 0 differing. */
s32 func_800C77D0(void *a, u8 *b, s16 c, s32 d)
{
    u8 *q;
    u8 *s;
    u8 *p;
    s32 r;
    s32 dx;
    s32 dz;

    q = D_80083780;
    dx = *(s16 *)(q + 2) - *(s16 *)(b + 2);
    dx = dx >= 0 ? dx : -dx;
    if (dx >= 0xC1)
    {
        return 0;
    }
    dz = *(s16 *)(q + 6) - *(s16 *)(b + 6);
    dz = dz >= 0 ? dz : -dz;
    if (dz >= 0xC1)
    {
        return 0;
    }
    s = (u8 *)&D_80083178;
    p = s + 0xB8;
    r = (*(u32 *)(p + 0xC) != (u32)b);
    *(u32 *)(p + 0x34) = (u32)a;
    *(u32 *)(p + 0xC) = (u32)b;
    *(u32 *)(p + 0x28) = 0;
    *(u32 *)(s + 0xB8) = (u32)(s + 0xBC);
    if (*(u32 *)(p + 0x14) != 9)
    {
        r |= 2;
    }
    *(u32 *)(p + 0x14) = 9;
    *(u32 *)(p + 0x18) = 0;
    *(s16 *)(p + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_800C75D0;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(p + 0x24) = c;
    return r;
}

/* Fire func_800C77D0 at (x, z) only while both axes are within 0xC0 of
   D_80083780's own position. THE FIRST THREE COORDINATE PARAMETERS ARE `s32`,
   not `s16`: the target subtracts the incoming registers raw and only
   truncates them at the `sh` stores, so an `s16` declaration adds a
   `sll 16`/`sra 16` pair per axis -- 37 words against 36 with 35 differing.
   The FIFTH argument, on the stack, IS narrow: it is `lw`-ed at entry and
   sign-extended at the call, which is what a promoted `s16` stack parameter
   gives. 36 words, 0 differing. */
void func_800C78A0(u8 *a, s32 x, s32 z, s32 ang, s16 e, s32 f)
{
    u8 *g;
    u8 *r;
    s32 d;

    g = D_80083780;
    d = *(s16 *)(g + 0x2) - x;
    if (d < 0)
    {
        d = -d;
    }
    if (d < 0xC1)
    {
        d = *(s16 *)(g + 0x6) - z;
        if (d < 0)
        {
            d = -d;
        }
        if (d < 0xC1)
        {
            r = D_800E58F8;
            *(s16 *)(r + 0x2) = x;
            *(s16 *)(r + 0x6) = z;
            *(s16 *)(r + 0xA) = ang;
            func_800C77D0(a, r, e, f);
        }
    }
}

/* Nothing happens unless D_80083780's position is within 0xC0 of the
   caller's on both axes. Both bounds are the `bgez`/nop/`negu` trio, so
   `abs()` with the same destination. The two tables are indexed
   `[(f >> 9) & 7]` on a u16 array -- cc1psx folds the element scale into the
   mask and emits `srl 8` / `andi 0xE`. The third parameter is s16, its
   `sll`/`sra` pair sitting at the forwarding call; the fourth is forwarded in
   $a3 with no setup. 67 words, 0 differing. */
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 d)
{
    u8 *q;
    u8 *v;
    u8 *e;

    q = D_80083780;
    if (abs(*(s16 *)(q + 0x2) - *(s16 *)(b + 0x2)) < 0xC1 &&
        abs(*(s16 *)(q + 0x6) - *(s16 *)(b + 0x6)) < 0xC1)
    {
        v = a + 0x20;
        e = D_800E58F8;
        *(s16 *)(e + 0x2) = *(u16 *)(b + 0x2) + (s16)D_800DCEAC[(*(u16 *)(v + 0x2A) >> 9) & 7] / 2;
        *(s16 *)(e + 0x6) = *(u16 *)(b + 0x6) + (s16)D_800DCEBC[(*(u16 *)(v + 0x2A) >> 9) & 7] / 2;
        *(s16 *)(e + 0xA) = *(u16 *)(b + 0xA);
        func_800C77D0(a, e, c, d);
    }
    return 0;
}

/* func_800C78A0's two-actor form: fire func_800C77D0 at the midpoint of the
   two actors' cell centres. THREE measured spellings carry it.

   (1) The two cell centres must be SEPARATE LOCALS. As one expression fold
   cancels the two +0x20s against each other and each axis loses its second
   `addiu`: 59 of 63 words.
   (2) THE SECOND AXIS'S ROUNDING HAS TO BE WRITTEN OUT. `q /= 2` compiles to
   a bare `sra`, not the target's `srl 31`/`addu`/`sra` triple, because
   nonzero_bits' MINUS case gives `result_low = MIN(low0, low1)` and
   `(byte << 6) + 0x20` has five low zero bits, so the rounding correction is
   provably redundant. Six spellings measure 61 words; only the two-statement
   form keeps it. The corollary: the FIRST axis's single `sra` is NOT evidence
   of a `>> 1` -- the two spellings are byte-identical there.
   (3) The frame is 0x20 where its one four-argument call needs 0x18, so there
   are 5..8 bytes of stack local nothing in the body reads -- a leftover
   declaration in the original. No local is 0x18 (4 differing), a 4-byte local
   is still 0x18 (6), an 8-byte local is 0x20 and 0. Only its SIZE is
   witnessed, never its type. 63 words, 0 differing. */
void func_800C7A3C(u8 *a, u8 *b, s16 c, s16 d, s16 e, s32 f)
{
    u8 *g;
    u8 *r;
    s32 p;
    s32 q;
    s32 x;
    s32 z;
    s32 t;
    s32 unused[2];

    p = (a[0x24] << 6) + 0x20;
    q = (b[0x24] << 6) + 0x20;
    q -= p;
    q >>= 1;
    x = p + q;
    g = D_80083780;
    t = *(s16 *)(g + 0x2) - x;
    if (t < 0)
    {
        t = -t;
    }
    if (t < 0xC1)
    {
        r = D_800E58F8;
        *(s16 *)(r + 0x2) = x;
        p = (a[0x25] << 6) + 0x20;
        q = (b[0x25] << 6) + 0x20;
        q -= p;
        q += (u32)q >> 31;
        q >>= 1;
        z = p + q;
        t = *(s16 *)(g + 0x6) - z;
        if (t < 0)
        {
            t = -t;
        }
        if (t < 0xC1)
        {
            *(s16 *)(r + 0x6) = z;
            *(s16 *)(r + 0xA) = c + (d - c) / 2;
            func_800C77D0(NULL, r, e, f);
        }
    }
}

/* The camera pull-back: state 0 eases D_80083178's +0xAC/+0xB0/+0xA4/+0xA6
   (and +0xA8 over half the countdown) towards the record's own targets and,
   when the countdown expires, snaps them and advances; state 1 runs
   func_800C77D0 once and drops D_80083460's +0xA counter. 135 words,
   0 differing.

   D_800DD264 NEEDS ITS OWN POINTER LOCAL AT EACH OF ITS TWO SITES. Written
   `D_800DD264[D_800120A2]` the index's `%hi` is what reorg steals into the
   `blez`/`bgtz` delay slots and the base's `lui` lands next to the `addiu`
   -- 135 words, 6 differing, all six a `lui`/`addiu` register. With a local
   per site the base's `high` is hoisted into the slot instead and it is 0;
   ONE local shared by both sites is 134 words / 121 differing.

   D_800120A2 IS KEPT AS THE DECLARED SYMBOL. `*(s16 *)0x800120A2` also
   measures 0 differing under ovlverify, but it emits one relocation fewer
   than the listing carries; the symbol spelling closes seqcheck at
   raw == relocdump == 22 with seq=OK. */
void func_800C7B38(u8 *p)
{
    u8 *g;
    u8 *h;
    s16 d;
    s16 *t;
    s16 *u;

    g = (u8 *)&D_80083178;
    switch (*(s32 *)(p + 0x18))
    {
    case 0:
        if (*(s16 *)(p + 0x24) > 0)
        {
            *(s32 *)(p + 0x10) -= *(s32 *)(p + 0x10) >> 2;
            *(s16 *)(g + 0x98) = *(u16 *)(p + 0x26) + *(u16 *)(p + 0x10);
            t = D_800DD264;
            *(s16 *)(g + 0xAC) += (t[D_800120A2] - *(s16 *)(g + 0xAC)) / *(s16 *)(p + 0x24);
            *(s16 *)(g + 0xB0) += (-*(s16 *)(g + 0xB0)) / *(s16 *)(p + 0x24);
            *(s16 *)(g + 0xA4) +=
                (*(s16 *)(*(u8 **)(p + 0xC) + 0x2) - *(s16 *)(g + 0xA4)) / *(s16 *)(p + 0x24);
            *(s16 *)(g + 0xA6) +=
                (*(s16 *)(*(u8 **)(p + 0xC) + 0x6) - *(s16 *)(g + 0xA6)) / *(s16 *)(p + 0x24);
            d = *(s16 *)(p + 0x24) / 2;
            if (d != 0)
            {
                *(s16 *)(g + 0xA8) += (*(s16 *)(*(u8 **)(p + 0xC) + 0xA) - *(s16 *)(g + 0xA8)) / d;
            }
            if (--*(s16 *)(p + 0x24) > 0)
            {
                break;
            }
        }
        *(s16 *)(g + 0xB0) = 0;
        u = D_800DD264;
        *(s16 *)(g + 0xAC) = u[D_800120A2];
        *(s16 *)(g + 0x98) = *(u16 *)(p + 0x26);
        *(s32 *)(p + 0x18) += 1;
        break;
    case 1:
        func_800C77D0(D_800E3D7C - 0x20, D_80083780, 8, D_800DCE66);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        break;
    }
}

/* Run one cue list: func_800BD184 on the byte D_800DF64C[k] names, then every
   entry of D_800DF640[k] until its function pointer is NULL. The second
   walked register is loop.c combining the four halfword reads into one
   address giv biased to the last of them (+0xA), which is why the listing
   reads at -6/-4/-2/0. 38 words, 0 differing. */
void func_800C7D54(s16 k)
{
    UnkDungeonCue *q;

    func_800BD184(D_800DF64C[k]);
    for (q = D_800DF640[k]; q->unk_00 != NULL; q++)
    {
        q->unk_00(q->unk_04, q->unk_06, q->unk_08, q->unk_0A);
    }
}

/* The "can this hit land" gate: refuse while the attacker's +0x4C record
   carries a 0x8000-flagged entry, while the combined +0x28/+0x64 total is
   not positive, and while the defender's quartered +0x64 does not exceed
   the negated total; then halve the margin for kinds 1 and 2, refuse kind
   10, and commit. 95 words, 0 differing.

   THE HALVING IS OF `w`, NOT OF `v`, and that one character was the whole
   residue. Written `w = (v - 1) / 2;` the function is 95 words with 13
   differing, which reads exactly like an allocation rotation -- two locals
   in each other's registers and the delay slot that follows them -- and
   twenty-odd spellings of the locals, the declaration order, the widths and
   the tests do not move it. The cause is that `v` is DEAD after the
   comparison in the target: `sll`/`sra $v1, $a0` overwrites v's register
   with the sign-extended w, and the later `addiu $v0, $v1, -0x1` reads that,
   not the total. A value the target lets die has its register reused, and
   that is indistinguishable from a bad allocation until you check which
   variable the arithmetic actually reads.

   decomp-permuter reached 0 here too, in 76k candidates, but its answer was
   `if (-v >= (v = w))` -- an unsequenced read and write of one object in one
   expression, i.e. undefined behaviour that happens to emit the right code
   because it models v dying. The honest spelling is better and is what is
   landed. */
s32 func_800C7DEC(u8 *a, u8 *b)
{
    u8 *c;
    u8 *d;
    s32 e;
    s32 v;
    s16 w;

    if (*(s32 *)(b + 0x54) & 0x8000)
    {
        c = *(u8 **)(a + 0x4C);
        if (c != NULL)
        {
            if ((*(s16 *)(D_80073414[c[1]].unk_0C + c[0] * 0x14) & 0x8000) == 0)
            {
                return 0;
            }
        }
        v = (s16)(a[0x28] + *(u16 *)(a + 0x64));
        if (v <= 0)
        {
            return 0;
        }
        w = (s16) * (u16 *)(b + 0x64) >> 2;
        if (w == 0)
        {
            w = -1;
        }
        if (-v >= w)
        {
            return 0;
        }
        d = *(u8 **)(a + 0x50);
        if (d != NULL)
        {
            e = d[0];
            if (e == 1 || e == 2)
            {
                w = (w - 1) / 2;
            }
            else if (e == 10)
            {
                return 0;
            }
        }
        *(s16 *)(a + 0x64) = w;
        D_80083470 = a - 0x20;
        *(s32 *)(a + 0x60) = 0;
        func_800A5A18(*(UnkVec **)(a - 0x18), a);
        func_800A56E0(0x60F);
        *(s32 *)(a + 0x1C) |= 0x20000000;
        if (a[0x13] == 0)
        {
            D_800E3E41 = 2;
        }
        return 1;
    }
    return 0;
}

/* True while anything is still blocking. THE LAST TEST HAS TO SHARE THE
   FUNCTION'S EXIT with the loop's, as an `else if` rather than a third
   top-level `if`: written flat, jump.c store-flags the final
   `return 1`/`return 0` pair into one `sltu $v0, $zero, $v0` and the
   function is 35 words against 37 with 10 differing. 37 words,
   0 differing. */
s32 func_800C7F68(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *e;

    if (D_800E296C & 0x10000000)
    {
        return 1;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        q = D_800E3D7C + 4;
        for (i = 1; i >= 0; i--, q -= 4)
        {
            e = *(u8 **)(q + 0xAC);
            if (e != NULL && (*(u32 *)(e + 0x54) & 0x20000))
            {
                return 1;
            }
        }
    }
    else if (*(u32 *)(p + 0x54) & 0x20000)
    {
        return 1;
    }
    return 0;
}

/* Six copies of one flag probe, one per bit. With the record's +0x14 bit
   0x4000 set the test runs over the engine's two-entry party table instead
   -- D_800E3D7C's +0xAC pointers, walked DOWNWARDS from entry 1, which is
   what the `addiu $v1, $v1, -0x4` says -- and otherwise it is the record's
   own +0x54.

   THE SINGLE TRAILING `return 0` IS THE SHAPE, and it is worth two words per
   member. Give each arm its own `return 0` and jump.c store-flags the last
   pair into one `sltu $v0, $zero, $v0`; with one shared exit the branch
   survives, and the `return` in each arm is also what keeps $v0 live into
   the epilogue so the else arm's mask chain lands in $v1/$a0 rather than
   $v0/$v1 (docs/matching-levers.md, "Duplicate the tail per arm"). Measured:
   six tail spellings -- bool return, explicit else, inverted test, goto,
   local, do-while loop -- all give 29 words and 9 differing; this gives 31
   and 0. 31 words each, 0 differing. */
s32 func_800C7FFC(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x20000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x20000000)
    {
        return 1;
    }
    return 0;
}

/* Does anything hold the 0x4000 flag? THE LAST TEST MUST SHARE THE
   FUNCTION'S ONE `return 0`: written flat, jump.c store-flags the final 1/0
   pair into a single `sltu $v0, $zero, $v0` and the function is 28 words / 8
   differing. 30 words, 0 differing. */
s32 func_800C8078(u8 *p)
{
    u8 *base;
    u8 *q;
    s32 i;

    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        base = D_800E3D7C;
        for (i = 1; i >= 0; i--)
        {
            q = *(u8 **)(base + i * 4 + 0xAC);
            if (q != NULL && (*(u32 *)(q + 0x54) & 0x4000))
            {
                return 1;
            }
        }
    }
    else if (*(u32 *)(p + 0x54) & 0x4000)
    {
        return 1;
    }
    return 0;
}

/* Does either of the engine's two +0xAC slots hold a record with 0x10000 set?
   The 0x4000 gate and the exhausted loop share the one `return 0`.
   24 words, 0 differing. */
s32 func_800C80F0(u8 *p)
{
    u8 *base;
    u8 *q;
    s32 i;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        base = D_800E3D7C;
        for (i = 1; i >= 0; i--)
        {
            q = *(u8 **)(base + i * 4 + 0xAC);
            if (q != NULL)
            {
                if (*(s32 *)(q + 0x54) & 0x10000)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* The 0x4000-flagged actor's "cannot act" gate: refuse while any of the two
   party slots carries +0x54 bit 0x40000, otherwise bump the +0x12 counter,
   publish it into the 0x80012098 table at the key func_800A1BD0 returns and
   report; for a plain actor it just forwards to func_800C8900.
   90 words, 0 differing on the first compile.

   D_80012098 IS THE CONSTANT BASE 0x80010000 WITH 0x2098 INSIDE THE INDEX,
   exactly as the landed func_8009458C spells it -- see the note there for
   the three spellings that are not it. */
s32 func_800C8150(u8 *p, s32 b, s32 c)
{
    u8 *base;
    u8 *q;
    u8 *t;
    s32 i;
    s16 r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        base = D_800E3D7C;
        for (i = 1; i >= 0; i--)
        {
            q = *(u8 **)(base + i * 4 + 0xAC);
            if (q != NULL && (*(s32 *)(q + 0x54) & 0x40000))
            {
                func_80099844(p, D_800E18F5);
                return 0;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x40000)
    {
        func_80099844(p, D_800E18F5);
        return 0;
    }
    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        if (p[0x12] != 4 && p[0x13] != 0)
        {
            p[0x12] = p[0x12] + 1;
            r = func_800A1BD0(p);
            if (r >= 0)
            {
                t = (u8 *)0x80010000;
                *(u16 *)(t + r * 2 + 0x2098) = p[0x12];
            }
            func_80099844(p, D_800E195C);
            return 1;
        }
        func_800C8900(p, b, c);
        return 1;
    }
    return (s16)func_800C8900(p, b, c) >= 0;
}

/* Count the engine's two occupied slots, but only for the player's own record
   and only while its +0x54 carries 8. The loop counts DOWN from 1 with the
   pointer biased +4 and decremented. 22 words, 0 differing. */
s32 func_800C82B8(u8 *p)
{
    s32 i;
    u8 *q;
    s32 n;

    n = 0;
    if (p == D_800E3D7C)
    {
        for (i = 1, q = p + 4; i >= 0; i--, q -= 4)
        {
            if (*(s32 *)(q + 0xAC) != 0)
            {
                if ((*(s32 *)(p + 0x54) & 8) != 0)
                {
                    n++;
                }
            }
        }
    }
    return n;
}

s32 func_800C8310(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x00080000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x00080000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C838C(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x08000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x08000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8408(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x02000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x02000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8484(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x10000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x10000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8500(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x00100000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x00100000)
    {
        return 1;
    }
    return 0;
}

/* The two-state swap between 0xB and 0xC with a percentage roll in front of
   it. THREE SPELLINGS carry it: `s32 v = func_800A6D30();` with the u16
   returner, so the `andi 0xFFFF` lands at the assignment; `d` HOLDS THE
   SHIFT, so the two arms share one `srl` (written out twice it is 103
   differing); and `s16 res`, not `s32` -- every test of it is `sll 16` then
   `bltz`/`bgez` and the return is the `nor`/`srl 31` of `res >= 0`.
   131 words, 0 differing. */
s32 func_800C857C(u8 *p, u8 *q)
{
    void *m;
    void *h;
    s32 v;
    s32 n;
    s32 d;
    s16 res;

    res = -1;
    if (func_800C8500(q) != 0)
    {
        if (*(u32 *)(q + 0x14) & 0x4000)
        {
            func_80099844(q, D_800E196F);
        }
        return 0;
    }
    v = func_800A6D30();
    d = p[0x11] >> 2;
    n = (p[3] != 0) ? (v % p[3] + d) : d;
    if (n >= 0x30)
    {
        if (*(u32 *)(p + 0x1C) & 0x2000)
        {
            if (!(*(u32 *)(q + 0x1C) & 0x2000))
            {
                if ((s16)func_80042900((s8 *)q, 0xC) != 0)
                {
                    func_80042B68((s8 *)q, 0xC);
                    res = 0;
                }
                else
                {
                    res = func_800A48F0((s8 *)q, 0xB, 0x10);
                    if (res >= 0)
                    {
                        h = func_800990FC();
                        m = h;
                        h = func_80099194(D_800E1987, h);
                        h = func_80099734(q, h);
                        h = func_80099194(D_8008942C, h);
                        func_80099290(h);
                        func_800A5720(m);
                    }
                }
            }
        }
        else if (*(u32 *)(q + 0x1C) & 0x2000)
        {
            if ((s16)func_80042900((s8 *)q, 0xB) != 0)
            {
                func_80042B68((s8 *)q, 0xB);
                res = 0;
            }
            else
            {
                res = func_800A48F0((s8 *)q, 0xC, 0x10);
                if (res >= 0)
                {
                    func_80099844(q, D_800E1996);
                }
            }
        }
    }
    if (res < 0)
    {
        if (*(u32 *)(q + 0x14) & 0x4000)
        {
            func_80099844(q, D_800E19AA);
        }
    }
    return res >= 0;
}

/* The `nor`/`srl 31` pair is `>= 0` on the sign-extended halfword.
   15 words, 0 differing. */
s32 func_800C8788(u8 *p, u8 *q)
{
    u8 v;

    v = p[0x11];

    return func_800C8DB0(q, v << 2, (v >> 2) + 0x10) >= 0;
}

/* And the three callers of those: report the outcome in a message, with a
   second message through the overlay's own slot when the record's +0x13
   byte is clear. 32 words each, 0 differing. */
s32 func_800C87C4(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8844(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E19C2);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF1C);
        }
        return 1;
    }
    func_80099844(p, D_800E19D3);
    return 0;
}

/* The probe's three callers: refuse outright if the flag is set, then draw
   a random number modulo the record's +0x03 and compare it with the chance
   the caller passed -- 0xFF being the unconditional case -- and on a pass
   apply the effect, which succeeds on a NON-NEGATIVE halfword result.

   TWO LOCALS, NOT ONE, and the types are both forced. The draw is an s32,
   which is where the `andi $a0, $v0, 0xFFFF` comes from -- func_800A6D30 is
   declared u16 and the widening is at the assignment -- and the remainder is
   an s16. Written as one variable (`roll = func_800A6D30(); roll = roll %%
   p[3];`) the pseudo lives from the call to the compare, which is longer
   than the promoted `chance`, so local-alloc hands $v1 to chance and $a0 to
   the remainder, the `addu $a0, $s0, $zero` for the next call fills the
   `bnez` delay slot, and the function comes out 46 words and 23 differing.
   Measured across eleven spellings; only the split pair with these two types
   reaches 0. Same single trailing `return 0` as the probes above.
   47 words each, 0 differing. */
s32 func_800C8844(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C7FFC(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x5, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C8900(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8980(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E19E8);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF18);
        }
        return 1;
    }
    func_80099844(p, D_800E1A01);
    return 0;
}

s32 func_800C8980(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C8484(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x4, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C8A3C(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8ABC(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E1A1D);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF0C);
        }
        return 1;
    }
    func_80099844(p, D_800E1A2D);
    return 0;
}

/* The fourth member of the func_800C8844 probe family: a different gate,
   effect 1 instead of 5, and the chance compared against HALF the caller's
   value -- the `srl 31` / `addu` / `sra 1` trio is the signed `/ 2`, and it
   is the whole 50-against-47-word difference. 50 words, 0 differing. */
s32 func_800C8ABC(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C8408(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance / 2 || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x1, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

/* func_800CD294's gate on its own. The HIT arm is written first -- the
   target's `beqz` puts the miss arm out of line, and inverting the test is
   38 words at 21 differing. 38 words, 0 differing. */
s32 func_800C8B84(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8C1C(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E1A44);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF14);
        }
        return 1;
    }
    if (*(u32 *)(p + 0x1C) & 0x10)
    {
        return 0;
    }
    func_80099844(p, D_800E191B);
    return 0;
}

s32 func_800C8C1C(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C838C(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x3, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

/* The func_800C8844 probe's two locals (s32 draw, s16 remainder) with
   func_800C8900's report tail folded in and no leading gate call. 54 words,
   0 differing. */
s32 func_800C8CD8(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    r = func_800A6D30();
    if (p[3] != 0)
    {
        roll = r % p[3];
    }
    else
    {
        roll = 0;
    }
    if (roll < chance || chance == 0xFF)
    {
        if (func_800A48F0((s8 *)p, 0x2, (s8)arg) >= 0)
        {
            func_80099844(p, D_800E1A55);
            if (p[0x13] == 0)
            {
                func_800DC1B8(D_800DCF10);
            }
            return 1;
        }
    }
    return 0;
}

/* The seventh flag probe, func_800C8844's recipe with its own guard and its
   own report. TWO LOCALS, NOT ONE, exactly as func_800C8844's comment
   records: `s32 r` for func_800A6D30's draw (that is where the
   `andi $a0, 0xFFFF` comes from) and `s16 roll` for the remainder.
   83 words, 0 differing on the first compile. */
s16 func_800C8DB0(u8 *p, s32 chance, s32 arg)
{
    s32 r;
    s16 roll;

    if (p[0x13] != 0 && ((*(s32 *)(p + 0x8) & 0xFF0000FF) != 0 || p[0xE] != 0) &&
        p[0x8] + p[0xB] + p[0xE] != 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < (s16)chance || (s16)chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x6, (s8)arg) >= 0)
            {
                func_80099844(p, D_800E1937);
                if (p[0x13] == 0)
                {
                    func_800DC1B8(D_800DCF20);
                }
                return 1;
            }
        }
    }
    if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
    {
        func_80099844(p, D_800E1A6D);
    }
    return 0;
}

/* The hunger/charge drain effect. The LAST func_80099194 is written in both
   arms so jump.c cross-jumps it into the one call the target has. The roll is
   func_800C8844's two forced locals, an s32 draw and an s16 remainder.
   78 words, 0 differing. */
s32 func_800C8EFC(u8 *p, s16 chance)
{
    s32 r;
    s16 roll;
    void *m;
    void *h;

    if (func_800C8078(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (p[0x26] >= 2)
            {
                p[0x26] = p[0x26] - 1;
                func_80041E70(p);
                h = func_800990FC();
                m = h;
                h = func_80099194(D_800E1A88, h);
                if (*(s32 *)(p + 0x14) & 0x4000)
                {
                    h = func_80099734(p, h);
                    h = func_80099194(D_800E1A9D, h);
                }
                else
                {
                    h = func_80099194(D_800E1AC2, h);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800E1AD0, h);
                }
                func_80099290(h);
                func_800A5720(m);
                func_800A56E0(0x615);
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C9034(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C9088(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

void func_800C9088(u8 *p, UnkVec *v, UnkSlot *s, s16 depth)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSP4 *sp = (UnkDungeonSP4 *)0x1F800000;
    u8 m[0x20];
    u32 *d;
    SVECTOR *tbl;
    u8 *tex;
    UnkDungeonFace *f;
    POLY_GT4 *q;

    PushMatrix();
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_38 = s->unk_20;
    sp->unk_40 = ((s16 *)v)[1];
    sp->unk_44 = ((s16 *)v)[3];
    sp->unk_48 = ((s16 *)v)[5];
    ReadRotMatrix((MATRIX *)m);
    sp->unk_50.m[0][0] = sp->unk_50.m[1][1] = sp->unk_50.m[2][2] = 0x1000;
    sp->unk_50.m[0][1] = sp->unk_50.m[0][2] = sp->unk_50.m[1][0] = sp->unk_50.m[1][2] =
        sp->unk_50.m[2][0] = sp->unk_50.m[2][1] = 0;
    RotMatrixX(s->unk_16, (MATRIX *)0x1F800050);
    RotMatrixZ(s->unk_1A, (MATRIX *)0x1F800050);
    RotMatrixY(s->unk_18, (MATRIX *)0x1F800050);
    if (s->unk_14 & 1)
    {
        sp->unk_30 = -sp->unk_30;
    }
    if (s->unk_14 & 2)
    {
        sp->unk_34 = -sp->unk_34;
    }
    ScaleMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800030);
    TransMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800040);
    CompMatrix((MATRIX *)0x1F800050, (MATRIX *)p, (MATRIX *)0x1F8000D0);
    CompMatrix((MATRIX *)m, (MATRIX *)0x1F8000D0, (MATRIX *)0x1F800050);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    sp->unk_20 = (u32)(D_80083160.unk_00 + 0xB0);
    q = (POLY_GT4 *)*(u32 *)(D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    d = (u32 *)s->unk_08;
    f = (UnkDungeonFace *)d[0];
    tbl = (SVECTOR *)d[1];
    tex = (u8 *)d[2];
    for (;;)
    {
        long sxy0, sxy1, sxy2;

        *(u32 *)&sp->unk_70[0] = *(u32 *)&tbl[f->unk_00];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&tbl[f->unk_02];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&tbl[f->unk_04];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&tbl[f->unk_06];
        sp->unk_70[0].vz = tbl[f->unk_00].vz;
        sp->unk_70[1].vz = tbl[f->unk_02].vz;
        sp->unk_70[2].vz = tbl[f->unk_04].vz;
        sp->unk_70[3].vz = tbl[f->unk_06].vz;
        sp->unk_C0 = RotAverage4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                 &q->x0, &q->x1, &q->x2, &q->x3, &sp->unk_90, &sp->unk_94);
        switch (s->unk_14 & 3)
        {
        case 0:
            sxy0 = *(s32 *)&q->x0;
            sxy1 = *(s32 *)&q->x1;
            sxy2 = *(s32 *)&q->x2;
            break;
        case 1:
            sxy0 = *(s32 *)&q->x1;
            sxy1 = *(s32 *)&q->x0;
            sxy2 = *(s32 *)&q->x2;
            break;
        case 2:
            sxy0 = *(s32 *)&q->x2;
            sxy1 = *(s32 *)&q->x1;
            sxy2 = *(s32 *)&q->x0;
            break;
        default:
            sxy0 = *(s32 *)&q->x2;
            sxy1 = *(s32 *)&q->x0;
            sxy2 = *(s32 *)&q->x1;
            break;
        }
        sp->unk_114 = NormalClip(sxy0, sxy1, sxy2);
        if (sp->unk_114 > 0)
        {
            sp->unk_C0 = sp->unk_C0 - depth;
            if ((u32)sp->unk_C0 < 0x1E0)
            {
                u16 t;

                func_80065034(tex + f->unk_10 * 8, s->unk_0C, (u8 *)&q->r0);
                func_80065034(tex + f->unk_12 * 8, s->unk_0C, (u8 *)&q->r1);
                func_80065034(tex + f->unk_14 * 8, s->unk_0C, (u8 *)&q->r2);
                func_80065034(tex + f->unk_16 * 8, s->unk_0C, (u8 *)&q->r3);
                *(u32 *)&q->u0 = f->unk_08;
                q->clut = q->clut + s->unk_12;
                *(u32 *)&q->u1 = f->unk_0C;
                t = s->unk_10;
                if (t != 0)
                {
                    q->tpage = t + (q->tpage & 0xFF9F);
                }
                *(u16 *)&q->u2 = f->unk_1A;
                *(u16 *)&q->u3 = f->unk_1C;
                SetPolyGT4(q);
                if (s->unk_14 & 8)
                {
                    if (s->unk_14 & 4)
                    {
                        q->code |= 2;
                    }
                }
                AddPrim((void *)(sp->unk_20 + sp->unk_C0 * 4), q);
                q++;
                s->unk_14 &= 0x7FFF;
            }
        }
        if (f->unk_1E == 1 && f->unk_1F < 0)
        {
            break;
        }
        f++;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The node walk behind func_800C96E8. Same `while (1)` shape as the landed
   func_800D5AC0, including `p = (u8 *)n + 0x20;` ahead of `v = n->unk_08;`.
   41 words, 0 differing. */
s32 func_800C95C0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDungeonSP2 *sp = (UnkDungeonSP2 *)0x1F800000;
    UnkNode *n;

    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        sp->unk_C0 =
            RotTransPers((SVECTOR *)&sp->unk_00, &sp->unk_B8, &sp->unk_90, &sp->unk_94) -
            s->unk_06;
        func_800C96E8(p, sp);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    return 0;
}

/* Walk a node and its +0x18 chain, projecting each one through the
   0x1F800000 scratchpad. +0xB8/+0xBA are inside Unk463ECSP's pad_98 and
   reached by cast; +0xC0 is its own field. 33 words, 0 differing. */
s32 func_800C9664(u8 *p, UnkVec *m)
{
    Unk463ECSP *sp;
    UnkNode *n;

    sp = (Unk463ECSP *)0x1F800000;
    for (;;)
    {
        *(s16 *)((u8 *)sp + 0xB8) = *(u16 *)((u8 *)m + 0x2) - (*(s16 *)p >> 1);
        *(s16 *)((u8 *)sp + 0xBA) = *(u16 *)((u8 *)m + 0x6);
        sp->unk_C0 = *(s16 *)((u8 *)m + 0xA);
        func_800C96E8(p, sp);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        m = (UnkVec *)n->unk_08;
    }
    return 0;
}

/* Clip a 4-halfword source/destination rect pair against the 0x140x0xE0
   frame buffer and, if both fit, emit a DR_MOVE at D_80083160's +0x8D0
   cursor onto the +0xB0 OT row indexed by the scratchpad's +0xC0.

   Three spellings carried this one, each measured:
   (1) THE TWO SUMS GO IN LOCALS AHEAD OF THE TEST CHAIN. Written as one
   nine-term `&&` the loads are emitted lazily, one per test block, and the
   `lhu` for r.x lands in the first block: 91 words, 68 differing. With `dx`
   and `dy` computed first all five loads sit in the entry block as the
   target has them.
   (2) `bx` IS A LOCAL BUT THE 0xBA READ IS NOT. The stores into `r` kill
   cse's entry for both scratchpad reads, so the +0x4 and +0xBA reads are
   re-done at the call; the target re-reads only +0xBA, which is a local for
   +0xB8 and none for +0xBA. Caching neither is one word long (91).
   (3) THE BIAS GOES THROUGH AN `s32 t` AND IS READ BACK OUT OF `r.y`.
   Accumulating in `r.y` itself stores it twice and reloads it (90 words, 32
   differing); an `s16 t` loads it with `lhu` where the target has `lh`.
   The final +0x8D0 store is the ONLY access through a pointer local, which
   is what buys $s3's lo_sum beside $s2's high; all three through `g` is 93
   words, all three direct is 89 and costs the $s3 pair.

   THE SECOND PARAMETER IS `void *`, not `u8 *`: the three landed callers
   pass UnkDungeonSP2 * and Unk463ECSP *, and a `u8 *` prototype warns at two
   of them. A `u8 *sp = b;` local instead of the casts is 20 differing. */
void func_800C96E8(u8 *p, void *sp)
{
    RECT r;
    Unk80083160 *g;
    u8 *q;
    s32 lean;
    s32 bx;
    s32 dx;
    s32 dy;
    s32 t;

    g = &D_80083160;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    lean = D_80083160.unk_00 != D_801C9E40;
    if (*(u32 *)((u8 *)sp + 0xC0) < 0x1E0)
    {
        bx = *(s16 *)((u8 *)sp + 0xB8);
        dx = bx + *(s16 *)(p + 0x4);
        dy = *(s16 *)((u8 *)sp + 0xBA) + *(s16 *)(p + 0x6);
        if (dx >= 0 && dx + *(s16 *)(p + 0x0) < 0x140 && dy >= 0 &&
            dy + *(s16 *)(p + 0x2) < 0xE0 && bx >= 0 && bx + *(s16 *)(p + 0x0) < 0x140 &&
            *(s16 *)((u8 *)sp + 0xBA) >= 0 && *(s16 *)((u8 *)sp + 0xBA) + *(s16 *)(p + 0x2) < 0xE0)
        {
            r.x = *(u16 *)((u8 *)sp + 0xB8);
            t = *(s16 *)((u8 *)sp + 0xBA);
            /* DEVICE: a zero-instruction re-tie of `lean`. The target tests a
               COPY of the flag -- `addu $v0, $t2, $zero` in the previous
               branch's delay slot, then `beqz $v0` -- and every spelling that
               reaches the flag through C has the copy folded away by cse, so
               the branch reads $t2 and the function is 90 words / 2 differing.
               Worth exactly those 2 words and no instruction of its own: the
               word is reload's own copy breaking the tie. Measured and
               rejected at 2 differing: `u32`/`register`/`s16`/`s8` lean (s16
               and s8 are 91 words), `lean != 0`, `!(a == b)`, `lean | lean`,
               `~~lean`, `-(-lean)`, `lean + (lean - lean)`, a `k = lean` copy
               in this block and in the one above it, and `t += lean ? 0xE0 :
               0`. `lean & 1` is 1 differing -- the right word in the right
               place, but `andi` where the target has `addu`. */
            __asm__("" : "=r"(lean) : "0"(lean));
            if (lean)
            {
                t += 0xE0;
            }
            r.y = t;
            r.w = *(u16 *)(p + 0x0);
            r.h = *(u16 *)(p + 0x2);
            SetDrawMove(q, &r, bx + *(s16 *)(p + 0x4), r.y + *(s16 *)(p + 0x6));
            AddPrim(D_80083160.unk_00 + (*(s32 *)((u8 *)sp + 0xC0) * 4 + 0xB0), q);
            q += 0x18;
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* Spawn the kind-0x112 marker on tile (x, z) at height y: dress its mover and
   slot, face it off D_80083160's +0xC8 stamp through the D_800E0334 table,
   copy the 11-byte record at D_80089430 into the node's +0x54, hang
   func_800C9F34 in the view's +0x8C and announce it. Returns the view, or
   NULL when the allocation failed. 151 words, 0 differing.

   THE +0x54 RECORD IS AN 11-BYTE `memcpy`, NOT FOUR ASSIGNMENTS. Only
   mips.c's `output_block_move` emits the `lh`/`lb` the target has (its
   load_store table at :2827 and :2839); a plain `*(s16 *)d = *(s16 *)s`
   narrows to `lhu`, and gcc also splits the source address into a `lui` plus
   a `%lo` displacement per access instead of the target's single
   `addiu $a2, $v0, %lo(D_80089430)` -- 152 words, 85 differing. A 12-byte
   struct assignment is a three-word block move and is 148. `memcpy` with a
   constant length is expanded inline by cc1psx, as csrc/unk_264E0.c and
   csrc/unk_12DEC.c already rely on.
   ALL THREE PARAMETERS ARE `s16`. Declared `(s32, s32, s32)` the three
   prologue parameter copies come out in parameter order (a0, a1, a2) where
   the target emits a2's pair first: 151 words, 20 differing, every
   difference in the first six words.
   THE SECOND VIEW POINTER IS ASSIGNED IMMEDIATELY AFTER THE FIRST. `a = q;`
   written just before the D_8006CCF8 test -- where the target's
   `addu $s0, $s2, $zero` actually sits, in the `beqz`'s delay slot -- gives
   it $a3 instead of $s0 and is 7 differing; assigned at q's own
   definition it takes the register the dead D_80083160 pointer leaves. */
u8 *func_800C9850(s16 x, s16 z, s16 y)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    u8 *a;
    u8 *g;

    q = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        a = q;
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800C9AAC;
        q[0x13] = 0x2F;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = y;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = z;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(q + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                           *(u16 *)((u8 *)m + 0xA) - 0x20);
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        *(u32 *)(q + 0xA4) = (u32)D_800E0334;
        *(u32 *)((u8 *)r + 0x2C) = (u32)D_800E0334;
        g = (u8 *)&D_80083160;
        func_8003DB94(r,
                      (void *)(*(u32 **)(q + 0xA4))[((*(s16 *)(g + 0xC8) + *(s16 *)(q + 0x2A) +
                                                      0x100) >>
                                                     9) &
                                                    7],
                      0);
        func_80099FDC(n);
        memcpy((u8 *)n + 0x54, D_80089430, 0xB);
        q[0x71] = 0;
        *(u32 *)(q + 0x1C) |= 0x40000000;
        if (D_8006CCF8[((*(s16 *)(g + 0xC8) + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7] != 0)
        {
            r->unk_14 |= 1;
        }
        else
        {
            r->unk_14 &= 0xFFFE;
        }
        a[0x9A] = 0xFF;
        *(s8 *)(a + 0x9C) = -1;
        *(u32 *)(a + 0x8C) = (u32)func_800C9F34;
        *(s16 *)(a + 0xB6) = 0x10;
        *(u32 *)(q + 0x1C) |= 0x40000;
        *(s16 *)(a + 0x92) = -0x20;
        *(u32 *)(a + 0xAC) = 0x404040;
        func_800CB4C0(n, 0);
        func_8004491C(n, (u32)D_80045340);
        func_800A48F0((s8 *)q, 0x1B, 0);
    }
    return q;
}

/* The UnkNode +0x10 per-frame handler for an actor: run the +0x8C step, then
   the +0x9A state function out of D_800E0354, advance the mover by the
   +0x0C/+0x10 velocity, pick the eight-way facing cell off D_80083228 plus
   the record's +0x2A, re-dress through func_8003DB94 when it changed, and
   settle the +0x92 pitch toward func_800BCB04's range.

   FIVE SPELLINGS CARRY IT, each measured against the 290-word target:

   (1) `d = p;` -- THE RECORD IS REACHED THROUGH TWO POINTER LOCALS. +0x1C,
       +0x2A and +0x88 go through `d`, everything else through `p`; the
       target copies $a0 into both $s1 and $s2 in the prologue. With one
       pointer it is 287 words, 278 differing.
   (2) `i = k;` -- THE FACING CELL IS TWO LOCALS, an `s16` that is stored back
       into +0x94 and an `s32` that indexes the two tables. The target keeps
       them in $s4 and $s0 and copies between them. One local is 287 / 277.
   (3) `lim = -0x20;` -- THE PITCH FLOOR GOES IN A LOCAL. Written
       `*(s16 *)(p + 0x92) > -0x20`, mips.c's `gen_int_relational` takes the
       GT entry's `const_add` and folds the bound into one `slti $v0, $a0,
       -31`; the target materialises -0x20 into a register and uses the
       reverse-operand `slt $v0, $v0, $a0`, which is what a non-constant
       second operand gives. One word: 289 / 105. `>= -0x1F`, `-0x20 <
       p[0x92]`, `!(... <= -0x20)` and swapping the arms are all 289 as well.
   (4) `w` -- the +0x9E counter is read ONCE, into a `u16` local: the store of
       `w + 1` sits in the call's delay slot and the multiply sign-extends the
       same load. Read twice around the call it is 289 / 131.
   (5) The two +0x8C calls are written through the MEMORY EXPRESSION, not a
       shared `UnkDungeonStep h` local. One local for both gives the handler
       $v1 where the second call wants $v0 -- 290 words, 3 differing, and
       nothing else in the function moves.

   290 words, 0 differing. */
void func_800C9AAC(u8 *p, UnkVec *v, u8 *c)
{
    u8 *d;
    s32 i;
    s16 k;
    s16 e;
    s16 g;
    u16 w;
    s32 lim;

    d = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        if (*(UnkDungeonStep *)(p + 0x8C) == func_800C9F34)
        {
            (*(UnkDungeonStep *)(p + 0x8C))(p, v, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    if (func_800A9E70((UnkDungeonActor *)p, v, c, p) != 0)
    {
        return;
    }
    if (*(UnkDungeonStep *)(p + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(p + 0x8C))(p, v, c, p);
    }
    D_800E0354[p[0x9A]](p, v, c, p);
    if (*(s16 *)(p + 0xB8) == 0)
    {
        c[0xC] += 4;
        c[0xD] += 4;
        c[0xE] += 4;
        if (c[0xC] >= 0x40)
        {
            *(s16 *)(p + 0xB8) = 1;
        }
    }
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    *(s32 *)(p + 0x90) += v->unk_14;
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        i = k;
        if (*(s16 *)(p + 0x94) != i)
        {
            func_8003DB94(c, ((void **)*(u8 **)(p + 0xA4))[i], *(s8 *)(c + 0x4));
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
        if ((*(s32 *)(d + 0x1C) & 0x40000) != 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                w = *(u16 *)(p + 0x9E);
                *(s16 *)(p + 0x9E) = w + 1;
                *(s32 *)(p + 0xA0) += func_800644B8((s16)w * 170) << 5;
            }
            if ((*(u16 *)(p + 0x98) & 8) == 0)
            {
                func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                              (s16)(*(u16 *)(d + 0x88) - 0x20));
                lim = -0x20;
                if (*(s16 *)(p + 0x92) > lim)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 8;
                }
                else if (*(s16 *)(p + 0x92) < -0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                }
            }
        }
        else
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
            *(s16 *)(p + 0x9E) = 0;
            *(s32 *)(p + 0xA0) = 0;
            if ((*(u16 *)(p + 0x98) & 8) == 0)
            {
                e = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                  (s16)(*(u16 *)(d + 0x88) - 0x20)) -
                    *(u16 *)(d + 0x88);
                if (*(s16 *)(p + 0x92) < e)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 8;
                    if (e < *(s16 *)(p + 0x92))
                    {
                        *(s16 *)(p + 0x92) = e;
                    }
                }
                else
                {
                    *(s16 *)(p + 0x92) = e;
                }
            }
        }
    }
    else if ((*(u16 *)(c + 0x14) & 0x800) != 0)
    {
        *(u16 *)(c + 0x14) &= 0x8FFF;
    }
    else
    {
        *(u16 *)(c + 0x14) |= 0x7000;
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
    *(s16 *)((u8 *)v + 0xA) = (*(u16 *)(d + 0x88) + *(u16 *)(p + 0x92)) - *(u16 *)(p + 0xA2);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The +0x8C handler func_800CAAC8 installs: run the waypoint step while the
   engine's +0x1000 bit is up, give up to func_800CAA94 when +0x2000 is down
   and the slot is either unoccupied or the engine says 8, otherwise re-key
   the tile and close on func_800CA93C.

   0x80013714 IS A CONSTANT ADDRESS HERE, exactly as func_800CAAC8 records
   it for its own site: as the declared `D_80013714` the symbol's `lui` is a
   separate schedulable insn and reorg puts it in the `beqz d[0x28]` delay
   slot, where the target has `addu $a0, $s1, $zero` stolen from the
   func_800CAA94 block -- 8 differing. The constant form is one unsplittable
   two-word macro, so the slot has to come from the branch target.
   D_80083462 is read FOUR times and in THREE spellings, each off its own
   site: twice and then once more through the `(u8 *)&D_80083460` pointer
   local at +0x2, and once through the declared scalar in the +0x6D arm,
   which is the `lui`/`lhu` pair with its own %hi. 106 words, 0 differing. */
void func_800C9F34(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(u16 *)(g + 0x2) & 0x1000)
    {
        p[0x9A] = 0xE;
        func_800CA0DC(p, b, c, d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        p[0x9A] = 0xE;
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        if (d[0x28] == 0 || (*(u16 *)0x80013714 & 8) != 0)
        {
            func_800CAA94((UnkDungeonActor *)p, b, c);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            if (*(u8 **)(g + 0xC) == d && *(s16 *)(g + 0xA) == 0 && (*(u16 *)(g + 0x2) & 0x8) == 0)
            {
                *(s32 *)(d + 0x18) = 0;
                *(s32 *)(g + 0xC) = 0;
            }
            return;
        }
    }
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((D_80083462 & 0x2000) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800CA93C(p, b, c);
    }
}

/* Step the record's waypoint cursor one entry on, move the tile with it, and
   re-aim. TWO WRITTEN ORDERS, 4 differing between them. The guard reads the
   LIMIT first (`d[0x71] > *(s16 *)(d + 0x8A)`) -- the other way round the
   `lh` and the `lbu` swap places. And the row address is BASE-FIRST:
   `*(u8 *)(d + idx + 0x74)` gives `addu $v0, $s0, $v1`, where the subscript
   `d[idx + 0x74]` gives the index-first `addu $v0, $v1, $s0`. The two
   coordinate reads before the stores are the OLD values the call still
   wants; gcc keeps them above the stores by itself. 65 words, 0 differing. */
void func_800CA0DC(u8 *p, void *b, u8 *c, u8 *d)
{
    s32 n;
    u8 x;
    u8 y;

    if (*(s8 *)(d + 0x71) > 0)
    {
        if (d[0x71] > *(s16 *)(d + 0x8A))
        {
            x = c[0x24];
            c[0x24] = *(u8 *)(d + *(s16 *)(d + 0x8A) + 0x74);
            y = c[0x25];
            c[0x25] = *(u8 *)(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (s32 *)(p + 0x98));
            *(s32 *)(d + 0x1C) |= 0x40000000;
            p[0x9A] = 0xF;
            *(s32 *)(p + 0x8C) = 0;
            if (D_80083462 & 0x80)
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
}

/* 153 words, 0 differing.  func_8009A66C's twin with a map-bounds pre-check in
   front and a 1/-1 tail; returns -1 blocked, 0 cannot, 1 ok.

   PARKED FOR A DAY AT ONE OPCODE and closed by ONE SPELLING: the argument
   copy is `ka = kg + z0` with `register s32 z0 __asm__("$0")`, i.e. an
   addsi3 whose second operand is literally $zero.

   WHY THAT IS THE WHOLE LANDING.  The target's word 22 is a REDUNDANT
   `addu $a0, $s5, $zero` -- $a0 already holds the index from word 4 and
   nothing writes $a0 before the call.  Every spelling that makes word 22 a
   reg-reg move loses it, because jump2 is `jump_optimize (insns, 1, 1, 0)`
   (toplev.c:3690, noop_moves == 1) and its block at jump.c:476-525 takes
   every `(set (reg hard) (reg hard))` whose two `true_regnum`s are both >= 0
   and calls `find_equiv_reg` (reload.c:5554), which scans BACKWARD over the
   linear insn stream -- stopping only at a CODE_LABEL -- for an insn copying
   into or out of $a0 whose other end is $s5.  The word-6 copy
   `addu $s5, $a0, $zero` is exactly that insn, same block, both SImode, so
   the move at 22 is deleted: 152 words.  Spelling word 22 as a PLUS escapes
   the test one step earlier -- `true_regnum (SET_SRC (pat))` is -1 for a
   PLUS, so jump2 never considers the insn at all -- and `addu $a0,$s5,$zero`
   is the same 0x02A02021 the target has.  Nothing else changed from the
   153-word / 1-row park; it went to 0 on the first compile.

   Routes measured and rejected for that one word (all on this body): the
   `& 7` on a fenced, pinned `kg` (`kz2 = (u16)kg; ka = kz2 & 7;`) gives
   `andi $a0,$s5,0x7` -- 153 words, right slot, right registers, wrong
   opcode, and an andi survives only because it is not a reg-reg SET;
   reading the UNFENCED `kk` instead folds the mask into a real move and
   jump2 deletes it, 152/1; plain `ka = kk;` or the argument inline 152/8;
   a second masked RECOMPUTATION from the parameter (`kz2 = (u16)(f >> 9);
   ka = kz2 & 7;`) does produce the target's move but keeps the srl result
   live and pushes the temp off $a0, 153/5.  Putting the PLUS at word 6
   instead (`kk = k + z0`) destroys combine's significant_bits proof for the
   later mask and costs a copy: 154/15.

   THE THREE FRONT DEVICES, all load-bearing, all from the park record:
     (a) `kz = (u16)k; kk = kz & 7;` in TWO STATEMENTS is the word-6 copy --
         combine folds `(and (zero_extend (subreg:HI k)) 7)` to `(reg k)` via
         significant_bits(k) == 7, so the insn is a bare move.  As one
         statement, `kk = (u16)k & 7;`, it is 152/10.
     (b) `__asm__("" ::"r"(k));` is k's SECOND USE.  With one use combine
         substitutes k's own `andi` into the widening and (a) collapses back
         to a single `andi` writing $s5.  No outputs, so it is implicitly
         volatile and survives flow; it emits nothing.
     (c) `__asm__("" : "=r"(kg) : "0"(kk));` with **kg pinned to $21** keeps
         the register file right, and the PIN is the load-bearing half:
         without it the fence's extra allocno pushes the index to $s0 and `c`
         to $s3, 153/36.  The fence must NOT be volatile.

   And three structural facts that must not be disturbed: the bounds check
   and the step target are the SAME two locals (`ax`/`az`, not fresh
   `tx`/`tz` -- that is 81 rows, reusing them 26, and it is what puts the two
   sums in $s1/$s0 and spills `s16 y` to `sh $a3, 0x18($sp)`); the tail is
   `if (t >= 0x201) return -1; return 1;` in that order (with `return -1`
   last, cross-jumping merges all five `return -1` bodies and the target's
   shared .L800CA2B4 block disappears, 150/26); and `m` is a LOCAL
   `u8 *m = (u8 *)D_8008333C;` -- folding the displacement into the
   relocation costs the shared base, 152/24. */
s32 func_800CA1E0(u32 f, u8 *c, u8 *d, s16 y)
{
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 kk;
    register s32 kg __asm__("$21");
    s32 ka;
    s32 kz;
    s32 kz2;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;
    register s32 z0 __asm__("$0");

    k = (f >> 9) & 7;
    __asm__("" ::"r"(k));
    m = (u8 *)D_8008333C;
    kz = (u16)k;
    kk = kz & 7;
    __asm__("" : "=r"(kg) : "0"(kk));
    ka = kg + z0;
    ax = c[0x24] + (u16)D_8006CCD8[kg];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[kg];
    if (az == 0 || (1 << *(s16 *)(m + 0x16)) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if (func_8009A540(ka, px >> 6, pz >> 6, h - y) == 0)
    {
        return 0;
    }
    ax = D_800DCEAC[kg] + wx;
    az = D_800DCEBC[kg] + wz;
    func_8009A350(c[0x24], c[0x25], kg, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[kg], c[0x25] + (u16)D_8006CCE8[kg]);
        if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 && (*(s32 *)(d + 0x1C) & 0x2000) == 0)
        {
            return 0;
        }
    }
    t = func_800BCB04(ax, az, h - y);
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}

void func_800CA444(u8 *p, void *a1, u8 *b, u8 *c)
{
    s16 i;
    s16 y;
    s16 *t;
    u8 *g;
    u8 *g2;
    u8 *q;
    s32 k;
    s32 v;
    s32 e;
    u16 f;

    f = D_80083462;
    if ((f & 0x4000) != 0)
    {
        func_800A9A0C(c);
        return;
    }
    if ((f & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)b, c, 3, 6, (u8 *)p + 0x9C);
    if (*(s8 *)(b + 0x26) >= 0 && (D_800E2970_s[*(s8 *)(b + 0x26)].unk_0C & 2) != 0)
    {
        func_800A0E6C(b, *(s8 *)(p + 0x9C), c, (u8 *)p + 0x98);
    }
    else if ((*(u16 *)(c + 0x46) & 0x8000) == 0)
    {
        func_800A0E6C(b, *(s8 *)(p + 0x9C), c, (u8 *)p + 0x98);
    }
    for (i = 0; i < 8; i++)
    {
        t = D_8006CD00;
        g = D_80082E80;
        v = *(s16 *)(c + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            e = t[i];
            y = v - e;
        }
        else
        {
            e = t[i];
            y = v + e;
        }
        if ((s16)func_800CA1E0(y, b, c, 0x20) > 0)
        {
            *(s16 *)(c + 0x2A) = y;
            *(u8 *)(c + (c[0x71] & 0x7F) + 0x74) = b[0x24];
            *(u8 *)(c + (c[0x71] & 0x7F) + 0x7C) = b[0x25];
            k = (*(u16 *)(c + 0x2A) >> 9) & 7;
            c[0x71] = c[0x71] + 1;
            b[0x24] = b[0x24] + (u16)D_8006CCD8[k];
            b[0x25] = b[0x25] + (u16)D_8006CCE8[k];
            break;
        }
        if (i == 0 && *(u16 *)(g + 0x24) != *(u16 *)(b + 0x24))
        {
            if ((s16)func_8009A180(c, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
        }
    }
    if (i >= 8)
    {
        q = c;
        q[0x71] &= 0x7F;
        *(u16 *)(q + 0x46) &= 0x7FFF;
        func_800A9A0C(q);
        return;
    }
    *(u16 *)(c + 0x46) &= 0x7FFF;
    p[0x9C] = b[0x26];
    c[0x6D] = c[0x6D] - 1;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(c + 0x6D) == 0)
    {
        c[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((b[0x24] << 6) | 0x20, (b[0x25] << 6) | 0x20, (s16)(*(u16 *)(c + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(c + 0x88) = i;
    }
}

/* func_800CA93C's "same slot" branch: with the renderable's two +0x2000 draw
   bits raised, ask func_8009B4B0 what sits under the cursor, bail on any of
   the D_80083460 gates, and if the two +0x88 depths are within 0x40 of each
   other put the actor into state 0x11 and re-aim through func_8009C93C.

   EVERY FAILURE EXIT IS ONE `goto fail`, AND THE LABEL SITS IN THE SECOND
   TEST. With five plain `return -1` statements cross-jumping keeps the merged
   `addiu $v0, $zero, -0x1` as its own block just above the epilogue and the
   function is 110 words; the shared label puts that block inline as
   `j .L800CA914` / `addiu $v0, $zero, -0x1` at the second test, where reorg
   then copies the constant into the other three branches' delay slots.
   Spelling the second and third gates as one `||` is 107 words.

   AND THE DEPTH WINDOW IS WRITTEN CONSTANT-FIRST WITH AN EXPLICIT NEGATION.
   `(u16)(0x40 - func_800A0134(t, d))` is `addiu $v1, $zero, 0x40` plus a
   `subu`; `0x40 + -func_800A0134(...)` is the target's `negu`/`addiu` pair --
   fold's additive-constant swap is an involution, so the constant goes on the
   operand the listing does NOT carry it on. Negating into its own local first
   is 111 words. 109 words, 0 differing. */
s32 func_800CA788(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    *(u32 *)(d + 0x14) |= 0x2000;
    *(u32 *)(d + 0x1C) |= 0x2000;
    t = func_8009B4B0(d, c[0x24], c[0x25]);
    *(u32 *)(d + 0x14) &= ~0x2000;
    *(u32 *)(d + 0x1C) &= ~0x2000;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
    fail:
        return -1;
    }
    if ((*(u16 *)(g + 0x2) & 0x8) != 0)
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
        goto fail;
    }
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x11;
    d[0x84] = 0x80;
    d[0x85] = 0;
    *(u32 *)(d + 0x14) |= 0x2000;
    *(u32 *)(d + 0x1C) |= 0x2000;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 0, 0);
    *(u32 *)(d + 0x14) &= ~0x2000;
    *(u32 *)(d + 0x1C) &= ~0x2000;
    d[0x6D] -= 1;
    return 1;
}

/* Pick the slot under the cursor with the +0x2000 draw bits raised around
   each engine call, then either re-enter func_800CA788 on the same slot or
   re-aim through func_800A0818 before closing on func_800CA444.

   THE FIRST PARAMETER IS HELD IN TWO LOCALS, and that is three words:
   `q = p;` with `q` used for every field access and for both calls' trailing
   argument, and the parameter itself used only as func_800CA444's first.
   With one variable the function is 83 words / 86 differing -- it saves five
   callee-saved registers instead of six and the frame is 0x30 rather than
   0x38. The two pseudos are simultaneously live to the last call, so
   local-alloc cannot coalesce them. 86 words, 0 differing. */
s32 func_800CA93C(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *e;
    u8 *t;

    q = p;
    *(u32 *)(q + 0x14) |= 0x2000;
    *(u32 *)(q + 0x1C) |= 0x2000;
    *(u32 *)(q + 0x60) = (u32)func_800A3D18(c, q, 4);
    *(u32 *)(q + 0x14) &= ~0x2000;
    *(u32 *)(q + 0x1C) &= ~0x2000;
    if (*(u32 *)(q + 0x60) != 0)
    {
        e = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
        *(u32 *)(q + 0x14) |= 0x2000;
        *(u32 *)(q + 0x1C) |= 0x2000;
        t = func_8009B4B0(q, c[0x24], c[0x25]);
        *(u32 *)(q + 0x14) &= ~0x2000;
        *(u32 *)(q + 0x1C) &= ~0x2000;
        if (t == *(u8 **)(q + 0x60))
        {
            func_800CA788(q, b, c, q);
            return 0;
        }
        *(s16 *)(q + 0x2A) = func_800A0818(c[0x24], c[0x25], e[0x24], e[0x25], (u16 *)(q + 0x98));
        *(u16 *)(q + 0x46) |= 0x8000;
    }
    func_800CA444(p, b, c, q);
    return 0;
}

/* 13 words, 0 differing. */
void func_800CAA94(UnkDungeonActor *p, void *b, void *c)
{
    p->unk_9A = 0x8;
    p->unk_96 = 0x40;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(s32 *)((u8 *)p + 0x1C) = (*(s32 *)((u8 *)p + 0x1C) | 0x4000000) & ~0x1638;
}

/* A +0x8C handler. TWO THINGS COST SOMETHING. 0x80013714 is a CONSTANT
   ADDRESS here: the symbol's `lui` is a separate `high` insn that reorg puts
   in the `blez`'s delay slot, where the target has a `nop` and the two-word
   macro after it -- 21 differing. And the RETURNING arm has to be written
   FIRST: with the install arm first the tests come out `beq` and the call
   block goes out of line, 27 differing. 56 words, 0 differing. */
void func_800CAAC8(UnkDungeonActor *p, void *b, void *c, u8 *d)
{
    if (func_800AB1C0((u8 *)p, b, c, d) != 0)
    {
        func_800A4ACC(d);
        *(s16 *)((u8 *)p + 0xB6) -= 1;
        if ((*(s16 *)((u8 *)p + 0xB6) <= 0 || (*(u16 *)0x80013714 & 8) != 0) &&
            d[0x71] == *(s16 *)(d + 0x8A))
        {
            func_800CAA94(p, b, c);
            return;
        }
        p->unk_8C = func_800C9F34;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)((u8 *)p + 0x92) = -0x20;
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_8008942C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800C2824", D_80089430);

/* The five-state spin-up/spin-down for the warp tile: state 0 arms +0xB8 and
   either bails to state 4 or latches the mover's bearing at +0xB0, state 1
   raises the node's +0x98 draw bit and winds +0xA2 up to 0x80, state 2 bleeds
   +0xA0 and drags the +0xAC colour triple towards 0xE0/8/8 while spawning
   func_800CAFDC sparks, state 3 drags it back to 0x40/0x40/0x40 and re-aims
   the mover by 0x200 a frame, state 4 re-keys the slot and either closes on
   func_800CAA94 or re-installs func_800C9F34 in +0x8C. The jump table is
   jtbl_80089440 at rodata +0xCE0, which is 0 mod 8, so its `.align 3` costs
   nothing. State 0 falls through into state 1 and state 3 into state 4.

   THREE THINGS COST SOMETHING.
   (1) In state 0's fall-through the +0xB0 LATCH IS WRITTEN BEFORE THE
   `p[0x9B]++`. The other order is 203 words / 161 differing: the scheduler
   will not hoist the `lhu` off $s2 above the `sb` to $s1, so both of that
   block's load delays stay `nop`.
   (2) THE "ALREADY ON BEARING" FLAG IN STATE 3 IS NARROWER THAN `int`.
   Declared `s32` the function is 201 words / 56 differing -- the target's
   `addu $v0, $t1, $zero` ahead of the `beqz` is the integer promotion of a
   narrow local, and a plain `s32` never emits it. `s16` and `s8` both land;
   `u16` and `u8` are 202 words / 9 differing (the promotion comes out as an
   `andi`).
   (3) 0x80013714 IS A CONSTANT ADDRESS, exactly as func_800CAAC8 and
   func_800C9F34 record for their own sites: as the declared `D_80013714` the
   `lui` is a separate schedulable insn and reorg puts it in the `blez`'s
   delay slot, where the target has the `addu $a0, $s1, $zero` stolen from the
   func_800CAA94 block -- 202 words, 8 differing.
   202 words, 0 differing. */
