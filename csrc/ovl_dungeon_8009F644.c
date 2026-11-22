#include "common.h"
#include "ovl_dungeon_decls.h"

/* dungeon.bin's overlay, translation unit 2 of 4: func_8009F644 to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

/* Data declarations the carve left behind in csrc/ovl_dungeon.c's own
 * extern block. Every carved unit gets the whole block: an unused extern
 * emits nothing, whereas hunting them one compiler error at a time took four
 * rebuilds on 2026-11-22. Only the DATA declarations are copied -- the carve
 * already synthesises the function prototypes each unit needs, and a second,
 * differing prototype for the same callee would be worse than the problem.
 * Copied VERBATIM, because the declared size is what makes a symbol a
 * symbolic address rather than a split lui/addiu.
 */
extern u8 D_800E0CAE[];
extern u8 D_800E0CDC[];
extern u8 D_800E0CF3[];
extern u8 D_800E0D1B[];
extern u8 D_800E0D36[];
extern u8 D_80089084[];
extern u8 D_80089080[];
extern u8 D_800DD880[];
extern u8 D_800DD8B4[];
extern u8 D_800E1C58[];
extern u8 *D_80073618[];
extern u8 *D_800DF03C[];

void func_8009345C(u8 *p, u8 *b, u8 *c, u8 *d);
void func_80093E74(u8 *p, void *b, void *c, u8 *d);
void func_80095A10(u8 *p, u8 *b, u8 *c, u8 *d);
s32 func_80096100(u8 *p);
u8 *func_80099254(u8 *s, u8 *d);
u8 *func_8009929C(s32 c, u8 *p);
s32 func_8009B164(u16 a, UnkVec *b, u8 *c);
u8 *func_8009B4B0(u8 *p, u16 x, u16 y);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_8009F644", func_8009F644);

/* Step the text cursor forward. The record is the fixed RAM address
   0x80013710, a constant address rather than a `D_` symbol.

   (1) THE BACK EDGE IS A `goto`, which loop.c cannot see, so the `lui`/`ori`
   pair is re-materialised at the top of every iteration where a `while (1)`
   would have let it be hoisted into a preheader.
   (2) THE LAST `return q` NEEDS ITS OWN BLOCK. Written as a plain tail the
   `move $v0, $a0` is scheduled into the `lhu 0xA`'s load-delay slot and the
   counter is pushed to $v1 -- 72 words, 5 differing. Sharing one exit through
   `goto out` puts the copy in a JOIN BLOCK where sched1 cannot reach it,
   which also lets the counter have $v0. An if/else with the tail duplicated
   is 75 words: cross-jumping does not merge them, because the two copies
   schedule differently. 72 words, 0 differing. */
u8 *func_8009F868(void)
{
    UnkDungeon13710 *p;
    u8 *q;

loop:
    p = (UnkDungeon13710 *)0x80013710;
    q = p->unk_0C + p->unk_0A * 2;
    if (q[1] == 0)
    {
        if (D_80013714 & 8)
        {
            *(u16 *)((u8 *)p + 0x4) |= 0x10;
            return NULL;
        }
        *(u16 *)((u8 *)p + 0x4) &= 0xC;
        p->unk_0A = 0;
        func_8004D0C8((u32)D_80083780);
        D_800832B4 = 0;
        func_8004D7A8(1);
        func_8004D294(NULL, D_800DCE60, 8);
        return NULL;
    }
    if (q[1] == 0xD0)
    {
        p->unk_08 = 0;
        p->unk_0A += *(s8 *)q;
        goto loop;
    }
    if (*(s8 *)q >= 0)
    {
        p->unk_08++;
        if (p->unk_08 < q[0])
        {
            goto out;
        }
    }
    p->unk_0A++;
    p->unk_08 = 0;
out:
    return q;
}

/* Step the text cursor back one entry and hand back where it now points.
   The record lives at the fixed RAM address 0x80013710, which arrives as a
   `lui`/`ori` pair -- the constant-address spelling, not a `D_` symbol.
   24 words, 0 differing. */
u8 *func_8009F988(void)
{
    UnkDungeon13710 *p;
    u8 *q;
    s32 c;

    p = (UnkDungeon13710 *)0x80013710;
    c = p->unk_0A;
    q = p->unk_0C + c * 2;
    if (p->unk_08 == 0)
    {
        q -= 2;
        p->unk_0A = c - 1;
        if (*(s8 *)q >= 0)
        {
            p->unk_08 = *q - 1;
        }
        else
        {
            p->unk_08 = 0;
        }
    }
    else
    {
        p->unk_08 = p->unk_08 - 1;
    }
    return q;
}

/* Search the 0x80013720 table downward for the entry whose type nibble is
   0x70, 0x78, 0x80 or 0x90 and whose +0x01 byte and 0x20 bit match (a, b).

   TWO SEPARATE DEVICES KEEP THE ARM'S READ WHERE IT IS, both ablated from
   this body. (1) The dead `c = 0;` ahead of the loop: without it loop.c
   hoists the arm's whole `base + n * 2` out and cse2 folds it onto the
   preheader load -- 45 words of 49, 36 differing. loop.c ~695 clause (3),
   `reg_in_basic_block_p`, opens on `regno_first_uid`, and the dead set moves
   that off the in-loop set. (2) The second `extern` under the same `__asm__`
   name: with the plain name in the arm the two `%hi(D_80013716)` are one
   pseudo and the arm gets `move $t0, $v1` where the target has its own `lui`
   -- 49 words, 3 differing.

   The type test must also be the `||` chain with the early return in the
   `else`: an `&&` chain of `!=` makes the return arm the fall-through, 47
   words of 49, 31 differing. 49 words, 0 differing. */
u8 *func_8009F9E8(u8 a, u8 b)
{
    s32 i;
    u8 *base;
    u8 *q;
    s32 t;
    u16 c;

    base = (u8 *)0x80013720;
    /* DEVICE: a dead set, so that `c`'s first mention is not the one inside
       the loop. flow deletes it and it costs no word. */
    c = 0;
    for (i = D_80013716 - 1; i >= 0; i--)
    {
        q = base + i * 2;
        t = q[1] & 0xF8;
        if (t == 0x70 || t == 0x78 || t == 0x80 || t == 0x90)
        {
            if (q[1] == a && (q[0] & 0x20) == b)
            {
                return q;
            }
        }
        else
        {
            c = D_80013716_b;
            return base + c * 2;
        }
    }
    return base;
}

/* TWO out-of-image accesses, so a BASE LOCAL. One access written as a plain
   constant address stays absolute and the assembler expands it with
   `lui $at`; two want the constant-base idiom, which gives one bare `lui`
   in an allocatable register and two displacements. `$at` in a listing is
   the tell for the first shape. 6 words, 0 differing. */
void func_8009FAAC(void)
{
    u8 *base = (u8 *)0x80010000;

    *(u16 *)(base + 0x3718) = 0;
    *(u16 *)(base + 0x371A) += 1;
}

/* ONE out-of-image access, so a plain constant address -- the assembler's
   `lui $at` expansion; see func_8009FAAC. 6 words, 0 differing. */
void func_8009FAC4(void)
{
    *(void **)0x8001371C = (void *)0x80013720;
}

/* 22 words, 0 differing. */
void *func_8009FADC(s16 x)
{
    s16 i;

    i = x - 1;
    if (i < 0x14)
    {
        return (u8 *)0x80010248 + i * 4;
    }
    if (i == 0x14)
    {
        return D_80081484;
    }
    return *(void **)(D_800E3D7C + 0xF0);
}

/* Find the D_800E2970 region containing tile (x, y). The lower-bound tests
   are `sltu` because both operands are u16 and shorten_compare keeps the
   comparison narrow; the upper-bound tests are `slt` because the sum is an
   int.

   THE BOUND IS READ IN THE `for` CONDITION, NOT INTO A LOCAL AHEAD OF IT. A
   local loads straight into the bound's register and the function is 46 words
   of 47 -- the target loads into $v0 for the guard and COPIES it to $t0,
   which is what the inline read gives. AND THE RECORD MUST BE INDEXED AS AN
   ARRAY OF STRUCTS: a hand-walked `u8 *` makes loop.c build two address givs,
   one biased by +6, and that is 48 words / 24 differing.
   47 words, 0 differing. */
s16 func_8009FB34(u16 x, u16 y)
{
    s32 i;

    for (i = 0; i < D_8008146E; i++)
    {
        if (D_800E2970_s[i].unk_0A != 0 && x >= D_800E2970_s[i].unk_00 &&
            x < D_800E2970_s[i].unk_00 + D_800E2970_s[i].unk_04 && y >= D_800E2970_s[i].unk_02 &&
            y < D_800E2970_s[i].unk_02 + D_800E2970_s[i].unk_06)
        {
            return i;
        }
    }
    return -1;
}

/* The room-rectangle lookup over D_800E2970's 0x14-byte records.

   TWO THINGS ARE FORCED. The record address is `D_800E2970 + i * 0x14` INSIDE
   the body, so loop.c makes it one giv; walking a `u8 *r` by hand gives TWO
   induction variables -- a second biased by 6, with the later fields read at
   -4/-2/0 -- 48 words against 47 and 22 differing. And the bound must be read
   in the loop CONDITION: a local is a second quantity and costs the whole
   prologue, 46 words / 42 differing. 47 words, 0 differing. */
s16 func_8009FBF0(u16 x, u16 y)
{
    s32 i;
    u8 *r;

    for (i = 0; i < D_8008146E; i++)
    {
        r = D_800E2970 + i * 0x14;
        if (*(s16 *)(r + 0xA) != 0 && x >= *(u16 *)r - 1 && *(u16 *)r + *(u16 *)(r + 4) >= x &&
            y >= *(u16 *)(r + 2) - 1 && *(u16 *)(r + 2) + *(u16 *)(r + 6) >= y)
        {
            return i;
        }
    }
    return -1;
}

/* `b = D_800E2970;` AS ITS OWN STATEMENT: both `&D_800E2970[i * 0x14]` and
   `D_800E2970 + i * 0x14` put the symbol's lui/addiu AFTER the index chain
   and sit at 6 differing. 17 words, 0 differing. */
s32 func_8009FCAC(s16 i)
{
    u8 *b;
    u8 *p;

    if (i < 0)
    {
        return 0;
    }
    b = D_800E2970;
    p = b + i * 0x14;
    if (*(s16 *)(p + 0xA) == 0)
    {
        return 0;
    }
    return *(s32 *)(p + 0x10);
}

/* Chebyshev distance between two tiles, and `abs()` IS THE WHOLE FUNCTION.
   The `bgez`/nop/`negu` trio plus the surviving `sll 16`/`sra 16` on the
   return is the abs() tell at docs/matching-levers.md:4615. A conditional
   negate loses BOTH -- reorg fills the trio's nop and combine's
   reg_sign_bit_copies then proves the s16 return extension redundant and
   deletes it: 17 words against 20. Nineteen other spellings were swept at
   15 differing; one near-miss, `s32` parameters with the operands masked in
   place, reproduces the target's register allocation and its cross-jumped
   tail exactly and still sits at 2 -- a right diagnosis of the allocation
   and a wrong one of the negate. 20 words, 0 differing. */
s16 func_8009FCF0(u8 x0, u8 y0, u8 x1, u8 y1)
{
    s32 dx;
    s32 dy;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (dx < dy)
    {
        return dy;
    }
    return dx;
}

/* 15 words, 0 differing. */
s16 func_8009FD40(u8 *a, u8 *b)
{
    return func_8009FCF0(a[0x24], a[0x25], b[0x24], b[0x25]);
}

/* Are the two tiles adjacent, non-identical and connected?

   ONE SHARED `return 0`, NOT FIVE EARLY ONES. Written as five early returns
   the first two tests still branch to the shared tail but the third gets its
   own `j`/`move v0,zero` pair: 70 words, 21 differing. Nesting the tests so
   every failure falls out of the bottom gives the target's three
   `beqz`. `w` is both func_800A0818's out-parameter and the variable the
   shifted result is parked in -- the address-take is why the store survives.
   70 words, 0 differing. */
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                w = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((w & 1) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* func_8009FD7C one argument wider: on a connected pair it asks
   func_8009A540 whether the step is legal rather than testing the wall bit.
   THE SHIFTED RESULT IS ITS OWN LOCAL HERE, which is the one place the two
   differ: reusing `w` for it, as func_8009FD7C does, costs the `sh`/`lhu`
   pair that spills it to the out-parameter's slot and reloads it for the call
   -- 81 words against 80, 34 differing. 80 words, 0 differing. */
/* SIX PARAMETERS, the sixth unread: func_800A3D18 sets $a0..$a3 plus
   0x10($sp) and 0x14($sp), and its frame starts the saved registers at 0x18,
   so the outgoing-argument area is six words wide. 80 words, 0 differing
   either way. */
s32 func_8009FE94(s16 x1, s16 y1, s16 c, s16 x2, s16 y2, s16 y3)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;
    s32 t;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                t = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((s16)func_8009A540((u16)t, x1, y1, (s16)(c - 0x20)) != 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Walk the eight neighbouring tiles and report the first that passes:
   D_8006CCD8 and D_8006CCE8 are the engine's sine and cosine tables, read
   here as u16 and added to the record's tile, and the index is handed back
   through the caller's halfword as `i << 9` -- the same facing encoding
   csrc/ovl_town.c decodes with `(ang >> 9) & 7`.

   func_8009FB34 RETURNS s16 AND TAKES u16: the `andi $a0, $a0, 0xFFFF`
   before the call is a u16 parameter truncating, and the `sll $v0, 16`
   after it is a halfword result being tested signed. The spawners above
   store its result with `sb` and so witness neither.
   44 words, 0 differing. */
s32 func_8009FFD4(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) < 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* func_8009FFD4 with the test inverted -- `bltz` for `bgez`, the first
   neighbour that FAILS. 44 words, 0 differing. */
s32 func_800A0084(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) >= 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* 7 words, 0 differing. */
s16 func_800A0134(u8 *a, u8 *b)
{
    return *(u16 *)(a + 0x88) - *(u16 *)(b + 0x88);
}

/* Tint three colour bytes by the engine's low-five-bit frame counter.

   TWO DECLARATIONS CARRY THE WHOLE FUNCTION, both about width.
   - THE THREE QUOTIENTS ARE `s16`. As s32 the signed divide-by-16 ends in
     `sra` where the target has `srl`: combine converts the ASHIFTRT to an
     LSHIFTRT only when it can see the result narrowed, and an s16
     destination is what narrows it. `u8` quotients, a `(u8)` cast at the
     store and an `s8 *` destination all leave the three `sra`.
   - `t` IS `s16` AND IS FIRST ASSIGNED THE UNMASKED HALFWORD, which is what
     keeps the `sll 16`/`sra 16` pair: with `t` only ever assigned masked
     values, combine's reg_nonzero_bits proves the top bits clear and elides
     the extension, one word short. The masked value lives in its own `s32 u`,
     which is also where the `addu $a2, $v1, $zero` copy comes from.
   47 words, 0 differing. */
void func_800A0150(u8 *p, u8 *d)
{
    s16 t;
    s32 u;
    s16 r;
    s16 g;
    s16 b;

    t = D_80083164[0];
    u = t & 0x1F;
    t = u;
    if (u & 0x10)
    {
        t = -u & 0x1F;
        if (t == 0)
        {
            t = 0x10;
        }
    }
    r = (0x80 - p[0]) * t / 16;
    g = (0x80 - p[1]) * t / 16;
    b = (0x80 - p[2]) * t / 16;
    d[0] = -0x80 - r;
    d[1] = -0x80 - g;
    d[2] = -0x80 - b;
}

/* Six bit tests in priority order, each arm a string address, and the NULL
   test after the chain is real: cc1psx does not fold `&array != 0`.
   40 words, 0 differing. */
void func_800A020C(s32 f, u8 *d)
{
    u8 *s;

    if (f & 0x10000000)
    {
        return;
    }
    if (f & 0x8)
    {
        s = D_800DD844;
    }
    else if (f & 0x20)
    {
        s = D_800DD850;
    }
    else if (f & 0x40)
    {
        s = D_800DD848;
    }
    else if (f & 0x400)
    {
        s = D_800DD840;
    }
    else if (f & 0x10)
    {
        s = D_800DD84C;
    }
    else
    {
        s = D_800DD83C;
    }
    if (s != NULL)
    {
        func_800A0150(s, d);
    }
}

/* Try the eight neighbouring tiles starting from the engine's current
   facing (D_8008347E & 7) and hand back the first that both func_8009B25C
   accepts and func_8009A540 confirms. The direction wraps with `& 7` while
   a separate counter runs 0..7, which is why there are two induction
   variables. 70 words, 0 differing. */
u8 *func_800A02AC(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
            {
                return r;
            }
        }
    }
    return NULL;
}

