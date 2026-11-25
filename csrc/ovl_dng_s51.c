#include "common.h"

void func_800478B8(u8 *);

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
void func_s51_80024734();
s32 func_s51_800244E4(u8 *p, UnkVec *v);

void func_s51_800249A0(u8 *p);
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
extern s16 D_8002571C;

extern u8 D_80045340[];
extern u8 D_800256E0[];
UnkNode *func_8003FC64(s32);
void func_s51_80024104(u8 *p);

UnkNode *func_8003FD64(s32, void *);

extern u8 D_s51_80024004[];
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
void func_s51_8002405C(u8 *p, UnkVec *v, UnkSlot *s);

/* func_s51_80024104's spawn stepped one frame: spend one of the payload's
   +0x2A frames, raise the D_8002571C tick, drive the renderable's three
   colour bytes off the remaining life over the original span at +0x2C (times
   128) with +0xC lifted 0x10 above the other two, bleed +0x6 down while it is
   still above -0x6, and when the counter runs out release the owner's
   busy-table slot at +0x46 and raise the two 0x8000 kill bits.  42 words,
   0 differing, first compile.  The twin is func_s58_8002407C
   (csrc/ovl_dng_s58.c, dngtwin 0.881, +0 words), carried with the relink
   constants and ONE shape delta read off this listing: the three colour bytes
   are PLAIN STORES here -- `s + 0xE = k', `s + 0xD = k', `s + 0xC = k + 0x10'
   -- where the twin has `*((u8 *)s + 0xE) += 4'.  There is no `lbu' at +0xE
   in this listing, so it is not a read-modify-write.

   THE COUNTER READ-MODIFY-WRITE MUST BE SPLIT AROUND THE D_8002571C STORE.
   Folded into `*(s16 *)(p + 0x2a) -= 1' with the quotient read back out of
   memory it is 42 words / 18 aligned differences; the target's
   read / foreign-store / write-back order cannot come from one compound
   assignment.  Same fact as func_s51_800249A0's, and it holds in three more
   bodies in this unit at three different prices -- 18 here, 12 on
   func_s51_80024734, 7 on func_s51_8002492C and FREE on func_s51_800248B8.

   THE THREE BYTE STORES RUN 0xE, 0xD, 0xC.  Written ascending the body is
   41 words / 11 aligned differences, because the `addiu 0x10' then has to
   precede both other `sb's instead of sitting between them.

   `s16 c' is load-bearing: as `s32' it is 41 words / 2 rows.

   MEASURED AND FREE, so none of these is why it lands: `(c << 7)' for
   `c * 128' (42 / 0); `s16 k' instead of `s32 k' (42 / 0 -- the narrow
   result lever does not reach a quotient here); the counter read as
   `*(s16 *)' instead of `*(u16 *)' (42 / 0); the guard written `> -0x7'
   (42 / 0); and the release index written base-first (42 / 0). */
