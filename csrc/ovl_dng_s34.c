#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_8003FD64(s32, void *);
s32 func_800A3820(s16);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A56E0(s32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800BCB04(s32, s32, s16);
s32 func_800C8900(u8 *, s16, s16);
void func_s34_80024538(u8 *p, UnkVec *b, u8 *c);
void func_s34_80024684(u8 *p);
s32 func_s34_80024714(u8 *p);
extern u8 D_80045340[];
extern u8 D_80083498[];
extern s32 D_8008346C;
extern u32 D_800814A0;
extern u8 D_800DEAE0[];
extern u8 D_800E3D68;

/* The bank's six-state cutscene director, a JUMP TABLE on *(s16 *)(p + 0xA)
   over cases 0..5. THIS FUNCTION OWNS jtbl_s34_80024008 (6 words = 6 cases +
   0 pad, dispatch bound 6, 6 distinct targets, arms written ascending so the
   entries come out in the listing's address order). The range check's `beqz'
   lands on the EPILOGUE, which is not one of the six entries, so there is no
   `default:'. ARM ORDER MEASURED, not assumed: a standalone `tools/cdkgcc -S'
   emits the six entries as $L3/$L7/$L22/$L26/$L30/$L32 at strictly ascending
   text positions, matching the listing's ascending 0x80024090..0x800244BC.

   State 0 waits for 0x80 in the record at p + 0x4, asks func_800A05A4 for the
   escort actor at the party's tile and parks it in s + 0x60, copies that
   actor's (or, when there is none, the caller's own record's) +0x24/+0x25
   facing pair into s + 0x72/+0x73, publishes the party record's integer
   position into b + 0x2/+0x6/+0xA, sets the 8-frame timer and then aims the
   three velocity words at b + 0xC/+0x10/+0x14 by dividing each leg by that
   timer, with the z leg coming out of func_800BCB04. State 1 advances the
   position by the velocity, then spawns the four 0x312 corner nodes (each
   offset +-0x10 in x and y off `i >> 1' and `i & 1', dressed from D_800DEAE0
   in 0xC0C0C0, stepped by func_s34_80024538) and, when s + 0x60 is set, the
   0x201 node whose step is func_s34_80024684 and whose renderer is
   func_s34_80024714. States 2..4 are timer waits; state 2 forks on s + 0x60.
   State 5 clears D_8008346C and tears the node down. 326 words, 0 differing.

   TWIN: csrc/ovl_dng_s68.c func_s68_80024020 (same address, same 6-entry
   table at 0x80024008, same `s = *(u8 **)(p + 0x0); o = *(u8 **)(s - 0x14);
   w = s - 0x20;' skeleton and the same case-5 teardown). dngtwin reports
   NOTHING at 0.70 for this stub; dngcallee found the family on a 7-of-14
   symbol overlap with a clean gap, which is what made this a first-day
   landing. Two of that twin's five recorded facts were re-measured here and
   both held; three are about locals it does not have.

   FOUR FACTS, priced from this body with aligndiff (words / aligned rows,
   R rows INCLUDED). The body went 332/134 -> 326/0 through these:

   * THE FOUR-CORNER OFFSET IS A `?:' WITH THE CONSTANT DISTRIBUTED INTO BOTH
     ARMS, AND IT IS WORTH 6 WORDS AND 83 ROWS. Written
     `*(s16 *)(X) = *(s16 *)((u8 *)b + 0x2) + (((i >> 1) != 0) ? -0x10 : 0x10)',
     fold-const.c:3736 distributes the `+ base' into both arms, so the base
     load AND the store's address are both evaluated BEFORE the branch and
     the two arms are bare `addiu's joining at one `sh'. Two full if/else
     stores instead reload `lw 0x8($s0)' and `lhu 0x2($s3)' in EACH arm:
     332 words / 123 rows. An if/else into a hoisted value local is 326/19 --
     correct length, but the `lw' lands in the join block instead of the
     dominator. This is the one place in this project where a `?:' is right;
     see docs/matching-levers.md.

   * THE CASE-0 ACTOR RECORD AND THE CASE-1 `n->unk_0C' ARE ONE LOCAL `q',
     worth 1 word and 5 rows -- the twin's fact, and it holds. But the
     func_800A05A4 RETURN needs a local of its own (`c'): sharing it with `q'
     costs a `move $a2, $v0' the target does not have. So the split runs
     BETWEEN the return and the record, not around them both.

   * THE TWO NODE PAYLOAD POINTERS ARE ALSO ONE LOCAL `e', worth 8 rows. The
     twin records the OPPOSITE for its own two payload pointers (23 rows to
     share); both of its arms were different cases, both of these are case 1,
     and the target spends $a3 on both. Re-price this per body.

   * ONE `s32' SCRATCH SHARED BY THREE ARMS COSTS 7 ROWS. Case 0's signed
     byte, case 3's 0x10/0xFF pick and the loop's 0xA/0x20 pick are three
     locals (`k', `u', `m') and not one: six sets on one pseudo put it in $a1
     where the target wants $v1 for each short-lived one.

   AND TWO SPELLINGS THAT ARE ABOUT TYPES, NOT ALLOCATION. `func_800A3820' is
   declared HERE as returning `s32' although its landed definition in
   csrc/ovl_dungeon_8009F644.c returns `u8'; with `u8' the caller truncates
   with `andi 0xff' and the target's `sll 16'/`sra 16' sign-extension cannot
   appear. Nothing declares it in any header, so the original unit declared it
   itself. And the two `(s8 *)(s + 0x72/0x73)' reads must be LIFTED INTO A
   LOCAL before being shifted: in the expression, combine fuses the
   sign-extension with the `<< 6' into `lbu'/`sll 24'/`sra 18' (one word more
   per site, 331/125), while a statement of its own leaves the extending load
   as the target's `lb'/`sll 6'. */
void func_s34_80024020(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *c;
    u8 *q;
    u8 *e;
    UnkNode *n;
    s32 i;
    s32 k;
    s32 m;
    s32 u;

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        c = (u8 *)func_800A05A4(s, *(u8 *)(o + 0x24), *(u8 *)(o + 0x25), *(s16 *)(s + 0x2A),
                                (s16)func_800A3820(0x22));
        *(u8 **)(s + 0x60) = c;
        if (c == NULL)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)(c - 0x14);
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
        *(s16 *)(p + 0x50) = 8;
        k = *(s8 *)(s + 0x72);
        *(s16 *)((u8 *)b + 0xE) = (k << 6) - (*(u16 *)((u8 *)b + 0x2) - 0x20);
        *(s32 *)((u8 *)b + 0xC) /= *(s16 *)(p + 0x50);
        k = *(s8 *)(s + 0x73);
        *(s16 *)((u8 *)b + 0x12) = (k << 6) - (*(u16 *)((u8 *)b + 0x6) - 0x20);
        *(s32 *)((u8 *)b + 0x10) /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x16) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) -
            *(u16 *)((u8 *)b + 0xA);
        *(s32 *)((u8 *)b + 0x14) /= *(s16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        *(s32 *)((u8 *)b + 0x0) += *(s32 *)((u8 *)b + 0xC);
        *(s32 *)((u8 *)b + 0x4) += *(s32 *)((u8 *)b + 0x10);
        *(s32 *)((u8 *)b + 0x8) += *(s32 *)((u8 *)b + 0x14);
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        for (i = 3; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)D_80045340);
                q = (u8 *)n->unk_0C;
                e = (u8 *)n + 0x20;
                n->unk_10 = (u32)func_s34_80024538;
                *(s16 *)((u8 *)n->unk_08 + 0x2) =
                    *(s16 *)((u8 *)b + 0x2) + (((i >> 1) != 0) ? -0x10 : 0x10);
                *(s16 *)((u8 *)n->unk_08 + 0x6) =
                    *(s16 *)((u8 *)b + 0x6) + (((i & 1) != 0) ? -0x10 : 0x10);
                *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)((u8 *)b + 0x8) + 0xFFF00000;
                *(s16 *)(q + 0x1E) = 0xC00;
                *(s16 *)(q + 0x10) = 0x60;
                *(u16 *)(q + 0x14) |= 0xC;
                *(u8 **)(q + 0x0) = D_800DEAE0;
                *(s16 *)(q + 0x1C) = 0;
                *(s32 *)(q + 0xC) = 0xC0C0C0;
                *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)(e + 0x0) = p;
                m = 0xA;
                if (*(s32 *)(s + 0x60) != 0)
                {
                    m = 0x20;
                }
                *(s16 *)(e + 0x48) = m;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
        if (*(s32 *)(s + 0x60) != 0)
        {
            n = func_8003FD64(0x201, D_80083498);
            if (n != NULL)
            {
                func_8004491C(n, (u32)func_s34_80024714);
                n->unk_10 = (u32)func_s34_80024684;
                e = (u8 *)n + 0x20;
                *(s16 *)(e + 0x4) = *(u16 *)((u8 *)b + 0x2);
                *(s16 *)(e + 0x6) = *(u16 *)((u8 *)b + 0x6);
                *(s16 *)(e + 0x8) = *(u16 *)((u8 *)b + 0xA);
                *(s16 *)(e + 0x12) = 1;
                *(s32 *)(e + 0xC) = -0x40;
                *(u8 **)((u8 *)n + 0x20) = p;
                *(s16 *)(e + 0x10) = 0x28;
            }
        }
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (*(s32 *)(s + 0x60) == 0)
        {
            *(s16 *)(p + 0x50) = 8;
            *(s16 *)(p + 0xA) = 5;
        }
        else
        {
            *(s16 *)(p + 0x50) = 0x14;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (*(s32 *)(s + 0x60) != 0 && func_8009D218(*(u8 **)(s + 0x60), 1, s) == 0 &&
            (*(s32 *)(*(u8 **)(s + 0x60) + 0x14) & 0x4) != 0)
        {
            u = 0x10;
            if (D_800E3D68 == 0xFF)
            {
                u = 0xFF;
            }
            func_800C8900(*(u8 **)(s + 0x60), u, 2);
        }
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 4;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            if (*(s16 *)(p + 0x50) > 0)
            {
                break;
            }
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The corner node's two-state step, registered as `n->unk_10' by
   func_s34_80024020's state 1 for each of the four 0x312 nodes: raise 0x8000
   in the view's +0x52, run the +0x48 countdown, re-send the view through
   func_800478B8 and drop its +0x4/+0x5 pair once the +0x14 flags carry one of
   0x6000. State 0 then swells the view by 0x200 in both +0x1C and +0x1E,
   snaps it back to 0xC00/0x1800 in white 0x808080 once +0x1E passes 0x1C00,
   and advances the state when the countdown runs out. State 1 is the standard
   teardown once the colour has faded under 9. 83 words, 0 differing, FIRST
   COMPILE. `b' is unused and is in the signature because
   func_s34_80024020's dress sites and func_s34_80024684 share this shape.

   NO TWIN at any ratio (dngtwin). The shape came from
   csrc/ovl_dng_b04.c func_b04_801743EC, whose state 0 is the same
   func_800478B8 / `& 0x6000' / +0x4+0x5 gate and whose state 1 is the same
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;' teardown.

   LANDED ON THE FIRST COMPILE, SO EVERY SPELLING IN IT WAS UNMEASURED WHEN
   WRITTEN. Two were then priced and they do not agree:

   * MEASURED AND FREE, so it is NOT the reason this lands: reading +0x1E back
     out of memory for the 0x1C00 test. `*(s16 *)(c + 0x1E) = *(u16 *)(c +
     0x1E) + 0x200; if (*(u16 *)(c + 0x1E) >= 0x1C00)' and an explicit
     `u = ...; *(s16 *)(c + 0x1E) = u; if ((u16)u >= 0x1C00)' both give
     83 words / 0 aligned differences. cse forwards the stored value and
     supplies the `andi $v0, $v0, 0xFFFF' either way.

   * MEASURED AND LOAD-BEARING: the teardown's two stores must be in this
     order. `D_800814A0 |= 0x8000;' before `*(u16 *)(p - 0x2) |= 0x8000;' is
     83 words / 12 aligned differences -- the `lui %hi(D_800814A0)' then
     cannot sit in the gate's branch delay slot. Same fact, same 12 rows, in
     func_s34_80024684 below. */
void func_s34_80024538(u8 *p, UnkVec *b, u8 *c)
{
    *(u16 *)(*(u8 **)(p + 0x0) + 0x52) |= 0x8000;
    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        c[0x4] = 0;
        c[0x5] = 0;
    }
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x200;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x200;
        if (*(u16 *)(c + 0x1E) >= 0x1C00)
        {
            *(s16 *)(c + 0x1C) = 0xC00;
            *(s16 *)(c + 0x1E) = 0x1800;
            *(s16 *)(p + 0x4A) = 0;
            *(s32 *)(c + 0xC) = 0x808080;
        }
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        if (c[0xC] < 9)
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(s32 *)(c + 0xC) += 0xFFEFEFF0;
        }
        break;
    }
}

/* The 0x201 node's step, registered as `n->unk_10' by func_s34_80024020's
   state 1 alongside func_s34_80024714 as its renderer: raise 0x8000 in the
   view's +0x52, walk the +0xC counter up by 8 while it is under 0x50 but
   clamp it to eight times +0x10 while that is under 9, then run the +0x10
   countdown and tear the node down when it expires. 36 words, 0 differing,
   FIRST COMPILE. A leaf -- no frame, no saves.

   NO TWIN at any ratio (dngtwin); dngcallee's only shared symbol is
   D_800814A0, which 608 functions reach, so it is no lead. The countdown
   idiom is csrc/ovl_dng_b04.c func_b04_801749F4's, recorded there: `lhu',
   `addiu -1', `sh', `sll 16', `bgtz' with no `sra' and no second load is
   `(s16)(--*(u16 *)(p + 0x10)) <= 0' -- the test reads the DECREMENTED value,
   not a reload.

   TWO SPELLINGS WORTH KNOWING. The +0x10 halfword is read twice through
   DIFFERENT types and both are needed: `*(s16 *)' for the `< 9' compare (the
   target's `lh', because the value is then shifted left 6... here `<< 3') and
   `*(u16 *)' for the decrement. And it must go through the local `h', because
   the unconditional `*(s32 *)(p + 0xC) = t' store between the compare and the
   `h * 8' use wipes cse's memory table and a second `*(s16 *)(p + 0x10)'
   would reload.

   MEASURED AND LOAD-BEARING: the teardown's store order, 12 aligned
   differences at the same 36 words if `D_800814A0 |= 0x8000;' is written
   first. See func_s34_80024538 above -- same fact, same cost, two bodies. */
void func_s34_80024684(u8 *p)
{
    s32 t;
    s32 h;

    *(u16 *)(*(u8 **)(p + 0x0) + 0x52) |= 0x8000;
    t = *(s32 *)(p + 0xC);
    if (t < 0x50)
    {
        t += 8;
    }
    *(s32 *)(p + 0xC) = t;
    h = *(s16 *)(p + 0x10);
    if (h < 9)
    {
        *(s32 *)(p + 0xC) = h * 8;
    }
    if ((s16)(--*(u16 *)(p + 0x10)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s34", func_s34_80024714);
