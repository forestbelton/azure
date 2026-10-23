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
