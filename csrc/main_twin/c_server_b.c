#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_80400948(void);

/* THE 0x80400000 DEBUG TWIN -- c_server.c, part 2 of 2: everything above jtbl_8040007C.

   .rodata 0x80400090..0x80400548   .text from 0x804017EC

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main_twin/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */

/* 0x80400090  print_card_status: func_804018FC prints six lines the live block does not */
const char D_80400090[12] __attribute__((section(".rodata"))) = "CHAN %x\n";

/* 0x8040009C */
const char D_8040009C[8] __attribute__((section(".rodata"))) = "ST %d\n";

/* 0x804000A4 */
const char D_804000A4[12] __attribute__((section(".rodata"))) = "EVFLG %ld\n";

/* 0x804000B0 */
const char D_804000B0[12] __attribute__((section(".rodata"))) = "HW_HD %d\n";

/* 0x804000BC */
const char D_804000BC[20] __attribute__((section(".rodata"))) = "CARD STATUS0 %x\n";

/* 0x804000D0 */
const char D_804000D0[20] __attribute__((section(".rodata"))) = "CARD STATUS1 %x\n";

/* 0x804000E4  MEM_CARD1; D_80408AA0[1] points here */
const char D_804000E4[8] __attribute__((section(".rodata"))) = "bu10:";

/* 0x804000EC  MEM_CARD0; D_80408AA0[0] points here */
const char D_804000EC[8] __attribute__((section(".rodata"))) = "bu00:";

/* 0x804000F4  the filename suffix func_804019B4 strcats */
const char D_804000F4[4] __attribute__((section(".rodata"))) = "*";

/* 0x804000F8  the retry counter func_80401B30 and func_804022C8 print by name */
const char D_804000F8[4] __attribute__((section(".rodata"))) = "cnt";

/* 0x804000FC  MEM_CARD_FILENAMES, D_80408AB8[5]..[0] */
const char D_804000FC[4] __attribute__((section(".rodata"))) = "TMP";

/* 0x80400100 */
const char D_80400100[4] __attribute__((section(".rodata"))) = "04";

/* 0x80400104 */
const char D_80400104[4] __attribute__((section(".rodata"))) = "03";

/* 0x80400108 */
const char D_80400108[4] __attribute__((section(".rodata"))) = "02";

/* 0x8040010C */
const char D_8040010C[4] __attribute__((section(".rodata"))) = "01";

/* 0x80400110 */
const char D_80400110[4] __attribute__((section(".rodata"))) = "00";

/* 0x80400114  func_80401F98 and func_80401FEC */
const char D_80400114[12] __attribute__((section(".rodata"))) = "%s = %x\n";

/* 0x80400120 */
const char D_80400120[20] __attribute__((section(".rodata"))) = "(int)gsw->check_sum";

/* 0x80400134 */
const char D_80400134[4] __attribute__((section(".rodata"))) = "c";

/* 0x80400138  func_80402154's complaint, live read_save_file's -- 13 bytes of
   string, then 00 02 00, which is not zero padding; see the block header */
const char D_80400138[16] __attribute__((section(".rodata"))) = "illegal data\0\0\x02";

/* --- THE MESSAGE STRINGS, 0x80400148..0x80400544. D_80408B2C[20][4] in this
   unit's .data points at them -- the four rows of each message record --
   and it spells the pointers as plain addresses because this chunk was
   not owned when it landed. --- */
/* 0x80400148 "データの消去に失敗しました。" */
const u8 D_80400148[32] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x8F\xC1\x8B\x8E"
    "\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400168 "        はい          いいえ        " */
const u8 D_80400168[40] __attribute__((section(".rodata"))) =
    "\x20\x20\x20\x20\x20\x20\x20\x20\x82\xCD\x82\xA2"
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x82\xA2"
    "\x82\xA2\x82\xA6\x20\x20\x20\x20\x20\x20\x20\x20";

/* 0x80400190 "データを消去します、よろしいですか？" */
const u8 D_80400190[40] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x8F\xC1\x8B\x8E"
    "\x82\xB5\x82\xDC\x82\xB7\x81\x41\x82\xE6\x82\xEB"
    "\x82\xB5\x82\xA2\x82\xC5\x82\xB7\x82\xA9\x81\x48";

