/* func_8005BBFC (unk_2CDC0) -- 333 of 333 words, 220 DIFF. NOT compiled.
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
 * 1. `prog` and `key` are the two parameters that outlive the calls and only
 *    ONE callee-saved register is free for them: the listing spills `prog` to
 *    a halfword slot and keeps `key` in $fp, and this source does the
 *    opposite. Writing the two late stores in the order +0x08 then +0x04
 *    (below) takes the register-only count from 90 to 87 but does not flip
 *    the choice. Measured and not working: `prog` as `s16`, `key` as `u16`
 *    (332 words), and `(s16) prog * 0x10` hoisted into its own local.
 *
 * 2. The two stack parameters are loaded in the listing's PROLOGUE (`lw $t0,
 *    0xA0($sp)` / `lw $t1, 0xA4($sp)` before anything else) and here they are
 *    loaded at their uses. All eight parameters as `s16` puts the loads in
 *    the right place but is 327 words, six short, because the `(s16)` casts
 *    then disappear from the four places the listing keeps them.
 *
 * 3. The order of the voice-record stores and of the attr fills: the listing
 *    interleaves them differently in the 0x8005BE50..0x8005BF00 stretch, and
 *    the source order below is m2c's reading of the listing rather than a
 *    measured choice. Those stores are all to the same base, so sched moves
 *    them freely and a permutation sweep over them is the obvious next step
 *    (perm.py, or search.py now that it has the declaration-order and
 *    per-arm transforms).
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
        if ((s16) vel >= e[6] && e[7] >= (s16) vel)
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
