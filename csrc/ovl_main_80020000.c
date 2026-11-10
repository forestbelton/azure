#include "common.h"
#include "main.h"
#include "strings.h"

/* The 0x80020000 overlay's DATA, and nothing else. The nine translation units
   it was built from are in csrc/main/; this file is what could not be split
   with them.

   WHY THE DATA IS NOT CARVED. The overlay's data region is two per-unit
   concatenations, not one: 0x80027E20..0x80028530 is each unit's tables in
   link order, and 0x80028530..0x8002BB6C is each unit's zero-initialised
   state in the same order -- c_server.c's counters, event descriptors,
   card_dir and card_st, then one node blob per window unit. Reproducing that
   with nine objects needs the second group emitted after the first, and
   splat's linker script has one DATA group per segment, with its BSS group
   ahead of TEXT. So every data object stays in one object here, where the
   source order already reproduces both runs, and the units reach what they
   need as externs. The attribution itself IS established -- see
   docs/overlays.md under "Carving the nine" -- it just is not buildable.

   The 59 Shift-JIS message strings below are this file's rodata, at
   0x800200C0..0x80020924. cc1psx emits a string literal into the unit that
   uses it, and the tables that reference them are all here, which is why
   this file is the last .rodata contributor in the segment. */

/* ------------------------------------------------------------------------- */
/* The overlay's own .data. It starts at 0x80027E20, immediately after the text
   end, runs to 0x8002BB6C, and ALL OF IT is defined here -- the splat yaml's
   `.data` subsegment is the whole range and splat writes not one byte of it,
   so an object dropped from this block is dropped from the container. The
   0x494 bytes above 0x8002BB6C are the zero pad to the 0x269000 sector
   boundary and stay in asm, as `ovl_main_80020000_pad`.

   DEFINITION ORDER IS ADDRESS ORDER. cc1psx lays .data objects out in the
   order they are written here, so a new object goes at its address position in
   this block and nowhere else.

   EVERY OBJECT NEEDS AN EXPLICIT INITIALISER, even the all-zero ones: without
   one cc1psx emits `.comm`, the object lands in .bss, and splat's generated
   `.bss` input-section line for this unit sits between the segment's .rodata
   and its .text -- so a forgotten `= {0}` does not shorten the data, it moves
   every function in the overlay. See docs/owning-data.md. */

/* ------------------------------------------------------------------------- */
/* The overlay's 59 Shift-JIS UI messages.

   THEY ARE STRING LITERALS, NOT NAMED OBJECTS, and that is a measurement
   rather than a preference. They occupy 0x800200C0..0x80020924, the top of
   this unit's .rodata, and cc1psx puts them there by itself: gcc 2.7.2 defers
   the string constants of a file-scope initialiser and flushes them after that
   initialiser's data, in REVERSE order of first appearance, merging
   duplicates. Simulating exactly that over the eight tables below -- in
   address order, reversing each table's own first-appearance list -- predicts
   all 59 addresses in the ROM's order, including where the empty string lands
   (35th of that table's 39, because it is 5th to appear) and that it is emitted
   once for all 60 slots that hold it. So the pool was never a separate thing to own: it is
   what these tables' initialisers compile to, which is also why asrc/ no
   longer has a hand-written copy of it.

   The text is in the comments because it cannot be in the source. cc1psx has
   no Shift-JIS input encoding, and `azure shiftjis` -- which is what let the
   old asrc file carry `.shiftjisz "Ｆａｉｌｅｄ..."` -- only runs on asm/ and
   asrc/ sources, never on the assembly cc1psx emits. The escapes and the
   comment are generated from the ROM together; keep them together.

   The `(u8 *)` in each macro is there so the eight tables, all of which are
   `u8 *`, take a `char *` literal without 121 warnings. */

/* 0x800200C0 "Ｆａｉｌｅｄ　ｔｏ　ｄｅｌｅｔｅ　ｄａｔａ．" */
#define MSG_FAILED_TO_DELETE_DATA ((u8 *)"\x82\x65\x82\x81\x82\x89\x82\x8C\x82\x85\x82\x84" \
                                         "\x81\x40\x82\x94\x82\x8F\x81\x40\x82\x84\x82\x85" \
                                         "\x82\x8C\x82\x85\x82\x94\x82\x85\x81\x40\x82\x84" \
                                         "\x82\x81\x82\x94\x82\x81\x81\x44")

/* 0x800200F0 "        ＹＥＳ                ＮＯ" */
#define MSG_YES_NO ((u8 *)"\x20\x20\x20\x20\x20\x20\x20\x20\x82\x78\x82\x64" \
                          "\x82\x72\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" \
                          "\x20\x20\x20\x20\x20\x20\x82\x6D\x82\x6E")

/* 0x80020114 "ｔｈｉｓ　ｄａｔａ？" */
#define MSG_THIS_DATA ((u8 *)"\x82\x94\x82\x88\x82\x89\x82\x93\x81\x40\x82\x84" \
                             "\x82\x81\x82\x94\x82\x81\x81\x48")

/* 0x8002012C "Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ　ｄｅｌｅｔｅ" */
#define MSG_DO_YOU_WISH_TO_DELETE ((u8 *)"\x82\x63\x82\x8F\x81\x40\x82\x99\x82\x8F\x82\x95" \
                                         "\x81\x40\x82\x97\x82\x89\x82\x93\x82\x88\x81\x40" \
                                         "\x82\x94\x82\x8F\x81\x40\x82\x84\x82\x85\x82\x8C" \
                                         "\x82\x85\x82\x94\x82\x85")

/* 0x80020158 "Ｄｅｌｅｔｅ　ｄａｔａ　ａｎｄ　ｒｅｔｒｙ．" */
#define MSG_DELETE_DATA_AND_RETRY ((u8 *)"\x82\x63\x82\x85\x82\x8C\x82\x85\x82\x94\x82\x85" \
                                         "\x81\x40\x82\x84\x82\x81\x82\x94\x82\x81\x81\x40" \
                                         "\x82\x81\x82\x8E\x82\x84\x81\x40\x82\x92\x82\x85" \
                                         "\x82\x94\x82\x92\x82\x99\x81\x44")

/* 0x80020188 "Ｃａｎｎｏｔ　ｏｖｅｒｗｒｉｔｅ　ｔｈｉｓ　ｄａｔａ．" */
#define MSG_CANNOT_OVERWRITE_THIS_DATA ((u8 *)"\x82\x62\x82\x81\x82\x8E\x82\x8E\x82\x8F\x82\x94" \
                                              "\x81\x40\x82\x8F\x82\x96\x82\x85\x82\x92\x82\x97" \
                                              "\x82\x92\x82\x89\x82\x94\x82\x85\x81\x40\x82\x94" \
                                              "\x82\x88\x82\x89\x82\x93\x81\x40\x82\x84\x82\x81" \
                                              "\x82\x94\x82\x81\x81\x44")

/* 0x800201C0 "Ｆａｉｌｅｄ　ｔｏ　ｃｏｐｙ　ｄａｔａ．" */
#define MSG_FAILED_TO_COPY_DATA ((u8 *)"\x82\x65\x82\x81\x82\x89\x82\x8C\x82\x85\x82\x84" \
                                       "\x81\x40\x82\x94\x82\x8F\x81\x40\x82\x83\x82\x8F" \
                                       "\x82\x90\x82\x99\x81\x40\x82\x84\x82\x81\x82\x94" \
                                       "\x82\x81\x81\x44")

