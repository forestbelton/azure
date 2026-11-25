#include "common.h"
extern u8 D_80077E00[];
extern u8 D_80077DE8[];
extern u32 D_8001F5B4[];

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
    u8 pad_06[0x1];
    u8 unk_07;
    u32 unk_08;
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
void func_lo16_80016BF0(s16 x, s16 y, s16 w, s16 h);
void func_lo16_80017480(s16 x, s16 y, s16 w, s16 h, u16 mask);
s16 func_lo16_80017BEC(s16);
void func_lo16_8001D4AC(s16 *p, s16 x);
void func_lo16_8001D5D8(s16 *, s16);

extern s16 D_8008146E;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80044BB0[];
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_800A5AF0(void);
s16 func_80053DA8(u16);
void func_800542BC(void);
void func_8003F320(void);
void func_8003F5E0(u32);
s16 func_8005405C(s16);
s32 func_800A6DA4(u32, u32);
extern u8 *D_80083478;
s16 func_8009A540(s32, s32, s32, s16);
s16 func_8009A350(s16, s16, s32, u16 *);
s32 func_800BCB04(s32, s32, s16);
void func_8003F6D4(s32, s32, s32 *, s32);
extern u8 D_8014A000[];
extern u8 D_800DDC9C[];
extern u8 D_lo16_8001F584;
s32 func_lo16_8001E660(u8 *a, u8 *b, s32 c, s32 d);
u16 func_800A6D30();

extern s32 D_8001F670;
void func_lo16_8001CFB8(s16 *, s16);
void func_lo16_8001D0F4(s16 *p, s16 b);
extern u16 D_80083460h[] __asm__("D_80083460");
extern s16 D_80081468h[] __asm__("D_80081468");
extern s32 D_800E3D70_w[] __asm__("D_800E3D70");
extern u16 D_800DCE68;
extern u8 D_800E3E40;
extern u32 D_800E296C;
extern s16 D_800E3CCC;
extern s16 D_800E3CCE;
extern u8 D_800DCF4F;
extern u8 *D_80080A90;
extern u8 D_800DCF58;
extern u8 D_800DCF5B;
extern u8 D_80082E6B;
extern u32 D_80012D60;
extern s8 D_800DCF4E;
extern u32 D_80080A80;
extern Unk41284 D_80080AE0;
extern Unk41284 D_80080AE8;
extern Unk41284 D_80080E18;
extern Unk41284 D_80080E20;
extern Unk41284 D_80080EE8;
extern Unk41284 D_80080E30;
u32 func_80048D00(void);
extern u8 D_8001F6F0;
extern u8 *D_800E3D7C;
s16 func_800A697C(u8, u8);
s32 func_lo16_8001EAA4(u8 *, u8 *);
extern s32 D_8001022C;
extern s32 D_80082EB8;
extern s16 D_8008347E;
extern s32 D_800E3D1C;
extern u8 D_800E3CD0;
void func_8004A918(void);
void func_800A9024(s32);
void func_80040AA0(s32);
void func_8009FAC4(void);
void func_lo16_80016EC0(void);
void func_lo16_80017018(void);
void func_lo16_80017068(void);
s16 func_800A4E2C(u8 *, u8 *);
void func_lo16_8001E108(s32 x, s32 y, UnkDungeonCell *c, s32 base, s32 k);
typedef struct UnkDungeonSpotB
{
    u8 pad_00[0x6];
    u8 unk_06;
    u8 unk_07;
    s32 unk_08;
    void *unk_0C;
    s16 unk_10;
    s16 unk_12;
    u16 unk_14;
    u8 pad_16[0x2];
} UnkDungeonSpotB;
extern UnkDungeonSpotB D_800E39C8[];
extern UnkDungeonSlot D_800E3648_s[] __asm__("D_800E3648");

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001607C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800161D8);

/* 102 words, 0 differing.  FIRST COMPILE.  ONE MEASURED FACT and it is the
   nine D_80083460 stores' ORDER: it is the order the LISTING stores in --
   0x08, 0x0A, 0x14, 0x0C, 0x10, 0x00, 0x02, 0x1C, 0x04 -- and sched1 does
   NOT transpose them.  Ascending (0x00 first) is 102 words / 10 aligned
   differences and the listing order REVERSED is 102 / 16.  That is worth
   recording because this unit's smaller clearers go the other way
   (func_lo16_8001DD48: "the order the listing STORES in is the scheduler's"):
   with NINE independent stores off one base the list scheduler leaves source
   order alone, with three it transposes two of them.
   D_80083460 is reached through ONE `u16 []' alias with casts for the two
   word stores, not a second `extern' -- the func_lo16_8001A710 rule -- and
   plain subscripts give the split-at-index-0 shape by themselves
   (`sh $v1, %lo(D_80083460)($a0)' off the `lui' with every other offset off
   the `addiu %lo' base), as in func_lo16_8001703C.  D_800E3D70 is the same
   shape.  D_800DCF4E is `s8' (`lb' into a `bnez'); D_80012D60 is `u32', which
   is what makes the clamp compare `sltu'.  func_80069EC8 is `bzero' out of
   splat/common_syms.txt and func_80041284 takes structs.h's `Unk41284 *'. */
void func_lo16_8001625C(void)
{
    bzero((u8 *)D_800E3548_s, 0x100);
    bzero((u8 *)D_800E36C8, 0x300);
    D_80083460h[4] = 0;
    D_80083460h[5] = 0;
    D_80083460h[10] = 0;
    *(s32 *)&D_80083460h[6] = 0;
    *(s32 *)&D_80083460h[8] = 0;
    D_80083460h[0] = 0x100;
    D_80083460h[1] = 0;
    D_80083460h[14] = 0;
    D_80083460h[2] = 0;
    D_800DCE68 = 0;
    D_800E3E40 = 0;
    D_800E296C = 0;
    D_800E3CCC = 0;
    D_800E3CCE = 0;
    D_800DCF4F = 0;
    D_80080A90 = D_8014A000;
    D_800E3D70_w[0] = 0;
    D_800E3D70_w[1] = 0;
    D_800E3D70_w[2] = 0;
    D_800DCF58 = 1;
    D_800DCF5B = 0;
    if (D_80082E6B != 5)
    {
        if (D_80081468h[2] >= 0x64)
        {
            D_80081468h[2] = 0x63;
        }
        if (D_80012D60 < D_80081468h[2])
        {
            D_80012D60 = D_80081468h[2];
        }
    }
    if (D_800DCF4E == 0)
    {
        func_80041284(&D_80080AE0);
        func_80041284(&D_80080AE8);
        func_80041284(&D_80080E18);
        func_80041284(&D_80080E20);
        func_80041284(&D_80080EE8);
        func_80041284(&D_80080E30);
        D_80080A80 = func_80048D00();
        D_800DCF4E = 1;
    }
}

