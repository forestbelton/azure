/* func_8005BBFC (unk_2CDC0) -- 333 of 333 words, 219 DIFF. NOT compiled.
 *
 * SPU voice start: resolves the program record for (bank, prog, key), waits
 * for the hardware voice to go idle, sums the sample-table lengths to get the
 * wave address, fills a SpuVoiceAttr on the stack and the 0x78-byte voice
 * record at D_80085458, and hands both to func_8005EC0C. Returns the channel,
 * or -1 if the channel is out of range or the key is outside the record's
 * [+0x06, +0x07] range.
 *
 * THE WORD COUNT IS EXACT and the logic is settled; what is left is register
 * allocation and two scheduling choices. advise.py reads 90 register-only
 * differences (87 with the store order below), and the positional DIFF is
 * mostly those shifting the comparison.
 *
 * WHAT IS PAID FOR
 *
 * * The channel is an `s16` PARAMETER copied into an `s32` LOCAL. The listing
 *   sign-extends $a0 once at entry (`sll 16` / `sra $s2`) and then treats it
 *   as 32 bits -- `ch = -1` and `if (ch != -1)` on the s16 parameter itself
 *   re-narrow at every step, which is 12 words. The return type stays `s16`,
 *   as csrc/unk_2CDC0.c already declares it.
 *
 * * `(s32) D_80073734[0] - 1 < ch` needs the cast: D_80073734 is `u32`, so
 *   without it the comparison is `sltu` where the listing has `slt`.
 *
 * * The pan split is `if (x >= 0x40) {...} else {...}` -- the 0x40-and-over
 *   arm FIRST, because the listing's `bnez` on `slti 0x40` jumps to the
 *   under-0x40 body, so that body is the out-of-line arm.
 *
 * * Both doublings in that split are `<< 1`, never `* 2`. With `* 2` fold
 *   moves the doubling onto the OTHER operand -- `sll $v0, $v0, 1` on
 *   `0x40 - (x & 0x3F)` instead of the listing's `sll $v1, $a2, 1` on the
 *   level. Same family as the `associate:` reassociation in
 *   docs/matching-levers.md.
 *
 * * The two adsr halfwords are read into a LOCAL and used twice:
 *   `t = *(u16 *) (e + 0x10); attr.adsr1 = t; *(u32 *) (v + 0x60) = t;`. Read
 *   twice from the source, the store to `attr` invalidates cse's memory
 *   equivalence (a stack MEM against a pointer MEM) and each pair costs a
 *   second load plus its delay nop -- 3 words for the two. The THIRD read,
 *   for the `& 0x80` test, is a genuine reload in the listing too.
 *
 * * `attr.volume.left` is READ BACK from the voice record
 *   (`attr.volume.left = *(s16 *) (v + 0x10);`), not assigned from a local
 *   holding the product. cse forwards the read to the stored value as a
 *   COPY, and that copy is what fills the load-delay slot of the +0x12
 *   read-back two statements later. With a local the copy coalesces away and
 *   the delay takes a nop. +0x12's own read-back is NOT forwarded, because
 *   the `attr.volume.left` store sits between it and its store.
 *
 * * The voice record is reached through a `u8 *v` ASSIGNED ONCE PER BLOCK, so
 *   +0x60/+0x64 and +0x10/+0x12 stay load/store displacements. Written as
 *   `*(u32 *) (D_80085458 + ch * 0x78 + 0x60)` the offset folds into the
 *   address arithmetic and costs an `addiu` each. cse cannot share the three
 *   base computations because branches separate them, which is why the
 *   listing recomputes it three times.
 *
 * * The summing loop is `sum += *w; i++; w++;` -- the load first, so `i++`
 *   can fill its delay slot. With `i++` first sched hoists it above the load
 *   and the delay takes a nop.
 *
 * * The stack struct IS a `SpuVoiceAttr` and the mask IS the SPU_VOICE_*
 *   macros ored together (0x701EF = VOLL|VOLR|VOLMODEL|VOLMODER|NOTE|
 *   SAMPLE_NOTE|WDSA|ADSR_AMODE|LSAX|ADSR_ADSR1|ADSR_ADSR2), and a_mode is
 *   SPU_VOICE_EXPIncN / SPU_VOICE_LINEARIncN. Every displacement in the
 *   0x10..0x4E range agrees with that layout.
 *
 * WHAT IS LEFT
 *
 * 1. SOLVED, and kept here because it is the lever: `prog` and `key` are the
 *    two parameters that outlive the calls and only ONE callee-saved register
 *    is free for them. The listing spills `prog` to a halfword slot and keeps
 *    `key` in $fp, and WRITING THE TWO LATE STORES IN THE ORDER +0x08 THEN
 *    +0x04 is what produces that: the parameter whose store comes LAST is the
 *    one that gets the register. With +0x04 first it is the other way round
 *    and three more words differ. The .greg dump explains the margin -- the
 *    two allocnos are 2 refs each with live lengths 136 and 144, priorities
 *    0.015 and 0.014, so the store order's effect on the live range is the
 *    whole decision. Measured and not working for this: `prog` as `s16`,
 *    `key` as `s32` or `u16` (332 words), `prog` as `s32`, `(s16) prog * 0x10`
 *    hoisted into its own local, and computing `e` before `q`.
 *
 * 2. The two stack parameters are loaded in the listing's PROLOGUE (`lw $t0,
 *    0xA0($sp)` / `lw $t1, 0xA4($sp)` before anything else) and here they are
 *    loaded at their uses. All eight parameters as `s16` puts the loads in
 *    the right place but is 327 words, six short, because the `(s16)` casts
 *    then disappear from the four places the listing keeps them.
 *
 * 3. ONE MISSING `addu` AND ONE EXTRA NOP, which cancel in the word count.
 *    The listing copies the pan level into BOTH arms of the 0x40 split
 *    (`addu $a2, $a0, $zero` and `addu $a1, $a0, $zero`); this source gets
 *    the first copy and coalesces the second, because `a`'s only remaining
 *    use is that arm. Measured and not working: both copies hoisted above
 *    the branch (`l = a; r = a;` then two one-statement arms, 221 DIFF), and
 *    naming `a` rather than the arm's own variable in either doubling (both
 *    220, i.e. gcc folds the copy back either way). `a` cannot be declared
 *    narrower to make the copy survive -- the product reaches 18 bits. The compensating extra nop is the summing loop's load
 *    delay: sched1 hoists `i++` above the `lhu` whatever order the body is
 *    written in. `sum += *w; w++; i++;` and `sum += *w++; i++;` DO kill the
 *    nop -- and then the function is 332 words, one short, which is the same
 *    two defects seen separately. Worth knowing when the copy is found.
 *
 * 4. The order of the voice-record stores and of the attr fills: the listing
 *    interleaves them differently in the 0x8005BE50..0x8005BF00 stretch.
 *    Those stores are all to the same base, so sched moves them freely and
 *    the emitted order is not the source order -- which is why a permutation
 *    sweep over the 15 of them is 15! and perm.py can only sample it. The
 *    source order below is the listing's own emitted order with the +0x04 and
 *    +0x08 stores swapped per item 1.
 *
 * search.py's whole sweep (127 candidate changes, 254 variants, all eleven
 * transform families including the declaration-order, per-arm and volatile
 * ones) finds exactly one improvement worth 1 DIFF, and it is REJECTED here
 * on purpose: it drops the `(s16)` from the FIRST of the two key-range
 * comparisons and leaves it on the second. The listing uses the same
 * sign-extended $a2 for both (`slt $v0, $a2, $v0` then `slt $v0, $v0, $a2`,
 * off one `sll 16` / `sra 16` pair), so both casts belong, and a differing-word
 * count that improves when one is removed is the score rewarding a semantic
 * change rather than a shape change. That is the judgement search.py cannot
 * make; the score is an oracle for shape, not for correctness.
 *
 * So what is left really is allocation and schedule, not shape.
 *
 * MEASURED 2026-10-28, the narrow-parameter lever (section 7), since every use
 * of `vel` and `fine` in this source is `(s16)`-cast: `s16 vel` is 333 words
 * and 219 DIFF (one better), `s16 fine` is 220 (unchanged), and BOTH together
 * is 334 words and 221. Recorded rather than adopted -- at 220 differing words
 * a single word is noise, and the two-parameter form going a word OVER says
 * the pair interact. Worth re-running once this function is close enough for
 * the score to mean something.
 *
 * THE FIRST DIVERGENCE IS DIAGNOSED, 2026-10-28, and it is the same fact at
 * thirteen of advise.py's MISSING sites. The target's PROLOGUE loads the
 * stack-passed arguments --
 *
 *     lw  $t0, 0xA0($sp)     <- arg 5, `vel`
 *     lw  $t1, 0xA4($sp)     <- arg 6, `fine`
 *     lhu $t2, 0xA8($sp)     <- arg 7, `volx`
 *
 * -- where this build loads only `volx` and defers the other two to their
 * uses inside the `ch != -1` block. Word counts agree (333 both), so this is
 * POSITION, not content, and it shifts everything after it.
 *
 * WHY: `update_equiv_regs` (local-alloc.c:1078) sinks a parameter's load to
 * its use when
 *
 *     reg_n_refs[regno] == 2 && reg_basic_block[regno] < 0
 *
 * i.e. the pseudo is set once and used EXACTLY ONCE, in more than one block.
 * This source reads `(s16) vel` three times and `(s16) fine` once, but cse
 * shares the three casts into a SINGLE read of `vel`, so both parameters hit
 * `reg_n_refs == 2` and both loads sink. The listing's do not, so there the
 * parameters carry a second RTL reference or are confined to one block.
 *
 * MEASURED and NOT working: computing `pit` before the `ch` test, in three
 * positions (272 DIFF each -- the arithmetic moves with the loads, which is
 * worse than the loads being late); and reading `vel`/`fine` into locals at
 * the top, one or both (220, unchanged -- the copies are propagated away and
 * the parameters still hit the two-reference rule).
 *
 * THE LEVER IS A SECOND REFERENCE, AND `vel` NOW HAS ONE: the range test reads
 * the RAW parameter, `if (vel >= e[6] && e[7] >= vel)`, instead of `(s16) vel`
 * twice. cse cannot share a raw read with the casts, so `vel` reaches three
 * references, `update_equiv_regs` leaves it alone, and its load stays where
 * assign_parms put it -- `lw $t0, 0xA0($sp)` in the prologue, which now MATCHES
 * the listing word for word. 220 DIFF -> 219.
 *
 * THE SEMANTIC NOTE, because this is the same class of change as the `s16 b`
 * rejected in func_8005A90C's park: dropping the cast is faithful only if
 * `vel` is always within s16 range, since `(s16) vel` is still what feeds
 * `pit` and func_80056DB4. Unlike that case, the evidence here is FOR the
 * change -- the listing's prologue load appears only in this form -- but the
 * caller should still be checked. Making the call's arguments raw as well is
 * also 219, so that part is not evidence either way.
 *
 * `fine` IS STILL SUNK and is the next word: the listing loads it as
 * `lw $t1, 0xA4($sp)` right after `vel`, and this source has only ONE use of
 * it (in `pit`), so it sits exactly on the two-reference rule. There is no
 * second use of `fine` anywhere in this source to give it, which says the
 * original reads it twice -- the obvious candidate being that `pit` is
 * recomputed at its second use rather than carried in a variable. Check
 * whether the listing has two `sll 8`/`addu` pairs before spending a run on
 * it.
 *
 * AND THE TWO HOISTS WERE RE-MEASURED ON THIS NEW BASELINE, 2026-10-28, since
 * the earlier 272 predates the `vel` fix and the interaction could have
 * changed. It did not, and both are far worse than the 219:
 *
 *     pit computed in the ENTRY block           334 words, 278 DIFF
 *     only `fine` read into an entry-block local  332 words, 306 DIFF
 *
 * The second is the informative one: giving `fine` an entry-block home does
 * not keep its load in the prologue, it moves the whole `pit` chain forward
 * and costs a word. So one-block confinement is not the route either -- the
 * second reference is, and it has to be a reference the SOURCE can spell.
 *
 * 2026-10-28, THE 219 IS ONE INSTRUCTION, and the prologue diff says so
 * exactly. Side by side from word 7:
 *
 *     target  lw $t0,0xA0 ; lw $t1,0xA4 ; lhu $t2,0xA8 ; li 1 ; ...
 *     built   lw $t0,0xA0 ;              lhu $t1,0xA8 ; li 1 ; ...
 *
 * every later word shifts by that one missing `lw`, which is where 219
 * differing words out of 333 come from. So this park is ONE LOAD, not a
 * structural problem, and anything that puts it back is worth a large number.
 *
 * AND THE MECHANISM IS A LOST REGISTER, NOT A SUBSTITUTION. The built code has
 * a SEPARATE `lw $v0, 0xA4($sp)` at the use site (word 56), not a memory
 * operand folded into the shift -- and it could not fold one, because the use
 * is `sll`, and `(set (reg) (ashift (mem) 16))` is not a MIPS insn, so
 * `update_equiv_regs`' `validate_replace_rtx` would fail. What has happened is
 * that `fine`'s pseudo did not get a hard register at all and reload
 * rematerialised it from its stack home at the use. A pseudo with a REG_EQUIV
 * stack slot is cheap for reload to spill AND has its live length doubled
 * (local-alloc.c:1018-1064), so it loses the register contest -- which is the
 * same arithmetic the `vel` fix beat by adding a third reference.
 *
 * TWO NEW SHAPES, neither adopted:
 *
 *   * `fn = fine << 16;` in the ENTRY block with `(fn >> 16)` at the use --
 *     the `(s16)` cast split across the two blocks. 334 words, 252 DIFF. It
 *     DOES put the load in the prologue (`lw $a0, 0xA4($sp)` at word 8 against
 *     the target's word 7), so the mechanism is beatable; the cost is that the
 *     `sll` lands in the entry block too and the function runs a word over.
 *     The target's entry block holds the load ALONE, with its `sll $a0,$t1,16`
 *     forty instructions later.
 *   * `fn = fine;` in the entry block with `(s16) fn` at the use -- 333 words
 *     and the same 219, because cse propagates the copy straight back out.
 *
 * So the shape wanted gives `fine`'s pseudo a reference, or a reason to win a
 * register, WITHOUT putting any instruction in the entry block. `fn = (s16)
 * fine;` there is 332 words and 306 DIFF for the opposite reason: gcc folds
 * the cast into the load and emits `lh`, losing the sll/sra pair as well.
 */

