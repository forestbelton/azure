# The item/spell description-and-name pool: the WHOLE run, hand-owned.
#
# Covers rom 0x002810..0x00660C (vram 0x8002F010..0x80032E0C). That line is
# not decoration: splat/ext/hrodata.py reads it back on every split and warns
# if the `hrodata` subsegment in splat/slus_006.14.yaml no longer matches, since
# it will never regenerate this file.
#
# 405 strings in ROM order, 0x2810..0x660C, replacing 322 one-per-string
# `shiftjis` subsegments (0x2810..0x57F8, which ended at str/item/jp_57f0) and
# the 83 further strings that `str_item_tail` held as one blob. Entries
# alternate a description with the short name it belongs to; the Japanese
# UNIDENTIFIED-item names, never translated for the US release, are still among
# them. The run ends at 0x660C: the `s16 {1, 2, 3, 0}` table that follows is
# not text and is split separately as `tbl_660C`, since nothing about it
# belongs to a string table.
#
# Every string is NUL-terminated and padded to the next 4-byte boundary, so the
# zero run after each is `4 - (len % 4)` bytes: 1, 2, 3 or 4, never none.
# `.shiftjisz` emits exactly that (src/azure/commands/shiftjis.py), which is why
# there is no `.fill` anywhere here and nothing to close at the end of the file.
# Measured over the whole run: 405 of 405 entries obey it.
#
# Symbol names are the yaml subsegment names the 322 `shiftjis` entries used, so
# nothing splat emitted before changed name. The tail's 83 are the same rule
# applied to the text: NFKC to ASCII, lowercase, non-alphanumerics to `_`,
# truncated to 36, `_2` on a clash, `jp_<offset>` when that comes out empty.
# That rule reproduces 297 of the 322 existing names exactly, the other 25
# being those two conventions.
#
# One label here is not a string name and must survive any regeneration: the
# alias D_8002F010 on the first entry, which `game_data` reaches by that name.
# See the comment there.
#
# To regenerate from the ROM instead of editing by hand, the walk is: split on
# the first NUL, skip the zero run, expect the next entry 4-aligned.

.include "macro.inc"

.section .rodata

/* 02810 8002F010 */
# D_8002F010 is an ALIAS, and it has to stay. `game_data` holds the item
# record table -- {name pointer, description pointer} pairs -- and the word at
# 0x80072260 reaches this address. Because this range is now scanned (it is
# `hrodata`, not 322 opaque `shiftjis` segments), splat knows a symbol lives
# here and writes that pointer as `.word D_8002F010` instead of a bare
# 0x8002F010, so dropping this label is an undefined reference at link time.
# Every other pointer into the pool is still bare hex, which is why this is the
# only alias in the file.
dlabel D_8002F010
dlabel STR_ITEM_MAKES_IT_POSSIBLE_TO_RETURN_TO_A_TOW
    .shiftjisz "Ｍａｋｅｓ　ｉｔ　ｐｏｓｓｉｂｌｅ　ｔｏ　ｒｅｔｕｒｎ\x0aｔｏ　ａ　ｔｏｗｎ．"

/* 0285C 8002F05C */
dlabel STR_ITEM_WIND
    .shiftjisz "Ｗｉｎｄ"

/* 02868 8002F068 */
dlabel STR_ITEM_RESTORES_STATUS_OF_FAMILIAR_AND_RECO
    .shiftjisz "Ｒｅｓｔｏｒｅｓ　ｓｔａｔｕｓ　ｏｆ　ｆａｍｉｌｉａｒ　ａｎｄ\x0aｒｅｃｏｖｅｒｓ　ＭＰ　＆　ＨＰ　ｔｏ　ｍａｘｉｍｕｍ．"

/* 028E0 8002F0E0 */
dlabel STR_ITEM_WATER
    .shiftjisz "Ｗａｔｅｒ"

/* 028EC 8002F0EC */
dlabel STR_ITEM_SUMMONS_SALAMANDER_WHICH_BURNS_UP_EN
    .shiftjisz "Ｓｕｍｍｏｎｓ　ＳＡＬＡＭＡＮＤＥＲ，　ｗｈｉｃｈ　ｂｕｒｎｓ\x0aｕｐ　ｅｎｅｍｉｅｓ　ｏｎｅ　ａｆｔｅｒ　ａｎｏｔｈｅｒ．"

/* 02968 8002F168 */
dlabel STR_ITEM_FIRE
    .shiftjisz "Ｆｉｒｅ"

/* 02974 8002F174 */
dlabel STR_ITEM_VANISHES_FAMILIAR_AND_THE_PLAYER_ESC
    .shiftjisz "Ｖａｎｉｓｈｅｓ　ｆａｍｉｌｉａｒ，　ａｎｄ　ｔｈｅ\x0aｐｌａｙｅｒ　ｅｓｃａｐｅｓ　ｆｒｏｍ　ｔｈｅ　ｔｏｗｅｒ．"

/* 029E8 8002F1E8 */
dlabel STR_ITEM_OLEEM
    .shiftjisz "Ｏｌｅｅｍ"

/* 029F4 8002F1F4 */
dlabel STR_ITEM_IF_YOU_HAVE_THIS
    .shiftjisz "Ｉｆ　ｙｏｕ　ｈａｖｅ　ｔｈｉｓ．．．．"

/* 02A20 8002F220 */
dlabel STR_ITEM_IT_ABSORBS_DAMAGE_TO_A_CERTAIN_DEGRE
    .shiftjisz "Ｉｔ　ａｂｓｏｒｂｓ　ｄａｍａｇｅ　ｔｏ　ａ　ｃｅｒｔａｉｎ\x0aｄｅｇｒｅｅ，　ｂｕｔ　ｉｆ　ｉｔ　ｂｕｒｓｔｓ．．．．"

/* 02A98 8002F298 */
dlabel STR_ITEM_BALLOON
    .shiftjisz "Ｂａｌｌｏｏｎ"

/* 02AA8 8002F2A8 */
dlabel STR_ITEM_DELICIOUS_MEAT_THAT_MONSTERS_LOVE
    .shiftjisz "Ｄｅｌｉｃｉｏｕｓ　ｍｅａｔ　ｔｈａｔ　ｍｏｎｓｔｅｒｓ\x0aｌｏｖｅ．"

/* 02AEC 8002F2EC */
dlabel STR_ITEM_MEAT
    .shiftjisz "Ｍｅａｔ"

/* 02AF8 8002F2F8 */
dlabel STR_ITEM_A_BOX_CONTAINING_SOMETHING_VALUABLE
    .shiftjisz "Ａ　ｂｏｘ　ｃｏｎｔａｉｎｉｎｇ　ｓｏｍｅｔｈｉｎｇ\x0aｖａｌｕａｂｌｅ．"

/* 02B40 8002F340 */
dlabel STR_ITEM_A_BLUE_MAGIC_COLLAR_TO_MAKE_MONSTER
    .shiftjisz "Ａ　ｂｌｕｅ　ｍａｇｉｃ　ｃｏｌｌａｒ　ｔｏ　ｍａｋｅ\x0aｍｏｎｓｔｅｒ　ｙｏｕｒ　ｆａｍｉｌｉａｒ．"

/* 02BA4 8002F3A4 */
dlabel STR_ITEM_B_COLLAR
    .shiftjisz "Ｂ．　Ｃｏｌｌａｒ"

/* 02BB8 8002F3B8 */
dlabel STR_ITEM_WILL_SHRINK_AN_OPPONENT
    .shiftjisz "Ｗｉｌｌ　ｓｈｒｉｎｋ　ａｎ　ｏｐｐｏｎｅｎｔ．"

/* 02BEC 8002F3EC */
dlabel STR_ITEM_MALLET
    .shiftjisz "Ｍａｌｌｅｔ"

/* 02BFC 8002F3FC */
dlabel STR_ITEM_BUNDLED_MEDICINAL_IT_S_POWERFUL_THAN
    .shiftjisz "Ｂｕｎｄｌｅｄ　Ｍｅｄｉｃｉｎａｌ．　Ｉｔ’ｓ\x0aｐｏｗｅｒｆｕｌ　ｔｈａｎ　Ｍｅｄｉｃｉｎａｌ　Ｈｅｒｂ．"

/* 02C68 8002F468 */
dlabel STR_ITEM_ROPE
    .shiftjisz "Ｒｏｐｅ"

/* 02C74 8002F474 */
dlabel STR_ITEM_IT_S_QUACK_THE_DUCK_CUTE_LITTLE_THIN
    .shiftjisz "Ｉｔ’ｓ　Ｑｕａｃｋ　ｔｈｅ　Ｄｕｃｋ．\x0aＣｕｔｅ　ｌｉｔｔｌｅ　ｔｈｉｎｇ．"

/* 02CC4 8002F4C4 */
dlabel STR_ITEM_DUCK
    .shiftjisz "Ｄｕｃｋ"

/* 02CD0 8002F4D0 */
dlabel STR_ITEM_WHEN_YOU_WANT_TO_EXPRESS_PASSION_RUB
    .shiftjisz "Ｗｈｅｎ　ｙｏｕ　ｗａｎｔ　ｔｏ　ｅｘｐｒｅｓｓ\x0aｐａｓｓｉｏｎ．　Ｒｕｂｙ　ｉｓ　ｔｈｅ　ｇｅｍ．"

/* 02D34 8002F534 */
dlabel STR_ITEM_RING
    .shiftjisz "Ｒｉｎｇ"

/* 02D40 8002F540 */
dlabel STR_ITEM_THE_KIND_THAT_GIRLS_LIKE
    .shiftjisz "Ｔｈｅ　ｋｉｎｄ　ｔｈａｔ　ｇｉｒｌｓ　ｌｉｋｅ．"

/* 02D74 8002F574 */
dlabel STR_ITEM_HAND_BAG
    .shiftjisz "Ｈａｎｄ　Ｂａｇ"

/* 02D88 8002F588 */
dlabel STR_ITEM_THE_ONE_GHOSH_OFTEN_GIVES_TO_GIRLS
    .shiftjisz "Ｔｈｅ　ｏｎｅ　Ｇｈｏｓｈ　ｏｆｔｅｎ　ｇｉｖｅｓ　ｔｏ\x0aｇｉｒｌｓ．"

/* 02DD0 8002F5D0 */
dlabel STR_ITEM_ROSES
    .shiftjisz "Ｒｏｓｅｓ"

/* 02DDC 8002F5DC */
dlabel STR_ITEM_BEAUTY_CREAM_THAT_MAKES_YOUR_SKIN_RE
    .shiftjisz "Ｂｅａｕｔｙ　ｃｒｅａｍ　ｔｈａｔ　ｍａｋｅｓ　ｙｏｕｒ\x0aｓｋｉｎ　ｒｅａｌｌｙ　ｓｍｏｏｔｈ．　"

/* 02E40 8002F640 */
dlabel STR_ITEM_CREAM
    .shiftjisz "Ｃｒｅａｍ"

/* 02E4C 8002F64C */
dlabel STR_ITEM_CHAPPIE
    .shiftjisz "Ｃｈａｐｐｉｅ"