/* func_800A02AC with a func_800A2CB8 gate between the two.
   75 words, 0 differing. */
u8 *func_800A03C4(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_800A2CB8(p, r) != 0)
            {
                if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
                {
                    return r;
                }
            }
        }
    }
    return NULL;
}

/* `dir` is UNSIGNED -- as s32 the shift is `sra` where the target has
   `srl`. 22 words, 0 differing. */
void func_800A04F0(u8 *p, s32 x, s32 y, u16 dir)
{
    s32 d;

    d = (dir >> 9) & 7;
    func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                  *(s16 *)(p + 0x88));
}

/* Is (x, y) outside the map? 1 when it is. TWO EARLY RETURNS, NOT AN `&&`
   CHAIN: the positive form is the same 23 words and 8 differing, with the
   four branches' delay slots and the two constant materialisations
   redistributed. 23 words, 0 differing. */
s32 func_800A0548(s16 x, s16 y)
{
    s16 *sh = (s16 *)D_8008333C;

    if (x < 0 || x >= (1 << sh[10]))
    {
        return 1;
    }
    if (y < 0 || y >= (1 << sh[11]))
    {
        return 1;
    }
    return 0;
}

s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n)
{
    u8 *r;
    s16 xx;
    s16 yy;
    u16 i;
    s32 g;
    s16 lim;
    s32 thr;
    s32 k;
    s32 d;

    r = NULL;
    g = ((u32)dir >> 9) & 7;
    thr = 0x20;
    lim = n + n - n;
    i = g;
    xx = (D_8006CCD8[g] & 0xFFFF) + x;
    yy = (D_8006CCE8[g] & 0xFFFF) + y;
    if (func_800A0548(xx, yy) != 0)
    {
        p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
        p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
        return 0;
    }
    k = 0;
    while (k < lim)
    {
        r = func_8009B4B0(p, xx, yy);
        if (r != NULL)
        {
            d = *(s16 *)(r + 0x88) - *(s16 *)(p + 0x88);
            d = d >= 0 ? d : -d;
            if (d <= (s16)thr)
            {
                break;
            }
        }
        if (func_800A0548(xx + (D_8006CCD8[g] & 0xFFFF), yy + (D_8006CCE8[g] & 0xFFFF)) != 0)
        {
            p[0x72] = *(u8 *)&D_8006CCD8[g] - xx;
            p[0x73] = *(u8 *)&D_8006CCE8[g] - yy;
            return 0;
        }
        if ((s16)func_800A44E0((xx << 6) & 0xFFC0, (yy << 6) & 0xFFC0,
                               (s16)(*(u16 *)(p + 0x88) - thr), i << 9) != 0)
        {
            p[0x72] = -xx;
            p[0x73] = -yy;
            return 0;
        }
        thr += 0x20;
        k++;
        xx += D_8006CCD8[g] & 0xFFFF;
        yy += D_8006CCE8[g] & 0xFFFF;
    }
    p[0x72] = xx;
    p[0x73] = yy;
    return (s32)r;
}

/* 18 words, 0 differing. */
s16 func_800A07D0(s16 a, s16 b, s16 c, s16 d)
{
    return func_80065F90(d - b, c - a);
}

/* func_800A07D0's angle with a quadrant flag written back through the fifth
   (stack) argument.

   THE RESULT IS KEPT IN A `u16` COPY, AND THE RETURN EXPRESSION IS NARROWED
   TO MATCH. The target masks the CALL'S OWN $v0 while a copy of it lives in
   $v1 for the tail. A single `s32 ang` cannot do that: cse's make_regs_eqv
   short-circuits on `firstr < FIRST_PSEUDO_REGISTER`, so the pseudo always
   becomes canonical and every later use is rewritten to it -- nine spellings
   all sit at 1 differing. A NARROWER copy is invisible to cse, because
   `(set (reg:HI r) (subreg:HI ...))` is not a same-mode REG, and MIPS emits
   it as a plain `addu`. The widening back has to be free too:
   `(r + 0x100) & 0xE00` on a u16 is 35 words, one `andi 0xFFFF` too many.
   34 words, 0 differing. */
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f)
{
    s32 ang;
    u16 r;

    ang = func_80065F90(d - b, c - a);
    r = ang;
    if ((ang & 0x1FF) >= 0x100)
    {
        *f |= 2;
    }
    else
    {
        *f &= 0xFFFD;
    }
    r += 0x100;
    return r & 0xE00;
}

/* Up to 16 attempts to place a spawn.  189 words, 0 differing.

   Each attempt asks func_800A4E2C for a tile and rejects it if func_8009FB34
   says no; when the argument is 0 it also rejects any tile within a
   Chebyshev-ish 0x20 of D_80082E80+0x24/+0x25 (for a nonzero argument the
   distance stays at its initial 0x100 and the test always passes).  It then
   picks a row of the D_80083478 table -- the argument's own row for n >= 2, a
   random one for n < 2 -- publishes its second byte in D_800E2968, loads the
   resource with func_800A1618, and builds the entity by CALLING
   func_800A0B94's RESULT, the same idiom the landed func_80095A10 uses.
   n >= 2 then re-centres the camera; n < 2 paces the spawn against
   D_800835E4 with func_800A1D4C.

   ====================== THE LAST SIX ROWS WERE ONE FOUR-INSN WINDOW, AND THE
   ====================== CAUSE IS `birthing_insn_p`, NOT A SPELLING

   The park sat at 189/189 words with six rows, all in 800A0990..800A09B0:
   the target interleaves the table load with the sign extension of `off`

       lui  $v0, %hi(D_80083478)
       sll  $v1, $v1, 16        <- the sign extension's first half
       lw   $v0, %lo(D_80083478)($v0)
       sra  $v1, $v1, 16
       addu $v1, $v1, $v0

   and `t = D_80083478; t += (s16)off;` always emitted the `lw` before the
   `sll`.  Read out of the .sched dump rather than guessed: in that block
   every insn has INSN_PRIORITY 1 except the `addu` at 2, and the ready list
   at the deciding step prints `171 (7f000001) 177 (1)` -- the sign
   extension's `sll` carries `max_priority` and the `lw` does not.  That is
   `adjust_priority` (sched.c:2551): REG_DEAD notes are gone by then so
   n_deaths is always 0, and case 0 sets INSN_PRIORITY to max_priority when
   `birthing_insn_p` holds -- dest is a REG, live before the insn, and
   **reg_n_sets == 1**.  The `sll`'s destination is a fresh temp, so it is
   boosted; `t` is written TWICE (`t = ...` then `t += ...`), so the `lw` is
   not.  sched1 runs backward, and a boosted insn is taken earlier in the
   backward pass, i.e. placed LATER -- hence `lw` then `sll`.

   THREE EDITS CLOSE IT, and all three are needed:

   1. **A SEPARATE BASE LOCAL** `tz`, so the `lw`'s destination has ONE set
      and is boosted too.  Both then carry max_priority, the class test ties
      (the `sll` feeds the `sra` at cost 1; the `lw` has no link to it), and
      rank_for_schedule falls through to INSN_LUID -- which prefers the
      HIGHER LUID, so the `lw` must come SECOND in the source.  Hence `tz`
      is assigned BEFORE the use of `off`, not after.
   2. **`tz` IS PINNED TO `$v0`** -- `register u8 *tz __asm__("$2");`.
      Without it `off` lands in $v0, reorg may not steal the else arm's
      `move` into a branch delay slot that reads $v0, and the arm costs a
      `nop`, a `j` and a duplicated `sll`: 191 words / 14-18 rows.  That is
      the same tension park lever 7 recorded for the one-expression form.
   3. **THE OFFSET IS THE FIRST ADDEND, AND THE ADD IS AN INTEGER ADD:**
      `t = (u8 *)((s16)off + (s32)tz);`.  This is the whole last five rows.
      With `t = tz + (s16)off;` block_alloc gives the destination a copy
      suggestion for the FIRST operand's register, so `t` takes $v0 (the
      dying pinned base) and the %hi base of the D_800E2968 store takes $v1;
      the target has them the other way round.  Writing the offset first --
      and casting both sides so it is an integer add rather than pointer
      arithmetic, which gcc canonicalises back to `tz + off` -- suggests
      $v1, the register `off` is dying in, and all five rows go.
      The pointer-arithmetic spelling `t = (s16)off + tz;` measures 189/5.

   Also measured on the pinned base: keeping `tz` alive past the add with
   `__asm__("" ::"r"(tz))` 189/7 (it moves the `sll` back); a free `t`
   reference in the `h == 0` arm to push `t` out of local-alloc 191/13, in
   the `e == 0` arm 192/42; `t` pinned to $v1 as well, no change (the
   suggestion already decides it).  And without the pinned base: the sign
   extension hoisted into its own local `so`, 189/6 byte-identical to the
   unsplit form; `so` or the sign extension routed through the existing `e`
   189/7 or `v` 191/44 (both are live across calls, so the shared allocno
   goes callee-saved); the extension spelled `(u16)off << 16` then `>> 16`
   191/18 (the cast is its own sll/sra pair).

   ========================================= THE EARLIER LEVERS, ALL STILL IN

   5. `c` IS A `u8` LOCAL THE STORE GOES THROUGH, NOT A SECOND `t[1]`:
      `cb = t[1]; D_800E2968 = cb; c = cb;` with `u8 cb` keeps the loaded
      byte in a QImode pseudo, so the store is `sb $a0` and the widening to
      `c` is zero_extendqisi2 on that register -- the target's
      `andi $s3, $a0, 0xFF`.  `D_800E2968 = t[1]; c = (u8)t[1];` reloads
      instead: cse does not forward the second read across the store to the
      global.  2 rows.
   6. `off` IS `s16`, WHICH IS WHAT MAKES loop.c HOIST THE WHOLE `(n - 2)*2`.
      With `s32 off` the shift writes `off` directly, `off` has two sets, and
      only `addiu $v0, $s2, -0x2` hoists.  With `s16 off` the assignment
      expands as `temp = (n-2)*2; off = (HI)temp`, `temp` is a non-user
      pseudo with ONE set, and force_movables (loop.c:1320) hands the
      movable that is the LAST USE of `(s32)n` the free move at loop.c:1731
      -- it is NOT the desirability test, which prints `not desirable` here.
   7. THE TABLE POINTER IS LOADED BEFORE THE INDEX IS ADDED -- two
      statements, never `t = D_80083478 + (s16)off;` (191 words / 14 rows).
   8. THE PACING BLOCK INDEXES THE ARRAY DIRECTLY AND STARTS THE POINTER AT
      [1], AND THE DO-WHILE TEST IS WRITTEN WITH THE TABLE ON THE LEFT.
      Fifteen rows, and two separate facts: reading the first element off the
      ARRAY and giving the variable the +1 address makes the base a
      block-local temp so `g` loses the local-alloc tie and takes $a0 while
      `tb` gets $v1; and `while (tp[q[0x11]] <= *(u32 *)(q + 0x18))` puts the
      TABLE load first in the RTL, where the `q + 0x18` reload then writes
      $v0 and sched2 cannot float it into the `lbu`'s load-delay slot.
   1. THE DISTANCE IS `abs()`, NOT `if (d < 0) d = -d;`.  `abssi2` emits its
      OWN delay slot, so it expands to `bgez / nop / negu` and the two
      `nop`s at 800A0958 and 800A0968 are part of the pattern.
   2. THE PACING LOOP NEEDS ITS OWN POINTER LOCAL FOR THE INNER TEST, and
      `tp = tb;` must stay INSIDE the guarded arm: moved above the `if` it is
      in the same basic block as its source, optimize_reg_copy_1
      (local-alloc.c:704) rewrites the guard's `tb` to `tp` and the copy
      evaporates.  187 words. */
