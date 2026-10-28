/* func_8005B4D0 (unk_2CDC0) -- 377 of 376 words, 344 DIFF. NOT compiled.
 *
 * func_8005BBFC's sibling: the note-on that has to FIND a voice. The packed
 * first argument carries the program in its low seven bits and the bank above
 * bit 8; it counts how many of the bank's first `prog` entries have a
 * non-zero +0x20, walks the layers of the +0x20 record, and for each layer
 * whose [+0x06, +0x07] range contains the key it scans for a hardware voice
 * that func_8005EB78 says is idle, falling back to a scan of D_80085458 for
 * an entry whose +0x1A is clear. Everything from func_80056DB4 onward -- the
 * idle wait, the length sum, the SpuVoiceAttr fill, the voice record, the pan
 * math -- is func_8005BBFC's block, so read that park's header first: every
 * shape it lists is paid for here too.
 *
 * THE NEW LEVER, worth 10 words here: THE TWO SCAN LOOPS NEED AN EXPLICIT
 * `goto`, not `while` or `for (;;)`.
 *
 * Both scans have the listing's single test at the TOP with the back edge
 * jumping to it:
 *
 *    .L8005B668: lhu  $v0, 0x1A($a0)
 *                nop
 *                beqz $v0, .L8005B690        <- the only copy of the test
 *                addiu $s1, $s1, 1
 *                slt  $v0, $v1, $s1
 *                beqz $v0, .L8005B668        <- back edge to the test
 *                addiu $a0, $a0, 0x78
 *
 * Written as `while (cond) { ... if (bound) break; ... }` or as
 * `for (;;) { if (!cond) break; ... }` loop.c's `duplicate_loop_exit_test`
 * copies the top test to the bottom -- the rotation -- and the loop comes out
 * four words longer with a second `lhu`/`nop`/`bnez`. Written as
 *
 *    scan:
 *        if (cond)
 *        {
 *            i++;
 *            if (bound) { i = -1; }
 *            else { p += STRIDE; goto scan; }
 *        }
 *
 * there are no loop notes at all, so nothing rotates and nothing is
 * strength-reduced -- which is also why the listing WALKS the D_80085458
 * pointer here (`addiu $a0, $a0, 0x78`) where every other loop in this unit
 * indexes. Six words for the second scan, four more for the first. Note this
 * is the OPPOSITE of func_8005914C, where the `while` form was wanted
 * precisely because duplicate_loop_exit_test produced the listing's shape:
 * read the listing for how many copies of the test it has before choosing.
 *
 * ALSO PAID FOR
 *
 * * The pan product is computed ONCE into a local and used twice: the listing
 *   stores `(rv * ps) >> 7` into the record's +0x12 and passes the UNSHIFTED
 *   `rv * ps` to func_8005EC0C. Written out twice it is a second `lhu`/`mult`
 *   pair.
 *
 * * `attr.sample_note`'s non-zero arm is `((e[4] - 1) << 8) | (0x7F - e[5])`
 *   -- e[4] minus one, where func_8005BBFC's same arm reads a byte 0x1C
 *   BELOW the record. The two functions differ there and the listing is the
 *   only authority.
 *
 * * `attr.note = b`, the whole second argument truncated: the listing reads
 *   its home slot with `lhu $t1, 0x94($sp)`, not the `b >> 8` the +0x0A store
 *   uses.
 *
 * WHAT IS LEFT -- three words and five nops, i.e. two non-nop words short.
 *
 * The frame is 0x90 with the SpuVoiceAttr at 0x10..0x4E and SIX more slots
 * above it: 0x50 and 0x52 (the two u16 volume arguments), 0x54 (`b >> 8`),
 * 0x58 (the +0x20 counter, loaded and stored inside the counting loop rather
 * than kept in a register), and 0x5C AND 0x60, which both hold the SAME
 * `D_80086A40[bank].unk_04` pointer. Those six are memory-resident because
 * the function uses every callee-saved register, and their displacements are
 * in the listing -- so the set of locals that lose the allocation has to
 * match, not just the code. advise.py's IMMEDIATE row points straight at the
 * first slot that does not line up, which is currently the second argument's
 * own home slot at 0x94 against a local at 0x60.
 *
 * Two source locals holding one pointer value is the odd part and is not yet
 * explained: cse would share one read, and reload gives a pseudo one slot, so
 * the original probably had two variables. The draft below uses two (`bs`
 * and `bs2`) on that reading.
 *
 * A SLOT CENSUS is the way to work on this, and it localises the defect to
 * one local. Counting every `(s|l)(w|h|hu|b|bu)` against `$sp` in both
 * streams:
 *
 *   0x10..0x4C   the SpuVoiceAttr -- every offset and every count already
 *                agrees, so the struct and its fills are right
 *   0x68..0x8C   the ten saved registers -- agree
 *   0x50, 0x52   the two u16 volume arguments -- agree
 *   0x54         `b >> 8`: this source writes and reads it twice, the
 *                listing once each
 *   0x58, 0x5C, 0x60   the counter and the two base pointers -- present in
 *                both, with the traffic off by one either way
 *   0x64         THIS SOURCE HAS A SEVENTH LOCAL AND THE LISTING HAS SIX
 *   0x94         the second argument's own home slot: the listing reads it
 *                TWICE (`lw` for `b >> 8`, `lhu` for `attr.note`), this
 *                source once
 *
 * So the seventh slot is a spilled copy of `b`, and the listing does not need
 * one because its `b` pseudo never won a register and reload used the
 * parameter's HOME slot instead. That is `assign_parms`' stack home being
 * reached through `reg_equiv_mem`, which only happens when the parameter
 * itself goes to memory -- not when a COPY of it does. Routing both uses
 * through a `bb = b;` local is 378 words but grows the frame to 0x98, an
 * eighth slot rather than six. Declaring the six in the listing's slot
 * order (below) moves the offsets but does not remove the seventh, and
 * dropping the `b >> 8` local to recompute it inline at all three uses is
 * 380 words, one worse. What would fix it is whatever makes `b` lose its
 * register outright -- an allocation question, and the first thing to try on
 * the next attempt.
 *
 * MEASURED 2026-10-28, the direct way of doing that: a `volatile` PARAMETER,
 * which keeps it in its stack home and makes every read a load -- which is
 * what the listing's two reads of 0x94 look like. It does not pay:
 *
 *     `volatile s32 b`   378 words, 362 DIFF
 *     `volatile s32 a`   380 words, 372 DIFF
 *
 * against 377 / 344. So the mechanism is reachable and the spelling is wrong:
 * `volatile` also stops the two reads sharing anything, which costs more than
 * the seventh slot saves. What the listing needs is `b` losing its register to
 * RELOAD under pressure, not being barred from one by its type.
 *
 * AND THE FIRST DIVERGENCE IS NOW IN THE PROLOGUE, worth writing down for the
 * next attempt: this build spills BOTH halves of the packed argument to the
 * frame and reloads one (`sw $a0, 0x64($sp)`, `sw $t1, 0x60($sp)`,
 * `lw $t1, 0x64($sp)`), where the listing keeps `a & 0x7F` in $fp from its
 * second instruction and never spills either. So the seventh slot and the
 * prologue spills are the same register-pressure fact seen twice.
 *
 * 2026-10-28: 379 -> 377 WORDS AND 367 -> 344 DIFF, from `unname.py`. The
 * `s32 m` holding `(s32) D_80073734[0] - 1` is deleted and the expression
 * written at its one use, `if ((s32) D_80073734[0] - 1 < ch)`.
 *
 * It is sound and it is the listing's shape, which is why it was adopted
 * rather than just recorded. The use sits inside the `scan:` goto-loop, so
 * one TEXTUAL use is many dynamic ones and inlining moves a load into the
 * loop -- but nothing in that loop writes D_80073734 (its body is `ch++`, the
 * bound test, `r += 0x78` and the `goto`, with no call), so the value is the
 * same on every pass. And the listing agrees: it materialises D_80073734
 * TWICE, `lui $s0 / lw $v0, %lo(...)($s0)` and then a second
 * `lui $v1 / lw $v1, %lo(...)($v1)` after .L8005B64C, i.e. inside the loop
 * region. A local for the bound is what gives this build only one.
 *
 * ALSO REPORTED AND NOT ADOPTED: dropping `q`
 * (`q = bs2 + (prog * 0x10 + 0x20)`, four uses) is 373 words / 355 DIFF on the
 * old base -- three words SHORT where the base was three OVER, so it trades
 * one sign of error for the other and only wins on the noisy axis. The address
 * is pure arithmetic on unmodified operands, so it is not unsound; it is just
 * not evidence. RE-MEASURED on this new base the same day and it no longer
 * improves at all -- a full `unname.py` pass over the 377-word source reports
 * zero hits -- so it was an artefact of the three-word overshoot that `m`
 * removed, and this park's locals are now exhausted for that tool.
 *
 * 2026-10-28, A FULL `search.py` PASS WHOSE ONLY HIT WAS UNSOUND -- so this
 * park is NOT swept out, and the note is here so nobody reads it as such. The
 * pass offered 562 candidates across all sixteen families and accepted exactly
 * one:
 *
 *     pass 1: hoist *(u32 *) (v + 0x64) into q0 at decl 10
 *             377 words (+1), 344 DIFF -> 376 words (+0), 333 DIFF
 *
 * which deletes the store. `t_hoist_read`'s `_READ` pattern matched the store's
 * DESTINATION and the family substituted its fresh local there, turning
 * `*(u32 *) (v + 0x64) = t;` into a dead `q0 = t;` -- and the word it "saved"
 * is that `sw`. Twelve of the twenty `_READ` matches in this file are store
 * destinations, so the family was offering unsound variants throughout the
 * run. It is fixed now (an lvalue is not a read), but the pass's own greedy
 * second round ran from that bad base, so nothing it reports about this
 * function can be trusted either way. RE-RUN the full pass.
 */

