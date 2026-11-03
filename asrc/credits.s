# The staff-credits roll: all 53 strings, hand-owned.
#
# Covers rom 0x002184..0x0027DC (vram 0x8002E984..0x8002EFDC). That line
# is read back by splat/ext/hrodata.py on every split, which warns if the
# `hrodata` subsegment in splat/slus_006.14.yaml no longer matches it -- the
# file is never regenerated once it exists.
#
# One subsegment replaces 53 one-per-string `shiftjis` subsegments. Entries are
# in roll order: a role, then the person who filled it.
#
# Every string is NUL-terminated and padded to the next 4-byte boundary, so the
# zero run after each is `4 - (len % 4)` bytes -- 1, 2, 3 or 4, never none, and
# `.shiftjisz` emits exactly that. Measured: 53 of 53 obey it, which is the same
# 160 bytes of inter-string zeros the yaml comment used to count.
#
# EVERY ONE of these is reached by a pointer in game_data's credits table (file
# 0x4528C onward), and `hrodata` scans its range, so splat binds those pointers
# by symbol. The names below are declared in splat/slus_006.14_symbol_addrs.txt
# so the table reads as `.word STR_CREDITS_*`; renaming a label here without
# updating that file is an undefined reference at link time.

.include "macro.inc"

.section .rodata

/* 02184 8002E984 */
dlabel STR_CREDITS_PRESENTED_BY
    .shiftjisz "Ｐｒｅｓｅｎｔｅｄ　ｂｙ"

/* 021A0 8002E9A0 */
dlabel STR_CREDITS_KITAUE_KAZUMI
    .shiftjisz "Ｋｉｔａｕｅ　Ｋａｚｕｍｉ"

/* 021BC 8002E9BC */
dlabel STR_CREDITS_PRODUCED_BY
    .shiftjisz "Ｐｒｏｄｕｃｅｄ　ｂｙ"

/* 021D4 8002E9D4 */
dlabel STR_CREDITS_YONEOKA_MASAKI
    .shiftjisz "Ｙｏｎｅｏｋａ　Ｍａｓａｋｉ"

/* 021F4 8002E9F4 */
dlabel STR_CREDITS_SUPERVISED_BY
    .shiftjisz "Ｓｕｐｅｒｖｉｓｅｄ　ｂｙ"

/* 02210 8002EA10 */
dlabel STR_CREDITS_HARADA_MASAAKI
    .shiftjisz "Ｈａｒａｄａ　Ｍａｓａａｋｉ"

/* 02230 8002EA30 */
dlabel STR_CREDITS_SPECIAL_THANKS_TO
    .shiftjisz "Ｓｐｅｃｉａｌ　ｔｈａｎｋｓ　ｔｏ"

/* 02254 8002EA54 */
dlabel STR_CREDITS_KASAI_HIDEKI
    .shiftjisz "ＫＡＳＡＩ　ＨＩＤＥＫＩ"

/* 02270 8002EA70 */
dlabel STR_CREDITS_FLATLINE
    .shiftjisz "ＦＬＡＴＬＩＮＥ"

/* 02284 8002EA84 */
dlabel STR_CREDITS_SOUND_EDITED_BY
    .shiftjisz "Ｓｏｕｎｄ　ｅｄｉｔｅｄ　ｂｙ"

/* 022A4 8002EAA4 */
dlabel STR_CREDITS_KIYOHIKO_YAMANE
    .shiftjisz "Ｋｉｙｏｈｉｋｏ　Ｙａｍａｎｅ"

/* 022C4 8002EAC4 */
dlabel STR_CREDITS_MUSIC_DRIVER_BY
    .shiftjisz "Ｍｕｓｉｃ　ｄｒｉｖｅｒ　ｂｙ"

/* 022E4 8002EAE4 */
dlabel STR_CREDITS_NORITADA_MATSUKAWA
    .shiftjisz "Ｎｏｒｉｔａｄａ　Ｍａｔｓｕｋａｗａ"

/* 0230C 8002EB0C */
dlabel STR_CREDITS_SOUND_MANAGEMENT_BY
    .shiftjisz "Ｓｏｕｎｄ　ｍａｎａｇｅｍｅｎｔ　ｂｙ"

