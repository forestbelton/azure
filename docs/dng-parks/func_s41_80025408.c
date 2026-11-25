/* PARKED by agent h158, batch 36. 242 target words, 242 built words,
   4 aligned differences (7 unaligned). One family member (dngfam: 1).

   WHAT IT IS. ovl_dng_s41's 0x212 draw step for the shutter effect: with
   the payload's +0x0 state 0 it walks a 4x4 grid of 0x18 x 0x15 tiles
   starting at (0x340, 0x154), reads each tile out of VRAM with StoreImage
   into D_80026820, waits on DrawSync, INVERTS all 0x1F8 halfwords (0 ->
   0xFFFF, anything else -> 0), writes it back with LoadImage, and then
   bumps +0x0 and clears the renderable's 0x80 flag. State 0 FALLS THROUGH
   into state 1, which draws two framed panels -- func_800B8FC8 for the
   +0x44 payload and this unit's own func_s41_80024B6C for the +0x48 one --
   fades the renderable's three colour bytes up over the first 0x15 frames
   of +0x2 and down again from 0x51, and at 0x64 raises +0x0, clears +0x2,
   sets the +0x40 object's +0x9C and raises both 0x8000 kill bits. The tail
   is func_s41_800252E4's first gate: the +0x3C object's +0x13 byte in
   0x33..0x36 sets the renderable's 0x80 flag.

   THE RESIDUE IS ONE ALLOCATION DECISION, and the cause is named. The two
   RECTs' `w = 0x60' stores must NOT share a pseudo. cse's insert_regs
   (cse.c, the `GET_MODE (classp->exp) == GET_MODE (x)' test) puts two
   same-mode registers equivalent to the same constant in ONE quantity, so
   two HImode `0x60' stores become one pseudo with two uses; that pseudo
   crosses the func_800B8FC8 call, takes a callee-saved register, and every
   other constant in the block shifts one register along -- 15 aligned
   differences and 241 words. The target rematerialises 0x60 at each of the
   two stores, so in the target they are two pseudos.

   WHAT IS IN THIS FILE: an `s32 k' local for the SECOND store only. The
   mode difference blocks insert_regs' merge, which is the whole mechanism,
   and it measures 242 words / 4 aligned differences -- the remaining four
   rows are that `k' is a real pseudo in $a0 born six insns before its use
   where the target's is a reload rematerialisation in $v0 born immediately
   before it. A spelling that makes the second store a PLAIN CONSTANT STORE
   like the first would land this body; I did not find one.

   MEASURED AND REJECTED, all at 242 target words:

   | spelling                                                   | aligned |
   |------------------------------------------------------------|---------|
   | `s32 k' for store 2, x,y,w,h order        (this file)      |    4    |
   | `s32 k' for store 2, w,x,y,h order                         |    6    |
   | `s32 k' for store 1 instead                                |   11    |
   | `u32 k' for store 2                                        |    5    |
   | `u16 k' for store 2                                        |   10    |
   | `s16 k' for store 2 (same mode -- cse merges again)        |   13    |
   | both stores plain `0x60', w,x,y,h order                    |   15    |
   | both stores plain `0x60', x,y,w,h order                    |   20    |
   | both stores plain `0x60', y,x,w,h order                     |   23   |
   | both stores plain `0x60', x,y,h,w order                     |   24   |
   | both stores plain `0x60', w,h,x,y order                     |   27   |
   | `do { } while (0)' round group 2's four stores (cse barrier)|   32    |
   | `do { } while (0)' round both groups                        |   39   |
   | `k = 0x60' hoisted before the switch                       |   69    |
   | DIAGNOSTIC: `r1.w = 0x61' (two distinct constants)         |    3    |

   That last row is the proof of the mechanism rather than a candidate: with
   two DIFFERENT constants the two stores cannot merge, and the body is 242
   words with 3 rows, all of them the 0x61 itself.

   TWO FACTS THAT ARE NOT ALLOCATION AND THAT CARRIED THIS BODY, both worth
   more than the residue:

   * THE INVERSION LOOP MUST RUN AN EXPLICIT `u16 *q' CURSOR, not
     `D_80026820[k]'. The index form makes the giv's initialiser a
     loop-invariant movable, move_movables hoists it out of BOTH tile loops
     to the function level, it takes $fp, and the THIRD PARAMETER then
     spills to its incoming slot and is reloaded at all five of its uses:
     61 aligned differences and 11 extra words. The cursor is set twice
     inside the j-loop (its init and its increment), so n_times_set is 2 and
     loop.c never considers it a movable at all.
   * THE SWITCH VALUE IS READ INTO AN `s32' LOCAL BEFORE THE D_800267B8
     STORE. An `s16' local adds the `sll 16'/`sra 16' pair (the switch
     promotes it back); reading it after the store costs the `beqz' delay
     slot, because the store is to a symbol and the load is through a
     parameter and gcc 2.7.2's memrefs_conflict_p cannot disambiguate them.

   ALSO MEASURED HERE AND FREE: the `*(RECT *)' and `*(UnkDngS58Blk32 *)'
   casts on the two initialiser copies are what give the lwl/lwr pairs --
   both types are under 4-byte aligned, which is all movstrsi needs.

DECLARATIONS
extern u8 D_s41_80024004[];
extern u8 D_s41_8002400C[];
extern u16 D_80026820[];
extern s16 D_800267B8;
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
void func_s41_80024B6C(u8 *p, s32 *b, s32 *b2, s32 c, s16 e);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
END DECLARATIONS
*/
void func_s41_80025408(u8 *p, UnkVec *b, u8 *c)
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
    s16 u;

    r1 = *(RECT *)D_s41_80024004;
    r2 = *(RECT *)D_s41_8002400C;
    t = *(s16 *)p;
    D_800267B8 = 1;
    switch (t)
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
                StoreImage(&r2, (u_long *)D_80026820);
                while (DrawSync(1) != 0)
                {
                    ;
                }
                q = D_80026820;
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
                LoadImage(&r2, (u_long *)D_80026820);
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
        func_800B8FC8(*(u8 **)(p + 0x44), (s32 *)&r2, (s32 *)&w, 0, 1);
        r1.x = 0x340;
        r1.y = 0x100;
        k = 0x60;
        r1.w = k;
        r1.h = 0x54;
        w.vx = 0x370;
        w.vy = 0x12A;
        func_s41_80024B6C(*(u8 **)(p + 0x48), (s32 *)&r1, (s32 *)&w, 1, 1);
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
            *(s16 *)(*(u8 **)(p + 0x40) + 0x9C) = 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    n = *(u8 **)(p + 0x3C);
    if (n[0x13] >= 0x33 && n[0x13] <= 0x36)
    {
        *(u16 *)(c + 0x14) |= 0x80;
    }
}
