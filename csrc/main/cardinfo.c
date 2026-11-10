#include "common.h"
#include "main.h"
#include "strings.h"

/* cardinfo.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 123..171. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine".

   Its rodata is 0x800200A8..0x800200C0: func_800258C8's own four-entry
   function-pointer constant, which cc1psx emits right after that function,
   then SAVE_ICON_VRAM_RECT. The RECT is declared at the END of this file so
   the constant precedes it, which is the order the rodata stream has. */

/* The save-slot detail panel, and the overlay's largest function: eight text
   sprites built into one 0x100-byte stack buffer and handed to the font
   renderer, then eight icon slots driven by a bitmask.

   Four things are load-bearing:

   - D_80083E98 has to be the UnkSaveRow STRUCT alias. The first row access
     in the function is the name at +0xC, so gcc relocates against
     D_80083E98+0xC -- splat calls that D_80083EA4 -- holds it in $s1 across
     every call and derives the row base as `$s1 - 0xC` in $fp for the other
     seven fields. Through the two separate symbols each access materialises
     its own %hi and the function is five words longer.
   - Each block's `e = ((UnkCardGrpB20 *)p)->ptrs[k]` has to come BEFORE that
     block's calls, not after: $s3 is callee-saved and the target loads it
     first.
   - The two clamps are `x >= 1000 ? 999 : x`, not `x < 1000 ? x : 999` --
     the target branches on the false case with the 999 in the delay slot.
   - In the icon loop the mask needs its own local (`m = 1 << i`) or combine
     rewrites `flags & (1 << i)` into `(flags >> i) & 1`, and the flags word
     needs one too, which is what settles the order of the two hoists in the
     loop preheader. The two arms assign a pointer and share one store.

   The playtime is `row.unk_1C / 60` unsigned for seconds, then `/ 3600` and
   `/ 60` signed for hours and minutes -- three different magic multiplies. */
void func_80024390(s32 *p)
{
    u8 buf[0x100];
    u8 num[0x10];
    UnkCardPair *e;
    s32 sec;
    s32 hr;
    s32 mn;
    s32 i;
    s32 m;
    u8 *s;

    e = ((UnkCardGrpB20 *)p)->ptrs[4];
    func_8003AD08(p[31] + 1, buf);
    strcat(buf, D_800282D8);
    strcat(buf, D_80083E98_s[p[31]].name);
    func_8004DA74((u8 *)p + 0x84, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x84);
    e->unk_04->unk_08 = 0xA8;
    e->unk_04->unk_0A = 0x110;
    e = ((UnkCardGrpB20 *)p)->ptrs[5];
    strcpy(buf, D_800282E8);
    func_8004DA74((u8 *)p + 0x204, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x204);
    e->unk_04->unk_08 = 0xA0;
    e->unk_04->unk_0A = 0x110;
    if (D_80083E98_s[p[31]].unk_08 != 0)
    {
        e = ((UnkCardGrpB20 *)p)->ptrs[6];
        strcpy(buf, D_800283A8[2]);
        func_8003AD08(D_80083E98_s[p[31]].unk_34, num);
        strcat(buf, num);
        strcat(buf, D_800283A8[3]);
        func_8004DA74((u8 *)p + 0x384, buf, 1);
        e->unk_00 = (s32)((u8 *)p + 0x384);
        e->unk_04->unk_08 = 0xB8;
        e->unk_04->unk_0A = 0x140;
    }
    e = ((UnkCardGrpB20 *)p)->ptrs[7];
    func_8004E5A0(D_80083E98_s[p[31]].unk_24, 0xA, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x504, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x504);
    e->unk_04->unk_08 = 0x148;
    e->unk_04->unk_0A = 0x110;
    sec = D_80083E98_s[p[31]].unk_1C / 60;
    hr = sec / 3600;
    e = ((UnkCardGrpB20 *)p)->ptrs[8];
    func_8004E5A0(hr >= 1000 ? 999 : hr, 3, buf);
    strcat(buf, (u8 *)D_800200A4);
    mn = sec / 60;
    func_8004E5E8(mn - hr * 60, 2, num);
    strcat(buf, num);
    strcat(buf, (u8 *)D_800200A4);
    func_8004E5E8(sec - mn * 60, 2, num);
    strcat(buf, num);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x684, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x684);
    e->unk_04->unk_08 = 0x150;
    e->unk_04->unk_0A = 0x120;
    e = ((UnkCardGrpB20 *)p)->ptrs[9];
    func_8004E5A0(D_80083E98_s[p[31]].unk_2C >= 1000 ? 999 : D_80083E98_s[p[31]].unk_2C,
                  3, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x804, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x804);
    e->unk_04->unk_08 = 0x100;
    e->unk_04->unk_0A = 0x120;
    e = ((UnkCardGrpB20 *)p)->ptrs[10];
    func_8004E5A0(D_80083E98_s[p[31]].unk_28, 2, buf);
    func_8004E69C(buf);
    func_8004E298((u8 *)p + 0x984, buf, 1);
    e->unk_00 = (s32)((u8 *)p + 0x984);
    e->unk_04->unk_08 = 0x188;
    e->unk_04->unk_0A = 0x130;
    for (i = 0; i < 8; i++)
    {
        s32 f = D_80083E98_s[p[31]].unk_30;

        e = ((UnkCardGrpB20 *)p)->ptrs[11 + i];
        m = 1 << i;
        if (f & m)
        {
            s = D_80027FD4[i];
        }
        else
        {
            s = D_80027FD4[8];
        }
        e->unk_00 = (s32)s;
        e->unk_04->unk_08 = 0x34 + 0x10 * i;
        e->unk_04->unk_0A = 0xB0;
    }
}

