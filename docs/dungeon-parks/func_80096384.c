/* RESIDUE 2026-11-16 j50: target 397 words, candidate 397 words, 29 differing. */
void func_80096384(u8 *p, void *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    void **t;
    u8 *m;
    u16 k;
    s32 e;
    s32 f;
    s32 w;
    s32 n;
    s32 r;
    s32 delta;
    s16 want;

    if (p[0x9A] != 0x32)
    {
        t = D_800DD274;
        *(void ***)(c + 0x2C) = t;
        func_8003DB94(c, t[((g->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9A] = 0x32;
        *(u16 *)(p + 0xA2) &= 0xFEFF;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B] = 0;
        *(s16 *)(p + 0x96) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) &= 0xFF7F;
    r = *(s16 *)(d + 0x64);
    if (r < 0)
    {
        func_80097898(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if ((*(s32 *)(d + 0x14) & 0x20000) != 0)
    {
        *(s32 *)(d + 0x14) &= 0xFFFDFFFF;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        func_80096AC0(p, b, c, d);
        return;
    }
    e = *(s32 *)(d + 0x1C);
    if ((e & 0x200) != 0)
    {
        func_80097C50((UnkDungeonActor *)p, b, c, d);
        return;
    }
    if ((*(u16 *)(q + 0x2) & 0x4) == 0)
    {
        if ((e & 0x20) != 0)
        {
            func_800969B8(p, b, c, d);
            return;
        }
        k = *(u16 *)0x80013714;
        if ((k & 1) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((*(u16 *)(p + 0xA2) & 0x200) != 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFDFF;
            }
            m = func_8009F868();
            if (m == NULL)
            {
                return;
            }
            want = (m[0x1] & 7) << 9;
            n = *(u16 *)(d + 0x2A) & 0xFFF;
            *(s16 *)(d + 0x2A) = n;
            if (n != want)
            {
                if ((*(u16 *)(d + 0x2A) & 0x800) != 0)
                {
                    *(s16 *)(d + 0x2A) |= 0xF800;
                }
                else
                {
                    *(s16 *)(d + 0x2A) &= 0x7FF;
                }
                want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                delta = *(s16 *)(d + 0x2A) - want;
                if (delta < 0)
                {
                    delta = -delta;
                }
                if (delta > 0x800)
                {
                    *(s16 *)(d + 0x2A) = (want & ~0xFFF) | (*(u16 *)(d + 0x2A) & 0xFFF);
                }
                if (*(s16 *)(d + 0x2A) > want)
                {
                    *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) - 0x200;
                }
                else
                {
                    *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
                }
                func_8009F988();
                goto tail;
            }
            D_800E3544 = m[0x1] & 0xF8;
            switch (D_800E3544)
            {
            case 8:
                func_80096C24(p, b, c, d);
                return;
            case 0x10:
                func_800969B8(p, b, c, d);
                return;
            case 0x18:
                func_800973F0(p, b, c, d);
                goto tail;
            case 0xD8:
                D_800DD830[m[0x0] & 0x7F]();
                goto tail;
            }
            goto tail;
        }
        D_800E3544 = 0;
        if ((k & 4) != 0)
        {
            func_80040AA0(3);
            return;
        }
        if ((s16)func_80094EA4() != 0)
        {
            *(s16 *)(p + 0xA4) = 0;
        }
        if ((*(u16 *)(p + 0xA2) & 0x200) != 0 && (s16)func_800A2C34(d) == 0 &&
            (*(u16 *)(q + 0x2) & 0x4) == 0 &&
            (s16)func_800A4474(c[0x24], c[0x25]) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFDFF;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
        f = g->unk_08;
        if ((f & 0x80) != 0)
        {
            *(s16 *)(d + 0x8A) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = NULL;
            *(s32 *)(p + 0xC8) = 0;
            *(s32 *)(p + 0x104) = 0;
            return;
        }
        if ((f & 3) != 0)
        {
            func_800A56E0(0x506);
            return;
        }
        n = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
        if ((s16)n != 0xFFF)
        {
            *(s16 *)(d + 0x2A) = n;
            if ((g->unk_08 & 0x10) == 0 && (*(u16 *)(p + 0xA2) & 0x400) == 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFFE;
                if ((*(s32 *)(d + 0x1C) & 0x400) != 0)
                {
                    *(u16 *)(d + 0x2A) += func_800A6D30() & 0xE00;
                }
                func_80096C24(p, b, c, d);
                if (p[0x9A] != 0x32)
                {
                    return;
                }
            }
            else
            {
                *(s16 *)(p + 0x96) = 0;
            }
        }
        w = g->unk_08;
        if ((w & 0x30) == 0x30)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            func_800969B8(p, b, c, d);
            return;
        }
        if ((*(u16 *)(p + 0xA2) & 0x40) != 0)
        {
            if ((w & 0x40) == 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFBF;
            }
        }
        else if ((w & 0x40) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((s16)func_800973F0(p, b, c, d) != 0)
            {
                return;
            }
        }
    }
tail:
    if ((*(u16 *)(p + 0xA2) & 0x1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        t = D_800DD274;
        *(void ***)(c + 0x2C) = t;
        func_8003DB94(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}
