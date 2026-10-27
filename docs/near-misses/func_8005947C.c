/* func_8005947C (unk_2879C) -- 81 of 81 words, 12 DIFF. NOT compiled.
 *
 * The sequencer's event dispatch: reads a byte, decides whether it is a
 * running-status continuation or a new status byte, looks the status up in
 * D_800737DC and either calls the three-argument handler or one of three
 * special cases. Worked from an m2c draft; the word count was right on the
 * first compile and every shape finding below was paid for after that.
 *
 * WHAT IS PAID FOR
 *
 * * TWO variables hold the byte: `r`, the full s32 the read returns (the
 *   target's $a1, used for the `& 0x80` test, the store to +0x4A and the copy
 *   into `i`), and `b`, a `u8` copy of it (the target's $a0, used for every
 *   `== 0xFF` / `== 0xF0` comparison and for `c`'s initial value). One
 *   variable gives one register and loses the `addu $a0, $a1, $zero` the
 *   target puts in the first `bnez`'s delay slot.
 *
 * * `i` is `u32`, NOT `u8`. As a `u8` it is masked to 0xFF before the shift,
 *   which makes `& 0xF` redundant and emits `srl 4` / `sll 2` where the target
 *   has the folded `srl 2` / `andi 0x3C`.
 *
 * * `i = r` is the LAST statement of the status-byte arm. Written first, cse
 *   uses `i`'s register for the +0x4A store (`sb $s1` where the target has
 *   `sb $a1`) and the `beq`'s delay slot goes empty: 82 words.
 *
 * * The three special cases are a `switch` on `b`, not an if-chain. The chain
 *   gets the same 81 words but lays the three bodies out in source order,
 *   where the switch puts the 0xFF body first and the 0xF0 body after it,
 *   which is the order the listing has. 14 DIFF as a nested if-else with the
 *   arms already permuted to match; 12 as the switch.
 *
 * WHAT IS LEFT: TWO COUPLED ALLOCATION FACTS
 *
 * 1. `b` is in $a2 here and $a0 in the target. Six of the twelve words are
 *    that, including its knock-on: with `b` in its own register the tail's
 *    mask is `andi $v1, $a2, 0xFF`, which reorg hoists into the `beqz $s3`
 *    delay slot, while the target's is the IN-PLACE `andi $a0, $a0, 0xFF` --
 *    which cannot be hoisted, because $a0 is live down the other arm -- and
 *    the slot goes to `addiu $v0, $zero, 0xF0` instead. So this is one fact,
 *    not two: get `b` into $a0 and the mask, the slot and the two comparisons
 *    all follow. `b` crosses no call on either path, so every caller-saved
 *    register is legal for it and the choice is global-alloc's.
 *
 * 2. `t` and `c` hold each other's callee-saved registers: $s2/$s3 here,
 *    $s3/$s2 in the target. Both have three references and live ranges within
 *    an insn or two of each other, so they are a near-tie in global-alloc's
 *    priority and the tie breaks on allocno number, i.e. which appears first
 *    in the RTL. `t` does, which is why it takes $s2 here; for the target to
 *    have given $s2 to `c`, `c` must have outranked it -- four references
 *    rather than three would do it (floor_log2(4)*4 = 8 against
 *    floor_log2(3)*3 = 3), which would mean the original reads `c` once more
 *    than this does.
 *
 * Measured and NOT working: `u32 t`; `t` declared before `c`; `b` declared
 * before `r`; `c = r` instead of `c = b` (15 DIFF); `d = 0` after the read
 * rather than before (19); the parameter as `s32` with a `u8 *` local (82);
 * `s32 b` with `(b & 0xFF)` at every comparison and `b &= 0xFF` in the tail
 * (80 words -- the in-place mask DOES appear, and costs the +0x4A arm's
 * `andi`); and the whole tail nested inside `if (t == 0)` instead of after an
 * early return (37).
 */

#include "common.h"

extern u32 D_800737DC[];

u32 func_800589B8(s32);
void func_8005914C(s32, u8, u8, u8);
void func_80058E64(void);
void func_80058E6C(s32, u8);
void func_800590E0(s32);

s32 func_8005947C(u8 *p) {
    s32 r;
    u8 b;
    u32 i;
    u8 c;
    u8 d;
    s32 t;

    d = 0;
    r = func_800589B8((s32) p);
    b = r;
    if ((r & 0x80) == 0) {
        i = p[0x4A];
        p[0x49] = 1;
    } else {
        if (b != 0xFF) {
            p[0x4A] = r;
        }
        i = r;
        p[0x49] = 0;
    }
    t = D_800737DC[(i >> 4) & 0xF];
    if (t != 0) {
        c = b;
        if (p[0x49] == 0) {
            c = func_800589B8((s32) p);
        }
        if (t == 2) {
            d = func_800589B8((s32) p);
        }
        func_8005914C((s32) p, i, c, d);
        return 0;
    }
    switch (b) {
    case 0xFF:
        func_80058E6C((s32) p, func_800589B8((s32) p));
        break;
    case 0xF0:
        func_800590E0((s32) p);
        break;
    default:
        func_80058E64();
        break;
    }
    return 0;
}
