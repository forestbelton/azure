#include "common.h"
#include "ovl_dungeon_decls.h"

/* dungeon.bin's overlay, translation unit 3 of 4: func_800A4B88 to the next
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

s32 func_800954E0(u8 *p, void *b, void *c, u8 *d);
void func_80095DD0(u8 *p, void *b, void *c, void *d);
void func_80096088(UnkDungeonActor *a, u8 *p);
void func_800974DC(u8 *p, UnkVec *b, u8 *c, u8 *d);
s16 func_80098FF8(void);
u8 *func_80099254(u8 *s, u8 *d);
u8 *func_8009929C(s32 c, u8 *p);
u8 *func_800999B0(u8 *p);
void func_800999D4(u8 *p);
s32 func_8009A180(u8 *a, u8 *b);
s32 func_8009B164(u16 a, UnkVec *b, u8 *c);
s32 func_8009B7E4(u8 *a, u8 *b);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
void func_8009CE1C(u8 *p, s16 b, s16 c, s16 e, s16 ang, u32 q, s16 g2);
void func_8009DA70(s32 x, s32 y, s32 *src, s32 tag);
void func_800A020C(s32 f, u8 *d);
u8 *func_800A03C4(u8 *p, u16 x, u16 y);
s32 func_800A0B94(s16 a, u8 *s, s16 c);
s32 func_800A152C(s16 a, s16 b);
void func_800A18E8(s16 a, s16 b);
void func_800A1B44(s16 x, s16 y);
s32 func_800A1BD0(u8 *key);
void func_800A1D4C(u8 *p, s16 f);
s32 func_800A2B5C(void *d);
s32 func_800A2BDC(u8 *a);
s32 func_800A2C78(u8 *p);
s32 func_800A2DB8(u8 *p);
void func_800A2FE0(u8 *p);
void func_800A34BC(u8 *p);

void func_800A4B88(s8 *p, s16 kind)
{
    void **t;
    u8 *c;
    u8 *g;
    s32 i;

    switch (kind)
    {
    case 1:
        *(u32 *)(p + 0x1C) |= 0x200;
        break;
    case 2:
        *(u32 *)(p + 0x1C) |= 0x400;
        break;
    case 3:
        *(u32 *)(p + 0x1C) = (*(u32 *)(p + 0x1C) & ~0x20000) | 0x10;
        break;
    case 4:
        *(u32 *)(p + 0x1C) |= 0x20;
        break;
    case 5:
        *(u32 *)(p + 0x1C) |= 0x40;
        break;
    case 6:
        *(u32 *)(p + 0x1C) |= 0x80;
        break;
    case 8:
        *(u32 *)(p + 0x1C) |= 0x800;
        break;
    case 9:
        *(u32 *)(p + 0x1C) |= 0x1000;
        break;
    case 27:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 11:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *)(p + 0x1C) |= 0x2000;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 12:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *)(p + 0x1C) &= ~0x2000;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 10:
        func_8003E188(0x2E, 0);
        if ((*(u32 *)(p + 0x14) & 0x20000000) != 0)
        {
            func_800ACB98((u8 *)p, *(u8 **)(p - 0x18), *(u8 **)(p - 0x14), (u8 *)p);
            return;
        }
        func_800D8590(p);
        if (*(u8 *)(p + 0x13) == 0)
        {
            func_80096088((UnkDungeonActor *)p, (u8 *)p);
            g = D_80082E80;
            t = D_800DD274;
            *(void ***)(g + 0x2C) = t;
            func_8003DB94(g, t[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
            for (i = 3; i >= 0; i--)
            {
                D_80083110[i] = 0;
            }
        }
        break;
    }
    func_80041E70((u8 *)p);
}

/* Pick a random free tile: 256 tries at a random D_800E2970 room, then a
   full 1..0x3E scan, then a range check on whatever the caller already had.
   Returns the room index on the random hit and 0 otherwise. 136 words,
   0 differing.

   BOTH HITS ARE `goto`s TO BLOCKS WRITTEN AFTER THE `return 0`. The two
   bodies are `*x = cx; *y = cy; return <n>;` and cross-jumping merges their
   tails at .L800A5000, which lets reorg steal `addu $v0, $s3, $zero` into
   the first loop's own `beqz` delay slot. Written as inline bodies inside
   the loops the merge still happens but the branch cannot be inverted over
   the `v0 = room` insn, and it is 135 words / 102 differing.

   THE TWO DRAWS COME FIRST AND THE TWO BASES ARE ADDED AFTER:
   `cx = rand % e->unk_04; cy = rand % e->unk_06; cx += e->unk_00;
   cy += e->unk_02;`. Folded into one expression each, the `+ unk_00` fills
   the second `jal`'s delay slot where the target has a `nop` -- 135 words,
   one short. Two extra `rx`/`ry` locals instead are 134 words, because the
   `%`'s HImode result then costs an `andi 0xFFFF` at the assignment.
   `(u32)func_800A6D30() % ...` is what makes those two divides `divu`; the
   room's `% *(s16 *)(g + 0x6)` is signed and stays `div`.

   `i = 0x100;` IS ITS OWN STATEMENT AHEAD OF `g = (u8 *)D_80081468;`. As the
   `for`'s init the preheader emits D_80081468's `lui`/`addiu` first and it
   is 5 differing; `for (i = 0x100, g = ...; ...)` measures identically to
   this spelling. */
s32 func_800A4E2C(u8 *x, u8 *y)
{
    u8 *g;
    s16 room;
    s16 cx;
    s16 cy;
    s32 i;
    u16 w;

    i = 0x100;
    g = (u8 *)D_80081468;
    for (; i > 0; i--)
    {
        room = func_800A6D30() % *(s16 *)(g + 0x6);
        if (D_800E2970_s[room].unk_0A != 0)
        {
            cx = (u32)func_800A6D30() % D_800E2970_s[room].unk_04;
            cy = (u32)func_800A6D30() % D_800E2970_s[room].unk_06;
            cx += D_800E2970_s[room].unk_00;
            cy += D_800E2970_s[room].unk_02;
            if ((s16)func_8009A350(cx - 1, cy, 0, &w) != 0 && (w & 0xF720) == 0)
            {
                goto found;
            }
        }
    }
    for (cy = 1; cy < 0x3F; cy++)
    {
        for (cx = 1; cx < 0x3F; cx++)
        {
            if ((s16)func_8009A350(cx - 1, cy, 0, &w) != 0 && (w & 0xF720) == 0)
            {
                goto found_scan;
            }
        }
    }
    if ((u32)(*x - 1) >= 0x3E || (u32)(*y - 1) >= 0x3E)
    {
        goto bad;
    }
    return 0;
found_scan:
    *x = cx;
    *y = cy;
    return 0;
found:
    *x = cx;
    *y = cy;
    return room;
bad:
    *x = 1;
    *y = 1;
    return 0;
}

/* 401 words, 0 differing.  Park closed by j63; the park header's own
   diagnosis of the stuck 6-row group was right and is what pointed here.

   TWO LEVERS, BOTH FROM docs/matching-levers.md.

   1. `d = 0;` AT THE END OF THE ELSE-BRANCH CASE 2 IS A DEAD STORE FLOW
   DELETES, AND IT BUYS cse CANONICALITY FOR THE COPY WITHOUT BUYING A
   REFERENCE ("CANONICALITY IS PAID AT cse1, THE REFERENCE COUNT AT flow").
   The target wants the call result in the 71-reference pseudo ($s0) and the
   COPY in the ten-reference one ($s2), with the block-move destination and
   the two pre-loop 0x5C/0x58 reads ON THE COPY.  cse1 only leaves those
   reads on `d` if `d` is qty_first_reg at the `d = q` insn, which
   make_regs_eqv (cse.c:865) grants only when regno_last_uid[d] >
   regno_last_uid[q] -- and `q` is reused by the other switch cases, so it
   always dies later.  The dead `d = 0;` is the latest reference to `d` in
   the function, survives cse1 and cse2, and is deleted at flow
   (flow.c:1539), so `d` is canonical with exactly the target's ten
   references and still scores 0.409, between `w` (0.551) and `p` (0.3105):
   q $s0, w $s1, d $s2, p $s3, g $s4.  Without it, the same spelling is
   48 aligned rows -- cse puts the pre-loop reads back on `q`, `d` falls to
   seven references (0.222), and `p` and the gate pseudo swap $s2/$s3.
   Inverting the copy instead (`d = <call>; q = d;`) is 10 rows.

   2. THE FIVE `goto out;` ARE WHAT KEEPS A `v0 = 0` OUT OF THE SHARED
   `D_800DCF5E++` BLOCK.  Written as `return 0;`, each of those four tails
   is `[lhu][set v0 0][addu][sh][jump]` already at sched1 -- sched1 fills
   the load-delay slot with the return value (visible in the .sched dump),
   so local-alloc gives the increment $v1 and jump2's cross-jump can no
   longer peel the `set` off the end: 4 rows.  Jumping to a shared
   `out: return 0;` leaves the set in its own block, the slot a nop, the
   increment in $v0 and the `j` pointing at .L800A5668.  The fifth goto,
   on the else-branch case 2's `q == NULL` exit, is what makes reorg fill
   that branch's delay slot from the fall-through (`addu $a0, $s3, $zero`)
   instead of stealing `addu $v0, $zero, $zero` from .L800A5668: 3 rows
   without it.  Making EVERY `return 0` a `goto out` is 400 words.

   Other spellings the park measured and rejected: a separate local for
   q-0x20 (404w), a separate `s` local for the 0x8C-field source (+6),
   a `s16 n` gate local with `D_800DCF5C = n` (402w / 397), `g = p; d = q;`
   moved down beside the field copies (399w, cse kills both copies),
   `d = q - 0x20` as the tail (27), `d = q; if (d == NULL)` (64). */
s32 func_800A504C(u8 *a, u8 *p)
{
    u8 *g;
    u8 *q;
    u8 *w;
    u8 *d;
    u8 *e;
    u8 *z;
    s16 k;
    s16 i;
    u16 m;
    s32 r;
    u8 id;
    s32 x;
    s32 y;
    s32 f;
    u32 ab;
    g = (u8 *)(&D_80083460);
    if ((*((s16 *)(g + 0xA))) != 1)
    {
        return 0;
    }
    if (func_8003FA44(8) == 0)
    {
        return 0;
    }
    if (((*((u32 *)(p + 0x14))) & 0x4000) != 0)
    {
        switch (D_800DCF5E)
        {
        case 0:
            if (func_80053EF0(4) != 0)
            {
                return 0;
            }
            q = func_800A1618(p[0x13], 2);
            if (q == 0)
            {
                return 0;
            }
            D_80081488 = (u32)func_80047DB8(p[0x13]);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)(&D_800E3E40));
            D_800DCF5E++;
            goto out;

        case 1:
            if (D_800E3E40 == 0)
            {
                return 0;
            }
            func_80047E30();
            D_800DCF5E++;
            goto out;

        case 2:
            q = func_800A1618(p[0x13], 2);
            if (q == 0)
            {
                return 0;
            }
            w = *((u8 **)(p - 0x14));
            q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(p[0x13], q, 1))(6, w[0x24], w[0x25], (s16)((*((u16 *)(p + 0x88))) - 0x20));
            if (q == 0)
            {
                return 0;
            }
            g = p;
            d = q;
            x = *((s32 *)(d + 0x5C));
            y = *((s32 *)(d + 0x58));
            *((Unk42710 *)d) = *((Unk42710 *)p);
            *((s32 *)(q + 0x4C)) = 0;
            *((s32 *)(q + 0x50)) = 0;
            *((s32 *)(q + 0x5C)) = x;
            *((s32 *)(q + 0x58)) = y;
            func_80042640((Unk42710 *)q, q[0x13]);
            w = *((u8 **)(q - 0x14));
            *((s16 *)(w + 0x1E)) = 0x1000;
            *((s16 *)(w + 0x1C)) = 0x1000;
            *((u32 *)(q + 0x14)) &= ~7;
            *((u32 *)(q + 0x1C)) &= ~7;
            *((u32 *)(q + 0x14)) |= (*((u32 *)(p + 0x14))) & 7;
            *((u32 *)(q + 0x1C)) |= (*((u32 *)(p + 0x1C))) & 7;
            func_800424E0(q, q[0x13], 0);
            func_80042710((Unk42710 *)q, (Unk42710 *)p);
            *((u32 *)(q + 0x14)) &= ~0x4000;
            *((u32 *)(d + 0x8C)) = *((u32 *)(g + 0x8C));
            *((u32 *)(d + 0x90)) = *((u32 *)(g + 0x90));
            *((u16 *)(d + 0x94)) = *((u16 *)(g + 0x94));
            *((u16 *)(d + 0x98)) = *((u16 *)(g + 0x98));
            d[0x9A] = g[0x9A];
            d[0x9B] = g[0x9B];
            k = (s16)func_800A1BD0(p);
            ab = D_800814A8;
            *((s32 *)((ab + (k * 4)) + 0xAC)) = (s32)q;
            func_8009A3D0(w[0x24], w[0x25], 0x300);
            func_8009A028(q);
            q -= 0x20;
            func_80044A50((UnkNode *)q);
            *((u32 *)(q + 0x10)) |= 0x80000000;
            *((u16 *)(p - 0x2)) |= 0x8000;
            D_800DCF5E = 0;
            D_800814A0 |= 0x8000;
            return 0;
        }

        return 0;
    }
    switch (D_800DCF5E)
    {
    case 0:
        q = func_800A1618(p[0x13] - 1, 3);
        if (q != 0)
        {
            p[0x13]--;
            func_8003E188(p[0x13], 0);
            D_80080A90 = D_80023000;
            D_80081488 = func_80048118(p[0x13], D_8008149C);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)(&D_800E3E40));
        }
        D_800DCF5E++;
        goto out;

    case 1:
        if (D_800E3E40 == 0)
        {
            return 0;
        }
        if (D_80081488 != 0)
        {
            func_80047FF4(p[0x13], D_80081488);
        }
        D_800DCF5E++;
        goto out;

    case 2:
        q = func_800A1618(p[0x13], 3);
        if (q == 0)
        {
            return 0;
        }
        w = *((u8 **)(p - 0x14));
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(p[0x13], q, 1))(1, w[0x24], w[0x25], (s16)((*((u16 *)(p + 0x88))) - 0x20));
        if (q == 0)
        {
            goto out;
        }
        *((u32 *)(q + 0x14)) |= 0x4000;
        k = (s16)func_800A1BD0(p);
        ab = D_800814A8;
        e = *((u8 **)((ab + (k * 4)) + 0xD0));
        *((s32 *)((ab + (k * 4)) + 0xAC)) = (s32)q;
        i = e[3] & 0x1F;
        e[0] = p[0x13];
        D_800E3DF0[i] = (s32)q;
        w = *((u8 **)(q - 0x14));
        *((s16 *)(w + 0x1E)) = 0x1000;
        *((s16 *)(w + 0x1C)) = 0x1000;
        func_80042640((Unk42710 *)q, q[0x13]);
        *((u32 *)(q + 0x14)) &= ~7;
        *((u32 *)(q + 0x1C)) &= ~7;
        *((u32 *)(q + 0x14)) |= (*((u32 *)(p + 0x14))) & 7;
        *((u32 *)(q + 0x1C)) |= (*((u32 *)(p + 0x1C))) & 7;
        func_800424E0(q, q[0x13], 0);
        func_80042710((Unk42710 *)q, (Unk42710 *)p);
        if (q[0x43] < 0x40)
        {
            z = (u8 *)0x80010000;
            r = q[0x43];
            id = q[0x13];
            *((u8 *)((z + (r * 0x54)) + 0xA93)) = id;
            *((u8 *)((z + (r * 4)) + 0x980)) = id;
        }
        func_800A152C(q[0x13], 3);
        func_80042560((s8 *)q);
        m = *((u16 *)(p + 0x2A));
        *((u16 *)(q + 0x2A)) = m;
        func_80047738(w, (*((u8 **)(w + 0x2C)))[(((D_80083228 + ((s16)m)) + 0x100) >> 9) & 7], *((s8 *)(w + 0x4)));
        ((u16 *)w)[0xA] &= 0xFFFE;
        D_80080A90 = D_8014A000;
        ((u16 *)g)[5] -= 1;
        D_800DCF5C = 1;
        *((u16 *)(p - 0x2)) |= 0x8000;
        f = D_800814A0;
        *((s32 *)(p + 0x90)) = 0;
        D_800814A0 = f | 0x8000;
        d = 0;
        break;
    }

out:
    return 0;
}

/* The bound test reads the counter SIGNED (`lh`) and the increment reads it
   UNSIGNED (`lhu`) off the same `lui`; that pair is what `s16` plus a bare
   `++` gives. 12 words, 0 differing. */
s32 func_800A5690(void)
{
    if (D_800E3CCE < 0x20)
    {
        D_800E3CCE++;
        return 1;
    }
    return 0;
}

/* Saturating countdown. 8 words, 0 differing. */
void func_800A56C0(void)
{
    if (D_800E3CCE != 0)
    {
        D_800E3CCE -= 1;
    }
}

/* A COMMON RETURN VARIABLE, as in func_800A9E38: with two `return`s jump.c
   inverts the test and moves the `return 0` past the call block. The result
   is the sound call's, which is why the prototype is s32 and not void --
   every call site in this unit drops it. 16 words, 0 differing. */
s32 func_800A56E0(s32 id)
{
    s32 r;

    if (D_80013714 & 2)
    {
        r = 0;
    }
    else
    {
        r = func_80053DA8(id);
    }

    return r;
}

/* Walk the message to its terminator, fold a shift-JIS second byte in
   0x81..0x9A down by 0x21 after an 0x82 lead byte, and hand the message to
   func_800353F4.

   THE EARLY EXIT IS WRITTEN LAST. As `if (D_80013714 & 2) return 0;` at the
   top the `return 0` is laid out inline and the body jumps round it: 36
   words against 37, 30 differing. Wrapping the body in the inverted test and
   leaving `return 0` as the final statement puts the zero at the epilogue
   where the target has it -- so the rule is about where the EPILOGUE ends
   up, not about which arm is trivial. The `(u8)` on `c - 0x81` is where
   `andi 0xFF` comes from, and combine rewrites the constant to +0x7F under
   that mask. 37 words, 0 differing. */
s32 func_800A5720(void *msg)
{
    u8 *p;
    u8 c;

    if ((D_80013714 & 2) == 0)
    {
        p = (u8 *)msg;
        while (*(s8 *)p > 0)
        {
            p++;
        }
        if (p[0] == 0x82)
        {
            c = p[1];
            if ((u8)(c - 0x81) < 0x1A)
            {
                p[1] = c - 0x21;
            }
        }
        return func_800353F4((u32)msg);
    }
    return 0;
}

/* Which of the three stride-3 slots holds a +8 byte in the same group of
   three as the given one. BOTH DIVISIONS ARE WRITTEN INSIDE THE LOOP AND THE
   INVARIANT ONE IS THE LEFT OPERAND. Computing `(n - 1) / 3` into a local
   ahead of the loop gives the magic constant TWICE -- once for it and once
   hoisted for the body's own division, 31 words at 27 differing; written
   inline in the comparison, cse unifies them in the body and loop.c hoists
   one. With the operands the other way round it is 29 words at 1 differing.
   29 words, 0 differing. */
