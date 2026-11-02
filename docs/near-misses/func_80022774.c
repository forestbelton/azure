#include "common.h"

/* NEAR MISS: 102 words -- the right count -- and 3 DIFF. Every opcode,
   immediate and displacement matches, and every register matches. What is
   left is a three-word ROTATION of the last three preheader arithmetic words:

       built   addiu $t4,$v0,0xA | addiu $a0,$a0,0xA | sra $t9,$a0,1
       target  addiu $a0,$a0,0xA | sra $t9,$a0,1     | addiu $t4,$v0,0xA

   i.e. loop.c hoists the `h + 0xA` movable BEFORE the `(s32)(w+0xA)>>1`
   pair instead of after it. Section "WHY THIS LAST ROTATION IS CLOSED"
   below argues from local-alloc's priority formula that the two cannot be
   had at once in this body, and gives the numbers.

   THE CAUSE, CORRECTED 2026-11-01 (second rewrite). Earlier headers said
   the residue was "the price of the `hh` marker: one more insn in the
   preheader, and sched1 then takes the giv initialiser first". The
   OBSERVATION was right and the CONCLUSION was wrong -- that permutation is
   fixable, and `"r"(w + 8)` on the same marker fixes it. The chain, every
   step read off the dumps (`cpppsx` + `cc1psx -O2 -G0 -da`):

   1. local-alloc runs on the SCHED1 order, and sched2 reorders again
      afterwards. So the emitted order tells you nothing about the births
      and deaths local-alloc saw. Both residues this function has had are
      sched2 permutations driven by LUIDs that sched1 fixed.
   2. The old giv permutation was a sched2 tie: at sched2's T-20 the ready
      list is `24 (1) 252 (1) ...` -- the `lo_sum` and the giv initialiser,
      both priority 1, both class 3 -- and `rank_for_schedule` falls through
      to `INSN_LUID (tmp) - INSN_LUID (tmp2)` (sched.c:2425), which takes the
      HIGHER luid first and so emits it LATER. The target needs the giv's
      luid above the lo_sum's, i.e. sched1 must leave the giv init LATE in
      the chain.
   3. sched1 leaves it late only if there is a HOLE at some clock <= 19 for
      it to fall into. The holes are the three load delays. With the bare
      `__asm__("" ::"r"(hh))` there is no hole: the asm is `recog`-
      unrecognised, so `insn_cost` forces every link into it to cost 1
      (sched.c, "A USE insn should never require the value used to be
      computed" / `LINK_COST_FREE`), its priority is therefore 2, and at
      T-13 -- the one clock where no priority-3 or LAUNCH_PRIORITY insn is
      ready -- it beats the giv's priority 1 and eats the hole. Scheduling
      the asm there also delays the `unk_17` load by one clock, which closes
      the hole the giv used in the marker-free build.
   4. Adding a second `"r"` operand whose producer has priority 3 raises the
      asm's own priority to 3 (`priority`, sched.c:1489: max over LOG_LINKS
      of `priority(pred) + cost - 1`). The asm then schedules inside the
      priority-3 pack at T-11, the hole at T-15 is left for the giv, and the
      first four preheader words come out in the target's order. `w + 8` is
      the right operand because it is pseudo 113, set by a priority-3 insn,
      and the extra reference it buys lands on a quantity (113, 3 refs -> 4)
      whose register is decided long after the three that matter.
   5. With the preheader fixed, what is left is the ALLOCATION of the
      unk_17/unk_16/h chains, and that is decided by `qty_compare_1`
      (local-alloc.c:1598), `floor_log2(refs)*refs*size/(death-birth)`, over
      the sched1 order. Moving `bh = h + 0xA` one statement earlier in the
      body moves loop.c's hoist of it one position earlier in that order,
      which shortens `h`'s quantity, and that is what puts all three chains
      in the target's registers -- at the cost of this rotation.

   THE THREE QUANTITIES, priced on the sched1 order (positions are
   local-alloc's `insn_number` within block 0; `qty_size` is 1 for every
   single-word quantity and cancels, so only refs and length count):

     qty           members          refs  birth..death  pri    reg
     h chain       {112, 83}          5    46..55       1.111  $v0
     unk_17 (hh)   {85}               4    40..49       0.889  $v1
     unk_16/w      {100, 82, 125}     7    32..59       0.519  $a0

   `combine_regs` (local-alloc.c:1858) is what makes the w chain a 7-ref
   quantity: the `sll` consumes the `lbu`'s pseudo and the `addiu ...,0xA`
   consumes w's, and each tie ADDS the dying pseudo's references. It is also
   why the w chain cannot be weakened -- the target's `sll $a0,$a0,2` and
   `addiu $a0,$a0,0xA` both reuse w's register, so both ties are what the
   target has.

   WHY THE MARKER CANNOT BE DROPPED -- the reference count is not optional.
   Without any marker the body is 102 words / 11 DIFF and the RTL is
   INSN-FOR-INSN the target's; only the $v1/$a0 assignment of the two chains
   differs. So the original source produced this same insn set, and the only
   things that can differ are reference counts, births/deaths and qty
   numbers. With 3 references `hh` prices at `floor_log2(3)*3/len = 3/len`
   and must beat the w chain's `14/len_w`, i.e. `len_w > 4.67 * len_hh`.
   `len_hh` is at best 5 (the `lbu`/`sll`/`addu` adjacent), which needs
   `len_w >= 24`, i.e. 12 insns between the unk_16 load and the `sra`; and
   `len_hh` cannot BE 5, because the unk_17 load's two-clock delay always
   leaves a clock that sched1 fills -- the priority-1 leftovers (the two
   constants, `i = 0`, the giv) are ready from the start and are never
   exhausted, so the ready list is never empty and the gap is never a stall.
   Measured `len_hh` is 7 or 9 in every build seen. Hence FOUR references,
   hence one extra RTL insn in the preheader, hence a marker.

   WHY THIS LAST ROTATION IS CLOSED. The rotation is the body order of
   `bh = h + 0xA` against `p[0xA] = (s32)(w+0xA)>>1`, and loop.c hoists
   movables in body order (`move_movables` emits each with
   `emit_insn_before (..., loop_start)`, so the list order is kept). sched2
   emits these three in their sched1 order, and their sched1 order is their
   hoist order, so the emitted order IS the body order. The two cases:

     bh BEFORE the sra statement  -> hoist h+0xA, w+0xA, sra.
       `h`'s last use is the h+0xA insn, now at position 27, so the h
       quantity is 46..55, length 9, 10/9 = 1.111 and it outranks `hh`.
       Registers all correct; 3 DIFF, the rotation.  <- parked body
     bh AFTER the sra statement   -> hoist w+0xA, sra, h+0xA (target order).
       h+0xA is now at position 29, the h quantity is 46..59, length 13,
       10/13 = 0.769, and `hh` at 8/9 = 0.889 outranks it: `hh` takes $v0
       and `h` takes $v1. 7 DIFF, all of it those two chains.

   To have both you need, in the sched1 order, `h+0xA` LATE (for the
   emission) and `h`'s death EARLY (for the price) -- but `h`'s death IS the
   h+0xA insn, since nothing uses `h` after it. The only escapes were each
   measured and each fails:
     * `h` with 6 or more references. The only free way to add one is the
       marker, which then has to sit after the `addu` that defines `h`,
       which lengthens `h` by as much as it gains: `"r"(h)` added once 12,
       twice 7, three times 7 (`"r"(h)` alone, no `"r"(w+8)`, 13).
     * `hh` with a longer range (needs one more insn between the unk_17 load
       and the `addu`): no such insn exists without a word -- the clocks
       around it are a tight pack of priority-3 insns and the window is
       exactly {marker, (w+8)>>1, sll}.
     * `w + 0xA` hoisted before the unk_17 load, which shortens `h`: then
       the w quantity dies early, its price jumps to 14/11 and it is
       allocated FIRST, taking $v0.
     * `bh` as a preheader statement instead of a movable (4 positions) 14;
       `(s32)(w+0xA)>>1` as a preheader local (2 positions) 11; a `w8`
       preloop local for q[6] with the marker on it 6.

   MEASURED, all at 102 words unless stated (verify.py, this unit, -G0).
   The first block is the original sweep; the second is 2026-11-01's.

     parked body, byte-0-first, four halvings inline, c/d locals       84
     byte-0-last only (park header's own note)                        85
     u8 locals for the four halvings (park header's own note)         88
     sibling's preheader-statement form: i=0,c,d,w,a,h,b,e statements
       then `for (; i<4; i++)` with the body 0xA,0xB,1,4,0            89
     same but all four halvings inline, c/d still locals              85
     ditto + `(s32)(w+0xA)>>1` for the target's `sra`                 85
     ditto + four separate `s32` locals for the 0x90D0E0 stores       85
     ditto + 0x48/0x104058 inline literals (insn_count 38 -> 40)      85
     ditto + `bh = h + 0xA` pre-loop, `bh >> 1` in the loop           85
     + `g = 0x90D0E0; __asm__("" : "+r"(g));`                         84
     + `__asm__ volatile("" : "+r"(g));`                              68
     + `__asm__("" : "+r"(g) : "r"(i));`   (the marker that works)    26
     ditto with `(h+0xA)>>1` inline again (bh hoisted, srl rides)     36
     ditto + in-loop `bh = h+0xA` and `"r"(bh)` on the same marker    11
     + `__asm__("" ::"r"(hh))` on a `u32 hh = ...unk_17` local         3
     + phantom `hh*17 + ((hh+hh)-hh-hh)` instead of that marker        7
     + phantom `hh*17 + ((hh<<1)-hh-hh)`                               7
     + phantom `hh*17 + ((hh<<2)-hh-hh-hh-hh)`                         7
     + phantom with `u16 hh` / `u8 hh`                                 7
     + phantom + a second empty `__asm__("")` in the preheader        30

   2026-11-01, second pass. All on the 3-DIFF body above, varying only the
   first marker's operand list and `bh`'s position in the body:

     `__asm__("" ::"r"(hh), "r"(w + 8))`, bh before the sra   3  <- body
     ditto, bh after the sra (the natural order)               7
     ditto, bh anywhere earlier in the body (6 placements)     3 (same 3)
     ditto, bh just before `p[0xB] = bh >> 1`                  7
     `"r"(hh), "r"(w)`                                        11
     `"r"(hh), "r"(h)` / `"r"(h), "r"(hh)`                    11
     `"r"(hh), "r"(w + 8), "r"(h)`                        12 / 10
     `"r"(hh), "r"(w + 8), "r"(h), "r"(h)`                     7
     `"r"(hh), "r"(w + 8), "r"(hh)`                        10 / 9
     `"r"(hh), "r"(w + 8), "r"(h + 8)`                    16 / 13
     `"r"(hh), "r"(w + 8), "r"(w + 0xA)`                  13 / 8
     `"r"(hh), "r"(w + 8), "r"(i)|"r"(p)|"r"(q)|"r"(w+8)`  7 / 3 (same 3)
     a second marker `__asm__("" ::"r"(hh))` after it          3 (giv perm)
     a second marker `__asm__("" ::"r"(hh), "r"(w + 8))`       7 / 3
     a second bare `__asm__("")` before or after it        28..31
     `"r"(hh), "r"(h + 8)` / `"r"(hh), "r"((h+8)>>1)`     14 / 79 (103w)
     `"r"(hh), "r"((w+8)>>1)` / `"r"(hh), "r"(h + 0xA)`   77 / 80 (103w)
     `__asm__("" : "+r"(p) : "r"(hh))`                        18
     `__asm__("" : "+r"(h)|"+r"(w)|"+r"(q[6]) : "r"(hh))`     11
     `__asm__("" : "+r"(q) : "r"(hh))`                         3 (giv perm)
     `__asm__("" ::"r"(hh), "r"(&D_800280B4[q[2]]))`           3 (giv perm)
     `"r"(hh)` added to the in-loop `g` marker instead     81..99
     `register u32 hh __asm__("$3")` INSTEAD of the marker    11 (combine
       deletes the hard-register copy, local-alloc.c is never reached)
     the same pin PLUS the `"r"(hh)` marker                    2 (the two
       `lbu` swap; three devices, so not taken -- see below)
     `do { hh = ...; } while (0)` for flow.c's loop_depth  19..36 (the
       notes split the block, so `hh` stops being block-local)

   A 256-point grid over {marker position} x {`i` init} x {0x48 local} x
   {0x104058 local} x {`(s32)w` cast inside/outside} x {`hh*17` vs
   `(hh<<4)+hh`} x {`w` before/after `hh`} on the parked body: every point 3
   or worse, so none of those knobs touches this residue.

   Inert at 3 on the FIRST body (the marker's position and spelling did not
   matter): marker after q[6], after q[7], before `h = hh*17`, with `"r"(p)`
   added, with `: "memory"` added, `s32 r = q[2]` hoisted,
   `((s32)w + 0xA) >> 1`, `w` declared `s32` with `(u32)` casts on the other
   three halvings, `h = (unk_17 << 4) + unk_17`, a separate `u8 *p = pp`
   walked pointer, `for (i = 0; ...)` instead of a pre-loop `i = 0`, an
   extra empty `__asm__("")` after the buf copy, `w8`/`h8` pre-loop locals,
   `for (; i < 4;)` with `p[0] = buf[i++];` on the LAST buf read, and the
   same with a bare `i++;` statement before the final `p += 0x18`.

   The last two are worth calling out as a NEGATIVE, because they are the
   lever that landed the sibling func_800242D8 (where `i++`'s position
   relative to the pointer bump decides which of two priority-1 insns takes
   sched1's load-delay filler -- see docs/matching-levers.md). It does not
   transfer: the body here has no free slot for the tie to be decided in.
   Measured 2026-11-01.

   Inert at 11 on the first body (only a reference that survives flow
   counts): `__asm__("" : "=r"(dum) : "r"(hh))` and
   `__asm__("" : "=r"(hh) : "0"(hh))` -- non-volatile, deleted as dead
   before flow counts it; `__asm__("" : "+r"(hh))`; `((hh+hh)-hh)*17`;
   `(hh + ((hh+hh)-hh-hh)) * 17`; phantoms whose terms are not all `hh`
   (`((hh+i)-hh-i)`, `((hh+1)-hh-1)`, `((hh-1)-hh+1)`) -- fold cancels those
   before cse ever sees them.

   Worse: `hh` loaded before `w` 85; `h` statement before `w` 85; a cached
   `UnkStrSet *t` row pointer 10; `u8 hh` without a phantom 14; `hh` phantom
   plus a phantom on `h` 76 at 103 words (a phantom on an additive-only value
   leaves one word behind -- combine folds it only when the first term is a
   MULT); `q[6]`/`q[7]` swapped 12; the buf copy moved after the row reads 76;
   an empty `__asm__("")` just before the loop 31; `__asm__("" ::"r"(w))` 12;
   `__asm__("" ::"r"(h))` 13; `i++` moved into the MIDDLE of the body (just
   after the first `p += 0xC`) 91 at 103 words.

   OPCODE CENSUS, re-run on this body 2026-11-01 -- this park does NOT have
   the hidden-load problem that turned out to be the whole of func_80023258's
   residue (where three surplus `lbu` hid inside load-delay `nop`s, so the
   word count and the measurement table both missed them). Every class is
   equal: lbu 6/6, lw 1/1, lwl 7/7, lwr 7/7, swl 7/7, swr 7/7, sb 12/12,
   sh 0/0, sw 8/8.

   NOT TAKEN: `register u32 hh __asm__("$3")` alongside the marker reaches 2
   DIFF (only the two `lbu` out of order). It is a third device for one word
   of residue, and a pin whose own effect is nil without the marker beside it
   -- combine deletes the hard-register copy unless an asm reads it
   (combine.c substitutes the pseudo into both arithmetic uses; the `.combine`
   dump shows insn 56 become a NOTE_INSN_DELETED). Recorded, not used.

   search.py: 37 variants compiled on the 11-DIFF body, none better.
   Hand sweeps of 2026-11-01: ~700 compiled variants, best 3.
   The permuter was not used (hand work, by request).

   Parked by hand; the live comment is in csrc/ovl_main_80020000.c, which also
   carries the declarations this needs. */

