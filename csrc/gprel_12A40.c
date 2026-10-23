/* A carved-out translation unit of ONE function: it reaches D_800814D0
 * $gp-relatively, so gas is left at -G8 here rather than maspsx's forced -G0
 * (MASPSXFLAGS_G8 in the Makefile).
 *
 * One function alone, and the reason is worth recording. func_8003F270, which
 * immediately follows, ALSO reaches D_800814D0 -- but absolutely, with the
 * lui/%lo pair, and it already matches that way. A $gp-relative reference
 * means the function was compiled in the unit that DEFINED the global, so the
 * two cannot have shared a translation unit: this one defined it, its
 * neighbour did not. The carve therefore stops here even though the next
 * function touches the same object, and D_800814D0 keeps the plain
 * `extern u8 D_800814D0[4]` declaration that func_8003F270 needs.
 *
 * Audited before carving: D_80083968 is an incomplete array of 0x18-byte
 * structs, so cc1psx compiler-splits its address itself and -G8 does not touch
 * it. (At -G8 gas makes any small UNDEFINED symbol $gp-relative, so every
 * absolutely-reached symbol in a carved unit has to be checked.) */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

/* The ring queue's "entry before the read index" peek: (read_idx - 1) & 0x1F,
 * written `+ 0x1F` because the target has addiu 0x1F / andi 0x1F. The
 * sll 1 / addu / sll 3 chain is Unk80083968's 0x18 stride and falls straight
 * out of the existing struct. */
u8 func_8003F240(void) {
    return D_80083968[(D_800814D0[0] + 0x1F) & 0x1F].unk_00;
}
