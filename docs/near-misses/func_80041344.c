/* func_80041344 (unk_14934) -- 145 of 145 words, 2 DIFF (was 67). NOT
 * compiled, and not landable alone: it dispatches through jtbl_8002D630, so it
 * waits on the splat/linker change in docs/open-matching-problems.md, and all
 * three unk_14934 switch functions have to land together. verify.py masks the
 * two relocated halfwords that reference the table, so 145/145 is necessary
 * and not sufficient -- only `make check` can see the table land at the wrong
 * address.
 *
 * The source below is VARIANT A, 2 DIFF. Both differing words are the switch
 * index: this writes `lhu $v1` / `addiu $v1,$v1,-1` where the target has
 * `lhu $v0` / `addiu $v1,$v0,-0x1`. That is a deliberate price -- splitting the
 * index into two sets is what buys the allocation priority that puts every
 * other register right, including `e` in $t0. So the shape
 * `op = *(u16 *) p; op -= 1;` is provably NOT what the original wrote; the
 * original got those references from a role not yet identified.
 *
 * VARIANT B is 4 DIFF with every shape defensible and nothing paid for in
 * words: `op = *(u16 *) p - 1;` and case 7's counter in its own variable. Its
 * four words are that counter, in $a2 where the target has $a3. Use B as the
 * base for another attempt and A as the proof that the rest is right.
 *
 * THE BIG STRUCTURAL FINDING: the switch index, the flag value, the loop bounds
 * and (in A) case 9's pointer are all ONE source variable. Writing
 * `op = *(u16 *) p - 1;` with the arms renumbered `case 0 ... case 8` makes
 * `expand_end_case` skip its own MINUS, because minval is 0 -- so `sltiu` and
 * `sll` read the user's pseudo and the index IS a source variable rather than a
 * compiler temp. That matters twice: it gives the index the flag value's
 * reference count, and it makes the index conflict with `e` (in case 6 the
 * `lhu 0xC` is the arm's first insn and `e` dies at its fourth), which is the
 * only conflict in the function capable of pushing `e` off $v1.
 *
 * This supersedes two earlier write-ups, both of which were wrong about the
 * index: the original note here had it block-local and assigned by local-alloc,
 * and func_80041588's correction had it as an ordinary global allocno that
 * "can never be allocated before the record pointer". It can -- once it is the
 * same VARIABLE as the flags, it carries their references.
 *
 * TRANSFERRED FROM THE SIBLING func_80041588, applied wholesale as the first
 * step: 67 DIFF -> 26, worth 41 words in one go. That was its items 1, 2, 3, 5
 * and 11 together -- the flag value as a function-scope s32 loaded then OR'd
 * in place, function-scope q/w rather than arm-locals (29 words here), one
 * counter shared across arms with a second for the inner loop (up to 29 words),
 * a separate `Unk41344 *e` inside the loop body with word loads off it and
 * halfword loads off `p`, and func_8003F80C's fourth argument as s16. Case 8's
 * single walking pointer also reproduces its 19 words exactly, including the
 * `addu $a1,$a0,$zero` / `addiu $a1,$a1,2` pair -- which resolves the old
 * note's "case 8's induction-variable structure, about 10 words".
 *
 * DONE DIFFERENTLY FROM THE SIBLING:
 *  - case 9 gets its own pointer; reusing `q` costs 3 words. In variant A it is
 *    folded into `op` instead, to buy that variable references.
 *  - case 9 does NOT advance its pointer, and there is a `D_80080A8C = 0` tail,
 *    so `default: return;` stays.
 *  - no chained store and no two-sets-of-base shape: neither appears here.
 *    `base` is a parameter and `p = base` already gives the target's
 *    `addu $s1,$a0,$zero` / `addu $s0,$s1,$zero` pair for free.
 *  - the counter grouping in B is (B,A,C,A), case 7's counter on its own. The
 *    target's registers REQUIRE the sibling's (A,A,B,A) -- independently
 *    confirmed from the conflict lists -- but that grouping loses the priority
 *    comparison below, which is exactly what variant A pays two words to win.
 *
 * WHAT IS LEFT, measured to the reference. The whole remaining permutation is
 * one priority comparison: the merged variable at 52 refs / 56 insns = 4.643
 * against the shared counter's 39 / 41 = 4.756. Whoever is allocated first has
 * its preferences collected into the other's avoid set. The thresholds are
 * exact -- the merged variable needs >= 54 references at length 56, or length
 * <= 54 at 52 references. The target has exactly eleven $v1-writing insns and
 * variant A already owns all of them, so the original's extra reference comes
 * from a role not yet found. Two priced ways to buy it: `size` in the same
 * variable (3 words, 3 DIFF) or splitting the index (2 words, 2 DIFF).
 *
 * MEASURED INERT, so do not retry: `e` hoisted to function scope; `size` as
 * s32; declaring `op` last; the fourth-argument prototype as s32 with the cast
 * kept, or s16 with no cast; splitting case 8's bound into its own variable
 * (35, and the counter sweep over that base is 32-39); the `(s16)` extension
 * routed through the shared counter (37) or a fresh function-scope variable
 * (24, eliminated by combine).
 *
 * THE COUNTER SWEEP, DIFF per grouping of (case7 / case8-outer / case8-inner /
 * case9), on the variant-A base: {c7}+{c8o,c9}+{c8i} = 4 and
 * {c7,c8i}+{c8o,c9} = 4 are the best; {c7,c8o,c9}+{c8i} = 24 (the target's own
 * grouping); {c7,c8o}+{c8i,c9} and {c7}+{c8o}+{c8i,c9} = 33; everything else
 * 24-27.
 *
 * include/structs.h, when this lands: `Unk41344` exactly as func_80041588's
 * near-miss file defines it -- same five fields, same dual word/halfword
 * reading, nothing missing and nothing unused, so add it once and use it from
 * both. (func_80042BDC does NOT share it: that function is not a
 * command-stream walker and uses the existing Unk42710.) Also
 * `void *memcpy(void *, const void *, u32);` beside bzero, and
 * `void func_8003F80C(u32, s32, s32, s16);`. The forward declaration in
 * csrc/unk_14934.c is `void func_80041344(u32, u32);` and must become
 * `void func_80041344(u8 *, u32 *);` -- until then a scratch file needs the
 * `#define`/`#undef` dance around common.h, as below.
 */

