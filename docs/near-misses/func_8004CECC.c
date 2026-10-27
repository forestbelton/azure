/* func_8004CECC (unk_1C540) -- 102 of 102 words, 48 DIFF. NOT compiled.
 *
 * HSV -> RGB, writing three bytes through the fourth argument and returning
 * it. The hue range is 1536 = 6 * 256 (`h %= 1536` by the 0xAAAAAAAB/10 magic,
 * sector = `h >> 8`, fraction = `h & 0xFF`), the three mixed components are
 * each `(val * (0xFF00 - k) + 0x7F80) / 0xFF00` -- a signed divide by 65280 by
 * the 0x80808081/15 magic, with 0x7F80 as the round-to-nearest bias -- and the
 * six switch arms are the textbook sector table (v,t,p / q,v,p / p,v,t /
 * p,q,v / t,p,v / v,p,q). `tools/decomp/magic.py` names both divisors; do not
 * estimate them from the arithmetic.
 *
 * THE 101 DIFF IS A ONE-WORD SHIFT, NOT 101 WRONG WORDS. Every opcode,
 * immediate, load displacement, branch offset and delay slot matches, and so
 * does the `beqz` -> end delta (0x1f in both), so the whole switch and all six
 * arms are byte-correct. The entire gap is ONE missing instruction near the
 * top, which shifts every later word and makes the positional diff useless.
 * Read the branch immediates, not the diff: the `bnez` is 4 against the
 * target's 5.
 *
 * THE MISSING WORD is the `v` parameter's entry copy, `addu $v1,$a2,$zero`.
 * The target spends four register copies at entry (h -> $t2, v -> $v1,
 * out -> $t1, then val = v as `addu $a2,$v1,$zero` in the bnez delay slot);
 * this source spends three, because its `v` pseudo simply KEEPS its incoming
 * $a2 and needs no copy. The RTL is otherwise structurally identical -- `v` is
 * read exactly twice in both, once into val in the grey arm and once through
 * the mask in the colour arm -- so this is purely which hard register the
 * long-lived `val` allocno wins, and nothing else.
 *
 * WHY, FROM THE DUMPS (tools/decomp/dump.sh; .flow, .lreg and .greg):
 *   - `val` is pseudo 85, "used 14 times across 68 insns", a global allocno
 *     (set in both arms). Its priority 3*14/68 = 0.62 puts it SECOND in
 *     `;; 18 regs to allocate`, well before the `v` parameter (pseudo 83,
 *     3 refs / 8 insns = 0.38, twelfth) -- so allocation ORDER is already
 *     right and is not the lever.
 *   - It still loses, because `;; 85 conflicts` contains hard regs 2 3 4 5 6 7:
 *     every one of $v0..$a3 is already held by a block-local quantity of the
 *     colour block, which `local-alloc` assigned BEFORE global-alloc ran.
 *     `find_reg` therefore takes the first free register, $t0, and `v` keeps
 *     $a2 unopposed.
 *   - local-alloc's priority is refs/live_length, so the short two-reference
 *     scratch values take $v0/$v1 repeatedly and FOUR longer-lived locals take
 *     $a0..$a3: the 0xFF00 constant (4 refs/44 insns, $a0), p's dividend
 *     (4/23, $a1), `sat` (4/21, $a2) and the 0x80808081 magic (4/22, $a3).
 *   - The target's assignment says its colour block had only THREE such locals
 *     on the low registers: 0xFF00 in $a0, p's dividend in $a1, `sat` in $a3,
 *     and **the 0x80808081 magic in $t0** -- with $a2 left to `val`. That the
 *     magic constant sits in $t0 rather than $a3 is the concrete, checkable
 *     signature of the original shape, and it is the thing to aim at.
 *
 * MEASURED AND NOT WORKING, all still 101 words unless noted:
 *   - `out[0] = out[1] = out[2] = v;` for the grey arm. store_expr's
 *     copy_to_reg does NOT emit a copy here (the operand is already a
 *     register), the stores come out 2,1,0, and jump2 cross-jumps the third
 *     into case 5: 100 words.
 *   - `u8 v` with no `val` at all. This DOES buy the entry copy and the
 *     natural `andi`, but the grey arm's third store then matches an arm's and
 *     jump2 merges it: 100 words. Note the target's case 3 and case 4 also end
 *     `sb $a2,0x2($t1)`, identical to its grey arm, and were NOT merged -- so
 *     cross-jumping here is not purely register-driven and that asymmetry is
 *     itself unexplained.
 *   - `val` hoisted above the `if` (`val = v;` then `val = v & 0xFF;` in the
 *     else). This correctly puts the copy in block 0 where the target's delay
 *     slot wants it, but cse then rewrites the mask to read val's own register
 *     (`andi $t0,$t0,0xff`) and the two pseudos coalesce: still 101.
 *   - Two separate block-scoped locals, one per arm: the grey one coalesces
 *     into the parameter outright, 99 words.
 *   - Three zero-cost reference levers from matching-levers section 3 -- the
 *     mask split as `val = v; val &= 0xFF;`, the grey stores reordered 2,1,0,
 *     and a separate `vv = v;` feeding the mask -- all coalesced away, 101.
 *   - `search.py`'s whole greedy sweep: 78 variants, 8 transform families. Its
 *     only accepted change, `sat = s; sat &= 0xFF;`, reaches 102 words and 48
 *     DIFF, but it buys the word with a spurious `sll`/`sra` sign-extension of
 *     `s` that the target does not have. That is the semantic accident
 *     search.py's own docstring warns about, not a lever -- do not resume from
 *     /tmp/azure-decomp/search_best.c.
 *
 * TYPES ARE SETTLED, and getting them wrong costs 11 words, not 1: `h` must be
 * unsigned (the 1536 division is `multu`) and everything else signed (all three
 * 65280 divisions are `mult` with the `sra 31` correction). `s` is `s16` -- the
 * test is `sll $a1,16` then `bnez`, one word, where `u16` gives
 * `andi 0xffff`. `v` is wider than a byte, because the colour arm masks it.
 *
 * NOT BLOCKED ON PLACEMENT, unlike the four functions in
 * docs/open-matching-problems.md: nothing here needs jtbl_8002E5F8 to land for
 * verify.py to be meaningful, because the shift is before the dispatch. But
 * landing it DOES need the placement -- see that document's region map, which
 * has the segment this unit's `.rodata` wants.
 *
 * SINCE PARKED, 2026-10-27: 101 of 102 words and 101 DIFF to 102 of 102 and
 * 48, by splitting `sat = s & 0xFF;` into `sat = s; sat &= 0xFF;`. Found by
 * search.py's split-binop family in the sweep of all forty parks.
 *
 * READ THAT CAREFULLY: the word it adds is an `sra`, and the listing has no
 * `sra`. With `sat` assigned from the `s16` parameter in a statement of its
 * own, gcc materialises the sign-extended value (`sll 16` / `sra 16`) where
 * the masked form folds it away -- and the listing's extra word, the one this
 * source has been missing all along, is still the `addu $v1, $a2, $zero`
 * entry copy of `v` described below. So the count now coincides for the wrong
 * reason.
 *
 * It is kept because the DIFF drop is real: with the lengths equal the two
 * streams align, and advise.py can now say the whole remainder is 44
 * register-only differences plus that one swap -- `sra $a1, $a1` here against
 * `addu $t2, $a0, $zero` there. That is a far better starting point than 101
 * differing words, and it confirms the allocation analysis below rather than
 * replacing it.
 *
 * Also measured on the new baseline, all of them 102 and 48 or worse: `u8 v`
 * (which gives an `andi` on the parameter, not the copy), `u16 s`, `s32 s`
 * with `(s16)` casts at both uses, `s16 sat` and `u16 sat` (both 101 again,
 * i.e. they remove the `sra` and the word with it), `sat = (u8) s` (103), and
 * `sat = sat & 0xFF` rather than `&=` (identical).
 */

