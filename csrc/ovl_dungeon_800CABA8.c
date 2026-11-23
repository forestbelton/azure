#include "common.h"
#include "ovl_dungeon_decls.h"

extern u8 D_800E1C3A[];

/* dungeon.bin's overlay, translation unit 3 of 4: func_800CABA8 to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

u8 *func_8009929C(s32 c, u8 *p);
void func_800999D4(u8 *p);
u8 *func_8009B390(u16 x, u16 y, s16 ang);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
void func_8009CE1C(u8 *p, s16 b, s16 c, s16 e, s16 ang, u32 q, s16 g2);
s32 func_800A2C78(u8 *p);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
s32 func_800A6780(void);
s16 func_800A70E4(s16 x, s16 y, s16 ang);
void func_800AA5E4(u8 *p, void *b, u8 *r, u8 *d);
s16 func_800B500C(s16 x, s16 y, s16 c);
s32 func_800BC054(u8 *p, UnkVec *v, UnkSlot *s);
s16 func_800BCA68(u16 x, u16 y);
UnkNode *func_800C1A44(s16 x, s16 y, s16 ang);
void func_800C5D80(u8 *p, UnkVec *b, u8 *c);
void func_800C5E5C(s32 x, s32 y, s32 z, void *d, s16 e);
s32 func_800C80F0(u8 *p);
void func_800CAA94(UnkDungeonActor *p, void *b, void *c);

void func_800CABA8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    s16 f;

    switch (p[0x9B])
    {
    case 0:
        *(s16 *)(p + 0xB8) = 1;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 0);
            p[0x9B] = 4;
            return;
        }
        *(s16 *)(p + 0xB0) = *(u16 *)(d + 0x2A);
        p[0x9B]++;
    case 1:
        *(u16 *)(p + 0x98) |= 8;
        *(s16 *)(p + 0xA2) = *(u16 *)(p + 0xA2) + 0x10;
        if (*(s16 *)(p + 0xA2) > 0x80)
        {
            *(s16 *)(p + 0xA2) = 0x80;
            *(s16 *)(p + 0x96) = 8;
            p[0x9B]++;
        }
        break;
    case 2:
        n = (UnkNode *)(p - 0x20);
        *(s32 *)(p + 0xA0) -= *(s32 *)(p + 0xA0) / *(s16 *)(p + 0x96);
        func_800CAFDC(n, *(s16 *)(d + 0x2A), *(s32 *)(p + 0xAC));
        func_800CB4C0(n, 1);
        p[0xAC] += (0xE0 - p[0xAC]) / *(s16 *)(p + 0x96);
        p[0xAD] += (8 - p[0xAD]) / *(s16 *)(p + 0x96);
        p[0xAE] += (8 - p[0xAE]) / *(s16 *)(p + 0x96);
        *(s16 *)(p + 0x96) -= 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 0);
            *(s16 *)(p + 0x96) = 4;
            p[0x9B]++;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0xB0) != *(s16 *)(d + 0x2A))
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            f = 0;
        }
        else
        {
            f = 1;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            p[0xAC] += (0x40 - p[0xAC]) / *(s16 *)(p + 0x96);
            p[0xAD] += (0x40 - p[0xAD]) / *(s16 *)(p + 0x96);
            p[0xAE] += (0x40 - p[0xAE]) / *(s16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(p + 0x96) = 0;
        }
        *(s16 *)(p + 0x96) -= 1;
        if (f == 0)
        {
            return;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
    case 4:
        *(s16 *)(p + 0xA2) = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A4ACC(d);
        *(s16 *)(p + 0xB6) -= 1;
        if (*(s16 *)(p + 0xB6) <= 0 || (*(u16 *)0x80013714 & 8) != 0)
        {
            func_800CAA94((UnkDungeonActor *)p, b, c);
        }
        else
        {
            *(u32 *)(p + 0x8C) = (u32)func_800C9F34;
        }
        break;
    }
}

/* Turn the mover towards func_800BCB04's bearing, or -- if it is already
   past it -- glide one frame and decay the three deltas by a quarter, with
   a 0x8000 climb on the third; the +0x96 countdown falls by 8 a frame and
   raises the node's redraw bit at zero. func_800D5CA0's glide with the
   deltas bled rather than halved. 67 words, 0 differing, first compile. */
void func_800CAED0(u8 *p, UnkVec *v)
{
    s16 r;

    r = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                      *(u16 *)((u8 *)v + 0xA) + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += v->unk_14;
        v->unk_0C -= v->unk_0C >> 2;
        v->unk_10 -= v->unk_10 >> 2;
        v->unk_14 += 0x8000;
    }
    else
    {
        *(s16 *)((u8 *)v + 0xA) = r;
    }
    if ((*(s16 *)(p + 0x96) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn TWO 0x212 sparks off the parent node, each jittered +/-0x20 from
   the parent's mover on all three axes, given a velocity from one angle
   through func_80064584/func_800644B8 and a lifetime of 0x80..0xFF frames.
   The loop is `for (i = 2; i > 0; i--)`, which is the `addiu $s3, -1` /
   `bgtz` pair at the bottom with no entry test. 101 words, 0 differing on
   the first compile. */
void func_800CAFDC(UnkNode *a, s16 b, s32 col)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s32 i;
    s16 t;

    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800CAED0;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) =
                *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)m + 0x6) =
                *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x3F) - 0x20;
            *(s16 *)((u8 *)m + 0xA) =
                *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x3F) - 0x20;
            t = rand();
            *(s32 *)((u8 *)m + 0xC) = func_80064584(t) << 5;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(t) << 5;
            *(s16 *)((u8 *)m + 0x16) = -(rand() & 7) - 8;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u8 **)(r + 0x8) = D_800E03B0;
            *(s32 *)(r + 0xC) = col;
            *(s16 *)(r + 0x6) = 8;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0xB0) = b;
            *(s16 *)(q + 0x96) = (rand() & 0xFF) | 0x80;
        }
    }
}

/* The same three-case spin-and-unwind as func_800A7548 (:9248), over a
   different record: case 0 gates on func_800A2C78, claims a slot in
   D_80083460's +0xA counter and arms the 4-frame countdown at +0xB2, case 1
   swings the renderable's +0x1C/+0x1E by rsin/rcos of a rising angle, and
   case 2 unwinds it, bleeds the mover's +0x14, fades the three colour bytes
   and -- when +0x1C has come back to zero -- releases the slot and tears the
   view down. case 0 FALLS THROUGH into case 1, and `*(s16 *)(p + 0x96) = 1`
   in case 2 reuses the constant 1 the switch head materialised for its
   `beq`, both exactly as func_800A7548 records.

   THE TWO D_80083460 BASES MUST BE TWO LOCALS. With one `u8 *g` reused in
   both arms the pointer still gets $v1 in both, but the symbol's `high`
   takes $v0 and the `addiu` reads across (`lui $v0` / `addiu $v1, $v0`)
   where the target ties both to $v1: 129 words, 4 differing, all four those
   two words. Two locals -- or one block-scoped per arm, which measures the
   same md5 -- is 0. Dropping the local and writing
   `*(u16 *)((u8 *)&D_80083460 + 0xA)` inline is 127 words / 101 differing,
   and a `u16 *` local the same.
   129 words, 0 differing. */
void func_800CB170(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;

    switch (p[0x9B])
    {
    case 0:
        if ((s16)func_800A2C78(d) != 0)
        {
            return;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        *(s16 *)(p + 0xB2) = 4;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B]++;
    case 1:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + func_800644B8(*(s16 *)(p + 0x96) * 32);
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - func_800644B8(*(s16 *)(p + 0x96) * 32);
        *(s16 *)(p + 0xB2) = *(u16 *)(p + 0xB2) - 1;
        if (*(s16 *)(p + 0xB2) <= 0)
        {
            p[0x9B]++;
        }
        break;
    case 2:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            *(s16 *)(p + 0x96) = 1;
        }
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - func_800644B8(*(s16 *)(p + 0x96) * 32) * 2;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + func_800644B8(*(s16 *)(p + 0x96) * 32) * 4;
        b->unk_14 -= 0x20000;
        c[0xC] = c[0xD] = c[0xE] -= 4;
        if (*(s16 *)(c + 0x1C) <= 0)
        {
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            func_800A32A4(d);
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* A node's +0x10 step for the timed marker: func_800478B8 first, then either
   fade the renderable's RGB by 1/N per frame while the view's +0xB4 flag is
   set, or copy the colour and the two scales out of the record at +0xA8; both
   arms fall into the shared kill tail that raises 0x8000 in the node's
   UnkNode.unk_1E (reached as `p - 0x2` from the actor view) and in
   D_800814A0. `*(u8 **)(p + 0xA8)` is RE-DEREFERENCED three times rather than
   cached, which is what the three `lw 0xA8($s1)` are.
   83 words, 0 differing on the first compile. */
void func_800CB374(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;

    func_800478B8(c);
    if (*(s16 *)(p + 0xB4) != 0)
    {
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x96);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x96);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x96);
        *(s16 *)(c + 0x6) -= 1;
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
    }
    else
    {
        *(s32 *)(c + 0xC) = *(s32 *)(*(u8 **)(p + 0xA8) + 0xCC);
        q = *(u8 **)(*(u8 **)(p + 0xA8) + 0xC);
        *(s16 *)(c + 0x1C) = *(u16 *)(q + 0x1C);
        *(s16 *)(c + 0x1E) = *(u16 *)(q + 0x1E);
        if ((*(u16 *)(*(u8 **)(p + 0xA8) + 0x1E) & 0x8000) == 0)
        {
            return;
        }
    }
    *(u16 *)(p - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* A 0x100 spawner whose mover is either SHARED with the parent node or
   copied from it a halfword at a time. THE COPY ARM RE-READS `a->unk_08` FOR
   EVERY FIELD and caches the destination once: a halfword store through a
   pointer invalidates every varying-address entry in cse's table. The last
   pair's order is load-bearing -- with `r->unk_10 = 0x60` written before the
   +0xC copy the two loads get no delay-slot filler and the function is 81
   against 80, 25 differing. 80 words, 0 differing. */
UnkNode *func_800CB4C0(UnkNode *a, s16 b)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FD64(0x100, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CB374;
        func_8004491C(n, (u32)D_80045340);
        r = (u8 *)n->unk_0C;
        func_8003DB94(r, D_800E02CC, 0);
        *(s16 *)(r + 0x1E) = 0x1400;
        *(s16 *)(r + 0x1C) = 0x1400;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x10) = 0x20;
        if (b == 0)
        {
            n->unk_08 = a->unk_08;
        }
        else
        {
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
            *(u32 *)(r + 0xC) = *(u32 *)((u8 *)a->unk_0C + 0xC);
            *(s16 *)(r + 0x10) = 0x60;
        }
        *(s16 *)(r + 0x6) = -1;
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0xA8) = a;
        *(s16 *)(p + 0xB4) = b;
        *(s16 *)(p + 0x96) = 8;
    }
    return n;
}

/* The 0x102 flash node's +0x10 step: ramp +0x94 to 0x40, strobe the
   renderable's RGB triple on the +0x9D parity, then one of three states --
   drop the mover's +0x8A to -0x80 and open the scale towards 0x1000 until
   the engine's D_80083460 +0xC slot is free, publish into it; wait for
   D_8008346C to clear and flag the node 0x80000000; or close the scale back
   down and retire. 139 words, 0 differing.

   THREE SPELLINGS, each measured:
   (1) THE STROBE VALUE IS ONE LOCAL. Written out three times, once per
   colour byte, it is 155 words / 131 differing -- gcc recomputes the whole
   `lbu`/`lh`/`sll`/`andi`/`mult` chain per store.
   (2) `*(s16 *)(p + 0x94) << 1` AND NOT `* 2`: through the multiply fold
   reassociates `(a * 2) * b` into `a * (b * 2)` and the two loads swap, 2
   differing.
   (3) THE TWO `&D_80083460` POINTERS ARE TWO LOCALS. One `g` reused across
   the state-0 and the default arm is one quantity and gets a lui/addiu pair
   in two registers in both arms; the target ties the pair into one register
   in the arm with a single access, 2 differing. */
void func_800CB600(u8 *p, UnkVec *m, u8 *r)
{
    u8 *g;
    u8 *g2;
    s32 t;
    u8 *q;

    *(s16 *)(p + 0x94) += 1;
    if (*(s16 *)(p + 0x94) > 0x40)
    {
        *(s16 *)(p + 0x94) = 0x40;
    }
    t = p[0x94] + (*(s16 *)(p + 0x94) << 1) * (p[0x9D] & 1);
    r[0xC] = t;
    r[0xE] = t;
    r[0xD] = t;
    p[0x9D] += 1;
    if (p[0x9A] == 0)
    {
        *(s16 *)(p + 0x8A) -= 8;
        if (*(s16 *)(p + 0x8A) < -0x80)
        {
            *(s16 *)(p + 0x8A) = -0x80;
        }
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x92) + *(u16 *)(p + 0x8A);
        *(s16 *)(r + 0x1E) = *(s16 *)(r + 0x1C) =
            *(u16 *)(r + 0x1C) + (0x1000 - *(u16 *)(r + 0x1C)) / *(s16 *)(p + 0x96);
        *(s16 *)(p + 0x96) -= 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            g = (u8 *)&D_80083460;
            *(s16 *)(p + 0x96) = 0;
            if (*(u32 *)(g + 0xC) == 0 && *(u32 *)(g + 0x10) == 0 && !(*(u16 *)(g + 0x2) & 8))
            {
                *(u8 **)(g + 0xC) = p;
                *(u16 *)(p + 0x98) |= 0x80;
                p[0x9A] += 1;
            }
        }
    }
    else if (p[0x9A] == 1)
    {
        if (D_8008346C == 0)
        {
            func_8009A028(p);
            q = p - 0x20;
            *(u32 *)(q + 0x10) |= 0x80000000;
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9A] += 1;
        }
    }
    else
    {
        *(s16 *)(r + 0x1E) = *(s16 *)(r + 0x1C) =
            *(u16 *)(r + 0x1C) - *(u16 *)(r + 0x1C) / *(s16 *)(p + 0x96);
        *(s16 *)(p + 0x96) -= 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Spawn the 0x102 node func_800CB600 steps: copy the caller's mover triple,
   point the renderable at the D_800E03BC template with its +0x14 flags, take
   the tile pair out of the mover's two integer halves and the D_800E3CC8 pair
   into +0x08/+0x09, then wire the view off the source actor and hand the
   result to func_800A94A0. A zero from that call is the failure exit: it
   raises 0x8000 on the node and on D_800814A0 and returns NULL, which is the
   `addu $v0, $zero, $zero` in the `bnez`'s delay slot.

   THE +0x88 COPY GOES BEFORE THE +0x13 BYTE. Written after it, the `li 0xFF`
   floats all the way into the `lbu 0x25` load-delay slot, the +0x25 load takes
   $v1 instead of $v0 and the two stores swap: 108 words, 7 differing. Caching
   the two +0x24/+0x25 bytes in one reused local is 10, and hoisting the +0x13
   store above the pair is 11. 108 words, 0 differing. */
UnkNode *func_800CB82C(UnkVec *src, u8 *v, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *e;
    u8 *g;

    n = func_8003FC64(0x102);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CB600;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src + 0xA);
        r = (UnkSlot *)n->unk_0C;
        r->unk_08 = (u32)D_800E03BC;
        r->unk_14 |= 0xC;
        *((u8 *)r + 0x24) = *(u16 *)((u8 *)m + 0x2) >> 6;
        *((u8 *)r + 0x25) = *(u16 *)((u8 *)m + 0x6) >> 6;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x96) = 0x10;
        p[0x8] = D_800E3CC8[0];
        p[0x9] = D_800E3CC8[1];
        *(u8 **)(p + 0x60) = d;
        e = *(u8 **)(d - 0x14);
        p[0x72] = e[0x24];
        p[0x73] = e[0x25];
        *(s16 *)(p + 0x88) = *(u16 *)(v + 0x88);
        p[0x13] = 0xFF;
        *(s32 *)(p + 0x14) = *(s32 *)(v + 0x14);
        *(s32 *)(p + 0x1C) = *(s32 *)(v + 0x1C);
        *(s16 *)(p + 0x2A) = *(u16 *)(d + 0x2A) - 0x800;
        *(s32 *)(p + 0x90) = src->unk_08;
        if (func_800A94A0(p, p + 8, 0, p + 0x98) == 0)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return NULL;
        }
        func_80099FDC(n);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

/* The +0x10 handler of the 0x202 tint node: it bleeds the mover's +0x14 into
   the dying record's +0x08 every frame, and once its own +0x06 counter passes
   0x40 (or the renderable's +0x14 raises 0x8000) it either runs the player's
   death chain or the ordinary one, then kills itself.
   111 words, 0 differing.

   0x80010000 IS ONE POINTER LOCAL, not two symbols: the target reads
   0x80013714 and 0x80010234 off a single `lui $s0, 0x8001`, which two
   declared `D_` symbols cannot share.

   DEVICE: the `__asm__("" ::"r"(c))` is a zero-instruction reference that
   gives the third parameter one more `reg_n_refs` than the first. Without it
   `p` outranks `c` in global-alloc and the two land in each other's
   registers -- $s2/$s3 swapped, 111 words / 11 differing, with every opcode,
   immediate and displacement already right. Ruled out first, all 11
   differing: re-ordering the four head statements (15 and 116 words/103),
   `*(u16 *)(p + 0x6) += 1`, an `s16` temp for the counter, the kill written
   as an explicit read-modify-write, the whole body under an early return,
   a `u8 *m = c;` copy (copy-propagated away), and pointer locals for `p + 6`
   and `p - 2` (cse puts the references straight back). */