#define func_80041344 func_80041344_decl_clash
#include "common.h"
#undef func_80041344

typedef struct Unk41344 {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
} Unk41344;

void *memcpy(void *, const void *, u32);
void func_8003F80C(u32, s32, s32, s16);

void func_80041344(u8 *base, u32 *arg1) {
    u8 *p = base;
    u16 size;
    s32 op;
    s32 i;
    s32 j;
    u16 *q;
    u16 *w;

    for (;;) {
        Unk41344 *e = (Unk41344 *) p;

        op = *(u16 *) p;
        op -= 1;
        switch (op) {
        case 0:
            func_8004068C(base + e->unk_04, (u8 *) arg1);
            LoadImage(p + 8, arg1);
            DrawSync(0);
            break;
        case 4:
            LoadImage(p + 8, (u32 *) (base + e->unk_04));
            DrawSync(0);
            break;
        case 1:
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), *(s16 *) (p + 0xC));
            DrawSync(0);
            break;
        case 5:
            op = *(u16 *) (p + 0xC);
            op |= 2;
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), (s16) op);
            DrawSync(0);
            break;
        case 6:
            q = (u16 *) (base + e->unk_04);
            op = *(u16 *) (p + 0xA) << 4;
            for (i = 1; i < op; i++) {
                q[i] |= 0x8000;
            }
            op = *(u16 *) (p + 0xC);
            op |= 4;
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) op);
            DrawSync(0);
            break;
        case 7:
            q = (u16 *) (base + e->unk_04);
            w = q;
            op = *(u16 *) (p + 0xA);
            for (i = 0; i < op; i++) {
                w++;
                for (j = 1; j < 16; j++) {
                    *w |= 0x8000;
                    w++;
                }
            }
            op = *(u16 *) (p + 0xC);
            op |= 2;
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) op);
            DrawSync(0);
            break;
        case 2:
            func_8004068C(base + e->unk_04, (u8 *) (D_80080A8C + e->unk_08));
            break;
        case 3:
            memcpy((void *) D_80080A8C, base + e->unk_04,
                   e->unk_0C + e->unk_08);
            break;
        case 8:
            op = (s32) (base + e->unk_04);
            for (i = *(u16 *) (p + 8); i > 0; i--) {
                ((u32 *) op)[1] = (u32) (base + ((u32 *) op)[1]);
            }
            break;
        default:
            return;
        }
        size = *(u16 *) (p + 2);
        if (size == 0) {
            break;
        }
        p += size;
    }
    D_80080A8C = 0;
}