/* 0x804001B8 "データを消去してからコピーしてください。" */
const u8 D_804001B8[44] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x8F\xC1\x8B\x8E"
    "\x82\xB5\x82\xC4\x82\xA9\x82\xE7\x83\x52\x83\x73"
    "\x81\x5B\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x804001E4 "上書きコピーは出来ません。" */
const u8 D_804001E4[28] __attribute__((section(".rodata"))) =
    "\x8F\xE3\x8F\x91\x82\xAB\x83\x52\x83\x73\x81\x5B"
    "\x82\xCD\x8F\x6F\x97\x88\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x80400200 "コピーに失敗しました。" */
const u8 D_80400200[24] __attribute__((section(".rodata"))) =
    "\x83\x52\x83\x73\x81\x5B\x82\xC9\x8E\xB8\x94\x73"
    "\x82\xB5\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x80400218 "データをコピー中です。" */
const u8 D_80400218[24] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xF0\x83\x52\x83\x73"
    "\x81\x5B\x92\x86\x82\xC5\x82\xB7\x81\x42";

/* 0x80400230 "メモリカードをロード中です。" */
const u8 D_80400230[32] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x83\x4A\x81\x5B\x83\x68"
    "\x82\xF0\x83\x8D\x81\x5B\x83\x68\x92\x86\x82\xC5"
    "\x82\xB7\x81\x42";

/* 0x80400250 "フォーマットが終了しました。" */
const u8 D_80400250[32] __attribute__((section(".rodata"))) =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xAA\x8F\x49\x97\xB9\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400270 "フォーマットに失敗しました。" */
const u8 D_80400270[32] __attribute__((section(".rodata"))) =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xC9\x8E\xB8\x94\x73\x82\xB5\x82\xDC\x82\xB5"
    "\x82\xBD\x81\x42";

/* 0x80400290 "データの読み込みに失敗しました。" */
const u8 D_80400290[36] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x93\xC7\x82\xDD"
    "\x8D\x9E\x82\xDD\x82\xC9\x8E\xB8\x94\x73\x82\xB5"
    "\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x804002B4 "別なメモリーカードをセットするか、" */
const u8 D_804002B4[36] __attribute__((section(".rodata"))) =
    "\x95\xCA\x82\xC8\x83\x81\x83\x82\x83\x8A\x81\x5B"
    "\x83\x4A\x81\x5B\x83\x68\x82\xF0\x83\x5A\x83\x62"
    "\x83\x67\x82\xB7\x82\xE9\x82\xA9\x81\x41";

/* 0x804002D8 "空きブロックが足りません。" */
const u8 D_804002D8[28] __attribute__((section(".rodata"))) =
    "\x8B\xF3\x82\xAB\x83\x75\x83\x8D\x83\x62\x83\x4E"
    "\x82\xAA\x91\xAB\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x804002F4 "消してください。" */
const u8 D_804002F4[20] __attribute__((section(".rodata"))) =
    "\x8F\xC1\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x80400308 "メモリーカード管理画面で不要なデータを" */
const u8 D_80400308[40] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x8A\xC7\x97\x9D\x89\xE6\x96\xCA\x82\xC5"
    "\x95\x73\x97\x76\x82\xC8\x83\x66\x81\x5B\x83\x5E"
    "\x82\xF0";

/* 0x80400330 "行うか、別なメモリーカードをセットするか、" */
const u8 D_80400330[44] __attribute__((section(".rodata"))) =
    "\x8D\x73\x82\xA4\x82\xA9\x81\x41\x95\xCA\x82\xC8"
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xF0\x83\x5A\x83\x62\x83\x67\x82\xB7"
    "\x82\xE9\x82\xA9\x81\x41";

/* 0x8040035C "空きブロックが足りません。上書きセーブを" */
const u8 D_8040035C[44] __attribute__((section(".rodata"))) =
    "\x8B\xF3\x82\xAB\x83\x75\x83\x8D\x83\x62\x83\x4E"
    "\x82\xAA\x91\xAB\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42\x8F\xE3\x8F\x91\x82\xAB\x83\x5A\x81\x5B"
    "\x83\x75\x82\xF0";

