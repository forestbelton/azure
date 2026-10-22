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
void func_800400B8(void);
void func_8003F6F4(void);
void func_8003E758(void);
void func_8003E2D8(void);
void func_80042B68(s8 *, s8);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041134);

void func_800411AC(void) {
    void (**p)(void) = D_80083C68;

    while (*p != NULL) {
        (*p++)();
    }
}

void func_800411FC(u16 arg0) {
    Unk8006CE80 *e = &D_8006CE80[arg0];
    u32 *p = e->unk_04;
    u32 **q;

    if (p == NULL) {
        return;
    }
    q = (u32 **) p[3];
    if (q == NULL) {
        return;
    }
    if (*q == NULL) {
        return;
    }
    do {
        func_80041284((Unk41284 *) *q);
        q++;
    } while (*q != NULL);
}

/* Splits a word into a 23-bit value and its top bits, publishes D_8008148C as
 * the current buffer pointer, and hands both halves to func_80041344.
 *
 * Two shapes are load-bearing, and together they are what this function was
 * parked for:
 *  - the parameter is a struct pointer, not a `u32 *`. That makes its two
 *    loads MEM_IN_STRUCT_P while the D_80081480 store is not, so
 *    true_dependence's differing-flags exception applies (the non-struct side
 *    is a symbol, so its address does not vary) and sched1 is free to hoist
 *    the second load above the store. Through a plain `u32 *` both MEMs are
 *    non-struct, the exception does not apply, store-before-load is a true
 *    dependence, and arm 1 comes out with the store first -- the four words
 *    this was parked on.
 *  - the assignment is a chained store whose value is wanted, which sends it
 *    through store_expr's copy_to_reg: the RTL is load-to-temp / store / copy.
 *    combine cannot fold the load into the copy because the store sits between
 *    them (use_crosses_set_p via mem_last_set), sched1 then lifts the copy
 *    above the store, and local-alloc rewrites the store's source. That is
 *    what gives arm 2 its `lw`/`or`/`addu`/`lui`/`sw` and the empty
 *    func_8003F320 delay slot. */
void func_80041284(Unk41284 *p) {
    u32 x;
    u32 y;

    DrawSync(0);
    x = p->unk_00 & 0x7FFFFF;
    if (x == 0) {
        x = D_80081480 = D_8008148C;
        y = x + ((p->unk_00 & 0xFF800000) >> 12);
    } else {
        x |= 0x80000000;
        y = D_80081480 = D_8008148C;
    }
    func_8003E4FC(6, (u8 *) p, 0);
    func_8003F320();
    func_80041344(x, y);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041344);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041588);

void func_80041818(void) {
    Unk8006CE80 *e = &D_8006CE80[D_80082E60.unk_0B];

    func_801768AC();
    D_80082E60.unk_08.h = e->unk_02;
    func_80040B88();
    e = &D_8006CE80[D_80082E60.unk_0B];
    if (e->unk_00 != 4) {
        func_800411FC(0);
        func_800499BC();
    }
}

void func_800418B4(void) {
    if (D_80080A89 != 0) {
        return;
    }
    func_8003E4FC(6, &D_80080C50, 0);
    func_8003F320();
    D_80080A89 = 1;
}

void func_80041900(u8 *arg0) {
    Unk80083160 *s = &D_80083160;
    Unk80083CA8 *sh = &D_80083CA8;
    Unk80083CA8 *t;

    if (sh->unk_08 == 0) {
        return;
    }
    if (s->unk_04 & 2) {
        *(s32 *) (arg0 + 0x18) += (s16) sh->unk_06 >> 1;
    } else {
        *(s32 *) (arg0 + 0x18) -= (s16) sh->unk_06 >> 1;
    }
    if (s->unk_04 & 1) {
        *(s32 *) (arg0 + 0x14) += (s16) D_80083CAE[0] >> 2;
    } else {
        *(s32 *) (arg0 + 0x14) -= (s16) D_80083CAE[0] >> 2;
    }
    t = &D_80083CA8;
    t->unk_08 = t->unk_08 - 1;
    if (t->unk_08 != 0) {
        s32 v = *(s32 *) &t->unk_04 - D_80083CA8.unk_00;

        *(s32 *) &t->unk_04 = v;
        if (v >= 0) {
            return;
        }
    }
    *(s32 *) &t->unk_04 = 0;
}

void func_800419EC(u16 arg0, u32 arg1) {
    D_80083CA8.unk_08 = arg0;
    D_80083CA8.unk_06 = arg1;
    D_80083CA8.unk_00 = arg1 / arg0;
}

void func_80041A10(void) {
    D_80082E60.unk_00 |= 1;
    D_80081480 = D_8008148C;
    func_8003FAD4();
    func_800410FC();
    D_80083160.unk_00 = D_801C9E40;
    ClearOTagR(D_801C9E40 + 0x70, 0x218);
    ClearOTagR(&D_801DA784, 0x218);
    *(u8 **) (D_80083160.unk_00 + 0x8D0) = D_80083160.unk_00 + 0x8D4;
    SetDispMask(1);
    func_8003F7E4();
    func_80040A88(func_80041AB0);
}

void func_80041AB0(void) {
    D_80081480 = D_8008148C;
    func_80040A88(func_80041AE4);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041AE4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041B98);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041BE4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041C64);

/* The per-frame flip: present the display and draw environments, run the draw
 * list unless bit 1 of the 0x80013714 flags is set, swap the double-buffered
 * work area at D_801C9E40 and relink its OT, then record the frame time from
 * counter 1 and optionally VSync.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - the 0x80013714 flags go through the out-of-image constant base pointer
 *    and are re-read at all three tests. The existing `extern u16 D_80013714`
 *    scalar costs 49 words, because the assembler-expanded load reuses its own
 *    destination register so the `lui` cannot stay in $s1 across the calls;
 *    `*(u16 *) 0x80013714` costs 88 (lui+ori plus 0($reg)). splat renders the
 *    bare `lui $s1, 0x8001` as %hi(D_80013714) because it can pair it with the
 *    later %lo, which hides the difference.
 *  - caching that halfword in a local costs 86 words; three separate lhu off
 *    $s1 is the target.
 *  - `p += 0x108D4;` as a pointer increment. Both `D_801C9E40 + 0x108D4` and
 *    `&D_801C9E40[0x108D4]` cost 57 words: nothing can fold a constant into a
 *    lo_sum, so those spellings rebuild the whole address instead of the
 *    target's lui 0x1 / ori 0x8D4 / addu.
 *  - the second D_80083160.unk_00 read is its own expression rather than the
 *    reused `p` (3 words). `p = D_80083160.unk_00;` after the ClearOTagR call
 *    puts the reload in $a1 where the target has $v1, because p's pseudo
 *    already has a live range before the call and that moves it down
 *    local-alloc's quantity order.
 *
 * The duplicated `addiu $a0, $a1, 0x70` in the bne's delay slot is reorg
 * stealing the join-point insn and redirecting the branch past it, not a
 * source duplication. */
void func_80041CBC(void) {
    u8 *base = (u8 *) 0x80010000;
    u8 *p;

    PutDispEnv(D_80083160.unk_00 + 0x5C);
    PutDrawEnv(D_80083160.unk_00);
    if ((*(u16 *) (base + 0x3714) & 2) == 0) {
        DrawOTag(D_80083160.unk_00 + 0x8CC);
    }
    func_8003E758();
    func_800542BC();
    D_80081480 = D_8008148C;
    func_800411AC();
    func_8003E2D8();
    p = D_801C9E40;
    if (D_80083160.unk_00 == p) {
        p += 0x108D4;
    }
    D_80083160.unk_00 = p;
    ClearOTagR(p + 0x70, 0x218);
    *(u8 **) (D_80083160.unk_00 + 0x8D0) = D_80083160.unk_00 + 0x8D4;
    func_8003F6F4();
    func_800894A0();
    if ((*(u16 *) (base + 0x3714) & 2) == 0) {
        DrawSync(0);
    }
    func_80048B28();
    DrawSync(0);
    D_80080A87 = (GetRCnt(1) + 0xFF) >> 8;
    if ((*(u16 *) (base + 0x3714) & 2) == 0 && D_80080A85 == 0) {
        VSync((D_80080A84 != 1) * 2);
    }
    ResetRCnt(1);
}

void func_80041E28(s32 *arg0, s32 *arg1) {
    func_8003DB6C(arg0, arg1, 0x13);
    arg0[0x15] = arg1[0x14];
    func_80041E70(arg0);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041E70);

s16 func_800422A8(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    u32 q = (u32) (arg0 - arg1) / (u32) (arg2 & 0xFFFF);

    /* Early-return on the failure case; the other order lays the arms out
     * backwards. */
    if (q >= (u32) (arg3 & 0xFFFF)) {
        return -1;
    }
    return (s16) q;
}

void func_800422DC(u8 *arg0, u8 *arg1) {
    func_80042B68((s8 *) arg1, 0x1A);
    func_80042518((s8 *) arg1, 0x1B);
    func_80042B68((s8 *) arg1, 0x1C);
    func_80042B68((s8 *) arg1, 0x1D);
    if (arg1[0x25] == 0 && arg1[0x13] != 0) {
        *(u32 *) (arg1 + 0x1C) |= 8;
    }
    func_8003DB6C((int *) arg0, (int *) arg1, 0x13);
    arg0[0x4C] = func_800422A8(*(s32 *) (arg1 + 0x4C), 0x80010248, 4, 0x14);
    arg0[0x4D] = func_800422A8(*(s32 *) (arg1 + 0x50), 0x80010248, 4, 0x14);
    *(u32 *) (arg0 + 0x50) = *(u32 *) (arg1 + 0x54);
}

