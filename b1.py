CANDS = [
("func_800399EC", "unk_72A8", """void func_800399EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    t[0x3D] = *(*q)++;
}"""),
("func_8003AAF8", "unk_72A8", """void func_8003AAF8(void) {
    *(s32 *) &D_80082A38[36] = D_80082A38[86];
    *(s32 *) &D_80082A38[38] = D_80082A38[87];
}"""),
("func_8003B210", "unk_72A8", """void func_8003B210(void) {
    func_8003B034();
    func_8003B06C();
}"""),
("func_80039B94", "unk_72A8", """void func_800383D4(u8 *);

void func_80039B94(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_800383D4;
    p[0x2F] = b;
}"""),
("func_80039BBC", "unk_72A8", """void func_80038690(u8 *);

void func_80039BBC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038690;
    p[0x32] = b;
}"""),
("func_80039BE4", "unk_72A8", """void func_80038708(u8 *);

void func_80039BE4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038708;
    p[0x32] = b;
}"""),
("func_8003B400", "unk_72A8", """void func_8003AF58(u8 *, u8 *);

void func_8003B400(void) {
    func_8003AF58(D_800717D0, D_8006AEAC);
}"""),
("func_8003B264", "unk_72A8", """void func_800C17A0(s32, s32);

void func_8003B264(void) {
    func_8003B034();
    func_8003B06C();
    func_800C17A0(0x2B, 0x200);
}"""),
("func_8003B2C4", "unk_72A8", """void func_800C178C(s32, s32);

void func_8003B2C4(void) {
    func_800C178C(0xD, -1);
    func_8003B034();
    func_8003B06C();
}"""),
("func_8003ADF4", "unk_72A8", """void func_8003ADF4(s16 x, s16 y, s16 w, s16 h, u32 *data) {
    RECT r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    LoadImage(&r, data);
}"""),
("func_80039EF4", "unk_72A8", """void func_80039EF4(u8 *p) {
    p[0x88] -= 1;
    if ((s8) p[0x88] >= 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}"""),
("func_80039F4C", "unk_72A8", """void func_80039F4C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 v = *(*q)++;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        *(u32 *) (t + i * 4 + 0x48) = v;
    }
}"""),
("func_800383D4", "unk_72A8", """void func_800383D4(u8 *p) {
    if ((s8) D_80082660[p[0x2F] * 8 + 1] == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8004AA44", "unk_1C540", """void func_8004AFC8(s32, s32, s32, void *);

void func_8004AA44(s32 a, s32 b) {
    func_8004AFC8(b, a, 4, func_8004AA34);
}"""),
("func_8004B500", "unk_1C540", """void func_8004B500(u8 *p) {
    if (p != 0) {
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
("func_8005845C", "unk_1C540", """void func_8005845C(u8 i, u8 v) {
    *(u32 *) &D_80084960[i * 0x9C] = v;
}"""),
("func_8005C858", "unk_1C540", """s32 func_8005C858(s32 a, s16 i) {
    return *(s32 *) &D_80084960[i * 0x9C + 0xC];
}"""),
("func_8005C8B8", "unk_1C540", """s32 func_8005C8B8(s32 a, s16 i) {
    return *(s32 *) &D_80084960[i * 0x9C + 4];
}"""),
("func_8005C918", "unk_1C540", """s32 func_8005C918(s32 a, s16 i) {
    return *(s32 *) &D_80084960[i * 0x9C + 0x1C];
}"""),
("func_8005C94C", "unk_1C540", """void func_8005C94C(s32 a, s16 i, u8 v) {
    *(u32 *) &D_80084960[i * 0x9C + 0x1C] = v & 0x7F;
}"""),
]