void func_800CB9DC(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *r;
    u8 *s;
    u8 *g;
    u8 *k;
    u8 *n;

    /* DEVICE: see the comment above. */
    __asm__("" ::"r"(c));
    func_800478B8(c);
    q = *(u8 **)p;
    r = *(u8 **)(q - 0x14);
    s = *(u8 **)(q - 0x18);
    b->unk_14 -= 0x60000;
    *(s32 *)(s + 0x8) += b->unk_14;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    if (*(s16 *)(p + 0x6) > 0x40 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        if (q == D_800E3D7C)
        {
            k = (u8 *)0x80010000;
            if ((*(u16 *)(k + 0x3714) & 4) != 0)
            {
                func_80040AA0(3);
            }
            else
            {
                func_800945E8(q);
                func_800948BC();
                func_800A6780();
                n = (u8 *)D_80081468;
                *(s32 *)(k + 0x234) = *(s32 *)(k + 0x234) + 1;
                *(u16 *)(n + 0x4) = *(u16 *)(n + 0x4) + 1;
                func_80040AA0(D_80082E6B);
                func_800481E0();
            }
        }
        else
        {
            func_8009A3D0(r[0x24], r[0x25], (*(u32 *)(q + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_800A32A4(q);
            func_8009A028(q);
            *(u16 *)(q - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            g = (u8 *)&D_80083460;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the kind-2 node the overlay's aimed-shot effect rides on.

   (1) The white has to be a LOCAL assigned before the mover is read: written
   at its store the `lui`/`ori` pair is emitted after the first tile centre's
   `andi`/`sll`/`addiu` instead of before it, 5 differing; one statement
   earlier is 68 words / 66, so the position is exact.
   (2) `v = x * 64; v = v + 0x20;` as two sets of ONE variable is what keeps
   the `addiu`: in one expression combine proves the shifted value's low bits
   clear and rewrites the add as `ori`. 66 words, 0 differing. */
UnkNode *func_800CBB98(u16 x, u16 y, s16 z, u8 *owner)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *g;
    s32 v;
    s32 col;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CB9DC;
        func_8004491C(n, (u32)D_80045340);
        col = 0x808080;
        m = n->unk_08;
        v = x * 64;
        v = v + 0x20;
        *(s16 *)((u8 *)m + 0x2) = v;
        v = y * 64;
        v = v + 0x20;
        *(s16 *)((u8 *)m + 0x6) = v;
        *(s16 *)((u8 *)m + 0xA) = z;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = col;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        func_8003DB94(r, D_800DF650, 0);
        *(u8 **)((u8 *)n + 0x20) = owner;
        *(s32 *)(owner + 0x14) |= 0x100000;
        func_800A56E0(0x614);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

/* The aimed-shot attempt. THE ROLL IS TWO LOCALS WITH FORCED TYPES, exactly
   as func_800C8844 records: an s32 for the draw and an s16 for the remainder.

   DEVICE: the empty `__asm__ volatile("")` before the early `return 1` is
   worth ONE WORD and emits no instruction. Without it jump.c cross-jumps that
   `return 1`'s `addiu $v0, $zero, 1` into the identical one before the
   epilogue -- find_cross_jump's minimum is 1 for an unconditional jump
   (jump.c:2188) -- which leaves the early block a bare `j`; the conditional
   branch above it is then "a conditional jump jumping over an unconditional
   jump" and jump.c inverts it, so the function is 68 words against 69 with 58
   differing. The barrier breaks the insn match and both the `j` and its
   `addiu` survive. Ruled out at 68/58 each: `&&` and nested ifs for the guard
   (identical output), the guard as `||` round the whole body, the tail's
   `return 1` duplicated into both arms, an explicit `else` on the roll test,
   and returning a local set to 1. The barrier at the TAIL's return instead is
   also 68/58, so it is this block's match that has to be broken.
   69 words, 0 differing. */
s32 func_800CBCA0(u8 *p)
{
    s32 r;
    s16 roll;
    UnkNode *n;
    u8 *q;

    if (p[0x13] != 0 && (*(s32 *)(p + 0x14) & 0x4000) != 0)
    {
        __asm__ volatile("");
        return 1;
    }
    if (D_800E3D40 != 0)
    {
        roll = 0;
    }
    else
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
    }
    if (roll < 0x30)
    {
        q = *(u8 **)(p - 0x14);
        n = func_800CBB98(q[0x24], q[0x25], *(s16 *)(p + 0x88), p);
        if (n != NULL && p[0x13] == 0)
        {
            func_80094E34();
            func_80099844(p, D_800E1AE6);
        }
        return n != NULL;
    }
    if (p[0x13] == 0)
    {
        func_800A6508();
    }
    return 1;
}

/* Three states of the screen-shake/pan effect over D_80083178's +0xA0: ease
   to +0x800, wait for the 0x10 flag and the 0x1C probe, then ease back to 0
   and kill the node. 117 words, 0 differing on the first compile.

   D_80083178 IS A POINTER LOCAL assigned before the switch -- that is where
   the prologue's `lui`/`addiu` pair comes from -- and the three
   D_80083460 bases are THREE SEPARATE locals, which is why case 1's lands in
   the callee-saved $s1 (it is live across two calls) while case 0's and
   case 2's are block-local $a1 and $v1. One shared variable would be a
   single global-alloc quantity in one register. */
void func_800CBDB4(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *m;
    s16 t;

    g = (u8 *)&D_80083178;
    switch (*(s16 *)(p + 0x2))
    {
    case 0:
        *(s16 *)(g + 0xA0) += (0x800 - *(s16 *)(g + 0xA0)) / *(s16 *)(p + 0x4);
        t = *(u16 *)(p + 0x4) - 1;
        *(s16 *)(p + 0x4) = t;
        if (t <= 0)
        {
            *(s16 *)(p + 0x2) += 1;
            *(s16 *)(g + 0xA0) = -0x800;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) -= 1;
            func_800997FC(D_800E1B08);
        }
        break;
    case 1:
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x10) && (s16)func_80042900((s8 *)D_800E3D7C, 0x1C) == 0)
        {
            *(s16 *)(p + 0x4) = 0x10;
            *(s16 *)(p + 0x2) += 1;
            *(u16 *)(h + 0xA) += 1;
            func_800419EC(0x10, 8);
            func_800A56E0(0x818);
        }
        break;
    case 2:
        *(s16 *)(g + 0xA0) += (-*(s16 *)(g + 0xA0)) / *(s16 *)(p + 0x4);
        t = *(u16 *)(p + 0x4) - 1;
        *(s16 *)(p + 0x4) = t;
        if (t <= 0)
        {
            *(s16 *)(g + 0xA0) = 0;
            m = (u8 *)&D_80083460;
            *(u16 *)(m + 0xA) -= 1;
            func_800997FC(D_800E1B2E);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawn the kind-2 node behind the 0x1C probe, or replay the refusal line.

   THE NODE'S TWO STORES MUST BE PLAIN CASTS, NOT `n->field`. As COMPONENT_REFs
   they carry MEM_IN_STRUCT_P, and true_dependence's exemption then lets the
   `lhu %lo(D_80083460 + 0xA)` hoist above them: the three address
   materialisations overlap, reload spreads them over $a2/$a3/$v1 instead of
   reusing $v1 three times, and it is 10-11 differing. Through a `u8 *` the
   stores are ordinary MEMs, nothing is exempt, and every value dies before
   the next is made.

   THE REFUSAL CALL IS WRITTEN LAST, inside the outer `if`, so it falls into
   the shared `return 1`; written as an early arm it is 28 differing.
   `t = 0x10;` stored and then passed is the reused-temporary idiom: the
   `addu $v1, $a0, $zero` is the copy that keeps $a0 for the call.
   52 words, 0 differing. */
s32 func_800CBF88(u8 *p)
{
    u8 *q;
    u8 *g;
    s32 t;

    if (p == D_800E3D7C)
    {
        if (func_8003FA44(1) == 0)
        {
            return 0;
        }
        if (!(s16)func_80042900((s8 *)p, 0x1C))
        {
            if ((s16)func_800A48F0(p, 0x1C, 0x10) < 0)
            {
                return 1;
            }
            q = (u8 *)func_8003FC64(2);
            *(u32 *)(q + 0x10) = (u32)func_800CBDB4;
            t = 0x10;
            *(s16 *)(q + 0x24) = t;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
            func_800419EC(t, 8);
            func_800A56E0(0x818);
            return 1;
        }
        func_800997FC(D_800E1B5D);
    }
    return 1;
}

/* The 0x615 shock effect: refuse while D_800E3D40 is up, otherwise draw a
   random number modulo the record's +0x03 and run the effect when it comes
   out under 0x30 and func_800A48F0 accepts. 77 words, 0 differing.

   `ret` IS NARROW. As `s32` the `sltiu` writes straight into its register
   and the test reads it there -- 75 words against 77. A narrow local makes
   the store a truncation and the read a widening, and because `sltiu` is
   provably 0 or 1 combine turns the widening into the target's plain
   `addu $v0, $s1, $zero`, which reorg then duplicates into the `j`'s delay
   slot. `s8` and `s16` both reach 0; `u8` and `u16` are 2 differing.
   The two-locals split for the draw is func_800C8844's, verbatim. */
s32 func_800CC058(u8 *p)
{
    u8 *q;
    s32 r;
    s16 roll;
    s16 ret;

    ret = 0;
    if (D_800E3D40 == 0)
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
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30 && func_800A48F0((s8 *)p, 7, -0x10) >= 0)
    {
        q = *(u8 **)(p - 0x14);
        if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
        {
            func_800C5BBC((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88),
                          0x202020, 0x40, 0);
            func_800A56E0(0x615);
        }
        if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            func_80099844(p, D_800E1BBC);
        }
    }
    else
    {
        ret = p[0x13] == 0;
    }
    if (ret != 0)
    {
        func_800A6508();
    }
    return 1;
}

/* The same roll one effect further on, with a 0x40 threshold.
   (1) `k`, the 0/1/2 selector, has to be an `s16`: as s32 the two arms write
   it in place and the join's `addu $v1, $v0, $zero` disappears, 70 words
   against 71. `s8` also reaches 0; `u8` and `u16` are 1 differing.
   (2) The tail is a TERNARY, not an if/else pair -- `?:` keeps the copy that
   `if` deletes; as three flat returns it is 67 words / 30 differing.
   (3) The ternary's arms are `!= 0 ? 1 : -1`, which puts the -1 in the `beqz`
   delay slot; the `== 0 ? -1 : 1` spelling branches the other way,
   1 differing. 71 words, 0 differing. */
s32 func_800CC18C(u8 *p)
{
    s32 r;
    s16 roll;
    s16 flag;
    s16 k;

    flag = 0;
    if (D_800E3D40 != 0)
    {
        roll = 0;
    }
    else
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
    }
    if (roll < 0x40)
    {
        func_800AA5E4(p, *(void **)(p - 0x18), *(u8 **)(p - 0x14), p);
        func_800A56E0(0x705);
        if (D_800E3D40 != 0)
        {
            flag = -1;
        }
        k = p[0x13] == 0;
    }
    else
    {
        flag = 1;
        k = (p[0x13] == 0) * 2;
    }
    if (k != 0)
    {
        if (k == 1)
        {
            func_80099844(p, D_800E1BDA);
        }
        else
        {
            func_800A6508();
        }
    }
    if (flag < 0)
    {
        return 0;
    }
    return (flag != 0) ? 1 : -1;
}

/* func_800CD1A4's third sibling: the same chance roll through func_800C8CD8
   -- doubled while D_800E3D40 is up -- with no announcement. 50 words,
   0 differing. */
s32 func_800CC2A8(u8 *p)
{
    u8 *r;

    if (func_800C8CD8(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802020, 0x20,
                  0);
    func_800A56E0(0x611);

    return 1;
}

/* Step the 0x817 marker one tile along its stored direction, re-arming
   whatever it lands on: a func_8009B390 hit takes func_8009CE1C, the
   D_800E3648 slot under it is either handed to func_800CCC20 or cleared
   outright, and the D_800E3548 slot is cleared. 96 words, 0 differing.

   THE RESIDUE WAS THE CALL'S SECOND ARGUMENT. Written `func_800CCC20(0)`
   -- which compiles, because the unit's declaration for it was still the
   empty-paren form -- the function is 96 words with exactly three
   differing: the func_800B500C result lives in $v1 instead of the target's
   $a1, and because it then dies at the `sll` local-alloc ties the scaled
   index to it. The target keeps it in $a1 across the shift because it IS
   the call's second argument. Nothing in the spelling of the index reaches
   that: `s32 s` with and without a cast, three declaration orders, a u8
   local for the byte and a struct-array alias for the subscript are all
   byte-identical, and inverting the if/else is 95 words / 42. */
void func_800CC370(u8 *p)
{
    u8 *r;
    s16 s;
    s16 t;

    *(s16 *)(p + 0x0) += D_8006CCD8[p[0x6]];
    *(s16 *)(p + 0x2) += D_8006CCE8[p[0x6]];
    r = func_8009B390(*(u16 *)(p + 0x0), *(u16 *)(p + 0x2), *(s16 *)(p + 0x4));
    if (r != NULL)
    {
        func_8009CE1C(r, 6, D_800E3D7C[0x11], 9, (s16)(p[0x6] << 9), 0, 3);
    }
    s = func_800B500C(*(s16 *)(p + 0x0), *(s16 *)(p + 0x2), *(s16 *)(p + 0x4));
    if (s >= 0)
    {
        if (D_800E3648[s * 4] == 7)
        {
            func_800CCC20(NULL, s);
        }
        else
        {
            *(s32 *)&D_800E3648[s * 4] = 0;
        }
    }
    t = func_800A70E4(*(s16 *)(p + 0x0), *(s16 *)(p + 0x2), *(s16 *)(p + 0x4));
    if (t >= 0)
    {
        *(s32 *)&D_800E3548[t * 4] = 0;
    }
    if (--p[0x7] == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A node's +0x10 per-frame step. The arity is the +0x10 convention
   func_80040190 casts to, since the only reference to this function anywhere
   is func_800CC5F0 TAKING its address. 64 words, 0 differing. */
void func_800CC4F0(u8 *p, UnkVec *m, u32 c)
{
    *(s16 *)((u8 *)m + 0xE) += func_80064584(*(s16 *)(p + 0x4)) >> 6;
    *(s16 *)((u8 *)m + 0x12) += func_800644B8(*(s16 *)(p + 0x4)) >> 6;
    if (*(s16 *)(p + 0x6) == 0)
    {
        func_800CC5F0(*(u16 *)((u8 *)m + 0x2) >> 6, *(u16 *)((u8 *)m + 0x6) >> 6,
                      *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x4), 0, 1);
        func_800CC5F0(*(u16 *)((u8 *)m + 0x2) >> 6, *(u16 *)((u8 *)m + 0x6) >> 6,
                      *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x4), 1, 1);
    }
    *(s16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_800CC4F0's pair of markers -- func_800CC88C
   three functions down is the line-for-line template, including the
   `*(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);` read-back and the two
   `(e != 0) ? :` ternaries off the same fifth argument.

   `*(u32 *)(r + 0xC) = 0x808080;` MUST COME BEFORE `*(u8 **)(r + 0x8)`:
   the other order is 85 words / 4 differing, because the `li` for 0x808080 is
   split into `lui`/`ori` after reload and sched2 then places the `ori` one
   slot early. The six-argument signature is read off the landed caller
   func_800CC4F0, which passes `(x >> 6, y >> 6, ang, d, 0|1, 1)`; adding the
   prototype leaves that caller at 64 words / 0 differing. 85 words,
   0 differing. */
UnkNode *func_800CC5F0(s16 x, s16 y, s16 ang, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CC4F0;
        func_8004491C(n, (u32)func_800CEEFC);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xE) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x12) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s16 *)((u8 *)m + 0x16) = ang;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        *(u8 **)(r + 0x8) = D_800E03D4;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1E) = (e != 0) ? 0x1000 : 0x400;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
        *(s16 *)(r + 0x10) = (e != 0) ? 0x60 : 0x20;
        *(s16 *)(r + 0x6) = 0x20;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4) = d;
        *(s16 *)(p + 0x2) = 8;
        *(s16 *)(p + 0x6) = f;
    }
    return n;
}

/* A node's +0x10 colour step: fade the renderable's RGB triple at +0xC..+0xE
   towards 0xE0 while the view's +0x00 counter is zero and away from it
   afterwards, each step a fraction 1/N of the remaining distance with N the
   countdown at +0x02. The divisor is RE-DEREFERENCED at all six sites --
   `*(s16 *)(p + 0x2)` written out each time -- which is what keeps the three
   `lh` reloads per arm. The two-arm shape (the `-=` arm reads the byte as the
   dividend, the `+=` arm reads 0xE0 minus it) is the whole difference between
   the arms. 82 words, 0 differing on the first compile. */
void func_800CC744(u8 *p, UnkVec *b, u8 *c)
{
    if (*(s16 *)p == 0)
    {
        c[0xC] += (0xE0 - c[0xC]) / *(s16 *)(p + 0x2);
        c[0xD] += (0xE0 - c[0xD]) / *(s16 *)(p + 0x2);
        c[0xE] += (0xE0 - c[0xE]) / *(s16 *)(p + 0x2);
        if (--*(s16 *)(p + 0x2) <= 0)
        {
            *(s16 *)(p + 0x2) = 0x10;
            *(s16 *)p += 1;
        }
    }
    else
    {
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x2);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x2);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x2);
        if (--*(s16 *)(p + 0x2) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* The kind-2 marker spawner. The 0x1C scale is a READ-BACK of the 0x1E one
   (`sh` then `lhu` with the load-delay `nop`), not a second copy of the
   ternary. The positions are s16 with `x * 64 + 0x20`, func_800C1A44's form
   -- the `sll 16`/`sra 10` pair is the narrow parameter's sign-extension
   folded into the multiply. 76 words, 0 differing on the first compile. */
UnkNode *func_800CC88C(s16 x, s16 y, s16 ang, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CC744;
        func_8004491C(n, (u32)func_80045C34);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800E03D4;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1E) = (e != 0) ? 0x6000 : 0x3000;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
        *(s16 *)(r + 0x10) = (e != 0) ? 0x60 : 0x20;
        *(s16 *)(r + 0x6) = 0x20;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4) = d;
        *(s16 *)(p + 0x2) = 4;
        *(s16 *)(p + 0x6) = e;
    }
    return n;
}

/* func_800999D4's close over a byte countdown at +0x7. 22 words,
   0 differing. */
void func_800CC9BC(u8 *p)
{
    u8 *g;

    if (--p[0x7] == 0)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x817 marker set: one owner node out of D_8014A000's row for
   D_800E5908, four direction nodes behind it in the same row, then the two
   func_800CC88C markers, four pairs of func_800CC5F0 sparks, and -- if
   func_8009B390 finds anything under the spot -- a func_8009CE1C hit.

   D_8014A000 IS A ROW OF FIVE 40-BYTE (0xA-WORD) NODE BUFFERS, and the
   address arithmetic says so: the owner is `&D_8014A000[D_800E5908 * 200]`
   and each of the other four is `&D_8014A000[(D_800E5908 * 5 + i) * 40 +
   0x28]` -- one combined `(row * 5 + i) * 40` multiply, which is what a
   row-of-five gives, not the two separate multiplies `D_800E5908 * 200 +
   i * 40` would. The `+ 0x28` folds onto the base and is hoisted as
   `addiu $s5, $v1, 0x28` off the `%lo` the owner's address already
   materialised.

   `*(u32 *)((u8 *)e + 0x10) = (u32)func_800CC9BC;` MUST BE THE CAST STORE.
   As `e->unk_10 = ...` -- a COMPONENT_REF, so MEM_IN_STRUCT_P -- that store
   is scheduled eleven words later, the loop-hoisted `&func_800CC370` is
   materialised BEFORE the `x` parameter's own store instead of after it, so
   it cannot reuse $s0 and takes a SEVENTH callee-saved register: 133 words
   against 131, 112 differing, and the two extra words are $s6's save and
   restore. The same cast on `n->unk_10` inside the loop changes nothing
   (same md5); moving either store in the source changes nothing or makes it
   worse (116 / 102 differing).

   `i` IS ONE VARIABLE ACROSS BOTH LOOPS AND LIVE BETWEEN THEM: the two
   func_800CC88C calls take `i << 10` with `i` still at the first loop's exit
   value of -1, which is where the `sll 26`/`sra 16` pair at words 67-68
   comes from -- the s16 parameter's narrowing folded into the shift.
   131 words, 0 differing. */
void func_800CCA14(s16 x, s16 y, s16 z)
{
    UnkNode *e;
    UnkNode *n;
    u8 *r;
    u8 *g;
    s32 i;

    e = func_8003FE78(0, (UnkNode *)&D_8014A000[D_800E5908 * 200], 0xA);
    /* The CAST store, not `e->unk_10 =`: see the comment block. */
    *(u32 *)((u8 *)e + 0x10) = (u32)func_800CC9BC;
    *(s16 *)((u8 *)e + 0x20) = x;
    *(s16 *)((u8 *)e + 0x22) = y;
    *(s16 *)((u8 *)e + 0x24) = z;
    *((u8 *)e + 0x27) = 8;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    for (i = 3; i >= 0; i--)
    {
        n = func_8003FE78(0, (UnkNode *)&D_8014A000[(D_800E5908 * 5 + i) * 40 + 0x28], 0xA);
        n->unk_10 = (u32)func_800CC370;
        *(s16 *)((u8 *)n + 0x20) = *(u16 *)((u8 *)e + 0x20);
        *(s16 *)((u8 *)n + 0x22) = *(u16 *)((u8 *)e + 0x22);
        *(s16 *)((u8 *)n + 0x24) = *(u16 *)((u8 *)e + 0x24);
        *((u8 *)n + 0x27) = 8;
        *((u8 *)n + 0x26) = i * 2;
    }
    func_800CC88C(*(s16 *)((u8 *)e + 0x20), *(s16 *)((u8 *)e + 0x22), *(s16 *)((u8 *)e + 0x24), i << 10, 0);
    func_800CC88C(*(s16 *)((u8 *)e + 0x20), *(s16 *)((u8 *)e + 0x22), *(s16 *)((u8 *)e + 0x24), i << 10, 1);
    for (i = 3; i >= 0; i--)
    {
        func_800CC5F0(*(s16 *)((u8 *)e + 0x20), *(s16 *)((u8 *)e + 0x22), *(s16 *)((u8 *)e + 0x24), i << 10, 0, 0);
        func_800CC5F0(*(s16 *)((u8 *)e + 0x20), *(s16 *)((u8 *)e + 0x22), *(s16 *)((u8 *)e + 0x24), i << 10, 1, 1);
    }
    r = func_8009B390(*(u16 *)((u8 *)e + 0x20), *(u16 *)((u8 *)e + 0x22), *(s16 *)((u8 *)e + 0x24));
    if (r != NULL)
    {
        func_8009CE1C(r, 0xC, D_800E3D7C[0x11], 9, 0x400, 0, 3);
    }
}

/* The 0x817 slot step func_800CC370 drives, in two modes. With a NULL first
   argument it re-arms slot `idx`: clear the +0x03 0x80 bit, clear 0x800 in
   the spot's +0x14 flags and bump D_800E5908. With an actor it rolls against
   the actor's +0x03, and on a roll under 0x40 spawns the func_800B835C tint
   node over {0x1000340, 0x200020} and clears the counter. Both modes close on
   the same func_800CCA14 / func_800A56E0(0x817) tail. 96 words, 0 differing.

   THE REMAINDER IS AN `s16` LOCAL AND THE DRAW AN `s32` ONE -- the same split
   func_800C8844 records, and the width is what decides the register. As
   `s32 roll` the remainder keeps mfhi's own $v1, the target's
   `addu $v0, $v1, $zero` copy disappears and reorg fills the freed `j` delay
   slot with a duplicate `slti`: 96 words, 4 differing. `u16` and `u8` are
   96 / 1, and narrowing the DRAW instead (`s16 r`) is 96 / 7.

   The `D_800E3D40 != 0` arm must be its own `roll = 0` assignment, not an
   initialiser before the `if`: hoisted, the whole block re-orders at
   90 differing. */
s32 func_800CCC20(u8 *p, s16 idx)
{
    s32 r;
    s16 roll;
    s32 arr[2];
    u8 f;

    if (p == NULL)
    {
        f = D_800E3648_s[idx].unk_03;
        if ((f & 0x80) == 0)
        {
            return 1;
        }
        D_800E3648_s[idx].unk_03 = f & 0x7F;
        D_800E39C8[idx].unk_14 &= 0xF7FF;
        D_800E5908++;
    }
    else
    {
        if (D_800E3D40 != 0)
        {
            roll = 0;
        }
        else
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
        }
        if (roll >= 0x40)
        {
            func_800A6508();
            return 1;
        }
        D_800DCF78 = 0x202080;
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C((u8 *)&D_800DCF78 - 0x10, arr, 1, 0);
        D_800E5908 = 0;
    }
    func_800CCA14(D_800E39C8[idx].unk_06, D_800E39C8[idx].unk_07, D_800E39C8[idx].unk_12);
    func_800A56E0(0x817);
    return 1;
}

/* The tumble step at a node's +0x10: state 0 counts +0x6 down, fires 0x701
   at zero and -- once the mover has fallen past its +0xA target -- spawns the
   0xA effect, flips the fall, re-aims the mover off D_80083160's +0xC8
   bearing and arms state 1; state 1 eases +0x1A toward 0x400, dithers it by
   +/-0x100 on the counter's low bit, bleeds a quarter off each colour byte
   and retires the node at zero. 153 words, 0 differing.

   THREE SPELLINGS ARE LOAD-BEARING:
   (1) THE DITHER IS A TERNARY, `r->unk_1A += (p6 & 1) ? 0x100 : -0x100;`.
   As an if/else writing `r->unk_1A` in each arm cse forwards the value just
   stored and the target's reload (`lhu $v1, 0x1A($s3)`) is gone -- 11
   differing, and neither a cast through `u8 *` nor a quotient temp moves it.
   (2) `g->unk_C8` AS A COMPONENT_REF, with `g` an `Unk80083160 *`. Through
   `*(s16 *)(g + 0xC8)` the argument load is a plain MEM, `v->unk_14 = -...`
   is MEM_IN_STRUCT_P, the two do not depend, and the store sinks into the
   first `jal`'s delay slot where the target has a `nop` -- 151 words.
   (3) TWO SEPARATE BASE LOCALS. D_80083160's wants $s0 across the two calls
   and D_80083460's wants a block-local $v1; one variable is a global-alloc
   quantity and takes $s0 in both arms (4 differing). Splitting them is only
   free BECAUSE of (2) -- with the cast form it drops the $s0 save entirely.
   `(s16)(*(s16 *)(p + 8) * 0x200)`: `<< 9` turns the `lh` into `lhu`, which
   is docs/matching-levers.md's `* (1 << k)` rule read from the other side. */
