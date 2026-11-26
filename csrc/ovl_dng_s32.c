#include "common.h"

s32 rand(void);

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
void func_s32_8002428C(u8 *p, UnkVec *v);

void func_s32_80024578(u8 *p);
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
extern s16 D_80025924;

UnkNode *func_8003FD64(s32, void *);

s32 func_800BCB04(s32, s32, s16);
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

typedef struct UnkDngS31Dirs
{
    u8 unk_00[0x20];
} UnkDngS31Dirs;
void func_s32_80024478(u8 *p, UnkVec *v, UnkSlot *c);
void func_800478B8(u8 *);

s16 func_800A45D8(u16, u16, s32);
extern u8 D_s32_80024004[];
extern u8 D_80025900[];
extern u8 D_8002590C[];
extern u8 D_80025918[];
extern u8 D_800DE870[];
extern u8 D_800E3D68;
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
s32 func_800A6D30(void);
s32 func_800C8CD8(u8 *, s16, s16);
extern u8 D_80045340[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern u8 D_800DEC00[];
void func_8003DB94(void *, void *, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
void func_s32_800248E8(u8 *p, UnkVec *v, UnkSlot *c);

s32 func_s32_8002403C(u8 *p, UnkVec *v)
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

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_80025924 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80025924
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80025924)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_80025924, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s32_8002428C(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025924 = 1;
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
   less 0x10, then park func_s32_8002428C as the step and
   func_s32_8002403C as the draw, with `b` at the payload's +0x14, `d` at
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
     `lw 0x8($s0)` moves ahead of the first `addu` on its own. */
void func_s32_80024358(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s32_8002428C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s32_8002403C);
        *(u32 *)(p + 0x8) = c;
    }
}

/* The plain mover step for this bank's 0x212 motes: integrate the three
   +0x58..+0x60 velocities into the mover, raise the bank tick, refresh the
   renderable once, lift both of its scale halves by 0x80 a frame and spend one
   of the payload's +0x2 frames; when they run out, or when the renderable's
   +0x14 carries 0x8000, raise the redraw bit in the node's -0x2 and in
   D_800814A0.  64 words, 0 differing, FIRST COMPILE.  2 members, 128 words
   (dngfam.py).  OWNS NO JUMP TABLE (jtcheck: 0 tables).

   NO TWIN WAS READ.  LEADS.txt offers func_s15_8002418C at 0.712 and
   func_s46_80024C0C at 0.638 and I opened neither; dngcallee's row for this
   function is the 2-of-3 noise tier the brief warns about (96 functions, mostly
   town).  The body is written from the listing against csrc/ovl_dng_s01.c's
   func_s01_80024280 -- the same shared library's step handler, which dngcallee
   named for func_s31_80024270 below -- and that gave the mover-add idiom, the
   D_800814A0 kill pair and the bank-tick spelling.

   THREE SPELLINGS PRICED FROM THIS BODY AT 0, since a first compile prices
   nothing:

   LOAD-BEARING:
   * THE TWO TAIL GATES ARE SEPARATE `if's WITH THE KILL PAIR WRITTEN OUT
     TWICE.  As one `if (frames <= 0 || (c->unk_14 & 0x8000) != 0)' it is 56
     words / 13 aligned differences, the eight missing words being the second
     copy.  func_s01_80024280 prices its own version of this at 13 rows too, so
     the figure reproduces across the relink.
   * THE TICK STORE IS THE FOURTH STATEMENT, after the three mover adds.  First
     in the function it is 64 words / 12 differences.

   MEASURED AND FREE, so NOT why this lands: the countdown folded into its gate,
   `if ((s16)(*(u16 *)(p + 0x2) -= 1) <= 0)', is also 64 words / 0 differing.
   Two statements kept, as in func_s01_80024280. 
   THIS COPY'S OWN CONSTANT: 0x58 -> 0x4c, 0x5c -> 0x50, 0x60 -> 0x54.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s32_80024478(u8 *p, UnkVec *v, UnkSlot *c)
{
    v->unk_00 += *(s32 *)(p + 0x4c);
    v->unk_04 += *(s32 *)(p + 0x50);
    v->unk_08 += *(s32 *)(p + 0x54);
    D_80025924 = 1;
    func_800478B8((u8 *)c);
    c->unk_1C += 0x80;
    c->unk_1E += 0x80;
    *(u16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((c->unk_14 & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_80025924
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_80025924 store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_80025924
   STORE.  Every folded spelling -- `if ((*(s16 *)(p + 0x32) -= 8) <= 0)`
   with the store first, the early-return form, the counter behind a
   `s16 *` local, a `do { } while (0)` around the store, and the
   decrement/test split into two statements -- is 20 words and 11 differing,
   and the same local written AFTER the store is 11 too.  The eleven rows are
   one allocation: with the load after the store, the constant 1 is the only
   block-local quantity alive over $v0, so local-alloc hands it $v0 and
   global-alloc then gives `p` its incoming $a0; reading the counter first
   keeps $v0/$v1 busy across the constant's range, so the constant takes $a0
   and `p` is copied to $a1 by the target's entry `addu`.  That also buys the
   schedule: `addiu $a0, $zero, 1` fills the `lhu`'s load-delay slot, where
   the store-first forms spend a `nop` there.  Hoisting the result into an
   `s32` instead loses the `sll 16`/`bgtz` pair: 19 words, 19 differing.
   THIS COPY'S OWN CONSTANT: 0x32 -> 0x2 at all 2 sites, -0x8 -> -0x1.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s32_80024578(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x2) - 1;
    D_80025924 = 1;
    *(s16 *)(p + 0x2) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The three-axis mover-with-drag step for this bank's 0x34-frame bolt: each of
   X and Y integrates its +0x4C/+0x50 velocity into the mover, advances that
   velocity by its +0x58/+0x5C acceleration and, if func_800A45D8 says the tile
   is blocked, rolls the move back and kills both; Z integrates +0x54 and
   advances it by +0x60, both horizontal velocities bleed to 96/100 a frame, and
   when the mover's height passes func_800BCB04's ceiling less 0x10 the climb
   stops, the height is clamped to ceiling - 0x11 and -- once only, gated on
   p + 0x8 -- the two horizontal velocities are re-rolled from rand().  Then one
   frame in four refreshes the renderable, both of its scale halves ease on by
   0x64 to a 0x1000 cap, and one of the payload's +0x2 frames is spent; when they
   run out, or when the renderable's +0x14 carries 0x8000, the redraw bit goes up
   in the node's -0x2 and in D_800814A0.  200 words, 0 differing.  OWNS NO JUMP
   TABLE (jtcheck: the unit's only table belongs to func_s32_80025374).
   relcand ok (2 data symbols, 7 calls).

   TWINS: dngtwin's top row func_s58_800244E4 (csrc/ovl_dng_s58.c, 0.874) gave
   the `* 96 / 100' drag and the bank-tick placement; func_b27_8017142C
   (csrc/ovl_dng_b27.c) was picked over it on the PROLOGUE -- -0x28 with
   s1,s2,ra,s4,s3,s0 in that order, byte-identical to this one, where s58 has no
   $s4 -- and gave the `(s16)func_800BCB04' cast, the `!= 0' test on the
   s16-returning func_800A45D8 and the cap read off the stored register.  The
   in-unit landed sibling func_s32_80024478 gave `D_80025924 = 1', the UnkSlot
   spelling and the D_800814A0 kill pair.

   THE THREE func_800A45D8 ARGUMENTS MUST BE ARRAY REFS, NOT PLAIN CASTS, AND
   THAT IS THE WHOLE FIGHT -- 4 aligned rows, and it is a DEPENDENCE, not an
   allocation.  `((u16 *)b)[1]', `((u16 *)b)[3]', `((s16 *)b)[5]' land at 0;
   `*(u16 *)((u8 *)b + 0x2)' and friends are 201 words / 4 rows.  An array ref
   sets MEM_IN_STRUCT_P, so true_dependence takes its fixed-scalar-versus-
   varying-struct escape and the three argument loads may hoist ABOVE the
   `D_80025924 = 1' store to that fixed-address scalar; the store then ends up
   the last insn before the `jal' and reorg steals it into the delay slot, which
   is what the target shows (`lui %hi(D_80025924)' at +0x58, `sh' in the slot).
   As plain INDIRECT_REFs the loads are pinned BELOW the store, the store is
   scheduled ahead of them and the delay slot is left a `nop'.  THE SECOND
   GATE'S THREE ARGUMENTS ARE FREE EITHER WAY -- measured, 200 words / 0 rows
   with array refs and with plain casts, the two emitting identical code --
   because there is no fixed-address store beside them for the loads to hoist
   over.  They are written as array refs here only to match the first gate; that
   choice is NOT evidence.  (The two func_800BCB04 argument triples are left as
   plain casts and were never priced.)

   THE BANK-TICK STORE IS THE THIRD STATEMENT, after `b->unk_00 += vx' and
   `vx += ax', the same slot func_s58_800244E4 uses.  Re-priced from this body at
   0: first statement is 202 words / 8 rows, second is 201 / 6.  The curve is
   monotone, and all of it is the same scheduling question as the array refs
   above -- the later the store sits, the closer it gets to the delay slot.

   `rand' is spelled `rand', not func_80069EF8 (splat/common_syms.txt has
   `rand = 0x80069ef8'); the climb re-roll is written `- 0x20000' and not
   `+ 0xFFFE0000' so the arithmetic stays signed. */
