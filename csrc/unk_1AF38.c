#include "common.h"

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047738);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047784);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800477F4);

void func_800477F4(UnkSlot *);

void func_800478B8(UnkSlot *s) {
    func_800477F4(s);
    func_80047694(s);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800478E8);

void func_800479AC(void) {
    bzero(D_80083D08, 0x30);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800479D4);

u8 *func_80047A44(s32 i) {
    return &D_800FC000[i * 0xD000];
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047A68);

u32 func_80047AA4(u32 a) {
    return a + 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047AB0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047B80);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047BC0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047C00);

s32 func_80047C60(u8 *p) {
    return (p - D_800FC000) / 0xD000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047C8C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047CD8);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047D44);

void func_800479D4(u8 *, u8 *, s32);

u8 *func_80047DB8(u16 a) {
    func_800479D4(D_80016000, D_80023000, a);
    /* `&D_8008152Cs`, a 4-byte scalar, and not the array D_8008152C: a small
       object's address is the one-insn `la` macro, and the target keeps the
       lui and the addiu adjacent and ahead of the epilogue. Through the
       incomplete array the compiler splits them and sinks the addiu past the
       `lw $ra`, which also lets the stack adjustment into the delay slot --
       5 DIFF. */
    return (u8 *) &D_8008152Cs;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047DF0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047E30);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047E78);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047EEC);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047F90);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047FF4);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048088);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048118);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048190);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800481E0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048224);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_8004827C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800482C8);

void func_80047784(u8 *, s32, s32);

void func_80048374(u8 *p, s16 b, s16 c) {
    *(u16 *) (p + 0x14) |= 0x400;
    func_80047784(p, b, c);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800483AC);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_8004846C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048500);

void func_80048500(s32, s32);

void func_80048568(s16 a) {
    func_80048500(a, 0);
}

void func_80048500(s32, s32);

void func_80048590(s16 a) {
    func_80048500(a, 1);
}

void func_80048500(s32, s32);

void func_800485B8(s16 a) {
    func_80048500(a, 2);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800485E0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048660);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048734);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_8004878C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048870);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800488F0);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048998);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800489F4);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048A44);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048AC8);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048B28);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048B8C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048C3C);

void func_80048C3C(s32);

void func_80048D00(void) {
    func_80048C3C(0);
}
