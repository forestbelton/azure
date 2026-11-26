#include "common.h"

void func_s54_8002434C(u8 *p, u8 *b, u8 *c);
void func_s54_80024654(u8 *p, s16 b, s16 c, u8 *d, s32 f);
void func_s54_80024830(u8 *p);
void func_s54_80024908(u8 *p, s16 b, s16 c, u32 d);
UnkNode *func_8003FD64(s32, u8 *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800644B8(s32);
s32 func_80064584(s32);
s32 func_800A56E0(s32);
void func_800B835C(u8 *, s32 *, s32, s32);
void func_800B8C20(s32, s32, u8 *, u8 *);
s32 func_800BCB04(s32, s32, s16);
s32 func_800CEEFC(u8 *, UnkVec *, UnkSlot *);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80082E80[];
extern u16 D_80082E94;
extern u8 D_800DF334[];
extern u8 *D_800E3D7C;
extern u8 D_80024980[];
extern u8 D_80024998[];
extern s16 D_800249A4;
extern s16 D_800249A6;

/* This bank's five-state boss cutscene dispatcher and the OWNER OF
   jtbl_s54_80024008 (5 words = 5 cases + 0 pad, dispatch bound 5, 5 distinct
   targets). A JUMP TABLE on *(s16 *)(p + 0xA) with no minval subtraction, so
   the case set is 0..4; the range-check `beqz' lands on the function's COMMON
   TAIL (`D_800249A6 = 0', which every arm falls into) rather than on a table
   entry, so there is no default among the five. State 0 runs func_800B835C
   twice over the stack pair -- {0x1000340, 0x200020} on this bank's
   D_800DF334, then {0x1000360, 0x200020} on D_80024980 -- publishes
   D_80024998 into D_800814A8's +0xF4, calls func_800B8C20 on the acting
   node less 0x20, and FALLS THROUGH into state 1, which waits on bit 0x80 of
   **(p + 0x4), arms the 0x12 countdown, decrements D_800814A8's +0xA6,
   copies p[8]/p[9] into its +0xA8/+0xA9, clears D_800249A4 and latches its
   +0x2A into +0x14. State 2 cues func_800A56E0(0x300) at 8 frames and
   spawns the 0x200 node through func_s54_80024908. State 3 ors 0x10000000
   into D_800814A8's +0x60 record, pulls that record's payload +0xC/+0xD/+0xE
   bytes down by 4 with a 0x20 floor, then (once D_80082E94's 0x8000 is set
   or the countdown goes negative) flattens D_800E3D7C's +0x60 payload to
   0x808080 and sets D_800249A4. State 4 waits for D_800249A6 to read 0,
   decrements D_80083460's +0xA, clears its +0xC and retires the actor.
   204 words, 0 differing.

   DONOR: csrc/ovl_dng_s69.c's landed func_s69_80024020, found by
   dngcallee (5 of 13 symbols, a 5-function tier). dngtwin finds NO carved
   sibling at any ratio for any function in this unit. s69's body is the same
   shape -- a one-parameter dispatcher on +0xA with an `s32 arr[2]' stack
   pair, a fall-through from state 0 into a `**(p + 0x4) & 0x80' gate, a
   D_800814A8 +0xA6 decrement, a &D_80083460 retire arm and a single
   fixed-address store as the COMMON TAIL after the switch. Its frame is
   -0x40 with five saves against this body's -0x20 with one, so none of its
   budget facts can transfer; its shape facts all did.

   FIVE FACTS, every one re-priced from this body AT 0 DIFFERING:

   * THE FOUR +0xA STATE BUMPS MUST BE u16 ARRAY REFS, `((u16 *)p)[5]'.
     As the plain cast `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1' this body
     is 202 WORDS / 16 ALIGNED ROWS. This is s69's own fact (1) reproducing
     on a relink with a different frame: the array ref needs a PLUS_EXPR on
     top to set MEM_IN_STRUCT_P, and true_dependence's
     fixed-scalar-versus-varying-struct exception then frees the `lhu 0xA'
     to move ABOVE the fixed-address `sh %lo(D_800249A4)' store beside it.
   * EVERY +0x18 COUNTDOWN ACCESS MUST BE `((s16 *)p)[0xC]' -- all six of
     them, across states 1, 2 and 3. Plain casts are 205 WORDS / 16 ROWS, the
     extra word being a `nop' the target fills. Same mechanism, this time
     against the `lw %lo(D_800814A8)' fixed-scalar LOAD.
   * STATE 3's TWO HALVES SHARE ONE POINTER PAIR. `q' and `r' are assigned
     twice each -- once off D_800814A8's +0x60 and again off D_800E3D7C's --
     and that is worth 23 ALIGNED ROWS: with separate `d'/`e' locals for the
     second half it is 204 words / 23 rows, every row a register rotation
     ($a0/$a1 against $v0/$v0 in the second half). Two pseudo pairs in
     disjoint arms each get their own registers; one pair spanning both arms
     is one allocno pair and takes $a0/$a1 throughout, which is what the ROM
     has. This is the brief's "one local too many", with the merge running
     ACROSS arms rather than within one.
   * STATE 4 NEEDS THE POINTER LOCAL `h = (u8 *)&D_80083460'. Written inline
     at both sites it is 204 words / 6 ROWS, because fold hoists
     `&D_80083460 + 0xA' into the base register and the +0xC store then emits
     as `+0x2' off that -- the brief's folded-offset trap. The same local
     appears in csrc/ovl_dng_s69.c and csrc/ovl_dng_s70.c for this symbol.
   * STATE 1's +0x18 STORE MUST BE ITS FIRST STATEMENT, before the +0xA6
     decrement. Second it is 204 words / 10 ROWS: reorg then fills the gate's
     `beqz' delay slot with the `addiu 0x12' instead of the
     `lui %hi(D_800814A8)' the ROM has, and the `lw' load delay goes unfilled.

   PRICED AND REJECTED, from mid-fight bodies rather than from 0 (so read the
   DIRECTION, not the figure): all twelve orderings of state 1's last three
   statements, 53..69 rows, with the kept order (D_800249A4, +0x14, +0xA) the
   best of its head group.

   MEASURED FREE MID-FIGHT, NOT RE-PRICED FROM 0 -- do not build on these:
   the switch's own read and the +0x16 head bump written as array refs, both
   43 rows, i.e. identical to the kept plain casts at that point.

   ARM ORDER, since ovlverify, aligndiff and relcand are all blind to it.
   Standalone `tools/cdkgcc -O2 -G0 -S' emits ONE `.rdata' table of exactly 5
   `.word's: [$L3, $L4, $L6, $L9, $L17], at strictly ascending arm-block
   addresses $L3 < $L4 < $L6 < $L9 < $L17. The ROM's five entries
   (80024064, 800240F4, 8002416C, 800241D8, 800242E4) are also strictly
   ascending and 1:1 positional with them, so the ascending source order is
   the ROM's entry order. No pad word either side. */
void func_s54_8002401C(u8 *p)
{
    u8 *q;
    u8 *r;
    u8 *h;
    s32 arr[2];

    *(s16 *)(p + 0x16) = *(u16 *)(p + 0x16) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_800DF334, arr, 1, 0);
        arr[0] = 0x1000360;
        func_800B835C(D_80024980, arr, 1, 0);
        *(u8 **)(D_800814A8 + 0xF4) = D_80024998;
        func_800B8C20(*(s32 *)(p + 0x0) - 0x20, *(s32 *)(p + 0x4), 0, 0);
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        ((s16 *)p)[0xC] = 0x12;
        *(s16 *)(D_800814A8 + 0xA6) = *(u16 *)(D_800814A8 + 0xA6) - 1;
        *(u8 *)(D_800814A8 + 0xA8) = *(u8 *)(p + 0x8);
        *(u8 *)(D_800814A8 + 0xA9) = *(u8 *)(p + 0x9);
        D_800249A4 = 0;
        *(s16 *)(p + 0x14) = *(u16 *)(D_800814A8 + 0x2A);
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        break;
    case 2:
        if (((s16 *)p)[0xC] == 8)
        {
            func_800A56E0(0x300);
        }
        ((s16 *)p)[0xC] = ((u16 *)p)[0xC] - 1;
        if (((s16 *)p)[0xC] > 0)
        {
            break;
        }
        ((s16 *)p)[0xC] = 0x20;
        func_s54_80024908(p - 0x20, *(s16 *)(p + 0x14), *(u8 *)(p + 0x9),
                          *(u32 *)(D_800814A8 + 0x60));
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        break;
    case 3:
        q = *(u8 **)(D_800814A8 + 0x60);
        if (q != NULL)
        {
            r = *(u8 **)(q - 0x14);
            *(s32 *)(q + 0x1C) |= 0x10000000;
            *(u8 *)(r + 0xC) = *(u8 *)(r + 0xC) - 4;
            *(u8 *)(r + 0xD) = *(u8 *)(r + 0xD) - 4;
            *(u8 *)(r + 0xE) = *(u8 *)(r + 0xE) - 4;
            if (*(u8 *)(r + 0xC) < 0x20)
            {
                *(u8 *)(r + 0xC) = 0x20;
            }
            if (*(u8 *)(r + 0xD) < 0x20)
            {
                *(u8 *)(r + 0xD) = 0x20;
            }
            if (*(u8 *)(r + 0xE) < 0x20)
            {
                *(u8 *)(r + 0xE) = 0x20;
            }
        }
        if ((D_80082E94 & 0x8000) == 0)
        {
            ((s16 *)p)[0xC] = ((u16 *)p)[0xC] - 1;
            if (((s16 *)p)[0xC] >= 0)
            {
                break;
            }
        }
        q = *(u8 **)(D_800E3D7C + 0x60);
        if (q != NULL)
        {
            r = *(u8 **)(q - 0x14);
            *(s32 *)(r + 0xC) = 0x808080;
        }
        D_800249A4 = 1;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        break;
    case 4:
        if (D_800249A6 != 0)
        {
            break;
        }
        h = (u8 *)&D_80083460;
        *(s16 *)(h + 0xA) = *(u16 *)(h + 0xA) - 1;
        *(s32 *)(h + 0xC) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    D_800249A6 = 0;
}

