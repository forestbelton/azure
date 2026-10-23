CANDS = [
("func_8004AC18", "unk_1C540", """u32 func_8004AC18(s32 i) {
    return D_8007359C[0][i].unk_04;
}"""),
("func_8004B4DC", "unk_1C540", """s32 func_8004B4DC(u8 *p) {
    s32 n = 0;
    u8 v;

    /* The advance is inside the loop body and BEFORE the test, which is how it
       ends up in the backward branch's delay slot and so runs on the exiting
       iteration too. Latching the byte in `v` is what lets it. */
    do {
        n++;
        v = *p;
        p += 0xC;
    } while ((v & 0x80) == 0);
    return n;
}"""),
("func_80053DA8", "unk_1C540", """s32 func_80055778(s32);

s16 func_80053DA8(u16 a) {
    return func_80055778(a);
}"""),
("func_80053DCC", "unk_1C540", """void func_80054D64(s32);

void func_80053DCC(s32 a) {
    D_8008480C[0] = a;
    func_80054D64(a);
}"""),
("func_80053DF0", "unk_1C540", """void func_800552C8(s32);

void func_80053DF0(s32 a) {
    D_8008480A[0] = a;
    func_800552C8(a);
}"""),
]