void func_800CCDA0(u8 *p, UnkVec *v, UnkSlot *r)
{
    Unk80083160 *g;
    u8 *h;

    if (*(s16 *)(p + 0x4) == 0)
    {
        v->unk_14 += 0x20000;
        if (--*(s16 *)(p + 0x6) == 0)
        {
            func_800A56E0(0x701);
        }
        if (*(s16 *)(p + 0xA) < *(s16 *)((u8 *)v + 0xA))
        {
            func_8009CE1C(*(u8 **)p, 0xA, D_800E3D7C[0x11], 8, (s16)(*(s16 *)(p + 0x8) * 0x200), 0,
                          2);
            v->unk_14 = -v->unk_14;
            g = &D_80083160;
            v->unk_0C = func_80064584(g->unk_C8) << 8;
            v->unk_10 = func_800644B8(g->unk_C8) << 8;
            r->unk_14 |= 0xC;
            *(s16 *)(p + 0x6) = 0xC;
            *(s16 *)(p + 0x4) += 1;
        }
    }
    else
    {
        v->unk_14 += 0x80000;
        r->unk_1A += (0x400 - r->unk_1A) / *(s16 *)(p + 0x6);
        r->unk_1A += (*(s16 *)(p + 0x6) & 1) ? 0x100 : -0x100;
        r->unk_0C[0] -= r->unk_0C[0] / *(s16 *)(p + 0x6);
        r->unk_0C[1] -= r->unk_0C[1] / *(s16 *)(p + 0x6);
        r->unk_0C[2] -= r->unk_0C[2] / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) == 0)
        {
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
}

/* The 0x50D spawner behind func_800CDD0C's roll: the s32 draw and the s16
   remainder again, then a node whose renderable picks one of two cel lists on
   a 2-bit roll.

   `return n != NULL;` MUST BE THE FUNCTION'S LAST STATEMENT, with the body
   FALLING THROUGH into it and the `roll >= 0x30` arm returning 1 above it.
   That one block placement is worth two words and both are invisible in the
   middle of the function: with the return at the end of the `roll < 0x30` arm
   it is 103 words / 47 differing, because the shared tail then sits directly
   after the body and reorg MOVES its `sltu` into the `j`'s delay slot instead
   of COPYING it; with the return ALSO written at the end of the body it is
   105 / 49, because the final block's only predecessor is then the n == NULL
   branch and gcc folds it to `move $v0, $zero`. Two predecessors plus an
   out-of-line placement is what gives the target its two `sltu $v0, $zero,
   $s1` words. 104 words, 0 differing. */
s32 func_800CD004(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *v;
    u8 *g;
    s32 rr;
    s16 roll;

    if (D_800E3D40 == 0)
    {
        rr = func_800A6D30();
        if (p[3] != 0)
        {
            roll = rr % p[3];
        }
        else
        {
            roll = 0;
        }
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30)
    {
        n = func_8003FC64(2);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800CCDA0;
            func_8004491C(n, (u32)D_80045340);
            q = *(u8 **)(p - 0x14);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = (q[0x24] << 6) + 0x20;
            *(s16 *)((u8 *)m + 0x6) = (q[0x25] << 6) + 0x20;
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x88) - 0x200;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u32 *)(r + 0xC) = 0x808080;
            *(u8 **)(r + 0x8) = (rand() & 3) ? D_800DFD0C : D_800DFD30;
            *(s16 *)(r + 0x10) = 0x20;
            v = (u8 *)n + 0x20;
            *(s16 *)(v + 0xA) = *(u16 *)(p + 0x88) - D_800DDC40[p[0x13]];
            *(u8 **)v = p;
            *(s16 *)(v + 0x6) = 0xC;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
            func_800A56E0(0x50D);
        }
    }
    else
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    return n != NULL;
}

/* THREE 60-WORD EFFECT HANDLERS, one source. Roll a chance through one of the
   probe callers -- the chance itself doubles from 0x40 to 0x400 while the
   engine's D_800E3D40 flag is up -- and on a miss poke func_800A6508 and
   leave. On a hit, announce it, show the overlay's own message when the
   record's +0x13 is clear, and unless the renderer already carries 0x8000,
   fire func_800C5BBC at the tile centre with a tint.

   func_800A6508 TAKES NO ARGUMENTS, and the witness is a `nop`. The target
   leaves its `jal` delay slot empty while `$a0` already holds the record --
   put there by the `addu $a0, $s0, $zero` in the BRANCH delay slot above,
   which is the OTHER path's func_80099844 argument that reorg hoisted
   because it is harmless here. Given a `u8 *` parameter the call emits its
   own copy, that copy fills the delay slot, and the function is one word
   over at the same length. An empty `jal` slot next to a live argument
   register is a nullary callee.

   60 words each, 0 differing. */
s32 func_800CD1A4(u8 *p)
{
    u8 *r;

    if (func_800C8ABC(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1BF3);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF0C);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x808080, 0x20,
                  0);
    func_800A56E0(0x520);

    return 1;
}

/* func_800CD1A4 with func_800C8C1C, D_800E1C03/D_800DCF14, a white tint, a
   0x40 fifth argument and sound 0x612. 60 words, 0 differing. */
s32 func_800CD294(u8 *p)
{
    u8 *r;

    if (func_800C8C1C(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1C03);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF14);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0xFFFFFF, 0x40,
                  0);
    func_800A56E0(0x612);

    return 1;
}

/* func_800CD1A4 with func_800C8844, the miss gate on the record's +0x14
   0x4000 flag instead of its +0x13 byte, a 1 in the sixth argument and no
   sound. 60 words, 0 differing. */
s32 func_800CD384(u8 *p)
{
    u8 *r;

    if (func_800C8844(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x8) == 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1C14);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF1C);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802080, 0x20,
                  1);

    return 1;
}

/* The 0x701/0x612 rise-and-settle handler, a three-state machine over the
   actor's +0x04 state byte: state 0 lifts the mover's +0x14 delta until the
   integer half of +0x08 reaches the actor's +0x0A, state 1 flashes the
   renderable's colour on a 0x10-frame countdown, and the default arm fades
   the three colour bytes by 1/N and retires the node. 142 words,
   0 differing.

   TWO STATEMENT ORDERS ARE LOAD-BEARING, 6 differing between them.
   (1) In the +0x612 arm, `*(u16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0xA);` must
   come BEFORE `m->unk_14 = 0;`: the other order puts `sw $zero, 0x14($s2)`
   ahead of the `lhu` and the `li 0x612` no longer fills the load-delay slot,
   3 differing.
   (2) In the fade arm, `g = (u8 *)&D_80083460;` must come AFTER the
   `p - 0x2` and D_800814A0 updates even though its own store is last: the
   target materialises %hi(D_800814A0) first and splits D_80083460's
   lui/addiu around the `lhu -0x2($s0)`, which is 3 differing the other way.

   func_800CD6AC is called with TWO arguments here and in func_800CD6E0, both
   times `0x10` in the `jal` delay slot, so its unread second parameter is
   real; the landed definition below was widened to match. */
void func_800CD474(u8 *p, UnkVec *m, u8 *r)
{
    u8 *g;

    if (*(s16 *)(p + 0x4) == 0)
    {
        m->unk_14 += 0x20000;
        *(s16 *)(p + 0x6) -= 1;
        if (*(s16 *)(p + 0x6) == 0)
        {
            func_800A56E0(0x701);
        }
        if (*(s16 *)((u8 *)m + 0xA) + 0x10 >= *(s16 *)(p + 0xA))
        {
            func_800419EC(8, 8);
            *(u16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0xA);
            m->unk_14 = 0;
            *(s16 *)(p + 0x6) = 8;
            *(s16 *)(p + 0x4) += 1;
            func_800A56E0(0x612);
        }
        *(u32 *)(r + 0xC) = (*(u16 *)(p + 0xC) & 1) ? 0x808080 : 0x404040;
    }
    else if (*(s16 *)(p + 0x4) == 1)
    {
        *(u32 *)(r + 0xC) = (*(u16 *)(p + 0xC) & 1) ? 0x808080 : 0x404040;
        *(s16 *)(p + 0x6) -= 1;
        if (*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x10;
            *(u32 *)(r + 0xC) = 0x808080;
            *(s16 *)(p + 0x4) += 1;
            if (*(s16 *)(p + 0x8) == 0)
            {
                func_800CD6AC(*(u8 **)p, 0x10);
            }
        }
    }
    else
    {
        r[0xC] -= r[0xC] / *(s16 *)(p + 0x6);
        r[0xD] -= r[0xD] / *(s16 *)(p + 0x6);
        r[0xE] -= r[0xE] / *(s16 *)(p + 0x6);
        *(s16 *)(p + 0x6) -= 1;
        if (*(s16 *)(p + 0x6) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
        }
    }
    m->unk_08 += m->unk_14;
    *(u16 *)(p + 0xC) += 1;
}

/* func_800CD1A4's charge roll on its own, result dropped. The record never
   enters a register of its own -- it is the incoming $a0 passed straight
   through. 13 words, 0 differing. */
/* TWO PARAMETERS, the second unread: both func_800CD474 and func_800CD6E0
   pass 0x10 in the `jal`'s delay slot, independently. Its own listing
   overwrites $a1 at entry and never reads it, which is why one parameter had
   no witness. 13 words, 0 differing either way. */
void func_800CD6AC(u8 *p, s16 arg)
{
    func_800C8900(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10);
}

/* The four-spark burst: refuse while D_800E3D40 is set or the roll lands at
   0x30 or more, bail out to func_800CD6AC when the owner's +0x14 is already
   0x8000-flagged, and otherwise spawn four kind-2 nodes on the four cardinal
   offsets of D_8006CCD8/D_8006CCE8 with func_800CD474 in their +0x10 slot.
   140 words, 0 differing.

   THE TABLE INDEX IS `i * 2`, NOT `i`. The walked pointer's stride is
   `addiu $s3, $s3, 0x4` and D_8006CCE8 is reached at `i << 2` bytes into an
   s16 array, so the four steps are every OTHER entry of the eight-way table.
   With `[i]` it is 140 words / 22 differing.

   AND THE TWO COORDINATE SUMS NEED BOTH HALVES OF ONE LEVER, 21 differing
   between the wrong and the right spelling:
     (q[0x24] << 6) + (D[i * 2] << 5) + 0x20   22 differing
     (D[i * 2] << 5) + 0x20 + (q[0x24] << 6)   21 differing
     (q[0x24] << 6) + 0x20 + (D[i * 2] << 5)    2 differing
     ... with `* 32` for `<< 5`                 0 differing
   The constant placement is fold's `associate:` moving it onto the other
   operand, and `* 32` rather than `<< 5` is what keeps the `lh`: through the
   shift the table read narrows to `lhu`, because only the low 16 bits of the
   sum are stored. `q[0x24] * 64` for `<< 6` and `i * 1024` for `i << 10` are
   both ties.

   The payload pointer store `*(u8 **)((u8 *)n + 0x20) = p;` really is
   written TWICE -- two `sw $s2, 0x20($s0)` with the three halfword stores
   through the `w` local between them. */
s32 func_800CD6E0(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    u8 *g;
    u8 *q;
    u8 *r;
    u8 *w;
    s32 rnd;
    s16 roll;
    s32 i;

    if (D_800E3D40 == 0)
    {
        rnd = func_800A6D30();
        if (p[0x3] != 0)
        {
            roll = rnd % p[0x3];
        }
        else
        {
            roll = 0;
        }
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30)
    {
        q = *(u8 **)(p - 0x14);
        if (*(u16 *)(q + 0x14) & 0x8000)
        {
            func_800CD6AC(p, 0x10);

            return 1;
        }
        if (func_8003FA44(4) == 0)
        {
            return 0;
        }
        for (i = 0; i < 4; i++)
        {
            n = func_8003FC64(2);
            n->unk_10 = (u32)func_800CD474;
            func_8004491C(n, (u32)func_80045C34);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = (q[0x24] << 6) + 0x20 + (D_8006CCD8[i * 2] * 32);
            *(s16 *)((u8 *)m + 0x6) = (q[0x25] << 6) + 0x20 + (D_8006CCE8[i * 2] * 32);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0x88) - 0x200;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(r + 0x1E) = 0x2000;
            *(u8 **)(r + 0x8) = D_800E03E0;
            *(s16 *)(r + 0x10) = 0x20;
            *(s16 *)(r + 0x16) = 0x400;
            *(u32 *)(r + 0xC) = 0x808080;
            *(s16 *)(r + 0x18) = (i << 10) + 0x400;
            *(u16 *)(r + 0x14) |= 0xC;
            w = (u8 *)n + 0x20;
            *(s16 *)(w + 0xA) = *(u16 *)(p + 0x88);
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)(w + 0x6) = 0xC;
            *(s16 *)(w + 0x8) = i;
            *(u8 **)((u8 *)n + 0x20) = p;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
            func_800A56E0(0x50D);
        }

        return 1;
    }
    if (p[0x13] == 0)
    {
        func_800A6508();
    }

    return 1;
}

/* 33 words, 0 differing. */
void func_800CD910(u8 *p)
{
    u8 *g;

    if (--*(s16 *)(p + 0x6) <= 0)
    {
        func_800CD994(*(u8 **)p, 0x10);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Apply effect 0xA to the record unless it is kind 0x2E. The restore pair is
   the point: +0x13 is read before func_800A48F0, read again after it, the OLD
   value is written back for the announcement and the NEW one put back
   afterwards. 55 words, 0 differing. */
s32 func_800CD994(u8 *p, s16 e)
{
    s32 t;
    s32 u;

    if (p[0x13] != 0x2E)
    {
        if (p[0x13] == 0x1E)
        {
            if (*(s32 *)(p + 0xA4) != 0)
            {
                return 0;
            }
            if (*(s16 *)(p + 0xAA) != 0)
            {
                return 0;
            }
        }
        if ((s16)func_80042900((s8 *)p, 0xA) != 0)
        {
            return 0;
        }
        t = p[0x13];
        if (func_800A48F0((s8 *)p, 0xA, (s8)e) < 0)
        {
            return 0;
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            u = p[0x13];
            p[0x13] = t;
            func_80099844(p, D_800E1C25);
            p[0x13] = u;
        }
    }
    return 0;
}

/* The 0x613 spawner: find the first free one of the four 2-byte slots at
   +0x2C, roll against it, and either report through func_800CD994 or put up
   the effect node. func_800CDD0C's roll with a 0x40 threshold.

   TWO SPELLINGS, both measured. (1) THE SLOT COUNTER AND THE ROLL ARE ONE
   `s16 i`. With a separate `s16 roll` the remainder's nonzero_bits are known
   (`r % p[3]` is under 256) and the sign-extension at the join disappears:
   104 words against 106, 64 differing. Sharing the loop counter's pseudo is
   what forces the three `sll 16`/`sra 16` the target has around
   .L800CDB24. (2) THE SCALE IS `i << 1`, NOT `i * 2`: as `i * 2` the row
   address comes out index-first, `addu $v0, $v0, $s1` for the target's
   `addu $v0, $s1, $v0` -- 1 differing. Five other spellings of the same
   subscript (index first, offset first, `((s8 *)(p + 0x2C))[i * 2]`, a
   parenthesised row, a `(s8 *)` cast on the base) all stay at 1.
   106 words, 0 differing. */
s32 func_800CDA70(u8 *p)
{
    UnkNode *n;
    u8 *q;
    u8 *g;
    s32 r;
    s16 i;

    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        return 1;
    }
    for (i = 0; i < 4; i++)
    {
        if (*(s8 *)(p + (i << 1) + 0x2C) == 0)
        {
            break;
        }
    }
    if (i < 4)
    {
        if (D_800E3D40 == 0)
        {
            r = func_800A6D30();
            if (p[3] != 0)
            {
                i = r % p[3];
            }
            else
            {
                i = 0;
            }
        }
        else
        {
            i = 0;
        }
        if (i < 0x40)
        {
            q = *(u8 **)(p - 0x14);
            if (*(u16 *)(q + 0x14) & 0x8000)
            {
                func_800CD994(p, 0x10);
                return -1;
            }
            if (func_8003FA44(2) == 0)
            {
                return 0;
            }
            n = func_8003FC64(2);
            n->unk_10 = (u32)func_800CD910;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)((u8 *)n + 0x26) = 0xC;
            func_800C5E5C((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88),
                          D_800DF820, 0);
            func_800A56E0(0x613);
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
            return -1;
        }
        else if (p[0x13] == 0)
        {
            func_800A6508();
        }
    }
    return 1;
}

/* The random charge roll on the player record. THE REMAINDER IS AN `s16`
   LOCAL, the same split func_800C8844 records: as `s32 roll` the `mfhi` lands
   straight in $s0 and the function is 59 words against 61 with 40 differing.
   D_800E3D7C is dereferenced INLINE while D_80083460 takes a local: a local
   for both is 61 / 2. 61 words, 0 differing. */
s32 func_800CDC18(u8 *p)
{
    s32 r;
    s16 roll;
    u8 *g;

    if (p[0x13] == 0)
    {
        roll = 0;
        if (D_800E3D40 == 0)
        {
            r = func_800A6D30();
            if (p[3] != 0)
            {
                roll = r % p[3];
            }
        }
        if ((s16)func_80042900((s8 *)p, 0xA) != 0)
        {
            roll = 0xFF;
        }
        if (roll < 0x30)
        {
            *(u16 *)(D_800E3D7C + 0xA2) |= 0x100;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
        }
        else if (p[0x13] == 0)
        {
            func_800A6508();
        }
    }
    return 1;
}

/* The func_800C8844 probe's s32 draw and s16 remainder again, here gated on
   D_800E3D40 as well as on +0x3, so the two `roll = 0` arms cross-jump into
   one block. 64 words, 0 differing. */
s32 func_800CDD0C(u8 *p)
{
    u8 *q;
    s32 r;
    s16 roll;

    if (D_800E3D40 == 0)
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
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30)
    {
        func_800C8DB0(p, D_800E3D40 != 0 ? 0x400 : 0x20, 0x10);
        q = *(u8 **)(p - 0x14);
        if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
        {
            func_800C5E5C((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88),
                          D_800DF870, 0);
            func_800A56E0(0x70A);
        }
    }
    else if (p[0x13] == 0)
    {
        func_800A6508();
    }
    return 1;
}

/* 13 words, 0 differing. */
void func_800CDE0C(void)
{
    u8 *p;
    u8 *h;

    p = D_800E3D7C;
    h = p;
    do
    {
        *(s32 *)(p + 0x1C) |= 0x40000000;
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != h);
}

/* Both tables in one pass: add to the counter of every live slot standing on
   the given tile. A leaf -- no frame, no $ra -- so the two parameters are
   re-sign-extended for the second loop out of $t1/$t2, which is what the
   second `sll`/`sra` pair is. func_800CDF40 is this source with `=` for
   `+=`. 64 words, 0 differing. */
void func_800CDE40(s16 x, s16 y, s16 v)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            D_800E36C8[i].unk_04 += v;
        }
    }
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            D_800E39C8[i].unk_12 += v;
        }
    }
}

/* func_800CDE40 with the counter SET rather than added to.
   58 words, 0 differing. */
void func_800CDF40(s16 x, s16 y, s16 v)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            D_800E36C8[i].unk_04 = v;
        }
    }
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            D_800E39C8[i].unk_12 = v;
        }
    }
}

/* The five-by-five tile-height ripple the dungeon runs while a floor
   rises or sinks: three phases off +0x6A, each walking the same 5x5 grid
   of the 6-byte cell table D_8008333C[0] and feeding every slot standing
   on the tile through func_800CDE40 / func_800CDF40.

   THE THIRD ARGUMENT OF func_800CDE40 AND func_800CDF40 IS `s16`, NOT
   `u16`, AND THIS FUNCTION IS THE FIRST WITNESS.  MIPS's PROMOTE_MODE
   keeps the signedness, so a `u16` parameter makes the call site
   `andi $a2, 0xFFFF` where the target has `sll`/`sra 16`: one word per
   call site, four sites, 301 words against 304.  Both callees stay at
   64/64 and 58/58 words, 0 differing, with the narrower type -- their own
   bodies never read the sign.

   EVERY TEMPORARY IS PER-LOOP, AND THAT IS THE WHOLE RESIDUE, 40 TO 0.
   sched.c's `birthing_insn_p` only lifts an insn to LAUNCH_PRIORITY when
   its destination pseudo has `reg_n_sets == 1`, so a local written in two
   of the four nests is never deferred and sched1 drops its load at the top
   of the block instead of at the end of the load group.  `c1`..`c4`,
   `wa`/`wc`, `ta`/`tc`, `d1`/`d3`, `e1`/`e3` and `qa`..`qd` are the same
   four expressions written four times for that reason; one shared set of
   names is 304 words and 40 differing, and the first spellings with one
   `c`, one `w`, one `t` and one `q` were 47.

   THE TILE BASE IS AN `s32`, NOT A `u8 *`.  C's `pointer_int_sum` puts the
   pointer operand FIRST whichever way the source is written, so a `u8 *`
   base gives `addu $v1, $s5, $v1` at all four sites where the target has
   `addu $v1, $v1, $s5`; that is the last 4 differing and no ordering of
   `base + index * 6` / `index * 6 + base` reaches it.

   Three more spellings, each measured: the cursor must be a real biv
   (`qa++`) or loop.c folds the 0x36 into the load displacement and the
   preheader gets `move $s2, $s4` instead of `addiu $s2, $s4, 0x36`;
   `for (j = 0, qa = ...)` rather than a statement before the loop, so the
   giv's init follows `j = 0`; and the +0x6A counter must be READ before
   D_800E296C's store (`n = *(u16 *)(p + 0x6A);` ... `= n + 1;`), because
   sched1 cannot lift that load over the global's `sw` and the plain
   statement order costs the `nop` after it -- 305 words against 304.
   304 words, 0 differing. */