/* 02E5C 8002F65C */
dlabel STR_ITEM_A_WORN_OUT_BLUE_CAPE_NOT_LIKELY_TO_F
    .shiftjisz "Ａ　ｗｏｒｎ−ｏｕｔ　ｂｌｕｅ　ｃａｐｅ．\x0aＮｏｔ　ｌｉｋｅｌｙ　ｔｏ　ｆｉｔ　ｍｅ．"

/* 02EB4 8002F6B4 */
dlabel STR_ITEM_B_CAPE
    .shiftjisz "Ｂ．　Ｃａｐｅ"

/* 02EC4 8002F6C4 */
dlabel STR_ITEM_EYE_GLASSES
    .shiftjisz "Ｅｙｅ　Ｇｌａｓｓｅｓ"

/* 02EDC 8002F6DC */
dlabel STR_ITEM_A_SMALL_MUSIC_BOX_WHAT_PLAYS_SOOTHIN
    .shiftjisz "Ａ　ｓｍａｌｌ　ｍｕｓｉｃ　ｂｏｘ　ｗｈａｔ　ｐｌａｙｓ　\x0aｓｏｏｔｈｉｎｇ　ｍｕｓｉｃ．"

/* 02F38 8002F738 */
dlabel STR_ITEM_MUSIC_BOX
    .shiftjisz "Ｍｕｓｉｃ　Ｂｏｘ"

/* 02F4C 8002F74C */
dlabel STR_ITEM_DELICIOUS_OCTOPUS_DUMPLINGS
    .shiftjisz "Ｄｅｌｉｃｉｏｕｓ　Ｏｃｔｏｐｕｓ　Ｄｕｍｐｌｉｎｇｓ．"

/* 02F88 8002F788 */
dlabel STR_ITEM_DUMPLINGS
    .shiftjisz "Ｄｕｍｐｌｉｎｇｓ"

/* 02F9C 8002F79C */
dlabel STR_ITEM_A_WATER_CLEANSING_MEDAL_STOLEN_FROM
    .shiftjisz "Ａ　ｗａｔｅｒ　ｃｌｅａｎｓｉｎｇ　ｍｅｄａｌ　ｓｔｏｌｅｎ\x0aｆｒｏｍ　ｔｈｅ　ｐｏｏｌ．"

/* 02FF8 8002F7F8 */
dlabel STR_ITEM_W_MEDAL
    .shiftjisz "Ｗ．　Ｍｅｄａｌ"

/* 0300C 8002F80C */
dlabel STR_ITEM_A_POT_CONTAINING_AMPLE_OIL_TO_USE_FO
    .shiftjisz "Ａ　ｐｏｔ　ｃｏｎｔａｉｎｉｎｇ　ａｍｐｌｅ　ｏｉｌ　ｔｏ\x0aｕｓｅ　ｆｏｒ　ａ　ｗｉｎｄｍｉｌｌ．"

/* 03070 8002F870 */
dlabel STR_ITEM_GURU_S_POT
    .shiftjisz "Ｇｕｒｕ’ｓ　Ｐｏｔ"

/* 03088 8002F888 */
dlabel STR_ITEM_GOLD_COINS_HIGHLY_VALUABLE
    .shiftjisz "Ｇｏｌｄ　ｃｏｉｎｓ．\x0aＨｉｇｈｌｙ　ｖａｌｕａｂｌｅ．"

/* 030C0 8002F8C0 */
dlabel STR_ITEM_GOLD
    .shiftjisz "Ｇｏｌｄ"

/* 030CC 8002F8CC */
dlabel STR_ITEM_SILVER_COINS_MODERATELY_VALUABLE
    .shiftjisz "Ｓｉｌｖｅｒ　ｃｏｉｎｓ．\x0aＭｏｄｅｒａｔｅｌｙ　ｖａｌｕａｂｌｅ．"

/* 03110 8002F910 */
dlabel STR_ITEM_SILVER
    .shiftjisz "Ｓｉｌｖｅｒ"

/* 03120 8002F920 */
dlabel STR_ITEM_COPPER_COINS_NOT_VERY_VALUABLE
    .shiftjisz "Ｃｏｐｐｅｒ　ｃｏｉｎｓ．　\x0aＮｏｔ　ｖｅｒｙ　ｖａｌｕａｂｌｅ．"

/* 03164 8002F964 */
dlabel STR_ITEM_COPPER
    .shiftjisz "Ｃｏｐｐｅｒ"

/* 03174 8002F974 */
dlabel STR_ITEM_A_LONG_RANGE_WEAPON_EXCLUSIVE_TO_TRO
    .shiftjisz "Ａ　ｌｏｎｇ−ｒａｎｇｅ　ｗｅａｐｏｎ．\x0aＥｘｃｌｕｓｉｖｅ　ｔｏ　Ｔｒｏｌｌ−ｔｙｐｅｓ．"

/* 031D0 8002F9D0 */
dlabel STR_ITEM_BOW_GUN
    .shiftjisz "Ｂｏｗ　Ｇｕｎ"

/* 031E0 8002F9E0 */
dlabel STR_ITEM_A_MACE_TYPE_CLUB_WEAPON_EXCLUSIVE_TO
    .shiftjisz "Ａ　ｍａｃｅ−ｔｙｐｅ　ｃｌｕｂ　ｗｅａｐｏｎ．\x0aＥｘｃｌｕｓｉｖｅ　ｔｏ　Ｔｒｏｌｌ−ｔｙｐｅｓ．"

/* 03244 8002FA44 */
dlabel STR_ITEM_HAMMER
    .shiftjisz "Ｈａｍｍｅｒ"

/* 03254 8002FA54 */
dlabel STR_ITEM_CAN_T_BE_BLOCKED_BY_A_SHIELD_EXCLUSI
    .shiftjisz "Ｃａｎ’ｔ　ｂｅ　ｂｌｏｃｋｅｄ　ｂｙ　ａ　Ｓｈｉｅｌｄ．\x0aＥｘｃｌｕｓｉｖｅ　ｔｏ　Ｔｒｏｌｌ−ｔｙｐｅｓ．"

/* 032C4 8002FAC4 */
dlabel STR_ITEM_TROLL
    .shiftjisz "Ｔｒｏｌｌ"

/* 032D0 8002FAD0 */
dlabel STR_ITEM_GUY_S_KEEPSAKE_SWORD_SERAPHIM_ATK_8
    .shiftjisz "Ｇｕｙ’ｓ　ｋｅｅｐｓａｋｅ　ｓｗｏｒｄ．\x0aＳｅｒａｐｈｉｍ　ＡＴＫ　８．"

/* 0331C 8002FB1C */
dlabel STR_ITEM_GUY_S_KEEPSAKE_SWORD_SERAPHIM
    .shiftjisz "Ｇｕｙ’ｓ　ｋｅｅｐｓａｋｅ　ｓｗｏｒｄ．\x0aＳｅｒａｐｈｉｍ"

/* 03358 8002FB58 */
dlabel STR_ITEM_SERAPHIM
    .shiftjisz "Ｓｅｒａｐｈｉｍ"

/* 0336C 8002FB6C */
dlabel STR_ITEM_HOLY_SWORD_WITH_DIVINE_PROTECTION_AT
    .shiftjisz "Ｈｏｌｙ　ｓｗｏｒｄ　ｗｉｔｈ　ｄｉｖｉｎｅ\x0aｐｒｏｔｅｃｔｉｏｎ．　ＡＴＫ　７．"

/* 033C0 8002FBC0 */
dlabel STR_ITEM_HOLY
    .shiftjisz "Ｈｏｌｙ"

/* 033CC 8002FBCC */
dlabel STR_ITEM_A_CURSED_SWORD_ATK_10
    .shiftjisz "Ａ　ｃｕｒｓｅｄ　ｓｗｏｒｄ．　ＡＴＫ　１０．"

/* 033FC 8002FBFC */
dlabel STR_ITEM_DARK
    .shiftjisz "Ｄａｒｋ"

/* 03408 8002FC08 */
dlabel STR_ITEM_PREVENTS_OPPONENT_FROM_MULTIPLYING_A
    .shiftjisz "Ｐｒｅｖｅｎｔｓ　ｏｐｐｏｎｅｎｔ　ｆｒｏｍ\x0aｍｕｌｔｉｐｌｙｉｎｇ．　ＡＴＫ　５．"

/* 0345C 8002FC5C */
dlabel STR_ITEM_VITAL
    .shiftjisz "Ｖｉｔａｌ"

/* 03468 8002FC68 */
dlabel STR_ITEM_WIND_GENUS_ATK_5
    .shiftjisz "Ｗｉｎｄ　ｇｅｎｕｓ．　ＡＴＫ　５．"

/* 03490 8002FC90 */
dlabel STR_ITEM_GULFWIND
    .shiftjisz "Ｇｕｌｆｗｉｎｄ"

/* 034A4 8002FCA4 */
dlabel STR_ITEM_WATER_GENUS_ATK_5
    .shiftjisz "Ｗａｔｅｒ　ｇｅｎｕｓ．　ＡＴＫ　５．"

/* 034CC 8002FCCC */
dlabel STR_ITEM_BLIZZARD
    .shiftjisz "Ｂｌｉｚｚａｒｄ"

/* 034E0 8002FCE0 */
dlabel STR_ITEM_FIRE_GENUS_ATK_5
    .shiftjisz "Ｆｉｒｅ　ｇｅｎｕｓ．　ＡＴＫ　５．"

/* 03508 8002FD08 */
dlabel STR_ITEM_MADE_OF_STEEL_ATK_4
    .shiftjisz "Ｍａｄｅ　ｏｆ　ｓｔｅｅｌ．　ＡＴＫ　４．"

/* 03534 8002FD34 */
dlabel STR_ITEM_STEEL
    .shiftjisz "Ｓｔｅｅｌ"

/* 03540 8002FD40 */
dlabel STR_ITEM_MADE_OF_IRON_ATK_3
    .shiftjisz "Ｍａｄｅ　ｏｆ　Ｉｒｏｎ．　ＡＴＫ　３．"

/* 0356C 8002FD6C */
dlabel STR_ITEM_IRON
    .shiftjisz "Ｉｒｏｎ"

/* 03578 8002FD78 */
dlabel STR_ITEM_MADE_OF_COPPER_ATK_2
    .shiftjisz "Ｍａｄｅ　ｏｆ　ｃｏｐｐｅｒ．　ＡＴＫ　２．"

/* 035A8 8002FDA8 */
dlabel STR_ITEM_COATED_WITH_GOLD_ATK_1
    .shiftjisz "Ｃｏａｔｅｄ　ｗｉｔｈ　ｇｏｌｄ．　ＡＴＫ　１．"

/* 035DC 8002FDDC */
dlabel STR_ITEM_SOMETIMES_SEALS_THE_SPECIAL_POWERS_O
    .shiftjisz "Ｓｏｍｅｔｉｍｅｓ　ｓｅａｌｓ　ｔｈｅ　ｓｐｅｃｉａｌ\x0aｐｏｗｅｒｓ　ｏｆ　ｍｏｎｓｔｅｒｓ．　ＡＴＫ　１．"

