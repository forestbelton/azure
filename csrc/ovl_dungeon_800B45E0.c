#include "common.h"
#include "ovl_dungeon_decls.h"

/* dungeon.bin's overlay, translation unit 2 of 3: func_800B45E0 to the next
 * boundary.  THE SPLIT IS FORCED BY JUMP-TABLE ALIGNMENT, not by size --
 * see splat/dungeon.bin.yaml for the offsets and why each one is where it
 * is.  The prototypes below are what this unit used to get for free by
 * being in the same file as the definitions; they are generated from those
 * definition lines, so they say exactly what the single-unit compile said.
 */

/* Only this unit needs it: the symbol is in
   splat/dungeon.bin_undefined_syms_auto.txt and nothing in csrc/ or include/
   declares it. func_800B45E0's park asks for the same line. */
extern u8 D_80044C54[];

s16 func_800A48F0(s8 *p, s8 kind, s8 c);
void func_800A7030(s16 x, s16 y);
void func_800B1320(u8 *p, s16 a, s16 b);
void func_800B13CC(u8 *p, u16 mask);
u8 *func_800B1484(u8 *p);

/* func_800B45E0 -- 203/203 words, 0 differing.

   THE COLOUR RAMP IS WRITTEN TWICE, NOT SHARED BY A `goto', AND EACH COPY
   HAS ITS OWN LOCALS.  The target's case 1 ends `j .L800B47D0' INTO case 4's
   block, which reads like a shared ramp -- but that is jump2's CROSS-JUMP of
   two identical tails, and jump2 runs AFTER reload, so the two copies had to
   get the same registers first.  Writing one ramp behind a `goto' instead
   puts every quantity in it in two basic blocks, which makes them all global
   pseudos, and global-alloc cannot reproduce the assignment: `goto ramp' is
   15 rows, and one shared name set with the ramp written twice is 16.

   With a copy per case, j1/k1/w1 and j4/k4/w4 are each referenced in ONE
   block, so local-alloc owns them: it ties the numerator (dying at the div)
   to the quotient, giving `mflo $v0', and ties `j' to `k' in `$v1'.  The
   dead REG_UNUSED MOD output of `divmodsi4' is then allocated last (1 ref,
   qty_compare priority 0) and finds $v0 and $v1 busy over its one slot, so
   it takes $a0 -- which costs nothing, because the divisor `n' dies at the
   div and global.c processes REG_DEAD before note_stores.  With the shared
   ramp the MOD was the only local in the block, took $v0, and the quotient
   could never have it (2 rows for a day: `mflo $a3').

   SO THE HEAD'S DUPLICATED-ARM DEVICE HAD TO GO.  The park carried
   `if (k) { *p = NULL; } else { ... }' nested under `if (w)'.  Naming k and
   w on a path from entry made them live-in globals; with the ramp duplicated
   that is 207 words / 9 rows.  A plain statement there is 0, and so are
   `do { } while (0);' and `if (p) { } else { }' -- see docs/matching-levers.md,
   "A DUPLICATED-ARM DEVICE'S CONDITION VARIABLE CAN CREATE THE RESIDUE IT IS
   BLAMED FOR".

   Still load-bearing, each measured on this body:
     * `do { m = *(u16 *)(p + 0xA) - 1; } while (0);' in case 1 AND case 4 --
       it keeps the counter store from sinking past its own sign extension.
       Dropping case 1's is 6 rows, case 4's is 7 (202 words), both 13.
     * `i = t >> 1;' hoisted into its own statement ahead of the `>> 0x11',
       with the store reading `i': undoing it is 204 words / 14 rows.
     * case 3 tests the SHIFTED value (`sll' then `bgtz', no `sra') while
       cases 1 and 4 sign-extend first; that asymmetry is in the target.
     * `abs()' is NOT the answer for the two absolute values -- the target's
       `bgez'/`nop'/`negu' is abssi2's own output, but spelling it `abs()'
       measured 2 rows worse.
   Inert, measured: declaration order of the two ramp groups, `s16' vs `s32'
   for k1/k4, and a separate numerator statement (`u1 = 0x80 - j1; w1 = u1/n;')
   instead of the single expression.

   It owns `jtbl_800892CC' at a 4-mod-8 rodata offset, which is why this unit
   exists; seqcheck reports `HI16 .rodata' against the listing's
   `HI16 jtbl_800892CC', exactly as the landed func_800B490C does. */
