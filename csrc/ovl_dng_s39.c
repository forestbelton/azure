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
void func_s39_8002515C(u8 *p, UnkVec *v);

void func_s39_80025348(u8 *p);
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
extern s16 D_800266BC;

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

typedef struct UnkDngS39Blk8
{
    u8 b[0x8];
} UnkDngS39Blk8;
extern UnkDngS39Blk8 D_s39_80024004;
extern UnkDngS39Blk8 D_s39_8002400C;
void func_s39_8002405C(u8 *a, u8 *pp, u8 *pp2, u8 *ot, s16 d);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s39", func_s39_8002405C);

/* The bank's draw bracket for this object, called by func_s39_80025398 and
   func_s39_80025648: SetDrawArea to the caller's RECT, one call of this
   object's renderer func_s39_8002405C at the position in the second RECT,
   and -- when the fifth argument's low half is set -- a 0x60000000 tint prim
   dressed from the first RECT.  130 words, 0 differing.  The twins are
   csrc/ovl_dng_s41.c's func_s41_80024B6C and csrc/ovl_dng_s37.c's
   func_s37_80024A5C, the same 126-word body at 0.922; this bank's renderer
   takes FIVE arguments where theirs take four, so there is an extra payload
   pointer parameter here and the trailing count is passed on the stack.

   THE `s32 ee = e;' COPY IS THE WHOLE OF THE +4 WORDS.  Passing `(s16)e'
   straight off the parameter is 127 words and 22 aligned differences:
   assign_parms hangs a REG_EQUIV on an SImode stack parameter whose
   nominal_mode equals its passed_mode, update_equiv_regs deletes the
   prologue load and the value is rematerialised with its own `lw' at the
   call, so there is no long-lived pseudo to compete for $s7 at all.  The
   user copy gives it one -- which is the target's `lw $s7, 0x64($sp)' at the
   top with the `sll 16'/`sra 16' left at the use -- and that makes TEN
   long-lived values for nine callee-saved registers, so `cw' is the one that
   loses and spills, which is the target's `sh $t0, 0x20($sp)' /
   `lhu $t0, 0x20($sp)' pair.

   THAT IS BOTH TWINS' `s16'-PARAMETER FACT INVERTED, and the reason is
   positional: their count is the LAST argument, mine is the SIXTH, and the
   `lw' plus `sll 16'/`sra 16' at the use says `s32' outright where an `s16'
   parameter would be a bare `lh'.

   Both twins' own facts re-measured here and both HOLD.  `u16 cw = (u16)c;'
   must be hoisted out of the `if': `if ((u16)c != 0)' with no local is 128
   words and 35 aligned differences.  The 0x8D0 store must be the array ref
   `((u32 *)X)[0x234]': as `*(u32 *)(X + 0x8D0)' it is 130 words and 29
   differing.  And their `do { } while (0)' around the renderer call is
   REMOVED here exactly as their comments say: added back it is 130 words and
   16 aligned differences.

   0x80067E2C is `SetDrawArea' in splat/common_syms.txt, so the two calls the
   listing spells `func_80067E2C' need no declaration.  Those two calls warn
   `incompatible pointer type' under declcheck, identically to the committed
   s37 and s41 twins; explicit `(DR_AREA *)' / `(RECT *)' casts are FREE
   (130 words, 0 differing) if a clean run is wanted.
   raw 10 == relocdump's 10: 1 HI16 + 4 LO16 on D_80083160, 1 + 1 on
   D_801C9E40, 2 R_MIPS_26 on SetDrawArea and 1 on func_s39_8002405C. */
