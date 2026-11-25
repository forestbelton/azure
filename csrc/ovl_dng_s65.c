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
extern u8 D_80045340[];
extern u8 D_8002533C[];
UnkNode *func_8003FC64(s32);
void func_s65_80024104(u8 *p);

extern s16 D_80025384;
extern u32 D_800814A8;
extern u8 D_s65_80024004[];
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
void func_s65_8002405C(u8 *p, UnkVec *v, UnkSlot *s);

/* func_s65_800243E4's spawn stepped one frame (the s58 twin
   func_s58_8002407C's shape with this object's own offsets): spend one of the
   payload's +0x2A frames, raise the D_80025384 tick, fade the renderable's
   colour bytes to the remaining life over the original at +0x2C (times 128),
   bleed +0xD down and +0x6 down, and when the counter runs out release the
   owner's busy-table slot at +0x46 and raise the two 0x8000 kill bits.
   42 words, 0 differing, first compile.  The twin is func_s58_8002407C
   (csrc/ovl_dng_s58.c, 0.976) and every one of its shapes held: the signed
   `c * 128 / divisor` as the fused `sll 16`/`sra 9`, the UNSIGNED `lhu`/`lbu`
   on each narrow read-modify-write against the signed `lh` for the separate
   `>= -0x6` test, and the busy-table release as the integer sum
   `index * 2 + base + offset`.  Only the offsets and one sign differ from
   that twin: this object's counter is +0x2A (its +0x2C), the original life
   +0x2C (its +0x2E), the slot index +0x32 (its +0x50), the owner +0x60 (its
   +0x7C), the busy table +0x46 (its +0x64) -- and the arithmetic colour byte
   is +0xD counting DOWN by 4 where the twin's is +0xE counting UP, with the
   two bytes assigned `k` being +0xE and +0xC here against its +0xD/+0xC. */
