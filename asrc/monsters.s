# The monster/spell/character string pool from main.bin, rom
# 0x20800..0x21268: 134 NUL-terminated strings in full-width Shift-JIS, each
# padded with zeros to the next 4-byte boundary. That is exactly what
# `.shiftjisz` emits -- the string, a NUL, then `.balign 4, 0` -- so every
# entry but the last is one directive.
#
# Full-width is how the US release renders Latin text in the Japanese font:
# 0x8260-0x8279 are A-Z, 0x8281-0x829A are a-z, 0x8140 is space, 0x8144 a
# period. One entry, 0x20DB0, spells "U-BOAT" with 0x817C, a full-width MINUS
# SIGN (U+2212) rather than a hyphen; `azure shiftjis` round-trips it, which
# was checked for all 134 strings before this file was written.
#
# WHAT IT IS. From 0x208A4 on, the pool runs in groups of three: an egg
# description, the monster NAME in capitals, then a colour in lower case. The
# grouping is not rigid -- 0x20E88 BLOCK and 0x20EB0 METAL share the one "sky
# blue" that follows them, and so do UNICORN/UNIVERN, CLOWN/DEATH,
# NYUEL/BATTNEL, ARACHNE/ASHRA, SNOWMAN/SABER, GRIFFON/GRINEUT, FLAME/IFRIT
# and DRAGON/KEWNE. The eleven entries before 0x208A4 are not in the pattern
# at all: four character names (Beldo, Selfi, Ghosh, Guy), the three wall
# spells, Salaman, FROG, and two colours.
#
# THE LAST ENTRY'S PADDING IS NOT ZEROS. "Ultimate" at 0x21254 ends with its
# NUL at 0x21264, and 0x21265..0x21268 is C4 0D 0E -- the original toolchain's
# buffer showing through where it padded the pool up to the 4-aligned table
# that follows. It is the fifth instance of that in the project and the first
# found outside a rodata/text boundary; docs/owning-data.md 4.8 has the other
# four. So this entry is `.shiftjis` plus the bytes, not `.shiftjisz`.
#
# HOW IT WAS BUILT. One `hrodata` subsegment replaced 17 one-string
# `shiftjis` segments covering 0x20800..0x20940 plus the 2,368-byte
# `[0x20940, bin, str/main/pool_tail]` after them -- 18 segment blocks in the
# linker script for 2.6 KB of text, two thirds of it never decoded. Sixteen of
# the 17 old symbol names come out of the rule below unchanged. The
# seventeenth was WRONG, and is corrected here: the old
# `[0x20904, shiftjis, str/main/cinnabar]` spanned 32 bytes, which is TWO
# strings, and put STR_MAIN_CINNABAR on the first of them. 0x20904 is GOLEM
# and 0x20910 is cinnabar; each has its own label now.
#
# Names are "NFKC to ASCII, strip a leading `A `/`An ` and a trailing ` egg.`
# down to `<NAME>_EGG`, non-alphanumerics to `_`, upper case". To regenerate
# the bytes from the ROM rather than editing by hand, the walk is: split on
# the first NUL, then expect the next entry at the next 4-aligned offset.

.include "macro.inc"

.section .rodata

/* 20800 */
dlabel STR_MAIN_BELDO
    .shiftjisz "Ｂｅｌｄｏ"

/* 2080C */
dlabel STR_MAIN_ROCK
    .shiftjisz "ｒｏｃｋ"

/* 20818 */
dlabel STR_MAIN_STORM_WALL
    .shiftjisz "ｓｔｏｒｍ　ｗａｌｌ"

/* 20830 */
dlabel STR_MAIN_ICE_WALL
    .shiftjisz "ｉｃｅ　ｗａｌｌ"

/* 20844 */
dlabel STR_MAIN_FIRE_WALL
    .shiftjisz "ｆｉｒｅ　ｗａｌｌ"

/* 20858 */
dlabel STR_MAIN_SELFI
    .shiftjisz "Ｓｅｌｆｉ"

/* 20864 */
dlabel STR_MAIN_GHOSH
    .shiftjisz "Ｇｈｏｓｈ"

/* 20870 */
dlabel STR_MAIN_GUY
    .shiftjisz "Ｇｕｙ"

/* 20878 */
dlabel STR_MAIN_SALAMAN
    .shiftjisz "Ｓａｌａｍａｎ"

/* 20888 */
dlabel STR_MAIN_FROG
    .shiftjisz "ＦＲＯＧ"

/* 20894 */
dlabel STR_MAIN_RAINBOW
    .shiftjisz "ｒａｉｎｂｏｗ"

