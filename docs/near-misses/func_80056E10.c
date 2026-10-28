/* func_80056E10 (unk_2879C) -- 718 of 718 words, 131 DIFF. COMPILES.
 *
 * SECOND ATTEMPT, 2026-10-28: 713/687 -> 718/131, word count exact. The
 * instruction stream now aligns with the listing everywhere (an alignment
 * that ignores register names leaves 21 words; one that keeps them, 48), and
 * every remaining difference is a register name or an in-block scheduling
 * order, listed under WHAT IS LEFT. The logic reading in the first attempt's
 * header still holds (seven fallback scans, portamento block between them);
 * what changed is the TYPES and the LOOP FORMS, and each change below names
 * the listing instruction that demanded it.
 *
 * CHANGES, each with its evidence:
 *
 * * `u8 key`, not `s32`. Every use is masked: `andi $fp,$s5,0xFF`,
 *   `andi $t0,$s5,0xFF`, `andi $a1,$s5,0xFF`, `sb $s5,0x5C($s3)`,
 *   `andi $v0,$s5,0x7F`. The old `(u8) key` casts said the same thing.
 *
 * * `u16 prog`, read as `*(u16 *) s`: the listing's `lhu $t0,0x0($s3)` is a
 *   halfword read of +0, where structs.h has `u32 unk_00` (func_8005845C
 *   stores a word there -- a different original TU). One `lhu`, then the
 *   spill/reload pair `sh 0x48 / lhu 0x48`, which `s16 prog` could not give
 *   (it emitted `lh` for the test and `lhu` for the spill).
 *
 * * `u16 j`: `lhu 0x38($sp)` throughout and `sltu` against `q[0]`. `s16 j`
 *   gave `sll 16 / sra 16 / slt`.
 *
 * * `u16 note` -- the register `$fp`. It is `key` at entry, `s->unk_5C & 0x7F`
 *   in the portamento head, `key` again in its equal arm, and feeds
 *   `v+0x0A/+0x0C = note & 0x7F` and `attr.note = note << 8`. The width is
 *   decided by the head's TWO andis, `andi $v0,$v0,0x7F; andi $fp,$v0,0xFF`:
 *   the AND is narrowed to QImode by the conversion and then extended to the
 *   variable's width. `s32 note` folds that to one `andi` (combine's
 *   zero_extend-of-and), `u8 note` is a plain move; only `u16`/`s16` give
 *   both. `u16` and `s16` measure identically.
 *
 * * `u32 k = key;` and `u16 chh = ch;` as the first statements of the layer
 *   loop body. They are the slots 0x60 (`andi $t0,$s5,0xFF; sw 0x60`) and
 *   0x64 (`lbu $t0,0x30; sh 0x64`), set once in the loop PREHEADER -- so they
 *   are loop invariants hoisted by loop.c, which is why they sit after the
 *   `beqz` and are not cse'd with the entry block's `andi $fp,$s5,0xFF`
 *   (`note = key`). Written before the loop they share that andi. `k` is the
 *   operand of every unsigned compare (`sltu $v0,$t0,$v0` range test, the
 *   portamento `sltu`/`beq`); `chh` of the scan-1/6 `beq $a1,$v0` and the
 *   +0x06 store. The portamento's up/down arms and scan 6's second test use
 *   `key` itself (`andi $v0,$s5,0xFF` recomputed).
 *
 * * THE SCANS ARE `do { if (hit) goto found; i++; } while (i < bound); i = -1;
 *   found:` -- real loops, not the first attempt's goto loops. The listing's
 *   preheaders hold loop.c hoists (`lw $a0,%lo(D_80073734)($v0)` and
 *   `lhu $a1,0x64($sp)` before scan 1, `lui $s0` before scan 2, `addiu $s0,
 *   $zero,2` before scan 3), which a goto loop never gets. stmt.c's
 *   expand_end_loop rotates a loop only when it finds a jump to the loop's
 *   own exit label in the first 30 insns; `goto found` targets a label
 *   OUTSIDE the loop, so there is no rotation and jump.c's
 *   duplicate_loop_exit_test (which needs the rotated form) never fires. That
 *   is the one test at the top with the back edge below it. `for (;;) { if
 *   (hit) break; ... }` DOES rotate (the `break` is a jump to the exit label)
 *   and duplicates.
 *
 * * THE BOUND D_80073734 IS READ THROUGH THREE SPELLINGS. Scans 1, 6, 7 use
 *   the incomplete-array alias `D_80073734a[0]`; scans 2 and 4 use structs.h's
 *   own `(s32) D_80073734[0]`; scans 3 and 5 use the scalar alias
 *   `D_80073734s`. Each is forced by the listing:
 *     - 3/5 have `lui $v1 / lw $v1,%lo(..)($v1)` INSIDE the loop while the
 *       constant 2 is hoisted into $s0. The array form is a compiler-split
 *       `(high)`/`(lo_sum)` pair that loop.c would hoist too; move_movables
 *       takes 3 off its threshold per move, a second hoist in a 20-insn loop
 *       is marginal, and the scalar form has no separate `high` to hoist.
 *     - 1/6/7 have `lui $v0 ... lw $a0,%lo(..)($v0)` in the PREHEADER (the
 *       load hoisted, no call in the loop) and 2/4 have `lui $s0` in the
 *       preheader with the `lw` inside -- five SEPARATE pseudos. With one
 *       array spelling for all five, loop.c's combine_movables merges the
 *       five `(high D_80073734)` movables in the LAYER-loop pass into one
 *       pseudo (savings 7, life 180: `.loop` says "matches"), which then
 *       loses allocation and is rematerialised through the spill register
 *       (`lui $t0 ... lw $a0,0x0($t0)` in 1/6/7, `lui $t0; lw` inside 2/4):
 *       165 DIFF. combine_movables compares the symbol_ref STRINGS, and the
 *       `__asm__` alias's symbol is "*D_80073734" against the plain
 *       declaration's "D_80073734", so two groups do not merge and neither
 *       group reaches the threshold alone: 133 DIFF, and every scan
 *       preheader now matches register for register. Which group takes
 *       which spelling does not matter (both ways measure the same).
 *   Three spellings of one symbol in one function is not something a
 *   programmer writes; the original most likely reached the bound through
 *   different macros or inline helpers for the three scan kinds (match by
 *   field, `== 0`, `== 2 || == 0`).
 *
 * * `st = func_8005EB78(..); if (st == 2) goto; if (st == 0) goto;` on its own
 *   variable. With the count-loop `c` reused, the return value was copied
 *   (`move $a1,$v0`) before `beq`; the listing tests `$v0` directly.
 *
 * * `u8 *p = D_80085458 + i * 0x78;` as a block-local inside each scan body
 *   and inside the found1 block. Gives the displacement form `lhu $v0,0x6($v1)`
 *   (the index expression folds +6 onto the symbol) and keeps the pointer out
 *   of `v`'s callee-saved register.
 *
 * * `func_80056DB4(i)` takes ONE argument here (`jal` with only `$a0` set),
 *   and `func_8005EC0C(&attr)` ONE (no `$a1`/`$a2` setup) -- declared through
 *   an `__asm__` alias so the siblings' three-argument prototype is untouched.
 *   `func_800565D8` returns `s32`: the listing uses `$v0` without an
 *   extension. `func_80055E74(i, key, ch, attr.volume.left, .right)` is five
 *   `s32`.
 *
 * * `D_80084918` through the typed alias `SpuVoiceAttr D_80084918s`: the
 *   first attempt's byte-array casts folded +0x24 onto the symbol and
 *   addressed everything with negative displacements. Typed, the +0 store is
 *   `sw %lo(D_80084918)($v1)` off the `high` register and the rest are
 *   displacements off the `addiu`'d pointer, as the listing.
 *
 * * THE PORTAMENTO BLOCK IS WRITTEN TWICE, in both arms of the `unk_98` test,
 *   with `i = -1;` ABOVE the test (the listing has it in the `beqz`'s delay
 *   slot, filled from before the branch; the then-arm starts with `i = 0`).
 *   jump.c's cross-jumping merges the two copies' arms and leaves the two
 *   heads, which is .L80057100 and .L80057144. The first attempt's
 *   measurement of this (729 words) was with `s32 note` and `key` as the
 *   compare operand; the equal arms then differed (`sw $s8` against
 *   `sw $v1`, cse having substituted the compared value) and did not merge.
 *   With `k` as the operand and `note` 16-bit they are identical.
 *
 * * `b2 = D_80086A40[D_80086D50].unk_04; b = b2;` -- two slots, 0x54 holding
 *   the loaded value (used once, by the `w` line's `lhu 0x12`) and 0x50 the
 *   copy (count loop, `q`, `e`), as the sibling func_8005B4D0 also has.
 *
 * * `s32 dummy[6];` -- 24 bytes of frame the listing never touches. The
 *   locals start at 0x30 and the outgoing-argument area is 0x18, and reload's
 *   spill slots are handed out in pseudo order with the parameters first, so
 *   the gap below `ch`'s slot can only be an expand-time allocation: an
 *   aggregate local the original declared and never used (or used in code
 *   that folded away). Its size is known, its contents are not.
 *
 * * Declaration order `j, n, prog, b, b2, q, flag, k, chh` -- the listing's
 *   slot order 0x38, 0x40, 0x48, 0x50, 0x54, 0x58, 0x5C, 0x60, 0x64 (halfword
 *   slots rounded to 8 because they have paradoxical SImode references). The
 *   frame is now 0x90 and every stack displacement agrees.
 *
 * * Record-fill order: loads from `e` and `s` cannot pass stores through `v`
 *   (different base registers conflict in sched's memrefs_conflict_p), so
 *   the LOAD order is the source order. `v[0x20] = e[0xD]` moves up next to
 *   `e[0xC]` (its `lbu $v1,0xD($s4)` precedes `sh $s2,0x0($s1)`, its store
 *   is delayed), `v[0x17] = e[3]` precedes the three zero bytes, `v[0x24]`
 *   precedes the zero words, `v[0x40] = unk_78` precedes +0x48/+0x58, and
 *   after the call `v[0x1D] = unk_18` precedes the +0x0A store and `+0x74 =
 *   unk_1C` precedes `+0x70 = -1`.
 *
 * * `q = b + (prog * 0x10 + 0x20)`, `flag = 0` before the range test (the
 *   listing's `sw $zero,0x5C` is in the first `bnez`'s delay slot, i.e. from
 *   before the branch), `j = 0` before the `q[0]` test (its `sh $zero,0x38` is
 *   in that `beqz`'s delay slot), `sum += *w; c++; w++;`, and `pan = (pan &
 *   0x7F) + (x << 8)` assigned back into `pan` so the 0x40 arm stores `$s0`
 *   itself (`sh $s0,0x16($s6)` in the `j`'s delay slot) and the two arms'
 *   stores are not cross-jumped; `x = (s16) pan >> 7; pan &= 0x7F; pan +=
 *   x << 8;` so the and is in place (`andi $s0,$s0,0x7F`) after the shift
 *   rather than into a temporary before it (2 words).
 *
 * MEASURED AND NOT ADOPTED:
 *   - the layer loop as a goto loop with `k`, `chh`, `g = D_80073740`,
 *     `vb = D_80085458` explicit: 718/489 at best. It gets the per-scan
 *     `lui` highs right but hoists the D_80085458 pair into each scan's
 *     preheader (allocated) where the listing rematerialises `lui $t0 /
 *     addiu $t0` at every use, and an explicit `vb` wins a register and
 *     pushes `note` out of $fp.
 *   - `volatile` on `ch`/`vel` (+2..+5 words), `&vel` to make it
 *     memory-resident (+1 word, slot order flips): both aimed at the record
 *     fill's `lbu $t0,0x31($sp)` staying below `sb $v0,0x15($s1)`.
 *   - `volatile` D_80086D50 for the prologue `lh` placement: +7 words.
 *   - `s32 note`, `u8 note`, `(u8)` cast, `u8 nk` intermediate: one andi.
 *   - `c = 0; sum = 0;` before the `w` line, a dedicated summing counter, a
 *     `for` summing loop, `b2` after `prog`, index-first `i * 0x78 + SYM`:
 *     all within noise or worse.
 *
 * WHAT IS LEFT (48 register-level words, 21 shape-level):
 *   1. The prologue's `lui/lh D_80086D50` is scheduled first here (word 3)
 *      and after `addu $s3` in the listing (word 24). Pure sched2 placement;
 *      nothing tried moves it.
 *   2. `c`/`sum` registers swapped ($a1/$a2) and, as a consequence, the attr
 *      pointer `addiu $a1` instead of `$a2`, and the `w` line's D_80086A40
 *      pointer in `$v1` with the D_80086D50 value in `$a0` instead of the
 *      reverse: ~30 words, all one allocation question. `.greg` allocates
 *      `c` (18 refs / 54 insns, priority 1.33) before `sum` (10 / 44, 0.68)
 *      so `c` takes $a1; the listing's order is the reverse. A separate
 *      count-loop counter, a dedicated summing counter, `c = 0` before or
 *      after `sum = 0`, and the `if (sum < n)` guard reference were all
 *      measured and none flips it. In the `w` line, `.greg` orders the
 *      D_80086A40 pointer before the D_80086D50 value because sched1 hoists
 *      the `lh` to the top of its block here, lengthening that pseudo's
 *      range -- the same placement fact as item 1.
 *   3. The summing loop's `c++` is scheduled before the `lhu` (equal
 *      priority, 3 each, with an r3000 load latency of 2) and the load delay
 *      takes a nop: 2 words. `c++; sum += *w; w++;`, `sum += *w++;`, a
 *      temporary for `*w`, a `for` loop and a local bound all leave it or
 *      lose a word.
 *   4. The record fill's `lbu $t0,0x31($sp)` (vel) is scheduled above
 *      `sb $v0,0x15($s1)` here and below it in the listing (3 words). sched1
 *      sees `vel` as a pseudo, so its store is ready and fills e[2]'s load
 *      delay; the listing's order is what a MEMORY-resident `vel` gives,
 *      where the load cannot pass the store (sp-based against a varying
 *      base in memrefs_conflict_p). `volatile` and `&vel` both make it
 *      memory-resident and both cost words elsewhere.
 *
 * The declarations it needs: `extern u8 D_80084930[];`, the two D_80073734
 * aliases, the SpuVoiceAttr alias, and the prototypes at the top.
 */
