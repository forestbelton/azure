#include "common.h"

s32 rand(void);

UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
void func_s29_80024024_3(u8 *, u8, u8 *) __asm__("func_s29_80024024");
UnkNode *func_s29_80025344(u8 *p, UnkVec *b);
void func_s29_80024F4C(u8 *p, UnkVec *b, u8 *c);
UnkNode *func_s29_800247B0(u8 *p, UnkVec *b, s16 d);
UnkNode *func_s29_800249C4(u8 *p, UnkVec *b, s16 d, s16 e);
UnkNode *func_s29_80024D2C(u8 *p, UnkVec *b, s16 d, s16 e);
void func_s29_80024088(u8 *, UnkVec *, u8 *);
s32 func_s29_800241CC(u8 *p, UnkVec *b, UnkSlot *c);
void func_s29_800248C0(u8 *p, UnkVec *b, u8 *c);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
s32 func_800644B8(s32);
s32 func_80064584(s32);
extern u8 D_80025E04[];
extern u8 D_800DEC70[];
void func_s29_80024BB0(u8 *p, UnkVec *b, u8 *c);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern u8 D_800DDC40[];
extern u8 D_80045340[];
extern u8 D_s29_80025DF8[];
void func_80067F20(u32 *, s32, s32, s32, s32);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);

/* Raise the actor's +0x64 pool by a flat 0x200, republish it with
   func_800AD568, float the 0x8004 number at the new height and refresh with
   func_800AD4D0 -- all gated on func_8009D218 coming back zero for gate 2.
   25 words, 0 differing, first compile.

   TWIN: the landed func_s26_8002403C in csrc/ovl_dng_s26.c, which is this
   body plus a func_800A6870 scale of its second parameter; this copy adds a
   constant instead, so the `s16 t'/`s16 u' pair and the whole doubling idiom
   its comment prices DO NOT ARISE here.  What carries is the two-argument
   func_8009D218 alias with gate 2 and the `ori'-form 0x8004 (a `u16' first
   parameter on func_800B4C7C).

   THE DEFINITION TAKES ONE PARAMETER AND ITS ONLY CALLER PASSES THREE.
   func_s29_800256BC's case 4 calls it as
   `func_s29_80024024_3(*(u8 **)(s + 0x60), p[0x9], s)' through the unit-local
   __asm__ alias in the declaration block, which is how csrc/ovl_dng_s26.c and
   csrc/ovl_dng_s28.c handle the same shape.  The body reads only $a0, so the
   extra two arguments are the caller's business; relcand reports the alias as
   a call mismatch, which is expected and is the alias, not a wrong callee. */
