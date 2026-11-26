#include "common.h"

s32 rand(void);

u8 *func_8003FD64(s32, u8 *);

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
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
void func_s72_80024BF4(u8 *p, UnkVec *v, UnkSlot *s, s16 d);

s32 func_800BCE7C(u8 *);
typedef struct UnkDng264D4Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng264D4Vec;
typedef struct UnkDng264D4SP
{
    u8 pad_00[0x30];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x28];
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng264D4Vec unk_98[4];
} UnkDng264D4SP;

/* The dungeon's height cast turned into a PICKER: clamp (x, z) onto the
   0x40-square cell grid, read the 6-byte row at D_8008333C[0] and walk that
   cell's 0x18-byte polygon records, keeping the lowest plane above the
   caller's floor -- but RETURN THE WINNING RECORD rather than its height.
   csrc/ovl_dungeon_800B6AC4.c's func_800BCB04 is the line-for-line template
   and all of its measured facts hold here: the record advance is
   `n * 0x18 + 0x18' and not `(n + 1) * 0x18', the bases are INTEGER so every
   `addu' is index-first, and the row address is written out at each of its
   three uses because the `sh' to +0x152 invalidates cse's load of +0x150.
   Two deltas off that twin, both read off this listing: the four grid clamps
   on ix and iy are GONE (the scratchpad's +0x148 is seeded with 0x400 as the
   function's FIRST statement instead of after them), and the result is the
   record pointer `r', seeded NULL and bumped alongside +0x148.

   THE BODY IS NESTED IN `if (cell != 0)' SO THERE IS EXACTLY ONE `return r'.
   Spelled with an early `return r' instead, the body is still 205 words but
   `r' and `norms' hold each other's callee-saved register: 16 aligned
   differences, every row one of that pair. It is global.c's
   `allocno_compare' -- both are 6 refs, `r' over 123 insns and `norms' over
   131, so `r' scores 975 against 916 and takes $s3. Merging the two returns
   drops `r' to 5 refs and 813, and `norms' takes $s3. 205 words,
   0 differing. */
UnkNode *func_8003FC64(s32);
extern u8 D_80045340[];
void func_s72_800247C0(u8 *, UnkVec *, u8 *);
void func_s72_80024AE8(UnkNode *src, UnkVec *b);
void func_s72_80024398(u8 *p, UnkVec *b, u8 *c);
void func_s72_800251F4(u8 *, s16, s16, s16);
s32 func_800BCB04(s32, s32, s16);
void func_800419EC(u16, u32);
s32 func_800A56E0(s32);
void func_800478B8(u8 *);
void func_8003DB94(void *, void *, s32);
extern u8 D_800DE870[];
extern s16 D_8002992E;
void func_s72_80025800(u8 *p, UnkVec *b, u8 *r);
s32 func_800C9034(u8 *, UnkVec *, UnkSlot *);
extern u8 D_s72_80024028[];
extern u8 D_80083498[];
extern u8 D_80028664[];
extern u32 D_80028630;
typedef struct UnkDngS72Dirs
{
    u8 unk_00[0x20];
} UnkDngS72Dirs;
typedef struct UnkDng25FB0Rot
{
    s16 m[3][3];
} UnkDng25FB0Rot;

typedef struct UnkDngS72Poly
{
    s16 m[12];
} UnkDngS72Poly;
typedef struct UnkDngS72Grid
{
    u16 m[3][3][2];
} UnkDngS72Grid;
extern u8 D_s72_80024004[];
extern s16 D_8002992C;
s32 func_800C77D0(void *, u8 *, s16, s32);
void func_80040490(void *, RECT *);
void func_8003F80C(u32, s32, s32, s16);
void func_800542BC(void);
extern u8 D_80083780[];
extern u8 D_8006E8A0[];
extern u8 D_8006EE50[];
extern u16 D_80082E94;
extern s16 D_80082E86;
extern Unk80083462 D_80083460;
void func_s72_80025FB0(MATRIX *d, u8 *f);
void func_s72_80026060(u8 *s, s16 *b, s16 lo, s16 hi);
void func_s72_8002612C(u8 *s, s16 d);

u8 *func_s72_80024064(s32 x, s32 z, s16 c)
{
    u8 *sp = (u8 *)0x1F800000;
    u8 *m = (u8 *)D_8008333C;
    u32 verts;
    u32 cells;
    u32 norms;
    u8 *p;
    u8 *r;
    SVECTOR *n;
    SVECTOR *v;
    u16 px;
    u16 py;

    r = 0;
    *(s32 *)(sp + 0x148) = 0x400;
    *(u16 *)(sp + 0x140) = x & 0x3F;
    *(u16 *)(sp + 0x142) = z & 0x3F;
    *(s32 *)(sp + 0x14C) = c;
    *(u16 *)(sp + 0x150) = (u16)x >> 6;
    verts = *(u32 *)(m + 0x8);
    cells = D_8008333C[0];
    norms = *(u32 *)(m + 0xC);
    *(u16 *)(sp + 0x150) =
        *(u16 *)(sp + 0x150) + (((u16)z >> 6) << *(s16 *)(m + 0x14));
    if (*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) != 0)
    {
        *(u16 *)(sp + 0x152) = -*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells + 2);
        r = *(u8 **)(*(u16 *)((s16) * (u16 *)(sp + 0x150) * 6 + cells) * 4 +
                     *(u32 *)(m + 0x4));
        p = r;
        while (*(s16 *)(*(u16 *)(p + 0x10) * 8 + norms + 4) < -0x7FF)
        {
            if (!(*(u8 *)(p + 0x17) & 1))
            {
                px = *(u16 *)(sp + 0x140);
                py = *(u16 *)(sp + 0x142);
                *(s32 *)(sp + 0x154) = *(s32 *)(*(u16 *)(p + 0x0) * 8 + verts);
                *(u16 *)(sp + 0x154) = *(u16 *)(sp + 0x154) - px;
                *(u16 *)(sp + 0x156) = *(u16 *)(sp + 0x156) - py;
                *(s32 *)(sp + 0x158) = *(s32 *)(*(u16 *)(p + 0x2) * 8 + verts);
                *(u16 *)(sp + 0x158) = *(u16 *)(sp + 0x158) - px;
                *(u16 *)(sp + 0x15A) = *(u16 *)(sp + 0x15A) - py;
                *(s32 *)(sp + 0x15C) = *(s32 *)(*(u16 *)(p + 0x6) * 8 + verts);
                *(u16 *)(sp + 0x15C) = *(u16 *)(sp + 0x15C) - px;
                *(u16 *)(sp + 0x15E) = *(u16 *)(sp + 0x15E) - py;
                *(s32 *)(sp + 0x160) = *(s32 *)(*(u16 *)(p + 0x4) * 8 + verts);
                *(u16 *)(sp + 0x160) = *(u16 *)(sp + 0x160) - px;
                *(u16 *)(sp + 0x162) = *(u16 *)(sp + 0x162) - py;
                if (func_800BCE7C(sp) != 0)
                {
                    n = (SVECTOR *)(*(u16 *)(p + 0x10) * 8 + norms);
                    v = (SVECTOR *)(*(u16 *)(p + 0x0) * 8 + verts);
                    *(s32 *)(sp + 0x144) =
                        (n->vx * (v->vx - (s16) * (u16 *)(sp + 0x140)) +
                         n->vy * (v->vy - (s16) * (u16 *)(sp + 0x142)) + n->vz * v->vz) /
                        n->vz;
                    *(s32 *)(sp + 0x144) =
                        *(s32 *)(sp + 0x144) + (s16) * (u16 *)(sp + 0x152);
                    if (*(s32 *)(sp + 0x144) >= *(s32 *)(sp + 0x14C))
                    {
                        if (*(s32 *)(sp + 0x144) < *(s32 *)(sp + 0x148))
                        {
                            r = p;
                            *(s32 *)(sp + 0x148) = *(s32 *)(sp + 0x144);
                        }
                    }
                }
            }
            if ((*(u8 *)(p + 0x16) & 0xF) == 1)
            {
                if (*(s8 *)(p + 0x17) < 0)
                {
                    break;
                }
                if ((*(u8 *)(p + 0x16) & 0xF0) != 0)
                {
                    p = p + ((*(u8 *)(p + 0x16) >> 4) * 0x18 + 0x18);
                    continue;
                }
            }
            p = p + 0x18;
        }
    }
    return r;
}

