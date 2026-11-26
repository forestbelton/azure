#include "common.h"

s32 rand(void);
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");
void func_800478B8(u8 *);
UnkNode *func_8003FD64(s32, void *);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A3820(s16);
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
void func_s06_80024860(u8 *p, UnkVec *b, u8 *c);
void func_s06_800248F0(u8 *p, UnkVec *b, u8 *c);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u32 D_800814A0;
extern s32 D_8008346C;
extern u8 D_80045340[];
extern u8 D_80083498[];
extern u8 D_800DEA68[];
extern u8 D_800DED70[];

/* This bank's 0x312 smoke/dust director: a COMPARE-CHAIN switch (no jump
   table -- jtcheck reports 0 tables for this unit) on *(s16 *)(p + 0xA) over
   cases 0, 1, 2, 3, 0xF0 and 0xFF, arms written ASCENDING, which is the
   listing's arm-address order. Before the switch it reads the actor's node
   `s', the node's -0x14 record `o', the -0x20 window `w', the facing pair out
   of D_8006CCD8 / D_8006CCE8, and runs the p + 0x50 timer down one; while the
   state is 1..3 it spawns four or six 0x312 nodes running
   func_s06_80024860, jittered +-0x10 twice around n->unk_08's own x/y and
   re-aimed with func_800BCB04, dressed from D_800DEA68. State 0 registers the
   party record through func_800A05A4, publishes w + 0x8's position into b,
   seeds the timer from the abs grid distance and loads b's two velocity words
   from the facing pair; 1 advances b; 2 either retires to 0xFF or marks the
   record 0x100000 and reloads at 0x14; 3 spawns four func_s06_800248F0
   nodes, bleeds the record's scale and ends with func_8009CE1C; 0xF0 eases
   the scale back to 0x1000; 0xFF is the teardown.
   505 words, 0 differing. func_80069EF8 is `rand' out of
   splat/common_syms.txt.

   DONOR: func_s08_80024020 in csrc/ovl_dng_s08.c, named by `dngcallee' at
   13 of 16 symbols with a clean gap to the 12/16 tier -- `dngtwin' found
   NOTHING for this stub even at ratio 0.50, so dngcallee beat it outright
   again. Its frame is identical, -0x58 with the same ten saves, and its
   s / o / w prologue, its spawn-loop skeleton, its dress block, its
   func_800A05A4 head and its 0xFF teardown all carried. It is a SWITCH
   body, not this shape: s08 owns a jump table over 0..5 and this one is a
   compare chain over a sparse set, so nothing about its arm order transfers.

   EVERY FIGURE BELOW IS PRICED ONE AT A TIME FROM THIS FINISHED 505-WORD
   BODY, not mid-fight.

   1. THE FACING INDEX IS A VARIABLE THAT LATER BECOMES THE LOOP COUNTER --
      15 rows, and it was the LAST thing to fall.
      `i = *(u16 *)(s + 0x2A) >> 8;' with the two reads spelled
      `*(s16 *)((i & 0xE) + (s32)D_8006CCD8)' is 0.
      Writing the index inline as `D_8006CCD8[(*(u16 *)(s + 0x2A) >> 9) & 7]'
      is 505 words / 15 aligned rows, ALL of them in the prologue: the two
      table addresses' `lui'/`addiu' pairs change places and the registers
      rotate behind them. The tell is that the target's shift lands in $s4 --
      a CALLEE-SAVED register for a value that dies two instructions later --
      which is only possible if the shift's pseudo IS the loop counter's.
      Inline, the shift is a combine-folded subexpression with no pseudo of
      its own and gets $v1.
      MEASURED NOT MEASURABLE, so do not spend compiles on them: the
      subscript versus byte-offset spelling of the two reads (bit-identical
      output), and a `s16 *' pointer local for either table (also
      bit-identical). Only giving the SHIFT a variable moves it.
      MEASURED WRONG-AND-INVISIBLE: swapping the dx and dy statements keeps
      505 words and 15 rows while exchanging the two table SYMBOLS, i.e. the
      verifier cannot see it. dx must come from D_8006CCD8 and feed
      b->unk_0C. `relcand' is what catches that.

   2. THE SPAWN COUNT GATE IS INVERTED FROM THE OBVIOUS READING -- 2 rows.
      `if (*(s16 *)(p + 0xA) < 3) i = 5; else i = 3;' is 0; the arms the
      other way round are 505 words / 2 rows. The listing puts
      `addiu $s4, $zero, 0x3' in the branch's DELAY SLOT and
      `addiu $s4, $zero, 0x5' after it, so the 3 is the always-executed set
      and the 5 is the fall-through one.

   3. THE 0x76C GATE IS ALSO INVERTED -- 1 row.
      `if (*(u16 *)(q + 0x1E) < 0x76C)' is 0; `>= 0x76C' is 1 row. The
      `sltiu'/`beqz' pair reads as the body running on the LOW side, where
      the 0x21C1 gate forty words earlier is `sltiu'/`bnez' and runs on the
      HIGH side -- the two are spelled oppositely in one basic block, so read
      each branch opcode rather than assuming a house direction.
      `*(u16 *)' compares emit `sltiu' here with no cast: combine proves the
      zero-extended halfword non-negative and converts the signed compare.

   4. THE INNER JITTER'S CONSTANT NEEDS A MODE-CHANGING CAST, NOT PARENTHESES
      AND NOT A TEMPORARY -- 1 word and 9 rows per site, 2 sites.
      `(s16)(*(u16 *)((u8 *)n->unk_08 + 0x2) - 0x10) + (rand() % 32)' is 0.
      Without the `(s16)': 506 words / 9 rows, with `li $t0, 0xFFF0' plus an
      `addu' where the target has one `addiu $a2, $v0, -0x10'.
      TWO different compiler guards are in play and the cast satisfies both,
      which is why no other spelling reached it. convert.c trunc1 takes typex
      unsigned if EITHER operand's UNWIDENED type is unsigned
      (convert.c:306-309), and `get_unwidened' keeps a same-precision NOP
      node, so `(s16)' of a `u16' read makes arg0 signed and the constant
      stays -0x10 rather than folding to 0xFFF0 in unsigned short. And
      fold's `associate' reassociates `(A - C) + B' into `A + (B - C)'
      unconditionally once `split_tree' succeeds -- `split_tree'
      (fold-const.c:1001-1006) strips only conversions that PRESERVE THE
      MACHINE MODE, so the HImode-to-SImode NOP the cast leaves makes
      split_tree fail and pins the group. The levers file says a temporary is
      the only way to pin such a constant; a temporary is the only way that
      does not need a cast, and here it is the cast that is free.

   5. THE CASE-3 JITTERS ARE CHAINED ASSIGNMENTS -- 2 words and 22 rows per
      site, 3 sites, and this took the body from 509/137 to 505/21.
      `*(s16 *)((u8 *)n->unk_08 + 0x2) =
           (*(s16 *)(e + 0xC) = *(u16 *)((u8 *)b + 0x2)) + (rand() % 128)
           - 0x40;' is 0.
      Written as two statements, either order, it is 507 words / 22 rows at
      that one site: the target loads `b + 0x2' ONCE and uses it both for the
      `e' store and for the sum, and the only way to get that is for both
      uses to sit after the `rand' call in ONE statement -- `preexpand_calls'
      hoists the call to the front of the expression, and a second statement's
      reload cannot be cse'd because the intervening store through
      `n->unk_08' invalidates it. With the e-store FIRST the load is before
      the call instead, which the registers rule out: the target's shared
      value lives in $a0, a caller-saved register, so it cannot cross the
      call. Also measured and rejected: a local for the `rand' result, 510
      words -- it buys the single load and pays a `move' per site, because
      the mod sequence's own copy and the local's copy stop coinciding.

   6. CASE 3's /10 WANTS THE FLAT `A + K - B' FORM -- 1 word and 19 rows.
      `(h + 0x18 - *(s16 *)((u8 *)n->unk_08 + 0xA)) / 10' is 0;
      `(h - (*(s16 *)(... + 0xA) - 0x18)) / 10' is 504 words / 19 rows.
      This is the levers file's `fold' entry used FORWARD: fold rewrites
      `A + K - B' into `A - (B - K)', so writing the flat form is how you GET
      the `addiu $v0, $v0, -0x18' followed by `subu $v0, $s2, $v0' that the
      listing has. Writing the already-reassociated form folds it the other
      way and loses the `addiu'. The `sra $v0, $v0, 31' in the magic multiply
      is the tell that the dividend is signed, and the divisor is 10
      (0x66666667 with a post-shift of 2), not 5 or 20.

   7. THE TWO RANDOM TERMS AFTER THE LOOP MUST BE LOCALS -- 7 rows.
      `k = (rand() & 0x1FF) + 0x100;' then `... + k' is 0; the flat
      `*(u16 *)(q + 0x1C) + (rand() & 0x1FF) + 0x100' is 505 words / 7 rows
      with the `addiu 0x100' on the LOAD instead of on the `andi'. Same
      mechanism as 5 and 6 from the other side: fold's arg1 branch
      reassociates `A + (B + K)' into `(A + K) + B', and a VAR_DECL is the
      one arg1 `split_tree' cannot decompose. Here the local costs nothing
      because there is no mod sequence to collide with -- which is exactly
      why the same device FAILED in case 3's loop. A local for a random term
      is free where the term is a plain and-plus-constant and costs a word
      where it is a modulo.

   8. LOOP 1 STORES THE OWNER THROUGH `e', LOOP 2 THROUGH `n + 0x20' -- 24
      rows. `*(u8 **)e = p;' in the first spawn loop is 0;
      `*(u8 **)((u8 *)n + 0x20) = p;' there is 505 words / 24 rows, a
      whole-body $s3/$s4 exchange. The second loop needs the opposite
      spelling and is 0 with it. Two sites, same expression, opposite
      answers, 40 words apart -- read the base register off each `sw'.

   9. THE 0xC DRESS STORE GOES BEFORE THE D_800DEA68 STORE -- 1 word and 9
      rows. Moving `*(s32 *)(q + 0xC) = 0xC0C0C0;' down to the donor's
      position after the two byte stores is 506 words / 9 rows; the listing
      materialises the constant early and stores it between +0x10 and +0x0.

   UNPRICED, so do NOT treat as facts: `__builtin_abs' for the two grid
   distances (the levers file's `bgez'/COPY/`negu' row already settles the
   distinct-destination spelling and it landed first try), the `(s16)' casts
   on func_800A3820's and func_800BCB04's results, the `>> 2' rather than
   `/ 4' in case 0xF0 (the bare `sra' with no bias word is the tell), and the
   three `*(u8 **)(w + 0x8)' reads being written out rather than hoisted. */
