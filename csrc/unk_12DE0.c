#include "common.h"

void func_80040BB4(void);
void func_80040CBC(u8);
void func_80040AA0(u16);
void func_8003E1FC(void);
u32 func_8003E240(s32);
void func_800411FC(u16);
u32 *func_80040574(s32);
void func_8003F80C(u32, u16, u16, s32);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F5E0);

void func_8003F5EC(void) {
    s32 i;

    for (i = 0x200; i > 0; i--) {
        VSync(0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F624);

s32 func_8003F688(s32 arg0) {
    s32 acc = 0;
    s32 mul = 1;
    s32 i;

    for (i = 0; i < 8; i++) {
        acc += (mul * (arg0 & (0xF << (i * 4)))) >> (i * 4);
        mul = mul * 10;
    }
    return acc;
}

void func_8003F6D4(s32 arg0, s32 arg1, s32 *arg2, s32 arg3) {
    arg2[0] = (arg1 & 0x7FFFFF) | (arg0 << 23);
    arg2[1] = arg3;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F6F4);

s32 func_8003F794(s16 arg0, s16 arg1) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        if (D_80083120[i].unk_00 == 0) {
            D_80083120[i].unk_00 = arg0;
            D_80083120[i].unk_02 = 0;
            D_80083120[i].unk_04 = arg1;
            D_80083120[i].unk_06 = 0;
            return (s16) i;
        }
    }
    return -1;
}

void func_8003F7E4(void) {
    s32 i;

    for (i = 0x7F; i >= 0; i--) {
        D_80082EC0[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F80C);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003F8F8);

void func_8003F9E0(UnkEntry *e) {
    while (e->unk_00 != 0) {
        func_8003F80C(e->unk_00, e->unk_04, e->unk_06, 0);
        e++;
    }
}

s32 func_8003FA44(s32 arg0) {
    UnkNode *n = (UnkNode *) D_80081490;
    s32 count = 0;

    /* Plain `while` loops: gcc's rotation peels the first increment and puts
       the back-edge one in the branch delay slot, which is where the target's
       compensating `addiu -1` comes from. Writing that peeling out by hand
       folds the peeled increment into a `li 1`. */
    while (n != NULL) {
        count++;
        if (count >= arg0) {
            return 1;
        }
        n = n->unk_00;
    }
    n = D_80081498;
    while (n != NULL) {
        if (n->unk_1E & 0x200) {
            count++;
            if (count >= arg0) {
                return 1;
            }
        }
        n = n->unk_00;
    }
    return 0;
}

void func_8003FAD4(void) {
    u8 *q = D_801EAFE8;
    s32 i;
    u8 *p;

    /* The counter has to be initialised before the walking pointer: a `for`
     * header puts `i = 0` after `p = q`, and the two callee-saved registers
     * come out saved in that order. */
    i = 0;
    p = q;
    do {
        func_8003DB4C((int *) p, 0x49);
        i++;
        p += 0x124;
    } while (i < 0x100);
    for (i = 0xFE; i >= 0; i--) {
        u8 *next = q + 0x124;

        *(u8 **) q = next;
        q = next;
    }
    *(u32 *) q = 0;
    D_80081498 = NULL;
    D_80081490 = (u32) D_801EAFE8;
    for (i = 0x1F; i >= 0; i--) {
        D_80083360[i] = 0;
        D_800833E0[i] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FB98);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FC64);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8003FD64);

