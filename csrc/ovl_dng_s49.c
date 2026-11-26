#include "common.h"

s32 rand(void);
void func_800478B8(u8 *);
void func_80067F20(u32 *, s32, s32, s32, s32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800C80F0(void);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
u8 *func_800A3F28(u16, u16, u8 *, u8 *);
s32 func_800A56E0(s32);
s32 func_s49_80024004(u8 *p);
void func_s49_8002445C(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_s49_80024740(u8 *p, UnkVec *q, UnkSlot *s);
void func_s49_80024928(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u32 D_800814A0;
extern u32 D_800814A8;
extern u8 D_80045340[];
extern u8 D_80082E80[];
extern s32 D_8008346C;
extern u8 D_80083498[];
extern u16 D_80083780_h[] __asm__("D_80083780");
extern u8 D_800DEDB0[];
extern u8 D_800DEE38[];

/* A per-actor tick helper, called once per surviving actor by
   func_s49_80024128's teardown walk. func_800C80F0 is a global gate (a pause
   or cutscene flag); when it is clear, each of the actor's two slot pointers
   at +0x4C and +0x50 is followed into D_80073414, and a slot whose
   D_80073414[q[1]].unk_0C row has 0x8000 set in its +0x0 halfword has its
   +0x2 countdown decremented, floored at -0x62. The return value is the
   number of slots that were still counting down, 0, 1 or 2.

   73 words, 0 differing, FIRST COMPILE. No twin: dngtwin found no carved
   sibling at 0.70 and dngcallee's best overlap was 1 of 2 symbols reached by
   23 functions (noise). Written from the listing, with the D_80073414 access
   spelling taken from the landed func_8004A940 region of csrc/unk_154BC.c
   (`*(u16 *)(D_80073414[p[1]].unk_0C + p[0] * 0x14)' there, `*(s16 *)' here
   because this reader's load is `lh', not `lhu').

   THE SINGLE `return n' AT THE BOTTOM IS WHAT THE EARLY EXIT RETURNS, and the
   listing says so before any compile: the gate's `bnez $v0' has
   `addu $v0, $s0, $zero' in its delay slot, NOT `addu $v0, $zero, $zero'. A
   `return 0' spelled at the gate would have been constant-folded to `$zero',
   because the only reaching definition of `n' there is the `n = 0' three
   insns earlier. So the whole body is wrapped in `if (func_800C80F0() == 0)'
   and there is one exit.

   THE SAME ASYMMETRY IS WHY BOTH BUMPS ARE `n++'. The first emits
   `addiu $s0, $zero, 0x1' and the second `addiu $s0, $s0, 0x1'; that is not
   two different source statements but cse propagating `n == 0' into the first
   one. cse_end_of_basic_block walks through a conditional jump and stops only
   at a CODE_LABEL (cse.c:8086), and .Ls49_80024098 is the first label in the
   function -- it sits between the two bumps. Writing `n = 1' in the first arm
   would be reading the compiler's output back as source.

   TWO LOADS OF ONE BYTE ARE ONE FIELD READ THROUGH TWO TYPES: `lb $v0, 0x2'
   for the clamp and `lbu $v1, 0x2' for the decrement, and cse cannot unify
   them because a sign_extend and a zero_extend of the same QImode MEM are
   different expressions. `*(s8 *)(q + 0x2) >= -0x62' gives the `lb' and
   `q[2]--' off the `u8 *' gives the `lbu'. MEASURED FREE: `(s8)q[2]' for
   `*(s8 *)(q + 0x2)' is also 73 words / 0 rows -- fold collapses the
   (signed char)(unsigned char) pair to one `lb' -- so the cast style is not
   the fact; the MIXED SIGNEDNESS is.

   The three tests are one `if' with `&&': all three of the first block's
   branches target .Ls49_80024098, so nesting them would be the same program.
   fold_truthop cannot merge them because each tests a different operand. */
s32 func_s49_80024004(u8 *p)
{
    u8 *q;
    s32 n;

    n = 0;
    if (func_800C80F0() == 0)
    {
        q = *(u8 **)(p + 0x4C);
        if ((q != NULL) && ((*(s16 *)(D_80073414[q[1]].unk_0C + q[0] * 0x14) & 0x8000) != 0) &&
            (*(s8 *)(q + 0x2) >= -0x62))
        {
            q[2]--;
            n++;
        }
        q = *(u8 **)(p + 0x50);
        if ((q != NULL) && ((*(s16 *)(D_80073414[q[1]].unk_0C + q[0] * 0x14) & 0x8000) != 0) &&
            (*(s8 *)(q + 0x2) >= -0x62))
        {
            q[2]--;
            n++;
        }
    }
    return n;
}

/* The bank's three-state cutscene dispatcher, the actor's own +0x10 handler.
   A COMPARE-CHAIN switch on *(s16 *)(p + 0xA) over {0, 1, 0xFF} -- no jump
   table, and this unit has no .rodata of any kind. State 0 waits on bit 0x80
   of the halfword behind +0x4, then copies the whole 0x18-byte UnkVec at
   D_80083780 into `b', seeds +0x10/+0x12/+0x14 from the INTEGER HALVES of
   that record's three 16.16 words (its +0x2/+0x6/+0xA), starts the 0x3C
   countdown at +0xC, cues func_800A56E0(0x300) and FALLS THROUGH into
   state 1. State 1 spawns up to nine 0x202 spark nodes per frame --
   func_s49_8002445C as each one's step, func_s49_80024740 as its
   func_8004491C tag callback -- scattered +-0x100 around the seeded centre,
   and on the single frame where +0xC reads 0x1E it walks every actor
   func_800A3F28 returns off D_800814A8, calls func_8009CE1C on the ones
   without 0x2000 in +0x1C and ticks each through func_s49_80024004. When the
   countdown expires it resets +0xC to 8 and goes to state 0xFF, which waits
   out another 8 frames and then either consumes the 0x8000 request bit at
   +0xE or clears D_8008346C and retires the actor.

   205 words, 0 differing. FOUR COMPILES, and all four were one fault.

   DONORS: the landed func_s70_8002401C in csrc/ovl_dng_s70.c and
   func_s63_8002401C in csrc/ovl_dng_s63.c -- dngcallee's joint 9-of-12-symbol
   row with a clean gap to the 8/12 tier, same -0x40 frame and the same six
   saves. dngtwin found NO carved sibling at 0.70 for this body, so the
   callee-set route is the only one that reached them. What carried, verbatim:
   the `*(s16 *)(p + N) = *(u16 *)(p + N) +- 1' bump idiom; the
   `if ((**(u16 **)(p + 0x4) & 0x80) == 0) break;' gate; the whole
   func_800A3F28 teardown walk including its `while (1)' shape, its separate
   `k' cursor and the (u32)s argument to func_8009CE1C; and the state-0xFF
   `*(s16 *)(p + 0xE) & 0x8000' / `*(u16 *)(p + 0xE) & 0x7FFF' pair.

   THE ONE FAULT, AND IT IS WORTH 26 ROWS AND A WHOLE-BLOCK REGISTER SHIFT:
   D_80083780 MUST BE DECLARED `u16 []' AND READ AS `D_80083780_h[1]',
   `[3]', `[5]' -- NOT `u8 []' read as `*(u16 *)(D_80083780 + 0x2)'. The
   struct copy `*b = *(UnkVec *)D_80083780_h' compiles to ONE `movstrsi' insn
   (output_block_move's 3-load/3-store/3-load/3-store sequence), and with the
   u8-plus-cast spelling sched1 HOISTS the first `lhu' and its address
   `addiu' ABOVE that insn: 204 words / 26 rows, the missing word being a
   load-delay `nop' the target cannot fill and mine fills with the block
   move's own base `addiu', and the extra live value then pushing `b' out of
   `$a3' into `$t1' and rotating every block-move temp.

   The mechanism is sched.c:835 `true_dependence', whose fixed-scalar versus
   varying-struct escape is

        && ! (MEM_IN_STRUCT_P (mem) && rtx_addr_varies_p (mem)
              && GET_MODE (mem) != QImode
              && ! MEM_IN_STRUCT_P (x) && ! rtx_addr_varies_p (x))

   The block move's destination `*b' is an in-struct BLKmode MEM at a varying
   address, so with the load spelled as a cast -- `! MEM_IN_STRUCT_P (x)' --
   the escape fires and sched sees no dependence. Declaring the symbol as an
   ARRAY and subscripting it makes the load a genuine ARRAY_REF, expand_expr
   sets MEM_IN_STRUCT_P on it, the escape's `! MEM_IN_STRUCT_P (x)' goes
   false, memrefs_conflict_p decides (two different pseudo bases, so
   conflict), and the load is pinned after the copy.

   A pointer local -- `t = D_80083780; *b = *(UnkVec *)t;' and
   `*(u16 *)(t + 0x2)' -- is NOT the fix: 204 words / 26 rows, bit-identical
   to the cast form, because combine folds the symbol back in and
   `rtx_addr_varies_p' is already true of both addresses (the %lo lives in a
   pseudo). Putting the three halfword stores BEFORE the copy is WORSE:
   204 / 38. And the array ref must use the SAME declaration as the copy:
   with `*b = *(UnkVec *)D_80083780' off a second `u8 []' extern beside the
   `u16 []' one it is 206 words / 10 rows, because gcc treats them as two
   symbols and emits a second `lui' where the target shares one.

   TWO MORE MEASURED FACTS, both priced from the body at 0:

     - `j' MUST HOLD `*(u16 *)(p + 0x14) - 0x100' FOR BOTH STORES. Written out
       twice it is 208 words / 8 rows: the `sh' to e+0x10 invalidates cse's
       entry for the p+0x14 load, so the read and the `addiu' are both
       repeated.
     - THE TWO STORES OF EACH JITTER EMIT IN SOURCE ORDER AND THE ORDER IS
       REAL: `*(s16 *)(e + 0x1E)' before `*(s16 *)(e + 0xC)' is 205 words /
       4 rows. sched1 normally transposes independent adjacent stores, but
       reorg steals the second of the pair into the NEXT `rand' call's delay
       slot, which pins both.

   Read off the listing and not priced: `rand() % 512' and not `rand() &
   0x1FF' -- the `bgez'/`addiu 0x1FF'/`sra 9'/`sll 9'/`subu' bias sequence is
   a signed remainder, and the mask would be one `andi'. `i = 0x3C -
   *(s16 *)(p + 0xC)' clamped with `if (i >= 8) i = 8;' gives `slti $s3, 0x8';
   `if (i > 8)' would have given `slti 0x9'. The address of
   func_s49_8002445C is hoisted into `$s5' outside the loop by loop.c on its
   own -- that is not a source hoist. */
void func_s49_80024128(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *e;
    u8 *h;
    u8 *k;
    UnkNode *n;
    s32 i;
    s32 j;

    s = *(u8 **)(p + 0x0);
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        *b = *(UnkVec *)D_80083780_h;
        *(s16 *)(p + 0x10) = D_80083780_h[1];
        *(s16 *)(p + 0x12) = D_80083780_h[3];
        *(s16 *)(p + 0x14) = D_80083780_h[5];
        *(s16 *)(p + 0xC) = 0x3C;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        func_800A56E0(0x300);
        /* fall through */
    case 1:
        i = 0x3C - *(s16 *)(p + 0xC);
        if (i >= 8)
        {
            i = 8;
        }
        for (; i >= 0; i--)
        {
            n = func_8003FD64(0x202, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s49_8002445C;
                func_8004491C(n, (u32)func_s49_80024740);
                e = (u8 *)n + 0x20;
                j = *(s16 *)(p + 0x10) + rand() % 512 - 0x100;
                if (j < 0)
                {
                    j = 0;
                }
                *(s16 *)(e + 0xC) = j;
                *(s16 *)(e + 0x1E) = j;
                j = *(s16 *)(p + 0x12) + rand() % 512 - 0x100;
                if (j < 0)
                {
                    j = 0;
                }
                *(s16 *)(e + 0xE) = j;
                *(s16 *)(e + 0x22) = j;
                j = *(u16 *)(p + 0x14) - 0x100;
                *(s16 *)(e + 0x10) = j;
                *(s16 *)(e + 0x26) = j;
                *(u8 **)(e + 0x0) = p;
            }
        }
        if (*(s16 *)(p + 0xC) == 0x1E)
        {
            h = (u8 *)D_800814A8;
            if (h != NULL)
            {
                k = h;
                while (1)
                {
                    h = func_800A3F28(D_80082E80[0x24], D_80082E80[0x25], k, h);
                    if (h == NULL)
                    {
                        break;
                    }
                    if ((*(s32 *)(h + 0x1C) & 0x2000) == 0)
                    {
                        func_8009CE1C(h, 0x20, *(u8 *)(p + 0x9), 0xA, *(s16 *)(s + 0x2A), (u32)s, 2);
                    }
                    func_s49_80024004(h);
                }
            }
        }
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) - 1;
        if (*(s16 *)(p + 0xC) <= 0)
        {
            *(s16 *)(p + 0xC) = 8;
            *(s16 *)(p + 0xA) = 0xFF;
        }
        break;
    case 0xFF:
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) - 1;
        if (*(s16 *)(p + 0xC) <= 0)
        {
            if ((*(s16 *)(p + 0xE) & 0x8000) != 0)
            {
                *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) & 0x7FFF;
            }
            else
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}

/* The spark's five-state step, installed on each 0x202 node by
   func_s49_80024128 as its +0x10 handler. A COMPARE-CHAIN switch on
   *(s16 *)(p + 0x4C) over {0, 1, 2, 3, 0xFF} -- NOT a jump table: five cases
   spanning 0..0xFF is far past gcc 2.7.2's density threshold, so
   expand_end_case emits the balanced tree rooted on `== 2' that the listing
   shows, and THIS UNIT OWNS NO .rodata AT ALL. Every frame the +0x48 fuse
   ticks down and 0x8000 is armed in the +0xE halfword of the node at the
   actor's +0x0. State 0 seeds the fuse (3), the two per-frame deltas
   (+0x2A = -4, +0x32 = 0x10) and FALLS THROUGH into state 1, which integrates
   the 16.16 position pair at +0x1C/+0x24 and advances when the fuse runs out.
   State 2 integrates, slews the packed +0xC/+0x10 screen pair by three
   quarters of each delta, and asks func_800BCB04 for the floor height under
   +0x1E/+0x22 at -0x400: above 0x200 with a positive +0x26 it retires to
   state 0xFF, and below the stored +0x26 it records the new height and spawns
   the 0x312 impact node -- func_s49_80024928 as its step, D_80045340 as its
   func_8004491C tag -- seeded from the actor's own position triple and dressed
   from D_800DEDB0 or D_800DEE38 on a coin flip. State 3 slews only, and
   clamps to +0x26 on contact. State 0xFF is the shared retire pair.

   185 words, 0 differing. THREE COMPILES.

   DONOR: the landed func_s12_80024020 in csrc/ovl_dng_s12.c -- dngcallee's
   7-of-9-symbol row with a clean gap to the 6/9 tier (7 functions). It is a
   different control flow (six states, two spawn loops) and was used only for
   this bank's SPELLINGS, all four of which carried:

     - `*(s16 *)(p + N) = *(u16 *)(p + N) + 1' for a halfword bump and
       `- 1' for a decrement. That mixed signedness is what gives `lhu' on the
       read and `sh' on the store, where the switch's own read of the same
       +0x4C field is `lh'.
     - the spawn skeleton: `n->unk_10 = (u32)<step>', then
       `func_8004491C(n, (u32)D_80045340)', then `q = (u8 *)n->unk_0C', then
       three stores through `(u8 *)n->unk_08' RE-READ EACH TIME (three `lw
       0x8($s0)' in the listing -- the store to the pointed-at word
       invalidates cse's entry, so the repetition is real source).
     - `*(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x500' as ONE chained
       assignment, and `*(u16 *)(q + 0x14) |= 0xC'.
     - the retire pair `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;'.

   WHAT THE THREE COMPILES BOUGHT, both facts priced from a body at 0:

   1. `s32 h = (s16)func_800BCB04(...)', NOT `s16 h'. As `s16 h' the body is
      189 words / 21 aligned rows: gcc keeps the raw `$v0' in a copy and
      RE-EXTENDS it at each use (`move $a0, $v0' plus a second
      `sll/sra' pair), and the `*(s16 *)(p + 0x26) = h' store then writes the
      unextended copy. With `s32 h = (s16)...' the `sll $v0,16 / sra $v1,16'
      pair runs once and `$v1' carries the value through both comparisons and
      the store. With no cast at all (`s32 h = func_800BCB04(...)') it is 184
      words / 5 rows -- one word SHORT, the extension gone. Same lever as
      func_800BFFF4's `s16 z' note in csrc/ovl_town_800BF3F0.c, with the sign
      INVERTED: there the narrow declaration is what lands, here the wide one
      with an explicit cast is. The discriminator is batch 28's: that body's
      `z' has only narrow uses, this body's `h' has a narrow STORE and two
      wide COMPARISONS.

   2. THE ANIMATION POINTER MUST BE SELECTED INTO A LOCAL IN THE ARMS AND
      STORED ONCE AFTER, not stored inside each arm. Written the committed s12
      way -- `*(u8 **)(q + 0x0) = D_800DEDB0;' in one arm and `= D_800DEE38;'
      in the other -- it is 187 words / 8 rows, because jump2 then has two
      stores to cross-join and the following
      `*(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4)' cannot see the
      stored value. With `a' assigned in the arms and one store after, the
      reload spelling stops mattering: `*(s32 *)(a + 0x4)' and
      `*(s32 *)(*(u8 **)(q + 0x0) + 0x4)' are BOTH 185 words / 0 rows and the
      output is byte-identical, so that second choice is NOT MEASURABLE rather
      than free -- cse forwards the store either way once there is only one.

      AND THE 8 ROWS ARE NOT THE SAME FAULT AS THE 21: `a' was introduced
      while fault 1 was still live, and from a body at 0 the re-read spelling
      measures FREE. Only the ARM-STORE form costs anything. The two faults
      were adding, which is the shape the brief warns about.

   3. THE CHAIN DIRECTION ON THE 0x500 PAIR IS LOAD-BEARING:
      `*(s16 *)(q + 0x1E) = *(s16 *)(q + 0x1C) = 0x500' is 185 words / 2 rows,
      both rows the `sh 0x1E' moving. A chained assignment stores the INNER
      target first, so the committed s12 direction is the one that emits
      0x1E before 0x1C. sched1 does NOT transpose these two independent
      stores, so for once the listing's order can be read -- backwards.

   Read off the listing and not priced: the `>> 2' on `delta * 3' is a bare
   `sra' with no bias correction, so it is a shift and not `/ 4'; state 3's
   `*(s16 *)(p + 0x10) = *(u16 *)(p + 0x26)' is the `lhu 0x26' beside the
   comparison's `lh 0x26' (MIPS movhi loads unsigned, arithmetic sign-extends);
   the two `*(s16 *)(p + 0x4C) = 0xFF' arms and the two
   `*(s16 *)(p + 0x4C) = ... + 1' arms are each one block in the output
   because jump.c cross-joined them, so the duplication in the source is free.
   The arms emit ascending, which is also address order. */
void func_s49_8002445C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *a;
    s32 h;

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    *(u16 *)(*(u8 **)p + 0xE) |= 0x8000;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = 3;
        *(s16 *)(p + 0x2A) = -4;
        *(s16 *)(p + 0x32) = 0x10;
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        /* fallthrough */
    case 1:
        *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28);
        *(s32 *)(p + 0x24) += *(s32 *)(p + 0x30);
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        *(s32 *)(p + 0x1C) += *(s32 *)(p + 0x28);
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + ((*(s16 *)(p + 0x2A) * 3) >> 2);
        *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + ((*(s16 *)(p + 0x32) * 3) >> 2);
        *(s32 *)(p + 0x24) += *(s32 *)(p + 0x30);
        h = (s16)func_800BCB04(*(u16 *)(p + 0x1E), *(u16 *)(p + 0x22), -0x400);
        if (h > 0x200 && *(s16 *)(p + 0x26) > 0)
        {
            *(s16 *)(p + 0x4C) = 0xFF;
        }
        else if (h < *(s16 *)(p + 0x26))
        {
            *(s16 *)(p + 0x26) = h;
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s49_80024928;
                *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n->unk_0C;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = *(s32 *)(p + 0x1C);
                *(s32 *)((u8 *)n->unk_08 + 0x4) = *(s32 *)(p + 0x20);
                *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)(p + 0x24);
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x500;
                *(u16 *)(q + 0x14) |= 0xC;
                if ((rand() & 1) != 0)
                {
                    a = D_800DEDB0;
                }
                else
                {
                    a = D_800DEE38;
                }
                *(u8 **)(q + 0x0) = a;
                *(s32 *)(q + 0x8) = *(s32 *)(a + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
            }
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 3:
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + ((*(s16 *)(p + 0x2A) * 3) >> 2);
        *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + ((*(s16 *)(p + 0x32) * 3) >> 2);
        if (*(s16 *)(p + 0x26) < *(s16 *)(p + 0x10))
        {
            *(s16 *)(p + 0x10) = *(u16 *)(p + 0x26);
            *(s16 *)(p + 0x4C) = 0xFF;
        }
        break;
    case 0xFF:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* A func_8004491C TAG CALLBACK, not an UnkNode +0x10 handler -- the arity
   witness is func_s49_80024128, which publishes it with
   `func_8004491C(n, (u32)func_s49_80024740)'. One LINE_G2 per node in the tag
   chain: a fixed 0x202020/0x000000 gouraud pair, the first endpoint the
   integer halves of the 16.16 triple at the actor view's +0x1C (read at
   +0x1E/+0x22/+0x26) copied into a stack SVECTOR, the second the packed s16
   triple at +0xC taken by address, each projected with RotTransPers straight
   into the primitive's own xy pair, then a DR_MODE behind it on the same OT
   bucket. It walks the whole chain itself through `*(UnkNode **)(p - 0x8)'
   and re-projects with `p = (u8 *)n + 0x20'.

   122 words, 0 differing, FIRST COMPILE.

   DONOR: the landed func_800BFFF4 in csrc/ovl_town_800BF3F0.c -- dngcallee's
   6-of-6-symbol row with a clean gap to the 4/6 tier (44 functions), ALSO 122
   words, ALSO frame -0x50 with the same ten saves. This is that body
   statement for statement; nothing was changed but the parameter names. Its
   header prices seven facts (`s16 z' against `s32 z' at 120/120; `>> 1'
   against `/ 2' at 124/72; the SVECTOR copy after SetLineG2/SetSemiTrans at
   124/103; the OT base re-derived at each addPrim rather than hoisted at
   118/117; `*(u32 *)&ln->r0' before `r1' at 2 rows; SetSemiTrans's abe = 1
   and GetTPage's x = 0 at 1 row each) and all of them are inherited here, not
   re-measured -- this body landed on the first compile, so the only figure it
   contributes is 122/0.

   ONE DELIBERATE DEPARTURE FROM THE DONOR, and it is worth saying because it
   is an improvement: the two out-parameters are `long pp; long flag;' and the
   sxy arguments are cast `(long *)', where the donor writes `s32' and
   `(s32 *)'. psyq/include/libgte.h:326 declares
   `long RotTransPers(SVECTOR *, long *, long *, long *)', so the donor's
   spelling raises SIX `passing arg N ... from incompatible pointer type'
   warnings under declcheck. MEASURED: `long' and `s32' are 122 words / 0
   differing BOTH WAYS -- the change is free and only the warnings move. The
   committed func_800BFFF4 still carries them. */
