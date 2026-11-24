#include "common.h"

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b30_80171DCC(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80174198[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern u16 D_80083462;

extern Unk80083462 D_80083460;
extern u8 D_80176660[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_801765D0[];
extern u8 D_80176650[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC7EC();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b30_80170EE4(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

s32 func_8009A180(u8 *a, u8 *b);
void func_800A9A0C(u8 *);
extern u8 D_80176668[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b30_801737B8(u8 *p, u8 *b, u8 *c, u8 *d);

u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);

s32 func_800AB464();

s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;

typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_b30_80175594(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

extern u8 D_801765E8[];

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b30_80171E38(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
void func_b30_80171460(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b30_801716A4(u8 *p, UnkVec *b, void *c, u8 *d);
s32 func_b30_80172114(u8 *a, void *b, void *c, s32 d);
void func_b30_80171FFC(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b30_80173C40(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800453E0(u8 *, UnkVec *, UnkSlot *, s16);
extern u8 D_80176678[];
extern u8 D_801765D8[];
extern u8 D_801765E0[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801708B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80170AD0);

/* The banked room script's main actor handler and this bank's default +0x8C
   continuation: the three early bails, the D_801765D8 facing-table swap, and
   then the twelve-way dispatch on `*(u16 *)(d + 0x46) & 0x3FFF'. 351 words,
   0 differing, on the first compile.

   IT OWNS A JUMP TABLE: jtbl_b30_80170808, 12 entries, `.align 3', in this
   unit's .rodata. Read off the listing, index = (that mask) - 1:
     cases 1 2 3  -> .Lb30_801712E0, the func_800AAF00 arm
     case  4      -> .Lb30_8017130C, default
     cases 5 6 7  -> .Lb30_8017128C, the func_800A0818 re-aim arm
     case  8      -> .Lb30_80171220, the func_b30_80171E38 arm
     case  9      -> .Lb30_80171248, the 0x20000000 arm
     cases 10 11  -> .Lb30_8017130C, default
     case  12     -> .Lb30_801712D0, the func_800A9A0C arm
   Cases 4, 10 and 11 carry no label of their own. MEASURED on the object:
   the table cc1psx emits lands at .rodata offset 0x38 with the section
   0xb4 long, which is byte-for-byte where the stub's listing-supplied copy
   was -- D_b30_8017086C stays at 0x0, D_b30_80170898 at 0x2c,
   jtbl_b30_80170838 at 0x68, jtbl_b30_80170850 at 0x80 and
   jtbl_b30_801708A0 at 0xa0, all unchanged. The twelve entries resolve to
   the six labels above at a single consistent function base, so the
   table's shape is right.

   TWIN: the landed carved sibling func_b37_80170E68 in csrc/ovl_dng_b37.c,
   345 words. THIS IS THAT BODY PLUS SIX WORDS, all in case 9: b37 calls
   func_b37_80174008 and returns, and here case 9 tests
   `*(s32 *)(d + 0x14) & 0x20000000' and picks func_b30_80173C40 or
   func_b30_80171FFC, with cross-jumping sharing the func_b30_80171FFC call
   with case 8's tail. Everything else is a rename:
   D_801741DC -> D_80176650, D_801741D4 -> D_80176678,
   D_80174194 -> D_801765D8, D_8017419C -> D_801765E0,
   D_801741CC -> D_80176660, and the callees func_b37_801713CC ->
   func_b30_80171460, func_b37_80171624 -> func_b30_801716A4,
   func_b37_80173CEC -> func_b30_801737B8, func_b37_801720AC ->
   func_b30_80172114, func_b37_80171DD0 -> func_b30_80171E38,
   func_b37_80171F94 -> func_b30_80171FFC.

   MEASURED ON THIS BODY, the one spelling that matters: the D_800814A8
   pointer load has to be emitted BETWEEN func_800A0818's return and the
   store of its result to d + 0x2A, which is what `m = func_800A0818(...);
   w = (u8 *)D_800814A8; *(s16 *)(d + 0x2A) = m;' gives. Any spelling that
   puts the store first -- dropping `m', or dropping `w', or dropping both --
   is 352 words / 118 differing, identically, because that is the only thing
   the three have in common.

   `(s16)func_b30_80172114(p, b, c, 0)' is load-bearing: without the cast it
   is 350 words / 175 differing.

   MEASURED AND FREE, so NOT the reason this lands -- b37's comment calls the
   first of these load-bearing on ITS body and it is not on this one:
     - the `case 12:' label sitting inside the `if (w[0x9A] != 0x11)' body
       and the fallthrough from cases 5/6/7 into cases 1/2/3. Written out
       flat -- the 0x11 test with its own `return', the func_800AAF00 call
       duplicated, and `case 12:' an arm of its own -- it is 351 words /
       0 differing. jump.c rebuilds both shared blocks by itself here.
     - the `e' / `e2' / `e3' locals holding the facing table. All three
       inlined at every use it is 351 words / 0 differing.

   `--raw' is 82 differing and every row is a relocation -- 23 `jal's,
   11 `j's and 46 `%hi'/`%lo' halves, including the two that take this
   function's own address for the func_800AAF00 arm -- so no masked word is
   hiding a constant address. */
void func_b30_80170EE4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b30_80171460(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80176650)
        {
            e = D_80176678;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80176650)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80176678) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            e2 = D_801765D8;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_801765E0) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b30_801737B8(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b30_801716A4(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 && (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b30_80172114(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b30_801716A4(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b30_80171E38(p, b, c, d) != 0)
            {
                return;
            }
            func_b30_80171FFC(p, b, c, d);
            return;
        case 9:
            if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
            {
                func_b30_80173C40(p, b, c, d);
                return;
            }
            func_b30_80171FFC(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
            case 12:
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_800AAF00(p, b, c, (s32)D_80176660, func_b30_80170EE4);
            return;
        default:
            func_b30_801716A4(p, b, c, d);
            return;
        }
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
        {
            if ((v & 0x430) == 0)
            {
                if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
                }
            }
        }
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    e3 = D_801765D8;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_801765D0 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b30_80171DCC.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b30_80171DCC is called only on
   `(s16)t == 3 && !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)`, and 0xF is
   the else arm.  The listing shows it: `bne $v0, $v1, .Lb07_8017172C` lands
   on an `addiu $v0, 0xF` of its own, the copy reorg left behind when it
   stole the else arm's `addiu` into the two `bnez` delay slots.  Written the
   twin's way round the function is 143 words and 46 differing: the two arms
   swap, and jump.c then cross-jumps the `sw $zero, 0x8C($s4)` that the
   target keeps in both.

   DEVICE: `x + ((x + x) - x - x)` on func_800A0818's first argument.  It is
   the phantom zero of docs/matching-levers.md :11693 -- cse keeps the three
   insns, flow counts them as references on `x`, and combine deletes them, so
   it emits nothing and ties nothing.  Its job here is allocation only:
   without it `d` outranks the `x`/`t` quantity in global-alloc and the two
   swap callee-saved registers, `d` taking $s0 and `x` $s1 where the target
   has $s1 and $s0 -- 145 words, 28 differing, every one of them that rename
   (and `t`, which shares `x`'s register, renames with it).  What the
   function does without it: the same 145 instructions with two callee-saved
   registers exchanged.  It does NOT warn under -Wall: nothing is read
   uninitialised.

   Measured and rejected, all 145 words unless stated:
     the phantom on func_8009A3D0's first argument instead          7
     the phantom as its own statement, `x = x + ((x + x) - x - x);` 2
     the phantom on `y` as well as `x`                              28
     `__asm__("" ::"r"(x));` before the func_8009A3D0 call          2
     no device at all                                               28
     no device, declaring `x` and `y` first                         28
     no device, `s16 x, y`                                          28
     no device, `d` copied into a local before the first test       28
     merging `t` into `x`                                           53
     merging `r` into `y`                                146 words, 112
     a local for the 0x300/0x3000 select                 146 words, 118
     `x` and `y` inline, no locals                       144 words, 120
     `x` and `y` computed before the table swap          146 words, 134 */
void func_b30_80171460(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_801765D0)
        {
            *(u8 **)(c + 0x2C) = D_801765D0;
            func_80047784(c, D_801765D0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(/* DEVICE: the phantom zero.  See the header. */
                          x + ((x + x) - x - x), y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
        t = func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if ((s16)t == 3 && (D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            func_b30_80171DCC(p, b, c, d);
        }
        else
        {
            p[0x9A] = 0xF;
        }
        *(s32 *)(p + 0x8C) = 0;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        if ((D_80083462 & 0x80) != 0)
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

/* The banked room script's walk step, the relinked twin of the landed carved
   func_b57_801726C8 (csrc/ovl_dng_b57.c, 466 words). 458 words, 0 differing,
   on the first compile. THIS COPY'S OWN DELTAS, all four read off the
   listing: func_800A19E4's third and fourth arguments are 2 and 4 where b57
   has 3 and 6; the state check is the bank-local func_b30_80171E38 where b57
   calls func_b57_80172E10; the step probe is func_8009A66C where b57 calls
   func_8009A8C0; and the LAST arm's 0x20000 block has NO
   `func_800A0134(D_800814A8, d) < 0x81' guard before func_8009A540 -- b57
   does, and that guard is the whole 8-word difference in size between the
   two bodies.

   MEASURED ON THIS BODY, and all three of b57's devices are needed here too,
   at this body's own prices:
     - the 0x80000000 in the local `cc2' with the read-modify-write in a
       `do { } while (0)': without it 458 words / 4 differing, the loaded
       word and the `lui 0x8000' swapping $v0 and $v1 (b57: also 4).
     - `t1 = (s32)D_8006CCD8; t1 += kk;' so the `%lo' insn's destination has
       reg_n_sets != 1: written inline as `*(u8 *)(kk + (s32)D_8006CCD8)' it
       is 11 differing (b57: 8). The SECOND lookup must stay inline.
     - the `return' in the i == 0 arm wrapped in a `do { } while (0)': a
       plain `return;' is 11 differing (b57: 4), reorg filling only one of
       the three branches into the `i++' join with the `addiu $v0, $s1, 0x1'
       copy instead of all three.
   All three are most likely macro residue rather than devices -- each wraps
   one coherent unit of work -- but the macro is not recoverable.

   `i = func_800BCB04h(...)' through the s16 alias decides the whole
   function's register assignment, exactly as b57 says: as
   `i = (s16)func_800BCB04x(...)' off an `s32'-returning declaration it is
   458 words / 124 differing (b57: 126), because the halfword assignment
   keeps `i's allocno references up and allocno_compare allocates it before
   `d' and `c'.

   The loop's heading value SHARES `e' with the two actor lookups: in its own
   local it is 3 differing (b57: 3), the `lh' of d + 0x2A landing in $a0
   where the target has $v1.

   MEASURED AND FREE, so NOT the reason this lands -- b57 carries both and
   this body does not need either:
     - `w = (u16 *)(p + 0x98)' in the +0x20000 arm: inlined at both uses it
       is 458 words / 0 differing.
     - `t80 = D_80082E80' in the last arm's +0x20000 block: inlined at all
       four uses it is 458 words / 0 differing. (b57's 23-differing note is
       about the OTHER arm's D_80082E80, which is inline in both bodies.) */
void func_b30_801716A4(u8 *p, UnkVec *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 t1;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b30_80171E38(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 2, 4, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) =
                    func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24],
                                                       D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                              (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                   (u16 *)(p + 0x98));
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24),
                                      *(u8 *)(c + 0x25), (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - tbl[i];
        }
        else
        {
            s = e + tbl[i];
        }
        if ((s16)func_8009A66C((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20,
                       (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* State 0x10 with a 0x9E countdown of 2, and hand c the bank-local
   D_801765E8 byte the facing at d+0x2A selects, through func_80047784 rather
   than the resident twin's func_8003DB94. 27 words, 0 differing.
   Shape is func_800D9820's (ratio 0.923): the table base is a POINTER LOCAL
   because it is used twice -- stored at c+0x2C and indexed -- but here the
   table is bytes and the index a plain subscript (`sra 9`, `andi 7`), not the
   resident's byte offset into a pointer array.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x3.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b30_80171DCC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    *(s16 *)(p + 0x9E) = 3;
    t = D_801765E8;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80176660 facing table and re-aim through func_8009C93C.

   TWO THINGS THAT ARE NOT VISIBLE IN THE LOGIC.

   THE FRAME IS 0x40 WHERE THE CALLS NEED 0x38, SO THE ORIGINAL DECLARED AN
   UNREFERENCED 8-BYTE AGGREGATE LOCAL. The widest call here is
   func_8009C93C's five arguments, which is a 0x18 outgoing-argument area and
   a 0x38 frame -- which is exactly what func_800CA788 gets for the same call
   set. gcc allocates the slot for an aggregate even when flow deletes every
   reference to it, so the extra 8 bytes are a leftover declaration. Only the
   size is witnessed, never the type. No local at all, and `s32 unused;`
   instead of the array, both give 113 words and 16 differing -- every
   $sp-relative offset and both ends of the frame.

   AND THE SHARED `-1` EXIT BELONGS TO THE *LAST* GATE, NOT THE FIRST ONE THAT
   REACHES IT. All four failure exits are one `goto fail`, and the label sits
   in the second func_800A2B5C test, which is where the merged
   `j .Lb03_80171F3C` / `addiu $v0, $zero, -0x1` block is emitted -- as the
   fall-through of that test's `beqz`, with reorg then copying the constant
   into the other three branches' delay slots. func_800CA788's note puts its
   own label in its SECOND test; that is its layout, not a rule.

   | spelling                                  | words | differing |
   | four plain `return -1;`, no label         |  113  |    35     |
   | label in the second D_80083460 gate       |  113  |    36     |
   | label in the `& 0x8` arm                  |  112  |    73     |
   | label in the second func_800A2B5C test    |  113  |     0     |

   func_800A04F0 is reached through a unit-local alias because the landed
   definition in csrc/ovl_dungeon_8009F644.c is `void` while this call site
   uses $v0, and its fourth parameter has to be declared `s16` here rather
   than the definition's `u16`: `u16` makes the `lh` of d + 0x2A an
   `lh`/`andi $a3, $a3, 0xFFFF` pair -- 113 words, 1 differing.

   `d[0x6D] -= 1;` is also BEFORE the func_8009C93C call, not after it as in
   func_800CA788: after the call is 113 words, 8 differing, because the store
   can no longer sink past the `lh` of d + 0x2A. 113 words, 0 differing. */
s32 func_b30_80171E38(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    u8 *e;
    s32 unused[2];

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
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
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    d[0x85] = 0;
    e = D_80176660;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The banked room script's unconditional "same slot" entry, the 70-word
   sibling of func_b03_80171DA0 above (the relinked twin of the resident
   overlay's func_800CA788) with the cursor pick and the heading window cut
   out: past the single D_80083462 0x2000 gate and the two func_800A2B5C
   tests, put the actor into state 0x11, swap the D_80176660 facing table
   into the slot's +0x2C and re-aim through func_8009C93C.

   THIS ONE IS `void` AND IT CARRIES NO LEFTOVER LOCAL. All three gates fall
   straight into the epilogue with no $v0 set, and the calls here need exactly
   the 0x30 frame the five register saves and func_8009C93C's 0x18 outgoing
   area add up to -- so the 8-byte aggregate func_b03_80171DA0's comment had
   to invent is absent. Adding `s32 unused[2];` back is 12 differing, every
   $sp-relative offset and both ends of the frame.

   AND THE +0x84/+0x85 PAIR MOVED TO THE TAIL. func_b03_80171DA0 stores them
   before the table swap; here they are AFTER the func_8009C93C call, which is
   the only place the scheduler cannot hoist them from. In the sibling's
   position they are 28 differing. `d[0x6D] -= 1;` stays BEFORE that call as
   in the sibling: after it is 10 differing.

   | spelling                                   | words | differing |
   | as written below                            |  70   |     0     |
   | +0x84/+0x85 before the table swap           |  70   |    28     |
   | `s32 unused[2];` restored                   |  70   |    12     |
   | `d[0x6D] -= 1;` after func_8009C93C         |  70   |    10     |
   | p stores in +0x9A, +0x9B, +0x8C order       |  70   |     2     |
   | plain `if (func_800A2B5C(d) != 0)`          |  68   |    51     |

   The `(s16)` on both func_800A2B5C tests is the target's `sll $v0, $v0, 16`
   / `bnez` halfword test; dropping it costs the two `sll`s and 51 differing.
   Three equal spellings, all 0: the gate read as
   `g = (u8 *)&D_80083460; *(u16 *)(g + 0x2)` the way the sibling spells it,
   D_80176660 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b30_80171FFC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    p[0x9A] = 0x11;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    e = D_80176660;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80172114);

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b30_80170EE4. 31 words,
   0 differing, first compile. */
void func_b30_80172240(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b30_80170EE4;
        *(s32 *)(p + 0x90) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801722BC);

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b30_80172640(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801726A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801728E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80172D04);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_80173084(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_801730A4(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b30_801730C4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b30_801730E4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* The banked room script's three-state tail, switching on the state index at
   p + 0x9B: the relinked twin of the resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c), which is this body statement for statement
   with four local changes -- two separate bank-local facing tables
   (D_80176650 in state 0, D_80176668 in state 1's re-dress) where the
   resident uses D_800E262C for both, func_80047784 for func_8003DB94,
   func_b30_801737B8/func_b30_80170EE4 for func_800DA660/func_800D8C64, and
   NEITHER of the twin's two `*(s32 *)(d + 0x1C) &= ~0x200' clears: state 1's
   0x8000 arm and state 2 install the handler and nothing else.
   228 words, 0 differing.

   STATE 1 OPENS WITH A func_80042900 GATE THE RESIDENT TWIN DOES NOT HAVE,
   and its false arm is the re-dress block the long refusal chain also falls
   into -- so the whole chain is the body of
   `if ((s16)func_80042900((s8 *)d, 1) != 0)' and the re-dress follows it.

   THE `p[0x9B] += 1;' TAIL IS WRITTEN OUT PER ARM, NOT `goto'-ed, and
   jump2's cross-jumping puts the single copy back: as `goto bump' it is
   226 words / 29 aligned differences, because with one bump block the two
   `sh' stores either side of it come out in the same registers and
   cross-jumping merges them too, costing state 0's `sh $v0, 0xA($v1)' and
   the load-delay `nop' the target has. Written per arm the two `sh's differ
   ($v1 address/$v0 value in state 0, $v0 address/$v1 value in state 1), the
   merge stops at the bump, and the word count is the target's.

   DEVICE: state 0's bump is wrapped in a `do { } while (0)'. Without it the
   body is 228 words / 18 aligned differences and every one of them is `p'
   and `b' holding each other's callee-saved register -- the target has
   p = $s2, b = $s3 and the plain body gives b = $s2, p = $s3. flow.c counts
   a reference once per enclosing loop depth, so the zero-trip loop makes
   state 0's two `p' references count twice, and that is enough to lift `p'
   past `b' in allocno_compare without lifting it past `c' and `d'. The
   cancelled pointer pair `p++; p--;' after the declarations overshoots --
   it also gives `p' a second set, which turns off update_equiv_regs'
   live-length doubling, and `p' comes out in $s0 ahead of `d': 49 aligned
   differences. Measured and equal at 0 differing, so the exact spelling of
   the wrapped statement is not the lever: `++p[0x9B];' inside the
   do-while, and widening it to wrap the `*(u16 *)(h + 0xA) -= 1;' as well.
   Measured and rejected: the do-while around state 1's bump instead is
   226 words / 11 differences and around both bumps 226 / 37;
   `p[0x9B] = p[0x9B] + 1;' long-hand in state 0 and declaring `b' as
   `void *' are both 18, i.e. no change at all.

   `&D_80083460' is FOUR separate pointer locals, one per arm, as in both
   resident twins; sharing state 0's with state 1's re-dress is 226 words /
   30 differences. */
void func_b30_8017311C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80176650;
        func_80047784(c, D_80176650[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        do
        {
            p[0x9B] += 1;
        } while (0);
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            g = (u8 *)&D_80083460;
            if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
            {
                return;
            }
            if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
            {
                return;
            }
            if (d[0x25] == 0)
            {
                if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
                {
                    return;
                }
                func_800AA79C(p, b, c, d);
                return;
            }
            if ((s16)func_800A2C34(d) != 0)
            {
                return;
            }
            v = *(s32 *)(d + 0x1C);
            if ((v & 0x100) != 0)
            {
                func_800AA258(p, b, c, d);
                return;
            }
            if ((v & 0x80000) != 0)
            {
                func_800AA888(p, b, c, d);
                func_b30_801737B8(p, (u8 *)b, c, d);
                return;
            }
            if (*(s8 *)(d + 0x6D) == 0)
            {
                return;
            }
            if ((s16)func_800A2C34(d) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            func_800A9A0C(d);
            func_800A9A04(d);
            if ((s16)func_80042900((s8 *)d, 1) != 0)
            {
                e = D_80082E80;
                if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
                {
                    if ((s16)func_8009FD40(e, c) >= 2)
                    {
                        goto again;
                    }
                }
                if ((func_800A6D30() & 7) == 0)
                {
                    func_80042B68((s8 *)d, 1);
                }
            }
        again:
            if ((s16)func_80042900((s8 *)d, 1) != 0)
            {
                return;
            }
        }
        *(u8 **)(c + 0x2C) = D_80176668;
        func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_80176650 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b30_80170EE4. The resident
   overlay's func_800DA398 (csrc/ovl_dungeon_800D50FC.c) is the same machine
   over the same record and its comment covers the shape; the landed
   func_b05_80173BEC is the nearer twin, since like this copy it opens state 1
   with the same d[0x25] test that closes it, so the dress/bump tail is
   written out twice. Representative of a 10-member family.

   THIS COPY CARRIES NO FLAG UPDATES. The resident twin's
   `*(s32 *)(d + 0x1C) |= 0x40000` at both state-1 dress sites and its
   `&= ~0x208` in state 2 are absent here: writing them in, which is exactly
   func_b05_80173BEC's body, is 207 words with 142 differing.

   THE `p[0x9B] += 1;` BUMP MUST BE WRITTEN OUT IN ALL THREE REACHING ARMS,
   NOT REACHED BY A SHARED `goto` LABEL, and that is the only thing that makes
   this function land. jump2 cross-jumps the three copies back into the one
   the target has, so they cost no code -- but flow counts them first, and
   they are worth four references on `p`. One shared label is 195 words with
   15 differing, EVERY ONE OF THEM A REGISTER: `p` takes $s3 and `b` takes
   $s2, the other way round from the target. Two copies -- the opening arm
   reaching the tail's label -- is the same 195 words and the same 15.

   The cause is read off the compiler, not guessed. global.c's
   allocno_compare ranks on floor_log2 (n_refs) * n_refs / live_length, and
   from `.flow` one bump copy gives `p` 9 references over 173 insns against
   `b`'s 5 over 47:

       p = 3 *  9 / 173 = 0.156     loses to
       b = 2 *  5 /  47 = 0.213

   so `p` is allocated second. Three copies put `p` at 13 references over 177
   insns -- 3 * 13 / 177 = 0.220 -- and it wins. The threshold either side was
   solved before compiling: 12 references scores 3 * 12 / 177 = 0.203 and
   still loses, which is why two copies cannot work and three must.
   (docs/matching-levers.md, "HOW MANY REFERENCES YOU NEED IS AN ARITHMETIC
   PROBLEM, NOT A SWEEP".)

   `&D_80083460` IS FOUR SEPARATE POINTER LOCALS, one per arm. Sharing one
   across the arms is 191 words with 146 differing -- the gate load sinks and
   the arms merge.

   The facing tables are NAMED DIRECTLY at all three dress sites. One shared
   `u8 *` local for the three is 195 words with 6 differing, the `%hi`/`%lo`
   pair splitting across registers at each site; three separate locals, one
   per site, also measure 0 and the bare symbols are kept because they need no
   extra declaration.

   Other spellings measured and rejected: `d[0x6D]` read as `u8` rather than
   `*(s8 *)(d + 0x6D)` is 1 differing, an `lbu` for the target's `lb`;
   `*(s32 *)(d + 0x64)` for the `lh` of d + 0x64 is 1 differing; dropping the
   `(s16)` cast on the first func_800A2C34 result is 194 words with 103
   differing. Measured and equal at 195 words / 0 differing, so none of these
   is the reason it lands: re-reading d + 0x1C at both flag tests instead of
   through the `v` local, writing state 2 as a positive `if` arm instead of an
   early return, nesting the d + 0x64 test instead of `&&`, and the three
   separate table locals above.

   195 words, 0 differing. */
void func_b30_801734AC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = D_80176650;
        func_80047784(c, D_80176650[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80176668;
            func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            p[0x9B] += 1;
            return;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b30_801737B8(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80176668;
        func_80047784(c, D_80176668[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_801765D0 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b30_801737B8(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80176650 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_801765D0;
        func_80047784(c, D_801765D0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b30_80173880(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b30_80170EE4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b30_801738E4(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_8017391C(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_80173954(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017398C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80173C40);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80173D40);

/* 0x2c bytes of this bank's own rodata, between its jump tables,
   read by func_b30_80173D78. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b30_8017086C[] = {
    0x81,
    0x40,
    0x82,
    0x83,
    0x82,
    0x81,
    0x82,
    0x8E,
    0x81,
    0x66,
    0x82,
    0x94,
    0x81,
    0x40,
    0x82,
    0x94,
    0x82,
    0x92,
    0x82,
    0x81,
    0x82,
    0x8E,
    0x82,
    0x93,
    0x82,
    0x86,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x8D,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x8E,
    0x82,
    0x94,
    0x82,
    0x8F,
    0x00,
    0x00,
};

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_b30_80173D78. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b30_80170898[] = {
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80173D78);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017427C);

/* 14 words, 0 differing. */
void func_b30_80174580(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

/* 14 words, 0 differing. */
void func_b30_801745B8(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b30_80170EE4;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801745F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80174690);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017474C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80174B90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80174CCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017506C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017516C);

/* Walk the tag chain from `p` and hand every node to func_b30_80175594 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b30_80175540(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b30_80175594(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80175594);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80175C60);

/* Zero the scratchpad word at 0x1F80013C, then walk the tag chain from `p'
   and hand every node to func_800453E0 with -0x10 as the fourth argument.
   23 words, 0 differing, on the first compile.

   THE BEST TWIN IS func_b30_80175540, FOUR LISTINGS UP IN THIS FILE: the
   same walk with a different per-node call and no scratchpad store, which
   is the whole 21 -> 23 word delta. (dngtwin named func_s72_80025034 at
   0.909, which is that same body in csrc/ovl_dng_s72.c; the in-unit one is
   the same shape and was already here.)

   MEASURED AND FREE, so NOT the reason this lands:
     - the scratchpad address written as the absolute `*(s32 *)0x1F80013C'
       rather than through a pointer local. Through
       `sp = (u8 *)0x1F800000; *(s32 *)(sp + 0x13C) = 0;' it is 23 words /
       0 differing. That REFINES the lever func_b06_80173E08 and
       func_b08_801740F4 record: the pointer local is needed only when the
       store is INSIDE the loop, where gas's `lui $at'/`sw' macro cannot be
       hoisted. Here the store is before the loop and both spellings are
       the same two words.
     - `-0x10' versus `0xFFF0' as the fourth argument: 23 words /
       0 differing either way, the `s16' parameter truncating both the same.

   LOAD-BEARING: the `for (;;)' with the NULL test in the MIDDLE of the body.
   That is what puts the `addiu $s0, $a2, 0x20' in the `beqz's delay slot.
   Rewritten as a peeled first call plus a `while (n != NULL)' it is
   24 words / 18 differing. */
s32 func_b30_80175D74(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    *(s32 *)0x1F80013C = 0;
    for (;;)
    {
        func_800453E0(p, v, s, -0x10);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80175DD0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80175F70);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_8017618C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_801763D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b30", func_b30_80176480);
