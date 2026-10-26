/* Another piece of the original unk_14934, carved off when the D_800814E8
 * $gp cluster was taken out of the middle of it. Named for its start offset.
 * See docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
s32  func_80042900(s8 *, s8);
extern u8 D_800E1D55[];
extern u8 D_800E1D61[];
extern u8 D_800E1D80[];
extern u8 D_800E1D9F[];
extern u8 D_800E1DCF[];
extern u8 D_800E1DEE[];
extern u8 D_800E1E0D[];
extern u8 D_800E1E32[];
extern u8 D_800E1E4B[];
extern u8 D_800E1E6A[];
extern u8 D_800E1E8C[];
extern u8 D_800E1EC1[];
extern u8 D_800E1EF7[];
extern u8 D_800E1F1C[];
extern u8 D_800E1F41[];
extern u8 *D_800E3D7C;
extern u8 *D_800E3DF0[];
typedef u8 *(*Fn42BDC)(s32, s32, s32, s16);
void *func_800990FC(void);
void *func_80099734(u8 *, void *);
void *func_80099194(u8 *, void *);
void func_80099290(void *);
void func_800A5720(void *);
void func_800AA53C(u8 *);
void func_800D5460(u8 *, u32, s32);
u8 *func_800A1618(s32, s16);
void func_8009A3D0(s32, s32, s32);
void func_8009A21C(s32, s32, s32);
Fn42BDC func_800A0B94(s32, u8 *, s32);
void func_800A9A0C(u8 *);
void func_800AA888(u8 *, u8 *, u8 *, u8 *);
void func_800AC82C(u8 *, u8 *, u8 *, u8 *);
void func_800A32A4(u8 *);
void func_8009A028(u8 *);
void func_800AA36C(u8 *, u8 *, u8 *, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u8 *, u32 *);
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

    PutDispEnv((DISPENV *)(D_80083160.unk_00 + 0x5C));
    PutDrawEnv((DRAWENV *)D_80083160.unk_00);
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

INCLUDE_ASM("asm/nonmatchings/unk_154BC", func_80041E70);

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

/* Snapshots the four (code, value) status-effect pairs at +0x2C, cancels each
 * one through func_80042518 with the 0x80013714 "no draw" bit forced on, then
 * re-applies them through func_800A48F0 with the bit restored.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *
 *  - `tmp[i] = *(Unk42710B *) (arg0 + i * 2 + 0x2C);` is a whole-struct
 *    assignment on purpose, and it is what this function was parked on. A
 *    small byte-aligned block copy is ONE RTL insn, `movstrsi_internal`, which
 *    survives combine, sched1, global-alloc and sched2 intact; the lb/lb/sb/sb
 *    only appear from its output template at final. So the two `sb`s cannot be
 *    scheduled against the `lb` of the test that follows, and that load's
 *    delay slot stays a nop. Copying the two members one at a time costs 44
 *    words, and one extra word, because sched1 then steals the second `sb` for
 *    the branch delay slot.
 *  - the source address is written out as `arg0 + i * 2 + 0x2C` at each
 *    access, as in func_80042518. A `Unk42710B *src = (Unk42710B *) (arg0 +
 *    0x2C);` local costs 51 words: the giv's initial value becomes arg0+0x2C,
 *    so the walker is `addiu $s2,$s3,0x2C` with 0/1 displacements where the
 *    target has `addu $s2,$s3,$zero` with 0x2C/0x2D.
 *  - both loops are `for` loops and both use the same `i`. A backward `goto`
 *    loop costs 52 words, because the loop notes rescale every priority; a
 *    separate counter for the second loop costs 9, because `i` is then two
 *    allocnos of 7 references instead of one of 14.
 *  - Unk42710B's members are `s8`; `u8` costs 36 words.
 *  - the 0x80013714 flags are genuinely mixed within the one function: a
 *    constant base pointer for the first two accesses and the `extern u16
 *    D_80013714` scalar for the third. `*(u16 *) 0x80013714` gives lui+ori
 *    plus a 0($v1) displacement, and `extern u16 D_80013714[]` splits all
 *    three accesses and rematerialises the third into $v0.
 *
 * The block move is also what puts the three `$s` registers right, and it is
 * the reference-count lever running downward: it removes one in-loop reference
 * (weight 2 at loop_depth 2) from each address pseudo, taking the arg0 walker
 * from 9 references to 7. That crosses the floor_log2 step at 8, which halves
 * its priority from 1.80 to 1.17 and drops it below the counter; the tmp
 * pointer's 11->9 crosses nothing and it stays above. The allocation order
 * becomes loop-2-tmp, loop-1-tmp, counter, walker -- $s0, $s0, $s1, $s2, with
 * the two tmp pointers sharing $s0 because their loops are disjoint.
 *
 * Not load-bearing, measured: caching the reload in an `s8` local, and
 * `if (...) continue;` instead of the positive `if`. Both still match. */
