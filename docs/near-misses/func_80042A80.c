/* func_80042A80 (unk_14934) -- one bit off. 58 words against 58, every
 * instruction identical except the last: the target ends the default arm with
 * `sra $v0, $v0, 16` and this ends it with `srl $v0, $v0, 16`.
 *
 * Both are the second half of a 16-bit truncation. gcc expands a cast to s16
 * as `sll 16` + `sra 16`, and combine's simplify_shift_const downgrades the
 * `sra` to `srl` whenever it can prove bit 15 of the operand is clear. The
 * `andi $v0, $v0, 0x400` immediately before it proves exactly that, so every
 * spelling of the cast gets `srl`.
 *
 * Measured, all `andi 0x400; sll 16; srl 16`: `(s16) (x & 0x400)`;
 * `s16 v = x & 0x400; return v;`; `s32 v = x & 0x400; return (s16) v;`;
 * `u16 m = x & 0x400; return (s16) m;`; `u16 m = x; return (s16) (m & 0x400);`;
 * `return *(s16 *) q & 0x400;`; `((s32) (x & 0x400) << 16) >> 16`;
 * a union with s16/u16 members; `*(s16 *) &m` on a u16 holding the masked
 * value; a vu16 (volatile) load; the same inside an `if` with the value
 * initialised to 0 beforehand; and a function returning s16 instead of s32.
 * For contrast, `andi 0xC000` and `ori 0x400` both do give `sra`, which
 * confirms the mechanism is the proof and not the spelling.
 *
 * So the mask cannot be visible to combine where the extension is expanded.
 * combine.c's nonzero_bits REG case (gcc 2.7.2, around line 6887) consults
 * reg_last_set_nonzero_bits only when `reg_last_set_mode == mode` and
 * (`reg_n_sets == 1` or `reg_last_set_label == label_tick`), so the way out is
 * a value with more than one set whose masking set is in a *different* basic
 * block from the extension, or a recorded mode that differs from the one
 * simplify_shift_const asks about. No natural source shape found yet puts a
 * block boundary between the `andi` and the shifts while still emitting them
 * adjacently, which is what the target shows.
 *
 * Everything else here is settled and worth keeping: D_80082E6A has to be read
 * as D_80082E60.unk_0A (the scalar declaration gives the assembler's
 * same-register form, the target has the compiler split), and the dispatch has
 * to be an if-chain, not a switch -- a switch on 4/0xF/0x10/0x11 makes gcc
 * build a balanced tree of `slti`s instead of the target's `== 4` followed by
 * `addiu -0xF` / `sltiu 3`.
 *
 * D_80073414 is a 0x14-byte-stride table; D_80073470 is &D_80073414[4].unk_0C,
 * so the `case 4` arm needs no separate symbol.
 */

#include "common.h"

typedef struct Unk80073414 {
    u8  unk_00;
    u8  pad_01[0xB];
    u8 *unk_0C;
    u8  pad_10[0x4];
} Unk80073414;

extern Unk80073414 D_80073414[];

s32 func_80042A80(u8 *p) {
    if (D_80082E60.unk_0A == 2) {
        u8 k = p[1];

        if (k == 4) {
            s32 r = ((*(u16 *) (D_80073414[4].unk_0C + p[0] * 0x14) & 0x400) != 0) << 10;

            if (p[3] & 0x80) {
                r |= 0x80;
            }
            return r;
        }
        if (k >= 0xF && k <= 0x11) {
            return p[3] & 0x80;
        }
        if (D_80073414[p[1]].unk_00 == 0) {
            return (s16) (*(u16 *) (D_80073414[p[1]].unk_0C + p[0] * 0x14) & 0x400);
        }
    }
    return 0;
}
