CANDS = [
("func_8004A0C0", "unk_1C540", """u32 func_8004A0C0(u32 m) {
    /* The result, the counter and the pointer initialise in source order, and
       the counter's `addu $a1, $a2, $zero` is cse sharing the result's zero. */
    u32 r = 0;
    s32 i = 0;
    u8 *p = D_800712FC;

    for (; i < 0xD; i++) {
        if ((*(u32 *) (p + 4) & m) != 0) {
            r = *(u32 *) p;
            break;
        }
        p += 8;
    }
    return r;
}"""),
]