/* func_s72_800244BC's node driver: on each tick poke D_8002992E, then pulse
   the slot's +0x1C/+0x1E scale in five bands of the payload's +0x28 fuse --
   grow by 0x88 at 0x1F and up and again over 0x10..0x18, shrink by 0x88 over
   0x19..0x1E and over 0xA..0xF, and below 0xA grow each axis separately with
   a clamp at 0x1000 -- and when the fuse runs out slam both axes to 0x1000
   and retire the actor.  73 words, 0 differing.

   No twin anywhere: dngtwin finds nothing at 0.70 and the five-band pulse is
   unique in the block.  The teardown is the same
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;' as
   func_s72_800246B0 and csrc/ovl_dng_s69.c.

   | spelling                                              | words | differing |
   | as written below                                       |  73   |     0     |
   | `t = *(s16 *)(p + 0x28)' as a plain cast               |  73   |     3     |
   | the teardown stores written +0x1C then +0x1E           |  73   |     2     |
   | the bands as nested ascending `t < K' tests            |  80   |    70     |

   THE FUSE READ MUST BE AN ARRAY REF -- the same MEM_IN_STRUCT_P hoist as
   func_s72_80024BA0 and func_s72_800246B0, and the third body in this unit
   to need it.  The target puts `lh $a2, 0x28($a0)' between the `lui' and the
   `sh' of `D_8002992E = 1', i.e. the fuse load is hoisted ABOVE the
   fixed-address scalar store and the store fills its load delay.  A plain
   cast keeps true_dependence's dependence and the three rows are that one
   instruction rotating through the block.

   AND THE BAND CHAIN MUST BE WRITTEN DESCENDING.  `if (t >= 0x1F) ... else
   if (t >= 0x19) ...' emits the target's `slti'/`bnez' pairs with the two
   GROW arms duplicated and the two SHRINK arms cross-joined into one block
   at .Ls72_800243FC; the equivalent nested ascending form is seven words
   longer because jump2 cross-joins a different pair.  Measured and FREE, so
   not why this lands: `t' as `s32' (73 / 0 -- one `lh' either way, since the
   value is only ever compared against constants in range). */
