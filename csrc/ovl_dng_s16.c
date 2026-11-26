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
void func_s16_800243BC(u8 *p, UnkVec *v);

UnkNode *func_8003FD64(s32, void *);

extern u8 D_800E1C64[];
extern u8 D_s16_80024004[];
extern u8 D_s16_80024034[];
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");
void func_80099844(u8 *, u8 *);
void *func_800990FC(void);
void *func_80099194(u8 *, u8 *);
void *func_80099734(u8 *, u8 *);
u8 *func_80099290(u8 *);
s32 func_800A5720(void *);

/* Apply status kind 0x13 at a strength func_800A6870 derives from the second
   parameter, then -- gated on the node's +0x14 & 0x4000 on both paths --
   print the resident D_800E1C64 message if the status took, or assemble the
   three-part bank-local message D_s16_80024004 / name / D_s16_80024034 if it
   did not.

   The gate is csrc/ovl_dng_s20.c's func_s20_800262B8 (func_8009D218 through
   a TWO-argument alias, func_800A48F0 through an s16-returning one) and the
   dialog chain is csrc/ovl_dng_b21.c's func_b21_80175E14 with one extra
   func_80099194 in front of the func_80099734 name insert.

   The second parameter is `u8', which is what makes the
   `andi $a0, $s0, 0xFF' ahead of the func_800A6870 call its own truncation
   rather than a mask -- s20's entry measures the s32 spelling at 9 differing
   in its own body; nothing had to be fought for here.
   60 words, 0 differing, first compile.

   D_s16_80024004 and D_s16_80024034 ARE EXTERNS, NOT DEFINITIONS: they sit in
   this object's `data' subsegment, 0x1894800..0x1894838, below the `.rodata'
   one that owns D_s17_80024058.
   THIS COPY'S OWN CONSTANT: 0x2 -> 0x1, 0x13 -> 0x12, 0x4 -> 0x2.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
s32 func_800BCB04(s32, s32, s16);
extern s16 D_80025308;
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

s32 func_800644B8(s32);
s32 func_80064584(s32);
void func_800478B8(u8 *);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
extern const u8 D_s16_80024038[];
extern u8 D_s16_800252FC[];
extern u8 D_80045340[];
extern u8 D_800DDC40[];
extern u8 D_800DEAE0[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
UnkNode *func_8003FC64(s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
void func_8003DB94(void *, void *, s32);
void func_s16_8002407C_3(u8 *, u8, u8 *) __asm__("func_s16_8002407C");

void func_s16_8002407C(u8 *p, u8 d)
{
    void *m;
    void *h;

    if (func_8009D218_2(p, 1) == 0)
    {
        if ((s16)func_800A48F0_i(p, 0x12, (s8)(func_800A6870(d) + 2)) != 0)
        {
            if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
            {
                func_80099844(p, D_800E1C64);
            }
        }
        else if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_s16_80024004, h);
            h = func_80099734(p, h);
            h = func_80099194(D_s16_80024034, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
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
s32 func_s16_8002416C(u8 *p, UnkVec *v)
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

/* One frame of the 0x211 spark func_s37_80024FA0 spawns: pin D_80025308 to 1,
   and while the mover's +0xA is still below the ground height func_800BCB04
   reports two units above it, climb +0x8 by 0x20000 plus a fresh
   rand() & 0xFFF; the payload's +0x32 countdown falls by 8 a frame and raises
   the node's redraw bit and D_800814A0's at zero. 51 words, 0 differing.

   THE THREE ARGUMENT LOADS ARE THREE LOCALS, ASSIGNED BEFORE THE D_80025308
   STORE, IN THE ORDER +0xA, +0x2, +0x6. Written with the loads inside the
   call it is 51 words and 6 differing, every row the `li 1' / `sh' pair and
   the three `lhu's trading places: gcc 2.7.2 cannot disambiguate the
   `mem:HI (lo_sum D_80025308)' store from `mem:HI ($s0 + 10)', so each
   argument load carries the store in its dependency list, and calls.c emits
   a call's argument register loads at the very END of its expansion -- the
   only way to get them above the store is to evaluate them in an earlier
   statement. The shape is csrc/ovl_dng_s18.c's func_s18_80024FF0, which
   documents it for the 48-word relative of this family; the bearing test
   itself is csrc/ovl_dungeon_800CABA8.c's func_800CAED0 (67 words) with a
   one-axis rand() climb in place of its three-axis glide and 8 for its 4.
   raw 7 == relocdump's 7: 2 on D_80025308, 3 on D_800814A0, 2 R_MIPS_26. */
