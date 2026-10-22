/* func_800407C0 (unk_12DE0) -- 178 of 178 words, 62 DIFF, and argued to be
 * UNREACHABLE from C. Not compiled; see docs/open-matching-problems.md.
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
 * WHY IT IS UNREACHABLE. The blocker is a preference, not a priority. Allocno
 * `end` carries `preferences: 3 6`, and find_reg's first pass avoids registers
 * preferred by conflicting allocnos -- which is what pushes `c` off $v1/$a2 to
 * $a0 and `m` off $a2 to $t4. The `6` is created by the memcpy call: with the
 * call deleted as a probe, end's preference collapses to {3} and src/d recover
 * their own parameter preferences, because nothing crosses a call any more. In
 * the real function d and size do cross it, so their preferences are pruned by
 * call_used_reg_set and the whole assignment shifts. No spelling of `end`
 * changes that.
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
 * Integration, when something does crack it: structs.h needs
 * `u8 *func_800407C0(u8 *, u8 *);` in place of
 * `void func_800407C0(u8 *, u32 *);`, and `void *memcpy(void *, const void *,
 * s32);` next to bzero -- memcpy is still undeclared project-wide, the same gap
 * func_80041344 hit. The one call site needs a (u8 *) cast on its second
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
