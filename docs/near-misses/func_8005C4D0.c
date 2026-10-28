/* func_8005C4D0 (unk_2CDC0) -- 64 of 64 words, 15 DIFF. NOT compiled.
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
 * * THE D_80085458 BASE takes a pointer local (`u8 *b = D_80085458;`); the
 *   D_80073740 one MUST NOT -- see the 22 -> 18 note at the end of this file.
 *   What this bullet used to say, that both want a local because
 *   `&D_80073740[n]` puts the symbol after the index and lands the
 *   `addiu %lo` past the `sll`, is true about the instruction order and wrong
 *   about the cost.
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
 *
 * 2026-10-27: WHAT THE FIRST DIVERGENCE ACTUALLY IS -- the ORDER OF THE TWO
 * SYMBOL MATERIALISATIONS, not a register on its own. From word 3:
 *
 *     target  lui $a1,%hi(D_80085F98) ; lui $v1,%hi(D_80085458) ;
 *             addiu $v1,$v1,%lo ; sll ; sw $s2 ; sra ; ... ; addu $a2,$v0,$v1
 *     built   lui $v1,%hi(D_80085458) ; addiu $v1,$v1,%lo ; sll ; sw $s2 ;
 *             sra ; ... ; addu $a1,$v0,$v1 ; lui $v1,%hi(D_80085F98)
 *
 * The listing puts the FLAG's high in $a1 at the very top, before the
 * D_80085458 base's pair, and keeps it until the `sw $v0,%lo(D_80085F98)($a1)`
 * sixteen words later. This build materialises the base pair first and the
 * flag's high afterwards -- and the element pointer's register follows from
 * that ($a2 in the listing, $a1 here), which is where most of the 22 words
 * come from. So the thing to move is which symbol is materialised first.
 *
 * Measured and NOT working:
 *   * a POINTER LOCAL for D_80085F98 (`u32 *f = D_80085F98;`), declared
 *     before `b` or after `g`: 46 and 48 DIFF. It shares the address but as a
 *     `lui`+`addiu` pair, where the listing shares only the HIGH and puts
 *     `%lo` in each store's displacement -- so a pointer local is the wrong
 *     construct here even though it is the standing recipe elsewhere.
 *   * `D_80085F98[0] = 1;` moved to the top of the function, or between
 *     `e = ...` and the `h` load: 66 words both times (two too many), 63
 *     DIFF. Its position in this file is load-bearing.
 *
 * `--greg` for the record: six global allocnos, 89 86 88 91 112 84 in that
 * order, and only two carry preferences -- 86 wants $a1 and 89 wants $v0.
 * Nothing prefers $a2, so the listing's element pointer there is a
 * lowest-free-register outcome downstream of the materialisation order, not a
 * preference to be engineered.
 *
 * 2026-10-28: 22 -> 18 -> 15, AND IT IS THE LEVER THIS FILE WAS ALREADY ASKING
 * FOR. The second step came from re-running the sweep on the 18-DIFF base:
 * dropping the `u32 *t` cursor too and reading the table twice,
 * `v = D_80073740[n];` and `func_8005E97C(0, D_80073740[n]);`, is 64 of 64
 * words and 15 DIFF. Same semantics -- `*t` was two reads of that location as
 * well -- and one less local. SWEEPS COMPOSE: each adopted hit changes the
 * base, so re-run rather than assuming the first pass exhausted it.
 *
 * Dropping the `u32 *g = D_80073740;` local and writing the one use as
 * `t = &D_80073740[n];` is 64 of 64 words and 18 DIFF. Found by a mechanical
 * sweep that deletes each park's locals one at a time, which is worth saying
 * because this file had reasoned its way PAST the shape: the bullet above
 * recorded that `&D_80073740[n]` moves the symbol after the index, which it
 * does, and concluded the local was wanted, which it is not.
 *
 * The mechanism is the one the analysis above already names. `e` goes to $a1
 * here and $a2 in the listing because nothing occupies $a1, and what occupies
 * it in the listing is the long-lived `lui $a1, %hi(D_80085F98)` emitted at
 * the top. Removing `g`'s local takes one materialisation out of the entry
 * block's ready list and moves the contest, which is why four words come back
 * without the statement order changing at all.
 *
 * STILL OPEN, and unchanged in kind: the FIRST divergence is still which
 * symbol is materialised first -- the listing's `lui %hi(D_80085F98)` ahead of
 * D_80085458's pair, this build's the other way round. Re-measured on top of
 * the new 18-DIFF base, both ways of moving the flag store up are worse than
 * they were on the old base:
 *
 *     D_80085F98[0] = 1; first                       65 words, 61 DIFF
 *     the same, between `e = ...` and the `h` load   65 words, 61 DIFF
 *
 * -- a word over, because the store then no longer reaches the `beqz`'s delay
 * slot and a nop stays. Inlining the OTHER base as well (`b`) changes nothing,
 * 64 words and the same 18, so `b`'s local is free either way and the
 * asymmetry between the two tables is real rather than a spelling accident.
 *
 * 2026-10-28, AND THE 15 IS SWEPT OUT. A full `search.py` pass on this new
 * base -- 40 candidates across all sixteen families, 41 variants compiled --
 * improves on nothing. That is worth recording because the pass was WORTH
 * re-running: the file's earlier sweeps were against the 22-DIFF source, and
 * two locals have gone since, so none of those results carried over. It did
 * not pay this time, but "sweeps compose" cuts both ways and the re-run is
 * what establishes that 15 is the floor for local rewrites here.
 *
 * `unname.py` is likewise exhausted on this base (zero hits), so what remains
 * is the materialisation ORDER described above -- the listing's
 * `lui %hi(D_80085F98)` ahead of D_80085458's pair -- and that is a sched1
 * LUID question, not a source-shape one.
 */

#include "common.h"

void func_80056D44(s16, u8 *, u8 *);
void func_80056DB4(s16);
void func_8005E97C(s32, s32);
s32 func_8005EB78(u32);

s32 func_8005C4D0(s16 n)
{
    u8 *b = D_80085458;
    u8 *e;
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
    v = D_80073740[n];
    *(u16 *)(e + 0x1A) = 0;
    *(u16 *)(e + 0xA) = 0;
    func_80056D44(n, e, e);
    func_80056DB4(n);
    func_8005E97C(0, D_80073740[n]);
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
