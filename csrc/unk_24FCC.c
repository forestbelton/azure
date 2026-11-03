#include "common.h"

/* func_800517CC and what follows it, split off from the first piece of the
   carved unit unk_1C540 so that jtbl_8002E964 gets a `.rodata` of its own.
   A translation unit emits ONE contiguous `.rodata`, and once func_8004CECC
   matched, that piece owned two separated runs: jtbl_8002E5F8 at 0x8002E5F8
   and this one at 0x8002E964, 0x354 bytes of Shift-JIS text apart. The piece
   above keeps the lower run. See csrc/unk_25944.c, which is the same cut made
   at func_80052144 for the same reason, and the jump-table section of
   docs/open-matching-problems.md for which piece carries which run.

   The tail moved here needs nothing from the head of the undivided file: it
   uses none of its typedefs, none of its five `__asm__`-aliased declarations,
   and every function it calls it declares itself (the two names that appear
   only above the cut, func_80051548 and func_80051D18, occur here inside
   comments and nowhere else). It needs none of the head's `.size` records
   either: the block the cut copied here registered nineteen symbols that no
   function in this piece touches, and it has been removed. */

/* func_80051548's bigger sibling: the same cutscene camera driver, eight
   states instead of four, dispatched through jtbl_8002E964 on the `s32` copy
   of +0x04. It takes func_80051548's whole recipe -- the switch subject
   through an `s32` local, `func_800478B8` with ONE argument, the default's
   three byte stores written out, the states that stop with `return` and the
   two that fall out of the switch to the shared tail -- and adds the damped
   interpolation in cases 5 and 6, which is where all the work was.

   Each of the four interpolation blocks is `b = goal; c = pos - BASE;
   pos_delta += (b - c) / 2; pos += pos_delta;` with **b and c declared in a
   brace of their own**, and all three parts are load-bearing:

   * The BASE must be subtracted as a signed constant (`- 0xD80000`), not
     added as the listing's literal `+ 0xFF280000`. The hex literal is
     unsigned, which makes the whole subtraction unsigned and turns the
     `/ 2` into a single `srl` -- eight words short.

   * `c` has to be a LOCAL, because the listing loads `pos` AFTER the
     func_80064584 call. Written inline, fold's `associate:` case runs
     `split_tree` on the second operand, finds (var, const) and rewrites
     `goal - (pos - BASE)` into `(goal + BASE) - pos` -- the same four
     instructions with the constant folded into the wrong operand. Two
     VAR_DECLs give it nothing to split. And `b` has to be a local too, or the
     statement that computes `c` sits ahead of the call.

   * The braces are the difference between MATCH and 32 register-only words.
     Function-scope `b` and `c` are two allocnos whose references sum over all
     four blocks, so their priority is inflated and the product takes $a0
     where the listing reuses $v1; one brace per case is still 28. The lever
     is docs/matching-levers.md's one-variable-per-loop, applied per block.

   The `+=` spelling of the two stores matters as well: with an explicit
   `a = pos_delta + ...` local the second store reads `a` instead of the
   stored word and the register permutation comes back. */
s32 rand(void);

