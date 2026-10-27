/* func_8004951C (unk_1C540) -- 156 of 157 words, 97 DIFF. NOT compiled.
 *
 * Builds the 0x20-byte status label for one party member into a stack buffer
 * and hands it to func_8004E298: memset to spaces, NUL at the end, then one
 * arm per entry kind (D_80073414[p[1]].unk_10) and per entry id (p[1]), and a
 * shared `kind == 1` tail that adds two more glyphs from p[3]'s flags.
 *
 * include/structs.h gained `Unk80073414.unk_10` (split out of pad_10) for
 * this: the kind byte, read with `lbu` and compared against 4, 3 and 1.
 *
 * WHAT IS PAID FOR -- the whole structure, and four spellings
 *
 * * `b = D_80073414;` is ASSIGNED, not an initialiser. As `Unk80073414 *b =
 *   D_80073414;` the materialisation is evaluated at block entry, lands ahead
 *   of the memset call, and the pointer then lives across it in $s0 -- which
 *   also pushes the whole prologue around. Assigned just before its use, the
 *   base comes out in $v0 after the two calls, exactly as the listing has it,
 *   and the `addu $v1, $a0, $zero` copy of p[1] appears by itself.
 * * D_80082E6A goes through an `__asm__` INCOMPLETE-ARRAY alias
 *   (`D_80082E6Aa[0]`). As the scalar `extern u8 D_80082E6A` both reads get
 *   the assembler-expanded form, whose `lui` and `lbu` share one register and
 *   leave a load-delay nop; the array form gives the compiler-split pair the
 *   listing has, with the address in $v0 and the byte in $v1, and the
 *   comparison constant filling the delay.
 * * the `& 0x1F` value goes into an `s32` LOCAL. Spelled inline, combine
 *   reads `nonzero_bits` off the `andi` directly, proves the sign bit zero and
 *   emits `sltiu`; through a pseudo it emits the `slti` the listing has. `(s32)`
 *   casts, `<= 0x13` and nesting the `&&` all stay `sltiu` (measured).
 * * the `p[1] == 4` arm and the `kind == 1` arm both end with
 *   `func_8004E5A0(.., 2, &buf[0xB]); func_8004E69C(&buf[0xB]);` and
 *   cross-jumping merges the two call pairs into the one the listing has at
 *   .L8004970C -- which is why the 4 arm is written out in full rather than
 *   falling into the other.
 *
 * WHAT IS LEFT: 2 words, and the opcode histogram names them exactly --
 * built is short two `addu` copies and one nop and has one `sb` too many.
 * All three come from ONE flow difference:
 *
 * The listing's `kind == 3` arm reaches the final call WITHOUT passing the
 * shared `kind == 1` test: its two exits are `bnez $v0, .L80049764` (with
 * `addu $a0, $s3, $zero` in the delay slot) and `j .L8004975C`, and
 * .L8004975C is the `sb $v0, 0x10($sp)` SHARED with the tail's
 * `buf[0] = 0x79` -- cross-jumping merged those two stores. Here the arm
 * falls through into the `kind == 1` test, so the store cannot merge (one
 * extra `sb`), the test's constant gets hoisted into the arm's delay slot
 * instead of `addu $a0, $s3` (one missing `addu`), and the third `addu $a0,
 * $s3` reorg would duplicate into a delay slot never appears.
 *
 * gcc cannot thread that fall-through away: `thread_jumps` needs the last
 * comparison before the jump to dominate the test, and the arm has three
 * comparisons of its own in between. So the arm's exit must be explicit in
 * the source -- and it is not an obvious `goto`: `goto out;` at the end of
 * the arm gives 154, a full goto-and-label rewrite of the whole tail gives
 * 152, and nesting the chain and tail inside the `else` gives 154.
 *
 * The other half is the `kind == 1` arm's abs. The listing sets $a1, $a0, $s0
 * and $a2 -- every argument of the pair of calls -- BEFORE the `bgez`, and
 * leaves the `bgez` and the first `jal` with nop delay slots. Here the
 * argument setup is emitted at the call, after the abs, and reorg fills both
 * slots with it. `v < 0 ? -v : v` as the argument, the inline form of the
 * same, and two separate call sites (159 words) all leave it there.
 *
 * SINCE PARKED, 2026-10-27: 155 of 157 words and 108 DIFF to 156 of 157 and
 * 97, by declaring `k` `s16` rather than `s32`. Found by search.py's
 * declaration-width family in the sweep of all forty parks. The WHAT IS LEFT
 * note above is written against the old numbers.
 *
 * THE ONE WORD LEFT IS A CROSS-JUMP THIS SOURCE WINS AND THE LISTING DOES
 * NOT. Four arms end in `func_8004E5A0(x, n, &buf[m]); func_8004E69C(...)`,
 * and jump2 merges their tails into one `jal` in both builds. But two of them
 * -- the `p[1] == 4` arm and the `k == 1` arm -- pass the SAME `&buf[0xB]`,
 * so here the `addiu $s0, $sp, 0x1B` and `addu $a2, $s0` merge into the
 * shared site as well and the listing keeps a copy in each arm.
 *
 * The listing cannot merge them because its `k == 1` arm has the conditional
 * `negu $a0, $a0` BETWEEN that setup and the call, so the streams have
 * already diverged where cross-jumping walks back from the `jal`. Here the
 * negate happens earlier, into a local `v`, and the whole argument setup is
 * common tail. Measured and not working: the call written out in BOTH arms of
 * the sign test (`func_8004E5A0(-*(s8 *) (p + 2), ...)` in one and the plain
 * value in the other), either arm first -- 160 words, three too many, because
 * the second call and its argument then stop merging too. What is wanted is
 * the negate last and the rest merged, which no spelling has produced yet.
 */

