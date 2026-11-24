#include "common.h"

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u16 func_800A6D30(void);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_8009A180(u8 *a, u8 *b);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_b01_8017237C(u8 *p, void *b, void *c, u8 *d);
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

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b01_80171C34(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b01_801714D4(u8 *p, UnkVec *b, u8 *c, u8 *d);

s32 func_800AB778();

s32 func_800AD058();

s32 func_800AB464();

extern s16 D_80083228;
extern u8 D_80174148[];
extern u8 D_80174158[];
extern u8 D_80174160[];
void func_80047784(u8 *p, s32 b, s32 c);
void func_800A2B04(UnkVec *, s32, s32);
s32 func_800A2C34(u8 *);
void func_800A4ACC(u8 *p);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800AB538();
void func_800AD4D0(u8 *);
void func_800AD594(u8 *p, s32 a);
void func_b01_80173E00(u8 *p, u8 *b, u8 *c, u8 *d);

void func_b01_80171C34(u8 *p, void *b, void *c, u8 *d);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
extern u8 D_80083498[];
extern u8 D_80045340[];
extern u8 D_80174128[];
void func_800478B8(u8 *);
void func_b01_80171A18();
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, s32 t, void (*h)());
s16 func_800A1C58(u8 *p);
s16 func_8009FB34(u16 x, u16 y);
s32 func_b01_80172628(u8 *a, void *b, void *c, s32 d);
void func_b01_80172548(u8 *p, UnkVec *b, u8 *c, u8 *d);
extern u8 D_801740E0[];
extern u8 D_801740E8[];
extern u8 D_80174140[];
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
extern u8 D_801740F0[];
extern u8 D_801740F8[];
extern u8 D_80174150[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
typedef struct UnkDngB01Word48
{
    u32 w[0xC];
} UnkDngB01Word48;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_801708E8);

/* The node driver func_b01_80170A44 installs at the spawned node's +0x10:
   copy the three integer halves of the owning mover parked at +0xB0 into the
   node's own mover, push the slot's scale pair out by 0x320, fade the three
   greys in proportion to the +0x96 countdown against the +0xAA span, run the
   node call, and close the node when the countdown reaches zero. 53 words,
   0 differing. 2 copies (dngfam.py).

   THE TWIN IS THE RESIDENT func_800D6FEC (csrc/ovl_dungeon_800D50FC.c), the
   36-word driver of the same +0xB0 / +0x96 pair, which supplies the mover
   copy and the whole closing tail verbatim. Three deltas, all read off the
   listing: the mover halves are read `lhu' here where func_800D6FEC reads
   `lh', so the source lvalues are `u16'; the decrement is NOT folded into
   the test -- it happens before the fade and the tail RELOADS +0x96 with a
   fresh `lh', so `--' is spelled in the fade expression and the gate is a
   plain `<= 0'; and the fade and the 0x320 scale bump are this body's own.

   Landed on the first compile, so nothing on it is load-bearing by
   measurement. `* 80' is a constant multiply expand_mult synthesises as
   `sll 2'/`addu'/`sll 4' -- it is not a variable in a register here, unlike
   func_800D6DC0's 3. The counter is written `(s16)(--*(u16 *)(p + 0x96))'
   because that is what the listing reads literally -- `lhu' and then a
   `sll 16'/`sra 16' sign-extension before the multiply -- but
   `--*(s16 *)(p + 0x96)' MEASURES THE SAME, 53 words and 0 differing, so the
   width of that lvalue is free here.

   The two chained assignments are the resident overlay's own idiom
   (func_800D6DC0 writes its counter pairs the same way) and give the
   target's store order: the inner lvalue first, so +0x1E before +0x1C and
   +0xE before +0xD before +0xC. */
