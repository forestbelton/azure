/* func_80042BDC (unk_14934) -- 543 of 543 words, 200 DIFF. NOT compiled, and
 * not landable alone: it dispatches through jtbl_8002D67C, so it needs the
 * splat/linker change in docs/open-matching-problems.md and all three
 * unk_14934 switch functions must land together. verify.py masks the two
 * relocated halfwords that reference the table, so 543/543 is necessary and
 * not sufficient; only `make check` is authority.
 *
 * "Remove status effect N from this entity": clear one bit in the +0x1C flag
 * word and, if +0x14 & 0x4000, print a message. Case 10 additionally performs a
 * whole transform-revert -- spawn a replacement entity, copy the record across
 * with func_80042710, relink it -- and that one arm is 180 instructions and
 * holds all 200 differing words.
 *
 * THE TABLE: 32 ENTRIES, not the 13 previously recorded. 0x8002D67C..0x8002D6FB,
 * 128 bytes. The range check is `sltiu $v0, $v1, 0x20`, and the word at
 * 0x8002D6FC is 0x94826082 -- Shift-JIS text, not a code address -- so the
 * table ends there. Verified twice, from the listing and from the file.
 * 0x80043430 is both the default label and the out-of-range target, so the
 * eleven values 13-17, 24, 25 and 28-31 have no `case` label at all.
 *
 * SOURCE ORDER of the arms, by body address:
 *   1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 27, 11, 12, 26, 32, (21/22/23), 18, 19, 20
 * Five arms are out of numerical order: 27 sits between 10 and 11, 26 between
 * 12 and 32, and 32 before the shared 21/22/23 body at 0x8004333C.
 *
 * `Unk41344` does NOT fit this function -- it is not a command-stream walker.
 * The record is the existing `Unk42710`: this function hands its own arg0
 * straight to func_80042710, and its only caller indexes `arg0[i * 2 + 0x2C]`,
 * which is Unk42710's `Unk42710B unk_2C[4]`. Two things about it are new:
 *
 *  - **The pointer is 0x20 bytes into a larger container.** The function reads
 *    -0x20 (handed to func_800D5460), -0x18 (a pointer word), -0x14 (a pointer
 *    to another record of the same kind) and -0x02 (a halfword OR'd with
 *    0x8000). func_80042984 already uses `*(u8 **) (arg0 - 0x14)`, so this
 *    matches existing practice, but the container itself is undescribed.
 *  - **Unk42710's size floor rises from 0x8C to at least 0xAA**, with these
 *    fields and this evidence:
 *      +0x13 u8   the entity "kind"; lbu, compared to 0x2E and 0, written from
 *                 +0xA8, and read back after the store (a real re-dereference)
 *      +0x14 u32  flag word; & 0x4000 gates every message, >> 14 & 1 taken as
 *                 a flag, |= 0x20000000
 *      +0x1C u32  second flag word; bits 0x10/0x20/0x40/0x80/0x200/0x400/
 *                 0x800/0x1000 cleared one per case, 0x2000 set and cleared,
 *                 |= 0x10000, >> 19 & 1 taken as a flag
 *      +0x71 u8   compared signed against the halfword at +0x8A, and
 *                 subtracted from it
 *      +0x88 s16  lh, passed as the indirect call's 4th (s16) argument
 *      +0x8A u16  read BOTH ways in one expression -- lh for the comparison and
 *                 lhu for the subtraction -- so both spellings are needed
 *      +0x98 u16  &= 0x7FFF then & 0x4000 tested
 *      +0xA6 u16  copied into (-0x14)->+0x12
 *      +0xA8 u8   copied into +0x13
 *    On the -0x14 neighbour: its +0x24 and +0x25 are read as separate bytes and
 *    passed as the first two arguments of func_8009A3D0/func_8009A21C, and
 *    +0x12 is written as a halfword. +0x24/+0x25 is exactly Unk42710's
 *    hand-split byte pair, which is what confirms -0x14 points at another
 *    record of the same type.
 *
 * LOAD-BEARING, each measured by reverting it from this source:
 *
 *  1. `extern u8 D_800Exxxx[];` for the 15 message strings, NOT
 *     `(u8 *) 0x800E....` constants: 543 words either way, but 15 extra
 *     differing words, because a constant gives lui/`ori` where a symbol gives
 *     lui/`addiu`. See the correction to matching-levers section 6.
 *  2. ONE function-scope `s32` scratch shared between the 1/3 value (passed as
 *     func_800A1618's s16 argument) and func_800A1BD0's return value (also
 *     cast `(s16)`). Separate locals: 542 words / 351 DIFF, with both
 *     `sll 16`/`sra 16` pairs collapsing to nothing. The call's return value is
 *     what keeps the casts alive; see the correction to section 7.
 *  3. `func_8003F320()` between func_800A0B94 and the indirect call -- a real
 *     call, easy to miss. Folding it away: 541 words / 332 DIFF.
 *  4. Case 10's head RE-READS `p[0x13]` at all three tests rather than caching
 *     it. A cached `s32 t` gives one `lbu` plus a `move` (541 words, two
 *     short); `u8 t` does force the reload (543) but adds an
 *     `andi $a0,$v1,0xff` the target does not have. The `goto done10` is
 *     needed, because the `== 0` exit and the `!= 0x2E` exit both land on the
 *     arm's own message block, not on the switch's exit.
 *  5. The ternary form at case 10's selection site and case 12's second site,
 *     the `if` form at the other five. All-ternary: 542 words / 305 DIFF.
 *     All-`if`: 543 words, but the argument `lbu`s fall below the branch at all
 *     seven sites. See the new section 8 entry.
 *  6. An `s16` second parameter. `s8` gives `sll 24`/`sra 24` and 202 DIFF;
 *     `s32` emits no extension at all. See the new section 8 entry.
 *  7. NO `default:` label. The out-of-range path and all eleven unlabelled
 *     values go to the statement after the switch, so there is no tail to
 *     protect -- the same as func_80041588, where func_80041344 is the odd one
 *     out with its `D_80080A8C = 0` tail.
 *  8. `p` is the parameter, REASSIGNED: case 10 ends with `p = q`
 *     (`addu $s1,$s0,$zero`), so the arm's message block and the trailing
 *     func_80041E70(p) both see the new entity. A `u8 *` parameter is
 *     byte-identical to `s8 *`, so the existing declaration can stay.
 *  9. D_80083460 is reached through a SEPARATE `u8 *` base local in each of the
 *     two arms that use it, with +2/+8 in one and +0xA in the other -- the
 *     func_800439F8 pattern. The existing Unk80083462 typedef is based two
 *     bytes past the label, so its field names do not line up with these
 *     displacements.
 *
 * WHAT IS LEFT, all inside case 10:
 *
 *  (i) ONE WORD, and it is the one to chase. The target emits
 *      `addu $a0,$s1,$zero` twice around the join at .L800430C4/.L800430C8 --
 *      once as its own single-insn basic block entered from
 *      `bnez $v0, .L800430C4`, and once stolen into the preceding
 *      `j .L800430C8`'s delay slot by reorg. This source emits it once, so
 *      case 10 is one word short, and that shortfall is currently paid for by
 *      the five `if`-form selection sites each being a word long. Since the
 *      all-ternary form is almost certainly the original shape at all seven
 *      sites, recovering this word should make the all-ternary source land at
 *      543. Ruled out for it: explicit `else { if (...) }` nesting (542/305)
 *      and inverting to `if (bit == 0)` (541/287).
 * (ii) the `|= 0x10000` flag temp is in $v1 where the target has $a0, which is
 *      why the target's first call argument goes `lbu $v0` then
 *      `addu $a0,$v0,$zero` in a delay slot while this loads straight into $a0
 *      and leaves a nop.
 *(iii) the `(*(u32 *) (p + 0x1C) >> 19) & 1` flag stays in $s2 and is tested
 *      `beqz $s2`; the target copies it to $v0 first and tests that.
 * (iv) D_800E3D7C's loaded base lands in $a0 and the scaled index in $v0, the
 *      reverse of the target, which flips the `addu`'s operand order and the
 *      registers of the following lw/lbu/andi/sll/sw chain. Writing the add
 *      with the operands the other way round made no difference.
 *
 * include/structs.h, when this lands: the 15 `extern u8 D_800Exxxx[];` message
 * strings (they are in build/slus_006.14.map, just not in structs.h),
 * `extern u8 *D_800E3D7C;`, `extern u8 *D_800E3DF0[];`, the Unk42710 fields
 * above, and `typedef u8 *(*Fn42BDC)(s32, s32, s32, s16);` for what
 * func_800A0B94 returns. Seventeen new function prototypes are listed in the
 * agent's report; func_800A1BD0 and func_8003F320 already exist and are right.
 * NOTE THE RISK: the declaration at structs.h:1102 must change from
 * `void func_80042BDC(s8 *, s8);` to `(s8 *, s16)`, and the only caller,
 * func_80042B68, is already-matching C that passes an `s8` -- so that function
 * needs re-verifying when this lands.
 */