/* 113 words, 0 differing.  THE STARTUP RESET.  FOUR MEASURED FACTS, all
   about how the 0x8001xxxx out-of-image globals are addressed, and they are
   NOT all the same:
   - the three STORES and two LOADS in the second D_80013714 gate go through
     ONE POINTER LOCAL `b = (u8 *)0x80010000', materialised just before that
     gate.  cc1psx cannot store to a bare constant address off a shared base:
     `*(s32 *)0x80013628 = ...' comes out as the gas macro
     `lui $at, 0x8001' / `sw $v0, 0x3628($at)' per store and the body is 122
     words / 22 differing.  A load CAN (`lui $reg' / `lX off($reg)'), which is
     why the two reads in that block look the same either way.
   - `b' must be assigned THERE and not at the top of the function: assigned
     at function scope it is one callee-saved register for the whole body,
     the early D_80013714 read goes through it too, and the body is 110 words
     / 15 differing with three of the target's `lui's missing.
   - `*(u16 *)0x80010234' is the CONSTANT ADDRESS and `D_80013714' /
     `D_8008146C' / `D_800DCF4E' are the declared SYMBOLS.  The discriminator
     is the one csrc/ovl_dungeon_800A4B88.c's func_800A9230 records and it is
     exactly 2 aligned differences: a declared symbol gives
     `lui $v0' / `lhu $v1, %lo($v0)' -- two registers -- where a constant
     address gives `lui $v1' / `lhu $v1, 0x234($v1)', the same register
     twice.  Read the two registers in the listing, not the symbol name.
   - the last `|= 5' is `*(u16 *)(b + 0x3714)', through `b' and not through
     the symbol; the symbol spelling costs the extra `lui' and takes the
     body to 114 words.
   `func_80040AA0(0x11)' is called in BOTH arms of the D_80082E60.unk_0B
   test, which is why `addiu $a0, $zero, 0x11' sits in the gate's own delay
   slot; one call before the `if' would be a single `jal'.  The 0x12 test is
   UNSIGNED (`sltiu') because unk_0B is `u8', and unk_0B is read TWICE -- the
   test and `n = unk_0B - 0x12'.  func_80069EC8/func_80069EF8 are
   `bzero'/`rand' out of splat/common_syms.txt. */
void func_lo16_800163F4(void)
{
    u8 *b;
    s32 buf[2];
    s32 n;

    func_lo16_80016EC0();
    func_lo16_80017018();
    bzero((u8 *)D_800E3E48, 0xAF0);
    D_80082EB8 = 0;
    D_8008347E = 0;
    D_80080A88 = 0;
    if ((D_80013714 & 3) == 0)
    {
        D_8008146C = 1;
        D_8001022C = D_8001022C + 1;
        func_lo16_80017068();
        func_8004A918();
    }
    else
    {
        D_8008146C = *(u16 *)0x80010234;
    }
    if (D_800DCF4E == 0)
    {
        D_80080A80 = func_80048D00();
        D_800DCF4E = 1;
    }
    b = (u8 *)0x80010000;
    if ((*(u16 *)(b + 0x3714) & 3) == 0)
    {
        *(s32 *)(b + 0x3628) = rand();
        *(s16 *)(b + 0x3624) = rand() & 0xFF;
        *(s16 *)(b + 0x209C) = rand() & 0xFF;
        func_800A9024(*(s32 *)(b + 0x2090));
    }
    func_8004AB7C();
    if (D_80082E60.unk_0B >= 0x12)
    {
        n = D_80082E60.unk_0B - 0x12;
        func_80040AA0(0x11);
        func_8003F6D4(0xC, 0x80010000, buf, n * 12 + 0x399D);
        func_8003E4FC(6, (u8 *)buf, 0);
        func_8003F320();
        D_800E3D1C = 0x384;
        *(u16 *)(b + 0x3714) |= 5;
    }
    else
    {
        func_80040AA0(0x11);
        D_800E3D1C = 0;
    }
    func_8009FAC4();
    func_800499BC();
    D_800E3CD0 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_800165B8);

/* 159 words, 0 differing.  The map-reveal pass: for every cell in the
   rectangle, look its tile id up in the byte table at g + 0x10 and OR the
   reveal bits into the cell and (for the 0x2 tile class) its four
   neighbours.  FOUR MEASURED FACTS:
   - `s32 k' for the tile id, NOT `u16'.  As a `u16' the body is 159 words /
     2 aligned differences and the two rows are `tbl' and `k' exchanged
     between $t4 and $a2 in both `addu $v1, $t4, $a2' -- a clean two-register
     exchange with nothing moved, and FOUR declaration permutations all give
     the same 2, so the brief's rule holds: declaration order is not the
     lever and the width is.
   - `y + i' MUST BE WRITTEN INLINE at all three sites.  Hoisted into an
     `ry' local before the inner loop the body is 159 / 9: the three
     parameter sign extensions come out in the order y, w, x instead of the
     target's w, y, x, the inner loop's guard moves a word and reorg
     duplicates `j = 0' into the outer back branch where the target has a
     `nop'.  Putting the same assignment INSIDE the inner loop also reaches
     159 / 2, so what matters is that loop.c, not the source, decides where
     the sum lives.
   - THERE IS NO `if (h <= 0) return;'.  The `for (i = 0; i < h; i++)' guard
     IS that test, and gcc tests it as `sll $v0, $a3, 16' / `blez $v0' with
     no `sra' -- the shifted value's sign is enough.  Written as an explicit
     early return the body is 161 words / 27 with the redundant `sra' and a
     `nop'.
   - THE NULL TEST READS THE MEMORY AND `tbl' IS ASSIGNED AFTERWARDS.  With
     `tbl = *(u8 **)((u8 *)g + 0x10); if (tbl == NULL) return;' the body is
     158 words -- ONE SHORT -- because the loaded pseudo serves both the test
     and the loop and the target's `addu $t4, $v1, $zero' copy never exists.
     Testing the MEM first and assigning `tbl' from the same expression after
     makes cse turn the second read into exactly that copy.  This is the
     brief's parameter-dies-at-its-copy rule, here on a global read.
   `b' and `g' are the unit's two pointer locals (`b = (u8 *)&D_80083160;
   g = b + 0x1DC'), and the CELL BASE is read as `*(UnkDungeonCell **)
   (b + 0x1DC)' through `b' -- the same address as `g->unk_00', spelled the
   other way, which is what gives the target TWO base registers for one
   struct.  All four parameters are `s16'; the declaration this unit carried
   said `(s32, s32, s16, s16)' and both callers pass 0 for the first two, so
   widening it to four `s16' moves nothing -- func_lo16_80016E6C and
   func_lo16_80017530 re-verified at 0 differing. */
void func_lo16_80016BF0(s16 x, s16 y, s16 w, s16 h)
{
    u8 *b;
    UnkDngLo16Grid *g;
    u8 *tbl;
    UnkDungeonCell *cb;
    UnkDungeonCell *c;
    s32 i;
    s32 j;
    s32 k;

    b = (u8 *)&D_80083160;
    g = (UnkDngLo16Grid *)(b + 0x1DC);
    if (*(u8 **)((u8 *)g + 0x10) == NULL)
    {
        return;
    }
    tbl = *(u8 **)((u8 *)g + 0x10);
    cb = *(UnkDungeonCell **)(b + 0x1DC);
    for (i = 0; i < h; i++)
    {
        for (j = 0; j < w; j++)
        {
            c = &cb[((y + i) << g->unk_14) + x + j];
            k = c->unk_00;
            if ((tbl[k] & 1) != 0)
            {
                c->unk_04 |= 1;
            }
            if ((tbl[k] & 4) != 0)
            {
                c->unk_04 |= 4;
            }
            if ((tbl[k] & 8) != 0)
            {
                c->unk_04 |= 8;
            }
            if ((tbl[k] & 0x20) != 0)
            {
                c->unk_04 |= 0x400;
            }
            if ((tbl[k] & 2) != 0)
            {
                if (j + x > 0)
                {
                    c[-1].unk_04 |= 1;
                }
                if (j + x < (1 << g->unk_14) - 1)
                {
                    c[1].unk_04 |= 1;
                }
                if (y + i > 0)
                {
                    c[-(1 << g->unk_14)].unk_04 |= 1;
                }
                if (y + i < (1 << g->unk_16) - 1)
                {
                    c[1 << g->unk_14].unk_04 |= 1;
                }
            }
            if ((tbl[k] & 0x80) != 0)
            {
                c->unk_04 |= 0x8000;
            }
            if ((tbl[k] & 0x40) != 0)
            {
                c->unk_04 |= 0x40;
            }
        }
    }
}

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

