#include "common.h"

typedef struct UnkDngS31Dirs
{
    u8 unk_00[0x20];
} UnkDngS31Dirs;
extern u8 D_s31_80024004[];
extern s16 D_s31_80024D04;
extern u8 D_80045340[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern u8 D_800DE870[];
extern u8 D_800DEC00[];
extern u8 D_800E3D68;
void func_s31_80024024(u8 *p, UnkVec *v, UnkSlot *c);
void func_s31_80024124(u8 *p, UnkVec *v, UnkSlot *c);
void func_s31_80024270(u8 *p, UnkVec *v, UnkSlot *c);
void func_8003DB94(void *, void *, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
void func_800478B8(u8 *);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
s32 func_800A6D30(void);
s32 func_800C8CD8(u8 *, s16, s16);

/* The plain mover step for this bank's 0x212 motes: integrate the three
   +0x58..+0x60 velocities into the mover, raise the bank tick, refresh the
   renderable once, lift both of its scale halves by 0x80 a frame and spend one
   of the payload's +0x2 frames; when they run out, or when the renderable's
   +0x14 carries 0x8000, raise the redraw bit in the node's -0x2 and in
   D_800814A0.  64 words, 0 differing, FIRST COMPILE.  2 members, 128 words
   (dngfam.py).  OWNS NO JUMP TABLE (jtcheck: 0 tables).

   NO TWIN WAS READ.  LEADS.txt offers func_s15_8002418C at 0.712 and
   func_s46_80024C0C at 0.638 and I opened neither; dngcallee's row for this
   function is the 2-of-3 noise tier the brief warns about (96 functions, mostly
   town).  The body is written from the listing against csrc/ovl_dng_s01.c's
   func_s01_80024280 -- the same shared library's step handler, which dngcallee
   named for func_s31_80024270 below -- and that gave the mover-add idiom, the
   D_800814A0 kill pair and the bank-tick spelling.

   THREE SPELLINGS PRICED FROM THIS BODY AT 0, since a first compile prices
   nothing:

   LOAD-BEARING:
   * THE TWO TAIL GATES ARE SEPARATE `if's WITH THE KILL PAIR WRITTEN OUT
     TWICE.  As one `if (frames <= 0 || (c->unk_14 & 0x8000) != 0)' it is 56
     words / 13 aligned differences, the eight missing words being the second
     copy.  func_s01_80024280 prices its own version of this at 13 rows too, so
     the figure reproduces across the relink.
   * THE TICK STORE IS THE FOURTH STATEMENT, after the three mover adds.  First
     in the function it is 64 words / 12 differences.

   MEASURED AND FREE, so NOT why this lands: the countdown folded into its gate,
   `if ((s16)(*(u16 *)(p + 0x2) -= 1) <= 0)', is also 64 words / 0 differing.
   Two statements kept, as in func_s01_80024280. */
void func_s31_80024024(u8 *p, UnkVec *v, UnkSlot *c)
{
    v->unk_00 += *(s32 *)(p + 0x58);
    v->unk_04 += *(s32 *)(p + 0x5C);
    v->unk_08 += *(s32 *)(p + 0x60);
    D_s31_80024D04 = 1;
    func_800478B8((u8 *)c);
    c->unk_1C += 0x80;
    c->unk_1E += 0x80;
    *(u16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((c->unk_14 & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The accelerated mover step for this bank's 0x212 flares: integrate the three
   +0x64..+0x6C accelerations into the +0x58..+0x60 velocities and those into
   the mover, raise the bank tick, bump the payload's +0x4 phase and refresh the
   renderable on even phases only, lift both scale halves by 0xC8 a frame, then
   the same two kill gates as func_s31_80024024 above.  83 words, 0 differing.
   1 member, 83 words (dngfam.py).  OWNS NO JUMP TABLE (jtcheck: 0 tables).

   Same donor as func_s31_80024024 above (csrc/ovl_dng_s01.c's
   func_s01_80024280) and it needed no new declaration: the tick symbol, the
   kill pair and the mover adds were already in this file.  Its two tail gates
   are the separate-`if' pair priced on func_s31_80024024 and are not re-priced
   here.

   ONE FACT, AND IT IS THE WHOLE OF THIS BODY'S FIGHT.  THE +0x4 PHASE IS READ
   INTO A LOCAL, THE TICK STORE SITS BETWEEN THE READ AND THE WRITE-BACK, AND
   THE GATE TESTS THE LOCAL.  Spelled `*(u16 *)(p + 0x4) += 1;' the frame is
   unreachable from ANY tick position: after the three mover adds 84 words / 12
   aligned differences, first in the function 85 / 6, after the three accel adds
   85 / 8, after the first mover add 84 / 12, after the second 84 / 14, and
   after the bump itself 86 words / 7 aligned (57 under ovlverify's
   word-against-word count, which is what three extra words do to it).  The
   mechanism is the load-delay slot behind `lhu $v0, 0x4($s0)': the target
   fills it with the tick store's
   `addiu $v1, $zero, 0x1', which can only happen when the tick's RTL lies
   BETWEEN the load and the increment, and a single `+= 1' statement leaves no
   such position.  The tick store then floats up the block and costs the third
   mover add its $v0/$v1 pair, which is where the 12 rows come from.  I read
   this as macro residue rather than a device -- a read-modify-write through a
   local with the gate on the local is ordinary C, and the tick store is
   plausibly the tail of a macro that wrapped the phase bump. */
void func_s31_80024124(u8 *p, UnkVec *v, UnkSlot *c)
{
    s32 t;

    *(s32 *)(p + 0x58) += *(s32 *)(p + 0x64);
    *(s32 *)(p + 0x5C) += *(s32 *)(p + 0x68);
    *(s32 *)(p + 0x60) += *(s32 *)(p + 0x6C);
    v->unk_00 += *(s32 *)(p + 0x58);
    v->unk_04 += *(s32 *)(p + 0x5C);
    v->unk_08 += *(s32 *)(p + 0x60);
    t = *(u16 *)(p + 0x4) + 1;
    D_s31_80024D04 = 1;
    *(s16 *)(p + 0x4) = t;
    if ((t & 0x1) == 0)
    {
        func_800478B8((u8 *)c);
    }
    c->unk_1C += 0xC8;
    c->unk_1E += 0xC8;
    *(u16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((c->unk_14 & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The summon bolt's own step, the one func_s31_800247D0 below installs on each
   0x12 node it spawns.  State 0 raises the bank tick, sheds one 0x212 mote a
   frame -- func_s31_80024124 as its step, D_800DE870 as its record, the mover
   jittered off the bolt's own and a `rand() - 0x20000' climb -- and spends one
   of the payload's +0x2 frames; when they run out it either advances to state 1
   and rings func_800A56E0(0x300), or, with +0x8 clear, kills the node.  Every
   frame of state 0 the mover integrates the +0x58..+0x60 velocity unless
   func_800A4778 says the destination tile is blocked, in which case the node
   dies.  State 1 counts +0x2 up and sheds, on the two live phases of
   `& 3', a 0x212 flare off the caster's own mover (phase 1, func_s31_80024124,
   D_800DE870, a doubled `rand() - 0x40000' climb) and a 0x212 mote (phase 2,
   func_s31_80024024, D_800DEC00, scales 0x800); at +0x2 past 0x29 it advances
   to state 2 and, when func_8009D218 lets it, rings func_800C8CD8 on the
   caster's target.  State 2 counts 0x15 frames and kills.  344 words,
   0 differing.  1 member, 344 words (dngfam.py).  OWNS NO JUMP TABLE (jtcheck:
   0 tables, both dispatches are `if' chains); relocations clean under
   relcand.py.  The third parameter is never read; it is declared to match the
   step signature func_8004491C installs and the other two steps in this unit.

   NO TWIN AT ANY RATIO -- LEADS.txt has nothing at 0.60 for this body and NEAR
   was empty, as the dispatch predicted.  THE DONOR IS dngcallee's SHARP ROW:
   7 of 15 symbols, ONE function, a clean gap to a three-function 6-of-15 tier,
   naming csrc/ovl_dng_s01.c's func_s01_800246DC -- a 392-word sibling step in
   the same shared library, 48 words larger and outside any length band a ratio
   could reach.  Its spawn skeleton (func_8003FC64(0x212) / +0x10 install /
   func_8004491C(n, D_80045340) / `s->unk_14 |= 0xC' / the jittered mover /
   func_8003DB94) was right on the FIRST COMPILE at 344 words and 58 aligned
   rows, and every row left was arithmetic or one wrong base.

   SIX FACTS, EACH PRICED BOTH WAYS FROM THIS BODY AT 0:

   * THE BOLT COUNT IS p[0x15], THE NODE PAYLOAD'S, NOT q[0x15].  Spelled
     q[0x15] it is 344 words / 49 aligned differences, and 45 of those are a
     WHOLE-BODY REGISTER RENAMING -- $s3/$s4 and $s6/$s7 exchanged -- because
     moving one reference off `p' onto `q' reorders allocno priority.  This is
     the brief's warning in its purest form: the residue reads as an allocation
     fault and the cause is a wrong base.  It was the entire difference between
     58 rows and 9.
   * THREE PLAIN SUMS MUST GO THROUGH LOCALS, because fold-const.c's
     `associate:' arm calls `split_tree (arg1, ...)' and rebuilds
     `A - ((rand() & 0x1F) + 0x10)' as `(A - 0x10) - (rand() & 0x1F)': the
     `addiu' lands on the loaded halfword where the target has it on the masked
     rand.  Written plainly the two jitter sites are 344 words / 17 rows.  A
     `(s16)' cast on the sum does NOT block it and neither does `(u16)' (both
     17) -- split_tree strips a same-mode conversion and convert_to_integer
     distributes a narrowing one into the operands -- nor does putting the
     constant first (17), nor hanging it on the load (17).  Only a VAR_DECL
     makes split_tree fail.  This is func_s46_80024C0C's FAULT 1 with the sign
     flipped: there the target wanted the constant on the load and the natural
     spelling put it on the rand.
   * AND EACH TEMP MUST BE ITS OWN LOCAL.  One local shared by all four sites is
     345 words / 12 differences: a pseudo live in three basic blocks is a global
     allocno and takes $a3, where every one of the target's is $v0.
   * THE func_800C8CD8 COUNT NEEDS TWO LOCALS, the masked roll and then the sum.
     With only the roll in a local the sum is a `simple' argument, expand_call
     defers it past the ternary, and it is 343 words / 11 rows with the `addu'
     in the jal's delay slot where the target leaves a `nop'.
   * `s' IS RE-READ OFF `n->unk_0C' BEFORE EACH DRESS BLOCK.  Carrying state 0's
     first `s' through is 343 words / 7 differences.  That is
     func_s46_80024C0C's "r IS ASSIGNED TWICE", reproduced here.
   * `q = *(u8 **)(p + 0x2C)' IS READ ONCE AT THE TOP, before the state
     dispatch.  Inline at its two uses it is 342 words / 13 -- the two missing
     words are $s7's save and restore.

   MEASURED AND FREE, so NEITHER is why this lands (both 344 / 0):
   * `s->unk_14 |= 0xC;' before `s->unk_10 = 0x60;'.  The listing's order --
     the 0x10 store sitting inside the flag raise -- is the `lhu 0x14' load
     delay, not source order, so func_s46_80024C0C's FAULT 2 (the 0x10 store
     must separate two flag raises or dead-store elimination eats the first)
     does NOT apply here: this body's two raises are a call apart.
   * `rand() + 0xFFFF8000' against `rand() - 0x8000' for the two velocities.
     Unlike func_s31_800247D0's climb below there is no division here, so the
     unsigned constant costs nothing; the signed form is kept for honesty. */
void func_s31_80024270(u8 *p, UnkVec *v, UnkSlot *c)
{
    UnkNode *n;
    u8 *w;
    UnkVec *m;
    UnkSlot *s;
    u8 *g;
    u8 *q;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t4;

    D_s31_80024D04 = 1;
    q = *(u8 **)(p + 0x2C);
    if (*(s16 *)(p + 0x0) == 0)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            w = (u8 *)n + 0x20;
            *(s16 *)(w + 0x2) = 0x1A;
            n->unk_10 = (u32)func_s31_80024124;
            func_8004491C(n, (u32)D_80045340);
            s = (UnkSlot *)n->unk_0C;
            s->unk_10 = 0x60;
            s->unk_14 |= 0xC;
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = (s16)(*(u16 *)((u8 *)v + 0x2) - 0x8) + (rand() & 0xF);
            *(s16 *)((u8 *)m + 0x6) = (s16)(*(u16 *)((u8 *)v + 0x6) - 0x8) + (rand() & 0xF);
            *(s16 *)((u8 *)m + 0xA) = (s16)(*(u16 *)((u8 *)v + 0xA) - 0x4) + (rand() & 0x7);
            *(s32 *)(w + 0x60) = rand() - 0x20000;
            s = (UnkSlot *)n->unk_0C;
            s->unk_1C = 0x1000;
            s->unk_1E = 0x1000;
            s->unk_0C[2] = 0x80;
            s->unk_0C[1] = 0x80;
            s->unk_0C[0] = 0x80;
            s->unk_12 = 0x7DCF;
            s->unk_14 |= 0x100;
            func_8003DB94(s, D_800DE870, 0);
        }
        *(u16 *)(p + 0x2) -= 1;
        if (*(s16 *)(p + 0x2) <= 0)
        {
            if (*(s16 *)(p + 0x8) != 0)
            {
                *(s16 *)(p + 0x0) = 1;
                *(s16 *)(p + 0x2) = 0;
                func_800A56E0(0x300);
            }
            else
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
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
        if ((*(s16 *)(p + 0x2) & 0x3) == 1)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                w = (u8 *)n + 0x20;
                *(s16 *)(w + 0x2) = 0x1A;
                n->unk_10 = (u32)func_s31_80024124;
                func_8004491C(n, (u32)D_80045340);
                s = (UnkSlot *)n->unk_0C;
                s->unk_10 = 0x60;
                s->unk_14 |= 0xC;
                m = n->unk_08;
                g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
                *(s16 *)((u8 *)m + 0x2) = (s16)(*(u16 *)(g + 0x2) - 0x10) + (rand() & 0x1F);
                *(s16 *)((u8 *)m + 0x6) = (s16)(*(u16 *)(g + 0x6) - 0x10) + (rand() & 0x1F);
                t1 = (rand() & 0x1F) + 0x10;
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(g + 0xA) - t1;
                *(s32 *)(w + 0x58) = rand() - 0x8000;
                *(s32 *)(w + 0x5C) = rand() - 0x8000;
                *(s32 *)(w + 0x60) = (rand() - 0x40000) << 1;
                s = (UnkSlot *)n->unk_0C;
                s->unk_1C = 0x1000;
                s->unk_1E = 0x1000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                s->unk_12 = 0x7DCF;
                s->unk_14 |= 0x100;
                func_8003DB94(s, D_800DE870, 0);
            }
        }
        if ((*(s16 *)(p + 0x2) & 0x3) == 2)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                w = (u8 *)n + 0x20;
                *(s16 *)(w + 0x2) = 0x14;
                n->unk_10 = (u32)func_s31_80024024;
                func_8004491C(n, (u32)D_80045340);
                s = (UnkSlot *)n->unk_0C;
                s->unk_10 = 0;
                s->unk_14 |= 0xC;
                m = n->unk_08;
                g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
                *(s16 *)((u8 *)m + 0x2) = (s16)(*(u16 *)(g + 0x2) - 0x10) + (rand() & 0x1F);
                *(s16 *)((u8 *)m + 0x6) = (s16)(*(u16 *)(g + 0x6) - 0x10) + (rand() & 0x1F);
                t2 = (rand() & 0x3F) + 0x20;
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)(g + 0xA) - t2;
                s = (UnkSlot *)n->unk_0C;
                s->unk_1C = 0x800;
                s->unk_1E = 0x800;
                *(s32 *)(w + 0x60) = rand() - 0x20000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                s->unk_12 = 0x7DC0;
                s->unk_14 |= 0x100;
                func_8003DB94(s, D_800DEC00, 0);
            }
        }
        if (*(s16 *)(p + 0x2) >= 0x29)
        {
            *(s16 *)(p + 0x0) = 2;
            *(s16 *)(p + 0x2) = 0;
            if (func_8009D218(*(u8 **)(q + 0x60), 1, q) == 0)
            {
                t3 = (func_800A6D30() & 0x3) + 2;
                t4 = (p[0x15] >> 2) + t3;
                func_800C8CD8(*(u8 **)(p + 0x30), D_800E3D68 == 0xFF ? 0xFF : 0x10, t4);
            }
        }
    }
    if (*(s16 *)(p + 0x0) == 2)
    {
        *(u16 *)(p + 0x2) += 1;
        if (*(s16 *)(p + 0x2) >= 0x15)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* States 0..3 of this bank's summon beam.  `p' is this node's payload, `v' its
   mover, `s' its renderable; `r = *(u8 **)p' is the CASTER payload,
   `g = (UnkNode *)(r - 0x20)' the caster node and `m = g->unk_08' the caster
   mover.  0 dresses white, binds D_800DEC00 and latches the caster's facing out
   of its +0x2A; 1 publishes the caster's hand through func_8003DF74 and, on the
   view's +0x80, advances; 2 spawns ONE 0x12 bolt running func_s31_80024270
   above -- its velocity pair read out of the D_s31_80024004 direction table at
   that facing, its tile distance twice the caster-to-target axis delta, its
   climb divided over that distance -- parks the node and its mover at +0xA8 and
   +0xAC and advances; 3 follows that bolt until +0x84 passes the +0x86 deadline
   and then kills, one frame late, through the bank tick.  333 words,
   0 differing.  1 member, 333 words (dngfam.py).  OWNS NO JUMP TABLE (jtcheck:
   0 tables): the switch on *(s16 *)(p + 0xA) is a compare chain and this unit
   has only `data' subsegments.  D_s31_80024004 is the eight (x, y) pairs splat
   emits as asm/data/18E8800.data.s -- declared extern here and never defined in
   C.  Relocations hand-checked and clean under relcand.py.

   TWIN: the landed func_s01_80024CFC (csrc/ovl_dng_s01.c, 399 words, dngtwin
   0.902), which itself landed out of docs/dng-parks/func_s01_80024CFC.c.  Both
   tools agree on it -- it is also dngcallee's sharp row, 8 of 10 symbols and one
   function.  This copy is 66 words SHORTER and every deletion is STATEMENTS,
   not references, as the brief's rule predicts: state 1 loses the
   func_800A56E0(0x300) ring and the three +0xB0..+0xB8 publish stores, state 2
   loses the +0x84 parity gate, the `== 9' deadline guard and the `== 1' guard
   on the division, and the follow pair lives at +0xA8/+0xAC rather than
   +0x90/+0x94.  Prologues agree -- seven callee-saved saves in the same order
   -- so the twin's ALLOCATION facts were taken as well as its shape facts, and
   all five re-priced here:

   | spelling                                              | s01 | here |
   |-------------------------------------------------------|-----|------|
   | the tile distance stored through `volatile'           |  36 |   9  |
   | the +0x84 counter as a plain cast, not ((u16 *)p)[]   |   3 |   3  |
   | the +0x7E facing computed inline instead of via `j'   |   4 |   4  |
   | case 2's slot scales written +0x1C before +0x1E       |   2 |   2  |
   | the climb as `+ 0xFFE00000' (unsigned, so `divu')     |   1 |   1  |

   FOUR OF THE FIVE REPRODUCE s01's FIGURE TO THE DIGIT -- all four are
   SELECTION facts, where the brief says no inversion has ever been seen.  The
   `volatile' is the outlier at 9 rows here against 36 there, and it is also the
   only one that moves a word: without it cse records the abs pseudo, substitutes
   it for the reload and sign-extends with `sll 16'/`sra 16' where the target has
   `lh $a1, 0x2($a0)', for 334 words / 9.  It is a DEVICE, not macro residue --
   nothing about that field is volatile -- and it is s01's device, kept because
   no plain-C spelling blocks the forwarding with both ends off `w'.

   ONE FACT THIS BODY ADDS: CASE 0's OWN SCALE HALVES ARE WRITTEN +0x1E BEFORE
   +0x1C, THE OPPOSITE ORDER FROM CASE 2's.  1C-first in case 0 is 333 words / 4
   differences; 1C-first in case 2 is 333 / 2.  Both halves are 0x1000 at both
   sites, so only the order distinguishes them, and the two sites disagree --
   there is no house order to infer.

   One reading note: the else arm's `*(s16 *)(p + 0x86) = *(u16 *)(w + 0x2) +
   0x1E' is why the listing shows `addiu $v0, $v0, 0x1E' on an `li 0x20' instead
   of a plain `li 0x3E'.  cse forwards that store because the value is a
   constant; the then arm's `+ 0x3C' is a real reload for the same reason the
   `volatile' above exists. */
void func_s31_800247D0(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 sp10[3];
    UnkDngS31Dirs dd;
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
    dd = *(UnkDngS31Dirs *)D_s31_80024004;
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
        D_s31_80024D04 = 1;
        *(s16 *)(p + 0x7E) = j;
        *(u16 *)(p + 0xA) += 1;
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)g->unk_0C)->unk_08, (UnkSlot *)g->unk_0C, sp10, 0) !=
                0 ||
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
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                *(s16 *)(p + 0x84) = 0;
                *(u16 *)(p + 0xA) += 1;
            }
        }
        break;
    case 2:
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
            p[0xA0] = u[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA1] = u[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            if (*(s32 *)(r + 0x60) != 0)
            {
                *(s16 *)(w + 0x8) = 1;
                *(volatile s16 *)(w + 0x2) =
                    __builtin_abs(*(s8 *)(r + 0x72) == u[0x24] ? *(s8 *)(r + 0x73) - u[0x25]
                                                               : *(s8 *)(r + 0x72) - u[0x24]) *
                    2;
                hh = *(u8 **)(*(s32 *)(r + 0x60) - 0x18);
                if (*(s16 *)(w + 0x2) != 0)
                {
                    *(s32 *)(w + 0x60) =
                        (*(s32 *)(hh + 0x8) - v->unk_08 - 0x200000) / (*(s16 *)(w + 0x2) - 1);
                }
                *(s16 *)(p + 0x86) = *(u16 *)(w + 0x2) + 0x3C;
            }
            else
            {
                *(s16 *)(w + 0x2) = 0x20;
                *(s16 *)(w + 0x8) = 0;
                *(s16 *)(p + 0x86) = *(u16 *)(w + 0x2) + 0x1E;
            }
            n->unk_10 = (u32)func_s31_80024270;
            h = (u8 *)n->unk_08;
            *(s32 *)(h + 0x0) = v->unk_00;
            *(s32 *)(h + 0x4) = v->unk_04;
            *(s32 *)(h + 0x8) = v->unk_08;
            k = (UnkSlot *)n->unk_0C;
            k->unk_0C[2] = 0x80;
            k->unk_0C[1] = 0x80;
            k->unk_0C[0] = 0x80;
            k->unk_1E = 0x1000;
            k->unk_1C = 0x1000;
            *(u32 *)(p + 0xA8) = (u32)n;
            *(u32 *)(p + 0xAC) = (u32)h;
            *(s16 *)(p + 0x88) = 0x63;
        }
        *(u16 *)(p + 0xA) += 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0xA8);
            h = *(u8 **)(p + 0xAC);
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
            if (D_s31_80024D04 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_s31_80024D04 = 0;
            }
        }
        break;
    }
}
