/* A carved-out translation unit: these functions reach D_800814C8
 * $gp-relatively, so gas has to be left at -G8 rather than maspsx's forced
 * -G0 (see MASPSXFLAGS_G8 in the Makefile and
 * docs/addressing-and-translation-units.md).
 *
 * func_8003F5E0 was the first function of unk_12DEC until the boundary was
 * moved to take it in. The two belong together on the evidence: one reads
 * D_800814C8 and the other writes it, $gp-relatively, and a $gp-relative
 * reference means the function was compiled in the unit that defined the
 * global. The unit keeps its name because its START is unchanged; what moved
 * is where the next unit begins.
 */
#include "common.h"

u32 func_8003F5D4(void) {
    return D_800814C8;
}

void func_8003F5E0(u32 arg0) {
    D_800814C8 = arg0;
}
