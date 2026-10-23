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
 * The obvious reading of combine.c's nonzero_bits REG case (gcc 2.7.2, around
 * line 6887) is that it consults reg_last_set_nonzero_bits only when
 * `reg_last_set_mode == mode` and (`reg_n_sets == 1` or
 * `reg_last_set_label == label_tick`), which would make "the masking set in a
 * different basic block from the extension" the way out.
 *
 * That reading is WRONG, and it has now been measured. Four probe functions
 * compiled with the real ccpsx (2026-10-22):
 *
 *   s32 A(u16 *p)            { return (s16) (*p & 0x400); }
 *   s32 B(u16 *p, s32 *q)    { s32 v = *p & 0x400; *q = v; return (s16) v; }
 *   s32 C(u16 *p, s32 c)     { s32 v = 0; if (c) v = *p & 0x400; return (s16) v; }
 *   s32 D(s32 v)             { return (s16) v; }
 *
 * A gives `andi 0x400; sll 16; srl 16`. B gives `andi 0x400; andi 0xffff; sw;
 * sll 16; srl 16` -- a second use does not stop the downgrade, it just adds a
 * truncation. C puts the mask in its own basic block, with the shifts in the
 * join block and a `v = 0` predecessor, and STILL gives `srl`, with the same
 * extra `andi 0xffff`. Only D, whose operand is wholly opaque, gives `sra`.
 *
 * Four more probes (same day) confirm it from the other direction: the masked
 * value returned on one path and cast on another; the loaded halfword stored
 * before the mask; and the masked value tested against 0 before the cast --
 * all still `srl`, and the last two also carry the spurious `andi 0xffff`.
 * Worth noting because the matching trick for the BYTE case is exactly this
 * one: a second use of a loaded byte does block the `lb` fold (see
 * docs/near-misses/func_8003D0F0.c). It does not transfer here, so the two
 * are separate mechanisms -- combine substituting an insn versus combine
 * proving a bit -- and only the first is reachable from C.
 *
 * NARROWED 2026-10-22, and `sra` IS reachable -- just not at 58 words.
 * `nonzero_bits` gives up on a value with more than one SET unless every set
 * is a known constant with bit 15 clear, so the way to keep the `sra` is a
 * second set that is opaque, or a constant with bit 15 set:
 *
 *   s32 G(u16 *p, s32 c)         { s32 v = 0;      if (c) v = *p & 0x400; return (s16) v; }  -> srl
 *   s32 E(u16 *p, s32 c, s32 w)  { s32 v = w;      if (c) v = *p & 0x400; return (s16) v; }  -> sra
 *   s32 F(u16 *p, s32 c, s32 *q) { s32 v = *q;     if (c) v = *p & 0x400; return (s16) v; }  -> sra
 *   s32 H(u16 *p, s32 c)         { s32 v = 0x8000; if (c) v = *p & 0x400; return (s16) v; }  -> sra
 *
 * G is the probe this file already recorded, and it is the odd one out BECAUSE
 * its other set is a known 0: the union still proves the bit clear. That is
 * the detail the earlier write-up missed when it concluded a block boundary
 * does not help -- it was not the boundary that failed, it was the value 0.
 *
 * It does not land this function, though, and the reason is exact: every
 * multi-set shape also emits an `andi 0xffff`, because combine cannot narrow
 * both sets to HImode and inserts the truncation on one path instead. The
 * target is 58 words against 58, with no truncation insn:
 *
 *     lhu $v0,0x0($v0) / nop / andi $v0,$v0,0x400 / sll $v0,$v0,16 / j / sra
 *
 * so the target's value is SINGLE-SET and the proof was defeated some other
 * way. Note also that the other three arms do not sign-extend at all (the
 * k == 4 arm ends `sll 10`/`ori 0x80`, the 0xF..0x11 arm `andi 0x80`), so
 * there is no shared `s16` result variable to make multi-set -- each arm writes
 * $v0 directly and jumps to the bare `jr`.
 *
 * So neither a second use nor a block boundary defeats the proof, and the
 * extra `andi 0xffff` that a second use, a block boundary and the multi-set
 * shape all introduce is not in the target anyway.
 * Whatever the original wrote, the value reaching the extension was opaque to
 * combine -- which is hard to square with the `andi 0x400` sitting immediately
 * before it. The remaining possibilities are that the extension is not a cast
 * of the masked value at all, or that something about the surrounding function
 * (not this expression) changes what combine has recorded. The same question
 * blocks func_8003D0F0's four `(s8)` conversions, so it is worth more than one
 * function.
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
 *
 * SEARCHER SWEEP (2026-10-22): tools/decomp/search.py compiled 29 variants of
 * this source across its whole transform set -- declared widths, (s16) casts,
 * splitting and joining a statement, `<` against `>`, the `& 1` mask -- and
 * none improved on 1 DIFF. **Do not re-sweep local shapes here.** The remaining
 * gap is combine proving bit 15 clear on a single-set value, where every multi-set shape costs an `andi 0xffff` this function has no room for,
 * which no local rewrite reaches.
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