void func_s32_800245C8(u8 *p, UnkVec *b, UnkSlot *c)
{
    b->unk_00 += *(s32 *)(p + 0x4C);
    *(s32 *)(p + 0x4C) += *(s32 *)(p + 0x58);
    D_80025924 = 1;
    if (func_800A45D8(((u16 *)b)[1], ((u16 *)b)[3], ((s16 *)b)[5]) != 0)
    {
        b->unk_00 -= *(s32 *)(p + 0x4C);
        *(s32 *)(p + 0x4C) = 0;
        *(s32 *)(p + 0x58) = 0;
    }
    b->unk_04 += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x50) += *(s32 *)(p + 0x5C);
    if (func_800A45D8(((u16 *)b)[1], ((u16 *)b)[3], ((s16 *)b)[5]) != 0)
    {
        b->unk_04 -= *(s32 *)(p + 0x50);
        *(s32 *)(p + 0x50) = 0;
        *(s32 *)(p + 0x5C) = 0;
    }
    b->unk_08 += *(s32 *)(p + 0x54);
    *(s32 *)(p + 0x4C) = *(s32 *)(p + 0x4C) * 96 / 100;
    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x50) * 96 / 100;
    *(s32 *)(p + 0x54) += *(s32 *)(p + 0x60);
    if (*(s16 *)((u8 *)b + 0xA) >
        (s16)func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)((u8 *)b + 0xA) - 0x4) - 0x10)
    {
        *(s32 *)(p + 0x54) = 0;
        *(s16 *)((u8 *)b + 0xA) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)((u8 *)b + 0xA) - 0x4) - 0x11;
        *(s16 *)((u8 *)b + 0x8) = 0;
        if (*(s16 *)(p + 0x8) == 0)
        {
            *(s16 *)(p + 0x8) = 1;
            *(s32 *)(p + 0x4C) = ((rand() & 0x7F) * 0x800 - 0x20000) * 3;
            *(s32 *)(p + 0x50) = ((rand() & 0x7F) * 0x800 - 0x20000) * 3;
        }
    }
    if ((++*(u16 *)(p + 0x4) & 3) == 0)
    {
        func_800478B8((u8 *)c);
    }
    c->unk_1C += 0x64;
    if (c->unk_1C > 0x1000)
    {
        c->unk_1C = 0x1000;
    }
    c->unk_1E += 0x64;
    if (c->unk_1E > 0x1000)
    {
        c->unk_1E = 0x1000;
    }
    if ((s16)(--*(u16 *)(p + 0x2)) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if (c->unk_14 & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The summon bolt's own step, the one func_s32_80025374 below installs on each
   0x12 node it spawns.  `p' is the bolt payload, `v' its mover, `c' its
   renderable, `q = *(u8 **)(p + 0x2C)' the caster payload parked there by the
   spawner.  State 0 raises the bank tick, sheds four func_s32_80024358 puffs a
   frame at 0xC0C0C0 with a rand()-dithered alpha, and dies on the spot if
   func_800A4778 says the destination tile is blocked.  Then, with +0x8 set, it
   HOMES on the target: the climb halves while the mover is more than 0x800000
   below the target's height; each of X and Y halves its velocity inside eight
   units of the target axis and, from a standing start outside it, kicks off at
   +/-0x200000; each axis then integrates at full rate beyond 0xC0 units, at
   `velocity * distance / 192' between 4 and 0xC0, and not at all inside 4,
   where reaching the target tile advances the state; Z integrates the same way
   off whichever horizontal axis is live.  With +0x8 clear it just integrates all
   three velocities and spends one of the payload's +0x4 frames, dying when they
   run out.  State 1 rings func_800A56E0(0x300) and func_800542BC once and counts
   0xF frames; state 2 dims both of the renderable's scale halves by 0x32 a
   frame, sheds one 0x212 flare off its own mover (func_s32_800245C8 as the step,
   D_800DE870, climb `rand() + 0x30000') plus, on phase 1 of `& 3', one 0x212
   mote off the CASTER's mover (func_s32_80024478, D_800DEC00, climb
   `rand() - 0x20000'), and at +0x2 past 0x29 advances and -- when func_8009D218
   lets it -- rings func_800C8CD8 on the caster's target; state 3 dims for 0x15
   frames and then dies.  Every death raises +0x86 in the spawner's payload
   through `*(u8 **)(p + 0x34)' as well as the two redraw bits.  Whatever state
   survives, the tail sheds two 0x12 func_s32_80024578 sparks at +0x6 of -1 and
   -2, templated from D_8002590C and D_80025918, and re-dithers the renderable's
   +0x1A.  675 words, 0 differing.  OWNS NO JUMP TABLE (jtcheck: both state
   dispatches are `if' chains).  relcand ok (8 data symbols, 28 calls).

   DONOR: dngcallee's SHARP ROW -- 14 of 18 symbols, ONE function, a clean gap to
   a two-function 10-of-18 tier -- naming csrc/ovl_dng_s31.c's LANDED
   func_s31_80024270, the same shared library's summon-bolt step.  dngtwin found
   NO carved sibling at any floor for this body.  The donor gave the spawn-and-
   dress skeleton, the `(s16)(load - 0x10) + (rand() & 0x1F)' jitter, the
   `(rand() & 0x3F) + 0x20' temp in its own local, the two locals for the
   func_800C8CD8 count and the func_8009D218 gate; this copy is 331 words larger
   and everything extra is the homing block, which the donor has no trace of.

   FIRST COMPILE 673 words / 111 aligned rows.  FIVE FAULTS.  Every figure below
   is RE-PRICED from this body at 0, one spelling at a time:

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | as written below                                         |  675  |   0  |
   | the X kick gate as `if (d > 0) -K else +K'               |  675  |   3  |
   | the X state gate as `(s16)x >= (s16)y'                   |  675  |   2  |
   | X's `a < 4' arm written as the MIDDLE arm of the chain   |  675  |  17  |
   | `s->unk_14 |= 0xC' and `|= 0x2' left ADJACENT            |  673  |   7  |
   | no reference bump on `w' (see below)                     |  675  |  60  |

   THE `a < 4' ARM MUST BE LAST IN THE CHAIN, 17 rows.
   `if (a > 0xC0) ... else if (a >= 4) ... else if (p38 != p3C) ...' is 0;
   putting the `a < 4' arm in the middle is 17, because gcc emits arms in SOURCE
   order and the target's `a < 4' block sits physically after the division arm's
   closing `j'.  The dispatch is the giveaway: `slti 0x4' with `bnez' jumping
   FORWARD past the division means the small-distance arm is written last.

   THE TWO `|=' ON THE SLOT'S +0x14 MUST BE SEPARATED BY THE +0x10 STORE, 2 words
   / 7 rows.  Adjacent, gcc merges them into a single `ori 0xE' and both the
   second `ori' and its `sh' vanish.  `s->unk_14 |= 0xC; s->unk_10 = 0x60;
   s->unk_14 |= 0x2;' keeps the two raises the target has.  Note this is the
   OPPOSITE conclusion from func_s31_80024270's own comment, which records the
   +0x10 store's position as FREE -- correctly, because there its two raises are
   a call apart and nothing needed interposing.

   THE X AND Y KICK GATES ARE WRITTEN `<= 0' WITH THE POSITIVE CONSTANT FIRST,
   3 rows a site.  `if (d <= 0) vx = 0x200000; else vx = -0x200000;' lands;
   `if (d > 0) vx = -0x200000; else vx = 0x200000;' is 3.  Both arms are bare
   constant sets, so jump.c:927 hoists ONE of them above the comparison into the
   branch's delay slot -- and which one it hoists is the arm written SECOND.  The
   target hoists the negative, so the negative is the `else'.

   THE X AND Y STATE GATES COMPARE WITH `<=' AND THE OPERANDS THE OTHER WAY
   ROUND, 2 rows a pair.  `(s16)y <= (s16)x' and `(s16)x <= (s16)y' land;
   `(s16)x >= (s16)y' and `(s16)y >= (s16)x' are 2.  Both spellings emit the same
   `slt' with the same operands -- what moves is which of the two
   `sll ..., 16' insns lands in the branch's delay slot, since `A <= B' evaluates
   B's operand first and `B >= A' evaluates B's second.  This is the brief's
   comparison-operand-order lever and it presents, exactly as advertised, as a
   pair of register-only rows with nothing moved.

   AND THE ONE THAT WAS THE WHOLE FIGHT: `w' NEEDS AN EIGHTH REFERENCE, worth
   60 rows, and the cure is a `do { } while (0)' whose SCOPE is the only thing
   that costs anything.

   The residue was a clean three-way rotation: `w' in $s5, `g' in $s3, `v' in $s4
   where the target has `w' in $s3, `g' in $s4, `v' in $s5, all 60 rows
   register-only and the word count already exact.  `-da' on a single-function
   probe reads the cause straight off `.greg's `Register dispositions' line.
   global.c's allocno_compare ranks on `floor_log2 (n_refs) * n_refs /
   live_length' and allocates in that order, lowest free register first.  `w' has
   SEVEN references in the emitted code (two sets, five uses -- `grep -c "\\$s3"'
   the listing), scoring `2 * 7', which puts it last of the three; an eighth
   reference scores `3 * 8' and reverses the rank.  Confirmed, not inferred:
   without the bump the dispositions are 83 in 21 / 87 in 19 / 81 in 20, with it
   83 in 19 / 87 in 20 / 81 in 21, which is the target's assignment exactly.

   NO PLAIN-C SPELLING SUPPLIES THAT REFERENCE, because cse runs BEFORE flow
   (toplev.c puts rerun_cse_after_loop ahead of life_analysis) and collapses
   every candidate before flow can count it.  Measured, all 675 words / 60 rows,
   i.e. no change at all:
       `w = (u8 *)n; w += 0x20;' instead of one initialiser
       a pointer local, `z = w + 0x54; *(s32 *)z = rand() + 0x30000;'
   The second was aimed at the levers file's "a reference combine deletes is free
   because flow runs first"; it fails here because cse propagates the address
   into the MEM and combine never sees the add.  flow.c:2122's
   `REG_N_REFS (regno) += loop_depth' is therefore the only lever left, and a
   `do { } while (0)' is the only zero-word loop.

   THE WRAPPER'S SCOPE IS THE ENTIRE RESULT, because sched.c makes a loop note in
   the MIDDLE of a basic block a HARD barrier -- it clears reg_last_uses and sets
   reg_pending_sets_all so that nothing may cross, precisely to keep loop_depth
   and the reference counts honest.  So both notes have to land on basic-block
   boundaries.  The spawn body has no internal branch, so it is ONE block, and
   every scope was priced:

   | do { } while (0) around ...                              | words | rows |
   |----------------------------------------------------------|-------|------|
   | the whole make-node-and-install unit (kept)              |  675  |   0  |
   | `w = (u8 *)n + 0x20;' alone                              |  675  |   2  |
   | the phase-1 `w = (u8 *)n + 0x20;' alone                  |  675  |   2  |
   | `*(s32 *)(w + 0x54) = rand() + 0x30000;'                 |  675  |   4  |
   | `*(s16 *)(w + 0x2) = 0x34;'                              |  675  |   6  |
   | `*(s32 *)(w + 0x60) = 0x6000;'                           |  675  |   6  |
   | the phase-1 `*(s32 *)(w + 0x54) = rand() - 0x20000;'     |  675  |   8  |
   | the whole `if (n != NULL)' body                          |  677  |  75  |

   Every row in the middle seven is a barrier cost and nothing else: an address
   or argument the target hoists above the wrapped statement cannot get there any
   more.  The LAST row is different -- wrapping the whole block doubles EVERY
   reference in it, not just `w's, and that moves enough else to cost two words.
   The kept scope is the only one that both reaches eight references on `w' and
   puts its closing note where the block's schedule already ends, right after
   the `jal func_8004491C'.

   THAT MAKES IT MACRO RESIDUE RATHER THAN A DEVICE, and the shape is the brief's
   own prediction: ask which coherent unit of work would have been a macro and
   wrap THAT.  The unit here is "take a node, stamp its lifetime, install its
   step" -- `w = (u8 *)n + 0x20; *(s16 *)(w + 0x2) = <life>;
   n->unk_10 = (u32)<step>; func_8004491C(n, (u32)D_80045340);' -- which is
   exactly the four statements, in that order, that every spawn site in this
   block repeats.  It warns under -Wall not at all and costs no words.  The
   phase-1 spawn repeats the same four statements and does NOT need the wrapper
   (it is 0 without it), so the macro was not expanded at both sites, or the
   second expansion's bump is redundant once the first has moved the rank.

   MEASURED AND REJECTED, so none of these is why it lands:
       `w' split into one local per spawn site                675 /  176
       `w' and `g' both split per site                        674 /  217
       per-axis `x'/`y' locals instead of one shared pair     675 /   65
       the declaration order permuted (`w' first)             675 /   60
   The first two add a short-range allocno that conflicts with `w' and takes $s3
   off it; the last says the rank is a strict inequality and not a tie, so the
   allocno-number tie-break in allocno_compare is not involved.

   TWO READING NOTES.  The `lhu' pair at +0x2/+0x6 and the `lh' pair at the same
   addresses are BOTH real: the `lh's feed the subtraction and the `lhu's are the
   `x'/`y' locals the state gate compares, and cse cannot unify loads of
   different modes, so the source really does read each address twice.  And
   `rand' is spelled `rand', not func_80069EF8.  This body contains NO AGGREGATE
   INITIALISER -- D_8002590C and D_80025918 are extern and the 0xC-byte copies
   are struct assignments from them. */
void func_s32_800248E8(u8 *p, UnkVec *v, UnkSlot *c)
{
    u8 *w;
    UnkNode *n;
    u8 *m;
    UnkSlot *s;
    u8 *g;
    u8 *q;
    u16 x;
    u16 y;
    s32 i;
    s32 a;
    s32 t;
    s32 t3;
    s32 t4;

    D_80025924 = 1;
    q = *(u8 **)(p + 0x2C);
    if (*(s16 *)(p + 0x0) == 0)
    {
        for (i = 0; i < 4; i++)
        {
            func_s32_80024358((UnkNode *)(p - 0x20), *(s16 *)(p + 0x16), 0xC0C0C0, (rand() & 0xFF) | 0x80, 0,
                              0, 0);
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6), *(s16 *)((u8 *)v + 0xA),
                               *(s32 *)(p + 0x30)) != 0)
        {
            *(s16 *)(*(u8 **)(p + 0x34) + 0x86) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        if (*(s16 *)(p + 0x8) != 0)
        {
            g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
            if (v->unk_08 < *(s32 *)(g + 0x8) - 0x800000)
            {
                *(s32 *)(p + 0x54) = *(s32 *)(p + 0x54) / 2;
            }
            if (__builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)(g + 0x2)) < 0x8)
            {
                if (__builtin_abs(*(s32 *)(p + 0x4C)) > 0x40000)
                {
                    *(s32 *)(p + 0x4C) = *(s32 *)(p + 0x4C) / 2;
                }
            }
            else if (*(s32 *)(p + 0x4C) == 0)
            {
                if (*(s16 *)((u8 *)v + 0x2) - *(s16 *)(g + 0x2) <= 0)
                {
                    *(s32 *)(p + 0x4C) = 0x200000;
                }
                else
                {
                    *(s32 *)(p + 0x4C) = -0x200000;
                }
            }
            if (__builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)(g + 0x6)) < 0x8)
            {
                if (__builtin_abs(*(s32 *)(p + 0x50)) > 0x40000)
                {
                    *(s32 *)(p + 0x50) = *(s32 *)(p + 0x50) / 2;
                }
            }
            else if (*(s32 *)(p + 0x50) == 0)
            {
                if (*(s16 *)((u8 *)v + 0x6) - *(s16 *)(g + 0x6) <= 0)
                {
                    *(s32 *)(p + 0x50) = 0x200000;
                }
                else
                {
                    *(s32 *)(p + 0x50) = -0x200000;
                }
            }
            x = *(u16 *)((u8 *)v + 0x2);
            y = *(u16 *)(g + 0x2);
            a = __builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)(g + 0x2));
            if (a > 0xC0)
            {
                v->unk_00 += *(s32 *)(p + 0x4C);
            }
            else if (a >= 0x4)
            {
                v->unk_00 += *(s32 *)(p + 0x4C) * a / 192;
            }
            else if (*(s16 *)(p + 0x38) != *(s16 *)(p + 0x3C))
            {
                *(s16 *)(p + 0x0) = 1;
                *(s16 *)(p + 0x2) = 0;
            }
            if (*(s16 *)(p + 0x38) != *(s16 *)(p + 0x3C))
            {
                if (*(s32 *)(p + 0x4C) > 0)
                {
                    if ((s16)y <= (s16)x)
                    {
                        *(s16 *)(p + 0x0) = 1;
                        *(s16 *)(p + 0x2) = 0;
                    }
                }
                else if (*(s32 *)(p + 0x4C) < 0)
                {
                    if ((s16)x <= (s16)y)
                    {
                        *(s16 *)(p + 0x0) = 1;
                        *(s16 *)(p + 0x2) = 0;
                    }
                }
            }
            x = *(u16 *)((u8 *)v + 0x6);
            y = *(u16 *)(g + 0x6);
            a = __builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)(g + 0x6));
            if (a > 0xC0)
            {
                v->unk_04 += *(s32 *)(p + 0x50);
            }
            else if (a >= 0x4)
            {
                v->unk_04 += *(s32 *)(p + 0x50) * a / 192;
            }
            else if (*(s16 *)(p + 0x3A) != *(s16 *)(p + 0x3E))
            {
                *(s16 *)(p + 0x0) = 1;
                *(s16 *)(p + 0x2) = 0;
            }
            if (*(s16 *)(p + 0x3A) != *(s16 *)(p + 0x3E))
            {
                if (*(s32 *)(p + 0x50) > 0)
                {
                    if ((s16)y <= (s16)x)
                    {
                        *(s16 *)(p + 0x0) = 1;
                        *(s16 *)(p + 0x2) = 0;
                    }
                }
                else if (*(s32 *)(p + 0x50) < 0)
                {
                    if ((s16)x <= (s16)y)
                    {
                        *(s16 *)(p + 0x0) = 1;
                        *(s16 *)(p + 0x2) = 0;
                    }
                }
            }
            if (*(s32 *)(p + 0x4C) != 0)
            {
                x = *(u16 *)((u8 *)v + 0x2);
                y = *(u16 *)(g + 0x2);
            }
            else
            {
                x = *(u16 *)((u8 *)v + 0x6);
                y = *(u16 *)(g + 0x6);
            }
            a = __builtin_abs((s16)x - (s16)y);
            if (a > 0xC0)
            {
                v->unk_08 += *(s32 *)(p + 0x54);
            }
            else if (a >= 0x4)
            {
                v->unk_08 += *(s32 *)(p + 0x54) * a / 192;
            }
        }
        else
        {
            *(u16 *)(p + 0x4) -= 1;
            v->unk_00 += *(s32 *)(p + 0x4C);
            v->unk_04 += *(s32 *)(p + 0x50);
            v->unk_08 += *(s32 *)(p + 0x54);
            if (*(s16 *)(p + 0x4) <= 0)
            {
                *(s16 *)(*(u8 **)(p + 0x34) + 0x86) = 1;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
                return;
            }
        }
    }
    if (*(s16 *)(p + 0x0) == 1)
    {
        if (*(s16 *)(p + 0x2) == 0)
        {
            func_800A56E0(0x300);
            func_800542BC();
        }
        *(u16 *)(p + 0x2) += 1;
        if (*(s16 *)(p + 0x2) >= 0xF)
        {
            *(s16 *)(p + 0x2) = 0;
            *(u16 *)(p + 0x0) += 1;
        }
    }
    if (*(s16 *)(p + 0x0) == 2)
    {
        *(u16 *)(p + 0x2) += 1;
        c->unk_1C -= 0x32;
        c->unk_1E -= 0x32;
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            /* DEVICE: this do { } while (0) is a reference bump, worth 60
               aligned rows -- flow.c:2122 counts `w' once per enclosing loop,
               and the eighth reference is what carries it past `g' and `v' in
               allocno_compare so the three take $s3/$s4/$s5 in the target's
               order. Read as MACRO RESIDUE rather than an invention: the four
               wrapped statements are the make-node-and-install-step unit every
               spawn site in this block repeats, and that scope is the only one
               of eight that costs nothing. Without it the body is 675 words /
               60 rows; the seven rejected scopes and the two plain-C spellings
               that cse eats are priced in the header comment. Costs no words
               and warns nothing under -Wall. */
            do
            {
                w = (u8 *)n + 0x20;
                *(s16 *)(w + 0x2) = 0x34;
                n->unk_10 = (u32)func_s32_800245C8;
                func_8004491C(n, (u32)D_80045340);
            } while (0);
            s = (UnkSlot *)n->unk_0C;
            s->unk_14 |= 0xC;
            s->unk_10 = 0x60;
            s->unk_14 |= 0x2;
            m = (u8 *)n->unk_08;
            *(s16 *)(m + 0x2) = (s16)(*(u16 *)((u8 *)v + 0x2) - 0x10) + (rand() & 0x1F);
            *(s16 *)(m + 0x6) = (s16)(*(u16 *)((u8 *)v + 0x6) - 0x10) + (rand() & 0x1F);
            *(s16 *)(m + 0xA) = *(u16 *)((u8 *)v + 0xA);
            *(s32 *)(w + 0x54) = rand() + 0x30000;
            *(s32 *)(w + 0x60) = 0x6000;
            s = (UnkSlot *)n->unk_0C;
            s->unk_1C = 0x800;
            s->unk_1E = 0x800;
            s->unk_0C[2] = 0x80;
            s->unk_0C[1] = 0x80;
            s->unk_0C[0] = 0x80;
            s->unk_12 = 0x7DCF;
            s->unk_14 |= 0x100;
            func_8003DB94(s, D_800DE870, 0);
        }
        if ((*(u16 *)(p + 0x2) & 0x3) == 1)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                w = (u8 *)n + 0x20;
                *(s16 *)(w + 0x2) = 0x14;
                n->unk_10 = (u32)func_s32_80024478;
                func_8004491C(n, (u32)D_80045340);
                s = (UnkSlot *)n->unk_0C;
                s->unk_10 = 0;
                s->unk_14 |= 0xC;
                m = (u8 *)n->unk_08;
                g = *(u8 **)(*(s32 *)(p + 0x30) - 0x18);
                *(s16 *)(m + 0x2) = (s16)(*(u16 *)(g + 0x2) - 0x10) + (rand() & 0x1F);
                *(s16 *)(m + 0x6) = (s16)(*(u16 *)(g + 0x6) - 0x10) + (rand() & 0x1F);
                t = (rand() & 0x3F) + 0x20;
                *(s16 *)(m + 0xA) = *(u16 *)(g + 0xA) - t;
                s = (UnkSlot *)n->unk_0C;
                s->unk_1C = 0x800;
                s->unk_1E = 0x800;
                *(s32 *)(w + 0x54) = rand() - 0x20000;
                s->unk_0C[2] = 0x80;
                s->unk_0C[1] = 0x80;
                s->unk_0C[0] = 0x80;
                s->unk_12 = 0x7DC0;
                s->unk_14 |= 0x100;
                func_8003DB94(s, D_800DEC00, 0);
            }
        }
        if (*(s16 *)(p + 0x2) >= 0x29)
        {
            *(s16 *)(p + 0x2) = 0;
            *(u16 *)(p + 0x0) += 1;
            if (func_8009D218(*(u8 **)(q + 0x60), 2, q) == 0)
            {
                t3 = (func_800A6D30() & 0x3) + 2;
                t4 = (p[0x15] >> 2) + t3;
                func_800C8CD8(*(u8 **)(p + 0x30), D_800E3D68 == 0xFF ? 0xFF : 0x10, t4);
            }
        }
    }
    if (*(s16 *)(p + 0x0) == 3)
    {
        *(u16 *)(p + 0x2) += 1;
        c->unk_1C -= 0x32;
        c->unk_1E -= 0x32;
        if (*(s16 *)(p + 0x2) >= 0x15)
        {
            *(s16 *)(*(u8 **)(p + 0x34) + 0x86) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        *(s16 *)((u8 *)n + 0x22) = 1;
        n->unk_10 = (u32)func_s32_80024578;
        func_8004491C(n, (u32)D_80045340);
        s = (UnkSlot *)n->unk_0C;
        s->unk_10 = 0x60;
        s->unk_14 |= 0xC;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)v + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)v + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)v + 0xA);
        s = (UnkSlot *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        s->unk_06 = -1;
        s->unk_0C[0] = c->unk_0C[0];
        s->unk_0C[1] = c->unk_0C[1];
        s->unk_0C[2] = c->unk_0C[2];
        s->unk_1A = rand() & 0xFFF;
        *(UnkDngS58Name12 *)((u8 *)n + 0x40) = *(UnkDngS58Name12 *)D_8002590C;
        s->unk_08 = (u32)((u8 *)n + 0x40);
    }
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        *(s16 *)((u8 *)n + 0x22) = 1;
        n->unk_10 = (u32)func_s32_80024578;
        func_8004491C(n, (u32)D_80045340);
        s = (UnkSlot *)n->unk_0C;
        s->unk_10 = 0x60;
        s->unk_14 |= 0xC;
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)((u8 *)v + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)v + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)v + 0xA);
        s = (UnkSlot *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        s->unk_06 = -2;
        s->unk_0C[0] = c->unk_0C[0];
        s->unk_0C[1] = c->unk_0C[1];
        s->unk_0C[2] = c->unk_0C[2];
        s->unk_1A = rand() & 0xFFF;
        *(UnkDngS58Name12 *)((u8 *)n + 0x40) = *(UnkDngS58Name12 *)D_80025918;
        s->unk_08 = (u32)((u8 *)n + 0x40);
    }
    c->unk_1A = rand() & 0xFFF;
}