/* 03648 8002FE48 */
dlabel STR_ITEM_SEAL
    .shiftjisz "Ｓｅａｌ"

/* 03654 8002FE54 */
dlabel STR_ITEM_A_WAND_OF_WIND_ATK_1
    .shiftjisz "Ａ　ｗａｎｄ　ｏｆ　ｗｉｎｄ．　ＡＴＫ　１．"

/* 03684 8002FE84 */
dlabel STR_ITEM_GULF
    .shiftjisz "Ｇｕｌｆ"

/* 03690 8002FE90 */
dlabel STR_ITEM_A_WAND_OF_WATER_ATK_1
    .shiftjisz "Ａ　ｗａｎｄ　ｏｆ　ｗａｔｅｒ．　ＡＴＫ　１．"

/* 036C0 8002FEC0 */
dlabel STR_ITEM_STREAM
    .shiftjisz "Ｓｔｒｅａｍ"

/* 036D0 8002FED0 */
dlabel STR_ITEM_A_WAND_OF_FIRE_ATK_1
    .shiftjisz "Ａ　ｗａｎｄ　ｏｆ　ｆｉｒｅ．　ＡＴＫ　１．"

/* 03700 8002FF00 */
dlabel STR_ITEM_SCARLET
    .shiftjisz "Ｓｃａｒｌｅｔ"

/* 03710 8002FF10 */
dlabel STR_ITEM_SOMETIMES_CHANGES_KILLED_MONSTERS_IN
    .shiftjisz "Ｓｏｍｅｔｉｍｅｓ　ｃｈａｎｇｅｓ　ｋｉｌｌｅｄ\x0aＭｏｎｓｔｅｒｓ　ｉｎｔｏ　ｍｏｎｅｙ．　ＡＴＫ　１．"

/* 03778 8002FF78 */
dlabel STR_ITEM_MONEY
    .shiftjisz "Ｍｏｎｅｙ"

/* 03784 8002FF84 */
dlabel STR_ITEM_SOMETIMES_FREEZES_MONSTER_S_MOBILITY
    .shiftjisz "Ｓｏｍｅｔｉｍｅｓ　ｆｒｅｅｚｅｓ　Ｍｏｎｓｔｅｒ’ｓ\x0aｍｏｂｉｌｉｔｙ．　ＡＴＫ　１．"

/* 037DC 8002FFDC */
dlabel STR_ITEM_PARALYZE
    .shiftjisz "Ｐａｒａｌｙｚｅ"

/* 037F0 8002FFF0 */
dlabel STR_ITEM_RECOVERS_HP_SLIGHTLY_IF_AN_OPPONENT
    .shiftjisz "Ｒｅｃｏｖｅｒｓ　ＨＰ　ｓｌｉｇｈｔｌｙ　ｉｆ　ａｎ\x0aｏｐｐｏｎｅｎｔ　ｉｓ　ｈｉｔ．　ＡＴＫ　１．"

/* 03854 80030054 */
dlabel STR_ITEM_LIFE
    .shiftjisz "Ｌｉｆｅ"

/* 03860 80030060 */
dlabel STR_ITEM_CAN_BE_TEMPERED_WITH_THE_RED_SAND_AT
    .shiftjisz "Ｃａｎ　ｂｅ　ｔｅｍｐｅｒｅｄ　ｗｉｔｈ　ｔｈｅ　\x0aＲｅｄ　Ｓａｎｄ．　ＡＴＫ　１．"

/* 038B4 800300B4 */
dlabel STR_ITEM_TRAINED
    .shiftjisz "Ｔｒａｉｎｅｄ"

/* 038C4 800300C4 */
dlabel STR_ITEM_A_WAND_MADE_OF_WOOD_ATK_1
    .shiftjisz "Ａ　ｗａｎｄ　ｍａｄｅ　ｏｆ　ｗｏｏｄ．　ＡＴＫ　１．"

/* 038FC 800300FC */
dlabel STR_ITEM_WOODEN
    .shiftjisz "Ｗｏｏｄｅｎ"

/* 0390C 8003010C */
dlabel STR_ITEM_SOMETIMES_RETALIATES_WHEN_IT_S_HIT_D
    .shiftjisz "Ｓｏｍｅｔｉｍｅｓ　ｒｅｔａｌｉａｔｅｓ　ｗｈｅｎ　ｉｔ’ｓ\x0aｈｉｔ．　ＤＥＦ　５．"

/* 03960 80030160 */
dlabel STR_ITEM_LIVE
    .shiftjisz "Ｌｉｖｅ"

/* 0396C 8003016C */
dlabel STR_ITEM_WIND_GENUS_SHIELD_DEF_5
    .shiftjisz "Ｗｉｎｄ　ｇｅｎｕｓ　ｓｈｉｅｌｄ．　ＤＥＦ　５．"

/* 039A0 800301A0 */
dlabel STR_ITEM_EARTH
    .shiftjisz "Ｅａｒｔｈ"

/* 039AC 800301AC */
dlabel STR_ITEM_WATER_GENUS_SHIELD_DEF_5
    .shiftjisz "Ｗａｔｅｒ　ｇｅｎｕｓ　ｓｈｉｅｌｄ．　ＤＥＦ　５．"

/* 039E4 800301E4 */
dlabel STR_ITEM_FIRE_GENUS_SHIELD_DEF_5
    .shiftjisz "Ｆｉｒｅ　ｇｅｎｕｓ　ｓｈｉｅｌｄ．　ＤＥＦ　５．"

/* 03A18 80030218 */
dlabel STR_ITEM_SCORCH
    .shiftjisz "Ｓｃｏｒｃｈ"

/* 03A28 80030228 */
dlabel STR_ITEM_A_SHIELD_MADE_OF_DIAMOND_DEF_7
    .shiftjisz "Ａ　ｓｈｉｅｌｄ　ｍａｄｅ　ｏｆ　ｄｉａｍｏｎｄ．\x0aＤＥＦ　７．"

/* 03A68 80030268 */
dlabel STR_ITEM_DIAMOND
    .shiftjisz "Ｄｉａｍｏｎｄ"

/* 03A78 80030278 */
dlabel STR_ITEM_A_VERY_TOUGH_STEEL_SHIELD_DEF_6
    .shiftjisz "Ａ　ｖｅｒｙ　ｔｏｕｇｈ　ｓｔｅｅｌ　ｓｈｉｅｌｄ．\x0aＤＥＦ　６．"

/* 03ABC 800302BC */
dlabel STR_ITEM_AN_IRON_SHIELD_DEF_5
    .shiftjisz "Ａｎ　ｉｒｏｎ　ｓｈｉｅｌｄ．　ＤＥＦ　５．"

/* 03AEC 800302EC */
dlabel STR_ITEM_A_COPPER_SHIELD_DEF_4
    .shiftjisz "Ａ　ｃｏｐｐｅｒ　ｓｈｉｅｌｄ．　ＤＥＦ　４．"

/* 03B1C 8003031C */
dlabel STR_ITEM_SHIELD_POLISHED_INTO_A_MIRROR_DEF_3
    .shiftjisz "Ｓｈｉｅｌｄ　ｐｏｌｉｓｈｅｄ　ｉｎｔｏ　ａ　ｍｉｒｒｏｒ．\x0aＤＥＦ　３．"

/* 03B68 80030368 */
dlabel STR_ITEM_MIRROR
    .shiftjisz "Ｍｉｒｒｏｒ"

/* 03B78 80030378 */
dlabel STR_ITEM_TANNED_LEATHER_SHIELD_DEF_1
    .shiftjisz "Ｔａｎｎｅｄ　ｌｅａｔｈｅｒ　ｓｈｉｅｌｄ．　ＤＥＦ　１．"

/* 03BB4 800303B4 */
dlabel STR_ITEM_LEATHER
    .shiftjisz "Ｌｅａｔｈｅｒ"

/* 03BC4 800303C4 */
dlabel STR_ITEM_WOODEN_SHIELD_DEF_2
    .shiftjisz "Ｗｏｏｄｅｎ　ｓｈｉｅｌｄ．　ＤＥＦ　２．"

/* 03BF0 800303F0 */
dlabel STR_ITEM_WOOD
    .shiftjisz "Ｗｏｏｄ"

/* 03BFC 800303FC */
dlabel STR_ITEM_BELDO
    .shiftjisz "Ｂｅｌｄｏ"

/* 03C08 80030408 */
dlabel STR_ITEM_ROCK
    .shiftjisz "Ｒｏｃｋ"

/* 03C14 80030414 */
dlabel STR_ITEM_WALL_OF_STORM
    .shiftjisz "Ｗａｌｌ　ｏｆ　ｓｔｏｒｍ"

/* 03C30 80030430 */
dlabel STR_ITEM_WALL_OF_ICE
    .shiftjisz "Ｗａｌｌ　ｏｆ　ｉｃｅ"

/* 03C48 80030448 */
dlabel STR_ITEM_WALL_OF_FIRE
    .shiftjisz "Ｗａｌｌ　ｏｆ　ｆｉｒｅ"

/* 03C64 80030464 */
dlabel STR_ITEM_SELFI
    .shiftjisz "Ｓｅｌｆｉ"

/* 03C70 80030470 */
dlabel STR_ITEM_GHOSH
    .shiftjisz "Ｇｈｏｓｈ"

/* 03C7C 8003047C */
dlabel STR_ITEM_SALAMAN
    .shiftjisz "ＳＡＬＡＭＡＮ"

/* 03C8C 8003048C */
dlabel STR_ITEM_FROG_TYPE_FAMILIAR
    .shiftjisz "ＦＲＯＧ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03CB4 800304B4 */
dlabel STR_ITEM_FROG
    .shiftjisz "ＦＲＯＧ"

/* 03CC0 800304C0 */
dlabel STR_ITEM_MAXIMUM_TYPE_FAMILIAR
    .shiftjisz "ＭＡＸＩＭＵＭ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03CF0 800304F0 */
dlabel STR_ITEM_MAXIMUM
    .shiftjisz "ＭＡＸＩＭＵＭ"

/* 03D00 80030500 */
dlabel STR_ITEM_GOLEM_TYPE_FAMILIAR
    .shiftjisz "ＧＯＬＥＭ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03D2C 8003052C */
dlabel STR_ITEM_GOLEM
    .shiftjisz "ＧＯＬＥＭ"

/* 03D38 80030538 */
dlabel STR_ITEM_TYRANT_TYPE_FAMILIAR
    .shiftjisz "ＴＹＲＡＮＴ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03D64 80030564 */
dlabel STR_ITEM_TYRANT
    .shiftjisz "ＴＹＲＡＮＴ"

/* 03D74 80030574 */
dlabel STR_ITEM_GLACIER_TYPE_FAMILIAR
    .shiftjisz "ＧＬＡＣＩＥＲ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03DA4 800305A4 */
dlabel STR_ITEM_GLACIER
    .shiftjisz "ＧＬＡＣＩＥＲ"