/* 208A4 */
dlabel STR_MAIN_MAXIMUM_EGG
    .shiftjisz "Ａ　ＭＡＸＩＭＵＭ　ｅｇｇ．"

/* 208C4 */
dlabel STR_MAIN_MAXIMUM
    .shiftjisz "ＭＡＸＩＭＵＭ"

/* 208D4 */
dlabel STR_MAIN_VERMILION
    .shiftjisz "ｖｅｒｍｉｌｉｏｎ"

/* 208E8 */
dlabel STR_MAIN_GOLEM_EGG
    .shiftjisz "Ａ　ＧＯＬＥＭ　ｅｇｇ．"

/* 20904 */
dlabel STR_MAIN_GOLEM
    .shiftjisz "ＧＯＬＥＭ"

/* 20910 */
dlabel STR_MAIN_CINNABAR
    .shiftjisz "ｃｉｎｎａｂａｒ"

/* 20924 */
dlabel STR_MAIN_TYRANT_EGG
    .shiftjisz "Ａ　ＴＹＲＡＮＴ　ｅｇｇ．"

/* 20940 */
dlabel STR_MAIN_TYRANT
    .shiftjisz "ＴＹＲＡＮＴ"

/* 20950 */
dlabel STR_MAIN_LAVENDER
    .shiftjisz "ｌａｖｅｎｄｅｒ"

/* 20964 */
dlabel STR_MAIN_GLACIER_EGG
    .shiftjisz "Ａ　ＧＬＡＣＩＥＲ　ｅｇｇ．"

/* 20984 */
dlabel STR_MAIN_GLACIER
    .shiftjisz "ＧＬＡＣＩＥＲ"

/* 20994 */
dlabel STR_MAIN_PURPLISH
    .shiftjisz "ｐｕｒｐｌｉｓｈ"

/* 209A8 */
dlabel STR_MAIN_GARUDA_EGG
    .shiftjisz "Ａ　ＧＡＲＵＤＡ　ｅｇｇ．"

/* 209C4 */
dlabel STR_MAIN_GARUDA
    .shiftjisz "ＧＡＲＵＤＡ"

/* 209D4 */
dlabel STR_MAIN_BURGUNDY
    .shiftjisz "ｂｕｒｇｕｎｄｙ"

/* 209E8 */
dlabel STR_MAIN_KILLER_EGG
    .shiftjisz "Ａ　ＫＩＬＬＥＲ　ｅｇｇ．"

/* 20A04 */
dlabel STR_MAIN_KILLER
    .shiftjisz "ＫＩＬＬＥＲ"

/* 20A14 */
dlabel STR_MAIN_SCARLET
    .shiftjisz "ｓｃａｒｌｅｔ"

/* 20A24 */
dlabel STR_MAIN_MANDARA_EGG
    .shiftjisz "Ａ　ＭＡＮＤＡＲＡ　ｅｇｇ．"

/* 20A44 */
dlabel STR_MAIN_MANDARA
    .shiftjisz "ＭＡＮＤＡＲＡ"

/* 20A54 */
dlabel STR_MAIN_MAGENTA
    .shiftjisz "ｍａｇｅｎｔａ"

/* 20A64 */
dlabel STR_MAIN_ZU_EGG
    .shiftjisz "Ａ　ＺＵ　ｅｇｇ．"

/* 20A78 */
dlabel STR_MAIN_ZU
    .shiftjisz "ＺＵ"

/* 20A80 */
dlabel STR_MAIN_RUSSET
    .shiftjisz "ｒｕｓｓｅｔ"

/* 20A90 */
dlabel STR_MAIN_NAPLASS_EGG
    .shiftjisz "Ａ　ＮＡＰＬＡＳＳ　ｅｇｇ．"

/* 20AB0 */
dlabel STR_MAIN_NAPLASS
    .shiftjisz "ＮＡＰＬＡＳＳ"

/* 20AC0 */
dlabel STR_MAIN_ORANGE
    .shiftjisz "ｏｒａｎｇｅ"

/* 20AD0 */
dlabel STR_MAIN_VIPER_EGG
    .shiftjisz "Ａ　ＶＩＰＥＲ　ｅｇｇ．"

/* 20AEC */
dlabel STR_MAIN_VIPER
    .shiftjisz "ＶＩＰＥＲ"

/* 20AF8 */
dlabel STR_MAIN_YELLOW
    .shiftjisz "ｙｅｌｌｏｗ"

/* 20B08 */
dlabel STR_MAIN_STEALTH_EGG
    .shiftjisz "Ａ　ＳＴＥＡＬＴＨ　ｅｇｇ．"