/* 57 words, 0 differing.  TWO FACTS, and the second is a device-shaped one.
   - `cy = y' into an `s32' LOCAL, not the `s16' parameter.  The target
     sign-extends y IN PLACE (`sll $a1, $a1, 16' / `sra $a1, $a1, 16') and then
     increments and shifts that register with no further extension; using the
     parameter directly keeps the raw y alive in a second register and
     re-extends it inside the loop -- 60 words / 28 aligned differences, of
     which the extra `move $t2, $a1' and the in-loop `sll'/`sra' pair are three.
     x and w stay as the parameters, where the target keeps `x << 16' in $a0
     and re-`sra's it per outer iteration.
   - gates 2, 3 and 4 are ONE `if' with two `||', and `g' is materialised
     inside the second term with a COMMA EXPRESSION.  That is ugly and it is
     the only spelling that puts `lui %hi(D_8008333C)' after the `blez $t0'
     so reorg can take it into that branch's delay slot: the same three gates
     written separately with `g = ...' before them is 58 words with a `nop'
     in the slot (3 aligned rows), `g' before the y gate is 57 words / 13, the
     cast inlined at all three use sites is 58 / 17, and gates 3||4 combined
     with gate 2 separate is 58 / 53.  Call it macro residue rather than a
     device: a comma in a condition is what a bounds-check macro collapses to.
   The subscript is `(cy << g->unk_14) + cx', shift first, as in
   func_lo16_80017668.  The return is `s16' -- `return v' on a `u16' local is
   the `sll 16'/`sra 16' pair at .Llo16_800175F4. */
s16 func_lo16_80017584(s16 x, s16 y, s16 w, s16 h)
{
    UnkDngLo16Grid *g;
    u16 v;
    s32 i;
    s32 j;
    s32 cx;
    s32 cy;

    cy = y;
    if (cy <= 0)
    {
        return -1;
    }
    if (x <= 0 || (g = (UnkDngLo16Grid *)D_8008333C, cy + h >= (1 << g->unk_16)) ||
        x + w >= (1 << g->unk_14))
    {
        return -1;
    }
    for (i = h; i > 0; i--)
    {
        for (j = w, cx = x; j > 0; j--, cx++)
        {
            v = D_800EA000_s[(cy << g->unk_14) + cx].unk_04;
            if (v != 0)
            {
                return v;
            }
        }
        cy++;
    }
    return 0;
}

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

/* 47 words, 0 differing.  FIRST COMPILE, so every spelling in it is a choice
   and not a measurement; what follows is what the listing dictated, not what
   was priced.  The compaction loop keeps TWO cursors into D_800E2970 -- the
   source `p' and the destination `q = p' -- and `k = i' rather than `k = 0',
   because the target's second zero is `addu $t1, $a3, $zero', a copy of the
   counter, exactly as in func_lo16_80016F70; with `s32' on both, cse.c:1355
   prefers the register already holding 0 and `k = 0' gives the same copy, so
   this one is NOT MEASURABLE rather than free.  `*q = *p' is a 0x14-byte
   struct assignment and comes out as the target's five aligned word moves.
   The second loop is plain ascending `D_800E2970_s[i]' subscripts that loop.c
   strength-reduces into the +0x14 cursor; it is NOT reversed because `k' is a
   variable, which is what distinguishes it from func_lo16_800177A8. */
void func_lo16_800176EC(void)
{
    UnkDngRoom2970 *p;
    UnkDngRoom2970 *q;
    s32 i;
    s32 k;

    i = 0;
    k = i;
    p = D_800E2970_s;
    q = p;
    for (; i < 0x24; i++)
    {
        if (p->unk_0A != 0)
        {
            if (i != k)
            {
                *q = *p;
            }
            q++;
            k++;
        }
        p++;
    }
    for (i = k; i < 0x24; i++)
    {
        D_800E2970_s[i].unk_0A = 0;
        D_800E2970_s[i].unk_0E = 0;
        D_800E2970_s[i].unk_10 = 0;
    }
    D_8008146E = k;
}

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

/* 148 words, 0 differing.  The four-spot placement: draw a random free cell,
   reject it against both occupancy tables, stamp it into D_800E3CD8 and stop
   on a 1-in-64 roll.  THREE MEASURED FACTS:
   - D_800E3CD8 IS SUBSCRIPTED, NOT REACHED THROUGH A POINTER LOCAL.  With
     `m = &D_800E3CD8[i]' and `m->unk_00' the body is 148 words / 12 aligned
     differences: `m' and the second buf[1] copy exchange $a3 and $t0 and the
     `sll $s0, 3' moves two words early.  Four other spellings (the `m'
     assignment before the two reads, after them, declared first, declared
     last) all measure the same 12, so this is a PSEUDO-COUNT fault and not
     a priority one -- the same shape as func_lo16_8001D9CC's 52 rows in this
     unit, and the same cure.
   - buf[0] and buf[1] ARE READ INTO TWO SEPARATE PAIRS OF LOCALS: `bx'/`by'
     for the D_800E3CD8 stores and the func_8009A21C arguments, and
     `cx'/`cy' for the three cell-address computations.  One pair used for
     everything is 147 words / 21; reading buf directly at the store and call
     sites with one pair for the addresses is 152 / 31.  The target's four
     `lbu's say it plainly: `buf' is a stack array whose address escaped into
     func_800A4E2C, so every intervening store through a register invalidates
     cse's record of it and only a LOCAL carries a value across one.
   - the cell base is an `s32' (`(index * 6) + cb'), which is what puts the
     index first in the final `addu' -- func_lo16_80019A84's rule -- and the
     cell address is written out THREE times rather than through `c',
     because the target recomputes it after each store.
   The retry on a collision is a `goto' back to the top of the loop body;
   both scans' hit arms share it and jump2 does not need any help.  The
   stray `i++' between the two loops is real: the target's exit block
   increments before the clear loop's guard, so the fall-out at i == 4 goes
   to 5 and the clear loop is skipped, while a `break' lands on i + 1.
   D_80012090 is the CONSTANT ADDRESS here (`lui $v1' / `lw $v1, %lo($v1)',
   one register), as in func_lo16_8001E660 -- SO THIS BODY ADDRESSES A
   CONSTANT BASE and the masked verifier cannot see that displacement. */