/* 0x800201EC "Ｃｏｐｙｉｎｇ．．．" */
#define MSG_COPYING ((u8 *)"\x82\x62\x82\x8F\x82\x90\x82\x99\x82\x89\x82\x8E" \
                           "\x82\x87\x81\x44\x81\x44\x81\x44")

/* 0x80020204 "Ｌｏａｄｉｎｇ．．．" */
#define MSG_LOADING ((u8 *)"\x82\x6B\x82\x8F\x82\x81\x82\x84\x82\x89\x82\x8E" \
                           "\x82\x87\x81\x44\x81\x44\x81\x44")

/* 0x8002021C "Ｆｏｒｍａｔｔｅｄ　ｓｕｃｃｅｓｓｆｕｌｌｙ．" */
#define MSG_FORMATTED_SUCCESSFULLY ((u8 *)"\x82\x65\x82\x8F\x82\x92\x82\x8D\x82\x81\x82\x94" \
                                          "\x82\x94\x82\x85\x82\x84\x81\x40\x82\x93\x82\x95" \
                                          "\x82\x83\x82\x83\x82\x85\x82\x93\x82\x93\x82\x86" \
                                          "\x82\x95\x82\x8C\x82\x8C\x82\x99\x81\x44")

/* 0x8002024C "Ｆａｉｌｅｄ　ｔｏ　ｆｏｒｍａｔ" */
#define MSG_FAILED_TO_FORMAT ((u8 *)"\x82\x65\x82\x81\x82\x89\x82\x8C\x82\x85\x82\x84" \
                                    "\x81\x40\x82\x94\x82\x8F\x81\x40\x82\x86\x82\x8F" \
                                    "\x82\x92\x82\x8D\x82\x81\x82\x94")

/* 0x80020270 "Ｆａｉｌｅｄ　ｔｏ　ｌｏａｄ　ｄａｔａ．" */
#define MSG_FAILED_TO_LOAD_DATA ((u8 *)"\x82\x65\x82\x81\x82\x89\x82\x8C\x82\x85\x82\x84" \
                                       "\x81\x40\x82\x94\x82\x8F\x81\x40\x82\x8C\x82\x8F" \
                                       "\x82\x81\x82\x84\x81\x40\x82\x84\x82\x81\x82\x94" \
                                       "\x82\x81\x81\x44")

/* 0x8002029C "ｓｃｒｅｅｎ．" */
#define MSG_SCREEN ((u8 *)"\x82\x93\x82\x83\x82\x92\x82\x85\x82\x85\x82\x8E" \
                          "\x81\x44")

/* 0x800202AC "ａｔ　ｔｈｅ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｍａｎａｇｅｒ" */
#define MSG_AT_THE_MEMORY_CARD_MANAGER ((u8 *)"\x82\x81\x82\x94\x81\x40\x82\x94\x82\x88\x82\x85" \
                                              "\x81\x40\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92" \
                                              "\x82\x99\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84" \
                                              "\x81\x40\x82\x8D\x82\x81\x82\x8E\x82\x81\x82\x87" \
                                              "\x82\x85\x82\x92")

/* 0x800202E4 "Ｃａｒｄ　ｏｒ　ｄｅｌｅｔｅ　ｕｎｎｅｃｅｓｓａｒｙ　ｄａｔａ" */
#define MSG_CARD_OR_DELETE_UNNECESSARY_DATA ((u8 *)"\x82\x62\x82\x81\x82\x92\x82\x84\x81\x40\x82\x8F" \
                                                   "\x82\x92\x81\x40\x82\x84\x82\x85\x82\x8C\x82\x85" \
                                                   "\x82\x94\x82\x85\x81\x40\x82\x95\x82\x8E\x82\x8E" \
                                                   "\x82\x85\x82\x83\x82\x85\x82\x93\x82\x93\x82\x81" \
                                                   "\x82\x92\x82\x99\x81\x40\x82\x84\x82\x81\x82\x94" \
                                                   "\x82\x81")

/* 0x80020324 "Ｍｅｍｏｒｙ　Ｃａｒｄ．　Ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ" */
#define MSG_MEMORY_CARD_USE_ANOTHER_MEMORY ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                                                  "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x44" \
                                                  "\x81\x40\x82\x74\x82\x93\x82\x85\x81\x40\x82\x81" \
                                                  "\x82\x8E\x82\x8F\x82\x94\x82\x88\x82\x85\x82\x92" \
                                                  "\x81\x40\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92" \
                                                  "\x82\x99")

/* 0x80020364 "ｔｈｅ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｍａｎａｇｅｒ　ｓｃｒｅｅｎ．" */
#define MSG_THE_MEMORY_CARD_MANAGER_SCREEN ((u8 *)"\x82\x94\x82\x88\x82\x85\x81\x40\x82\x6C\x82\x85" \
                                                  "\x82\x8D\x82\x8F\x82\x92\x82\x99\x81\x40\x82\x62" \
                                                  "\x82\x81\x82\x92\x82\x84\x81\x40\x82\x8D\x82\x81" \
                                                  "\x82\x8E\x82\x81\x82\x87\x82\x85\x82\x92\x81\x40" \
                                                  "\x82\x93\x82\x83\x82\x92\x82\x85\x82\x85\x82\x8E" \
                                                  "\x81\x44")

/* 0x800203A4 "ｏｒ　ｄｅｌｅｔｅ　ｕｎｎｅｃｅｓｓａｒｙ　ｄａｔａ　ａｔ" */
#define MSG_OR_DELETE_UNNECESSARY_DATA_AT ((u8 *)"\x82\x8F\x82\x92\x81\x40\x82\x84\x82\x85\x82\x8C" \
                                                 "\x82\x85\x82\x94\x82\x85\x81\x40\x82\x95\x82\x8E" \
                                                 "\x82\x8E\x82\x85\x82\x83\x82\x85\x82\x93\x82\x93" \
                                                 "\x82\x81\x82\x92\x82\x99\x81\x40\x82\x84\x82\x81" \
                                                 "\x82\x94\x82\x81\x81\x40\x82\x81\x82\x94")

/* 0x800203E0 "ｄａｔａ　ｏｒ　ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ　Ｃａｒｄ" */
#define MSG_DATA_OR_USE_ANOTHER_MEMORY_CARD ((u8 *)"\x82\x84\x82\x81\x82\x94\x82\x81\x81\x40\x82\x8F" \
                                                   "\x82\x92\x81\x40\x82\x95\x82\x93\x82\x85\x81\x40" \
                                                   "\x82\x81\x82\x8E\x82\x8F\x82\x94\x82\x88\x82\x85" \
                                                   "\x82\x92\x81\x40\x82\x6C\x82\x85\x82\x8D\x82\x8F" \
                                                   "\x82\x92\x82\x99\x81\x40\x82\x62\x82\x81\x82\x92" \
                                                   "\x82\x84")

