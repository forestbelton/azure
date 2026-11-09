#include "common.h"

/* main.bin's DEBUG TWIN of the 0x80020000 card overlay: the same source built
 * at -G8 with its asserts compiled in, linked against a different engine
 * build, and dead on retail (0x80400000 is 8 MB dev-board RAM). It is in the
 * container, so it still has to be reproduced. See docs/overlays.md.
 *
 * THE PLAN IS NOT A SECOND DECOMPILATION. The live overlay's own source
 * rebuilt at -G8 already reproduces 86 of this block's 263 functions
 * instruction-for-instruction, 231 of 263 pair 1:1 with a function it
 * defines, and the 32 that do not are engine code this build linked
 * statically (memory.c, the PSY-Q stubs) which is decompiled in the resident
 * units. tools/assertmap.py reads the missing asserts straight out of here.
 *
 * WHY THE SYMBOLS ARE SPELLED 0x8040xxxx RATHER THAN SHARED. Both blocks sit
 * in ONE container, so a name cannot resolve to two addresses, and the two
 * builds do not agree on any constant offset: card_slot is D_800287CC live
 * and D_804094EC here, a delta of 0x3E0D20, while the data bases differ by
 * 0x3E1320. The external calls are worse than renamed -- this block was
 * linked against a different engine build, where printf is at 0x8007C040,
 * which in the shipped executable is inside libgte's rcossin_tbl.
 */

/* func_804014E4 is reset_sw_events, func_80408654 is this build's own static
 * copy of _card_wait (the live block calls the resident one at 0x80069C28),
 * func_804013F8 is check_card_slot, and D_804094EC is card_slot. All four are
 * its counterparts under the alignment, not guesses: the bodies are
 * instruction-identical. */
void func_804014E4(void);
void func_80408654(s32 arg0);
void func_804013F8(s32 arg0);
extern s32 D_804094EC;

#define dbg_card_slot D_804094EC

/* The twin of func_80020924, and the pair that measures the -G difference:
 * 24 words at -G0 in the live block, 26 here at -G8, where a global access
 * comes out as the assembler-expanded `lui $at` + `%lo` pair instead of the
 * hoisted-lui split. Same body as the live one, including the pointer local,
 * which is the in-place flag update device. */
void func_804008A0(s32 use_second_slot)
{
    s32 *p;

    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    p = &dbg_card_slot;
    if (use_second_slot)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    func_804013F8(dbg_card_slot);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400908);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400948);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400998);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804009C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804009D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804009E8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804009FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400A10);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400E34);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400EF8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804010E0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804010F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040122C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804012B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804013A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804013D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804013F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401444);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804014E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040153C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401578);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804015D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804016D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804017EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804018FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804019A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804019B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401A50);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401AA4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401B30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401C70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401D28);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401E14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401ED4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401F98);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80401FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402084);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402154);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402214);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402268);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402290);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804022C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804023C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804023E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402428);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804024A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402508);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804025B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804025F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402670);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040274C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804027F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040293C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402974);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402A1C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402BB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402CD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402D90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402EBC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F04);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804030CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804030F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040311C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403144);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040316C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403194);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804031BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804031E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040320C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403234);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040325C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403284);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403324);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040334C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403374);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040339C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804033C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804033EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040343C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804034D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403698);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403738);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804037F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403868);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804038C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804039F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403AA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403AC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403B7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403B84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403CD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403E54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403F1C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403F7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804040C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404178);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404224);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040422C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404254);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404308);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404314);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404364);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804044D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040454C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404570);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804045C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404618);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404668);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404688);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804046C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404708);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804047B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404818);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404888);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804048F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404A3C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404B10);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404B5C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404B84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404C44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404C84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404DBC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404DEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804053A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804054E8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040553C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405624);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804056CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804056F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405900);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804059A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A00);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A3C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A64);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405AB4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405B98);

void func_80405C44(void) {
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405C4C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405CB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405D2C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405D54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405DE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405EAC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405ED4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405F24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405FC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406150);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804061C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406274);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804062F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406358);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406368);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406568);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406570);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406590);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804065CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406678);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406720);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804067BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406844);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804069FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406A24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406A74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406AA8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406B74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406B9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406BEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406C2C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406C74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406CB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406CFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F34);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040701C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804070C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407160);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407298);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804072CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804073B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407458);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804074C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407558);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804075A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804075D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407688);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804076C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804077D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407830);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040787C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040789C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804078D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407904);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407964);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407A44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407B14);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407BC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407BD0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407C10);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407DB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407E08);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407E64);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407F40);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407F68);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407FD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040801C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408060);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804080A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804081AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408394);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804083A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804083FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804084DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040855C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040859C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804085DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040861C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408644);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408654);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408664);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408674);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408684);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086DC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408730);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408768);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408798);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804088A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408928);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408958);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804089C4);
