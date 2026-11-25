#include "common.h"

s32 func_lo20_800258B0(u8 *p, void *b, void *c, u8 *d);

u32 func_800491F4(s32, u8 *, s32);
s32 func_80049DE8(s32, s32, s32);
u8 *func_8004DC14(u8 *, s32);
void func_8004E99C(s32);
void func_lo20_80025D9C(u8 *, u8 *, u8 *, u8 *);
void func_lo20_80025E3C(u8 *, s32);
void func_lo20_80025FFC(u8 *, s32, s32, s32);
void func_lo20_800267D8(u8 *, u8 *, u8 *, u8 *);
void func_lo20_80026878(u8 *, s32);
void func_lo20_8002692C(u8 *, s32, s32, s32);
void func_lo20_80026B44(u8 *);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800258B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025964);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025C6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025CD8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025D9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025E3C);

void func_lo20_80025FF4(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80025FFC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800260CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026168);

/* 17 words, 0 differing.  First compile.  The second `lw $v0, 0x14($s0)' is
   not a source repetition to avoid -- the call invalidates cse's entry, so
   one `**(s32 **)(p + 0x14)' per statement is what emits it. */
void func_lo20_80026250(u8 *p)
{
    func_8004E99C(**(s32 **)(p + 0x14));
    **(s32 **)(p + 0x14) = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026294);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002654C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800265B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002661C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026708);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002675C);

/* 10 words, 0 differing.  First compile.  func_lo20_80025D74's shape. */
u32 func_lo20_800267B0(s32 a)
{
    return func_800491F4(a, D_80029500, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800267D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026878);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002692C);

/* 18 words, 0 differing.  First compile.  func_lo20_80026084's shape; its
   four-argument forwarding fact (4 rows / 3 words) was priced there, and
   func_lo20_8002692C likewise saves $a0..$a3. */
void func_lo20_800269A0(u8 *p, s32 b, s32 c, s32 d)
{
    func_lo20_8002692C(p, b, c, d);
    func_lo20_800267D8(p, p + 0x60, p + 0x38, p + 0x48);
    func_lo20_80026878(p, *(s32 *)(p + 0x70));
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800269E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026A64);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026A84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026B44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026B94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026BFC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026DE0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026E70);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026EC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026F04);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026F1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026F54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80026FA8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027384);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800273E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027454);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800274A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027534);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002773C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800277F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027A20);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027AD0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027B6C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80027FF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028050);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800280F4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028284);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028320);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800283B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_8002845C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028534);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_800285B0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_lo20", func_lo20_80028774);