#include "common.h"

u32 func_800492B0(u32);
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s32 func_8004951C(u8 *dst, s32 p)
{
    u8 buf[0x20];
    Unk80073414 *b;
    s16 k;

    memset(buf, 0x20, 0x20);
    buf[0x1F] = 0;
    if (D_80082E6Aa[0] == 2 && func_80042A80(p) != 0)
    {
        return 0;
    }
    b = D_80073414;
    k = b[*(u8 *)(p + 1)].unk_10;
    if (k >= 4)
    {
        return 0;
    }
    if (k == 3)
    {
        {
            s32 m = *(u8 *)(p + 3) & 0x1F;

            if (m < 0x14 && D_80082E6Aa[0] != 1 &&
                *(u8 *)(func_800492B0(m) + 0x28) == 0)
            {
                buf[0] = 0x7A;
            }
        }
    }
    else if (*(u8 *)(p + 1) == 0x12)
    {
        if (D_80082E6Aa[0] != 1)
        {
            func_8004E5A0(0x64 - *(s8 *)(p + 2), 3, &buf[9]);
            func_8004E69C(&buf[9]);
            buf[0xC] = 0xA;
            buf[0xD] = 9;
            buf[0xE] = 0x39;
            buf[0x12] = 0x25;
        }
    }
    else if (*(u8 *)(p + 1) == 4)
    {
        func_8004E5A0(*(s8 *)(p + 2), 2, &buf[0xB]);
        func_8004E69C(&buf[0xB]);
    }
    else if (*(u8 *)(p + 1) == 0xE)
    {
        func_8004E5A0(func_800438E4(p), 5, &buf[6]);
        func_8004E69C(&buf[6]);
        buf[0xB] = 0xA;
        buf[0xC] = 9;
        buf[0xD] = 0x39;
        buf[0x10] = 0x47;
    }
    else if (k == 1)
    {
        if (*(s8 *)(p + 2) != 0)
        {
            s32 v;

            if (*(s8 *)(p + 2) > 0)
            {
                buf[0xA] = 0x6B;
            }
            else
            {
                buf[0xA] = 0x6C;
            }
            v = *(s8 *)(p + 2);
            if (v < 0)
            {
                v = -v;
            }
            func_8004E5A0(v, 2, &buf[0xB]);
            func_8004E69C(&buf[0xB]);
        }
    }
    if (k == 1)
    {
        if ((*(u8 *)(p + 3) & 0x40) != 0)
        {
            buf[0xD] = 0xA;
            buf[0xE] = 0x7A;
        }
        if ((*(u8 *)(p + 3) & 0x20) != 0)
        {
            buf[0] = 0x79;
        }
    }
    func_8004E298(dst, buf, 0);
}