void func_s29_80024024(u8 *p)
{
    if (func_8009D218_2(p, 2) == 0)
    {
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 0x200;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s29", func_s29_80024088);

/* The per-slot walk of the fan node func_s29_800247B0 spawns, and the only
   renderer in this object: FOUR gouraud-textured POLY_GT4 blades per call,
   each 0x34 bytes off the +0x8D0 cursor, with a 0x3F-square UV window whose
   v row is the payload's `+0x4 % 4 * 8', the payload's +0xC clut and its
   +0xE byte in all three colour channels.  The blade's four corners are the
   mover's position offset by the camera-facing sin/cos of the payload's +0x8
   span -- `i' choosing which diagonal and which z sign -- and are projected
   by RotAverage4 through the 0x1F800000 scratchpad; when the returned otz is
   in range the quad is hung on the OT row between two func_80067F20 mode
   primitives, one per texture window.  377 words, 0 differing.

   SCRATCHPAD AND DONORS.  `u8 *sp = (u8 *)0x1F800000' with explicit offsets
   rather than a typedef: this body's layout (four SVECTORs at +0x64..+0x7F,
   RotAverage4's four sxy at +0xD8..+0xE7, its p/flag at +0x84/+0x88, the otz
   at +0xB4 and the OT base at +0x18) matches none of the eleven existing
   `*SP' structs, and structs.h's own note says each function sees a
   different subset.  The idioms came from the landed func_b09_801708A4
   (csrc/ovl_dng_b09.c) -- `addPrim(ot + otz * 4, prim)', the
   `(u32)otz < 0x1E0' range test, func_80067F20 with GetTPage -- and from
   func_b06_80174334 (csrc/ovl_dng_b06.c), the POLY_GT4/RotAverage4 pair in
   another bank.  Neither is a dngtwin hit: dngtwin finds nothing at 0.70 and
   the callee-set intersection finds nothing either, because RotAverage4 and
   GetTPage are spelled by their REAL NAMES everywhere in csrc/ and the
   address form the tool searches for occurs nowhere.  Grepping the real
   names is what found both donors.

   THE TWO FACTS THAT LANDED IT, from 162 aligned rows on the first compile:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | A SEPARATE LOCAL FOR EACH OF THE TWO MODE PRIMITIVES  |  377  |   0  |
   |   ... one `q' reused for both                         |  377  | 152  |
   | the dress block in UV-PAIR order: clut, then          |       |      |
   |   (0xC,0xD), (0x18,0x19), (0x24,0x25), (0x30,0x31),   |       |      |
   |   then the three +0xE colour bytes, then the zeros    |  377  |   0  |
   |   ... the same stores grouped by constant/variable    |  377  |  17  |
   |   ... clut second, or before +0x30                    |  377  |  13-19|
   | `RECT r2' declared BEFORE `RECT r1'                   |  377  |   0  |
   |   ... the other order                                 |  377  |  10  |

   THE TWO-LOCAL FACT IS THE BIG ONE AND IT IS AN ALLOCATION FACT WITH A PASS
   EXPLANATION, not a sweep result.  With one `q' the whole body is shifted
   one callee-saved register: the prim cursor takes $s0, the scratchpad base
   $s1 and `q' $s2, where the target has $s0 shared between the camera angle
   and the mode primitives, $s1 for the cursor and $s2 for the base.  Two
   locals make each mode primitive BLOCK-LOCAL, so local-alloc -- which runs
   before global -- assigns both to $s0 and penalises $s0 for the global
   allocnos, and the remaining three fall into the target's registers.  152
   rows for one declaration.

   THE DRESS ORDER WAS READ, NOT SWEPT, and the reading is reusable: loads and
   stores through DIFFERENT base registers cannot be reordered by sched (no
   alias exemption applies -- both vary), while stores through the SAME base
   at distinct offsets commute freely.  So in the listing the LOAD positions
   are source order and the stores between two consecutive loads are the
   statements between them; walking that out gives the four UV pairs in
   ascending corner order with the clut first, which is also the only order
   anyone would write by hand.  The brief's "a listing's store order is not
   the source's" cuts the other way here: the loads pin it.

   THREE READING NOTES.  `s32 k' for the camera angle (`g->unk_C8' is `s16'):
   an `s16' local costs `lhu'/`sll 16'/`sra 16' where the target has one
   `lh'.  `u = ... % 4 << 3' and not `* 8': the MULT narrows through
   convert.c to the `s16' destination and emits `sll 16'/`srl 13'.  And the
   `lui $v0, 0x80070000' artefact the other banks' renderers record does not
   appear here -- the dead `lui` at 800259C8 in the sibling listing is
   func_s29_800256BC's, not this body's. */
s32 func_s29_800241CC(u8 *p, UnkVec *b, UnkSlot *c)
{
    u8 *sp = (u8 *)0x1F800000;
    Unk80083160 *g;
    u8 *m;
    s32 i;
    s32 dx;
    s32 dy;
    s16 u;
    RECT r2;
    RECT r1;
    s32 k;

    g = &D_80083160;
    k = g->unk_C8;
    *(u8 **)(sp + 0x18) = D_80083160.unk_00 + 0xB0;
    dx = (u32)((func_80064584(k) >> 4) * *(s16 *)(p + 0x8)) >> 8;
    dy = (u32)((func_800644B8(k) >> 4) * *(s16 *)(p + 0x8)) >> 8;
    u = *(s16 *)(p + 0x4) % 4 << 3;
    for (i = 0; i < 4; i++)
    {
        m = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)m + 0x34;
        m[0x3] = 0xC;
        m[0x7] = 0x3E;
        *(u16 *)(m + 0x1A) = GetTPage(0, 1, 0x280, 0x100);
        *(s16 *)(m + 0xE) = *(u16 *)(p + 0xC);
        m[0xC] = 0;
        m[0xD] = u;
        m[0x18] = 0x3F;
        m[0x19] = u;
        m[0x24] = 0x3F;
        m[0x25] = u + 0x3F;
        m[0x30] = 0;
        m[0x31] = u + 0x3F;
        m[0x4] = p[0xE];
        m[0x5] = p[0xE];
        m[0x6] = p[0xE];
        m[0x10] = 0;
        m[0x11] = 0;
        m[0x12] = 0;
        m[0x1C] = 0;
        m[0x1D] = 0;
        m[0x1E] = 0;
        m[0x28] = 0;
        m[0x29] = 0;
        m[0x2A] = 0;
        switch (i)
        {
        case 0:
            *(s16 *)(sp + 0x64) = *(s16 *)(sp + 0x74) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(sp + 0x6C) = *(s16 *)(sp + 0x7C) = *(u16 *)((u8 *)b + 0x2) + dx;
            *(s16 *)(sp + 0x66) = *(s16 *)(sp + 0x76) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(sp + 0x6E) = *(s16 *)(sp + 0x7E) = *(u16 *)((u8 *)b + 0x6) - dy;
            *(s16 *)(sp + 0x68) = *(s16 *)(sp + 0x70) = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)(sp + 0x78) = *(s16 *)(sp + 0x80) =
                *(u16 *)((u8 *)b + 0xA) + *(u16 *)(p + 0x8);
            break;
        case 1:
            *(s16 *)(sp + 0x64) = *(s16 *)(sp + 0x74) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(sp + 0x6C) = *(s16 *)(sp + 0x7C) = *(u16 *)((u8 *)b + 0x2) - dx;
            *(s16 *)(sp + 0x66) = *(s16 *)(sp + 0x76) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(sp + 0x6E) = *(s16 *)(sp + 0x7E) = *(u16 *)((u8 *)b + 0x6) + dy;
            *(s16 *)(sp + 0x68) = *(s16 *)(sp + 0x70) = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)(sp + 0x78) = *(s16 *)(sp + 0x80) =
                *(u16 *)((u8 *)b + 0xA) + *(u16 *)(p + 0x8);
            break;
        case 2:
            *(s16 *)(sp + 0x64) = *(s16 *)(sp + 0x74) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(sp + 0x6C) = *(s16 *)(sp + 0x7C) = *(u16 *)((u8 *)b + 0x2) + dx;
            *(s16 *)(sp + 0x66) = *(s16 *)(sp + 0x76) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(sp + 0x6E) = *(s16 *)(sp + 0x7E) = *(u16 *)((u8 *)b + 0x6) - dy;
            *(s16 *)(sp + 0x68) = *(s16 *)(sp + 0x70) = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)(sp + 0x78) = *(s16 *)(sp + 0x80) =
                *(u16 *)((u8 *)b + 0xA) - *(u16 *)(p + 0x8);
            break;
        case 3:
            *(s16 *)(sp + 0x64) = *(s16 *)(sp + 0x74) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(sp + 0x6C) = *(s16 *)(sp + 0x7C) = *(u16 *)((u8 *)b + 0x2) - dx;
            *(s16 *)(sp + 0x66) = *(s16 *)(sp + 0x76) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(sp + 0x6E) = *(s16 *)(sp + 0x7E) = *(u16 *)((u8 *)b + 0x6) + dy;
            *(s16 *)(sp + 0x68) = *(s16 *)(sp + 0x70) = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)(sp + 0x78) = *(s16 *)(sp + 0x80) =
                *(u16 *)((u8 *)b + 0xA) - *(u16 *)(p + 0x8);
            break;
        }
        *(s32 *)(sp + 0xB4) = RotAverage4(
            (SVECTOR *)(sp + 0x64), (SVECTOR *)(sp + 0x6C), (SVECTOR *)(sp + 0x74),
            (SVECTOR *)(sp + 0x7C), (long *)(sp + 0xD8), (long *)(sp + 0xDC),
            (long *)(sp + 0xE0), (long *)(sp + 0xE4), (long *)(sp + 0x84),
            (long *)(sp + 0x88));
        *(s16 *)(m + 0x8) = *(u16 *)(sp + 0xD8);
        *(s16 *)(m + 0xA) = *(u16 *)(sp + 0xDA);
        *(s16 *)(m + 0x14) = *(u16 *)(sp + 0xDC);
        *(s16 *)(m + 0x16) = *(u16 *)(sp + 0xDE);
        *(s16 *)(m + 0x20) = *(u16 *)(sp + 0xE0);
        *(s16 *)(m + 0x22) = *(u16 *)(sp + 0xE2);
        *(s16 *)(m + 0x2C) = *(u16 *)(sp + 0xE4);
        *(s16 *)(m + 0x2E) = *(u16 *)(sp + 0xE6);
        if ((u32) * (s32 *)(sp + 0xB4) < 0x1E0)
        {
            u8 *q;
            u8 *q2;

            r1.x = r1.y = 0;
            r1.w = r1.h = 0xFF;
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0xC;
            func_80067F20((u32 *)q, 0, 0, GetTPage(0, 1, 0x280, 0x100), (s32)&r1);
            addPrim(*(u8 **)(sp + 0x18) + *(s32 *)(sp + 0xB4) * 4, q);
            addPrim(*(u8 **)(sp + 0x18) + *(s32 *)(sp + 0xB4) * 4, m);
            r2.x = 0;
            r2.y = 0x80;
            r2.w = r2.h = 0x40;
            q2 = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q2 + 0xC;
            func_80067F20((u32 *)q2, 0, 0, GetTPage(0, 1, 0x280, 0x100), (s32)&r2);
            addPrim(*(u8 **)(sp + 0x18) + *(s32 *)(sp + 0xB4) * 4, q2);
        }
    }
    return 0;
}

