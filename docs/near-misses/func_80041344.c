/* func_80041344 (unk_14934) -- 145/145 words, 67 DIFF, and BLOCKED on a spec
 * change rather than on a source shape. Not compiled; see
 * docs/open-matching-problems.md.
 *
 * This is a 9-case dense switch and the target dispatches through
 * jtbl_8002D630, which lives inside the OPENING `[0x800, bin]` subsegment
 * (vram 0x8002D000..0x80032E60) -- data that sits before all the code in the
 * image. Compiling a switch does emit gcc's own table into .rdata, with
 * exactly nine .words in exactly the target's order, but
 * build/slus_006.14.ld collects every .rodata at _800_RODATA_START, far past
 * .text, so the table lands nowhere near 0x8002D630 and the image shifts.
 * verify.py cannot see this: it masks the two relocated halfwords that
 * reference the table. Only `make check` can.
 *
 * Landing it needs the table carved out of the opening bin blob as a .rodata
 * subsegment belonging to unk_14934, and the rodata collection moved to the
 * front of the linker script. jtbl_8002D5C0, jtbl_8002D658 and jtbl_8002D67C
 * sit in the same blob, so the change unblocks more than one function.
 *
 * What is left in the source, once that is fixed: case 8's induction-variable
 * structure (about 10 words) and one coupled register permutation. The chain
 * starts at the switch index (`op - 1`), which is block-local so LOCAL-alloc
 * assigns it before global-alloc runs: it takes $a0 here and $v1 in the
 * target. Once $v1 is occupied, `e` (26 refs / 30 insns, priority 3.47) can no
 * longer take it, and because cases 7 and 8 set their counters before `e`'s
 * last use those argument registers conflict too, so `e` falls to $t0 as the
 * target has it. No source lever was found that moves local-alloc's choice.
 * The global side is reachable in principle -- lowering `e`'s priority below
 * 3.0 needs its live length >= 35 or its refs <= 15 -- but `e`'s 13
 * references times loop depth 2 are all forced by the target's own
 * lw 0x4($t0)/0x8/0xC loads, and the outer loop cannot become a goto loop:
 * the hoisted $s3 table base proves loop-invariant motion ran, so the loop
 * note has to be there.
 *
 * Established and worth not re-deriving:
 *  - The case order in the source must be 1, 5, 2, 6, 7, 8, 3, 4, 9. gcc emits
 *    arm bodies in source order; the real table was read out of
 *    data/slus_006.14 at file offset 0xE30: 800413a0 800413dc 800414dc
 *    80041500 800413c4 800413f4 80041410 80041460 80041528. In numerical order
 *    the arms land in the wrong places.
 *  - `default: return;` -- the out-of-range path goes straight to the epilogue
 *    and must NOT zero D_80080A8C, unlike the size == 0 exit.
 *  - A separate `Unk41344 *e` inside the loop body is required, and the
 *    apparently redundant `addu $t0, $s0, $zero` is real: cse's copy
 *    propagation works on extended basic blocks and cannot cross the `jr`, so
 *    the copy survives. Dropping `e` gives 144 words.
 *  - Case 9's guard is `blez`, not `beqz`, which falls out of
 *    `for (i = *(u16 *) (p + 8); i > 0; i--)`: `i` has two sets, so combine's
 *    nonzero_bits REG case refuses to use the lhu's known 0xFFFF and cannot
 *    narrow `i > 0` to `i != 0`. A single-assignment local loses it.
 *  - Case 9's body really does not advance its pointer -- `r[1] = (u32)(base +
 *    r[1])` with `r` loop-invariant, repeated n times. That is what the target
 *    does, off an unchanging $v1. It looks like a bug in the original.
 *
 * Ruled out for case 8: `q[j * 16 + i]` (147 words -- gcc rebuilds the index
 * in the loop instead of strength-reducing), `q += 16` with `q[i]` (144), and
 * the `q++` pointer walks (145 words / 70 DIFF -- the right single-pointer
 * shape, but gcc coalesces the walker with the func_8003F80C argument and pays
 * an `addiu $a0,$a0,-2` fix-up; adding a second base local folds the copy away
 * again and gives 144). No effect at all, all still 67: the opcode in a u16 or
 * s32 local, `switch (e->unk_00)`, `e` hoisted above the loop,
 * declaration-order swaps, `size` as s32, `e` as `u32 *` with e[1]/e[2]/e[3],
 * and the walker as a halfword-field struct.
 *
 * structs.h wants Unk41344 (u16 unk_00 opcode, u16 unk_02 byte size, then
 * u32 unk_04/unk_08/unk_0C, read either as words or as halfwords, which is
 * why the function keeps two pointers to the same record), the forward
 * declaration in csrc/unk_14934.c corrected to
 * `void func_80041344(u8 *, u32 *);`, and `void *memcpy(void *, const void *,
 * u32);` next to bzero.
 */

#define func_80041344 func_80041344_decl_clash
#include "common.h"
#undef func_80041344

typedef struct Unk41344 {
    u16 unk_00;
    u16 unk_02;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
} Unk41344;

void *memcpy(void *, const void *, u32);

void func_80041344(u8 *base, u32 *arg1) {
    u8 *p = base;
    u16 size;

    for (;;) {
        Unk41344 *e = (Unk41344 *) p;

        switch (*(u16 *) p) {
        case 1:
            func_8004068C(base + e->unk_04, (u8 *) arg1);
            LoadImage(p + 8, arg1);
            DrawSync(0);
            break;
        case 5:
            LoadImage(p + 8, (u32 *) (base + e->unk_04));
            DrawSync(0);
            break;
        case 2:
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), *(s16 *) (p + 0xC));
            DrawSync(0);
            break;
        case 6:
            func_8003F80C((u32) (base + e->unk_04), *(u16 *) (p + 8),
                          *(u16 *) (p + 0xA), (s16) (*(u16 *) (p + 0xC) | 2));
            DrawSync(0);
            break;
        case 7: {
            u16 *q = (u16 *) (base + e->unk_04);
            s32 n = *(u16 *) (p + 0xA) << 4;
            s32 i;

            for (i = 1; i < n; i++) {
                q[i] |= 0x8000;
            }
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) (*(u16 *) (p + 0xC) | 4));
            DrawSync(0);
            break;
        }
        case 8: {
            u16 (*q)[16] = (u16 (*)[16]) (base + e->unk_04);
            s32 n = *(u16 *) (p + 0xA);
            s32 i;
            s32 j;

            for (j = 0; j < n; j++) {
                for (i = 1; i < 16; i++) {
                    q[j][i] |= 0x8000;
                }
            }
            func_8003F80C((u32) q, *(u16 *) (p + 8), *(u16 *) (p + 0xA),
                          (s16) (*(u16 *) (p + 0xC) | 2));
            DrawSync(0);
            break;
        }
        case 3:
            func_8004068C(base + e->unk_04, (u8 *) (D_80080A8C + e->unk_08));
            break;
        case 4:
            memcpy((void *) D_80080A8C, base + e->unk_04, e->unk_0C + e->unk_08);
            break;
        case 9: {
            u32 *r = (u32 *) (base + e->unk_04);
            s32 i;

            for (i = *(u16 *) (p + 8); i > 0; i--) {
                r[1] = (u32) (base + r[1]);
            }
            break;
        }
        default:
            return;
        }
        size = *(u16 *) (p + 2);
        if (size == 0) {
            break;
        }
        p += size;
    }
    D_80080A8C = 0;
}