/* 03DB4 800305B4 */
dlabel STR_ITEM_GARUDA_TYPE_FAMILIAR
    .shiftjisz "ＧＡＲＵＤＡ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03DE0 800305E0 */
dlabel STR_ITEM_GARUDA
    .shiftjisz "ＧＡＲＵＤＡ"

/* 03DF0 800305F0 */
dlabel STR_ITEM_KILLER_TYPE_FAMILIAR
    .shiftjisz "ＫＩＬＬＥＲ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03E1C 8003061C */
dlabel STR_ITEM_KILLER
    .shiftjisz "ＫＩＬＬＥＲ"

/* 03E2C 8003062C */
dlabel STR_ITEM_MANDARA_TYPE_FAMILIAR
    .shiftjisz "ＭＡＮＤＡＲＡ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03E5C 8003065C */
dlabel STR_ITEM_MANDARA
    .shiftjisz "ＭＡＮＤＡＲＡ"

/* 03E6C 8003066C */
dlabel STR_ITEM_ZU_TYPE_FAMILIAR
    .shiftjisz "ＺＵ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03E90 80030690 */
dlabel STR_ITEM_NAPLASS_TYPE_FAMILIAR
    .shiftjisz "ＮＡＰＬＡＳＳ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03EC0 800306C0 */
dlabel STR_ITEM_NAPLASS
    .shiftjisz "ＮＡＰＬＡＳＳ"

/* 03ED0 800306D0 */
dlabel STR_ITEM_VIPER_TYPE_FAMILIAR
    .shiftjisz "ＶＩＰＥＲ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03EFC 800306FC */
dlabel STR_ITEM_VIPER
    .shiftjisz "ＶＩＰＥＲ"

/* 03F08 80030708 */
dlabel STR_ITEM_STEALTH_TYPE_FAMILIAR
    .shiftjisz "ＳＴＥＡＬＴＨ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03F38 80030738 */
dlabel STR_ITEM_STEALTH
    .shiftjisz "ＳＴＥＡＬＴＨ"

/* 03F48 80030748 */
dlabel STR_ITEM_WEADOG_TYPE_FAMILIAR
    .shiftjisz "ＷＥＡＤＯＧ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03F74 80030774 */
dlabel STR_ITEM_WEADOG
    .shiftjisz "ＷＥＡＤＯＧ"

/* 03F84 80030784 */
dlabel STR_ITEM_KRAKEN_TYPE_FAMILIAR
    .shiftjisz "ＫＲＡＫＥＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03FB0 800307B0 */
dlabel STR_ITEM_KRAKEN
    .shiftjisz "ＫＲＡＫＥＮ"

/* 03FC0 800307C0 */
dlabel STR_ITEM_PICKET_TYPE_FAMILIAR
    .shiftjisz "ＰＩＣＫＥＴ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 03FEC 800307EC */
dlabel STR_ITEM_PICKET
    .shiftjisz "ＰＩＣＫＥＴ"

/* 03FFC 800307FC */
dlabel STR_ITEM_BARONG_TYPE_FAMILIAR
    .shiftjisz "ＢＡＲＯＮＧ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04028 80030828 */
dlabel STR_ITEM_BARONG
    .shiftjisz "ＢＡＲＯＮＧ"

/* 04038 80030838 */
dlabel STR_ITEM_MANOEVA_TYPE_FAMILIAR
    .shiftjisz "ＭＡＮＯＥＶＡ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04068 80030868 */
dlabel STR_ITEM_MANOEVA
    .shiftjisz "ＭＡＮＯＥＶＡ"

/* 04078 80030878 */
dlabel STR_ITEM_CYCLONE_TYPE_FAMILIAR
    .shiftjisz "ＣＹＣＬＯＮＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 040A8 800308A8 */
dlabel STR_ITEM_CYCLONE
    .shiftjisz "ＣＹＣＬＯＮＥ"

/* 040B8 800308B8 */
dlabel STR_ITEM_VOLCANO_TYPE_FAMILIAR
    .shiftjisz "ＶＯＬＣＡＮＯ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 040E8 800308E8 */
dlabel STR_ITEM_VOLCANO
    .shiftjisz "ＶＯＬＣＡＮＯ"

/* 040F8 800308F8 */
dlabel STR_ITEM_BLUME_TYPE_FAMILIAR
    .shiftjisz "ＢＬＵＭＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04124 80030924 */
dlabel STR_ITEM_BLUME
    .shiftjisz "ＢＬＵＭＥ"

/* 04130 80030930 */
dlabel STR_ITEM_DREAMIN_TYPE_FAMILIAR
    .shiftjisz "ＤＲＥＡＭＩＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04160 80030960 */
dlabel STR_ITEM_DREAMIN
    .shiftjisz "ＤＲＥＡＭＩＮ"

/* 04170 80030970 */
dlabel STR_ITEM_BALOON_TYPE_FAMILIAR
    .shiftjisz "ＢＡＬＯＯＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 0419C 8003099C */
dlabel STR_ITEM_BALOON
    .shiftjisz "ＢＡＬＯＯＮ"

/* 041AC 800309AC */
dlabel STR_ITEM_U_BOAT_TYPE_FAMILIAR
    .shiftjisz "Ｕ−ＢＯＡＴ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 041D8 800309D8 */
dlabel STR_ITEM_U_BOAT
    .shiftjisz "Ｕ−ＢＯＡＴ"

/* 041E8 800309E8 */
dlabel STR_ITEM_NOISE_TYPE_FAMILIAR
    .shiftjisz "ＮＯＩＳＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04214 80030A14 */
dlabel STR_ITEM_NOISE
    .shiftjisz "ＮＯＩＳＥ"

/* 04220 80030A20 */
dlabel STR_ITEM_TROLL_TYPE_FAMILIAR
    .shiftjisz "ＴＲＯＬＬ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 0424C 80030A4C */
dlabel STR_ITEM_TROLL_2
    .shiftjisz "ＴＲＯＬＬ"

/* 04258 80030A58 */
dlabel STR_ITEM_PULUNPA_TYPE_FAMILIAR
    .shiftjisz "ＰＵＬＵＮＰＡ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04288 80030A88 */
dlabel STR_ITEM_PULUNPA
    .shiftjisz "ＰＵＬＵＮＰＡ"

/* 04298 80030A98 */
dlabel STR_ITEM_BLOCK_TYPE_FAMILIAR
    .shiftjisz "ＢＬＯＣＫ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 042C4 80030AC4 */
dlabel STR_ITEM_BLOCK
    .shiftjisz "ＢＬＯＣＫ"

/* 042D0 80030AD0 */
dlabel STR_ITEM_METAL_TYPE_FAMILIAR
    .shiftjisz "ＭＥＴＡＬ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 042FC 80030AFC */
dlabel STR_ITEM_METAL
    .shiftjisz "ＭＥＴＡＬ"

/* 04308 80030B08 */
dlabel STR_ITEM_UNICORN_TYPE_FAMILIAR
    .shiftjisz "ＵＮＩＣＯＲＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04338 80030B38 */
dlabel STR_ITEM_UNICORN
    .shiftjisz "ＵＮＩＣＯＲＮ"

/* 04348 80030B48 */
dlabel STR_ITEM_UNIVERN_TYPE_FAMILIAR
    .shiftjisz "ＵＮＩＶＥＲＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04378 80030B78 */
dlabel STR_ITEM_UNIVERN
    .shiftjisz "ＵＮＩＶＥＲＮ"

/* 04388 80030B88 */
dlabel STR_ITEM_CLOWN_TYPE_FAMILIAR
    .shiftjisz "ＣＬＯＷＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 043B4 80030BB4 */
dlabel STR_ITEM_CLOWN
    .shiftjisz "ＣＬＯＷＮ"

/* 043C0 80030BC0 */
dlabel STR_ITEM_DEATH_TYPE_FAMILIAR
    .shiftjisz "ＤＥＡＴＨ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 043EC 80030BEC */
dlabel STR_ITEM_DEATH
    .shiftjisz "ＤＥＡＴＨ"

/* 043F8 80030BF8 */
dlabel STR_ITEM_NYUEL_TYPE_FAMILIAR
    .shiftjisz "ＮＹＵＥＬ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04424 80030C24 */
dlabel STR_ITEM_NYUEL
    .shiftjisz "ＮＹＵＥＬ"

/* 04430 80030C30 */
dlabel STR_ITEM_BATTNEL_TYPE_FAMILIAR
    .shiftjisz "ＢＡＴＴＮＥＬ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04460 80030C60 */
dlabel STR_ITEM_BATTNEL
    .shiftjisz "ＢＡＴＴＮＥＬ"

/* 04470 80030C70 */
dlabel STR_ITEM_ARACHNE_TYPE_FAMILIAR
    .shiftjisz "ＡＲＡＣＨＮＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 044A0 80030CA0 */
dlabel STR_ITEM_ARACHNE
    .shiftjisz "ＡＲＡＣＨＮＥ"

/* 044B0 80030CB0 */
dlabel STR_ITEM_ASHRA_TYPE_FAMILIAR
    .shiftjisz "ＡＳＨＲＡ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 044DC 80030CDC */
dlabel STR_ITEM_ASHRA
    .shiftjisz "ＡＳＨＲＡ"

/* 044E8 80030CE8 */
dlabel STR_ITEM_SNOWMAN_TYPE_FAMILIAR
    .shiftjisz "ＳＮＯＷＭＡＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04518 80030D18 */
dlabel STR_ITEM_SNOWMAN
    .shiftjisz "ＳＮＯＷＭＡＮ"

/* 04528 80030D28 */
dlabel STR_ITEM_SABER_TYPE_FAMILIAR
    .shiftjisz "ＳＡＢＥＲ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04554 80030D54 */
dlabel STR_ITEM_SABER
    .shiftjisz "ＳＡＢＥＲ"

/* 04560 80030D60 */
dlabel STR_ITEM_GRIFFON_TYPE_FAMILIAR
    .shiftjisz "ＧＲＩＦＦＯＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04590 80030D90 */
dlabel STR_ITEM_GRIFFON
    .shiftjisz "ＧＲＩＦＦＯＮ"

/* 045A0 80030DA0 */
dlabel STR_ITEM_GRINEUT_TYPE_FAMILIAR
    .shiftjisz "ＧＲＩＮＥＵＴ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 045D0 80030DD0 */
dlabel STR_ITEM_GRINEUT
    .shiftjisz "ＧＲＩＮＥＵＴ"

/* 045E0 80030DE0 */
dlabel STR_ITEM_FLAME_TYPE_FAMILIAR
    .shiftjisz "ＦＬＡＭＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 0460C 80030E0C */
dlabel STR_ITEM_FLAME
    .shiftjisz "ＦＬＡＭＥ"

/* 04618 80030E18 */
dlabel STR_ITEM_IFRIT_TYPE_FAMILIAR
    .shiftjisz "ＩＦＲＩＴ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04644 80030E44 */
dlabel STR_ITEM_IFRIT
    .shiftjisz "ＩＦＲＩＴ"