#include "common.h"

void func_80056DB4(s16, s32, s16);
void func_8005E4A0(s32, u32);
void func_8005E97C(s32, u32);
s32 func_8005EB78(u32);
void func_8005EC0C(SpuVoiceAttr *, s32, s32);

s32 func_8005BBFC(s16 ch0, s16 bank, u16 prog, s16 key, s32 vel, s32 fine, u16 volx, u16 panx)
{
    SpuVoiceAttr attr;
    s32 ch;
    u8 *b;
    u8 *q;
    u8 *e;
    u8 *v;
    u32 *g;
    u16 *w;
    s32 pit;
    s32 i;
    s32 n;
    s32 sum;
    s32 k;
    s32 x;
    s32 a;
    s32 l;
    s32 r;
    u16 t;

    ch = ch0;
    D_80085F98[0] = 1;
    if ((s32) D_80073734[0] - 1 < ch)
    {
        ch = -1;
    }
    if (ch != -1)
    {
        b = D_80086A40[bank].unk_04;
        q = b + ((s16) prog * 0x10 + 0x20);
        e = b + ((s16) prog * 0x10 + key) * 0x20 + 0x820;
        pit = ((s16) vel << 8) + (s16) fine;
        if (vel >= e[6] && e[7] >= vel)
        {
            func_80056DB4(ch, (s16) vel << 8, (s16) vel);
            g = &D_80073740[ch];
            do
            {
                func_8005E97C(0, *g);
                k = func_8005EB78(*g);
            } while (k != 2 && k != 0);
            sum = 0;
            n = *(s16 *) (e + 0x16);
            w = (u16 *) (D_80086A40[bank].unk_04 + (*(u16 *) (b + 0x12) << 9) + 0x820);
            i = 0;
            if (n > 0)
            {
                do
                {
                    sum += *w;
                    i++;
                    w++;
                } while (i < n);
            }
            attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL | SPU_VOICE_VOLMODER |
                        SPU_VOICE_NOTE | SPU_VOICE_SAMPLE_NOTE | SPU_VOICE_WDSA |
                        SPU_VOICE_ADSR_AMODE | SPU_VOICE_LSAX | SPU_VOICE_ADSR_ADSR1 |
                        SPU_VOICE_ADSR_ADSR2;
            attr.volmode.left = 0;
            attr.volmode.right = 0;
            attr.voice = D_80073740[ch];
            attr.addr = D_80086A40[bank].unk_10 + sum * 8;
            v = D_80085458 + ch * 0x78;
            t = *(u16 *) (e + 0x10);
            attr.adsr1 = t;
            *(u32 *) (v + 0x60) = t;
            t = *(u16 *) (e + 0x12);
            attr.adsr2 = t;
            *(u32 *) (v + 0x64) = t;
            if ((*(u16 *) (e + 0x10) & 0x80) != 0)
            {
                attr.a_mode = SPU_VOICE_EXPIncN;
            }
            else
            {
                attr.a_mode = SPU_VOICE_LINEARIncN;
            }
            attr.loop_addr = attr.addr;
            v = D_80085458 + ch * 0x78;
            v[0x22] = e[4];
            v[0x23] = e[5];
            v[0x21] = e[0xC];
            *(s16 *) (v + 0x00) = ch;
            *(s16 *) (v + 0x08) = key;
            *(s16 *) (v + 0x0A) = pit >> 8;
            *(s16 *) (v + 0x06) = 0x11;
            *(u16 *) (v + 0x04) = prog;
            *(s16 *) (v + 0x1A) = 1;
            v[0x20] = e[0xD];
            v[0x14] = q[1];
            v[0x16] = q[4];
            v[0x15] = e[2];
            v[0x17] = e[3];
            *(s16 *) (v + 0x5C) = bank;
            x = D_80086A40[bank].unk_1B + v[0x16] + v[0x17] - 0x80;
            if (x < 0)
            {
                x = 0;
            }
            if (x >= 0x80)
            {
                x = 0x7F;
            }
            *(s16 *) (v + 0x18) = x;
            a = D_80086A40[bank].unk_18 * v[0x14] * v[0x15] >> 7;
            if (x >= 0x40)
            {
                r = a;
                l = (0x40 - (x & 0x3F)) * (r << 1) >> 7;
            }
            else
            {
                l = a;
                r = x * (l << 1) >> 7;
            }
            l = l * D_80086A40[bank].unk_18 >> 7;
            r = r * D_80086A40[bank].unk_18 >> 7;
            v = D_80085458 + ch * 0x78;
            *(s16 *) (v + 0x10) = l * (s16) volx >> 7;
            *(s16 *) (v + 0x12) = r * (s16) panx >> 7;
            attr.volume.left = *(s16 *) (v + 0x10);
            attr.note = pit;
            attr.volume.right = *(u16 *) (v + 0x12);
            if (e[5] != 0)
            {
                attr.sample_note = (*(e - 0x1C) << 8) | (0x7F - e[5]);
            }
            else
            {
                attr.sample_note = e[5] | (e[4] << 8);
            }
            func_8005EC0C(&attr, l, r);
            if ((e[1] & 4) != 0)
            {
                func_8005E4A0(1, D_80073740[ch]);
            }
            else
            {
                func_8005E4A0(0, D_80073740[ch]);
            }
        }
        else
        {
            ch = -1;
        }
    }
    D_80085F98[0] = 0;
    return ch;
}
