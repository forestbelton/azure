/* A carved-out translation unit of ONE function: it reaches D_800814D0
 * $gp-relatively, which is the same as saying this unit DEFINED it. The
 * tentative definition below is that statement, and all this unit needs:
 * cc1psx emits `.comm`, maspsx's `--use-comm-section` files it under
 * `sbss_entries`, and `_uses_gp` rewrites this unit's loads and stores of the
 * symbol to %gp_rel at the project default gas -G0. Nothing is allocated --
 * the object is already a GLOBAL dlabel in asm/data/gp_data.data.s and a C
 * tentative definition is a COMMON, which merges with it. See
 * docs/addressing-and-translation-units.md.
 *
 * One function alone, and the reason is worth recording. func_8003F270, which
 * immediately follows, ALSO reaches D_800814D0 -- but absolutely, with the
 * lui/%lo pair, and it already matches that way. A $gp-relative reference
 * means the function was compiled in the unit that DEFINED the global, so the
 * two cannot have shared a translation unit: this one defined it, its
 * neighbour did not. The carve therefore stops here even though the next
 * function touches the same object. The definition here is file-scope, so it
 * covers only this unit; structs.h keeps the plain `extern u8 D_800814D0[4]`
 * that func_8003F270 needs, and that unit stays absolute precisely because it
 * does not define the object.
 *
 * Audited back when this unit was built at -G8: D_80083968 is an incomplete
 * array of 0x18-byte structs, so cc1psx compiler-splits its address itself and
 * -G8 did not touch it. That audit is moot now -- at -G0 gas makes no
 * small-data decision of its own -- but the finding is kept as evidence that
 * the carve is right. */
#include "common.h"

s32 func_8003F270(void);
void func_8003E758(void);
u32 func_8003E240(s32);

u8 D_800814D0[4];

/* The ring queue's "entry before the read index" peek: (read_idx - 1) & 0x1F,
 * written `+ 0x1F` because the target has addiu 0x1F / andi 0x1F. The
 * sll 1 / addu / sll 3 chain is Unk80083968's 0x18 stride and falls straight
 * out of the existing struct. */
u8 func_8003F240(void) {
    return D_80083968[(D_800814D0[0] + 0x1F) & 0x1F].unk_00;
}
