.include "macro.inc"

/* HAND-OWNED .rodata. splat seeded this file once, from the ROM, and
 * will not rewrite it: the `hrodata` subsegment type exists to hand the
 * file over. Edit it freely -- `.shiftjis "..."` works here, because
 * `make` pipes every asm source through `azure shiftjis`. That filter is
 * line-anchored, so a `.shiftjis` directive must sit ALONE on its line:
 * leave the offset note on the line above it, or the directive reaches
 * GNU as unexpanded and `unknown pseudo-op` is what you get.
 *
 * Covers rom 0x25D0C0..0x25D924 (vram 0x800200C0..0x80020924).
 * Keep that size exact and keep the symbol names: the linker script
 * places this object's .rodata at the offset above, and other asm still
 * resolves these symbols by name. Moving the subsegment in the yaml does
 * NOT move the bytes here -- splat warns, and `make check` reports the
 * first offset that drifted.
 *
 * The `nonmatching` markers below are splat's note that a symbol is not
 * decompiled yet. Nothing in tools/ reads them; drop them as the data
 * here becomes real, named, understood data.
 */

/* The 0x80020000 overlay's Shift-JIS UI message pool: 59 NUL-terminated
 * strings, each padded to the next 4-byte boundary, which is exactly what
 * `.shiftjisz` emits (the string, a NUL, then `.balign 4, 0`).
 *
 * The D_ labels are the load-bearing ones: the 0x1E0-byte table at
 * 0x800280B4 indexes into this pool and 121 words in the `bin` at 0x265000
 * point at these addresses. They are raw bytes with no relocations today, so
 * nothing resolves these names yet -- but claiming that table is what will,
 * and 0x80020688 (the empty string) is pointed at three times on its own.
 *
 * Add STR_* aliases beside the D_ labels as the messages get identified; see
 * asrc/names.s for that convention.
 */

.section .rodata, "a"

dlabel D_800200C0
    .shiftjisz "Ｆａｉｌｅｄ　ｔｏ　ｄｅｌｅｔｅ　ｄａｔａ．"

dlabel D_800200F0
    .shiftjisz "        ＹＥＳ                ＮＯ"

dlabel D_80020114
    .shiftjisz "ｔｈｉｓ　ｄａｔａ？"

dlabel D_8002012C
    .shiftjisz "Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ　ｄｅｌｅｔｅ"

dlabel D_80020158
    .shiftjisz "Ｄｅｌｅｔｅ　ｄａｔａ　ａｎｄ　ｒｅｔｒｙ．"

dlabel D_80020188
    .shiftjisz "Ｃａｎｎｏｔ　ｏｖｅｒｗｒｉｔｅ　ｔｈｉｓ　ｄａｔａ．"

dlabel D_800201C0
    .shiftjisz "Ｆａｉｌｅｄ　ｔｏ　ｃｏｐｙ　ｄａｔａ．"

dlabel D_800201EC
    .shiftjisz "Ｃｏｐｙｉｎｇ．．．"

dlabel D_80020204
    .shiftjisz "Ｌｏａｄｉｎｇ．．．"

dlabel D_8002021C
    .shiftjisz "Ｆｏｒｍａｔｔｅｄ　ｓｕｃｃｅｓｓｆｕｌｌｙ．"

dlabel D_8002024C
    .shiftjisz "Ｆａｉｌｅｄ　ｔｏ　ｆｏｒｍａｔ"

dlabel D_80020270
    .shiftjisz "Ｆａｉｌｅｄ　ｔｏ　ｌｏａｄ　ｄａｔａ．"

dlabel D_8002029C
    .shiftjisz "ｓｃｒｅｅｎ．"

dlabel D_800202AC
    .shiftjisz "ａｔ　ｔｈｅ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｍａｎａｇｅｒ"

dlabel D_800202E4
    .shiftjisz "Ｃａｒｄ　ｏｒ　ｄｅｌｅｔｅ　ｕｎｎｅｃｅｓｓａｒｙ　ｄａｔａ"

dlabel D_80020324
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ．　Ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ"

dlabel D_80020364
    .shiftjisz "ｔｈｅ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｍａｎａｇｅｒ　ｓｃｒｅｅｎ．"

dlabel D_800203A4
    .shiftjisz "ｏｒ　ｄｅｌｅｔｅ　ｕｎｎｅｃｅｓｓａｒｙ　ｄａｔａ　ａｔ"

dlabel D_800203E0
    .shiftjisz "ｄａｔａ　ｏｒ　ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ　Ｃａｒｄ"

dlabel D_80020420
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ．　Ｏｖｅｒｗｒｉｔｅ　ｓａｖｅｄ"