/* Spawn the 0x212 fan node: func_s29_80024088 as the +0x10 step and
   func_s29_800241CC as the per-slot walk (both this object's own), the
   caller's +0x0 node at the payload's +0x0, a zeroed +0x4/+0x6/+0x8, the
   caller's angle at +0xA and 0x7DCF at +0xC, 0x1000 scale on the renderable,
   and the caller's mover copied with its z nudged by `d * 3 / 4'.
   68 words, 0 differing, first compile.

   The declaration block's `s16 d' is what the `sll 16'/`sra 16' ahead of the
   `* 3' asks for: the payload store at +0xA takes the parameter register
   straight (`sh $s1'), and only the arithmetic sign-extends.  Its caller
   passes a `D_800DDC40' byte, which needs no extension of its own.

   UNMEASURED: this landed on the first compile, so nothing in it is priced.
   The spellings most likely to be load-bearing, in case a later body needs
   them, are the `k = n->unk_08' alias for the mover copy (the +0xA
   read-modify-write is named off it, not off `n') and the +0x1E-before-+0x1C
   scale order, which is priced at 4 rows two functions up in this file. */
UnkNode *func_s29_800247B0(u8 *p, UnkVec *b, s16 d)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *k;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s29_80024088;
        func_8004491C(n, (u32)func_s29_800241CC);
        q = (u8 *)n + 0x20;
        *(u8 **)q = *(u8 **)p;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0;
        *(s16 *)(q + 0x8) = 0;
        *(s16 *)(q + 0xA) = d;
        *(s16 *)(q + 0xC) = 0x7DCF;
        q[0xE] = 0;
        m = (u8 *)n->unk_0C;
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        k = n->unk_08;
        *k = *b;
        *(s16 *)((u8 *)k + 0xA) = *(u16 *)((u8 *)k + 0xA) + d * 3 / 4;
        return n;
    }
    return NULL;
}

