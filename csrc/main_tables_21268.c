/* main.bin's monster/string index tables, rom 0x21268..0x21768, vram
 * 0x8002CA68: a 64-element array of a 20-byte record, followed by zero
 * padding to 0x21800 (the `pad` segment after this one in splat/main.bin.yaml).
 *
 * WHAT WALKS IT. func_8004A574 in csrc/unk_1C540.c reaches it through the
 * resident descriptor array at D_80073414:
 *
 *     e = (u8 *) D_80073414 + p[1] * 20;          // the resident descriptors
 *     r = (u8 *) *(u32 *) (e + 0xC) + p[0] * 20;  // == this array
 *     v = *(u16 *) (r + (w ? 0x12 : 0x10)) * D_800713CC[e[3]];
 *
 * so `+0xC` of the outer record is this array's base, `p[0]` indexes it by
 * 20, and unk_10/unk_12 are the two u16s it scales. That function still uses
 * raw byte offsets; it can be rewritten against this struct, which is why the
 * type is declared here rather than inside the array's own definition.
 *
 * THE FIELD NAMES ARE DELIBERATELY unk_. What the record MEANS is not
 * established -- D_80073414 and D_800713CC are both still asm in
 * slus_006.14 -- and the only semantics anything here can honestly claim are
 * the ones the POOL LABELS give away by sight: unk_04 is a monster's name in
 * capitals, unk_08 the matching egg description, unk_0C a colour in lower
 * case, and the groups of two and three match the pool's own grouping that
 * asrc/str/monsters.txt describes. That is an observation about 63 records,
 * not a decoding of the format, so it lives in this comment.
 *
 * WHAT THE 64 RECORDS ACTUALLY HOLD, counted rather than asserted:
 *   unk_00  0x10 in all 64.
 *   unk_01  0x00 in 52, 0x01 in 11, 0x80 in exactly one (record 20, BLOCK).
 *   unk_02  0x0C in 63; 0x24 in record 0 alone.
 *   unk_03  0 in all 64.
 *   unk_10  100 in 62 records. Record 0 is 0 and RECORD 23 IS 150. The splat
 *           comment used to say "100 in every record"; it is not, and the one
 *           outlier is the sort of thing a `bin` segment hides.
 *   unk_12  varies: 50000, 15000, 10000, 6000, 4000, 3200, 2000, 600, 100...
 *   135 of the pointer words are pool pointers covering all 134 distinct
 *   strings, the first pointed at twice. Records 60..63 carry no pointers at
 *   all and are the array's empty tail.
 *
 * RECORD 0 IS NOT A HEADER, and it is not a normal record either. The array
 * base 0x8002CA68 is rom 0x21268, so func_8004A574's index 0 lands HERE --
 * that is what settles it. But its unk_02 is 0x24 rather than 0x0C and its
 * unk_04 is 0xA001020C, which is not a pool pointer: every real one is in
 * 0x8002C000..0x8002CA54. 0xA001020C is the KSEG1 (uncached) alias of
 * 0x8001020C, which would be inside the resident executable rather than this
 * resource -- that reading is UNVERIFIED and is recorded only so the next
 * person does not have to notice it again. Written as a cast so the link does
 * not try to relocate it.
 */

#include "common.h"

typedef struct
{
    /* 0x00 */ u8 unk_00;
    /* 0x01 */ u8 unk_01;
    /* 0x02 */ u8 unk_02;
    /* 0x03 */ u8 unk_03;
    /* 0x04 */ const char *unk_04;
    /* 0x08 */ const char *unk_08;
    /* 0x0C */ const char *unk_0C;
    /* 0x10 */ u16 unk_10;
    /* 0x12 */ u16 unk_12;
} UnkMainTbl14; /* sizeof == 0x14 */

