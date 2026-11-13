#include "common.h"
#include "main_twin.h"

/* THE TWIN'S RODATA, 0x80400090..0x804008A0 -- 0x810 bytes, 98 objects.
 *
 * This is csrc/ovl_main_80400000.c's data, and it is in a FILE OF ITS OWN
 * because it cannot be in that file at all. The reason is measured and it
 * corrects docs/owning-data.md SS9.1 and SS9.4, which assumed otherwise.
 *
 * AT -G8 ALL FILE-SCOPE DATA IS EMITTED BEFORE ALL TEXT, so a compiler-emitted
 * constant can never precede a file-scope object in the same unit. mips.c's
 * `mips_asm_file_start` diverts the TEXT to a temp file when TARGET_GP_OPT is
 * on -- "so that data decls come before references to the data", for the
 * assembler's own small-data optimisation -- and `mips_asm_file_end` appends
 * it after everything the data stream wrote (mips.c:4181 and :4237;
 * `function_prologue`/`function_epilogue` are what switch `asm_out_file`
 * between the two streams). Measured with tools/psx272gcc on
 *
 *     const char A[12] = "hello";
 *     int f(int x) { switch (x) { ... } }    <- five arms, a jump table
 *     const char B[12] = "world";
 *
 *   -G8: A, B, then the text, then the jump table.  <- NOT source order
 *   -G0: A, then the jump table, then B.            <- source order
 *
 * The twin's `.rodata` runs [jtbl_8040007C at 0x8040007C][these 98 objects at
 * 0x80400090], and jtbl_8040007C is the jump table func_804016D0's switch
 * emits. Defining these objects in that unit puts all 0x810 of them in front
 * of the table however the file is ordered: measured, `size -A` read the
 * right total, 2084, and main.bin came out `0x79a of 0x275800 bytes differ,
 * first at 0x26907c` -- the table's own address. A unit gets ONE `.rodata`
 * subsegment, so the two runs cannot be two windows of one unit either. They
 * are two units, which is what docs/owning-data.md SS6 says to do with
 * disjoint ranges, and the yaml orders the two input sections.
 *
 * (SS9.3's `.align 3` was a second, independent reason for a THIRD unit below
 * the table, and it is gone: the table is at a 4-aligned address because this
 * block's toolchain did not 8-align jump tables, so
 * tools/decomp/jtbl_align.py rewrites that one directive and the function
 * unit holds both the 0x7C and the table. 0x810 is itself 8-aligned, which is
 * why the wrong-order measurement above still totalled 2084 and not 2088.)
 *
 * THIS UNIT IS -G0 AND BUILT BY tools/cdkgcc, not by the twin's own compiler:
 * it matches the Makefile's `build/csrc/ovl_%.o` pattern rule and takes no
 * target-specific override. For data that is a difference without a
 * consequence -- `.rdata`, `.align 2`, `.ascii` either way, and the containers
 * say so -- but it has two consequences for anyone editing this file:
 *
 *   * NOTHING HERE NEEDS `__attribute__((section(".rodata")))`. At -G8 every
 *     object of eight bytes or less goes to `.sdata`, which the linker script
 *     does not collect; 27 of the 98 objects below are in that class and would
 *     each need the attribute in the -G8 unit. At -G0 the small-data threshold
 *     is zero and the question does not arise. If anything here ever moves
 *     into csrc/ovl_main_80400000.c, it needs the attribute, spelled
 *     `.rodata` and not `.rdata` (docs/owning-data.md SS9.2). The 27 are
 *     D_8040009C, D_804000E4, D_804000EC, D_804000F4, D_804000F8, D_804000FC,
 *     D_80400100, D_80400104, D_80400108, D_8040010C, D_80400110, D_80400134,
 *     D_804004D0, D_80400544, D_80400588, D_80400590, D_80400598, D_804005D4,
 *     D_804005EC, D_804005F0, D_80400660, D_804006BC, D_804006C0, D_80400714,
 *     D_8040080C, D_80400814 and D_80400834.
 *   * NO FUNCTION BELONGS IN THIS FILE. A function here would be built with
 *     the wrong compiler at the wrong -G and would not match.
 *
 * A RODATA OBJECT MUST BE `const`, or cc1psx files it under `.data`, which in
 * this segment is 0x8000 further on; the symptom is a rodata function-pointer
 * table differing nowhere near the object. csrc/main_twin.h's declarations
 * gained the keyword to match. The sizes are written out because cc1psx pads
 * a `[]` array only up to its own alignment.
 *
 * THE ASSERTION FORMAT IS HERE EIGHT TIMES, AND THAT IS NOT A MISTAKE. The
 * original was nine translation units -- c_server.c, card_mes.c, card_opt.c,
 * card_win.c, cardbk.c, cardinfo.c, cardslot.c, cardsme.c, memory.c -- each
 * carrying its own copy of the format string followed by its own filename.
 * c_server.c's pair is in the function unit, above jtbl_8040007C; the other
 * eight are below, and they are
 * what marks the unit boundaries in this region. gcc merges identical string
 * literals globally, so a literal would be emitted once: the only way to get
 * eight copies out of one C file is eight NAMED objects. docs/overlays.md
 * wants the twin split along those nine boundaries one day, and this is
 * evidence for it rather than something to fix here.
 *
 * SHIFT-JIS CANNOT BE IN THE SOURCE. cc1psx has no Shift-JIS input encoding
 * and `azure shiftjis` never runs on what it emits, so the bytes are `\x`
 * escapes with the decoded text in the comment above, as csrc/main/memory.c
 * does. The escapes and the comments were generated from the ROM together.
 *
 * SPLAT'S 47 LABELS ARE 98 OBJECTS. splat sizes a rodata object from the next
 * REFERENCED symbol, and nothing in this segment refers to these interiors
 * symbolically -- the tables that point at them are plain address words in
 * the twin's own `.data`, which splat did not relocate. Those tables are the
 * oracle: D_80408AA0 names the two 8-byte card prefixes inside D_804000D0's
 * label, D_80408AB8 the six filename suffixes inside D_804000F8's,
 * D_80408B2C[20][4] all 31 message strings inside D_80400138's 1036 bytes,
 * D_80408C8C the four menu rows inside D_80400570's, D_80408CB0 two inside
 * D_804005C8's, D_80408CF4/D_80408CF8 two inside D_80400668's, and D_804090C8
 * the six slot-state names inside D_80400800's. Two labels run the other way,
 * D_804006C2 and D_80400718, each the tail of the object above it: a
 * file-scope object cannot start off the 4-byte grid, since gcc emits
 * `.align 2` before every one, and both are a byte or halfword read inside
 * something larger. They are absorbed into D_804006C0 and D_80400714, nothing
 * refers to either name, and splat/main.bin_symbol_addrs.txt now gives
 * D_80400714 a size so the still-parked func_804053A0's listing relocates
 * against D_80400714+4 instead of a name nothing defines -- the landing-time
 * chore docs/near-misses/func_804053A0.c asks for.
 *
 * SEVEN OBJECTS END IN BYTES THAT ARE NOT ZERO PADDING: D_80400138,
 * D_80400620, D_80400654, D_8040077C, D_804007CC, D_80400800 and D_80400894
 * each hold a string and then something else in the bytes up to the next
 * object -- 02, AE, A2, 0C, 3C, 03 and " B0". It is the same off-cut
 * docs/owning-data.md SS4.8 describes at a section boundary, ASPSX padding
 * with whatever was in its buffer, except that it happens at OBJECT
 * boundaries inside the section too. Each is part of the object above it and
 * is written into the initialiser. The last of them, " B0" at 0x26989D, is
 * SS4.8's own main.bin entry and needs no asm escape hatch: that span is
 * 4-aligned and twelve bytes, so it is one named object like the rest.
 *
 * DEFINITION ORDER IS ADDRESS ORDER, and the group comments mark which
 * translation unit's assert pair opens each run. */