/* The step of the node func_s29_800249C4 spawns: bump the owner's +0x14 and
   the payload's +0x6, let func_800478B8 advance the slot, and when the
   slot's +0x14 has either 0x6000 bit set, count a hit at +0x4 -- retiring on
   the third -- and otherwise re-blit the renderable from D_80025E04 and
   ROTATE its colour channel, moving the 0x80 from +0xC to +0xD to +0xE and
   back.  65 words, 0 differing, first compile.

   `b' is unused: the step signature is fixed at (p, b, c) by the +0x10
   handler slot and this body only needs the payload and the slot.

   UNMEASURED (first compile).  The one spelling worth naming is the hit test
   `*(s16 *)(p + 0x4) >= 3' reading the value back through the same cast it
   was just stored with, which is what gives the `sll 16'/`sra 16' pair on
   the stored register rather than a reload. */
void func_s29_800248C0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        if (*(s16 *)(p + 0x4) >= 3)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            func_8003DB94(c, D_80025E04, 0);
            if (c[0xC] != 0)
            {
                c[0xC] = 0;
                c[0xD] = 0x80;
            }
            else if (c[0xD] != 0)
            {
                c[0xD] = 0;
                c[0xE] = 0x80;
            }
            else
            {
                c[0xE] = 0;
                c[0xC] = 0x80;
            }
        }
    }
}

/* Spawn the 0x212 shard node: the renderable is blitted from D_80025E04 at
   0x7DCF with the +0x14 0xC/0x100 and +0x10 0x60 bits, a random rotation and
   0x2000 scale, and its colour is a PURE channel chosen by `d % 3' -- 0x80
   into one of +0xC/+0xD/+0xE and 0 into the other two.  func_s29_800248C0 is
   the step; the mover is the caller's with its z raised by `e'.
   123 words, 0 differing, first compile.

   THE THREE-ARM SWITCH IS A COMPARE CHAIN, NOT A TABLE: three cases is below
   gcc 2.7.2's `case_values_threshold', and `d % 3' is signed so -2 and -1
   fall to a default that needs no arm.  The arms are written ascending 0..1..2
   because a compare-chain switch emits its arms in SOURCE order and the
   target's three blocks ascend.

   THE CHANNEL WRITES GO 0x80 FIRST, THEN THE OTHER TWO DESCENDING, and that
   is what lets jump2 cross-join cases 1 and 2 on their shared `m[0xC] = 0'
   while case 0 jumps past it.  Written any other way the three arms stop
   sharing a tail and the body grows.  (Unmeasured -- first compile -- but the
   descending order is also csrc/ovl_dng_s28.c's priced 4-row fact for the
   same three bytes.) */
UnkNode *func_s29_800249C4(u8 *p, UnkVec *b, s16 d, s16 e)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *k;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s29_800248C0;
        q = (u8 *)n + 0x20;
        *(u8 **)q = *(u8 **)p;
        *(s16 *)(q + 0x6) = 0;
        *(s16 *)(q + 0x8) = 0;
        m = (u8 *)n->unk_0C;
        switch (d % 3)
        {
        case 0:
            m[0xC] = 0x80;
            m[0xE] = 0;
            m[0xD] = 0;
            break;
        case 1:
            m[0xD] = 0x80;
            m[0xE] = 0;
            m[0xC] = 0;
            break;
        case 2:
            m[0xE] = 0x80;
            m[0xD] = 0;
            m[0xC] = 0;
            break;
        }
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025E04, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x2000;
        *(s16 *)(m + 0x1C) = 0x2000;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *b;
        *(s16 *)((u8 *)k + 0xA) = *(u16 *)((u8 *)k + 0xA) - e;
        return n;
    }
    return NULL;
}

