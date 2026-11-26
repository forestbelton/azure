/* func_s72_800251F4 -- ovl_dng_s72 -- PARKED at 277 words / 18 aligned rows.

   WHAT IT DOES.  The thrown object's impact burst: four times over, allocate a
   0x212 node driven by func_s72_80025088 and registered against
   func_s72_80025034, seed its payload's 0x78-tick fuse and a rand()&1 phase,
   dress the slot (0x20 at +0x10, 0xC raised in +0x14, 0x1000 scales), publish
   (x, y, z) jittered by rand()&0x3F into the new node's vector, cast the
   ground record under (x, y) with this unit's func_s72_80024064 and copy that
   0x18-byte polygon record into the node at +0x52, then pick a random quarter
   of the record's UV rectangle (a min/max over u0/u2 and v0/v1, a rand()&3
   quadrant index, a signed /4 step), write the four corner SVECTORs at
   +0x4A..+0x60 as rand()&3 plus a per-axis bias, paint the four colour words
   0x80808080, seed the +0x16/+0x18/+0x1A rotation from rand()&0xFFF and the
   +0x7C..+0x84 velocity from scaled rand()&0x7FFF draws, and hand the slot to
   func_8003DB94 against D_800DE870.

   NO TWIN.  dngcallee and dngtwin find nothing; written from the listing with
   this unit's own landed func_s72_8002626C (the n->unk_08-read-inline fact),
   func_s72_800244BC (the 0x212 spawn head) and func_s72_80025CE8 (the
   s16-loop-counter idiom) as the sources of spelling.

   THE WORD COUNT IS EXACT.  277 candidate words against 277 target words, and
   every instruction outside a 30-word window (rows 114..146, the two UV-axis
   arithmetic blocks) is bit-identical.  relcand: ok, 1 data symbol, 27 calls.
   declcheck: clean.

   | spelling                                              | words | rows |
   | as written below                                       |  277  |  18  |
   | the two min/max gates read NON-volatile                |  275  |  26  |
   | one shared dividend local `d = hi - lo; w = d / 4'     |  278  |  14  |
   | per-axis `dv'/`dv2' dividend locals                    |  277  |  18  |
   | no `jp' local, `(j + 1)' inline, `w' per axis           |  277  |  29  |
   | no `jp', `w' SHARED between the axes                    |  277  |  21  |
   | `jp' per axis instead of shared                        |  277  |  29  |
   | `w' shared and `jp' shared                             |  277  |  21  |
   | no `j' copy and no `b' copy (b2/j2 per axis)           |  273  |  35  |
   | `b' single-set with a second local for the accumulate   |  275  |  32  |
   | `hi'/`lo' updated in place instead of `b'               |  271  |  73  |
   | `b = lo' before `j = k'                                |  277  |  18  |
   | `w' computed after `j'/`b'                             |  277  |  89  |
   | `b' then `w' then `j'                                  |  278  |  44  |
   | `j'/`b'/`w' last                                       |  277  |  97  |
   | lo2/hi2 declaration order swapped                      |  277  |  25  |
   | k/k2 as `s16'                                          |  277  |  18  |
   | `jp' as `s16'                                          |  277  |  29  |
   | `b' as `s16'                                           |  273  |  64  |
   | the first gate as `q[0x44] < q[0x3A]'                  |  277  |  22  |
   | `x - 0x20 + (rand() & 0x3F)' without the `(s16)' cast  |  277  |  28  |
   | the min/max arms reached by explicit `goto'            |  275  |  26  |
   | a `uv = q + 0x3A' pointer local for the gates          |  278  |  25  |

   FOUR MEASURED FACTS THAT HOLD.

   (1) THE `(s16)' CAST ON `x - 0x20' IS LOAD-BEARING, 10 ROWS.  The target
   emits `addiu $v1, $fp, -0x20' BEFORE the `andi' that masks the rand draw,
   i.e. the tree is PLUS(PLUS(x, -32), r).  Written `x - 0x20 + (rand() &
   0x3F)' fold's associate rule (fold-const.c, the PLUS arm) rewrites it as
   PLUS(PLUS(x, r), -32) and the constant folds into the LAST addiu instead of
   the first.  `(s16)(x - 0x20) + (rand() & 0x3F)' blocks the association and
   the narrowing costs nothing because the result is stored as a halfword.

   (2) BOTH MIN/MAX GATES MUST BE WRITTEN `q[hi_off] > q[lo_off]', NOT
   `q[lo_off] < q[hi_off]' -- 4 rows.  Same `sltu', but the operands are
   evaluated in SOURCE order, so the `>' form loads +0x3A first, which is what
   the target does.

   (3) `jp = j + 1' MUST BE A SEPARATE SHARED LOCAL AND `w' MUST BE PER AXIS.
   This is a CONJUNCTION and a one-at-a-time sweep cannot find it: w-shared /
   no-jp is 21, w-per-axis / no-jp is 29, w-per-axis / jp-per-axis is 29, and
   only w-per-axis / jp-SHARED reaches 18.  The mechanism is sched.c's
   `adjust_priority': a single-set pseudo whose dest is still live gets
   `birthing_insn_p' and its priority raised to max_priority, and because
   sched1 builds the block backwards that means it is EMITTED LAST.  With `w'
   per axis the `sra' is boosted and lands after the `move'/`addiu' pair the
   way the target has it; with `jp' shared its `addiu' is NOT boosted and so
   does not overtake the `sra'.

   (4) THE `j = k' AND `b = lo' COPIES ARE REQUIRED AND THEY COST TWO WORDS
   EACH.  `j' and `b' must be SHARED across the two axes, i.e. multi-set, so
   local-alloc.c's `combine_regs' refuses to tie them (a pseudo that "dies more
   than once" has reg_qty -1) and the `addu $a0, $s1, $zero' / `addu $v1, $s3,
   $zero' copies the target has survive.  Written per axis they are single-set,
   cse propagates through them and the body comes out FOUR WORDS SHORT at 273.

   WHAT IS LEFT, AND THE DIAGNOSIS.  All 18 rows are inside the two UV
   arithmetic blocks and they are ONE cause with two presentations:

     (a) a single sched1 TRANSPOSITION.  The target emits the second axis's
         `subu $a1, $s4, $s5' BEFORE the first axis's `addu $v1, $s3, $zero'
         (`b = lo'); this body emits them the other way round.  Mechanism,
         read off sched.c: the subtraction's destination is a single-set pseudo
         that is still live, so `adjust_priority' boosts it to max_priority and
         the backward scheduler places it first, i.e. emits it LAST.  `b' is
         multi-set (fact 4) so its copy is never boosted.  THE ONLY SPELLING
         THAT UN-BOOSTS THE SUBTRACTION IS A SHARED DIVIDEND LOCAL, and that
         costs exactly one word: `expand_divmod' (expmed.c:3021) does
         `t1 = copy_to_mode_reg (compute_mode, op0)' unconditionally for a
         signed power-of-two divide, and with a multi-set `op0' the copy is in
         a different basic block from one of its sets, so combine_regs cannot
         tie it and the `move' survives.  That branch measures 278 / 14 -- the
         four SCRATCH registers then match the target exactly and the body is
         one word long.  Read it as evidence that the mechanism is right and
         the spelling is not.

     (b) a four-register SCRATCH permutation that follows from (a).  This body
         puts (hi-lo) in $v1, jp in $a0, j in $a1, (hi2-lo2) in $a2 and k2 in
         $a3 -- six scratch quantities.  The target puts jp in $v1, j in $a0,
         k2 in $a2 and BOTH subtraction temps in $a1 -- five, because its two
         dividend ranges are disjoint and get the same register.  Every
         callee-saved register already matches (q=$s0, r/k=$s1, n=$s2, lo=$s3,
         hi2=$s4, lo2=$s5, hi=$s6, 0x80=$s7, x=$fp) and so does the frame.

     DECLARATION ORDER IS EXHAUSTED AS AN AXIS: five permutations (w/b/j, jp
     first, w2 adjacent, k2 before k, lo2/hi2) all measure identically at 18,
     which by docs/matching-levers.md's rule puts allocation priority out and
     the cause in the arithmetic or the statement order.

   THE `volatile' GATE IS A DEVICE AND IT IS WORTH TWO WORDS.  The target
   reloads BOTH bytes inside the fall-through arm of each min/max gate even
   though the gate's own loads are still available; this body cannot reproduce
   that without stopping cse from recording them.  cse1's basic block ends only
   at a CODE_LABEL (cse.c, `cse_end_of_basic_block') or a NOTE_INSN_LOOP_END,
   and the fall-through arm has neither, so with plain reads cse rewrites the
   arm's `lbu' as a `move' and reorg then fills the branch's delay slot with
   it: 275 words / 26 rows.  Ruled out as legal spellings, all measured:
   `*(u8 *)(q + K)' against `q[K]' (identical trees -- build_array_ref on a
   pointer is build_indirect_ref of a PLUS, so MEM_IN_STRUCT_P is not set
   either way and cse's exp_equiv_p compares addresses only); the arms reached
   by an explicit `goto' with a `<=' gate (275 / 26, cse still unifies); and a
   `uv = q + 0x3A' pointer local for the gate, which DOES defeat cse1 -- both
   arms reload -- but combine then folds `uv' through `q = (u8 *)n + 0x20' to
   base $s2 where the target has base $s0, and the surviving `addiu' makes it
   278 words / 25 rows.  If a legal spelling exists it has to keep base $s0.

DECLARATIONS
s32 rand(void);
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
extern u8 D_800DE870[];
typedef struct UnkDngS72Poly
{
    s16 m[12];
} UnkDngS72Poly;
END DECLARATIONS
*/
void func_s72_800251F4(u8 *p, s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    u8 *e;
    s16 i;
    s32 lo;
    s32 hi;
    s32 hi2;
    s32 lo2;
    s32 k;
    s32 k2;
    s32 w;
    s32 b;
    s32 j;
    s32 w2;
    s32 jp;

    for (i = 0; i < 4; i++)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x28) = 0x78;
            *(s16 *)(q + 0x2C) = rand() & 0x1;
            n->unk_10 = (u32)func_s72_80025088;
            func_8004491C(n, (u32)func_s72_80025034);
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            *(s16 *)((u8 *)n->unk_08 + 0x2) = (s16)(x - 0x20) + (rand() & 0x3F);
            *(s16 *)((u8 *)n->unk_08 + 0x6) = (s16)(y - 0x20) + (rand() & 0x3F);
            *(s16 *)((u8 *)n->unk_08 + 0xA) = z;
            e = func_s72_80024064((u16)x, (u16)y, z);
            *(UnkDngS72Poly *)((u8 *)n + 0x52) = *(UnkDngS72Poly *)e;
            /* DEVICE: the two `volatile' reads keep cse1 from recording the
               gate's bytes, so both arms reload them the way the target does.
               Worth 2 words -- without it the body is 275 / 26.  The legal
               spellings ruled out are listed in the header. */
            if (*(volatile u8 *)(q + 0x3A) > *(volatile u8 *)(q + 0x44))
            {
                lo = q[0x44];
                hi = q[0x3A] + 1;
            }
            else
            {
                lo = q[0x3A];
                hi = q[0x44] + 1;
            }
            if (*(volatile u8 *)(q + 0x3B) > *(volatile u8 *)(q + 0x3F))
            {
                lo2 = q[0x3F];
                hi2 = q[0x3B] + 1;
            }
            else
            {
                lo2 = q[0x3B];
                hi2 = q[0x3F] + 1;
            }
            k = rand() & 0x3;
            k2 = rand() & 0x3;
            w = (hi - lo) / 4;
            j = k;
            b = lo;
            jp = j + 1;
            q[0x3E] = b + jp * w - 1;
            q[0x3A] = b + jp * w - 1;
            b = b + j * w;
            q[0x46] = b;
            q[0x44] = b;
            w2 = (hi2 - lo2) / 4;
            j = k2;
            b = lo2;
            jp = j + 1;
            q[0x47] = b + jp * w2 - 1;
            q[0x3F] = b + jp * w2 - 1;
            b = b + j * w2;
            q[0x45] = b;
            q[0x3B] = b;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(s16 *)(q + 0x4A) = (rand() & 0x3) - 5;
            *(s16 *)(q + 0x4C) = (rand() & 0x3) - 2;
            *(s16 *)(q + 0x4E) = (rand() & 0x3) - 5;
            *(s16 *)(q + 0x50) = (rand() & 0x3) + 1;
            *(s16 *)(q + 0x52) = (rand() & 0x3) - 2;
            *(s16 *)(q + 0x54) = (rand() & 0x3) - 5;
            *(s16 *)(q + 0x56) = (rand() & 0x3) - 5;
            *(s16 *)(q + 0x58) = (rand() & 0x3) - 2;
            *(s16 *)(q + 0x5A) = (rand() & 0x3) + 1;
            *(s16 *)(q + 0x5C) = (rand() & 0x3) + 1;
            *(s16 *)(q + 0x5E) = (rand() & 0x3) - 2;
            *(s16 *)(q + 0x60) = (rand() & 0x3) + 1;
            q[0xE] = 0x80;
            q[0xD] = 0x80;
            q[0xC] = 0x80;
            q[0x12] = 0x80;
            q[0x11] = 0x80;
            q[0x10] = 0x80;
            q[0x16] = 0x80;
            q[0x15] = 0x80;
            q[0x14] = 0x80;
            q[0x1A] = 0x80;
            q[0x19] = 0x80;
            q[0x18] = 0x80;
            *(s16 *)(r + 0x16) = rand() & 0xFFF;
            *(s16 *)(r + 0x18) = rand() & 0xFFF;
            *(s16 *)(r + 0x1A) = rand() & 0xFFF;
            *(s32 *)(q + 0x7C) = ((rand() & 0x7FFF) - 0x4000) * 32;
            *(s32 *)(q + 0x80) = ((rand() & 0x7FFF) - 0x4000) * 32;
            *(s32 *)(q + 0x84) = ((rand() & 0x7FFF) - 0x6000) * 64 - 0x20000;
            *(s32 *)(q + 0x90) = 0x10000;
            r[0xE] = 0x80;
            r[0xD] = 0x80;
            r[0xC] = 0x80;
            func_8003DB94(r, D_800DE870, 0);
        }
    }
}
