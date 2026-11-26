#include "common.h"

s32 func_lo20_800258B0(u8 *p, void *b, void *c, u8 *d);

u32 func_800491F4(s32, u8 *, s32);
s32 func_80049DE8(s32, s32, s32);
u8 *func_8004DC14(u8 *, s32);
void func_8004E99C(s32);
void func_lo20_80025D9C(u8 *p, u8 *b, u8 *c, u8 *d);
void func_lo20_80025E3C(u8 *, s32);
void func_lo20_80025FFC(u8 *p, s32 b, s32 c, s32 d);
void func_lo20_800267D8(u8 *p, u8 *b, u8 *c, u8 *d);
void func_lo20_80026878(u8 *p, s32 b);
void func_lo20_8002692C(u8 *p, s32 b, s32 c, s32 d);
void func_lo20_80026B44(u8 *p);
s32 func_lo20_80026C20(u8 *);
void func_lo20_80028050(u8 *, u8 *, u8 *, u8 *);
void func_lo20_800280FC(u8 *, s32);
extern char *D_8002946C[];
extern void (*D_800294D0[])(void *);
extern u8 D_800294E8[];
extern u8 D_800294F8[];
extern u8 D_80029500[];
extern u8 D_80029534[];
extern u8 D_80029544[];
extern u8 D_800789B8[];
extern u8 D_lo20_80027F34[];
extern u8 D_lo20_800287EC[];
extern u8 D_lo20_80025548[] __asm__("func_lo20_8002553C + 0xC");

void func_8004B1A4(s32);
void func_8004B248(u32 *);
u32 func_8004B404(s32);
u8 *func_8004EAC4(u8 *, u8 *, s32);
u8 *func_800499E8(u8 *, u8 *, u8 *);
char *func_lo20_80025040_4(char *, s32, s32, s32) __asm__("func_lo20_80025040");
s32 func_lo20_80027008(s32, s32, s32);
s32 func_lo20_8002714C(s32, s32, s32, s32);
void func_lo20_80027328(u8 *, s32);
void func_lo20_80028534(u8 *p);
u8 *func_lo20_80028774(u8 *p);
extern u8 D_800294C4[];
extern u32 D_800294E4;
extern u32 D_800294FC;
extern u8 D_80029550[];
extern u8 D_lo20_80025034[];
extern u8 D_lo20_800266B0[];

extern s32 D_80029508[];
void func_8004A7D8(s32, s32);
extern u8 D_lo20_80027D7C[];
void func_lo20_80028648(u8 *);
u8 *func_80049004(u8 **, s32, s32);
s32 func_80048FBC(u8 *, s32);
extern u8 D_lo20_80025BF4[];
u8 *func_80048EE4(u8 **, s32);
void func_800491CC(u32 **, u32, s32);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8001E814);

/* 16 words, 0 differing.  First compile.
   Measured and FREE, so neither is why this lands: returning `dst' rather
   than strcpy's own result (strcpy already leaves it in $v0, so the
   `addu $v0, $s0, $zero' is not evidence either way). */
char *func_lo20_80025040(char *dst, s32 i)
{
    strcpy(dst, D_8002946C[i]);
    return dst;
}

/* 13 words, 0 differing.  First compile.
   Measured and FREE at 13/0, so it is NOT the reason this lands: the `(u32)'
   cast on strlen's result.  `strlen(...) / 2' gives the same `srl $v0, $v0, 1'
   -- strlen's declared return type already makes the divide unsigned here, so
   do not spend the narrow-call-result lever on it.  (declcheck is clean, so
   this is a real FREE and not the undeclared-callee trap.) */
s32 func_lo20_80025080(s32 i)
{
    return (u32)strlen(D_8002946C[i]) / 2;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800250B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025170);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025198);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800253C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025514);

/* 3 words, 0 differing.  First compile.  A plain accessor; the `nop' in the
   delay slot is the load delay, not something to fill. */
s32 func_lo20_80025530(u8 *p)
{
    return *(s32 *)(p + 0x44);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002553C);

/* 5 words, 0 differing.  First compile.  Installs the handler at 0x80025548
   -- see the D_lo20_80025548 __asm__ alias in the declaration block, which is
   how the unnamed second function inside func_lo20_8002553C's listing is
   reached.  Verified with `ovlverify --raw' plus `objdump -r': the two words
   carry R_MIPS_HI16/LO16 against func_lo20_8002553C with 0xC in the
   immediate, matching splat's `%hi(func_lo20_8002553C + 0xC)'. */
void func_lo20_80025598(u8 *p)
{
    *(s32 *)(p + 0x2C) = 0;
    *(u8 **)(p + 0x10) = D_lo20_80025548;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800255AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800255FC);

/* 14 words, 0 differing.  First compile.
   NOT MEASURABLE (not free): whether `p' is passed to the table's handler.
   $a0 is never written, so a call with no arguments emits the same words. */
void func_lo20_80025850(void *p, s32 i)
{
    D_800294D0[i](p);
}

/* 10 words, 0 differing.  First compile.
   THREE spellings measured, ALL 10 words / 0 aligned differences, so the
   target does not constrain the choice here: reusing the parameter `v' as
   written, a separate `s32 k' local, and the expression `v * 16 - 0x38'
   written out at both stores.  cse unifies it either way.  Do not read the
   shared register as evidence of a source local. */
void func_lo20_80025888(u8 *p, s32 v)
{
    v = v * 16 - 0x38;
    *(s16 *)(*(u8 **)(*(u8 **)(p + 0x4) + 0x4) + 0xA) = v;
    *(s16 *)(*(u8 **)(*(u8 **)(p + 0x8) + 0x4) + 0xA) = v;
}

/* A stub that always reports "no": two words, `jr $ra' with
   `addu $v0, $zero, $zero' in the delay slot.  Replicated by dngclone from
   func_b55_80172328 (csrc/ovl_dng_b55.c), which is the same two words.
   2 words, 0 differing.

   Its caller here is func_lo20_80025A0C, still an INCLUDE_ASM, which is why
   the four parameters are the ones the pre-existing forward declaration in
   this unit names even though the body reads none of them.

   (The b55 original's comment cited ITS caller, func_b55_80171BE0, and
   dngclone carried that sentence over while renaming only the symbols in its
   own map -- so the replicated comment named another unit's caller as if it
   were local.  Rewritten here; see docs/matching-levers.md.) */
s32 func_lo20_800258B0(u8 *p, void *b, void *c, u8 *d)
{
    return 0;
}

/* 43 words, 0 differing, after eleven spellings.  The +0x14 record's inner
   block gets the caller's slot index published into the resident
   D_80073414 descriptor's +0x0C table (`Unk80073414' is 0x14 bytes, which is
   the listing's `sll 2'/`addu'/`sll 2'); kind 0x13 and an empty slot both
   fall out to the name copy instead.  func_80069E78 is `strcpy'
   (splat/common_syms.txt).

   THE FACT, worth 13 aligned differences, and it is a MACRO-RESIDUE
   `do { } while (0)' around the else arm.  The residue without it is a clean
   whole-body $s0/$s1 exchange with every instruction in place: `p' wants
   $s0 and gets $s1.  `-da' gives the margin outright, and it is 6%:

       p   6 refs across 28 insns   floor_log2(6)*6/28 = 4285
       q   5 refs across 22 insns   floor_log2(5)*5/22 = 4545

   so `allocno_order' comes out `81 80' (q first, q takes $s0).  The cure is
   one more reference on `p', and flow.c:2122 counts a reference once per
   ENCLOSING LOOP, so the `do'-wrapper round the two-p-reference strcpy call
   takes p to 8 refs and 24/28 = 8571.  THE SCOPE IS FORCED -- this is not a
   case where several scopes measure 0:

       do { } round the else arm (the strcpy) only      43w /  0
       do { } round the work arm instead                43w / 23
       do { } round the whole if/else                   44w / 38
       do { } round the `q = ...' assignment            44w / 42

   The last two are worse than no device at all because they also bump `q',
   which is the wrong side.  Everything plain was tried first and all of it
   leaves the identical 13 rows, which is the signature of one remaining
   fault: the gate as `||' with the arms swapped (44w/37), as `||' with an
   early return (44w/37), as two `goto's (13), the final add spelled
   int-first (13), a separate `r = *(u8 **)(p + 0x14)' local (13), and a
   local for the strcpy destination (13).  The duplicated else arm -- the
   non-device way to buy references -- is 44 words / 30 here, because jump2
   does NOT cross-join two copies that sit in different arms of an
   `else if' chain.
   Called a macro and not a device because `strcpy(*(char **)(p + 0x88),
   p + 0x78)' is a whole coherent unit of work (publish the name) and that is
   exactly the boundary a macro would have. */
