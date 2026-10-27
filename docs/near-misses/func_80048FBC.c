/* func_80048FBC (unk_1C540) -- 18 of 18 words, 7 DIFF. NOT compiled.
 *
 * Written out of its carved-sweep row on 2026-10-27 so there is a source to
 * sweep. Eighteen words, one statement, and every one of them right except
 * the two callee-saved registers.
 *
 * `p[func_80048F8C(p, n)] = func_8004B404(0x104);` -- both arguments of the
 * first call pass through untouched, so neither move is emitted, and the
 * second call's constant is its only setup.
 *
 * WHAT IS LEFT is ONLY which of $s0/$s1 holds the pointer and which the index
 * chain: the target keeps the pointer in $s0 and builds the chain in $s1,
 * this source the other way round. local-alloc's own arithmetic says why --
 * the chain (the result copy, the `sll` and the `addu`, all one quantity) has
 * six references over four insns and the pointer two over nine, so the chain
 * outranks the pointer and takes $s0 first.
 *
 * MEASURED, all 18 words and all 7 DIFF: the statement as one expression; an
 * `s32 i` local for the index; a `u32 v` local for the value as well, so both
 * calls are their own statements; `*(u32 *) ((u8 *) p + i * 4)` as the
 * destination; `*(p + i)`; and a `(u32)` cast on the call's result. The row
 * also measured `p += f(...)` and a separate `q = p + i`, which move the
 * arithmetic ahead of the second call instead (16 DIFF).
 *
 * So the lever is not a spelling of the statement. To flip the two
 * quantities, either the pointer needs more references or the chain fewer,
 * at zero instruction cost -- matching-levers section 3's dead-reference
 * entry is the thing to read, and `advise.py --greg` prints both priorities.
 * Note the pointer's nine-insn range is the whole function (it is the
 * parameter, live from entry to the store), so shortening it is not
 * available; raising its reference count is the only side with any room.
 */

#include "common.h"

s32 func_80048F8C(u32 *, s32);
u32 func_8004B404(s32);

void func_80048FBC(u32 *p, s32 n) {
    p[func_80048F8C(p, n)] = func_8004B404(0x104);
}
