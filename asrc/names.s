/* HAND-OWNED .rodata: the name table at 0x800809D0, all of it.
 *
 * Covers rom 0x0541D0..0x05426C (vram 0x800809D0..0x80080A6C).
 *
 * Nineteen names of full-width Shift-JIS text in 8-byte slots, with one
 * 4-byte pointer wedged in at 0x54250. `.shiftjisz` emits each entry exactly
 * -- text, NUL, `.balign 4` -- so the file carries no `.fill` and no counting.
 * Sixteen slots run 8-aligned from 0x541D0; the pointer shifts the last three
 * (max, zu, kid) to 4 mod 8, which is why this is not one uniform array.
 *
 * `make` pipes this through `azure shiftjis`, whose directive pattern is
 * line-anchored: keep each `.shiftjisz` ALONE on its line, offset notes above
 * it, or GNU as gets the directive unexpanded and says `unknown pseudo-op`.
 *
 * Nothing decompiled reaches the strings by name -- the D_ labels came from
 * splat's seed and the STR_NAME_ ones are ours. The one entry code does name
 * is D_80080A50, read by func_80035DBC in csrc/unk_72A8.c.
 */

.include "macro.inc"

.section .rodata

dlabel D_800809D0
dlabel STR_NAME_BOY
    .shiftjisz "Ｂｏｙ"

dlabel D_800809D8
dlabel STR_NAME_MAN
    .shiftjisz "Ｍａｎ"

dlabel D_800809E0
dlabel STR_NAME_CAT
    .shiftjisz "Ｃａｔ"

dlabel D_800809E8
dlabel STR_NAME_DOG
    .shiftjisz "Ｄｏｇ"

dlabel D_800809F0
dlabel STR_NAME_POP
    .shiftjisz "Ｐｏｐ"

dlabel D_800809F8
dlabel STR_NAME_SEE
    .shiftjisz "Ｓｅｅ"

dlabel D_80080A00
dlabel STR_NAME_BET
    .shiftjisz "Ｂｅｔ"

dlabel D_80080A08
dlabel STR_NAME_DAR
    .shiftjisz "Ｄａｒ"

dlabel D_80080A10
dlabel STR_NAME_KAY
    .shiftjisz "Ｋａｙ"

dlabel D_80080A18
dlabel STR_NAME_MEE
    .shiftjisz "Ｍｅｅ"

dlabel D_80080A20
dlabel STR_NAME_DAI
    .shiftjisz "Ｄａｉ"

dlabel D_80080A28
dlabel STR_NAME_FON
    .shiftjisz "Ｆｏｎ"

dlabel D_80080A30
dlabel STR_NAME_PEN
    .shiftjisz "Ｐｅｎ"

dlabel D_80080A38
dlabel STR_NAME_MIA
    .shiftjisz "Ｍｉａ"

dlabel D_80080A40
dlabel STR_NAME_FUR
    .shiftjisz "Ｆｕｒ"

dlabel D_80080A48
dlabel STR_NAME_GUY
    .shiftjisz "Ｇｕｙ"

/* NOT a name: a pointer to "Ｇｉｒｌ" at 0x8002D34C, which is entry 58 of the
   CHARACTER-name table in str_pool_800 (file 0x848..0xD98, ~103 entries --
   ＫｉｍＢｒ, Ｓｈｉｅｌａ, Ｂｅｌｄｏ, then the monsters in lower case).
   Different family of names from the nineteen here, so this is a pointer out
   to that table rather than a twentieth slot. func_80035DBC in
   csrc/unk_72A8.c returns it for kind 0xA.

   It would not fit inline anyway: "Ｇｉｒｌ" is four full-width characters,
   eight bytes, and an 8-byte slot leaves nothing for the terminator. */
dlabel D_80080A50
    .word 0x8002D34C

dlabel D_80080A54
dlabel STR_NAME_MAX
    .shiftjisz "ｍａｘ"

dlabel D_80080A5C
dlabel STR_NAME_ZU
    .shiftjisz "ｚｕ"

dlabel D_80080A64
dlabel STR_NAME_KID
    .shiftjisz "ｋｉｄ"