/* 04650 80030E50 */
dlabel STR_ITEM_KID_TYPE_FAMILIAR
    .shiftjisz "ＫＩＤ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 04678 80030E78 */
dlabel STR_ITEM_DRAGON_TYPE_FAMILIAR
    .shiftjisz "ＤＲＡＧＯＮ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 046A4 80030EA4 */
dlabel STR_ITEM_DRAGON
    .shiftjisz "ＤＲＡＧＯＮ"

/* 046B4 80030EB4 */
dlabel STR_ITEM_KEWNE_TYPE_FAMILIAR
    .shiftjisz "ＫＥＷＮＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 046E0 80030EE0 */
dlabel STR_ITEM_KEWNE
    .shiftjisz "ＫＥＷＮＥ"

/* 046EC 80030EEC */
dlabel STR_ITEM_HIKEWNE_TYPE_FAMILIAR
    .shiftjisz "ＨＩＫＥＷＮＥ−ｔｙｐｅ　ｆａｍｉｌｉａｒ．"

/* 0471C 80030F1C */
dlabel STR_ITEM_HIKEWNE
    .shiftjisz "ＨＩＫＥＷＮＥ"

/* 0472C 80030F2C */
dlabel STR_ITEM_AN_ELEVATOR_WANNA_GO_UP
    .shiftjisz "Ａｎ　ｅｌｅｖａｔｏｒ．　Ｗａｎｎａ　ｇｏ　ｕｐ？\x0a"

/* 04760 80030F60 */
dlabel STR_ITEM_ELEVATOR
    .shiftjisz "Ｅｌｅｖａｔｏｒ"

/* 04774 80030F74 */
dlabel STR_ITEM_THE_EQUIPMENT_ON_HAND_WILL_RUST
    .shiftjisz "Ｔｈｅ　ｅｑｕｉｐｍｅｎｔ　ｏｎ　ｈａｎｄ　ｗｉｌｌ\x0aｒｕｓｔ．"

/* 047B4 80030FB4 */
dlabel STR_ITEM_RUST
    .shiftjisz "Ｒｕｓｔ"

/* 047C0 80030FC0 */
dlabel STR_ITEM_SEALS_MAGIC
    .shiftjisz "Ｓｅａｌｓ　ｍａｇｉｃ．"

/* 047DC 80030FDC */
dlabel STR_ITEM_THE_TERRAIN_RISES
    .shiftjisz "Ｔｈｅ　ｔｅｒｒａｉｎ　ｒｉｓｅｓ．"

/* 04804 80031004 */
dlabel STR_ITEM_UPHEAVAL
    .shiftjisz "Ｕｐｈｅａｖａｌ"

/* 04818 80031018 */
dlabel STR_ITEM_THE_TERRAIN_CAVES_IN
    .shiftjisz "Ｔｈｅ　ｔｅｒｒａｉｎ　ｃａｖｅｓ　ｉｎ．"

/* 04844 80031044 */
dlabel STR_ITEM_CRACK
    .shiftjisz "Ｃｒａｃｋ"

/* 04850 80031050 */
dlabel STR_ITEM_WAKES_UP_A_SLEEPING_MONSTER
    .shiftjisz "Ｗａｋｅｓ　ｕｐ　ａ　ｓｌｅｅｐｉｎｇ　ｍｏｎｓｔｅｒ．"

/* 0488C 8003108C */
dlabel STR_ITEM_BUMP
    .shiftjisz "Ｂｕｍｐ"

/* 04898 80031098 */
dlabel STR_ITEM_TURNS_INTO_A_FROG
    .shiftjisz "Ｔｕｒｎｓ　ｉｎｔｏ　ａ　ｆｒｏｇ．"

/* 048C0 800310C0 */
dlabel STR_ITEM_FROG_2
    .shiftjisz "Ｆｒｏｇ"

/* 048CC 800310CC */
dlabel STR_ITEM_MOVEMENT_WILL_BE_FROZEN_AND_THE_PLAY
    .shiftjisz "Ｍｏｖｅｍｅｎｔ　ｗｉｌｌ　ｂｅ　ｆｒｏｚｅｎ　ａｎｄ　ｔｈｅ\x0aｐｌａｙｅｒ　ｗｉｌｌ　ｂｅｃｏｍｅ　ｉｍｍｏｂｉｌｅ．"

/* 04944 80031144 */
dlabel STR_ITEM_PRISON
    .shiftjisz "Ｐｒｉｓｏｎ"

/* 04954 80031154 */
dlabel STR_ITEM_THE_PLAYER_GETS_POISONED
    .shiftjisz "Ｔｈｅ　ｐｌａｙｅｒ　ｇｅｔｓ　ｐｏｉｓｏｎｅｄ．"

/* 04988 80031188 */
dlabel STR_ITEM_POISON
    .shiftjisz "Ｐｏｉｓｏｎ"

/* 04998 80031198 */
dlabel STR_ITEM_THE_TRAP_BLINDS_THE_PLAYER
    .shiftjisz "Ｔｈｅ　ｔｒａｐ　ｂｌｉｎｄｓ　ｔｈｅ　ｐｌａｙｅｒ．"

/* 049D0 800311D0 */
dlabel STR_ITEM_BLINDER
    .shiftjisz "Ｂｌｉｎｄｅｒ"

/* 049E0 800311E0 */
dlabel STR_ITEM_THE_TRAP_PUTS_THE_PLAYER_TO_SLEEP
    .shiftjisz "Ｔｈｅ　ｔｒａｐ　ｐｕｔｓ　ｔｈｅ　ｐｌａｙｅｒ　ｔｏ\x0aｓｌｅｅｐ．"

/* 04A24 80031224 */
dlabel STR_ITEM_SLEEP
    .shiftjisz "Ｓｌｅｅｐ"

/* 04A30 80031230 */
dlabel STR_ITEM_AN_OBJECT_FALLS_ONTO_THE_PLAYER
    .shiftjisz "Ａｎ　ｏｂｊｅｃｔ　ｆａｌｌｓ　ｏｎｔｏ　ｔｈｅ\x0aｐｌａｙｅｒ．"

/* 04A70 80031270 */
dlabel STR_ITEM_SLAM
    .shiftjisz "Ｓｌａｍ"

/* 04A7C 8003127C */
dlabel STR_ITEM_IT_EXPLODES_AND_CAUSES_DAMAGE
    .shiftjisz "Ｉｔ　ｅｘｐｌｏｄｅｓ　ａｎｄ　ｃａｕｓｅｓ\x0aｄａｍａｇｅ．"

/* 04AB8 800312B8 */
dlabel STR_ITEM_BOMB
    .shiftjisz "Ｂｏｍｂ"

/* 04AC4 800312C4 */
dlabel STR_ITEM_THE_TRAP_PUTS_THE_PLAYER_INTO_CHAOS
    .shiftjisz "Ｔｈｅ　ｔｒａｐ　ｐｕｔｓ　ｔｈｅ　ｐｌａｙｅｒ　ｉｎｔｏ\x0aｃｈａｏｓ．"

/* 04B0C 8003130C */
dlabel STR_ITEM_CHAOS
    .shiftjisz "Ｃｈａｏｓ"

/* 04B18 80031318 */
dlabel STR_ITEM_THE_PLAYER_WILL_BE_SHIFTED_TO_AN_UPP
    .shiftjisz "Ｔｈｅ　ｐｌａｙｅｒ　ｗｉｌｌ　ｂｅ　ｓｈｉｆｔｅｄ　ｔｏ\x0aａｎ　ｕｐｐｅｒ　ｆｌｏｏｒ．"

/* 04B74 80031374 */
dlabel STR_ITEM_GO_UP
    .shiftjisz "Ｇｏ　ｕｐ"

/* 04B80 80031380 */
dlabel STR_ITEM_THE_PLAYER_WILL_BE_SHIFTED_TO_ANOTHE
    .shiftjisz "Ｔｈｅ　ｐｌａｙｅｒ　ｗｉｌｌ　ｂｅ　ｓｈｉｆｔｅｄ　ｔｏ\x0aａｎｏｔｈｅｒ　ｌｏｃａｔｉｏｎ．"

/* 04BE0 800313E0 */
dlabel STR_ITEM_WARP
    .shiftjisz "Ｗａｒｐ"

/* 04BEC 800313EC */
dlabel STR_ITEM_SPEED_WILL_BE_REDUCED_BY_HALF
    .shiftjisz "Ｓｐｅｅｄ　ｗｉｌｌ　ｂｅ　ｒｅｄｕｃｅｄ　ｂｙ　ｈａｌｆ．"

/* 04C2C 8003142C */
dlabel STR_ITEM_SLOW
    .shiftjisz "Ｓｌｏｗ"

/* 04C38 80031438 */
dlabel STR_ITEM_CEILING_AND_FLOOR_WILL_BE_TURNED_UPS
    .shiftjisz "Ｃｅｉｌｉｎｇ　ａｎｄ　ｆｌｏｏｒ　ｗｉｌｌ　ｂｅ\x0aｔｕｒｎｅｄ　ｕｐｓｉｄｅ　ｄｏｗｎ．"

/* 04C94 80031494 */
dlabel STR_ITEM_REVERSAL
    .shiftjisz "Ｒｅｖｅｒｓａｌ"

/* 04CA8 800314A8 */
dlabel STR_ITEM_JP_4CA8
    .shiftjisz "紺色の草"

/* 04CB4 800314B4 */
dlabel STR_ITEM_JP_4CB4
    .shiftjisz "朱色の草"

/* 04CC0 800314C0 */
dlabel STR_ITEM_ENABLES_EYE_SIGHT
    .shiftjisz "Ｅｎａｂｌｅｓ　ｅｙｅ　ｓｉｇｈｔ．"

/* 04CE8 800314E8 */
dlabel STR_ITEM_ROEAM
    .shiftjisz "Ｒｏｅａｍ"

/* 04CF4 800314F4 */
dlabel STR_ITEM_JP_4CF4
    .shiftjisz "オレンジ色の草"

/* 04D04 80031504 */
dlabel STR_ITEM_PUTS_THE_RECIPIENT_TO_SLEEP
    .shiftjisz "Ｐｕｔｓ　ｔｈｅ　ｒｅｃｉｐｉｅｎｔ　ｔｏ　ｓｌｅｅｐ．"

/* 04D40 80031540 */
dlabel STR_ITEM_JP_4D40
    .shiftjisz "銀色の草"

/* 04D4C 8003154C */
dlabel STR_ITEM_WEAKENS_DEFENSIVE_POWER
    .shiftjisz "Ｗｅａｋｅｎｓ　ｄｅｆｅｎｓｉｖｅ　ｐｏｗｅｒ．"

/* 04D80 80031580 */
dlabel STR_ITEM_HORREY
    .shiftjisz "Ｈｏｒｒｅｙ"

/* 04D90 80031590 */
dlabel STR_ITEM_JP_4D90
    .shiftjisz "金色の草"