/* The step of the node func_s29_80024D2C spawns: bump the payload's +0x4 and
   retire once it passes the +0x6 budget, otherwise drive the mover on a
   circle -- the +0xA radius through sin/cos of `+0x4 * 128' for the z half
   and of the +0xC angle, which winds by 0x80 a frame, for x and y -- then
   advance the slot and re-blit it from D_800DEC70 while either 0x6000 bit is
   set.  95 words, 0 differing, first compile.

   THE SHIFTS ARE SHIFTS, NOT DIVISIONS: `func_80064584(...) >> 4' and
   `... >> 8' are `sra', and a signed `/ 16' or `/ 256' would each cost a bias
   correction.  func_800644B8/func_80064584 are the libgte sin/cos pair this
   block spells by address (they are above 0x8006a3a8 in common_syms.txt only
   as func_, so the address form is what the rest of csrc/ uses).

   `*(s16 *)(p + 0x8)' IS WRITTEN TWICE AND RELOADED, once for each axis
   pair; the second write is what the third and fourth blocks read.  That is
   source, not an artefact: the store to b invalidates the cse entry between
   them.  UNMEASURED -- first compile. */
void func_s29_80024BB0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    if (*(s16 *)(p + 0x4) > *(s16 *)(p + 0x6))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        *(s16 *)(p + 0x8) =
            (func_80064584(*(s16 *)(p + 0x4) * 128) >> 4) * *(s16 *)(p + 0xA) >> 8;
        b->unk_08 = b->unk_14 - (*(s16 *)(p + 0x8) << 16);
        *(s16 *)(p + 0x8) =
            (func_800644B8(*(s16 *)(p + 0x4) * 128) >> 4) * *(s16 *)(p + 0xA) >> 8;
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x80;
        b->unk_00 =
            b->unk_0C + ((func_80064584(*(s16 *)(p + 0xC)) >> 4) * *(s16 *)(p + 0x8) << 8);
        b->unk_04 =
            b->unk_10 + ((func_800644B8(*(s16 *)(p + 0xC)) >> 4) * *(s16 *)(p + 0x8) << 8);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            func_8003DB94(c, D_800DEC70, 0);
        }
    }
}

/* Spawn the 0x212 ring node: the payload gets a zeroed +0x4, 0x10 at +0x6,
   the owner's D_800DDC40 footprint times 3/4 at BOTH +0x8 and +0xA, and the
   caller's angle at +0xC; the renderable is blitted from D_800DEC70 at
   0x7DCF with 0xC00 scale and a colour built from `d + 1' -- /4 into the red
   channel, (t/2) % 2 into green and t % 2 into blue, each scaled by 128 with
   no +0x40 bias; the mover is the caller's with its velocity seeded from its
   own position and the z radius folded in at 16.16.  func_s29_80024BB0 is
   the step.  136 words, 0 differing.

   ONE PRICED FACT, and it is a position, not a spelling:

     `t = d + 1;' placed AFTER the +0xC payload store          136 w,  0
       ... anywhere before the payload block (four slots tried) 136 w,  9

   All nine rows are a $v0/$v1/$t0 rotation through the two divisions with
   nothing moved -- the textbook "wrong register within a class" residue --
   and the cure is the statement's POSITION, which is the brief's
   "re-check statement order before reaching for a device".  The sweep is
   flat across the four early slots and flat at 0 for the two late ones.

   THE TWO FOOTPRINT STORES ARE ONE CHAINED ASSIGNMENT
   (`*(s16 *)(q + 0x8) = *(s16 *)(q + 0xA) = ...'), which emits +0xA then
   +0x8 -- the inner lvalue first -- and which is also what keeps the
   D_800DDC40 load from being repeated: written as two statements the
   register store to q+0xA invalidates the fixed-address load and the whole
   index is recomputed. */
UnkNode *func_s29_80024D2C(u8 *p, UnkVec *b, s16 d, s16 e)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkVec *k;
    s16 t;
    s16 u;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s29_80024BB0;
        q = (u8 *)n + 0x20;
        *(u8 **)q = *(u8 **)p;
        *(s16 *)(q + 0x4) = 0;
        *(s16 *)(q + 0x6) = 0x10;
        *(s16 *)(q + 0x8) = *(s16 *)(q + 0xA) =
            D_800DDC40[(*(u8 **)(p + 0x18))[0x13]] * 3 / 4;
        *(s16 *)(q + 0xC) = e;
        t = d + 1;
        m = (u8 *)n->unk_0C;
        m[0xC] = t / 4 * 128;
        u = t / 2;
        m[0xD] = u % 2 * 128;
        m[0xE] = t % 2 * 128;
        *(s16 *)(m + 0x12) = 0x7DCF;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC70, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0xC00;
        *(s16 *)(m + 0x1C) = 0xC00;
        func_8004491C(n, (u32)D_80045340);
        k = n->unk_08;
        *k = *b;
        k->unk_0C = k->unk_00;
        k->unk_10 = k->unk_04;
        k->unk_14 = k->unk_08;
        k->unk_14 += *(s16 *)(q + 0xA) << 16;
        return n;
    }
    return NULL;
}