#include "common.h"

extern u8 D_80084930[];
extern s32 D_80073734a[] __asm__("D_80073734");
extern s32 D_80073734s __asm__("D_80073734");
extern SpuVoiceAttr D_80084918s __asm__("D_80084918");

void func_80056DB4(s32);
void func_8005E4A0(s32, u32);
s32 func_8005E78C(void);
void func_8005E97C(s32, u32);
s32 func_8005EB78(u32);
void func_8005EC0C1(SpuVoiceAttr *) __asm__("func_8005EC0C");
s32 func_800565D8(u8 *, u32);
void func_800561D8(u8 *, Unk80084960 *);
void func_80055E74(s32, s32, s32, s32, s32);

void func_80056E10(u8 ch, u8 key, u8 vel)
{
    s32 dummy[6];
    u16 j;
    s32 n;
    u16 prog;
    u8 *b;
    u8 *b2;
    u8 *q;
    s32 flag;
    u32 k;
    u16 chh;
    Unk80084960 *s;
    u8 *r;
    u8 *e;
    u8 *v;
    u16 *w;
    s32 sum;
    s32 c;
    s32 x;
    s32 pan;
    u16 note;
    s32 st;
    s16 i;
    u16 t;

    n = 0;
    s = &D_80084960[ch];
    b2 = D_80086A40[D_80086D50].unk_04;
    b = b2;
    prog = *(u16 *)s;
    c = 0;
    if (prog != 0)
    {
        r = b;
        do
        {
            if (r[0x20] != 0)
            {
                n = n + 1;
            }
            c = c + 1;
            r += 0x10;
        } while (c < prog);
    }
    q = b + (prog * 0x10 + 0x20);
    note = key;
    j = 0;
    if (q[0] == 0)
    {
        return;
    }
    k = key;
    chh = ch;
    do
    {
        e = b + ((n * 0x10 + j) * 0x20 + 0x820);
        flag = 0;
        if (k >= e[6] && e[7] >= k)
        {
            i = -1;
            if (s->unk_98 != 0)
            {
                i = 0;
                do
                {
                    u8 *p = D_80085458 + i * 0x78;
                    if (chh == *(u16 *)(p + 6))
                    {
                        goto found1;
                    }
                    i = i + 1;
                } while (i < D_80073734a[0]);
                i = -1;
                found1:
                if (i != -1)
                {
                    u8 *p = D_80085458 + i * 0x78;
                    if (*(u16 *)(p + 0x1A) == 0)
                    {
                        func_80056DB4(i);
                        func_8005E97C(0, D_80073740[i]);
                    }
                    else
                    {
                        flag = 1;
                    }
                }
                else
                {
                    i = 0;
                    do
                    {
                        if (func_8005EB78(D_80073740[i]) == 0)
                        {
                            goto found2;
                        }
                        i = i + 1;
                    } while (i < (s32)D_80073734[0]);
                    i = -1;
                    found2:
                    if (i == -1)
                    {
                        i = 0;
                        do
                        {
                            st = func_8005EB78(D_80073740[i]);
                            if (st == 2)
                            {
                                goto found3;
                            }
                            if (st == 0)
                            {
                                goto found3;
                            }
                            i = i + 1;
                        } while (i < D_80073734s);
                        i = -1;
                        found3:;
                    }
                }
                if (s->unk_50 != 0)
                {
                    note = s->unk_5C & 0x7F;
                    s->unk_52 = 0;
                    if (s->unk_5C < k)
                    {
                        s->unk_56 = (key - s->unk_5C) << 7;
                        s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                        s->unk_58 = 1;
                    }
                    else if (s->unk_5C == k)
                    {
                        s->unk_54 = 0;
                        note = key;
                    }
                    else
                    {
                        s->unk_56 = (s->unk_5C - key) << 7;
                        s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                        s->unk_58 = 0;
                    }
                }
                else
                {
                    s->unk_52 = 0;
                }
            }
            else
            {
                if (s->unk_50 != 0)
                {
                    note = s->unk_5C & 0x7F;
                    s->unk_52 = 0;
                    if (s->unk_5C < k)
                    {
                        s->unk_56 = (key - s->unk_5C) << 7;
                        s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                        s->unk_58 = 1;
                    }
                    else if (s->unk_5C == k)
                    {
                        s->unk_54 = 0;
                        note = key;
                    }
                    else
                    {
                        s->unk_56 = (s->unk_5C - key) << 7;
                        s->unk_54 = (s->unk_56 << 2) / s->unk_50;
                        s->unk_58 = 0;
                    }
                }
                else
                {
                    s->unk_52 = 0;
                }
            }
            if (i == -1)
            {
                i = 0;
                do
                {
                    if (func_8005EB78(D_80073740[i]) == 0)
                    {
                        goto found4;
                    }
                    i = i + 1;
                } while (i < (s32)D_80073734[0]);
                i = -1;
                found4:
                if (i == -1)
                {
                    i = 0;
                    do
                    {
                        st = func_8005EB78(D_80073740[i]);
                        if (st == 2)
                        {
                            goto found5;
                        }
                        if (st == 0)
                        {
                            goto found5;
                        }
                        i = i + 1;
                    } while (i < D_80073734s);
                    i = -1;
                    found5:
                    if (i == -1)
                    {
                        i = 0;
                        do
                        {
                            u8 *p = D_80085458 + i * 0x78;
                            if (chh == *(u16 *)(p + 6) && *(u16 *)(p + 0xA) == key)
                            {
                                goto found6;
                            }
                            i = i + 1;
                        } while (i < D_80073734a[0]);
                        i = -1;
                        found6:
                        if (i == -1)
                        {
                            i = 0;
                            do
                            {
                                u8 *p = D_80085458 + i * 0x78;
                                if (*(u16 *)(p + 0x1A) == 0)
                                {
                                    goto found7;
                                }
                                i = i + 1;
                            } while (i < D_80073734a[0]);
                            i = -1;
                            found7:
                            if (i == -1)
                            {
                                goto next;
                            }
                        }
                    }
                }
            }
            v = D_80085458 + i * 0x78;
            if (s->unk_98 == 0)
            {
                func_80056DB4(i);
                func_8005E97C(0, D_80073740[i]);
                func_80055E74(i, key, ch, D_80084918s.volume.left, D_80084918s.volume.right);
            }
            w = (u16 *)(D_80086A40[D_80086D50].unk_04 + (*(u16 *)(b2 + 0x12) << 9) + 0x820);
            sum = 0;
            c = 0;
            if (*(s16 *)(e + 0x16) > 0)
            {
                do
                {
                    sum += *w;
                    c++;
                    w++;
                } while (c < *(s16 *)(e + 0x16));
            }
            D_80084918s.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER |
            SPU_VOICE_NOTE | SPU_VOICE_SAMPLE_NOTE | SPU_VOICE_WDSA |
            SPU_VOICE_ADSR_AMODE | SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
            D_80084918s.volmode.left = 0;
            D_80084918s.volmode.right = 0;
            D_80084918s.voice = D_80073740[i];
            D_80084918s.addr = D_80086A40[D_80086D50].unk_10 + sum * 8;
            t = *(u16 *)(e + 0x10);
            D_80084918s.adsr1 = t;
            *(u32 *)(v + 0x60) = t;
            t = *(u16 *)(e + 0x12);
            D_80084918s.adsr2 = t;
            *(u32 *)(v + 0x64) = t;
            if ((*(u16 *)(e + 0x10) & 0x80) != 0)
            {
                D_80084918s.a_mode = SPU_VOICE_EXPIncN;
            }
            else
            {
                D_80084918s.a_mode = SPU_VOICE_LINEARIncN;
            }
            *(u32 *)(v + 0x68) = D_80084918s.a_mode;
            s->unk_5C = key;
            *(u32 *)((u8 *)s + 0x10) = vel;
            v[0x22] = e[4];
            v[0x23] = e[5];
            v[0x21] = e[0xC];
            v[0x20] = e[0xD];
            *(s16 *)(v + 0x00) = i;
            *(u16 *)(v + 0x04) = prog;
            *(s16 *)(v + 0x08) = j;
            *(s16 *)(v + 0x0A) = note & 0x7F;
            *(s16 *)(v + 0x0C) = note & 0x7F;
            *(s16 *)(v + 0x06) = chh;
            *(s16 *)(v + 0x1A) = 1;
            s->unk_3A = 0;
            v[0x14] = q[1];
            v[0x16] = q[4];
            v[0x15] = e[2];
            v[0x1C] = vel;
            v[0x17] = e[3];
            v[0x27] = 0;
            v[0x26] = 0;
            v[0x28] = 0;
            v[0x34] = s->unk_6C;
            v[0x36] = s->unk_6E;
            *(u32 *)(v + 0x38) = s->unk_70;
            *(u32 *)(v + 0x30) = s->unk_68;
            v[0x35] = s->unk_6D;
            v[0x24] = s->unk_5D;
            *(u32 *)(v + 0x2C) = 0;
            *(u32 *)(v + 0x3C) = 0;
            v[0x43] = 0;
            v[0x42] = 0;
            v[0x44] = 0;
            v[0x50] = s->unk_88;
            v[0x52] = s->unk_8A;
            *(u32 *)(v + 0x54) = s->unk_8C;
            *(u32 *)(v + 0x4C) = s->unk_84;
            v[0x51] = s->unk_89;
            v[0x40] = s->unk_78;
            *(u32 *)(v + 0x48) = 0;
            *(u32 *)(v + 0x58) = 0;
            func_800561D8(D_80085458 + i * 0x78, &D_80084960[ch]);
            D_80084918s.volume.left = *(u16 *)(v + 0x10);
            D_80084918s.volume.right = *(u16 *)(v + 0x12);
            v[0x1D] = s->unk_18;
            *(s16 *)(v + 0x0A) = key & 0x7F;
            *(u32 *)(v + 0x74) = s->unk_1C;
            *(u32 *)(v + 0x70) = -1;
            if (s->unk_28 < 0x40)
            {
                pan = *(u16 *)(v + 0x3C) + (s->unk_3A + s->unk_52);
                x = func_800565D8(v, s->unk_1C);
                pan = pan + ((*(u16 *)(v + 0x0C) << 7) + x);
                x = (s16)pan >> 7;
                pan &= 0x7F;
                pan += x << 8;
                D_80084918s.note = pan;
            }
            else
            {
                s->unk_1C = 0x40;
                D_80084918s.note = note << 8;
            }
            if (e[5] != 0)
            {
                *(s16 *)D_80084930 = ((e[4] - 1) << 8) | (0x7F - e[5]);
            }
            else
            {
                *(s16 *)D_80084930 = e[5] | (e[4] << 8);
            }
            if (flag == 0)
            {
                func_8005EC0C1(&D_80084918s);
            }
            if (s->unk_30 == 0)
            {
                if ((e[1] & 4) != 0)
                {
                    do
                    {
                        func_8005E4A0(1, D_80073740[i]);
                    } while ((func_8005E78C() & D_80073740[i]) == 0);
                }
                else
                {
                    do
                    {
                        func_8005E4A0(0, D_80073740[i]);
                    } while ((func_8005E78C() & D_80073740[i]) != 0);
                }
            }
            else if (s->unk_30 != 1)
            {
                do
                {
                    func_8005E4A0(1, D_80073740[i]);
                } while ((func_8005E78C() & D_80073740[i]) == 0);
            }
            else
            {
                do
                {
                    func_8005E4A0(0, D_80073740[i]);
                } while ((func_8005E78C() & D_80073740[i]) != 0);
            }
        }
        next:
        j = j + 1;
    } while (j < q[0]);
}
