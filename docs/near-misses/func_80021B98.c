#include "common.h"

/* 45 words, 11 DIFF (was 28). IMPROVED 2026-11-01: the park said "allocation
   only", and that was wrong -- there was a structural difference worth 17
   words. The target shifts `n` IN PLACE and EARLY: word 19 is
   `sll $s2, $s2, 7` writing back into n's own register, scheduled into the
   delay slot of the `off` test at word 18, and that register is reused at
   word 29 (`addu $a2, $s2, $zero`) and word 30 (`xor $v0, $v0, $s2`). The
   parked body computed `n << 7` twice inside the inner expression, which puts
   the shift after the lseek call and into a register of its own. Writing
   `n = n << 7;` as its own statement before the `if` took it from 28 to 11.

   WHAT IS LEFT is a single register swap, $s1 against $s2: the target has
   n = $s2 and r = $s1, this body has n = $s1 and r = $s2. Everything else --
   every opcode, immediate, displacement, and the other four callee-saved
   assignments (fd = $s0, -1 = $s3, off = $s4, buf = $s5) -- already matches,
   as does the register save/restore order in the prologue and epilogue.

   So it is a local-alloc tie-break between the `n` and `r` quantities, and
   the usual source handles do NOT move it. Measured, all 45 words / 11 DIFF:

     `n = n << 7;` before the if (this body)                         11
     `fd` declared before `r`                                        11
     `n <<= 7;` instead of `n = n << 7;`                             11
     `r = (... == n) ? 1 : 0;`                                       11
     `s32 r;` with `r = 0;` as a statement before the open call      11

   For reference, the three spellings the original park measured, all 28:
   an explicit -1 local; `fd` declared first with a `len` local; the plain
   form. The -1 IS in a register across both comparisons in this body already
   ($s3, words 15/16 and 24).

   NEXT STEP: read `.lreg`/`.greg` for the two quantities' reference counts
   and birth/death and work out which way qty_compare_1 orders them and why,
   rather than guessing more spellings. Note the priority formula's size
   factor is uniform and cancels -- see the "`qty_size` IS IN WORDS" heading
   in docs/matching-levers.md; do not use the old byte-based arithmetic.

   Parked by hand; the live comment is in csrc/ovl_main_80020000.c, which also
   carries the declarations this needs. */

s32 func_80021B98(u8 *name, u8 *buf, s32 n, s32 off)
{
    s32 r = 0;
    s32 fd;

    fd = func_8006A0A4(name, 1);
    if (fd != -1)
    {
        n = n << 7;
        if (off == 0 || func_8006A0B4(fd, off << 7, 0) != -1)
        {
            r = func_8006A0C4(fd, buf, n) == n;
        }
        func_8006A0E4(fd);
    }
    return r;
}