extern const char STR_MAIN_AQUA_BLUE[];
extern const char STR_MAIN_ARACHNE[];
extern const char STR_MAIN_ARACHNE_EGG[];
extern const char STR_MAIN_ASHRA[];
extern const char STR_MAIN_ASHRA_EGG[];
extern const char STR_MAIN_BALOON[];
extern const char STR_MAIN_BALOON_EGG[];
extern const char STR_MAIN_BARONG[];
extern const char STR_MAIN_BARONG_EGG[];
extern const char STR_MAIN_BATTNEL[];
extern const char STR_MAIN_BATTNEL_EGG[];
extern const char STR_MAIN_BELDO[];
extern const char STR_MAIN_BLACK[];
extern const char STR_MAIN_BLOCK[];
extern const char STR_MAIN_BLOCK_EGG[];
extern const char STR_MAIN_BLUE[];
extern const char STR_MAIN_BLUME[];
extern const char STR_MAIN_BLUME_EGG[];
extern const char STR_MAIN_BROWN[];
extern const char STR_MAIN_BURGUNDY[];
extern const char STR_MAIN_CINNABAR[];
extern const char STR_MAIN_CLOWN[];
extern const char STR_MAIN_CLOWN_EGG[];
extern const char STR_MAIN_CYAN[];
extern const char STR_MAIN_CYCLONE[];
extern const char STR_MAIN_CYCLONE_EGG[];
extern const char STR_MAIN_DARK_GRAY[];
extern const char STR_MAIN_DEATH[];
extern const char STR_MAIN_DEATH_EGG[];
extern const char STR_MAIN_DEEP_BLUE[];
extern const char STR_MAIN_DRAGON[];
extern const char STR_MAIN_DRAGON_EGG[];
extern const char STR_MAIN_DREAMIN[];
extern const char STR_MAIN_DREAMIN_EGG[];
extern const char STR_MAIN_FIRE_WALL[];
extern const char STR_MAIN_FLAME[];
extern const char STR_MAIN_FLAME_EGG[];
extern const char STR_MAIN_FROG[];
extern const char STR_MAIN_GARUDA[];
extern const char STR_MAIN_GARUDA_EGG[];
extern const char STR_MAIN_GHOSH[];
extern const char STR_MAIN_GLACIER[];
extern const char STR_MAIN_GLACIER_EGG[];
extern const char STR_MAIN_GOLEM[];
extern const char STR_MAIN_GOLEM_EGG[];
extern const char STR_MAIN_GRAY[];
extern const char STR_MAIN_GREEN[];
extern const char STR_MAIN_GRIFFON[];
extern const char STR_MAIN_GRIFFON_EGG[];
extern const char STR_MAIN_GRINEUT[];
extern const char STR_MAIN_GRINEUT_EGG[];
extern const char STR_MAIN_GUY[];
extern const char STR_MAIN_ICE_WALL[];
extern const char STR_MAIN_IFRIT[];
extern const char STR_MAIN_IFRIT_EGG[];
extern const char STR_MAIN_INDIGO[];
extern const char STR_MAIN_KEWNE[];
extern const char STR_MAIN_KEWNE_EGG[];
extern const char STR_MAIN_KHAKI[];
extern const char STR_MAIN_KID[];
extern const char STR_MAIN_KID_EGG[];
extern const char STR_MAIN_KILLER[];
extern const char STR_MAIN_KILLER_EGG[];
extern const char STR_MAIN_KRAKEN[];
extern const char STR_MAIN_KRAKEN_EGG[];
extern const char STR_MAIN_LAVENDER[];
extern const char STR_MAIN_MAGENTA[];
extern const char STR_MAIN_MANDARA[];
extern const char STR_MAIN_MANDARA_EGG[];
extern const char STR_MAIN_MANOEVA[];
extern const char STR_MAIN_MANOEVA_EGG[];
extern const char STR_MAIN_MAXIMUM[];
extern const char STR_MAIN_MAXIMUM_EGG[];
extern const char STR_MAIN_METAL[];
extern const char STR_MAIN_METAL_EGG[];
extern const char STR_MAIN_NAPLASS[];
extern const char STR_MAIN_NAPLASS_EGG[];
extern const char STR_MAIN_NOISE[];
extern const char STR_MAIN_NOISE_EGG[];
extern const char STR_MAIN_NYUEL[];
extern const char STR_MAIN_NYUEL_EGG[];
extern const char STR_MAIN_OCHER[];
extern const char STR_MAIN_ORANGE[];
extern const char STR_MAIN_PALE_PINK[];
extern const char STR_MAIN_PICKET[];
extern const char STR_MAIN_PICKET_EGG[];
extern const char STR_MAIN_PINK[];
extern const char STR_MAIN_PULUNPA[];
extern const char STR_MAIN_PULUNPA_EGG[];
extern const char STR_MAIN_PURPLE[];
extern const char STR_MAIN_PURPLISH[];
extern const char STR_MAIN_RAINBOW[];
extern const char STR_MAIN_RED[];
extern const char STR_MAIN_ROCK[];
extern const char STR_MAIN_RUSSET[];
extern const char STR_MAIN_SABER[];
extern const char STR_MAIN_SABER_EGG[];
extern const char STR_MAIN_SALAMAN[];
extern const char STR_MAIN_SCARLET[];
extern const char STR_MAIN_SEA_BLUE[];
extern const char STR_MAIN_SELFI[];
extern const char STR_MAIN_SIENNA[];
extern const char STR_MAIN_SKY_BLUE[];
extern const char STR_MAIN_SNOWMAN[];
extern const char STR_MAIN_SNOWMAN_EGG[];
extern const char STR_MAIN_STEALTH[];
extern const char STR_MAIN_STEALTH_EGG[];
extern const char STR_MAIN_STORM_WALL[];
extern const char STR_MAIN_THE_ULTIMATE_EGG[];
extern const char STR_MAIN_TROLL[];
extern const char STR_MAIN_TROLL_EGG[];
extern const char STR_MAIN_TYRANT[];
extern const char STR_MAIN_TYRANT_EGG[];
extern const char STR_MAIN_U_BOAT[];
extern const char STR_MAIN_U_BOAT_EGG[];
extern const char STR_MAIN_ULTIMATE[];
extern const char STR_MAIN_UMBER[];
extern const char STR_MAIN_UNICORN[];
extern const char STR_MAIN_UNICORN_EGG[];
extern const char STR_MAIN_UNIVERN[];
extern const char STR_MAIN_UNIVERN_EGG[];
extern const char STR_MAIN_VERMILION[];
extern const char STR_MAIN_VIOLET[];
extern const char STR_MAIN_VIPER[];
extern const char STR_MAIN_VIPER_EGG[];
extern const char STR_MAIN_VOLCANO[];
extern const char STR_MAIN_VOLCANO_EGG[];
extern const char STR_MAIN_WEADOG[];
extern const char STR_MAIN_WEADOG_EGG[];
extern const char STR_MAIN_WHITE[];
extern const char STR_MAIN_WINE_RED[];
extern const char STR_MAIN_YELLOW[];
extern const char STR_MAIN_ZU[];
extern const char STR_MAIN_ZU_EGG[];