void func_b01_80170970(u8 *p, u8 *b, u8 *c)
{
    u8 *m;

    m = *(u8 **)(p + 0xB0);
    *(s16 *)(b + 0x2) = *(u16 *)(m + 0x2);
    *(s16 *)(b + 0x6) = *(u16 *)(m + 0x6);
    *(s16 *)(b + 0xA) = *(u16 *)(m + 0xA);
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x320;
    c[0xC] = c[0xD] = c[0xE] = (s16)(--*(u16 *)(p + 0x96)) * 80 / *(s16 *)(p + 0xAA);
    func_800478B8(c);
    if (*(s16 *)(p + 0x96) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The banked room script's SPAWNER for the mover-follower actor: allocate a
   0x112 node off D_80083498, seed the two 7-tick countdowns at +0x96 and
   +0xAA, install the bank-local func_b01_80170970 as the node's +0x10
   driver, park the caller's mover at +0xB0, COPY THE CALLER'S 0x30-BYTE
   RENDERABLE TEMPLATE over the slot, then overwrite the scale pair, the
   three 0x50 greys, +0x10, +0x12 and +0x22, add the 0xC bits, hand the node
   to func_8004491C/D_80045340, dress it from the bank-local D_80174128
   facing table and finally seed the node's own mover from the caller's.
   91 words, 0 differing. 2 copies (dngfam.py).
   No rodata of its own.

   THE TEMPLATE COPY GOES THROUGH A WORD-ALIGNED STRUCT, which is the
   resident func_800D6DC0's (csrc/ovl_dungeon_800D50FC.c) first named lever
   and is what this body was built from: that function is the same
   allocate / seed-w / install-+0x10 / copy-template / 0x10-and-0xC /
   func_8004491C / mover-halfwords / 0x1000-pair / grey-triple / dress
   skeleton, eight times over in a ring. A `u8 b[0x30]' struct or a `memcpy'
   is alignment 1 and cc1psx then emits the runtime `or'/`andi 3' test and an
   lwl/lwr path as well; `u32 w[0xC]' picks the aligned expansion off
   TYPE_ALIGN and gives the target's single four-register loop at
   .Lb01_80170AB0.

   Three deltas from that twin, all read off the listing: the grey triple and
   the 0x1000 scale pair run BEFORE func_8004491C here, not after it, and the
   three mover halfwords after the dress rather than before the pair; +0x12
   and +0x22 MUST BE WRITTEN THROUGH `u16' LVALUES, which is the only thing
   on this body that had to be measured -- the target loads both constants
   with `ori $zero', and through an `s16' lvalue the constant is truncated to
   -0x80 and -0xF at tree level and comes out as `addiu $zero' instead:
   91 words, 2 differing, those two words and nothing else. Measured and
   free at 91 words / 0 differing, so it is NOT the reason this lands: the
   two tail stores in the other order, `+0x22' before `+0x14 |= 0xC'. The
   order below is the one the listing reads literally, with the 0x22 store
   sitting in the `lhu 0x14' load-delay slot.

   THE FIRST PARAMETER IS UNUSED, exactly as in func_800D6DC0: nothing reads
   $a0 before `addiu $a0, $zero, 0x112' overwrites it, while $a1, $a2 and $a3
   all have prologue copies, so the arity is four with the first dead. */
void func_b01_80170A44(void *a, u8 *b, void *src, u8 *e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *w;
    u8 *r;

    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        w = (u8 *)n + 0x20;
        *(s16 *)(w + 0x96) = 7;
        *(s16 *)(w + 0xAA) = 7;
        n->unk_10 = (u32)func_b01_80170970;
        *(u8 **)(w + 0xB0) = b;
        r = (u8 *)n->unk_0C;
        *(UnkDngB01Word48 *)r = *(UnkDngB01Word48 *)src;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        r[0xE] = 0x50;
        r[0xD] = 0x50;
        r[0xC] = 0x50;
        *(s16 *)(r + 0x10) = 0x20;
        *(u16 *)(r + 0x12) = 0xFF80;
        *(u16 *)(r + 0x14) |= 0xC;
        *(u16 *)(r + 0x22) = 0xFFF1;
        func_8004491C(n, (u32)D_80045340);
        *(u8 **)(r + 0x2C) = D_80174128;
        func_80047784(r, D_80174128[((D_80083228 + *(s16 *)(e + 0x2A) + 0x100) >> 9) & 7], 0);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(b + 0xA);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80170BB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80170DA8);

/* The banked room script's IDLE handler -- the body parked at the actor's
   +0x8C by func_b01_80173AAC and func_b01_8017360C, and the one the switch
   below re-installs through func_800AAF00. OWNS A JUMP TABLE:
   jtbl_b01_80170808, twelve entries for the +0x46 state 1..12, with 4 and 11
   carrying the default label rather than arms of their own.
   337 words, 0 differing.

   THE TWIN IS THE LANDED func_b03_80170E54 (csrc/ovl_dng_b03.c), 338 words,
   and ALL THREE of its named facts are carried over here unchanged:

   (1) THE func_b01_80171C34 TAIL IS WRITTEN OUT AT ALL THREE SITES, NOT
   `goto'-ed. jump2 folds the three copies into the one block at
   .Lb01_80171944, but flow has already counted their references, and that is
   what wins `c' its allocno_compare race against the D_80082E80 pointer
   local in the d[0x6D] <= 0 block -- b03 measures the shared-label form at
   62 aligned differences, every one of them `p' and `c' exchanging $s0/$s1.
   (2) ONE TABLE LOCAL PER DRESS SITE, not one shared across the function.
   (3) ARM B's D_800814A8 IS A POINTER LOCAL ASSIGNED BEFORE THE d + 0x2A
   STORE, with the call result held in `m': the target fills the `lw's
   load-delay slot with that `sh', which needs the store between the `lw' and
   the `lbu'. b03 measures the two alternatives at 16 and 13 differing.

   Deltas from that twin, all read off the listing. The 0x1000 escape calls
   func_b01_80171A18 WITH NO ARGUMENTS AT ALL -- nothing writes $a0..$a3
   before the `jal', so the four incoming registers reach it untouched, which
   is why it is declared with empty parens. The +0x9A gate is its own
   statement here and the dress that follows it is a SEPARATE two-way table
   test (D_801740E0 or D_801740E8) whose body also sets c[5] and clears
   p + 0xA2 / p + 0x9E, where b03 nests one table test inside the +0x9A gate.
   `*(s32 *)(d + 0x1C) |= 0x40000' before `*(u16 *)(p + 0x98) &= ~8' is this
   copy's, and the mask is ~8 and not b03's ~0xC. The 0x80000 arm carries the
   same six-word p + 0x92 / p + 0xA2 / p + 0x9E block
   func_b01_80173AAC and func_b01_8017360C have, ahead of the bank-local
   func_b01_80173E00. The switch has case 10 with 8 and 9, func_800AAF00's
   fourth argument is 0 rather than a table, and its fifth is this function
   itself. And THIS BODY ENDS AT THE func_800A0818 STORE: b03's trailing
   D_80083462 / c + 0x14 / third-dress tail is absent, which is the 337
   against b03's 338.

   ONE SPELLING HAD TO BE MEASURED, AND IT IS A DELAY SLOT. The two-way
   table test must load c + 0x2C INTO A LOCAL (`f') AHEAD OF `e2', so that
   the block after the +0x9A gate OPENS WITH THE `lw'. The target leaves the
   `beq $v0, $v1' delay slot at 0x80171650 EMPTY, because a MEM cannot be
   moved into a conditional branch's delay slot (may_trap_p) and the `lui'
   for D_801740E0 sits behind the `lw'. Written `e2 = D_801740E0;' first and
   the load inside the comparison, that `lui' is what fills the slot: the
   body is 336 words -- one SHORT -- with 5 aligned differences, the `nop'
   and the displaced `lui' plus three branch rows that realign behind them.

   Landed with that one change. 1 copy (dngfam.py). */
void func_b01_801714D4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *f;
    s32 m;
    u8 *w;

    if ((D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b01_80171A18();
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80174158)
        {
            e = D_80174150;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80174158)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801740F0) != 0)
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
        f = *(u8 **)(c + 0x2C);
        e2 = D_801740E0;
        if (f != e2 && f != D_801740E8)
        {
            *(u8 **)(c + 0x2C) = e2;
            func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            c[5] = 1;
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80174140) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b01_80173E00(p, (u8 *)b, c, d);
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
            func_b01_80171C34(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b01_80172628(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b01_80171C34(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
        case 10:
            if ((s16)func_b01_8017237C(p, b, c, d) != 0)
            {
                return;
            }
            func_b01_80172548(p, b, c, d);
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
            func_800AAF00(p, b, c, 0, func_b01_801714D4);
            return;
        default:
            func_b01_80171C34(p, b, c, d);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80171A18);

/* The banked room script's walk step, the relinked twin of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c) with three extra
   pre-loop arms: the +0x400/+0x10 pair that re-aims through func_800A02AC or
   func_800A04F0, and the +0x2000/+0x20000 arm that compares the cursor
   against D_80082E80's. Everything after that is func_800D92C0's eight-step
   D_8006CD00 sweep. 466 words, 0 differing.

   THE SECOND PARAMETER IS NEVER READ IN THE BODY AND MUST STILL BE DECLARED
   AND PASSED ON. func_b01_8017237C's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A8C0 2 words, func_b01_8017237C 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b01_80171C34(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b01_8017237C(p, b, c, d) == 0)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_8017237C);

/* State 8..10's second half, reached from func_b01_801714D4's jump table
   once func_b01_8017237C has returned 0: clear d + 0x71's top bit, and --
   unless the engine is in its 0x2000 mode or func_800A2B5C refuses on either
   side of it -- push the record 0x300 through func_800C7930, reset the actor
   to state 0x11 with its +0x8C handler and both counters cleared, spend one
   of d + 0x6D, re-aim with func_8009C93C and leave 0x7C in d + 0x84.
   56 words, 0 differing, on the first compile.

   The resident func_800AAF00 (csrc/ovl_dungeon_800A4B88.c) supplies the two
   idioms this body needs -- `p[0x71] &= 0x7F' as its opening statement and
   `func_800C77D0(p - 0x20, b, 8, 0x300)' for the sibling of this function's
   func_800C7930 call -- but it is NOT the same body: the 0x7F mask and the
   0x300 push are on the FOURTH parameter here, not the first, and the
   0x11/0x8C/+0x6D/func_8009C93C tail is this one's own.

   Two things read off the listing rather than guessed: `func_800A2B5C' is
   called TWICE, once on each side of func_800C7930, and both results are
   tested as halfwords (`sll 16' with no `sra'); and the four actor stores
   come out 0x8C, 0x9A, 0x9B, 0x96, which is the order they are written in
   below. 1 copy (dngfam.py). No rodata of its own. */
void func_b01_80172548(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x11;
    p[0x9B] = 0;
    *(s16 *)(p + 0x96) = 0;
    d[0x6D] -= 1;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, NULL);
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
s32 func_b01_80172628(u8 *a, void *b, void *c, s32 d)
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
        func_b01_80171C34(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b01_80171C34(p, b, c, p);
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
   func_b01_801714D4. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b01_8017276C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b01_801714D4;
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
void func_b01_80172800(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80172864);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80172A48);

/* The banked room script's three-state glide, switching on p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it, state 2 steers onto the tile centre and installs
   func_b01_801714D4. The resident overlay's func_800D9C7C
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
   bank-local D_80174148 rather than 0. 181 words, 0 differing, on the first
   compile. */
void func_b01_80173294(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800AAA54(p, b, c, D_80174148);
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
        *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b01_80173568(void)
{
    func_800AD058();
}

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile. */
void func_b01_80173588(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b01_801735B4(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b01_801735D4(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}

/* The banked room script's six-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B. OWNS A JUMP TABLE:
   jtbl_b01_801708D0, six entries, one per case, no case falling to default.

   296 words, 0 differing. The twin is the landed func_b02_80173E50
   (csrc/ovl_dng_b02.c), whose case 1 is this body's case 3 almost verbatim:
   the same `func_80042900(d, 1) == 0' opening test, the same func_800AA79C
   arm behind the g + 0x2 0x2008 gate inside `d[0x25] == 0', the same
   D_80082E80 / func_8009FD40 / func_800A6D30 window reached past `again:',
   the same six-word p + 0x92 / p + 0xA2 / p + 0x9E block in the 0x80000 arm,
   and the dress/gate/bump tail WRITTEN OUT TWICE -- once in the opening
   test's `== 0' arm and once after `again:' -- which jump.c cross-jumps into
   the single block at this target's .Lb01_80173920. Read that comment and
   func_b01_80173AAC's above for the mechanism: a separate `&D_80083460'
   pointer local per arm, and the facing table named directly at every dress
   site rather than held in a shared `u8 *' local.

   Deltas from that twin, all read off the listing: five numbered states
   around that chain instead of two, so the three arms that dress and then
   bump (cases 1, 2 and 4) cross-join at .Lb01_801739C0 -- the `lui'/`addiu'
   of each one's own table stays in its own arm and only the suffix from
   `sw $a1, 0x2C($s1)' on is shared, which is why naming the table directly
   still produces one block; the bump pair in that shared tail is
   `p[0x9B] += 1' then `*(s16 *)(p + 0x96) = 0', and case 0's bare
   `p[0x9B] += 1' shares only the final `sb' with it at .Lb01_80173A00;
   `*(s32 *)(d + 0x1C) |= 0x40000' is case 4's, not the dress tail's; and
   cases 2 and 5 gate on a POST-INCREMENT of the p + 0x96 counter read
   UNSIGNED and compared SIGNED -- `lhu', `addiu 1', `sll 16'/`sra 16',
   `slti' -- with the `sh' landing in the branch delay slot, so the bump
   happens on both paths.

   Landed on the first compile. 2 copies (dngfam.py). */
void func_b01_8017360C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801740F8;
        func_80047784(c, D_801740F8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if (*(s16 *)(p + 0x92) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80174150;
        func_80047784(c, D_80174150[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        *(s16 *)(p + 0x96) = 0;
        return;
    case 2:
        if ((s16)(*(u16 *)(p + 0x96))++ < 2)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_80174158;
        func_80047784(c, D_80174158[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        *(s16 *)(p + 0x96) = 0;
        return;
    case 3:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_80174160;
            func_80047784(c, D_80174160[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
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
            func_b01_80173E00(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174160;
        func_80047784(c, D_80174160[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
            return;
        }
        p[0x9B] += 1;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        return;
    case 4:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u8 **)(c + 0x2C) = D_801740F8;
        func_80047784(c, D_801740F8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        *(s16 *)(p + 0x96) = 0;
        return;
    case 5:
        if ((s16)(*(u16 *)(p + 0x96))++ < 4)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801740F0;
        func_80047784(c, D_801740F0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_80174158 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b01_801714D4. The landed
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
   func_800AA888 and the bank-local func_b01_80173E00 -- p + 0x92 reduced by
   p + 0xA2 as halfwords (both reads `lhu', so both are unsigned), then
   p + 0xA2 and p + 0x9E cleared. p + 0x92 is READ BEFORE p + 0xA2 IS
   CLEARED in the listing, which is the order the three statements are
   written in. 213 words, 0 differing, on the first compile; the six-word
   block and the `~8' account for the 213 against the twin's 207. */
void func_b01_80173AAC(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_80174158;
        func_80047784(c, D_80174158[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_80174160;
            func_80047784(c, D_80174160[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b01_80173E00(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_80174160;
        func_80047784(c, D_80174160[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
        return;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b01", func_b01_80173E00);

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
void func_b01_80173F64(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b01_801714D4;
        }
    }
}

/* 14 words, 0 differing. */
void func_b01_80173FC8(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}

/* 14 words, 0 differing. */
void func_b01_80174000(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}

/* 14 words, 0 differing. */
void func_b01_80174038(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}

/* 14 words, 0 differing. */
void func_b01_80174070(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}

/* 14 words, 0 differing. */
void func_b01_801740A8(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b01_801714D4;
    }
}