void func_80042560(s8 *arg0) {
    u8 *base = (u8 *) 0x80010000;
    Unk42710B tmp[4];
    u16 saved;
    s32 i;

    saved = *(u16 *) (base + 0x3714);
    *(u16 *) (base + 0x3714) = saved | 2;
    func_80042518(arg0, 0x1B);

    for (i = 0; i < 4; i++) {
        tmp[i] = *(Unk42710B *) (arg0 + i * 2 + 0x2C);
        if (tmp[i].unk_00 != 0) {
            func_80042518(arg0, tmp[i].unk_00);
        }
    }
    for (i = 0; i < 4; i++) {
        if (tmp[i].unk_00 != 0) {
            func_800A48F0(arg0, tmp[i].unk_00, tmp[i].unk_01);
        }
    }
    D_80013714 = saved;
}

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

/* Looks up the status-effect slot matching arg1 and returns its magnitude, or
 * a code for two special effect numbers.
 *
 * THE LAST WORD WAS A `reorg` DECISION, and the rule generalises.
 * `own_thread_p` decides whether the insn reorg puts in a delay slot is
 * STOLEN -- the original deleted -- or COPIED, leaving the original behind and
 * costing a word. It fails when the thread is reachable by fall-through, so the
 * shape to aim for is a shared epilogue insn that has its OWN thread. Here that
 * means the 0xD arm falls off the end of the function while the `else` holds
 * the `return 0`: gcc then lays out `bne -> L_else` / `lw` / `jump L_end` /
 * barrier / `L_else: v0 = 0` / `L_end:`, `own_thread_p` holds, reorg steals the
 * `addu $v0,$zero,$zero` into the bne's delay slot and deletes the original,
 * and the arm's jump-to-next goes away. Written the other way round -- the 0xD
 * arm owning the only `return 0` -- the insn is copied instead: 34 words.
 *
 * The C is formally UB on the 0xD path, which falls off the end of a
 * value-returning function. The emitted code returns 0 anyway, because the
 * stolen insn sits in a non-annulled delay slot and runs on both paths -- and
 * that is exactly what the target's single `addu $v0,$zero,$zero` means. The
 * original almost certainly wrote it this way.
 *
 * Two more shapes are load-bearing. `t` is reused for both the effect number
 * and the dead volatile load, which is what puts that load in $v1. And the
 * inner early-return order is `if (byte == 0) return 0x100; return byte;` --
 * the opposite nesting from the obvious one -- which gives the `beqz` with
 * `addiu $v0,0x100` in its delay slot. That part of the loop only becomes
 * order-sensitive once the tail above is right. */
s32 func_80042900(s8 *arg0, s8 arg1) {
    s32 i;
    s32 t;

    if (arg1 != 0) {
        for (i = 3; i >= 0; i--) {
            if (arg0[i * 2 + 0x2C] == arg1) {
                if (arg0[i * 2 + 0x2D] == 0) {
                    return 0x100;
                }
                return arg0[i * 2 + 0x2D];
            }
        }
    }
    t = arg1;
    if (t == 0xE) {
        return ((u8 *) arg0)[0x28] == 0;
    }
    if (t == 0xD) {
        t = *(volatile u32 *) (arg0 + 0x14);
    } else {
        return 0;
    }
}

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

