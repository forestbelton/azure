B = "unk_1C540"
CANDS = [
("func_80049944", B, """s32 func_80049944(u32 a) {
    s32 r = 3;

    if (a & 1) {
        r = 0;
    } else if (a & 2) {
        r = 1;
    } else if (a & 4) {
        r = 2;
    }
    return r;
}"""),
("func_8004A6C0", B, """s32 func_8004A6C0(s16 a, s16 b) {
    u8 *p = (u8 *) 0x80010000;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (*(s16 *) (p + 0x3564) == a && *(s16 *) (p + 0x3566) == b) {
            break;
        }
        p += 4;
    }
    return i;
}"""),
("func_8004A8D8", B, """s32 func_8004A6C0(s16, s16);

void func_8004A8D8(s16 a, s16 b) {
    s32 i = func_8004A6C0(a, b);
    u8 *p;

    if (i < 0x14) {
        p = (u8 *) 0x80010000 + i * 4;
        *(s16 *) (p + 0x3564) = 0;
        *(s16 *) (p + 0x3566) = 0;
    }
}"""),
("func_8004AFE8", B, """s32 func_8004AFE8(u8 **p) {
    u8 k = (*p)[1];
    s32 n = 0;

    while (p != 0) {
        if ((*p)[1] != k) {
            break;
        }
        p++;
        n++;
    }
    return n;
}"""),
("func_8004B324", B, """void func_8004B298(s32, s32);
void func_8004B2E0(s32, s32);

void func_8004B324(s32 a, s32 b) {
    func_8004B298(a, b);
    func_8004B2E0(a, b);
}"""),
("func_8004D7A8", B, """void func_8004D75C(u8 *, u8 *);

void func_8004D7A8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xF8, p + 0xB4);
    } else {
        func_8004D75C(p + 0x180, p + 0x13C);
    }
}"""),
("func_8004D7E8", B, """void func_8004D7E8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xB4, p + 0xF8);
    } else {
        func_8004D75C(p + 0x13C, p + 0x180);
    }
}"""),
("func_8004EEBC", B, """void func_8004EEBC(u8 *p) {
    u32 *q = *(u32 **) (p + 4);

    if (q != 0) {
        if (q[0] != 0) {
            *(u32 *) (p + 0x28C) = 0;
            *(u32 *) (p + 0x29C) = (u32) (p + 0xF8);
        } else {
            *(u32 *) (p + 0x28C) = (u32) (p + 0x80);
            *(u32 *) (p + 0x29C) = 0;
        }
    }
}"""),
("func_8004F3D8", B, """void func_8004EEFC(u8 *, u8 *, u8 *);
void func_8004F2BC(u8 *);

void func_8004F3D8(u8 *p, u32 v) {
    func_8004F3D0((u32 *) p, v);
    func_8004EEFC(p + 0x210, p + 0x1E8, p + 0x1F8);
    func_8004F2BC(p);
}"""),
("func_800541E8", B, """void func_8005E7E0(s32);

void func_800541E8(void) {
    func_8005B300();
    func_8005B418(0);
    func_8005B348();
    func_8005E7E0(4);
    func_8005B320();
}"""),
("func_8005A2FC", B, """void func_8005D838(s32, u8 *);

void func_8005A2FC(void) {
    func_8005CA70();
    func_80056C30();
    func_8005A26C();
    func_8005D838(0x10, D_80086CC0);
}"""),
("func_8005A33C", B, """void func_800585A0(void);

void func_8005A33C(void) {
    if ((s32) D_80086D4C[0] > 0 && (s32) D_80086D4C[0] < 4) {
        func_800585A0();
    }
    D_80073824[0] = 1;
}"""),
("func_8005A5C8", B, """void func_8005EDA0(s16 *);

void func_8005A5C8(s16 x, s16 y) {
    s16 t[20];

    *(s32 *) &t[0] = 0xF;
    t[2] = x << 7;
    t[3] = y << 7;
    t[4] = 0;
    t[5] = 0;
    func_8005EDA0(t);
}"""),
("func_8005B3D8", B, """void func_8005DA88(s16 *);

void func_8005B3D8(s16 x, s16 y) {
    s16 t[12];

    *(s32 *) &t[0] = 6;
    t[4] = x << 8;
    t[5] = y << 8;
    func_8005DA88(t);
}"""),
("func_8005C818", B, """void func_8005C818(s32 a, u32 v) {
    u32 n = D_800869B4;
    u8 *p = D_80085FA8;
    u32 i;

    for (i = 0; i < n; i++) {
        *(u32 *) (p + 0x24) = v;
        *(u32 *) (p + 0x20) = v;
        p += 0x50;
    }
}"""),
]
