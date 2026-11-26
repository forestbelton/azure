#include "common.h"

s32 rand(void);

s32 func_800BCE7C(u8 *);
extern u16 D_80027330;
extern u8 D_80045340[];
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_s56_80027204(UnkVec *b);
UnkNode *func_8003FC64(s32);
s32 func_800CEEFC();
extern u8 D_800273B0[];
extern u8 D_80027380[];
extern u8 D_800DF334[];
void func_800B835C(u8 *, s32 *, s32, s32);
void func_s56_80025540(u8 *p, UnkVec *v, UnkSlot *s, s16 c);
extern s16 D_800273BC;
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
void func_s56_80026DE4(u8 *p, UnkVec *b, u8 *c, u8 *d);

typedef struct UnkDng25540SP
{
    u8 pad_00[0x20];
    u8 *unk_20;
    u8 pad_24[0xC];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x34];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u8 pad_F0[0x10];
    SVECTOR unk_100;
} UnkDng25540SP;
void func_s56_80024C3C(u8 *p, UnkVec *v, u8 *c, s16 i, s16 j);
void func_s56_8002616C(s32, u8 *, u8 *, u8 *);
void func_s56_800248BC(u8 *p, UnkVec *v, UnkSlot *s);
s32 func_s56_800269CC(u8 *, UnkVec *, u8 *);
extern u8 D_800273BE;
extern u32 D_800273C0;
extern u8 D_80027398[];
extern s8 D_80027334[][8];
void func_800419EC(u16, u32);
s16 func_800A07D0(s16, s16, s16, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
void func_s56_8002592C(s16 x, s16 y, s16 z, s16 i, s16 j);
void func_s56_80026BA8(s16 i, s16 j, UnkVec *b);
extern u16 D_8002745C[][8];
extern u8 D_80046398[]; /* csrc/unk_19434.c's func_80046398, the engine node walk */
u8 *func_8009B4B0(u8 *, u16, u16);
extern u8 *D_800E3D7C;
extern u8 *D_s56_8002732C;
s32 func_80064584(s32);
s32 func_800644B8(s32);
void func_s56_80025460(u8 *p, UnkVec *v, UnkSlot *s);
extern u16 D_800273CC[];
extern u8 D_8002744C;
extern u8 D_8002744D;

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 *D_800E3D18;
extern u8 D_80082E80[];
extern u8 D_80083780[];
extern s16 D_80083228;
void func_800B8D64(s16, s16, s16);
typedef struct UnkDng24C3CSP
{
    u8 pad_00[0x8];
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    u8 pad_18[0x8];
    s32 unk_20;
    u8 pad_24[0xC];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x14];
    u16 unk_50;
    u8 pad_52[0x6];
    u16 unk_58;
    u8 pad_5A[0x16];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u8 pad_F0[0x10];
    SVECTOR unk_100;
} UnkDng24C3CSP;

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_8002401C);

/* The 0x202 crawler's step, in three states on *(s16 *)(p + 0x48). State 0
   waits for func_8003DE58 to project the owner node's slot into the payload's
   +0x40 offset triple (or for that slot to retire), then adds the triple onto
   the owner's mover, spawns the nine-node burst through func_s56_80027204 and
   advances; state 1 fades the renderable's colour word up by 0x202020 a frame
   until its first byte reaches 0xC0; state 2 fades it down by 0x101010 and
   retires the actor at zero. No xsibs twin at ratio 0.70 and none from the
   callee set -- written from the listing on the block's idioms:
   `func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340)' is
   csrc/ovl_dng_b35.c's and the `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |=
   0x8000' retirement is csrc/ovl_dng_b44.c's.

   THE STATE IS READ TWICE AT THE TOP, ONCE SIGNED AND ONCE UNSIGNED: both an
   `lh' and an `lhu' of +0x48 sit in block 0, ahead of the dispatch, and only
   the unsigned one feeds state 1's bump. The owner node load
   `*(UnkNode **)(p + 0xC)' is likewise ABOVE the +0x4A test, which is what
   lets the already-projected path reach the shared tail on the hoisted $v0
   while the just-projected path reloads it after the call.
   109 words, 0 differing, first compile. */
void func_s56_80024388(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *o;
    UnkSlot *r;
    UnkVec *m;
    s16 st;
    u16 n;

    D_80027330 += 1;
    st = *(s16 *)(p + 0x48);
    n = *(u16 *)(p + 0x48);
    if (st == 0)
    {
        o = *(UnkNode **)(p + 0xC);
        r = (UnkSlot *)o->unk_0C;
        if (*(s16 *)(p + 0x4A) == 0)
        {
            if (func_8003DE58((u8 *)r->unk_08, r, (s16 *)(p + 0x40), 0) != NULL)
            {
                *(s16 *)(p + 0x4A) = 1;
            }
            else if ((r->unk_14 & 0x8000) != 0)
            {
                *(s16 *)(p + 0x4A) = 1;
                *(s16 *)(p + 0x44) = -0x40;
            }
            if (*(s16 *)(p + 0x4A) == 0)
            {
                return;
            }
        }
        m = (*(UnkNode **)(p + 0xC))->unk_08;
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2) + *(u16 *)(p + 0x40);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6) + *(u16 *)(p + 0x42);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA) + *(u16 *)(p + 0x44);
        if (func_s56_80027204(b) != NULL)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        }
    }
    else if (st == 1)
    {
        if (*(u8 *)(c + 0xC) < 0xC0)
        {
            *(s32 *)(c + 0xC) += 0x202020;
        }
        else
        {
            *(s16 *)(p + 0x48) = n + 1;
        }
    }
    else if (st == 2)
    {
        *(s32 *)(c + 0xC) -= 0x101010;
        if (*(u8 *)(c + 0xC) == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* The 0x202 tint node's spawner: run func_800B835C over {0x1000340,
   0x200020} on D_800DF334 and then over {0x1000360, 0x200020} on
   D_80027380, park func_s56_80024388 as the new node's step, seed its mover
   from the caller's, point its slot at D_800273B0 with a 0xC00 scale, stash
   the owning node at the payload's +0xC and return the payload.
   csrc/ovl_dungeon_800C2824.c's func_800C4AFC is the template: same stack
   `s32 arr[2]', same `func_800B835C(..., arr, 1, 0)', same mover copy. Three
   deltas, all off this listing: func_8003FC64(2) in place of
   func_8003FD64(0x202, D_80083498), TWO func_800B835C calls (the second
   rewrites arr[0] and arr[1], which is the second `sw 0x14($sp)'), and no
   payload pointer -- the node is reached at +0x10 and +0x2C directly and the
   return is `(n != NULL) ? (u8 *)n + 0x20 : NULL'.
   67 words, 0 differing, first compile. */
u8 *func_s56_8002453C(u8 *m, u8 *act)
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
        func_800B835C(D_800DF334, arr, 1, 0);
        arr[0] = 0x1000360;
        arr[1] = 0x200020;
        func_800B835C(D_80027380, arr, 1, 0);
        v = n->unk_08;
        n->unk_10 = (u32)func_s56_80024388;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x8) = (u32)D_800273B0;
        *(s16 *)(r + 0x1E) = 0xC00;
        *(s16 *)(r + 0x1C) = 0xC00;
        *(u8 **)((u8 *)n + 0x2C) = act - 0x20;
    }
    return (n != NULL) ? (u8 *)n + 0x20 : NULL;
}

/* The 0x202 flare node's step, two states on *(s16 *)(p + 0x48). State 0 eases
   the payload's +0x3C angle 1/(+0x4C) of the way towards +0x60 + +0x62, then
   publishes it into the mover's +0xA with a +/-2 jitter from rand(), and
   retires the easing after 0x20 more frames by reseeding +0x4C and advancing
   the state; state 1 just integrates the mover's velocity. The +0x4C counter
   is read SIGNED for the `n > 0' guard and the divisor and UNSIGNED for the
   decrement, which is why it appears as both an `lh' and an `lhu'.

   THE STATE READ MUST BE AN ARRAY REF, `((s16 *)p)[0x24]'. As the plain cast
   `*(s16 *)(p + 0x48)' the body is 64 words / 8 aligned differences: the load
   is not MEM_IN_STRUCT_P, `true_dependence' refuses to excuse it against the
   `sh' to the fixed-address scalar D_80027330, so it cannot move above that
   store -- the counter bump completes first, there is nothing left to fill the
   load's delay slot, and `addu $s1, $a1, $zero' gets pushed out of the
   prologue into the branch's. The array ref lets the state load hoist above
   the store and the whole prologue falls into place. Splitting the counter
   bump round the state read (read, load state, store counter) reaches the same
   place and is 0 differing WITH the array ref and 1 without, so the array ref
   is the fact and the split is redundant. Every other p access in the body is
   a plain cast and measures 0.
   63 words, 0 differing. */
