/* PARK: func_s03_800245B4, csrc/ovl_dng_s03.c, agent-h249, 2026-11-26.

   TARGET 282 words. BEST BUILT 282 words, 113 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), of which 103 are register-only (`R') rows,
   2 are prologue save-slot rows and 8 are one-slot position shifts that
   follow from the register choice. 1 member, 282 words (dngfam) -- no
   replication value. No jump table.

   VERDICT: ALLOC, AND IT IS ONE SWAP OF TWO CALLEE-SAVED REGISTERS.
   Every opcode, immediate and displacement matches. The parameter `p' is in
   $s1 here and in $s2 in the target; the early clamp value is in $s0 here
   and in $s1 in the target. Nothing about the logic is in doubt: the
   candidate and the target agree word for word on all 282 words modulo
   those register names.

   THE CAUSE IS MEASURED ARITHMETICALLY, NOT GUESSED. `global.c' ranks
   allocnos on `floor_log2(n_refs) * n_refs / live_length' and assigns the
   lowest-numbered free call-preserved register in that order. From a
   single-function `-da' probe of THIS body (`probe.c.lreg', the
   `;; N regs to allocate:' line of `probe.c.greg'):

     pseudo 80 = `p'       84 refs / 454 insns / 10 calls   6*84/454 = 1.110
     pseudo 83 = clamp `n' 12 refs /  25 insns /  3 calls   3*12/25  = 1.440
     pseudo 84 = `y'        6 refs /  13 insns /  1 call    2* 6/13  = 0.923
     pseudo 81 = rand-loop  7 refs /  29 insns /  2 calls   2* 7/29  = 0.483
     pseudo 82 = 2nd loop   7 refs /  13 insns /  0 calls   2* 7/13  = 1.077

   The printed order is `342 83 86 80 82 344 84 112 81 118 236': the clamp
   takes $s0, then `p' takes $s1, and the target's assignment needs ONE MORE
   call-crossing allocno ranked above `p''s 1.110 so that `p' falls to $s2.
   The gap is 1.110 against 0.923 -- `y' is the allocno the target has in
   $s1 -- and because `floor_log2' steps 2 -> 3 at eight references, `y'
   flips the order at **8 refs** (3*8/13 = 1.846) but NOT at 7
   (2*7/13 = 1.077 < 1.110). So the remedy is two more weighted references
   on `y', or `y''s live length cut from 13 to under 11. I could not find a
   spelling that does either without changing the emitted words: `y' is
   referenced exactly three times in both streams (`sra 1', `sra 3',
   `sra 4'), which is what the target emits too.

   WHAT WAS PRICED, EACH ALONE AGAINST THE OTHERWISE-MATCHING BODY:

   | spelling                                               | words | rows |
   |--------------------------------------------------------|-------|------|
   | the body as parked                                KEPT |  282  | 113  |
   | `rand()' called BEFORE the `h' shift chain        KEPT |  282  | 113  |
   |   ... `h' computed before the rand() call               |  282  | 120  |
   | `j' its own local for `(rand() & 7) + 8'          KEPT |  282  | 113  |
   |   ... written inline as `h + ((rand() & 7) + 8)'       |  281  | 119  |
   | `w' its own local for the +0x1700 value           KEPT |  282  | 113  |
   |   ... `p+0x18 += 0x1700;' then `p+0xC += p+0x18;'      |  282  | 120  |
   | two loop counters `i' and `k'                           |  282  | 113  |
   | one loop counter `i' for both loops               KEPT |  282  | 113  |

   THE 281-WORD POINT IS THE INTERESTING ONE AND IT IS A FOLD FACT.
   Written inline, `h + ((rand() & 7) + 8)' is reassociated by
   `fold-const.c' into `(h + 8) + (rand() & 7)' at BOTH sites, and cse1 then
   shares the single `addiu $s0, $s0, 8' between them -- one word short of
   the target, which materialises `(rand() & 7) + 8' separately each time
   (`andi 7' / `addiu 8' / `addu'). Hoisting the jitter into its own local
   `j' blocks the reassociation and restores the word. This is the
   `A + (B + C)' grouping lever with a CONSTANT addend, where parentheses
   alone are NOT enough because fold moves constants outward; the local is.

   STRUCTURE, ALL OF IT CONFIRMED BY THE MATCHING WORDS:
   The per-frame step of a four-corner billboard actor. `p + 0x52' is a
   countdown whose PRE-decrement value is tested (`(s16)((*(u16 *)(p+0x52))--)
   < 0'), and on expiry a two-case `switch' on `p + 0x56' advances the state,
   with case 1 falling through into case 2 when `p + 0x54' is negative --
   written as a `break' inside case 1 and no `default'. The four corners live
   at `p + 0x28 + i*8' as (dx, dy, jitter) triples; state <2 lays out a
   diamond of half-height `h = (y>>1) + ((y>>3) * (y>>4))' where
   `y = -*(s16 *)(p + 0xE)', re-jitters each corner's third halfword with
   `rand() & 0xF', and hands func_800DBA90 a `u8 rec[0x20]' rotation record
   (`rec+0' and `rec+4' both the corner array, rotation (0,0,p+0x4C),
   translation from func_80064584/func_800644B8 >> 8, extent 4). State >=2
   decays the +0x48..+0x4A colour by a quarter and drifts the corners.

   DONOR: func_s19_800250E8 (csrc/ovl_dng_s19.c), found by `dngcallee.py' at
   5/6 symbols -- `dngtwin --unit s03' reports NOTHING for any stub in this
   unit. The donor is 126 words against this 282 and shares only the
   func_800BCB04 call and the D_800814A0 teardown, but two of its recorded
   facts carried and are load-bearing here: `s32 func_800BCB04(s32, s32,
   s16)' with an `(s16)' cast on the result (the cast is the `sll 16'/`sra
   16' pair at words 155-156), and the grouped-addend rule above.
   func_800DBA90's record type `u8 rec[0x20]' came from csrc/ovl_dng_b04.c.

   RULED OUT ARITHMETICALLY: priority is not a tie anywhere in this body, so
   `qty_compare_1''s quantity-number tie-break (and therefore local
   declaration order) cannot reach it -- and declaration order measured 113
   either way, as did the counter's type.

   NEXT STEP FOR WHOEVER PICKS THIS UP: do not sweep spellings. The question
   is a single number -- get `y' to eight weighted references, or shorten its
   live range by three insns, without adding a word. A permuter run over this
   body is likely to find it, since the body is already byte-exact apart from
   two register names. */