/* Applies one monster's species record to an instance, then advances the
 * shared 3-bit animation counter at 0x80012094 and copies two of its bytes
 * into the instance.
 *
 * arg1 is genuinely unused: $a1 is overwritten with the -8 mask before any use
 * of it, and that mask constant is what happens to be sitting in $a1 at the
 * func_800A1BD0 call, which takes one argument.
 *
 * Five load-bearing shapes, four of them about how the three `lui 0x8001`s are
 * materialised. The target has three independent ones where the obvious source
 * gives gcc one that cse shares, so each of these blocks a reuse:
 *
 *  - The base pointer is built by an IN-PLACE update: `p = (u8 *) 0x80010000;`
 *    and then `p += r * 2;`, never `p = (u8 *) 0x80010000 + r * 2;`. In place,
 *    the add's destination ties to the constant's own pseudo and consumes it,
 *    so nothing holds 0x80010000 afterwards and the 0x80012096 arm needs a
 *    fresh `lui`. As one expression the constant survives in its own register
 *    and cse reuses it with a `move` (4 DIFF, but the wrong instructions).
 *    Same mechanism as func_8003FE78's in-place `flags |= 0x80`.
 *  - `cmp` is declared and initialised BEFORE `p`. The other order puts p's
 *    `lui` first and cse shares it with the 0x80012094 constant instead -- the
 *    sharing just moves (14 DIFF).
 *  - `p += r * 2;` comes BEFORE the `if (r == 0)`. After it, the sharing comes
 *    back as a `move` (8 DIFF).
 *  - The indexed base is the plain constant 0x80010000 with 0x2094/0x2098 as
 *    load displacements, NOT the symbol D_80012094, even though splat renders
 *    it as `%hi(D_80012094)`: splat prints a bare `lui reg, 0x8001` that way
 *    whenever it can pair the register with a later `%lo`. A symbol array with
 *    a variable index always materialises the whole address with
 *    `lui %hi` + `addiu %lo` and indexes off that, for every spelling tried.
 *    Inert, all 4 DIFF: `(u8 *) (r * 2 + 0x80010000)`, an `off` local, and
 *    `&((u8 *) 0x80010000)[r * 2]`.
 *
 * And the fifth, which is why the `else` arm duplicates a store that could
 * just follow the `if`: it buys the base pointer one extra REFERENCE at
 * global-alloc time, and jump2's cross-jumping then merges the copies back
 * together for free. r wants $a0 and p wants $v1; without the duplicate the
 * allocation order is `95 113 ...` and they come out swapped -- every
 * instruction, immediate and displacement already matched, with 12 register
 * fields wrong. p has 7 references across 15 insns for a global.c priority of
 * floor_log2(7) * 7 / 15 = 0.93, just under r's floor_log2(4) * 4 / 8 = 1.00,
 * and whichever is allocated first takes $v1. Duplicating the store takes p to
 * 8 references across 18 insns, which crosses the floor_log2 boundary: 3 * 8 /
 * 18 = 1.33, the order becomes `113 95 ...`, and the word count is unchanged.
 * Duplicating the 0x12 store instead works identically. A 7% priority gap was
 * the closest margin in the project, so this is the one case where the
 * allocator was persuaded rather than explained; see docs/matching-levers.md
 * sections 1 and 3. */
void func_800423C0(u8 *arg0, s32 arg1, u8 *arg2) {
    if (arg2 != NULL) {
        Unk42710 *e = (Unk42710 *) &D_800E3E48[arg2[3] & 0x1F];
        s32 r;

        func_80042710((Unk42710 *) arg0, e);
        *(u32 *) (arg0 + 0x14) &= ~7;
        *(u32 *) (arg0 + 0x1C) &= ~7;
        *(u32 *) (arg0 + 0x14) |= e->unk_14 & 7;
        *(u32 *) (arg0 + 0x1C) |= e->unk_1C & 7;
        r = (s16) func_800A1BD0(arg0);
        if (r >= 0) {
            u16 *cmp = (u16 *) 0x80012094;
            u8 *p = (u8 *) 0x80010000;
            u16 t;

            p += r * 2;
            if (r == 0) {
                cmp = (u16 *) 0x80012096;
            }
            t = *(u16 *) (p + 0x2094);
            if (t == *cmp) {
                *(u16 *) (p + 0x2094) = (t + 1) & 7;
                arg0[0x45] = p[0x2094];
            } else {
                arg0[0x45] = p[0x2094];
            }
            arg0[0x12] = p[0x2098];
        }
    }
    *(u32 *) (arg0 + 0x1C) |= 0x20000;
}

void func_800424E0(u8 *arg0, s16 arg1, u8 *arg2) {
    func_800423C0(arg0, arg1, arg2);
    func_80042984(arg0);
}

