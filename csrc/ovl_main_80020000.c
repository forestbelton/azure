#include "common.h"

/* main.bin's overlay at 0x80020000: the memory card save/load subsystem.
   Built at cc1psx -G0, not the -G8 the resident executable uses; the Makefile
   has the per-unit override and says why.

   This file is still one splat unit, but the original source was NINE:
   c_server.c, card_mes.c, card_opt.c, card_win.c, cardbk.c, cardinfo.c,
   cardslot.c, cardsme.c and memory.c, in that link order. The names and the
   boundaries come from the debug twin at 0x80400000, which is the same source
   built at -G8 with more of its asserts compiled in; `tools/assertmap.py`
   recovers the map and docs/overlays.md records it. Carve this file along
   those boundaries before matching anything whose shape depends on which unit
   it is in. */

/* 0x80069C28 is libcard's `_card_wait`, resident in the engine. splat names
   it positionally here because main.bin has no symbol_addrs file yet. */
void func_80069C28(s32 chan);

void func_800213C4(s32 arg0);
void func_800214A4(s32 arg0);

extern s32 D_800287CC;

/* c_server.c, somewhere in lines 1-180: the first function in the overlay and
   the first one landed. Its twin is func_804008A0.

   The pointer local is load-bearing and is the lever at
   docs/matching-levers.md:3173, spent at the two stores and NOT at the read.
   Storing straight to the global emits each `%hi` beside its own store, so the
   else branch gets a second `lui` and the function runs one word long (25).
   Through a local assigned after the three calls, the `%hi` is emitted at the
   assignment -- before the branch, so both stores share $v1, and reorg pulls
   it into the beqz delay slot. The trailing read is left as the bare symbol so
   it keeps its own re-materialised `lui $v0`, which is what the listing has.
   The local costs no instruction: combine folds the `%lo` back into the
   stores. */
void func_80020924(s32 arg0)
{
    s32 *p;

    func_800214A4(arg0);
    func_80069C28(0);
    func_80069C28(1);
    p = &D_800287CC;
    if (arg0)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    func_800213C4(D_800287CC);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020984);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800209C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A58);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A6C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A80);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020F18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800210E0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800210F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002121C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021298);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002137C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800213A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800213C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021410);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800214A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800214FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021538);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021594);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020000);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020028);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020034);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020048);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002168C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800217D4);

void func_80021904(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002190C);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020074);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002191C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800219B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021A04);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021A84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021B18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021B98);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021C4C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021D38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021D54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021DF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021EB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021EE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021F18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800220DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022138);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022160);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002219C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002223C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002225C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022300);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002233C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800223B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022488);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022524);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800226A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800226E0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022774);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002290C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022934);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022A30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022B20);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022B48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022BE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022C48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022C90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022CD8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022D60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022E74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022E9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022EC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022EEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022F14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022F3C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022F64);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022F8C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022FB4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022FDC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023004);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002302C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023054);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002307C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800230A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800230CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800230F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002311C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023144);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002316C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023194);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800231E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023258);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800233D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023470);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002352C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800235A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023600);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023730);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800237C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800237EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002388C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023894);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800238D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800239B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023A00);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023A50);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023A78);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023B18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023BE4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023C44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023D64);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023E14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023EC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023ECC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023EF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023F94);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023FA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800240EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024160);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024184);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800241D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024224);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024274);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024298);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800242D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024390);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024818);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800249B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024A50);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024ACC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024BB4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024C48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024C70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024D10);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024D18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024D58);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024E44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024EEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024F3C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024F7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024FAC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025030);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800250E8);

void func_8002519C(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800251A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025230);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800252A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800252CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025358);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025398);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002546C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025494);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800254E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025584);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002570C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025788);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800258C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002592C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002593C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B40);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025BD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025D34);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025E54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025ECC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026128);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800261C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800261F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026240);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026270);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800262BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026398);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800263C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026410);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002644C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026494);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800264D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002651C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800265B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026720);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026728);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026748);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026784);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800267C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026868);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026920);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800269B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026B18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026C00);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026C94);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026D78);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026E90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026FB4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026FD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026FF8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027018);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027054);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027080);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800270E0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800271C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027254);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002727C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002731C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027324);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027364);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002742C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027458);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800274A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800274D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027528);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027584);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002765C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027684);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800276EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027734);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027778);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800277BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002789C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027A68);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027AFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027BC0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027C60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027C90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D40);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027DD0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027DF4);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200B8);