/* 04D9C 8003159C */
dlabel STR_ITEM_WEAKENS_ATTACKING_POWER
    .shiftjisz "Ｗｅａｋｅｎｓ　ａｔｔａｃｋｉｎｇ　ｐｏｗｅｒ．"

/* 04DD0 800315D0 */
dlabel STR_ITEM_HARASH
    .shiftjisz "Ｈａｒａｓｈ"

/* 04DE0 800315E0 */
dlabel STR_ITEM_JP_4DE0
    .shiftjisz "水色の草"

/* 04DEC 800315EC */
dlabel STR_ITEM_HERB_THAT_FREEZES_MOTION
    .shiftjisz "Ｈｅｒｂ　ｔｈａｔ　ｆｒｅｅｚｅｓ　ｍｏｔｉｏｎ．"

/* 04E20 80031620 */
dlabel STR_ITEM_JP_4E20
    .shiftjisz "紫色の草"

/* 04E2C 8003162C */
dlabel STR_ITEM_TOXIC_HERB
    .shiftjisz "Ｔｏｘｉｃ　ｈｅｒｂ．"

/* 04E44 80031644 */
dlabel STR_ITEM_JP_4E44
    .shiftjisz "灰色の草"

/* 04E50 80031650 */
dlabel STR_ITEM_CAN_HEAL_DISEASE_OF_CHERRL
    .shiftjisz "Ｃａｎ　ｈｅａｌ　ｄｉｓｅａｓｅ　ｏｆ　Ｃｈｅｒｒｌ．"

/* 04E88 80031688 */
dlabel STR_ITEM_HEALING
    .shiftjisz "Ｈｅａｌｉｎｇ"

/* 04E98 80031698 */
dlabel STR_ITEM_RESTORES_WEAKENED_DEFENSE
    .shiftjisz "Ｒｅｓｔｏｒｅｓ　ｗｅａｋｅｎｅｄ　ｄｅｆｅｎｓｅ．"

/* 04ED0 800316D0 */
dlabel STR_ITEM_SHOMURO
    .shiftjisz "Ｓｈｏｍｕｒｏ"

/* 04EE0 800316E0 */
dlabel STR_ITEM_JP_4EE0
    .shiftjisz "ピンクの草"

/* 04EEC 800316EC */
dlabel STR_ITEM_RECOVERS_WEAKENED_ATTACK
    .shiftjisz "Ｒｅｃｏｖｅｒｓ　ｗｅａｋｅｎｅｄ　ａｔｔａｃｋ．"

/* 04F20 80031720 */
dlabel STR_ITEM_HAZAK
    .shiftjisz "Ｈａｚａｋ"

/* 04F2C 8003172C */
dlabel STR_ITEM_HERB_THAT_RESTORES_EVERYTHING
    .shiftjisz "Ｈｅｒｂ　ｔｈａｔ　ｒｅｓｔｏｒｅｓ　ｅｖｅｒｙｔｈｉｎｇ．"

/* 04F6C 8003176C */
dlabel STR_ITEM_CURE_ALL
    .shiftjisz "Ｃｕｒｅ−ａｌｌ"

/* 04F80 80031780 */
dlabel STR_ITEM_WAKES_UP_A_SLEEPING_FAMILIAR
    .shiftjisz "Ｗａｋｅｓ　ｕｐ　ａ　ｓｌｅｅｐｉｎｇ　ｆａｍｉｌｉａｒ．"

/* 04FBC 800317BC */
dlabel STR_ITEM_WAKE_UP
    .shiftjisz "Ｗａｋｅ−ｕｐ"

/* 04FCC 800317CC */
dlabel STR_ITEM_JP_4FCC
    .shiftjisz "黄色い草"

/* 04FD8 800317D8 */
dlabel STR_ITEM_HERB_THAT_RESTORES_CHAOS
    .shiftjisz "Ｈｅｒｂ　ｔｈａｔ　ｒｅｓｔｏｒｅｓ　ｃｈａｏｓ．"

/* 0500C 8003180C */
dlabel STR_ITEM_ANTICHAOS
    .shiftjisz "Ａｎｔｉｃｈａｏｓ"

/* 05020 80031820 */
dlabel STR_ITEM_HERB_NEUTRALIZES_A_POISON
    .shiftjisz "Ｈｅｒｂ　ｎｅｕｔｒａｌｉｚｅｓ　ａ　ｐｏｉｓｏｎ．"

/* 05058 80031858 */
dlabel STR_ITEM_ANTIDOTE
    .shiftjisz "Ａｎｔｉｄｏｔｅ"

/* 0506C 8003186C */
dlabel STR_ITEM_HERB_THAT_RESTORES_HP
    .shiftjisz "Ｈｅｒｂ　ｔｈａｔ　ｒｅｓｔｏｒｅｓ　ＨＰ．"

/* 0509C 8003189C */
dlabel STR_ITEM_MEDICINAL
    .shiftjisz "Ｍｅｄｉｃｉｎａｌ"

/* 050B0 800318B0 */
dlabel STR_ITEM_JP_50B0
    .shiftjisz "水色の実"

/* 050BC 800318BC */
dlabel STR_ITEM_VANISHES_MP_OF_THE_MONSTER_ATE_THIS
    .shiftjisz "Ｖａｎｉｓｈｅｓ　ＭＰ　ｏｆ　ｔｈｅ　ｍｏｎｓｔｅｒ　ａｔｅ\x0aｔｈｉｓ　ｆｒｕｉｔ．"

/* 05110 80031910 */
dlabel STR_ITEM_GEROPITA
    .shiftjisz "Ｇｅｒｏｐｉｔａ"

/* 05124 80031924 */
dlabel STR_ITEM_VANISHES_FAMILIAR_AND_THE_PLAYER_ESC_2
    .shiftjisz "Ｖａｎｉｓｈｅｓ　ｆａｍｉｌｉａｒ　ａｎｄ　ｔｈｅ\x0aｐｌａｙｅｒ　ｅｓｃａｐｅｓ　ｆｒｏｍ　ｔｈｅ　ｔｏｗｅｒ．"

/* 05194 80031994 */
dlabel STR_ITEM_JP_5194
    .shiftjisz "茶色い実"

/* 051A0 800319A0 */
dlabel STR_ITEM_INCREASE_PERCENTAGE_OF_CRITICAL_HITS
    .shiftjisz "Ｉｎｃｒｅａｓｅ　ｐｅｒｃｅｎｔａｇｅ　ｏｆ\x0aｃｒｉｔｉｃａｌ　ｈｉｔｓ　ｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 05204 80031A04 */
dlabel STR_ITEM_LIMIT
    .shiftjisz "Ｌｉｍｉｔ"

/* 05210 80031A10 */
dlabel STR_ITEM_WILL_REVERT_MONSTER_TO_ITS_EGG_STAGE
    .shiftjisz "Ｗｉｌｌ　ｒｅｖｅｒｔ　ｍｏｎｓｔｅｒ　ｔｏ　ｉｔｓ\x0aｅｇｇ−ｓｔａｇｅ．"

/* 0525C 80031A5C */
dlabel STR_ITEM_ROCHE
    .shiftjisz "Ｒｏｃｈｅ"

/* 05268 80031A68 */
dlabel STR_ITEM_JP_5268
    .shiftjisz "ピンクの実"

/* 05274 80031A74 */
dlabel STR_ITEM_MP_OF_MONSTER_TENDS_TO_DECREASE_ON_T
    .shiftjisz "ＭＰ　ｏｆ　ｍｏｎｓｔｅｒ　ｔｅｎｄｓ　ｔｏ\x0aｄｅｃｒｅａｓｅ　ｏｎ　ｔｈａｔ　ｌｅｖｅｌ．"

/* 052D0 80031AD0 */
dlabel STR_ITEM_LAEV
    .shiftjisz "Ｌａｅｖ"

/* 052DC 80031ADC */
dlabel STR_ITEM_JP_52DC
    .shiftjisz "黄色い実"

/* 052E8 80031AE8 */
dlabel STR_ITEM_MP_OF_THE_MONSTER_WILL_NOT_DECREASE
    .shiftjisz "ＭＰ　ｏｆ　ｔｈｅ　ｍｏｎｓｔｅｒ　ｗｉｌｌ　ｎｏｔ\x0aｄｅｃｒｅａｓｅ　ｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 05348 80031B48 */
dlabel STR_ITEM_LEOLAM
    .shiftjisz "Ｌｅｏｌａｍ"

/* 05358 80031B58 */
dlabel STR_ITEM_FAMILIAR_WILL_NOT_CHANGE_FORM_AFTER
    .shiftjisz "Ｆａｍｉｌｉａｒ　ｗｉｌｌ　ｎｏｔ　ｃｈａｎｇｅ　ｆｏｒｍ\x0aａｆｔｅｒ　ｔｈｅ　ｎｅｘｔ　ｆｕｓｉｏｎ．"

/* 053C0 80031BC0 */
dlabel STR_ITEM_LEVA
    .shiftjisz "Ｌｅｖａ"

/* 053CC 80031BCC */
dlabel STR_ITEM_TURNS_INTO_A_FROG_FOR_A_WHILE
    .shiftjisz "Ｔｕｒｎｓ　ｉｎｔｏ　ａ　ｆｒｏｇ　ｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 0540C 80031C0C */
dlabel STR_ITEM_TUMNA
    .shiftjisz "Ｔｕｍｎａ"

/* 05418 80031C18 */
dlabel STR_ITEM_FOOD_FOR_FAMILIAR_RECOVERS_MP_BY_100
    .shiftjisz "Ｆｏｏｄ　ｆｏｒ　ｆａｍｉｌｉａｒ．\x0aＲｅｃｏｖｅｒｓ　ＭＰ　ｂｙ　１００．"

/* 05464 80031C64 */
dlabel STR_ITEM_BIG_PITA
    .shiftjisz "Ｂｉｇ　Ｐｉｔａ"

/* 05478 80031C78 */
dlabel STR_ITEM_FOOD_FOR_FAMILIAR_RECOVERS_MP_BY_50
    .shiftjisz "Ｆｏｏｄ　ｆｏｒ　ｆａｍｉｌｉａｒ．\x0aＲｅｃｏｖｅｒｓ　ＭＰ　ｂｙ　５０．"

/* 054C4 80031CC4 */
dlabel STR_ITEM_PITA
    .shiftjisz "Ｐｉｔａ"

/* 054D0 80031CD0 */
dlabel STR_ITEM_JP_54D0
    .shiftjisz "銀色の種"

/* 054DC 80031CDC */
dlabel STR_ITEM_EATING_THIS_SEED_LEADS_TO_GOOD_FORTU
    .shiftjisz "Ｅａｔｉｎｇ　ｔｈｉｓ　ｓｅｅｄ　ｌｅａｄｓ　ｔｏ　ｇｏｏｄ\x0aｆｏｒｔｕｎｅ．"

/* 0552C 80031D2C */
dlabel STR_ITEM_TOVAR
    .shiftjisz "Ｔｏｖａｒ"

/* 05538 80031D38 */
dlabel STR_ITEM_JP_5538
    .shiftjisz "金色の種"

