#include "common.h"

/* The first fourteen functions of the old unk_72A8, carved off for ONE WORD.
   func_80033D44 and func_80033D54 -- the last two functions in this file --
   reach D_8008099C $gp-relatively, while func_80034EF8 and
   func_8003C0C8, further down the old unit, reach the same word ABSOLUTELY
   and are matched that way. maspsx's size record is file-scope, so no single
   translation unit can give one symbol both forms; the __asm__-alias trick
   that gives one symbol two spellings elsewhere in this project does not lift
   it either (measured 2026-10-27, see the note at the top of csrc/unk_72A8.c).
   So the unit is cut immediately after func_80033D54, which is also the
   earliest cut that separates the two groups, and this piece asks for the
   size record the stubs need.

   This unit stays at the project default -G0, like gprel_11B4C and unlike the
   other gprel_* units: the one access it wants is asked for by the size
   record below, and nothing here reaches a small object absolutely.

   The twelve functions above them came along because they are on this
   side of the cut, not because they need anything from it -- none of them
   touches D_8008099C or any of the old unit's other size-recorded symbols,
   which is why the carve moves no matched code. See
   docs/addressing-and-translation-units.md. */
__asm__(".sdata\n\t.size D_8008099C,4\n\t.text");

/* The bitmap set, clear and test on the out-of-image words at 0x80012D70,
   whose base 0x80010000 has no low half, so 0x2D70 stays in the load and store
   displacements. The base is assigned AFTER the two guards so its `lui` is the
   first insn of the following block and reorg lifts it into the second guard's
   delay slot; at the top of the function the index chain is emitted first and
   the `lui` lands after the `sll` (3 DIFF). Adding the index to the base in a
   separate statement (`p += ...`) also hands the `addu`'s destination to the
   pointer's quantity instead of the index chain's, which permutes $a0/$v0
   through the whole function (7 DIFF). */
void func_80033AA8(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return;
    }
    if (k == 1) {
        return;
    }
    p = (u8 *) 0x80010000;
    *(u32 *) (p + (k >> 5) * 4 + 0x2D70) |= 1 << (id & 0x1F);
}

/* The bitmap CLEAR, func_80033AA8's twin with `&= ~` for `|=`.

   ITS PARAMETER IS `u16`, and that is the whole difference from the twin: the
   listing's `andi $a0, $v1, 0xFFFF` is the PARAMETER's own truncation, which
   leaves the raw argument alive in $v1 for the `& 0x1F` and lets the `sllv`
   take the constant 1 out of $a1 before the `lw`. Declared `s32 id` with the
   mask written in the source, the raw value and the constant 1 hold each
   other's registers and six words differ. Nothing is lost by narrowing: both
   spellings use only the low sixteen bits of the argument.

   It is DEFINED UNDER AN __asm__ ALIAS so that this unit's own
   `void func_80033AE8(s32);` declarations and their call sites keep the
   32-bit view they are matched against -- func_80039258's
   `func_80033AE8((s16) v)` needs the `sll`/`sra` pair that a u16 prototype
   in scope would replace with a truncation. In the original sources the
   definition and those callers were in different files; the alias is how a
   carved unit says that (see docs/carved-sweep.md). */
void func_80033AE8w(u16) __asm__("func_80033AE8");

void func_80033AE8w(u16 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return;
    }
    if (k == 1) {
        return;
    }
    p = (u8 *) 0x80010000;
    *(u32 *) (p + (k >> 5) * 4 + 0x2D70) &= ~(1 << (id & 0x1F));
}

s32 func_80033B2C(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return 0;
    }
    if (k == 1) {
        return 1;
    }
    p = (u8 *) 0x80010000;
    return (*(u32 *) (p + (k >> 5) * 4 + 0x2D70) & (1 << (id & 0x1F))) != 0;
}

void func_80033AE8(s32);

void func_80033B78(s16 a) {
    func_80033AE8(a);
}

void func_80033AA8(s32);

void func_80033B9C(s16 a) {
    func_80033AA8(a);
}

s32 func_80033B2C(s32);

void func_80033BC0(s16 a) {
    func_80033B2C(a);
}

u8 *func_80033BE4(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (p[0] == 0) {
            return p;
        }
        p += 0x64;
    }
    return 0;
}

void func_80033C1C(u8 *p, u32 a) {
    *(u32 *) (p + 0x00) = 1;
    *(u32 *) (p + 0x04) = a;
    *(u16 *) (p + 0x08) = 0;
    *(u16 *) (p + 0x0A) = 0;
    *(u16 *) (p + 0x0C) = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    *(u32 *) (p + 0x18) = 0;
    *(u32 *) (p + 0x20) = 0x1000;
    *(u32 *) (p + 0x24) = 0x1000;
    *(u32 *) (p + 0x28) = 0x1000;
    *(u32 *) (p + 0x30) = 0;
    *(u32 *) (p + 0x34) = 0;
    *(u32 *) (p + 0x38) = 0;
    *(u32 *) (p + 0x40) = 0;
    *(u32 *) (p + 0x44) = 0;
    *(u32 *) (p + 0x48) = 0;
    *(u32 *) (p + 0x4C) = 0;
    *(u32 *) (p + 0x50) = 0;
    *(u32 *) (p + 0x54) = 0;
    *(u32 *) (p + 0x58) = 0;
    *(u32 *) (p + 0x5C) = 0;
    *(u32 *) (p + 0x60) = 0;
}

void func_80033C84(u32 *p) {
    *p = 0;
}

void func_80033C8C(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        func_80033C84((u32 *) p);
        p += 0x64;
    }
}

void func_80033CD8(u8 *p, u32 tag) {
    if (*(u32 *) (p - 0xC) == 0) {
        func_8004491C((UnkNode *) (p - 0x20), tag);
    }
}

void func_80033D08(u8 *p) {
    if (*(u32 *) (p - 0xC) != 0) {
        func_80044A50((UnkNode *) (p - 0x20));
    }
}

s32 func_80033D38(u32 *p) {
    return p[-3] != 0;
}

/* Records the argument in the $gp word and hands it back. The copy to $v0
   ahead of the store is the return value; the two-statement spelling
   (`D_8008099C = a; return a;`) gives the same four words, measured. First
   draft, 4 of 4 -- the first thing the carve bought. */
s32 func_80033D44(s32 a)
{
    return D_8008099C = a;
}

INCLUDE_ASM("asm/nonmatchings/gprel_72A8", func_80033D54);