UnkNode *func_8003FE78(s32 flags, UnkNode *arg1, s16 count) {
    UnkNode *n;
    UnkNode *old;
    u8 *p;

    /* Both of these are load-bearing for the match. `flags` must be updated
     * in place (a separate `f = flags | 0x80` lets cse fold the later
     * `| 0x4000` through it into one `ori 0x4080`), and the node pointer
     * must be copied into a local *after* that update, which is what puts
     * `ori $s1` ahead of the `$s0` copy in the prologue. */
    flags |= 0x80;
    n = arg1;

    func_8003DB4C((int *) n, count);

    old = D_80081498;
    D_80081498 = n;
    n->unk_04 = NULL;
    n->unk_00 = old;
    if (old != NULL) {
        old->unk_04 = &n->unk_00;
    }
    n->unk_1E = flags | 0x4000;

    p = (u8 *) n + count * 4;
    if (flags & 6) {
        p -= 0x24;
        n->unk_0C = (u32) p;
    } else if (flags & 0x41) {
        p -= 0x18;
        n->unk_0C = (u32) p;
    }
    if (flags & 0x57) {
        p -= 0x18;
        n->unk_08 = (u32) p;
    }
    return n;
}

UnkNode *func_8003FF2C(s32 flags, UnkNode *arg1, s16 count, UnkNode **link) {
    UnkNode *n;
    u8 *p;

    flags |= 0x80;
    n = arg1;

    func_8003DB4C((int *) n, count);

    /* No saved `old`: the target re-reads n->unk_00 for the back link, in
     * the same register it loaded *link into. */
    n->unk_00 = *link;
    *link = n;
    n->unk_04 = link;
    if (n->unk_00 != NULL) {
        n->unk_00->unk_04 = &n->unk_00;
    }
    n->unk_1E = flags | 0x4000;

    p = (u8 *) n + count * 4;
    if (flags & 6) {
        p -= 0x24;
        n->unk_0C = (u32) p;
    } else if (flags & 0x41) {
        p -= 0x18;
        n->unk_0C = (u32) p;
    }
    if (flags & 0x57) {
        p -= 0x18;
        n->unk_08 = (u32) p;
    }
    return n;
}

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

void func_800402F4(void) {
    s32 i;

    for (i = 0; i < 0x20; i++) {
        void (*f)(u8 *, UnkVec *, UnkSlot *);
        UnkNode *n;

        f = (void (*)(u8 *, UnkVec *, UnkSlot *)) D_80083360[i];
        if (f == NULL) {
            continue;
        }
        n = (UnkNode *) D_800833E0[i];
        /* The clearing store has to sit at the end of the body, falling into
         * the loop advance; as an `else` arm gcc lays it out inline with a
         * jump over it. */
        if (n == NULL) {
            goto clear;
        }
        if (n->unk_1E & 0x800) {
            continue;
        }
        f((u8 *) n + 0x20, n->unk_08, (UnkSlot *) n->unk_0C);
        if (func_80045310(*(u32 *) (D_80083160.unk_00 + 0x8D0)) != 0) {
            return;
        }
        continue;
    clear:
        D_80083360[i] = 0;
    }
}

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

void func_80040490(void *arg0, s16 *rect) {
    /* The temporary fixes the grouping: written as one expression gcc
       doubles rect[2] instead of rect[3]. */
    s32 n = rect[3] * 2;
    u32 *p = func_80040574(rect[2] * n);

    func_8004068C(arg0, p);
    LoadImage(rect, p);
}

void func_800404F8(u16 *rect) {
    u32 *p = func_80040574(((s16) rect[2] >> 1) * *(s16 *) (rect + 3));

    func_8003DB4C((int *) p, ((s16) rect[2] >> 1) * *(s16 *) (rect + 3));
    LoadImage(rect, p);
}

u32 *func_80040574(s32 size) {
    u32 *p;

    /* The limit has to be the left operand: written the other way round gcc
       loads D_80081480 first, where the target loads it last. */
    if (D_8008148C + D_80080A7C <= D_80081480 + size) {
        D_80081480 = D_8008148C;
        DrawSync(0);
    }
    p = (u32 *) D_80081480;
    D_80081480 = D_80081480 + size;
    return p;
}

u32 *func_800405E8(s32 size) {
    u32 *p;

    if (D_8008148C + D_80080A7C <= D_80081480 + size) {
        D_80081480 = D_8008148C;
        DrawSync(0);
    }
    p = (u32 *) (D_8008148C + D_80080A7C - size);
    D_80080A7C = D_80080A7C - size;
    return p;
}

