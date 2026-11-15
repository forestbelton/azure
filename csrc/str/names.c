/*
 * The name table at 0x800809D0, all of it: nineteen names of full-width
 * Shift-JIS in 8-byte slots, with one 4-byte pointer wedged in at 0x54250.
 *
 * Covers rom 0x0541D0..0x05426C (vram 0x800809D0..0x80080A6C).
 *
 * Sixteen slots run 8-aligned from 0x541D0; the pointer shifts the last three
 * (max, zu, kid) to 4 mod 8, which is why this is not one uniform array.
 *
 * D_80080A50 IS NOT A NAME. It is a pointer to "Girl" at 0x8002D34C, entry 58
 * of the CHARACTER-name table in str_pool_800 (file 0x848..0xD98, ~103 entries
 * -- KimBr, Shiela, Beldo, then the monsters in lower case). A different family
 * of names from the nineteen here, so it points out to that table rather than
 * being a twentieth slot, and it would not fit inline anyway: "Girl" is four
 * full-width characters, eight bytes, and an 8-byte slot leaves nothing for the
 * terminator. It is written as a `const u32` rather than a string -- the word
 * itself, no NUL and no padding -- because it is not text.
 *
 * Nothing decompiled reaches the strings by name; the D_ labels came from
 * splat's seed and the STR_NAME_ ones are ours. The one entry code does name is
 * D_80080A50, read by func_80035DBC in csrc/unk_72A8.c.
 *
 *
 * Each entry is a separate object, so the compiler's own `.align 2` supplies
 * the zero padding up to the next 4-byte boundary -- the same bytes the old
 * generator emitted as `.balign 4, 0`. ORDER IS THE LAYOUT: cc1psx emits
 * file-scope objects in source order, so moving a definition moves the bytes.
 * The last entry carries an explicit size where the pool needs trailing
 * padding, because nothing follows it to align against.
 */

#include "common.h"

/* Ｂｏｙ */
const char STR_NAME_BOY[] = "\x82\x61\x82\x8F\x82\x99";

/* Ｍａｎ */
const char STR_NAME_MAN[] = "\x82\x6C\x82\x81\x82\x8E";

/* Ｃａｔ */
const char STR_NAME_CAT[] = "\x82\x62\x82\x81\x82\x94";

/* Ｄｏｇ */
const char STR_NAME_DOG[] = "\x82\x63\x82\x8F\x82\x87";

/* Ｐｏｐ */
const char STR_NAME_POP[] = "\x82\x6F\x82\x8F\x82\x90";

/* Ｓｅｅ */
const char STR_NAME_SEE[] = "\x82\x72\x82\x85\x82\x85";

/* Ｂｅｔ */
const char STR_NAME_BET[] = "\x82\x61\x82\x85\x82\x94";

/* Ｄａｒ */
const char STR_NAME_DAR[] = "\x82\x63\x82\x81\x82\x92";

/* Ｋａｙ */
const char STR_NAME_KAY[] = "\x82\x6A\x82\x81\x82\x99";

/* Ｍｅｅ */
const char STR_NAME_MEE[] = "\x82\x6C\x82\x85\x82\x85";

/* Ｄａｉ */
const char STR_NAME_DAI[] = "\x82\x63\x82\x81\x82\x89";

/* Ｆｏｎ */
const char STR_NAME_FON[] = "\x82\x65\x82\x8F\x82\x8E";

/* Ｐｅｎ */
const char STR_NAME_PEN[] = "\x82\x6F\x82\x85\x82\x8E";

/* Ｍｉａ */
const char STR_NAME_MIA[] = "\x82\x6C\x82\x89\x82\x81";

/* Ｆｕｒ */
const char STR_NAME_FUR[] = "\x82\x65\x82\x95\x82\x92";

/* Ｇｕｙ */
const char STR_NAME_GUY[] = "\x82\x66\x82\x95\x82\x99";

/* NOT A NAME: a pointer to "Girl" at 0x8002D34C, entry 58 of the
   CHARACTER-name table in str_pool_800. See the header above. */
const u32 D_80080A50 = 0x8002D34C;

/* ｍａｘ */
const char STR_NAME_MAX[] = "\x82\x8D\x82\x81\x82\x98";

/* ｚｕ */
const char STR_NAME_ZU[] = "\x82\x9A\x82\x95";

/* ｋｉｄ */
const char STR_NAME_KID[8] = "\x82\x8B\x82\x89\x82\x84\x00\x00";