#define func_80042BDC func_80042BDC_decl_clash
#include "common.h"
#undef func_80042BDC

extern u8 D_800E1D55[];
extern u8 D_800E1D61[];
extern u8 D_800E1D80[];
extern u8 D_800E1D9F[];
extern u8 D_800E1DCF[];
extern u8 D_800E1DEE[];
extern u8 D_800E1E0D[];
extern u8 D_800E1E32[];
extern u8 D_800E1E4B[];
extern u8 D_800E1E6A[];
extern u8 D_800E1E8C[];
extern u8 D_800E1EC1[];
extern u8 D_800E1EF7[];
extern u8 D_800E1F1C[];
extern u8 D_800E1F41[];
extern u8 *D_800E3D7C;
extern u8 *D_800E3DF0[];

typedef u8 *(*Fn42BDC)(s32, s32, s32, s16);

void *func_800990FC(void);
void *func_80099734(u8 *, void *);
void *func_80099194(u8 *, void *);
void func_80099290(void *);
void func_800A5720(void *);
void func_800AA53C(u8 *);
void func_800D5460(u8 *, u32, s32);
u8 *func_800A1618(s32, s16);
void func_8009A3D0(s32, s32, s32);
void func_8009A21C(s32, s32, s32);
Fn42BDC func_800A0B94(s32, u8 *, s32);
void func_800A9A0C(u8 *);
void func_800AA888(u8 *, u8 *, u8 *, u8 *);
void func_800AC82C(u8 *, u8 *, u8 *, u8 *);
void func_800A32A4(u8 *);
void func_8009A028(u8 *);
void func_800AA36C(u8 *, u8 *, u8 *, u8 *);
void func_80041E70(void *);
void func_8004397C(u8 *);
void func_80042984(u8 *);

