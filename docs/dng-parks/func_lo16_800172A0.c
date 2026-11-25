/* func_lo16_800172A0 -- ovl_dng_lo16, 107 target words.
   PARKED by agent h176, batch 35, 2026-11-25.  107 words / 4 ALIGNED
   DIFFERENCES.  One-member family (dngfam), so no replication rides on it.

   WHAT IT IS: the dungeon map engine's "spawn a floating marker" routine.
   It picks a random angle, offsets a point by rcos/rsin of it, clamps the
   point into the grid's extents, fills a node record at D_80083178 + 0xB8,
   installs func_800C7B38 as its 0xB4 callback and bumps D_80083460h[5].

   THE REMAINING RESIDUE IS FOUR ROWS AND IT IS WHOLLY UNDERSTOOD:

       +   76                sh s4,38(s1)
       +   77                sw s3,12(s1)
       -        81 800173E4   sw $s3, 0xC($s1)
       -        83 800173EC   sh $s4, 0x26($s1)

   i.e. the `r + 0xC' and `r + 0x26' stores are emitted FIVE WORDS EARLY,
   because the `do { } while (0)' the body needs as a reference bump is also
   a sched1 BARRIER and the address materialisations the target hoists above
   those two stores cannot cross it.  Everything else -- all 107 words, every
   register, every delay slot -- agrees.

   WHY THE DEVICE IS THERE, with the arithmetic, from `-da' on the real unit:

     ;; 11 regs to allocate: 105 121 88 103 119 82 84 85 80 83 81
     Register 81 used 3 times across 150 insns     <- n, the 2nd parameter
     Register 83 used 3 times across  78 insns     <- q, the grid pointer

   allocno_compare's `floor_log2(R) * R / L' gives q 1*3/78 = 0.0385 and n
   1*3/150 = 0.0200, so q is allocated first and takes $s4; the target has
   n in $s4 and q in $s5.  No reference count or live length reachable from
   this source flips that: q cannot drop below n at 2 refs (0.0256) and n
   cannot shorten (it is a parameter read in the last statement).  n needs a
   FOURTH reference -- 2*4/150 = 0.0533 > 0.0385 -- and `flow.c:2122' only
   grants one per ENCLOSING LOOP, so only a loop will do it.  Measured:
   without the device the body is 107 words / 10 aligned differences and
   every one of the ten rows is the $s4/$s5 exchange.

   SIX DECLARATION PERMUTATIONS of `b q r a t k v' all measure 107 / 10, so
   allocation PRIORITY ORDER is ruled out as a lever and the brief's
   conclusion holds: the cause is in the statement order / arithmetic.

   DEVICE SCOPES MEASURED (all 107 words unless noted):
     do{} round `*(s16 *)(r + 0x26) = n' with 0xC and 0x18 BEFORE it     6
     ... with 0xC and 0x18 AFTER it                     KEPT, see below  4
     ... with the device after the 0xB4 store                           8
     ... with the device after the whole k block                 108 /  13
     do{} round the three stores 0xC/0x18/0x26                           6
     do{} round 0x18 + 0x26                                              6
     do{} round 0x26 + 0xB4 + k + 0x98 + 0x10                   108 /  16
     do{} round 0xC .. 0x10 (seven statements)                  108 /  13
     do{} round the whole r-record block                        108 /  38
     do{} round `k = n + ...' alone                             108 /  20
     do{} round the k block (k, 0x98, 0x10)                     109 /  13
     do{} round 0x98 = k alone (no n inside -- no bump)         107 /  15
     no device at all                                           107 /  10
   NEXT AGENT: the thing to find is a FOURTH SOURCE REFERENCE TO n that is
   not a loop, or a statement order in which the barrier is harmless.  Two
   spellings that do NOT supply it, both measured: writing the 0x98/0x10
   value out twice instead of through `k' (113 words / 7 -- the b+0x98 store
   invalidates cse's record of r+0x24 so the second copy reloads and the
   multiply is duplicated), and reading 0x10's value back out of b+0x98
   (107 / 10, identical to no device -- cse forwards the stored value and
   there is no extra reference).

   FACTS MEASURED AND LOAD-BEARING (these are the content of this park):

   1. `s32 a' for the angle, NOT `s16'.  `a = (rand() & 0x1FFF) - 0x1000'
      as an `s16' sign-extends at BOTH rcos/rsin call sites and is
      110 words / 52 aligned differences.  The target passes `$s2' raw.
   2. `*(s16 *)(b + 0xA8) = *(u16 *)(p + 0xA) - 0x400;' must come BEFORE
      `*(s16 *)(b + 0xAC) = 0;'.  The other order is 108 words / 14: the
      0xA shortword load then has nothing to fill its load delay and gets a
      `nop' where the target schedules the independent 0xAC store into it.
      This is the cheapest single fact in the body and it is a STATEMENT
      ORDER, not a spelling.
   3. The 0xB0 sign is a TWO-ARM `if/else' -- `if ((rand() & 1) != 0)
      { v = 0x800; } else { v = -0x800; }'.  Written as
      `v = -0x800; if ((rand() & 1) != 0) { v = 0x800; }' the -0x800 is
      hoisted into a CALLEE-SAVED register ahead of the call and the body is
      110 / 52 with the 0xB0 block three words long.  NOTE THAT THIS IS THE
      OPPOSITE OF func_lo16_8001E824's case 4, 300 words away in the same
      unit, where the `v = const; if (...) v = other;' form is what lands --
      the brief's "TWO BODIES IN ONE UNIT CAN WANT OPPOSITE ANSWERS", with
      both halves measured here.
   4. The clamp reads its bound TWICE with DIFFERENT SIGNEDNESS and both are
      real: `if (t >= *(s16 *)(q + 0x1C))' is the `lh' and
      `*(s16 *)(b + 0xA4) = *(u16 *)(q + 0x1C) - 1' is the `lhu'.  One
      shared read cannot produce both.
   5. THREE POINTER LOCALS OFF ONE `lui', the unit's central addressing
      idiom one step further than func_lo16_80016E6C's two: the base symbol
      is D_80083178 and `q = b + 0x1C4' is the GRID DESCRIPTOR
      (0x80083178 + 0x1C4 == 0x8008333C) while `r = b + 0xB8' is the node
      record.  So this is a FIFTH way the grid at 0x8008333C is reached in
      this unit, and the comment at the top of ovl_dng_lo16.c should say so.
   6. `*(s16 *)(r + 0x24) = 0x40;' is READ BACK (`lh $v1, 0x24($s1)') rather
      than constant-folded, because the intervening stores through `r'
      invalidate cse's record of that MEM.  No device is needed for it.

   DECLARATIONS
   extern Unk80083178 D_80083178;
   extern u16 D_80013714;
   extern u16 D_80083460h[] __asm__("D_80083460");
   s32 func_80064584(s32);
   s32 func_800644B8(s32);
   void func_800C7B38(u8 *);
   END DECLARATIONS
*/
void func_lo16_800172A0(u8 *p, s32 n)
{
    u8 *b;
    u8 *q;
    u8 *r;
    s32 a;
    s16 t;
    s16 k;
    s16 v;

    b = (u8 *)&D_80083178;
    q = b + 0x1C4;
    r = b + 0xB8;
    a = (rand() & 0x1FFF) - 0x1000;
    if ((D_80013714 & 2) == 0)
    {
        t = *(u16 *)(p + 0x2) + (func_80064584(a) << 1);
        *(s16 *)(b + 0xA4) = t;
        if (t < 0)
        {
            *(s16 *)(b + 0xA4) = 0;
        }
        else if (t >= *(s16 *)(q + 0x1C))
        {
            *(s16 *)(b + 0xA4) = *(u16 *)(q + 0x1C) - 1;
        }
        t = *(u16 *)(p + 0x6) + (func_800644B8(a) << 1);
        *(s16 *)(b + 0xA6) = t;
        if (t < 0)
        {
            *(s16 *)(b + 0xA6) = 0;
        }
        else if (t >= *(s16 *)(q + 0x1E))
        {
            *(s16 *)(b + 0xA6) = *(u16 *)(q + 0x1E) - 1;
        }
        *(s16 *)(b + 0xA8) = *(u16 *)(p + 0xA) - 0x400;
        *(s16 *)(b + 0xAC) = 0;
        if ((rand() & 1) != 0)
        {
            v = 0x800;
        }
        else
        {
            v = -0x800;
        }
        *(s16 *)(b + 0xB0) = v;
        *(u8 **)(r + 0x0) = r + 4;
        *(s32 *)(r + 0x14) = 1;
        *(s16 *)(r + 0x24) = 0x40;
        do
        {
            *(s16 *)(r + 0x26) = n;
        } while (0);
        *(u8 **)(r + 0xC) = p;
        *(s32 *)(r + 0x18) = 0;
        *(u32 *)(b + 0xB4) = (u32)func_800C7B38;
        k = n + (*(s16 *)(r + 0x24) + 1) * 48;
        *(s16 *)(b + 0x98) = k;
        *(s32 *)(r + 0x10) = k;
        D_80083460h[5]++;
    }
}
