/* func_8004DF8C (unk_1C540) -- 72 of 72 words, 0 DIFF. NOT compiled.
 *
 * MATCH (verify.py RESULT: MATCH, 2026-10-27). Deliberately NOT integrated
 * into csrc/ -- report first. Was 70 of 72 / 63 DIFF when picked up.
 *
 * The sibling of func_8004DE88 (also parked): the same command buffer in
 * D_80083E18, but with a strcat of D_80071404 after the strcpy, and the two
 * addresses it writes out byte-wise are D_80081548 and D_8008154C instead of
 * D_80083D88 and D_80083D88+4.
 *
 * TWO SHAPES CLOSED IT, one word each, and both generalise.
 *
 * 1. ONE `u8 *p` FOR BUFFER AND CURSOR, advanced IN PLACE:
 *        p = D_80083E18; *p++ = 8; strcpy(p, s); strcat(p, ..);
 *        p = strrchr(D_80083E18, 0); *p++ = 0x19; ...
 *    not `b = &D_80083E18[1]` plus a separate cursor. Two effects, read in
 *    the cse dump: (a) `p++` writes the pseudo that held the lowered symbol,
 *    so cse's class {p, D_80083E18} is invalidated and strrchr's argument is
 *    re-lowered from the still-valid `(high)` pseudo -- the target's
 *    `addiu $s0,$s0,%lo(D_80083E18)` after strcat, and `$s0` is the lui
 *    register because the high dies into that lo_sum. With a separate `b`,
 *    reg 84 (= D_80083E18) stays valid and every later `lo_sum` collapses
 *    onto it, one pseudo live across all three calls, one word short.
 *    (b) the cursor after strrchr is the SAME pseudo as `p`, so it keeps the
 *    callee-saved `$s1` it already had and the `addu $s1,$v0,$zero` copy
 *    survives -- the word func_8004DE88's header hunts in local-alloc.
 *    REVERT COST (park's `b` + separate `p`, everything else as matched):
 *    68 words, 69 DIFF.
 *
 * 2. THE FINAL STORES GO THROUGH THE ADDRESS POINTER, `u32 *q`:
 *        q = &D_80081548; p[0] = (u32) q; ...   q = &D_8008154C; ...
 *        dispatch;   q[-1] = 0x2300030C;   q[0] = 0x2300040C;
 *    with BOTH symbols declared as scalars `extern u32 D_80081548;`.
 *    Three mechanisms, all measured in isolation (probes below):
 *      - scalar (size <= -G8) sets SYMBOL_REF_FLAG, so cc1psx emits
 *        `la $18,SYM` for the address and a bare-symbol `sw $2,SYM` for the
 *        store. maspsx's aspsx-2.56 config has gp_allow_la=False, so the
 *        `la` is left to gas, which at -G0 expands it to `lui`/`addiu` in the
 *        SAME register (the target's `lui $s2`/`addiu $s2,$s2,%lo`); the
 *        `sw` IS rewritten to `%gp_rel` because of the `.size` line.
 *      - cse's find_best_addr replaces a bare `(mem (reg q))` by the symbol
 *        (mips_address_cost: REG costs 4, a flagged SYMBOL_REF 1), which is
 *        `q[0]` -> `sw $v0,%gp_rel(D_8008154C)($gp)`; it leaves
 *        `(mem (plus q -4))` alone (checked in the cse dump).
 *      - combine folds `reg+const` into `SYM-4` ONLY when the store is the
 *        pointer's sole use (LOG_LINK to the set, and q dies there). Here q
 *        has the four byte-write uses, so `-4($s2)` survives and q, the
 *        FULL address, is what lives across the jalr in `$s2` -- not a high
 *        half. That is why the target has three callee-saved registers and
 *        the park's `%lo(SYM)($sN)` stores needed a fourth.
 *    Equivalent spellings that ALSO MATCH: `u32 a` with
 *    `*(u32 *) (a - 4) = X; D_8008154C = Y;`, and `q[-1] = X; D_8008154C = Y;`.
 *    REVERT COSTS: plain `D_80081548 = X; D_8008154C = Y;` -> 70 words,
 *    62 DIFF (both symbolic, nothing crosses the jalr, two words short).
 *    Incomplete arrays for both (the old park's declarations) with q[-1]/q[0]
 *    -> 74 words, 70 DIFF (two `high` pseudos cross the jalr for the
 *    `%lo()($sN)` stores, plus an `$s3` save/restore). Only D_80081548 as an
 *    incomplete array -> 72 words, 2 DIFF: `lui $v0`/`addiu $s2,$v0` -- the
 *    "declared size decides how many registers the address costs" lever.
 *
 * MEASURED AND RULED OUT, with the corrections to the old header:
 *  - The old claim that a scalar `extern u32 D_80081548;` makes the address
 *    a single `la` "which maspsx then makes $gp-relative" is WRONG (see
 *    gp_allow_la above). The "four words short" it reports came from
 *    `a = (u32) D_80081548` with the SCALAR declaration, which reads the
 *    VALUE: `lw $3,D_8008154C` via %gp_rel, one word. Re-measured: 71 words,
 *    53 DIFF. Write `&` when switching a symbol from array to scalar.
 *  - Shape 1 alone, with the old park's store half: 74 words, 69 DIFF.
 *  - Shape 1 + old declarations + `*(u32 *) (a - 4)`: 74 words, 70 DIFF.
 *  - The offset's SIGN is irrelevant: addresses taken in the other order
 *    with `D_80081548 = X; *(u32 *) (a + 4) = Y;` -> 72 words, 2 DIFF, the
 *    two words being `sw %gp_rel(D_80081548)` / `sw 4($s2)` where the target
 *    has `sw -4($s2)` / `sw %gp_rel(D_8008154C)`. Which symbol the pointer
 *    ends on is read straight off the listing's `-0x4`.
 *  - The old header's "68 words with `&D_80081548[1]`" stands: that is
 *    cse_insn's use_related_value sharing the first materialisation.
 *  - advise.py on the matching source says VERDICT: SHAPE with an IMMEDIATE
 *    row at 8004E058; that is the `%gp_rel(D_80081550)` load's relocation
 *    (built shows 0). verify.py is the authority and says MATCH.
 *
 * Probes (each a 1-5 line function, cc1psx -O2 -G8 -S):
 *    q = &SYM; q[-1] = 1;                      -> sw $2,SYM-4   (combine)
 *    q = &SYM; OTHER = (u32) q; q[-1] = 1;     -> la $3; sw $2,-4($3)
 *    q = &SYM; q[-1] = 1; OTHER = (u32) q;     -> la $3; sw $2,-4($3)
 *    q = &SYM; OTHER = (u32) q; q[0] = 1;      -> sw $2,SYM      (cse)
 *    q = &SYM; q[-1] = 1; q[0] = 2;            -> sw SYM-4; sw SYM
 *    *(u32 *) ((u8 *) &SYM - 4) = 1;           -> sw $3,SYM-4 (legit addr)
 *
 * CANDIDATE LEVER A -- "a walked pointer re-lowers its symbol": a listing
 * with `lui $sA,%hi(SYM)` whose `%lo` is added TWICE, once into another
 * register that is then incremented in place and once back into `$sA` after
 * some calls, is one pointer variable assigned the symbol and advanced with
 * `p++`; the in-place add invalidates cse's register-equals-symbol class and
 * the second lowering comes from the surviving high. A separate `b = &SYM[1]`
 * keeps the symbol register valid and loses the second `addiu`.
 *
 * CANDIDATE LEVER B -- "`sw X,-C($sN)` beside `sw Y,%gp_rel(SYM)` after a
 * call": the two stores are `q[-1]`/`q[0]` through the pointer whose value
 * was also USED (written out, passed, shifted) before the call. cse turns the
 * bare `q[0]` into the symbol, combine cannot fold `q-4` because the pointer
 * has other uses, so the full address crosses the call in a callee-saved
 * register. Needs the symbol small (scalar declaration + `.size` for maspsx);
 * as an incomplete array both stores come out `%lo(SYM)($sN)` off high halves.
 */

