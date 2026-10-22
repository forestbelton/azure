#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u16);
void func_80047468(Unk800777AC *, u32, u16);
void func_80042984(u8 *);
void func_80041AE4(void);
void func_80043D04(void);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041134);

void func_800411AC(void) {
    void (**p)(void) = D_80083C68;

    while (*p != NULL) {
        (*p++)();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800411FC);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041284);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041344);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041588);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041818);

void func_800418B4(void) {
    if (D_80080A89 != 0) {
        return;
    }
    func_8003E4FC(6, &D_80080C50, 0);
    func_8003F320();
    D_80080A89 = 1;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041900);

void func_800419EC(u16 arg0, u32 arg1) {
    D_80083CA8.unk_08 = arg0;
    D_80083CA8.unk_06 = arg1;
    D_80083CA8.unk_00 = arg1 / arg0;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041A10);

void func_80041AB0(void) {
    D_80081480 = D_8008148C;
    func_80040A88(func_80041AE4);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041AE4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041B98);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041BE4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041C64);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041CBC);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800422DC);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800423C0);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042640);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042710);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80042900);

void func_80042984(u8 *arg0) {
    u8 t = arg0[0x13];

    if (t >= 3 && t < 0x2E) {
        u8 *p = *(u8 **) (arg0 - 0x14);
        u16 v = *(u16 *) (p + 0x12) & 0xFFC0;

        *(u16 *) (p + 0x12) = v + (func_800429E4(arg0) << 2);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800429E4);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043458);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800434E4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043568);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043674);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043704);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800437A4);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800439F8);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043A68);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043B4C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043C30);

void func_80043CD0(void) {
    D_80081480 = D_8008148C;
    func_80040A88(func_80043D04);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043D04);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043DB8);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043E04);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043E60);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043EB8);

s32 func_80043FB8(s32 arg0, ...) {
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043FD0);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044144);

void func_80044234(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    func_8003F540(arg0, arg1, arg2, arg3);
    func_80053DA8(0x300);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004425C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004437C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004450C);

u32 func_800445E0(void) {
    Unk8006CE34 *p = func_8003F534();
    u32 t = (p->unk_08 & 0xFFFFFF) - 0x20;

    return t + p->unk_04;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044618);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044698);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044724);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800448BC);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004491C);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044AAC);

void func_80044B48(void) {
    u32 *p = *(u32 **) (D_80083160.unk_00 + 0x8D0);

    func_80067F20(p, 0, 0, 0, 0);
    AddPrim(D_80083160.unk_00 + 0x8AC, p);
    *(u32 **) (D_80083160.unk_00 + 0x8D0) = p + 3;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044BB0);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044C54);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044D24);

s32 func_80045310(u32 arg0) {
    /* D_80081478 must be declared with an incomplete array type: a complete
     * one of 8 bytes is small data, which makes the base an `la` constant and
     * folds the +4 into it, where the target bumps a register. */
    u32 *p = D_80081478;

    if (D_80083160.unk_00 != (u8 *) &D_801C9E40) {
        p++;
    }
    return *p < arg0;
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80045340);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800453E0);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80045C34);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80045CC4);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046398);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_800463EC);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046884);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046A5C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046AFC);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046C20);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046D64);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046E38);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80046F88);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047054);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_8004713C);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047200);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80047270);

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
