/* PARK: func_s14_80024AE4, csrc/ovl_dng_s14.c, agent-h33, 2026-11-23.

   TARGET 438 words. BEST BUILT 438 words, 65 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 303 unaligned, which is the
   same body -- the stream is shifted by one word at 0x80024AF4 and every
   later word misses twice). 1 member, 438 words (tools/decomp/dngfam.py).

   THE FUNCTION OWNS jtbl_s14_80024048, SIX ENTRIES, one per case 0..5, and
   case 0 FALLS THROUGH into case 1. Entries, in order:
   .Ls14_80024B58 .Ls14_80024B9C .Ls14_8002505C .Ls14_800250D0
   .Ls14_80025114 .Ls14_80025150.

   WHAT IT DOES. The +0x10 step of the scene's 6-state beam node. The counter
   at +0x10 is bumped on entry; the state at +0xA selects the arm; every arm
   falls out through one shared `*(s16 *)(p + 0x14) = 0;'.
   State 0 seeds the run: zero the counter, bump the state, take the caster's
   facing octant out of its +0x2A angle, clear the eight bytes at +0x16 and
   set the renderable white, then fall into state 1.
   State 1 re-reads the caster's node, asks func_8003DE58 for its muzzle
   offset, writes the mover to the muzzle, and -- only when the caster's
   +0x4 record has bit 0x80 -- saves the mover into the payload at +0x20 with
   a struct assignment and then picks a target: with a locked-on actor at the
   caster's +0x60 it records the three absolute axis deltas and the actor's
   own position, otherwise it WALKS UP TO EIGHT TILES along the octant with
   func_800A44E0 and func_800BCB04 and takes the last tile that was not
   blocked, is below 0x201 and is within 0x3F of the caster. Either way the
   mover gets a velocity of (target - position) / 15, the state is bumped and
   func_800A56E0(0x300) plays.
   States 2..4 integrate that velocity and draw: state 2 moves the mover,
   calls func_s14_800246D8 for the beam line and hands off to
   func_s14_80024428 after 0xF frames, state 3 pulses func_s14_80024428 every
   eighth frame until 0x18, state 4 fires func_s14_80024060 at the recorded
   tile at 0x24.
   State 5 is the kill: clear D_8008346C, set +0x8000 on the node and on
   D_800814A0.

   NO TWIN. xsibs.py over ovl_dng_s14 at --min-words 18 --ratio 0.70 finds
   exactly one near-sibling in the whole unit and it is for a function that is
   not on my list (func_s14_80024374 at 0.723 to town.bin:func_800BC6CC).
   The callee-set intersection recipe does not reach it either: of the eight
   distinctive symbols only func_800A44E0 is rare, and its two landed callers
   are func_800A36B4 and func_800A4BB4 in csrc/ovl_dungeon_8009F644.c -- both
   plain line walks, neither a state machine. func_800A36B4 IS the right model
   for the walk block and three of its measured spellings were taken from
   there and hold: the third argument of func_800A44E0 is sign-extended at the
   call, the running ceiling is the `u16' read of the caster's +0x88, and the
   `(s16)(z - 0x20)' cast on func_800BCB04 is that callee prototype.

   EVERY ONE OF THE 65 ROWS IS IN ONE OF FIVE WINDOWS. Instruction selection,
   word count and the whole control-flow shape agree.

   1. 44 ROWS: `b' AND `q' ARE RIGHT, `n'/`m'/`d' ARE NOT. The target holds
      p in $s3, b in $s6, q in $s7, i in $s1, tx in $s4, ty in $s5, x in $s0
      and y in $s2, and this body agrees on all eight. What differs is the
      caller-saved pair inside the three abs() blocks and inside the
      `b + 0x2 / 0x6 / 0xA' copies: the target computes the subtraction into
      $v0 with the subtrahend in $v1, this body the other way round. All 44
      rows are that one exchange, repeated.

   2. THE SPILL IS ON THE WRONG ONE OF THE TWO WALK RESULTS, 6 ROWS AND THEY
      ARE THE EXPENSIVE ONES. The walk keeps two copies of the accepted tile:
      the target has the Y copy in $fp and SPILLS the X copy to 0x18($sp),
      reloading it with `lhu' twice. This body does the reverse. Everything I
      measured, all 438 words:

        this body                                                      65
        `bx = tx; by = ty;' in the loop instead of `by'-first           65
        the same with the two declarations swapped                      65
        `by' declared s16 instead of u16                                65
        the two `p + 0x50 / p + 0x52' stores swapped                    65
        the two `t + 0x2 / t + 0x6' shifts swapped                      65
        `u16 bx[1]' -- a one-element array, to force it into memory      65
        `s16 out[5]' with out[4] standing in for bx                     65 (435 words)
        `bx' and `by' both plain `s16' with `(s16)bx << 6' at the use   185
        no `bx'/`by' at all, `tx'/`ty' used directly                  (not reachable: the asm sets all four)

      The two have IDENTICAL reference counts and near-identical live
      lengths, so allocno_compare's ratio ties and the tie goes to the lower
      allocno number -- which declaration order does not move. This is the
      one window that needs the `-da' recipe in matching-levers.md rather
      than another spelling sweep.

   3. ONE DELAY SLOT, 2 ROWS. At 0x80024CB8 the target fills the
      `beqz $v0, .Ls14_80024E00' slot with `lui $a0, %hi(0x88888889)' -- the
      /15 reciprocal, hoisted out of BOTH arms -- and this body fills it with
      the walk counter's `i = 0'. Moving `i = 0' to the top, bottom or middle
      of the else arm does not change it (all 65), and nor does splitting the
      counter into two variables (that one is much worse, see 5).

   4. ONE LOAD-DELAY NOP, 2 ROWS. At 0x80024D04 the target leaves the
      `lw $v0, 0x60($s7)' delay slot EMPTY and loads `b + 0xA' two words
      later; this body fills the slot with that load. Both are the same
      insns. Reading `b + 0xA' into a local first was not tried and is the
      obvious next move.

   5. THE PREHEADER, 4 ROWS. The target emits a DEAD `lui $v0, 0x8007' --
      a hoisted `%hi(D_8006CCD8)' whose register is overwritten on the first
      loop insn -- and loads the caster tile's +0x25 before its +0x24. This
      body has a nop in place of the dead lui and loads +0x24 first. Writing
      `tx = m[0x24]; ty = m[0x25];' instead flips neither and costs 34 more
      rows (99).

   MEASURED FACTS THAT DID LAND AND MUST NOT BE UNDONE. These are the five
   that took the body from 192 aligned differences to 65, each one re-measured
   on this body:

     THE SIX-WORD PAYLOAD SAVE IS A STRUCT ASSIGNMENT. `*(UnkVec *)(p + 0x20)
     = *b;' gives MIPS block_move_sequence's four `lw' then four `sw' then two
     `lw' then two `sw'. Written as six scalar copies it is load/nop/store
     eighteen times: 446 words, 192 differing.

     ONE LOOP COUNTER, NOT TWO. State 0's eight-byte clear and the walk share
     $s1 in the target, so they are the SAME variable. Split into `i' and `j'
     the whole global allocation shifts one register down -- p lands in $s2,
     b in $s5, q in $s6 -- and it is 113 differing against 63.

     THE TWO WALK RESULTS ARE ASSIGNED Y-FIRST. `by = ty; bx = tx;' in both
     the preheader and the loop tail is 63; `bx = tx; by = ty;' is 113.

     func_800A44E0's FIRST TWO PARAMETERS ARE `s32', NOT `u16'. Through the
     `u16' prototype that csrc/ovl_dungeon_8009F644.c uses, the two
     `(x << 6) & 0xFFC0' masks stop being `andi' and become a shared
     `li $t0, -64' plus two `and': 440 words, 194 differing.

     THE +0x42 STORE COMES BEFORE THE +0x40 ZERO IN THE SOURCE. The target
     loads the actor's +0x88 ABOVE both stores, which it can only do if the
     zero store is second; written +0x40 first the body is 437 words -- one
     SHORT, because the load moves into the store's delay slot.

   AND THE `(bx << 16) >> 10' SPELLING IS NOT COSMETIC. The accepted tile is
   scaled by `((s32)(bx << 16) >> 10) + 0x20'. As `((s16)bx << 6) + 0x20' gcc
   drops the sign extension entirely and emits a bare `sll 6': 435 words.


   DISPATCHER PASS, 2026-11-23: FOUR MORE SPELLINGS, ALL REJECTED. The window
   above is an allocno_compare TIE between the two tile copies, so I tried to
   break it by moving a BIRTH or a DEATH rather than by adding a reference --
   live length is the denominator of the priority, so either should work in
   principle. None did:

       the park's body (baseline)                  438 words,  65 aligned
       bx born first (tx, bx, ty, by)              439 words, 118
       by born last  (ty, tx, bx, by)              438 words, 115
       in-loop copies swapped (bx then by)         438 words,  65
       by's final store first (y before x)         438 words,  65

   The two statement swaps are inert and the two birth moves are sharply
   worse, which says the tie is NOT decided by the order of these four
   statements at all. Twelve spellings have now failed on this window (eight
   of h33's, four of mine), so the park's own advice stands and is worth
   repeating: READ THE -da DUMP AND NAME THE TWO PSEUDOS before trying
   anything else here. Note also that the reference-bump recipe that worked
   on func_b12_80173FDC (route a nearby constant through the pseudo that is
   short of a reference) is NOT available here: both tile copies are live
   from before the loop until the two stores after it, so neither can carry a
   constant.

   DECLARATIONS
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   s32 func_800A44E0(s32, s32, s16, u32);
   s32 func_800BCB04(s32, s32, s16);
   s32 func_800A56E0(s32);
   s32 func_s14_800246D8(u8 *);
   void func_s14_80024060(s16, s16, u8 *, u8);
   void func_s14_80024428(u8 *, UnkVec *, s32, u8 *);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   END DECLARATIONS
*/
void func_s14_80024AE4(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *n;
    u8 *m;
    u8 *e;
    u8 *r;
    u8 *t;
    s16 out[4];
    s32 d;
    s32 i;
    s32 x;
    s32 y;
    s32 k;
    s16 tx;
    s16 ty;
    u16 bx;
    u16 by;
    s16 h;
    s16 z;

    q = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(q + 0x2A) >> 9) & 7;
        t = p + 7;
        for (i = 7; i >= 0; i--, t--)
        {
            t[0x16] = 0;
        }
        *(u32 *)(c + 0xC) = 0x808080;
        /* fallthrough */
    case 1:
        n = q - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(n + 0xC) + 0x8), (UnkSlot *)*(u8 **)(n + 0xC), out,
                          0) == 0 &&
            (*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        m = *(u8 **)(n + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) += out[0];
            *(s16 *)((u8 *)b + 0x6) += out[1];
            *(s16 *)((u8 *)b + 0xA) += out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        *(UnkVec *)(p + 0x20) = *b;
        e = *(u8 **)(q + 0x60);
        if (e != NULL)
        {
            n = *(u8 **)(e - 0x18);
            d = *(s16 *)(n + 0x2) - *(s16 *)((u8 *)b + 0x2);
            if (d < 0)
            {
                d = -d;
            }
            out[0] = d;
            d = *(s16 *)(n + 0x6) - *(s16 *)((u8 *)b + 0x6);
            if (d < 0)
            {
                d = -d;
            }
            out[1] = d;
            d = *(s16 *)(*(u8 **)(q + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA);
            if (d < 0)
            {
                d = -d;
            }
            out[2] = d;
            r = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
            *(s16 *)(p + 0x50) = r[0x24];
            *(s16 *)(p + 0x52) = r[0x25];
            *(s32 *)(p + 0x38) = *(s32 *)(n + 0x0);
            *(s32 *)(p + 0x3C) = *(s32 *)(n + 0x4);
            *(s16 *)(p + 0x42) = *(u16 *)(*(u8 **)(q + 0x60) + 0x88);
            *(s16 *)(p + 0x40) = 0;
            b->unk_0C = (*(s32 *)(p + 0x38) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x3C) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x40) - b->unk_08) / 15;
        }
        else
        {
            i = 0;
            m = *(u8 **)(n + 0xC);
            ty = m[0x25];
            tx = m[0x24];
            by = ty;
            bx = tx;
            do
            {
                x = tx;
                y = ty;
                if ((s16)func_800A44E0((x << 6) & 0xFFC0, (y << 6) & 0xFFC0,
                                       *(s16 *)(q + 0x88), (s16)(*(u16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                k = *(s16 *)(p + 0xE) * 2;
                h = func_800BCB04((((x + *(s16 *)(k + (s32)D_8006CCD8)) << 6) + 0x20) & 0xFFE0,
                                  (((y + *(s16 *)(k + (s32)D_8006CCE8)) << 6) + 0x20) & 0xFFE0,
                                  (s16)(*(u16 *)(q + 0x88) - 0x20));
                if (h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(q + 0x88)) < -0x3F)
                {
                    break;
                }
                i++;
                k = *(s16 *)(p + 0xE) * 2;
                tx = tx + *(u16 *)(k + (s32)D_8006CCD8);
                ty = ty + *(u16 *)(k + (s32)D_8006CCE8);
                by = ty;
                bx = tx;
            } while (i < 8);
            t = p + 0x38;
            *(s32 *)(t + 0x8) = 0;
            *(s32 *)(t + 0x4) = 0;
            *(s32 *)(p + 0x38) = 0;
            *(s16 *)(t + 0x2) = ((s32)(bx << 16) >> 10) + 0x20;
            *(s16 *)(t + 0x6) = ((s32)(by << 16) >> 10) + 0x20;
            *(s16 *)(t + 0xA) = -0x400;
            *(s16 *)(t + 0xA) = func_800BCB04(*(u16 *)(t + 0x2), *(u16 *)(t + 0x6), -0x400);
            if (*(s16 *)(t + 0xA) >= 0x201)
            {
                *(s16 *)(t + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            }
            b->unk_0C = (*(s32 *)(p + 0x38) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x3C) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x40) - b->unk_08) / 15;
            *(s16 *)(p + 0x50) = bx;
            *(s16 *)(p + 0x52) = by;
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        func_800A56E0(0x300);
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        func_s14_800246D8(p);
        if (*(s16 *)(p + 0x10) >= 0xF)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            func_s14_80024428(p, b, 0, p + 0x16);
        }
        break;
    case 3:
        if ((*(u16 *)(p + 0x10) & 7) == 0)
        {
            func_s14_80024428(p, b, (s16)*(u16 *)(p + 0x10) >> 3, p + 0x16);
        }
        if (*(s16 *)(p + 0x10) >= 0x18)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x24)
        {
            func_s14_80024060(*(s16 *)(p + 0x50), *(s16 *)(p + 0x52), q, p[0x9]);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(s16 *)(p - 0x2) = *(u16 *)(p - 0x2) | 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