/* 20B28 */
dlabel STR_MAIN_STEALTH
    .shiftjisz "ＳＴＥＡＬＴＨ"

/* 20B38 */
dlabel STR_MAIN_INDIGO
    .shiftjisz "ｉｎｄｉｇｏ"

/* 20B48 */
dlabel STR_MAIN_WEADOG_EGG
    .shiftjisz "Ａ　ＷＥＡＤＯＧ　ｅｇｇ．"

/* 20B64 */
dlabel STR_MAIN_WEADOG
    .shiftjisz "ＷＥＡＤＯＧ"

/* 20B74 */
dlabel STR_MAIN_PURPLE
    .shiftjisz "ｐｕｒｐｌｅ"

/* 20B84 */
dlabel STR_MAIN_KRAKEN_EGG
    .shiftjisz "Ａ　ＫＲＡＫＥＮ　ｅｇｇ．"

/* 20BA0 */
dlabel STR_MAIN_KRAKEN
    .shiftjisz "ＫＲＡＫＥＮ"

/* 20BB0 */
dlabel STR_MAIN_VIOLET
    .shiftjisz "ｖｉｏｌｅｔ"

/* 20BC0 */
dlabel STR_MAIN_PICKET_EGG
    .shiftjisz "Ａ　ＰＩＣＫＥＴ　ｅｇｇ．"

/* 20BDC */
dlabel STR_MAIN_PICKET
    .shiftjisz "ＰＩＣＫＥＴ"

/* 20BEC */
dlabel STR_MAIN_SIENNA
    .shiftjisz "ｓｉｅｎｎａ"

/* 20BFC */
dlabel STR_MAIN_BARONG_EGG
    .shiftjisz "Ａ　ＢＡＲＯＮＧ　ｅｇｇ．"

/* 20C18 */
dlabel STR_MAIN_BARONG
    .shiftjisz "ＢＡＲＯＮＧ"

/* 20C28 */
dlabel STR_MAIN_UMBER
    .shiftjisz "ｕｍｂｅｒ"

/* 20C34 */
dlabel STR_MAIN_MANOEVA_EGG
    .shiftjisz "Ａ　ＭＡＮＯＥＶＡ　ｅｇｇ．"

/* 20C54 */
dlabel STR_MAIN_MANOEVA
    .shiftjisz "ＭＡＮＯＥＶＡ"

/* 20C64 */
dlabel STR_MAIN_BROWN
    .shiftjisz "ｂｒｏｗｎ"

/* 20C70 */
dlabel STR_MAIN_CYCLONE_EGG
    .shiftjisz "Ａ　ＣＹＣＬＯＮＥ　ｅｇｇ．"

/* 20C90 */
dlabel STR_MAIN_CYCLONE
    .shiftjisz "ＣＹＣＬＯＮＥ"

/* 20CA0 */
dlabel STR_MAIN_OCHER
    .shiftjisz "ｏｃｈｅｒ"

/* 20CAC */
dlabel STR_MAIN_VOLCANO_EGG
    .shiftjisz "Ａ　ＶＯＬＣＡＮＯ　ｅｇｇ．"

/* 20CCC */
dlabel STR_MAIN_VOLCANO
    .shiftjisz "ＶＯＬＣＡＮＯ"

/* 20CDC */
dlabel STR_MAIN_GREEN
    .shiftjisz "ｇｒｅｅｎ"

/* 20CE8 */
dlabel STR_MAIN_BLUME_EGG
    .shiftjisz "Ａ　ＢＬＵＭＥ　ｅｇｇ．"

/* 20D04 */
dlabel STR_MAIN_BLUME
    .shiftjisz "ＢＬＵＭＥ"

/* 20D10 */
dlabel STR_MAIN_KHAKI
    .shiftjisz "ｋｈａｋｉ"

/* 20D1C */
dlabel STR_MAIN_DREAMIN_EGG
    .shiftjisz "Ａ　ＤＲＥＡＭＩＮ　ｅｇｇ．"

/* 20D3C */
dlabel STR_MAIN_DREAMIN
    .shiftjisz "ＤＲＥＡＭＩＮ"

/* 20D4C */
dlabel STR_MAIN_BLACK
    .shiftjisz "ｂｌａｃｋ"

/* 20D58 */
dlabel STR_MAIN_BALOON_EGG
    .shiftjisz "Ａ　ＢＡＬＯＯＮ　ｅｇｇ．"

/* 20D74 */
dlabel STR_MAIN_BALOON
    .shiftjisz "ＢＡＬＯＯＮ"