void func_lo20_800258B8(u8 *p)
{
    u8 *q;

    q = *(u8 **)(p + 0x14) + 0x20;
    if (*(s32 *)(p + 0xC) != 0x13 && *(s32 *)(q + 0x8) != 0)
    {
        func_8004A7D8(*(s32 *)(p + 0xC), *(s32 *)(q + 0x1C));
        *(s32 *)(D_80073414[*(s32 *)(p + 0xC)].unk_0C + *(s32 *)(q + 0x1C) * 0x14 + 0xC) =
            *(s32 *)(q + 0x8);
    }
    else
    {
        do
        {
            strcpy(*(char **)(p + 0x88), (char *)(p + 0x78));
        } while (0);
    }
}

/* 26 words, 0 differing.  First compile.
   LOAD-BEARING and worth 1 word / 14 aligned differences: the gate is
   `if (func_lo20_80026C20(...) != 0)' with the `= 0' store as the THEN arm
   and the D_800294C4 store as the `else'.  Inverted, the whole block moves
   and the `lui' that the target lifts into the `beqz' delay slot no longer
   reaches it.
   The two arms each recompute `*(u8 **)(p + 0x54) + 0x30'; that is not a
   repetition to hoist -- an arm boundary is a new cse block, so hoisting it
   would make the body shorter than the target. */
void func_lo20_80025964(u8 *p)
{
    if (func_lo20_80026C20(*(u8 **)(*(u8 **)(p + 0x18) + 0x28)) != 0)
    {
        **(s32 **)(*(u8 **)(p + 0x54) + 0x30) = 0;
    }
    else
    {
        **(u8 ***)(*(u8 **)(p + 0x54) + 0x30) = D_800294C4;
    }
}

/* 16 words, 0 differing.  First compile.  The `sltiu $a1, $v0, 0x1' is
   `== 0' on the call's result, and the gate falls through into the then-arm,
   so the source test is `== 4' and not its inverse. */
s32 func_lo20_800259CC(u8 *p)
{
    s32 r;

    r = 0;
    if (*(s32 *)(p + 0x8) == 4)
    {
        r = func_lo20_80026C20(*(u8 **)(*(u8 **)(p + 0x18) + 0x28)) == 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025A0C);

/* 3 words, 0 differing.  First compile. */
void func_lo20_80025C4C(u8 *p)
{
    **(s32 **)(p + 0x4) = 0;
}

/* 5 words, 0 differing.  First compile. */
void func_lo20_80025C58(u8 *p)
{
    **(u8 ***)(p + 0x4) = D_800789B8;
}

/* 27 words, 0 differing, two compiles.  func_lo20_80025CD8's record build
   over five 0xC-byte slots, with the +0x4 word from D_800294E4 and the
   0x80 flag written to the LAST slot after the loop.

   THE FACT, worth 7 aligned differences: the slot address must be
   RECOMPUTED in the loop body as `q = p + i * 12', not carried in a cursor
   advanced by `q += 0xC'.  With the cursor, loop.c picks the giv for the
   HIGHEST offset in the body as the representative and the biv comes out
   `addiu $v1, $a0, 0xB' with every displacement 11 lower (-10, -9, -8, -7,
   -1, 0 against the target's 1, 2, 3, 4, 0xA, 0xB).
   Measured and FREE at 27/0: the post-loop store as `p[i * 12 - 12]'
   instead of `p[(i - 1) * 12]' -- the same tree after fold, and either way
   the -0xC lands in the `sb' displacement. */
u8 *func_lo20_80025C6C(u8 *p)
{
    s32 i;
    u8 *q;

    for (i = 0; i < 5; i++)
    {
        q = p + i * 12;
        q[1] = 0x28;
        *(u32 *)(q + 0x4) = D_800294E4;
        q[2] = 2;
        q[3] = i * 16 - 0x3E;
        q[0xA] = 0xC;
        q[0xB] = 0xC;
    }
    p[(i - 1) * 12] = 0x80;
    return p;
}

/* 20 words, 0 differing.  The landed func_lo20_80025D28's shape with
   different constants; its `u32 t[2]' fact reproduces here at 5 words / 20
   aligned differences (a plain `u32 t' is 15 words with no frame at all), so
   that is three bodies in this unit on the array-of-two lever.

   ONE OF ITS OWN, and it is new in this unit: `*(s8 *)(p + 0x2) = -0x18'
   rather than `p[2] = -0x18'.  Through a `u8' lvalue the constant converts to
   232 at compile time and cc1psx emits `addiu $v0, $zero, 0xE8'; the target
   has `addiu $v0, $zero, -0x18'.  1 aligned difference, and the same fact is
   priced again on func_lo20_80026708 and func_lo20_80027FF4. */
void func_lo20_80025CD8(u8 *p)
{
    u32 t[2];

    t[0] = 0;
    p[1] = 0x28;
    *(u32 *)(p + 0x4) = t[0];
    *(s8 *)(p + 0x2) = -0x18;
    p[3] = 4;
    p[0xA] = 0x30;
    p[0xB] = 0x58;
    p[0] |= 0x80;
}

/* 19 words, 0 differing, two compiles.  TWO facts, both measured here:

   1. `u32 t[2]' is load-bearing and worth 14 aligned differences / 5 words.
      The target stores 0xB8C8 to sp+0 and reads it back into a register
      before the word store, which only happens for an ADDRESSABLE local; a
      plain `u32 t' and the constant written inline both come out 14 words
      with no frame at all.  This is the "array of TWO" lever
      (docs/matching-levers.md): an array of one is register-allocated like a
      scalar.  The frame is 0x8, which is exactly two words.

   2. The word store must be the THIRD statement, right after `p[1] = 0x28'.
      The reload cannot be scheduled across the byte stores through `p' (gcc
      cannot prove the stack slot and `p' do not alias), so the `lw' sits
      where the source put it -- with the store written last it is 11
      differing.

   Measured and FREE at 19/0, so NOT why this lands: the position of
   `p[0] |= 0x80' (last, or immediately after the word store), and splitting
   the reload and the `p[0]' read into their own locals.  Three spellings,
   all 0. */
void func_lo20_80025D28(u8 *p)
{
    u32 t[2];

    t[0] = 0xB8C8;
    p[1] = 0x28;
    *(u32 *)(p + 0x4) = t[0];
    p[2] = 2;
    p[3] = 2;
    p[0xA] = 0xC;
    p[0xB] = 0xC;
    p[0] |= 0x80;
}

/* 10 words, 0 differing.  First compile.  One of four one-line wrappers round
   func_800491F4 in this unit (with func_lo20_800267B0, func_lo20_80027FA4 and
   func_lo20_80027FCC); they differ only in the string and the length.
   NOT MEASURABLE: whether the result is returned or dropped -- $v0 is already
   the callee's return register. */
u32 func_lo20_80025D74(s32 a)
{
    return func_800491F4(a, D_800294E8, 0xD);
}

/* 40 words, 0 differing.  The resident twin is csrc/ovl_town.c's
   func_800AEBC4 / func_800B1960 group -- the "template-copy constructor":
   copy the two resident GPU templates D_8002E5D8 (4 words) and D_8002E5E8
   (3 words) into the caller's two buffers, aim the pair's two prim pointers
   at them, then write the prim's own fields back THROUGH b + 0x4.
   The first parameter is untouched: $a0 is never read and the town group's
   first parameter is a separate object this member does not use.

   TWO facts, each worth 2 words / 32 aligned differences, and BOTH are
   needed -- either one alone leaves the same 42/32 residue:

   1. The destinations must be ARRAY REFS, `((u32 *)c)[1]', not plain casts
      `*(u32 *)(c + 0x4)'.  The array ref sets MEM_IN_STRUCT_P, so
      true_dependence stops the stores to c from blocking the SECOND
      template's address chain, and sched1 lifts `lui %hi(D_8002E5E8)' and
      its `lw %lo' / `addiu %lo' into the load-delay slots after
      `lw 0x8($v1)' and `lw 0xC($v1)'.  Through plain casts those three words
      sit after the whole first group and the two delays stay `nop' -- which
      is the 2 extra words.
   2. Element 0 must be read through the POINTER LOCAL, `s1[0]', not through
      the symbol `D_8002E5D8[0]'.  Both fold to `lw %lo(D_8002E5D8)($v1)',
      but through the symbol the local's own materialisation is a separate
      expansion and the same two delays go unfilled.
   Measured and FREE at 40/0: `u32 *s1 = D_8002E5D8;' as an initialiser
   against a separate `s1 = D_8002E5D8;' statement, and that statement's
   position across all five slots of the first group (flat sweep, so NOT
   MEASURABLE rather than free).
   `ovlverify --raw' is 0 differing. */
void func_lo20_80025D9C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    ((u32 *)c)[0] = s1[0];
    ((u32 *)c)[1] = s1[1];
    ((u32 *)c)[2] = s1[2];
    ((u32 *)c)[3] = s1[3];
    ((u32 *)d)[0] = s2[0];
    ((u32 *)d)[1] = s2[1];
    ((u32 *)d)[2] = s2[2];
    *(u8 **)(b + 0x4) = c;
    *(u8 **)(b + 0x8) = d;
    *(s32 *)b = 0;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x8) = 0x2C;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xA) = -0x8;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xC) = 0x200;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x6) = 0;
    (*(u8 **)(b + 0x4))[0xF] = 4;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025E3C);