s32 func_s49_80024740(u8 *p, UnkVec *q, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    LINE_G2 *ln;
    DR_MODE *dm;
    SVECTOR v;
    long pp;
    long flag;
    s16 z;

    while (1)
    {
        ln = (LINE_G2 *)*(u32 *)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(ln + 1);
        *(u32 *)&ln->r0 = 0x202020;
        *(u32 *)&ln->r1 = 0;
        SetLineG2(ln);
        SetSemiTrans(ln, 1);
        v.vx = *(u16 *)(p + 0x1E);
        v.vy = *(u16 *)(p + 0x22);
        v.vz = *(u16 *)(p + 0x26);
        z = RotTransPers(&v, (long *)&ln->x0, &pp, &flag);
        z = (z + RotTransPers((SVECTOR *)(p + 0xC), (long *)&ln->x1, &pp, &flag)) >> 1;
        if ((u16)z < 0x1E0)
        {
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, ln);
            dm = (DR_MODE *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim((u32 *)(g->unk_00 + 0xB0) + z, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

/* The spark's per-frame step, installed by func_s49_8002445C's state 2 as the
   spawned 0x312 node's +0x10 handler. It arms 0x8000 in the +0xE halfword of
   the node at the actor's +0x0, hands the renderable `c' to func_800478B8,
   jitters the three bytes at c+0xC/+0xD/+0xE to 0x20..0x9F, and when c's +0x14
   reports 0x6000 marks the owning actor retired and sets the same bit in
   D_800814A0.

   42 words, 0 differing, FIRST COMPILE.

   TWIN: the landed func_s70_80024ACC in csrc/ovl_dng_s70.c (dngtwin 0.740, 31
   words, and dngtwin names s63 as a second bank with the identical shape).
   Its gate, its retire pair and its `*(u16 *)(*(u8 **)p + N) |= 0x8000' head
   carry over verbatim; the deltas are the node halfword at +0xE rather than
   +0x52, and the three `rand' byte jitters this body adds, which the twin
   has not got.

   `rand' IS func_80069EF8 (splat/common_syms.txt). Each jitter is
   `(rand() & 0x7F) + 0x20' stored through a `u8 *' -- `andi 0x7F',
   `addiu 0x20', `sb' -- and the three stores emit in source order because
   each sits in the NEXT `rand' call's delay slot, so reorg pins them.

   FIRST COMPILE, so nothing above is measured on this body except the word
   count; the twin's facts are measured on the twin. */
void func_s49_80024928(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(u16 *)(*(u8 **)p + 0xE) |= 0x8000;
    func_800478B8(c);
    c[0xC] = (rand() & 0x7F) + 0x20;
    c[0xD] = (rand() & 0x7F) + 0x20;
    c[0xE] = (rand() & 0x7F) + 0x20;
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