void func_lo16_8001784C(void)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    u8 buf[2];
    s32 bx;
    s32 by;
    s32 cx;
    s32 cy;
    s32 cb;
    s32 i;
    s32 j;

    g = (UnkDngLo16Grid *)D_8008333C;
    cb = (s32)D_8008333C[0];
    if (*(s32 *)0x80012090 != 2 && D_8008146C == 0x28)
    {
        return;
    }
    for (i = 0; i < 4; i++)
    {
    retry:
        while ((s16)func_800A4E2C(&buf[0], &buf[1]) < 0)
        {
        }
        for (j = 0; j < 0x40; j++)
        {
            if (D_800E3548_s[j].unk_01 != 0 && D_800E36C8[j].unk_00 == buf[0] &&
                D_800E36C8[j].unk_01 == buf[1])
            {
                goto retry;
            }
        }
        for (j = 0; j < 0x20; j++)
        {
            if (D_800E3648_s[j].unk_01 != 0 && D_800E39C8[j].unk_06 == buf[0] &&
                D_800E39C8[j].unk_07 == buf[1])
            {
                goto retry;
            }
        }
        bx = buf[0];
        by = buf[1];
        D_800E3CD8[i].unk_00 = 2;
        D_800E3CD8[i].unk_02 = bx;
        D_800E3CD8[i].unk_04 = by;
        cx = buf[0];
        cy = buf[1];
        c = (UnkDungeonCell *)((cx + (cy << g->unk_14)) * 6 + cb);
        c->unk_02 -= 0x20;
        D_800E3CD8[i].unk_06 = ((UnkDungeonCell *)((cx + (cy << g->unk_14)) * 6 + cb))->unk_02;
        ((UnkDungeonCell *)((cx + (cy << g->unk_14)) * 6 + cb))->unk_00 = 1;
        func_8009A21C(bx, by, 0x20);
        if ((func_800A6D30() & 0x3F) != 0)
        {
            break;
        }
    }
    i++;
    for (; i < 4; i++)
    {
        D_800E3CD8[i].unk_00 = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017BEC);

/* 51 words, 0 differing.  THE IN-LOOP ROOM POINTER IS A SECOND `&base[r]'
   WRITTEN INSIDE THE LOOP, and that is the whole fight -- loop.c hoists the
   invariant address into the preheader, cse2 then finds `base + r * 0x14'
   already live in the entry block (the preheader is the blez's fall-through,
   so it is the SAME cse block) and rewrites it as `addu $t0, $v1, $zero'
   AFTER the gate.  Figures, all mid-fight and taken while the head was still
   wrong, so read them as directions: one pointer and no alias at all is 50
   words / 49; `q = p' before the loop puts the copy BEFORE the gate and makes
   the pre-gate reads go through the copy's register, 51 / 10; `q = p' as the
   first loop-body statement is coalesced away, 50 / 42.
   `base = D_800E2970_s; p = &base[r]' as in func_lo16_800177D8 -- off the bare
   symbol the lui/addiu lands after the index arithmetic, 51 / 17.
   The cell base is `D_8008333C[0]' SUBSCRIPTED OFF THE DECLARED ARRAY, which
   is what gives `lw $t2, %lo(D_8008333C)($t1)' symbolic off the lui while
   `g->unk_14' goes through the `addiu %lo' base -- the split-at-index-0 shape
   of func_lo16_8001703C, here with the two halves of one symbol in two
   registers at once.  `g->unk_00' puts that load at 0($a0) instead.
   The shift read is NOT hoisted out of either loop even though nothing in the
   loop stores: `g->unk_14' through UnkDngLo16Grid is MEM_IN_STRUCT_P and the
   target reads it once in the preheader, which the struct form gives. */
s16 func_lo16_80017EBC(s16 r)
{
    UnkDngLo16Grid *g;
    UnkDngRoom2970 *base;
    UnkDngRoom2970 *p;
    UnkDngRoom2970 *q;
    UnkDungeonCell *c;
    s16 n;
    s16 i;
    s16 j;
    s16 y;

    n = 0;
    base = D_800E2970_s;
    p = &base[r];
    g = (UnkDngLo16Grid *)D_8008333C;
    y = p->unk_02;
    for (i = p->unk_06; i > 0; i--)
    {
        q = &base[r];
        c = (UnkDungeonCell *)D_8008333C[0] + (y << g->unk_14) + q->unk_00;
        for (j = q->unk_04; j > 0; j--)
        {
            if ((c->unk_04 & 0x8500) == 0)
            {
                n++;
            }
            c++;
        }
        y++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80017F88);

/* 102 words, 0 differing.  func_lo16_80017EBC's room walk with a height
   probe in the inner loop.  ONE MEASURED FACT and it is the brief's
   fold_truthop precondition firing exactly as stated: the two cell-flag
   tests must be NESTED `if's.  Written
   `if ((v & 0x8400) == 0 && (v & 0x300) == 0x200)' fold_truthop merges them
   into a single `andi $v0, $v0, 0x8700' against 0x200 -- two BIT TESTS ON
   ONE LOCAL, which is the one case where it fires -- and the body comes out
   99 words / 12 aligned differences, three words SHORT.
   The room record is re-read off the BARE SYMBOL inside the outer loop
   (`D_800E2970_s[r].unk_00' etc.), not through a `base' local as in
   func_lo16_80017EBC: the `lui'/`addiu %lo(D_800E2970)' pair is inside the
   loop in this listing.  The cell pointer is TWO pointer additions --
   `g->unk_00 + (y << g->unk_14) + x', which is the target's two separate
   *6 multiplies; one subscript `[(y << g->unk_14) + x]' would be one.
   `u16 t' is what gives the `andi $a1, $s4, 0xFFFF' on the second argument
   where the first is masked 0xFFE0 in the source.  `y' is `s16' off the u16
   field, which is why its load is `lhu' and the three loop counters' are
   `lh' -- the same split func_lo16_80017EBC has. */
s32 func_lo16_8001816C(s16 r, s16 *ox, s16 *oy)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s16 y;
    s16 n;
    s16 x;
    s16 w;
    s16 h;
    u16 v;
    u16 t;

    g = (UnkDngLo16Grid *)D_8008333C;
    y = D_800E2970_s[r].unk_02;
    for (n = D_800E2970_s[r].unk_06; n > 0; n--)
    {
        x = D_800E2970_s[r].unk_00;
        c = g->unk_00 + (y << g->unk_14) + x;
        for (w = D_800E2970_s[r].unk_04; w > 0; w--)
        {
            v = c->unk_04;
            if ((v & 0x8400) == 0)
            {
                if ((v & 0x300) == 0x200)
                {
                    t = y * 64 + 0x20;
                    h = (s16)func_800BCB04((x * 64 + 0x20) & 0xFFE0, t, -0x400);
                    if (h < 0x200)
                    {
                        *ox = x;
                        *oy = y;
                        return h;
                    }
                }
            }
            x++;
            c++;
        }
        y++;
    }
    return 0x200;
}