void func_s56_80024648(u8 *p, UnkVec *v)
{
    s32 n;
    s16 c;

    D_80027330 += 1;
    if (((s16 *)p)[0x24] == 0)
    {
        n = *(s16 *)(p + 0x4C);
        if (n > 0)
        {
            *(s16 *)(p + 0x3C) =
                *(u16 *)(p + 0x3C) +
                (*(s16 *)(p + 0x62) + *(s16 *)(p + 0x60) - *(s16 *)(p + 0x3C)) / n;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x3C) + (rand() & 3) - 2;
        }
        c = *(u16 *)(p + 0x4C) - 1;
        *(s16 *)(p + 0x4C) = c;
        if (c <= 0)
        {
            *(s16 *)(p + 0x4C) = 0x20;
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        }
    }
    else
    {
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += v->unk_14;
    }
}

/* The 0x202 flare spawner: allocate a node, look up the cell at (x / 0x40,
   y / 0x40) in D_8008333C's grid, publish that cell's first halfword into the
   new slot's +0x8 and -- when it is zero, i.e. the point is off the map --
   retire the node and return NULL; otherwise park func_s56_80024648 as the
   step, D_80046398 (csrc/unk_19434.c's func_80046398, the node walk) as the tag, and seed the
   mover and the payload's +0x38 triple with (x, y, z) plus +0x60/+0x62 from
   the fourth argument and z.

   TWO FACTS WORTH THE NAMES. The grid index divides SIGNED: `x / 0x40' is the
   `bgez'/`addiu 0x3F'/`sra 6' triple and `x >> 6' is not it. And
   `(x / 0x40 + ((y / 0x40) << *(s16 *)(m + 0x14))) * 6 + D_8008333C[0]' is an
   INTEGER sum, which is what puts the index first in every `addu'; `m' is the
   pointer local `(u8 *)D_8008333C' (as in func_s56_80025D30) while
   `D_8008333C[0]' is named directly, which is what shares one `lui' between
   the `%hi' in $s6 and the full address in $s5.
   94 words, 0 differing, FIRST COMPILE -- so the two facts above are read off
   the listing, not priced. */
UnkNode *func_s56_80024744(s16 x, s16 y, s16 z, s16 w)
{
    UnkNode *n;
    UnkVec *v;
    u8 *m = (u8 *)D_8008333C;
    u8 *q;
    u8 *r;
    u16 c;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        r = (u8 *)n->unk_0C;
        c = *(u16 *)((x / 0x40 + ((y / 0x40) << *(s16 *)(m + 0x14))) * 6 +
                     D_8008333C[0]);
        *(u32 *)(r + 0x8) = c;
        if (c == 0)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return NULL;
        }
        n->unk_10 = (u32)func_s56_80024648;
        func_8004491C(n, (u32)D_80046398);
        v = n->unk_08;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)v + 0x2) = x;
        *(s16 *)(q + 0x38) = x;
        *(s16 *)((u8 *)v + 0x6) = y;
        *(s16 *)(q + 0x3A) = y;
        *(s16 *)((u8 *)v + 0xA) = z;
        *(s16 *)(q + 0x3C) = z;
        *(s16 *)(r + 0x20) = 0x1000;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x10) = 0x20;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x14) = 0xC;
        *(s16 *)(q + 0x4C) = 0xC;
        *(s16 *)(q + 0x60) = w;
        *(s16 *)(q + 0x62) = z;
    }
    return n;
}

/* The quake step func_s56_80025334 parks: state 0 eases D_80027398's three
   colour bytes toward (0xF0, 0x40, 0x20) by 1/(+0x4C) a frame, jolts every
   interior cell of the 7x7 height patch D_8002745C by its D_80027334 weight
   minus `rand() & 7', republishes each jolted cell into the actor living
   there (through its -0x18 owner), and spawns a func_s56_8002592C debris
   burst on a 1-in-8 roll per cell; when the +0x4C counter runs out it walks
   the whole 7x7 again calling func_8009CE1C on every actor and
   func_s56_80026BA8 on every cell, reseeds +0x4C to 8 and advances. State 1
   counts +0x4C down and retires the actor.

   TWO MEASURED FACTS, 66 differing without them and 0 with:

   * THE STATE READ MUST BE AN ARRAY REF, `((s16 *)p)[0x24]'. The plain cast
     `*(s16 *)(p + 0x48)' is not MEM_IN_STRUCT_P, so `true_dependence' will
     not excuse it against the `sh' to the fixed-address scalar D_80027330 and
     the load cannot hoist above the counter bump: the `lh' then needs an
     unfillable delay slot and `addu $s7, $a1, $zero' is pushed out of the
     prologue. Identical mechanism, identical cure, to func_s56_80024648's.
   * THE TWO GRID ORIGINS NEED A `h' LOCAL, assigned and then added to:
     `h = (*(u16 *)((u8 *)v + 0x6) >> 6) - 3; iz = h + j;'. In one expression
     fold's `split_tree' moves the -3 off the shift and onto the loop counter,
     giving `addiu $v0, $s3, -0x3' then `addu' where the target has
     `addiu $v0, $v0, -0x3' then `addu $s5, $v0, $s3' -- 6 rows across the two
     loops. Note the SECOND double loop wants the opposite grouping and gets
     it from `+ (i - 3)' written inline, so the two loops disagree and both
     spellings are load-bearing.
   224 words, 0 differing. */
