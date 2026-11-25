/* PARKED: func_b24_801717D0 (ovl_dng_b24), 172 target words.
   BEST: 172 candidate words, 26 aligned differences (aligndiff), with the
   shape fully settled -- the word count is exact and every instruction is in
   place.  1 copy (dngfam.py).  No jump table and no rodata of its own.
   agent h170, batch 35.

   WHAT IT IS.  The banked room script's state-0x12 installer, reached from
   func_b24_801723F8 with the bank-local D_800E2418 facing table and
   func_b24_80171A80 as the +0x8C continuation.  Past the record's +0x71 bump
   it either takes the D_800E3D7C "party member 2" hand-off (the engine 0x2000
   flag plus a +0x46 room id of 5 or more) or runs the ordinary
   D_80083462 & 8 / func_800A2B5C / func_800C77D0 / func_800A2B5C gate chain,
   then publishes state 0x12, dresses the slot and bumps D_80083460 + 0xA.

   FIVE SPELLINGS WERE MEASURED AND ARE IN THE BODY BELOW; all five are
   needed to reach 172 / 26 and none of them is in doubt:

   | spelling                                              | words | aligned |
   | as written below                                       |  172  |   26    |
   | func_800A4ACC arm as an EARLY RETURN, not an `else'     |  169  |   55    |
   | `(j - 1) * 3 + 8' inlined, no `n' local                 |  171  |   33    |
   | no `(s16)' on the `>= 5' gate                           |  172  |   27    |
   | the +0x4000 gate's +0x46 read through `w', not `p'       |  171  |   33    |
   | `s32 j' local for the `>= 5' gate                       |  170  |   44    |
   | `n' as u16 / s8                                         |  173  | 31 / 33 |
   | `n' as s16 / u8                                         |  172  | 26 / 27 |

   The early-return row is the brief's then/else rule: the target branches
   FORWARD to the func_800A4ACC block, so it is the `else' arm.  The `n' local
   is the fold rule: `(j - 1) * 3 + 8' is distributed to `j * 3 + 5' and the
   target materialises `j - 1' (`addiu $a0, $v0, -0x1') instead.  The `(s16)'
   cast is worth exactly the one word at 8017182C, `slti' against `sltiu':
   fold proves `x & 0x3FFF' non-negative and turns the signed compare
   unsigned, and the cast is free in words because the mask kills the
   extension.

   WHAT IS LEFT, 26 rows in THREE independent causes.

   (1) 9 ROWS: `b' AND `c' EXCHANGE $s3 AND $s4.  Every instruction is in
   place; only the two prologue saves, their two copies and the four use
   sites differ.  `cdkgcc -da' settles the mechanism exactly and the margin
   is one part in fifty:

       pseudo 81 (b)   2 refs / 46 insns   ->  1*2/46  * 10000 = 434
       pseudo 82 (c)   5 refs / 226 insns  ->  2*5/226 * 10000 = 442
       pseudo 80 (p)   7 refs / 226 insns  ->  2*7/226 * 10000 = 620
       pseudo 83 (t)   4 refs / 236 insns  ->  2*4/236 * 10000 = 338
       pseudo 86 (w)  22 refs / 116 insns  -> 4*22/116 * 10000 = 7586

   and `;; 10 regs to allocate: ... 80 82 81 83' confirms c is sorted ahead
   of b.  The target needs b ahead of c, which `allocno_compare' gives for
   EITHER `b' live 45 insns instead of 46 OR `c' live 230 instead of 226 (a
   tie suffices -- `allocno_compare' falls through to `v1 - v2' and 81 < 82).
   PROOF THAT THIS IS THE WHOLE CAUSE: moving `*(u8 **)(c + 0x2C) = t' to
   AFTER the func_80047784 call lengthens c by those four insns and the nine
   rows VANISH -- 172 words / 18 aligned differences, the b/c assignment
   exactly right.  That spelling is REJECTED because the listing puts the
   store before the call (it sits in the `lh %lo(D_80083228)' load-delay slot
   at 80171A10 and sched cannot move a store across a `jal'), so it buys nine
   rows and sells two.  The real spelling must lengthen c -- or shorten b by
   ONE insn -- some other way.  Measured and NOT it: `do { } while (0)` round
   the func_800C77D0 call, which gives b 3 refs (652) and overshoots PAST p,
   so b takes $s2 and p takes $s3 -- 28 rows; `w = p' before `k = 0'; `k = 0'
   after the +0x71 bump; `t != 0' for `t != NULL'; declaring `k' first; `s16'
   or `u8' k (both 174 words).

   (2) 12 ROWS: THE D_8006DE24 INDEX BLOCK IS A $v0/$v1/$a0 ROTATION, and one
   row of it is a SECOND LOAD the target has and no spelling here reproduces.
   The target reads `*(u16 *)(w + 0x46)' TWICE in one cse block --
   `lhu $v0, 0x46($s0)' at 80171980 for the +0x4000 gate and again at
   80171994 for the +0x3FFF index -- with no store, no call and no CODE_LABEL
   between them, so cse should have unified them and in this body it does.
   Reading the FIRST one through `p' instead buys the word back (172 instead
   of 171) at the cost of one wrong base register, which is what the body
   below does.  What was measured and does NOT produce two loads: either read
   as `*(s16 *)(w + 0x46) & ...' (171), either read as an array ref
   `((u16 *)w)[0x23]' (171), the gate written `!= 0' with an empty then-arm
   and the work in the else (171).  The rest of the 12 rows is that the
   target keeps the lhu/andi chain in $v0 and moves `n' to $a0, where
   local-alloc here ties `n' to the dying andi result and chains in $v1 --
   `combine_regs', local-alloc.c:1759.  A separate `j' local for the masked
   value, with or without `tbl' moved, does not untie it (18 either way on
   top of the c-lengthening row).

   (3) 4 ROWS: `*(u32 *)(w + 0x14) |= 0x80000000' puts the loaded word in
   $v1 and the `lui' constant in $v0 where the target has them the other way
   round; the operand order of the `or' is already right in both.  Measured
   and free: `0x80000000 | *(u32 *)(w + 0x14)' is the same 4 rows.  Reading
   or writing +0x14 through `p' is 174 words / 24 and is wrong.

   The three causes are independent: (1) was isolated by the c-lengthening
   test, and (2) and (3) are unchanged across every variant of (1).

DECLARATIONS
s32 func_800C77D0(void *, u8 *, s16, s32);
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
extern Unk8006DE24 D_8006DE24[];
s32 func_800A56E0(s32);
END DECLARATIONS

   The unit already declares func_800A4ACC, func_800A2B5C, func_800A6D30,
   func_80047784, D_80083228, D_800E3D7C and this function's own
   5-parameter prototype; D_80083460 and D_800814A0 come from structs.h. */
