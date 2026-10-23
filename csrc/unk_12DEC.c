#include "common.h"

void func_80040BB4(void);
void func_80040CBC(s16);
void func_800418B4(void);
void func_80040AA0(u16);
void func_8003E1FC(void);
void func_8003E70C(void);
u32 func_8003E240(s32);
void func_800411FC(u16);
UnkNode *func_8003FC64(s32);
UnkNode *func_8003FD64(s32, UnkNode *);
void func_8003FFF0(UnkNode *);
void func_80040044(u32 *);
void func_80040190(void);
void func_800401FC(void);
void func_8004027C(void);
void func_800402F4(void);
u32 *func_80040574(s32);

void func_8003F5EC(void) {
    s32 i;

    for (i = 0x200; i > 0; i--) {
        VSync(0);
    }
}

/* Tells the drive to spin down, retrying up to sixteen times, then resets the
 * audio state either way.
 *
 * THE BLOCKER THIS FUNCTION USED TO BE, and how it was removed. D_800814D4 is
 * reached BOTH ways here: the `D_800814D4 = 0x84` store is
 * `sb %gp_rel(D_800814D4)` while `&D_800814D4` passed to CdControlB is an
 * absolute `lui %hi` / `addiu %lo` pair. No unit-level setting can express
 * that -- a `-G` value decides per symbol SIZE and an `.extern` override
 * decides per SYMBOL, and both of those apply to every access alike -- so this
 * could not be carved into a -G8 unit like its neighbours (18 differing words
 * there).
 *
 * maspsx can express it, because its %gp_rel rewrite fires on a load or store
 * and leaves an address materialisation alone. What blocked that route was
 * that it was written as a .sdata DEFINITION -- `.globl` + `.type` + `.size` +
 * a label + `.byte 0` -- which makes the symbol a defined OBJECT in a 1-byte
 * .sdata section, and splat's legacy linker-script generation cannot place one
 * object's .sdata at a distant fixed address.
 *
 * It does not need to be a definition. maspsx's `_uses_gp` only consults its
 * `sdata_entries` table, which the `.size` record alone populates, so:
 *
 *     __asm__(".sdata\n\t.size D_800814D4,1\n\t.text");
 *
 * gives the identical 25 words while DEFINING NOTHING -- `readelf -sS` shows
 * .sdata at 0 bytes and D_800814D4 still UND with size 1, so the linker
 * script's own `D_800814D4 = 0x800814D4;` resolves it and there is no
 * placement to arrange. Verified both ways: definition route MATCH, size-record
 * route MATCH, and the object differs only in whether the symbol is defined.
 *
 * The `goto done` is load-bearing in the usual way (section 8): the retry
 * exhaustion path has to fall through to func_8003E70C while the success path
 * jumps past it, and an inline `return` in the arm costs a `j` plus a second
 * epilogue. */
__asm__(".sdata\n\t.size D_800814D4,1\n\t.text");

void func_8003F624(void) {
    s32 i;

    D_800814D4 = 0x84;
    for (i = 0x10; i != 0; i--) {
        if (CdControlB(0xE, &D_800814D4, NULL) != 0) {
            goto done;
        }
    }
    func_8003E70C();
done:
    func_8003F5EC();
}

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

void func_8003F6F4(void) {
    s32 i;

    for (i = 7; i >= 0; i--) {
        s32 r;

        /* abs(): the target's `bgez / addu / negu` is the literal output
         * template of the MIPS abssi2 pattern. Any C conditional leaves a copy
         * visible to cse, which canonicalises either the compare or the
         * negation to the other register. Indexing D_80083120[i] rather than
         * walking a pointer gives the single giv decrement that reorg
         * duplicates into the j's delay slot, and makes the loop large enough
         * that move_movables leaves the `li 5` in place. */
        switch (abs(D_80083120[i].unk_00)) {
        case 5:
            r = func_80053428(&D_80083120[i]);
            break;
        case 6:
            r = func_80053604(&D_80083120[i]);
            break;
        default:
            continue;
        }
        if ((s16) r != 0) {
            D_80083120[i].unk_00 = 0;
        }
    }
}

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