void func_s72_80024398(u8 *p, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *r;
    s16 t;

    D_8002992E = 1;
    t = ((s16 *)p)[0x14];
    n = *(UnkNode **)(p + 0x20);
    r = (u8 *)n->unk_0C;
    if (t >= 0x1F)
    {
        *(s16 *)(r + 0x1C) += 0x88;
        *(s16 *)(r + 0x1E) += 0x88;
    }
    else if (t >= 0x19)
    {
        *(s16 *)(r + 0x1C) -= 0x88;
        *(s16 *)(r + 0x1E) -= 0x88;
    }
    else if (t >= 0x10)
    {
        *(s16 *)(r + 0x1C) += 0x88;
        *(s16 *)(r + 0x1E) += 0x88;
    }
    else if (t >= 0xA)
    {
        *(s16 *)(r + 0x1C) -= 0x88;
        *(s16 *)(r + 0x1E) -= 0x88;
    }
    else
    {
        if (*(u16 *)(r + 0x1C) < 0x1000)
        {
            *(s16 *)(r + 0x1C) += 0x88;
            if (*(u16 *)(r + 0x1C) > 0x1000)
            {
                *(s16 *)(r + 0x1C) = 0x1000;
            }
        }
        if (*(u16 *)(r + 0x1E) < 0x1000)
        {
            *(s16 *)(r + 0x1E) += 0x88;
            if (*(u16 *)(r + 0x1E) > 0x1000)
            {
                *(s16 *)(r + 0x1E) = 0x1000;
            }
        }
    }
    if ((*(s16 *)(p + 0x28) -= 1) <= 0)
    {
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_s72_80024AE8 with a different handler and a dressed slot: spawn a
   0x212 node driven by func_s72_80024398, seed the payload's +0x28/+0x2A
   with 0x27, park the source node at +0x20, copy the vector, then set the
   slot's +0x1E/+0x1C scale to 0x1000, its three colour bytes to 0x80 and
   raise 0x80 in +0x14.  52 words, 0 differing, FIRST COMPILE -- it is
   func_s72_80024AE8 above plus six words, and that body landing first is
   why this one did.

   TWO FACTS, both measured here.

   | spelling                                              | words | differing |
   | as written below                                       |  52   |     0     |
   | the three 0x80 bytes written 0xC, 0xD, 0xE             |  52   |     2     |
   | the +0x14 flag raised FIRST instead of last            |  55   |    20     |

   THE +0x14 FLAG FACT INVERTS AGAINST THE DONOR.  csrc/ovl_dng_b08.c's
   func_b08_801743BC records the position of its `r->unk_14' flag store as
   MEASURED AND FREE ("sched1 hoists the `lhu 0x14' to fill the load delay
   either way").  Here it is three words and 20 rows: this body has five
   more stores between the `lw 0xC($s0)' and the flag, so the `lhu' has
   somewhere to be hoisted TO and the target does constrain it.  That is the
   brief's "a fact's price depends on whether your body already does the
   device's job", with the sign the other way round.

   NOT MEASURABLE, so not a fact: the three bytes written as the chain
   `*((u8 *)r + 0xC) = *((u8 *)r + 0xD) = *((u8 *)r + 0xE) = 0x80' is also
   52 / 0 -- a chain emits the INNER lvalue first, so it is the same
   0xE/0xD/0xC code, not a second spelling. */
void func_s72_800244BC(UnkNode *src, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x28) = 0x27;
        *(s16 *)(q + 0x2A) = 0x27;
        *(UnkNode **)(q + 0x20) = src;
        n->unk_10 = (u32)func_s72_80024398;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_14 |= 0x80;
    }
}

/* The bank's "random value below n": six masked draws from rand,
   picked by the size of n, each retried until it falls under n. The mask and
   shift pairs are read straight off the listing -- 0xC >> 2, 0xF0 >> 4,
   0xFC >> 2, 0xFF0 >> 4, then 0xFFF and 0xFFFF unshifted -- and the six
   bounds are the `slti' constants 5, 0x11, 0x41, 0x101, 0x1001 and the
   fall-through. xsibs found no twin at ratio 0.70 and nothing in the project
   reads rand this way, so this is plain decompilation.

   Two shape facts read off the listing rather than guessed. The n == 0 test
   shares its `addu $v0, $v1, $zero' with the LAST arm's fall-through at
   .Lb25_80171768, so `r = 0' is initialised before the test and the last
   `do' loop has NO `return' of its own -- the five earlier arms do, which is
   what puts the return-value copy in each of their `beqz' delay slots and a
   bare `nop' in the following `j'. The retry test is a signed `slt', so n and
   r are both s32.

   73 words, 0 differing, on the first compile; no alternative spelling was
   measured, so nothing here is claimed to be load-bearing. No jump table and
   no rodata of its own -- the listing opens at `glabel'. */
s32 func_s72_8002458C(s32 n)
{
    s32 r;

    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

/* Integrate the payload's +0x7C..+0x84 velocity into the caller's vector,
   then accelerate it by +0x88..+0x90, shrink the slot's +0x1C/+0x1E scale by
   0xC8 a tick, poke D_8002992E, and on every other tick step the primitive
   through func_800478B8; when the +0x28 fuse runs out, retire the actor and
   set 0x8000 in D_800814A0.  68 words, 0 differing.

   func_s72_800246B0 and func_s72_8002615C are the SAME BODY with one
   constant changed -- 0xC8 against 0x1F4 -- and both land on the identical
   spelling, so every figure below is measured twice.  No twin: dngtwin finds
   no carved sibling at 0.70 for either.  The teardown is
   csrc/ovl_dng_s69.c's landed `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |=
   0x8000;' verbatim.

   | spelling                                              | words | differing |
   | as written below                                       |  68   |     0     |
   | the +0x2A bump as `*(s16 *)(p + 0x2A) = *(u16 *)...'   |  69   |    40     |
   | the two scale decrements written +0x1E then +0x1C      |  68   |     4     |
   | those two as `= *(u16 *)(r + K) - K2' in 1C/1E order   |  68   |     4     |

   THE +0x2A BUMP MUST BE AN ARRAY REF, AND THE WORD IT BUYS IS A DELAY
   SLOT.  The target fills the `lhu 0x2A($s0)' load delay with the `sh' to
   D_8002992E, which means the +0x2A load was HOISTED ABOVE that store.  A
   plain cast keeps the dependence -- true_dependence will not excuse an
   anonymous halfword load against a fixed-address scalar store -- so the
   load stays put, the delay slot becomes a `nop' and the body is one word
   LONG.  As `((u16 *)p)[0x15]' MEM_IN_STRUCT_P is true and the hoist is
   allowed.  Same lever, same direction, as func_s72_80024BA0 above.

   AND THE TWO SCALE DECREMENTS ARE A STORE-ORDER FACT WITH A SPELLING
   PRECONDITION.  Written `*(s16 *)(r + K) = *(u16 *)(r + K) - K2' in source
   order 1C then 1E, the loads come out 1C/1E but sched1 TRANSPOSES the two
   independent stores to 1E/1C -- 4 rows.  As the compound `-=' (or as array
   refs) in the same source order both loads and stores keep source order and
   it is 0.  So the read-modify-write's spelling decides whether the pair is
   transposable at all; source order alone cannot fix it (1E/1C with the long
   form is the same 4 rows). */
void func_s72_800246B0(u8 *p, UnkVec *b, u8 *r)
{
    b->unk_00 += *(s32 *)(p + 0x7C);
    b->unk_04 += *(s32 *)(p + 0x80);
    b->unk_08 += *(s32 *)(p + 0x84);
    *(s32 *)(p + 0x7C) += *(s32 *)(p + 0x88);
    *(s32 *)(p + 0x80) += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x84) += *(s32 *)(p + 0x90);
    *(s16 *)(r + 0x1C) -= 0xC8;
    *(s16 *)(r + 0x1E) -= 0xC8;
    D_8002992E = 1;
    ((s16 *)p)[0x15] = ((u16 *)p)[0x15] + 1;
    if ((((u16 *)p)[0x15] & 1) == 0)
    {
        func_800478B8(r);
    }
    if ((*(s16 *)(p + 0x28) -= 1) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800247C0);

/* Spawn a 0x212 node driven by func_s72_800247C0: seed its payload's +0x28
   and +0x2A with 0x1E, park the SOURCE node at the payload's +0x20 (which is
   what func_s72_800247C0 reads back through `lw 0x20($s5)'), register it with
   func_8004491C against the bank's D_80045340 script, copy the source
   vector's three words into the new node's own UnkVec and raise 0x80 in the
   slot's +0x14.  46 words, 0 differing, FIRST COMPILE.

   DONOR: csrc/ovl_dng_b08.c's landed func_b08_801743BC (dngtwin 0.703) --
   the same func_8003FC64(0x212) / handler-in-unk_10 / func_8004491C /
   D_80045340 opening and the same n->unk_08 vector copy.  The deltas off
   this listing: the payload stores replace the donor's `+ 0x3A' seed, the
   vector copy is three WHOLE WORDS where the donor copies three halfwords,
   and the slot work is only the +0x14 flag.

   LOAD-BEARING, measured: `q = (u8 *)n + 0x20' must be a LOCAL.  With the
   three payload stores written as `(u8 *)n + 0x48/0x4A/0x40' the body is
   45 words / 35 differing -- the target computes `addiu $v0, $s0, 0x20'
   once and offsets 0x28/0x2A/0x20 off it.  The donor's equivalent fact (its
   `m = n->unk_08' local) therefore holds in this unit too. */
void func_s72_80024AE8(UnkNode *src, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x28) = 0x1E;
        *(s16 *)(q + 0x2A) = 0x1E;
        *(UnkNode **)(q + 0x20) = src;
        n->unk_10 = (u32)func_s72_800247C0;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08;
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0x80;
    }
}

/* Walk the whole +0x5C actor ring from D_800814A8 and clear the 0x2000 bit in
   every node's +0x1E flag halfword.  21 words, 0 differing.

   The ring idiom is csrc/ovl_dng_b42.c's func_b42_801748FC (the `g = p - 0x20'
   node base at the top of the loop body) crossed with csrc/ovl_dng_s69.c's
   func_s69_8002501C fact (3) -- the cursor is loaded from D_800814A8 in a
   statement of its OWN and then advanced, so the head value passes through
   `p''s register and the test gets it as a copy.  Both facts re-priced here:

   | spelling                                              | words | differing |
   | as written below                                       |  21   |     0     |
   | `h = (u8 *)D_800814A8; p = *(u8 **)(h + 0x5C) + 0x20'  |  19   |    19     |
   | the store as `*(u16 *)(g + 0x1E) &= 0xDFFF'            |  19   |    20     |
   | no `g', `((u16 *)(p - 0x20))[0xF]' written inline      |  18   |    12     |

   THE ARRAY-REF STORE IS THE WHOLE SECOND WORD PAIR.  The target reads
   D_800814A8 once and keeps the ring head in TWO registers -- `addu $v0, $a0,
   $zero' for the entry test and `addu $a1, $v0, $zero' for the loop test.
   The second copy only exists if loop.c may hoist the loop test's read of the
   global above the `sh' in the body, and a PLAIN CAST store blocks that:
   true_dependence refuses to excuse an anonymous store against a
   fixed-address scalar, so the test re-reads the global inside the loop
   (`lw $v1, 0($a1)') and the body comes out two words short.  Written as an
   array ref MEM_IN_STRUCT_P is true, the hoist is allowed, and cse2 turns the
   hoisted load back into the copy.  This is the MEM_IN_STRUCT_P hoist
   polarity, measured on a 21-word body. */
void func_s72_80024BA0(void)
{
    u8 *p;
    u8 *g;

    p = (u8 *)D_800814A8;
    p = *(u8 **)(p + 0x5C) + 0x20;
    while (p != (u8 *)D_800814A8)
    {
        g = p - 0x20;
        ((u16 *)g)[0xF] &= 0xDFFF;
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* One semi-transparent TEXTURED gouraud quad per slot element: compose the
   slot's rotation with the camera matrix in the 0x1F800000 scratchpad, then
   RotAverage4 the four SVECTORs taken off the payload's +0x4A..+0x61 into a
   0x34-byte POLY_GT4 at the +0x8D0 cursor and -- when the OT index is in
   range and at least one corner is on screen -- give it the payload's four
   colour words, its four UV pairs and two CLUT/TPAGE halfwords, the element's
   +0x1 flag byte, and hang it on the OT.  272 words, 0 differing,
   FIRST COMPILE.

   DONOR: csrc/ovl_dng_s67.c's landed func_s67_8002408C (310 words, dngtwin
   0.735, and the same body is in s46 and s59), itself the POLY_G4 cousin of
   csrc/ovl_dng_s17.c's func_s17_80024188 and csrc/ovl_dng_b14.c's
   func_b14_80173B4C.  The UnkDng74334SP scratchpad typedef and this
   function's prototype were ALREADY in this unit's declaration block, which
   is what made the donor findable: the +0x74 MATRIX's `t[0..2]' at
   0x88/0x8C/0x90, the +0xA4 SVECTOR, the +0xB0 SVECTOR[4], +0xD0/+0xD4 and
   +0x100 all fall out of it with no guessing.

   WHAT IS THIS COPY'S OWN, read off the listing: the primitive is a POLY_GT4
   so the cursor steps 0x34 and the vertex out-pointers are at stride 0xC
   (+0x8/+0x14/+0x20/+0x2C); there are NO DR_MODE blocks at all, so no
   func_80067F20 and only ONE addPrim; the four source vertices are at p +
   0x4A with stride 6; the colours come off p + 0xC/0x10/0x14/0x18; the UVs
   come off p + 0x3A/0x3B, 0x3E/0x3F, 0x44/0x45, 0x46/0x47 with the CLUT and
   TPAGE halfwords at p + 0x3C and p + 0x40; the tag is a real SetPolyGT4
   CALL (func_8006671C, splat/common_syms.txt) and not the macro; and the
   element flag is OR'd into the primitive's own code byte, `q[0x7] |=
   s->unk_0C[3]'.

   ALL FOUR OF THE DONOR'S FACTS HOLD, re-priced here from a body at 0 --
   which the donor did not do, because it took them as given:

   | spelling                                              | words | rows |
   | as written below                                       |  272  |   0  |
   | the scale vector as three separate statements           |  272  |   4  |
   | the four on-screen tests joined with `||' not `|'        |  274  |  18  |
   | `&m' through a `MATRIX *' pointer local                  |  275  | 139  |
   | `e = s->unk_08' AFTER `sp->unk_28 = s->unk_14'            |  272  |   3  |

   TWO MORE THAT ARE THIS BODY'S OWN:

   | `s->unk_14 &= 0x7FFF' after the four colour words         |  273  |  12  |
   | SetPolyGT4 AFTER `f = e[1]; s->unk_0C[3] = f'              |  272  |   9  |

   CONSTANT BASE -- READ THE RAW CHECK, NOT THE MASKED ONE.  Every scratchpad
   access here is a displacement off `(UnkDng74334SP *)0x1F800000', which
   ovlverify's masked mode cannot see.  `--raw' is 272/272 with 13 differing
   and ALL THIRTEEN ARE RELOCATIONS: `lui'/`lw'/`lw' for %hi/%lo(D_80083160)
   at rows 2, 3 and 63, plus the ten `jal' target fields (PushMatrix,
   RotMatrix, ReadRotMatrix, CompMatrix, ScaleMatrix, SetTransMatrix,
   SetRotMatrix, RotAverage4, SetPolyGT4, PopMatrix).  Nothing else differs,
   so every 0x1F8000xx offset and every `ori' low half is confirmed
   bit-for-bit.  relcand: ok, 1 data symbol, 10 calls.

   The six RotAverage4 out-pointers go through `(long *)' casts, which are
   MEASURED FREE at 272 / 0 and which clear six `passing arg N ... from
   incompatible pointer type' warnings declcheck reports.  The donor and its
   relatives (s17, s46, s59, s67, b14) all carry those same six warnings, and
   the same free cast would clear them there. */
void func_s72_80024BF4(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 f;
    u8 *e;

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    ReadRotMatrix(&m);
    CompMatrix(&m, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    e = (u8 *)s->unk_08;
    sp->unk_28 = s->unk_14;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x34;
    sp->unk_B0[0].vx = *(u16 *)(p + 0x4A);
    sp->unk_B0[1].vx = *(u16 *)(p + 0x50);
    sp->unk_B0[2].vx = *(u16 *)(p + 0x56);
    sp->unk_B0[3].vx = *(u16 *)(p + 0x5C);
    sp->unk_B0[0].vy = *(u16 *)(p + 0x4C);
    sp->unk_B0[1].vy = *(u16 *)(p + 0x52);
    sp->unk_B0[2].vy = *(u16 *)(p + 0x58);
    sp->unk_B0[3].vy = *(u16 *)(p + 0x5E);
    sp->unk_B0[0].vz = *(u16 *)(p + 0x4E);
    sp->unk_B0[1].vz = *(u16 *)(p + 0x54);
    sp->unk_B0[2].vz = *(u16 *)(p + 0x5A);
    sp->unk_B0[3].vz = *(u16 *)(p + 0x60);
    sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2], &sp->unk_B0[3],
                              (long *)(q + 0x8), (long *)(q + 0x14), (long *)(q + 0x20),
                              (long *)(q + 0x2C), (long *)&sp->unk_D0,
                              (long *)&sp->unk_D4) -
                  d - 6;
    if ((u32)sp->unk_100 < 0x1E0)
    {
        if (((u16)(*(u16 *)(q + 0x8) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0xA) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x14) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x16) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x20) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x22) + 0x20) < 0x121) |
            ((u16)(*(u16 *)(q + 0x2C) + 0x20) < 0x181 && (u16)(*(u16 *)(q + 0x2E) + 0x20) < 0x121))
        {
            s->unk_14 &= 0x7FFF;
            *(u16 *)(q + 0xE) = *(u16 *)(p + 0x3C);
            *(u16 *)(q + 0x1A) = *(u16 *)(p + 0x40);
            q[0xC] = p[0x3A];
            q[0x18] = p[0x3E];
            q[0x24] = p[0x44];
            q[0x30] = p[0x46];
            q[0xD] = p[0x3B];
            q[0x19] = p[0x3F];
            q[0x25] = p[0x45];
            q[0x31] = p[0x47];
            *(s32 *)(q + 0x4) = *(s32 *)(p + 0xC);
            *(s32 *)(q + 0x10) = *(s32 *)(p + 0x10);
            *(s32 *)(q + 0x1C) = *(s32 *)(p + 0x14);
            *(s32 *)(q + 0x28) = *(s32 *)(p + 0x18);
            SetPolyGT4((POLY_GT4 *)q);
            f = e[1];
            s->unk_0C[3] = f;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] = f | 0x2;
                }
                else
                {
                    s->unk_0C[3] = f & 0xFD;
                }
            }
            q[0x7] |= s->unk_0C[3];
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
        }
    }
    PopMatrix();
}

