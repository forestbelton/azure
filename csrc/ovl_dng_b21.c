#include "common.h"

void func_b21_80174B68(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80176328[];

void func_8009A3D0(s32, s32, s32);
s32 func_800A0818(s16, s16, s16, s16, u16 *);
s32 func_8009A66C(u32, u8 *, u8 *, s16);
void func_8009A21C(s32, s32, s32);
void func_b21_80171DF4(u8 *, UnkVec *, u8 *, u8 *);
extern void (*D_80174198[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
extern u8 D_801762C8[];

extern u8 D_80176368[];
extern u8 D_80176360[];
void func_b21_80173B98();

extern u8 D_80176308[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
u16 func_800A6D30();
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
extern u32 D_800814A8;
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
s32 func_8009A180(u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b21_80173720(u8 *p, u8 *b, u8 *c, u8 *d);
extern void (*D_80173CEC[])(u8 *, UnkVec *, u8 *, u8 *);
u16 func_800A6D30(void);
s32 func_8009A180(u8 *a, u8 *b);
extern u8 D_80082E80[];
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern u8 D_80176300[];
void func_80047784(u8 *, s32, s32);
void func_b21_80170F20();

extern u8 D_80176320[];

extern Unk80083462 D_80083460;
extern s16 D_80083228;
extern u8 D_801762C0[];
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b21_80171468(u8 *, void *, void *, u8 *);

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801708A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80170AE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80170F20);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80171468);

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_801762C8 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b21_80171DF4.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b21_80171DF4 is called only on
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
void func_b21_80171BB0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_801762C8)
        {
            *(u8 **)(c + 0x2C) = D_801762C8;
            func_80047784(c, D_801762C8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b21_80171DF4(p, b, c, d);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80171DF4);

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_801762C0 facing table and re-aim through func_8009C93C.

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
s32 func_b21_80171E58(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    if ((u16)(0x3F + -func_800A0134(t, d)) > 0x7E)
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
    e = D_801762C0;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_8017201C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80172134);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801721B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80172538);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801725B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80172798);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80172C10);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b21_80172FA0(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b21_80172FC0(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b21_80172FE0(void)
{
    func_800AB778();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173000);

/* The banked room script's three-state tail, state index at +0x9B: state 0
   re-dresses the renderable from the D_80176300 facing table, state 1 is the
   long refusal chain, and state 2 hands the actor back to
   func_b21_80170F20. The resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c) is the same machine over the same record, and
   its comment covers the shape; read that first. 231 words, 0 differing.

   THE D_80176308 RE-DRESS TAIL IS WRITTEN TWICE AND CROSS-JUMPED BACK INTO
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
void func_b21_80173054(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80176300;
        func_80047784(c, D_80176300[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80176308;
            func_80047784(c, D_80176308[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b21_80170F20;
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
            func_b21_80173720(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80176308;
        func_80047784(c, D_80176308[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b21_80170F20;
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
        *(u32 *)(p + 0x8C) = (u32)func_b21_80170F20;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801733F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173720);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173830);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801738B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173904);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173958);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801739AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173A00);

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
s32 func_b21_80173A54(u8 *a, void *b, void *c, s32 d)
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
        func_b21_80171468(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b21_80171468(p, b, c, p);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80173B98);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_8017405C);

/* The banked room script's step-the-walk handler, the relinked twin of the
   resident overlay's func_800D904C in csrc/ovl_dungeon_800D50FC.c: advance
   the actor one cell of its queued path, swap in the D_80176328 facing
   table, re-aim the sprite, and either stop at state 0xF or hand on to
   func_b21_80174B68.  145 words, 0 differing.

   THE GATE IS THE TWIN'S, INVERTED.  func_800D904C sets state 0xF on
   `(s16)t == 3 && (D_80083462 & 0x80 || c[0x14] & 0x8000)`; here the `t`
   test goes the other way -- func_b21_80174B68 is called only on
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
void func_b21_80174924(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80176328)
        {
            *(u8 **)(c + 0x2C) = D_80176328;
            func_80047784(c, D_80176328[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b21_80174B68(p, b, c, d);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80174B68);

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80176320 facing table and re-aim through func_8009C93C.

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
s32 func_b21_80174BCC(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    if ((u16)(0x3F + -func_800A0134(t, d)) > 0x7E)
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
    e = D_80176320;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80174D90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80174EA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80174F24);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_801752A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80175480);

/* The banked room script's three-state tail, state index at +0x9B: state 0
   re-dresses the renderable from the D_80176360 facing table, state 1 is the
   long refusal chain, and state 2 hands the actor back to
   func_b21_80173B98. The resident overlay's func_800DA014
   (csrc/ovl_dungeon_800D50FC.c) is the same machine over the same record, and
   its comment covers the shape; read that first. 231 words, 0 differing.

   THE D_80176368 RE-DRESS TAIL IS WRITTEN TWICE AND CROSS-JUMPED BACK INTO
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
void func_b21_801758F8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80176360;
        func_80047784(c, D_80176360[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80176368;
            func_80047784(c, D_80176368[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b21_80173B98;
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
            func_b21_80173720(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80176368;
        func_80047784(c, D_80176368[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b21_80173B98;
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
        *(u32 *)(p + 0x8C) = (u32)func_b21_80173B98;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80175C94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80175DA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80175E14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80175E6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b21", func_b21_80176028);