/* Three label layouts for the save-slot list, then a loop writing the text
   sprites. `k` is `p[32] == 1` -- gcc's `xori` plus `sltu 1` -- and it is used
   BOTH as the first condition and as an index into D_800283B8 and
   D_800283F4, even though it is provably 0 on the two arms that index with
   it. n/3 is the signed magic multiply by 0x55555556. */
void func_80024818(s32 *p)
{
    u8 buf[0x40];
    u8 num[0x10];
    s32 k = p[32] == 1;
    s32 n;
    s32 cnt;
    s32 i;

    n = get_free_blocks();
    if (k != 0 && count_used_save_slots() == 0)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
        func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
        cnt = 2;
    }
    else if (n < 3)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283C4, 1);
        func_8004DA74((u8 *)p + 0x204, D_800283B8[k], 1);
        func_8004DA74((u8 *)p + 0x384, D_800283B8[2], 1);
        cnt = 3;
    }
    else
    {
        func_8004DA74((u8 *)p + 0x84, D_800283F4[k], 1);
        func_8003AD08(n / 3, num);
        strcpy(buf, num);
        strcat(buf, D_800283FC);
        func_8004DA74((u8 *)p + 0x204, buf, 1);
        cnt = 2;
    }
    for (i = 0; i < cnt; i++)
    {
        UnkCardPair *e = ((UnkCardGrpB20 *)p)->ptrs[4 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + 0x180 * i);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + 0x10 * i;
    }
}

void func_800249B4(s32 *p)
{
    s32 i;

    func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
    func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
    for (i = 0; i < 2; i++)
    {
        UnkCardPair *e = ((UnkCardGrpB20 *)p)->ptrs[4 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + i * 0x180);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + i * 0x10;
    }
}

void func_80024A50(s32 *p)
{
    s32 n = p[0x1F];

    if (D_80083E98[n][0] != 0)
    {
        func_80024390(p);
    }
    else
    {
        s32 m = p[0x20];

        if (m != 0 && m != 2)
        {
            func_80024818(p);
        }
        else
        {
            func_800249B4(p);
        }
    }
}

void func_80024ACC(UnkCardGrpB20 *c)
{
    s32 *p = (s32 *)c;
    UnkCardPair *q;

    func_800242D8((u8 *)(p + 1), 0xF8, 0x48);
    q = c->ptrs[3];
    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0xB0;
    q->unk_00 = (s32)(p + 1);
    q = c->ptrs[0];
    q->unk_04->unk_08 = 0x60;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079054;
    q = c->ptrs[1];
    q->unk_04->unk_08 = 0xD0;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_800790B4;
    q = c->ptrs[2];
    q->unk_04->unk_08 = 0x20;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079114;
}