void func_s16_800243BC(u8 *p, UnkVec *v)
{
    s32 x;
    s32 y;
    s32 z;
    s16 r;

    z = *(u16 *)((u8 *)v + 0xA);
    x = *(u16 *)((u8 *)v + 0x2);
    y = *(u16 *)((u8 *)v + 0x6);
    D_80025308 = 1;
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
   less 0x10, then park func_s16_800243BC as the step and
   func_s16_8002416C as the draw, with `b` at the payload's +0x14, `d` at
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
void func_s16_80024488(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s16_800243BC;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s16_8002416C);
        *(u32 *)(p + 0x8) = c;
    }
}

/* The 0x212 orbiter's step, installed by func_s16_800248C4's case 3. Pin
   D_80025308 to 1, place the mover on a circle of radius +0xA about the
   centre cached at +0x4C / +0x50 at the angle in +0xC -- func_800644B8 and
   func_80064584 are the sine/cosine pair and the product is scaled by 16 --
   then run the four-state script in +0x0 and raise the node's redraw bit
   whenever the slot's +0x14 carries 0x8000. 199 words, 0 differing.

   THE FIRST CALL'S ANGLE ARGUMENT MUST BE AN ARRAY REF, `((s16 *)p)[6]', AND
   THAT IS THE WHOLE RESIDUE: with a plain `*(s16 *)(p + 0xC)' cast at both
   call sites the body is 199 words / 9 aligned differences, every row in the
   prologue. The mechanism is the fixed-scalar-versus-varying-struct rule:
   `calls.c' emits an argument register load at the very END of a call's
   expansion, so the `lh 0xC($s1)' starts out BELOW the
   `sh %lo(D_80025308)' store, and through a plain cast gcc 2.7.2 cannot
   disambiguate the two MEMs and the load carries the store in its dependency
   list. The array ref sets `MEM_IN_STRUCT_P', `true_dependence' goes false
   against the fixed-address scalar, sched1 hoists the load above the store,
   and dbr then fills the `jal's delay slot with the `sh' instead of with the
   `$a2' -> `$s2' parameter copy -- which is why the residue reads as four
   prologue instructions in the wrong order rather than as a dependence.
   This is the same obstacle func_s16_800243BC records three lines up, with
   the opposite remedy: that body evaluates its three argument loads into
   locals ahead of the store, which is not available here because a local
   would survive into the SECOND call and the target reloads `lh 0xC($s1)'
   for it (an `s32' local for the angle is 198 words).

   ONLY THE FIRST SITE IS LOAD-BEARING, measured three ways: array ref at
   both sites 199/0 (kept), array ref at the first and a plain cast at the
   second 199/0 -- so the second site is FREE, not a fact -- plain cast at
   the first and an array ref at the second 199/9. Both sites are written the
   same way because the field is one field, not because the second is worth
   anything.

   THE ANGLE WRAP READS +0xC BACK OUT OF MEMORY. `*(s16 *)(p + 0xC) =
   *(u16 *)(p + 0xC) - *(u16 *)(p + 0xE);' followed by
   `if (*(s16 *)(p + 0xC) < 0)' is what produces the `sll 16' / `bgez' pair:
   cse forwards the stored value but the test still needs the HImode sign
   extension. Through an `s32' local -- `k = ...; *(s16 *)(p + 0xC) = k; if
   (k < 0)' -- the extension is gone and the body is 198 words / 18 aligned
   differences, ONE WORD SHORT. Cases 1 and 2 share the idiom with different
   ceilings (0x7F8 and 0x100).

   CASE 1's THREE `c[]' BYTE BUMPS ARE WRITTEN 0xC, 0xD, 0xE AND EMIT
   0xC, 0xE, 0xD -- written 0xE, 0xD, 0xC it is 4 aligned differences. sched1
   exchanges the last two independent read-modify-writes, which is
   csrc/ovl_dng_s42.c func_s42_80024AD0's fact 2 with the ascending sign, and
   the `*(s16 *)(c + 0x1C)' / `+ 0x1E' pair above them is NOT exchanged
   because both are written before any of the bytes. Do not transcribe the
   emitted order.

   The `k' chain is four DESCENDING `>=' tests, 0x65 / 0x47 / 0x33, under an
   outer `>= 0x41' gate, and the 0x33 arm is unreachable in the target too:
   gcc 2.7.2 has no range propagation, so the dead `slti 0x33' is emitted.
   Two of the four arms assign 2; that is what the bytes say.

   Hand-audited symbol multiset, since `relocheck' needs a build I may not
   run: raw 18 == the listing's own 18 relocated-or-text-relative fields,
   itemised and matched one for one -- 3 `jal' (func_800644B8, func_80064584,
   func_800478B8, once each), 7 internal `j' to `.text' labels,
   1 HI16 + 1 LO16 on D_80025308 and 2 HI16 + 4 LO16 on D_800814A0. Nothing
   sits on an unexpected field and no `%hi' names a function. */
