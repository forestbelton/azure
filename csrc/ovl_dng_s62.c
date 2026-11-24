#include "common.h"

UnkNode *func_8003FC64(s32);
extern u8 D_800269FC[];
extern u8 D_800269D4[];
extern u8 D_800DF31C[];
void func_800B835C(u8 *, s32 *, s32, s32);
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
void func_s62_8002432C(u8 *p, UnkVec *b, u8 *c, u8 *d);

extern u16 D_800269F8;
extern u8 D_80045340[];
extern u8 D_800269EC[];
extern s16 D_80026B28;
extern u8 D_80027CA4[];
extern u8 *D_80027C98;
extern u8 D_800E3D20;
extern u8 D_80026BE4;
s32 func_8004491C(UnkNode *, u32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_800478B8(u8 *);
s32 func_s62_80025F54(s16, s16, s16, s16);
UnkNode *func_s62_80024B2C(s16 x, s16 y, s16 z, s16 a, s16 leaf);
void func_s62_80024AB4(u8 *p, UnkVec *v, UnkSlot *s);
void func_s62_8002654C(u8 *p);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024064);

/* The step func_s62_800244EC parks, in three states on *(s16 *)(p + 0x2C).
   State 0 waits for func_8003DE58 to project the owning node's slot into the
   payload's +0x24 offset triple (or for that slot to retire), then adds the
   triple onto the owner's mover, re-dresses the actor through func_8004491C
   and advances; state 1 fades the renderable's colour word up by 0x202020 a
   frame until its first byte reaches 0xC0, then advances if
   func_s62_80025F54 accepts the mover's position at the D_80027C98
   record's +0x2A bearing; state 2 fades the word down by 0x40404 and retires
   the actor at zero. The owner node is reached at p+0x8, which is where
   func_s62_800244EC stashes `act - 0x20'.
   112 words, 0 differing, THREE compiles.
   TWIN: the landed func_s56_80024388 in csrc/ovl_dng_s56.c (dngtwin 0.778,
   109 words). Of its two stated facts:
     - HOLDS: the owner node load sits ABOVE the +0x2E test, which is what
       lets the already-projected path reach the shared tail on the hoisted
       $v0 while the just-projected path reloads it after the call. Moved
       inside the `== 0' arm it is 114 words / 12 aligned differences.
     - NOT APPLICABLE / INVERTED: it reads the state TWICE at the top, once
       signed and once unsigned, and feeds state 1's bump from the unsigned
       one. Here there is ONE signed `lh' at the top and the bump re-reads
       `lhu' in the block the two arms share. Adding the twin's second top
       read is 114 words / 22 aligned differences.
   THE TWO FACTS THAT COST THE THREE COMPILES, both about the head:
     - The state local must be `s32', not `s16'. As `s16' the load comes out
       `lhu' plus an `sll 16'/`sra 16' pair instead of `lh': 114 words / 7
       aligned differences.
     - The state READ must come BEFORE the counter bump. With
       `D_800269F8++' first the counter store schedules ahead of the `lh',
       reorg can no longer carry it into the `bnez' delay slot and steals the
       `b' parameter copy instead, leaving the load delay a `nop': 113 words
       / 11 aligned differences. This is csrc/ovl_dng_s46.c's
       func_s46_80024C0C fact with the global store on the OTHER side of the
       load. The SPLIT form that unit uses --
       `t = D_800269F8 + 1; st = load; D_800269F8 = t;' -- is also 112 words
       / 0, so the split is FREE and the fact is the ORDER alone.
   Measured and FREE, so neither is why this lands: the `st' local itself
   (read inline at all three gates, cse unifies it and the figure is
   unchanged), and `*(s32 *)(c + 0xC) += 0xFFFBFBFC' in place of
   `-= 0x40404'. LOAD-BEARING: the shared tail's mover load must be the
   re-dereferenced `(*(UnkNode **)(p + 0x8))->unk_08' -- reusing the hoisted
   `o' is 112 words / 25 aligned differences. */
