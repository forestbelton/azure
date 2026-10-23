/* A carved-out translation unit: these two consecutive functions own the two
 * texture-atlas cursors -- D_80081518/1A/1C/1E and D_80081520/22/24/26 -- which
 * nothing else reaches, so they were one original translation unit. Built at
 * -G8 (MASPSXFLAGS_G8); no window definition, no linker placement.
 *
 * They are near-twins: the same cursor bookkeeping with the other quartet, two
 * different thresholds and four constant biases. The second was derived from
 * the first by substitution and matched on the first compile.
 *
 * Audited before carving: the only symbol reached absolutely is D_80083164,
 * which had no declaration at all and now has an incomplete-array one in
 * include/structs.h -- it sits inside the $gp window, so a scalar declaration
 * would wrongly go $gp-relative at -G8. The eight u16 cursor objects are
 * (0, 8] by declared size and come out GPREL16 as wanted.
 *
 * KEEP D_80081518, D_8008151A, D_80081520 and D_80081522 declared as one-field
 * structs. That spelling is what sets MEM_IN_STRUCT_P on the four sites that
 * need it (it is what keeps the target's four delay-slot nops), while the one
 * site that must be non-struct is spelled `*(u16 *) &SYM`. Retyping them to
 * plain u16 scalars would flip all five sites at once. */
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

/* Allocates a slot in the first texture atlas: D_80081518 is the column x,
 * D_8008151A the row y, D_8008151C the current column width and D_8008151E a
 * generation stamp compared against D_80083164. Builds a four-halfword RECT on
 * the stack and hands it to func_80047270.
 *
 * Four load-bearing shapes, each measured by reverting it:
 *  - `u8 uw = w;` placed BETWEEN the first and second `if`. It buys two things
 *    at once. The `sltu` comes from shorten_compare: D_8008151C is u16 and
 *    `uw` u8, both unsigned and narrower than int, so the comparison is
 *    unsigned -- s8, s16, s32 or u32, or comparing `w` directly, all give
 *    `slt` (2 words). And the `addu $v1,$a0,$zero` copy in the bnez's delay
 *    slot is the narrow local's truncation folding to the identity: gcc proves
 *    w's bits, so `(and w 0xFF)` becomes a bare copy that survives as its own
 *    pseudo, where a u32/s32 local has no truncation and cse propagates it
 *    away. PLACEMENT is the third requirement and the expensive one to find:
 *    the copy must be DEFINED in the test block and USED in the join block.
 *    Before the first `if` it exists in the wrong block (+1 word, 57 DIFF);
 *    inside the join block combine substitutes it away (2 DIFF). Only the
 *    middle block leaves it as the last insn before the `bnez`, where
 *    fill_simple_delay_slots takes it for free.
 *  - `((u8 **) src)[3]`, not `*(u8 **) (src + 0xC)` (1 word). The index
 *    spelling sets MEM_IN_STRUCT_P where the cast arithmetic does not, which
 *    is what lets sched1 hoist the `lw` and the `addiu $a1,$sp,0x10` above the
 *    store and fill a load-delay slot. See matching-levers section 5.
 *  - `*(u16 *) &D_8008151A += rect[3];` on the FINAL store only (1 word). The
 *    other four sites use the struct spelling and must: the hoist above needs
 *    the store to be the non-struct side at a non-varying address. Section 6's
 *    "mix the two spellings for one object", applied per access site.
 *  - `rect[3]` read back off the stack rather than reusing `h` (1 word, 24
 *    DIFF) -- section 4. The readback is `lhu` although rect is s16[], because
 *    the u16 destination makes it a HImode consumer and combine narrows the
 *    load; that is not evidence of an unsigned array.
 *
 * `s32 w` with `u8 uw` is also what gives the target's opposite signedness in
 * the two tests: `D_80081518 + w >= 0x40` is a signed `slti` while
 * `D_8008151C < uw` is `sltu`. `u32 w` costs 1 word. */
void func_80047338(Unk800777AC *p, u32 src, u32 arg2) {
    s16 rect[4];
    s32 w;
    s32 h;
    u8 uw;

    if (D_80083164[0] != D_8008151E) {
        D_8008151C = 0;
        D_8008151A.unk_00 = 0;
        D_80081518.unk_00 = 0;
        D_8008151E = D_80083164[0];
    }
    w = (p->unk_0A + 3) / 4;
    rect[2] = w;
    h = p->unk_0B + 1;
    rect[3] = h;
    if (D_8008151A.unk_00 + h >= 0x100) {
        D_80081518.unk_00 += D_8008151C;
        D_8008151A.unk_00 = 0;
    }
    uw = w;
    if (D_80081518.unk_00 + w >= 0x40) {
        D_8008151C = 0;
        D_8008151A.unk_00 = 0;
        D_80081518.unk_00 = 0;
    }
    if (D_8008151C < uw) {
        D_8008151C = w;
    }
    rect[0] = D_80081518.unk_00 + ((p->unk_04 << 6) & 0x3C0);
    rect[1] = D_8008151A.unk_00 + ((p->unk_04 << 4) & 0x100);
    p->unk_08 = D_80081518.unk_00 << 2;
    p->unk_09 = D_8008151A.unk_00;
    *(u16 *) &D_8008151A += rect[3];
    func_80047270(((u8 **) src)[3] + arg2, rect);
}

/* The second atlas's cursor: identical in shape to func_80047338, with the
 * other quartet, thresholds of 0x80/0x20 and four constant biases. Every shape
 * noted above applies here unchanged. */
void func_80047468(Unk800777AC *p, u32 src, u32 arg2) {
    s16 rect[4];
    s32 w;
    s32 h;
    u8 uw;

    if (D_80083164[0] != D_80081526) {
        D_80081524 = 0;
        D_80081522.unk_00 = 0;
        D_80081520.unk_00 = 0;
        D_80081526 = D_80083164[0];
    }
    w = (p->unk_0A + 3) / 4;
    rect[2] = w;
    h = p->unk_0B + 1;
    rect[3] = h;
    if (D_80081522.unk_00 + h >= 0x80) {
        D_80081520.unk_00 += D_80081524;
        D_80081522.unk_00 = 0;
    }
    uw = w;
    if (D_80081520.unk_00 + w >= 0x20) {
        D_80081524 = 0;
        D_80081522.unk_00 = 0;
        D_80081520.unk_00 = 0;
    }
    if (D_80081524 < uw) {
        D_80081524 = w;
    }
    rect[0] = D_80081520.unk_00 + ((p->unk_04 << 6) & 0x3C0) + 0x20;
    rect[1] = D_80081522.unk_00 + ((p->unk_04 << 4) & 0x100) + 0x80;
    p->unk_08 = (D_80081520.unk_00 << 2) - 0x80;
    p->unk_09 = D_80081522.unk_00 + 0x80;
    *(u16 *) &D_80081522 += rect[3];
    func_80047270(((u8 **) src)[3] + arg2, rect);
}