void func_s16_800245A8(u8 *p, UnkVec *v, u8 *c)
{
    s32 k;

    D_80025308 = 1;
    v->unk_00 = *(s32 *)(p + 0x4C) + *(s16 *)(p + 0xA) * func_800644B8(((s16 *)p)[6]) * 16;
    v->unk_04 = *(s32 *)(p + 0x50) + *(s16 *)(p + 0xA) * func_80064584(((s16 *)p)[6]) * 16;
    func_800478B8(c);
    switch (*(s16 *)p)
    {
    case 0:
        if ((*(s16 *)(p + 0x2) -= 1) <= 0)
        {
            *(s16 *)p = *(u16 *)p + 1;
        }
        break;
    case 1:
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) - *(u16 *)(p + 0xE);
        if (*(s16 *)(p + 0xC) < 0)
        {
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x1000;
        }
        if (*(s16 *)(p + 0xE) < 0x7F8)
        {
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 0x8;
        }
        if (*(s16 *)(p + 0xA) >= 0x41)
        {
            if (*(s16 *)(p + 0xA) >= 0x65)
            {
                k = 3;
            }
            else if (*(s16 *)(p + 0xA) >= 0x47)
            {
                k = 2;
            }
            else if (*(s16 *)(p + 0xA) >= 0x33)
            {
                k = 2;
            }
            else
            {
                k = 1;
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) - k;
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - 0x46;
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - 0x46;
            c[0xC] = c[0xC] + 0x4;
            c[0xD] = c[0xD] + 0x4;
            c[0xE] = c[0xE] + 0x4;
        }
        else
        {
            *(s16 *)(p + 0x2) = 0x28;
            *(s16 *)p = *(u16 *)p + 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) - *(u16 *)(p + 0xE);
        if (*(s16 *)(p + 0xC) < 0)
        {
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) + 0x1000;
        }
        if (*(s16 *)(p + 0xE) < 0x100)
        {
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 0x8;
        }
        if (*(s16 *)(p + 0xA) >= 0x1B)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) - 1;
        }
        if (c[0xC] >= 2)
        {
            c[0xC] = c[0xC] - 0x2;
            c[0xD] = c[0xD] - 0x2;
            c[0xE] = c[0xE] - 0x2;
        }
        if (c[0xC] == 0)
        {
            *(s16 *)(p + 0x2) = 0;
            *(s16 *)p = *(u16 *)p + 1;
        }
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - 0x14;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0xC8;
        break;
    case 3:
        if ((*(s16 *)(p + 0x2) -= 1) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* THE MASTER STEP OF THIS ROOM'S SCRIPT, AND IT OWNS jtbl_s16_80024058 AND
   READS THE 0x20-BYTE D_s16_80024038 TABLE ABOVE IT. 654 words, 0 differing,
   0 aligned differences. Nine `.word's = 9 cases + 0 pad, dispatch
   `sltiu $v0, $v1, 0x9' with no minval subtraction, so the case set is 0..8.
   Entry list in table order:

     case 0  .Ls16_800249B0      case 5  .Ls16_800252D4  (NO ARM)
     case 1  .Ls16_80024A30      case 6  .Ls16_800252D4  (NO ARM)
     case 2  .Ls16_80024CD8      case 7  .Ls16_800252D4  (NO ARM)
     case 3  .Ls16_80024ED4      case 8  .Ls16_80025270
     case 4  .Ls16_80025074

   `.Ls16_800252D4' is BOTH the range-check `beqz' target AND the first word
   of the epilogue, so there is no `default:', no `case 5:', no `case 6:' and
   no `case 7:'. Six real arms written 0 1 2 3 4 8 -- ascending, which here
   IS entry-address order, case 8's block sitting just above the epilogue --
   with case 0 falling through into case 1.

   ARM ORDER WAS SETTLED BY COMPILING, NOT BY SWEEPING, because `ovlverify',
   `aligndiff' and `relcand' are all blind to it. `tools/cdkgcc -O2 -G0 -S'
   on this file emits `.word $L55 $L56 $L67 $L80 $L85 $L54 $L54 $L54 $L98'
   with `beq $2,$0,$L54' as the range check and `$L54' last in the file, and
   the label-address ranks are 1 2 3 4 5 7 7 7 6 -- the identical permutation
   to the ROM's. That is a direct measurement of the emitted order; the 0
   differing is not evidence about it.

   DONOR: csrc/ovl_dng_s42.c's LANDED func_s42_80024AD0 (722 words), which
   `dngtwin' ranks 0.788 and `dngcallee' puts top of a clean 12-of-19 tier --
   the two tools agreeing on one FILE was worth more than either row. Same
   `(u8 *p, UnkVec *v, u8 *c)' signature, same `o'/`n'/`m' head with the tbl
   copy BEFORE `n' and `m', same 9-entry table with cases ABSENT rather than
   empty, same D_80045340 tag block, same `D_8006CCD8[facing]' /
   `D_8006CCE8[facing]' corner pair, same
   `__builtin_abs(... ? ... : ...) * 2 - 1' stride, and the same case 8. Mine
   is 68 words shorter for three reasons and they are all absences: no
   `RECT r; DVECTOR w;' stack pair (its case 3 has no func_800B8FC8 framing
   call), no `*(s16 *)(c + 0x1C/0x1E) = 0' in either escape arm, and no
   `|= 0x100000' on the target's +0x14. It gains the +0x92/+0x9C toggle,
   which the donor has nowhere and which appears THREE times here.
   Both bodies save seven callee-saved registers, so the budget facts were
   admissible; the frame differs only in the locals area (-0x68 against
   -0x78), which is exactly the two absent stack structs.

   FIRST COMPILE WAS 654 WORDS / 7 ALIGNED DIFFERENCES -- the word count was
   never wrong, so every fact below is schedule, allocation or one arithmetic
   slip, and only `aligndiff' could see any of them. Each row below was
   RE-PRICED ONE AT A TIME against the finished body, not taken from the
   mid-fight figure.

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | as kept                                               |  654  |   0  |
   |  case 3's +0xA store written BEFORE the +0x2 store    |  654  |   2  |
   |  case 3's angle written `<< 9' instead of `* 0x200'   |  654  |   1  |
   |  case 4's 0x4C gate written `< 0x4C'                  |  654  |   1  |
   |  case 4's g[] triples written 0xE, 0xC, 0xD           |  654  |  21  |
   |  case 4's two o+0x60 pointers merged into one local   |  654  |  25  |
   |  case 3's slot pointer assigned ONCE, not twice       |  654  | 118  |

   1. CASE 3 WRITES `*(s16 *)(q + 0x2) = 8 - *(u16 *)(p + 0x82);' BEFORE
      `*(s16 *)(q + 0xA) = 0x80;' AND THE LISTING SHOWS THE OPPOSITE ORDER.
      Two independent adjacent stores, so sched1 transposes them, and the
      `lhu 0x82($s2)' has to end up ABOVE the `sh 0xA($s0)' -- which it
      cannot do from the other order, because two pseudo bases are not
      disambiguable and the load carries the store in its dependency list.
      Writing the emitted order is the last 2 rows. The alternative remedy --
      reading +0x82 into a local ahead of both stores, which is what
      func_s16_800243BC does for its D_80025308 store -- was NOT needed and
      was not measured; the plain reorder is free and is not a device.

   2. CASE 3's ANGLE IS `* 0x200', NOT `<< 9'. One row, and the diagnostic is
      exactly `lhu' where the target has `lh' with nothing else moving. Both
      expressions are the low 16 bits of the same value, but `convert.c's
      narrowing arm is guarded on `TREE_UNSIGNED (type)' for `LSHIFT_EXPR'
      and not for `MULT_EXPR', so into the `*(s16 *)' store the shift form
      lets the load narrow to a zero-extend and the multiply form does not.
      The levers file's older note that `i * 0x400' and `i << 10' are "the
      same tree and free either way" holds only WITHOUT a narrowing context;
      this site has one.

   3. CASE 4's 0x4C GATE IS `>= 0x4C' WITH THE `+3' ARM FIRST. One row, the
      branch opcode alone -- `bnez' in the target against `beqz' -- with both
      blocks in the same place either way, because the arms are the same
      length. The `< 0x4C' reading is the natural one off the listing and it
      is wrong: read which arm is the FALL-THROUGH, not which way the branch
      goes.

   4. CASE 4's THREE g[] BYTE BUMPS ARE WRITTEN 0xE, 0xD, 0xC AND EMIT
      0xE, 0xC, 0xD. 21 rows transcribed in the emitted order. This is the
      donor's fact 2 reproducing almost to the digit (it prices 20), and the
      cause is the same: sched1 exchanges the last two independent
      read-modify-writes, and jump2 cross-joins the two arms, so the shared
      `sb' after the `j' belongs to neither arm's last source statement. Both
      arms must agree. Note the sign is the OPPOSITE of the same mechanism in
      func_s16_800245A8's case 1, where ascending 0xC, 0xD, 0xE is what
      lands -- the rule is "the last two are exchanged", not a direction.

   5. CASE 4's TWO `*(u8 **)(o + 0x60)' POINTERS ARE TWO LOCALS, NOT ONE.
      25 rows shared, every row a register exchange rather than a moved
      instruction. The donor's fact 4 prices the same change at 31 on its own
      body: the DIRECTION transferred and the magnitude did not, which is
      what the levers file predicts for a reference-count fact. `g', which is
      genuinely shared between the two blocks, stays one local.

   6. CASE 3's SLOT POINTER IS ASSIGNED TWICE AND THAT IS THE LARGEST FACT IN
      THE BODY. `s = (UnkSlot *)n2->unk_0C;' appears once before `s->unk_10'
      and again before `s->unk_1E', matching the listing's two
      `lw $a0, 0xC($s3)'. With ONE assignment the body is still 654 words but
      118 aligned differences, with $s2 and $s3 exchanged through the WHOLE
      function including the prologue -- a single-set pseudo lives across the
      two intervening calls in a callee-saved register and never reloads,
      which is `reg_n_sets' presenting as a whole-body rotation exactly as
      the levers file describes. A residue that large reads as "nothing
      matches"; it is one line.

   The arithmetic slip worth recording because it is easy to repeat: case 4's
   three `c[]' fades divide by 0x14, not by 0xA. `0x66666667' with
   `sra $v1, $t0, 3' is /20; `sra ..., 2' is /10, and that was 3 of the first
   compile's 7 rows. Read the shift after the `mfhi', not just the magic
   constant. The multipliers are 0xE0 for 0xC and 0x80 for both 0xD and 0xE,
   in case 2 as well as case 4.

   THE func_s16_8002407C CALL IN CASE 4 PASSES THREE ARGUMENTS AND THE LANDED
   DEFINITION IN THIS FILE TAKES TWO, so it goes through a unit-local arity
   alias, `func_s16_8002407C_3', which leaves that 60-word body untouched --
   re-verified at 60 words / 0 differing afterwards, as were all four of the
   unit's other landed bodies. $a1 and $a2 are both set only for the call and
   dead after it, so the third argument is real and not a scheduling
   artefact; the donor's equivalent, func_s42_800240AC, takes three.

   D_s16_80024038 and D_s16_800252FC ARE EXTERNS, NOT DEFINITIONS, and both
   are read by STRUCT ASSIGNMENT through a cast -- `lwl'/`lwr'/`swl'/`swr'
   block moves of 0x20 and 0xC bytes, not aggregate initialisers, so no
   `$LC0' pool is emitted and nothing lands in this unit's rodata but the
   jump table.

   Hand-audited symbol multiset, since `relocheck' needs a build I may not
   run: raw 59 == the listing's own 59 relocated-or-text-relative fields,
   itemised and matched one for one -- 14 `jal' (func_8004491C, func_80069EF8
   and func_s16_80024488 twice each, the other eight once), 15 HI16, 17 LO16
   and 13 internal `j' to `.text' labels. `relcand' is `ok' on 10 data
   symbols and 14 calls. ONE `%hi'/`%lo' PAIR DOES NAME A FUNCTION --
   func_s16_800245A8, the step handler case 3 parks at `n2->unk_10' -- which
   is expected here and is the one place the usual "no `%hi' names a
   function" line does not apply. */