/* 02334 8002EB34 */
dlabel STR_CREDITS_SOUND_PROGRAMMING_BY
    .shiftjisz "Ｓｏｕｎｄ　ｐｒｏｇｒａｍｍｉｎｇ　ｂｙ"

/* 02360 8002EB60 */
dlabel STR_CREDITS_IMOHORE_IMAI
    .shiftjisz "Ｉｍｏｈｏｒｅ　ＩＭＡＩ"

/* 0237C 8002EB7C */
dlabel STR_CREDITS_MOVIE_SE_BY
    .shiftjisz "Ｍｏｖｉｅ　ＳＥ　ｂｙ"

/* 02394 8002EB94 */
dlabel STR_CREDITS_HIROE_NOGUCHI
    .shiftjisz "Ｈｉｒｏｅ　Ｎｏｇｕｃｈｉ"

/* 023B0 8002EBB0 */
dlabel STR_CREDITS_SOUND_EFFECTS_BY
    .shiftjisz "Ｓｏｕｎｄ　ｅｆｆｅｃｔｓ　ｂｙ"

/* 023D4 8002EBD4 */
dlabel STR_CREDITS_HIROSHI_TAMAWARI
    .shiftjisz "Ｈｉｒｏｓｈｉ　Ｔａｍａｗａｒｉ"

/* 023F8 8002EBF8 */
dlabel STR_CREDITS_MUSIC_BY
    .shiftjisz "Ｍｕｓｉｃ　ｂｙ"

/* 0240C 8002EC0C */
dlabel STR_CREDITS_TADANO_TSUGUO
    .shiftjisz "ＴＡＤＡＮＯ　ＴＳＵＧＵＯ"

/* 02428 8002EC28 */
dlabel STR_CREDITS_IMAIZUMI_KENICHIRO
    .shiftjisz "Ｉｍａｉｚｕｍｉ　Ｋｅｎｉｃｈｉｒｏ"

/* 02450 8002EC50 */
dlabel STR_CREDITS_NAKANO_KAZUAKI
    .shiftjisz "Ｎａｋａｎｏ　Ｋａｚｕａｋｉ"

/* 02470 8002EC70 */
dlabel STR_CREDITS_MOVIE_BY
    .shiftjisz "Ｍｏｖｉｅ　ｂｙ"

/* 02484 8002EC84 */
dlabel STR_CREDITS_TAJIMA_TERUTO
    .shiftjisz "Ｔａｊｉｍａ　Ｔｅｒｕｔｏ"

/* 024A0 8002ECA0 */
dlabel STR_CREDITS_AOKI_YOKO
    .shiftjisz "Ａｏｋｉ　Ｙｏｋｏ"

/* 024B4 8002ECB4 */
dlabel STR_CREDITS_ADDITIONAL_DESIGN_BY
    .shiftjisz "Ａｄｄｉｔｉｏｎａｌ　ｄｅｓｉｇｎ　ｂｙ"

/* 024E0 8002ECE0 */
dlabel STR_CREDITS_MASAKO_YASHIMA
    .shiftjisz "Ｍａｓａｋｏ　Ｙａｓｈｉｍａ"

/* 02500 8002ED00 */
dlabel STR_CREDITS_KANEGUCHI_KENJI
    .shiftjisz "Ｋａｎｅｇｕｃｈｉ　Ｋｅｎｊｉ"

/* 02520 8002ED20 */
dlabel STR_CREDITS_SATOH_HIROKI
    .shiftjisz "Ｓａｔｏｈ　Ｈｉｒｏｋｉ"

/* 0253C 8002ED3C */
dlabel STR_CREDITS_IWAKURA_KOUSUKE
    .shiftjisz "Ｉｗａｋｕｒａ　Ｋｏｕｓｕｋｅ"

/* 0255C 8002ED5C */
dlabel STR_CREDITS_KOMA
    .shiftjisz "ＫＯＭＡ"

