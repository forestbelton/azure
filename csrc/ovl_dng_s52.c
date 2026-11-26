#include "common.h"

void func_800478B8(u8 *);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_80047784(u8 *, s32, s32);
s32 func_80053EF0(s32);
s32 func_800A56E0(s32);
void func_s52_80024374(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s52_80024A88(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];
extern u8 D_80083498[];
extern u16 D_80082E94;
extern s16 D_80083228;
s32 func_800644B8(s32);
s32 rand(void);
extern u8 D_80082E80[];
extern u8 D_80083780[];
extern u8 D_800DE870[];
extern u8 D_800DEA68[];

/* The bank's FOUR-STATE cutscene dispatcher, a COMPARE CHAIN on
   *(s16 *)(p + 0xA) over cases 0..3 -- no .rodata and no jump table, this
   function owns none. State 0 clears D_800814A8's +0xF4, publishes the party
   record's position triple into b and FALLS THROUGH into state 1, which waits
   for 0x80 in the record at p + 0x4, decrements D_800814A8's +0xA6 counter,
   parks p[8] at its +0xA8 and spawns the 0x312 node that runs
   func_s52_80024374 with D_80045340 as its renderable -- the node's payload
   cloned out of the party member's own record at s - 0x14 (+0x28, +0x14 masked
   with 0x97FF, +0x2C) and dressed through func_80047784 off the facing index,
   with the position offset by func_8003DE58's projection -- then fires
   func_800A56E0 with 0x4300 or 0x300 off func_80053EF0(4). State 2 waits out
   the +0x50 timer and rearms it to 8. State 3 tears the actor down once
   D_80082E94 carries 0x8000 or the timer goes negative. 220 words,
   0 differing, FIRST COMPILE.

   DONOR: csrc/ovl_dng_s61.c's landed func_s61_8002401C, which dngcallee named
   at 11 of 13 symbols with a clean gap to the next tier (5 functions at
   10/13); csrc/ovl_dng_s68.c's func_s68_80024020 is the other 11/13 row and
   its header carries the richer fact table. dngtwin found NOTHING at 0.70 for
   this stub, so dngcallee is the only tool that reached the family -- the
   ninth unit running where that is true. s61's case 0, case 1 skeleton, case 2
   and case 5 teardown are this body's text; the deltas are that the whole
   func_80053EF0/func_800A56E0 pair lives INSIDE state 1 here rather than being
   state 2, state 2 only rearms the timer to 8, the node payload is cloned from
   s - 0x14 instead of written from D_800DEDB0, and +0x12 and +0xC get a
   -0x80 bump and 0x80FF that no donor has.

   THREE FACTS PRICED FROM THIS BODY AT 220 WORDS / 0 DIFFERING. The first is
   an INVERSION of both donors and it is the one that would have cost a run:

   * `*(u8 **)((u8 *)n + 0x20) = p;' MUST COME BEFORE `func_8004491C(n, ...)',
     where s61 and s68 BOTH put it last in the spawn block. At the donors'
     position it is 220 words / 5 ALIGNED ROWS: the store is what fills
     func_8004491C's delay slot here, so moving it past the call leaves the
     slot to `sw $v0, 0x10($s1)' and displaces the func_80053EF0 call too.
     A store after a CALL_INSN cannot be scheduled back across it -- sched
     adds a dependence on all memory at a call -- so this is readable off the
     listing before compiling: the store is IN the delay slot, therefore it
     was before the call in the insn stream.
   * THE POSITION CHAIN'S DIRECTION IS LOAD-BEARING, 8 ALIGNED ROWS. It must be
     `*(s16 *)((u8 *)n->unk_08 + 0x2) = *(s16 *)(e + 0xC) = <w's record>', with
     the e + 0xC store INNERMOST so it is emitted first; the other way round
     all three `sh ...($a1)' slide one slot later and take the func_80053EF0
     call with them. C evaluates `a = b = v' as `a = (b = v)', so the inner
     lvalue is the one that stores first, and the listing shows the e stores
     first -- read the chain direction off the store order, do not guess it.
   * THE TEARDOWN'S TWO STORES ARE FREE IN EITHER ORDER -- `*(s32 *)(z + 0xC)
     = 0' before or after `*(s16 *)(z + 0xA) = *(u16 *)(z + 0xA) - 1' is
     220 words / 0 differing both ways, so the emitted code is byte-identical
     and sched absorbs it. FREE, not NOT-MEASURABLE: both spellings are inside
     this function's own words. Worth recording because s61 and s68 DISAGREE
     with each other on this order and neither prices it -- the fact does not
     exist.

   D_800814A8 is dereferenced with no pointer local in case 0 (one use) and
   with one, `h', in case 1 (three uses), which is what the listing shows;
   s68's comment prices the separate-local-per-arm question at 3 and 5 rows on
   a body with two arms that each use it, and that condition does not hold
   here. func_s52_80024374 is this unit's own state function, forward
   declared. */
void func_s52_80024004(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *w;
    u8 *h;
    u8 *d;
    u8 *t;
    u8 *q;
    u8 *e;
    u8 *z;
    UnkNode *n;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    w = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)(w + 0x8) + 0x0);
        *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)(w + 0x8) + 0x4);
        *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)(w + 0x8) + 0x8);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        h = (u8 *)D_800814A8;
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(h + 0xA6) = *(u16 *)(h + 0xA6) - 1;
        *(u8 *)(h + 0xA8) = *(u8 *)(p + 0x8);
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out,
                              0) == 0)
            {
                out[0] = out[1] = out[2] = 0;
            }
            n->unk_10 = (u32)func_s52_80024374;
            *(u8 **)((u8 *)n + 0x20) = p;
            func_8004491C(n, (u32)D_80045340);
            d = *(u8 **)(s - 0x14);
            q = (u8 *)n->unk_0C;
            *(s32 *)(q + 0x28) = *(s32 *)(d + 0x28);
            *(s16 *)(q + 0x14) = *(u16 *)(d + 0x14) & 0x97FF;
            t = *(u8 **)(d + 0x2C);
            *(u8 **)(q + 0x2C) = t;
            func_80047784(q, t[((D_80083228 + *(s16 *)(s + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
            *(s32 *)(q + 0xC) = 0x80FF;
            *(s16 *)(q + 0x10) = 0x20;
            *(s16 *)(q + 0x12) = *(u16 *)(q + 0x12) - 0x80;
            *(u16 *)(q + 0x14) |= 0xC;
            e = (u8 *)n + 0x20;
            *(s32 *)(e + 0x1C) = *(s32 *)(*(u8 **)(w + 0x8) + 0x0) + (out[0] << 16);
            *(s32 *)(e + 0x20) = *(s32 *)(*(u8 **)(w + 0x8) + 0x4) + (out[1] << 16);
            *(s32 *)(e + 0x24) = *(s32 *)(*(u8 **)(w + 0x8) + 0x8) + (out[2] << 16);
            *(s16 *)((u8 *)n->unk_08 + 0x2) = *(s16 *)(e + 0xC) =
                *(u16 *)(*(u8 **)(w + 0x8) + 0x2);
            *(s16 *)((u8 *)n->unk_08 + 0x6) = *(s16 *)(e + 0xE) =
                *(u16 *)(*(u8 **)(w + 0x8) + 0x6);
            *(s16 *)((u8 *)n->unk_08 + 0xA) = *(s16 *)(e + 0x10) =
                *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
            if (func_80053EF0(4) != 2)
            {
                func_800A56E0(0x300);
            }
            else
            {
                func_800A56E0(0x4300);
            }
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        *(s16 *)(p + 0x50) = 0x8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if ((D_80082E94 & 0x8000) != 0 || *(s16 *)(p + 0x50) < 0)
        {
            if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
            {
                *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
            }
            else
            {
                z = (u8 *)&D_80083460;
                *(s16 *)(z + 0xA) = *(u16 *)(z + 0xA) - 1;
                *(s32 *)(z + 0xC) = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}

/* The step of the 0x312 node func_s52_80024004 spawns in its state 1, so the
   signature is fixed by that call site. Bumps the +0x48 frame counter, sets
   bit 0x8000 in the +0x52 halfword of the record at the actor's +0x0, hands
   the renderable to func_800478B8, then runs a FOUR-CASE COMPARE CHAIN on
   *(s16 *)(p + 0x4C) -- no .rodata, no jump table, this function owns none,
   and the chain's default falls into the common tail rather than returning.
   State 0 just advances. State 1 wraps the renderable's +0x1C/+0x1E spin past
   0x140, drifts the +0x1C/+0x20/+0x24 position triple a 0x4000th of the way
   toward the party record D_80083780 offset by func_8003DE58's projection of
   D_80082E80's slot, pulls +0x24 down by func_800644B8 of the counter's
   eleventh, republishes the triple into b and halves the renderable's
   +0xC/+0xD alphas -- or, when the projection fails, eases b half way to the
   actor instead -- and advances at counter 0xA. State 2 is the same drift at
   0x8000 with the spin wrapped past 0xA00 or reset to 0x800, advancing at
   0xC. State 3 drifts once more, and once D_800814A8's +0x60 escort exists and
   the counter has reached 0x19 it spawns FOUR D_800DE870 nodes scattered
   +-0x3F around the actor, then tears the actor down at 0x1C. The tail spawns
   `k + 4 * g' further D_800DEA68 nodes, where g is the counter's 0x12..0x15
   window, each one's position jittered by rand() at a shift the state picks.
   453 words, 0 differing.

   DONOR: csrc/ovl_dng_s70.c's landed func_s70_8002466C (280 words), the single
   dngcallee row at 11 of 14 symbols with a clean gap to the next tier (2
   functions at 10/14). dngtwin found NOTHING at 0.70 for this stub, and the
   frames are far apart -- s70 is -0x38 with 7 saves, this is -0x48 with TEN
   (s0-s7, fp, ra) and one spilled pseudo -- so by the three-kinds taxonomy
   every budget fact of s70's was assumed void here and none was used. What
   carried is the SHAPE: the head pair, `c = -1' before the switch, the
   per-arm D_80082E80/D_80083780 pointer locals, the drift triple, the
   (counter << 11) / 10 division written inline, the spawn loop as
   `for (i = ...; i >= 0; i--)' and the whole dress block. s70's `volatile' on
   a +0x4C payload store has no site here -- this body does not write +0x4C.

   FIVE FACTS, all priced from this body at 453 words / 0 differing:

   * `k = -1;' MUST BE THE FIRST STATEMENT, ahead of the +0x48 bump and the
     +0x52 flag. After them, as the donor writes it, is 453 words / 4 ALIGNED
     ROWS -- the `addiu $s6, $zero, -0x1' leaves the PROLOGUE (where the target
     has it, between two register saves) and lands at word 20. The donor's
     order is harmless on the donor because its `c = -1' has nothing to be
     hoisted past; here the earliest LUID is what puts it in the prologue.
   * THE +0x14 JITTER GATE'S ARMS MUST BE WRITTEN `!= 3 -> store 0' FIRST,
     `else -> -0x80000 - (rand() << 3)'. The natural `== 3' order is 454 words
     -- ONE WORD LONG -- and 12 ALIGNED ROWS, because the arm containing the
     call cannot be the fall-through without an extra jump. gcc emits the THEN
     arm as the fall-through, so read which arm the listing puts immediately
     after the branch and make that one the `then'.
   * IN THE TAIL LOOP `*(u16 *)(q + 0x14) |= 0xC;' MUST COME BEFORE
     `*(u8 **)(q + 0x0) = D_800DEA68;'. The donor's order is 453 words /
     10 ALIGNED ROWS: with the symbol's lui/addiu ahead of the read-modify-
     write, three values are live at once and the symbol is pushed out of
     $v0/$v1 into $t0, while the target interleaves `sh $v0, 0x10($s1)' and
     `lhu $v0, 0x14($s1)' between the lui and the addiu. This is the same
     sentence func_s68_80024020's header prices at 10 rows in the opposite
     direction, and the residue here was exactly 10 -- a donor's rejected
     price used as a lookup table for a residue, which worked.
   * AND THE SAME TWO STATEMENTS ARE FREE 40 WORDS AWAY, in state 3's own spawn
     loop: either order there is 453 words / 0 differing, byte-identical. So
     one body wants the order fixed at one site and does not care at the other,
     and the discriminator is the pressure -- the tail loop also carries
     $s5, $s6, $s7 and $fp live across it where state 3's loop carries only
     $s3 and $s5. Do not pick a house style for this pair; price it per site.
   * `m = 0xA; if (*(s16 *)(p + 0x4C) != 3) { m = 0xC; }' is right and
     `m = *(s16 *)(p + 0x4C) == 3 ? 0xA : 0xC' is 453 words / 7 ALIGNED ROWS --
     five `li $s5' sites all materialising 0xC where the target has 0xA.
     `m = *(s16 *)(p + 0x4C) != 3 ? 0xC : 0xA' is 0 differing, i.e.
     byte-identical to the kept form. See the lever note below; the guard is
     jump.c:789.

   ALSO PRICED: the `?:' feeding `*(s16 *)(q + 0x1E)' is NOT interchangeable
   with an if/else here -- two `if' arms storing DIFFERENT values to one
   address is 455 words, TWO WORDS LONG, and 10 ALIGNED ROWS, because jump2 has
   nothing to cross-join. The levers file's `a ?: is not a neutral spelling'
   entry is about two bare sets of ONE PSEUDO and does not reach this case.

   NOT RE-PRICED, kept because the body reaches 0 with them: the separate
   per-arm pointer locals for D_80082E80 and D_80083780 (three arms, so r/t,
   r2/t2, r3/t3), the `(s16) *(u16 *)(p + 0x48) >> 1' cast idiom in state 1's
   count, state 1's spin clamp as a bare `if' against state 2's `if/else',
   g written as the single `&& ' range test (the listing's
   `addiu -0x12' / `sltiu 0x4' is fold's unsigned range check), and
   `rand() % 96' left as a modulus (the listing's 0x2AAAAAAB multiply is
   expand_divmod's, not source). rand is the listing's func_80069EF8 out of
   splat/common_syms.txt and is declared in this unit's block because
   common.h does not declare it. */
void func_s52_80024374(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *q;
    u8 *r;
    u8 *t;
    u8 *r2;
    u8 *t2;
    u8 *r3;
    u8 *t3;
    UnkNode *n;
    s32 k;
    s32 m;
    s32 g;
    s32 i;
    s16 out[4];

    k = -1;
    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        break;
    case 1:
        k = ((s16) * (u16 *)(p + 0x48) >> 1) + 5;
        if (*(u16 *)(c + 0x1E) >= 0x140)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x140;
        }
        r = D_80082E80;
        if (func_8003DE58(*(u8 **)(r + 0x8), (UnkSlot *)r, out, 0) != 0)
        {
            t = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t + 0x2) - *(s16 *)(p + 0x1E)) << 14;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t + 0x6) - *(s16 *)(p + 0x22)) << 14;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t + 0xA) - *(s16 *)(p + 0x26)) << 14;
            *(s32 *)(p + 0x24) -= func_800644B8((*(s16 *)(p + 0x48) << 11) / 10) << 9;
            *(s32 *)((u8 *)b + 0x0) = *(s32 *)(p + 0x1C);
            *(s32 *)((u8 *)b + 0x4) = *(s32 *)(p + 0x20);
            *(s32 *)((u8 *)b + 0x8) = *(s32 *)(p + 0x24);
            *(u8 *)(c + 0xC) -= *(u8 *)(c + 0xC) >> 1;
            *(u8 *)(c + 0xD) -= *(u8 *)(c + 0xD) >> 1;
        }
        else
        {
            *(s32 *)((u8 *)b + 0x0) += (*(s32 *)(p + 0x1C) - *(s32 *)((u8 *)b + 0x0)) >> 1;
            *(s32 *)((u8 *)b + 0x4) += (*(s32 *)(p + 0x20) - *(s32 *)((u8 *)b + 0x4)) >> 1;
            *(s32 *)((u8 *)b + 0x8) += (*(s32 *)(p + 0x24) - *(s32 *)((u8 *)b + 0x8)) >> 1;
        }
        if (*(s16 *)(p + 0x48) >= 0xA)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if (*(u16 *)(c + 0x1E) >= 0xA00)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x200;
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x800;
        }
        k = 9;
        r2 = D_80082E80;
        if (func_8003DE58(*(u8 **)(r2 + 0x8), (UnkSlot *)r2, out, 0) != 0)
        {
            t2 = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t2 + 0x2) - *(s16 *)(p + 0x1E)) << 15;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t2 + 0x6) - *(s16 *)(p + 0x22)) << 15;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t2 + 0xA) - *(s16 *)(p + 0x26)) << 15;
            *(u8 *)(c + 0xC) -= *(u8 *)(c + 0xC) >> 1;
            *(u8 *)(c + 0xD) -= *(u8 *)(c + 0xD) >> 1;
        }
        if (*(s16 *)(p + 0x48) >= 0xC)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 3:
        k = 0xA;
        r3 = D_80082E80;
        if (func_8003DE58(*(u8 **)(r3 + 0x8), (UnkSlot *)r3, out, 0) != 0)
        {
            t3 = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t3 + 0x2) - *(s16 *)(p + 0x1E)) << 15;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t3 + 0x6) - *(s16 *)(p + 0x22)) << 15;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t3 + 0xA) - *(s16 *)(p + 0x26)) << 15;
        }
        if (*(s32 *)(D_800814A8 + 0x60) != 0 && *(s16 *)(p + 0x48) >= 0x19)
        {
            for (i = 3; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s52_80024A88;
                    func_8004491C(n, (u32)D_80045340);
                    q = (u8 *)n->unk_0C;
                    *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(p + 0x1E) + (rand() & 0x3F) - 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(p + 0x22) + (rand() & 0x3F) - 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(p + 0x26) - rand() % 96;
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                    *(s16 *)(q + 0x10) = 0x60;
                    *(u8 **)(q + 0x0) = D_800DE870;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DE870 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x808080;
                }
            }
        }
        if (*(s16 *)(p + 0x48) >= 0x1C)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    m = 0xA;
    if (*(s16 *)(p + 0x4C) != 3)
    {
        m = 0xC;
    }
    g = (*(u16 *)(p + 0x48) >= 0x12 && *(u16 *)(p + 0x48) < 0x16);
    for (i = k + g * 4; i >= 0; i--)
    {
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s52_80024A88;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
            *(s32 *)((u8 *)n->unk_08 + 0x0) =
                *(s32 *)(p + 0x1C) + (((rand() & 0x3FF) - 0x1FF) << (m + g));
            *(s32 *)((u8 *)n->unk_08 + 0x4) =
                *(s32 *)(p + 0x20) + (((rand() & 0x3FF) - 0x1FF) << (m + g));
            *(s32 *)((u8 *)n->unk_08 + 0x8) =
                *(s32 *)(p + 0x24) +
                (((rand() & 0x3FF) - 0x1FF) << (*(s16 *)(p + 0x4C) == 3 ? m + 1 : m - 1));
            if (*(s16 *)(p + 0x4C) != 3)
            {
                *(s32 *)((u8 *)n->unk_08 + 0x14) = 0;
            }
            else
            {
                *(s32 *)((u8 *)n->unk_08 + 0x14) = -0x80000 - (rand() << 3);
            }
            *(s16 *)(q + 0x1C) = (g << 10) + 0x1000;
            *(s16 *)(q + 0x1E) = *(s16 *)(p + 0x4C) == 3 ? (g << 10) + 0x15E0 : 0x1000;
            *(s16 *)(q + 0x10) = 0x60;
            *(u16 *)(q + 0x14) |= 0xC;
            *(u8 **)(q + 0x0) = D_800DEA68;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0x4040C0;
        }
    }
}

