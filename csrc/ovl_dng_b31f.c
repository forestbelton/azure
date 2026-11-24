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

u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s32 func_b31f_8016C720(u8 *p, UnkVec *b, u8 *c, u8 *d);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

void func_800478B8(u8 *);

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

/* Can the record step onto the slot it is facing, and if so close on it:
   func_b04_80172270 (csrc/ovl_dng_b04.c) with its D_80175140 re-dress and
   func_80047784 call removed and d[0x85] set to 4 rather than 0 -- so the
   whole func_800A04F0 / func_800A2CB8 / func_800A0134 / func_800A2B5C
   interrogation, the func_800C7930 push, state 0x11 and the func_8009C93C
   re-aim are that function's verbatim. 99 words, 0 differing, first compile.
   Three of the twin's facts were carried over unmeasured because of that:
   the 8-byte `unused' local that makes the frame 0x40, the shared `-1' exit
   living in the SECOND func_800A2B5C test, and func_800A04F0 coming through
   the unit's `_r' alias with an `s16' fourth parameter.

   THIS UNIT'S FORWARD DECLARATION OF THIS FUNCTION HAD TO BE WIDENED from
   `s32 func_b31f_8016C720(u8 *p, void *b, void *c, u8 *d)' to the real
   `(u8 *p, UnkVec *b, u8 *c, u8 *d)'. Its only caller, func_b15_801721D8
   below, passes `void *' arguments that convert implicitly, so no call site
   changed; every landed body in this unit was re-verified afterwards and all
   are still 0 differing, func_b15_801721D8's 466 words included.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x6.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_b31f_8016C720(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 unused[2];

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) == 0)
    {
        return 0;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
    {
        goto fail;
    }
    if ((u16)(0x40 + -func_800A0134(t, d)) > 0x80)
    {
        return 0;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    d[0x84] = 0x7C;
    d[0x85] = 6;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

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

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_8016E138. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A858[] = {
    0x01,
    0xF4,
    0x08,
    0xF8,
    0x08,
    0xF9,
    0x06,
    0x07,
    0xF8,
    0xF8,
    0x01,
    0xF2,
    0x00,
    0x00,
    0x00,
    0x00,
};

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

/* 27 words, 0 differing. */
/* The 0x312 spark's own step function: hand the renderable to func_800478B8,
   and when its +0x14 reports 0x6000 mark the owning actor retired and set the
   same bit in D_800814A0. No xsibs twin at ratio 0.40; written from the
   listing. D_800814A0 is the PLAIN scalar here, not ovl_main_8008A288's
   `D_800814A0_a[0]` array alias -- the `lui`/`lw %lo` pair is what the
   non-array spelling emits. 27 words, 0 differing, first compile. */
void func_b31f_80170D60(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* One frame of a fading burst, and a leaf: func_800D5CA0
   (csrc/ovl_dungeon_800D50FC.c) byte for byte, with nothing relinked -- the
   only symbol it touches outside itself is D_800814A0, which is resident.
   Integrate the mover, halve each of its three velocities, scale the three
   colour bytes by the remaining life over the original life, copy the colour
   word forward, and when the life runs out raise the node's and the engine's
   0x8000 flags. The three `srl 31'/`addu'/`sra 1' triples are the signed
   `/= 2'; `>>= 1' would drop the correction. 75 words, 0 differing, first
   compile. func_b15_80170F2C below installs it as its step function. */
void func_b31f_801718E4(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* 0x18 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_80171168. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A87C[] = {
    0x00,
    0xF5,
    0x00,
    0xF5,
    0x01,
    0xF5,
    0x0B,
    0xF5,
    0xF5,
    0xFD,
    0x08,
    0xF8,
    0x00,
    0xF2,
    0x08,
    0xF2,
    0xF8,
    0xF5,
    0xFD,
    0xEB,
    0xFF,
    0xF4,
    0xFA,
    0x08,
};

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_b31f_80171A10, func_b31f_801730AC. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b31f_8016A894[] = {
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0x01,
    0x00,
    0x00,
    0x00,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0xFF,
    0xFF,
    0x00,
    0x00,
    0xFF,
    0xFF,
    0x01,
    0x00,
    0xFF,
    0xFF,
};

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
