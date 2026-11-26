#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
extern u8 D_80045340[];
extern u32 D_800814A0;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s04", func_s04_80024004);

/* The 0x112/0x312 debris node's per-frame step, installed as the +0x10 step at
   three sites by func_s04_80024004 (still an INCLUDE_ASM stub -- parked at
   docs/dng-parks/func_s04_80024004.c), so the four-argument signature is fixed
   by those sites taking its address and `d' is unused. Sets bit 0x8000 in the
   +0x52 halfword of the record at the actor's +0x0, then runs a two-case
   compare chain on +0x4C: state 0 counts the +0x48 timer down and, when it
   expires, re-registers the renderable through func_8004491C, reloads the timer
   from +0x4E and advances to state 1; state 1 counts the same timer down, hands
   the renderable to func_800478B8 and reloads from +0x4E when it expires, then
   retires the actor once the renderable's +0x14 reports 0x6000.
   66 words, 0 differing, FIRST COMPILE. One-member family (dngfam).

   DONOR: csrc/ovl_dng_s08.c func_s08_80024688 (84 words), which `dngcallee'
   named at 4 of 4 symbols; csrc/ovl_dng_s09.c func_s09_80024C84 and
   csrc/ovl_dng_s12.c func_s12_80024850 are the same body. Frame and save list
   are identical (-0x20, s0/s1/ra), but this relink reverses which parameter
   gets the low callee-saved register -- $s0 holds `p' and $s1 holds `c' here,
   where the donor has them the other way round, because this relink never
   touches `b' at all. That costs nothing: it is the allocator following the
   parameter it actually uses.

   THREE FACTS, all priced one at a time from this finished 66-word body:

   1. THE TWO STATES SPELL THE SAME DECREMENT DIFFERENTLY, AND IT IS WORTH A
      WORD. State 0 tests the NEW value (`*(s16 *)(p + 0x48) = *(u16 *)(p +
      0x48) - 1;' then `if (*(s16 *)(p + 0x48) <= 0)', which cse forwards into
      `addiu'/`sh'/`sll'/`bgtz'); state 1 tests the OLD value and must be
      written as the post-decrement `if ((*(s16 *)(p + 0x48))-- <= 0)'. State
      1 spelled state 0's way is 67 words / 4 aligned differences, with an
      extra `sh' and the `addiu' writing the wrong register. The tell is in
      the listing: in state 1 the `sh' of the decremented value sits in the
      `bgtz' DELAY SLOT, so it is unconditional while the test is of the value
      before it.
   2. STATE 0's LAST TWO STORES GO +0x48 THEN +0x4C -- 4 aligned differences
      the other way round, same word count. The emitted order is the same
      either way (`sh' +0x48, then `sh' +0x4C in the `j' delay slot) but the
      two LOADS swap, so this is a case where the listing's store order is
      readable and its load order is not.
   3. MEASURED FREE, so NOT the reason this lands: the read width on +0x4C and
      +0x4E. `*(u16 *)' and `*(s16 *)' both give 66 words / 0 aligned
      differences at every one of the four sites, because each value only
      feeds a halfword store or an increment. The `*(u16 *)' spelling is kept
      because it is the family idiom, not because it was measured to matter. */
void func_s04_8002498C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x4E);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        if ((*(s16 *)(p + 0x48))-- <= 0)
        {
            func_800478B8(c);
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x4E);
        }
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