/* The particle step func_s52_80024374 installs on every D_800DE870 and
   D_800DEA68 node it spawns, so the signature is fixed by those two call
   sites. Advances the mover by its own +0x08/+0x14 velocity word, sets bit
   0x8000 in the +0x52 halfword of the record at the actor's +0x0, hands the
   renderable to func_800478B8, and when its +0x14 reports 0x6000 marks the
   owning actor retired and sets the same bit in D_800814A0. 35 words,
   0 differing, FIRST COMPILE.

   TWIN: the landed func_s70_80024ACC in csrc/ovl_dng_s70.c (dngtwin 0.879,
   31 words; func_s63_80024ACC is identical at the same ratio and width).
   The twin is this body minus its first statement, and the four extra words
   are exactly that statement's lw/lw/addu/sw. The twin's note about
   D_800814A0 being the PLAIN scalar rather than ovl_main_8008A288's array
   alias holds here too and is what gives the lui/lw %lo pair.

   ONE FACT, priced from this body at 35 words / 0 differing:

   * `b->unk_08 += b->unk_14;' MUST BE THE FIRST STATEMENT, ahead of the +0x52
     flag. One statement later it is 36 words -- ONE WORD LONG -- and 17
     ALIGNED ROWS: the `move $s1, $a0' and `move $s0, $a2' parameter copies
     both shift, `lw $v1, 0x0($s1)' gains a `nop' behind it, and the whole
     head slides. The two statements touch different memory and neither
     depends on the other, so this is a pure LUID question and the position is
     not readable off the listing -- it had to be measured. 17 rows for one
     statement's position on a 35-word body is the largest price-per-word
     figure in this unit.

   b is UnkVec and the two fields are its declared s32 unk_08 and unk_14, so
   no cast is needed; the other two bodies in this unit reach b through
   `*(s32 *)((u8 *)b + N)' because they also write offsets UnkVec does not
   name. func_800478B8 is declared in this unit's block. */
void func_s52_80024A88(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    b->unk_08 += b->unk_14;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