void func_s56_800248BC(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *d;
    u8 *o;
    s16 n;
    s32 i;
    s32 j;
    s32 ix;
    s32 iz;
    s32 r;
    s32 h;

    D_80027330 += 1;
    if (((s16 *)p)[0x24] == 0)
    {
        if ((*(u16 *)(p + 0x4C) & 3) == 0)
        {
            func_800419EC(8, 0x10);
        }
        d = D_80027398;
        d[0x10] += (0xF0 - d[0x10]) / *(s16 *)(p + 0x4C);
        d[0x11] += (0x40 - d[0x11]) / *(s16 *)(p + 0x4C);
        d[0x12] += (0x20 - d[0x12]) / *(s16 *)(p + 0x4C);
        for (j = 1; j < 7; j++)
        {
            h = (*(u16 *)((u8 *)v + 0x6) >> 6) - 3;
            iz = h + j;
            for (i = 1; i < 7; i++)
            {
                h = (*(u16 *)((u8 *)v + 0x2) >> 6) - 3;
                ix = h + i;
                r = rand() & 7;
                D_8002745C[j][i] = D_8002745C[j][i] + (D_80027334[j][i] - r);
                o = func_8009B4B0(D_800E3D7C, (u16)ix, (u16)iz);
                if (o != NULL && o != D_s56_8002732C)
                {
                    *(s16 *)(*(u8 **)(o - 0x18) + 0xA) =
                        D_8002745C[j][i] + *(u16 *)((u8 *)v + 0xA);
                }
                if ((rand() & 7) == 0)
                {
                    func_s56_8002592C(ix * 0x40, iz * 0x40, D_8002745C[j][i], i, j);
                }
            }
        }
        n = *(u16 *)(p + 0x4C) - 1;
        *(s16 *)(p + 0x4C) = n;
        if (n <= 0)
        {
            for (j = 0; j < 7; j++)
            {
                for (i = 0; i < 7; i++)
                {
                    o = func_8009B4B0(D_800E3D7C,
                                      (u16)((*(u16 *)((u8 *)v + 0x2) >> 6) + (i - 3)),
                                      (u16)((*(u16 *)((u8 *)v + 0x6) >> 6) + (j - 3)));
                    if (o != NULL && o != D_s56_8002732C)
                    {
                        func_8009CE1C(o, 0x10, D_800273BE, 9,
                                      func_800A07D0(i, j, 3, 3), D_800273C0, 2);
                        *(u32 *)(o + 0x14) &= 0xFFEFFFFF;
                    }
                    func_s56_80026BA8(i, j, v);
                }
            }
            *(s16 *)(p + 0x4C) = 8;
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        }
    }
    else
    {
        n = *(u16 *)(p + 0x4C) - 1;
        *(s16 *)(p + 0x4C) = n;
        if (n <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* One cell of the quake's 7x7 terrain patch, called 49 times by
   func_s56_80025238. It builds the same composed matrix as
   func_s56_80025540 -- RotMatrix of a ZERO rotation into 0x1F8000D0,
   ReadRotMatrix into 0x1F80011C, CompMatrix, ScaleMatrix by the slot's
   +0x1C/+0x1E doubled with 0x2000 for z -- but seeds the translation from the
   mover plus the cell's (i - 3, j - 3) offset in 0x40 units, and gives the
   quad's four corners their heights out of the height patch D_8002745C at
   [i][j], [i+1][j], [i][j+1] and [i+1][j+1], halved. The corner vx/vy come
   from the slot record's +0x2/+0x3 with +0xA/+0xB as the edge sizes. After
   RotAverage4 into the +0x8D0 cursor's POLY_FT4, if the averaged z is under
   0x1E0 and a corner is inside the 0x181 x 0x121 box it packs the four UVs as
   HALFWORDS -- (v << 8) + u in one `sh' per corner, where
   func_s56_80025540's variant writes two `sb' -- clamps u and v off the
   matrix's m[0][0]/m[1][1] and off the corner x/y ordering, `setlen(t, 9)',
   sets the code byte to 0x2C (0x2E when the slot's +0x14 bit 2 is up),
   `addPrim's it onto OT row z and advances the cursor. `p' is unused: the
   mover arrives in $a1, as in func_s56_80025540.
   383 words, 0 differing.

   *** CONSTANT-BASE BLIND SPOT CHECKED. This body addresses the 0x1F800000
   scratchpad at about thirty displacements, which ovlverify masks. --raw
   reports 15 differences and all 15 are unresolved relocations (D_80083160's
   lui/lw/addiu, D_8002745C's lui/addiu, nine `jal' targets and one `j'), so
   every constant-base displacement matches byte for byte, and relcand is ok
   on 2 data symbols and 9 calls. ***

   The donor is this unit's own landed func_s56_80025540 -- dngcallee puts 16
   functions in one 10-of-11 tier, which is its documented noise band, and the
   in-unit sibling was the one worth reading. Its statement-order facts both
   carried (`sp->unk_38 = 0x2000' last of the three ScaleMatrix components;
   the +0xC word copy before `setlen'), and its `UnkSlot *' parameter did NOT.

   TWELVE FACTS, every figure re-measured from the body at 0 differing:

     the four `/ 2' on the patch heights (as written)           383 /  0
     the same four as `>> 1'                              371 words / 42
     the two grid origins as `* 64' (as written)                383 /  0
     the same two as `<< 6'                                     383 /  0  FREE
     zeroing sp->unk_100 vy, vz, vx (as written)                383 /  0
     zeroing it vx, vy, vz                                      383 /  2
     corner vx/vy shared by reading sp->unk_70[0] back          383 /  0
     the `(s8)q[0x2]'/`(s8)q[0x3]' reads written out twice 389 words / 15
     two `s32' locals holding them instead               379 words / 12
     `sp->unk_70[0].vx = sp->unk_70[2].vx = ...' (as written)   383 /  0
     the two chained stores the other way round                 383 / 10
     the UV spills stored 0x08, 0x10, 0x0C, 0x14 (as written)   383 /  0
     the same four stored 0x08, 0x0C, 0x10, 0x14                383 /  6
     sp->unk_0C's `<< 8' before sp->unk_14's (as written)       383 /  0
     sp->unk_14's `<< 8' first                                  383 / 16
     an `unsigned' w in the two clamp blocks (as written)       383 /  0
     `s32 w'                                                    383 /  2
     `u32 w' in place of `u8 w'                                 383 /  0  FREE
     the clamp's 0x24 store before its 0x14 store (as written)  383 /  0
     the two stores the other way round                         383 /  8
     `if (*(s16 *)(t + 0x8) > *(s16 *)(t + 0x20))' (as written) 383 /  0
     `if (*(s16 *)(t + 0x20) < *(s16 *)(t + 0x8))'              383 /  4
     `g = &D_80083160' declared BEFORE `sp' (as written)        383 /  0
     `sp' declared first                                        383 /  2
     the +0x16 blend as a ternary (as written)                  383 /  0
     the same as an if/else with a store in each arm            383 /  0  FREE
     `c' reached by plain casts (as written)                    383 /  0
     `UnkSlot *s = (UnkSlot *)c' with struct fields        382 words / 18

   FOUR OF THOSE ARE WORTH CARRYING, with the mechanism:

   * `/ 2' IS NOT `>> 1' HERE BECAUSE THE VALUE IS SIGNED, and the price is
     three words a site: `(s16)D_8002745C[i][j] / 2' emits
     `srl $v0, $v0, 31 / addu / sra 1' -- the round-toward-zero bias -- where
     `>> 1' is a bare `sra'. Four sites, 12 words, which is the whole
     371-versus-383 gap. The `* 64' row beside it is the control: a power of
     two is free as a shift when nothing is signed-divided.

   * THE CORNER vx/vy MUST BE SHARED BY READING THE SCRATCHPAD FIELD BACK, not
     by a local and not by writing the read out twice, and the reason is
     `MEM_IN_STRUCT_P' on BOTH sides. `q[0x2]' written twice does not cse:
     the store between the two reads is `sp->unk_70[2].vx', which is at a
     FIXED address but is itself a struct ref, so
     `fixed_scalar_and_varying_struct_p' does not fire, `true_dependence'
     keeps the edge and the second `lbu'/`sll'/`sra' survives -- 6 words, 15
     rows. Reading `sp->unk_70[0].vx' back is a load of the address just
     stored, which cse forwards from the register unconditionally, so it costs
     nothing. The two `s32' locals go the other way and come out FOUR WORDS
     SHORT, because the two `lbu' then hoist together and jump2 joins what the
     target keeps apart.

   * THE CLAMP'S `w' MUST BE UNSIGNED -- convert.c, not allocation. With
     `s32 w' the byte store narrows `w + 0xFF' through `signed char' and fold
     rewrites the constant, giving `addiu $v0, $v1, -0x1' where the target has
     `addiu $v0, $v1, 0xFF'; `u8' and `u32' both measure 0, so the fact is the
     UNSIGNEDNESS and not the width. The two `- 1' clamps forty words later
     really are `addiu -0x1' in the ROM, so the two spellings are both in the
     original and the diagnostic is the immediate itself.

   * THE DECLARATION ORDER OF THE TWO POINTER LOCALS IS WORTH 2 ROWS, and it
     is an insn-birth-order fact, not an allocation one. `g = &D_80083160'
     first makes `lui $t1, %hi(D_80083160)' the function's third instruction,
     ahead of `lui $s1, 0x1F80'; reorg then steals the pair's
     `addiu $s4, $t1, %lo' into PushMatrix's delay slot, which is where the
     target has it. With `sp' first the two `lui' are born the other way
     round. func_s56_80025540 declares them `sp' then `g' and matches, so this
     does not transfer between the two bodies -- it is decided by which
     symbol's base register the first statement needs.

   AND ONE INVERSION TO RECORD LOUDLY: func_s56_80025540 takes the slot as a
   real `UnkSlot *' parameter and reads it as struct fields. Here the same
   seven accesses as `UnkSlot' fields are 382 words / 18 rows -- one word
   short, with the `lhu' of the slot's +0x12 hoisted over the UV spill stores
   that `MEM_IN_STRUCT_P' then stops excusing. So the third parameter's
   declared `u8 *' is CORRECT and must not be tightened to `UnkSlot *'; the
   two bodies in this one unit want opposite answers about the same object,
   40 words apart. */
void func_s56_80024C3C(u8 *p, UnkVec *v, u8 *c, s16 i, s16 j)
{
    Unk80083160 *g = &D_80083160;
    UnkDng24C3CSP *sp = (UnkDng24C3CSP *)0x1F800000;
    u8 *t;
    u8 *q;

    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    sp->unk_E4 = *(s16 *)((u8 *)v + 0x2) + (i - 3) * 64;
    sp->unk_E8 = *(s16 *)((u8 *)v + 0x6) + (j - 3) * 64;
    sp->unk_EC = *(s16 *)((u8 *)v + 0xA);
    sp->unk_70[0].vz = (s16)D_8002745C[i][j] / 2;
    sp->unk_70[1].vz = (s16)D_8002745C[i + 1][j] / 2;
    sp->unk_70[2].vz = (s16)D_8002745C[i][j + 1] / 2;
    sp->unk_70[3].vz = (s16)D_8002745C[i + 1][j + 1] / 2;
    t = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    PushMatrix();
    sp->unk_30 = *(u16 *)(c + 0x1C) << 1;
    sp->unk_34 = *(u16 *)(c + 0x1E) << 1;
    sp->unk_38 = 0x2000;
    sp->unk_100.vy = 0;
    sp->unk_100.vz = 0;
    sp->unk_100.vx = 0;
    RotMatrix((SVECTOR *)0x1F800100, (MATRIX *)0x1F8000D0);
    ReadRotMatrix((MATRIX *)0x1F80011C);
    CompMatrix((MATRIX *)0x1F80011C, (MATRIX *)0x1F8000D0, (MATRIX *)0x1F800050);
    ScaleMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800030);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    q = *(u8 **)(c + 0x8);
    sp->unk_70[0].vx = sp->unk_70[2].vx = (s8)q[0x2];
    sp->unk_70[1].vx = sp->unk_70[3].vx = sp->unk_70[0].vx + q[0xA];
    sp->unk_70[0].vy = sp->unk_70[1].vy = (s8)q[0x3];
    sp->unk_70[2].vy = sp->unk_70[3].vy = sp->unk_70[0].vy + q[0xB];
    sp->unk_C0 = RotAverage4((SVECTOR *)0x1F800070, (SVECTOR *)0x1F800078,
                             (SVECTOR *)0x1F800080, (SVECTOR *)0x1F800088,
                             (long *)(t + 0x8), (long *)(t + 0x10),
                             (long *)(t + 0x18), (long *)(t + 0x20),
                             (long *)0x1F800090, (long *)0x1F800094);
    if ((u32)sp->unk_C0 < 0x1E0)
    {
        if ((((u16)(*(u16 *)(t + 0x8) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0xA) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x10) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x12) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x18) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x1A) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x20) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x22) + 0x20) < 0x121)))
        {
            sp->unk_08 = q[0x8];
            sp->unk_10 = q[0xA];
            sp->unk_0C = q[0x9];
            sp->unk_14 = q[0xB];
            *(s16 *)(t + 0xE) = *(u16 *)(c + 0x12) + *(u16 *)(q + 0x6);
            *(s16 *)(t + 0x16) = (*(u16 *)(c + 0x10) != 0)
                                     ? *(u16 *)(c + 0x10) + (*(u16 *)(q + 0x4) & 0xFF9F)
                                     : *(u16 *)(q + 0x4);
            if (sp->unk_08 + sp->unk_10 >= 0x100)
            {
                sp->unk_10 = sp->unk_10 - 1;
            }
            if (sp->unk_0C + sp->unk_14 >= 0x100)
            {
                sp->unk_14 = sp->unk_14 - 1;
            }
            sp->unk_10 = sp->unk_10 + sp->unk_08;
            sp->unk_14 = sp->unk_14 + sp->unk_0C;
            sp->unk_0C = sp->unk_0C << 8;
            sp->unk_14 = sp->unk_14 << 8;
            *(s16 *)(t + 0xC) = *(u16 *)&sp->unk_0C + *(u16 *)&sp->unk_08;
            *(s16 *)(t + 0x14) = *(u16 *)&sp->unk_0C + *(u16 *)&sp->unk_10;
            *(s16 *)(t + 0x1C) = *(u16 *)&sp->unk_14 + *(u16 *)&sp->unk_08;
            *(s16 *)(t + 0x24) = *(u16 *)&sp->unk_14 + *(u16 *)&sp->unk_10;
            if ((s16)sp->unk_50 >= 0x1800)
            {
                u8 w = t[0x24];
                t[0x24] = w + 0xFF;
                t[0x14] = w;
            }
            if ((s16)sp->unk_58 >= 0x1800)
            {
                u8 w = t[0x25];
                t[0x25] = w + 0xFF;
                t[0x1D] = w;
            }
            if (*(s16 *)(t + 0x8) > *(s16 *)(t + 0x20))
            {
                t[0x14] = t[0x14] - 1;
                t[0x24] = t[0x24] - 1;
            }
            if (*(s16 *)(t + 0xA) > *(s16 *)(t + 0x22))
            {
                t[0x1D] = t[0x1D] - 1;
                t[0x25] = t[0x25] - 1;
            }
            *(u32 *)(t + 0x4) = *(u32 *)(c + 0xC);
            setlen(t, 9);
            t[0x7] = 0x2C;
            if ((*(u16 *)(c + 0x14) & 4) != 0)
            {
                t[0x7] = 0x2E;
            }
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            t = t + 0x28;
        }
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t;
}

