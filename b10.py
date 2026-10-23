A = "unk_72A8"
CANDS = [
("func_80033AA8", A, """void func_80033AA8(u16 id) {
    if (id != 0 && id != 1) {
        ((u32 *) 0x80012D70)[id >> 5] |= 1 << (id & 0x1F);
    }
}"""),
("func_80036988", A, """void func_80036988(u8 *d, u8 *s) {
    *(u16 *) (d + 0x8) = *(u16 *) (s + 0x0) + *(u16 *) (s + 0x8);
    *(u16 *) (d + 0xA) = *(u16 *) (s + 0x2) + *(u16 *) (s + 0xA);
    *(u16 *) (d + 0xC) = *(u16 *) (s + 0xC);
    *(u16 *) (d + 0xE) = *(u16 *) (s + 0xE);
}"""),
("func_800374F4", A, """u32 func_800374F4(u16 n) {
    return (n * (u16) (rand() << 1)) >> 16;
}"""),
("func_80038394", A, """void func_80038A10(void);

void func_80038394(u8 *p) {
    if (func_80033B2C(*(s16 *) (p + 0x30)) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_800384F8", A, """void func_80038538(u8 *);

void func_800384F8(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80038538(p);
    }
}"""),
("func_80039760", A, """void func_80039760(u8 *p) {
    u8 *c = p[0x27] * 4 + *(u8 **) (p + 0x1C);

    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}"""),
("func_80039F80", A, """void func_80039F80(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}"""),
("func_8003A248", A, """void func_8003A248(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + b * 4 + 0x48) = *(u32 *) (t + a * 4 + 0x48);
}"""),
("func_8003A7BC", A, """void func_8003A7BC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}"""),
("func_8003AA84", A, """void func_8003AA84(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 **s = (u32 **) (p + 0x7C);
    u32 i = *(*q)++;

    **s = (u32) *q;
    *s = *s + 1;
    *q = *(u8 **) (t + i * 4 + 0x48);
}"""),
("func_8003ADB4", A, """s16 func_8003ADB4(u16 a) {
    if (D_80082E6A == 2) {
        return 0;
    } else {
        return func_80053DA8(a);
    }
}"""),
("func_8003C2B8", A, """void func_8003C0C0(u8 *, u32);

void func_8003C2B8(void) {
    u8 *p = (u8 *) D_80082D58;
    u32 a = *(u32 *) (p - 0x14);

    D_80082D58[0] = 0;
    func_8003C0C0(p, a);
    func_80033D08(p);
}"""),
("func_80047B80", "unk_1AF38", """s32 func_80047B80(void) {
    u8 *p = D_80083D08;
    s32 r = -1;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (*(u32 *) p == 0) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}"""),
("func_80047BC0", "unk_1AF38", """s32 func_80047BC0(s16 k) {
    u8 *p = D_80083D08;
    s32 r = -1;
    s32 i;

    for (i = 0; i < 6; i++) {
        if (*(s16 *) (p + 6) == k) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}"""),
("func_80048D60", "unk_1C540", """void func_8004B568(u8 *);

void func_80048D60(u8 *p) {
    u16 f = *(u16 *) (p - 2);

    if ((f & 0x2000) != 0) {
        *(u16 *) (p - 2) = f & 0xDFFF;
        *(u32 *) (p - 0x10) = (u32) D_800212DC;
        func_8004B568(p);
    }
}"""),
]
