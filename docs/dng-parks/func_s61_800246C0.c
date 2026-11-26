/* func_s61_800246C0 -- ovl_dng_s61, 318 words, 1 member (dngfam.py).

   VERDICT: ALLOC-ish. 318 words candidate against 318 target, 5 ALIGNED
   DIFFERENCES, all five inside ONE eleven-word block -- case 1's
   `*(s16 *)(p + 0x48) >= 0x1C' arm. Every other word of the body matches,
   including both func_8003DE58 projection blocks, the two abs/modulo
   scatter sites, the ten-node spawn loop and its whole renderable dress.

   WHAT IT IS. The step of the 0x312 node func_s61_8002401C's state 1 spawns:
   bump p + 0x48, raise the actor's +0x52 flag, hand the renderable to
   func_800478B8, snapshot b's three position halves into a local `base[3]',
   then in state 0 pull b toward func_8003DE58's projection at << 14 with a
   func_800644B8(x / 10) z sag and, past frame 0xA, raise the renderable's
   +0x14 0x800 and advance the state; in state 1 pull at << 15, strip that
   0x800 again past frame 0x12, and past frame 0x1C retire the actor. The
   common tail recolours p + 0x40/+0x42 off p + 0x48 and spawns 4, 0xC or 3
   more 0x312 nodes with func_s61_80024BB8 as their step, each scattered
   between `base' and b by `rand() % w' where `w' is the abs of the gap.

   DONOR: the landed func_s70_8002466C in csrc/ovl_dng_s70.c, found by
   dngcallee (11 of 12 symbols, clean gap to the 10/12 tier); dngtwin found NO
   carved sibling at ratio 0.70. Everything structural carried: the head's
   `*(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1' / `*(u16 *)(*(u8 **)p + 0x52)
   |= 0x8000' / func_800478B8 order, both `r = D_80082E80; if
   (func_8003DE58(*(u8 **)(r + 0x8), (UnkSlot *)r, out, 0) != 0)' blocks with
   their `t = D_80083780' locals, the `func_800644B8((... << 11) / 10) << 9'
   sag, the `((*(u16 *)(p + 0x48) & 3) << 5) + 0x80' /
   `(((s16) * (u16 *)(p + 0x48) >> 2) << 5) + 0x80' recolour pair, and
   `for (i = cnt; i >= 0; i--)'.

   THE RESIDUE, exactly. Target:

       164  lui   $a0, %hi(D_800814A0)      [bnez delay slot]
       165  lhu   $v0, -0x2($s5)
       166  nop
       167  ori   $v0, $v0, 0x8000
       168  sh    $v0, -0x2($s5)
       169  lhu   $v0, 0x4C($s5)
       170  lw    $v1, %lo(D_800814A0)($a0)
       171  addiu $v0, $v0, 0x1
       172  ori   $v1, $v1, 0x8000
       173  sh    $v0, 0x4C($s5)
       174  sw    $v1, %lo(D_800814A0)($a0)

   Candidate (statement order D_800814A0, p + 0x4C, p - 0x2): the SAME eleven
   words with the D_800814A0 chain and the p - 0x2 chain EXCHANGED -- the
   D chain emits at 165/168 and the p - 0x2 chain at 170/174, the 0x4C chain
   interleaves identically, and the `lui' fills the same delay slot. The five
   rows are one `R' on the `lui's register and four `X' on that exchange.
   Nothing is moved, nothing is missing, and the word count is exact.

   MEASUREMENT TABLE -- the six orders of those three statements, every one
   measured with `aligndiff' (counts include register-only rows):

     statement order in the arm                     words / aligned rows
     -----------------------------------------------------------------
     D_800814A0 ; p+0x4C ; p-0x2        KEPT         318 / 5
     p+0x4C ; p-0x2 ; D_800814A0                     319 / 13
     D_800814A0 ; p-0x2 ; p+0x4C                     318 / 15
     p-0x2 ; p+0x4C ; D_800814A0                     319 / 17
     p+0x4C ; D_800814A0 ; p-0x2                     320 / 17
     p-0x2 ; D_800814A0 ; p+0x4C                     316 / 19  (see below)

   The last row is not a near miss: with the p + 0x4C bump LAST in this arm it
   becomes textually identical to the tail of case 0's `>= 0xA' arm, jump2
   cross-joins the two, and case 0 loses four words. So the ROM's case 1 arm
   does NOT end with the 0x4C bump, which rules that order and
   `D_800814A0 ; p-0x2 ; p+0x4C'-with-cross-join out on structure rather than
   on score.

   OTHER SPELLINGS MEASURED AND REJECTED, all against the 318/5 base:

     `cnt' as a SEPARATE local from `w' (the abs width)   319 / 17
        -- this one is a REAL fact and it is kept: sharing ONE local between
        the count selection and the two abs widths is worth 12 rows and one
        word, because the shared pseudo spans the spawn loop and is forced
        callee-saved into $s0 in both roles. Re-priced at two settings of the
        statement-order axis (9 rows and 12 rows), same sign both times.
     two `if's instead of `__builtin_abs' at both scatter sites   320 / 48
        -- also a real kept fact, worth 16 rows: the ROM has the
        `bgez'/`addu'/`negu' expansion WITH the register-to-register copy,
        which `w = x - y; if (w < 0) w = -w;' coalesces away.
     `*(u8 **)e = *(u8 **)p' AFTER `*(s16 *)(e + 0x4C) = 0'       320 / 26
        -- kept the other way round, worth one word: with the pointer store
        first, sched lifts the `lw $v0, 0x0($s5)' into the 0x4C store's slot
        and the load-delay `nop' disappears.
     `volatile' on the p + 0x4C bump (both halves)       319 / 17  (no change)
     `volatile' on the p - 0x2 store                     319 / 17  (no change)
     `pp = p;' alias local for the p + 0x4C bump         319 / 17  (no change)
     `D_800814A0' read as `D_800814A0_a[0]' off a
        unit-local `extern u32 []' __asm__ alias         319 / 17  (no change)

   CAUSE -- HYPOTHESIS, NOT MEASURED. In the target the `lw' of D_800814A0 and
   the `lhu' of p + 0x4C both sit AFTER the `sh' to p - 0x2, and the p - 0x2
   load's own delay slot is left as a `nop' even though the 0x4C load is
   otherwise ready. The reading is that the target's scheduler could not hoist
   the 0x4C load above the p - 0x2 store, i.e. that `memrefs_conflict_p' did
   not disambiguate those two accesses -- which would happen if they reach
   `p' through DIFFERENT pseudos at sched time. Three attempts to manufacture
   that (an alias local, `volatile' on either side, an array-ref form of
   D_800814A0 to defeat `true_dependence's fixed-scalar escape) all left the
   figure at 319/17, so the hypothesis is UNCONFIRMED and may be wrong; the
   alternative is simply that MIPS's backward list scheduler ranks the two
   chains differently for a reason not visible in the source. Read the table
   above as fact and this paragraph as a guess.

   NEXT THING TO TRY: a permuter run over this arm only. The body is one
   eleven-word block away and every other word is exact, so the search space
   is tiny.

   DECLARATIONS
#include "common.h"

void func_800478B8(u8 *);
s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800644B8(s32);
void func_s61_80024BB8(u8 *p, UnkVec *b, u8 *c, u8 *d);

extern u32 D_800814A0;
extern u8 D_80045340[];
extern u8 D_80082E80[];
extern u8 D_80083498[];
extern u8 D_80083780[];
extern u8 D_800DEA68[];
   END DECLARATIONS
*/
void func_s61_800246C0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *r;
    u8 *t;
    u8 *r2;
    u8 *t2;
    s32 cnt;
    s32 i;
    s32 w;
    s32 k;
    s16 out[4];
    s16 base[3];

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    func_800478B8(c);
    base[0] = *(u16 *)((u8 *)b + 0x2);
    base[1] = *(u16 *)((u8 *)b + 0x6);
    base[2] = *(u16 *)((u8 *)b + 0xA);
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        if (*(s16 *)(p + 0x48) < 8)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x78;
        }
        r = D_80082E80;
        if (func_8003DE58(*(u8 **)(r + 0x8), (UnkSlot *)r, out, 0) != 0)
        {
            t = D_80083780;
            b->unk_00 += (out[0] + *(s16 *)(t + 0x2) - *(s16 *)((u8 *)b + 0x2)) << 14;
            b->unk_04 += (out[1] + *(s16 *)(t + 0x6) - *(s16 *)((u8 *)b + 0x6)) << 14;
            b->unk_08 += (out[2] + *(s16 *)(t + 0xA) - *(s16 *)((u8 *)b + 0xA)) << 14;
            b->unk_08 -= func_800644B8((*(s16 *)(p + 0x48) << 11) / 10) << 9;
        }
        if (*(s16 *)(p + 0x48) >= 0xA)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        r2 = D_80082E80;
        if (func_8003DE58(*(u8 **)(r2 + 0x8), (UnkSlot *)r2, out, 0) != 0)
        {
            t2 = D_80083780;
            b->unk_00 += (out[0] + *(s16 *)(t2 + 0x2) - *(s16 *)((u8 *)b + 0x2)) << 15;
            b->unk_04 += (out[1] + *(s16 *)(t2 + 0x6) - *(s16 *)((u8 *)b + 0x6)) << 15;
            b->unk_08 += (out[2] + *(s16 *)(t2 + 0xA) - *(s16 *)((u8 *)b + 0xA)) << 15;
        }
        if (*(s16 *)(p + 0x48) >= 0x12)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x50;
        }
        if (*(s16 *)(p + 0x48) >= 0x1C)
        {
            D_800814A0 |= 0x8000;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
            *(u16 *)(p - 0x2) |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x40) = ((*(u16 *)(p + 0x48) & 3) << 5) + 0x80;
    *(s16 *)(p + 0x42) = (((s16) * (u16 *)(p + 0x48) >> 2) << 5) + 0x80;
    if (*(s16 *)(p + 0x48) < 8)
    {
        w = 4;
    }
    else if (*(u16 *)(p + 0x48) >= 8 && *(u16 *)(p + 0x48) <= 0xC)
    {
        w = 0xC;
    }
    else
    {
        w = 3;
    }
    for (i = w; i >= 0; i--)
    {
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s61_80024BB8;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            e = (u8 *)n + 0x20;
            w = __builtin_abs(*(s16 *)((u8 *)b + 0x2) - base[0]);
            if (w == 0)
            {
                w = 1;
            }
            k = rand() % w;
            *(s16 *)((u8 *)n->unk_08 + 0x2) =
                ((*(s16 *)((u8 *)b + 0x2) + base[0]) >> 1) + k - (w >> 1);
            w = __builtin_abs(*(s16 *)((u8 *)b + 0x6) + base[1]);
            if (w == 0)
            {
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04;
            }
            else
            {
                *(s16 *)((u8 *)n->unk_08 + 0x6) =
                    *(u16 *)&base[1] + (*(s16 *)((u8 *)b + 0x6) - base[1]) * k / w;
            }
            *(s32 *)((u8 *)n->unk_08 + 0x0) += ((rand() & 0x3FF) - 0x1FF) << 10;
            *(s32 *)((u8 *)n->unk_08 + 0x4) += ((rand() & 0x3FF) - 0x1FF) << 10;
            *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x3FF) - 0x1FF) << 10);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1800;
            *(s16 *)(q + 0x10) = 0x60;
            *(u8 **)(q + 0x0) = D_800DEA68;
            *(u16 *)(q + 0x14) |= 0xC;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DEA68 + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0xC06060;
            *(u8 **)e = *(u8 **)p;
            *(s16 *)(e + 0x4C) = 0;
        }
    }
}