void func_80024BB4(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

void func_80024C48(s32 a)
{
    func_800491F4(a, D_80027FF8, 0x15);
}

void func_80024C70(UnkCardGrpB20 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

void func_80024D10(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80024D18(UnkCardGrpB20 *c, s32 v)
{
    func_80024D10((s32 *)c, v);
    func_80024BB4(&c->root, c->root_prim, c->root_shared);
    func_80024ACC(c);
}

s32 *func_80024D58(s32 *p, s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002A990;
    s32 *r = q + 8;
    UnkCardGrpB20 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002A990, 0x394);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024EEC(q);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0xE30);
    }
    r[0x1F] = a;
    r[0x20] = b;
    c = (UnkCardGrpB20 *)r;
    func_80024C70(c, 0x15);
    q[3] = (s32)&c->root;
    c->root.unk_0C = (s32)func_80024C48w((s32 *)c->ptrs);
    func_80024D18(c, (s32)p);
    q[4] = 0;
    return q;
}

void func_80024E44(UnkCardGrpB20 *c)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004E994(c->ptrs[4 + i]->unk_00);
        c->ptrs[4 + i]->unk_00 = 0;
    }
    for (i = 0; i < 4; i++)
    {
        c->ptrs[7 + i]->unk_00 = 0;
    }
    for (i = 0; i < 10; i++)
    {
        c->ptrs[11 + i]->unk_00 = 0;
    }
}

void func_80024EEC(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80024E44((UnkCardGrpB20 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80024F3C(s32 *p, s32 a, s32 b)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x1F] = a;
        q[0x20] = b;
        func_80024E44((UnkCardGrpB20 *)q);
        func_80024A50(q);
    }
}

void func_80024F7C(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80024FAC(s32 *p)
{
    func_80024F7C(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002593C;
    }
}

void func_80024FFC(s32 *p)
{
    func_80025030(p);
    p[-4] = (s32)func_8002593C;
}

void func_80025030(s32 *p)
{
    s32 flag;
    s32 off;
    s32 i;

    if (p[0x12] == 1)
    {
        flag = 1;
    }
    else
    {
        func_80023A50((s32 *)p[6], p[0x11]);
        flag = 0;
    }
    off = 0;
    i = 0;
    do
    {
        if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], flag);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}

void func_800250E8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_80024224((s32 *)p[off + 1], 1);
        }
        else if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}

/* Empty, but it takes an argument: func_80025230 sets $a0 to the bare
   0x80010000 base before calling it and sets nothing else. A callee that
   never reads its parameter compiles identically either way, so this is the
   caller's requirement, not the callee's. */
void func_8002519C(u8 *p)
{
}

void func_800251A4(s32 *p)
{
    u16 *b = (u16 *)0x80010000;
    s32 v;

    func_80027BF4();
    v = 1;
    if (p[0x11] == 0)
    {
        v = 3;
    }
    b[0x1B8A] = v;
    D_80082E60.unk_16 |= 2;
    func_80025D34(p - 8);
    func_800439F8();
    func_80040AA0(b[0x104] != 0 ? 5 : 6);
}

void func_80025230(s32 *p)
{
    if (read_save_file(p[10], (u8 *)0x80010000, 1) == 0)
    {
        p[13] = (s32)func_8002593C;
        msg_load_failed((s32)(p - 8));
        p[-4] = (s32)func_80024F7C;
    }
    else
    {
        func_8002519C((u8 *)0x80010000);
        p[-4] = (s32)func_800251A4;
    }
    recheck_card_slot();
    p[16] = 0;
}

void func_800252A4(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025230;
    }
}