void func_800B45E0(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    s32 i;
    s32 n;
    s32 t;
    s32 v;
    u16 m;
    s32 j1;
    s32 k1;
    s32 w1;
    s32 j4;
    s32 k4;
    s32 w4;

    q = *(u8 **)p;
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        *(u8 **)p = NULL;
        *(u8 **)(p + 0x14) = NULL;
    }
    if (*(u8 **)(p + 0x14) != NULL)
    {
        *(u16 *)(b + 0x2) -= *(u16 *)(p + 0x18) - *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(p + 0x18) = *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(b + 0x6) -= *(u16 *)(p + 0x1A) - *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(p + 0x1A) = *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(b + 0xA) -= *(u16 *)(p + 0x1C) - *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
        *(u16 *)(p + 0x1C) = *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
    }
    switch (*(s16 *)(p + 0x8))
    {
    case 0:
        *(s32 *)(b + 0x14) = 0x200000;
        *(u16 *)(b + 0xA) -= 0x40;
        *(s32 *)(p + 0x4) = 0x200000;
        *(u16 *)(c + 0x1E) = 0x1000;
        *(u16 *)(c + 0x1C) = 0x1000;
        func_8004491C(p - 0x20, (u32)&D_80044C54);
        *(u16 *)(p + 0xA) = 0x10;
        *(u16 *)(p + 0x8) += 1;
    case 1:
        do
        {
            m = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = m;
        n = (s16)m;
        if (n > 0)
        {
            j1 = c[0xE];
            w1 = (0x80 - j1) / n;
            k1 = j1 + w1;
            c[0xE] = k1;
            c[0xD] = k1;
            c[0xC] = k1;
            break;
        }
        *(u16 *)(p + 0x8) += 1;
        break;
    case 2:
        break;
    case 3:
        m = *(u16 *)(p + 0xA) - 1;
        *(u16 *)(p + 0xA) = m;
        if ((s32)(m << 16) <= 0)
        {
            *(u16 *)(p + 0xA) = 8;
            *(u16 *)(p + 0x8) += 1;
        }
        break;
    case 4:
        do
        {
            m = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = m;
        n = (s16)m;
        if (n > 0)
        {
            j4 = c[0xE];
            w4 = -j4 / n;
            k4 = j4 + w4;
            c[0xE] = k4;
            c[0xD] = k4;
            c[0xC] = k4;
            break;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
    *(u16 *)(p + 0xC) += 1;
    if ((*(u16 *)(p + 0xC) & 1) != 0)
    {
        t = *(s32 *)(b + 0x14);
        i = t >> 1;
        v = t >> 0x11;
        if (v < 0)
        {
            v = -v;
        }
        *(s32 *)(b + 0x14) = i;
        if (v < 2)
        {
            t = -(*(s32 *)(p + 0x4) >> 1);
            *(s32 *)(p + 0x4) = t;
            *(s32 *)(b + 0x14) = t;
            v = *(s16 *)(p + 0x6);
            if (v < 0)
            {
                v = -v;
            }
            if (v < 2 && *(s16 *)(p + 0x8) < 3)
            {
                *(s16 *)(p + 0x8) = 3;
                *(u16 *)(p + 0xA) = 8;
            }
        }
    }
    if (*(s16 *)(p + 0xE) != 0)
    {
        i = 0x2CF0F0F0;
        if ((*(u16 *)(p + 0xC) & 3) != 0)
        {
            i = 0x2C404040;
        }
        *(s32 *)(c + 0xC) = i;
    }
}

/* 220 words, 0 differing.  The sibling of func_800B45E0, and most of the body
   transferred from it, but three things differ: an extra early-out on the
   +0x10 counter before the +0x14 block, `*(u16 *)(b + 0xA) -= 0x80' in case 0
   where the sibling has 0x40, and cases 1 and 4 do NOT share a ramp here --
   case 1 ramps the bytes c[0xC..0xE] toward 0x80 and case 4 the halfwords
   c+0x1C/0x1E toward zero, so each is written out and case 4's `else' carries
   the `|= 0x8000' tail that in the sibling sits after the switch.

   THIS FUNCTION OWNS jtbl_800892E4 AT 0x800892E4, and the carve is what
   unblocked it: ovl_dungeon_800B45E0's .rodata starts at 0xa3b6c, the table
   sits at 0xa3b84, so inside this unit it is at offset 0x18 -- 0 mod 8 -- and
   cc1psx's `.align 3' is a no-op.  `seqcheck' reports raw == relocdump ==
   masked_words == 13, on_function 0, arith OK, and exactly two sequence
   entries differ: `HI16/LO16 jtbl_800892E4' against `HI16/LO16 .rodata', the
   documented benign class for a compiled jump table.  Every intra-function
   `j' offset, the `jal func_8004491C' and both D_ symbols match in order.

   THREE DEVICES, AND EVERY ONE IS A FREE REFERENCE RATHER THAN A SHAPE.  A
   `do { } while (0);' weights every reference inside it by loop_depth
   (flow.c:2128) and compiles to nothing.

   * one around EACH of the two counter decrements, which stops the
     `sh 0xA($s0)' sinking past its own sign extension.  Re-measured on THIS
     body: both 220 words / 0 rows, case 1's alone 219/7, case 4's alone
     219/6, neither 218/12.
   * one around `i = t >> 1;' in the tail.  That is the last word and the last
     six rows, and WHICH device it is matters far more than that there is one.
     The duplicated-arm `if (n) { i = t >> 1; } else { i = t >> 1; }' this
     function carried for a day measures 220 words / 6 rows, and all six rows
     are ONE REGISTER: the divisor `n' comes out `$a3' where the target has
     `$a0', in BOTH ramps.  THE CAUSE IS THE CONDITION, NOT THE DEVICE.
     Reading `n' in the tail keeps it live on the switch paths that never set
     it -- case 2, case 3 and the jump table's default -- so `n' is live at
     function ENTRY, and global.c's record_one_conflict adds $a0/$a1/$a2, the
     incoming argument registers, to its hard-conflict set.  It already
     conflicts with both outputs of `divmodsi4' because it is live across the
     division: the quotient in $v0, and the dead REG_UNUSED MOD, which
     local-alloc puts in $a0 because $v0 and $v1 are already the quotient and
     the `u'/`k' pseudo.  $a3 is the lowest register left.  The do-while
     weights the same statement without NAMING anything, so `n' never leaves
     cases 1 and 4 and lands in $a0.  Measured at that one statement:
     do-while 220/0; no device at all 219/8; `if (p)', `if (b)', `if (c)' and
     `if (t < 0)' also 220/0; `if (n)' 220/6; `if (t)' 222/3, because the
     condition proves `t == 0' in the else arm and gcc folds `t >> 1' there to
     `move $v1, $zero' instead of cross-jumping the arms together; `if (i)'
     222/8, `if (q)' 222/9, `if (j)' 222/11, `if (k)' 222/21, `if (v)' 221/29,
     `if (u)' 221/38.

   TWO MORE SPELLINGS THAT ARE LOAD-BEARING, both re-measured here:
   `if (p[0xE] != 0)' in the last block is a SIGNED HALFWORD -- the target
   opens it `lh $v0, 0xE($s0)' -- and as a byte it is 220/1.  And case 1's
   ramp keeps its self-assignment, `k = u; k = u + (0x80 - k) / n;', which
   gives the dividend the result variable's register; `k = u + (0x80 - u) / n'
   is 220/19.

   widthsweep and devsweep both come back empty on this body. */
void func_800B490C(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    s32 i;
    s32 n;
    s32 t;
    s32 v;
    s32 j;
    s16 k;
    u16 u;

    q = *(u8 **)p;
    if (q != NULL && (*(u16 *)(q + 0x1E) & 0x8000) != 0)
    {
        *(u8 **)p = NULL;
        *(u8 **)(p + 0x14) = NULL;
    }
    if (*(s16 *)(p + 0x10) != 0)
    {
        *(u16 *)(p + 0x10) -= 1;
        return;
    }
    if (*(u8 **)(p + 0x14) != NULL)
    {
        *(u16 *)(b + 0x2) -= *(u16 *)(p + 0x18) - *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(p + 0x18) = *(u16 *)(*(u8 **)(p + 0x14) + 0x2);
        *(u16 *)(b + 0x6) -= *(u16 *)(p + 0x1A) - *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(p + 0x1A) = *(u16 *)(*(u8 **)(p + 0x14) + 0x6);
        *(u16 *)(b + 0xA) -= *(u16 *)(p + 0x1C) - *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
        *(u16 *)(p + 0x1C) = *(u16 *)(*(u8 **)(p + 0x14) + 0xA);
    }
    switch (*(s16 *)(p + 0x8))
    {
    case 0:
        *(s32 *)(b + 0x14) = 0x200000;
        *(u16 *)(b + 0xA) -= 0x80;
        *(s32 *)(p + 0x4) = 0x200000;
        *(u16 *)(c + 0x1E) = 0x1000;
        *(u16 *)(c + 0x1C) = 0x1000;
        func_8004491C(p - 0x20, (u32)&D_80044C54);
        *(u16 *)(p + 0xA) = 0x10;
        *(u16 *)(p + 0x8) += 1;
    case 1:
        do
        {
            k = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = k;
        n = (s16)k;
        if (n > 0)
        {
            u = c[0xE];
            k = u;
            k = u + (0x80 - k) / n;
            c[0xE] = k;
            c[0xD] = k;
            c[0xC] = k;
            break;
        }
        *(u16 *)(p + 0x8) += 1;
        break;
    case 2:
        break;
    case 3:
        k = *(u16 *)(p + 0xA) - 1;
        *(u16 *)(p + 0xA) = k;
        if ((s32)(k << 16) <= 0)
        {
            *(u16 *)(p + 0xA) = 8;
            *(u16 *)(p + 0x8) += 1;
        }
        break;
    case 4:
        do
        {
            k = *(u16 *)(p + 0xA) - 1;
        } while (0);
        *(u16 *)(p + 0xA) = k;
        n = (s16)k;
        if (n > 0)
        {
            u = *(u16 *)(c + 0x1E);
            k = u + -(s32)u / n;
            *(u16 *)(c + 0x1E) = k;
            *(u16 *)(c + 0x1C) = k;
            *(u16 *)(b + 0xA) += 6;
        }
        else
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
    *(u16 *)(p + 0xC) += 1;
    if ((*(u16 *)(p + 0xC) & 1) != 0)
    {
        t = *(s32 *)(b + 0x14);
        do
        {
            i = t >> 1;
        } while (0);
        v = t >> 0x11;
        if (v < 0)
        {
            v = -v;
        }
        *(s32 *)(b + 0x14) = i;
        if (v < 2)
        {
            t = -(*(s32 *)(p + 0x4) >> 1);
            *(s32 *)(p + 0x4) = t;
            *(s32 *)(b + 0x14) = t;
            v = *(s16 *)(p + 0x6);
            if (v < 0)
            {
                v = -v;
            }
            if (v < 2 && *(s16 *)(p + 0x8) < 3)
            {
                *(s16 *)(p + 0x8) = 3;
                *(u16 *)(p + 0xA) = 8;
            }
        }
    }
    if (*(s16 *)(p + 0xE) != 0)
    {
        j = 0x2CF0F0F0;
        if ((*(u16 *)(p + 0xC) & 3) != 0)
        {
            j = 0x2C404040;
        }
        *(s32 *)(c + 0xC) = j;
    }
}

/* The dungeon's floating damage/EXP/G number: a 0x212 node whose +0x10 step
   is func_800B490C or func_800B45E0, with the caller's record parked at
   +0x20 and the glyph string built in a 0x10-byte stack buffer.

   FOUR SPELLINGS, EACH MEASURED.
   (1) The FIRST parameter is NARROW (`u16`), not `s32`. The target copies
       $a0 into $s1 AND makes a second copy in $s7 which only the
       `(a & 0xF0) < 0x80` test reads -- that is the narrow-parameter raw
       copy of docs/matching-levers.md :5157, and $s1 is then free to be
       reused for the switch's 0x50 constant. As `s32 a` (with or without a
       hand-written `fl = a` copy, which cse coalesces) the function is 221
       words: gcc keeps one register for `a`, the fourth parameter stays in
       $fp instead of being spilled to 0x20($sp), and the frame comes out
       0x48 instead of 0x50.
   (2) `u16`, not `s16`, because `(a & 0xF0) < 0x80` must be `sltiu`. With
       `s16 a` the sign extension stops combine proving the masked value
       non-negative and all five copies of the test are `slti` -- 5 differing
       for free. See "A MASKED COMPARE IS UNSIGNED" in matching-levers.
       `(u16)a`, `(u8)(a & 0xF0)` and `(u32)(a & 0xF0)` at the test site are
       byte-identical to the `u16` parameter.
   (3) THE POSITIVE ARM IS WRITTEN FIRST: `if (v >= 0) { switch } else
       { the D_800DF24C path }`. Written the other way round the negative
       block is laid out inline behind a `bgez` and the function is 225 words
       at 195 differing -- the target's `bltz` puts it out of line past the
       switch.
   (4) The three (x, z, y) copies are CHAINED assignments
       (`q[0x18] = m->unk_02 = *(u16 *)(*(u8 **)(q + 0x14) + 0x2)`). As two
       statements the `*(u8 **)(q + 0x14)` load and the `lhu` are each done
       twice, because the `sh` through `m` invalidates cse's entry for a
       store through `q`: 232 words.
   228 words, 0 differing. */
UnkNode *func_800B4C7C(u16 a, u8 *p, s16 v, s16 k)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *g;
    u8 *u;
    u8 *s;
    u8 *e;
    u8 *h;
    s32 fl;
    s32 lo;
    s32 lo2;
    s32 c;
    s16 i;
    u8 buf[0x10];

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (u8 *)n->unk_0C;
        e = p - 0x20;
        *(u8 **)((u8 *)n + 0x20) = e;
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x14) = *(u32 *)(e + 0x8);
        *(s16 *)(q + 0x18) = *(s16 *)((u8 *)m + 0x2) = *(u16 *)(*(u8 **)(q + 0x14) + 0x2);
        *(s16 *)(q + 0x1A) = *(s16 *)((u8 *)m + 0x6) = *(u16 *)(*(u8 **)(q + 0x14) + 0x6);
        *(s16 *)(q + 0x1C) = *(s16 *)((u8 *)m + 0xA) = *(u16 *)(*(u8 **)(q + 0x14) + 0xA);
        *(u32 *)(r + 0x8) = (u32)((u8 *)n + 0x40);
        g = (u8 *)n + 0x40;
        lo = a & 0xF;
        lo2 = lo;
        *(s16 *)(r + 0x6) = 0x20;
        if ((a & 0x8000) != 0)
        {
            *(s16 *)(q + 0xE) = 1;
        }
        if (v >= 0)
        {
            s = buf;
            switch (a & 0xF0)
            {
            case 0x10:
                c = 0x7F;
                goto pre;
            case 0x20:
                c = 0x80;
                goto pre;
            case 0x30:
                buf[0] = 0x2B;
                break;
            case 0x40:
                c = 0x2D;
                goto pre;
            case 0x50:
                buf[0] = 0x81;
                break;
            case 0x60:
                c = 0x82;
            pre:
                buf[0] = c;
                s = &buf[1];
                break;
            case 0x80:
                func_8004E634(v, s);
                s = s + strlen(s);
                *s = 0x45;
                s++;
                *s = 0x58;
                s++;
                *s = 0x50;
                s++;
                *s = 0;
                s++;
                func_8004E298(g, buf, lo);
                break;
            case 0xA0:
                buf[0] = 0x20;
                func_8004E5A0(v, 3, &buf[1]);
                s = &buf[6];
                buf[4] = 0x25;
                buf[5] = 0;
                func_8004E298(g, buf, lo);
                break;
            case 0x90:
                func_8004E5A0(v, 4, s);
                s = &buf[6];
                buf[4] = 0x47;
                buf[5] = 0;
                func_8004E298(g, buf, lo);
                break;
            }
            if ((a & 0xF0) < 0x80)
            {
                func_8004E5A0(v, 3, s);
                func_8004E298(g, buf, lo2);
            }
        }
        else
        {
            i = ~v;
            if (i > 0)
            {
                *(s16 *)(q + 0x10) = 0x18;
            }
            func_8004E298(g, D_800DF24C[i], lo);
        }
        if (k == 1)
        {
            n->unk_10 = (u32)func_800B490C;
        }
        else
        {
            n->unk_10 = (u32)func_800B45E0;
        }
        /* DEVICE: a zero-instruction reference on `g` ($s3), worth the whole
           register allocation. `g` has 7 references over 142 insns, which scores
           floor_log2(7)*7/142 = 0.099 in global.c's allocno_compare, behind the
           `a & 0xF` local's 6/72 = 0.167 and the node's 10/188 = 0.160; the
           target wants g, then the local, then the node, in $s3/$s4/$s5, and one
           more reference takes g to floor_log2(8)*8/142 = 0.169, which is above
           the local and below the next allocno up (0.190). Without it the
           function is 228 words at 22 differing -- every opcode, immediate and
           displacement already right, three saved registers rotated. Warns under
           -Wall? No; it reads an initialised variable. Legal spellings ruled out,
           all measured: assigning `g` BEFORE the `r->unk_08` store and storing
           `(u32)g` (227 words, the `addu $s3, $v0, $zero` disappears, 195
           differing); the same with the +0x10 store as a ternary (227/195);
           `g = q + 0x20` or `n->payload + 0x20` (228/22, no change);
           `u = (u8 *)n + 0x40` instead of `q + 0x20` (228/17 -- fixes the local
           but swaps g and n); `u = g` (227/35); passing `g` to func_800B13CC
           (227/55) or to func_800B1320 (227/48); `lo2 = a & 0xF` instead of
           `lo2 = lo` (228/22). A second marker is byte-identical to one. */
        __asm__("" ::"r"(g));
        h = func_800B1484(g);
        u = q + 0x20;
        func_800B1320(u, (s16)(0x80 - (*(s8 *)(h + 0x2) + 0x88) / 2),
                      (s16)(-*(s8 *)(q + 0x23) - 4));
        func_800B13CC(u, 0x20);
    }
    return n;
}

/* Find the spot whose tile is (x, y) and whose +0x12 facing is within 0x40
   of c, over the same 0x20-entry pair of tables func_800B6008 sweeps.
   71 words, 0 differing.

   TWO THINGS WERE MEASURED HERE. (1) The +0x12 field is READ WITH `lh`, so
   UnkDungeonSpotB.unk_12 is `s16`; spelled `*(s16 *)&...unk_12` off the u16
   field it is a SECOND induction variable biased by 0x12 -- 74 words against
   71, the extra `addiu $a3, $v0, 0x12` / `addiu $a3, $a3, 0x18` pair plus a
   load-delay nop. Both existing users of the field only STORE through it and
   both still measure 0 differing. (2) The 0x20 mask test is a NESTED `if`
   around the loop, not a third early `return -1`: as an early return it gets
   its own `j`/`addiu -1` pair instead of sharing the tail's, and the first
   return then collapses into a direct `beqz` -- 11 differing, all layout. */
s16 func_800B500C(s16 x, s16 y, s16 c)
{
    s32 i;
    s32 d;
    u16 m;

    if (func_8009A350(x - 1, y, 0, &m) == 0)
    {
        return -1;
    }
    if (m & 0x20)
    {
        for (i = 0; i < 0x20; i++)
        {
            if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x &&
                D_800E39C8[i].unk_07 == y)
            {
                d = c - D_800E39C8[i].unk_12;
                d = abs(d);
                if (d < 0x40)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

s16 func_800B5128(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3648; i < 0x20; i++, p += 4)
    {
        if (p[1] == 0)
        {
            return i;
        }
    }
    return -1;
}

void func_800B5168(void)
{
    UnkNode *n;
    UnkDungeonSprite *s;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        n->unk_10 = 0;
        func_8004491C(n, (u32)func_800B51C8);
        s->unk_0E = 0x80;
        s->unk_0D = 0x80;
        s->unk_0C = 0x80;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon_800B45E0", func_800B51C8);

/* Step one tile along the eight-way direction in `dir` and claim the spot
   there. TWO OF THE THREE `return -1`s SHARE THE EXIT: written as three early
   returns the first takes its own `li $v0, -1` in the `bltz`'s delay slot and
   the function is 45 words of 44, 22 differing; wrapping the body in
   `if (k >= 0) { ... }` with one trailing `return -1` leaves exactly the two
   copies the target has. 44 words, 0 differing. */
s32 func_800B5ED0(s16 x, s16 y, u16 dir, s16 w)
{
    s32 i;
    s16 k;
    u8 f;

    i = (dir >> 9) & 7;
    k = func_800B500C((s16)(x + ((u16 *)D_8006CCD8)[i]), (s16)(y + ((u16 *)D_8006CCE8)[i]), w);
    if (k >= 0)
    {
        f = D_800E3648_s[k].unk_03;
        if (f & 0x40)
        {
            return -1;
        }
        if (f & 0x80)
        {
            D_800E3648_s[k].unk_03 = f & 0x7F;
            return k;
        }
    }
    return -1;
}

/* Free slot `i` of the second spot table: clear its flag word, play the
   effect at its tile and drop the renderable it holds at +0x0C.

   THE SUBSCRIPT IS WRITTEN AT EVERY SITE, not hoisted into a pointer local.
   `q = &D_800E39C8[i]` is 34 words / 9 differing -- `&arr[i]` gives a
   BASE-FIRST `addu` and materialises the symbol after the index, where the
   target has the `lui`/`addiu` first and an index-first
   `addu $s0, $v0, $v1`. 34 words, 0 differing. */
void func_800B5F80(s32 i)
{
    UnkNode *n;

    *(s32 *)&D_800E3648_s[i] = 0;
    func_8009A3D0(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, 0x20);
    n = D_800E39C8[i].unk_0C;
    if (n != NULL)
    {
        n->unk_1E |= 0x8000;
        D_800814A0 |= 0x8000;
        D_800E39C8[i].unk_0C = NULL;
    }
}

/* func_800A7030 over the OTHER pair of tables -- 0x20 entries at stride 0x18,
   tile at +0x06/+0x07 -- handing the index to func_800B5F80 rather than
   clearing the slot itself. 44 words, 0 differing. */
void func_800B6008(s16 x, s16 y)
{
    s32 i;

    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            func_800B5F80(i);
        }
    }
}

/* Claim a spot slot for the entity at (x, y): ask func_800B500C for the one
   already at that tile, fall back to func_800B5128's first free slot and then
   to 0x1F, and fill the two parallel tables.

   THE FIRST FOUR PARAMETERS ARE s16, AND THE UNIT'S DECLARATION SAID s32.
   The three `sll 16`/`sra 16` pairs in the prologue are the s16 parameters'
   sign-extension at their first int use (the func_800B500C call); with the
   declared `(s32, s32, s16, s32, u8 *)` gcc emits none of them and the
   function is 77 words of 83. `d`'s own width is witnessed by the fused
   `sll 16`/`sra 14` of the D_800DF258 index. The declaration at the top of
   the unit was widened to match and func_800C2F2C, its one landed caller,
   re-measures at 106 words / 0 differing. 83 words, 0 differing. */
s32 func_800B60B8(s16 x, s16 y, s16 c, s16 d, u8 *e)
{
    s16 k;

    k = func_800B500C(x, y, c);
    if (k < 0)
    {
        k = func_800B5128();
        if (k < 0)
        {
            k = 0x1F;
        }
    }
    func_800B5F80(k);
    D_800E3648_s[k].unk_01 = 0x15;
    D_800E3648_s[k].unk_03 = 0xC0;
    D_800E3648_s[k].unk_00 = d;
    D_800E3648_s[k].unk_02 = 0;
    D_800E39C8[k].unk_06 = x;
    D_800E39C8[k].unk_07 = y;
    func_8009A21C(x, y, 0x20);
    D_800E39C8[k].unk_10 = c;
    D_800E39C8[k].unk_14 = 0;
    D_800E39C8[k].unk_0C = (UnkNode *)e;
    D_800E39C8[k].unk_08 = (s32)D_800DF258[d];
    return k;
}

/* 9 words, 0 differing. */
s32 func_800B6204(s8 *p)
{
    func_800A48F0(p, 7, 0x10);
    return 1;
}

/* 21 words, 0 differing. */
s32 func_800B6228(u8 *p)
{
    if ((s16)func_80042900((s8 *)p, 2))
    {
        func_80042B68((s8 *)p, 2);
        func_80099844(p, D_800E0DF5);
    }
    return 1;
}

/* Dispatch to the per-kind hatch in D_800DF2A8, and on a non-zero answer
   tear the old view down (only while D_800E3D40 survived the hatch) and
   re-dress the spot's renderer from D_800DF258 when its +0x14 carries 0x40.
   The +0x14 flags word is a field of UnkDungeonSpotB added for this; the
   +0x24-stride byte beside it is the flat `D_800E3648[idx * 4]` spelling,
   which is what gives the `sll 2` / `addu` / `lbu 0` the target has.
   75 words, 0 differing, first compile. */
s32 func_800B627C(s16 kind, u8 *b, s16 idx, u8 flag)
{
    s32 r;
    u8 *g;

    D_800E3D40 = flag;
    r = D_800DF2A8[kind](b, idx);
    if (r != 0)
    {
        if (D_800E3D40 != 0)
        {
            func_80099F70(*(u8 **)(b + 0x5C));
            func_80099F04(*(u8 **)(b + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x812;
        }
        if (D_800E39C8[idx].unk_14 & 0x40)
        {
            func_8003DB94(&D_800E39C8[idx], D_800DF258[D_800E3648[idx * 4]], 0);
            D_800E39C8[idx].unk_14 &= 0xF7FF;
        }
    }
    return r;
}

/* A particle's per-frame step: accelerate the three deltas from the record's
   own three constants (the third scaled by the frame counter), advance the
   position by them, then bleed the renderable's three colour bytes by the
   record's three rates, clamping each at 0x80 when it wraps. 101 words,
   0 differing on the first compile.

   The clamp is `if (c[N] > 0x80) c[N] = 0x80;` on a `u8` field, which is the
   `andi 0xFF` / `sltiu 0x81` / `bnez` triple -- the mask is on the register
   cse forwarded from the `sb`, not a reload. */
void func_800B63A8(u8 *p, UnkVec *v, u8 *c)
{
    s16 t;

    v->unk_0C += *(s32 *)p;
    v->unk_10 += *(s32 *)(p + 0x4);
    v->unk_14 += *(s32 *)(p + 0x8) * *(s16 *)(p + 0x10);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    func_800478B8(c);
    if (c[0xC] != 0)
    {
        c[0xC] -= p[0x14];
        if (c[0xC] > 0x80)
        {
            c[0xC] = 0x80;
        }
    }
    if (c[0xD] != 0)
    {
        c[0xD] -= p[0x16];
        if (c[0xD] > 0x80)
        {
            c[0xD] = 0x80;
        }
    }
    if (c[0xE] != 0)
    {
        c[0xE] -= p[0x18];
        if (c[0xE] > 0x80)
        {
            c[0xE] = 0x80;
        }
    }
    t = *(u16 *)(p + 0xE) - 1;
    *(s16 *)(p + 0xE) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_800B653C(u8 *t, s16 ang)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *v;
    s32 i;

    i = (ang >> 9) & 7;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (u8 *)n->unk_0C;
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B63A8;
        func_8004491C(n, (u32)D_80045340);
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(t + 0x2) - D_8006CCD8[i] * 16;
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(t + 0x6) - D_8006CCE8[i] * 16;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(t + 0xA);
        m->unk_0C = -(*(s32 *)(t + 0xC) * ((rand() & 1) + 2));
        m->unk_10 = -(*(s32 *)(t + 0x10) * ((rand() & 1) + 2));
        m->unk_14 = (~rand() & 1) << 15;
        func_8003DB94(r, D_800DEA68, 0);
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x10) = 0x60;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(v + 0x18) = 0x10;
        *(s16 *)(v + 0x16) = 0x10;
        *(s16 *)(v + 0x14) = 0x10;
        *(s16 *)(v + 0xE) = (rand() & 3) | 4;
        *(s32 *)(v + 0x8) = 0x800;
    }
}

/* Burst of debris around the view at (p+0x2, p+0x6): a terrain probe first,
   then either func_800B6814's single effect when the tile carries 0x400 or
   4..11 particles spread over a full turn of angle.

   func_800B653C's SECOND PARAMETER IS s16, NOT THE u16 IT WAS LANDED WITH.
   The call site sign-extends the angle accumulator (`sll 16`/`sra 16`); a u16
   parameter takes `andi $a1, 0xFFFF` instead, which is 82 words of 83 and 22
   differing. func_800B653C re-measures at 99 words / 0 differing under the
   narrowed declaration, because its only use of the parameter is
   `(ang >> 9) & 7` and bits 9..11 of a 16-bit value survive both shifts.
   The `t[0xA] = p[0xA]` store must also be written BEFORE the `t[0x16] = -4`
   one: the other order is 4 differing, the load being hoisted past the
   constant. 83 words, 0 differing. */
void func_800B66C8(u8 *p)
{
    u8 t[0x18];
    u16 m;
    s32 i;
    s32 n;
    s32 step;
    s32 ang;

    func_8009A350(*(s16 *)(p + 0x2) / 64 - 1, *(s16 *)(p + 0x6) / 64, 0, &m);
    if (m & 0x400)
    {
        func_800B6814(p);
    }
    else
    {
        n = (rand() & 7) | 4;
        step = 0x1000 / n;
        ang = rand();
        *(s16 *)(t + 0xA) = *(u16 *)(p + 0xA);
        *(s16 *)(t + 0x16) = -4;
        for (i = 0; i < n; i++)
        {
            *(s16 *)(t + 0x2) = *(u16 *)(p + 0x2) + (rand() & 0x1F) - 0x10;
            *(s16 *)(t + 0x6) = *(u16 *)(p + 0x6) + (rand() & 0x1F) - 0x10;
            *(s32 *)(t + 0xC) = func_80064584(ang) << 5;
            *(s32 *)(t + 0x10) = func_800644B8(ang) << 5;
            func_800B653C(t, ang);
            ang += step;
        }
    }
}

/* Burst 8..15 0x212 sparks evenly around the circle from one random start
   angle: each gets func_800B63A8 as its step, a position offset by
   (cos, sin) >> 11, a velocity of (cos, sin) << 6 with a -0x30000 climb,
   and a drag record at +0x20 holding the negated velocity >> 6. The count
   is `(rand & 0xF) | 8`, which is why the loop's entry guard is `beqz` and
   not `blez` -- nonzero_bits proves it is in [8, 15]. 114 words,
   0 differing on the first compile. */
void func_800B6814(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s32 cnt;
    s32 step;
    s32 ang;
    s32 i;

    cnt = (rand() & 0xF) | 8;
    step = 0x1000 / cnt;
    ang = rand();
    for (i = 0; i < cnt; i++, ang += step)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            m = n->unk_08;
            r = (u8 *)n->unk_0C;
            *(u32 *)((u8 *)n + 0x10) = (u32)func_800B63A8;
            func_8004491C(n, (u32)D_80045340);
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(p + 0x2) + (func_80064584(ang) >> 11);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(p + 0x6) + (func_800644B8(ang) >> 11);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(p + 0xA);
            *(s32 *)((u8 *)m + 0xC) = func_80064584(ang) << 6;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(ang) << 6;
            *(s32 *)((u8 *)m + 0x14) = 0xFFFD0000;
            q = (u8 *)n + 0x20;
            *(s32 *)(q + 0x0) = -*(s32 *)((u8 *)m + 0xC) >> 6;
            *(s32 *)(q + 0x4) = -*(s32 *)((u8 *)m + 0x10) >> 6;
            *(s32 *)(q + 0x8) = 0x2000;
            func_8003DB94(r, D_800DEC00, 0);
            *(s32 *)(r + 0xC) = 0x808080;
            *(s16 *)(r + 0x1E) = 0x400;
            *(s16 *)(r + 0x1C) = 0x400;
            *(s16 *)(r + 0x10) = 0x60;
            *(s16 *)(r + 0x12) = 0x7E80;
            *(u16 *)(r + 0x14) |= 0x10C;
            *(s16 *)(q + 0xE) = (rand() & 3) | 0x10;
            *(s16 *)(q + 0x16) = 0x10;
            *(s16 *)(q + 0x14) = 0x10;
            *(s16 *)(q + 0x18) = 8;
        }
    }
}

/* The +0x64 halfword is read ONCE into an s32 local and tested twice (`bltz`
   then `blez`), and the `&= 0xF7FF` is written out in both arms --
   cross-jumping is what keeps that one copy. 58 words, 0 differing. */
void func_800B69DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 t;

    t = *(s16 *)(d + 0x64);
    if (t < 0 || (*(u16 *)(p + 0x10C) & 1) != 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_8008CAA0(p, b, c, d);
    }
    else
    {
        if (t > 0)
        {
            func_8008CBA0(p, b, c, d);
        }
        if (p[0x9A] != 0xD && (*(u32 *)(d + 0x1C) & 0x200) != 0)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_80090200(p, b, c, d);
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon_800B45E0", D_800892F8);