/* 02568 8002ED68 */
dlabel STR_CREDITS_HAGIHARA_TORU
    .shiftjisz "Ｈａｇｉｈａｒａ　Ｔｏｒｕ"

/* 02584 8002ED84 */
dlabel STR_CREDITS_SCENARIO_CODED_BY
    .shiftjisz "Ｓｃｅｎａｒｉｏ　ｃｏｄｅｄ　ｂｙ"

/* 025A8 8002EDA8 */
dlabel STR_CREDITS_KATANA_1
    .shiftjisz "ＫＡＴＡＮＡ−１"

/* 025BC 8002EDBC */
dlabel STR_CREDITS_KIKUCHI_YUJI
    .shiftjisz "Ｋｉｋｕｃｈｉ　Ｙｕｊｉ"

/* 025D8 8002EDD8 */
dlabel STR_CREDITS_ISHIKAWA_KAZUO
    .shiftjisz "Ｉｓｈｉｋａｗａ　Ｋａｚｕｏ"

/* 025F8 8002EDF8 */
dlabel STR_CREDITS_OGISO_KAZUMASA
    .shiftjisz "Ｏｇｉｓｏ　Ｋａｚｕｍａｓａ"

/* 02618 8002EE18 */
dlabel STR_CREDITS_HAKODATE_ZIRO
    .shiftjisz "ＨＡＫＯＤＡＴＥ　ＺＩＲＯ"

/* 02634 8002EE34 */
dlabel STR_CREDITS_PROGRAMMING_BY
    .shiftjisz "Ｐｒｏｇｒａｍｍｉｎｇ　ｂｙ"

/* 02654 8002EE54 */
dlabel STR_CREDITS_INABA_HARUHIKO
    .shiftjisz "Ｉｎａｂａ　Ｈａｒｕｈｉｋｏ"

/* 02674 8002EE74 */
dlabel STR_CREDITS_ASSISTANT_DIRECTED_BY
    .shiftjisz "Ａｓｓｉｓｔａｎｔ　ｄｉｒｅｃｔｅｄ　ｂｙ"

/* 026A0 8002EEA0 */
dlabel STR_CREDITS_BACKGROUND_DESIGN_BY
    .shiftjisz "Ｂａｃｋｇｒｏｕｎｄ　ｄｅｓｉｇｎ　ｂｙ"

/* 026CC 8002EECC */
dlabel STR_CREDITS_HATTORI_K_SUKE
    .shiftjisz "Ｈａｔｔｏｒｉ　Ｋ−ｓｕｋｅ"

/* 026EC 8002EEEC */
dlabel STR_CREDITS_MONSTER_DESIGN_BY
    .shiftjisz "Ｍｏｎｓｔｅｒ　ｄｅｓｉｇｎ　ｂｙ"

/* 02710 8002EF10 */
dlabel STR_CREDITS_CHARACTER_DESIGN_BY
    .shiftjisz "Ｃｈａｒａｃｔｅｒ　ｄｅｓｉｇｎ　ｂｙ"

/* 02738 8002EF38 */
dlabel STR_CREDITS_YAMADA_KOJI
    .shiftjisz "Ｙａｍａｄａ　Ｋｏｊｉ"

/* 02750 8002EF50 */
dlabel STR_CREDITS_SCENARIO_WRITTEN_BY
    .shiftjisz "Ｓｃｅｎａｒｉｏ　ｗｒｉｔｔｅｎ　ｂｙ"

/* 02778 8002EF78 */
dlabel STR_CREDITS_HORIO_KENICHIRO
    .shiftjisz "Ｈｏｒｉｏ　Ｋｅｎｉｃｈｉｒｏ"

/* 02798 8002EF98 */
dlabel STR_CREDITS_DIRECTED_BY
    .shiftjisz "Ｄｉｒｅｃｔｅｄ　ｂｙ"

/* 027B0 8002EFB0 */
dlabel STR_CREDITS_CREDITS
    .shiftjisz "Ｃｒｅｄｉｔｓ"

/* 027C0 8002EFC0 */
dlabel STR_CREDITS_AZURE_DREAMS
    .shiftjisz "Ａｚｕｒｅ　Ｄｒｅａｍｓ"
