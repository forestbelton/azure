/*
 * The speaker/short-name pool: four debug printf formats, a 9-byte key table,
 * 100 names of full-width Shift-JIS in ROM order, and two multi-field objects
 * at the tail.
 *
 * Covers rom 0x000800..0x000DC0 (vram 0x8002D000..0x8002D5C0).
 *
 * The 100 names are the dialogue speaker table -- the characters first
 * (3Men, KimBr, Shiela, Beldo ...) and then the monsters in lower case
 * (golem, tyrant, kewne ...). csrc/str/names.c's D_80080A50 points into it,
 * at entry 58, "Girl".
 *
 * The object boundaries are measured, not guessed. Every one of the 100 names
 * is pointed at from a table inside `game_data` at rom 0x03E5A0..0x03E824,
 * those 100 pointer targets are exactly the 100 NUL-walked string starts, and
 * no byte of 0x848..0xD94 is outside a string and non-zero. None of the
 * pointers is a relocated symbol, so a wrong split here would have been
 * silent; the first entry goes in `symbol_addrs` so at least the head of the
 * table still reads symbolically.
 *
 * D_8002D000 is also the image's own load address, which is why
 * asm/psyq/libsn/SNMAIN.s reaches it: `__main` takes `%hi/%lo` of it twice.
 * It keeps its splat name for that reason, as do the other four objects code
 * names.
 *
 * Each entry is a separate object, so the compiler's own `.align 2` supplies
 * the zero padding up to the next 4-byte boundary. ORDER IS THE LAYOUT:
 * cc1psx emits file-scope objects in source order, so moving a definition
 * moves the bytes.
 */

#include "common.h"

const u8 D_8002D000[] = "MESSYS  %x \n";

const u8 D_8002D010[] = "MESPNT  %x \n";

const u8 D_8002D020[] = "STEP    %x \n";

const u8 D_8002D030[] = "XASTAT %x\n";

/* The nine key codes func_80038C40 copies to its stack and scans. */
const s8 D_8002D03C[9] = {10, -3, -2, -1, 0, 75, 21, 22, 23};

/* ３Ｍｅｎ */
const char STR_SPEAKER_3MEN[] = "\x82\x52\x82\x6C\x82\x85\x82\x8E";

/* ＫｉｍＢｒ */
const char STR_SPEAKER_KIMBR[] = "\x82\x6A\x82\x89\x82\x8D\x82\x61\x82\x92";

/* Ｇｉｒｌ２ */
const char STR_SPEAKER_GIRL2[] = "\x82\x66\x82\x89\x82\x92\x82\x8C\x82\x51";

/* Ｇｉｒｌ１ */
const char STR_SPEAKER_GIRL1[] = "\x82\x66\x82\x89\x82\x92\x82\x8C\x82\x50";

/* Ｂｕｎｎｙ２ */
const char STR_SPEAKER_BUNNY2[] = "\x82\x61\x82\x95\x82\x8E\x82\x8E\x82\x99\x82\x51";

/* Ｂｕｔｌｅｒ */
const char STR_SPEAKER_BUTLER[] = "\x82\x61\x82\x95\x82\x94\x82\x8C\x82\x85\x82\x92";

/* Ｓｈｉｅｌａ */
const char STR_SPEAKER_SHIELA[] = "\x82\x72\x82\x88\x82\x89\x82\x85\x82\x8C\x82\x81";

/* Ｈｏｒｓｅ */
const char STR_SPEAKER_HORSE[] = "\x82\x67\x82\x8F\x82\x92\x82\x93\x82\x85";

/* Ｄａｎｓｅｒ */
const char STR_SPEAKER_DANSER[] = "\x82\x63\x82\x81\x82\x8E\x82\x93\x82\x85\x82\x92";

/* Ｍｉｎｇ */
const char STR_SPEAKER_MING[] = "\x82\x6C\x82\x89\x82\x8E\x82\x87";

/* Ｓｕｎｇ */
const char STR_SPEAKER_SUNG[] = "\x82\x72\x82\x95\x82\x8E\x82\x87";

/* Ｏｒｃｈｉｄ */
const char STR_SPEAKER_ORCHID[] = "\x82\x6E\x82\x92\x82\x83\x82\x88\x82\x89\x82\x84";

