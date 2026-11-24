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
s32 func_b04_80172270(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_80175140[];
extern u8 D_80175188[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b04_80171B28(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b04_80171400();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AB464();

s32 func_800AC7EC();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

extern s16 D_800DCF5E;
extern s16 D_80175218;
extern s32 D_80175220;
extern u8 D_80175180[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, UnkVec *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b04_80174218(u8 *p, u8 *b, u8 *c, u8 *d);
void func_800478B8(u8 *);
s32 func_800453E0(u8 *, UnkVec *, u8 *, s16);

void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
extern u8 D_80175148[];
extern u8 D_80175158[];
extern u8 D_80175168[];
extern u8 D_80083498[];
extern u8 D_80045340[];
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_800478E8(u8 *, u32, s32);
void func_800D78C0(u8 *);
void func_800D71A8();
void func_b04_80170BF8();
void func_800DBA90(u8 *);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
s32 func_800AA79C(u8 *, void *, u8 *, u8 *);
extern u8 D_80175190[];

/* The banked room script's SPAWNER: allocate the 0x112 node this bank's
   actor lives in off D_80083498, install func_b04_80170BF8 as the node's
   +0x10 driver with state 4 at +0x13, dress the slot from the bank-local
   D_80175140 facing table, pick the +0x14/+0x1C flag pair from the low two
   bits of `a' and -- only for a==0, one actor in two -- raise +0x1C's 0x200,
   recolour through func_800A48F0 and swap in D_80175188. Then arm the slot
   (+0x9A/+0x9C/+0x8C/+0x92/+0xA8/+0x9E/+0x98) and, for each of the +0x9E
   sub-slots, allocate ANOTHER 0x112 node, park it in the +0xA4 pointer array
   and dress each of its 0x30-byte sub-renderables at +0x8 from D_80175168
   with func_800D78C0 as the node driver and func_800D71A8 as its node call.
   199 words, 0 differing.

   The first half is the landed func_b09_80171158's (csrc/ovl_dng_b09.c)
   skeleton and ALL of its measured facts hold here -- the s16 first
   parameter whose second copy is the schedulable prologue copy the
   func_800A9C18 call's `sll'/`sra' reads, the u16 `b' and `c' that are only
   ever stored as the bytes at r+0x24/+0x25, the `w' second payload pointer,
   `*(s8 *)(w + 0x9C) = -1' rather than `w[0x9C] = 0xFF', and
   `func_800AA36C(w, m, r, p)' with p and not w as the fourth argument. Read
   that comment for their costs; none was re-measured here because the body
   landed with all of them in place. The deltas from it are state 4 instead
   of 9, the extra `*(u32 *)(p + 0x1C) |= 0x200' in the a==0 arm, and the
   whole second half.

   BOTH LOOPS ARE `do'-WHILE AND NEITHER HAS AN ENTRY TEST. The target falls
   straight into each loop head and reads its bound -- `*(s16 *)(w + 0x9E)'
   and `*(s16 *)(q + 0x2)' -- at the BOTTOM of every iteration, although both
   were stored as 1 a few insns earlier, so neither bound is a loop invariant
   the compiler could hoist and neither loop can have been a `for'.

   THE INNER ADDRESS IS `q + (j * 0x30 + 8)', WITH THE 8 INSIDE THE
   PARENTHESES. That is what makes loop.c's strength reduction build the
   OFFSET giv the target keeps in $s1 (init 8, `addiu 0x30' at the bottom)
   and add it to q with `addu $a0, $s0, $s1'. Written `q + j * 0x30 + 8' the
   giv is a POINTER starting at q and the access becomes
   `addiu $a0, $s1, 0x8': 200 words, 15 aligned differences.

   AND `*(s32 *)(e + 0x28) = *(s32 *)(r + 0x28)' COMES FIRST OF THE THREE
   SUB-RENDERABLE STORES. The three stores share base $a0 at constant
   offsets, so sched reorders them freely into the target's 0x10/0xC/0x28
   order, but the `lw' of r + 0x28 is a different base and can never be
   hoisted above them -- written third it is stranded behind `sw $a1, 0xC'
   with a `nop' in its load delay, which is the 200th word.

   `j = 0' PRECEDES `t = s' (the other way round is 2 aligned differences,
   $s2 and $fp set in the other order), and the node pointer is re-read from
   `t + 0xA4' inside the inner loop rather than reused from the local the
   allocation returned, which is what the `lw $v0, 0xA4($fp)' is. */
UnkNode *func_8003FC64(s32);
void func_b04_80174600();
extern u8 D_8017520C[];
typedef struct UnkDngB04Name12
{
    u8 b[0xC];
} UnkDngB04Name12;
void func_80041588(Unk41284 *, s8 *, s32);
u32 func_800445E0(void);
s32 func_800C77D0(void *, u8 *, s16, s32);
s32 func_800A56E0(s32);
s32 func_800498A0(u8 *);
void func_8003DB94(void *, void *, s32);
void func_800A18E8(s16, s16);
void func_8009A028(u8 *);
UnkNode *func_800ADC4C(u8 *, s32, s16, u8 *);
void func_80047738(u8 *, s16, s16);
extern u8 *D_800DCEEC[];
extern u8 D_8014A000[];
extern u8 D_800DCF5C;
extern s32 D_8006CD58;
extern u8 D_8003E140[];
extern Unk41284 D_801751F8[];
extern u8 D_8017521C[];
extern u8 D_8017521D[];
extern s32 D_80175200[];
extern s16 D_8008346A;
void func_800ACF88(u8 *);
void func_800A2FE0(u8 *);
u8 *func_800A32A4(u8 *);
s32 func_800A504C(u8 *, u8 *);

u8 *func_b04_801708DC(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkNode *n2;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    u8 *q;
    u8 *e;
    u8 *s;
    u8 *t;
    s32 i;
    s32 j;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b04_80170BF8;
        p[0x13] = 4;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *(u8 **)((u8 *)r + 0x2C) = D_80175140;
        *((u8 *)r + 0x24) = b;
        k = a & 3;
        *((u8 *)r + 0x25) = c;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    *(u32 *)(p + 0x1C) |= 0x200;
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80175188;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b04_80171400;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x20;
        *(s16 *)(w + 0xA8) = 0;
        *(s16 *)(w + 0x9E) = 1;
        *(u16 *)(w + 0x98) |= 0x4000;
        s = w;
        i = 0;
        do
        {
            n2 = func_8003FD64(0x112, D_80083498);
            *(UnkNode **)(s + 0xA4) = n2;
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(s16 *)(q + 0x2) = 1;
                j = 0;
                t = s;
                do
                {
                    e = q + (j * 0x30 + 8);
                    *(u16 *)(q + 0x4) |= 0x8000;
                    *(s32 *)(e + 0x28) = *(s32 *)((u8 *)r + 0x28);
                    *(s16 *)(e + 0x10) = 0x20;
                    *(s32 *)(e + 0xC) = 0x808080;
                    (*(UnkNode **)(t + 0xA4))->unk_10 = (u32)func_800D78C0;
                    *(u8 **)(e + 0x2C) = D_80175168;
                    func_80047784(e, D_80175168[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
                    j++;
                    *(s16 *)(q + 0x6) = 1;
                    func_800478E8((u8 *)r, (u32)func_800D71A8, 1);
                    *(s32 *)(q + 0x98) = (s32)n + 0x1E;
                } while (j < *(s16 *)(q + 0x2));
            }
            s += 4;
        } while (++i < *(s16 *)(w + 0x9E));
        func_800AA36C(w, m, r, p);
    }
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b04", func_b04_80170BF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b04", func_b04_80171400);

/* The banked room script's path-step advance: past the d + 0x71 / d + 0x8A
   step gates, dress the renderable from D_80175140, retire the current
   waypoint through func_8009A3D0, take the next pair out of d + 0x74 /
   d + 0x7C, and re-aim with func_8009A21C and func_800A0818. 114 words,
   0 differing, first compile.

   THE TWIN IS THE LANDED func_b38_80171670 (csrc/ovl_dng_b38.c, dngtwin
   0.951, 111 words), whose frame is identical to this one -- `addiu $sp,
   $sp, -0x30' with $s0..$s4 and $ra in that order -- so its allocation facts
   were the transferable ones and ALL THREE HELD, two of them reproducing its
   figures to the digit:

     the p + 0xA8 zero hoisted ABOVE the func_80047784 call
                                           114 words /  2 aligned  (twin: 2)
     `x'/`y' reassigned from c + 0x24 / c + 0x25 before the func_8009A21C
       call instead of the two fresh `lbu's the listing has
                                           116 words / 55 aligned  (twin: 55)
     the two 0x2000 tests collapsed into one hoisted local
                                           113 words / 63 aligned  (twin: 33)

   The three deltas from it are the table (D_80175140), the +0xA8 zero where
   it writes +0xA0, and -- the three words over its 111 -- THE DRESS GATE
   TESTS TWO TABLES, `!= D_80175140 && != D_80175148', against its one.

   MEASURED AND FREE on that gate: the two tests as NESTED `if's instead of
   `&&' are also 114 / 0. fold_truthop has nothing to merge here because both
   tests are on one `lw' of c + 0x2C rather than on a local, so the `&&' and
   the nesting emit the same `beq' pair. */
void func_b04_80171960(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 n;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_80175140 && *(u8 **)(c + 0x2C) != D_80175148)
        {
            *(u8 **)(c + 0x2C) = D_80175140;
            func_80047784(c, D_80175140[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s16 *)(p + 0xA8) = 0;
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
   AND PASSED ON. func_b04_80172270's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A8C0 2 words, func_b04_80172270 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b04_80171B28(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b04_80172270(p, b, c, d) == 0)
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
   D_80175140 facing table and re-aim through func_8009C93C.

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
s32 func_b04_80172270(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_80175140;
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
void func_b04_80172434(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
s32 func_b04_80172554(u8 *a, void *b, void *c, s32 d)
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
        func_b04_80171B28(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b04_80171B28(p, b, c, p);
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
   func_b04_80171400. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b04_80172698(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b04_80171400;
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
void func_b04_8017272C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b04", func_b04_80172790);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b04", func_b04_80172D74);

/* The banked room script's three-state glide, switched on p + 0x9B: state 0
   seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair, state 1
   decays it by the same pair shifted 14, state 2 eases onto the tile centre,
   re-dresses from the bank-local D_80175148 and installs
   func_b04_80171400. The resident overlay's func_800D9C7C
   (csrc/ovl_dungeon_800D50FC.c) is the same machine and the landed
   func_b05_801730B8 (csrc/ovl_dng_b05.c) is the banked shape of it; both of
   those comments carry the spelling facts this body relies on. 244 words,
   0 differing.

   THE func_800AAA54 EXIT IS WRITTEN OUT TWICE, NOT REACHED BY `goto'. With
   func_800D9C7C's single `stop:' label inside case 1 this body is 239 words
   and 15 aligned differences: the target sets up $a0..$a3 in case 0's own
   arm and `j's past case 1's `sb $zero, 0x9B($a0)', which is jump2
   cross-jumping two separate calls down to their common tail, exactly as
   func_b05_801730B8 writes it.

   THREE DELTAS FROM func_b05_801730B8, read off this listing: case 0 does
   NOT fall through into case 1 -- it ends at the `/4' decay with a `j' to
   the epilogue -- and its d[0x28] test comes BEFORE the two seeds, with
   `p[0x9B] += 1' after them; case 1's refusal arm additionally writes
   `p[0x9B] = 0' before the shared stores; and case 2 opens with
   `b->unk_14 = 0xFFFE0000' (the `lui 0xFFFE' sits in the dispatch `beq's
   delay slot) and carries the extra `+= >> 1' pair, the p + 0x90 / p + 0x98
   flag work and the D_80175148 dress that func_800D991C's tail has.

   THE PAIRED FLAG CLEARS AND SETS ARE TWO STATEMENTS EACH ON THE SAME WORD.
   `&= ~0x8000000' then `&= ~0x40000' (and `|=' likewise in case 2) emits one
   `lw', two `and's with two separately materialised masks and one `sw',
   because flow.c deletes the first store as dead while combine can no longer
   reach across it to fold the masks together. One statement
   `&= ~0x8040000' is a single mask and two words short.

   Case 2's two eases keep func_b05_801730B8's EMBEDDED assignment form --
   the `(u = ...)' inside the subtraction is what stops fold-const turning
   `- (*(s16 *)(b + 2) - 0x20)' into `+ 0x20'. The second ease written as a
   preceding statement instead is 244 words and 2 aligned differences, the
   `lh' of b + 0x6 evaluated before the `lbu' of c + 0x25. */
void func_b04_80173354(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800AAA54(p, b, c, D_80175158);
            return;
        }
        if (*(u16 *)(c + 0x14) & 0x8000)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 2;
            return;
        }
        b->unk_0C = D_8006CCD8[k] << 18;
        b->unk_10 = D_8006CCE8[k] << 18;
        b->unk_14 = 0x40000;
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        p[0x9B] += 1;
        *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        return;
    case 1:
        b->unk_0C -= D_8006CCD8[k] << 14;
        b->unk_10 -= D_8006CCE8[k] << 14;
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
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_80175158);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            p[0x9B] += 1;
        }
        return;
    case 2:
        b->unk_14 = 0xFFFE0000;
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (u = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) >> 1;
            b->unk_10 = (((c[0x25] << 6) - (w = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) >> 1;
            b->unk_0C += b->unk_0C >> 1;
            b->unk_10 += b->unk_10 >> 1;
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(s32 *)(p + 0x90) = 0;
        *(u16 *)(p + 0x98) &= ~8;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u8 **)(c + 0x2C) = D_80175148;
        func_80047784(c, D_80175148[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
        return;
    }
}

/* The banked room script's five-state death throw, switched on p + 0x9B and
   driven by the D_8006CCD8 / D_8006CCE8 facing pair read ONCE before the
   dispatch: state 0 seeds the velocity and kicks func_800A56E0(0x805),
   states 1 and 3 drift it by 0x1C000 behind the D_8008346A and c + 0x14
   gates, state 2 re-dresses the renderable to 0x60/0xC through the
   func_800ACF88 arm, and state 4 fades c + 0xC..0xE out of p + 0x96 and,
   when the fade is under 0x18, releases D_80083460's +0x10 claim and tears
   the actor down. 164 words, 0 differing.

   THIS FUNCTION OWNS jtbl_b04_801708A0, at 0x801708A0 (rodata offset 0x98).
   It has FIVE live entries, for states 0..4 at 0x801737A4, 0x80173800,
   0x80173838, 0x80173884 and 0x801738C0, matching the `sltiu $v0, $a1, 0x5'
   gate; everything else goes to the epilogue and no two states share a
   label. The SIXTH `.word 0x00000000' in the listing is NOT a case -- it is
   the `.align 3' pad between this table and jtbl_b04_801708B8 at
   0x801708B8, which func_b04_80174B20 owns. States 0, 1 and 2 fall through
   to their successors; state 3 ends in its own `j' to the epilogue.

   THE TWIN IS func_b04_80173354, TWO LISTINGS UP IN THIS UNIT -- the same
   p + 0x9B machine over the same facing pair -- and no carved or resident
   sibling of this body scores 0.70 anywhere. Its documented double-clear
   fact HOLDS: `*(s32 *)(d + 0x1C) &= ~0x8000000' then `&= ~0x40000' emits
   one `lw', two `and's against two separately materialised masks and one
   `sw', because flow.c deletes the first store as dead while combine can no
   longer reach across it to fold the masks. The single statement
   `&= ~0x8040000' is 161 words / 5 aligned differences.

   THREE SPELLINGS THIS BODY NEEDED THAT THE TWIN HAS NO SITE FOR:

   (1) THE b + 0x14 DRIFT IS A PLAIN CAST, NOT THE STRUCT FIELD. As
   `b->unk_14 += 0x1C000' the MEM is MEM_IN_STRUCT_P and gcc's fixed-scalar-
   versus-varying-struct rule lets sched1 hoist state 1's `lh' of the fixed
   scalar D_8008346A above the store, filling the `bnez's delay slot with it;
   the target leaves two `nop's there. 162 words / 12 aligned differences,
   and the same 162 / 12 with only the state-1 site changed, so all three
   sites are one fact.

   (2) STATE 2'S TWO d + 0x14 TESTS MUST BE NESTED `if's. Written
   `(v & 0x4000) && (v & 0x20000000) == 0', fold_truthop merges them into one
   `and' against 0x20004000 compared to 0x4000: 163 words / 11 aligned
   differences. Nested, the two masks stay separate over one cse'd `lw'.

   (3) STATE 4'S FADE BYTE COMES THROUGH A LOCAL. `c[0xE] = c[0xD] =
   c[0xC] = p[0x96]' written as three reads of p[0x96] is 168 words / 7
   aligned differences -- the stores through `c' may alias `p', so cse
   reloads for each one, where the target has a single `lbu'.

   MEASURED AND FREE, so NOT the reason this lands: the facing index as the
   byte offset the listing shows, `*(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(d +
   0x6A) >> 8) & 0xE))', instead of the twin's array ref
   `D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7]'. Still 164 / 0: combine folds
   the array ref's `sll 1' into the `srl'/`andi' pair by itself. */
void func_b04_80173724(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 x;
    s32 y;
    s32 v;

    x = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    y = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        b->unk_0C = x << 16;
        b->unk_10 = y << 16;
        b->unk_14 = 0xFFF40000;
        func_800A56E0(0x805);
        p[0x9B] += 1;
        /* fallthrough */
    case 1:
        *(s32 *)((u8 *)b + 0x14) += 0x1C000;
        if (D_8008346A != 0)
        {
            return;
        }
        p[0x9B] += 1;
        /* fallthrough */
    case 2:
        if (*(s32 *)(d + 0x14) & 0x4000)
        {
            if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
            {
                func_800ACF88(d);
            }
        }
        *(s16 *)(c + 0x10) = 0x60;
        *(u16 *)(c + 0x14) |= 0xC;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        *(s32 *)((u8 *)b + 0x14) += 0x1C000;
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0x80;
        p[0x9B] += 1;
        return;
    case 4:
        *(s32 *)(d + 0x1C) |= 0x10000000;
        *(s32 *)((u8 *)b + 0x14) += 0x20000;
        v = p[0x96];
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        if ((s16)(*(u16 *)(p + 0x96) -= 0x18) < 0x18)
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            func_800A2FE0(d);
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b04_801739B4(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b04_801739D4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b04_801739F4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}

/* The banked room script's OTHER three-state tail, state index at +0x9B, and
   the near-twin of the landed func_b03_80173358 (csrc/ovl_dng_b03.c), itself
   the banked copy of the resident overlay's func_800DA014: state 0 bumps the
   slot's +0x90 and re-dresses the renderable from the bank-local D_80175188
   facing table, state 1 is the long refusal chain with the D_80175190
   dress/flag/bump tail written out twice, state 2 runs the +0x96 countdown,
   re-dresses from D_80175140 and hands the actor back to func_b04_80171400.
   299 words, 0 differing.

   EVERYTHING func_b03_80173358's HEADER NAMES WAS KEPT AND IS WHAT MAKES
   THIS LAND -- read it, not this, for the costs: the D_80175190 tail written
   TWICE and cross-jumped back into one (worth two references on `p', which
   is what wins it $s1 over `b'), the TWO SEPARATE `&D_80083460' pointer
   locals for those two copies, a separate one per arm besides, the facing
   tables named directly at all four dress sites rather than held in a local,
   `*(s8 *)(d + 0x6D)' and the `lh' of d + 0x64, and the `(s16)' casts on
   func_80042900, func_800A2C34 and func_8009FD40. None of them was
   re-measured here: the body landed with all of them in place, which is
   itself the evidence that this is the same allocation problem.

   FIVE DELTAS FROM func_b03_80173358, all read off this listing:
   (1) state 0 opens with `*(s32 *)(p + 0x90) += 0x100000', the constant
   materialised by the dispatch `beqz's delay slot;
   (2) both dress tails add `*(s32 *)(d + 0x1C) |= 0x40000' before the
   c + 0x14 0x8000 test, and the `&= ~0x200' that follows it reuses that
   store's own value -- the target has no second `lw' of d + 0x1C;
   (3) their `else' adds `*(s16 *)(p + 0x96) = 3' and
   `*(u16 *)(p + 0x98) &= ~0x4000' ahead of the +0xA bump, and the two
   copies of that whole block cross-jump into the one the target has;
   (4) the +0x80000 arm clears +0xA8 BETWEEN the func_800AA888 call and the
   func_b04_80174218 hand-over, which is the same fact the landed
   func_b04_80173ED8 above measures: written before the call it is 2 aligned
   differences here, because func_800AA888's delay slot then takes the `sh'
   instead of the `addu $a3, $s0, $zero' argument copy the target has;
   (5) state 2 is new work -- the `lhu'/`addiu -1'/`sh'/`sll 16'/`bgtz'
   countdown on +0x96, which is `(s16)(--*(u16 *)(p + 0x96)) <= 0' with no
   second load, then b->unk_14 = 0xFFEC0000, and on the E000 gate's far side
   b->unk_14 = 0 and +0xA8 cleared before the dress. */
void func_b04_80173A2C(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(s32 *)(p + 0x90) += 0x100000;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80175188;
        func_80047784(c, D_80175188[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80175190;
            func_80047784(c, D_80175190[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
                return;
            }
            *(s16 *)(p + 0x96) = 3;
            *(u16 *)(p + 0x98) &= ~0x4000;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
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
            *(s16 *)(p + 0xA8) = 0;
            func_b04_80174218(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175190;
        func_80047784(c, D_80175190[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
            return;
        }
        *(s16 *)(p + 0x96) = 3;
        *(u16 *)(p + 0x98) &= ~0x4000;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 2:
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(p + 0x98) |= 0x4000;
            b->unk_14 = 0xFFEC0000;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_14 = 0;
        *(s16 *)(p + 0xA8) = 0;
        *(u8 **)(c + 0x2C) = D_80175140;
        func_80047784(c, D_80175140[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
        return;
    }
}

/* The banked room script's three-state tail, state index at +0x9B, and the
   near-twin of the landed func_b03_801736F4 (csrc/ovl_dng_b03.c), itself the
   banked copy of the resident overlay's func_800DA398: state 0 re-dresses the
   renderable from the bank-local D_80175188 facing table, state 1 is the long
   refusal chain with the D_80175180 dress/flag/bump tail written out twice,
   state 2 hands the actor back to func_b04_80171400. 208 words, 0 differing.

   THE DELTA FROM func_b03_801736F4 IS ONE WORD: the +0x80000 arm clears the
   slot's halfword at +0xA8 BETWEEN the func_800AA888 call and the
   func_b04_80174218 hand-over. Written before the call instead it is 19
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
   dress sites rather than held in a local. */
void func_b04_80173ED8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80175188;
        func_80047784(c, D_80175188[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80175180;
            func_80047784(c, D_80175180[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            *(s16 *)(p + 0xA8) = 0;
            func_b04_80174218(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80175180;
        func_80047784(c, D_80175180[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
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
   call emits the bare `jal` either way); hoisting D_80175140 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b04_80174218(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_80175188 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_80175140;
        func_80047784(c, D_80175140[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b04_801742E0(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b04_80171400;
        }
    }
}

/* 14 words, 0 differing. */
void func_b04_80174344(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}

/* 14 words, 0 differing. */
void func_b04_8017437C(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}

/* 14 words, 0 differing. */
void func_b04_801743B4(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}

/* The two-state fade step of the node at p, switched on the s16 at p + 0x4C:
   state 0 re-sends the view, drops its +0x4/+0x5 pair once the +0x14 flags
   carry one of 0x6000, steps the +0xC colour down by 0x181818 and advances
   the state when its low byte falls under 0x30; then it swings p + 0x8 by
   0xA, parks `-(((s16)(p + 0x8) >> 2) ** 2) >> 4' in p + 0x4 and builds
   func_800DBA90's 0x20-byte matrix record out of p + 0x4, the output vector
   and two eight-byte copies of p + 0x14 and p + 0xC, then writes the
   rotated vector back into b + 0x2 / +0x6 / +0xA. State 1 is the standard
   teardown: raise 0x8000 in the node's -0x2 and in D_800814A0.
   98 words, 0 differing.

   No twin: no landed body in the block calls func_800DBA90, and the resident
   overlay's two callers (func_800DB3F8 and func_800DB660,
   csrc/ovl_dungeon_800D50FC.c) build the record for a different primitive.
   What came from func_800DB660 is the RECORD'S TYPE: `u8 rec[0x20]', which
   is both why it sits at 0x10($sp) with the output vector at 0x30($sp) and
   why the two field copies are `lwl'/`lwr' pairs with no runtime alignment
   test -- a `u8' array is alignment 1, so cc1psx expands an 8-byte copy
   straight into one unaligned load pair and one unaligned store pair per
   word. The copies are `memcpy(rec + k, p + n, 8)' with both pointers u8 *,
   which is the only spelling that keeps that alignment (see
   docs/matching-levers.md, "`memcpy's ALIGNMENT COMES OFF THE POINTER
   EXPRESSION").

   THE p + 0x8 STORE IS WRITTEN AFTER THE SHIFT THAT FEEDS THE SQUARE, AND
   THE SHIFTED VALUE NEEDS ITS OWN LOCAL. The target's `sh $v1, 0x8($s1)'
   sits between the `mult' and its `mflo', in the multiply's latency, which
   sched can only do if the store comes after the `sll'/`sra' in the insn
   stream. Written `*(s16 *)(p + 0x8) = k; k = (s16)k >> 2;' -- one local,
   store first -- the body is still 98 words but the store is emitted
   immediately and it is 5 aligned differences. */
void func_b04_801743EC(u8 *p, u8 *b, u8 *c)
{
    u8 rec[0x20];
    SVECTOR vec;
    s32 j;
    s32 k;

    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            c[0x4] = 0;
            c[0x5] = 0;
        }
        *(s32 *)(c + 0xC) += 0xFFE7E7E8;
        if (c[0xC] < 0x30)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        k = *(u16 *)(p + 0x8) + 0xA;
        j = (s16)k >> 2;
        *(s16 *)(p + 0x8) = k;
        *(s16 *)(p + 0x4) = -(j * j) >> 4;
        *(u8 **)rec = p + 0x4;
        *(u8 **)(rec + 0x4) = (u8 *)&vec;
        memcpy(rec + 0x8, p + 0x14, 8);
        memcpy(rec + 0x10, p + 0xC, 8);
        *(s16 *)(rec + 0x18) = 1;
        *(s16 *)(rec + 0x1A) = 1;
        func_800DBA90(rec);
        *(s16 *)(b + 0x2) = vec.vx;
        *(s16 *)(b + 0x6) = vec.vy;
        *(s16 *)(b + 0xA) = vec.vz;
        break;
    case 1:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* The node walk, the relinked twin of the resident overlay's func_800ADBCC
   (csrc/ovl_dungeon_800A4B88.c): walk the second list off p - 8, park a
   halfword in the scratchpad each turn and call func_800453E0 for every slot
   whose +0x14 does not carry 0x80. 35 words, 0 differing, first compile.

   THE RELINK IS THE SCRATCHPAD VALUE. func_800ADBCC parks the record's own
   `*(s16 *)(p + 0x8)'; this copy parks the bank-local s16 D_80175218, whose
   `lui' is hoisted out of the loop while the `lh' stays inside it -- that is
   the whole of the three-word difference from the twin.

   Both of the twin's load-bearing spellings were kept and still are: the
   scratchpad goes through a POINTER LOCAL (as an absolute `*(s32 *)0x1F80013C'
   it is the assembler's `lui $at'/`sw' macro, which cannot leave the loop),
   and the slot stays a `u8 *' rather than a struct pointer so its +0x14 read
   is not MEM_IN_STRUCT_P and sched1 leaves the load-delay `nop's alone. */
s32 func_b04_80174574(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = D_80175218;
        if (!(*(u16 *)(s + 0x14) & 0x80))
        {
            func_800453E0(p, v, s, *(s16 *)(s + 0x6));
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (u8 *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b04", func_b04_80174600);

/* This bank's spawner for the 0x212 decoration node: 0x41 at the actor's
   +0x1A, func_b04_80174600 as the node's step, the renderable cleared to
   0x60/0xC, the mover seeded from the caller's vector with the third channel
   pulled 0x28 back, and then -- through a RE-READ of n->unk_0C -- a 6 at
   +0x06, the caller's s16 as both scale halves, a 0x70/0x10/0x10 colour over
   p[0..2], three zero bytes at r + 0xC..0xE, a 12-bit random spin at +0x1A
   and the 12-byte D_8017520C record copied to the node's own +0x48 for the
   slot's +0x08 to point at. 82 words, 0 differing, first compile.
   func_b04_80174B20's state 4 calls it seven times.

   THE TWIN IS THE LANDED func_b20_80173CFC (csrc/ovl_dng_b20.c, dngtwin
   0.982). Three deltas, all read off this listing: the +0x1A/0x41 seed in
   place of its +0x24/5, the node step, and the record landing at n + 0x48
   rather than n + 0x9A.

   ITS ALIGNMENT-1 RECORD TYPE HOLDS. The 12 bytes move with `lwl'/`lwr' +
   `swl'/`swr', which only a type of alignment 1 forces; as `u32 b[3]' the
   copy becomes three aligned `lw'/`sw' pairs, 76 words / 13 aligned
   differences.

   ITS STATEMENT ORDER INVERTS. The twin writes p[1], p[2], p[0] -- its three
   colour bytes are all 0x30, and cse rewrites the p[0] MEM back to the node
   base either way. Here the bytes are 0x70, 0x10, 0x10, the two constants
   force an order on the register, and p[0] must be written FIRST: p[0] last,
   the twin's way, is 82 words / 5 aligned differences.

   MEASURED AND FREE, so NOT the reason this lands: the three mover reads as
   `*(s16 *)' instead of the twin's `*(u16 *)', still 82 / 0. */
void func_b04_80174800(u8 *a, UnkVec *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x1A) = 0x41;
        n->unk_10 = (u32)func_b04_80174600;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x28;
        r = (UnkSlot *)n->unk_0C;
        r->unk_06 = 6;
        r->unk_1E = d;
        r->unk_1C = d;
        p[0] = 0x70;
        p[1] = 0x10;
        p[2] = 0x10;
        *((u8 *)r + 0xE) = 0;
        *((u8 *)r + 0xD) = 0;
        *((u8 *)r + 0xC) = 0;
        r->unk_1A = rand() & 0xFFF;
        *(UnkDngB04Name12 *)((u8 *)n + 0x48) = *(UnkDngB04Name12 *)D_8017520C;
        r->unk_08 = (u32)((u8 *)n + 0x48);
    }
}

/* State 0x17's entry: clear the node's +0x71 top bit and +0x6D, install
   ITSELF in the slot's +0x8C handler slot and mark state 0x17/0; then, unless
   D_80083460's +0x2 0x2000 bit is up, if func_800A2BDC says the engine is
   idle, drop the handler again, re-mark 0x17/0, reset D_800DCF5E and bump
   D_80083460's +0xA counter. 43 words, 0 differing, first compile.

   THE +0x8C STORE IS THE FUNCTION'S OWN ADDRESS -- `lui'/`addiu' of
   %hi/%lo(func_b04_80174948) -- so the self-reference is written as the
   plain identifier, not as a table lookup.

   D_80083460 goes through the unit's `g = (u8 *)&D_80083460' idiom: one
   `addiu' base in $s1 serves both the +0x2 gate and the +0xA bump. */
void func_b04_80174948(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    *(void **)(p + 0x8C) = func_b04_80174948;
    p[0x9A] = 0x17;
    p[0x9B] = 0;
    d[0x6D] = 0;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        if ((s16)func_800A2BDC(d) == 0)
        {
            *(s32 *)(p + 0x8C) = 0;
            p[0x9A] = 0x17;
            p[0x9B] = 0;
            D_800DCF5E = 0;
            *(u16 *)(g + 0xA) += 1;
        }
    }
}

/* The two-phase white-out. With the slot's +0x9A still zero, park the view's
   +0x8 in the bank-local D_80175220, re-send the view through func_800478B8
   and -- only if its +0x14 carries one of 0xE000 -- unlink the node, set the
   0x10-frame countdown in +0x96 and step +0x9A. Afterwards, ease
   D_80083160's three +0xA8..+0xAA colour bytes towards 0x80 by
   (0x80 - v) / +0x96 and, when the countdown runs out, raise 0x8000 in the
   node's -0x2 and in D_800814A0. 75 words, 0 differing, first compile.

   THE EASE IS func_800ACE34's (csrc/ovl_dungeon_800A4B88.c), with the
   divisor counting DOWN in the slot's +0x96 instead of up in the view's
   +0xF: the `lh' of +0x96 is re-read for each of the three bytes for the
   same reason its note gives -- each `sb' through the `u8 *' wipes cse's
   memory table.

   D_80083160 IS REACHED THROUGH A `u8 *' BASE TAKEN BEFORE THE GATE. Its
   `addiu' sits in the `bnez's delay slot although the only use is in the
   else arm, which is the materialise-up-front shape unk_108F0.c records for
   this object; the bytes at +0xA8 also have their own splat label
   (D_80083208) and are NOT spelled that way here.

   THE COUNTDOWN TEST READS THE DECREMENTED VALUE, not a reload: `lhu',
   `addiu -1', `sh', `sll 16', `bgtz' with no `sra' and no second load, which
   is `(s16)(--*(u16 *)(p + 0x96)) <= 0'. */
void func_b04_801749F4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (p[0x9A] == 0)
    {
        D_80175220 = *(s32 *)(c + 0x8);
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9A] += 1;
        }
    }
    else
    {
        g[0xA8] += (0x80 - g[0xA8]) / *(s16 *)(p + 0x96);
        g[0xA9] += (0x80 - g[0xA9]) / *(s16 *)(p + 0x96);
        g[0xAA] += (0x80 - g[0xAA]) / *(s16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* The bank's nine-state white-out cutscene, run off the slot's +0x9B: step
   the counter, hand func_80041588 the D_801751F8 command stream and its
   D_8017521C handshake byte, turn the actor to face 2, swap the HUD pair
   through func_8003F540/func_8003E4FC, open with func_800C77D0, ease
   D_80083160's three colour bytes while the D_8017521D handshake is pending,
   spawn the 0x12 node whose driver is func_b04_801749F4, fire
   func_b04_80174800 seven times, wait for the node's +0x14 to carry one of
   0xE000, then func_800ADC4C the replacement in, re-aim it and finally raise
   0x8000 in the node's -0x2 and in D_800814A0 and drop D_80083460's +0xA
   counter. 364 words, 0 differing, first compile.

   THIS FUNCTION OWNS jtbl_b04_801708B8, the 9-entry table at 0x801708B8
   (rodata offset 0xB0) for states 0..8. Every state has a label of its own,
   no two share an arm and there is no `default': the `sltiu $v0, $v1, 0x9'
   gate sends everything else to the epilogue. States 1, 2, 5 and 7 fall
   through to their successors. Its entries are, in order, the heads of
   states 0..8 at 0x80174B80, 0x80174B94, 0x80174BD8, 0x80174CB8, 0x80174D8C,
   0x80174EE0, 0x80174F4C, 0x80174FCC and 0x80175054.

   TWIN: the landed func_b06_80175270 (csrc/ovl_dng_b06.c, dngtwin 0.946,
   355 words). BYTE-IDENTICAL PROLOGUE -- `addiu $sp, $sp, -0x28', six saves
   $s0..$s4/$ra at 0x10..0x24 in the same order -- so its allocation facts
   were expected to carry, and ALL FOUR DID with none inverted:

     state 3's D_8017521D flag NOT read into a local before the +0x96 restore
       store                               365 words /  8 aligned  (twin: 8)
     state 6's `d - 0x20' given its own local rather than states 4/5/7's
       slot pointer                        364 words /  3 aligned  (twin: 14)
     D_80083160's `u8 *' base taken inside state 3 instead of before the gate
                                           365 words / 11 aligned  (twin: 19)
     state 5 reduced to a bump-only arm instead of the whole of state 6's
       body written out a second time      350 words / 19 aligned  (twin: 21)

   MEASURED AND FREE, so NOT the reason this lands, and a free fact that
   reproduces as a free fact: the position of `D_80175218 = -0x4' inside
   state 4. Written after `r = (u8 *)n->unk_0C' instead of before it, still
   364 / 0 -- the twin reported the same freedom for its own D_8017591C.

   THE DELTAS from it: state 1 clears +0xB0 where it clears +0xBA; state 4
   installs func_b04_801749F4 as the node's +0x10 driver and hands
   func_b04_80174574 to func_8004491C, PARKS THE NODE IN THE SLOT'S +0xAC
   rather than a global, writes -0x4 into D_80175218, picks out of D_80175200,
   and -- the nine words over its 355 -- ENDS IN A SEVEN-ITERATION LOOP over
   func_b04_80174800 in place of its single tail call; state 6's
   func_800ADC4C reads D_80175220/D_80175218.

   THAT LOOP'S COUNTER IS AN s16. The fourth argument is built as
   `sll $a3, $s0, 26' + `sra $a3, 16' and the bound as `sll'/`sra'/`slti 0x8',
   which is the s16 local's own sign-extension reused; as an `s32 i' the
   compiler drops both extensions and the loop runs on the full word:
   361 words / 10 aligned differences.

   MEASURED AND FREE: `i << 10' for the argument instead of `i * 0x400',
   still 364 / 0 -- unlike the brief's `* 640' case, a power-of-two multiply
   and the shift are the same tree by the time cc1psx sees them. */
void func_b04_80174B20(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *q;
    u8 *r;
    u8 *w;
    UnkNode *n;
    s32 t2;
    s32 v;
    s32 f;
    s16 t;
    s16 k;
    s16 i;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        p[0x9B] += 1;
        return;
    case 1:
        func_80041588(D_801751F8, (s8 *)D_8017521C, 0);
        *(s16 *)(p + 0xB0) = 0;
        p[0x9B] += 1;
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x2A);
        *(s32 *)(d + 0x1C) &= ~0x40000;
        /* fallthrough */
    case 2:
        t2 = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (D_8017521C[0] == 0 || t2 != 2)
        {
            if (t2 != 2)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            return;
        }
        func_80041588(D_801751F8, (s8 *)D_8017521C, 1);
        func_8003F540(0, D_8006CD58, 0x4000AD4, 0x5000CC4);
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        D_8017521D[0] = 0;
        func_8003E4FC(0xFF, D_8003E140, (s32)D_8017521D);
        func_800C77D0(d - 0x20, (u8 *)b, 8, 0x300);
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        /* fallthrough */
    case 3:
        t = *(u16 *)(p + 0x96);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            f = D_8017521D[0];
            *(s16 *)(p + 0x96) = t;
            if (f == 0)
            {
                return;
            }
            p[0x9B] += 1;
            func_800A56E0(0x300);
            return;
        }
        q = D_800DCEEC[func_800498A0(d)];
        g[0xA8] += (q[0] - g[0xA8]) / *(s16 *)(p + 0x96);
        g[0xA9] += (q[1] - g[0xA9]) / *(s16 *)(p + 0x96);
        g[0xAA] += (q[2] - g[0xAA]) / *(s16 *)(p + 0x96);
        return;
    case 4:
        n = func_8003FC64(0x12);
        if (n == NULL)
        {
            return;
        }
        n->unk_10 = (u32)func_b04_801749F4;
        func_8004491C(n, (u32)func_b04_80174574);
        *(UnkNode **)(p + 0xAC) = n;
        n->unk_08->unk_00 = b->unk_00;
        n->unk_08->unk_04 = b->unk_04;
        n->unk_08->unk_08 = b->unk_08;
        D_80175218 = -0x4;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u32 *)(r + 0xC) = 0x808080;
        k = func_800498A0(d) - 1;
        if (k < 0)
        {
            k = rand() % 3;
        }
        func_8003DB94(r, D_8014A000 + D_80175200[k], 0);
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        for (i = 1; i < 8; i++)
        {
            func_b04_80174800(p, b, c, i * 0x400);
        }
        return;
    case 5:
        n = *(UnkNode **)(p + 0xAC);
        r = (u8 *)n->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0xE000) != 0)
        {
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9B] += 1;
            func_800A18E8(d[0x13], 3);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            func_8009A028(d);
            r = d - 0x20;
            *(u32 *)(r + 0x10) |= 0x80000000;
        }
        /* fallthrough */
    case 6:
        if (p[0x9B] == 6 &&
            func_800ADC4C((u8 *)b, D_80175220, D_80175218, &D_800DCF5C) != NULL)
        {
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9B] += 1;
            func_800A18E8(d[0x13], 3);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            func_8009A028(d);
            r = d - 0x20;
            *(u32 *)(r + 0x10) |= 0x80000000;
        }
        return;
    case 7:
        v = func_800A504C(c, d);
        *(s32 *)(d + 0x60) = v;
        if (v == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B] += 1;
        *(s16 *)(*(u8 **)(d + 0x60) + 0x2A) = *(u16 *)(d + 0x2A);
        w = *(u8 **)(d + 0x60);
        r = *(u8 **)(w - 0x14);
        func_80047738(r,
                      (*(u8 **)(r + 0x2C))[((D_80083228 + *(s16 *)(w + 0x2A) + 0x100) >> 9) & 7],
                      *(s8 *)(r + 0x4));
        *(u16 *)(r + 0x14) &= ~1;
        /* fallthrough */
    case 8:
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0)
        {
            *(s16 *)(*(u8 **)(d + 0x60) + 0x2A) = *(u16 *)(d + 0x8A);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            d[0x6D] = 0;
        }
        return;
    }
}

/* 14 words, 0 differing. */
void func_b04_801750D0(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}

/* 14 words, 0 differing. */
void func_b04_80175108(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b04_80171400;
    }
}
