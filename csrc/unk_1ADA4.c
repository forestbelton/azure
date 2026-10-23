/* The remainder of unk_19434 after the gprel_1AB38 carve-out. Named for its
 * start offset. */
/* The remainder of unk_1811C after the gprel_18B40 carve-out. Named for its
 * start offset. */
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

void func_800475A4(Unk800777AC *p, u32 *q) {
    u16 *h = (u16 *) p;

    for (;;) {
        if ((p->unk_00 & 0x60) == 0) {
            h--;
            func_80047338(p, *q, *h);
        }
        if (p->unk_00 & 0x80) {
            break;
        }
        p++;
    }
}

void func_8004761C(Unk800777AC *p, u32 *q) {
    u16 *h = (u16 *) p;

    for (;;) {
        if ((p->unk_00 & 0x60) == 0) {
            h--;
            func_80047468(p, *q, *h);
        }
        if (p->unk_00 & 0x80) {
            break;
        }
        p++;
    }
}

void func_80047694(UnkSlot *s) {
    if (s->unk_14 & 0x400) {
        func_800482C8((Unk800777AC *) s->unk_08, (u32 *) s->unk_28);
    } else if (D_80082E60.unk_0A == 1) {
        func_8004761C((Unk800777AC *) s->unk_08, (u32 *) s->unk_28);
    } else {
        func_800475A4((Unk800777AC *) s->unk_08, (u32 *) s->unk_28);
    }
}