/* Ｃｓｔｍｅｒ */
const char STR_SPEAKER_CSTMER[] = "\x82\x62\x82\x93\x82\x94\x82\x8D\x82\x85\x82\x92";

/* Ｎａｄａ */
const char STR_SPEAKER_NADA[] = "\x82\x6D\x82\x81\x82\x84\x82\x81";

/* Ｈｅａｒ */
const char STR_SPEAKER_HEAR[] = "\x82\x67\x82\x85\x82\x81\x82\x92";

/* Ｌ．Ｂｒｏ． */
const char STR_SPEAKER_L_BRO[] = "\x82\x6B\x81\x44\x82\x61\x82\x92\x82\x8F\x81\x44";

/* Ｂ．Ｂｒｏ． */
const char STR_SPEAKER_B_BRO[] = "\x82\x61\x81\x44\x82\x61\x82\x92\x82\x8F\x81\x44";

/* Ｈｕｎｔｅｒ */
const char STR_SPEAKER_HUNTER[] = "\x82\x67\x82\x95\x82\x8E\x82\x94\x82\x85\x82\x92";

/* Ｂｕｎｎｙ */
const char STR_SPEAKER_BUNNY[] = "\x82\x61\x82\x95\x82\x8E\x82\x8E\x82\x99";

/* Ｋｉｋｋａ */
const char STR_SPEAKER_KIKKA[] = "\x82\x6A\x82\x89\x82\x8B\x82\x8B\x82\x81";

/* Ｇｒａｎｎｙ */
const char STR_SPEAKER_GRANNY[] = "\x82\x66\x82\x92\x82\x81\x82\x8E\x82\x8E\x82\x99";

/* Ｇｒａｎｄｐ */
const char STR_SPEAKER_GRANDP[] = "\x82\x66\x82\x92\x82\x81\x82\x8E\x82\x84\x82\x90";

/* Ｕｎｃｌｅ */
const char STR_SPEAKER_UNCLE[] = "\x82\x74\x82\x8E\x82\x83\x82\x8C\x82\x85";

/* Ａｕｎｔ */
const char STR_SPEAKER_AUNT[] = "\x82\x60\x82\x95\x82\x8E\x82\x94";

/* Ｎａｒｒａｔ */
const char STR_SPEAKER_NARRAT[] = "\x82\x6D\x82\x81\x82\x92\x82\x92\x82\x81\x82\x94";

/* ＬＢｒ．ＫＭ */
const char STR_SPEAKER_LBR_KM[] = "\x82\x6B\x82\x61\x82\x92\x81\x44\x82\x6A\x82\x6C";

/* ＢＢｒ．ＫＭ */
const char STR_SPEAKER_BBR_KM[] = "\x82\x61\x82\x61\x82\x92\x81\x44\x82\x6A\x82\x6C";

/* Ｍａｓｔｅｒ */
const char STR_SPEAKER_MASTER[] = "\x82\x6C\x82\x81\x82\x93\x82\x94\x82\x85\x82\x92";

/* Ｃｈｅｒｏ */
const char STR_SPEAKER_CHERO[] = "\x82\x62\x82\x88\x82\x85\x82\x92\x82\x8F";

/* Ｎａｃｏ */
const char STR_SPEAKER_NACO[] = "\x82\x6D\x82\x81\x82\x83\x82\x8F";

/* Ｓｈｉｎ */
const char STR_SPEAKER_SHIN[] = "\x82\x72\x82\x88\x82\x89\x82\x8E";

/* Ｒｏｍｉ */
const char STR_SPEAKER_ROMI[] = "\x82\x71\x82\x8F\x82\x8D\x82\x89";

/* Ｅｔｔｏ */
const char STR_SPEAKER_ETTO[] = "\x82\x64\x82\x94\x82\x94\x82\x8F";

/* Ｊａｃｏｆ */
const char STR_SPEAKER_JACOF[] = "\x82\x69\x82\x81\x82\x83\x82\x8F\x82\x86";

/* Ｉｓｓａｃ */
const char STR_SPEAKER_ISSAC[] = "\x82\x68\x82\x93\x82\x93\x82\x81\x82\x83";

/* Ａｎｇｅｌ */
const char STR_SPEAKER_ANGEL[] = "\x82\x60\x82\x8E\x82\x87\x82\x85\x82\x8C";

