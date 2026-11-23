#include "common.h"

s32 func_800AC010();

s32 func_800AC82C();
s32 func_800AD9B4();

void func_b31f_8016F78C();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b31f_8016B778();

s32 func_b31f_8016ADB8();

s32 func_800AB778();

s32 func_800AB538();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

typedef struct UnkDng708A4SP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng708A4SP;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;

void func_80047784(u8 *p, s32 b, s32 c);
extern s16 D_80083228;
void func_80047784(u8 *, s32, s32);
extern u8 D_b31f_801746D4[];

extern u8 D_b31f_80174A7C[];

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A908);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A928);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016A9FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016ABC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AC30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AD00);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016ADB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016AEDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016B0E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016B778);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016BD14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016BF74);

/* csrc/ovl_dng_b05.c's func_b05_80171D6C facing-table installer on the
   bank-local D_b31f_801746D4, with a *(s32 *)(p + 0x90) = 0 / b->unk_14 = 0 /
   p[0x9D] = 0 tail. The table base is a pointer local because it is used
   twice -- stored at c+0x2C and indexed. 34 words, 0 differing, first
   compile. */
void func_b31f_8016C698(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_b31f_801746D4;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(s32 *)(p + 0x90) = 0;
    b->unk_14 = 0;
    p[0x9D] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016C720);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016C8AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016C98C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CB0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CB80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CED4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016CF30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D1CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D204);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D5B4);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b31f_8016D678(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b31f_8016D698(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b31f_8016D6B8(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016D6F0);

void func_b31f_8016DA9C(void)
{
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b31f_8016DAA4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b31f_8016B778;
        }
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b31f_8016DB08(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b31f_8016B778;
        }
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DB6C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DBA4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DBDC(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DC14(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

/* 14 words, 0 differing. */
void func_b31f_8016DC4C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016B778;
    }
}

void func_b31f_8016DC84(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DC8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DCC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DDB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DE1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DECC);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_b31f_8016ADB8's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b31f_8016DEE8(void)
{
    func_b31f_8016ADB8();
}

void func_b31f_8016DF08(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF50);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016DF94);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E120);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E138);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E264);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E300);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E434);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016E528);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EB14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016ECE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EEBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016EF10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F140);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F160);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F2E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016F78C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016FA84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8016FCE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170214);

void func_b31f_8017021C(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170224);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801703A4);

/* csrc/ovl_dng_b05.c's func_b05_80171D6C facing-table installer on the
   bank-local D_b31f_80174A7C, with a *(s32 *)(p + 0x90) = 0 / b->unk_14 = 0 /
   p[0x9D] = 0 tail. The table base is a pointer local because it is used
   twice -- stored at c+0x2C and indexed. 34 words, 0 differing, first
   compile. */
void func_b31f_80170418(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_b31f_80174A7C;
    *(u8 **)(c + 0x2C) = t;
    func_80047784(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    *(s32 *)(p + 0x90) = 0;
    b->unk_14 = 0;
    p[0x9D] = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801704A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801707D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801707F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170814);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170834);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170854);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170874);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170894);

/* 14 words, 0 differing. */
void func_b31f_801708B4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801708EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017090C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017092C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017094C);

/* 14 words, 0 differing. */
void func_b31f_8017096C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801709A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801709C4);

/* 14 words, 0 differing. */
void func_b31f_801709E4(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

/* 14 words, 0 differing. */
void func_b31f_80170A1C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b31f_8016F78C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170A54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170AEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170BAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170BC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170C24);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170CE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170D44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170D60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170DCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80170EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017105C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017110C);

void func_b31f_80171128(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171130);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171168);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801712C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_8017140C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171470);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171488);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801714B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171594);

/* func_800D5AC0 (csrc/ovl_dungeon_800D50FC.c), byte for byte: one
   semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, whose layout here is the
   twin's exactly (+0x1C cursor, +0x24 OT base, +0xD0/+0xD4 out-parameters,
   +0x100 index). The three dead lbu's at +0x4/+0x5/+0x6 are the
   setRGB0(t, t->r0, t->g0, t->b0) echo of the colour word just copied in;
   the twin's own comment carries the measurements for that and for its three
   load-bearing orderings, which were not re-measured here because the first
   compile of the twin's body landed. 120 words, 0 differing. */
s32 func_b31f_80171704(u8 *p, UnkVec *v)
{
    UnkDng708A4SP *sp = (UnkDng708A4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = RotTransPers((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801718E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171A10);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171CC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171D90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171E60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80171F30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801720D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172160);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172274);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801722F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801723D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801724D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801725F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172610);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172874);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172A14);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172B00);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172CC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172DFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80172F44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801730AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E38);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173E84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80173F6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_801740F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80174320);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b31f", func_b31f_80174674);