/* 20D84 */
dlabel STR_MAIN_WHITE
    .shiftjisz "ｗｈｉｔｅ"

/* 20D90 */
dlabel STR_MAIN_U_BOAT_EGG
    .shiftjisz "Ａｎ　Ｕ−ＢＯＡＴ　ｅｇｇ．"

/* 20DB0 */
dlabel STR_MAIN_U_BOAT
    .shiftjisz "Ｕ−ＢＯＡＴ"

/* 20DC0 */
dlabel STR_MAIN_CYAN
    .shiftjisz "ｃｙａｎ"

/* 20DCC */
dlabel STR_MAIN_NOISE_EGG
    .shiftjisz "Ａ　ＮＯＩＳＥ　ｅｇｇ．"

/* 20DE8 */
dlabel STR_MAIN_NOISE
    .shiftjisz "ＮＯＩＳＥ"

/* 20DF4 */
dlabel STR_MAIN_AQUA_BLUE
    .shiftjisz "ａｑｕａ　ｂｌｕｅ"

/* 20E08 */
dlabel STR_MAIN_TROLL_EGG
    .shiftjisz "Ａ　ＴＲＯＬＬ　ｅｇｇ．"

/* 20E24 */
dlabel STR_MAIN_TROLL
    .shiftjisz "ＴＲＯＬＬ"

/* 20E30 */
dlabel STR_MAIN_DEEP_BLUE
    .shiftjisz "ｄｅｅｐ　ｂｌｕｅ"

/* 20E44 */
dlabel STR_MAIN_PULUNPA_EGG
    .shiftjisz "Ａ　ＰＵＬＵＮＰＡ　ｅｇｇ．"

/* 20E64 */
dlabel STR_MAIN_PULUNPA
    .shiftjisz "ＰＵＬＵＮＰＡ"

/* 20E74 */
dlabel STR_MAIN_SEA_BLUE
    .shiftjisz "ｓｅａ　ｂｌｕｅ"

/* 20E88 */
dlabel STR_MAIN_BLOCK_EGG
    .shiftjisz "Ａ　ＢＬＯＣＫ　ｅｇｇ．"

/* 20EA4 */
dlabel STR_MAIN_BLOCK
    .shiftjisz "ＢＬＯＣＫ"

/* 20EB0 */
dlabel STR_MAIN_METAL_EGG
    .shiftjisz "Ａ　ＭＥＴＡＬ　ｅｇｇ．"

/* 20ECC */
dlabel STR_MAIN_METAL
    .shiftjisz "ＭＥＴＡＬ"

/* 20ED8 */
dlabel STR_MAIN_SKY_BLUE
    .shiftjisz "ｓｋｙ　ｂｌｕｅ"

/* 20EEC */
dlabel STR_MAIN_UNICORN_EGG
    .shiftjisz "Ａｎ　ＵＮＩＣＯＲＮ　ｅｇｇ．"

/* 20F0C */
dlabel STR_MAIN_UNICORN
    .shiftjisz "ＵＮＩＣＯＲＮ"

/* 20F1C */
dlabel STR_MAIN_UNIVERN_EGG
    .shiftjisz "Ａｎ　ＵＮＩＶＥＲＮ　ｅｇｇ．"

/* 20F3C */
dlabel STR_MAIN_UNIVERN
    .shiftjisz "ＵＮＩＶＥＲＮ"

/* 20F4C */
dlabel STR_MAIN_BLUE
    .shiftjisz "ｂｌｕｅ"

/* 20F58 */
dlabel STR_MAIN_CLOWN_EGG
    .shiftjisz "Ａ　ＣＬＯＷＮ　ｅｇｇ．"

/* 20F74 */
dlabel STR_MAIN_CLOWN
    .shiftjisz "ＣＬＯＷＮ"

/* 20F80 */
dlabel STR_MAIN_DEATH_EGG
    .shiftjisz "Ａ　ＤＥＡＴＨ　ｅｇｇ．"

/* 20F9C */
dlabel STR_MAIN_DEATH
    .shiftjisz "ＤＥＡＴＨ"

/* 20FA8 */
dlabel STR_MAIN_PALE_PINK
    .shiftjisz "ｐａｌｅ　ｐｉｎｋ"

/* 20FBC */
dlabel STR_MAIN_NYUEL_EGG
    .shiftjisz "Ａ　ＮＹＵＥＬ　ｅｇｇ．"

/* 20FD8 */
dlabel STR_MAIN_NYUEL
    .shiftjisz "ＮＹＵＥＬ"

/* 20FE4 */
dlabel STR_MAIN_BATTNEL_EGG
    .shiftjisz "Ａ　ＢＡＴＴＮＥＬ　ｅｇｇ．"

