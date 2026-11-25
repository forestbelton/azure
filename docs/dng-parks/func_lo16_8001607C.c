/* func_lo16_8001607C -- ovl_dng_lo16, 87 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 85 candidate words, 5 ALIGNED differences (51 under ovlverify, which
   is the two-word shift and its wake -- price this body with aligndiff).

   THE RESIDUE IS EXACTLY TWO INSTRUCTIONS, and both are RE-MATERIALISATIONS
   of a constant the candidate keeps in a register.  The target fills the
   second matrix (nine halfwords at D_80083160 + 0x50) like this:

       33 addiu $v1, $zero, -0x800
       34 addiu $v0, $zero, 0x800
       35 sh    $v1, 0x38($s0)        -0x800
       36 sh    $v1, 0x3C($s0)        -0x800
       37 addiu $v1, $zero, 0x800     <<< re-materialised
       38 sh    $v0, 0x3A($s0)        0x800
       39 addiu $v0, $zero, -0x800    <<< re-materialised
       40 sh    $v1, 0x3E($s0)        0x800
       41 sh    $v0, 0x40($s0)        -0x800
       42 sh    $v1, 0x42($s0)        0x800
       43-45 sh $zero, 0x44/0x46/0x48

   FOUR materialisations for six stores of two distinct values.  The candidate
   emits TWO and keeps -0x800 and 0x800 live in $v1/$v0 across all six, which
   is one word short per missing materialisation.  cse unifies two identical
   constants inside one basic block (there is no label and no call here), so
   no store ORDER reaches four materialisations: every order either groups the
   values (2 materialisations) or interleaves them (still 2, because cse keeps
   both registers live).  This is the same shape as the 800161D8 park's
   "6 is materialised TWICE" problem one function earlier in the same unit,
   and it is probably the same cause.

   EVERYTHING ELSE IS RIGHT.  The two pointer locals are the unit's central
   addressing idiom: `b = (u8 *)&D_80083160' then `q = b + 0x18', three insns
   (`lui %hi' / `addiu %lo' / `addiu 0x18'), and the LAST four stores go
   through `b' at +0x18..+0x1E while every other access goes through `q'.
   Six of the seven calls have real names out of common_syms.txt --
   SetGeomScreen, SetFarColor, SetColorMatrix, SetLightMatrix, SetBackColor,
   SetGeomOffset -- and the two matrix arguments are `b + 0x70' and
   `b + 0x50'.

   SPELLINGS MEASURED (candidate words / aligned rows):

     first matrix ascending 0x58..0x68, second ascending
       0x38..0x48                                         85w / 14
     first matrix GROUPED BY VALUE (0x58/0x5E/0x64 = n,
       then 0x5A/0x60/0x66 = -0x100, then the zeros),
       second as the listing emits it (0x38, 0x3C, 0x3A,
       0x3E, 0x40, 0x42)                                   85w / 5   <- kept

   So the first matrix's SOURCE ORDER IS GROUPED BY VALUE and that is worth 9
   rows; it is not the scheduler regrouping an ascending source.

   WHAT TO TRY NEXT: whatever breaks cse's constant unification for the second
   matrix without adding a word.  A `volatile' store would do it and is a
   device (combine.c:502 means a volatile MEM is never re-recognised), so
   price it only as a last resort and say so.  Before that: check whether the
   two 0x800/-0x800 pairs could be DIFFERENT TYPES in the source (a `s16'
   local assigned between the rows, a cast that makes one HImode and one
   SImode), since cse only unifies constants of the same mode.

   DECLARATIONS
   extern u8 D_801C9E40[];
   extern u8 D_801DA714_b[] __asm__("D_801DA714");
   extern u8 *D_80083478;
   extern u8 D_800DDC7C[];
   void func_80064624(s32, s32);
   END DECLARATIONS

   D_80083160 is `extern Unk80083160 D_80083160' in include/structs.h; every
   offset this body touches except +0xA8 lies inside its `pad_14[0x94]', so
   they are all reached through the `u8 *' cast.  D_801DA714 is `DRAWENV' in
   structs.h and the three stores are its r0/g0/b0; the `u8 *' alias is
   needed because a DECLARED-SIZE object folds the offset into the
   `R_MIPS_LO16' addend and costs a `lui' per store, where the target uses one
   `addiu %lo' base with three displacements.
*/
void func_lo16_8001607C(void)
{
    u8 *b;
    u8 *q;
    s32 n;

    b = (u8 *)&D_80083160;
    q = b + 0x18;
    n = 0x200;
    *(s32 *)(q + 0x88) = n;
    SetGeomScreen(0x200);
    *(s32 *)(q + 0x78) = 0;
    *(s32 *)(q + 0x7C) = 0;
    *(s32 *)(q + 0x80) = 0;
    SetFarColor(0, 0, 0);
    *(s16 *)(q + 0x58) = n;
    *(s16 *)(q + 0x5E) = n;
    *(s16 *)(q + 0x64) = n;
    *(s16 *)(q + 0x5A) = -0x100;
    *(s16 *)(q + 0x60) = -0x100;
    *(s16 *)(q + 0x66) = -0x100;
    *(s16 *)(q + 0x5C) = 0;
    *(s16 *)(q + 0x62) = 0;
    *(s16 *)(q + 0x68) = 0;
    SetColorMatrix((MATRIX *)(b + 0x70));
    *(s16 *)(q + 0x38) = -0x800;
    *(s16 *)(q + 0x3C) = -0x800;
    *(s16 *)(q + 0x3A) = 0x800;
    *(s16 *)(q + 0x3E) = 0x800;
    *(s16 *)(q + 0x40) = -0x800;
    *(s16 *)(q + 0x42) = 0x800;
    *(s16 *)(q + 0x44) = 0;
    *(s16 *)(q + 0x46) = 0;
    *(s16 *)(q + 0x48) = 0;
    SetLightMatrix((MATRIX *)(b + 0x50));
    *(s32 *)(q + 0x84) = 0x1000;
    func_80064624(0x1000, *(s32 *)(q + 0x88));
    SetBackColor(0xA0, 0xA0, 0xA0);
    D_801C9E40[0x19] = 0;
    D_801C9E40[0x1A] = 0;
    D_801C9E40[0x1B] = 0;
    D_801DA714_b[0x19] = 0;
    D_801DA714_b[0x1A] = 0;
    D_801DA714_b[0x1B] = 0;
    SetGeomOffset(0xA0, 0x78);
    *(s16 *)(b + 0x18) = -0xBC;
    *(s16 *)(b + 0x1A) = -0x88;
    *(s16 *)(b + 0x1C) = 0x172;
    *(s16 *)(b + 0x1E) = 0x19A;
    D_80083478 = D_800DDC7C;
}