s32 func_80042518(s8 *arg0, s8 arg1) {
    s32 i;

    for (i = 3; i >= 0; i--) {
        if (arg0[i * 2 + 0x2C] == arg1) {
            s32 r = arg0[i * 2 + 0x2D];

            arg0[i * 2 + 0x2C] = 0;
            arg0[i * 2 + 0x2D] = 0;
            return r;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042560);

/* Initialises a record from template `idx`: a 0x14-byte block assignment, the
 * flag word merged in, then a handful of bytes picked back out of the same
 * template, and finally the per-template word from D_8006D6D8.
 *
 * Two shapes are load-bearing, and together they are what this function was
 * parked for ("gcc hoists each constant store into the preceding load's delay
 * slot"):
 *  - `d->unk_13 = idx;` comes after the unk_14 merge, not before it. Written
 *    first, its `sb` is scheduled between the merge's two loads and the second
 *    load's delay then needs a nop, which is one word too many. Written after,
 *    the `sb` fills that delay slot itself.
 *  - the three stores at +0x28/+0x25/+0x24 are in that order, so the 0xFF
 *    constant is the LAST of the three. In any order that puts it earlier,
 *    sched1 hoists it into the first lbu's delay slot, which forces that load
 *    to a different register and reorders the whole group; two of the six
 *    orderings also cost a word. With the constant last, the first lbu and its
 *    store stay adjacent -- the target's nop -- because the only candidate for
 *    that slot would clobber the loaded register.
 *
 * The block assignment is `*(UnkTemplate *) d = e->unk_00;`: UnkTemplate keeps
 * u32 members for its upper half purely so its alignment stays 4 and the copy
 * is lw/sw rather than lwl/lwr. */
void func_80042640(Unk42710 *d, s16 idx) {
    Unk8006D168 *e = &D_8006D168[idx];

    *(UnkTemplate *) d = e->unk_00;
    d->unk_14 |= e->unk_14;
    d->unk_13 = idx;
    d->unk_28 = e->unk_00.unk_05;
    d->unk_25 = e->unk_00.unk_04;
    d->unk_24 = 0xFF;
    d->unk_26 = e->unk_00.unk_00;
    d->unk_27 = e->unk_00.unk_01;
    d->unk_45 = 6;
    d->unk_42 = 0;
    d->unk_41 = 0;
    d->unk_54 = D_8006D6D8[idx];
    func_80041E70(d);
}

/* Copies one record over another field by field, merging the two flag words
 * rather than replacing them, then hands the destination to func_80041E70.
 *
 * The asymmetry between the three loops is load-bearing, and loop 1's is
 * semantic as well:
 *  - loop 1 copies +0x00 and +0x02 of each 3-byte element and deliberately
 *    leaves the middle byte alone. A whole-struct assignment copies all three
 *    and costs 2 words.
 *  - loop 2 IS a whole-struct assignment. move_by_pieces emits the 2-byte copy
 *    as two QImode loads then two QImode stores in one straight-line group,
 *    which is both the lb/lb/sb/sb the target has and the stores-before-
 *    increments order. Field by field gives lbu/sb/lbu/sb; through s32
 *    temporaries it gives lb/lb but with the pointer increments hoisted
 *    between the loads and the stores, and that is forced -- with two loads,
 *    two stores and three increments the dependence graph gives the stores
 *    scheduling depth 2 against the increments' 1, so gcc always picks a store
 *    at T-3. The extra register pressure the block-move group creates is also
 *    what forces the second parameter out of $a1 into $t2, the word that
 *    spelling was short.
 *  - loop 3 is the direct array copy; routing it through a temporary costs 4
 *    words.
 *
 * The 0x49 tag byte lies inside the unk_48 word, so it needs its own spelling;
 * the byte-pointer form, `((u8 *) p)[0x49]` and a union member all match. */
void func_80042710(Unk42710 *d, Unk42710 *s) {
    s32 i;

    d->unk_00 = s->unk_00;
    d->unk_01 = s->unk_01;
    d->unk_02 = s->unk_02;
    d->unk_03 = s->unk_03;
    d->unk_04 = s->unk_04;
    d->unk_05 = s->unk_05;
    d->unk_06 = s->unk_06;
    for (i = 2; i >= 0; i--) {
        d->unk_08[i].unk_00 = s->unk_08[i].unk_00;
        d->unk_08[i].unk_02 = s->unk_08[i].unk_02;
    }
    d->unk_11 = s->unk_11;
    d->unk_12 = s->unk_12;
    d->unk_13 = s->unk_13;
    d->unk_14 = (d->unk_14 | s->unk_14) & 0xFFEFFFFF;
    d->unk_18 = s->unk_18;
    d->unk_1C = (d->unk_1C | s->unk_1C) & 0xEFF6FEFF;
    d->unk_20 = s->unk_20;
    d->unk_22 = s->unk_22;
    *(u16 *) &d->unk_24 = *(u16 *) &s->unk_24;
    d->unk_26 = s->unk_26;
    d->unk_27 = s->unk_27;
    d->unk_28 = s->unk_28;
    d->unk_2A = s->unk_2A;
    for (i = 3; i >= 0; i--) {
        d->unk_2C[i] = s->unk_2C[i];
    }
    for (i = 0xC; i >= 0; i--) {
        d->unk_34[i] = s->unk_34[i];
    }
    d->unk_43 = s->unk_43;
    d->unk_44 = s->unk_44;
    if (((u8 *) &d->unk_48)[1] == 0) {
        d->unk_48 = s->unk_48;
    }
    if (((u8 *) &s->unk_48)[1] == 0xF) {
        d->unk_48 = s->unk_48;
        if (s->unk_4C != 0) {
            d->unk_4C = (u32) &d->unk_48;
        }
    }
    d->unk_45 = s->unk_45;
    d->unk_54 = s->unk_54;
    func_80041E70(d);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042900);

void func_80042984(u8 *arg0) {
    u8 t = arg0[0x13];

    if (t >= 3 && t < 0x2E) {
        u8 *p = *(u8 **) (arg0 - 0x14);
        u16 v = *(u16 *) (p + 0x12) & 0xFFC0;

        *(u16 *) (p + 0x12) = v + (func_800429E4(arg0) << 2);
    }
}

s32 func_800429E4(u8 *arg0) {
    u32 flags = *(u32 *) (arg0 + 0x14);
    s32 r = 0;

    if (flags & 0x2000) {
        Unk8006D168 *e = &D_8006D168[arg0[0x13]];
        u32 m = e->unk_14 & flags;

        /* Single exit: an early `return` loads the byte straight into the
         * return register, where the target puts it in r's register and
         * reorg copies it into a delay slot. */
        if ((m & 7) != 0) {
            r = D_8006D160[m & 7];
        } else if (flags & 1) {
            r = 1;
        } else if (flags & 2) {
            r = 2;
        } else if (flags & 4) {
            r = 3;
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042A80);

void func_80042B68(s8 *arg0, s8 arg1) {
    s32 i;

    for (i = 3; i >= 0; i--) {
        if (arg0[i * 2 + 0x2C] == arg1) {
            arg0[i * 2 + 0x2C] = 0;
            arg0[i * 2 + 0x2D] = 0;
            func_80042BDC(arg0, arg1);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042BDC);

void func_80043458(void) {
    u8 *base = (u8 *) 0x80010000;
    u8 *base2;
    s32 i;

    *(u32 *) (base + 0x208C) = 0;
    *(u32 *) (base + 0x2090) = 0;
    base[0x2D52] = 0xFF;
    base[0x2D53] = 0xFF;
    base[0x21E0] = 0xFF;
    base[0x21E1] = 0xFF;
    /* The 0x24B and 0x304 have to stay inside the index expressions, or they
       become part of the materialised constant instead of the load's
       displacement. The second loop needs its own base, assigned after the
       first loop: reusing `base` copies the register where the target
       re-materialises it, and folding the base into the address constant
       loses the 0xA94 displacement. */
    for (i = 0; i < 0x14; i++) {
        base[i * 4 + 0x24B] &= 0x5F;
        *(u32 *) (base + i * 0x54 + 0x304) &= ~0x4000;
    }
    base2 = (u8 *) 0x80010000;
    for (i = 0; i < 0x40; i++) {
        *(u32 *) (base2 + i * 0x54 + 0xA94) &= ~0x4000;
    }
}

void func_800434E4(void) {
    u8 *base = (u8 *) 0x80010000;

    base[0x3185] = 1;
    *(u32 *) (base + 0x2D5C) = 0;
    *(u32 *) (base + 0x2D68) = 0;
    *(u32 *) (base + 0x2D58) = 0;
    *(u32 *) (base + 0x22C) = 0;
    func_80043458();
    *(u16 *) (base + 0x3630) = 0xD5;
    *(u16 *) (base + 0x3632) = 0x38;
    *(u16 *) (base + 0x3634) = 0x6B;
    *(u16 *) (base + 0x3636) = 0x70;
    *(u16 *) (base + 0x3638) = 0x100;
    *(u16 *) (base + 0x363A) = 0x78;
    *(u32 *) (base + 0x363C) = 0x4000;
    *(u16 *) (base + 0x20A0) = 0;
    *(u16 *) (base + 0x20A2) = 0;
    *(u16 *) (base + 0x20A4) = 0;
}

void func_80043568(void) {
    u8 *base;
    u8 *base2;
    s32 i;
    s32 t;
    s32 u;

    i = 0x3F;
    base = (u8 *) 0x80010000;
    for (; i >= 0; i--) {
        if (base[i * 4 + 0x981] == 0x13 && base[i * 4 + 0x980] == 0x39) {
            t = base[i * 4 + 0x983];
            base[i * 4 + 0x980] = 2;
            u = t & 0x3F;
            base[u * 84 + 0xA93] = 2;
        }
    }
    i = 0x13;
    base2 = (u8 *) 0x80010000;
    for (; i >= 0; i--) {
        if (base2[i * 4 + 0x249] == 0x13 && base2[i * 4 + 0x248] == 0x39) {
            base2[i * 4 + 0x248] = 2;
        }
        if (base2[i * 4 + 0x21E9] == 0x13 && base2[i * 4 + 0x21E8] == 0x39) {
            t = base2[i * 4 + 0x21EB];
            base2[i * 4 + 0x21E8] = 2;
            u = t & 0x1F;
            base2[u * 140 + 0x2273] = 2;
        }
    }
}

void func_80043674(void) {
    s32 x = 4;
    s32 y = -0x14;
    s32 i;

    /* The destination has to be written first in the statement: expand_assignment
     * expands the store address before the RHS, so lo_sum(D_800835E8) enters
     * cse's table first and D-4 becomes its related value. Written the other way
     * round both givs anchor on %hi(D_800835E8-4) and cost an extra addu. */
    D_800835E8[0] = 0;
    for (i = 1; i < 0x64; i++, x++) {
        D_800835E8[i] = ((u32) (x * x * x) >> 3) + D_800835E8[i - 1];
        y++;
        if (y > 0) {
            D_800835E8[i] += (y * y) << 5;
        }
    }
    D_80083778[0] = 0xFA56EA00;
}

s32 *func_80043704(s16 arg0, s32 arg1) {
    s32 n;
    s32 k;

    if (arg0 == 0x38) {
        n = 0x421F;
        k = 0x4E;
    } else {
        if (arg0 >= 0x3A) {
            n = (arg0 - 0x3A) * 43 + 0x1B8D;
        } else {
            n = (arg0 - 1) * 43 + 0x56D6;
        }
        k = 0x1A;
    }
    func_8003F6D4(k, arg1, D_800814F0, n);
    return D_800814F0;
}

s32 *func_800437A4(s16 arg0, s32 arg1) {
    s32 n;

    if (arg0 == 0x38) {
        n = 0x426D;
    } else if (arg0 >= 0x3A) {
        n = (arg0 - 0x3A) * 43 + 0x1BA7;
    } else {
        n = (arg0 - 1) * 43 + 0x56F0;
    }
    func_8003F6D4(0x11, arg1, D_800814F8, n);
    return D_800814F8;
}

u32 func_8004383C(u8 *arg0, s16 arg1) {
    return D_800835E8[arg0[0x11] + arg1];
}

s32 func_80043868(u8 *arg0) {
    s32 a = func_8004383C(arg0, -1);
    s32 b = func_8004383C(arg0, 0);
    s32 cur = *(s32 *) (arg0 + 0x18);
    s32 d = b - a;

    a = cur - a;
    d = (a * 20) / d;
    if (d >= 0x15) {
        d = 0x14;
    }
    return d;
}

s32 func_800438E4(u8 *arg0) {
    s32 v = (s8) arg0[2];
    u32 n = arg0[3] & 0x1F;
    s32 t;

    if (n != 0) {
        /* The temporary is load-bearing: `v = v * 10` writes the product back
         * into v's register, and the target keeps it in a scratch one. */
        t = v * 10;
        v = t * n;
    }
    return v;
}

void func_80043914(u8 *arg0) {
    u8 v;
    u8 w;

    if (arg0[0] != 0x16) {
        return;
    }
    w = D_800E3E48[arg0[3] & 0x1F].unk_48;
    if (w == 0xD) {
        v = 0xF;
    } else if (w == 0xF) {
        v = 0xA;
    } else {
        return;
    }
    D_800E2968 = v;
}

void func_8004397C(u8 *arg0) {
    u8 v;

    if (arg0[0x13] != 0x16) {
        return;
    }
    if (arg0[0x48] == 0xD) {
        v = 0xF;
    } else if (arg0[0x48] == 0xF) {
        v = 0xA;
    } else {
        return;
    }
    D_800E2968 = v;
}

s32 func_800439BC(s16 arg0) {
    u8 b = D_8006DE24[arg0].unk_10;

    return (((b >> 4) ^ b) & 3) == 0;
}

void func_800439F8(void) {
    /* A separate base local inside each arm, not one function-scope variable:
     * a block-local base is allocated first and lands in $v0, and reorg then
     * finds the two arms' leading `lui`s identical and hoists one copy into
     * the branch delay slot. A function-scope base becomes a global allocno,
     * conflicts with the values already in $v0..$a0, and ends up in $a1. */
    if (D_80080A8A != 0) {
        u8 *base = (u8 *) 0x80010000;

        D_80080A8A = 0;
        *(u32 *) (base + 0x3180) = D_80081494;
        base[0x3184] = D_800814AC;
        base[0x3186] = D_800814A4;
    } else {
        u8 *base = (u8 *) 0x80010000;

        D_80081494 = *(u32 *) (base + 0x3180);
        D_800814AC = base[0x3184];
        D_800814A4 = base[0x3186];
    }
}

/* Clears bit 15 of the flag halfword of four elements -- n, n+1, n+22 and n+23
 * -- for each of the two rows n = i << D_8008333C[5], i in 10..11.
 *
 * Two shapes are load-bearing:
 *  - the shift has to be re-read inside every one of the four statements, not
 *    hoisted into a local. Each `sh` invalidates cse's memory entry for it, so
 *    the target genuinely reloads it and rebuilds the whole index chain four
 *    times; with the index in a local the chain is computed once and the
 *    function is 35 words instead of 57.
 *  - `sh` has to be declared and initialised BEFORE `base`. Both symbol
 *    references need `(high D_8008333C)` in a register, and only in this order
 *    does cse share it: the word read then gets its `%lo` riding on the load
 *    while the halfword base is an `addiu` off the same `lui`. Reading the
 *    halfword as `((s16 *) D_8008333C)[10]` inline, or declaring `sh` after
 *    `base`, costs a second `lui`.
 *
 * func_80043B4C is the same function with `|= 0x8000`; keeping them spelled
 * identically is deliberate. */
void func_80043A68(void) {
    s16 *sh = (s16 *) D_8008333C;
    u8 *base = (u8 *) D_8008333C[0];
    s32 i;

    for (i = 10; i < 12; i++) {
        ((Unk43A68 *) (base + 0x78))[i << sh[10]].unk_04 &= 0x7FFF;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 1].unk_04 &= 0x7FFF;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 22].unk_04 &= 0x7FFF;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 23].unk_04 &= 0x7FFF;
    }
}

/* Sets bit 15 of the same four elements func_80043A68 clears it in; see the
 * comment there for the two load-bearing shapes. */
void func_80043B4C(void) {
    s16 *sh = (s16 *) D_8008333C;
    u8 *base = (u8 *) D_8008333C[0];
    s32 i;

    for (i = 10; i < 12; i++) {
        ((Unk43A68 *) (base + 0x78))[i << sh[10]].unk_04 |= 0x8000;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 1].unk_04 |= 0x8000;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 22].unk_04 |= 0x8000;
        ((Unk43A68 *) (base + 0x78))[(i << sh[10]) + 23].unk_04 |= 0x8000;
    }
}

void func_80043C30(void) {
    D_80082E60.unk_00 |= 1;
    D_80081480 = D_8008148C;
    func_8003FAD4();
    func_800410FC();
    D_80083160.unk_00 = D_801C9E40;
    ClearOTagR(D_801C9E40 + 0x70, 0x218);
    ClearOTagR(&D_801DA784, 0x218);
    *(u8 **) (D_80083160.unk_00 + 0x8D0) = D_80083160.unk_00 + 0x8D4;
    SetDispMask(1);
    func_8003F7E4();
    func_80040A88(func_80043CD0);
}

void func_80043CD0(void) {
    D_80081480 = D_8008148C;
    func_80040A88(func_80043D04);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043D04);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043DB8);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043E04);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043E60);

