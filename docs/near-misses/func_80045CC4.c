/* func_80045CC4 (unk_176B8) -- 436 of 437 words, 47 DIFF. Not compiled; see
 * docs/open-matching-problems.md.
 *
 * It emits one slot's elements: builds the matrix in the scratchpad, then per
 * 0xC-byte element either calls the element's own callback or clips, projects
 * and links one 0x28-byte primitive into the OT.
 *
 * Every instruction is accounted for and the word count is one short. What is
 * left is ONE register swap (about 160 of the 167 differing words), a
 * four-word tension in the mask block, five words of local-alloc, and one word
 * in the tail. All four are measured below.
 *
 * SOLVED, 2026-10-22: the `(s8)` conversions, and the earlier mechanism
 * recorded here was wrong. This header used to say the fix was to load the
 * byte ONCE above the `if` and sign-extend it in both arms. That gives 435
 * words, not 437, because the target has a separate `lbu` IN EACH ARM:
 *
 *   lbu $v0,-0x6($s2) / lhu / lhu / sll 24 / sra 24 / negu / subu / sh / sh / j / subu
 *   lbu $v0,-0x6($s2) / lhu / lhu / sll 24 / sra 24 /        subu / sh / sh /     addu
 *
 * What works is a **HImode consumer**: declare the accumulator `s16` rather
 * than `s32` and the `lbu` / `sll 24` / `sra 24` survives in each arm, first
 * try, with no extra register. combine then runs `force_to_mode` with a 16-bit
 * mask down the chain instead of refolding to `lb`. The same fact explains why
 * the already-matching func_8003DBD0 is right. Full mechanism in
 * docs/matching-levers.md section 7; the `volatile` route that closed
 * func_80044D24 was tried here and gives 438 words / 382 DIFF, so it is not a
 * drop-in.
 *
 * Also solved: the mask hoisting. The hypothesis recorded here -- that 0xFFFFFF
 * costs lui+ori so loop.c hoists it while 0xFF000000 is one lui so it does not
 * -- is BACKWARDS. loop.c hoists whichever invariant is SET FIRST in the body,
 * regardless of cost, and hoists only one. Swapping the two operands of the `|`
 * in the first OT statement flips both, and then the hoisted `lui`/`ori`, the
 * inline `lui`, the `and` and their two prologue positions all match the target
 * exactly, at the right addresses.
 *
 * THE TENSION, worth 4 words and still open. The target's `or $v1,$v1,$v0` has
 * `q->unk_00 & 0xFF000000` as operand 0, which needs the NATURAL operand order
 * -- but the natural order hoists the wrong constant. With the swap, `or` and
 * `sw` match and only the two `lw`s and two `and`s have $v0/$v1 exchanged.
 * Ruled out: the masked OT value hoisted into a temp one statement earlier
 * (436/169 -- operand order right, registers still swapped); `(u32) q &
 * 0xFFFFFF` hoisted a statement earlier (438/425, BOTH constants then
 * hoisted); `q->unk_00` pre-loaded into a temp (436/167, cse folds it); a
 * function-scope `u32 m = 0xFFFFFF` (438/425); the same with `~m` for the other
 * mask (438/425). The open question is whether 0xFFFFFF's pseudo can be made
 * the first-set invariant without moving its first USE.
 *
 * THE REGISTER SWAP IS SOLVED, 2026-10-22: 167 DIFF -> 47. All eight
 * callee-saved registers and $fp now match ($s0 scratchpad, $s1 q+4, $s2 e+8,
 * $s3 s, $s4 q, $s5 e, $s6 0xFFFFFF, $s7 v, $fp arg3). The gap was 0.9% --
 * scratchpad 148 refs / 724 insns = 1.4309 against the q+4 giv's 77 / 320 =
 * 1.4438 -- and section 3's duplicate-an-insn lever closed it. The whole
 * change is `s16 *p70;` declared beside the other block locals and
 * `p70 = &p->unk_70;` duplicated into BOTH arms of the second sign site, with
 * `RotAverage4(p70, ...)` taking it. Word count unchanged: jump2 merges the
 * duplicate back out.
 *
 * Four things that cost real time to learn, all of which generalise and are
 * now in docs/matching-levers.md sections 1 and 3:
 *   - The ratio converges slowly, so count the live-length cost. Each
 *     duplicated 4-insn statement gave the scratchpad +4 refs and +8 live
 *     length and the giv +2 and +4 -- the duplicate costs the longer-lived
 *     pseudo twice as much live length. The obvious candidate (the
 *     unk_1C/unk_24 pair, appended to both arms of `if (s->unk_10 != 0)`,
 *     whose tail the target already cross-jumps) moved the gap only from
 *     -0.90% to -0.41%, and four such statements would have been needed where
 *     only two exist.
 *   - jump2 does NOT cross-jump a call. Duplicating the RotAverage4 call into
 *     both arms kept both copies: 451 words.
 *   - The duplicate must mention ONLY the pseudo you are raising. Hoisting the
 *     site-2 condition into a `c2` variable fixed $s0/$s1 (62 DIFF) but its
 *     extra `e->unk_00` read took `e` from 16 to 18 refs and flipped `e`
 *     against `q` -- 0.2400 against 0.2366, a 1.4% gap in the wrong direction.
 *   - cse can spoil a duplicate asymmetrically. `f2 = p->unk_24;` in both arms
 *     is one `lhu`, but the site-1 condition had already loaded that field, so
 *     cse turned the fall-through copy into `move $a1,$v1` while the other arm
 *     kept the `lhu`; the copies then differ, jump2 cannot merge them, and it
 *     cost a real word (59 DIFF). So duplicate an ADDRESS COMPUTATION that is
 *     not already live, not a reload of a field the enclosing block just read.
 *     `p70 = &p->unk_70;` works because nothing nearby computes that address.
 *
 * It must also be argument 0: `&p->unk_78`/`unk_80`/`unk_88` all give 435
 * words and 273-275 DIFF, because only argument 0's allocno carries the $a0
 * preference that keeps the duplicate from being folded differently. Even so
 * it lands in $a1, so 2 of the remaining 47 words are a `move $a0,$a1` that is
 * the lever's own cost. Declaring `p70` at function scope makes no difference.
 *
 * THE 47 REMAINING WORDS: 4 are the mask tension above, 2 the lever's cost,
 * 23 local-alloc, and 18 are one word in the tail whose mechanism is now known
 * but is not reachable from this function's control flow.
 *
 * THE TAIL WORD, and a new mechanism worth more than this function. The target
 * reads D_80083160 through the compiler-split form (`lui %hi` / `addiu %lo` /
 * `lw 0x0`) where this emits `lui` / `lw %lo`, and that one word shifts the
 * whole epilogue, hence 18. **A pointer-to-symbol local with TWO SETS selects
 * the split form:**
 *
 *     void b3(u32 x, s32 c) { Unk80083160 *g;
 *         if (c) { g = &D_80083160; } else { g = &D_80083160; }
 *         *(u32 *) (g->unk_00 + 0x8D0) = x; }
 *     ->  lui %hi / addiu %lo / lw 0($2) / sw
 *
 * With one set it folds back to `lui` / `lw %lo`. A CONST_INT folds through two
 * sets -- a two-set `(u8 **) 0x80083160` still folds -- so it is specifically a
 * SYMBOL_REF that two sets block. This is the first spelling found that picks
 * the split form for an in-image symbol, and it explains how one function can
 * mix both spellings of one object. It does NOT apply here: the tail block's
 * only predecessor is the loop's `break`, so there is no join where two sets
 * both reach the use, and putting the sets in the two arms of the `& 0x20` if
 * costs 2 words inside the loop plus a ninth callee-saved register. Also
 * measured as folding: the plain expression, a `u8 *o =` local, a block-local
 * `Unk80083160 *g = &D_80083160;`, `*(u8 **) &D_80083160`,
 * `((u32 *) ...)[0x234]`, a `volatile u8 **` local, a
 * `(volatile Unk80083160 *)` cast, and the constant address
 * `((Unk80083160 *) 0x80083160)->unk_00` (which makes cc1psx emit an absolute
 * `lw $2,-2146946720` that the assembler still expands to two words).
 *
 * SIXTEEN WORDS: the prologue order. Every save is in the right slot and
 * order; the difference is that this source's `%hi` scratch for the first
 * D_80083160 read gets $v0 where the target's gets $a0, which forces the
 * target to emit the `sw $a0,0x50($sp)` parameter spill before it -- an
 * anti-dependence sched2 can only see after allocation -- rotating 16 words.
 * That pseudo is born and dies in the entry block, so it is local-alloc;
 * promoting it to a global allocno would push it to a callee-saved register,
 * not to $a0.
 *
 * THE MASK TENSION, 4 words, still open. The two-set lever was probed for it
 * and does not separate: two sets of the 0xFF000000 pseudo would stop loop.c
 * hoisting it, but the only word-neutral place for them lands the merged `lui`
 * about 80 words earlier than the target's .L800462AC, and two sets in the
 * same block as the use are collapsed by cse.
 *
 * SEVEN WORDS: in the flag-merge block, `t` and `p->unk_24` are exchanged
 * between $v1 and $a0. Note this is the OPPOSITE of func_80042900's case: here
 * the target gives the block-local value (`p->unk_24`) the low register $v1
 * and the cross-block `u8 t` the higher $a0, where this source does the
 * reverse. `t` is already a global allocno (three sets in three blocks), so
 * there is nothing to promote, and promoting `p->unk_24` would push it off
 * $v1, which is where the target wants it.
 *
 * THREE MORE LOAD-BEARING SHAPES found with the above, each measured:
 *  - the four clip tests must be ONE expression, not four `s32` locals.
 *    Through locals, all four are computed and then three `or`s run at the
 *    end; inline, each partial `or` is emitted in the preceding block and
 *    reorg puts it in that block's branch delay slot, which is the target.
 *    Worth 27 DIFF.
 *  - `t` must be `u8`, not `s32`: with `s32`, combine narrows `t + 0xFF` to
 *    QImode for the byte store and trunc_int_for_mode rewrites the constant as
 *    -1, giving `addiu $v0,$v1,-0x1` where the target has `0xFF`. A separate
 *    `s32 u = t + 0xFF;` also works. Worth 2 words, at both clamp pairs.
 *  - the two min tests are `>`, not `<`: `if ((s16) q->unk_08 > (s16)
 *    q->unk_20)`, not `q->unk_20 < q->unk_08`. Same `slt`, but it swaps which
 *    field is loaded first, and the target loads 0x4($s1) then 0x1C($s1).
 *  - the join stores are duplicated into BOTH arms so that each arm's own
 *    `subu`/`addu` stays last and lands in the `j`'s delay slot; without it
 *    jump2 cross-jumps all four stores into the join. Worth 4 words. This is
 *    section 1's cross-jump lever used to PREVENT a merge.
 *
 * LOAD-BEARING, each measured by reverting it:
 *  - one scratchpad struct through one pointer local at 0x1F800000, but the
 *    libgte arguments as literal constants: `RotMatrix((void *) 0x1F800100,
 *    (void *) 0x1F8000D0)` gives the target's `addu $a0,$s0,$zero` / `ori`
 *    pairs, and twelve argument setups came out byte-exact first try. The
 *    RotAverage4 arguments are the opposite -- `&p->unk_70` etc., giving
 *    `addiu`. Same mixing as func_8003D0F0 and func_800463EC.
 *  - the loop tests before advancing: `for (;;) { body; if ((s8) e->unk_00 < 0)
 *    break; e++; }`. With `e++` first and `e[-1].unk_00` as the condition,
 *    combine_givs picks `e-0xC` as the representative and the second induction
 *    pointer comes out `addiu $s2,$s5,-12` where the target has `+0x8`. Worth
 *    about 40 differing words, and exactly the trap recorded for
 *    func_8003D0F0.
 *  - the `& 0x20` dispatch must be `if ((e->unk_00 & 0x20) == 0) { main }
 *    else { callback }` so the main body is the fall-through and the callback
 *    goes out of line. The other polarity inverts the branch: about 20 words.
 *  - unk_100, then unk_104, then unk_102 -- the same order func_8003DBD0
 *    needs.
 *  - the clip-test vertex fields are u16, giving `lhu`/`addiu`/`andi`/`sltiu`
 *    for `(u16) (q->unk_08 + 0x20) < 0x181`, while the SAME fields need
 *    `(s16)` casts in the later min tests to get `lh`. Declaring them s16 is
 *    wrong.
 *  - the OT address is recomputed per statement, not cached: two
 *    `lw 0xC0`/`lw 0x20` pairs, because the intervening `sw` invalidates cse.
 *  - unk_08/0C/10/14 are word/halfword unions, stored with `sw` and read back
 *    with `lhu`, like func_8003DBD0's Unk3DBD0W.
 *  - both clamp pairs are store-then-overwrite, not if/else.
 *
 * Writing the sign-arm expressions out four times instead of through a local
 * costs 80 words (509 against 429). A field-by-field OT update without the
 * second statement is 10 words short.
 *
 * TYPES this needs, all confirmed against the match (2026-10-22):
 *  - Unk45CC4E (0xC bytes: u8 unk_00..unk_03, u16 unk_04, u16 unk_06, then
 *    u8 unk_08..unk_0B, which the callback arm reads as one function pointer
 *    through a cast) and Unk45CC4P (0x28 POLY_FT4 with a
 *    `union { u16 h; u8 b[2]; }` at +0x0C/+0x14/+0x1C/+0x24). The vertex
 *    halfwords at +0x08/0A/10/12/18/1A/20/22 must be u16, read as
 *    `(u16) (x + 0x20) < 0x181` for the clip tests (lhu/addiu/andi/sltiu) and
 *    with `(s16)` casts for the min tests (lh).
 *  - Unk3DBD0SP needs unk_08/unk_0C/unk_10/unk_14 as the word/halfword union
 *    (stored `sw`, read back `lhu` -- the union is what makes that a real
 *    `lhu`), `u8 *unk_20` (byte-scaled OT base, `p->unk_20 + p->unk_C0 * 4`),
 *    u16 unk_50 and u16 unk_58 (read `lhu` then `(s16)` cast, so u16-with-cast
 *    is right and s16 is wrong), the unk_78/7A/7C, unk_80/82/84 and
 *    unk_88/8A/8C SVECTOR halfwords, s32 unk_90 beside the existing unk_94,
 *    and s32 unk_C0 (written `sw` and read back twice as `lw` -- the recompute
 *    is load-bearing). All fit inside existing pad_ ranges. `u8 unk_11C[0x14]`
 *    is NOT needed: 0x1F80011C and the other scratchpad addresses are reached
 *    as literal constants for the libgte calls, which is the load-bearing
 *    spelling.
 *  - UnkSlot needs u32 unk_08 (the element-list pointer, read as a word and
 *    cast), u32 unk_0C (written as a word to q->unk_04 and byte-patched at
 *    [3]) and the u16 run unk_10..unk_22.
 *  - CompMatrix and RotAverage4 are already prototyped in include/structs.h by
 *    func_800463EC, with `s32 *` primitive arguments. The local copies this
 *    file used to carry are DELETED: they are a hard compile error now
 *    (`conflicting types for 'RotAverage4'`). There is no
 *    s32-versus-u32 pointer disagreement to resolve -- the `s32 *` form is
 *    byte-identical here, and the `u32 *` spelling recorded earlier was
 *    simply obsolete.
 *
 * The shimmed typedefs below are layout-identical to what the real names must
 * become, so the codegen is unaffected, but this was never re-verified under
 * the real names in include/structs.h -- that check is still owed.
 *
 * SEARCHER SWEEP (2026-10-22): tools/decomp/search.py found 74 candidate
 * changes inside this function's body and compiled 75 variants; none improved
 * on 47 DIFF. That covers its eight transform families at the sites they
 * recognise, not every source shape. Worth knowing that the one candidate
 * which DID close the word count -- 436 to 437 -- did it by narrowing a u16
 * struct field to u8, which shifts every later field offset and took the
 * differing count to 143. The searcher now refuses that class of change and
 * reports it separately; the missing word is still missing.
 *
 * UNIT RENAMED: this function was in unk_14934 until a $gp cluster was carved
 * out of the middle of that unit and its tail became unk_176B8. The listing is
 * now asm/nonmatchings/unk_176B8/, which is what verify.py wants as its UNIT
 * argument.
 */

