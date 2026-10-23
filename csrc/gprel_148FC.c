/* A carved-out translation unit: these functions reach D_800814E0
 * $gp-relatively, so gas has to be left at -G8 rather than maspsx's forced
 * -G0 (see the MASPSXFLAGS_G8 line in the Makefile and
 * docs/addressing-and-translation-units.md).
 *
 * The unit's extent is evidence, not a guess. Of the 38 $gp-relative symbols
 * the blocked functions reach, only two are referenced from more than one of
 * the project's current units, and D_800814E0 is one of them: func_800410FC
 * was the last function of unk_12DEC, func_80041134 the first of unk_14934,
 * and func_80041110 sat between them in a carved unit of its own. A
 * $gp-relative reference means the function was compiled in the unit that
 * defined the global, so all three were one original translation unit and the
 * boundaries at 0x800410FC and 0x80041134 both cut through it. Moving the
 * carve-out's two ends to take them in is what lets the other two match.
 *
 * It is named for its start offset, like the other provisional units.
 */
#include "common.h"

/* Registers a callback: store it at the write pointer, then clear the new
 * tail. The pointer is bumped by func_80041110 and backed off by
 * func_80041134, so the three share the table and its cursor. */
void func_800410FC(void) {
    D_800814E0 = (s32 *) D_80083C68;
    D_80083C68[0] = NULL;
}

void func_80041110(s32 arg0) {
    s32 *temp;

    *D_800814E0 = arg0;
    temp = D_800814E0;
    D_800814E0 = temp + 1;
    temp[1] = 0;
}

/* Unregisters one callback: find it, back the cursor off by a slot, then shift
 * the tail down over it.
 *
 * The inner shift loop is a `do`/`while` -- its test is at the bottom and its
 * exit joins the function's single `return`, which is why `beqz` goes straight
 * to the epilogue and the `j` back to the top is unconditional. The outer loop
 * is a `while`, so its guard reads D_80083C68[0] through the `%lo`
 * displacement while the body reads through the induction register; that
 * guard/body split is what duplicates the load. */
void func_80041134(void (*arg0)(void)) {
    void (**p)(void) = D_80083C68;

    while (*p != NULL) {
        if (*p == arg0) {
            D_800814E0--;
            do {
                p[0] = p[1];
                p++;
            } while (*p != NULL);
            return;
        }
        p++;
    }
}