/* 0x80020420 "Ｍｅｍｏｒｙ　Ｃａｒｄ．　Ｏｖｅｒｗｒｉｔｅ　ｓａｖｅｄ" */
#define MSG_MEMORY_CARD_OVERWRITE_SAVED ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                                               "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x44" \
                                               "\x81\x40\x82\x6E\x82\x96\x82\x85\x82\x92\x82\x97" \
                                               "\x82\x92\x82\x89\x82\x94\x82\x85\x81\x40\x82\x93" \
                                               "\x82\x81\x82\x96\x82\x85\x82\x84")

/* 0x8002045C "Ｎｏｔ　ｅｎｏｕｇｈ　ｆｒｅｅ　ｂｌｏｃｋｓ　ｏｎ　ｔｈｉｓ" */
#define MSG_NOT_ENOUGH_FREE_BLOCKS_ON_THIS ((u8 *)"\x82\x6D\x82\x8F\x82\x94\x81\x40\x82\x85\x82\x8E" \
                                                  "\x82\x8F\x82\x95\x82\x87\x82\x88\x81\x40\x82\x86" \
                                                  "\x82\x92\x82\x85\x82\x85\x81\x40\x82\x82\x82\x8C" \
                                                  "\x82\x8F\x82\x83\x82\x8B\x82\x93\x81\x40\x82\x8F" \
                                                  "\x82\x8E\x81\x40\x82\x94\x82\x88\x82\x89\x82\x93")

/* 0x8002049C "Ｆａｉｌｅｄ　ｔｏ　ｏｖｅｒｗｒｉｔｅ" */
#define MSG_FAILED_TO_OVERWRITE ((u8 *)"\x82\x65\x82\x81\x82\x89\x82\x8C\x82\x85\x82\x84" \
                                       "\x81\x40\x82\x94\x82\x8F\x81\x40\x82\x8F\x82\x96" \
                                       "\x82\x85\x82\x92\x82\x97\x82\x92\x82\x89\x82\x94" \
                                       "\x82\x85")

/* 0x800204C4 "Ｎｏ　ｓａｖｅｄ　ｇａｍｅ　ｄｅｔｅｃｔｅｄ．" */
#define MSG_NO_SAVED_GAME_DETECTED ((u8 *)"\x82\x6D\x82\x8F\x81\x40\x82\x93\x82\x81\x82\x96" \
                                          "\x82\x85\x82\x84\x81\x40\x82\x87\x82\x81\x82\x8D" \
                                          "\x82\x85\x81\x40\x82\x84\x82\x85\x82\x94\x82\x85" \
                                          "\x82\x83\x82\x94\x82\x85\x82\x84\x81\x44")

/* 0x800204F4 "Ｓａｖｉｎｇ．．．" */
#define MSG_SAVING ((u8 *)"\x82\x72\x82\x81\x82\x96\x82\x89\x82\x8E\x82\x87" \
                          "\x81\x44\x81\x44\x81\x44")

/* 0x80020508 "        ＹＥＳ            ＮＯ" */
#define MSG_YES_NO_2 ((u8 *)"\x20\x20\x20\x20\x20\x20\x20\x20\x82\x78\x82\x64" \
                            "\x82\x72\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" \
                            "\x20\x20\x82\x6D\x82\x6E")

/* 0x80020528 "ｏｖｅｒｗｒｉｔｅ　ｔｈｉｓ　ｄａｔａ？" */
#define MSG_OVERWRITE_THIS_DATA ((u8 *)"\x82\x8F\x82\x96\x82\x85\x82\x92\x82\x97\x82\x92" \
                                       "\x82\x89\x82\x94\x82\x85\x81\x40\x82\x94\x82\x88" \
                                       "\x82\x89\x82\x93\x81\x40\x82\x84\x82\x81\x82\x94" \
                                       "\x82\x81\x81\x48")

/* 0x80020554 "Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ" */
#define MSG_DO_YOU_WISH_TO ((u8 *)"\x82\x63\x82\x8F\x81\x40\x82\x99\x82\x8F\x82\x95" \
                                  "\x81\x40\x82\x97\x82\x89\x82\x93\x82\x88\x81\x40" \
                                  "\x82\x94\x82\x8F")

/* 0x80020574 "Ｍｅｍｏｒｙ　Ｃａｒｄ．" */
#define MSG_MEMORY_CARD ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                               "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x44")

/* 0x80020590 "Ｄｏ　ｎｏｔ　ｒｅｍｏｖｅ" */
#define MSG_DO_NOT_REMOVE ((u8 *)"\x82\x63\x82\x8F\x81\x40\x82\x8E\x82\x8F\x82\x94" \
                                 "\x81\x40\x82\x92\x82\x85\x82\x8D\x82\x8F\x82\x96" \
                                 "\x82\x85")

/* 0x800205AC "Ｆｏｒｍａｔｔｉｎｇ．．．" */
#define MSG_FORMATTING ((u8 *)"\x82\x65\x82\x8F\x82\x92\x82\x8D\x82\x81\x82\x94" \
                              "\x82\x94\x82\x89\x82\x8E\x82\x87\x81\x44\x81\x44" \
                              "\x81\x44")

/* 0x800205C8 "Ｐｌｅａｓｅ　ｕｓｅ　ａｎｏｔｈｅｒ　Ｍｅｍｏｒｙ　Ｃａｒｄ．" */
#define MSG_PLEASE_USE_ANOTHER_MEMORY_CARD ((u8 *)"\x82\x6F\x82\x8C\x82\x85\x82\x81\x82\x93\x82\x85" \
                                                  "\x81\x40\x82\x95\x82\x93\x82\x85\x81\x40\x82\x81" \
                                                  "\x82\x8E\x82\x8F\x82\x94\x82\x88\x82\x85\x82\x92" \
                                                  "\x81\x40\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92" \
                                                  "\x82\x99\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84" \
                                                  "\x81\x44")

/* 0x80020608 "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｄａｍａｇｅｄ．" */
#define MSG_MEMORY_CARD_IS_DAMAGED ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                                          "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x40" \
                                          "\x82\x89\x82\x93\x81\x40\x82\x84\x82\x81\x82\x8D" \
                                          "\x82\x81\x82\x87\x82\x85\x82\x84\x81\x44")

/* 0x80020638 "Ｐｌｅａｓｅ　ｗａｉｔ．" */
#define MSG_PLEASE_WAIT ((u8 *)"\x82\x6F\x82\x8C\x82\x85\x82\x81\x82\x93\x82\x85" \
                               "\x81\x40\x82\x97\x82\x81\x82\x89\x82\x94\x81\x44")

/* 0x80020654 "Ｎｏ　Ｍｅｍｏｒｙ　Ｃａｒｄ　ｄｅｔｅｃｔｅｄ．" */
#define MSG_NO_MEMORY_CARD_DETECTED ((u8 *)"\x82\x6D\x82\x8F\x81\x40\x82\x6C\x82\x85\x82\x8D" \
                                           "\x82\x8F\x82\x92\x82\x99\x81\x40\x82\x62\x82\x81" \
                                           "\x82\x92\x82\x84\x81\x40\x82\x84\x82\x85\x82\x94" \
                                           "\x82\x85\x82\x83\x82\x94\x82\x85\x82\x84\x81\x44")

/* 0x80020688 "" */
#define MSG_EMPTY ((u8 *)"")

/* 0x8002068C "          ＹＥＳ                    ＮＯ" */
#define MSG_YES_NO_3 ((u8 *)"\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x82\x78" \
                            "\x82\x64\x82\x72\x20\x20\x20\x20\x20\x20\x20\x20" \
                            "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20" \
                            "\x82\x6D\x82\x6E")