#define Unk3DBD0SP Unk3DBD0SP_orig
#define UnkSlot UnkSlot_orig
#define func_80045CC4 func_80045CC4_decl
#include "common.h"
#undef Unk3DBD0SP
#undef UnkSlot
#undef func_80045CC4

typedef struct UnkSlot {
    UnkElem *unk_00;
    s8       unk_04;
    u8       unk_05;
    s16      unk_06;
    u32      unk_08;
    u32      unk_0C;
    u16      unk_10;
    u16      unk_12;
    u16      unk_14;
    u16      unk_16;
    u16      unk_18;
    u16      unk_1A;
    u16      unk_1C;
    u16      unk_1E;
    u16      unk_20;
    u16      unk_22;
    u8       pad_24[0x4];
    u32      unk_28;
} UnkSlot;

typedef struct Unk3DBD0SP {
    s16         unk_00;
    s16         unk_02;
    u8          pad_04[0x4];
    Unk3DBD0W   unk_08;
    Unk3DBD0W   unk_0C;
    Unk3DBD0W   unk_10;
    Unk3DBD0W   unk_14;
    u8          pad_18[0x8];
    u8         *unk_20;
    u16         unk_24;
    u8          pad_26[0xA];
    Unk3DBD0W   unk_30;
    Unk3DBD0W   unk_34;
    s32         unk_38;
    u8          pad_3C[0x14];
    u16         unk_50;
    u8          pad_52[0x6];
    u16         unk_58;
    u8          pad_5A[0x16];
    s16         unk_70;
    s16         unk_72;
    s16         unk_74;
    u8          pad_76[0x2];
    s16         unk_78;
    s16         unk_7A;
    s16         unk_7C;
    u8          pad_7E[0x2];
    s16         unk_80;
    s16         unk_82;
    s16         unk_84;
    u8          pad_86[0x2];
    s16         unk_88;
    s16         unk_8A;
    s16         unk_8C;
    u8          pad_8E[0x2];
    s32         unk_90;
    s32         unk_94;
    u8          pad_98[0x28];
    s32         unk_C0;
    u8          pad_C4[0xC];
    u8          unk_D0[0x14];
    s32         unk_E4;
    s32         unk_E8;
    s32         unk_EC;
    u8          pad_F0[0x10];
    s16         unk_100;
    s16         unk_102;
    s16         unk_104;
    u8          pad_106[0x2];
    u16         unk_108;
    u16         unk_10A;
    u8          pad_10C[0x10];
    u8          unk_11C[0x14];
} Unk3DBD0SP;