/* The root step of the spark func_s29_80025344 spawns: bump +0x10, +0x12 and
   the +0x16 phase by 0x140, colour the renderable from the +0x12 lane
   (`% 7 + 1' split three ways, each 128-scaled with a +0x40 bias) whenever
   the +0xE state is nonzero, spin the slot by 0x300, then dispatch on +0xE.
   State 0 integrates the mover until +0x10 reaches the +0x14 budget; state 1
   clears the slot's 0x100 bit, ramps its scale down from 0x19 and, after
   0x10 frames, spawns a func_s29_800249C4 shard and a func_s29_800247B0 fan
   both aimed by the target's D_800DDC40 footprint; state 2 fires four
   func_s29_80024D2C rings a quarter-turn apart each frame and retires the
   actor after 0x30.  254 words, 0 differing.

   This function owns NO jump table: three cases is a compare chain, and the
   arms are written ascending 0..2 because such a switch emits them in source
   order.

   THE FACT WORTH THE WHOLE BODY -- `MEM_IN_STRUCT_P' ON THE READ-MODIFY-WRITE
   IN CASE 2's RETIRE BLOCK.  Priced from the 0 state:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | `((s16 *)p)[7] = ((u16 *)p)[7] + 1;'  (the +0xE bump) |  254  |   0  |
   |   ... the same statement as a plain `*(s16 *)(p + 0xE)'|  256  |  16  |
   |   ... array refs on ALL FOUR statements in the block   |  253  |  15  |
   |   ... array ref on `*(u16 *)(p - 0x2) |= 0x8000' only  |  254  |  12  |
   |   ... array ref on `*(s16 *)(p + 0x10) = 0' only       |  255  |  17  |
   | the four statements in any of the 24 ORDERS, with the  |       |      |
   |   array ref in place                                   |  254  |   0  |
   |   ... best order with plain casts throughout           |  255  |  14  |

   So the array ref is the whole fault and the statement ORDER is NOT
   MEASURABLE once it is in place (four orders tried at 0).  The mechanism is
   the levers file's polarity: as a COMPONENT-style ref the +0xE MEM is
   `in_struct', `true_dependence' goes false against the fixed-address
   D_800814A0 beside it, and the `lw %lo(D_800814A0)' reaches the register
   store's delay slot -- the target has no `nop' there and the plain cast
   needs two.  Exactly ONE of the four statements wants it; the brief's
   "read the nops, do not pick a house style" is the right instruction.

   TWO SMALLER PRICED FACTS:

     `*(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = (0x19 - [0x10]) * 256;'
        as ONE CHAIN                                        254 w,  0
       ... two separate statements                          256 w,  4+
     `u = t / 2;' placed AFTER the +0xC colour store         254 w,  0
       ... before it                                        255 w,  1+

   The chain is needed because the store to c+0x1E invalidates the p+0x10
   read, so two statements reload it; a chain computes the value once and
   emits +0x1E first, which is the order the target has. */
