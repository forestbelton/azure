/* func_s21_80025BA0 -- PARKED at 170/170 words, 16 differing (ovlverify masked),
   16 aligned differences (aligndiff).  2 copies in its family
   (tools/decomp/dngfam.py: func_s20_80026010 in ovl_dng_s20 is the twin stub,
   still an INCLUDE_ASM, so there was no landed sibling to read).

   WHAT IT IS: the 0x202 spawner for func_s21_800255E8's step and
   func_s21_800259DC's renderer, run once per live entry of the trail ring
   D_800265C8.  For each entry it averages the four ring corners -- entry i,
   the next entry in its 0x10-wide ring, and the two matching entries of the
   second bank at D_80026648 -- into the new mover's position, gives it a
   random velocity triple and a random spin, copies the four corners into the
   node payload at +0x34..+0x53, and picks the colour pair by i %% 0x20.  The
   loop clears D_80026476 on the way out.

   WHAT IS ALREADY EXACT: everything from the first ring load (word 44) to the
   end of the function (word 169) except six register names.  All 126 of those
   words agree, including the two D_80026648 folds, the i %% 0x20 arm order and
   the epilogue.

   THE RESIDUE IS TEN WORDS OF INDEX-BLOCK ORDER PLUS SIX REGISTER NAMES
   CARRIED BY IT.  Target words 30..43 are strictly serial -- a, b, j, j<<3,
   &t[j], k, k<<3, &t[k], b+0x10, l, l<<3, &t[l] -- i.e. each corner's whole
   address is finished before the next corner's index is started.  Every
   candidate measured interleaves the three chains instead, and the resulting
   live ranges swap two callee-saved registers: the candidate puts j<<3 in
   $s2 and &t[k] in $s3 where the target has them the other way round, which
   is words 49, 52, 54, 118, 119 and 121.

   The serial order is what sched1 gives when every insn in the block ties at
   max_priority and the tie breaks on LUID, so the address computations have
   to carry LUIDs INTERLEAVED with the index computations.  That says the
   source holds the three corners in POINTER locals rather than index locals.
   Measured: three pointer locals (q, u, w) assigned in that order is 172
   words and 153 differing -- it buys a tenth callee-saved register and the
   whole allocation moves -- so the shape is right and the spelling that pays
   for it without the extra register has not been found.

   EVERY SPELLING MEASURED, all at 170 words unless stated:

     a, b, c = b + 0x10, j = a + b, k, l = a + c   (this body)      16
     ... with the c statement anywhere after j                      36
     ... c = 0x10 + b  /  l = c + a                             16 / 24
     ... j and k statements swapped / l before j / b killed after   16
     ... D_80026648[j] in place of the written-out index            16
     a, b, j, k, b += 0x10, l = a + b                               19
     ... with that index block in any of five other orders          19
     ... and with the 14 locals declared in five different orders    19
     a, b, j, k, l = a + (b + 0x10)                                 34
     a, b, j, k, l = (i & -0x10) + (((i + 1) & 0xF) + 0x10)         34
     a, b, j, k, a += b + 0x10        (a holds l)                   27
     a, b, j, k, l = b + 0x10; l += a                               24
     a, b, j, k, l = a + 0x10; l += b                               22
     b, c, j = (i & -0x10) + b, k, l = (i & -0x10) + c              18
     a, b, j, k, l = a + b + 0x10                     169 words,   142
     a, b, j, k, b = a + b + 0x10     (b holds l)     169 words,   142
     three pointer locals for the corners             172 words,   153

   FOUR SPELLING FACTS THAT ARE MEASURED AND SHOULD BE KEPT:

   * THE THREE AVERAGES GO INTO s32 LOCALS AND ARE STORED AFTERWARDS.  Stored
     straight to the mover (`*(s16 *)(m + 0x2) = (...) >> 2;') the nine ring
     loads are not hoisted into one block and three load-delay nops appear:
     172 words, 71 aligned differences against 28 with the locals.  That one
     change is the difference between a body that is 2 long and one that is
     the right length.

   * `t = D_800265C8;' IS ASSIGNED INSIDE THE LOOP, not in the declaration.
     loop.c then hoists it into the preheader, which is where the target's
     `lui/addiu D_800265C8' sits -- AFTER the `blez'.  Assigned at the
     declaration it is emitted before the `blez' and the prologue is 8 words
     out of order.

   * THE SECOND BANK IS ITS OWN SYMBOL FOR THE TWO WORD COPIES AND THE
     POINTER LOCAL FOR EVERYTHING ELSE.  `*(u32 *)&D_80026648[i]' and
     `*(u32 *)&D_80026648[(i & -0x10) + ((i + 1) & 0xF)]' give the target's
     two `lui/addiu D_80026648' pairs and the `addu $s3, $s3, $t3' that
     reuses j<<3; `t[k]' and `t[l]' for the same two addresses fold the 0x80
     into a displacement off the biv (`lw 132($s3)') and lose six words:
     164 words, 102 aligned differences.

   * `k = i + 0x10' MUST BE AN INDEX LOCAL.  Written inline as `t[i + 0x10]'
     loop.c combines it with the `t[i]' giv and the address becomes the biv
     plus 0x80, which deletes the target's `addiu/sll/addu' triple.

   The two `%hi/%lo' pairs on D_80026648 are relocations, confirmed with
   `ovlverify --raw': raw 25 == 25 relocations (R_MIPS_26 on func_8003FC64,
   func_8004491C and rand five times, HI16/LO16 pairs on D_80026476 three
   times, D_800265C8, func_s21_800255E8, func_s21_800259DC, D_80026648 twice,
   D_800265C0 twice and D_800265C4 twice).

   func_s21_800255E8 and func_s21_800259DC both need NO declaration: both are
   defined above this function in ovl_dng_s21.c (func_s21_800259DC landed in
   this batch), so only D_80026648 and func_8003FC64 are new.

   Not a decomp-device residue: no device was tried, because the residue is a
   schedule and an allocation that follows it, and no plain-C device moves a
   whole block's LUID order.

DECLARATIONS
typedef struct UnkDng26478
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng26478;
extern UnkDng26478 D_800265C8[];
extern UnkDng26478 D_80026648[];
extern s16 D_80026476;
extern u32 D_800265C0;
extern u32 D_800265C4;
UnkNode *func_8003FC64(s32);
END DECLARATIONS */
void func_s21_80025BA0(void)
{
    UnkDng26478 *t;
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *p;
    s32 i;
    s32 a;
    s32 b;
    s32 j;
    s32 k;
    s32 l;
    s32 c;
    s32 x;
    s32 y;
    s32 z;

    for (i = 0; i < D_80026476; i++)
    {
        t = D_800265C8;
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s21_800255E8;
            func_8004491C(n, (u32)func_s21_800259DC);
            a = i & -0x10;
            b = (i + 1) & 0xF;
            c = b + 0x10;
            j = a + b;
            k = i + 0x10;
            l = a + c;
            m = (u8 *)n->unk_08;
            x = (t[i].unk_00 + t[j].unk_00 + t[k].unk_00 + t[l].unk_00) >> 2;
            y = (t[i].unk_02 + t[j].unk_02 + t[k].unk_02 + t[l].unk_02) >> 2;
            z = (t[i].unk_04 + t[j].unk_04 + t[k].unk_04 + t[l].unk_04) >> 2;
            *(s16 *)(m + 0x2) = x;
            *(s16 *)(m + 0x6) = y;
            *(s16 *)(m + 0xA) = z;
            *(u32 *)(m + 0xC) = ((rand() & 0xF) - 0x8) << 16;
            *(u32 *)(m + 0x10) = ((rand() & 0xF) - 0x8) << 16;
            *(u32 *)(m + 0x14) = ((rand() & 0xF) - 0x8) << 16;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x16) = (rand() & 0x1FF) - 0x100;
            *(s16 *)(r + 0x18) = (rand() & 0x1FF) - 0x100;
            p = (u8 *)n + 0x20;
            *(s16 *)(p + 0x1A) = 0x20;
            *(u32 *)(p + 0x34) = *(u32 *)&t[i];
            *(s16 *)(p + 0x38) = *(u16 *)&t[i].unk_04;
            *(u32 *)(p + 0x3C) = *(u32 *)&t[j];
            *(s16 *)(p + 0x40) = *(u16 *)&t[j].unk_04;
            *(u32 *)(p + 0x44) = *(u32 *)&D_80026648[i];
            *(s16 *)(p + 0x48) = *(u16 *)&t[k].unk_04;
            *(u32 *)(p + 0x4C) = *(u32 *)&D_80026648[j];
            *(s16 *)(p + 0x50) = *(u16 *)&t[l].unk_04;
            if (i % 0x20 < 0x10)
            {
                *(u32 *)(p + 0x54) = D_800265C0;
                *(u32 *)(p + 0x58) = D_800265C4;
            }
            else
            {
                *(u32 *)(p + 0x58) = D_800265C0;
                *(u32 *)(p + 0x54) = D_800265C4;
            }
        }
    }
    D_80026476 = 0;
}
