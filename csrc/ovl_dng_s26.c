#include "common.h"

UnkNode *func_8003FC64(s32);
s32 func_800644B8(s32);
s32 func_80064584(s32);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
void func_s26_800240D8(u8 *p, UnkVec *b, UnkSlot *c);
extern u8 D_80045340[];
extern u32 D_800777DC[];
extern u8 D_800DECF8[];
extern u32 D_800814A0;
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
void func_s26_8002403C_3(u8 *, u8, u8 *) __asm__("func_s26_8002403C");
UnkNode *func_s26_800243B8_4(u8 *, UnkVec *, UnkVec *, s16) __asm__("func_s26_800243B8");

void func_s26_8002403C(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 2) == 0)
    {
        t = func_800A6870(k) + 16;
        u = t;
        if ((p[0x28] & 2) != 0)
        {
            u = t + (t >> 3);
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

void func_s26_800240D8(u8 *p, UnkVec *b, UnkSlot *c)
{
    u8 g;
    u16 t;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x36) = *(u16 *)(p + 0x36) + 1;
    c->unk_1A += 0x10;
    c->unk_1A %= 0x1000;
    switch (*(s16 *)(p + 0x34))
    {
    case 0:
        if (*(s16 *)(p + 0x36) >= *(s16 *)(p + 0x38))
        {
            *(s16 *)(p + 0x36) = 0;
            *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 1;
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        }
        break;
    case 1:
        b->unk_00 = *(s32 *)(p + 0x4) +
                    (func_800644B8(0x400 / *(s16 *)(p + 0x3A) * *(s16 *)(p + 0x36)) >> 4) * (*(s32 *)(p + 0x28) >> 8);
        b->unk_04 = *(s32 *)(p + 0x8) +
                    (func_800644B8(0x400 / *(s16 *)(p + 0x3A) * *(s16 *)(p + 0x36)) >> 4) * (*(s32 *)(p + 0x2C) >> 8);
        b->unk_08 = *(s32 *)(p + 0xC) +
                    (func_800644B8(0x400 / *(s16 *)(p + 0x3A) * *(s16 *)(p + 0x36)) >> 4) * (*(s32 *)(p + 0x30) >> 8);
        if (*(s16 *)(p + 0x36) >= *(s16 *)(p + 0x3A))
        {
            *(s16 *)(p + 0x36) = 0;
            *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 1;
        }
        break;
    case 2:
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 1 - rand() % 4;
        g = c->unk_0C[2] - 8;
        c->unk_0C[2] = g;
        c->unk_0C[1] = g;
        c->unk_0C[0] = g;
        t = c->unk_1E + 0x100;
        c->unk_1E = t;
        c->unk_1C = t;
        if (t > 0x1800)
        {
            *(s16 *)(p + 0x34) = *(u16 *)(p + 0x34) + 1;
            c->unk_0C[2] = 0x80;
            c->unk_0C[1] = 0x80;
            c->unk_0C[0] = 0x80;
            c->unk_1E = 0x1000;
            c->unk_1C = 0x1000;
            c->unk_10 |= 0x20;
            func_8003DB94(c, D_800DECF8, 0);
        }
        break;
    case 3:
        func_800478B8((u8 *)c);
        if ((c->unk_14 & 0x6000) == 0)
        {
            break;
        }
    case 4:
    default:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

UnkNode *func_s26_800243B8(u8 *p, UnkVec *b, UnkVec *c)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *v;
    s16 k;
    s32 a;
    s32 e;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s26_800240D8;
        *(u8 **)((u8 *)n + 0x20) = p;
        *(s16 *)(v + 0x34) = 0;
        *(s16 *)(v + 0x36) = 0;
        *(s16 *)(v + 0x3A) = *(u16 *)(p + 0x12);
        k = rand() % 48;
        a = rand() % 0x1000;
        e = rand() % 0x1000;
        *(s32 *)(v + 0x4) = b->unk_00;
        *(s32 *)(v + 0x8) = b->unk_04;
        *(s32 *)(v + 0xC) = b->unk_08;
        *(s32 *)(v + 0x1C) = c->unk_00 + (func_800644B8(a) >> 4) * (func_800644B8(e) >> 4) * k;
        *(s32 *)(v + 0x20) = c->unk_04 + (func_800644B8(a) >> 4) * (func_80064584(e) >> 4) * k;
        *(s32 *)(v + 0x24) = c->unk_08 + (func_80064584(a) >> 4) * k * 0x100;
        *(s32 *)(v + 0x28) = *(s32 *)(v + 0x1C) - b->unk_00;
        *(s32 *)(v + 0x2C) = *(s32 *)(v + 0x20) - b->unk_04;
        *(s32 *)(v + 0x30) = *(s32 *)(v + 0x24) - b->unk_08;
        r = (UnkSlot *)n->unk_0C;
        r->unk_0C[2] = 0xFF;
        r->unk_0C[1] = 0xFF;
        r->unk_0C[0] = 0xFF;
        r->unk_1E = 0x300;
        r->unk_1C = 0x300;
        r->unk_1A = rand() % 0x1000;
        r->unk_12 = 0x7E06;
        r->unk_08 = (u32)D_800777DC;
        r->unk_14 |= 0x100;
        r->unk_10 |= 0x60;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08;
        return n;
    }
    return NULL;
}

void func_s26_80024660(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q4;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
    UnkVec vv;
    u16 out[4];
    s16 oy;
    s32 bz;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(s32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        w = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        z = *(u16 *)(w + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            bz = *(s16 *)((u8 *)b + 0xA) + 0x20;
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - bz);
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) =
                ((s16) * (u16 *)(p + 0x12) >> 4) + ((s16) * (u16 *)(p + 0x12) >> 5);
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            func_800A56E0(0x300);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        else
        {
            q4 = *(u8 **)(o + 0xC);
            xx = q4[0x24];
            yy = q4[0x25];
            ox = xx;
            oy = yy;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88),
                                       (s16)(*(u16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                oy = yy;
                ox = xx;
            }
            v = &vv;
            i = 1;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) =
                ((s16) * (u16 *)(p + 0x12) >> 4) + ((s16) * (u16 *)(p + 0x12) >> 5);
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            *(s16 *)(p + 0xA) = 5;
        }
        for (i = 0x1F; i >= 0; i--)
        {
            *(UnkNode **)(p + 0x18 + i * 4) = func_s26_800243B8_4(p, b, v, i);
        }
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x28)
        {
            func_s26_8002403C_3(*(u8 **)(s + 0x60), p[0x9], s);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 5:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        if (*(s16 *)(p + 0x10) >= 0x28)
        {
            *(s16 *)(p + 0xA) = 4;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