void func_s06_80024004(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q;
    u8 *e;
    UnkNode *n;
    s32 i;
    s32 j;
    s32 r;
    s32 h;
    s32 k;
    s32 k2;
    s32 dx;
    s32 dy;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    i = *(u16 *)(s + 0x2A) >> 8;
    dx = *(s16 *)((i & 0xE) + (s32)D_8006CCD8);
    dy = *(s16 *)((i & 0xE) + (s32)D_8006CCE8);
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    if (*(u16 *)(p + 0xA) != 0 && *(u16 *)(p + 0xA) < 4)
    {
        if (*(s16 *)(p + 0xA) < 3)
        {
            i = 5;
        }
        else
        {
            i = 3;
        }
        for (; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s06_80024860;
                e = (u8 *)n + 0x20;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00;
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
                for (j = 1; j >= 0; j--)
                {
                    *(s16 *)((u8 *)n->unk_08 + 0x2) =
                        (s16)(*(u16 *)((u8 *)n->unk_08 + 0x2) - 0x10) + (rand() % 32);
                    *(s16 *)((u8 *)n->unk_08 + 0x6) =
                        (s16)(*(u16 *)((u8 *)n->unk_08 + 0x6) - 0x10) + (rand() % 32);
                }
                *(s16 *)((u8 *)n->unk_08 + 0xA) =
                    func_800BCB04(*(u16 *)((u8 *)n->unk_08 + 0x2), *(u16 *)((u8 *)n->unk_08 + 0x6),
                                  *(u16 *)((u8 *)b + 0xA) - 0x30);
                if (*(s16 *)((u8 *)n->unk_08 + 0xA) >= 0x200)
                {
                    *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)b + 0xA);
                }
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)n->unk_08 + 0xA);
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1400;
                *(s16 *)(q + 0x10) = 0;
                *(s32 *)(q + 0xC) = 0xC0C0C0;
                *(u8 **)(q + 0x0) = D_800DEA68;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)e = p;
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
                          (s16)func_800A3820(6));
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
                *(s16 *)(p + 0xA) = 0xF0;
                break;
            }
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        b->unk_00 = *(s32 *)(*(u8 **)(w + 0x8) + 0x0);
        b->unk_04 = *(s32 *)(*(u8 **)(w + 0x8) + 0x4);
        b->unk_08 = *(s32 *)(*(u8 **)(w + 0x8) + 0x8);
        i = __builtin_abs(*(s8 *)(s + 0x72) - *(u8 *)(o + 0x24));
        j = __builtin_abs(*(s8 *)(s + 0x73) - *(u8 *)(o + 0x25));
        if (i < j)
        {
            i = j;
        }
        *(s16 *)(p + 0x50) = i * 4;
        b->unk_0C = dx << 20;
        b->unk_10 = dy << 20;
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        if (*(s16 *)(p + 0x50) >= 0)
        {
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
        }
        else
        {
            *(s16 *)(p + 0x50) = 6;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(u8 **)(s + 0x60) == NULL)
            {
                *(s16 *)(p + 0xA) = 0xFF;
            }
            else
            {
                *(s32 *)(*(u8 **)(s + 0x60) + 0x14) |= 0x100000;
                *(s16 *)(p + 0x50) = 0x14;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 3:
        h = (s16)func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                               *(u16 *)((u8 *)b + 0xA) - 0x30);
        for (i = 3; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)D_80045340);
                e = (u8 *)n + 0x20;
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s06_800248F0;
                *(s16 *)((u8 *)n->unk_08 + 0x2) =
                    (*(s16 *)(e + 0xC) = *(u16 *)((u8 *)b + 0x2)) + (rand() % 128) - 0x40;
                *(s16 *)((u8 *)n->unk_08 + 0x6) =
                    (*(s16 *)(e + 0xE) = *(u16 *)((u8 *)b + 0x6)) + (rand() % 128) - 0x40;
                *(s16 *)((u8 *)n->unk_08 + 0xA) =
                    (*(s16 *)(e + 0x10) = h) + (rand() % 64) - 0x80;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                *(s16 *)(q + 0x10) = 0x20;
                *(s32 *)(q + 0xC) = 0x101010;
                *(u8 **)(q + 0x0) = D_800DED70;
                *(u16 *)(q + 0x14) |= 0xC;
                *(s32 *)(q + 0x8) = *(s32 *)(D_800DED70 + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
        n = (UnkNode *)(*(u8 **)(s + 0x60) - 0x20);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)n->unk_08 + 0xA) +
            (h + 0x18 - *(s16 *)((u8 *)n->unk_08 + 0xA)) / 10;
        q = (u8 *)n->unk_0C;
        if (*(u16 *)(q + 0x1C) >= 0x21C1)
        {
            *(s16 *)(q + 0x1C) = 0x2000;
        }
        k = (rand() & 0x1FF) + 0x100;
        *(s16 *)(q + 0x1C) = *(u16 *)(q + 0x1C) + k;
        if (*(u16 *)(q + 0x1E) < 0x76C)
        {
            *(s16 *)(q + 0x1E) = 0x800;
        }
        k2 = (rand() & 0xFF) + 0x80;
        *(s16 *)(q + 0x1E) = *(u16 *)(q + 0x1E) - k2;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(u8 **)(s + 0x60) == NULL)
            {
                *(s16 *)(p + 0xA) = 0xFF;
            }
            else
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 8, *(u8 *)(p + 0x9), 4, *(s16 *)(s + 0x2A), (u32)s,
                              2);
                *(s16 *)(p + 0x50) = 6;
                *(s16 *)(p + 0xA) = 0xF0;
            }
        }
        break;
    case 0xF0:
        q = *(u8 **)(*(u8 **)(s + 0x60) - 0x14);
        *(s16 *)(q + 0x1C) = *(u16 *)(q + 0x1C) + ((0x1000 - *(u16 *)(q + 0x1C)) >> 2);
        *(s16 *)(q + 0x1E) = *(u16 *)(q + 0x1E) + ((0x1000 - *(u16 *)(q + 0x1E)) >> 2);
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(q + 0x1E) = 0x1000;
            *(s16 *)(q + 0x1C) = 0x1000;
            *(s16 *)(p + 0xA) = 0xFF;
            *(s32 *)(*(u8 **)(s + 0x60) + 0x14) &= 0xFFEFFFFF;
        }
        break;
    case 0xFF:
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

