/* func_800B1768 (ovl_dungeon) -- PARK, work in progress: 234 of 234 words, 56 aligned differences. */
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *t;
    u32 *col;
    s32 i;
    s32 j;
    s32 has;
    s32 w;
    s32 v;
    s32 base;
    s32 z;

    i = 0;
    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    if (e != 0)
    {
        *(u16 *)(r + 0x14) |= 1;
    }
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0xC) = y;
    *(s16 *)((u8 *)m + 0xA) = 0;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    j = d & 0xFF;
    *(s16 *)(q + 0x24) = d >> 8;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    col = (u32 *)((u8 *)n + 0x58);
    u = func_800B12F4();
    *(u8 **)(q + 0x28) = u;
    if (u != NULL)
    {
        if (k != 0)
        {
            i = 1;
            col = (u32 *)((u8 *)n + 0x5C);
            *(u32 *)((u8 *)n + 0x70) = (u32)u;
            u = func_800B1434(u, D_80073618[k]);
            has = 1;
            *(u32 *)((u8 *)n + 0x58) = 0x808080;
        }
        else
        {
            has = 0;
        }
        *(s32 *)(q + i * 4 + 0x50) = (s32)func_8004E298(u, D_800DF03C[j], 0);
        i++;
        w = func_80069E98(D_800DF03C[j]) * 8;
        base = has * 10;
        func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : (s16)(base + 0x80),
                      (s16)(-*(s8 *)(u + 0x3) - 0x9));
        func_800B135C(u, e != 0 ? 4 : 8);
        func_800B13CC(u, 0x20);
        func_800B1400(u, 2);
        *col++ = 0x808080;
        u += 0x60;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        t = func_800B1434(u, u - 0x60);
        func_800B1320(u, 1, 1);
        *col = 0x80808;
        *(s32 *)(q + i * 4 + 0x50) = (s32)t;
        i++;
        t[0x1] = 0x38;
        v = has == 0 ? -0x2 : -0xA;
        if (e != 0)
        {
            v -= 2;
        }
        *(s8 *)(t + 0x2) = v;
        *(s8 *)(t + 0x3) = -0xE;
        t[0x9] = 0;
        t[0x8] = 0;
        z = w + has * 24;
        if (has == 0)
        {
            z += 4;
        }
        if (e != 0)
        {
            z -= 2;
        }
        t[0xA] = z;
        t[0xB] = 0x10;
        t[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(t + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(t + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(t + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(t + 0x14) = *(s32 *)(q + 0x1C);
        t[0x1] |= 2;
        t += 0x18;
        *(s32 *)(q + i * 4 + 0x50) = (s32)t;
        t[0x1] = 0x81;
        *(s16 *)(t + 0x4) = 0x20;
        *(s16 *)(t + 0x6) = 0;
        t[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}