void func_s62_8002432C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *o;
    UnkSlot *r;
    UnkVec *m;
    s32 st;

    st = *(s16 *)(p + 0x2C);
    D_800269F8++;
    if (st == 0)
    {
        o = *(UnkNode **)(p + 0x8);
        r = (UnkSlot *)o->unk_0C;
        if (*(s16 *)(p + 0x2E) == 0)
        {
            if (func_8003DE58((u8 *)r->unk_08, r, (s16 *)(p + 0x24), 0) != NULL)
            {
                *(s16 *)(p + 0x2E) = 1;
            }
            else if ((r->unk_14 & 0x8000) != 0)
            {
                *(s16 *)(p + 0x2E) = 1;
                *(s16 *)(p + 0x28) = -0x40;
            }
            if (*(s16 *)(p + 0x2E) == 0)
            {
                return;
            }
        }
        m = (*(UnkNode **)(p + 0x8))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2) + *(u16 *)(p + 0x24);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6) + *(u16 *)(p + 0x26);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA) + *(u16 *)(p + 0x28);
        func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
        *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
    }
    else if (st == 1)
    {
        if (*(u8 *)(c + 0xC) < 0xC0)
        {
            *(s32 *)(c + 0xC) += 0x202020;
        }
        else if (func_s62_80025F54(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6),
                                   *(s16 *)((u8 *)b + 0xA), *(s16 *)(D_80027C98 + 0x2A)) != 0)
        {
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
        }
    }
    else if (st == 2)
    {
        *(s32 *)(c + 0xC) -= 0x40404;
        if (*(u8 *)(c + 0xC) == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* The 0x202 tint node's spawner: run func_800B835C over {0x1000340,
   0x200020} on D_800DF31C and then over {0x1000360, 0x200020} on
   D_800269D4, park func_s62_8002432C as the new node's step, seed its mover
   from the caller's, point its slot at D_800269FC with a 0xC00 scale, stash
   the owning node at the payload's +0xC and return the payload.
   csrc/ovl_dungeon_800C2824.c's func_800C4AFC is the template: same stack
   `s32 arr[2]', same `func_800B835C(..., arr, 1, 0)', same mover copy. Three
   deltas, all off this listing: func_8003FC64(2) in place of
   func_8003FD64(0x202, D_80083498), TWO func_800B835C calls (the second
   rewrites arr[0] and arr[1], which is the second `sw 0x14($sp)'), and no
   payload pointer -- the node is reached at +0x10 and +0x2C directly and the
   return is `(n != NULL) ? (u8 *)n + 0x20 : NULL'.
   67 words, 0 differing, first compile.
   THIS COPY'S OWN CONSTANT: 0x1000360 -> 0x1200340, 0x2c -> 0x28.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
u8 *func_s62_800244EC(u8 *m, u8 *act)
{
    UnkNode *n;
    UnkVec *v;
    u8 *r;
    s32 arr[2];

    n = func_8003FC64(2);
    if (n != NULL)
    {
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_800DF31C, arr, 1, 0);
        arr[0] = 0x1200340;
        arr[1] = 0x200020;
        func_800B835C(D_800269D4, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s62_8002432C;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_800269FC;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x28) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

/* A 0x202 sibling step with a scale ramp: bump the bank's counter, advance
   the mover by its velocity, add 0xC000 to the +0x14 component, re-send the
   slot through func_800478B8, fade all three colour bytes by one 1/(+0x30)
   step of a single computed value, ease both scale halves 1/(+0x30) of the
   way towards 0x400, and retire the actor when +0x30 runs out.
   69 words, 0 differing, TWO compiles.
   TWINS: the landed func_s56_80025460 (csrc/ovl_dng_s56.c, dngtwin 0.752,
   56 words) for the fade and the retirement, and the landed
   func_s21_800253F8 (csrc/ovl_dng_s21.c, 0.701, 65 words) for the
   func_800478B8 hand-off. Added here over both: the +0x1C/+0x1E scale ramp.
   Of their stated facts:
     - HOLDS (s56): the three colour bytes share ONE computed value.
       Re-reading per channel is 83 words / 16 aligned differences; the
       chained `[0] = [1] = [2] = ...' is 69 words / 4.
     - INVERTED (s21): its "three colour fades are separate statements" is
       the s56 shape's opposite and is the 16-difference row above. The
       discriminator is whether the fade divides (one value, three stores) or
       shifts per channel (three values).
     - FREE (s56): the three-statement retirement. `if (--*(s16 *)(p + 0x30)
       <= 0)' is also 69 words / 0.
   THE FACT THAT COST THE SECOND COMPILE, and it is a WIDTH, not a local: the
   scale ramp must not pass through an `s32'. Written
   `f = s->unk_1C; f += (0x400 - f) / n; s->unk_1C = f; s->unk_1E = f;' with
   `s32 f' the `lhu 0x1C' hoists into the first division's mflo hazard slot,
   above the three colour stores, and it is 68 words / 17 aligned
   differences. The kept memory form and the same body with `u16 f' are both
   69 words / 0, so the target does not choose between them -- only the
   32-bit local is wrong.
   LOAD-BEARING: `v->unk_08 += v->unk_14' must precede
   `v->unk_14 += 0xC000' -- reversed it is 68 words / 29 aligned
   differences. */
void func_s62_800245F8(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 e;
    s16 n;

    D_800269F8++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_14 += 0xC000;
    func_800478B8((u8 *)s);
    e = s->unk_0C[0] - s->unk_0C[0] / *(s16 *)(p + 0x30);
    s->unk_0C[0] = e;
    s->unk_0C[1] = e;
    s->unk_0C[2] = e;
    s->unk_1C += (0x400 - s->unk_1C) / *(s16 *)(p + 0x30);
    s->unk_1E = s->unk_1C;
    n = *(u16 *)(p + 0x30) - 1;
    *(s16 *)(p + 0x30) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_8002470C);

/* A 0x202 sibling step: bump the bank's counter, advance the mover by its
   velocity, add 0x2000 of gravity to the +0x14 component, fade all three of
   the slot's colour bytes by one sixteenth of a single computed value, and
   retire the actor when the payload's +0x30 counter runs out. A leaf -- no
   frame, and `p' lives in $t0 because $a0 is reused as a temp.
   44 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s56_80025460 in csrc/ovl_dng_s56.c (dngtwin 0.800,
   56 words). Mine is 12 words SHORTER and all twelve are MISSING STATEMENTS,
   not missing references: the twin's two rotation bumps (+0x16 += +0x20,
   +0x18 += +0x22) are absent here and its 1/(+0x4C) division is a plain
   `>> 4'. Of its two stated facts:
     - HOLDS: the three colour bytes SHARE ONE COMPUTED VALUE -- one lbu, one
       srl/subu and three sb of the same register, so the fade is an `s32'
       local assigned once and stored three times. Re-reading the byte per
       channel is 49 words / 9 aligned differences; the chained
       `[0] = [1] = [2] = ...' is 44 words / 4, because a QImode store wipes
       cse's memory table and the chain cannot carry the value either.
     - FREE, so it does not constrain this body: it says the retirement is
       the three-statement `n = load - 1; store n; if (n <= 0)' form. Here
       `if (--*(s16 *)(p + 0x30) <= 0)' is also 44 words / 0.
   Also measured and FREE: the shared value declared `u8' instead of `s32'
   (44 / 0). LOAD-BEARING: `v->unk_08 += v->unk_14' must precede
   `v->unk_14 += 0x2000' -- reversed it is 43 words / 8 aligned
   differences. */
void func_s62_80024888(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 b;
    s16 n;

    D_800269F8++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_14 += 0x2000;
    b = s->unk_0C[0] - (s->unk_0C[0] >> 4);
    s->unk_0C[0] = b;
    s->unk_0C[1] = b;
    s->unk_0C[2] = b;
    n = *(u16 *)(p + 0x30) - 1;
    *(s16 *)(p + 0x30) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024938);

/* The +0x10 step of the 0x202 burst node this bank's recursive spawner parks:
   bump the bank's u16 counter, fade the slot's RGB triple at +0xC..+0xE by one
   quarter each, and raise the kill bit on the node flag at -0x2 and on
   D_800814A0 once the red channel drops under 4.
   30 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s21_800248E4 in csrc/ovl_dng_s21.c (dngtwin 0.967,
   30 words). Deltas: this bank's counter, `>> 2' for its `>> 3' and a guard
   of 4 for its 8. Of its three stated facts:
     - HOLDS: the three fades are SEPARATE statements, not a chain. Written
       `[0] = [1] = [2] = [0] - ([0] >> 2)' it is 26 words / 17 aligned
       differences -- the chain has one value where the target has three
       lbu/srl/subu runs.
     - HOLDS: the guard RE-READS the byte (a second `lbu 0xC'). Guarding on
       the computed value through an `s32' local is 29 words / 18 aligned
       differences.
     - FREE, so its reason is weaker than stated: it says `>> 2' and not
       `/ 4' because the bytes are u8 and the shift needs no sign
       correction. Both spellings are 30 words / 0 -- an unsigned divide by
       a power of two is the same `srl', so nothing selects the shift.
   This copy's own measured facts: the counter bump must be the FIRST
   statement (after the three fades it is 33 words / 23 aligned differences),
   and the kill tail is FREE as an `if' or as an early `return' with the
   teardown flat (30 / 0 either way).
   The third parameter is `UnkSlot *', which is what csrc/ovl_dng_s69.c
   already declares for this family's other member; as `u8 *c' with
   `c[0xC]' casts it is also 30 words / 0. */
void func_s62_80024AB4(u8 *p, UnkVec *v, UnkSlot *s)
{
    D_800269F8++;
    s->unk_0C[0] -= s->unk_0C[0] >> 2;
    s->unk_0C[1] -= s->unk_0C[1] >> 2;
    s->unk_0C[2] -= s->unk_0C[2] >> 2;
    if (s->unk_0C[0] < 4)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The recursive burst at this bank's D_80027CA4 aim point. Called with
   leaf != 0 it spawns ONE 0x202 node at (x, y, z) stepped by
   func_s62_80024AB4, with 0x800 scales, D_800269EC at the slot's +0x8, a
   0x808080 tint, 0x60 at +0x10, the slot's 0xC flags and 0x10 at the
   payload's +0x50, and returns it. Called with leaf == 0 it is the DRIVER:
   when D_80026B28 is exactly 3 it walks sixteen steps of one sixteenth of
   the way from (x, y, z) to D_80027CA4, offsetting each step by rcos/rsin of
   the caller's angle times rsin of (i << 7) >> 21, and recurses with
   leaf = 1 at each; then, if D_80026B28 is 0, it publishes (x, y, z) into
   D_80027CA4, and either way bumps D_80026B28.
   160 words, 0 differing, FIRST COMPILE.
   TWIN: the landed func_s69_8002614C in csrc/ovl_dng_s69.c (dngtwin 0.975,
   160 words, same frame -0x58 and the same ten saves). Deltas are this
   bank's relink only: 0x202 for 0x212, this bank's step, aim point and
   counter, 0x800 scales for 0x400 and 0x60 at +0x10 for 0x20.
   Of its three stated facts:
     - HOLDS, and reproduces its prices nearly to the digit: the aim point
       needs TWO SEPARATE `u8 *' LOCALS, one per basic block -- `g' in the
       delta block and `h' in the publish. One local shared by both blocks is
       12 aligned differences (the twin: 12); inline as
       `*(s16 *)(D_80027CA4 + 0x2)' the `+ 2' folds into the symbol address
       and the base is never materialised, 24 (the twin: 22).
     - HOLDS, but its price does NOT transfer: the `leaf' gate must be
       `if (leaf != 0)' with the spawn as the `then' arm. With the driver as
       the `then' arm it is 160 words / 67 aligned differences, where the
       twin measured the same inversion at 1.
     - INVERTED: the twin measured three `sx'/`sy'/`sz' locals copied from
       the parameters and used in the publish as FREE. Here they are 160
       words / 10 aligned differences; the target's three unconditional
       `sh' stores into the frame at 0x18/0x1A/0x1C are the parameters being
       SPILLED round the loop, not declared locals, so the publish must read
       the parameters directly. */
UnkNode *func_s62_80024B2C(s16 x, s16 y, s16 z, s16 a, s16 leaf)
{
    UnkNode *n;
    UnkSlot *r;
    UnkVec *m;
    s16 dx;
    s16 dy;
    s16 dz;
    s16 cx;
    s16 cy;
    s16 cz;
    s32 i;
    u8 *g;
    u8 *h;

    n = NULL;
    if (leaf != 0)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s62_80024AB4;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = x;
            *(s16 *)((u8 *)m + 0x6) = y;
            *(s16 *)((u8 *)m + 0xA) = z;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x800;
            r->unk_1C = 0x800;
            r->unk_08 = (u32)D_800269EC;
            *(u32 *)r->unk_0C = 0x808080;
            r->unk_10 = 0x60;
            r->unk_14 |= 0xC;
            *(s16 *)((u8 *)n + 0x50) = 0x10;
        }
    }
    else
    {
        if (D_80026B28 == 3)
        {
            g = D_80027CA4;
            dx = (*(s16 *)(g + 0x2) - x) / 16;
            dy = (*(s16 *)(g + 0x6) - y) / 16;
            dz = (*(s16 *)(g + 0xA) - z) / 16;
            cx = x + dx;
            cy = y + dy;
            cz = z + dz;
            for (i = 0; i < 0x10; i++)
            {
                func_s62_80024B2C(cx + ((func_80064584(a) * func_800644B8(i << 7)) >> 21),
                                  cy + ((func_800644B8(a) * func_800644B8(i << 7)) >> 21), cz, a,
                                  1);
                cx += dx;
                cy += dy;
                cz += dz;
            }
        }
        if (D_80026B28 == 0)
        {
            h = D_80027CA4;
            *(s16 *)(h + 0x2) = x;
            *(s16 *)(h + 0x6) = y;
            *(s16 *)(h + 0xA) = z;
        }
        D_80026B28++;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80024DAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80025F54);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80026240);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_800262B0);

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
void func_s62_800262F4(u8 *s, s16 *b, s16 lo, s16 hi)
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
void func_s62_800263C0(u8 *s, s16 d)
{
    *(s32 *)s = *(s32 *)(*(u8 **)(s + 0x8) + 0xC) + d * 22;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_800263F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s62", func_s62_80026444);

/* The camera step the installer below parks in D_80083178's +0xB4 slot.
   `p' is the record it fills, D_80083178 + 0xB8. Ease the engine's +0x98
   bearing towards the request's over the frames left at +0x24, run that
   counter down with a -0x80 floor, spend one frame of D_800E3D20, copy the
   mover's (x, z, bearing) into the record's +0x4/+0x6/+0x8 and ease the three
   +0xA4/+0xA6/+0xA8 axes a quarter of the way towards them -- the third only
   while this bank's D_80026BE4 is set, which is then re-armed from whether
   +0xA8 has caught up -- then halve the two +0x94/+0x96 rates.
   82 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_80024024, which is still an INCLUDE_ASM. The
   body came from the DATA SYMBOL: `grep -n D_80083178 csrc/*.c' names
   csrc/ovl_dungeon_800C2824.c's landed func_800C75D0, whose own header
   already says it is the step installed at +0xB4 and that `p' is
   D_80083178 + 0xB8. That is a 128-word body against this 82, a size ratio
   no shape matcher can see past. The 46 missing words are MISSING STATEMENTS:
   the +0x34 pending-request retirement, the `else D_800E3D20 = 0' on the
   first gate, and the func_800BCB04 call with its 0x200 clamp that computes
   the bearing instead of copying +0xA.
   Of func_800C75D0's three stated facts:
     - HOLDS, far more cheaply: every read of +0x0C is RE-DEREFERENCED, never
       cached in a local. A cached `m = *(u8 **)(p + 0xC);' is 82 words / 3
       aligned differences here, where the twin priced the same change at 124
       words against 128 with 109 differing. Without its +0x34 block there
       are only two `lw 0xC' in the target, not four, so three rows is the
       whole fact.
     - HOLDS and reproduces exactly: the two trailing halvings read `lhu' and
       sign-extend at the shift (`sll 16'/`sra 18'), so `*(s16 *)' and
       `(s16)*(u16 *)' are byte-identical -- both 82 words / 0, because the
       `sh' truncates either way.
     - HOLDS: D_80083178 is reached through a `u8 *' LOCAL. Inline as
       `*(s16 *)((u8 *)&D_80083178 + 0x98)' at each of the seven sites it is
       82 words / 38 aligned differences.
   Measured and FREE: the -0x80 clamp read back out of memory rather than off
   an `s16' local (82 / 0 either way), and `D_800E3D20--' for
   `D_800E3D20 -= 1'. */
void func_s62_8002654C(u8 *p)
{
    u8 *g;

    g = (u8 *)&D_80083178;
    if (*(s16 *)(p + 0x24) > 0)
    {
        *(s16 *)(g + 0x98) += (*(s16 *)(p + 0x26) - *(s16 *)(g + 0x98)) / *(s16 *)(p + 0x24);
    }
    *(s16 *)(p + 0x24) = *(u16 *)(p + 0x24) - 1;
    if (*(s16 *)(p + 0x24) < -0x80)
    {
        *(s16 *)(p + 0x24) = -0x80;
    }
    if (D_800E3D20 != 0)
    {
        D_800E3D20 -= 1;
    }
    *(s16 *)(p + 0x4) = *(u16 *)(*(u8 **)(p + 0xC) + 0x2);
    *(s16 *)(p + 0x6) = *(u16 *)(*(u8 **)(p + 0xC) + 0x6);
    *(s16 *)(p + 0x8) = *(u16 *)(*(u8 **)(p + 0xC) + 0xA);
    *(s16 *)(g + 0xA4) += (*(s16 *)(p + 0x4) - *(s16 *)(g + 0xA4)) >> 2;
    *(s16 *)(g + 0xA6) += (*(s16 *)(p + 0x6) - *(s16 *)(g + 0xA6)) >> 2;
    if (D_80026BE4 != 0)
    {
        *(s16 *)(g + 0xA8) += (*(s16 *)(p + 0x8) - *(s16 *)(g + 0xA8)) >> 2;
    }
    D_80026BE4 = *(s16 *)(g + 0xA8) != *(s16 *)(p + 0x8);
    *(s16 *)(g + 0x94) = *(s16 *)(g + 0x94) >> 2;
    *(s16 *)(g + 0x96) = *(s16 *)(g + 0x96) >> 2;
}

/* Install func_s62_8002654C as D_80083178's +0xB4 camera step and fill its
   request record at +0xB8: point the record's own +0x0 at +0x4, park the
   caller's mover at +0xC, clear +0x28 and +0x18, set the mode word at +0x14
   to 0xD, store the frame count at +0x26 and the bearing at +0x24, and
   return a two-bit result -- bit 0 when the mover CHANGED, bit 1 when the
   mode was not already 9 -- publishing the bearing into D_800E3D20 when bit
   0 is set. The first parameter is UNUSED here.
   30 words, 0 differing, FIRST COMPILE.
   NO CARVED SIBLING AND NO RESIDENT TWIN BY EITHER TOOL: dngtwin's only hit
   is the 1.000 clone func_s44_8002416C, still an INCLUDE_ASM. Found the same
   way as the step above -- off D_80083178 -- in the TAIL of
   csrc/ovl_dungeon_800C2824.c's landed func_800C77D0 (52 words), whose first
   30 words are a pair of abs range tests this copy does not have. The other
   deltas: 0xD at +0x14 where the resident writes 9 (the TEST is `!= 9' in
   both), and no `*(u32 *)(p + 0x34) = a' store, which is why the first
   parameter is dead and why the step above has no +0x34 block.
   Of func_800C77D0's facts:
     - HOLDS: TWO locals, `s' for the base and `q = s + 0xB8' for the record.
       Folding `s' away and spelling the three base-relative sites off `q'
       is 29 words / 25 aligned differences.
     - HOLDS as to the THIRD parameter, INVERTED as to the claim that the
       callee cannot see it. It says the third is `s16' and the fourth `s32'
       but that both are stored with `sh' so the callee is 0 differing either
       way. The fourth is indeed FREE here (30 words / 0 as `s16' or `s32'),
       but the third is NOT: as `s32 c' it is 29 words / 6 aligned
       differences, because `D_800E3D20 = c' adds an `sb' use and the
       parameter copy into $t1 goes with it.
   This copy's own measured facts: the +0x24 store must be the LAST statement,
   after the D_800E3D20 gate -- moved ahead of it, 29 words / 2 aligned
   differences -- and the 0xD store must follow the `!= 9' test, 26 words / 28
   the other way. */
s32 func_s62_80026694(void *a, u8 *b, s16 c, s32 d)
{
    u8 *s;
    u8 *q;
    s32 r;

    s = (u8 *)&D_80083178;
    q = s + 0xB8;
    r = (*(u32 *)(q + 0xC) != (u32)b);
    *(u32 *)(q + 0xC) = (u32)b;
    *(u32 *)(q + 0x28) = 0;
    *(u32 *)(s + 0xB8) = (u32)(s + 0xBC);
    if (*(u32 *)(q + 0x14) != 9)
    {
        r |= 2;
    }
    *(u32 *)(q + 0x14) = 0xD;
    *(u32 *)(q + 0x18) = 0;
    *(s16 *)(q + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_s62_8002654C;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(q + 0x24) = c;
    return r;
}