/* The step of the 0x212 node func_s54_80024654 spawns, so the (p, b, c)
   signature is fixed by that call site's func_8004491C/func_800CEEFC pair.
   Bumps D_800249A6, and ONLY WHILE D_800249A4 IS SET seeds the velocity pair
   at +0x0/+0x4 from func_80064584/func_800644B8 of the +0x28 heading scaled
   by the +0x34 speed and advances the state to 2. Copies the mover's
   +0x2/+0x6/+0xA position into b's +0xE/+0x12/+0x16 shadow, then runs a
   THREE-CASE COMPARE CHAIN on *(s16 *)(p + 0x24) -- three arms is below gcc
   2.7.2's case_values_threshold so this is a balanced comparison tree and
   the function owns NO table. Case 0 eases the slot's +0x1C/+0x1E scale a
   +0x2C'th of the way to 0x1000 with an 0x800 ceiling, brightens the
   +0xC/+0xD/+0xE colour triple by 2 with a 0x80 ceiling, recomputes the
   velocity against the +0x10/+0x12 base, lifts +0x8 by 0x5000, grows the
   speed by 4 to a 0x40 cap, turns the heading by 0x180 and advances when the
   +0x2C counter runs out. Case 1 is EMPTY and must be written out. Case 2
   adds half of each velocity component back, lifts +0x8 by 0x10000, fades
   the colour triple by 4 and retires the actor when it goes non-positive --
   that arm RETURNS rather than breaking, which is why its `j' goes to the
   epilogue and not to the shared tail. The tail bumps +0x2A and integrates
   the velocity triple into b. 194 words, 0 differing.

   NO DONOR: dngtwin finds no carved sibling at any ratio and dngcallee's
   best overlap is 3 of 5 symbols reached by 43 functions -- the noise tier.
   Written from the listing.

   FOUR FACTS, all re-priced from this body AT 0 DIFFERING:

   * THE FIRST GATE IS `if (D_800249A4 != 0)', NOT `== 0'. Spelled the other
     way this body is 195 words / 43 ALIGNED ROWS. The target's
     `beqz $v1, .Ls54_800243E8' SKIPS the velocity seed when the flag is
     clear, which is the opposite of the reading the arm's position suggests.
   * EVERY +0x24 STATE ACCESS MUST BE `((s16 *)p)[0x12]' -- the gate's read,
     the `= 2' store, the switch's own read and both bumps. Plain casts are
     195 WORDS / 18 ROWS. Same MEM_IN_STRUCT_P mechanism as
     func_s54_8002401C's +0xA bumps, here against the `lw %lo(D_800814A0)'
     fixed-scalar load in case 2's retire block.
   * THE THREE READ-MODIFY-WRITE TRIPLES NEED AN s32 LOCAL holding the
     value, with the ceiling test on `(u16)t' / `(u8)u' / `(s8)u'. Written as
     chained assignments with the test re-reading memory the body is 195
     words / 43 rows -- the target forwards one SImode value into the
     `andi 0xFFFF' / `andi 0xFF' / `sll 24' the tests compile to, and a
     narrowed store plus a widened reload cannot be equated by cse.
     (Priced inside that 43 alongside the inverted gate, not separated.)
   * `((s16 *)p)[0x12] = 2' MUST BE THE LAST STATEMENT OF THE SEED BLOCK,
     after BOTH `sw's. Between them it is 22 rows, because a store between
     two calls cannot be scheduled past either, and the ROM has it inside the
     second `mult's shadow.

   PRICED AND REJECTED: all six statement orders of case 2's retire block,
   with the +0x24 accesses still plain casts -- 18 / 16 / 14 / 5 / 17 / 19
   aligned rows. The 5-row order (D_800814A0 first, then +0x24, then -0x2)
   looked like the answer and was not: once the +0x24 accesses became array
   refs the NATURAL order (-0x2, +0x24, D_800814A0) measured 0 and that
   5-row order did not. A statement-order sweep found a local minimum around
   a fault that was not a statement-order fault at all.

   `((u16 *)p)[-1]' for the -0x2 retire flag is 11 to 17 rows depending on
   the block order: KEEP the plain cast there. The two polarities 40 words
   apart are the documented MEM_IN_STRUCT_P pattern -- read the `nop's, do
   not pick a house style. */
