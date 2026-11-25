#include "common.h"

extern u8 D_800DCF5A;
extern s32 D_8008346C;
extern s8 D_80012D52;
extern s8 D_800E3DA0[];
extern u8 D_800E3DB0[];
extern s16 D_80083350;
extern u8 D_8001F660[];
extern u8 D_8001F6A0;
extern u8 D_8001F6A4[];
extern u8 D_8001F6E8[];
extern s16 D_8001F668;
extern u8 D_8001F6A8[];
typedef struct UnkDungeonSlot
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkDungeonSlot;
extern UnkDungeonSlot D_800E3548_s[] __asm__("D_800E3548");
extern s32 D_800E3548_w[] __asm__("D_800E3548");
extern s32 D_800E3648_w[] __asm__("D_800E3648");
typedef struct UnkDungeonSpotA
{
    u8 unk_00;
    u8 unk_01;
    u8 pad_02[0x2];
    u16 unk_04;
    u8 pad_06[0x6];
} UnkDungeonSpotA;
extern UnkDungeonSpotA D_800E36C8[];
typedef struct UnkDungeonCell
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
} UnkDungeonCell;
extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
typedef struct UnkDngLo16Grid
{
    UnkDungeonCell *unk_00;
    u8 pad_04[0x10];
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    s16 unk_1E;
} UnkDngLo16Grid;
typedef struct UnkDngLo16Rec8
{
    s32 unk_00;
    s16 unk_04;
    s16 unk_06;
} UnkDngLo16Rec8;
extern UnkDngLo16Rec8 D_80083CE8_s[4] __asm__("D_80083CE8");
typedef struct UnkDngRoom2970
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u8 pad_08[0x2];
    s16 unk_0A;
    u16 unk_0C;
    s16 unk_0E;
    void *unk_10;
} UnkDngRoom2970;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
typedef struct UnkDngMark2C40
{
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
} UnkDngMark2C40;
extern UnkDngMark2C40 D_800E2C40_s[][0x10] __asm__("D_800E2C40");
typedef struct UnkDungeonMark
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    s16 unk_06;
} UnkDungeonMark;
extern UnkDungeonMark D_800E3CD8[];
s32 func_80033BC0();
s16 func_800A6928(u8, u8);
void func_8009A21C(s32, s32, s32);
void func_lo16_80016BF0(s32, s32, s16, s16);
void func_lo16_80017480(s16 x, s16 y, s16 w, s16 h, u16 mask);
s16 func_lo16_80017BEC(s16);
void func_lo16_8001D4AC(s16 *, s16);
void func_lo16_8001D5D8(s16 *, s16);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001607C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800161D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001625C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800163F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800165B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80016BF0);

/* 21 words, 0 differing.  TWO POINTER LOCALS, and this is the unit's central
   addressing fact: `&D_80083160' into `b' and then `g = b + 0x1DC' gives the
   target's THREE-insn shape (`lui %hi' / `addiu %lo' / `addiu 0x1DC').  Any
   spelling that names the offset inside the address expression --
   `(u8 *)&D_80083160.unk_1DC', `(u8 *)&D_80083160 + 0x1DC' -- folds 0x1DC into
   the R_MIPS_LO16 addend and is 20 words / 18 differing.  The `sll 16'/`sra 16'
   pairs on the two `1 << s' values are func_lo16_80016BF0's `s16' parameters,
   not casts in this body. */
void func_lo16_80016E6C(void)
{
    u8 *b;
    u8 *g;

    b = (u8 *)&D_80083160;
    g = b + 0x1DC;
    func_lo16_80016BF0(0, 0, 1 << *(s16 *)(g + 0x14), 1 << *(s16 *)(g + 0x16));
}

/* 44 words, 0 differing.  First compile, as func_lo16_80016F70 plus
   `*(s32 *)(p + 4) = 0' in each loop body -- which moves the giv's bias from
   +3 to +4, since the bias follows the LAST subscript in the body.  Every
   spelling fact priced on func_lo16_80016F70 carries here unmeasured. */