/* 0x80400388 "データの書き込みに失敗しました。" */
const u8 D_80400388[36] __attribute__((section(".rodata"))) =
    "\x83\x66\x81\x5B\x83\x5E\x82\xCC\x8F\x91\x82\xAB"
    "\x8D\x9E\x82\xDD\x82\xC9\x8E\xB8\x94\x73\x82\xB5"
    "\x82\xDC\x82\xB5\x82\xBD\x81\x42";

/* 0x804003AC "セーブデータがありません。" */
const u8 D_804003AC[28] __attribute__((section(".rodata"))) =
    "\x83\x5A\x81\x5B\x83\x75\x83\x66\x81\x5B\x83\x5E"
    "\x82\xAA\x82\xA0\x82\xE8\x82\xDC\x82\xB9\x82\xF1"
    "\x81\x42";

/* 0x804003C8 "メモリカードにセーブ中です。" */
const u8 D_804003C8[32] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x83\x4A\x81\x5B\x83\x68"
    "\x82\xC9\x83\x5A\x81\x5B\x83\x75\x92\x86\x82\xC5"
    "\x82\xB7\x81\x42";

/* 0x804003E8 "     はい      いいえ     " */
const u8 D_804003E8[28] __attribute__((section(".rodata"))) =
    "\x20\x20\x20\x20\x20\x82\xCD\x82\xA2\x20\x20\x20"
    "\x20\x20\x20\x82\xA2\x82\xA2\x82\xA6\x20\x20\x20"
    "\x20\x20";

/* 0x80400404 "上書きしてよろしいですか？" */
const u8 D_80400404[28] __attribute__((section(".rodata"))) =
    "\x8F\xE3\x8F\x91\x82\xAB\x82\xB5\x82\xC4\x82\xE6"
    "\x82\xEB\x82\xB5\x82\xA2\x82\xC5\x82\xB7\x82\xA9"
    "\x81\x48";

/* 0x80400420 "メモリーカードをフォーマット中です。" */
const u8 D_80400420[40] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xF0\x83\x74\x83\x48\x81\x5B\x83\x7D"
    "\x83\x62\x83\x67\x92\x86\x82\xC5\x82\xB7\x81\x42";

/* 0x80400448 "別のメモリーカードをセットしてください。" */
const u8 D_80400448[44] __attribute__((section(".rodata"))) =
    "\x95\xCA\x82\xCC\x83\x81\x83\x82\x83\x8A\x81\x5B"
    "\x83\x4A\x81\x5B\x83\x68\x82\xF0\x83\x5A\x83\x62"
    "\x83\x67\x82\xB5\x82\xC4\x82\xAD\x82\xBE\x82\xB3"
    "\x82\xA2\x81\x42";

/* 0x80400474 "メモリーカードが壊れています。" */
const u8 D_80400474[32] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x89\xF3\x82\xEA\x82\xC4\x82\xA2"
    "\x82\xDC\x82\xB7\x81\x42";

/* 0x80400494 "しばらくお待ちください。" */
const u8 D_80400494[28] __attribute__((section(".rodata"))) =
    "\x82\xB5\x82\xCE\x82\xE7\x82\xAD\x82\xA8\x91\xD2"
    "\x82\xBF\x82\xAD\x82\xBE\x82\xB3\x82\xA2\x81\x42";

/* 0x804004B0 "メモリーカードがありません。" */
const u8 D_804004B0[32] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x82\xA0\x82\xE8\x82\xDC\x82\xB9"
    "\x82\xF1\x81\x42";

/* 0x804004D0  the empty row, D_80408B2C's filler */
const char D_804004D0[4] __attribute__((section(".rodata"))) = "";

/* 0x804004D4 "            はい        いいえ" */
const u8 D_804004D4[32] __attribute__((section(".rodata"))) =
    "\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20\x20"
    "\x82\xCD\x82\xA2\x20\x20\x20\x20\x20\x20\x20\x20"
    "\x82\xA2\x82\xA2\x82\xA6";

/* 0x804004F4 "フォーマットしてよろしいですか？" */
const u8 D_804004F4[36] __attribute__((section(".rodata"))) =
    "\x83\x74\x83\x48\x81\x5B\x83\x7D\x83\x62\x83\x67"
    "\x82\xB5\x82\xC4\x82\xE6\x82\xEB\x82\xB5\x82\xA2"
    "\x82\xC5\x82\xB7\x82\xA9\x81\x48";

