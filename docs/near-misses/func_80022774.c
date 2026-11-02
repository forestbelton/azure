#include "common.h"

/* NEAR MISS: 102 words -- the right count -- and 3 DIFF, down from 84. Every
   opcode, immediate and displacement matches and every register matches; what
   is left is a three-word PERMUTATION at the top of the preheader:

       built   addiu $t1,$t2,0xB | lui $v1,%hi | lw $a0,8($a1) | addiu $v1,%lo
       target  lui $v1,%hi | addiu $v1,%lo | lw $a0,8($a1) | addiu $t1,$t2,0xB

   i.e. loop.c's giv initialiser `p + 0xB` is scheduled FIRST of the four
   instead of LAST. sched1 is a backward list scheduler here (the .sched dump
   prints "ready list at T-1 .. T-29"); the giv has priority 1 and no in-block
   dependents, so it sits in the ready list from the start and is taken when
   nothing is hazard-boosted. Removing the ONE extra insn the `hh` marker puts
   in the preheader puts the giv back in its place (see the table), so this
   residue is a consequence of that marker, not of the body.

   WHAT WAS WRONG WITH THE PARKED 84, and what fixed it -- all measured from
   the per-pass RTL dumps (`cpppsx` + `cc1psx -O2 -G0 -da`, dumps under
   ~/.cache/azure-psyq/drive_c/users/case/AppData/Local/Temp/PQ2.*):

   1. The park's stated cause ("loop.c hoists all FOUR halvings, the target
      hoists three") was right about the symptom and wrong about the lever.
      loop.c's move_movables test is
          threshold * savings * lifetime >= insn_count
      with threshold = (loop_has_call ? 1 : 2) * (1 + n_non_fixed_regs),
      decaying by 3 per move (loop.c:1631, :1719, :1904). MEASURED here:
      threshold starts at 60 and insn_count is the ".loop" dump's "N real
      insns". So the way to keep an invariant IN the loop is to spend the
      threshold on movables that come EARLIER in body order, and to raise
      insn_count. Writing 0x48 and 0x104058 as inline literals instead of
      pre-loop locals adds two movables and two insns, which is exactly what
      moves `(h+0xA)>>1`, `%hi(D_80027F48)` and the 0x90D0E0 constant from
      "moved to" to "not desirable" in the dump.
   2. The in-loop `srl` of `h+0xA` can NEVER be left behind while its `addiu`
      is hoisted if both are movables: force_movables (loop.c:1245) links the
      shift to the addiu, and a forced movable moves for free regardless of
      the threshold. So `h + 0xA` has to be a non-movable -- here a pre-loop
      statement `bh = h + 0xA`, or an in-loop one whose register's last use is
      a non-movable insn.
   3. The 0x90D0E0 constant cannot be blocked by the threshold at all: cse1
      merges the four stores onto one pseudo whose lifetime is 4, so
      threshold*1*4 >= insn_count always holds. It takes a second, variant set
      of the same register to make the set non-movable.

   Two markers are what close it, both of the established kinds. See the
   DEVICE comments at the sites.

   MEASURED, all at 102 words unless stated (verify.py, this unit, -G0):

     parked body, byte-0-first, four halvings inline, c/d locals     84
     byte-0-last only (park header's own note)                       85
     u8 locals for the four halvings (park header's own note)        88
     sibling's preheader-statement form: i=0,c,d,w,a,h,b,e statements
       then `for (; i<4; i++)` with the body 0xA,0xB,1,4,0           89
     same but all four halvings inline, c/d still locals             85
     ditto + `(s32)(w+0xA)>>1` for the target's `sra`                85
     ditto + four separate `s32` locals for the 0x90D0E0 stores      85
     ditto + 0x48/0x104058 inline literals (insn_count 38 -> 40)     85
     ditto + `bh = h + 0xA` pre-loop, `bh >> 1` in the loop          85
     + `g = 0x90D0E0; __asm__("" : "+r"(g));`                        84
     + `__asm__ volatile("" : "+r"(g));`                             68
     + `__asm__("" : "+r"(g) : "r"(i));`   (the marker that works)   26
     ditto with `(h+0xA)>>1` inline again (bh hoisted, srl rides)    36
     ditto + in-loop `bh = h+0xA` and `"r"(bh)` on the same marker   11
     + `__asm__("" ::"r"(hh))` on a `u32 hh = ...unk_17` local        3  <- body
     + phantom `hh*17 + ((hh+hh)-hh-hh)` instead of that marker       7
     + phantom `hh*17 + ((hh<<1)-hh-hh)`                              7
     + phantom `hh*17 + ((hh<<2)-hh-hh-hh-hh)`                        7
     + phantom with `u16 hh` / `u8 hh`                                7
     + phantom + a second empty `__asm__("")` in the preheader       30

   Inert at 3 (the marker's position and spelling do not matter): marker after
   q[6], after q[7], before `h = hh*17`, with `"r"(p)` added, with
   `: "memory"` added, `s32 r = q[2]` hoisted, `((s32)w + 0xA) >> 1`,
   `w` declared `s32` with `(u32)` casts on the other three halvings,
   `h = (unk_17 << 4) + unk_17`, a separate `u8 *p = pp` walked pointer,
   `for (i = 0; ...)` instead of a pre-loop `i = 0`, an extra empty
   `__asm__("")` after the buf copy, `w8`/`h8` pre-loop locals,
   `for (; i < 4;)` with `p[0] = buf[i++];` on the LAST buf read, and the same
   with a bare `i++;` statement before the final `p += 0x18`.

   The last two are worth calling out as a NEGATIVE, because they are the
   lever that landed the sibling func_800242D8 (where `i++`'s position
   relative to the pointer bump decides which of two priority-1 insns takes
   sched1's load-delay filler -- see docs/matching-levers.md). It does not
   transfer: this residue is in the PREHEADER, not the body, and the body here
   has no free slot for the tie to be decided in. Measured 2026-11-01.

   Inert at 11 (the `hh` marker's reference is what moves the allocation, and
   only a reference that survives flow counts): `__asm__("" : "=r"(dum) :
   "r"(hh))` and `__asm__("" : "=r"(hh) : "0"(hh))` -- non-volatile, deleted
   as dead before flow counts it; `__asm__("" : "+r"(hh))`; `((hh+hh)-hh)*17`;
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

   search.py: 37 variants compiled on the 11-DIFF body, none better.
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
    /* DEVICE: a zero-instruction reference (docs/matching-levers.md, "`__asm__("" ::"r"(x))`
       IS A ZERO-INSTRUCTION REFERENCE"). Worth 8 words: it swaps the allocation
       order of two block-local quantities in the preheader, so the unk_17 load
       takes $v1 and the unk_16/w chain takes $a0 as the target has them (11
       DIFF -> 3). local-alloc sorts by `floor_log2(refs)*refs*size/(death-birth)`
       and takes the lowest free register, so the pair needs the unk_17
       quantity at exactly FOUR references: three is below the w chain and five
       or more is above the h chain. A phantom zero adds three at once (7 DIFF)
       and every non-volatile asm form is deleted before flow counts it (11).
       Without it the function is 11 DIFF, all of it those two chains'
       registers. Its price is the three words still differing: it is one more
       insn in the preheader and sched1 then takes the giv initialiser first.
       Warns under -Wall? No. */
    __asm__("" ::"r"(hh));
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
        p[0xA] = (s32)(w + 0xA) >> 1;
        bh = h + 0xA;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = *(UnkBlock24 *)D_80027F48;
        g = 0x90D0E0;
        /* DEVICE: one marker doing two jobs, worth 25 words (36 DIFF -> 11).
           (a) `"+r"(g)` is a SECOND set of g in the loop whose source is not
           invariant, so `consec_sets_invariant_p` fails and `g = 0x90D0E0`
           never becomes a movable: the lui/ori stays in the loop where the
           target has it. The threshold cannot do this job -- cse1 merges the
           four stores onto one pseudo of lifetime 4 and `threshold*1*4 >=
           insn_count` always holds. `"r"(i)` is what makes the asm variant;
           without it (`"+r"(g)` alone) loop.c moves BOTH insns out as a
           two-insn consec movable and the score is 84, and `volatile` instead
           is 68.
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