/* Types and declarations this needs; they live in csrc/ovl_main_80020000.c,
   which is also where the live comment is. Repeated here so the park compiles
   standalone, which is what tools/decomp/permimport.py needs. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;

typedef struct
{
    u8 b[24];
} UnkBlock24;

typedef struct
{
    u8 *strs[5];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStrSet;

extern u8 D_800200A0[];
extern u8 D_80027F48[];
extern UnkStrSet D_800280B4[];

u8 *func_80022774(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    u32 bh;
    u32 hh;
    s32 g;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    i = 0;
    w = D_800280B4[q[2]].unk_16 * 4;
    hh = D_800280B4[q[2]].unk_17;
    h = hh * 17;
    /* DEVICE: one zero-instruction reference marker doing two jobs, worth 8
       words (11 -> 3) (docs/matching-levers.md, "`__asm__("" ::"r"(x))` IS A
       ZERO-INSTRUCTION REFERENCE").
       (a) `"r"(hh)` is the FOURTH reference on the unk_17 quantity.
       local-alloc's `qty_compare_1` prices a quantity at
       `floor_log2(refs)*refs*size/(death-birth)` and `find_free_reg` then
       takes the lowest register free over its range, so the unk_17 load
       needs to be priced above the unk_16/w chain to take $v1 and leave
       $a0. Three references cannot do it at any live length this block
       admits (the header proves the bound); four can. Every non-volatile asm
       form is deleted before flow counts the reference (11), and a phantom
       zero adds three at once (7).
       (b) `"r"(w + 8)` is what keeps the giv initialiser `p + 0xB` in its
       place. The operand's producer has sched1 priority 3, so the marker
       inherits priority 3 (sched.c:1489) instead of the 2 it has with
       `"r"(hh)` alone, and therefore does not win sched1's one priority-1
       hole at T-13 -- the giv does, which puts its luid above the
       `lo_sum`'s and lets sched2 emit the four preheader words in the
       target's order. Without this operand the first four preheader words
       come out `addiu $t1 | lui $v1 | lw $a0 | addiu $v1` and the score is
       3 with a different residue. The reference it adds lands on pseudo 113
       (3 refs -> 4), whose register is settled well after the three that
       matter.
       Legal spellings ruled out: see the operand table in the header --
       `"r"(w)` 11, `"r"(h)` 11, `"r"(h + 8)` 14, `"r"(i)`/`"r"(p)`/`"r"(q)`
       7 or 3-with-the-old-residue, and the hoisted expressions
       `"r"((w+8)>>1)`/`"r"(h + 0xA)` cost a word (103).
       Warns under -Wall? No. */
    __asm__("" ::"r"(hh), "r"(w + 8));
    q[6] = w + 8;
    q[7] = h + 8;
    for (; i < 4; i++)
    {
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        /* `bh` is computed here, one statement EARLIER than reads naturally,
           because loop.c hoists movables in body order: with it here the
           `h + 0xA` movable lands one position earlier in the sched1 order,
           `h`'s quantity is 9 units instead of 13, and all three chains take
           the target's registers. Written after the `p[0xA]` store below --
           which is the order the target EMITS these three words in -- the
           unk_17 quantity outranks `h` and the score is 7. This is the
           three-word rotation the header describes; it is not a device, it
           is the only side of that trade that is available. */
        bh = h + 0xA;
        p[0xA] = (s32)(w + 0xA) >> 1;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = *(UnkBlock24 *)D_80027F48;
        g = 0x90D0E0;
        /* DEVICE: one marker doing two jobs, worth 25 words (36 -> 11).
           (a) `"+r"(g)` is a SECOND set of g in the loop whose source is not
           invariant, so `consec_sets_invariant_p` fails and `g = 0x90D0E0`
           never becomes a movable: the lui/ori stays in the loop where the
           target has it. The threshold cannot do this job -- cse1 merges the
           four stores onto one pseudo of lifetime 4 and `threshold*1*4 >=
           insn_count` always holds. `"r"(i)` is what makes the asm variant;
           without it (`"+r"(g)` alone) loop.c moves BOTH insns out as a
           two-insn consec movable and the score is 84, and `volatile`
           instead is 68. Re-measured on this body: dropping `"r"(bh)` 30,
           dropping `"r"(i)` 81, dropping the marker entirely 84, `volatile`
           with no inputs 62.
           (b) `"r"(bh)` makes the asm the last use of `h + 0xA`, so
           force_movables finds no movable at `regno_last_uid[bh]` and does not
           link the `srl` to the `addiu`; the addiu is then hoisted on its own
           merit and the shift is left in the loop on the threshold, as the
           target has it. Written `p[0xB] = (h + 0xA) >> 1;` the shift rides
           out free on the addiu's link and the score is 36.
           Legal spellings ruled out for (a): four separate `s32` locals, one
           per store (85 -- cse1 collapses them anyway); `g` assigned 0 before
           the loop to make the gate's basic-block test fail (85 -- the gate
           also needs maybe_never, which a rotated for-loop never gives).
           Warns under -Wall? No -- g is initialised. */
        __asm__("" : "+r"(g) : "r"(i), "r"(bh));
        *(s32 *)(p + 4) = g;
        *(s32 *)(p + 0xC) = g;
        *(s32 *)(p + 0x10) = g;
        *(s32 *)(p + 0x14) = g;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}