/* 0x80400518 "メモリーカードがフォーマットされていません" */
const u8 D_80400518[44] __attribute__((section(".rodata"))) =
    "\x83\x81\x83\x82\x83\x8A\x81\x5B\x83\x4A\x81\x5B"
    "\x83\x68\x82\xAA\x83\x74\x83\x48\x81\x5B\x83\x7D"
    "\x83\x62\x83\x67\x82\xB3\x82\xEA\x82\xC4\x82\xA2"
    "\x82\xDC\x82\xB9\x82\xF1";

/* 0x80400544  the live const u8 D_800200A0[4]; func_80402A1C and func_80402CD4 copy it as one UnkFour */
const u8 D_80400544[4] __attribute__((section(".rodata"))) = {0, 1, 2, 3};


/* Live func_800217D4, with the same two revision differences as
 * func_804016D0 above: neither arm zeroes D_80409260/D_80409268 after
 * setting the state, and there is no counterpart to the live
 * `(&D_80084118)[i] = 0;`. */
s32 *func_804017EC(void)
{
    s32 v = func_80401444();
    s32 i;

    if (v != 0)
    {
        i = func_804019A4();
        if (D_80409268[i] == v)
        {
            D_80409260[i]++;
        }
        else
        {
            D_80409260[i] = 0;
        }
        D_80409268[i] = v;
        if (v == 1 || v == 4)
        {
            if (D_80409260[i] >= 4)
            {
                D_804094F8[i] = 1;
            }
        }
        else
        {
            if (D_80409260[i] >= 4)
            {
                D_804094F8[i] = 0;
            }
        }
        func_80400948();
    }
    return D_804094F8;
}

/* print_card_status, which is EMPTY in the live block and prints six lines
 * here. That is the clearest single piece of evidence that this block is a
 * debug build of the same source. */
void func_804018FC(void)
{
    func_8003FA78(D_80400090, D_804094EC);
    func_8003FA78(D_8040009C, D_804094E8);
    func_8003FA78(D_804000A4, D_80409500);
    func_8003FA78(D_804000B0, D_80409504);
    func_8003FA78(D_804000BC, func_80408644(0));
    func_8003FA78(D_804000D0, func_80408644(1));
}

int func_804019A4(void)
{
    return D_804094EC != 0;
}

/* NEAR MISS: one insertion: the conditional +4 is re-materialised as SYM+4 instead of added to the live register
   Source in docs/near-misses/func_804019B4.c. */
/* Live count_card_files. TWO RENAMES THE SYMMAP DID NOT HAVE --
 * MEM_CARD_NAMES -> D_80408AA0 and D_80020084 -> D_804000F4 -- AND THEN
 * ONE DECLARED SIZE. See csrc/main_twin.h on D_80408AA0: at [2] the array
 * is 8 bytes, takes the -G8 small-data path, and the conditional `+4` gets
 * folded into a fresh SYM+4 address where the target adds 4 to the live
 * register -- one inserted word and 28 positional differences behind it.
 * An incomplete declaration, or any size over 8, matches exactly.
 *
 * REJECTED on the way: a byte-cast increment, `+ 1`, and dropping the
 * `&...[0]` (all 40 words); and an index local, which gets the length
 * right but goes BRANCHLESS where the target branches. */
s32 func_804019B4(void)
{
    u8 name[0x20];
    s32 n = 0;
    struct DIRENTRY *e = D_80409290;
    const char **mem_card_name = &D_80408AA0[0];

    if (D_804094EC != 0)
    {
        mem_card_name++;
    }
    func_8007BF80(name, *mem_card_name);
    func_8007BF50(name, (u8 *)D_804000F4);
    if (func_8007CAB8(name, e) != NULL)
    {
        do
        {
            n++;
            e = (struct DIRENTRY *)((u8 *)e + 0x28);
        } while (func_8007CAC8(e) == e);
    }
    return n;
}

s32 func_80401A50(s32 n)
{
    s32(*p)[10] = (s32(*)[10])D_80409290;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < n; i++)
    {
        sum += p[i][6];
    }
    sum = sum / 0x2000;
    return 0xF - sum;
}