void func_s54_8002434C(u8 *p, u8 *b, u8 *c)
{
    s32 t;
    s32 u;

    D_800249A6 = (u16)D_800249A6 + 1;
    if (D_800249A4 != 0)
    {
        if (((s16 *)p)[0x12] < 2)
        {
            *(s32 *)(p + 0x0) = (func_80064584(*(s16 *)(p + 0x28)) * *(s16 *)(p + 0x34)) << 3;
            *(s32 *)(p + 0x4) = (func_800644B8(*(s16 *)(p + 0x28)) * *(s16 *)(p + 0x34)) << 3;
            ((s16 *)p)[0x12] = 2;
        }
    }
    *(s16 *)(b + 0xE) = *(u16 *)(b + 0x2);
    *(s16 *)(b + 0x12) = *(u16 *)(b + 0x6);
    *(s16 *)(b + 0x16) = *(u16 *)(b + 0xA);
    switch (((s16 *)p)[0x12])
    {
    case 0:
        t = *(u16 *)(c + 0x1E) + (0x1000 - *(u16 *)(c + 0x1E)) / *(s16 *)(p + 0x2C);
        *(s16 *)(c + 0x1E) = t;
        *(s16 *)(c + 0x1C) = t;
        if ((u16)t > 0x800)
        {
            *(s16 *)(c + 0x1E) = 0x800;
            *(s16 *)(c + 0x1C) = 0x800;
        }
        u = *(u8 *)(c + 0xE) + 2;
        *(u8 *)(c + 0xE) = u;
        *(u8 *)(c + 0xD) = u;
        *(u8 *)(c + 0xC) = u;
        if ((u8)u > 0x80)
        {
            *(s32 *)(c + 0xC) = 0x808080;
        }
        *(s32 *)(b + 0x0) = (*(s16 *)(p + 0x10) << 16) +
                            ((func_80064584(*(s16 *)(p + 0x28)) * *(s16 *)(p + 0x34)) << 3);
        *(s32 *)(b + 0x4) = (*(s16 *)(p + 0x12) << 16) +
                            ((func_800644B8(*(s16 *)(p + 0x28)) * *(s16 *)(p + 0x34)) << 3);
        *(s32 *)(p + 0x8) = *(s32 *)(p + 0x8) - 0x5000;
        *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 4;
        if (*(s16 *)(p + 0x34) > 0x40)
        {
            *(s16 *)(p + 0x34) = 0x40;
        }
        *(s16 *)(p + 0x28) = *(u16 *)(p + 0x28) + 0x180;
        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) - 1;
        if (*(s16 *)(p + 0x2C) <= 0)
        {
            ((s16 *)p)[0x12] = ((u16 *)p)[0x12] + 1;
        }
        break;
    case 1:
        break;
    case 2:
        *(s32 *)(p + 0x0) = *(s32 *)(p + 0x0) + (*(s32 *)(p + 0x0) >> 1);
        *(s32 *)(p + 0x4) = *(s32 *)(p + 0x4) + (*(s32 *)(p + 0x4) >> 1);
        *(s32 *)(p + 0x8) = *(s32 *)(p + 0x8) + 0x10000;
        u = *(u8 *)(c + 0xE) - 4;
        *(u8 *)(c + 0xE) = u;
        *(u8 *)(c + 0xD) = u;
        *(u8 *)(c + 0xC) = u;
        if ((s8)u <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            ((s16 *)p)[0x12] = ((u16 *)p)[0x12] + 1;
            D_800814A0 |= 0x8000;
            return;
        }
        break;
    }
    *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) + 1;
    *(s32 *)(b + 0x0) = *(s32 *)(b + 0x0) + *(s32 *)(p + 0x0);
    *(s32 *)(b + 0x4) = *(s32 *)(b + 0x4) + *(s32 *)(p + 0x4);
    *(s32 *)(b + 0x8) = *(s32 *)(b + 0x8) + *(s32 *)(p + 0x8);
}

