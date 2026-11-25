#include "common.h"

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b22_80172F58(u8 *p, void *b, void *c, u8 *d);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b22_801724BC();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AD058();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

extern s16 D_80083228;
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_b22_801720B4(u8 *p, UnkVec *b, u8 *c);
extern u8 D_8006CCF8[];
extern void (*D_80175EF4[])(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800478B8(u8 *);
void func_800A020C(s32, u8 *);
s32 func_800BCB04(s32, s32, s16);

s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
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

typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_b22_8017086C(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

s32 func_8009A180(u8 *a, u8 *b);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_b22_80173734(u8 *p, void *b, void *c, u8 *d);
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

s32 func_800644B8(s32);
typedef struct UnkDngB22Name12
{
    u8 b[0xC];
} UnkDngB22Name12;
UnkNode *func_8003FC64(s32);
extern u8 D_80045340[];
extern u8 D_80175EDC[];
void func_b22_80170EF4(u8 *p, u8 *b, u8 *c);
void func_80047784(u8 *, s32, s32);
extern s32 D_8008346C;
s32 func_800A56E0(s32);
s32 func_8003F270(void);
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
extern u8 D_80175E24[];
extern u8 D_80175E2C[];
extern u8 D_80175E34[];
void func_b22_80171790(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80175E3C[];
extern u8 D_80175E44[];
extern u8 D_80175E4C[];
void func_800A2B04(UnkVec *, s32, s32);
void func_800A9A04(u8 *);

extern u8 D_80175E9C[];
extern u8 D_80175EA4[];
extern u8 D_80175EAC[];
UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
u8 *func_8003DE58(u8 *, u8 *, void *, s16);
void func_b22_80171A38(u8 *p, UnkVec *b, u8 *c);
extern u8 D_80175EE8[];
extern u8 D_80175E54[];
extern u8 D_80175E5C[];
extern u8 D_80175E64[];
s32 func_80064584(s32);
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s16 func_800A45D8(u16, u16, s32);
typedef struct UnkDngB01Word48
{
    u32 w[0xC];
} UnkDngB01Word48;
extern u8 D_80175EB4[];
extern u8 D_80175EBC[];
extern u8 D_800DDC40[];
extern u8 D_b22_80175DC4[];
extern u8 D_b22_80175DCC[];
extern u8 D_80175DD4[];
void func_b22_80173678(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_800AC82C();

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_8017086C);

/* Walk the tag chain from `p` and hand every node to func_b22_8017086C with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_b22_80170CEC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_b22_8017086C(p, v, s, 0);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* The grey-decay node driver installed at a spawned node's +0x10 by
   func_b22_8017121C: knock 0x40 off the record's three colour bytes, publish
   them into the slot's +0xC/+0xD/+0xE, and when the +0x18 countdown runs out
   raise 0x8000 in the node's -0x2 and in D_800814A0.  33 words, 0 differing.
   A leaf -- no frame, no `$ra' save.

   The twin is the landed func_b22_80170DC4 one listing up in this unit (the
   same retire pair and the same +0xC..+0xE triple); only the fade differs.

   THE ONE FACT IS THE ORDER OF THE THREE DECREMENTS, AND IT IS DESCENDING:
   p[2], p[1], p[0].  Ascending is 34 words / 13 aligned differences -- one
   word longer with three reloads instead of two.  The mechanism is cse: the
   three `c' stores go through a DIFFERENT register base, so the first of them
   invalidates every `p' entry in cse's table, and only the byte decremented
   LAST still has its value in hand when `c[0xC] = p[0]' is reached.  So
   p[0] forwards (the `sb $v1, 0xC($a2)' with no load) and p[1] and p[2]
   reload -- which is only possible if p[0] is the last decrement.  The
   emitted store order p[2], p[0], p[1] is sched1 transposing two independent
   adjacent `sb's and is NOT the source order.

   The countdown is the FOLDED form, `(s16)(--*(u16 *)(p + 0x18)) <= 0' with
   one `lhu' and no reload -- the opposite polarity from func_b22_80170DC4
   below, which reads +0x96 back for its gate.  Here there is only one read.
   The second parameter is never used and still has to be declared, because
   the slot pointer arrives in $a2. */
void func_b22_80170D40(u8 *p, u8 *b, u8 *c)
{
    p[2] -= 0x40;
    p[1] -= 0x40;
    p[0] -= 0x40;
    c[0xC] = p[0];
    c[0xD] = p[1];
    c[0xE] = p[2];
    if ((s16)(--*(u16 *)(p + 0x18)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node driver installed at a spawned node's +0x10: fade the slot's three
   grey bytes in proportion to the +0x96 countdown against the +0x9E span,
   and when the countdown runs out raise 0x8000 in the node's -0x2 and in
   D_800814A0 so the engine reaps it. 26 words, 0 differing. A leaf -- no
   frame, no `$ra' save -- and the `addu $a1, $a0, $zero' is just `p' losing
   $a0 to the divisor.

   TWO TWINS. The body is the landed func_b01_801708E8's
   (csrc/ovl_dng_b01.c, 34 words) with its func_800478B8 call absent and the
   span read from +0x9E rather than +0xAA; the COUNTDOWN is
   func_b01_80170970's idiom instead -- the `--' spelled inside the fade and
   the gate a plain `<= 0' off a FRESH `lh' of +0x96.

   b01_801708E8's ONE STATED FACT INVERTS, AND NOT TO THE SPELLING IT
   REJECTED. Its rule is that the chain be written
   `c[0xC] = c[0xD] = c[0xE] = ...' so the stores run +0xE, +0xD, +0xC; this
   target stores +0xC, +0xE, +0xD, which is a THIRD order neither b01 nor
   func_s67_800247C0 measured, and it comes from
   `c[0xD] = c[0xE] = c[0xC] = ...' (inner lvalue first, so +0xC, then +0xE,
   then +0xD). Measured here: b01's own order is 26 words / 2 aligned
   differences, and `c[0xE] = c[0xD] = c[0xC]' is 26 / 2 as well -- the same
   two transposed `sb's either way, which is why the residue cannot pick
   between them and the listing's store order has to.

   THE RELOAD IS LOAD-BEARING: folding the decrement into the gate instead,
   b01_801708E8's `(s16)(--*(u16 *)(p + 0x96)) <= 0' with the fade reading a
   plain +0x96, is 26 words / 14 aligned differences.

   MEASURED AND FREE, so neither is the reason this lands: `(x << 7) / y' in
   place of `x * 128 / y' (26 / 0 -- the power-of-two multiply is the same
   tree) and dropping the `(s16)' by decrementing through an `s16 *' lvalue
   (26 / 0). The second parameter is never read and still has to be declared,
   because the slot pointer arrives in $a2. */
void func_b22_80170DC4(u8 *p, u8 *b, u8 *c)
{
    c[0xD] = c[0xE] = c[0xC] = (s16)(--*(u16 *)(p + 0x96)) * 128 / *(s16 *)(p + 0x9E);
    if (*(s16 *)(p + 0x96) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The proportional-fade node driver: scale each of the record's three colour
   bytes by the +0x18 countdown over the +0x1A span into the slot's
   +0xC/+0xD/+0xE, then retire through the node's -0x2 and D_800814A0 when the
   countdown reaches 0.  50 words, 0 differing, first compile.  A leaf.

   The shape is func_b22_80170DC4's (above) with the per-channel multiply
   repeated three times instead of one chained store, which is why the `div'
   runs three times: the divisor is a variable so expand_divmod cannot
   strength-reduce it, and each `c' store invalidates the +0x18 and +0x1A
   entries so both halfwords are re-read per channel.

   MEASURED AND FREE, so it is NOT the reason this lands: the `--' written as
   its own statement ahead of the first fade instead of folded into it
   (`*(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - 1;' then a plain read) is
   50 words / 0 aligned differences -- cse forwards the stored halfword to the
   first fade either way.  func_b22_80170DC4 prices the MIRROR choice -- its
   `--' moved out of the fade and into the GATE -- at 26 words / 14 aligned
   differences, so the position of the decrement is priced there and free
   here. */
void func_b22_80170E2C(u8 *p, u8 *b, u8 *c)
{
    c[0xC] = p[0] * (s16)(--*(u16 *)(p + 0x18)) / *(s16 *)(p + 0x1A);
    c[0xD] = p[1] * *(s16 *)(p + 0x18) / *(s16 *)(p + 0x1A);
    c[0xE] = p[2] * *(s16 *)(p + 0x18) / *(s16 *)(p + 0x1A);
    if (*(s16 *)(p + 0x18) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node driver func_b22_80171928 installs at its 0x212 node's +0x10: grow
   the slot's scale pair by 0x78 and its +0x1A by 0x2BC every frame, and once
   the +0x18 countdown is under 0xC ramp the three grey bytes down with
   `k * 128 / 12'; retire through -0x2 and D_800814A0 when it reaches 0.
   40 words, 0 differing, first compile.  A leaf.

   THE PROTOTYPE OF THIS FUNCTION WAS WIDENED from `(u8 *p)' to
   `(u8 *p, u8 *b, u8 *c)'.  The slot arrives in $a2, so three parameters have
   to be declared; the only pre-existing reference is the
   `(u32)func_b22_80170EF4' address-take in the landed func_b22_80171928
   below, so no call site changed and all 33 defined bodies in the unit
   re-verify at 0 differing.

   TWO STORE-ORDER FACTS, BOTH CHAINS AND BOTH PRICED HERE.  gcc emits an
   assignment chain INNER LVALUE FIRST, so:
     `*(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E) += 0x78' gives +0x1E then +0x1C;
     the other way round is 40 words / 3 aligned differences.
     `c[0xC] = c[0xD] = c[0xE] = v' gives +0xE, +0xD, +0xC; `c[0xE] = c[0xD] =
     c[0xC] = v' is 40 words / 4.
   (This is the same rule the landed func_b22_80170DC4 records, read from the
   chain side rather than the separate-statement side.)

   THE DIVISOR IS 12 AND THE MAGIC CONSTANT DOES NOT SAY SO: 0x2AAAAAAB with
   `mfhi' alone is /6, and the `sra 1' after it makes it /12.

   The tail gate is a FRESH `lh' of p+0x18 and not the local `k', because the
   three grey `sb's go through the slot pointer and invalidate cse's entry for
   the counter -- `if (k <= 0)' would reuse the register and lose the load. */
void func_b22_80170EF4(u8 *p, u8 *b, u8 *c)
{
    s16 k;

    *(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E) += 0x78;
    *(u16 *)(c + 0x1A) += 0x2BC;
    k = (s16)(--*(u16 *)(p + 0x18));
    if (k < 0xC)
    {
        c[0xC] = c[0xD] = c[0xE] = k * 128 / 12;
    }
    if (*(s16 *)(p + 0x18) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 particle's three-axis integrator: add the payload's
   +0x40/+0x44/+0x48 velocity words into the mover, accelerate each by
   +0x4C/+0x50/+0x54, back the x and y steps out when func_800A45D8 reports
   the new cell blocked, clamp +0xA to func_800BCB04's ground height (arming
   +0xE and clearing +0x18 on first contact), re-send the view through
   func_800478B8 every fourth frame on the +0x1A counter, ease the slot's
   scale pair up to 0x1000 by 0x64 a frame, and raise 0x8000 in the node's
   -0x2 and in D_800814A0 when the +0x18 life runs out or the slot reports
   0x8000.  162 words, 0 differing, FIRST COMPILE.

   The donor is the landed func_b42_80170B48 (csrc/ovl_dng_b42.c, 162 words,
   dngtwin 0.951) and it is a known-correct donor by construction -- same
   family.  The whole delta is five offsets: the velocities and accelerations
   sit 0x10 lower (0x40/0x44/0x48 and 0x4C/0x50/0x54 against b42's 0x50.. and
   0x5C..), the contact flag is +0xE where b42 has +0x14, the life is +0x18
   where b42 has +0x1E and the frame counter +0x1A where b42 has +0x20.
   Nothing else moved; read b42's comment for the statements.

   B42'S ONE PRICED FACT HOLDS AND REPRODUCES TO THE DIGIT: the ground-height
   local must be `s32 h' and not `s16 h' -- 163 words / 5 aligned differences
   as `s16', which is b42's own figure exactly.  A 16-bit local is a HImode
   pseudo, so cse derives it from the `lhu' the func_800BCB04 argument already
   needs and pays a `sll'/`sra' to sign-extend it again, where the target has
   an independent `lh $s0, 0xA($s1)' beside the argument's `lhu $a2'.
   `func_800BCB04h' is the s16-returning unit-local alias for the same
   routine, as in b42; the unit's landed `s32 func_800BCB04' declaration is
   untouched and still serves func_b22_80172F58. */
void func_b22_80170F94(u8 *p, UnkVec *b, u8 *c)
{
    s32 h;

    b->unk_00 += *(s32 *)(p + 0x40);
    *(s32 *)(p + 0x40) += *(s32 *)(p + 0x4C);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x40);
        *(s32 *)(p + 0x40) = 0;
        *(s32 *)(p + 0x4C) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x44);
    *(s32 *)(p + 0x44) += *(s32 *)(p + 0x50);
    if (func_800A45D8(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA)) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x44);
        *(s32 *)(p + 0x44) = 0;
        *(s32 *)(p + 0x50) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x48);
    *(s32 *)(p + 0x48) += *(s32 *)(p + 0x54);
    h = *(s16 *)((u8 *)b + 0xA);
    if (func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                       *(u16 *)((u8 *)b + 0xA) - 4) -
            0x10 <
        h)
    {
        *(s32 *)(p + 0x48) = 0;
        *(s16 *)((u8 *)b + 0xA) = func_800BCB04h(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                                 *(u16 *)((u8 *)b + 0xA) - 4) -
                                  0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        if (*(s16 *)(p + 0xE) == 0)
        {
            *(s16 *)(p + 0xE) = 1;
            *(s16 *)(p + 0x18) = 0;
        }
    }
    *(s16 *)(p + 0x1A) = *(u16 *)(p + 0x1A) + 1;
    if ((*(u16 *)(p + 0x1A) & 3) == 0)
    {
        func_800478B8(c);
    }
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x64;
    if (*(u16 *)(c + 0x1C) > 0x1000)
    {
        *(s16 *)(c + 0x1C) = 0x1000;
    }
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x64;
    if (*(u16 *)(c + 0x1E) > 0x1000)
    {
        *(s16 *)(c + 0x1E) = 0x1000;
    }
    *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - 1;
    if (*(s16 *)(p + 0x18) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 trail spawn: allocate a node, arm its +0x38/+0x3A pair at 2,
   install func_b22_80170D40 as the step and func_b22_80170CEC as the
   func_8004491C descriptor, clear the slot's 0xC bits, seed the mover from
   the caller's and offset it by func_8003DE58's projection (or drop z by 0x14
   when the projection fails), give the slot a 0x1000 scale pair and a 0xA0
   grey mirrored into the payload's own three bytes, and finally aim the
   payload's +0x58..+0x62 velocity block either half-way to the +0x60 target's
   position less the D_80175EBC facing offset, or half-way to the actor's own
   |+0x72|,|+0x73| tile centre.  184 words, 0 differing.

   A CONSTANT'S SIDE IN A SUBTRACTION DECIDES WHICH REGISTER IT LANDS ON, and
   only one of three spellings reaches 0.  All at 184 words:

     | `(x * 64 + 0x20 - m2) / 2'    KEPT   | 0 rows | `addiu -0x20' on the
     |                                      |        |   LOADED value, `subu'
     | `(x * 64 - m2 + 0x20) / 2'           | 4 rows | `subu', then `addiu
     |                                      |        |   +0x20' on the product
     | `(x * 64 - (m2 - 0x20)) / 2'         | 7 rows | `addiu +0x20' on the
     |                                      |        |   product, then `subu'
     | the subtrahend in an `s32' local     | 186 words / 18 rows            |

   fold-const.c:4196 `associate' tries `split_tree (arg0)' FIRST, and
   split_tree (:1008) splits only a PLUS or a MINUS.  With `arg0 = VAR + CON'
   it succeeds and rebuilds the expression as `VAR - (ARG1 - CON)' (:4234),
   moving the constant onto the subtrahend.  With a bare MULT as arg0 it fails
   there, splits `arg1 = load - CON' instead and builds `(VAR + CON) - load'
   (:4278).  So `A + K - B' and `A - (B - K)' are DIFFERENT CODE rather than
   two spellings of one tree, and the diagnostic is an `addiu' on the wrong
   register with nothing else moved.  A local does not help: split_tree strips
   only mode-preserving conversions, so the VAR_DECL is still reached through
   its definition, and the two extra words are unfillable delay slots plus a
   register shift on an unrelated local.

   `s16 x' and `s16 y' for the two abs'd bytes: HImode is what makes the
   `* 64' come out `sll 16'/`sra 10' and the `y < 0' test `sll 16'/`bgez'.
   The same `(g2 - m2 - tbl * 16) / 2' in the first arm needs no such care --
   its first operand is a signed load, so convert.c leaves it alone.

   MEASURED AND FREE, so it is NOT the reason this lands: `s16 out[3]' in
   place of `u16 out[3]' is 184 words / 0 -- the three sums are narrowed to
   unsigned short either way, so the array's declared signedness is invisible
   at these sites.  The +0x60/+0x5C/+0x58 zero triple is written descending as
   in func_b22_80171790 above, which prices ascending at 4 rows; it was not
   re-priced here. */
void func_b22_8017121C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    u8 *m;
    u8 *g;
    u8 *t;
    s16 x;
    s16 y;
    u16 out[3];

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x18) = 2;
        *(s16 *)(q + 0x1A) = 2;
        n->unk_10 = (u32)func_b22_80170D40;
        func_8004491C(n, (u32)func_b22_80170CEC);
        r = (u8 *)n->unk_0C;
        *(u16 *)(r + 0x14) &= 0xFFF3;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        t = *(u8 **)(p - 0x14);
        if (func_8003DE58(*(u8 **)(t + 0x8), t, out, 0) != NULL)
        {
            *(s16 *)(m + 0x2) = *(u16 *)(m + 0x2) + out[0];
            *(s16 *)(m + 0x6) = *(u16 *)(m + 0x6) + out[1];
            *(s16 *)(m + 0xA) = *(u16 *)(m + 0xA) + out[2];
        }
        else
        {
            *(s16 *)(m + 0xA) = *(u16 *)(m + 0xA) - 0x14;
        }
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
        r[0xE] = 0xA0;
        r[0xD] = 0xA0;
        r[0xC] = 0xA0;
        q[0] = r[0xC];
        q[1] = r[0xD];
        q[2] = r[0xE];
        if (*(u8 **)(d + 0x60) != NULL && (*(s32 *)(d + 0x14) & 0x4000000) == 0)
        {
            *(s16 *)(q + 0x60) = 0;
            *(s16 *)(q + 0x5C) = 0;
            *(s16 *)(q + 0x58) = 0;
            g = *(u8 **)(*(u8 **)(d + 0x60) - 0x18);
            *(s16 *)(q + 0x5A) =
                (*(s16 *)(g + 0x2) - *(s16 *)(m + 0x2) -
                 ((UnkDngB09A28 *)((u8 *)D_80175EBC + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_00 *
                     16) /
                2;
            *(s16 *)(q + 0x5E) =
                (*(s16 *)(g + 0x6) - *(s16 *)(m + 0x6) -
                 ((UnkDngB09A28 *)((u8 *)D_80175EBC + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_02 *
                     16) /
                2;
            *(s16 *)(q + 0x62) = (*(s16 *)(g + 0xA) - *(s16 *)(m + 0xA) -
                                  D_800DDC40[(*(u8 **)(d + 0x60))[0x13]] / 2) /
                                 2;
        }
        else
        {
            *(s16 *)(q + 0x60) = 0;
            *(s16 *)(q + 0x5C) = 0;
            *(s16 *)(q + 0x58) = 0;
            x = *(s8 *)(d + 0x72);
            y = *(s8 *)(d + 0x73);
            if (x < 0)
            {
                x = -x;
            }
            if (y < 0)
            {
                y = -y;
            }
            *(s16 *)(q + 0x5A) = (x * 64 + 0x20 - *(s16 *)(m + 0x2)) / 2;
            *(s16 *)(q + 0x5E) = (y * 64 + 0x20 - *(s16 *)(m + 0x6)) / 2;
            *(s16 *)(q + 0x62) = 0;
        }
    }
}

/* The 0x112 ghost spawn, fired when the actor has a +0x60 target or a
   negative +0x72: allocate off D_80083498, arm the payload's +0x96 countdown
   and +0x9E span at 0xA, install func_b22_80170DC4 as the step, copy the
   caller's 0x30-byte renderable template over the new slot, overwrite the
   scale pair and the grey triple, clear the 0xC bits, dress from D_80175EB4,
   and then place the node's mover either on the target's position minus the
   D_80175EBC facing offset and half the D_800DDC40 height for its kind, or --
   when there is no live target -- on the actor's own |+0x72|,|+0x73| tile
   centre with func_8003DE58's projected z or 0x28 below the caller's.
   165 words, 0 differing.

   TWO SITES OF convert.c's LSHIFT_EXPR GUARD IN ONE BODY, AND THE FIX AT BOTH
   IS A MULTIPLY.  In a narrowing store (`*(s16 *)dst = <load> - <shift>')
   convert_to_integer distributes the truncation into the sum, and its
   LSHIFT_EXPR arm is guarded on `TREE_UNSIGNED (type)' while MULT_EXPR's is
   not -- so a shift makes the whole subtree unsigned short and flips the
   signed loads beside it:
     | `->unk_00 << 4' for the D_80175EBC field    | 165 words |  1 row  |
     |   ... the diagnostic: `lhu' for the target's `lh', nothing else moved |
     | `(*(s8 *)(d + 0x72) << 6)' for the tile     | 166 words |  6 rows |
     |   ... `lbu' plus a merged `sll 24'/`sra 18' for the target's
     |       `lb'/`sll 6' -- and two words longer for the two extra shifts   |
   With both shifts in place the body was 167 words / 11.  `* 16' and `* 64'
   land it; the trees are otherwise identical.

   THE 0x30-BYTE SLOT COPY NEEDS `UnkDngB01Word48' (`u32 w[0xC]'), NOT
   `UnkSlot': UnkSlot is 0x2C bytes, so cc1psx emits a two-iteration loop plus
   a 12-byte tail where the target has one three-iteration loop and
   `addiu $t0, $a2, 0x30'.  csrc/ovl_dng_b24.c's func_b24_801708B8 has the
   mechanism and csrc/ovl_dng_b25.c prices the `u8 w[0x30]' spelling at 105
   differing; neither was re-measured here.

   `*(u8 **)(d + 0x60)' is written INLINE at all three sites -- the gate, the
   `- 0x18' owner fetch and the D_800DDC40 index -- which is what
   csrc/ovl_dng_s20.c's func_s20_80024064 prices at 9 rows for the hoisted
   form.  cse unifies the first two for free and the third reloads because the
   mover stores between them invalidate it. */
void func_b22_801714FC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    u8 *m;
    u8 *g;
    u8 *t;
    u16 out[3];

    if (*(u8 **)(d + 0x60) != NULL || *(s8 *)(d + 0x72) < 0)
    {
        n = func_8003FD64(0x112, D_80083498);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x96) = 0xA;
            *(s16 *)(q + 0x9E) = 0xA;
            n->unk_10 = (u32)func_b22_80170DC4;
            *(s16 *)(q + 0x94) = *(u16 *)(d + 0x2A);
            r = (u8 *)n->unk_0C;
            *(UnkDngB01Word48 *)r = *(UnkDngB01Word48 *)c;
            *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = 0x1000;
            r[0xE] = 0x80;
            r[0xD] = 0x80;
            r[0xC] = 0x80;
            *(u16 *)(r + 0x14) &= 0xFFF3;
            func_8004491C(n, (u32)D_80045340);
            *(u8 **)(r + 0x2C) = D_80175EB4;
            func_80047784(r, D_80175EB4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            m = (u8 *)n->unk_08;
            if (*(u8 **)(d + 0x60) != NULL && (*(s32 *)(d + 0x14) & 0x4000000) == 0)
            {
                g = *(u8 **)(*(u8 **)(d + 0x60) - 0x18);
                *(s16 *)(m + 0x2) =
                    *(u16 *)(g + 0x2) -
                    (((UnkDngB09A28 *)((u8 *)D_80175EBC + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))
                         ->unk_00 *
                     16);
                *(s16 *)(m + 0x6) =
                    *(u16 *)(g + 0x6) -
                    (((UnkDngB09A28 *)((u8 *)D_80175EBC + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))
                         ->unk_02 *
                     16);
                *(s16 *)(m + 0xA) = *(u16 *)(g + 0xA) - D_800DDC40[(*(u8 **)(d + 0x60))[0x13]] / 2;
            }
            else if (*(s8 *)(d + 0x72) < 0)
            {
                d[0x72] = -d[0x72];
                if (*(s8 *)(d + 0x73) < 0)
                {
                    d[0x73] = -d[0x73];
                }
                *(s16 *)(m + 0x2) = *(s8 *)(d + 0x72) * 64 + 0x20;
                *(s16 *)(m + 0x6) = *(s8 *)(d + 0x73) * 64 + 0x20;
                *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA);
                t = *(u8 **)(p - 0x14);
                if (func_8003DE58(*(u8 **)(t + 0x8), t, out, 0) != NULL)
                {
                    *(s16 *)(m + 0xA) = *(u16 *)(m + 0xA) + out[2];
                }
                else
                {
                    *(s16 *)(m + 0xA) = *(u16 *)(m + 0xA) - 0x28;
                }
            }
        }
    }
}

/* The 0x212 spark spawn: allocate a node, arm the payload's +0x38/+0x3A pair
   at 6, install func_b22_80170E2C as the step and func_b22_80170CEC as the
   func_8004491C descriptor, set the slot's +0x10 and the 0xC bits, seed the
   mover from the caller's with the third component pulled 0x50 back, then
   give the slot a 0x1000 scale pair and three RANDOM grey bytes, mirror those
   three bytes into the payload's own +0x0/+0x1/+0x2, zero the +0x58/+0x5C/
   +0x60 triple and finally spin a random direction: one rand picks a 0xFFF
   angle whose sine and cosine scale the record's +0xAE radius into +0x5A and
   +0x62, and a second rand's pair scales that result again into +0x5A and
   +0x5E.  102 words, 0 differing, first compile.

   func_80069EF8 IS `rand' (splat/common_syms.txt) and needs no declaration.

   THE ONE FACT PRICED HERE IS THE ZERO TRIPLE'S ORDER, AND IT IS DESCENDING:
   q+0x60, q+0x5C, q+0x58.  Ascending is 102 words / 4 aligned differences.
   These are three SEPARATE statements, so they emit in SOURCE order -- the
   opposite half of the rule from func_b22_80170EF4's chains above, where the
   innermost lvalue emits first.  (func_b22_8017121C below writes the same
   triple the same way round.)

   The `q' local off n+0x20 is func_b22_80171928's, and the second
   `r = n->unk_0C' re-read is func_b22_80171BEC's; neither was re-priced here.
   `*(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x50' keeps its `addiu
   -0x50': convert.c's narrowing distribution needs two or more non-constant
   operands and there is only one here. */
void func_b22_80171790(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *m;
    UnkSlot *r;
    s32 k;
    s32 t;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x18) = 6;
        *(s16 *)(q + 0x1A) = 6;
        n->unk_10 = (u32)func_b22_80170E2C;
        func_8004491C(n, (u32)func_b22_80170CEC);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x50;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1C = r->unk_1E = 0x1000;
        *((u8 *)r + 0xC) = rand();
        *((u8 *)r + 0xD) = rand();
        *((u8 *)r + 0xE) = rand();
        q[0] = *((u8 *)r + 0xC);
        q[1] = *((u8 *)r + 0xD);
        q[2] = *((u8 *)r + 0xE);
        *(s16 *)(q + 0x60) = 0;
        *(s16 *)(q + 0x5C) = 0;
        *(s16 *)(q + 0x58) = 0;
        k = rand() & 0xFFF;
        t = (*(s16 *)(p + 0xAE) * func_80064584(k)) >> 12;
        *(s16 *)(q + 0x62) = (*(s16 *)(p + 0xAE) * func_800644B8(k)) >> 12;
        k = rand() & 0xFFF;
        *(s16 *)(q + 0x5A) = (t * func_80064584(k)) >> 12;
        *(s16 *)(q + 0x5E) = (t * func_800644B8(k)) >> 12;
    }
}

/* The 0x212 node spawn func_b22_8017472C's state 2 fires when its +0x96
   timer reads 0x16: allocate the node, arm the payload's +0x38/+0x3A pair at
   0x1C, install func_b22_80170EF4 as the node's +0x10 step, hand it to
   func_8004491C/D_80045340, put 0x60 in the slot's +0x10 and raise +0x14's
   0xC bits, seed the node's own mover from the CALLER's mover with the third
   component pulled 0x500000 back, set the scale pair to 0x190 and the three
   colour bytes to 0x80, then copy the 12-byte D_80175EDC record into the
   node's own +0x44 and point the slot's +0x8 at it.
   68 words, 0 differing, first compile.

   The twin is the landed func_s65_800243E4 (csrc/ovl_dng_s65.c, 67 words,
   dngtwin 0.904) and BOTH of its shape facts hold, re-measured here: the
   mover copy is THREE ALIGNED WORDS -- `m' and the source are unrelated
   pointers gcc cannot prove disjoint, so each `sw' pins the next `lw' and
   the three load-delay slots stay `nop' -- and the 12-byte record copy goes
   through a struct whose type has ALIGNMENT 1 (`u8 b[0xC]'), which is what
   forces the `lwl'/`lwr' + `swl'/`swr' pairs. Spelled as three aligned word
   stores instead, the record copy is 64 words / 30 aligned differences.

   TWO FACTS OF ITS OWN, both measured on this body:
     the SECOND `r = (UnkSlot *)n->unk_0C;' before the scale/colour block is
     what keeps the second `lw 0xC($s0)'; with one `r' it is 67 words / 27
     aligned differences;
     the `q' local for the +0x38/+0x3A pair is load-bearing -- written direct
     as `*(s16 *)((u8 *)n + 0x38)' it is 67 words / 4.

   MEASURED AND FREE, so neither is the reason this lands: the third mover
   component written `+ 0xFFB00000' instead of `- 0x500000' (68 / 0), and the
   mover copy spelled `*(s32 *)((u8 *)b + N)' instead of `b->unk_NN' (68 / 0).

   DELTAS FROM s65, all read off the listing: no `r->unk_06' store, the
   payload pair is +0x38/+0x3A at one constant rather than an owner pointer
   and a tick count, the mover source is the SECOND PARAMETER rather than
   `*(u8 **)(a - 0x18)', the scale is 0x190 and the colour 0x80 where s65
   zeroes both, and the record lands at +0x44. The first and third parameters
   are never read and still have to be declared and accepted, because
   func_b22_8017472C calls this with three arguments. */
void func_b22_80171928(u8 *a, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x18) = 0x1C;
        *(s16 *)(q + 0x1A) = 0x1C;
        n->unk_10 = (u32)func_b22_80170EF4;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08 - 0x500000;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x190;
        r->unk_1C = 0x190;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *(UnkDngB22Name12 *)((u8 *)n + 0x44) = *(UnkDngB22Name12 *)D_80175EDC;
        r->unk_08 = (u32)((u8 *)n + 0x44);
    }
}

/* The follower node's per-frame step, parked at +0x10 by func_b22_80171BEC:
   reach the owner through the node's +0x40 back-pointer, retire with it when
   its +0x1E has 0x8000, otherwise track its slot -- +0x6 one above the
   owner's, a scale pair chosen from the owner's +0x4 kind, the owner's mover
   mirrored into this node's and offset by func_8003DE58's projection -- and
   retire anyway unless the owner's facing table is one of D_80175E54,
   D_80175E5C or D_80175E64.  109 words, 0 differing, first compile.

   THE FIVE KIND TESTS ARE FIVE INDEPENDENT `if's, EACH RE-READING
   `*(s8 *)(r + 0x4)'.  Written as an `else if' chain the body is 98 words /
   40 aligned differences -- eleven words short, because the chain's jumps
   replace the fall-throughs the target has.  The reloads are not source:
   each scale store goes through the slot pointer and invalidates cse's entry
   for the kind byte, so the tests that follow a TAKEN arm reload and the ones
   that follow an untaken arm do not.  Kinds 0 and 4 both set 0x400 and kinds
   1 and 3 both set 0x700; they are still five separate tests.

   The retire block is SHARED between the +0x1E gate at the top and the
   three-table test at the bottom, and the way to get that is to put the
   table test's success arm on a `return' and let the big `if' fall THROUGH
   into the retire -- reorg rebuilds the shared block by itself, with no
   `goto' and no duplicated arm. */
void func_b22_80171A38(u8 *p, UnkVec *b, u8 *c)
{
    u8 *o;
    u8 *r;
    u8 *m;
    u8 *t;
    u16 out[3];

    o = *(u8 **)(p + 0x20) - 0x20;
    if ((*(u16 *)(o + 0x1E) & 0x8000) == 0)
    {
        r = *(u8 **)(o + 0xC);
        m = *(u8 **)(o + 0x8);
        *(s16 *)(c + 0x6) = *(u16 *)(r + 0x6) + 1;
        if (*(s8 *)(r + 0x4) == 0)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x400;
        }
        if (*(s8 *)(r + 0x4) == 1)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x700;
        }
        if (*(s8 *)(r + 0x4) == 2)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0xA00;
        }
        if (*(s8 *)(r + 0x4) == 3)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x700;
        }
        if (*(s8 *)(r + 0x4) == 4)
        {
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x400;
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(m + 0xA);
        t = *(u8 **)(o + 0xC);
        if (func_8003DE58(*(u8 **)(t + 0x8), t, out, 0) != NULL)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        if (*(u8 **)(r + 0x2C) == D_80175E54 || *(u8 **)(r + 0x2C) == D_80175E5C ||
            *(u8 **)(r + 0x2C) == D_80175E64)
        {
            return;
        }
    }
    *(u16 *)(p - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* The 0x112 node spawn for the follower sprite: allocate off D_80083498, park
   the CALLER's record at the node's +0x40, install func_b22_80171A38 as the
   node's +0x10 step, arm the slot (+0x10 = 0x20, +0x6 = 8, 0xC bits in
   +0x14), seed the node's mover from the caller's three words, offset it by
   func_8003DE58's projection of the record's -0x14 owner, then through a
   RE-READ of n->unk_0C set the 0x800 scale pair and the 0x80 grey and copy
   the 12-byte D_80175EE8 record into the node's own +0x44.  94 words,
   0 differing, first compile.

   The in-unit sibling func_b22_80171928 supplied the whole skeleton and both
   of its stated facts HOLD here, one of them with a much bigger number:

   THE SECOND `r = (UnkSlot *)n->unk_0C' IS LOAD-BEARING.  With one `r' the
   body is 95 words / 39 aligned differences -- a word longer and a whole
   register rotation.  func_b22_80171928 prices the same re-read at 67/27, so
   the direction transfers and the size does not.

   The 12-byte copy goes through `UnkDngB22Name12' (`u8 b[0xC]', alignment 1),
   which is what forces the `lwl'/`lwr' + `swl'/`swr' pairs; func_b22_80171928
   measures three aligned word stores instead at 30 aligned differences.

   `u16 out[3]' is func_8003DE58's out-parameter and all three halves are read
   UNSIGNED here (`lhu 0x10/0x12/0x14($sp)'), unlike csrc/ovl_dng_b55.c's
   func_b55_80170898 where the averages read it signed.  The third parameter
   is never used and still has to be declared: func_b22_80175050 calls this
   with three arguments. */
void func_b22_80171BEC(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *t;
    u16 out[3];

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x40) = p;
        n->unk_10 = (u32)func_b22_80171A38;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_06 = 8;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08;
        t = *(u8 **)(p - 0x14);
        if (func_8003DE58(*(u8 **)(t + 0x8), t, out, 0) != NULL)
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + out[2];
        }
        r = (UnkSlot *)n->unk_0C;
        r->unk_1C = r->unk_1E = 0x800;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *(UnkDngB22Name12 *)((u8 *)n + 0x44) = *(UnkDngB22Name12 *)D_80175EE8;
        r->unk_08 = (u32)((u8 *)n + 0x44);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80171D64);

/* The banked room script's per-frame actor driver, the same relinked twin of
   the resident overlay's func_800D8728 (csrc/ovl_dungeon_800D50FC.c) that
   landed as func_b03_80170A28 and its thirteen siblings: past the engine's
   0x2000 gate it runs func_800A9E70, then the record's own +0x8C
   continuation, then the +0x9A entry of this bank's D_80175EF4 facing table,
   re-aims the tile when the +0x6D facing moved under it, re-skins the slot
   for the eight-way facing, steps the mover by its velocity, runs the +0x9D
   bob counter and leashes +0x92/+0x88 to the ground height func_800BCB04
   reports. Bank-local deltas from func_b03_80170A28: the +0x8C gate compares
   against func_b22_801724BC and the facing table is D_80175EF4.

   THE ONE SHAPE DELTA FROM THE FOURTEEN LANDED SIBLINGS, and the reason this
   body is 258 words where theirs are 267: the three-way ground-height arm's
   common tail -- b->unk_14 = 0, the +0x1C 0x8000000 set and p[0x9D] = 0 --
   is written ONCE here, with the +0x90 / +0x92 stores as a nested if/else
   inside the else arm, not duplicated into both arms the way every sibling
   writes it. The siblings' duplicated spelling is not merely longer: the
   extra arm carries a second `q + 0x1C` reference, and dropping it moves q
   below p in global.c's allocno rank, so p takes $s1 and q takes $s2 here
   where the siblings have them the other way round. That is not a choice --
   writing the tail twice puts q back in $s1 and misses everywhere.

   The three spellings func_800D8728's and func_b03_80170A28's comments name
   are all load-bearing here too, re-measured in this unit:

   | spelling                                      | words | aligned diffs |
   | as written below                              |  258  |      0        |
   | the siblings' duplicated tail                 |  267  |     74        |
   | one handler local instead of `h` and `h2`     |  258  |      3        |
   | `s32 k1`/`s32 k2` instead of `s16`            |  255  |     15        |
   | no `q` alias, `p` at every use                |  255  |    120        |

   The `& 7` facing index is read four times and the target holds it in three
   registers -- $s0 for the +0x94 store, $v1 for the +0x94 compare and the
   +0x2C subscript, $s5 for the D_8006CCF8 test, the last live across
   func_80047738 and the reason a sixth callee-saved register is saved. A
   same-width copy is copy-propagated away; the NARROWER copies survive
   because cse's make_regs_eqv never equates a subreg set. The two +0x8C
   handler reads need their own locals for the same reason: one local is a
   single global-alloc quantity and takes $v1 at both sites, where the
   target's second site is $v0. 258 words, 0 differing, first compile. */
void func_b22_801720B4(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    u8 *e;
    s16 dir;
    s32 r;
    s32 v;
    s16 k1;
    s16 k2;

    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b22_801724BC)
        {
            h(p, b, c, p);
        }
        else
        {
            p[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)p[0x6D];
    if (func_800A9E70(p, b, c, p) != 0)
    {
        return;
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, p);
    }
    D_80175EF4[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        k1 = v;
        k2 = v;
        if (*(s16 *)(p + 0x94) != k1)
        {
            e = *(u8 **)(c + 0x2C);
            if (e != NULL)
            {
                func_80047738(c, e[k1], *(s8 *)(c + 0x4));
            }
            *(s16 *)(p + 0x94) = v;
        }
        if (D_8006CCF8[k2] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        if ((*(s32 *)(q + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
    }
    else if ((*(u16 *)(c + 0x14) & 0x800) != 0)
    {
        *(u16 *)(c + 0x14) &= 0x8FFF;
    }
    else
    {
        *(u16 *)(c + 0x14) |= 0x7000;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(u16 *)(p + 0x98) & 0x8) != 0)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(p + 0x98) & 0x4) == 0)
    {
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
            }
            else
            {
                if ((s16)r >= *(s16 *)(q + 0x88))
                {
                    *(s32 *)(p + 0x90) = 0;
                }
                else
                {
                    *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
                }
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
                p[0x9D] = 0;
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(q + 0x88) - 0x20);
                *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - r;
                *(u16 *)(q + 0x88) = r;
            }
        }
        else
        {
            *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
        }
    }
    else
    {
        *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801724BC);

/* The step-the-walk handler: advance the actor one cell of its queued path,
   swap in whichever of D_b22_80175DC4 / D_b22_80175DCC / D_80175DD4 its
   d+0x48 kind calls for, re-aim the sprite, and either stop at state 0xF or
   hand on to func_b22_80173678.  166 words, 0 differing, first compile.

   The donor is the landed func_b25_801724B0 (csrc/ovl_dng_b25.c, 143 words,
   dngtwin 0.867), itself out of func_b07_80171570; read b25's comment for the
   statements and for b07's measurements.  The 23-word delta is ONE thing:
   b25's single D_800E2348 dress becomes a three-arm switch on d[0x48] with
   the guard and the dress written out per arm (jump2 cross-joins them from
   the `beq' down, exactly as in func_b22_80173678 above).  b25's trailing
   `*(s16 *)(p + 0x9E) = 0' is b25's own delta from b07 and is absent here.

   B25'S THIRD DELTA REPRODUCES TO THE DIGIT: the tail divisor must REUSE `t'
   rather than take a local of its own.  A separate `s32 n' is 167 words / 18
   aligned differences here; b25 records 144/18 on its 143-word body.  Sharing
   `t' puts the divisor in $s0 beside `x' and `t' and the whole schedule falls
   out of it.

   b25's `t = (s16)func_8009A66C(...)' cast-at-the-assignment form and its
   `(s16)r' at that call are both kept and neither was re-priced here (b25
   prices the cast-at-the-use form at 8 rows and the dropped `(s16)r' at 4).
   `func_800AC82C' and `func_800AD9B4' stay K&R empty-paren declarations, as
   everywhere else in the corpus, and are called with four and two
   arguments. */
void func_b22_80172CC0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        switch (d[0x48])
        {
        case 0xD:
            if (*(u8 **)(c + 0x2C) != D_b22_80175DC4)
            {
                *(u8 **)(c + 0x2C) = D_b22_80175DC4;
                func_80047784(c, D_b22_80175DC4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
            break;
        case 0xE:
            if (*(u8 **)(c + 0x2C) != D_b22_80175DCC)
            {
                *(u8 **)(c + 0x2C) = D_b22_80175DCC;
                func_80047784(c, D_b22_80175DCC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
            break;
        case 0xF:
            if (*(u8 **)(c + 0x2C) != D_80175DD4)
            {
                *(u8 **)(c + 0x2C) = D_80175DD4;
                func_80047784(c, D_80175DD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            break;
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(x, y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C),
                          (u16 *)(p + 0x98));
        t = (s16)func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if (t == 3 && (D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            func_b22_80173678(p, b, c, d);
        }
        else
        {
            p[0x9A] = 0xF;
        }
        *(s32 *)(p + 0x8C) = 0;
        *(s32 *)(d + 0x1C) |= 0x40000000;
        if ((D_80083462 & 0x80) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            t = d[0x71];
            if (t > 0)
            {
                *(s16 *)(p + 0x96) = 8 / t;
            }
        }
    }
}

/* The banked room script's walk step: the 456-WORD RELINK of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c), and a DIFFERENT
   SOURCE from the 466-word relink that 46 other stubs in this block share
   (func_b01_80171C34, func_b14_80171410, ...). Read func_b01_80171C34's
   comment first: everything about the three extra pre-loop arms, the
   func_800BCB04 register-assignment fact, the `e'-sharing of the loop's
   heading value, and the D_800E2970 0x14-byte indexing is its, holds here
   verbatim, and is not repeated.

   FOUR SOURCE DELTAS FROM THE 466-WORD BODY, AND THEY ACCOUNT FOR ALL TEN
   WORDS. Every number below was measured on THIS body, one change at a time.

   1. `s32 i', not `s16 i'. That is the whole 456-vs-466 story: an s32 index
      lets loop.c strength-reduce the D_8006CD00 lookup to a pointer walk, so
      the two arms become a bare `lh $v0, 0x0($s1)' with an `addiu $s1, $s1,
      0x2' in the tail, and the `i >= 3', `i == 0' and `i < 8' tests lose
      their `sll 16'/`sra 16' pairs. `s16 i' is 468 words and 144 aligned
      differences.

   2. THE SWEEP TABLE IS NAMED INLINE AS `D_8006CD00[i]', WITH NO `s16 *tbl'
      LOCAL. This is the one spelling fact that is not visible in the
      listing's arithmetic, and it is why the 466-word body cannot simply be
      retyped. b01's `tbl = D_8006CD00;' local is 457 words and 21 aligned
      differences HERE: naming the table through a local takes four RTL insns
      out of the loop, the loop's insn_count drops below gcc 2.7.2's
      invariant-motion threshold (loop.c:532 computes
      `(loop_has_call ? 1 : 2) * (1 + n_non_fixed_regs)'; loop.c:1631 moves a
      movable when `threshold * savings * lifetime >= insn_count'), and the
      halfword extension of `f' in `i >= 3 && f != 0' is then hoisted into
      the loop preheader as an extra `move' -- costing the one word and
      rotating $s1/$s5/$s6. Written inline the index arithmetic is
      strength-reduced away AFTER that count is taken, so the final code is
      two words SHORTER while the count stays above the threshold. The
      preheader's single `lui'/`addiu' pair for D_8006CD00 is the giv's
      initial value, not a source assignment.

   3. D_8006CCD8 IS HOISTED, THROUGH A `u8 *tb' SET BETWEEN `i = 0;' AND THE
      LOOP, AND INDEXED `*(u8 *)(kk + (s32)tb)' WITH kk FIRST. This is the
      exact inverse of b01, whose `t1 = (s32)D_8006CCD8; t1 += kk;' device
      exists to give the `%lo' destination reg_n_sets != 1 and PIN the
      address inside the loop. Here the target carries it in $s7 across the
      whole loop, which is also why this copy saves nine registers' worth of
      frame plus $s7 (sp -0x40, $ra at 0x38) where b01 saves eight (sp -0x38).
      Measured: inline `*(u8 *)(kk + (s32)D_8006CCD8)' with no local is 454
      words / 20 differences; `tb[kk]' instead of `kk + (s32)tb' is 456 words
      / 1 difference, the `addu $v1, $a0, $s7' coming out `addu $v1, $s7,
      $a0'; putting `tb = ...' BEFORE `i = 0' is 456 words / 8 differences,
      because `i = 0' must be the first insn of the pre-loop join block for
      reorg to copy it into the seven branch delay slots that reach it. The
      SECOND lookup stays inline as `*(u8 *)(kk + (s32)D_8006CCE8)' -- it
      must, for the reason b01 gives.

   4. `i = (s16)func_800BCB04(...)' INTO THE s32 `i', not b01's s16-returning
      func_800BCB04h alias. The target's `sll $v0, $v0, 16' / `sra $s3, $v0,
      16' sign-extends straight into `i'; dropping the `(s16)' is 455 words /
      5 differences.

   TWO OF b01'S DEVICES DIVERGE. Its `do { return; } while (0)' in the
   `i == 0' arm must be REMOVED -- 459 words / 14 differences with it, because
   with an s32 index reorg does not want `i++' in those delay slots and fills
   them with the `lui %hi(D_80082EA4)' and a `nop' instead. Its `cc2 =
   0x80000000' do-while(0) reference bump is KEPT verbatim: spelled plainly as
   `*(s32 *)(d + 0x14) |= 0x80000000;' this body is 456 words / 4 differences,
   the same swap of $v0 and $v1 b01 measures.

   456 words, 0 differing, and 0 aligned differences. No jump table and no
   rodata of its own -- the listing opens at `glabel'. The second parameter is
   never read and still has to be declared and passed on, as in b01. */
void func_b22_80172F58(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 f;
    s32 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 x;
    s32 y;
    u8 *tb;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b22_80173734(p, b, c, d) == 0)
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
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
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
    tb = (u8 *)D_8006CCD8;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - D_8006CD00[i];
        }
        else
        {
            s = e + D_8006CD00[i];
        }
        if ((s16)func_8009A66C((s16)s, (u8 *)c, d, 0x20) > 0)
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
            *(u8 *)(c + 0x24) += *(u8 *)(kk + (s32)tb);
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
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
    i = (s16)func_800BCB04((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

/* The state-0x10 facing-table installer for this bank's three actor kinds:
   park the actor at state 0x10, then swap in whichever of D_80175E9C (d+0x48
   == 0xD), D_80175EA4 (0xE) or D_80175EAC (0xF) is not already at the slot's
   +0x2C and dress the sprite through func_80047784.  47 words, 0 differing,
   first compile.

   The donor is IN THIS UNIT: func_b22_8017472C's state-3 block has the same
   three-arm d[0x48] dress with the same `*(u8 **)(c + 0x2C) != tbl' guard
   (csrc/ovl_dng_b22.c, and func_b05_80171D6C / func_b25_80172E0C are the
   one-table form).  The arms are written ASCENDING 0xD, 0xE, 0xF, which is
   the order their blocks appear at: 0x801736C8, 0x801736D4, 0x801736E4.
   func_b22_8017472C's own table prices those arms descending at 324 words /
   8 aligned differences and func_b22_80174C44's d[0x48] switch at 218 / 16,
   so the order was not re-measured here.
   Nothing was fought for.  Three cases clear gcc 2.7.2's
   `case_values_threshold', so this compiles to a compare chain and the
   function owns NO jump table (jtcheck agrees: the unit's two tables belong
   to func_b22_801724BC and func_b22_801743C4).

   jump2 cross-joins the three arms from the `beq' down, which is why the
   listing has one `lw 0x2C($a2)' per arm and a single shared dress.  The
   frame is -0x18 for func_80047784's three outgoing argument words. */
void func_b22_80173678(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    p[0x9A] = 0x10;
    p[0x9B] = 0;
    switch (d[0x48])
    {
    case 0xD:
        if (*(u8 **)(c + 0x2C) != D_80175E9C)
        {
            *(u8 **)(c + 0x2C) = D_80175E9C;
            func_80047784(c, D_80175E9C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        break;
    case 0xE:
        if (*(u8 **)(c + 0x2C) != D_80175EA4)
        {
            *(u8 **)(c + 0x2C) = D_80175EA4;
            func_80047784(c, D_80175EA4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        break;
    case 0xF:
        if (*(u8 **)(c + 0x2C) != D_80175EAC)
        {
            *(u8 **)(c + 0x2C) = D_80175EAC;
            func_80047784(c, D_80175EAC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80173734);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80173B48);

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
s32 func_b22_80173EAC(u8 *a, void *b, void *c, s32 d)
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
        func_b22_80172F58(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b22_80172F58(p, b, c, p);
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

/* func_800D9884 (csrc/ovl_dungeon_800D50FC.c) minus its trailing D_80083462
   test: the outer gate's failure falls through to the epilogue and the inner
   gate's failure returns, so both branches land on the same word, and the
   installed handler is the bank-local func_b22_801724BC. 31 words,
   0 differing, first compile. */
void func_b22_80173FF0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b22_801724BC;
        *(s32 *)(p + 0x90) = 0;
    }
}

/* The banked room script's fall-and-land state machine: the three-case
   p[0x9B] fallthrough switch, the `(num << 16) / step' interpolation into
   b->unk_0C/unk_10, the `-func_800644B8(step * 409)' drift and the shared
   func_800A2B04 / func_800AD594 / func_800A4ACC / func_800AD9B4 tail. The
   twin read for this body is the landed func_b27_80173210
   (csrc/ovl_dng_b27.c, 221 words, dngtwin 0.912) -- read its comment first;
   the resident original is func_800D991C (csrc/ovl_dungeon_800D50FC.c).
   189 words, 0 differing, FIRST COMPILE.

   THIS COPY DRESSES NOTHING, and that is the whole 32-word difference from
   b27: neither case 0 nor case 2 has b27's
   `*(u8 **)(c + 0x2C) = T; func_80047784(c, T[...], 0)' pair, and case 0 has
   no leading `(c + 0x14) & 0x6000' gate either, so it falls straight into
   case 1. No jump table and no rodata of its own -- the listing opens at
   `glabel'.

   ALL FIVE OF b27's NAMED FACTS HOLD HERE, three of them reproducing its
   figures to the digit. Every row re-measured on THIS body with aligndiff,
   one change at a time:

   | spelling                                        | words | aligned |
   | as written below                                |  189  |    0    |
   | tail gate's 0x2000 arm as the `else'            |  189  |   20    |
   | the 0x410 and 0x20000 tests folded into one &&  |  189  |   10    |
   | the first numerator as one expression           |  189  |    1    |
   | the second numerator split like the first       |  190  |   12    |
   | no `(s16)' on the func_800AD9B4 result          |  188  |    2    |

   TWO CONSTANTS ARE THIS COPY'S OWN AND BOTH ARE PRICED: the drift shifts by
   10 where b27 shifts by 9 (b27's `<< 9' is 189 words / 1 aligned
   difference, the one `sll'), and case 2 ends `p[0x9B] += 1' where b27
   writes `p[0x9B] = 3' (b27's `= 3' is 187 words / 4). The sub-counter is
   p + 0x9E and the accumulator p + 0xA0, as in b27. */
void func_b22_8017406C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;
    s32 v;
    u16 x;

    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s16 *)(p + 0x9E) = 5;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
    case 1:
        *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
        if (*(s16 *)(p + 0x9E) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x9E);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x9E);
            *(s32 *)(p + 0xA0) = -func_800644B8(*(s16 *)(p + 0x9E) * 409) << 10;
        }
        *(s32 *)(p + 0x90) += *(s32 *)(p + 0xA0);
        if ((*(s16 *)(p + 0x9E) -= 1) < 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u16 *)(p + 0x98) &= ~8;
            *(s32 *)(d + 0x1C) |= 0x8000000;
            p[0x9B] += 1;
        }
    case 2:
        if ((*(s32 *)(d + 0x1C) & 0x8000000) != 0)
        {
            *(u16 *)(p + 0x98) &= ~8;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            p[0x9B] += 1;
        }
        break;
    }

    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_800AD594(d, 4);
    func_800A4ACC(d);
    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(u16 *)(g + 0x8) -= 1;
    }
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) != 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
    }
    else if ((v & 0x410) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
        }
    }
    if ((s16)func_800AD9B4(c, d) > 0)
    {
        *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        func_800A9A04(d);
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
void func_b22_80174360(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_801743C4);

/* The actor's +0x9B state machine for the three-slot carry, four states:
   state 0 picks the slot triple out of (d + 0x46) & 0x3FFF and hands it to
   func_800A94A0, state 1 waits on func_8003F270 and FALLS THROUGH into
   state 2, state 2 runs func_b22_80171790 ten times and spawns the 0x212
   node through func_b22_80171928, and state 3 re-dresses from one of three
   bank-local facing tables and re-installs func_b22_801724BC.
   326 words, 0 differing.

   OWNS jtbl_b22_80170850 (rodata +0x48, .align 3): seven entries, cases 1..7
   of state 0's FIRST switch over the 0x3FFF field, and NO TRAILING PAD WORD
   despite the odd count. The reason is that this table is LAST in the unit's
   .rodata, not that anything follows it 8-aligned: splat/dungeon.bin.yaml
   puts ovl_dng_b22's .rodata at 0xd07008..0xd0706c and the `c' subsegment at
   0xd0706c, so the section ends on this table's last word at 0x8017086C
   (4 mod 8) and the text starts there. The unit's three tables sit at +0x00
   (12 entries, 0x30), +0x30 (6 entries, 0x18) and +0x48 (7 entries, 0x1C),
   summing to exactly the 0x64 the yaml gives, so there is no pad between any
   of them either -- the first two end 8-aligned on their own.
   docs/matching-levers.md's entry on this reads unconditionally ("AN
   ODD-SIZED JUMP TABLE ALWAYS HAS EXACTLY ONE TRAILING PADDING WORD"); the
   last table in a .rodata is the exception, and this is a worked case of it.
   Entries in order:
     +0x00  .Lb22_80174850   case 1 -> g = d + 0x8
     +0x04  .Lb22_80174848   case 2 -> g = d + 0xB
     +0x08  .Lb22_80174840   case 3 -> g = d + 0xE
     +0x0C  .Lb22_80174858   case 4 -> the `default', g = NULL
     +0x10  .Lb22_80174800   case 5 -> k = 1, falls into case 1
     +0x14  .Lb22_801747F8   case 6 -> k = 1, falls into case 2
     +0x18  .Lb22_801747F0   case 7 -> k = 1, falls into case 3
   The SECOND switch -- the (d + 0x1C) & 0x2000 `else' -- has three cases and
   compiles to a compare chain, and so does state 3's switch on d[0x48].

   THREE OF THE SEVEN ARMS HAVE AN ORDER NO TOOL CAN SEE. Written ascending
   (5,1 / 6,2 / 7,3) this body measures 326 words, 0 differing under
   ovlverify, 0 aligned differences under aligndiff AND
   `ok (8 data symbols, 12 calls)' under relcand -- and the EMITTED TABLE IS
   WRONG: entries +0x10 and +0x18 come out swapped. All three tools were run
   on this body in that spelling. Each is blind for its own reason: ovlverify
   and aligndiff MASK the `j' target, and relcand compares the call SET, not
   the sequence, so two exchanged arms are the same set. relcand is not
   merely vacuous here -- swapping func_800A4ACC for func_800AD4D0 in state 3
   makes it name both symbols -- so its `ok' is a working check passing on
   wrong code. Confirmed by compiling the unit standalone with cdkgcc -S and
   reading the emitted `.word $L...' list against this listing's: descending
   emits the blocks case 7, case 6, case 5 and ascending emits 5, 6, 7, which
   permutes the last three entries.

   BUT THE BLINDNESS IS NARROW, AND ONLY THOSE THREE NEED HAND-SETTLING. An
   arm is invisible only when it is WORD-IDENTICAL to its neighbour after
   masking, which cases 5/6/7 are: a bare `addiu $s5, $zero, 0x1' and a
   masked `j', so only their ADDRESSES separate them and only the table
   carries those. Any arm that keeps a distinguishing instruction -- its own
   store, a constant in a delay slot, a different argument -- is already
   priced by the ordinary measurement, and cases 1/2/3 are: each keeps an
   `addiu $s0, $s1, 0x8/0xB/0xE', and the else chain's arms written ascending
   is 328 words / 11 aligned differences. So settle by hand only the arms
   that are word-identical after cross-joining -- read their entry addresses
   and write them in that order -- and do not read a clean 0 from ANY of the
   three tools as evidence about those.
   (csrc/ovl_dng_b20.c's func_b20_80172BCC records the same ascending
   spelling as a measured FREE fact. It is free to all three tools and it is
   not free in the container; its own kept spelling is the descending one.)

   The twin is the landed func_b20_80172BCC (csrc/ovl_dng_b20.c, 325 words,
   dngtwin 0.842), out of func_b16_801725A4. THREE of its four priced facts
   HOLD here and ONE INVERTS. Re-measured on this body:

   | spelling                                        | words | aligned |
   | as written below                                |  326  |    0    |
   | `s16 k', which is what b20 KEEPS                |  327  |    7    |
   | the else compare chain's arms ascending 1, 2, 3 |  328  |   11    |
   | the sign fixes as `if (x < 0) x = -x;'          |  325  |   15    |
   | `&D_80083460' direct instead of the `g2' local  |  325  |   13    |
   | state 3's d[0x48] arms descending 0xF, 0xE, 0xD |  324  |    8    |
   | state 1 ending in `break' instead of falling    |  327  |    7    |

   THE INVERSION IS `k'. b20 prices `s32 k' at 324 / 8 and keeps `s16';
   here `s16 k' costs the one word and 7 rows, all of them a `move $v0, $s5'
   that the halfword test needs and the target does not have. Both bodies
   save seven callee-saved registers and both have the same frame, so the
   frame rule does not predict this one -- it is a SELECTION fact that
   inverted, which is the first one of that class seen to do so, and the
   residue is the giveaway: 7 rows with no register rotation.

   AND THE LOOP COUNTER MUST BE ITS OWN VARIABLE. Reusing state 0's
   D_8006DE24 index as state 2's counter is 329 words / 127 aligned
   differences: the counter lives across func_b22_80171790 so it takes a
   callee-saved register, and sharing it with the index makes it conflict
   with `g', which costs a SEVENTH save and shifts every parameter one
   register up. b20 has no loop and so cannot state this.

   MEASURED AND FREE: `D_8006DE24[g[0]].pad_11[1] == 2' with no tbl/i/e
   locals (326 / 0, as on b20), the ten-iteration loop as a `for' rather than
   the `i = 0; do ... while' below (326 / 0), and state 2's `-= 1' / 0xE000
   gate as two nested `if's rather than one `&&' (326 / 0).

   THIS COPY'S OWN DELTAS from b20: state 0's success arm arms +0x96 to 0x16
   and +0xAE to 0xE before `p[0x9B]++'; state 1 has no 0x212 spawn of its own
   and does NOT break, so it falls into state 2; state 2 is entirely new --
   ten func_b22_80171790 calls, the func_b22_80171928 spawn at +0x96 == 0x16,
   then the counter-and-0xE000 gate; and state 3 tests D_8008346C where b20
   tests `g2 + 0xC', and swaps one of THREE tables on d[0x48] where b20 has a
   single unconditional D_801752B4. */
void func_b22_8017472C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    s32 k;
    s32 i;
    s32 j;
    s32 x;
    s32 y;

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                m = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)m;
                q = *(u8 **)(m - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                tbl = D_8006DE24;
                i = g[0];
                e = (u8 *)&tbl[i];
                if (e[0x12] == 2)
                {
                    m = *(u8 **)(d + 0x60);
                    if (m != NULL)
                    {
                        q = *(u8 **)(m - 0x14);
                        d[0x72] = q[0x24];
                        d[0x73] = q[0x25];
                    }
                }
                else
                {
                    *(s32 *)(d + 0x60) = func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            if (func_800A94A0(d, g, k, p + 0x98) != 0)
            {
                *(s16 *)(p + 0x96) = 0x16;
                *(s16 *)(p + 0xAE) = 0xE;
                p[0x9B] += 1;
            }
        }
        else
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B] += 1;
        func_800A56E0(0x703);
    case 2:
        if ((*(u16 *)(p + 0x96) & 1) == 0)
        {
            if (*(s16 *)(p + 0xAE) != 0)
            {
                *(u16 *)(p + 0xAE) -= 1;
            }
        }
        j = 0;
        do
        {
            func_b22_80171790(p, b, c, d);
            j += 1;
        } while (j < 0xA);
        if (*(s16 *)(p + 0x96) == 0x16)
        {
            func_b22_80171928(p, b, c);
        }
        if ((*(s16 *)(p + 0x96) -= 1) > 0 && (*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u16 *)(p + 0x98) |= 0x80;
        p[0x9B] += 1;
        break;
    case 3:
        if (D_8008346C == 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            switch (d[0x48])
            {
            case 0xD:
                if (*(u8 **)(c + 0x2C) != D_80175E24)
                {
                    *(u8 **)(c + 0x2C) = D_80175E24;
                    func_80047784(c, D_80175E24[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
                break;
            case 0xE:
                if (*(u8 **)(c + 0x2C) != D_80175E2C)
                {
                    *(u8 **)(c + 0x2C) = D_80175E2C;
                    func_80047784(c, D_80175E2C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
                break;
            case 0xF:
                if (*(u8 **)(c + 0x2C) != D_80175E34)
                {
                    *(u8 **)(c + 0x2C) = D_80175E34;
                    func_80047784(c, D_80175E34[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
                break;
            }
            g2 = (u8 *)&D_80083460;
            if (*(u32 *)(g2 + 0xC) == 0)
            {
                *(u16 *)(g2 + 0xA) -= 1;
                *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
                func_800A4ACC(d);
                d[0x73] = 0;
                d[0x72] = 0;
                d[0x6D] -= 1;
                *(u16 *)(d + 0x46) &= 0x7FFF;
                func_800A56E0(0xB4);
            }
        }
        break;
    }
}

/* The banked room script's four-state glide, switching on p[0x9B]: state 0
   calls func_800AD4D0 and arms the +0x96 counter, state 1 seeds the velocity
   from the D_8006CCD8/D_8006CCE8 facing pair while the counter runs, state 2
   steers onto the tile centre with the two-numerator divide, and state 3
   stops, re-centres and installs func_b22_801724BC. 221 words, 0 differing.

   The twin is the landed func_b42_80173730 (csrc/ovl_dng_b42.c, 178 words,
   dngtwin 0.837), itself out of func_b23_80173B4C; read b42's comment for
   the statements, which are all its. THE 43-WORD DIFFERENCE IS ONE THING:
   where b42 calls `func_800AAA54(p, b, c, D_801760D4)' with a single
   bank-local table, this body switches on d[0x48] for one of D_80175E3C
   (0xD), D_80175E44 (0xE) or D_80175E4C (0xF) -- and it does that at BOTH
   call sites, state 0's `d[0x28] == 0' arm and state 2's. jump2 cross-joins
   the three arm bodies, which is why the listing has one `jal func_800AAA54'
   per table and two `lbu 0x48($s2)' dispatches.

   THE ONE THING THAT HAD TO BE FOUND IS WHICH ARM IS THE `then'. b42 writes
   state 0's gate `if (d[0x28] != 0) { ...0x8000 arm... } else { stop }';
   the target's `bnez' says the stop block is the FALL-THROUGH, so it is
   `if (d[0x28] == 0) { stop } else if (c + 0x14 & 0x8000) { ... }'. b42's
   way round is 221 words / 20 aligned differences -- the whole 0x8000 arm
   emitted in the other position plus the inverted branch, and nothing else.

   Measured on this body, one change at a time (aligndiff):

   | spelling                                        | words | aligned |
   | as written below                                |  221  |    0    |
   | b42's `if (d[0x28] != 0)' arms round the other  |  221  |   20    |
   | b42's/b23's shared stop block reached by `goto' |  207  |   21    |
   | state 1's `n > 0' arm shifting by 19, as b42    |  221  |    2    |
   | d[0x48] arms descending 0xF, 0xE, 0xD           |  218  |   16    |
   | the tail gate as `-= 1' plus a reloaded `>= 0'  |  222  |   11    |

   b42's "write the stop block twice" fact HOLDS, and here it is not a device
   at all: the two sites differ in nothing but their position, and written
   once with a `goto' into state 2 the body loses 14 words. b42's `s32' for
   state 1's counter value is FREE here -- `s16 n' measures 221 / 0 -- so it
   is not the reason this lands. Both of state 1's arms shift by 18, where
   b42's first arm shifts by 19. No jump table and no rodata of its own. */
void func_b22_80174C44(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 n;
    s32 goal;
    s32 now;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        *(s16 *)(p + 0x96) = 6;
        p[0x9B] += 1;
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            switch (d[0x48])
            {
            case 0xD:
                func_800AAA54(p, b, c, D_80175E3C);
                break;
            case 0xE:
                func_800AAA54(p, b, c, D_80175E44);
                break;
            case 0xF:
                func_800AAA54(p, b, c, D_80175E4C);
                break;
            }
        }
        else if (*(u16 *)(c + 0x14) & 0x8000)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 3;
        }
        return;
    case 1:
        n = (*(s16 *)(p + 0x96) -= 1);
        if (n > 0)
        {
            b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
            b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
        }
        else if (n == 0)
        {
            b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
            b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 18;
            *(s16 *)(p + 0x96) = 0xC;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (d[0x28] == 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            switch (d[0x48])
            {
            case 0xD:
                func_800AAA54(p, b, c, D_80175E3C);
                break;
            case 0xE:
                func_800AAA54(p, b, c, D_80175E44);
                break;
            case 0xF:
                func_800AAA54(p, b, c, D_80175E4C);
                break;
            }
            return;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 16) / *(s16 *)(p + 0x96);
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 16) / *(s16 *)(p + 0x96);
        }
        if ((*(s16 *)(p + 0x96))-- > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        p[0x9B] += 1;
        return;
    case 3:
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        return;
    }
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AD058's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FB8(void)
{
    func_800AD058();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB538's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FD8(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b22_80174FF8(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b22_80175018(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80175050);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b22", func_b22_80175574);

/* The arrival handler: pick this actor kind's facing table -- and, for each
   kind, one of two tables depending on whether the record's +0x1C has 0x200
   or its +0x25 is zero -- dress the sprite if it is not already wearing it,
   then ask func_800AC82C whether the walk continues; if it does and
   func_800AD9B4 agrees, install func_b22_801724BC as the next step, and if it
   does not, swap the kind's SECOND table in when the actor is still wearing
   the first and +0x1C is clear of 0x208.  170 words, 0 differing.

   THE FIRST SWITCH'S GUARD AND DRESS ARE WRITTEN OUT IN BOTH ARMS OF ALL
   THREE CASES -- six copies.  Hoisted into a shared `u8 *t' set by a ternary
   with ONE guarded dress after it, the body is 166 words / 21 aligned
   differences, and the residue names two separate causes:

     * FOUR WORDS of it are the `lw 0x2C($s0)' the target loads SIX TIMES,
       once per path.  jump2 cross-joins the arms only from the `beq' down, so
       the load and the table's `addiu' stay per-path and the comparison and
       the dress are shared.  Writing the dress once gives one load and an
       unfillable delay slot.
     * the rest is `addu $v0, $a1, $v0' where the target has
       `addu $v0, $v0, $a1'.  An array ref off a pointer LOCAL goes through
       `pointer_int_sum', which always builds `PLUS(ptr, intop)' and emits
       base-first; off the SYMBOL the base costs no insn at expand time, so
       `expand_binop' forces it into a register AFTER the index and the index
       gets the lower `INSN_LUID'.  Naming the symbol at every site fixes the
       operand order for free.

   The two-table choice is `((*(s32 *)(d + 0x1C) & 0x200) != 0 || d[0x25] ==
   0)', read off the `bnez'/`bnez' pair: the 0x200 arm and the `d[0x25] == 0'
   fall-through both reach the FIRST table.  The second switch's dress has no
   `!=' guard -- it stores unconditionally -- because its gate already tested
   +0x2C against the other table. */
void func_b22_801759A0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (d[0x48])
    {
    case 0xD:
        if ((*(s32 *)(d + 0x1C) & 0x200) != 0 || d[0x25] == 0)
        {
            if (*(u8 **)(c + 0x2C) != D_80175E54)
            {
                *(u8 **)(c + 0x2C) = D_80175E54;
                func_80047784(c, D_80175E54[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        else
        {
            if (*(u8 **)(c + 0x2C) != D_80175E24)
            {
                *(u8 **)(c + 0x2C) = D_80175E24;
                func_80047784(c, D_80175E24[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        break;
    case 0xE:
        if ((*(s32 *)(d + 0x1C) & 0x200) != 0 || d[0x25] == 0)
        {
            if (*(u8 **)(c + 0x2C) != D_80175E5C)
            {
                *(u8 **)(c + 0x2C) = D_80175E5C;
                func_80047784(c, D_80175E5C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        else
        {
            if (*(u8 **)(c + 0x2C) != D_80175E2C)
            {
                *(u8 **)(c + 0x2C) = D_80175E2C;
                func_80047784(c, D_80175E2C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        break;
    case 0xF:
        if ((*(s32 *)(d + 0x1C) & 0x200) != 0 || d[0x25] == 0)
        {
            if (*(u8 **)(c + 0x2C) != D_80175E64)
            {
                *(u8 **)(c + 0x2C) = D_80175E64;
                func_80047784(c, D_80175E64[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        else
        {
            if (*(u8 **)(c + 0x2C) != D_80175E34)
            {
                *(u8 **)(c + 0x2C) = D_80175E34;
                func_80047784(c, D_80175E34[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              0);
            }
        }
        break;
    }
    if (func_800AC82C(p, b, c, d) != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
        }
    }
    else
    {
        switch (d[0x48])
        {
        case 0xD:
            if (*(u8 **)(c + 0x2C) == D_80175E54 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
            {
                *(u8 **)(c + 0x2C) = D_b22_80175DC4;
                func_80047784(
                    c, D_b22_80175DC4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            break;
        case 0xE:
            if (*(u8 **)(c + 0x2C) == D_80175E5C && (*(s32 *)(d + 0x1C) & 0x208) == 0)
            {
                *(u8 **)(c + 0x2C) = D_b22_80175DCC;
                func_80047784(
                    c, D_b22_80175DCC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            break;
        case 0xF:
            if (*(u8 **)(c + 0x2C) == D_80175E64 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
            {
                *(u8 **)(c + 0x2C) = D_80175DD4;
                func_80047784(
                    c, D_80175DD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            break;
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
void func_b22_80175C48(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b22_801724BC;
        }
    }
}

/* 14 words, 0 differing. */
void func_b22_80175CAC(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175CE4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D1C(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D54(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}

/* 14 words, 0 differing. */
void func_b22_80175D8C(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b22_801724BC;
    }
}
