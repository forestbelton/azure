/* func_lo16_800161D8 -- ovl_dng_lo16, 33 target words.
   PARKED by agent h163, batch 35, 2026-11-25.

   BEST: 32 candidate words, 18 ALIGNED differences (aligndiff), which is
   ONE MISSING WORD and everything after it shifted by one.

   THE RESIDUE IS EXACTLY ONE INSTRUCTION, and it is a SECOND
   `addiu $v0, $zero, 0x6'.

   The target sets six halfwords of the grid descriptor at 0x8008333C:

       6  addiu $v0, $zero, 0x6
       7  addiu $a1, $zero, 0x3F
       8  sw    $ra, 0x10($sp)
       9  sh    $v0, 0x14($v1)
      10  addiu $v0, $zero, 0x6        <<< the word no spelling reproduces
      11  sh    $v0, 0x16($v1)
      12  addiu $v0, $zero, 0x180
      13  sh    $v0, 0x1C($v1)
      14  sh    $v0, 0x1E($v1)
      15  lui   $v0, %hi(D_800EA000)
      16  addiu $v0, $v0, %lo(D_800EA000)
      17  sh    $a1, 0x18($v1)
      18  sh    $a1, 0x1A($v1)
      19  sw    $a2, 0xA8($a0)

   0x3F is materialised ONCE for its two stores and 0x180 ONCE for its two,
   but 6 is materialised TWICE.  Every candidate materialises 6 once.

   WHAT IS KNOWN ABOUT THE MECHANISM.  The .rtl dump confirms the C does emit
   TWO `(set (reg:HI) (const_int 6))' insns and that CSE merges them: in
   probe.c.rtl there are two, in probe.c.cse one.  cse substitutes the
   register already holding 6 for the second constant (cse.c:1355 puts a
   register ahead of an equal-cost constant in the equivalence class) and flow
   then coalesces the copy away.  cse merges any two identical constants of the
   SAME MODE inside one basic block, and a cse block ends only at a CODE_LABEL
   (cse.c:8086), so the target's two sets must be either in two blocks or in
   two modes.  Neither is reachable from any spelling tried.

   A reload rematerialisation (`reg_equiv_constant' re-emitting the `li' at
   each use of a spilled pseudo) would explain it, but the body is nowhere near
   spill pressure -- the target's widest live set is five values ($a0, $a1,
   $a2, $v0, $v1) and the candidate's is four -- so that is ruled out unless
   something adds a fifth simultaneously live value.  Note that the target has
   0x3F live in $a1 across the 6 and 0x180 stores where the candidate shares
   one scratch register for all three constants: THE TARGET IS USING ONE MORE
   REGISTER THAN ANY CANDIDATE DOES.  Finding what that fifth value is may be
   the whole answer.

   EVERYTHING ELSE IN THE BODY IS RIGHT, and most of it was fought for:
   the three-insn address shape (`lui %hi(D_80083160)' / `addiu %lo' /
   `addiu 0x1DC') needs TWO POINTER LOCALS -- `b = (u8 *)&D_80083160' then
   `g = b + 0x1DC' -- because any spelling that names 0x1DC inside the address
   expression folds it into the R_MIPS_LO16 addend and loses a word.  That fact
   is the unit's central addressing idiom and it LANDED func_lo16_80016E6C
   (21/21, 0 differing).

   SPELLINGS MEASURED AND REJECTED, all at 32 candidate words:

     body as parked (pair order 14/16, 1C/1E, 18/1A)        32w / 30 ovlverify
     pair order 14/16, 18/1A, 1C/1E                         32w / 30
     pair order 14/16, 18, 1C, 16, 1A, 1E (interleaved)     32w / 31
     pair order 18/1A first, then 14/16, then 1C/1E         32w / 30
     pair order 18/1A, 1C/1E, 14/16                         32w / 30
     pair order 1C/1E, 14/16, 18/1A                         32w / 30
     pair order 1C/1E, 18/1A, 14/16                         32w / 30
     all six through `u8 *' casts, *(s16 *)(g + 0x14) = 6   32w / 30
     `*(volatile s16 *)&g->unk_14 = 6' (first only)         32w / 30
     both 0x14 and 0x16 stores volatile                     32w / 30
     `do { g->unk_16 = 6; } while (0)'                      32w / 32
     `s32 t = 6; g->unk_14 = t; g->unk_16 = 6'              32w / 28
     unk_A8 and unk_1DC stored FIRST                        32w / 32
     unk_1DC first, unk_A8 last                             32w / 30
     a `UnkDungeonCell *c = D_800EA000_s' local for +0x1DC  32w / 30
     `g->unk_00 = D_800EA000_s' instead of D_80083160.unk_1DC  31w / 27

   AND THE ONE THAT IS WORSE BUT MAY BE NECESSARY IN COMBINATION: nothing
   here moves the figure at all, which is this problem's signature.  A linear
   sweep cannot reach it; the next agent should look for the FIFTH LIVE VALUE
   rather than for another order of the six stores.

   DECLARATIONS
   extern u8 D_80080AA0;
   extern u8 D_800E3D20;
   extern u8 D_800E03EC[];
   typedef struct UnkDungeonCell
   {
       u16 unk_00;
       u16 unk_02;
       u16 unk_04;
   } UnkDungeonCell;
   extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
   typedef struct UnkDngLo16Grid
   {
       UnkDungeonCell *unk_00;
       u8 pad_04[0x10];
       s16 unk_14;
       s16 unk_16;
       s16 unk_18;
       s16 unk_1A;
       s16 unk_1C;
       s16 unk_1E;
   } UnkDngLo16Grid;
   void func_lo16_80018A70(void);
   void func_80099188(s32);
   END DECLARATIONS

   D_80083160 is `extern Unk80083160 D_80083160' in include/structs.h; its
   unk_A8 and unk_1DC are already declared there.
*/
void func_lo16_800161D8(void)
{
    u8 *b;
    UnkDngLo16Grid *g;

    b = (u8 *)&D_80083160;
    g = (UnkDngLo16Grid *)(b + 0x1DC);
    g->unk_14 = 6;
    g->unk_16 = 6;
    g->unk_1C = 0x180;
    g->unk_1E = 0x180;
    g->unk_18 = 0x3F;
    g->unk_1A = 0x3F;
    D_80083160.unk_A8 = 0x2C808080;
    D_80083160.unk_1DC = (u32)D_800EA000_s;
    func_lo16_80018A70();
    D_80080AA0 = 0;
    D_800E3D20 = 0;
    func_80099188((s32)D_800E03EC);
}