void func_800CE028(u8 *p)
{
    s16 *wa;
    s16 *wc;
    s32 ta;
    s32 tc;
    u8 *c1;
    u8 *c2;
    u8 *c3;
    u8 *c4;
    u8 *ga;
    u8 *gb;
    u8 *gc;
    u8 *r;
    s32 i;
    s32 j;
    s32 d1;
    s32 e1;
    s32 d3;
    s32 e3;
    s32 n;
    s16 *qa;
    s16 *qb;
    s16 *qc;
    s16 *qd;

    if (*(s16 *)(p + 0x6A) == 0)
    {
        wa = (s16 *)D_8008333C;
        ta = D_8008333C[0];
        for (i = 0, r = p; i < 5; i++)
        {
            for (j = 0, qa = (s16 *)(r + 0x36); j < 5; j++)
            {
                c1 = (u8 *)((((i + *(s16 *)(p + 0x2)) << wa[10]) + *(s16 *)p + j) * 6 + ta);
                e1 = *qa;
                d1 = (e1 - *(s16 *)(c1 + 0x2)) / *(s16 *)(p + 0x68);
                *(u16 *)(c1 + 0x2) += d1;
                func_800CDE40(*(u16 *)p + j, *(u16 *)(p + 0x2) + i, -d1);
                qa++;
            }
            r += 0xA;
        }
        func_800CDE0C();
        if ((*(u16 *)(p + 0x68) & 7) == 0)
        {
            func_800A56E0(0x818);
        }
        if (--*(s16 *)(p + 0x68) <= 0)
        {
            for (i = 0, r = p; i < 5; i++)
            {
                for (j = 0, qb = (s16 *)(r + 0x36); j < 5; j++)
                {
                    c2 = (u8 *)((((i + *(s16 *)(p + 0x2)) << wa[10]) + *(s16 *)p + j) * 6 + ta);
                    *(u16 *)(c2 + 0x2) = *(u16 *)qb;
                    func_800CDF40(*(u16 *)p + j, *(u16 *)(p + 0x2) + i, -*(u16 *)qb);
                    qb++;
                }
                r += 0xA;
            }
            func_8009D6F4();
            ga = (u8 *)&D_80083460;
            *(u16 *)(ga + 0xA) -= 1;
            n = *(u16 *)(p + 0x6A);
            D_800E296C &= 0xFFF7FFFF;
            *(s16 *)(p + 0x6A) = n + 1;
        }
    }
    else if (*(s16 *)(p + 0x6A) == 1)
    {
        gb = (u8 *)&D_80083460;
        if (*(u16 *)(gb + 0x2) & 0x10)
        {
            if (--*(s16 *)(p + 0x6C) <= 0)
            {
                *(u16 *)(gb + 0xA) += 1;
                *(s16 *)(p + 0x68) = 0x20;
                func_800419EC(0x20, 8);
                *(s16 *)(p + 0x6A) += 1;
                D_800E296C |= 0x80000;
            }
        }
    }
    else
    {
        wc = (s16 *)D_8008333C;
        tc = D_8008333C[0];
        for (i = 0, r = p; i < 5; i++)
        {
            for (j = 0, qc = (s16 *)(r + 0x4); j < 5; j++)
            {
                c3 = (u8 *)((((i + *(s16 *)(p + 0x2)) << wc[10]) + *(s16 *)p + j) * 6 + tc);
                e3 = *qc;
                d3 = (e3 - *(s16 *)(c3 + 0x2)) / *(s16 *)(p + 0x68);
                *(u16 *)(c3 + 0x2) += d3;
                func_800CDE40(*(u16 *)p + j, *(u16 *)(p + 0x2) + i, -d3);
                qc++;
            }
            r += 0xA;
        }
        if ((*(u16 *)(p + 0x68) & 7) == 0)
        {
            func_800A56E0(0x818);
        }
        if (--*(s16 *)(p + 0x68) <= 0)
        {
            for (i = 0, r = p; i < 5; i++)
            {
                for (j = 0, qd = (s16 *)(r + 0x4); j < 5; j++)
                {
                    c4 = (u8 *)((((i + *(s16 *)(p + 0x2)) << wc[10]) + *(s16 *)p + j) * 6 + tc);
                    *(u16 *)(c4 + 0x2) = *(u16 *)qd;
                    func_800CDF40(*(u16 *)p + j, *(u16 *)(p + 0x2) + i, -*(u16 *)qd);
                    qd++;
                }
                r += 0xA;
            }
            func_8009D6F4();
            gc = (u8 *)&D_80083460;
            *(u16 *)(gc + 0xA) -= 1;
            if (*(s16 *)(p + 0x6E) == 0)
            {
                D_800E296C &= 0x7FF7FFFF;
            }
            else
            {
                D_800E296C &= 0xBFF7FFFF;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        func_800CDE0C();
    }
}

s32 func_800CE4E8(s32 x, s32 y, s32 ang, u8 *e, s32 f)
{
    UnkNode *n;
    u8 *p;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *c;
    u8 *d;
    u8 *g;
    s16 t;
    s32 s;
    s32 w;
    s32 a;
    s32 b;
    s16 v;
    s32 i;
    s32 j;

    n = func_8003FC64(2);
    t = f;
    if (n != NULL)
    {
        j = 0;
        s = t;
        p = (u8 *)n + 0x20;
        q = p;
        n->unk_10 = (u32)func_800CE028;
        *(s16 *)p = x - 2;
        *(s16 *)(p + 0x2) = y - 2;
        for (; j < 5; j++)
        {
            i = 0;
            r = q;
            u = q + 0x36;
            for (; i < 5; i++)
            {
                a = (i + *(s16 *)p) * 64;
                b = (j + *(s16 *)(p + 0x2)) * 64;
                v = func_800BCA68(a, b);
                d = (u8 *)((s32)r + 4);
                c = (u8 *)(i * 2 + (s32)d);
                *(s16 *)c = -v;
                w = (u16)func_800A6DA4(1, 3) << 5;
                *(s16 *)u = w;
                if (s != 0)
                {
                    *(s16 *)u = -w;
                }
                *(s16 *)u = *(u16 *)u + *(u16 *)c;
                u += 2;
                d = c;
            }
            q += 0xA;
        }
        *(s16 *)(p + 0x6C) = 0x10;
        *(s16 *)(p + 0x68) = 0x20;
        *(s16 *)(p + 0x6E) = t;
        func_800419EC(0x20, 8);
        if (*(s16 *)(p + 0x6E) == 0)
        {
            D_800E296C |= 0x80080000;
        }
        else
        {
            D_800E296C |= 0x40080000;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return (s32)n;
}

/* 21 words, 0 differing. */
s32 func_800CE69C(u8 *p)
{
    u8 *e;

    if (D_800E296C & 0xC0000000)
    {
        return 1;
    }
    e = *(u8 **)(p - 0x14);
    return func_800CE4E8(e[0x24], e[0x25], *(s16 *)(p + 0x88), p, 0) != 0;
}

/* THE `return 1` GUARD IS WRITTEN FIRST AND IS LAID OUT LAST. The other way
   round gcc emits the `return 1` as the branch's fall-through and pushes the
   body behind it: 22 words, 11 differing, every body instruction correct but
   two words late. 22 words, 0 differing. */
s32 func_800CE6F0(u8 *p)
{
    u8 *e;

    if (D_800E296C & 0xC0000000)
    {
        return 1;
    }
    e = *(u8 **)(p - 0x14);
    return func_800CE4E8(e[0x24], e[0x25], *(s16 *)(p + 0x88), p, 1) != 0;
}

/* The per-turn hunger/companion tick: two slots (+0x4C, +0x50), each rolled
   against the record's +0x03, each decrementing its own +0x02 counter and
   setting a bit in f2, then one combined message and the 0x615 effect.
   191 words, 0 differing.

   BLOCK 1'S FLAG IS `f2 = 1;` AT THE MERGE OF THE INNER IFS, NOT `f2 |= 1;`.
   That is where the target's TWO `addiu $s2, $zero, 0x1` come from: reorg
   steals the single merge insn into the `bnez (f1 & 1)` delay slot and
   redirects BOTH inner branches past the original. `f2 |= 1` emits
   `ori $s2, $s2, 0x1`, which reorg declines to steal, leaving a `nop` --
   190 words, 143 differing. Four hand-duplicated spellings (a dominating
   `f2 = 1` plus a copy in the innermost arm, the copy at the inner merge, an
   `|=` copy, an explicit `else`) are all 190/143 as well: cse deletes the
   second copy along the extended-basic-block path every time.

   `(c == 2 || c == 3)` MUST BE PARENTHESISED. Flat
   `c == 1 || c == 7 || c == 2 || c == 3` is left-associative, so
   `fold_range_test` never sees the pair and emits four `li`/`beq`: 2 words
   long, 9 differing.

   `u8 c` for the +0x00 kind byte is what emits the `andi $v1, $a0, 0xFF`
   while the range test goes on using the raw `lbu` register; `s32 c` is one
   word short. `s16 roll` is func_800CC058's lever verbatim -- as `s32` the
   mfhi result keeps its own register and the join vanishes, 6 differing.
   The two func_80099194 string picks are written out as a CALL PER ARM, not
   a ternary and not a `u8 *` local: only that gives each arm its own
   `lui $a0`/`addiu $a0` split, with cross-jumping merging the shared `jal`
   tail. Both other spellings are 190 words / 143 differing. */
s32 func_800CE748(u8 *p)
{
    u8 *b;
    u8 *q;
    void *h;
    void *m;
    s32 r;
    s16 roll;
    u8 c;
    s16 f1;
    s16 f2;

    f1 = f2 = 0;
    if (func_800C80F0(p) == 0)
    {
        b = *(u8 **)(p + 0x4C);
        if (b != NULL)
        {
            f1 = 1;
            if (b[1] == 0xF)
            {
                f1 = b[0] == 1;
            }
            if (D_800E3D40 == 0)
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
            }
            else
            {
                roll = 0;
            }
            if (roll < 0x40)
            {
                if ((f1 & 1) == 0)
                {
                    if ((s8)b[2] >= -0x62)
                    {
                        b[2]--;
                    }
                }
                f2 = 1;
            }
        }
        b = *(u8 **)(p + 0x50);
        if (b != NULL)
        {
            c = b[0];
            if (c == 1 || c == 7 || (c == 2 || c == 3))
            {
                f1 |= 2;
            }
            if (D_800E3D40 == 0)
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
            }
            else
            {
                roll = 0;
            }
            if (roll < 0x40)
            {
                if ((f1 & 2) == 0)
                {
                    if ((s8)b[2] >= -0x62)
                    {
                        b[2]--;
                    }
                }
                f2 |= 2;
            }
        }
    }
    if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
    {
        if (f2 == 0)
        {
            func_800A6508();
        }
        else
        {
            h = func_800990FC();
            m = h;
            if (f2 & 1)
            {
                h = func_8009955C(*(u8 **)(p + 0x4C), h);
                if ((f1 & 1) == 0)
                {
                    h = func_80099194(D_800E1B76, h);
                }
                else
                {
                    h = func_80099194(D_800E1B87, h);
                }
                if (f2 & 2)
                {
                    h = func_8009929C(0xA, h);
                }
            }
            if (f2 & 2)
            {
                h = func_8009955C(*(u8 **)(p + 0x50), h);
                if ((f1 & 2) == 0)
                {
                    h = func_80099194(D_800E1B99, h);
                }
                else
                {
                    h = func_80099194(D_800E1BAA, h);
                }
            }
            func_80099290(h);
            func_800A5720(m);
        }
    }
    if (f2 != 0)
    {
        q = *(u8 **)(p - 0x14);
        if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
        {
            func_800C5BBC((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0xC0C040, 0x20, 0);
            func_800A56E0(0x615);
        }
    }
    return 1;
}

/* 242 words, 0 differing.  A three-state machine on the halfword at `p + 0x6`.
   State 0 spends the D_800E3648 slot for room `*(s16 *)(p + 0xC)`: for each
   of its `*(s8 *)(e + 2)` charges it rolls a random pair out of D_80083460's
   +0x18 table, asks func_800A1618 for a template, then tries up to sixteen
   random tiles inside the D_800E2970 room record for one that func_8009A350
   reports with no 0xB700 flag, and spawns there through the function pointer
   func_800A0B94 RETURNS -- the `jalr $v0`, called with (0, (u8)a, (u8)c,
   -0x400).  It then drains the spawn's +0x18 budget through func_800A1D4C and
   stamps +0x9A/+0x8C/+0x1C/+0x92.  State 1 counts down +0x4 and plays the
   message; the default state counts down, clears the slot and sets the two
   0x8000 request bits.

   THE LAST TWO LEVERS, both register names with every instruction already in
   place, and both derived from the compiler rather than swept for -- all five
   repo sweeps (part/width/stmt/dev/ref) come back empty on the 3-row body AND
   on the 2-row body.

   (A) `c0 = func_800A6DA4(...); ... c = t[2] + c0;` -- THE SUM'S DESTINATION
   MUST NOT BE ONE OF ITS OPERANDS.  optabs.c:412-434 swaps a commutative
   optab's operands when
     ((target == 0 || REG_P (target))
        ? ((REG_P (op1) && ! REG_P (op0)) || target == op1)
        : rtx_equal_p (op1, target)) || GET_CODE (op0) == CONST_INT
   -- "try to make the first operand a register; even better, try to make it
   the same as the target".  With one accumulator `c`, `c = t[2] + c;` hits
   `target == op1` and expand emits `(plus c t[2])`, which is exactly what
   `c = c + t[2];` gives with no swap: so BOTH orders of one variable produce
   `addu $s0, $v0, $a0` and eight spellings all measured the same 3 rows.
   Giving the second func_800A6DA4 result its own local makes `target` a third
   pseudo, the swap does not fire, and the target's `addu $s0, $a0, $v0`
   appears.  3 rows -> 2.  (`c = c0 + t[2];` puts the operands back the wrong
   way round; the device must stay on `c`, not move to `c0` -- 22 rows.)

   (B) `j = *(u32 *)(o + 0x18) < tb[o[0x11]]; if (j == 0)` -- THE GUARD'S
   `sltu` DESTINATION IS A GLOBAL-ALLOC DECISION, AND A BLOCK-LOCAL CANNOT
   REACH $a0.  Written as a plain `if (tb[o[0x11]] <= *(u32 *)(o + 0x18))`
   the comparison result is a fresh block-local pseudo; block_alloc
   (local-alloc.c:1404) walks the operands 1..n calling
   combine_regs(operand[i], operand[0]) and takes the first that wins, and
   combine_regs (:1791) refuses operand 1 at its very first test because
   `j` is a global allocno (`reg_qty < 0`; local-alloc.c:469 needs
   reg_basic_block >= 0 and `j` is also read at the loop top).  The tie
   therefore falls to operand 2, the limit -- a block-local -- and the result
   takes ITS register: `sltu $v0, $a0, $v0`.  Untying it does not help, and
   neither does moving the value into a block-local of its own: MIPS defines
   no REG_ALLOC_ORDER, so find_free_reg hands an unsuggested block-local
   quantity $v0, then $v1, then $a0, and the store temp just swaps registers
   with `tb` (10 rows, `sltu $v1, $v1, $v0`).  docs/matching-levers.md,
   "BREAKING combine_regs' TIE IS NOT THE SAME AS MOVING THE REGISTER": to
   reach an $aN a value needs BOTH halves -- multi-block, so it is a
   global-alloc allocno at all, and conflicting with $v0.  Storing the
   comparison into `j` gives it both for free, because `j` already has them
   (`.greg`: `98 conflicts: ... 95 ... 2 29`, and `98 in 4`), and the one
   `sltu` then writes $a0 because its destination IS `j`.  2 rows -> 0.
   `j = (u32)j < tb[o[0x11]];` with `if (j == 0)` or `if (!j)` is the same
   object; `j = tb[o[0x11]] <= *(u32 *)(o + 0x18); if (j)` costs a word and 11
   rows (the `<=` needs an extra `xori`), and `a`/`c`/`x`/`n` in place of `j`
   measure 15/26/8/13 because only `j`'s allocno has the $v0 conflict.

   `j` IS DEAD AT BOTH REUSES: it is `func_800A6D30() & 0x1E` from the top of
   the same iteration and is reassigned every iteration, so using it first as
   the o+0x18 store temp and then as the guard's boolean is safe, and the
   stored value round-trips bit-exactly through s32.

   THE FRAME IS 0x48 BECAUSE `k` IS A SPILLED HImode PSEUDO.  `w` sits at 0x10
   and `k` at 0x18 with six bytes between them: `(s16)k` is a PARADOXICAL
   subreg (mips.md's extendhisi2 is `gen_lowpart (SImode, ...)`), so
   scan_paradoxical_subregs (reload1.c:3772) sets reg_max_ref_width[k] = 4 and
   alter_reg passes align == -1 to assign_stack_local, which rounds the slot
   up to 8 at BIGGEST_ALIGNMENT -- vars 16, frame 0x48.  That is also why the
   reload is `lhu` + `sll 16` + `sra 16` and both ends use $t0.  `(void)&k`
   makes `k` an ordinary addressable 2-byte local and loses all of it (~23
   rows of prologue/epilogue offsets).  AND IT ONLY SURVIVES WHILE
   `g = (u8 *)&D_80083460;` IS WRITTEN BEFORE THE func_800A6D30 CALL: with `g`
   written after, the symbol+0x18 folds to two words instead of three, the
   `while (n >= 0)` body gets small enough for loop.c:1730's
   `threshold * savings * m->lifetime >= insn_count` to flip, and
   `(s16)k * 0x14` is hoisted out of the loop, undoing the spill.  6 words.

   ONE LOCAL PER SITE for D_800E3648 (b/e, b2/e2, b3/e3) and D_80083460
   (g, g2): one variable is one pseudo spanning three basic blocks, which
   global_alloc gives a callee-saved register so the symbol's `addiu` writes
   $s0 at every site; the target materialises it into a dead temp each time.
   Shared: 81 rows.  Same finding as func_800CB170 above.

   `rm = D_800E2970; t = rm + k * 0x14;` -- the symbol is its own local for the
   same reason, 8 rows.  `tb` + `tb1` are two pseudos because the target has
   two: the `tb1 = tb` copy survives cse only inside the `if`, where the `do`
   body's label has two predecessors and resets cse's table.

   BOTH func_800A6DA4 CALLS BEFORE EITHER `t[...]` LOAD: a load cannot be
   scheduled across a call, so `a = call() + t[0];` reads t[0] between the two
   calls.  17 rows.  `(s16)x` at the three post-spawn calls folds to a plain
   move (num_sign_bit_copies on a `lbu` is 24, so combine's
   simplify_shift_const kills the sll/sra pair) and nothing after combine
   deletes copies; plain `x` is 1 row worse, `(u8)x` leaves an `andi`.

   THE ONE DEVICE: `c++; c--;` after `c = t[2] + c0;`, worth 7 rows at the
   same word count -- four more references to `c`, no instructions.  Without
   it the two `lbu`s and `addiu $a3, $sp, 0x10` schedule in the wrong order
   (0x800CEB68..0x800CEB78).  No honest statement was found that supplies
   those references. */
void func_800CEA44(u8 *p, UnkVec *v, u32 q)
{
    u8 *b;
    u8 *b2;
    u8 *b3;
    u8 *e2;
    u8 *e3;
    u8 *g2;
    u8 *e;
    u8 *t;
    u8 *rm;
    u8 *r;
    u8 *o;
    u8 *g;
    u32 *tb;
    u32 *tb1;
    s32 n;
    s32 j;
    s16 i;
    u16 w;
    s16 k;
    s32 a;
    s32 c;
    s32 c0;
    s32 x;
    s32 y;
    u8 *s;

    if (*(s16 *)(p + 0x6) == 0)
    {
        k = D_80082EA6;
        if (k >= 0)
        {
            b = D_800E3648;
            e = b + *(s16 *)(p + 0xC) * 4;
            n = *(s8 *)(e + 2);
            while (n >= 0)
            {
                if (func_8003FA44(3) == 0)
                {
                    b2 = D_800E3648;
                    e2 = b2 + *(s16 *)(p + 0xC) * 4;
                    e2[2] = e2[2] - n;
                    return;
                }
                g = (u8 *)&D_80083460;
                j = func_800A6D30();
                r = *(u8 **)(g + 0x18) + (j & 0x1E);
                x = r[0];
                y = r[1];
                s = func_800A1618(x, 1);
                if (s != NULL)
                {
                    i = 0xF;
                    rm = D_800E2970;
                    t = rm + k * 0x14;
                    while (1)
                    {
                        a = func_800A6DA4(0, (u16)(*(u16 *)(t + 4) - 1));
                        c0 = func_800A6DA4(0, (u16)(*(u16 *)(t + 6) - 1));
                        a = a + t[0];
                        c = t[2] + c0;
                        c++;
                        c--;
                        if (func_8009A350((u8)a - 1, (u8)c, 0, &w) != 0 && (w & 0xB700) == 0)
                        {
                            break;
                        }
                        i--;
                        if (i < 0)
                        {
                            break;
                        }
                    }
                    if (i >= 0)
                    {
                        D_800E2968 = 1;
                        o = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94((s16)x, s, 1))(
                            0, (u8)a, (u8)c, -0x400);
                        if (o != NULL)
                        {
                            func_800A152C((s16)x, 1);
                            func_80042640((Unk42710 *)o, (s16)x);
                            o[0x43] = func_800A6D30();
                            tb = D_800835E4;
                            o[0x12] = 0;
                            j = tb[y];
                            *(u32 *)(o + 0x18) = j;
                            tb++;
                            j = *(u32 *)(o + 0x18) < tb[o[0x11]];
                            if (j == 0)
                            {
                                tb1 = tb;
                                do
                                {
                                    func_800A1D4C(o, 0);
                                } while (tb1[o[0x11]] <= *(u32 *)(o + 0x18));
                            }
                            o[0x9A] = 0x16;
                            *(u32 *)(o + 0x1C) &= 0xF7FBFFFF;
                            *(u32 *)(o + 0x8C) = 0;
                            *(u16 *)(o + 0x92) -= 0x200;
                        }
                    }
                }
                n--;
            }
        }
        *(s16 *)(p + 0x4) = 0xA;
        *(u16 *)(p + 0x6) += 1;
    }
    else if (*(s16 *)(p + 0x6) == 1)
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_800997FC(D_800E1C3A);
            func_800419EC(0xC, 0x18);
            func_800A56E0(0x602);
            *(s16 *)(p + 0x4) = 0x10;
            *(u16 *)(p + 0x6) += 1;
        }
    }
    else
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_8004437C(0, 1);
            func_80053DA8(0x200);
            b3 = D_800E3648;
            e3 = b3 + *(s16 *)(p + 0xC) * 4;
            g2 = (u8 *)&D_80083460;
            *(u32 *)e3 = 0;
            *(u16 *)(g2 + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Charge one of the 4-byte D_800E3648 slots and spawn the node that spends
   it. THE ROW POINTER IS `base + index`, NOT AN ARRAY REF: `&D_800E3648[i*4]`
   expands the symbol AFTER the index shift and commutes the `addu`, 5
   differing. The node must also be a `u8 *` with cast stores: as `UnkNode *n`
   the COMPONENT_REF lets sched1 hoist the unrelated D_80083460 load above it,
   13 differing. 58 words, 0 differing. */
s32 func_800CEE0C(u8 *p, s16 i)
{
    u8 *n;
    u8 *g;
    u8 *b;
    u8 *e;

    if (p[0x13] != 0)
    {
        return 1;
    }
    g = (u8 *)&D_80083460;
    b = D_800E3648;
    e = b + i * 4;
    if ((s16)(*(u16 *)(g + 0x1C) + *(s8 *)(e + 2)) >= 0x21)
    {
        *(s8 *)(e + 2) = 0x20 - *(u8 *)(g + 0x1C);
    }
    if (func_8003FA44(*(s8 *)(e + 2) + 1) == 0)
    {
        return 0;
    }
    n = (u8 *)func_8003FC64(2);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800CEA44;
        *(s16 *)(n + 0x2C) = i;
        *(u16 *)(g + 0xA) += 1;
        func_80053DA8(0xB1);
    }
    return n != NULL;
}

/* Three stubs that return zero and nothing else. */
s32 func_800CEEF4(void)
{
    return 0;
}

/* Variant C: a five-argument visitor, the fifth a constant 0 passed on the
   stack, which is the whole reason this one's frame is 0x20 rather than
   0x18. 22 words, 0 differing. */
s32 func_800CEEFC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800CEFB8(p, v, s, s->unk_06, 0);
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

/* Variant C of the node walk again -- see func_800CEEFC above -- with the
   fifth argument 1 rather than 0. The constant has to live across the call,
   so this one saves $s1 and its frame is 0x28: 25 words against that one's
   22. 25 words, 0 differing. */
s32 func_800CEF54(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800CEFB8(p, v, s, s->unk_06, 1);
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

/* Emits one POLY_FT4 per 0xC-byte element for one slot, the same renderer as
   func_800D3738 over libgpu's 0x28-byte FLAT primitive instead of its
   0x34-byte Gouraud one, and with two RotTransPers calls instead of one: the
   renderable's +0x02/+0x06/+0x0A and +0x0E/+0x12/+0x16 points are both
   projected, the OT index is their mean and func_80065F90 (ratan2) of their
   screen delta is parked at the scratchpad's +0x10E as the sprite's bearing.
   The fifth argument picks the quad's shear: nonzero shears the top edge,
   zero adds that bearing into the view rotation.

   Unk453E0SP has no +0x10E and UnkDungeonGT4 is 0x34 bytes, hence the two new
   types above.

   SEVEN SPELLING FACTS, each measured on this function.

   (1) THE FIFTH PARAMETER IS `s16`, NOT THE `s32` THE FORWARD DECLARATION
   CARRIED.  It is tested twice as `sll 16` / `bnez`, and the two tests are in
   DIFFERENT registers because gcc copies the parameter's pseudo ($s4, which
   the element cursor then reuses) into $fp -- no source device is needed for
   that, `move $fp,$20` falls out of the plain parameter.  Both callers
   (func_800CEEFC, func_800CEF54) pass a literal and re-measure at 0 differing.

   (2) EACH ARM OF THE TWO BYTE-OFFSET IF/ELSES DUPLICATES ITS TAIL.  With
   `p->unk_88`/`p->unk_78` written once after the if/else, `jump.c`
   cross-jumps all FOUR stores into the join, the offset local needs a second
   register for the sum and the function is 588 words / 62 aligned
   differences; duplicated, the shared tail is the two stores the target has
   at .L800CF31C and the sum lands back in the offset's own register.

   (3) `e->unk_01` GOES THROUGH A LOCAL.  The `s->unk_0C[3]` store between the
   first read and the two patched ones invalidates cse's memory, so each arm
   reloads the byte: 4 words.

   (4) `g->unk_B8` NEEDS AN EXPLICIT `(s16)`.  Unk80083160 declares it `u16`
   -- correctly, func_800A7CC4 below reads the same field with `lhu` -- but
   here the target reads it with `lh`.  1 word.

   (5) THE SUBTRACTION IN `p->unk_104` READS `g->unk_C6`, NOT `p->unk_34.h`.
   Through the scratchpad field the halfword is reloaded and the two camera
   components swap registers; through `g->unk_C6` cse forwards the `lh` it
   just stored from.  2 words.  (func_800D3738 wants the other spelling at the
   same site; it has no conditional after it.)

   (6) THE FOUR SCRATCHPAD DECREMENTS ARE IN SOURCE ORDER B8, F0, BA, F2.
   All 24 permutations were measured at 587 words: B8/F0/BA/F2 is 5 aligned
   differences, the next best 9, the "natural" B8/BA/F0/F2 13 and the worst 21.

   (7) EACH ADDEND OF `p->unk_102` IS IN ITS OWN LOCAL, AND THE SECOND ONE
   REUSES THE LOCAL THAT HELD THE SECOND PROJECTION'S RETURN.  Written inline
   `fold` moves the `- 0x100` onto the masked value (`addiu ...,-0x100` after
   the `andi`) and that is the last differing word; a fresh local for the
   second addend is 4, `h` for both is 1, and `z` is 0.  The same split is why
   `p->unk_10.w - 1 + p->unk_08.w` is two statements: inline, fold rewrites it
   as `p->unk_10.w + (p->unk_08.w - 1)` and the two `lw` come out swapped.

   587 words, 0 differing. */
void func_800CEFB8(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3, s16 arg4)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSP8 *p = (UnkDungeonSP8 *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    UnkDungeonFT4 *q;
    Unk44D24E *e;
    s32 y;
    s32 h;
    s32 z;
    s32 u;

    p->unk_EC = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    p->unk_20 = base + 0xB0;
    q = *(UnkDungeonFT4 **)(base + 0x8D0);
    s->unk_14 |= 0x8000;
    p->unk_00 = ((u16 *)v)[1];
    p->unk_02 = ((u16 *)v)[3];
    p->unk_04 = ((u16 *)v)[5];
    p->unk_C0 = RotTransPers((void *)0x1F800000, (void *)0x1F8000B8, (void *)0x1F800090,
                             (void *)0x1F800094);
    p->unk_00 = ((u16 *)v)[7];
    p->unk_02 = ((u16 *)v)[9];
    p->unk_04 = ((u16 *)v)[11];
    z = RotTransPers((void *)0x1F800000, (void *)0x1F8000F0, (void *)0x1F800090,
                     (void *)0x1F800094);
    p->unk_C0 = (p->unk_C0 + z) >> 1;
    p->unk_10E = func_80065F90((s16)p->unk_F2 - (s16)p->unk_BA, (s16)p->unk_F0 - (s16)p->unk_B8);
    D_8006CD30.unk_1C = p->unk_C0 * 4;
    p->unk_C0 = p->unk_C0 - arg3;
    if ((u32)p->unk_C0 < 0x1E0)
    {
        PushMatrix();
        p->unk_B8 = p->unk_B8 - 0xA0;
        p->unk_F0 = p->unk_F0 - 0xA0;
        p->unk_BA = p->unk_BA - 0x78;
        p->unk_F2 = p->unk_F2 - 0x78;
        p->unk_30.w = g->unk_C4;
        p->unk_34.w = g->unk_C6;
        p->unk_38.w = g->unk_C8;
        p->unk_100 = s->unk_16;
        y = (s->unk_1A - g->unk_C6) + (s16)g->unk_B8;
        if (arg4 == 0)
        {
            y += (s16)p->unk_10E;
        }
        p->unk_104 = y;
        h = (p->unk_38.h + 0x100) & 0x1FF;
        z = s->unk_18 - 0x100;
        p->unk_102 = h + z;
        p->unk_108 = p->unk_E4 = s->unk_20;
        p->unk_10A = p->unk_E8 = s->unk_22;
        RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
        p->unk_30.w = s->unk_1C;
        p->unk_34.w = s->unk_1E;
        p->unk_38.w = 0x1000;
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
                u8 t;
                u8 c;

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
                RotTransPers4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &q->unk_08,
                              &q->unk_10, &q->unk_18, &q->unk_20, &p->unk_90, &p->unk_94);
                if (arg4 != 0)
                {
                    if ((s16)p->unk_BA < (s16)p->unk_F2)
                    {
                        q->unk_08 += p->unk_B8;
                        q->unk_10 += p->unk_B8;
                        q->unk_0A += p->unk_BA;
                        q->unk_12 += p->unk_BA;
                        q->unk_18 += p->unk_F0;
                        q->unk_20 += p->unk_F0;
                        q->unk_1A += p->unk_F2;
                        q->unk_22 += p->unk_F2;
                    }
                    else
                    {
                        q->unk_18 += p->unk_B8;
                        q->unk_20 += p->unk_B8;
                        q->unk_1A += p->unk_BA;
                        q->unk_22 += p->unk_BA;
                        q->unk_08 += p->unk_F0;
                        q->unk_10 += p->unk_F0;
                        q->unk_0A += p->unk_F2;
                        q->unk_12 += p->unk_F2;
                    }
                }
                else
                {
                    q->unk_18 += p->unk_B8;
                    q->unk_1A += p->unk_BA;
                    q->unk_10 += p->unk_F0;
                    q->unk_12 += p->unk_F2;
                    q->unk_08 += p->unk_B8;
                    q->unk_0A += p->unk_BA;
                    q->unk_20 += p->unk_F0;
                    q->unk_22 += p->unk_F2;
                }
                if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                     ((u16)(q->unk_10 + 0x20) < 0x181 && (u16)(q->unk_12 + 0x20) < 0x121) |
                     ((u16)(q->unk_18 + 0x20) < 0x181 && (u16)(q->unk_1A + 0x20) < 0x121) |
                     ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121)) != 0)
                {
                    setlen(q, 9);
                    s->unk_14 &= 0x7FFF;
                    u = p->unk_10.w - 1;
                    p->unk_10.w = u + p->unk_08.w;
                    if (p->unk_10.w & 0x100)
                    {
                        p->unk_10.w = p->unk_10.w - 1;
                    }
                    u = p->unk_14.w - 1;
                    p->unk_14.w = u + p->unk_0C.w;
                    if (p->unk_14.w & 0x100)
                    {
                        p->unk_14.w = p->unk_14.w - 1;
                    }
                    p->unk_14.w = p->unk_14.w << 8;
                    p->unk_0C.w = p->unk_0C.w << 8;
                    if (s->unk_12 != 0)
                    {
                        if (p->unk_24 & 0x100)
                        {
                            q->unk_0E = s->unk_12;
                        }
                        else
                        {
                            q->unk_0E = s->unk_12 + e->unk_06;
                        }
                    }
                    else
                    {
                        q->unk_0E = e->unk_06;
                    }
                    q->unk_0C.h = p->unk_0C.h + p->unk_08.h;
                    q->unk_14.h = p->unk_0C.h + p->unk_10.h;
                    if (s->unk_10 != 0)
                    {
                        q->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
                    }
                    else
                    {
                        q->unk_16 = e->unk_04;
                    }
                    q->unk_1C.h = p->unk_14.h | p->unk_08.h;
                    q->unk_24.h = p->unk_14.h | p->unk_10.h;
                    if ((s16)q->unk_08 > (s16)q->unk_20)
                    {
                        t = q->unk_24.b[0];
                        q->unk_24.b[0] = t + 0xFF;
                        q->unk_14.b[0] = t;
                    }
                    if ((s16)q->unk_0A > (s16)q->unk_22)
                    {
                        t = q->unk_24.b[1];
                        q->unk_24.b[1] = t + 0xFF;
                        q->unk_1C.b[1] = t;
                    }
                    c = e->unk_01;
                    s->unk_0C[3] = c;
                    if (p->unk_24 & 8)
                    {
                        if (p->unk_24 & 4)
                        {
                            s->unk_0C[3] = c | 2;
                        }
                        else
                        {
                            s->unk_0C[3] = c & 0xFD;
                        }
                    }
                    q->unk_04 = *(u32 *)s->unk_0C;
                    AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                    q++;
                }
            }
            if ((s8)e->unk_00 < 0)
            {
                break;
            }
            e++;
        }
        PopMatrix();
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

