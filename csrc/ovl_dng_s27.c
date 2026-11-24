#include "common.h"

s32 func_8004491C(UnkNode *, u32);
void func_800478B8(u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FC64(s32);
void func_s27_80024050(u8 *p, u8 k);
void func_s27_800240EC(void);
void func_s27_8002417C(u8 *p, UnkVec *b, u8 *c, u8 *d);
UnkNode *func_s27_800241F8(u8 *v, UnkVec *q);
void func_s27_800249B4(u8 *, UnkVec *, u8 *);
void func_s27_80024B34(u8 *, UnkVec *, u8 *);
void func_s27_80024C84(u8 *, UnkVec *, u8 *);
s32 func_s27_80024DF4(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];
extern u32 D_800814A0;
extern u8 D_800DEC70[];

/* Raise the actor's +0x64 pool by func_800A6870's scale of the second
   parameter plus 8, a quarter more when the actor's +0x28 has bit 2, then
   republish it with func_800AD568, float the 0x8004 number and refresh with
   func_800AD4D0. All of it gated on func_8009D218 coming back zero.
   39 words, 0 differing.

   No carved twin above 0.500 (dngtwin.py); main.bin:func_804025F8 sits at
   0.667 and its code was read for the shape, but NO SYMBOL NAME is taken
   from it -- main.bin is not resident alongside DUNGEON.BIN, so a name out
   of its slot would be a lie here.

   TWO SPELLINGS TAKEN FROM csrc/ovl_dng_s18.c's func_s18_800240B8 AND
   RE-MEASURED HERE: the second parameter is `u8', which is what makes the
   `andi $a0, $s1, 0xFF' ahead of func_800A6870 the parameter's own
   truncation rather than a mask; and func_8009D218 is reached through the
   project's TWO-argument alias, because the landed three-argument
   definition's third argument is not passed at this site.

   THE SPELLING THAT TOOK THIS FROM 30 DIFFERING TO 0 IS THE PAIR OF `s16'
   LOCALS WITH THE PLAIN `u = t;' AHEAD OF THE GATE. The target keeps the
   scale in $v1 and the applied amount in $a1 and copies one to the other in
   the `beqz' delay slot, so they are TWO variables and gcc must not coalesce
   them. Measured, all on this body:
     `s16 t; s16 u;' with `u = t;' before an else-less gate   39 w,  0
     `s32 t; s16 u;' with `((s16)t >> 2)'                     39 w,  0
     `s32 t; s32 u;' -- gcc coalesces the copy away           38 w, 28 (8 aligned)
     one variable, `t += (s16)t >> 2;' then `u = t;'          38 w, 28
     `s16 t; s32 u;' with a two-armed if/else                 40 w, 29 (11 aligned)
     `s16 t; s32 u;' with `u = t + (t >> 2)' in a two-armed
       if/else -- the extra sign extension of the addend      42 w, 30 (15 aligned) */
void func_s27_80024050(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 4) == 0)
    {
        t = func_800A6870(k) + 8;
        u = t;
        if ((p[0x28] & 4) != 0)
        {
            u = t + (t >> 2);
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

/* Paint the two 1x1 VRAM texels this effect's sprite samples: transparent
   black at (0x340, 0x100) and opaque white at (0x340, 0x180), then DrawSync.
   36 words, 0 differing.

   func_800672D8 and func_80067014 ARE `LoadImage' AND `DrawSync' --
   splat/common_syms.txt names both, they come from psyq/include/libgpu.h
   through common.h, and they need no declaration here. The pixel is ONE
   halfword on the stack whose address `&px' is what gcc keeps in $s1 across
   both calls. */
void func_s27_800240EC(void)
{
    RECT r;
    u16 px;

    px = 0;
    r.x = 0x340;
    r.y = 0x100;
    r.w = 1;
    r.h = 1;
    LoadImage(&r, (u_long *)&px);
    px = 0xFC00;
    r.x = 0x340;
    r.y = 0x180;
    r.w = 1;
    r.h = 1;
    LoadImage(&r, (u_long *)&px);
    DrawSync(0);
}

/* The 0x212 sprite's own step function: bump the owning node's +0x14 frame
   counter, hand the renderable to func_800478B8, and when its +0x14 reports
   0x6000 mark the owning actor retired and set the same bit in D_800814A0.
   31 words, 0 differing.

   This is the landed func_s10_80024374 in csrc/ovl_dng_s10.c (0.862 by
   dngtwin.py) plus the four-word counter bump on `*(u8 **)(p + 0x0)' at the
   front; that twin's note about D_800814A0 being the PLAIN scalar rather than
   an array alias holds here too and is what gives the `lui' / `lw %lo' pair.
   Measured and FREE at 31 words / 0 differing, so it is NOT the reason this
   lands: reading the counter back through `*(s16 *)' instead of `*(u16 *)'
   for the `+ 1'. */
void func_s27_8002417C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0x0) + 0x14) = *(u16 *)(*(u8 **)(p + 0x0) + 0x14) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that carries one of the effect's ten sprites: white
   renderable, the 0x7E09 sprite, 0x1000 scale both ways, the +0x14 0x100 bit,
   D_80045340 as its dresser and func_s27_8002417C as its per-frame step, with
   the caller's node saved at +0x20 and the position copied off q. Returns the
   node, or NULL when the allocator refuses. 58 words, 0 differing.

   THE ONE SPELLING FACT, measured on this body: the NULL exit is an `else'
   arm AFTER the whole gate, not an early return. Written as
   `if (n == NULL) { return NULL; }' ahead of the body it is 58 words and 5
   aligned differences (38 unaligned) -- jump.c's `if (foo) bar; else break'
   transform then swaps the two blocks and inverts the branch, because the
   body ends in a jump to the return label and the early-return block is a
   simplejump. With the arms this way round the SAME transform fires in the
   other direction and lands the target's layout exactly.

   Derived from the landed func_s14_8002425C in csrc/ovl_dng_s14.c (0.828 by
   dngtwin.py) with that body's three devices stripped: no `+0x24 = 0' store,
   no rand() jitter and no 0x60 bit on +0x10. That twin's own documented fact
   -- the +0x20 store FIRST inside the gate -- is INVERTED here: written first
   it is 10 aligned differences, and third (after the +0x10 step pointer and
   the `m = n->unk_0C' load) it is 5. A separate local for the second
   `n->unk_08' pointer, rather than reusing `m', is worth another 5. */
UnkNode *func_s27_800241F8(u8 *v, UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s27_8002417C;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DEC70, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(s16 *)(m + 0x12) = 0x7E09;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
    }
    else
    {
        return NULL;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_800242E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_8002443C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_80024610);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_800249B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_80024B34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_80024C84);

