CANDS = [
("func_800383D4", "unk_72A8", """void func_80038A10(void);

void func_800383D4(u8 *p) {
    if (D_80082660[p[0x2F]].unk_01 == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}"""),
("func_8003AAF8", "unk_72A8", """void func_8003AAF8(void) {
    /* A local pointer, not `D_80082A38[36]` directly: with constant indices on
       the array name the base comes out biased to the first one
       (`addiu %lo(D_80082A38)+0x48`) and a second addiu undoes it, 8 words
       against 7. Through a pointer the symbol is materialised once and all
       four accesses are displacements, as the target has them. */
    s16 *b = D_80082A38;

    *(s32 *) &b[36] = b[86];
    *(s32 *) &b[38] = b[87];
}"""),
("func_8004B500", "unk_1C540", """void func_8004B500(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0;
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0 = g | 0x8000;
    }
}"""),
("func_8004EE90", "unk_1C540", """void func_8004EE90(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}"""),
("func_8004F52C", "unk_1C540", """void func_8004F52C(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}"""),
]
