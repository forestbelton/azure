#include "common.h"

/* 14 words, 6 DIFF, all register names: the target has the cursor in $a1 and
   the counter in $v1, this has them swapped. See csrc/ovl_main_80020000.c for
   the seven earlier spellings and why the indexed loop must stay.

   RE-MEASURED 2026-11-01, five more spellings, all 14w/6: `q` declared before
   `sum`, `i` declared before both, `i = 0` as a declaration initialiser
   rather than in the `for`, and that last with `while` and with `do`/`while`.
   Two spellings do change the cursor but for the worse -- a walking `q++` or
   `*q++` drops to 13 words (gcc reverses the loop), and dropping the `q`
   local entirely makes the cursor start at `gsw` with 0x208 in the memory
   displacement instead of starting at `gsw + 0x208`.

   The residue is insn ORDER in the loop preheader, not anything spellable.
   Both versions emit `sum = 0` first and both turn `i = 0` into a copy of
   that register; the target then emits the cursor init BEFORE the copy and
   this emits it after, because loop.c inserts the giv's initialisation at the
   loop start, which is after the source's own `i = 0`. local_alloc then
   assigns in insn order, so whichever is initialised first takes $a1. */

s32 func_80021EE0(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 i;

    for (i = 0; i < 0x177E; i++)
    {
        sum ^= q[i];
    }
    return gsw[0x81] == sum;
}
