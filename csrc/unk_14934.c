#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u16);
void func_80047468(Unk800777AC *, u32, u16);
void func_800463EC(u8 *, UnkVec *, s16 *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_8003F6D4(s32, s32, s32 *, s32);
void func_80040B88(void);
void func_80042984(u8 *);
void func_80041AB0(void);
void func_80041AE4(void);
void func_80043D04(void);

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
        func_80041284(*q);
        q++;
    } while (*q != NULL);
}

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041284);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80041900);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043568);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80043674);

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

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044BB0);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044C54);

INCLUDE_ASM("asm/nonmatchings/unk_14934", func_80044D24);

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

s32 func_80046398(u8 *p, UnkVec *v, s16 *arg2) {
    UnkNode *n;

    for (;;) {
        func_800463EC(p, v, arg2, arg2[3]);
        n = *(UnkNode **) (p - 8);
        if (n == NULL) {
            return 0;
        }
        p = (u8 *) n + 0x20;
        v = n->unk_08;
        arg2 = (s16 *) n->unk_0C;
    }
}

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
