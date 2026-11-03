/* A carved-out translation unit: these functions reach D_800814C8
 * $gp-relatively, which is the same as saying this unit DEFINED it. The
 * tentative definition below is that statement, and all this unit needs:
 * cc1psx emits `.comm`, maspsx's `--use-comm-section` files it under
 * `sbss_entries`, and `_uses_gp` rewrites this unit's loads and stores of the
 * symbol to %gp_rel at the project default gas -G0. Nothing is allocated --
 * the object is already a GLOBAL dlabel in asm/data/gp_data.data.s and a C
 * tentative definition is a COMMON, which merges with it. See
 * docs/addressing-and-translation-units.md.
 *
 * func_8003F5E0 was the first function of unk_12DEC until the boundary was
 * moved to take it in. The two belong together on the evidence: one reads
 * D_800814C8 and the other writes it, $gp-relatively, and a $gp-relative
 * reference means the function was compiled in the unit that defined the
 * global. The unit keeps its name because its START is unchanged; what moved
 * is where the next unit begins.
 */
#include "common.h"

u32 D_800814C8;

u32 func_8003F5D4(void) {
    return D_800814C8;
}

void func_8003F5E0(u32 arg0) {
    D_800814C8 = arg0;
}
