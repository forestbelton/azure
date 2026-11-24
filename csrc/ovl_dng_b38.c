#include "common.h"

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b38_80171F74(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern u8 D_80082E80[];
extern u16 D_80082EA4;
typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

extern Unk80083462 D_80083460;
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s16 func_800A0134(u8 *a, u8 *b);
s16 func_800A2CB8(u8 *a, u8 *b);
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *p, s32 b, s32 c);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);

extern s16 D_80083228;
extern u8 D_80174AD4[];
extern u8 D_80174AFC[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b38_8017182C(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b38_80171138();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AB464();

s32 func_800AC7EC();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

extern u8 D_80174AF4[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, UnkVec *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b38_80173D38(u8 *p, u8 *b, u8 *c, u8 *d);

extern u8 D_80175140[];

void func_b38_8017182C(u8 *p, void *b, void *c, u8 *d);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, u8 *t, void (*h)());
s16 func_800A1C58(u8 *p);
s16 func_8009FB34(u16 x, u16 y);
void func_b38_80171670();
void func_b38_801740F4(u8 *, UnkVec *, u8 *, u8 *);
void func_b38_80172138(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_80174ADC[];
extern u8 D_80174AEC[];
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_8017087C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80170A70);

/* The banked room script's IDLE handler -- the body parked at the actor's
   +0x8C by func_b38_8017087C above, and the one the switch below re-installs
   through func_800AAF00. OWNS A JUMP TABLE: jtbl_b38_80170808, twelve entries
   for the +0x46 state 1..12, with 4, 10 and 11 carrying the default label
   rather than arms of their own. 334 words, 0 differing.

   THE TWIN IS THE LANDED 337-word func_b01_801714D4 (csrc/ovl_dng_b01.c).
   Its frame is identical, `addiu $sp, $sp, -0x38' with $s0..$s3 and $ra
   saved, so its allocation facts are the transferable ones, and ALL THREE of
   its named facts hold here. Re-measured on this body, with the rejected
   spelling's price beside each (aligned differences):
     the func_b38_8017182C tail `goto'-ed to one shared label instead of
       written out at all THREE sites        333 words /  27 aligned
     one table local shared across the two dress sites rather than one each
                                             335 words /  10 aligned
     arm B's D_800814A8 NOT held in `w', inline as
       `((u8 *)D_800814A8)[0x9A]'            335 words /  16 aligned
     the d + 0x2A store moved BELOW the w[0x9A] test
                                             335 words /  14 aligned
   The last two are b01's fact (3) and reproduce ITS prices almost exactly --
   b01 recorded 16 and 13 -- which is the clearest evidence in this unit that
   a same-frame twin's allocation facts transfer at their recorded cost.

   ONE DEVICE, AND IT IS NEEDED: `*(s32 *)(d + 0x1C) |= 0x40000' is wrapped
   in a `do { } while (0)'. Without it the body is 334 words and 99 aligned
   differences, EVERY ONE of them `p', `c' and `d' rotated through
   $s0/$s1/$s2 -- it compiles to b01's register assignment, not this ROM's.
   The arithmetic, from `tools/cdkgcc -da' on this file: global.c ranks on
   floor_log2(n_refs) * n_refs / live_length, and plain this is `c' 5*33/468
   = 0.3526, `d' 5*36/512 = 0.3516 and the D_80082E80 pointer in the
   d[0x6D] <= 0 tail 2*5/28 = 0.3571. That pointer conflicts with `c' and
   `d' but NOT with `p', so whoever is allocated after it shares its
   register: at 0.3571 > 0.3526 > 0.3516 it takes $s0, `c' takes $s1, `d'
   $s2 and `p' shares $s0. The ROM wants `d' FIRST. flow.c:2122 adds
   loop_depth references per mention and `if (1)' is not a loop, so the
   `do'-wrapper takes `d' to 38 refs and 5*38/512 = 0.3711, which puts it
   above the pointer and rotates all three into place. Called MACRO RESIDUE
   rather than a device because the wrapped statement is one flag publish,
   the canonical macro body -- but THE TARGET DOES NOT CONSTRAIN THE SCOPE:
   wrapping `v = *(s32 *)(d + 0x1C)' in the tail instead, or wrapping the
   0x40000 set TOGETHER WITH the following `*(u16 *)(p + 0x98) &= ~8', both
   also measure 334 words / 0 differing. Wrapping the OTHER 0x1C update, the
   `&= ~0x40000' in the 0x200 arm, is 334 words / 7 aligned -- that one is
   inside a block that returns, so its references do not reach the race.

   Deltas from that twin, all read off the listing. The 0x1000 escape calls
   the bank-local func_b38_80171670 WITH NO ARGUMENTS AT ALL -- nothing writes
   $a0..$a3 before the `jal', so the four incoming registers reach it
   untouched, which is why there is a second, empty-paren declaration of it at
   the top of this file even though the definition below takes four
   parameters. The section-4 dress is a ONE-WAY test against D_80174AD4 where
   b01 has a two-way D_801740E0/D_801740E8 pair, so b01's `f' local and its
   measured delay-slot fact do not arise; its body zeroes p + 0xA0 and then
   HANDS THAT WORD BACK as func_80047784's third argument, where b01 passes a
   literal 0 and then sets c[5] / p + 0xA2 / p + 0x9E. The 0x80000 arm's
   six-word p + 0x92 / p + 0xA2 / p + 0x9E block is likewise one
   `p + 0xA0' zero here, ahead of the bank-local func_b38_80173D38. The
   switch gives case 9 an arm of its own (func_b38_801740F4) AHEAD of case 8
   (func_b38_80171F74 then func_b38_80172138), where b01 has a single 8/9/10
   arm, and func_800AAF00's fourth argument is the table D_80174AEC rather
   than b01's 0. Tables: D_80174AFC and D_80174AF4 in the two 0x200 arms,
   D_80174ADC for func_800AA6B4.

   THE CASE ARMS ARE EMITTED IN SOURCE ORDER, so `case 9' has to be written
   before `case 8': the ROM has func_b38_801740F4's arm at 0x801714C4 and
   func_b38_80171F74's at 0x801714E0. Declared the other way round it is
   334 words / 4 aligned, all four rows the two arms' `jal's and the branch
   between them.

   1 copy (dngfam.py). */
void func_b38_80171138(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b38_80171670();
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174AFC)
        {
            e = D_80174AF4;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174AFC)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_80174AF4) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            p[0x9A] = 0xE;
        }
        if (*(u8 **)(c + 0x2C) != D_80174AD4)
        {
            *(s16 *)(p + 0xA0) = 0;
            *(u8 **)(c + 0x2C) = D_80174AD4;
            func_80047784(c, D_80174AD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          *(s16 *)(p + 0xA0));
        }
        do
        {
            *(s32 *)(d + 0x1C) |= 0x40000;
        } while (0);
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174ADC) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(s16 *)(p + 0xA0) = 0;
            func_b38_80173D38(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4)
        {
            func_b38_8017182C(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b38_80172258(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b38_8017182C(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 9:
            func_b38_801740F4(p, b, c, d);
            return;
        case 8:
            if ((s16)func_b38_80171F74(p, b, c, d) != 0)
            {
                return;
            }
            func_b38_80172138(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
            case 12:
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_800AAF00(p, b, c, D_80174AEC, func_b38_80171138);
            return;
        default:
            func_b38_8017182C(p, b, c, d);
            return;
        }
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) == 0)
    {
        if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
        {
            if ((v & 0x430) == 0)
            {
                if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    *(s16 *)(d + 0x2A) =
                        func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
                }
            }
        }
    }
}

