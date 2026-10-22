#include "common.h"

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F5E0);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F5EC);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F624);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F688);

void func_8003F6D4(s32 arg0, s32 arg1, s32 *arg2, s32 arg3) {
    arg2[0] = (arg1 & 0x7FFFFF) | (arg0 << 23);
    arg2[1] = arg3;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F6F4);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F794);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F7E4);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F80C);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F8F8);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F9E0);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FA44);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FAD4);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FB98);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FC64);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FD64);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FE78);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FF2C);

void func_8003FFF0(UnkNode *n) {
    UnkNode **link = n->unk_04;
    UnkNode *next;

    n->unk_1E &= 0xBFFF;
    if (link != NULL) {
        next = n->unk_00;
        if (next != NULL) {
            next->unk_04 = link;
        }
        *link = next;
    } else {
        next = n->unk_00;
        D_80081498 = next;
        if (next != NULL) {
            next->unk_04 = NULL;
        }
    }
    n->unk_00 = NULL;
    n->unk_04 = NULL;
}

void func_80040044(u32 *arg0) {
    u32 temp = D_80081490;

    D_80081490 = (u32) arg0;
    arg0[1] = 0;
    arg0[0] = temp;
}

void func_80040060(UnkNode **link, UnkNode *n) {
    UnkNode *old;

    n->unk_1E |= 0x4000;
    old = *link;
    *link = n;
    if (old != NULL) {
        old->unk_04 = &n->unk_00;
        n->unk_00 = old;
    } else {
        n->unk_00 = NULL;
    }
    /* Compares against the head pointer's value, not its address. */
    if (link == (UnkNode **) D_80081498) {
        n->unk_04 = NULL;
        return;
    }
    n->unk_04 = link;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800400B8);

void func_80040190(void) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            u32 fn = p->unk_10;
            UnkNode *next = p->unk_00;   /* read before the call clobbers it */

            if ((s32) fn < 0 && !(p->unk_1E & 0x400)) {
                ((void (*)(void *, UnkVec *, u32)) fn)((u8 *) p + 0x20,
                                                       p->unk_08, p->unk_0C);
            }
            p = next;
        } while (p != NULL);
    }
}

void func_800401FC(void) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            UnkNode *next = p->unk_00;

            if (p->unk_1E & 0x8000) {
                func_80044A50(p);
                func_8003FFF0(p);
                if (!(p->unk_1E & 0x80)) {
                    func_80040044((u32 *) p);
                }
            }
            p = next;
        } while (p != NULL);
    }
}

void func_8004027C(void) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            if (p->unk_1E & 8) {
                UnkVec *v = p->unk_08;

                v->unk_00 += v->unk_0C;
                v->unk_04 += v->unk_10;
                v->unk_08 += v->unk_14;
            }
            p = p->unk_00;
        } while (p != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800402F4);

void func_800403BC(s32 arg0) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            if (p->unk_10 == arg0) {
                p->unk_1E |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            p = p->unk_00;
        } while (p != NULL);
    }
}

void func_80040418(void) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            p->unk_1E |= 0xC00;
            p = p->unk_00;
        } while (p != NULL);
    }
}

void func_80040454(void) {
    UnkNode *p = D_80081498;

    if (p != NULL) {
        do {
            p->unk_1E &= 0xF3FF;
            p = p->unk_00;
        } while (p != NULL);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040490);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800404F8);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040574);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800405E8);

void func_8004066C(s32 arg0) {
    D_80080A7C += arg0;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8004068C);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800407C0);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040A88);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040AA0);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040B88);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040BB4);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040C08);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040CBC);

s32 func_80040F00(s32 *arg0) {
    s32 count = 0;

    if (*arg0 != 0) {
        do {
            arg0++;
            count++;
        } while (*arg0 != 0);
    }
    return count;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040F2C);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040FDC);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80041038);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80041094);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800410FC);
