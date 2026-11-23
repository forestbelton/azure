#include "common.h"

extern s16 D_80083228;
extern u8 D_800E2348[];
extern u8 D_800E23A8[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b25_801726EC(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b25_80171F1C();

s32 func_800AB778();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

s32 func_800AB538();

s32 func_8009A180(u8 *a, u8 *b);
extern u8 D_800E23B0[];
s32 func_800A2C34(u8 *);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b25_8017531C(u8 *p, u8 *b, u8 *c, u8 *d);

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E2390[];
void func_800A2B04(UnkVec *, s32, s32);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD4D0(u8 *);

u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170A44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170BB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170D34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171010);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801711DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171384);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_8017145C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_8017165C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171780);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171964);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171F1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801724B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801726EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80172E0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80172E80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801730A4);

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
s32 func_b25_801731DC(u8 *a, void *b, void *c, s32 d)
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
        func_b25_801726EC(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b25_801726EC(p, b, c, p);
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

/* The banked room script's two-gate installer, the relinked twin of the
   resident overlay's func_800D9884 (csrc/ovl_dungeon_800D50FC.c): the same
   func_800AB1C0 gate, the same func_800AD594 / func_800A4ACC pair, the same
   `blez'-to-the-epilogue inner gate, and the same D_80083462 & 0x80 tail.
   Two deltas, both read off the listing: this copy does NOT clear p + 0x90
   after installing the handler, and its flag arm writes -0x20 to p + 0x92
   where the resident writes 0. The handler is the bank-local
   func_b25_80171F1C. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b25_80173320(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b25_80171F1C;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = -0x20;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801733B4);

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
void func_b25_80173794(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801737F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80173B08);

/* The banked room script's three-state glide, switching on p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it, state 2 steers onto the tile centre and installs
   func_b25_80171F1C. The resident overlay's func_800D9C7C
   (csrc/ovl_dungeon_800D50FC.c) is the same machine -- the same three cases
   FALLING THROUGH into one another, the same `stop:' label inside case 1
   that case 0's `else' reaches by `goto', the same func_800AAA54 call -- and
   func_800D991C in the same unit supplies the divide form of case 2. Both
   of those comments carry the measured spelling facts this body relies on:
   the three-statement numerator `u = c[0x24] << 6; w = b2 - 0x20; u -= w;'
   (nine other spellings 1 to 4 differing on func_800D991C), the second
   numerator left two-statement (1 word and 145 differing when split), and
   `if (x > 0) x--;' in case 1 against `if (--x > 0)' in case 2, which
   func_800D9C7C measures as not interchangeable.

   Four deltas from func_800D9C7C, all read off the listing: the
   D_8006CCD8 index is NOT a local here -- state 0 recomputes
   `(*(u16 *)(d + 0x6A) >> 9) & 7' for each of the two tables and state 1
   never uses it, so there is no $s4 to hold it; the seeds are negated and
   scaled by 15 (`negu' then `sll 15') rather than shifted by 18; state 1
   decays by a quarter (`b->unk_0C -= b->unk_0C / 4', the
   `bgez'/`addiu 3'/`sra 2' triple) where func_800D9C7C subtracts
   `D_8006CCD8[k] << 14'; and func_800AAA54's fourth argument is the
   bank-local D_800E2390 rather than 0. 181 words, 0 differing, on the first
   compile. */
void func_b25_80174238(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) -= 1;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_800E2390);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 15) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 15) / *(s16 *)(p + 0x96);
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_8017450C. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_8017088C[] = {
    0x10,
    0x00,
    0x00,
    0x00,
    0x0D,
    0x00,
    0x0D,
    0x00,
    0x00,
    0x00,
    0x10,
    0x00,
    0xF3,
    0xFF,
    0x0D,
    0x00,
    0xF0,
    0xFF,
    0x00,
    0x00,
    0xF3,
    0xFF,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0xF0,
    0xFF,
    0x0D,
    0x00,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_8017450C);

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile. */
void func_b25_80174B80(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b25_80174BAC(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b25_80174BCC(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_800E23A8 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b25_80171F1C. 241 words,
   0 differing, on the first compile.

   THE TWIN IS func_b03_80173358 (csrc/ovl_dng_b03.c), not the 213-word
   func_b01_80173AAC / func_b16_80173114 pair: like func_b03_80173358 this
   copy opens state 1 with a func_80042900 test rather than the d[0x25] one,
   carries the func_800AA79C arm behind the g + 0x2 0x2008 gate, closes state
   1 with the D_80082E80 / func_8009FD40 / func_800A6D30 window and the second
   func_80042900 test, and writes the dress/flag/bump tail out TWICE -- once
   in the first test's `== 0' arm and once after `again:' -- which gcc
   cross-jumps into the single block at the target's .Lb02_80174134. Read that
   comment for the mechanism: `&D_80083460' as separate pointer locals per
   arm, and the facing table named directly at every site.

   Four deltas from that twin, all read off the listing. The tail's flag
   update is `*(s32 *)(d + 0x1C) |= 0x40000' and it sits BEFORE the c + 0x14
   0x8000 gate, unconditionally, where func_b03_80173358 has `&= ~0x200'
   inside the gate -- that is func_b01_80173AAC's constant in
   func_b03_80173358's position. The 0x80000 arm has the same six-word block
   func_b01_80173AAC has between func_800AA888 and the bank-local tail call,
   p + 0x92 reduced by p + 0xA2 as halfwords then p + 0xA2 and p + 0x9E
   cleared, and the tail call is this unit's already-landed func_b25_8017531C.
   State 2 changes no flag word at all. And in the tail the two bumps are in
   the opposite order to the twin's -- `p[0x9B] += 1' first, then
   `*(u16 *)(k + 0xA) += 1' -- which is the order the `sb 0x9B' / `lhu 0xA'
   pair appears in at .Lb02_80174194. */
void func_b25_80174C04(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23A8;
        func_80047784(c, D_800E23A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_800E23B0;
            func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
                return;
            }
            p[0x9B] += 1;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
            return;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C(p, b, c, d);
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b25_8017531C(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23B0;
        func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
            return;
        }
        p[0x9B] += 1;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_800E23A8 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b25_80171F1C. The landed
   func_b05_80173BEC (csrc/ovl_dng_b05.c) is the twin -- like this copy it
   opens state 1 with the same d[0x25] test that closes it, so the
   dress/flag/bump tail is written out twice, and it carries the same
   `*(s32 *)(d + 0x1C) |= 0x40000' at both dress sites. Read its comment for
   the mechanism: the facing table named directly at all three sites rather
   than held in a `u8 *' local (6 differing there), `&D_80083460' as separate
   pointer locals per arm (160 differing shared there), and the `p[0x9B] += 1'
   bump as a `goto' label inside the switch (177 differing after the switch
   there).

   Two deltas from that twin, both read off the listing: state 2 clears only
   bit 3 of d + 0x1C (`addiu $v1, $zero, -0x9') where func_b05_80173BEC
   clears 0x208, and the 0x80000 arm has a six-word block between
   func_800AA888 and the bank-local func_b25_8017531C -- p + 0x92 reduced by
   p + 0xA2 as halfwords (both reads `lhu', so both are unsigned), then
   p + 0xA2 and p + 0x9E cleared. p + 0x92 is READ BEFORE p + 0xA2 IS
   CLEARED in the listing, which is the order the three statements are
   written in. 213 words, 0 differing, on the first compile; the six-word
   block and the `~8' account for the 213 against the twin's 207. */
void func_b25_80174FC8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = D_800E23A8;
        func_80047784(c, D_800E23A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_800E23B0;
            func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            goto bump;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b25_8017531C(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23B0;
        func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~8;
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_800E2348 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b25_8017531C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_800E23A8 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_800E2348;
        func_80047784(c, D_800E2348[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b25_801753E4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
}

/* 14 words, 0 differing. */
void func_b25_80175448(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_80175480(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_801754B8(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801754F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175594);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175BE0);

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170940[] = {
    0x82,
    0x61,
    0x82,
    0x95,
    0x82,
    0x94,
    0x81,
    0x44,
    0x81,
    0x44,
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0x38 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170950[] = {
    0x82,
    0x68,
    0x82,
    0x94,
    0x81,
    0x66,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x8D,
    0x82,
    0x90,
    0x82,
    0x8F,
    0x82,
    0x93,
    0x82,
    0x93,
    0x82,
    0x89,
    0x82,
    0x82,
    0x82,
    0x8C,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x94,
    0x82,
    0x8F,
    0x81,
    0x40,
    0x82,
    0x8D,
    0x82,
    0x81,
    0x82,
    0x8B,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x94,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0xc bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170988[] = {
    0x82,
    0x97,
    0x82,
    0x89,
    0x82,
    0x94,
    0x82,
    0x88,
    0x81,
    0x40,
    0x00,
    0x00,
};

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170994[] = {
    0x81,
    0x66,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x83,
    0x82,
    0x95,
    0x82,
    0x92,
    0x82,
    0x92,
    0x82,
    0x85,
    0x82,
    0x8E,
    0x82,
    0x94,
    0x81,
    0x40,
    0x82,
    0x8C,
    0x82,
    0x85,
    0x82,
    0x96,
    0x82,
    0x85,
    0x82,
    0x8C,
    0x81,
    0x44,
    0x00,
    0x00,
};

/* 0x38 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_801709B8[] = {
    0x82,
    0x6D,
    0x82,
    0x8F,
    0x81,
    0x40,
    0x82,
    0x8D,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x86,
    0x82,
    0x8C,
    0x82,
    0x8F,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x81,
    0x82,
    0x82,
    0x82,
    0x8F,
    0x82,
    0x96,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x99,
    0x82,
    0x8F,
    0x82,
    0x95,
    0x81,
    0x49,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175CD8);

/* 14 words, 0 differing. */
void func_b25_801765DC(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_80176614(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}
