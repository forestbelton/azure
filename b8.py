A = "unk_72A8"
B = "unk_1C540"
CANDS = [
("func_80035ABC", A, """void func_80035ABC(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v -= 0x10;
    if ((s16) *v < 2) {
        *v = 2;
        *(u32 *) (p + 0x68) = (u32) func_80035AF8;
    }
}"""),
("func_80035C44", A, """void func_80035C80(void);

void func_80035C44(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v += 0x10;
    if ((s16) *v > 0x40) {
        *v = 0x40;
        *(u32 *) (p + 0x68) = (u32) func_80035C80;
    }
}"""),
("func_800382F0", A, """void func_8003832C(u8 *);

void func_800382F0(u8 *p) {
    if (func_80053EF0(4) == 0) {
        func_8003832C(p);
    }
}"""),
("func_8003832C", A, """void func_80039AF0(u8 *);
void func_80038368(u8 *);

void func_8003832C(u8 *p) {
    func_80039AF0(p);
    *(u16 *) (p + 0x18) = 5;
    *(u32 *) (p + 0x10) = (u32) func_80038368;
}"""),
("func_80039B28", A, """void func_80039B28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] | (c[1] << 8);

    *q = c + 2;
    func_80053DA8(v);
}"""),
("func_8003B9E8", A, """s32 func_800B2834(void);
void func_8003BAF8(u32);

void func_8003B9E8(void) {
    func_8003BAF8(D_8006AFDC[func_800B2834()]);
}"""),
("func_8003BA24", A, """void func_8003BA24(void) {
    func_8003BAF8(D_8006AFE8[func_800B2834()]);
}"""),
("func_8003BA60", A, """void func_8003BA60(void) {
    func_8003BAF8(D_8006AFF4[func_800B2834()]);
}"""),
("func_8003BA9C", A, """void func_8003BA9C(void) {
    func_8003BAF8(D_8006B000[func_800B2834()]);
}"""),
("func_8003C8E4", A, """s32 func_8003C5A4(void);

u32 *func_8003C8E4(void) {
    s32 i = func_8003C5A4();

    if (i < 0) {
        return 0;
    } else {
        return &((u32 *) 0x800135C4)[i];
    }
}"""),
("func_80047A68", "unk_1AF38", """u8 *func_80047A44(s32);
void func_800479D4(u8 *, u8 *, s32);

void func_80047A68(s32 a, u16 b) {
    func_800479D4(func_80047A44(a), D_80080A90, b);
}"""),
("func_80049F2C", B, """void func_80049F2C(u8 *p) {
    *(u16 *) (p + 0x28) += (*(s16 *) (p + 0x4) - *(s16 *) (p + 0x28)) >> 1;
    *(u16 *) (p + 0x2A) += (*(s16 *) (p + 0x6) - *(s16 *) (p + 0x2A)) >> 1;
}"""),
]
