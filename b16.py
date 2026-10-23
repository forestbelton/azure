B = "unk_1C540"
CANDS = [
("func_8004CAE8", B, """u8 *func_8004CAE8(u8 *p, u16 k) {
    for (;;) {
        if ((p[0] & 0x20) != 0) {
            if (*(u16 *) (p + 6) == k) {
                return p;
            }
        }
        if ((p[0] & 0x80) != 0) {
            return 0;
        }
        p += 0xC;
    }
}"""),
("func_8004D5D0", B, """void func_8004D4AC(void);

void func_8004D5D0(void) {
    u8 *d = (u8 *) &D_80083178;
    u8 *s = D_80083780;

    *(u16 *) (d + 0xA4) = *(u16 *) (s + 0x2);
    *(u16 *) (d + 0xA6) = *(u16 *) (s + 0x6);
    *(u16 *) (d + 0xA8) = *(u16 *) (s + 0xA);
    func_8004D4AC();
}"""),
("func_8004D9E0", B, """void func_8004D9E0(u8 *p, s32 b, s32 c, s32 d, s32 e) {
    p[2] = b * d / 2 - 0x80 + b * 4;
    p[3] = c * (e + 0x10) - 0x80;
}"""),
("func_8004EBDC", B, """s32 func_8004EBDC(void) {
    s32 r = 0;

    if (func_80053EF0(1) == 0) {
        r = func_80053EF0(4) == 0;
    }
    return r;
}"""),
("func_8004F5B0", B, """void func_8004F558(u8 *);

void func_8004F5B0(u8 *p) {
    if ((D_80083170[0] & 0x20) != 0) {
        func_80053DA8(0x515);
        func_8004F558(p);
    }
}"""),
("func_80053D64", B, """s32 func_80053D64(void) {
    u8 loc[8];

    if (CdSync(1, 0) == 2) {
        CdControl(0x10, 0, loc);
    }
    return CdPosToInt(loc);
}"""),
("func_800543C8", B, """void func_80055CF4(void);
void func_800544A4(void);
void func_8005440C(void);

void func_800543C8(void) {
    if ((D_800847D4 & 0xFF000000) != 0) {
        func_80055CF4();
    }
    func_800544A4();
    func_8005440C();
}"""),
("func_80055778", B, """s32 func_80055778(s16 a) {
    if (D_80084778[0] < 0x20) {
        D_80084778[0] += 1;
        D_80084778[D_80084778[0]] = a;
        return 0;
    } else {
        return -1;
    }
}"""),
("func_80057A50", B, """u8 *func_80057A50(u32 *p, u16 b) {
    return D_80086A40[D_80086D50].unk_04 + ((*p * 0x10 + b) * 0x20 + 0x820);
}"""),
("func_800587B4", B, """void func_800599B0(void);

void func_800587B4(void) {
    D_80085FA4[0] = 0x80080000;
    D_800869B0[0] = 0xC000;
    func_80056C30();
    func_800599B0();
    D_800737A4[0] = 1;
}"""),
("func_80058B2C", B, """u32 func_800589B8(s32);

u16 func_80058B2C(s32 a) {
    u8 x = func_800589B8(a);
    u8 y = func_800589B8(a);

    return func_80058AA8(x, y);
}"""),
("func_80059D1C", B, """void func_80059D1C(void) {
    u32 n = D_800869B4;
    u32 i;

    for (i = 0; i < n; i++) {
        D_80085FA8s[i].unk_2C = 1;
        D_80085FA8s[i].unk_00 = 0;
    }
}"""),
]
