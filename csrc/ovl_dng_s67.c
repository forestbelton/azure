#include "common.h"

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_8002408C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024564);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_800245B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_800246F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_800247C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024834);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024918);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024B3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024BD0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024C50);

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s67_80024C50. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s67_80024034[] = {
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
};

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_s67_80024E54. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s67_80024054[] = {
    0x05,
    0x00,
    0x00,
    0x00,
    0x05,
    0x00,
    0x05,
    0x00,
    0x00,
    0x00,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0xFB,
    0xFF,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0xFB,
    0xFF,
    0x05,
    0x00,
    0xFB,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s67", func_s67_80024E54);
