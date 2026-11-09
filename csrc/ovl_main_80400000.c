#include "common.h"
#include "main_twin.h"

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
 *
 * HOW A FUNCTION GETS HERE. `tools/decomp/twinalign.py --todo` pairs each of
 * this block's functions with the live function it came from and sorts by how
 * much they differ; --symmap gives the live -> twin rename. A body is moved
 * over with its references renamed, then measured with
 * `tools/decomp/ovlverify.py --G 8` -- note the --G 8, which is the opposite
 * of every other overlay unit.
 *
 * FIFTEEN PARKS, and their measurements are the interesting part. All 86
 * functions whose masked bodies were already identical at -G8 were moved;
 * 71 came out at 0 differing words and these 15 did not. Every one of them
 * differs ONLY in immediates, in four families, which says the shapes are
 * right and the twin's DATA LAYOUT and a couple of its constants are not:
 *
 *   delta  functions                                       what it looks like
 *   +0x18  func_804038C8 func_80403738 func_80403B84       one extra 24-byte
 *          func_80403CD4, with a +1 loop bound             UnkStrSet record
 *   -0x240 func_80402EBC func_804033EC func_80402BB8        a group struct's
 *   -0x84  func_80404314 func_804044D8 func_8040454C        pad differs; 0x240
 *   -0x34  func_8040553C func_80405900 func_804057B0        and 0x8B0 are
 *                                                           UNK_CARD_GROUP pads
 *   +8     func_804025F8                                    8 more bytes of
 *                                                           stack frame
 *   -1     func_80402DC8                                     one count lower
 *
 * The +0x18-with-a-+1-bound family is the one to read carefully: a table with
 * one MORE entry is not something an assert does. It is the first concrete
 * evidence for the open question in docs/where-we-are.md -- whether part of
 * this block is an EARLIER REVISION of the source rather than the same source
 * with asserts. Fixing these needs twin-specific struct instantiations (a
 * UNK_CARD_GROUP at the twin's pad), not a different spelling of the body.
 */

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

void func_80400908(void)
{
    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    func_804013F8(D_804094EC);
}

void func_80400948(void)
{
    long next_card_slot;

    func_804014E4();
    func_80408654(0);
    func_80408654(1);
    next_card_slot = 0x10 - D_804094EC;
    D_804094EC = next_card_slot;
    func_804013F8(next_card_slot);
}

void func_80400998(void)
{
    s32 *p = &D_804094E8;

    *p = 0;
    func_804014E4();
}

long func_804009C0(void)
{
    D_80409504 = CARD_STATE_EOF;
    return 0;
}

long func_804009D4(void)
{
    D_80409504 = CARD_STATE_ERROR;
    return 0;
}

long func_804009E8(void)
{
    D_80409504 = CARD_STATE_TIMEOUT;
    return 0;
}

long func_804009FC(void)
{
    D_80409504 = CARD_STATE_NEW;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400A10);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400E34);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80400EF8);

s32 func_804010E0(void)
{
    return D_804094E8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804010F0);

s32 func_8040122C(void)
{
    s32 r = 0;

    if (D_804094E8 == 2)
    {
        func_804014E4();
        if (func_80408674(D_804094EC) == 0)
        {
            D_804094E8 = 0;
            r = 5;
        }
        else
        {
            D_80409500 = 0;
            D_804094E8 = D_804094E8 + 1;
        }
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804012B0);

s32 func_804013A8(void)
{
    s32 *p = &D_804094E8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 4;
        *p = 0;
    }
    return r;
}

s32 func_804013D0(void)
{
    s32 *p = &D_804094E8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 5;
        *p = 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804013F8);

s32 func_80401444(void)
{
    s32 r = 0;

    if (func_8007C9C8(D_80409270) != 0)
    {
        r = 1;
    }
    else if (func_8007C9C8(D_80409274) != 0)
    {
        r = 2;
    }
    else if (func_8007C9C8(D_80409278) != 0)
    {
        r = 3;
    }
    else if (func_8007C9C8(D_8040927C) != 0)
    {
        r = 4;
    }
    return r;
}

void func_804014E4(void)
{
    func_8007C9C8(D_80409270);
    func_8007C9C8(D_80409274);
    func_8007C9C8(D_80409278);
    func_8007C9C8(D_8040927C);
}

CARD_STATE func_8040153C(void)
{
    CARD_STATE state = D_80409504;
    CARD_STATE old_state = CARD_STATE_OK;

    if (state != CARD_STATE_OK)
    {
        old_state = state;
        func_80401578();
    }
    return old_state;
}

void func_80401578(void)
{
    D_80409504 = CARD_STATE_OK;
    func_8007C9C8(D_80409280);
    func_8007C9C8(D_80409284);
    func_8007C9C8(D_80409288);
    func_8007C9C8(D_8040928C);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804015D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804016D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804017EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804018FC);

int func_804019A4(void)
{
    return D_804094EC != 0;
}

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

void func_804023C4(void)
{
    func_804022C8(5);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804023E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402428);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804024A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402508);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804025B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804025F8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402670);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040274C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804027F8);

void func_8040293C(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_8040343C(0);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402974);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402A1C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402BB8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402CD4);

void func_80402D90(s32 *p)
{
    func_80402CD4((u8 *)(p + 9), p);
    func_80402BB8(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402EBC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F04);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80402F84);

void func_804030CC(s32 a, s32 p)
{
    func_80402F84(a, 0, 1, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804030F4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040311C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403144);

void func_8040316C(s32 a, s32 p)
{
    func_80402F84(a, 4, 2, p);
}

void func_80403194(s32 a, s32 p)
{
    func_80402F84(a, 5, 1, p);
}

void func_804031BC(s32 a, s32 p)
{
    func_80402F84(a, 6, 2, p);
}

void func_804031E4(s32 a, s32 p)
{
    func_80402F84(a, 14, 2, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040320C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403234);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040325C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403284);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032AC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804032FC);

