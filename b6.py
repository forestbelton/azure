CANDS = [
("func_8003C128", "unk_72A8", """UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_8003C3B4(void);

void func_8003C128(void) {
    UnkNode *n = func_8003FE78(0x15, &D_80082D38, 0x49);

    n->unk_10 = (u32) func_8003C3B4;
    n->unk_20 = 0;
}"""),
("func_8003C450", "unk_72A8", """void func_8003C488(u32 *, s32, u8 *);

void func_8003C450(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1E);

    *v += 0x400;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C488;
    }
}"""),
("func_8003C488", "unk_72A8", """void func_8003C4C0(void);

void func_8003C488(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1C);

    *v += 0x200;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C4C0;
    }
}"""),
("func_80047DB8", "unk_1AF38", """void func_800479D4(u8 *, u8 *, s32);

u8 *func_80047DB8(u16 a) {
    func_800479D4(D_80016000, D_80023000, a);
    return D_8008152C;
}"""),
("func_80048374", "unk_1AF38", """void func_80047784(u8 *, s32, s32);

void func_80048374(u8 *p, s16 b, s16 c) {
    *(u16 *) (p + 0x14) |= 0x400;
    func_80047784(p, b, c);
}"""),
("func_80049984", "unk_1C540", """s32 func_80049944(u32 *);

s32 func_80049984(u8 *p) {
    s32 r = 3;

    if (p[0x13] != 0) {
        r = func_80049944(*(u32 **) (p + 0x14));
    }
    return r;
}"""),
("func_8004B1A4", "unk_1C540", """void func_8004B1A4(u8 *p) {
    if (p != 0) {
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
("func_8004B530", "unk_1C540", """void func_8004B530(u8 *p) {
    if (p != 0) {
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
("func_8004D064", "unk_1C540", """u8 *func_8004D064(u8 *p) {
    switch (p[1] & 0xF0) {
    case 0x20:
        return p + 0xC;
    case 0x30:
        return p + 0x18;
    }
    return p + 0xC;
}"""),
("func_80053C6C", "unk_1C540", """void func_80053C6C(void) {
    u32 *b = (u32 *) D_8006ADBC;
    u32 v = b[1];

    *(u32 *) D_8006ADBC = v;
    if (v != 0) {
        func_80053C3C((u8 *) v);
    }
}"""),
("func_8005AC30", "unk_1C540", """s32 func_8005ECF8(s32);

s16 func_8005AC30(s16 a) {
    return func_8005ECF8(a != 0);
}"""),
("func_8005B2C8", "unk_1C540", """void func_8005B2C8(s16 i, s32 b, u16 *x, u16 *y) {
    *x = D_80086C00[i].unk_08;
    *y = D_80086C00[i].unk_0A;
}"""),
]
