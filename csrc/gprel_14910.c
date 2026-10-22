#include "common.h"

void func_80041110(s32 arg0) {
    s32 *temp;

    *D_800814E0 = arg0;
    temp = D_800814E0;
    D_800814E0 = temp + 1;
    temp[1] = 0;
}
