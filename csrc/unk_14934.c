#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u8 *, u32 *);
void func_80041588(Unk41284 *, s8 *, s32);
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
    func_80041344((u8 *) x, (u32 *) y);
}

/* Walks a command stream, applying one record at a time: DMA a texture, patch
 * palette bits, copy a payload, or relocate a pointer table, advancing by the
 * record's own byte size until a zero size ends it. Dispatches through
 * jtbl_8002D630, whose nine entries fix the source order of the arms as
 * 1, 5, 2, 6, 7, 8, 3, 4, 9 -- hence the `case 0 ... case 8` numbering below.
 *
 * `op` carries FOUR roles -- switch index, flag word, loop bound, and case 9's
 * pointer -- and that is not a tidiness choice, it is the whole function. Each
 * role is load-bearing for a different reason:
 *
 *  1. `op = *(u16 *) p - 1;` with the arms numbered from 0 makes
 *     expand_end_case skip its own MINUS (minval is 0), so the `sltiu` and
 *     `sll` read the user's pseudo and the index IS a source variable rather
 *     than a compiler temp. Writing `case 1 ... case 9` and subtracting in the
 *     switch instead gives the index a block-local temp with no references of
 *     its own.
 *  2. The flag word and the loop bounds share that variable, which is what
 *     makes it conflict with `e` -- in the op==7 arm the `lhu 0xC` is the arm's
 *     first insn and `e` dies at its fourth. Nothing else in the function can
 *     push `e` off $v1.
 *  3. Case 9's pointer is `op` too, and it is SPLIT INTO TWO STATEMENTS
 *     (`op = (s32) base; op += e->unk_04;`) although one `addu` is emitted
 *     either way. This is the subtle one. Writing it as the single expression
 *     `op = (s32) (base + e->unk_04);` costs 24 words, and the cause is
 *     global.c's `regs_someone_prefers`: `op` is copied into $a3 for
 *     func_8003F80C's fourth argument, so it carries a hard-reg preference for
 *     $a3, and find_reg makes a conflicting HIGHER-priority allocno avoid a
 *     register that a lower-priority conflicting allocno prefers. With one
 *     statement `op` has 52 references over a live length of 56, priority
 *     floor_log2(52)*52/56 = 4.64, just under the shared counter's 39/41 =
 *     4.76 -- so the counter is allocated first, avoids $a3 for `op`'s sake,
 *     takes $t0, and `e` is left with $a3. The extra statement is two more
 *     references for `op` (54/56 = 4.82), which flips the comparison: `op` is
 *     allocated first, its preference is spent, the counter takes $a3 and `e`
 *     takes $t0. A free +2 references is the only lever here that costs no
 *     instruction; the two measured alternatives both cost words -- splitting
 *     the index into `op = *(u16 *) p; op -= 1;` leaves the load in `op`'s own
 *     register (2 words), and folding the tail `size` into `op` takes over the
 *     tail load, which the target keeps block-local in $v0 (3 words).
 *
 * The counters are ONE variable `i` for the op==7 loop, the op==8 outer loop
 * and the op==9 loop, with `j` for the op==8 inner loop. All ten groupings of
 * the four sites were swept against all three spellings of case 9's pointer,
 * 30 variants: this grouping is the only one that matches, and the two that
 * come closest (case 7's counter on its own, 4 words) put it in $a2.
 *
 * Case 9 does NOT advance its pointer -- it rewrites the same slot every
 * iteration. That is in the target, and it is a bug in the original; the
 * sibling func_80041588 gets the same arm right.
 *
 * See docs/matching-levers.md for the shared shapes: the walking pointer in
 * case 8, the chained store, and the separate record pointer `e`. */
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

        op = *(u16 *) p - 1;
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
            op = (s32) base;
            op += e->unk_04;
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

/* The asynchronous sibling of func_80041344 over the same command format: same
 * record type, same nine arms in the same source order (1, 5, 2, 6, 7, 8, 3,
 * 4, 9, from jtbl_8002D658), same `default: return;`. arg2 == 0 takes the
 * deferred path, registering func_8003E140 to set the completion flag.
 *
 * The same four-role `op` as the sibling, and for the same reasons, with two
 * differences that are both in the target:
 *  - case 9 DOES advance its pointer here, and the explicit `| 0x80000000` on
 *    an already-KSEG0 `base` is real: it is the `lui $v0,0x8000` / `or` pair
 *    hoisted into the preheader, and cse will not fold it because the cheapest
 *    equivalent of `base` is the register itself.
 *  - case 9's pointer is its own `u32 *r`, and `op` needs no extra references:
 *    here `op` already outranks the shared counter, so the sibling's
 *    two-statement pointer split is unnecessary (and the tail `size` must stay
 *    a separate variable -- folding it into `op` costs 24 words).
 *
 * Load-bearing and each measured by reverting it: `base` has TWO sets, the
 * masked value then the KSEG0 pointer copied back (folding them costs 3 words,
 * because the target's `and $s1,$v0,$v1` puts the masked value in a
 * callee-saved register, which only happens if it is `base`'s own pseudo);
 * `dst = D_80081480 = D_8008148C;` is a chained store (two statements cost 132
 * words, the store_expr mechanism commented on func_80041284 above); case 8
 * walks a SINGLE pointer `w`, which reproduces the arm's 19 words including the
 * `addu $a1,$a0,$zero` / `addiu $a1,$a1,2` pair that loop.c creates in the
 * preheader after cse; and func_8003F80C's fourth argument must be s16, so
 * case 2 gets a bare `lh` and cases 6/7/8 get `sll 16`/`sra 16`. */
void func_80041588(Unk41284 *arg0, s8 *arg1, s32 arg2) {
    u8 *p;
    u8 *base;
    u32 dst;
    u16 size;
    s32 op;
    s32 i;
    s32 j;
    u16 *q;
    u16 *w;

    if (arg2 == 0) {
        func_8003E4FC(6, (u8 *) arg0, 0);
        *arg1 = 0;
        func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) arg1);
        return;
    }

    base = (u8 *) (arg0->unk_00 & 0x7FFFFF);
    p = (u8 *) ((u32) base | 0x80000000);
    base = p;
    DrawSync(0);
    dst = D_80081480 = D_8008148C;

    for (;;) {
        Unk41344 *e = (Unk41344 *) p;

        op = *(u16 *) p - 1;
        switch (op) {
        case 0:
            func_8004068C(base + e->unk_04, (u8 *) dst);
            LoadImage(p + 8, (u32 *) dst);
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
        case 6: {
            q = (u16 *) (base + e->unk_04);
            op = *(u16 *) (p + 0xA) << 4;
            for (i = 1; i < op; i++) {
                q[i] |= 0x8000;
            }
            op = *(u16 *) (p + 0xC);
            op |= 2;
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) op);
            DrawSync(0);
            break;
        }
        case 7: {
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
        }
        case 2:
            func_8004068C(base + e->unk_04, (u8 *) e->unk_08);
            break;
        case 3:
            memcpy((void *) e->unk_08, base + e->unk_04, e->unk_0C);
            break;
        case 8: {
            u32 *r = (u32 *) (base + e->unk_04);

            for (i = *(u16 *) (p + 8); i > 0; i--) {
                r[1] = r[1] + ((u32) base | 0x80000000);
                r += 2;
            }
            break;
        }
        default:
            return;
        }
        size = *(u16 *) (p + 2);
        if (size == 0) {
            break;
        }
        p += size;
    }
}

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