/* Spawn one 0x212 projectile node, stepped by func_s54_8002434C and rendered
   by func_800CEEFC, unless D_800249A4 is already set. Points the slot at
   D_80024998 with a 0x20 scale and 0xC in its +0x14 flags, writes the
   heading `b' (plus 0x800 when `f' is set) at the payload's +0x26 and
   0x1000 minus it at +0x28, then dresses the position two ways: with no
   target record it reads this bank's facing pair D_8006CCD8/D_8006CCE8 at
   (b >> 9) & 7, biases by D_80082E80's +0x24/+0x25 cell, scales by 64 and
   adds 0x20, and takes the height from D_800814A8's +0x88; with a target
   record `d' it copies the record's own +0x2/+0x6/+0xA and clamps the height
   to the record's when func_800BCB04's bearing exceeds 0x200. 119 words,
   0 differing.

   THE THIRD PARAMETER `c' IS NEVER READ. The ROM saves $a1, $a3 and the
   stack argument and uses them; $a2 is set by both call sites in
   func_s54_80024830 (from the payload's +0x30) and the body never touches
   it. That is not a transcription gap -- it is what the listing shows, and
   removing the parameter would change both call sites' register setup.

   DONOR: csrc/ovl_dng_s70.c's landed func_s70_8002401C, which dngcallee
   named at 6 of 9 symbols in a clean two-function tier. Its state 3 reads the
   same D_8006CCD8/D_8006CCE8 facing pair with the same D_80082E80 bias, and
   its `(x >> 9) & 7' index spelling carried verbatim. dngtwin finds nothing.

   TWO FACTS, both re-priced from this body AT 0 DIFFERING:

   * THE INDEX MUST NOT CARRY A `(u16)' CAST. `D_8006CCD8[(b >> 9) & 7]' is
     what lands; `((u16)b >> 9) & 7' is 122 WORDS / 16 ALIGNED ROWS. The cast
     makes a SECOND pseudo for the parameter -- the emitted code gains a bare
     `move $19,$17' on top of the entry copy -- which is one long-lived value
     too many, so the frame grows from -0x28 to -0x30, a sixth callee-saved
     register is saved and restored, and every later name shifts. The
     diagnostic is the brief's "extra or missing `move' with every later
     register displaced is a WRONG PSEUDO COUNT": read the prologue, not the
     shift. Note the kept form still emits `srl'/`andi 0xE' on a SIGNED
     parameter, because combine merges the shift and the mask.
   * THE SIX HALFWORD STORES INTO `e' AND `q' MUST BE ARRAY REFS
     (`((s16 *)e)[0x9]', `((s16 *)q)[0x3]', ...). As plain casts the body is
     120 WORDS / 9 ROWS: MEM_IN_STRUCT_P is false, so true_dependence refuses
     to excuse them against the fixed-address `lw %lo(D_800814A8)' load
     beside them, that load cannot hoist into the second scale's shadow, and
     the extra word is the `nop' it then needs. The two +0x26 stores and the
     +0x28, +0xC, +0x2C and +0x32 stores stay PLAIN CASTS and are 0 either
     way -- only the three dress pairs are load-bearing.

   MEASURED FREE, re-priced from 0, so NOT the reason this lands: the facing
   lookup written as the integer sum `*(s16 *)(((b >> 9) & 7) * 2 +
   (s32)D_8006CCD8)' instead of the array ref is 119 words / 0 ROWS -- byte
   identical. A RETRACTION: mid-fight that integer form measured 2 rows and
   I read it as the brief's "a SYMBOL_REF costs no insn at expand time, so
   hoist the base to give it an INSN_LUID" lever deciding the `andi's
   scheduler tie. It was not. Those 2 rows were the plain-cast stores above
   still being wrong, and once they were array refs BOTH index spellings
   reached 0. The array ref is kept only because it is the corpus idiom and
   the shorter C. The scales must be `* 64' rather than `<< 6' (convert.c
   narrowing, per csrc/ovl_dng_s70.c's func_s70_80024B48) -- NOT re-priced
   here, inherited. */