/* The 7x7 driver for func_s56_80024C3C: stamp (i * 0x10, j * 0x10 - 0x80)
   into the payload's +0x8/+0x9 and the edge sizes 0xF/0x10 into +0xA/+0xB,
   then call func_s56_80024C3C(p, b, c, i, j) for each of the 49 cells.
   The five constants 6, 0xF, 0x10 and the -0x80 + j * 0x10 giv are loop.c's
   hoists, not source locals, and `c' is spilled to its incoming stack slot
   and reloaded per call because all nine callee-saved registers are spent.
   63 words, 0 differing, FIRST COMPILE.
   MEASURED AND FREE, so NOT the reason this lands: `p[0x8]' array refs versus
   `*(u8 *)(p + 0x8)' plain casts -- both 63 words / 0 differing. */
s32 func_s56_80025238(u8 *p, UnkVec *b, u8 *c)
{
    s32 i;
    s32 j;

    for (j = 0; j < 7; j++)
    {
        for (i = 0; i < 7; i++)
        {
            p[0x8] = i * 0x10;
            p[0x9] = j * 0x10 - 0x80;
            if (i == 6)
            {
                p[0xA] = 0xF;
            }
            else
            {
                p[0xA] = 0x10;
            }
            if (j == 6)
            {
                p[0xB] = 0xF;
            }
            else
            {
                p[0xB] = 0x10;
            }
            func_s56_80024C3C(p, b, c, i, j);
        }
    }
    return 0;
}

/* Clears the 8x8 height patch D_8002745C and spawns the quake actor: a 0x202
   node with func_s56_800248BC as its step and func_s56_80025238 as its tag,
   the mover seeded from (x, y, z), a 0x2000 slot scale, a 0x808080 colour and
   0xF8F82CC0 / 0x13D / 0x10 in the payload.
   THE TWO LOOP COUNTERS ARE `s16' HERE and `s32' in func_s56_80025C8C: the
   `sll 16'/`sra 16' on every counter use is in the target, and it is also
   what stops loop.c strength-reducing `D_8002745C[j][i]' -- the row address is
   rebuilt by `sll/sra/sll 4/addu' every iteration, which is exactly the shape
   the 7x7 fill in func_s56_80025C8C does NOT have.
   75 words, 0 differing, FIRST COMPILE. */
void func_s56_80025334(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    UnkVec *v;
    u8 *q;
    u8 *r;
    s16 i;
    s16 j;

    for (j = 0; j < 8; j++)
    {
        for (i = 0; i < 8; i++)
        {
            D_8002745C[j][i] = 0;
        }
    }
    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s56_800248BC;
        func_8004491C(n, (u32)func_s56_80025238);
        v = n->unk_08;
        *(s16 *)((u8 *)v + 0x2) = x;
        *(s16 *)((u8 *)v + 0x6) = y;
        *(s16 *)((u8 *)v + 0xA) = z;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x2000;
        *(s16 *)(r + 0x1C) = 0x2000;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x10) = 0x20;
        q = (u8 *)n + 0x20;
        *(u32 *)q = 0xF8F82CC0;
        *(s16 *)(q + 0x4) = 0x13D;
        *(u32 *)(r + 0x8) = (u32)q;
        *(s16 *)(q + 0x4C) = 0x10;
    }
}

/* func_s56_800264D4's sibling step without the quad: advance the mover by its
   velocity, add 0x40000 of gravity to the +0x14 component, spin the slot's
   two rotations by the payload's +0x20/+0x22 rates, fade all three colour
   bytes by one 1/(+0x4C) step and retire the actor when the counter runs out.
   A leaf -- no frame, and `p' lives in $t0 because $a0 is reused as a temp.

   THE THREE COLOUR BYTES SHARE ONE COMPUTED VALUE: one `lbu', one `div' and
   three `sb' of the same register, so the fade is an `s32' local assigned
   once and stored three times. Re-reading the byte per channel would cost two
   more lbu/div runs -- a QImode store wipes cse's whole memory table, so
   `[1] = [0]' cannot carry it. The retirement is the block's
   `n = *(u16 *)(p + 0x4C) - 1; *(s16 *)(p + 0x4C) = n; if (n <= 0)'
   three-statement form, which is what gives `sll 16' then `bgtz'.
   56 words, 0 differing, first compile. */
