#include "common.h"

typedef struct UnkDngS36Scale
{
    u8 unk_00[0x18];
} UnkDngS36Scale;
extern u8 D_s35_80024004[];

s32 rand(void);
s32 func_800644B8(s32);
void func_800478B8(u8 *);
void func_800DBA90(u8 *);
void func_800419EC(u16, u32);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_8003FD64(s32, void *);
s32 func_800A3820(s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A56E0(s32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800BCB04(s32, s32, s16);
s32 func_800C8900(u8 *, s16, s16);
void func_s35_80024594(u8 *p, u8 *b, u8 *c);
void func_s35_80024784(u8 *p, s16 i);
extern u8 D_80045340[];
extern u8 D_80083498[];
extern s32 D_8008346C;
extern u32 D_800814A0;
extern u8 D_800DEC28[];
extern u8 D_800E3D68;

/* This bank's five-state cutscene director on *(s16 *)(p + 0xA), a COMPARE
   CHAIN over {0, 1, 2, 0xF0, 0xFF}. It owns NO jump table: five sparse cases
   over a 0..0xFF range, so expand_end_case builds the balanced tree
   `beq 2' / `slti 3' this listing has, and jtcheck reports 0 tables for the
   whole unit. The arms are written ASCENDING and the body is at 0 aligned
   differences, which MEASURES the order -- a compare chain's arms emit in
   source order and the five come out ascending 0x80024208 / 0x800243D0 /
   0x80024438 / 0x80024480 / 0x80024510. I did not price another order.

   Before the switch, a block guarded by `*(s16 *)(p + 0xA) == 2' copies the
   party record's -- or the escort actor's -- three position words into
   b + 0x0/+0x4/+0x8 and spawns SIX 0x312 corner nodes, each dressed from
   D_800DEC28 at scale 0x1000 and stepped by this unit's own
   func_s35_80024594. State 0 waits for 0x80 in the record at p + 0x4, asks
   func_800A05A4 for the escort actor, bails to state 0xF0 when both that
   actor's record and the caller's THIRD argument carry 0x8000 in +0x14,
   copies the +0x24/+0x25 facing pair into s + 0x72/+0x73 and aims the three
   velocity words by dividing each leg by the 8-frame timer. State 1
   integrates. State 2 forks on s + 0x60 into state 0xFF or state 0xF0. State
   0xF0 drives func_s35_80024784's scale ramp and flashes the escort through
   func_800C8900. State 0xFF is the standard teardown. 350 words, 0
   differing; SIX compiles.

   DONOR: csrc/ovl_dng_s34.c's landed 326-word func_s34_80024020, found by
   dngcallee.py at 13 of 16 symbols with a CLEAN GAP to the 11/16 tier -- the
   strongest donor row I have seen. dngtwin was not consulted. THE FRAMES
   DIFFER: -0x48 with ten saves here against the donor's -0x38 with eight, so
   I treated every BUDGET fact of the donor's as VOID and carried only its
   SELECTION facts. Both selection facts I carried held, at different prices.
   Its states 0, 1, 2, 3 and 5 are this body's 0, 1, 2, 0xF0 and 0xFF; its
   four-corner `?:' fact does not apply, because these six nodes take no
   per-corner offset.

   THE FRAME IS THE WHOLE STORY OF THIS BODY, AND IT IS READABLE BEFORE ANY
   COMPILE. The target saves ten registers AND spills `o' to 0x18($sp), which
   is ELEVEN long-lived values against ten registers; the eleventh is the
   hoisted D_800DEC28 address in $fp. Spelled so loop.c cannot hoist it the
   body is 347 words at -0x40, same ten saves, no spill slot, with $fp
   holding `o' instead. Comparing `addiu $sp, -N' against the listing is what
   named the missing value.

   NINE FACTS, every one priced from THIS body AT 0 differing and RE-MEASURED
   after it closed (aligndiff, words / aligned rows, R rows INCLUDED). Three
   had different figures mid-fight -- f2 read 19 rows then and 7 now -- so no
   mid-fight number appears in this table.

     landed as written below                                     350 /  0
   f1  `*(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4)',
       the donor's own spelling                                   347 / 65
   f12 the three b position stores written ONCE through one
       pointer instead of duplicated in both arms of the gate     334 / 62
   f6  the two `*(s8 *)(s + 0x72/0x73)' reads inlined into the
       `<< 6' instead of lifted into `k' first                    351 / 24
   f8  case 0xF0's null test written FIRST in the `&&' chain      351 / 13
   f5  `u8 func_800A3820(s16)' -- its real landed return type     349 /  8
   f4a a separate `u8 *g' assigned INSIDE the else arm            350 /  6
   f3  the three `e' zero stores before the `e + 0x10' statement  350 /  6
   f4b a separate `u8 *g' assigned BEFORE the gate                350 /  4
   f2  `q + 0x4'/`q + 0x5' zeroed BEFORE the `q + 0x8' load       351 /  7
   f1b `t = D_800DEC28; ... *(s32 *)(t + 0x4)', a pointer local   350 /  0

   f1 IS THE LEVER AND f1b IS WHY IT IS NOT ABOUT A LOCAL. Spelled the
   donor's way, cse still forwards the stored symbol to the reload, so the
   SITE is identical either way -- the difference is upstream. Read back
   through the payload the symbol is a one-reference in-loop constant and
   loop.c hoists nothing; named a second time (or held in a pointer local, f1b,
   which is FREE against it -- measured, not assumed) it is an invariant with
   two references, loop.c lifts it to $fp, and `o' goes to the stack. I kept
   the named form because it needs no local and is this block's idiom. The 65
   rows are mostly the register rotation that follows the spill, not 65
   causes. See `A LOOP-INVARIANT SYMBOL MUST BE *NAMED* AGAIN' in
   docs/matching-levers.md, where this is now the second witness.

   f4 IS A MERGE, NOT A PRIORITY FAULT, AND THE RESIDUE NAMED THE CLASS.
   With a `g' of its own the pointer sits in $v1 where the target has $s2 --
   mine CALLER-saved where the target is CALLEE-saved. But no call lies
   anywhere in `g''s range, nor before it in the function, so there is
   nothing to move the definition in front of and no spelling of `g' can
   raise allocno_calls_crossed above 0. $s2 is the loop's NODE pointer, which
   crosses func_8004491C; writing
   `n = (UnkNode *)(*(u8 **)(s + 0x60) - 0x20)' before the gate and reaching
   the arm through `(u8 *)n' gives the two values ONE pseudo that crosses
   that call, and it lands. READ THE LADDER: 6 rows with `g' inside the arm,
   4 with `g' hoisted out of it, 0 merged -- and the 6-to-4 step is NOT the
   class. Hoisting is what breaks the combine_regs tie and produces the
   target's `addiu $s2, $v0, -0x20', an insn writing a DIFFERENT register
   from the one it reads. The listing states that tie break outright, so both
   halves were readable before the first compile.

   f5 AND f6 ARE THE DONOR'S TWO SELECTION FACTS, BOTH HELD, BOTH REPRICED.
   func_800A3820's landed definition in csrc/ovl_dungeon_8009F644.c returns
   `u8'; declared honestly the call truncates with `andi 0xff' and the
   target's `sll 16'/`sra 16' cannot appear. The donor gives no figure for
   it; here it is 349 / 8. The `s8' facing reads must each be a statement of
   their own or combine fuses the sign-extension into `lbu'/`sll 24'/
   `sra 18' -- `lbu' where the target has `lb', the donor's diagnostic
   exactly, priced 331 / 125 there and 351 / 24 here. Direction carried,
   magnitude did not, on both.

   f8 IS THIS BODY'S OWN AND IT IS THE ONE THAT LOOKS WRONG IN THE SOURCE.
   `func_8009D218(...) == 0 && (*(s32 *)(... + 0x14) & 0x1) != 0 &&
   *(u8 **)(s + 0x60) != NULL' tests for null LAST, after dereferencing. That
   is what the listing has -- `beqz $a0' comes after the `andi 0x1' -- and
   the order anyone would write costs a word and 13 rows. Do not tidy it.

   UNMEASURED (chosen, worked, never flipped -- NOT facts): the
   `if (p + 0xA == 2)' block placed before the switch rather than inside
   `case 2:' (a different program, so there is no price to quote); the loop
   as `i = 5; i >= 0; i--'; the three `/= *(s16 *)(p + 0x50)' re-reading the
   divisor out of memory, which is why the listing has three `lh 0x50($s5)'
   and three `div' -- a literal divisor never reaches `div' at all; and
   `rand' as the name of 0x80069EF8, which comes from splat/common_syms.txt
   and not from a measurement, since no tool can see which routine a `jal'
   names. */
void func_s35_8002401C(u8 *p, UnkVec *b, u8 *d)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *c;
    u8 *q;
    u8 *e;
    UnkNode *n;
    s32 i;
    s32 k;
    s32 u;
    s32 m;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    if (*(s16 *)(p + 0xA) == 2)
    {
        n = (UnkNode *)(*(u8 **)(s + 0x60) - 0x20);
        if (*(s32 *)(s + 0x60) == 0)
        {
            *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)(w + 0x8) + 0x0);
            *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)(w + 0x8) + 0x4);
            *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)(w + 0x8) + 0x8);
        }
        else
        {
            *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)((u8 *)n + 0x8) + 0x0);
            *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)((u8 *)n + 0x8) + 0x4);
            *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)((u8 *)n + 0x8) + 0x8);
        }
        for (i = 5; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s35_80024594;
                e = (u8 *)n + 0x20;
                *(s16 *)(e + 0xC) = *(u16 *)((u8 *)b + 0x2);
                *(s16 *)(e + 0xE) = *(u16 *)((u8 *)b + 0x6);
                *(s16 *)(e + 0x10) = *(u16 *)((u8 *)b + 0xA) + 0x10;
                *(s16 *)(e + 0x48) = 0;
                *(s16 *)(e + 0x16) = 0;
                *(s16 *)(e + 0x14) = 0;
                *(s16 *)(e + 0x18) = rand();
                *(s16 *)(q + 0x1E) = 0x1000;
                *(s16 *)(q + 0x1C) = 0x1000;
                *(s16 *)(q + 0x10) = 0x60;
                *(u8 **)(q + 0x0) = D_800DEC28;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC28 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)(e + 0x0) = p;
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
        c = (u8 *)func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25), *(s16 *)(s + 0x2A),
                                (s16)func_800A3820(0x23));
        *(u8 **)(s + 0x60) = c;
        if (c == NULL)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)(c - 0x14);
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (*(u16 *)(d + 0x14) & 0x8000) != 0)
            {
                *(s16 *)(p + 0xA) = 0xF0;
                break;
            }
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
        *(s16 *)(p + 0x50) = 8;
        k = *(s8 *)(s + 0x72);
        *(s16 *)((u8 *)b + 0xE) = (k << 6) - (*(u16 *)((u8 *)b + 0x2) - 0x20);
        *(s32 *)((u8 *)b + 0xC) /= *(s16 *)(p + 0x50);
        k = *(s8 *)(s + 0x73);
        *(s16 *)((u8 *)b + 0x12) = (k << 6) - (*(u16 *)((u8 *)b + 0x6) - 0x20);
        *(s32 *)((u8 *)b + 0x10) /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x16) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) -
            *(u16 *)((u8 *)b + 0xA);
        *(s32 *)((u8 *)b + 0x14) /= *(s16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        *(s32 *)((u8 *)b + 0x0) += *(s32 *)((u8 *)b + 0xC);
        *(s32 *)((u8 *)b + 0x4) += *(s32 *)((u8 *)b + 0x10);
        *(s32 *)((u8 *)b + 0x8) += *(s32 *)((u8 *)b + 0x14);
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        m = 0xA;
        if (*(s32 *)(s + 0x60) != 0)
        {
            m = 0x18;
        }
        *(s16 *)(p + 0x50) = m;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (*(s32 *)(s + 0x60) == 0)
        {
            *(s16 *)(p + 0x50) = 0xA;
            *(s16 *)(p + 0xA) = 0xFF;
        }
        else
        {
            func_800419EC(8, 0x10);
            *(s16 *)(p + 0x50) = 6;
            *(s16 *)(p + 0xA) = 0xF0;
        }
        break;
    case 0xF0:
        func_s35_80024784(*(u8 **)(s + 0x60), *(s16 *)(p + 0x50));
        if (*(s16 *)(p + 0x50) == 2)
        {
            if (func_8009D218(*(u8 **)(s + 0x60), 2, s) == 0 &&
                (*(s32 *)(*(u8 **)(s + 0x60) + 0x14) & 0x1) != 0 &&
                *(u8 **)(s + 0x60) != NULL)
            {
                u = 0x10;
                if (D_800E3D68 == 0xFF)
                {
                    u = 0xFF;
                }
                func_800C8900(*(u8 **)(s + 0x60), u, 2);
            }
        }
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        *(s16 *)(p + 0xA) = 0xFF;
        break;
    case 0xFF:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            if (*(s16 *)(p + 0x50) > 0)
            {
                break;
            }
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* This bank's thrown-prop step: it sets the owner record's +0x52 retire flag,
   ticks p + 0x48 up, slides the renderable's +0x1C/+0x1E scale pair down by
   0x50, adds a random 0x200..0x23F to p + 0x18, builds a frame SVECTOR from
   `func_800644B8(50 * p + 0x48 + 0xC8) / 80', 0 and `-4 * p + 0x48 +
   (rand() & 3) - 1', writes the +0xC/+0xD/+0xE colour bytes from that
   vector's low byte, rotates it through func_800DBA90's 0x20-byte matrix
   record, publishes it into b + 0x2 / +0x6 / +0xA, re-dresses the renderable
   and retires the actor once p + 0x48 reaches 0x20. 124 words, 0 differing;
   SECOND compile, one fault.

   DONOR: csrc/ovl_dng_b38.c's landed 122-word func_b38_80173F0C, found by
   dngcallee.py (4 of 5 symbols; of the ten functions in that tier it is the
   only one with BOTH this frame and this save list, -0x48 with
   $s0/$s1/$s2/$ra in the same slots -- a byte-identical prologue, so its
   budget facts carry). dngtwin was not consulted. b38's case 0 is this
   body's statement list reordered: this body has no switch, the +0x48 count
   goes UP at the top where b38's goes DOWN at the bottom, the
   func_800478B8/+0x14-0x6000 pair moves from the head to the tail, and b38's
   state 1 teardown becomes the trailing `>= 0x20' gate. CARRIED FROM b38
   UNMEASURED HERE: `u8 rec[0x20]' rather than `u32 rec[0x8]' -- b38 prices
   the alignment-1 fact at 11 aligned differences and this listing has the
   same testless lwl/lwr + swl/swr pair per word, so it is the same fact, but
   I did not re-price it on this body.

   THE ONE FAULT, and the only thing that was not free -- 14 aligned
   differences, the whole residue, at both +0xC/+0xD and +0xE:
   `0x28 - p[0x48] + (u8)vec.vx'                    124 words / 0
   `z = (u8)vec.vx + 0x28; ... z - p[0x48]'         124 words / 9
   `(u8)vec.vx + 0x28 - p[0x48]'                    124 words / 14
   The middle row matters: a STATEMENT BOUNDARY is what the levers file's
   `FOLD REASSOCIATES A + K - B INTO A - (B - K)' entry prescribes, and here
   it fixes the arithmetic -- the `addiu 0x28' goes back on the vector byte
   and the `subu' turns round -- and then costs 9 rows of its own, because
   the extra pseudo is born before the `lbu' pair instead of between them.
   Reordering the operands needs no local and reaches 0. I have added this as
   an amendment to that entry.
   Same value, and the second is the form anyone writes first. fold's
   `associate' rewrites `(VAR + CON) - ARG1' into `(ARG1 - CON) ... VAR', so
   the wrong order emits `addiu $v0, $v0, -0x28' on the p + 0x48 load and
   subtracts the vector byte from THAT; the target has `addiu $v0, $v0, 0x28'
   on the vector byte. Writing the constant and the subtrahend first leaves
   split_tree nothing to move. Each site is 2 real rows plus a register
   exchange and a transposed load pair.

   MEASURED AND FREE ON THIS BODY, so none of these is why it lands:
     * splitting the chained scale store into
       `*(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x50;' followed by
       `*(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);'   0 either way (cse
       forwards the stored halfword, so there is no reload to lose).
     * `*(u8 *)&vec' in place of `(u8)vec.vx'        0 either way. The
       NARROWING is load-bearing -- both spellings reach the `lbu 0x30($sp)'
       the target has -- but which of the two you write is not.

   NOT FREE, 2 aligned differences: `c[0xD] = c[0xC] = ...' instead of
   `c[0xC] = c[0xD] = ...'. The inner assignment is emitted first, so the
   chain written this way order puts `sb' to +0xD before +0xC, which is the
   listing's order.

   UNMEASURED SPELLINGS (chosen, worked, never flipped -- do not read these as
   facts): `* 50' as a plain constant for the sll/addu/sll/addu/sll chain;
   `/ 80' for the 0x66666667 magic multiply at post-shift 5; the position of
   `vec.vy = 0' between vx and vz; `rand' as the name of 0x80069EF8 (from
   splat/common_syms.txt, and no tool can see a jal's name). */
void func_s35_80024594(u8 *p, u8 *b, u8 *c)
{
    u8 rec[0x20];
    SVECTOR vec;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x50;
    *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) + 0x200 + (rand() & 0x3F);
    vec.vx = func_800644B8(*(s16 *)(p + 0x48) * 50 + 0xC8) / 80;
    vec.vy = 0;
    vec.vz = -*(s16 *)(p + 0x48) * 4 + (rand() & 0x3) - 1;
    c[0xC] = c[0xD] = 0x28 - p[0x48] + (u8)vec.vx;
    c[0xE] = 0x60 - p[0x48] + (u8)vec.vx;
    *(u8 **)rec = (u8 *)&vec;
    *(u8 **)(rec + 0x4) = (u8 *)&vec;
    memcpy(rec + 0x8, p + 0x14, 8);
    memcpy(rec + 0x10, p + 0xC, 8);
    *(s16 *)(rec + 0x18) = 1;
    *(s16 *)(rec + 0x1A) = 0;
    func_800DBA90(rec);
    *(s16 *)(b + 0x2) = vec.vx;
    *(s16 *)(b + 0x6) = vec.vy;
    *(s16 *)(b + 0xA) = vec.vz;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        c[0x4] = 0;
        c[0x5] = 0;
    }
    if (*(s16 *)(p + 0x48) >= 0x20)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Copy this bank's six-entry {u16, u16} scale table into the frame and write
   the entry the caller's +0x50 state selects into the renderable's +0x1C/+0x1E
   scale pair, clamping an out-of-range index to 0.  47 words, 0 differing,
   FIRST COMPILE.

   NO TWIN at any ratio and nothing shares a symbol with it (D_s35_80024004 is
   bank-local).  The donor is csrc/ovl_dng_s67.c's landed func_s67_80024C50,
   found by its comment rather than by a ratio: the same alignment-1 block move
   of a bank table into a frame struct, the same `<< 2' index.

   THREE FACTS, all priced from this body at 0 differing:
     * `u8 *d = t.unk_00' as a POINTER LOCAL.  Indexing through `t.unk_00'
       directly is 49 words / 13 aligned differences -- two words and a
       register exchange for one folded pointer.
     * The index is `i << 2', not `i * 4': 4 differing.  s67 priced the same
       fact at 2 on its own body.
     * The 0x1C store before the 0x1E store: swapped is 4 differing.
   MEASURED AND FREE, so neither is the reason this lands:
   `memcpy(d, D_s35_80024004, 0x18)' in place of the struct assignment (0
   either way, where s67 priced the same swap at 18 -- it has stack parameters
   for the scheduler to sink past the block move and this one has none), and
   `if ((u16)i >= 6)' in place of `if (i < 0 || i >= 6)' (0 either way; both
   reach build_range_check's `andi 0xFFFF' + `sltiu 6'). */
void func_s35_80024784(u8 *p, s16 i)
{
    UnkDngS36Scale t;
    u8 *d = t.unk_00;
    UnkSlot *s;

    t = *(UnkDngS36Scale *)D_s35_80024004;
    if (i < 0 || i >= 6)
    {
        i = 0;
    }
    s = (UnkSlot *)*(u32 *)(p - 0x14);
    s->unk_1C = *(u16 *)(d + (i << 2));
    s->unk_1E = *(u16 *)(d + (i << 2) + 2);
}
