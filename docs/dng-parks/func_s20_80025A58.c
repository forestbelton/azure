/* PARK: func_s20_80025A58, csrc/ovl_dng_s20.c, agent-h13, 2026-11-23.

   TARGET 253 words. BEST BUILT 253 words, 7 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify also reports 7). 2 members, 253 words
   each, 506 words total (tools/decomp/dngfam.py: func_s20_80025A58,
   func_s21_800255E8).

   WHAT IT DOES. The +0x10 step of the 0x202 node func_s20_80026010 spawns:
   take the four corner records at the payload's +0x30..+0x4F, rebase them on
   the mover's own origin into the scratchpad quad at 0x1F800070, build a
   rotation-only matrix out of the slot's +0x16/+0x18 on the STACK, push it,
   run all four corners through func_80065320, pop, integrate the mover's
   velocity into its position, write the four rotated corners back onto the
   payload biased by the mover again, fade the six colour bytes at +0x50..+0x56
   by one 1/(+0x1A) step each, and when the +0x1A counter runs out raise bit
   0x8000 on the node flag at -0x2 and on D_800814A0.

   NO TWIN. tools/decomp/dngfam.py's only sibling is func_s21_800255E8, which
   is the identical body. An opcode-sequence sweep of every listing in
   asm/nonmatchings (difflib on the mnemonic stream) puts the best
   NON-family neighbour at 0.715 -- func_s56_800264D4, 318 words, itself still
   a stub -- and the best LANDED neighbour at 0.363, ovl_dungeon's
   func_8009E798. So this body is written from the listing, not relinked from
   anything. func_8009E798 did supply the addressing convention and it is
   worth keeping: a scratchpad STORE is `sp->unk_NN' through a struct pointer
   at 0x1F800000 and comes out `sh $v0, 0xNN($s2)', while a scratchpad
   ADDRESS PASSED AS AN ARGUMENT is the integer constant `(SVECTOR *)0x1F800070'
   and comes out `addu'/`ori' off the same hoisted `lui'. Spelling the stores
   as constant addresses too is 275 words and 258 differing, because each one
   then gets its own `lui'.

   THE RESIDUE IS SEVEN WORDS IN ONE WINDOW AND IT IS ONE sched1 CHOICE.
   Target indices 59..65 against the same indices here:

     target                              this body
     59 lhu $v0, %lo(D_s20_80026326)     59 lhu v0, 0x44(s4)
     60 lhu $v1, 0x44($s4)               60 lhu a0, 0xA(s0)
     61 lhu $a0, 0xA($s0)                61 lhu v1, %lo(D_s20_80026326)
     62 addiu $v0, $v0, 0x1              62 subu v0, v0, a0
     63 subu $v1, $v1, $a0               63 addiu v1, v1, 1
     64 sh $v0, %lo(D_s20_80026326)      64 sh v0, 0x84(s2)
     65 sh $v1, 0x84($s2)                65 sh v1, %lo(D_s20_80026326)

   The `D_s20_80026326++' chain and the `sp->unk_70[2].vz' chain are
   interleaved in the SAME window in both, in the same cycles, and only their
   order inside it differs -- which then decides which of them gets $v0.
   Every other word of the 253, including the whole `div'/`mflo' colour-fade
   block and the two abs-difference groups around it, is byte-identical.

   WHY IT IS A PURE TIE. The two chains are alias-INDEPENDENT:
   `sp->unk_70[2].vz' is an ARRAY_REF so MEM_IN_STRUCT_P is set and its
   address varies, while a -G0 global's `(lo_sum (reg) (symbol_ref))' address
   does NOT vary (rtlanal.c's LO_SUM case), so true_dependence's one escape
   applies and the increment is free to move across the quad stores. The
   target proves it moves: its `lhu' is ahead of the 0x44 load and its `sh'
   ahead of the 0x84 store, neither of which is source order for ANY position
   of the statement.

   THE STATEMENT'S POSITION IS THE ONLY LEVER I FOUND, AND IT IS QUANTISED BY
   GROUP. Sweeping `D_s20_80026326++' through the five slots of the four-
   statement vz block:

     before vz[0]                                               23
     between vz[0] and vz[1]                                    18
     between vz[1] and vz[2]                                    13
     between vz[2] and vz[3]                            THIS BODY, 7
     after vz[3]                               256 words, 198 differing

   At every position the chain is emitted ONE GROUP EARLIER than where it was
   written, so no position puts it first-in-the-0x84-window the way the target
   has it. At 13 the whole chain sits in the 0x7C window with the increment
   FIRST (the register pattern the target wants, one group too soon); at 7 it
   sits in the 0x84 window with the increment SECOND.

   MEASURED AND REJECTED, all at the best position unless noted:

     `D_s20_80026326 += 1' and `= D_s20_80026326 + 1'   identical RTL, 7
     a u16 local for the read, store in place                        7
     the local's read hoisted one group / two groups earlier         7
     the local's read early, the store one group later  252 words, 195
     `D_s20_80026326_a[0]++' through an `extern u16 [0x10]' alias
         (an ARRAY_REF, to put MEM_IN_STRUCT_P on the global)    13 / 7
     the counter sharing the `s16 n' local already in the body   13 / 7
     `c = D_...; __asm__("" : "=r"(c) : "0"(c)); D_... = c + 1'
         (a second set, to kill the birthing boost)  252 words, 204 / 198
     the four quad stores through constant addresses        275 words, 258
     the payload reads as `(*(u16 (*)[0x40])p)[...]' ARRAY_REFs      13 / 7
     the scratchpad quad as twelve flat `s16' members instead of
         `SVECTOR unk_70[4]'                                          7

   TWO SPELLINGS WERE WORTH ROWS AND ARE ALREADY IN THE BODY, recorded so the
   next reader does not undo them:

     the stack MATRIX's translation is zeroed t[2], t[1], t[0] -- DESCENDING.
       Ascending is 2 rows (`sw $zero, 0x2C/0x30/0x34($sp)' swapped).
     the colour fade is `p[0x50] -= p[0x50] / *(s16 *)(p + 0x1A);' six times
       with the divisor RE-READ each time; the six `lbu'/`lh'/`div'/`mflo'
       runs then hoist into one block ahead of the three remaining quad
       stores on their own, which is what the target has.

   WHAT I WOULD TRY NEXT. This is one cycle of sched1's backward scan inside a
   single basic block, with both chains at LAUNCH_PRIORITY from
   `adjust_priority's birthing boost and the tie falling to INSN_LUID. The
   target needs `sh $v0, 0x84($s2)' to win the last cycle of the window (so
   the 0x84 statement must be LATER in source, i.e. position 13's order) and
   `sh %lo(D_s20_80026326)' to win the one before it against that group's
   `subu' (so that `subu' must NOT be birthing, i.e. its destination needs a
   second flow-counted set). Those two requirements point at a device that
   gives the subtraction's temporary a second set without a scheduling
   barrier -- the matched-operand `asm' is the only free one in the
   catalogue, and the one I tried was on the counter, not on the
   subtraction. That is the untried experiment. decomp-permuter's statement
   and temporary passes cover the same ground.

   DECLARATIONS
   typedef struct UnkDng25A58Vec
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
       u8 pad_06[0x2];
   } UnkDng25A58Vec;
   typedef struct UnkDng25A58SP
   {
       u8 pad_00[0x70];
       SVECTOR unk_70[4];
       u8 pad_90[0x8];
       UnkDng25A58Vec unk_98[4];
   } UnkDng25A58SP;
   extern u16 D_s20_80026326;
   void func_800649A0(void);
   void func_80064A40(void);
   void func_80064CF0(MATRIX *);
   void func_80064D80(MATRIX *);
   s32 func_80065320(SVECTOR *, s32 *, s32 *);
   void func_80065820(SVECTOR *, MATRIX *);
   END DECLARATIONS

   D_800814A0 is already `extern u32' in include/structs.h and UnkVec and
   UnkSlot are already in it, so nothing else is needed. The scratchpad reads
   at +0x98 are u16 because the function reads them with `lhu'; the +0x70
   group is a real SVECTOR because it is stored only. */
