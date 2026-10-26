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

INCLUDE_ASM("asm/nonmatchings/gprel_17B7C", func_8004437C);

/* Switches the active buffer pair to entry arg0, unless it is already current.
 *
 * Load-bearing: `s16 arg0`, so the raw parameter stays in $s1 for the `sb`
 * while the sign-extended copy is a separate pseudo in $s0; `(s8) D_80080AF3`
 * for the single `lb`; the 8-byte two-pointer element with unk_04 consumed
 * before unk_00; `return 1` out of line and physically last; and D_8008148C
 * read twice rather than cached.
 *
 * THE `__asm__` IS LOAD-BEARING -- see the unit header. D_80081480 and
 * D_8008148C are u32 reached ABSOLUTELY, and this unit is built at -G8 because
 * func_800448BC needs the equally-4-byte D_80081508 $gp-relative. No -G value
 * separates two symbols of the same size, and no C declaration gives
 * bare-symbol-but-absolute, so the size record is overridden directly. gas
 * resolves small-vs-large at relaxation time and the LAST .extern wins;
 * cc1psx's own `.extern X,4` sits in the file header, so a statement-level
 * directive anywhere in a function body overrides it. It emits no bytes
 * (readelf shows UND, value 0, size 0). It IS a scheduling barrier, so it
 * lives in the `return 1` arm where it is free -- as the function's first
 * statement it costs 6 words. Every declaration-based spelling measured 2
 * words worse. */
s32 func_8004450C(s16 arg0) {
    if ((s8) D_80080AF3 != arg0) {
        func_8003E4FC(6, D_8006E6F4[arg0].unk_04, 0);
        DrawSync(0);
        D_80081480 = D_8008148C;
        func_8003E4FC(6, D_8006E6F4[arg0].unk_00, 0);
        D_80080AF3 = arg0;
        func_8003F320();
        func_8003F5E0(D_8008148C);
        func_80053DA8(0x11);
        func_800542BC();
        while (func_8005405C(0) != 1) {
        }
        return 0;
    }
    __asm__(".extern D_80081480, 16\n\t.extern D_8008148C, 16");
    return 1;
}

u32 func_800445E0(void) {
    Unk8006CE34 *p = func_8003F534();
    u32 t = (p->unk_08 & 0xFFFFFF) - 0x20;

    return t + p->unk_04;
}

/* Waits a frame and, if the frame counter's low bits changed, pokes the sound
 * driver. arg0 == 2 selects a 9-bit counter rather than 8.
 *
 * Load-bearing, and it is the whole function: `t` is assigned TWICE, once
 * before the `if` and once inside it. cse does NOT delete the second
 * `t = v >> 8;`, and the two sets promote the shift from a block-local
 * quantity to a global allocno, which is what orders the registers $v0 (masks)
 * / $v1 (shift) / $a0 (load) / $a1 (v) as the target has them. One set costs
 * 29 words and 19 differing; no `t` at all, with `(v >> 8)` written into each
 * ternary arm, costs 7 differing words. `u32 v`, not s32, for `srl` over
 * `sra`. The ternary appears in both the test and the store, which is where
 * the second srl/andi pair comes from; reorg then steals each srl into a
 * branch delay slot, so one statement prints twice.
 *
 * The +2 halfword is reached as a cast off &D_80080AFC rather than as an array
 * element: `extern u16 D_80080AFC[2]` with `[1]` also matches HERE, but costs
 * func_8004437C a word and 79 differing ones (see structs.h). */
void func_80044618(s32 arg0) {
    u32 v = VSync(1);
    u32 t;

    t = v >> 8;

    if (((arg0 == 2) ? (t & 0x1FF) : (t & 0xFF)) != *(u16 *) ((u8 *) &D_80080AFC + 2)) {
        t = v >> 8;
        *(u16 *) ((u8 *) &D_80080AFC + 2) = (arg0 == 2) ? (t & 0x1FF) : (t & 0xFF);
        func_800542BC();
    }
}

/* Drains the sound driver's queue, nudging it once on the first iteration
 * unless it reports 3, and tracking the frame counter.
 *
 * Load-bearing: the `(u32)` cast on VSync's return (`srl`, not `sra` -- 1
 * word); `u8 flag` with the redundant `& 1`, which keeps the one-insn
 * materialisation as `addu $v0,$s0,$zero` in the beqz's delay slot (a plain
 * `flag != 0` on a u8 gives `andi 0xFF`, and an s32 with the mask gives
 * `andi 0x1` -- all three are 35 words, only this one matches); `flag = 1;`
 * before the `r != 3` test, so the `addiu $s0,1` lands in the beq's delay
 * slot; and the call inside the `while` condition, whose loop note is what
 * hoists the constant 3 into $s1. */
void func_80044698(void) {
    u8 flag = 0;
    s32 r;
    u32 t;

    while ((r = func_80053EF0(1)) != 0) {
        if ((flag & 1) == 0) {
            flag = 1;
            if (r != 3) {
                func_80053DA8(0xC1);
            }
        }

        func_80044618(2);
        t = ((u32) VSync(1) >> 8) & 0x1FF;
        if (t != D_80080B00) {
            D_80080B00 = t;
        }
    }
}

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

    if (s->unk_94[0].vx != D_80083CE8[0].vx ||
        s->unk_94[0].vy != D_80083CE8[0].vy ||
        s->unk_94[0].vz != D_80083CE8[0].vz ||
        s->unk_94[1].vx != D_80083CE8[1].vx ||
        s->unk_94[1].vy != D_80083CE8[1].vy ||
        s->unk_94[1].vz != D_80083CE8[1].vz ||
        s->unk_94[2].vx != D_80083CE8[2].vx ||
        s->unk_94[2].vy != D_80083CE8[2].vy ||
        s->unk_94[2].vz != D_80083CE8[2].vz ||
        s->unk_94[3].vx != D_80083CE8[3].vx ||
        s->unk_94[3].vy != D_80083CE8[3].vy ||
        s->unk_94[3].vz != D_80083CE8[3].vz) {
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
