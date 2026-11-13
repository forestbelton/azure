#include "common.h"
#include "main_twin.h"

/* THE TWIN'S FIRST RODATA, 0x80400000..0x8040007C -- 0x7C bytes, 7 objects.
 *
 * c_server.c's rodata: the assert pair, the debug-print pair, and the two card
 * event handler tables.  This is csrc/ovl_main_80400000.c's data, and like
 * csrc/ovl_main_80400000_rodata.c it is in a file of its own -- but for a
 * DIFFERENT reason, and the two should not be collapsed into one unit even
 * though they are both "the twin's rodata".
 *
 * THE REASON IS jtbl_8040007C, WHICH SITS BETWEEN THEM AT 0x8040007C.  It is
 * the jump table cc1psx emits for func_804016D0's switch, so it has to be
 * emitted by the unit that compiles that function -- its entries are `.L`
 * labels inside the function body, and a local label is only visible in its
 * own object.  And gcc aligns a jump table to BIGGEST_ALIGNMENT:
 *
 *     final.c:1516   readonly_data_section ();
 *     final.c:1518   ASM_OUTPUT_ALIGN (file, exact_log2 (BIGGEST_ALIGNMENT
 *                                                        / BITS_PER_UNIT));
 *
 * guarded only by `#ifdef READONLY_DATA_SECTION`, which mips.h defines
 * (:3690), and BIGGEST_ALIGNMENT is 64 (mips.h:1080) -- so `.align 3`, eight
 * bytes.  0x8040007C is 4-aligned and NOT 8-aligned, so the original compiler
 * used `.align 2` here and ours cannot be made to.
 *
 * The consequence is the whole of this file's existence: the jump table has to
 * land at OFFSET 0 of its own input section, where an 8-byte alignment request
 * costs nothing.  Put these seven objects in the function unit and they take
 * the first 0x7C bytes of that section, gas pads from 0x7C to 0x80 to satisfy
 * the `.align 3`, and the overlay is four bytes long -- measured: `.rodata`
 * 148 against the window's 144, `make containers` reporting
 * `size differs by +0x4` with the first difference at 0x269000.  So the yaml
 * gives the segment three `.rodata` windows in a row, 0x269000 here,
 * 0x26907c for the jump table alone, 0x269090 for the rest.
 *
 * This unit is -G0, like the other rodata unit and unlike the function unit:
 * it matches the Makefile's `ovl_%` pattern rule.  That is why D_80400000, a
 * four-byte object, needs no `section(".rodata")` attribute to stay out of
 * `.sdata` -- see docs/owning-data.md SS9.2.  It emits no instructions, so the
 * compiler that builds it cannot affect the bytes.
 *
 * Nothing references D_80400000 -- not this unit, not the function unit, not
 * any remaining asm stub -- so it must be non-static or gcc discards it.  It
 * is one word holding a function address, and the live 0x80020000 overlay has
 * no counterpart for it: that overlay's rodata starts at ASSERT_FMT.  What
 * registers it, if anything does, is not known.
 */

/* 0x80400000 */
void (*const D_80400000)(s32 *) = func_8040855C;

/* 0x80400004 -- ASSERT_FMT.  c_server.c's own copy; each of the nine original
   translation units has one, and the other eight are in
   csrc/ovl_main_80400000_rodata.c. */
const char D_80400004[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x8040002C -- FILENAME_C_SERVER. */
const char D_8040002C[12] = "c_server.c";

/* 0x80400038 -- the debug-print format, used from three sites. */
const char D_80400038[12] = "%s = %d\n";

/* 0x80400044 -- the variable name the line above prints, which is how we know
   the original called this flag `card_first_flg`. */
const char D_80400044[16] = "card_first_flg";

/* 0x80400054 -- gSwCardEventHandlers, and 0x80400068 gHwCardEventHandlers.
   The live csrc/main/c_server.c spells these the same way; only the first
   handler differs between the two tables. */
const UnkFnTab5 D_80400054 = {{
    NULL,
    func_804010F0,
    func_804013D0,
    func_804013A8,
    func_804012B0,
}};

/* 0x80400068 */
const UnkFnTab5 D_80400068 = {{
    NULL,
    func_8040122C,
    func_804013D0,
    func_804013A8,
    func_804012B0,
}};