void func_s54_80024654(u8 *p, s16 b, s16 c, u8 *d, s32 f)
{
    UnkNode *n;
    u8 *e;
    u8 *q;
    u8 *r;
    u8 *w;
    s32 t;

    if (D_800249A4 != 0)
    {
        return;
    }
    n = func_8003FD64(0x212, p);
    if (n == NULL)
    {
        return;
    }
    n->unk_10 = (u32)func_s54_8002434C;
    func_8004491C(n, (u32)func_800CEEFC);
    q = (u8 *)n->unk_08;
    r = (u8 *)n->unk_0C;
    *(u8 **)(r + 0x8) = D_80024998;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x14) |= 0xC;
    e = (u8 *)n + 0x20;
    *(s16 *)(e + 0x26) = b;
    if (f != 0)
    {
        *(s16 *)(e + 0x26) = b + 0x800;
    }
    *(s16 *)(e + 0x28) = 0x1000 - *(u16 *)(e + 0x26);
    if (d == NULL)
    {
        ((s16 *)e)[0x8] = ((s16 *)q)[0x1] =
            (D_80082E80[0x24] + D_8006CCD8[(b >> 9) & 7]) * 64 + 0x20;
        ((s16 *)e)[0x9] = ((s16 *)q)[0x3] =
            (D_80082E80[0x25] + D_8006CCE8[(b >> 9) & 7]) * 64 + 0x20;
        ((s16 *)e)[0xA] = ((s16 *)q)[0x5] = *(u16 *)(D_800814A8 + 0x88);
    }
    else
    {
        w = *(u8 **)(d - 0x18);
        ((s16 *)e)[0x8] = ((s16 *)q)[0x1] = *(u16 *)(w + 0x2);
        ((s16 *)e)[0x9] = ((s16 *)q)[0x3] = *(u16 *)(w + 0x6);
        ((s16 *)e)[0xA] = *(u16 *)(w + 0xA);
        t = func_800BCB04(((u16 *)q)[0x1], ((u16 *)q)[0x3], *(s16 *)(w + 0xA));
        if ((s16)t > 0x200)
        {
            t = *(u16 *)(w + 0xA);
        }
        ((s16 *)q)[0x5] = t;
    }
    *(u8 **)(e + 0xC) = d;
    *(s16 *)(e + 0x2C) = 0x40;
    *(s16 *)(e + 0x32) = f;
}

