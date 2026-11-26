#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
void func_s09_80024C84(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80045340[];

s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A3820(s16);
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
void func_s09_8002472C(u8 *p);
s32 func_s09_80024868(u8 *p);
extern s32 D_8008346C;
extern u8 D_80083498[];
extern u8 D_800DEA68[];
extern u8 D_800DECF8[];

/* The bank's six-state cutscene director and the relink of ovl_dng_s12's
   func_s12_80024020: a JUMP TABLE on *(s16 *)(p + 0xA) over cases 0..5
   (jtbl_s09_80024008, 6 words = 6 cases + 0 pad, dispatch bound 6, 6 distinct
   targets). THIS FUNCTION OWNS THAT TABLE; the arms are written ascending and
   a standalone `tools/cdkgcc -S' emits the six entries in ascending
   entry-ADDRESS order, which is the listing's order. The range check's `beqz'
   lands on the epilogue, which is not one of the six entries, so no
   `default:' is written.

   Before the switch, every state but 0 and 5 spawns THIRTEEN 0x312 smoke
   nodes running func_s09_80024C84, jittered +-0xFF<<13 / <<10 around the
   caller's vector and dressed from D_800DEA68. State 0 registers the party
   record through func_800A05A4, publishes its projected position into b,
   seeds the +0x50 timer at 8 and divides the three velocity words by it --
   or, when the record and the third parameter both carry 0x8000 at +0x14,
   calls func_8009CE1C and goes straight to state 5. State 1 advances b and
   reloads the timer at 0xC; state 2 either retires to state 5 or reloads at
   0xA; state 3 spawns the single 0x201 node that gets func_s09_8002472C as
   its step and func_s09_80024868 as its renderer, then twenty-one more 0x312
   nodes from D_800DECF8; state 4 runs the timer down; state 5 tears the actor
   down. 451 words, 0 differing.

   NO TWIN AT ANY RATIO -- dngtwin reports nothing at 0.70 for any stub in
   this unit. `dngcallee' named func_s12_80024020 at 10 of 14 symbols with a
   clean gap to the 9/14 tier, and that donor's skeleton, local set and
   declaration block are what this body is built from. THE DONOR'S FRAME DOES
   NOT MATCH: it is -0x50 with ten saves where this is -0x58 with the same
   ten, the extra eight bytes being `w' spilled to 0x28($sp).
   func_80069EF8 is rand out of splat/common_syms.txt.

   FOUR FACTS, all RE-PRICED from the finished body -- the first three were
   first measured mid-fight and two of those readings were wrong:

   * EACH SPAWN LOOP MUST NAME ITS DRESS SYMBOL DIRECTLY, NOT RE-READ IT
     THROUGH THE PAYLOAD. `*(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4)' is
     0; the donor's `*(s32 *)(*(u8 **)(q + 0x0) + 0x4)' -- right there,
     because its two dress arms load different symbols -- is 446 words / 69
     aligned differences HERE, with a whole-body $s0/$s1/$s2 rotation. Naming
     the symbol twice makes it a loop invariant with two references, which
     loop.c hoists into $fp (and $s3 in the second loop) in the preheader;
     read back through the payload it is an in-loop constant plus a register
     copy -- five words and one callee-saved register short. Largest fact on
     this body.
   * THE DRESS BLOCK'S +0xC COLOUR STORE GOES LAST, after the two zero bytes.
     `|= 0xC; +0x8; +0x4; +0x5; +0xC' is 0 and the donor's `+0xC; |= 0xC;
     +0x8; +0x4; +0x5' is 4 aligned differences in the first loop alone: the
     0x404040 `ori' then has nothing left to schedule into the +0x8 load's
     delay slot and the body gains a `nop'.
   * THE 0x201 PAYLOAD POINTER NEEDS ITS OWN LOCAL, ASSIGNED AFTER THE
     func_8004491C CALL -- 7 aligned differences otherwise. Sharing the 0x312
     loops' `e' puts it in a callee-saved register ($s2) where the target
     spends $v1, and assigning a separate local BEFORE the call does the same
     thing, because the range then crosses the call. The register, not the
     local count, is what this buys.
   * THE 0xB0 JITTER CONSTANT NEEDS A LOCAL. `j = (rand() & 0x3F) - 0xB0;'
     then `*(u16 *)((u8 *)b + 0xA) - j' is 0; the expression written out is 2
     aligned differences, because fold's PLUS/MINUS reassociation lifts the
     constant out of `x - (y - 0xB0)' and emits `addiu $v1, 0xB0' before the
     `subu' where the target has `addiu $v0, -0xB0' after the `andi'. A
     VAR_DECL is the only spelling fold cannot see through -- moving the
     constant to the other addend or to the far side of the subtraction
     reassociates identically.

   RETRACTED: `e = (u8 *)n + 0x20' hoisted ABOVE the `if (n != NULL)' read 30
   aligned differences better at an intermediate state and is MEASURED FREE
   from the finished body, in both loops, at 451/0. It was compensating for
   the +0xC store order above -- the two faults were adding. The natural form,
   inside the `if', is what is kept. */
void func_s09_80024020(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *t;
    s32 i;
    s32 j;
    s32 r;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    if (*(s16 *)(p + 0xA) != 0 && *(s16 *)(p + 0xA) < 5)
    {
        for (i = 0xC; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                e = (u8 *)n + 0x20;
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s09_80024C84;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x1FF) - 0xFF) << 10);
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                *(s16 *)(q + 0x10) = 0x40;
                *(u8 **)(q + 0x0) = D_800DEA68;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0x404040;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4A) = 0x8;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
    }
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        r = func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25), *(s16 *)(s + 0x2A),
                          (s16)func_800A3820(9));
        *(s32 *)(s + 0x60) = r;
        if (r == 0)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)((u8 *)r - 0x14);
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 4, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
                *(s16 *)(p + 0xA) = 5;
                break;
            }
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out, 0) ==
            0)
        {
            out[0] = out[1] = out[2] = 0;
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2) + out[0];
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6) + out[1];
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA) + out[2];
        *(s16 *)(p + 0x50) = 8;
        *(s16 *)((u8 *)b + 0xE) = (*(s8 *)(s + 0x72) * 0x40) - (*(u16 *)((u8 *)b + 0x2) - 0x20);
        b->unk_0C /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x12) = (*(s8 *)(s + 0x73) * 0x40) - (*(u16 *)((u8 *)b + 0x6) - 0x20);
        b->unk_10 /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x16) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) -
            (*(u16 *)((u8 *)b + 0xA) + 0xB0);
        b->unk_14 /= *(s16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 0xC;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) == 0)
            {
                *(s16 *)(p + 0x50) = 8;
                *(s16 *)(p + 0xA) = 5;
            }
            else
            {
                *(s16 *)(p + 0x50) = 0xA;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 3:
        if ((*(u16 *)(p + 0x52) & 0x7FFF) == 0)
        {
            n = func_8003FD64(0x201, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s09_8002472C;
                func_8004491C(n, (u32)func_s09_80024868);
                t = (u8 *)n + 0x20;
                *(s16 *)(t + 0x2A) = 8;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(t + 0x4) = *(u16 *)((u8 *)b + 0x2);
                *(s16 *)(t + 0x6) = *(u16 *)((u8 *)b + 0x6);
                *(s16 *)(t + 0x8) = *(u16 *)((u8 *)b + 0xA);
                *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) + 1;
                *(s16 *)(t + 0x2C) = 0;
            }
        }
        if (*(s16 *)(p + 0x50) == 7 && *(s32 *)(s + 0x60) != 0)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 4, *(s16 *)(s + 0x2A), (u32)s,
                          2);
        }
        for (i = 0x14; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                e = (u8 *)n + 0x20;
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s09_80024C84;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00;
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
                j = (rand() & 0x3F) - 0xB0;
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)b + 0xA) - j;
                *(s16 *)((u8 *)n->unk_08 + 0xE) = (rand() & 0xF) - 0x8;
                *(s16 *)((u8 *)n->unk_08 + 0x12) = (rand() & 0xF) - 0x8;
                *(s16 *)((u8 *)n->unk_08 + 0x16) = -(rand() & 0x7);
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                *(s16 *)(q + 0x10) = 0x60;
                *(u8 **)(q + 0x0) = D_800DECF8;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DECF8 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(s32 *)(q + 0xC) = 0x404040;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4A) = 0x10;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 8;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x201 node's per-frame step, installed by func_s09_80024020's case 3
   with func_s09_80024868 as its renderer, so the one-parameter signature is
   fixed by that site taking its address. Sets bit 0x8000 in the +0x52
   halfword of the record at +0x0, advances the +0x4/+0x6 position halfwords
   by their +0xC/+0xE velocities, counts the +0x2A timer down, then runs a
   three-case compare chain on +0x2C: state 0 either lights one of the five
   +0x14 colour words to 0x808080 or decays all five by 0x202020, and reloads
   the timer at 3 and bumps the state when it expires; state 1 bumps the state
   on expiry; state 2 retires the actor. 79 words, 0 differing, FIRST COMPILE.

   NO TWIN AT ANY RATIO. Written from the listing plus the landed
   func_s09_80024C84 below it in this unit, which supplies the +0x52 flag
   idiom, the `*(s16 *)x = *(u16 *)x + 1' bump and the retire pair.

   FOUR FACTS, all priced from this finished body:

   * THE TIMER LOCAL MUST BE `s16', NOT `s32' -- `s32 t' is 76 words / 34
     aligned differences. The `s16' local keeps the raw halfword in a register
     and each use pays its own `sll 16'/`sra 16'; the wide form sign-extends
     once and loses three words.
   * THE DECAY LOOP COUNTS DOWN IN SOURCE. `for (i = 4; i >= 0; i--)' is 0 and
     `for (i = 0; i < 5; i++)' is 80 words / 8 aligned differences. The usual
     rule -- an emitted count-DOWN means the source counted up, reversed by
     check_dbra_loop -- does NOT apply when the strength-reduced POINTER also
     descends: here the giv walks p+0x24 down to p+0x14, which a reversed
     counter cannot produce. Read the giv's direction, not the counter's.
   * MEASURED AND FREE, so NOT the reason this lands: the array form
     `((s32 *)(p + 0x14))[i]' against `*(s32 *)(p + i * 4 + 0x14)', 0 either
     way. pointer_int_sum's base-first PLUS does not bite here -- fold reaches
     the same tree from both spellings even though the listing's `addu' puts
     the index first.
   * MEASURED AND FREE: `-= 0x202020' against `+= 0xFFDFDFE0', and the two
     stores of the state-0 reload in the other order (+0x2C before +0x2A) --
     79/0 with both changed together. */