void func_lo16_80016EC0(void)
{
    u8 *p;
    s16 i;
    s16 n;

    p = D_800E3DB0;
    n = i = 0;
    do
    {
        i++;
        p[0] = 1;
        p[1] = 0;
        p[2] = 0;
        p[3] = n;
        n++;
        *(s32 *)(p + 4) = 0;
        p += 8;
    } while (i < 4);
    i = 0;
    do
    {
        i++;
        p[0] = 3;
        p[1] = 0;
        p[2] = 0;
        p[3] = n;
        n++;
        *(s32 *)(p + 4) = 0;
        p += 8;
    } while (i < 2);
    p[0] = 2;
    p[1] = 0;
    p[2] = 0;
    p[3] = n;
    *(s32 *)(p + 4) = 0;
}

/* 42 words, 0 differing.  Three measured facts, and the giv one is the
   expensive one:
   - the store order is p[0], p[1], p[2], p[3].  loop.c's combine_givs takes
     the giv at the HEAD of bl->giv -- the LAST one discovered -- as the
     representative, so the giv's bias is the offset of the last subscript in
     the body.  Target bias is +3; with p[3] stored second the bias is +2 and
     it is 10 differing;
   - `n' is `s16', and `p[3] = n; n++;' SPLIT.  `p[3] = n++' with `s16 n' puts
     the increment ahead of the stores with a copy (44 words / 43), and with
     `s32 n' the init copy below folds;
   - `n = i = 0' and not `i = 0; n = 0' or `n = 0; i = 0': the target's second
     zero is `addu $a1, $a2, $zero', a copy of the counter.  With `s32 n' the
     s16 -> s32 conversion insn is constant-folded by cse and the copy becomes
     `addu $a1, $zero, $zero' -- 1 differing. */
void func_lo16_80016F70(void)
{
    u8 *p;
    s16 i;
    s16 n;

    p = D_800E3DB0;
    n = i = 0;
    do
    {
        i++;
        p[0] = 1;
        p[1] = 0;
        p[2] = 0;
        p[3] = n;
        n++;
        p += 8;
    } while (i < 4);
    i = 0;
    do
    {
        i++;
        p[0] = 3;
        p[1] = 0;
        p[2] = 0;
        p[3] = n;
        n++;
        p += 8;
    } while (i < 2);
    p[0] = 2;
    p[1] = 0;
    p[2] = 0;
    p[3] = n;
    *(s32 *)(p + 4) = 0;
}

/* 9 words, 0 differing.  THE SOURCE LOOP COUNTS DOWN: the array ref stays an
   `addu $v0, $v1, $a0' per iteration, which is what an unreduced subscript on
   the counter looks like.  Written ascending (`for (i = 0; i < 7; i++)')
   loop.c reverses it AND strength-reduces the subscript into a decrementing
   pointer -- 10 words, 8 differing. */
void func_lo16_80017018(void)
{
    s32 i;

    for (i = 6; i >= 0; i--)
    {
        D_800E3DA0[i] = 0;
    }
}

/* 11 words, 0 differing.  Plain subscripts of a declared array reproduce the
   split-at-index-0 shape by themselves: element 0's word goes symbolic off the
   `lui' (`sw $zero, %lo(D_80083CE8)($v0)') and every nonzero offset goes
   through the `addiu %lo' base.  No device. */
void func_lo16_8001703C(void)
{
    D_80083CE8_s[0].unk_00 = 0;
    D_80083CE8_s[0].unk_04 = 0;
    D_80083CE8_s[1].unk_00 = 0;
    D_80083CE8_s[1].unk_04 = 0;
    D_80083CE8_s[2].unk_00 = 0;
    D_80083CE8_s[2].unk_04 = 0;
    D_80083CE8_s[3].unk_00 = 0;
    D_80083CE8_s[3].unk_04 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017068);

/* 3 words, 0 differing. */
void func_lo16_80017294(void)
{
    D_800DCF5A = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800172A0);