/* 0x800206B8 "ｆｏｒｍａｔ　ｉｔ　ｎｏｗ？" */
#define MSG_FORMAT_IT_NOW ((u8 *)"\x82\x86\x82\x8F\x82\x92\x82\x8D\x82\x81\x82\x94" \
                                 "\x81\x40\x82\x89\x82\x94\x81\x40\x82\x8E\x82\x8F" \
                                 "\x82\x97\x81\x48")

/* 0x800206D8 "ｆｏｒｍａｔｔｅｄ．　Ｄｏ　ｙｏｕ　ｗｉｓｈ　ｔｏ" */
#define MSG_FORMATTED_DO_YOU_WISH_TO ((u8 *)"\x82\x86\x82\x8F\x82\x92\x82\x8D\x82\x81\x82\x94" \
                                            "\x82\x94\x82\x85\x82\x84\x81\x44\x81\x40\x82\x63" \
                                            "\x82\x8F\x81\x40\x82\x99\x82\x8F\x82\x95\x81\x40" \
                                            "\x82\x97\x82\x89\x82\x93\x82\x88\x81\x40\x82\x94" \
                                            "\x82\x8F")

/* 0x8002070C "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｎｏｔ" */
#define MSG_MEMORY_CARD_IS_NOT ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                                      "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x40" \
                                      "\x82\x89\x82\x93\x81\x40\x82\x8E\x82\x8F\x82\x94")

/* 0x80020734 "：" */
#define MSG_COLON ((u8 *)"\x81\x46")

/* 0x80020738 "Ｔ" */
#define MSG_T ((u8 *)"\x82\x73")

/* 0x8002073C "Ｄ" */
#define MSG_D ((u8 *)"\x82\x63")

/* 0x80020740 "Ａｚｕｒｅ　Ｄｒｅａｍｓ　" */
#define MSG_AZURE_DREAMS ((u8 *)"\x82\x60\x82\x9A\x82\x95\x82\x92\x82\x85\x81\x40" \
                                "\x82\x63\x82\x92\x82\x85\x82\x81\x82\x8D\x82\x93" \
                                "\x81\x40")

/* 0x8002075C "Ｃａｎ’ｔ　ｕｓｅ" */
#define MSG_CANT_USE ((u8 *)"\x82\x62\x82\x81\x82\x8E\x81\x66\x82\x94\x81\x40" \
                            "\x82\x95\x82\x93\x82\x85")

/* 0x80020770 "Ｒｅｐｌａｙ" */
#define MSG_REPLAY ((u8 *)"\x82\x71\x82\x85\x82\x90\x82\x8C\x82\x81\x82\x99")

/* 0x80020780 "Ｄｅｌｅｔｅ" */
#define MSG_DELETE ((u8 *)"\x82\x63\x82\x85\x82\x8C\x82\x85\x82\x94\x82\x85")

/* 0x80020790 "Ｃｏｐｙ" */
#define MSG_COPY ((u8 *)"\x82\x62\x82\x8F\x82\x90\x82\x99")

/* 0x8002079C "Ｌｏａｄ" */
#define MSG_LOAD ((u8 *)"\x82\x6B\x82\x8F\x82\x81\x82\x84")

/* 0x800207A8 "Ｆ" */
#define MSG_F ((u8 *)"\x82\x65")

/* 0x800207AC "Ｃｏｎｔｉｎｕｅ ｑｕｅｓｔ ｆｒｏｍ　" */
#define MSG_CONTINUE_QUEST_FROM ((u8 *)"\x82\x62\x82\x8F\x82\x8E\x82\x94\x82\x89\x82\x8E" \
                                       "\x82\x95\x82\x85\x20\x82\x91\x82\x95\x82\x85\x82" \
                                       "\x93\x82\x94\x20\x82\x86\x82\x92\x82\x8F\x82\x8D" \
                                       "\x81\x40")

/* 0x800207D4 "Ｆ ＭＡＧＩＣＡＬ ＴＯＷＥＲ" */
#define MSG_F_MAGICAL_TOWER ((u8 *)"\x82\x65\x20\x82\x6C\x82\x60\x82\x66\x82\x68\x82" \
                                   "\x62\x82\x60\x82\x6B\x20\x82\x73\x82\x6E\x82\x76" \
                                   "\x82\x64\x82\x71")

/* 0x800207F4 "Ｍｏｎｓｔｅｒ　Ｔｏｗｅｒ" */
#define MSG_MONSTER_TOWER ((u8 *)"\x82\x6C\x82\x8F\x82\x8E\x82\x93\x82\x94\x82\x85" \
                                 "\x82\x92\x81\x40\x82\x73\x82\x8F\x82\x97\x82\x85" \
                                 "\x82\x92")

/* 0x80020810 "Ｍｅｍｏｒｙ　Ｃａｒｄ　ｉｓ　ｆｕｌｌ．　" */
#define MSG_MEMORY_CARD_IS_FULL ((u8 *)"\x82\x6C\x82\x85\x82\x8D\x82\x8F\x82\x92\x82\x99" \
                                       "\x81\x40\x82\x62\x82\x81\x82\x92\x82\x84\x81\x40" \
                                       "\x82\x89\x82\x93\x81\x40\x82\x86\x82\x95\x82\x8C" \
                                       "\x82\x8C\x81\x44\x81\x40")

/* 0x8002083C "Ｃａｎｎｏｔ　ｃｏｐｙ　ｄａｔａ　ｂｅｃａｕｓｅ" */
#define MSG_CANNOT_COPY_DATA_BECAUSE ((u8 *)"\x82\x62\x82\x81\x82\x8E\x82\x8E\x82\x8F\x82\x94" \
                                            "\x81\x40\x82\x83\x82\x8F\x82\x90\x82\x99\x81\x40" \
                                            "\x82\x84\x82\x81\x82\x94\x82\x81\x81\x40\x82\x82" \
                                            "\x82\x85\x82\x83\x82\x81\x82\x95\x82\x93\x82\x85")

/* 0x80020870 "Ｃａｎｎｏｔ　ｓａｖｅ　ｄａｔａ　ｂｅｃａｕｓｅ" */
#define MSG_CANNOT_SAVE_DATA_BECAUSE ((u8 *)"\x82\x62\x82\x81\x82\x8E\x82\x8E\x82\x8F\x82\x94" \
                                            "\x81\x40\x82\x93\x82\x81\x82\x96\x82\x85\x81\x40" \
                                            "\x82\x84\x82\x81\x82\x94\x82\x81\x81\x40\x82\x82" \
                                            "\x82\x85\x82\x83\x82\x81\x82\x95\x82\x93\x82\x85")

/* 0x800208A4 "ｄｅｔｅｃｔｅｄ．" */
#define MSG_DETECTED ((u8 *)"\x82\x84\x82\x85\x82\x94\x82\x85\x82\x83\x82\x94" \
                            "\x82\x85\x82\x84\x81\x44")