/* Returns the 0x400 flag of the record addressed by p, with two special cases
 * keyed on p[1], and 0 unless the global mode is 2.
 *
 * THE LAST DIFFERENCE WAS ONE BIT -- `0x00021402` against `0x00021403`, bit 0
 * of a SPECIAL shift's funct field: SRL 0b000010 against SRA 0b000011 -- and
 * the mechanism behind it corrects what this function was parked on for two
 * sessions.
 *
 * combine does NOT downgrade `sra` to `srl` when it looks at the extension
 * insn. It downgrades it when it SUBSTITUTES that insn into its single
 * consumer, which here is the copy into the hard return register $v0. At that
 * moment `simplify_shift_const` sees `(ashiftrt (reg) 16)` whose varop is a
 * plain REG, consults `nonzero_bits` through `reg_last_set_nonzero_bits`, finds
 * bit 31 clear and rewrites the code to LSHIFTRT. (combine never simplifies the
 * sra on its own: `try_combine` of the two shifts fails, because the merged
 * `(sign_extend:SI (subreg:HI ...))` has no MIPS pattern -- which is why both
 * shifts survive at all.)
 *
 * So the lever is to DENY that substitution, and a basic-block boundary buys it
 * for free, because combine's LOG_LINKS are built per block. Assigning the
 * value to a shared result variable and returning it once -- the `else { r = 0;
 * }` below -- puts the copy into $v0 in a later block, combine never
 * re-examines the insn, `nonzero_bits` is never consulted, and the `sra`
 * survives. A `goto out;` form matches identically. The value stays
 * SINGLE-SET, so none of the `andi 0xFFFF` that the multi-set shapes produce
 * appears.
 *
 * **This supersedes the old rule in docs/matching-levers.md section 10 that a
 * single-set masked value cannot keep its `sra` and that multi-set costs a
 * word.** The real rule is that the extension's result must not be combinable
 * into its consumer. It should transfer to func_8003D0F0's four `(s8)`
 * conversions, which are parked on the same question.
 *
 * verify.py flags two relocated words: `D_80082E60.unk_0A` for 0x80082E6A and
 * `D_80073414[4].unk_0C` for 0x80073470. Both resolve to the listing's bytes. */
s32 func_80042A80(u8 *p) {
    s32 r;

    if (D_80082E60.unk_0A == 2) {
        u8 k = p[1];

        if (k == 4) {
            s32 v = ((*(u16 *) (D_80073414[4].unk_0C + p[0] * 0x14) & 0x400) != 0) << 10;

            if (p[3] & 0x80) {
                v |= 0x80;
            }
            return v;
        }
        if (k >= 0xF && k <= 0x11) {
            return p[3] & 0x80;
        }
        if (D_80073414[p[1]].unk_00 == 0) {
            r = (s16) (*(u16 *) (D_80073414[p[1]].unk_0C + p[0] * 0x14) & 0x400);
        } else {
            r = 0;
        }
    } else {
        r = 0;
    }
    return r;
}

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

/* Dispatches one of 32 status-effect expiry handlers for the entity at arg0:
 * clears the effect's bit in the flag word at +0x1C and, when the entity is
 * visible (+0x14 & 0x4000), builds a message through the overlay's
 * func_800990FC / func_80099734 / func_80099194 / func_80099290 /
 * func_800A5720 idiom to announce it.
 *
 * It dispatches through jtbl_8002D67C, whose 32 entries fix the source order
 * of the arms. 0x80043430 is both the default label and the out-of-range
 * target, so the eleven values 13-17, 24, 25 and 28-31 carry no `case` label
 * at all, and cases 21, 22 and 23 share one body. The recovered order is in
 * the jump-table section of docs/open-matching-problems.md.
 *
 * This source is a verify.py MATCH and `make check` confirms the table lands,
 * but **its load-bearing shapes are not written up**: the subagent that found
 * it was stopped by a usage limit immediately after reporting the match, so
 * only the source survived. The near-miss file it replaces -- 543 of 543
 * words, 14 DIFF, all in case 10 -- is in git history if the diff is wanted.
 * Treat the shapes here as unexplained rather than as deliberate, and if you
 * change anything in it, re-run `make check` rather than trusting verify.py:
 * verify.py masks the two relocated halfwords that reference the table. */