void func_80403324(s32 a, s32 p)
{
    func_80402F84(a, 15, 2, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040334C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403374);

void func_8040339C(s32 a, s32 p)
{
    func_80402F84(a, 18, 1, p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804033C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804033EC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040343C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804034D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403698);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403738);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804037F4);

s32 func_80403868(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804038C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804039F8);

s32 func_80403AA0(s32 a)
{
    return func_80047358(a, D_80408C9C, 0x12);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403AC8);

void func_80403B7C(s32 *p, s32 v)
{
    p[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403B84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403CD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403D7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403DA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403E54);

s32 func_80403F1C(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80403F7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804040C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404178);

void func_80404224(s32 *p)
{
    p[-4] = 0;
}

s32 func_8040422C(s32 a)
{
    return func_80047358(a, D_80408CB8, 0xA);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404254);

void func_80404308(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404314);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404364);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804044D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040454C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404570);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804045C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404618);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80404668);

void func_80404688(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_804044D8(q);
        func_8040454C((s32 **)q);
        func_80404178(q);
    }
}

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

void func_804056CC(s32 a)
{
    func_80047358(a, D_80408D28, 0x15);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804056F4);

void func_804057A8(s32 *p, s32 v)
{
    p[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804057F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405900);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804059A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A00);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405A3C);

void func_80405A64(s32 *p)
{
    func_80405A3C(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406368;
    }
}

void func_80405AB4(s32 *p)
{
    func_80405AE8(p);
    p[-4] = (s32)func_80406368;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405AE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405B98);

/* splat seeded this as an empty `(void)` function, which is a correct
 * spelling of its two words (`jr $ra`, `nop`) but not the source's: the
 * live counterpart func_8002519C takes a u8 *, and an empty body emits the
 * same two words either way. */
void func_80405C44(u8 *p)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405C4C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405CB8);

void func_80405D2C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405CB8;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405D54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405DE0);

void func_80405EAC(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80405DE0;
    }
}

void func_80405ED4(s32 *p)
{
    p[15] = 1;
    func_80403324((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80405EAC;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405F24);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80405FC4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406150);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804061C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406274);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804062F4);

void func_80406358(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406368);

void func_80406568(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406570(s32 *p, s32 v)
{
    func_80406568(p, v);
}

void func_80406590(s32 *p)
{
    func_80405AE8(p);
    func_80400908();
    p[-4] = (s32)func_80406368;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804065CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406678);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406720);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804067BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406844);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804069FC);

void func_80406A24(s32 *p)
{
    func_804069FC(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406DA0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406A74);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406AA8);

void func_80406B74(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80406AA8;
    }
}

void func_80406B9C(s32 *p)
{
    p[15] = 1;
    func_804031BC((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80406B74;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406BEC);

void func_80406C2C(s32 *p)
{
    p[13] = (s32)func_80406BEC;
    func_80403194((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_80406A24;
}

void func_80406C74(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_8040320C((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

void func_80406CB8(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_80403234((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406CFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406DA0);

void func_80406F0C(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406F14(s32 *p, s32 v)
{
    func_80406F0C(p, v);
}

void func_80406F34(s32 *p)
{
    func_804067BC(p);
    func_80400908();
    p[-4] = (s32)func_80406DA0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80406F70);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040701C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804070C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407160);

void func_80407298(s32 *p)
{
    func_8004DCCC(p + 1);
    func_8004DCCC(p + 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804072CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804073B0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407458);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804074C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407558);

s32 func_804075A8(s32 a)
{
    return func_80047358(a, D_80408D64, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804075D0);

void func_80407688(s32 *p, s32 v)
{
    p[0] = v;
    func_804073B0((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80407558(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804076C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804077D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407830);

void func_8040787C(s32 *p, s32 *b, s32 *c)
{
    func_804072CC((u8 *)(p + 8), b, c);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_8040789C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804078D8);

s32 func_80407904(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

void func_80407964(s32 *p)
{
    func_804078D8((u8 *)p);
    func_80407904((s32)(p + 1), 0x36, 0x10);
    func_8040789C(p);
    ((UnkCardPair *)p[0x3F])->unk_04->unk_08 = 0x8E;
    ((UnkCardPair *)p[0x3F])->unk_04->unk_0A = 0x82;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_08 = 9;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_08 = 0x42;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_08 = 0x24;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_0A = 0xA;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407A44);

s32 func_80407AEC(s32 a)
{
    return func_80047358(a, D_80408D74, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407B14);

void func_80407BC8(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80407BD0(s32 *p, s32 v)
{
    func_80407BC8(p, v);
    func_80407A44((UnkCardPair *)(p + 0x38), (u32 *)(p + 0x2E), (u32 *)(p + 0x32));
    func_80407964(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407C10);

void func_80407D0C(s32 *p)
{
    func_80051FBC(*(s32 *)p[0x3F]);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D38);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407D90);

void func_80407DB8(s32 *p)
{
    func_80407D90(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_804081AC;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407E08);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407E64);

void func_80407F40(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80407E64;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80407F68);

void func_80407FD4(s32 *p)
{
    p[11] = (s32)func_80407F68;
    func_804030CC((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_80407DB8;
}

void func_8040801C(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804030F4((s32)(p - 8));
    p[-4] = (s32)func_80407D90;
}

void func_80408060(s32 *p)
{
    p[11] = (s32)func_804081AC;
    func_804032D4((s32)(p - 8));
    p[-4] = (s32)func_80407DB8;
}

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