#include "common.h"

void func_80056DB4(s32);
void func_8005E4A0(s32, u32);
void func_8005E97C(s32, u32);
s32 func_8005EB78(u32);
void func_8005EC0C(SpuVoiceAttr *, s32, s32);

void func_8005B4D0(s32 a, s32 b, u16 volx, u16 panx, s32 force)
{
    SpuVoiceAttr attr;
    u16 vs;
    u16 ps;
    s32 key;
    s32 cnt;
    u8 *bs2;
    u8 *bs;
    u8 *q;
    u8 *e;
    u8 *v;
    u8 *r;
    u32 *g;
    u16 *w;
    s32 prog;
    s32 bank;
    s32 layer;
    s32 ch;
    s32 i;
    s32 n;
    s32 sum;
    s32 k;
    s32 x;
    s32 c;
    s32 l;
    s32 rv;
    s32 z;
    u16 t;

    i = 0;
    prog = a & 0x7F;
    bank = a >> 8;
    D_80085F98[0] = 1;
    cnt = 0;
    vs = volx;
    ps = panx;
    bs = D_80086A40[(s16) bank].unk_04;
    bs2 = bs;
    if (prog != 0)
    {
        r = bs;
        do
        {
            if (r[0x20] != 0)
            {
                cnt++;
            }
            i++;
            r += 0x10;
        } while (i < prog);
    }
    layer = 0;
    q = bs2 + (prog * 0x10 + 0x20);
    key = b >> 8;
    if (q[0] != 0)
    {
        do
        {
            e = bs2 + ((cnt * 0x10 + layer) * 0x20 + 0x820);
            if (key >= e[6] && e[7] >= key)
            {
                ch = 0;
            busy:
                if (func_8005EB78(D_80073740[ch]) != 0)
                {
                    ch++;
                    if ((s32) D_80073734[0] - 1 < ch)
                    {
                        ch = -1;
                    }
                    else
                    {
                        goto busy;
                    }
                }
                if (ch == -1)
                {
                    ch = 0;
                    r = D_80085458;
                scan:
                    if (*(u16 *) (r + 0x1A) != 0)
                    {
                        ch++;
                        if ((s32) D_80073734[0] - 1 < ch)
                        {
                            ch = -1;
                        }
                        else
                        {
                            r += 0x78;
                            goto scan;
                        }
                    }
                }
                if (ch != -1)
                {
                    func_80056DB4(ch);
                    g = &D_80073740[ch];
                    do
                    {
                        func_8005E97C(0, *g);
                        k = func_8005EB78(*g);
                    } while (k != 2 && k != 0);
                    sum = 0;
                    n = *(s16 *) (e + 0x16);
                    w = (u16 *) (D_80086A40[(s16) bank].unk_04 + (*(u16 *) (bs + 0x12) << 9) + 0x820);
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
                    attr.mask = SPU_VOICE_VOLL | SPU_VOICE_VOLR | SPU_VOICE_VOLMODEL |
                                SPU_VOICE_VOLMODER | SPU_VOICE_NOTE | SPU_VOICE_SAMPLE_NOTE |
                                SPU_VOICE_WDSA | SPU_VOICE_ADSR_AMODE | SPU_VOICE_LSAX |
                                SPU_VOICE_ADSR_ADSR1 | SPU_VOICE_ADSR_ADSR2;
                    attr.volmode.left = 0;
                    attr.volmode.right = 0;
                    attr.voice = D_80073740[ch];
                    attr.addr = D_80086A40[(s16) bank].unk_10 + sum * 8;
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
                    *(u16 *) (v + 0x04) = prog;
                    *(s16 *) (v + 0x08) = layer;
                    *(s16 *) (v + 0x0A) = key;
                    *(s16 *) (v + 0x06) = 0x11;
                    *(s16 *) (v + 0x1A) = 1;
                    v[0x20] = e[0xD];
                    v[0x14] = q[1];
                    v[0x16] = q[4];
                    v[0x15] = e[2];
                    *(s16 *) (v + 0x5C) = bank;
                    v[0x17] = e[3];
                    x = D_80086A40[(s16) bank].unk_1B + v[0x16] + v[0x17] - 0x80;
                    if (x < 0)
                    {
                        x = 0;
                    }
                    if (x >= 0x80)
                    {
                        x = 0x7F;
                    }
                    *(s16 *) (v + 0x18) = x;
                    c = D_80086A40[(s16) bank].unk_18 * v[0x14] * v[0x15] >> 7;
                    if (x >= 0x40)
                    {
                        rv = c;
                        l = (0x40 - (x & 0x3F)) * (rv << 1) >> 7;
                    }
                    else
                    {
                        l = c;
                        rv = x * (l << 1) >> 7;
                    }
                    rv = rv * D_80086A40[(s16) bank].unk_18 >> 7;
                    v = D_80085458 + ch * 0x78;
                    *(s16 *) (v + 0x10) = (l * D_80086A40[(s16) bank].unk_18 >> 7) * vs >> 7;
                    z = rv * ps;
                    *(s16 *) (v + 0x12) = z >> 7;
                    attr.volume.left = *(s16 *) (v + 0x10);
                    attr.note = b;
                    attr.volume.right = *(u16 *) (v + 0x12);
                    if (e[5] != 0)
                    {
                        attr.sample_note = ((e[4] - 1) << 8) | (0x7F - e[5]);
                    }
                    else
                    {
                        attr.sample_note = e[5] | (e[4] << 8);
                    }
                    func_8005EC0C(&attr, z, rv);
                    if ((e[1] & 4) != 0 || force != 0)
                    {
                        func_8005E4A0(1, D_80073740[ch]);
                    }
                    else
                    {
                        func_8005E4A0(0, D_80073740[ch]);
                    }
                }
            }
            layer++;
        } while (layer < q[0]);
    }
    D_80085F98[0] = 0;
}
