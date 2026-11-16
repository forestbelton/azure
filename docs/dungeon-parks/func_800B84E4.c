/* PARK: func_800B84E4, 211 of 211 words, 10 aligned differences (ovlverify
   reports 11 masked).  ovl_dungeon.  Measured 2026-11-16.

   The fan/arc emitter func_800B835C calls: one POLY_GT4 per step of a
   `*(u16 *)(p + 0xE)'-step sweep, each quad spanning the previous step's
   screen point and the current one, with the u/v pair walked by rsin/rcos
   of the step angle.

   NEW DECLARATIONS NEEDED (all three typedefs and the one prototype are in
   the agent's report; they go in the block at the top of the unit, after
   UnkDungeonSPXY):

       typedef union UnkDungeonSPUV { s32 w; u8 b; } UnkDungeonSPUV;
       typedef struct UnkDungeonSP9 {...} UnkDungeonSP9;    /* scratchpad */
       typedef struct UnkDungeonGT4W {...} UnkDungeonGT4W;  /* POLY_GT4 */
       void func_8006671C(void *);

   The signature is the one docs/dungeon-parks/func_800B835C.c pins:
   `void func_800B84E4(s16 *, u8 *, u8 *, s32);'.  The first argument is read
   as `*(u16 *)a' / `*((u16 *)a + 1)' / `*(s32 *)a' -- all three loads in the
   listing are `lhu'/`lw', never `lh', so the reads are UNSIGNED even though
   the caller hands it an `s16 w[2]'.

   ------------------------------------------------------------------ residue

   ONE SCHEDULING DECISION IN THE PROLOGUE, plus the loop-top copy that
   follows from it.  Ten aligned rows, all between word 37 and word 52:

   * word 37: the target schedules `addu $fp, $a2, $zero' (the `ot' parameter
     copy) into the load-delay slot of `lbu $v0, 0xB($s3)'; this candidate
     leaves a `nop' there and the copy stays at the end of the first block.
   * word 42: because that copy is still before the branch, `reorg' fills the
     guard's delay slot with it.  The target, having nothing left before the
     branch, reaches into the FALL-THROUGH and takes `i = 0' instead -- so
     the target's preheader is three insns (andi dd / giv init / ang = i) and
     this candidate's is four.
   * words 46-48 / 49-52: the target emits the loop-top
     `sp->unk_70.w = sp->unk_78.w' copy as a serial `lw' / `nop' / `sw';
     sched1 here interleaves it into the first UV chain's load-delay slots.
     Everything from word 53 to word 190 is identical.

   Both halves are allocation/scheduling, not shape: every opcode, immediate
   and displacement in the body matches, and the only register difference
   left is which of two adjacent slots holds the `ot' copy.

   ---------------------------------------- what paid, in the order it paid

   175 -> 23 -> 16 -> 11 differing.

   * THE LOOP IS A `do'/`while' INSIDE AN EXPLICIT `if', with `dd' and `ang'
     as locals initialised INSIDE the if.  Worth 152 of the 175.  Written as
     `for (i = 0; i < *(u16 *)(p + 0xE); i++)' with `(u16)d' inline, gcc
     hoists the `andi' as a loop invariant and the parameter `d' survives in
     its own callee-saved register -- ten values for nine registers, so `ot'
     spills and the body carries an extra `lw $a0, 0x40($sp)' before
     func_8006658C.  With the explicit `if', `andi $s6, $a3, 0xFFFF' reads
     $a3 directly, `d' dies there, and `ot' gets $fp as in the target.
   * A POINTER LOCAL `Unk80083160 *g = &D_80083160;' USED ONLY IN THE TAIL.
     23 -> 16 and the last missing word.  The target's tail is
     `lui $t0, %hi / addiu $t0, %lo / lw $v0, 0x0($t0)' -- three words, which
     is reload REMATERIALISING a pseudo it could not give a hard register to
     (nine callee-saved registers are already spoken for).  Spelled
     `D_80083160.unk_00' the tail folds to `lui $v0 / lw %lo($v0)', two
     words.  The head still reads `D_80083160.unk_00' directly: cse folds
     that one because the def is in the same block.
   * HOISTING `*(u16 *)(p + 0x4)' INTO A LOCAL before the tpage store.  Read
     twice (once for the unconditional store, once inside the `if (dd)'
     arm), the second read is re-emitted as its own `lhu' and the function
     is one word long.
   * `i = 0;' AS THE SECOND STATEMENT INSIDE THE `if', not the first and not
     the last.  16 -> 11.  All six orderings of {dd, i, ang} were measured:
     dd,i,ang = 10 aligned / 11 masked; dd,ang,i = 10 aligned / 12 masked;
     ang,dd,i = 11; i,dd,ang = 16; i,ang,dd = 17; ang,i,dd = 17.  `i = 0'
     placed BEFORE the `if' is 16 and two words short.  The lever is that
     `ang = 0' written after `i = 0' is copy-propagated to `ang = i'
     (`addu $s1, $s5, $zero'), which is what the target has.

   -------------------------------------------- measured and rejected, no better

   All at 211 words unless noted, against the 10-aligned base:

   * declaration order of the locals: dd,i,ang / i,dd,ang / ang,dd,i all 10.
     The allocation does not move with declaration order here.
   * `ot' copied into a local at the top: 10.  `ot' declared `void *': 10.
   * `q' declared before `sp': 10.  `sp' assigned as a statement rather than
     an initialiser: 10.  Reading `q' AFTER the scratchpad stores instead of
     before: 208 words / 62.
   * the loop-top copy as a union AGGREGATE assignment (`sp->unk_70 =
     sp->unk_78;'): 10, byte-identical to the `.w' form.
   * the loop-top copy through `*(volatile s32 *)': 10.  A volatile store
     does not serialise against a non-volatile load -- sched.c's
     `true_dependence' needs BOTH sides volatile.
   * the loop-top copy via a temp local (`tp = sp->unk_78.w; sp->unk_70.w =
     tp;'): 14.
   * the loop-top copy moved after the first UV pair: 212 words / 11.
   * the loop-top copy as a 4-byte STRUCT assignment, both `struct {u16
     h[2];}' and `struct {u16 a, b;}': 214 words / 12 -- gcc expands both as
     two halfword moves, not one word move.
   * a SECOND pointer local `u8 *b = (u8 *)0x1F800070;' for the 0x70/0x78
     region, to break the alias disambiguation: 209 words / 98.
   * the whole scratchpad through `u8 *sp' with casts instead of the struct:
     206 words / 34.  The struct spelling is load-bearing.

   The alias route is a dead end and the reason is in the compiler: both
   references are MEM_IN_STRUCT_P at `(plus (reg) (const_int))' off the SAME
   pseudo, so `memrefs_conflict_p' proves them distinct, and the
   MEM_IN_STRUCT_P exemption in `true_dependence' (sched.c) only fires when
   one side is a FIXED address, which clears the dependence rather than
   creating one.  What is left is sched1's own priority order. */

/* ---- the new declarations this body needs, verbatim ---- */
/* A scratchpad word written whole and read back as its low BYTE and as the
   signed word -- the byte-wide face of the slot UnkDungeonSPW gives a
   halfword face to. */
typedef union UnkDungeonSPUV
{
    s32 w;
    u8 b;
} UnkDungeonSPUV;
/* The 0x1F800000 scratchpad as func_800B84E4 lays it out: the four UV
   scalars at +0x08..+0x17 and two packed screen pairs at +0x70 and +0x78,
   each the XY half of an 8-byte slot whose Z halfword is zeroed. */
typedef struct UnkDungeonSP9
{
    u8 pad_00[0x8];
    UnkDungeonSPUV unk_08;
    UnkDungeonSPUV unk_0C;
    UnkDungeonSPUV unk_10;
    UnkDungeonSPUV unk_14;
    u8 pad_18[0x58];
    UnkDungeonSPXY unk_70;
    s16 unk_74;
    u8 pad_76[0x2];
    UnkDungeonSPXY unk_78;
    s16 unk_7C;
} UnkDungeonSP9;
/* The same 0x34-byte POLY_GT4 as UnkDungeonGT4 above with its four screen
   pairs under their WORD face and ALL FOUR u/v pairs byte-addressed:
   func_800B84E4 copies each screen pair in with one `sw' and reads its own
   u0/v0 back a byte at a time. */
typedef struct UnkDungeonGT4W
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    Unk453E0UV unk_0C;
    u16 unk_0E;
    u32 unk_10;
    s32 unk_14;
    Unk453E0UV unk_18;
    u16 unk_1A;
    u32 unk_1C;
    s32 unk_20;
    Unk453E0UV unk_24;
    u16 unk_26;
    u32 unk_28;
    s32 unk_2C;
    Unk453E0UV unk_30;
    u16 unk_32;
} UnkDungeonGT4W;
/* func_800B84E4's per-primitive hand-off, beside func_8006658C's. */
void func_8006671C(void *);

void func_800B84E4(s16 *a, u8 *p, u8 *ot, s32 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSP9 *sp = (UnkDungeonSP9 *)0x1F800000;
    UnkDungeonGT4W *q;
    s32 i;
    s32 ang;
    s32 dd;
    s32 t;
    s32 tp;

    q = *(UnkDungeonGT4W **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_74 = 0;
    sp->unk_7C = 0;
    sp->unk_78.h[0] = *(u16 *)a + p[0xC];
    sp->unk_78.h[1] = *((u16 *)a + 1);
    sp->unk_08.w = p[0x8];
    sp->unk_0C.w = p[0x9];
    sp->unk_10.w = p[0xA];
    sp->unk_14.w = p[0xB];
    if (*(u16 *)(p + 0xE) != 0)
    {
        dd = (u16)d;
        i = 0;
        ang = 0;
        do
        {
            sp->unk_70.w = sp->unk_78.w;
            t = sp->unk_08.b + (sp->unk_10.w >> 1);
            q->unk_18.b[0] = t;
            q->unk_0C.b[0] = t;
            t = sp->unk_0C.b + (sp->unk_14.w >> 1);
            q->unk_18.b[1] = t;
            q->unk_0C.b[1] = t;
            i++;
            q->unk_30.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_30.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 13);
            ang += 0x1000;
            sp->unk_78.h[0] = *(u16 *)a + ((p[0xC] * func_80064584(ang / *(u16 *)(p + 0xE))) >> 12);
            sp->unk_78.h[1] = *((u16 *)a + 1) + ((p[0xD] * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 12);
            q->unk_08 = q->unk_14 = *(s32 *)a;
            q->unk_20 = sp->unk_78.w;
            q->unk_2C = sp->unk_70.w;
            q->unk_24.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_24.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_0E = *(u16 *)(p + 0x6);
            tp = *(u16 *)(p + 0x4);
            q->unk_1A = tp;
            if (dd != 0)
            {
                q->unk_1A = (tp & 0xFF9F) | ((dd - 1) << 5);
            }
            q->unk_04 = *(u32 *)(p + 0x10);
            q->unk_10 = *(u32 *)(p + 0x10);
            q->unk_1C = *(u32 *)(p + 0x14);
            q->unk_28 = *(u32 *)(p + 0x14);
            func_8006671C(q);
            if (dd != 0)
            {
                *((u8 *)q + 0x7) |= 2;
            }
            func_8006658C(ot, q);
            q++;
        } while (i < *(u16 *)(p + 0xE));
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}