void func_800CF8E4(void)
{
    UnkDungeonCF8E4SP *sp = (UnkDungeonCF8E4SP *)0x1F800000;
    Unk80083160 *g;
    UnkDungeonCF8E4Map *m;
    UnkDungeonCF8E4Cell *cell;
    UnkDungeonCF8E4Vtx *vt;
    UnkDungeonCF8E4Nrm *nm;
    UnkDungeonCF8E4Obj *o;
    UnkDungeonCF8E4View *v;
    POLY_FT4 *p;
    LINE_F4 *l;
    SVECTOR buf[6];
    u8 *q;
    u8 *z;
    u8 *z2;
    s32 i;
    s32 t;
    s32 vi0, vi1, vi2;
    u16 h1;
    s32 one;
    s32 h;
    s32 a, b, c, d;
    u32 w;
    u16 x, y;
    u16 cf;

    g = &D_80083160;
    v = (UnkDungeonCF8E4View *)((u8 *)g + 0x18);
    m = (UnkDungeonCF8E4Map *)((u8 *)g + 0x1DC);
    cell = (UnkDungeonCF8E4Cell *)g->unk_1DC;
    vt = m->unk_08;
    nm = m->unk_0C;
    if ((D_80013714 & 2) != 0)
    {
        return;
    }
    t = (s16)func_800BCB04(v->unk_A4, v->unk_A6, v->unk_A8 - 0x20);
    if (t < 0x201)
    {
        sp->unk_158 = t;
    }
    else
    {
        sp->unk_158 = 0;
    }
    SetColorMatrix((MATRIX *)((u8 *)v + 0x58));
    func_80064624(v->unk_84, v->unk_88);
    SetLightMatrix((MATRIX *)((u8 *)v + 0x38));
    *(s16 *)((u8 *)g + 0x1E) = 0x1BA;
    func_80046884((RECT *)v, buf, sp->unk_158);
    if (D_800DCF58 != 0)
    {
        D_800DCF58 = 0;
        v->unk_28 = v->unk_08 = buf[0].vx;
        v->unk_0A = buf[0].vy;
        v->unk_2C = v->unk_0C = buf[0].vz;
        v->unk_10 = v->unk_10;
        v->unk_12 = v->unk_12;
        v->unk_14 = v->unk_14;
        v->unk_18 = v->unk_18;
        v->unk_1A = v->unk_1A;
        v->unk_1C = v->unk_1C;
        v->unk_20 = v->unk_20;
        v->unk_22 = v->unk_22;
        v->unk_24 = v->unk_24;
    }
    else
    {
        v->unk_08 += (buf[0].vx - v->unk_08) >> 1;
        v->unk_0A += (buf[0].vy - v->unk_0A) >> 1;
        v->unk_0C += (buf[0].vz - v->unk_0C) >> 1;
        v->unk_10 += (buf[1].vx - v->unk_10) >> 1;
        v->unk_12 += (buf[1].vy - v->unk_12) >> 1;
        v->unk_14 += (buf[1].vz - v->unk_14) >> 1;
        v->unk_18 += (buf[2].vx - v->unk_18) >> 1;
        v->unk_1A += (buf[2].vy - v->unk_1A) >> 1;
        v->unk_1C += (buf[2].vz - v->unk_1C) >> 1;
        v->unk_20 += (buf[3].vx - v->unk_20) >> 1;
        v->unk_22 += (buf[3].vy - v->unk_22) >> 1;
        v->unk_24 += (buf[3].vz - v->unk_24) >> 1;
        v->unk_28 = v->unk_08;
        v->unk_2A = v->unk_0A;
        v->unk_2C = v->unk_0C;
    }
    sp->unk_00 = v->unk_90;
    q = (u8 *)0x80010000;
    z = g->unk_00;
    a = m->unk_14;
    b = m->unk_16;
    c = m->unk_18;
    d = m->unk_1A;
    w = *(u32 *)(q + 0x3180);
    x = q[0x3184];
    y = q[0x3185];
    sp->unk_BC = (u32 *)(z + 0xB8);
    sp->unk_124 = a;
    sp->unk_178 = y;
    sp->unk_114 = 0x40 << sp->unk_124;
    sp->unk_128 = b;
    sp->unk_11C = c;
    sp->unk_120 = d;
    sp->unk_110 = w;
    sp->unk_176 = x;
    sp->unk_118 = 0x40 << sp->unk_128;
    z2 = g->unk_00;
    p = (POLY_FT4 *)*(u32 *)(z2 + 0x8D0);
    sp->unk_17C = (u32)z2 + 0xAF3A;
    sp->unk_14C = 0xFFFF;
    sp->unk_174 = 4;
    sp->unk_0C = (s16)func_80046C20((Unk46C20A *)&v->unk_08, sp->unk_1C, &sp->unk_174);
    while (sp->unk_174 != 0)
    {
        one = 1;
        h = 0x7FFF;
        for (i = 3; i >= 0; i--)
        {
            if (sp->unk_1C[i].unk_1C == 0 && sp->unk_1C[i].unk_18 <= sp->unk_0C)
            {
                sp->unk_1C[i].unk_1C = one;
            }
        }
        sp->unk_14 = h;
        sp->unk_18 = -0x7FFF;
        for (i = 3; i >= 0; i--)
        {
            if (sp->unk_1C[i].unk_1C > 0)
            {
                sp->unk_08 = sp->unk_1C[i].unk_00;
                if (sp->unk_08 < sp->unk_14)
                {
                    sp->unk_14 = sp->unk_08;
                }
                else if (sp->unk_08 > sp->unk_18)
                {
                    sp->unk_18 = sp->unk_08;
                }
                sp->unk_1C[i].unk_10 += sp->unk_1C[i].unk_08;
                while (sp->unk_1C[i].unk_10 >= 0)
                {
                    sp->unk_1C[i].unk_00 += sp->unk_1C[i].unk_14;
                    sp->unk_1C[i].unk_10 -= sp->unk_1C[i].unk_04;
                    sp->unk_1C[i].unk_20 -= 0x40;
                    if (sp->unk_1C[i].unk_20 <= 0)
                    {
                        break;
                    }
                }
                sp->unk_08 = sp->unk_1C[i].unk_00;
                if (sp->unk_08 < sp->unk_14)
                {
                    sp->unk_14 = sp->unk_08;
                }
                if (sp->unk_08 > sp->unk_18)
                {
                    sp->unk_18 = sp->unk_08;
                }
            }
        }
        if (sp->unk_0C >= 0 && sp->unk_0C <= sp->unk_118)
        {
            sp->unk_14 = (sp->unk_14 - 0x20) & -0x40;
            if (sp->unk_14 < 0)
            {
                sp->unk_14 = 0;
            }
            sp->unk_18 = (sp->unk_18 + 0x20) & -0x40;
            if (sp->unk_18 > sp->unk_114)
            {
                sp->unk_18 = sp->unk_114;
            }
            sp->unk_138 = (((u32)sp->unk_0C >> 6) & sp->unk_120) << sp->unk_124;
            sp->unk_134 = ((u32)sp->unk_14 >> 6) & sp->unk_11C;
            while (sp->unk_14 <= sp->unk_18)
            {
                sp->unk_144 = sp->unk_138 + sp->unk_134;
                if (cell[sp->unk_144].unk_00 != 0)
                {
                    sp->unk_12C = cell[sp->unk_144].unk_02;
                    o = m->unk_04[cell[sp->unk_144].unk_00];
                    for (;;)
                    {
                        sp->unk_170 = o->unk_10;
                        vi0 = o->unk_00;
                        sp->unk_16C = o->unk_14;
                        sp->unk_164 = vt[vi0].unk_00;
                        sp->unk_E0.unk_04 = vt[o->unk_00].unk_04 - (u16)sp->unk_12C;
                        vi1 = o->unk_02;
                        sp->unk_E0.unk_00 = (u16)((u16)sp->unk_14 + (u16)sp->unk_164) |
                                            ((sp->unk_0C + (s16)(*(u16 *)((u8 *)sp + 0x166))) << 16);
                        sp->unk_164 = vt[vi1].unk_00;
                        h1 = (u16)sp->unk_14;
                        sp->unk_E8.unk_04 = vt[o->unk_02].unk_04 - (u16)sp->unk_12C;
                        vi2 = o->unk_04;
                        sp->unk_E8.unk_00 = (u16)(h1 + (u16)sp->unk_164) |
                                            ((sp->unk_0C + (s16)(*(u16 *)((u8 *)sp + 0x166))) << 16);
                        sp->unk_164 = vt[vi2].unk_00;
                        sp->unk_F0.unk_00 = (u16)((u16)sp->unk_14 + (u16)sp->unk_164) |
                                            ((sp->unk_0C + (s16)(*(u16 *)((u8 *)sp + 0x166))) << 16);
                        sp->unk_F0.unk_04 = vt[o->unk_04].unk_04 - (u16)sp->unk_12C;
                        gte_ldv3(&sp->unk_E0, &sp->unk_E8, &sp->unk_F0);
                        sp->unk_F8.unk_04 = vt[o->unk_06].unk_04 - (u16)sp->unk_12C;
                        gte_rtpt_b();
                        gte_nclip();
                        gte_stopz(&sp->unk_DC);
                        if (sp->unk_DC < 0)
                        {
                            goto backFace;
                        }
                        gte_stflg(&sp->unk_D8);
                        if (sp->unk_D8 != 0)
                        {
                            goto cellDone;
                        }
                        gte_stsxy3_ft4(p);
                        gte_avsz3();
                        gte_stotz(&sp->unk_C8);
                        t = (u16)sp->unk_170;
                        if (sp->unk_14C != t)
                        {
                            sp->unk_14C = t;
                            if (nm[t].unk_04 < 0)
                            {
                                sp->unk_148 = h;
                                goto doLight;
                            }
                            if ((nm[t].unk_00 & 0xFFF0FFF) != 0)
                            {
                                sp->unk_148 = h;
                                goto doLight;
                            }
                            sp->unk_10 = nm[t].unk_00;
                            if ((s16)(*(u16 *)((u8 *)sp + 0x10)) > 0)
                            {
                                *(s16 *)((u8 *)sp + 0x10) = one;
                            }
                            else if ((s16)(*(u16 *)((u8 *)sp + 0x10)) < 0)
                            {
                                *(s16 *)((u8 *)sp + 0x10) = -1;
                            }
                            if ((s16)(*(u16 *)((u8 *)sp + 0x12)) > 0)
                            {
                                *(s16 *)((u8 *)sp + 0x12) = one << sp->unk_124;
                            }
                            else if ((s16)(*(u16 *)((u8 *)sp + 0x12)) < 0)
                            {
                                *(s16 *)((u8 *)sp + 0x12) = -(one << sp->unk_124);
                            }
                            sp->unk_13C =
                                (sp->unk_134 + (s16)(*(u16 *)((u8 *)sp + 0x10))) & sp->unk_11C;
                            sp->unk_13C += (sp->unk_138 + (s16)(*(u16 *)((u8 *)sp + 0x12))) &
                                           (sp->unk_120 << sp->unk_124);
                            cf = cell[sp->unk_13C].unk_04;
                            if ((cf & 1) != 0)
                            {
                                sp->unk_148 = h;
                                goto doLight;
                            }
                            if ((cf & 0x80) != 0)
                            {
                                if ((cell[sp->unk_144].unk_04 & 0x80) == 0)
                                {
                                    sp->unk_148 = h;
                                    goto doLight;
                                }
                                if (((*(u8 *)((u8 *)sp + 0x16F)) & 2) != 0)
                                {
                                    sp->unk_148 = h;
                                    goto doLight;
                                }
                                if ((*(u8 *)((u8 *)sp + 0x16E)) == one && (s8)(*(u8 *)((u8 *)sp + 0x16F)) < 0)
                                {
                                    goto cellDone;
                                }
                                o++;
                                sp->unk_14C = 0xFFFF;
                                continue;
                            }
                            if ((cf & 0x40) != 0)
                            {
                                goto setBig;
                            }
                            sp->unk_148 = -cell[sp->unk_13C].unk_02;
                            goto doLight;
                        setBig:
                            sp->unk_148 = h;
                        doLight:
                            gte_ldrgb(&sp->unk_00);
                            gte_ldv0(&nm[sp->unk_14C]);
                            gte_nccs();
                            gte_strgb(&sp->unk_04);
                        }
                        if ((s16)sp->unk_E0.unk_04 >= sp->unk_148 &&
                            (s16)sp->unk_E8.unk_04 >= sp->unk_148 &&
                            (s16)sp->unk_F0.unk_04 >= sp->unk_148 &&
                            (s16)sp->unk_F8.unk_04 >= sp->unk_148)
                        {
                            if ((s8)(*(u8 *)((u8 *)sp + 0x16F)) < 0)
                            {
                                goto cellDone;
                            }
                            o += (*(u8 *)((u8 *)sp + 0x16E)) & 0xF;
                            continue;
                        }
                        sp->unk_164 = vt[o->unk_06].unk_00;
                        sp->unk_F8.unk_00 = (u16)((u16)sp->unk_14 + (u16)sp->unk_164) |
                                            ((sp->unk_0C + (s16)(*(u16 *)((u8 *)sp + 0x166))) << 16);
                        gte_ldv0(&sp->unk_F8);
                        *(u32 *)&p->u0 = o->unk_08;
                        gte_rtps_b();
                        *(u32 *)&p->u1 = o->unk_0C;
                        *(u16 *)&p->u2 = *(u16 *)((u8 *)sp + 0x172);
                        *(u16 *)&p->u3 = (u16)sp->unk_16C;
                        gte_stsxy(&p->x3);
                        gte_stszotz(&sp->unk_D4);
                        sp->unk_C8 = (sp->unk_C8 + sp->unk_C8 * 2 + sp->unk_D4) >> 2;
                        if ((u32)sp->unk_C8 < 0x1BE)
                        {
                            setlen(p, 9);
                            *(u32 *)&p->r0 = sp->unk_04;
                            if (((*(u8 *)((u8 *)sp + 0x16F)) & 1) != 0)
                            {
                                p->code |= 2;
                            }
                            else if ((u16)sp->unk_178 != 0 &&
                                     (cell[sp->unk_144].unk_04 & 0x80) != 0 &&
                                     ((*(u8 *)((u8 *)sp + 0x16F)) & 2) == 0 &&
                                     ((s16)sp->unk_E0.unk_04 < sp->unk_158 ||
                                      (s16)sp->unk_E8.unk_04 < sp->unk_158 ||
                                      (s16)sp->unk_F0.unk_04 < sp->unk_158 ||
                                      (s16)sp->unk_F8.unk_04 < sp->unk_158))
                            {
                                l = (LINE_F4 *)p;
                                *(u32 *)&l->x1 = *(u32 *)&p->x1;
                                *(u32 *)&l->x2 = *(u32 *)&p->x2;
                                *(u32 *)&l->x3 = *(u32 *)&p->x3;
                                *(u32 *)&p->r0 = sp->unk_110;
                                setlen(p, 5);
                                addPrim(&sp->unk_BC[sp->unk_C8], p);
                                p++;
                                setlen(p, one);
                                *(u32 *)&p->r0 = ((u16)sp->unk_176 & 0x9FF) | 0xE1000000;
                            }
                            addPrim(&sp->unk_BC[sp->unk_C8], p);
                            p++;
                        }
                        else
                        {
                            if ((u32)sp->unk_C8 >= 0x1DE)
                            {
                                goto cellDone;
                            }
                            setlen(p, 9);
                            *(u32 *)&p->r0 = sp->unk_04;
                            p->code |= 2;
                            addPrim(&sp->unk_BC[sp->unk_C8], p);
                            p++;
                        }
                        if (((*(u8 *)((u8 *)sp + 0x16E)) & 0xF) != one)
                        {
                            o++;
                            continue;
                        }
                        if ((s8)(*(u8 *)((u8 *)sp + 0x16F)) < 0)
                        {
                            goto cellDone;
                        }
                        o += ((*(u8 *)((u8 *)sp + 0x16E)) >> 4) + 1;
                        continue;
                    backFace:
                        if (nm[(u16)sp->unk_170].unk_04 < 0)
                        {
                            if ((*(u16 *)((u8 *)sp + 0x16E) & 0x80FF) == 0x8001)
                            {
                                goto cellDone;
                            }
                            o++;
                            continue;
                        }
                        if (((*(u8 *)((u8 *)sp + 0x16E)) & 0xF0) == 0 && (s8)(*(u8 *)((u8 *)sp + 0x16F)) < 0)
                        {
                            goto cellDone;
                        }
                        o += (*(u8 *)((u8 *)sp + 0x16E)) & 0xF;
                    }
                cellDone:
                    if (sp->unk_17C < (u32)p)
                    {
                        goto allDone;
                    }
                }
                sp->unk_134++;
                sp->unk_134 &= sp->unk_11C;
                sp->unk_14 += 0x40;
            }
        }
        sp->unk_0C += 0x40;
        for (i = 3; i >= 0; i--)
        {
            if (sp->unk_1C[i].unk_1C > 0)
            {
                sp->unk_1C[i].unk_0C -= 0x40;
                if (sp->unk_1C[i].unk_0C < -0x7F)
                {
                    sp->unk_1C[i].unk_1C = -1;
                    sp->unk_174--;
                }
            }
        }
    }
allDone:
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)p;
}

