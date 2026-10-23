/* A carved-out translation unit: these functions reach D_80080AF3, D_80080AFC,
 * D_80080B00 and D_80081508 $gp-relatively, so gas is left at -G8 here rather
 * than maspsx's forced -G0 (MASPSXFLAGS_G8 in the Makefile). No window
 * definition and no linker placement -- see gprel_17504 and
 * docs/addressing-and-translation-units.md.
 *
 * The two decompiled functions in the middle of the run were audited before
 * carving, which is the step that matters: at -G8 gas makes any small
 * UNDEFINED symbol $gp-relative, so every symbol reached absolutely must
 * either sit outside gp +/- 0x8000 or be declared so cc1psx compiler-splits
 * the address itself. func_800445E0 reaches no global at all; func_80044724
 * reaches D_80083178 (a 0x94+ struct) and D_80083CE8 (an array of 4), both
 * split by their declared size. func_8004491C's D_80083360/D_800833E0 are
 * u32[0x20] and would also have been safe, but it is left outside the carve
 * because it needs no $gp access.
 *
 * Still to watch when the stubs here are decompiled: func_8004437C reaches
 * D_80080AF4 and func_8004450C reaches D_80081480 and D_8008148C, all small
 * scalars INSIDE the $gp window that the target reaches absolutely. They are
 * harmless while those functions are asm -- the listing carries its own
 * %hi/%lo and -G does not touch it -- but each will need an incomplete-array
 * declaration, as D_80082E6E did.
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
void func_80047338(Unk800777AC *, u32, u16);
void func_80047468(Unk800777AC *, u32, u16);
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

INCLUDE_ASM("asm/nonmatchings/gprel_17B7C", func_8004437C);

INCLUDE_ASM("asm/nonmatchings/gprel_17B7C", func_8004450C);

u32 func_800445E0(void) {
    Unk8006CE34 *p = func_8003F534();
    u32 t = (p->unk_08 & 0xFFFFFF) - 0x20;

    return t + p->unk_04;
}

INCLUDE_ASM("asm/nonmatchings/gprel_17B7C", func_80044618);

INCLUDE_ASM("asm/nonmatchings/gprel_17B7C", func_80044698);

/* Compares four 3-halfword vectors against a cache and, if any component
 * differs, copies all four over and returns 1; returns 0 if they already
 * match.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - the source side goes through a pointer local (106 words otherwise). As a
 *    direct symbol reference the comparisons still come out right, but
 *    expand_block_move sees a `(const (plus symbol 0x94))` address, which is
 *    not offsettable, so it force_regs a fresh source pointer per group: two
 *    extra addiu plus two extra lui. Through the pointer local the MEMs are
 *    already offsettable, so block_move_sequence just adjusts the displacement
 *    and reuses the comparison chain's own $a3.
 *  - but the destination stays a direct array reference (98 words the other
 *    way). That asymmetry is what gives the %lo riding on element 0's load,
 *    which is only possible at offset 0, followed by the addiu materialising
 *    the base for the rest. Same deliberate mixing of the two spellings for
 *    one object as in func_8003D5A4.
 *  - the copies go through Unk44724Blk, whose alignment of 1 makes each
 *    assignment the byte-aligned 8-byte block move; u32 members give plain
 *    lw/sw and cost 86 words.
 *  - four separate 8-byte assignments, not one 32-byte one: a single 32-byte
 *    copy is emitted as 4 loads then 4 stores per 16-byte group, where the
 *    target has 2 loads then 2 stores, four times.
 *  - an `||` chain of `!=` with the copy in the body and `return 0` last. The
 *    equivalent `&&` chain of `==` with `return 0` in the body has the same
 *    word count but 49 differ: the `||` form is what puts eleven
 *    `bne` to the copy and an inverted `beq` on the twelfth, and leaves the
 *    `return 0` block physically between the copy and the epilogue.
 *  - the components are s16; as u16 the comparisons become lhu. */
s32 func_80044724(void) {
    Unk80083178 *s = &D_80083178;

    if (s->unk_94[0].unk_00 != D_80083CE8[0].unk_00 ||
        s->unk_94[0].unk_02 != D_80083CE8[0].unk_02 ||
        s->unk_94[0].unk_04 != D_80083CE8[0].unk_04 ||
        s->unk_94[1].unk_00 != D_80083CE8[1].unk_00 ||
        s->unk_94[1].unk_02 != D_80083CE8[1].unk_02 ||
        s->unk_94[1].unk_04 != D_80083CE8[1].unk_04 ||
        s->unk_94[2].unk_00 != D_80083CE8[2].unk_00 ||
        s->unk_94[2].unk_02 != D_80083CE8[2].unk_02 ||
        s->unk_94[2].unk_04 != D_80083CE8[2].unk_04 ||
        s->unk_94[3].unk_00 != D_80083CE8[3].unk_00 ||
        s->unk_94[3].unk_02 != D_80083CE8[3].unk_02 ||
        s->unk_94[3].unk_04 != D_80083CE8[3].unk_04) {
        *(Unk44724Blk *) &D_80083CE8[0] = *(Unk44724Blk *) &s->unk_94[0];
        *(Unk44724Blk *) &D_80083CE8[1] = *(Unk44724Blk *) &s->unk_94[1];
        *(Unk44724Blk *) &D_80083CE8[2] = *(Unk44724Blk *) &s->unk_94[2];
        *(Unk44724Blk *) &D_80083CE8[3] = *(Unk44724Blk *) &s->unk_94[3];
        return 1;
    }
    return 0;
}

/* Walks the 4-byte records at D_80081508 looking for a (u16, u16) pair.
 *
 * Load-bearing: both parameters are `u16`. They are what produce the
 * `andi $a0,0xFFFF` / `andi $a1,0xFFFF` pair, which loop invariant motion then
 * sinks into the preheader below the guard branch, exactly as the listing has
 * it; `s32` gives 22 words against 24 with the whole body shifted. The `while`
 * with its test at the top (rather than a do/while) is what duplicates the
 * `lbu 0x0($v1)` into the entry path, since the guard's load and the rotated
 * body's load are in different basic blocks and cse does not merge them. The
 * `&&` chain rather than nested ifs gives the bne-then-beq pair with the
 * `addiu $v0,1` in the second branch's delay slot. */
s32 func_800448BC(u16 arg0, u16 arg1) {
    u8 *p = (u8 *) D_80081508;

    while (p[0] != 0) {
        if (p[0] == arg0 && p[1] == arg1) {
            return 1;
        }
        p += 4;
    }
    return 0;
}
