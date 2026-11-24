#include "common.h"

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;
void func_800478B8(u8 *);

void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
void func_s10_80024048(u8 *, UnkVec *, u8 *, u8 *);
void func_s10_80024224(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s10_80024CFC(u8 *, UnkVec *, u8 *);
void func_s10_80024E44(u8 *, UnkVec *, u8 *);
void func_s10_80024EF8(u8 *, UnkVec *, u8 *);
s32 func_800644B8(s32);
s32 func_80064584(s32);
void func_s10_800246AC(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_800DEB28[];
extern u8 D_80045340[];
extern u8 D_800DE870[];
extern u8 D_800DE8E8[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024048);

/* Spawn the 0x212 node that carries this effect's 0x1000-scale sprite with a
   random start phase: D_800DE8E8 as its dresser, D_80045340 through
   func_8004491C, func_s10_80024048 as its per-frame step, the caller's record
   at +0x2C, and `rand() % 0x1000' into the renderable's +0x1A rotation.
   Returns the node, or NULL when the allocator refuses.
   61 words, 0 differing.

   TWIN: the landed func_s27_800241F8 in csrc/ovl_dng_s27.c (dngtwin 0.824,
   58 words) by way of func_s25_80024148 in csrc/ovl_dng_s25.c, which is the
   same 0x212 spawner. func_80069EF8 IS `rand' -- splat/common_syms.txt names
   it and it needs no declaration; the `bgez'/`addiu 0xFFF'/`sra 12'/`sll
   12'/`subu' run is a signed `% 0x1000', not a mask, exactly as
   func_s25_800240C4 records.

   THE TWIN'S ONE STATED FACT IS NEITHER HELD NOR INVERTED HERE -- IT IS A
   THIRD SHAPE. The twin insists the NULL exit be an `else' arm AFTER the
   whole gate with a trailing `return n' (and prices the early return at 5
   aligned). On this body all three forms are distinguishable and only one
   lands; measured here, aligned differences:

     `if (n != NULL) { ...; return n; } return NULL;'   61 w,  0   kept
     `if (n == NULL) { return NULL; } ...; return n;'   61 w,  5   (41 by
                                                        ovlverify)
     the twin's `else' arm with a trailing `return n'   62 w,  7

   The 62-word row is the tell: with `return n' outside the gate the body
   ends in a jump to the return label AND the `addu $v0,$s1,$zero' needs its
   own block, so jump.c cannot fold the two and the function is a word long.
   Note the sibling func_s10_800242A0 below takes EITHER of the first two at
   0 differing -- the discriminator is this body's `j .Ls10_80024204' tail,
   which only the `return n'-inside-the-gate form produces. */
UnkNode *func_s10_80024130(u8 *v, UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s10_80024048;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x2C) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DE8E8, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
        return n;
    }
    return NULL;
}

/* The 0x212 sprite's own step function: bump the +0x16 counter on the node
   at the actor's +0xC, hand the renderable to func_800478B8, and when its
   +0x14 reports 0x6000 mark the owning actor retired and set the same bit in
   D_800814A0. 31 words, 0 differing, first compile.

   This is the already-landed func_s10_80024374 four words lower in this file
   plus the counter bump, which is exactly what csrc/ovl_dng_s27.c's
   func_s27_8002417C is (dngtwin 0.968) -- the only delta is that the counted
   node is at the actor's +0xC and its counter at +0x16, where the twin uses
   +0x0 and +0x14. That twin's note about D_800814A0 being the PLAIN scalar
   rather than ovl_main_8008A288's array alias holds here too and is what
   gives the `lui' / `lw %lo' pair.

   Measured and FREE at 31 words / 0 differing, so it is NOT why this lands:
   hoisting `*(u8 **)(p + 0xC)' into a pointer local instead of naming it
   twice. cse unifies the two loads either way -- the listing has one
   `lw 0xC($s1)' -- which is the same free that func_s25_800240C4 records. */