/* 13 words, 0 differing.  The counter decrement is the FIRST statement of the
   body, not the `for' increment: as the increment it is 13 words / 5 differing,
   every row being `addiu $a0, $a0, -0x1' after the three stores instead of
   before them.  The three `sh's come out on TWO base registers (the cell and
   the cell + 2) because loop.c reduces the three subscripts into two givs; the
   flat three-halfword UnkDungeonCell gives that with no device. */
void func_lo16_8001744C(void)
{
    UnkDungeonCell *p;
    s32 i;

    p = D_800EA000_s;
    i = 0x1000;
    do
    {
        i--;
        p->unk_00 = 0;
        p->unk_04 = 0;
        p->unk_02 = 0;
        p++;
    } while (i > 0);
}

/* 44 words, 0 differing.  func_lo16_80017668's loop with bounds instead of
   counts.  `i < y + h' must be written INLINE: hoisting it into a local
   (`e = y + h; i < e') loses the target's `addu $t2, $v1, $zero' copy of the
   bound -- 43 words / 34 differing.  The cell base is an `s32' for the same
   reason as in func_lo16_80019A84, and it ALSO keeps the `lui'/`addiu' pair
   ahead of the outer gate, which is why the `beqz' delay slot is a `nop'; left
   to loop.c the pair lands in the delay slot and the body is 42 / 40.
   Subscript `(i << g->unk_14) + j' as in func_lo16_80017668. */
void func_lo16_80017480(s16 x, s16 y, s16 w, s16 h, u16 mask)
{
    UnkDngLo16Grid *g;
    s32 c;
    s32 i;
    s32 j;

    g = (UnkDngLo16Grid *)D_8008333C;
    c = (s32)D_800EA000_s;
    for (i = y; i < y + h; i++)
    {
        for (j = x; j < x + w; j++)
        {
            ((UnkDungeonCell *)((j + (i << g->unk_14)) * 6 + c))->unk_04 &= mask;
        }
    }
}

/* 12 words, 0 differing. */
void func_lo16_80017530(void)
{
    func_lo16_80017480(0, 0, 0x40, 0x40, 0);
}

/* 9 words, 0 differing.  Counter down, cursor UP, so the two are separate
   induction variables and the source cannot be an ascending subscript:
   `for (i = 0; i < 0x24; i++) D_800E2970_s[i].unk_0C = 0;' is reversed by
   loop.c into a bgez countdown with a DECREMENTING pointer -- 10 words, 10
   differing. */