s32 func_800D0774(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800D07C8(p, v, s, s->unk_06);
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

void func_800D07C8(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3)
{
    Unk80083160 *g = &D_80083160;
    Unk453E0SP *p = (Unk453E0SP *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    UnkDungeonGT4 *q;
    Unk44D24E *e;
    u8 m[0x20];

    p->unk_20 = base + 0xB0;
    p->unk_E4 = ((s16 *)v)[1];
    p->unk_E8 = ((s16 *)v)[3];
    p->unk_EC = ((s16 *)v)[5];
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    q = *(UnkDungeonGT4 **)(base + 0x8D0);
    s->unk_14 |= 0x8000;
    PushMatrix();
    p->unk_108 = s->unk_20;
    p->unk_10A = s->unk_22;
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38.w = 0;
    p->unk_100 = s->unk_16;
    p->unk_104 = s->unk_1A;
    p->unk_102 = s->unk_18;
    RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
    ReadRotMatrix(m);
    CompMatrix(m, (void *)0x1F8000D0, (void *)0x1F800050);
    ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
    SetTransMatrix((void *)0x1F800050);
    SetRotMatrix((void *)0x1F800050);
    e = (Unk44D24E *)s->unk_08;
    p->unk_24 = s->unk_14;
    for (;;)
    {
        if ((e->unk_00 & 0x20) == 0)
        {
            s16 a;

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
            p->unk_C0 = RotAverage4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88, &q->unk_08,
                                    &q->unk_14, &q->unk_20, &q->unk_2C, &p->unk_90, &p->unk_94) -
                        arg3 - 6;
            if ((u32)p->unk_C0 < 0x1E0 &&
                ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                  ((u16)(q->unk_14 + 0x20) < 0x181 && (u16)(q->unk_16 + 0x20) < 0x121) |
                  ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121) |
                  ((u16)(q->unk_2C + 0x20) < 0x181 && (u16)(q->unk_2E + 0x20) < 0x121)) != 0))
            {
                u32 col;
                u8 f;

                s->unk_14 &= 0x7FFF;
                p->unk_10.w = p->unk_10.w + p->unk_08.w;
                if (p->unk_10.w & 0x100)
                {
                    p->unk_10.w = p->unk_10.w - 1;
                }
                p->unk_14.w = p->unk_14.w + p->unk_0C.w;
                if (p->unk_14.w & 0x100)
                {
                    p->unk_14.w = p->unk_14.w - 1;
                }
                p->unk_0C.w = p->unk_0C.w << 8;
                p->unk_14.w = p->unk_14.w << 8;
                if (p->unk_24 & 0x100)
                {
                    q->unk_0E = s->unk_12;
                }
                else
                {
                    q->unk_0E = s->unk_12 + e->unk_06;
                }
                q->unk_0C = p->unk_0C.h + p->unk_08.h;
                q->unk_18.h = p->unk_0C.h + p->unk_10.h;
                q->unk_1A = (s->unk_10 != 0) ? s->unk_10 + (e->unk_04 & 0xFF9F) : e->unk_04;
                q->unk_24.h = p->unk_14.h + p->unk_08.h;
                q->unk_30.h = p->unk_14.h + p->unk_10.h;
                if ((s16)q->unk_08 > (s16)q->unk_2C)
                {
                    q->unk_18.b[0] = q->unk_18.b[0] - 1;
                    q->unk_30.b[0] = q->unk_30.b[0] - 1;
                }
                if ((s16)q->unk_0A > (s16)q->unk_2E)
                {
                    q->unk_24.b[1] = q->unk_24.b[1] - 1;
                    q->unk_30.b[1] = q->unk_30.b[1] - 1;
                }
                col = *(u32 *)s->unk_0C;
                q->unk_28 = 0;
                q->unk_1C = 0;
                setlen(q, 0xC);
                q->unk_10 = col;
                q->unk_04 = col;
                f = e->unk_01;
                setcode(q, f);
                if (p->unk_24 & 8)
                {
                    setcode(q, (p->unk_24 & 4) ? (f | 2) : (f & 0xFD));
                }
                AddPrim(p->unk_20 + p->unk_C0 * 4, q);
                q++;
            }
        }
        else if (*(u32 *)&e->unk_08 != 0)
        {
            (*(void (**)(u8 *, UnkVec *, UnkSlot *, Unk44D24E *))&e->unk_08)(arg0, v, s, e);
        }
        if ((s8)e->unk_00 < 0)
        {
            break;
        }
        e++;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

s16 func_800D0DE0(s16 arg0, s16 arg1, s16 arg2)
{
    u8 *g;
    UnkDungeonTile *p;
    s16 dirA;
    s16 dirB;
    s16 xA;
    s16 yA;
    s16 xB;
    s16 yB;
    u16 cntA;
    u16 cntB;
    s16 blockedA;
    s16 blockedB;
    s32 i;

    g = (u8 *)D_8008333C;
    blockedA = blockedB = 0;
    dirA = (arg0 + 2) & 7;
    dirB = (arg0 - 2) & 7;
    xA = arg1 + D_8006CCD8[arg0];
    yA = arg2 + D_8006CCE8[arg0];
    xB = xA + D_8006CCD8[dirB];
    yB = yA + D_8006CCE8[dirB];
    cntA = cntB = 0;

    for (i = 0; i < 10; i++)
    {
        if (xA < 0 || xA >= (1 << *(s16 *)(g + 0x14)))
        {
            blockedA = 1;
        }
        if (yA < 0 || yA >= (1 << *(s16 *)(g + 0x16)))
        {
            blockedA = 1;
        }
        if (blockedA == 0)
        {
            p = &D_800EA000[xA + (yA << *(s16 *)(g + 0x14))];
            if (p->unk_04 & 4)
            {
                cntA++;
                p->unk_04 |= 0x80;
                func_800D169C((u16)xA, (u16)yA, (s16)func_800D175C(xA, yA), p->unk_04 & 8);
                xA += D_8006CCD8[dirA];
                yA += D_8006CCE8[dirA];
            }
            else
            {
                blockedA = 1;
            }
        }
        if (xB < 0 || xB >= (1 << *(s16 *)(g + 0x14)))
        {
            blockedB = 1;
        }
        if (yB < 0 || yB >= (1 << *(s16 *)(g + 0x16)))
        {
            blockedB = 1;
        }
        if (blockedB == 0)
        {
            p = &D_800EA000[xB + (yB << *(s16 *)(g + 0x14))];
            if (p->unk_04 & 4)
            {
                cntB++;
                p->unk_04 |= 0x80;
                func_800D169C((u16)xB, (u16)yB, (s16)func_800D175C(xB, yB), p->unk_04 & 8);
                xB += D_8006CCD8[dirB];
                yB += D_8006CCE8[dirB];
            }
            else
            {
                blockedB = 1;
            }
        }
    }
    return cntA + cntB;
}

/* Walk up to eleven cells along facing `d`, starting one step BEHIND (x, y),
   and count the ones whose map flags have bit 2 set; stop early when
   func_800D0DE0 reports 2 or more, and stop as soon as the walk leaves the
   map. Returns 0 outright when the first stepped-back cell is already off the
   map, which is why the two entry gates jump past the `sra` that narrows the
   count.

   THREE SPELLING FACTS, each measured on this function.

   (1) `xx`/`yy` ARE `s16` AND THAT IS FOUR WORDS. The running coordinates are
   HImode locals, so every update is `addu $v0, $sN, $v0` plus a mode-change
   copy `addu $sN, $v0, $zero`, and the `(s16)` the tests need is then folded
   onto the SImode result in $v0 rather than reloaded. As `s32` with explicit
   `(s16)` casts at the tests the four copies vanish: 127 words of 131.

   (2) D_8008333C MUST BE A `u8 *` POINTER LOCAL. Written as the inline
   subscript `((s16 *)D_8008333C)[10]` the shared base comes out as
   `SYM + 0x14` with displacements 0 and 2, rematerialised twice more in the
   loop -- 131 words, 109 differing, and `$fp` goes to `&D_8006CCD8[d]`
   instead. A NAMED `s16 *sh` local is worse still (132 words, 95 differing):
   it carries a REG_EQUIV constant and reload substitutes the address at all
   four uses. The `u8 *` base with `*(s16 *)(g + 0x14)` is the one spelling
   that keeps the symbol itself in one callee-saved register.

   (3) THE THREE IN-LOOP EXITS ARE `break`, NOT `return cnt`. With `return
   cnt` inside the loop, loop.c sees `cnt << 16` used in the body and
   strength-reduces it into a second induction variable (`lui $v0, 0x1` /
   `addu $s2, $s2, $v0`): 137 words, 136 differing. With `break` the narrowing
   lives in the single post-loop `return` and reorg copies the `sll` into each
   exit branch's delay slot, which is the target's four duplicated `sll`s
   against one shared `sra`.

   131 words, 0 differing. */
s16 func_800D112C(s16 d, s32 x, s32 y)
{
    u8 *g;
    s32 cnt;
    s32 n;
    s16 xx;
    s16 yy;
    u16 w;

    n = 11;
    cnt = 0;
    xx = x - ((u16 *)D_8006CCD8)[d];
    g = (u8 *)D_8008333C;
    if (xx < 0 || xx >= (1 << *(s16 *)(g + 0x14)))
    {
        return 0;
    }
    yy = y - ((u16 *)D_8006CCE8)[d];
    if (yy < 0 || yy >= (1 << *(s16 *)(g + 0x16)))
    {
        return 0;
    }
    do
    {
        if ((s16)func_8009A350(xx, yy, d, &w) != 0 && (w & 4) != 0)
        {
            cnt++;
            if ((s16)func_800D0DE0(d, xx, yy) >= 2)
            {
                break;
            }
        }
        xx += ((u16 *)D_8006CCD8)[d];
        if (xx < 0 || xx >= (1 << *(s16 *)(g + 0x14)))
        {
            break;
        }
        yy += ((u16 *)D_8006CCE8)[d];
        if (yy < 0 || yy >= (1 << *(s16 *)(g + 0x16)))
        {
            break;
        }
    } while (--n > 0);
    return cnt;
}

/* Clear bit 0x80 of each listed cell's +0x04 halfword and empty the list.
   The cell address is the same `x + (y << D_8008333C[10])` index into the
   stride-6 map that func_8009A350 computes. The `while` with its test at the
   top is what duplicates the `lbu 0x0($a1)` into the entry path. 32 words,
   0 differing. */
void func_800D1338(void)
{
    s16 *sh;
    u8 *p;

    p = (u8 *)D_80081508;
    sh = (s16 *)D_8008333C;
    while (p[0] != 0)
    {
        D_800EA000[p[0] + (p[1] << sh[10])].unk_04 &= 0xFF7F;
        p += 4;
    }
    D_80080B04 = 0;
    *(u8 *)D_80081508 = 0;
}