void func_b24_801717D0(u8 *p, UnkVec *b, u8 *c, u8 *t, void (*h)())
{
    Unk8006DE24 *tbl;
    u8 *w;
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *m;
    u8 *e;
    s32 i;
    s32 n;
    s32 k;

    k = 0;
    w = p;
    w[0x71] &= 0x7F;
    if ((*(u32 *)(w + 0x1C) & 0x2000) != 0 && (s16)(*(u16 *)(w + 0x46) & 0x3FFF) >= 5)
    {
        k = 1;
        if (*(u16 *)(D_800E3D7C + 0xA6) == 2)
        {
            g = (u8 *)&D_80083460;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
        }
        m = D_800E3D7C;
        if ((*(u32 *)(m + 0x98) & 0x3000) == 0x2000 && *(u16 *)(m + 0xA6) != k)
        {
            *(s16 *)(m + 0xA6) = k;
            *(u16 *)(m + 0x98) |= 0x1000;
            g2 = (u8 *)&D_80083460;
            *(s16 *)(g2 + 0xA) = *(u16 *)(g2 + 0xA) + 1;
        }
        else
        {
            func_800A4ACC(w);
            w[0x6D] -= 1;
            *(u32 *)(p + 0x8C) = (u32)h;
            return;
        }
    }
    else
    {
        if ((D_80083462 & 8) != 0)
        {
            return;
        }
        if ((s16)func_800A2B5C(w) != 0)
        {
            return;
        }
        func_800C77D0(w - 0x20, (u8 *)b, 8, 0x300);
        if ((s16)func_800A2B5C(w) != 0)
        {
            return;
        }
    }
    if (k == 0)
    {
        if ((*(u32 *)(w + 0x1C) & 0x400) != 0 && *(s32 *)(w + 0x14) >= 0)
        {
            *(u32 *)(w + 0x14) |= 0x80000000;
            *(s16 *)(w + 0x2A) = *(u16 *)(w + 0x2A) + ((func_800A6D30() & 7) << 9);
        }
        if ((*(u16 *)(w + 0x46) & 0x4000) == 0)
        {
            tbl = D_8006DE24;
            n = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
            i = w[n * 3 + 8];
            e = (u8 *)&tbl[i];
            if (e[0x12] == 2)
            {
                *(s32 *)(w + 0x60) = func_800A05A4(w, c[0x24], c[0x25], *(s16 *)(w + 0x2A), e[0x13]);
            }
        }
    }
    p[0x9A] = 0x12;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        *(u8 **)(c + 0x2C) = t;
        func_80047784(c, t[((D_80083228 + *(s16 *)(w + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    g3 = (u8 *)&D_80083460;
    *(s16 *)(g3 + 0xA) = *(u16 *)(g3 + 0xA) + 1;
}