/* D_8040076C's four entries, defined in csrc/ovl_main_80400000.c. */
extern void func_80405D54(s32 *p);
extern void func_80406150(s32 *p);
extern void func_80406274(s32 *p);

/* --- c_server.c's assert pair is in the function unit, ABOVE jtbl_8040007C,
   so everything from here to D_80400548 follows the table. --- */
/* 0x80400090  print_card_status: func_804018FC prints six lines the live block does not */
const char D_80400090[12] = "CHAN %x\n";

/* 0x8040009C */
const char D_8040009C[8] = "ST %d\n";

/* 0x804000A4 */
const char D_804000A4[12] = "EVFLG %ld\n";

/* 0x804000B0 */
const char D_804000B0[12] = "HW_HD %d\n";

/* 0x804000BC */
const char D_804000BC[20] = "CARD STATUS0 %x\n";

/* 0x804000D0 */
const char D_804000D0[20] = "CARD STATUS1 %x\n";

/* 0x804000E4  MEM_CARD1; D_80408AA0[1] points here */
const char D_804000E4[8] = "bu10:";

/* 0x804000EC  MEM_CARD0; D_80408AA0[0] points here */
const char D_804000EC[8] = "bu00:";

/* 0x804000F4  the filename suffix func_804019B4 strcats */
const char D_804000F4[4] = "*";

