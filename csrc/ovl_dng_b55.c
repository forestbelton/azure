#include "common.h"

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b55_80171BE0(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AC82C();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b55_80171514();

s32 func_800AB778();

s32 func_800AD058();

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80170898);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80170B50);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80170D44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80170E18);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80170F84);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_8017112C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80171514);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_801719C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80171BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80172328);

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b55_80172330(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b55_80171BE0(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b55_80171BE0(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80172474);

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
void func_b55_80172508(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b55_80171514;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_8017256C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80172B4C);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b55_80172E20(void)
{
    func_800AD058();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80172E40);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b55_80172E6C(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b55_80172E8C(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b55_80171514;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80172EC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173364);

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
void func_b55_801736B8(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b55_80171514;
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
void func_b55_8017371C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b55_80171514;
        }
    }
}

/* 14 words, 0 differing. */
void func_b55_80173780(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b55_80171514;
    }
}

/* 14 words, 0 differing. */
void func_b55_801737B8(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b55_80171514;
    }
}

/* 14 words, 0 differing. */
void func_b55_801737F0(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b55_80171514;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173828);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173860);

/* 14 words, 0 differing. */
void func_b55_801738B8(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b55_80171514;
    }
}

void func_b55_801738F0(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_801738F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173934);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_8017394C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173A48);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173B74);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173BC0);

void func_b55_80173BE8(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173BF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173C30);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173C60);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b55", func_b55_80173D6C);
