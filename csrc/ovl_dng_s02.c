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
void func_s02_80024604();

UnkNode *func_8003FD64(s32, void *);

extern s16 D_80025914;
void func_800478B8(u8 *);

typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk64
{
    u8 b[0x40];
} UnkDngS58Blk64;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;

/* The same spawn stepped one frame, re-sending the renderable instead of
   spinning it: spend one of the payload's +0x2A frames, raise the D_80025914
   tick, push the slot back through func_800478B8, and close the node when
   the counter runs out.  29 words, 0 differing.  The countdown-plus-call
   idiom is func_b01_801708E8's (csrc/ovl_dng_b01.c) and the teardown is this
   file's own landed func_s51_800249A0.

   THE COUNTER READ-MODIFY-WRITE MUST BE SPLIT AROUND THE TICK STORE, which
   is func_s51_800249A0's own fact at its own sign.  Folded into
   `*(s16 *)(p + 0x2a) -= 1' it is 29 words / 7 aligned differences: the
   target emits read, foreign store, write-back in that order, and that order
   cannot come from one compound assignment.  The tick store must sit BETWEEN
   the read and the write-back -- moved after the write-back it is also
   29 / 7, the same residue.

   func_800478B8(c) GOES BEFORE THE GATE.  Moved after it the body is 26 words
   / 19 aligned differences, which is b01's own stated fact in the same
   direction.

   MEASURED AND FREE, so NOT the reason this lands: the gate's load as the
   array ref `((s16 *)p)[0x15]' (29 / 0).  That spelling is worth 11 rows on
   func_s51_800248B8 40 words above, where the tick store has to sink past
   the gate; here the call already separates them, so the MEM_IN_STRUCT_P
   escape has nothing to buy. 
   THIS COPY'S OWN CONSTANT: 0x2a -> 0x2 at all 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
s32 func_800BCB04(s32, s32, s16);
extern u8 D_80045340[];
extern u8 D_800DE870[];
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
extern u8 D_80025908[];

void func_s02_8002404C(u8 *p, u8 *b, u8 *c)
{
    s16 t;

    t = *(s16 *)(p + 0x2) - 1;
    D_80025914 = 1;
    *(s16 *)(p + 0x2) = t;
    func_800478B8(c);
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* This bank's spawner for the 0x212 decoration node: one frame at the
   payload's +0x2 and +0x4, func_s02_8002404C as the node's step, the
   renderable set to 0x60 with 0 at +0x06, 0xC raised in its flags, then the
   mover SEEDED FROM THE THREE RAW s16 PARAMETERS and immediately incremented
   by the caller's vector, and finally 0x400 as both scale halves, 0x80 over
   r+0xC..0xE, 0x7DCF at +0x12, 0x100 raised in the flags again and the record
   registered with func_8003DB94 against D_800DE870 -- third argument 2, not
   the 0 every other copy of this skeleton passes.  75 words, 0 differing,
   FIRST COMPILE.  1 member, 75 words (dngfam.py).

   TWIN: the landed func_s46_80024D34 (csrc/ovl_dng_s46.c, 74 words,
   dngtwin 0.752).  I took it over the 0.816 func_s59_80024DD4 on the word
   delta -- s46 is one word away, s59 is +24 -- and the choice was right: the
   body landed first compile.  Both of s46's facts are inapplicable or absent
   here rather than priced: it has no jitter (so its fold/associate fact about
   `load + ((rand() & 0x1F) - 0x10)' has nothing to apply to, there being no
   rand() in this body at all), and its `r->unk_10 = 0x60' between two flag
   raises is moot because this copy raises 0xC once and 0x100 once with the
   whole mover block between them.  What DID carry is s46's shape fact that
   `r' IS ASSIGNED TWICE -- the target reloads n->unk_0C after the mover
   stores, and the second block's `lw $a0, 0xC($s0)' is that reload.

   MEASURED HERE, and the one fact that is this body's own:

   * THE THREE MOVER CHANNELS ARE WRITTEN TWICE: once from the raw parameter
     and once `+=' the caller's vector.  Written as the single statement
     `*(s16 *)((u8 *)m + 0x2) = dx + *(u16 *)((u8 *)v + 0x2);' it is 74 words
     and 14 aligned differences -- the missing word and the rows are the
     three `addu $v0, $sN, $zero' copies, which are cse forwarding the stored
     parameter into the read-modify-write.  A plain sum has no load to
     forward and emits one `addu' per channel.  Six stores, not three. */