void func_s56_80025460(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 b;
    s16 n;

    D_80027330 += 1;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_14 += 0x40000;
    s->unk_16 += s->unk_20;
    s->unk_18 += s->unk_22;
    b = s->unk_0C[0] - s->unk_0C[0] / *(s16 *)(p + 0x4C);
    s->unk_0C[0] = b;
    s->unk_0C[1] = b;
    s->unk_0C[2] = b;
    n = *(u16 *)(p + 0x4C) - 1;
    *(s16 *)(p + 0x4C) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The visitor func_s56_800258D8 drives over the tag chain: build a composed
   matrix (RotMatrix of the slot's +0x16/+0x18/+0x1A into the scratchpad at
   0x1F8000D0, ReadRotMatrix of the current one, CompMatrix, then ScaleMatrix
   by the slot's +0x1C/+0x1E doubled with 0x2000 for z), RotAverage4 a unit
   0x20 quad into the +0x8D0 cursor's POLY_FT4, and if the averaged z - c is
   under 0x1E0 AND at least one corner is inside the 0x181 x 0x121 screen box,
   fill the UVs from the slot's +0x8 record, `setlen(t, 9)', set the code byte
   to 0x2C (0x2E when the slot's +0x14 bit 2 is up), `addPrim' it onto OT row
   z and advance the cursor.  `p' is unused -- the mover arrives in $a1.

   *** THIS BODY ADDRESSES A CONSTANT BASE (the 0x1F800000 scratchpad) at a
   dozen displacements, which is ovlverify's masked blind spot. Checked with
   --raw: the only raw differences are the twelve unresolved relocations (one
   %hi/%lo pair and nine `jal' targets), so every constant-base displacement
   matches byte for byte, and relcand reports ok on all nine calls. ***

   TWO STATEMENT-ORDER FACTS, both needed, 14 differing without either:
   * `sp->unk_38 = 0x2000' IS THE LAST of the three ScaleMatrix components,
     after unk_30 and unk_34 -- written second (between them, where the
     emitted order puts it) the 0x2000 materialisation and RotMatrix's
     argument copy swap slots: 6 rows.
   * `*(u32 *)(t + 0x4) = *(u32 *)((u8 *)s + 0xC)' COMES BEFORE
     `setlen(t, 9)'. The other way the `lw' of the slot's +0xC lands after the
     tpage store with a `nop' behind it: 8 rows.
   And the two together are 0 while each alone is 8 and 6 -- additive, not a
   conjunction.
   The quad's four corner words are (0, 0x20, 0x200000, 0x200000) -- corners 2
   and 3 really are equal in the ROM, which is not a transcription slip.
   230 words, 0 differing. */
void func_s56_80025540(u8 *p, UnkVec *v, UnkSlot *s, s16 c)
{
    UnkDng25540SP *sp = (UnkDng25540SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    u8 *t;
    u8 *q;
    MATRIX m;

    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    sp->unk_E4 = *(s16 *)((u8 *)v + 0x2);
    sp->unk_E8 = *(s16 *)((u8 *)v + 0x6);
    sp->unk_EC = *(s16 *)((u8 *)v + 0xA);
    t = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    q = (u8 *)s->unk_08;
    PushMatrix();
    sp->unk_30 = s->unk_1C << 1;
    sp->unk_34 = s->unk_1E << 1;
    sp->unk_38 = 0x2000;
    sp->unk_100.vx = s->unk_16;
    sp->unk_100.vz = s->unk_1A;
    sp->unk_100.vy = s->unk_18;
    RotMatrix((SVECTOR *)0x1F800100, (MATRIX *)0x1F8000D0);
    ReadRotMatrix(&m);
    CompMatrix(&m, (MATRIX *)0x1F8000D0, (MATRIX *)0x1F800050);
    ScaleMatrix((MATRIX *)0x1F800050, (VECTOR *)0x1F800030);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    *(u32 *)&sp->unk_70[1] = 0x20;
    *(u32 *)&sp->unk_70[3] = 0x200000;
    *(u32 *)&sp->unk_70[2] = 0x200000;
    *(u32 *)&sp->unk_70[0] = 0;
    sp->unk_70[3].vz = 0;
    sp->unk_70[2].vz = 0;
    sp->unk_70[1].vz = 0;
    sp->unk_70[0].vz = 0;
    sp->unk_C0 = RotAverage4((SVECTOR *)0x1F800070, (SVECTOR *)0x1F800078,
                             (SVECTOR *)0x1F800080, (SVECTOR *)0x1F800088,
                             (long *)(t + 0x8), (long *)(t + 0x10),
                             (long *)(t + 0x18), (long *)(t + 0x20),
                             (long *)0x1F800090, (long *)0x1F800094) -
                 c;
    if ((u32)sp->unk_C0 < 0x1E0)
    {
        if ((((u16)(*(u16 *)(t + 0x8) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0xA) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x10) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x12) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x18) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x1A) + 0x20) < 0x121)) |
            (((u16)(*(u16 *)(t + 0x20) + 0x20) < 0x181) &&
             ((u16)(*(u16 *)(t + 0x22) + 0x20) < 0x121)))
        {
            t[0xC] = t[0x1C] = q[0x8];
            t[0x14] = t[0x24] = q[0x8] + q[0xA];
            t[0xD] = t[0x15] = q[0x9];
            t[0x1D] = t[0x25] = q[0x9] + q[0xB];
            *(s16 *)(t + 0x16) = *(u16 *)(q + 0x4);
            *(u32 *)(t + 0x4) = *(u32 *)((u8 *)s + 0xC);
            setlen(t, 9);
            t[0x7] = 0x2C;
            if ((s->unk_14 & 4) != 0)
            {
                t[0x7] = 0x2E;
            }
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            t = t + 0x28;
        }
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t;
}

/* The engine's node walk, func_80046398 of csrc/unk_19434.c, relinked here
   with func_s56_80025540 as the visitor: it is variant A -- the slot's
   +0x06 as the visitor's fourth argument -- exactly as
   csrc/ovl_dungeon_800B6AC4.c's func_800BC054 has it, and xsibs scores it
   1.000 against slus_006.14's func_80046398. Nothing was changed.
   21 words, 0 differing, first compile. */
s32 func_s56_800258D8(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s56_80025540(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* The debris burst func_s56_800248BC fires per jolted cell: a 0x202 node with
   func_s56_80025460 as its step and func_s56_800258D8 as its tag, the mover
   seeded from (x, y, z) with a +/-8 jitter on the two horizontal velocity
   components and -0x10 - (rand() & 0xF) on the vertical, a 0x400 slot scale
   rotated by rsin/rcos of one rand(), and (i, j) stamped into the payload's
   +0x8/+0x9 with the 0xF/0x10 edge flags at +0xA/+0xB.

   THE ONE FACT: `*(u32 *)(r + 0xC) = 0x808080' MUST BE THE FIRST of the three
   slot stores, before +0x10 and +0x14. Written last (the natural order of the
   emitted `sh's) the two large constants 0x808080 and 0xF8F82CC0 exchange
   registers -- local-alloc gives the lower one to whichever dies first, and
   moving the 0x808080 store earlier is what makes it that one: 8 differing,
   every row in the two `lui'/`ori' pairs and the two `sw's they feed. Moving
   the 0xF8F82CC0 store after the 0x13D store instead reaches 3.
   107 words, 0 differing. */
void func_s56_8002592C(s16 x, s16 y, s16 z, s16 i, s16 j)
{
    UnkNode *n;
    UnkVec *v;
    u8 *q;
    u8 *r;
    s32 t;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s56_80025460;
        func_8004491C(n, (u32)func_s56_800258D8);
        v = n->unk_08;
        *(s16 *)((u8 *)v + 0x2) = x;
        *(s16 *)((u8 *)v + 0x6) = y;
        *(s16 *)((u8 *)v + 0xA) = z;
        *(s16 *)((u8 *)v + 0xE) = (rand() & 0xF) - 8;
        *(s16 *)((u8 *)v + 0x12) = (rand() & 0xF) - 8;
        *(s16 *)((u8 *)v + 0x16) = -0x10 - (rand() & 0xF);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x400;
        *(s16 *)(r + 0x1C) = 0x400;
        t = rand();
        *(s16 *)(r + 0x20) = func_80064584(t);
        *(s16 *)(r + 0x22) = func_800644B8(t);
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x10) = 0x20;
        *(s16 *)(r + 0x14) = 0xC;
        q = (u8 *)n + 0x20;
        *(u32 *)q = 0xF8F82CC0;
        *(s16 *)(q + 0x4) = 0x13D;
        q[0x8] = i * 0x10;
        q[0x9] = j * 0x10 - 0x80;
        if (i == 7)
        {
            q[0xA] = 0xF;
        }
        else
        {
            q[0xA] = 0x10;
        }
        if (j == 7)
        {
            q[0xB] = 0xF;
        }
        else
        {
            q[0xB] = 0x10;
        }
        *(s16 *)(q + 0x4C) = (rand() & 0x1F) + 0x20;
        *(u32 *)(r + 0x8) = (u32)q;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025AD8);

/* Stamps the 7x7 halfword patch at D_800273CC into the dungeon's cell grid,
   centred on (D_8002744C, D_8002744D): the destination index is
   `x + (z << D_80083160.unk_1F0)' scaled by the 6-byte cell record and taken
   off the row base D_80083160.unk_1DC, and the +0x2 field of each record is
   what gets written. The grid base and the shift are re-read every iteration
   because the `sh' invalidates cse's load of them -- the same reason
   func_s56_80025D30 writes its row address out at each of its three uses.

   FOUR MEASURED FACTS, all of them structural rather than allocation:
     * `m' is TWO STATEMENTS. `(u8 *)&D_80083160 + 0x1DC' in one expression
       folds the 476 into the symbol's %lo and the address costs two words,
       not three: 49 words / 48 differing. Taking the struct's address into
       `g' first forces the separate `addiu $t4, $v0, 0x1DC'.
     * the two loop counters are `s32'. As `s16' every `i++'/`j++' carries an
       `sll 16'/`sra 16' pair and, worse, the sign extension blocks loop.c's
       strength reduction of the row index, so the cursors are recomputed by
       `addu' per iteration: 49 words / 48.
     * `row' must be initialised in the OUTER for's comma init. As a plain
       statement before the loop it is born one insn earlier, loses
       `allocno_compare' to `j' and the two exchange $t2/$t3; with the inner
       cursor spelled the same way that is 17 aligned differences.
     * `x++' belongs in the INNER for's increment, AFTER `i++'. In the body it
       is 5 differing, and as `x++, i++' it is also 5.
   And the row is read as `row[i]', NOT through a `*src++' cursor: a source
   cursor's `src = row' is a for-init, which gcc emits BEFORE loop.c's
   invariant hoist of `(s16)z', where the giv's own init is emitted AFTER it.
   Everything else matches either way, so that is exactly 3 differing.
   41 words, 0 differing. relcand clean on all four data symbols. */
void func_s56_80025C8C(void)
{
    u8 *g = (u8 *)&D_80083160;
    u8 *m;
    u16 *row;
    s16 x0;
    s16 z;
    s16 x;
    s32 i;
    s32 j;

    m = g + 0x1DC;
    z = D_8002744D - 3;
    x0 = D_8002744C - 3;
    for (j = 0, row = D_800273CC; j < 7; j++)
    {
        x = x0;
        for (i = 0; i < 7; i++, x++)
        {
            *(u16 *)((x + (z << *(s16 *)(m + 0x14))) * 6 + *(u32 *)m + 2) = row[i];
        }
        row += 8;
        z++;
    }
}

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
u8 *func_s56_80025D30(s32 x, s32 z, s16 c)
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

/* func_s56_8002616C's wrapper: emit the draw-area primitive for
   D_80083160's 0x8D0 ring, then call func_s56_8002616C once for `b' (when it
   is non-NULL) and once for NULL, each with a -0xE0 y offset when the frame
   buffer is the far one, and finally a second draw-area primitive for `p'.
   csrc/ovl_dungeon_800B6AC4.c's func_800B8FC8 is the line-for-line template
   and every one of its facts carried: `s16 lean' (not `s32') for the
   D_801C9E40 comparison, `g = &D_80083160' as a pointer local used only by
   the LAST pair while the first pair names `D_80083160.unk_00' directly, and
   `((u32 *)D_80083160.unk_00)[0x234]' for the cursor bump.
   66 words, 0 differing, FIRST COMPILE -- the facts above are inherited from
   func_800B8FC8 and were not re-priced here. */
void func_s56_80026064(u8 *p, u8 *b)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    s16 lean;
    s32 t;

    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    ((u32 *)D_80083160.unk_00)[0x234] = (u32)(q + 0xC);
    SetDrawArea((DR_AREA *)q, (RECT *)D_80083160.unk_00);
    AddPrim(r, q);
    if (b != NULL)
    {
        t = 0;
        if (lean)
        {
            t = -0xE0;
        }
        func_s56_8002616C(t, p, b, r);
    }
    t = 0;
    if (lean)
    {
        t = -0xE0;
    }
    func_s56_8002616C(t, p, NULL, r);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea((DR_AREA *)q, (RECT *)p);
    AddPrim(r, q);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_8002616C);

/* The 0x202 debris node's step: rebase the four 8-byte corner records at the
   payload's +0x10..+0x2F on the mover's own origin into the scratchpad quad
   at 0x1F800070, build a rotation-only matrix from the slot's +0x16/+0x18 on
   the stack INTO THE SCRATCHPAD MATRIX AT 0x1F800050, push it, run the four
   corners through RotTransSV, pop, integrate the mover's velocity, write the
   rotated corners back biased by the mover, and then fade the slot's three
   colour bytes -- toward 0xC0 while the payload's +0x48 is 0, toward 0 after
   -- one 1/(+0x4C) step each, reseeding the mover's velocity and the slot's
   two rotations from rand() when the first phase's counter runs out and
   retiring the actor when the second's does.

   csrc/ovl_dng_s20.c's func_s20_80025A58 is the family template and its
   measured facts carry over unchanged: a scratchpad STORE is `sp->unk_NN'
   through a struct pointer at 0x1F800000 while a scratchpad ADDRESS PASSED
   AS AN ARGUMENT is the integer constant off the same hoisted `lui'; the
   matrix's translation is zeroed t[2], t[1], t[0] DESCENDING; the colour fade
   re-reads `*(s16 *)(p + 0x4C)' at every site so the lbu/lh/div/mflo runs
   hoist into one block; and THE COUNTER BUMP IS SPLIT AROUND THE THIRD vz
   CORNER -- read D_80027330 before the corner, compute the corner into a
   local, store the counter between them, store the corner last. That entry
   prices the alternatives at 7 to 51 differences on the s20 body.

   Two things are this copy's own. The MATRIX is the scratchpad's at +0x50,
   not a stack local, so its t[] is `sp->unk_6C/68/64'; and D_800273BC is read
   BOTH SIGNED AND UNSIGNED in the retire path (`lh' for the `!= 0' gate,
   `lhu' for the decrement), which `(u16)D_800273BC - 1' is the spelling for.
   318 words, 0 differing, first compile. */
void func_s56_800264D4(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng264D4SP *sp = (UnkDng264D4SP *)0x1F800000;
    SVECTOR rot;
    s32 t;
    s16 n;
    s16 n2;
    u16 c;

    sp->unk_70[0].vx = *(u16 *)(p + 0x10) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[1].vx = *(u16 *)(p + 0x18) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[2].vx = *(u16 *)(p + 0x20) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[3].vx = *(u16 *)(p + 0x28) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[0].vy = *(u16 *)(p + 0x12) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[1].vy = *(u16 *)(p + 0x1A) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[2].vy = *(u16 *)(p + 0x22) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[3].vy = *(u16 *)(p + 0x2A) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[0].vz = *(u16 *)(p + 0x14) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[1].vz = *(u16 *)(p + 0x1C) - *(u16 *)((u8 *)v + 0xA);
    c = D_80027330;
    t = *(u16 *)(p + 0x24) - *(u16 *)((u8 *)v + 0xA);
    D_80027330 = c + 1;
    sp->unk_70[2].vz = t;
    sp->unk_70[3].vz = *(u16 *)(p + 0x2C) - *(u16 *)((u8 *)v + 0xA);
    PushMatrix();
    rot.vx = s->unk_16;
    rot.vy = s->unk_18;
    rot.vz = 0;
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_38 = 0x1000;
    sp->unk_6C = 0;
    sp->unk_68 = 0;
    sp->unk_64 = 0;
    RotMatrix(&rot, (MATRIX *)0x1F800050);
    SetTransMatrix((MATRIX *)0x1F800050);
    SetRotMatrix((MATRIX *)0x1F800050);
    RotTransSV((SVECTOR *)0x1F800070, (s32 *)0x1F800098, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800078, (s32 *)0x1F8000A0, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800080, (s32 *)0x1F8000A8, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800088, (s32 *)0x1F8000B0, (s32 *)0x1F800094);
    PopMatrix();
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    *(s16 *)(p + 0x10) = sp->unk_98[0].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x18) = sp->unk_98[1].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x20) = sp->unk_98[2].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x28) = sp->unk_98[3].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x12) = sp->unk_98[0].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x1A) = sp->unk_98[1].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x22) = sp->unk_98[2].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x2A) = sp->unk_98[3].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x14) = sp->unk_98[0].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x1C) = sp->unk_98[1].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x24) = sp->unk_98[2].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x2C) = sp->unk_98[3].unk_04 + *(u16 *)((u8 *)v + 0xA);
    if (*(s16 *)(p + 0x48) == 0)
    {
        s->unk_0C[0] += (0xC0 - s->unk_0C[0]) / *(s16 *)(p + 0x4C);
        s->unk_0C[1] += (0xC0 - s->unk_0C[1]) / *(s16 *)(p + 0x4C);
        s->unk_0C[2] += (0xC0 - s->unk_0C[2]) / *(s16 *)(p + 0x4C);
        n = *(u16 *)(p + 0x4C) - 1;
        *(s16 *)(p + 0x4C) = n;
        if (n <= 0)
        {
            v->unk_0C = ((rand() & 0x1F) - 0x10) << 16;
            v->unk_10 = ((rand() & 0x1F) - 0x10) << 16;
            v->unk_14 = ((rand() & 0x3F) - 0x20) << 16;
            s->unk_16 = (rand() & 0x3FF) - 0x200;
            s->unk_18 = (rand() & 0x3FF) - 0x200;
            *(s16 *)(p + 0x4C) = 0x10;
            *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        }
    }
    else
    {
        s->unk_0C[0] -= s->unk_0C[0] / *(s16 *)(p + 0x4C);
        s->unk_0C[1] -= s->unk_0C[1] / *(s16 *)(p + 0x4C);
        s->unk_0C[2] -= s->unk_0C[2] / *(s16 *)(p + 0x4C);
        n = *(u16 *)(p + 0x4C) - 1;
        *(s16 *)(p + 0x4C) = n;
        if (n <= 0)
        {
            if (D_800273BC != 0)
            {
                n2 = (u16)D_800273BC - 1;
                D_800273BC = n2;
                if (n2 == 1)
                {
                    D_800273BC = 0;
                }
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_800269CC);

/* The debris spawner func_s56_800248BC calls per cell on the second pass: a
   0x202 node with func_s56_800264D4 as its step and func_s56_800269CC as its
   tag; the mover goes to the cell centre with its height the `>> 2' average
   of the four corner entries of D_8002745C, and the payload's four 8-byte
   corner records get the cell's four corners biased by -0x20 and each
   corner's own D_8002745C height.

   THREE MEASURED FACTS:
   * `*(u32 *)(r + 0x8) = (u32)q' MUST BE AN ARRAY REF, `((u32 *)r)[0x2]'.
     As a plain cast the store cannot sink past the D_800273BC read and comes
     out one slot early: 2 aligned differences. The array ref is
     MEM_IN_STRUCT_P, `true_dependence' excuses it against the fixed-address
     scalar, and reorg gets the order the target has.
   * THE D_800273BC BUMP IS SPLIT around the other two tail stores --
     `cc = (u16)D_800273BC; <two stores>; D_800273BC = cc + 1;'. Written as
     `D_800273BC += 1' anywhere in that group the body is 143 words with 5-8
     rows; the split is what puts the `lhu' first and the `sh' last. And `cc'
     must be read AFTER the +0x8 store, not before: before, the address
     register and the constant 8 exchange $v0/$a0 (5 rows).
   * THE THREE MOVER COMPONENTS GO THROUGH `s32' LOCALS and are stored
     0x2, 0x6, 0xA after all three are computed. Inline, convert.c narrows
     each sum through unsigned short and the `lh' of the mover's +0x2/+0x6
     becomes `lhu' with the final `addu' operands flipped: 89 rows. Storing
     them in the emitted order 0xA, 0x2, 0x6 instead of source order is 16.
   The four x/y corner pairs are CHAINED with the HIGHER offset as the inner
   lvalue (`*(s16 *)(q + 0x10) = *(s16 *)(q + 0x20) = ...'), which is what
   emits +0x20 first; the mover reads +0x2/+0x6 SIGNED and the corner reads of
   the same halfwords UNSIGNED, and both appear in the target.
   143 words, 0 differing. */
void func_s56_80026BA8(s16 i, s16 j, UnkVec *b)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    s16 jj;
    u16 cc;
    s32 tx;
    s32 ty;
    s32 tz;

    jj = j;
    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s56_800264D4;
        func_8004491C(n, (u32)func_s56_800269CC);
        m = n->unk_08;
        tx = *(s16 *)((u8 *)b + 0x2) + (i - 3) * 0x40;
        ty = *(s16 *)((u8 *)b + 0x6) + (j - 3) * 0x40;
        tz = (((s16)D_8002745C[j][i] + (s16)D_8002745C[j][i + 1] +
               (s16)D_8002745C[j + 1][i] + (s16)D_8002745C[j + 1][i + 1]) >>
              2) +
             *(s16 *)((u8 *)b + 0xA);
        *(s16 *)((u8 *)m + 0x2) = tx;
        *(s16 *)((u8 *)m + 0x6) = ty;
        *(s16 *)((u8 *)m + 0xA) = tz;
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1E) = 0x800;
        *(s16 *)(r + 0x1C) = 0x800;
        *(s16 *)(r + 0x14) = 0xC;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x10) = *(s16 *)(q + 0x20) =
            *(u16 *)((u8 *)b + 0x2) + (i - 3) * 0x40 - 0x20;
        *(s16 *)(q + 0x18) = *(s16 *)(q + 0x28) =
            *(u16 *)((u8 *)b + 0x2) + (i - 2) * 0x40 - 0x20;
        *(s16 *)(q + 0x12) = *(s16 *)(q + 0x1A) =
            *(u16 *)((u8 *)b + 0x6) + (j - 3) * 0x40 - 0x20;
        *(s16 *)(q + 0x22) = *(s16 *)(q + 0x2A) =
            *(u16 *)((u8 *)b + 0x6) + (j - 2) * 0x40 - 0x20;
        *(s16 *)(q + 0x14) = *(u16 *)((u8 *)b + 0xA) + D_8002745C[j][i];
        *(s16 *)(q + 0x1C) = *(u16 *)((u8 *)b + 0xA) + D_8002745C[j][i + 1];
        *(s16 *)(q + 0x24) = *(u16 *)((u8 *)b + 0xA) + D_8002745C[j + 1][i];
        *(s16 *)(q + 0x2C) = *(u16 *)((u8 *)b + 0xA) + D_8002745C[j + 1][i + 1];
        *(u32 *)q = 0xF8F82CC0;
        *(s16 *)(q + 0x4) = 0x13D;
        q[0x8] = i * 0x10;
        q[0x9] = j * 0x10 - 0x80;
        if (i == 6)
        {
            q[0xA] = 0xF;
        }
        else
        {
            q[0xA] = 0x10;
        }
        if (jj == 6)
        {
            q[0xB] = 0xF;
        }
        else
        {
            q[0xB] = 0x10;
        }
        ((u32 *)r)[0x2] = (u32)q;
        cc = (u16)D_800273BC;
        *(s16 *)(q + 0x4C) = 8;
        D_800273BC = cc + 1;
    }
}

