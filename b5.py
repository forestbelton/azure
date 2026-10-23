U = "unk_72A8"
CANDS = [
("func_80033BE4", U, """u8 *func_80033BE4(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (p[0] == 0) {
            return p;
        }
        p += 0x64;
    }
    return 0;
}"""),
("func_80036484", U, """void func_80036484(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        p[0x4D] = 0xFF;
        p[0x51] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800364BC;
    }
}"""),
("func_80036C44", U, """s32 func_80037534(s32, s32, s32);
void func_800478B8(s32);

void func_80036C44(s32 a, s32 b, s32 c) {
    if (func_80037534(a, b, c) == 0) {
        func_800478B8(c);
    }
}"""),
("func_80038538", U, """void func_80038A10(void);

void func_80038538(u8 *p) {
    func_80053DA8(0xB4);
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}"""),
("func_80039578", U, """void func_80038128(u8 *);

void func_80039578(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    *(u32 *) (p + 0x10) = (u32) func_80038128;
    *(u16 *) (p + 0x18) = v;
}"""),
("func_800395B0", U, """void func_80038394(u8 *);

void func_800395B0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    *(u32 *) (p + 0x10) = (u32) func_80038394;
    *(u16 *) (p + 0x30) = v;
}"""),
("func_8003965C", U, """void func_8003965C(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);

    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}"""),
("func_80039884", U, """s32 func_80039884(u8 *p) {
    return *(s8 *) (p + 0x87) != 0 || (D_80083160.unk_08 & 0x60) == 0;
}"""),
("func_80039AF0", U, """void func_80039AF0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 **r = (u8 **) (p + 0x80);
    u8 *t;

    (*r)[1] = *(*q)++;
    t = *r;
    t[0x3C] = D_8007216C[t[1]];
}"""),
("func_80039C0C", U, """void func_8003875C(u8 *);

void func_80039C0C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x32] = *(*q)++;
    p[0x33] = *(*q)++;
    *(u32 *) (p + 0x10) = (u32) func_8003875C;
}"""),
("func_8003AE28", U, """void func_8003AE28(s16 x, s16 y, s16 w, s16 h, s32 a, s32 b) {
    s16 r[4];

    r[0] = x;
    r[1] = y;
    r[2] = w;
    r[3] = h;
    MoveImage(r, a, b);
}"""),
("func_8003B034", U, """u32 func_80048D20(void);
void func_800483AC(s32);

void func_8003B034(void) {
    D_80080A80 = func_80048D20();
    func_800483AC(1);
    func_800483AC(0);
}"""),
("func_8003B324", U, """void func_8003AE28(s16, s16, s16, s16, s32, s32);

void func_8003B324(void) {
    func_8003AE28(0x90, 0x1F2, 0x10, 1, 0, 0x1F2);
}"""),
("func_8003C06C", U, """u32 func_8003C06C(s32 i) {
    u32 *t;

    if (D_80082E6A == 2) {
        t = D_8006B008;
    } else {
        t = D_800D0858;
    }
    return t[i];
}"""),
]