/* ＮｃｓＤａｄ */
const char STR_SPEAKER_NCSDAD[] = "\x82\x6D\x82\x83\x82\x93\x82\x63\x82\x81\x82\x84";

/* Ｃｏｐｐｅ */
const char STR_SPEAKER_COPPE[] = "\x82\x62\x82\x8F\x82\x90\x82\x90\x82\x85";

/* Ｎａｄｉａ */
const char STR_SPEAKER_NADIA[] = "\x82\x6D\x82\x81\x82\x84\x82\x89\x82\x81";

/* Ｈｕｓｈ */
const char STR_SPEAKER_HUSH[] = "\x82\x67\x82\x95\x82\x93\x82\x88";

/* Ｔｏｎｋａ */
const char STR_SPEAKER_TONKA[] = "\x82\x73\x82\x8F\x82\x8E\x82\x8B\x82\x81";

/* Ｄｒ．Ｈａｌ */
const char STR_SPEAKER_DR_HAL[] = "\x82\x63\x82\x92\x81\x44\x82\x67\x82\x81\x82\x8C";

/* Ｊｏｒｄａ */
const char STR_SPEAKER_JORDA[] = "\x82\x69\x82\x8F\x82\x92\x82\x84\x82\x81";

/* Ｓｉｌｖｅｒ */
const char STR_SPEAKER_SILVER[] = "\x82\x72\x82\x89\x82\x8C\x82\x96\x82\x85\x82\x92";

/* Ｗｏｔｔａ */
const char STR_SPEAKER_WOTTA[] = "\x82\x76\x82\x8F\x82\x94\x82\x94\x82\x81";

/* Ｂａｒｔｅｎ */
const char STR_SPEAKER_BARTEN[] = "\x82\x61\x82\x81\x82\x92\x82\x94\x82\x85\x82\x8E";

/* Ｏｋａｍｉ */
const char STR_SPEAKER_OKAMI[] = "\x82\x6E\x82\x8B\x82\x81\x82\x8D\x82\x89";

/* Ｂｅｌｄｏ */
const char STR_SPEAKER_BELDO[] = "\x82\x61\x82\x85\x82\x8C\x82\x84\x82\x8F";

/* Ｂａｒｒｙ */
const char STR_SPEAKER_BARRY[] = "\x82\x61\x82\x81\x82\x92\x82\x92\x82\x99";

/* Ｃｈｅｒｒｌ */
const char STR_SPEAKER_CHERRL[] = "\x82\x62\x82\x88\x82\x85\x82\x92\x82\x92\x82\x8C";

/* Ｖｉｖｉａｎ */
const char STR_SPEAKER_VIVIAN[] = "\x82\x75\x82\x89\x82\x96\x82\x89\x82\x81\x82\x8E";

/* Ｎｉｃｏ */
const char STR_SPEAKER_NICO[] = "\x82\x6D\x82\x89\x82\x83\x82\x8F";

/* Ｐａｔｔｙ */
const char STR_SPEAKER_PATTY[] = "\x82\x6F\x82\x81\x82\x94\x82\x94\x82\x99";

/* Ｓｅｌｆｉ */
const char STR_SPEAKER_SELFI[] = "\x82\x72\x82\x85\x82\x8C\x82\x86\x82\x89";

/* Ｇｈｏｓｈ */
const char STR_SPEAKER_GHOSH[] = "\x82\x66\x82\x88\x82\x8F\x82\x93\x82\x88";

/* Ｗｅｅｄｙ */
const char STR_SPEAKER_WEEDY[] = "\x82\x76\x82\x85\x82\x85\x82\x84\x82\x99";

/* Ｗｒｅａｔｈ */
const char STR_SPEAKER_WREATH[] = "\x82\x76\x82\x92\x82\x85\x82\x81\x82\x94\x82\x88";

/* Ｇｉｒｌ */
const char STR_SPEAKER_GIRL[] = "\x82\x66\x82\x89\x82\x92\x82\x8C";

/* ｇｏｌｅｍ */
const char STR_SPEAKER_GOLEM[] = "\x82\x87\x82\x8F\x82\x8C\x82\x85\x82\x8D";

