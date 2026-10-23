A = "unk_72A8"
CANDS = [
("func_800374F4", A, """u32 func_800374F4(u16 n) {
    /* The product must be unsigned -- `(u32)` -- or the high half comes out
       with `sra` where the target has `srl`. */
    return (u32) (n * (u16) (rand() << 1)) >> 16;
}"""),
("func_80039760", A, """void func_80039760(u8 *p) {
    u32 i = p[0x27];
    u8 *c = *(u8 **) (p + 0x1C);

    /* The byte offsets are written INSIDE each index, not folded into a
       `c = base + i * 4` pointer: through a derived pointer the four loads
       interleave with the shifts and the registers shift with them (12 DIFF).
       Same rule as the field offsets in docs/matching-levers.md section 6. */
    *(u32 *) (p + 0x1C) = c[i * 4] + (c[i * 4 + 1] << 8) +
                          (c[i * 4 + 2] << 16) + (c[i * 4 + 3] << 24);
}"""),
("func_8003ADB4", A, """/* The array spelling, not the scalar `D_80082E6A`: a 1-byte scalar is small
   data at -G8 and maspsx expands its load into one register, where the target
   has the compiler-split pair across two. */
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s16 func_8003ADB4(u16 a) {
    if (D_80082E6Aa[0] != 2) {
        return func_80053DA8(a);
    } else {
        return 0;
    }
}"""),
("func_80047B80", "unk_1AF38", """s32 func_80047B80(void) {
    /* The three initialisers are in source order in the listing, so the
       result and the counter are declared before the pointer. */
    s32 r = -1;
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if (*(u32 *) p == 0) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}"""),
("func_80047BC0", "unk_1AF38", """s32 func_80047BC0(s32 k) {
    s32 r = -1;
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if (*(s16 *) (p + 6) == k) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}"""),
]