#include "common.h"

s32 rand(void);
s32 func_80064584(s32);
s32 func_800644B8(s32);
s32 func_800BCB04(s32, s32, s16);
void func_800DBA90(u8 *);

void func_s03_800245B4(u8 *p)
{
    u8 rec[0x20];
    s32 i;
    s32 n;
    s32 h;
    s32 y;
    s32 j;
    s32 w;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    if ((s16)((*(u16 *)(p + 0x52))--) < 0)
    {
        *(s16 *)(p + 0x56) = *(u16 *)(p + 0x56) + 1;
        switch (*(s16 *)(p + 0x56))
        {
        case 1:
            *(s16 *)(p + 0x52) = 5;
            if (*(s16 *)(p + 0x54) >= 0)
            {
                break;
            }
            *(s16 *)(p + 0x56) = *(u16 *)(p + 0x56) + 1;
            /* fallthrough */
        case 2:
            *(s16 *)(p + 0x52) = 0xC;
            *(s32 *)(p + 0x10) = *(s32 *)(p + 0x10) * 2;
            *(s32 *)(p + 0x14) = *(s32 *)(p + 0x14) * 2;
            break;
        }
    }
    if (*(s16 *)(p + 0x56) == 1 && *(s16 *)(p + 0x52) < 5)
    {
        *(s16 *)(p + 0x54) = 1;
        p[0x48] += (0xFF - p[0x48]) >> 2;
        p[0x49] += (0x80 - p[0x49]) >> 2;
    }
    else if (*(s16 *)(p + 0x56) < 2)
    {
        n = (-*(s16 *)(p + 0xE) >> 2) + 0xF;
        if (n < 0x10)
        {
            n = 0x10;
        }
        p[0x48] = ((rand() & 0xF) + 0xBE) * 16 / n;
        p[0x49] = ((rand() & 0xF) + 0x91) * 16 / n;
        p[0x4A] = ((rand() & 0xF) + 0xA) * 16 / n;
    }
    *(s16 *)(p + 0x4C) = (*(u16 *)(p + 0x4C) + ((rand() & 0xFF) - *(u16 *)(p + 0xE) * 4)) & 0xFFF;
    if (*(s32 *)(p + 0xC) < -0x300000 || *(s16 *)(p + 0x56) == 3)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if (*(s16 *)(p + 0x56) == 0)
    {
        *(s32 *)(p + 0x4) += *(s32 *)(p + 0x10);
        *(s32 *)(p + 0x8) += *(s32 *)(p + 0x14);
    }
    n = (s16)func_800BCB04(*(u16 *)(p + 0x6), *(u16 *)(p + 0xA), *(u16 *)(p + 0x50) - 0x30);
    if (n < 0x200)
    {
        *(s16 *)(p + 0x50) = n;
    }
    if (*(s16 *)(p + 0x56) < 2)
    {
        w = *(s32 *)(p + 0x18) + 0x1700;
        *(s32 *)(p + 0xC) += w;
        *(s32 *)(p + 0x18) = w;
        y = -*(s16 *)(p + 0xE);
        j = (rand() & 7) + 8;
        h = (y >> 1) + ((y >> 3) * (y >> 4));
        *(s16 *)(p + 0x32) = h + j;
        *(s16 *)(p + 0x28) = *(s16 *)(p + 0x32);
        j = (rand() & 7) + 8;
        h = -(h + j);
        *(s16 *)(p + 0x42) = h;
        *(s16 *)(p + 0x38) = h;
        *(s16 *)(p + 0x40) = 0;
        *(s16 *)(p + 0x3A) = 0;
        *(s16 *)(p + 0x30) = 0;
        *(s16 *)(p + 0x2A) = 0;
        *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 0x100;
        for (i = 3; i >= 0; i--)
        {
            *(s16 *)(p + 0x2C + i * 8) = rand() & 0xF;
        }
        *(u8 **)rec = p + 0x28;
        *(u8 **)(rec + 0x4) = p + 0x28;
        *(s16 *)(rec + 0x8) = 0;
        *(s16 *)(rec + 0xA) = 0;
        *(s16 *)(rec + 0xC) = *(u16 *)(p + 0x4C);
        *(s16 *)(rec + 0x10) =
            *(u16 *)(p + 0x6) +
            (func_80064584(*(s16 *)(p + 0x4E) + *(s16 *)(p + 0xE) * 16) >> 8);
        *(s16 *)(rec + 0x12) =
            *(u16 *)(p + 0xA) +
            (func_800644B8(*(s16 *)(p + 0x4E) + *(s16 *)(p + 0xE) * 16) >> 8);
        *(s16 *)(rec + 0x14) = *(u16 *)(p + 0x50) + *(u16 *)(p + 0xE) * 2;
        *(s16 *)(rec + 0x18) = 4;
        *(s16 *)(rec + 0x1A) = 0;
        func_800DBA90(rec);
    }
    else
    {
        p[0x48] -= p[0x48] >> 2;
        p[0x49] -= p[0x49] >> 2;
        p[0x4A] -= p[0x4A] >> 2;
        if (*(s16 *)(p + 0x52) < 0xA)
        {
            for (i = 3; i >= 0; i--)
            {
                *(s16 *)(p + 0x28 + i * 8) = *(u16 *)(p + 0x28 + i * 8) + *(u16 *)(p + 0x12);
                *(s16 *)(p + 0x2A + i * 8) = *(u16 *)(p + 0x2A + i * 8) + *(u16 *)(p + 0x16);
            }
        }
    }
}