/* The banked room script's path-step advance, the twin of the landed
   110-word func_b57_80172510 (csrc/ovl_dng_b57.c): past the d + 0x71 /
   d + 0x8A step gates, dress the renderable from D_80174AD4, retire the
   current waypoint through func_8009A3D0, take the next pair out of d + 0x74
   / d + 0x7C, and re-aim with func_8009A21C and func_800A0818. 111 words,
   0 differing, ON THE FIRST COMPILE, and WITH NO NEW DECLARATIONS -- every
   symbol it needs, D_80174AD4 included, was already in this unit's
   declaration block from the bodies landed before this batch.

   The frame is identical to that twin's, `addiu $sp, $sp, -0x30' with
   $s0..$s4 and $ra saved, so its allocation facts are the transferable ones.
   It names none; its three facts are all SHAPE facts and all three hold
   here, since this copy has the same shape: no func_8009A66C arm so
   `p[0x9A] = 0xF' is unconditional, func_800A0818 called AFTER func_8009A21C
   with the `x'/`y' locals straight rather than through its phantom-zero
   device, and a dress with only the `!=' arm.

   Two deltas from it, both read off the listing: the table is D_80174AD4,
   and -- the one extra word against b57's 110 -- the dress arm also zeroes
   p + 0xA0 AFTER the func_80047784 call. Three spellings measured on this
   body, with both prices, since a first-compile landing proves nothing about
   which of its choices mattered:
     the p + 0xA0 zero hoisted ABOVE the func_80047784 call
                                              111 words /  2 aligned
     `x'/`y' reassigned from c + 0x24 / c + 0x25 before the func_8009A21C
       call instead of the two fresh `lbu's the listing has
                                              113 words / 55 aligned
     the two 0x2000 tests collapsed into one hoisted `g' local
                                              110 words / 33 aligned
   That last one is the fact to carry forward: the DUPLICATED test reads like
   a device and is not one -- writing it once costs a word and a third of the
   body, because the second test is what keeps $a2 set up inside the
   func_8009A21C argument block. 1 copy (dngfam.py). */