void func_800252CC(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[15] = 1;
        msg_loading((s32)(p - 8), (s32)(p + 16));
        p[16] = 1;
        p[-4] = (s32)func_800252A4;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        msg_no_saved_game((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025358(s32 *p, s32 v)
{
    set_save_title((u8 *)p, v);
    set_save_icon(p, v);
}

/* func_80025358's first argument is the 0x80010000 base, not 'p - 8': the
   'lui $a0, 0x8001' in that jal's delay slot is the live argument. The
   'addiu $a0, $s2, -0x20' that reorg puts in BOTH beqz delay slots is the
   fail block's own msg_copy_failed argument, hoisted out of the block because
   every predecessor carries it. Spelling 'p - 8' here instead cost exactly
   one word (53w/1 DIFF) across twelve measured variants of the constant and
   of the 'p - 8' sites; nothing about the local or the goto structure was
   ever wrong. */
void func_80025398(s32 *p)
{
    u8 *b = (u8 *)0x80010000;
    s32 i;
    s32 off;

    if (read_save_file(p[11], b, 0) == 0)
    {
        goto fail;
    }
    func_80025358((s32 *)b, p[10]);
    if (write_save_file(p[10], b) == 0)
    {
        goto fail;
    }
    func_8002519C(b);
    refresh_card_info();
    off = 0;
    for (i = 0; i < 5; i++)
    {
        func_80024298((s32 *)p[off + 1]);
        off++;
    }
    func_80024FFC(p);
    goto done;
fail:
    p[13] = (s32)func_80024FFC;
    msg_copy_failed((s32)(p - 8));
    p[-4] = (s32)func_80024F7C;
done:
    recheck_card_slot();
    p[16] = 0;
}

void func_8002546C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025398;
    }
}

void func_80025494(s32 *p)
{
    p[15] = 1;
    msg_copying((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_8002546C;
}

void func_800254E4(s32 *p)
{
    s32(*tbl)[32] = D_80083E98;

    if (tbl[p[10]][0] == 0)
    {
        if (get_free_blocks() < 3)
        {
            p[13] = (s32)func_80024FFC;
            msg_not_enough_blocks((s32)(p - 8));
            p[-4] = (s32)func_80024FAC;
        }
        else
        {
            func_80025494(p);
        }
    }
    else
    {
        p[13] = (s32)func_80024FFC;
        msg_cannot_overwrite((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

/* The third of the input handlers, and the closest to func_800265B8 -- same
   0x40 / 0x5000 chain, same `p[12] >= 0xD` arm first, same pair of separate
   exit tests -- with three differences: the 0x20 case FALLS THROUGH into the
   0x40 test instead of leaving through a goto, the held-key step is -2 rather
   than -4, and func_80027AFC takes a literal 0 as its second argument. The
   unk_08 test has to be an enclosing `if`, not an early return: on the
   zero path the target still runs the func_8002168C/print_card_status tail. */
void func_80025584(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            p[-4] = (s32)func_80024FFC;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_800254E4(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 2;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            p[10] = func_80049DE8(p[10], dir, 5);
            func_800250E8(p);
        }
    }
    r = func_8002168C();
    print_card_status();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

void func_8002570C(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[11] = n;
        func_800250E8(p);
        p[-4] = (s32)func_80025584;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        msg_no_saved_game((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025788(s32 *p)
{
    s32 i;
    s32 off;

    if (p[14] != 0)
    {
        p[-4] = (s32)func_8002593C;
    }
    else
    {
        if (erase_save_file(p[10]) != 0)
        {
            p[-4] = (s32)func_8002593C;
            off = 0;
            for (i = 0; i < 5; i++)
            {
                func_80024298((s32 *)p[off + 1]);
                off++;
            }
        }
        else
        {
            p[13] = (s32)func_8002593C;
            msg_delete_failed((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
        recheck_card_slot();
        p[16] = 0;
    }
}

void func_80025848(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[13] = (s32)func_80025788;
        msg_delete_confirm((s32)(p - 8), (s32)(p + 14));
        p[-4] = (s32)func_80024FAC;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        msg_no_saved_game((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_800258C8(s32 *p)
{
    void (*fns[4])(s32 *) = {func_800252CC, func_8002570C, func_80025848, func_800252CC};

    fns[p[0x11]](p);
}

void func_8002592C(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

/* The fifth input handler, and the widest: two flags rather than one. `dir`
   drives the func_80049DE8 step (through p[10] with a limit of 5 or p[17]
   with 3, depending on p[18]) and `upd` records that the layout changed, so
   func_80025030 can run once at the very end. Its 0x20 case RETURNS, which
   is why it skips that final call. Same two levers as its siblings -- the
   `p[12] >= 0xD` arm first, and the two exit tests nested rather than
   `r == 0 || r == 1`. */
void func_8002593C(s32 *p)
{
    s32 dir = 0;
    s32 upd = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
            return;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            if (p[18] == 0)
            {
                func_8002592C(p);
                upd = 1;
            }
            else
            {
                func_800258C8(p);
            }
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x2000)
            {
                p[18] = 1;
            }
            else
            {
                p[18] = 0;
            }
            upd = 1;
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            if (p[18] == 1)
            {
                p[10] = func_80049DE8(p[10], dir, 5);
            }
            else
            {
                p[17] = func_80049DE8(p[17], dir, 3);
            }
            func_80025030(p);
        }
    }
    r = func_8002168C();
    print_card_status();
    if (r != 0)
    {
        if (r != 1)
        {
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
        }
    }
    if (upd != 0)
    {
        func_80025030(p);
    }
}

void func_80025B38(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80025B40(s32 *p, s32 v)
{
    func_80025B38(p, v);
}

void func_80025B60(s32 *p)
{
    func_80025030(p);
    recheck_card_slot();
    p[-4] = (s32)func_8002593C;
}

void func_80025B9C(s32 *p)
{
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

void func_80025BD4(s32 *p)
{
    s32 n = p[9];

    if (n < MEM_CARD_FILE_ID_TMP)
    {
        if (load_save_slot_info(n) != 0)
        {
            p[n + 1] = func_80023FF0((s32)(p - 8), n);
            p[9] = p[9] + 1;
        }
        else
        {
            p[13] = (s32)func_80025B9C;
            msg_load_failed((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
    }
    if (n == MEM_CARD_FILE_ID_TMP)
    {
        p[-4] = (s32)func_80025B60;
    }
}

s32 *func_80025C7C(s32 *p, s32 a)
{
    s32 *q = func_8003FE78(0, D_8002B7E0, 0x1C);
    s32 *r = q + 8;

    func_80025B40(r, (s32)p);
    erase_tmp_file();
    refresh_card_info();
    r[0x12] = 1;
    q[8] = func_8004F418(q, q + 0x1A);
    r[6] = (s32)func_800238D4(q);
    r[7] = (s32)func_80024D58(q, r[10], 0);
    r[9] = 0;
    q[4] = (s32)func_80025BD4;
    return q;
}

/* The destructor family's other member, and identical in shape to
   func_80026920: only the LAST of the four calls is inside the loop. The park
   had all three of func_80023A00, func_80024EEC and func_80024184 in the
   loop body, which is 36 words against 37 and 31 differing; reading the
   branch target (word 22's `bnez` goes to word 17, not word 11) says
   otherwise. The two outside calls take q[6] and q[7], the one inside takes
   q[1] with the cursor advancing. */
void func_80025D34(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        func_80023A00((s32 *)q[6]);
        func_80024EEC((s32 *)q[7]);
        i = 0;
        do
        {
            func_80024184((s32 *)q[1]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_80025DC8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_800241D4((s32 *)p[off + 3], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 3]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[1], p[11], 3);
}

s32 func_80025E54(void)
{
    s32 mask = 0;
    s32 i = 0;

    do
    {
        if (func_80033B2C(D_80028010[i]) != 0)
        {
            mask += 1 << i;
        }
    } while (++i < 8);
    return mask;
}

/* The byte table is NOT the symbol splat prints. The listing reads
   `lui $v1, %hi(D_800133A4)` / `lbu $v0, %lo(D_800133A4)($v1)` with `$v1`
   incremented by 2 each iteration -- a %lo in the displacement with a
   REGISTER index, which per docs/matching-levers.md:1870 cannot be a symbol
   reference at all: cse folds a %lo into a MEM only when what is left is a
   constant offset off the `(high)` pseudo. It is the constant base
   0x80010000, whose low half is zero so it needs a bare `lui`, with 0x33A4
   written inside the index expression. splat prints the displacement
   symbolically because 0x800133A4 happens to be in its symbol list.

   Four symbol spellings cost the extra `addiu` the target does not have, all
   17 words against 16: `extern u8 D_800133A4[]` indexed, the same as a
   scalar through `(&D_800133A4)[k]`, a pointer walked by 2, and
   `*((u8 *)0x800133A4 + i * 2)` -- the last one being a constant too, but
   with a NONZERO low half, so it still has to be materialised. The zero low
   half of 0x80010000 is the whole point. */
s32 func_80025ECC(void)
{
    u8 *b = (u8 *)0x80010000;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < 0x22; i++)
    {
        sum += D_80028020[b[i * 2 + 0x33A4]];
    }
    return sum;
}

/* Two levers here, and the long-standing park had both backwards.

   0x80010208 and 0x8001022C are CONSTANT addresses, not symbols. Through a
   symbol the address is split into a separate `high` pseudo, so the word load
   comes out as `lui $v0, %hi` + `lw $a0, %lo($v0)` -- two registers. A constant
   address materialises 0x80010000 into the DESTINATION and puts 0x22C in the
   load's displacement, which is the listing's `lui $a0` + `lw $a0, 0x22C($a0)`.
   ovlverify's mask cannot see the difference (the immediate is the masked
   field), so this one only shows up in the link.

   `p + 4` is written inline at every call site, not kept in a local. cse1
   gives it one pseudo per extended basic block, so the uses before the `if`
   are one block-local quantity and the two after the join are another; both
   are local-alloc's, and because the first outranks `n` it takes $s0 and
   pushes `n` to $s1. A single local for p+4 spans the branch, becomes
   global-alloc's, and the two end up the other way round (61w/31).

   The ternary -- rather than an `if`/`else` over a local -- is what buys the
   last word. expand_call computes arg0 before expanding the COND_EXPR, so
   `p + 4` lands in a fresh pseudo BEFORE the branch; cse1 rewrites it to a
   copy of the first quantity, and that copy survives into sched1 because
   combine cannot propagate across the block boundary. The copy's only use is
   `$a0 = <copy>` at the shared call, so local-alloc hands the copy $a0 itself:
   the copy becomes `addu $a0, $s0, $zero` in the block ABOVE the branch, where
   the beqz's backward scan takes it for the delay slot, and the argument move
   in the join block collapses to a self-move and disappears. The join then
   begins with the call, so its backward scan stops at the label and its slot
   is filled forwards with the next `addiu $s0, $s3, 0x4`. Writing the test as
   a statement leaves both insns in the join and costs the word (60w/4). */
void set_save_title(u8 *p, s32 n)
{
    u8 buf[0x10];

    strcpy(p + 4, D_80028294[0]);
    func_8003AD08(n + 1, buf);
    strcat(p + 4, buf);
    strcat(p + 4, D_80028294[3]);
    strcat(p + 4, (u8 *)0x8001020C);
    strcat(p + 4, D_80028294[3]);
    strcat(p + 4, *(s16 *)0x80010208 != 0 ? D_80028294[1] : D_80028294[2]);
    strcat(p + 4, D_80028294[3]);
    func_8003AD08(*(s32 *)0x8001022C, buf);
    strcat(p + 4, buf);
}

/* A three-pass VRAM upload. 0x80010080 is a constant address, not a symbol --
   the `lui` + `ori` pair is the tell (see docs/addressing-and-translation-units.md).
   Both block copies have to go through the alignment-1 structs: the loads are
   lwl/lwr, so gcc is not allowed to assume either side is word-aligned.
   The `r.y` update must come AFTER the row copy (43 DIFF before it), and the
   source pointer must be `(u8 *)0x80010080 + i * 0x80` rather than a walking
   local advanced at the bottom of the loop (56 DIFF). */
void set_save_icon(s32 *p, s32 n)
{
    RECT r;
    s32 i;

    r = SAVE_ICON_VRAM_RECT;
    *(UnkBlock20 *)((u8 *)p + 0x60) = *(UnkBlock20 *)SAVE_ICON_CLUTS[n];
    r.y += n * 16;
    for (i = 0; i < 3; i++)
    {
        StoreImage(&r, (u_long *)((u8 *)0x80010080 + i * 0x80));
        r.x += 4;
    }
    DrawSync(0);
}

void init_save_file_header(u8 *p, s32 a, s32 mode)
{
    u8 *b = (u8 *)0x80010000;
    s32 f;

    *(UnkFour *)p = *(UnkFour *)SAVE_FILE_MAGIC;
    f = (mode ^ 2) == 0;
    *(s32 *)(b + 0x224) = *(s32 *)(b + 0x2D5C);
    *(s16 *)(b + 0x208) = f;
    *(s16 *)(b + 0x20A) = f;
    *(s32 *)(b + 0x230) = func_80025E54();
    *(s32 *)(b + 0x228) = func_80025ECC();
    set_save_title((u8 *)p, a);
    set_save_icon((s32 *)p, a);
}

const RECT SAVE_ICON_VRAM_RECT = {0x380, 0x100, 4, 0x10};