/* 0x800208B8 "Ｎｏ　Ａｚｕｒｅ　Ｄｒｅａｍｓ　ｇａｍｅ　ｄａｔａ" */
#define MSG_NO_AZURE_DREAMS_GAME_DATA ((u8 *)"\x82\x6D\x82\x8F\x81\x40\x82\x60\x82\x9A\x82\x95" \
                                             "\x82\x92\x82\x85\x81\x40\x82\x63\x82\x92\x82\x85" \
                                             "\x82\x81\x82\x8D\x82\x93\x81\x40\x82\x87\x82\x81" \
                                             "\x82\x8D\x82\x85\x81\x40\x82\x84\x82\x81\x82\x94" \
                                             "\x82\x81")

/* 0x800208EC "Ｙｏｕ　ｃａｎ　ｃｏｐｙ" */
#define MSG_YOU_CAN_COPY ((u8 *)"\x82\x78\x82\x8F\x82\x95\x81\x40\x82\x83\x82\x81" \
                                "\x82\x8E\x81\x40\x82\x83\x82\x8F\x82\x90\x82\x99")

/* 0x80020908 "Ｙｏｕ　ｃａｎ　ｓａｖｅ" */
#define MSG_YOU_CAN_SAVE ((u8 *)"\x82\x78\x82\x8F\x82\x95\x81\x40\x82\x83\x82\x81" \
                                "\x82\x8E\x81\x40\x82\x93\x82\x81\x82\x96\x82\x85")

/* ------------------------------------------------------------------------- */
/* 0x80027E20 */
/* The 8-frame animation script reached through D_80028074[1]; the last
   step is the op-1 jump back to the top. Nothing names it directly. */
UnkAnimStep D_80027E20[9] = {
    {0x6, 2, D_80028448},
    {0x4, 2, D_80028454},
    {0x2, 2, D_80028460},
    {0x4, 2, D_8002846C},
    {0x6, 2, D_80028478},
    {0x4, 2, D_80028484},
    {0x2, 2, D_80028490},
    {0x4, 2, D_8002849C},
    {0x0, 1, D_80027E20},
};

/* 0x80027E68 */
/* The 1-frame script reached through D_80028074[0], and handed to
   func_8004CBFC directly by func_80026B18 and func_80026CF0. */
UnkAnimStep D_80027E68[2] = {
    {0x2, 2, D_80028448},
    {0x0, 0, D_80027E68},
};

/* 0x80027E78 */
/* Five 16-entry 5551 CLUTs, one 0x20-byte row each; set_save_icon copies
   the selected row into its primitive at +0x60 as an UnkBlock20. */
u8 SAVE_ICON_CLUTS[5][0x20] = {{0x00, 0x00, 0x5A, 0x6F, 0xDF, 0x53, 0x3A, 0x26, 0x04, 0x6A, 0x04, 0x55, 0xE4, 0x3C, 0x85, 0x20,
                                0xCC, 0x10, 0x3F, 0x1F, 0x9A, 0x11, 0x34, 0x15, 0x73, 0x5A, 0x93, 0x7F, 0x30, 0x15, 0x66, 0x0C},
                               {0x00, 0x00, 0xFF, 0x7F, 0xFB, 0x5B, 0xD6, 0x47, 0x2D, 0x43, 0x49, 0x2E, 0x46, 0x19, 0x82, 0x0C,
                                0x9F, 0x57, 0xBF, 0x46, 0x1D, 0x1D, 0xD7, 0x62, 0xA6, 0x1C, 0x4C, 0x31, 0x65, 0x14, 0x43, 0x0C},
                               {0x00, 0x00, 0x9C, 0x73, 0xDD, 0x5A, 0x7A, 0x49, 0x1E, 0x27, 0x97, 0x15, 0xF3, 0x51, 0x67, 0x14,
                                0xAF, 0x10, 0x73, 0x37, 0x89, 0x2E, 0x86, 0x25, 0xC4, 0x10, 0xDD, 0x4F, 0x94, 0x30, 0x68, 0x18},
                               {0x00, 0x00, 0xFD, 0x6B, 0xFA, 0x52, 0x56, 0x7F, 0x6E, 0x72, 0x4F, 0x6D, 0xEA, 0x58, 0x84, 0x1C,
                                0xDF, 0x76, 0x7C, 0x51, 0x90, 0x38, 0x69, 0x18, 0x8C, 0x24, 0x34, 0x3E, 0x6E, 0x29, 0xC7, 0x14},
                               {0x00, 0x00, 0x6D, 0x20, 0x93, 0x3C, 0xD9, 0x58, 0x9A, 0x5D, 0x5B, 0x66, 0x2D, 0x09, 0xB4, 0x05,
                                0x5B, 0x02, 0xDB, 0x0A, 0x7B, 0x17, 0x42, 0x08, 0x29, 0x25, 0x52, 0x4A, 0x9C, 0x6F, 0x1F, 0x00}};

/* 0x80027F18 */
const char *MEM_CARD_NAMES[2] = {
    MEM_CARD0,
    MEM_CARD1,
};

/* 0x80027F20 */
char MEM_CARD_PREFIX[16] = "BASLUS-00614";

/* 0x80027F30 */
const char *MEM_CARD_FILENAMES[6] = {
    D_8002009C, // 00
    D_80020098, // 01
    D_80020094, // 02
    D_80020090, // 03
    D_8002008C, // 04
    D_80020088, // TMP
};

UnkBlock24 D_80027F48 = {{0x00, 0x38, 0x00, 0x00, 0xbc, 0xbc, 0x9c, 0x00, 0x00, 0x00, 0xa0, 0x70, 0xbc, 0xbc, 0x9c, 0x00,
                          0xbc, 0xbc, 0x9c, 0x00, 0xbc, 0xbc, 0x9c, 0x00}};
/* 0x80027F60 */
/* func_800491F4 index list for func_80022B20: 7 entries, then 0x07
   terminates it (the loop runs while the byte is < the count it is
   passed). */
u8 D_80027F60[0x8] = {0x05, 0x06, 0x00, 0x01, 0x02, 0x03, 0x04, 0x07};

/* 0x80027F68 */
/* func_800491F4 index list for func_800237C4, 18 entries then the 0x12
   terminator. */
u8 D_80027F68[0x14] = {0x00, 0x01, 0x0A, 0x0B, 0x0C, 0x0D, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0E, 0x0F,
                       0x10, 0x11, 0x12, 0x00};

/* 0x80027F7C */
/* Six 12-byte cel records, stored by address into a primitive by
   func_80023D64 (`q->unk_00 = (s32)D_80027F7C`). */
u8 D_80027F7C[0x48] = {0x41, 0x2C, 0x02, 0xFC, 0x17, 0x00, 0xC0, 0x7C, 0x10, 0x00, 0x08, 0x10, 0x41, 0x2C, 0x02, 0x04,
                       0x17, 0x00, 0xC0, 0x7C, 0x10, 0x10, 0x08, 0x10, 0x40, 0x2C, 0x26, 0xFC, 0x17, 0x00, 0xC0, 0x7C,
                       0x10, 0x00, 0x08, 0x10, 0x40, 0x2C, 0x26, 0x04, 0x17, 0x00, 0xC0, 0x7C, 0x10, 0x10, 0x08, 0x10,
                       0x40, 0x2C, 0xF1, 0x00, 0x17, 0x00, 0xC0, 0x7C, 0x68, 0x20, 0x08, 0x10, 0xC0, 0x2C, 0x2A, 0xFC,
                       0x17, 0x00, 0xC1, 0x7C, 0x20, 0x00, 0x08, 0x08};