void func_s09_8002472C(u8 *p)
{
    s16 t;
    s32 i;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + *(u16 *)(p + 0xC);
    *(s16 *)(p + 0x6) = *(u16 *)(p + 0x6) + *(u16 *)(p + 0xE);
    t = *(u16 *)(p + 0x2A) - 1;
    *(s16 *)(p + 0x2A) = t;
    switch (*(s16 *)(p + 0x2C))
    {
    case 0:
        i = 7 - t;
        if (i < 5)
        {
            *(s32 *)(p + i * 4 + 0x14) = 0x808080;
        }
        else
        {
            for (i = 4; i >= 0; i--)
            {
                *(s32 *)(p + i * 4 + 0x14) += 0xFFDFDFE0;
            }
        }
        if (*(s16 *)(p + 0x2A) <= 0)
        {
            *(s16 *)(p + 0x2A) = 3;
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        }
        break;
    case 1:
        if (t <= 0)
        {
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        }
        break;
    case 2:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* The 0x201 node's renderer, installed by func_s09_80024020's case 3 beside
   func_s09_8002472C: walk the tag chain from p and, per node, project the
   payload's +0x4 SVECTOR twice through RotTransPers -- as read, then 0xB0
   higher -- and hang up to two semi-transparent POLY_FT4 quads on OT row
   (z - 8). The first quad is gated on (u32)z < 0x1E0, dressed 0x808080 with
   GetTPage(0, 0, 0x2C0, 0x100) / GetClut(0xA0, 0x1F7) and takes its UVs from
   (7 - *(s16 *)(q + 0x2A)); the second is unconditional, 0x161616 with
   GetTPage(0, 3, ...) / GetClut((rand() & 0xF) * 0x10, 0x1F8), UVs from a
   random one of two presets, and its width jittered by rand() % 0x40 - 0x20.
   263 words, 0 differing.

   NO TWIN AT ANY RATIO. `dngcallee' named func_s36_80024798 and
   func_s36_80024C7C in csrc/ovl_dng_s36.c at 6 of 7 symbols with a clean gap
   to the 5/7 tier; func_s36_80024798 is the donor and ALL THREE of its stated
   facts carry -- the projected pair must be `DVECTOR xy[2]', the loop must
   pass `(long *)&xy[i]' rather than walk a cursor local, and a chained
   assignment emits its stores RIGHT TO LEFT, which is where every
   `t->x0 = t->x1 = ...' pair's order comes from. Its `w' fact carries too:
   the half-span AND the UV seed are both stored into the address-taken
   RotTransPers scratch and read back from the frame. Its FRAME does not
   match (-0x50 / nine saves against -0x60 / ten), so no budget fact was taken
   from it.

   THREE FACTS, all priced from this finished body:

   * THE TWO UV PRESET ARRAYS MUST BE AGGREGATE INITIALISERS, NOT ASSIGNMENTS.
     `s32 ut[2] = {0xA0, 0x80}; s32 vt[2] = {0, 0x20};' is 0; the same four
     values assigned as statements after the declarations is 15 aligned
     differences -- a pure sched2 permutation of the prologue, with the ten
     register saves and the preheader's `addiu $fp, $sp, 0x28' / `lui $s3,
     0xFF' interleaved the other way round. CONFIRMED SAFE: a standalone
     `tools/cdkgcc -S' of the whole unit emits NO `$LC' pool, because gcc
     stores a two-element initialiser element-wise, so the only `.rdata'
     object in this unit is still jtbl_s09_80024008.
   * THE FIRST QUAD'S UV SEED IS READ AS A BYTE OUT OF THE SCRATCH SLOT.
     `(*(u8 *)&w & 3) * 0x20' is 0 and `(w & 3) * 0x20' is 261 words / 5
     aligned differences, because combine will not narrow `(and (mem:SI) 3)'
     to a QImode load on its own and the word load costs two words. `w' is
     address-taken -- it is the `(long *)&w' out-parameter -- so the byte
     spelling is a real QImode MEM and not a truncation gcc can fold away.
   * THE SECOND QUAD'S JITTER AND HALF-SPAN MUST REUSE THE LOOP COUNTER `i'
     rather than get locals of their own: two fresh `s32' locals are 20
     aligned differences. `i' crosses the RotTransPers call inside the
     projection loop, so its allocno has calls_crossed > 0 and find_reg gives
     it the callee-saved $s1 that the target spends on all four of the values
     $s1 carries; separate short-lived pseudos land in $v0/$a0 instead. The
     same pseudo also carries the `rand() & 1' UV index, for the same reason. */
s32 func_s09_80024868(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    s32 i;
    s32 z;
    s32 k;
    s32 c;
    s32 ut[2] = {0xA0, 0x80};
    s32 vt[2] = {0, 0x20};
    SVECTOR sv;
    DVECTOR xy[2];
    s32 w;

    while (1)
    {
        q = p;
        sv = *(SVECTOR *)(p + 0x4);
        for (i = 0; i < 2; i++)
        {
            z = RotTransPers(&sv, (long *)&xy[i], (long *)&w, (long *)&w) - 8;
            sv.vz += 0xB0;
        }
        k = xy[0].vy - xy[1].vy;
        if ((u32)z < 0x1E0)
        {
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
            *(u32 *)&t->r0 = 0x808080;
            SetPolyFT4(t);
            SetSemiTrans((u8 *)t, 1);
            t->tpage = GetTPage(0, 0, 0x2C0, 0x100);
            t->clut = GetClut(0xA0, 0x1F7);
            w = k / 6;
            t->x0 = t->x1 = xy[0].vx + w;
            t->x2 = t->x3 = xy[0].vx - w;
            t->y0 = t->y2 = xy[0].vy;
            t->y1 = t->y3 = xy[1].vy;
            w = 7 - *(s16 *)(q + 0x2A);
            c = (*(u8 *)&w & 3) * 0x20;
            t->u0 = t->u2 = c - 0x80;
            t->u1 = t->u3 = c - 0x61;
            c = (w >> 2) * 0x20;
            t->v2 = t->v3 = c;
            t->v0 = t->v1 = c + 0x1F;
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
        }
        t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x34;
        *(u32 *)&t->r0 = 0x161616;
        SetPolyFT4(t);
        SetSemiTrans((u8 *)t, 1);
        t->tpage = GetTPage(0, 3, 0x2C0, 0x100);
        t->clut = GetClut((rand() & 0xF) * 0x10, 0x1F8);
        i = rand() & 1;
        t->u0 = t->u2 = ut[i];
        t->u1 = t->u3 = ut[i] + 0x5F;
        t->v0 = t->v1 = vt[i] + 0x20;
        t->v2 = t->v3 = vt[i];
        i = rand() % 0x40 - 0x20;
        i = (i >> 1) + (i >> 2);
        w = k / 6;
        t->x0 = t->x1 = xy[0].vx + w + i;
        t->x2 = t->x3 = xy[0].vx - w + i * 2;
        t->y0 = t->y2 = xy[0].vy;
        t->y1 = t->y3 = xy[1].vy;
        addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

/* The 0x312 smoke node's per-frame step, installed as the +0x10 step at TWO
   sites in THIS unit by func_s09_80024020, so the four-argument signature is
   fixed by those call sites.

   *** THE NAME IN THIS SENTENCE WAS WRONG UNTIL 2026-11-26: it read
   "func_s12_80024020's spawn loops and by its case 4". This body is a dngclone
   replica of the s12 original, and the clone carried the DONOR unit's symbol
   names into its prose as though they were local. Nothing in ovl_dng_s12
   installs this function -- func_s09_80024020 does, at lines 117 and 242.
   An agent read the sentence as naming the real caller; it happened to point
   at the right DONOR, so the residue helped rather than hurt that time, which
   is exactly why it survived this long. Sets bit 0x8000 in the +0x52
   halfword of the record at the actor's +0x0, decays the caller's +0x14
   velocity component by 0x20000 unless it has already reached zero, advances
   the vector by its own velocity half, then runs a two-case compare chain on
   +0x4C: state 0 counts the +0x48 timer down and re-registers the renderable
   through func_8004491C when it expires, state 1 hands the renderable to
   func_800478B8, clears its +0x4/+0x5 once +0x14 reports 0x6000, and fades
   its +0xC/+0xD/+0xE colour bytes by the +0x4A step until the red byte would
   pass the actor's own level, at which point it clears the colour word and
   retires. 91 words, 0 differing. 2-member family with
   func_s09_80024C84 (dngfam); I did not touch the sibling.

   NO TWIN AT ANY RATIO. Landed off the IN-UNIT route plus one donor row:
   every idiom but the fade came from func_s12_80024F40 below (the +0x52 flag,
   the retire pair, the two-case compare chain), and the
   `b->unk_00 += b->unk_0C' triple from csrc/ovl_dng_s23.c
   func_s23_80024024, which the callee-set overlap on
   D_80045340 + D_800814A0 + func_8004491C + func_800478B8 named.

   FOUR FACTS, all priced from this body:

   * THE FADE/RETIRE GATE'S ARMS WERE THE WRONG WAY ROUND and that was 17 of
     the 19 aligned differences this body started at -- one character, and the
     residue was the whole subu block emitted in the wrong position with an
     inverted branch, exactly as the brief describes.
   * THE COMPARE PUTS THE STRUCT FIELD ON THE LEFT:
     `*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A)' is 0 and
     `*(s16 *)(p + 0x4A) >= *(u8 *)(c + 0xC)' is 2 aligned differences, a pure
     transposition of the two loads, because operand 0 of a comparison is
     expanded first.
   * THE +0x14 DECAY NEEDS A LOCAL: read into `v', test, decay, store back
     unconditionally. Written in place as
     `if (b->unk_14 != 0) { b->unk_14 -= 0x20000; }' it is 5 aligned
     differences, because the target's store at the join is unconditional.
   * THE COLOUR FADE IS ONE CHAINED ASSIGNMENT, not three statements -- three
     statements are 92 words / 3 aligned differences. Chained, the stores come
     out +0xE, +0xD, +0xC, which is a chained assignment's right-to-left
     order. */
void func_s09_80024C84(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 v;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    v = b->unk_14;
    if (v != 0)
    {
        v -= 0x20000;
    }
    b->unk_14 = v;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
        }
        if (*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A))
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= *(u8 *)(p + 0x4A);
        }
        break;
    }
}
