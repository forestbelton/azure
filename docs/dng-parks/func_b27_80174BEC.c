/* func_b27_80174BEC -- PARK.  479 words built, target 484 words, 105 aligned
   differences (tools/decomp/aligndiff.py).  ovlverify masked: 479/484, 373
   differing position-by-position -- read the aligned figure, not that one.

   WHAT IT IS.  The spawner for the two mover steps this agent landed in the
   same unit.  A four-state machine on p[0x9B] (a balanced compare tree over
   {0,1,2,3}; the unit owns no jump table and jtcheck agrees).  State 0 zeroes
   the UnkVec deltas, clears c + 0x14 0x6000 and FALLS THROUGH into state 1.
   State 1, gated on p + 0x96 being 0 or 7, runs two spawn loops: five 0x112
   nodes whose step is func_b27_80171040 and four 0x212 nodes whose step is
   func_b27_8017142C, each node getting a two-axis rand jitter, a table term
   out of a 0x20-byte stack copy of this unit's const D_b27_80170854, and a
   negative rand spin.  State 2 re-dresses off D_b27_801753BC.  State 3 is the
   post-decrement countdown that installs func_b27_80171E20 at p + 0x8C.

   THE STRUCTURE IS SETTLED AND IS NOT WHERE THE RESIDUE IS.  All four states,
   both loops, every call, every constant and both struct copies are read off
   the listing and agree; `relcand' is ok on the candidate (data symbols and
   call targets both audited).  The residue is FOUR allocation/placement faults
   and five missing words, listed below with what was measured.

   DONOR.  The in-unit landed func_b27_801737C8 (426 words, same file) is the
   idiom donor -- its `jx = (rand() & 0x3F) - 0x20;' pair, its
   `*(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)mv + 0x2) + jx;' read-modify-write
   spelling, its `UnkDngB27Name12' struct-copy idiom and its `>> 7 & 0x1C'
   table index all carried verbatim and none of them is in the residue.

   FAULT 1 -- FIVE MISSING WORDS, FOUR OF THEM `d' RELOADS.  The target spills
   the fourth parameter and RELOADS it at every use: between the two table
   reads it re-issues `lw $t1, 0x74($sp)' + `nop' (aligndiff rows 207-208 of
   the 481-word revision), and the same pair again in the second loop.  The
   candidate loads `d' once per loop into $t0 and keeps it, so those four words
   are absent.  `d' is written to its home slot 0x74 in BOTH, so the difference
   is that the target's pseudo is spilled by the allocator and the candidate's
   is not -- one unit of register pressure short.  The fifth word is not
   isolated.

   FAULT 2 -- THE WRONG LOOP INVARIANT OCCUPIES sp + 0x38 / sp + 0x3C.  Both
   frames are -0x68 with the same two 4-byte slots.  The target spills `&tbl'
   there (`addiu $t1, $sp, 0x18' / `sw $t1, 0x38($sp)' in each loop preheader)
   and recomputes the struct copy's bound `c + 0x30' inside the loop body.  The
   candidate does the opposite: it hoists `c + 0x30' into that slot and folds
   `sp + 0x18' into the table load's displacement (`addu $v0, $sp, $v0' /
   `lh $v0, 0x18($v0)' where the target has `addu $v0, $t1, $v0' /
   `lh $v0, 0x0($v0)').  This is loop.c's `move_movables' cost test picking a
   different movable, and it is the "A CONSTANT INSIDE A HOISTED ADDRESS IS
   UNREACHABLE BY ANY CAST ... only a VAR_DECL holding the row base keeps the
   addiu" lever, seen from the losing side.

   MEASURED AND REJECTED for fault 2: TWO `u8 *' POINTER LOCALS holding
   `tbl.b', one assigned in each loop preheader, which is the obvious way to
   force the VAR_DECL.  It OVERSHOOTS: 489 words / 149 rows, and the frame
   grows to -0x70 because the two pointers are allocated as new locals on top
   of the slots the target reuses.  The next attempt should look for a spelling
   that makes `&tbl' a movable WITHOUT adding frame -- a single pointer local
   reused by both loops, or `-da' `.loop' to read which movable won and by what
   margin (`threshold * savings * lifetime >= insn_count', threshold 29 then
   `threshold -= 3' per successful move, loop.c:1631/1718 -- the arithmetic the
   landed func_800453E0 comment in csrc/gprel_18B40.c works through for the
   same pass).

   FAULT 3 -- jx / jy HOLD EACH OTHER'S REGISTER ($s0 / $s1 exchanged against
   the target, uniformly, in BOTH loops), and ALLOCATION PRIORITY IS RULED OUT.
   All three declaration permutations measure the IDENTICAL 479 words /
   105 rows -- `jx; jy;' (kept), `jy; jx;', and `i;' moved above both -- which
   is this brief's test for "if all permutations measure identically, the cause
   is in the arithmetic or the statement order".  Statement order was then
   tried and is WORSE: exchanging the two `*(s32 *)(w + 0xA4) = jx << 12;' /
   `+ 0xA8 = jy << 12;' stores is 479 / 114.

   FAULT 4 -- $s3 / $s4 EXCHANGED IN THE SECOND LOOP ONLY (`w' and `r' hold
   each other's register there; the first loop's pair is correct).  Not
   separately priced; it is likely downstream of faults 1 and 2.

   THE -0x14 NARROWING WAS A REAL FAULT AND IS FIXED -- convert.c, and the
   figures are worth keeping because five spellings were priced from one body:

       *(s16 *)(mv+0xA) = (s16)(*(u16 *)(mv+0xA) - 0x14) + (rand()&0x1F)
                                                 479 / 105   <- KEPT
       ... = *(s16 *)(mv+0xA) - 0x14 + (rand()&0x1F)         479 / 109
       ... = *(u16 *)(mv+0xA) + (rand()&0x1F) - 0x14         479 / 109
       ... = (s32)(*(u16 *)(mv+0xA) - 0x14) + (s32)(rand()&0x1F)
                                                 481 / 113
       ... = (*(u16 *)(mv+0xA) - 0x14) + (rand()&0x1F)       481 / 113
       ... = *(u16 *)(mv+0xA) - 0x14 + (rand()&0x1F)         481 / 113  (first draft)

   The two 481-word rows emit `li $v1, 0xFFEC' + `addu' where the target has a
   single `addiu $v1, $v1, -0x14': the destination is narrower than int, the
   first operand is unsigned and there are three operands, so convert.c
   distributes the truncation into the sum and -0x14 becomes 65516, which is no
   `addiu' immediate.  **The documented remedy -- "the cast goes around the
   SUBTRACTION, not the load" -- is NOT sufficient here: `(s32)(load - 0x14)'
   is still 481 / 113.**  What works is the NARROW cast `(s16)(load - 0x14)',
   and a signed load is 4 rows worse than that rather than equal.  That is a
   correction to the lever as written in the batch brief and it was measured
   from one body with everything else held.

   OTHER FACTS MEASURED ON THIS BODY, all from the 481-word revision:
     * The 0x20-byte `D_b27_80170854' copy must be a BYTE-aligned struct
       (`UnkDngB27Name32 { u8 b[0x20]; }'): that is what gives the target's
       `lwl'/`lwr' + `swl'/`swr' groups of 3, 3 and 2 words.  A 2-aligned
       struct would emit halfword moves.
     * LOCAL DECLARATION ORDER SETS THE FRAME LAYOUT AND IS WORTH 32 ROWS.
       With `tbl' declared FIRST it lands at sp + 0x10 and `out[3]' above it;
       the target has `out[3]' at sp + 0x10 and `tbl' at sp + 0x18.  Declaring
       `tbl' LAST fixes it: 145 rows -> 113 rows, with the whole `swl'/`swr'
       block coming into agreement.
     * `*(s16 *)((u8 *)n + 0x20) = 0;' rather than `*(s16 *)w = 0;' -- the
       donor func_b27_801737C8's spelling, kept because the target's store is
       `sh $zero, 0x20($s0)' off the node and not off `w'.
     * The interposed `r->unk_10 = 0x60;' between `r->unk_14 |= 0xC;' and
       `r->unk_14 |= 0x2;' is LOAD-BEARING and is in the candidate: the target
       emits TWO `ori's, and without the store between them gcc merges the two
       `|=' on one lvalue into one.
     * func_800C857C is called with ONE argument in the candidate.  The `lw
       $a1, 0x60($t1)' before it is the `!= 0' test's own load, which is live
       before the call and used in the branch, so by the brief's discriminator
       it is not evidence of a second argument.  NOT separately measured.

   DECLARATIONS
   u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
   u8 *func_8003DE58(u8 *, u8 *, void *, s16);
   void func_8003DB94(void *, void *, s32);
   s32 func_8004491C(UnkNode *, u32);
   void func_800C857C(u8 *);
   extern const u8 D_b27_80170854[];
   extern u8 D_8017541C[];
   extern u8 D_800DE870[];
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   extern u8 D_b27_801753BC[];
   extern s16 D_80083228;
   extern s32 D_8008346C;
   extern u32 D_800814A0;
   s32 rand(void);
   UnkNode *func_8003FC64(s32);
   UnkNode *func_8003FD64(s32, void *);
   void func_80047784(u8 *, s32, s32);
   void func_800A2B04(UnkVec *, s32, s32);
   void func_800AD594(u8 *, s32);
   s32 func_800A56E0(s32);
   void func_b27_80171040(u8 *, UnkVec *, u8 *);
   void func_b27_8017142C(u8 *, UnkVec *, u8 *);
   void func_b27_80171E20(u8 *, UnkVec *, u8 *, u8 *);
   typedef struct UnkDngB27Name32
   {
       u8 b[0x20];
   } UnkDngB27Name32;
   typedef struct UnkDngB27Word48
   {
       u32 w[0xC];
   } UnkDngB27Word48;
   END DECLARATIONS

   dngfam.py: 1 copy -- a one-member family, so landing it lands it once. */