void func_800517CC(u8 *p, u8 *q, u8 *r)
{
    s32 s;

    *(u16 *)(p + 6) = *(u16 *)(p + 6) + 1;
    func_800478B8((s32)r);
    s = *(s16 *)(p + 4);
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 1:
        if (*(s16 *)(p + 6) >= 0x60)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 2:
        *(s32 *)(q + 0) = 0x1A00000 - ((func_800644B8(*(s16 *)(p + 6) * 0x10) >> 4) << 16);
        *(s32 *)(q + 4) = ((func_80064584(*(s16 *)(p + 6) * 0x10) >> 4) << 14) + 0x200000;
        if (*(s16 *)(p + 6) >= 0x40)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 3:
        *(s32 *)(q + 0) = 0xC00000 - ((func_80064584(*(s16 *)(p + 6) * 0x20) >> 4) << 13);
        *(s32 *)(q + 4) = ((func_800644B8(*(s16 *)(p + 6) * 0x20) >> 4) << 12) + 0x200000;
        if (*(s16 *)(p + 6) >= 0x20)
        {
            *(s32 *)(q + 8) = 0x800000;
            *(s16 *)(p + 8) = rand() % 0x40 + 0x20;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 4:
        if (*(s16 *)(p + 6) >= *(s16 *)(p + 8))
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = 7;
        }
        return;
    case 5:
        {
            s32 b;
            s32 c;

            b = (func_80064584((*(s16 *)(p + 6) + 0x100) * 8) >> 4) * 0x1800;
            c = *(s32 *)(q + 0) - 0xD80000;
            *(s32 *)(q + 0xC) = *(s32 *)(q + 0xC) + ((b - c) / 2);
            *(s32 *)(q + 0) = *(s32 *)(q + 0) + *(s32 *)(q + 0xC);
        }
        {
            s32 b;
            s32 c;

            b = (func_80064584(*(s16 *)(p + 6) * 8) >> 4) << 10;
            c = *(s32 *)(q + 4) - 0x2C0000;
            *(s32 *)(q + 0x10) = *(s32 *)(q + 0x10) + ((b - c) / 2);
            *(s32 *)(q + 4) = *(s32 *)(q + 4) + *(s32 *)(q + 0x10);
        }
        if (*(s16 *)(p + 6) >= 0x100)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = 7;
        }
        return;
    case 6:
        {
            s32 b;
            s32 c;

            b = (func_80064584(*(s16 *)(p + 6) * 0x10) >> 4) * 0x1800;
            c = *(s32 *)(q + 0) - 0xD80000;
            *(s32 *)(q + 0xC) = *(s32 *)(q + 0xC) + ((b - c) / 2);
            *(s32 *)(q + 0) = *(s32 *)(q + 0) + *(s32 *)(q + 0xC);
        }
        {
            s32 b;
            s32 c;

            b = (func_80064584((*(s16 *)(p + 6) + 0x80) * 0x10) >> 4) << 10;
            c = *(s32 *)(q + 4) - 0x2C0000;
            *(s32 *)(q + 0x10) = *(s32 *)(q + 0x10) + ((b - c) / 2);
            *(s32 *)(q + 4) = *(s32 *)(q + 4) + *(s32 *)(q + 0x10);
        }
        if (*(s16 *)(p + 6) >= 0x80)
        {
            *(s16 *)(p + 8) = rand() % 0x40 + 0x20;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = 4;
        }
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = 7;
        }
        return;
    case 7:
        *(s32 *)(q + 0) = *(s32 *)(q + 0) + 0xFFF40000;
        *(s32 *)(q + 4) = *(s32 *)(q + 4) + 0x20000;
        if (*(s16 *)(p + 6) < 0x40)
        {
            return;
        }
        break;
    default:
        r[0xE] = 0;
        r[0xD] = 0;
        r[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0x10) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

extern UnkElem D_800720AC[];
void func_800517CC(u8 *, u8 *, u8 *);
void func_80044BB0(void);

void func_80051BFC(u8 *p, s32 a) {
    u8 *q;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_800517CC;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 8) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0x808080;
    func_8003DB94((UnkSlot *) e, D_800720AC, 0);
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0x1A00000;
    *(u32 *) (f + 4) = 0x600000;
    *(u32 *) (f + 8) = 0x700000;
}

void func_80051CC4(u8 *, s32, u8 *);

void func_80051CA4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051CC4) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

extern u32 D_80071A68[];

/* func_80051F58's family again, and the same two declaration facts carry it:
   the switch subject as `s32`, and the UNSIGNED read of +4 as a second local
   that only case 2 uses. The extras here:

   `a` is `u16` and the `!= 7` test and the `% 7` are on `(s16) a`. The listing
   stores the unextended value to +0xA (`sh $v0`) and sign-extends a COPY for
   the comparison and the division, so the cast goes on the uses and not on the
   variable.

   The table index is `s16`, which is what folds `sll 16` / `sra 16` / `sll 2`
   into the listing's `sll 16` / `sra 14`. Same spelling as func_80051D18's
   `D_80071A68[*(s16 *) (q + 0xA)]` below. */