void func_s10_80024224(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(s16 *)(*(u8 **)(p + 0xC) + 0x16) = *(u16 *)(*(u8 **)(p + 0xC) + 0x16) + 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that carries this effect's 0x1000-scale sprite:
   white renderable, D_800DE8E8 as its dresser, D_80045340 through
   func_8004491C and func_s10_80024224 as its per-frame step, with the
   caller's record saved at +0x2C and the position copied off q. Returns the
   node, or NULL when the allocator refuses. 53 words, 0 differing.

   TWIN: the landed func_s25_80024148 in csrc/ovl_dng_s25.c (dngtwin 0.955,
   58 words) and func_s27_800241F8 in csrc/ovl_dng_s27.c (0.937, same 58).
   Both are 5 words longer and the 5 are MISSING STATEMENTS, not missing
   references: the `+0x12' sprite-id store (2 words) and the `+0x14 |= 0x100'
   read-modify-write (3). The relink also moves the caller's record from
   +0x20 to +0x2C.

   THREE FACTS MEASURED ON THIS BODY, both spellings priced (aligned
   differences at 53 words unless noted):

     the +0x2C store THIRD inside the gate, after the step pointer
       and the `m = n->unk_0C' load                               0   kept
       ... FIRST inside the gate                           51 w, 11
     a SEPARATE local for the `n->unk_08' pointer                 0   kept
       ... reusing `m'                                            5
     the NULL exit as an `else' arm after the gate                0   kept
       ... `if (n == NULL) { return NULL; }' first                5
       ... `return n' inside the gate, `return NULL' after        0

   The +0x20-versus-+0x2C store position and the separate `m2' reproduce
   func_s27_800241F8's own figures (10 and 5) to within one. The gate is the
   interesting one: BOTH the `else' arm and `return n'-inside-the-gate land
   here, where the sibling func_s10_80024130 above accepts only the latter --
   so this body's price table does NOT settle the gate and the twin's fact is
   WEAKER THAN STATED on this copy. */
UnkNode *func_s10_800242A0(u8 *v, UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s10_80024224;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x2C) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DE8E8, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
    }
    else
    {
        return NULL;
    }
    return n;
}

/* 27 words, 0 differing. */
/* The 0x312 spark's own step function: hand the renderable to func_800478B8,
   and when its +0x14 reports 0x6000 mark the owning actor retired and set the
   same bit in D_800814A0. No xsibs twin at ratio 0.40; written from the
   listing. D_800814A0 is the PLAIN scalar here, not ovl_main_8008A288's
   `D_800814A0_a[0]` array alias -- the `lui`/`lw %lo` pair is what the
   non-array spelling emits. 27 words, 0 differing, first compile. */