void func_s51_8002405C(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 c;
    s32 k;

    c = *(u16 *)(p + 0x2a) - 1;
    k = c * 128 / *(s16 *)(p + 0x2c);
    D_8002571C = 1;
    *(s16 *)(p + 0x2a) = c;
    *((u8 *)s + 0xE) = k;
    *((u8 *)s + 0xD) = k;
    *((u8 *)s + 0xC) = k + 0x10;
    if (*(s16 *)((u8 *)s + 0x6) >= -0x6)
    {
        *(s16 *)((u8 *)s + 0x6) -= 2;
    }
    if (*(s16 *)(p + 0x2a) <= 0)
    {
        *(s16 *)(*(s16 *)(p + 0x32) * 2 + *(s32 *)(p + 0x60) + 0x46) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 spawner behind func_s51_800243D8's step, and the owner of this
   object's eight-slot busy table at +0x46.  Copy the 0x40-byte
   D_s51_80024004 record table onto the stack, scan the eight slots for a free
   one, and on finding it mark the slot, place the record through
   func_800B8FC8, spawn a 0x212 node with func_s51_8002405C as its step, clear
   and tint its renderable, seed its mover from the player record and copy the
   12-byte D_800256E0 record; then spend one frame of the owner's own +0x2A
   counter and raise the two 0x8000 kill bits when it runs out.  181 words,
   0 differing, first compile.

   THE TWIN IS func_s65_80024104 (csrc/ovl_dng_s65.c, dngtwin 0.964, 184
   words), taken verbatim with the relink constants and ONE statement DELETED:
   its `D_80025384 = 1' tick store has no counterpart in this listing, and
   that single statement is the whole three-word deficit dngtwin flagged.
   This copy raises no tick at all; func_s51_8002405C, the step it installs,
   raises D_8002571C instead.

   THE TWIN'S `i * 8' FACT CARRIED, and its rejected column reproduced to the
   digit.  Through its own `s32 k' the shift is emitted before the
   `addiu $v0, $sp, 0x20' and the pair matches; inlined as `buf + i * 8' it is
   181 words / 4 aligned differences (the twin records 3 on its own body, so
   this is the one row of its table that does NOT reproduce exactly), and
   `(u8 *)(i * 8 + (s32)buf)' is 181 / 1, which is the twin's recorded 1.

   THE TWIN'S ALIGNMENT-1 FACT ALSO CARRIED, and it is the biggest thing in
   this body.  The 0x40-byte copy's struct type has `u8 b[0x40]', alignment 1,
   which is what makes expand_block_move emit the `andi 0x3' runtime
   alignment test and BOTH loops.  Through an otherwise identical struct of
   `u32 w[0x10]' the whole test and the entire `lwl'/`lwr' arm vanish:
   157 words / 25 aligned differences.

   `s16 i' is load-bearing: as `s32' it is 175 words / 18 rows (the six
   missing words are the `sll 16'/`sra 16' pairs the scan loop needs).
   `SVECTOR w' must be declared BEFORE `buf' -- after it, 181 / 5, the two
   stack objects exchanged.  The two 7-seeds go +0x2A then +0x2C (swapped,
   181 / 2) and the two tail `sb's go +0x7A then +0x7B (swapped, 181 / 6).

   MEASURED AND FREE, so NOT the reason this lands: the scan index written
   `*(s16 *)(p + 0x46 + i * 2)' instead of `*(s16 *)(i * 2 + (s32)p + 0x46)'
   -- 181 / 0, so only the `k' statement's existence matters, not the operand
   order of either sum. */
void func_s51_80024104(u8 *p)
{
    SVECTOR w;
    u8 buf[0x40];
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;
    s16 i;
    s32 k;

    *(UnkDngS58Blk64 *)buf = *(UnkDngS58Blk64 *)D_s51_80024004;
    for (i = 0; i < 8; i++)
    {
        if (*(s16 *)(i * 2 + (s32)p + 0x46) == 0)
        {
            break;
        }
    }
    if (i != 8)
    {
        *(s16 *)(i * 2 + (s32)p + 0x46) = 1;
        k = i * 8;
        e = buf + k;
        w.vx = *(u16 *)(e + 0x0) + ((s16) * (u16 *)(e + 0x4) >> 1);
        w.vy = *(u16 *)(e + 0x2) + 0x30;
        func_800B8FC8((u8 *)D_800814A8, (s32 *)e, (s32 *)&w, 1, 1);
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x2a) = 7;
            *(s16 *)(q + 0x2c) = 7;
            *(s16 *)(q + 0x32) = i;
            *(u8 **)(q + 0x60) = p;
            n->unk_10 = (u32)func_s51_8002405C;
            func_8004491C(n, (u32)D_80045340);
            r = (UnkSlot *)n->unk_0C;
            r->unk_06 = 4;
            r->unk_14 &= ~0xC;
            s = *(u8 **)(D_800814A8 - 0x18);
            m = n->unk_08;
            m->unk_00 = *(s32 *)(s + 0x0);
            m->unk_04 = *(s32 *)(s + 0x4);
            m->unk_08 = *(s32 *)(s + 0x8);
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            *((u8 *)r + 0xE) = 0x80;
            *((u8 *)r + 0xD) = 0x80;
            *((u8 *)r + 0xC) = 0x80;
            *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_800256E0;
            r->unk_08 = (u32)((u8 *)n + 0x92);
            q[0x7a] = e[0] - 0x40;
            q[0x7b] = e[2];
        }
    }
    if (--*(s16 *)(p + 0x2a) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_s51_80024104's own spawner, run as a step: allocate a 0x212 node, give
   it 0xB frames at the payload's +0x2C and the owner `a` at +0x7C, park
   func_s51_80024104 as its step, then clear the renderable -- 0x20 at +0x10,
   0 at +0x06, the 0xC bits in +0x14, a black colour and a zero scale -- seed
   its mover from the owner's name record and point the slot's +0x8 at the
   node's own +0x58, into which the 12-byte D_800256E0 record is copied.
   67 words, 0 differing, first compile.  The skeleton is func_800D4AB0
   (csrc/ovl_dungeon_800CABA8.c): same 0x212/func_8004491C/D_80045340 sequence
   and the same `n->unk_0C` RE-READ after the mover stores, which is what
   keeps the second `lw 0xC($s0)`.

   Two shapes differ from that twin and both are read off this listing.  The
   mover copy is THREE ALIGNED WORDS, not the twin's three halfwords: `m` and
   `s` are unrelated pointers gcc cannot prove disjoint, so each `sw` pins the
   next `lw` and the three load-delay slots stay `nop`.  And the 12-byte
   record copy is a struct whose type has ALIGNMENT 1 (`u8 b[0xC]`), which is
   what forces the `lwl`/`lwr` + `swl`/`swr` pairs instead of aligned `lw`s --
   the same fact UnkDungeonName12 carries in csrc/ovl_dungeon_decls.h.
   THIS COPY'S OWN CONSTANT: 0x2c -> 0x2a, 0x7c -> 0x60, 0x58 -> 0x92 at all 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_800243D8(u8 *a)
{
    UnkNode *n;
    u8 *p;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x2a) = 0xB;
        *(u8 **)(p + 0x60) = a;
        n->unk_10 = (u32)func_s51_80024104;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        s = *(u8 **)(a - 0x18);
        m = n->unk_08;
        m->unk_00 = *(s32 *)(s + 0x0);
        m->unk_04 = *(s32 *)(s + 0x4);
        m->unk_08 = *(s32 *)(s + 0x8);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0;
        r->unk_1C = 0;
        *((u8 *)r + 0xE) = 0;
        *((u8 *)r + 0xD) = 0;
        *((u8 *)r + 0xC) = 0;
        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_800256E0;
        r->unk_08 = (u32)((u8 *)n + 0x92);
    }
}

/* One semi-transparent 1x1 TILE_1 and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with each node's colour word
   scaled by the payload's +0x32 life over 128.  148 words, 0 differing, first
   compile.  The twin is func_800D68F4 (csrc/ovl_dungeon_800D50FC.c), which is
   this walk with a constant colour; that function's long note on the shape --
   `addPrim` as a bitfield store, the two loop-hoisted masks in $s2/$s4,
   `setTile1` plus `setSemiTrans` folding to one `sb 0x6A` -- applies here
   unchanged.  The only relink is the scratchpad struct: this object's fields
   sit at +0x00/+0x02/+0x04, +0x18, +0x20, +0x90/+0x94 and +0xC0 where the
   resident original has them at +0x04/+0x06/+0x08, +0x1C, +0x24, +0xD0/+0xD4
   and +0x100, which is what UnkDng73E94SP already describes.  The twin's three
   dead colour `lbu`s are live here: they are `setRGB0`'s three operands, and
   the three `mult`/`bgez`/`addiu 0x7F`/`sra 7` runs are the signed `/ 128`. */
s32 func_s51_800244E4(u8 *p, UnkVec *v)
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
        sp->unk_C0 = RotTransPers((SVECTOR *)&sp->unk_00, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setRGB0(t, t->r0 * *(s16 *)(p + 0x32) / 128, t->g0 * *(s16 *)(p + 0x32) / 128,
                    t->b0 * *(s16 *)(p + 0x32) / 128);
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

/* func_s51_80024798's spawn stepped one frame: pull the mover's +0x8 channel
   back by 0x18000, spend 8 of the payload's +0x32 life, raise the D_8002571C
   tick, and when the life runs out raise the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  25 words, 0 differing, first compile.

   THE TWIN IS THIS FILE'S OWN LANDED func_s51_800249A0 (dngtwin 0.889, 20
   words), two listings up, PLUS ONE STATEMENT.  `*(s32 *)(b + 0x8) -=
   0x18000' written FIRST is the whole five-word delta -- `lui'/`ori' for the
   constant plus `lw'/`addu'/`sw' -- and nothing else changes but 0x2a -> 0x32
   and -1 -> -8.  No new declarations were needed.

   800249A0's ALLOCATION FACT CARRIED AT ITS OWN SIGN: the counter must be
   read into an `s16' local BEFORE the D_8002571C store, with the gate testing
   the LOCAL rather than reloading.  Folded into `*(s16 *)(p + 0x32) -= 8'
   with a reloading `<= 0' gate it is 25 words / 12 aligned differences.
   Both bodies are leaves with no saved registers, so the frame matched and
   the budget rule predicted the carry.  The same fact is FREE on
   func_s51_800248B8 in this file -- see that comment.

   THE VECTOR STATEMENT MUST COME FIRST.  Moved below the counter read it is
   25 words / 16 aligned differences: the `sw 0x8($a1)' then cannot be
   emitted before the `lhu 0x32' and the tick store's `lui' loses its hole.

   `s16 t' is load-bearing: as `s32' it is 23 words / 5 rows, losing the
   `sll 16'/`bgtz' pair -- 800249A0 records 19 words / 19 differing for the
   same substitution, so the direction reproduces and the magnitude does not.

   MEASURED AND FREE, so NOT the reason this lands: the pull written
   `+= 0xFFFE8000' instead of `-= 0x18000' (25 / 0).  Both give the same
   `lui'/`ori'/`addu'; the C89 signedness trap does not fire because the value
   never feeds a division, a shift or a comparison. */
void func_s51_80024734(u8 *p, u8 *b, u8 *c)
{
    s16 t;

    *(s32 *)(b + 0x8) -= 0x18000;
    t = *(s16 *)(p + 0x32) - 8;
    D_8002571C = 1;
    *(s16 *)(p + 0x32) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A 0x211 spawner: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset
   less 0x10, then park func_s51_80024734 as the step and
   func_s51_800244E4 as the draw, with `b` at the payload's +0x14, `d` at
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
   THIS COPY'S OWN CONSTANT: 0x1f -> 0xf at 2 of 3 sites, -0x10 -> -0x8 at 2 of 3 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_80024798(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s51_80024734;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0xf) + (s16)(dx - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0xf) + (s16)(dy - 0x8);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s51_800244E4);
        *(u32 *)(p + 0x8) = c;
    }
}

/* A spawn stepped one frame with a spin: spend one of the payload's +0x2A
   frames, push the renderable's scale pair at +0x1C/+0x1E out by 0x320 and
   its rotation at +0x1A by 0x190, raise the D_8002571C tick, and close the
   node when the counter runs out.  29 words, 0 differing.  No twin above
   0.717; read off the listing with func_s51_800249A0's teardown and
   func_b01_80170970's (csrc/ovl_dng_b01.c) scale-bump idiom.

   THE GATE'S LOAD MUST BE THE ARRAY REF `((s16 *)p)[0x15]', NOT
   `*(s16 *)(p + 0x2a)'.  As a cast it is 30 words / 11 aligned differences.
   `expr.c:5344' sets MEM_IN_STRUCT_P when the INDIRECT_REF's operand is a
   PLUS_EXPR, which an array ref on a pointer is and a cast is not;
   `true_dependence's last clause then drops the edge between this varying
   load and the fixed-address, non-struct `sh %lo(D_8002571C)', and reorg
   sinks the tick store into the `bgtz's delay slot.  The cast form ALSO
   loses the entry `addu $a1, $a0, $zero': with the gate's value no longer
   taking $a0, the parameter coalesces with its incoming register and every
   later reference reads $a0.  One spelling, two symptoms.

   THE THREE BUMPS ARE WRITTEN 0x1C, 0x1E, 0x1A AND EMITTED 0x1C, 0x1A, 0x1E.
   sched1 exchanges the last two load/add/store chains -- `memrefs_conflict_p'
   disambiguates the three constant offsets off $a2, so nothing pins them and
   the tie breaks the other way round.  THE LISTING'S STORE ORDER IS NOT THE
   SOURCE ORDER HERE.  All five other permutations, each the single remaining
   fault: 1C/1A/1E 10 rows, 1E/1C/1A 4, 1A/1E/1C 10, 1A/1C/1E 13,
   1E/1A/1C 13.

   MEASURED FREE HERE AND LOAD-BEARING 40 WORDS AWAY, both directions:
   * splitting the counter decrement into an `s16' local around the tick store
     is 29 / 0 here and is what func_s51_8002492C and func_s51_80024734 need
     (7 and 12 rows folded).  Both those bodies and this one have identical
     frames -- leaves, no saves -- so the save-count rule predicts nothing;
     what differs is that this body's tick store is already free to sink past
     the gate, so the split buys it nothing.
   * conversely the array-ref gate above is FREE on func_s51_8002492C (29 / 0)
     and worth 11 rows here.

   MEASURED AND FREE: the counter read-modify-write itself as
   `((s16 *)p)[0x15] -= 1' (29 / 0) -- only the GATE's load needs the flag;
   the three bumps as `((u16 *)c)[0xE]' array refs (29 / 0); and the three
   bumps as explicit `*(s16 *)(c + K) = *(u16 *)(c + K) + N' assignments
   (29 / 0). */
void func_s51_800248B8(u8 *p, u8 *b, u8 *c)
{
    *(s16 *)(p + 0x2a) -= 1;
    *(u16 *)(c + 0x1C) += 0x320;
    *(u16 *)(c + 0x1E) += 0x320;
    *(u16 *)(c + 0x1A) += 0x190;
    D_8002571C = 1;
    if (((s16 *)p)[0x15] <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same spawn stepped one frame, re-sending the renderable instead of
   spinning it: spend one of the payload's +0x2A frames, raise the D_8002571C
   tick, push the slot back through func_800478B8, and close the node when
   the counter runs out.  29 words, 0 differing.  The countdown-plus-call
   idiom is func_b01_801708E8's (csrc/ovl_dng_b01.c) and the teardown is this
   file's own landed func_s51_800249A0.

   THE COUNTER READ-MODIFY-WRITE MUST BE SPLIT AROUND THE TICK STORE, which
   is func_s51_800249A0's own fact at its own sign.  Folded into
   `*(s16 *)(p + 0x2a) -= 1' it is 29 words / 7 aligned differences: the
   target emits read, foreign store, write-back in that order, and that order
   cannot come from one compound assignment.  The tick store must sit BETWEEN
   the read and the write-back -- moved after the write-back it is also
   29 / 7, the same residue.

   func_800478B8(c) GOES BEFORE THE GATE.  Moved after it the body is 26 words
   / 19 aligned differences, which is b01's own stated fact in the same
   direction.

   MEASURED AND FREE, so NOT the reason this lands: the gate's load as the
   array ref `((s16 *)p)[0x15]' (29 / 0).  That spelling is worth 11 rows on
   func_s51_800248B8 40 words above, where the tick store has to sink past
   the gate; here the call already separates them, so the MEM_IN_STRUCT_P
   escape has nothing to buy. */
void func_s51_8002492C(u8 *p, u8 *b, u8 *c)
{
    s16 t;

    t = *(s16 *)(p + 0x2a) - 1;
    D_8002571C = 1;
    *(s16 *)(p + 0x2a) = t;
    func_800478B8(c);
    if (*(s16 *)(p + 0x2a) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_8002571C
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_8002571C store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_8002571C
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
   THIS COPY'S OWN CONSTANT: 0x32 -> 0x2a at all 2 sites, -0x8 -> -0x1.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s51_800249A0(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x2a) - 1;
    D_8002571C = 1;
    *(s16 *)(p + 0x2a) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The unit's general-purpose 0x212 decoration spawner, called twice from
   func_s51_80024B38's state 0: allocate the node, give it 0x3C frames at the
   payload's +0x2A, install func_s51_800249A0 as its step, raise the 0xC and
   0x80 bits in the slot's +0x14 with the caller's scale factor at +0x10
   between them, seed the mover from the caller's own record at p - 0x18, then
   -- through a RE-READ of n->unk_0C -- the caller's two scale halves, a
   0x80/0x80 pair with the caller's colour byte at +0xC, and the 12-byte
   record at `s' copied to the node's own +0x92 for the slot's +0x08 to point
   at.  RETURNS the payload pointer, which the caller parks at its own +0x64.
   82 words, 0 differing.

   NO USABLE TWIN: dngtwin's best is 0.805 at -15 words and the 0.919 donor
   dngnear names is a different shape.  It was read off the listing on this
   file's own landed func_s51_800243D8 skeleton.  EIGHT PARAMETERS, OF WHICH
   $a1 AND $a2 ARE NEVER READ -- that is not a mistake: the two call sites in
   asm/nonmatchings/ovl_dng_s51/func_s51_80024B38.s set $a0..$a3 and four
   stack words (1, 0x1000, 0x1000, 0x808080 at the first site), and the frame
   is -0x28 with four outgoing slots and five saves, so +0x38..+0x44 are
   incoming arguments 5 to 8.  The colour word arrives whole and only its low
   byte is stored.

   `r->unk_10 = e * 32' MUST SIT BETWEEN THE TWO `r->unk_14 |=' STATEMENTS.
   With the two `|=' adjacent gcc folds them to a single `ori 0x8C' and
   DELETES the first `sh': 77 words / 10 aligned differences.  The two
   plain-cast dodges -- `*(u16 *)((u8 *)r + 0x14) |= ...' on either the first
   or the second lvalue -- give the identical 77 / 10 with a bit-identical
   residue, so that is ONE fact and not three; MEM_IN_STRUCT_P is not the
   lever here, the intervening store is.

   THE GATE IS `if (n != NULL) { ...; return q; } return NULL;'.  The
   early-return form `if (n == NULL) return NULL;' collapses the test to a
   single `beqz' past the whole body and is 82 words / 6 aligned differences.
   RE-PRICED: that same change read 77 words / 10 rows while the flag fault
   above was still live, which is two faults adding, not one fact.

   THE SECOND `r = (UnkSlot *)n->unk_0C' RE-READ IS LOAD-BEARING: dropped, the
   body is 81 words / 25 rows.  `t = *(u8 **)(p - 0x18)' must be a local: with
   the three mover stores written through the inline dereference it is 86
   words / 12 rows, one reload per store.  The three colour bytes go 0xE, 0xD,
   0xC (ascending, 82 / 6) and the scale pair goes +0x1C then +0x1E (swapped,
   82 / 6).

   MEASURED AND FREE, so neither is why it lands: `e << 5' for `e * 32'
   (82 / 0 -- a power-of-two multiply is the same tree) and
   `r->unk_0C[0] = col' for `*((u8 *)r + 0xC) = col' (82 / 0). */
u8 *func_s51_800249F0(u8 *p, UnkVec *b, u8 *c, u8 *s, s32 e, s32 f, s32 g, s32 col)
{
    UnkNode *n;
    u8 *q;
    u8 *t;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2a) = 0x3C;
        n->unk_10 = (u32)func_s51_800249A0;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0xC;
        r->unk_10 = e * 32;
        r->unk_14 |= 0x80;
        t = *(u8 **)(p - 0x18);
        m = n->unk_08;
        m->unk_00 = *(s32 *)(t + 0x0);
        m->unk_04 = *(s32 *)(t + 0x4);
        m->unk_08 = *(s32 *)(t + 0x8);
        r = (UnkSlot *)n->unk_0C;
        r->unk_1C = f;
        r->unk_1E = g;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = col;
        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)s;
        r->unk_08 = (u32)((u8 *)n + 0x92);
        return q;
    }
    return NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s51", func_s51_80024B38);