void func_80042BDC(s8 *arg0, s16 arg1) {
    u8 *p = (u8 *) arg0;
    void *msg;
    u8 *e;
    s32 m;
    s32 k;

    switch (arg1) {
    case 1:
        *(u32 *) (p + 0x1C) &= ~0x200;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D55, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 2:
        *(u32 *) (p + 0x1C) &= ~0x400;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D61, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 3:
        *(u32 *) (p + 0x1C) &= ~0x10;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D80, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        if (p[0x13] != 0) {
            func_800AA53C(p);
        }
        break;
    case 4:
        *(u32 *) (p + 0x1C) &= ~0x20;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D9F, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 5:
        *(u32 *) (p + 0x1C) &= ~0x40;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DCF, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 6:
        *(u32 *) (p + 0x1C) &= ~0x80;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DEE, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 7:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E0D, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 8:
        *(u32 *) (p + 0x1C) &= ~0x800;
        break;
    case 9:
        *(u32 *) (p + 0x1C) &= ~0x1000;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E32, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 10: {
        if (p[0x13] == 0x2E || p[0x13] == 0) {
            func_800D5460(p - 0x20, 0x20A0A0, 0x613);
            if (p[0x13] == 0) {
                goto done10;
            }
        }
        if (p[0x13] == 0x2E) {
            u8 *q;
            Fn42BDC fn;
            s32 s3;
            s32 bit;
            u8 *g;

            e = *(u8 **) (p - 0x14);
            k = 1;
            p[0x13] = p[0xA8];
            if (*(u32 *) (p + 0x14) & 0x4000) {
                k = 3;
            }
            q = func_800A1618(p[0x13], (s16) k);
            *(u32 *) (p + 0x1C) |= 0x10000;
            s3 = (*(u32 *) (p + 0x14) >> 14) & 1;
            func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8004397C(p);
            fn = func_800A0B94(p[0x13], q, 1);
            func_8003F320();
            q = fn(s3, e[0x24], e[0x25], *(s16 *) (p + 0x88));
            *(u32 *) (q + 0x14) = 0;
            *(u32 *) (q + 0x1C) = 0;
            bit = (*(u32 *) (p + 0x1C) >> 19) & 1;
            func_80042710((Unk42710 *) q, (Unk42710 *) p);
            func_80042984(q);
            g = (u8 *) &D_80083460;
            if (*(u16 *) (g + 2) & 0x1000) {
                if (*(s16 *) (p + 0x8A) < p[0x71]) {
                    *(u16 *) (g + 8) -= p[0x71] - *(u16 *) (p + 0x8A);
                }
            }
            func_800A9A0C(q);
            *(u32 *) (q + 0x1C) &= ~0x10000;
            if (*(u32 *) (p + 0x14) & 0x4000) {
                u8 *w;

                k = func_800A1BD0(p);
                w = ((s16) k) * 4 + D_800E3D7C;

                D_800E3DF0[(*(u8 **) (w + 0xD0))[3] & 0x1F] = q;
                *(u8 **) (w + 0xAC) = q;
            }
            if (bit != 0) {
                u8 *w = D_800E3D7C;

                *(u8 **) (w + 0x124) = q;
                *(u8 **) (q + 0x60) = w;
                *(u32 *) (q + 0x1C) |= 0x80000;
                func_800AA888(q, *(u8 **) (q - 0x18), *(u8 **) (q - 0x14), q);
                func_800AC82C(q, *(u8 **) (q - 0x18), *(u8 **) (q - 0x14), q);
            } else if (q[0x25] == 0) {
                u8 *g2 = (u8 *) &D_80083460;

                *(u16 *) (g2 + 0xA) += 1;
                *(u32 *) (q + 0x1C) &= ~8;
            }
            *(u32 *) (p + 0x14) |= 0x20000000;
            func_800A32A4(p);
            func_8009A028(p);
            *(u16 *) (p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
            p = q;
        }
    done10:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E4B, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    case 27:
        e = *(u8 **) (p - 0x14);
        m = 0x3000;
        if (*(u32 *) (p + 0x1C) & 0x2000) {
            m = 0x300;
        }
        func_8009A21C(e[0x24], e[0x25], m);
        func_800AA36C(p, *(u8 **) (p - 0x18), e, p);
        break;
    case 11:
        e = *(u8 **) (p - 0x14);
        m = 0x3000;
        if (*(u32 *) (p + 0x1C) & 0x2000) {
            m = 0x300;
        }
        func_8009A3D0(e[0x24], e[0x25], m);
        *(u32 *) (p + 0x1C) &= ~0x2000;
        func_8009A21C(e[0x24], e[0x25], 0x3000);
        break;
    case 12:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E6A, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        e = *(u8 **) (p - 0x14);
        m = 0x3000;
        if (*(u32 *) (p + 0x1C) & 0x2000) {
            m = 0x300;
        }
        func_8009A3D0(e[0x24], e[0x25], m);
        *(u32 *) (p + 0x1C) |= 0x2000;
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 26:
        e = *(u8 **) (p - 0x14);
        *(u16 *) (p + 0x98) &= 0x7FFF;
        m = 0x3000;
        if (*(u32 *) (p + 0x1C) & 0x2000) {
            m = 0x300;
        }
        func_8009A3D0(e[0x24], e[0x25], m);
        if (*(u16 *) (p + 0x98) & 0x4000) {
            *(u32 *) (p + 0x1C) |= 0x2000;
        }
        m = 0x3000;
        if (*(u32 *) (p + 0x1C) & 0x2000) {
            m = 0x300;
        }
        func_8009A21C(e[0x24], e[0x25], m);
        *(u16 *) (e + 0x12) = *(u16 *) (p + 0xA6);
        break;
    case 32:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E8C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 21:
    case 22:
    case 23:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EC1, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 18:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EF7, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 19:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F1C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 20:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F41, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    func_80041E70(p);
}
