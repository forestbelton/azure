#include "common.h"

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
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
void func_s01_80024448();
s32 func_s01_80024024(u8 *p, UnkVec *v);

UnkNode *func_8003FD64(s32, void *);

extern s16 D_s01_80025338;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
s32 func_800BCB04(s32, s32, s16);
void func_800478B8(u8 *);

/* One semi-transparent 1x1 TILE_1 and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with each node's colour word
   scaled by the payload's +0x32 life over 256.  151 words, 0 differing.

   TWIN: the landed func_s58_80024124 (csrc/ovl_dng_s58.c, 148 words, 0.977).
   Two deltas, both off this listing: the bank tick `D_s01_80025338 = 1' that
   s58's copy does not have (the three extra words) and the fade is `/ 256',
   not s58's `/ 128' -- spelled `/ 128' it is 151 words and 6 aligned
   differences, three `addiu 0x7F'/`sra 7' pairs.  Everything else in the
   twin, including its own note's account of the shape (addPrim as a bitfield
   store, the two loop-hoisted masks in $s2/$s4, setTile1 plus setSemiTrans
   folding to one `sb 0x6A', and the UnkDng73E94SP offsets) carried over
   unchanged.  s58's copy landed on its first compile, so those were
   assertions rather than measurements; the two that are load-bearing HERE
   are measured:

   * THE TICK STORE IS THE FIRST STATEMENT.  After the two scratchpad
     initialisers it is 151 words / 12 aligned differences -- the `lui`/`sh`
     pair moves four words down the prologue and takes the D_80083160 load
     with it.
   * THE TAIL DRESS GOES THROUGH THE `g' POINTER LOCAL while the head uses
     the symbol inline, which is s58's split and which HOLDS: with
     `D_80083160.unk_00' at the tail too it is 149 words / 27 differences.
     The two missing words are $s5's save and restore. */