s32 func_80401AA4(s32 n, u8 *s)
{
    s32 i = 0;
    s32 ret = 0;
    u8 *tbl = (u8 *)D_80409290;
    u8 *row;
    /* DEVICE: 8 bytes of unread local. The target builds a 0x8 frame where
       this body is frameless, and every other word matches once it shifts --
       no extra calls, no extra registers saved, just the prologue, the
       epilogue adjust and the displacement. AND THIS ONE IS NOT THE COMPILER:
       func_804025F8 looked identical and turned out to be FSF 2.7.2 reserving
       8 bytes that the Cygnus snapshot does not, but BOTH sides here are FSF
       2.7.2, so the twin's source really does have a local this one lost. A
       scalar is eliminated; it has to be an aggregate, and 8 bytes of one.
       `s32 unused[1]` is 32 words, `u8 buf[8]` also matches. */
    s32 unused[2];

    if (n > 0)
    {
        row = tbl;
        do
        {
            s32 bad = 0;
            s32 j = 0;

            do
            {
                u8 c = s[j];

                if (c == 0)
                {
                    break;
                }
                if (c != row[j])
                {
                    bad++;
                    break;
                }
                j++;
            } while (j < 0x15);
            if (bad == 0)
            {
                ret = 1;
                break;
            }
            row += 0x28;
        } while (++i < n);
    }
    return ret;
}

/* A `cnt` site: the $s1 the VSync poll loop increments is printed as
   `printf("%s = %d\n", "cnt", cnt)` once the loop ends, so name it `cnt` when
   this lands. `cnt` is TWIN-ONLY -- the live format_card has no counter at
   all, because the print was its only use -- so there is nothing to rename in
   the live unit. Same story in func_804022C8. */
/* Live format_card, with a VSync COUNTER the live one does not keep and a
 * printf of it after the wait loop. */
s32 func_80401B30(s32 card_slot)
{
    const char **mem_card;
    s32 n = 0;
    CARD_STATE r;

    func_80408654(0);
    func_80408654(1);
    func_80401578();
    mem_card = D_80408AA0;
    if (card_slot != 0)
    {
        mem_card++;
    }
    func_8007CAA8(*mem_card);
    while ((r = func_8040153C()) == 0)
    {
        n++;
        func_800702F0(0);
    }
    func_8007C040(D_80400038, D_804000F8, n);
    if (r != 1)
    {
        if (r == 3)
        {
            r = 4;
        }
        else
        {
            r = 5;
        }
    }
    return r;
}

void func_80401BF4(char *out, s32 file_id)
{
    const char **mem_card_name = &D_80408AA0[0];

    if (D_804094EC != 0)
    {
        mem_card_name++;
    }
    func_8007BF80(out, *mem_card_name);
    func_8007BF50(out, D_80408AA8);
    func_8007BF50(out, D_80408AB8[file_id]);
}

/* Live read_card_file, and NOT its two devices. The shifted length is its
 * own local here -- assigning back into `n` as the live source does swaps
 * $s1 and $s2 between `n` and `off` -- and the result is a branch, not the
 * live `r = read(...) == n;`, which comes out branchless and a word short.
 * The live source's two nested degenerate loops around `return r` are wrong
 * here: they cost 17 differing. */
s32 func_80401C70(u8 *name, u8 *buf, s32 n, s32 off)
{
    s32 r = 0;
    s32 fd;

    fd = func_8007CA58(name, 1);
    if (fd != -1)
    {
        s32 len = n << 7;

        if (off == 0 || func_8007CA68(fd, off << 7, 0) != -1)
        {
            if (func_8007CA78(fd, buf, len) == len)
            {
                r = 1;
            }
        }
        func_8007CA98(fd);
    }
    return r;
}

/* Live write_card_file. SINGLE EXIT, not the live source's two early
 * `return ok;` -- with those, gcc proves `ok` is still 0 at each and
 * materialises the constant, where the target reads it back out of $s3. A
 * `goto out` to one `return ok;` makes both exits share the tail and it
 * matches exactly. (Writing `return 0;` at both sites is the same 3
 * differing, because the fold is what the source cannot reach.) */
