#include "common.h"

/* Bare file-scope size records: each symbol lands in maspsx's sdata_entries
   while nothing is DEFINED, so maspsx rewrites every load and store of it in
   this unit to %gp_rel while leaving address materialisation absolute. See the
   $gp section of docs/carved-sweep.md; adding a symbol changes every access to
   it in this whole unit, so grep this file first. */
__asm__(".sdata\n\t.size D_8008152C,4\n\t.size D_80081528,4\n"
        "\t.text");

void func_80047738(u8 *p, s16 a, s16 b) {
    u8 *q = *(u8 **) (*(u8 **) *(u8 **) *(u8 **) (p + 0x28) + a * 4) + b * 8;
    u32 v;

    *(u8 **) p = q;
    v = *(u32 *) (q + 4);
    p[4] = b;
    *(u16 *) (p + 0x14) &= 0xFFBF;
    *(u32 *) (p + 8) = v;
}

void func_80047738(u8 *, s16, s16);

/* s32 parameters with explicit `(s16)` casts at the call, because the caller
   func_80048374 already declares it that way and sign-extends its own s16s
   before passing them. The casts are the sll/sra pair here. */
void func_80047784(u8 *p, s32 b, s32 c) {
    s32 v;

    func_80047738(p, (s16) b, (s16) c);
    v = **(u16 **) p / D_80080A84;
    *(u16 *) (p + 0x14) &= 0x9FFF;
    p[5] = v;
    func_80047694((UnkSlot *) p);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800477F4);

void func_800477F4(UnkSlot *, s32);

/* The second argument passes through untouched, which is invisible here and
   shows up at func_80048AC8. */
void func_800478B8(UnkSlot *s, s32 b) {
    func_800477F4(s, b);
    func_80047694(s);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800478E8);

void func_800479AC(void) {
    bzero(D_80083D08, 0x30);
}

void func_800479D4(u8 *a, u8 *b, u16 c) {
    s32 i = (s16) c;
    s32 *r;

    r = func_80043704(i, (s32) a);
    func_8003E4FC(6, (u8 *) r, 0);
    r = func_800437A4(i, (s32) b);
    func_8003E4FC(6, (u8 *) r, 0);
}

u8 *func_80047A44(s32 i) {
    return &D_800FC000[i * 0xD000];
}

u8 *func_80047A44(s32);
void func_800479D4(u8 *, u8 *, u16);

void func_80047A68(s32 a, s32 b) {
    func_800479D4(func_80047A44(a), D_80080A90, b);
}

u32 func_80047AA4(u32 a) {
    return a + 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80047AB0);

s32 func_80047B80(void) {
    /* The three initialisers are in source order in the listing, so the
       result and the counter are declared before the pointer. */
    s32 r = -1;
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if (*(u32 *) p == 0) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}

s32 func_80047BC0(s32 k) {
    s32 r = -1;
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if (*(s16 *) (p + 6) == k) {
            r = i;
            break;
        }
        p += 8;
    }
    return r;
}

void func_80047C00(void) {
    s32 i = 0;
    u8 *p = D_80083D08;

    for (; i < 6; i++) {
        if ((*(u16 *) (p + 4) & 1) != 0) {
            bzero(p, 8);
        }
        p += 8;
    }
}

s32 func_80047C60(u8 *p) {
    return (p - D_800FC000) / 0xD000;
}

s32 func_80047C8C(s32 a) {
    s32 i = func_80047B80();

    if (i != -1) {
        func_80047A68(i, a);
    }
    return i;
}

s32 func_80047BC0(s32);
s32 func_80047C8C(s32);
void func_80047C00(void);
u8 *func_80047A44(s32);

/* The first call's result goes into `r` and `r` is reused for the retry: with
   the test written as `func_80047BC0(i) != -1` cse notices the value IS -1 on
   the other arm and reuses that pseudo as the constant, where the target keeps
   -1 in $s1 for both comparisons (28 words of 27). */
u8 *func_80047CD8(s32 k) {
    s16 i = k;
    s32 r = func_80047BC0(i);

    if (r != -1) {
        return 0;
    }
    r = func_80047C8C(i);
    if (r == -1) {
        func_80047C00();
        r = func_80047C8C(i);
    }
    return func_80047A44(r);
}

u8 *func_80047D44(u16 a) {
    func_800479D4(D_80016000, D_80023000, a);
    func_8003F320();
    D_8008152Cs = func_80047AB0(D_80016000, 0xB, 0x7900, 0xD, D_80023000);
    /* The STORE is $gp-relative and the address materialisation is not --
       maspsx rewrites loads and stores only. */
    return (u8 *) &D_8008152Cs;
}