/* The step of the 0x200 node func_s54_80024908 spawns. Bumps D_800249A6 and
   fires func_s54_80024654 TWICE off the node base -- once with the flag 0
   and once with it 1 -- each time re-reading the payload's +0x26 heading and
   +0x30 value and D_800814A8's +0x60 target record, then counts the +0x2C
   timer down and retires the actor when it goes negative or D_800249A4 has
   been set. 54 words, 0 differing.

   NO DONOR WORTH THE NAME: dngtwin's best is 0.755 on a 23-word body.
   Written from the listing.

   ONE MECHANISM, AND IT IS A CONJUNCTION OF TWO SPELLINGS -- a one-at-a-time
   sweep cannot reach this body. Both argument forms must be ARRAY REFS:

       both array refs (kept)                                 54 /  0
       `((s16 *)p)[0x13]'/`[0x18]' only, 4th arg plain cast   54 / 14
       `((u8 **)D_800814A8)[0x18]' only, the two lh plain      54 / 17
       neither                                                54 / 17

   Each half alone leaves the other half's residue, so neither reaches 0 and
   neither looks like progress worth keeping. The mechanism is one thing seen
   twice: `D_800249A6' is a FIXED-ADDRESS SCALAR and its store has to reach
   the first `jal's delay slot, which means the varying loads that make up
   the call's arguments must be free to schedule ABOVE it. `-da' `.sched'
   names it outright -- with plain casts the two `lh' argument loads carry
   `(insn_list 22 ...)', a TRUE dependence on the `sh %lo(D_800249A6)' store,
   because fixed_scalar_and_varying_struct_p only excuses a varying reference
   that is MEM_IN_STRUCT_P. With the array refs the dump shows `mem/s:HI' and
   no dependence at all. The 4th argument is the same thing one level down:
   `*(u8 **)(D_800814A8 + 0x60)' is a varying load off a fixed-scalar pointer
   and needs the same flag.

   NOT MEASURABLE, so do not read it as free: the `o = p - 0x20' local. With
   the body still at 17 rows, `o' first, `o' after the bump, and no local at
   all with `p - 0x20' written inline at both calls ALL measured 54 / 17 --
   a flat sweep, which says the position is not measurable rather than that
   it does not matter.

   PRICED AND REJECTED: the D_800249A6 bump as the array ref
   `((u16 *)&D_800249A6)[0]' (17 rows, i.e. nothing -- and it would be the
   wrong direction, since it is the FIXED scalar here and must stay one);
   the bump moved between the two calls (21 rows). */