void func_b38_80171670(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80174AD4)
        {
            *(u8 **)(c + 0x2C) = D_80174AD4;
            func_80047784(c, D_80174AD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s16 *)(p + 0xA0) = 0;
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (u16 *)(p + 0x98));
        *(u32 *)(d + 0x1C) |= 0x40000000;
        p[0x9A] = 0xF;
        *(s32 *)(p + 0x8C) = 0;
        if ((D_80083462 & 0x80) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            n = d[0x71];
            if (n > 0)
            {
                *(s16 *)(p + 0x96) = 8 / n;
            }
        }
    }
}

/* The banked room script's walk step, the relinked twin of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c) with three extra
   pre-loop arms: the +0x400/+0x10 pair that re-aims through func_800A02AC or
   func_800A04F0, and the +0x2000/+0x20000 arm that compares the cursor
   against D_80082E80's. Everything after that is func_800D92C0's eight-step
   D_8006CD00 sweep. 466 words, 0 differing.

   THE SECOND PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED
   AND PASSED ON. func_b38_80171F74's call sets only $a0, $a2 and $a3, so its
   $a1 is this function's own incoming $a1, reaching it untouched; the
   parameter is there to carry it.

   func_800BCB04 HAS TO COME THROUGH THE s16 ALIAS, AND IT DECIDES THE WHOLE
   FUNCTION'S REGISTER ASSIGNMENT. `i = func_800BCB04h(...)' is a halfword
   assignment into `i' itself; with the landed `s32 func_800BCB04' and an
   `(s16)' cast the result needs a temporary instead, which takes `i's
   allocno from 17 references over 113 insns down to 14 over 107. The
   priority global.c's allocno_compare computes is
   floor_log2(refs)*refs/live_length, so 4*17/113 beats `d's 6*85/860 and `i'
   is allocated first and takes $s1, while 3*14/107 does not and `i', `d' and
   `c' come out rotated: 466 words, 126 differing.

   AND THE LOOP'S HEADING VALUE SHARES `e' WITH THE TWO ACTOR LOOKUPS. In its
   own local of any type or spelling it is 3 differing -- the `lh' of d + 0x2A
   lands in $a0 where the target has $v1 -- because global.c's
   expand_preferences gives it `s's hard-register preference (it dies in the
   insn that sets `s') and `s' prefers $a0, where the `sll'/`sra' feeding
   func_8009A8C0's first argument is local-allocated. Sharing `e', whose other
   live range is in the pre-loop arms, gives the allocno the hard-register
   conflicts that put $a0 out of reach, and find_reg keeps $v1.

   Measured, on this body, by deletion: writing the 0x20000 arm's
   D_80082E80 through a local is 465 words / 23 differing, and the resident
   twin's own note explains why the D_8006CCE8 lookup must stay inline. The
   first gate with the func_800A9A0C call as the *taken* arm rather than the
   if-body is 11 differing. The tail's D_80083460 through `g' instead of its
   own `g2' is 3 differing. The `(s16)' casts on the three s32-returning
   callees are load-bearing -- func_8009A8C0 2 words, func_b38_80171F74 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b38_8017182C(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 t1;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b38_80171F74(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            /* DEVICE: the constant in a local, and the read-modify-write
               wrapped in a do-while(0). Worth 4 differing: without it the
               function still builds at 466 words but the loaded word and
               the `lui 0x8000' swap $v0 and $v1. Plain
               `*(s32 *)(d + 0x14) |= 0x80000000;' is 4 differing, as are
               `= *(s32 *)(d + 0x14) | 0x80000000', `|= (s32)0x80000000' and
               hoisting the loaded value into a local of its own. No
               uninitialised read, so nothing warns under -Wall. Same
               construct, same place, as the landed func_800D92C0's `cc2'. */
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A8C0(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                              (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                   (u16 *)(p + 0x98));
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - tbl[i];
        }
        else
        {
            s = e + tbl[i];
        }
        if ((s16)func_8009A8C0((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            /* The table address is a local that the index add writes back
               into, exactly as the landed func_800D92C0 needs it: the `%lo'
               insn's destination then has reg_n_sets != 1. Written inline as
               `*(u8 *)(kk + (s32)D_8006CCD8)' it is 8 differing. The second
               lookup must stay inline -- cse knows D_8006CCE8 is
               D_8006CCD8 + 0x10 and folds its `%hi'/`%lo' away. */
            t1 = (s32)D_8006CCD8;
            t1 += kk;
            *(u8 *)(c + 0x24) += *(u8 *)t1;
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                /* DEVICE: the return wrapped in a do-while(0). Worth 4
                   differing. A plain `return;' here leaves reorg filling
                   only one of the three branches into the `i++' join with a
                   copy of `addiu $v0, $s1, 0x1', where the target has all
                   three, and puts a `nop' in the third branch's delay slot
                   instead. No uninitialised read, so nothing warns under
                   -Wall. Same construct, same place, as the landed
                   func_800D92C0. */
                do
                {
                    return;
                } while (0);
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = func_800BCB04h((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* The banked room script's "same slot" branch, the relinked twin of the
   resident overlay's func_800CA788: with the D_80083460 gates clear, ask
   func_800A04F0 what sits under the cursor, and if the two headings are
   within 0x40 of each other put the actor into state 0x11, swap in the
   D_80174AD4 facing table and re-aim through func_8009C93C.

   TWO THINGS THAT ARE NOT VISIBLE IN THE LOGIC.

   THE FRAME IS 0x40 WHERE THE CALLS NEED 0x38, SO THE ORIGINAL DECLARED AN
   UNREFERENCED 8-BYTE AGGREGATE LOCAL. The widest call here is
   func_8009C93C's five arguments, which is a 0x18 outgoing-argument area and
   a 0x38 frame -- which is exactly what func_800CA788 gets for the same call
   set. gcc allocates the slot for an aggregate even when flow deletes every
   reference to it, so the extra 8 bytes are a leftover declaration. Only the
   size is witnessed, never the type. No local at all, and `s32 unused;`
   instead of the array, both give 113 words and 16 differing -- every
   $sp-relative offset and both ends of the frame.

   AND THE SHARED `-1` EXIT BELONGS TO THE *LAST* GATE, NOT THE FIRST ONE THAT
   REACHES IT. All four failure exits are one `goto fail`, and the label sits
   in the second func_800A2B5C test, which is where the merged
   `j .Lb03_80171F3C` / `addiu $v0, $zero, -0x1` block is emitted -- as the
   fall-through of that test's `beqz`, with reorg then copying the constant
   into the other three branches' delay slots. func_800CA788's note puts its
   own label in its SECOND test; that is its layout, not a rule.

   | spelling                                  | words | differing |
   | four plain `return -1;`, no label         |  113  |    35     |
   | label in the second D_80083460 gate       |  113  |    36     |
   | label in the `& 0x8` arm                  |  112  |    73     |
   | label in the second func_800A2B5C test    |  113  |     0     |

   func_800A04F0 is reached through a unit-local alias because the landed
   definition in csrc/ovl_dungeon_8009F644.c is `void` while this call site
   uses $v0, and its fourth parameter has to be declared `s16` here rather
   than the definition's `u16`: `u16` makes the `lh` of d + 0x2A an
   `lh`/`andi $a3, $a3, 0xFFFF` pair -- 113 words, 1 differing.

   `d[0x6D] -= 1;` is also BEFORE the func_8009C93C call, not after it as in
   func_800CA788: after the call is 113 words, 8 differing, because the store
   can no longer sink past the `lh` of d + 0x2A. 113 words, 0 differing. */
s32 func_b38_80171F74(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    u8 *e;
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
    d[0x85] = 0;
    e = D_80174AD4;
    *(u8 **)(c + 0x2C) = e;
    func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    return 1;
}

/* The unconditional "close on the slot" entry: the gate-free sibling of the
   landed func_b04_80172270 immediately above, with that function's whole
   func_800A04F0 / func_800A2CB8 / func_800A0134 cursor interrogation removed.
   What is left is its tail verbatim -- the two func_800A2B5C tests around the
   func_800C7930 push, state 0x11, the re-dress and the func_8009C93C re-aim.
   72 words, 0 differing, first compile.

   THREE DELTAS FROM func_b04_80172270, all measured off this listing and not
   assumed from it:

   THE FACING TABLE IS READ FROM THE RENDERABLE, NOT WRITTEN TO IT. The
   target has `lw $v1, 0x2C($s2)' and indexes that, where func_b04_80172270
   stores D_80175140 into c + 0x2C first and indexes its own local -- so the
   argument is spelled `(*(u8 **)(c + 0x2C))[...]' with no local and no store.

   THE FIRST GATE IS THE SCALAR D_80083462, NOT D_80083460 + 0x2. The target
   loads it with a bare `lui'/`lhu %lo(D_80083462)' and never builds a base
   register, which is the spelling the landed func_b04_80172554 below already
   uses in this unit; func_b04_80172270's `g = (u8 *)&D_80083460' idiom would
   add the `addiu'.

   d + 0x84 AND d + 0x85 COME AFTER THE func_8009C93C CALL, not before it as
   in func_b04_80172270, and p + 0x98 picks up 0x8 between them and the call.
   `d[0x6D] -= 1' stays BEFORE the call, as it is there.

   There is NO unreferenced aggregate local here: the frame is 0x30, exactly
   the 0x18 outgoing-argument area func_8009C93C's five arguments need plus
   the five saved registers, where func_b04_80172270's is 0x40. */
void func_b38_80172138(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    p[0x9A] = 0x11;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9B] = 0;
    func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    *(u16 *)(p + 0x98) |= 0x8;
    d[0x84] = 0x7C;
    d[0x85] = 0;
}

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
s32 func_b38_80172258(u8 *a, void *b, void *c, s32 d)
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
        func_b38_8017182C(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b38_8017182C(p, b, c, p);
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
   func_b38_80171138. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b38_8017239C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b38_80171138;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = -0x20;
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
void func_b38_80172430(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b38_80171138;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80172494);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80172A40);

/* 0x14 bytes of this bank's own rodata, between its jump tables,
   read by func_b38_801740F4. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b38_80170854[] = {
    0x81,
    0x40,
    0x82,
    0x92,
    0x82,
    0x8F,
    0x82,
    0x81,
    0x82,
    0x92,
    0x82,
    0x85,
    0x82,
    0x84,
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0xc bytes of this bank's own rodata, between its jump tables,
   read by func_b38_80174260. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b38_80170868[] = {
    0x00,
    0x10,
    0x00,
    0x0C,
    0x00,
    0x10,
    0x00,
    0x18,
    0x00,
    0x14,
    0x00,
    0x00,
};

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_b38_80174260. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b38_80170874[] = {
    0x00,
    0x10,
    0x00,
    0x18,
    0x00,
    0x10,
    0x00,
    0x08,
};

/* The banked room script's three-state glide, switched on p + 0x9B: state 0
   seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair, state 1
   decays it by the same pair shifted 15, state 2 eases onto the tile centre,
   re-dresses from D_80174AD4 and installs func_b38_80171138 above.
   229 words, 0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED 244-word func_b17_80173244 (csrc/ovl_dng_b17.c),
   and func_b04_80173354 is the same 244 words. The frame is identical,
   `addiu $sp, $sp, -0x28' with $s0..$s4 and $ra saved, so b17's allocation
   facts are the transferable ones, and ALL THREE of its named facts hold.
   Re-measured here, with the rejected spelling's price beside each:
     the func_800AAA54 exit reached by one `goto stop' instead of written
       out at BOTH sites                     223 words /  14 aligned
     the paired flag work as one mask, `&= ~0x8040000' and `|= 0x8040000'
       in place of two statements each        225 words /  12 aligned
     case 2's eases WITHOUT the embedded `(u = ...)' assignment, hoisted
       into preceding statements              229 words /   7 aligned
   b17 priced the first at 239 words / 15 aligned and the second at "two
   words short"; here it is four words and 12 rows, because this copy's
   state 2 has the `|=' pair as well.

   Deltas from b17, all read off the listing. State 0 has NO
   `(c + 0x14) & 0x8000' refusal arm at all -- that absence is most of the 15
   words -- and seeds b->unk_14 with 0x20000 where b17 uses 0x40000. State
   1's decay shift is 15, not b17's 14, and its waiting arm sets p + 0x96 to
   5 rather than 8. State 2 opens with `b->unk_14 = 0xFFFD0000' (b17:
   0xFFFE0000, and the `lui' sits in the dispatch `beq's delay slot either
   way), drops b17's `>> 1' on both eases AND the `+= >> 1' pair that
   follows them, and drops `*(s32 *)(p + 0x90) = 0'.

   THE SHARED EXIT STORES b + 0x14 TWICE ON STATE 0'S PATH, and that is a
   real word: `b->unk_14 = 0' ahead of the block whose last store is
   `b->unk_14 = 0xFFFD0000'. gcc 2.7.2 does not eliminate a dead MEMORY
   store, so both survive, and the first one is what fills the `j's delay
   slot into the cross-joined tail. Without it the body is 228 words /
   5 aligned. State 1's path has no such store, which is why the two paths
   cross-join only from `b->unk_10 = 0' down.

   AND ONE MEASURED FREE, so it is NOT why this lands: state 1's
   `*(s16 *)(p + 0x96) = 5' before `p[0x9B] += 1'. In the other order it is
   also 229 words / 0 differing -- the target's `sh'/`sb' pair is reorg
   filling the `j's delay slot and does not constrain the source order.
   1 copy (dngfam.py). */
void func_b38_80172F00(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 k;

    k = (*(u16 *)(d + 0x6A) >> 9) & 7;
    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            b->unk_14 = 0xFFFD0000;
            func_800AAA54(p, b, c, D_80174ADC);
            return;
        }
        b->unk_0C = D_8006CCD8[k] << 18;
        b->unk_10 = D_8006CCE8[k] << 18;
        b->unk_14 = 0x20000;
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        p[0x9B] += 1;
        *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        return;
    case 1:
        b->unk_0C -= D_8006CCD8[k] << 15;
        b->unk_10 -= D_8006CCE8[k] << 15;
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
            p[0x9B] = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            b->unk_14 = 0xFFFD0000;
            func_800AAA54(p, b, c, D_80174ADC);
        }
        else
        {
            *(s16 *)(p + 0x96) = 5;
            p[0x9B] += 1;
        }
        return;
    case 2:
        b->unk_14 = 0xFFFD0000;
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_0C = ((c[0x24] << 6) - (u = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15;
            b->unk_10 = ((c[0x25] << 6) - (w = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15;
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= ~8;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u8 **)(c + 0x2C) = D_80174AD4;
        func_80047784(c, D_80174AD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b38_80171138;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80173294);

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b38_80173504(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b38_80173524(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b38_80173544(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_8017357C);

/* The banked room script's three-state tail, state index at +0x9B, and the
   near-twin of the landed func_b03_801736F4 (csrc/ovl_dng_b03.c), itself the
   banked copy of the resident overlay's func_800DA398: state 0 re-dresses the
   renderable from the bank-local D_80174AFC facing table, state 1 is the long
   refusal chain with the D_80174AF4 dress/flag/bump tail written out twice,
   state 2 hands the actor back to func_b38_80171138. 208 words, 0 differing.

   THE DELTA FROM func_b03_801736F4 IS ONE WORD: the +0x80000 arm clears the
   slot's halfword at +0xA8 BETWEEN the func_800AA888 call and the
   func_b38_80173D38 hand-over. Written before the call instead it is 19
   aligned differences.

   `p[0x9B] += 1' IS WRITTEN OUT AT ALL THREE REACHING SITES, NOT REACHED BY
   `goto'. jump2 cross-jumps the three copies back into the one block the
   target has, so the length is the same either way -- but the three extra
   references on `p' are what win it $s2. With the single `goto bump' label
   func_b03_801736F4 uses, this body is 208 words with 17 ALIGNED DIFFERENCES,
   every one a register: `p' takes $s3 and `b' takes $s2, the other way round
   from the target, exactly the allocno_compare loss func_b03_80173358's
   header measures from the other side. The extra +0xA8 store on `p' is not
   enough on its own.

   Measured and rejected: `b' declared `void *' with `(UnkVec *)' casts at the
   three calls -- 208 words, 16 differing, so the parameter's type is not what
   decides the swap.

   The rest is func_b03_801736F4's shape verbatim, including its four
   SEPARATE `&D_80083460' pointer locals (one per arm, with the gate's shared
   with the state 1 tail) and the facing tables named directly at all three
   dress sites rather than held in a local.
   THIS COPY'S OWN CONSTANT: 0xa8 -> 0xa0.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b38_801739F8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174AFC;
        func_80047784(c, D_80174AFC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174AF4;
            func_80047784(c, D_80174AF4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            p[0x9B] += 1;
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
            *(s16 *)(p + 0xa0) = 0;
            func_b38_80173D38(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174AF4;
        func_80047784(c, D_80174AF4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x208;
        *(u32 *)(p + 0x8C) = (u32)func_b38_80171138;
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
   call emits the bare `jal` either way); hoisting D_80174AD4 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b38_80173D38(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b38_80171138;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80174AFC && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80174AD4;
        func_80047784(c, D_80174AD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b38_80173E00(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b38_80171138;
        }
    }
}

/* 14 words, 0 differing. */
void func_b38_80173E64(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}

/* 14 words, 0 differing. */
void func_b38_80173E9C(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}

/* 14 words, 0 differing. */
void func_b38_80173ED4(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80173F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_801740F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80174260);

/* A frameless leaf timer on one record: bias +0x58 while the +0x4C phase is
   8, count the +0x48 ticks down, and on expiry reload +0x48 (0xA out of
   phase 7, otherwise 1), republish +0x58 as the 0x80 flag of "was zero" and
   advance the phase; at phase 0xF raise 0x8000 in the halfword BELOW the
   record and in D_800814A0. 42 words, 0 differing, ON THE FIRST COMPILE.

   NO TWIN, and none was needed. dngtwin found no carved sibling at 0.70 or
   better; the callee set is EMPTY -- there is no `jal' in the listing at all,
   so the brief's intersect-the-callees recipe has nothing to intersect -- and
   the distinctive-symbol grep on `D_800814A0 |= 0x8000' matches 20 landed
   units because it is the global "redraw" bit. Nothing in this unit or in
   asm/nonmatchings/ovl_dng_b38 references func_b38_801745E0 either, by `jal'
   or by address, so it is an orphan the carve left behind and its single
   parameter is all the listing witnesses.

   FOUR SPELLINGS MEASURED AND ALL FOUR FREE, so this body constrains almost
   nothing and a later copy should not preserve any of them: the predecrement
   test split into a store and a separate compare, the phase-7 reload as an
   `if'/`else' instead of a ternary, the +0x58 republish as `? 0x80 : 0'
   instead of `== 0) << 7', and the reload value through a local -- all
   42 words / 0 differing. The reason is that there is no register pressure
   to resolve: the body is frameless, saves nothing, and every value dies in
   the instruction after the one that sets it. 1 copy (dngfam.py). */
void func_b38_801745E0(u8 *p)
{
    if (*(s16 *)(p + 0x4C) == 8)
    {
        *(s32 *)(p + 0x58) += 8;
    }
    if (--*(s16 *)(p + 0x48) <= 0)
    {
        *(s16 *)(p + 0x48) = (*(s16 *)(p + 0x4C) == 7) ? 0xA : 1;
        *(s32 *)(p + 0x58) = (*(s32 *)(p + 0x58) == 0) << 7;
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
    }
    if (*(s16 *)(p + 0x4C) == 0xF)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b38", func_b38_80174688);

/* 14 words, 0 differing. */
void func_b38_80174A5C(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}

/* 14 words, 0 differing. */
void func_b38_80174A94(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
    }
}
