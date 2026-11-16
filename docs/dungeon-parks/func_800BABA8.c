/* LANDED (not a park): func_800BABA8, 215 of 215 words, 0 differing.
   ovl_dungeon, 2026-11-16.  Kept here only because the agent's working copy
   lives in /tmp; splice this into csrc/ovl_dungeon.c over the INCLUDE_ASM and
   delete the file.

   The 32-element fan of semi-transparent Gouraud quads a kind-0x202-style
   node carries at +0x10: each element's +0x12 phase byte advances by 4 a
   frame and wraps (or sticks at 0x30 while p[0x10] is set), the pair of
   RotTransPers input vectors is placed on a circle of radius
   func_80064584/func_800644B8 of `(i + (p->unk_0E << 6)) << 7', the three
   colour channels are gated by the slot's +0x14 flag word, and each visible
   element emits one 0x24-byte POLY_G4 plus the 0xC-byte primitive
   func_80067F20 fills behind it.

   NEW DECLARATIONS NEEDED:

       typedef struct UnkDungeonSPA {...} UnkDungeonSPA;   /* scratchpad */
       typedef struct UnkDungeonG4 {...} UnkDungeonG4;     /* POLY_G4 */
       s32 func_80065420(SVECTOR *, s32 *, s32 *, s32 *);
       s32 func_80069EF8();

   AND ONE EXISTING DECLARATION CHANGES: line 1352's `void func_800BABA8();'
   must become `s32 func_800BABA8();'.  The function ends
   `addu $v0, $zero, $zero' before the epilogue, which is a `return 0;', and
   its only use is `func_8004491C(..., (u32)func_800BABA8)' -- a cast to u32,
   so nothing else moves.

   ---------------------------------------- what paid, in the order it paid

   13 -> 12 -> 0 differing, 215 of 215 words from the first compile.

   * `s16 i = 0;' DECLARED AND INITIALISED IMMEDIATELY AFTER `g', with the
     loop written `for (; i < 0x20; i++)'.  12 -> 0, and it is the whole
     remaining residue.  The target sets `i' at word 8, between `$fp =
     &D_80083160' and the `lui' for 0x1F800090 -- ahead of all three constant
     pointer locals -- and that one position decides which of `sp' and `fb'
     owns the shared `lui 0x1F80' (`lui $s2' then `addu $s6, $s2' in the
     target; `lui $s6' then `move $s2, $s6' with `i = 0' left in the `for').
     `i' initialised in the `for', or declared `s16 i = 0;' after `q', are
     both 12.  All six declaration orders of {sp, fa, fb} were measured with
     `i' in the `for': s,a,b / a,s,b / a,b,s are 12 and s,b,a / b,s,a / b,a,s
     are 16, and none of them reaches 0 -- the lever is `i', not the pointers.
   * `*(p + i + 0x12)' AND NOT `p[i + 0x12]'.  13 -> 12.  The subscript form
     folds to `addu $v1, $v0, $s4' (index first); the target has
     `addu $v1, $s4, $v0', which is the same operand order its own
     `addu $s0, $s4, $s0' uses later in the body for the identical address.
   * The two out-parameters at +0x90/+0x94 are CONSTANT ADDRESSES IN POINTER
     LOCALS (`s32 *fa = (s32 *)0x1F800090;'), not members of the scratchpad
     struct.  That is what gives `lui`/`ori' once in the prologue and a
     one-word `addu $a2, $s7, $zero' at each of the two calls, where
     `&sp->unk_90' would be a two-word `addiu' per call.  Same device
     func_800B9144 uses for its `flag'.
   * The +0x12 phase byte is `u8' for the increment (`lbu'/`addiu'/`sb', with
     cse forwarding the stored value) and `(s8)' at every test and use
     (`sll 24'/`sra 24' on the forwarded value, and a real `lb' at the two
     later reads).  Both faces are in the listing and they are not
     interchangeable. */

/* ---- the new declarations this body needs, verbatim ---- */
/* The 0x1F800000 scratchpad as func_800BABA8 lays it out: the mover's three
   position halfwords at +0x00/+0x02/+0x04, the OT base at +0x20, the pair of
   RotTransPers input vectors at +0x70 and the OT index at +0xC0.  Its two
   out-parameters at +0x90/+0x94 are CONSTANT ADDRESSES in pointer locals,
   not members, which is what keeps them in callee-saved registers. */
typedef struct UnkDungeonSPA
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x1A];
    u8 *unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[2];
    u8 pad_80[0x40];
    s32 unk_C0;
} UnkDungeonSPA;
/* The 0x24-byte Gouraud quad func_800BABA8 emits -- libgpu's POLY_G4, with
   the two screen pairs RotTransPers writes as words and read back as the
   halfword UnkDungeonSPXY gives a face to. */
typedef struct UnkDungeonG4
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    u32 unk_0C;
    UnkDungeonSPXY unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 pad_17[0x1];
    s32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 pad_1F[0x1];
    UnkDungeonSPXY unk_20;
} UnkDungeonG4;
/* RotTransPers and the engine's random, the spellings csrc/ovl_town_800BEBA4.c
   uses. */
s32 func_80065420(SVECTOR *, s32 *, s32 *, s32 *);
s32 func_80069EF8();

s32 func_800BABA8(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g = &D_80083160;
    s16 i = 0;
    UnkDungeonSPA *sp = (UnkDungeonSPA *)0x1F800000;
    s32 *fa = (s32 *)0x1F800090;
    s32 *fb = (s32 *)0x1F800094;
    UnkDungeonG4 *q;
    s32 n;
    s32 cc;
    s32 z;
    s32 r;
    s32 gg;
    s32 bb;

    q = *(UnkDungeonG4 **)(D_80083160.unk_00 + 0x8D0);
    p[0x11] = 0;
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    sp->unk_04 = *(u16 *)((u8 *)b + 0xA);
    for (; i < 0x20; i++)
    {
        *(p + i + 0x12) += 4;
        if ((s8) * (p + i + 0x12) >= 0x30)
        {
            if (p[0x10] != 0)
            {
                *(p + i + 0x12) = 0x30;
                continue;
            }
            *(p + i + 0x12) = 0;
        }
        q->unk_04 = 0;
        q->unk_0C = 0;
        sp->unk_70[1].vx = sp->unk_70[0].vx =
            sp->unk_00 + ((func_80064584((i + (*(s16 *)(p + 0xE) << 6)) << 7) << 4) >> 11);
        sp->unk_70[1].vy = sp->unk_70[0].vy =
            sp->unk_02 + ((func_800644B8((i + (*(s16 *)(p + 0xE) << 6)) << 7) << 4) >> 11);
        z = sp->unk_04 - (*(s8 *)(p + i + 0x12) << 1);
        sp->unk_70[0].vz = z;
        sp->unk_70[1].vz = z + 0x40;
        n = func_80069EF8();
        cc = *(s8 *)(p + i + 0x12);
        if (cc > 0x20)
        {
            n = (s16)n / (cc - 0x20);
        }
        else if (cc < 0x10)
        {
            n = (s16)n / (0x10 - cc);
        }
        r = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x1) != 0)
        {
            r = n;
        }
        q->unk_14 = r;
        q->unk_1C = r;
        gg = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4) != 0)
        {
            gg = n;
        }
        q->unk_15 = gg;
        q->unk_1D = gg;
        bb = 0;
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x2) != 0)
        {
            bb = n;
        }
        q->unk_16 = bb;
        q->unk_1E = bb;
        sp->unk_C0 = func_80065420(&sp->unk_70[0], &q->unk_08, fa, fb);
        q->unk_10.w = q->unk_08;
        q->unk_10.h[0] += 4;
        sp->unk_C0 += func_80065420(&sp->unk_70[1], &q->unk_18, fa, fb);
        q->unk_20.w = q->unk_18;
        q->unk_20.h[0] += 2;
        sp->unk_C0 >>= 1;
        *((u8 *)q + 0x3) = 8;
        *((u8 *)q + 0x7) = 0x3A;
        func_8006658C(sp->unk_20 + sp->unk_C0 * 4, q);
        q++;
        func_80067F20((u32 *)q, 0, 0, 0x60, 0);
        func_8006658C(sp->unk_20 + sp->unk_C0 * 4, q);
        q = (UnkDungeonG4 *)((u8 *)q + 0xC);
        p[0x11]++;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}
