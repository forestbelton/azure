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
void func_800400B8(void);
void func_8003F6F4(void);
void func_8003E758(void);
void func_8003E2D8(void);
void func_80042B68(s8 *, s8);


void func_80043EB8(void) {
    u8 *p;

    PutDispEnv((DISPENV *)(D_80083160.unk_00 + 0x5C));
    PutDrawEnv((DRAWENV *)D_80083160.unk_00);
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