s16 func_800A57B4(u8 *p, s16 n)
{
    s32 i;
    s32 c;

    for (i = 0; i < 3; i++, p += 3)
    {
        c = p[8];
        if (c != 0 && (n - 1) / 3 == (c - 1) / 3)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* The UnkNode +0x10 handler func_800A5A18 installs: a two-phase colour ramp
   on the renderable at +0x0C and a sine wobble on its scale. State +0x64 is
   0 for the ramp up and nonzero for the ramp down; D_80083160's +0x4 flag
   bypasses the ease in each phase, and the down phase ends by whiting out
   the partner node's renderable and raising both 0x8000 flags.

   THE -0x40 IS WRITTEN 0xFFC0 THROUGH A `u16` STORE. With `*(s16 *)` the
   ternary's constant folds to a signed -64 and gcc emits `addiu $v1, $zero,
   -0x40` where the target has `ori $v1, $zero, 0xFFC0`; `-0x40`, `0xFFC0`
   and `0xFFC0u` through the s16 store are all 1 differing, and only the u16
   destination keeps the constant unsigned. 124 words, 0 differing. */
void func_800A5828(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    u8 *q;
    s32 v;

    g = (u8 *)&D_80083160;
    if (*(s16 *)(p + 0x64) == 0)
    {
        if ((*(u16 *)(g + 0x4) & 1) != 0)
        {
            v = *(s16 *)(p + 0x6A) >> 1;
        }
        else
        {
            *(s16 *)(p + 0x6A) += (0xC0 - *(s16 *)(p + 0x6A)) / *(s16 *)(p + 0x66);
            v = *(u8 *)(p + 0x6A);
        }
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        if (--*(s16 *)(p + 0x66) <= 0)
        {
            *(s16 *)(p + 0x66) = 0x10;
            *(s16 *)(p + 0x64) += 1;
        }
    }
    else
    {
        if ((*(u16 *)(g + 0x4) & 1) != 0)
        {
            *(s32 *)(c + 0xC) = 0;
        }
        else
        {
            *(s16 *)(p + 0x6A) -= *(s16 *)(p + 0x6A) / *(s16 *)(p + 0x66);
            v = *(u8 *)(p + 0x6A);
            c[0xE] = v;
            c[0xD] = v;
            c[0xC] = v;
        }
        if (--*(s16 *)(p + 0x66) <= 0)
        {
            q = *(u8 **)(*(u8 **)(p + 0x4) - 0x14);
            q[0xC] = 0x80;
            q[0xD] = 0x80;
            q[0xE] = 0x80;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
    *(s16 *)(c + 0x1C) = (func_800644B8((++*(s16 *)(p + 0x68)) << 9) >> 4) + 0x1000;
    *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C);
    *(u16 *)(c + 0x1A) = (*(s16 *)(p + 0x68) & 1) ? 0x40 : 0xFFC0;
    q = *(u8 **)(*(u8 **)(p + 0x4) - 0x14);
    v = ((*(s16 *)(p + 0x68) & 3) << 4) + 0x40;
    q[0xC] = v;
    q[0xD] = v;
    q[0xE] = v;
}

/* Spawn the kind-0x212 node under D_80083498. TWO POINTER LOCALS, NOT ONE
   REUSED: with a single `q` for both the +0x0C renderable and the payload
   base the two stack constants land in $a2/$v1 instead of $a3/$a2 and the
   payload pointer takes $a3 -- 54 words, 14 differing. 54 words,
   0 differing. */
UnkNode *func_800A5A18(UnkVec *a, u8 *b)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *q;
    s32 v[2];

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A5828;
        func_8004491C(n, (u32)D_80045340);
        n->unk_08 = a;
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DD854;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0x4) = b;
        *(s16 *)(q + 0x66) = 4;
        v[0] = 0x1800340;
        v[1] = 0x400040;
        func_800B835C(D_800DD860, v, 1, 0);
        func_800A56E0(0x60F);
    }
    return n;
}

/* Three-state fade: state 0 eases +0x2 towards p->unk_08 and the renderer's
   +0x1C towards 0x1000 over the +0x6 countdown, state 1 waits (restarting the
   countdown while D_80083170[0] is set), and the last state decays the three
   colour bytes and raises the 0x8000 flags. 96 words, 0 differing, first
   compile. Each arm's countdown is `if (--*(s16 *)(p + 0x6) <= 0)`, the
   func_800999D4 idiom (:3385); the two `+=` in state 0 keep the whole
   expression inline, which is what gives the `lh` for the subtraction and a
   second `lhu` for the accumulate. */
void func_800A5AF0(u8 *p, u8 *b, u8 *c)
{
    if (*(s16 *)(p + 0x4) == 0)
    {
        *(s16 *)(b + 0x2) += (*(s16 *)(p + 0x8) - *(s16 *)(b + 0x2)) / *(s16 *)(p + 0x6);
        *(s16 *)(c + 0x1C) += (0x1000 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x30;
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(p + 0x4) += 1;
        }
    }
    else if (*(s16 *)(p + 0x4) == 1)
    {
        if (D_80083170[0] != 0)
        {
            *(s16 *)(p + 0x6) = 0;
        }
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x20;
            *(s16 *)(p + 0x4) += 1;
        }
    }
    else
    {
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x6);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x6);
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Is any visible ring member standing within four tiles of D_80082E80's
   (0x24, 0x25)? THE RING HEAD IS READ FIRST and the flag word second: the
   other order puts the flags in $a0 and the head in $a1's place, 4
   differing, although the target emits the two `lui`s head-first and the
   two loads flags-first. Both abs() pairs are the `bgez`/nop/`negu` trio.
   53 words, 0 differing. */
s32 func_800A5C70(void)
{
    u8 *p;
    u8 *q;
    u8 *end;
    u8 *g;
    u16 f;

    p = D_800E3D7C;
    f = D_80013714;
    if (f & 2)
    {
        return 1;
    }
    if (f & 9)
    {
        return 0;
    }
    if (D_800E296C & 0x200000)
    {
        return 0;
    }
    g = D_80082E80;
    end = p;
    do
    {
        if (!(*(s32 *)(p + 0x1C) & 0x2208))
        {
            q = *(u8 **)(p - 0x14);
            if (abs(g[0x24] - q[0x24]) < 5 && abs(g[0x25] - q[0x25]) < 5)
            {
                return 0;
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != end);
    return 1;
}

/* 6 words, 0 differing. */
void func_800A5D44(void)
{
    *(s32 *)0x8001362C = D_8008347E;
}

/* func_800999D4's close, counted up instead of down: bump the record's own
   halfword and at 0x10 retire it. 24 words, 0 differing. */
void func_800A5D5C(u8 *p)
{
    u8 *g;

    if (++*(s16 *)p >= 0x10)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* THE REUSED TEMPORARY MUST BE THE NODE'S OWN VARIABLE. Two separate
   locals, in either order, are 15 words / 10-12 differing: sched1
   interleaves the two split address pairs and fills the lhu's load-delay
   slot. A separate reused `u8 *g` alongside `UnkNode *n` is 16 / 2 -- right
   shape, wrong registers, because the second (high)/(lo_sum) pair fails to
   tie. Reusing `n` itself ties them. 16 words, 0 differing. */
void func_800A5DBC(void)
{
    u8 *n;

    n = (u8 *)func_8003FC64(0);
    *(u32 *)(n + 0x10) = (u32)func_800A5D5C;
    n = (u8 *)&D_80083460;
    *(u16 *)(n + 0xA) += 1;
}

/* Spend the charge at +0x29 on the gauge at +0x28/+0x64. The `bgez`/+0x1E
   pair at the top is `(p[0x29] + 15) / 16` -- gcc's signed divide-by-16
   bias, not source. The total is RE-READ for the clamp rather than kept from
   the first test, which is the second `lbu 0x28`/`lh 0x64` pair.
   79 words, 0 differing on the first compile. */
void func_800A5DFC(u8 *p, u8 *v)
{
    s32 n;
    s32 tot;
    u8 *q;

    if ((*(s32 *)(p + 0x1C) & 0x40) == 0)
    {
        return;
    }
    if (p[0x28] + *(s16 *)(p + 0x64) < 2)
    {
        return;
    }
    *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x2A) + 0x800;
    n = (p[0x29] + 15) / 16;
    tot = p[0x28] + *(s16 *)(p + 0x64);
    if (tot - n <= 0)
    {
        n = tot - 1;
    }
    if (n == 0)
    {
        return;
    }
    *(s32 *)(p + 0x1C) &= ~0x20000000;
    q = *(u8 **)(p - 0x14);
    if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
    {
        func_800B4C7C(3, p, (s16)n, 0);
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) - n;
        func_800C5BBC(*(s16 *)(v + 0x2), *(s16 *)(v + 0x6), *(u16 *)(v + 0xA) - 8, 0x802080, 0x20,
                      1);
    }
    else
    {
        p[0x28] = p[0x28] - n;
    }
}

/* The kill handlers' shared report: turn the record's name pair into a
   message id, announce it at the record's tile, play 0x50C, release the
   message and drop the D_80083460 + 0xA live count. The name pair goes in as
   `(q[1], q[0])`, the same order func_800A7A38 records. The record the tile
   comes from is the one at p - 0x18. 34 words, 0 differing. */
void func_800A5F38(u8 *p, u8 *q)
{
    u8 *r;
    u8 *g;
    u32 t;

    r = *(u8 **)(p - 0x18);
    t = func_8004A658(q[1], q[0]);
    func_800A7A7C(*(u16 *)(r + 0x2) >> 6, *(u16 *)(r + 0x6) >> 6, *(s16 *)(r + 0xA), t, q);
    func_800A56E0(0x50C);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

/* The bar-wipe primitive pair, func_800A624C's allocator shape with
   different fill.

   (1) The base must be a POINTER LOCAL: through the bare COMPONENT_REF the
   two post-call reads re-materialise a `lui` into the dead $s0 instead of
   keeping the symbol's address in $s3 across the calls, 80 words against 81.
   (2) The +0xA store belongs in BOTH arms of the counter test, not after the
   join: with a `v` local holding the value the whole chain lands one register
   across, 12 differing, and nine spellings of the division move none of it.
   (3) The read-back of +0xA goes in a local: written out twice the store to
   the renderable's +0x14 invalidates cse's entry and the second read is a
   second `lhu`, 83 words. 81 words, 0 differing. */
s32 func_800A5FC0(u8 *p)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    s16 n;
    s32 w;

    g = &D_80083160;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, *(s32 *)(p + 0x4), 0);
    *(s32 *)(r + 0x4) = *(s32 *)(p + 0x0);
    SetPolyF4(r);
    SetSemiTrans(r, 1);
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x8) = 0;
    *(s16 *)(r + 0xE) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    n = *(u16 *)(p + 0x8) - 1;
    *(s16 *)(p + 0x8) = n;
    if (n > 0)
    {
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + (0x140 - *(s16 *)(p + 0xA)) / n;
    }
    else
    {
        *(s16 *)(p + 0x8) = 0;
        *(s16 *)(p + 0xA) = 0x140;
    }
    w = *(u16 *)(p + 0xA);
    *(s16 *)(r + 0x14) = w;
    *(s16 *)(r + 0xC) = w;
    AddPrim(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), r);
    AddPrim(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), q);
    return 0;
}

/* The base local is advanced halfway through, which is where the target's
   `addiu $v1, $v1, 0x20` is. 14 words, 0 differing. */
void func_800A6104(void)
{
    u8 *p;

    p = D_800DCF64;
    if (p != NULL)
    {
        *(s32 *)(p + 0x20) = 0xFFFFFF;
        p += 0x20;
        *(s32 *)(p + 0x4) = 0x40;
        *(s16 *)(p + 0xC) = 1;
    }
}

/* 22 words, 0 differing. */
UnkNode *func_800A613C(void)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        func_8004491C(n, (u32)func_800A5FC0);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 6;
        *(s16 *)(p + 0xC) = 0x10;
    }
    return n;
}

/* A node's +0x10 step: fade the level at +0x0A up to 0xFF over the frames
   left at +0x08.

   THE `|=` ON D_800E296C HAS TO BE SPLIT AROUND THE +0x08 STORE. gcc cannot
   prove the store through `p` misses the global, so written as one compound
   assignment the `sh` is pinned either ahead of the `lw` or behind the `sw`,
   and the target has it BETWEEN them. All six orderings of the arm's three
   statements were measured: 3, 3, 4, 8 differing and two at 44 words.
   46 words, 0 differing. */
void func_800A6194(u8 *p)
{
    s32 t;
    s32 f;

    t = --*(s16 *)(p + 0x8);
    if (t > 0)
    {
        *(s16 *)(p + 0xA) += (0xFF - *(s16 *)(p + 0xA)) / t;
    }
    else
    {
        *(s16 *)(p + 0xA) = 0xFF;
        f = D_800E296C;
        *(s16 *)(p + 0x8) = 0;
        D_800E296C = f | 0x2000000;
    }
    if ((D_800E296C & 0x1000000) == 0)
    {
        D_800E296C &= ~0x2000000;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A node handler: take a 0xC-byte and a 0x18-byte primitive off D_80083160's
   +0x8D0 cursor and hang both on an OT row.

   THE CURSOR MUST BE READ THROUGH THE STRUCT FIELD. With
   `b = *(u8 **)&D_80083160;` and a `u8 *` store -- func_8009DA70's spelling,
   which is right for ONE allocation -- cse forwards the first bump into the
   second read, deletes the dead store and folds both allocations into one
   `addiu +0x24`: 61 words against 64, 62 differing. Reading the COMPONENT_REF
   and storing through `*(u32 *)` makes each store invalidate the next read,
   which is the two reloads the target has. The discriminator is the READ's
   spelling, not the store's. 64 words, 0 differing. */
s32 func_800A624C(u8 *p)
{
    u8 *q;
    u8 *r;
    s32 c;

    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, 0x40, 0);
    c = *(s16 *)(p + 0xA);
    *(s32 *)(r + 0x4) = c | (c << 8) | (c << 16);
    SetPolyF4(r);
    SetSemiTrans(r, 1);
    *(s32 *)(r + 0x8) = 0;
    *(s32 *)(r + 0xC) = 0x140;
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x14) = 0xE00140;
    AddPrim(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), r);
    AddPrim(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), q);
    return 0;
}

/* The width of arg0 is NOT witnessed: the only caller passes a constant 0
   and the callee only stores it with `sh`, so s16 and s32 are the same
   bytes. s16 matches the landed func_800BC228, one constant over.
   27 words, 0 differing. */
UnkNode *func_800A634C(s16 arg0)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A6194;
        func_8004491C(n, (u32)func_800A624C);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 8;
        *(s16 *)(p + 0xC) = arg0;
    }
    return n;
}

/* func_800A6480's message with the noun chosen by `k`. The whole chain is
   duplicated into both arms and cross-jumping merges it back from the
   `jal func_80099194` onwards: one shared func_80099368 call ahead of the
   `if` is 46 words of 50 and 43 differing.

   THE `__asm__` MARKER IS A DEVICE and it is the whole difference between 0
   and 9. Without it `q` (3 references over 16 insns, 1 * 3 / 16) loses $s1 to
   the handle pseudo (4 over 34, 2 * 4 / 34) in global.c's allocno_compare.
   The marker's fourth reference on `q` crosses floor_log2's step (2 * 4 / 16
   = 5000 against 2352) and emits nothing. Seven structural spellings were
   ruled out at 50 words / 9 differing, so no source spelling reaches it.
   50 words, 0 differing. */
void func_800A63B8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    /* DEVICE: a zero-instruction reference on q, worth the whole 9-differing
       residue above and no words. */
    __asm__("" ::"r"(q));
    if (k == 0x15)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A03, h);
    }
    else
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A12, h);
    }
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* The smallest of the nineteen: the reused-temporary idiom and nothing
   else. See func_800BD45C for why `h` is the name that is written again.
   34 words, 0 differing. */
void func_800A6480(u8 *p, u8 *q)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099368(q, h);
    h = func_80099194(D_800E0A1B, h);
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* 9 words, 0 differing. */
void func_800A6508(void)
{
    func_800997FC(D_800E092B);
}

/* D_800DCF3C/44/48 are D_800DCF08[13], [15] and [16] by address, but the
   target gives each its own `lui`/`lw %lo(SYM)` pair and all four names are
   in splat/dungeon.bin_undefined_syms_auto.txt, so they are written as
   separate symbols; only the VARIABLE index goes through the array face.
   `s2 = 7` in the loop preheader is loop.c hoisting the `i == 7` constant.
   61 words, 0 differing. */
void *func_800A652C(u8 *p)
{
    s32 i;
    s16 r;

    if (p[0x13] != 0)
    {
        if (p[0x25] == 0)
        {
            return D_800DCF44;
        }
        if ((*(u32 *)(p + 0x1C) & 0x20000) == 0)
        {
            return D_800DCF3C;
        }
    }
    for (i = 0xE; i >= 0; i--)
    {
        r = func_80042900((s8 *)p, i);
        if (r > 0 && i == 7)
        {
            return D_800DCF48;
        }
        if (r != 0)
        {
            return D_800DCF08[i];
        }
    }
    return D_800DCF08[0];
}

/* Release the D_800E3DF0 slot the actor's +0x3 byte names, close its record
   down and report the ring index. THE MODE ARGUMENT IS A TERNARY, not the
   `if/else` into a local that func_800AA258 uses next to the same call:
   with the local, the two `lbu` argument loads stay below the branch and
   the function is 88 words / 5 differing, with exactly those two loads and
   the branch rotated; as a ternary both loads rise above it.
   ONE LOCAL CARRIES BOTH THE SLOT INDEX AND THE RESULT -- they share $s1,
   which is what two disjoint ranges in one declared local do.
   0x80010A80 is a constant address (`lui`/`ori`, not `lui`/`addiu`).
   88 words, 0 differing. */
s32 func_800A6620(u8 *p, s32 f)
{
    u8 *q;
    u8 *base;
    u8 *e;
    s32 r;

    r = p[3] & 0x1F;
    q = (u8 *)D_800E3DF0[r];
    D_800E3DF0[r] = 0;
    r = q[0x43];
    if (r < 0x40)
    {
        q[0x44] = 0;
        func_800422DC((u8 *)(r * 0x54 + 0x80010A80), q);
    }
    if ((p[3] & 0x20) != 0)
    {
        r = (s16)func_800A1BD0(q);
        if (r >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(r * 4 + base + 0xAC) = 0;
            *(s32 *)(r * 4 + base + 0xD0) = 0;
        }
        if ((s16)func_80042900((s8 *)q, 0x1B) == 0)
        {
            e = *(u8 **)(q - 0x14);
            func_8009A3D0(e[0x24], e[0x25], (*(s32 *)(q + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        if (f != 0)
        {
            func_800A32A4(q);
        }
        func_8009A028(q);
        *(u16 *)(q - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_80098B38(p);
    return r;
}

/* The "is the shop open" probe. ONE 0x80010000 BASE, shared by the two
   globals.

   AND THE FIRST READ MUST BE AN ARRAY REF, which is the whole fight. Spelled
   `*(s32 *)(b + 0x2090)` the MEM is not MEM_IN_STRUCT_P, so true_dependence
   keeps a dependence against the `sb` to D_80080A88 -- its second exemption
   needs the READ in-struct and the WRITE's address non-varying, and a
   LO_SUM's address does not vary -- the store stays pinned ahead of the load
   and the `beq` keeps a `nop`: 30 words, 28 differing. As `((s32 *)b)[0x824]`
   expr.c stamps the ARRAY_REF in-struct, the exemption fires, and reorg takes
   the store into the delay slot. A pointer local frees the slot but
   rematerialises the base, 30 / 25. 29 words, 0 differing. */
s32 func_800A6780(void)
{
    u8 *b;
    s32 v;
    s32 t;

    b = (u8 *)0x80010000;
    D_80080A88 = 0;
    v = ((s32 *)b)[0x824];
    if (v == 1)
    {
        D_80080A88 = 1;
        return 1;
    }
    if (v != 0)
    {
        return 0;
    }
    t = ((s32 *)b)[0x8D];
    if (t / 5 != (t + 1) / 5)
    {
        return 0;
    }
    D_80080A88 = 1;
    return 1;
}

/* D_800DCF4F IS READ WITH `lb`, so its declaration above was corrected from
   u8 to s8; as u8 the test is `lbu` and the word is wrong. 31 words,
   0 differing. */
void func_800A67F4(void)
{
    u8 *g;

    if ((D_800E296C & 0x200000) == 0)
    {
        if ((D_80013714 & 8) == 0)
        {
            if (D_800DCF4F != 0)
            {
                D_800DCF4F = 0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) -= 1;
            }
            func_80035208(D_800E045C);
            func_80035348();
        }
    }
}

/* 20 words, 0 differing. */
s32 func_800A6870(s16 x)
{
    if (x == 0xFF)
    {
        x = 0x1E;
    }
    return (x >> 2) + (func_800A6D30() & 3);
}

/* Save bit 4 of the engine record's +0x1C flag word, then drop it. The
   field is re-read for the mask because the intervening byte store
   invalidates cse's entry. 13 words, 0 differing. */
void func_800A68C0(void)
{
    u8 *p;

    p = D_800E3D7C;
    D_800DD878 = (*(u32 *)(p + 0x1C) >> 4) & 1;
    *(u32 *)(p + 0x1C) &= ~0x10;
}

/* func_800A68C0's restore. 13 words, 0 differing. */
void func_800A68F4(void)
{
    u8 *p;

    if (D_800DD878 != 0)
    {
        p = D_800E3D7C;
        *(u32 *)(p + 0x1C) |= 0x10;
    }
}

/* func_80098FF8's base rule again; here splat could not pattern-match the
   `lui 0x8001`/`ori 0x4C` at all, which is the proof there is no
   relocation. 21 words, 0 differing. */
s16 func_800A6928(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 n;
    s32 i;

    t = (u8 *)0x80010000;
    key = (a << 8) | b;
    n = 0;
    for (i = 0x13; i >= 0; i--)
    {
        if (**(u16 **)(t + i * 4 + 0x29C) == key)
        {
            n++;
        }
    }
    return n;
}

/* func_800A6928's backward scan over a second 64-entry stride-4 table.

   THE BASE CONSTANT CARRIES THE WHOLE ADDRESS: `t = (u8 *)0x80011F80` and
   `t + i * 4`. Written func_800A6928's way -- 0x80010000 with the table
   offset in the index, as `+ 0x1F80`, as `(i + 0x7E0) * 4`, or as
   `&((u32 *)t)[i + 0x7E0]` -- fold distributes the constant back out and
   loop.c leaves it in the load's DISPLACEMENT, so the giv's initial value is
   0x800100FC: 26 words, 2 differing. 26 words, 0 differing. */
s16 func_800A697C(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 i;

    t = (u8 *)0x80011F80;
    key = (a << 8) | b;
    for (i = 0x3F; i >= 0; i--)
    {
        if (*(u16 *)(t + i * 4) == key)
        {
            return 1;
        }
    }
    return func_800A6928(a, b);
}

/* Sixteen calls with a rising s16 argument from 2, then one per unit of
   func_800A6DA4's halfword result.

   THE RISING ARGUMENT IS `(s16)(i + 2)`, NOT AN `s16` COUNTER. With a second
   variable `s16 k = 2; ... k++` the sign extension stays at the use and the
   loop costs two extra words. Cast inline, loop.c strength-reduces
   `(i + 2) << 16` into the induction variable the target walks by 0x10000.
   The second bound is masked at the ASSIGNMENT: declared `u16` the mask
   lands at the guard instead and costs a word. 34 words, 0 differing. */
void func_800A69E4(void)
{
    s32 i;
    s32 n;

    n = func_800A6DA4(4, 8) & 0xFFFF;
    for (i = 0; i < 0x10; i++)
    {
        func_800A08A0((s16)(i + 2));
    }
    for (i = 0; i < n; i++)
    {
        func_800A08A0(1);
    }
}

/* A two-byte stack record handed to a ONE-argument callee -- the `jal`'s
   delay slot is the second `sb`, not an argument. 12 words, 0 differing. */
void func_800A6A6C(void)
{
    u8 buf[2];

    buf[0] = 1;
    buf[1] = 0x12;
    func_800A90E8(buf);
}

/* Clear bit 0x400 on every entry of every descriptor's 0x14-stride table,
   then re-set it from the 19x8-byte bitmap at 0x80015720.

   THREE SPELLINGS. (1) The bitmap is a CONSTANT BASE, not the symbol: a
   register index never lets cse fold a symbol's `%lo` into the MEM, so the
   declared array spends an `addiu %lo` the target does not have (54 words).
   (2) The mask has to be a named `s32`: `*(u16 *)q &= ~0x400` is narrowed by
   the front end to `andi 0xFBFF` (3 differing), where the target keeps -0x401
   in a register. (3) Both invariants are assigned INSIDE the outer loop so
   loop.c hoists them; written above the loop they are preheader STATEMENTS
   and sit ahead of `j = 0`, 3 differing. 53 words, 0 differing. */
void func_800A6A9C(void)
{
    s32 j;
    s32 i;
    s32 v;
    s32 m;
    u8 *t;

    for (j = 0; j < 0x13; j++)
    {
        m = ~0x400;
        t = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            v = *(u16 *)(D_80073414[j].unk_0C + i * 0x14);
            v &= m;
            *(u16 *)(D_80073414[j].unk_0C + i * 0x14) = v;
            if ((t[j * 8 + i / 8 + 0x5720] >> (i & 7)) & 1)
            {
                *(u16 *)(D_80073414[j].unk_0C + i * 0x14) |= 0x400;
            }
        }
    }
    func_800A6A6C();
}

/* 19 words, 0 differing. */
void func_800A6B70(u8 *p, s32 b, UnkDungeonSprite *s)
{
    s32 v;

    v = *(s32 *)(p + 0xC) + 1;
    *(s32 *)(p + 0xC) = v;
    v = (func_800644B8(v * 64) >> 7) + 0x60;
    s->unk_0E = v;
    s->unk_0D = v;
    s->unk_0C = v;
}

/* `s = step;` FIRST, `q = p + 2;` SECOND. With the s16 parameter used
   directly the sign-extension pair is scheduled to the end of the preheader
   (4 differing in every control-flow spelling); copied out first the
   preheader comes out sll / sra / addiu q / acc = 0. A `goto` back-edge is
   12-17 differing -- loop.c never sees the loop and the two cursors are not
   givs. 17 words, 0 differing. */
void func_800A6BBC(u8 *p, s16 step)
{
    s32 s;
    u8 *q;
    s32 acc;

    s = step;
    q = p + 2;
    acc = 0;
    for (;;)
    {
        *q += acc;
        acc += s;
        if (*(s8 *)p >= 0)
        {
            q += 0xC;
            p += 0xC;
            continue;
        }
        break;
    }
}

/* The 0x12-kind node for the dungeon's own HUD panel. `-r[3] + 8`, NOT
   `8 - r[3]`: the subtraction form materialises the 8 with an `li` and emits
   `subu $a2, $a2, $v0`, where the target negates the byte in place and adds
   -- 7 differing, all in those three words. 76 words, 0 differing. */
UnkNode *func_800A6C00(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkDungeonSprite *s;
    u8 *q;
    u8 *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        m = n->unk_08;
        s = (UnkDungeonSprite *)n->unk_0C;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = 0x3C;
        *(s16 *)((u8 *)m + 0x6) = 0x40;
        *(s16 *)(q + 0x8) = 1;
        s->unk_1E = 0x1800;
        s->unk_1C = 0x1800;
        n->unk_10 = (u32)func_800A6B70;
        func_8004491C(n, (u32)func_800B06F0);
        r = (u8 *)n + 0x30;
        *(u32 *)q = (u32)((u8 *)n + 0xF0);
        *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0xC0);
        *(s32 *)((u8 *)n + 0xF0) = func_8004DA74(r, D_80089074, 0) & 0x7FFFFFFF;
        func_800B1320(r, 0xA8, (s16)(-*(s8 *)(r + 0x3) + 8));
        func_800A6BBC(r, 4);
        func_800B135C(r, 8);
        func_800B13CC(r, 0x20);
        func_800B1400(r, 2);
        *(u32 *)((u8 *)n + 0xC0) = 0x808080;
        *(s32 *)((u8 *)n + 0xF4) = 0;
    }
    return n;
}

/* The LCG. D_80083160 + 0x1FC is the seed, and with two accesses it arrives
   as a base pointer with folded displacements rather than as D_8008335C.
   12 words, 0 differing. */
u16 func_800A6D30(void)
{
    u8 *g;
    s32 v;

    g = (u8 *)&D_80083160;
    v = *(s32 *)(g + 0x1FC) * 0x41C64E6D + 0x3039;
    *(s32 *)(g + 0x1FC) = v;
    return ((u32)v >> 16) & 0x7FFF;
}

/* ONE BASE LOCAL FOR BOTH ARMS: the `addiu %lo(D_80083160)` sits in the
   `beqz` delay slot and both sides use it. The halfword parameter is only
   tested, so its `sra` is combined away and the bare `sll` is what is left.
   14 words, 0 differing. */
void func_800A6D60(s16 flag)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (flag != 0)
    {
        D_800DD87C = *(s32 *)(g + 0x1FC);
    }
    else
    {
        *(s32 *)(g + 0x1FC) = D_800DD87C;
    }
}

void func_800A6D98(s32 v)
{
    D_8008335C = v;
}

/* A random number in the inclusive range the two arguments bound, in either
   order.

   ONE LOCAL DOES THE SWAP AND THE DRAW. With a separate `s32 r` for the call
   result -- or with the call written inline, or `lo + ...` operand order, or
   signed parameters with casts, or `lo > hi` for `hi < lo` -- the
   `andi $v0, 0xFFFF` lands in $v1 and the divisor in $v0: 27 words, 4-5
   differing. Reusing the swap temporary for the draw gives that pseudo the
   second set that keeps it in $v0. The divisor needs its own local either
   way. 27 words, 0 differing. */
s32 func_800A6DA4(u32 lo, u32 hi)
{
    u32 t;
    u32 n;

    if (hi < lo)
    {
        t = hi;
        hi = lo;
        lo = t;
    }
    t = func_800A6D30();
    n = hi - lo + 1;
    return (u16)(t % n + lo);
}

/* COUNT the live slots standing on a tile -- func_800A7030's sweep with a
   counter instead of a clear. Both tables are indexed off the loop counter
   for the reason recorded there, and the count is initialised before the
   counter, which is the `addu $a3, $t0, $zero` the listing opens with.
   31 words, 0 differing. */
s16 func_800A6E10(s16 x, s16 y)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if (D_800E36C8[i].unk_00 == x)
            {
                if (D_800E36C8[i].unk_01 == y)
                {
                    n++;
                }
            }
        }
    }
    return n;
}

/* The nearest matching spot on D_800E3548/D_800E36C8, searched from slot 0x3F
   down: the slot's +0x01 kind must equal the key's low byte, its +0x00 must
   equal the key's second byte when that byte is nonzero, and the spot must be
   on the floor the record's +0x26 names. The winner's coordinates go to the
   two out-parameters and the return is whether anything matched.

   Four spellings, each worth what the next line says:
   * `i = 0x3F;` IS ITS OWN STATEMENT AHEAD OF `lo`/`hi`, with the `for`
     reduced to `for (; i >= 0; i--)`. With the init in the `for` the counter's
     `li $s4, 0x3F` lands AFTER the `andi`/`sw` pair in the preheader and that
     is 3 differing -- the preheader runs in source order and the `for`'s init
     is the last source statement before it.
   * The two absolute values are TERNARIES, `dx = (dx < 0) ? -dx : dx;`. As
     `if (dx < 0) { dx = -dx; }` the sign-extension of dx for the comparison
     moves out of the +0x25 block into the compare block, a `nop` takes its
     place, and it is 106 words / 51 differing.
   * The test is `best > d`, not `d < best`: the other order evaluates d's
     `sll` before best's and swaps $a2/$a3 for the flag and the maximum
     (105 / 10).
   * The maximum is written TWICE, once into `d` and once into `best`. With
     `best = d;` the three-word re-select collapses to one `move` and the
     function is 101 words / 61 differing.
   `lo` is an `s32` holding `key & 0xFF`, not a `u8`: as a `u8` local it is
   spilled with `sb`/`lbu` instead of the target's `andi`/`sw`/`lw`
   (112 words / 91 differing). 105 words, 0 differing. */
s32 func_800A6E8C(u8 *p, s32 key, s16 *ox, s16 *oy)
{
    s32 i;
    s32 lo;
    s8 hi;
    s16 best;
    s16 found;
    s16 dx;
    s16 dy;
    s16 d;
    s32 x;
    s32 y;

    if (*(s8 *)(p + 0x26) < 0)
    {
        return 0;
    }
    best = 0x100;
    found = -1;
    i = 0x3F;
    lo = key & 0xFF;
    hi = key >> 8;
    for (; i >= 0; i--)
    {
        if (D_800E3548_s[i].unk_01 != lo)
        {
            continue;
        }
        if (hi != 0 && D_800E3548_s[i].unk_00 != hi)
        {
            continue;
        }
        x = D_800E36C8[i].unk_00;
        y = D_800E36C8[i].unk_01;
        if (func_8009FB34(x, y) != *(s8 *)(p + 0x26))
        {
            continue;
        }
        dx = x - p[0x24];
        dx = (dx < 0) ? -dx : dx;
        dy = y - p[0x25];
        dy = (dy < 0) ? -dy : dy;
        d = (dx < dy) ? dy : dx;
        if (best > d)
        {
            best = (dx < dy) ? dy : dx;
            *ox = x;
            *oy = y;
            found = i;
        }
    }
    return found >= 0;
}

/* Clear any of the 0x40 live slots standing on the given tile and report it.
   D_800E3548's +0x01 is the slot's live flag and D_800E36C8 holds the tile.

   BOTH TABLES ARE INDEXED OFF THE COUNTER, not walked as pointers, and the
   tile bytes are held in LOCALS. Three measurements behind that: a `u8 *`
   walk over the wide table collapses its two field reads into one giv
   biased by the later offset and loses the base (`addiu $s0, $v0, 7`, reads
   at -1 and 0) where the target keeps the base and uses +6/+7; a
   hand-initialised pointer for the narrow table is a biv rather than a giv,
   which reverses the order loop.c emits the two initialisations in; and
   passing `D_800E36C8[i].unk_00` to the call a second time costs two
   reloads, because the word store to the slot between them invalidates
   cse's memory. 45 words, 0 differing. */
void func_800A7030(s16 x, s16 y)
{
    s32 i;
    u8 cx;
    u8 cy;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            cx = D_800E36C8[i].unk_00;
            if (cx == x)
            {
                cy = D_800E36C8[i].unk_01;
                if (cy == y)
                {
                    *(s32 *)&D_800E3548_s[i] = 0;
                    func_8009A3D0(cx, cy, 0x800);
                }
            }
        }
    }
}

/* func_800A7030's sweep as a SEARCH: the first live slot on (x, y) whose
   +0x04 facing is within 0x40 of `ang`. The +0x04 halfword is read with `lh`
   through an explicit `(s16)` cast rather than through the D_800E36C8_u
   alias: a second `extern` under the same `__asm__` name would defeat cse's
   unification of the bases, and the target has one `addiu` for all three
   reads. 68 words, 0 differing. */
s16 func_800A70E4(s16 x, s16 y, s16 ang)
{
    s32 i;
    s32 d;
    u16 m;

    func_8009A350(x - 1, y, 0, &m);
    if ((m & 0x800) == 0)
    {
        return -1;
    }
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            d = ang - (s16)D_800E36C8[i].unk_04;
            if (d < 0)
            {
                d = -d;
            }
            if (d < 0x40)
            {
                return i;
            }
        }
    }
    return -1;
}

/* Find the first free slot of a stride-4 table by its +0x01 byte, returning
   the index as a halfword and -1 when the table is full. The index is cast
   on the way out -- that is the `sll`/`sra` pair in the delay slots -- while
   the -1 needs no widening, which is why only one arm has it. */
s16 func_800A71F4(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3548; i < 0x40; i++, p += 4)
    {
        if (p[1] == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Find a free tile for a new spot near (x, y): the tile west of it, then the
   eight neighbours starting at D_8008347E's facing, then the sixteen offsets
   in D_800DCE6C/D_800DCE8C starting at D_8008347E's low nibble. A candidate
   passes when func_8009A350 reports the tile exists, none of the 0x8820 flags
   is set, and the floor height func_800BCB04 gives for its centre is below
   0x200; the tile and that height go out through the three pointers.
   197 words, 0 differing.

   FOUR SPELLINGS, each measured against the landing:
   (1) `r` IS `s32` WITH THE `(s16)` AT THE COMPARE. As `s16` the first
   probe's result lands in $v1 while both loops' land in $a0, so jump.c cannot
   cross-jump the three `*oz = r; return 1` tails into one: 200 words, 150
   differing.
   (2) EACH OF THE THREE PROBES NEEDS ITS OWN PAIR of func_800BCB04 argument
   locals. One pair shared by all three is a global allocno and the `<< 6`
   chain comes out of $v1/$v0 with the final `andi` copying into $a0/$a1
   instead of running in $a0/$a1 from the load: 27 differing.
   (3) THE TABLES ARE INDEXED, NOT WALKED THROUGH `s16 *` LOCALS. Pointer
   locals are two more allocnos and they outrank the direction counter in
   global-alloc, so the three callee-saved registers come out permuted
   ($s0/$s1/$s2 against the target's $s2/$s0/$s1) and the scheduler can no
   longer hoist the `z - 0x20` and `&m` setups past them: 32 differing.
   (4) THE EIGHT-WAY LOOP'S INDEX IS AN `s16` COPY of the `s32` counter --
   that is the `addu $s0, $s1, $zero` / `addu $a2, $s0, $zero` pair. An `s32`
   copy is propagated away by cse and the function is a word short: 196 words,
   120 differing. The `s16` copy survives cse because the modes differ, and
   combine still drops the sign-extension at the use because the counter is
   masked to 0..7.

   The counter has to be stepped BEFORE the index, which is why both loops are
   `while` and not `for (i = 0; i < 8; i++)` with the mask last: the other
   order fills both early-exit delay slots from the wrong block, 59
   differing. */
s32 func_800A7234(s16 x, s16 y, s16 z, s16 *ox, s16 *oy, s16 *oz)
{
    u16 m;
    s32 r;
    s16 gx;
    s16 gy;
    s32 i;
    s16 j;
    s32 k;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 cx;
    s32 cy;

    if ((s16)func_8009A350(x - 1, y, 0, &m) != 0)
    {
        if ((m & 0x8820) == 0)
        {
            ax = x << 6;
            ax = (ax + 0x20) & 0xFFE0;
            ay = y << 6;
            ay = (ay + 0x20) & 0xFFE0;
            r = func_800BCB04(ax, ay, z - 0x20);
            if ((s16)r < 0x200)
            {
                *ox = x;
                *oy = y;
                *oz = r;
                return 1;
            }
        }
    }
    gx = x;
    gy = y;
    k = D_8008347E & 7;
    i = 0;
    while (i < 8)
    {
        j = k;
        if ((s16)func_8009A350(gx, gy, j, &m) != 0)
        {
            if ((m & 0x8820) == 0)
            {
                bx = (gx + D_8006CCD8[j]) << 6;
                bx = (bx + 0x20) & 0xFFE0;
                by = (gy + D_8006CCE8[j]) << 6;
                by = (by + 0x20) & 0xFFE0;
                r = func_800BCB04(bx, by, z - 0x20);
                if ((s16)r < 0x200)
                {
                    *ox = x + (u16)D_8006CCD8[j];
                    *oy = y + (u16)D_8006CCE8[j];
                    *oz = r;
                    return 1;
                }
            }
        }
        i++;
        k = (k + 1) & 7;
    }
    gx = x;
    gy = y;
    k = D_8008347E & 0xF;
    i = 0;
    while (i < 0x10)
    {
        if ((s16)func_8009A350(x + (u16)D_800DCE6C[k] - 1, y + (u16)D_800DCE8C[k], 0, &m) != 0)
        {
            if ((m & 0x8820) == 0)
            {
                cx = (gx + D_800DCE6C[k]) << 6;
                cx = (cx + 0x20) & 0xFFE0;
                cy = (gy + D_800DCE8C[k]) << 6;
                cy = (cy + 0x20) & 0xFFE0;
                r = func_800BCB04(cx, cy, z - 0x20);
                if ((s16)r < 0x200)
                {
                    *ox = x + (u16)D_800DCE6C[k];
                    *oy = y + (u16)D_800DCE8C[k];
                    *oz = r;
                    return 1;
                }
            }
        }
        i++;
        k = (k + 1) & 0xF;
    }
    return 0;
}

/* func_800A7700's entity, stepped: case 0 arms the 4-frame countdown, case 1
   swings the renderable's +0x1C/+0x1E by rsin/rcos of a rising angle, case 2
   unwinds it, bleeds the mover's +0x14 and fades the three colour bytes, and
   the mover's +0x08 takes the accumulated +0x14 on every path.
   110 words, 0 differing, first compile.

   The switch's `case 0` FALLS THROUGH into `case 1`, and `*(s16 *)(a + 0xA4)
   = 1;` in case 2 reuses the constant 1 the switch head already materialised
   for its `beq`. */
void func_800A7548(u8 *a, UnkVec *b, u8 *c)
{
    switch (*(s16 *)(a + 0xA0))
    {
    case 0:
        *(s16 *)(a + 0xA6) = 4;
        *(s16 *)(a + 0xA4) = 0;
        *(s16 *)(a + 0xA0) = *(u16 *)(a + 0xA0) + 1;
    case 1:
        *(s16 *)(a + 0xA4) = *(u16 *)(a + 0xA4) + 1;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + func_800644B8(*(s16 *)(a + 0xA4) * 32);
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - func_800644B8(*(s16 *)(a + 0xA4) * 32);
        *(s16 *)(a + 0xA6) = *(u16 *)(a + 0xA6) - 1;
        if (*(s16 *)(a + 0xA6) <= 0)
        {
            *(s16 *)(a + 0xA0) = *(u16 *)(a + 0xA0) + 1;
        }
        break;
    case 2:
        *(s16 *)(a + 0xA4) = *(u16 *)(a + 0xA4) - 1;
        if (*(s16 *)(a + 0xA4) <= 0)
        {
            *(s16 *)(a + 0xA4) = 1;
        }
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - func_800644B8(*(s16 *)(a + 0xA4) * 32) * 2;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + func_800644B8(*(s16 *)(a + 0xA4) * 32) * 4;
        b->unk_14 -= 0x20000;
        c[0xC] = c[0xD] = c[0xE] -= 4;
        if (*(s16 *)(c + 0x1C) <= 0)
        {
            *(u16 *)(a - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    b->unk_08 += b->unk_14;
}

/* The node spawner for func_800A7548's entity. THE THIRD PARAMETER IS `s32`
   although it is only ever `sh`-stored: as s16 (or u16) its HImode pseudo
   outranks the other three in global-alloc and the four parameter copies come
   out $s0/$s2/$s3/$s1 against the target's $s1/$s3/$s0/$s2 -- 43 words, 13
   differing, every one a register. Caching n->unk_08 in a local is required:
   reaching it twice is 46 words / 31. 43 words, 0 differing. */
UnkNode *func_800A7700(s32 x, s32 y, s32 ang, s32 *q)
{
    UnkNode *n;
    UnkVec *m;

    n = (UnkNode *)func_800A8608(D_80083498, q, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    n->unk_10 = (u32)func_800A7548;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = (x << 6) + 0x20;
    *(s16 *)((u8 *)m + 0x6) = (y << 6) + 0x20;
    *(s16 *)((u8 *)m + 0xA) = ang;
    *(s32 *)((u8 *)n + 0xB8) = *q;
    return n;
}

/* Ease the mover onto the tile at +0xAA/+0xAC over the +0xA0 countdown, and
   on the last frame claim a D_800E3548 slot for it: the four-byte word at
   +0x98 goes into the slot, the two tile bytes, the +0xAE height twice and
   the renderer's +0x08 go into the matching D_800E36C8 record, and the view
   is re-aimed. 104 words, 0 differing.

   THE +0x02 AND +0x04 STORES SHARE ONE READ -- written as two statements
   each reading `*(u16 *)(p + 0xAE)` the second `lhu` and its load-delay nop
   survive, 106 words. A chained assignment or one `u16` local is 104.
   The third numerator has no `<< 6` and no `+ 0x20`: it is a raw height,
   where the first two are tile coordinates. */
void func_800A77AC(u8 *p, UnkVec *v, u8 *c)
{
    s16 i;
    s16 t;
    u8 *g;

    v->unk_00 += ((((*(s16 *)(p + 0xAA) << 6) + 0x20) << 16) - v->unk_00) / *(s16 *)(p + 0xA0);
    v->unk_04 += ((((*(s16 *)(p + 0xAC) << 6) + 0x20) << 16) - v->unk_04) / *(s16 *)(p + 0xA0);
    v->unk_08 += ((*(s16 *)(p + 0xAE) << 16) - v->unk_08) / *(s16 *)(p + 0xA0);
    t = *(u16 *)(p + 0xA0) - 1;
    *(s16 *)(p + 0xA0) = t;
    if (t <= 0)
    {
        i = func_800A71F4();
        if (i >= 0)
        {
            *(s32 *)&D_800E3548_t[i] = *(s32 *)(p + 0x98);
            D_800E36C8_u[i].unk_00 = p[0xAA];
            D_800E36C8_u[i].unk_01 = p[0xAC];
            D_800E36C8_u[i].unk_04 = D_800E36C8_u[i].unk_02 = *(u16 *)(p + 0xAE);
            D_800E36C8_u[i].unk_08 = *(s32 *)(c + 0x8);
            func_8009A21C(*(s16 *)(p + 0xAA), *(s16 *)(p + 0xAC), 0x800);
            func_800A4300(D_80082E80, D_800E3D7C);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212-style node through func_800A8608.

   THE THREE STACK PARAMETERS ARE `s32`. Declared s16 they are hoisted into
   $s0/$s1/$s2 at entry instead of being reloaded at their stores, and the
   function is 63 words against 59 with 60 differing -- the stack-parameter
   memory discount only applies while the parameter's mode is the slot's. AND
   THE RECORD READ NEEDS ITS OWN LOCAL: written inline at the +0x98 store the
   `lw` is scheduled eight words later, 59 / 9. 59 words, 0 differing. */
u8 *func_800A794C(s32 x, s32 z, s32 ang, u8 *p, s32 e, s32 f, s32 h)
{
    u8 *n;
    u8 *q;
    u8 *r;
    u8 *g;
    u8 *v;

    n = (u8 *)func_800A8608(D_80083498, p, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    *(u32 *)(n + 0x10) = (u32)func_800A77AC;
    q = *(u8 **)(n + 0x8);
    *(s16 *)(q + 0x2) = x * 64 + 0x20;
    *(s16 *)(q + 0x6) = z * 64 + 0x20;
    *(s16 *)(q + 0xA) = ang;
    r = n + 0x20;
    v = *(u8 **)p;
    *(s16 *)(r + 0xA0) = 4;
    *(s16 *)(r + 0xAA) = e;
    *(s16 *)(r + 0xAC) = f;
    *(u8 **)(r + 0x98) = v;
    *(s16 *)(r + 0xAE) = h;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    return n;
}

/* ONE RESULT VARIABLE, NOT TWO RETURNS: the call block is a branch target
   rather than a fall-through, so cse's table does not reach it and p[1] is
   reloaded for the call. Two returns let jump.c swap the arms (7
   differing). 17 words, 0 differing. */
u32 func_800A7A38(u8 *p)
{
    u32 r;

    if (p[1] == 0x12)
    {
        r = (u32)D_8006E240;
    }
    else
    {
        r = func_8004A658(p[1], p[0]);
    }
    return r;
}

/* Claim a stride-4 slot for a new spot, or report that it could not be done.
   The record's fields go in through UnkDungeonSpotD rather than a `u8 *`
   cast: see that type's comment for the four words it costs otherwise.
   122 words, 0 differing. */
s32 func_800A7A7C(s16 x, s16 y, s16 z, s32 u, u8 *w)
{
    void *m;
    void *h;
    s32 i;
    UnkDungeonSpotD *r;
    s16 a;
    s16 b;
    s16 c;

    if ((s16)func_800A7234(x, y, z, &a, &b, &c) != 0)
    {
        i = func_800A71F4();
        if (i >= 0)
        {
            if (func_800A794C(x, y, z, w, a, b, c) == 0)
            {
                *(s32 *)&D_800E3548_s[i] = *(s32 *)w;
                r = &D_800E36C8_u[i];
                r->unk_00 = a;
                r->unk_01 = b;
                r->unk_04 = c;
                r->unk_02 = c;
                r->unk_08 = func_800A7A38(w);
                func_8009A21C(a, b, 0x800);
                func_800A4300(D_80082E80, D_800E3D7C);
            }
            return 1;
        }
    }
    func_800A7700(x, y, z, w);
    h = func_800990FC();
    m = h;
    h = func_80099368(w, h);
    h = func_80099194(D_800E0B54, h);
    func_80099290(h);
    func_800A5720(m);
    return 0;
}

/* Allocate a kind-2 node, park a handler at its +0x10 and set the
   renderable it carries to white at unit scale. The `lw 0xC` and the
   `sw $zero, 0x10` both happen BEFORE the install call, which is where the
   reads of the node have to be: func_8004491C clobbers $v0. */
void func_800A7C64(void)
{
    UnkNode *n;
    UnkDungeonSprite *s;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        n->unk_10 = 0;
        func_8004491C(n, (u32)func_800A7CC4);
        s->unk_0E = 0x80;
        s->unk_0D = 0x80;
        s->unk_0C = 0x80;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
    }
}

/* The 0x40 ground-item slots: for each live D_800E3548 slot, project
   D_800E36C8's tile into the scratchpad, then emit TWO POLY_FT4s per element
   of its 0xC-byte list -- the sprite itself and, under a second matrix that
   has been flattened by ScaleMatrix and offset by six, a semi-transparent
   shadow whose colour word is the sprite's plus 0x7FC00000.  func_800D3738's
   renderer with the slot table's outer loop around it; the u/v + CLUT and
   u/v + TPAGE halves arrive as ONE `sw` each here, which is what
   UnkDungeonFT4W above exists for.

   SIX SPELLING FACTS, each measured on this function.

   (1) `s32 i = 0;` IS THE FIRST DECLARATION.  Left as `for (i = 0; ...)`
   after the four pointer initialisers, the counter's zeroing and its own
   `sw $s6` come out seventh and eighth instead of first and second: 4 aligned
   differences and nothing else wrong.

   (2) THE BYTE-OFFSET LOCAL IS TWO VARIABLES, ONE PER GROUP.  One `s16 a`
   across both the +0x02 and the +0x03 group keeps its quantity live over the
   whole stretch, so local-alloc never ties the sum to it, hands it $t0, and
   EVERY compiler temporary in the function shifts from $t0 to $t1 -- 87
   aligned differences, all of them register numbers.  Two locals die at their
   own sums, both get $v0, and the shift goes away.

   (3) `u16 cam[3]`, NOT `s16`.  Only the `(cam[2] + 0x100)` read shows it:
   `lhu` against `lh`, 1 word.  The array has to be an array (three `sh` into
   consecutive stack halfwords at 0x28); three scalars would be registers.

   (4) `Unk80083160 *g = &D_80083160;`.  All nine callee-saved registers are
   taken ($s0..$s7 and $fp), so `g` gets none and reload rematerialises its
   `lui`/`addiu` at each use -- which is exactly the target's three
   re-materialisations and its closing `lw 0x0($t0)`.  Reaching the struct by
   name instead gives the symbolic `lw %lo(D_80083160)($v0)` at the tail, one
   word short of the target's three.

   (5) EACH ADDEND OF `p->unk_102` IS IN ITS OWN LOCAL.  Inline, the
   `s->unk_18 - 0x100` half is computed in HImode as `ori $v1,0xFF00` plus
   `addu` rather than one `addiu`: one word long and 15 more aligned
   differences.

   (6) THE TILE'S +0x02 IS SEEDED FROM THE SCRATCHPAD, NOT FROM +0x04 AGAIN.
   `D_800E36C8_u[i].unk_02 = sp->unk_04;` lets cse forward the halfword it has
   just stored; `= D_800E36C8_u[i].unk_04;` reloads it, because the scratchpad
   store between the two invalidates cse's memory.  1 word.  The same
   mechanism is load-bearing in the shadow's scale: `sc.vz` is assigned twice
   with the second reading the first back, and the store between the two
   `(s16)sp->unk_04` reads is why they are not commoned -- which is where the
   target's dead `-4` store and its subtract-itself pair come from.

   593 words, 0 differing. */
s32 func_800A7CC4(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 i = 0;
    Unk80083160 *g = &D_80083160;
    MATRIX *m = (MATRIX *)0x1F8000D0;
    MATRIX *mm = (MATRIX *)0x1F800050;
    Unk453E0SP *sp = (Unk453E0SP *)0x1F800000;
    UnkDungeonFT4W *q;
    Unk44D24E *e;
    u16 cam[3];
    VECTOR sc;
    s32 k;
    s32 n;

    q = *(UnkDungeonFT4W **)(g->unk_00 + 0x8D0);
    sp->unk_20 = g->unk_00 + 0xB0;
    sp->unk_E4 = 0;
    sp->unk_E8 = 0;
    sp->unk_EC = 0;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    cam[0] = g->unk_C4;
    cam[1] = g->unk_C6;
    cam[2] = g->unk_C8;
    for (; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            sp->unk_00 = (D_800E36C8_u[i].unk_00 << 6) + 0x20;
            sp->unk_02 = (D_800E36C8_u[i].unk_01 << 6) + 0x20;
            sp->unk_04 = D_800E36C8_u[i].unk_04;
            D_800E36C8_u[i].unk_02 = sp->unk_04;
            e = (Unk44D24E *)D_800E36C8_u[i].unk_08;
            sp->unk_C0 = RotTransPers((void *)sp, &sp->unk_B8, &sp->unk_90, &sp->unk_94);
            D_8006CD30.unk_1C = sp->unk_C0 * 4;
            sp->unk_C0 = sp->unk_C0 - 2;
            if ((u32)sp->unk_C0 < 0x1E0)
            {
                PushMatrix();
                sp->unk_B8 = sp->unk_B8 - 0xA0;
                sp->unk_BA = sp->unk_BA - 0x78;
                sp->unk_100 = s->unk_16;
                sp->unk_104 = g->unk_B8 + (s->unk_1A - cam[1]);
                k = (cam[2] + 0x100) & 0x1FF;
                n = s->unk_18 - 0x100;
                sp->unk_102 = k + n;
                RotMatrix(&sp->unk_100, m);
                CompMatrix(&D_8006CD30, m, mm);
                SetTransMatrix(mm);
                SetRotMatrix(mm);
                for (;;)
                {
                    if ((e->unk_00 & 0x20) == 0)
                    {
                        s32 x;
                        s32 y;
                        s16 a;
                        s16 b;
                        u8 t;

                        x = e->unk_08;
                        sp->unk_08.w = x;
                        y = e->unk_0A;
                        sp->unk_10.w = y;
                        if (x + y >= 0x100)
                        {
                            sp->unk_10.w = y - 1;
                        }
                        x = e->unk_09;
                        sp->unk_0C.w = x;
                        y = e->unk_0B;
                        sp->unk_14.w = y;
                        if (x + y >= 0x100)
                        {
                            sp->unk_14.w = y - 1;
                        }
                        a = (s8)e->unk_02;
                        sp->unk_80 = a;
                        sp->unk_70 = a;
                        a = a + sp->unk_10.h;
                        sp->unk_88 = a;
                        sp->unk_78 = a;
                        b = (s8)e->unk_03;
                        sp->unk_7A = b;
                        sp->unk_72 = b;
                        b = b + sp->unk_14.h;
                        sp->unk_8A = b;
                        sp->unk_82 = b;
                        RotTransPers4(&sp->unk_70, &sp->unk_78, &sp->unk_80, &sp->unk_88,
                                      &sp->unk_F0, &sp->unk_F4, &sp->unk_F8, &sp->unk_FC,
                                      &sp->unk_90, &sp->unk_94);
                        q->unk_08 = sp->unk_F0 + sp->unk_B8;
                        q->unk_0A = sp->unk_F2 + sp->unk_BA;
                        q->unk_10 = sp->unk_F4 + sp->unk_B8;
                        q->unk_12 = sp->unk_F6 + sp->unk_BA;
                        q->unk_18 = sp->unk_F8 + sp->unk_B8;
                        q->unk_1A = sp->unk_FA + sp->unk_BA;
                        q->unk_20 = sp->unk_FC + sp->unk_B8;
                        q->unk_22 = sp->unk_FE + sp->unk_BA;
                        if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                             ((u16)(q->unk_10 + 0x20) < 0x181 && (u16)(q->unk_12 + 0x20) < 0x121) |
                             ((u16)(q->unk_18 + 0x20) < 0x181 && (u16)(q->unk_1A + 0x20) < 0x121) |
                             ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121)) !=
                            0)
                        {
                            sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
                            sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
                            sp->unk_0C.w = sp->unk_0C.w << 8;
                            q->unk_0C.w = sp->unk_0C.w + sp->unk_08.w + (e->unk_06 << 16);
                            q->unk_14.w = sp->unk_0C.w + sp->unk_10.w + ((s16)e->unk_04 << 16);
                            q->unk_1C.h = sp->unk_14.h + sp->unk_08.h;
                            q->unk_24.h = sp->unk_14.h + sp->unk_10.h;
                            if ((s16)sp->unk_50 >= 0x1800)
                            {
                                t = q->unk_24.b[0];
                                q->unk_24.b[0] = t + 0xFF;
                                q->unk_14.b[0] = t;
                            }
                            if ((s16)sp->unk_58 >= 0x1800)
                            {
                                t = q->unk_24.b[1];
                                q->unk_24.b[1] = t + 0xFF;
                                q->unk_1C.b[1] = t;
                            }
                            if ((s16)q->unk_08 > (s16)q->unk_20)
                            {
                                q->unk_14.b[0] = q->unk_14.b[0] - 1;
                                q->unk_24.b[0] = q->unk_24.b[0] - 1;
                            }
                            if ((s16)q->unk_0A > (s16)q->unk_22)
                            {
                                q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                                q->unk_24.b[1] = q->unk_24.b[1] - 1;
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            SetPolyFT4(q);
                            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
                            q++;
                            PushMatrix();
                            sc.vz = (s16)sp->unk_04 - 4;
                            sc.vz = (sc.vz - (s16)sp->unk_04) * 32 + 0x1000;
                            if (sc.vz < 0)
                            {
                                sc.vz = 0;
                            }
                            sc.vx = sc.vz;
                            sc.vy = sc.vz / 2;
                            sp->unk_102 = 0;
                            sp->unk_100 = -cam[0];
                            sp->unk_104 = g->unk_B8 - cam[1];
                            RotMatrix(&sp->unk_100, m);
                            CompMatrix(&D_8006CD30, m, mm);
                            ScaleMatrix(mm, &sc);
                            SetTransMatrix(mm);
                            SetRotMatrix(mm);
                            sp->unk_70 = sp->unk_70 + 6;
                            sp->unk_78 = sp->unk_78 + 6;
                            RotTransPers4(&sp->unk_70, &sp->unk_78, &sp->unk_80, &sp->unk_88,
                                          &sp->unk_F0, &sp->unk_F4, &sp->unk_F8, &sp->unk_FC,
                                          &sp->unk_90, &sp->unk_94);
                            q->unk_08 = sp->unk_F0 + sp->unk_B8;
                            q->unk_0A = sp->unk_F2 + sp->unk_BA;
                            q->unk_10 = sp->unk_F4 + sp->unk_B8;
                            q->unk_12 = sp->unk_F6 + sp->unk_BA;
                            q->unk_18 = sp->unk_F8 + sp->unk_B8;
                            q->unk_1A = sp->unk_FA + sp->unk_BA;
                            q->unk_20 = sp->unk_FC + sp->unk_B8;
                            q->unk_22 = sp->unk_FE + sp->unk_BA;
                            q->unk_0C.w = sp->unk_0C.w + sp->unk_08.w + 0x7FC00000;
                            q->unk_14.w = sp->unk_0C.w + sp->unk_10.w + ((s16)e->unk_04 << 16);
                            q->unk_1C.h = sp->unk_14.h + sp->unk_08.h;
                            q->unk_24.h = sp->unk_14.h + sp->unk_10.h;
                            if ((s16)q->unk_08 > (s16)q->unk_20)
                            {
                                q->unk_14.b[0] = q->unk_14.b[0] - 1;
                                q->unk_24.b[0] = q->unk_24.b[0] - 1;
                            }
                            if ((s16)q->unk_0A > (s16)q->unk_22)
                            {
                                q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                                q->unk_24.b[1] = q->unk_24.b[1] - 1;
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            SetPolyFT4(q);
                            setSemiTrans(q, 1);
                            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
                            q++;
                            PopMatrix();
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
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

/* The three halfword stores into the mover are the 16.16 integer halves at
   +0x2/+0x6/+0xA, which is why c, d and e are s16; no caller witnesses the
   width, so the `sh` stores are the whole evidence. The second parameter is
   `void *` and not `u8 *`: func_800A7700 passes its own `s32 *q`. This is
   also the witness for func_800BC26C's fourth parameter -- it sets all four
   argument registers before the `jal`. 53 words, 0 differing. */
UnkNode *func_800A8608(void *a, void *b, s16 c, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        *(s16 *)((u8 *)m + 0x2) = c;
        *(s16 *)((u8 *)m + 0x6) = d;
        *(s16 *)((u8 *)m + 0xA) = e;
        func_8004491C(n, (u32)D_80045340);
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_08 = func_800A7A38(b);
        func_800BC26C(n, 0, 0, 0);
    }
    return n;
}

/* THE BASE IS REMATERIALISED: one `b = ...` at the top is 15 words / 7
   differing because it stays live in $a1 across the loop. Assigning it
   again before the tail store is 0. The constant written inline at every
   site is 23 words. 16 words, 0 differing. */
void func_800A86DC(s32 i)
{
    u8 *b;

    b = (u8 *)0x80010000;
    for (; i < 0x13; i++)
    {
        *(s32 *)(b + i * 4 + 0x29C) = *(s32 *)(b + i * 4 + 0x2A0);
    }
    b = (u8 *)0x80010000;
    *(s32 *)(b + i * 4 + 0x29C) = 0;
}

/* func_800A8E74's node steps here: the OTHER 0x110-node +0x10 handler (see
   func_8008D470, which is this one's sibling), a two-state machine on the
   view's +0xA0. State 0 either walks the carrier's own tile one step along
   the stored facing (+0x1C bit 0x200000) or projects
   the carrier, seeds the mover from the view's +0x8C record and arms the
   +0xA2 flag; state 1 eases the mover onto c[0x24]/c[0x25] over the +0xA4
   countdown and, when it expires, either lands on a target func_8009B25C
   finds or steps one more tile. Every path that does not return falls into
   the three 16.16 integrations at the end. 470 words, 0 differing;
   seqcheck seq=OK with raw == relocdump == 71 and no relocation on a
   function symbol.

   THE DIRECTION INDEX `i` IS `s16`, AND THAT IS THE WHOLE LAST WORD. As
   `s32` the `andi $v1, $v0, 0x7` and the variable are one pseudo and the
   `addu $s4, $v1, $zero` at 0x800A8878 never exists: 470 words, 96
   differing. An `s16` local gets an HImode pseudo here, so the assignment
   is a mode-changing copy that jump2's find_equiv_reg cannot delete, while
   the value 0..7 needs no re-extension at either `sll i, 1`. `u16` and `u8`
   are 469 / 80. func_8008D470's own comment records the same mechanism on
   its `s16` latch, so this is the second measurement of it in this unit.
   One `s16 i` serves all three index sites; three separate `s32` locals are
   also 0 but a single `s32` one is 470 / 98.

   TWO OF THE NINE `b + 0xA` ACCESSES MUST BE THE INDEX FORM `*((s16 *)b + 5)`
   -- the `+ 8` store in the 0x80000 arm and the third argument of the SECOND
   func_800BCB04 in the big arm. Only the index form leaves a top-level
   PLUS_EXPR, which is what sets MEM_IN_STRUCT_P, so sched1 lifts
   `lw %lo(D_800814A8)` and `lh 0xA($s3)` over the store beside them and the
   load-delay slot stops being a `nop`: the flat `*(s16 *)((u8 *)b + 0xA)` is
   471 / 9 and 471 / 5 respectively. The other seven are ties, so they keep
   the unit's usual spelling.

   func_8009B164 IS CALLED THROUGH A PROTOTYPED CAST. Its landed definition
   takes `u16`, which makes the argument `lhu`; the target has `lh`, i.e. the
   original translation unit had no prototype in scope and the argument
   promoted to `int`. The cast emits the same `jal` and the same relocation.
   Calling it directly is 470 / 1.

   Also measured, each against the landed body: `h` as `s16` with the `(s16)`
   casts dropped 470 / 2 (the compares read the call's return register
   instead of h's own); one `(u8 *)&D_80083460` local instead of two 470 / 4;
   `x` and `y` merged, or hoisted out of the subtraction, 470 / 3 each
   (func_800AB1C0's idiom); `e` inlined twice instead of held in a local
   472 / 9, because the `sb` to c[0x24] wipes cse's memory table; dropping
   `q = p` and using one pointer 464 / 88. */
void func_800A871C(u8 *p, UnkVec *b, u8 *c)
{
    s16 out[4];
    u8 *q;
    u8 *d;
    u8 *e;
    u8 *g;
    u8 *g2;
    s16 i;
    s32 h;
    s32 x;
    s32 y;

    q = p;
    if (*(u8 **)(q + 0x9C) == D_80081484 && (*(u8 **)(q + 0x9C))[1] == 0)
    {
        *(u8 **)(q + 0x9C) = (u8 *)&D_800E3540;
    }
    switch (*(s16 *)(p + 0xA0))
    {
    case 0:
        if ((*(s32 *)(q + 0x1C) & 0x200000) != 0)
        {
            *(s16 *)(q + 0x2A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            if (func_8003DE58(*(u32 *)(*(u8 **)(*(u8 **)*(u8 **)*(u8 **)(*(u8 **)(p + 0x90) + 0x28) +
                                                (*(u8 **)(*(u8 **)(p + 0x90) + 0x2C))[2] * 4) +
                                       4),
                              *(u8 **)(p + 0x90), out, 0) == 0)
            {
                out[2] = -0x30;
            }
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(p + 0x8C) + 0xA) + (u16)out[2];
            c[0x24] = (*(u8 **)(p + 0x90))[0x24];
            c[0x25] = (*(u8 **)(p + 0x90))[0x25];
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(p + 0xA0) = *(u16 *)(p + 0xA0) + 1;
            *(s16 *)(q + 0x6A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            i = (*(u16 *)(q + 0x6A) >> 9) & 7;
            if (*(s16 *)(q + 0x8A) != 0)
            {
                h = func_800BCB04((((c[0x24] + D_8006CCD8[i] * *(s16 *)(q + 0x8A)) << 6) + 0x20) & 0xFFE0,
                                  (((c[0x25] + D_8006CCE8[i] * *(s16 *)(q + 0x8A)) << 6) + 0x20) & 0xFFE0,
                                  *(s16 *)((u8 *)b + 0xA));
                if ((s16)h < 0x200)
                {
                    b->unk_14 = (((s16)h - *(s16 *)((u8 *)b + 0xA)) << 15) / *(s16 *)(q + 0x8A);
                }
                else
                {
                    b->unk_14 = ((*(s16 *)(*(u8 **)(q + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA)) << 15) /
                                *(s16 *)(q + 0x8A);
                }
            }
            c[0x24] += *(u8 *)&D_8006CCD8[i];
            c[0x25] += *(u8 *)&D_8006CCE8[i];
            *(s16 *)(p + 0xA4) = 2;
            return;
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(p + 0x90) + 0x8), *(u8 **)(p + 0x90), out, 0) != 0 &&
            (*(s32 *)(q + 0x1C) & 0x80000) != 0)
        {
            *(s16 *)(p + 0xA2) = 1;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(p + 0x8C) + 0x2) + (u16)out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(p + 0x8C) + 0x6) + (u16)out[1];
            *((s16 *)b + 5) = *(u16 *)(*(u8 **)(p + 0x8C) + 0xA) + (u16)out[2] + 8;
            *(s16 *)(q + 0x88) = *(u16 *)((u8 *)D_800814A8 + 0x88);
        }
        else if (*(s16 *)(p + 0xA2) == 0)
        {
            return;
        }
        else if ((*(s32 *)(q + 0x1C) & 0x80000) == 0)
        {
            e = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
            *(s16 *)(q + 0x6A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            i = (*(u16 *)(q + 0x6A) >> 9) & 7;
            c[0x24] = e[0x24] + *(u8 *)&D_8006CCD8[i];
            c[0x25] = e[0x25] + *(u8 *)&D_8006CCE8[i];
            func_800A2B04(b, c[0x24], c[0x25]);
            h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(s16 *)((u8 *)b + 0xA));
            if ((s16)h > 0x200)
            {
                h = *(u16 *)((u8 *)b + 0xA);
            }
            func_800A7A7C(c[0x24], c[0x25], h, *(s32 *)(c + 0x8), *(u8 **)(p + 0x9C));
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            *(s16 *)((u8 *)b + 0xA) =
                func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *((s16 *)b + 5));
            func_800B66C8((u8 *)b);
            func_8009A028(q);
            return;
        }
        if (*(s32 *)(D_800E3D7C + 0x124) != 0)
        {
            break;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_8009A028(q);
        return;
    case 1:
        if (*(s16 *)(p + 0xA4) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(p + 0xA4);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(p + 0xA4);
        }
        *(s16 *)(p + 0xA4) = *(u16 *)(p + 0xA4) - 1;
        if (*(s16 *)(p + 0xA4) > 0)
        {
            break;
        }
        d = func_8009B25C(q, c[0x24], c[0x25], *(s16 *)(q + 0x88));
        if (d != NULL)
        {
            if (d[0x13] == 0x1F && (*(s32 *)(d + 0x1C) & 0x228) == 0 && D_800DD7DC != 0)
            {
                *(s32 *)(d + 0x14) |= 0x800000;
                *(s32 *)(d + 0x48) = **(s32 **)(p + 0x9C);
            }
            else if (func_80098920(d, *(u8 **)(p + 0x9C), 0xD, (s32)q) == 0)
            {
                goto cont;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            func_8009A028(q);
            g = (u8 *)&D_80083460;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
            return;
        }
    cont:
        if ((s16)((s32 (*)(s32, UnkVec *, u8 *))func_8009B164)(*(s16 *)(q + 0x6A), b, c) != 0)
        {
            if (--*(s16 *)(q + 0x8A) > 0)
            {
                func_800A2B04(b, c[0x24], c[0x25]);
                i = (*(u16 *)(q + 0x6A) >> 9) & 7;
                c[0x24] += *(u8 *)&D_8006CCD8[i];
                c[0x25] += *(u8 *)&D_8006CCE8[i];
                *(s16 *)(p + 0xA4) = 2;
                break;
            }
        }
        h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(s16 *)((u8 *)b + 0xA));
        if ((s16)h > 0x200)
        {
            h = *(u16 *)((u8 *)b + 0xA);
        }
        func_800A7A7C(c[0x24], c[0x25], h, *(s32 *)(c + 0x8), *(u8 **)(p + 0x9C));
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_8009A028(q);
        g2 = (u8 *)&D_80083460;
        *(s16 *)(g2 + 0xA) = *(u16 *)(g2 + 0xA) - 1;
        break;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
}

/* Spawn the 0x110 node func_800A871C steps, seeding the mover's three 16.16
   integer halves and the view's +0x88 either from the D_800E36C8 spot record
   `idx` names or, when `idx` is negative, from the caller's own record and
   source view. The tail is func_800A8608's word for word: the engine list,
   the three 0x80 colour bytes, the two 0x1000 scales, and func_800A7A38's
   handle into the renderable's +0x08.

   THE TABLE ARM IS WRITTEN FIRST. Both arms fall into the shared tail, so
   they are emitted in source order; writing the `idx < 0` arm first is
   108 words, 38 differing, with the `bltz` coming out as a `bgez`.

   THE +0x98 STORE GOES BEFORE THE +0x9C AND +0x13 STORES. Written last, the
   two `lw`s off `d` get different registers, the second one floats above its
   own store and the whole five-store block reschedules: 108 words,
   9 differing. Caching the two loads in one reused `s32` local instead is 11
   differing, and with the +0x98 load cached as well 4. Moving only the +0x98
   store between +0x9C and +0x13 is 107 words.

   The sixth parameter is `s16`: it arrives as a stack word (`lw` at
   0x44($sp)) and is re-extended with `sll 16`/`sra 16` at its one use, and
   both ASM callers pass a sign-extended halfword. The third is never read.
   108 words, 0 differing. */
UnkNode *func_800A8E74(u8 *a, u8 *b, s32 c, u8 *d, u8 *e, s16 idx)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;

    n = func_8003FD64(0x110, a - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A871C;
        p = (u8 *)n + 0x20;
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        *(u8 **)(p + 0x60) = d;
        if (idx >= 0)
        {
            *(s16 *)((u8 *)m + 0x2) = (D_800E36C8_u[idx].unk_00 << 6) + 0x20;
            *(s16 *)((u8 *)m + 0x6) = (D_800E36C8_u[idx].unk_01 << 6) + 0x20;
            *(s16 *)((u8 *)m + 0xA) = D_800E36C8_u[idx].unk_02;
            *(s16 *)(p + 0x88) = D_800E36C8_u[idx].unk_04;
        }
        else
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(b + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(b + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(b + 0xA);
            *(s16 *)(p + 0x88) = *(u16 *)(d + 0x88);
        }
        func_8004491C(n, (u32)D_80045340);
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_06 = 2;
        r->unk_08 = func_800A7A38(e);
        *(s32 *)(p + 0x8C) = *(s32 *)(d - 0x18);
        *(s32 *)(p + 0x90) = *(s32 *)(d - 0x14);
        *(s32 *)(p + 0x98) = *(s32 *)e;
        *(u8 **)(p + 0x9C) = p + 0x98;
        *(p + 0x13) = 0xFF;
        func_80099FDC(n);
        func_800BC26C(n, 0, 0, 0);
    }
    return n;
}

/* Set bit (i & 7) of a bitmap at 0x80015720 for every live entry of every
   D_80073414 descriptor whose +0x01 has the caller's bit.

   0x80015720 IS A CONSTANT ADDRESS, NOT A SYMBOL: a declared array (or a
   sized, scalar or 2-D alias of it) materialises the whole `lui`/`addiu` as a
   loop invariant and the `%lo` leaves the `lbu`/`sb` -- 50 words against 49.
   THE CONSTANT BASE MUST BE A POINTER LOCAL RE-ASSIGNED AT THE TOP OF THE
   OUTER LOOP: at function scope loop.c turns `b + j * 8` into a giv, 21
   differing; inside the inner loop 17; inside the `if` 30.

   Both `addu`s are INDEX-FIRST, so both operands must reach them as integers:
   `i * 0x14 + e->unk_0C` is pointer arithmetic and comes out base-first, 1
   differing, and so does `(j * 8 + i / 8) + b`. Note the scaled operand is
   the base's PARTNER, not the base, so the "unscaled index only" caveat on
   that lever does not hold here. 49 words, 0 differing. */
void func_800A9024(s32 bit)
{
    s32 i;
    s32 j;
    Unk80073414 *e;
    u8 *q;
    u8 *b;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        e = &D_80073414[j];
        if (((*((u8 *)e + 1) >> bit) & 1) == 0)
        {
            continue;
        }
        for (i = 0; i < e->unk_02; i++)
        {
            q = (u8 *)(i * 0x14 + (s32)e->unk_0C);
            if ((*(u16 *)q & 0x200) == 0)
            {
                *(u16 *)q |= 0x400;
                b[(j * 8 + i / 8) + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* Clear bit 0x400 of the halfword at the head of entry p[0] of the
   0x14-stride table D_80073414[p[1]].unk_0C points at. 30 words,
   0 differing. */
void func_800A90E8(u8 *p)
{
    u8 *q;

    func_8004A8D8(p[1], p[0]);
    q = D_80073414[p[1]].unk_0C;
    *(u16 *)(q + p[0] * 0x14) &= 0xFBFF;
}

/* Rebuild the 0x13 x 8-byte occupancy bitmap in low RAM.

   THE BITMAP IS A CONSTANT BASE, NOT A SYMBOL: as `extern u8 D_80015720[]`
   the whole address is materialised and hoisted, the stores lose their
   displacement and the function is 53 words. A 2-D `[][8]` declaration
   reaches 52 but keeps the row base in a register, 38 differing. `b` IS
   ASSIGNED INSIDE THE OUTER LOOP so loop.c hoists it after `j = 0`; at
   function scope the `lui` is emitted first, 2 differing. AND THERE IS NO
   OFFSET VARIABLE: `o += 0x14` as a source local puts `o = 0` ahead of the
   loop guard, 9 differing. 52 words, 0 differing. */
void func_800A9160(void)
{
    u8 *b;
    s32 j;
    s32 i;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            if ((i & 7) == 0)
            {
                b[(j << 3) + i / 8 + 0x5720] = 0;
            }
            if (*(u16 *)(D_80073414[j].unk_0C + i * 0x14) & 0x400)
            {
                b[(j << 3) + i / 8 + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* The one-shot 0x20-kind prompt, and then a random D_800E3548 slot claim.
   THE RESULT IS A FOUR-BYTE STACK OBJECT, not a register value: it is
   zeroed as a word, filled two bytes at a time in one arm and replaced
   whole in the other, and taking its address is what keeps every access in
   memory.
   THE THREE SLOT TESTS MUST BE SEPARATE `if`s. Written
   `k != 0 && k < 0xB && k == 1` the front end's range folding collapses the
   first two into the third and the function is 84 words / 25 differing --
   exactly the `beqz` / `sltiu 0xB` / `beqz` triple and its load-delay nop
   missing. Nested, they survive.
   AND D_80012090 IS THE CONSTANT ADDRESS, as it is everywhere else in this
   unit: as the declared symbol the pair is `lui $v0` / `lw $v1, %lo($v0)`
   against the target's `lui $v1` / `lw $v1, %lo($v1)` -- 2 differing, and
   NOT hidden by the relocation mask. 88 words, 0 differing;
   `seqcheck` reports the two constant-address words with raw == relocdump
   at 20 == 20. */
s32 func_800A9230(u8 *p)
{
    UnkDungeonSlot4 s;
    s32 i;

    *(s32 *)&s = 0;
    if (p[0x13] == 0x20 && D_800E3CCC == 0)
    {
        D_800E3CCC = 1;
        if (*(s32 *)0x80012090 == 0)
        {
            if (D_8008146C == 0x19 && func_80033BC0(0x11F9) != 0 &&
                func_80033BC0(0x11FC) == 0 && (s16)func_800A697C(0xD, 2) == 0)
            {
                s.unk_00 = 2;
                s.unk_01 = 0xD;
                *(s16 *)(*(u8 **)(p - 0x14) + 0x12) = 0xC;
            }
        }
    }
    if (D_800DCF5A != 0 && p[0x49] == 0 && (func_800A6D30() & 3) == 0)
    {
        i = (u16)func_800A6DA4(1, 0x1F);
        if (D_800E3548_t[i].unk_01 != 0)
        {
            if (D_800E3548_t[i].unk_01 < 0xB)
            {
                if (D_800E3548_t[i].unk_01 == 1 && D_800E3548_t[i].unk_00 == 8)
                {
                    *(s32 *)&s = *(s32 *)&D_800E3548_t[i];
                    *(s32 *)&D_800E3548_t[i] = 0;
                }
            }
        }
    }
    return *(s32 *)&s;
}

/* THE LOCAL IS `s16`. The target copies the `lbu` result into a second
   register before the three compares; `u8 v` makes that copy an
   `andi $v1, $a0, 0xFF` (1 differing) and `s32 v` deletes it altogether,
   loading straight into the compare register (27 of 28 words, 19 differing).
   Only the widening copy of a NARROW local is a bare `addu`. This is the
   function the c6 batch left at 1 differing after twenty-one declarations;
   `s16` was not among them. 28 words, 0 differing. */
s16 func_800A9390(s16 idx)
{
    s16 v;

    v = D_800DD8E7[idx];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (v == 0x32)
        {
            return 0x33;
        }
        if (v == 0x39)
        {
            return 0x3A;
        }
        if (v == 0x40)
        {
            return 0x42;
        }
    }
    return v;
}

/* `s32 c`, NOT `u8 c`: an lbu already zero-extends, but a QImode local makes
   each compare carry its own `andi 0xFF` and the function is 41 words.
   40 words, 0 differing. */
s16 func_800A9400(s16 x)
{
    s32 c;

    c = D_800DD8E7[x];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (c == 0x32)
        {
            return 7;
        }
        if (c == 0x39)
        {
            return 8;
        }
        if (c == 0x40)
        {
            return 9;
        }
    }
    else
    {
        if (c == 0x32)
        {
            return 1;
        }
        if (c == 0x39)
        {
            return 2;
        }
        if (c == 0x40)
        {
            return 3;
        }
    }
    return x;
}

/* 227/227 words, 0 differing.  Came down from 76 aligned differences over two
   days of parking; the five levers below are what closed it, and every device
   in the body is load-bearing -- a leave-one-out sweep costs 8, 16, 21, 30 and
   45 rows respectively.

   1. `y' AND `z' ARE TWO SEPARATE SINGLE-SET LOCALS, not one variable and not
      the parameter `q'.  The target's `addu $a0, $s4, $zero' comes BEFORE
      `addu $s3, $v0, $zero' at words 169/170, and that order is unreachable
      while the call result lands in a variable with more than one set:
      expand_call pins the return copy's LUID immediately after the call, and
      sched.c's rank_for_schedule falls through to INSN_LUID, so the copy comes
      out first.  What flips it is `adjust_priority': the return copy becomes a
      BIRTHING insn (LAUNCH_PRIORITY 7f000001 in the ready list) only when its
      destination pseudo has reg_n_sets == 1, and then it wins the tie and is
      emitted second.  `z' therefore has to be assigned exactly once -- the
      `if (q) { q = func_800990FC(); } else { ... }' device this park carried
      for two days gave it two sets and made those two words unreachable.
      Same for `y': func_80069EF8's result needs its own single-set local, and
      all three of q, y and z end up in $s3 because their ranges are disjoint.

   2. `if (y) { func_800C77D0(...); } else { func_800C77D0(...); }' -- a
      duplicated-arm device whose only purpose is a free reference on `y'.
      Without it the dead `addu $s3, $v0, $zero' at word 70 (func_80069EF8's
      result, never read on any path) is deleted and the whole file rotates:
      45 rows.  The condition must be `y' and the device must sit inside the
      `(s16)c == 0' arm, where `y' is defined.

   3. `if (cc) { cc = c; } else { cc = c; }' READS `cc' BEFORE IT IS SET, and
      that is the one device here that is not ordinary C.  It is worth 21 rows
      and nothing defined reproduces it: every other condition (c, a, q, n, s,
      e, y, z, t, q0, q1, k, cc2, h, m, p, g, src, o), initialising `cc' to 0
      or to `c' first, moving the device onto `cc2', and a cancelled
      `cc++; cc--;' in its place ALL measure 21.  Both arms store the same
      value and `cc' is never read again, so nothing the function computes
      depends on the branch, and gcc 2.7.2 does not exploit the UB.
      `cc2 = c;' must stay AHEAD of it -- after it costs 10 rows.

   4. TWO CANCELLED PAIRS, `n++; n--;' (30 rows) and `q0++; q0--;' (16 rows).
      Each compiles to nothing -- combine folds the two addiu -- but flow
      counts four references per pair, and global.c's allocno_compare orders
      the queue by floor_log2(n_refs) * n_refs / live_length * 10000, with the
      numbers printed by `dump_flow_info' at the top of the `.lreg' dump.  The
      `q0' pair takes q0 from 10 refs/148 insns to 12/152, i.e. 2027 -> 2368,
      which lifts it past `y' (2 refs/9 insns = 2222) so q0 takes $s2 and
      y/q/z take $s3.  `q0 += 1; q0 -= 1;' and `n += 1; n -= 1;' are the same
      bytes; about 30 statement positions for the q0 pair all land.

   5. EACH ARM OF THE LAST `if' GETS ITS OWN BASE AND RESULT POINTER, and the
      base is materialised BEFORE the index: `b1 = (u8 *)D_8006DE24_n;
      n1 = b1 + q0 * 0x14;'.  The target materialises %hi/%lo(D_8006DE24)
      first in each arm; `(u8 *)&D_8006DE24_n[q0]' makes expand emit the index
      chain first and the base last, and nothing moves it back.  One shared
      base is a global quantity by flow.c:2561 (two sets, two blocks) and
      global_alloc gives it a register of its own after local_alloc has spent
      $v1/$v0 on the block-local HIGH temps; per-arm locals keep it block-local
      so the HIGH and the LO_SUM share $a0 as the target has them.  Worth 15
      rows on the park's 26-row body in two halves (base-before-index 7,
      per-arm locals 8); writing both arms back as `(u8 *)&D_8006DE24_n[q0]'
      on THIS body is 16 rows.

   And two spellings that must not be touched, both already in the park:
   `q0 = func_800A9400(q0);' self-assigns so q0 keeps a second set and combine
   cannot fold the three sll/sra pairs away (`k = func_800A9400(q0);' is 16
   rows here and was 222 words on the park's earlier body), and the two
   D_800DCEAC/D_800DCEBC halves are `>> 1', not `/ 2' -- `/ 2' is 233 words. */
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *p;
    u8 *s;
    u8 *g;
    u8 *src;
    u8 *o;
    void *h;
    s16 q0;
    s32 cc;
    s32 cc2;
    u8 q1;
    s16 k;
    s32 t;
    u8 *y;
    u8 *z;

    s = *(u8 **)(a - 0x14);
    n = func_8003FD64(0x12, D_80083498);
    n++;
    n--;
    if (n != NULL)
    {
        cc2 = c;
        if (cc)
        {
            cc = c;
        }
        else
        {
            cc = c;
        }
        q0 = q[0];
        D_800E3D68 = q[1];
        t = func_800A982C(q0, (s16)c, a[0x13]);
        q0++;
        q0--;
        if (t == 0)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return 0;
        }
        n->unk_10 = t;
        p = (u8 *)n + 0x20;
        *(u8 **)p = a;
        *(u8 **)(p + 0x4) = e;
        p[0x8] = q0;
        D_800E3CC8[0] = q0;
        q1 = q[1];
        p[0x9] = q1;
        D_800E3CC8[1] = q1;
        if ((s16)c == 0)
        {
            m = n->unk_08;
            src = *(u8 **)(a - 0x18);
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(src + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(src + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(src + 0xA);
            y = (u8 *)func_80069EF8();
            if ((D_80013714 & 1) == 0)
            {
                o = *(u8 **)(a + 0x60);
                if (o != NULL)
                {
                    func_8009FD40(*(u8 **)(a - 0x14), *(u8 **)(o - 0x14));
                }
            }
            if (y)
            {
                func_800C77D0(n, (u8 *)m, 8, 0x300);
            }
            else
            {
                func_800C77D0(n, (u8 *)m, 8, 0x300);
            }
        }
        else
        {
            g = D_80082E80;
            if ((*(u16 *)(s + 0x14) & 0x8000) == 0 && (*(u16 *)(g + 0x14) & 0x8000) == 0)
            {
                func_800C7A3C(g, s, *(s16 *)(D_800E3D7C + 0x88), *(s16 *)(a + 0x88), 8, 0x300);
            }
            else
            {
                k = (*(u16 *)(D_800E3D7C + 0x2A) >> 9) & 7;
                func_800C78A0(a - 0x20, (g[0x24] << 6) + (*(s16 *)&D_800DCEAC[k] >> 1) + 0x20,
                              (g[0x25] << 6) + (*(s16 *)&D_800DCEBC[k] >> 1) + 0x20,
                              *(s16 *)(D_800E3D7C + 0x88), 8, 0x300);
            }
        }
        if ((s16)cc2 != 0)
        {
            func_800AD594(a, D_800DD8B4[q0] << 6);
        }
        else
        {
            func_800AD594(a, D_800DD880[q0] << 8);
        }
        if (n != NULL && *(s8 *)(a + 0x13) > 0)
        {
            z = (u8 *)func_800990FC();
            h = func_80099734(a, z);
            h = func_80099194(D_800E1C58, h);
            if ((s16)cc2 != 0)
            {
                u8 *b1;
                u8 *n1;

                q0 = func_800A9400(q0);
                b1 = (u8 *)D_8006DE24_n;
                n1 = b1 + q0 * 0x14;
                h = func_80099194(*(u8 **)(n1 + 8), h);
            }
            else
            {
                u8 *b2;
                u8 *n2;

                b2 = (u8 *)D_8006DE24_n;
                n2 = b2 + q0 * 0x14;
                h = func_80099194(*(u8 **)n2, h);
            }
            func_80099290(func_80099194(D_80089080, h));
            func_800A5720(z);
        }
    }
    return (s32)n;
}

/* Re-upload the dungeon HUD's 0xC x 0x18 tile when the (kind, variant) pair
   at D_800DCF4D changes, then hand func_8003F540 the pair of words the
   variant's table holds, and leave D_800DCF59 as a two-frame handshake.
   118 words, 0 differing.

   THE "CHANGED" FLAG MUST BE NARROW. As `s32 f` the function is 116 words:
   both `if (f != 0)` tests become a bare `beqz $s3` where the target copies
   the flag into $v0 first (`addu $v0, $s3, $zero` at words 26/28 and 97).
   With `s16 f` the read of the flag is a mode conversion whose extension
   combine drops -- nonzero_bits is 1 -- leaving a plain register copy cse
   cannot propagate away, which is those two words.
   docs/matching-levers.md :16019. `s8` measures identically; `u8` and `u16`
   are 118 words at 3 differing.

   0x80024000 IS A CONSTANT ADDRESS, not a symbol: words 2/5 are a
   `lui`/`ori` pair, which is the assembler's `li` macro and not a
   `%hi`/`%lo` relocation. It is held in a local because it is used twice --
   as func_8003F6D4's second argument and as the function's return value.

   ARITY: no landed caller. The stub func_800A94A0 sets $a0 (delay slot),
   $a1 and $a2 for this `jal` -- `lbu 0x13($s4)` two insns ahead -- so the
   third parameter is real but unread here, and it is left `s32`. */
s32 func_800A982C(s16 a, s16 b, s32 c)
{
    s32 *q;
    u8 *tbl;
    s16 f;
    s32 t;
    s16 k;

    q = (s32 *)0x80024000;
    if (D_800DCF59[0] == 0)
    {
        f = 0;
        if (a != (D_800DCF4D & 0x3F) || ((D_800DCF4D >> 6) & 1) != b)
        {
            f = 1;
        }
        if (f != 0)
        {
            if (b != 0)
            {
                t = func_800A9390(a) * 12 + 0x607F;
            }
            else
            {
                t = (a - 1) * 12 + 0x607F;
            }
            func_8003F6D4(0xC, q, D_800E58B0, t);
            func_8003E4FC(6, D_800E58B0, 0);
            func_8003E4FC(0xFF, D_8003E140, (s32)D_800DCF59);
            D_800DCF4D = a | (b << 6);
        }
        if (b != 0)
        {
            tbl = D_800DDAB8;
            k = func_800A9400(a) * 2;
        }
        else
        {
            tbl = D_800DD918;
            k = a * 2;
        }
        func_8003F540(0, D_8006CD58, *(u32 *)(tbl + k * 4), *(u32 *)(tbl + k * 4 + 4));
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        if (f != 0)
        {
            D_800DCF59[0] = 2;
            return 0;
        }
    }
    else
    {
        if (D_800DCF59[0] != 1)
        {
            return 0;
        }
        D_800DCF59[0] = 0;
    }
    return *q;
}

void func_800A9A04(u8 *p)
{
}

/* The per-frame tick for an actor's 0x6D countdown. TWO SPELLINGS COST TWO
   WORDS EACH. 0x80013714 has to be the CONSTANT ADDRESS: as structs.h's
   symbol the `high` is a separate insn, and because the use is inside the
   loop, loop.c hoists it into a callee-saved register -- 62 words against 60,
   the two being $s2's save and restore. And the 0x2F bound is UNSIGNED: an
   s32 local gives `slti` where the target has `sltiu`. 60 words,
   0 differing. */
void func_800A9A0C(u8 *p)
{
    u32 v;

    p[0x71] &= 0x7F;
    if ((D_80083462 & 0x2800) != 0)
    {
        return;
    }
    if (*(s8 *)(p + 0x6D) != 0)
    {
        do
        {
            if ((*(u16 *)0x80013714 & 0x8) == 0)
            {
                if ((*(u32 *)(p + 0x1C) & 0x8) == 0)
                {
                    v = p[0x13];
                    if (v < 0x2F || v == 0x39)
                    {
                        func_800AD594(p, 4);
                    }
                }
                func_800A4ACC(p);
            }
            *(s8 *)(p + 0x6D) -= 1;
        } while (*(s8 *)(p + 0x6D) != 0);
    }
    *(u16 *)(p + 0x46) &= 0x7FFF;
    func_800AA53C(p);
}

/* Push the actor's grid square onto the record's +0x73/+0x7B queue.

   THREE THINGS THE LISTING FORCES. `(s16)func_80042900(...)` assigned to an
   `s32` puts the `sll`/`sra` pair at the ASSIGNMENT, which is what makes the
   two `bnez` test the sign-extended $s4; an s16 local narrows at the use
   instead (70 words / 26) and dropping the cast is 68 / 54. The whole FLAG
   WORD is hoisted into a local before the two byte arguments -- hoisting the
   already-MASKED boolean instead measures identically to not hoisting at all
   (4 differing), because what decides sched1's order is which load's
   statement comes first, not where the mask is. And the queue stores are
   written base, index, constant last: the subscript form folds the constant
   in first and leaves the index as the `addu`'s left operand, 2 differing.
   69 words, 0 differing. */
void func_800A9AFC(u8 *a, u8 *b)
{
    s32 t;
    s32 u;
    s32 x;
    s32 y;
    s32 x2;
    s32 y2;
    s32 f;

    if ((b[0x71] & 0x7F) != 0)
    {
        t = (s16)func_80042900((s8 *)b, 0x1B);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A3D0(x2, y2, u);
        }
        x = b[0x74];
        y = b[0x7C];
        func_80069F28(b + 0x74, b + 0x75, 0xF);
        *(b + b[0x71] + 0x73) = a[0x24];
        *(b + b[0x71] + 0x7B) = a[0x25];
        a[0x24] = x;
        a[0x25] = y;
        *(s16 *)(b + 0x88) = *(u16 *)(b + 0x8A);
        *(s16 *)(b + 0x2A) = *(u16 *)(b + 0x6A);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A21C(x2, y2, u);
        }
        *(s16 *)(b + 0x8A) = 0;
    }
}

