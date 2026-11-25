/* func_s36_8002401C -- PARKED.  479 target words, 478 built, 65 ALIGNED
   differences (ovlverify's unaligned figure is 415 -- the one missing word
   shifts the stream, so always size this one with aligndiff.py).  dngfam.py:
   1 MEMBER, 479 words, so this is the whole yield; it is not a replication
   representative.

   Progress across this run, every figure from aligndiff.py:

       first compile                                   479 w / 152 rows
       + `r->unk_08 = r->unk_00->unk_04'               475 w /  89
       + `(s16)' on func_800A3820's result             476 w /  86
       + separate loop counter `for (a = n; ...)'      476 w /  68
       + `(s16)' on func_800BCB04's result             478 w /  65

   WHAT THIS FUNCTION IS.  The thrown-object actor's per-frame step, a
   seven-case state machine on *(s16 *)(p + 0xA) (cases 0, 1, 2, 0xF0, 0xF1,
   0xFF, 0x100, no jump table -- jtcheck reports none in this unit and the
   dispatch is emit_case_nodes' balanced compare tree).  Ahead of the machine
   it counts the +0x50 timer down and, while the state is still positive, runs
   a SECOND switch on the same state to decide how many 0x312 trail nodes to
   spawn this frame (0x14 - timer for state 1, eight for states 2, 0xF0 and
   0xF1, none otherwise), then spawns that many with func_s36_800249DC as
   their step, each jittered +-0x100 on x and y and +-8 on z off the caller's
   UnkVec and dressed 0x101010 from the D_800DECF8 template.  State 0 aims the
   throw: it reads the thrower's aim through func_8003DE58, picks a target with
   func_800A05A4, parks the target's +0x24/+0x25 tile in the actor's
   +0x72/+0x73, publishes func_s36_80024798 as the actor's draw, derives a
   flight time from max(|dx|, |dy|) clamped to at least 4, and divides the
   three axis distances by it into the UnkVec's +0xC/+0x10/+0x14 velocity.
   State 1 integrates that velocity and cycles the +0x52 frame.  State 2
   spawns the landing effect (func_s36_80024C7C draw, func_s36_80024B60 step)
   once the timer hits 2.  0xF0 unlinks the node and re-arms, 0xF1 drives
   func_s36_8002506C and the func_800C8900 hit, 0xFF clears the +0x10 flag,
   0x100 retires the actor and drops D_8008346C.

   NO TWIN at any ratio.  dngcallee's best overlap is 8 of 18 symbols across
   four bodies (func_s38_80024FA4, func_s02_80024BE8, func_b37_80172840,
   func_b09_80173230) and none of them is this shape.  What the body was
   actually read off: the listing, plus csrc/ovl_dng_s59.c's landed
   func_s59_80024DD4 for the 0x312/0x212 spawn skeleton and its jitter fact
   (the constant stays OUTSIDE the addend here -- `= base + (rand() & 0x1FF) -
   0x100' gives the target's `addu' then `addiu -0x100', which is the
   SPELLING s59 REJECTED on its own body, so that fact is inverted here), and
   csrc/ovl_dungeon_800D50FC.c / csrc/ovl_dungeon.c for the INLINED
   D_800DECF8 slot template (every other site in the tree calls
   func_8003DB94(r, D_800DECF8, 0); this one writes the four stores out).

   THE RESIDUE IS THREE GROUPS AND 62 OF THE 65 ROWS ARE ONE CAUSE.

   GROUP 1 -- 62 rows and BOTH missing words, the spawn loop at
   0x80024114..0x80024160.  The target reads `np->unk_08' THREE TIMES, once
   per jitter channel, each read sunk past that channel's own `rand()' call
   into the caller-saved $v0 (`lw $v0, 0x8($s2)' at 0x80024120, 0x8002413C and
   0x80024158).  The parked body reads it ONCE into a callee-saved register,
   which is two words short and displaces the four pointers after it.

   I HAVE THE SPELLING THAT REPRODUCES THE TARGET EXACTLY AND IT COSTS MORE
   THAN IT SAVES, which is why this is parked rather than landed.  Writing the
   three statements with the destination inline --
   `*(s16 *)((u8 *)np->unk_08 + 0x2) = ...' and no `m' local -- emits the
   target's three loads, in $v0, after each call, verbatim (I checked the
   compiler's own output, not just the score).  But with `m' gone, loop.c's
   move_movables HOISTS the `&D_800DECF8' pair out of the spawn loop into a
   callee-saved register: two extra words before the loop and every
   callee-saved name from $s3 up shifted by one.  That is 479 words / 204-206
   aligned differences, strictly worse than the 478 / 65 kept here.

   THE HOIST IS THE WHOLE PROBLEM AND I MEASURED ITS ARITHMETIC.  `cdkgcc -da'
   writes a .loop dump with move_movables' own decisions, and for this loop it
   says `Loop from 104 to 314: 76 real insns' and prints one line per movable:
   a movable is moved iff `threshold * savings * lifetime >= insn_count' with
   threshold 29, savings = n_times_used of the pseudo and lifetime its luid
   span.  With the `m' local the D_800DECF8 pair is one of the
   `savings 1 not desirable' lines (29 * 1 * 2 = 58 < 75) and stays inside the
   loop as a single `la'; with the destination inline it comes out
   `regno 154 (life 6), savings 2  moved'.  So the question for whoever picks
   this up is narrow: WHAT SPELLING GIVES THE INLINE DESTINATION *AND* LEAVES
   THE D_800DECF8 PSEUDO WITH ONE USE AT LOOP TIME?  In the kept body the
   second use is not a use at all at loop time -- `r->unk_08 =
   r->unk_00->unk_04' reads the field back, and it is cse2, AFTER loop, that
   forwards the stored register and produces the target's
   `addu $v1, $t0, $zero' copy.  Nine spellings of that pair were measured on
   the inline base and all nine hoist (table below).

   GROUP 2 -- 2 rows and the one extra word, 0x80024458..0x80024474.  After
   `mflo $v1' for the x velocity the target fills the slot with the NEXT
   channel's base computation (`lb $v0, 0x73($s4)', `sll 6', `addiu 0x20',
   `lh $v0, 0x6($s6)') and only then does `sll $v1, 16' / `sw $v1, 0xC($s6)'.
   The parked body emits a load-delay nop there instead.  Both channels are
   separate statements in the obvious order; I did not find the shape that
   makes sched1 interleave them.

   GROUP 3 -- 1 row, 0x800242B4: `move a3,zero' against a nop, the fourth
   argument of func_8003DE58 set one slot early.  Free-standing scheduling.

   WHAT A PERMUTER RUN SHOULD BE POINTED AT.  The three jitter statements

       *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)v + 0x2) + (rand() & 0x1FF)
                                 - 0x100;

   and the four D_800DECF8 slot stores beside them, together -- they are one
   question, not two, and every one-at-a-time sweep over either half has
   already been done here.  Everything outside the spawn loop is at 0.

   MEASURED AND REJECTED -- every spelling tried, with its aligndiff score.
   The baseline each was measured against is named, because three of these
   are worse alone and two are only worth anything in combination.

     THE JITTER DESTINATION (on the 478 / 65 kept baseline):
       `m = np->unk_08' once, three uses               478 w /  65  KEPT
       destination inline at all three channels        481 w / 206  (hoists)
       inline at channel 1, `m' for 2 and 3            480 w /  62  (one
                                                       word over; the only
                                                       variant that beats
                                                       the kept score)
       inline at channels 1 and 2, `m' for 3           481 w /  64
       `m = np->unk_08' repeated before each channel   482 w /  84
       destination inline, no `m', earlier baseline    476 w / 219

     THE D_800DECF8 SLOT TEMPLATE (on the inline base, 481 w / 206, where
     killing the hoist is what is wanted -- all nine hoist):
       `r->unk_08 = r->unk_00->unk_04'                 481 w / 206
       `*(u32 *)(D_800DECF8 + 0x4)'                    479 w / 206
       `((UnkElem *)D_800DECF8)->unk_04'               479 w / 204
       `*(u32 *)((u8 *)r->unk_00 + 0x4)'               481 w / 206
       `el' pointer local, `el->unk_04'                479 w / 204
       `*(u32 *)&D_800DECF8[4]'                        479 w / 206
       load written BEFORE the `r->unk_00' store       480 w / 210
       `(UnkElem *)&D_800DECF8[0]' for the store       479 w / 204
       `|= 0xC' moved after the +0x08 store            479 w / 204-206
     and on the kept (`m') baseline, where the first of these is the one
     that KILLS the hoist:
       `r->unk_08 = r->unk_00->unk_04'                 475 w /  89  KEPT
       `*(u32 *)(D_800DECF8 + 0x4)'                    479 w / 152
       `((UnkElem *)D_800DECF8)->unk_04'               479 w / 149
       `el' pointer local, `el->unk_04'                479 w / 149

     THE SPAWN-LOOP COUNTER (on the 476 / 86 baseline):
       `for (a = n; a >= 0; a--)', reusing case 0's
         scratch as the counter                        476 w /  68  KEPT
       `for (; n >= 0; n--)', one variable             476 w /  86  (no
                                                       `addu $s3, $a0, $zero'
                                                       copy at 0x800240D0)
       `for (k = n; ...)', a third variable            478 w / 173

     THE TWO CAST FACTS (each on the baseline before it):
       `(s16)func_800A3820_i(0x24)' through an s32
         alias declaration                             476 w /  86  KEPT
       `(s16)func_800A3820(0x24)' with the landed
         `u8' prototype -- emits `andi 0xff' where
         the target has `sll 16' / `sra 16'            475 w /  89
       `a = (s16)func_800BCB04(...) - 0xC0'            478 w /  65  KEPT
       `a = func_800BCB04(...) - 0xC0'                 476 w /  68

   NOTE ON THE ALIAS DECLARATION.  func_800A3820 is LANDED in
   csrc/ovl_dungeon_8009F644.c as `u8 func_800A3820(s16 i)', and that
   prototype cannot produce this call site's sign extension, so the park
   declares a unit-local alias rather than touching the landed definition.
   That is the project's documented fix for a prototype a call site disagrees
   with.

DECLARATIONS
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A3820_i(s16) __asm__("func_800A3820");
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
void func_80044A50(UnkNode *);
void func_800419EC(u16, u32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800C8900(u8 *, s16, s16);
void func_s36_800249DC(u8 *, UnkVec *, UnkSlot *);
void func_s36_80024B60(u8 *, UnkVec *, UnkSlot *);
s32 func_s36_80024798(u8 *, UnkVec *);
s32 func_s36_80024C7C(u8 *);
void func_s36_8002506C(u8 *, s16);
extern u8 D_80083498[];
extern u8 D_80045340[];
extern u8 D_800DECF8[];
extern s32 D_8008346C;
extern u8 D_800E3D68;
END DECLARATIONS

   D_800814A0 and UnkNode/UnkSlot/UnkElem/UnkVec come from include/structs.h,
   and `rand' from common.h; the five func_s36_* above are this unit's own
   landed bodies, all of which are DEFINED BELOW this one in
   csrc/ovl_dng_s36.c, so in the unit they need the forward declarations
   listed here. */