void func_lo16_80017560(void)
{
    UnkDngRoom2970 *p;
    s32 i;

    p = D_800E2970_s;
    for (i = 0x24; i > 0; i--)
    {
        p->unk_0C = 0;
        p++;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017584);

/* 33 words, 0 differing.  UnkDngLo16Grid, as in func_lo16_80019A84: a `u8 *'
   grid pointer lets loop.c hoist the shift read out of the inner loop, 33
   words / 12 differing.  The subscript is `(y << g->unk_14) + j' -- the
   shift FIRST; `j + (y << ...)' is 1 differing, the one `addu' with its
   operands exchanged.  func_lo16_80017480 is this body with a bound-tested
   loop and `&=' for `=', and there the subscript is the other way round. */
void func_lo16_80017668(s16 x, s16 y, s16 w, s16 h, u16 v)
{
    UnkDngLo16Grid *g;
    s32 i;
    s32 j;

    g = (UnkDngLo16Grid *)D_8008333C;
    for (; h > 0; h--)
    {
        for (i = w, j = x; i > 0; i--, j++)
        {
            D_800EA000_s[(y << g->unk_14) + j].unk_04 = v;
        }
        y++;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800176EC);

/* 12 words, 0 differing.  Descending subscript, as in func_lo16_80017018 --
   ascending is 12 words / 9 differing, every row being the reversal's
   forward-walking pointer against the target's backward one. */
void func_lo16_800177A8(void)
{
    s32 i;

    for (i = 0x23; i >= 0; i--)
    {
        D_800E2970_s[i].unk_0A = 0;
        D_800E2970_s[i].unk_0E = 0;
        D_800E2970_s[i].unk_10 = 0;
    }
}

/* 29 words, 0 differing.  Three facts, each measured:
   - the room base is a pointer local assigned BEFORE the subscript, which is
     what puts `lui'/`addiu %lo(D_800E2970)' ahead of the index arithmetic;
   - `s16 n = p->unk_0E' is read ONCE and reused for both the subscript and the
     bump -- reading the field twice is `lhu' + `lh' and 29 words / 20
     differing;
   - the mark address associates as (n * 4 + r * 0x40) + base.  `&D_800E2C40_s[r][n]'
     reassociates to (n * 4 + base) + r * 0x40 (5 differing) and the flat
     `&D_800E2C40_s[r * 0x10 + n]' is 30 words / 23, so the integer form is
     written out. */
void func_lo16_800177D8(s16 r, u8 a, u8 b, s16 c)
{
    UnkDngRoom2970 *base;
    UnkDngRoom2970 *p;
    UnkDngMark2C40 *m;
    s16 n;

    base = D_800E2970_s;
    p = &base[r];
    if (p->unk_0A != 0)
    {
        n = p->unk_0E;
        p->unk_10 = D_800E2C40_s[r];
        m = (UnkDngMark2C40 *)(n * 4 + (r << 6) + (s32)D_800E2C40_s);
        p->unk_0E = n + 1;
        m->unk_02 = c;
        m->unk_00 = a;
        m->unk_01 = b;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001784C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017BEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017EBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017F88);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001816C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018304);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018464);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018A70);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018E80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800194C4);

/* 15 words, 0 differing.  0x800121E8 arrives as `lui'/`ori', which is the
   constant-address spelling -- it is NOT a declared symbol, so it is written
   as the integer.  D_80012D52 is, and is `s8' (`lb' straight into a `bltz'). */
