#include "common.h"

void func_8003DB4C(int *dst, int n) {
    while (n != 0) {
        *dst = 0;
        dst++;
        n--;
    }
}

void func_8003DB6C(int *dst, int *src, int n) {
    while (n != 0) {
        *dst = *src;
        src++;
        n--;
        dst++;
    }
}

INCLUDE_ASM("asm/nonmatchings/mem", func_8003DB94);