/* 0x804000F8  the retry counter func_80401B30 and func_804022C8 print by name */
const char D_804000F8[4] = "cnt";

/* 0x804000FC  MEM_CARD_FILENAMES, D_80408AB8[5]..[0] */
const char D_804000FC[4] = "TMP";

/* 0x80400100 */
const char D_80400100[4] = "04";

/* 0x80400104 */
const char D_80400104[4] = "03";

/* 0x80400108 */
const char D_80400108[4] = "02";

/* 0x8040010C */
const char D_8040010C[4] = "01";

/* 0x80400110 */
const char D_80400110[4] = "00";

/* 0x80400114  func_80401F98 and func_80401FEC */
const char D_80400114[12] = "%s = %x\n";

/* 0x80400120 */
const char D_80400120[20] = "(int)gsw->check_sum";

/* 0x80400134 */
const char D_80400134[4] = "c";

/* 0x80400138  func_80402154's complaint, live read_save_file's -- 13 bytes of
   string, then 00 02 00, which is not zero padding; see the block header */
const char D_80400138[16] = "illegal data\0\0\x02";

/* --- THE MESSAGE STRINGS, 0x80400148..0x80400544. D_80408B2C[20][4] in this
   unit's .data points at them -- the four rows of each message record --
   and it spells the pointers as plain addresses because this chunk was
   not owned when it landed. --- */
/* 0x80400148 "データの消去に失敗しました。" */
const u8 D_80400148[32] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x8F\xC1\x8B\x8E"
    "\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400168 "        はい          いいえ        " */
const u8 D_80400168[40] =
    "\x20\x20\x20\x20\x20\x20\x20\x20\x82\xCD\x82\xA2"
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x82\xA2"
    "\x82\xA2\x82\xA6\x20\x20\x20\x20\x20\x20\x20\x20";

/* 0x80400190 "データを消去します、よろしいですか？" */
const u8 D_80400190[40] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x8F\xC1\x8B\x8E"
    "\x82\xB5\x82\xDC\x82\xB7\x81\x41\x82\xE6\x82\xEB"
    "\x82\xB5\x82\xA2\x82\xC5\x82\xB7\x82\xA9\x81\x48";

/* 0x804001B8 "データを消去してからコピーしてください。" */
const u8 D_804001B8[44] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x8F\xC1\x8B\x8E"
    "\x82\xB5\x82\xC4\x82\xA9\x82\xE7\x83\x52\x83\x73"
    "\x81\x5B\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x804001E4 "上書きコピーは出来ません。" */
const u8 D_804001E4[28] =
    "\x8F\xE3\x8F\x91\x82\xAB\x83\x52\x83\x73\x81\x5B"
    "\x82\xCD\x8F\x6F\x97\x88\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x80400200 "コピーに失敗しました。" */
const u8 D_80400200[24] =
    "\x83\x52\x83\x73\x81\x5B\x82\xC9\x8E\xB8\x94\x73"
    "\x82\xB5\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x80400218 "データをコピー中です。" */
const u8 D_80400218[24] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x83\x52\x83\x73"
    "\x81\x5B\x92\x86\x82\xC5\x82\xB7\x81\x42";

/* 0x80400230 "メモリカードをロード中です。" */
const u8 D_80400230[32] =
    "\x83\x81\x83\x82\x83\x8A\x83\x4A\x81\x5B\x83\x68"
    "\x82\xF0\x83\x8D\x81\x5B\x83\x68\x92\x86\x82\xC5"
    "\x82\xB7\x81\x42";

/* 0x80400250 "フォーマットが終了しました。" */
const u8 D_80400250[32] =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xAA\x8F\x49\x97\xB9\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400270 "フォーマットに失敗しました。" */
const u8 D_80400270[32] =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400290 "データの読み込みに失敗しました。" */
const u8 D_80400290[36] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x93\xC7\x82\xDD"
    "\x8D\x9E\x82\xDD\x82\xC9\x8E\xB8\x94\x73\x82\xB5"
    "\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x804002B4 "別なメモリーカードをセットするか、" */