void func_s10_80024374(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same spawner for this effect's 0x2000-scale sprite: the resident
   D_800DE870 as its dresser, the already-landed func_s10_80024374 as its
   per-frame step, and NO caller record -- this one takes the position vector
   ALONE, which is the four words func_s10_800242A0 spends on `v' (its own
   `addu', its save, its restore and the +0x2C store). 49 words, 0 differing,
   AND IT LANDED BECAUSE func_s10_800242A0 WAS ALREADY THERE: same body, same
   declarations, nothing new.

   Measured here, aligned differences at 49 words:

     the NULL exit as an `else' arm after the gate                0   kept
       ... `return n' inside the gate, `return NULL' after        0
       ... `if (n == NULL) { return NULL; }' first                5

   Same as func_s10_800242A0: two of the three forms land, so this body does
   not settle the gate either. See func_s10_80024130 for the one body in this
   unit that does. */
UnkNode *func_s10_800243E0(UnkVec *q)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s10_80024374;
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DE870, 0);
        *(s16 *)(m + 0x1E) = 0x2000;
        *(s16 *)(m + 0x1C) = 0x2000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        *(s16 *)(m2 + 0x2) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m2 + 0x6) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m2 + 0xA) = *(u16 *)((u8 *)q + 0xA);
    }
    else
    {
        return NULL;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800244A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024550);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_800246AC);

/* Spawn the 0x212 node for one of the ring's eight frames: the caller's
   position vector into the node payload at +0x20, the frame index's
   0x100-per-step angle at +0x28, the renderable dressed from D_800DEB28 at
   frame `7 - k % 8', darkened to `~(k << 3)', scaled `k * 64 + 0x1000',
   given the +0x10 0x20 and +0x14 0xC bits and published through D_80045340;
   then the mover at n->unk_08 is placed at the payload position offset by
   the angle's sine and cosine and lowered by `k << 3'. 104 words,
   0 differing.

   NO TWIN. dngtwin.py --unit ovl_dng_s10 reports no carved sibling at 0.70
   and xsibs.py is useless on an s-unit. The route in was the callee-set grep
   on func_800644B8 / func_80064584, which is where the two-statement
   `sra'/`sll'/`subu' modulos and the `>> 4' scaling idiom come from.

   THE WHOLE FUNCTION IS THE SECOND PARAMETER'S TYPE. `s16 k', used
   unqualified at every site, is what lets combine drop the sign extension at
   the two narrowing uses (`~(k << 3)' into a u8 and `- (k << 3)' into an
   s16, both off the RAW register) while keeping it at the three that need
   the signed value (`% 0x1000', `% 8', `* 64'). That is also what supplies
   the target's `addu $s5, $s4, $zero' -- the parameter and its promoted copy
   are two live pseudos, which is the seventh callee-saved register and so
   the `-0x30' frame. Measured, aligned differences:

     `s16 k' parameter, no narrowing local            104 w,  0   kept
       ... `s32 kp' parameter with an `s16 k = kp'    101 w, 11
     `k * 64 + 0x1000' for the scale                  104 w,  0   kept
       ... `(k << 6) + 0x1000'                        102 w, 29
     `m = (u8 *)n + 0x20' as a pointer local          104 w,  0   kept
       ... `(u8 *)n + 0x20/0x22/0x24/0x28' inline     102 w, 21
     `(s16)' cast on func_8003DB94's frame argument   104 w,  0   kept
       ... the bare `7 - k % 8'                       104 w,  9
     the +0x1E scale store BEFORE the +0x1C one       104 w,  0   kept
       ... the two swapped                            104 w,  2

   The `k * 64' row is convert.c's narrowing rule from docs/matching-levers.md
   firing in the direction the file predicts: the store is to `*(s16 *)', the
   LSHIFT_EXPR arm is guarded on TREE_UNSIGNED and MULT_EXPR's is not, so the
   shift gets narrowed and loses the sign-extended operand while the multiply
   keeps it -- the diagnostic is `sll $v0,$s4,6' where the target has
   `sll $v0,$s2,6'.

   MEASURED AND FREE, so neither is why this lands: the three colour bytes as
   one chained `r[0xC] = r[0xD] = r[0xE] = ~(k << 3);' rather than three
   statements in 0xE/0xD/0xC order (104 / 0 either way), and an `s16 j = k'
   copy used at the colour site, which cse deletes (104 / 0). */
UnkNode *func_s10_80024790(UnkVec *q, s16 k)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s10_800246AC;
        m = (u8 *)n + 0x20;
        *(s16 *)(m + 0x0) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m + 0x4) = *(u16 *)((u8 *)q + 0xA);
        *(s16 *)(m + 0x8) = (k << 8) % 0x1000;
        r = (u8 *)n->unk_0C;
        r[0xE] = ~(k << 3);
        r[0xD] = ~(k << 3);
        r[0xC] = ~(k << 3);
        func_8003DB94(r, D_800DEB28, (s16)(7 - k % 8));
        *(s16 *)(r + 0x1E) = k * 64 + 0x1000;
        *(s16 *)(r + 0x1C) = k * 64 + 0x1000;
        *(u16 *)(r + 0x10) |= 0x20;
        *(u16 *)(r + 0x14) |= 0xC;
        func_8004491C(n, (u32)D_80045340);
        w = (u8 *)n->unk_08;
        *(s16 *)(w + 0x2) = *(u16 *)(m + 0x0) + ((func_800644B8(*(s16 *)(m + 0x8)) << 1) >> 8);
        *(s16 *)(w + 0x6) = *(u16 *)(m + 0x2) + ((func_80064584(*(s16 *)(m + 0x8)) << 1) >> 8);
        *(s16 *)(w + 0xA) = *(u16 *)(m + 0x4) - (k << 3);
        return n;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024930);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024CFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024E44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80024EF8);