/* 88 words, 0 differing.  THE WHOLE FIGHT WAS GATE SHAPE, and it is three
   facts about where a shared `return 0' comes from:
   - the first two gates are ONE `if' with `||'.  Written as two separate
     `if's the second one's `return 0' is not cross-joined with the others and
     costs a duplicated `j'/`move $v0, $zero' -- 87 words / 13 aligned rows.
   - the 0x200 gate must be an `if (h < 0x200) { ... }' WRAPPING the tail with
     the final `return 0' as its fall-through, not `if (h >= 0x200) return 0;'.
     The latter duplicates the return-0 block again: 87 / 11.
   - with that wrapping, `if (z >= h) return 2;' is right and
     `if (z < h) {...} return 2;' is 87 / 11.
   `nx' and `ny' are `s16' locals: that is what turns `(x + dx[d]) * 64' into
   the target's single `sll 16'/`sra 10' pair.  The `& 0xFFE0' is in the
   source -- a u16 parameter would have masked with 0xFFFF.  The step tables
   are read `(u16)D_8006CCD8[d]' because the listing uses `lhu' and the
   project declares them `s16 []'.  `(s16)' on func_800BCB04's result is real:
   its declaration returns `s32' and the listing sign-extends. */
s32 func_lo16_80018304(s16 x, s16 y, s16 z, s16 d)
{
    u16 buf[4];
    s16 nx;
    s16 ny;
    s16 h;

    if (func_8009A540((u16)d, x, y, z - 0x20) == 0 || func_8009A350(x, y, d, buf) == 0)
    {
        return 0;
    }
    nx = x + (u16)D_8006CCD8[d];
    ny = y + (u16)D_8006CCE8[d];
    if ((buf[0] & 0x8400) != 0)
    {
        return 0;
    }
    h = (s16)func_800BCB04((nx * 64 + 0x20) & 0xFFE0, (ny * 64 + 0x20) & 0xFFE0, z - 0x20);
    if (h < 0x200)
    {
        if (z >= h)
        {
            return 2;
        }
        if (h - z < 0x21)
        {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018464);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018A70);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_80018E80);

/* 97 words, 0 differing.  FIRST COMPILE.  The two 0x20-byte block copies are
   `memcpy' with `u8 *' pointers, which is where the lwl/lwr/swl/swr pairs come
   from -- the alignment comes off the pointer type, as docs/matching-levers.md
   records, and nothing else in the project's vocabulary produces an unaligned
   inline block move.  The two source spellings are deliberately different:
   the first copy names the bare symbol `D_8014A000' and the second goes
   through the `b' local, which is what gives the target TWO `addiu %lo'
   registers off ONE `lui' ($s0 at insn 5 and $a2 at insn 16).  Spelling both
   through `b' or both bare was not measured. */
void func_lo16_800194C4(s16 n)
{
    u8 *b;
    s32 buf[2];

    b = D_8014A000;
    func_8003F6D4(1, (s32)b, buf, n + 0x3F5E);
    func_8003E4FC(6, (u8 *)buf, 0);
    func_8003F320();
    memcpy(D_80083478, D_8014A000, 0x20);
    memcpy(D_800DDC9C, b + 0x20, 0x20);
    D_lo16_8001F584 = b[0x40];
}

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

/* 77 words, 0 differing.  ONE FACT and it is one character: the gate is
   `if (*q == i + 1) return 0;' -- the EQUAL case is the early exit.  Spelled
   `!=' the body is 77 words with 1 aligned difference, the `beq'/`bne' with
   the same operands, which is the whole residue and is exactly the brief's
   "check which arm is the then".
   `n = r->unk_00 - 1' is `s16' and both its uses -- `&D_80080AF0[n]' and
   `2 << n' -- need the sign extension, so cse emits `sra $s3' once and shares
   it; that is why the value survives in a callee-saved register across six
   calls.  func_80067014 and func_8005FE18 are DrawSync and VSync out of
   splat/common_syms.txt.  `func_80053DA8((2 << n) | 0x10)' gets its
   `andi $a0, $a0, 0xFFFF' free from the `u16' parameter in the project's
   existing declaration.  The loop is `while (func_8005405C(r->unk_00) != 1)'
   with the comparison constant hoisted into $s0. */
s32 func_lo16_80019894(s16 i)
{
    Unk8006E61C *r;
    u8 *q;
    s16 n;

    r = &D_8006E61C[i];
    n = r->unk_00 - 1;
    q = &D_80080AF0[n];
    if (*q == i + 1)
    {
        return 0;
    }
    func_80053DA8(0x72);
    func_800542BC();
    *q = i + 1;
    DrawSync(0);
    VSync(0);
    D_80081480 = D_8008148C;
    func_8003E4FC(6, r->unk_08, 0);
    func_8003E4FC(6, r->unk_04, 0);
    func_8003F320();
    func_8003F5E0(D_8008148C);
    func_80053DA8((2 << n) | 0x10);
    func_800542BC();
    while (func_8005405C(r->unk_00) != 1)
    {
    }
    return 1;
}

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

/* 74 words, 0 differing.  FIRST COMPILE.  A compare-chain `switch' on the
   direction with cases 0, 2, 4 and `default' -- three real labels plus a
   default, so it stays under gcc 2.7.2's case_values_threshold and emits the
   balanced tree the listing shows rather than a jump table (which this unit
   could not have held).  The arms are written ASCENDING, which is their
   source order: the four arm blocks' addresses are strictly increasing, and
   case 0's two-insn block is jump2 cross-joining its tail onto case 4's,
   which is why case 0 ends in `j .Llo16_8001CDA0'.
   `func_800A6D30() % ((u16)p[2] - 2) + 1' is a SIGNED `div' because both
   operands promote to `int'; the `andi $v0, $v0, 0xFFFF' is the u16 return
   value being widened, which comes free from `u16 func_800A6D30()'.
   p[2] and p[3] are read `(u16)' because the listing uses `lhu'. */
void func_lo16_8001CCEC(s16 d, s16 *ox, s16 *oy, s16 *p)
{
    if (p[5] == 0)
    {
        *ox = 0;
        *oy = 0;
        return;
    }
    switch (d)
    {
    case 0:
        *ox = (u16)p[2];
        *oy = func_800A6D30() % ((u16)p[3] - 2) + 1;
        break;
    case 2:
        *ox = func_800A6D30() % ((u16)p[2] - 2) + 1;
        *oy = (u16)p[3];
        break;
    case 4:
        *ox = -1;
        *oy = func_800A6D30() % ((u16)p[3] - 2) + 1;
        break;
    default:
        *ox = func_800A6D30() % ((u16)p[2] - 2) + 1;
        *oy = -1;
        break;
    }
}

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

/* 62 words, 0 differing.  ONE MEASURED FACT: the store order inside the gated
   block is unk_00, unk_02, unk_04, and the EMITTED order is the reverse of the
   source order for the last two.  Written unk_00, unk_04, unk_02 -- the order
   the listing appears to show -- it is 62 words / 6 aligned differences, and
   those six rows are the unk_02 load and the unk_04 reload exchanged between
   the two scheduler slots either side of the unk_00 store.  sched1 transposes
   the two independent loads, so this is an order to PRICE and not to read.
   The two `lhu 0x4($v1)' reads are both real: the unk_00 store between them
   is a struct ref, so true_dependence does not excuse the in-struct reload and
   it survives.
   The bounds are `(u16)p[0] + (u16)p[2]' and `(u16)p[1] + (u16)p[3]' computed
   before the loops -- they cannot be inline, because the inner loop stores to
   cells and loop.c will not hoist a MEM out of a loop with unknown stores,
   and the target holds both sums in registers across both loops.  The inner
   loop's init, by contrast, RE-READS `(u16)p[0]' every outer iteration, which
   is what `for (j = (u16)p[0]; ...)' gives.  `(i & 1) == t && (j & 1) == t'
   with t = `func_800A6D30() & 1'; the i-parity is loop-invariant and hoisted
   into the preheader by itself. */
