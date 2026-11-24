#include "common.h"

extern u8 D_80174008[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
u16 func_800A6D30();
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
u16 func_800A6D30(void);
extern u8 D_80082E80[];

extern u32 D_800814A8;
extern u8 D_80173FF8[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
s32 func_8009A180(u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b43_80173900(u8 *p, u8 *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_8009A180(u8 *a, u8 *b);
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern Unk80083462 D_80083460;
void func_80047784(u8 *p, s32 b, s32 c);

extern s16 D_80083228;
extern u8 D_80173FB0[];
extern u8 D_80174000[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b43_80171768(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b43_80170F68(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

extern u8 D_80173FB8[];
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
void func_b43_80171EC0(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB464();

s32 func_800AC7EC();

extern u8 D_80173FD0[];

s32 func_800AB1C0();
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
extern u8 D_80173FC8[];

extern u8 D_80173FD8[];
extern u8 D_80173FE0[];

s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_8009FB34(u16 x, u16 y);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
extern u16 D_80082EA4;
extern u8 D_80173FC0[];
extern u8 D_80173FF0[];
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");
void func_b43_80171510(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b43_80171F24(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b43_80172110(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b43_80172228(u8 *a, void *b, void *c, s32 d);
void func_b43_80173AD4(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s32 D_8008346C;
s32 func_8003F270(void);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
void func_800DA840(SVECTOR *src, s16 a);
s32 func_800A56E0(s32);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80170898);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80170AA4);

/* The banked room script's main actor handler and the bank's default +0x8C
   continuation: the three early bails, the D_80173FB8 facing-table swap, and
   then the twelve-way dispatch on `*(u16 *)(d + 0x46) & 0x3FFF'.
   362 words, 0 differing. Landed on the first compile. 1 copy (dngfam.py).

   IT OWNS A JUMP TABLE: jtbl_b43_80170808, 12 entries, in this unit's
   .rodata at offset 0x0. Read off the listing, index = (that mask) - 1, the
   entries are cases 1 2 3 -> the func_800AAF00 arm, 4 -> default, 5 6 7 ->
   the func_800A0818 re-aim arm, 8 -> the func_b43_80171F24 arm, 9 -> the
   func_b43_80173AD4 arm, 10 11 -> default, 12 -> the func_800A9A0C arm.
   Cases 4, 10 and 11 carry no label of their own.

   TWIN: the landed carved sibling func_b37_80170E68 in csrc/ovl_dng_b37.c,
   345 words. THIS IS THAT BODY PLUS SEVENTEEN WORDS, and the deltas are:

     - the `d[0x25] == 0' arm does not return after the facing swap: it falls
       on to `*(s16 *)(p + 0xAE) = 0' and `func_80042B68((s8 *)d, 0x1A)',
       which is 4 of the 17;
     - b37's case 8 is one call and a tail call; here it is
       `if ((s16)func_b43_80171F24(...) != 0) return;' followed by
       func_b43_80172110, which is the rest.

   Everything else is a rename: D_801741DC -> D_80174000,
   D_801741D4 -> D_80173FF8, D_80174194 -> D_80173FB8,
   D_8017419C -> D_80173FC0, D_801741CC -> D_80173FF0, and the callees
   func_b37_801713CC -> func_b43_80171510,
   func_b37_80171624 -> func_b43_80171768,
   func_b37_80173CEC -> func_b43_80173900,
   func_b37_801720AC -> func_b43_80172228,
   func_b37_80171DD0 -> func_b43_80171F24,
   func_b37_80171F94 -> func_b43_80172110,
   func_b37_80174008 -> func_b43_80173AD4.

   The `case 12:' label inside the `if (w[0x9A] != 0x11)' body and the
   fallthrough from cases 5/6/7 into cases 1/2/3 are the family's and are
   load-bearing: that is how the target reaches .Lb43_80171384 from two
   places and .Lb43_80171398 from the 0x11 test.

   Because it landed on the first compile every spelling in it is UNMEASURED
   except the ones named above; do not read the b37 twin's own prices as
   this body's.

   `--raw' is 85 differing and every one is a relocation -- the `jal's, the
   `j's and the `%hi'/`%lo' halves, including the jtbl pair and the two that
   take this function's own address for the func_800AAF00 arm -- so no masked
   word is hiding a constant address. */
void func_b43_80170F68(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_b43_80171510(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174000)
        {
            e = D_80173FF8;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        *(s16 *)(p + 0xAE) = 0;
        func_80042B68((s8 *)d, 0x1A);
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174000)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80173FF8) != 0)
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
            e2 = D_80173FB8;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80173FC0) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b43_80173900(p, (u8 *)b, c, d);
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
            func_b43_80171768(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b43_80172228(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b43_80171768(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
            if ((s16)func_b43_80171F24(p, b, c, d) != 0)
            {
                return;
            }
            func_b43_80172110(p, b, c, d);
            return;
        case 9:
            func_b43_80173AD4(p, b, c, d);
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
            func_800AAF00(p, b, c, (s32)D_80173FF0, func_b43_80170F68);
            return;
        default:
            func_b43_80171768(p, b, c, d);
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
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
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
    e3 = D_80173FB8;
    if (*(u8 **)(c + 0x2C) == e3)
    {
        return;
    }
    *(u8 **)(c + 0x2C) = e3;
    func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

/* The banked room script's step-the-walk handler, the relinked twin of the
   landed func_b07_80171570 in csrc/ovl_dng_b07.c (itself the resident
   overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c): advance the actor
   one cell of its queued path, swap in the D_80173FB0 facing table,
   re-aim the sprite, and either stop at state 0xF or hand on to
   func_b43_80171EC0.  150 words, 0 differing.  12 copies (dngfam.py).

   IT IS func_b07_80171570 PLUS FIVE WORDS: the `t != 3' arm also clears bit 3
   of the halfword at p + 0x98, so the two 0xF stores cannot be one else arm
   and the gate has to be NESTED.  b07's flat
   `(s16)t == 3 && !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)' with 0xF as
   the single else is 145 words / 42 differing here.  Spelling the inner gate
   the other way round -- `||' with the 0xF store as the taken arm -- is 10
   differing: jump.c then cross-jumps one of the two `sw $zero, 0x8C($s4)'
   stores the target keeps in both.  Hoisting the 0xFFF7 clear into its own
   `if ((s16)t != 3)' ahead of the flat `&&' gate is 24, and leading with
   `if ((s16)t != 3)' and trailing the call as the final else is 149 words /
   60.

   DEVICE: `x + ((x + x) - x - x)' on func_800A0818's first argument.  It is
   the phantom zero of docs/matching-levers.md :11693 -- cse keeps the three
   insns, flow counts them as references on `x', and combine deletes them
   (simplify_binary_operation's `X * C - X' and `X - X' cases), so it emits
   nothing and ties nothing.  Its job here is allocation only: without it `d'
   outranks the `x'/`t' quantity in global-alloc and the two swap
   callee-saved registers, `d' taking $s0 and `x' $s1 where the target has
   $s1 and $s0 -- 150 words, 28 differing, every one of them that rename
   (and `t', which shares `x's register, renames with it).  What the function
   does without it: the same 150 instructions with two callee-saved registers
   exchanged.  It does NOT warn under -Wall: nothing is read uninitialised.

   Measured and rejected, all 150 words unless stated:
     no device at all                                              28
     no device, declaring `x' and `y' first                         28
     no device, `s16 x, y'                                          28
     no device, `d' copied into a local before the first test       28
     the phantom on `y' as well as `x'                              28
     the phantom on func_8009A3D0's first argument instead           7
     the phantom as its own statement, `x = x + ((x + x) - x - x);'  2
     `__asm__("" ::"r"(x));' after the two assignments               2
     `__asm__("" ::"r"(x));' before the func_8009A3D0 call           2
     the entry gate as `*(s16 *)(d + 0x8A) < d[0x71]'                2
     the entry gate's first test as `d[0x71] > 0', no `(s8)' cast    3
     merging `t' into `x'                                           37
     no device, `u8 x, y'                                151 words, 123
     merging `r' into `y'                                151 words, 107
     a local for the 0x300/0x3000 select                 151 words,  75
     `x' and `y' inline, no locals                       149 words, 125

   `--raw' is 17 differing and every one is a relocation -- the six `jal's,
   the three `j's, and the `%hi'/`%lo' pairs for D_80173FB0, D_80083228
   and the two D_80083462 loads -- so no masked word is hiding a constant
   address.  Hoisting D_80173FB0 into a local and writing the 0x8A bump
   as `*(s16 *)(d + 0x8A) += 1' are each 0 differing too; the forms kept here
   are b07's, for the replicator's sake. */
void func_b43_80171510(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80173FB0)
        {
            *(u8 **)(c + 0x2C) = D_80173FB0;
            func_80047784(c, D_80173FB0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        if ((s16)t == 3)
        {
            if ((D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
            {
                func_b43_80171EC0(p, b, c, d);
            }
            else
            {
                p[0x9A] = 0xF;
            }
        }
        else
        {
            p[0x9A] = 0xF;
            *(u16 *)(p + 0x98) &= 0xFFF7;
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80171768);

/* State 0x10, and hand c the facing cell that the heading at d+0x2A selects
   out of the bank-local byte table D_80173FD0: the relinked twin of the
   resident overlay's func_800D9820 in csrc/ovl_dungeon_800D50FC.c.  The
   relink is the one the rest of this block shows -- a BYTE table pushed
   through func_80047784 (`>> 9 & 7', one `lbu') where the resident has a
   POINTER table pushed through func_8003DB94 (`>> 7 & 0x1C', a `lw').  The
   base is a local because it is used twice, stored at c+0x2C and indexed,
   exactly as in the twin.  25 words, 0 differing, first compile.
   24 copies (dngfam.py).

   THE ONE FACT THAT MATTERS IS STATEMENT ORDER, not the table spelling:
   the two p stores have to come before the c+0x2C store, because 0x10 is
   materialised into $v0 and spent on p+0x9A before the table address is ever
   loaded.  Writing them after the table store is 24 words / 17 differing.
   Hoisting the d+0x2A halfword into an `s16' local is 26 words / 19 -- the
   reload is re-sign-extended instead of feeding the `addu' straight.

   Measured and rejected, all 0 differing at 25 words, so the form kept here
   is the twin's for the replicator's sake:
     no local, the symbol written out twice                         0
     `*(t + idx)' instead of `t[idx]'                               0
     the local assigned inside the store, `= t = D_80173FD0'        0
     the index hoisted into an `s32' local                          0
     reading the base back out of c+0x2C after storing it           0
   Measured and rejected, differing:
     the two p stores moved after the table store    24 words,  17
     the d+0x2A facing hoisted into an `s16' local   26 words,  19

   `--raw' is 6 differing and every one is a relocation -- the `jal', the
   `%hi'/`%lo' pair for D_80173FD0 and the pair for D_80083228 -- so no
   masked word is hiding a constant address. */
void func_b43_80171EC0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_80173FD0;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80171F24);

/* The banked room script's unconditional "same slot" entry, the 70-word
   sibling of func_b03_80171DA0 above (the relinked twin of the resident
   overlay's func_800CA788) with the cursor pick and the heading window cut
   out: past the single D_80083462 0x2000 gate and the two func_800A2B5C
   tests, put the actor into state 0x11, swap the D_80173FB8 facing table
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
   D_80173FB8 used directly at both sites with no `e` local, and one
   shared `goto fail` label in place of the three plain `return;`s. The
   simpler of each pair is kept. 70 words, 0 differing on the first compile. */
void func_b43_80172110(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80173FB8;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b43_80172228(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b43_80171768(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b43_80171768(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

/* The two-gate handler installer of this unit's own landed func_b05_80172670,
   with func_800AD594(d, 4) and func_800A4ACC(d) run between the gates, a
   second store at +0x90, and a D_80083462 0x80 tail. 38 words, 0 differing
   on the first compile.

   The outer gate is called with no arguments at all -- no instruction sets up
   $a0..$a3 before it -- so func_800AB1C0 needs the empty-paren declaration
   this unit already uses for the same shape; its landed definition
   (csrc/ovl_dungeon_800A4B88.c:4277) takes four, and spelled that way the
   call site does not compile ("too few arguments"). The inner gate's result
   is read as a halfword, `sll $v0, 16` with no `sra` and then `blez`, which
   is `(s16)x > 0`.

   THE `blez` BRANCHES PAST THE D_80083462 TAIL, not back to the gate's join:
   its target is the epilogue, so the failing inner gate is an early `return`
   and not a nested `if` that falls into the tail. The second parameter is
   never mentioned. */
void func_b43_8017236C(u8 *p, void *b, u8 *c, u8 *d)
{
    if (func_800AB1C0() != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        *(s32 *)(p + 0x90) = 0;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = 0;
    }
}

/* The relinked twin of the landed func_800D991C in
   csrc/ovl_dungeon_800D50FC.c: a three-state "knocked back and falling"
   handler whose shared tail runs once the +0x96 countdown expires. 217 words,
   0 differing, on the first compile against that twin. Six copies in the
   family, so 1,302 words of yield.

   The twin's two load-bearing spellings are carried over unchanged and are
   the reason this compiled clean, so they are repeated here: the FIRST
   numerator must be three statements -- `u = c[0x24] << 6;
   w = b2 - 0x20; u -= w;` -- or combine folds the subtract into the shift and
   the `lbu' sinks below the `lh'; and the SECOND numerator must stay two
   statements, because there the `lh' has the mflo bubble to fall into. No
   alternative spelling was measured here; it landed first time.

   Seven measured deltas from the resident original, all read off this
   listing:
     - the accumulator is +0xA8, not the resident's +0xA4;
     - the fall seed is 0xFFEE0000, not 0xFFF00000, and the per-frame step is
       0x40000, not 0x48000;
     - the resident's single D_800E262C POINTER table pushed through
       func_8003DB94 becomes TWO bank-local BYTE tables pushed through
       func_80047784 (`>> 9 & 7', one `lbu'): D_80173FD8 in state 0 and
       D_80173FE0 in state 2;
     - state 0 gains a leading `(*(u16 *)(c + 0x14) & 0x6000)' gate, and
       failing it `break's out of the switch to the countdown rather than
       falling through;
     - the tail gains a `(v & 0x2000)' arm that clears bit 15 of
       `*(u16 *)(d + 0x46)', so the resident's THREE NESTED `if's become
       `if / else if / if'. The `else' is what puts the `andi $v0, $v1, 0x410'
       in the `beqz' delay slot; an `&&' chain would still be folded by
       fold_truthop into one `(v & 0x22410) != 0x20000' test;
     - the re-installed handler is this bank's func_b43_80170F68.

   The states FALL THROUGH into one another and each bumps +0x9B separately;
   the countdown and everything after it is the code following the switch,
   which state 2 reaches by `break'. The listing has no `.section .rodata'
   block, so this function owns no jump table -- the switch is a compare
   chain (`beq', `slti', `beq') because the three labels are dense but too
   few.
   THIS COPY'S OWN CONSTANT: 0xffee0000 -> 0xffe80000, 0xa8 -> 0xa0 at all 4 sites, 0x40000 -> 0x60000.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b43_80172404(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;
    u16 y;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_80173FD8;
        func_80047784(c, D_80173FD8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 8;
        b->unk_14 = 0xffe80000;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(p + 0xa0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xa0);
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
            *(s32 *)(p + 0xa0) += b->unk_14;
            b->unk_14 += 0x60000;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xa0);
        if (*(s16 *)(p + 0x96) < 2)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            p[0x9B] += 1;
        }
    case 2:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(u8 **)(c + 0x2C) = D_80173FE0;
            func_80047784(c, D_80173FE0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B] += 1;
        }
        break;
    }

    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_800AD594(d, 4);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) != 0)
    {
        y = *(u16 *)(d + 0x46);
        if ((y & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) = y & 0x7FFF;
        }
    }
    else if ((v & 0x410) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            *(s16 *)(d + 0x2A) =
                func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        func_800A9A04(d);
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
void func_b43_80172768(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_801727CC);

/* The actor's +0x9B state machine for the three-slot carry: state 0 picks the
   slot triple out of (d + 0x46) & 0x3FFF and hands it to func_800A94A0,
   state 1 waits on func_8003F270, state 2 runs the +0x96 countdown and the
   D_80173FB8 facing table, then re-installs func_b43_80170F68.
   302 words, 0 differing. 1 copy (dngfam.py).

   IT OWNS A JUMP TABLE: jtbl_b43_80170838, 7 entries, in this unit's
   .rodata at offset 0x30, for state 0's FIRST switch -- index =
   ((d + 0x46) & 0x3FFF) - 1, cases 1,2,3,5,6,7 over that field with case 4
   falling to `default'. The second switch (the (d + 0x1C) & 0x2000 `else')
   has only three cases and compiles to a compare chain, which is why this
   function gets one new table and not two.

   TWIN: the landed carved sibling func_b35_80172AA8 in csrc/ovl_dng_b35.c,
   316 words. YOURS IS 14 WORDS SHORTER AND THE REMOVED REFERENCES ARE A
   WHOLE BLOCK, not a reference count: b35's `k == 0' arm looks the slot up
   in the D_8006DE24 record and only calls func_800A05A4 when
   `e[0x12] != 2', and here that test is gone -- the arm is the bare
   func_800A05A4 call and the two __builtin_abs stores. So b35's `tbl', `i'
   and `e' locals and its `m != NULL' path are all absent, and with them
   b35's "THE D_8006DE24 RECORD NEEDS BOTH THE POINTER LOCAL AND THE INDEX
   LOCAL" fact, which has nothing to apply to here.

   OF b35's OTHER NAMED FACTS: `k' MUST BE `s16' HELD (it is what buys the
   `addu $v0, $s4, $zero' ahead of the `beqz'); THE INNER THREE-CASE SWITCH
   WRITTEN 3, 2, 1 HELD; `D_8008346C = 0' BEFORE THE D_800814A8 DECREMENT
   HELD. THIS COPY'S OWN CONSTANTS run the other way from b35's: the slot
   compare is `*(s8 *)(c + 0x4) == 8' where b35 has 5, and the countdown
   reload is `*(s16 *)(p + 0x96) = 3' where b35 has 32 -- i.e. this bank
   carries the family representative's values and b35 is the one that
   changed them.

   MEASURED HERE: THE case-2 FACING-TABLE POINTER NEEDS ITS OWN LOCAL.
   Sharing `m' with the `k != 0' arm's D_800814A8 pointer is 302 words and
   3 aligned differences -- the D_800814A8 load and its two uses take $a1
   where the target has $v0 -- and two separate locals is 302 / 0. That is
   the brief's "a twin's separate LOCALS are load-bearing" measured on this
   body.

   `--raw' is 42 differing and every one is a relocation, so no masked word
   is hiding a constant address. */
void func_b43_801729D4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR t;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    s16 k;
    s32 x;
    s32 y;
    s16 n;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                m = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)m;
                q = *(u8 **)(m - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                *(s32 *)(d + 0x60) = func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                x = *(s8 *)(d + 0x72);
                y = *(s8 *)(d + 0x73);
                d[0x72] = __builtin_abs(x);
                d[0x73] = __builtin_abs(y);
            }
            t.vx = *(u16 *)((u8 *)b + 0x2);
            t.vy = *(u16 *)((u8 *)b + 0x6);
            t.vz = *(u16 *)((u8 *)b + 0xA);
            if (func_800A94A0(d, g, k, p + 0x98) == 0)
            {
                break;
            }
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800A56E0(0x703);
            func_800DA840(&t, (g[0] - 1) % 3);
            p[0x9B]++;
        }
        else
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B]++;
    case 2:
        if ((*(s8 *)(c + 0x4) == 8 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 3;
            *(u16 *)(p + 0x98) |= 0x80;
        }
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            *(s16 *)(p + 0x96) = 0;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        e = D_80173FB8;
        if (*(u8 **)(c + 0x2C) != e)
        {
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) > 0)
        {
            d[0x6D] = d[0x6D] - 1;
        }
        d[0x73] = 0;
        d[0x72] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}

/* The banked twin of the resident overlay's func_800B2CA0
   (csrc/ovl_dungeon_800A4B88.c:9260): the same three-state lunge switched on
   the actor's +0x9B, with the same cross-jumped shared exit, the same
   `t = -1; if (flags) t = 8;` shape for +0x96, and the same +0x8C handoff.
   193 words, 0 differing. It differs from the twin in three measured ways:
   the facing index is computed ONCE into a local before the switch (the
   target keeps it in $s4 across both arms and reuses the `sll 1` byte offset,
   where the twin re-reads d + 0x6A four times); case 0 seeds the velocity
   from the tables POSITIVE and shifted 18 rather than negated and shifted 15;
   and the shared exit is func_800AAA54 with this bank's D_80173FC8 table in
   $a3 rather than the twin's D_80083460 teardown.

   THE ONE SPELLING THAT HAD TO BE FOUGHT FOR IS CASE 2's TWO EASES. The
   target computes `c[0x24] << 6` FIRST, then the cell centre, then
   `sll 15`/`sra 1` -- two shifts, not one:

   | spelling of the two case-2 eases                     | words | aligned |
   | as written below: `(x = ...)` inside, then `>> 1`    |  193  |    0    |
   | `((...) * 0x8000) >> 1`, x assigned beforehand       |  193  |    7    |
   | `x = ...;` as its own statement, then `>> 1`         |  193  |    7    |
   | shift into x, then `x >> 1` as a second statement    |  193  |   11    |
   | `/ 2` in place of `>> 1`, assignment either way      |  193  |   31    |
   | no local at all, `>> 1`                              |  191  |    8    |
   | no local at all, `* 0x4000`                          |  191  |   12    |

   Two separate facts are in that table. The EMBEDDED assignment is what
   stops fold-const turning `- (*(s16 *)(b + 2) - 0x20)` into `+ 32`, worth
   two words; written as a preceding statement the fold is also stopped but
   the `lh` is then evaluated before the `lbu`, which is the 7. And `>> 1` is
   NOT interchangeable with `/ 2` here even though both emit `sll 15`/`sra 1`
   -- `/ 2` adds an eight-byte unreferenced stack local (`vars= 8`, frame 0x30
   against the target's 0x28), so all 31 of its differences are the prologue,
   the epilogue and the register renumbering they force. */
void func_b43_80172E8C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 k;
    s32 t;
    s32 x;
    s32 y;

    k = (*(u16 *)(d + 0x6A) >> 9) & 7;
    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[k] << 18;
        b->unk_10 = D_8006CCE8[k] << 18;
        p[0x9B]++;
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80173FC8);
            return;
        }
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 2;
            return;
        }
        t = -1;
        if ((*(s32 *)(d + 0x1C) & 0x228) != 0)
        {
            t = 8;
        }
        *(s16 *)(p + 0x96) = t;
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        /* fallthrough */
    case 1:
        b->unk_0C -= D_8006CCD8[k] << 14;
        b->unk_10 -= D_8006CCE8[k] << 14;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80173FC8);
            return;
        }
        *(s16 *)(p + 0x96) = 8;
        p[0x9B]++;
        break;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) >> 1;
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) >> 1;
        }
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0)
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
        *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        break;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b43_80173190(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b43_801731B0(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b43_801731D0(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b43_801731F0(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}

/* The banked room script's three-state tail, state index at +0x9B: state 0
   re-dresses the renderable from the D_80174000 facing table, state 1 is the
   long refusal chain, and state 2 hands the actor back to
   func_b43_80170F68. The resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c) is the same machine over the same record, and
   its comment covers the shape; read that first. 231 words, 0 differing.

   THE D_80174008 RE-DRESS TAIL IS WRITTEN TWICE AND CROSS-JUMPED BACK INTO
   ONE, and that duplication is the only thing that makes this function land:
   it is worth two references on `p`. The single-copy body -- the obvious one,
   with the chain inside `if (func_80042900(d, 1) != 0) { ... }` and the tail
   after it -- is 231 words with 18 ALIGNED DIFFERENCES, every one of them a
   register: `p` takes $s3 and `b` takes $s2, the other way round from the
   target. The cause is measured, not guessed. At flow time the single-copy
   body gives `p` 13 references over 215 insns and `b` 6 over 60, and
   global.c's allocno_compare ranks on floor_log2(n_refs) * n_refs /
   live_length: p scores 3*13/215 = 0.1814 against b's 2*6/60 = 0.200, loses,
   and is allocated second. Duplicating the tail puts p at 16 references
   (4*16/254 = 0.252) and it wins. 15 references would have been enough;
   14 is 1953 and still loses, so one extra store would not have done it.

   THE TWO COPIES NEED TWO DIFFERENT `&D_80083460` LOCALS. Sharing one
   (`k` in both) is 229 words with 10 differences: one pseudo spanning both
   ranges is allocated to $v1 in each copy, which makes state 1's increment
   `lhu $v0, 0xA($v1)` / `sh $v0, 0xA($v1)` -- identical to state 0's
   decrement, so jump2 merges those two `sh`s as well and takes the
   load-delay `nop` with them. Two locals get $v0 as the state 1 base and
   $v1 as the state 0 one, the `sh`s differ, and only the intended tail
   merges. (Same trap as func_800DA014's header records from the other side.)

   Measured and equal at 231 words / 0 differing, so none of these is the
   reason it lands: nesting the `d + 0x64` test instead of `&&`; reading
   `d + 0x1C` twice instead of into `v`; the chain reached by `goto` past an
   inverted gate instead of nested inside it; `d[0x25] == 0` written with an
   inner `else`; the duplicated tail placed inside the gate rather than
   ahead of the chain. Rejected: `goto` to a shared `p[0x9B] += 1` label
   (229 words, 32 differing -- the bump must be written out in both states
   and cross-jumped); a dead `z = (u32)p;` pair as a cheap reference bump
   (231 words, 18 differing -- cse deletes it before flow counts refs, so
   `p` stays at 13); using D_80082E80 directly instead of through `e`
   (231 words, 25 differing). */
void func_b43_80173228(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
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
        *(u8 **)(c + 0x2C) = D_80174000;
        func_80047784(c, D_80174000[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80174008;
            func_80047784(c, D_80174008[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
                return;
            }
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
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
            func_b43_80173900(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174008;
        func_80047784(c, D_80174008[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
            return;
        }
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        return;
    }
}

/* The banked twin of the resident overlay's func_800DA398
   (csrc/ovl_dungeon_800D50FC.c): the three-state tail of the handler
   installed at p + 0x8C, switching on the state index at p + 0x9B. State 0
   re-dresses the renderable from the D_80174000 facing table, state 1 is the
   long refusal chain, state 2 hands the actor back to func_b43_80170F68.
   This copy adds an arm the resident twin does not have -- state 1 opens with
   the same d[0x25] test that closes it, so the dress/flag/bump tail is
   written out twice.

   THE FACING TABLE MUST BE NAMED DIRECTLY AT ALL THREE SITES, NOT HELD IN A
   `u8 *` LOCAL. One shared local for the three dressings is 207 words, 6
   differing: it is live in more than one basic block, so it becomes a global
   allocno and global.c breaks the tie between the `%hi` and the `%lo` -- the
   target's `lui $a1, %hi` / `addiu $a1, $a1, %lo` comes out as `lui $a0` /
   `addiu $a1, $a0` at each of the three sites, 2 words apiece. Three
   SEPARATE locals, one per site, also measures 0; the bare symbol is kept
   because it needs no declaration. The local is what the landed
   func_b03_80171DA0 above uses, and it is correct there because that
   function dresses only once.

   `&D_80083460` is FOUR separate pointer locals, one per arm. Sharing one
   across the arms is 179 words, 160 differing -- the shared gate load sinks
   and the arms merge.

   Other spellings measured and rejected: `d[0x6D]` read as `u8` rather than
   `*(s8 *)(d + 0x6D)` is 1 differing (`lbu` for the target's `lb`);
   `*(s32 *)(d + 0x64)` for the `lh` of d + 0x64 is 1 differing; dropping the
   `(s16)` cast on the first func_800A2C34 result is 206 words, 111
   differing; writing `p[0x9B] += 1;` after the switch with `break` in the
   reaching arms instead of as a `goto` label inside it is 207 words, 177
   differing. Measured and equal at 0 differing, so neither is the reason it
   lands: re-reading d + 0x1C at both flag tests instead of through the `v`
   local, and the three separate table locals above.

   207 words, 0 differing. */
void func_b43_801735C4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174000;
        func_80047784(c, D_80174000[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80173FF8;
            func_80047784(c, D_80173FF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            goto bump;
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
            func_b43_80173900(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80173FF8;
        func_80047784(c, D_80173FF8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x208;
        *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
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
   call emits the bare `jal` either way); hoisting D_80173FB0 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b43_80173900(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174000 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80173FB0;
        func_80047784(c, D_80173FB0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b43_801739C8(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b43_80170F68;
        }
    }
}

/* 14 words, 0 differing. */
void func_b43_80173A2C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}

/* 14 words, 0 differing. */
void func_b43_80173A64(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}

/* 14 words, 0 differing. */
void func_b43_80173A9C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80173AD4);

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b43_80173AD4. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b43_80170854[] = {
    0x81,
    0x40,
    0x82,
    0x97,
    0x82,
    0x85,
    0x82,
    0x8E,
    0x82,
    0x94,
    0x81,
    0x40,
    0x82,
    0x82,
    0x82,
    0x85,
    0x82,
    0x92,
    0x82,
    0x93,
    0x82,
    0x85,
    0x82,
    0x92,
    0x82,
    0x8B,
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b43", func_b43_80173C5C);

/* 14 words, 0 differing. */
void func_b43_80173F40(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}

/* 14 words, 0 differing. */
void func_b43_80173F78(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b43_80170F68;
    }
}