void func_800A9C10(void)
{
}

/* The arrival tail for a kind-(d & 3) == 2 entity: pick the cast or
   D_80081488 for the renderable's +0x28, place the mover, measure the leash
   into +0x88, dress the slot, face it off D_80083160's +0xC8 stamp, light
   the node and hang the 0x110 marker, then set or clear the slot's bit 0
   from D_8006CCF8. 136 words, 0 differing on the first compile.

   func_800B22F0 (csrc/ovl_dungeon.c:12451) is the line-for-line template for
   everything from the func_8009FB34 call to func_800BC26C; only the head,
   the D_80083160 facing lookup and the D_8006CCF8 tail are new.

   THE HEAD'S `(d & ~3)` IS A SEPARATE s16 LOCAL: the `sll 16`/`bnez` pair is
   the HImode comparison, where the `(d & 3) == 2` test next to it needs no
   extension because `andi 3` has already proved the range. */
void func_800A9C18(UnkNode *n, UnkVec *m, UnkSlot *r, s16 d)
{
    u8 *p;
    u8 *g;
    s16 id;

    p = (u8 *)n + 0x20;
    id = d & ~3;
    if ((d & 3) == 2)
    {
        if (id == 0)
        {
            r->unk_28 = func_80047D44(p[0x13]);
        }
        else
        {
            r->unk_28 = D_80081488;
        }
        D_800DCF4D = -1;
    }
    else
    {
        if (p[0x13] != 0x38)
        {
            func_80048088(p[0x13]);
        }
        func_8004827C(n, p[0x13]);
    }
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s16 *)(p + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                       *(u16 *)((u8 *)m + 0xA) - 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    r->unk_14 |= 0x8000;
    g = (u8 *)&D_80083160;
    func_80047784((u8 *)r,
                  (*(u8 **)((u8 *)r + 0x2C))[((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7],
                  0);
    func_80099FDC(n);
    if ((*(u32 *)(p + 0x14) & 0x4000) == 0)
    {
        func_800A34BC(p);
    }
    func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                  (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0x71] = 0;
    *(u32 *)(p + 0x1C) |= 0x40000000;
    func_800BC26C(n, 0, (s32)((u8 *)r + 0x2C), (s32)(p + 0x2A));
    if (D_8006CCF8[((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7] != 0)
    {
        r->unk_14 |= 1;
    }
    else
    {
        r->unk_14 &= 0xFFFE;
    }
}

/* A COMMON RETURN VARIABLE, not two returns. With `return 1;` as its own
   statement reorg lifts the `li 1` into the `beqz` delay slot and the
   function is 12 words; one local keeps the `nop` and the `j`.
   14 words, 0 differing. */
s32 func_800A9E38(u8 *p)
{
    u8 *r;
    s32 v;

    r = *(u8 **)(p - 0x14);
    if (*(s32 *)r != 0)
    {
        v = (u32)(p[0x13] - 0x2E) < 2;
    }
    else
    {
        v = 1;
    }

    return v;
}

/* The actor's per-frame gate for the +0x100000 and +0x400/+0x8000 modes: on
   the record's 0x100000 flag it re-points the slot's sprite for the current
   eight-way facing through the +0x2C table, sets or clears the slot's bit 0
   from D_8006CCF8 and reports 1; otherwise it ages the +0x46 mode word, keeps
   the +0x98 0x400 bit in step with the slot's 0x8000 flag, and then runs
   func_800B253C's claim-and-dispatch tail verbatim, always reporting 0.

   THE +0x46 GATE IS TWO NESTED `if`s, NOT AN `&&`. Written
   `(w & 0x8000) != 0 && (w & 0x4000) == 0` fold_truthop merges the pair into
   one `andi 0xC000` / `bne 0x8000` -- 247 words, 163 differing. This is the
   existing fold_truthop lever read in the direction that COSTS words.

   THE SLOT'S 0x8000 TEST IS SPELT `== 0`, with the `|= 0x400` in the `else`.
   Written `!= 0` with the redraw in the `else if` the two arms come out in
   the other order: 249 words, 162 differing from that word on.

   THE +0x9A VALUE MUST REUSE THE MASK'S OWN PSEUDO -- func_800B253C's lever
   again, but here it buys a REGISTER and not the hoist. The target KEEPS
   jump.c's store-flag hoist (`addiu $v0, $zero, 1` in the `bnez`'s delay
   slot), so both

       if ((t & 0x4000) == 0) { v = 2; } else { v = 1; }
       v = t & 0x4000; if (v == 0) { v = 2; } else { v = 1; }

   emit the same 250 words here. Only the second shares $v0 between the
   `andi`, the branch and the two `li`s; the first gives `v` $a0 and is
   3 differing on exactly the two `li`s and the `sb`.

   250 words, 0 differing. */
s32 func_800A9E70(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *cur;
    u32 f;
    u32 t;
    s32 k;
    s32 m;
    s32 v;

    if ((*(u32 *)(d + 0x14) & 0x100000) != 0)
    {
        k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (*(s16 *)((u8 *)p + 0x94) != k)
        {
            if (func_800A9E38(d) != 0)
            {
                func_8003DB94(c, *(void **)(*(u8 **)(c + 0x2C) + k * 4), *(s8 *)(c + 0x4));
            }
            else
            {
                func_80047738(c, (*(u8 **)(c + 0x2C))[k], *(s8 *)(c + 0x4));
            }
            *(s16 *)((u8 *)p + 0x94) = k;
        }
        if (D_8006CCF8[k] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        func_800478B8(c);
        return 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x400) != 0)
    {
        t = *(u16 *)(d + 0x46);
        if ((t & 0x8000) != 0)
        {
            if ((t & 0x4000) == 0)
            {
                m = t & 0x3FFF;
                if ((u32)(m - 0x5) < 3 || m == 0xC)
                {
                    *(u16 *)(d + 0x46) = t & 0x7FFF;
                }
            }
        }
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        if ((*(u16 *)((u8 *)p + 0x98) & 0x400) != 0)
        {
            k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            if (func_800A9E38(d) != 0)
            {
                func_8003DB94(c, *(void **)(*(u8 **)(c + 0x2C) + k * 4), *(s8 *)(c + 0x4));
            }
            else
            {
                func_80047738(c, (*(u8 **)(c + 0x2C))[k], *(s8 *)(c + 0x4));
            }
            *(u16 *)((u8 *)p + 0x98) &= 0xFBFF;
        }
    }
    else
    {
        *(u16 *)((u8 *)p + 0x98) |= 0x400;
    }
    if ((*(u32 *)(d + 0x1C) & 0x2000) == 0 && (*(u32 *)(d + 0x14) & 0x4000) == 0)
    {
        return 0;
    }
    if (p->unk_9A == 0x13 || p->unk_9A == 0x14)
    {
        return 0;
    }
    if (*(s8 *)(d + 0x6D) != 0)
    {
        return 0;
    }
    if (D_800DCE68 == 0)
    {
        if ((s16)func_800A2BDC(d) != 0)
        {
            return 0;
        }
    }
    else
    {
        g = (u8 *)&D_80083460;
        cur = *(u8 **)(g + 0xC);
        if (cur != d)
        {
            if (*(u32 *)(g + 0x10) != 0)
            {
                return 0;
            }
            if (cur != NULL)
            {
                return 0;
            }
            if (*(s16 *)(g + 0x8) != 0)
            {
                return 0;
            }
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return 0;
            }
            if (d[0x43] != 0xFD)
            {
                return 0;
            }
            *(u8 **)(g + 0xC) = d;
        }
        if (d[0x43] != 0xFD)
        {
            return 0;
        }
    }
    f = *(u32 *)(d + 0x1C);
    if ((f & 0x400000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x400000;
        t = *(u32 *)(d + 0x14);
        if ((t & 0x20000000) != 0)
        {
            *(u32 *)(d + 0x14) = t | 0x400000;
            func_800ACB98((u8 *)p, b, c, d);
            return 0;
        }
        *(u32 *)(d + 0x14) = t & ~0x400000;
        v = t & 0x4000;
        if (v == 0)
        {
            v = 2;
        }
        else
        {
            v = 1;
        }
        p->unk_9A = v;
    }
    else if ((f & 0x2000000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x2000000;
        d[0x71] = 0;
        if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
        {
            func_800ACD74((u8 *)p, b, c, d);
            return 0;
        }
        p->unk_9A = 0;
    }
    else
    {
        return 0;
    }
    p->unk_9B = 0;
    return 0;
}

/* State 0xA, func_800AA888's installer one state over. THE MODE IS A `u16`
   LOCAL, not the s32 func_800AA888 uses: it is live across both reports, so
   it needs a callee-saved register, and the `andi $s0, 0xFFFF` at the join is
   what the narrow type buys. The sine/cosine tables are read a BYTE at a time
   at a doubled index, which is what gives `lbu` rather than the `lh` an s16
   subscript would. 69 words, 0 differing. */
void func_800AA258(u8 *p, void *b, u8 *c, u8 *d)
{
    u16 t;
    s32 i;
    u8 *g;

    p[0x9A] = 0xA;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(c[0x24], c[0x25], t);
    i = (*(u16 *)(d + 0x6A) >> 9) & 7;
    c[0x24] += ((u8 *)D_8006CCD8)[i * 2];
    c[0x25] += ((u8 *)D_8006CCE8)[i * 2];
    func_8009A21C(c[0x24], c[0x25], t);
    *(s32 *)(d + 0x1C) &= ~0x100;
    *(s16 *)(p + 0x96) = 0x21;
    *(u16 *)(p + 0x98) |= 0x8;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* The seven-gate "can this actor act" entry: the engine must be idle
   (func_800A2BDC), the node must not be dying, neither the view's +0x1C
   0x80000 nor its +0x14 0x100000 request bit may be up, the player record
   must not be in state 0x18 or 0x11, func_800A41F0 must agree, and
   func_8009B7E4 must return a non-zero mode -- then the +0x40000000 request
   goes up, the move is reported twice and mode 1 or anything else picks
   func_800AA508 or func_800AA5E4.

   `x` AND `y` MUST BE `s16`. They hold c+0x24/c+0x25 across the
   func_8009B7E4 call, and the width decides nothing about the copies
   themselves -- `addu $a0, $s2, $zero` either way -- but it does decide
   sched1's priority for them: as `s32` both copies sink below the mode
   ternary's branch, the second one lands in the first `jal`'s delay slot
   where the target has a `nop`, and the function is 102 words.
   `u8` and `u16` are 103 words and 2 differing.

   THE GATES ARE NESTED, with ONE trailing `return 0`. Written as seven
   `if (...) return 0;` early returns the shared zero block is cross-jumped
   to a spot early in the function instead of sitting between the two
   `return 1` tails and the epilogue: 103 words, 71 differing (102 and 75
   before the `s16` fix).

   Both reports take the 0x300/0x3000 choice as an INLINE TERNARY. Lifted
   into an `s32 t` local, each one's argument setup moves below the test:
   one local for both is 103 words / 20 differing, one for the first call
   only is 102 / 57.

   TWO LANDED CALLEES HAD TO BE WIDENED FOR THIS CALL SITE -- see the report:
   func_800A2BDC was `(void)` and takes one unread pointer (all five callers
   set $a0), func_800AA508 was `(u8 *)` and takes the handler four. Both
   still measure 0 differing at 22 and 13 words.

   103 words, 0 differing. */
s32 func_800AA36C(u8 *p, void *b, u8 *c, u8 *d)
{
    s16 k;
    s16 x;
    s16 y;

    if ((s16)func_800A2BDC(d) == 0)
    {
        if ((*(u16 *)(p - 0x2) & 0x8000) == 0)
        {
            if ((*(u32 *)(d + 0x1C) & 0x80000) == 0)
            {
                if ((*(u32 *)(d + 0x14) & 0x100000) == 0)
                {
                    if (*(u8 *)(D_800814A8 + 0x9A) != 0x18 && *(u8 *)(D_800814A8 + 0x9A) != 0x11)
                    {
                        if ((s16)func_800A41F0(d) != 0)
                        {
                            x = c[0x24];
                            y = c[0x25];
                            k = func_8009B7E4(c, d);
                            if (k != 0)
                            {
                                *(u32 *)(d + 0x1C) |= 0x40000000;
                                func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                                func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                                if (k == 1)
                                {
                                    func_800AA508(p, b, c, d);
                                }
                                else
                                {
                                    func_800AA5E4(p, b, c, d);
                                }
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Installer: state 3, handler slot cleared, a 4-step life, and the
   D_80083460 counter at +0xA bumped. 13 words, 0 differing. */
/* FOUR PARAMETERS, as its twin func_800AA5E4 has: func_800AA36C calls both
   the same way. 13 words, 0 differing either way. */
void func_800AA508(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 3;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 4;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* Re-aim one record's facing and re-run its walk.

   D_80082E80'S SUBSCRIPTS ARE WRITTEN AT THE SITE. Hoisted into the same
   `u8 *r` the else arm uses, the symbol's `high` gets its own register
   (`lui $v0` / `addiu $s0, $v0`) where the target ties both to $s0: 2
   differing. `&D_80082E80[0]` is the same 2; a block-scoped `r` per arm
   measures 0, like the subscript. 42 words, 0 differing. */
void func_800AA53C(u8 *a)
{
    u8 *r;

    if (a == NULL || a[0x13] == 0)
    {
        D_80082E80[0x26] = func_8009FB34(D_80082E80[0x24], D_80082E80[0x25]);
        func_800A1B44(3, 6);
    }
    else
    {
        r = *(u8 **)(a - 0x14);
        r[0x26] = func_8009FB34(r[0x24], r[0x25]);
        func_800A19E4((s32)r, a, 3, 6, a + 0x9C);
    }
}

/* State 4. The `sll 16` / `bltz` with no `sra` is `(s16)result < 0`.
   52 words, 0 differing. */
void func_800AA5E4(u8 *p, void *b, u8 *r, u8 *d)
{
    u8 *g;

    p[0x9A] = 4;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 0x10;
    *(u16 *)(p + 0x98) |= 8;
    *(u32 *)(d + 0x1C) |= 0x10000;
    func_8009A3D0(r[0x24], r[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    do
    {
    } while ((s16)func_800A4E2C(r + 0x24, r + 0x25) < 0);
    r[0x26] = func_8009FB34(r[0x24], r[0x25]);
    func_800AA53C(d);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* THE NULL TEST NESTS, IT DOES NOT RETURN. `if (t == NULL) return 1;` as its
   own early exit materialises a third `addiu $v0, 1` and the function is 41
   words; nesting the rest inside `if (t != NULL)` lets the NULL path fall
   into the tail's `return 1` while the 0x20 path keeps its own, which is the
   two the target has. The prototype gains a return type and a pointer fourth
   parameter; the four landed callers pass a literal 0 and discard the
   result, so neither changes a byte. 40 words, 0 differing. */
s32 func_800AA6B4(u8 *p, UnkVec *b, u8 *c, u8 *t)
{
    s32 v;

    if (*(s16 *)(p + 0x64) > 0)
    {
        func_800AA754(p, b, c, p);
        return 0;
    }
    D_80083470 = p - 0x20;
    p[0x9A] = 0x7;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        if (*(s32 *)(p + 0x1C) & 0x20)
        {
            return 1;
        }
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    return 1;
}

/* 18 words, 0 differing. */
void func_800AA754(u8 *a, UnkVec *b, u8 *c, u8 *p)
{
    func_800AD4D0(p);
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

/* The actor's "give up and go back to state 5" transition. The call to
   func_800ACB98 forwards all four of this function's own parameters, which is
   why the `jal` has a bare `nop` delay slot and no argument setup.
   59 words, 0 differing. */
s32 func_800AA79C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        p->unk_9A = 0xE;
        return 0;
    }
    if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
    {
        func_800ACB98((u8 *)p, b, c, d);
        return 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x8) == 0)
    {
        *(u16 *)(g + 0xA) -= 1;
    }
    p->unk_9A = 5;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(u16 *)((u8 *)p + 0x98) &= ~0x8;
    *(s16 *)(d + 0x24) = 0;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) = (*(u32 *)(d + 0x1C) | 0x8) & ~0x40000;
    *(u16 *)(c + 0x14) |= 0x6000;
    return 1;
}

/* THE TWO BYTE ARGUMENTS ARE READ BEFORE THE `if`, NOT AT THE CALL. Written
   as `func_8009A3D0(c[0x24], c[0x25], t)` the loads land after the branch,
   $a2 has to be copied to a callee-live register to survive it, and the
   function is 41 words. Hoisted into two locals they are issued from $a2 in
   the prologue and nothing is saved: 39 words, 0 differing. */
void func_800AA888(u8 *p, void *b, u8 *c, u8 *d)
{
    s32 t;
    s32 x;
    s32 y;

    p[0x9A] = 0xB;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    x = c[0x24];
    y = c[0x25];
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(x, y, t);
    *(u16 *)(p + 0x98) |= 0x8;
    *(s32 *)(d + 0x1C) &= ~0x40000;
    *(s16 *)(p + 0x96) = 0;
    *(u16 *)(d + 0x6A) = *(u16 *)(*(u8 **)(d + 0x60) + 0x2A);
}

/* Installer: two refusals into state 0xE, else state 0xD with the facing
   pushed through func_80047784. 65 words, 0 differing. */
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t)
{
    u8 *g;
    s32 v;

    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if (*(u16 *)(g + 0x2) & 0x2008)
    {
        p[0x9A] = 0xE;
        return 1;
    }
    if (*(s32 *)(p + 0x1C) & 0x20)
    {
        p[0x9A] = 0xE;
        *(s32 *)(p + 0x1C) &= ~0x200;
        return 0;
    }
    p[0x9A] = 0xD;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    *(u16 *)(g + 0xA) += 1;
    *(s32 *)(p + 0x1C) &= ~0x40000;
    return 1;
}

/* func_80095DD0 on the SECOND argument and without the parked pointer.
   11 words, 0 differing. */
/* TWO parameters, not four. func_800AD9B4 is this function's only reference
   anywhere in the overlay -- no other `jal`, no address-take -- and it passes
   two. The four-parameter signature was the handler convention applied to a
   function nothing dispatches; narrowed, this still measures 11 words,
   0 differing. */
void func_800AAA28(void *a, u8 *p)
{
    u8 *g;

    p[0x9A] = 0x15;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x14) += 1;
}

/* The shared "actor dies" tail the four +0x8C state machines call with a NULL
   fourth argument. The fourth parameter keeps the `s32` the unit already
   declares even though the body indexes it: all eight landed call sites pass
   a literal 0, so the width is not witnessed. 47 words, 0 differing. */
void func_800AAA54(UnkDungeonActor *a, UnkVec *v, u8 *c, s32 d)
{
    a->unk_9A = 8;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    *(s32 *)(c + 0x2C) = d;
    if (d != 0)
    {
        func_80047784(c, ((u8 *)d)[((D_80083228 + *(s16 *)((u8 *)a + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u32 *)((u8 *)a + 0x1C) |= 0x4000000;
    func_80042B68((s8 *)a, 4);
    if ((s16)func_80042900((s8 *)a, 0xA) != 0)
    {
        *((u8 *)a + 0x13) = *((u8 *)a + 0xA8);
        func_80042518((s8 *)a, 0xA);
    }
    a->unk_96 = 0x40;
}

/* The 0x201 hand-over: refuse unless D_800E296C's 0x100000 bit is up and the
   D_80083460 record names THIS view with its counter clear, then re-dress
   through func_800C77D0, announce with func_800A56E0(0x201), and report 1
   only if the actor passes a seven-term screen. func_8008D1D0 is the same
   head, call for call. 101 words, 0 differing.

   TWO THINGS WERE MEASURED HERE, both worth a word each.

   (1) `ok` MUST BE NARROW. As `s32 ok` the `w != d` arm's `return 0` comes
   out as `addu $v0, $s1, $zero` -- cse substitutes the pseudo it knows holds
   zero for the constant -- 101 words, 1 differing. An HImode (or QImode)
   pseudo cannot stand in for an SImode `const_int 0`, so the arm keeps its
   own `addu $v0, $zero, $zero`. `s16` and `u8` are byte-identical; the return
   needs no widening either way because the value is only ever 0 or 1.

   (2) THE `w == NULL` ARM AND THE func_800A1C58 ARM ARE ONE `goto`. The
   target's `return 0` at 0x800AAC7C is a block with two predecessors -- the
   `beqz` at 0x800AAB4C and the fall-through of the tail's `beqz` -- which is
   what stops jump.c folding it into either branch. Written as two separate
   `return 0` statements the tail's collapses into its own branch and the
   function is 100 words, 17 differing.

   The D_8007359C entry is read as a HALFWORD: `*(u16 *)&...unk_00` is the
   target's `lhu`, where the declared `u32 unk_00` gives `lw`. The index is
   `[d[0x13] - 1]`, whose -1 folds into the -0x14 displacement. */
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *w;
    s16 ok;

    ok = 0;
    if ((D_800E296C & 0x100000) != 0)
    {
        g = (u8 *)&D_80083460;
        w = *(u8 **)(g + 0xC);
        if (w == NULL)
        {
            goto fail;
        }
        if (w != d)
        {
            return 0;
        }
        if (*(s16 *)(g + 0xA) != 0)
        {
            return 0;
        }
        if ((*(u16 *)(g + 0x2) & 8) != 0)
        {
            return 0;
        }
        func_800C77D0(p - 0x20, b, 8, 0x300);
        func_800A1D4C(d, 1);
        func_800A56E0(0x201);
        if ((D_80013714 & 2) == 0)
        {
            D_80080AA0 = 1;
            D_80080A88 = 0;
        }
        if ((*(s32 *)(d + 0x54) & 0x800000) == 0 && (*(s32 *)(d + 0x14) & 0x4000) != 0 &&
            (*(u16 *)&D_8007359C[0][d[0x13] - 1].unk_00 & 0x80) != 0 && d[0x13] >= 2 &&
            d[0x11] >= 0x14 && (*(s32 *)(d + 0x1C) & 0x228) == 0 &&
            (s16)func_80042900((s8 *)d, 0x18) == 0)
        {
            ok = 1;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
        fail:
            return 0;
        }
        D_8008346C = 0;
    }
    return ok;
}

/* The state-0xC installer: park the handler slot, copy the tile the record at
   d's +0x60 - 0x14 is standing on into c, project it with func_8003DE58,
   place the mover on it and -- when d still has a +0x8A sweep left -- give
   the mover a +0x14 lift towards either the probed floor or d's own +0x88,
   then step c one tile along the stored facing. 151 words, 0 differing.

   THE +0x6A STORE COMES BEFORE THE +0x1C MASK. The other way round the
   `lw 0x60($s1)` that feeds the `lhu 0x2A` is issued after the mask's own
   load and its load-delay slot comes out a `nop` -- 149 words, 136
   differing, with the whole function shifted by one.
   THE DIVISION IS WRITTEN IN BOTH ARMS, not once after the join. The
   divisor is `(s16)t` where `t` is the just-stored +0x8A + 2, and the target
   sign-extends it inside EACH arm; computing the numerator per arm and
   dividing once after the join leaves one of the two `sll 16`/`sra 16` pairs
   out and is 150 words against 151. Cross-jumping puts the `div`, the `mflo`
   and the store back together. */
void func_800AACA4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 out[4];
    u8 *q;
    s16 h;
    s32 n;
    s32 t;

    p[0x9A] = 0xC;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(d + 0x6A) = *(u16 *)(*(u8 **)(d + 0x60) + 0x2A);
    *(u32 *)(d + 0x1C) &= 0xFFF7FFFF;
    *(s16 *)(p + 0x96) = 2;
    q = *(u8 **)(*(u8 **)(d + 0x60) - 0x14);
    if (func_8003DE58(*(u32 *)(*(u8 **)(*(u8 **)*(u8 **)*(u8 **)(q + 0x28) +
                                        (*(u8 **)(q + 0x2C))[2] * 4) +
                               4),
                      q, out, 0) == 0)
    {
        out[2] = -0x60;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(*(u8 **)(d + 0x60) - 0x18) + 0xA) + (u16)out[2];
    c[0x24] = q[0x24];
    c[0x25] = q[0x25];
    func_800A2B04(b, c[0x24], c[0x25]);
    if (*(s16 *)(d + 0x8A) != 0)
    {
        h = func_800BCB04(
            (((c[0x24] + D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] * *(s16 *)(d + 0x8A)) << 6) +
             0x20) &
                0xFFE0,
            (((c[0x25] + D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] * *(s16 *)(d + 0x8A)) << 6) +
             0x20) &
                0xFFE0,
            *(s16 *)((u8 *)b + 0xA));
        t = *(u16 *)(d + 0x8A) + 2;
        *(s16 *)(d + 0x8A) = t;
        if (h < 0x200)
        {
            b->unk_14 = -((*(s16 *)((u8 *)b + 0xA) - h) << 15) / (s16)t;
        }
        else
        {
            b->unk_14 =
                -((*(s16 *)((u8 *)b + 0xA) - *(s16 *)(*(u8 **)(d + 0x60) + 0x88)) << 15) / (s16)t;
        }
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) - 2;
    }
    c[0x24] += D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    c[0x25] += D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7];
}

/* 176 words, 0 differing.  Parked at 4 for a day as "the proved-unreachable
   `|= 0x80000000` gate" and closed by the cure in docs/matching-levers.md,
   "THE `|= 0x80000000` GATE IS A LOCALITY PROBLEM, NOT UNREACHABLE": assign
   the constant to `cc2` ONE BLOCK EARLIER -- in the block holding the
   `& 0x400` test -- and give it a SECOND reference so a single-use REG_EQUIV
   constant is not rematerialised back into the body block.  Wrapping the
   store in `do { } while (0);` is that reference in ordinary C: flow.c:2128
   weights each reference by `loop_depth` and the loop NOTEs emit nothing.
   (`__asm__("" ::"r"(cc2));` after the store does the same thing and is how
   this first landed; the do-while is preferred because it is not a scaffold.
   Both are 176 words / 0 differing; without either, 4 rows.  Of the plain-C
   placements, `do { }` around the store and around `cc2 = 0x80000000;` are
   both 0, a duplicated-arm `if (p)` around the store is 0, and everything
   else measured runs 2 or worse.)  Multi-block, the constant leaves local_alloc,
   `block_alloc` can no longer tie the `ior`'s destination to it, and
   global_alloc hands the loaded value $v0 and the constant $v1 -- the
   target's `lw $v0` / `lui $v1` / `or $v0, $v0, $v1`.  Nothing else in the
   parked body changed; the four rows went to 0 on the first compile.

   The other four levers, all still load-bearing (see the park record in
   git history, docs/dungeon-parks/func_800AAF00.c before this landing):
   `*(volatile u16 *)` on the FIRST read of (p + 0x46) -- the 0x4000 test,
   not the re-read, because that is where the target has an unfilled delay
   slot; the table as a pointer local `tbl = (UnkDungeonName10 *)D_8006DE24`
   with `e = &tbl[i]`, which splits `lui %hi` / `addiu %lo` around the
   stride-0x14 multiply; `i` REUSED for the table index it loads, so a
   second set takes it out of local-alloc and global_alloc hands it $a0
   (worth ten of the twelve table rows); and `gp = p + i * 3` as its own
   pointer local, which is the only one of seven spellings that emits
   `addu $v0, $s0, $v0` in the source operand order.  The first gate is
   `(*(s16 *)(p + 0x46) & 0x3FFF) >= 5` through s16, not u16: through u16
   combine proves the masked value non-negative and the compare becomes
   `sltiu` where the target has `slti`. */
void func_800AAF00(UnkDungeonActor *a, UnkVec *b, u8 *c, s32 t, void (*h)())
{
    u8 *p;
    u8 *g;
    u8 *g2;
    u8 *q;
    u8 *gp;
    UnkDungeonName10 *e;
    UnkDungeonName10 *tbl;
    u16 f;
    s32 k;
    s32 i;
    s32 cc2;

    p = (u8 *)a;
    k = 0;
    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    f = *(u16 *)(g + 0x2);
    if ((f & 0x2008) != 0)
    {
        return;
    }
    if ((*(u32 *)(p + 0x1C) & 0x2000) != 0 && (*(s16 *)(p + 0x46) & 0x3FFF) >= 5)
    {
        q = D_800E3D7C;
        k = 1;
        if ((*(u32 *)(q + 0x98) & 0x3000) == 0x2000)
        {
            if (*(u16 *)(q + 0xA6) == 0)
            {
                *(u16 *)(q + 0x98) |= 0x1000;
                *(u16 *)(q + 0xA6) += 1;
                *(u16 *)(g + 0xA) += 1;
                goto join;
            }
        }
        func_800A4ACC(p);
        p[0x6D] -= 1;
        a->unk_8C = h;
        return;
    }
    if ((f & 8) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
    func_800C77D0(p - 0x20, b, 8, 0x300);
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
join:
    if (k == 0)
    {
        if ((*(u32 *)(p + 0x1C) & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(p + 0x14) >= 0)
            {
                do
                {
                    *(u32 *)(p + 0x14) |= cc2;
                } while (0);
                *(u16 *)(p + 0x2A) += (func_800A6D30() & 7) << 9;
            }
        }
        if ((*(volatile u16 *)(p + 0x46) & 0x4000) == 0)
        {
            i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
            gp = p + i * 3;
            i = gp[8];
            tbl = (UnkDungeonName10 *)D_8006DE24;
            e = &tbl[i];
            if (e->unk_12 == 2 && *(s32 *)(p + 0x60) == 0)
            {
                *(s32 *)(p + 0x60) =
                    func_800A05A4(p, c[0x24], c[0x25], *(s16 *)(p + 0x2A), e->unk_13);
            }
        }
    }
    else
    {
        *(u8 **)(D_800E3D7C + 0x11C) = p;
    }
    a->unk_9A = 0x12;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    if (t != 0)
    {
        *(s32 *)(c + 0x2C) = t;
        func_80047784(c, ((u8 *)t)[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u16 *)(c + 0x14) |= 0x800;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0xA) += 1;
}

/* Ease the mover onto the tile at c[0x24]/c[0x25] over the +0x96 countdown,
   and on the last frame re-place it, bleed the engine's +0x08 counter, clear
   d's +0x46 bit and re-probe the floor height. Returns 0 while the countdown
   is still running and 1 once it has expired. 110 words, 0 differing.

   EACH AXIS' `- 0x20` NEEDS ITS OWN LOCAL, ASSIGNED INSIDE THE EXPRESSION.
   Written `((c[0x24] << 6) - (*(s16 *)((u8 *)b + 0x2) - 0x20)) << 16` fold's
   `associate:` moves the constant to the other operand -- `addiu $v0, 0x20`
   on the shifted tile where the target has `addiu $v1, -0x20` on the loaded
   position -- 111 words / 81 differing. Hoisting the subtraction into a
   statement of its own fixes the arithmetic but leaves the two chains in the
   wrong order (110 / 4), and ONE local shared by both axes keeps that 4;
   two distinct locals assigned INSIDE the subtraction, `(... - (x = ...))`,
   is 0. */
s32 func_800AB1C0(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s16 h;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        *(s16 *)(a + 0x96) = 0;
    }
    if (*(s16 *)(a + 0x96) != 0)
    {
        b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(a + 0x96);
        b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(a + 0x96);
    }
    *(s16 *)(a + 0x96) = *(u16 *)(a + 0x96) - 1;
    if (*(s16 *)(a + 0x96) > 0)
    {
        return 0;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(s16 *)(g + 0x8) = *(u16 *)(g + 0x8) - 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
    {
        *(u16 *)(d + 0x46) &= 0x7FFF;
    }
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        *(u32 *)(d + 0x1C) &= 0xBFFFFFFF;
        h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(d + 0x88) - 0x20);
        *(s16 *)(a + 0x92) = *(u16 *)(a + 0x92) + (*(u16 *)(d + 0x88) - h);
        *(s16 *)(d + 0x88) = h;
        if ((*(u32 *)(d + 0x1C) & 0x40000) == 0)
        {
            *(s32 *)(a + 0x90) = 0;
        }
    }
    return 1;
}

/* WRITTEN K&R ON PURPOSE. The body reads all four argument registers, but the
   three landed installers that call it (func_800B2C68, func_800D45D0,
   func_800D9C18) are PURE FORWARDS written with no arguments at all, so a
   prototyped four-parameter definition is `too few arguments to function`
   three times over. clang-format wants the first parameter declaration at
   COLUMN ZERO here -- an `s32` return with a pointer first parameter -- not
   indented as the K&R note in docs/town-sweep.md says. 59 words,
   0 differing. */
s32 func_800AB378(p, b, c, d)
u8 *p;
UnkVec *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *q;
    s16 t;

    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(s16 *)(p + 0x96) = 0;
    }
    if (*(s16 *)(p + 0x96) != 0)
    {
        q = *(u8 **)(*(u8 **)(d + 0x60) - 0x18);
        b->unk_0C = *(s32 *)(q + 0xC);
        b->unk_10 = *(s32 *)(q + 0x10);
    }
    t = *(u16 *)(p + 0x96) - 1;
    *(s16 *)(p + 0x96) = t;
    if (t <= 0)
    {
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        return 1;
    }
    return 0;
}

/* The `blez` installer gate. MUST STAY K&R, like func_800AC7EC above:
   func_800D491C and func_800DA7D0 call this with NO arguments.

   THE RESULT LOCAL IS `s32` WITH AN EXPLICIT `(s16)` AT BOTH USES. As `s16 r`
   the test's `sll` takes the RAW call result instead of the copy in $v1 -- 53
   words, 3 differing -- and `return r` without the cast is 52 / 14. The big
   arm must also be written FIRST: with the `r = 1` arm first the `bnez`
   inverts and the layout costs 32 differing.

   func_800ADAE4 is deliberately NOT declared at the top of this unit: it is
   landed below as s16, and with that prototype in scope gcc drops both casts
   and the function is 52 words / 16 differing. Widening the landed definition
   to s32 breaks func_800ADAE4 itself (42 words / 12). The implicit `int`
   declaration this call site rides is the only resolution, and it costs three
   warnings of the class this unit already carries. 53 words, 0 differing. */
s32 func_800AB464(a, b, c, p)
UnkDungeonActor *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 r;
    u8 *g;

    if (a->unk_9B == 0)
    {
        p[0x71] &= 0x7F;
        g = (u8 *)&D_80083460;
        if (*(u16 *)(g + 0x2) & 0x2000)
        {
            return 0;
        }
        if ((s16)func_800A2B5C(p) != 0)
        {
            return 0;
        }
        r = func_800ADAE4(c, p);
        if ((s16)r != 0)
        {
            *(s32 *)(g + 0xC) = 0;
            *(u16 *)(g + 0x14) -= 1;
            a->unk_9B = 1;
        }
    }
    else
    {
        r = 1;
    }
    return (s16)r;
}

/* The three-state cutscene pan: state 0 arms the +0x96 countdown and either
   jumps straight to state 2 or seeds the view's +0x8A step, state 1 runs the
   per-frame glide towards D_80083780 and hands the leash back at the end,
   state 2 stops the pan and raises the view's 0x800000.

   WRITTEN K&R, AND THE DECLARATION IS WIDENED TO `s32`. Every exit sets
   `$v0` to zero, which is a `return 0` and not a `void` tail, so the
   declaration at the top of the file changed from `void func_800AB538();` to
   `s32 func_800AB538();` -- both landed callers (func_800B306C :12519 and
   func_800D9F9C :24048) discard it and both still measure 8 words,
   0 differing. They sit BELOW this definition and pass no arguments, so an
   ANSI four-parameter definition is `too few arguments` twice over; K&R is
   the only form that compiles.

   THE MOVER IS `u8 *`, NOT `UnkVec *`, and that is worth two words. A
   COMPONENT_REF store is MEM_IN_STRUCT_P with a varying address, which
   `true_dependence` (sched.c:854) excuses against a non-in-struct load at a
   non-varying address -- so with `b->unk_0C = 0` the two D_80083780 reads and
   the D_8008378A read are free to float into the stores' load-delay slots and
   two `nop`s the target keeps disappear. Through `*(s32 *)(b + 0xC)` casts
   the dependence is recorded and the order is the target's: 144 words against
   142 with 76 differing.

   Case 2's two `sh $zero` stores are in one basic block with nothing to
   split it, so their order is a TIE in the bytes; they are written +0x1E
   then +0x1C to match. 144 words, 0 differing. */
s32 func_800AB538(p, b, c, d)
u8 *p;
u8 *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    s16 t;

    switch (p[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x96) = 0x10;
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        *(u16 *)(p + 0x98) |= 0x8;
        if ((D_80013714 & 2) != 0)
        {
            p[0x9B] = 2;
        }
        else
        {
            *(s16 *)(d + 0x8A) = 0x800 / *(s16 *)(p + 0x96);
            p[0x9B]++;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x96) - 2 > 0)
        {
            g = D_80083780;
            *(s32 *)(b + 0xC) = ((*(s16 *)(g + 0x2) - *(s16 *)(b + 0x2)) << 16) / (*(s16 *)(p + 0x96) - 2);
            *(s32 *)(b + 0x10) = ((*(s16 *)(g + 0x6) - *(s16 *)(b + 0x6)) << 16) / (*(s16 *)(p + 0x96) - 2);
        }
        else
        {
            g2 = D_80083780;
            *(s32 *)(b + 0x10) = 0;
            *(s32 *)(b + 0xC) = 0;
            *(s16 *)(b + 0x2) = *(u16 *)(g2 + 0x2);
            *(s16 *)(b + 0x6) = *(u16 *)(g2 + 0x6);
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            *(s32 *)(b + 0x14) = ((D_8008378A - (func_800644B8(*(s16 *)(p + 0x96) << 6) >> 4) - *(s16 *)(b + 0xA)) << 16) / *(s16 *)(p + 0x96);
        }
        t = *(u16 *)(c + 0x1C) - *(u16 *)(d + 0x8A);
        *(s16 *)(c + 0x1C) = t;
        *(s16 *)(c + 0x1E) = t;
        t = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = t;
        if (t < 0)
        {
            *(s32 *)(b + 0x14) = 0;
            *(s16 *)(d + 0x88) = D_8008378A;
            *(s16 *)(b + 0xA) = D_8008378A;
            *(s16 *)(p + 0x92) = 0;
            p[0x9B]++;
        }
        break;
    case 2:
        *(s16 *)(c + 0x1E) = 0;
        *(s16 *)(c + 0x1C) = 0;
        *(s32 *)(d + 0x1C) |= 0x800000;
        break;
    }
    return 0;
}

/* The engine's four-state teardown for a retiring actor, switched on the
   actor's +0x9B: state 0 bumps D_80083460's +0x0A census and advances to 3
   (clearing the resident's +0x110 slot when the record's +0x43 is 0xFD and
   otherwise refusing if func_800A2C78 says the floor is busy); state 3
   either hands a sub-0x40 record to func_800C542C and jumps straight to
   0x10, or arms the +0x96 fade and falls into state 4; state 4 runs the
   five-way fade down -- three colour bytes and two scales, each divided by
   what is left of the counter -- and on the last frame announces the record,
   releases its D_800E3DF0 slot and its D_800E3E48 shadow and tears it down;
   state 0x10 re-raises the 0x4000 flag, announces D_800E0C34 and runs the
   same teardown without the census arithmetic. 401 words, 0 differing.

   ONE `u8 *` LOCAL PER `&D_80083460` MATERIALISATION -- g, g2, g3, g4, four
   sites in three cases. With ONE local reassigned the `%hi` lands in a
   different register from its `%lo` at three of the four sites and the +0x0A
   read-modify-writes lose their scheduling: 402 words, 127 differing, where
   four locals plus the in-place `f` mask below is 401 / 8. This is
   docs/matching-levers.md's "THE NUMBER OF SOURCE LOCALS HOLDING ONE
   GLOBAL'S ADDRESS DECIDES WHETHER ITS %hi AND %lo SHARE A REGISTER", and
   it is the single biggest lever in the function.

   TWO `Unk800E3E48 *` LOCALS, ONE PER SITE, AND THEY ARE WHAT KEEPS +0x13 A
   DISPLACEMENT. Written `((u8 *)&D_800E3E48[j])[0x13] = 0` the address is
   `plus(plus(mult, symbol), 19)` and plus_constant folds the 19 into the
   symbol, giving `%lo(D_800E3E48)+0x13` and `sb $zero, 0x0($v0)` where the
   target has `addiu %lo(D_800E3E48)` and `sb $zero, 0x13($v0)` -- 2
   differing. A pointer LOCAL makes the base a register, so the 19 has
   nowhere to fold: `sh = D_800E3E48; *((u8 *)&sh[j] + 0x13) = 0;`. One local
   SHARED between the two sites is 403 words / 176 differing; two locals are
   401 / 4. `*(u8 *)((s32)&D_800E3E48[j] + 0x13)` folds the same way as the
   array form and buys nothing.

   THE SLOT INDEX IS TWO NARROW LOCALS: `s16 i` for D_800E3DF0 and `s16 j`
   for D_800E3E48, per docs/matching-levers.md's "THE SCRATCHPAD INDEX TYPE
   CAN BE WORTH 45 DIFFERING WORDS BY ITSELF", which was measured on this
   very function: `s32`/`u32` is 403 words / 53 differing, one narrow index
   is 401 / 18, two are 401 / 8.

   `k = f & 0x4000;` IS A SEPARATE LOCAL *BEFORE* THE CALL, AND THAT IS THE
   `jal func_80099844` DELAY SLOT. Written `if ((f & 0x4000) == 0)` after the
   call the mask gets its own pseudo in $v0 and dbr fills the slot with
   `addiu $a1, $a1, %lo(D_800E0C34)` instead -- 4 differing. `f &= 0x4000;`
   in place before the call ties the result to $s0 but sched1 then hoists the
   `andi` ahead of both argument setups -- 6 differing. Only a separate local
   leaves the `andi` adjacent to the `jal`, which is where dbr takes it from.
   401 / 2 -> this is worth 2 words.

   STATE 0'S PROBE EXIT IS A `break`, AND THE `!= 0xFD` ARM ORDER GOES WITH
   IT. As `return 0` the exit compiles to an inline `move $v0,0` + `j`, which
   is two insns, so jump.c's conditional-jump-around-an-unconditional-jump
   rule cannot fire and the branch comes out `beqz` into the join with a
   `nop` in the following `j`'s delay slot -- 2 differing. As `break` the
   exit is a bare `j` to the switch's shared `return 0`, jump.c inverts it to
   `bnez` and reorg steals the zero into its delay slot and redirects to the
   epilogue, which is the target. But `break` ALONE also lets gcc reorder the
   two arms of the +0x43 test and merge both `j`s: 399 words, 349 differing.
   Writing the test `if (d[0x43] != 0xFD) { probe } else { clear }` puts the
   arms back in the target's order. Either change alone is no better than the
   base; together they are the last 2 words.

   `k` carries three disjoint values -- the `f` mask, the +0x43 record index
   and func_800A1BD0's slot index -- and that is what the register assignment
   wants. 0x80010A80 is a CONSTANT ADDRESS (`lui`/`ori`, not `lui`/`addiu`);
   `ovlverify --raw` has both words byte-identical to the target, and the
   other 90 raw differences are all `jal`/`j`/`%hi`/`%lo` relocation fields. */
s32 func_800AB778(p, b, c, d)
u8 *p;
void *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *g4;
    u8 *e;
    u8 *base;
    Unk800E3E48 *sh;
    Unk800E3E48 *sh2;
    void *h;
    void *m;
    s32 f;
    s16 k;
    s16 n;
    s16 i;
    s16 j;

    f = *(s32 *)(d + 0x14);
    if ((f & 0x20000000) != 0)
    {
        *(s32 *)(d + 0x14) = f | 0x400000;
        func_800ACB98(p, b, c, d);
        return 0;
    }
    switch (p[0x9B])
    {
    case 0:
        if (d[0x43] != 0xFD)
        {
            if ((s16)func_800A2C78(d) != 0)
            {
                break;
            }
        }
        else
        {
            *(s32 *)(D_800E3D7C + 0x110) = 0;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] = 3;
        break;
    case 3:
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_80094E34();
        }
        if (d[0x43] < 0x40)
        {
            func_800C542C(d, D_800DCED4[func_800429E4(d)], d == *(u8 **)(D_800E3D7C + 0xAC), 2);
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 0x10;
            break;
        }
        if (*(u16 *)(c + 0x12) == 0)
        {
            *(u16 *)(c + 0x12) = 0xFF80;
        }
        *(s16 *)(c + 0x10) = 0x20;
        *(u16 *)(c + 0x14) |= 0xC;
        *(s16 *)(p + 0x96) = 0x10;
        *(s32 *)(d + 0x1C) |= 0x10000000;
        p[0x9B]++;
        break;
    case 4:
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x96);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x96);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1C) += (0x400 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1E) += (0x400 - *(u16 *)(c + 0x1E)) / *(s16 *)(p + 0x96);
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return 0;
        }
        h = func_800990FC();
        m = func_8009929C(8, h);
        if (d[0x43] == 0xFF)
        {
            m = func_80099194(D_800E0C58, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else if (d[0x43] == 0xFE)
        {
            m = func_80099194(D_800E0C67, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else
        {
            D_800DCE68 -= 1;
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
        }
        func_800A31D0(d);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        if (*(s32 *)(D_800E3D7C + 0x110) != 0)
        {
            func_800A18E8(d[0x13], 3);
            e = *(u8 **)(D_800E3D7C + 0x110);
            i = e[3] & 0x1F;
            j = i;
            ((u8 *)D_800E3DF0[i])[0x13] = 0;
            sh = D_800E3E48;
            *((u8 *)&sh[j] + 0x13) = 0;
            D_800E3DF0[i] = 0;
            func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        }
        else
        {
            func_800A18E8(d[0x13], 2);
        }
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) -= 1;
        if (*(s32 *)(g3 + 0xC) == (s32)d)
        {
            *(s32 *)(g3 + 0xC) = 0;
        }
        break;
    case 0x10:
        *(s32 *)(d + 0x14) = f | 0x4000;
        k = f & 0x4000;
        func_80099844(d, D_800E0C34);
        if (k == 0)
        {
            *(s32 *)(d + 0x14) &= ~0x4000;
        }
        func_800A31D0(d);
        k = d[0x43];
        d[0x44] = 0;
        if ((s16)func_80042900((s8 *)d, 0xA) != 0)
        {
            d[0x13] = d[0xA8];
        }
        func_800422DC((u8 *)(k * 0x54 + 0x80010A80), (s32)d);
        func_800A18E8(d[0x13], 3);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        e = *(u8 **)(D_800E3D7C + 0x110);
        i = e[3] & 0x1F;
        j = i;
        ((u8 *)D_800E3DF0[i])[0x13] = 0;
        sh2 = D_800E3E48;
        *((u8 *)&sh2[j] + 0x13) = 0;
        D_800E3DF0[i] = 0;
        func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g4 = (u8 *)&D_80083460;
        *(u16 *)(g4 + 0xA) -= 1;
        break;
    }
    return 0;
}

/* The three-state climb/descend glide: case 0 seeds the 0x10-frame countdown
   and either re-places the mover and jumps to state 2 or parks the per-frame
   lift in d's +0x8A, case 1 eases the mover onto the tile over the remaining
   frames and, on the frame the countdown goes negative, stops it dead and
   advances the state, and case 2 reports done. 149 words, 0 differing.

   MUST STAY K&R: the landed func_800B30AC and func_800D9FDC call this with
   NO arguments, so a prototyped four-parameter definition is "too few
   arguments to function" twice over -- the same rule
   func_800AB378 and func_800AC7EC above record.

   EACH `- 0x20` AND THE `+ 0x20` NEEDS ITS OWN LOCAL ASSIGNED INSIDE THE
   EXPRESSION, the func_800AB1C0 lever one function up. Written plainly,
   fold's `associate:` moves the constant onto the other operand -- `addiu
   $v0, 0x20` on the shifted tile byte where the target has `addiu $v1,
   -0x20` on the loaded position, and `addiu $v0, -0x20` on the negated
   lift where the target has `addiu $v1, 0x20` on the +0x92 read.
   AND CASE 1'S FIRST TEST IS WRITTEN THE WAY THE TARGET BRANCHES: as
   `if (unk_96 + 2 >= 0x10) { zero the two deltas } else if ...` the `slti`'s
   `bnez` goes to the else and the two `sw $zero` are inline; the natural
   `< 0x10` spelling inverts the whole arm.
   THE QUOTIENT OF case 0's ELSE ARM IS A LOCAL. As
   `*(s16 *)(d + 0x8A) = 0x800 / *(s16 *)(a + 0x96);` the `mflo` lands in $v0,
   the store cannot be deferred past the +0x9B read-modify-write and the arm
   is one word long with the operands of the `div` swapped (150 words,
   104 differing with the other two levers already applied). */
s32 func_800ABDBC(a, b, c, d)
u8 *a;
UnkVec *b;
u8 *c;
u8 *d;
{
    s16 q;
    s32 x;
    s32 y;
    s32 z;

    switch (a[0x9B])
    {
    case 0:
        *(s16 *)(a + 0x96) = 0x10;
        *(u32 *)(d + 0x1C) &= 0xFFFBFFFF;
        if (D_80013714 & 2)
        {
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(c + 0x1E) = 0x1000;
            a[0x9B] = 2;
        }
        else
        {
            q = 0x800 / *(s16 *)(a + 0x96);
            a[0x9B] += 1;
            *(s16 *)(d + 0x8A) = q;
        }
        break;
    case 1:
        if (*(s16 *)(a + 0x96) + 2 >= 0x10)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
        }
        else if (*(s16 *)(a + 0x96) - 2 > 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) /
                        (*(s16 *)(a + 0x96) - 2);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) /
                        (*(s16 *)(a + 0x96) - 2);
        }
        else
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
        }
        if (*(s16 *)(a + 0x96) != 0)
        {
            b->unk_14 = ((-(func_800644B8(*(s16 *)(a + 0x96) << 6) >> 4) -
                          (z = *(s16 *)(a + 0x92) + 0x20))
                         << 16) /
                        *(s16 *)(a + 0x96);
        }
        *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(d + 0x8A);
        if ((*(s16 *)(a + 0x96) -= 1) < 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(a + 0x92) = -0x20;
            a[0x9B] += 1;
        }
        break;
    case 2:
        return 1;
    }
    return 0;
}

