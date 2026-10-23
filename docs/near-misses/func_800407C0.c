/* func_800407C0 (unk_12DEC) -- 178 of 178 words, 62 DIFF. Not compiled; see
 * docs/open-matching-problems.md. BOUNDED: the blocker is identified and
 * priced, under this project's assumed compilation context. That context -- the
 * unit boundary, the declared sizes, the -G value, what else shares the unit --
 * is inferred, not known, so read the measurements below as evidence and the
 * word "unreachable" as shorthand for "not reachable by any source shape
 * measured, with the context held fixed". A C source producing these bytes
 * exists by construction. See the context audit in docs/where-we-are.md.
 *
 * An RLE/nibble-stream blitter. `*src++` is a mode byte: bit 0 selects a
 * linear decoder, mode 0 is a plain memcpy, and anything else is a strided
 * decoder whose stride is `mode >> 1`. Both decoders read a control byte per
 * run: a zero high nibble means "copy low*2 nibbles from the stream",
 * otherwise "repeat the low nibble (high >> 4) + 1 times".
 *
 * Every opcode, immediate, displacement, branch offset, delay slot and
 * basic-block boundary matches. All 62 differences are one seven-value
 * register permutation:
 *
 *     c/b  $a0 -> $a1      rowend   $a2 -> $t4
 *     n    $a1 -> $t0      sphase_A $v1 -> $a3
 *     m    $t4 -> $a2      q        $a3 -> $a0
 *                          t_A      $a2 -> $v1
 *
 * d ($s0), size ($s1), phase ($t1), src ($t2), row ($t3), end ($v1) and
 * sphase_B ($a3) are already right.
 *
 * WHY NO SOURCE SHAPE REACHES IT. The blocker is a preference, not a
 * priority. Allocno
 * `end` carries `preferences: 3 6`, and find_reg's first pass avoids registers
 * preferred by conflicting allocnos -- which is what pushes `c` off $v1/$a2 to
 * $a0 and `m` off $a2 to $t4. The `6` is created by the memcpy call: with the
 * call deleted as a probe, end's preference collapses to {3} and src/d recover
 * their own parameter preferences, because nothing crosses a call any more. In
 * the real function d and size do cross it, so their preferences are pruned by
 * call_used_reg_set and the whole assignment shifts. No spelling of `end`
 * changes that.
 *
 * ONE OPEN IDEA, added 2026-10-23 from reading global.c and NOT yet measured.
 * Everything above argues on `end`'s side -- that no spelling changes `end`'s
 * `preferences: 3 6`. That is true and it is the wrong side. `prune_preferences`
 * (global.c, the loop at ~871) builds `regs_someone_prefers[A]` from the
 * lower-priority conflicting allocnos' preferences but **subtracts A's own
 * preferences first**, and the source comment is explicit: if A also prefers
 * one of those registers, it is not excluded. The subtraction is skipped only
 * when the rival allocno is larger, so for same-mode allocnos it always
 * applies.
 *
 * So `c` is not pushed off $v1/$a2 because `end` prefers them; it is pushed off
 * because `end` prefers them AND `c` does not. Give `c` its own preference for
 * $v1 or $a2 -- a copy relating it to that hard register, i.e. being an
 * outgoing argument in it, a call return value in it, or an incoming parameter
 * in it, which is what `set_preference` records -- and R leaves
 * `regs_someone_prefers[c]` without anybody's priority moving.
 *
 * CAVEAT CHECKED, and the escape SURVIVES it. `prune_preferences` OR-s
 * `call_used_reg_set` into the exclusion set of any allocno that CROSSES A CALL
 * (global.c:851-854, an if/else on `allocno_calls_crossed[allocno] == 0`) and
 * then AND_COMPLs all three preference sets with it, deleting every
 * caller-saved preference. $v1 and $a2 are caller-saved, so the escape needs
 * `c` not to cross the memcpy.
 *
 * It does not. The .greg dump does NOT print `allocno_calls_crossed`, and the
 * hard registers in its `conflicts:` lines are not call-clobber marks, so the
 * conflict lists cannot answer this -- but `find_reg` can. At global.c:923-926
 * it COPIES `call_used_reg_set` into `used1` for a call-crossing allocno, i.e.
 * excludes every caller-saved register outright. Allocnos 86 and 87 are both
 * assigned hard reg 4 ($a0), which IS caller-saved, so neither crosses a call.
 * (The one loophole is the `accept_call_clobbered` retry at 921-922, which is a
 * last-resort pass; it would show as a spill, and there is none here.)
 *
 * And the .greg dump shows exactly why the subtraction currently does nothing:
 * `;; 114 preferences: 3 6` for `end`, and allocnos 86 and 87 have **no
 * `preferences:` line at all**. So regs_someone_prefers[86] = {3,6} minus the
 * empty set = {3,6}, 86 avoids both and takes $a0. There is nothing to subtract
 * because `c` prefers nothing.
 *
 * SO THE TARGET IS NARROW AND NAMED: give allocno 86 or 87 a hard-register
 * preference for 3 ($v1) or 6 ($a2). `set_preference` records one from a COPY
 * relating the pseudo to a hard register, which from C means an argument
 * position or a return value. That makes reg 3 effectively unreachable ($v1 is
 * only the high word of a 64-bit return) and leaves reg 6 = $a2 = **the third
 * argument of a call** as the single live route -- and this function has
 * exactly one call, the memcpy, whose third argument is the count. So the
 * question to answer is whether the original's memcpy count expression is
 * written in terms of the value that becomes `c`/`b`, rather than in terms of
 * `size`.
 *
 * That is a much narrower claim than this header's original one. It is not
 * "no spelling changes end's preference" (true, and the wrong side of the
 * conflict); it is "the pushed-off allocno needs a preference, and only one
 * argument slot can give it one". UNMEASURED -- the .greg evidence above is
 * solid, the source experiment has not been run.
 *
 * Independently, the target's order is out of reach arithmetically: `m` would
 * have to be allocated before `n`, and their floor_log2(refs)*refs/live_length
 * priorities are 0.54 against 2.55. m's nine references and its live range over
 * the whole of branch B are both fixed by the target's own code, so neither
 * number can move by five times.
 *
 * TWO FINDINGS WORTH KEEPING, both of which improved it from 67 to 62:
 *
 * `t` must be TWO pseudos, one per branch. The target loads it into $v1 in
 * branch A and $a0 in branch B; one shared variable is one allocno and
 * therefore one register, so no allocation can produce both. On its own the
 * split is worse (74) -- it only pays off together with the next item, which
 * is presumably why it was rejected earlier.
 *
 * Branch A's literal loop has to be a `goto` loop, not a `for`. Dropping its
 * NOTE_INSN_LOOP_BEG reweights the references inside it and moves sphase_B from
 * $t0 to $a3. The emitted shape is unaffected: the guard-plus-rotate is the
 * same either way. The same change to any other loop is worse -- branch A's
 * RLE loop and branch B's literal loop cost words (176 and 91), and both outer
 * loops cost 23-29 diffs. Whether the goto is the original shape or a
 * coincidental win is not decidable from the emitted code.
 *
 * Also load-bearing, established before this pass:
 *  - `d` is the second parameter, reassigned throughout, with `phase = 0;
 *    row = d;` before `m = *src++;`.
 *  - `size` is ONE function-scope variable shared by branch B and the memcpy
 *    arm -- worth 50 words. A separate local in the memcpy arm is block-local,
 *    so local-alloc grabs $s0 for it before global-alloc runs and `d` is pushed
 *    to $s1.
 *  - `c` and `b` are different variables and `b` is `u8`. That is what produces
 *    the `andi $v0,$a1,0xF` + `addu $a1,$v0,$zero` + `sll $v1,$v0,4` triple,
 *    and it is the whole reason the word count reaches 178; one u32 variable is
 *    2 words short.
 *  - `c` is `u32` with multiple sets, which keeps `andi $v0,$a1,0xFF` alive and
 *    makes bltz/sra survive instead of being narrowed by combine's
 *    nonzero_bits.
 *  - `n` is one variable across all four arms, which keeps the bltz guards.
 *  - `m` is one variable for both the mode and the stride (`m = m >> 1` in
 *    place); that reassignment is what gives `sra` rather than `srl`.
 *  - the literal arm is the fall-through and the RLE arm the else in both
 *    decoders; branch B is the if and the memcpy arm the else; the sphase
 *    sub-tests are `if (sphase != 0) {low} else {high}`; and branch B's phase-0
 *    arm is the preset form while branch A's is a plain if/else -- the two are
 *    NOT interchangeable (the preset form in branch A costs 97 words).
 *  - `break` out of branch A's loop with `return d + 1;` after it.
 *  - branch A's phase-1 arm needs `q = d;` BEFORE the `if (phase == 0)` test:
 *    the target fills that delay slot with `addu $a0,$s0,$zero`, which reorg
 *    can only take from an insn preceding the branch, and the increment must
 *    precede the store or the copy coalesces away. The natural
 *    `*d = *d | w; d++;` leaves the slot a nop and gives 179 words.
 *
 * Also rejected, none better than 62: splitting `n` and/or `c` per branch (the
 * 61 that produces is noise -- the target has one $a1 for c and one $t0 for n
 * across both branches, so the split is structurally wrong); every scope and
 * declaration-order change; swapping end/rowend's assignment order or
 * spelling; a single exit for branch B via goto; `c = *src; src++;` split;
 * `(c >> 4) == 0` instead of `(c & 0xF0) == 0`; `size` as u32; `w` as u8.
 *
 * RE-EXAMINED 2026-10-23 with the levers that closed four other functions,
 * and **the unreachability argument survives.** The new material was section
 * 3's `regs_someone_prefers` entry and the func_8003E4FC finding that a
 * hard-reg COPY preference can be deleted by changing how the argument is
 * spelled -- which is exactly the shape of this blocker, since `end`'s `6`
 * comes from `size` being copied into $a2 as memcpy's third argument and
 * propagating back through the `end = d + size` addu. Four attempts on that,
 * all 178 words:
 *
 *  - a SECOND function-scope `size2` used only in the memcpy arm, so `end`'s
 *    chain never touches the value that is copied into $a2: **124 DIFF**, and
 *    the same declared before `size`: 124. This is the strongest confirmation
 *    yet that one shared `size` is right -- the note above had only ruled out a
 *    block-local, which fails for a different reason (local-alloc grabs $s0).
 *  - memcpy's third argument spelled as the expression instead of the variable,
 *    `memcpy(d, src + 2, src[0] + (src[1] << 8))`: 62 DIFF and no extra word,
 *    because cse folds it straight back to `size`'s pseudo. That is the
 *    func_8003E4FC mechanism running the other way: there, writing the variable
 *    where a literal belonged CREATED a copy preference cse had manufactured;
 *    here the copy is genuine, so no spelling removes it.
 *  - `end = d; end += size;` to break the arithmetic chain into two sets: 62,
 *    unchanged. Preferences propagate through arithmetic regardless of the
 *    number of sets.
 *
 * So both halves of the original argument hold: the preference is created by a
 * real copy of a genuinely shared value, and the priority gap that would have
 * to close is 0.54 against 2.55. **Treat this as a wall.** The one thing that
 * would change the picture is a reason to believe `size` was NOT shared in the
 * original -- but sharing is worth 50 words, and not sharing is worth 62 more
 * differing ones, so the evidence is firmly against it.
 *
 * Integration, when something does crack it: structs.h needs
 * `u8 *func_800407C0(u8 *, u8 *);` in place of
 * `void func_800407C0(u8 *, u32 *);`, and a memcpy declaration that
 * agrees with this file's. **memcpy IS now declared project-wide** -- it went
 * into include/structs.h as `void *memcpy(void *, const void *, u32);` when
 * func_80041588 landed -- so this file's local `s32` version now draws
 * "conflicting types for built-in function `memcpy'" from cc1psx. That is a
 * warning, not an error, and the 62 reproduces either way; but whichever
 * signature this function needs has to be reconciled with func_80041588's
 * before it can land. The one call site needs a (u8 *) cast on its second
 * argument.
 */