void func_s02_800240C0(u8 *a, UnkVec *v, u8 *c, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x2) = 1;
        *(s16 *)(p + 0x4) = 1;
        n->unk_10 = (u32)func_s02_8002404C;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = dx;
        *(s16 *)((u8 *)m + 0x6) = dy;
        *(s16 *)((u8 *)m + 0xA) = dz;
        *(s16 *)((u8 *)m + 0x2) += *(u16 *)((u8 *)v + 0x2);
        *(s16 *)((u8 *)m + 0x6) += *(u16 *)((u8 *)v + 0x6);
        *(s16 *)((u8 *)m + 0xA) += *(u16 *)((u8 *)v + 0xA);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x400;
        r->unk_1C = 0x400;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_12 = 0x7DCF;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DE870, 2);
    }
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 life counter at +0x32 over 0x100. The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D5AC0 (120 words, resident), which is
   this function without the fade; the relink moves every scratchpad slot down
   by 4 -- the input SVECTOR to +0x00, the OT cursor to +0x18, the OT base to
   +0x20, RotTransPers' two out-parameters to +0x90/+0x94 and the OT index to
   +0xC0 -- so the struct is this unit's own, and the input vector is the
   scratchpad base itself, which is why $a0 is a bare register copy.

   THE FADE REPLACES THE TWIN'S DEAD setRGB0 ECHO. func_800D5AC0 needs
   setRGB0(t, t->r0, t->g0, t->b0) to keep three loads whose stores cse
   strips; here the three lbu/mult/sb runs are the real work and the echo must
   NOT be written -- added in front of the tag setup it is 151 words and 76
   differing.

   `/ 0x100' AND NOT `>> 8': the bgez/addiu 0xFF pair ahead of each sra is the
   signed division's correction, and `>> 8' drops it -- 139 words, 102
   differing.

   THE SCALE FACTOR IS RE-READ AT ALL THREE CHANNELS. A local for
   *(s16 *)(p + 0x32) is 145 words and 109 differing; the target has three
   separate `lh 0x32($s3)'.

   Three orderings, every one re-measured here rather than taken from the
   twin: the colour word and its three scalings come BEFORE the tag setup
   (setTile1 first is 147 words and 111 differing); `sp->unk_18' is assigned
   BEFORE `sp->unk_20' even though the target STORES them the other way round
   (swapped, 11 differing, against the twin's 20 for the same pair); and
   `p = (u8 *)n + 0x20;' comes before `v = n->unk_08;' (swapped, 17
   differing, against the twin's 4).

   setTile1(t) plus setSemiTrans(t, 1) is the whole `sb 2, 0x3' /
   `sb 0x6A, 0x7' pair: cse folds the getcode back to the 0x68 setTile1 just
   stored and drops the dead store. `(SVECTOR *)&sp->unk_00' written for
   `(SVECTOR *)sp' is a tie at 0.

   148 words, 0 differing. raw 7 == relocdump's 7, none on a function symbol:
   1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on RotTransPers, GetTPage and
   func_80067F20, and one on .text for the loop's own backward `j'. The
   +0x90/+0x94 addresses that splat auto-names D_1F800090/D_1F800094 carry no
   relocation on this side and come out byte-identical, so seqcheck counts one
   extra entry in the target and reports seq=FAIL on that bookkeeping alone. */
s32 func_s02_800241EC(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / 0x100;
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / 0x100;
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / 0x100;
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
void func_s02_8002443C(u8 *p, UnkVec *v, u8 *c)
{
    s32 i;

    *(u16 *)(p + 0x2) -= 1;
    *(s32 *)(p + 0x58) += *(s32 *)(p + 0x64);
    *(s32 *)(p + 0x5C) += *(s32 *)(p + 0x68);
    *(s32 *)(p + 0x60) += *(s32 *)(p + 0x6C);
    v->unk_00 += *(s32 *)(p + 0x58);
    v->unk_04 += *(s32 *)(p + 0x5C);
    v->unk_08 += *(s32 *)(p + 0x60);
    D_80025914 = 1;
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
   +0x14 scaled by its +0x32 countdown and shifted 10, with a fresh
   rand() & 0xFFFF jitter per axis and a flat +0x20000 climb PLUS
   rand() & 0xFFF on the third; then raise the bank tick, drop +0x32 by 8 and,
   at zero, raise the redraw bit in the node's -0x2 and in D_800814A0.
   85 words, 0 differing, FIRST COMPILE.  1 member, 85 words (dngfam.py).

   TWIN: the landed func_s01_80024448 (csrc/ovl_dng_s01.c, 93 words,
   dngtwin 0.921).  ovl_dng_s01 is this unit's sibling bank throughout --
   func_s02_8002443C/func_s01_80024280 are a 1.000 clone pair and
   func_s02_80024758/func_s01_800245BC another -- so s01 was read for the
   shape and this file's own landed bodies for the spellings.

   Four deltas, all read off this listing rather than guessed:
     - the shift is 10, not s01's 9;
     - the third axis is `+= 0x20000 + (rand() & 0xFFF)', the opposite SIGN
       from s01's `+= 0xFFE00000 - (rand() & 0xFFF)' (listing: `lui $a0,
       (0x20000 >> 16)' then two `addu', so a positive constant and a
       positive jitter);
     - the bank tick is this unit's D_80025914;
     - THE TAIL GATE HAS NO VIEW ARM.  s01's is
       `... <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0'; this copy tests only
       the countdown, and that absence is the whole of the 8-word delta
       dngtwin reports.  `c' is therefore never read and is declared only to
       match the step signature func_8004491C installs.

   Of s01's own stated facts, taken as a SHAPE twin (both frames are -0x20
   with two callee-saved saves, $s0/$s1, in the same order):
     - HOLDS: THE THREE PROBE ARGUMENTS GO THROUGH LOCALS AND `z' IS WRITTEN
       FIRST.  The listing emits `lhu 0xA', `lhu 0x2', `lhu 0x6' and only then
       the `li 1'/`sh' of the tick store, which is s01's measured z,x,y order
       and its measured position for the tick.  Not re-priced here: the body
       landed first compile with that order and I spent the budget on the
       bodies below instead, so treat it as INHERITED, not measured on this
       relink.
     - HOLDS: the rand() term comes SECOND in each sum (`addu $v0, $a0' with
       the product first).
     - FREE, as s01 also found: `*(u16 *)(p + 0x32) -= 8' folded into its own
       gate.  Kept folded. */
void func_s02_80024604(u8 *p, UnkVec *v, u8 *c)
{
    s16 z;
    s32 x;
    s32 y;

    z = *(u16 *)((u8 *)v + 0xA) + 2;
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025914 = 1;
    if (*(s16 *)((u8 *)v + 0xA) < (s16)func_800BCB04(x, y, z))
    {
        v->unk_00 += ((D_8006CCD8[*(s16 *)(p + 0x14)] * *(s16 *)(p + 0x32)) << 10) + (rand() & 0xFFFF);
        v->unk_04 += ((D_8006CCE8[*(s16 *)(p + 0x14)] * *(s16 *)(p + 0x32)) << 10) + (rand() & 0xFFFF);
        v->unk_08 += 0x20000 + (rand() & 0xFFF);
    }
    if ((s16)(*(u16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s02_80024604 as the step and
   func_s02_800241EC as the draw, with `b` at the payload's +0x14, `d` at
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
void func_s02_80024758(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s02_80024604;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s02_800241EC);
        *(u32 *)(p + 0x8) = c;
    }
}

/* The three-phase shuffle step: raise the bank tick, spend one of the
   payload's +0x2 frames and bump the +0x1C sub-counter; every second frame
   clear that counter and advance the +0x1E phase 0 -> 1 -> 2 -> 0, nudging
   the +0x28 byte by +8, +8, -0x10 as it goes; then integrate the +0x60
   velocity into the mover's Z and raise the redraw bit when the frames run
   out or when the view's +0x14 carries 0x8000.  74 words, 0 differing.
   1 member, 74 words (dngfam.py).

   NO TWIN at any ratio by dngtwin, and CALLEE.txt's row for it is the
   degenerate 2-symbol one the brief warns about -- its two hits are this
   file's own func_s02_8002404C and func_s02_8002443C, and that turned out to
   be the right answer rather than noise: both are landed HERE, and between
   them they supply every idiom this body needs (the D_80025914 tick, the
   `*(u16 *)(p + 0x1C) += 1' counter-bump-then-gate pair, the D_800814A0 kill
   tail written out TWICE for the two gates).  Neither xsibs nor the
   callee-set grep was run.

   ONE FACT MEASURED, AND IT INVERTS func_s02_8002404C FORTY WORDS ABOVE:

   * THE FRAME DECREMENT IS THE PLAIN COMPOUND `*(u16 *)(p + 0x2) -= 1', NOT
     A SPLIT READ/WRITE-BACK.  Split into `t = *(s16 *)(p + 0x2) - 1; <bump>;
     *(s16 *)(p + 0x2) = t;' -- which is exactly the spelling
     func_s02_8002404C's comment prices at 7 rows in its own favour -- this
     body is 74 words / 10 aligned differences, all ten in the prologue.  The
     mechanism is cse, not allocation: with the 0x2 write-back placed after
     the 0x1C bump it invalidates cse's entry for 0x1C, so the gate reloads
     with `lh $v1, 0x1C($a0)' where the target forwards the just-stored value
     through `sll 16'/`sra 16'.  Two compound assignments leave both loads at
     the top and let reorg take the 0x2 store into the branch's delay slot,
     which is what the target does.
     This is a REFERENCE-COUNT/STATEMENT-ORDER fact in the brief's taxonomy --
     the class where every recorded inversion lives -- so the inversion is
     the predicted outcome rather than a surprise.

   * The switch is a COMPARE CHAIN, not a table (jtcheck: 0 tables for this
     function), so its arms emit in source order and the order is priced:
     written descending 2, 1, 0 it is 70 words / 16 aligned differences.
     Ascending 0, 1, 2 is what lands. */
void func_s02_80024878(u8 *p, UnkVec *v, u8 *c)
{
    D_80025914 = 1;
    *(u16 *)(p + 0x2) -= 1;
    *(u16 *)(p + 0x1C) += 1;
    if (*(s16 *)(p + 0x1C) >= 2)
    {
        *(s16 *)(p + 0x1C) = 0;
        switch (*(s16 *)(p + 0x1E))
        {
        case 0:
            *(s16 *)(p + 0x1E) = 1;
            *(u8 *)(p + 0x28) += 8;
            break;
        case 1:
            *(s16 *)(p + 0x1E) = 2;
            *(u8 *)(p + 0x28) += 8;
            break;
        case 2:
            *(s16 *)(p + 0x1E) = 0;
            *(u8 *)(p + 0x28) -= 0x10;
            break;
        }
    }
    v->unk_08 += *(s32 *)(p + 0x60);
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

/* The 0x212 spark spawner: allocate the node, give it the caller's own s16
   at the payload's +0x2 and a random (rand() & 0x1FFFF) + 0x10000 fall rate
   at +0x60, park func_s02_80024878 as the step, raise 0xC in the
   renderable's flags, seed the mover from the parent node's own mover plus
   the three s16 offsets, then dress it -- 0x1000 both scale halves, 0x80 over
   r+0xC..0xE -- and copy the 12-byte D_80025908 name record into the node's
   own +0x40, pointing the slot's +0x8 at it.  93 words, 0 differing, FIRST
   COMPILE.  1 member, 93 words (dngfam.py).

   dngtwin's best row is func_b08_801741D0 at 0.743 and I never opened it.
   What landed this body is the UNUSED TYPEDEF LEAD from the brief: this
   unit's declaration block already carried `UnkDngS58Name12' with no user,
   and `grep -ln UnkDngS58Name12 csrc/*.c' names csrc/ovl_dng_s58.c, whose
   landed func_s58_8002553C is this exact skeleton --
   func_8003FC64(0x212) / +0x10 install / func_8004491C(n, D_80045340) /
   n->unk_0C re-read after the mover stores / a 12-byte struct copy into the
   node / slot->unk_08 pointing at it.  The `lwl'/`lwr' + `swl'/`swr' run in
   the listing IS that typedef: a struct whose type has ALIGNMENT 1
   (`u8 b[0xC]'), which is s58's own stated fact and the reason the copy is
   not three aligned `lw's.

   Carried from s58 and HOLDING: `r' IS ASSIGNED TWICE -- the target reloads
   n->unk_0C after the mover stores.  MEASURED here rather than inherited:
   with a single assignment it is 92 words / 28 aligned differences, the
   missing word being the second `lw $a3, 0xC($s1)'.

   Deltas off my own listing: 0x1000 scales rather than s58's zeros, no
   r->unk_10/r->unk_06 clear, the mover is `parent mover + parameter' per
   channel instead of a flat copy, and the record goes to +0x40 not +0x58.
   Everything else is first-compile and therefore UNMEASURED -- in particular
   the 0xE/0xD/0xC descending byte order and `*((u8 *)r + 0xE)' as a plain
   cast rather than `r->unk_0C[2]' are s58's/s46's spellings taken on trust,
   not facts about this body. */
void func_s02_800249A0(UnkNode *a, s16 dx, s16 dy, s16 dz, s16 e)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x2) = e;
        *(s32 *)(p + 0x60) = (rand() & 0x1FFFF) + 0x10000;
        n->unk_10 = (u32)func_s02_80024878;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + dx;
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + dy;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + dz;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *(UnkDngS58Name12 *)((u8 *)n + 0x40) = *(UnkDngS58Name12 *)D_80025908;
        r->unk_08 = (u32)((u8 *)n + 0x40);
    }
}

/* The growth step: raise the bank tick, add 2 to the payload's +0x2 age
   and, while that is under 0x28, drive BOTH of the renderable's scale halves
   at age * 204; at 0x78 and again when the view's +0x14 carries 0x8000,
   raise the redraw bit in the node's -0x2 and in D_800814A0.  53 words,
   0 differing, FIRST COMPILE.  1 member, 53 words (dngfam.py).

   NO TWIN at any ratio, and as with func_s02_80024878 the only lead was
   CALLEE.txt's degenerate 2-symbol row naming this file's own landed
   func_s02_8002404C and func_s02_8002443C.  8002404C is the whole answer:
   its read/tick/write-back split and its `lhu'-from-an-`s16'-read idiom are
   what this body is built from, and 8002443C supplied the D_800814A0 kill
   tails.  No xsibs, no callee-set grep, no donor outside this file.

   ONE FACT MEASURED, and it is func_s02_8002404C's at 8002404C's own sign --
   note that func_s02_80024878 forty words below INVERTS it, so the two
   neighbours disagree and the target is the only authority:

   * THE AGE READ-MODIFY-WRITE MUST BE SPLIT AROUND THE TICK STORE.
     `t = *(s16 *)(p + 0x2) + 2; D_80025914 = 1; *(s16 *)(p + 0x2) = t;' is
     53 words / 0 differing; folded into the compound
     `*(u16 *)(p + 0x2) += 2' with the tick store first it is 53 words /
     14 aligned differences.  The target emits read, foreign store,
     write-back in that order and one compound assignment cannot produce it.

   * `* 204' IS WRITTEN AS THE PLAIN CONSTANT.  The listing's
     sll/addu/sll/addu/sll run is what cc1psx turns `* 204' into; it is not
     a decomposition to be transcribed.

   * NOT MEASURED, because the body landed first compile: the four reads of
     +0x2 after the write-back are all plain `*(s16 *)(p + 0x2)'.  cse
     forwards the first two from the write-back and the `sh 0x1C($a2)' store
     invalidates the third, which is why the target has one `sll'/`sra' pair
     and then two `lh' reloads -- no local, and no hoist, is needed to get
     that pattern. */
void func_s02_80024B14(u8 *p, UnkVec *v, u8 *c)
{
    s16 t;

    t = *(s16 *)(p + 0x2) + 2;
    D_80025914 = 1;
    *(s16 *)(p + 0x2) = t;
    if (*(s16 *)(p + 0x2) < 0x28)
    {
        *(s16 *)(c + 0x1C) = *(s16 *)(p + 0x2) * 204;
        *(s16 *)(c + 0x1E) = *(s16 *)(p + 0x2) * 204;
    }
    if (*(s16 *)(p + 0x2) >= 0x78)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s02", func_s02_80024BE8);