const u8 D_804002B4[36] =
    "\x95\xCA\x82\xC8\x83\x81\x83\x82\x83\x8A\x81\x5B"
    "\x83\x4A\x81\x5B\x83\x68\x82\xF0\x83\x5A\x83\x62"
    "\x83\x67\x82\xB7\x82\xE9\x82\xA9\x81\x41";

/* 0x804002D8 "空きブロックが足りません。" */
const u8 D_804002D8[28] =
    "\x8B\xF3\x82\xAB\x83\x75\x83\x8D\x83\x62\x83\x4E"
    "\x82\xAA\x91\xAB\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x804002F4 "消してください。" */
const u8 D_804002F4[20] =
    "\x8F\xC1\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x80400308 "メモリーカード管理画面で不要なデータを" */
const u8 D_80400308[40] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5"
    "\x95\x73\x97\x76\x82\xC8\x83\x66\x81\x5B\x83\x5E"
    "\x82\xF0";

/* 0x80400330 "行うか、別なメモリーカードをセットするか、" */
const u8 D_80400330[44] =
    "\x8D\x73\x82\xA4\x82\xA9\x81\x41\x95\xCA\x82\xC8"
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xF0\x83\x5A\x83\x62\x83\x67\x82\xB7"
    "\x82\xE9\x82\xA9\x81\x41";

/* 0x8040035C "空きブロックが足りません。上書きセーブを" */
const u8 D_8040035C[44] =
    "\x8B\xF3\x82\xAB\x83\x75\x83\x8D\x83\x62\x83\x4E"
    "\x82\xAA\x91\xAB\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42\x8F\xE3\x8F\x91\x82\xAB\x83\x5A\x81\x5B"
    "\x83\x75\x82\xF0";

/* 0x80400388 "データの書き込みに失敗しました。" */
const u8 D_80400388[36] =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x8F\x91\x82\xAB"
    "\x8D\x9E\x82\xDD\x82\xC9\x8E\xB8\x94\x73\x82\xB5"
    "\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x804003AC "セーブデータがありません。" */
const u8 D_804003AC[28] =
    "\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E"
    "\x82\xAA\x82\xA0\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x804003C8 "メモリカードにセーブ中です。" */
const u8 D_804003C8[32] =
    "\x83\x81\x83\x82\x83\x8A\x83\x4A\x81\x5B\x83\x68"
    "\x82\xC9\x83\x5A\x81\x5B\x83\x75\x92\x86\x82\xC5"
    "\x82\xB7\x81\x42";

/* 0x804003E8 "     はい      いいえ     " */
const u8 D_804003E8[28] =
    "\x20\x20\x20\x20\x20\x82\xCD\x82\xA2\x20\x20\x20"
    "\x20\x20\x20\x82\xA2\x82\xA2\x82\xA6\x20\x20\x20"
    "\x20\x20";

/* 0x80400404 "上書きしてよろしいですか？" */
const u8 D_80400404[28] =
    "\x8F\xE3\x8F\x91\x82\xAB\x82\xB5\x82\xC4\x82\xE6"
    "\x82\xEB\x82\xB5\x82\xA2\x82\xC5\x82\xB7\x82\xA9"
    "\x81\x48";

/* 0x80400420 "メモリーカードをフォーマット中です。" */
const u8 D_80400420[40] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xF0\x83\x74\x83\x48\x81\x5B\x83\x7D"
    "\x83\x62\x83\x67\x92\x86\x82\xC5\x82\xB7\x81\x42";

/* 0x80400448 "別のメモリーカードをセットしてください。" */
const u8 D_80400448[44] =
    "\x95\xCA\x82\xCC\x83\x81\x83\x82\x83\x8A\x81\x5B"
    "\x83\x4A\x81\x5B\x83\x68\x82\xF0\x83\x5A\x83\x62"
    "\x83\x67\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x80400474 "メモリーカードが壊れています。" */
const u8 D_80400474[32] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x89\xF3\x82\xEA\x82\xC4\x82\xA2"
    "\x82\xDC\x82\xB7\x81\x42";

