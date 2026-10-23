B = "unk_1C540"
CANDS = [
("func_8005B3D8", B, """void func_8005DA88(s16 *);

void func_8005B3D8(s32 x, s32 y) {
    s16 t[12];

    *(s32 *) &t[0] = 6;
    /* `(x << 16) >> 8`, not `(s16) x << 8`: the two shifts cannot be combined
       (the arithmetic right shift fills from bit 15, so the result is not
       x << 8), which is why the target spends both. Any spelling that means
       "sign-extend then shift" collapses to one `sll` because the value is
       stored as a halfword. */
    t[4] = (x << 16) >> 8;
    t[5] = (y << 16) >> 8;
    func_8005DA88(t);
}"""),
("func_8005A33C", B, """void func_800585A0(void);

void func_8005A33C(void) {
    /* Nested ifs, not `> 0 && < 4`: written with && the two bounds combine
       into one `addiu -1` / `sltiu 3` range check (13 DIFF). */
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_800585A0();
        }
    }
    D_80073824[0] = 1;
}"""),
]