/* func_800AC010 -- 284 words, 0 differing.  State 4's per-frame step for the
   four-argument installer family.

   THE LAST FOUR WORDS WERE A sched1 PRIORITY BOOST, AND THE CURE IS FOUR
   SINGLE-SET LOCALS.  The residue was one transposition of two independent
   two-insn chains at 0x800AC04C -- target `lbu`/`lh`/`sll`/`addiu`, candidate
   `lh`/`lbu`/`addiu`/`sll` -- and it survived sixteen spellings of the
   subtraction.  Cause, read out of `tools/decomp/sched.py --sched1`:
   `adjust_priority` (sched.c:2551) hands LAUNCH_PRIORITY (0x7f000001) to any
   insn that becomes ready while `birthing_insn_p` holds, i.e. that SETs a
   live pseudo with `reg_n_sets == 1`.  With ONE `s32 x` shared by the two
   subtractions, `x` has two sets, the `addiu` is not birthing and not
   boosted, the `sll` into its compiler temp IS, and the boosted insn wins the
   cycle in which both become ready -- which in a backward scheduler means it
   is EMITTED LAST, putting the `lh` chain first.  Give every operand of both
   divisions its own write-once local (`u1`/`x1`, `u2`/`x2`) and both chain
   tails are birthing, the tie falls through to INSN_LUID, and the chain
   written SECOND is emitted first, which is the target's order.
   The subtrahend still MUST go through a local: written inline, `fold`'s
   associate path rewrites `A - (B - 0x20)` into `(A + 0x20) - B` and the
   `addiu` comes out `+32` on the shifted byte.  `s32 x; s32 y; s32 u;` with
   `u` shared is 285 words / 12 -- a shared local goes to global_alloc
   (local-alloc.c:472 wants `reg_n_deaths == 1`) and costs the extra word.

   THE THREE DEVICES THAT GOT IT TO 284 WORDS BEFORE THAT, each measured:
   1. `goto fail;` IN THE SECOND func_8009B88C ARM.  Both arms end in the same
      `func_800A2B04` / `func_800AA5E4` / `return 0` tail.  Written out twice,
      jump.c's forward scan always deletes the FIRST copy, so the shared tail
      lands at the END of the function where the target has it at 0x800AC1C0.
      Both arms `== 0`: 282 / 38.  Arm 1 `== 0`, arm 2 `!= 0`: 282 / 33.
      Label in arm 1 and `goto fail` in arm 2: 282 / 12.
   2. A TRAILING `out: return 0;`.  The `*(s16 *)(a + 0x96) = 2` arm's
      `return 0` must cross-jump onto a one-insn `v0 = 0` block that falls
      into the epilogue; that is what makes the final `return 1` need its own
      `j .L800AC458`.  Worth the last two words: 282 -> 284.
   3. `s32 func_8009B164s(s16, UnkVec *, u8 *) __asm__("func_8009B164");`
      The landed definition takes `u16` (its own `(a >> 9) & 7` needs the
      logical shift), and through it the argument load is `lhu` where the
      target has `lh`.

   MUST STAY K&R: `s32 func_800AC010();` at the top of the unit and two landed
   call sites pass NO arguments, so an ANSI four-parameter definition makes
   both "too few arguments" -- the same constraint func_800AC82C records. */

