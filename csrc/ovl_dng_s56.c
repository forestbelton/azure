#include "common.h"

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
void func_s56_80025540(u8 *, UnkVec *, UnkSlot *, s16);
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
void func_s56_80026DE4(u8 *, UnkVec *, u8 *, u8 *);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80024648);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80024744);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_800248BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80024C3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025238);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025334);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025540);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_8002592C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025AD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80025C8C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80026064);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80026BA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s56", func_s56_80026DE4);

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