void func_lo20_80025FF4(void)
{
}

/* 34 words, 0 differing.  func_lo20_8002692C's constructor shape: build the
   two sub-records at p + 0x74, publish the caller's four values and install
   the handler at p - 0x10.  func_80069E78 is `strcpy'
   (splat/common_syms.txt), so the fourth parameter is a string pointer even
   though this unit's pre-existing declaration calls it `s32'.

   THE FACT, worth 7 aligned differences, and it is a STATEMENT ORDER fault
   that presents as a clean $s2/$s3 exchange with nothing moved: the
   `p + 0xC' store (c) must be written BEFORE the `p + 0x14' store (b),
   although the listing emits 0x14 first -- sched1 transposes the two
   adjacent independent stores.  All 24 permutations of the four stores were
   measured; only this one reaches 0:
     0xC, 0x14, 0x18, 0x88   0   <- as written
     0x14, 0xC, 0x18, 0x88   7      (the listing's own order)
     0xC, 0x14, 0x88, 0x18   2
     0xC, 0x88, 0x14, 0x18   2
     0xC, 0x18, 0x14, 0x88   5
     the twelve orders starting 0x18 or 0x88   7-12
   So `-da' and a reference bump were both the wrong tool here; the whole
   residue is one adjacent pair's source order.
   Measured and FREE at 34/0: strcpy's second argument as a re-read
   `*(char **)(p + 0x88)' instead of `d' (cse forwards the store either way,
   so d keeps its reference count and the register assignment does not move).
   D_lo20_80025BF4 is an alabel inside func_lo20_80025A0C's listing, the same
   device as D_lo20_800266B0. */
void func_lo20_80025FFC(u8 *p, s32 b, s32 c, s32 d)
{
    func_lo20_80025CD8(*(u8 **)(p + 0x74));
    func_lo20_80025D28(*(u8 **)(p + 0x74) + 0xC);
    *(s32 *)(p + 0xC) = c;
    *(s32 *)(p + 0x14) = b;
    *(s32 *)(p + 0x18) = b + 0x20;
    *(s32 *)(p + 0x88) = d;
    strcpy((char *)(p + 0x78), (char *)d);
    *(u8 **)(p - 0x10) = D_lo20_80025BF4;
}

/* 18 words, 0 differing.  First compile.
   LOAD-BEARING and worth 4 aligned differences / 3 words: all four parameters
   are forwarded to func_lo20_80025FFC.  That callee saves $a0..$a3 into
   $s0..$s3 in its prologue, this unit's only caller of THIS function
   (func_lo20_80026168) sets all four, and forwarding them costs no
   instruction because they are already in place -- but spelling the call with
   constants instead adds three `addiu' and rotates a register.  This is the
   call-arity fact from the brief, in the direction where the arguments are
   invisible in the listing. */
void func_lo20_80026084(u8 *p, s32 b, s32 c, s32 d)
{
    func_lo20_80025FFC(p, b, c, d);
    func_lo20_80025D9C(p, p + 0x44, p + 0x1C, p + 0x2C);
    func_lo20_80025E3C(p, *(s32 *)(p + 0x54));
}

/* 39 words, 0 differing.  First compile.  Allocates the p + 0x58 handle
   table and its 6-entry index, copies the 3-word D_8002E5E8 template to
   p + 0x38 and publishes it; returns whether both allocations succeeded.
   `u8 *func_80049004(u8 **, s32, s32)' and `s32 func_80048FBC(u8 *, s32)'
   are both landed in csrc/unk_1C540.c.

   THE FACT, worth 1 word / 32 aligned differences: func_80049004 takes
   THREE arguments and the middle one is THIS function's own second
   parameter, forwarded.  $a1 is never written before the `jal', which reads
   as no argument at all -- but an explicit constant there is 40 words and
   32 rows, because the pass-through costs no instruction.  That is what
   settles the arity: a one-parameter reading cannot produce this listing.
   Also LOAD-BEARING, 3 aligned differences: `r = 1' must come AFTER the
   func_800491CC call, the same fact func_lo20_800269E8 prices at 2.
   Measured and FREE at 39/0, so NEITHER is why this lands:
     - a `u8 *q' local for p + 0x58 against writing it inline at both call
       sites (cse unifies it, so the `addu $a0, $s1, $zero' repeated for the
       second call is not evidence of a source local);
     - the two gates as one `&&' with the assignment inside it instead of
       nested `if's.  They test different addresses, so fold_truthop never
       applies. */
