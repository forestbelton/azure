#include "common.h"

INCLUDE_ASM("asm/nonmatchings/unk_113D0", func_8003DBD0);

INCLUDE_ASM("asm/nonmatchings/unk_113D0", func_8003DE58);

Unk800777AC *func_8003DF0C(Unk800777AC *p, s32 arg1, s16 *arg2) {
    Unk800777AC *found = NULL;
    u32 i;

    *arg2 = 0;
    i = 0;
    do {
        if ((p->unk_00 & 0x20) && p->unk_06 == arg1) {
            found = p;
            *arg2 = i;
        }
        i++;
    } while ((p++->unk_00 & 0x80) == 0);
    *arg2 = *arg2 - (i >> 1);
    return found;
}

INCLUDE_ASM("asm/nonmatchings/unk_113D0", func_8003DF74);

INCLUDE_ASM("asm/nonmatchings/unk_113D0", func_8003E02C);

void func_8003E0C4(void *m, void *arg1, void *arg2) {
    s32 flag;

    PushMatrix();
    SetTransMatrix(m);
    SetRotMatrix(m);
    RotTrans(arg1, arg2, &flag);
    PopMatrix();
}