void func_80051CC4(u8 *p, s32 b, u8 *q)
{
    s32 s;
    u16 w;
    u16 n;
    u8 v;
    u16 a;
    s16 m;

    n = *(u16 *)(p + 6) + 1;
    s = *(s16 *)(p + 4);
    w = *(u16 *)(p + 4);
    *(u16 *)(p + 6) = n;
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 1:
        v = q[0xE] + 4;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) >= 0x20)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 2:
        if ((s16)n >= 0x3F)
        {
            *(u16 *)(p + 4) = w + 1;
            *(u16 *)(p + 6) = 0;
        }
        return;
    case 3:
        v = q[0xE] - 4;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) < 0x20)
        {
            return;
        }
        a = *(u16 *)(p + 0xA) + 1;
        *(u16 *)(p + 0xA) = a;
        if ((s16)a != 7)
        {
            m = (s16)a % 7;
            *(u16 *)(p + 0xA) = m;
            *(u32 *)(q + 8) = D_80071A68[m];
            *(s16 *)(p + 4) = 0;
            *(u16 *)(p + 6) = 0;
            return;
        }
        break;
    default:
        q[0xE] = 0;
        q[0xD] = 0;
        q[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0x14) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

extern u32 D_80071A68[];
void func_80051CC4(u8 *, s32, u8 *);
void func_80044BB0(void);

void func_80051E94(u8 *p, s32 a) {
    u8 *q;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_80051CC4;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u16 *) (q + 0xC) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0;
    *(u32 *) (e + 8) = D_80071A68[*(s16 *) (q + 0xA)];
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0x380000;
    *(u32 *) (f + 4) = 0x280000;
    *(u32 *) (f + 8) = 0x900000;
}

void func_80051F58(u8 *, s32, u8 *);

void func_80051F38(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051F58) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

/* Three declaration facts, each worth a word or two:

   * The switch subject is `s32`, not `s16`. As an `s16` the pseudo is HImode,
     so the read is a `lhu` and the switch sign-extends it with `sll`/`sra`
     where the target has one `lh`.

   * `w`, the UNSIGNED read of the same halfword, is a second load in the
     listing -- `lh $v1, 0x4($a0)` and `lhu $a3, 0x4($a0)` back to back --
     because cse relates a sign-extend and a zero-extend of one `(mem:HI)`
     only one way: with both reads before the +6 store, whichever it folds
     costs the `sll`/`sra` pair. Reading `w` before the store works only with
     the `s32` spelling above; reading it after the store also works (the
     store invalidates all memory, both addresses being p-relative bare
     dereferences), and is a word worse.

   * The three byte stores take ONE value in a local. `q[0xD] = q[0xE]` after
     the store to +0xE re-reads it: two words in each of the two arms.

   The cases are written 0, 1, 2, 3, default, which is the listing's physical
   order -- case 0's tail is cross-jumped into case 1's, so case 0 has to come
   first. */
void func_80051F58(u8 *p, s32 b, u8 *q)
{
    s32 s;
    u16 w;
    u16 n;
    u8 v;

    n = *(u16 *)(p + 6) + 1;
    s = *(s16 *)(p + 4);
    w = *(u16 *)(p + 4);
    *(u16 *)(p + 6) = n;
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 1:
        v = q[0xE] + 2;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) >= 0x40)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 2:
        if ((s16)n >= 0x100)
        {
            *(u16 *)(p + 4) = w + 1;
            *(u16 *)(p + 6) = 0;
        }
        return;
    case 3:
        v = q[0xE] - 2;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) < 0x40)
        {
            return;
        }
        break;
    default:
        q[0xE] = 0;
        q[0xD] = 0;
        q[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0x18) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80051F58(u8 *, s32, u8 *);
void func_80044BB0(void);

/* func_8004491C's first argument is the pass-through: nothing sets $a0 before
   that `jal`, so `p` is still there. */
void func_800520B4(u8 *p, s32 a) {
    u8 *q;
    u8 *e;

    *(u32 *) (p + 0x10) = (u32) func_80051F58;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u16 *) (q + 0xC) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 8) = (u32) D_80071FF0;
    *(u32 *) (e + 0xC) = 0;
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    e = *(u8 **) (p + 8);
    *(u32 *) (e + 0) = 0x980000;
    *(u32 *) (e + 4) = 0x800000;
    *(u32 *) (e + 8) = 0xA00000;
}

void func_80051528(u8 *);
void func_800517AC(u8 *);
void func_80051CA4(u8 *);
void func_80051F38(u8 *);
void func_800530A4(u8 *);
void func_800530C4(u8 *);
s32 func_80053EF0(s32);
void func_80041094(u16, s16, s16, s16, u16);
