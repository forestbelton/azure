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

void func_8003DB94(UnkSlot *slot, UnkElem *elems, s16 idx) {
    slot->unk_00 = elems + idx;
    slot->unk_04 = idx;
    slot->unk_05 = elems->unk_00;
    slot->unk_14 &= 0x9FFF;
    slot->unk_08 = slot->unk_00->unk_04;
}