#include "common.h"

u8 *func_8004CECC(u32 h, s16 s, s32 v, u8 *out) {
    s32 val;
    s32 sat;
    s32 f;
    s32 p;
    s32 q;
    s32 t;

    if (s == 0) {
        val = v;
        out[0] = val;
        out[1] = val;
        out[2] = val;
    } else {
        val = v & 0xFF;
        sat = s;
        sat &= 0xFF;
        p = (val * (0xFF00 - (sat << 8)) + 0x7F80) / 0xFF00;
        h %= 1536;
        f = h & 0xFF;
        q = (val * (0xFF00 - sat * f) + 0x7F80) / 0xFF00;
        t = (val * (0xFF00 - sat * (0x100 - f)) + 0x7F80) / 0xFF00;

        switch (h >> 8) {
        case 0:
            out[0] = val;
            out[1] = t;
            out[2] = p;
            break;
        case 1:
            out[0] = q;
            out[1] = val;
            out[2] = p;
            break;
        case 2:
            out[0] = p;
            out[1] = val;
            out[2] = t;
            break;
        case 3:
            out[0] = p;
            out[1] = q;
            out[2] = val;
            break;
        case 4:
            out[0] = t;
            out[1] = p;
            out[2] = val;
            break;
        case 5:
            out[0] = val;
            out[1] = p;
            out[2] = q;
            break;
        }
    }
    return out;
}