/* Uploads one 16x1 strip of a texture page to VRAM and records its owner in
 * D_80082EC0, unless the caller's flag bits say to skip either half.
 *
 * Three shapes are load-bearing:
 *  - arg3 is s16. As s32 the parameter has only three references, which leaves
 *    its global.c priority below arg2's, and the two swap $s1 and $s2 -- the
 *    difference this function was parked for. Narrowing it adds the HImode
 *    pseudo that tips the order. It is also what the callers show: both
 *    func_80041344 and func_80041588 sign-extend the fourth argument with
 *    sll/sra before the jal, which is the conversion to a short parameter.
 *  - rect[0] is assigned first even though its store comes out last. Its value
 *    is computed before rect[1]'s in the target and sched1 sinks only the
 *    store; writing it last costs 14 words.
 *  - the second D_80082EC0 store goes through a base pointer local so the
 *    +0x100 stays on the store as a displacement. Written as
 *    `D_80082EC0[n + 0x40] = 0;` gcc folds the 0x100 into the lo_sum and the
 *    store's displacement becomes 0. One pointer local at the top of the
 *    function, shared with the first store, is much worse (45 DIFF): the
 *    target rebuilds the base in each arm. */
void func_8003F80C(u32 arg0, s32 arg1, s32 arg2, s16 arg3) {
    s16 rect[4];
    s32 n = (arg1 - 0x7000) >> 6;

    if ((arg3 & 1) == 0) {
        rect[0] = ((n & 0x40) << 1) + ((arg1 & 0x3F) << 4);
        rect[1] = (n & 0x3F) + 0x1C0;
        rect[2] = arg2 << 4;
        rect[3] = 1;
        LoadImage(rect, (u32 *) arg0);
    }
    if ((arg3 & 2) == 0) {
        D_80082EC0[n] = (arg0 & 0xFFFFFF) | (arg2 << 24);
    }
    if (n < 0x40 && arg2 >= 0x11) {
        u32 *q = D_80082EC0;

        q[n + 0x40] = 0;
    }
}

/* Walks a table of image entries, unpacking each into the work buffer and
 * loading it to VRAM, resetting the buffer pointer when the next image would
 * not fit.
 *
 * The parked symptom was "the width/height pair wants one induction variable
 * with a -2 displacement; gcc makes two". Two source facts fix it:
 *  - the width goes through a local read BEFORE the height's. gcc builds the
 *    giv list by prepending as it discovers them, so the LAST access in the
 *    loop body becomes combine_givs' representative and sets the surviving
 *    pointer's base offset. With the height read last the base is +0x0A and
 *    the width rides it at -0x2, which is the target; with the width last the
 *    base comes out +0x08. sched1 then swaps the two loads back so the height
 *    is loaded first, which is why the emitted order looks like the opposite.
 *  - the size is computed twice, once for the fit test and once for the
 *    accumulation, rather than kept in one variable. The two calls in between
 *    invalidate cse, so the target genuinely recomputes it.
 *
 * `w` and `n` are separate locals for the same reason func_80040490 needs its
 * temporary: written as one expression gcc groups the doubling onto the wrong
 * halfword. */
void func_8003F8F8(Unk3F8F8 *e) {
    while (e->unk_00 != 0) {
        s32 w = e->rect[2];
        s32 n = e->rect[3] * 2;

        if (D_8008148C + D_80080A7C >= D_80081480 + w * n) {
            D_80081480 = D_8008148C;
            DrawSync(0);
        }
        func_8004068C((u8 *) e->unk_00, (u8 *) D_80081480);
        LoadImage(e->rect, (u32 *) D_80081480);
        w = e->rect[2];
        n = e->rect[3] * 2;
        D_80081480 = D_80081480 + w * n;
        e++;
    }
}

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

