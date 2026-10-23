/* A carved-out translation unit: these two consecutive functions reach
 * D_80081510 $gp-relatively and nothing else does, so they were one original
 * translation unit. gas is left at -G8 here rather than maspsx's forced -G0
 * (MASPSXFLAGS_G8 in the Makefile); no window definition and no linker
 * placement needed -- see docs/addressing-and-translation-units.md.
 *
 * Audited before carving: the only symbol reached absolutely is D_80083160,
 * already `extern Unk80083160` at 0x1F2 bytes and therefore compiler-split, so
 * -G8 does not touch it. (At -G8 gas makes any small UNDEFINED symbol
 * $gp-relative, so every absolutely-reached symbol in a carved unit must be
 * checked.) */
/* Another piece of the original unk_14934 chain, carved off when the
 * gprel_17B7C $gp cluster was taken out of the middle of unk_176B8. Named for
 * its start offset. See docs/addressing-and-translation-units.md.
 */
/* The tail of the original unk_14934 split, carved off when gprel_17504 was
 * taken out of the middle of it: a $gp cluster cannot be compiled with the
 * same -G as its neighbours, so the unit had to be cut in three. Named for its
 * start offset, like the other provisional units. See
 * docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u32, u32);
void func_8003F6D4(s32, s32, s32 *, s32);
void func_80040B88(void);
void func_80042984(u8 *);
void func_80041AB0(void);
void func_80041AE4(void);
void func_80043CD0(void);
void func_80043D04(void);
s32  func_80042518(s8 *, s8);
UnkNode *func_8003FC64(s32);
void func_80043FD0(Unk44144 *);
s32  func_8003F794(s16, s16);
void func_80040454(void);

/* Links the node into one of the 0x20 tag slots at D_80083360/D_800833E0,
 * pushing any existing head down, or claims a free slot. Returns 0 only when
 * every slot is taken.
 *
 * Three shapes are load-bearing:
 *  - `D_800833E0[i] = (u32) n;` is duplicated into both arms of the null
 *    check. That gives the store its own basic block instead of putting it in
 *    the join block, so sched1 never has it and the `return 1` constant in one
 *    ready list, where the hazard heuristic always hoists the constant past
 *    it. jump2's cross-jumping then merges the two copies back and plants the
 *    join label in front of the store, which is why .L80044964 sits before it
 *    and the word count is unchanged.
 *  - the free-slot bookkeeping is `if (first == -1) { first = i; } last = i;`
 *    in that order. With `last = i` first, cse folds the second assignment to
 *    a copy of last's register; in this order the `last = i` insn is instead
 *    stolen into the bne's delay slot. The test has to be against -1, not
 *    `< 0`: the target compares against a loop-hoisted -1 in a register.
 *  - `return 0` is the last statement, so its block lands just before the
 *    epilogue and the test is a single bltz. An early return costs a bgez, a
 *    `j` and a move.
 *
 * The two tails of the D_8006E7B4 walk are genuinely duplicated in the target
 * -- they differ only in which index they use -- so they are written out
 * twice here as well. */

/* Walks the node list, drawing each slot through func_800453E0 and stopping
 * early if func_80045310 reports the OT is full.
 *
 * This is the fifth member of a family: func_80044BB0, func_80044C54,
 * func_80045C34 and func_80046398 are the same node walk with a different
 * per-slot callee, and func_80045C34 is the exact skeleton. **Start there if
 * you are writing anything else in this unit** -- that is worth more than any
 * lever.
 *
 * Two things in the listing that look like source and are not: there is only
 * ONE `return 0` worth of `addu $v0,$zero,$zero` in the source -- jump2
 * cross-jumped the two return tails and reorg then stole the survivor into the
 * bnez's delay slot and retargeted the branch past it, which is why the branch
 * appears to skip the `sw $zero, %gp_rel(D_80081510)` while still returning 0.
 * And `*(u32 *) 0x1F80013C = 0;` is a single-use constant address, so it
 * collapses to the two-word absolute form (matching-levers section 6). */
s32 func_80045340(u8 *p, UnkVec *v, UnkSlot *s) {
    D_80081510 = 0;
    *(u32 *) 0x1F80013C = 0;
    for (;;) {
        UnkNode *n;

        if (!(s->unk_14 & 0x80)) {
            func_800453E0(p, v, s, s->unk_06);
            if (func_80045310(*(u32 *) (D_80083160.unk_00 + 0x8D0)) != 0) {
                return 0;
            }
        }
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            D_80081510 = 0;
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/gprel_18B40", func_800453E0);