void func_800D13B8(void)
{
    u8 *g;
    u8 *e;
    s32 t;
    s16 k;
    s16 x;
    s16 y;

    g = (u8 *)&D_80083160;
    e = g + 0x18;
    if (func_80044724() == 0)
    {
        return;
    }
    t = 2 - ((*(s16 *)(g + 0xC8) + 0x100) >> 9);
    func_800D1338();
    k = t & 7;
    x = *(u16 *)(e + 0xA4) >> 6;
    y = *(u16 *)(e + 0xA6) >> 6;
    if ((t & 1) != 0)
    {
        func_800D112C((k + 1) & 7, x, y);
        func_800D112C((k - 1) & 7, x, y);
        func_800D112C((k + 3) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        func_800D112C((k - 3) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
    }
    else
    {
        func_800D112C(k, x, y);
        func_800D112C((k + 2) & 7, x, y);
        func_800D112C((k - 2) & 7, x, y);
        func_800D112C((k + 4) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        func_800D112C((k + 2) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        func_800D112C((k - 2) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
    }
}

/* 21 words, 0 differing. */
void func_800D1648(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    func_8004491C(n, (u32)func_800D1824);
    D_80081508 = (u32)((u8 *)n + 0x20);
    *((u8 *)n + 0x20) = 0;
    D_80080B04 = 0;
}

/* 48 words, 0 differing. D_80080B04 is read with `lh` for the bound and the
   index and with `lhu` for the `++`, off TWO separate `lui`s -- cse restarts
   at the if/else join, so the second materialisation is free rather than a
   second local. */
void func_800D169C(u16 x, u16 y, s16 v, s16 flag)
{
    u8 *e;

    if (D_80080B04 >= 0x40)
    {
        return;
    }
    if ((s16)func_800448BC(x, y) != 0)
    {
        return;
    }
    e = (u8 *)D_80081508 + D_80080B04 * 4;
    e[0] = x;
    e[1] = y;
    if (flag != 0)
    {
        *(s16 *)(e + 2) = v;
    }
    else
    {
        *(s16 *)(e + 2) = -0x8000;
    }
    D_80080B04++;
    e[4] = 0;
}

/* The vertical offset of the tile at (x, y).

   ONE BASE FOR BOTH ENGINE GLOBALS. D_80083178 is reached as
   `(u8 *)D_8008333C - 0x1C4`, which is the single `lui`/`addiu` plus one
   `addiu` the target has, and the map-width shift at D_80083350 is then the
   +0x14 displacement off the same register. Naming D_80083350 and
   &D_80083178 separately is the same 50 words with two `lui`s: 17 differing.

   AND THE VALUE THE 0x3F IS ADDED TO MUST BE ONE VARIABLE SPANNING BOTH ARMS.
   `d += 0x3F` in place is 4 differing; a separate sum variable with the else
   arm's load written inline is 2, and a named local for the load does not
   help because gcc ties the arithmetic result to its dying source. Assigning
   the load back into `d` -- the pseudo that already crosses the branch --
   unties it. 50 words, 0 differing. */
s16 func_800D175C(s16 x, s16 y)
{
    s32 s;
    u16 w;
    s32 d;
    s32 e;
    u8 *g;
    u8 *q;

    g = (u8 *)D_8008333C;
    q = g - 0x1C4;
    s = *(s16 *)(g + 0x14);
    w = D_800EA000_s[(y << s) + x].unk_02;
    d = func_800BCB04(*(u16 *)(q + 0xA4), *(u16 *)(q + 0xA6), (s16)(*(u16 *)(q + 0xA8) - 0x20));
    if ((s16)d < 0x201)
    {
        e = d + 0x3F;
    }
    else
    {
        d = *(u16 *)(D_800814A8 + 0x88);
        e = d + 0x3F;
    }
    return ((w + e) & ~0x3F) - w;
}

/* The tile-strip billboard pass: for each 4-byte cell in the 0-terminated
   list at `p`, park a 0x40-unit quad in the 0x1F800000 scratchpad, project it
   with RTPT, back-face-cull it with NCLIP, depth-cue it with AVSZ3 plus a
   second RTPS on the fourth corner, and emit one POLY_FT4 from the +0x1E0[4]
   descriptor onto D_80083160's +0xB0 OT row. 151 words, 0 differing.

   THE ONE SPELLING THAT CARRIES IT IS THE PROLOGUE'S MEMORY ORDER. The entry
   block's 23 insns are the whole residue of every other attempt: sched2 runs
   them all at priority 1, so the ready list is the RTL order and the only
   constraint is the memory dependences. The target's order is
   [sw sp+0x28][lw unk_00][lw +0x1E0][lw +0x10][lw +0x8D0][sh sp+0x2C]
   [sw sp+0x20], which is exactly `sp+0x28 store, f, q, sp+0x2C store,
   sp->unk_20 store` in source -- the 0x28 store is a PREDECESSOR of the four
   loads and the 0x2C store a SUCCESSOR, and that is what makes the loads sit
   where they do and the parameter copy fill a load-delay slot at word 16.
   With both scratchpad stores ahead of the loads it is 151 / 17.

   AND IT ONLY WORKS THROUGH `u8 *` CASTS. With `Unk80083160 *g` and
   `D_80083160.unk_00` the same statement order hoists the `lui`/`lw` pair
   ABOVE the stack adjust and the function is 151 / 36; through
   `*(u8 **)&D_80083160` and `*(u8 **)(g + 0x1E0)` the hoist does not happen.
   All 120 permutations of the five prologue statements were measured in both
   spellings: the cast form has exactly two at 0 and the struct form none
   below 17.

   `sp->unk_C0 += sp->unk_C0 * 2;` not `* 3`: `x * 3` puts the shift in $v0
   and the load in $v1, which is 2 differing. The fourth corner's vz is stored
   AFTER gte_ldv3, which only loads corners 0..2 (10 differing before it). */
void func_800D1824(u8 *p)
{
    u8 *g = (u8 *)&D_80083160;
    Unk463ECSP *sp = (Unk463ECSP *)0x1F800000;
    u8 *f;
    u8 *q;
    s32 a;
    s32 b;

    *(s32 *)((u8 *)sp + 0x28) = 0;
    f = *(u8 **)(*(u8 **)(g + 0x1E0) + 0x10);
    q = *(u8 **)(*(u8 **)&D_80083160 + 0x8D0);
    *(s16 *)((u8 *)sp + 0x2C) = -0x1000;
    sp->unk_20 = (u32)(*(u8 **)&D_80083160 + 0xB0);
    while (p[0] != 0)
    {
        if (*(u16 *)(p + 2) != 0x8000)
        {
            a = p[0] << 6;
            sp->unk_70[0].vx = sp->unk_70[2].vx = a;
            sp->unk_70[1].vx = sp->unk_70[3].vx = a + 0x40;
            b = p[1] << 6;
            sp->unk_70[0].vy = sp->unk_70[1].vy = b;
            sp->unk_70[2].vy = sp->unk_70[3].vy = b + 0x40;
            sp->unk_70[0].vz = sp->unk_70[1].vz = sp->unk_70[2].vz = *(u16 *)(p + 2);
            gte_ldv3(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2]);
            sp->unk_70[3].vz = *(u16 *)(p + 2);
            gte_rtpt_b();
            gte_nclip();
            gte_stopz(&sp->unk_114);
            if (sp->unk_114 > 0)
            {
                gte_stsxy3_ft4(q);
                gte_avsz3();
                gte_stotz(&sp->unk_C0);
                gte_ldv0(&sp->unk_70[3]);
                sp->unk_C0 += sp->unk_C0 * 2;
                gte_rtps_b();
                gte_stszotz((u8 *)sp + 0xCC);
                sp->unk_C0 = (sp->unk_C0 + *(s32 *)((u8 *)sp + 0xCC)) >> 2;
                if ((u32)sp->unk_C0 < 0x1E0)
                {
                    gte_stsxy(q + 0x20);
                    func_80065034((u8 *)sp + 0x28, g + 0xA8, q + 0x4);
                    *(u32 *)(q + 0xC) = *(u32 *)(f + 0x8);
                    *(u32 *)(q + 0x14) = *(u32 *)(f + 0xC);
                    *(u16 *)(q + 0x1C) = *(u16 *)(f + 0x12);
                    *(u16 *)(q + 0x24) = *(u16 *)(f + 0x14);
                    q[3] = 9;
                    AddPrim((u8 *)(sp->unk_20 + sp->unk_C0 * 4), q);
                    q += 0x28;
                }
            }
        }
        p += 4;
    }
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)q;
}

/* State machine for the fade-to-white transition, driven off the actor's
   +0x9B: state 0 arms it (unless D_8008346A says a transition is already
   running), state 1 drives the three colour bytes at +0x0C..+0x0E towards
   0x20 over the +0x96 countdown and, when the countdown runs out or the
   renderable raises its 0x8000 flag, drops the engine's +0x10 slot tag,
   closes the node and reports 1.

   TWO SPELLINGS ARE LOAD-BEARING, both measured here:
   (1) IT IS A `switch`, NOT AN if/else CHAIN. The two tests have to come
       out `beqz` to the case-0 body and `beq 1` to the case-1 body with the
       default falling through to a bare `j`; an
       `if (x == 0) {...} else if (x != 1) return 0;` chain inverts the first
       test and loses the `j`'s `nop` -- 123 words against 124, 112
       differing. With the switch it is 124 / 38.
   (2) THE COUNTDOWN GATE IS ONE `||` WITH THE BODY INSIDE IT, not an early
       return. Written `if (--a->unk_96 > 0 && (c->unk_14 & 0x8000) == 0)
       { return 0; }` the zeroing is laid out inline after the test and the
       function's single `return 0` exit at the bottom disappears: 124 words,
       38 differing. Written as above -- the whole closing block inside
       `if (--a->unk_96 <= 0 || ...)` and `return 0;` as the tail -- it is 0.

   THE DEFINITION IS OLD-STYLE ON PURPOSE. Three landed callers
   (func_800D2464 and its two copies) call it with NO arguments, riding the
   unit's `s32 func_800D1A80();` declaration because their own $a0..$a3 are
   already the ones this function wants; a prototyped definition above them
   turns those three call sites into "too few arguments". Same decision
   csrc/ovl_town.c's func_8009C7A8 records. 124 words, 0 differing. */
s32 func_800D1A80(a, b, c, d)
UnkDungeonActor *a;
void *b;
u8 *c;
u8 *d;
{
    u8 *g;

    switch (a->unk_9B)
    {
    case 0:
        if (D_8008346A != 0)
        {
            return 0;
        }
        *(s16 *)(c + 0x10) = 0x20;
        *(s16 *)(c + 0x12) -= 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u32 *)(d + 0x1C) |= 0x10000000;
        func_800A56E0(0x805);
        *(u32 *)(c + 0xC) = 0x808080;
        a->unk_96 = 0x10;
        a->unk_9B++;
        /* fall through */
    case 1:
        break;
    default:
        return 0;
    }
    c[0xC] += (0x20 - c[0xC]) / a->unk_96;
    c[0xD] += (0x20 - c[0xD]) / a->unk_96;
    c[0xE] += (0x20 - c[0xE]) / a->unk_96;
    if (--a->unk_96 <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return 1;
    }
    return 0;
}

/* THE CLUSTER'S SPAWNER, and the third shape in the set: func_800D24A8 and
   func_800D3F74 below are this source with the template, the scale and the
   two hooks substituted. Allocate a 0x112 node out of D_80083498, tag it
   0x34, take the engine's D_80045340 list, seed the mover's facing and the
   renderer's colour, scale and tile, park func_800D1E34 as the node's step
   and func_800D20D8 in the record's +0x8C slot, then measure the leash with
   func_800BCB04 and light the record up. Returns the node's payload, or
   NULL when the allocation fails -- one exit, with `p` preset to NULL, which
   is what the early `addu $s2, $zero, $zero` is.

   ALL FOUR PARAMETERS ARE s16 AND THAT IS WORTH ELEVEN WORDS. Only `kind`
   is witnessed directly -- `sll $v0, $s0, 16` / `sra` / `bne $v0, 1` is a
   narrow parameter re-extended, which an `s32` would not need. The other
   three are invisible in their own instructions: `sb $s6, 0x24($s0)` and
   `sh $s4, 0xA($s1)` truncate whatever they are given. They show up in the
   PROLOGUE instead. As `s32`, the tile pair's two allocnos come out at
   2 refs / 72 and 74 insns, the x pseudo wins `allocno_compare` by 3%, the
   two take each other's callee-saved registers, and the whole save-and-copy
   sequence reschedules around them: 13 differing words, none of them in the
   body. As `s16` they land the right way round and the function is 0. A
   parameter whose only use is a narrowing store still has a declared type,
   and the prologue is where you read it.

   113 words, 0 differing. */
u8 *func_800D1C70(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1800;
        r->unk_1C = 0x1800;
        func_8003DB94(r, D_800E2178, 0);
        n->unk_10 = (u32)func_800D1E34;
        *(u32 *)(p + 0x8C) = (u32)func_800D20D8;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* THE CLUSTER'S PER-FRAME STEP DRIVER, and the entry point the four machines
   above are reached through. Three copies: func_800D2E9C is this source with
   D_800E2284 for D_800E21C0 and nothing else changed, and func_800D2664 is
   it again with D_800E2228 and WITHOUT the func_800478B8 guard -- 160 words
   against 169.

   A frame for one entity: bail out with the record's +0x71 top bit cleared
   while the engine's global 0x2000 flag is up; run the entity's own +0x8C
   hook if it has one and then its state handler out of the table, both with
   the same four arguments the installed machines take; integrate the mover
   (+0x00 += +0x0C, +0x04 += +0x10) and its +0x14 accumulator; then decide
   whether the entity is still inside its leash, which is what the two
   func_800BCB04 calls measure -- the first from the mover's own position,
   the second from the tile centre once the +0x40000000 request bit is set.
   Out of range, or with +0x98 bit 2 up, the +0x8000000 bit is dropped.

   THE RECORD IS REACHED THROUGH TWO POINTERS and this is not a cosmetic
   choice: `addu $s0, $s1, $zero` makes a second pseudo for the same address,
   and the fields are split between them -- +0x1C, +0x71, +0x88, +0x8C and
   +0x9A on the copy, +0x90, +0x92, +0x98 and +0x9D on the parameter. Both
   survive to allocation and both take a callee-saved register. The copy is
   also what the handler calls pass as their FIRST and FOURTH arguments, which
   is why func_800D21CC above takes the same record twice.

   The two tests before func_800478B8 are NESTED `if`s, not `&&`. Written with
   `&&`, fold merges `(x & 0x8000) == 0 && (x & 0x40) == 0` into a single
   `andi $v0, $v0, 0x8040` and the function comes out two words short; the
   target keeps one `lhu` and two `andi`/`bnez` pairs, which is what two
   statements give.

   169 words, 0 differing. */
void func_800D1E34(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E21C0[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* THREE BYTE-IDENTICAL COPIES -- func_800D28E4 and func_800D3140 differ from
   this one in nothing but their label -- and the handler the four machines
   above park in the actor's +0x8C slot once their glide finishes.

   It runs the record's +0x6D countdown down: while it is positive, clear
   +0x60, let func_800A9A0C act on the record, and if the counter started
   non-zero and has since been zeroed, decrement the +0x28 population byte.
   When that reaches zero the record is published at D_80083470 (biased by
   -0x20 to the node, the same bias the walk family uses) and handed to
   func_800AAA54; otherwise, if +0x64 is non-zero, to func_800AA6B4.

   THE STORE COMES FIRST AND THE COUNTER IS READ SECOND, and the order is
   worth four words. Written the other way round -- the counter read, then
   the store -- cse still holds the `lb $v0, 0x6D($s1)` the guard above
   loaded, combine folds the `lbu`/`sll 24`/`sra 24` triple back into that
   one `lb`, and the function comes out 57 words. With the store between
   them the load is reissued as the target has it. `s16 n = (s8)d[0x6D]` is
   both casts: the `(s8)` is the `sll 24`/`sra 24` pair and the `s16`
   declaration is the `sll 16` before the `beqz`, which is how an HImode
   compare against zero is spelled. Five statement orders measured; only
   this one reaches 0, and the nearest two sit at 4 differing.

   61 words each, 0 differing. */
void func_800D20D8(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* FOUR IDENTICAL COPIES, one per entity, differing in exactly one symbol:
   the handler parked in the actor's +0x8C slot at the end -- func_800D20D8,
   func_800D28E4, func_800D3140 and func_800D4494 respectively. Every other
   word of all four listings is the same.

   The machine is a step hook, switched on the actor's +0x9B state. State 0
   seeds the mover's two velocity words from the engine's sine/cosine tables
   at the entity's facing (+0x6A, top three bits -- the same index expression
   csrc/ovl_town.c uses) and either arms an eight-step glide or, when the
   record's +0x14 bit 15 is set, jumps straight to state 2 with a zero count.
   State 1 bleeds a quarter off each velocity per step. State 2 steers the
   mover towards the record's +0x24/+0x25 tile centre, dividing the remaining
   distance by the steps left, and on the last step snaps it there, clears
   the engine's +0x10 slot tag if it points at this node, and hands the
   actor to its next handler.

   Three spellings to note:

   - `case 0` FALLS THROUGH into `case 1`, and its `d[0x28] == 0` arm reaches
     case 1's tail with a `goto`. Both are in the listing: the fallthrough is
     the absent `j` after `sh $v0, 0x96($s0)`, and the `goto` is
     `beqz $v0, .L800D2334` branching from case 0 into the middle of case 1.
   - `d` is the node PAYLOAD pointer, not the node: the engine's slot holds
     `d - 0x20`, which is the same +0x20 bias the walk family uses.
   - D_80083460 is reached through a `u8 *` local, which is what gives the
     split `lui`/`addiu` and a `lw`/`sw` pair off one register rather than a
     `%lo` displacement; csrc/unk_154BC.c spells the same object the same way.

   TWO THINGS COST THE WHOLE SESSION'S MEASUREMENT, and both are now levers in
   docs/matching-levers.md.

   The `do { } while (0)` around the last store is NOT decoration. `a` and `b`
   are both live across calls, so both are global-alloc allocnos, and
   `allocno_compare`'s priority put them 2% apart the wrong way:
   `a` = 19 refs / 226 insns = 0.3363 against `b` = 16 / 186 = 0.3441, so `b`
   sorted first and took $s0. That is 36 of the 40 differing words -- every
   `$s0`/`$s1` in the function swapped -- at the exact word count. The
   degenerate loop weights the reference inside it by two, `a` reaches 20
   refs and 0.3540, and the swap goes away. Solve for the count before
   spelling anything: `floor_log2(R) * R / 226 > 0.3441` first holds at
   R = 20, so exactly ONE extra weighted reference is wanted and two would be
   no better. Eleven structural rewrites -- nested tails, split decrements,
   `u8 *` parameters in place of the typed ones, local aliases, reordered
   stores -- all left both numbers at 19/226 and 16/186 exactly, which is
   what sent this to the compiler's own dumps.

   The first division is in THREE statements and the second in two, and the
   asymmetry is measured, not an accident. Written as one expression,
   `X - (b02 - 0x20)`, fold's `associate` (fold-const.c:4183, reached through
   `split_tree` on the second operand) moves the constant to the other side
   and emits `addiu $v0, $v0, 32` where the target has `addiu $v1, $v1,
   -0x20`: one word. Written with the difference in its own local, fold is
   defeated but sched1 then emits the b-side load first and the pair at
   0x800D23A4 comes out swapped: two words. Only `goal -= now;` as its own
   statement gets both -- it keeps the constant on the b-term AND puts the
   `subu` in a statement of its own, which is what orders the two loads.
   Giving the second division the same three-statement shape costs a word
   (167 and 54 differing), and ten other spellings of the first were measured
   in between.

   166 words each, 0 differing. */
void func_800D21CC(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
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
        a->unk_8C = func_800D20D8;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* One gate, two arms, twice over: the same source compiled at two
   addresses -- the only word that differs between the pair is the `j` to its
   own epilogue. */
void func_800D2464(void *a, void *b, u8 *c)
{
    if (func_800D1A80() != 0)
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0;
    }
}

/* func_800D1C70 with D_800E221C stored straight into the renderer's +0x08
   instead of handed to func_8003DB94, a 0x1400 scale, and func_800D2664 /
   func_800D28E4 as the two hooks. 111 words, 0 differing. */
u8 *func_800D24A8(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        r->unk_08 = (u32)D_800E221C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1400;
        r->unk_1C = 0x1400;
        n->unk_10 = (u32)func_800D2664;
        *(u32 *)(p + 0x8C) = (u32)func_800D28E4;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E2228 and no func_800478B8 guard -- the entity
   this one drives is never handed to it. Same source otherwise, and the
   nine words it loses are why the two parameters land in the other order:
   fewer references to the slot record moves it past the mover in
   `allocno_compare`. 160 words, 0 differing. */
void func_800D2664(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E2228[a[0x9A]](a, b, c, a);
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 again, byte for byte. */
void func_800D28E4(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D29D8(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
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
        a->unk_8C = func_800D28E4;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

void func_800D2C70(void *a, void *b, u8 *c)
{
    if (func_800D1A80() != 0)
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0;
    }
}

/* The cluster's FOURTH spawner, and the one that shows the list tag is a
   FUNCTION: func_8004491C takes func_800D36D0 where the other three pass
   D_80045340, so this entity's nodes are walked by the guarded walk below
   rather than by the engine's own list. Otherwise func_800D1C70 with the
   D_800DEF08 template, no colour bytes, a different scale pair, three more
   renderer flags, func_800D2E9C / func_800D3140 as hooks, a plain 0x808080
   colour word and a zeroed +0x9E. 122 words, 0 differing. */
u8 *func_800D2CB4(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)func_800D36D0);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        func_8003DB94(r, D_800DEF08, 0);
        r->unk_12 = 0x7E02;
        r->unk_1C = 0x1800;
        r->unk_1E = 0x2000;
        r->unk_14 |= 0xC;
        r->unk_10 |= 0x20;
        r->unk_14 |= 0x100;
        n->unk_10 = (u32)func_800D2E9C;
        *(u32 *)(p + 0x8C) = (u32)func_800D3140;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(s16 *)(p + 0x9E) = 0;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E2284 for D_800E21C0; see it for the whole
   mechanism. 169 words, 0 differing. */
void func_800D2E9C(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E2284[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 again, byte for byte. */
void func_800D3140(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D3234(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
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
        a->unk_8C = func_800D3140;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* A two-state fade-out: state 0 raises the record's 0x10000000 bit, asks
   func_800A56E0 for sound 0x805 and arms a sixteen-step ramp, and state 1
   walks the renderer's three colour bytes towards 0x20 and its two scales
   towards zero, a step's worth of the remaining distance each frame. On
   the last step -- or at once if the renderer's 0x8000 flag is up -- it
   releases the engine's +0x10 slot, retires the record and rings
   func_800A56C0.

   The 0x20 the three colour terms are measured against is materialised
   ONCE, in the dispatch's `beq` delay slot, and again at the end of state
   0 -- one constant reaching the fallthrough and the direct entry. State 0
   falls through into state 1, as in func_800D21CC. 129 words, 0
   differing. */
void func_800D34CC(UnkDungeonActor *a, UnkVec *b, UnkSlot *r, u8 *d)
{
    u8 *g;

    switch (a->unk_9B)
    {
    case 0:
        if (D_8008346A != 0)
        {
            return;
        }
        *(u32 *)(d + 0x1C) |= 0x10000000;
        func_800A56E0(0x805);
        *(u32 *)((u8 *)r + 0xC) = 0x808080;
        a->unk_96 = 0x10;
        a->unk_9B++;
        /* fallthrough */
    case 1:
        *((u8 *)r + 0xC) += (0x20 - *((u8 *)r + 0xC)) / a->unk_96;
        *((u8 *)r + 0xD) += (0x20 - *((u8 *)r + 0xD)) / a->unk_96;
        *((u8 *)r + 0xE) += (0x20 - *((u8 *)r + 0xE)) / a->unk_96;
        r->unk_1C -= r->unk_1C / a->unk_96;
        r->unk_1E -= r->unk_1E / a->unk_96;
        if (--a->unk_96 > 0 && !(r->unk_14 & 0x8000))
        {
            return;
        }
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                          (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_800A56C0();
        return;
    }
}

/* The node walk func_800D2CB4 hands to func_8004491C as its list tag: the
   same four-member walk as func_800BC054 above, with the visitor skipped
   for any node whose renderer carries the 0x80 flag. 26 words, 0
   differing. */
s32 func_800D36D0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        if (!(s->unk_14 & 0x80))
        {
            func_800D3738(p, v, s, s->unk_06);
        }
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

/* Emits one POLY_GT4 per 0xC-byte element for one slot: the resident
   func_800453E0 (csrc/gprel_18B40.c) over a 0x34-byte Gouraud primitive
   instead of its 0x28-byte flat one, with the quad's TOP edge widened by
   half.  Unk453E0SP, Unk44D24E and Unk453E0T are that function's own types
   and carry over unchanged.

   TWO SPELLING FACTS, and each was worth the whole match.

   (1) THE ELEMENT'S x AND y BYTES GO THROUGH AN `s16` LOCAL AND THE
   WIDENING IS `a = a + a / 2;` WITH NO CAST.  With `s32 a` and
   `a + (s16)a / 2` all six `(s8)e->unk_02`/`unk_03` reads come out as a
   single `lb` instead of the target's `lbu` + `sll 24` + `sra 24`, and the
   function is 514 words / 102 aligned differences.  The HImode local is
   what forces the pair: the sign extension is of a QImode value already in
   a register, because the `a + a / 2` store is HImode and combine rewrites
   the extension as shifts rather than deleting it.  `volatile` on the byte
   read (docs/matching-levers.md) produces the same triple and is the WRONG
   cure here -- loop.c reverts the DEST_ADDR giv substitution on a volatile
   MEM (`validate_change` cannot re-recognise an insn holding one), so the
   six reads come off the biv at `2($s2)` instead of the e+4 giv at
   `-0x2($s2)`, the three callee-saved registers permute, and it is 527
   words but 91 aligned differences.

   (2) THE SECOND COLOUR WORD IS STORED THROUGH A `u8 *` CAST.  Written
   `q->unk_28 = *(u32 *)s->unk_0C;` the store is an in-struct MEM, its
   pseudo outlives the `(sp->unk_24 & 0xC) == 0xC` gate, it lands in $a0
   instead of $v1, and reorg then steals it into the `bne`'s delay slot
   where the target has `lui $a0, 0xFF` -- 527 words, 8 aligned differences.
   The cast kills the last eight.  Also measured on that tail, all at
   527 words: `1C, 28, setlen, setcode` 8; `1C, 28, setcode, setlen` 11;
   `setlen, setcode, 1C, 28` 19; `28, 1C, setlen, setcode` 9; the zero
   stores swapped 10; a `u32` local for the second store, `0xC == (x & 0xC)`
   and `*(u32 *)&p->unk_24` all compiled to the identical program.

   527 words, 0 differing. */
void func_800D3738(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3)
{
    Unk80083160 *g = &D_80083160;
    Unk453E0SP *p = (Unk453E0SP *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    UnkDungeonGT4 *q;
    Unk44D24E *e;
    s32 w;
    s32 h;

    p->unk_EC = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    p->unk_20 = base + 0xB0;
    p->unk_00 = ((u16 *)v)[1];
    p->unk_02 = ((u16 *)v)[3];
    p->unk_04 = ((u16 *)v)[5];
    q = *(UnkDungeonGT4 **)(base + 0x8D0);
    p->unk_C0 = RotTransPers((void *)0x1F800000, (void *)0x1F8000B8, (void *)0x1F800090,
                             (void *)0x1F800094);
    s->unk_14 |= 0x8000;
    w = p->unk_C0 - 0xA;
    D_8006CD30.unk_1C = p->unk_C0 * 4;
    p->unk_C0 = w - arg3;
    if ((u32)p->unk_C0 < 0x1D6)
    {
        PushMatrix();
        p->unk_B8 = p->unk_B8 - 0xA0;
        p->unk_BA = p->unk_BA - 0x78;
        p->unk_30.w = g->unk_C4;
        p->unk_34.w = g->unk_C6;
        p->unk_38.w = g->unk_C8;
        p->unk_100 = s->unk_16;
        p->unk_104 = g->unk_B8 + (s->unk_1A - p->unk_34.h);
        h = s->unk_18 - 0x100;
        p->unk_102 = h + ((p->unk_38.h + 0x100) & 0x1FF);
        p->unk_108 = p->unk_E4 = s->unk_20;
        p->unk_10A = p->unk_E8 = s->unk_22;
        RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
        p->unk_30.w = s->unk_1C;
        p->unk_34.w = s->unk_1E;
        p->unk_38.w = 0x1000;
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
                s32 x;
                s32 y;
                s16 a;

                x = e->unk_08;
                p->unk_08.w = x;
                y = e->unk_0A;
                p->unk_10.w = y;
                if (x + y >= 0x100 || s->unk_1A != 0)
                {
                    p->unk_10.w = y - 1;
                }
                x = e->unk_09;
                p->unk_0C.w = x;
                y = e->unk_0B;
                p->unk_14.w = y;
                if (x + y >= 0x100 || s->unk_1A != 0)
                {
                    p->unk_14.w = y - 1;
                }
                if ((e->unk_00 ^ p->unk_24) & 1)
                {
                    a = -(s8)e->unk_02 - p->unk_108;
                    a = a + a / 2;
                    p->unk_70 = a;
                    a = a - (p->unk_10.h + p->unk_10.w / 2);
                    p->unk_78 = a;
                    a = -(s8)e->unk_02 - p->unk_108;
                    p->unk_80 = a;
                    a = a - p->unk_10.h;
                    p->unk_88 = a;
                }
                else
                {
                    a = (s8)e->unk_02 - p->unk_108;
                    a = a + a / 2;
                    p->unk_70 = a;
                    a = a + (p->unk_10.h + p->unk_10.w / 2);
                    p->unk_78 = a;
                    a = (s8)e->unk_02 - p->unk_108;
                    p->unk_80 = a;
                    a = a + p->unk_10.h;
                    p->unk_88 = a;
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
                q->unk_08 = p->unk_F0 + p->unk_B8;
                q->unk_0A = p->unk_F2 + p->unk_BA;
                q->unk_14 = p->unk_F4 + p->unk_B8;
                q->unk_16 = p->unk_F6 + p->unk_BA;
                q->unk_20 = p->unk_F8 + p->unk_B8;
                q->unk_22 = p->unk_FA + p->unk_BA;
                q->unk_2C = p->unk_FC + p->unk_B8;
                q->unk_2E = p->unk_FE + p->unk_BA;
                if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                     ((u16)(q->unk_14 + 0x20) < 0x181 && (u16)(q->unk_16 + 0x20) < 0x121) |
                     ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121) |
                     ((u16)(q->unk_2C + 0x20) < 0x181 && (u16)(q->unk_2E + 0x20) < 0x121)) != 0)
                {
                    u8 t;

                    s->unk_14 &= 0x7FFF;
                    p->unk_10.w = p->unk_10.w + p->unk_08.w;
                    p->unk_14.w = (p->unk_14.w + p->unk_0C.w) << 8;
                    p->unk_0C.w = p->unk_0C.w << 8;
                    if (p->unk_24 & 0x100)
                    {
                        q->unk_0E = s->unk_12;
                    }
                    else
                    {
                        q->unk_0E = s->unk_12 + e->unk_06;
                    }
                    q->unk_0C = p->unk_0C.h + p->unk_08.h;
                    q->unk_18.h = p->unk_0C.h + p->unk_10.h;
                    if (s->unk_10 != 0)
                    {
                        q->unk_1A = s->unk_10 + (e->unk_04 & 0xFF9F);
                    }
                    else
                    {
                        q->unk_1A = e->unk_04;
                    }
                    q->unk_24.h = p->unk_14.h | p->unk_08.h;
                    q->unk_30.h = p->unk_14.h | p->unk_10.h;
                    if ((s16)p->unk_50 >= 0x1800)
                    {
                        t = q->unk_30.b[0];
                        q->unk_30.b[0] = t + 0xFF;
                        q->unk_18.b[0] = t;
                    }
                    if ((s16)p->unk_58 >= 0x1800)
                    {
                        t = q->unk_30.b[1];
                        q->unk_30.b[1] = t + 0xFF;
                        q->unk_24.b[1] = t;
                    }
                    if ((s16)q->unk_08 > (s16)q->unk_2C)
                    {
                        t = q->unk_30.b[0];
                        q->unk_30.b[0] = t + 0xFF;
                        q->unk_18.b[0] = t;
                    }
                    if ((s16)q->unk_0A > (s16)q->unk_2E)
                    {
                        t = q->unk_30.b[1];
                        q->unk_30.b[1] = t + 0xFF;
                        q->unk_24.b[1] = t;
                    }
                    q->unk_04 = 0;
                    q->unk_10 = 0;
                    q->unk_1C = *(u32 *)s->unk_0C;
                    *(u32 *)((u8 *)q + 0x28) = *(u32 *)s->unk_0C;
                    setlen(q, 12);
                    setcode(q, 0x3C);
                    if ((*(s32 *)&p->unk_24 & 0xC) == 0xC)
                    {
                        setcode(q, 0x3E);
                    }
                    ((Unk453E0T *)q)->addr = ((Unk453E0T *)(p->unk_20 + p->unk_C0 * 4))->addr;
                    ((Unk453E0T *)(p->unk_20 + p->unk_C0 * 4))->addr = (u32)q;
                    q++;
                }
            }
            if ((s8)e->unk_00 < 0)
            {
                break;
            }
            e++;
        }
        PopMatrix();
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* func_800D1C70 with the D_800DEEC0 template, a 0x1000 scale,
   func_800D4158 / func_800D4494 as the hooks, two more renderer fields set
   (+0x12 and the 0x100 flag), and a tail the other two do not have: the
   leash measurement is kept in a local so the record's +0x92 can be seeded
   with `ang - v` after +0x88 takes `v` itself. 121 words, 0 differing. */
u8 *func_800D3F74(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    s32 v;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_12 = 0x7E40;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DEEC0, 0);
        n->unk_10 = (u32)func_800D4158;
        *(u32 *)(p + 0x8C) = (u32)func_800D4494;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        v = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(s16 *)(p + 0x88) = v;
        *(s32 *)(p + 0x90) = 0;
        *(s16 *)(p + 0x92) = ang - v;
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E22E0 and a 38-word tail of its own: a +0x9E
   counter stepped modulo 96, and a brightness ramp over its first 25 steps
   written into the renderer's three colour bytes.

   The modulo is cc1psx's signed-by-constant sequence and comes out of a
   plain `% 96` -- `mult` by 0x2AAAAAAB, `sra 4`, minus the sign, then
   `q*3 << 5` subtracted back off. `t` is s16: the `sll 16`/`sra 16` ahead
   of the `mult` is the dividend being narrowed, and the second pair after
   the `sh` is the same value re-extended for `t < 0x19`. The argument is
   `t * 85`, which synth_mult splits into `t*5` and `(t*5) << 4` added --
   two shifts and two adds, not a `mult`.
   207 words, 0 differing. */
void func_800D4158(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;
    s16 t;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E22E0[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    t = (s16)(*(u16 *)(p + 0x9E) + 1) % 96;
    *(s16 *)(p + 0x9E) = t;
    if (t < 0x19)
    {
        s32 w = ((func_800644B8(t * 85) >> 4) * 96 >> 8) + 0x80;

        c[0xE] = w;
        c[0xD] = w;
        c[0xC] = w;
    }
    else
    {
        c[0xE] = 0x80;
        c[0xD] = 0x80;
        c[0xC] = 0x80;
    }
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 with a guard ahead of it: while the engine's 0x2000 flag is
   down, a record carrying the 0x100 bit is handed to func_800AA258 and the
   frame ends there, and otherwise the actor's state byte is forced to 0xE.
   The 0xE is materialised once and used for both the compare and the store,
   which is the `addiu $v1, $zero, 0xE` in the `beqz` delay slot.
   79 words, 0 differing. */
void func_800D4494(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (!(D_80083462 & 0x2000))
    {
        if (*(u32 *)(d + 0x1C) & 0x100)
        {
            func_800AA258(a, b, c, d);
            return;
        }
        if (a[0x9A] != 0xE)
        {
            a[0x9A] = 0xE;
        }
    }
    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four two-gate installers: the same park as the ten above behind a second
   test, whose result is read as a halfword. The second parameter is unused --
   no instruction in any of the four mentions $a1 -- and the third and fourth
   are what the inner gate is handed. 25 words each, 0 differing. */
void func_800D45D0(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D4494;
        }
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D4634(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
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
        a->unk_8C = func_800D4494;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* 20 words, 0 differing. */
void func_800D48CC(u8 *a, void *b, u8 *c)
{
    if (func_800D1A80())
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0x7E40;
        *(u16 *)(c + 0x14) |= 0x100;
    }
}

/* Three more installers, with a `blez` gate instead of the `beqz` one: the
   predicate's result is tested as positive, not merely non-zero. */
void func_800D491C(UnkDungeonActor *p)
{
    if (func_800AB464() > 0)
    {
        p->unk_8C = func_800D4494;
    }
}

/* func_800D4AB0's step: re-seed the spawned node's mover from the owner's
   name record, tint the renderable and fade it, then raise the kill flag on
   two conditions. THE OWNER'S FLAG WORD IS REACHED THROUGH AN `UnkNode *` AT
   `q - 0x20`, NOT AS `*(u16 *)(q - 0x2)`: the listing keeps `q - 0x20` in
   $s1 across the call and reads `0x1E($s1)`, and the folded spelling is
   86 words / 60 differing -- a word short, with everything after the call
   shifted. Writing the 0x1E/0x1C pair as two statements and as one chained
   assignment are both 0 differing, so that order is a TIE. 87 words,
   0 differing. */
void func_800D4954(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    UnkNode *n;
    u8 *s;

    q = *(u8 **)(p + 0x88);
    n = (UnkNode *)(q - 0x20);
    s = *(u8 **)(q - 0x18);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(s + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(s + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(s + 0xA) - 0x32;
    c[0xE] = 0x28;
    c[0xD] = 0x28;
    c[0xC] = 0x28;
    *(s16 *)(c + 0x1A) += 0x190;
    if (*(u16 *)(p + 0x5A) & 1)
    {
        c[0xC] = 0x64;
    }
    if (*(u16 *)(p + 0x5A) & 2)
    {
        c[0xD] = 0x64;
    }
    if (*(u16 *)(p + 0x5A) & 4)
    {
        c[0xE] = 0x64;
    }
    if (*(u16 *)(c + 0x1C) < 0x1800)
    {
        *(s16 *)(c + 0x1E) += 0x190;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
    }
    if ((s16)func_80042900((s8 *)q, 0x1D) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if (n->unk_1E & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D4FC8's spawner over a SECOND actor. The +0x88 field is read TWICE
   because func_8004491C sits between the two reads; the 12-byte name copy is
   UnkDungeonName12, whose alignment of 1 produces the `lwl`/`lwr` +
   `swl`/`swr` pairs. 73 words, 0 differing. */
void func_800D4AB0(u8 *a)
{
    UnkNode *n;
    u8 *p;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x5A) = 0x78;
        *(s16 *)(p + 0x5C) = 0x78;
        *(s32 *)(p + 0x88) = *(s32 *)(a + 0x88);
        n->unk_10 = (u32)func_800D4954;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        s = *(u8 **)(*(u8 **)(a + 0x88) - 0x18);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(s + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(s + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(s + 0xA) - 0x32;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0;
        r->unk_1C = 0;
        *(UnkDungeonName12 *)((u8 *)n + 0x96) = *(UnkDungeonName12 *)D_800E233C;
        r->unk_08 = (u32)((u8 *)n + 0x96);
    }
}

/* Wire a newly spawned node to its owner. TWO THINGS THE LISTING FORCES.
   `n->unk_0C` is READ TWICE -- the halfword stores through the vector pointer
   wipe cse's memory table -- and so is `*(u32 *)(b + 0xC)`, for the same
   reason across the store to the view. The last four stores were swept
   exhaustively: of the 24 orders only this one and its 1-3-4-2 sibling reach
   0, the reader's order is 7 differing and three orders are a word short
   because the two `b + 0xC` reads merge. 55 words, 0 differing. */
/* FOUR PARAMETERS, the fourth unread, which is why the three-parameter form
   had no witness: both call sites -- func_800D4DE8 and the stub
   func_800D5294 -- set $a3 in the `jal`'s delay slot. 55 words, 0 differing
   either way. */
void func_800D4BD4(UnkNode *n, u8 *b, u8 *c, u8 *d)
{
    u8 *p;
    u8 *r;
    UnkVec *m;

    p = (u8 *)n + 0x20;
    *(u8 **)(p + 0x24) = b;
    func_8004491C(n, (u32)D_80045340);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x14) |= 0xC;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(c + 0x2);
    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(c + 0x6);
    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(c + 0xA);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x1E) = 0x800;
    *(s16 *)(r + 0x1C) = 0x800;
    *(u32 *)(p + 0xC) = *(u32 *)(b + 0xC);
    *(u32 *)(r + 0xC) = *(u32 *)(b + 0xC);
    *(s16 *)(r + 0x12) = 0x7DCF;
    *(u16 *)(r + 0x14) |= 0x100;
    func_8003DB94(r, D_800DECF8, 0);
}

/* One frame of the two-record glide. The countdown is func_800C5D80's idiom,
   and the life pair at +0x1E/+0x20 is RE-READ for each of the three channels
   rather than cached, which is what the three `lh`/`lh` pairs are.
   78 words, 0 differing. */
void func_800D4CB0(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C /= 2;
    b->unk_10 /= 2;
    b->unk_14 /= 2;
    c[0xC] = p[0xC] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xD] = p[0xD] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xE] = p[0xE] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    t = *(u16 *)(p + 0x1E) - 1;
    *(s16 *)(p + 0x1E) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 burst handler on the node at +0x24: track that node's vector
   with the three +0x3C/+0x3E/+0x40 offsets, and for the first 0x14 frames
   throw four more 0x212 nodes per frame, each wired by func_800D4BD4, given
   func_800D4CB0 as its step and a random (angle - 0x4000) << 7 on each of
   the three velocity words.

   func_800D4BD4 TAKES FOUR ARGUMENTS, NOT THREE -- see the note on its own
   definition. Both of its call sites (this one and the stub func_800D5294)
   set $a3, and the landed three-parameter definition had no witness because
   the fourth is never read.

   THE D_80083460 POINTER LOCAL IS ASSIGNED AFTER THE D_800814A0 STATEMENT.
   The preheader emits the two `lui`s in first-use order, so with
   `g = (u8 *)&D_80083460;` at the head of the block its `lui`/`addiu` take
   the `bgtz` delay slot and D_800814A0's `lui` follows -- 120 words,
   3 differing, the two pairs exactly swapped. Written inline as
   `*(u16 *)((u8 *)&D_80083460 + 0xA) -= 1;` it is 119 words, 25 differing,
   because the address then costs one word fewer and nothing fills the slot.
   120 words, 0 differing. */
void func_800D4DE8(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkNode *q;
    UnkVec *m;
    u8 *v;
    u8 *g;
    s32 i;

    q = *(UnkNode **)(p + 0x24);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)q->unk_08 + 0x2) + *(u16 *)(p + 0x3C);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)q->unk_08 + 0x6) + *(u16 *)(p + 0x3E);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)q->unk_08 + 0xA) + *(u16 *)(p + 0x40);
    if (++*(s16 *)(p + 0x16) < 0x14)
    {
        for (i = 0; i < 4; i++)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                func_800D4BD4(n, p, b, c);
                v = (u8 *)n + 0x20;
                *(s16 *)(v + 0x1E) = 0xA;
                *(s16 *)(v + 0x20) = 0xA;
                n->unk_10 = (u32)func_800D4CB0;
                m = n->unk_08;
                *(s32 *)((u8 *)m + 0xC) = ((rand() & 0x7FFF) - 0x4000) << 7;
                *(s32 *)((u8 *)m + 0x10) = ((rand() & 0x7FFF) - 0x4000) << 7;
                *(s32 *)((u8 *)m + 0x14) = ((rand() & 0x7FFF) - 0x4000) << 7;
                *(s16 *)((u8 *)m + 0xA) -= D_800DDC40[(*(u8 **)p)[0x13]] >> 1;
            }
        }
    }
    if (--*(s16 *)(p + 0x1E) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
}

/* The effect spawner the kill handlers call: allocate a 0x12 node, park a
   step in it, seed its mover from the caller's, tint it and ring a sound.
   func_800D5460 below is this source with func_800D5294 as the step and a
   0x32 life. See its `&colour` note. 77 words, 0 differing. */
void func_800D4FC8(UnkNode *src, s32 colour, u16 snd)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    u8 *g;

    /* `&colour` IS LOAD-BEARING and costs no code. The target keeps this
       parameter in its incoming home slot at 0x24($sp) -- stored once in the
       jal's delay slot, reloaded once -- and assign_parms (function.c:3775)
       gives a register parameter a MEM for its DECL_RTL at -O2 in exactly
       three cases: TREE_ADDRESSABLE, TREE_SIDE_EFFECTS, or a BLKmode type.
       A struct is ruled out by the call sites, which pass integer constants
       with `lui`/`ori`, so the original took its address. Without this the
       parameter takes a third callee-saved register and pays the
       save/restore pair: 78 words. `volatile` reaches 77 with 3 differing --
       the prologue permuted -- and the address-take reaches 0. */
    (void)&colour;
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0x24) = src;
        n->unk_10 = (u32)func_800D4DE8;
        *(s16 *)(p + 0x1E) = 0x1E;
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src->unk_08 + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src->unk_08 + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src->unk_08 + 0xA);
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *(s32 *)(p + 0xC) = *(s32 *)((u8 *)r + 0xC) = colour;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
        if (snd != 0)
        {
            func_800A56E0(snd);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}
