A = "unk_72A8"
CANDS = [
("func_80037D50", A, """void func_80037D50(u8 *p) {
    u8 *q = p + 0x8C;

    q[0x25] = D_8006A908[p[2]];
    *(u16 *) (q + 0xE) = D_8006A8B4[q[0x2A]] * q[0x25];
}"""),
("func_800389B4", A, """void func_80038A10(void);

void func_800389B4(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8003A468", A, """void func_8003A468(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) += *(u32 *) (t + b * 4 + 0x48);
}"""),
("func_8003A4B4", A, """void func_8003A4B4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) -= *(u32 *) (t + b * 4 + 0x48);
}"""),
("func_8003A500", A, """void func_8003A500(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + 0x84) = *(u32 *) (t + a * 4 + 0x48) -
                          *(u32 *) (t + b * 4 + 0x48);
}"""),
("func_8003AF94", A, """void func_8003AF94(s32 i, u32 v) {
    D_80189394 = v;
    D_801B8EB8 = D_8006CD50[i];
    func_801768AC();
    func_800411FC(0);
}"""),
("func_8003C5A4", A, """s32 func_8003C5A4(s32 k) {
    s8 *p = D_8006B1EC;
    s32 i = 0;

    while (*p != -1) {
        if (*p == k) {
            return i;
        }
        p++;
        i++;
    }
    return -1;
}"""),
("func_80047738", "unk_1AF38", """void func_80047738(u8 *p, s32 b, s32 c) {
    u8 *t = **(u8 ***) (p + 0x28);
    u8 *e = *(u8 **) (t + ((b << 16) >> 14)) + ((c << 16) >> 13);

    *(u8 **) p = e;
    p[4] = c;
    *(u16 *) (p + 0x14) &= 0xFFBF;
    *(u32 *) (p + 8) = *(u32 *) (e + 4);
}"""),
("func_80047C8C", "unk_1AF38", """s32 func_80047C8C(u16 a) {
    s32 i = func_80047B80();

    if (i != -1) {
        func_80047A68(i, a);
    }
    return i;
}"""),
("func_8004827C", "unk_1AF38", """s32 func_80047BC0(s32);

void func_8004827C(u8 *p, s16 k) {
    s32 i = func_80047BC0(k);
    u8 *e = *(u8 **) (p + 0xC);

    *(u8 **) (e + 0x28) = &D_80083D08[i * 8];
}"""),
]