void func_b27_80174BEC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkSlot *r;
    UnkSlot *e;
    UnkVec *mv;
    u8 *w;
    s16 jx;
    s16 jy;
    s16 i;
    u16 out[3];
    UnkDngB27Name32 tbl = *(UnkDngB27Name32 *)D_b27_80170854;

    switch (p[0x9B])
    {
    case 0:
        *(s32 *)((u8 *)b + 0x14) = 0;
        *(s32 *)((u8 *)b + 0x10) = 0;
        *(s32 *)((u8 *)b + 0xC) = 0;
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        *(u16 *)(c + 0x14) &= 0x9FFF;
    case 1:
        if (*(s16 *)(p + 0x96) == 0 || *(s16 *)(p + 0x96) == 7)
        {
            if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
            {
                func_800A56E0(0x80D);
            }
            for (i = 0; i < 5; i++)
            {
                n = func_8003FD64(0x112, D_80083498);
                if (n != NULL)
                {
                    w = (u8 *)n + 0x20;
                    w[0x9B] = 0;
                    *(s16 *)(w + 0x96) = 0x28 - *(u16 *)(p + 0x96);
                    r = (UnkSlot *)n->unk_0C;
                    n->unk_10 = (u32)func_b27_80171040;
                    *(UnkDngB27Word48 *)r = *(UnkDngB27Word48 *)c;
                    r->unk_14 &= 0xFFFC;
                    func_8004491C(n, (u32)D_80045340);
                    *(u8 **)((u8 *)r + 0x2C) = D_8017541C;
                    func_80047784((u8 *)r, D_8017541C[0], 0);
                    mv = n->unk_08;
                    *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)b + 0x2);
                    *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)b + 0x6);
                    *(s16 *)((u8 *)mv + 0xA) = *(u16 *)((u8 *)b + 0xA);
                    e = *(UnkSlot **)(p - 0x14);
                    if (func_8003DF74((u8 *)e->unk_08, e, (s16 *)out, 1) != NULL)
                    {
                        *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)mv + 0x2) + out[0];
                        *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)mv + 0x6) + out[1];
                        *(s16 *)((u8 *)mv + 0xA) = *(u16 *)((u8 *)mv + 0xA) + out[2];
                    }
                    jx = (rand() & 0x1F) - 0x10;
                    jy = (rand() & 0x1F) - 0x10;
                    *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)mv + 0x2) + jx;
                    *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)mv + 0x6) + jy;
                    *(s16 *)((u8 *)mv + 0xA) = (s16)(*(u16 *)((u8 *)mv + 0xA) - 0x14) + (rand() & 0x1F);
                    *(s32 *)(w + 0xA4) = jx << 12;
                    *(s32 *)(w + 0xA8) = jy << 12;
                    *(s32 *)(w + 0xA4) =
                        (jx << 12) + (*(s16 *)(tbl.b + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)) << 19);
                    *(s32 *)(w + 0xA8) =
                        (jy << 12) + (*(u16 *)(tbl.b + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C) + 0x2) << 19);
                    *(s32 *)(w + 0xAC) = -((rand() & 0x7FFF) << 1);
                    *(s32 *)(w + 0xB0) = 0x1000;
                    r->unk_1E = 0x1000;
                    r->unk_1C = 0x1000;
                    r->unk_0C[2] = 0x80;
                    r->unk_0C[1] = 0x80;
                    r->unk_0C[0] = 0x80;
                }
            }
            for (i = 0; i < 4; i++)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    w = (u8 *)n + 0x20;
                    *(s16 *)(w + 0x24) = 0x19 - *(u16 *)(p + 0x96);
                    *(s16 *)((u8 *)n + 0x20) = 0;
                    n->unk_10 = (u32)func_b27_8017142C;
                    func_8004491C(n, (u32)D_80045340);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_14 |= 0xC;
                    r->unk_10 = 0x60;
                    r->unk_14 |= 0x2;
                    mv = n->unk_08;
                    *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)b + 0x2);
                    *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)b + 0x6);
                    *(s16 *)((u8 *)mv + 0xA) = *(u16 *)((u8 *)b + 0xA);
                    e = *(UnkSlot **)(p - 0x14);
                    if (func_8003DE58((u8 *)e->unk_08, (u8 *)e, out, 1) != NULL)
                    {
                        *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)mv + 0x2) + out[0];
                        *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)mv + 0x6) + out[1];
                        *(s16 *)((u8 *)mv + 0xA) = *(u16 *)((u8 *)mv + 0xA) + out[2];
                    }
                    jx = (rand() & 0x1F) - 0x10;
                    jy = (rand() & 0x1F) - 0x10;
                    *(s16 *)((u8 *)mv + 0x2) = *(u16 *)((u8 *)mv + 0x2) + jx;
                    *(s16 *)((u8 *)mv + 0x6) = *(u16 *)((u8 *)mv + 0x6) + jy;
                    *(s16 *)((u8 *)mv + 0xA) = (s16)(*(u16 *)((u8 *)mv + 0xA) - 0x14) + (rand() & 0x1F);
                    *(s32 *)(w + 0x60) = jx << 12;
                    *(s32 *)(w + 0x64) = jy << 12;
                    *(s32 *)(w + 0x60) =
                        (jx << 12) + (*(s16 *)(tbl.b + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)) << 19);
                    *(s32 *)(w + 0x64) =
                        (jy << 12) + (*(u16 *)(tbl.b + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C) + 0x2) << 19);
                    *(s32 *)(w + 0x68) = -((rand() & 0x7FFF) << 1);
                    *(s32 *)(w + 0x74) = 0x400;
                    r->unk_1C = 0x800;
                    r->unk_1E = 0x800;
                    r->unk_0C[1] = 0x80;
                    r->unk_0C[0] = 0x80;
                    r->unk_0C[2] = 0;
                    r->unk_12 = 0x7DCF;
                    r->unk_14 |= 0x100;
                    func_8003DB94(r, D_800DE870, 0);
                }
            }
        }
        *(u16 *)(p + 0x96) += 1;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        p[0x9B] += 1;
        if (*(u32 *)(d + 0x60) == 0)
        {
            break;
        }
        func_800C857C(d);
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        *(s32 *)((u8 *)b + 0x14) = 0;
        *(s32 *)((u8 *)b + 0x10) = 0;
        *(s32 *)((u8 *)b + 0xC) = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        if (*(u8 **)(c + 0x2C) == D_b27_801753BC)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_b27_801753BC;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s16 *)(p + 0x96) = 0x14;
        p[0x9B] += 1;
        break;
    case 3:
        if ((s16)((*(u16 *)(p + 0x96))--) > 0)
        {
            if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
            {
                break;
            }
        }
        func_800AD594(d, 0x1000);
        *(u32 *)(p + 0x8C) = (u32)func_b27_80171E20;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        break;
    }
}