void func_s65_8002405C(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 c;
    s32 k;

    c = *(u16 *)(p + 0x2A) - 1;
    k = c * 128 / *(s16 *)(p + 0x2C);
    D_80025384 = 1;
    *(s16 *)(p + 0x2A) = c;
    *((u8 *)s + 0xD) -= 4;
    *((u8 *)s + 0xE) = k;
    *((u8 *)s + 0xC) = k;
    if (*(s16 *)((u8 *)s + 0x6) >= -0x6)
    {
        *(s16 *)((u8 *)s + 0x6) -= 2;
    }
    if (*(s16 *)(p + 0x2A) <= 0)
    {
        *(s16 *)(*(s16 *)(p + 0x32) * 2 + *(s32 *)(p + 0x60) + 0x46) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x212 spawner behind func_s58_8002553C's step, and the owner of this
   object's eight-slot busy table at +0x64.  Copy the 0x40-byte
   D_s65_80024004 record table onto the stack, raise the D_80025384 tick, scan
   the eight slots for a free one, and on finding it mark the slot, place the
   record through func_800B8FC8, spawn a 0x212 node with func_s65_8002405C as
   its step, clear and tint its renderable, seed its mover from the player
   record and copy the same 12-byte D_8002533C record func_s58_8002553C
   copies; then spend one frame of the owner's own +0x2C counter and raise the
   two 0x8000 kill bits when it runs out.  184 words, 0 differing.

   THE `i * 8` MUST BE COMPUTED INTO ITS OWN `s32` LOCAL BEFORE THE `buf +`:
   `buf + i * 8`, `(u8 *)&buf[i * 8]`, `(u8 *)buf + i * 8`, `i * 8 + buf` and
   `buf + (s32)i * 8` are all 184 words and 3 differing -- one aligned
   transposition, the `addiu $v0, $sp, 0x20` emitted ahead of the
   `sll $v1, $a0, 3` where the target has the shift first, which also swaps
   the pair's registers.  Written through a local the shift is emitted first
   and the pair matches; `k + buf` is identical to `buf + k`, so the operand
   order of the sum is NOT the lever, only whether the index is its own
   statement.  `(u8 *)(i * 8 + (s32)buf)` is 1 differing -- it fixes the
   order and loses the sum's own operand order instead.

   The 0x40-byte copy is a struct whose alignment is 1 (u8 b[0x40]), which is
   what makes expand_block_move emit the `andi 0x3` runtime alignment test and
   BOTH loops -- the `lwl`/`lwr` arm and the aligned `lw` arm.  The mover copy
   is three serialised aligned words for the same reason as in
   func_s58_8002553C.
   THIS COPY'S OWN CONSTANT: 0x64 -> 0x46 at all 2 sites, 0x2c -> 0x2a at all 2 sites, 0x2e -> 0x2c, 0x50 -> 0x32, 0x7c -> 0x60, 0x58 -> 0x92 at all 2 sites, 0x40 -> 0x7a at 1 of 3 sites, 0x41 -> 0x7b.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s65_80024104(u8 *p)
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

    *(UnkDngS58Blk64 *)buf = *(UnkDngS58Blk64 *)D_s65_80024004;
    D_80025384 = 1;
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
            n->unk_10 = (u32)func_s65_8002405C;
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
            *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_8002533C;
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

/* func_s65_80024104's own spawner, run as a step: allocate a 0x212 node, give
   it 0xB frames at the payload's +0x2C and the owner `a` at +0x7C, park
   func_s65_80024104 as its step, then clear the renderable -- 0x20 at +0x10,
   0 at +0x06, the 0xC bits in +0x14, a black colour and a zero scale -- seed
   its mover from the owner's name record and point the slot's +0x8 at the
   node's own +0x58, into which the 12-byte D_8002533C record is copied.
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
   THIS COPY'S OWN CONSTANT: 0xb -> 0x9, 0x2c -> 0x2a, 0x7c -> 0x60, 0x58 -> 0x92 at all 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s65_800243E4(u8 *a)
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
        *(s16 *)(p + 0x2a) = 0x9;
        *(u8 **)(p + 0x60) = a;
        n->unk_10 = (u32)func_s65_80024104;
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
        *(UnkDngS58Name12 *)((u8 *)n + 0x92) = *(UnkDngS58Name12 *)D_8002533C;
        r->unk_08 = (u32)((u8 *)n + 0x92);
    }
}

/* func_s65_80024104's spawn stepped one frame, the sibling of
   func_s65_8002405C two listings up and the same skeleton with no division:
   spend one of the payload's +0x2A frames, raise the D_80025384 tick, clamp
   the renderable's three colour bytes down to 0x40 once they exceed it, then
   fade all three by 5 a frame until they reach 0, and raise the two 0x8000
   kill bits when the counter runs out (this one releases no busy-table slot).
   39 words, 0 differing.  No twin at any ratio; the shape came off the
   listing with func_s65_8002405C, landed an hour earlier in this file,
   supplying the idiom and the symbols.

   BOTH LOCALS ARE LOAD-BEARING AND NEITHER IS IN THE 8002405C TWIN'S
   COMMENT.  `c` must be its own `s16` local: written in place as
   `*(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) - 1;` the body is still 39 words
   but 9 differing, because the tick's `%hi` then takes $a0, `p` keeps it
   instead of being copied to $a1, and the whole head reorders -- the
   target's opening `addu $a1, $a0, $zero` is the copy that costs.  And the
   fade's new value must go through the `u8` local `t`: spelled as the
   read-modify-write plus two same-base reloads
   (`*((u8 *)s + 0xE) -= 5; *((u8 *)s + 0xD) = *((u8 *)s + 0xE); ...`) cse
   forwards the stored value but regmove leaves a `move $v1, $v0` behind, for
   40 words / 18 differing. */
void func_s65_800244F0(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 c;
    u8 t;

    c = *(u16 *)(p + 0x2A) - 1;
    D_80025384 = 1;
    *(s16 *)(p + 0x2A) = c;
    if (*((u8 *)s + 0xC) > 0x40)
    {
        *((u8 *)s + 0xE) = 0x40;
        *((u8 *)s + 0xD) = 0x40;
        *((u8 *)s + 0xC) = 0x40;
    }
    if (*((u8 *)s + 0xC) != 0)
    {
        t = *((u8 *)s + 0xE) - 5;
        *((u8 *)s + 0xE) = t;
        *((u8 *)s + 0xD) = t;
        *((u8 *)s + 0xC) = t;
    }
    if (*(s16 *)(p + 0x2A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s65", func_s65_8002458C);