/* 05544 80031D44 */
dlabel STR_ITEM_EATING_THIS_SEED_DECREASES_SPEED
    .shiftjisz "Ｅａｔｉｎｇ　ｔｈｉｓ　ｓｅｅｄ　ｄｅｃｒｅａｓｅｓ\x0aｓｐｅｅｄ．"

/* 05588 80031D88 */
dlabel STR_ITEM_JP_5588
    .shiftjisz "水色の種"

/* 05594 80031D94 */
dlabel STR_ITEM_EATING_THIS_SEED_LOWERS_A_LEVEL
    .shiftjisz "Ｅａｔｉｎｇ　ｔｈｉｓ　ｓｅｅｄ　ｌｏｗｅｒｓ　ａ\x0aｌｅｖｅｌ．"

/* 055D4 80031DD4 */
dlabel STR_ITEM_JP_55D4
    .shiftjisz "緑色の種"

/* 055E0 80031DE0 */
dlabel STR_ITEM_CHANGES_THE_GENUS_OF_A_FAMILIAR_TO_W
    .shiftjisz "Ｃｈａｎｇｅｓ　ｔｈｅ　ｇｅｎｕｓ　ｏｆ　ａ　ｆａｍｉｌｉａｒ\x0aｔｏ　ｗｉｎｄ．"

/* 05630 80031E30 */
dlabel STR_ITEM_JP_5630
    .shiftjisz "紫色の種"

/* 0563C 80031E3C */
dlabel STR_ITEM_CHANGES_THE_GENUS_OF_A_FAMILIAR_TO_W_2
    .shiftjisz "Ｃｈａｎｇｅｓ　ｔｈｅ　ｇｅｎｕｓ　ｏｆ　ａ　ｆａｍｉｌｉａｒ\x0aｔｏ　ｗａｔｅｒ．"

/* 05690 80031E90 */
dlabel STR_ITEM_JP_5690
    .shiftjisz "黄色い種"

/* 0569C 80031E9C */
dlabel STR_ITEM_CHANGES_THE_GENUS_OF_A_FAMILIAR_TO_F
    .shiftjisz "Ｃｈａｎｇｅｓ　ｔｈｅ　ｇｅｎｕｓ　ｏｆ　ａ　ｆａｍｉｌｉａｒ\x0aｔｏ　ｆｉｒｅ．"

/* 056EC 80031EEC */
dlabel STR_ITEM_LIGHT
    .shiftjisz "Ｌｉｇｈｔ"

/* 056F8 80031EF8 */
dlabel STR_ITEM_INCREASES_SPEED_FOR_A_WHILE
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ｓｐｅｅｄ　ｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 05734 80031F34 */
dlabel STR_ITEM_MAHELL
    .shiftjisz "Ｍａｈｅｌｌ"

/* 05744 80031F44 */
dlabel STR_ITEM_INCREASES_THE_LEVEL
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ｔｈｅ　ｌｅｖｅｌ．"

/* 05770 80031F70 */
dlabel STR_ITEM_MAZARR
    .shiftjisz "Ｍａｚａｒｒ"

/* 05780 80031F80 */
dlabel STR_ITEM_INCREASES_DEFENSIVE_POWER
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ｄｅｆｅｎｓｉｖｅ　ｐｏｗｅｒ．"

/* 057B8 80031FB8 */
dlabel STR_ITEM_INCREASES_ATTACKING_POWER
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ａｔｔａｃｋｉｎｇ　ｐｏｗｅｒ．"

/* 057F0 80031FF0 */
dlabel STR_ITEM_JP_57F0
    .shiftjisz "つるつるな玉"

/* 05800 80032000 */
dlabel STR_ITEM_CONTAINS_ACID_RAIN_AND_ATTACKS_ENEMI
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ａｃｉｄ　ｒａｉｎ　ａｎｄ　ａｔｔａｃｋｓ\x0aｅｎｅｍｉｅｓ．"

/* 05850 80032050 */
dlabel STR_ITEM_ACID_RAIN
    .shiftjisz "Ａｃｉｄ　Ｒａｉｎ"

/* 05864 80032064 */
dlabel STR_ITEM_JP_5864
    .shiftjisz "ごつごつな玉"

/* 05874 80032074 */
dlabel STR_ITEM_CONTAINS_A_BEAM_WHICH_DECREASES_LEVE
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ａ　ｂｅａｍ　ｗｈｉｃｈ\x0aｄｅｃｒｅａｓｅｓ　ｌｅｖｅｌ　ｏｆ　ｅｎｅｍｉｅｓ．"

/* 058D8 800320D8 */
dlabel STR_ITEM_WEAK
    .shiftjisz "Ｗｅａｋ"

/* 058E4 800320E4 */
dlabel STR_ITEM_JP_58E4
    .shiftjisz "透明な玉"

/* 058F0 800320F0 */
dlabel STR_ITEM_A_BALL_THAT_CONTAINS_MORPHEUS_AND_PU
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｃｏｎｔａｉｎｓ　Ｍｏｒｐｈｅｕｓ\x0aａｎｄ　ｐｕｔｓ　ｅｎｅｍｉｅｓ　ｔｏ　ｓｌｅｅｐ．"

/* 05960 80032160 */
dlabel STR_ITEM_JP_5960
    .shiftjisz "ざらざらな玉"

/* 05970 80032170 */
dlabel STR_ITEM_A_BALL_THAT_HAS_THE_POWER_TO_BIND_MO
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｈａｓ　ｔｈｅ　ｐｏｗｅｒ　ｔｏ\x0aｂｉｎｄ　ｍｏｎｓｔｅｒｓ　ｗｉｔｈ　ｔｈｕｎｄｅｒ．"

/* 059E0 800321E0 */
dlabel STR_ITEM_BINDING
    .shiftjisz "Ｂｉｎｄｉｎｇ"

/* 059F0 800321F0 */
dlabel STR_ITEM_CONTAINS_DARKNESS_WHICH_BLINDS_THE_E
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ｄａｒｋｎｅｓｓ　ｗｈｉｃｈ　ｂｌｉｎｄｓ\x0aｔｈｅ　ｅｙｅｓ　ｏｆ　ｅｎｅｍｉｅｓ．"

/* 05A58 80032258 */
dlabel STR_ITEM_JP_5A58
    .shiftjisz "ぬるぬるの玉"

/* 05A68 80032268 */
dlabel STR_ITEM_CONTAINS_A_SOAP_BUBBLE_WHICH_RECOVER
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ａ　ｓｏａｐ　ｂｕｂｂｌｅ　ｗｈｉｃｈ\x0aｒｅｃｏｖｅｒｓ　ＨＰ．"

/* 05ABC 800322BC */
dlabel STR_ITEM_RECOVERY
    .shiftjisz "Ｒｅｃｏｖｅｒｙ"

/* 05AD0 800322D0 */
dlabel STR_ITEM_CONTAINS_A_HUGE_ICEBERG_WHICH_BLOCKS
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ａ　ｈｕｇｅ　ｉｃｅｂｅｒｇ　ｗｈｉｃｈ\x0aｂｌｏｃｋｓ　ｏｎｅ’ｓ　ｗａｙ．"

/* 05B30 80032330 */
dlabel STR_ITEM_ICE_ROCK
    .shiftjisz "Ｉｃｅ　Ｒｏｃｋ"

/* 05B44 80032344 */
dlabel STR_ITEM_JP_5B44
    .shiftjisz "大きい玉"

/* 05B50 80032350 */
dlabel STR_ITEM_A_BALL_THAT_HAS_THE_POWER_TO_REPEL_M
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｈａｓ　ｔｈｅ　ｐｏｗｅｒ　ｔｏ\x0aｒｅｐｅｌ　ｍａｇｉｃ．"

/* 05BA4 800323A4 */
dlabel STR_ITEM_REPEL
    .shiftjisz "Ｒｅｐｅｌ"

/* 05BB0 800323B0 */
dlabel STR_ITEM_JP_5BB0
    .shiftjisz "小さい玉"

/* 05BBC 800323BC */
dlabel STR_ITEM_A_BALL_THAT_CONTAINS_A_WATER_WALL_WH
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｃｏｎｔａｉｎｓ　ａ　ｗａｔｅｒ\x0aｗａｌｌ　ｗｈｉｃｈ　ｐｒｏｔｅｃｔｓ　ｔｈｅ　ｕｓｅｒ．"

/* 05C30 80032430 */
dlabel STR_ITEM_JP_5C30
    .shiftjisz "きれいな玉"

/* 05C3C 8003243C */
dlabel STR_ITEM_CONTAINS_POISON_CONTAGIOUS_TO_ALL_EN
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ｐｏｉｓｏｎ　ｃｏｎｔａｇｉｏｕｓ　ｔｏ\x0aａｌｌ　ｅｎｅｍｉｅｓ　ｉｎ　ｔｈｅ　ｓａｍｅ　ｒｏｏｍ．"

/* 05CB4 800324B4 */
dlabel STR_ITEM_CONTAINS_A_FIERY_PILLAR_WHICH_ASCEND
    .shiftjisz "Ｃｏｎｔａｉｎｓ　ａ　ｆｉｅｒｙ　ｐｉｌｌａｒ　ｗｈｉｃｈ\x0aａｓｃｅｎｄｓ　ｆｒｏｍ　ｔｈｅ　ｇｒｏｕｎｄ．"

/* 05D20 80032520 */
dlabel STR_ITEM_PILLAR
    .shiftjisz "Ｐｉｌｌａｒ"

/* 05D30 80032530 */
dlabel STR_ITEM_A_BALL_THAT_CONTAINS_A_FIREBALL
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｃｏｎｔａｉｎｓ　ａ\x0aｆｉｒｅｂａｌｌ．"

/* 05D70 80032570 */
dlabel STR_ITEM_FLAME_2
    .shiftjisz "Ｆｌａｍｅ"

/* 05D7C 8003257C */
dlabel STR_ITEM_JP_5D7C
    .shiftjisz "明るい玉"

/* 05D88 80032588 */
dlabel STR_ITEM_A_BALL_THAT_BLAZES_ALONG_THE_GROUND
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｂｌａｚｅｓ　ａｌｏｎｇ　ｔｈｅ\x0aｇｒｏｕｎｄ　ｉｎ　ａ　ｓｔｒａｉｇｈｔ　ｌｉｎｅ．"

/* 05DF8 800325F8 */
dlabel STR_ITEM_BLAZE
    .shiftjisz "Ｂｌａｚｅ"

/* 05E04 80032604 */
dlabel STR_ITEM_JP_5E04
    .shiftjisz "いびつな玉"

/* 05E10 80032610 */
dlabel STR_ITEM_A_BALL_THAT_CONTAINS_A_FIRE_ARROW
    .shiftjisz "Ａ　ｂａｌｌ　ｔｈａｔ　ｃｏｎｔａｉｎｓ　ａ　ｆｉｒｅ\x0aａｒｒｏｗ．"

/* 05E54 80032654 */
dlabel STR_ITEM_JP_5E54
    .shiftjisz "厚い巻き物"