/* Walk the tag chain from `p` and hand every node to func_s72_80024BF4 with a
   zero fourth argument.  21 words, 0 differing, first compile.

   The twin is func_b06_80173E08 two listings up in THIS unit -- the same
   walk with a different per-node call -- and csrc/ovl_dungeon_800D50FC.c's
   func_800D67B0 is the resident original, identical down to the
   `s = (UnkSlot *)n->unk_0C;` cast.  Nothing was fought for; the loop is a
   `for (;;)` with the NULL test in the middle, which is what puts the
   `addiu $s0, $a2, 0x20` in the `beqz`'s delay slot. */
s32 func_s72_80025034(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s72_80024BF4(p, v, s, 0);
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

/* Another node driver: poke D_8002992E, step the primitive on every other
   tick, kick the slot's +0x16 rotation on state 0 and its +0x1A on state 1,
   accumulate +0x90 into the +0x84 velocity, then add each of the three
   velocities DIVIDED BY 0x10000 into the caller's vector, and retire the
   actor when the slot's +0x14 already carries 0x8000 or when the +0x28 fuse
   runs out.  91 words, 0 differing, FIRST COMPILE.

   No twin at 0.70.  Written off this unit's own landed func_s72_800246B0
   (the +0x2A parity step and the whole teardown) and func_s72_80024398.

   | spelling                                              | words | differing |
   | as written below                                       |  91   |     0     |
   | the +0x2A bump as a plain cast                         |  92   |    86     |
   | the two +0x2C tests joined with `else if'              |  90   |    69     |
   | `>> 16' in place of `/ 0x10000'                        |  89   |    53     |

   THREE FACTS, AND THE TWO CHEAP-LOOKING ONES ARE THE EXPENSIVE ONES.

   (1) THE VELOCITY SCALING IS A SIGNED DIVIDE, NOT A SHIFT.  Each
   `bgez'/`ori 0xFFFF'/`addu'/`sra 16' quartet is cc1psx's bias correction
   for `/ 0x10000' on a signed `s32'; `>> 16' emits the bare `sra' and loses
   it.

   (2) THE TWO +0x2C STATE TESTS ARE TWO SEPARATE `if's AND THE +0x2C LOAD
   HAPPENS TWICE.  The target re-reads `lh 0x2C($s0)' for the second test,
   which an `else if' makes impossible -- the arms are not exclusive in the
   emitted code even though they are in the data.

   (3) The +0x2A bump needs the array ref for the same MEM_IN_STRUCT_P hoist
   as func_s72_800246B0 -- the `sh' to D_8002992E fills its load delay.

   Measured and FREE, so NOT why this lands: the retire gate written as a
   nested `if' with an early `return' instead of the `||' (91 / 0 -- jump2
   rebuilds the shared block either way). */
void func_s72_80025088(u8 *p, UnkVec *b, u8 *r)
{
    D_8002992E = 1;
    ((s16 *)p)[0x15] = ((u16 *)p)[0x15] + 1;
    if ((((u16 *)p)[0x15] & 1) == 0)
    {
        func_800478B8(r);
    }
    if (*(s16 *)(p + 0x2C) == 0)
    {
        *(s16 *)(r + 0x16) += 0x190;
    }
    if (*(s16 *)(p + 0x2C) == 1)
    {
        *(s16 *)(r + 0x1A) += 0x190;
    }
    *(s32 *)(p + 0x84) += *(s32 *)(p + 0x90);
    *(s16 *)((u8 *)b + 0x2) += *(s32 *)(p + 0x7C) / 0x10000;
    *(s16 *)((u8 *)b + 0x6) += *(s32 *)(p + 0x80) / 0x10000;
    *(s16 *)((u8 *)b + 0xA) += *(s32 *)(p + 0x84) / 0x10000;
    *(u16 *)(r + 0x14) &= 0x8000;
    if (*(u16 *)(r + 0x14) != 0 || (*(s16 *)(p + 0x28) -= 1) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s72", func_s72_800251F4);

/* The thrown object's landing test: unless it is already above 0x200, cast
   the ground height under (x, y) from the caller's floor with func_800BCB04
   and, if the object has fallen below the cast height but not below the
   floor, hand its payload to func_s72_800251F4 and fire the 6/0xC and 0x50B
   cues.  50 words, 0 differing, FIRST COMPILE.

   No twin: dngtwin finds nothing at 0.70.  Written off this unit's own
   landed func_s72_80024064 (which names func_800BCB04 as its template) and
   csrc/ovl_dng_b09.c's func_b09_801745BC for the `func_800419EC(6, 0xC)' /
   func_800A56E0 cue pair.

   The parameter widths are read off the prologue and they are the only thing
   this body could get wrong: `x' and `y' are `u16' -- passed to
   func_800BCB04's `s32' as `andi $aN, $vN, 0xFFFF' and to
   func_s72_800251F4's `s16' as `sll'/`sra' -- while `z' and `f' are `s16',
   each sign-extended once.  Nothing here was fought for, so no spelling in
   it is claimed to be load-bearing. */
void func_s72_80025648(UnkNode *n, s16 x, s16 y, s16 z, s16 f)
{
    u8 *q;
    s16 h;

    q = (u8 *)n + 0x20;
    if (z < 0x200)
    {
        h = func_800BCB04((u16)x, (u16)y, f);
        if (h < z && h >= f)
        {
            func_s72_800251F4(q, x, y, z);
            func_800419EC(6, 0xC);
            func_800A56E0(0x50B);
        }
    }
}

/* Walk the +0x5C actor ring from `p' and, for every other actor standing on
   cell (x, y) whose vector height lies in (lo, hi], raise 0x2000 in its
   +0x1E flags and spawn func_s72_80024AE8 against it -- once per actor,
   because the flag is also the guard.  60 words, 0 differing, FIRST COMPILE.

   DONORS: csrc/ovl_dng_b42.c's func_b42_801748FC for the ring (the `g = p -
   0x20' node base at the top of the loop body, the `*(u8 **)(p - 0x14)' cell
   record, the `q[0x24] == x' tests with `u16' x and y) and
   csrc/ovl_dng_b09.c's func_b09_801745BC for the parameter-as-cursor chain.
   dngtwin offered func_b10_80174670 at 0.731; the b42 pair is the closer
   read and it is where the two facts below come from.

   | spelling                                              | words | differing |
   | as written below                                       |  60   |     0     |
   | a separate cursor `q', `p' left holding the head       |  59   |    59     |
   | `*(UnkVec **)(g + 0x8)' inline at both height tests    |  64   |    53     |

   BOTH OF THE DONORS' FACTS HOLD AND BOTH ARE EXPENSIVE HERE.  The
   parameter must carry the cursor (b09's "`p' dies at `h = p' and regmove
   folds it into the parameter's own `addu'"), and the node's UnkVec needs
   its own local read ONCE at the top -- the target's `lw $a1, 0x8($a0)' sits
   above the first `bne', before the cell comparison that can skip it. */
void func_s72_80025710(u8 *p, u16 x, u16 y, s16 hi, s16 lo)
{
    u8 *h;
    u8 *g;
    u8 *c;
    UnkVec *m;

    h = p;
    p = *(u8 **)(p + 0x5C) + 0x20;
    while (p != h)
    {
        g = p - 0x20;
        c = *(u8 **)(p - 0x14);
        m = *(UnkVec **)(g + 0x8);
        if (c[0x24] == x && c[0x25] == y && *(s16 *)((u8 *)m + 0xA) <= hi &&
            *(s16 *)((u8 *)m + 0xA) > lo && (*(u16 *)(g + 0x1E) & 0x2000) == 0)
        {
            *(u16 *)(g + 0x1E) |= 0x2000;
            func_s72_80024AE8((UnkNode *)g, m);
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* PENDING func_s72_80025800 */
/* The 0x12 cluster node's step, three states on the payload's +0x2C while it
   still owns itself, and a three-way follow once it has been given a parent at
   +0x20.  State 0 counts +0x32 up and +0x30 down, eases the slot's three scales
   to 0x100 per tick and re-aims the renderable through func_s72_80026060 until
   +0x32 reaches 0x19, then zeroes both counters and advances; state 1 steps the
   renderable on every other tick and, once D_800814A8's +0x96 timer reaches
   zero, parks 0x200000 in +0x50, clears +0x5C and advances; state 2 integrates
   +0x50 (itself accumulating +0x5C) into the caller's vector height and, for
   each of the nine compass cells of D_s72_80024004 around the cell the vector
   now sits in, calls this unit's func_s72_80025710 and func_s72_80025648 --
   retiring the actor after 0x28 ticks and clearing D_8002992C and D_80028630 as
   it goes.  With a parent, the node either retires with it or copies its
   position and its slot's whole dress across.  314 words, 0 differing.

   No twin: dngtwin and dngcallee find nothing at 0.70.  Written from the
   listing off this unit's own landed func_s72_80025CE8 (which installs this
   handler and whose D_s72_80024028 grid is the same idiom) and
   func_s72_80025088.

   LOUD: THIS BODY CORRECTED func_s72_80025648's PROTOTYPE.  Its second and
   third parameters are `s16', not `u16'.  The callee cannot tell -- its own
   parameters arrive in registers with no entry extension, so `(u16)x' for
   func_800BCB04's `s32' is an `andi' and `x' for func_s72_800251F4's `s16' is
   an `sll'/`sra' either way -- but the CALLER can: this listing sign-extends
   both arguments with `sll'/`sra' where a `u16' parameter would have made it
   zero-extend them with `andi'.  func_s72_80025648 re-measured at 50 words /
   0 differing after the change, and its body now reads
   `func_800BCB04((u16)x, (u16)y, f)' / `func_s72_800251F4(q, x, y, z)'.

   | spelling                                              | words | rows |
   | as written below                                       |  314  |   0  |
   | the two cell divisions into `s32' locals               |  314  |   2  |
   | no `(s32)' cast on the grid element in the first call  |  314  |   4  |
   | the grid element written `g.m[i][j][0] + gx'            |  314  |   4  |
   | the cell address through a `u16 *c = g.m[i][j]' local    |  314  |   5  |
   | func_s72_80025648's parameters left as `u16'            |  312  |  12  |

   THREE FACTS, all measured here.

   (1) THE TWO CELL DIVISIONS MUST LAND IN 16-BIT LOCALS.  `(*(s16 *)((u8 *)b
   + 0x2) - 0x20) / 0x40' is a signed divide, so cc1psx emits the
   `bgez'/`addiu +0x1F' bias and an arithmetic `sra 6' -- but the target has a
   LOGICAL `srl 6'.  Declaring the result `u16' makes the store to it a
   HImode truncation, which lets combine's `force_to_mode' narrow the
   `ashiftrt' to an `lshiftrt' in the same block; with an `s32' local the only
   use is a masked sum two basic blocks away and combine never gets there.
   `s16' and `u16' measure identically (314 / 0 both), so only the WIDTH is a
   fact and the signedness is NOT MEASURABLE.

   (2) THE GRID ELEMENT NEEDS AN EXPLICIT `(s32)' IN THE FIRST CALL, 4 ROWS.
   `(u16)(gx + (s32)g.m[i][j][0])' emits `addu $a1, $s7, $a1' (the division
   result first, as the target has it); without the cast BOTH source orders --
   `gx + g.m[i][j][0]' and `g.m[i][j][0] + gx' -- emit `addu $a1, $a1, $s7'
   with the array element first.  The cast is what stops the u16 member's
   promotion being re-ordered into the first operand position.

   (3) THE 0x24-BYTE GRID IS A `u16 m[3][3][2]' STRUCT AND THE SECOND CALL
   READS ITS TWO HALVES DIFFERENTLY.  `(s16)g.m[i][j][0]' folds into one `lh',
   while `(s16)g.m[i][j][1]' comes out `lhu' plus `sll 16'/`sra 10' because the
   first call already loaded that element zero-extended and cse keeps the one
   load.  36 bytes with alignment 2 is also what gives the target's runtime
   alignment test and its two block-move loops.

   Measured and FREE, so not why this lands: the `u8 *h = (u8 *)D_800814A8'
   local at the top (state 1 re-reads the global inline either way, because the
   payload stores between invalidate cse's entry while the shared `%hi' pseudo
   survives). */
void func_s72_80025800(u8 *p, UnkVec *b, u8 *r)
{
    UnkDngS72Grid g;
    u8 *h;
    u8 *s;
    UnkVec *v;
    s16 i;
    s16 j;
    s16 gx;
    s16 gy;

    g = *(UnkDngS72Grid *)D_s72_80024004;
    D_8002992E = 1;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x14) |= 0xC;
    h = (u8 *)D_800814A8;
    if (*(UnkNode **)(p + 0x20) == NULL)
    {
        switch (*(s16 *)(p + 0x2C))
        {
        case 0:
            *(s16 *)(p + 0x30) -= 1;
            *(s16 *)(p + 0x32) += 1;
            if (*(s16 *)(p + 0x32) < 0x19)
            {
                *(s16 *)(r + 0x1C) = *(s16 *)(r + 0x1E) = *(s16 *)(r + 0x20) =
                    *(s16 *)(p + 0x32) * 0x100;
                *(s16 *)(p + 0x3A) = 0;
                *(s16 *)(p + 0x38) = 1;
                *(s16 *)(p + 0x34) = 1;
                func_s72_80026060(r, (s16 *)(p + 0x34), *(s16 *)(p + 0x3A), *(s16 *)(p + 0x38));
                func_s72_80025FB0((MATRIX *)p, *(u8 **)r);
            }
            else
            {
                *(s16 *)(p + 0x30) = 0;
                *(s16 *)(p + 0x32) = 0;
                *(s16 *)(p + 0x2C) += 1;
            }
            break;
        case 1:
            *(s16 *)(p + 0x32) += 1;
            if ((*(u16 *)(p + 0x32) & 1) != 0)
            {
                *(s16 *)(p + 0x3A) = 1;
                *(s16 *)(p + 0x38) = 0xF;
                *(s16 *)(p + 0x34) = 1;
                func_s72_80026060(r, (s16 *)(p + 0x34), *(s16 *)(p + 0x3A), *(s16 *)(p + 0x38));
                func_s72_80025FB0((MATRIX *)p, *(u8 **)r);
            }
            if (*(s16 *)((u8 *)D_800814A8 + 0x96) == 0)
            {
                *(s16 *)(p + 0x30) = 0;
                *(s32 *)(p + 0x50) = 0x200000;
                *(s32 *)(p + 0x5C) = 0;
                *(s16 *)(p + 0x2C) += 1;
            }
            break;
        case 2:
            b->unk_14 = b->unk_08;
            *(s32 *)(p + 0x50) += *(s32 *)(p + 0x5C);
            b->unk_08 += *(s32 *)(p + 0x50);
            gx = (*(s16 *)((u8 *)b + 0x2) - 0x20) / 0x40;
            gy = (*(s16 *)((u8 *)b + 0x6) - 0x20) / 0x40;
            for (j = 0; j < 3; j++)
            {
                for (i = 0; i < 3; i++)
                {
                    func_s72_80025710(h, (u16)(gx + (s32)g.m[i][j][0]), (u16)(gy + (s32)g.m[i][j][1]),
                                      *(s16 *)((u8 *)b + 0xA),
                                      (s16)(*(u16 *)((u8 *)b + 0xA) - 0x40));
                    func_s72_80025648((UnkNode *)(p - 0x20),
                                      (s16)(*(u16 *)((u8 *)b + 0x2) + (s16)g.m[i][j][0] * 0x40),
                                      (s16)(*(u16 *)((u8 *)b + 0x6) + (s16)g.m[i][j][1] * 0x40),
                                      *(s16 *)((u8 *)b + 0xA), *(s16 *)((u8 *)b + 0x16));
                }
            }
            *(s16 *)(p + 0x30) += 1;
            if (*(s16 *)(p + 0x30) >= 0x28)
            {
                D_8002992C = 0;
                D_80028630 = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            break;
        }
    }
    else if (((*(UnkNode **)(p + 0x20))->unk_1E & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else
    {
        v = (*(UnkNode **)(p + 0x20))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)v + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)v + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)v + 0xA);
        s = (u8 *)(*(UnkNode **)(p + 0x20))->unk_0C;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x10) = 0x20;
        func_s72_8002612C(r, *(s8 *)(s + 0x4));
        func_s72_80025FB0((MATRIX *)p, *(u8 **)r);
        *(s16 *)(r + 0x1C) = *(u16 *)(s + 0x1C);
        *(s16 *)(r + 0x1E) = *(u16 *)(s + 0x1E);
        *(s16 *)(r + 0x20) = *(u16 *)(s + 0x20);
        *(s32 *)(r + 0xC) = *(s32 *)(s + 0xC);
        *(s16 *)(r + 0x16) = *(u16 *)(s + 0x16);
        *(s16 *)(r + 0x18) = *(u16 *)(s + 0x18);
        *(s16 *)(r + 0x1A) = *(u16 *)(s + 0x1A);
    }
}

/* Spawn the 0x12 cluster: twenty-one nodes stepped by this unit's
   func_s72_80025800, each offset from (x, y, z) by 0x80 times the compass row
   that `h' selects out of D_s72_80024028, 0x90 below the caller's height,
   with the slot dressed (0x400 at +0x16, h - 0x400 at +0x1A, 0x1000 scales
   that are then zeroed, 0x808080 colour, 0x20 at +0x10, 0xC raised in +0x14)
   and its sheet taken from D_80028664 sixteen bytes per node.  Node 0 goes
   against the resident D_80083498, the rest against node 0, which they also
   stash at their payload's +0x20 -- while node 0 publishes its own vector in
   D_80028630.  UNWINDS everything it has made if an allocation fails.
   178 words, 0 differing.

   DONOR: csrc/ovl_dng_s55.c's landed func_s55_80026384 (113 words), found by
   grepping `func_8003FD64(0x12,' -- dngtwin reports no carved sibling at
   0.70, because that body is 65 words shorter and the length band hides it.
   Its `(i != 0) ? (void *)arr[0] : (void *)D_80083498' argument, its slot
   dressing order and its unwind-in-the-else all carried verbatim.  The
   direction-table idiom is csrc/ovl_dng_s67.c's func_s67_80024C50
   (`UnkDngS67Dirs' as a 0x20-byte `u8' struct assigned from the bank's data
   symbol).  D_s72_80024028 lives in this unit's `data' SUBSEGMENT, not in
   its .rodata, so it is an extern and nothing here owns a table.

   | spelling                                              | words | differing |
   | as written below                                       | 178   |     0     |
   | `i' as `s32'                                           | 165   |   136     |
   | the unwind as an early return, not the `else' arm      | 176   |   112     |
   | the three vector stores in x, z, y order               | 178   |    47     |
   | a `u8 *d = dd.unk_00' local for the table base         | 179   |   175     |

   THE TABLE BASE MUST BE WRITTEN INLINE IN THE LOOP, which is the donor's
   one fact arriving at the same answer from a different symbol.  With a
   `d' pointer local the body is one word LONG and $fp holds `x' instead of
   `sp + 0x68': `d' is born in the function's first block, loses
   allocno_compare to the three spilled parameters, and gets a stack slot of
   its own at sp+0x98 -- so one more parameter stays in a register and the
   whole frame rotates.  Written inline, loop.c hoists the frame address into
   the PREHEADER, where its shorter live range wins $fp.  The donor's figure
   was two words short and 9 rows; here it is one word long and 175, so the
   DIRECTION transfers and the price does not.

   THE THREE PARAMETERS ARE SPILLED, and that is a fact about the body rather
   than a choice: x, y and z live at sp+0x88/0x90/0x98 and are re-read with
   `lhu' every iteration, which is also what dates them as `u16' (an `s16'
   would be `lh').  `h' is `u16' too -- `srl $v1, $s5, 7' is a LOGICAL shift.

   Measured and FREE, so not why this lands: `(u32)(D_80028664 + i * 0x10)'
   for the index-first form below (178 / 0, so pointer_int_sum's operand
   order is not measurable at this site); `D_800814A0 |= 0x8000' after the
   node's +0x1E store in the unwind; and either row halfword read as `*(s16
   *)' or as `(s16) * (u16 *)' at either offset -- all four combinations are
   178 / 0. */
UnkNode *func_s72_80025CE8(s16 x, s16 y, s16 z, s16 h)
{
    UnkNode *arr[0x15];
    UnkDngS72Dirs dd;
    UnkVec *m;
    u8 *e;
    u8 *r;
    u8 *q;
    s16 i;

    dd = *(UnkDngS72Dirs *)D_s72_80024028;
    for (i = 0; i < 0x15; i++)
    {
        arr[i] = func_8003FD64(0x12, (i != 0) ? (void *)arr[0] : (void *)D_80083498);
        if (arr[i] != NULL)
        {
            arr[i]->unk_10 = (u32)func_s72_80025800;
            e = dd.unk_00 + (((u32)h >> 7) & 0x1C);
            m = arr[i]->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x + *(s16 *)e * 0x80;
            *(s16 *)((u8 *)m + 0x6) = y + (s16) * (u16 *)(e + 0x2) * 0x80;
            *(s16 *)((u8 *)m + 0xA) = z - 0x90;
            r = (u8 *)arr[i]->unk_0C;
            *(s16 *)(r + 0x1A) = h - 0x400;
            *(s16 *)(r + 0x16) = 0x400;
            *(s16 *)(r + 0x20) = 0x1000;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u32 *)(r + 0x8) = (u32)(i * 0x10 + (s32)D_80028664);
            *(u32 *)(r + 0xC) = 0x808080;
            *(s16 *)(r + 0x10) = 0x20;
            *(u16 *)(r + 0x14) |= 0xC;
            func_8004491C(arr[i], (u32)func_800C9034);
            q = (u8 *)arr[i] + 0x20;
            if (i != 0)
            {
                *(UnkNode **)(q + 0x20) = arr[0];
            }
            else
            {
                D_80028630 = (u32)m;
            }
            *(s16 *)(r + 0x1E) = 0;
            *(s16 *)(r + 0x1C) = 0;
            *(s16 *)(r + 0x20) = 0;
            *(s16 *)(q + 0x30) = 0x3C;
            *(s16 *)(q + 0x36) = h;
            *(s16 *)(q + 0x3A) = 0;
            *(s16 *)(q + 0x38) = 0xF;
        }
        else
        {
            for (i = i - 1; i >= 0; i--)
            {
                D_800814A0 |= 0x8000;
                arr[i]->unk_1E |= 0x8000;
            }
            return NULL;
        }
    }
    return arr[0];
}

/* Unpack one 22-byte animation frame record into a MATRIX: the first 18 bytes
   ARE the 3x3 rotation, the three signed bytes at +0x12..+0x14 are the
   translation, and when the +0x15 marker byte is zero the matrix is scaled by
   a zero vector.  44 words, 0 differing, FIRST COMPILE.

   The 22-byte stride is witnessed in this unit by the landed
   func_s72_8002612C and func_s72_80026060 (`+ d * 22' off the sheet's +0xC),
   so the record is 0x16 bytes and this body gives its layout.  The 18-byte
   rotation block is ONE struct assignment of alignment 2, which is what
   mips.c's output_block_move emits as four `lwl'/`lwr' word chunks plus an
   `lh'/`sh' remainder -- align 4 would be plain `lw'/`sw' and align 1 would
   be `lb'/`sb' for the tail, so the `lh' at 0x10 dates the alignment at
   exactly 2 and the members at `s16'.  No twin: dngtwin finds no carved
   sibling at 0.70 and nothing in the project unpacks a frame this way.

   LOAD-BEARING, measured: the zero vector must be the CHAIN
   `v.vx = v.vy = v.vz = 0'.  As three separate statements it is 44 words /
   2 differing -- the chain emits the INNER lvalue first, so the stores come
   out +0x18, +0x14, +0x10, and source order gives the opposite pair. */
void func_s72_80025FB0(MATRIX *d, u8 *f)
{
    VECTOR v;

    *(UnkDng25FB0Rot *)d = *(UnkDng25FB0Rot *)f;
    d->t[0] = *(s8 *)(f + 0x12);
    d->t[1] = *(s8 *)(f + 0x13);
    d->t[2] = *(s8 *)(f + 0x14);
    if (*(s8 *)(f + 0x15) == 0)
    {
        v.vx = v.vy = v.vz = 0;
        ScaleMatrix(d, &v);
    }
}

/* A renderable's facing step: point the slot's +0x00 element at its sheet's
   +0xC plus 22 bytes per frame of the signed +0x04 index, step that index by
   one towards the sign of *b, then clamp it into [lo, hi] -- raising the
   slot's 0x4000 flag when it was clamped and clearing it when it was not.
   The three clamp arms share the one `sh 0x14($t0)' because jump2
   cross-jumps them. No twin anywhere: dngtwin finds nothing above 0.62
   landed, and the resident units have nothing at this shape.
   51 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: the `lo' test must be the FIRST arm. With the
   `hi' test first it is 51 words / 11 differing. Measured and FREE at 51
   words / 0 differing, so neither is why this lands: the 0x4000 flag store
   written before the clamp store in each arm rather than after, and the
   whole body written through `UnkSlot *s' field accesses (unk_00/unk_04/
   unk_08/unk_14) instead of `u8 *s' with casts -- the slot type is
   witnessed here by +0x04 as a signed byte, +0x08 as a pointer and +0x14 as
   the flag halfword, and it measures identically either way. */
void func_s72_80026060(u8 *s, s16 *b, s16 lo, s16 hi)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + *(s8 *)(s + 0x4) * 22;
    if (*b > 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) + 1;
    }
    else if (*b < 0)
    {
        *(s8 *)(s + 0x4) = *(s8 *)(s + 0x4) - 1;
    }
    if (*(s8 *)(s + 0x4) < lo)
    {
        *(s8 *)(s + 0x4) = lo;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else if (hi < *(s8 *)(s + 0x4))
    {
        *(s8 *)(s + 0x4) = hi;
        *(u16 *)(s + 0x14) |= 0x4000;
    }
    else
    {
        *(u16 *)(s + 0x14) &= 0xBFFF;
    }
}

/* Point the slot's +0x00 element at its sheet's +0xC plus 22 bytes per frame
   of `d' -- func_s69_80025840's head without the step or the clamp.
   12 words, 0 differing, FIRST COMPILE.

   LOAD-BEARING, measured: `d' must be an `s16' PARAMETER. As `s32' the
   `sll'/`sra' promotion pair disappears and the body is 10 words /
   12 differing. Measured and FREE at 12 words / 0 differing: `d * 11 * 2'
   in place of `d * 22' -- fold reassociates it to the same
   sll/addu/sll/subu/sll chain. */
void func_s72_8002612C(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
}

/* func_s72_800246B0's body with the scale decrement changed from 0xC8 to
   0x1F4 -- see that function's comment for both measured facts, each of
   which reproduces here to the digit.  68 words, 0 differing. */
void func_s72_8002615C(u8 *p, UnkVec *b, u8 *r)
{
    b->unk_00 += *(s32 *)(p + 0x7C);
    b->unk_04 += *(s32 *)(p + 0x80);
    b->unk_08 += *(s32 *)(p + 0x84);
    *(s32 *)(p + 0x7C) += *(s32 *)(p + 0x88);
    *(s32 *)(p + 0x80) += *(s32 *)(p + 0x8C);
    *(s32 *)(p + 0x84) += *(s32 *)(p + 0x90);
    *(s16 *)(r + 0x1C) -= 0x1F4;
    *(s16 *)(r + 0x1E) -= 0x1F4;
    D_8002992E = 1;
    ((s16 *)p)[0x15] = ((u16 *)p)[0x15] + 1;
    if ((((u16 *)p)[0x15] & 1) == 0)
    {
        func_800478B8(r);
    }
    if ((*(s16 *)(p + 0x28) -= 1) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 spark driven by func_s72_8002615C: publish (x, y, z) plus
   the (dx, dy, dz) offset into the new node's vector, park the un-offset
   (x, y, z) at the payload's +0x2C..+0x30 and the offset NEGATED and scaled
   by 0x200 as the payload's velocity, then dress the slot -- 0x40 in +0x10,
   0xC in +0x14, 0x3000 scale, one of the three colour bytes raised to 0x10
   on a func_s72_8002458C(3) draw, 0x7DCF in +0x12, 0x100 in +0x14 -- and
   register it with func_8003DB94 against D_800DE870.  107 words,
   0 differing.

   TEN PARAMETERS and the first FOUR are never read: `$a0..$a3' are dead in
   the prologue and arguments 5..10 come off the stack at `sp + 0x48'
   upwards.  Their types are read off the one call site in
   func_s72_80026418's listing (`$a0 = $s2 - 0x20', `$a1 = lh 0x24($s2)',
   `$a2 = 0xC0C0C0', `$a3 = 0x28'), and arguments 5..10 are all `s32'
   because the callee loads each with `lw' and the caller `sw's a
   sign-extended halfword into each slot.

   | spelling                                              | words | differing |
   | as written below                                       | 107   |     0     |
   | `m = (u8 *)n->unk_08' hoisted into a local             | 105   |    67     |
   | `r = (u8 *)n->unk_0C' read ONCE instead of twice       | 106   |    61     |
   | the velocity stores before the +0x2C position stores   | 107   |    22     |
   | the three zero colour bytes written 0xC, 0xD, 0xE      | 107   |     2     |

   THE +0x08 VECTOR POINTER MUST BE READ INLINE AT ALL THREE STORES, WHICH
   INVERTS func_s72_80024AE8's FACT FORTY LINES UP.  That body needs
   `m = n->unk_08' in a local (45 words / 35 differing without it); this one
   needs the three `lw 0x8($s2)' reloads the target actually has, because
   each anonymous `sh' invalidates cse's copy and the target does not fight
   it.  Same unit, same field, opposite answers -- "what the rest of this
   unit does" is not evidence.

   AND +0x0C IS READ TWICE, which is the same shape from the other side: the
   target has two `lw 0xC($s2)' separated by the vector and velocity stores,
   so the slot pointer is NOT one long-lived local -- but it must survive
   the func_s72_8002458C call, so it cannot be inline either.

   THE POSITION STORES COME FIRST and that is an ALLOCATION fact, not a
   schedule: with the velocities first, `dx..dz' die last and take $s3/$s4/$s5
   while `x..z' take $s6/$s7/$fp -- the target's assignment exactly reversed,
   22 rows, with every instruction in place.  Writing the positions first
   shortens `x..z''s live ranges, which is what wins them the low registers.

   Measured and FREE, so not why this lands: `-((s16)dx << 9)' for
   `-((s16)dx * 0x200)' (107 / 0 -- fold gives the same `sll 16'/`sra 7'
   pair either way). */
void func_s72_8002626C(u8 *p, s16 i, s32 col, s32 d, s32 dx, s32 dy, s32 dz, s32 x, s32 y,
                       s32 z)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    s16 k;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x28) = 0x14;
        n->unk_10 = (u32)func_s72_8002615C;
        func_8004491C(n, (u32)D_80045340);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x10) = 0x40;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = x + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = y + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = z + dz;
        *(s16 *)(q + 0x2C) = x;
        *(s16 *)(q + 0x2E) = y;
        *(s16 *)(q + 0x30) = z;
        *(s32 *)(q + 0x88) = -((s16)dx * 0x200);
        *(s32 *)(q + 0x8C) = -((s16)dy * 0x200);
        *(s32 *)(q + 0x90) = -((s16)dz * 0x200);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x3000;
        *(s16 *)(r + 0x1C) = 0x3000;
        k = func_s72_8002458C(3);
        *(s8 *)(r + 0xE) = 0;
        *(s8 *)(r + 0xD) = 0;
        *(s8 *)(r + 0xC) = 0;
        if (k == 0)
        {
            *(s8 *)(r + 0xC) = 0x10;
        }
        if (k == 1)
        {
            *(s8 *)(r + 0xD) = 0x10;
        }
        if (k == 2)
        {
            *(s8 *)(r + 0xE) = 0x10;
        }
        *(s16 *)(r + 0x12) = 0x7DCF;
        *(u16 *)(r + 0x14) |= 0x100;
        func_8003DB94(r, D_800DE870, 0);
    }
}