/* 0x80027FC4 */
/* func_800491F4 index list for func_80023ECC, 9 entries then the 0x0A
   terminator. */
u8 D_80027FC4[0xC] = {0x07, 0x08, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x09, 0x0A, 0x00, 0x94};

/* 0x80027FD0 */
/* Walked one byte per row by func_800242D8 as `D_80027FD0[i++]`, and read
   by func_80024298. */
u8 D_80027FD0[0x4] = {0x00, 0x01, 0x02, 0x03};

/* 0x80027FD4 */
/* Nine cel-record pointers; func_80024390 reads [i] and [8]. */
u8 *D_80027FD4[9] = {
    D_800284A8,
    D_800284B4,
    D_800284C0,
    D_800284CC,
    D_800284D8,
    D_800284E4,
    D_800284F0,
    D_800284FC,
    D_80028520,
};

/* 0x80027FF8 */
/* func_800491F4 index list for func_80024C48, 21 entries then the 0x15
   terminator. splat
   labels 0x80028000 because that is where the old `bin` segment began;
   nothing references it, so the list is one object here. */
u8 D_80027FF8[0x18] = {0x03, 0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
                       0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x6F, 0x73};

/* 0x80028010 */
/* Eight sound ids, polled one per bit by func_80025E54. */
s16 D_80028010[8] = {
    0x146E,
    0x1472,
    0x1470,
    0x147A,
    0x1476,
    0x1474,
    0x1478,
    0x1391,
};

/* 0x80028020 */
/* A 68-entry byte table func_80025ECC sums through, indexed by the bytes
   at 0x800133A4. */
u8 D_80028020[0x44] = {0x00, 0x00, 0x01, 0x02, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x00, 0x00, 0x00,
                       0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                       0x00, 0x00, 0x00, 0x00};

/* 0x80028064 */
/* The UnkFour template init_save_file_header copies with lwl/lwr. */
u8 SAVE_FILE_MAGIC[0x4] = {0x53, 0x43, 0x13, 0x03};

/* 0x80028068 */
/* The two-slot pick table, indexed [p[9]][i] in func_800269B4. */
u8 D_80028068[2][2] = {{0x01, 0x00},
                       {0x00, 0x01}};

/* 0x8002806C */
/* The two colours those two slots select. */
s32 D_8002806C[2] = {
    0x00404040,
    0x00808080,
};

/* 0x80028074 */
/* The two animation scripts those two slots select. */
u8 *D_80028074[2] = {
    (u8 *)D_80027E68,
    (u8 *)D_80027E20,
};

/* 0x8002807C */
/* Read as `(s32 *)` by func_80026D78 -- a pair of zeros. */
u8 D_8002807C[0x8] = {0};