void func_s29_80024F4C(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s16 u;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
    *(s16 *)(p + 0x16) = *(u16 *)(p + 0x16) + 0x140;
    if (*(s16 *)(p + 0xE) != 0)
    {
        t = *(s16 *)(p + 0x12) % 7 + 1;
        c[0xC] = t / 4 * 128 + 0x40;
        u = t / 2;
        c[0xD] = u % 2 * 128 + 0x40;
        c[0xE] = t % 2 * 128 + 0x40;
    }
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    switch (*(s16 *)(p + 0xE))
    {
    case 0:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 1:
        *(s16 *)(c + 0x12) = 0;
        *(u16 *)(c + 0x14) &= 0xFEFF;
        *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = (0x19 - *(s16 *)(p + 0x10)) * 256;
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            func_s29_800249C4(p, b, (s16)rand(),
                              -(D_800DDC40[(*(u8 **)(p + 0x18))[0x13]] * 3 / 4));
            func_s29_800247B0(p, b, D_800DDC40[(*(u8 **)(p + 0x18))[0x13]]);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 2:
        func_s29_80024D2C(p, b, (s16)(rand() % 7), *(s16 *)(p + 0x16));
        func_s29_80024D2C(p, b, (s16)(rand() % 7), (s16)(*(u16 *)(p + 0x16) + 0x400));
        func_s29_80024D2C(p, b, (s16)(rand() % 7), (s16)(*(u16 *)(p + 0x16) + 0x800));
        func_s29_80024D2C(p, b, (s16)(rand() % 7), (s16)(*(u16 *)(p + 0x16) + 0xC00));
        if (*(s16 *)(p + 0x10) >= 0x30)
        {
            *(s16 *)(p + 0x10) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            ((s16 *)p)[7] = ((u16 *)p)[7] + 1;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* Spawn the 0x212 spark node that func_s29_800256BC's lock-on arm hands off
   to: the same renderable dress as func_s29_8002558C, but the payload gets a
   zeroed +0xE/+0x10, a `rand() % 7' lane index at +0x12, the caller's +0x12
   budget at +0x14, a `rand() % 0x1000' phase at +0x16 and the caller's
   +0x18 target pointer copied to +0x18.  func_s29_80024F4C is the step.
   103 words, 0 differing, first compile.

   It landed off func_s29_8002558C, which was already in this file -- the
   brief's "land the small siblings in your own unit first".  The whole
   renderable half is that body's verbatim, including the three priced facts
   in its comment, and the only new reading is the payload.

   THE LANE INDEX IS `% 7', NOT `% 9'.  The magic is 0x92492493 and the
   reconstruction is `sll 3' / `subu' -- x*8-x -- so the divisor is 7; read
   the multiply-back, not the constant (0x92492493 is also /9's magic at a
   different shift).  `q' and `m' share $s0 because q dies at the +0x18
   store, which is automatic and needs no device. */
UnkNode *func_s29_80025344(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s29_80024F4C;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0xE) = 0;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x12) = rand() % 7;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        *(s16 *)(q + 0x16) = rand() % 0x1000;
        *(s32 *)(q + 0x18) = *(s32 *)(p + 0x18);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7E01;
        *(u32 *)(m + 0x8) = (u32)D_s29_80025DF8;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)D_80045340);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* The per-frame step of the 0x212 trail node func_s29_8002558C spawns: bump
   the owner node's +0x14 counter and the payload's own +0x10, spin the
   renderable's +0x1A by 0x300, integrate the mover by its velocity, and
   retire once +0x10 passes the +0x14 budget.  A leaf -- no frame.
   43 words, 0 differing, first compile.

   TWIN: the landed func_s28_8002553C in csrc/ovl_dng_s28.c (dngtwin 0.804,
   54 words).  The delta is entirely subtractive: this copy drops the
   func_800478B8(c) call, which is what makes it a leaf and accounts for all
   11 missing words.  Its ONE priced fact HOLDS at the same figure:

     the gate as `*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14)'    43 w,  0  kept
       ... as `*(s16 *)(p + 0x14) < *(s16 *)(p + 0x10)'       43 w,  2

   and the 2 rows are the LOAD ORDER, not the compare: both spellings emit
   `slt $v0, [0x14], [0x10]' and only the kept one loads +0x10 first.  Third
   body in the block to reproduce that figure. */
void func_s29_800254E0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 trail node: a renderable dressed straight from this
   object's own D_s29_80025DF8 (no func_8003DB94 blit), sprite 0x7E01 with
   the +0x14 0xC and 0x100 bits and the +0x10 0x20 bit set, a random starting
   rotation, 0x400 scale on both axes, D_80045340 as the per-slot walk,
   func_s29_800254E0 as the step, the caller's record at the node's +0x20
   with its +0x12 budget copied to the payload's +0x14, and the caller's
   mover copied wholesale.  Returns the node, or NULL when the pool is empty.
   76 words, 0 differing, first compile.

   TWIN: the landed func_s28_80025614 in csrc/ovl_dng_s28.c, 73 words, the
   same spawn one bank over.  Its `q = (u8 *)n + 0x20' payload view is kept
   and is what puts the +0x10/+0x14 stores on $v1; its func_8003DB94 blit is
   replaced here by a single `*(u32 *)(m + 0x8) = (u32)D_s29_80025DF8;',
   which is csrc/ovl_dng_s22.c's idiom for the same field.

   FACTS PRICED ON THIS BODY from the 0 state (aligndiff):

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | `rand() % 0x1000' for the start rotation       KEPT   |   76  |   0  |
   |   ... `rand() & 0xFFF'                                |   70  |  11  |
   | the two `|=' on +0x14 (0xC then 0x100) SEPARATE,      |       |      |
   |   with the +0x10 `|= 0x20' between them        KEPT   |   76  |   0  |
   |   ... merged into one `|= 0x10C'                      |   74  |  10  |
   | `*(s16 *)(m + 0x1E) = 0x400;' BEFORE +0x1C     KEPT   |   76  |   0  |
   |   ... the other order                                 |   76  |   4  |

   The merged-or row is the levers file's "gcc MERGES TWO ADJACENT `|=' ON ONE
   LVALUE": the interposed +0x10 store is load-bearing, and two `ori' in the
   listing means two statements gcc was prevented from merging. */