s32 func_lo20_800260CC(u8 *p, s32 n)
{
    s32 r;
    u8 *q;

    r = 0;
    q = p + 0x58;
    *(u8 **)(p + 0x54) = func_80049004((u8 **)q, n, 6);
    if (*(u8 **)(p + 0x54) != NULL)
    {
        *(s32 *)(p + 0x74) = func_80048FBC(q, 6);
        if (*(s32 *)(p + 0x74) != 0)
        {
            u32 *s2 = D_8002E5E8;

            *(u32 *)(p + 0x38) = s2[0];
            *(u32 *)(p + 0x3C) = s2[1];
            *(u32 *)(p + 0x40) = s2[2];
            func_800491CC(*(u32 ***)(p + 0x54), (u32)(p + 0x38), 0xD);
            r = 1;
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026168);

/* 17 words, 0 differing.  First compile.  The second `lw $v0, 0x14($s0)' is
   not a source repetition to avoid -- the call invalidates cse's entry, so
   one `**(s32 **)(p + 0x14)' per statement is what emits it. */
void func_lo20_80026250(u8 *p)
{
    func_8004E99C(**(s32 **)(p + 0x14));
    **(s32 **)(p + 0x14) = 0;
}

/* 22 words, 0 differing.
   THE FACT, and it is worth 1 word / 14 aligned differences, reproduced on
   func_lo20_80026B94 (15) and func_lo20_800285B0 (15): the halfword
   read-modify-write must be an ARRAY REF, `((u16 *)p)[0xF] |= 0x8000', not
   the plain cast `*(u16 *)(p + 0x1E) |= 0x8000'.  The array ref sets
   MEM_IN_STRUCT_P, true_dependence then lets the file-scope scalar
   D_800814A0's `lw' hoist ABOVE the `sh', and the two read-modify-writes
   interleave -- two loads, two `ori', two stores -- so neither pays a load
   delay.  Through the plain cast gcc cannot excuse the field against the
   fixed-address scalar, the D_800814A0 load serialises behind the `sh' and
   costs a `nop'.
   Also load-bearing, 4 aligned differences: p + 0x1E first, D_800814A0
   second. */
void func_lo20_80026294(u8 *p)
{
    if (p != NULL)
    {
        func_lo20_80026250(*(u8 **)(p + 0x74));
        func_8004B248((u32 *)(p + 0x78));
        ((u16 *)p)[0xF] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 10 words, 0 differing.  First compile.  TWO facts, both measured here and
   both reproduced on func_lo20_80026314 and func_lo20_80028620, which are the
   same shape:

   - the store must come BEFORE the call (9 aligned differences / 13 words
     after it).  reorg sinks it into the `jal' delay slot, so its position in
     the listing is not its position in the source -- but the direction is
     fixed, because after the call it cannot reach the delay slot at all.
   - the pointer local `q = p + 0x20' is worth 3 aligned differences against
     folding 0x20 into each displacement (`p + 0x30' / `p + 0x74'); the target
     has the `addiu $v0, $a0, 0x20'. */
void func_lo20_800262EC(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x10) = 0;
    func_lo20_80025C4C(*(u8 **)(q + 0x54));
}

/* 11 words, 0 differing.  First compile.  func_lo20_800262EC's shape with a 1
   stored instead of a 0 and a different callee; its two facts (store before
   the call, pointer local for `p + 0x20') were priced there. */
void func_lo20_80026314(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x10) = 1;
    func_lo20_80025C58(*(u8 **)(q + 0x54));
}

/* 12 words, 0 differing.  First compile.
   Measured and FREE at 12/0, so NOT why this lands: writing the constant 4
   twice against hoisting it into a local.  cse produces the
   `addu $a1, $v0, $zero' copy either way, so the shared register is not
   evidence of a source local. */
void func_lo20_80026340(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x8) = 4;
    func_lo20_80025888(*(u8 **)(q + 0x54), 4);
}

/* 6 words, 0 differing.  First compile.
   Measured and FREE at 6/0: `v * 8 - 0x18' against `(v << 3) - 0x18'.  Same
   tree for a power of two, as the brief's lever says. */
void func_lo20_80026370(u8 *p, s32 v)
{
    *(s16 *)(*(u8 **)(*(u8 **)(p + 0x4) + 0x4) + 0x8) = v * 8 - 0x18;
}

/* 14 words, 0 differing.  First compile.  The arguments are permuted: the
   callee gets (b, a, D_800294F8[c]), which is what the three entry copies in
   the listing say. */
s32 func_lo20_80026388(s32 a, s32 b, s32 c)
{
    return func_80049DE8(b, a, D_800294F8[c]);
}

/* 19 words, 0 differing.  First compile.  func_lo20_80026B44 takes ONE
   argument: the `addu $s0, $a1, $zero' in its delay slot is this function
   saving its own second parameter, not an argument being passed.  The
   unit's other caller (func_lo20_80026B94) sets only $a0, which is what
   settles it. */
void func_lo20_800263C0(u8 *p, u8 *q)
{
    func_lo20_80026B44(p);
    **(u8 ***)(p + 0xC) = func_8004DC14(q, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002640C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800264D4);

/* 27 words, 0 differing.  First compile.  func_lo20_800265B8's sibling for
   the empty case: 0x80069EE8 is `memset' (splat/common_syms.txt), so the
   first call clears q + 0x14 for `D_800294F8[q[0x10]] * 2 | 1' bytes.
   LOAD-BEARING, 2 aligned differences: the `*(s32 *)(q + 0x8) = 0' store is
   written BEFORE the func_lo20_80026370 call so reorg can sink it into the
   `jal' delay slot.
   Measured and FREE at 27/0: `* 2 + 1' instead of `* 2 | 1'.  combine knows
   the low bit of a doubled value is clear and emits `ori' for both. */
void func_lo20_8002654C(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    memset(*(u8 **)(q + 0x14), 0, D_800294F8[*(s32 *)(q + 0x10)] * 2 | 1);
    func_lo20_800263C0(*(u8 **)(q + 0x70), *(u8 **)(q + 0x14));
    *(s32 *)(q + 0x8) = 0;
    func_lo20_80026370(*(u8 **)(q + 0x70), 0);
}

/* 25 words, 0 differing.  First compile.  The pointer local `q = p + 0x20'
   is the unit's idiom (func_lo20_800262EC prices it at 3 rows).
   TWO facts of its own:
   - this function's SECOND parameter is forwarded as func_8004EAC4's second
     argument.  $a1 is never written before that `jal', which reads as no
     argument at all -- but dropping it is 1 aligned difference, because the
     pass-through is what keeps $a1 live into the call.
   - the `*(s32 *)(q + 0x8) =' store of func_lo20_80026388's result is
     written BEFORE the func_lo20_80026370 call that reads it back: 3 aligned
     differences with the call first and the store after.  cse forwards the
     stored value, so the `addu $a1, $v0, $zero' is the argument copy and not
     a reload. */
void func_lo20_800265B8(u8 *p, u8 *b)
{
    u8 *q;

    q = p + 0x20;
    func_8004EAC4(*(u8 **)(q + 0x14), b, *(s32 *)(q + 0x8));
    func_lo20_800263C0(*(u8 **)(q + 0x70), *(u8 **)(q + 0x14));
    *(s32 *)(q + 0x8) = func_lo20_80026388(1, *(s32 *)(q + 0x8), *(s32 *)(q + 0x10));
    func_lo20_80026370(*(u8 **)(q + 0x70), *(s32 *)(q + 0x8));
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002661C);

/* 21 words, 0 differing.  func_lo20_80025CD8's shape with the +0x4 word
   coming from the global D_800294FC instead of a stack slot, p[0] a plain
   store rather than `|= 0x80', and p itself returned.
   LOAD-BEARING, 1 aligned difference: `*(s8 *)(p + 0x2) = -0x21' -- see
   func_lo20_80025CD8's comment for the mechanism.
   Measured and FREE at 21/0: `p[0xA] = v == 1 ? 0x42 : 0x5A' instead of the
   two-armed `if'.  Neither value is 0, so jump.c's store-flag transform does
   not apply and both spellings keep the branch. */
u8 *func_lo20_80026708(u8 *p, s32 v)
{
    p[1] = 0x28;
    *(u32 *)(p + 0x4) = D_800294FC;
    *(s8 *)(p + 0x2) = -0x21;
    p[3] = 5;
    if (v == 1)
    {
        p[0xA] = 0x42;
    }
    else
    {
        p[0xA] = 0x5A;
    }
    p[0xB] = 0x16;
    p[0] = 0x80;
    return p;
}

/* 21 words, 0 differing.  func_lo20_80025CD8's shape again; the `u32 t[2]'
   addressable local is worth 5 words / 21 aligned differences here (a scalar
   `u32 t' is 16 words with no frame).  The constant needs `lui' + `ori'
   because 0x8040F0 is not an `addiu' immediate, which is the one word this
   body has over func_lo20_80025CD8. */
void func_lo20_8002675C(u8 *p)
{
    u32 t[2];

    t[0] = 0x8040F0;
    p[1] = 0x28;
    *(u32 *)(p + 0x4) = t[0];
    p[2] = 0;
    p[3] = 0x18;
    p[0xA] = 7;
    p[0xB] = 2;
    p[0] |= 0x80;
}

/* 10 words, 0 differing.  First compile.  func_lo20_80025D74's shape. */
u32 func_lo20_800267B0(s32 a)
{
    return func_800491F4(a, D_80029500, 4);
}

/* 40 words, 0 differing.  func_lo20_80025D9C's body with -0x60/-0x70 in
   place of 0x2C/-0x8; both of its facts (array-ref destinations, element 0
   through the pointer local) were priced there at 2 words / 32 rows each and
   both are needed here too -- the plain-cast form is 42/32 on this body as
   well.  `ovlverify --raw' is 0 differing. */
void func_lo20_800267D8(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    ((u32 *)c)[0] = s1[0];
    ((u32 *)c)[1] = s1[1];
    ((u32 *)c)[2] = s1[2];
    ((u32 *)c)[3] = s1[3];
    ((u32 *)d)[0] = s2[0];
    ((u32 *)d)[1] = s2[1];
    ((u32 *)d)[2] = s2[2];
    *(u8 **)(b + 0x4) = c;
    *(u8 **)(b + 0x8) = d;
    *(s32 *)b = 0;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x8) = -0x60;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xA) = -0x70;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xC) = 0x200;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x6) = 0;
    (*(u8 **)(b + 0x4))[0xF] = 4;
}

/* 45 words, 0 differing.  Aims the four slot pointers of the +0x10 record
   at their payloads: slot 0 gets D_80029508[p->unk_10], slots 1 and 2 get
   p + 0x2C and p + 0x20, slot 3 a handle built by func_8004DC14, and then
   the handle's prim gets its two coordinates.

   THE SHAPE FACT: each slot is TWO dereferences deep but the prim fields are
   only ONE deep off the slot -- `*(u8 **)(*(u8 **)(b + 0xC) + 0x4)', not
   `**(u8 ***)(b + 0xC) + 0x4'.  Written with the extra indirection the body
   is 49 words / 18 differing, two words per site.
   Also LOAD-BEARING, 4 aligned differences each way: the b + 0x4 store must
   PRECEDE the func_lo20_80026370 call (reorg sinks it into the `jal' delay
   slot) and the b + 0x8 store must FOLLOW it.  Written the other way round
   each is 45 words / 4 rows, so the two stores' positions relative to the
   call are both real and not a schedule to read off the listing.
   The second parameter stays `s32' because that is what this unit's
   pre-existing declaration says and what the landed caller
   func_lo20_800269A0 passes. */
void func_lo20_80026878(u8 *p, s32 b)
{
    **(s32 **)b = D_80029508[*(s32 *)(p + 0x10)];
    **(u8 ***)(b + 0x4) = p + 0x2C;
    func_lo20_80026370((u8 *)b, 0);
    **(u8 ***)(b + 0x8) = p + 0x20;
    **(u8 ***)(b + 0xC) = func_8004DC14(*(u8 **)(p + 0x14), 0);
    *(s16 *)(*(u8 **)(*(u8 **)(b + 0xC) + 0x4) + 0x8) = 0x68;
    *(s16 *)(*(u8 **)(*(u8 **)(b + 0xC) + 0x4) + 0xA) = 0x88;
}

/* 29 words, 0 differing.  First compile.  func_lo20_80028210's shape: the
   record's own fields, two sub-records built by func_lo20_80026708 and
   func_lo20_8002675C, and the handler installed at p - 0x10.
   D_lo20_800266B0 is an `alabel' inside func_lo20_8002661C's listing, the
   same device as D_lo20_80027F34; relcand confirms the symbol.
   Measured and FREE at 29/0: `c == 0x13' written twice (once for the p +
   0x10 store, once as func_lo20_80026708's argument) against hoisting it
   into a local.  cse unifies it either way, so the shared `$a1' is not
   evidence of a source local -- and note the flag really is one `xori' +
   `sltiu', which is the store-flag transform this unit fights elsewhere
   working for us. */