UnkNode *func_8003FB98(s32 arg0, UnkNode *arg1) {
    UnkNode *n = D_80081498;

    if (n != NULL) {
        do {
            if ((n->unk_1E & 0x200) && arg1 != n) {
                func_80044A50(n);
                func_8003FFF0(n);
                if (!(n->unk_1E & 0x80)) {
                    func_80040044((u32 *) n);
                }
                if (arg1 != NULL) {
                    n = func_8003FD64(arg0, arg1);
                } else {
                    n = func_8003FC64(arg0);
                }
                return n;
            }
            n = n->unk_00;
        } while (n != NULL);
    }
    return NULL;
}

UnkNode *func_8003FC64(s32 arg0) {
    /* The list head is read into its own variable for the test: assigning n
     * directly loads it straight into $s0, where the target loads into $v0 and
     * copies. */
    u32 head;

    if ((D_80013714 & 2) && (arg0 & 0x200)) {
        return NULL;
    }
    head = D_80081490;
    if (head != 0) {
        UnkNode *n = (UnkNode *) head;
        UnkNode *old;
        u8 *p;

        D_80081490 = (u32) n->unk_00;
        func_8003DB4C((int *) n, 0x49);
        old = D_80081498;
        D_80081498 = n;
        n->unk_00 = old;
        if (old != NULL) {
            old->unk_04 = &n->unk_00;
        }
        n->unk_1E = arg0 | 0x4000;
        p = (u8 *) n + 0x124;
        if (arg0 & 0x100) {
            p = (u8 *) n + 0xF4;
            n->unk_0C = (u32) p;
        } else if (arg0 & 6) {
            p = (u8 *) n + 0x100;
            n->unk_0C = (u32) p;
        } else if (arg0 & 0x41) {
            p = (u8 *) n + 0x10C;
            n->unk_0C = (u32) p;
        }
        if (arg0 & 0x157) {
            /* In place, not `(UnkVec *) (p - 0x18)`: the target reuses p's
         * register, and reorg then lifts the decrement into the delay slot. */
        p -= 0x18;
        n->unk_08 = (UnkVec *) p;
        }
        return n;
    }
    if (arg0 & 0x200) {
        return NULL;
    }
    return func_8003FB98(arg0, NULL);
}

UnkNode *func_8003FD64(s32 arg0, UnkNode *arg1) {
    /* Same two shapes as func_8003FC64: the list head read into its own
     * variable for the test, and p decremented in place. */
    u32 head;

    if ((D_80013714 & 2) && (arg0 & 0x200)) {
        return NULL;
    }
    head = D_80081490;
    if (head != 0) {
        UnkNode *n = (UnkNode *) head;
        u8 *p;

        D_80081490 = (u32) n->unk_00;
        func_8003DB4C((int *) n, 0x49);
        n->unk_00 = arg1->unk_00;
        arg1->unk_00 = n;
        n->unk_04 = &arg1->unk_00;
        if (n->unk_00 != NULL) {
            n->unk_00->unk_04 = &n->unk_00;
        }
        n->unk_1E = arg0 | 0x4000;
        p = (u8 *) n + 0x124;
        if (arg0 & 0x100) {
            p = (u8 *) n + 0xF4;
            n->unk_0C = (u32) p;
        } else if (arg0 & 6) {
            p = (u8 *) n + 0x100;
            n->unk_0C = (u32) p;
        } else if (arg0 & 0x41) {
            p = (u8 *) n + 0x10C;
            n->unk_0C = (u32) p;
        }
        if (arg0 & 0x157) {
            p -= 0x18;
            n->unk_08 = (UnkVec *) p;
        }
        return n;
    }
    if (arg0 & 0x200) {
        return NULL;
    }
    return func_8003FB98(arg0, arg1);
}

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

void func_800400B8(void) {
    Unk80083160 *s = &D_80083160;

    if (D_80080A86 == 0 && s->unk_1DC != 0) {
        func_80046884((u16 *) ((u8 *) s + 0x18), (Unk46AFC *) ((u8 *) s + 0x20), 0);
        func_8003BFE4();
    }
    func_80040190();
    if (D_800814A0 & 0x8000) {
        func_800401FC();
    }
    D_800814A0 = 0;
    if (D_80080A86 == 0) {
        func_8004027C();
        func_8004D70C();
        if (s->unk_1DC != 0) {
            func_800AC3EC();
            func_800AC4C4();
        }
    }
    func_800402F4();
    func_80044B48();
}

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

    func_8004068C(arg0, (u8 *) p);
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

