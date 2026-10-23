CANDS = [
("func_8003C8E4", "unk_72A8", """s32 func_8003C5A4(void);

u32 *func_8003C8E4(void) {
    s32 i = func_8003C5A4();

    /* The arm written FIRST ends up SECOND in memory, so the `>= 0` arm first
       is what gives the target's `bgez`; written as `if (i < 0) return 0;`
       the branch comes out `bltz` and the two arms swap (6 DIFF). */
    if (i >= 0) {
        return &((u32 *) 0x800135C4)[i];
    } else {
        return 0;
    }
}"""),
]
