#include "common.h"

void func_8003D7DC(void);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CAA8);

void func_8003CAC4(void) {
    s32 i;

    for (i = 0x3F; i >= 0; i--) {
        D_80083858[i] = 0x500;
        D_800838D8[i] = 0x380;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CB08);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003CCB0);

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D0F0);

void func_8003D468(void) {
    u8 *base = (u8 *) 0x80010000;
    u8  a = base[0x3186];
    u32 b = *(u32 *) (base + 0x3180);
    u8  c = base[0x3184];
    u8  d = base[0x2D6E];
    u16 e = *(u16 *) (base + 0x2094);
    u16 f = *(u16 *) (base + 0x2098);

    bzero(base, 0x5C84);
    /* Both assignments are byte-aligned block moves: 6 bytes comes out as one
     * lwl/lwr pair plus two lb/sb, 10 bytes as two pairs plus two lb/sb. The
     * cast supplies the size and the alignment; D_80080AB4's own declaration
     * is an incomplete array so that its address is compiler-split. */
    *(Unk80080AB4 *) 0x8001020C = *(Unk80080AB4 *) D_80080AB4;
    *(Unk8002D5B4 *) 0x80010238 = D_8002D5B4;
    base[0x3186] = a;
    *(u32 *) (base + 0x3180) = b;
    base[0x3184] = c;
    base[0x2D6E] = d;
    *(u16 *) (base + 0x2094) = e;
    *(u16 *) (base + 0x2098) = f;
}

/* arg0 is unused; it exists because func_8003D5A4's call site passes 1 and an
   unused register parameter generates no code. */
void func_8003D56C(s32 arg0) {
    func_80069C90();
    func_80069CE8();
    _bu_init();
    ChangeClearPAD(0);
}

/* Boot-time graphics/pad/CD setup.
 *
 * Load-bearing shapes:
 *  - `Unk80083160 *s = &D_80083160;` as the first statement. The whole address
 *    is materialised up front into a callee-saved register and carried across
 *    every call, even though its first use is 30 instructions from the end.
 *    Reaching the fields as `D_80083160.unk_00` instead costs 105 words: it
 *    rebuilds a lui/%lo pair per access.
 *  - but D_80083160.unk_A8 stays a plain symbol reference. Through the pointer
 *    it becomes 0xA8($s0); the target has its own compiler-split lui/%lo pair,
 *    which splat renders as the known label D_80083208 at that address. So the
 *    function really does mix the two spellings for one object, which is
 *    evidence D_80083208 was a separate global in the original. `s->unk_A8`
 *    costs 16 words.
 *  - the two buffer addresses go in as `(u32) D_801D8D7A`, with those symbols
 *    declared as incomplete arrays. The scalar-plus-& spelling emits the same
 *    eight instructions in the same order but with $v0 and $v1 swapped: a
 *    scalar is one `(set reg (symbol_ref))` the assembler expands to `la`,
 *    while the array gives the compiler-split high/lo_sum pair, which adds a
 *    pseudo per value and reshuffles local-alloc.
 *  - D_80082E60.unk_0B, not a scalar D_80082E6B: the struct is 0x19 bytes,
 *    past -G8, so the store is compiler-split where a scalar gets the
 *    assembler-expanded $at form.
 *  - the 0x80010000 accesses use the out-of-image constant-base-pointer idiom,
 *    as func_8003D468 does three functions earlier. */
void func_8003D5A4(void) {
    Unk80083160 *s = &D_80083160;
    u8 *base;
    u8 *p;
    s32 i;

    SetVideoMode(0);
    CdInit();
    func_8003E34C();
    ResetGraph(0);
    func_80053F98();
    func_80053DCC(D_80080A94);
    func_80053DF0(D_80080A98);
    func_80053E14(D_80080A9C);
    for (i = 0; i < 0x22; i++) {
        D_800830C0[i] = 0xFF;
        D_800830E8[i] = 0xFF;
    }
    InitPAD(D_800830C0, 0x22, D_800830E8, 0x22);
    func_8006A444();
    func_8003D56C(1);
    SetGraphDebug(0);
    func_80064688();
    D_80082E60.unk_0B = 0;
    bzero(D_80081468, 8);
    func_8003D468();
    base = (u8 *) 0x80010000;
    *(u32 *) (base + 0x3180) = 0x2A080808;
    D_80081494 = 0x2A080808;
    base[0x3184] = 0x20;
    D_800814AC = 0x20;
    base[0x3186] = 3;
    D_800814A4 = 3;
    *(u16 *) (base + 0x2094) = 5;
    base[0x2D6E] = 0;
    *(u16 *) (base + 0x2098) = 2;
    func_80043674();
    p = D_801C9E40;
    if (s->unk_00 == p) {
        p += 0x108D4;
    }
    s->unk_00 = p;
    *(u8 **) (p + 0x8D0) = p + 0x8D4;
    D_80083160.unk_A8 = 0x2C808080;
    func_8003CAC4();
    D_80081478[0] = (u32) D_801D8D7A;
    D_80081478[1] = (u32) D_801E964E;
}

void func_8003D760(void) {
    /* The rect goes out as two words: four halfword field assignments would
       give four `sh`s, and an aggregate initializer lands in .sdata, which
       this unit's linker script discards. */
    u32 r[2];

    func_8005FFFC();
    func_8004EB30();
    func_8003D7DC();
    r[0] = 0;
    r[1] = 0x2000400;
    ClearImage(r, 0, 0, 0);
    D_8008148C = (u32) &D_80126804;
    D_80081480 = (u32) &D_80126804;
    D_80080A7C = 0x38000;
}

void func_8003D7DC(void) {
    /* The base has to come out as a lo_sum in a register: a constant address
     * or a scalar's `&` folds the +0x5C into a second `la` instead of the
     * target's single `addiu` off the base. */
    u8 *p = D_801C9E40;

    SetDefDrawEnv(p, 0, 0, 0x140, 0xE0);
    SetDefDrawEnv(&D_801DA714, 0, 0xE0, 0x140, 0xE0);
    SetDefDispEnv(p + 0x5C, 0, 0xE0, 0x140, 0xF0);
    SetDefDispEnv(&D_801DA770, 0, 0, 0x140, 0xF0);
    *(s16 *) (p + 0x1093A) = 0xC;
    *(s16 *) (p + 0x66) = 0xC;
    *(s16 *) (p + 0x1093E) = 0xE0;
    *(s16 *) (p + 0x6A) = 0xE0;
    D_801DA72C[0] = 1;
    *(u32 *) (p + 0x18) = 1;
}

void func_8003D8B0(void) {
    s32 i;

    func_8003DB4C((int *) &D_80083120, 0x10);
    func_8003DB4C((int *) &D_80082EC0, 0x80);

    /* Must be an indexed store into a real array: only an array reference
     * forms the address as (lo_sum base) + i*4, which is what leaves the
     * strength-reduced +0xC as its own addiu. A pointer form folds it. */
    for (i = 3; i >= 0; i--) {
        D_80083110[i] = 0;
    }

    D_80013714 = 0;
    func_800499BC();
    func_8004437C(0x2A, 0);
    func_8004425C(0);
}

INCLUDE_ASM("asm/nonmatchings/unk_102A8", func_8003D92C);