/* The actor's hit-reaction poke: if the party record at p + 0x60 exists and
   its +0x14 flags say 0x1 without 0x20, set 0x20 at +0x1C and report it
   through func_800A48F0 with kind 4 and a random 0x20..0x5F code.
   30 words, 0 differing, first compile. No carved sibling at any ratio and
   no donor; written off the listing.

   TWO MEASURED FACTS:

   * THE TWO BIT TESTS MUST BE NESTED IFS, NOT `&&' -- 2 words SHORT and 10
     aligned rows. Nested is 0; `(f & 0x20) == 0 && (f & 0x1) != 0' across a
     single `&&' is 28 words / 10 rows, because `fold_truthop' merges two bit
     tests on ONE operand into a single `and' and the target has two separate
     `andi'/branch pairs. This is the levers file's entry read in the
     diagnostic direction: two `andi' in the listing means the source nested
     them.

   * THE RECORD IS RE-DEREFERENCED, NOT CACHED. `*(u8 **)(p + 0x60)' is
     written out at all four sites and cse unifies the first three -- an `if'
     arm that is a fall-through is the same cse block as its gate
     (cse.c:8086) -- while the `rand' call invalidates it so the fourth
     reloads, which is the `lw $a0, 0x60($s0)' the listing has before the
     func_800A48F0 call. A pointer local would have to live across that call
     and the prologue saves only $s0, which holds `p'.

   func_800A48F0 is declared through the unit-local WIDE alias
   func_800A48F0_i(u8 *, s32, s32); the project's landed `s8' prototype would
   truncate the third argument and the listing passes `ori $a2, $v0, 0x20'
   with no sign-extension. func_80069EF8 is `rand'. */