/* The bank's four-state cutscene mover, switched on *(s16 *)(p + 0x48) as a
   compare chain over {0,1,2,3} (beq / slti / beqz / beq / beq -- no .rodata
   block, so this function owns no jump table). State 0 seeds the aim point
   from the party record D_80083780 and the view's +0x88, offsets it by
   func_8003DE58's projection of D_800E3D18's current record into D_80082E80's
   slot, publishes it at p+0x30..0x34 and FALLS THROUGH into state 1. State 1
   eases the mover an eighth of the way to the aim point, scaled by p+0x54 + 1
   and divided by the p+0x4C countdown, settles p+0x38..0x3C a quarter of the
   way toward the mover, republishes them into the vector at b+0x2/0x6/0xA,
   and at the end of the countdown fires func_800B8D64, reloads the countdown
   with 5 and advances. State 2 bleeds the slot's +0xC by a p+0x4Cth a frame
   and on the last frame either tears the actor down or arms state 3's
   four-frame timer. State 3 reprojects every frame (ignoring the result,
   unlike the donor), then once the p+0x50 timer runs out fires
   func_s56_80025334 at the slot's +0x24/+0x25 cell scaled by the D_8006CCD8 /
   D_8006CCE8 direction pair and tears down.
   264 words, 0 differing.

   DONOR: csrc/ovl_dng_s69.c's func_s69_800245A0, named by dngcallee at 8 of
   11 symbols with a clean gap to the 7/11 tier, and with this body's exact
   save list (s0, s1, ra). dngtwin was not needed. Both of the donor's
   positional facts carried, one to the digit:

     state 0's `g = D_80083780' pointer local (as written)   264 /  0
     the same two reads inline off the symbol            263 words / 12
       (donor records 270 words / 12 -- same figure, same direction)
     state 3's `sl = D_80082E80' pointer local (as written)  264 /  0
     the same four reads inline off the symbol           265 words /  7
       (donor records 272 words / 5)

   THE UNIT'S OWN `THE STATE READ MUST BE AN ARRAY REF' FACT IS FREE HERE, and
   the precondition is readable off the listing before any compile. On
   func_s56_80024648 and func_s56_800248BC the plain cast costs 8 and 66 rows,
   because the `lh' of the state cannot hoist above the `sh' to the
   fixed-address scalar D_80027330 and then has NO INSTRUCTION LEFT to fill
   its load slot, so the `addu $sN, $a1, $zero' copy is pushed out of the
   prologue. A compare-chain switch supplies its own filler -- the root test's
   `addiu $v0, $zero, 0x1' sits between the `lh' and the `beq' -- so the
   dependence costs nothing and the copy stays in the branch's delay slot in
   the target too. Measured: plain cast (as written) 264 / 0, array ref
   `((s16 *)p)[0x24]' 264 / 0, FREE either way. Both earlier bodies gate on
   `(... == 0)', which is a bare `beqz' with no comparand to materialise, so
   nothing covers the pinned load there.

   THE POINTER-SUM/INTEGER-SUM OPERAND-ORDER LEVER ONLY BITES WHEN THE BASE IS
   A POINTER *LOADED FROM MEMORY*, not when it is a symbol's address:

     state 0's index off D_800E3D18, `*(u8 **)(i + (s32)D_800E3D18)'  264 / 0
     the same as a pointer sum, `*(u8 **)((u8 *)D_800E3D18 + i)'      264 / 2
     state 3's two table reads as pointer sums, `(u8 *)D_8006CCD8 + k' 264 / 0
     the same two as integer sums, `k + (s32)D_8006CCD8'              264 / 0

   D_800E3D18 is an `lw'-loaded `u8 *', so both addends are pseudos that
   already exist and `pointer_int_sum's PLUS(ptr, intop) reaches the `addu'
   base-first -- the target has `addu $v0, $v0, $v1' with the index first, so
   the source added the integer. D_8006CCD8 is a SYMBOL_REF, which costs no
   insn at expand time and has to be forced into a register after the index is
   computed, and that forcing -- not the tree -- fixes the operand order, so
   the spelling is NOT MEASURABLE there. The 2-row figure is the whole residue
   of the first compile; everything else in this body landed off the donor. */
