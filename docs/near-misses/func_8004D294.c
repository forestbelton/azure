/* func_8004D294 (unk_1C540) -- 114 of 114 words, 9 DIFF. NOT compiled.
 *
 * Wraps a caller's three Euler angles into 12-bit signed range, wraps
 * D_80083178's own three the same way, and snaps any axis that ended up more
 * than half a turn away from it; then hands both vectors to func_8004D1EC.
 * Three identical nine-word blocks, one per axis.
 *
 * WHAT IS PAID FOR
 *
 * * There are TWO func_8004D1EC calls, not one. m2c's output was truncated at
 *   the first; the second is the last statement of the `q != NULL` block,
 *   `func_8004D1EC(b + 0x140, q, a & 0xFFFF, (u16 *) (b + 0xAC))`. Without it
 *   the function is eight words short -- five for the call and three for the
 *   $s2 save, copy and restore, which only exist because `a` is used on both
 *   sides of the first call and therefore has to survive it.
 *
 * * D_80083178 is reached through a `u8 *` base with byte displacements, NOT
 *   through the struct. The listing materialises `%lo(D_80083178)` once into
 *   $s1 and uses 0xAC/0xAE/0xB0/0xB8/0xA4/0x140 off it -- and 0x140 is past
 *   the end of Unk80083178 anyway. The angles are read `lhu` and stored `sh`,
 *   so they are `*(u16 *)` and not the struct's `s16` SVECTOR fields.
 *
 * * The caller's vector is read BOTH ways per axis: `lhu` for the two masks
 *   and `lh` for the difference, so it is a `u8 *` with `*(u16 *)` and
 *   `*(s16 *)` at the same offset.
 *
 * WHAT IS LEFT: 27 words, nine in each of the three blocks, and it is ONE
 * register choice replicated three times. The target holds the caller's angle
 * in $v0 and the sign-extended wrapped reference in $v1:
 *
 *     sll  $v1, $a0, 16      <- this source: sll $v0, $a0, 16
 *     sh   $a0, 0xAC($s1)
 *     lh   $v0, 0x0($s0)     <- this source: lh $v1, 0x0($s0)
 *     sra  $v1, $v1, 16
 *     subu $v0, $v0, $v1     <- this source: subu $v0, $v1, $v0
 *
 * and the `lhu` for the `& 0xFFF` mask lands between the `slti` and the
 * `bnez` where this source puts it right after the `subu`, which shifts four
 * more rows per block. The two facts are one: whichever quantity gets $v0
 * decides where the spare load goes.
 *
 * Measured, all 114 words and all 27 DIFF: `w` as `s16` with no cast on the
 * subtraction; a separate `s16` local holding the sign-extended copy; the
 * subtraction written `-((s16) w - q)`; and re-reading the field
 * (`*(s16 *) (b + 0xAC)`) instead of using `w`. Reading the caller's angle
 * into its own local first is WORSE, 33 DIFF. So no spelling of the
 * subtraction moves it, and the next step is `tools/decomp/sched.py` on one
 * block -- section 8 of docs/matching-levers.md for what its output means.
 *
 * 2026-10-27: 27 DIFF -> 18 -> 9, by two changes that cost nothing.
 *
 * 1. THE THREE CONDITIONAL NEGATES ARE `abs()`. `d = EXPR; if (d < 0) { d =
 *    -d; }` puts the value and the negation in one cse class, canon_reg
 *    rewrites the negation to read the older member, and the copy that should
 *    sit in the branch's delay slot is hoisted out -- here it cost 9 words
 *    across the three sites. `d = abs(EXPR)` gives the listing's shape
 *    directly: `bgez` / delay slot / `negu $v0, $v0` in place. Same cure as
 *    func_8003BD84, which landed on it; see matching-levers section 5.
 *
 * 2. THE OPERANDS INSIDE `abs()` ARE REVERSED -- `abs((s16) w - *(s16 *) (q +
 *    k))`, not `abs(*(s16 *) (q + k) - (s16) w)`. This is FREE semantically,
 *    since abs(a - b) == abs(b - a), and it is worth 9 more words: it makes
 *    the subtraction `subu $v0, $v0, $v1` with the SAME ENCODING as before
 *    while swapping which value each register holds, so the result lands in
 *    the register the negate wants and the `move` disappears at all three
 *    sites.
 *
 * WHAT IS LEFT: NINE WORDS, three at each site, and they are one register
 * swap. The listing extends `w` into $v1 and loads q[k] into $v0; this source
 * does the opposite:
 *
 *     target  sll $v1,$a0,16 ; lh $v0,0($s0) ; sra $v1,$v1,16 ; subu $v0,$v0,$v1
 *     built   sll $v0,$a0,16 ; lh $v1,0($s0) ; sra $v0,$v0,16 ; subu $v0,$v0,$v1
 *
 * Note the two `subu` words are IDENTICAL by coincidence: each form puts the
 * difference in its own op0's register, and `d` wants $v0 either way. The
 * target's `subu $v0, $v0, $v1` is `load - ext`, so the TARGET'S OPERAND ORDER
 * IS `abs(*(s16 *) (q + k) - (s16) w)`, which is the opposite of what this
 * file compiles -- see the escapes below for why this file keeps the other.
 *
 * 2026-10-28: THE NINE WORDS ARE EXPLAINED, and the explanation is an exact
 * allocator TIE. From the `.lreg` dump, at each site local-alloc sees two
 * COMBINED quantities, because `block_alloc` ties an insn's destination to a
 * source that dies in it and `qty_n_refs` is then the SUM of both pseudos'
 * references (local-alloc.c:1870):
 *
 *     the extension  {sll dest, sra dest}    4 refs over a 4-insn span
 *     the delta      {lh dest, subu dest}    4 refs over a 4-insn span
 *
 * so both price out at floor_log2(4) * 4 * 4 / 4 = 8 -- the SAME priority.
 * `qty_compare_1` breaks an exact tie on the LOWER QTY NUMBER
 * (local-alloc.c:1622), and qty numbers are handed out in birth order, so the
 * register goes to whichever quantity is born first. The extension is born
 * first because sched1 puts the `sll` ahead of the `lh`: the `lh` carries a
 * true-dependence edge on the `sh` to `b + 0xAC` (`true_dependence`,
 * sched.c:817 -- both MEMs have varying addresses, so neither disambiguation
 * clause fires), and `rank_for_schedule`'s class key prefers the candidate
 * that is independent of the last-scheduled insn. The `.lreg` dump prints the
 * edge as the `(insn_list 94 (nil))` on the load.
 *
 * MEASURED PROOF of exactly that: move the write-back AFTER the delta
 * (`*(u16 *) (b + 0xAC) = w;` below the `d = abs(...)` line) and the edge is
 * gone, the load is born first, and ALL NINE REGISTER WORDS COME OUT RIGHT --
 * `sll $v1` / `lh $v0` / `sra $v1` / `subu $v0,$v0,$v1`, with the `nop` in the
 * `bgez` delay slot instead of this file's `move`. It is not a match overall
 * (42 DIFF) because the `sh` then sinks eight words down and the if-body's two
 * `andi`s swap registers, but nothing else is left to explain.
 *
 * So the target needs the DELTA's quantity to win the tie WITH the store still
 * ahead of the load. Three escapes, all closed:
 *
 *   * REVERSING THE OPERANDS ALONE (`abs(load - (s16) w)`, the target's own
 *     order) keeps every opcode and displacement right but leaves the load in
 *     $v1, which costs a `move` in the delay slot as well: 18 DIFF. Hoisting
 *     `(s16) w` into a local first measures the same 18 -- sched1 normalises
 *     the order, and a local shared by the three sites is a global allocno.
 *   * RTX_UNCHANGING_P on the load would cut the edge at the first line of
 *     `true_dependence`, but cc1psx does not set it for a cast pointer
 *     dereference: `*(const s16 *) (q + k)` still prints as a plain `(mem:HI`
 *     with no `/u` and the edge survives, 9 DIFF unchanged.
 *   * THE MEM_IN_STRUCT_P CLAUSE needs the load IN a struct at a varying
 *     address AND the store at a FIXED one. The store's base has to stay the
 *     register `$s1` to match the listing at all (see WHAT IS PAID FOR), so
 *     its address varies and the clause cannot fire.
 *
 * WHAT TO TRY NEXT, in the arithmetic above and without moving the store:
 * give the DELTA quantity a fifth reference (floor_log2(5) * 5 * 4 / 4 = 10
 * beats 8) or lengthen the EXTENSION's span to five insns (32 / 5 = 6.4 loses
 * to 8). A free reference on a block-local quantity DOES move it -- that is
 * the 2026-10-28 correction in section 3 of docs/matching-levers.md, which
 * until then said references were global-alloc's currency only.
 *
 * EXHAUSTION: a full `tools/decomp/search.py` sweep over every transform
 * family, 178 variants, run 2026-10-28 on this 9-DIFF base, improved on
 * nothing. The per-shape "measured" list that used to stand here was taken
 * before the two `abs()` changes and is superseded by that sweep.
 */