s32 func_80401D28(u8 *name, u8 *buf, s32 n, s32 off, s32 skip)
{
    s32 ok = 0;
    s32 fd;

    if (skip == 0)
    {
        fd = func_8007CA58(name, 0x30200);
        if (fd == -1)
        {
            goto out;
        }
        func_8007CA98(fd);
    }
    fd = func_8007CA58(name, 2);
    if (fd == -1)
    {
        goto out;
    }
    if (off == 0 || func_8007CA68(fd, off << 7, 0) != -1)
    {
        if (func_8007CA88(fd, buf, n << 7) == (n << 7))
        {
            ok = 1;
        }
    }
    func_8007CA98(fd);
out:
    return ok;
}

s32 func_80401E14(s32 i)
{
    return D_8009DDD8[i][0];
}

/* Live load_save_slot_info, with the row pointer NOT kept in a local: every
 * one of the three D_8009DDD8 references re-materialises the base, which is
 * what the two assembler-expanded stores and the re-loaded $a1 say. */
/* Live load_save_slot_info. The row is addressed by a BYTE OFFSET, not a
 * pointer local: the target's two stores are `sw $v0, D_8009DDD8($s0)` with
 * $s0 holding idx << 7, and a `s32 *e = D_8009DDD8[idx];` local costs the add
 * and comes out at 40 words. */
s32 func_80401E2C(s32 idx)
{
    char name[0x20];
    s32 r = 1;
    s32 **t;
    s32 off;
    s32 v;
    s32 slot;

    func_80401BF4(name, idx);
    off = idx * 0x80;
    slot = D_804094EC;
    /* DEVICE: one marker, worth a word (43 -> 42, 34 differing -> 0). The
       block before the branch is three insns -- `la $4,D_80136140`,
       `lw $2,D_804094EC` and `sll $16,$16,7`. Written plainly gcc sinks the
       load to its comparison, so the `la` is first in LUID order, sched2 ties
       on priority and keeps that order, the load's delay has nothing in it
       and gcc emits its own `#nop`. The target is the same four insns with
       the first two swapped, the la's two assembler-expanded words covering
       the delay. Reading the flag into a local does not move it on its own --
       gcc sinks it again -- so the marker is what pins the load ahead of the
       `la`, and then the scheduler fills the delay by itself.
       Legal spellings ruled out: no marker 43 words, 34 differing; the marker
       AFTER the `t =` 43/34 (too late to change the order); `volatile` 43/34
       (it is a scheduling barrier, so the la cannot move into the delay
       either); `"=r"(slot)`/`"0"(slot)` also matches, and so does moving the
       `off` multiply after the marker.
       Warns under -Wall? No. */
    __asm__("" : : "r"(slot));
    t = (s32 **)D_80136140;
    if (slot != 0)
    {
        t++;
    }
    v = func_80401AA4((s32)*t, (u8 *)(name + 5));
    *(s32 *)((u8 *)D_8009DDD8 + off) = v;
    if (v != 0)
    {
        r = func_80401C70((u8 *)name, (u8 *)D_8009DDD8 + off, 1, 4);
        *(s32 *)((u8 *)D_8009DDD8 + off) = 1;
    }
    return r;
}

void func_80401ED4(s32 idx, u8 *src)
{
    s32 *row;

    /* THE LIVE SOURCE'S DEVICE IS THE OTHER WAY ROUND HERE, and the reason
       is the compiler. Live store_save_slot_info computes the row pointer
       for the SECOND access in its own statement, because as a MEM address
       4.1's memory_address forces the symbol into a register before the
       index shift (lui/addiu/sll) and only a pointer assignment gets the
       target's sll/lui/addiu order. Without split addresses there is a third
       option that build did not have: a MEM address FOLDS, and the second
       access comes out as `lui $at` / `addu $at,$at,$v1` /
       `sw %lo(SYM)($at)` -- three words, not four. So here it is the FIRST
       access that needs the pointer local and the second that must be
       written as the plain 2-D index. Swapping the two spellings takes it
       from 4 differing to 0. */
    row = D_8009DDD8[idx];
    *(UnkRow *)row = *(UnkRow *)(src + 0x200);
    D_8009DDD8[idx][0] = 1;
}

/* Prints `(int)gsw->check_sum` after storing it, so the parameter is `gsw` and
   the word at +0x204 is its `check_sum` member -- GSW_CHECK_SUM in csrc/main.h,
   now used by the live set_gsw_checksum/check_gsw_checksum. */