void func_s56_80026DE4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u16 out[4];
    u16 pos[3];
    u8 *g;
    u8 *sl;
    s32 i;

    D_80027330 += 1;
    switch (*(s16 *)(p + 0x48))
    {
    case 0:
        g = D_80083780;
        pos[0] = *(u16 *)(g + 0x2);
        pos[1] = *(u16 *)(g + 0x6);
        pos[2] = *(u16 *)(D_800E3D7C + 0x88) - 0x50;
        i = ((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 7) & 0x1C;
        func_8003DE58(*(u8 **)(i + (s32)D_800E3D18), (UnkSlot *)D_80082E80, (s16 *)out, 0);
        pos[0] += out[0];
        *(s16 *)(p + 0x30) = pos[0];
        pos[1] += out[1];
        *(s16 *)(p + 0x32) = pos[1];
        pos[2] += out[2];
        *(s16 *)(p + 0x34) = pos[2];
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        /* fallthrough */
    case 1:
        *(s16 *)((u8 *)b + 0xE) =
            *(u16 *)((u8 *)b + 0xE) +
            (*(s16 *)(p + 0x38) +
             (*(s16 *)(p + 0x30) - *(s16 *)(p + 0x38)) / 8 * (*(s16 *)(p + 0x54) + 1) -
             *(s16 *)((u8 *)b + 0xE)) /
                *(s16 *)(p + 0x4C);
        *(s16 *)((u8 *)b + 0x12) =
            *(u16 *)((u8 *)b + 0x12) +
            (*(s16 *)(p + 0x3A) +
             (*(s16 *)(p + 0x32) - *(s16 *)(p + 0x3A)) / 8 * (*(s16 *)(p + 0x54) + 1) -
             *(s16 *)((u8 *)b + 0x12)) /
                *(s16 *)(p + 0x4C);
        *(s16 *)((u8 *)b + 0x16) =
            *(u16 *)((u8 *)b + 0x16) +
            (*(s16 *)(p + 0x3C) +
             (*(s16 *)(p + 0x34) - *(s16 *)(p + 0x3C)) / 8 * (*(s16 *)(p + 0x54) + 1) -
             *(s16 *)((u8 *)b + 0x16)) /
                *(s16 *)(p + 0x4C);
        *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) + ((*(s16 *)((u8 *)b + 0xE) - *(s16 *)(p + 0x38)) >> 2);
        *(s16 *)((u8 *)b + 0x2) = *(s16 *)(p + 0x38);
        *(s16 *)(p + 0x3A) = *(u16 *)(p + 0x3A) + ((*(s16 *)((u8 *)b + 0x12) - *(s16 *)(p + 0x3A)) >> 2);
        *(s16 *)((u8 *)b + 0x6) = *(s16 *)(p + 0x3A);
        *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + ((*(s16 *)((u8 *)b + 0x16) - *(s16 *)(p + 0x3C)) >> 2);
        *(s16 *)((u8 *)b + 0xA) = *(s16 *)(p + 0x3C);
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) > 0)
        {
            return;
        }
        func_800B8D64(*(s16 *)(p + 0x30), *(s16 *)(p + 0x32), *(s16 *)(p + 0x34));
        *(s16 *)(p + 0x4C) = 5;
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        return;
    case 2:
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x4C);
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) - 1;
        if (*(s16 *)(p + 0x4C) > 0)
        {
            return;
        }
        if (*(s16 *)(p + 0x54) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        c[0xC] = 0;
        *(s16 *)(p + 0x50) = 4;
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
        return;
    case 3:
        sl = D_80082E80;
        func_8003DE58(*(u8 **)(sl + 0x8), (UnkSlot *)sl, (s16 *)pos, 0);
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) > 0)
        {
            return;
        }
        func_s56_80025334(
            (sl[0x24] + *(s16 *)((u8 *)D_8006CCD8 + ((*(u16 *)(D_800E3D7C + 0x2A) >> 8) & 0xE)) * 4) * 64 + 0x20,
            (sl[0x25] + *(s16 *)((u8 *)D_8006CCE8 + ((*(u16 *)(D_800E3D7C + 0x2A) >> 8) & 0xE)) * 4) * 64 + 0x20,
            *(s16 *)(D_800E3D7C + 0x88));
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
}

