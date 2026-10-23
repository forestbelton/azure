A = "unk_72A8"
B = "unk_1C540"
CANDS = [
("func_8003571C", A, """void func_80035760(void);

void func_8003571C(u8 *p) {
    *(u16 *) (*(u8 **) p + 8) -= 0xA0;
    if (*(s16 *) (*(u8 **) p + 8) < 0) {
        *(u16 *) (*(u8 **) p + 8) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80035760;
    }
}"""),
("func_80036110", A, """void func_80036110(u8 *p) {
    *(u16 *) (*(u8 **) p + 0xA) -= 0x140;
    if (*(s16 *) (*(u8 **) p + 0xA) < 0) {
        *(u16 *) (*(u8 **) p + 0xA) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}"""),
("func_80039214", A, """void func_80039214(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AA8((s16) v);
}"""),
("func_80039258", A, """void func_80039258(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AE8((s16) v);
}"""),
("func_80039A88", A, """void func_80039A88(u8 *p) {
    if (func_80053EF0(4) != 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}"""),
("func_8003A01C", A, """void func_8003A01C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *(u32 *) (t + a * 4 + 0x48) = *tab[b];
}"""),
("func_8003A29C", A, """void func_8003A29C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *tab[b] = *(u32 *) (t + a * 4 + 0x48);
}"""),
("func_8003B42C", A, """void func_8003B42C(s32 a) {
    if (a != 0) {
        func_80033AA8(0x3E8);
        func_80033AE8(0xA);
    } else {
        func_80033AE8(0x3E8);
        func_80033AA8(0xA);
    }
}"""),
("func_8003C5F0", A, """u32 func_8003C5F0(void) {
    s32 i = func_8003C5A4();

    if (i < 0) {
        return 0;
    } else {
        return D_8006B1A8[i];
    }
}"""),
("func_800481E0", "unk_1AF38", """void func_800481E0(void) {
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if (*(u32 *) p != 0) {
            *(u16 *) (p + 4) |= 1;
        }
        p += 8;
    }
}"""),
("func_80049330", B, """u32 func_800492B0(u32);

u32 func_80049330(u8 *p) {
    if (p[0] != 0) {
        return func_800492B0(p[3] & 0x1F);
    } else {
        return D_800814A8;
    }
}"""),
("func_8004B2E0", B, """void func_8004B2E0(s32 a, s32 b) {
    s32 *n = (s32 *) b;

    *n -= 1;
    if (*n == 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
]