void func_80043EB8(void) {
    u8 *p;

    PutDispEnv(D_80083160.unk_00 + 0x5C);
    PutDrawEnv(D_80083160.unk_00);
    DrawOTag(D_80083160.unk_00 + 0x8CC);
    func_8003E758();
    func_800542BC();
    D_80081480 = D_8008148C;
    func_800411AC();
    func_8003E2D8();
    /* One local for the buffer address: the two arms share the `lui` and
     * reorg folds one copy into the branch delay slot. */
    p = D_801C9E40;
    if (D_80083160.unk_00 == p) {
        p += 0x108D4;
    }
    D_80083160.unk_00 = p;
    ClearOTagR(p + 0x70, 0x218);
    *(u8 **) (D_80083160.unk_00 + 0x8D0) = D_80083160.unk_00 + 0x8D4;
    func_8003F6F4();
    func_800400B8();
    func_80048B28();
    DrawSync(0);
    if (D_80080A85 == 0) {
        VSync((D_80080A84 != 1) << 1);
    }
}

s32 func_80043FB8(s32 arg0, ...) {
    return 0;
}

/* The per-frame tick for a func_80044144 node. kind == 2 finishes a fade and
 * clears itself; otherwise it runs the node's own callback and either records
 * a failure in the D_80083120 slot or marks the node and D_800814A0 dirty.
 *
 * Two shapes are load-bearing:
 *  - the node's flag word is reached as a plain pointer dereference,
 *    `*(u16 *) ((u8 *) e - 2)`. Every struct or array spelling --
 *    `((UnkNode *) e)[-1].unk_1E`, `((UnkNode *) e - 1)->unk_1E`, even
 *    `((u16 *) e)[-1]` -- costs 2 words, because those are COMPONENT_REFs or
 *    ARRAY_REFs so gcc sets MEM_IN_STRUCT_P, and true_dependence's exception
 *    (no dependence when the in-struct flags differ and the non-struct side
 *    has a non-varying address) then frees sched1 to interleave this
 *    read-modify-write with the D_800814A0 one, filling both load-delay slots.
 *    With a bare pointer dereference neither MEM is in-struct, the exception
 *    does not apply, memrefs_conflict_p cannot separate a register base from a
 *    symbol, and the two statements stay strictly sequential as the target has
 *    them.
 *  - e->unk_08 is re-read for each of the four D_80083120 stores. Hoisting
 *    &D_80083120[e->unk_08] into a local costs 11 words: each store
 *    invalidates cse's memory entry for e->unk_08, so the target genuinely
 *    reloads it and recomputes the sll/addu four times, and only the array
 *    base survives in $s2.
 *
 * The early-return shape of the kind == 2 arm is a free choice: a nested
 * else-if matches too, as do re-reading e->unk_0A instead of keeping it in a
 * local and hoisting e->unk_0C. */
void func_80043FD0(Unk44144 *e) {
    s16 kind = e->unk_0A;

    if (kind == 2) {
        if (*(u8 *) e->unk_04 == 0) {
            return;
        }
        if (e->unk_0C != 0) {
            e->unk_08 = func_8003F794(5, e->unk_0C);
        }
        func_80040454();
        D_80080A86 = 0;
        e->unk_0A = 0;
        return;
    }
    if (D_80083120[e->unk_08].unk_00 != 0) {
        return;
    }
    if (kind != 0) {
        UnkNode *n = (*(UnkNode *(*)(u32)) e->unk_00)(e->unk_04);

        if (n == NULL) {
            D_80083120[e->unk_08].unk_00 = 6;
            D_80083120[e->unk_08].unk_02 = 1;
            D_80083120[e->unk_08].unk_04 = 1;
            D_80083120[e->unk_08].unk_06 = 0;
            return;
        }
        func_80040418();
        D_80080A86 = 1;
        n->unk_1E &= 0xF3FF;
    }
    *(u16 *) ((u8 *) e - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

UnkNode *func_80044144(s16 arg0, s16 arg1, u32 arg2, u32 arg3) {
    UnkNode *n = func_8003FC64(0);

    if (n != NULL) {
        Unk44144 *e = (Unk44144 *) (n + 1);

        n->unk_10 = (u32) func_80043FD0;
        if (arg0 == 1) {
            e->unk_08 = func_8003F794(6, arg1);
        } else if (arg0 == 0) {
            e->unk_08 = func_8003F794(5, arg1);
            func_80040454();
            D_80080A86 = 0;
        }
        e->unk_04 = arg3;
        e->unk_0A = arg0;
        e->unk_0C = arg1;
        e->unk_00 = arg2;
    }
    return n;
}

void func_80044234(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_8003F540(arg0, arg1, arg2, arg3);
    func_80053DA8(0x300);
}

/* arg0 is a short: the index and the `arg0 + 1` compared against the byte are
 * sign-extended, but the byte *written* only needs the low bits, so the raw
 * incoming register is reused -- which is what leaves the copy to $s2 free to
 * fill the beq's delay slot. Declaring arg0 s32 forces the copy up front and
 * the slot goes empty. */
void func_8004425C(s16 arg0) {
    s16 i = arg0;
    Unk8006E61C *e = &D_8006E61C[i];
    s16 n = e->unk_00 - 1;
    u8 *p = &D_80080AF0[n];

    if (*p == i + 1) {
        return;
    }
    func_800542BC();
    *p = arg0 + 1;
    DrawSync(0);
    D_80081480 = D_8008148C;
    func_8003E4FC(6, e->unk_08, 0);
    func_8003E4FC(6, e->unk_04, 0);
    func_8003F320();
    func_8003F5E0(D_8008148C);
    func_80053DA8((u16) ((2 << n) | 0x10));
    func_800542BC();
    do {
    } while (func_8005405C(e->unk_00) != 1);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004437C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004450C);

u32 func_800445E0(void) {
    Unk8006CE34 *p = func_8003F534();
    u32 t = (p->unk_08 & 0xFFFFFF) - 0x20;

    return t + p->unk_04;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044618);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044698);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800448BC);

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
s32 func_8004491C(UnkNode *n, u32 tag) {
    s32 first = -1;
    s32 last = 0;
    s32 i;
    s32 j;

    for (i = 0; i < 0x20; i++) {
        if (D_80083360[i] == tag) {
            UnkNode *h = (UnkNode *) D_800833E0[i];

            n->unk_18 = h;
            if (h != NULL) {
                h->unk_14 = n;
                D_800833E0[i] = (u32) n;
            } else {
                D_800833E0[i] = (u32) n;
            }
            n->unk_14 = (UnkNode *) &D_800833E0[i];
            n->unk_1C = i;
            return 1;
        }
        if (D_80083360[i] == 0) {
            if (first == -1) {
                first = i;
            }
            last = i;
        }
    }
    if (first >= 0) {
        n->unk_18 = NULL;
        j = 0;
        while (D_8006E7B4[j] != 0) {
            if (D_8006E7B4[j] == tag) {
                D_80083360[first] = tag;
                D_800833E0[first] = (u32) n;
                n->unk_14 = (UnkNode *) &D_800833E0[first];
                n->unk_1C = first;
                return 1;
            }
            j++;
        }
        D_80083360[last] = tag;
        D_800833E0[last] = (u32) n;
        n->unk_14 = (UnkNode *) &D_800833E0[last];
        n->unk_1C = last;
        return 1;
    }
    return 0;
}