void func_lo16_8001CEC0(s16 *p, s16 b)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s32 t;
    s32 xe;
    s32 ye;
    s32 i;
    s32 j;

    g = (UnkDngLo16Grid *)D_8008333C;
    t = func_800A6D30() & 1;
    xe = (u16)p[0] + (u16)p[2];
    ye = (u16)p[1] + (u16)p[3];
    for (i = (u16)p[1]; i < ye; i++)
    {
        for (j = (u16)p[0]; j < xe; j++)
        {
            if ((j & 1) == t && (i & 1) == t)
            {
                c = &D_800EA000_s[(i << g->unk_14) + j];
                c->unk_00 = (c->unk_04 >> 1) + 0x63;
                c->unk_02 -= 0x60;
                c->unk_04 |= 0x8000;
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001CFB8);

/* 141 words, 0 differing.  FIRST COMPILE, off the landed func_lo16_8001D328
   in this unit -- the same `k' clamp, the same two func_800A6DA4 draws and
   the same func_8009A350 probe, with a kind/flag publish instead of the
   func_lo16_8001E660 call.  Every spelling here is therefore a CHOICE and
   not a measurement; what follows is what the listing dictated.
   `p' is `s16 *' but the two record stores read it as BYTES
   (`*(u8 *)p' and `*((u8 *)p + 2)', the target's `lbu 0x0' and `lbu 0x2'),
   while the func_8009A350 arguments read the same two offsets as HALFWORDS
   -- so the same address is read both ways and both are real.
   D_800E39C8 and D_800E3648 are STRUCT ARRAYS subscripted on the loop
   counter, which is what loop.c reduces into the target's -0x18 and -0x4
   cursors; csrc/ovl_dungeon_decls.h records why a `u8 *' walk loses the
   base.  The 0x3000 test's index is `func_800A6D30() % 19 + 1' held in an
   `s16' -- the 0xAF286BCB magic with the extra add is the unsigned divide
   by 19, and the `s16' is what gives the `sll 16'/`sra 16' before the *12
   while the `unk_00' store uses the raw value. */
void func_lo16_8001D0F4(s16 *p, s16 b)
{
    u16 buf[4];
    s16 k;
    s16 j;
    s32 i;
    s32 f;
    s32 r;
    s32 r2;

    f = func_800A6D30() & 0xFF;
    k = ((u16)p[3] * (u16)p[2] >> 3) + (func_800A6D30() & 7);
    if (k >= 0x1D)
    {
        k = 0x1C;
    }
    for (i = k; i >= 0; i--)
    {
        r = func_800A6DA4(0, (u16)((u16)p[2] - 1)) & 0xFFFF;
        r2 = func_800A6DA4(0, (u16)((u16)p[3] - 1)) & 0xFFFF;
        func_8009A350((u16)p[0] + r - 1, (u16)p[1] + r2, 0, buf);
        if ((buf[0] & 0x8820) == 0)
        {
            D_800E39C8[i].unk_06 = *(u8 *)p + r;
            D_800E39C8[i].unk_07 = *((u8 *)p + 2) + r2;
            if (f == 0)
            {
                D_800E3648_s[i].unk_00 = 7;
            }
            else
            {
                j = func_800A6D30() % 19 + 1;
                if ((*(u16 *)((u8 *)D_80073414[0x15].unk_0C + j * 12) & 0x3000) == 0x3000)
                {
                    continue;
                }
                D_800E3648_s[i].unk_00 = j;
            }
            D_800E3648_s[i].unk_01 = 0x15;
            D_800E3648_s[i].unk_03 = 0x80;
            D_800E3648_s[i].unk_02 = 0;
            func_8009A21C(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, 0x20);
        }
    }
}

/* 97 words, 0 differing.  TWO FACTS, and the second one is the general lesson
   of this batch:
   - both func_800A6DA4 calls come BEFORE both stores.  With the first store
     between them the first random value never has to survive the second call,
     the body saves one fewer callee-saved register and it is 94 words / 43
     aligned differences (six saves against the target's seven).
   - `s32 r = func_800A6DA4(...) & 0xFFFF' and NOT `u16 r = (u16)func_...'.
     The `u16' spelling lets combine delete the mask -- the value's only use is
     an `addu' feeding an `sb', so eight bits are provably enough -- and the
     candidate comes out `move $s0, $v0' where the target has
     `andi $s0, $v0, 0xFFFF': 96 words / 7.  The explicit `&' on an `s32' local
     keeps it.  This is worth remembering outside this body; it is the one
     change that separated a 96-word near miss from the landing.
   `k' is `s16' and `i' is `s32': the clamp compares `(s16)' values (the
   `sll'/`sra' pair is duplicated into both arms of the 0x3D test and merged at
   `.Llo16_8001D394'), while the loop's `bgez $s2' and `addiu $s2, -0x1' carry
   no extension at all, which only an `s32' counter gives.
   Every record reference is a plain subscript on `i'; loop.c reduces the three
   of them into the target's three cursors (-4, -4, -0xC), including deriving
   `&D_800E3548_s[i]' from `&D_800E3548_s[i].unk_01' with `addiu $a0, $v0,
   -0x1' rather than a second `addiu %lo'. */
void func_lo16_8001D328(s16 *p, s16 b)
{
    u16 buf[4];
    s16 k;
    s32 i;
    s32 r;
    s32 r2;

    k = (((u16)p[3] * (u16)p[2]) >> 6) + (func_800A6D30() & 7) + 4;
    if (k >= 0x3D)
    {
        k = 0x3C;
    }
    for (i = k; i >= 0; i--)
    {
        r = func_800A6DA4(0, (u16)((u16)p[2] - 1)) & 0xFFFF;
        r2 = func_800A6DA4(0, (u16)((u16)p[3] - 1)) & 0xFFFF;
        D_800E36C8[i].unk_00 = p[0] + r;
        D_800E36C8[i].unk_01 = p[1] + r2;
        if (func_8009A350(D_800E36C8[i].unk_00 - 1, D_800E36C8[i].unk_01, 0, buf) == 0 ||
            (buf[0] & 0xFF20) == 0)
        {
            func_8009A21C(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01, 0x800);
            func_lo16_8001E660(&D_800E3548_s[i].unk_01, &D_800E3548_s[i].unk_00, 0, 1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D4AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo16", func_lo16_8001D5D8);

/* 154 words, 0 differing.  The room-border stamp: four corner cells, then
   the top/bottom edge run, then the left/right edge run.  THREE MEASURED
   FACTS, and the first is the brief's "two locals that must be ONE":
   - ONE cell pointer local is shared between the four corner stores AND the
     top/left cursor of both loops.  With a second local for the loops it is
     154 words / 52 ALIGNED DIFFERENCES -- every row a pure register rename,
     no instruction moved -- because the target packs both into $t7.  That
     residue shape is exactly what the brief says to look for: a clean
     whole-body rotation with nothing moved is a pseudo-count fault, not an
     allocno_compare one.
   - BOTH loops are `while' loops with the counter INCREMENT AS THE FIRST
     STATEMENT of the body, not `for (x = x0; x < x1; x++)'.  As a `for'
     increment the body is 148 words / 97 differing: gcc puts the `addiu'
     after the six stores and loses the target's `sll 16'/`sra 16' pair on
     the loop test.  The counters are `s16', which is what that pair is.
     Same rule as func_lo16_8001744C and func_lo16_8001DC34 in this unit.
   - THE EDGE CURSORS ARE POINTER LOCALS, not subscripts on the counter.
     Written as `D_800EA000_s[((y0 - 1) << g->unk_14) + x]' the address is
     recomputed in full every iteration and the body is 203 words / 228.
     loop.c only reduces the +0x6 and `6 << g->unk_14' strides out of an
     explicit cursor here.
   `&D_800EA000_s[...] - 1' folds the -6 into the symbol
   (`addiu $t4, %lo(D_800E9FFA)') and D_800EA000 itself comes out as
   `$t4 + 6' by cse -- the func_lo16_8001DC34 rule.  `g' is a STRUCT pointer
   so the `g->unk_14' read stays inside the second loop, which is what the
   `sllv $v1, $a2, $v1' stride recomputation is. */
void func_lo16_8001D9CC(s16 x0, s16 y0, s16 x1, s16 y1, s32 v)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    UnkDungeonCell *cb;
    s16 w;
    s16 x;
    s16 y;

    g = (UnkDngLo16Grid *)D_8008333C;
    w = v + 0x40;
    c = &D_800EA000_s[((y0 - 1) << g->unk_14) + x0] - 1;
    c->unk_00 = 0xE;
    c->unk_02 = w;
    c->unk_04 = 9;
    c = &D_800EA000_s[((y0 - 1) << g->unk_14) + x1];
    c->unk_00 = 0xB;
    c->unk_02 = w;
    c->unk_04 = 9;
    c = &D_800EA000_s[(y1 << g->unk_14) + x0] - 1;
    c->unk_00 = 0xD;
    c->unk_02 = w;
    c->unk_04 = 9;
    c = &D_800EA000_s[(y1 << g->unk_14) + x1];
    c->unk_00 = 0xC;
    c->unk_02 = w;
    c->unk_04 = 9;
    c = &D_800EA000_s[((y0 - 1) << g->unk_14) + x0];
    cb = &D_800EA000_s[(y1 << g->unk_14) + x0];
    x = x0;
    while (x < x1)
    {
        x++;
        c->unk_00 = 0x12;
        cb->unk_00 = 0x10;
        cb->unk_02 = w;
        c->unk_02 = w;
        cb->unk_04 = 9;
        c->unk_04 = 9;
        c++;
        cb++;
    }
    c = &D_800EA000_s[(y0 << g->unk_14) + x0] - 1;
    cb = &D_800EA000_s[(y0 << g->unk_14) + x1];
    y = y0;
    while (y < y1)
    {
        y++;
        c->unk_00 = 0x11;
        cb->unk_00 = 0xF;
        cb->unk_02 = w;
        c->unk_02 = w;
        cb->unk_04 = 9;
        c->unk_04 = 9;
        c += 1 << g->unk_14;
        cb += 1 << g->unk_14;
    }
}

/* 53 words, 0 differing.  FIRST COMPILE, off func_lo16_8001744C's loop shape
   plus this unit's documented grid-pointer rule, so the spellings below are
   assertions and not prices.
   The row base is `&D_800EA000_s[(i << g->unk_14) + (u16)p[0]] - 1': the `- 1'
   folds into the symbol and gives the target's hoisted
   `addiu %lo(D_800E9FFA)' -- D_800EA000 less one cell -- which an `s32' cell
   base could not, because the -6 cannot fold into a register.
   The inner counter's DECREMENT is the first statement of the body, as in
   func_lo16_8001744C, and the three cell stores are 00, 02, 04 (that unit's
   other clearer is 00, 04, 02).  `e = i + (u16)p[3] + 2' reads the raw
   `(u16)p[1] - 1' value a second time rather than reading `i', which is why
   there is no sign extension between the two `addiu's.  g is a STRUCT pointer:
   the shift read stays inside the loop. */
void func_lo16_8001DC34(s16 *p)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s16 i;
    s16 e;
    s16 j;

    g = (UnkDngLo16Grid *)D_8008333C;
    p[5] = 0;
    i = (u16)p[1] - 1;
    e = i + (u16)p[3] + 2;
    for (; i < e; i++)
    {
        c = &D_800EA000_s[(i << g->unk_14) + (u16)p[0]] - 1;
        j = (u16)p[2] + 2;
        while (j > 0)
        {
            j--;
            c->unk_00 = 0;
            c->unk_02 = 0;
            c->unk_04 = 0;
            c++;
        }
    }
}

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

/* 183 words, 0 differing.  The post-generation tile pass: for every live
   room, walk its rectangle and dispatch each cell's tile id through nine
   range tests.  THREE MEASURED FACTS:
   - `g = (UnkDngLo16Grid *)D_8008333C' AT FUNCTION SCOPE, even though the
     target re-materialises the `lui'/`addiu %lo' pair INSIDE the inner loop
     (the `lui' is in the back branch's delay slot).  That is reload
     rematerialising a constant-valued pseudo under pressure, not a
     per-iteration source expression: assigned inside the inner loop, or the
     cast inlined at the use site, combine folds `%lo + 0x14' into the `lh's
     displacement and the body is 182 words / 10 with the `addiu' gone.
     Assigning it in the MIDDLE loop also measures 0, so the two are NOT
     distinguishable -- call the function-scope form the one that reads as
     source.  This is the brief's "a constant inside a hoisted address is
     unreachable by any cast; only a VAR_DECL keeps the addiu", with the
     extra condition that the VAR_DECL must outlive the loop.
   - THE PARITY TEST IS A NESTED `if/else if', NOT `(y & 1) == (x & 1)'.
     The equality form is `andi'/`andi'/`bne' -- three words -- against the
     target's six, and the body is 177 / 18.  Written as
     `if ((y & 1) != 0) { if ((x & 1) != 0) ... } else if ((x & 1) == 0) ...'
     the `andi $v0, $s1, 0x1' is computed ONCE in the outer test's delay slot
     and shared by both arms, and jump2 cross-joins the two `+= 0xA' blocks
     with no goto in the source.
   - the nine range tests are an `if / else if' CHAIN on
     func_lo16_8001CE14 (this unit's landed `b <= a && c >= a'), and the
     fifth argument to func_lo16_8001E108 is 0x20 for the first five arms
     and 0x100 for the last four -- the target holds both constants in
     $s4 and $s5 across the whole function.
   PROTOTYPE CHANGE: func_lo16_8001E108 was landed in this unit as
   `void func_lo16_8001E108(void)' and is called here with FIVE arguments.
   Its definition is empty, so the five parameters cost nothing -- it
   re-verifies at 2 words / 0 differing -- but the arity change is real and
   has to ride along with this body. */
void func_lo16_8001DE2C(void)
{
    UnkDngLo16Grid *g;
    UnkDungeonCell *c;
    s32 r;
    s32 x;
    s32 y;
    s32 xe;
    s32 ye;

    g = (UnkDngLo16Grid *)D_8008333C;
    for (r = 0; r < 0x24; r++)
    {
        if (D_800E2970_s[r].unk_0A != 0)
        {
            y = D_800E2970_s[r].unk_02;
            xe = D_800E2970_s[r].unk_00 + D_800E2970_s[r].unk_04;
            ye = y + D_800E2970_s[r].unk_06;
            for (; y < ye; y++)
            {
                for (x = D_800E2970_s[r].unk_00; x < xe; x++)
                {
                    c = &D_800EA000_s[(y << g->unk_14) + x];
                    if (func_lo16_8001CE14(c->unk_00, 0x13, 0x1C) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x13, 0x20);
                        if ((y & 1) != 0)
                        {
                            if ((x & 1) != 0)
                            {
                                c->unk_00 += 0xA;
                            }
                        }
                        else if ((x & 1) == 0)
                        {
                            c->unk_00 += 0xA;
                        }
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x27, 0x30) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x27, 0x20);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x31, 0x3A) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x31, 0x20);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x3B, 0x44) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x3B, 0x20);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x45, 0x4E) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x45, 0x20);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x6B, 0x6F) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x6B, 0x100);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x70, 0x74) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x70, 0x100);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x75, 0x79) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x75, 0x100);
                    }
                    else if (func_lo16_8001CE14(c->unk_00, 0x7A, 0x7E) != 0)
                    {
                        func_lo16_8001E108(x, y, c, 0x7A, 0x100);
                    }
                }
            }
        }
    }
}

