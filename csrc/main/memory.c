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
