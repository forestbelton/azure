#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

void func_8003E140(s8 *arg0) {
    *arg0 = 1;
}

s32 func_8003E14C(void) {
    s32 v;

    /* The mode byte is a field of the Unk80082E60 struct, not a standalone
     * global: going through the struct is what makes the compiler split the
     * address into a shared lui plus a lbu with a %lo displacement. */
    if (D_80082E60.unk_0A != 2) {
        return 0;
    }

    v = 0x28;
    if (D_8008146C < 0x29) {
        v = D_8008146C;
    }
    return v;
}

void func_8003E188(s16 arg0, s16 arg1) {
    /* Two details are load-bearing, both scheduler tie-breaks. `p` must be a
     * two-set pointer so the `lui` lands first in RTL order and the `addu`
     * writes back into the base register; and `sh` must be assigned in two
     * statements, because gcc's pre-reload scheduler bumps an insn that
     * births a *single*-set pseudo to maximum priority, which sinks the
     * `bits = 1` past the shift and loses it the branch delay slot. */
    u8 *p = (u8 *) 0x80010000;
    s32 sh;
    u8 v;
    s32 bits;

    p += arg0 >> 2;
    sh = arg0 & 3;
    sh *= 2;
    v = p[0x3614];
    bits = 1;
    if (arg1 != 0) {
        bits = 3;
    }
    p[0x3614] = v | (bits << sh);
}

void func_8003E1C4(s16 arg0) {
    func_80041284(D_8006CDC8[arg0]);
}

void func_8003E1FC(void) {
    u8 *p = &D_80080AC0;

    if (D_80082E60.unk_0A == 2) {
        p = &D_80080AC8;
    }
    func_8003E4FC(6, p, 0);
}

u32 func_8003E240(s32 arg0) {
    u32 r = 0xFFFFFFFF;

    if (D_800830C0[0] == 0 && (u32) (D_800830C0[1] - 0x20) < 0x51) {
        /* Assigning the high half into `r` first and then or-ing the pad
         * bytes *into* it is what puts the `lui` ahead of the two loads and
         * in r's own register. */
        r = 0xFFFF0000;
        r = (D_800830C0[3] | (D_800830C0[2] << 8)) | r;
    }
    if (D_800830E8[0] == 0 && (u32) (D_800830E8[1] - 0x20) < 0x51) {
        r &= 0xFFFF;
        r |= (D_800830E8[3] | (D_800830E8[2] << 8)) << 16;
    }
    return ~r;
}

void func_8003E2D8(void) {
    /* The struct pointer must be the primary base and the word pointer
       derived from it: deriving the other way round gives `addu $s0,$s1,-8`
       where the target has `addiu $s1,$s0,8`. */
    Unk80083160 *s = &D_80083160;
    u32 *q = &s->unk_08;

    q[1] = s->unk_08;
    s->unk_08 = func_8003E240(0);
    if ((s->unk_08 & 0xF000) == 0xF000) {
        s->unk_08 = 0;
    }
    q[2] = (s->unk_08 ^ q[1]) & s->unk_08;
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E34C);

/* Pushes an entry onto the 32-slot ring queue at D_80083968, spinning on
 * func_8003E758 while the queue is full, and returns the slot it filled.
 *
 * Arm 1's store has to be byte-pointer arithmetic, not
 * `D_80083968[i].unk_08 = (s32) arg2;`. As a struct-field store, cse folds the
 * address onto the entry pointer already materialised for unk_00/unk_04/unk_17
 * and emits a bare `sw $s1, 0x8($v1)` -- one word short. The target recomputes
 * it as `addu $v0, $a2, $a0`, with the base and the scaled index in the
 * opposite operand order from the `addu $v1, $a0, $a2` that built the entry
 * pointer; a different rtx, so cse misses it, and the byte-pointer spelling is
 * what produces that order. Correspondingly the other three field stores must
 * stay plain array references and arm 3 must take `&D_80083968[i].unk_08`:
 * hoisting the common fields into a pointer local inverts the problem and
 * costs 57 words.
 *
 * arg0 is a u8, so the raw register is stored with `sb` while the == 0xFF test
 * needs its own `andi`, and that 0xFF then shares a register with the
 * unk_17 = 0xFF store.
 *
 * The loop head's shape falls out of D_800814D0's existing complete-array
 * declaration: inside -G8 it gets the assembler-expanded symbolic form, gcc
 * materialises &D_800814D0[1] once with an `la` so it can keep it across the
 * call, and reads [0] as -0x1 off that. */
Unk80083968 *func_8003E39C(u8 arg0, s32 arg1, void *arg2) {
    s32 i;

    while (((D_800814D0[1] + 1) & 0x1F) == D_800814D0[0]) {
        func_8003E758();
    }
    i = D_800814D0[1];
    D_80083968[i].unk_00 = arg0;
    D_80083968[i].unk_04 = arg1;
    D_80083968[i].unk_17 = 0;
    if (arg0 == 0xFF) {
        *(void **) ((u8 *) D_80083968 + i * 0x18 + 8) = arg2;
    } else if (arg2 == (void *) 1) {
        D_80083968[i].unk_17 = 0xFF;
    } else if (arg2 != NULL) {
        *(Unk3E39C *) &D_80083968[i].unk_08 = *(Unk3E39C *) arg2;
    }
    D_800814D0[1] = (D_800814D0[1] + 1) & 0x1F;
    return &D_80083968[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E4FC);

void func_8003E70C(void) {
    D_80083958.unk_08 = D_80083958.unk_08 + 1;
    D_80083958.unk_06 = D_80083958.unk_06 + 1;
    if (D_80083958.unk_06 > 0x400) {
        D_80083958.unk_06 = 0;
        CdInit();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003E758);

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F240);

s32 func_8003F270(void) {
    /* The target materialises the address of element 1 and reaches element 0
     * through it with a -1 displacement, and reloads element 0 for the
     * subtraction rather than keeping it live; the separate `t` is what stops
     * the second read being held in a register. */
    u8 *p = &D_800814D0[1];
    u32 v = D_800814D0[1];
    u8 t = p[-1];

    if (v < t) {
        v += 0x20;
    }
    return v - p[-1];
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F2A4);

void func_8003F320(void) {
    while (func_8003F270() != 0) {
        func_8003CB08();
        func_80044618(2);
        func_8003E758();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F368);

s32 func_8003F52C(s32 arg0) {
    return arg0;
}

Unk8006CE34 *func_8003F534(void) {
    return &D_8006CE34;
}

void func_8003F540(u32 arg0, u32 arg1, u32 arg2, u32 arg3) {
    D_8006CE34.unk_00 = arg0;
    D_8006CE34.unk_04 = arg1;
    D_8006CE34.unk_08 = arg2;
    D_8006CE34.unk_0C = arg3;
}

s32 func_8003F55C(void) {
    if (CdSync(1, NULL) == 2) {
        CdControl(0x10, NULL, D_800814B8);
    }
    return CdPosToInt(D_800814B8);
}

INCLUDE_ASM("asm/nonmatchings/unk_11940", func_8003F5AC);