#include "common.h"
extern u8 D_80083E18[];
extern u8 D_80071404[];
__asm__(".sdata\n\t.size D_80081548,4\n\t.size D_8008154C,4\n"
        "\t.size D_80081550,4\n\t.text");

extern u32 D_80081548;
extern u32 D_8008154C;

char *strcpy(char *, const char *);
char *strcat(char *, const char *);
char *strrchr(const char *, s32);

void func_8004DF8C(u8 *s) {
    u8 *p;
    u32 *q;

    p = D_80083E18;
    *p++ = 8;
    strcpy((char *) p, (char *) s);
    strcat((char *) p, (char *) D_80071404);
    p = (u8 *) strrchr((char *) D_80083E18, 0);
    *p++ = 0x19;
    *p++ = 2;
    *p++ = 0x1A;
    q = &D_80081548;
    p[0] = (u32) q;
    p[1] = (u32) q >> 8;
    p[2] = (u32) q >> 16;
    p[3] = (u32) q >> 24;
    p += 4;
    q = &D_8008154C;
    p[0] = (u32) q;
    p[1] = (u32) q >> 8;
    p[2] = (u32) q >> 16;
    p[3] = (u32) q >> 24;
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80083E18);
    q[-1] = 0x2300030C;
    q[0] = 0x2300040C;
}