void func_800479D4(u8 *, u8 *, u16);

u8 *func_80047DB8(s32 a) {
    func_800479D4(D_80016000, D_80023000, a);
    /* `&D_8008152Cs`, a 4-byte scalar, and not the array D_8008152C: a small
       object's address is the one-insn `la` macro, and the target keeps the
       lui and the addiu adjacent and ahead of the epilogue. Through the
       incomplete array the compiler splits them and sinks the addiu past the
       `lw $ra`, which also lets the stack adjustment into the delay slot --
       5 DIFF. */
    return (u8 *) &D_8008152Cs;
}

u32 func_80047AB0(u8 *, s32, s32, s32, u8 *);

void func_80047DF0(void) {
    D_8008152Cs = func_80047AB0(D_80016000, 0xB, 0x7900, 0xD, D_80023000);
}

void func_80046F88(u8 *);
void func_80047054(u8 *, s32, s32, s32);

void func_80047E30(void) {
    func_80046F88(D_80016000);
    func_80047054(D_80016000, 0xB, 0x7900, 0xD);
    D_8008152Cs = (u32) D_80016000;
}

extern u32 D_80081528s __asm__("D_80081528");

u8 *func_80047E78(u16 a) {
    func_800479D4(D_80119000, D_80126000, a);
    func_8003F320();
    D_80081528s = func_80047AB0(D_80119000, 0x5, 0x7F80, 0xF, D_80126000);
    return (u8 *) &D_80081528s;
}

extern u8 D_8014A000[];

/* `s32 n` and not `s16`: the listing has no sll/sra pair for it, and the three
   landed callers' prototype already says s32. */
void func_80047EEC(s32 n, u8 *p) {
    s32 i = func_80047C60(p);

    D_80083D08s[i].unk_00 = (u32) p;
    D_80083D08s[i].unk_06 = n;
    D_80083D08s[i + 1].unk_00 = (u32) p;
    D_80083D08s[i + 1].unk_06 = n;
    D_80083D08s[i + 2].unk_00 = (u32) p;
    D_80083D08s[i + 2].unk_06 = n;
    func_80047AB0(p, i + 5, (i * 3 + 0x1D2) << 6, 0xD, D_8014A000);
}

u8 *func_80047CD8(s32);
void func_80047EEC(s32, u8 *);
void func_800481E0(void);
void func_80047C00(void);
void func_8003F320(void);

void func_80047F90(s16 a) {
    u8 *r;

    func_800481E0();
    func_80047C00();
    r = func_80047CD8(a);
    if (r != 0) {
        func_8003F320();
        func_80047EEC(a, r);
    }
}

s32 func_80047C60(u8 *);
u32 func_80047AB0(u8 *, s32, s32, s32, u8 *);

/* The two slot stores go through D_80083D08s DIRECTLY, not through a pointer
   local: as COMPONENT_REFs on the array they carry MEM_IN_STRUCT_P and sched1
   leaves them after the whole argument chain, where through a local they float
   up ahead of it (12-14 DIFF, same word count). */
void func_80047FF4(s32 k, u8 *p) {
    s32 i;

    if (k == 0x38) {
        func_80047EEC(0x38, p);
        return;
    }
    i = func_80047C60(p);
    D_80083D08s[i].unk_00 = (u32) p;
    D_80083D08s[i].unk_06 = k;
    func_80047AB0(p, i + 5, (i * 3 + 0x1D2) << 6, 0xD, D_80080A90);
}

void func_8003F320(void);
void func_80047FF4(s32, u8 *);

/* The 0x38 in the second test is the same literal as the first, which is what
   puts it in a callee-saved register across the calls -- and func_80047EEC
   gets the LITERAL, not `k`, which is why its $a0 is materialised again where
   the other arm has `k` already in the delay slot. */
void func_80048088(s16 k) {
    u8 *r;

    if (k == 0x38) {
        func_800481E0();
        func_80047C00();
    }
    r = func_80047CD8(k);
    if (r != 0) {
        func_8003F320();
        if (k == 0x38) {
            func_80047EEC(0x38, r);
        } else {
            func_80047FF4(k, r);
        }
    }
}

u8 *func_80047CD8(s32);
void func_800499DC(s32 *);

