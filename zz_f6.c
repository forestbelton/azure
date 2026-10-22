#include "common.h"
typedef struct { u32 a; u8 b; u8 pad; u8 c; } SaveBlk;
void func_800439F8(void) {
    SaveBlk *p = (SaveBlk *) 0x80013180;
    if (D_80080A8A != 0) {
        D_80080A8A = 0;
        p->a = D_80081494;
        p->b = D_800814AC;
        p->c = D_800814A4;
    } else {
        D_80081494 = p->a;
        D_800814AC = p->b;
        D_800814A4 = p->c;
    }
}
