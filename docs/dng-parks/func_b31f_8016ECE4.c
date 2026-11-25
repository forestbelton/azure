/* PARK: func_b31f_8016ECE4 (ovl_dng_b31f) -- 117 words against a 118-word
   target / 8 ALIGNED DIFFERENCES.  Agent h173, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   WHAT IT DOES: func_b31f_8016EB14's 0x12 helper node again (read THAT
   function's landed comment in csrc/ovl_dng_b31f.c first -- this is the same
   spawner with a probability gate and a zeroed +0x4C..+0x60 block), gated on
   `(D_80013611 & 3) == 1' and a coin flip.  It installs
   func_b31f_8016E528 as the driver, parks the actor's +0x20 record at the
   new payload's +0x24, copies the mover with +0xA pulled 0x80 back, zeroes
   +0x4C..+0x60, stores the octant at +0x18, 0x1000 scale halves, the slot's
   +0x6 at six times the octant table's second column, 0x4D at the record's
   +0xAA, the /64 pair at +0x48/+0x49, the grey triple at 0x80, D_800777F4 at
   the slot's +0x8 with 0x80 raised in its flags, and 3/6/0/0 over
   +0x44..+0x47.

   THE RESIDUE IS ONE INSN AND IT IS THE MISSING WORD.  Rows 0 and 5..53 and
   55..117 match.  The target reads the gate byte at stream position 11 with
   a `nop' in its load-delay slot at 12; the candidate reads it at position 4
   where the six prologue saves then fill that slot, so the `nop' never
   appears and the body is one word short.  Everything else in the eight rows
   is the $v0/$v1 exchange between the two `lui's that follows from it, plus
   the realignment of the four zero stores.

   FACTS THIS BODY ESTABLISHES (all measured, all kept below):
   1. THE 8-BYTE `unused' LOCAL IS REAL AND IS WORTH A WORD AND 15 ROWS.
      Without `s32 unused[2]' the frame is -0x28 against the target's -0x30
      with the SAME five callee-saved registers in the same order, so the
      extra 8 bytes are neither a save nor an argument slot -- nothing in the
      body addresses $sp.  That is the brief's frame-delta rule read in
      reverse, and func_b31f_8016C720 in this unit carries the same device.
      117 words / 24 rows without it.
   2. `D_80013611' AS A DECLARED SYMBOL, NOT THROUGH THIS UNIT'S
      `g = (u8 *)0x80010000' CONSTANT BASE: the constant base is
      117 words / 9 rows, the declared symbol 117 / 8.  The symbol is real --
      splat/dungeon.bin_undefined_syms_auto.txt defines it -- and with ONE
      access to the 0x8001 block the base buys nothing, which is what
      func_b31f_8016DE1C in this unit already records.  So THIS BODY DOES NOT
      ADDRESS A CONSTANT BASE in its parked form.
   3. `w = (u8 *)n + 0x20' before func_8004491C and the four +0x4C..+0x60
      stores in ascending order: both carried from func_b31f_8016EB14, where
      they are priced at 2 words / 59 rows and 24 rows.

   MEASURED AND REJECTED:
   | spelling                                               | words | aligned |
   | as written below                                       |  117  |    8    |
   | the gate byte through `g = (u8 *)0x80010000'            |  117  |    9    |
   | no `s32 unused[2]'                                      |  117  |   24    |
   | the two tests nested instead of `&&'                     |  117  |    8    |
   | `1 == (D_80013611 & 3)'                                 |  117  |    8    |
   | `q' computed before `src'                               |  117  |    8    |
   | the parking slot read into its own local first          |  117  |    8    |
   | the mask through an `s32' local before `src'/`q'        |  117  |    7    |
   The last row IMPROVES and is NOT banked: the word count does not move, so
   by the brief's rule it is not evidence.

   WHAT I WOULD TRY NEXT: the question is only how to make the gate's `lbu'
   schedule just before its `andi'.  gcc 2.7.2's sched1 sorts the ready list
   by priority and breaks ties on INSN_LUID with the HIGHEST luid first
   (sched.c rank_for_schedule, read this batch), so the lever is anything
   that shortens the gate chain's priority or moves its luid later -- a
   statement after the gate that the gate's value feeds, or the gate read
   from a different expression.  Nine source orders did not move it.

   DECLARATIONS
   extern u8 D_80013611;
   extern u8 *D_b31f_80174704;
   extern s16 D_b31f_80174708[][2];
   extern u8 D_80045340[];
   extern u32 D_800777F4[];
   void func_b31f_8016E528();
   END DECLARATIONS */
void func_b31f_8016ECE4(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkVec *src;
    UnkSlot *r;
    u8 *q;
    u8 *w;
    s32 unused[2];

    src = *(UnkVec **)(D_b31f_80174704 + 0x8);
    q = D_b31f_80174704 + 0x20;
    if ((D_80013611 & 3) == 1 && (rand() & 1) == 0)
    {
        n = func_8003FC64(0x12);
        if (n != NULL)
        {
            w = (u8 *)n + 0x20;
            *(s16 *)(w + 0x12) = 3;
            n->unk_10 = (u32)func_b31f_8016E528;
            func_8004491C(n, (u32)D_80045340);
            r = (UnkSlot *)n->unk_0C;
            r->unk_14 &= 0xFFF3;
            *(u8 **)(w + 0x24) = q;
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src + 0xA) - 0x80;
            *(s32 *)(w + 0x4C) = 0;
            *(s32 *)(w + 0x50) = 0;
            *(s32 *)(w + 0x54) = 0;
            *(s32 *)(w + 0x60) = 0;
            *(s16 *)(w + 0x18) = (*(u16 *)(q + 0x2A) >> 9) & 7;
            r = (UnkSlot *)n->unk_0C;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            *(s16 *)((u8 *)r + 0x6) = D_b31f_80174708[(*(u16 *)(q + 0x2A) >> 9) & 7][1] * 6;
            *(s16 *)(q + 0xAA) = 0x4D;
            *(s8 *)(w + 0x48) = *(s16 *)((u8 *)src + 0x2) / 64;
            *(s8 *)(w + 0x49) = *(s16 *)((u8 *)src + 0x6) / 64;
            ((u8 *)r)[0xC] = ((u8 *)r)[0xD] = ((u8 *)r)[0xE] = 0x80;
            r->unk_08 = (u32)D_800777F4;
            r->unk_14 |= 0x80;
            w[0x44] = 3;
            w[0x45] = 6;
            w[0x46] = 0;
            w[0x47] = 0;
        }
    }
}