/* Live set_gsw_checksum, with FOUR more words in the run and a printf after. */
void func_80401F98(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 n = 0x1781;

    do
    {
        sum ^= *q++;
        n--;
    } while (n >= 0);
    gsw[0x81] = sum;
    func_8007C040(D_80400114, D_80400120);
}

/* Prints both halves of the comparison: `c` (the xor accumulator) and then
   `(int)gsw->check_sum`. The live check_gsw_checksum's accumulator is now `c`. */
/* Live check_gsw_checksum, with the run four words longer and both the
 * computed and the stored sum printed before the comparison. */
s32 func_80401FEC(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 i = 0;

    for (i = 0; i < 0x1782; i++)
    {
        sum ^= *q;
        q++;
    }
    func_8007C040(D_80400114, D_80400134, sum);
    func_8007C040(D_80400114, D_80400120, gsw[0x81]);
    return gsw[0x81] == sum;
}

/* Live write_save_file, without its `*(s32 *)(buf + 0x204) = 0;` and without
 * the refresh_card_info call. */
s32 func_80402084(s32 idx, u8 *buf)
{
    char nameA[0x20];
    char nameB[0x20];
    s32 r;

    func_80401BF4(nameA, idx);
    func_80401BF4(nameB, 5);
    if (D_8009DDD8[idx][0] != 0)
    {
        func_8007CAD8(nameA, nameB);
    }
    func_80401F98((s32 *)buf);
    r = func_80401D28((u8 *)nameB, buf, 0xC0, 0, D_8009DDD8[idx][0]);
    if (r != 0)
    {
        func_80401ED4(idx, buf);
    }
    func_8007CAD8(nameB, nameA);
    return r;
}

/* Live read_save_file. TWO arguments here, no `flag` arm at all, and a
 * RETURN: the live one returns nothing. The checksum failure prints. */
s32 func_80402154(s32 idx, u8 *buf)
{
    char name[0x20];
    s32 r;

    func_80401BF4(name, idx);
    r = func_80401C70((u8 *)name, buf, 0xC0, 0);
    if (r != 0)
    {
        r = func_80401FEC((s32 *)buf);
        if (r != 0)
        {
            r = func_80401D28((u8 *)name, (u8 *)D_8009DDD8[idx], 1, 4,
                              D_8009DDD8[idx][0]);
        }
        else
        {
            func_8007BF18(D_80400138);
        }
    }
    return r;
}

void func_80402214(void)
{
    s32 i = D_804094EC != 0;
    s32 n;

    n = func_804019B4();
    D_80136140[i] = n;
    D_8009EB58[i] = func_80401A50(n);
}

s32 func_80402268(void)
{
    s32 *p = D_8009EB58;

    if (D_804094EC != 0)
    {
        p++;
    }
    return *p;
}

/* Live count_used_save_slots. TWO SPELLING CHOICES, both forced by the
 * listing. The loop variable is a BYTE OFFSET stepping 0x80 against a limit
 * of 0x280, not an index with a `sll 7` -- the load is the folded
 * `lui $at` / `addu $at,$at,$v1` / `lw %lo(SYM)($at)`. And the value is read
 * into a local with the increment BEFORE the test, which is what frees the
 * `beqz`'s delay slot for that increment; with the increment after the `if`,
 * reorg finds `count++` in the fall-through first, cannot use it, and leaves
 * a nop. Worth one word. */
s32 func_80402290(void)
{
    s32 count = 0;
    s32 off = 0;

    do
    {
        s32 v = *(s32 *)((u8 *)D_8009DDD8 + off);

        off += 0x80;
        if (v != 0)
        {
            count++;
        }
    } while (off < 0x280);
    return count;
}

/* The other `cnt` site; see func_80401B30. The live erase_save_file has no
   counter either. */
