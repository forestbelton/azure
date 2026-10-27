/* func_8005C4D0 (unk_2CDC0) -- 64 of 64 words, 22 DIFF. NOT compiled.
 *
 * Tears down the D_80085458 entry for index n: clears its +0x1A and +0x0A,
 * runs func_80056D44/func_80056DB4, then spins on func_8005EB78 until the
 * sound handle reports 2 or 0. Returns 0 on success, -1 if the entry was
 * already free or its +0x06 index is out of range.
 *
 * WHAT IS PAID FOR -- 62 words on the first draft, 64 now
 *
 * * BOTH tails are out of line, and the FAIL tail comes first. The loop's two
 *   exits jump forward past it to the success tail, so the shape needs labels
 *   and gotos: `if (h == 0) goto fail; if (... < 0x10) goto fail; ... body ...
 *   goto ok; fail: ... return -1; ok: ... return 0;`. As
 *   `if (A && B) { body; return 0; } ... return -1;` the success tail is in
 *   line and the fail tail last -- the reverse of the listing, and 6 words'
 *   worth of displacement on the loop's `beq`.
 *
 * * `h = *(u16 *) (e + 0x1A);` is read into a local BEFORE
 *   `D_80085F98[0] = 1`. After the store, the load cannot be hoisted across
 *   it -- a store to a global and a load through a pointer are a real
 *   dependence -- so the store cannot reach the `beqz`'s delay slot and a nop
 *   stays. This is advise.py's DELAY-NOPS advice applied literally, and it is
 *   two words.
 *
 * * BOTH tables take a pointer local for the base (`u8 *b = D_80085458;`,
 *   `u32 *g = D_80073740;`). Written as `&D_80073740[n]` fold puts the symbol
 *   after the index and the `addiu %lo` lands past the `sll`.
 *
 * * `func_80056D44` takes THREE arguments here, `(n, e, e)`. Its own listing
 *   never reads $a1 or $a2, so two are ignored -- but the two-argument and
 *   one-argument forms both come out 63 words, because the third argument is
 *   what makes gcc keep `e` in a register and copy it, which is the
 *   `addu $a1, $a2, $zero` the listing has before the two `sh`s.
 *
 * WHAT IS LEFT: 22 words, and `advise.py --greg` traces them to ONE conflict.
 *
 * `e` is pseudo 86: 4 refs, live 16, no call crossed, and its conflict set is
 * $v0 $v1 $a0 $s0 $sp -- $a1 and $a2 both free, so global-alloc takes the
 * lower-numbered $a1. The target has it in $a2, which means $a1 must be
 * occupied there, and the listing says by what: `lui $a1, %hi(D_80085F98)` is
 * the FIRST insn of the function while its `sw $v0, %lo(D_80085F98)($a1)` is
 * in the `beqz`'s delay slot twenty insns later. That long-lived %hi pseudo is
 * what conflicts with `e` and pushes it to $a2; `n`'s sign-extended copy and
 * the rest follow.
 *
 * So the question is not `e`'s allocation, it is why the %hi half of the
 * D_80085F98 store is hoisted to the top of the function here and not there.
 * The array spelling is right -- the `__asm__` scalar alias gives 63 words and
 * 63 DIFF -- so it is not a declaration question. Measured and not moving it:
 * a second pointer local for the two `sh`s and func_80056D44's second
 * argument, which gcc coalesces back into `e`.
 *
 * Both of the other functions parked on a one-slot scheduling difference have
 * since landed -- func_8005B070 on the struct stack buffer, func_80056C30 on
 * moving `i++` after the call -- so this one's remaining question is
 * allocation, not scheduling, and `--greg` rather than `sched.py` is the tool
 * for it.
 */

#include "common.h"

void func_80056D44(s16, u8 *, u8 *);
void func_80056DB4(s16);
void func_8005E97C(s32, s32);
s32 func_8005EB78(u32);

s32 func_8005C4D0(s16 n)
{
    u8 *b = D_80085458;
    u32 *g = D_80073740;
    u8 *e;
    u32 *t;
    u32 v;
    s32 r;
    u16 h;

    e = b + n * 0x78;
    h = *(u16 *)(e + 0x1A);
    D_80085F98[0] = 1;
    if (h == 0)
    {
        goto fail;
    }
    if (*(u16 *)(e + 6) < 0x10)
    {
        goto fail;
    }
    t = &g[n];
    v = *t;
    *(u16 *)(e + 0x1A) = 0;
    *(u16 *)(e + 0xA) = 0;
    func_80056D44(n, e, e);
    func_80056DB4(n);
    func_8005E97C(0, *t);
    do
    {
        func_8005E97C(0, v);
        r = func_8005EB78(v);
        if (r == 2)
        {
            goto ok;
        }
    } while (r != 0);
    goto ok;
fail:
    D_80085F98[0] = 0;
    return -1;
ok:
    D_80085F98[0] = 0;
    return 0;
}
