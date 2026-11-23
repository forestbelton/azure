#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024098);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024188);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024704);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024758);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80024FD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025028);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025278);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025338);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025470);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_80025814);

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s17_800259D8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s17_80024058[] = {
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0x08,
    0x00,
    0x00,
    0x00,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0xF8,
    0xFF,
    0x00,
    0x00,
    0xF8,
    0xFF,
    0x08,
    0x00,
    0xF8,
    0xFF,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s17", func_s17_800259D8);