void func_800A08A0(s32 a)
{
    u8 x;
    u8 y;
    u8 *t;
    u8 *q;
    u8 *w;
    u32 *tb;
    u32 *tp;
    u32 g;
    s32 e;
    s32 v;
    s32 d1;
    s32 d2;
    s16 dist;
    s16 n;
    s16 k;
    s16 off;
    s16 h;
    s16 c;
    u8 cb;
    register u8 *tz __asm__("$2");

    dist = 0x100;
    if ((D_800E296C & 0x10000000) != 0)
    {
        return;
    }
    if (D_8008347C >= 0x20)
    {
        return;
    }
    n = a;
    for (k = 0; k < 0x10; k++)
    {
        func_800A4E2C(&x, &y);
        if (func_8009FB34(x, y) < 0)
        {
            continue;
        }
        if (n == 0)
        {
            dist = abs(D_80082E80[0x24] - x) + abs(D_80082E80[0x25] - y);
        }
        if (dist < 0x21)
        {
            continue;
        }
        if (n < 2)
        {
            off = (func_800A6D30() & 0xF) * 2;
        }
        else
        {
            off = (n - 2) * 2;
        }
        tz = D_80083478;
        t = (u8 *)((s16)off + (s32)tz);
        cb = t[1];
        D_800E2968 = cb;
        c = cb;
        h = t[0];
        if (h == 0)
        {
            return;
        }
        e = (s32)func_800A1618(h, 1);
        if (e == 0)
        {
            continue;
        }
        v = func_800BCB04((x << 6) | 0x20, (y << 6) | 0x20, -0x400);
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(h, (u8 *)e, 1))(0, x, y, (s16)v);
        if (q == NULL)
        {
            return;
        }
        func_800A152C(h, 1);
        func_80042640((Unk42710 *)q, h);
        q[0x43] = func_800A6D30();
        q[0x12] = 0;
        if (n >= 2)
        {
            w = *(u8 **)(q - 0x14);
            func_8009A3D0(w[0x24], w[0x25], 0x3000);
            func_800A32A4(q);
            func_8009A028(q);
            *(u16 *)(q - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        g = D_800835E4[c];
        *(u32 *)(q + 0x18) = g;
        tb = &D_800835E4[1];
        if (g >= tb[q[0x11]])
        {
            tp = tb;
            do
            {
                func_800A1D4C(q, 0);
            } while (tp[q[0x11]] <= *(u32 *)(q + 0x18));
        }
        if (q[0x49] != 0)
        {
            return;
        }
        *(s32 *)(q + 0x48) = func_800A9230(q);
        return;
    }
}

/* Upload one of three HUD image buffers for kind `a` and report the word the
   buffer starts with: slot 0x38 and 0x31 have fixed buffers and fixed
   descriptors, everything else indexes D_80170800 downwards by the slot byte
   at s[3] and computes its descriptor from (a - 1) and that byte.

   THE abs IS A TERNARY OVER A SECOND LOCAL, and that is one word. mips.md's
   `abssi2` emits `bgez %1,1f / move %0,%1 / subu %0,$0,%0` -- three words,
   with the copy in the delay slot -- only when operands 0 and 1 are
   DIFFERENT registers; `if (v < 0) { v = -v; }` and `v = v < 0 ? -v : v;`
   over ONE variable both give the two-word same-register form and the
   function is 143 against 145. `u = D_800E3DA0[s[3]]; v = u >= 0 ? u : -u;`
   fires it (and is byte-identical to `__builtin_abs(u)`). Measured and
   rejected at the two-word form: v as s8 (147, the QImode compare costs a
   `sll`/`sra 24` pair), v as s16 (147), v as u8 (140), a plain copy
   `v = u; if (u < 0) v = -v;` with u as s32/s16/s8 (all 144, gcc coalesces
   the copy away), and `__asm__("" : "=r"(v) : "0"(v))` (146).

   THE DESCRIPTOR'S 0x4340 MUST RIDE THE s[3] TERM, assigned INSIDE the sum.
   `(a - 1) * 84 + s[3] * 12 + 0x4340` lets fold factor the common 4 out of
   both products: 143 words, two short. Grouped `(a - 1) * 84 + (s[3] * 12 +
   0x4340)` the arithmetic is right but the constant still migrates onto the
   (a-1) chain, 144/124; hoisted into its own statement first the chains
   schedule the wrong way round, 144/125. `n = (a - 1) * 84 + (m = s[3] * 12
   + 0x4340);` is 0. 145 words, 0 differing. */
s32 func_800A0B94(s16 a, u8 *s, s16 c)
{
    u8 *t;
    s32 r;
    s32 v;
    s32 u;
    s32 w;
    s32 n;
    s32 m;

    r = 0;
    if (s[0] != 2)
    {
        u = D_800E3DA0[s[3]];
        v = u >= 0 ? u : -u;
        if (v == a && v != 0x39)
        {
            s[1] = a;
        }
    }
    if (s[1] == 0)
    {
        if (a == 0x38)
        {
            t = D_80164800;
            n = 0x56B1;
            w = 0x24;
        }
        else if (a == 0x31)
        {
            t = D_8016A800;
            n = 0x569C;
            w = 0x18;
        }
        else
        {
            w = 0xC;
            t = D_80170800 - s[3] * 0x6000;
            n = (a - 1) * 84 + (m = s[3] * 12 + 0x4340);
        }
        func_8003F6D4(w, t, D_800E58A8, n);
        func_8003E4FC(6, D_800E58A8, 0);
        if (c != 0)
        {
            func_8003F320();
            if (s[0] != 2)
            {
                D_800E3DA0[s[3]] = a;
            }
            r = *(s32 *)t;
            *(s32 *)(s + 4) = r;
        }
        else
        {
            if (s[0] != 2)
            {
                D_800E3DA0[s[3]] = -a;
            }
        }
    }
    else
    {
        if (s[0] != 2)
        {
            D_800E3DA0[s[3]] = a;
        }
        if (a == 0x38)
        {
            t = D_80164800;
        }
        else if (a == 0x31)
        {
            t = D_8016A800;
        }
        else
        {
            t = D_80170800 - s[3] * 0x6000;
        }
        r = *(s32 *)t;
    }
    return r;
}

/* Run the D_80083460 + 0x6 countdown down and, when it goes negative, reseed
   it from the LCG with 0x20 forced on. 27 words, 0 differing. */
void func_800A0DD8(void)
{
    u8 *g;
    s16 t;

    if (D_8008146E != 0)
    {
        g = (u8 *)&D_80083460;
        t = *(u16 *)(g + 0x6) - 1;
        *(s16 *)(g + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(g + 0x6) = (func_800A6D30() & 0x1F) | 0x20;
            func_800A08A0(0);
        }
    }
}

/* Park a random nibble in the engine's D_80083466.

   THE CALL IS ITS OWN STATEMENT AND THE MASK IS NOT. Spelled
   `D_80083466 = func_800A6D30() & 0xF;` the assignment expands its LHS
   address FIRST, so the `%hi` lands in the jal's delay slot in a
   callee-saved register and the function is two words longer; with the
   mask folded into the call's own statement the `lui` comes out after the
   `andi`. 10 words, 0 differing. */
void func_800A0E44(void)
{
    s32 v;

    v = func_800A6D30();
    D_80083466 = v & 0xF;
}

s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d)
{
    u8 seen[8];
    u8 *g;
    u8 *r;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s16 i;
    s16 j;
    s16 k;
    s16 t;
    s32 v;
    s32 q;
    s32 w;
    s16 m;

    if (*(s8 *)(b + 0x26) >= 0)
    {
        if ((c[0x72] | c[0x73]) == 0 || *(s8 *)(b + 0x26) != f)
        {
            do
            {
                c[0x73] = 0;
            } while (0);
            c[0x72] = 0;
            c[0x67] = 0;
            b[0x27] = 0xFF;
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                g = D_800E2970;
                for (i = 0; i < *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE); i++)
                {
                    e = (u8 *)(i * 4 + (s32)r);
                    if (e[0] != 0)
                    {
                        v = b[0x24] - e[0];
                        v = v >= 0 ? v : -v;
                        if (v < 2)
                        {
                            v = b[0x25] - e[1];
                            v = v >= 0 ? v : -v;
                            if (v < 2)
                            {
                                goto hit;
                            }
                        }
                    }
                }
            again:
                j = func_800A6D30();
                for (i = 0; i < D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E; i++)
                {
                    j = j % D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E;
                    e2 = (u8 *)(j * 4 + (s32)r);
                    if (e2[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e2 + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e2 + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e2 + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            hit:
                b[0x27] = i;
                c[0x67] = e[2];
                goto again;
            }
        }
        else if (b[0x24] == c[0x72] && b[0x25] == c[0x73])
        {
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                j = D_8008347E;
                for (i = 0; i < D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E; i++)
                {
                    j = j % D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E;
                    e2 = (u8 *)(j * 4 + (s32)r);
                    if (e2[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e2 + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e2 + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e2 + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            }
        }
        else
        {
            k = (((s16) * (u16 *)(c + 0x2A) >> 9) - 4) & 7;
            *(s32 *)&seen[0] = 0;
            *(s32 *)&seen[4] = 0;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            w = (s16)(func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73),
                                    (u16 *)d) &
                      0xFFF);
            k = ((w >> 9) - 4) & 7;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            for (i = 0; i < 8; i++)
            {
                if ((*(u16 *)d & 2) != 0)
                {
                    m = ((w - D_8006CD00[i]) >> 9) & 7;
                }
                else
                {
                    m = ((w + D_8006CD00[i]) >> 9) & 7;
                }
                if (seen[m] == 0)
                {
                    if ((s16)func_800BCB04((u16)(D_800DCEAC[m] + (b[0x24] << 6)),
                                           (u16)(D_800DCEBC[m] + (b[0x25] << 6)),
                                           (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
                    {
                        *(s16 *)(c + 0x2A) = m << 9;
                        return 0;
                    }
                }
            }
            return 0;
        }
        goto out;
    found:
        e3 = (u8 *)(j * 4 + (s32)r);
        c[0x72] = e3[0];
        c[0x73] = e3[1];
        c[0x67] = e3[2];
        *(s16 *)(c + 0x2A) =
            func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73), (u16 *)d);
        b[0x27] = j;
    }
    else
    {
        b[0x27] = 0xFF;
        q = ((*(s16 *)(c + 0x2A) - D_8006CD02) >> 9) & 7;
        if ((s16)func_800BCB04((u16)(D_800DCEAC[q] + (b[0x24] << 6)),
                               (u16)(D_800DCEBC[q] + (b[0x25] << 6)),
                               (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
        {
            *(s16 *)(c + 0x2A) = q << 9;
        }
    }
out:
    return 0;
}

/* The `q == NULL` arm is laid out INLINE here, so it is the second arm of the
   `if`, not an early return: `if (q == NULL) { return 0; }` puts the
   `return 0` at the END and measures 33 words / 12 differing. Both spellings
   are 33 words, so only the differing count separates them.
   33 words, 0 differing. */
s32 func_800A152C(s16 a, s16 b)
{
    u8 *q;
    u8 *g;

    if (a == 0x39)
    {
        a = 2;
    }
    q = func_800A1618(a, b);
    if (q != NULL)
    {
        q[1] = a;
        q[2] += 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x1C) += 1;
        return 1;
    }
    return 0;
}

/* Is the given kind (0x39 read as 2) in the six-entry table at
   D_800E3DB0 + 1, stride 8?

   THE TABLE POINTER IS ASSIGNED BEFORE THE `if`. Left in the `for`
   initialiser it is 26 words with 16 differing: the `lui`/`addiu` pair lands
   after the 0x39 test instead of at the top, and every register in the
   function moves with it. `i = 0` stays in the `for` -- hoisting that too is
   6 differing. 26 words, 0 differing. */
s32 func_800A15B0(s16 k)
{
    s16 i;
    u8 *p;

    p = D_800E3DB0;
    if (k == 0x39)
    {
        k = 2;
    }
    for (i = 0; i < 6; i++, p += 8)
    {
        if (p[1] == k)
        {
            return 1;
        }
    }
    return 0;
}

/* Find the table slot for kind `a` in mode `b`: the 0x38 and 0x31 kinds reset
   their counters and hand back a fixed slot, b == 3 searches D_800E3DD0 from
   index 4, b == 2 hands back D_800E3DE0, and otherwise the search starts in
   D_800E3DB0 at whichever of index 3, 2 or 0 the two stored kinds select.
   This is the long form of the landed func_800A17E8 above -- same dispatch
   chain, same six-entry stride-8 table, same `i` allocation race.
   116 words, 0 differing.

   `v = p[1];` FOR THE FIRST TEST IS WORTH 13 WORDS OF DIFFERENCE. Written
   `if (p[1] == 0 || p[1] == a)` the giv reading p[1] gains a reference, wins
   the allocation race against `p` itself, and the two come out in each
   other's registers: 116 words, 16 differing, every one of them that pair.
   The THIRD read of p[1], inside the arm, must stay a read -- it is the
   target's second `lbu 0x0($a0)`, which cse cannot common because the arm is
   reached from two predecessors. */
u8 *func_800A1618(s16 a, s16 b)
{
    u8 *found;
    u8 *p;
    s32 i;
    u8 v;

    /* DEVICE: a zero-instruction reference that makes `i` live at function
       entry, exactly as the landed func_800A17E8 above needs it -- that
       function is this one's shorter sibling and its comment block has the
       `.lreg`/`.greg` reading. It emits nothing: `i` is assigned on every
       path that reaches the loop, so the read is dead in the abstract
       machine and gcc expands no code for it. It warns under `-Wall`
       (uninitialised use); the default CFLAGS has no `-Wall`, which is why
       the build is silent.
       WORTH 3 OF 116 WORDS: without it this body is 116 words and 3
       differing, the whole residue being the `b == 3` arm, where
       `addiu $a2, $zero, 0x4` and `addiu $v1, $v0, %lo(D_800E3DD0)` come out
       in each other's slots. Ruled out, all at 116 words:
         every arm writing `i` before `p`              17 differing
         only the 0x38 and 0x39-table arms doing so     3
         `i = 4` hoisted above the chain               13
         `i = 0` hoisted above the chain               27 (dead, same md5)
         `s16 i` with the device                      123 words
         `u32 i`                                       27
         declaration order, all 6 permutations         27 (same md5)
         `p = &D_800E3DB0[0x18]` for `p += 0x18`      117 words
         a separate `g` base as func_800A17E8 uses    117 words
         a second walked pointer for `p[1]`            22, and it moves the
           giv's initialiser out of the hoist block
       func_800A17E8 records two permuter runs, 352 and 82,594 candidates,
       agreeing that no legal spelling reaches 0 on its own copy of this
       allocation race. */
    __asm__("" ::"r"(i));
    found = NULL;
    if (a == 0x38)
    {
        D_800E3DB0[0x11] = 0;
        D_800E3DA0[2] = 0;
        D_800E3DA0[1] = 0;
        D_800E3DA0[0] = 0;
        return &D_800E3DB0[0x10];
    }
    if (a == 0x31)
    {
        D_800E3DB0[9] = 0;
        D_800E3DA0[1] = 0;
        D_800E3DA0[0] = 0;
        return &D_800E3DB0[8];
    }
    if (a == 0x39)
    {
        a = 2;
    }
    if (b == 3)
    {
        p = D_800E3DD0;
        i = 4;
    }
    else if (b == 2)
    {
        return D_800E3DE0;
    }
    else
    {
        p = D_800E3DB0;
        if (p[0x11] == 0x38)
        {
            p += 0x18;
            i = 3;
        }
        else if (p[9] == 0x31)
        {
            p += 0x10;
            i = 2;
        }
        else
        {
            i = 0;
        }
    }
    for (; i < 6; i++, p += 8)
    {
        if (b == 1 || b == 3)
        {
            if (p[0] != b)
            {
                break;
            }
        }
        v = p[1];
        if (v == 0 || v == a)
        {
            if (b == 2)
            {
                D_800E3DA6 = 0;
                return p;
            }
            found = p;
            if (p[1] == a)
            {
                break;
            }
            if (D_800E3DA0[i] == a)
            {
                break;
            }
        }
    }
    if (found != NULL)
    {
        func_80048224(a);
    }
    return found;
}

/* CARRIES A DEVICE, and the device-free floor is 5 differing.

   The whole residue is an ALLOCATION one: every opcode, immediate and
   displacement already matches and only three registers are wrong. The target
   puts the loop counter in $a2, the sign-extended `k` in $a1 and the raw `k`
   copy in $a3; the plain source puts the counter in $a1 and both `k`s in $a2.
   Read off `.lreg`/`.greg`: global.c's `floor_log2(n_refs) * n_refs /
   live_length` scores the counter 9 refs over 30 insns (0.90) against the
   extended `k`'s 7 over 18 (0.78), so the counter is allocated first and
   takes the lower register. The counter has to LOSE that race, and the only
   thing that moves it is making it live at FUNCTION ENTRY: that stretches its
   live length to 48, scores it 0.69, and every register falls into place.

   Measured, all 64 words: plain 13 differing; `i = 4` before the chain 5;
   every arm writing `i` before `p` 9 (all-four-or-nothing -- the other
   fifteen subsets are 15..19); `i = 0` before the chain 25; a third `s16 i`
   parameter 13 (its incoming copy is dead on every path, so flow deletes it);
   a two-case switch 24; a `do`/`while` 13; `s32 i` 63 words / 22. Two
   permuter runs, 352 and 82,594 candidates, agree that no legal spelling
   reaches 0.

   THE DEVICE READS AN UNINITIALISED LOCAL. It emits no instruction and exists
   only to make `i` live-in at entry. The function has no callers and no
   address-takers anywhere in asm/, so nothing downstream depends on it. If
   this is not wanted, delete the two device lines and write `i = 4;` before
   the `if` chain with the k==3 arm reduced to `p = D_800E3DD0;` -- that is 64
   words, 5 differing, all inside that one arm. 64 words, 0 differing. */
u8 *func_800A17E8(s16 x, s16 k)
{
    u8 *g;
    u8 *p;
    s16 i;

    /* DEVICE: a zero-instruction reference that makes `i` live at entry. */
    __asm__("" ::"r"(i));
    if (k == 3)
    {
        p = D_800E3DD0;
        i = 4;
    }
    else if (k == 2)
    {
        return D_800E3DE0;
    }
    else
    {
        g = D_800E3DB0;
        if (g[0x11] == 0x38)
        {
            p = g + 0x18;
            i = 3;
        }
        else if (g[0x9] == 0x31)
        {
            p = g + 0x10;
            i = 2;
        }
        else
        {
            p = g;
            i = 0;
        }
    }
    while (i < 6)
    {
        if (k == 1 || k == 3)
        {
            if (p[0] != k)
            {
                return NULL;
            }
        }
        if (p[1] == x)
        {
            return p;
        }
        i++;
        p += 8;
    }
    return NULL;
}

/* Run down one 8-byte D_800E3DB0 entry whose kind and owner match.

   THE OWNER LOCAL IS `s16`. As `s32 m` the whole register assignment rotates
   -- the value is loaded into $a0 and copied into $s0 at the call where the
   target loads it into $s0 and copies it into $a0: 63 words, 16 differing,
   every one a register name, with no extra instruction anywhere. As `u8 m`
   it also costs an `andi` (64 words). 63 words, 0 differing. */
void func_800A18E8(s16 a, s16 b)
{
    s16 i;
    u8 *p;
    s16 m;

    p = D_800E3DB0;
    if (a == 0x39)
    {
        a = 2;
    }
    if (b == 1 || b == 3)
    {
        for (i = 0; i < 6; i++, p += 8)
        {
            if (p[0] == b)
            {
                m = p[1];
                if (m == a)
                {
                    if (*(s8 *)(p + 2) <= 0)
                    {
                        return;
                    }
                    if (p[0] == 1)
                    {
                        return;
                    }
                    p[2]--;
                    if ((s8)p[2] > 0)
                    {
                        return;
                    }
                    p[1] = 0;
                    p[2] = 0;
                    if ((s16)func_800A15B0(m) == 0)
                    {
                        func_80048190(m);
                    }
                    return;
                }
            }
        }
    }
}

/* Raise or clear the view's 0x20000 "this one is interesting" bit from the
   distance to D_80082E80, and hand the caller back 0 when the bit ends up
   clear. THREE THINGS THE LISTING FORCES.
   THE RETURN TYPE IS NOT `void`: the declaration at the top of the unit said
   so, but the tail reads +0x1C again and shifts it into $v0, which a void
   function would have deleted -- so it is now `s32`. Both landed callers
   (func_800A1B44, func_800AA53C) discard the result and both still measure
   0 differing.
   THE SHIFT IS UNSIGNED: through `*(s32 *)` it is `sra 17` (1 differing);
   the field has to be read as `u32` for the `srl`.
   AND THE OUT-PARAMETER'S -1 IS SIGNED: `*out = -1` through the declared
   `u8 *` is `li $v0, 255` where the target has `addiu $v0, $zero, -0x1`
   (2 differing, one per arm), so the store casts to `s8 *`.
   D_80082EA6 is its own symbol and not `D_80082E80[0x26]`: the `lb` carries
   its own `lui`, in a block the call's `addiu $a0, %lo(D_80082E80)` does not
   reach. 88 words, 0 differing. */
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out)
{
    s32 fl;

    fl = *(s32 *)(b + 0x1C);
    if ((fl & 0x20000) != 0)
    {
        if ((fl & 0x10) != 0)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
        else if ((*(s8 *)(a + 0x26) < 0 || *(s8 *)(a + 0x26) != D_80082EA6) &&
                 func_8009FD40(D_80082E80, (u8 *)a) >= d)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
    }
    else if ((fl & 0x10) == 0)
    {
        if ((*(s8 *)(a + 0x26) >= 0 && *(s8 *)(a + 0x26) == D_80082EA6) ||
            func_8009FD40(D_80082E80, (u8 *)a) < c)
        {
            *(s32 *)(b + 0x1C) |= 0x20000;
        }
    }
    return (*(u32 *)(b + 0x1C) >> 17) & 1;
}

/* Walk the engine's own ring and run func_800A19E4 on every member. The head
   is RE-READ out of D_800E3D7C at the bottom of each turn rather than held in
   a local, which is the second `lw %lo(D_800E3D7C)($s3)` inside the loop; the
   two s16 parameters have their `sll` hoisted into the preheader and their
   `sra` left in the body. 35 words, 0 differing. */
void func_800A1B44(s16 x, s16 y)
{
    u8 *p;

    p = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
    while (p != D_800E3D7C)
    {
        func_800A19E4(*(s32 *)(p - 0x14), p, x, y, p + 0x9C);
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* The same search over the engine's four-entry table, keyed on a POINTER
   field rather than a byte: +0xAC in one and +0xE4 in the other. The base is
   read from D_800E3D7C ONCE and walked, which is what keeps the `lw` out of
   the loop.

   func_800A1BD0 IS DECLARED IN include/structs.h, as `s32 func_800A1BD0(u8
   *)`, because a resident engine unit calls this address -- and the engine
   reaches whichever overlay is loaded, so that declaration is about this
   function. It is adopted here rather than guessed at, and it is what fixes
   the key as a `u8 *`: csrc/unk_154BC.c writes the same +0xAC field with
   `*(u8 **)(((s16)k) * 4 + D_800E3D7C + 0xAC) = nw;`. The s32 return then
   makes the index's `sll`/`sra` an explicit cast rather than a consequence
   of the return type. */
s32 func_800A1BD0(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xAC) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

s32 func_800A1C14(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xE4) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* A five-gate predicate returning 0 or 1 as an s16 -- the shared exit is
   `sra $v0, $v0, 16`, and each path narrows with its own `sll 16`. The last
   comparison is written TABLE-FIRST: the other order emits the two `lw`s
   reversed and swaps $v0/$v1 across six words. 61 words, 0 differing. */
s16 func_800A1C58(u8 *p)
{
    s16 r;

    r = 0;
    if (*(s8 *)(p + 0x13) < 0)
    {
        return 0;
    }
    if (p[0x11] >= 0x63)
    {
        return 0;
    }
    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        return 0;
    }
    if ((*(u32 *)(p + 0x1C) & 0x80000) != 0)
    {
        return 0;
    }
    if (p[0x13] == 0)
    {
        if (D_800E3D7C[0x9A] == 0x22)
        {
            return 0;
        }
    }
    if ((*(u32 *)(p + 0x1C) & 0x228) == 0 && D_800835E8[p[0x11]] <= *(u32 *)(p + 0x18))
    {
        r++;
    }
    return r;
}

/* The dungeon "level up" twin of func_800A2424.  438 words, 0 differing.

   SIX LEVERS, and the sixth is one free reference.

   1. THE SPELL LOOP'S RULE INDEX IS A FRESH LOCAL `k' THAT IS *ALSO* READ IN
      THE MESSAGE LOOP -- `k = p[i * 3 + 8];' in both, in place of the inlined
      subscript, lifted verbatim from the landed twin.  The message-loop read
      is the half that pays; the spell loop alone was measured and is not
      enough.  Fixes the whole spell-loop rotation (induction ptr $a2->$a1,
      k $a1->$a0, &e[i] $a0->$a2).  13 rows.
   2. `i = 0;' BEFORE `d = (UnkDungeonName0 *)D_8006DE24;', with the message
      loop written `for (; i < 3; i++)'.  The two insns are independent, so
      sched1 orders them by INSN_LUID and nothing downstream recovers the
      swap.  2 rows.
   3. `hi -= lo;' AS ITS OWN STATEMENT for STAT 5 ONLY (`hi -= lo; o = n =
      p[5]; n += hi;' rather than `o = n = p[5]; n += hi - lo;').  15 rows.
      It fixes both halves at once: the `lbu $v1, 0x5($s2)' stops being
      scheduled two words early, AND the $a0/$v1 rotation in the `hi' block
      comes out the target's way, because the statement changes the block's
      LUIDs and with them global.c's priority order over the t[5] reload and
      the two `mflo's.  The other five stat blocks keep `n += hi - lo;'.
      Note the direction: `hi -= lo' -- the level-DOWN twin has `lo - hi'.
   4. `hi = 0x63;' SPLIT OUT AS ITS OWN SET inside the loop, then
      `r = (hi - p[i*3+0xA]) / u2 + p[i*3+0xA];'.  Lever "DENY loop.c A
      CONSTANT HOIST BY PUTTING THE CONSTANT IN A *USER* VARIABLE": the holder
      is a user variable, set three times, live across basic blocks, so all
      three scan_loop arms fail, no movable is made, and the target's
      `addiu $v0, $zero, 0x63' is rematerialised at the use instead of
      hoisted to `li $t1, 0x63' in the preheader.  9 rows.
   5. THE 0x63 IS HELD IN `hi', NOT IN `r'.  Which user variable holds it
      decides whether the constant and the subtraction share a register.
      Measured holders, aligned rows: hi 4, u 6, w 6, r 6, hp 7, n 7, c 17,
      k 23, o 436 words/70, lo 440 words/104.

   6. `u2 = 0x64 - lvl;' GOES IN THE LOOP BODY AFTER `e[i] = 1;' AND IS
      HOISTED BY loop.c -- AND THE HOIST IS PAID FOR WITH ONE FREE REFERENCE
      TO `lvl'.  This is what took the park's last 4 rows to 0.

      The target's `addiu $v0, $zero, 0x64' / `subu $a3, $v0, $s1' sit at
      words 272/273, at the END of the loop preheader, after four other
      loop.c hoists.  That position can ONLY be a loop.c hoist: sched1 leaves
      the preheader in INSN_LUID order (all eight insns have priority 1), and
      `move_movables' does `emit_insn_before (..., loop_start)', so a hoist's
      position in the preheader is its SOURCE position among the movables --
      and arm insns are always emitted before every hoist, so no placement
      inside the `if (p[0x13] != 0)' arm can reach word 272.  In the arm the
      pair comes out at 267/268 instead: 438 words, 4 rows, which is where
      this park sat.

      But the hoist adds the five preheader insns to lvl's live range and
      that LOSES THE p/lvl ALLOCATION RACE BY 0.2%.  global.c's
      `allocno_compare' priority is floor_log2(n_refs) * n_refs / live_length:

        u2 in the arm      lvl: 4*16/197 = 0.3249  >  p 0.3188   OK
        u2 hoisted         lvl: 4*16/202 = 0.3168  <  p 0.3188   FLIP
                           p:   5*38/596 = 0.3188

      lvl must be allocated before p to get the target's lvl=$s1 / p=$s2.
      Hoisted and unpaid-for the whole register file rotates: 437 words,
      62 rows.

      THE CURE IS TO WRAP `lvl = p[0x11];' IN `do { } while (0);'.  That
      buys lvl a 17th reference for nothing: flow.c:2128 weights each
      reference by `loop_depth', and the inner loop's NOTEs raise the depth
      over the statement without emitting an instruction.  The priority
      becomes 4*17/202 = 0.3366, well clear of p, and the margin is wide
      rather than marginal -- 17 references stay ahead of p until lvl's live
      length reaches 213 against the 202 it has.

      `__asm__("" ::"r"(lvl));' as the first statement of the arm does the
      same thing and is how this first landed, but it is a scaffold where the
      do-while is ordinary C, so the do-while is what is in the tree.  Of the
      twelve statements mentioning `lvl', exactly three wrappers reach 0 or
      near it -- `lvl = p[0x11];' 0 rows, the `hp = ...' line 0 rows,
      `p[0x11] = lvl + 1;' 2 rows -- and the rest run 29 to 72, so the
      position is a real choice and not a formality.

      That is the general shape of this fix -- `flow_analysis' runs ONCE
      (toplev.c:3463) and BEFORE combine (:3488), so a reference that costs
      no instruction is free in the allocator's arithmetic.  The alternative
      route, taking one reference OFF p by gating the spell loop on
      `if (c != 0)', does produce the exact preheader but makes `c'
      callee-saved ($s6, pushing f to $s7 and the frame to 0x40): 438 words,
      16 rows.

      `u2' POSITIONS MEASURED (with levers 1-5 in place, aligned rows):
      first statement in the arm 4; split as `u2 = 0x64; u2 -= lvl;' in the
      arm 4 (identical); `i = 2;' before or after it in the arm 4
      (identical); before the `if' 437 words/25; top of the loop body 14
      (hoisted to the WRONG slot, before the D_8006DE24 base, and the `1'
      hoist is then LOST into the loop); after `e[i] = 0;' 14 (same);
      before `e[i] = 1;' 437/86; `(0x64 - lvl)' inline with no u2 local
      437/62; `u2 = -lvl; u2 += 0x64;' in the loop 437/63.

   Also measured and kept: `(*(u16 *)0x80013714 & 1) == 0' as a literal
   address, not through a declared symbol -- the symbol lets reorg lift its
   `lui' into the `beqz $v0, .L800A23F8' delay slot where the target has a
   `nop', and that is the whole word.  The second test measures the same
   either way. */
void func_800A1D4C(u8 *p, s16 f)
{
    u8 *t;
    u8 *q;
    u8 *g;
    void *m;
    void *h;
    UnkDungeonName0 *d;
    s32 lvl;
    s32 c;
    s32 k;
    s32 i;
    s32 r;
    s32 w;
    s32 u;
    s32 u2;
    s32 lo;
    s32 hi;
    u32 n;
    u32 o;
    u32 hp;
    u8 e[3];

    if (p[0x11] < 0x63)
    {
        do
        {
            lvl = p[0x11];
        } while (0);
        c = p[0x13];
        p[0x11] = lvl + 1;
        t = D_800DDCBC[c];
        q = (u8 *)&D_8006D168[c];
        lo = q[5] + t[5] * (lvl - 1) / 16 + t[5] * func_800647A0(((lvl - 1) * t[5]) << 11) / 32768;
        hi = q[5] + t[5] * lvl / 16 + t[5] * func_800647A0((lvl * t[5]) << 11) / 32768;
        hi -= lo;
        o = n = p[5];
        n += hi;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[5] = n;
        o = n - o;
        p[0x28] += o;

        i = t[4] * q[4];
        lo = q[4] + i * (lvl - 1) / 1024;
        hi = q[4] + i * lvl / 1024;
        o = n = p[4];
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[4] = n;
        o = n - o;
        p[0x25] += o;

        i = t[0] * q[0];
        o = n = p[0];
        lo = q[0] + i * (lvl - 1) / 64;
        hi = q[0] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[0] = n;
        o = n - o;
        p[0x26] += o;

        i = t[1] * q[1];
        o = n = p[1];
        lo = q[1] + i * (lvl - 1) / 64;
        hi = q[1] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[1] = n;
        o = n - o;
        p[0x27] += o;

        i = t[2] * q[2];
        n = p[2];
        lo = q[2] + i * (lvl - 1) / 64;
        hi = q[2] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[2] = n;

        i = t[3] * q[3];
        n = p[3];
        lo = q[3] + i * (lvl - 1) / 1024;
        hi = q[3] + i * lvl / 1024;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[3] = n;

        w = *(s16 *)(q + 6);
        hp = w + lvl * w + lvl * lvl * (w + t[6] * lvl) / 512;
        if (hp > 0xFFFF)
        {
            hp = 0xFFFF;
        }
        *(u16 *)(p + 6) = hp;
        if (p[0x13] != 0)
        {
            for (i = 2; i >= 0; i--)
            {
                e[i] = 0;
                k = p[i * 3 + 8];
                if (k != 0)
                {
                    u = D_8006DE24[k].unk_10 >> 4;
                    if ((u & *(u32 *)(p + 0x14)) != 0 || (u & 7) == 0)
                    {
                        if (p[i * 3 + 0xA] < 0x63)
                        {
                            p[i * 3 + 0xA] += 1;
                            e[i] = 1;
                            u2 = 0x64 - lvl;
                            hi = 0x63;
                            r = (hi - p[i * 3 + 0xA]) / u2 + p[i * 3 + 0xA];
                            if (r >= 0x64)
                            {
                                r = 0x63;
                            }
                            p[i * 3 + 0xA] = r;
                        }
                    }
                }
            }
        }
        func_80041E70(p);
        if (f != 0)
        {
            if ((*(u16 *)0x80013714 & 1) == 0)
            {
                D_800DCF4F = 1;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
            }
            func_800B4C7C((*(u32 *)(p + 0x14) & 0x2000) ? 0x8002 : 0x8003, p, -2, 1);
            m = func_800990FC();
            h = func_80099194(D_800E0953, m);
            h = (void *)func_8009929C(0xA, h);
            h = func_80099734(p, h);
            h = func_80099194(D_800E095F, h);
            h = func_8003AD08(p[0x11], h);
            h = func_80099194(D_80089000, h);
            if (p[0x13] != 0 && (*(u32 *)(p + 0x14) & 0x4000) != 0)
            {
                i = 0;
                d = (UnkDungeonName0 *)D_8006DE24;
                for (; i < 3; i++)
                {
                    if (e[i] != 0)
                    {
                        h = (void *)func_8009929C(0xA, h);
                        k = p[i * 3 + 8];
                        h = func_80099194(d[k].unk_00, h);
                        h = func_80099194(D_800E0970, h);
                        h = func_80099194(D_800E0979, h);
                    }
                }
            }
            if ((D_80013714 & 1) == 0)
            {
                h = (void *)func_8009929C(0x11, h);
                h = (void *)func_8009929C(0x4C, h);
                h = (void *)func_80099254(D_800E0458, h);
            }
            func_80099290(h);
            func_800A5720(m);
        }
    }
}

/* The dungeon "level down" twin of func_800A1D4C. 432 words, 0 differing.
   The second parameter is a SHORT (`sll $v0, $s6, 16' + `beqz'), not the
   s32 the unit declared.

   FOUR LEVERS, all allocation and scheduling rather than shape:

   * THE `t[x] * q[x]' PRODUCT HAS TO SHARE THE MESSAGE LOOP'S COUNTER `i'.
     The target holds it in `$s0' although it crosses no call; as its own
     `s32 r' it is a short allocno and global.c's find_reg takes $a0/$v1
     from the call-clobbered end of reg_alloc_order. Written into `i', which
     crosses twelve calls, find_reg goes to the callee-saved end. 36 -> 17
     aligned differences. Six rewrites of the expression could not move it
     (fully inlined 42, product and high half reordered 56, explicit r1/r2
     temporaries +16 words, one whole-difference expression 86).

   * THE SPELL LOOP'S RULE INDEX IS A LOCAL OF ITS OWN THAT IS ALSO READ IN
     THE MESSAGE LOOP (`k = p[i * 3 + 8]; h = func_80099194(d[k].unk_00, h);'
     in place of the inlined subscript). 2 -> 0. This is the whole residue
     the park was left on: the target loads it into `$a0' and loop.c's
     induction pointer into `$a1', and this candidate had them the other way
     round. The reason is a hard-register conflict, read out of `.greg':
     with the index written into the class local `c' the allocno is live
     across the `sll $a0, $v1, 11' that sets up func_800647A0's argument at
     word 38, so `c' conflicts with hard reg 4 and find_reg skips to $a1;
     with a fresh local used only in the spell loop the allocno conflicts
     with $v0 alone and find_reg takes $v1, which shifts every register in
     the loop. Reading the SAME local in the message loop adds the $v1
     conflict without adding an $a0 one -- `k' dies building the argument
     address, so it crosses no call -- and $a0 is the first register left.
     Measured with the index in `c' 10, `hi' 14 (and `hi' then takes $a0 in
     all six stat blocks, 12 rows), a fresh local used only in the spell
     loop 17, `u' 17 / 30 asm lines, `w'/`n'/`hp'/a second fresh local 17,
     `lvl' and `lo' over 100.

   * `i = 0;' BEFORE `d = (UnkDungeonName0 *)D_8006DE24;', with the message
     loop then written `for (; i < 3; i++)'. 2 -> 0, and the last two words.
     `addu $s0, $zero, $zero' and `addiu $s2, $v0, %lo(D_8006DE24)' are
     independent, so sched1 orders them by INSN_LUID and nothing downstream
     can recover the swap. Assigning `d' inside the loop body and letting
     loop.c hoist it measures the same.

   * `(*(u16 *)0x80013714 & 1) == 0' FOR THE FIRST OF THE TWO TESTS. The
     declared `D_80013714' lets reorg lift the `lui %hi' into the
     `beqz $v0, .L800A2AB8' delay slot; the target's slot holds the
     `addiu $v0, $zero, 0x1' reorg COPIES out of the shared `return 1'
     block, which a constant address's single rtl insn leaves it free to
     take. 431 -> 432 words and 149 -> 36 differing. The SECOND test
     measures identically either way, so it is left as the symbol.

   Also measured and rejected: the p[0..3] loads read after the `hi' line
   (431/202), `lo' used as the product with `hi' computed first (432/56),
   the product fully inlined twice (432/42), explicit product temporaries
   (448 words), the whole difference as one expression with no lo/hi locals
   (432/86), and five spellings of the `return 1' itself (all 431 words --
   the delay slot is the only control there is). */
s32 func_800A2424(u8 *p, s16 f)
{
    u8 *t;
    u8 *q;
    u8 *g;
    void *m;
    void *h;
    UnkDungeonName0 *d;
    s32 lvl;
    s32 c;
    s32 k;
    s32 i;
    s32 w;
    s32 u;
    s32 lo;
    s32 hi;
    s32 n;
    u32 hp;
    u8 e[3];

    if (p[0x11] < 2)
    {
        return 0;
    }
    p[0x11] -= 1;
    lvl = p[0x11];
    c = p[0x13];
    *(u32 *)(p + 0x18) = D_800835E8[lvl] - 1;
    t = D_800DDCBC[c];
    q = (u8 *)&D_8006D168[c];
    lo = q[5] + t[5] * (lvl - 1) / 16 + t[5] * func_800647A0(((lvl - 1) * t[5]) << 11) / 32768;
    hi = q[5] + t[5] * lvl / 16 + t[5] * func_800647A0((lvl * t[5]) << 11) / 32768;
    n = p[5] + (lo - hi);
    if (n == 0)
    {
        n = 1;
    }
    p[5] = n;

    i = t[4] * q[4];
    lo = q[4] + i * (lvl - 1) / 1024;
    hi = q[4] + i * lvl / 1024;
    n = p[4] + (lo - hi);
    if (n == 0)
    {
        n = 1;
    }
    p[4] = n;

    i = t[0] * q[0];
    n = p[0];
    lo = q[0] + i * (lvl - 1) / 64;
    hi = q[0] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[0] = n;

    i = t[1] * q[1];
    n = p[1];
    lo = q[1] + i * (lvl - 1) / 64;
    hi = q[1] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[1] = n;

    i = t[2] * q[2];
    n = p[2];
    lo = q[2] + i * (lvl - 1) / 64;
    hi = q[2] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[2] = n;

    i = t[3] * q[3];
    n = p[3];
    lo = q[3] + i * (lvl - 1) / 1024;
    hi = q[3] + i * lvl / 1024;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[3] = n;

    w = *(s16 *)(q + 6);
    hp = w + lvl * w + lvl * lvl * (w + t[6] * lvl) / 512;
    if (hp > 0xFFFF)
    {
        hp = 0xFFFF;
    }
    *(u16 *)(p + 6) = hp;
    if (p[0x13] != 0)
    {
        for (i = 2; i >= 0; i--)
        {
            e[i] = 0;
            k = p[i * 3 + 8];
            if (k != 0)
            {
                u = D_8006DE24[k].unk_10 >> 4;
                if ((u & *(u32 *)(p + 0x14)) != 0)
                {
                    if (p[i * 3 + 0xA] >= 2)
                    {
                        p[i * 3 + 0xA] -= 1;
                        e[i] = 1;
                    }
                }
            }
        }
    }
    func_80041E70(p);
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
    if (p[0x66] < p[0x25])
    {
        p[0x25] = p[0x66];
        p[0x24] = 0xFF;
    }
    if (p[0x68] < p[0x26])
    {
        p[0x26] = p[0x68];
    }
    if (p[0x69] < p[0x27])
    {
        p[0x27] = p[0x69];
    }
    if (f != 0)
    {
        if ((*(u16 *)0x80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
        }
        func_800B4C7C(0x8003, p, -3, 1);
        func_800A56E0(0x615);
        m = func_800990FC();
        h = func_80099194(D_800E098B, m);
        h = (void *)func_8009929C(0xA, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800E099A, h);
        h = func_8003AD08(p[0x11], h);
        h = func_80099194(D_80089000, h);
        if (p[0x13] != 0 && (*(u32 *)(p + 0x14) & 0x4000) != 0)
        {
            i = 0;
            d = (UnkDungeonName0 *)D_8006DE24;
            for (; i < 3; i++)
            {
                if (e[i] != 0)
                {
                    h = (void *)func_8009929C(0xA, h);
                    k = p[i * 3 + 8];
                    h = func_80099194(d[k].unk_00, h);
                    h = func_80099194(D_800E09B2, h);
                    h = func_80099194(D_800E09BB, h);
                }
            }
        }
        if ((D_80013714 & 1) == 0)
        {
            h = (void *)func_8009929C(0x11, h);
            h = (void *)func_8009929C(0x4C, h);
            h = (void *)func_80099254(D_800E0458, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    return 1;
}

/* 8 words, 0 differing. */
void func_800A2AE4(void)
{
    u8 *g;

    D_800DCF4F = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

/* Tile (x, z) to the halfword world coordinates at +0x2 and +0x6. The
   `(s16)` casts are what give the `sll 16`/`sra 10` pairs, and neither
   field is an UnkVec member, so both go in as offsets.
   9 words, 0 differing. */
void func_800A2B04(UnkVec *p, s32 x, s32 z)
{
    *(s16 *)((u8 *)p + 0x2) = ((s16)x) * 64 + 0x20;
    *(s16 *)((u8 *)p + 0x6) = ((s16)z) * 64 + 0x20;
}

/* func_80094EA4's shape over two different globals, nested for the same
   reason. 13 words, 0 differing. */
s32 func_800A2B28(void)
{
    if ((D_80013714 & 2) == 0)
    {
        if (D_800E3D20 != 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Claim D_80083460 + 0xC for the caller's object unless anything else is
   pending, then report func_800A2B28's answer. ONE `||` CHAIN, NOT FOUR
   `if`s, for the same reason func_800A2C34 above records: each test's
   `addiu $v0, $zero, 1` is reorg duplicating ONE `return 1` into four delay
   slots. 32 words, 0 differing. */
s32 func_800A2B5C(void *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(void **)(g + 0xC) != d)
    {
        if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0 ||
            (*(u16 *)(g + 0x2) & 8))
        {
            return 1;
        }
        *(void **)(g + 0xC) = d;
    }
    return (s16)func_800A2B28();
}

/* Four engine flags, any of which blocks whatever the caller is about to do.

   A COMMON RESULT VARIABLE, NOT FOUR EARLY `return 1`s. Written as four
   `if (...) return 1;` and a trailing `return 0`, jump.c store-flags the LAST
   pair into `j` + `sltu $v0, $zero, $v0` and the fourth `bnez` is gone: 22
   words, 5 differing. An `&&` chain whose fall-through returns 0 is the same
   5, and so is a `goto`-shared `return 1`. Only the explicit if/else over one
   variable keeps all four branches -- which CORRECTS the escape the
   store-flag lever records. 22 words, 0 differing. */
/* ONE PARAMETER, unread. The body never touches $a0, which is why `void`
   had never been falsified -- but all five call sites set it in the `jal`'s
   delay slot (func_80096100 and func_8009345C pass $zero, func_800A9E70
   passes $s1, func_800B253C passes $s0, func_800AA36C passes its fourth
   argument). 22 words, 0 differing either way. */
s32 func_800A2BDC(u8 *a)
{
    u8 *g;
    s32 r;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s32 *)(g + 0x8) != 0 ||
        (*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        r = 1;
    }
    else
    {
        r = 0;
    }
    return r;
}

/* ONE `||` CHAIN, NOT THREE `if`s: separate returns are 17 words / 8
   differing because jump.c's store-flag transform turns the last pair into
   `sltu`. Sharing one `return 1` lets reorg duplicate the `addiu $v0, 1`
   into all three delay slots. 17 words, 0 differing. */
/* ONE PARAMETER, unread. Across its four ASM callers the setup is four
   `addu $a0, $zero, $zero` and two `addu $a0, $sN, $zero`, so it is real and
   it is a pointer. 17 words, 0 differing either way. Every future landing of
   one of those callers has to pass it. */
s32 func_800A2C34(u8 *q)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0)
    {
        return 1;
    }
    return 0;
}

/* Two accesses off D_80083460, so a base pointer. The result accumulating
   in $v1 and moving to $v0 at the `jr` is the `||`. 13 words, 0 differing. */
/* ONE PARAMETER, unread: func_800CB170 sets $a0 in the `jal`'s delay slot
   and the now-landed func_800AB778 does too. 13 words, 0 differing
   either way. */
s32 func_800A2C78(u8 *p)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    return *(s32 *)(g + 0xC) != 0 || *(s16 *)(g + 0xA) != 0;
}

/* 3 words, 0 differing. */
s32 func_800A2CAC(u8 *p)
{
    return p[0x25] < 0x40;
}

/* THE TAIL'S XOR MUST LAND IN AN `s32` LOCAL FIRST. Written as one
   expression, the s16 return's truncation is folded back into both loads and
   they come out `lhu` -- 44 words, 2 differing. The local keeps the two `lw`
   and leaves the narrowing as the trailing `sll`/`srl` pair. 44 words,
   0 differing. */
s16 func_800A2CB8(u8 *a, u8 *b)
{
    s32 t;

    if (b == NULL)
    {
        return 0;
    }
    if ((*(u32 *)(a + 0x1C) & 0x410) != 0)
    {
        return 1;
    }
    if (a[0x13] == 0x2B && (*(u16 *)(a + 0x98) & 0x8000) != 0)
    {
        return 1;
    }
    if ((*(u32 *)(b + 0x14) & 0x4000) != 0 && (*(u32 *)(a + 0x14) & 0x4000) != 0)
    {
        return (s16)func_80042900((s8 *)a, 0xC) != 0;
    }
    t = *(u32 *)(a + 0x1C) ^ *(u32 *)(b + 0x1C);
    return t & 0x2000;
}

/* The read and the add are SEPARATE STATEMENTS: folded into one expression
   the `or`'s result takes $v0 instead of reusing the low byte's $v1.
   20 words, 0 differing. */
void func_800A2D68(u8 *p, u16 n)
{
    u32 v;

    v = (p[0x41] << 8) | p[0x42];
    v += n;
    if (v > 0xFFFF)
    {
        v = 0xFFFF;
    }
    p[0x41] = v >> 8;
    p[0x42] = v;
    D_800E296C |= 0x100000;
}

/* Split the actor's +0x06 damage figure across the party: double it once per
   flag on the target's +0x54 and once per flag on its own, count the live
   D_800E3D7C +0xAC slots, divide rounding up, and then either hand each live
   slot its share through func_800B4C7C/func_800A2D68 or -- when the +0x60
   occupant is not 0x4000-flagged -- give the whole figure to that occupant.
   138 words, 0 differing.

   ONE TRAILING `return v`, WITH THE WHOLE BODY NESTED. Written as two early
   `return v;` exits the two zero-returns compile to `move $v0, zero` in
   their own delay slots instead of sharing .L800A2FB8's `addu $v0, $s3`,
   and `v = 0` is constant-propagated away: 138 words, 3 differing.

   THE VARIABLE SHIFT NEEDS ITS OWN LOCAL. `w = v + (v >> (d > 1));` written
   inline is 141 words / 76 differing -- gcc specialises the boolean shift
   count into a branch and two shifts where the target has
   `slti`/`xori`/`srav`. Bound to an `s32 sh` first it is 138 / 4, and that
   one change also stops D_800E3D7C being loaded twice in the tail.

   `n = 1;` sits AFTER the +0x40 test, not before it: in the same basic block
   as the loop's `i = 1` cse turns the second constant into
   `addu $s1, $a1, $zero`, which is 1 differing the other way. `i = n` and
   `i = 1` are a tie. */
s32 func_800A2DB8(u8 *p)
{
    u8 *q;
    u8 *s;
    s32 v;
    s32 n;
    s32 i;
    s32 d;
    s32 w;
    s32 t;
    s32 sh;

    v = 0;
    q = *(u8 **)(p + 0x60);
    if (q != NULL && *(s8 *)(q + 0x13) >= 0)
    {
        v = *(u16 *)(p + 0x6);
        if (*(u32 *)(q + 0x54) & 0x20)
        {
            v *= 2;
        }
        if (*(u32 *)(p + 0x54) & 0x40)
        {
            v *= 2;
        }
        n = 1;
        for (i = 1; i >= 0; i--)
        {
            s = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
            if (s != NULL && s[0x28] != 0)
            {
                n++;
            }
        }
        v = (v + n - 1) / n;
        if (*(u32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x4000)
        {
            for (i = 1; i >= 0; i--)
            {
                s = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
                if (s != NULL && s[0x28] != 0)
                {
                    d = s[0x11] - p[0x11];
                    w = v;
                    if (d < 0)
                    {
                        sh = d > 1;
                        w = v + (v >> sh);
                    }
                    func_800B4C7C(0x82, s, (s16)w, 1);
                    func_800A2D68(s, w);
                }
            }
            if (D_800E3D7C[0x28] == 0)
            {
                return 0;
            }
            d = D_800E3D7C[0x11] - p[0x11];
            w = v;
            if (d < 0)
            {
                sh = d > 1;
                w = v + (v >> sh);
            }
            func_800A2D68(D_800E3D7C, w);
            func_800B4C7C(0x82, D_800E3D7C, (s16)w, 1);

            return w;
        }
        v = *(u16 *)(p + 0x6);
        func_800A2D68(*(u8 **)(p + 0x60), v);
        t = (*(u32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x2000) ? 0x82 : 0x83;
        func_800B4C7C(t, *(u8 **)(p + 0x60), *(s16 *)(p + 0x6), 1);
    }

    return v;
}

/* The "what did I just bump into" announcer. Run func_800A2DB8 unless the
   record and its +0x60 occupant are both 0x4000-flagged, then post up to two
   messages off func_800990FC's handle: the plain one when the record itself
   is not 0x4000-flagged, and -- if func_800A2DB8 returned a count -- either
   the D_8007359C[0] name line or the +0x60 occupant's own, each closing on
   func_8003AD08 and a trailing string. The two arms CROSS-JOIN onto the last
   func_80099194, which is why that call is written in both.

   TWO SPELLINGS ARE LOAD-BEARING, 10 differing between them.

   (1) The two EARLY +0x60 reads are written INLINE, not through the `q`
   local the later ones use. A single named local for all three is one
   quantity and takes $v1 everywhere; the target has $v0 for the first two
   and $v1 for the third. Worth 8 differing.

   (2) THE 0x4000 MASK NEEDS ITS OWN STATEMENT. `ok = (flags & 0x4000) != 0;`
   lets combine rewrite the test to `(flags >> 14) & 1` -- `srl $a0, $v0,
   0xE` / `andi $a0, $a0, 1` where the target has `andi $v0, $v0, 0x4000` /
   `sltu $v1, $zero, $v0`. A `(u32)` mask and a `? 1 : 0` are both still
   2 differing; only binding the masked value to its own local first keeps
   the `andi`. That is lever 16, "`flags & (1 << i)` needs the mask in a
   local", on a test rather than on a loop. `ok = flags & 0x4000;` without
   the comparison is 123 words / 108 differing.

   124 words, 0 differing. */
void func_800A2FE0(u8 *p)
{
    u8 *q;
    void *m;
    u8 *h;
    s32 r;
    s32 ok;
    s32 v;

    r = 0;
    ok = 0;
    if (*(u8 **)(p + 0x60) != NULL && (*(s32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x4000) != 0)
    {
        v = *(s32 *)(p + 0x14) & 0x4000;
        ok = v != 0;
    }
    if (!ok)
    {
        r = func_800A2DB8(p);
    }
    if ((*(s32 *)(p + 0x14) & 0x20000000) != 0)
    {
        return;
    }
    if ((*(s32 *)(p + 0x14) & 0x4000) == 0)
    {
        if (*(u8 **)(p + 0x60) != NULL && *(s8 *)(*(u8 **)(p + 0x60) + 0x13) >= 0)
        {
            m = func_800990FC();
            h = func_80099194(D_800E09CD, m);
            h = func_80099734(p, h);
            h = func_80099194(D_80089000, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    if (r == 0)
    {
        return;
    }
    q = *(u8 **)(p + 0x60);
    if (q == NULL)
    {
        return;
    }
    if ((*(s32 *)(q + 0x14) & 0x4000) != 0)
    {
        if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            return;
        }
        m = func_800990FC();
        h = func_80099194((u8 *)D_8007359C[0]->unk_04, m);
        h = func_80099194(D_800E09D9, h);
        h = func_8003AD08(r, h);
        h = func_80099194(D_800E09E6, h);
    }
    else
    {
        if (*(s8 *)(q + 0x13) < 0)
        {
            return;
        }
        m = func_800990FC();
        h = func_80099734(*(u8 **)(p + 0x60), m);
        h = func_80099194(D_800E09EE, h);
        h = func_8003AD08(r, h);
        h = func_80099194(D_800E09FB, h);
    }
    func_80099290(h);
    func_800A5720(m);
}

/* Walks the engine's node list and, for every kind-0x1E node whose +0xA4
   owner is this record, bumps two counters and sets a request bit.
   53 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, and they fix OPPOSITE halves of one
   scheduling problem -- the 52-word plateau two earlier attempts and a
   permuter run all sat on.

   (1) `__asm__("")' BETWEEN THE TWO COUNTER INCREMENTS -- the missing word.
   A traditional asm (ASM_INPUT, no operands) makes sched.c flush every
   pending read/write list and depend on all of memory (sched.c:1943-1970),
   so the D_800DCE68 load cannot be hoisted into the load-delay slot of
   `lhu $v0, 0xA($a3)' and the target's bare `nop' survives.  Nothing else
   reaches it: `*(u16 *)(g + 0xA) += 1' and `D_800DCE68 += 1' are both
   non-struct refs whose addresses canon_rtx resolves to distinct symbols
   (a once-set `g = (u8 *)&D_80083460' carries a REG_EQUIV), so
   memrefs_conflict_p proves them disjoint and true_dependence's
   MEM_IN_STRUCT_P clauses -- which only ever CLEAR a dependence -- never
   get the chance to fire.  It emits no instruction.  Without it, with
   everything else below: 52 words, 9 aligned.

   (2) THE THREE RECORD STORES AS STRUCT MEMBERS, not `*(s16 *)(n + 0x6E)'
   casts -- the second half.  COMPONENT_REF sets MEM_IN_STRUCT_P, and with
   the varying `n' base that is exactly true_dependence's exemption against
   the fixed, non-struct `sh %lo(D_800DCE68)', so `lw $v1, 0x1C($a1)' moves
   up into that store's own delay slot as the target has it.  As casts the
   load is pinned below the store and the two counter `lhu's interleave
   instead: 52 words, 17 aligned.

   The two together are what land it; the barrier ALONE (casts kept) is
   54 words / 16 aligned, because the record load then cannot fill the
   second slot either and a second `nop' appears.

   Also load-bearing, from the park and re-confirmed here: `c - 0x20'
   written INLINE in the comparison rather than through a local (a `d'
   local is 4 differences worse at the same length), and
   `g = (u8 *)&D_80083460' assigned INSIDE the loop body immediately before
   its use -- before the loop or before the `if' it is 51 words, because
   the `lui' leaves the branch delay slot the target fills it from.  loop.c
   hoists the pair to the preheader by itself. */
void func_800A31D0(u8 *c)
{
    u8 *g;
    u8 *n;

    if (c[0x13] == 0x1E && (*(s32 *)(c + 0x14) & 0x4000) != 0 && *(s16 *)(c + 0xAA) != 0)
    {
        n = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
        while (n != D_800E3D7C)
        {
            if (n[0x13] == 0x1E && *(u8 **)(n + 0xA4) == c - 0x20)
            {
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                __asm__("");
                D_800DCE68 += 1;
                ((UnkDungeonA31D0 *)n)->unk_6E = 0;
                ((UnkDungeonA31D0 *)n)->unk_6D = 0;
                ((UnkDungeonA31D0 *)n)->unk_1C |= 0x400000;
            }
            n = *(u8 **)(n + 0x5C) + 0x20;
        }
    }
}

/* Retire the record `p` from the party: drop its 0x1A/0x1C/0x1D attributes,
   hand it to func_800A31D0, and if it still owns one of the four +0xAC slots,
   clear the slot, copy the live record into its D_800E3E48 shadow and point
   D_800E3DF0 at the copy -- func_80093E74 at :3068 does the same shadow copy
   from the other side. The 0/1/2/3 code the walk ends with is what
   func_800A18E8 is told, and the slot record it found is returned.

   THE RETURN VALUE IS REAL: the two `addu $v0, $s2, $zero` at the tail are
   `return r;`, so the unit's `void func_800A32A4(u8 *);` declaration had to
   become `u8 *`. All seven landed call sites drop the result and are
   unchanged.

   TWO ALLOCATION FACTS, 12 differing between them, and neither is reachable
   by moving statements.

   (1) THE TWO IDENTICAL D_80083460 ARMS NEED TWO DIFFERENT LOCALS. With one
   `g` written in both arms the base lands in $v1 and the `lhu` result in $a0,
   where the target has $a0 and $v1 -- 10 differing across the ten words. A
   second local for the second arm changes the quantity numbering and both
   arms come out right. (The base itself must still be re-assigned per arm:
   the target has one `lui` in the `beqz`'s delay slot and an `addiu %lo` in
   each arm.)

   (2) `f` IS `s16`. As `s32` the flag and `r` tie, reload_cse rewrites
   `r = NULL` into `addu $s2, $s1, $zero` and the loop's `i = 0` then copies
   $s1 instead of $s2: 2 differing. The width is invisible at the one use --
   func_800A18E8's second argument is a plain `addu $a1, $s1, $zero` either
   way.

   Measured and neutral: the decrement as `-= 1`, `--`, or written out with
   the `lhu` read spelled explicitly; `g` as `s16 *` with `g[14]`; every
   declaration order of the six locals; `r = NULL` before `f = 0` (15
   differing) and either of them moved between the func_80042B68 calls (16,
   129, 131).

   134 words, 0 differing. */
u8 *func_800A32A4(u8 *p)
{
    u8 *g;
    u8 *g2;
    u8 *r;
    s32 i;
    s32 k;
    s16 f;
    u32 w;

    f = 0;
    r = NULL;
    func_80042B68((s8 *)p, 0x1A);
    func_80042B68((s8 *)p, 0x1C);
    func_80042B68((s8 *)p, 0x1D);
    if ((*(u32 *)(p + 0x14) & 0x20000000) == 0)
    {
        func_800A31D0(p);
        for (i = 0; i < 4; i++)
        {
            if (*(u8 **)(D_800814A8 + i * 4 + 0xAC) == p)
            {
                *(u32 *)(p + 0x14) &= ~0x4000;
                *(u32 *)(D_800814A8 + i * 4 + 0xAC) = 0;
                w = *(u32 *)(p + 0x1C);
                w |= 0x4000000;
                w |= 0x20000;
                *(u32 *)(p + 0x1C) = w;
                r = *(u8 **)(D_800814A8 + i * 4 + 0xD0);
                r[3] &= 0xDF;
                f = 3;
                *(u32 *)(D_800814A8 + i * 4 + 0xD0) = 0;
                k = r[3] & 0x1F;
                D_800E3DF0[k] = (s32)&D_800E3E48[k];
                D_800E3E48[k] = *(Unk800E3E48 *)p;
                break;
            }
        }
    }
    if (f == 0)
    {
        if ((*(u32 *)(p + 0x14) & 0x4000) != 0)
        {
            f = 3;
        }
        else if ((*(u32 *)(p + 0x14) & 0x2000) != 0)
        {
            g = (u8 *)&D_80083460;
            if (*(s16 *)(g + 0x1C) != 0)
            {
                *(s16 *)(g + 0x1C) -= 1;
            }
            f = 2;
        }
        else
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0x1C) != 0)
            {
                *(s16 *)(g2 + 0x1C) -= 1;
            }
            f = 1;
        }
    }
    if ((*(u32 *)(p + 0x14) & 0x20000000) == 0)
    {
        func_800A18E8(p[0x13], f);
    }
    return r;
}

/* Copy the twelve-byte name the D_8007359C record at +0x04 points at into
   the actor's +0x34, then drop a NUL on its last word's first byte. The
   copy's type has to have ALIGNMENT 1 -- see UnkDungeonName12 -- which is
   what gives the `lwl`/`lwr` + `swl`/`swr` pairs rather than aligned `lw`.
   23 words, 0 differing. */
void func_800A34BC(u8 *p)
{
    *(UnkDungeonName12 *)(p + 0x34) = *(UnkDungeonName12 *)D_8007359C[0][p[0x13]].unk_04;
    p[0x3C] = 0;
}

/* A capacity predicate: true once +0x29 over half of +0x11 plus two has
   caught up with +0x28.

   THE DIVISOR IS ITS OWN STATEMENT -- the target loads +0x11 before +0x29,
   which is the divisor's expression first; inline, the dividend goes first.
   And `>> 1`, not `/ 2`: the value comes out of an `lbu`, so combine knows
   the sign bit is clear and the shift stays the `srl` the target has, where
   a signed divide by two costs three words. 11 words, 0 differing. */
s32 func_800A3518(u8 *p)
{
    s32 d;

    d = (p[0x11] >> 1) + 2;
    return p[0x29] / d >= p[0x28];
}

/* func_800A35A4's record lookup as a scan over three stride-3 slots.

   TWO PREHEADER FACTS, each worth measuring: the subscript `p[i * 3 + 8]` is
   what loop.c reduces to the target's `addiu $a0, $a0, 0x3` biv, while
   walking `p += 3` myself leaves the giv's own `addiu $a0, $a0, 8` in the
   preheader and is 25 words / 24 differing; and `t` has to be written INSIDE
   the loop, because the target's preheader sets `$a2` (i) before the
   `%hi(D_8006DE24)`, which the hoist reproduces and an assignment ahead of
   the `for` does not (24 words / 3 differing, the three being just those two
   insns and the `j`). Hoisting `p[8]` into a local is 24 / 10.
   24 words, 0 differing. */
s16 func_800A3544(u8 *p, u8 k)
{
    UnkDungeonName *t;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        t = (UnkDungeonName *)D_8006DE24;
        if (p[i * 3 + 8] != 0 && t[p[i * 3 + 8]].unk_11 == k)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* func_800A3820's +0x13 read with the index taken out of a record. THE
   TABLE ARRIVES AS A POINTER LOCAL, not as a symbol added to the index:
   added, it expands SECOND and the `lui` lands after the multiply, which is
   what func_800A3820 wants and this one does not (9 differing). `p += i *
   3;` as its own statement is what keeps the update in $a0.
   13 words, 0 differing. */
u8 func_800A35A4(u8 *p, s32 i)
{
    UnkDungeonName *t;

    t = (UnkDungeonName *)D_8006DE24;
    p += i * 3;
    return t[p[8]].unk_13;
}

/* A three-bit cross-product: for each bit of `a`, the bit one place round the
   3-cycle of `b` adds and the bit one place the other way subtracts.

   THE PARAMETER WIDTH IS WORTH TWO WORDS. Declared `s32`, gcc keeps both
   parameters in $a0/$a1, the first block's `sltu` writes straight into the
   counter, and the function is 31 words. Declared sub-word, the two incoming
   copies `addu $a3, $a0, $zero` / `addu $a2, $a1, $zero` survive (a sub-word
   copy of a word register is not the no-op jump2 deletes) and all 33 words
   match. u8, s8, u16 and s16 all measure 33 / 0 here, so the callee cannot
   pick between them; u16 is what the CALLERS witness -- func_800A40AC passes
   an `lhu` result with no mask, which rules out u8, and with no `sll`/`sra`,
   which rules out s16. 33 words, 0 differing. */
s16 func_800A35D8(u16 a, u16 b)
{
    s16 r;

    r = 0;
    if (a & 1)
    {
        if (b & 4)
        {
            r++;
        }
        if (b & 2)
        {
            r--;
        }
    }
    if (a & 2)
    {
        if (b & 1)
        {
            r++;
        }
        if (b & 4)
        {
            r--;
        }
    }
    if (a & 4)
    {
        if (b & 2)
        {
            r++;
        }
        if (b & 1)
        {
            r--;
        }
    }
    return r;
}

/* Two tile positions are "in line" when their x and y distances are equal or
   either one is zero.

   BOTH abs() ARE THE `bgez`/nop/`negu` TRIO, and the spelling matters: the
   TERNARY form `dx = dx >= 0 ? dx : -dx;` writing back into its own variable
   is 22 words / 14 differing, every difference a register. Written into a
   SEPARATE variable the ternary is 0, and `abs()` inline is 0 and the same
   bytes. Computing both differences before either abs is one word SHORT: the
   second pair of `lbu` fills the first subtraction's load-delay slot, which
   the target leaves as a `nop`. 22 words, 0 differing. */
s32 func_800A365C(u8 *a, u8 *b)
{
    s32 dx;
    s32 dy;
    s32 r;

    dx = abs(a[0x24] - b[0x24]);
    dy = abs(a[0x25] - b[0x25]);
    r = 0;
    if (dx == dy || dx == 0 || dy == 0)
    {
        r = 1;
    }
    return r;
}

/* Walk the tile line from a's record to b's one step at a time and ask
   func_800A44E0 whether each step is blocked; the running ceiling at +0x88
   is lowered by whatever func_800BCB04 reports under each tile.
   91 words, 0 differing on the first compile.

   The three spellings it needed are all func_800A44E0's own, recorded at its
   landing: the third argument is `s16` and is sign-extended at the call (the
   note there names this function as one of its two witnesses), `z` is the
   `u16` read of +0x88 held in an `s16`, and the `(s16)(z - 0x20)` cast comes
   from func_800BCB04's prototype. */
s32 func_800A36B4(u8 *a, u8 *b)
{
    u8 *sa;
    u8 *sb;
    s32 dx;
    s32 dy;
    s32 x;
    s32 y;
    s16 z;
    s16 w;
    s16 lim;
    s16 h;

    sa = *(u8 **)(a - 0x14);
    sb = *(u8 **)(b - 0x14);
    w = *(u16 *)(a + 0x2A);
    dx = sa[0x24] - sb[0x24];
    if (dx != 0)
    {
        if (dx > 0)
        {
            dx = 1;
        }
        else
        {
            dx = -1;
        }
    }
    dy = sa[0x25] - sb[0x25];
    if (dy != 0)
    {
        if (dy > 0)
        {
            dy = 1;
        }
        else
        {
            dy = -1;
        }
    }
    x = sa[0x24];
    y = sa[0x25];
    z = *(u16 *)(a + 0x88);
    while (x != sb[0x24] || y != sb[0x25])
    {
        lim = z;
        if ((s16)func_800A44E0((x << 6) & 0xFFC0, (y << 6) & 0xFFC0, lim, w) != 0)
        {
            return 0;
        }
        x -= dx;
        y -= dy;
        h = func_800BCB04(((x << 6) + 0x20) & 0xFFE0, ((y << 6) + 0x20) & 0xFFE0,
                          (s16)(z - 0x20));
        if (h < lim)
        {
            z = h;
        }
    }
    return 1;
}

/* Read +0x13 out of the D_8006DE24 record; see UnkDungeonName above. The
   index is a separate statement, or fold hands the symbol to expand first
   and the `lui` comes out ahead of the multiply. 11 words, 0 differing. */
u8 func_800A3820(s16 i)
{
    s32 off;

    off = (s16)i * 0x14;
    return ((UnkDungeonName *)(off + (u8 *)D_8006DE24))->unk_13;
}

/* Pick which of the actor's three 3-byte D_8006DE24 rules it should react to
   `b` with: gate on the two +0x88 heights being within 0x40, aim through
   func_800A0818, latch the +0x14/+0x98 "noticed" flags once, then scan the
   three slots from D_8008347E's low two bits, keeping the rule whose
   func_800A35D8 score beats the best so far -- and, for rule kind 1, only if
   an eight-way walk of `dist` steps from the actor reaches `b` without
   crossing a blocking tile. 307 words, 0 differing.

   EVERY FAILED TEST IS A `continue`, AND THE TAIL IS WRITTEN ONCE. The
   target's two dispatch arms both reach `bestt = t; best = i + 1;` at
   .L800A3C40, and the copy in the `j`'s delay slot is reorg's
   fill_slots_from_thread, not a second source statement. Written with the
   tail duplicated in both arms the extra reference lifts `t`'s allocno above
   `dist`'s, `t` takes a callee-saved register, `dist` spills to the frame and
   the function is 315 words / 126 aligned differences; with the shared tail
   it is 307 / 11.

   THE FIRST PARAMETER IS COPIED INTO A LOCAL USED FOR -0x14 AND +0x98 ONLY.
   The target loads 0x68($sp) twice in a row, into $t1 for the +0x88 read and
   into $s1 for `*(u8 **)(h - 0x14)` and the five +0x98 accesses; all the
   other uses reload from the home slot. Without the local the parameter wins
   $fp outright, `e` and `dist` both spill and it is 212 aligned differences
   against 163.

   THE FLAG WORD MUST BE A LOCAL DECLARED **BEFORE** `mm`, AND THAT ONE LINE
   IS THE LAST THREE WORDS. `fl2`, `mm` and `w` all measure refs 9 / length 4,
   so `allocno_compare` ties on all three and global.c falls through to
   `return v1 - v2` -- the allocno INDEX, which follows the pseudo number,
   which follows DECLARATION ORDER. Read as `*(u32 *)(a + 0x1C)` twice the
   value is a cse-created pseudo (242 here) and loses the tie to `mm` (94), so
   `mm` is allocated first and takes $v1; `fl2` declared above `mm` wins the
   tie, takes $v1, and `mm` gets the target's $a0. Thirty-one other spellings
   -- every local type, every declaration position, block scope, swapped arms,
   ternaries, a pointer read -- all sit at exactly 3 aligned differences.
   `fl = fl2 & 0x2000;` must also precede `mm = m;`, or the `lh` is scheduled
   ahead of the `andi` and reorg fills the gate's delay slot from the wrong
   thread (10 differences).

   `kind` IS AN `s32` LOCAL, WORTH ONE OPCODE. With `d->unk_12` read directly
   at the `>= 4` test combine folds the `lbu`'s zero-extension into the
   comparison and emits `sltiu`; the target has the signed `slti`.

   `j` IS `s16` AND STARTS AT `n`. The guard is `slt $s3, $fp` with no
   sign-extension because combine proves `n` already has 17 sign bits (it is
   `(s16)func_80042900(...)`), while the bottom test re-extends; `j = 0` gives
   `blez $fp` and `s32 j` loses the two `sll`/`sra` pairs. */
s32 func_800A384C(u8 *a, u8 *b, u8 *c, s32 s)
{
    UnkDungeonName10 *d;
    u8 *h;
    u8 *e;
    u8 *f;
    u8 *g;
    s16 m;
    s32 k;
    s32 n;
    s16 j;
    s16 w;
    s32 fl2;
    s32 mm;
    s32 fl;
    s32 off;
    s32 kind;
    s32 dist;
    s16 best;
    s16 bestt;
    s16 ang;
    s16 i;
    s16 t;
    s16 x;
    s16 y;
    s16 dir;

    best = -1;
    bestt = -0x100;
    *(s16 *)c = *(u16 *)(a + 0x2A);
    h = a;
    e = *(u8 **)(h - 0x14);
    f = *(u8 **)(b - 0x14);
    if (abs(*(s16 *)(a + 0x88) - *(s16 *)(b + 0x88)) >= 0x40)
    {
        return -1;
    }
    ang = func_800A365C(e, f);
    dist = func_8009FD40(f, e);
    *(s16 *)c = func_800A0818(e[0x24], e[0x25], f[0x24], f[0x25], (u16 *)&m);
    if (s == 0 && dist == 1)
    {
        if (func_8009A540((u16)(*(s16 *)c >> 9), e[0x24], e[0x25], *(u16 *)(a + 0x88) - 0x20) != 0)
        {
            return 8;
        }
    }
    if ((*(u32 *)(a + 0x14) & 0x10000) == 0)
    {
        *(u32 *)(a + 0x14) |= 0x10000;
        if (func_8009FD40(D_80082E80, e) < 0x11 &&
            ((*(u32 *)(a + 0x14) & 0x6000) != 0 || (*(u16 *)(h + 0x98) & 0x10) == 0 ||
             (func_800A6D30() & 0xF) == 0) &&
            (func_800A6D30() & 3) == 0 && (s16)func_80042900((s8 *)a, 6) == 0)
        {
            *(u16 *)(h + 0x98) |= 0x11;
        }
        else
        {
            *(u16 *)(h + 0x98) &= 0xFFFE;
        }
    }
    if (*(u16 *)(h + 0x98) & 1)
    {
        i = D_8008347E & 3;
        for (k = 0; k < 3; k++, i++)
        {
            if (i == 3)
            {
                i = 0;
            }
            g = a + i * 3;
            if (g[8] == 0)
            {
                continue;
            }
            off = g[8] * 0x14;
            d = (UnkDungeonName10 *)(off + (u8 *)D_8006DE24);
            if (d->unk_11 >= 0x12)
            {
                continue;
            }
            n = (s16)func_80042900((s8 *)b, d->unk_11);
            if (n != 0)
            {
                continue;
            }
            if (d->unk_13 < dist)
            {
                continue;
            }
            t = func_800A35D8(d->unk_10, *(u16 *)(a + 0x14));
            if (bestt >= t)
            {
                continue;
            }
            kind = d->unk_12;
            if (kind != 1)
            {
                if (kind == 0)
                {
                    continue;
                }
                if (kind >= 4)
                {
                    continue;
                }
            }
            else
            {
                if (ang == 0)
                {
                    continue;
                }
                x = e[0x24];
                y = e[0x25];
                dir = (*(u16 *)c >> 9) & 7;
                for (j = n; j < dist; j++)
                {
                    func_8009A350(x, y, dir, (u16 *)&m);
                    fl2 = *(u32 *)(a + 0x1C);
                    if ((fl2 & 0x410) == 0)
                    {
                        mm = m;
                        fl = fl2 & 0x2000;
                        if (fl != 0)
                        {
                            w = mm & 0x300;
                        }
                        else
                        {
                            w = mm & 0x3000;
                        }
                        if (w != 0)
                        {
                            break;
                        }
                    }
                    x += (u16)D_8006CCD8[dir];
                    y += (u16)D_8006CCE8[dir];
                }
                if (j < dist)
                {
                    continue;
                }
            }
            bestt = t;
            best = i + 1;
        }
    }
    if (s != 0 && dist == 1)
    {
        if (func_8009A540((u16)(*(s16 *)c >> 9), e[0x24], e[0x25], *(u16 *)(a + 0x88) - 0x20) != 0)
        {
            if (best < 0)
            {
                best = 8;
            }
        }
    }
    return best;
}

/* Pick the ring member `a` should react to: walk `b`'s ring, keep the nearest
   candidate that func_800A2CB8 and func_800A41F0 both admit, and score it two
   ways -- same `+0x26` class members by func_8009FD40's distance, different
   ones by func_8009FE94's line-of-sight test, which breaks the walk. Two
   candidates are tracked, `r4` for the one that IS the resident's current
   actor (D_800E3D7C) and `r3` for everything else, and the tail chooses
   between them.

   FOUR MEASURED FACTS.

   (1) `b` IS THE WALKER, as in func_8009B25C. `h = b; b = *(u8 **)(b + 0x5C)
   + 0x20;` gives the target's `addu $s0, $a1, $zero` / `addu $s2, $s0, $zero`
   pair; a separate `p` local for the walk and `b` kept as the head is 130
   words of 132 and 129 differing, because the parameter's own prologue copy
   disappears.

   (2) THE TAIL IS ONE `return r3` WITH FOUR `r3 = r4` SITES, not seven
   returns. Written as early `return r3` / `return r4` statements it is 132
   words and 35 differing: cross-jumping then produces a bare `addu $v0, $s4`
   where the target has `.L800A3EF0: addu $s3, $s4` falling into
   `.L800A3EF4: addu $v0, $s3`, and reorg copies that second insn into every
   early exit's delay slot.

   (3) THE `d6 == d1` ARM MUST BE WRITTEN BEFORE THE `d6 < d1` ONE. With
   `else if (d6 != d1) { if (d6 < d1) ... }` first, the equal case goes out of
   line and the compare comes out `beq` where the target has `bne`:
   17 differing at the same 132 words.

   (4) func_8009FE94 TAKES SIX ARGUMENTS HERE, not the five its landed
   definition declared -- the sixth is `*(s16 *)(b + 0x88)`, stored at
   0x14($sp), and the frame's 0x18 start proves the outgoing area is six words
   wide. The definition was widened with an unread sixth `s16`; it still
   measures 80 words, 0 differing, since an unread parameter costs nothing.

   132 words, 0 differing. */
u8 *func_800A3D18(u8 *a, u8 *b, s16 c)
{
    u8 *h;
    u8 *e;
    u8 *r4;
    u8 *r3;
    s16 d6;
    s16 d1;
    s16 t;

    r3 = NULL;
    r4 = r3;
    d6 = 0x100;
    h = b;
    if (c < 0)
    {
        d1 = -c;
    }
    else if (*(s8 *)(a + 0x26) < 0)
    {
        d1 = c;
    }
    else
    {
        d1 = 0x100;
    }
    b = *(u8 **)(b + 0x5C) + 0x20;
    if (b != h)
    {
        do
        {
            if ((s16)func_800A2CB8(h, b) != 0 && (s16)func_800A41F0(b) != 0)
            {
                e = *(u8 **)(b - 0x14);
                if (*(s8 *)(a + 0x26) == *(s8 *)(e + 0x26))
                {
                    t = func_8009FD40(e, a);
                    if (t < d1)
                    {
                        if (b == D_800E3D7C)
                        {
                            r4 = b;
                            d6 = t;
                        }
                        else
                        {
                            r3 = b;
                            d1 = t;
                        }
                    }
                }
                else if ((s16)func_8009FE94(a[0x24], a[0x25], *(s16 *)(h + 0x88), e[0x24], e[0x25],
                                            *(s16 *)(b + 0x88)) != 0)
                {
                    if (b == D_800E3D7C)
                    {
                        r4 = b;
                        d6 = 1;
                    }
                    else
                    {
                        r3 = b;
                        d1 = 1;
                    }
                    break;
                }
            }
            b = *(u8 **)(b + 0x5C) + 0x20;
        } while (b != h);
    }
    if (r4 != NULL)
    {
        if (r3 == NULL)
        {
            r3 = r4;
        }
        else if (d6 == d1)
        {
            if (*(u32 *)(r3 + 0x1C) & 8)
            {
                r3 = r4;
            }
            else if (((*(u32 *)(D_800E3D7C + 0x128) + b[0x43]) & 3) == 0)
            {
                r3 = r4;
            }
        }
        else if (d6 < d1)
        {
            r3 = r4;
        }
    }
    return r3;
}

/* Walk the ring from q looking for a neighbour that is either in the same
   room as (x, y) or standing within one tile of it; func_8009B25C is the
   same walk against a facing instead of a room. 73 words, 0 differing.

   THREE SPELLINGS WERE MEASURED, and two of them are not guessable.
   (1) The comparison is `r == e[0x26]`, NOT `e[0x26] == r`: with the operands
   the other way round every callee-saved register rotates by one -- 24
   differing with the whole structure already exact -- because the HImode
   extension of `r` is born in the other order.
   (2) `r` IS `s16`, so the call result's `sll`/`sra` sits at the first INT
   use, inside the `if`, where reorg can take the `sll` into the `beq`'s
   delay slot. As `s32` the extension is at the assignment and lands ahead of
   the `lw` -- 5 differing, no word change.
   (3) THE 1 IS A VARIABLE. `abs(d) > 1` folds to `d >= 2` and comes out as
   `slti $v0, $v0, 2`; the target's `slt $v0, $s2, $v0` needs the constant in
   a register, which only a local gives, and the local must be `s16`: as
   `s32` it takes a seventh callee-saved register and the function is 75
   words. */
u8 *func_800A3F28(u16 x, u16 y, u8 *h, u8 *q)
{
    u8 *p;
    u8 *e;
    s16 r;
    s16 lim;
    s16 xx;
    s16 yy;
    s32 d;

    r = func_8009FB34(x, y);
    p = *(u8 **)(q + 0x5C) + 0x20;
    if (p != h)
    {
        xx = x;
        lim = 1;
        yy = y;
        do
        {
            if (func_800A41F0(p) != 0)
            {
                e = *(u8 **)(p - 0x14);
                if (r == *(s8 *)(e + 0x26) && r >= 0)
                {
                    return p;
                }
                d = xx - e[0x24];
                d = abs(d);
                if (d <= lim)
                {
                    d = yy - e[0x25];
                    d = abs(d);
                    if (d <= lim)
                    {
                        return p;
                    }
                }
            }
            p = *(u8 **)(p + 0x5C) + 0x20;
        } while (p != h);
    }
    return NULL;
}

/* Is either of the engine's two slots holding a record whose +0x46 is flagged
   and whose low 14 bits sit in 5..7?

   THE RANGE TEST MUST BE PARENTHESISED. Written
   `(t & 0x8000) && (t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7` the `&&` chain
   associates left, fold never sees the two bounds as one range, and it is 25
   words against 24 with 15 differing -- two `sltiu`s instead of the target's
   `addiu -5`/`sltiu 3`. 24 words, 0 differing. */
s32 func_800A404C(void)
{
    s32 i;
    u8 *p;
    u8 *q;
    u16 t;

    for (i = 0, p = (u8 *)D_800814A8; i < 2; i++, p += 4)
    {
        q = *(u8 **)(p + 0xAC);
        if (q != NULL)
        {
            t = *(u16 *)(q + 0x46);
            if ((t & 0x8000) && ((t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7))
            {
                return 1;
            }
        }
    }
    return 0;
}

/* Pick the best of the record's three 3-byte slots, starting from
   D_8008347E's low two bits and wrapping at 3; the winner is the one with
   the highest +0x09, ties broken by func_800A35D8's score, and the answer
   is the slot index plus 5. 81 words, 0 differing.

   `q[9]` IS READ BEFORE THE CALL. It is live across it in $s0, and gcc will
   not move a load across a call, so reading it after is 11 differing. The
   two score locals are `s16`, which is what gives the tie-break its
   `sll`/`sll`/`slt` with no `sra`. */
s16 func_800A40AC(u8 *p, u16 arg)
{
    s32 n;
    s16 i;
    s16 best;
    s16 bestt;
    s16 ans;
    s16 t;
    s16 q9;
    u8 *q;

    ans = -1;
    bestt = -4;
    best = 0;
    i = D_8008347E & 3;
    for (n = 0; n < 3; n++, i++)
    {
        if (i == 3)
        {
            i = 0;
        }
        q = p + i * 3;
        if (q[8] != 0)
        {
            q9 = q[9];
            t = func_800A35D8(D_8006DE24[q[8]].unk_10, arg);
            if (best < q9)
            {
                best = q9;
                ans = i + 5;
                bestt = t;
            }
            else if (best == q9 && bestt < t)
            {
                bestt = t;
                ans = i + 5;
            }
        }
    }
    return ans;
}

/* Can this actor act? THE LAST TEST IS NARROW: `sll $v0, 16` / `sltiu $v0, 1`
   is the low halfword of func_80042900's result compared against zero, so the
   result takes an `(s16)` cast at the point of use rather than a changed
   prototype. 42 words, 0 differing. */
s16 func_800A41F0(u8 *p)
{
    u8 *q;
    u16 r;

    if (p[0x28] + *(s16 *)(p + 0x64) <= 0)
    {
        return 0;
    }
    if (*(s32 *)(p + 0x1C) & 0x2000)
    {
        q = *(u8 **)(p - 0x14);
        func_8009A350(q[0x24] - 1, q[0x25], 0, &r);
        if (r & 2)
        {
            return 0;
        }
    }
    if (*(s32 *)(p + 0x1C) & 0x290000)
    {
        return 0;
    }
    return (s16)func_80042900((s8 *)p, 0x1B) == 0;
}

/* Find the entry of the 0x80010248 record array whose +1 byte is 0x13 and
   whose +3 byte indexes a D_800E3DF0 word equal to the key; return that
   index. THE CURSOR MUST BE A GIV, not a pointer walked from a statement
   before the loop: as `q = (u8 *)0x8001024B;` ahead of the `for` it is a
   SOURCE statement and sits at the head of the preheader, where the target
   emits it LAST, after the counter, the 0x13 and the table base -- 26 words
   at 5 differing, all of them preheader order. 26 words, 0 differing. */
s32 func_800A4298(s32 key)
{
    s32 i;
    u8 *q;
    s32 k;

    for (i = 0; i < 0x14; i++)
    {
        q = (u8 *)0x8001024B + i * 4;
        if (q[-2] == 0x13)
        {
            k = q[0] & 0x1F;
            if (D_800E3DF0[k] == key)
            {
                return k;
            }
        }
    }
    return -1;
}

/* Pick what the cursor is standing on. The `sw $zero, 0xF0($v1)` tail is
   shared by the first and last arms, which is cross-jumping putting the
   duplicated statement back. 75 words, 0 differing. */
void func_800A4300(u8 *a, u8 *b)
{
    s16 r;
    s16 i;
    s16 j;

    r = func_800A4474(a[0x24], a[0x25]);
    if (r != 0)
    {
        *(u16 *)D_80081470 = r | 0x1400;
        *(u32 *)(D_800814A8 + 0xF0) = 0;
    }
    else
    {
        i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
        if (i >= 0)
        {
            if ((D_800E3648_s[i].unk_03 & 0x40) == 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3648_s[i];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3648_s[i];
            }
            else
            {
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
        else
        {
            j = func_800A70E4(a[0x24], a[0x25], *(s16 *)(b + 0x88));
            if (j >= 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3548_s[j];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3548_s[j];
            }
            else
            {
                *(u32 *)D_80081470 = 0;
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
    }
}

/* 18 words, 0 differing. */
u8 *func_800A442C(void)
{
    u8 *r;

    if (D_80081470[1] < 0x14)
    {
        r = *(u8 **)(D_800814A8 + 0xF0);
        if (r != NULL)
        {
            return r;
        }
        *(s32 *)D_80081470 = 0;
    }
    return D_80081470;
}

/* Find (x, y) among the four D_800E3CD8 marks. SUBSCRIPT THE STRUCT ARRAY;
   DO NOT WALK A `s16 *`: the walk makes TWO address givs -- a base walked by
   8 and a second biased by 4, read at -2 and 0 -- and the counter moves to
   $t0, 29 words / 29 differing. 27 words, 0 differing. */
s32 func_800A4474(s16 x, s16 y)
{
    s32 i;
    s16 n;

    for (i = 0; i < 4; i++)
    {
        n = D_800E3CD8[i].unk_00;
        if (n < 2)
        {
            return 0;
        }
        if (x == D_800E3CD8[i].unk_02 && y == D_800E3CD8[i].unk_04)
        {
            return n;
        }
    }
    return 0;
}

/* TWO ALLOCATION FACTS, and nothing else in the function is free. The
   residue of the obvious spelling is 11 to 23 differing with every opcode,
   immediate and displacement already right and only $s1/$s2/$s3 permuted.

   (1) THE TWO SUMS GO THROUGH `u16` LOCALS, so the truncation happens at the
   ASSIGNMENT and not inside the argument expression. Written inline, `u`
   scores 3 refs over 24 insns against `v`'s 3 over 30 and takes the lower
   register, which is the wrong way round; through the two `u16` temporaries
   the lengths become 28 and 26 and `v` wins. Hoisting the same sums into
   `s32` temporaries changes NOTHING -- cse folds them back -- so it is the
   TYPE of the temporary, not the statement split.

   (2) THE THIRD PARAMETER IS `s16`, and that is what puts it in $s1: as s32
   it is live over 34 insns and scores 0.059, below both coordinates; as s16
   the pseudo is live over 17 and scores 0.118. Two callers confirm it --
   func_800A36B4 and func_800A05A4 both sign-extend $a2 immediately before the
   `jal`. `dir` is the other way: func_800A05A4 passes `$t0 << 9` with no
   truncation, so it is u32. 62 words, 0 differing. */
s32 func_800A44E0(u16 x, u16 y, s16 c, u32 dir)
{
    s16 t;
    s32 u;
    s32 v;
    s32 i;
    u16 aa;
    u16 bb;

    u = x >> 6;
    v = y >> 6;
    i = (dir >> 9) & 7;
    if (func_8009A350(u, v, i, (u16 *)&t) == 0)
    {
        return 0;
    }
    if ((t & 0x8000) != 0)
    {
        return 1;
    }
    aa = ((u16 *)D_8006CCD8)[i] + u;
    bb = ((u16 *)D_8006CCE8)[i] + v;
    t = func_800BCB04((((s32)aa << 6) + 0x20) & 0xFFE0, (((s32)bb << 6) + 0x20) & 0xFFE0,
                      c - 0x20);
    return t > 0x200;
}

/* Is the tile ahead blocked? THE TWO TILE INDICES ARE `u16` LOCALS. As s32
   the shift lands straight in the callee-saved register and the function is
   43 words against 44 with 41 differing; an HImode pseudo cannot be the
   `srl`'s destination, so the shift goes to a temp and the copy into the
   callee-saved register is the missing word. 44 words, 0 differing. */
s16 func_800A45D8(u16 x, u16 y, s32 c)
{
    u16 tx;
    u16 ty;
    s16 r;

    tx = x >> 6;
    ty = y >> 6;
    if (func_8009A350(tx - 1, ty, 0, (u16 *)&r) == 0)
    {
        return 0;
    }
    if (r & 0x8000)
    {
        return 1;
    }
    r = func_800BCB04((tx << 6) | 0x20, (ty << 6) | 0x20, (s16)(c - 0x20));
    return r > 0x200;
}

/* Is the tile one step along `ang` from (px, py) blocked? func_8009B164 is
   the same probe one record out and its three spellings transfer.

   THREE THINGS ARE SPECIFIC. (1) `face` is a NARROW parameter: as s32 with
   the `(s16)` cast at the call it is 60 words and 11 differing, every
   difference a callee-saved register -- the sign-extension of a 16-bit
   parameter is one more reference on its allocno, which lifts it past
   `gx`/`gy` in global-alloc's priority. Nothing calls this function anywhere
   in the project, so the width is unwitnessed. (2) `k` must be s16 as well:
   as s32 the index is computed straight into $s0 and copied to $a2 at the
   call, where the target computes it into $a2 and copies it into $s0, 17
   differing. (3) The `+ 0x20` must be its own set of an ALREADY-SET variable:
   in one expression combine proves the low bits clear and turns the add into
   `ori`, 21-32 differing. 60 words, 0 differing. */
s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)
{
    u16 m;
    s32 gx;
    s32 gy;
    s16 k;
    u16 u0;
    u16 u1;
    s32 x;
    s32 y;
    s16 r;

    gx = px >> 6;
    gy = py >> 6;
    k = (ang >> 9) & 7;
    if (flag == 0)
    {
        if ((s16)func_8009A350(gx, gy, k, &m) != 0)
        {
            u0 = (u16)D_8006CCD8[k] + gx;
            u1 = (u16)D_8006CCE8[k] + gy;
            x = u0 * 64;
            x = (x + 0x20) & 0xFFE0;
            y = u1 * 64;
            y = (y + 0x20) & 0xFFE0;
            r = func_800BCB04(x, y, (s16)(face - 0x20));
            m = r;
            return r > 0x200;
        }
    }
    return 0;
}

/* func_800A45D8's probe one tile further: when the tile west of (x, y) is
   clear and its centre is above 0x200, nudge the sample point by a tile in
   whichever axis is more than 0x15 off centre and probe again.

   FOUR TYPE AND SPELLING FACTS, all measured:
   (1) `gx`/`gy` ARE `u16`, exactly as in func_800A45D8 -- as `s32` the `srl`
   lands straight in the callee-saved register and the two `addu` copies the
   target has go missing: 89 words of 94, 79 differing.
   (2) `nx`/`ny` ARE `u16` TOO. As `s32` the second call's two arguments come
   out `andi 0xFFE0` instead of `andi 0xFFFF`, because combine can still see
   that `(gx << 6) + 0x20` has five clear low bits, and the two copies into
   the callee-saved registers disappear again: 90 words, 68 differing.
   (3) THE X STEP GOES THROUGH A TEMPORARY AND THE Y STEP DOES NOT -- that
   asymmetry is in the target, not a guess. Spelled the same way as the x step
   the y block loses its out-of-line zero and its `j`: 93 words, 32 differing.
   Spelled the same way as the y step the x block loses the `addu $a0, $v0`
   copy at its join: 93 / 45.
   (4) THE Y STEP STILL HAS TO PASS THROUGH `t`, as the assignment inside the
   conditional expression. Without it the y argument is `andi 0xFFE0` against
   the target's `andi 0xFFFF` -- one word, and the only difference left after
   everything above. Measured and rejected at 1 differing: both plain-`if` and
   plain-`?:` forms of the y step, `u16`/`s16`/`s8` for `sy`, `(u16)(sy + ny)`,
   `(ny + sy) & 0xFFFF`, `(u16)(ny + (u16)sy)`, a second temporary `u` holding
   the ternary, and `u16` locals for the two sums (3 differing). An `if/else`
   with `t` in the else arm is 6. 94 words, 0 differing. */
s32 func_800A4778(u16 x, u16 y, s16 c, s32 flag)
{
    u16 gx;
    u16 gy;
    u16 nx;
    u16 ny;
    s32 dx;
    s32 dy;
    s32 t;
    s32 sx;
    s32 sy;
    s16 r;

    if (flag == 0)
    {
        gx = x >> 6;
        gy = y >> 6;
        if (func_8009A350(gx - 1, gy, 0, (u16 *)&r) != 0)
        {
            nx = gx * 64;
            nx = nx + 0x20;
            ny = gy * 64;
            ny = ny + 0x20;
            r = func_800BCB04(nx & 0xFFE0, ny & 0xFFE0, (s16)(c - 0x20));
            if (r > 0x200)
            {
                dx = (s16)(nx - x);
                sx = 0;
                if (abs(dx) >= 0x16)
                {
                    if (dx > 0)
                    {
                        t = -0x40;
                    }
                    else
                    {
                        t = 0x40;
                    }
                    sx = t;
                }
                dy = (s16)(ny - y);
                sy = abs(dy) < 0x16 ? 0 : (t = dy > 0 ? -0x40 : 0x40);
                r = func_800BCB04((u16)(nx + sx), (u16)(ny + sy), (s16)(c - 0x20));
                return r > 0x200;
            }
        }
    }
    return 0;
}

/* The four-slot status table at +0x2C: refresh an existing entry of the same
   kind, otherwise take the first free slot, otherwise evict the weakest one
   and retry. The recursion is the retry, and the engine's own declaration of
   this function lives in include/structs.h.

   THE XOR IS ON `(u8)c`, NOT ON `c`. The sign-extended copy of c is already
   live in $a2 for the `c == 0` test, so `(c ^ byte) & 0x80` reuses it and the
   whole register assignment shifts behind it -- 67 differing, with p/kind/c
   landing in $s2/$s4/$s5 where the target has $s3/$s5/$s2. The target xors
   the RAW parameter out of its callee-saved home. `(u8)c` and
   `(c ^ *(s8 *)...)` both give 50; `(u32)c` stays at 67.

   AND THE LAST TEST IS `if (best > kind) { evict; retry; } return -1;`.
   Written as the early return -- `if (kind >= best) return -1;` -- gcc lays
   the `return -1` out at the END of the function instead of in the branch's
   shadow, the two loops' `func_800A4B88(p, kind); return i;` tails then
   cross-jump into one, and it is 46 differing; `best <= kind` fixes the
   `slt`'s operand order but not the layout, and `kind < best` as the if test
   is 5. The `else` forms of both are 46 and 0. 107 words, 0 differing. */
s16 func_800A48F0(s8 *p, s8 kind, s8 c)
{
    s32 i;
    s16 best;
    s8 v;

    best = 0x100;
    for (i = 3; i >= 0; i--)
    {
        if (*(s8 *)(p + i * 2 + 0x2C) == kind)
        {
            if ((((u8)c ^ *(u8 *)(p + i * 2 + 0x2D)) & 0x80) != 0 || c == 0)
            {
                func_80042BDC(p, *(s8 *)(p + i * 2 + 0x2C));
                *(s8 *)(p + i * 2 + 0x2C) = 0;
                *(s8 *)(p + i * 2 + 0x2D) = 0;
            }
            else
            {
                *(s8 *)(p + i * 2 + 0x2D) = c;
            }
            func_800A4B88(p, kind);
            return i;
        }
    }
    for (i = 3; i >= 0; i--)
    {
        v = *(s8 *)(p + i * 2 + 0x2C);
        if (v == 0)
        {
            if (kind == 1)
            {
                func_80042B68(p, 4);
            }
            else if (kind == 4)
            {
                func_80042B68(p, 1);
            }
            *(s8 *)(p + i * 2 + 0x2C) = kind;
            *(s8 *)(p + i * 2 + 0x2D) = c;
            func_800A4B88(p, kind);
            return i;
        }
        if (v < best)
        {
            best = v;
        }
    }
    if (best > kind)
    {
        func_80042B68(p, best);
        return func_800A48F0(p, kind, c);
    }
    return -1;
}

/* Four 2-byte records scanned DOWNWARD from +0x32; the counter and the -2
   cursor are the target's `bgez`/`addiu -2` pair. 12 words, 0 differing. */
s32 func_800A4A9C(u8 *p)
{
    s32 i;

    for (i = 3; i >= 0; i--)
    {
        if (*(s8 *)(p + i * 2 + 0x2C) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Run the four two-byte slots at +0x2C/+0x2D down from the top.

   THE SUBSCRIPT IS WRITTEN AT EVERY SITE, NOT WALKED. `s = p + 6;` with
   `s -= 2` in the loop's third clause folds the +0x2C into the biv's
   initialiser, so the cursor starts at p + 0x32 and the two accesses come out
   at displacements 0 and 1 where the target keeps p + 6 and +0x2C/+0x2D: 47
   words, 8 differing, every one a displacement. The degenerate `bgez` after
   the `blez` is the `else if (d < 0)` arm's own test, not a redundancy.
   47 words, 0 differing. */
void func_800A4ACC(u8 *p)
{
    s32 i;
    s32 c;
    s32 d;
    u8 v;

    for (i = 3; i >= 0; i--)
    {
        c = *(s8 *)(p + i * 2 + 0x2C);
        if (c != 0)
        {
            if (c == 5)
            {
                func_800A5DFC(p, *(s32 *)(p - 0x18));
            }
            d = *(s8 *)(p + i * 2 + 0x2D);
            if (d != 0)
            {
                if (d > 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))--;
                }
                else if (d < 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))++;
                }
                if (*(s8 *)(p + i * 2 + 0x2D) == 0)
                {
                    v = *(u8 *)(p + i * 2 + 0x2C);
                    *(u8 *)(p + i * 2 + 0x2C) = 0;
                    func_80042BDC((s8 *)p, (s8)v);
                }
            }
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_8009F644", D_80089000);

/* The status-kind dispatcher func_800A48F0 calls after it writes a slot:
   twelve of twenty-seven kinds set a bit in the view's +0x1C flags, kinds
   11/12/27 flash the tile through func_8009A3D0/func_8009A21C with the
   0x2000 bit choosing 0x300 or 0x3000, and kind 10 is the petrify path --
   func_8003E188(0x2E, 0), then either func_800ACB98 and an early return or
   func_800D8590 plus a full re-skin from D_800DD274 and a four-word clear of
   D_80083110. Every other kind falls straight through to func_80041E70(p).
   The jump table is jtbl_80089008 at rodata offset 0x8A8, 0 mod 8, so
   cc1psx's `.align 3` is a no-op.

   `s16 kind` IS THE SWITCH SUBJECT'S OWN TYPE, not a cast: the listing biases
   first (`addiu $a1, -1`) and sign-extends after (`sll 16`/`sra 16`), which
   is what `stmt.c` does when index_type is HImode -- docs/matching-levers.md,
   "A `switch` ON A `short` SUBJECT PUTS THE SIGN EXTENSION *AFTER* THE RANGE
   BIAS". func_800A48F0 passes an `s8` and re-measures at 107 words /
   0 differing against the forward declaration, so the narrowing costs its
   caller nothing.

   D_80082E80 NEEDS A POINTER LOCAL. Written out at both the +0x2C store and
   the func_8003DB94 call it is still 169 words but 7 differing: the
   `addiu %lo` is biased by +0x2C for the store and then un-biased with
   `addiu $a0, $a0, -0x2C` for the call, where the target keeps the bare
   symbol in $a0 and puts 0x2C in the store's displacement.
   169 words, 0 differing. */