u8 *func_80048118(s16 k, s32 *out) {
    u8 *r;

    if (k == 0x38) {
        func_800481E0();
        func_80047C00();
    }
    r = func_80047CD8(k);
    if (r != 0) {
        *out = 0;
        func_8003E4FC(0xFF, (u8 *) func_800499DC, (s32) out);
    }
    return r;
}

s32 func_80047BC0(s32);

void func_80048190(s16 k) {
    s32 i = func_80047BC0(k);

    if (i != -1) {
        D_80083D08s[i].unk_04 |= 1;
    }
}

void func_800481E0(void) {
    s32 i = 0;

    /* Indexed through the struct array, not a walked `u8 *`: two accesses at
       different offsets off a walked pointer become TWO induction variables
       (one for +0, one for +4), where the struct's COMPONENT_REFs leave
       loop.c one pointer with both offsets as displacements. 18 DIFF. */
    for (; i < 6; i++) {
        if (D_80083D08s[i].unk_00 != 0) {
            D_80083D08s[i].unk_04 |= 1;
        }
    }
}

s32 func_80047BC0(s32);

s32 func_80048224(s16 k) {
    s32 i = func_80047BC0(k);

    if (i != -1) {
        D_80083D08s[i].unk_04 &= 0xFFFE;
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_8004827C);

void func_80047468(Unk800777AC *, u32, u32);
void func_80047338(Unk800777AC *, u32, u32);

/* func_800488F0's walk with a flag test instead of two. D_80082E60 is read as
   a STRUCT FIELD, not through a pointer local: the local makes loop.c hoist
   the constant 1 of the comparison as well as the address, which costs a
   fifth callee-saved register and three words. */
void func_800482C8(Unk800777AC *p, u32 *q) {
    u32 *r = (u32 *) p;
    s32 t;

    do {
        if ((p->unk_00 & 0x60) == 0) {
            r--;
            if (D_80082E60.unk_0A == 1) {
                func_80047468(p, *q, *r);
            } else {
                func_80047338(p, *q, *r);
            }
        }
        t = p->unk_00 & 0x80;
        p++;
    } while (t == 0);
}

void func_80047784(u8 *, s32, s32);

void func_80048374(u8 *p, s16 b, s16 c) {
    *(u16 *) (p + 0x14) |= 0x400;
    func_80047784(p, b, c);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_800483AC);

void func_80047200(u32 *, s32, s32);

/* The VRAM read-back: a four-halfword RECT on the stack ahead of the 0x20-byte
   buffer StoreImage fills. */
void func_8004846C(s32 k) {
    s16 r[4];
    u32 buf[8];

    r[0] = (k & 0x3F) << 4;
    r[1] = k >> 6;
    r[2] = 0x10;
    r[3] = 1;
    StoreImage(r, buf);
    DrawSync(0);
    func_80047200(buf, 1, 0);
    func_8003F80C((u32) buf, k - 0x80, 1, 2);
    DrawSync(0);
}

void func_8004846C(s32);

void func_80048500(s32 a, s32 b) {
    /* s32, not s16: as an s16 local the halfword comes out `lhu` plus an
       sll/sra pair at the call instead of the target's single `lh`. */
    s32 v;

    func_80041284((Unk41284 *) (D_800710E4[b] + a * 8));
    func_8003F320();
    v = *(s16 *) (D_80081480 + 0x18);
    DrawSync(0);
    func_8004846C(v);
}

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

/* The triple dereference is written out again after the if rather than reusing
   `t`, because the arms leave cse nothing in common -- and it is read into a
   local BEFORE `e -= 8`, so the join starts with a load. With the subtraction
   first, reorg copies it into the `j`'s delay slot and the function carries two
   of them (18 DIFF). */
u8 *func_800485E0(u8 *p, s16 i, s16 x, s16 y) {
    u8 *t = *(u8 **) *(u8 **) (p + 0x28);
    u8 *e;
    u32 v;

    if ((u32) (i + 1) < *(u32 *) (t + 4)) {
        e = *(u8 **) (*(u8 **) t + i * 4 + 4);
    } else {
        e = *(u8 **) (t + 8);
    }
    v = *(u32 *) (*(u8 **) *(u8 **) *(u8 **) (p + 0x28) + x * 4);
    e -= 8;
    *(s16 *) (e + 2) = 1;
    *(u32 *) (e + 4) = v + y * 8;
    return e;
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048660);

/* The RLE expander: the top two bits of each byte are a repeat count and the
   low six the value, and a byte of 1 twice over ends it.

   `n` and `k` are two variables with the MASK BETWEEN them, which is what
   keeps the `addu $v1, $v0, $zero` copy the target spends: the counter is
   written in place by `k--`, so the copy has two sets and combine cannot
   substitute it away -- but only in this order. Shifting straight into the
   counter (`k = c >> 6;`) is one word short, and so is masking into a separate
   variable; `k = n;` written before the mask folds the copy away again. */
void func_80048734(u8 *src, u8 *dst) {
    u32 c;
    s32 n;
    s32 k;

    while (c = *src, c != 1 || src[1] != c) {
        n = c >> 6;
        c &= 0x3F;
        k = n;
        while (k != -1) {
            *dst++ = c;
            k--;
        }
        src++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_8004878C);

void func_8004878C(u32, s16 *);

/* `(p[0xA] + 1) >> 1` is an `sra` (the sum is an int) where `p[8] >> 1` is an
   `srl` (a u8 stays unsigned). */
void func_80048870(u8 *p, u8 *q, s32 c) {
    s16 t[4];

    t[2] = (p[0xA] + 1) >> 1;
    t[3] = p[0xB] + 1;
    t[0] = (p[8] >> 1) + ((*(u16 *) (p + 4) << 6) & 0x3C0);
    t[1] = p[9] + ((*(u16 *) (p + 4) << 4) & 0x100);
    func_8004878C(*(u32 *) (q + 0xC) + c, t);
}

void func_80048660(u8 *, u8 *);

void func_800488F0(u32 a, u32 b, s32 f) {
    u8 *p = (u8 *) a;
    u8 **q = (u8 **) b;
    u8 *r = p;
    s32 c = f & 2;
    s32 d = f & 1;
    s32 t;

    do {
        if ((*p & 0x60) == 0) {
            r -= 4;
            if (c != 0) {
                func_80048660(p, *q);
            }
            if (d != 0) {
                func_80048870(p, *q, *(s32 *) r);
            }
        }
        t = *p & 0x80;
        p += 0xC;
    } while (t == 0);
}

void func_800488F0(u32, u32, s32);

void func_80048998(u8 *p, s32 n) {
    if (n != 0) {
        /* `n -= 1` as its own statement: written `D_80083110[n - 1]` the -1
           folds into the base (`addiu %lo(D_80083110)-4`) and the function is
           a word short. */
        n -= 1;
        D_80083110[n] = (u32) p;
        func_800488F0(*(u32 *) (p + 0x8), *(u32 *) (p + 0x28), 2);
    } else {
        func_800488F0(*(u32 *) (p + 0x8), *(u32 *) (p + 0x28), 3);
    }
}

void func_800489F4(u8 *p, s16 a, s16 b) {
    u8 *q = *(u8 **) (*(u8 **) *(u8 **) *(u8 **) (p + 0x28) + a * 4) + b * 8;
    u32 v;

    *(u8 **) p = q;
    v = *(u32 *) (q + 4);
    p[4] = b;
    *(u16 *) (p + 0x14) = (*(u16 *) (p + 0x14) & 0xFFBF) | 0x20;
    *(u32 *) (p + 8) = v;
}

void func_800489F4(u8 *, s16, s16);
void func_80048998(u8 *, s32);

void func_80048A44(u8 *p, s32 b, s32 c, s32 d) {
    s32 v;

    func_800489F4(p, (s16) b, (s16) c);
    func_80048998(p, d);
    v = **(u16 **) p / D_80080A84;
    *(u16 *) (p + 0x14) &= 0x9FFF;
    p[5] = v;
}

void func_80048998(u8 *, s32);

void func_80048AC8(u8 *p, s32 b) {
    func_800477F4((UnkSlot *) p, b);
    if ((*(u16 *) (p + 0x14) & 0x1220) != 0) {
        func_80048998(p, b);
        *(u16 *) (p + 0x14) &= 0xFFDF;
    }
}

void func_800488F0(u32, u32, s32);

void func_80048B28(void) {
    u8 **p = (u8 **) D_80083110;
    s32 i = 4;

    do {
        if (*p != 0) {
            func_800488F0(*(u32 *) (*p + 0x8), *(u32 *) (*p + 0x28), 1);
            *p = 0;
        }
        i--;
        p++;
    } while (i > 0);
}

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048B8C);

INCLUDE_ASM("asm/nonmatchings/unk_1AF38", func_80048C3C);

void func_80048C3C(s32);

void func_80048D00(void) {
    func_80048C3C(0);
}