/* ｔｙｒａｎｔ */
const char STR_SPEAKER_TYRANT[] = "\x82\x94\x82\x99\x82\x92\x82\x81\x82\x8E\x82\x94";

/* ｇｌａｃｙ */
const char STR_SPEAKER_GLACY[] = "\x82\x87\x82\x8C\x82\x81\x82\x83\x82\x99";

/* ｇａｒｕｄａ */
const char STR_SPEAKER_GARUDA[] = "\x82\x87\x82\x81\x82\x92\x82\x95\x82\x84\x82\x81";

/* ｋｉｌｌｅｒ */
const char STR_SPEAKER_KILLER[] = "\x82\x8B\x82\x89\x82\x8C\x82\x8C\x82\x85\x82\x92";

/* ｍａｎｄａ */
const char STR_SPEAKER_MANDA[] = "\x82\x8D\x82\x81\x82\x8E\x82\x84\x82\x81";

/* ｎａｐｌａｓ */
const char STR_SPEAKER_NAPLAS[] = "\x82\x8E\x82\x81\x82\x90\x82\x8C\x82\x81\x82\x93";

/* ｖｉｐｅｒ */
const char STR_SPEAKER_VIPER[] = "\x82\x96\x82\x89\x82\x90\x82\x85\x82\x92";

/* ｓｔｅａｌ */
const char STR_SPEAKER_STEAL[] = "\x82\x93\x82\x94\x82\x85\x82\x81\x82\x8C";

/* ｗｅａｄｏｇ */
const char STR_SPEAKER_WEADOG[] = "\x82\x97\x82\x85\x82\x81\x82\x84\x82\x8F\x82\x87";

/* ｋｒａｋｅｎ */
const char STR_SPEAKER_KRAKEN[] = "\x82\x8B\x82\x92\x82\x81\x82\x8B\x82\x85\x82\x8E";

/* ｐｉｃｋｅｔ */
const char STR_SPEAKER_PICKET[] = "\x82\x90\x82\x89\x82\x83\x82\x8B\x82\x85\x82\x94";

/* ｂａｒｏｎｇ */
const char STR_SPEAKER_BARONG[] = "\x82\x82\x82\x81\x82\x92\x82\x8F\x82\x8E\x82\x87";

/* ｍａｎｅｖａ */
const char STR_SPEAKER_MANEVA[] = "\x82\x8D\x82\x81\x82\x8E\x82\x85\x82\x96\x82\x81";

/* ｃｙｃｌｏ */
const char STR_SPEAKER_CYCLO[] = "\x82\x83\x82\x99\x82\x83\x82\x8C\x82\x8F";

/* ｖｏｌｃａｎ */
const char STR_SPEAKER_VOLCAN[] = "\x82\x96\x82\x8F\x82\x8C\x82\x83\x82\x81\x82\x8E";

/* ｂｌｕｍｅ */
const char STR_SPEAKER_BLUME[] = "\x82\x82\x82\x8C\x82\x95\x82\x8D\x82\x85";

/* ｄｒｅａｍ */
const char STR_SPEAKER_DREAM[] = "\x82\x84\x82\x92\x82\x85\x82\x81\x82\x8D";

/* ｂａｌｏｏｎ */
const char STR_SPEAKER_BALOON[] = "\x82\x82\x82\x81\x82\x8C\x82\x8F\x82\x8F\x82\x8E";

/* ｕ－ｂｏａｔ */
const char STR_SPEAKER_U_BOAT[] = "\x82\x95\x81\x7C\x82\x82\x82\x8F\x82\x81\x82\x94";

/* ｎｏｉｓｅ */
const char STR_SPEAKER_NOISE[] = "\x82\x8E\x82\x8F\x82\x89\x82\x93\x82\x85";

/* ｔｒｏｌｌ */
const char STR_SPEAKER_TROLL[] = "\x82\x94\x82\x92\x82\x8F\x82\x8C\x82\x8C";

/* ｐｕｌｕｎ */
const char STR_SPEAKER_PULUN[] = "\x82\x90\x82\x95\x82\x8C\x82\x95\x82\x8E";

/* ｂｌｏｃｋ */
const char STR_SPEAKER_BLOCK[] = "\x82\x82\x82\x8C\x82\x8F\x82\x83\x82\x8B";