/* States 0..4 of this bank's summon beam, and the function that spawns
   func_s32_800248E8 above.  `p' is this node's payload, `v' its mover, `c' its
   renderable; `o = *(u8 **)p' is the CASTER payload, `g = (UnkNode *)(o - 0x20)'
   the caster node and `m = g->unk_08' the caster mover.  0 dresses white, binds
   D_800DEC00, raises the bank tick and latches the caster's facing out of its
   +0x2A; 1 publishes the caster's hand through func_8003DF74 and, on the view's
   +0x80, advances; 2 spawns ONE 0x12 bolt running func_s32_800248E8 -- its
   velocity pair read out of the D_s32_80024004 direction table at that facing,
   its tile distance twice the caster-to-target axis delta, its climb divided
   over that distance, the 0xC-byte D_80025900 template parked at w + 0x20 as the
   primitive -- parks the node and its mover at +0xA8 and +0xAC and advances;
   3 follows that bolt until its +0x1E carries 0x8000; 4 counts 0xB frames and
   then kills, one frame late, through the bank tick.  355 words, 0 differing.
   relcand ok (9 data symbols, 4 calls).

   OWNS jtbl_s32_80024028: 5 words = 5 CASES + 0 pad, dispatch bound 5, five
   distinct targets (jtcheck agrees with the label count).  The arms are written
   ASCENDING by case value, which is also ENTRY-ADDRESS order
   (.Ls32_8002545C < .Ls32_800254B0 < .Ls32_8002557C < .Ls32_800257FC <
   .Ls32_80025878).  Since ovlverify and aligndiff are both blind to arm order,
   that was settled by a standalone `tools/cdkgcc -S' of this unit: the emitted
   .rdata table is `.word $L36 / $L37 / $L43 / $L50 / $L55' and those five labels
   occur in the text in that same ascending order, so the emitted arm order
   matches the listing.  Case 0 FALLS THROUGH into case 1.

   DONORS: func_s31_800247D0 (csrc/ovl_dng_s31.c, dngtwin 0.797) for the state
   skeleton, the `||' gate on func_8003DF74, the else arm's `- 0x40' and the
   `D_8006CCD8[facing]'/`D_8006CCE8[facing]' corner pair; func_s33_80024668
   (csrc/ovl_dng_s33.c) for the spawn-and-dress idiom, the `tbl[facing].a/.b'
   records and the `r' re-read.  The FOUR UNUSED TYPEDEFS ALREADY SITTING IN THIS
   UNIT named both block moves before any compile -- `UnkDngS31Dirs' is the
   0x20-byte lwl/lwr copy of D_s32_80024004, `UnkDngS58Blk32'/`UnkDngS58Rec' the
   8 x 4-byte direction records it is read back as, `UnkDngS58Name12' the 0xC-byte
   D_80025900 copy.  That is the brief's "an unused typedef in your unit's
   declaration block can name your twin", and here it was worth three spellings.

   FIRST COMPILE 359 words / 42 aligned rows.  THREE FAULTS, each re-priced
   from this body at 0:

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | as written below                                         |  355  |   0  |
   | `*(u8 **)(w + 0x2C) = o' BETWEEN the two tbl stores      |  356  |   5  |
   | `*(s16 *)(w + 0x8) = 1' between +0x3C and +0x3E          |  355  |   8  |
   | the slot pair inline as `((UnkSlot *)n->unk_0C)->...'    |  358  |  31  |
   | the abs as ONE statement with the ternary inside         |  356  |   8  |

   THE SLOT POINTER NEEDS THE LOCAL `k' AND IT MUST BE RE-READ, 3 words / 31 rows.
   Written inline at the +0x10 and +0x14 sites gcc emits TWO `lw 0xC($s3)' plus
   two load-delay `nop's where the target has one load serving both; written as a
   single `k' carried across the three `h' stores it loses the second `lw' the
   target does have, because those stores invalidate cse's entry for n->unk_0C.
   So: one local, assigned twice, once per dress block.

   THE ABS GOES IN TWO `if'/`else' ARMS, EACH WITH ITS OWN __builtin_abs AND ITS
   OWN STORE -- and that is how this body gets the `lh $a0, 0x2($s0)' RELOAD for
   free where func_s31_800247D0 needed a `volatile' DEVICE for the same reload
   (priced 9 rows in its own comment).  As one statement with the ternary inside
   __builtin_abs -- s31's spelling -- it is 356 words / 8 rows: the store's value
   then lives in a single pseudo at the join, cse records it and forwards it to
   the reload, sign-extending with `sll 16'/`sra 16'.  With two arms the value is
   in a different pseudo per arm, cse has nothing to forward across the join, and
   the reload survives with no device at all.  NO DEVICE IN THIS BODY.

   D_s32_80024004 is the eight (x, y) direction pairs splat emits as
   asm/data/18EE800.data.s; it and D_80025900 are declared extern here and never
   defined in C, so this body contains NO AGGREGATE INITIALISER. */
