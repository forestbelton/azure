CANDS = [
("func_8004D9E0", "unk_1C540", """void func_8004D9E0(u8 *p, s32 b, s32 c, s32 d, s32 e) {
    /* `b * 4` is the LEFT operand of the outer add: the target's
       `addu $a1, $a1, $a3` puts the sum in the register holding b * 4. */
    p[2] = b * 4 + (b * d / 2 - 0x80);
    p[3] = c * (e + 0x10) - 0x80;
}"""),
]