void func_lo20_8002692C(u8 *p, s32 b, s32 c, s32 d)
{
    *(s32 *)(p + 0xC) = c;
    *(s32 *)(p + 0x10) = c == 0x13;
    func_lo20_80026708(p + 0x20, c == 0x13);
    func_lo20_8002675C(p + 0x2C);
    *(s32 *)(p + 0x14) = d;
    *(s32 *)(p + 0x1C) = b;
    *(u8 **)(p - 0x10) = D_lo20_800266B0;
}

/* 18 words, 0 differing.  First compile.  func_lo20_80026084's shape; its
   four-argument forwarding fact (4 rows / 3 words) was priced there, and
   func_lo20_8002692C likewise saves $a0..$a3. */
void func_lo20_800269A0(u8 *p, s32 b, s32 c, s32 d)
{
    func_lo20_8002692C(p, b, c, d);
    func_lo20_800267D8(p, p + 0x60, p + 0x38, p + 0x48);
    func_lo20_80026878(p, *(s32 *)(p + 0x70));
}

/* 31 words, 0 differing.  Allocates the p + 0x74 handle table, and on
   success copies the 3-word D_8002E5E8 template to p + 0x54 and publishes it
   through func_800491CC.  Returns whether the allocation succeeded.

   TWO facts:
   1. There must be NO pointer local for the call's result -- the test and
      the p + 0x70 store both read `*(u8 **)(p + 0x70)' and cse forwards the
      stored value.  With a `u8 *q' holding it, the `addu $a0, $v0, $zero'
      moves ahead of the `beqz' and the branch tests $a0: 10 aligned
      differences.  func_800491CC's first argument is that same forwarded
      read, which is where the target's `addu $a0, $v0, $zero' comes from.
   2. `r = 1' must be the LAST statement of the arm, after the
      func_800491CC call -- 2 aligned differences at every one of the other
      four slots (a flat sweep: 2, 2, 2, 2, 0), which is the signature of a
      position that only the last slot satisfies rather than a curve.
   Here the plain cast `*(u32 *)(p + 0x54)' is what lands and the array-ref
   form func_lo20_80025D9C needs is not required: there is only one template
   symbol, so nothing has to be lifted past the stores. */
s32 func_lo20_800269E8(u8 *p)
{
    s32 r;

    r = 0;
    *(u8 **)(p + 0x70) = func_80048EE4((u8 **)(p + 0x74), 4);
    if (*(u8 **)(p + 0x70) != NULL)
    {
        u32 *s2 = D_8002E5E8;

        *(u32 *)(p + 0x54) = s2[0];
        *(u32 *)(p + 0x58) = s2[1];
        *(u32 *)(p + 0x5C) = s2[2];
        func_800491CC(*(u32 ***)(p + 0x70), (u32)(p + 0x54), 4);
        r = 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026A64);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026A84);

/* 20 words, 0 differing.  First compile.  func_lo20_80026250's two
   statements under a gate on the same value; the gate's arm is a
   fall-through, so it is the same cse block and the second read of p + 0xC
   collapses into the branch's own register, which is why the body reads the
   expression three times and the listing loads it twice.
   Measured and FREE at 20/0: an `s32 v' local for the gated value instead of
   writing `**(s32 **)(p + 0xC)' at all three sites. */
void func_lo20_80026B44(u8 *p)
{
    if (**(s32 **)(p + 0xC) != 0)
    {
        func_8004E99C(**(s32 **)(p + 0xC));
        **(s32 **)(p + 0xC) = 0;
    }
}

/* 26 words, 0 differing.  func_lo20_80026294's teardown with a `q = p +
   0x20' indirection on the two callees and `p' itself still used for the
   +0x1E halfword.  Its array-ref fact reproduces at 1 word / 15 aligned
   differences; see func_lo20_80026294's comment for the mechanism.
   func_lo20_80026B44 takes ONE argument (func_lo20_800263C0's comment
   settles that). */
void func_lo20_80026B94(u8 *p)
{
    u8 *q;

    if (p != NULL)
    {
        q = p + 0x20;
        func_lo20_80026B44(*(u8 **)(q + 0x70));
        func_8004B1A4(*(s32 *)(q + 0x74));
        ((u16 *)p)[0xF] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 9 words, 0 differing.  Resurrects docs/dng-parks/func_lo20_80026BFC.c
   (agent-h161, 9 words / 3 aligned differences); the park's diagnosis was
   right and its remedy was one spelling short.

   THE FACT: the `return 1' arm must be an EARLY RETURN and the zero arm a
   LABELLED FALL-OUT.  jump.c's store-flag transform (jump.c:1139-1200, and
   the park proved it fires here despite BRANCH_COST 1 because
   HAVE_conditional_move is defined) needs the insn right after the condjump
   to be a lone `SET x, const' whose block then reaches the condjump's OWN
   label.  An early `return 1' makes the taken arm a value-plus-jump-to-
   epilogue, and the trailing `zero: return 0;' makes the fall-through block
   start with a CODE_LABEL, so `next_nonnote_insn (insn)' is not an INSN at
   all.  reorg then builds the target: it steals `addiu $v0, 1' out of the
   return-1 block into the branch's delay slot, the block dies, and
   relax_delay_slots inverts the branch over the `j' that is left.

   Priced on this body, all comparable because at 9 words the gate IS the
   body:

     as written                                             9w /  0
     `if (X != 0) goto zero; return 1; zero: return 0;'     9w /  0
     park's kept body (r pre-set, `else return 1')          9w /  3
     `goto zero'/`goto one' with BOTH arms as stores        9w /  4
     the same with the zero arm labelled inside the `if'    9w /  4
     `r = 0' PRE-SET plus any both-arms-store form          7w /  7
     `r = 0' pre-set plus a label on the zero arm           7w /  7

   So the park's own observation that a pre-set `r' is what puts the load in
   $v1 is true and is a DEAD END: every pre-set form hands jump.c its `x = a'
   and collapses to the 7-word `sltiu'.  The load lands in $v1 for free once
   the arms are right. */
s32 func_lo20_80026BFC(u8 *p)
{
    if (p == NULL)
    {
        goto zero;
    }
    if (*(s32 *)(p + 0x28) == 0)
    {
        return 1;
    }
zero:
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026C20);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026C88);

/* 3 words, 0 differing.  First compile. */
s32 func_lo20_80026CCC(u8 *p)
{
    return *(s32 *)(p + 0x34);
}

/* 6 words, 0 differing.  First compile. */
void func_lo20_80026CD8(u8 *p)
{
    **(s32 **)(p + 0xC) = 0;
    **(s32 **)(p + 0x10) = 0;
}

/* 7 words, 0 differing.  First compile.  func_lo20_80026CD8's two indirect
   stores with real values; func_lo20_80025C58 supplies the D_800789B8 half. */
void func_lo20_80026CF0(u8 *p, s32 v)
{
    **(u8 ***)(p + 0xC) = D_800789B8;
    **(s32 **)(p + 0x10) = v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026D0C);

/* 20 words, 0 differing.  First compile.
   THE CALL IS FOUR ARGUMENTS TO A LANDED TWO-PARAMETER FUNCTION.  The
   listing sets $a0..$a3 before `jal func_lo20_80025040', and that body is
   already landed here as `char *func_lo20_80025040(char *dst, s32 i)'.  The
   three entry copies are real -- they are dead after the call and computed
   only for it -- so this unit declares the arity-4 alias
   func_lo20_80025040_4 rather than widening the landed prototype.  Spelled
   with two arguments the body is three words short. */
void func_lo20_80026DE0(s32 a, s32 b)
{
    char *r;

    r = (char *)func_8004B404(0x91);
    if (r != NULL)
    {
        func_lo20_80025040_4(r, a, b, 0x48);
    }
}