void func_80044A50(UnkNode *n) {
    UnkNode *a = n->unk_14;
    UnkNode *b;

    if (a == NULL) {
        return;
    }
    b = n->unk_18;
    if (b != NULL) {
        b->unk_14 = a;
    }
    if ((u8 *) a >= (u8 *) &D_800833E0 &&
        (u8 *) a < (u8 *) &D_800833E0 + 0x80) {
        a->unk_00 = b;
    } else {
        a->unk_18 = b;
    }
    n->unk_18 = NULL;
    n->unk_14 = NULL;
}

void func_80044AAC(UnkNode *n, UnkNode *m) {
    /* One function-scope local shared by both arms, not a block-local in
     * each: set in two blocks it becomes a global allocno and lands in $a1,
     * which is the register the target uses in both arms. */
    UnkNode *p;

    func_80044A50(m);
    if (n != NULL) {
        p = n->unk_18;
        n->unk_18 = m;
        m->unk_14 = n;
        m->unk_18 = p;
        if (p != NULL) {
            p->unk_14 = m;
        }
    } else {
        p = (UnkNode *) D_800833E0[m->unk_1C];
        /* The store between the first two reads of unk_1C is what stops gcc
         * sharing the index: it cannot prove the store does not alias. */
        m->unk_18 = p;
        m->unk_14 = (UnkNode *) &D_800833E0[m->unk_1C];
        D_800833E0[m->unk_1C] = (u32) m;
        p->unk_14 = m;
    }
}

void func_80044B48(void) {
    u32 *p = *(u32 **) (D_80083160.unk_00 + 0x8D0);

    func_80067F20(p, 0, 0, 0, 0);
    AddPrim(D_80083160.unk_00 + 0x8AC, p);
    *(u32 **) (D_80083160.unk_00 + 0x8D0) = p + 3;
}