/* ｍｅｔａｌ */
const char STR_SPEAKER_METAL[] = "\x82\x8D\x82\x85\x82\x94\x82\x81\x82\x8C";

/* ｕｎｉｃｏｎ */
const char STR_SPEAKER_UNICON[] = "\x82\x95\x82\x8E\x82\x89\x82\x83\x82\x8F\x82\x8E";

/* ｕｎｉｖｅｒ */
const char STR_SPEAKER_UNIVER[] = "\x82\x95\x82\x8E\x82\x89\x82\x96\x82\x85\x82\x92";

/* ｃｌｏｗｎ */
const char STR_SPEAKER_CLOWN[] = "\x82\x83\x82\x8C\x82\x8F\x82\x97\x82\x8E";

/* ｄｅａｔｈ */
const char STR_SPEAKER_DEATH[] = "\x82\x84\x82\x85\x82\x81\x82\x94\x82\x88";

/* ｎｙｕｅｌ */
const char STR_SPEAKER_NYUEL[] = "\x82\x8E\x82\x99\x82\x95\x82\x85\x82\x8C";

/* ｂａｔｔｎｅ */
const char STR_SPEAKER_BATTNE[] = "\x82\x82\x82\x81\x82\x94\x82\x94\x82\x8E\x82\x85";

/* ａｒａｎ */
const char STR_SPEAKER_ARAN[] = "\x82\x81\x82\x92\x82\x81\x82\x8E";

/* ａｓｈｒａ */
const char STR_SPEAKER_ASHRA[] = "\x82\x81\x82\x93\x82\x88\x82\x92\x82\x81";

/* ｓｎｏｗ */
const char STR_SPEAKER_SNOW[] = "\x82\x93\x82\x8E\x82\x8F\x82\x97";

/* ｓａｂｅｒ */
const char STR_SPEAKER_SABER[] = "\x82\x93\x82\x81\x82\x82\x82\x85\x82\x92";

/* ｇｒｉｆｆ */
const char STR_SPEAKER_GRIFF[] = "\x82\x87\x82\x92\x82\x89\x82\x86\x82\x86";

/* ｇｒｉｎｅｕ */
const char STR_SPEAKER_GRINEU[] = "\x82\x87\x82\x92\x82\x89\x82\x8E\x82\x85\x82\x95";

/* ｆｌａｍｅ */
const char STR_SPEAKER_FLAME[] = "\x82\x86\x82\x8C\x82\x81\x82\x8D\x82\x85";

/* ｉｆｒｉｔ */
const char STR_SPEAKER_IFRIT[] = "\x82\x89\x82\x86\x82\x92\x82\x89\x82\x94";

/* ｄｒａｇｏｎ */
const char STR_SPEAKER_DRAGON[] = "\x82\x84\x82\x92\x82\x81\x82\x87\x82\x8F\x82\x8E";

/* ｋｅｗｎｅ */
const char STR_SPEAKER_KEWNE[] = "\x82\x8B\x82\x85\x82\x97\x82\x8E\x82\x85";

/* ｈｉｋｅｗ */
const char STR_SPEAKER_HIKEW[] = "\x82\x88\x82\x89\x82\x8B\x82\x85\x82\x97";

/* Four {0, buffer} pairs, the buffers 0x4000 apart.  func_8003C920
   copies all eight words twice; see Blk20 in include/structs.h. */
const Blk20 D_8002D594 = {{
    0x00000000, 0x80100E80, 0x00000000, 0x80104E80,
    0x00000000, 0x80108E80, 0x00000000, 0x8010CE80,
}};

/* Ｋｅｗｎｅ -- ten bytes of full-width Shift-JIS, typed as a struct
   because func_8003D468 assigns it with two lwl/lwr pairs and two lb.
   It is the last object and it is 10 bytes, so the pool's own two bytes of
   trailing padding come from SUBALIGN(4) in the generated linker script
   rather than from an object -- the one pool here where an explicit size on
   the last entry would make the unit two bytes LONG, because cc1psx has no
   next object to align against and emits nothing. */
const Unk8002D5B4 D_8002D5B4 = {
    {0x82, 0x6A, 0x82, 0x85},
    {0x82, 0x97, 0x82, 0x8E},
    0x82,
    0x85,
};
