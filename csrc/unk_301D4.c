#include "common.h"

void func_8005A428(void);

void func_8005C9D4(void) {
    func_8005A428();
}

void func_8005A4E8(u8, u8, u8);

void func_8005C9F4(u8 a, u8 b, u8 c) {
    func_8005A4E8(a, b, c);
}

INCLUDE_ASM("asm/nonmatchings/unk_301D4", func_8005CA1C);

void func_8005B418(s32);

void func_8005CA4C(s16 a) {
    func_8005B418(a);
}

void func_8005CA90(s32);

void func_8005CA70(void) {
    func_8005CA90(0);
}