/* 16 words, 0 differing, after seven spellings.  THE FACT, and it generalises
   to every walk of this 0x13-stride table in the bank:

   The base must be a CONSTANT POINTER LOCAL holding 0x80010000, loop-invariant,
   with the stride AND the 0x57D2 inside the subscript.  Then loop.c reduces
   `i * 0x13' onto the base register and leaves 0x57D2 as the load's
   displacement, which is the target's `lui $v1, 0x8001' + `lbu 0x57D2($v1)'
   + `addiu $v1, $v1, 0x13' -- 16 words, one address instruction.

   splat prints those two words as `%hi(D_800157D2)' / `%lo(D_800157D2)', and
   that rendering is NOT evidence of a symbol: the identical instruction words
   appear in func_lo20_80026F04/func_lo20_80026F1C, where splat prints them as
   the constant `(0x80010000 >> 16)' and `0x57D2'.  Rejected, all 17 words
   (one address word too many) and 2-3 aligned differences:

     D_800157D2[i * 0x13]                                17w / 3
     u8 *q = D_800157D2, *q, q += 0x13                   17w / 3
     the same with `extern u8 D_800157D2[0x4C0]'         17w / 3
     the same with `extern u8 D_800157D2[][0x13]'        17w / 3
     the same with `extern u8 D_800157D2' (scalar)       17w / 3
     u8 *q = (u8 *)0x800157D2, *q, q += 0x13             17w / 2 (lui+ori)
     u8 *q = (u8 *)0x80010000, q[0x57D2], q += 0x13      17w / 2 (lui+ori)
     s32 k, D_800157D2[k], k += 0x13                     19w / 12

   Why no symbol spelling can reach it, checked with a standalone
   tools/cdkgcc probe: cc1psx materialises a SYMBOL's address with
   `lui %hi' + `addiu %lo' whenever the address goes into a register, and
   folds `%lo' into a MEM only for a single loop-invariant access.  And once
   the pointer is a biv, loop.c folds a constant subscript into its INIT
   (`lui' + `ori'), so the displacement only survives when the base register
   is loop-INVARIANT and the index is the giv. */
s32 func_lo20_80026E30(s32 a)
{
    s32 n;
    s32 i;
    u8 *b;

    n = 0;
    b = (u8 *)0x80010000;
    for (i = 0; i < 0x40; i++)
    {
        if (b[i * 0x13 + 0x57D2] == 0)
        {
            break;
        }
        if (b[i * 0x13 + 0x57D2] == a)
        {
            n++;
        }
    }
    return n;
}

/* 20 words, 0 differing.  First compile off the landed func_lo20_80026E30,
   whose constant-base fact carries verbatim: the base must be a
   loop-INVARIANT pointer local holding 0x80010000 with the 0x13 stride and
   the 0x57D2 inside the subscript, so loop.c reduces `i * 0x13' onto the
   base register and leaves the displacement in the `lbu'.  See
   func_lo20_80026E30's comment for the seven rejected symbol spellings.
   `ovlverify --raw' is 0 differing too, so the 0x57D2/0x57D1 displacements
   and the 0x13 stride are byte-confirmed and not masked relocations.
   Measured and FREE at 20/0: the two byte tests as nested `if's instead of
   `&&'.  They read different addresses, so fold_truthop never applies. */
s32 func_lo20_80026E70(s32 a, s32 b)
{
    s32 n;
    s32 i;
    u8 *q;

    n = 0;
    q = (u8 *)0x80010000;
    for (i = 0; i < 0x40; i++)
    {
        if (q[i * 0x13 + 0x57D2] == 0)
        {
            break;
        }
        if (q[i * 0x13 + 0x57D2] == a && q[i * 0x13 + 0x57D1] == b)
        {
            n++;
        }
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026EC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026F04);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026F1C);

/* 21 words, 0 differing.  First compile.  func_lo20_80026E70's table walk
   with a match COUNTER and an early return of the index: same constant base,
   same 0x13 stride, no zero terminator test.  `ovlverify --raw' is 0 too. */
s32 func_lo20_80026F54(s32 a, s32 b, s32 c)
{
    s32 n;
    s32 i;
    u8 *q;

    n = 0;
    q = (u8 *)0x80010000;
    for (i = 0; i < 0x40; i++)
    {
        if (q[i * 0x13 + 0x57D2] == a && q[i * 0x13 + 0x57D1] == b)
        {
            if (n == c)
            {
                return i;
            }
            n++;
        }
    }
    return -1;
}

/* 24 words, 0 differing, after fifteen spellings.  THE FACT, and it is the
   one thing that separates this from func_lo20_80026E70/func_lo20_80026F54:
   because the loop starts at a RUNTIME index, gcc emits an entry guard, and
   the constant base must be materialised INSIDE the loop body (loop.c hoists
   it) rather than before the loop.

     base assigned inside the loop body               24w /  0
     base assigned before the loop                    23w / 24
     base assigned after an explicit `if (c < 0x40)'  24w /  0
     base written inline at both subscripts           30w / 28
     a cursor `q = base + c * 0x13', `q += 0x13'      25w / 24
     an `s32 k' offset local, base invariant          23w / 24

   Two things go wrong with the base before the loop, and the hoist fixes
   both.  The entry guard splits the preheader off, so `i = c' and the giv's
   init land in different basic blocks and the copy `addu $a3, $a2, $zero'
   survives -- with the base before the loop cse propagates the constant into
   the giv init, the copy is folded away and the body is a word short.  And
   the lui's LUID then falls AFTER the whole `* 0x13' shift-add chain, so
   sched1 (equal costs, tie broken on INSN_LUID) emits it at word 7 where the
   target has it at word 4.
   Measured and FREE at 24/0: reusing the parameter `c' as the counter
   instead of a separate `i'.  `ovlverify --raw' is 0 differing. */
s32 func_lo20_80026FA8(s32 a, s32 b, s32 c)
{
    s32 i;
    u8 *q;

    for (i = c; i < 0x40; i++)
    {
        q = (u8 *)0x80010000;
        if (q[i * 0x13 + 0x57D2] == a && q[i * 0x13 + 0x57D1] == b)
        {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027008);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027070);

/* 15 words, 0 differing, after eleven spellings.  THREE facts:

   1. The gate is written `if (a != 0x39) k = a; else k = 2;'.  Written the
      other way round the then-arm becomes the fall-through, regmove folds
      `k = a' into $a0 and `k' loses its own register: 13 aligned differences.
      Priced: `!=' form 12, `==' form 13, ternary 13 (before fact 2 was
      found, so these are mid-fight figures -- see below).
   2. TWO locals, `o' for `k * 65' and `q' for `&D_lo20_800287EC[j * 13]',
      and `o' MUST be assigned first: 8 aligned differences with the two
      assignments swapped.  Without the locals fold reassociates the symbol
      onto `k * 65' instead of onto `j * 13' and the two `addu' take each
      other's operands -- every inline spelling is 11 or 12 rows:
        &D[k * 65 + j * 13]                         12
        &D[j * 13] + k * 65                         11
        D + j * 13 + k * 65                         11
        (u8 *)(k * 65 + (s32)&D[j * 13])            12
        (u8 *)(k * 65 + ((s32)D + j * 13))          12
        o = k * 65; (u8 *)(o + (s32)&D[j * 13])     12
        o = k * 65; &D[j * 13] + o                   8
   3. The final add is spelled INT FIRST, `(u8 *)(o + (s32)q)', worth exactly
      1 aligned difference against `q + o'.  pointer_int_sum always builds
      PLUS(ptr, intop), and the target's `addu $v0, $v1, $v0' has the index in
      $v1 as rs. */
u8 *func_lo20_80027110(s32 a, s32 j)
{
    s32 k;
    s32 o;
    u8 *q;

    if (a != 0x39)
    {
        k = a;
    }
    else
    {
        k = 2;
    }
    o = k * 65;
    q = &D_lo20_800287EC[j * 13];
    return (u8 *)(o + (s32)q);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002714C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027328);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027368);

/* 24 words, 0 differing.  First compile.
   LOAD-BEARING and worth 3 words / 24 aligned differences: the
   `*(s32 *)(p + 0x4) = n' store is written BEFORE the func_lo20_80027008
   call.  reorg sinks it into the `jal' delay slot, so its position in the
   listing is not its position in the source -- but written after the call it
   cannot reach the slot at all.  Third time this unit has priced that one
   (see func_lo20_800262EC). */
u8 *func_lo20_80027384(u8 *p, s32 b)
{
    s32 n;

    func_8004B1A4(*(s32 *)(p + 0x8));
    n = func_lo20_80026E30(*(s32 *)p);
    *(s32 *)(p + 0x4) = n;
    *(s32 *)(p + 0x8) = func_lo20_80027008(*(s32 *)p, b, n);
    return p;
}

/* 28 words, 0 differing.  First compile.  func_lo20_80027384's shape with
   a third parameter, func_lo20_80026E70 in place of func_lo20_80026E30 and a
   `+ 5' on its result; its store-before-the-call fact was priced there.
   func_lo20_8002714C takes four arguments and the third is that `+ 5'
   value, which is also what reaches p + 0x4. */
u8 *func_lo20_800273E4(u8 *p, s32 b, s32 c)
{
    s32 n;

    func_8004B1A4(*(s32 *)(p + 0x8));
    n = func_lo20_80026E70(*(s32 *)p, c) + 5;
    *(s32 *)(p + 0x4) = n;
    *(s32 *)(p + 0x8) = func_lo20_8002714C(*(s32 *)p, b, n, c);
    return p;
}

