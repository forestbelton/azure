CANDS = [
("func_8005C858", "unk_1C540", """s32 func_8005C858(s32 a, s16 i) {
    return D_80084960[i].unk_0C;
}"""),
("func_8005C8B8", "unk_1C540", """s32 func_8005C8B8(s32 a, s16 i) {
    return D_80084960[i].unk_04;
}"""),
("func_8005C918", "unk_1C540", """s32 func_8005C918(s32 a, s16 i) {
    return D_80084960[i].unk_1C;
}"""),
("func_8005C94C", "unk_1C540", """void func_8005C94C(s32 a, s16 i, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
}"""),
("func_8005845C", "unk_1C540", """void func_8005845C(u8 i, u8 v) {
    D_80084960[i].unk_00 = v;
}"""),
("func_80058494", "unk_1C540", """void func_80058494(u8 i, s32 b, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
    func_80055E7C(1, i, v);
}"""),
("func_8003ADF4", "unk_72A8", """void func_8003ADF4(s16 x, s16 y, s16 w, s16 h, u32 *data) {
    s16 r[4];

    r[0] = x;
    r[1] = y;
    r[2] = w;
    r[3] = h;
    LoadImage(r, data);
}"""),
("func_800383D4", "unk_72A8", """void func_80038A10(void);

void func_800383D4(u8 *p) {
    if ((s8) D_80082660[p[0x2F] * 8 + 1] == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8004AA44", "unk_1C540", """s32 func_8004AA34(s32 *, s32 *);
void func_8004AFC8(s32, s32, s32, void *);

void func_8004AA44(s32 a, s32 b) {
    func_8004AFC8(b, a, 4, func_8004AA34);
}"""),
]
