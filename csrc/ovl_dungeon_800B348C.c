#include "common.h"
#include "ovl_dungeon_decls.h"

/* dungeon.bin's overlay, translation unit 4 of 4: func_800B348C to the next
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

s16 func_800981F8(u8 *p);
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
s16 func_800A9400(s16 x);
void func_800B341C(u8 *a, void *b, s16 c);

void func_800B348C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t1;
    u8 *t2;
    u8 *t3;
    u8 *t4;
    u8 *t5;
    u8 *t6;
    u8 *t7;
    u8 *t8;
    u8 *t9;
    u8 *t10;
    u8 *e1;
    u8 *e2;
    u8 *e;
    u8 *g;
    u8 *f;
    u8 *h;
    s32 x;
    s32 y;
    s32 fall;
    s16 n;
    s32 k;
    s32 k2;
    s32 v2;
    s32 m;

    switch (p[0x9B])
    {
    case 0:
        *(s32 *)(d + 0x14) &= -8;
        p[0x9B]++;
        D_800DF248 = 0;
        break;
    case 1:
        if (*(u16 *)(p + 0xA6) != 0)
        {
            t1 = D_800DD030;
            *(u8 **)(c + 0x2C) = t1;
            func_80048A44(c, t1[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(s32 *)(p + 0xCC) = 0;
            if (*(u16 *)(p + 0xA6) >= 2)
            {
                break;
            }
        }
        else
        {
            *(s32 *)(p + 0xF4) = 0;
        }
        *(u16 *)(p + 0x98) &= 0xDFFF;
        p[0x9B]++;
        /* fall through */
    case 2:
        if (*(u16 *)(p + 0xA6) != 0)
        {
            break;
        }
        func_800C77D0(d - 0x20, b, 8, 0x300);
        t2 = D_800DD068;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s32 *)(p + 0xCC) = 0;
        p[0x9B]++;
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_0C = D_8006CCD8[k] << 17;
        b->unk_10 = D_8006CCE8[k] << 17;
        b->unk_14 = 0xFFEBC000;
        t3 = D_800DD070;
        *(u8 **)(c + 0x2C) = t3;
        func_80048A44(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        if (*(s32 *)(d + 0x4C) != 0)
        {
            e1 = D_800DD078;
        }
        else
        {
            e1 = D_800DD0A8;
        }
        *(u8 **)(p + 0xCC) = e1;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
        break;
    case 4:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        t4 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t4;
        func_80048A44(c, t4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        if (*(s32 *)(d + 0x4C) != 0)
        {
            e2 = D_800DD080;
        }
        else
        {
            e2 = D_800DD0B0;
        }
        *(u8 **)(p + 0xCC) = e2;
        p[0x9B]++;
        break;
    case 5:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        if (p[0xA8] != 0)
        {
            D_800DF248 = 1;
            p[0x9B]++;
            break;
        }
        *(s16 *)(p + 0x96) = 4;
        p[0x9B] += 0xA;
        break;
    case 15:
        if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        func_800A56E0(*(s32 *)(d + 0x4C) != 0 ? 0x605 : 0x606);
        t5 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t5;
        func_80048A44(c, t5[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD088;
        *(s16 *)(p + 0x96) = 2;
        p[0x9B] = 7;
        break;
    case 6:
        *(u16 *)(c + 0x14) |= 0x800;
        *(u16 *)(p + 0x98) |= 0x8;
        fall = b->unk_14;
        b->unk_0C = 0;
        b->unk_10 = 0;
        b->unk_14 = 0;
        n = --*(s16 *)(p + 0x96);
        D_800DF244 = fall;
        if (n >= 0)
        {
            break;
        }
        k2 = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_0C = D_8006CCD8[k2] << 17;
        b->unk_10 = D_8006CCE8[k2] << 17;
        b->unk_14 = fall;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s16 *)(p + 0x96) = 4;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
        func_800B341C(p - 0x20, *(void **)(p + 0xF4), 0x40);
        t6 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t6;
        func_80048A44(c, t6[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD088;
        func_800B3D10(p[0xA8], *(s32 *)(d + 0x60), d);
        break;
    case 7:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        if (*(s16 *)(p + 0x96) == 0)
        {
            if (p[0x102] == 0)
            {
                e = func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
                if (e != NULL)
                {
                    func_800B4194(p[0xA8], e, d);
                    p[0xA8] = 0;
                }
                else
                {
                    e = *(u8 **)(d + 0x60);
                    if (e != NULL)
                    {
                        *(s32 *)(d + 0x60) = 0;
                        *(s32 *)(e + 0x1C) &= 0xEFFFFFFF;
                        g = *(u8 **)(e - 0x14);
                        *(s32 *)(g + 0xC) = 0x808080;
                    }
                }
            }
            else
            {
                f = *(u8 **)(d + 0x60);
                if (f != NULL)
                {
                    *(s32 *)(f + 0x14) |= 0x1000000;
                }
            }
        }
        if (*(s16 *)(p + 0x96) <= 0 && (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            t7 = D_800DD088;
            *(u8 **)(c + 0x2C) = t7;
            func_80048A44(c, t7[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u8 **)(p + 0xCC) = D_800DD090;
            p[0x9B]++;
        }
        /* fall through */
    case 8:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        if (p[0xA8] != 0)
        {
            p[0xA8] = 0;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        t8 = D_800DD090;
        *(u8 **)(c + 0x2C) = t8;
        func_80048A44(c, t8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD098;
        p[0x9B] = 9;
        break;
    case 9:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        t9 = D_800DD098;
        *(u8 **)(c + 0x2C) = t9;
        func_80048A44(c, t9[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s16 *)(p + 0x96) = 2;
        b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(p + 0x96);
        b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(p + 0x96);
        p[0x9B]++;
        break;
    case 10:
        if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        t10 = D_800DD0A0;
        *(u8 **)(c + 0x2C) = t10;
        func_80048A44(c, t10[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        func_80099F04(*(u8 **)(d + 0x5C));
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0x2) |= 0x412;
        *(void **)(p + 0x8C) = func_8008ACDC;
        m = -8;
        v2 = D_800DF248;
        *(s32 *)(d + 0x14) &= m;
        if (v2 == 0)
        {
            *(s32 *)(h + 0xC) = 0;
        }
        *(s16 *)(p + 0x96) = 0;
        break;
    }
}

/* The weapon-element speed table: look the slot's kind up with
   func_800A9400 and, per element, raise one of the renderable's three
   +0x14 bits and scale its +0x20 speed by D_800E3D68 / (D_800E3D68 + 1).
   Kinds 1..3 scale unconditionally, kinds 7..9 scale by 2x when the
   wielder's +0x4C record is kind 0x10 and matches the element, by 1.5x
   when it is 0x10 and does not, and otherwise work off the wielder's own
   func_800981F8 reading; kinds 13..45 only set the bit. 289 words,
   0 differing, and the 49-entry jtbl_80089200 comes out entry for entry.

   IT OWNS A JUMP TABLE, AND THAT TABLE IS AT 0 MOD 8, so the `.align 3`
   cc1psx writes before it is a no-op -- `jtbl_80089200` is NOT one of the
   seven 4-mod-8 tables docs/dungeon-sweep.md lists, and this is the second
   landed table in the unit after func_80095538's.

   THE TABLE'S LENGTH IS THE ONLY WITNESS FOR THE TOP CASE LABEL. gcc sizes
   a switch table max-min+1, the table is 49 entries and the dispatch's
   `addiu $v0, -0x1` / `sltiu 0x31` puts min at 1, so a case 49 exists even
   though its body is empty; 46..48 are written with it because they share
   the triple rhythm of 13..45. Stopping at `case 45:` is a 45-entry table
   and four words short.

   FOUR SPELLINGS ARE LOAD-BEARING, each measured on this function:

   (1) IN KINDS 1..3 THE `+=` COMES FIRST AND THE `|=` SECOND. The other way
   round the mult/div chain loses sched1's first slot to the `|=`'s
   load/or/store, the `sw +0x14` no longer sits next to the `addu`/`sh +0x20`
   pair, and cross-jumping then merges only the `sh` instead of all four
   words: 286 words against 289, 250 differing.

   (2) IN KINDS 7..9 THE `|=` COMES FIRST. The two orders are not the same
   question: here the store is followed by a `lw +0x4C` and two byte tests,
   not by a divide.

   (3) THE func_800981F8 PATH NEEDS AN EXPLICIT `u` FOR THE OLD +0x20 AND
   AN EXPLICIT STORE. Written `*(s16 *)(c + 0x20) += ...` the `+=`'s own
   `lhu` is commoned with the `lh` the subtraction already did and the
   target's second load disappears -- the two live in different arms of the
   same statement, so nothing later re-separates them. Read into `u` right
   after the call, it lands in the `lh`'s load-delay slot exactly as the
   target has it. Worth 3 words.

   (4) EACH ARM OF THE 1.5x/1x CHOICE CARRIES ITS OWN DIVISION. With the
   `/ (D_800E3D68 + 1)` factored out of the ternary, gcc materialises
   D_800E3D68 and the `+ 1` ABOVE the arms and the target's second
   `lbu %lo(D_800E3D68)` and its in-arm `mult` both vanish: 284 words
   against 289. With the division inside both arms, cross-jumping puts the
   shared `mflo`/`addiu 1`/`div` tail back at .L800B40EC by itself.

   `D_800E3D68 / 2` is a bare `srl` and not the signed bias sequence because
   c-typeck's shorten path does a `u8 / small-constant` division in the
   UNSIGNED type; `D_800E3D68 + 1` is a plain int add, which is why the
   divide beside it is a signed `div`.

   The second parameter is never read here; the only caller, func_800B348C,
   sets $a0/$a1/$a2, so the arity is three. */
void func_800B3D10(s16 a, s32 b, u8 *c)
{
    s32 t;
    s32 u;

    switch (func_800A9400(a))
    {
    case 1:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 2:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 3:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 4;
        break;
    case 7:
        *(s32 *)(c + 0x14) |= 1;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 6)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 5
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 8:
        *(s32 *)(c + 0x14) |= 2;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 7)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 6
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 9:
        *(s32 *)(c + 0x14) |= 4;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 8)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 7
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 13:
    case 40:
    case 43:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 14:
    case 41:
    case 44:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 16:
    case 19:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 17:
    case 20:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 22:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 23:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 31:
    case 34:
    case 37:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 32:
    case 35:
    case 38:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 15:
    case 18:
    case 21:
    case 24:
    case 33:
    case 36:
    case 39:
    case 42:
    case 45:
        *(s32 *)(c + 0x14) |= 4;
        break;
    case 46:
    case 47:
    case 48:
    case 49:
        break;
    }
}

/* func_800B4194's two rodata strings (defined by the INCLUDE_RODATA just after
   it) and the five engine strings its message chains use. */
extern u8 D_800892C4[];
extern u8 D_800892C8[];
extern u8 D_800E0BC7[];
extern u8 D_800E0BDC[];
extern u8 D_800E0BF3[];
extern u8 D_800E0C09[];
extern u8 D_800E0C1D[];

s32 func_800B4194(s16 a, u8 *p, u8 *c)
{
    void *m;
    void *h;
    u8 *d;
    u8 *q;
    s16 flag;
    s16 k;
    s16 t;
    s32 x;
    s32 y;

    k = func_800A9400(a);
    flag = p[0x28] + *(s16 *)(p + 0x64) <= 0;
    if (k != 0)
    {
        switch (k)
        {
        case 0x10:
        case 0x13:
            if (!flag)
            {
                if (func_800C8C1C(p, D_800E3D7C[0xA9] + 0x80,
                                  (D_800E3D7C[0xA9] >> 2) + 0x10 + (func_800A6D30() & 3)) != 0)
                {
                    m = func_800990FC();
                    h = func_80099194(D_800E0BC7, m);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800892C4, h);
                    func_80099290(h);
                    func_800A5720(m);
                }
            }
            break;
        case 0x12:
        case 0x15:
            if (!flag && (*(s32 *)(p + 0x1C) & 0x20) == 0)
            {
                if (func_800C8980(p, D_800E3D7C[0xA9] + 0x80,
                                  (D_800E3D7C[0xA9] >> 2) + 0x2 + (func_800A6D30() & 3)) != 0)
                {
                    m = func_800990FC();
                    h = func_80099194(D_800E0BDC, m);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800892C8, h);
                    func_80099290(h);
                    func_800A5720(m);
                }
            }
            break;
        }
    }
    d = *(u8 **)(c + 0x4C);
    if (d == NULL)
    {
        return 0;
    }
    if (d[1] != 0x10)
    {
        return 0;
    }
    switch (d[0])
    {
    case 3:
        t = -(*(s16 *)(p + 0x64) >> 3);
        if (t != 0)
        {
            *(s16 *)(c + 0x64) = *(u16 *)(c + 0x64) + t;
            func_800B4C7C(0x10, c, t, 1);
        }
        break;
    case 4:
        if (!flag && (*(s32 *)(p + 0x1C) & 0x20) == 0 && func_800C8980(p, 8, 8) != 0)
        {
            func_800A56E0(0x700);
            m = func_800990FC();
            h = func_80099194(D_800E0BF3, m);
            h = func_80099734(p, h);
            h = func_80099194(D_800892C4, h);
            func_80099290(h);
            func_800A5720(m);
        }
        break;
    case 5:
        if (flag)
        {
            q = p + 0x48;
            if (q[1] == 0 && (func_800A6D30() & 3) == 0)
            {
                q[1] = 0xE;
                if (D_80012090 != 2)
                {
                    if (D_8008146C < 0xC)
                    {
                        x = 0x10;
                        y = 0x18;
                        q[0] = 1;
                    }
                    else if (D_8008146C < 0x16)
                    {
                        x = 0x50;
                        y = 0x78;
                        q[0] = 2;
                    }
                    else
                    {
                        goto three;
                    }
                    q[2] = func_800A6DA4(x, y);
                    q[3] = 0;
                    break;
                }
            three:
                p[0x48] = 3;
                q[2] = func_800A6DA4(0x10, 0x18);
                q[3] = 1;
            }
        }
        break;
    case 9:
        if (!flag && (s16)func_80042900((s8 *)p, 6) == 0 && (func_800A6D30() & 7) == 0 &&
            func_800A48F0((s8 *)p, 6, 0x10) >= 0)
        {
            func_800A56E0(0x700);
            m = func_800990FC();
            h = func_80099194(D_800E0C09, m);
            h = func_80099734(p, h);
            h = func_80099194(D_800E0C1D, h);
            func_80099290(h);
            func_800A5720(m);
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B348C", D_800892C4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B348C", D_800892C8);