/* 21 words, 0 differing.  A three-way dispatcher.
   LOAD-BEARING and worth 3 aligned differences: the inner gate is written
   `if (*(s32 *)p != 0x13) func_lo20_80027384(...) else
   func_lo20_800273E4(...)', i.e. the 80027384 call is the THEN arm and the
   fall-through.  With the arms the other way round the target's
   `beq $v1, $v0' comes out `bne' and the two `addu' entry copies take each
   other's places. */
void func_lo20_80027454(u8 *p, s32 b, s32 c, s32 d)
{
    if (b == 0)
    {
        func_lo20_80027328(p, c);
    }
    else if (*(s32 *)p != 0x13)
    {
        func_lo20_80027384(p, c);
    }
    else
    {
        func_lo20_800273E4(p, c, d);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800274A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027534);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002773C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800277F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027A20);

/* 29 words, 0 differing.  First compile.  Copies the 8-byte D_lo20_80025034
   descriptor onto the stack, overwrites its +0x6 halfword with
   `v * 144 / d + 4' and hands it to func_800499E8, whose landed definition
   (csrc/unk_1C540.c, `u8 *func_800499E8(u8 *p, u8 *s, u8 *c)') reads s[0],
   s[2], s[4] and *(s16 *)(s + 6) -- so the +0x6 store is this body setting
   that field.  `v * 144' is the plain constant: the listing's
   `sll 3'/`addu'/`sll 4' is what cc1psx makes of it, and the `div' is a real
   variable divisor.
   Measured and FREE at 29/0: `u32 t[2]' instead of `u8 t[8]' for the stack
   copy.  THE ALIGNMENT-1 LEVER DOES NOT APPLY TO THE DESTINATION HERE --
   gcc's block move takes the MINIMUM of the two alignments and the SOURCE
   symbol D_lo20_80025034 is already alignment 1, so the `lwl'/`lwr' +
   `swl'/`swr' pairs come out either way.  Do not pay for the `u8' local in
   this shape; it only matters when both ends are 4-aligned. */
void func_lo20_80027AD0(u8 *p, s32 v, s32 d)
{
    u8 t[8];

    memcpy(t, D_lo20_80025034, 8);
    *(s16 *)(t + 0x6) = v * 144 / d + 4;
    func_800499E8(*(u8 **)(p + 0x4C) + 0x18, t, D_80080B54);
}

/* 10 words, 0 differing.  First compile.
   Measured and FREE at 10/0: `v * 144' against `(v * 9) << 4'.  The listing's
   `sll 3' / `addu' / `sll 4' is what cc1psx makes of the plain constant, so
   do not transcribe the decomposition.
   The `div' is a real variable divisor -- a literal one never reaches `div'
   (expand_divmod strength-reduces it four passes before cse). */
void func_lo20_80027B44(u8 *p, s32 v, s32 d)
{
    *(s16 *)(*(u8 **)(*(u8 **)(p + 0x4) + 0x4) + 0xA) = v * 144 / d;
}

/* 20 words, 0 differing, three compiles.  func_lo20_80027B44's store over
   eight slots of the p + 0x18 pointer array.  TWO facts:

   1. The row offset must be spelled `(i - 0xF) * 0x10 + 0x90', NOT
      `i * 16 - 0x60' -- 19 words / 16 aligned differences.  Both are
      16 * (i - 6), but fold does not distribute a constant multiply over
      `i - 0xF', so the giv reaching loop.c is `16i - 0xF0' with the +0x90 a
      separate PLUS.  loop.c then initialises the biv to -0x90 (= 16*6-0xF0)
      and emits the +0x90 as its own `addiu'; written `i * 16 - 0x60' the
      single constant folds into the giv's init, loop.c declines to reduce a
      one-insn giv at all, and the `i * 16' stays an in-loop `sll'.  The
      target's `addiu $a1, $zero, -0x90' plus `addiu $v0, $v0, 0x90' is the
      whole tell.
   2. The slot address must be written inline as `p + i * 4', not hoisted
      into a cursor local -- 6 aligned differences with a cursor, which puts
      its `addiu $a0, $a0, 0x18' at word 0 where the target has it at word 5,
      after the `v * 144' multiply and the row biv's init. */
void func_lo20_80027B6C(u8 *p, s32 v, s32 d)
{
    s32 i;

    for (i = 6; i < 0xE; i++)
    {
        *(s16 *)(*(u8 **)(*(u8 **)(p + i * 4) + 0x4) + 0xA) =
            v * 144 / d + (i - 0xF) * 0x10 + 0x90;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027BBC);

/* 10 words, 0 differing.  First compile.  func_lo20_80025D74's shape.
   Byte-identical to func_lo20_80027FCC after normalising the string symbol --
   dngtiny's one multi-member class in this unit -- and both were verified
   separately, which is the point: the tool normalises a per-copy symbol
   rename and the compiler does not. */
u32 func_lo20_80027FA4(s32 a)
{
    return func_800491F4(a, D_80029534, 0xF);
}

/* 10 words, 0 differing.  First compile.  func_lo20_80027FA4's twin with
   D_80029544 in place of D_80029534; verified separately, and relcand
   confirms each names its own string. */
u32 func_lo20_80027FCC(s32 a)
{
    return func_800491F4(a, D_80029544, 0xF);
}

/* 23 words, 0 differing.
   LOAD-BEARING, 1 aligned difference: the two -0x31 stores go through
   `*(s8 *)' -- see func_lo20_80025CD8.  The 0x63 pair needs no cast because
   it is positive.
   func_lo20_80028774 is called with ONE argument: it clobbers $a1 as a
   block-move temp, and the `addu $s0, $a1, $zero' in the `jal' delay slot is
   this function saving its own second parameter across the call, not an
   argument being passed.  NOT MEASURABLE either way -- $a1 already holds v
   at the call site, so a second argument would emit no instruction. */
u8 *func_lo20_80027FF4(u8 *p, s32 v)
{
    func_lo20_80028774(p);
    if (v == 1)
    {
        *(s8 *)(p + 0x2) = -0x31;
        *(s8 *)(p + 0xE) = -0x31;
        p[0xA] = 0x63;
        p[0x16] = 0x63;
    }
    return p;
}

void func_lo20_80028050(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    ((u32 *)c)[0] = s1[0];
    ((u32 *)c)[1] = s1[1];
    ((u32 *)c)[2] = s1[2];
    ((u32 *)c)[3] = s1[3];
    ((u32 *)d)[0] = s2[0];
    ((u32 *)d)[1] = s2[1];
    ((u32 *)d)[2] = s2[2];
    *(u8 **)(b + 0x4) = c;
    *(u8 **)(b + 0x8) = d;
    *(s32 *)b = 0;
    *(s16 *)(*(u8 **)(b + 0x4) + 0x8) = -0x58;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xA) = -0x40;
    *(s16 *)(*(u8 **)(b + 0x4) + 0xC) = 0x200;
    *(s16 *)(*(u8 **)(b + 0x8) + 0x2) = 0x400;
    *(s16 *)(*(u8 **)(b + 0x8) + 0x6) = -0x50;
    (*(u8 **)(b + 0x4))[0xF] = 4;
}


INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800280FC);

void func_lo20_80028208(void)
{
}

/* 11 words, 0 differing, two compiles.
   LOAD-BEARING and worth 3 aligned differences: the `p + 0x48' store must be
   written BEFORE the `p + 0x4' store.  With `= 4' second the `addiu $v0, 4'
   that feeds it is scheduled to the top of the body and the first three words
   come out transposed.  Measured FREE on top of that: the order of the
   `p + 0x4' and `p + 0x18' stores (0 either way).
   The handler stored at `p - 0x10' is D_lo20_80027F34, an alabel inside
   func_lo20_80027BBC's listing; relcand confirms the symbol. */
void func_lo20_80028210(u8 *p, u8 *q, s32 c)
{
    *(u8 **)(p + 0x44) = q;
    *(u8 **)(p + 0x48) = q + 0x20;
    *(s32 *)(p + 0x4) = 4;
    *(s32 *)(p + 0x18) = c;
    *(s32 *)p = 0;
    *(u8 **)(p - 0x10) = D_lo20_80027F34;
}

/* 18 words, 0 differing.  First compile.  func_lo20_80026084's shape with
   three parameters rather than four -- func_lo20_80028210, which it lands
   above, reads exactly $a0/$a1/$a2, and the call site in
   func_lo20_8002845C sets three.  That is how the arity was settled; the
   forwarding costs no instruction. */
void func_lo20_8002823C(u8 *p, u8 *q, s32 c)
{
    func_lo20_80028210(p, q, c);
    func_lo20_80028050(p, p + 0x78, p + 0x50, p + 0x60);
    func_lo20_800280FC(p, *(s32 *)(p + 0x88));
}