/* LZSS-style unpacker: a bit stream of flags interleaved with literals, a
 * 2-byte long match (12-bit offset, 4-bit length with an escape to a
 * following byte) and a 1-byte short match (2 flag bits of length, offset
 * byte where 0 means 0x100). Returns the end of the output.
 *
 * The outer loop has to be a `goto` loop. A `for`/`while` emits
 * NOTE_INSN_LOOP_BEG, which puts the whole body at loop_depth 2, and
 * reg_n_refs is incremented by loop_depth per reference. The backward goto
 * gets no loop note, so every reference outside the copy loop drops from
 * weight 2 to weight 1 -- and because len's references are split across the
 * two depths while off's and p's each sit at one, that rescales the three
 * contested allocnos unequally: len goes from 31 refs (priority 4.00, last of
 * the three) to 17 (2.19, first). len then takes $v1 and off and p, which
 * conflict with len but not with each other, both land in $a2, as in the
 * target. No live range changes; only the weighting does.
 *
 * `goto done` for the end-of-stream case is also load-bearing: an inline
 * `return dst` there costs 49 words. And the length test in the long arm has
 * to put the nonzero arm first, or the wrong arm goes out of line. */
u8 *func_8004068C(u8 *src, u8 *dst) {
    u32 c = 0;
    s32 n = 1;

top:
    if (--n == 0) {
        c = *src++;
        n = 8;
    }
    if ((c & 1) == 0) {
        c >>= 1;
        *dst++ = *src++;
        goto top;
    }
    {
        u32 off;
        s32 len;
        u8 *p;

        c >>= 1;
        if (--n == 0) {
            c = *src++;
            n = 8;
        }
        if ((c & 1) == 0) {
            off = (src[0] << 8) | src[1];
            c >>= 1;
            if (off == 0) {
                goto done;
            }
            len = off & 0xF;
            src += 2;
            if (len != 0) {
                len += 2;
            } else {
                len = *src++ + 1;
            }
            off >>= 4;
        } else {
            c >>= 1;
            if (--n == 0) {
                c = *src++;
                n = 8;
            }
            len = (c & 1) << 1;
            c >>= 1;
            if (--n == 0) {
                c = *src++;
                n = 8;
            }
            len += 2;
            len += c & 1;
            c >>= 1;
            off = *src++;
            if (off == 0) {
                off = 0x100;
            }
        }
        p = dst - off;
        if (len != 0) {
            do {
                *dst++ = *p++;
                len--;
            } while (len != 0);
        }
    }
    goto top;
done:
    return dst;
}

INCLUDE_ASM("asm/nonmatchings/unk_12DEC", func_800407C0);

void func_80040A88(void (*arg0)(void)) {
    D_80082E60.unk_04 = arg0;
    D_80082E60.unk_0D = 0;
    D_80082E60.unk_0C = 0;
}

void func_80040AA0(u16 arg0) {
    Unk8006CE80 *e = &D_8006CE80[arg0];
    Unk80082E60 *s = &D_80082E60;
    Unk80082E60 *q;

    if (s->unk_08.h == 3) {
        return;
    }
    if (arg0 == 6 && s->unk_0A == 2) {
        func_80043568();
    }
    q = &D_80082E60;
    q->unk_0E = 1;
    q->unk_08.h = arg0;
    if (q->unk_0A != e->unk_00) {
        if (arg0 == 5) {
            func_80053DA8(0x72);
        }
        D_80080A88 = 0;
    }
    if (D_80080A88 == 0) {
        func_80053DA8(0xCF);
        func_800542BC();
    }
}

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