s32 func_800AC010(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *d;
    u8 *g;
    u8 *h;
    s32 u1;
    s32 x1;
    s32 u2;
    s32 x2;
    s16 o0;
    s16 o1;
    s16 t;

    if (*(s16 *)(a + 0x96) != 0)
    {
        u1 = c[0x24] << 6;
        x1 = *(s16 *)((u8 *)b + 0x2) - 0x20;
        b->unk_0C = ((u1 - x1) << 16) / *(s16 *)(a + 0x96);
        u2 = c[0x25] << 6;
        x2 = *(s16 *)((u8 *)b + 0x6) - 0x20;
        b->unk_10 = ((u2 - x2) << 16) / *(s16 *)(a + 0x96);
    }
    *(s16 *)(a + 0x96) = *(u16 *)(a + 0x96) - 1;
    if (*(s16 *)(a + 0x96) > 0)
    {
        return 0;
    }
    d = (u8 *)func_8009C93C(p, c, *(s16 *)(p + 0x6A), 0, 0);
    if (d != NULL)
    {
        func_8009CE1C(p, 4, 1, 8, (s16)(*(u16 *)(p + 0x6A) + 0x800), 0, 1);
        func_8009CE1C(d, 8, 1, 8, *(s16 *)(p + 0x6A), 0, 1);
        *(u16 *)(a + 0x98) &= 0xFFF7;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s32 *)(p + 0x1C) |= 0x40000000;
        a[0x9C] = c[0x26];
        c[0x26] = func_8009FB34(c[0x24], c[0x25]);
        func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
        if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
        {
        fail:
            func_800A2B04(b, c[0x24], c[0x25]);
            func_800AA5E4(a, b, c, p);
            return 0;
        }
        c[0x24] = o0;
        c[0x25] = o1;
        func_800A2B04(b, c[0x24], c[0x25]);
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s32 *)(p + 0x1C) &= 0xFFDFFFFF;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(s16 *)(a + 0x96) = 0;
        p[0x71] = 0;
        return 1;
    }
    if ((s16)func_8009B164s(*(s16 *)(p + 0x6A), b, c) != 0)
    {
        *(s16 *)(p + 0x8A) = *(u16 *)(p + 0x8A) - 1;
        if (*(s16 *)(p + 0x8A) > 0)
        {
            c[0x24] += *(u8 *)&D_8006CCD8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            c[0x25] += *(u8 *)&D_8006CCE8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            *(s16 *)(a + 0x96) = 2;
            goto out;
        }
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    *(u16 *)(a + 0x98) &= 0xFFF7;
    a[0x9C] = c[0x26];
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
    t = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                      (s16)(*(u16 *)((u8 *)b + 0xA) - 0x20));
    if ((s16)t < 0x200)
    {
        *(s16 *)(a + 0x92) = 0;
        *(s16 *)(p + 0x88) = t;
    }
    h = (u8 *)&D_80083460;
    *(u16 *)(h + 0xA) -= 1;
    *(s16 *)(a + 0x96) = 0;
    *(s32 *)(p + 0x1C) = (*(s32 *)(p + 0x1C) | 0x40000000) & 0xFFDFFFFF;
    if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
    {
        goto fail;
    }
    c[0x24] = o0;
    c[0x25] = o1;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0x71] = 0;
    return 1;
out:
    return 0;
}

/* Glide the mover towards the tile record's centre over the +0x96 countdown,
   then -- on the frame it reaches zero -- stop it dead, re-place it on the
   tile, and hand the leash at +0x88 back to the view. 82 words, 0 differing.

   THE 0x20 IN THE FIRST NUMERATOR IS A VARIABLE, and that is the whole
   function. Both halves want `(byte << 6) - (halfword - 0x20)`, which fold
   reassociates into `(byte << 6) - halfword + 0x20` unless the subtrahend is
   its own statement -- and with the statement the two equal-depth chains of
   the FIRST numerator schedule the other way round: `lh 0x2($a0)` before
   `lbu 0x24($s0)`, four words differing, with every other word in the
   function exact. rank_for_schedule has nothing left to separate them but
   INSN_LUID, and no source order reaches it: thirty-three spellings were
   measured, including the byte hoisted into a local of either type, two
   numerator locals, either store as a cast, `* 0x10000` for `<< 16`, and an
   empty `__asm__ volatile("")` between the chains (84 words) or before them
   (no change). Holding the 0x20 in a local instead blocks the reassociation
   without a statement boundary, and the chains come out in the target's
   order. The second numerator keeps the ordinary spelling; it was never
   wrong. decomp-permuter found this in two minutes after the hand sweep had
   run out. */