/* 39 words, 0 differing.  First compile off func_lo20_800260CC, which lands
   just above: the identical body with p + 0x8C/0x88/0x4C/0x6C in place of
   p + 0x58/0x54/0x74/0x38 and 8/8/0xF for 6/6/0xD.  Its three-argument
   func_80049004 fact (1 word / 32 rows) and its `r = 1'-last fact (3 rows)
   were priced there. */
s32 func_lo20_80028284(u8 *p, s32 n)
{
    s32 r;
    u8 *q;

    r = 0;
    q = p + 0x8C;
    *(u8 **)(p + 0x88) = func_80049004((u8 **)q, n, 8);
    if (*(u8 **)(p + 0x88) != NULL)
    {
        *(s32 *)(p + 0x4C) = func_80048FBC(q, 8);
        if (*(s32 *)(p + 0x4C) != 0)
        {
            u32 *s2 = D_8002E5E8;

            *(u32 *)(p + 0x6C) = s2[0];
            *(u32 *)(p + 0x70) = s2[1];
            *(u32 *)(p + 0x74) = s2[2];
            func_800491CC(*(u32 ***)(p + 0x88), (u32)(p + 0x6C), 0xF);
            r = 1;
        }
    }
    return r;
}

/* 37 words, 0 differing.  First compile.  The 0x8002845C object's "enter
   state 0" entry: reset the +0x20 record's counters, clear the +0x80 prim's
   +0x02 halfword, publish the caller's value at +0x38 and run the four
   rebuild calls, then install the handler at p + 0x10.
   D_lo20_80027D7C is an alabel inside func_lo20_80027BBC's listing, the same
   device as D_lo20_80027F34 (func_lo20_80028210's comment).

   LOAD-BEARING: `*(s32 *)(q + 0x8) = 0' is written BEFORE the
   func_lo20_80027454 call -- reorg sinks it into the `jal' delay slot, and
   after the call it cannot reach the slot at all.  Fourth body in this unit
   on that one (func_lo20_800262EC prices it at 9 rows).
   NOT MEASURABLE (not free -- the tools cannot see the difference): whether
   func_lo20_80027454's first argument is spelled `p + 0x58' or `q + 0x38',
   and whether func_lo20_80028648's is `p' or `q - 0x20'.  Both are the same
   tree after fold and both read 37/0; `p'-relative is kept because the
   listing computes them off $s1, which holds the parameter.
   The `*(s32 *)(q + 0x14)' read passed to both callees is a real reload
   each time: the `sh' through the +0x80 prim and then the calls invalidate
   cse's entry, which is why the body reads it three times and the listing
   loads it three times. */
void func_lo20_80028320(u8 *p, s32 b)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x14) = 0;
    *(s16 *)(*(u8 **)(q + 0x80) + 0x2) = 0;
    *(s32 *)(q + 0x38) = b;
    *(s32 *)(q + 0x8) = 0;
    func_lo20_80027454(p + 0x58, *(s32 *)(q + 0x14), 0, *(s32 *)(q + 0x28));
    func_lo20_80027FF4(*(u8 **)(q + 0x4C), *(s32 *)(q + 0x14));
    *(s32 *)(q + 0x84) = func_lo20_80027FA4(*(s32 *)(q + 0x88));
    func_lo20_80028534(*(u8 **)(q + 0x88));
    func_lo20_80028648(p);
    *(u8 **)(q - 0x10) = D_lo20_80027D7C;
}

/* 42 words, 0 differing.  First compile.  func_lo20_80028320's "enter state
   1" twin: one parameter instead of two, `= 1' at +0x14 rather than 0, and
   two extra stores feeding func_lo20_80027454's fourth argument.  Its
   store-before-the-call fact was priced there.

   TWO facts of its own:
   1. func_lo20_80027454's fourth argument must be READ BACK from q + 0x28,
      not re-derived from `*(u8 **)(q + 0x48) + 0x1C' -- 43 words / 29
      aligned differences re-derived.  cse forwards the store it has just
      made, which is the target's `addu $a3, $v1, $zero' sharing the
      register with the `sw $v1, 0x28($s0)' beside it.
   2. The q + 0x28 store must come BEFORE the q + 0x38 store: swapped, the
      body is 41 words / 31 rows, because the forwarded value no longer
      reaches the argument copy. */
void func_lo20_800283B4(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x14) = 1;
    *(s16 *)(*(u8 **)(q + 0x80) + 0x2) = 0;
    *(s32 *)(q + 0x8) = 0;
    *(s32 *)(q + 0x28) = *(s32 *)(*(u8 **)(q + 0x48) + 0x1C);
    *(s32 *)(q + 0x38) = *(s32 *)(q + 0x18);
    func_lo20_80027454(p + 0x58, *(s32 *)(q + 0x14), 0, *(s32 *)(q + 0x28));
    func_lo20_80027FF4(*(u8 **)(q + 0x4C), *(s32 *)(q + 0x14));
    *(s32 *)(q + 0x84) = func_lo20_80027FA4(*(s32 *)(q + 0x88));
    func_lo20_80028534(*(u8 **)(q + 0x88));
    func_lo20_80028648(p);
    *(u8 **)(q - 0x10) = D_lo20_80027D7C;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002845C);

/* 31 words, 0 differing.  func_lo20_80026250's release-and-clear over the
   eight pointer slots at p + 0x18..p + 0x34, then slot 0xE (p + 0x38) once
   more WITHOUT the zero store.  The re-read of the slot after the call is
   not a source repetition to avoid -- the call invalidates cse's entry (see
   func_lo20_80026250).

   THE FACT, worth 4 aligned differences: the counter and the cursor must be
   initialised in ONE `for' initialiser with a comma, counter FIRST --
   `for (i = 6, q = p + 0x18; ...)'.  Written as a separate `q = p + 0x18;'
   statement before the loop, the two prologue saves and their two
   initialising `addiu's come out in the opposite order ($s0 before $s1).
   A cursor is right here and NOT the inline `p + i * 4' that
   func_lo20_80027B6C needs: this listing has both a `addiu $s0, $s0, 4' and
   a separate `addiu $s1, $s1, 1', i.e. two inductions. */
void func_lo20_80028534(u8 *p)
{
    s32 i;
    u8 *q;

    for (i = 6, q = p + 0x18; i < 0xE; i++)
    {
        func_8004E99C(**(s32 **)q);
        **(s32 **)q = 0;
        q += 4;
    }
    func_8004E99C(**(s32 **)(p + 0x38));
}

/* 28 words, 0 differing.  func_lo20_80026B94's teardown with three callees;
   the array-ref fact on p + 0x1E reproduces at 1 word / 15 aligned
   differences (see func_lo20_80026294).  Note the func_8004B248 argument is
   `p + 0xAC' off the PARAMETER, not `q + 0x8C' off the +0x20 local -- that
   is what the listing's `addiu $a0, $s1, 0xAC' says. */
void func_lo20_800285B0(u8 *p)
{
    u8 *q;

    if (p != NULL)
    {
        q = p + 0x20;
        func_lo20_80028534(*(u8 **)(q + 0x88));
        func_8004B1A4(*(s32 *)(q + 0x40));
        func_8004B248((u32 *)(p + 0xAC));
        ((u16 *)p)[0xF] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 10 words, 0 differing.  First compile.  func_lo20_800262EC's shape; its two
   facts were priced there. */
void func_lo20_80028620(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    *(s32 *)(q + 0x20) = 0;
    func_lo20_80026CD8(*(u8 **)(q + 0x88));
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028648);

/* 30 words, 0 differing, three compiles.  Copies the 0x18-byte D_80029550
   record onto p.  TWO facts, both measured here:

   1. It must be TWO 12-byte copies, not one 0x18-byte copy -- 29 words / 15
      aligned differences as a single `memcpy(p, D_80029550, 0x18)'.  One
      copy gives gcc's block mover a single expansion, so it emits six
      unaligned word pairs off ONE source base with displacements 0..0x17.
      The target materialises the source address TWICE off one shared `lui'
      (`addiu $a3, $v1, %lo' then `addiu $v1, $v1, %lo', with the second
      group's displacements 0xC..0x17 folded into the MEM), which is two
      separate expansions of `+ 0xC' apart.
   2. It RETURNS p -- 29 words / 30 differing as `void'.  The return value
      pins p into $v0 before the move starts, which is the target's opening
      `addu $v0, $a0, $zero' and the reason $a0 is free to be the move's
      first temp.  Spelled `void', regmove folds the copy away, p stays in
      $a0 and the temps rotate to $v1/$a1/$a2.
      Its only caller here, func_lo20_80027FF4, drops the result. */
u8 *func_lo20_80028774(u8 *p)
{
    memcpy(p, D_80029550, 0xC);
    memcpy(p + 0xC, D_80029550 + 0xC, 0xC);
    return p;
}
