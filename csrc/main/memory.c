#include "common.h"
#include "main.h"
#include "strings.h"

/* memory.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 228..240. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine". */

/* The fourth input handler. Its tail is the interesting part: it calls
   func_800217D4 for the &D_800287D8 pair that function returns, records
   whether each word changed, then copies the pair into p[6]/p[7]. That copy
   has to be an 8-byte STRUCT assignment -- gcc's aligned movstrsi, lw/lw
   then sw/sw -- because as two field assignments it comes out interleaved
   and one word longer. The 0x40 case returns outright rather than falling
   into the shared tail. */
void func_8002789C(s32 *p)
{
    s32 flag = 0;
    s32 *t;
    s32 diff[2];

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80027C90(p - 8);
            func_80027BF4();
            if (D_80082E60.unk_0B == 3)
            {
                func_80040AA0(3);
            }
            else
            {
                func_80044144(0, 0, 0, 0);
                ((u16 *)p[5])[15] |= 0x2000;
                if (p[0] == 2)
                {
                    func_800A68F4();
                }
            }
        }
        else if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            set_card_slot(p[3]);
            p[13] = 0;
            p[-4] = (s32)func_800277BC;
            return;
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                if (p[3] != 0)
                {
                    flag = 1;
                }
                p[3] = 0;
            }
            else
            {
                if (p[3] == 0)
                {
                    flag = 1;
                }
                p[3] = 1;
            }
        }
        if (flag != 0)
        {
            func_80053DA8(0x502);
            func_80026FD4((s32 *)p[9], p[3]);
        }
    }
    t = func_800217D4();
    diff[0] = t[0] != p[6];
    diff[1] = t[1] != p[7];
    *(UnkTwoWords *)&p[6] = *(UnkTwoWords *)t;
    func_80026FF8((s32 *)p[9], t, diff);
    print_card_status();
}

void func_80027A68(s32 *p, s32 a, s32 b)
{
    p[5] = a;
    p[0] = b;
    p[3] = is_second_card_slot();
}

void func_80027A9C(s32 *p)
{
    s32 *q = p - 8;

    p[8] = func_8004F418(q, p + 3);
    p[9] = (s32)func_80026ED0(q, p[3]);
    switch_card_slot();
    p[-4] = (s32)func_8002789C;
}

s32 *func_80027AFC(s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002BB08;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002BB08, 0x19);
    }
    else
    {
        func_80027C90(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x44);
    }
    D_80082E60.unk_0F |= 0x80;
    func_80027A68(r, a, b);
    q[4] = (s32)func_80027A9C;
    return q;
}

void func_80027BC0(void)
{
    EnterCriticalSection();
    StartRCnt(0xF2000002);
    ExitCriticalSection();
}

void func_80027BF4(void)
{
    D_80080A84 = 2;
    close_card_events();
    func_800585A0();
    if (D_80082E6A == 1)
    {
        func_80053DA8(0x200);
    }
    else
    {
        D_80080A88 = 0;
    }
    D_80082E60.unk_0F &= 0x7F;
}

void func_80027C60(void)
{
    func_8005863C();
    D_80080A84 = 1;
    start_card_server();
}

void func_80027C90(s32 *p)
{
    s32 *q = p + 8;

    func_8004F52C(q[8]);
    func_80026FB4((u16 *)q[9]);
    if (p[8] == 2)
    {
        func_800A6104();
    }
    ((u16 *)p)[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80027D0C(s32 *p)
{
    func_80027C60();
    func_80027AFC((s32)p, 1);
}

void func_80027D40(s32 *p)
{
    func_800A68C0();
    func_80027C60();
    func_80027AFC((s32)p, 2);
}

void func_80027D7C(s32 *p)
{
    func_80027C60();
    D_80083200 = 0x200;
    SetGeomScreen(0x200);
    SetGeomOffset(0xA0, 0x78);
    func_80027AFC((s32)p, 0);
}

s16 func_80027DD0(void)
{
    return func_8004CAA0();
}

void func_80027DF4(s32 *p, s32 v)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[3] = v;
        func_80026FD4((s32 *)q[9], v);
    }
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S .data AND .rodata, moved here from csrc/ovl_main_80020000.c on
   2026-11-10: the 59 Shift-JIS UI messages and the eight tables that point at
   them, 0x800280B4..0x80028530, plus the sixteen animation payloads above
   them.

   THIS IS THE OVERLAY'S ONE INFERRED DATA BOUNDARY, and the alternative is
   cardsme.c. What is proved: the run below 0x800280B4 partitions over units
   one to eight with zero order violations, so this block is cardsme.c's or
   memory.c's; `gCardMessages` and `D_800282A4` share `MSG_EMPTY`, of which
   the pool holds one copy, so they and everything between them are ONE unit;
   and the pool is the segment's last .rodata, which fits either. What picks
   memory.c is parsimony -- the cut at 0x800280B4 is the only position that
   needs no second choice, since cardsme.c owning part would leave "how much"
   open. The counter-signal is that `D_800282CC` is read by cardsme.c and
   nothing here is read by memory.c at all; that is weak, because this overlay
   has two PROVEN cross-unit data uses already (c_server.c defines
   `D_80027E68`, read only by cardslot.c, and `SAVE_ICON_CLUTS`, read only by
   cardinfo.c).

   `make containers` CANNOT referee this: any partition respecting address
   order links identically. If better evidence turns up, moving the block to
   cardsme.c is mechanical and the build will not notice. docs/overlays.md,
   "Rodata is per unit; so is data", carries the same caveat.

   The .rodata is not written out here and does not need to be: cc1psx emits a
   string literal into the unit whose initialiser uses it, so the pool lands in
   memory.o's .rodata by itself, at 0x800200C0..0x80020924 -- memory.c is the
   last of the nine, and no other unit's rodata follows. The 59 addresses,
   including where the deduplicated empty string lands, are predicted by
   simulating gcc 2.7.2's deferred-constant flush over these eight tables; the
   message block below records that measurement.
   --------------------------------------------------------------------------- */

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
