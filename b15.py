CANDS = [
("func_80036110", "unk_72A8", """void func_80036154(void);

void func_80036110(u8 *p) {
    *(u16 *) (*(u8 **) p + 0xA) -= 0x140;
    if (*(s16 *) (*(u8 **) p + 0xA) < 0) {
        *(u16 *) (*(u8 **) p + 0xA) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}"""),
("func_8004B2E0", "unk_1C540", """void func_8004B2E0(s32 a, s32 b) {
    s32 *n = (s32 *) b;

    /* `!= 0`: the flag goes in while the count is still nonzero. */
    *n -= 1;
    if (*n != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}"""),
]