void func_s54_80024830(u8 *p)
{
    u8 *o;

    o = p - 0x20;
    D_800249A6 = (u16)D_800249A6 + 1;
    func_s54_80024654(o, ((s16 *)p)[0x13], ((s16 *)p)[0x18],
                      ((u8 **)D_800814A8)[0x18], 0);
    func_s54_80024654(o, ((s16 *)p)[0x13], ((s16 *)p)[0x18],
                      ((u8 **)D_800814A8)[0x18], 1);
    *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) - 1;
    if (*(s16 *)(p + 0x2C) < 0 || D_800249A4 != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x200 node that func_s54_8002401C's state 2 hands the boss
   heading to: func_s54_80024830 as its step, the heading at the payload's
   +0x26, the target record at +0x2C, the +0x30 value and a 0x40 timer at
   +0x2C of the node (payload +0xC/+0x30/+0x2C). 30 words, 0 differing,
   FIRST COMPILE.

   FIRST COMPILE, so NOTHING IN THIS BODY IS MEASURED -- every spelling in it
   was chosen and happened to work. In particular the four payload stores are
   written in the listing's emitted order (+0x26, +0xC, +0x30, +0x2C) and
   that order was never priced; three of them are independent and sched is
   free to transpose them, so treat the order as unmeasured rather than as a
   fact. The `e = (u8 *)n + 0x20' local is likewise unpriced: `n' dies at
   that assignment, so combine_regs ties the two pseudos and the ROM's
   `addiu $v0, $v1, 0x20' comes out either way, but that was not tested. */
void func_s54_80024908(u8 *p, s16 b, s16 c, u32 d)
{
    UnkNode *n;
    u8 *e;

    n = func_8003FD64(0x200, p);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s54_80024830;
        e = (u8 *)n + 0x20;
        *(s16 *)(e + 0x26) = b;
        *(u32 *)(e + 0xC) = d;
        *(s16 *)(e + 0x30) = c;
        *(s16 *)(e + 0x2C) = 0x40;
    }
}