void func_lo16_8001E108(s32 x, s32 y, UnkDungeonCell *c, s32 base, s32 k) {
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

/* 113 words, 0 differing.  TWO MEASURED FACTS.
   - D_80012090 IS THE CONSTANT ADDRESS `*(s32 *)0x80012090', not the
     structs.h symbol.  As the symbol the body is 112 words / 3 aligned
     differences: the `lui %hi' moves up into the `bnez $a3' delay slot that
     the target leaves as a `nop'.  This is the same verdict
     csrc/ovl_dungeon_800A4B88.c's func_800A9230 records ("D_80012090 IS THE
     CONSTANT ADDRESS, as it is everywhere else in this unit"), reproduced
     here on a relink.  THIS BODY THEREFORE ADDRESSES A CONSTANT BASE and
     the masked verifier cannot see the displacement -- `--raw' is the check.
   - `s32 k', NOT `s16'.  `s16 k = D_8008146C' makes gcc keep TWO pseudos
     for it -- an `lh' for the four compares and an `lhu' for the `a[0] = k'
     byte store -- and the body is 114 words / 16 differing.  The target
     loads once with `lh' and stores that register.  The brief's "narrow
     signed call result cuts both ways" rule, here on a global read: narrow
     and wide uses together want the WIDE local.
   The four-way test on D_8008146C is an if/else-if CHAIN, not a `switch':
   the target's compares are strictly linear (0xC, 0xF, 0x1C, 0x14 in that
   order, each `bne' falling to the next), where four case nodes would have
   given expand_end_case's BALANCED tree.  `a[0] = k' in the 0xC arm reuses
   the compared value, which is the target's `sb $v1, 0x0($s0)'.
   `func_800A697C' needs no cast: csrc/ovl_dungeon_800A4B88.c declares it
   `s16 func_800A697C(u8, u8)' and the `sll 16'/`bnez' is that return type.
   The shared `b[0] = X; D_8001F6F0 = 1; return 1;' tail is cross-joined by
   jump2 between the 0xC and 0x14 arms with no goto in the source. */
s32 func_lo16_8001E660(u8 *a, u8 *b, s32 c, s32 d)
{
    s32 k;

    if (D_8001F6F0 == 0 && d == 0 && *(s32 *)0x80012090 == 0)
    {
        k = D_8008146C;
        if (k == 0xC)
        {
            if (D_800E3D7C[0xFB] == 0)
            {
                a[0] = k;
                b[0] = 4;
                D_8001F6F0 = 1;
                return 1;
            }
        }
        else if (k == 0xF)
        {
            if (func_80033BC0(0x145E) != 0 && func_80033BC0(0x1460) == 0 &&
                func_800A697C(0xD, 1) == 0)
            {
                a[0] = 0xD;
                b[0] = 1;
                D_8001F6F0 = 1;
                return 1;
            }
        }
        else if (k == 0x1C)
        {
            if (func_80033BC0(0xFC3) != 0 && func_80033BC0(0xFAD) == 0 &&
                func_800A697C(1, 8) == 0)
            {
                a[0] = 1;
                b[0] = 8;
                D_8001F6F0 = 1;
                return 1;
            }
        }
        else if (k == 0x14)
        {
            if (func_80033BC0(0xAC) != 0 && func_80033BC0(0x12C6) == 0 &&
                func_800A697C(0xD, 6) == 0)
            {
                a[0] = 0xD;
                b[0] = 6;
                D_8001F6F0 = 1;
                return 1;
            }
        }
    }
    func_lo16_8001EAA4(a, b);
    return 0;
}

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

/* 63 words, 0 differing.  ONE MEASURED FACT: `t = x' is assigned BEFORE the
   func_8003FC64 call, outside the `if'.  Inside the `if' the copy is sunk past
   func_8004491C by sched1, `t' never has to live across a call, the body saves
   six registers instead of seven and it is 61 words / 22 aligned differences.
   Hoisting it to the top of the function pins it to a callee-saved register
   and gives the target's `addu $s4, $s1, $zero' in the `beqz' delay slot.
   Otherwise this is csrc/ovl_dng_s69.c's func_s69_80025FCC skeleton: the node
   header's unk_08 vector block, the `r = n->unk_0C' payload and the
   `q = (u8 *)n + 0x20' tail base, with a different allocation size (2 rather
   than 0x202), a data symbol where that body passes a function address, and a
   two-way colour word instead of a straight store. */
void func_lo16_8001F354(s16 x, s16 y, s16 f, u32 v)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s16 t;

    t = x;
    n = func_8003FC64(2);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A5AF0;
        func_8004491C(n, (u32)D_80044BB0);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x + 0x280;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = 1;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1C) = 0x2000;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(u32 *)(r + 0x8) = v;
        if (f != 0)
        {
            *(u32 *)(r + 0xC) = 0x202080;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0x808080;
        }
        *(s16 *)(r + 0x10) = 0x20;
        *(u16 *)(r + 0x14) |= 0xC;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x6) = 0x18;
        *(s16 *)(q + 0x8) = t;
    }
}

void func_lo16_8001F450(void)
{
    u8 buf[8];
    u8 *b;
    s16 x;
    s16 y;
    s32 f;
    int new_var;
    u8 *t;
    int new_var2;

    new_var = 2;
    t = NULL;
    new_var2 = 0xAA;
    if (D_80012090 != new_var)
    {
        if (D_8008146C == 0x28)
        {
            t = D_80077E00;
        }
        f = 0;
    }
    else
    {
        f = 1;
    }
    if (t != NULL)
    {
        func_lo16_8001F354(0xAA, 0x78, 0, (u32)t);
        return;
    }
    func_lo16_8001F354(new_var2, 0x78, f, (u32)D_80077DE8);
    b = buf;
    func_8004E634(D_8008146C, b);
    if (buf[1] == 0)
    {
        x = 0xDA;
    }
    else
    {
        x = 0xC8;
    }
    if (buf[0] == 0)
    {
        return;
    }
    y = 0x78;
    while (*b != 0)
    {
        if (*b != 0x20)
        {
            func_lo16_8001F354(x, y, f, D_8001F5B4[*b]);
        }
        b++;
        x += 0x12;
    }
}
