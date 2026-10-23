/* The remainder of unk_11940 after the gprel_11B4C carve-out. Named for its
 * start offset. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

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