/* Live erase_save_file, plus a VSync counter and its printf, plus a
 * free-block fixup the live one has no counterpart for. THE FIXUP IS ONE
 * SCHEDULING DECISION and three devices that reach it.
 *
 *   * The asm between the store and `q = w` is what lands the function. It
 *     gives the COPY a memory dependence on the store. Without it sched1
 *     puts the copy SECOND in the block, ahead of `addu $at,$at,$v0`, where
 *     it is live across the index pseudo of `D_8009DDD8[idx][0] = 0`;
 *     global.c records the conflict and `q` cannot have $v0, so `q` takes
 *     $a0, the flag takes $a1, and seven words differ by register name.
 *     NO ORDER OF THE SOURCE CAN DO THIS. The copy and the store are both
 *     priority 1 and both ready at the last slot of the block, and
 *     gcc 2.7.2's schedule_select breaks that tie on function-unit hazard
 *     (sched.c:2655), where a store always outranks a register move. Only a
 *     dependence moves it. See docs/matching-levers.md.
 *   * `tbl`, the live erase_save_file's own `s32 (*tbl)[32]`, makes the
 *     store's address opaque to the dependence analyser, so the `slot` load
 *     ABOVE it becomes an anti-dependence the store cannot be scheduled
 *     past. That is what keeps the flag read ahead of the store; with the
 *     flag read inline in the `if` the dependence runs the other way and
 *     the load lands in the branch's shadow for a nop.
 *   * The empty asm between the two conditionals keeps them apart: written
 *     as two plain `if`s gcc proves the second redundant and merges them. */
s32 func_804022C8(s32 idx)
{
    char name[0x20];
    s32 n = 0;
    s32 v;

    func_80401BF4(name, idx);
    func_80408654(0);
    func_80408654(1);
    func_80401578();
    func_8007CAE8(name);
    while ((v = func_8040153C()) == 0)
    {
        n++;
        func_800702F0(0);
    }
    func_8007C040(D_80400038, D_804000F8, n);
    if (idx < 5)
    {
        s32(*tbl)[32];
        s32 *w = D_8009EB58;
        s32 *q;
        s32 slot = D_804094EC;

        tbl = D_8009DDD8;
        tbl[idx][0] = 0;
        __asm__("" : "=r"(w) : "0"(w), "m"(D_8009DDD8[idx][0]));
        q = w;
        if (slot != 0)
        {
            w++;
        }
        __asm__("" : : "r"(w));
        if (slot != 0)
        {
            q++;
        }
        *w = *q + 3;
    }
    return v == 1;
}

void func_804023C4(void)
{
    func_804022C8(5);
}

/* The flag has to be read into a local and `p` decremented IN PLACE: the
 * target reuses $s0 for both and puts the `addiu $s0,$s0,-0x20` in the
 * beqz's delay slot. A separate `q = p - 8` costs the extra register and
 * leaves the slot empty. */
void func_804023E4(s32 *p)
{
    s32 v;

    func_80047938(p);
    v = ((u16 *)p)[-1];
    p -= 8;
    if (v & 0x2000)
    {
        func_804024A4(p);
    }
}

/* The FIRST argument is never read -- the allocator is called with a literal
 * zero -- and the constructor comes out of D_80408AD0 by index. */
s32 *func_80402428(s32 unused, s32 a, s32 n)
{
    s32 *p = func_8003C480(0);

    if (p != 0)
    {
        p[8] = a;
        p[9] = n;
        D_80408AD0[n](p);
        p[4] = (s32)func_804023E4;
    }
    return p;
}

void func_804024A4(s32 *p)
{
    if (p != 0)
    {
        *(s32 *)p[8] = func_800484A4(p[9] + 6, (s32 *)p[8]);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

/* The space-skipping width measurer; live func_8002225C, which csrc/main/card_mes.c
   carries verbatim apart from the glyph walk.

   TWO devices, and the second was only found by reading the live one. The
   space skip is a plain `while`, not a guarded `do`/`while`: that is what
   makes loop.c hoist a SECOND copy of the 0x20 it compares against, and the
   target holds that constant in three separate registers ($t4 for the outer
   tests, $v1 for the skip loop, $t1 for the glyph walk). Written as a guard
   plus `do`/`while` cse1 merges all three into one pseudo before loop ever
   runs, and every register in the function shifts.

   Then the glyph walk goes through a COPY of the pointer. Walking `s` itself
   is 42 words of the target's 44: cse1 propagates the `lbu` across the
   rotated loop's guard, so the preheader reload the target has never appears.
   Through `t` it survives, because the copy gives the loop its own pseudo and
   there is no earlier load of `*t` to propagate. The live function walks `s`
   directly, which is why this did not come out of the counterpart. */