#define func_800407C0 func_800407C0_decl_clash
#include "common.h"
#undef func_800407C0

void *memcpy(void *, const void *, s32);

u8 *func_800407C0(u8 *src, u8 *d) {
    s32 phase = 0;
    u8 *row = d;
    s32 m = *src++;
    s32 n;
    u32 c;
    u8 b;
    s32 size;
    u32 w;

    if ((m & 1) != 0) {
        for (;;) {
            c = *src++;
            if ((c & 0xF0) == 0) {
                s32 sphase;
                u8 *q;

                if ((c & 0xFF) == 0) {
                    break;
                }
                n = (c & 0xFF) * 2;
                sphase = 0;
                if (n > 0) {
                  loopA:
                    if (sphase == 0) {
                        c = *src++;
                    }
                    q = d;
                    if (phase == 0) {
                        if (sphase != 0) {
                            *d = c & 0xF;
                        } else {
                            *d = c >> 4;
                        }
                    } else {
                        u32 t;

                        t = *d;
                        d++;
                        if (sphase != 0) {
                            w = (c << 4) & 0xF0;
                        } else {
                            w = c & 0xF0;
                        }
                        *q = t | w;
                    }
                    sphase ^= 1;
                    phase ^= 1;
                    n--;
                    if (n > 0) {
                        goto loopA;
                    }
                }
            } else {
                n = c >> 4;
                b = c & 0xF;
                for (; n >= 0; n--) {
                    if (phase == 0) {
                        *d = b;
                    } else {
                        *d = *d | (b << 4);
                        d++;
                    }
                    phase ^= 1;
                }
            }
        }
        return d + 1;
    }
    if (m != 0) {
        u8 *rowend;
        u8 *end;
        s32 sphase;

        m = m >> 1;
        size = src[0] + (src[1] << 8);
        src += 2;
        rowend = d + m;
        end = d + size;
        do {
            c = *src++;
            if ((c & 0xF0) == 0) {
                n = c * 2;
                for (sphase = 0; n > 0; n--) {
                    u32 v;

                    if (sphase == 0) {
                        c = *src++;
                    }
                    if (phase == 0) {
                        v = c & 0xF;
                        if (sphase == 0) {
                            v = c >> 4;
                        }
                    } else {
                        u32 t;

                        t = *d;
                        if (sphase != 0) {
                            w = (c << 4) & 0xF0;
                        } else {
                            w = c & 0xF0;
                        }
                        v = t | w;
                    }
                    *d = v;
                    d += m;
                    sphase ^= 1;
                    if (d >= end) {
                        phase ^= 1;
                        if (phase == 0) {
                            row++;
                            if (row >= rowend) {
                                return end;
                            }
                        }
                        d = row;
                    }
                }
            } else {
                n = c >> 4;
                b = c & 0xF;
                for (; n >= 0; n--) {
                    if (phase == 0) {
                        *d = b;
                    } else {
                        *d = *d | (b << 4);
                    }
                    d += m;
                    if (d >= end) {
                        phase ^= 1;
                        if (phase == 0) {
                            row++;
                            if (row >= rowend) {
                                return end;
                            }
                        }
                        d = row;
                    }
                }
            }
        } while (d + m < end || phase == 0 || row + 1 < rowend);
        return end;
    } else {
        size = src[0] + (src[1] << 8);
        memcpy(d, src + 2, size);
        return d + size;
    }
}
