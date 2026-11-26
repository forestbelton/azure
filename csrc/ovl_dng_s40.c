#include "common.h"

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
void func_s40_80024400(u8 *p, UnkVec *v);
s32 func_s40_800241B0(u8 *p, UnkVec *v);

UnkNode *func_8003FD64(s32, void *);

s32 func_800BCB04(s32, s32, s16);
extern s16 D_80025E80;
typedef struct UnkDng24094SP
{
    u8 pad_00[0x8];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8 pad_3C[0x34];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8 pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8 pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    u16 unk_104;
    u16 unk_106;
    u16 unk_108;
    u16 unk_10A;
} UnkDng24094SP;

typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk64
{
    u8 b[0x40];
} UnkDngS58Blk64;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
s32 func_8009D218(u8 *, s32, u8 *);
u16 func_800A6D30(void);
void *func_800990FC(void);
u8 *func_8009929C(s32, u8 *);
void *func_80099194(u8 *, u8 *);
void *func_80099734(u8 *, u8 *);
u8 *func_80099290(u8 *);
s32 func_800A5720(void *);
void func_80099844(u8 *, u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
extern u8 D_800E2096[];
extern u8 D_800E20BC[];

/* The slot's "use the item on me" responder, reached from
   func_s41_800257D0's state 5: refuse if func_8009D218 does, then on a
   1-in-p[0x3] roll (or with the caller's strength at 0xFF) spend half the
   remaining 0x27 charges and print the three-part D_800E2096 message,
   otherwise print D_800E20BC, and tell func_800B4C7C 0x53 either way.
   77 words, 0 differing, 0 aligned differences.

   func_8009D218 TAKES THE THIRD PARAMETER STRAIGHT THROUGH. Nothing in
   this body sets $a2 and nothing reads it either; the caller at
   0x8002661C passes `g' there and the call inherits it, so the arity is
   three and `q' is otherwise unused. A two-argument alias is 1 word short.

   THE DIVIDEND IS AN `s32' LOCAL AND THE TRUNCATION IS WRITTEN AT THE
   DIVISION: `x = (u16)r % p[0x3]'. With `u16 r' and a plain `r % p[0x3]'
   it is 76 words and 5 aligned differences -- the divisor's extra `addu'
   never appears, because the HImode pseudo dies at the zero-extend and the
   remainder reuses its register. The compound form
   `x = func_800A6D30(); x %= p[0x3];' is the right 77 words and 3 rows:
   expand_divmod drops a target mentioned in op0, so the remainder lands in
   the dividend's own register where the target gives it a third. The
   divisor guard is a real source-level `if (p[0x3] != 0)', not the
   assembler's div-by-zero expansion: the zero arm sets the remainder to 0
   and jumps, where a `break 7' would trap. Reading p[0x3] once into a
   local measures the same 0; the two reads are not what buys the copy.

   The message-assembly chain is csrc/ovl_dng_s18.c's func_s18_800240B8's,
   with func_8009929C(8, h) in place of its first func_80099194 -- two
   locals, `h' chained and `m' kept for func_800A5720. 
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x1 at 1 of 2 sites, 0x27 -> 0x26 at all 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s40_80024750(u8 *p);
UnkNode *func_8003FC64(s32);
extern u8 D_80045340[];
extern u8 D_80025E64[];
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
extern u16 D_80025EE8[];
extern const u8 D_s40_80024028[];
extern const u8 D_s40_80024030[];

s32 rand(void);
extern const u8 D_s40_80024038[];
u8 *func_8003DE58(u8 *, u8 *, void *, s16);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E3D68;
extern s32 D_8008346C;
extern u8 D_80025E34[];
extern u8 D_80025E40[];
extern u8 D_80025E58[];
extern u8 D_80025E70[];
extern u8 D_s40_80025E28[];

void func_s40_8002407C(u8 *p, u8 b, u8 *q)
{
    void *m;
    void *h;
    s32 r;
    s32 x;
    s16 n;

    if (func_8009D218(p, 1, q) != 0)
    {
        return;
    }
    r = func_800A6D30();
    if (p[0x3] != 0)
    {
        x = (u16)r % p[0x3];
    }
    else
    {
        x = 0;
    }
    if (x < (b << 5) || b == 0xFF)
    {
        if (p[0x26] >= 2)
        {
            n = (p[0x26] + 1) >> 1;
            p[0x26] -= n;
            h = func_800990FC();
            m = h;
            h = func_8009929C(8, h);
            h = func_80099734(p, h);
            h = func_80099194(D_800E2096, h);
            func_80099290(h);
            func_800A5720(m);
            func_800B4C7C(0x53, p, n, 1);
            return;
        }
    }
    func_80099844(p, D_800E20BC);
    func_800B4C7C(0x53, p, -1, 1);
}

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 life counter at +0x32 over 0x100. The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D5AC0 (120 words, resident), which is
   this function without the fade; the relink moves every scratchpad slot down
   by 4 -- the input SVECTOR to +0x00, the OT cursor to +0x18, the OT base to
   +0x20, RotTransPers' two out-parameters to +0x90/+0x94 and the OT index to
   +0xC0 -- so the struct is this unit's own, and the input vector is the
   scratchpad base itself, which is why $a0 is a bare register copy.

   THE FADE REPLACES THE TWIN'S DEAD setRGB0 ECHO. func_800D5AC0 needs
   setRGB0(t, t->r0, t->g0, t->b0) to keep three loads whose stores cse
   strips; here the three lbu/mult/sb runs are the real work and the echo must
   NOT be written -- added in front of the tag setup it is 151 words and 76
   differing.

   `/ 0x100' AND NOT `>> 8': the bgez/addiu 0xFF pair ahead of each sra is the
   signed division's correction, and `>> 8' drops it -- 139 words, 102
   differing.

   THE SCALE FACTOR IS RE-READ AT ALL THREE CHANNELS. A local for
   *(s16 *)(p + 0x32) is 145 words and 109 differing; the target has three
   separate `lh 0x32($s3)'.

   Three orderings, every one re-measured here rather than taken from the
   twin: the colour word and its three scalings come BEFORE the tag setup
   (setTile1 first is 147 words and 111 differing); `sp->unk_18' is assigned
   BEFORE `sp->unk_20' even though the target STORES them the other way round
   (swapped, 11 differing, against the twin's 20 for the same pair); and
   `p = (u8 *)n + 0x20;' comes before `v = n->unk_08;' (swapped, 17
   differing, against the twin's 4).

   setTile1(t) plus setSemiTrans(t, 1) is the whole `sb 2, 0x3' /
   `sb 0x6A, 0x7' pair: cse folds the getcode back to the 0x68 setTile1 just
   stored and drops the dead store. `(SVECTOR *)&sp->unk_00' written for
   `(SVECTOR *)sp' is a tie at 0.

   148 words, 0 differing. raw 7 == relocdump's 7, none on a function symbol:
   1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on RotTransPers, GetTPage and
   func_80067F20, and one on .text for the loop's own backward `j'. The
   +0x90/+0x94 addresses that splat auto-names D_1F800090/D_1F800094 carry no
   relocation on this side and come out byte-identical, so seqcheck counts one
   extra entry in the target and reports seq=FAIL on that bookkeeping alone. */
s32 func_s40_800241B0(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / 0x100;
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / 0x100;
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / 0x100;
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_80025E80 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80025E80
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80025E80)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_80025E80, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s40_80024400(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025E80 = 1;
    r = func_800BCB04(x, y, z + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_08 += 0x20000 + (rand() & 0xFFF);
    }
    if ((*(s16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s40_80024400 as the step and
   func_s40_800241B0 as the draw, with `b` at the payload's +0x14, `d` at
   +0x32 and `c` at +0x8.  72 words, 0 differing.  The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 -- the same 0x211 spawner
   without the jitter.

   THE THREE STACK OFFSETS ARE `s16` AND THE SUBTRACTION WEARS AN `(s16)`
   CAST, and both halves of that are load-bearing:

   * `s32 dx/dy/dz` is 66 words and 71 differing.  An `s32` stack parameter
     keeps its incoming MEM as its rtl and is rematerialised with a
     `lw 0x48($sp)` at its single use; the promoted `s16` goes through a
     pseudo, which register allocation then parks in $s2/$s3/$s4.  The six
     missing words are those three registers' save/restore pairs.  (Same
     mechanism, opposite sign, as func_800D6AD4's own note.)
   * `s16` with a bare `(dx - 0x10)` is 74 words and 20 aligned differences:
     fold's associate step splits `(X + jitter) + (dx + -0x10)` into
     `(X + jitter + dx) + -0x10` and the `addiu` lands on the running sum
     instead of on the parameter.  The NOP_EXPR the cast leaves blocks that
     split, which is the whole of the remaining 20 rows -- with it the
     `andi` falls into the `lhu`'s load-delay slot and the destination's
     `lw 0x8($s0)` moves ahead of the first `addu` on its own.
   THIS COPY'S OWN CONSTANT: -0x10 -> 0x10 at 1 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s40_800244CC(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s40_80024400;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz + 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s40_800241B0);
        *(u32 *)(p + 0x8) = c;
    }
}

/* The shutter's frame counter and its eight-step shutter animation, and IT
   OWNS A JUMP TABLE.  jtbl_s40_80024008: 8 `.word's = 8 cases + 0 pad,
   dispatch `sltiu $v0, $v1, 0x8' with NO minval subtraction, so the case set
   is 0..7 and every entry is a real case -- the range-check `beqz' target
   `.Ls40_800246C0' is the body's COMMON TAIL, not one of the entries, so
   there is no `default:' statement.  Three distinct targets:

     .Ls40_80024658   cases 0 1 2 4 5 6      .Ls40_80024674   case 3
     .Ls40_8002469C   case 7

   67 words, 0 differing, 0 aligned differences.

   THE DECREMENT IS SPLIT AROUND THE D_80025E80 STORE.  `t = *(u16 *)(p + 0x2)
   - 1; D_80025E80 = 1; *(u16 *)(p + 0x2) = t;' is 67 / 0; written in place as
   `*(u16 *)(p + 0x2) -= 1;' before the store it is 66 words and 31 aligned
   rows.  gcc 2.7.2 cannot disambiguate `mem:HI (lo_sum D_80025E80)' from
   `mem:HI ($a1 + 2)', so neither the load nor the store can cross it and the
   only way to get the `sh %lo(D_80025E80)' emitted BETWEEN them is to split
   the read-modify-write.  Same device as csrc/ovl_dng_s58.c's
   func_s58_80024374.

   ARM ORDER IS MEASURABLE HERE -- the three arms differ in length and in
   which byte they touch, so jump2 does not collapse them -- and the entries
   want ENTRY-ADDRESS order: the {0,1,2,4,5,6} group, then case 3, then case
   7.  Case 3's arm written first is 67 / 15, case 7's first is 67 / 16.

   Inside case 3, `p[0x28] -= 0x60' comes BEFORE `p[0x29] += 0x20' (the other
   way is 67 / 10).  The gate is `== 0' with the SWITCH as the then arm;
   inverted, with the increment as the then, it is 67 / 6.

   p+0x1C is read TWICE because the two reads differ in extension: `lh' for
   the `*(s16 *)' test and `lhu' for the `*(u16 *) += 1', which cse cannot
   unify.  The third parameter is an UnkSlot the body only writes
   (+0x1C/+0x1E = 0x2000); the second is never read, which is why gcc parks
   `p' in $a1 and the body opens with `addu $a1, $a0, $zero'. */
void func_s40_800245EC(u8 *p, UnkVec *v, u8 *q)
{
    u16 t;

    t = *(u16 *)(p + 0x2) - 1;
    D_80025E80 = 1;
    *(u16 *)(p + 0x2) = t;
    *(s16 *)(q + 0x1C) = 0x2000;
    *(s16 *)(q + 0x1E) = 0x2000;
    if (*(s16 *)(p + 0x1C) == 0)
    {
        *(s16 *)(p + 0x1C) = 0;
        switch (*(s16 *)(p + 0x1E))
        {
        case 0:
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
            *(u16 *)(p + 0x1E) += 1;
            p[0x28] += 0x20;
            break;
        case 3:
            *(u16 *)(p + 0x1E) += 1;
            p[0x28] -= 0x60;
            p[0x29] += 0x20;
            break;
        case 7:
            *(s16 *)(p + 0x1E) = 0;
            p[0x28] -= 0x60;
            p[0x29] -= 0x20;
            break;
        }
    }
    else
    {
        *(u16 *)(p + 0x1C) += 1;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same frame counter with no animation: spend p+0x2, publish 0x1000 into
   the renderable's two scale words and raise both kill bits at zero.
   22 words, 0 differing, 0 aligned differences.

   THE TEST VALUE IS AN `s32' LOCAL READ BEFORE THE D_80025E80 STORE.  With
   the test inline (`if (*(s16 *)(p + 0x2) <= 0)') it is 23 words and 11
   aligned rows -- the `lh' cannot hoist above the fixed-address halfword
   store, so the store cannot reach the `bgtz' delay slot.  Declared `s16 t'
   it is 23 / 6: the HImode pseudo is loaded with `lhu' and sign-extended
   with an `sll' where the target has a plain `lh'.

   MEASURED AND FREE, so it is NOT the reason this lands: the decrement's
   lvalue.  `*(s16 *)(p + 0x2) -= 1' is also 22 / 0.

   The second parameter is never read; `p' takes $a1 and the body opens with
   `addu $a1, $a0, $zero'.  csrc/ovl_dng_s58.c's func_s58_80024374 is the
   same local-around-the-global-store shape one size down. */
void func_s40_800246F8(u8 *p, UnkVec *v, u8 *q)
{
    s32 t;

    *(u16 *)(p + 0x2) -= 1;
    *(s16 *)(q + 0x1C) = 0x1000;
    *(s16 *)(q + 0x1E) = 0x1000;
    t = *(s16 *)(p + 0x2);
    D_80025E80 = 1;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The bare teardown step: pin D_80025E80 and raise both kill bits.
   12 words, 0 differing, 0 aligned differences.  One parameter -- nothing in
   the body reads $a1.  `D_80025E80 = 1;' is FIRST; written last it is 11
   words and 8 aligned rows, because the two 0x8000 read-modify-writes then
   lose the `lui' that fills the load delay. */
void func_s40_80024750(u8 *p)
{
    D_80025E80 = 1;
    *(u16 *)(p - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* The shutter's third step: give up at once when the +0x30 object's +0x13
   byte has reached 0x33..0x36, otherwise spend p+0x2, flip the +0xA toggle
   and glide the mover's +0x8 by +/-0x20000, hand the 0x80 flag from the
   renderable to the object at -0x14 of the +0x30 object, and drive the three
   colour bytes off the counter -- up over the first 0x3C frames and down
   again after -- then raise both kill bits and take the 0x80 flag back.
   135 words, 0 differing, 0 aligned differences.

   THREE STATEMENTS IN THIS ORDER, AND ALL THREE POSITIONS ARE PRICED:
   `h = *(u8 **)(p + 0x30);' then `D_80025E80 = 1;' then `s = h[0x13];'.
   Without the `h' local, reading the byte in one expression before the store,
   it is 133 words / 8 rows; with the store FIRST it is 136 / 4.  The local is
   what puts the `lw 0x30' ABOVE the fixed-address store and the `lbu 0x13'
   BELOW it, which is the only arrangement gcc can reach -- it cannot move
   either memref across that store itself.

   THE FIRST GATE IS A RANGE TEST IN THE SOURCE: `s >= 0x33 && s <= 0x36' is
   what fold's make_range turns into the listing's `addiu -0x33' / `sltiu 4'.
   csrc/ovl_dng_s41.c's func_s41_800252E4 is this body's closest relative and
   prices the same sentence.

   MEASURED AND FREE, so it is NOT the reason this lands: writing the three
   colour bytes as `*(u8 *)((u8 *)r + 0xE)' plain casts instead of
   `r->unk_0C[2]' field refs -- 135 / 0 either way.

   The fade is `(0x3C - x) * 0x40 / 0x3C + 0x40' at both ends; the 0x88888889
   magic with `sra 5' is divisor 0x3C, not the 0x1E the same magic means with
   `sra 4'.  Both arms spell the three stores out, 0xE then 0xD then 0xC, and
   jump2 merges their last one. */
void func_s40_80024780(u8 *p, UnkVec *v, UnkSlot *r)
{
    UnkSlot *t;
    u8 *h;
    s32 s;

    h = *(u8 **)(p + 0x30);
    D_80025E80 = 1;
    s = h[0x13];
    if (s >= 0x33 && s <= 0x36)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    *(u16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0xA) == 0)
    {
        *(s16 *)(p + 0xA) = 1;
        v->unk_08 -= 0x20000;
    }
    else
    {
        *(s16 *)(p + 0xA) = 0;
        v->unk_08 += 0x20000;
    }
    t = *(UnkSlot **)(*(u8 **)(p + 0x30) - 0x14);
    r->unk_14 &= 0xFF7F;
    t->unk_14 |= 0x80;
    if (*(s16 *)(p + 0x2) < 0x3C)
    {
        r->unk_0C[2] = (0x3C - *(s16 *)(p + 0x2)) * 0x40 / 0x3C + 0x40;
        r->unk_0C[1] = (0x3C - *(s16 *)(p + 0x2)) * 0x40 / 0x3C + 0x40;
        r->unk_0C[0] = (0x3C - *(s16 *)(p + 0x2)) * 0x40 / 0x3C + 0x40;
    }
    else
    {
        r->unk_0C[2] = (*(s16 *)(p + 0x2) - 0x3C) * 0x40 / 0x3C + 0x40;
        r->unk_0C[1] = (*(s16 *)(p + 0x2) - 0x3C) * 0x40 / 0x3C + 0x40;
        r->unk_0C[0] = (*(s16 *)(p + 0x2) - 0x3C) * 0x40 / 0x3C + 0x40;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        t->unk_14 &= 0xFF7F;
    }
}

/* The shutter's fourth step and its two renderables: flip the +0xA toggle,
   then on state 0 invert a 4x4 grid of 0x18x0x15 VRAM tiles through
   D_80025EE8 and fall into state 1, which parks the two func_800B8FC8
   renderables, fades the three colour bytes off the +0x2 counter and kills
   the node at 0x64; the tail hands the 0x80 flag to the +0x30 object's -0x14
   sibling.  263 words, 0 differing, 0 aligned differences.  A compare-chain
   switch, not a table: two arms with case 0 falling through into case 1.

   THE TOGGLE'S TEST VALUE IS AN `s32' LOCAL READ BEFORE THE D_80025E80
   STORE, exactly as in func_s40_800246F8 above: inline it is 265 words and 9
   aligned rows.

   `e = *(s16 *)(p + 0x2) / 4 - 0x4E; w.vy = r1.y - e;' -- THE LOCAL IS THE
   FACT.  Written `w.vy = r1.y - (*(s16 *)(p + 0x2) / 4 - 0x4E)' it is 263
   words and 11 rows: fold's associate step rewrites `A - (B - C)' as
   `(A + C) - B' and the `addiu' lands on r1.y instead of on the quotient.

   `(r1.w >> 1)' AND NOT `/ 2': the divide form is 267 words / 9 rows, the
   four extra words being the signed-division correction.  `s16 u' for the
   +0x2 counter: `s32' is 261 / 2.

   The twin is csrc/ovl_dng_s41.c's func_s41_80025408 (242 words), which has
   the same `RECT r1; RECT r2; DVECTOR w;' stack trio off this bank's own
   rodata and the same grid loop; r1 here is the D_s40_80024028 copy and is
   only ever READ, which is why its three `lhu' reads at +0x18/+0x1A/+0x1C
   come out unsigned. */
void func_s40_8002499C(u8 *p, UnkVec *b, u8 *c)
{
    RECT r1;
    RECT r2;
    DVECTOR w;
    u8 *n;
    u16 *q;
    s32 i;
    s32 j;
    s32 k;
    s32 t;
    s32 e;
    s16 u;

    r1 = *(RECT *)D_s40_80024028;
    r2 = *(RECT *)D_s40_80024030;
    t = *(s16 *)(p + 0xA);
    D_80025E80 = 1;
    if (t == 0)
    {
        *(s16 *)(p + 0xA) = 1;
        b->unk_08 -= 0x20000;
    }
    else
    {
        *(s16 *)(p + 0xA) = 0;
        b->unk_08 += 0x20000;
    }
    switch (*(s16 *)p)
    {
    case 0:
        for (i = 0; i < 4; i++)
        {
            for (j = 0; j < 4; j++)
            {
                r2.x = i * 0x18 + 0x340;
                r2.y = j * 0x15 + 0x154;
                r2.w = 0x18;
                r2.h = 0x15;
                StoreImage(&r2, (u_long *)D_80025EE8);
                while (DrawSync(1) != 0)
                {
                    ;
                }
                q = D_80025EE8;
                for (k = 0; k < 0x1F8; k++)
                {
                    if (*q == 0)
                    {
                        *q = 0xFFFF;
                    }
                    else
                    {
                        *q = 0;
                    }
                    q++;
                }
                LoadImage(&r2, (u_long *)D_80025EE8);
            }
        }
        *(s16 *)p = *(u16 *)p + 1;
        *(u16 *)(c + 0x14) &= 0xFF7F;
    case 1:
        r2.x = 0x340;
        r2.y = 0x100;
        r2.w = 0x60;
        r2.h = 0x54;
        w.vx = 0x370;
        w.vy = 0x148;
        func_800B8FC8(*(u8 **)(p + 0x38), (s32 *)&r2, (s32 *)&w, 0, 1);
        w.vx = r1.x + (r1.w >> 1) - 0x6;
        e = *(s16 *)(p + 0x2) / 4 - 0x4E;
        w.vy = r1.y - e;
        func_800B8FC8(*(u8 **)(p + 0x3C), (s32 *)&r1, (s32 *)&w, 1, 1);
        if (*(s16 *)(p + 0x2) < 0x15)
        {
            c[0xE] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
            c[0xD] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
            c[0xC] = *(s16 *)(p + 0x2) * 0x80 / 0x14;
        }
        if (*(s16 *)(p + 0x2) >= 0x51)
        {
            c[0xE] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
            c[0xD] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
            c[0xC] = (0x64 - *(s16 *)(p + 0x2)) * 0x80 / 0x14;
        }
        u = *(u16 *)(p + 0x2) + 1;
        *(s16 *)(p + 0x2) = u;
        if (u > 0x64)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p = *(u16 *)p + 1;
            *(s16 *)(*(u8 **)(p + 0x34) + 0x90) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    n = *(u8 **)(p + 0x30);
    if (n[0x13] >= 0x33 && n[0x13] <= 0x36)
    {
        *(u16 *)(c + 0x14) |= 0x80;
    }
}

/* The 0x212 renderable the three shutter steps publish through: seed the
   mover from `b', tag it with D_80045340 and func_s40_80024750, point its
   +0x8 at the 12-byte D_80025E64 template and carry the caller's three
   colour bytes and 0x1000-complemented +0x1A across.  86 words, 0 differing,
   0 aligned differences, FIRST COMPILE off csrc/ovl_dng_b22.c's
   func_b22_80171928 (68 words) -- same 0x212 / func_8004491C / D_80045340
   opening, same `n->unk_0C' reload before the second half.

   Three orderings, all priced here rather than inherited:
   the payload stores are 0x2C, 0x30, 0x34 in source order and sched
   transposes the last two (writing 0x34 before 0x30 is 86 / 2);
   `r->unk_1E' is assigned BEFORE `r->unk_1C' (swapped, 86 / 2);
   and `r->unk_10 = 0x20' comes BEFORE `r->unk_06 = 0' (swapped, 86 / 6). */
void func_s40_80024DB8(u8 *a, UnkVec *b, u8 *c)
{
    UnkNode *n;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x2C) = *(u32 *)a;
        *(u32 *)(q + 0x30) = *(u32 *)(*(u8 **)a + 0x60);
        *(u32 *)(q + 0x34) = (u32)a;
        n->unk_10 = (u32)func_s40_80024750;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        m->unk_00 = b->unk_00;
        m->unk_04 = b->unk_04;
        m->unk_08 = b->unk_08;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x800;
        r->unk_1C = 0x800;
        r->unk_0C[0] = c[0xC];
        r->unk_0C[1] = c[0xD];
        r->unk_0C[2] = c[0xE];
        r->unk_1A = 0x1000 - *(u16 *)(c + 0x1A);
        *(UnkDngS58Name12 *)((u8 *)n + 0x40) = *(UnkDngS58Name12 *)D_80025E64;
        r->unk_08 = (u32)((u8 *)n + 0x40);
    }
}

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_s40_8002499C, func_s40_80024F10. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s40_80024028[] = {
    0x40,
    0x03,
    0x00,
    0x01,
    0x60,
    0x00,
    0x54,
    0x00,
};

/* 0x8 bytes of this bank's own rodata, between its jump tables,
   read by func_s40_8002499C. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_s40_80024030[] = {
    0x40,
    0x03,
    0x54,
    0x01,
    0x04,
    0x00,
    0x04,
    0x00,
};

/* 0x20 bytes of this bank's own rodata, between its jump tables,
   read by func_s40_80024F10 as the eight-direction velocity table.
   Spelled as BYTES because that is all the reference witnesses -- the
   s16 pair shape is only visible on the STACK copy -- and `const'
   because without it the array goes to .data and the linker puts it
   outside this segment's declared .rodata. */
const u8 D_s40_80024038[] = {
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0x20,
    0x00,
    0x00,
    0x00,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0xE0,
    0xFF,
    0x00,
    0x00,
    0xE0,
    0xFF,
    0x20,
    0x00,
    0xE0,
    0xFF,
};

/* THE SHUTTER SCRIPT'S MASTER STEP, AND IT OWNS A JUMP TABLE AND THE
   0x20-BYTE D_s40_80024038 TABLE ABOVE IT.  jtbl_s40_80024058: 9 `.word's =
   9 cases + 0 pad, dispatch `sltiu $v0, $v1, 0x9' with NO minval
   subtraction, so the case set is 0..8.  The entry list in table order:

     case 0  .Ls40_80025030      case 5  .Ls40_80025CDC
     case 1  .Ls40_800250A4      case 6  .Ls40_80025D20
     case 2  .Ls40_8002536C      case 7  .Ls40_80025DF4  (NO ARM)
     case 3  .Ls40_8002561C      case 8  .Ls40_80025D90
     case 4  .Ls40_80025BB0

   THE DEFAULT IS THE EPILOGUE AND CASE 7 IS WRITTEN AS NOTHING AT ALL.
   `.Ls40_80025DF4' is both the range-check `beqz' target and the first word
   of the epilogue, so there is no `default:' and no `case 7:'.  Eight real
   arms, written 0 1 2 3 4 5 6 8 -- ascending, which here IS entry-address
   order -- with case 0 falling through into case 1.  ARM ORDER IS MEASURABLE
   on this table: exchanging the case 5 and case 6 arms is 966 / 36 and
   writing case 8 before case 6 is 966 / 54.  And `case 7: break;' or
   `default: break;' each read 966 / 0, which is NOT MEASURABLE rather than
   free -- both put the switch's own end label in entry 7, the same label the
   implicit default uses, so the emitted table is byte-identical either way.

   966 words, 0 differing, 0 aligned differences.

   DONOR: csrc/ovl_dng_s38.c's LANDED func_s38_80024FA4 (627 words), which
   dngtwin and xsibs both miss entirely and which dngcallee names alone at
   12-of-23 symbols with a clean gap to the next tier.  Same
   `(u8 *p, UnkVec *v, u8 *c)' signature, same `o'/`n'/`m' head, same 9-entry
   table with case 7 absent, same `u16 out[3]' hand read, same D_80045340 tag
   block, same `D_8006CCD8[facing]'/`D_8006CCE8[facing]' corner pair and the
   same 0x20-byte 1-aligned table copy, which is what this unit's
   already-present `UnkDngS58Blk32'/`UnkDngS58Rec' typedefs are for.

   THE WHOLE FIGHT WAS ONE WORD IN CASE 3.  The second func_800B8FC8 block's
   `r.w = 0x60' must go through a LOCAL (`ww = 0x60; r.w = ww;') while the
   first block writes the literal:

     | spelling                                   | words | rows |
     |--------------------------------------------|-------|------|
     | `ww' local in BLOCK 2 ONLY           KEPT  |  966  |   0  |
     |   ... the literal 0x60 in both blocks      |  965  | 156  |
     |   ... `ww' local in BOTH blocks            |  965  | 156  |

   With the literal in both blocks cse puts the two `(const_int 96)' pseudos
   in ONE QUANTITY, the unified pseudo spans both calls, global-alloc gives it
   a callee-saved register, and that hoisted `addiu' replaces the target's
   second `addiu 0x60'.  That one extra allocno also displaces `p' from $s5
   to $s6 and `o' from $s7 to $fp across the whole 966 words, which is where
   150 of the 156 rows come from -- the residue reads as a whole-body
   register rotation and is a single `reg_n_sets' question.  A local in BOTH
   blocks has two sets and becomes a global allocno again; in block 2 only it
   is one set and one use, stays block-local and caller-saved, and the
   constant is materialised twice as the ROM has it.

   The diagnostic that isolated it: with block 2's `r.w = 0x61' the body is
   966 words / 1 aligned row (the immediate alone), which proved every one of
   the other 155 rows was downstream of that single hoist.

   SWEPT FLAT FIRST, so none of these is the cause: 8 permutations of the
   local DECLARATION order (965 / 156 for every one), 8 permutations of the
   six rect/vector store orders inside the two blocks (965 words throughout,
   147 to 175 rows), and either `r.w' lvalue written as a plain cast
   (965 / 156).  csrc/ovl_dng_s41.c's func_s41_80025408 carries the same
   shape under m2c's name for it, `int new_var = 0x60; ... k = new_var;
   r1.w = k;'.

   THE DONOR'S ARRAY-REF FACT IS FREE HERE AND ITS FIGURE BELONGS TO A
   DIFFERENT SENTENCE.  func_s38_80024FA4 prices case 0's +0xA bump as
   `((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1' at 7 rows against the plain cast;
   on this body both spellings are 966 / 0.  What that position actually buys
   here is the STORE ORDER: `D_80025E80 = 1;' must come BEFORE
   `*(s16 *)(p + 0x7E) = ...', and the other way round is 967 words / 14 rows.
   The `lhu 0x2A($s7)' that feeds +0x7E is already an ARRAY_REF
   (`((u16 *)o)[0x15]'), so MEM_IN_STRUCT_P lets it hoist above the
   fixed-address store without any help from the bump.

   EACH OF THE FOUR SPAWNS NEEDS EXACTLY ONE STATEMENT BETWEEN ITS TWO
   `s->unk_14' UPDATES, AND WHICH ONE IS PRICED.  Two updates of one lvalue
   with nothing between them collapse: gcc merges them and the first `sh'
   dies, which is one word short.  Spawns A, C and D interpose
   `s->unk_10 = <const>' and put `s->unk_06 = 0' after it; spawn B interposes
   `s->unk_06 = 0' and puts `s->unk_10 = 0x20' first.  Writing `s->unk_06 = 0'
   first is 965 / 12 on A, 965 / 12 on C and 965 / 12 on D; writing
   `s->unk_10' first on B is 965 / 6.  The interposed statement is also what
   decides whether the 0x14 value needs the target's `addu $v1, $v0, $zero'
   copy, so the two halves of the fact are one fact.

   Spawn A's `s->unk_22 = -D_800DDC40[...] / 2;' comes BEFORE its three
   colour bytes: after them it is 967 words / 17 rows.

   FOUR OF THE DONOR'S FACTS HELD AT ITS OWN MAGNITUDES, re-measured here:
   the `tbl' copy BEFORE `n'/`m' (after them, 967 / 6); case 1's `k' local in
   the `t == NULL' arm (without it, 967 / 8); case 1's +0xA store before the
   +0x88 store (swapped, 966 / 8); and the 0x8000 gate with the `out[]' arm
   as the `then' (arms swapped, 965 / 5).

   Case 2's three colour bytes ascend 0xC, 0xD, 0xE here (descending is
   966 / 2) and each writes the whole squared expression out; one local for
   the squared base is 966 / 9.  Case 2 also calls func_s40_80024DB8 in ALL
   THREE arms of its exit gate -- written once after the gate, jump2 cannot
   rebuild it and the body is 959 words / 77 rows.

   THE SPAWNER INTERPRETS ITS OWN STEPS, which is why no payload offset in
   this unit needed guessing: n5's payload takes +0x2C = o, +0x30 =
   *(u32 *)(o + 0x60), +0x34 = p and +0x38/+0x3C/+0x40 = the three sibling
   payloads, and func_s40_8002499C reads exactly those at +0x38 and +0x3C
   (its two func_800B8FC8 renderables), +0x34 and +0x30. */
void func_s40_80024F10(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    RECT r;
    DVECTOR w;
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    UnkNode *n3;
    UnkNode *n4;
    UnkNode *n5;
    UnkSlot *s;
    u8 *o;
    u8 *m;
    u8 *m2;
    u8 *e;
    u8 *t;
    u8 *u;
    u8 *g;
    u8 *q;
    s32 i;
    s32 k;
    s32 z;
    s32 ww;
    s16 j;

    o = *(u8 **)p;
    r = *(RECT *)D_s40_80024028;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s40_80024038;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0x96) = *(UnkDngS58Name12 *)D_80025E70;
        *(u32 *)(c + 0x8) = (u32)(p + 0x96);
        D_80025E80 = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DE58((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (u8 *)n->unk_0C, out, 0) == NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = z;
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) = z - 0x40;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x60;
            c[0xE] = 0x14;
            c[0xD] = 0x14;
            c[0xC] = 0x14;
            *(s16 *)(c + 0x1E) = 0x1000;
            *(s16 *)(c + 0x1C) = 0x1000;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] + 0x40);
            e = *(u8 **)(o - 0x14);
            p[0xA2] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA3] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24] ? *(s8 *)(o + 0x73) - e[0x25]
                                                                 : *(s8 *)(o + 0x72) - e[0x24]) *
                          2 -
                      1;
        }
        else
        {
            k = *(u16 *)(o + 0x88) - 0x50;
            p[0x7B] = 0x20;
            *(s16 *)(p + 0x78) = k;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x8A) = *(s8 *)(p + 0x7B) * *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
        func_s40_80024DB8(p, v, c);
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            func_s40_800244CC((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x2020E0,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        c[0xC] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xD] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xE] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B) + 1) * 0x6C / *(s16 *)(p + 0x8A) +
                 0x14;
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            break;
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                *(s16 *)(p + 0xA) = 3;
                *(s16 *)(p + 0x82) = 0;
                func_800A56E0(0x300);
                g = *(u8 **)(*(u8 **)(o + 0x60) - 0x14);
                if ((*(u8 **)(o + 0x60))[0x13] < 0x33 || (*(u8 **)(o + 0x60))[0x13] > 0x36)
                {
                    *(u16 *)(g + 0x14) |= 0x800;
                    *(u32 *)(*(u8 **)(o + 0x60) + 0x14) |= 0x100000;
                }
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                func_s40_80024DB8(p, v, c);
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
                func_s40_80024DB8(p, v, c);
            }
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            func_s40_80024DB8(p, v, c);
        }
        break;
    case 3:
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        func_s40_80024DB8(p, v, c);
        j = *(u16 *)(p + 0x82) + 1;
        *(s16 *)(p + 0x82) = j;
        if (j == 1)
        {
            r.x = 0x340;
            r.y = 0x154;
            r.w = 0x60;
            r.h = 0x54;
            w.vx = 0x370;
            w.vy = 0x19A;
            func_800B8FC8(*(u8 **)(o + 0x60), (s32 *)&r, (s32 *)&w, 1, j);
            ww = 0x60;
            r.x = 0x340;
            r.y = 0x1A8;
            r.w = ww;
            r.h = 0x54;
            w.vx = 0x370;
            w.vy = 0x1EF;
            func_800B8FC8(*(u8 **)(o + 0x60), (s32 *)&r, (s32 *)&w, 1, j);
        }
        if (*(s16 *)(p + 0x82) == 4)
        {
            n2 = func_8003FC64(0x212);
            if (n2 != NULL)
            {
                q = (u8 *)n2 + 0x20;
                *(s16 *)(q + 0x2) = 0x78;
                n2->unk_10 = (u32)func_s40_800246F8;
                func_8004491C(n2, (u32)D_80045340);
                s = (UnkSlot *)n2->unk_0C;
                s->unk_14 |= 0xC;
                s->unk_10 = 0x40;
                s->unk_06 = 0;
                s->unk_14 |= 0x80;
                m2 = (u8 *)n2->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(m2 + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(m2 + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(m2 + 0x8) = *(s32 *)(u + 0x8);
                s = (UnkSlot *)n2->unk_0C;
                s->unk_1C = 0x1004;
                s->unk_1E = 0x1004;
                s->unk_22 = -D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] / 2;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n2 + 0x40) = *(UnkDngS58Name12 *)D_80025E34;
                s->unk_08 = (u32)((u8 *)n2 + 0x40);
                *(s16 *)(s->unk_08 + 0x4) = GetTPage(2, 2, 0x340, 0x154);
            }
            n3 = func_8003FC64(0x212);
            if (n3 != NULL)
            {
                q = (u8 *)n3 + 0x20;
                *(s16 *)(q + 0x2) = 0x78;
                n3->unk_10 = (u32)func_s40_80024780;
                func_8004491C(n3, (u32)D_80045340);
                s = (UnkSlot *)n3->unk_0C;
                s->unk_14 &= 0xFFF3;
                s->unk_10 = 0x20;
                s->unk_14 |= 0x80;
                *(u32 *)(q + 0x2C) = (u32)o;
                *(u32 *)(q + 0x30) = *(u32 *)(o + 0x60);
                *(u32 *)(q + 0x34) = (u32)p;
                s->unk_06 = 0;
                m2 = (u8 *)n3->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(m2 + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(m2 + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(m2 + 0x8) = *(s32 *)(u + 0x8);
                s = (UnkSlot *)n3->unk_0C;
                s->unk_1E = 0x1000;
                s->unk_1C = 0x1000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n3 + 0x40) = *(UnkDngS58Name12 *)D_80025E40;
                s->unk_08 = (u32)((u8 *)n3 + 0x40);
                *(s16 *)(s->unk_08 + 0x4) = GetTPage(2, 1, 0x340, 0x1A8);
            }
            n4 = func_8003FC64(0x212);
            if (n4 != NULL)
            {
                q = (u8 *)n4 + 0x20;
                *(s16 *)(q + 0x2) = 0x78;
                n4->unk_10 = (u32)func_s40_800245EC;
                func_8004491C(n4, (u32)D_80045340);
                s = (UnkSlot *)n4->unk_0C;
                s->unk_14 &= 0xFFF3;
                s->unk_10 = 0x20;
                s->unk_06 = 0;
                s->unk_14 |= 0x80;
                m2 = (u8 *)n4->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(m2 + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(m2 + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(m2 + 0x8) = *(s32 *)(u + 0x8);
                s = (UnkSlot *)n4->unk_0C;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                s->unk_1E = 0;
                s->unk_1C = 0;
                *(UnkDngS58Name12 *)((u8 *)n4 + 0x40) = *(UnkDngS58Name12 *)D_80025E58;
                s->unk_08 = (u32)((u8 *)n4 + 0x40);
            }
            n5 = func_8003FC64(0x212);
            if (n5 != NULL)
            {
                q = (u8 *)n5 + 0x20;
                *(u32 *)(q + 0x2C) = (u32)o;
                *(u32 *)(q + 0x30) = *(u32 *)(o + 0x60);
                *(u32 *)(q + 0x34) = (u32)p;
                *(u32 *)(q + 0x38) = (u32)((u8 *)n2 + 0x20);
                *(u32 *)(q + 0x3C) = (u32)((u8 *)n4 + 0x20);
                *(u32 *)(q + 0x40) = (u32)((u8 *)n3 + 0x20);
                n5->unk_10 = (u32)func_s40_8002499C;
                func_8004491C(n5, (u32)D_80045340);
                s = (UnkSlot *)n5->unk_0C;
                s->unk_14 |= 0xC;
                s->unk_10 = 0x20;
                s->unk_06 = 0;
                s->unk_14 |= 0x80;
                m2 = (u8 *)n5->unk_08;
                *(s16 *)(q + 0xA) = 0;
                *(s16 *)(q + 0xC) = 0;
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s32 *)(m2 + 0x0) = *(s32 *)(u + 0x0);
                *(s32 *)(m2 + 0x4) = *(s32 *)(u + 0x4);
                *(s32 *)(m2 + 0x8) = *(s32 *)(u + 0x8);
                s = (UnkSlot *)n5->unk_0C;
                s->unk_1E = 0x1000;
                s->unk_1C = 0x1000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n5 + 0x40) = *(UnkDngS58Name12 *)D_s40_80025E28;
                s->unk_08 = (u32)((u8 *)n5 + 0x40);
            }
            if (*(s16 *)(p + 0x82) == 4)
            {
                *(s16 *)(p + 0xA) = 4;
                *(s16 *)(p + 0x82) = 0;
                *(s16 *)(p + 0x90) = 0;
            }
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        for (i = 0; i < 3; i++)
        {
            func_s40_800244CC((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x2020E0,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x82) >= 0x51)
        {
            c[0xC] = (0x64 - *(s16 *)(p + 0x82)) * 0x80 / 0x14;
            c[0xD] = (0x64 - *(s16 *)(p + 0x82)) * 0x80 / 0x14;
            c[0xE] = (0x64 - *(s16 *)(p + 0x82)) * 0x80 / 0x14;
        }
        func_s40_80024DB8(p, v, c);
        if (*(s16 *)(p + 0x82) >= 0x64)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x90) != 0)
        {
            func_s40_8002407C(*(u8 **)(o + 0x60), D_800E3D68 == 0xFF ? 0xFF : p[0x9], o);
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            *(u16 *)(*(u8 **)(*(u8 **)(o + 0x60) - 0x14) + 0x14) &= 0xF7FF;
            *(u32 *)(*(u8 **)(o + 0x60) + 0x14) &= 0xFFEFFFFF;
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_80025E80 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025E80 = 0;
            }
        }
        break;
    }
}
