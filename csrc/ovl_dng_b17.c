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
s32 func_b17_801721E0(u8 *p, UnkVec *b, u8 *c, u8 *d);
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
extern u8 D_b17_80174494[];
extern u8 D_801744FC[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b17_80171A98(u8 *, void *, void *, u8 *);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b17_801713A8();

s32 func_800AB778();

s32 func_800AB538();

s32 func_800AB464();

s32 func_800AC7EC();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

extern u8 D_801744F4[];
extern u8 D_801744EC[];
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800A2C34(u8 *);
void func_800AA258(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, UnkVec *, u8 *, u8 *);
void func_800A9A04(u8 *);
void func_b17_80174250(u8 *p, u8 *b, u8 *c, u8 *d);

extern u8 D_80175140[];

void func_800AD4D0(u8 *);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800A2B04(UnkVec *, s32, s32);
extern u8 D_b17_8017449C[];
extern u8 D_801744AC[];

extern u8 D_801744E4[];
extern u8 D_80083498[];
extern u8 D_80045340[];
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_800478E8(u8 *, u32, s32);
void func_800D78C0(u8 *);
void func_800D71A8();
void func_b17_80170BB8();

/* The banked room script's SPAWNER: allocate the 0x112 node this bank's
   actor lives in off D_80083498, install func_b17_80170BB8 as the node's
   +0x10 driver with state 4 at +0x13, dress the slot from the bank-local
   D_b17_80174494 facing table, pick the +0x14/+0x1C flag pair from the low two
   bits of `a' and -- only for a==0, one actor in two -- raise +0x1C's 0x200,
   recolour through func_800A48F0 and swap in D_801744FC. Then arm the slot
   (+0x9A/+0x9C/+0x8C/+0x92/+0xA8/+0x9E/+0x98) and, for each of the +0x9E
   sub-slots, allocate ANOTHER 0x112 node, park it in the +0xA4 pointer array
   and dress each of its 0x30-byte sub-renderables at +0x8 from D_801744E4
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
   allocation returned, which is what the `lw $v0, 0xA4($fp)' is.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x11 at 1 of 3 sites, -0x20 -> -0x30.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_b17_80171A98(u8 *p, void *b, void *c, u8 *d);
void func_800AA79C(u8 *, void *, u8 *, u8 *);
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t);
s16 func_800A1C58(u8 *p);
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d);
s16 func_8009FB34(u16 x, u16 y);
void func_800AAF00(u8 *a, UnkVec *b, u8 *c, u8 *t, void (*h)());
void func_b17_801718D0();
void func_b17_801723A4(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_b17_801724C4(u8 *a, void *b, void *c, s32 d);
extern u8 D_801744A4[];
extern u8 D_801744D4[];
s32 func_800A05A4(u8 *p, s32 x, s32 y, s16 dir, s32 n);
void func_800DAE44(u8 *, s16);
s32 func_800A56E0(s32);
s32 func_8003F270(void);
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e);
extern s32 D_8008346C;
extern Unk8006DE24 D_8006DE24[];
extern u8 D_801744DC[];

s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
void func_80047738(u8 *, s16, s16);
void func_800A020C(s32, u8 *);
void func_800478B8(u8 *);
s32 func_800644B8(s32);
s32 func_800BCB04x(s32, s32, s16) __asm__("func_800BCB04");
extern u8 D_8006CCF8[];
extern void (*D_80174504[])(u8 *, UnkVec *, u8 *, u8 *);
extern s16 D_8008346A;
void func_800ACF88(u8 *);
void func_800A2FE0(u8 *);
u8 *func_800A32A4(u8 *);
void func_8009A028(u8 *);

extern u8 D_801744B4[];
extern u8 D_801744BC[];
extern u8 D_801744C4[];
extern u8 D_801744CC[];
extern u8 *D_800E3DE8;
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k);
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);

u8 *func_b17_8017089C(s16 a, u16 b, u16 c, s16 d)
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
        n->unk_10 = (u32)func_b17_80170BB8;
        p[0x13] = 17;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *(u8 **)((u8 *)r + 0x2C) = D_b17_80174494;
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
                    *(u8 **)((u8 *)r + 0x2C) = D_801744FC;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b17_801713A8;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x30;
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
                    *(u8 **)(e + 0x2C) = D_801744E4;
                    func_80047784(e, D_801744E4[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
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

/* The banked room script's per-frame actor driver -- this object's handler
   word -- and a 508-word member of the func_800D8728 family carved out of
   DUNGEON.BIN: the D_80083462 0x2000 engine gate, func_800A9E70, the
   record's own +0x8C continuation, the bank-local D_80174504 +0x9A dispatch,
   the +0x6D re-aim, the b-vector integration, a facing re-dress that TOGGLES
   c + 0x2C between D_b17_8017449C and D_b17_80174494, a +0xA8 lean timer and
   then a +0x92 ride-height servo that this copy runs FOUR different ways.
   508 words, 0 differing, ON THE FIRST COMPILE. 1 copy (dngfam.py).

   It does NOT own a jump table -- the +0x9A dispatch is an indirect call
   through the bank-local handler array D_80174504, not a switch, and this
   listing has no .rodata block.

   dngtwin RANKED A STUB FIRST: its top candidate func_b04_80170BF8 (0.967,
   +6 words) is still assembly. The answer is its SECOND row, the landed
   434-word func_b38_80170A70 (csrc/ovl_dng_b38.c) at 0.868 and -74 words,
   whose own root is the 305-word func_b43_80170AA4. THE FRAMES MATCH --
   `addiu $sp, $sp, -0x30' with $s0..$s6 and $ra, SEVEN callee-saved
   registers, the seventh being the `m' lean accumulator -- which is why
   every one of b38's allocation facts transferred at its recorded price.

   DELTAS FROM b38, read off this listing: the lean timer lives at p + 0xA8
   not p + 0xA0, its divisor is 21 not 40, its low shift is `>> 8' not
   `>> 9', and it is bumped with a plain `+= 1' rather than `% 40' -- the
   reset to zero is explicit, in the c + 0x14 0x6000 arm and at the end of
   the c + 0x14 0x8000 arm. The lean block GAINS two sub-blocks ahead of the
   timer (the p + 0x98 0x8000 clear with the c + 0x2C table toggle, and the
   c + 0x14 0x6000 arm that raises p + 0x98 0x8000 and zeroes the timer),
   and two paths gain a `*(u16 *)(p + 0x98) &= 0x7FFF'. Like b38 the whole
   lean-plus-servo block is written out TWICE, once per arm of the c + 0x14
   0x8000 test, which is what the two label sets around .Lb17_80170EC4 and
   .Lb17_80171174 are.

   THE BIGGEST FACT ON THIS BODY IS THE `q = p' ALIAS: 505 words / 193
   aligned differences with every later site spelled `p'. b38 states it and
   b43 before it; here it is worth more than all its other facts together.
   The listing reads the EARLY d + 0x1C gate through $s1 (p) and every site
   after the dress block through $s2 (q), exactly as b38 describes.

   b38'S DECIDING allocno_compare SPLIT CARRIES, AND ITS THREE FAILING
   COMBINATIONS REPRODUCE ITS FIGURES EXACTLY: the signed p + 0x92 read is
   ONE SHARED local `w' and the narrowed delta is a SEPARATE local per
   duplicated servo (`t' and `t2'):

     w shared, t split (below)                   508 words /   0 aligned
     w shared, t ALSO shared (one `t')           508 words /  12 aligned
     w inline at all four sites, t shared        508 words /  12 aligned
     w inline at all four sites, t split         508 words /  12 aligned

   b38 measured 12, 12 and 12 for the same three. All three failures are the
   identical residue, a pure $a0/$a1 exchange between the servo's two live
   values, so the answer is a CROSS no one-variable sweep reaches. `w' must
   be `s32': as `s16' it is 508 / 20, and b38 also recorded 20.

   FOUR MORE OF b38'S FACTS, re-measured here, both spellings each time:
     the facing index `k' inlined, `v' used at all three sites
                                                 505 words /  82 aligned
       (b38 recorded 431 / 72)
     the +0x9D gate as `||' with `p[0x9D] = 0' as the `then' instead of
       `&&' with the b-vector arm as the `then'
                                                 508 words /   7 aligned
       (b38 recorded 7)
     the servo delta as `(s16)r - *(s16 *)(q + 0x88)' instead of
       `(s16)(r - *(u16 *)(q + 0x88))' -- the subtraction must happen BEFORE
       the narrowing, which is what the listing's `subu' then `sll'/`sra'
       order says                                508 words /  14 aligned
     `s16 k' rather than `s32 k'                 508 words /  42 aligned
   The last two are sites b38 never priced.

   THREE FACTS THIS BODY NEEDED THAT b38 HAS NO SITE FOR:

   (1) THE c + 0x2C TOGGLE IS TWO DUPLICATED ARMS, NOT ONE SHARED BLOCK.
   Written through a single `u8 *f' with a NULL guard and the dress emitted
   once it is 514 words / 33 aligned differences. Written out twice, jump2
   cross-joins the two copies down to the common tail at .Lb17_80170EF0 and
   the only surviving difference is which symbol reaches $a1 -- which is why
   the second arm's comparison operand IS its table pointer.

   (2) `*(u16 *)(p + 0x98) &= 0x7FFF' SITS ABOVE THE p + 0x98 0x8 GATE, NOT
   INSIDE IT. Inside the gate: 509 words / 12 aligned. Deleted: 507 / 7.
   Above it, combine folds `(x & 0x7FFF) & 8' back to `x & 8', so the gate
   tests the ORIGINAL `lhu' and the masked store lands in the branch's delay
   slot, which is what the listing shows.

   (3) THE LEAN DIVISOR IS 21. `/ 40' is 508 words / 8 aligned differences;
   the diagnostic is `sra $v0, $t0, 2' after `mult' by 0x30C30C31 where 40
   gives `sra 4' after 0x66666667.

   MEASURED AND FREE, so NOT why this lands:
     the b-vector integration as plain casts rather than b38's COMPONENT_REFs
       (`*(s32 *)((u8 *)b + 0x0) += *(s32 *)((u8 *)b + 0xC)' and its pair)
                                                 508 words /   0 aligned
       -- so the MEM_IN_STRUCT_P polarity b38 and b04 pay for elsewhere is
          free at THIS site, with no fixed-address read across it
     the +0xA8 bump as `*(s16 *)' instead of `*(u16 *)'
                                                 508 words /   0 aligned
     the lean shift gate inverted -- `if (p[0x9A] != 0x12) m = n >> 8; else
       m = n >> 10;'                             508 words /   0 aligned
       -- both spellings emit the IDENTICAL words; the target's `sra 8' in
          the `bne's delay slot with `sra 10' after it does not say which
          way the source wrote the test, so this is free rather than forced.

   NOT COMPARABLE, so not a fact: merging the q + 0x1C 0x20 and c + 0x14
   0x40 tests into one `&&'. It reads 508 / 4, but the merge CHANGES THE
   PROGRAM -- with `&&' the `|= 0x7000' else arm is also reached when the
   0x40 test fails, where nested it is reached only when the 0x20 test does.
   Different programs, no price to compare. */
void func_b17_80170BB8(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    u8 *e;
    s16 dir;
    s32 n;
    s32 r;
    s32 w;
    s32 t;
    s32 t2;
    s32 v;
    s16 k;
    s32 m;

    m = 0;
    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b17_801713A8)
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
    D_80174504[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(s32 *)(p + 0x1C) & 0x40000) == 0 && (*(u16 *)(p + 0x98) & 0x8) == 0)
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    else
    {
        p[0x9D] = 0;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        v = ((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7;
        k = v;
        if (*(s16 *)(p + 0x94) != k)
        {
            e = *(u8 **)(c + 0x2C);
            func_80047738(c, e[k], *(s8 *)(c + 0x4));
            *(s16 *)(p + 0x94) = v;
        }
        if (D_8006CCF8[k] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        if (p[0x9A] != 8)
        {
            func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
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
            *(s32 *)(q + 0x1C) &= ~0x40000;
        }
        *(s32 *)(q + 0x1C) &= ~0x8000000;
        if ((*(s32 *)(q + 0x1C) & 0x40000) != 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                if ((*(u16 *)(p + 0x98) & 0x8000) != 0)
                {
                    *(u16 *)(p + 0x98) &= 0x7FFF;
                    if (*(u8 **)(c + 0x2C) == D_b17_8017449C)
                    {
                        *(u8 **)(c + 0x2C) = D_b17_80174494;
                        func_80047784(c, D_b17_80174494[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0);
                    }
                    else if (*(u8 **)(c + 0x2C) == D_b17_80174494)
                    {
                        *(u8 **)(c + 0x2C) = D_b17_8017449C;
                        func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0);
                    }
                }
                if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
                {
                    *(u16 *)(p + 0x98) |= 0x8000;
                    if (*(u8 **)(c + 0x2C) == D_b17_80174494)
                    {
                        *(s16 *)(p + 0xA8) = 0;
                    }
                }
                n = func_800644B8((*(s16 *)(p + 0xA8) << 12) / 21);
                if (p[0x9A] == 0x12)
                {
                    m = n >> 10;
                }
                else
                {
                    m = n >> 8;
                }
                *(u16 *)(p + 0xA8) += 1;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04x(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
                w = *(s16 *)(p + 0x92);
                t = (s16)(r - *(u16 *)(q + 0x88));
                if (t - 0x30 < w)
                {
                    *(u16 *)(p + 0x92) -= 8;
                }
                else if (w < t - 0x3A)
                {
                    *(u16 *)(p + 0x92) += 8;
                }
            }
        }
        else
        {
            *(s16 *)(p + 0xA8) = 0;
            *(u16 *)(p + 0x98) &= 0x7FFF;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04x(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
                r = (s16)r - *(s16 *)(q + 0x88);
                if (r < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = r;
                    p[0x9D] = 0;
                    b->unk_14 = 0;
                    *(s32 *)(q + 0x1C) |= 0x8000000;
                }
            }
        }
    }
    else
    {
        if ((*(u16 *)(c + 0x14) & 0x800) != 0)
        {
            *(u16 *)(c + 0x14) &= 0x8FFF;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        *(s32 *)(q + 0x1C) &= ~0x8000000;
        if ((*(s32 *)(q + 0x1C) & 0x40000) == 0)
        {
            *(s16 *)(p + 0xA8) = 0;
            *(u16 *)(p + 0x92) -= m;
            m = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04x(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
                r = (s16)r - *(s16 *)(q + 0x88);
                if (r < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = r;
                    p[0x9D] = 0;
                    b->unk_14 = 0;
                    *(s32 *)(q + 0x1C) |= 0x8000000;
                }
            }
        }
        else
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                if ((*(u16 *)(p + 0x98) & 0x8000) != 0)
                {
                    *(u16 *)(p + 0x98) &= 0x7FFF;
                    if (*(u8 **)(c + 0x2C) == D_b17_8017449C)
                    {
                        *(u8 **)(c + 0x2C) = D_b17_80174494;
                        func_80047784(c, D_b17_80174494[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0);
                    }
                    else if (*(u8 **)(c + 0x2C) == D_b17_80174494)
                    {
                        *(u8 **)(c + 0x2C) = D_b17_8017449C;
                        func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0);
                    }
                }
                if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
                {
                    *(u16 *)(p + 0x98) |= 0x8000;
                    if (*(u8 **)(c + 0x2C) == D_b17_80174494)
                    {
                        *(s16 *)(p + 0xA8) = 0;
                    }
                }
                n = func_800644B8((*(s16 *)(p + 0xA8) << 12) / 21);
                if (p[0x9A] == 0x12)
                {
                    m = n >> 10;
                }
                else
                {
                    m = n >> 8;
                }
                *(u16 *)(p + 0xA8) += 1;
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04x(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(q + 0x88) - 0x20);
                w = *(s16 *)(p + 0x92);
                t2 = (s16)(r - *(u16 *)(q + 0x88));
                if (t2 - 0x30 < w)
                {
                    *(u16 *)(p + 0x92) -= 8;
                }
                else if (w < t2 - 0x3A)
                {
                    *(u16 *)(p + 0x92) += 8;
                }
            }
        }
        *(s16 *)(p + 0xA8) = 0;
        *(u16 *)(p + 0x98) &= 0x7FFF;
    }
    if ((*(s32 *)(q + 0x1C) & 0x40000000) != 0)
    {
        *(s32 *)(q + 0x1C) &= 0xBFFFFFFF;
        n = func_800BCB04x((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(q + 0x88) - 0x20);
        if ((s16)n < 0x200)
        {
            *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - n;
            *(u16 *)(q + 0x88) = n;
        }
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92) + m;
    *(u16 *)(c + 0x14) |= 0x40;
}

/* The banked room script's IDLE handler -- the body this bank's spawner parks
   at the actor's +0x8C and the one the switch below re-installs through
   func_800AAF00. OWNS A JUMP TABLE: jtbl_b17_80170808, rodata offset 0x0,
   `.align 3', twelve entries for the +0x46 state 1..12 with 4, 10 and 11
   carrying the default arm's label rather than arms of their own. 330 words,
   0 differing, ON THE FIRST COMPILE.

   THE TWIN IS THE LANDED 334-word func_b38_80171138 (csrc/ovl_dng_b38.c),
   dngtwin 0.919. Its frame is identical -- `addiu $sp, $sp, -0x38' with
   $s0..$s3 and $ra, same save order -- and so is its register assignment,
   p $s0, d $s1, c $s2, b $s3, so both its shape facts and its allocation
   facts were in scope. Re-measured here, with b38's own figure last:

     as written below                        330 /  0  |   0
     D_800814A8 inline, not held in `w'      331 / 16  |  16
     the d + 0x2A store below the w[0x9A] test
                                             331 / 14  |  14
     the dress table through a shared `e' local
                                             331 / 11  |  10
     b38's `do { } while (0)' on the 0x40000 set
                                             330 / 65  | needed, 99 without
     b38's shared tail `goto'-ed to one label instead of
       written out at all three sites        330 /  0  |  27

   ITS THREE SELECTION FACTS REPRODUCED ITS PRICES, ITS ONE DEVICE INVERTS AND
   ITS ONE GOTO FACT GOES FREE. The `do'-wrapper b38 needs, to lift `d' above
   the D_80082E80 tail pointer in allocno_compare, is 65 aligned differences
   here: this body's extra D_801744A4 and D_801744D4 arms give `d' those
   references for nothing. And the three shared-tail sites cost nothing either
   way, because jump2 cross-joins them by itself.

   THIS BODY'S OWN:
     the case 8/9 arm written AFTER case 5/6/7 rather than first
                                                   330 words / 31 aligned
     func_80047784's third argument a literal 0 instead of the
       p + 0xA8 read-back                          330 words /  6 aligned
   MEASURED AND FREE: the two-table dress test as nested `if's, 330 / 0.

   Deltas from b38, all read off the listing. The 0x1000 escape calls this
   unit's own func_b17_801718D0 WITH NO ARGUMENTS AT ALL -- nothing writes
   $a0..$a3 before the `jal', so the four incoming registers reach it
   untouched, which is why there is a second, empty-paren declaration of it at
   the top of this file even though the definition above takes four
   parameters. p[0x9B] = 2 where b38 sets 1. The section-4 dress is the
   two-table test against D_b17_80174494 and D_b17_8017449C, installing the
   second, with p + 0xA8 in place of b38's single D_80174AD4 and p + 0xA0.
   CASES 8 AND 9 SHARE ONE ARM (func_b17_801721E0, then func_b17_801723A4 if
   it returns zero) where b38 gives case 9 an arm of its own, and that shared
   arm is FIRST in source order. The shared tail is func_b17_80171A98 at all
   three sites, the 0x80000 arm ends in func_b17_80174250, and the tables are
   D_801744FC, D_801744EC, D_801744A4 and D_801744D4.

   THE TABLE'S TWELVE ENTRIES, for states 1..12 in order: 1, 2, 3 -> the
   func_800AAF00 re-install arm; 4 -> default; 5, 6, 7 -> the func_800A0818 /
   D_800814A8[0x9A] arm; 8, 9 -> the func_b17_801721E0 arm; 10, 11 ->
   default; 12 -> the func_800A9A0C arm, which jump2 cross-joins with the
   d + 0x1C 0x20 gate's copy of the same two statements. */
void func_b17_801713A8(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        func_b17_801718D0();
        return;
    }
    if (d[0x25] == 0)
    {
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_801744FC)
        {
            e = D_801744EC;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_801744FC)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 2;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        if (func_800AA924(p, b, c, D_801744EC) != 0)
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
        if (*(u8 **)(c + 0x2C) != D_b17_80174494 && *(u8 **)(c + 0x2C) != D_b17_8017449C)
        {
            *(s16 *)(p + 0xA8) = 0;
            *(u8 **)(c + 0x2C) = D_b17_8017449C;
            func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          *(s16 *)(p + 0xA8));
        }
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(p + 0x98) &= ~8;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_801744A4) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(s16 *)(p + 0xA8) = 0;
            func_b17_80174250(p, (u8 *)b, c, d);
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
            func_b17_80171A98(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if ((s16)func_b17_801724C4(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b17_80171A98(p, b, c, d);
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 8:
        case 9:
            if ((s16)func_b17_801721E0(p, b, c, d) != 0)
            {
                return;
            }
            func_b17_801723A4(p, b, c, d);
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
            func_800AAF00(p, b, c, D_801744D4, func_b17_801713A8);
            return;
        default:
            func_b17_80171A98(p, b, c, d);
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

/* The banked room script's path-step advance, the relinked twin of the landed
   111-word func_b38_80171670 (csrc/ovl_dng_b38.c), dngtwin 0.951. 114 words,
   0 differing, ON THE FIRST COMPILE and WITH NO NEW DECLARATIONS -- every
   symbol it needs, both of this bank's facing tables included, was already in
   this unit's declaration block.

   The frame is that twin's exactly, `addiu $sp, $sp, -0x30' with $s0..$s4 and
   $ra saved in the same order, so its allocation facts are the transferable
   ones. It names three and all three are SHAPE facts and all three hold:
   p[0x9A] = 0xF unconditional, func_800A0818 called AFTER func_8009A21C with
   the `x'/`y' locals straight, and a dress with only the `!=' arm.

   Two deltas, both read off the listing: the dress tests the slot's table
   against BOTH bank-local facing tables and installs the second, and the
   post-call zero is at p + 0xA8 where b38's is at p + 0xA0.

   Measured on this body, both spellings priced, since a first-compile landing
   proves nothing about which of its choices mattered:
     as written below                              114 words /  0 aligned
     the two 0x2000 tests collapsed into one hoisted local
                                                   113 words / 43 aligned
     the p + 0xA8 zero hoisted ABOVE the func_80047784 call
                                                   114 words /  2 aligned
   The first is b38's own last fact at this body's scale -- it recorded 33 for
   110 words -- and the point carries: the DUPLICATED test reads like a device
   and is not one, because the second test is what keeps $a2 set up inside the
   func_8009A21C argument block. The second reproduces b38's 2 to the digit.

   MEASURED AND FREE, so it is NOT the reason this lands: the two-table dress
   test written as two nested `if's instead of one `&&', 114 words / 0. The
   two tests are on different symbols, so fold_truthop never sees them. */
void func_b17_801718D0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 x;
    s32 y;
    s32 n;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_b17_80174494 && *(u8 **)(c + 0x2C) != D_b17_8017449C)
        {
            *(u8 **)(c + 0x2C) = D_b17_8017449C;
            func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
   AND PASSED ON. func_b17_801721E0's call sets only $a0, $a2 and $a3, so its
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
   callees are load-bearing -- func_8009A8C0 2 words, func_b17_801721E0 1,
   func_8009A180 1 -- while those on the s16-returning func_800A0134,
   func_8009FD7C and func_8009A540 measure 0 and are left off.

   D_800E2970 is indexed as an array of a 0x14-byte record so the +0xC read
   keeps its own displacement; written as `*(u16 *)(D_800E2970 + j * 0x14 +
   0xC)' the constant folds into the `%lo' and the displacement goes to 0. */
void func_b17_80171A98(u8 *p, void *b, void *c, u8 *d)
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
        if (d[0x12] >= 2 || (s16)func_b17_801721E0(p, b, c, d) == 0)
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
   D_b17_80174494 facing table and re-aim through func_8009C93C.

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
s32 func_b17_801721E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
    e = D_b17_80174494;
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
void func_b17_801723A4(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
s32 func_b17_801724C4(u8 *a, void *b, void *c, s32 d)
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
        func_b17_80171A98(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b17_80171A98(p, b, c, p);
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
   func_b17_801713A8. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b17_80172608(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b17_801713A8;
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
void func_b17_8017269C(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        }
    }
}

/* The banked room script's six-state throw and tear-down, switched on
   p + 0x9B with cases 0..4 and 0xFF and a 5..254 default, driven by the
   D_8006CCD8 / D_8006CCE8 facing pair read ONCE before the dispatch and by
   four bank-local facing tables, one per re-dress: state 0 seeds the
   b-vector backwards off the pair and dresses from D_801744B4 -- or, if
   c + 0x14 has already reported 0x8000, jumps straight to 0xFF after
   raising c + 0x14's 0x6000 and kicking func_8009C12C; state 1 eases
   b + 0x14 up by a quarter a frame; state 2 bleeds all three b-vector
   components by an eighth and re-dresses twice, D_801744BC at the
   +0x96 == 4 mark and D_801744C4 once c + 0x14 reports 0xE000; state 3
   drives p + 0x90 and the b-vector forwards off the pair and kicks
   func_800A56E0(0x809) at +0x96 == 2; state 4 re-dresses from D_801744CC
   and hands over to 0xFF; state 0xFF eases onto the tile centre, releases
   D_80083460's +0xA claim and tears the actor back down onto
   func_b17_801713A8 through D_b17_8017449C. 326 words, 0 differing.
   1 copy (dngfam.py). This function owns NO jump table: six cases is over
   gcc 2.7.2's case_values_threshold but the 0xFF case puts the range at
   255, so expand_end_case emits the balanced compare chain the listing
   shows (beq 2 / slti 3 / beqz 0 / beq 1 / beq 4 / slti 4 / beq 0xFF) and
   ovl_dng_b17's only .rodata listing stays func_b17_801738E0's.

   THE DONOR IS THE LANDED 269-word func_b13_801725D0 (csrc/ovl_dng_b13.c),
   named by dngcallee (11 of 16 symbols, top tier) and NOT reachable by
   dngtwin, which reports no carved sibling at all at ratio 0.70 for this
   body. Its frame is this body's exactly -- -0x30 with $s0..$s6 and $ra,
   the same save SET and the same tail order ra,s6,s5,s4 -- so its
   allocation facts were in scope as well as its selection facts, and the
   three that this body exercises all carried: the ascending arm order, the
   `(*(u16 *)(d + 0x2A) >> 9) & 7' index shared in one `k' local with the
   two facing reads off it, and the d + 0x6D arms with the `== 0' mask arm
   first. The four extra facing tables and the absent func_800BCB04 call
   are the whole delta from it.

   MEASURED HERE, each alone against the body below, restored to
   byte-identical between every pricing edit:

     as written below                                   326 /  0 aligned
     `j * 0x60000' and `v * 0x60000' in state 2 instead
       of the two explicit shifts                       326 /  8 aligned
     state 0's two d + 0x1C clears merged into one
       `&= 0xF7FBFFFF'                                  323 / 10 aligned
     state 0xFF's two tile-centre eases written
       `<< 14' instead of `<< 15) >> 1'                 324 / 10 aligned

   THE MULTIPLY IS THE ONE THAT MATTERED AND IT IS A synth_mult CHOICE, not
   a word count: `j * 0x60000' and `(j << 18) + (j << 17)' are BOTH three
   instructions and both 326 words, but expand_mult picks
   `sll 1 / addu / sll 17' where the target has `sll 18 / sll 17 / addu'.
   So read WHICH three instructions the listing has, not how many -- the
   older lever "a multiply the listing shows as sll/addu/sll wants the plain
   constant" is the same mechanism read in the other direction, and its
   precondition is the sll/addu/sll ORDER. Here the order is sll/sll/addu
   and the plain constant is wrong.

   THE TWO d + 0x1C CLEARS ARE TWO STATEMENTS AND THE LISTING SAYS SO: one
   `lw', two `and's against two separately materialised constants, one `sw'.
   fold associates `(x & C1) & C2' into `x & (C1 & C2)' only inside one
   expression tree, so the merged spelling comes out three words SHORT.

   MEASURED AND FREE, so neither is why this lands:
     the c + 0x14 read inlined at both of state 0's tests instead of held
       in `t'                                           326 /  0 aligned
     state 4's `*(s32 *)(p + 0x90) = 0' placed after the p + 0x98 mask
       instead of before it                             326 /  0 aligned
   The first corroborates the existing lever that cse unifies two loads of
   one address whether or not the source hoists them -- the `t' local reads
   as source and buys nothing. The second is sched1 sinking an independent
   store into the `lhu's load-delay slot either way.
*/
void func_b17_80172700(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 k;
    s32 j;
    s32 v;
    s32 x;
    s32 y;
    u16 t;

    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    j = D_8006CCD8[k];
    v = D_8006CCE8[k];
    *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
    switch (p[0x9B])
    {
    case 0:
        t = *(u16 *)(c + 0x14);
        if ((t & 0x8000) != 0)
        {
            p[0x9B] = 0xFF;
            *(u16 *)(c + 0x14) |= 0x6000;
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            return;
        }
        if ((t & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801744B4;
        func_80047784(c, D_801744B4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        b->unk_0C = -j << 18;
        b->unk_10 = -v << 18;
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= 0xF7FFFFFF;
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        *(s16 *)(p + 0x96) = 4;
        b->unk_14 = (s32)0xFFFE8000;
        p[0x9B]++;
        break;
    case 1:
        b->unk_14 += b->unk_14 >> 2;
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 8;
        p[0x9B]++;
        break;
    case 2:
        b->unk_0C -= b->unk_0C >> 3;
        b->unk_10 -= b->unk_10 >> 3;
        b->unk_14 -= b->unk_14 >> 3;
        if (*(s16 *)(p + 0x96) == 4)
        {
            b->unk_0C = 0;
            b->unk_10 = 0;
            b->unk_14 = 0;
            *(u8 **)(c + 0x2C) = D_801744BC;
            func_80047784(c, D_801744BC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 4;
        b->unk_0C = (j << 18) + (j << 17);
        b->unk_10 = (v << 18) + (v << 17);
        *(u8 **)(c + 0x2C) = D_801744C4;
        func_80047784(c, D_801744C4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B]++;
        break;
    case 3:
        *(s32 *)(p + 0x90) += 0x80000;
        b->unk_0C += j << 18;
        b->unk_10 += v << 18;
        if (*(s16 *)(p + 0x96) == 2)
        {
            func_800A56E0(0x809);
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
        p[0x9B]++;
        break;
    case 4:
        *(s32 *)(p + 0x90) += 0x80000;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801744CC;
        func_80047784(c, D_801744CC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        b->unk_14 = 0;
        *(s32 *)(p + 0x90) = 0;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        p[0x9B] = 0xFF;
        break;
    case 0xFF:
        b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) >> 1;
        b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) >> 1;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s32 *)(d + 0x1C) |= 0x40000;
        func_800A2B04(b, c[0x24], c[0x25]);
        func_800AD594(d, 0x100);
        *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        D_8008346C = 0;
        func_800A4ACC(d);
        *(u8 **)(c + 0x2C) = D_b17_8017449C;
        func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                      0);
        if (*(s8 *)(d + 0x6D) == 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        else
        {
            D_800E3DE8 = d - 0x20;
        }
        break;
    }
}

/* The banked room script's four-state close-out, switched on p + 0x9B. State 0
   picks a gate byte out of d + 0x8 / 0xB / 0xE off the +0x46 state, aims the
   actor, dresses this bank's first sub-renderable and hands the gate to
   func_800A94A0; state 1 eases p + 0x92 toward -0x60 and re-dresses the slot
   from D_801744DC; state 2 waits on func_8003F270 and falls through into
   state 3, which eases toward -0x30, counts p + 0x96 down and tears the actor
   back down onto func_b17_801713A8. 395 words, 0 differing, ON THE FIRST
   COMPILE.

   OWNS jtbl_b17_80170838, the 7-entry table at rodata offset 0x30,
   `.align 3', for state 0's FIRST switch -- cases 1..7 over
   (d + 0x46) & 0x3FFF, with case 4 carrying the default arm's label. ENTRIES
   IN ROM ORDER: 1 -> the `g = d + 0x8' block, 2 -> `d + 0xB', 3 -> `d + 0xE',
   4 -> default (`g = NULL'), 5 -> `k = 1' then `d + 0x8', 6 -> `k = 1' then
   `d + 0xB', 7 -> `k = 1' then `d + 0xE', followed by one zero word of
   padding. The three `k = 1' preambles emit ahead of the four shared
   assignment blocks, which jump2 cross-joins with the `else' switch's. That
   SECOND switch -- the (d + 0x1C) & 0x2000 `else' -- has three cases and
   compiles to a compare chain, so this unit gains ONE table here and not two.

   THE TWIN IS THE LANDED 304-word func_b38_80172A40 (csrc/ovl_dng_b38.c),
   dngtwin 0.824 and 91 words short, so the listing was read line by line
   rather than leant on. ITS FRAME DOES NOT MATCH: b38 is -0x38 with
   $s0..$s5, this body is -0x40 with $s0..$s7, because it carries TWO extra
   callee-saved pointers -- the sub-node's payload at
   *(u8 **)(p + 0xA4) + 0x20 and that payload's first sub-renderable at
   + 0x28. ELEVEN of its fourteen priced spellings still held and SEVEN
   reproduce its figure to the digit, which is the evidence that all of them
   are SELECTION facts following the shape rather than the frame. Measured
   here, b38's own number last:

     as written below                               395 /  0  |   0
     `s32 k'                                        394 /  8  |   7
     `u8 k'                                         395 /  1  |   1
     `s8 k'                                         395 /  0  |   0
     `k > 0' instead of `k != 0'                    395 /  1  |   1
     inner three-case switch written 1, 2, 3        397 / 11  |  10
     `D_8008346C = 0' after the 0xA6 decrement      396 / 10  |   9
     the D_8006DE24 record indexed inline, with no
       tbl / i / e locals                           395 /  1  |   1
     the facing table read through *(u8 **)(c + 0x2C)
       instead of the bare symbol                   395 /  0  | free
     the two `&= ~' written as one `&= ~0x8040000'  392 /  9  |  54
     the two `|=' written as one `|= 0x8040000'     393 /  6  |   6
     `p[0x9B]++' after the +0x98 0x8 raise          395 /  7  |   7
     func_800DAE44 before the c + 0x14 clear        395 /  5  |  50
     the +0x92 ease through `s16 *' not `u16 *'     395 /  0  | free
   Only three rows came out at a different price and all three are cheaper
   here, so no fact of its inverted.

   THIS BODY'S OWN, none of which b38 has:
     `w' and `t' computed inside the arms that use them rather than
       unconditionally at the top                   394 words / 32 aligned
     state 1's `*(s16 *)(p + 0x96) = 1' written `= p[0x9B]'
                                                    396 words / 10 aligned
     state 0's sub-renderable dress moved BELOW the func_800A94A0 gate
                                                    395 words / 14 aligned
     `__builtin_abs' as two explicit `if (x < 0) x = -x;'
                                                    394 words / 15 aligned
   The first is the whole reason the frame is -0x40: both pointers have to be
   live across all four states for $s6 and $s7 to be spent on them. The second
   is worth knowing as a reading hazard -- the ROM's `sh $s0, 0x96($s2)'
   stores the SWITCH VALUE's register because after `beq $s0, $a0' cse knows
   $s0 holds 1, so the source really said `= 1' and the apparent read of
   p + 0x9B is an artefact.

   MEASURED AND FREE, so none is why this lands: `w' and `t' derived through
   one intermediate node local instead of two loads of p + 0xA4 (395 / 0, cse
   unifies the load); state 1's 0xE000 gate as a positive `if' rather than an
   early `break' (395 / 0); and state 3's 0x80 raise written
   `!(A > 0 && B == 0)' instead of `A <= 0 || B != 0' (395 / 0).

   Deltas from b38, read off the listing. A FOURTH STATE: b38's case 2 splits
   into this body's 2 and 3, the func_8003F270 gate falling through into the
   ease. State 0's success path passes 5 to func_800DAE44 where b38 passes 4,
   and sets no p + 0x96 at all. State 0 gains the sub-renderable dress --
   clear the payload's +0x4 0x8000, install D_801744E4, call func_80047784
   with a literal 0 -- ahead of func_800A94A0. State 1 is new entirely. State
   3 gains the +0x98 0x80 raise's inverted gate, the 0x4000 / p + 0xA8 == 2
   gate, the payload's +0x4 0x8000 raise, the `*(u16 *)(p + 0x98) &= 0xBFFF'
   inside the dress, a second facing table in the dress test, and the extra
   `(p + 0x98) & 0x4000' gate after the D_80083460 + 0xC one. Tables:
   D_801744E4, D_801744DC, D_b17_80174494 and D_b17_8017449C. */
void func_b17_80172C18(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *q;
    u8 *g2;
    u8 *w;
    u8 *t;
    s16 k;
    s32 i;
    s32 x;
    s32 y;

    k = 0;
    w = *(u8 **)(p + 0xA4) + 0x20;
    t = *(u8 **)(p + 0xA4) + 0x28;
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
                    *(s32 *)(d + 0x60) =
                        func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            *(u16 *)(w + 0x4) &= 0x7FFF;
            *(u8 **)(t + 0x2C) = D_801744E4;
            func_80047784(t, D_801744E4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            if (func_800A94A0(d, g, k, p + 0x98) == 0)
            {
                break;
            }
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_800DAE44((u8 *)b, 5);
            func_800A56E0(0x703);
            p[0x9B]++;
            *(u16 *)(p + 0x98) |= 0x8;
            *(s32 *)(d + 0x1C) &= ~0x8000000;
            *(s32 *)(d + 0x1C) &= ~0x40000;
        }
        else
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        *(u16 *)(p + 0x92) += (-0x60 - *(s16 *)(p + 0x92)) >> 3;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_801744DC;
        func_80047784(c, D_801744DC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 1;
        p[0x9B]++;
        break;
    case 2:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B]++;
    case 3:
        *(u16 *)(p + 0x92) += (-0x30 - *(s16 *)(p + 0x92)) >> 3;
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0 || (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x80;
        }
        if (((*(u16 *)(p + 0x98) & 0x4000) == 0 && *(s16 *)(p + 0xA8) == 2) ||
            (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(p + 0x98) |= 0x4000;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(u16 *)(w + 0x4) |= 0x8000;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= ~0x8;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        *(s32 *)(d + 0x1C) |= 0x40000;
        if (*(u8 **)(c + 0x2C) != D_b17_80174494 && *(u8 **)(c + 0x2C) != D_b17_8017449C)
        {
            *(s16 *)(p + 0xA8) = 0;
            *(u16 *)(p + 0x98) &= 0xBFFF;
            *(u8 **)(c + 0x2C) = D_b17_80174494;
            func_80047784(c, D_b17_80174494[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          *(s16 *)(p + 0xA8));
        }
        g2 = (u8 *)&D_80083460;
        if (*(s32 *)(g2 + 0xC) != 0)
        {
            break;
        }
        if ((*(u16 *)(p + 0x98) & 0x4000) == 0)
        {
            break;
        }
        *(u16 *)(g2 + 0xA) -= 1;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) > 0)
        {
            d[0x6D] = d[0x6D] - 1;
        }
        d[0x73] = 0;
        d[0x72] = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A56E0(0xB4);
        break;
    }
}

/* The banked room script's three-state glide, switched on p + 0x9B: state 0
   seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair, state 1
   decays it by the same pair shifted 14, state 2 eases onto the tile centre,
   re-dresses from the bank-local D_b17_8017449C and installs
   func_b17_801713A8. The resident overlay's func_800D9C7C
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
   flag work and the D_b17_8017449C dress that func_800D991C's tail has.

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
void func_b17_80173244(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
            func_800AAA54(p, b, c, D_801744AC);
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
            func_800AAA54(p, b, c, D_801744AC);
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
        *(u8 **)(c + 0x2C) = D_b17_8017449C;
        func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        return;
    }
}

/* The banked room script's four-state death throw, switched on p + 0x9B and
   driven by the D_8006CCD8 / D_8006CCE8 facing pair read ONCE before the
   dispatch: state 0 seeds the velocity from the pair and holds behind the
   D_8008346A gate, state 1 re-dresses the renderable to 0x60 / -0x80 / 0xC
   through the func_800ACF88 arm and kicks func_800A56E0(0x805), state 2
   waits on c + 0x14's 0x6000 and arms the +0x96 fade, state 3 fades
   c + 0xC..0xE out of p + 0x96 and, when the fade drops under 0x10,
   releases D_80083460's +0x10 claim and tears the actor down. 149 words,
   0 differing, ON THE FIRST COMPILE. 1 copy (dngfam.py).

   This function does NOT own a jump table: four cases is under gcc 2.7.2's
   case_values_threshold, so the dispatch is the balanced compare chain
   (beq 1 / slti 2 / beqz 0 / beq 2 / beq 3) the listing shows, and
   ovl_dng_b17's only .rodata listing is func_b17_801738E0's.

   THE TWIN IS THE LANDED 156-word func_b38_80173294 (csrc/ovl_dng_b38.c),
   dngtwin 0.925 and its top pick; its own root is the 164-word
   func_b04_80173724 (csrc/ovl_dng_b04.c), which owns a jump table because
   it has FIVE states. DELTAS FROM b38, read off this listing: no b + 0x14
   `+= 0xC000' drift ahead of the switch; case 0 seeds `= 0x8000' where b38
   has `= 0xFFFE8000'; and case 1 ASSIGNS c + 0x12 where b38 decrements it.
   Everything else is b38 statement for statement.

   THE LARGEST FACT IS THE PRE-SWITCH HOIST, AND NEITHER TWIN PRICES IT.
   Both `lh's of the facing pair sit above the `lbu' of p + 0x9B, so the two
   reads are LOCALS ASSIGNED BEFORE THE SWITCH even though only case 0 uses
   them. Inlined into case 0 this body is 152 words / 67 aligned differences.

   THE MEM_IN_STRUCT_P FACT CARRIES AND IS THREE FACTS, as b38 found, but at
   this body's own prices -- each measured alone against the body below:

     all three b-vector stores as COMPONENT_REFs  147 words /   9 aligned
     only b->unk_14 a COMPONENT_REF               147 words /   9 aligned
     only b->unk_0C a COMPONENT_REF               147 words /  10 aligned
     only b->unk_10 a COMPONENT_REF               147 words /  10 aligned

   Any ONE of them lets sched hoist the fixed scalar D_8008346A's `lh' above
   the store and into the `bnez's delay slot; the target leaves a `nop'
   there and another after the `lh'. b38's companion fact -- that a b + 0x14
   `+=' is free because nothing fixed-address is read across it -- is NOT
   APPLICABLE here: this body has no `+=' site on the vector at all.

   b38'S OTHER THREE FACTS HOLD AND TWO REPRODUCE ITS PRICES TO THE DIGIT:
     case 1's two d + 0x14 tests written `(v & 0x4000) && (v & 0x20000000)
       == 0' instead of nested, fold_truthop merging them into one `and'
       against 0x20004000                        148 words /  12 aligned
       (b38 recorded 12)
     case 3's fade byte read three times instead of through the local `v'
                                                 153 words /   6 aligned
       (b38 recorded 6)
     the two d + 0x1C clears written as one `&= ~0x8040000'
                                                 146 words /   7 aligned
       (b38 recorded 7; two other landed bodies in b38 record 54 and 6)

   ONE SITE NEITHER TWIN HAS: c + 0x12 IS AN ASSIGNMENT OF THE UNSIGNED
   CONSTANT. `*(u16 *)(c + 0x12) = 0xFF80' emits `ori $v1, $zero, 0xFF80';
   `*(s16 *)(c + 0x12) = -0x80' emits `addiu $v1, $zero, -0x80' and is
   149 words / 1 aligned difference. Same stored halfword, different opcode.

   MEASURED AND FREE, so NOT why this lands: the facing pair spelled as the
   byte offset the listing literally shows,
   `*(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(d + 0x6A) >> 8) & 0xE))', rather
   than the array ref -- 149 / 0, as both twins also found, because combine
   folds the array ref's `sll 1' into the `srl'/`andi' pair. */
void func_b17_80173614(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(s32 *)((u8 *)b + 0xC) = x << 16;
        *(s32 *)((u8 *)b + 0x10) = y << 16;
        *(s32 *)((u8 *)b + 0x14) = 0x8000;
        if (D_8008346A != 0)
        {
            return;
        }
        p[0x9B] += 1;
        /* fallthrough */
    case 1:
        if (*(s32 *)(d + 0x14) & 0x4000)
        {
            if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
            {
                func_800ACF88(d);
            }
        }
        func_800A56E0(0x805);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        *(s16 *)(c + 0x10) = 0x60;
        *(u16 *)(c + 0x12) = 0xFF80;
        *(u16 *)(c + 0x14) |= 0xC;
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0x80;
        p[0x9B] += 1;
        return;
    case 3:
        *(s32 *)(d + 0x1C) |= 0x10000000;
        v = p[0x96];
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        if ((s16)(*(u16 *)(p + 0x96) -= 0x10) < 0x10)
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
void func_b17_80173868(void)
{
    func_800AB538();
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b17_80173888(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b17_801738A8(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}

/* The banked room script's seven-state refusal machine, switched on
   p + 0x9B with cases 0..4 and 16 and a 5..15 default: state 0 either
   zeroes p + 0x90 and dresses from D_801744FC on c + 0x14's 0x8000 (then
   steps straight to 2) or raises p + 0x98's 0x8, clears d + 0x1C's
   0x8000000 and 0x40000 and dresses from D_801744F4; state 1 eases
   b + 0x14 up to 0xFFFFF in 0x20000 steps and, once p + 0x90 goes
   non-negative, re-dresses from D_801744FC and steps to 2; state 2 is the
   long refusal chain (func_80042900 / func_800AA6B4 / func_800AA79C /
   func_800A2C34 / func_800AA258 / func_800AA888 / func_8009A180 /
   func_8009FD40 / func_800A6D30 / func_80042B68) with the D_801744F4
   dress, the 0x40000 raise and the +0x9B bump written out TWICE, once in
   the `func_80042900 == 0' arm and once after `again:'; state 3 re-dresses
   from D_801744EC; state 4 arms +0xA8, drops p + 0x98's 0x4000 and dresses
   from D_b17_8017449C before jumping to 16; state 16 releases
   D_80083460's +0xA claim and hands the actor back to
   func_b17_801713A8. 396 words, 0 differing. 1 copy (dngfam.py).

   OWNS ONE TABLE. jtbl_b17_80170858, `.align 3', SEVENTEEN entries =
   17 cases + 0 pad (jtcheck; the dispatch bound `sltiu $v0, $v1, 0x11'
   agrees), with SEVEN distinct targets: cases 0, 1, 2, 3, 4 and 16 have
   their own arms and cases 5..15 all carry the DEFAULT arm's label, which
   is also the range-check `beqz's target -- so those eleven are gcc
   padding to maxval and there is nothing to hand-settle in them.
   ARM ORDER WAS SETTLED BY A STANDALONE COMPILE, not by the (blind) 0:
   `tools/cdkgcc -O2 -G0 -S' emits the seven labels in the definition order
   case0 < case1 < case2 < case3 < case4 < case16 < default, which is the
   listing's own entry-address order 8017393C < 801739E4 < 80173A80 <
   80173D80 < 80173DE0 < 80173E74 < 80173EEC. Writing the arms ascending by
   case value, as below, is therefore also entry-address order here.

   THE DONOR IS THE IN-UNIT SIBLING func_b17_80173F10, 208 words, sitting
   directly below this body -- the same refusal chain with three states
   instead of seven. Its statements carried verbatim, including
   `func_b17_80174250(p, (u8 *)b, c, d)' and the four separate
   `&D_80083460' pointer locals. The 44-member dngcallee family (17 of 21
   symbols) was the route in, and func_b05_8017388C is the exact
   frame match -- -0x28, saves s1,s3,s2 + s0,ra,s4, a0->p/$s1, a1->b/$s3,
   a2->c/$s2, a3->d/$s0 -- which is what picked it out of 44 rows.

   MEASURED HERE, each alone against the body below, restored to
   byte-identical between every pricing edit:

     as written below                                   396 /   0 aligned
     state 16's `b->unk_14 = 0' as a COMPONENT_REF
       instead of the plain cast                        395 /  17 aligned
     the `again:' tail's D_80083460 bump taken through
       the gate's own `g' instead of its own local      394 /  17 aligned
     `p[0x9B] += 1' shared through ONE `goto bump'
       label instead of written out at all four sites   397 / 102 aligned

   STATE 16's ZERO STORE MUST BE A PLAIN CAST, and this is the
   MEM_IN_STRUCT_P dependence lever, not an allocation: written
   `b->unk_14 = 0' the COMPONENT_REF makes MEM_IN_STRUCT_P true,
   true_dependence excuses the fixed-address `lhu' of D_80083460 + 0xA
   against it, sched hoists that load above the store into its own
   delay slot and the body comes out ONE WORD SHORT where the target leaves
   a `nop'. 15 of those 17 rows are register-only branch-target rows -- the
   relocation wake of the one missing word -- and only 2 are the fault.
   `*(s32 *)((u8 *)b + 0x14) = 0' keeps the dependence. Note the opposite
   polarity 40 words up: state 1's `b->unk_14 = 0' is correct as the field
   ref, because nothing fixed-address is read across it.

   THE SIBLING'S `goto bump' FACT HOLDS AND AT SIX TIMES ITS PRICE, so
   carry the direction and re-measure the magnitude. The sibling records 17
   aligned rows for the shared label on its 208-word body; here it is 102
   (72 of them register-only) AND one word LONG, because beside the
   p/b $s1/$s2 exchange the sibling describes, the lost references also
   stop jump2 cross-joining state 0's and state 1's
   `*(u16 *)(h + 0xA) -= 1; p[0x9B] = 2' tails into the one block the
   target has. The prologue reads the inversion directly: `sw $s2, 0x18'
   first where the target has `sw $s1, 0x14'.

   AND ITS SHARED-`g' FACT INVERTS. The sibling shares the gate's
   `&D_80083460' local with its state-1 tail and says so; here that
   spelling is TWO WORDS SHORT at 17 rows, and the listing says why before
   any compile: $s4 holds `g' only across the two `(g + 0x2)' tests and
   dies at the second, while the `again:' tail re-materialises
   `%hi/%lo(D_80083460)' with $s4 still in the frame. A fresh materialisation
   where a live callee-saved register already holds the address NAMES a
   second local.

   CARRIED BUT NOT RE-PRICED: the `*(s16 *)(p + 0xA8) = 0' store sits
   BETWEEN the func_800AA888 call and the func_b17_80174250 hand-over, the
   sibling's one-word delta, which it prices at 19 aligned rows written
   before the call. No pricing edit was made for it here because that
   three-line construct occurs THREE times in this unit -- the other two in
   already-landed bodies -- so no unique anchor inside this function's span
   was available, and an edit anchored on it would have silently rewritten
   a finished neighbour. The position is read off the listing instead: the
   `sh $zero, 0xA8($s1)' is the first instruction after the jal's delay
   slot.
*/
void func_b17_801738E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *m;
    u8 *n;
    u8 *q;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(p + 0x90) = 0;
            *(u8 **)(c + 0x2C) = D_801744FC;
            func_80047784(c, D_801744FC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0xA) -= 1;
            p[0x9B] = 2;
            return;
        }
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= 0xF7FFFFFF;
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
        b->unk_14 = (s32)0xFFF80000;
        *(u8 **)(c + 0x2C) = D_801744F4;
        func_80047784(c, D_801744F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (b->unk_14 <= 0xFFFFF)
        {
            b->unk_14 += 0x20000;
        }
        if (*(s32 *)(p + 0x90) < 0)
        {
            return;
        }
        *(s32 *)(p + 0x90) = 0;
        b->unk_14 = 0;
        *(u8 **)(c + 0x2C) = D_801744FC;
        func_80047784(c, D_801744FC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        func_800A9A0C(d);
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        p[0x9B] = 2;
        return;
    case 2:
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_801744F4;
            func_80047784(c, D_801744F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(s32 *)(d + 0x1C) &= ~0x200;
                *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
                return;
            }
            m = (u8 *)&D_80083460;
            *(u16 *)(m + 0xA) += 1;
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
            func_b17_80174250(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801744F4;
        func_80047784(c, D_801744F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s32 *)(d + 0x1C) &= ~0x200;
            *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
            return;
        }
        n = (u8 *)&D_80083460;
        *(u16 *)(n + 0xA) += 1;
        p[0x9B] += 1;
        return;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_801744EC;
        func_80047784(c, D_801744EC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 4:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_14 = (s32)0xFFF80000;
        *(s16 *)(p + 0xA8) = 8;
        *(u16 *)(p + 0x98) &= 0xBFFF;
        *(u8 **)(c + 0x2C) = D_b17_8017449C;
        func_80047784(c, D_b17_8017449C[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                      0);
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= 0xF7FFFFFF;
        p[0x9B] = 0x10;
        return;
    case 16:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        *(u16 *)(p + 0x98) |= 0x4000;
        *(s32 *)((u8 *)b + 0x14) = 0;
        q = (u8 *)&D_80083460;
        *(u16 *)(q + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~0x200;
        *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        return;
    }
}

/* The banked room script's three-state tail, state index at +0x9B, and the
   near-twin of the landed func_b03_801736F4 (csrc/ovl_dng_b03.c), itself the
   banked copy of the resident overlay's func_800DA398: state 0 re-dresses the
   renderable from the bank-local D_801744F4 facing table, state 1 is the long
   refusal chain with the D_801744EC dress/flag/bump tail written out twice,
   state 2 hands the actor back to func_b17_801713A8. 208 words, 0 differing.

   THE DELTA FROM func_b03_801736F4 IS ONE WORD: the +0x80000 arm clears the
   slot's halfword at +0xA8 BETWEEN the func_800AA888 call and the
   func_b17_80174250 hand-over. Written before the call instead it is 19
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
void func_b17_80173F10(u8 *p, UnkVec *b, u8 *c, u8 *d)
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
        *(u8 **)(c + 0x2C) = D_801744F4;
        func_80047784(c, D_801744F4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9B] += 1;
        return;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_801744EC;
            func_80047784(c, D_801744EC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
            func_b17_80174250(p, (u8 *)b, c, d);
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
        *(u8 **)(c + 0x2C) = D_801744EC;
        func_80047784(c, D_801744EC[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
        *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
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
   call emits the bare `jal` either way); hoisting D_b17_80174494 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b17_80174250(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_801744FC && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_b17_80174494;
        func_80047784(c, D_b17_80174494[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
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
void func_b17_80174318(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b17_801713A8;
        }
    }
}

/* 14 words, 0 differing. */
void func_b17_8017437C(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}

/* 14 words, 0 differing. */
void func_b17_801743B4(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}

/* 14 words, 0 differing. */
void func_b17_801743EC(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}

/* 14 words, 0 differing. */
void func_b17_80174424(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}

/* 14 words, 0 differing. */
void func_b17_8017445C(u8 *p)
{
    if (func_800AC7EC(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b17_801713A8;
    }
}