s32 func_800AC480(u8 *p, UnkVec *v, u8 *r, u8 *b)
{
    s16 n;
    s16 d;
    s32 w;
    u8 *g;

    n = *(s16 *)(p + 0x96);
    if (n != 0)
    {
        w = 0x20;
        v->unk_0C = (((r[0x24] << 6) - (*(s16 *)((u8 *)v + 0x2) - w)) << 16) / n;
        w = *(s16 *)((u8 *)v + 0x6) - 0x20;
        v->unk_10 = (((r[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
    }
    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return 0;
    }
    v->unk_14 = 0;
    v->unk_10 = 0;
    v->unk_0C = 0;
    func_800A2B04(v, r[0x24], r[0x25]);
    d = func_800BCB04((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(u16 *)(b + 0x88) - 0x20);
    if (d < 0x200)
    {
        *(s16 *)(p + 0x92) += *(u16 *)(b + 0x88) - d;
        *(s16 *)(b + 0x88) = d;
    }
    g = (u8 *)&D_80083460;
    *(s16 *)(g + 0xA) -= 1;
    return 1;
}

/* The three-state landing glide: state 0 stamps the facing into +0x6A, state
   1 pans +0x2A, bleeds the mover's +0x14 and on the last frame either tears
   the actor down or re-measures its leash, state 2 pans and closes with
   func_8009A21C and the +0x6A restore. 137 words, 0 differing.

   MUST STAY K&R: the landed func_800B321C and func_800DA760 both call this
   with NO arguments, and an ANSI four-parameter definition makes those calls
   "too few arguments to function" -- the same resolution func_800AB464 and
   func_800AC7EC next door already carry.

   THE TWO D_80083460 BASES ARE TWO SEPARATE LOCALS. One shared `u8 *g` is a
   single global-alloc quantity, so both sites get the same hard register and
   the `high` keeps its own -- 137 words, 6 differing, every differing word a
   `lui`/`addiu` register. Two locals are local-alloc quantities, the `high`
   is combined into each one's own register ($v1 and $v0) and it is 0.
   The spelling of the local is inert: `s16 *`, `u16 *` and `*(s16 *)(g+0xA)`
   all give the identical program. */
s32 func_800AC5C8(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *g;
    u8 *h;

    switch (a[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x2A) & 0xFFF;
        a[0x9B] += 1;
    case 1:
        *(s16 *)(p + 0x2A) += 0x200;
        b->unk_14 += 0xFFFF0000;
        if ((*(s16 *)(a + 0x96) -= 1) > 0)
        {
            return 0;
        }
        b->unk_14 = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(s16 *)(a + 0x96) = 0x10;
        if ((*(s32 *)(p + 0x14) & 0x4000) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            func_800A32A4(p);
            func_8009A028(p);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(s16 *)(p + 0x88) = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, -0x400);
            *(u16 *)(a + 0x98) &= 0xFFF7;
            *(u32 *)(p + 0x1C) &= 0xFFFEFFFF;
            *(s16 *)(a + 0x96) = 0x10;
            a[0x9B] += 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x2A) += 0x200;
        if ((*(s16 *)(a + 0x96) -= 1) > 0)
        {
            return 0;
        }
        *(s32 *)(a + 0x90) = 0;
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x6A);
        return 1;
    }
    return 0;
}

/* MUST STAY K&R: the landed func_800DA808 below calls this with NO
   arguments, and an ANSI four-parameter definition makes that call "too few
   arguments to function". 16 words, 0 differing. */
s32 func_800AC7EC(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 f;

    f = *(s32 *)(p + 0x1C);
    if (f & 0x8000000)
    {
        if (*(s32 *)(p + 0x14) & 0x40000)
        {
            *(s32 *)(p + 0x1C) = f | 0x40000;
        }
        return 1;
    }
    return 0;
}

/* The 0x9B state pair that re-seats an actor on its carrier: state 0 waits
   for func_8004CAE8 on the carrier's source record, state 1 copies the
   carrier's tile pair into the renderable, then either walks the eight-way
   D_8006CCD8/E8 offset onto it and asks func_8009B88C for a free cell, or --
   when the view carries 0x80000 -- places the mover from func_8003DE58's
   projection and pans +0x2A by the facing delta.

   MUST STAY K&R: the declaration at the top of the file is
   `s32 func_800AC82C();` and two landed call sites pass NO arguments, so an
   ANSI four-parameter definition makes both "too few arguments".

   THREE SPELLINGS IN THE 0x80000 ARM ARE EACH WORTH WORDS, measured:
   (1) the arm must be the `else` -- the 0x80000 test is written `== 0` with
       the ordinary block inline, since the target's `bnez` sends the 0x80000
       arm out of line (as the `!= 0` arm it is 221 words / 153 differing);
   (2) the arm must END BY FALLING INTO the function's shared `return 0`
       rather than returning itself: written `if (... == 0) { return 0; }
       <stores> return 0;` it is 221 words, the extra two being a `j` and a
       second `addu $v0, $zero, $zero`;
   (3) the facing delta is an `s32` LOCAL. The target reads the carrier's
       +0x2A twice, once `lhu` for the +0x6A store and once `lh` for the
       subtraction; with the subtraction inline, force_to_mode masks the
       whole chain to 16 bits, both loads come out `lhu` and cse then folds
       them into one (219 words, 24 differing). An `s16` temp does the same;
       only the `s32` keeps the sign extension and therefore both loads.
   The carrier pointer must also be its own local in that tail -- re-read as
   `*(u8 **)(p + 0x60)` it is reloaded after the +0x2A store (223 words).
   219 words, 0 differing. */
s32 func_800AC82C(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *e;
    u8 *f;
    s16 d;
    s16 o0;
    s16 o1;
    u8 *x;
    s32 t;
    s16 out[4];

    *(s16 *)(c + 0x6) = 0;
    switch (a[0x9B])
    {
    case 0:
        e = *(u8 **)(*(u8 **)(p + 0x60) - 0x14);
        if (func_8004CAE8(*(u8 **)(e + 0x8), 0) == 0)
        {
            return 0;
        }
        a[0x9B] += 1;
    case 1:
        if (*(s16 *)(p + 0x64) < 0)
        {
            func_80094E34();
        }
        if (*(s8 *)(p + 0x6D) != 0)
        {
            func_800A9A0C(p);
        }
        if ((*(s32 *)(p + 0x1C) & 0x200000) != 0)
        {
            func_800AACA4(a, b, c, p);
            return 0;
        }
        e = *(u8 **)(*(u8 **)(p + 0x60) - 0x14);
        f = *(u8 **)(*(u8 **)(p + 0x60) - 0x18);
        c[0x24] = e[0x24];
        c[0x25] = e[0x25];
        if ((*(s32 *)(p + 0x1C) & 0x80000) == 0)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
            *(u16 *)(a + 0x98) &= 0xFFF7;
            *(s16 *)(p + 0x6A) = *(u16 *)(*(u8 **)(p + 0x60) + 0x2A);
            c[0x24] = e[0x24] + (u16)D_8006CCD8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            c[0x25] = e[0x25] + (u16)D_8006CCE8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            func_800A2B04(b, c[0x24], c[0x25]);
            d = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(p + 0x88) - 0x20);
            if (d < 0x200)
            {
                *(s16 *)(a + 0x92) = 0;
                *(s16 *)(p + 0x88) = d;
            }
            if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) != 0)
            {
                c[0x24] = o0;
                c[0x25] = o1;
                func_800A2B04(b, c[0x24], c[0x25]);
                *(s32 *)(p + 0x1C) |= 0x40000000;
                func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                p[0x71] = 0;
                return 1;
            }
            func_800A2B04(b, c[0x24], c[0x25]);
            func_800AA36C(a, b, c, p);
            return 0;
        }
        if (func_8003DE58(*(u8 **)(e + 0x8), e, out, 0) != 0)
        {
            *(s16 *)((u8 *)b + 0x2) = (u16)D_800E3DA8[0] + (*(u16 *)(f + 0x2) + out[0]);
            *(s16 *)((u8 *)b + 0x6) = (u16)D_800E3DA8[1] + (*(u16 *)(f + 0x6) + out[1]);
            *(s16 *)(p + 0x88) = *(u16 *)(*(u8 **)(p + 0x60) + 0x88);
            *(s16 *)(a + 0x92) = (*(u16 *)(f + 0xA) + out[2]) - *(u16 *)(p + 0x88) + 8;
            *(s16 *)(c + 0x6) = 0x10;
            *(s32 *)(p + 0x1C) |= 0x40000000;
            x = *(u8 **)(p + 0x60);
            t = *(s16 *)(p + 0x6A) - *(s16 *)(x + 0x2A);
            *(s16 *)(p + 0x6A) = *(u16 *)(x + 0x2A);
            *(s16 *)(p + 0x2A) -= t;
        }
        break;
    }
    return 0;
}

/* 19 words, 0 differing. */
void func_800ACB98(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 0x14;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(c + 0x14) |= 0x800;
    *(s16 *)(p + 0x96) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) |= 0x10000000;
}

/* The two-state fade-out behind the func_800C6AEC binder: case 0 re-tags the
   node and clears the colour cursor, case 1 eases the three +0xC..+0xE bytes
   towards zero by `-v / ++c[0xF]` and, once the divisor reaches 8, runs
   func_800B30E4's teardown with one extra flag move. 100 words, 0 differing,
   first compile.

   THE EASE IS A NEGATE-THEN-DIVIDE, not a subtract: `c[0xE] += -c[0xE] /
   c[0xF]` is the `negu`/`div` pair the target has, where `c[0xE] -= c[0xE] /
   c[0xF]` would be `div`/`subu`. The three stores are ONE CHAINED
   assignment, which is why they come out 0xE, 0xD, 0xC -- the same rule
   func_800ACD74 records for its four.

   The teardown is func_800B30E4 verbatim plus the +0x14 / +0x1C 0x400000
   hand-over, whose source value is read into a local because the target's
   `and` at 0x800ACD24 reuses the register the test loaded. */
void func_800ACBE4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;
    s32 f;

    switch (p[0x9B])
    {
    case 0:
        n = (UnkNode *)(p - 0x20);
        func_80044A50(n);
        *(u16 *)(c + 0x12) -= 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        func_8004491C(n, (u32)func_800C6AEC);
        c[0xF] = 0;
        p[0x9B] += 1;
        break;
    case 1:
        if (c[0xF] < 8)
        {
            c[0xF] += 1;
            c[0xC] = c[0xD] = c[0xE] += -c[0xE] / c[0xF];
        }
        else
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            f = *(s32 *)(d + 0x14);
            if (f & 0x400000)
            {
                *(s32 *)(d + 0x14) = f & ~0x400000;
                *(s32 *)(d + 0x1C) |= 0x400000;
            }
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* State 0x13. The four byte stores come out 0xF, 0xE, 0xD, 0xC because they
   are ONE CHAINED ASSIGNMENT, which emits right to left. 48 words,
   0 differing. */
void func_800ACD74(u8 *p, void *b, u8 *r, u8 *d)
{
    UnkNode *n;
    u8 *g;

    p[0x9A] = 0x13;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(r + 0x14) &= ~0x800;
    *(s16 *)(p + 0x96) = 0;
    n = (UnkNode *)(p - 0x20);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    func_80044A50(n);
    func_8004491C(n, (u32)func_800C6AEC);
    r[0xC] = r[0xD] = r[0xE] = r[0xF] = 0;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x12) -= 0x80;
    *(s32 *)(d + 0x1C) |= 0x10000000;
}

/* The fade handler behind func_800B3254 and func_800DA798. Case 0 eases the
   three +0xC..+0xE bytes towards 0x80 by (0x80 - v) / ++r[0xF]; the divisor is
   re-read FIVE times because each `sb` through the `u8 *` wipes cse's memory
   table and the next `lbu` is a zero-extend the SImode store cannot be
   forwarded into. Case 1 unlinks the node and returns 1.

   FOUR PARAMETERS, FORWARDED: func_800B3254 and func_800DA798 called this
   with NO arguments while it was a stub, exactly as their func_800AC480
   siblings did before that function was read. Both are widened in the same
   change and both stay 14 words / 0 differing. 85 words, 0 differing on the
   first compile. */
s32 func_800ACE34(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;

    switch (p[0x9B])
    {
    case 0:
        c[0xF] += 1;
        c[0xC] += (0x80 - c[0xC]) / c[0xF];
        c[0xD] += (0x80 - c[0xD]) / c[0xF];
        c[0xE] += (0x80 - c[0xE]) / c[0xF];
        if (c[0xF] >= 8)
        {
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            p[0x9B] += 1;
        }
        break;
    case 1:
        if (--c[0xF] == 0)
        {
            n = (UnkNode *)(p - 0x20);
            func_80044A50(n);
            *(u16 *)(c + 0x12) += 0x80;
            *(u16 *)(c + 0x14) &= 0xFFF3;
            func_8004491C(n, (u32)D_80045340);
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            return 1;
        }
        break;
    }
    return 0;
}

/* 0x80013714 IS A CONSTANT ADDRESS HELD IN A POINTER LOCAL, and here the
   local is what produces the shared `lui $s1` both tests read through. As
   structs.h's SYMBOL the prologue schedules differently -- `sw $ra` lands in
   the first branch's delay slot and the parameter copy fills the `lhu`'s load
   delay, so the target's `nop` is gone and the function is 51 words against
   52. Spelled `*(u16 *)0x80013714` at each site it is 53.
   52 words, 0 differing. */
void func_800ACF88(u8 *p)
{
    u8 *f;
    u8 *g;
    void *m;
    void *h;

    f = (u8 *)0x80010000;
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        D_800DCF4F = 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    m = func_800990FC();
    h = func_80099734(p, m);
    h = func_80099194(D_800E0C78, h);
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        h = (void *)func_8009929C(0x11, h);
        h = (void *)func_8009929C(0x4C, h);
        h = (void *)func_80099254(D_800E0458, h);
    }
    func_80099290(h);
    func_800A5720(m);
}

/* The four-state fade-and-retire machine the actor's +0x9B drives, and the
   widest member of the func_800D1A80 / func_800D34CC / func_800AB538 family:
   state 0 arms it unless D_8008346A says a transition is already running,
   state 1 sets the renderable's +0x10/+0x12/+0x14, raises the record's
   0x10000000, rings 0x806 for kinds 5 and 0x1E and 0x805 otherwise and arms
   the sixteen-step ramp, state 2 walks the three colour bytes towards 0x20
   and -- on the last step or when the renderable raises 0x8000 -- releases
   the engine's +0x10 slot and retires the record down one of two tails, and
   state 3 glides the mover towards D_80083780, shrinks the renderable's two
   scales and retires it the same way. 286 words, 0 differing.

   THREE SPELLINGS ARE LOAD-BEARING, each measured on this function:

   (1) THE RETIRE TAIL IS WRITTEN OUT IN BOTH ARMS, not reached by `break`
   from a shared one, and that is 52 of the 67 differing words. Cross-jumping
   runs in jump2, AFTER global-alloc, so the duplicated tail is still two
   copies when `allocno_compare` prices the four parameters -- it is what puts
   `d`'s allocno ahead of `p`'s, and the two swap $s1 and $s2 without it.
   With ONE copy in the source the function is still 286 words (the arm
   simply branches to it), which is why the error reads as pure register
   noise.

   (2) THE TWO D_80083460 POINTERS ARE TWO LOCALS, one per case -- the g/g2
   split func_800AB538 records for D_80083780. Shared, the pointer lives in
   two basic blocks, global-alloc owns it, and both blocks come out with the
   pointer and the value in each other's registers: 9 differing.

   (3) THE FIRST D_80083780 READ GOES THROUGH THE SYMBOL AND THE POINTER
   LOCAL IS ASSIGNED AFTER IT. cse unifies the two `lui`s either way, so the
   bytes are the same instructions; but with `h = D_80083780;` ahead of the
   first group the `addiu $s0, $s0, %lo` starts above it and sched2 leaves it
   one slot BELOW the second `jal` instead of one slot above -- 6 differing
   words, every one of them a shift.

   The three `/ *(s16 *)(p + 0x96)` divisions are written out, not cached in
   a local: the target reloads +0x96 for each one.

   K&R AND `s32`, both forced: the landed func_800D9F7C below calls this with
   NO arguments, so a prototyped four-parameter definition is "too few
   arguments"; and every exit sets $v0, which is a `return 0`/`return 1` and
   not a `void` tail. func_800D9F7C still measures 8 words, 0 differing. */
s32 func_800AD058(p, b, c, d)
u8 *p;
u8 *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    u8 *h;

    switch (p[0x9B])
    {
    case 0:
        if (D_8008346A != 0)
        {
            return 0;
        }
        p[0x9B] = 1;
        /* fall through */
    case 1:
        *(s16 *)(c + 0x10) = 0x20;
        *(s16 *)(c + 0x12) -= 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u32 *)(d + 0x1C) |= 0x10000000;
        func_800A56E0((d[0x13] == 5 || d[0x13] == 0x1E) ? 0x806 : 0x805);
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B]++;
        /* fall through */
    case 2:
        c[0xC] += (0x20 - c[0xC]) / *(s16 *)(p + 0x96);
        c[0xD] += (0x20 - c[0xD]) / *(s16 *)(p + 0x96);
        c[0xE] += (0x20 - c[0xE]) / *(s16 *)(p + 0x96);
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return 0;
        }
        if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
        }
        if ((*(s32 *)(d + 0x14) & 0x4000) == 0)
        {
            func_800A2FE0(d);
            func_800A32A4(d);
            if (d[0x49] != 0 && (d[0x4B] & 0x20) == 0)
            {
                func_800B8228(*(s16 *)(b + 0x2), *(s16 *)(b + 0x6), *(s16 *)(d + 0x88), d + 0x48);
            }
            if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
            {
                func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            }
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return 1;
        }
        if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
        {
            func_800ACF88(d);
        }
        func_800A2FE0(d);
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_800A56E0(0x609);
        return 1;
    case 3:
        *(s32 *)(b + 0x0) +=
            ((*(s32 *)D_80083780 + ((*(s16 *)(p + 0x96) * func_80064584(c[0x27] << 7)) << 5)) - *(s32 *)(b + 0x0)) >> 2;
        h = D_80083780;
        *(s32 *)(b + 0x4) +=
            ((*(s32 *)(h + 0x4) + ((*(s16 *)(p + 0x96) * func_800644B8(c[0x27] << 7)) << 5)) - *(s32 *)(b + 0x4)) >> 2;
        *(s16 *)(d + 0x88) +=
            ((*(s16 *)(h + 0xA) - (func_800644B8(*(s16 *)(p + 0x96) << 3) >> 6)) - *(s16 *)(d + 0x88)) >> 4;
        *(s16 *)(c + 0x1C) -= *(u16 *)(c + 0x1C) / *(s16 *)(p + 0x96);
        *(s16 *)(c + 0x1E) -= *(u16 *)(c + 0x1E) / *(s16 *)(p + 0x96);
        c[0x27]++;
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0)
        {
            return 0;
        }
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0xA) -= 1;
        func_800A2FE0(d);
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

/* 38 words, 0 differing. */
void func_800AD4D0(u8 *p)
{
    s32 v;

    if (*(s16 *)(p + 0x64) < 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x1D) != 0)
        {
            *(s16 *)(p + 0x64) = 0;
        }
    }
    v = p[0x28] + *(s16 *)(p + 0x64);
    if (v < 0)
    {
        v = 0;
    }
    else if (v >= 0x100)
    {
        v = 0xFF;
    }
    p[0x28] = v;
    *(s16 *)(p + 0x64) = 0;
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

/* Clamp the +0x64 cursor so cursor + step never passes the limit at +0x29.

   THE THREE LOADS ARE LOCALS. As one condition the two bytes are re-read
   inside the arm -- the `slt` kills the register holding +0x29 before cse
   can reuse it -- and the function comes out 16 words against 11.
   11 words, 0 differing. */
/* NOT `s16`: the return is already a `lh`, so the width costs this function
   nothing -- but an s16 return makes every caller sign-extend $v0 before
   testing it, which is a word func_800BD1C4 does not have (167 against 166).
   That also says the two were in different translation units: the caller saw
   no prototype at all. */
s32 func_800AD568(u8 *p)
{
    s32 v;
    s32 a;
    s32 b;

    v = *(s16 *)(p + 0x64);
    a = p[0x28];
    b = p[0x29];
    if (v + a > b)
    {
        *(s16 *)(p + 0x64) = b - a;
    }
    return *(s16 *)(p + 0x64);
}

/* Apply `dmg` to the actor's +0x24 HP word: halve it behind the +0x54 flag,
   double it behind probe 0xF, clamp, then fold the high byte against +0x66
   and run the death path.

   D_80012090 IS THE CONSTANT ADDRESS READ INTO A LOCAL, and that single
   spelling is the whole landing. As the declared symbol it is a two-insn
   `high` + `lo_sum`, so the `lui` goes to its own register, reorg steals it
   into the preceding `bnez`'s delay slot and the comparison's
   `addiu $v0, $zero, 1` covers the load delay the target leaves as part of
   its macro: 90 words / 13 differing with `p[0x25] = D_80012090` (which also
   re-reads the global as an `lbu`), 89 / 38 once a local kills that re-read.
   `*(s32 *)0x80012090` used at BOTH sites is 91 / 54 -- two uses build the
   address in a register with `lui`/`ori`. Read once into `s32 k` it is the
   one tied `lui $a0` / `lw $a0, %lo($a0)` macro the target has, and `sb $a0`
   is that same register. 90 words, 0 differing. The candidate therefore
   emits no relocation for those two words, as every other D_80012090 site in
   this unit does. */
void func_800AD594(u8 *p, s32 dmg)
{
    u8 *g;
    s32 hp;
    s32 hi;
    s32 k;

    if (*(s8 *)(p + 0x13) > 0)
    {
        hp = *(u16 *)(p + 0x24);
        hi = p[0x25];
        if ((s16)func_80042900((s8 *)p, 0x20) != 0)
        {
            dmg = 0;
        }
        else if (*(s32 *)(p + 0x54) & 0x100)
        {
            dmg = (dmg + 1) >> 1;
        }
        if ((s16)func_80042900((s8 *)p, 0xF) != 0)
        {
            dmg = dmg << 1;
        }
        hp -= dmg;
        if (hp < 0)
        {
            hp = 0;
        }
        else if (hp > 0x10000)
        {
            hp = 0xFFFF;
        }
        *(s16 *)(p + 0x24) = hp;
        if (p[0x66] < p[0x25])
        {
            p[0x25] = p[0x66];
            p[0x24] = 0xFF;
        }
        if (p[0x25] == 0)
        {
            k = *(s32 *)0x80012090;
            if (k == 1 && (p[0x13] == 2 || p[0x13] == 0x39))
            {
                p[0x25] = k;
            }
            else
            {
                if (*(s32 *)(p + 0x1C) & 0x80000)
                {
                    func_80094E34();
                }
                if (hi != 0)
                {
                    g = (u8 *)&D_80083460;
                    *(u16 *)(g + 0xA) += 1;
                }
            }
        }
    }
}

/* 174/174 words, 0 differing.  Builds one of four message strings for a
   record, adds a score to its +0x24 counter and clamps it.
   Parked at 31 aligned differences; the whole residue was one $s2/$s4
   exchange between `mode' and the third argument, plus six rows of delay
   slots around the double condition, and TWO SPELLING FACTS closed both.

   (1) THE THIRD ARGUMENT IS USED THROUGH A CAST AT EVERY USE AND IS NEVER
   COPIED TO A LOCAL.  A `p = (u8 *)arg;' at the top -- which reads far
   better -- costs 25 of the 31 rows, and the mechanism is a REGISTER NOTE,
   not the copy instruction (combine deletes that).  assign_parms
   (function.c:4014) puts a REG_EQUIV on each incoming parameter's copy
   insn, and update_equiv_regs (local-alloc.c:1094) DOUBLES the
   REG_LIVE_LENGTH of any pseudo that has one and is set exactly once.
   When a local copy exists, cse rewrites the parameter pseudo away and the
   surviving insn's note is a REG_EQUAL, which update_equiv_regs ignores:
   the argument prices at 10 refs / 89 insns = 3370 instead of 10 / 178 =
   1685, and sorts ABOVE `c' (16 / 240 = 2666) in allocno_compare instead of
   below it.  Measured: with the copy, `arg' got $s2 and `mode' $s4; without
   it, the order is c > mode > arg and the two had merely swapped (39 rows).

   (2) THE DOUBLE CONDITION IS TWO `else if' ARMS WITH THEIR OWN `mode = 2;'
   -- not `if (a && b || c && d) mode = 2;'.  That is worth both of the
   remaining levers at once, which is why neither was findable alone:
     * the second `mode = 2' is an 11th reference to `mode', taking it from
       10 / 116 = 2586 to 11 / 118 = 2796, just past `c' at 16 / 244 = 2622.
       Nothing else cheap reaches that: `mode++; mode--;' (14 refs) and a
       matched-operand asm (12 refs) both overshoot but still measure 10.
     * each arm materialises `addiu $v0, 0xC' for itself, so jump2's
       cross-jump merges the arms only from the `bne' onward and dbr has a
       constant to steal into BOTH delay slots (0x800AD7B0 and 0x800AD7C0),
       leaving the shared block's head as the `lbu's load-delay nop.
   The single-condition form needs a device to stop -fcse-skip-blocks
   commoning the two `p[1]' reads, and the best one found --
   `*(volatile u8 *)(arg + 1) == 0xC' on the second test -- puts the re-read
   at the TOP of the join block, where nothing can move across it: 174 words
   but 10 rows, and 31 before fact (1).  Two arms need no volatile at all.

   Measured and rejected on the final body: the condition as one `if' with
   the volatile re-read (10), with `mode++; mode--;' instead of the second
   assignment (10), the arms duplicated down to two full `p[1] == 0xC &&
   p[0] == 6' tests (171-182 words, 8-28 rows over all 16 volatile
   placements), the third parameter redeclared `u8 *' (0 differing as well,
   but it needs 53 call sites in two other units changed, so the cast is
   preferred).  `((u8 *)arg)[1]' and `*(u8 *)(arg + 1)' are both 0.  */
s32 func_800AD6FC(u8 *c, s32 mode, s32 arg)
{
    u8 *m;
    u8 *h;
    s32 t;
    s32 t0;

    m = func_800990FC();
    h = m;
    if ((s16)func_80042900((s8 *)c, 0x19) != 0)
    {
        h = func_80099194(D_800E0CAE, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        func_80099290(h);
        func_800A5720(m);
        return 0;
    }
    if (mode == 3 && arg != 0)
    {
        if (((u8 *)arg)[1] == 2 && ((u8 *)arg)[0] == 1)
        {
            mode = 2;
        }
        else if (((u8 *)arg)[1] == 0xC && ((u8 *)arg)[0] == 6)
        {
            mode = 2;
        }
    }
    t = *(u16 *)(c + 0x24);
    t0 = t;
    switch (mode)
    {
    case 0:
        h = func_80099194(D_800E0CDC, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        break;
    case 1:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0CF3, h);
        t += 0x300;
        break;
    case 2:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D1B, h);
        if (arg != 0 && ((u8 *)arg)[1] == 0xC && ((u8 *)arg)[0] == 6)
        {
            t += 0xA00;
        }
        else
        {
            t += 0x3200;
        }
        break;
    case 3:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D36, h);
        t += 0x6400;
        break;
    }
    if (arg != 0 || mode == 0)
    {
        func_80099290(h);
        func_800A5720(m);
    }
    if (t0 != t)
    {
        func_800A56E0(0x51E);
    }
    if (t > 0x10000)
    {
        t = 0xFFFF;
    }
    *(u16 *)(c + 0x24) = t;
    if (c[0x25] != 0)
    {
        *(s32 *)(c + 0x1C) &= ~0x8;
    }
    if (c[0x66] < c[0x25])
    {
        c[0x25] = c[0x66];
        c[0x24] = 0xFF;
    }
    return mode;
}

/* Can the actor step onto the tile the view names? THE func_800B500C RESULT
   IS `s16`: through an s32 local the `sll 16`/`sra 16` pair in front of the
   `bltz` disappears, 75 words / 41 differing. func_800AAA28's signature was
   narrowed to two parameters on this function's evidence -- see its comment.
   76 words, 0 differing. */
s32 func_800AD9B4(u8 *p, u8 *q)
{
    u8 *g;
    u8 *h;
    s16 i;
    s32 d;

    if (*(u16 *)(q - 0x2) & 0x8000)
    {
        return 1;
    }
    g = D_80082E80;
    if (*(s8 *)(g + 0x26) != *(s8 *)(p + 0x26) && func_8009FD40(g, p) >= 7)
    {
        return 1;
    }
    if (q[0x25] == 0)
    {
        return 1;
    }
    i = func_800B500C(p[0x24], p[0x25], *(s16 *)(q + 0x88));
    if (i < 0)
    {
        return 1;
    }
    if ((D_800E3648_s[i].unk_03 & 0x80) == 0)
    {
        return 1;
    }
    h = (u8 *)&D_80083460;
    if (*(u16 *)(h + 0x2) & 0x1000)
    {
        d = *(s8 *)(q + 0x71);
        if (d > 0)
        {
            *(u16 *)(h + 0x8) -= d - *(u16 *)(q + 0x8A);
            q[0x71] = 0;
        }
    }
    func_800A9A0C(q);
    func_800AAA28(p, q);
    return 0;
}

/* func_80095464's probe again, carried past the two `return 1` gates into the
   +0x03 clear of func_800954E0 and a fourth call. The return is s16: the two
   constants need no extension, and the `sll`/`sra` pair on the tail is where
   func_800B627C's s32 result is narrowed. 42 words, 0 differing. */
s16 func_800ADAE4(u8 *a, u8 *b)
{
    s16 i;
    u8 f;
    s32 r;

    i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
    if (i < 0)
    {
        return 1;
    }
    f = D_800E3648_s[i].unk_03;
    if ((f & 0x80) == 0)
    {
        return 1;
    }
    if ((f & 0x40) == 0)
    {
        D_800E3648_s[i].unk_03 = f & 0x7F;
    }
    r = func_800B627C(*(u8 *)&D_800E3648_s[i], b, i, 0);
    if (r == 0)
    {
        return 0;
    }
    return r;
}

