/* func_80056654 (unk_2879C) -- 107 of 107 words, 21 DIFF. NOT compiled.
 *
 * Builds one sprite primitive in D_80084918 from the D_80084960 entry the
 * sprite's +0x06 index selects, then calls func_8005F134 to submit it, and
 * finally clears +0x30/+0x3C when func_8005EB78 says the texture is done.
 *
 * WHAT IS PAID FOR (116 words on the first compile, now 107)
 *
 * * `func_800565D8` is declared returning `s32` HERE, even though it really
 *   returns a sign-extended `s16` (see its own near-miss file). The listing
 *   adds $v0 straight into the accumulator with no `sll`/`sra`, so the
 *   caller's prototype is the wide one. Two words.
 *
 * * D_80084918's base is a `u8 *` local assigned INSIDE the `if`, after the
 *   call. At function scope it is live across the call, takes $s2, and costs
 *   its save and restore. Five words.
 *
 * * The +0x16 store goes through that local, NOT through
 *   `*(s16 *) (D_80084918 + 0x16)`. The latter makes gcc materialise
 *   `D_80084918 + 0x16` as the shared base and then re-bias it for
 *   func_8005F134's argument. One word.
 *
 * * The D_80073740 lookup is read into a LOCAL early and stored to +0x00
 *   late. As one statement its load sits at the top of the group and its
 *   load-delay slot goes empty; split, the accumulator arithmetic fills it.
 *   One word.
 *
 * * `x` is refined in place (`x = (((s16) x >> 7) << 8) | (x & 0x7F);`) with
 *   the shift term FIRST, which is the order the listing evaluates them in.
 *
 * * The first statement of the block is `x = *(u16 *) (p + 0x3C);`, before the
 *   +0x70 store, which is what fills that store's load-delay slot.
 *
 * WHAT IS LEFT: 21 words, all schedule and allocation in the primitive-build
 * block. The target interleaves the D_80073740 address chain with the
 * `(p[0xC] << 7)` read and defers the table load until the accumulator work
 * can fill its delay slot; this one groups them differently. `p` and `x` also
 * hold each other's callee-saved registers ($s1/$s0 against the target's
 * $s1/$s0 the other way round in two places). No shape findings remain --
 * `tools/decomp/sched.py` is the tool to point at it next, per section 8 of
 * docs/matching-levers.md.
 *
 * Also note the struct: Unk80084960 gained unk_08, unk_3A, unk_50 and unk_52
 * out of its pad runs for this function. The offsets are unchanged, so every
 * other user of the table is unaffected and `make check` stays identical.
 *
 * AUDIT 2026-10-27 -- pseudo 80 has a DOUBLED live length, and
 * it reports roughly twice its real range in this 107-word function.
 * `update_equiv_regs` (local-alloc.c) does `reg_live_length *= 2` for a once-set
 * pseudo whose init carries a constant REG_EQUAL note, and global-alloc reads the
 * doubled value.
 *
 * READ THAT CORRECTLY: global-alloc really does use the doubled number, so the
 * ORDER `--greg` prints is the order gcc chose -- it is not wrong about the
 * assignment. What is wrong is treating those priorities as a measure of the real
 * live range, and the MARGINS table's `live <= N` instruction, which is in DOUBLED
 * units: shortening the real range by k moves the stored value by 2k, so the
 * instruction costs half as many real insns as it reads.
 *
 * AND IT IS A LEVER. A pseudo that is NOT once-set escapes the doubling entirely
 * (a large-constant pointer does it by accident: its pre-sched lui/ori split
 * leaves reg_n_sets == 2 before local-alloc runs). So giving 80 a second
 * set that survives cse DOUBLES its priority at zero instruction cost, and
 * then 80 would pass 93 (0.905 against 0.659).
 * Worth trying before any reference arithmetic. See matching-levers section 3.
 */

#include "common.h"

s32 func_800565D8(u8 *, u32);
void func_8005F134(void *);
s32 func_8005EB78(u32);

void func_80056654(u8 *p, s32 f)
{
    Unk80084960 *e = &D_80084960[*(u16 *)(p + 6)];
    u8 *b;
    s32 x;
    s32 y;
    u32 z;

    if (*(s32 *)(p + 0x4C) != 0 || *(s32 *)(p + 0x30) != 0 || e->unk_08 != 0 ||
        e->unk_50 != 0 || e->unk_1C != *(s32 *)(p + 0x70) || f != 0)
    {
        x = *(u16 *)(p + 0x3C);
        *(s32 *)(p + 0x70) = e->unk_1C;
        x = x + (e->unk_3A + e->unk_52);
        y = func_800565D8(p, e->unk_1C);
        b = D_80084918;
        *(u32 *)(b + 4) = 0x60;
        z = D_80073740[*(u16 *)p];
        y = (*(u16 *)(p + 0xC) << 7) + y;
        x = x + y;
        x = (((s16)x >> 7) << 8) | (x & 0x7F);
        *(u32 *)D_80084918 = z;
        if (p[0x23] != 0)
        {
            *(s16 *)(b + 0x18) = ((p[0x22] - 1) << 8) | (0x7F - p[0x23]);
        }
        else
        {
            *(s16 *)(b + 0x18) = p[0x23] | (p[0x22] << 8);
        }
        *(s16 *)(b + 0x16) = x;
        func_8005F134(D_80084918);
    }
    if (*(s32 *)(p + 0x30) != 0 && func_8005EB78(D_80073740[*(u16 *)p]) == 0)
    {
        *(s32 *)(p + 0x30) = 0;
        *(s32 *)(p + 0x3C) = 0;
    }
}