void func_s06_800247E8(u8 *p)
{
    if (*(u8 **)(p + 0x60) != 0)
    {
        if ((*(s32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x20) == 0)
        {
            if ((*(s32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x1) != 0)
            {
                *(s32 *)(*(u8 **)(p + 0x60) + 0x1C) |= 0x20;
                func_800A48F0_i(*(u8 **)(p + 0x60), 4, (rand() & 0x3F) | 0x20);
            }
        }
    }
}

/* The step function func_s06_80024004's FIRST spawn loop installs: shrink
   the renderable's +0x1E scale by 0x80 and copy it across to +0x1C, mark the
   owning node's +0x52 with 0x8000, hand the renderable to func_800478B8, and
   when its +0x14 reports 0x6000 retire the owning actor and set the same bit
   in D_800814A0.
   36 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.861,
   36 words, +0); func_s23_800241D8 and func_s70_80024ACC score higher and are
   the same shape. Deltas off this listing: the +0x1E step is -0x80 rather
   than +0x800, and this copy marks the node's +0x52 where s13 bumps a +0x14
   counter.

   THE TWIN'S READ-BACK FACT HELD, re-priced here: the scale pair is
   `*(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);' at 36 words / 0 differing;
   written as a second sum off +0x1C it is 38 words / 7 aligned rows -- two
   words LONG, because the +0x1C load cannot be cse'd against the value just
   stored at +0x1E and costs its own `lhu'. s13 records 37 words / 6 rows for
   the same mistake, so the direction reproduced and the magnitude did not
   quite. The target has ONE `lhu 0x1E' and two `sh' of the same register.

   The `b' parameter is unused here and is kept so this unit's three
   handlers share one (u8 *, UnkVec *, u8 *) signature, which is also
   func_s13_8002418C's. */
void func_s06_80024860(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x80;
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The step function func_s06_80024004's SECOND spawn loop installs: ease the
   node's three position halfwords a twelfth (and a tenth, on z) of the way
   towards the targets parked at the owner's +0xC/+0xE/+0x10, add 0xC0C0C to
   the renderable's +0xC colour word, mark the owning node's +0x52 with
   0x8000, hand the renderable to func_800478B8, and once the low byte of
   that colour word has wrapped past 0x90 retire the owning actor and set
   0x8000 in D_800814A0.
   75 words, 0 differing, first compile. Shares the +0x52 mark and the
   D_800814A0 teardown with func_s06_80024860 above, which is where those
   came from; no twin of its own at any ratio.

   UNPRICED but read straight off the listing, recorded so the next reader
   does not re-derive them:

   * THE DIVISORS ARE 12, 12 AND 10, NOT 6, 6 AND 5. All three are highpart
     multiplies; 0x2AAAAAAB with a post-shift of 1 is /12 (/6 takes `mfhi'
     with no shift) and 0x66666667 with a post-shift of 2 is /10. The
     `sra $a0, $a0, 31' inside each sequence is the tell that the dividend is
     SIGNED, which is why the subtrahend is `*(s16 *)' and the destination's
     read-back is `*(u16 *)' -- one `lh' and one `lhu' of the same address
     per component, which is what the listing has.

   * `*(u8 *)(c + 0xC) >= 0x91' EMITS `sltiu' WITH NO CAST. combine proves
     the zero-extended byte non-negative and rewrites the signed compare as
     unsigned, so no `0x91u' or `(u32)' is needed. The byte read is the low
     byte of the +0xC word the line above just added 0xC0C0C to. */
void func_s06_800248F0(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)((u8 *)b + 0x2) =
        *(u16 *)((u8 *)b + 0x2) + (*(s16 *)(p + 0xC) - *(s16 *)((u8 *)b + 0x2)) / 12;
    *(s16 *)((u8 *)b + 0x6) =
        *(u16 *)((u8 *)b + 0x6) + (*(s16 *)(p + 0xE) - *(s16 *)((u8 *)b + 0x6)) / 12;
    *(s16 *)((u8 *)b + 0xA) =
        *(u16 *)((u8 *)b + 0xA) + (*(s16 *)(p + 0x10) - *(s16 *)((u8 *)b + 0xA)) / 10;
    *(s32 *)(c + 0xC) += 0xC0C0C;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    if (*(u8 *)(c + 0xC) >= 0x91)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