/* PENDING func_s72_80026418 */
/* The cutscene director's seven-state script on the payload's +0xA.  State 0
   opens the scene -- func_800C77D0 over D_80083780, a func_80040490 RECT, the
   func_8003F80C cue, this unit's func_s72_80024BA0 ring clear, D_800814A8's
   +0xF4 cleared and its +0x96 timer set to 0x1E -- and falls THROUGH into
   state 1, which waits for bit 0x80 of the first word's owner and then fires
   func_800A56E0(0x300) and func_800542BC.  State 2 runs the burst: after 0x52
   ticks it drops D_800814A8's +0xA6 counter, publishes the payload's +0x8 byte
   at +0xA8 and takes a compass row out of D_800814A8's +0x2A; on tick 0x4A it
   spawns this unit's func_s72_80025CE8 cluster; and every tick it fires two
   func_s72_8002626C sparks jittered by three rand draws and offset by that row
   out of D_s72_80024028, easing D_80083160's three colour bytes down by 2.
   State 3 falls through into state 4, which eases the colour down and runs a
   0x3C-tick countdown; state 5 eases it back UP by 0xA and waits on D_80082E94
   or the countdown before clearing D_8002992E; state 6 slams the colour to 0x80
   after four ticks, clears D_80083460's +0xC and D_80082E86, drops its +0xA
   counter and retires the actor.  334 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s72_80024048.  jtcheck.py: 7 words = 7 cases + 0
   pad, dispatch bound 7, 7 distinct targets, and the range-check `beqz' goes
   to the function exit, so there is no `default'.  Entries in table order:

       case 0 -> .Ls72_800264FC     case 4 -> .Ls72_800267C8
       case 1 -> .Ls72_8002659C     case 5 -> .Ls72_80026820
       case 2 -> .Ls72_800265D0     case 6 -> .Ls72_800268B4
       case 3 -> .Ls72_800267B8

   The seven entry addresses are strictly ascending, so the arms are written
   ascending 0..6, which is their source order.  ARM ORDER SETTLED BY A
   STANDALONE COMPILE, because neither ovlverify nor aligndiff can see it:
   `tools/cdkgcc -O2 -G0 -S' on this unit emits
   `.word $L209, $L210, $L212, $L221, $L222, $L225, $L231' and those seven
   labels appear in the emitted text in that same order with no trailing pad
   word, so substituting address order for label gives the ROM's sequence
   entry for entry.  Cases 0 and 3 have no `break' -- they fall through into
   cases 1 and 4 -- and that is visible in the listing, not inferred.

   DONOR: csrc/ovl_dng_s69.c's landed func_s69_80024020, the same
   `switch (*(s16 *)(p + 0xA))' director with the same `g = (u8 *)&D_80083160'
   taken once before the switch, the same `arr[2]' RECT idiom, the same
   `**(u16 **)(p + 0x4) & 0x80' gate and the same `h = (u8 *)&D_80083460'
   teardown.  Both of its transferable facts HOLD here:
     - the `+0xA' state bumps are u16 ARRAY REFS, `((u16 *)p)[5] =
       ((u16 *)p)[5] + 1';
     - D_800814A8 is a pointer local PER ARM, assigned BEFORE the store that
       fills its load delay, while the one remaining use in that arm stays
       inline (which is what makes the target load it twice in state 2).

   | spelling                                              | words | rows |
   | as written below                                       |  334  |   0  |
   | `*(s16 *)(p + 0x1C) = 0' as a plain cast in state 5     |  335  |   4  |
   | the state-2 tail written BUMP, p[0x24], p[0x28]         |  334  |   6  |
   | the state-2 tail written BUMP, p[0x28], p[0x24]         |  335  |  23  |
   | `d = dd.unk_00' in the preheader, not in the loop        |  334  |   2  |
   | the `for' init left in the `for' statement              |  334  |   4  |
   | `vp' assigned before the p[0x24] store                  |  334  |   2  |
   | no `d' local, `dd.unk_00 + i * 4' inline                |  333  |  10  |
   | no `d' local, `&dd.unk_00[i * 4]'                       |  330  |  22  |
   | D_80083780 inline in the 0x4A arm (no second local)     |  335  |  25  |
   | a shared `u16 t' for the two post-increment states      |  338  |  50  |
   | `dd' declared before `arr'                              |  338  |  50  |

   THREE FACTS WORTH CARRYING.

   (1) THE SCALED ROW INDEX MUST BE A MULTI-SET PSEUDO, 2 ROWS.  The target's
   row address is `addu $v0, $s4, $v0' -- base first -- and NO spelling of the
   sum reaches that while the shift feeding it is a single-set pseudo:
   `d + i * 4', `i * 4 + (s32)d', `(u8 *)&dd + i * 4', an `s16 *' or an
   `s16 (*)[2]' base and a real ARRAY_REF all emit `addu $v0, $v0, $s4'.  The
   mechanism is combine.c's `gen_binary', which for a commutative code swaps
   the operands when the first is class `o' and the second is not: combine
   substitutes the `sll' into the `addu', builds `(plus (reg) (ashift ...))',
   swaps it, fails to recognise it and splits it back into the two insns IN THE
   SWAPPED ORDER.  Assigning the product to a local inside the address --
   `d + (ix = *(s16 *)(p + 0x24) * 4)', twice, so `ix' has two sets -- makes
   `can_combine_p' refuse the substitution and the order survives.  Probably
   macro residue (an offset macro writing a scratch variable); it is annotated
   at the site.

   (2) STATE 5's `+0x1C' CLEAR MUST BE AN ARRAY REF, ONE WORD.  `((s16 *)p)[0xE]
   = 0' lets reorg move the store into the D_8002992E branch's delay slot; as
   the plain cast `*(s16 *)(p + 0x1C) = 0' true_dependence cannot excuse the
   anonymous store against the fixed-address `lh', the store cannot cross the
   load, and the delay slot becomes a `nop'.  Same MEM_IN_STRUCT_P polarity as
   the donor's fact (1).

   (3) THE LOOP COUNTER'S INIT MUST BE HOISTED OUT OF THE `for', 4 ROWS.
   `k = 0;' written as its own statement ahead of `vp' and the +0x24 store puts
   it first in the preheader's RTL, which is what lets reorg lift it into the
   `bne' delay slot of the `p[0x28] == 0x4A' test the way the target does.
   Left inside `for (k = 0; ...)' it is emitted in the preheader instead.

   Measured and FREE, so not why this lands: `(s32)d' on the base; `s16' or
   `u16' for the two post-increment reads; and either source order for the
   row index sum. */