/* 0x80400494 "しばらくお待ちください。" */
const u8 D_80400494[28] =
    "\x82\xB5\x82\xCE\x82\xE7\x82\xAD\x82\xA8\x91\xD2"
    "\x82\xBF\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81\x42";

/* 0x804004B0 "メモリーカードがありません。" */
const u8 D_804004B0[32] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x82\xA0\x82\xE8\x82\xDC\x82\xB9"
    "\x82\xF1\x81\x42";

/* 0x804004D0  the empty row, D_80408B2C's filler */
const char D_804004D0[4] = "";

/* 0x804004D4 "            はい        いいえ" */
const u8 D_804004D4[32] =
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
    "\x82\xCD\x82\xA2\x20\x20\x20\x20\x20\x20\x20\x20"
    "\x82\xA2\x82\xA2\x82\xA6";

/* 0x804004F4 "フォーマットしてよろしいですか？" */
const u8 D_804004F4[36] =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xB5\x82\xC4\x82\xE6\x82\xEB\x82\xB5\x82\xA2"
    "\x82\xC5\x82\xB7\x82\xA9\x81\x48";

/* 0x80400518 "メモリーカードがフォーマットされていません" */
const u8 D_80400518[44] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x83\x74\x83\x48\x81\x5B\x83\x7D"
    "\x83\x62\x83\x67\x82\xB3\x82\xEA\x82\xC4\x82\xA2"
    "\x82\xDC\x82\xB9\x82\xF1";

/* 0x80400544  the live const u8 D_800200A0[4]; func_80402A1C and func_80402CD4 copy it as one UnkFour */
const u8 D_80400544[4] = {0, 1, 2, 3};

