#include "common.h"

s32 rand(void);

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
typedef struct UnkDngS01Dirs
{
    u8 unk_00[0x20];
} UnkDngS01Dirs;
extern u8 D_s01_80024004[];
extern u8 D_800DEC00[];
extern u8 D_80045340[];
extern u8 D_800DE870[];
extern u8 D_800DE938[];
extern s32 D_8008346C;
void func_8003DB94(void *, void *, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
s32 func_800A56E0(s32);
s32 func_800A4778(u16, u16, s16, s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);

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

/* The summon bolt's own step, the one func_s01_80024CFC below installs on each
   0x12 node it spawns.  State 0 sheds one func_s01_800245BC spark a frame
   (unless the payload's +0xA carries bit 0), spawns a 0x212 flare with
   D_800DE938 at 0x2000/0x1000 and hands it to func_800478B8 twice, and spends
   one of the +0x2 frames; when they run out it either just advances (+0x8
   clear) or BURSTS -- four more sparks, two 0x212 motes on D_800DE870 with a
   random 0xFFF80000-based velocity triple, two 0x212 D_800DE938 flares whose
   scale halves swap on the second and whose +0xA lifts 0x14 on the first --
   then, if the +0xA mode is still 4, rings func_8009CE1C on the caster's
   target, advances to state 1, raises 0x10000000 in that target's +0x1C and
   dresses its slot 0x30.  Every frame the mover integrates the +0x58..+0x60
   velocity unless func_800A4778 says the destination tile is blocked.
   State 1 counts +0x2 up to 0x15, then clears the 0x10000000 bit, restores
   the 0x80 dress and kills the node.  392 words, 0 differing.  1 member,
   392 words (dngfam.py).  OWNS NO JUMP TABLE: jtcheck reports 0 tables, both
   dispatches here are if-chains and ovl_dng_s01 has no .rodata subsegment at
   all.

   NO TWIN, EITHER TOOL -- no carved sibling at ratio 0.60 and no resident
   one.  dngcallee's sharp row (7 of 11 symbols, one function) is
   csrc/ovl_dng_s46.c's func_s46_80024C0C, a 74-word step: it confirmed the
   family's spawn idiom (func_8003FC64(0x212) / +0x10 install /
   func_8004491C(n, D_80045340) / slot->unk_14 |= 0xC) and nothing else,
   because this body is five times its size.  It is written from the listing,
   and the three in-unit landings above it are what paid for it:
   func_s01_800245BC's seven-argument prototype, the `+= 1'/`-= 1' halfword
   idiom and the D_800814A0 kill pair all came from func_s01_80024280 and
   func_s01_80024448 rather than from any donor.

   FOUR FACTS, each priced BOTH WAYS from this body at 0 differing (aligndiff):

   * THE FIVE JITTER STATEMENTS NEED `(s16)' AROUND THE SUBTRACTION, NOT
     AROUND THE LOAD AND NOT AT ALL.  `*(s16 *)(m + 0x2) =
     (s16)(*(u16 *)((u8 *)v + 0x2) - 0x10) + (rand() & 0x1F);' is 392/0.
     Written plainly it is 397 words / 33 differences, and the mechanism is
     convert.c's narrowing, not allocation: the store is 16 bits, so
     convert_to_integer narrows the arithmetic, and it takes its UNSIGNEDNESS
     FROM THE FIRST OPERAND -- a `u16' load -- so the narrowed type is
     `unsigned short' and -0x10 becomes 0xFFF0.  0xFFF0 is outside `addiu's
     signed range, so cc1psx materialises it with an `ori' into a register and
     uses `addu' where the target has `addiu $v1, $v1, -0x10'; cse then shares
     that register across the sites, which is the five extra words.  The cast
     makes the narrowed type SIGNED short, and the `lhu' survives because the
     load is still a `u16' MEM.  With the cast on the LOAD instead --
     `(s16)*(u16 *)(...) - 0x10' -- the type is right but fold reassociates to
     `(jitter - 0x10) + X' and it is 390 words / 25, with every `addiu' on the
     wrong operand.  Same NOP_EXPR barrier func_s01_800245BC above documents,
     used here for the narrowing rather than for the associate step.
   * THE NEW NODE'S PAYLOAD POINTER AND ITS MOVER ARE ONE LOCAL.  `m' is
     `(u8 *)n + 0x20' for the +0x2/+0x58/+0x5C/+0x60 stores and is then
     reassigned `(u8 *)n->unk_08' for the three mover stores.  As TWO locals
     it is 392 words / 49 differences, and all 49 are one clean $s0/$s1
     exchange with nothing moved: the slot local `s' wants $s0 and the
     payload/mover local wants $s1, and splitting the pointer in two lifts
     that pair's allocno priority past `s'.  DECLARATION ORDER IS FREE --
     eight permutations of the locals all measure 392/49 split and 392/0
     merged -- so this is a reference count, not a tie-break.
   * STATE 1's TWO `*(u8 **)(p + 0x30)' READS MUST BE WRITTEN INLINE, NOT
     THROUGH A LOCAL.  Inline it is 392/0; hoisted into `q' as the state-0
     site does it is 389 words / 18, the three missing words being the second
     `lw 0x30($s5)' the target reloads because the `&= 0xEFFFFFFF' store
     between them invalidates that MEM.  The state-0 site has no store before
     its own `-0x14' read, which is why `q' is right THERE and wrong here.
   * THE LOOP-2 SLOT SCALES ARE WRITTEN +0x1E BEFORE +0x1C.  The other way
     round is 392 words / 2 differences, the two `sh's exchanged.  Both halves
     are 0x1000 so only the order distinguishes them; the loop-3 and state-0
     pairs carry different values and are not a choice.

   MEASURED AND FREE, so NONE of these is why this lands (all 392 / 0):
   * The three kill tails written out as `|= 0x8000' pairs plus `return',
     against ONE `goto kill' label: 392/0 either way, so the target does not
     constrain it.  Written out because jump2 cross-joins the copies itself.
   * `((rand() & 0x7FFF) << 5) - 0x80000' against `+ 0xFFF80000'.  Unlike
     func_s01_80024CFC's climb below there is no division here, so the
     unsigned constant costs nothing; the signed form is kept for honesty.
   * `(s16)' ADDED to the two subtractions that have NO jitter term
     (`*(u16 *)(g + 0xA) - 0x20' and `*(u16 *)(m + 0xA) - 0x14').  With a
     single operand both spellings give `addiu', so the cast is left off.
   * `*(u16 *)(p + 0x2) -= 1;' split from its gate, against
     `if ((s16)(*(u16 *)(p + 0x2) -= 1) <= 0)'.  Two statements kept, as in
     func_s01_80024280 above.

   The third parameter is never read; it is declared to match the step
   signature func_8004491C installs and the other two steps in this unit. */
void func_s01_800246DC(u8 *p, UnkVec *v, u8 *c)
{
    UnkNode *n;
    u8 *m;
    UnkSlot *s;
    u8 *g;
    u8 *q;
    UnkSlot *e;
    s32 i;

    if (*(s16 *)(p + 0x0) == 0)
    {
        if ((*(u16 *)(p + 0xA) & 0x1) == 0)
        {
            func_s01_800245BC((UnkNode *)(p - 0x20), rand() & 0x7, 0xF0, (rand() & 0xFF) | 0x80, 0, 0,
                              -0x20 - (rand() & 0x1F));
        }
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            *(s16 *)((u8 *)n + 0x22) = 0x9;
            n->unk_10 = (u32)func_s01_80024280;
            func_8004491C(n, (u32)D_80045340);
            s = (UnkSlot *)n->unk_0C;
            s->unk_14 |= 0xC;
            m = (u8 *)n->unk_08;
            *(s16 *)(m + 0x2) = (s16)(*(u16 *)((u8 *)v + 0x2) - 0x10) + (rand() & 0x1F);
            *(s16 *)(m + 0x6) = (s16)(*(u16 *)((u8 *)v + 0x6) - 0x10) + (rand() & 0x1F);
            *(s16 *)(m + 0xA) = (s16)(*(u16 *)((u8 *)v + 0xA) - 0x4) + (rand() & 0x7);
            s = (UnkSlot *)n->unk_0C;
            s->unk_1C = 0x2000;
            s->unk_1E = 0x1000;
            s->unk_0C[2] = 0x80;
            s->unk_0C[1] = 0x80;
            s->unk_0C[0] = 0x80;
            func_8003DB94(s, D_800DE938, 0);
            func_800478B8((u8 *)s);
            func_800478B8((u8 *)s);
        }
        *(u16 *)(p + 0x2) -= 1;
        if (*(s16 *)(p + 0x2) <= 0)
        {
            if (*(s16 *)(p + 0x8) != 0)
            {
                for (i = 0; i < 4; i++)
                {
                    func_s01_800245BC((UnkNode *)(p - 0x20), rand() & 0x7, 0xF0,
                                      (rand() & 0xFF) | 0x80, 0, 0, -0x20 - (rand() & 0x1F));
                }
                for (i = 0; i < 2; i++)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        m = (u8 *)n + 0x20;
                        *(s16 *)(m + 0x2) = 0xD;
                        g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
                        n->unk_10 = (u32)func_s01_80024280;
                        func_8004491C(n, (u32)D_80045340);
                        s = (UnkSlot *)n->unk_0C;
                        s->unk_14 |= 0xC;
                        *(s32 *)(m + 0x58) = ((rand() & 0x7FFF) << 5) - 0x80000;
                        *(s32 *)(m + 0x5C) = ((rand() & 0x7FFF) << 5) - 0x80000;
                        *(s32 *)(m + 0x60) = ((rand() & 0x3FFF) << 5) - 0x80000;
                        m = (u8 *)n->unk_08;
                        *(s16 *)(m + 0x2) = (s16)(*(u16 *)(g + 0x2) - 0x8) + (rand() & 0xF);
                        *(s16 *)(m + 0x6) = (s16)(*(u16 *)(g + 0x6) - 0x1) + (rand() & 0x1);
                        *(s16 *)(m + 0xA) = *(u16 *)(g + 0xA) - 0x20;
                        s = (UnkSlot *)n->unk_0C;
                        s->unk_1E = 0x1000;
                        s->unk_1C = 0x1000;
                        s->unk_0C[2] = 0x80;
                        s->unk_0C[1] = 0x80;
                        s->unk_0C[0] = 0x80;
                        func_8003DB94(s, D_800DE870, 0);
                    }
                }
                for (i = 0; i < 2; i++)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        *(s16 *)((u8 *)n + 0x22) = 0xB;
                        g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
                        n->unk_10 = (u32)func_s01_80024280;
                        func_8004491C(n, (u32)D_80045340);
                        s = (UnkSlot *)n->unk_0C;
                        s->unk_14 |= 0xC;
                        m = (u8 *)n->unk_08;
                        *(s16 *)(m + 0x2) = (s16)(*(u16 *)(g + 0x2) - 0x8) + (rand() & 0xF);
                        *(s16 *)(m + 0x6) = (s16)(*(u16 *)(g + 0x6) - 0x1) + (rand() & 0x1);
                        *(s16 *)(m + 0xA) = *(u16 *)(g + 0xA);
                        s = (UnkSlot *)n->unk_0C;
                        s->unk_0C[2] = 0x80;
                        s->unk_0C[1] = 0x80;
                        s->unk_0C[0] = 0x80;
                        s->unk_06 = 0x7;
                        if (i == 0)
                        {
                            s->unk_1C = 0x1000;
                            s->unk_1E = 0x2000;
                            *(s16 *)(m + 0xA) = *(u16 *)(m + 0xA) - 0x14;
                        }
                        else
                        {
                            s->unk_1C = 0x2000;
                            s->unk_1E = 0x1000;
                        }
                        func_8003DB94(s, D_800DE938, 0);
                    }
                }
                if (*(s16 *)(p + 0xA) != 0x4)
                {
                    *(u16 *)(p - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                    return;
                }
                func_8009CE1C(*(u8 **)(p + 0x30), 0x10, p[0x15], 1, (s16)(*(u16 *)(p + 0x16) << 9),
                              *(u32 *)(p + 0x2C), 2);
                if (*(s16 *)(p + 0xA) != 0x4)
                {
                    *(u16 *)(p - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                    return;
                }
                q = *(u8 **)(p + 0x30);
                *(s16 *)(p + 0x0) = 1;
                *(s32 *)(q + 0x1C) |= 0x10000000;
                e = *(UnkSlot **)(q - 0x14);
                e->unk_0C[2] = 0x30;
                e->unk_0C[1] = 0x30;
                e->unk_0C[0] = 0x30;
                *(s16 *)(p + 0x2) = 0;
            }
            else
            {
                *(s16 *)(p + 0x0) = 1;
            }
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(p + 0x30)) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        v->unk_00 += *(s32 *)(p + 0x58);
        v->unk_04 += *(s32 *)(p + 0x5C);
        v->unk_08 += *(s32 *)(p + 0x60);
    }
    if (*(s16 *)(p + 0x0) == 1)
    {
        *(u16 *)(p + 0x2) += 1;
        if (*(s16 *)(p + 0x2) >= 0x15)
        {
            if (*(s16 *)(p + 0xA) == 0x4 && *(s16 *)(p + 0x8) == 1)
            {
                *(s32 *)(*(u8 **)(p + 0x30) + 0x1C) &= 0xEFFFFFFF;
                e = *(UnkSlot **)(*(u8 **)(p + 0x30) - 0x14);
                e->unk_0C[2] = 0x80;
                e->unk_0C[1] = 0x80;
                e->unk_0C[0] = 0x80;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Landed OUT OF docs/dng-parks/func_s01_80024CFC.c, which stood at 12 aligned
   rows.  The park's shape, its word count, its declaration block and its
   account of what the function does were all correct and are kept; its
   residue was closed by two things the park did not have.

   FIRST, THE LANDED func_s21_80024050 (csrc/ovl_dng_s21.c), which itself
   landed out of its OWN 61-row park after this one was written.  The park's
   header names it as the family's master step handler and reads it for the
   diagnosis; what it could not read is its TABLE, whose third row gives the
   family's tile distance as ONE `__builtin_abs' OF A TERNARY.  That one
   change is worth 11 of the 12 rows: the park's `if/else' into an `s32 t'
   re-measures at 399 words / 16 differences from this body.  It is a
   SELECTION fact and it reproduced across the relink, as the brief predicts
   for that class.

   SECOND, the park's own stated open question -- "the spelling that makes the
   +0x2 tile distance RELOAD off `w' rather than be forwarded from its own
   store".  It is `volatile' ON THE STORE.  Non-volatile it is 398 words / 36
   differences: cse records the stored value, substitutes it for the `lh' and
   sign-extends it with `sll 16'/`sra 16', which keeps the abs pseudo live
   long enough to take $a1 and pushes `w' out to $a2 and `n' to $a3 for 30 of
   those rows.  cse never records a volatile store, so `lh $a0, 0x2($a1)'
   survives at the right base.  The park's workaround -- reading the same
   address as `*(s16 *)((u8 *)n + 0x22)' -- also defeats cse and, with the
   ternary in place, reaches 399 words and exactly ONE difference, that row
   being the base register.  The rule behind both, measured across seven
   pairs: EVERY SAME-BASE PAIR FORWARDS (plain store with an array-ref read,
   array-ref store with a plain read, `u16' store with an `s16' read --
   398/36 for all three) and every cross-base pair leaves one row on
   whichever side names `n'.  I call the `volatile' A DEVICE, not macro
   residue: nothing about this field is volatile, and no plain-C spelling I
   measured blocks the forwarding with both ends off `w'.

   RE-PRICED FROM THIS BODY AT 0, because every figure in a park was measured
   against a body with a live fault.  Four of the park's load-bearing facts
   HOLD, three of them at a fraction of their recorded cost:
   | spelling                                          | park | here |
   |---------------------------------------------------|------|------|
   | `*(u16 *)(p + 0x84) -= 1' as a plain cast         |  15  |   3  |
   | the +0x7E facing computed inline after the tick   |  17  |   4  |
   | fresh locals for the p+0x90 / p+0x94 follow pair  |  20  |   6  |
   | the climb as `+ 0xFFE00000' (unsigned -> `divu')  |   1  |   1  |
   The `do { } while (0)' around the +0x2 store, which the park priced at 5
   rows and called macro residue, is NOT in this body: added back it is 399
   words / 8 differences, so with the ternary and the volatile store it
   inverts from a 5-row gain to an 8-row cost.  That is the brief's split
   exactly -- the four SELECTION facts carried and the one REFERENCE-COUNT
   fact inverted.

   WHAT IT IS, as the park has it: state 0..3 of the summon beam.  `p' is this
   node payload, `v' its mover, `s' its renderable; `r = *(u8 **)p' is the
   CASTER payload, `g = (UnkNode *)(r - 0x20)' the caster node and
   `m = g->unk_08' the caster mover.  0 dresses white and binds D_800DEC00,
   1 publishes the caster hand through func_8003DF74, 2 spawns the 0x12 bolts
   that run func_s01_800246DC above, 3 follows the last one and kills.

   399 words, 0 differing.  1 member, 399 words (dngfam.py).  OWNS NO JUMP
   TABLE (jtcheck: 0 tables); the switch on *(s16 *)(p + 0xA) is a compare
   chain and ovl_dng_s01 has only a `data' subsegment.  D_s01_80024004 is
   emitted by splat as asm/data/1834800.data.s -- declared extern here and
   never defined in C.  Relocations hand-checked and clean under relcand.py. */
void func_s01_80024CFC(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 sp10[3];
    UnkDngS01Dirs dd;
    u8 *d;
    u8 *r;
    UnkNode *g;
    UnkVec *m;
    UnkNode *n;
    u8 *w;
    u8 *u;
    u8 *h;
    u8 *hh;
    s32 j;
    UnkSlot *k;

    r = *(u8 **)(p + 0x0);
    d = dd.unk_00;
    dd = *(UnkDngS01Dirs *)D_s01_80024004;
    g = (UnkNode *)(r - 0x20);
    m = g->unk_08;
    *(u16 *)(p + 0x82) += 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)s->unk_0C = 0x808080;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        func_8003DB94(s, D_800DEC00, 0);
        j = (*(u16 *)(r + 0x2A) >> 9) & 7;
        D_s01_80025338 = 1;
        *(s16 *)(p + 0x7E) = j;
        *(u16 *)(p + 0xA) += 1;
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)g->unk_0C)->unk_08, (UnkSlot *)g->unk_0C, sp10, 0) != 0 ||
            (((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) != 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)m + 0xA);
            if ((((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)v + 0x2) += sp10[0];
                *(s16 *)((u8 *)v + 0x6) += sp10[1];
                *(s16 *)((u8 *)v + 0xA) += sp10[2];
            }
            else
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) - 0x40;
            }
            *(s32 *)(p + 0xB0) = v->unk_00;
            *(s32 *)(p + 0xB4) = v->unk_04;
            *(s32 *)(p + 0xB8) = v->unk_08;
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                *(s16 *)(p + 0x84) = 0;
                *(u16 *)(p + 0xA) += 1;
                func_800A56E0(0x300);
            }
        }
        break;
    case 2:
        *(u16 *)(p + 0x84) += 1;
        if ((*(u16 *)(p + 0x84) & 1) != 0)
        {
            n = func_8003FC64(0x12);
            if (n != NULL)
            {
                w = (u8 *)n + 0x20;
                *(s32 *)(w + 0x58) = *(s16 *)(d + (*(s16 *)(p + 0x7E) << 2)) << 16;
                *(s32 *)(w + 0x5C) = *(u16 *)(d + (*(s16 *)(p + 0x7E) << 2) + 0x2) << 16;
                *(u8 **)(w + 0x2C) = r;
                *(s32 *)(w + 0x30) = *(s32 *)(r + 0x60);
                *(s16 *)(w + 0xA) = (*(s16 *)(p + 0x84) - 1) / 2;
                w[0x15] = p[0x9];
                *(s16 *)(w + 0x16) = *(u16 *)(p + 0x7E);
                u = *(u8 **)(r - 0x14);
                p[0xA8] = u[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
                p[0xA9] = u[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
                if (*(s32 *)(r + 0x60) != 0)
                {
                    *(s16 *)(w + 0x8) = 1;
                    *(volatile s16 *)(w + 0x2) =
                        __builtin_abs(*(s8 *)(r + 0x72) == u[0x24] ? *(s8 *)(r + 0x73) - u[0x25]
                                                                   : *(s8 *)(r + 0x72) - u[0x24]) *
                        2;
                    hh = *(u8 **)(*(s32 *)(r + 0x60) - 0x18);
                    if (*(s16 *)(p + 0x84) == 1)
                    {
                        if (*(s16 *)(w + 0x2) != 0)
                        {
                            v->unk_14 = *(s32 *)(w + 0x60) =
                                (*(s32 *)(hh + 0x8) - v->unk_08 - 0x200000) / (*(s16 *)(w + 0x2) - 1);
                        }
                    }
                    else
                    {
                        *(s32 *)(w + 0x60) = v->unk_14;
                    }
                }
                else
                {
                    *(s16 *)(w + 0x2) = 8;
                    *(s16 *)(w + 0x8) = 0;
                }
                if (*(s16 *)(p + 0x84) == 9)
                {
                    if (*(s32 *)(r + 0x60) != 0)
                    {
                        *(s16 *)(p + 0x86) = *(u16 *)(w + 0x2) + 0x28;
                    }
                    else
                    {
                        *(s16 *)(p + 0x86) = 0x1E;
                    }
                }
                n->unk_10 = (u32)func_s01_800246DC;
                h = (u8 *)n->unk_08;
                *(s32 *)(h + 0x0) = *(s32 *)(p + 0xB0);
                *(s32 *)(h + 0x4) = *(s32 *)(p + 0xB4);
                *(s32 *)(h + 0x8) = *(s32 *)(p + 0xB8);
                k = (UnkSlot *)n->unk_0C;
                k->unk_0C[2] = 0x80;
                k->unk_0C[1] = 0x80;
                k->unk_0C[0] = 0x80;
                k->unk_1E = 0x1000;
                k->unk_1C = 0x1000;
                if (*(s16 *)(p + 0x84) == 1)
                {
                    *(u32 *)(p + 0x90) = (u32)n;
                    *(u32 *)(p + 0x94) = (u32)h;
                    *(s16 *)(p + 0x88) = 0x63;
                }
            }
        }
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0x90);
            h = *(u8 **)(p + 0x94);
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = *(s32 *)(h + 0x0);
                v->unk_04 = *(s32 *)(h + 0x4);
                v->unk_08 = *(s32 *)(h + 0x8);
            }
        }
        if (*(s16 *)(p + 0x84) >= 0xB)
        {
            *(s16 *)(p + 0x84) = 0;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0x90);
            h = *(u8 **)(p + 0x94);
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = *(s32 *)(h + 0x0);
                v->unk_04 = *(s32 *)(h + 0x4);
                v->unk_08 = *(s32 *)(h + 0x8);
            }
        }
        ((u16 *)p)[0x42] += 1;
        if (*(s16 *)(p + 0x84) > *(s16 *)(p + 0x86))
        {
            ((u16 *)p)[0x42] -= 1;
            if (D_s01_80025338 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_s01_80025338 = 0;
            }
        }
        break;
    }
}