void func_s72_80026418(u8 *p)
{
    s32 arr[2];
    UnkDngS72Dirs dd;
    u8 *g;
    u8 *h;
    u8 *n;
    u8 *w;
    u8 *vp;
    u8 *d;
    s16 k;
    s32 ix;
    u8 c;

    dd = *(UnkDngS72Dirs *)D_s72_80024028;
    g = (u8 *)&D_80083160;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        func_800C77D0((u8 *)D_800814A8 - 0x20, D_80083780, 8, 0x400);
        *(u8 *)((u8 *)D_800814A8 + 0x102) = 1;
        arr[0] = 0x10003A0;
        arr[1] = 0x400020;
        func_80040490(D_8006E8A0, (RECT *)arr);
        func_8003F80C((u32)D_8006EE50, 0x7AC0, 1, 2);
        func_s72_80024BA0();
        ((s32 *)(u8 *)D_800814A8)[0x3D] = 0;
        *(s16 *)((u8 *)D_800814A8 + 0x96) = 0x1E;
        D_8002992E = 1;
        *(s16 *)(p + 0x2A) = 0;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) != 0)
        {
            func_800A56E0(0x300);
            func_800542BC();
            *(s16 *)(p + 0x28) = 0;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 2:
        if ((s16)(*(u16 *)(p + 0x28))++ >= 0x52)
        {
            n = (u8 *)D_800814A8;
            *(s16 *)(p + 0x1C) = 0x21;
            *(s16 *)(n + 0xA6) -= 1;
            *(u8 *)(n + 0xA8) = p[0x8];
            *(s16 *)(p + 0x24) = (*(u16 *)((u8 *)D_800814A8 + 0x2A) >> 9) & 0x7;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
            *(s16 *)(p + 0x28) = 0;
        }
        if (*(s16 *)(p + 0x28) == 0x4A)
        {
            w = (u8 *)D_80083780;
            func_s72_80025CE8(*(s16 *)(w + 0x2), *(s16 *)(w + 0x6), *(s16 *)(w + 0xA),
                              *(s16 *)((u8 *)D_800814A8 + 0x2A));
        }
        k = 0;
        *(s16 *)(p + 0x24) = (*(u16 *)((u8 *)D_800814A8 + 0x2A) >> 9) & 0x7;
        vp = (u8 *)D_80083780;
        for (; k < 2; k++)
        {
            d = dd.unk_00;
            /* MACRO RESIDUE: the row offset is assigned into `ix' inside each
               address so that the product is a MULTI-SET pseudo.  combine's
               `gen_binary' otherwise swaps `(plus base (ashift idx 2))' and the
               split emits the index first -- 2 aligned rows.  See the header. */
            func_s72_8002626C(p - 0x20, *(s16 *)(p + 0x24), 0xC0C0C0, 0x28,
                              (s16)((rand() & 0x7F) - 0x40), (s16)((rand() & 0x7F) - 0x40),
                              (s16)((rand() & 0x7F) - 0x40),
                              (s16)(*(u16 *)(vp + 0x2) +
                                    *(s16 *)(d + (ix = *(s16 *)(p + 0x24) * 4)) * 0x80),
                              (s16)(*(u16 *)(vp + 0x6) +
                                    (s16)(u32) * (u16 *)(d + (ix = *(s16 *)(p + 0x24) * 4) + 2) * 0x80),
                              (s16)(*(u16 *)(vp + 0xA) - 0x74));
        }
        if (g[0xA8] >= 0x3D)
        {
            g[0xA8] -= 2;
            g[0xA9] -= 2;
            g[0xAA] -= 2;
        }
        break;
    case 3:
        *(s16 *)(p + 0x28) = 0;
        ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
    case 4:
        if (g[0xA8] >= 0x3D)
        {
            g[0xA8] -= 2;
            g[0xA9] -= 2;
            g[0xAA] -= 2;
        }
        if ((*(s16 *)(p + 0x1C) -= 1) <= 0)
        {
            *(s16 *)(p + 0x1C) = 0x3C;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        break;
    case 5:
        c = g[0xA8];
        if (c < 0x80)
        {
            g[0xA8] = c + 0xA;
            g[0xA9] += 0xA;
            g[0xAA] += 0xA;
        }
        if ((D_80082E94 & 0x8000) == 0)
        {
            if ((*(s16 *)(p + 0x1C) -= 1) >= 0)
            {
                break;
            }
        }
        ((s16 *)p)[0xE] = 0;
        if (D_8002992E == 0)
        {
            *(s16 *)(p + 0x28) = 0;
            ((u16 *)p)[5] = ((u16 *)p)[5] + 1;
        }
        else
        {
            D_8002992E = 0;
        }
        break;
    case 6:
        if ((s16)(*(u16 *)(p + 0x28))++ >= 0x4)
        {
            g[0xAA] = 0x80;
            g[0xA9] = 0x80;
            g[0xA8] = 0x80;
            h = (u8 *)&D_80083460;
            *(s32 *)(h + 0xC) = 0;
            D_80082E86 = 0;
            *(s16 *)(h + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