void func_lo16_80019648(void)
{
    u8 *p;

    if (D_80012D52 >= 0)
    {
        p = (u8 *)(D_80012D52 * 4 + 0x800121E8);
        if (*p == 2)
        {
            *p = 0x39;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80019684);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80019894);

/* 43 words, 0 differing.  The D_800E3CD8 base is a pointer local assigned
   before the subscript, as in func_lo16_800177D8 -- `&D_800E3CD8[r]' off the
   bare symbol puts the `sll'/`sra' ahead of the `lui'/`addiu' and is 43 words
   / 11 differing.  `(s16)r * 8' is the `sll 16'/`sra 13' pair.  func_80033BC0
   is `void' where it is defined, so it needs the unit-local alias form. */
s32 func_lo16_800199C8(s16 r)
{
    UnkDungeonMark *base;
    UnkDungeonMark *m;

    if (D_80012090 != 0)
    {
        return 0;
    }
    if (D_8008146C != 2)
    {
        return 0;
    }
    if (func_80033BC0(0xA2) == 0)
    {
        return 0;
    }
    if (func_800A6928(0xC, 8) == 0)
    {
        return 0;
    }
    base = D_800E3CD8;
    m = &base[r];
    m->unk_00 = 4;
    m->unk_02 = 8;
    m->unk_04 = 8;
    m->unk_06 = 0;
    func_8009A21C(8, 8, 0x20);
    return 1;
}

/* 4 words, 0 differing. */
void func_lo16_80019A74(void)
{
    D_8008346C = 1;
}

/* 29 words, 0 differing.  The grid descriptor must be a STRUCT pointer, not a
   `u8 *': `*(s16 *)(g + 0x14)' is a non-struct MEM that loop.c's invariant_p
   can disambiguate from the struct store, so the shift read is HOISTED out of
   the inner loop -- 27 words / 26 differing.  Reaching it as `g->unk_14'
   through UnkDngLo16Grid sets MEM_IN_STRUCT_P on both and the read stays in
   the loop.  The cell base is an `s32' so that the final `addu' is
   (index * 6) + base; `&D_800EA000_s[idx]' off a UnkDungeonCell * local puts
   the base first (1 differing) and off the bare symbol loop.c hoists it after
   the `y = 1' init (3 differing). */
void func_lo16_80019A84(s16 v)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *p;
    s32 c;
    s32 x;
    s32 y;

    g = (UnkDngLo16Grid *)D_8008333C;
    c = (s32)D_800EA000_s;
    for (y = 1; y < 0x3F; y++)
    {
        for (x = 1; x < 0x3F; x++)
        {
            p = (UnkDungeonCell *)((x + (y << g->unk_14)) * 6 + c);
            if (p->unk_00 == 0)
            {
                p->unk_02 = v;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80019AF8);

/* 22 words, 0 differing.  D_8001F660 is read as a WORD and as a BYTE off ONE
   `lui', so it is ONE declaration reached through a cast and not two
   `extern's: declaring it twice under one assembler name is two symbols and a
   second `lui' -- 25 words / 18 differing.  The D_8001F6A4 clear comes FIRST
   (its `lui' is first in the target); with the count read first the two `lui's
   are exchanged, 22 words / 2 differing.  The `q = &D_8001F6E8[i]' cursor is
   what keeps +0 and +4 as displacements off ONE giv -- `D_8001F6E8[i]' and
   `D_8001F6E8[i + 4]' are two givs and two `addu's. */
void func_lo16_8001A710(void)
{
    u8 *q;
    s32 n;
    s32 i;

    D_8001F6A4[3] = 0;
    D_8001F6A4[2] = 0;
    D_8001F6A4[1] = 0;
    D_8001F6A4[0] = 0;
    n = *(s32 *)D_8001F660;
    D_8001F6A0 = D_8001F660[0];
    for (i = n - 1; i >= 0; i--)
    {
        q = &D_8001F6E8[i];
        q[0] = 0;
        q[4] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001A768);

/* 33 words, 0 differing.  `q = &D_8001F6A8[i * 2]' then `q[0]'/`q[1]':
   subscripting the byte array directly (`D_8001F6A8[i * 2]',
   `D_8001F6A8[i * 2 + 1]') makes loop.c build a SECOND giv biased +1 and is 36
   words / 22 differing.  The gate is the four-way
   (a == x || a == y) && (b == x || b == y). */
s32 func_lo16_8001ABC8(s16 x, s16 y)
{
    u8 *q;
    s32 i;

    for (i = D_8001F668 - 1; i >= 0; i--)
    {
        q = &D_8001F6A8[i * 2];
        if ((q[0] == x || q[0] == y) && (q[1] == x || q[1] == y))
        {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001AC4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001C06C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001C5E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001CBD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001CCEC);

/* 12 words, 0 differing.  `b <= a' and not `a >= b': the sll/sra pair is
   emitted for the comparison's op0 FIRST, so the spelling decides whether $a1
   or $a0 is extended first -- `a >= b && c >= a' is 12 words / 2 differing,
   the two `sll's exchanged.  One `&&' expression and not an early `return 0',
   which is what puts the zero in the `bnez' delay slot and shares the `jr'. */
s32 func_lo16_8001CE14(s16 a, s16 b, s16 c)
{
    return b <= a && c >= a;
}

/* 31 words, 0 differing.  First compile.  Measured and FREE, so neither is the
   reason it lands: `while (func_lo16_80017BEC(x) == 0)' as a do/while tail
   rather than a head test, and the `s16 *' parameter spelled `p[0]..p[3]'
   rather than a struct.  func_lo16_80017BEC returns `s16', which is the
   `sll 16' with no `sra' before the `beqz'. */
void func_lo16_8001CE44(s16 *p, s16 x)
{
    do
    {
        func_lo16_8001D5D8(p, x);
        func_lo16_8001D4AC(p, x);
    } while (func_lo16_80017BEC(x) == 0);

    func_lo16_80017480(p[0], p[1], p[2], p[3], 0xF);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001CEC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001CFB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D0F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D328);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D4AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D5D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D9CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001DC34);

/* 16 words, 0 differing.  func_lo16_8001DD48 with the three fields bumped the
   other way.  The gate is UNSIGNED: `(u16)p[2] < 9' gives `lhu'/`sltiu', and
   without the cast it is `slti' -- 16 words / 2 differing.  Statement order
   00, 02, 04 as in func_lo16_8001DD48. */
void func_lo16_8001DD08(s16 *p, s16 b)
{
    if ((u16)p[2] < 9)
    {
        p[0]++;
        p[1] += 0x20;
        p[2]++;
    }
}

/* 15 words, 0 differing.  Statement order 00, 02, 04 -- the order the listing
   STORES in (00, 04, 02) is the scheduler's, and writing that order is 15
   words / 6 differing.  The parameter is `s16 *' rather than a struct: the
   second, unused parameter is in the prototype because func_lo16_8001D5D8
   passes one, and it costs nothing. */
void func_lo16_8001DD48(s16 *p, s16 b)
{
    if (p[2] > 0)
    {
        p[0]--;
        p[1] -= 0x20;
        p[2]--;
    }
}

/* 42 words, 0 differing.  Two facts:
   - the first cell goes through the same `p' the loop uses.  Reading it as
     `D_800EA000_s[...].unk_02' without the pointer local is one pseudo too
     many and costs a `move $t0, $a0' copy of the x cursor, with every later
     register displaced -- 42 words / 31 differing;
   - the subscript is `(y << D_80083350) + x', shift first; `x + (y << ...)' is
     2 differing, the two `addu's with their operands exchanged.
   The `(s16)' on the compared field is real: `p->unk_02 < best' on a `u16'
   field is unsigned and 40 words / 30 differing.  D_80083350 is 0x8008333C's
   +0x14 under its own symbol, which is why the shift read is `lui %hi'/`lh
   %lo' here and `g->unk_14' off a register in the three grid walkers. */
s16 func_lo16_8001DD84(s32 x, s32 y, s32 n, s16 dx, s16 dy)
{
    UnkDungeonCell *p;
    s16 best;

    p = &D_800EA000_s[(y << D_80083350) + x];
    best = p->unk_02;
    x += dx;
    y += dy;
    for (n--; n > 0; n--)
    {
        p = &D_800EA000_s[(y << D_80083350) + x];
        if ((s16)p->unk_02 < best)
        {
            best = p->unk_02;
        }
        x += dx;
        y += dy;
    }
    return best;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001DE2C);

void func_lo16_8001E108(void) {
}

/* 31 words, 0 differing.  First compile, off the landed func_800CDE40 in
   csrc/ovl_dungeon_800CABA8.c -- the same two-table walk with the i == n slot
   skipped and an early `return 1'.  The gate's operand order is load-bearing:
   `n != i' is 0 and `i != n' is 1 differing (`beq $a0, $a3' with the operands
   exchanged). */
s32 func_lo16_8001E110(s32 n, s16 x, s16 y)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        if (n != i && D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x &&
            D_800E36C8[i].unk_01 == y)
        {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001E18C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001E660);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001E824);

/* 10 words, 0 differing.  Here the ascending subscript IS right: with one
   `sw' and a 4-byte stride loop.c reverses the loop and reduces the subscript,
   which is exactly the target's `addiu $v0, $v0, -0x4' walk down from
   D_800E3548 + 0xFC.  Same body as func_lo16_8001F32C with 0x40 for 0x20. */
void func_lo16_8001E96C(void)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        D_800E3548_w[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001E994);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001EAA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001EC54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001EF0C);

/* 10 words, 0 differing.  func_lo16_8001E96C with 0x20 words of D_800E3648. */
void func_lo16_8001F32C(void)
{
    s32 i;

    for (i = 0; i < 0x20; i++)
    {
        D_800E3648_w[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001F354);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001F450);