/* 05E60 80032660 */
dlabel STR_ITEM_THE_BEAM_TURNS_ALL_ITEMS_ON_THE_FLOO
    .shiftjisz "Ｔｈｅ　ｂｅａｍ　ｔｕｒｎｓ　ａｌｌ　ｉｔｅｍｓ　ｏｎ　ｔｈｅ\x0aｆｌｏｏｒ　ｉｎｔｏ　ｇｏｌｄ　ｃｏｉｎｓ．"

/* 05ECC 800326CC */
dlabel STR_ITEM_ALCHEMIC
    .shiftjisz "Ａｌｃｈｅｍｉｃ"

/* 05EE0 800326E0 */
dlabel STR_ITEM_JP_5EE0
    .shiftjisz "薄い巻き物"

/* 05EEC 800326EC */
dlabel STR_ITEM_THE_BEAM_REMOVES_STEPS_AND_FLATTENS
    .shiftjisz "Ｔｈｅ　ｂｅａｍ　ｒｅｍｏｖｅｓ　ｓｔｅｐｓ　ａｎｄ\x0aｆｌａｔｔｅｎｓ　ｔｈｅ　ｅｎｔｉｒｅ　ｆｌｏｏｒ．"

/* 05F58 80032758 */
dlabel STR_ITEM_FLAT
    .shiftjisz "Ｆｌａｔ"

/* 05F64 80032764 */
dlabel STR_ITEM_JP_5F64
    .shiftjisz "新しい巻き物"

/* 05F74 80032774 */
dlabel STR_ITEM_THE_BEAM_REMOVES_A_CURSE_CAST_UPON_E
    .shiftjisz "Ｔｈｅ　ｂｅａｍ　ｒｅｍｏｖｅｓ　ａ　ｃｕｒｓｅ　ｃａｓｔ\x0aｕｐｏｎ　ｅｑｕｉｐｍｅｎｔ．"

/* 05FD0 800327D0 */
dlabel STR_ITEM_DE_CURSE
    .shiftjisz "Ｄｅ−ｃｕｒｓｅ"

/* 05FE4 800327E4 */
dlabel STR_ITEM_JP_5FE4
    .shiftjisz "奇蹟の巻き物"

/* 05FF4 800327F4 */
dlabel STR_ITEM_THE_BEAM_RESUSCITATES_A_FAINTED_FAMI
    .shiftjisz "Ｔｈｅ　ｂｅａｍ　ｒｅｓｕｓｃｉｔａｔｅｓ　ａ　ｆａｉｎｔｅｄ\x0aｆａｍｉｌｉａｒ．"

/* 06048 80032848 */
dlabel STR_ITEM_RESTORE
    .shiftjisz "Ｒｅｓｔｏｒｅ"

/* 06058 80032858 */
dlabel STR_ITEM_JP_6058
    .shiftjisz "古い巻き物"

/* 06064 80032864 */
dlabel STR_ITEM_ALL_TRAPS_ON_THE_FLOOR_BECOME_INVISI
    .shiftjisz "Ａｌｌ　ｔｒａｐｓ　ｏｎ　ｔｈｅ　ｆｌｏｏｒ　ｂｅｃｏｍｅ\x0aｉｎｖｉｓｉｂｌｅ．"

/* 060B4 800328B4 */
dlabel STR_ITEM_TRAP
    .shiftjisz "Ｔｒａｐ"

/* 060C0 800328C0 */
dlabel STR_ITEM_JP_60C0
    .shiftjisz "ぼろの巻き物"

/* 060D0 800328D0 */
dlabel STR_ITEM_MALICIOUS_BEAM_BRINGING_CHAOS_FOR_A
    .shiftjisz "Ｍａｌｉｃｉｏｕｓ　ｂｅａｍ　ｂｒｉｎｇｉｎｇ　ｃｈａｏｓ\x0aｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 06124 80032924 */
dlabel STR_ITEM_MALICIOUS
    .shiftjisz "Ｍａｌｉｃｉｏｕｓ"

/* 06138 80032938 */
dlabel STR_ITEM_JP_6138
    .shiftjisz "豪華な巻き物"

/* 06148 80032948 */
dlabel STR_ITEM_HOLY_BEAM_PROTECTS_THE_READER_FROM_E
    .shiftjisz "Ｈｏｌｙ　ｂｅａｍ　ｐｒｏｔｅｃｔｓ　ｔｈｅ　ｒｅａｄｅｒ\x0aｆｒｏｍ　ｅｎｅｍｙ　ａｔｔａｃｋ　ｆｏｒ　ａ　ｗｈｉｌｅ．"

/* 061C0 800329C0 */
dlabel STR_ITEM_CALL_BACK_FAMILIARS_TO_YOU
    .shiftjisz "Ｃａｌｌ　ｂａｃｋ　ｆａｍｉｌｉａｒｓ　ｔｏ　ｙｏｕ．"

/* 061F8 800329F8 */
dlabel STR_ITEM_FAMILIAR
    .shiftjisz "Ｆａｍｉｌｉａｒ"

/* 0620C 80032A0C */
dlabel STR_ITEM_JP_620C
    .shiftjisz "小さい鈴"

/* 06218 80032A18 */
dlabel STR_ITEM_INCREASES_THE_LEVEL_OF_ENEMY_MONSTER
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ｔｈｅ　ｌｅｖｅｌ　ｏｆ　ｅｎｅｍｙ\x0aｍｏｎｓｔｅｒｓ．"

/* 06264 80032A64 */
dlabel STR_ITEM_JP_6264
    .shiftjisz "大きい鈴"

/* 06270 80032A70 */
dlabel STR_ITEM_CHASES_A_MONSTER_OUT_OF_A_ROOM
    .shiftjisz "Ｃｈａｓｅｓ　ａ　ｍｏｎｓｔｅｒ　ｏｕｔ　ｏｆ　ａ　ｒｏｏｍ．"

/* 062B0 80032AB0 */
dlabel STR_ITEM_JP_62B0
    .shiftjisz "四角いメガネ"

/* 062C0 80032AC0 */
dlabel STR_ITEM_MAKE_THE_CURRENT_FLOOR_ENTIRELY_VISI
    .shiftjisz "Ｍａｋｅ　ｔｈｅ　ｃｕｒｒｅｎｔ　ｆｌｏｏｒ\x0aｅｎｔｉｒｅｌｙ　ｖｉｓｉｂｌｅ．"

/* 06310 80032B10 */
dlabel STR_ITEM_STAR
    .shiftjisz "Ｓｔａｒ"

/* 0631C 80032B1C */
dlabel STR_ITEM_JP_631C
    .shiftjisz "丸いメガネ"

/* 06328 80032B28 */
dlabel STR_ITEM_IDENTIFY_ITEMS_EVEN_TYPES_OF_EGGS
    .shiftjisz "Ｉｄｅｎｔｉｆｙ　ｉｔｅｍｓ．\x0aＥｖｅｎ　ｔｙｐｅｓ　ｏｆ　ｅｇｇｓ．"

/* 06370 80032B70 */
dlabel STR_ITEM_TRUTH
    .shiftjisz "Ｔｒｕｔｈ"

/* 0637C 80032B7C */
dlabel STR_ITEM_JP_637C
    .shiftjisz "黒いルーペ"

/* 06388 80032B88 */
dlabel STR_ITEM_LOUPE_THAT_IDENTIFIES_THE_LOCATION_O
    .shiftjisz "Ｌｏｕｐｅ　ｔｈａｔ　ｉｄｅｎｔｉｆｉｅｓ　ｔｈｅ\x0aｌｏｃａｔｉｏｎ　ｏｆ　ｉｔｅｍｓ．"

/* 063E0 80032BE0 */
dlabel STR_ITEM_TREASURE
    .shiftjisz "Ｔｒｅａｓｕｒｅ"

/* 063F4 80032BF4 */
dlabel STR_ITEM_JP_63F4
    .shiftjisz "白いルーペ"

/* 06400 80032C00 */
dlabel STR_ITEM_IDENTIFIES_THE_LOCATION_OF_MONSTERS
    .shiftjisz "Ｉｄｅｎｔｉｆｉｅｓ　ｔｈｅ　ｌｏｃａｔｉｏｎ　ｏｆ\x0aｍｏｎｓｔｅｒｓ．"

/* 06448 80032C48 */
dlabel STR_ITEM_MONSTER
    .shiftjisz "Ｍｏｎｓｔｅｒ"

/* 06458 80032C58 */
dlabel STR_ITEM_JP_6458
    .shiftjisz "茶色いルーペ"

/* 06468 80032C68 */
dlabel STR_ITEM_MAKES_INVISIBLES_VISIBLE
    .shiftjisz "Ｍａｋｅｓ　ｉｎｖｉｓｉｂｌｅｓ　ｖｉｓｉｂｌｅ．"

/* 0649C 80032C9C */
dlabel STR_ITEM_FOCUS
    .shiftjisz "Ｆｏｃｕｓ"

/* 064A8 80032CA8 */
dlabel STR_ITEM_JP_64A8
    .shiftjisz "紫のルーペ"

/* 064B4 80032CB4 */
dlabel STR_ITEM_MAKES_TRAPS_VISIBLE
    .shiftjisz "Ｍａｋｅｓ　ｔｒａｐｓ　ｖｉｓｉｂｌｅ．"

/* 064E0 80032CE0 */
dlabel STR_ITEM_JP_64E0
    .shiftjisz "水色のルーペ"

/* 064F0 80032CF0 */
dlabel STR_ITEM_IDENTIFIES_THE_LOCATIONS_OF_ELEVATOR
    .shiftjisz "Ｉｄｅｎｔｉｆｉｅｓ　ｔｈｅ　ｌｏｃａｔｉｏｎｓ　ｏｆ\x0aｅｌｅｖａｔｏｒｓ．"

/* 0653C 80032D3C */
dlabel STR_ITEM_EXIT
    .shiftjisz "Ｅｘｉｔ"

/* 06548 80032D48 */
dlabel STR_ITEM_INCREASES_THE_NUMBER_OF_TIMES_A_BALL
    .shiftjisz "Ｉｎｃｒｅａｓｅｓ　ｔｈｅ　ｎｕｍｂｅｒ　ｏｆ　ｔｉｍｅｓ\x0aａ　ｂａｌｌ　ｃａｎ　ｂｅ　ｕｓｅｄ．"

/* 065AC 80032DAC */
dlabel STR_ITEM_WHITE
    .shiftjisz "Ｗｈｉｔｅ"

/* 065B8 80032DB8 */
dlabel STR_ITEM_TEMPERS_A_SHIELD
    .shiftjisz "Ｔｅｍｐｅｒｓ　ａ　ｓｈｉｅｌｄ．"

/* 065DC 80032DDC */
dlabel STR_ITEM_BLUE
    .shiftjisz "Ｂｌｕｅ"

/* 065E8 80032DE8 */
dlabel STR_ITEM_TEMPERS_A_SWORD
    .shiftjisz "Ｔｅｍｐｅｒｓ　ａ　ｓｗｏｒｄ．"