void func_s20_80025A58(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng25A58SP *sp = (UnkDng25A58SP *)0x1F800000;
    SVECTOR rot;
    MATRIX m;
    s16 n;

    sp->unk_70[0].vx = *(u16 *)(p + 0x30) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[1].vx = *(u16 *)(p + 0x38) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[2].vx = *(u16 *)(p + 0x40) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[3].vx = *(u16 *)(p + 0x48) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[0].vy = *(u16 *)(p + 0x32) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[1].vy = *(u16 *)(p + 0x3A) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[2].vy = *(u16 *)(p + 0x42) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[3].vy = *(u16 *)(p + 0x4A) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[0].vz = *(u16 *)(p + 0x34) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[1].vz = *(u16 *)(p + 0x3C) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[2].vz = *(u16 *)(p + 0x44) - *(u16 *)((u8 *)v + 0xA);
    D_s20_80026326++;
    sp->unk_70[3].vz = *(u16 *)(p + 0x4C) - *(u16 *)((u8 *)v + 0xA);
    func_800649A0();
    rot.vx = s->unk_16;
    rot.vy = s->unk_18;
    rot.vz = 0;
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    func_80065820(&rot, &m);
    func_80064D80(&m);
    func_80064CF0(&m);
    func_80065320((SVECTOR *)0x1F800070, (s32 *)0x1F800098, (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800078, (s32 *)0x1F8000A0, (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800080, (s32 *)0x1F8000A8, (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800088, (s32 *)0x1F8000B0, (s32 *)0x1F800094);
    func_80064A40();
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    *(s16 *)(p + 0x30) = sp->unk_98[0].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x38) = sp->unk_98[1].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x40) = sp->unk_98[2].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x48) = sp->unk_98[3].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x32) = sp->unk_98[0].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x3A) = sp->unk_98[1].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x42) = sp->unk_98[2].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x4A) = sp->unk_98[3].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x34) = sp->unk_98[0].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x3C) = sp->unk_98[1].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x44) = sp->unk_98[2].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x4C) = sp->unk_98[3].unk_04 + *(u16 *)((u8 *)v + 0xA);
    p[0x50] -= p[0x50] / *(s16 *)(p + 0x1A);
    p[0x51] -= p[0x51] / *(s16 *)(p + 0x1A);
    p[0x52] -= p[0x52] / *(s16 *)(p + 0x1A);
    p[0x54] -= p[0x54] / *(s16 *)(p + 0x1A);
    p[0x55] -= p[0x55] / *(s16 *)(p + 0x1A);
    p[0x56] -= p[0x56] / *(s16 *)(p + 0x1A);
    n = *(u16 *)(p + 0x1A) - 1;
    *(s16 *)(p + 0x1A) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