/* --- card_mes.c's assert pair. --- */
/* 0x80400548 */
const char D_80400548[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400570 */
const char D_80400570[12] = "card_mes.c";

/* 0x8040057C "リプレイ" -- the four menu rows, D_80408C8C[3]..[0] */
const u8 D_8040057C[12] = "\x83\x8A\x83\x76\x83\x8C\x83\x43";

/* 0x80400588 "消す" */
const u8 D_80400588[8] = "\x8F\xC1\x82\xB7";

/* 0x80400590 "コピー" */
const u8 D_80400590[8] = "\x83\x52\x83\x73\x81\x5B";

/* 0x80400598 "ロード" */
const u8 D_80400598[8] = "\x83\x8D\x81\x5B\x83\x68";

/* --- card_opt.c's assert pair. --- */
/* 0x804005A0 */
const char D_804005A0[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x804005C8 */
const char D_804005C8[12] = "card_opt.c";

/* 0x804005D4  D_80408CB0[1] */
const char D_804005D4[4] = "";

/* 0x804005D8 "使用できません。" -- D_80408CB0[0] */
const u8 D_804005D8[20] =
    "\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xDC\x82\xB9"
    "\x82\xF1\x81\x42";

/* 0x804005EC "回" */
const u8 D_804005EC[4] = "\x89\xF1";

/* 0x804005F0 "冒険中" */
const u8 D_804005F0[8] = "\x96\x60\x8C\xAF\x92\x86";

/* --- card_win.c's assert pair, and nothing else in this unit's rodata. --- */
/* 0x804005F8 */
const char D_804005F8[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400620 */
const char D_80400620[12] = "card_win.c\0\xAE";

/* --- cardbk.c's assert pair. --- */
/* 0x8040062C */
const char D_8040062C[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400654 */
const char D_80400654[12] = "cardbk.c\0\0\0\xA2";

/* 0x80400660 " 名前 " */
const char D_80400660[8] = "\x20\x96\xBC\x91\x4F\x20";

/* 0x80400668  the save-slot line template func_80404EA0 memcpy's,
   0x2A bytes of it; u32 because the copy is 4-aligned */
const u32 D_80400668[11] = {
    0x99923909, 0x960AE08B, 0x20AF8C60, 0xF1893509, 0x8A9E8E20, 0x202020D4,
    0x46812020, 0x46812020, 0x0935090A, 0x959A8C39, 0x000000A8};

/* 0x80400694 "不思議な塔" -- D_80408CF8 points here */
const u8 D_80400694[12] = "\x95\x73\x8E\x76\x8B\x63\x82\xC8\x93\x83";

/* 0x804006A0 "魔物の塔" -- D_80408CF4 points here */
const u8 D_804006A0[12] = "\x96\x82\x95\xA8\x82\xCC\x93\x83";

/* 0x804006AC "Ｆから冒険再開" */
const char D_804006AC[16] =
    "\x82\x65\x82\xA9\x82\xE7\x96\x60\x8C\xAF\x8D\xC4"
    "\x8A\x4A";

/* 0x804006BC */
const char D_804006BC[4] = " ";

/* 0x804006C0 "10", three bytes and a pad; func_80404EA0 memcpy's 3 of them.
   u16 because that is what main_twin.h declares, and D_804006C2 -- splat's
   label on the `lb` half of that copy -- is the second halfword here. */
const u16 D_804006C0[2] = {0x3031, 0x0000};

/* 0x804006C4 "メモリーカードに他のゲームのデータが" */
const u8 D_804006C4[40] =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xC9\x91\xBC\x82\xCC\x83\x51\x81\x5B"
    "\x83\x80\x82\xCC\x83\x66\x81\x5B\x83\x5E\x82\xAA";

/* 0x804006EC "あります。これ以上セーブできません。" */
const u8 D_804006EC[40] =
    "\x82\xA0\x82\xE8\x82\xDC\x82\xB7\x81\x42\x82\xB1"
    "\x82\xEA\x88\xC8\x8F\xE3\x83\x5A\x81\x5B\x83\x75"
    "\x82\xC5\x82\xAB\x82\xDC\x82\xB9\x82\xF1\x81\x42";

/* 0x80400714 "あと" -- 82 A0 82 C6 00 and three bytes of pad, ONE object, the
   free-block line's prefix. s32 because func_804053A0's five-byte block move
   needs it 4-aligned; see csrc/main_twin.h and the park. D_80400718 is this
   object's tail, not a symbol. */
const s32 D_80400714[2] = {(s32)0xC682A082, 0};

/* 0x8040071C "つデータがセーブできます。" */
const u8 D_8040071C[28] =
    "\x82\xC2\x83\x66\x81\x5B\x83\x5E\x82\xAA\x83\x5A"
    "\x81\x5B\x83\x75\x82\xC5\x82\xAB\x82\xDC\x82\xB7"
    "\x81\x42";

/* --- cardinfo.c's assert pair. --- */
/* 0x80400738 */
const char D_80400738[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400760 */
const char D_80400760[12] = "cardinfo.c";

/* 0x8040076C */
const UnkTwinDispatch4 D_8040076C = {{func_80405D54, func_80406150, func_80406274, func_80405D54}};

/* 0x8040077C  the card-info header template func_80406844 memcpy's,
   0x27 bytes of it; u16 because the copy is 2-aligned */
const u16 D_8040077C[20] = {
    0x4183, 0x5583, 0x5B81, 0x8983, 0x4383, 0x7483, 0x4581, 0x4183, 0x5583, 0x5B81,
    0x6883, 0x8A83, 0x5B81, 0x8083, 0x5883, 0x4081, 0x6D82, 0x6E82, 0x4481, 0x0C00};

/* --- cardslot.c's assert pair, and nothing else. --- */
/* 0x804007A4 */
const char D_804007A4[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x804007CC */
const char D_804007CC[12] = "cardslot.c\0<";

/* --- cardsme.c's assert pair. --- */
/* 0x804007D8 */
const char D_804007D8[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400800 */
const char D_80400800[12] = "cardsme.c\0\x03";

/* 0x8040080C  the six slot-state names, D_804090C8[5]..[0] and D_804090F8's copy */
const char D_8040080C[8] = "ERROR";

/* 0x80400814 */
const char D_80400814[8] = "NOTHING";

/* 0x8040081C */
const char D_8040081C[12] = "UN FORMAT";

/* 0x80400828 */
const char D_80400828[12] = "NEW READY";

/* 0x80400834 */
const char D_80400834[8] = "READY";

/* 0x8040083C */
const char D_8040083C[12] = "NOW CHECK";

/* 0x80400848 */
const char D_80400848[12] = "CARD%d %s\n";

/* 0x80400854 */
const char D_80400854[12] = "CARD1 %s\n";

/* 0x80400860 */
const char D_80400860[12] = "CARD2 %s\n";

/* --- memory.c's assert pair, and the section ends inside the filename's
   object: see the block header. --- */
/* 0x8040086C */
const char D_8040086C[40] = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400894 */
const char D_80400894[12] = "memory.c\0 B0";