void func_s39_80024CD4(u8 *p, u8 *pp, s32 *b, s32 *b2, s32 c, s32 e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    u16 cw;
    s32 ee;

    ee = e;
    cw = (u16)c;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    ((u32 *)D_80083160.unk_00)[0x234] = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    addPrim(r, q);
    w[0] = *(u16 *)b2;
    t = *((s16 *)b2 + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_s39_8002405C((u8 *)w, p, pp, r, (s16)ee);
    if (cw != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t = *((s16 *)b + 1);
        if (lean)
        {
            t -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t;
        *(u32 *)(u + 0xC) = b[1];
        addPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    addPrim(r, q);
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
s32 func_s39_80024EDC(u8 *p, UnkVec *v)
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

/* A random sign: rand() & 1, with the zero case turned into -1. 12 words,
   0 differing. ONE LOCAL CARRIES BOTH VALUES, not two `return's: the target
   reuses the `andi' result on the taken path and only materialises
   `addiu $v0, $zero, -0x1' on the fall-through. As
   `if ((rand() & 1) == 0) return -1; return 1;' it is 12 words and 4
   differing -- the taken path gets its own `li 1'. 0x80069EF8 is `rand' in
   splat/common_syms.txt, so the call needs no declaration.
   raw 1 == relocdump's 1, the R_MIPS_26 on rand. */
s32 func_s39_8002512C(void)
{
    s32 r;

    r = rand() & 1;
    if (r == 0)
    {
        r = -1;
    }
    return r;
}

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_800266BC to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_800266BC
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_800266BC)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_800266BC, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s39_8002515C(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_800266BC = 1;
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
   less 0x10, then park func_s39_8002515C as the step and
   func_s39_80024EDC as the draw, with `b` at the payload's +0x14, `d` at
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
void func_s39_80025228(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s39_8002515C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s39_80024EDC);
        *(u32 *)(p + 0x8) = c;
    }
}

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_800266BC
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_800266BC store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_800266BC
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
void func_s39_80025348(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x2) - 1;
    D_800266BC = 1;
    *(s16 *)(p + 0x2) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The state 0/1 frame of the banked actor drawn by func_s39_80024CD4: copy
   this bank's 8-byte draw RECT D_s39_80024004 to the stack, pin D_800266BC,
   fade the slot's greys as `n * 128 / limit' and push its scale pair out by
   0x50, wrap the +0x98 heading by 0x1000 after taking 0x28 off it, refill the
   0x60-byte strip at +0x38 with 0x46 while the counter is still under 0x32,
   and on state 0 or 1 draw through func_s39_80024CD4 at the RECT's own centre
   and reap the node when +0x4 reaches +0x2.  125 words, 0 differing,
   0 aligned differences.

   NO TWIN AT ANY RATIO; the donors are the two landed in-unit siblings
   func_s39_8002515C and func_s39_8002558C, which dngcallee named off the
   D_800266BC / D_800814A0 callee set, plus csrc/ovl_dng_b01.c's
   func_b01_801708E8 for the grey chain.  func_s39_80024CD4 landing an hour
   earlier in this same file is what fixed the six-argument call.

   THE 0x60-BYTE FILL COUNTS UP.  `for (i = 0; i < 0x60; i++)' is the target;
   written DESCENDING -- which is what the emitted loop looks like, a counter
   from 0x5F and a pointer walked down -- it is 124 words and 4 aligned
   differences.  loop.c reverses the ascending loop in `check_dbra_loop', and
   a reversed biv is the one case where loop.c:3996 reduces an address giv
   whose benefit is 0 anyway, which is where the target's separate
   `addiu $v0, $v0, -0x1' pointer and its `addiu $v0, $s0, 0x5F' preheader
   init come from.  Written descending the giv is never reduced and the
   address is an `addu' recomputed inside the loop.  The init ORDER is the
   tell: the 0x46 movable lands before the giv's init and after the source
   counter's.

   `w = *(u16 *)p;' IS HOISTED ABOVE THE SWITCH AND THE `+ 1' STAYS IN THE
   ARM.  `w = *(u16 *)p + 1;' with `*(s16 *)p = w;' is 126 words and 3 aligned
   differences and `(*(u16 *)p)++;' left inside case 0 is 126 and 7.  With the
   add in the arm the pre-dispatch block is one insn shorter and sched1 puts
   the `lh' that feeds the `beqz' first instead of last; both rejected forms
   spend the saved word on a `nop' in the load-delay slot before the branch.

   A `switch', NOT `if' / `else if': the chain is 124 words and 10 aligned
   differences, because the switch lays case 0's block out AFTER the `j' to
   the default arm and the `if' makes it the fall-through.

   TWO LOCALS FOR THE TWO ARMS' QUOTIENTS.  One shared local is 116 words and
   28 aligned differences: the arms become identical and jump2 cross-joins the
   0x1C/0x1E bumps and the grey triple into one copy, where the target has
   them twice.

   The two loads before the `D_800266BC = 1' store: store first is 126 words
   and 5 aligned differences, the same fact and the same price as
   func_s39_8002558C's.

   MEASURED AND FREE, so it is NOT the reason this lands: `a * 128 / b' in
   place of `(a << 7) / b' (125 words, 0 differing -- the power-of-two
   multiply is the same tree, reproducing b01's finding).

   THE RECT COPY IS A BYTE-ALIGNED STRUCT.  `UnkDngS39Blk8 area =
   D_s39_80024004;' is what gives the target's `lwl'/`lwr' + `swl'/`swr'
   unaligned word moves out of a symbol that is in fact 4-aligned; the
   declared alignment, not the address, is what picks them.
   raw 10 == relocdump's 10: 2 on D_s39_80024004, 2 on D_800266BC, 3 on
   D_800814A0, 1 R_MIPS_26 on func_s39_80024CD4 and 2 on .text for the two
   `j's.  THE SYMBOL IS D_s39_80024004 AND NOT D_s39_8002400C -- the sibling
   twelve listings down loads the other one, both are 8-byte RECTs four bytes
   apart, and the relocation COUNT is identical, so neither ovlverify nor
   --raw can tell them apart. */
void func_s39_80025398(u8 *p, UnkVec *v, u8 *q)
{
    UnkDngS39Blk8 area;
    s16 pos[2];
    s32 a;
    s32 b;
    s32 c;
    s32 t;
    s32 u;
    s32 h;
    s32 st;
    s32 w;
    s32 i;

    area = D_s39_80024004;
    a = *(s16 *)(p + 0x4);
    b = *(s16 *)(p + 0xA);
    D_800266BC = 1;
    if (a < b)
    {
        t = (a << 7) / b;
        *(s16 *)(q + 0x1C) = *(u16 *)(q + 0x1C) + 0x50;
        *(s16 *)(q + 0x1E) = *(u16 *)(q + 0x1E) + 0x50;
        q[0xC] = q[0xD] = q[0xE] = t;
    }
    else
    {
        c = *(s16 *)(p + 0x2);
        u = ((c - a) << 7) / (c - b);
        *(s16 *)(q + 0x1C) = *(u16 *)(q + 0x1C) + 0x50;
        *(s16 *)(q + 0x1E) = *(u16 *)(q + 0x1E) + 0x50;
        q[0xC] = q[0xD] = q[0xE] = u;
    }
    h = *(u16 *)(p + 0x98);
    *(s16 *)(p + 0x98) = h - 0x28;
    if ((s16)(h - 0x28) < 0)
    {
        *(s16 *)(p + 0x98) = h - 0x28 + 0x1000;
    }
    if (*(s16 *)(p + 0x4) < 0x32)
    {
        for (i = 0; i < 0x60; i++)
        {
            p[i + 0x38] = 0x46;
        }
    }
    st = *(s16 *)p;
    *(s16 *)(p + 0x9A) = 0x64;
    w = *(u16 *)p;
    switch (st)
    {
    case 0:
        *(s16 *)p = w + 1;
        *(u16 *)(q + 0x14) &= 0xFF7F;
        break;
    case 1:
        break;
    default:
        return;
    }
    pos[0] = *(u16 *)&area.b[0] + ((s16)(*(u16 *)&area.b[4]) >> 1);
    pos[1] = *(u16 *)&area.b[2] + 0x3C;
    func_s39_80024CD4(p, *(u8 **)(p + 0x34), (s32 *)&area, (s32 *)pos, 1, 1);
    if ((s16)(++*(u16 *)(p + 0x4)) == *(s16 *)(p + 0x2))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* One frame of the simple grey fade: pin D_800266BC, set the slot's three
   greys from the +0x4 frame counter -- rising as `n * 96 / limit' while it is
   under the +0xA threshold and falling as the complement against +0x2 after
   it -- then bump the counter and raise the 0x8000 kill bit in the node's
   -0x2 and in D_800814A0 when it reaches the +0x2 limit.  47 words,
   0 differing, 0 aligned differences.

   NO TWIN AT ANY RATIO.  Built from two donors, neither of them a shape
   match: the in-unit landed sibling func_s39_8002515C for the spellings, and
   csrc/ovl_dng_b01.c's func_b01_801708E8 for the grey chain.

   `s32 a/b/c', NOT `s16': as `s16' it is 51 words and 22 aligned
   differences, because an HImode local is loaded `lhu' and then sign-extended
   `sll 16'/`sra 16' at every int use, where the target's `lh' loads straight
   into an SImode pseudo.  func_s39_8002515C declares the same three loads
   `s32' and that is where this came from.

   THE TWO LOADS GO BEFORE THE `D_800266BC = 1' STORE, which is
   func_s39_8002515C's documented fact and HOLDS here: the store first is 48
   words and 5 aligned differences (6 on the sibling, same mechanism -- gcc
   cannot disambiguate the `mem:HI (lo_sum D_800266BC)' store from
   `mem:HI ($a3 + 4)').

   THE CHAIN GOES INSIDE EACH ARM WITH THE QUOTIENT INLINE, not into one
   local with the chain after the `if': one local `t' is 47 words and 6
   aligned differences, every row $v1 where the target has $v0, because a
   local set in both arms is not a single-set pseudo and loses
   adjust_priority's `birthing_insn_p' boost.  Written per arm the two
   quotients are two single-set pseudos, they land in the same register, and
   jump2 cross-joins the three stores for free -- the target's one copy of
   `sb 0xE' with reorg stealing it into the arm's delay slot.

   `q[0xC] = q[0xD] = q[0xE] = ...' (inner lvalue first), which is b01's
   measured fact: reversed to `q[0xE] = q[0xD] = q[0xC] = ...' it is 50 words
   and 4 aligned differences.
   raw 6 == relocdump's 6: 2 on D_800266BC, 3 on D_800814A0 and 1 on .text
   for the first arm's own `j'. */
void func_s39_8002558C(u8 *p, UnkVec *v, u8 *q)
{
    s32 a;
    s32 b;
    s32 c;

    a = *(s16 *)(p + 0x4);
    b = *(s16 *)(p + 0xA);
    D_800266BC = 1;
    if (a < b)
    {
        q[0xC] = q[0xD] = q[0xE] = a * 96 / b;
    }
    else
    {
        c = *(s16 *)(p + 0x2);
        q[0xC] = q[0xD] = q[0xE] = (c - a) * 96 / (c - b);
    }
    if ((s16)(++*(u16 *)(p + 0x4)) >= *(s16 *)(p + 0x2))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The state 0/1 frame of the other banked actor drawn by func_s39_80024CD4,
   the same shape as func_s39_80025398 with its own RECT D_s39_8002400C: step
   the +0x38 strip up by one a byte at a time while the counter is under 0x32
   and down by one after that, ramp the greys as a two-sided `n * 6' fade on
   +0x2, advance +0x98 by 0xC8 and +0x9A by 4, seed the caller's mover from
   the owner parked at +0x2C, and on state 0 or 1 borrow the owner slot's
   scale pair at 0x1000 across the draw and put it back.  185 words,
   0 differing, 0 aligned differences.

   NO TWIN AT ANY RATIO; donors as for func_s39_80025398, which is this
   body's real source -- it landed first in this same file and every spelling
   below except the `addu' carried over to the first compile.

   `(p + i)[0x38]' AND NOT `p[i + 0x38]' IN BOTH STRIP LOOPS.  The plain index
   form is 185 words and 4 aligned differences, all four rows the loops' own
   `addu' with its two operands exchanged.  Neither loop is strength-reduced
   here -- both count up, neither biv is reversed, and the address giv's
   benefit is 0 -- so the address is recomputed in the loop from the giv
   expression, and `(p + i)' is what makes `pointer_int_sum' put the POINTER
   first.  `p[i + 0x38]' associates the constant into the index and the sum
   comes out `addu $v0, $a0, $s3' where the target has `addu $v0, $s3, $a0'.
   Note the contrast with func_s39_80025398, whose loop IS reduced and where
   the same two spellings therefore measure the same.

   THE q+0x1E STORE RE-READS THE SLOT.  `*(s16 *)(q + 0x1E) = sy;' with the
   value already in a callee-saved register is 184 words and 8 aligned
   differences.  The `sh $s0, 0x1C($a3)' store two lines above invalidates
   cse's copy of `s + 0x1E' -- q and s are unrelated pointers -- so the
   target reloads it, and the register form loses that word.  The 0x1C store
   two lines up has no store before it and does reuse its register, which is
   why the two look inconsistent in the listing.

   THE q+0x14 MASK AND THE OR ARE TWO STATEMENTS: as one expression it is 182
   words and 5 aligned differences.

   `*(s16 *)(s + 0x1C) = *(s16 *)(s + 0x1E) = 0x1000;' -- inner lvalue first,
   so 0x1E is stored before 0x1C.  Reversed it is 185 words and 2 aligned
   differences.  MEASURED AND FREE: the same pair written as two separate
   statements in that order, 185 words and 0 differing, so the chain is a
   style choice and only the ORDER is a fact.

   MEASURED AND FREE, and THE CONTRAST WITH func_s39_80025398 IS THE POINT:
   `(*(u16 *)p)++;' left inside case 0 instead of the hoisted
   `w = *(u16 *)p;' is 185 words and 0 differing HERE, where the same change
   costs func_s39_80025398 a word and 7 aligned differences.  The hoist buys
   nothing without the `sh 0x9A' store sitting in the pre-dispatch block and
   competing with the `lh' for the schedule.  A twin's statement-order fact
   priced on one body is worth re-pricing on the next even inside one unit.

   THE `D_800266BC = 1' STORE COMES FIRST HERE, before the +0x98 read -- the
   opposite of func_s39_8002558C's and func_s39_80025398's order, and it
   matches, because the loads it would be hoisted above are not call
   arguments and carry no dependency on it.
   raw 11 == relocdump's 11: 2 on D_s39_8002400C, 2 on D_800266BC, 3 on
   D_800814A0, 1 R_MIPS_26 on func_s39_80024CD4 and 3 on .text.
   D_s39_8002400C, not D_s39_80024004 -- see func_s39_80025398's note. */
void func_s39_80025648(u8 *p, u8 *b, u8 *q)
{
    UnkDngS39Blk8 area;
    s16 pos[2];
    u8 *m;
    u8 *r;
    u8 *s;
    s32 a;
    s32 i;
    s32 st;
    s32 w;
    s32 sx;
    s32 sy;

    area = D_s39_8002400C;
    D_800266BC = 1;
    *(s16 *)(p + 0x98) = *(u16 *)(p + 0x98) + 0xC8;
    if (*(s16 *)(p + 0x2) < 0x32)
    {
        for (i = 0; i < 0x60; i++)
        {
            (p + i)[0x38]++;
        }
    }
    else
    {
        for (i = 0; i < 0x60; i++)
        {
            if ((p + i)[0x38] != 0)
            {
                (p + i)[0x38]--;
            }
        }
    }
    a = *(s16 *)(p + 0x2);
    if (a < 0x15)
    {
        q[0xC] = q[0xD] = q[0xE] = a * 6 - 0x79;
    }
    else if (a >= 0x50)
    {
        q[0xC] = q[0xD] = q[0xE] = (0x64 - a) * 6 - 0x80;
    }
    *(s16 *)(p + 0x9A) = *(u16 *)(p + 0x9A) + 0x4;
    m = *(u8 **)(p + 0x2C);
    r = *(u8 **)(m - 0x18);
    s = *(u8 **)(m - 0x14);
    *(u32 *)b = *(u32 *)r;
    *(u32 *)(b + 0x4) = *(u32 *)(r + 0x4);
    *(u32 *)(b + 0x8) = *(u32 *)(r + 0x8);
    st = *(s16 *)p;
    w = *(u16 *)p;
    switch (st)
    {
    case 0:
        *(s16 *)p = w + 1;
        *(u16 *)(q + 0x14) &= 0xFF7F;
        *(u16 *)(q + 0x14) |= *(u16 *)(s + 0x14) & 0xC;
        *(u16 *)(q + 0x10) = *(u16 *)(s + 0x10);
        q[0xC] = s[0xC];
        q[0xD] = s[0xD];
        q[0xE] = s[0xE];
        *(u16 *)(s + 0x14) |= 0x80;
        break;
    case 1:
        break;
    default:
        return;
    }
    pos[0] = *(u16 *)&area.b[0] + ((s16)(*(u16 *)&area.b[4]) >> 1);
    pos[1] = *(u16 *)&area.b[2] + 0x48;
    sx = *(u16 *)(s + 0x1C);
    sy = *(u16 *)(s + 0x1E);
    *(s16 *)(q + 0x1C) = sx;
    *(s16 *)(q + 0x1E) = *(u16 *)(s + 0x1E);
    *(s16 *)(s + 0x1C) = *(s16 *)(s + 0x1E) = 0x1000;
    func_s39_80024CD4(p, *(u8 **)(p + 0x2C), (s32 *)&area, (s32 *)pos, 1, 1);
    *(s16 *)(s + 0x1C) = sx;
    *(s16 *)(s + 0x1E) = sy;
    if ((s16)(++*(u16 *)(p + 0x2)) >= 0x65)
    {
        *(s16 *)(p + 0x2) = 0;
        (*(u16 *)p)++;
        *(u16 *)(s + 0x14) &= 0xFF7F;
        *(s16 *)(*(u8 **)(p + 0x30) + 0x9C) = 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s39", func_s39_8002592C);