s32 func_80044BB0(u8 *p, UnkVec *v, UnkSlot *s) {
    u16 *scratch = (u16 *) 0x1F800000;
    /* One temporary for both halves: two separate single-set pseudos get
     * maximum scheduler priority and both loads float above both stores,
     * where the target pairs each load with its store. */
    u16 t;

    for (;;) {
        UnkNode *n;

        if (!(s->unk_14 & 0x80)) {
            t = ((u16 *) v)[1];
            scratch[0] = t;
            t = ((u16 *) v)[3];
            scratch[1] = t;
            func_80044D24(p, s, ((u16 *) v)[5] + s->unk_06);
        }
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

s32 func_80044C54(u8 *p, UnkVec *v, UnkSlot *s) {
    u8 *scratch = (u8 *) 0x1F800000;
    u16 t;

    for (;;) {
        UnkNode *n;

        if (!(s->unk_14 & 0x80)) {
            t = ((u16 *) v)[1];
            *(u16 *) (scratch + 0x70) = t;
            t = ((u16 *) v)[3];
            *(u16 *) (scratch + 0x72) = t;
            t = ((u16 *) v)[5];
            *(u16 *) (scratch + 0x74) = t;
            RotTransPers(scratch + 0x70, scratch, scratch + 0x90,
                         scratch + 0x94);
            func_80044D24(p, s, s->unk_06);
            if (func_80045310(*(u32 *) (D_80083160.unk_00 + 0x8D0)) != 0) {
                return 0;
            }
        }
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

/* Emits one sprite's pieces: builds the matrix, then per piece clamps the UV
 * pairs, converts the two signed byte offsets into four corner vectors,
 * projects each through the GTE and adds the primitive.
 *
 * The seven words this was parked on are the three byte-to-signed
 * conversions, and `volatile` is the answer -- which is worth stating plainly
 * because docs/matching-levers.md section 10 had `volatile` down as a dead
 * end. It is a dead end as a *nudge*; against this particular fold it is the
 * mechanism. extendqisi2's expander does force_not_mem under -O, so
 * `lbu` / `sll 24` / `sra 24` is the NATURAL expansion of the conversion and
 * combine is what refolds it to `lb`, via a 3-insn combine into
 * extendqisi2_internal, which wants a memory_operand.
 * gen_lowpart_for_combine returns a CLOBBER for a volatile MEM, so the refold
 * cannot happen.
 *
 * Two caveats make it awkward, and both are load-bearing here:
 *  - loop.c's find_mem_givs skips volatile MEMs, so a volatile FIELD is
 *    addressed off the biv where the target reads off the combined giv. The
 *    address therefore goes through a `volatile u8 *` local instead, which is
 *    an ordinary DEST_REG giv that combine_givs merges into the
 *    representative. Pointing the pointer at the field and dereferencing it
 *    directly costs 53 words, because cse eliminates the pointer.
 *  - the pointer must be assigned in the ENCLOSING block, and there is one per
 *    block (vp and wp hold the same address on purpose). Assigned in the same
 *    block as its use, cse folds it into the address before loop runs and no
 *    giv is formed. Shared across both blocks it is not `replaceable`
 *    (loop.c's last_use_this_basic_block), so loop.c leaves a `move` copy and
 *    cse2 propagates it away only within its path-length window -- which came
 *    out as one block's loads correct and the other's off the copy's register.
 *
 * Four more shapes, each measured by reverting it (all stay at 379 words):
 *  - the negation happens inside each arm, not at the join (8 words). That
 *    keeps the sra's destination a block-local temp, and jump2 cross-jumps the
 *    two `negu`s back into one at the join -- the duplicate-store trick of
 *    matching-levers section 1, applied to an arithmetic insn.
 *  - the arm intermediates are separate locals, `n` in the then-arm and `w` in
 *    the else-arm, not one `t` (15 words). The target has four live ranges
 *    there where one variable gives one allocno.
 *  - the e->unk_01 byte goes through its own `f`, not the clamps' `c`
 *    (7 words), which otherwise spans both as one allocno.
 *  - the clamp comparisons are written `prim->unk_08 > prim->unk_20`, not
 *    reversed (4 words): operand order decides which `lh` sched1 issues first.
 *
 * And from the earlier pass: `e = (Unk44D24E *) s->unk_08;` must sit between
 * the unk_14 update and the unk_24 store or the whole prologue's register
 * assignment changes; one scratchpad pointer local but literal constants for
 * the libgte arguments; D_80083160 reached both ways, pointer local declared
 * first so its lui is shared; store-then-overwrite clamps; arg0 unused and
 * arg2 s16; and one `Unk44D24E *e` with `e++`, which makes it the biv. */
void func_80044D24(u8 *arg0, UnkSlot *s, s16 arg2) {
    Unk80083160 *g = &D_80083160;
    Unk44D24SP *p = (Unk44D24SP *) 0x1F800000;
    u8 *base = D_80083160.unk_00;
    Unk44D24P *prim = *(Unk44D24P **) (base + 0x8D0);
    Unk44D24E *e;
    volatile u8 *vp;
    volatile u8 *wp;
    s32 t;
    s32 w;
    s32 n;
    s32 x;
    s32 y;
    u8 c;
    u8 f;

    p->unk_20 = (s32) (base + 0x70);
    p->unk_38 = 0x1000;
    p->unk_48 = 0;
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    PushMatrix();
    s->unk_14 |= 0x8000;
    e = (Unk44D24E *) s->unk_08;
    p->unk_24 = s->unk_14;
    p->unk_30 = s->unk_1C;
    p->unk_34 = s->unk_1E;
    p->unk_40 = p->unk_00 + s->unk_20;
    p->unk_44 = (s16) p->unk_02 + s->unk_22;
    TransMatrix((void *) 0x1F800050, (void *) 0x1F800040);
    RotMatrix(&s->unk_16, (void *) 0x1F800050);
    ScaleMatrix((void *) 0x1F800050, (void *) 0x1F800030);
    SetRotMatrix((void *) 0x1F800050);
    SetTransMatrix((void *) 0x1F800050);
    for (;;) {
        if ((e->unk_00 & 0x20) == 0) {
            x = e->unk_08;
            p->unk_08.w = x;
            y = e->unk_0A;
            p->unk_10.w = y;
            if (x + y >= 0x100) {
                p->unk_10.w = y - 1;
            }
            x = e->unk_09;
            p->unk_0C.w = x;
            y = e->unk_0B;
            p->unk_14.w = y;
            if (x + y >= 0x100) {
                p->unk_14.w = y - 1;
            }
            vp = (volatile u8 *) &e->unk_01;
            if (((e->unk_00 ^ p->unk_24) & 1) != 0) {
                if (s->unk_14 & 0x400) {
                    n = -((s8) vp[1] * 2);
                } else {
                    n = -((s8) vp[1]);
                }
                p->unk_80 = n;
                p->unk_70 = n;
                t = n - p->unk_10.h;
            } else {
                w = (s8) vp[1];
                if (s->unk_14 & 0x400) {
                    w = w * 2;
                }
                p->unk_80 = w;
                p->unk_70 = w;
                t = w + p->unk_10.h;
            }
            p->unk_88 = t;
            p->unk_78 = t;
            wp = (volatile u8 *) &e->unk_01;
            if (((e->unk_00 ^ p->unk_24) & 2) != 0) {
                if (s->unk_14 & 0x400) {
                    n = -((s8) wp[2] * 2);
                } else {
                    n = -((s8) wp[2]);
                }
                p->unk_7A = n;
                p->unk_72 = n;
                t = n - p->unk_14.h;
            } else {
                w = (s8) wp[2];
                if (s->unk_14 & 0x400) {
                    w = w * 2;
                }
                p->unk_7A = w;
                p->unk_72 = w;
                t = w + p->unk_14.h;
            }
            p->unk_8A = t;
            p->unk_82 = t;
            gte_ldv0(&p->unk_70);
            gte_rtv0();
            gte_stsv(&prim->unk_08);
            gte_ldv0(&p->unk_78);
            gte_rtv0();
            gte_stsv(&prim->unk_10);
            gte_ldv0(&p->unk_80);
            gte_rtv0();
            gte_stsv(&prim->unk_18);
            gte_ldv0(&p->unk_88);
            gte_rtv0();
            gte_stsv(&prim->unk_20);
            prim->unk_03 = 9;
            s->unk_14 &= 0x7FFF;
            p->unk_10.w = p->unk_10.w + p->unk_08.w;
            p->unk_14.w = (p->unk_14.w + p->unk_0C.w) << 8;
            p->unk_0C.w = p->unk_0C.w << 8;
            if (p->unk_24 & 0x100) {
                prim->unk_0E = s->unk_12;
            } else {
                prim->unk_0E = s->unk_12 + e->unk_06;
            }
            prim->unk_0C = p->unk_0C.h + p->unk_08.h;
            prim->unk_14.h = p->unk_0C.h + p->unk_10.h;
            if (s->unk_10 != 0) {
                prim->unk_16 = s->unk_10 + (e->unk_04 & 0xFF9F);
            } else {
                prim->unk_16 = e->unk_04;
            }
            prim->unk_1C.h = p->unk_14.h + p->unk_08.h;
            prim->unk_24.h = p->unk_14.h + p->unk_10.h;
            if ((s16) p->unk_50 >= 0x1800) {
                c = prim->unk_24.b[0];
                prim->unk_24.b[0] = c + 0xFF;
                prim->unk_14.b[0] = c;
            }
            if ((s16) p->unk_58 >= 0x1800) {
                c = prim->unk_24.b[1];
                prim->unk_24.b[1] = c + 0xFF;
                prim->unk_1C.b[1] = c;
            }
            if (prim->unk_08 > prim->unk_20) {
                c = prim->unk_24.b[0];
                prim->unk_24.b[0] = c + 0xFF;
                prim->unk_14.b[0] = c;
            }
            if (prim->unk_0A > prim->unk_22) {
                c = prim->unk_24.b[1];
                prim->unk_24.b[1] = c + 0xFF;
                prim->unk_1C.b[1] = c;
            }
            f = e->unk_01;
            s->unk_0C[3] = f;
            if (p->unk_24 & 8) {
                if (p->unk_24 & 4) {
                    s->unk_0C[3] = f | 2;
                } else {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            prim->unk_04 = *(u32 *) s->unk_0C;
            AddPrim((void *) (p->unk_20 + arg2 * 4), prim);
            prim++;
        }
        if ((s8) e->unk_00 < 0) {
            break;
        }
        e++;
    }
    PopMatrix();
    *(Unk44D24P **) ((u8 *) g->unk_00 + 0x8D0) = prim;
}

s32 func_80045310(u32 arg0) {
    /* D_80081478 must be declared with an incomplete array type: a complete
     * one of 8 bytes is small data, which makes the base an `la` constant and
     * folds the +4 into it, where the target bumps a register. */
    u32 *p = D_80081478;

    if (D_80083160.unk_00 != D_801C9E40) {
        p++;
    }
    return *p < arg0;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80045340);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800453E0);

s32 func_80045C34(u8 *p, UnkVec *v, UnkSlot *s) {
    for (;;) {
        UnkNode *n;

        if (!(s->unk_14 & 0x80)) {
            func_80045CC4(p, v, s, s->unk_06);
            if (func_80045310(*(u32 *) (D_80083160.unk_00 + 0x8D0)) != 0) {
                return 0;
            }
        }
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80045CC4);

s32 func_80046398(u8 *p, UnkVec *v, UnkSlot *s) {
    UnkNode *n;

    for (;;) {
        func_800463EC(p, v, s, s->unk_06);
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *) n->unk_0C;
    }
}

/* Transforms and emits one model's faces: builds the matrix, then per face
 * loads four vertices into the scratchpad, projects them with RotAverage4,
 * back-face-culls with NormalClip and adds the primitive.
 *
 * The load-bearing shape is that the scratchpad vertex and the vertex table
 * are both 4-halfword SVECTORs, and the x/y pair is copied as one u32 through
 * a cast while vz is copied as a struct field. That asymmetry pipelines the
 * x/y group exactly one deep and leaves the z group serial -- five stall nops,
 * which is the target. Which MEMs are casts and which are struct fields is
 * what decides it, and the four combinations are measured below; the mechanism
 * is NOT established, though. The obvious reading is
 * true_dependence's MEM_IN_STRUCT_P exception, but that needs the non-struct
 * side's address not to vary and here every base is a register, so something
 * else is feeding in -- most likely memrefs_conflict_p comparing constant
 * offsets off a shared base. See docs/matching-levers.md section 5. It is
 * also what the original would naturally write, since the word store spans two
 * fields and needs a cast while the vz store does not. Measured alternatives:
 * both sides fields 297 words with no pipelining at all, both sides casts 293,
 * stores as casts with loads as fields 286, the reverse 300, and literal
 * constant scratchpad addresses 294 words but 138 differing (every store
 * rebuilds `lui $at, 0x1F80`, so the matching count is a coincidence).
 *
 * unk_114 is volatile: the target stores NormalClip's result and reloads it
 * before the blez, and without volatile cse folds the reload away (3 words).
 *
 * Also established and worth not re-deriving: one pointer local for every
 * scratchpad field access but literal `(void *) 0x1F8000XX` constants for the
 * libgte calls, which is the mixing func_8003D0F0 and func_80044724 also need;
 * RotAverage4's arguments through the pointer local instead; D_80083160
 * reached both ways in one function, with the pointer local declared first so
 * cse shares the one `lui` (the same ordering constraint as func_80043A68);
 * tbl and tex read before PushMatrix; a three-case switch plus default, which
 * is below gcc's jump-table threshold so it builds the decision tree the
 * target has; and `break` out of `for (;;)` with the termination test at the
 * bottom. */
void func_800463EC(u8 *arg0, UnkVec *v, UnkSlot *s, s16 depth) {
    Unk80083160 *g = &D_80083160;
    Unk463ECSP *sp = (Unk463ECSP *) 0x1F800000;
    Unk463ECV *tbl = (Unk463ECV *) g->unk_1E4;
    u8 *tex = (u8 *) g->unk_1E8;
    u8 m[0x20];
    Unk463ECF *f;
    Unk463ECP *p;

    PushMatrix();
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_38 = s->unk_20;
    sp->unk_40 = ((s16 *) v)[1];
    sp->unk_44 = ((s16 *) v)[3];
    sp->unk_48 = ((s16 *) v)[5];
    ReadRotMatrix(m);
    TransMatrix((void *) 0x1F800050, (void *) 0x1F800040);
    RotMatrix(&s->unk_16, (void *) 0x1F800050);
    if (s->unk_14 & 1) {
        sp->unk_30 = -sp->unk_30;
    }
    if (s->unk_14 & 2) {
        sp->unk_34 = -sp->unk_34;
    }
    ScaleMatrix((void *) 0x1F800050, (void *) 0x1F800030);
    CompMatrix(m, (void *) 0x1F800050, (void *) 0x1F8000D0);
    SetTransMatrix((void *) 0x1F8000D0);
    SetRotMatrix((void *) 0x1F8000D0);
    sp->unk_20 = (u32) (D_80083160.unk_00 + 0xB0);
    p = (Unk463ECP *) *(u32 *) (D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    f = ((Unk463ECF **) g->unk_1E0)[s->unk_08];
    for (;;) {
        s32 x;
        s32 y;
        s32 z;

        *(u32 *) &sp->unk_70[0] = *(u32 *) &tbl[f->unk_00];
        *(u32 *) &sp->unk_70[1] = *(u32 *) &tbl[f->unk_02];
        *(u32 *) &sp->unk_70[2] = *(u32 *) &tbl[f->unk_04];
        *(u32 *) &sp->unk_70[3] = *(u32 *) &tbl[f->unk_06];
        sp->unk_70[0].unk_04 = tbl[f->unk_00].unk_04;
        sp->unk_70[1].unk_04 = tbl[f->unk_02].unk_04;
        sp->unk_70[2].unk_04 = tbl[f->unk_04].unk_04;
        sp->unk_70[3].unk_04 = tbl[f->unk_06].unk_04;
        sp->unk_C0 = RotAverage4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2],
                                 &sp->unk_70[3], &p->unk_08, &p->unk_10,
                                 &p->unk_18, &p->unk_20, &sp->unk_90, &sp->unk_94);
        switch (s->unk_14 & 3) {
        case 0:
            x = p->unk_08;
            y = p->unk_10;
            z = p->unk_18;
            break;
        case 1:
            x = p->unk_10;
            y = p->unk_08;
            z = p->unk_20;
            break;
        case 2:
            x = p->unk_20;
            y = p->unk_18;
            z = p->unk_10;
            break;
        default:
            x = p->unk_18;
            y = p->unk_20;
            z = p->unk_08;
            break;
        }
        sp->unk_114 = NormalClip(x, y, z);
        if (sp->unk_114 > 0) {
            sp->unk_C0 = sp->unk_C0 - depth;
            if ((u32) sp->unk_C0 < 0x1E0) {
                u16 t;

                func_80065034(tex + f->unk_10 * 8, s->unk_0C, (u8 *) &p->unk_04);
                *(u32 *) &p->unk_0C = f->unk_08;
                p->unk_0E = p->unk_0E + s->unk_12;
                *(u32 *) &p->unk_14 = f->unk_0C;
                t = s->unk_10;
                if (t != 0) {
                    p->unk_16 = t + (p->unk_16 & 0xFF9F);
                }
                p->unk_1C = f->unk_12;
                p->unk_24 = f->unk_14;
                SetPolyFT4(p);
                if (s->unk_14 & 8) {
                    if (s->unk_14 & 4) {
                        p->unk_07 |= 2;
                    }
                }
                AddPrim((void *) (sp->unk_20 + sp->unk_C0 * 4), p);
                p++;
                s->unk_14 &= 0x7FFF;
            }
        }
        if (f->unk_16 == 1 && f->unk_17 < 0) {
            break;
        }
        f++;
    }
    PopMatrix();
    *(u32 *) (g->unk_00 + 0x8D0) = (u32) p;
}

/* Projects the four corners of a rect through func_80046A5C, clips each
 * against the reference point in out[5] with func_80046AFC, and if any corner
 * was clipped replaces all four with a ring of offsets around the transformed
 * camera position; then copies out[0] into out[4].
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - the third parameter is s32, not s16 (8 DIFF). With s16, gcc splits the
 *    parameter copy into an SImode copy plus a HImode subreg copy, and that
 *    second insn is schedulable, so it drifts to the end of the prologue
 *    block; the target has the plain SImode copy pinned right after its
 *    register save. The sll/sra in the loop is func_80046AFC's own s16
 *    conversion, not a cast here.
 *  - func_80064584/func_800644B8 take s32 (120 words otherwise): `ang` is s32,
 *    so an s16 parameter makes gcc sign-extend at both call sites.
 *  - `origin` is two explicit word stores into a u32[2]. Every aggregate
 *    initialiser spelling calls bzero instead (126 words), which also forces
 *    `r` out of $a0 into a callee-saved register; three `sh` field zeroes on an
 *    Unk46AFC cost 99 words.
 *  - the origin zeroing sits before the ReadGeomScreen statement, not just
 *    before the first func_80046A5C call (22 DIFF): the two stores have to be
 *    in the prologue block so sched1 can use them to cover the
 *    `lhu $v1,0x6($a0)` load delay.
 *  - the corner stores are chained assignments, in this direction. Eight
 *    separate statements cost 128 words; `A = B = v` stores B first, and the
 *    target stores the higher address of each pair first, so reversing any
 *    pair is 8 DIFF. r[0] and r[1] really are reloaded for the second and
 *    fourth statements -- the intervening stack stores invalidate cse's memory
 *    table -- so they must not be hoisted into locals.
 *  - the z store is one four-deep chain (119 words otherwise); it evaluates
 *    innermost-first, which gives the target's descending 0x2C/0x24/0x1C/0x14.
 *  - D_80083160 is reached through a pointer local declared at the top (116
 *    words otherwise), as in func_8003D5A4: the whole address is materialised
 *    in ReadGeomScreen's delay slot and carried in $s6 across every call even
 *    though its first use is inside the if.
 *  - the tail copy is field-by-field; `out[4] = out[0];` is an 8-byte block
 *    move where the target has three lhu/sh pairs.
 *
 * arg0 is a four-halfword x/y/w/h rect, the same shape func_800404F8 and
 * LoadImage take; if the project ever grows a RECT type those three belong
 * together. */
void func_80046884(u16 *r, Unk46AFC *out, s32 t) {
    Unk46AFC c[4];
    Unk46AFC xf[4];
    u32 origin[2];
    u16 v58[4];
    u16 v60[4];
    s32 i;
    s32 flag = 0;
    Unk80083160 *g = &D_80083160;

    c[0].unk_00 = c[3].unk_00 = r[0];
    c[1].unk_00 = c[2].unk_00 = r[0] + r[2];
    c[0].unk_02 = c[1].unk_02 = r[1];
    c[3].unk_02 = c[2].unk_02 = r[1] + r[3];
    origin[0] = 0;
    origin[1] = 0;
    c[0].unk_04 = c[1].unk_04 = c[2].unk_04 = c[3].unk_04 = ReadGeomScreen();
    func_80046A5C((u16 *) origin, &out[5]);
    for (i = 3; i >= 0; i--) {
        func_80046A5C((u16 *) &c[i], &xf[i]);
        if (func_80046AFC(&out[5], &xf[i], &out[i], t) != 0) {
            flag = 1;
        }
    }
    if (flag != 0) {
        s32 ang = g->unk_C8 + 0x600;

        v58[0] = g->unk_AC;
        v58[1] = g->unk_AE;
        v58[2] = g->unk_B0;
        func_80046A5C(v58, v60);
        for (i = 3; i >= 0; i--) {
            s32 cc = func_80064584(ang);
            s32 ss = func_800644B8(ang);

            ang += 0x400;
            out[i].unk_00 = v60[0] + (cc >> 3) + (cc >> 4);
            out[i].unk_02 = v60[1] + (ss >> 3) + (ss >> 4);
            out[i].unk_04 = v60[2];
        }
    }
    out[4].unk_00 = out[0].unk_00;
    out[4].unk_02 = out[0].unk_02;
    out[4].unk_04 = out[0].unk_04;
}

void func_80046A5C(u16 *arg0, void *out) {
    u16 m[0x10];
    u16 t[0x10];
    s16 v[4];

    ReadRotMatrix(m);
    PushMatrix();
    v[0] = arg0[0] - m[10];
    v[1] = arg0[1] - m[12];
    v[2] = arg0[2] - m[14];
    TransposeMatrix(m, t);
    ApplyMatrixSV(t, v, out);
    PopMatrix();
}

/* Interpolates a/b at height t into out, clamps both coordinates to
 * +/-0x7FF0, and returns 1 if the result lies more than 0x1000 from a in
 * either axis.
 *
 * Three shapes are load-bearing:
 *  - `dx` is hoisted into its own local one statement before `f`. That costs
 *    no instruction (sched1 still issues f's subtract first) but it pushes
 *    the last use of the CSE'd b->unk_04 load one slot later, so its live
 *    range grows from 11 insns to 12 and its global.c priority drops just
 *    below b's own. b is then allocated first and takes $a0, the load is
 *    pushed to $a1, and a/out/t slide down to $t1/$t2/$t3 as in the target.
 *  - cx's clamp is the preset-then-nested form and cy's is the if/else form
 *    with the -0x7FF0 arm in the else. They are not interchangeable: that
 *    asymmetry is what makes both clamps' branch layout and delay slots come
 *    out right.
 *  - cx and cy have to be separate variables; one shared variable is
 *    coalesced with nx and costs a `j`. */
s16 func_80046AFC(Unk46AFC *a, Unk46AFC *b, Unk46AFC *out, s16 t) {
    s32 dz = b->unk_04 - a->unk_04;
    s32 f;
    s32 nx;
    s32 ny;
    s32 cx;
    s32 cy;
    s32 dx;

    if (dz < 0x40) {
        return 1;
    }
    dx = b->unk_00 - a->unk_00;
    f = t - b->unk_04;
    nx = dx * f / dz + b->unk_00;
    ny = (b->unk_02 - a->unk_02) * f / dz + b->unk_02;
    cx = -0x7FF0;
    if (nx >= -0x7FF0) {
        cx = 0x7FF0;
        if (nx < 0x7FF1) {
            cx = nx;
        }
    }
    out->unk_00 = cx;
    if (ny >= -0x7FF0) {
        cy = 0x7FF0;
        if (ny < 0x7FF1) {
            cy = ny;
        }
    } else {
        cy = -0x7FF0;
    }
    out->unk_02 = cy;
    out->unk_04 = t;
    if (out->unk_00 < a->unk_00 - 0x1000) {
        return 1;
    }
    if (a->unk_00 + 0x1000 < out->unk_00) {
        return 1;
    }
    if (out->unk_02 < a->unk_02 - 0x1000) {
        return 1;
    }
    return a->unk_02 + 0x1000 < out->unk_02;
}

/* Builds four per-edge records from five points, clamping each y to a
 * multiple of 0x40 and returning the lowest y seen, likewise clamped.
 * Degenerate pairs are marked with unk_1C = -1 and decrement *cnt.
 *
 * Load-bearing shapes:
 *  - `dx = abs(dx);` rather than `if (dx < 0) { dx = -dx; }`. The MIPS abssi2
 *    pattern is a single insn whose template contains its own `bgez`/`subu`,
 *    so it introduces no basic-block boundary and reorg cannot fill the branch
 *    delay slot inside it -- which is why the target has a bare nop there.
 *    With the abs as one insn the whole else-arm tail is a single block, so
 *    sched1 can hoist the out->unk_14 reload (emitted at the `> 0` test, where
 *    cse genuinely cannot fold it because the unk_20/unk_08 stores invalidated
 *    the entry) up to immediately after the store. Written as an if, the
 *    reload is stuck in the join block. It also settles the $v0/$v1 choice on
 *    the two loads feeding dx. Same pattern as func_8003F6F4.
 *  - `out` is a pointer incremented by the loop, with every field reached as
 *    out->field. That is what gives the target's two induction registers: $a1
 *    is the biv and $t0 = $a1 + 0x1C the one surviving address giv. Indexing
 *    as out[i].field makes gcc use a single giv at +0 with displacements
 *    0..0x24.
 *  - `out->unk_1C = 0;` is the LAST out statement in the else arm even though
 *    the emitted code shows it in the middle. gcc builds the giv list by
 *    prepending as it discovers them, so the last access becomes
 *    combine_givs' representative and sets the surviving pointer's +0x1C base;
 *    written earlier the base comes out +0x24.
 *  - `out->unk_04 = out->unk_0C << 7;` reads the field back rather than using
 *    `dy << 7`, which is what produces the copy-then-shift pair instead of a
 *    single `sll`, and it also settles min/base into $t3/$t4.
 *  - `s32 m = -0x40;` is a local declared in the loop BODY, used for both the
 *    mask and the -0x40 store but not for the return. The mask has to reach
 *    the `and` as a loop-invariant register: as a literal in both places gcc
 *    keeps the constant inside the loop and combine narrows it to an
 *    `andi 0xffc0` immediate using nonzero_bits of the lhu. loop.c hoists the
 *    local's single set into the preheader, which sits after `i = 0`, giving
 *    the target's register order. At function scope the set is emitted ahead
 *    of `i = 0`; in the else arm it is not a movable of the loop proper and
 *    costs 26 words.
 *  - `min` is s32; as s16 both the compare and the return need their own
 *    sll/sra pair. */
s32 func_80046C20(Unk46C20A *a, Unk46C20B *out, u16 *cnt) {
    s32 min = 0x7FFF;
    s32 i;

    for (i = 0; i < 4; i++, out++) {
        s32 d = a[i].unk_02 - a[i + 1].unk_02;
        s32 m = -0x40;

        if (d == 0) {
            out->unk_1C = -1;
            *cnt = *cnt - 1;
        } else {
            s32 lo;
            s32 hi;
            s32 y;
            s32 dy;
            s32 dx;

            if (d < 0) {
                lo = i;
                hi = i + 1;
            } else {
                lo = i + 1;
                hi = i;
            }
            out->unk_00 = a[lo].unk_00;
            y = (s16) ((u16) a[lo].unk_02 & m);
            out->unk_18 = y;
            if (y < min) {
                min = y;
            }
            dy = a[hi].unk_02 - y;
            out->unk_0C = dy;
            out->unk_10 = dy;
            out->unk_04 = out->unk_0C << 7;
            dx = a[hi].unk_00 - out->unk_00;
            out->unk_14 = dx;
            dx = abs(dx);
            out->unk_20 = dx;
            out->unk_08 = dx << 7;
            if (out->unk_14 > 0) {
                out->unk_14 = 0x40;
            } else if (out->unk_14 < 0) {
                out->unk_14 = m;
            }
            out->unk_20 = out->unk_20 + 0x20;
            out->unk_24 = out->unk_20;
            out->unk_1C = 0;
        }
    }
    return min & ~0x3F;
}

void func_80046D64(u8 *base, s16 arg1) {
    Unk80083160 *s = &D_80083160;
    u32 *p = &s->unk_1DC;
    u32 *q;
    u32 v;
    s32 i;

    /* v is used twice, so combine cannot fold the copy into q and q is a loop
     * variable (a global allocno) so local-alloc cannot tie them: the
     * `addu $a2,$v0,$zero` survives, which renumbers the allocnos enough that
     * p takes $a0 and base is pushed to $t0. */
    v = (u32) (base + D_8006E7F0[arg1][4] * 4);
    p[1] = v;
    q = (u32 *) v;
    for (i = D_8006E7F0[arg1][3]; i > 0; i--) {
        *q += (u32) base;
        q++;
    }
    p[2] = (u32) (base + D_8006E7F0[arg1][5] * 4);
    p[3] = (u32) (base + D_8006E7F0[arg1][6] * 4);
    p[4] = (u32) (base + D_8006E7F0[arg1][7] * 4);
}

/* Streams a run of compressed blocks through the unpacker, then hands the
 * result to func_80046D64.
 *
 * Load-bearing shapes, each measured by reverting it:
 *  - D_80080A7C is written by two separate stores in an if/else, not from a
 *    variable. Every single-store spelling costs 30 words: with one store the
 *    jump optimizer hoists the 0x8000 assignment above the lbu, so the pseudo
 *    is live across the comparison, conflicts with $v0/$v1 and lands in $a0,
 *    and sched2 is then free to hoist the (base+3)&~3 chain in front of the
 *    store. As two stores, cross-jumping merges the common `sw` and leaves
 *    each arm's constant in its own block, so the value is defined after the
 *    comparison, takes $v0, and the anti-dependence pins the store ahead of
 *    the addiu. It also recovers the word that the duplicated
 *    `addiu $a0,$zero,0x6` costs, which reorg steals from the loop top into
 *    the bgtz's delay slot.
 *  - `base = D_80081480 = D_8008148C;` as a chained store (78 words
 *    otherwise). Same store_expr-with-want_value pattern as func_80041284:
 *    load to temp, store, copy -- and the copy fills DrawSync's delay slot.
 *  - `n--` after the func_8004068C call, not before it (4 words otherwise):
 *    it has to be the insn immediately before the jal so reorg takes it for
 *    the delay slot instead of the $a1 argument setup.
 *  - the table entry is re-dereferenced at each use rather than cached in a
 *    local (55 words otherwise). The two stores to the stack local invalidate
 *    cse's memory table, so those loads are genuinely reloaded; only the
 *    address arithmetic survives in a register.
 *  - `buf = *...->unk_00;` as a whole-struct assignment, which is gcc's
 *    8-byte movstrsi (lw/lw/sw/sw); two field copies cost 6 words.
 *  - arg0 is s16 (76 words otherwise): the raw parameter stays in $s0 and each
 *    use sign-extends it separately, once by 14 for the *4 index and once by
 *    16 for the func_80046D64 argument.
 *  - the parameter arg1 is what the loop walks and a separate `orig` keeps the
 *    original (12 words the other way round).
 *  - D_80082E60.unk_0A, not a scalar at D_80082E6A -- the struct-field read is
 *    what puts the %hi in a different register from the lbu, as recorded for
 *    func_8003E14C.
 *  - `while (n > 0)`, not `!= 0`, with n a plain s32 holding the lhu-loaded
 *    count, so the test is blez/bgtz and the decrement needs no masking. */
void func_80046E38(s16 arg0, u8 *arg1) {
    Unk46E38A buf;
    u8 *orig = arg1;
    u32 base;
    s32 n;
    s32 addr;

    base = D_80081480 = D_8008148C;
    DrawSync(0);
    buf = *((Unk8006E7F0 *) D_8006E7F0[arg0])->unk_00;
    n = ((Unk8006E7F0 *) D_8006E7F0[arg0])->unk_04;
    addr = ((Unk8006E7F0 *) D_8006E7F0[arg0])->unk_00->unk_04;
    while (n > 0) {
        func_8003E4FC(6, (u8 *) &buf, 0);
        addr += 0x10;
        func_8003F320();
        arg1 = func_8004068C((u8 *) D_80081480, arg1);
        n--;
        func_8003F6D4(0x10, 0, (s32 *) &buf, addr);
    }
    func_80046D64(orig, arg0);
    if (D_80082E60.unk_0A == 2) {
        D_80080A7C = 0x4000;
    } else {
        D_80080A7C = 0x8000;
    }
    D_80081480 = (base + 3) & ~3;
}

s32 func_80046F88(u32 *p) {
    s32 delta = 0;
    s32 i;
    u32 *q;

    /* Single exit returning delta, and 0xC updated before 0x14: the
     * scheduler emits that pair in the opposite order to the source. */
    if (p != (u32 *) p[9]) {
        delta = (s32) ((u8 *) p - (u8 *) p[9]);
        p[9] = (u32) p;
        p[0] += delta;
        p[2] += delta;
        p[3] += delta;
        p[5] += delta;
        p[7] += delta;
        q = (u32 *) p[0];
        for (i = 0; i < (s32) p[1]; i++) {
            *q += delta;
            q++;
        }
        while ((u32) q < p[2]) {
            q[1] += delta;
            q += 2;
        }
    }
    return delta;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047054);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004713C);

void func_80047200(u16 *p, s32 rows, s32 flag) {
    s32 i;
    s32 j;

    if (flag == 0) {
        for (i = 0; i < rows; i++) {
            /* p++ first in the body, not last: the back-edge bnez then finds
             * `addiu $a0,$a0,2` at its target, and reorg's
             * fill_slots_from_thread steals it into the delay slot, redirects
             * the branch past it and emits the compensating addiu -2. */
            p++;
            for (j = 1; j < 0x10; j++) {
                *p |= 0x8000;
                p++;
            }
        }
        return;
    }
    for (j = 1; j < 0x100; j++) {
        *p |= 0x8000;
        p++;
    }
}

void func_80047270(u8 *arg0, s16 *rect) {
    s32 t = rect[3] * 2;
    s32 n = rect[2] * t;

    if (D_8008148C + D_80080A7C <= D_80081480 + n) {
        D_80081480 = D_8008148C;
        DrawSync(0);
    }
    func_800407C0(arg0, (u32 *) D_80081480);
    LoadImage(rect, (u32 *) D_80081480);
    t = rect[3] * 2;
    D_80081480 = D_80081480 + rect[2] * t;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047338);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047468);

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
