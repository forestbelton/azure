/* PARK func_800AB778 -- agent j51, 2026-11-16.
   RESIDUE: 401 words, 4 differing (aligndiff 4). See the final report for the
   full spelling table. Remaining: the case-0 `func_800A2C78` branch sense
   (target `bnez -> return` with the D_80083460 `lui` in the following `j`'s
   delay slot; this body gets `beqz -> join` and a `nop`), 2 words; and the
   `jal func_80099844` delay slot (target `andi $s0,$s0,0x4000`, this body
   `addiu $a1,$a1,%lo(D_800E0C34)`), 2 words.
*/

s32 func_800AB778(p, b, c, d)
u8 *p;
void *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *g4;
    u8 *e;
    u8 *base;
    Unk800E3E48 *sh;
    Unk800E3E48 *sh2;
    void *h;
    void *m;
    s32 f;
    s16 k;
    s16 n;
    s16 i;
    s16 j;

    f = *(s32 *)(d + 0x14);
    if ((f & 0x20000000) != 0)
    {
        *(s32 *)(d + 0x14) = f | 0x400000;
        func_800ACB98(p, b, c, d);
        return 0;
    }
    switch (p[0x9B])
    {
    case 0:
        if (d[0x43] == 0xFD)
        {
            *(s32 *)(D_800E3D7C + 0x110) = 0;
        }
        else if ((s16)func_800A2C78(d) != 0)
        {
            return 0;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] = 3;
        break;
    case 3:
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_80094E34();
        }
        if (d[0x43] < 0x40)
        {
            func_800C542C(d, D_800DCED4[func_800429E4(d)], d == *(u8 **)(D_800E3D7C + 0xAC), 2);
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 0x10;
            break;
        }
        if (*(u16 *)(c + 0x12) == 0)
        {
            *(u16 *)(c + 0x12) = 0xFF80;
        }
        *(s16 *)(c + 0x10) = 0x20;
        *(u16 *)(c + 0x14) |= 0xC;
        *(s16 *)(p + 0x96) = 0x10;
        *(s32 *)(d + 0x1C) |= 0x10000000;
        p[0x9B]++;
        break;
    case 4:
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x96);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x96);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1C) += (0x400 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1E) += (0x400 - *(u16 *)(c + 0x1E)) / *(s16 *)(p + 0x96);
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return 0;
        }
        h = func_800990FC();
        m = func_8009929C(8, h);
        if (d[0x43] == 0xFF)
        {
            m = func_80099194(D_800E0C58, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else if (d[0x43] == 0xFE)
        {
            m = func_80099194(D_800E0C67, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else
        {
            D_800DCE68 -= 1;
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
        }
        func_800A31D0(d);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        if (*(s32 *)(D_800E3D7C + 0x110) != 0)
        {
            func_800A18E8(d[0x13], 3);
            e = *(u8 **)(D_800E3D7C + 0x110);
            i = e[3] & 0x1F;
            j = i;
            ((u8 *)D_800E3DF0[i])[0x13] = 0;
            sh = D_800E3E48;
            *((u8 *)&sh[j] + 0x13) = 0;
            D_800E3DF0[i] = 0;
            func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        }
        else
        {
            func_800A18E8(d[0x13], 2);
        }
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) -= 1;
        if (*(s32 *)(g3 + 0xC) == (s32)d)
        {
            *(s32 *)(g3 + 0xC) = 0;
        }
        break;
    case 0x10:
        *(s32 *)(d + 0x14) = f | 0x4000;
        func_80099844(d, D_800E0C34);
        f &= 0x4000;
        if (f == 0)
        {
            *(s32 *)(d + 0x14) &= ~0x4000;
        }
        func_800A31D0(d);
        k = d[0x43];
        d[0x44] = 0;
        if ((s16)func_80042900((s8 *)d, 0xA) != 0)
        {
            d[0x13] = d[0xA8];
        }
        func_800422DC((u8 *)(k * 0x54 + 0x80010A80), (s32)d);
        func_800A18E8(d[0x13], 3);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        e = *(u8 **)(D_800E3D7C + 0x110);
        i = e[3] & 0x1F;
        j = i;
        ((u8 *)D_800E3DF0[i])[0x13] = 0;
        sh2 = D_800E3E48;
        *((u8 *)&sh2[j] + 0x13) = 0;
        D_800E3DF0[i] = 0;
        func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g4 = (u8 *)&D_80083460;
        *(u16 *)(g4 + 0xA) -= 1;
        break;
    }
    return 0;
}