void func_8004066C(s32 arg0) {
    D_80080A7C += arg0;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_8004068C);

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800407C0);

void func_80040A88(void (*arg0)(void)) {
    D_80082E60.unk_04 = arg0;
    D_80082E60.unk_0D = 0;
    D_80082E60.unk_0C = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_80040AA0);

void func_80040B88(void) {
    D_80082E60.unk_18 = D_80082E60.unk_0B;
    func_80040BB4();
}

void func_80040BB4(void) {
    u8 n = D_80082E60.unk_08.b;

    D_80082E60.unk_0B = n;
    func_80040CBC(n);
    D_80082E60.unk_0E = 0;
    D_80082E60.unk_08.h = 0;
    D_80083160.unk_154 = 0;
    D_80083160.unk_198 = 0;
    D_80083160.unk_CC = 0;
    D_80083160.unk_110 = 0;
}

void func_80040C08(void) {
    /* Two struct pointers, and the mode stored directly in each arm rather
     * than through a local: the second pointer is re-materialised where
     * sharing the first would reuse the register, and the three direct
     * stores are cross-jumped into one with the constant in $v0, where a
     * local keeps the value in $v1 and reorders the store. */
    Unk80082E60 *p = &D_80082E60;
    Unk80082E60 *q;

    D_80080A7C = 0x38000;
    D_80081480 = D_8008148C;
    if (p->unk_0B == 0) {
        D_8008148C = (u32) &D_80126804;
        func_800411FC(0);
        func_8003E1FC();
        if (func_8003E240(0) == 0) {
            p->unk_0B = 1;
        } else {
            p->unk_0B = 3;
        }
    } else {
        p->unk_0B = 3;
    }
    q = &D_80082E60;
    q->unk_0D = 0;
    q->unk_0C = 0;
    q->unk_18 = 0;
    q->unk_0F &= 0x7E;
    func_80040CBC(q->unk_0B);
}

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

s32 func_80040F2C(u16 arg0) {
    Unk8006CE80 *e = &D_8006CE80[arg0];
    u32 *p = e->unk_04;
    void (**q)(void);

    /* All three early exits go through one label at the end, which is what
     * lets reorg steal the shared `v0 = 0` into one branch's delay slot and
     * leaves the other two branching to the label itself. Written as three
     * separate `return 0`s, gcc emits a trampoline for the shared pair. */
    if (p == NULL) {
        goto done;
    }
    q = (void (**)(void)) p[0];
    if (q == NULL) {
        goto done;
    }
    if (*q == NULL) {
        goto done;
    }
    do {
        (*q)();
        if (D_80082E60.unk_0E != 0) {
            return 1;
        }
        q++;
    } while (*q != NULL);
done:
    return 0;
}

void func_80040FDC(u16 arg0) {
    /* The element pointer must be its own local: taking the field directly
     * off D_8006CE80[arg0] schedules the table's address ahead of the index
     * multiply, where the target has it after. */
    Unk8006CE80 *e = &D_8006CE80[arg0];
    u32 *p = e->unk_04;

    if (p != NULL) {
        u32 *q = (u32 *) p[2];

        if (q != NULL) {
            func_8003F8F8(q);
        }
    }
}

void func_80041038(u16 arg0) {
    Unk8006CE80 *e = &D_8006CE80[arg0];
    u32 *p = e->unk_04;

    if (p != NULL) {
        u32 *q = (u32 *) p[1];

        if (q != NULL) {
            func_8003F9E0((UnkEntry *) q);
        }
    }
}

void func_80041094(u16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4) {
    func_80040AA0(arg0);
    D_80082E60.unk_10 = arg1;
    D_80082E60.unk_12 = arg2;
    D_80082E60.unk_14 = arg3;
    D_80082E60.unk_16 = arg4;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DE0", func_800410FC);
