/* func_80056C30 (unk_1C540) -- 67 of 67 words, 4 DIFF. NOT compiled.
 *
 * Resets every live D_80085458 entry -- index, +0x1A, +0x18/+0x70/+0x74 all
 * 0x40 -- stopping each one's sound and OR-ing the handles together, then
 * fills D_80085014's mixer fields and stops the accumulated handles.
 *
 * WHAT IS PAID FOR -- 78 words on the first draft, 67 now
 *
 * * The loop counter is `s32`, not `s16`. As an `s16` gcc sign-extends it at
 *   every use (`sll`/`sra` before the call and before the compare) and keeps
 *   an extra copy for the increment: six words and two more callee-saved
 *   registers. The listing stores it with `sh` and compares it with `slt`
 *   straight out of $s2.
 *
 * * The bound uses the `__asm__` SCALAR alias, so its load is one macro insn
 *   that maspsx splits inside the loop. Through the incomplete array the
 *   compiler splits it itself and loop.c hoists the `lui` half into a
 *   callee-saved register -- two words. That is the OPPOSITE of what
 *   func_8005B070 needs for the same symbol; see matching-levers section 6.
 *
 * * The entry pointer is INDEXED inside the body (`u8 *e = D_80085458 +
 *   i * 0x78;`). Walked from a pointer declared outside, loop.c keeps the
 *   walked pointer AND adds a second giv biased to +0x74, so the five stores
 *   come out with negative displacements and the loop updates two pointers:
 *   four words.
 *
 * * `D_80073740[i]` is indexed too, which gives the `addiu $s3, $s3, 0x4`
 *   giv the listing has.
 *
 * WHAT IS LEFT: one slot. `addu $a0, $zero, $zero` -- func_8005E97C's first
 * argument -- sits immediately after func_80056DB4's delay slot in the
 * listing and immediately before the second `jal` here, with the three giv
 * updates on the other side of it:
 *
 *     target                   this
 *     jal   func_80056DB4      jal   func_80056DB4
 *      addu $a0, $s2            addu $a0, $s2
 *     addu  $a0, $zero, $zero  addiu $s3, $s3, 0x4
 *     addiu $s3, $s3, 0x4      addiu $s1, $s1, 0x78
 *     addiu $s1, $s1, 0x78     addiu $s2, $s2, 0x1
 *     addiu $s2, $s2, 0x1      addu  $a0, $zero, $zero
 *     jal   func_8005E97C      jal   func_8005E97C
 *
 * The insn has NO dependencies, so nothing in the dependence graph pins it;
 * for the target's order its LUID would have to be ahead of the giv updates,
 * and expand_call emits a call's argument setup when the call is expanded,
 * which is after the `i++` the givs come from. A `s32 w = 0;` assigned before
 * the `i++` and passed as the argument does not move it (gcc propagates the
 * constant back to the call).
 *
 * This is the THIRD function parked on one dependency-free insn placed a slot
 * differently -- see func_8005C4D0 and the earlier func_8005B070 note, now
 * landed. They are worth one focused `tools/decomp/sched.py` session rather
 * than more source permutations.
 */

#include "common.h"

extern u8 D_80085014[];
extern s32 D_80073734s __asm__("D_80073734");

void func_80056A08(void);
void func_80056DB4(s32);
void func_8005E97C(s32, s32);

void func_80056C30(void)
{
    u8 *b;
    s32 i;
    u32 m;
    u32 v;

    func_80056A08();
    i = 0;
    m = 0;
    if (D_80073734s > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            *(s16 *)e = i;
            *(s16 *)(e + 0x1A) = 0;
            *(s16 *)(e + 0x18) = 0x40;
            *(u32 *)(e + 0x70) = 0x40;
            *(u32 *)(e + 0x74) = 0x40;
            v = D_80073740[i];
            func_80056DB4(i);
            i++;
            func_8005E97C(0, v);
            m |= v;
        } while (i < D_80073734s);
    }
    b = D_80085014;
    *(u32 *)(b + 0xC) = 0x7F;
    *(u32 *)(b + 4) = 0x40;
    *(u32 *)(b + 0x18) = 0;
    *(u32 *)(b + 0x1C) = 0x40;
    *(u32 *)(b + 0x14) = 0x7F;
    *(u32 *)(b + 0x20) = 0x7F;
    *(u32 *)(b + 0x24) = 0x7F;
    *(u32 *)(b + 8) = 0;
    *(u32 *)(b + 0x94) = 0;
    *(s16 *)(b + 0x50) = 0;
    func_8005E97C(0, m);
}
