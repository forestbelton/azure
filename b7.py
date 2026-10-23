CANDS = [
("func_8003C128", "unk_72A8", """UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_8003C3B4(void);

void func_8003C128(void) {
    /* u8 * rather than UnkNode *: the node's +0x20 is past the end of the
       struct, so the offsets stay explicit until something says what is
       there. */
    u8 *n = (u8 *) func_8003FE78(0x15, &D_80082D38, 0x49);

    *(u32 *) (n + 0x10) = (u32) func_8003C3B4;
    *(u32 *) (n + 0x20) = 0;
}"""),
("func_80047DB8", "unk_1AF38", """void func_800479D4(u8 *, u8 *, s32);

u8 *func_80047DB8(u16 a) {
    func_800479D4(D_80016000, D_80023000, a);
    /* `&D_8008152Cs`, a 4-byte scalar, and not the array D_8008152C: a small
       object's address is the one-insn `la` macro, and the target keeps the
       lui and the addiu adjacent and ahead of the epilogue. Through the
       incomplete array the compiler splits them and sinks the addiu past the
       `lw $ra`, which also lets the stack adjustment into the delay slot --
       5 DIFF. */
    return (u8 *) &D_8008152Cs;
}"""),
("func_8004D064", "unk_1C540", """u8 *func_8004D064(u8 *p) {
    /* The parameter is advanced in place and returned, which is the `addu
       $v0, $a0, $zero` at the end; computing `p + 0xC` into the return value
       instead fills both delay slots and loses a word. */
    switch (p[1] & 0xF0) {
    case 0x20:
        p += 0xC;
        break;
    case 0x30:
        p += 0x18;
        break;
    default:
        p += 0xC;
        break;
    }
    return p;
}"""),
("func_8005AC30", "unk_1C540", """s32 func_8005ECF8(s32);

s16 func_8005AC30(s16 a) {
    /* Two returns of the same call, not `func_8005ECF8(a != 0)`, which
       store-flags into one `sltu`. And the arm written FIRST ends up SECOND in
       memory: crossjumping merges the two tails and leaves the second arm as
       the fall-through, so `a == 0` first is what gives the target's `beqz`. */
    if (a == 0) {
        return func_8005ECF8(0);
    } else {
        return func_8005ECF8(1);
    }
}"""),
]