void func_s16_800248C4(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    UnkSlot *s;
    u8 *o;
    u8 *m;
    u8 *m2;
    u8 *e;
    u8 *t;
    u8 *t2;
    u8 *t3;
    u8 *u;
    u8 *g;
    u8 *q;
    s32 i;
    s32 k;
    s32 z;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s16_80024038;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(UnkDngS58Name12 *)(p + 0x94) = *(UnkDngS58Name12 *)D_s16_800252FC;
        *(u32 *)(c + 0x8) = (u32)(p + 0x94);
        D_80025308 = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out,
                          0) == NULL)
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
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x14;
            c[0xD] = 0x14;
            c[0xC] = 0x14;
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
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] + 0x20);
            e = *(u8 **)(o - 0x14);
            p[0xA0] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xA1] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
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
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            func_s16_80024488((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x2020E0,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        if (*(s16 *)(p + 0x92) == 0)
        {
            *(s16 *)(p + 0x92) = 1;
            p[0x9C] = p[0x9C] + 0x18;
        }
        else
        {
            *(s16 *)(p + 0x92) = 0;
            p[0x9C] = p[0x9C] - 0x18;
        }
        c[0xC] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) * 0xE0 / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xD] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) * 0x80 / *(s16 *)(p + 0x8A) +
                 0x14;
        c[0xE] = (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) *
                     (*(s16 *)(p + 0x88) - *(s8 *)(p + 0x7B)) * 0x80 / *(s16 *)(p + 0x8A) +
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
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                func_800A56E0(0x300);
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
            }
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x92) == 0)
        {
            *(s16 *)(p + 0x92) = 1;
            p[0x9C] = p[0x9C] + 0x18;
        }
        else
        {
            *(s16 *)(p + 0x92) = 0;
            p[0x9C] = p[0x9C] - 0x18;
        }
        n2 = func_8003FC64(0x212);
        if (n2 != NULL)
        {
            q = (u8 *)n2 + 0x20;
            *(s16 *)(q + 0x2) = 8 - *(u16 *)(p + 0x82);
            *(s16 *)(q + 0xA) = 0x80;
            *(s16 *)(q + 0xC) = *(s16 *)(p + 0x82) * 0x200;
            u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
            *(s32 *)(q + 0x4C) = *(s32 *)(u + 0x0);
            *(s32 *)(q + 0x50) = *(s32 *)(u + 0x4);
            n2->unk_10 = (u32)func_s16_800245A8;
            func_8004491C(n2, (u32)D_80045340);
            s = (UnkSlot *)n2->unk_0C;
            s->unk_10 = 0x20;
            s->unk_14 |= 0xC;
            m2 = (u8 *)n2->unk_08;
            *(s32 *)(m2 + 0x0) =
                *(s32 *)(q + 0x4C) + *(s16 *)(q + 0xA) * func_800644B8(*(s16 *)(q + 0xC)) * 16;
            *(s32 *)(m2 + 0x4) =
                *(s32 *)(q + 0x50) + *(s16 *)(q + 0xA) * func_80064584(*(s16 *)(q + 0xC)) * 16;
            *(s32 *)(m2 + 0x8) =
                v->unk_08 + (D_800DDC40[(*(u8 **)(o + 0x60))[0x13]] << 16);
            s = (UnkSlot *)n2->unk_0C;
            s->unk_1E = 0x1000;
            s->unk_1C = 0x1000;
            s->unk_0C[2] = 0x14;
            s->unk_0C[1] = 0x14;
            s->unk_0C[0] = 0x14;
            func_8003DB94(s, D_800DEAE0, 0);
        }
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) == 8)
        {
            *(s16 *)(p + 0xA) = 4;
            *(s16 *)(p + 0x82) = 0;
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) < 0x14)
        {
            c[0xC] = (0x14 - *(s16 *)(p + 0x82)) * 0xE0 / 0x14;
            c[0xD] = (0x14 - *(s16 *)(p + 0x82)) * 0x80 / 0x14;
            c[0xE] = (0x14 - *(s16 *)(p + 0x82)) * 0x80 / 0x14;
            if (*(s16 *)(p + 0x92) == 0)
            {
                *(s16 *)(p + 0x92) = 1;
                p[0x9C] = p[0x9C] + 0x18;
            }
            else
            {
                *(s16 *)(p + 0x92) = 0;
                p[0x9C] = p[0x9C] - 0x18;
            }
            for (i = 0; i < 4; i++)
            {
                func_s16_80024488((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0x2020E0,
                                  (rand() & 0xFF) | 0x80, 0, 0, 0);
            }
        }
        if (*(s16 *)(p + 0x82) >= 0x32)
        {
            t2 = *(u8 **)(o + 0x60);
            *(u32 *)(t2 + 0x1C) |= 0x10000000;
            g = *(u8 **)(t2 - 0x14);
            if (*(s16 *)(p + 0x82) >= 0x4C)
            {
                g[0xE] = g[0xE] + 0x3;
                g[0xD] = g[0xD] + 0x3;
                g[0xC] = g[0xC] - 0x4;
            }
            else
            {
                g[0xE] = g[0xE] - 0x3;
                g[0xD] = g[0xD] - 0x3;
                g[0xC] = g[0xC] + 0x4;
            }
        }
        if (*(s16 *)(p + 0x82) >= 0x65)
        {
            t3 = *(u8 **)(o + 0x60);
            *(u32 *)(t3 + 0x1C) &= 0xEFFFFFFF;
            g = *(u8 **)(t3 - 0x14);
            g[0xE] = 0x80;
            g[0xD] = 0x80;
            g[0xC] = 0x80;
            func_s16_8002407C_3(*(u8 **)(o + 0x60), p[0x9], o);
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_80025308 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025308 = 0;
            }
        }
        break;
    }
}