UnkNode *func_s29_8002558C(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s29_800254E0;
        q = (u8 *)n + 0x20;
        *(u8 **)q = p;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7E01;
        *(u32 *)(m + 0x8) = (u32)D_s29_80025DF8;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)D_80045340);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* This bank's root step for the lock-on flyer: bump the +0x10 frame counter,
   then dispatch on the +0xA state.  State 0 seeds the heading out of the
   actor's +0x2A and whitens the renderable; state 1 re-reads the slot through
   func_8003DE58, publishes the mover's position, and then AIMS -- at the
   locked-on target's record if the actor has one at +0x60, otherwise by
   walking up to eight cells along the heading with func_800A44E0 and
   func_800BCB04 and aiming at the last cell that admitted it.  Either way the
   per-axis velocity is the span over a /16 step count floored at 1, it hands
   off to func_s29_80025344 or func_s29_8002558C, and states 2 and 6
   integrate that velocity until the step count runs out.  States 3, 4 and 5
   are the timed teardown, with state 4 calling this object's own
   func_s29_80024024 and state 5 clearing D_8008346C and retiring the actor.
   463 words, 0 differing, first compile.

   OWNS jtbl_s29_80024008: 7 words, 7 cases, NO pad, dispatch bound
   `sltiu $v0, $v1, 0x7' -- so all 7 entries are real arms and none is a
   default pad.  Entries in order: 0 .Ls29_8002572C, 1 .Ls29_80025758,
   2 .Ls29_80025C8C, 3 .Ls29_80025CE4, 4 .Ls29_80025D00, 5 .Ls29_80025D38,
   6 .Ls29_80025D70.  All 7 distinct, their addresses ascend, so the arms are
   written ascending 0..6; case 0 falls through into case 1.  The range
   check's `beqz $v0, .Ls29_80025DC0' lands on the function's COMMON TAIL
   (the `*(s16 *)(p + 0x14) = 0' after the switch), which is not a table
   entry, so there is no default label to write -- and an explicit
   `default: break;' measures 463 / 0, i.e. FREE, confirmed by compiling it.

   TWIN: the landed func_s28_80025738 in csrc/ovl_dng_s28.c (dngtwin 0.928,
   449 words), the same root step one bank over, which also owns a 7-word
   7-case table.  Byte-identical frame (-0x60, s0..s7/fp/ra, the same 40-byte
   local area and the same `s16 ox; UnkVec vv; u16 out[4]; s16 oy'
   declaration order, which is what puts `oy' in memory at $sp+0x30 and `ox'
   in $fp).  EVERY ONE of its twelve recorded facts carried unchanged, which
   is why this landed first compile: the shared `UnkVec *v', the
   `& 0xFFFFFFC0' mask, `ox * 64' as a multiply, the aim point's `+ 0x20',
   the `p+0x18 = (s32)*(u8 **)(s + 0x60)' statement, case 6's store order,
   the `u16 z' across state 1's flag gate, the single `p+0x10 = 0' after the
   if/else, the hoisted `i = 1', `(D_8006CCD8[dir] + 1) * 32', and the
   three-argument alias on this object's own func_s29_80024024.

   WHAT THIS COPY ADDS, and it is the only delta: the D_800DDC40 HEIGHT
   CORRECTION in the lock-on arm, which s28 drops and csrc/ovl_dng_s25.c has.
   The aim point is the target's own mover at `v + 0xA' less
   `D_800DDC40[(*(u8 **)(p + 0x18))[0x13]]', and the z velocity subtracts
   that footprint's `/ 2 * 3' shifted into 16.16.  The index is read back
   through `*(u8 **)(p + 0x18)' -- the pointer this arm has just stored --
   and NOT through `*(u8 **)(s + 0x60)' again; the listing's `lw 0x18($s0)'
   is the discriminator, and it is read twice because the velocity stores
   between the two uses invalidate it.

   FACTS RE-PRICED ON THIS BODY (aligndiff, from the 0 state):

     case 6's `p+0xA = 5' BEFORE `p+0x10 = 0'               463 w,  0  kept
       ... the other order                                  468 w, 20
     an explicit `default: break;'                          463 w,  0  FREE
     the height bias as `h / 2 * 3 << 16'                   463 w,  0  kept
       ... as `(h >> 1) * 0x30000'                          463 w,  0  FREE

   The case-6 figure reproduces the twin's 20 rows to the digit on a body
   five words longer, which is the fourth time that fact has been priced in
   this family.  The two height-bias spellings are the same tree, so that row
   is FREE and not evidence about either. */
void func_s29_800256BC(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q4;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
    UnkVec vv;
    u16 out[4];
    s16 oy;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        w = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        z = *(u16 *)(w + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            *(s32 *)(p + 0x18) = (s32) * (u8 **)(s + 0x60);
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) -
                         D_800DDC40[(*(u8 **)(p + 0x18))[0x13]] - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - (D_800DDC40[(*(u8 **)(p + 0x18))[0x13]] / 2 * 3 << 16) -
                         b->unk_08) /
                        *(s16 *)(p + 0x12);
            func_s29_80025344(p, b);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        else
        {
            q4 = *(u8 **)(o + 0xC);
            xx = q4[0x24];
            yy = q4[0x25];
            ox = xx;
            oy = yy;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88),
                                       (s16)(*(s16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                oy = yy;
                ox = xx;
            }
            v = &vv;
            i = 1;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            func_s29_8002558C(p, b);
            *(s16 *)(p + 0xA) = 6;
        }
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x30)
        {
            func_s29_80024024_3(*(u8 **)(s + 0x60), p[0x9], s);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 6:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 5;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