s32 func_s01_80024024(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    D_s01_80025338 = 1;
    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)&sp->unk_00, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setRGB0(t, t->r0 * *(s16 *)(p + 0x32) / 256, t->g0 * *(s16 *)(p + 0x32) / 256,
                    t->b0 * *(s16 *)(p + 0x32) / 256);
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* The simple actor step: spend one of the payload's +0x2 frames, integrate
   the three +0x58..+0x60 velocities by their +0x64..+0x6C accelerations and
   add them into the mover, raise the bank tick, then dispatch on the +0x18
   mode -- 0 refreshes once, 1 refreshes +0x1A times, 2 refreshes every +0x1A
   frames off the +0x1C counter -- and raise the redraw bit when the frame
   count runs out or when the view's +0x14 carries 0x8000.  114 words,
   0 differing, FIRST COMPILE.  2 members, 228 words (dngfam.py).

   NO TWIN by either tool: dngtwin's only row is func_s02_8002443C at 1.000,
   which is this function's exact clone and STILL AN INCLUDE_ASM, so there was
   nothing to read.  It is written from the listing, and it compiled first
   time because func_s01_80024024 and func_s01_80024448 were already in this
   file: the bank tick symbol, the D_800814A0 kill pair and their spellings
   all came from them, and the only new declaration is func_800478B8.

   FOUR SPELLINGS MEASURED HERE, since a first compile prices nothing:

   LOAD-BEARING:
   * THE TWO TAIL GATES ARE SEPARATE `if's WITH THE BLOCK WRITTEN OUT TWICE.
     As one `if (frames <= 0 || (view & 0x8000))' it is 106 words / 13
     aligned differences -- the eight missing words are the second copy.
     Note this is the OPPOSITE of func_s01_80024448 forty words above, which
     wants exactly that gate shared; the discriminator is the target, not a
     house style.
   * THE TICK STORE IS THE EIGHTH STATEMENT, after the three mover adds.
     First in the function it is 115 words / 11; before the three mover adds
     it is 115 / 13.  (And again the opposite of func_s01_80024024, where
     first is the only position that measures 0.)
   * THE SWITCH ARMS ARE WRITTEN ASCENDING 0, 1, 2.  Descending is 111 words
     / 24: this is a COMPARE-CHAIN switch, not a table, so the arms emit in
     source order and the order is priced.
   * THE `case 1' LOOP RE-READS ITS BOUND.  `n = *(s16 *)(p + 0x1A);' in
     front of the loop is 115 words / 44 differences -- loop.c hoists the
     load, the `lh' leaves the body and the whole loop is rebuilt.

   MEASURED AND FREE, so NOT why this lands: the `case 2' counter bump folded
   into its own gate, `if ((s16)(*(u16 *)(p + 0x1C) += 1) >= ...)', is also
   114 words / 0 differing.  Two statements is kept because it reads better. */
void func_s01_80024280(u8 *p, UnkVec *v, u8 *c)
{
    s32 i;

    *(u16 *)(p + 0x2) -= 1;
    *(s32 *)(p + 0x58) += *(s32 *)(p + 0x64);
    *(s32 *)(p + 0x5C) += *(s32 *)(p + 0x68);
    *(s32 *)(p + 0x60) += *(s32 *)(p + 0x6C);
    v->unk_00 += *(s32 *)(p + 0x58);
    v->unk_04 += *(s32 *)(p + 0x5C);
    v->unk_08 += *(s32 *)(p + 0x60);
    D_s01_80025338 = 1;
    switch (*(s16 *)(p + 0x18))
    {
    case 0:
        func_800478B8(c);
        break;
    case 1:
        for (i = 0; i < *(s16 *)(p + 0x1A); i++)
        {
            func_800478B8(c);
        }
        break;
    case 2:
        *(u16 *)(p + 0x1C) += 1;
        if (*(s16 *)(p + 0x1C) >= *(s16 *)(p + 0x1A))
        {
            func_800478B8(c);
            *(s16 *)(p + 0x1C) = 0;
        }
        break;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node's drift step, gated on height: when the caster-relative floor
   probe at (+0x2, +0x6, +0xA + 2) comes back above the node's own +0xA, bleed
   the mover along the D_8006CCD8/D_8006CCE8 direction pair at the payload's
   +0x14 scaled by its +0x32 countdown and shifted 9, with a fresh
   rand() & 0xFFFF jitter per axis and a flat 0xFFFE0000 climb less
   rand() & 0xFFF on the third; then raise the bank tick, drop +0x32 by 8 and,
   at zero OR when the view's +0x14 carries 0x8000, raise the redraw bit in
   the node's -0x2 and in D_800814A0.  93 words, 0 differing.

   TWIN: the landed func_b06_801740E4 (csrc/ovl_dng_b06.c, 76 words, 0.864).
   Deltas off this listing: the tick store, the whole func_800BCB04 gate, the
   shift is 9 not 7 and the jitter mask 0xFFFF not 0x7FFF.  Of b06's four
   stated facts:
     - HOLDS: the rand() term comes SECOND in each sum.  Written first on the
       first axis it is 93 words / 2 aligned differences, both `addu`s with
       their operands exchanged -- b06 priced 4 for both axes, so this
       reproduces its figure per axis.
     - HOLDS, to the digit: the third axis is `+= 0xFFE00000 - (rand() &
       0xFFF)'.  As `= v->unk_08 - 0x200000 - (rand() & 0xFFF)' it is 5
       aligned differences, which is b06's own 5.
     - FREE, as b06 also found: the countdown split into
       `*(u16 *)(p + 0x32) -= 8;' plus a separate test is 93 words /
       0 differing.
     - NOT APPLICABLE: b06's note on the two table reads as integer sums
       rather than subscripts -- free there, and this body uses subscripts.

   WHAT THIS BODY NEEDED THAT b06 DID NOT, measured here: THE THREE PROBE
   ARGUMENTS GO THROUGH LOCALS AND `z' IS WRITTEN FIRST.  With the three
   loads inline in the call it is 93 words / 4 differences (the `li 1'/`sh'
   of the tick store sit four words early, ahead of the three `lhu`s); with
   `x, y, z' assigned in that order it is 2; assigned `z, x, y' -- the order
   the target emits them -- it is 0.  The `s16' width on `z' is FREE: `s32 z'
   is also 0, because the `(s16)' truncation comes from func_800BCB04's third
   parameter either way. */
void func_s01_80024448(u8 *p, UnkVec *v, u8 *c)
{
    s16 z;
    s32 x;
    s32 y;

    z = *(u16 *)((u8 *)v + 0xA) + 2;
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_s01_80025338 = 1;
    if (*(s16 *)((u8 *)v + 0xA) < (s16)func_800BCB04(x, y, z))
    {
        v->unk_00 += ((D_8006CCD8[*(s16 *)(p + 0x14)] * *(s16 *)(p + 0x32)) << 9) + (rand() & 0xFFFF);
        v->unk_04 += ((D_8006CCE8[*(s16 *)(p + 0x14)] * *(s16 *)(p + 0x32)) << 9) + (rand() & 0xFFFF);
        v->unk_08 += 0xFFFE0000 - (rand() & 0xFFF);
    }
    if ((s16)(*(u16 *)(p + 0x32) -= 8) <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s01_80024448 as the step and
   func_s01_80024024 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own. */
void func_s01_800245BC(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s01_80024448;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s01_80024024);
        *(u32 *)(p + 0x8) = c;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s01", func_s01_800246DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s01", func_s01_80024CFC);