const UnkMainTbl14 D_8002CA68[64] = {
    { 0x10, 0x00, 0x24, 0x00, (const char *)0xA001020C, NULL,                      NULL,                 0,     0 },  /*  0 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_ULTIMATE,        STR_MAIN_THE_ULTIMATE_EGG, NULL,               100, 50000 },  /*  1 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_KEWNE,           STR_MAIN_KEWNE_EGG,        NULL,               100,   100 },  /*  2 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_DRAGON,          STR_MAIN_DRAGON_EGG,       NULL,               100, 10000 },  /*  3 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_KID,             STR_MAIN_KID_EGG,          STR_MAIN_RED,       100, 15000 },  /*  4 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_IFRIT,           STR_MAIN_IFRIT_EGG,        NULL,               100,   100 },  /*  5 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_FLAME,           STR_MAIN_FLAME_EGG,        STR_MAIN_WINE_RED,  100,   600 },  /*  6 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_GRINEUT,         STR_MAIN_GRINEUT_EGG,      NULL,               100,   100 },  /*  7 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GRIFFON,         STR_MAIN_GRIFFON_EGG,      STR_MAIN_GRAY,      100,  2000 },  /*  8 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_SABER,           STR_MAIN_SABER_EGG,        NULL,               100,   100 },  /*  9 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_SNOWMAN,         STR_MAIN_SNOWMAN_EGG,      STR_MAIN_DARK_GRAY, 100,  6000 },  /* 10 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_ASHRA,           STR_MAIN_ASHRA_EGG,        NULL,               100,   100 },  /* 11 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_ARACHNE,         STR_MAIN_ARACHNE_EGG,      STR_MAIN_PINK,      100,  2400 },  /* 12 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_BATTNEL,         STR_MAIN_BATTNEL_EGG,      NULL,               100,   100 },  /* 13 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_NYUEL,           STR_MAIN_NYUEL_EGG,        STR_MAIN_PALE_PINK, 100,  1800 },  /* 14 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_DEATH,           STR_MAIN_DEATH_EGG,        NULL,               100,   100 },  /* 15 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_CLOWN,           STR_MAIN_CLOWN_EGG,        STR_MAIN_BLUE,      100,  1000 },  /* 16 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_UNIVERN,         STR_MAIN_UNIVERN_EGG,      NULL,               100,   100 },  /* 17 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_UNICORN,         STR_MAIN_UNICORN_EGG,      STR_MAIN_SKY_BLUE,  100,  3200 },  /* 18 */
    { 0x10, 0x01, 0x0C, 0x00, STR_MAIN_METAL,           STR_MAIN_METAL_EGG,        NULL,               100,   100 },  /* 19 */
    { 0x10, 0x80, 0x0C, 0x00, STR_MAIN_BLOCK,           STR_MAIN_BLOCK_EGG,        STR_MAIN_SEA_BLUE,  100,  4000 },  /* 20 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_PULUNPA,         STR_MAIN_PULUNPA_EGG,      STR_MAIN_DEEP_BLUE, 100,   400 },  /* 21 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_TROLL,           STR_MAIN_TROLL_EGG,        STR_MAIN_AQUA_BLUE, 100,   800 },  /* 22 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_NOISE,           STR_MAIN_NOISE_EGG,        STR_MAIN_CYAN,      150,   600 },  /* 23 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_U_BOAT,          STR_MAIN_U_BOAT_EGG,       STR_MAIN_WHITE,     100,  1200 },  /* 24 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_BALOON,          STR_MAIN_BALOON_EGG,       STR_MAIN_BLACK,     100,  1000 },  /* 25 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_DREAMIN,         STR_MAIN_DREAMIN_EGG,      STR_MAIN_KHAKI,     100,  1300 },  /* 26 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_BLUME,           STR_MAIN_BLUME_EGG,        STR_MAIN_GREEN,     100,   800 },  /* 27 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_VOLCANO,         STR_MAIN_VOLCANO_EGG,      STR_MAIN_OCHER,     100,  1600 },  /* 28 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_CYCLONE,         STR_MAIN_CYCLONE_EGG,      STR_MAIN_BROWN,     100,   800 },  /* 29 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_MANOEVA,         STR_MAIN_MANOEVA_EGG,      STR_MAIN_UMBER,     100,   800 },  /* 30 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_BARONG,          STR_MAIN_BARONG_EGG,       STR_MAIN_SIENNA,    100,  8000 },  /* 31 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_PICKET,          STR_MAIN_PICKET_EGG,       STR_MAIN_VIOLET,    100,  2400 },  /* 32 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_KRAKEN,          STR_MAIN_KRAKEN_EGG,       STR_MAIN_PURPLE,    100,  2200 },  /* 33 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_WEADOG,          STR_MAIN_WEADOG_EGG,       STR_MAIN_INDIGO,    100,  3000 },  /* 34 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_STEALTH,         STR_MAIN_STEALTH_EGG,      STR_MAIN_YELLOW,    100,  3600 },  /* 35 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_VIPER,           STR_MAIN_VIPER_EGG,        STR_MAIN_ORANGE,    100,  4500 },  /* 36 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_NAPLASS,         STR_MAIN_NAPLASS_EGG,      STR_MAIN_RUSSET,    100,  7600 },  /* 37 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_ZU,              STR_MAIN_ZU_EGG,           STR_MAIN_MAGENTA,   100,  5000 },  /* 38 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_MANDARA,         STR_MAIN_MANDARA_EGG,      STR_MAIN_SCARLET,   100,  5000 },  /* 39 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_KILLER,          STR_MAIN_KILLER_EGG,       STR_MAIN_BURGUNDY,  100,  8000 },  /* 40 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GARUDA,          STR_MAIN_GARUDA_EGG,       STR_MAIN_PURPLISH,  100,  2400 },  /* 41 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GLACIER,         STR_MAIN_GLACIER_EGG,      STR_MAIN_LAVENDER,  100, 12000 },  /* 42 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_TYRANT,          STR_MAIN_TYRANT_EGG,       STR_MAIN_CINNABAR,  100, 10000 },  /* 43 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GOLEM,           STR_MAIN_GOLEM_EGG,        STR_MAIN_VERMILION, 100, 23000 },  /* 44 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_MAXIMUM,         STR_MAIN_MAXIMUM_EGG,      STR_MAIN_RAINBOW,   100, 35000 },  /* 45 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_FROG,            NULL,                      NULL,               100,   100 },  /* 46 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_SALAMAN,         NULL,                      NULL,               100,   100 },  /* 47 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GUY,             NULL,                      NULL,               100,   100 },  /* 48 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_GHOSH,           NULL,                      NULL,               100,   100 },  /* 49 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_SELFI,           NULL,                      NULL,               100,   100 },  /* 50 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_FIRE_WALL,       NULL,                      NULL,               100,   100 },  /* 51 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_ICE_WALL,        NULL,                      NULL,               100,   100 },  /* 52 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_STORM_WALL,      NULL,                      NULL,               100,   100 },  /* 53 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_ROCK,            NULL,                      NULL,               100,   100 },  /* 54 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_BELDO,           NULL,                      NULL,               100,   100 },  /* 55 */
    { 0x10, 0x00, 0x0C, 0x00, STR_MAIN_BELDO,           NULL,                      NULL,               100,   100 },  /* 56 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 57 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 58 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 59 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 60 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 61 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 62 */
    { 0x10, 0x00, 0x0C, 0x00, NULL,                     NULL,                      NULL,               100,   100 },  /* 63 */
};