/* The effect's dresser callback: states 4, 5 and 6 of the node's +0xA hand
   the three arguments straight through to func_s27_800249B4 /
   func_s27_80024B34 / func_s27_80024C84; every other state does nothing.
   Always returns 0. 28 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s27_80024008, EIGHT ENTRIES: 0..3 and 7 are the
   default label .Ls27_80024E54, 4 is .Ls27_80024E2C, 5 is .Ls27_80024E3C and
   6 is .Ls27_80024E4C.

   THE ONE SPELLING FACT: THE FIVE EMPTY CASES ARE LOAD-BEARING. With only
   cases 4, 5 and 6 written, three labels are under gcc 2.7.2's
   case_values_threshold and expand_end_case emits a COMPARISON TREE instead
   of the table -- 32 words, 20 differing (19 aligned), and the unit loses the
   rodata table the ROM has. Spelling 0..3 and 7 as empty `break' arms puts
   the range at 0..7, which is what gives the target's bare `sltiu $v0, $v1,
   0x8' with no minval subtraction.

   No twin above 0.62 anywhere (dngtwin.py, xsibs.py); written from the
   listing and the three callees' prologues, which is where the arity comes
   from -- each takes $a0/$a1/$a2 and this function sets NONE of them at the
   call, so it passes its own first three parameters through unchanged. */
s32 func_s27_80024DF4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
    case 1:
    case 2:
    case 3:
        break;
    case 4:
        func_s27_800249B4(p, b, c);
        break;
    case 5:
        func_s27_80024B34(p, b, c);
        break;
    case 6:
        func_s27_80024C84(p, b, c);
        break;
    case 7:
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s27", func_s27_80024E64);