/* Loads a mode's image data: for kind 1/2/3 it streams the entry's VRAM blocks
 * in 0x100-row pairs through func_8003F6D4/func_8003E4FC, flushes the tail
 * partial block, and for kind == 4 instead publishes two pointers out of the
 * entry. Either way it then selects one of two work buffers by kind == 2 and
 * records the mode's handler in D_80082E60.unk_04.
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - `kind` is s32, not u16 (146 words): as a u16 the pseudo stays in HImode
 *    and every test needs its own andi, where the target compares the register
 *    straight off the lhu.
 *  - the dispatch is `if (kind != 4) { ... } else { arm4 }`, not
 *    `if (kind == 4) { arm4 } else ...` (78 DIFF). The target branches TO the
 *    kind == 4 arm, so that arm has to be the else, out of line, falling
 *    through into the common tail.
 *  - the tail block's height reuses the variable `n` that held the row count
 *    (144 words). That pseudo is live across the loop's calls, so it is in a
 *    callee-saved register and needs the explicit copy to become the argument;
 *    a fresh variable is allocated $a0 directly and the copy coalesces away.
 *    Harmless semantically -- the row count is dead by then -- but it is the
 *    only spelling that reproduces the register.
 *  - e->unk_08 is dereferenced twice rather than cached (144 words). cse
 *    rewrites the second load as a copy of the first one's register, and that
 *    copy is the target's `addu $v1,$v0,$zero`, needed because the first field
 *    load clobbers the base.
 *  - the second pointer hop is a struct field, not `*(u32 *) ...` (5 DIFF).
 *    Via a plain dereference that MEM is not MEM_IN_STRUCT_P and neither are
 *    the two scalar stores, so true_dependence's differing-flags exception
 *    does not apply and sched1 cannot interleave the three loads the way the
 *    target does.
 *  - D_800E6000 is an incomplete array used as `(u32) D_800E6000` (3 DIFF as a
 *    scalar): the compiler-split high/lo_sum pair is two insns, so reorg can
 *    put the addiu in the `j`'s delay slot. D_80126804 deliberately stays a
 *    scalar -- its lui/addiu are adjacent in the target, so the `la` macro is
 *    right there. */
void func_80040CBC(s16 arg0) {
    s32 buf1[2];
    s32 buf2[2];
    Unk8006CE80 *e = &D_8006CE80[arg0];
    s32 kind = e->unk_00;

    D_80080A86 = 0;
    D_8008333C[0] = 0;
    if (kind != 0) {
        if (kind != 4) {
            if (D_80082E60.unk_0A != kind) {
                s32 n;
                Unk40CBCB *p;
                s32 i;

                D_80082E60.unk_0A = kind;
                n = D_8006CE6C[kind];
                p = D_8006CE44[kind].unk_04;
                i = 0;
                while (i < n - 0x200) {
                    func_8003F6D4(0x100, (p->unk_00 & 0x7FFFFF) + (i << 11), buf1,
                                  p->unk_04 + i);
                    func_8003E4FC(6, (u8 *) buf1, 0);
                    i += 0x100;
                    func_8003F6D4(0x100, (p->unk_00 & 0x7FFFFF) + (i << 11), buf2,
                                  p->unk_04 + i);
                    func_8003E4FC(6, (u8 *) buf2, 0);
                    i += 0x100;
                    func_8003F320();
                }
                n = p->unk_00 >> 23;
                if (n != 0) {
                    func_8003F6D4(n, (p->unk_00 & 0x7FFFFF) + (i << 11), buf1,
                                  p->unk_04 + i);
                    func_8003E4FC(6, (u8 *) buf1, 0);
                }
                func_8003F320();
            }
        } else {
            func_800418B4();
            D_80189394 = ((Unk40CBCA *) e->unk_08)->unk_04;
            D_801B8EB8 = ((Unk40CBCA *) e->unk_08)->unk_00->unk_00;
        }
    }
    if (kind == 2) {
        D_8008148C = (u32) D_800E6000;
        D_80080A7C = 0x18800;
    } else {
        D_8008148C = (u32) &D_80126804;
        D_80080A7C = 0x38000;
    }
    D_80082E60.unk_04 = (void (*)(void)) D_8006CE44[kind].unk_00;
}

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