/* The nine-node burst func_s56_80024388 fires: allocate nine 0x2 nodes, give
   each func_s56_80026DE4 as its step and D_800273B0 as its slot's +0x8
   record, copy the caller's mover position into both the new mover's position
   and velocity and into the payload's +0x38 triple, scale the last node 0x800
   and the rest 0x200, ramp the grey down by 4 per node and return the last
   node allocated. No xsibs twin; the spawner skeleton is
   csrc/ovl_dng_s21.c's func_s21_8002495C (same func_8003FC64 /
   `n->unk_10 = step' / func_8004491C(n, (u32)func_800CEEFC) order).

   Three spellings were measured here, and each is worth real rows:

     * `q = (u8 *)n + 0x20' must be computed AFTER both calls. Before them it
       is live across a call, takes a SEVENTH callee-saved register and the
       body is 74 words / 63 differing.
     * the 0x200-vs-0x800 scale is an `if'/`else' with the `sh' DUPLICATED PER
       ARM, which cross-jumping merges back. A value in a temp collapses to a
       pre-loaded 0x800 plus a conditional overwrite: 72 words / 45 differing.
     * each position group is ONE CHAINED assignment,
       `*(q + 0x38) = *(m + 0xE) = *(m + 0x2) = *(u16 *)(b + 0x2)', emitted
       innermost-first. A `u16' local holding the loaded halfword gives the
       same one `lhu' but swaps $v0 and $v1 between `m' and the value:
       13 differing with every opcode right.

   73 words, 0 differing. */
UnkNode *func_s56_80027204(UnkVec *b)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    s32 i;

    for (i = 0; i < 9; i++)
    {
        n = func_8003FC64(2);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s56_80026DE4;
            func_8004491C(n, (u32)func_800CEEFC);
            m = (u8 *)n->unk_08;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x38) = *(s16 *)(m + 0xE) = *(s16 *)(m + 0x2) =
                *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(q + 0x3A) = *(s16 *)(m + 0x12) = *(s16 *)(m + 0x6) =
                *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(q + 0x3C) = *(s16 *)(m + 0x16) = *(s16 *)(m + 0xA) =
                *(u16 *)((u8 *)b + 0xA);
            r = (u8 *)n->unk_0C;
            *(u32 *)(r + 0x8) = (u32)D_800273B0;
            if (i != 8)
            {
                *(s16 *)(r + 0x1E) = 0x200;
            }
            else
            {
                *(s16 *)(r + 0x1E) = 0x800;
            }
            r[0xC] = -0x80 - ((8 - i) << 2);
            *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
            *(s16 *)(q + 0x54) = i;
            if (i == 8)
            {
                *(s16 *)(q + 0x54) = 7;
            }
            *(s16 *)(q + 0x4C) = 8;
        }
    }
    return n;
}
