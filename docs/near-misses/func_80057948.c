/* func_80057948 (unk_2879C) -- 64 of 64 words, 9 DIFF. NOT compiled.
 *
 * Stops every D_80085458 entry whose +0x06 and +0x0A match the two arguments,
 * OR-ing their sound handles together, except that an entry with +0x1D set
 * gets +0x1C |= 0x80 instead; then stops the accumulated handles.
 *
 * WHAT IS PAID FOR -- 69 words on the first draft, 64 now
 *
 * * The entry pointer is INDEXED inside the body. Walked from a pointer
 *   declared outside, loop.c keeps the walk AND adds a second giv biased to
 *   +0x1C, so the five entry accesses get negative displacements and the loop
 *   carries an extra callee-saved register: five words.
 * * `D_80073740[i]` is indexed too. Walked as `*t` with `t++` it is 22 DIFF.
 * * The bound takes the `__asm__` SCALAR alias: the listing re-materialises
 *   `lui $v0, %hi(D_80073734)` inside the loop with the `lw` using the SAME
 *   register, which is the assembler macro's form.
 * * `(a & 0xFF)` and `(b & 0xFF)` are left in the comparison; loop.c hoists
 *   them into $s5 and $s4 by itself.
 *
 * WHAT IS LEFT: 9 words, one register swap, and `advise.py --greg` prices it
 * exactly. `m` (the OR accumulator) is pseudo 83 -- 7 refs, live 36, priority
 * 0.389 -- and the D_80073740 giv is pseudo 116 -- 7 refs, live 28, priority
 * 0.500. Equal references, so the shorter live range wins and the giv takes
 * $s2 where the target has `m` there. The margin is 22.2%, and the table's own
 * advice is "to flip 83: 8 refs (+1), live <= 28 (-8)".
 *
 * `m`'s live range spans the guard -- it has to, because the `beqz $s2` after
 * the loop is reached both from the loop's exit and from the guard -- so the
 * live-range side is not available. That leaves ONE more reference to `m` that
 * costs no instruction, which is matching-levers section 3's trick and is what
 * to look for next.
 *
 * Measured and NOT working: `m` declared before `i` (11 DIFF); both
 * initialised by statements rather than initialisers, in either order (9 and
 * 11); and a local for the loaded handle so `D_80073740[i]` appears once (9).
 */

#include "common.h"

extern s32 D_80073734s __asm__("D_80073734");

void func_80056D44(s32, u8 *);
void func_8005E97C(s32, s32);

void func_80057948(s32 a, s32 b)
{
    s32 i = 0;
    s32 m = 0;
    if (D_80073734s > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            if ((a & 0xFF) == *(u16 *)(e + 6) && (b & 0xFF) == *(u16 *)(e + 0xA))
            {
                if (e[0x1D] == 0)
                {
                    func_80056D44(i, e);
                    m |= D_80073740[i];
                    func_8005E97C(0, D_80073740[i]);
                    *(u16 *)(e + 0x1A) = 0;
                }
                else
                {
                    e[0x1C] |= 0x80;
                }
            }
            i++;
        } while (i < D_80073734s);
    }
    if (m != 0)
    {
        func_8005E97C(0, m);
    }
}
