B = "unk_1C540"
REV = """    /* The PARAMETER is the moving pointer and the saved copy is returned. */
    u8 *r = p;
    u8 *b = r + (n - 1);
    u8 x;
    u8 y;

    while (p < b) {
        x = *p;
        y = *b;
        *b-- = x;
        *p++ = y;
    }
    return r;
}"""
CANDS = [
("func_80049D80", B, """void func_80049CF4(s32, s32);
void func_800499E8(u8 *, s32, u8 *);

void func_80049D80(u8 *p, s32 b) {
    func_80049CF4((s32) p, b);
    func_800499E8(p + 0x18, b, D_80080B54);
}"""),
("func_8004B298", B, """void func_8004B298(s32 a, s32 b) {
    u32 *q = *(u32 **) (b + 4);

    if (q[0] == 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
("func_8004E21C", B, """typedef struct Unk4E21C {
    u8 unk_00[4];
} Unk4E21C;

void func_8004E188(u8 *, u8);

u8 *func_8004E21C(u8 *p, u8 b) {
    u8 t[16];

    func_8004E188(t, b);
    /* A byte-aligned 4-byte struct assignment: that is the lwl/lwr and
       swl/swr pair rather than a word move. */
    *(Unk4E21C *) p = *(Unk4E21C *) (t + 8);
    return p;
}"""),
("func_8004E478", B, """u8 *func_8004E478(u8 *p, u16 n) {\n""" + REV),
("func_8004E5A0", B, """u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5A0(s32 a, u16 n, u8 *dst) {
    func_8004E57C(a, n, (s32) dst);
    func_8004E478(dst, n);
    return dst;
}"""),
("func_8005097C", B, """void func_800508F0(u8 *, u8 *);

void func_8005097C(u8 *p) {
    func_800508F0(p, D_80080B88);
    func_800499E8(p + 0x18, (s32) D_80080B88, D_80080B90);
}"""),
("func_80033B2C", "unk_72A8", """s32 func_80033B2C(s32 id) {
    u16 i = id;
    u8 *p;

    if (i == 0) {
        return 0;
    }
    if (i == 1) {
        return 1;
    }
    p = (u8 *) 0x80010000;
    p += (i >> 5) * 4;
    return (*(u32 *) (p + 0x2D70) & (1 << (id & 0x1F))) != 0;
}"""),
("func_80033C8C", "unk_72A8", """void func_80033C8C(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        func_80033C84((u32 *) p);
        p += 0x64;
    }
}"""),
]