void func_80042BDC(s8 *arg0, s16 arg1) {
    u8 *p = (u8 *) arg0;
    void *msg;
    u8 *e;
    s32 k;

    switch (arg1) {
    case 1:
        *(u32 *) (p + 0x1C) &= ~0x200;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D55, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 2:
        *(u32 *) (p + 0x1C) &= ~0x400;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D61, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 3:
        *(u32 *) (p + 0x1C) &= ~0x10;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D80, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        if (p[0x13] != 0) {
            func_800AA53C(p);
        }
        break;
    case 4:
        *(u32 *) (p + 0x1C) &= ~0x20;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1D9F, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 5:
        *(u32 *) (p + 0x1C) &= ~0x40;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DCF, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 6:
        *(u32 *) (p + 0x1C) &= ~0x80;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1DEE, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 7:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E0D, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 8:
        *(u32 *) (p + 0x1C) &= ~0x800;
        break;
    case 9:
        *(u32 *) (p + 0x1C) &= ~0x1000;
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E32, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 10: {
        if (p[0x13] == 0x2E || p[0x13] == 0) {
            func_800D5460(p - 0x20, 0x20A0A0, 0x613);
            if (p[0x13] == 0) {
                goto done10;
            }
        }
        if (p[0x13] == 0x2E) {
            u8 *q;
            u8 *nw;
            Fn42BDC fn;
            s32 s3;
            u8 bit;
            u8 *g;

            e = *(u8 **) (p - 0x14);
            k = 1;
            p[0x13] = p[0xA8];
            if (*(u32 *) (p + 0x14) & 0x4000) {
                k = 3;
            }
            q = func_800A1618(p[0x13], (s16) k);
            *(u32 *) (p + 0x1C) |= 0x10000;
            s3 = (*(u32 *) (p + 0x14) >> 14) & 1;
            func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8004397C(p);
            fn = func_800A0B94(p[0x13], q, 1);
            func_8003F320();
            nw = fn(s3, e[0x24], e[0x25], *(s16 *) (p + 0x88));
            *(u32 *) (nw + 0x14) = 0;
            *(u32 *) (nw + 0x1C) = 0;
            bit = (*(u32 *) (p + 0x1C) >> 19) & 1;
            func_80042710((Unk42710 *) nw, (Unk42710 *) p);
            func_80042984(nw);
            g = (u8 *) &D_80083460;
            if (*(u16 *) (g + 2) & 0x1000) {
                if (p[0x71] > *(s16 *) (p + 0x8A)) {
                    *(u16 *) (g + 8) -= p[0x71] - *(u16 *) (p + 0x8A);
                }
            }
            func_800A9A0C(nw);
            *(u32 *) (nw + 0x1C) &= ~0x10000;
            if (*(u32 *) (p + 0x14) & 0x4000) {
                k = func_800A1BD0(p);
                D_800E3DF0[(*(u8 **) (((s16) k) * 4 + D_800E3D7C + 0xD0))[3] & 0x1F] = nw;
                *(u8 **) (((s16) k) * 4 + D_800E3D7C + 0xAC) = nw;
            }
            if ((bit & 1) != 0) {
                u8 *w = D_800E3D7C;

                *(u8 **) (w + 0x124) = nw;
                *(u8 **) (nw + 0x60) = w;
                *(u32 *) (nw + 0x1C) |= 0x80000;
                func_800AA888(nw, *(u8 **) (nw - 0x18), *(u8 **) (nw - 0x14), nw);
                func_800AC82C(nw, *(u8 **) (nw - 0x18), *(u8 **) (nw - 0x14), nw);
            } else if (nw[0x25] == 0) {
                u8 *g2 = (u8 *) &D_80083460;

                *(u16 *) (g2 + 0xA) += 1;
                *(u32 *) (nw + 0x1C) &= ~8;
            }
            *(u32 *) (p + 0x14) |= 0x20000000;
            func_800A32A4(p);
            func_8009A028(p);
            *(u16 *) (p - 2) |= 0x8000;
            D_800814A0 |= 0x8000;
            p = nw;
        }
    done10:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E4B, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    case 27:
        e = *(u8 **) (p - 0x14);
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_800AA36C(p, *(u8 **) (p - 0x18), e, p);
        break;
    case 11:
        e = *(u8 **) (p - 0x14);
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *) (p + 0x1C) &= ~0x2000;
        func_8009A21C(e[0x24], e[0x25], 0x3000);
        break;
    case 12:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E6A, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        e = *(u8 **) (p - 0x14);
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *) (p + 0x1C) |= 0x2000;
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 26:
        e = *(u8 **) (p - 0x14);
        *(u16 *) (p + 0x98) &= 0x7FFF;
        func_8009A3D0(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        if (*(u16 *) (p + 0x98) & 0x4000) {
            *(u32 *) (p + 0x1C) |= 0x2000;
        }
        func_8009A21C(e[0x24], e[0x25], (*(u32 *) (p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u16 *) (e + 0x12) = *(u16 *) (p + 0xA6);
        break;
    case 32:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1E8C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 21:
    case 22:
    case 23:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EC1, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 18:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1EF7, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 19:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F1C, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    case 20:
        if (*(u32 *) (p + 0x14) & 0x4000) {
            msg = func_800990FC();
            func_80099290(func_80099194(D_800E1F41, func_80099734(p, msg)));
            func_800A5720(msg);
        }
        break;
    }
    func_80041E70(p);
}

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
