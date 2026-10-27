/* func_800550E8 (unk_2879C) -- 119 of 120 words, 45 DIFF. NOT compiled.
 *
 * The turn-commit: validates the queued move at D_800847D0+0x26, charges its
 * cost through func_8005537C, looks the move up in two parallel tables indexed
 * by [+0x20][nibble of +0x26], starts its animation, clears the pending state,
 * and sets up the follow-up in D_800848F8 or D_800848FC depending on the top
 * nibble.
 *
 * WHAT IS PAID FOR (122 words on the first compile, now 119)
 *
 * * The three-way store to `D_800848F8 + 0x16` is a `switch` on the top
 *   nibble with `case 0x9000` written FIRST, then `case 0x8000`, then
 *   `default`. gcc emits a two-case switch as `beq <first value> -> its body`
 *   then `bne <second value> -> default`, falling through to the body written
 *   first, which is exactly the listing's `beq 0x8000` / `bne 0x9000` with the
 *   `= 4` arm falling through. As an if-else chain the opcodes come out
 *   `bne`/`bne` and the bodies in source order -- measured in four orderings,
 *   51, 51, 48 and 51 DIFF, against 45 for the switch.
 *
 * * `*(u32 *) (d + 4) = 1;` and `*(s16 *) (d + 8) = 0;` are hoisted ABOVE the
 *   three-way test, not repeated in each arm. Five words.
 *
 * * `func_8005537C` is called TWICE, once in each arm of the
 *   `func_8005B470` test, not once on a joined value. jump2 cross-joins the
 *   two `jal`s; a single call site lets reorg put the `0x64` into the test's
 *   delay slot and drops the `j` the listing has.
 *
 * * Both tables take a `u8 *` pointer local for the base, and ONE local
 *   serves both. Without it fold reassociates the symbol to the end of the
 *   index expression; with two separate locals the function comes out 118
 *   words and 66 DIFF.
 *
 * * `func_800552C8` is called with NO argument, through an `__asm__` alias, as
 *   in func_800553D4. $a0 holds a dead table value at that point and the
 *   listing sets up nothing.
 *
 * WHAT IS LEFT: one word and the registers.
 *
 * The missing word is at the very end: the target RE-MATERIALISES the
 * D_800847D0 base after `D_800847D0[0] |= 0x100`, so its `lui $s0` serves the
 * symbolic store and `addiu $s0, $s0, %lo(D_800847D0)` then rebuilds the base
 * in the call's delay slot. Assigning `b` again there does not do it -- gcc
 * deletes the redundant copy -- and nor does a second local, since cse equates
 * the two.
 *
 * The rest is allocation in the two lookups: the shared base local lands in
 * $a2 where the target uses $v1 and then $a0, and the first lookup's result
 * lands in $v0 and needs `addu $a0, $v0, $zero` where the target loads
 * straight into $a0. Both come from the same choice, so they are one fact.
 * `tools/decomp/sched.py` and section 8 of docs/matching-levers.md are the
 * next step.
 */

#include "common.h"

extern u8 D_80084810[];
extern u32 D_800848FC[];

s16 func_8005405C(s16);
s32 func_8005B470(s16, u8 *);
void func_8005537C(s16);
void func_800553D4(s32);
void func_8005B3D8(s32, s32);
s16 func_8005AE08(u32 *, s16);
void func_8005AE90(s16, u8, s16);
extern void func_800552C8b(void) __asm__("func_800552C8");

void func_800550E8(void)
{
    u8 *b = (u8 *)D_800847D0;
    u8 tmp[0x20];
    s32 n;
    s32 c;
    s32 v;
    u16 w;
    u16 m;
    u8 *t;

    if (*(s16 *)(b + 0x26) == -1)
    {
        func_800553D4(0x71);
        return;
    }
    if (func_8005405C(0) == 0)
    {
        *(u32 *)(b + 4) |= 2;
        return;
    }
    n = -((b[0x26] >> 4) * 2);
    if ((s16)func_8005B470(0, tmp) == 0)
    {
        func_8005537C(tmp[0x18] + n);
    }
    else
    {
        func_8005537C(0x64);
    }
    b = (u8 *)D_800847D0;
    w = *(u16 *)(b + 0x26);
    c = w & 0xF;
    *(s16 *)(b + 0x26) = c;
    t = D_80084810;
    v = *(s16 *)(t + (*(u16 *)(b + 0x20) << 5) + c * 2);
    m = w & 0xF000;
    if (v != 0)
    {
        func_8005B3D8(v, v);
    }
    t = D_80084878;
    *(s16 *)(b + 0x22) =
        func_8005AE08((u32 *)(t + (*(u16 *)(b + 0x20) << 6) + *(s16 *)(b + 0x26) * 4), 0);
    *(s16 *)(b + 0x26) = -1;
    D_800847D0[0] &= ~0x1000;
    *(u32 *)(b + 4) &= ~2;
    if (m != 0)
    {
        u8 *d = (u8 *)D_800848F8;

        *(u32 *)(d + 4) = 1;
        *(s16 *)(d + 8) = 0;
        switch (m)
        {
        case 0x9000:
            *(s16 *)(d + 0x16) = 4;
            break;
        case 0x8000:
            *(s16 *)(d + 0x16) = 2;
            break;
        default:
            *(s16 *)(d + 0x16) = 0xA;
            break;
        }
    }
    else
    {
        D_800848FC[0] = 3;
    }
    D_800847D0[0] |= 0x100;
    func_800552C8b();
    func_8005AE90(*(s16 *)(b + 0x22), 1, 1);
}