/* The effect's dresser callback: states 3, 4 and 5 of the node's +0xA hand
   the first three arguments straight through to func_s10_80024CFC /
   func_s10_80024E44 / func_s10_80024EF8; every other state does nothing.
   Always returns 0. 28 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s10_80024008: EIGHT `.word's = SEVEN CASES PLUS
   ONE PAD. The dispatch bound `sltiu $v0, $v1, 0x7' is the authority and
   tools/decomp/jtcheck.py agrees (`8 words = 7 cases + 1 pad   bound 7
   4 distinct target(s)'); the eighth word is the `.align 3' filler an odd
   case count needs, not a case. Entries, in order:

     0 .Ls10_80025080   1 .Ls10_80025080   2 .Ls10_80025080
     3 .Ls10_80025058   4 .Ls10_80025068   5 .Ls10_80025078
     6 .Ls10_80025080   pad 0x00000000

   .Ls10_80025080 is the epilogue, so 0, 1, 2 and 6 are `break' arms.

   THE FOUR EMPTY CASES ARE LOAD-BEARING. With only 3, 4 and 5 written, three
   labels are under gcc 2.7.2's case_values_threshold, expand_end_case emits
   a COMPARISON TREE instead of the table -- 32 words, 19 aligned
   differences -- and the unit loses the rodata table the ROM has. Spelling
   0..2 and 6 as empty `break' arms puts the range at 0..6, which is what
   gives the bare `sltiu $v0, $v1, 0x7' with no minval subtraction. This is
   csrc/ovl_dng_s27.c:func_s27_80024DF4's fact reproducing to within one row
   (it records 32 words / 19 aligned for a 0..7 table).

   THE ARM ORDER IS HAND-SETTLED, NOT MEASURED. ovlverify masks a `jal's
   target field, so exchanging two of these arms is invisible to it, and
   relcand compares the call SET rather than the sequence, so it reads `ok'
   either way (it does: 0 data symbols, 3 calls). What pins the order is the
   table's entry ADDRESSES, since the arms emit in SOURCE order:
   case 3 at 0x80025058 < case 4 at 0x80025068 < case 5 at 0x80025078, so
   ascending. Case 5 is independently pinned as LAST by measurement, because
   its arm has no trailing `j'. Confirmed by compiling this unit standalone
   with tools/cdkgcc -S: the emitted table is
   $L46,$L46,$L46,$L50,$L51,$L52,$L46 with $L50/$L51/$L52 in that order in
   .text and $L52 falling through into $L46.

   THE THREE-ARGUMENT CALLS ARE NOT MEASURABLE AND COME FROM THE CALLEES'
   PROLOGUES. This function sets none of $a0/$a1/$a2 at any arm, and all
   three callees read them (func_s10_80024CFC takes $a0/$a1, the other two
   $a0/$a1/$a2), so it passes its own first three parameters through
   unchanged. Written `func_s10_80024CFC()' with no arguments the body is
   also 28 words / 0 differing, i.e. the tools cannot see the difference --
   that is NOT a free fact, it is an unmeasurable one. */
s32 func_s10_80025020(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
    case 1:
    case 2:
        break;
    case 3:
        func_s10_80024CFC(p, b, c);
        break;
    case 4:
        func_s10_80024E44(p, b, c);
        break;
    case 5:
        func_s10_80024EF8(p, b, c);
        break;
    case 6:
        break;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s10", func_s10_80025090);