void func_s32_80025374(u8 *p, UnkVec *v, UnkSlot *c)
{
    u16 out[3];
    UnkDngS58Rec tbl[8];
    UnkNode *g;
    UnkNode *n;
    u8 *o;
    u8 *m;
    u8 *w;
    u8 *e;
    u8 *h;
    u8 *hh;
    UnkSlot *k;
    s32 j;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s32_80024004;
    g = (UnkNode *)(o - 0x20);
    m = (u8 *)g->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)c->unk_0C = 0x808080;
        c->unk_1E = 0x1000;
        c->unk_1C = 0x1000;
        func_8003DB94(c, D_800DEC00, 0);
        j = (*(u16 *)(o + 0x2A) >> 9) & 0x7;
        D_80025924 = 1;
        *(s16 *)(p + 0x7E) = j;
        *(u16 *)(p + 0xA) += 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)g->unk_0C)->unk_08, (UnkSlot *)g->unk_0C, (s16 *)out, 0) != NULL ||
            (((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) != 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
            if ((((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
            }
            else
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) - 0x40;
            }
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                *(s16 *)(p + 0x84) = 0;
                *(u16 *)(p + 0xA) += 1;
            }
        }
        break;
    case 2:
        n = func_8003FC64(0x12);
        if (n != NULL)
        {
            w = (u8 *)n + 0x20;
            *(s32 *)(w + 0x4C) = tbl[*(s16 *)(p + 0x7E)].a << 16;
            *(s32 *)(w + 0x50) = tbl[*(s16 *)(p + 0x7E)].b << 16;
            *(u8 **)(w + 0x2C) = o;
            *(s32 *)(w + 0x30) = *(s32 *)(o + 0x60);
            *(u8 **)(w + 0x34) = p;
            w[0x15] = p[0x9];
            *(s16 *)(w + 0x16) = *(u16 *)(p + 0x7E);
            e = *(u8 **)(o - 0x14);
            p[0xA0] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA1] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            if (*(s32 *)(o + 0x60) != 0)
            {
                *(s16 *)(w + 0x38) = *(s8 *)(o + 0x72);
                *(s16 *)(w + 0x3A) = *(s8 *)(o + 0x73);
                *(s16 *)(w + 0x3C) = e[0x24];
                *(s16 *)(w + 0x3E) = e[0x25];
                *(s16 *)(w + 0x8) = 1;
                if (*(s8 *)(o + 0x72) == e[0x24])
                {
                    *(s16 *)(w + 0x2) = __builtin_abs(*(s8 *)(o + 0x73) - e[0x25]) * 2;
                }
                else
                {
                    *(s16 *)(w + 0x2) = __builtin_abs(*(s8 *)(o + 0x72) - e[0x24]) * 2;
                }
                hh = *(u8 **)(*(s32 *)(o + 0x60) - 0x18);
                if (*(s16 *)(w + 0x2) != 0)
                {
                    *(s32 *)(w + 0x54) =
                        (*(s32 *)(hh + 0x8) - v->unk_08 - 0x800000) / (*(s16 *)(w + 0x2) - 1);
                }
            }
            else
            {
                *(s16 *)(w + 0x2) = 0x20;
                *(s16 *)(w + 0x4) = 0x20;
                *(s16 *)(w + 0x8) = 0;
            }
            *(s16 *)(p + 0x86) = 0;
            n->unk_10 = (u32)func_s32_800248E8;
            func_8004491C(n, (u32)D_80045340);
            k = (UnkSlot *)n->unk_0C;
            k->unk_10 = 0;
            k->unk_14 |= 0xC;
            h = (u8 *)n->unk_08;
            *(s32 *)(h + 0x0) = v->unk_00;
            *(s32 *)(h + 0x4) = v->unk_04;
            *(s32 *)(h + 0x8) = v->unk_08;
            k = (UnkSlot *)n->unk_0C;
            k->unk_0C[2] = 0x80;
            k->unk_0C[1] = 0x80;
            k->unk_0C[0] = 0x80;
            k->unk_1E = 0x1000;
            k->unk_1C = 0x1000;
            *(UnkDngS58Name12 *)(w + 0x20) = *(UnkDngS58Name12 *)D_80025900;
            k->unk_08 = (u32)(w + 0x20);
            *(u32 *)(p + 0xA8) = (u32)n;
            *(u32 *)(p + 0xAC) = (u32)h;
            *(s16 *)(p + 0x88) = 0x63;
        }
        *(u16 *)(p + 0xA) += 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0xA8);
            h = *(u8 **)(p + 0xAC);
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = *(s32 *)(h + 0x0);
                v->unk_04 = *(s32 *)(h + 0x4);
                v->unk_08 = *(s32 *)(h + 0x8);
            }
        }
        if (*(s16 *)(p + 0x86) != 0)
        {
            *(s16 *)(p + 0x84) = 0;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 4:
        ((u16 *)p)[0x42] += 1;
        if (*(s16 *)(p + 0x84) > 0xA)
        {
            ((u16 *)p)[0x42] -= 1;
            if (D_80025924 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025924 = 0;
            }
        }
        break;
    }
}
