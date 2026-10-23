B = "unk_1C540"
CANDS = [
("func_80049D80", B, """void func_80049CF4(s32, s32);
void func_800499E8(u8 *, s32, u8 *);

void func_80049D80(u8 *p, s32 b) {
    func_80049CF4((s32) p, b);
    func_800499E8(p + 0x18, b, D_80080B54);
}"""),
("func_8005097C", B, """void func_800508F0(u8 *, u8 *);

void func_8005097C(u8 *p) {
    func_800508F0(p, D_80080B88);
    func_800499E8(p + 0x18, (s32) D_80080B88, D_80080B90);
}"""),
]