/* 0x80028084 */
/* The matching pair of ones. */
u8 D_80028084[0x8] = {0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* 0x8002808C */
/* func_800491F4 index list for func_80026DC8, 4 entries then the 0x04
   terminator; three more bytes follow it inside the object. */
u8 D_8002808C[0x8] = {0x00, 0x01, 0x02, 0x03, 0x04, 0x08, 0x00, 0x0C};

/* 0x80028094 */
/* func_800491F4 index list for func_80027254, 4 entries then the 0x04
   terminator. */
u8 D_80028094[0x8] = {0x02, 0x00, 0x01, 0x03, 0x04, 0x04, 0x6D, 0x70};

/* 0x8002809C */
/* The six input handlers, indexed by func_8002168C's return. Entry 0 is the
   null word: func_800277BC only indexes this after testing `n != 0`, so the
   slot exists to make the index direct rather than biased. The handlers land
   in p[-4], which is an s32 slot because p is an s32 *, so the store casts --
   the same `(s32)func_...` idiom the handlers themselves use. */
void (*D_8002809C[6])(s32 *) = {
    NULL,
    func_80027528,
    func_80027528,
    func_800276EC,
    func_80027734,
    func_80027778,
};

/* 0x800280B4 */
/* The 20-message table: five Shift-JIS lines from the rodata pool plus
   four bytes each. 0x80020688 is the empty line. */
UnkStrSet gCardMessages[20] = {
    /*  0 */ {{MSG_MEMORY_CARD_IS_NOT, MSG_FORMATTED_DO_YOU_WISH_TO, MSG_FORMAT_IT_NOW, MSG_YES_NO_3, MSG_EMPTY}, 2, 1, 0, 0},
    /*  1 */ {{MSG_NO_MEMORY_CARD_DETECTED, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  2 */ {{MSG_PLEASE_WAIT, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  3 */ {{MSG_MEMORY_CARD_IS_DAMAGED, MSG_PLEASE_USE_ANOTHER_MEMORY_CARD, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  4 */ {{MSG_FORMATTING, MSG_DO_NOT_REMOVE, MSG_MEMORY_CARD, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  5 */ {{MSG_DO_YOU_WISH_TO, MSG_OVERWRITE_THIS_DATA, MSG_YES_NO_2, MSG_EMPTY, MSG_EMPTY}, 2, 0, 0, 0},
    /*  6 */ {{MSG_SAVING, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  7 */ {{MSG_NO_SAVED_GAME_DETECTED, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  8 */ {{MSG_FAILED_TO_OVERWRITE, MSG_MEMORY_CARD, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /*  9 */ {{MSG_NOT_ENOUGH_FREE_BLOCKS_ON_THIS, MSG_MEMORY_CARD_OVERWRITE_SAVED, MSG_DATA_OR_USE_ANOTHER_MEMORY_CARD, MSG_OR_DELETE_UNNECESSARY_DATA_AT, MSG_THE_MEMORY_CARD_MANAGER_SCREEN}, 0, 0, 0, 0},
    /* 10 */ {{MSG_NOT_ENOUGH_FREE_BLOCKS_ON_THIS, MSG_MEMORY_CARD_USE_ANOTHER_MEMORY, MSG_CARD_OR_DELETE_UNNECESSARY_DATA, MSG_AT_THE_MEMORY_CARD_MANAGER, MSG_SCREEN}, 0, 0, 0, 0},
    /* 11 */ {{MSG_FAILED_TO_LOAD_DATA, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 12 */ {{MSG_FAILED_TO_FORMAT, MSG_MEMORY_CARD, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 13 */ {{MSG_FORMATTED_SUCCESSFULLY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 14 */ {{MSG_LOADING, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 15 */ {{MSG_COPYING, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 16 */ {{MSG_FAILED_TO_COPY_DATA, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 17 */ {{MSG_CANNOT_OVERWRITE_THIS_DATA, MSG_DELETE_DATA_AND_RETRY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
    /* 18 */ {{MSG_DO_YOU_WISH_TO_DELETE, MSG_THIS_DATA, MSG_YES_NO, MSG_EMPTY, MSG_EMPTY}, 2, 0, 0, 0},
    /* 19 */ {{MSG_FAILED_TO_DELETE_DATA, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY, MSG_EMPTY}, 0, 0, 0, 0},
};

/* 0x80028294 */
/* The four fragments set_save_title builds its save title from. splat names
   the fourth separately, D_800282A0, because set_save_title reaches it with
   its own %hi/%lo pair -- but the function already indexes this table past
   its old three-entry bound to get at it, and %lo(D_80028294 + 12) IS
   %lo(D_800282A0), so the one array is both the honest spelling and
   byte-identical. Measured 2026-11-09. */
u8 *D_80028294[4] = {
    MSG_AZURE_DREAMS,
    MSG_D,
    MSG_T,
    MSG_COLON,
};

/* 0x800282A4 */
/* The two headings func_80023B18 draws, indexed by
   `get_free_blocks() >= 3`. */
u8 *D_800282A4[2] = {
    MSG_CANT_USE,
    MSG_EMPTY,
};

/* 0x800282AC */
/* Unreferenced 4-byte string.
   "ｎ" */
u8 D_800282AC[0x4] =
    "\x82\x8E\x00\x00";

/* 0x800282B0 */
/* func_80023D64's subtitle.
   "ｑｕｅｓｔ" */
u8 D_800282B0[0xC] =
    "\x82\x91\x82\x95\x82\x85\x82\x93\x82\x94\x00\x00";

/* 0x800282BC */
/* The four menu entries func_8002352C draws. */
u8 *D_800282BC[4] = {
    MSG_LOAD,
    MSG_COPY,
    MSG_DELETE,
    MSG_REPLAY,
};

/* 0x800282CC */
/* func_80027054's heading.
   "Ｓａｖｅ" */
u8 D_800282CC[0xC] =
    "\x82\x72\x82\x81\x82\x96\x82\x85\x00\x00\x00\x00";

/* 0x800282D8 */
/* Appended to the save-row buffer by func_80024390.
   "　ＮＡＭＥ：" */
u8 D_800282D8[0x10] =
    "\x81\x40\x82\x6D\x82\x60\x82\x6C\x82\x64\x81\x46\x00\x00\x00\x00";

/* 0x800282E8 */
/* The save-row template func_80024390 copies out and fills in; the runs
   of spaces are where the numbers go.
   "　　　　　　　　　　　　　　　　ＭＯＮＥＹ                    "
   "　　　＃ ＯＦ ＧＡＭＥ 　　　   ＴＩＭＥ　　　　　：    ：　　"
   "　　　　　　　　　　　　　　　　　  　　Ｂｕｉｌｄｉｎｇ    　" */
u8 D_800282E8[0xC0] =
    "\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40"
    "\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40"
    "\x82\x6C\x82\x6E\x82\x6D\x82\x64\x82\x78\x20\x20\x20\x20\x20\x20"
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x0A\x81"
    "\x40\x81\x40\x81\x40\x81\x94\x20\x82\x6E\x82\x65\x20\x82\x66\x82"
    "\x60\x82\x6C\x82\x64\x20\x81\x40\x81\x40\x81\x40\x20\x20\x20\x82"
    "\x73\x82\x68\x82\x6C\x82\x64\x81\x40\x81\x40\x81\x40\x81\x40\x81"
    "\x40\x81\x46\x20\x20\x20\x20\x81\x46\x81\x40\x81\x40\x0A\x81\x40"
    "\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40"
    "\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40\x81\x40"
    "\x20\x20\x81\x40\x81\x40\x82\x61\x82\x95\x82\x89\x82\x8C\x82\x84"
    "\x82\x89\x82\x8E\x82\x87\x20\x20\x20\x20\x81\x40\x00\x00\x00\x00";

/* 0x800283A8 */
/* Four more fragments for the same buffer. */
u8 *D_800283A8[4] = {
    MSG_MONSTER_TOWER,
    MSG_F_MAGICAL_TOWER,
    MSG_CONTINUE_QUEST_FROM,
    MSG_F,
};

/* 0x800283B8 */
/* The three error lines func_80024390 and func_80024818 pick between. */
u8 *D_800283B8[3] = {
    MSG_CANNOT_SAVE_DATA_BECAUSE,
    MSG_CANNOT_COPY_DATA_BECAUSE,
    MSG_MEMORY_CARD_IS_FULL,
};

/* 0x800283C4 */
/* The fourth, inline (func_80024818).
   "Ｏｕｔ　ｏｆ　ｆｒｅｅ　ｂｌｏｃｋｓ．" */
u8 D_800283C4[0x28] =
    "\x82\x6E\x82\x95\x82\x94\x81\x40\x82\x8F\x82\x86\x81\x40\x82\x86"
    "\x82\x92\x82\x85\x82\x85\x81\x40\x82\x82\x82\x8C\x82\x8F\x82\x83"
    "\x82\x8B\x82\x93\x81\x44\x00\x00";

/* 0x800283EC */
/* The two-line default pair, for func_80024818 and func_800249B4. */
u8 *D_800283EC[2] = {
    MSG_NO_AZURE_DREAMS_GAME_DATA,
    MSG_DETECTED,
};

/* 0x800283F4 */
/* The indexed alternative. */
u8 *D_800283F4[2] = {
    MSG_YOU_CAN_SAVE,
    MSG_YOU_CAN_COPY,
};

/* 0x80020921..0x80020924 -- THREE BYTES THAT BELONG TO NO OBJECT, which is why
   they are spelled as raw section content rather than as C.

   The message above ends with its NUL at 0x80020920 and the overlay's text
   starts at 0x80020924, so three bytes sit in between -- and they are
   05 61 64, not zeros. They are not the message's padding and not an object of
   their own: the original toolchain filled the gap it needed in order to
   4-align the text with whatever was in its buffer. Every other section
   boundary in the project that does not land on 4 looks the same:

       town.bin  0x00C19D   2e 73 65   ".se"
       town.bin  0x00D521   35 2f 30   "5/0"
       main.bin  0x25D921   05 61 64   "\5ad"   <- this one
       main.bin  0x26989D   20 42 30   " B0"    (the debug twin, after its
                                                 last string, "memory.c")

   Four boundaries over two containers, a printable-ASCII fragment every time
   and never the same fragment twice; 05 61 64 occurs nowhere else in
   main.bin. What the fragments are off-cuts OF is not known.

   It could not be a C object in any case. cc1psx emits `.align 2` before every
   file-scope object -- measured, and `__attribute__((aligned(1)))` does not
   change it -- so a named 3-byte array would be pushed to 0x80020924, on top
   of the first function. A deferred string constant gets no TRAILING align,
   which is what leaves 0x80020921 reachable from here at all.

   The closing `.data` is belt and braces. It is not needed HERE -- cc1psx has
   just flushed those constants into .rdata itself, so it emits `.data` of its
   own accord for the next object, and dropping the line still builds
   byte-identical (measured). But cc1psx only emits a section directive when
   the section changes by ITS reckoning, and this block is invisible to that,
   so an `__asm__` like this placed anywhere else in the data block would send
   the object after it into .rdata. Leave the restore in. */
__asm__(".rdata\n"
        "\t.byte 0x05, 0x61, 0x64\n"
        "\t.data");

/* 0x800283FC */
/* Appended after it.
   "　ｍｏｒｅ　ｆｉｌｅ（ｓ）" */
u8 D_800283FC[0x1C] =
    "\x81\x40\x82\x8D\x82\x8F\x82\x92\x82\x85\x81\x40\x82\x86\x82\x89"
    "\x82\x8C\x82\x85\x81\x69\x82\x93\x81\x6A\x00\x00";

/* 0x80028418 */
/* Two cel records, stored by address by func_80026C94. */
u8 D_80028418[0x18] = {0x40, 0x2C, 0x2C, 0x04, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0x80, 0x10, 0x10, 0xC0, 0x2C, 0x00, 0x00,
                       0x9E, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x40, 0x60};

/* 0x80028430 */
/* Two more, from the same function. */
u8 D_80028430[0x18] = {0x40, 0x2C, 0x2C, 0x04, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0x90, 0x10, 0x10, 0xC0, 0x2C, 0x00, 0x00,
                       0x9E, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x40, 0x60};

/* 0x80028448 */
/* The 12-byte cel records the tables above point at. Nothing in this unit
   reads one: each is stored by address into a primitive field, or handed
   to func_8004CBFC as an UnkAnimStep arg, and consumed outside the
   overlay. Four objects hold more than one record. */
u8 D_80028448[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x00, 0x80, 0x30, 0x40};

/* 0x80028454 */
u8 D_80028454[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x30, 0x40};

/* 0x80028460 */
u8 D_80028460[0xC] = {0xC0, 0x2C, 0x10, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x60, 0x80, 0x20, 0x40};

/* 0x8002846C */
u8 D_8002846C[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x00, 0xC0, 0x30, 0x40};

/* 0x80028478 */
u8 D_80028478[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x30, 0xC0, 0x30, 0x40};

/* 0x80028484 */
u8 D_80028484[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9E, 0x00, 0x40, 0x7D, 0x00, 0x80, 0x30, 0x40};

/* 0x80028490 */
u8 D_80028490[0xC] = {0xC0, 0x2C, 0x10, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x60, 0xC0, 0x20, 0x40};

/* 0x8002849C */
u8 D_8002849C[0xC] = {0xC0, 0x2C, 0x08, 0x18, 0x9E, 0x00, 0x40, 0x7D, 0x00, 0xC0, 0x30, 0x40};

/* 0x800284A8 */
u8 D_800284A8[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0xE0, 0x10, 0x10};

/* 0x800284B4 */
u8 D_800284B4[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0xF0, 0x10, 0x10};

/* 0x800284C0 */
u8 D_800284C0[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xB0, 0x10, 0x10};

/* 0x800284CC */
u8 D_800284CC[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xA0, 0x10, 0x10};

/* 0x800284D8 */
u8 D_800284D8[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xC0, 0x10, 0x10};

/* 0x800284E4 */
u8 D_800284E4[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xD0, 0x10, 0x10};

/* 0x800284F0 */
u8 D_800284F0[0xC] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x40, 0xE0, 0x10, 0x10};

/* 0x800284FC */
u8 D_800284FC[0x24] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x50, 0xE0, 0x10, 0x10, 0xC0, 0x2C, 0x00, 0x00,
                       0x9E, 0x00, 0x40, 0x7D, 0x60, 0xE0, 0x10, 0x10, 0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D,
                       0x50, 0xF0, 0x10, 0x10};

/* 0x80028520 */
u8 D_80028520[0x10] = {0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x40, 0xF0, 0x10, 0x10, 0x00, 0x94, 0x08, 0x00};

/* 0x80028530 */
/* Four two-word counters, one word per card slot and indexed by
   is_second_card_slot(): func_8002168C counts repeats of the card state in
   D_80028530 and remembers the state itself in D_80028538, and func_800217D4
   does the same for the button value in D_80028540 / D_80028548. Two
   elements, not two scalars -- measured 2026-11-09, `D_80028530[i]` and the
   old `(&D_80028530)[i]` spelling build byte-identical. */
s32 D_80028530[2] = {0, 0};

/* 0x80028538 */
s32 D_80028538[2] = {0, 0};

/* 0x80028540 */
s32 D_80028540[2] = {0, 0};

/* 0x80028548 */
s32 D_80028548[2] = {0, 0};

/* 0x80028550 */
/* The eight card event descriptors OpenEvent fills in. */
long gSwCardEOFEvent = 0;

/* 0x80028554 */
long gSwCardErrorEvent = 0;

/* 0x80028558 */
long gSwCardTimeoutEvent = 0;

/* 0x8002855C */
long gSwCardNewEvent = 0;

/* 0x80028560 */
long gHwCardEOFEvent = 0;

/* 0x80028564 */
long gHwCardErrorEvent = 0;

/* 0x80028568 */
long gHwCardTimeoutEvent = 0;

/* 0x8002856C */
long gHwCardNewEvent = 0;

/* 0x80028570 */
/* firstfile2/nextfile walk this 15-row directory buffer 0x28 bytes at a
   time (count_card_files); count_free_blocks reads row word 6 and
   card_file_exists compares the 0x15-byte name. */
struct DIRENTRY card_dir[15] = {{{0}}};

/* 0x800287C8 */
/* The card state machine's step. */
s32 card_st = 0;

/* 0x800287CC */
s32 card_slot = 0;

/* 0x800287D0 */
/* The two per-slot outputs of those counters: func_8002168C writes
   D_800287D0[i] and func_800217D4 writes D_800287D8[i], and start_card_server
   clears all four. */
s32 D_800287D0[2] = {0, 0};

/* 0x800287D8 */
s32 D_800287D8[2] = {0, 0};

/* 0x800287E0 */
s32 card_evflg = 0;

/* 0x800287E4 */
CARD_STATE gCardState = CARD_STATE_OK;

/* 0x800287E8 */
/* One 0x9E8-byte UnkNode buffer -- func_8003FE78 is passed 0x27A words.
   D_80028808 names its +0x20 word, the only part the overlay reaches by
   name, so the buffer is spelled as three objects. */
u8 D_800287E8[0x20] = {0};

/* 0x80028808 */
s32 D_80028808 = 0;

/* 0x8002880C */
/* The rest of it. */
u8 D_8002880C[0x9C4] = {0};

/* 0x800291D0 */
/* A 0x112-word UnkNode buffer. */
u8 D_800291D0[0x448] = {0};

/* 0x80029618 */
/* Five 0x3E4-byte rows, one 0xF9-word UnkNode each (func_80023FF0
   indexes them as D_80029618[b]). */
u8 D_80029618[5][0x3E4] = {{0}};

/* 0x8002A98C */
/* Four bytes of non-zero residue between two buffers, with no symbol and
   no reader. See docs/owning-data.md on why this region cannot be .bss. */
u32 D_8002A98C = 0xF97F8BEF;

/* 0x8002A990 */
/* A 0x394-word UnkNode buffer. */
u8 D_8002A990[0xE50] = {0};

/* 0x8002B7E0 */
/* A 0x1C-word UnkNode buffer. */
u8 D_8002B7E0[0x70] = {0};

/* 0x8002B850 */
/* A 0x45-word UnkNode buffer. */
u8 D_8002B850[0x114] = {0};

/* 0x8002B964 */
/* The second four bytes of residue. */
u32 D_8002B964 = 0x6F7C7327;

/* 0x8002B968 */
/* A 0x68-word UnkNode buffer. */
u8 D_8002B968[0x1A0] = {0};

/* 0x8002BB08 */
/* A 0x19-word UnkNode buffer, and the last object in the overlay: the
   0x494 bytes above it are the zero pad to the 0x269000 sector boundary. */
u8 D_8002BB08[0x64] = {0};