#include "common.h"

void func_8004D1EC(u8 *, u16 *, s32, u16 *);

void func_8004D294(u16 *p, u8 *q, s32 a)
{
    u8 *b = (u8 *)&D_80083178;
    u16 t;
    u16 v;
    u16 w;
    s32 d;

    if (p != NULL)
    {
        func_8004D1EC(b + 0xB8, p, a & 0xFFFF, (u16 *)(b + 0xA4));
    }
    if (q != NULL)
    {
        t = *(u16 *)(q + 0);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 0) = v;
        t = *(u16 *)(b + 0xAC);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xAC) = w;
        d = abs((s16)w - *(s16 *)(q + 0));
        if (d >= 0x801)
        {
            *(u16 *)(q + 0) = (w & 0xF000) | (*(u16 *)(q + 0) & 0xFFF);
        }

        t = *(u16 *)(q + 2);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 2) = v;
        t = *(u16 *)(b + 0xAE);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xAE) = w;
        d = abs((s16)w - *(s16 *)(q + 2));
        if (d >= 0x801)
        {
            *(u16 *)(q + 2) = (w & 0xF000) | (*(u16 *)(q + 2) & 0xFFF);
        }

        t = *(u16 *)(q + 4);
        if ((t & 0x800) != 0)
        {
            v = t | 0xF800;
        }
        else
        {
            v = t & 0x7FF;
        }
        *(u16 *)(q + 4) = v;
        t = *(u16 *)(b + 0xB0);
        if ((t & 0x800) != 0)
        {
            w = t | 0xF800;
        }
        else
        {
            w = t & 0x7FF;
        }
        *(u16 *)(b + 0xB0) = w;
        d = abs((s16)w - *(s16 *)(q + 4));
        if (d >= 0x801)
        {
            *(u16 *)(q + 4) = (w & 0xF000) | (*(u16 *)(q + 4) & 0xFFF);
        }
        func_8004D1EC(b + 0x140, (u16 *)q, a & 0xFFFF, (u16 *)(b + 0xAC));
    }
}