dlabel D_8002045C
    .shiftjisz "Ｎｏｔ　ｅｎｏｕｇｈ　ｆｒｅｅ　ｂｌｏｃｋｓ　ｏｎ　ｔｈｉｓ"

dlabel D_8002049C
    .shiftjisz "Ｆａｉｌｅｄ　ｔｏ　ｏｖｅｒｗｒｉｔｅ"

dlabel D_800204C4
    .shiftjisz "Ｎｏ　ｓａｖｅｄ　ｇａｍｅ　ｄｅｔｅｃｔｅｄ．"

dlabel D_800204F4
    .shiftjisz "Ｓａｖｉｎｇ．．．"

dlabel D_80020508
    .shiftjisz "        ＹＥＳ            ＮＯ"

dlabel D_80020528
    .shiftjisz "ｏｖｅｒｗｒｉｔｅ　ｔｈｉｓ　ｄａｔａ？"

dlabel D_80020554
    .shiftjisz "Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ"

dlabel D_80020574
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ．"

dlabel D_80020590
    .shiftjisz "Ｄｏ　ｎｏｔ　ｒｅｍｏｖｅ"

dlabel D_800205AC
    .shiftjisz "Ｆｏｒｍａｔｔｉｎｇ．．．"

dlabel D_800205C8
    .shiftjisz "Ｐｌｅａｓｅ　ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ　Ｃａｒｄ．"

dlabel D_80020608
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｄａｍａｇｅｄ．"

dlabel D_80020638
    .shiftjisz "Ｐｌｅａｓｅ　ｗａｉｔ．"

dlabel D_80020654
    .shiftjisz "Ｎｏ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｄｅｔｅｃｔｅｄ．"

dlabel D_80020688
    .shiftjisz ""

dlabel D_8002068C
    .shiftjisz "          ＹＥＳ                    ＮＯ"

dlabel D_800206B8
    .shiftjisz "ｆｏｒｍａｔ　ｉｔ　ｎｏｗ？"

dlabel D_800206D8
    .shiftjisz "ｆｏｒｍａｔｔｅｄ．　Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ"

dlabel D_8002070C
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｎｏｔ"

dlabel D_80020734
    .shiftjisz "："

dlabel D_80020738
    .shiftjisz "Ｔ"

dlabel D_8002073C
    .shiftjisz "Ｄ"

dlabel D_80020740
    .shiftjisz "Ａｚｕｒｅ　Ｄｒｅａｍｓ　"

dlabel D_8002075C
    .shiftjisz "Ｃａｎ’ｔ　ｕｓｅ"

dlabel D_80020770
    .shiftjisz "Ｒｅｐｌａｙ"

dlabel D_80020780
    .shiftjisz "Ｄｅｌｅｔｅ"

dlabel D_80020790
    .shiftjisz "Ｃｏｐｙ"

dlabel D_8002079C
    .shiftjisz "Ｌｏａｄ"

dlabel D_800207A8
    .shiftjisz "Ｆ"

dlabel D_800207AC
    .shiftjisz "Ｃｏｎｔｉｎｕｅ ｑｕｅｓｔ ｆｒｏｍ　"

dlabel D_800207D4
    .shiftjisz "Ｆ ＭＡＧＩＣＡＬ ＴＯＷＥＲ"

dlabel D_800207F4
    .shiftjisz "Ｍｏｎｓｔｅｒ　Ｔｏｗｅｒ"

dlabel D_80020810
    .shiftjisz "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｆｕｌｌ．　"

dlabel D_8002083C
    .shiftjisz "Ｃａｎｎｏｔ　ｃｏｐｙ　ｄａｔａ　ｂｅｃａｕｓｅ"

dlabel D_80020870
    .shiftjisz "Ｃａｎｎｏｔ　ｓａｖｅ　ｄａｔａ　ｂｅｃａｕｓｅ"

dlabel D_800208A4
    .shiftjisz "ｄｅｔｅｃｔｅｄ．"

dlabel D_800208B8
    .shiftjisz "Ｎｏ　Ａｚｕｒｅ　Ｄｒｅａｍｓ　ｇａｍｅ　ｄａｔａ"

dlabel D_800208EC
    .shiftjisz "Ｙｏｕ　ｃａｎ　ｃｏｐｙ"

dlabel D_80020908
    /* LAST ENTRY. `.shiftjis`, not `.shiftjisz`: the three bytes after
       this string's NUL are 05 61 64, not the zero padding every other
       entry has, so the terminator and the pad are written out. They are
       the last bytes of .rodata -- the text starts at 0x80020924. */
    .shiftjis "Ｙｏｕ　ｃａｎ　ｓａｖｅ"
    .byte 0x00, 0x05, 0x61, 0x64