/* 16 words, 0 differing. */
void func_800ADB8C(u8 *p)
{
    if (**(u8 **)p != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node walk again (see func_800BC054), with the record's +0x8 parked in
   the scratchpad each turn and the visitor skipped when the slot's +0x14
   carries 0x80.

   TWO SPELLINGS ARE LOAD-BEARING. The scratchpad address needs a POINTER
   LOCAL: written as `*(s32 *)0x1F80013C` it is the assembler's absolute
   macro, a `lui $at`/`sw` pair that cannot be hoisted out of the loop, and
   the function is 28 words. And the slot must be a `u8 *`, not an
   `UnkSlot *`: through the struct pointer the +0x14 read is MEM_IN_STRUCT_P,
   sched1 hoists it over the scratchpad store and the three load-delay `nop`s
   the target has disappear -- 29 words at 26 differing.
   32 words, 0 differing. */
s32 func_800ADBCC(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = *(s16 *)(p + 0x8);
        if (!(*(u16 *)(s + 0x14) & 0x80))
        {
            func_800453E0(p, v, s, *(s16 *)(s + 0x6));
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (u8 *)n->unk_0C;
    }
}

/* The 0x12 spawner. No caller anywhere in the project, so the arity is read
   off the four argument registers alone. 53 words, 0 differing. */
UnkNode *func_800ADC4C(u8 *a, s32 b, s16 c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800ADB8C;
        func_8004491C(n, (u32)func_800ADBCC);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(a + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(a + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(a + 0xA);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x8) = b;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)((u8 *)n + 0x28) = c;
        *(u8 **)((u8 *)n + 0x20) = d;
        *d = 0;
    }
    return n;
}

/* Set bit 0x8000 of +0x14 once, then park bit 0x100 of +0x98 on whether a
   fresh random number divides by n. The draw goes into an `s32` and
   func_800A6D30 is declared u16, so the `andi $v0, 0xFFFF` is the widening at
   the assignment. 32 words, 0 differing. */
void func_800ADD20(u8 *p, s32 n)
{
    s32 r;

    if ((*(s32 *)(p + 0x14) & 0x8000) == 0)
    {
        *(s32 *)(p + 0x14) |= 0x8000;
        r = func_800A6D30();
        if (r % n != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFEFF;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0x100;
        }
    }
}

/* Two narrow-type facts, each worth one word. (1) The result local is `s32`
   with `(s16)` casts at its uses, not an `s16` local: the target's first
   `sll $v0, $s1, 16` reads the CALLEE-SAVED copy, and an s16 local lets cse
   substitute the return register back into the extension. (2) func_8009A180's
   result is tested as a HALFWORD -- without the cast the test is a bare
   `beqz` and the function is 52 words / 21 differing. 53 words,
   0 differing. */
s16 func_800ADDA0(void *a, void *b, u8 *c, s16 d, s16 e, s32 f)
{
    u8 *g;
    s32 r;

    r = func_800ADE74(a, b, c, d, e, f);
    if ((s16)r != 0 && (s16)r != 2 && (s16)r != 3)
    {
        g = (u8 *)&D_80083460;
        if (*(s16 *)(g + 0x8) != 0 || (*(u16 *)(g + 0x2) & 0x2808) != 0)
        {
            if ((s16)func_8009A180(c, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                *(u16 *)(c + 0x46) &= 0x7FFF;
                return -1;
            }
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800A4B88", D_80089074);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800A4B88", D_80089080);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800A4B88", D_80089084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800A4B88", func_800ADE74);

/* The four-state step of the 0x800AFAD8 object. The `% 7168` tail is written
   in BOTH arms and jump.c cross-jumps it, which is the single `mfhi`/`subu`
   run the two cases share. 73 words, 0 differing. */
void func_800AFAD8(u8 *p)
{
    s16 v;

    switch (*(s16 *)(p + 0x2))
    {
    case 0:
        *(u16 *)(p + 0x6) += 2;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 1:
        *(u16 *)(p + 0x6) += 4;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 2:
        v = *(u16 *)(p + 0x8) + 0x40;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    case 3:
        *(u16 *)(p + 0x6) += 8;
        v = (func_800644B8(*(s16 *)(p + 0x0) << 5) >> 1) + 0xC00;
        *(u16 *)(p + 0x0) += 1;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800A4B88", func_800AFBFC);

/* The vertical strip of gouraud-textured quads one ring segment is drawn as:
   fourteen RotAverageNclip4 passes up one 0xE00-per-step column of the
   scratchpad quad, each visible pass emitting TWO POLY_GT4s split across the
   mid-edge, and returning the advanced primitive cursor (or NULL when nothing
   was emitted). func_800AFBFC is the only caller and passes 0 or 1 as the
   fifth argument.

   THIS IS csrc/ovl_town.c's func_800A41D8 WITH POLY_GT4 FOR POLY_FT4. Every
   load-bearing spelling that function's comment records transfers verbatim --
   the bitwise `|` of four `&&` pairs with the `(u16)` casts, the `>> 1`
   midpoints, the chained colour assignment running b/g/r with p[0] before
   p[1], the two divides by twelve, `sp->unk_0C` first in the +0x14 sum, the
   `u0`/`u1` word pair with `u2`/`u3` as halves and the four decrements after,
   and `s16 flag` arriving in a stack slot. The differences are that this one
   has no `p->code |= c->unk_0F & 2;`, no DR_MODE pass and so no second
   AddPrim loop, and that the two colour values come out of a depth the
   scratchpad carries instead of the caller's +0x16 field. 356 words,
   0 differing; seqcheck `seq=OK`, `arith=OK`, 4 relocated words.

   THE SECOND COLOUR VALUE IS THE SAME VARIABLE, `d = d - 0x10;`, NOT A
   SECOND LOCAL. With a separate `s16 w = d - 0x10;` the function is still
   356 words but 8 differing, and every one of the eight is a register
   number: a three-way rotation in which the SImode temp holding
   `0xE0 - (...)`, its sign-extension and `d` itself take $v1/$a1/$a2 where
   the target takes $a1/$a2/$v1. The cause is global.c's `allocno_compare`,
   read out of the .greg/.lreg dumps rather than swept for: with a separate
   `w`, `d` is pseudo 91 with 4 refs across 9 insns, priority
   floor_log2(4)*4/9*10000 = 8888, and it sorts SEVENTEENTH in
   `allocno_order` -- behind the temp (8 refs / 7 insns, 34285), which
   therefore takes $v1 first. Reusing `d` deletes the third pseudo and the
   order comes out right. Declaration order of the locals is NOT the lever
   here: d/w/v, v/w/d and `w = d; w -= 0x10;` all measure the same 8.

   `lo` IS `s16`, NOT `u16`. As `u16` the function is 355 words / 181
   differing: the one missing word is `addu $v0, $t0, $zero`, the copy that
   stands between the spilled flag's `lhu` and its `beqz`, and the shift it
   causes drags the whole `d` block with it. `s32` is 355 / 184, `u8` 355 /
   182, `volatile u16` 354 / 200.

   THE CALLEES ARE SPELLED `RotNclip4` / `SetPolyGT4` / `AddPrim`.  They are
   declared by psyq/include and resolve through splat/common_syms.txt, which
   build/dungeon.bin did NOT link until 2026-11-15 -- the Makefile passed that
   script for town.bin and main.bin only, so the library names compiled,
   measured 356/356 at 0 differing, and then failed the link.  The dungeon
   rule passes it now and all four containers stay byte-identical. */
POLY_GT4 *func_800AFFB4(u8 *a, void *c, UnkDungeonSP5 *sp, POLY_GT4 *p, s16 flag)
{
    s32 i;
    s32 j;
    s32 n;
    u16 z;
    s16 lo;
    s16 d;
    s16 v;
    UnkDungeonAFFB4T *t;

    n = 0;
    lo = *(s16 *)(a + 0x8) >= 0xE00;
    z = *(u16 *)(a + 0x8);
    sp->unk_70[0].vz = sp->unk_70[1].vz = z - 0x6200;
    sp->unk_70[2].vz = sp->unk_70[3].vz = z - 0x5400;
    for (i = 0; i < 0xE; i++)
    {
        sp->unk_70[0].vz += 0xE00;
        sp->unk_70[1].vz += 0xE00;
        sp->unk_70[2].vz += 0xE00;
        sp->unk_70[3].vz += 0xE00;
        if (RotNclip4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                      (s32 *)&sp->unk_F0[0], (s32 *)&sp->unk_F0[1], (s32 *)&sp->unk_F0[2],
                      (s32 *)&sp->unk_F0[3], (s32 *)&sp->unk_90, (s32 *)&sp->unk_C0,
                      (s32 *)&sp->unk_94) > 0)
        {
            if (((u16)(sp->unk_F0[0].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[0].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[1].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[1].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[2].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[2].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[3].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[3].vy + 0x20) < 0x121))
            {
                *(u32 *)&p[0].x0 = *(u32 *)&sp->unk_F0[0];
                *(u32 *)&p[0].x2 = *(u32 *)&sp->unk_F0[2];
                *(u32 *)&p[1].x1 = *(u32 *)&sp->unk_F0[1];
                *(u32 *)&p[1].x3 = *(u32 *)&sp->unk_F0[3];
                p[0].x1 = p[1].x0 = (sp->unk_F0[0].vx + sp->unk_F0[1].vx) >> 1;
                p[0].x3 = p[1].x2 = (sp->unk_F0[2].vx + sp->unk_F0[3].vx) >> 1;
                p[0].y1 = p[1].y0 = (sp->unk_F0[0].vy + sp->unk_F0[1].vy) >> 1;
                p[0].y3 = p[1].y2 = (sp->unk_F0[2].vy + sp->unk_F0[3].vy) >> 1;
                sp->unk_90 = sp->unk_70[0].vz % 0xE00;
                sp->unk_90 = sp->unk_90 / 0x100;
                if (lo)
                {
                    sp->unk_90 += 0x10;
                }
                d = 0xE0 - ((u16)sp->unk_90 + i * 16);
                v = 0x80;
                if (d <= 0x80)
                {
                    v = d;
                    if (d < 0)
                    {
                        v = 0;
                    }
                }
                p[1].r0 = p[1].r1 = p[0].r1 = p[0].r0 = p[1].g0 = p[1].g1 = p[0].g1 = p[0].g0 =
                    p[1].b0 = p[1].b1 = p[0].b1 = p[0].b0 = v;
                d = d - 0x10;
                if (d > 0x80)
                {
                    d = 0x80;
                }
                else if (d < 0)
                {
                    d = 0;
                }
                p[1].r2 = p[1].r3 = p[0].r3 = p[0].r2 = p[1].g2 = p[1].g3 = p[0].g3 = p[0].g2 =
                    p[1].b2 = p[1].b3 = p[0].b3 = p[0].b2 = d;
                t = &((UnkDungeonAFFB4T *)sp->unk_118)[(i & 1) * 2 + flag * 4];
                for (j = 0; j < 2; j++)
                {
                    SetPolyGT4(p);
                    sp->unk_08 = t->unk_08;
                    sp->unk_0C = t->unk_09;
                    sp->unk_10 = t->unk_0A;
                    sp->unk_14 = t->unk_0B;
                    sp->unk_10 = sp->unk_10 + sp->unk_08;
                    sp->unk_14 = (sp->unk_0C + sp->unk_14) * 0x100;
                    sp->unk_0C = sp->unk_0C * 0x100;
                    *(u32 *)&p->u0 = (sp->unk_0C + sp->unk_08) | (t->unk_06 << 16);
                    *(u32 *)&p->u1 = (sp->unk_0C + sp->unk_10) | (t->unk_04 << 16);
                    *(u16 *)&p->u2 = sp->unk_14 + sp->unk_08;
                    *(u16 *)&p->u3 = sp->unk_14 + sp->unk_10;
                    p->u1--;
                    p->u3--;
                    p->v2--;
                    p->v3--;
                    AddPrim((u32 *)sp->unk_20 + (t - (UnkDungeonAFFB4T *)sp->unk_118) /
                                                    sizeof(UnkDungeonAFFB4T),
                            p);
                    p++;
                    t++;
                }
                n++;
            }
        }
    }
    if (n == 0)
    {
        return NULL;
    }
    return p;
}

/* Open the kind-0x12 panel for corner `k`. THE RENDERABLE POINTER IS RE-READ
   PER GROUP, and the source says so by assigning the local three times: the
   +0x8 store, the three colour bytes and the two scales are each one group
   off their own `lw 0xC($s0)`. Written inline per field every store reloads
   and the function is 74 words / 46 differing; with ONE assignment it would
   be one load for all six. The three colour bytes go 0xE, 0xD, 0xC and the
   two scales 0x1E, 0x1C -- descending in both groups. The mover at +0x08 IS
   written inline, which is where its three reloads come from. 68 words,
   0 differing. */
void func_800B0544(s16 k)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *w;
    s32 v;

    if (D_800DCF4C != k)
    {
        func_8003E1C4(k);
        D_800DCF4C = k;
    }
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800AFAD8;
        func_8004491C(n, (u32)func_800AFBFC);
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DEF90;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_0E = 0xE0;
        s->unk_0D = 0xE0;
        s->unk_0C = 0xE0;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = 0x4000;
        v = *(s32 *)0x80012090;
        w = (u8 *)n + 0x20;
        if (v == 0)
        {
            if (D_8008146C == 0x28)
            {
                *(s16 *)(w + 0x2) = 1;
            }
        }
        else if (v == 2)
        {
            *(s16 *)(w + 0x2) = v;
        }
    }
}

/* Project the record's (x, y, z) through RotTransPers out of the third
   argument's own scratch at +0x28, and bias the returned depth by the second
   argument's signed +0x4 byte. 27 words, 0 differing. */
void func_800B0654(u8 *a, u8 *b, u8 *c)
{
    *(s16 *)(c + 0x28) = *(u16 *)(a + 0x2);
    *(s16 *)(c + 0x2A) = *(u16 *)(a + 0x6);
    *(s16 *)(c + 0x2C) = *(u16 *)(a + 0xA);
    *(s32 *)(c + 0xC0) = RotTransPers((SVECTOR *)(c + 0x28), (s32 *)(c + 0xB8), (s32 *)(c + 0x90),
                                      (s32 *)(c + 0x94)) +
                         *(s8 *)(b + 0x4);
}

/* 12 words, 0 differing. */
void func_800B06C0(u8 *v, u8 *q, u8 *p)
{
    *(s16 *)(p + 0xB8) = *(u16 *)(v + 0x2);
    *(s16 *)(p + 0xBA) = *(u16 *)(v + 0x6);
    *(s32 *)(p + 0xC0) = *(s16 *)(v + 0xA) + *(s8 *)(q + 0x4);
}

s32 func_800B06F0(u8 *p, UnkVec *v, UnkSlot *s)
{
    MATRIX *m = (MATRIX *)0x1F8000D0;
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPC *sp = (UnkDungeonSPC *)0x1F800000;
    UnkNode *n;
    u8 ***e;
    u32 *col;
    POLY_FT4 *q;
    volatile u8 *r;
    s32 ta;
    s32 tb;
    s32 tc;
    s32 td;
    s32 u;

    sp->unk_EC = *(s32 *)((u8 *)g + 0xA0);
    q = *(POLY_FT4 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = (u32)(D_80083160.unk_00 + 0xB0);
    for (;;)
    {
        D_800DEFF0[*(u16 *)(p + 0x8) & 1](v, s, sp);
        e = *(u8 ***)p;
        col = *(u32 **)(p + 0x4);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            sp->unk_C0 = sp->unk_C0 * 4 + sp->unk_20;
            sp->unk_B8 -= 0xA0;
            sp->unk_BA -= 0x78;
            PushMatrix();
            sp->unk_100.vx = s->unk_16;
            sp->unk_100.vy = s->unk_18;
            sp->unk_100.vz = s->unk_1A;
            sp->unk_E4 = s->unk_20;
            sp->unk_E8 = s->unk_22;
            sp->unk_30 = s->unk_1C;
            sp->unk_34 = s->unk_1E;
            sp->unk_38 = 0x1000;
            RotMatrix(&sp->unk_100, m);
            ScaleMatrix(m, (VECTOR *)&sp->unk_30);
            SetTransMatrix(m);
            SetRotMatrix(m);
            r = (volatile u8 *)*e;
            if (r != NULL)
            {
                do
                {
                again:
                {
                    sp->unk_70[3].vz = 0;
                    sp->unk_70[2].vz = 0;
                    sp->unk_70[1].vz = 0;
                    sp->unk_70[0].vz = 0;
                    r = (volatile u8 *)((u32)r | 0x80000000);
                    if ((r[0x0] ^ s->unk_14) & 1)
                    {
                        ta = -(s8)r[0x2] - s->unk_20;
                        sp->unk_70[2].vx = ta;
                        sp->unk_70[0].vx = ta;
                        ta = ta - (*(u8 *)(r + 0xA));
                        sp->unk_70[3].vx = ta;
                        sp->unk_70[1].vx = ta;
                    }
                    else
                    {
                        tb = (s8)r[0x2] - s->unk_20;
                        sp->unk_70[2].vx = tb;
                        sp->unk_70[0].vx = tb;
                        tb += (*(u8 *)(r + 0xA));
                        sp->unk_70[3].vx = tb;
                        sp->unk_70[1].vx = tb;
                    }
                    if ((r[0x0] ^ s->unk_14) & 2)
                    {
                        tc = -(s8)r[0x3] - s->unk_22;
                        sp->unk_70[1].vy = tc;
                        sp->unk_70[0].vy = tc;
                        tc = tc - (*(u8 *)(r + 0xB));
                        sp->unk_70[3].vy = tc;
                        sp->unk_70[2].vy = tc;
                    }
                    else
                    {
                        td = (s8)r[0x3] - s->unk_22;
                        sp->unk_70[1].vy = td;
                        sp->unk_70[0].vy = td;
                        td += (*(u8 *)(r + 0xB));
                        sp->unk_70[3].vy = td;
                        sp->unk_70[2].vy = td;
                    }
                    RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                  &sp->unk_F0[0].w, &sp->unk_F0[1].w, &sp->unk_F0[2].w,
                                  &sp->unk_F0[3].w, &sp->unk_90, &sp->unk_94);
                    if (*(s8 *)r >= 0 && (*(u8 *)r & 8))
                    {
                        u = *(u8 *)r & 4;
                        if (NormalClip(sp->unk_F0[0].w, sp->unk_F0[1].w, sp->unk_F0[2].w) <= 0)
                        {
                            if (u == 0)
                            {
                                goto skip;
                            }
                        }
                        else if (u != 0)
                        {
                            goto skip;
                        }
                    }
                    goto draw;
                skip:
                    if (r[0x1] != 0x2C)
                    {
                        goto nextelem;
                    }
                    goto bump;
                draw:
                    if (*(u8 *)r & 4)
                    {
                        func_8003E12C(&sp->unk_F0[0].w, &sp->unk_F0[1].w);
                        func_8003E12C(&sp->unk_F0[2].w, &sp->unk_F0[3].w);
                    }
                    q->x0 = sp->unk_F0[0].h[0] + sp->unk_B8;
                    q->y0 = sp->unk_F0[0].h[1] + sp->unk_BA;
                    q->x1 = sp->unk_F0[1].h[0] + sp->unk_B8;
                    q->y1 = sp->unk_F0[1].h[1] + sp->unk_BA;
                    q->x2 = sp->unk_F0[2].h[0] + sp->unk_B8;
                    q->y2 = sp->unk_F0[2].h[1] + sp->unk_BA;
                    q->x3 = sp->unk_F0[3].h[0] + sp->unk_B8;
                    q->y3 = sp->unk_F0[3].h[1] + sp->unk_BA;
                    if ((r[0x1] & 0xFC) == 0x2C)
                    {
                        q = (POLY_FT4 *)func_800B0BE0((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        func_8004C010((u8 *)q - 0x24, col);
                        if (r != NULL)
                        {
                            goto again;
                        }
                    bump:
                        col++;
                    }
                    else
                    {
                        if (sp != NULL || s->unk_14 != 0)
                        {
                            q = (POLY_FT4 *)func_8004CD28((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        }
                        else
                        {
                            q = (POLY_FT4 *)func_8004CD28((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        }
                        if (r != NULL)
                        {
                            goto again;
                        }
                    }
                }
                nextelem:
                    e++;
                    r = (volatile u8 *)*e;
                } while (*(u8 *volatile *)&r != NULL);
            }
            PopMatrix();
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

/* Build one POLY_FT4 (0x28 bytes) for the 0xC-byte sprite descriptor at *c
   and hand it to AddPrim, then step the cursor or clear it when the
   descriptor's first byte is negative.

   THE CURSOR IS ADVANCED BEFORE THE TAIL `if`, not in the `return`:
   `return q + 0x28;` is 84 words / 12 differing, because the target has
   `addiu $s1, $s1, 0x28` in the `bgez`'s delay slot and then a bare
   `addu $v0, $s1, $zero`. The four UV pairs are CHAINED assignments
   (`q[0xC] = q[0x1C] = ...`), which is what gives the target's
   high-offset-then-low store order off one computed value; `*c` is re-read
   once per group because the previous group's stores invalidate it.
   85 words, 0 differing. */
u8 *func_800B0BE0(u8 *a, u8 *b, u8 **c, u8 *q)
{
    s32 t;

    *(u32 *)(q + 0x4) = *(u32 *)(b + 0xC);
    SetPolyFT4(q);
    *(s16 *)(q + 0xE) = *(u16 *)(*c + 0x6);
    t = *(u16 *)(b + 0x10);
    if (t != 0)
    {
        *(s16 *)(q + 0x16) = t + (*(u16 *)(*c + 0x4) & 0xFF9F);
    }
    else
    {
        *(s16 *)(q + 0x16) = *(u16 *)(*c + 0x4);
    }
    q[0xC] = q[0x1C] = (*c)[0x8];
    q[0x14] = q[0x24] = (*c)[0x8] + (*c)[0xA] - 1;
    q[0xD] = q[0x15] = (*c)[0x9];
    q[0x1D] = q[0x25] = (*c)[0x9] + (*c)[0xB] - 1;
    AddPrim(a, q);
    q += 0x28;
    if (*(s8 *)*c < 0)
    {
        *c = NULL;
    }
    else
    {
        *c = *c + 0xC;
    }
    return q;
}

/* The mirror/tail step on a node's +0x10: pick the reflected direction from
   the +0x12 phase, stamp two 3-byte colour cells when the actor's gauges say
   so, then run the two-state fade on the slot's +0x18 -- state 1 tears the
   node and its four children down below -0x3FF -- and finally track the
   actor's position into the mover with D_800DDC40's per-kind height.
   135 words, 0 differing.

   `m` MUST BE DECLARED `s16`. The direction is `m = k;` and then, inside the
   phase test, `m = -k & 0x1F;` and `m = 0x10;` -- three copies into one
   variable. As `s32` cse's `make_regs_eqv` merges m with k (m's last mention
   is later, so m becomes the canonical register), both copies vanish and the
   function is 133 words / 126 differing; an `if`/`else` instead is 135/100
   but lays the `else` arm out after a `j`. As `s16` the copy is
   `(set (reg:HI m) (subreg:HI k))`, `make_regs_eqv` never runs, and both
   `addu $a3` copies survive -- and `m << 3` still costs no extension because
   gcc knows the value is 0..0x1F.

   CASE 1'S EARLY ARM IS THE *NORMAL* ONE. With the teardown written first as
   `if (c18 < -0x3FF) { ...; return; }` it is laid out inline and the branch
   sense inverts -- 135 words, 7 differing.

   THE FOUR-SLOT SWEEP IS AN INDEX, NOT A CURSOR. `*(u8 **)(p + 0x28 + i * 4)`
   lets loop.c make the giv, whose initialiser does not sit ahead of the
   D_800814A0 hoist in the preheader; a `r = p; ... r += 4;` cursor is a
   source statement that does, and it is 2 differing. */
void func_800B0D34(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    u8 *t;
    u8 *q;
    s32 k;
    s16 m;
    s32 i;

    a = *(u8 **)(p + 0x4);
    t = *(u8 **)(p + 0x14);
    k = *(u16 *)(p + 0x12) & 0x1F;
    m = k;
    if ((*(u16 *)(p + 0x12) & 0x10) != 0)
    {
        m = -k & 0x1F;
        if (m == 0)
        {
            m = 0x10;
        }
    }
    a += 4;
    if (t[0x29] / (t[0x11] / 2 + 2) >= t[0x28])
    {
        a[0] = -0x80 - (m << 3);
        a[2] = 0x10;
        a[1] = 0x10;
    }
    a += 4;
    if (t[0x25] < 0xA)
    {
        a[0] = -0x80 - (m << 3);
        a[2] = 0x10;
        a[1] = 0x10;
    }
    *(s16 *)(p + 0x12) += 1;
    switch (*(s16 *)(p + 0x10))
    {
    case 0:
        if (*(u16 *)(c + 0x18) != 0)
        {
            *(s16 *)(c + 0x18) += 0x80;
        }
        if (*(u8 **)(D_800E3D7C + 0x104) != *(u8 **)(p + 0x14))
        {
            *(s16 *)(p + 0x10) += 1;
        }
        break;
    case 1:
        if (*(s16 *)(c + 0x18) >= -0x3FF)
        {
            *(s16 *)(c + 0x18) = *(u16 *)(c + 0x18) - 0x100;
            break;
        }
        func_8004E994(*(u8 **)(p + 0x5C));
        for (i = 0; i < 4; i++)
        {
            if (*(u8 **)(p + 0x28 + i * 4) != NULL)
            {
                *(u16 *)(*(u8 **)(p + 0x28 + i * 4) - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    c[0x4] = 0;
    q = *(u8 **)(t - 0x18);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(q + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(q + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0xA) - (D_800DDC40[t[0x13]] + 0x50);
}

/* The dungeon HUD panel's 0x12-kind node: two number plates from the actor's
   +0x28 and +0x25, a name string picked out of D_800DEFF8, and two bare
   elements parked in the +0x70 slot array. 233 words, 0 differing.

   THE LENGTH CALL IS DUPLICATED INTO BOTH ARMS. Written once after the join
   over a shared `name` local it is 235 words / 226 differing: `i = 4` then
   lives inside the arms, sched1 lifts its `li` above the D_800DEFF8 base's
   last use, `i` loses $s0 to that base and `c` takes an EIGHTH callee-saved
   register -- the two extra words are its save and restore. Duplicated, the
   arms end at the call, cross-jumping merges the tail, and $s0 carries the
   base and then `i` exactly as the target does.

   `c = (u8 *)n + 0x64;` BOUND BEFORE THE `if` is the 233rd word. Spelled
   `*(u32 *)((u8 *)n + 0x64) = 0x808080;` at its use the address folds into
   the store and the function is 232 words; the target computes it ahead of
   the branch and reorg takes it into the delay slot.

   `r[1] |= 2;` GOES AFTER ALL FOUR `q` COLOUR COPIES. Its lbu/ori/sb chain
   outranks the +0x10 copy and is hoisted past it wherever it sits earlier:
   before +0x4 is 236 / 67, before +0xC 234 / 64, before +0x10 233 / 11,
   between +0x10 and +0x14 233 / 8.

   Three narrower spellings, each measured: `*(s8 *)(r + 0x2) = -2` (through
   the `u8 *` the -2 is narrowed to `li 254`), `*(u16 *)` for the 0xFC00 at
   the sprite's +0x18 (`ori`, where `s16` gives `li -1024`), and the paired
   byte and `buf` stores written high offset first. */
UnkNode *func_800B0F50(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    UnkDungeonSprite *s;
    u8 *q;
    u8 *r;
    u8 *t;
    u8 *c;
    s32 w;
    s32 i;
    u8 buf[0x10];

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        m = n->unk_08;
        s->unk_1C = 0x1000;
        s->unk_1E = 0x1000;
        *(u32 *)((u8 *)s + 0xC) = 0x808080;
        *(u16 *)((u8 *)s + 0x18) = 0xFC00;
        t = *(u8 **)(p - 0x18);
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(t + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(t + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(t + 0xA) - (D_800DDC40[p[0x13]] + 0x50);
        n->unk_10 = (u32)func_800B0D34;
        func_8004491C(n, (u32)func_800B06F0);
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x14) = (u32)p;
        *(u32 *)((u8 *)n + 0x20) = (u32)((u8 *)n + 0x70);
        *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
        *(u32 *)((u8 *)n + 0x70) = (u32)D_80077FEC;
        *(u32 *)((u8 *)n + 0x58) = 0x808080;
        r = (u8 *)func_800B12F4();
        *(u32 *)(q + 0x28) = (u32)r;
        if (r != NULL)
        {
            buf[2] = 9;
            buf[0] = 9;
            buf[3] = 0x38;
            buf[1] = 0x38;
            func_8004E5A0(p[0x28], 3, &buf[4]);
            *(u32 *)((u8 *)n + 0x74) = (u32)func_8004E298(r, buf, 0);
            func_800B1320(r, 8, (s16)(-*(s8 *)(r + 0x3)));
            func_800B135C(r, 8);
            r += 0x24;
            *(u32 *)((u8 *)n + 0x5C) = 0x808080;
            func_8004E5A0(p[0x25], 3, &buf[4]);
            *(u32 *)((u8 *)n + 0x78) = (u32)func_8004E298(r, buf, 0);
            func_800B1320(r, 8, (s16)(-*(s8 *)(r + 0x3) + 8));
            func_800B135C(r, 8);
            r += 0x24;
            *(u32 *)((u8 *)n + 0x60) = 0x808080;
            c = (u8 *)n + 0x64;
            if ((*(u16 *)(p + 0x46) & 0x8000) != 0)
            {
                *(u32 *)((u8 *)n + 0x7C) =
                    (u32)func_8004DA74(r, D_800DEFF8[*(u16 *)(p + 0x46) & 0x3FFF] + 1, 0);
                i = 4;
                w = strlen(D_800DEFF8[*(u16 *)(p + 0x46) & 0x3FFF] + 1) * 4;
            }
            else
            {
                *(u32 *)((u8 *)n + 0x7C) = (u32)func_8004DA74(r, D_800DEFF8[4] + 1, 0);
                i = 4;
                w = strlen(D_800DEFF8[4] + 1) * 4;
            }
            func_800B1320(r, (s16)(((0x20 - w) >> 1) + 0x80), (s16)(-*(s8 *)(r + 0x3) + 0x11));
            func_800B135C(r, 8);
            r += 0x30;
            *(u32 *)c = 0x808080;
            *(u32 *)(q + i * 4 + 0x50) = (u32)r;
            r[0x1] = 0x38;
            *(s8 *)(r + 0x2) = -2;
            *(s8 *)(r + 0x3) = -2;
            r[0x9] = 0;
            r[0x8] = 0;
            r[0xB] = 0x24;
            r[0xA] = 0x24;
            r[0x0] = 0xC0;
            *(u32 *)(q + 0x18) = 0x202020;
            *(u32 *)(q + 0x1C) = 0x402020;
            *(u32 *)(q + 0x20) = 0x204020;
            *(u32 *)(q + 0x24) = 0x202040;
            *(u32 *)(r + 0x4) = *(u32 *)(q + 0x18);
            *(u32 *)(r + 0xC) = *(u32 *)(q + 0x1C);
            *(u32 *)(r + 0x10) = *(u32 *)(q + 0x20);
            *(u32 *)(r + 0x14) = *(u32 *)(q + 0x24);
            r[0x1] |= 2;
            i++;
            r += 0x18;
            *(u32 *)(q + i * 4 + 0x50) = (u32)r;
            r[0x1] = 0x81;
            *(s16 *)(r + 0x4) = 0x40;
            *(s16 *)(r + 0x6) = 0;
            r[0x0] = 0xC0;
        }
    }
    return n;
}

/* 11 words, 0 differing. */
u8 *func_800B12F4(void)
{
    UnkNode *n;
    u8 *r;

    n = func_8003FC64(0);
    r = NULL;
    if (n != NULL)
    {
        r = (u8 *)n + 0x20;
    }
    return r;
}

/* A POINTER WALK IS WHAT THIS TARGET WANTS, the reverse of func_800A7030's
   note: loop.c turns p[2]/p[3] into one giv biased by +3 and read at -1 and
   0, and the terminator keeps the base. 15 words, 0 differing. */
void func_800B1320(u8 *p, s16 a, s16 b)
{
    while (1)
    {
        p[0x2] += a;
        p[0x3] += b;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* `g = f; f &= 0x4;` -- the hoisted test must land in the PARAMETER's own
   register with the original copied out, which `m = f & 4;` does not do
   (19 words against 20). 20 words, 0 differing. */
void func_800B135C(u8 *p, s32 f)
{
    s32 g;

    g = f;
    f &= 0x4;
    while (1)
    {
        p[0] |= g;
        if (f)
        {
            p[2] = -p[2];
        }
        if ((s8)p[0] < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* Walk a 0xC-stride table terminated by a negative first byte, writing `v`
   into each entry's +0x06.

   THE BACKWARD `goto` IS LOAD-BEARING. A `for (;;)` with a `break` is 9
   words and 8 differing -- loop.c rotates it and emits a preheader jump --
   and tail recursion is 14, because this compiler does not eliminate it
   here. 8 words, 0 differing. */
void func_800B13AC(s8 *p, s16 v)
{
top:
    *(s16 *)(p + 0x6) = v;
    if (*p < 0)
    {
        return;
    }
    p += 0xC;
    goto top;
}

/* OR a mask into every record's +0x4 halfword until the lead byte goes
   negative. The +0x4 cursor and the record base are two induction
   variables, which is the two `addiu 0xC`s. 13 words, 0 differing. */
void func_800B13CC(u8 *p, u16 mask)
{
    while (1)
    {
        *(u16 *)(p + 4) |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* func_800B13CC on the +0x1 byte. 13 words, 0 differing. */
void func_800B1400(u8 *p, u8 mask)
{
    while (1)
    {
        p[1] |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* 20 words, 0 differing. */
u8 *func_800B1434(u8 *d, u8 *s)
{
    while (1)
    {
        *(s32 *)(d + 0x0) = *(s32 *)(s + 0x0);
        *(s32 *)(d + 0x4) = *(s32 *)(s + 0x4);
        *(s32 *)(d + 0x8) = *(s32 *)(s + 0x8);
        d += 0xC;
        if ((s8)s[0] < 0)
        {
            return d;
        }
        s += 0xC;
    }
}

/* 12 words, 0 differing. */
u8 *func_800B1484(u8 *p)
{
    while (*(s8 *)p >= 0)
    {
        p += 0xC;
    }
    return p;
}

/* 6 words, 0 differing. */
s32 func_800B14B4(void)
{
    return *(s32 *)(D_800E3D7C + 0x104) == 0;
}

/* 4 words, 0 differing. */
s32 func_800B14CC(void)
{
    return D_80081485[0] == 0;
}

/* 8 words, 0 differing. */
s32 func_800B14DC(void)
{
    return D_800E3D7C[0x9A] != 0x29;
}

/* The fade driver at a node's +0x10: run the +0x24 probe out of D_800DF030,
   arm state 2 for 4 frames when it fires, then state 0 fades the renderable's
   +0x1E and three colour bytes UP toward 0x1000/0x80 over the +0x22 counter
   and hands on to state 1, while state 2 fades them DOWN and, at zero,
   releases the +0x50 handle list, flags the four +0x28 children and itself.
   155 words, 0 differing.

   TWO LOOP SPELLINGS ARE LOAD-BEARING:
   (1) THE HANDLE LIST TESTS `> 0` FIRST. `for (q = p;; q += 4) { v = ...;
   if (v > 0) call; else if (v == 0) break; }` gives the target's
   `blez`/`jal`/`j` + `bnez` pair; a `while ((v = ...) != 0)` with the `> 0`
   test inside puts the `beqz` first and is three words long (158).
   (2) THE FOUR-SLOT SWEEP IS AN INDEX, `*(u8 **)(p + 0x28 + i * 4)`, not a
   cursor. A `w = p; ... w += 4;` cursor is a source statement and takes the
   callee-saved register the target spends on nothing -- 7 differing, and 2
   with the counter initialised first. Same rule func_800B0D34 records. */
void func_800B14FC(u8 *p, UnkVec *b, UnkSlot *c)
{
    u8 *q;
    u8 *n;
    s32 v;
    s32 i;

    if (D_800DF030[*(s16 *)(p + 0x24)](p, b, c) != 0)
    {
        if (*(s16 *)(p + 0x20) < 2)
        {
            *(s16 *)(p + 0x22) = 4;
            *(s16 *)(p + 0x20) = 2;
        }
    }
    switch (*(s16 *)(p + 0x20))
    {
    case 0:
        if (*(s16 *)(p + 0x26) != 0)
        {
            *(u16 *)(p + 0x26) -= 1;
            break;
        }
        if (*(s16 *)(p + 0x22) == 0)
        {
            break;
        }
        c->unk_1E += (0x1000 - c->unk_1E) / *(s16 *)(p + 0x22);
        c->unk_0C[0] = c->unk_0C[1] = c->unk_0C[2] += (0x80 - c->unk_0C[2]) / *(s16 *)(p + 0x22);
        if (--*(s16 *)(p + 0x22) == 0)
        {
            c->unk_1E = 0x1000;
            *(u32 *)c->unk_0C = 0x808080;
            *(s16 *)(p + 0x20) += 1;
        }
        break;
    case 1:
        break;
    case 2:
        if (*(s16 *)(p + 0x22) != 0)
        {
            c->unk_1E += (-c->unk_1E) / *(s16 *)(p + 0x22);
            c->unk_0C[0] = c->unk_0C[1] = c->unk_0C[2] += (-c->unk_0C[2]) / *(s16 *)(p + 0x22);
        }
        if (--*(s16 *)(p + 0x22) <= 0)
        {
            for (q = p;; q += 4)
            {
                v = *(s32 *)(q + 0x50);
                if (v > 0)
                {
                    func_8004E994(v);
                }
                else if (v == 0)
                {
                    break;
                }
            }
            for (i = 0; i < 4; i++)
            {
                n = *(u8 **)(p + 0x28 + i * 4);
                if (n != NULL)
                {
                    *(u16 *)(n - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                }
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The dungeon panel's three-chain cursor: a kind-0x12 node whose +0x20 view
   takes the x/y pair, the func_800B14FC step and the func_800B06F0 tag, and
   whose +0x50 array collects the func_800B12F4 chains -- `k' picks a
   D_80073618 prologue chain, `d''s low byte the D_800DF03C label, `e' the
   narrow layout and `f' the +0x26 tag.  234 words, 0 differing.

   NO DEVICES.  The park this closes carried three -- two `do { } while (0);'
   wrappers and a duplicated-arm `if (tmp)' -- and none of them survives: the
   duplicated arm COSTS 7 rows on this body (235/7) and the two wrappers are
   inert (0 rows either way).  Four spellings carry it instead.

   (1) THE SECOND CHAIN'S OLD CURSOR GOES IN THE TEMP, NOT ITS RESULT:
           tmp = u;
           u = func_800B1434(u, u - 0x60);
           func_800B1320(tmp, 1, 1);
       The park had it the other way round -- `tmp = func_800B1434(u, u -
       0x60); func_800B1320(u, 1, 1); u = tmp;' -- and that is 235 words /
       25 rows here and was the whole of its residue for two sessions.  It is
       unreachable, not merely worse.  expand_call emits the return copy
       IMMEDIATELY after the call, so with the RESULT in the temp the order is
       `(set tmp v0)' then `(set a0 u)', both priority 1; `rank_for_schedule'
       breaks that tie on INSN_LUID and schedules the HIGHER LUID first, which
       (sched runs backwards) puts the LOWER-LUID copy FIRST in the program.
       `u' is then still live when `tmp' is born, the two conflict, `tmp'
       takes a callee-saved register and `u = tmp' is the extra word the park
       could never lose.  Putting the OLD cursor in the temp makes that insn
       `(set tmp u)', ahead of the call, which sched1 sinks BELOW the call to
       the target's `addu $a0, $s1, $zero' at word 148, and the result copy is
       then a plain `(set u v0)' -- word 149 -- sharing $s1 with `u'.
       `u = func_800B1434(tmp = u, u - 0x60);' is the same insns and also 0.
   (2) THE TERNARY'S ELSE ARM IS A LOCAL ASSIGNED BEFORE THE TEST:
           arg = base + 0x80;
           func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : arg, ...);
       The target computes `base + 0x80' unconditionally and reorg fills the
       branch's delay slot from the insn PRECEDING it (`addiu $a1, $v1, 0x80',
       word 107), so the then arm falls through into the join with no `j' and
       the load delay after `lhu 0x10($sp)' costs the `nop' at 104.  Inline in
       the ternary (`: (s16)(base + 0x80)') is 236 words / 6 rows.  A full
       `if (e != 0) arg = ...;' STATEMENT is 234 / 19 -- it narrows through
       func_800B1320's `s16' prototype after the join, and its own test keeps
       the `sra' that cse then shares with the three later `e != 0' tests,
       deleting the target's second `lhu'/`nop'/`sll' reload group at 121-123.
       Free: `arg' as `s16' or `u32' (both 0), `: (s16)arg' (0), and
       `e == 0 ? arg : (s16)(...)' (0).  Not free: dropping the THEN arm's
       `(s16)' (2 rows) and `base - w + 0x76' for `base + 0x76 - w' (3).
   (3) FIVE SEPARATE TAIL LOCALS -- base, v, v2, z, z2 -- WITH NO SHARING.
       The park's "one local, two roles" rule is refuted on this body:
       `base' reused as `z' is 233 / 14 and as `v' 234 / 7.  Each
       `x2 = x; if (e != 0) x2 = x - 2;' pair is the target's copy-then-add
       (`addu $v1, $a0, $zero' / `addiu $v1, $v1, -0x2', words 166/167);
       folding it to an in-place `v -= 2' is 233 / 9, and `z -= 2' 233 / 6.
   (4) `u32 w' -- `s32' 234/2, `u16' 235/15, `s16' 236/16, `u8' 233/17 -- and
       `s16 j', not `s32' (230 / 50): `s16 j' is what puts the
       `andi $s7, $s6, 0xFF' at word 52.

   Also measured and inert: `has = i' for `has = 1' (cse folds it back),
   `u32 has', `u32 base' (`s16 base' is 4 rows), and both do-while wrappers. */
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *tmp;
    u32 *col;
    s32 i;
    s16 j;
    s32 has;
    u32 w;
    s32 base;
    s32 v2;
    s32 z2;
    s32 z;
    s32 v;
    s32 arg;

    i = 0;
    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    if (e != 0)
    {
        *(u16 *)(r + 0x14) |= 1;
    }
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0xC) = y;
    *(s16 *)((u8 *)m + 0xA) = 0;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    j = d & 0xFF;
    *(s16 *)(q + 0x24) = d >> 8;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    col = (u32 *)((u8 *)n + 0x58);
    u = func_800B12F4();
    *(u8 **)(q + 0x28) = u;
    if (u != NULL)
    {
        if (k != 0)
        {
            i = 1;
            col = (u32 *)((u8 *)n + 0x5C);
            *(u32 *)((u8 *)n + 0x70) = (u32)u;
            u = func_800B1434(u, D_80073618[k]);
            has = 1;
            *(u32 *)((u8 *)n + 0x58) = 0x808080;
        }
        else
        {
            has = 0;
        }
        *(s32 *)(q + i * 4 + 0x50) = (s32)func_8004E298(u, D_800DF03C[j], 0);
        i++;
        w = func_80069E98(D_800DF03C[j]) * 8;
        base = has * 10;
        arg = base + 0x80;
        func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : arg, (s16)(-*(s8 *)(u + 0x3) - 0x9));
        func_800B135C(u, e != 0 ? 4 : 8);
        func_800B13CC(u, 0x20);
        func_800B1400(u, 2);
        *col++ = 0x808080;
        u += 0x60;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        tmp = u;
        u = func_800B1434(u, u - 0x60);
        func_800B1320(tmp, 1, 1);
        *col = 0x80808;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        u[0x1] = 0x38;
        v = has == 0 ? -0x2 : -0xA;
        v2 = v;
        if (e != 0)
        {
            v2 = v - 2;
        }
        *(s8 *)(u + 0x2) = v2;
        *(s8 *)(u + 0x3) = -0xE;
        u[0x9] = 0;
        u[0x8] = 0;
        z = w + has * 24;
        if (has == 0)
        {
            z += 4;
        }
        z2 = z;
        if (e != 0)
        {
            z2 = z - 2;
        }
        u[0xA] = z2;
        u[0xB] = 0x10;
        u[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(u + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(u + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(u + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(u + 0x14) = *(s32 *)(q + 0x1C);
        u[0x1] |= 2;
        u += 0x18;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        u[0x1] = 0x81;
        *(s16 *)(u + 0x4) = 0x20;
        *(s16 *)(u + 0x6) = 0;
        u[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}

/* The dungeon's message panel: a 0x12-kind node whose +0x10 step is
   func_800B14FC, three func_800B12F4 sprite chains (the frame, the
   func_8004DA74 text laid out from func_800990FC's handle, and a two-piece
   cursor), and the five chain heads parked in the +0x50 array off the node.

   THE PARAMETERS ARE ALL `s16`. The fourth arrives as `d >> 8`
   (`sll 16`/`sra 24`), the fifth is a stack word tested twice as a halfword
   -- the two tests are why it is COPIED into $fp at entry -- and the sixth
   is spilled to 0x10($sp) because every callee-saved register is already
   taken.

   FOUR SPELLINGS THAT EACH COST WORDS, all measured:
   (1) func_800B1320's SECOND AND THIRD PARAMETERS HAD TO BE WIDENED from
       `u8` to `s16` (its definition below). This call site needs
       `sra 16`/`sra 24` where the `u8` prototype narrows to `andi 0xFF`,
       and it folds `(s16)(0x89 - len)` into `li -119`. func_800B1320 itself
       and its other caller func_800A6C00 both re-measure at 0 differing
       after the widening.
   (2) The five slots at +0x50 off the NODE are written off the node
       (`(u8 *)n + 0x70`, `+0x74`, `+0x58`), not off the +0x20 view, exactly
       as the landed func_800A6C00 writes its own.
   (3) ONE cursor local for the second and third func_800B12F4 chains. As a
       separate local the second chain lands in $a1 and the whole
       n/view/cursor trio rotates through $s1/$s2/$s3: 80 differing with
       every other word already exact.
   (4) `t[1] |= 2` goes AFTER all four +0x10..+0x1C copies. Written between
       the third and fourth copy it is 8 differing, before the first 223
       words; the `|=` reads back a byte of the record the copies write, so
       its position is the block's only scheduling freedom.
   The two negative bytes must be stored through `s8 *`: as `t[2] = -0xA` on
   a `u8 *` the constant folds to `li 246`.
   220 words, 0 differing. */
void func_800B1B10(u8 *p, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *u;
    u8 *t;
    u8 *s;
    u32 *col;
    s32 k;
    s32 len;
    void *h;

    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    *(s16 *)(q + 0x24) = d >> 8;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)(q + 0xC) = y;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    if (func_8003FA44(3) != 0)
    {
        col = (u32 *)((u8 *)n + 0x5C);
        k = 2;
        u = func_800B12F4();
        *(u8 **)(q + 0x28) = u;
        t = u;
        *(u32 *)((u8 *)n + 0x70) = (u32)t;
        t = func_800B1434(t, (u8 *)func_8004A658(p[1], p[0]));
        func_800B13CC(u, 0x20);
        func_800B1400(u, k);
        *(u32 *)((u8 *)n + 0x58) = 0x808080;
        h = func_800990FC();
        *(u8 *)func_80099368(p, h) = 0;
        s = (u8 *)func_8004DA74(t, h, 0);
        *(s32 *)((u8 *)n + 0x74) = (s32)s & 0x7FFFFFFF;
        len = strlen(h) * 4;
        func_800B1320(t, e != 0 ? (s16)(0x89 - len) : 0x8A, (s16)(-*(s8 *)(t + 0x3) - 0xE));
        func_800B135C(t, e != 0 ? 4 : 8);
        func_800B13CC(t, 0x20);
        func_800B1400(t, 2);
        *col++ = 0x808080;
        strlen(h);
        t = func_800B12F4();
        *(u8 **)(q + 0x2C) = t;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t & 0x7FFFFFFF;
        k++;
        func_800B1434(t, s);
        func_800B1320(s, 1, 1);
        t = func_800B12F4();
        *(u8 **)(q + 0x30) = t;
        *col = 0x80808;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t;
        t[0x1] = 0x38;
        *(s8 *)(t + 0x2) = -0xA;
        *(s8 *)(t + 0x3) = -0x10;
        t[0xA] = len + 0x18;
        t[0x9] = 0;
        t[0x8] = 0;
        t[0xB] = 0x14;
        t[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(t + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(t + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(t + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(t + 0x14) = *(s32 *)(q + 0x1C);
        t[0x1] |= 2;
        k++;
        t += 0x18;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t;
        t[0x1] = 0x81;
        *(s16 *)(t + 0x4) = 0x20;
        *(s16 *)(t + 0x6) = 0;
        t[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}

/* A node +0x10 step: every fourth frame advance the +0x2 cursor by 8, wrap it
   at 0x60, and play the row it now points at. The field is read BACK for the
   call because the wrap store sits between. 32 words, 0 differing. */
void func_800B1E80(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s16 u;

    t = *(u16 *)p - 1;
    *(s16 *)p = t;
    if (t <= 0)
    {
        *(s16 *)p = 4;
        u = *(u16 *)(p + 0x2) + 8;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x60)
        {
            *(s16 *)(p + 0x2) = 0;
        }
        func_8003F80C((u32)&D_800DF068[*(s16 *)(p + 0x2)], 0x7340, 1, 2);
    }
}

/* 13 words, 0 differing. */
void func_800B1F00(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B1E80;
    }
}

/* The node's +0x10 step for func_800B2074's marker: a four-state machine on
   the view's +0x04. TWO FALL-THROUGHS ARE REAL -- state 0 falls into state 1,
   and state 2 falls into state 3. Four cases plus a default is under
   CASE_VALUES_THRESHOLD, so this is a comparison tree and not a jump table.
   80 words, 0 differing. */
void func_800B1F34(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(p + 0x6) = 0x10;
        *(s16 *)(p + 0x4) += 1;
    case 1:
        b->unk_14 -= 0x4000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x40;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 2:
        b->unk_14 += 0x6000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
    case 3:
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            b->unk_14 = 0;
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    }
    b->unk_08 -= b->unk_14;
    *(u32 *)(D_800814A8 + 0x1C) |= 0x40000000;
    **(s16 **)p = 0x20 - *(u16 *)((u8 *)b + 0xA);
}

/* Spawn the marker node.

   THE TWO COORDINATES ARE `s32`, not s16. The cell index is written
   `(x << 10) >> 16`; with s16 parameters gcc folds the entry sign-extension
   into the pair and emits `sll 16`/`sra 22` instead of the target's
   `sll 10`/`sra 16` -- 7 differing, with no other difference.

   THE VIEW POINTER MUST BE BOUND BEFORE ITS USE, not at it. Assigned on the
   statement that uses it, `(u8 *)n + 0x20` folds into the store's
   displacement (3 differing); bound at the top of the `if` it survives as the
   target's own `addiu $v0, $s1, 0x20`, even though update_equiv_regs still
   moves the set down to the use. 81 words, 0 differing. */
void func_800B2074(s32 x, s32 y)
{
    UnkNode *n;
    u8 *g;
    u8 *r;
    u8 *v;
    UnkVec *m;
    UnkSlot *s;

    g = (u8 *)&D_80083160;
    r = *(u8 **)(g + 0x1DC);
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B1F34;
        func_8004491C(n, (u32)D_80046398);
        s = (UnkSlot *)n->unk_0C;
        s->unk_20 = 0x1000;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        s->unk_08 = 2;
        *(s32 *)((u8 *)s + 0xC) = 0x808080;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x16) = 6;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        r += (((x << 10) >> 16) + (((y << 10) >> 16) << *(s16 *)(g + 0x1F0))) * 6;
        *(u8 **)((u8 *)n + 0x20) = r + 2;
        *(s16 *)r = 3;
        *(s16 *)((u8 *)m + 0xA) = -*(u16 *)(*(u8 **)((u8 *)n + 0x20));
        *(u16 *)(r + 0x2) += 0x20;
        *(u16 *)(r + 0x4) |= 1;
        func_800A56E0(0x603);
        *(s16 *)(v + 0xE) = *(u16 *)(D_800814A8 + 0x2A);
    }
}

/* A node +0x10 step: lift the mover 0x8000 a frame, re-register the
   renderable, and mark the node dead when the +0x6 countdown goes negative.
   30 words, 0 differing. */
void func_800B21B8(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    b->unk_08 += 0x8000;
    func_800478B8(c);
    t = *(u16 *)(p + 0x6) - 1;
    *(s16 *)(p + 0x6) = t;
    if (t < 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 48 words, 0 differing. The whole-UnkVec assignment is the 24-byte block
   move: four `lw`/`sw` pairs then two. */
void func_800B2230(UnkVec *src)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B21B8;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *m = *src;
        r = (UnkSlot *)n->unk_0C;
        *(u32 *)r->unk_0C = 0x808080;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_8003DB94(r, D_800DEC70, 0);
        *(s16 *)((u8 *)n + 0x26) = 0x20;
    }
}

/* The spawner tail func_800D1C70 shares: stamp the record's room id, place
   the mover, measure the leash into the view's +0x88, dress the renderer,
   light the node up and hang a 0x110 marker node off it. FOUR PARAMETERS --
   the fourth is never read here, but func_800B23F8 sign-extends $a3 into
   the `jal`'s delay slot. 66 words, 0 differing, first compile. */
void func_800B22F0(UnkNode *n, UnkVec *m, UnkSlot *r, s16 d)
{
    u8 *p;

    p = (u8 *)n + 0x20;
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s16 *)(p + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                       *(u16 *)((u8 *)m + 0xA) - 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    r->unk_14 |= 0x8000;
    func_80099FDC(n);
    func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                  (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0xAD] = 1;
    p[0x71] = 0;
    *(u32 *)(p + 0x1C) |= 0x40000000;
    func_800BC26C(n, 0, (s32)((u8 *)r + 0x2C), (s32)((u8 *)n + 0x4A));
}

/* The 0x112 spawner: allocate a node, install func_800B274C as its handler
   and func_800B2A60 as the payload's +0x8C step, return the payload or NULL.
   81 words, 0 differing.

   THE PARAMETER WIDTHS ARE THE WHOLE FUNCTION.  `b' and `c' are stored with
   `sb' but must be DECLARED ONE STEP WIDER, as u16.  A sub-word declared type
   makes assign_parms emit a raw SImode copy plus a separate truncation (and
   function.c:4093 flushes every truncation after every parameter has left its
   hard register); when the only use is NARROWER than the declared type, cse
   cannot fold the truncation away, so it survives, combine folds the raw copy
   into it, and the raw copy at the parameter's declaration position becomes a
   NOTE_INSN_DELETED.  sched1's parameter-pinning prefix loop (sched.c:3247)
   requires GET_CODE (head) == INSN, so it BREAKS AT THAT NOTE and pins only
   the first parameter -- `b', `c' and `d' are then schedulable and sched1's
   birthing-insn rule sinks them into the entry block's call setup, which is
   the listing's interleaved `move $sN,$aN' order.  Measured on this body:
   `u8 b, u8 c' 34 rows, `u16 b, u8 c' 5, `u8 b, u16 c' 25, `u32 b' 29,
   `u32 c' 11.  Signedness is free (all four s16/u16 combinations are 0) and
   the width is exact: same-width keeps the raw copy pinned, and u32/s32 emits
   no truncation at all and also keeps it pinned.  See
   docs/matching-levers.md, "A SUB-WORD parameter type is how a parameter copy
   becomes schedulable".

   DO NOT PUT THIS PROTOTYPE IN csrc/ovl_dungeon_decls.h.  Nothing calls this
   function from C today, and a narrow prototype in scope rewrites a future
   caller's `sll'/`sra' into a truncation.  The definition's own parameter list
   is all the landing needs.

   `w' is a SECOND payload pointer and it is load-bearing: the late stores go
   through $s3, the register `c' dies in.  Writing them through `p' is 80 words
   and 31 rows.  `w = p;' and `w = (u8 *)n + 0x20;' are byte-identical.

   `*(s8 *)(w + 0x9C) = -1;' and NOT `w[0x9C] = 0xFF;' -- through a u8 lvalue
   gcc folds -1 to 0xFF, cse shares it with the neighbouring 0x9A store and the
   `addiu $v0, $zero, -0x1' disappears: 80 words, 22 rows.

   NO DEVICES.  The park carried two -- an `__asm__("" ::"r"(n));' and a
   `c = (s32)p;' merge that made the parameter double as the second payload
   pointer -- and both are unnecessary once the widths are right. */
/* func_800B274C is defined below this use and its address is taken, so it
   needs a declaration here; across the parameter list it is not implicit. */
void func_800B274C(u8 *, UnkVec *, u8 *);
u8 *func_800B23F8(s16 a, u16 b, u16 c, s16 d, s32 e)
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
        n->unk_10 = (u32)func_800B274C;
        p[0x13] = 0x1E;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *((u8 *)r + 0x24) = b;
        k = a & 3;
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
        func_800B22F0(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_800B2A60;
        *(s32 *)((u8 *)r + 0x8) = e;
    }
    return p;
}

/* The +0x8C handler's wake-up gate: if the record is flagged busy or
   queued, and the actor is not in state 0x13/0x14, hand it to one of two
   continuations or drop it back to a plain state. The D_80083460 block is the
   engine's single "current record" slot: when D_800DCE68 is set this function
   claims that slot for `d` (only while it is empty, its +0x10 partner is
   clear, +0x08 is zero, +0x02 has neither of 0x2008 set and d[0x43] is 0xFD);
   when it is clear, func_800A2BDC decides instead.

   THE WHOLE RESIDUE WAS ONE REGISTER, AND THE CURE IS TO REUSE THE TEST'S OWN
   TEMPORARY AS THE VALUE. Written as

       if ((t & 0x4000) == 0) { v = 2; } else { v = 1; }

   the function is 132 words and 3 differing: `jump.c:789` rewrites it to
   `v = 1; if (...) v = 2;` and hoists the `v = 1` above the compare, so `v` is
   live across the `andi` whose result holds $v0 and global-alloc gives `v`
   $a0 instead. Writing

       v = t & 0x4000;
       if (v == 0) { v = 2; } else { v = 1; }

   makes the mask, the branch operand and the stored value ONE pseudo, which
   both blocks the hoist (`reg_referenced_between_p` sees `v` in the test) and
   forces the shared $v0. Also measured at 3 differing, all on the same word
   triple: `v` as u8/s16/u32/s32, declared first or last or block-scoped, `v`
   reused from `t`, the arms written in the other order, a do/while(0) wrapper,
   and a dead second assignment in each arm. `v` reused from `f` is 31, the
   0x14 clear moved below the if/else is 12, and storing the field in both
   arms instead of a shared `v` is 134 words / 44.

   A second spelling fact, worth 2 words: the two arms must share the `sb`.
   `p->unk_9A = (t & 0x4000) ? 1 : 2;` and a store per arm both give 134 words.

   132 words, 0 differing. */
void func_800B253C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *cur;
    u32 f;
    u32 t;
    s32 v;

    if ((*(u32 *)(d + 0x1C) & 0x2000) == 0 && (*(u32 *)(d + 0x14) & 0x4000) == 0)
    {
        return;
    }
    if (p->unk_9A == 0x13 || p->unk_9A == 0x14)
    {
        return;
    }
    if (*(s8 *)(d + 0x6D) != 0)
    {
        return;
    }
    if (D_800DCE68 == 0)
    {
        if ((s16)func_800A2BDC(d) != 0)
        {
            return;
        }
    }
    else
    {
        g = (u8 *)&D_80083460;
        cur = *(u8 **)(g + 0xC);
        if (cur != d)
        {
            if (*(u32 *)(g + 0x10) != 0)
            {
                return;
            }
            if (cur != NULL)
            {
                return;
            }
            if (*(s16 *)(g + 0x8) != 0)
            {
                return;
            }
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            if (d[0x43] != 0xFD)
            {
                return;
            }
            *(u8 **)(g + 0xC) = d;
        }
        if (d[0x43] != 0xFD)
        {
            return;
        }
    }
    f = *(u32 *)(d + 0x1C);
    if ((f & 0x400000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x400000;
        t = *(u32 *)(d + 0x14);
        if ((t & 0x20000000) != 0)
        {
            *(u32 *)(d + 0x14) = t | 0x400000;
            *((u8 *)p + 0xAD) = 0;
            func_800ACB98((u8 *)p, b, c, d);
            return;
        }
        *(u32 *)(d + 0x14) = t & ~0x400000;
        v = t & 0x4000;
        if (v == 0)
        {
            v = 2;
        }
        else
        {
            v = 1;
        }
        p->unk_9A = v;
    }
    else if ((f & 0x2000000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x2000000;
        d[0x71] = 0;
        if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
        {
            func_800ACD74((u8 *)p, b, c, d);
            return;
        }
        p->unk_9A = 0;
    }
    else
    {
        return;
    }
    p->unk_9B = 0;
}

/* The dungeon actor's per-frame driver: past the engine's 0x2000 gate it runs
   func_800B253C, then the record's own +0x8C continuation, then the +0x9A
   entry of the D_800DF1E8 table, re-aims the tile when the +0x6D facing moved
   under it, steps the mover by its velocity, runs the +0x9D bob counter and
   finally leashes +0x92/+0x88 to the ground height func_800BCB04 reports.

   THE RECORD IS ITS OWN ACTOR: all four four-argument calls pass $a0 as both
   the first and the fourth argument. AND IT IS READ THROUGH TWO ALIASES --
   `q` carries +0x88 and +0x1C, `p` carries everything else INCLUDING the
   +0x1C that the func_800A020C argument reads, which is the same
   one-field-two-spellings split func_800B2A60 records for D_80083462.
   Dropping `q` and spelling every access through `p` is 194 words / 192
   differing: the `move $s1, $s0` and its two save/restore words go, and the
   body re-registers wholesale.

   `*(u32 *)(q + 0x1C) &= 0xF7FFFFFF;` is written out in all three arms that
   need it; cross-jumping merges the two that fall into the tail and leaves
   the third sharing only the `sw` at word 146.

   197 words, 0 differing, first compile. */
void func_800B274C(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    s16 dir;
    s32 r;

    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        p[0x71] &= 0x7F;
        return;
    }
    dir = (s8)p[0x6D];
    *(u16 *)(p + 0x98) &= 0xFBFF;
    if ((*(u32 *)(p + 0x14) & 0x100000) != 0)
    {
        return;
    }
    func_800B253C((UnkDungeonActor *)p, b, c, p);
    h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h != NULL)
    {
        h(p, b, c, p);
    }
    D_800DF1E8[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        func_800A020C(*(s32 *)(p + 0x1C), c + 0xC);
    }
    *(u16 *)(c + 0x14) |= 0x7000;
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
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
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
                p[0x9D] = 0;
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                                  *(u16 *)(q + 0x88) - 0x20);
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

/* State 0xE's gate chain, the func_800C9F34 (:18679) shape one record over:
   the D_80083460 +0x2 flags, the view's +0x25 and func_80042900, then --
   only when the +0x2000 draw flag is clear -- the +0x1C bits, the state
   byte, the +0x98 mask, the +0x64 probe and the +0x80000 hand-off; the tail
   re-aims the tile and, past the +0x6D/D_80083462/func_800A2C34 gates, asks
   func_800A03C4 whether to tear down or clear +0x6D.

   D_80083460 + 0x2 IS READ TWICE THROUGH THE POINTER LOCAL AND ONCE AS THE
   DECLARED `D_80083462`, which is the three-spelling split func_800C9F34
   records -- the declared scalar is the `lui`/`lhu` pair with its own `%hi`
   at word 95. `p[0xAD] = 0; func_800ACB98(p, b, c, d);` is written out at
   all four exits: cross-jumping merges three of them onto the copy at word
   112 and leaves the `d[0x25] == 0` one its own two words, because there the
   incoming $a0..$a3 are still the call's own arguments and it needs no setup.
   130 words, 0 differing, first compile. */
void func_800B2A60(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
    {
        return;
    }
    if (d[0x25] == 0)
    {
        p[0xAD] = 0;
        func_800ACB98(p, b, c, d);
        return;
    }
    if ((s16)func_80042900((s8 *)d, 1) != 0)
    {
        p[0xAD] = 0;
        func_800ACB98(p, b, c, d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= 0xFFF3;
        if (*(s16 *)(d + 0x64) != 0)
        {
            if (func_800AA6B4(p, b, c, NULL) != 0)
            {
                return;
            }
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_800B318C(p, b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            p[0xAD] = 0;
            func_800ACB98(p, b, c, d);
            return;
        }
    }
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((D_80083462 & 0x1000) == 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                if (func_800A03C4(d, c[0x24], c[0x25]) != NULL)
                {
                    p[0xAD] = 0;
                    func_800ACB98(p, b, c, d);
                }
                else
                {
                    d[0x6D] = 0;
                }
            }
        }
    }
}

/* Ten one-gate installers: run the predicate, and on a pass park the next
   handler in the record's +0x8C slot. csrc/ovl_town.c's func_800A4F4C is the
   same source with a different record offset and predicate. 14 words each,
   0 differing. */
void func_800B2C68(UnkDungeonActor *p)
{
    if (func_800AB378() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* State 1's three-step lunge, switched on the actor's +0x9B. Step 0 faces the
   record with func_800AD4D0 and seeds the mover's velocity from the facing
   tables, then either hands straight to step 2, tears the actor down, or arms
   the +0x96 countdown; step 1 bleeds a quarter off each axis per frame and
   runs the countdown out; step 2 eases the mover onto c[0x24]/c[0x25] over
   what is left of the countdown and parks func_800B2A60 in the +0x8C slot.

   CASE 0 FALLS THROUGH INTO CASE 1 -- the `sh $v1, 0x96($s1)` at word 43 is
   the last insn before .L800B2DB8, which is also case 1's switch arm. The
   gcc switch tree for cases 0/1/2 is the balanced one: `beq 1` first, then
   `slti 2` splitting 0 from 2.

   THE D_80083460 TEARDOWN IS WRITTEN OUT TWICE AND CROSS-JUMPING MERGES IT.
   Case 0's `d[0x28] == 0` exit and case 1's countdown-expired exit are the
   same seventeen words ending in the same `j`, so they fold onto one copy at
   word 100; writing it once behind a label is not expressible and writing it
   as a helper would cost the call. Each copy gets ITS OWN `u8 *` local, the
   usual rule for this base in this unit.

   The second `if` is inverted against the first. Case 0 writes
   `if (d[0x28] != 0) { +0x9B = 2; return; }` and gets `beqz` past the then-
   block; case 1 writes `if (d[0x28] == 0) { <teardown> return; }` so the
   `+0x96 = 8` tail sits AFTER the if and the `bnez` reaches it with the 8
   already in its delay slot.

   `p->unk_96 = -1; if (d+0x1C & 0x228) p->unk_96 = 8;` written as a shared
   local assigned -1 FIRST is the jump.c:789 shape the target has: the -1
   rides the `beqz` delay slot and both arms share the one `sh`.

   The two facing-table reads are spelled out in full rather than through a
   shared index: the `b->unk_0C` store between them is a QImode-free MEM
   store that invalidates cse's memory table, so the target reloads
   `*(u16 *)(d + 0x6A)` and recomputes the index, which is exactly what two
   written-out copies give. 195 words, 0 differing, first compile. */
void func_800B2CA0(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    s32 t;
    s32 x;
    s32 y;

    switch (p->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p->unk_9B++;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            if (d[0x28] != 0)
            {
                p->unk_9B = 2;
                return;
            }
            g = (u8 *)&D_80083460;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) + 1;
            func_8009A028(d);
            *(u16 *)((u8 *)p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        t = -1;
        if ((*(s32 *)(d + 0x1C) & 0x228) != 0)
        {
            t = 8;
        }
        p->unk_96 = t;
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (p->unk_96 > 0)
        {
            p->unk_96--;
        }
        else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            p->unk_96 = 0;
        }
        if (p->unk_96 != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
            h = (u8 *)&D_80083460;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            if (*(s32 *)(h + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(h + 0x10) &= 0x7FFFFFFF;
            }
            *(s16 *)(h + 0xA) = *(u16 *)(h + 0xA) + 1;
            func_8009A028(d);
            *(u16 *)((u8 *)p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        p->unk_96 = 8;
        p->unk_9B++;
        break;
    case 2:
        if (p->unk_96 != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) / p->unk_96;
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) / p->unk_96;
        }
        p->unk_96--;
        if (p->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        k = (u8 *)&D_80083460;
        if (*(s32 *)(k + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(k + 0x10) &= 0x7FFFFFFF;
        }
        p->unk_8C = func_800B2A60;
        break;
    }
}

/* func_800B30E4 behind the renderable's 0xE000 gate and with func_800A2DB8
   ahead of the teardown. 48 words, 0 differing. */
void func_800B2FAC(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    if (*(u16 *)(c + 0x14) & 0xE000)
    {
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A2DB8(d);
        func_800A32A4(d);
        func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Six forwarders, one call each and nothing else. 8 words each,
   0 differing. */
void func_800B306C(void)
{
    func_800AB538();
}

void func_800B308C(void)
{
    func_800AB778();
}

void func_800B30AC(UnkDungeonActor *p)
{
    if (func_800ABDBC() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* The +0x8C step that tears an actor down. THE 0x300 / 0x3000 CHOICE IS A
   TERNARY IN THE CALL, not an if/else around two calls: the target sets
   $a0/$a1 above the branch and only $a2 inside it. 42 words, 0 differing. */
void func_800B30E4(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
    {
        *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
    }
    func_800A32A4(d);
    func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    func_8009A028(d);
    *(u16 *)(d - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_800B318C(void)
{
    func_800AC82C();
}

void func_800B31AC(UnkDungeonActor *p)
{
    if (func_800AC010() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* FOUR PARAMETERS, forwarded. func_800AC480 reads all four argument
   registers; this caller only passes them through, so the wrong arity was
   invisible until that function was read. 14 words, 0 differing either way. */
void func_800B31E4(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AC480((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

void func_800B321C(UnkDungeonActor *p)
{
    if (func_800AC5C8() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* FOUR PARAMETERS, forwarded, exactly as the func_800AC480 pair next door:
   func_800ACE34 reads $a0 and $a2 and this caller only passes them through.
   14 words, 0 differing either way. */
void func_800B3254(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800ACE34((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* The kind-0x210 node's +0x10 step func_800B341C parks: project the parent
   slot into `out`, move the mover onto the parent mover's position plus that
   offset, and -- once the +0x8C latch is set -- spawn a func_800B8EA8 trail
   from the OLD position to the new one. 100 words, 0 differing, first
   compile.

   THE OLD POSITION IS THREE `s16` LOCALS, read before the three updates
   overwrite the same halfwords. Each one costs an `lhu` plus its own
   `sll 16`/`sra 16` at the int use, which is what an `s16` local costs
   here, and the loads sit above the stores because the source reads them
   first.

   The two stores per axis are a CHAINED assignment: the mover's field is
   the inner store and the `u16 trail[]` slot the outer, which is the order
   the listing emits them in.

   The dead-node tail is reached from TWO places -- the `r == NULL` arm with
   the latch set, and the +0x8E countdown going negative -- and the `bnez`
   at 0x800B33C0 branches straight into it, so it is one `goto` label and
   not a duplicated arm. */
void func_800B328C(u8 *q, UnkVec *b, u32 c)
{
    UnkSlot *s;
    u8 *r;
    u8 *v;
    s16 x;
    s16 y;
    s16 z;
    s16 t;
    s16 out[4];
    u16 trail[4];

    s = (UnkSlot *)(*(UnkNode **)q)->unk_0C;
    r = (u8 *)func_8003DE58((u8 *)s->unk_08, s, out, 0);
    if (r != NULL)
    {
        v = (u8 *)(*(UnkNode **)q)->unk_08;
        if (*(s16 *)(q + 0x8C) != 0)
        {
            x = *(s16 *)((u8 *)b + 0x2);
            y = *(s16 *)((u8 *)b + 0x6);
            z = *(s16 *)((u8 *)b + 0xA);
            trail[0] = *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2) + out[0];
            trail[1] = *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6) + out[1];
            trail[2] = *(s16 *)((u8 *)b + 0xA) = *(u16 *)(v + 0xA) + out[2];
            func_800B8EA8(*(void **)q, x, y, z, trail, *(void **)(q + 0x4), 2, 0x800, 1);
        }
        else
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(v + 0xA) + out[2];
            *(s16 *)(q + 0x8C) = 1;
        }
    }
    else if (*(s16 *)(q + 0x8C) != 0)
    {
        goto dead;
    }
    t = *(u16 *)(q + 0x8E) - 1;
    *(s16 *)(q + 0x8E) = t;
    if (t < 0)
    {
    dead:
        *(u16 *)(q - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Allocate a kind-0x210 node under the caller's record, park func_800B328C at
   its +0x10 and write the two pointers and a halfword into the payload.
   `*(void **)q = a` comes back as `sw $s1, 0x20($v1)` off the NODE while
   `*(void **)(q + 4)` keeps the `$v0` base -- cse rewrites `*q` into
   `*(p + C)` and leaves `*(q + 4)` alone. 28 words, 0 differing. */
void func_800B341C(u8 *a, void *b, s16 c)
{
    UnkNode *n;
    u8 *q;

    if (b != NULL)
    {
        n = func_8003FD64(0x210, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800B328C;
            q = (u8 *)n + 0x20;
            *(void **)q = a;
            *(void **)(q + 0x4) = b;
            *(s16 *)(q + 0x8E) = c;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800A4B88", D_80089118);

/* The torch/lantern cast's state machine over the actor's +0x9B, sixteen
   entries in jtbl_800891C0 with 11..14 falling to the default. States 0..2
   open the effect (D_800DD030/D_800DD068 skins, func_800C77D0 on the
   record's -0x20), 3..6 run the knock-back -- the eight-way D_8006CCD8/E8
   step into the mover, the saved +0x14 fall rate parked in D_800DF244 and
   func_800B341C/func_800B3D10 on the way out -- 7..9 close on
   func_8009C12C's message node, and 10 hands the actor back to
   func_8008ACDC. The table is at rodata offset 0xA60, 0 mod 8, so cc1psx's
   `.align 3` is a no-op: NOT one of the seven at 4 mod 8.
   545 words, 0 differing.

   THREE LEVERS, and the function went 106 -> 39 -> 18 -> 4 -> 0 aligned
   differences on them, with no register work at all.

   (1) ONE POINTER LOCAL PER ARM, func_800974DC's rule, and it applies to
   the SCRATCH INTEGERS too. One shared `t` over the ten skin-table arms is
   a global allocno and comes out in $a3 where the target has $a1 in every
   arm: 106 differences. Ten per-arm locals: 39. The same rule on the ONE
   eight-way index shared by case 3 and case 6 -- `k` and `k2`, each
   written once -- is worth another FIVE (23 -> 18), because a local
   written in two arms is never deferred by sched1 (:26241) and its `lhu`
   outranks the table's `high` in the ready list. With the index split, the
   `lui %hi(D_8006CCD8)` lands ahead of the `lhu 0x2A(d)` and its `addiu`
   fills the load-delay slot, exactly as func_800974DC's landed case 0 does.

   (2) THE THREE MOVER STORES ARE IN SOURCE ORDER +0x0C, +0x10, +0x14 IN
   BOTH ARMS, and the order is worth 14 of the last 18. All six
   permutations were measured at 545 words with the index split: case 3
   gives 18 (C,A,B... i.e. 0x0C/0x14/0x10), 18, 20, 10, 20 and **8** for
   0x0C/0x10/0x14; case 6 gives 14 for the same order and 28/35 at 544
   words -- one SHORT -- for the two that put +0x14 first or last-but-one.
   Both arms at 0x0C/0x10/0x14 together: 4. The +0x14 store is the only
   insn in the block with no consumer, so wherever it sits in the source it
   eats whichever load-delay hole is open there.

   (3) THE `-8` MASK IN CASE 10 MUST BE ITS OWN STATEMENT, worth the last
   four. `*(s32 *)(d + 0x14) &= -8;` emits `li $a0,-8` after the
   `lhu 0x2(&D_80083460)`, where the target has it before; `m = -8;` ahead
   of the D_800DF248 read swaps the two and the function is byte-exact.
   `&= ~7`, `&= 0xFFFFFFF8`, `(u32 *)`, a `d + 0x14` pointer local and the
   expanded `x = x & -8` form are all still 4.

   `v2 = D_800DF248;` as its own statement before the mask is the lever
   this file already records for this function at :27577 -- without it the
   `lui`/`lh` pair falls after the `and` and three nops appear: 547 words.

   All 24 orderings of the case-10 tail's four statements were measured;
   none beats the one here, and six of them cost words (546 or 548).

   func_800B3D10, func_800B4194 and func_800C77D0 are called IMPLICITLY and
   must stay that way: all three are defined BELOW this function, and a
   declaration at the top of the unit would reach every other call site. */