/* The 0xC-byte element func_80045CC4 walks. */
typedef struct Unk45CC4E {
    u8  unk_00;
    u8  unk_01;
    u8  unk_02;
    u8  unk_03;
    u16 unk_04;
    u16 unk_06;
    u8  unk_08;
    u8  unk_09;
    u8  unk_0A;
    u8  unk_0B;
} Unk45CC4E;

/* The 0x28-byte primitive it emits. */
typedef union Unk45CC4UV {
    u16 h;
    u8  b[2];
} Unk45CC4UV;

typedef struct Unk45CC4P {
    u32         unk_00;
    u32         unk_04;
    u16         unk_08;
    u16         unk_0A;
    Unk45CC4UV  unk_0C;
    u16         unk_0E;
    u16         unk_10;
    u16         unk_12;
    Unk45CC4UV  unk_14;
    u16         unk_16;
    u16         unk_18;
    u16         unk_1A;
    Unk45CC4UV  unk_1C;
    u16         unk_1E;
    u16         unk_20;
    u16         unk_22;
    Unk45CC4UV  unk_24;
    u16         unk_26;
} Unk45CC4P;


void func_80045CC4(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3) {
    Unk3DBD0SP *p = (Unk3DBD0SP *) 0x1F800000;
    Unk45CC4E *e;
    Unk45CC4P *q;

    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_E4 = *(s16 *) ((u8 *) v + 2);
    p->unk_E8 = *(s16 *) ((u8 *) v + 6);
    p->unk_EC = *(s16 *) ((u8 *) v + 0xA);
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    q = *(Unk45CC4P **) (D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    PushMatrix();
    p->unk_108 = s->unk_20;
    p->unk_10A = s->unk_22;
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38 = 0x2000;
    p->unk_100 = s->unk_16;
    p->unk_104 = s->unk_1A;
    p->unk_102 = s->unk_18;
    RotMatrix((void *) 0x1F800100, (void *) 0x1F8000D0);
    ReadRotMatrix((void *) 0x1F80011C);
    CompMatrix((void *) 0x1F80011C, (void *) 0x1F8000D0, (void *) 0x1F800050);
    ScaleMatrix((void *) 0x1F800050, (void *) 0x1F800030);
    SetTransMatrix((void *) 0x1F800050);
    SetRotMatrix((void *) 0x1F800050);
    e = (Unk45CC4E *) s->unk_08;
    p->unk_24 = s->unk_14;
    for (;;) {
        if ((e->unk_00 & 0x20) == 0) {
            s32 u;
            s32 w;
            s16 a;
            s32 otz;
            s16 *p70;

            u = e->unk_08;
            p->unk_08.w = u;
            w = e->unk_0A;
            p->unk_10.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0) {
                p->unk_10.w = w - 1;
            }
            u = e->unk_09;
            p->unk_0C.w = u;
            w = e->unk_0B;
            p->unk_14.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0) {
                p->unk_14.w = w - 1;
            }
            if ((e->unk_00 ^ p->unk_24) & 1) {
                a = -(s8) ((u8 *) e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a - p->unk_10.h;
                p->unk_88 = a;
                p->unk_78 = a;
            } else {
                a = (s8) ((u8 *) e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a + p->unk_10.h;
                p->unk_88 = a;
                p->unk_78 = a;
            }
            if ((e->unk_00 ^ p->unk_24) & 2) {
                a = -(s8) ((u8 *) e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a - p->unk_14.h;
                p->unk_8A = a;
                p->unk_82 = a;
                p70 = &p->unk_70;
            } else {
                a = (s8) ((u8 *) e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a + p->unk_14.h;
                p->unk_8A = a;
                p->unk_82 = a;
                p70 = &p->unk_70;
            }
            otz = RotAverage4(p70, &p->unk_78, &p->unk_80, &p->unk_88,
                              (s32 *) &q->unk_08, (s32 *) &q->unk_10,
                              (s32 *) &q->unk_18, (s32 *) &q->unk_20,
                              &p->unk_90, &p->unk_94) - arg3 - 8;
            p->unk_C0 = otz;
            if ((u32) otz < 0x1E0) {
                u8 t;

                if ((((u16) (q->unk_08 + 0x20) < 0x181 &&
                      (u16) (q->unk_0A + 0x20) < 0x121) |
                     ((u16) (q->unk_10 + 0x20) < 0x181 &&
                      (u16) (q->unk_12 + 0x20) < 0x121) |
                     ((u16) (q->unk_18 + 0x20) < 0x181 &&
                      (u16) (q->unk_1A + 0x20) < 0x121) |
                     ((u16) (q->unk_20 + 0x20) < 0x181 &&
                      (u16) (q->unk_22 + 0x20) < 0x121)) != 0) {
                    ((u8 *) &q->unk_00)[3] = 9;
                    s->unk_14 &= 0x7FFF;
                    p->unk_10.w = p->unk_10.w + p->unk_08.w;
                    p->unk_14.w = p->unk_14.w + p->unk_0C.w;
                    p->unk_0C.w = p->unk_0C.w << 8;
                    p->unk_14.w = p->unk_14.w << 8;
                    if (p->unk_24 & 0x100) {
                        q->unk_0E = s->unk_12;
                    } else {
                        q->unk_0E = s->unk_12 + e->unk_06;
                    }
                    q->unk_0C.h = p->unk_0C.h + p->unk_08.h;
                    q->unk_14.h = p->unk_0C.h + p->unk_10.h;
                    if (s->unk_10 != 0) {
                        q->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
                    } else {
                        q->unk_16 = e->unk_04;
                    }
                    q->unk_1C.h = p->unk_14.h + p->unk_08.h;
                    q->unk_24.h = p->unk_14.h + p->unk_10.h;
                    if ((s16) p->unk_50 >= 0x1800) {
                        t = q->unk_24.b[0];
                        q->unk_24.b[0] = t + 0xFF;
                        q->unk_14.b[0] = t;
                    }
                    if ((s16) p->unk_58 >= 0x1800) {
                        t = q->unk_24.b[1];
                        q->unk_24.b[1] = t + 0xFF;
                        q->unk_1C.b[1] = t;
                    }
                    if ((s16) q->unk_08 > (s16) q->unk_20) {
                        q->unk_14.b[0] = q->unk_14.b[0] - 1;
                        q->unk_24.b[0] = q->unk_24.b[0] - 1;
                    }
                    if ((s16) q->unk_0A > (s16) q->unk_22) {
                        q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                        q->unk_24.b[1] = q->unk_24.b[1] - 1;
                    }
                    t = e->unk_01;
                    ((u8 *) &s->unk_0C)[3] = t;
                    if (p->unk_24 & 8) {
                        if (p->unk_24 & 4) {
                            ((u8 *) &s->unk_0C)[3] = t | 2;
                        } else {
                            ((u8 *) &s->unk_0C)[3] = t & 0xFD;
                        }
                    }
                    q->unk_04 = s->unk_0C;
                    q->unk_00 = (*(u32 *) (p->unk_20 + p->unk_C0 * 4) & 0xFFFFFF) |
                        (q->unk_00 & 0xFF000000);
                    *(u32 *) (p->unk_20 + p->unk_C0 * 4) =
                        (*(u32 *) (p->unk_20 + p->unk_C0 * 4) & 0xFF000000) |
                        ((u32) q & 0xFFFFFF);
                    q++;
                }
            }
        } else {
            void (*f)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *);

            f = *(void (**)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *)) &e->unk_08;
            if (f != NULL) {
                f(arg0, v, s, e);
            }
        }
        if ((s8) e->unk_00 < 0) {
            break;
        }
        e++;
    }
    PopMatrix();
    *(u32 *) (D_80083160.unk_00 + 0x8D0) = (u32) q;
}