void func_s36_8002401C(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *e;
    u8 *f;
    UnkNode *g;
    UnkNode *np;
    UnkSlot *r;
    UnkVec *m;
    u8 *q;
    u8 *h;
    s32 n;
    s32 a;
    s32 b;
    s16 loc[3];

    e = *(u8 **)p;
    f = *(u8 **)(e - 0x14);
    g = (UnkNode *)(e - 0x20);
    *(u16 *)(p + 0x50) -= 1;
    if (*(s16 *)(p + 0xA) > 0)
    {
        switch (*(s16 *)(p + 0xA))
        {
        case 1:
            n = 0x14 - *(s16 *)(p + 0x50);
            break;
        case 2:
            n = 8;
            break;
        case 0xF0:
        case 0xF1:
            n = 8;
            break;
        default:
            n = -1;
            break;
        }
        for (a = n; a >= 0; a--)
        {
            np = func_8003FD64(0x312, D_80083498);
            if (np != NULL)
            {
                func_8004491C(np, (u32)D_80045340);
                r = (UnkSlot *)np->unk_0C;
                np->unk_10 = (u32)func_s36_800249DC;
                m = np->unk_08;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)v + 0x2) + (rand() & 0x1FF) - 0x100;
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)v + 0x6) + (rand() & 0x1FF) - 0x100;
                *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)v + 0xA) + (rand() & 0xF) - 0x8;
                q = (u8 *)np + 0x20;
                *(s32 *)(q + 0x1C) = v->unk_00;
                *(s32 *)(q + 0x20) = v->unk_04;
                *(s32 *)(q + 0x24) = v->unk_08;
                *(s32 *)(q + 0x34) = v->unk_0C;
                *(s32 *)(q + 0x38) = v->unk_10;
                *(s32 *)(q + 0x3C) = v->unk_14;
                *(u16 *)(q + 0x48) = *(u16 *)(p + 0x50);
                r->unk_1E = 0x1000;
                r->unk_1C = 0x1000;
                r->unk_10 = 0x20;
                *(u32 *)r->unk_0C = 0x101010;
                r->unk_00 = (UnkElem *)D_800DECF8;
                r->unk_14 |= 0xC;
                r->unk_08 = r->unk_00->unk_04;
                r->unk_04 = 0;
                r->unk_05 = 0;
                *(u32 *)q = (u32)p;
                *(u16 *)(q + 0x4C) = 0;
            }
        }
    }
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        r = (UnkSlot *)g->unk_0C;
        if (func_8003DE58((u8 *)r->unk_08, r, loc, 0) == NULL)
        {
            loc[1] = 0;
            loc[0] = 0;
            if ((*(u16 *)(f + 0x14) & 0x8000) != 0)
            {
                b = -0x30;
            }
            else
            {
                b = 0;
            }
            loc[2] = b;
        }
        h = (u8 *)func_800A05A4(e, *(u8 *)(f + 0x24), *(u8 *)(f + 0x25), *(s16 *)(e + 0x2A),
                                (s16)func_800A3820_i(0x24));
        *(u32 *)(e + 0x60) = (u32)h;
        if (h == NULL)
        {
            *(u8 *)(e + 0x72) = *(u8 *)(f + 0x24);
            *(u8 *)(e + 0x73) = *(u8 *)(f + 0x25);
        }
        else
        {
            q = *(u8 **)(h - 0x14);
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (s->unk_14 & 0x8000) != 0)
            {
                *(u16 *)(p + 0xA) = 0xF0;
                break;
            }
            *(u8 *)(e + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(e + 0x73) = *(u8 *)(q + 0x25);
        }
        v->unk_00 = ((UnkVec *)g->unk_08)->unk_00 + (loc[0] << 16);
        v->unk_04 = ((UnkVec *)g->unk_08)->unk_04 + (loc[1] << 16);
        v->unk_08 = ((UnkVec *)g->unk_08)->unk_08 + (loc[2] << 16);
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_s36_80024798);
        a = __builtin_abs(*(s8 *)(e + 0x72) - *(u8 *)(f + 0x24));
        b = __builtin_abs(*(s8 *)(e + 0x73) - *(u8 *)(f + 0x25));
        if (a < b)
        {
            a = b;
        }
        if (a < 4)
        {
            a = 4;
        }
        *(u16 *)(p + 0x50) = a * 4;
        a = *(s8 *)(e + 0x72) * 0x40 + 0x20;
        v->unk_0C = ((a - *(s16 *)((u8 *)v + 0x2)) / *(s16 *)(p + 0x50)) << 16;
        b = *(s8 *)(e + 0x73) * 0x40 + 0x20;
        v->unk_10 = ((b - *(s16 *)((u8 *)v + 0x6)) / *(s16 *)(p + 0x50)) << 16;
        a = (s16)func_800BCB04(*(s8 *)(e + 0x72) * 0x40 & 0xFFC0, *(s8 *)(e + 0x73) * 0x40 & 0xFFC0,
                               *(u16 *)((u8 *)g->unk_08 + 0xA) - 0x30) -
            0xC0;
        v->unk_14 = ((a - *(s16 *)((u8 *)v + 0xA)) / *(s16 *)(p + 0x50)) << 16;
        *(u16 *)(p + 0x52) = 0;
        func_800A56E0(0x300);
        *(u16 *)(p + 0xA) += 1;
        break;
    case 1:
        if (*(s16 *)(p + 0x50) >= 0)
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        *(u16 *)(p + 0x52) += 1;
        if (*(s16 *)(p + 0x52) >= 8)
        {
            *(u16 *)(p + 0x52) = 4;
        }
        if (*(s16 *)(p + 0x50) <= 0 && *(s16 *)(p + 0x52) == 7)
        {
            *(u16 *)(p + 0x50) = 8;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 2:
        *(u16 *)(p + 0x52) += 1;
        if (*(s16 *)(p + 0x50) == 2 && *(u32 *)(e + 0x60) != 0)
        {
            np = func_8003FD64(0x302, D_80083498);
            if (np != NULL)
            {
                func_8004491C(np, (u32)func_s36_80024C7C);
                np->unk_10 = (u32)func_s36_80024B60;
                q = (u8 *)np + 0x20;
                *(u16 *)(q + 0xC) = *(s8 *)(e + 0x72) * 0x40;
                *(u16 *)(q + 0xE) = *(s8 *)(e + 0x73) * 0x40;
                *(u16 *)(q + 0x10) = *(u16 *)((u8 *)v + 0xA);
                *(u32 *)q = (u32)p;
                *(u16 *)(q + 0x4C) = 0;
            }
        }
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(u32 *)(e + 0x60) != 0)
            {
                *(u16 *)(p + 0xA) = 0xF0;
            }
            else
            {
                func_80044A50((UnkNode *)(p - 0x20));
                *(u16 *)(p + 0xA) = 0xFF;
            }
        }
        break;
    case 0xF0:
        func_80044A50((UnkNode *)(p - 0x20));
        *(u16 *)(p + 0x50) = 0xA;
        func_800419EC(8, 0x10);
        *(u16 *)(p + 0xA) += 1;
        break;
    case 0xF1:
        func_s36_8002506C(*(u8 **)(e + 0x60), *(s16 *)(p + 0x50));
        if (*(s16 *)(p + 0x50) == 6 && func_8009D218(*(u8 **)(e + 0x60), 4, e) == 0 &&
            *(u8 **)(e + 0x60) != NULL)
        {
            func_800C8900(*(u8 **)(e + 0x60), D_800E3D68 == 0xFF ? 0xFF : 0x10, 4);
        }
        if (*(s16 *)(p + 0x50) <= 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(u16 *)(p + 0xA) = 0xFF;
        }
        break;
    case 0xFF:
        if ((*(u32 *)(p + 0x10) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x10) &= ~0x8000;
        }
        else
        {
            *(u16 *)(p + 0x50) = 0xA;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 0x100:
        if (*(s16 *)(p + 0x50) < 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