/* 21004 */
dlabel STR_MAIN_BATTNEL
    .shiftjisz "ＢＡＴＴＮＥＬ"

/* 21014 */
dlabel STR_MAIN_PINK
    .shiftjisz "ｐｉｎｋ"

/* 21020 */
dlabel STR_MAIN_ARACHNE_EGG
    .shiftjisz "Ａｎ　ＡＲＡＣＨＮＥ　ｅｇｇ．"

/* 21040 */
dlabel STR_MAIN_ARACHNE
    .shiftjisz "ＡＲＡＣＨＮＥ"

/* 21050 */
dlabel STR_MAIN_ASHRA_EGG
    .shiftjisz "Ａｎ　ＡＳＨＲＡ　ｅｇｇ．"

/* 2106C */
dlabel STR_MAIN_ASHRA
    .shiftjisz "ＡＳＨＲＡ"

/* 21078 */
dlabel STR_MAIN_DARK_GRAY
    .shiftjisz "ｄａｒｋ　ｇｒａｙ"

/* 2108C */
dlabel STR_MAIN_SNOWMAN_EGG
    .shiftjisz "Ａ　ＳＮＯＷＭＡＮ　ｅｇｇ．"

/* 210AC */
dlabel STR_MAIN_SNOWMAN
    .shiftjisz "ＳＮＯＷＭＡＮ"

/* 210BC */
dlabel STR_MAIN_SABER_EGG
    .shiftjisz "Ａ　ＳＡＢＥＲ　ｅｇｇ．"

/* 210D8 */
dlabel STR_MAIN_SABER
    .shiftjisz "ＳＡＢＥＲ"

/* 210E4 */
dlabel STR_MAIN_GRAY
    .shiftjisz "ｇｒａｙ"

/* 210F0 */
dlabel STR_MAIN_GRIFFON_EGG
    .shiftjisz "Ａ　ＧＲＩＦＦＯＮ　ｅｇｇ．"

/* 21110 */
dlabel STR_MAIN_GRIFFON
    .shiftjisz "ＧＲＩＦＦＯＮ"

/* 21120 */
dlabel STR_MAIN_GRINEUT_EGG
    .shiftjisz "Ａ　ＧＲＩＮＥＵＴ　ｅｇｇ．"

/* 21140 */
dlabel STR_MAIN_GRINEUT
    .shiftjisz "ＧＲＩＮＥＵＴ"

/* 21150 */
dlabel STR_MAIN_WINE_RED
    .shiftjisz "ｗｉｎｅ　ｒｅｄ"

/* 21164 */
dlabel STR_MAIN_FLAME_EGG
    .shiftjisz "Ａ　ＦＬＡＭＥ　ｅｇｇ．"

/* 21180 */
dlabel STR_MAIN_FLAME
    .shiftjisz "ＦＬＡＭＥ"

/* 2118C */
dlabel STR_MAIN_IFRIT_EGG
    .shiftjisz "Ａ　ＩＦＲＩＴ　ｅｇｇ．"

/* 211A8 */
dlabel STR_MAIN_IFRIT
    .shiftjisz "ＩＦＲＩＴ"

/* 211B4 */
dlabel STR_MAIN_RED
    .shiftjisz "ｒｅｄ"

/* 211BC */
dlabel STR_MAIN_KID_EGG
    .shiftjisz "Ａ　ＫＩＤ　ｅｇｇ．"

/* 211D4 */
dlabel STR_MAIN_KID
    .shiftjisz "ＫＩＤ"

/* 211DC */
dlabel STR_MAIN_DRAGON_EGG
    .shiftjisz "Ａ　ＤＲＡＧＯＮ　ｅｇｇ．"

/* 211F8 */
dlabel STR_MAIN_DRAGON
    .shiftjisz "ＤＲＡＧＯＮ"

/* 21208 */
dlabel STR_MAIN_KEWNE_EGG
    .shiftjisz "Ａ　ＫＥＷＮＥ　ｅｇｇ．"

/* 21224 */
dlabel STR_MAIN_KEWNE
    .shiftjisz "ＫＥＷＮＥ"

/* 21230 */
dlabel STR_MAIN_THE_ULTIMATE_EGG
    .shiftjisz "Ｔｈｅ　ｕｌｔｉｍａｔｅ　ｅｇｇ．"

/* 21254 */
dlabel STR_MAIN_ULTIMATE
    .shiftjis "Ｕｌｔｉｍａｔｅ"
    .byte 0x00, 0xC4, 0x0D, 0x0E
