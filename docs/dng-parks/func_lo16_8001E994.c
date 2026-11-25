/* func_lo16_8001E994 -- ovl_dng_lo16, 68 target words.
   PARKED by agent h172, batch 35, 2026-11-25.

   BEST: 61 candidate words, 48 ALIGNED differences (66 under ovlverify).
   SEVEN WORDS SHORT, and they are accounted for exactly:

   FAULT 1 (4 words) -- THE SECOND READ OF THE ENTRY HALFWORD IS CSE'd AWAY.
   The target reads `*(u16 *)(D_80073414[i].unk_0C + j * 0x14)' TWICE: once at
   the top of the inner loop for the 0x10/0x40 flag tests, and again in the
   gated block for the 0x3000 field --

       31 lw   $v0, 0xC($t2)      <<< note $t2, NOT $a3
       33 addu $v0, $a1, $v0
       34 lhu  $v0, 0x0($v0)
       36 andi $v0, $v0, 0x3000

   and the candidate's second read is replaced by the first read's register,
   so `andi $v0, $v1, 0x3000' ends up in the gate's delay slot.  $t2 is a
   PREHEADER COPY of the outer record cursor $a3 (`addu $t2, $a3, $zero',
   target insn 14), i.e. the second read goes through a DIFFERENT pointer
   pseudo -- which is also why cse1 cannot equate the two MEMs in the target.
   Writing `s = &D_80073414[i];' as the first statement of the inner loop body
   produces that copy but does NOT stop the halfword read being forwarded.

   FAULT 2 (2 words) -- D_80012090 IS HOISTED OUT OF BOTH LOOPS.  The target
   reads it inside the inner loop, in the conditionally-executed arm of the
   `(v & 0x40)' test (`lui'/`lw' at insns 26-27, which loop.c will not move
   because the insn is not executed on every iteration).  The candidate hoists
   the load to the function head.

   FAULT 3 (1 word, FIXED) -- the signed division.  `((x & 0x3000) / 0x1000)'
   written on an `int' expression came out as a single `srl'; routing it
   through an `(s16)' cast -- `(s16)(x & 0x3000) / 0x1000' -- restores the
   target's `bgez' / `addiu 0xFFF' / `sra 12' bias sequence, which is +4 words
   and is already in the parked body.

   WHAT THE FUNCTION DOES.  For each of the 0x12 resident descriptors
   D_80073414[1..0x12] it walks that descriptor's +0x0C table (unk_02 entries
   of 0x14 bytes, starting at index 1), and for every entry whose halfword at
   +0 has 0x10 clear and either 0x40 clear or D_80012090 == 2, it adds a
   weight chosen by `(halfword >> 12) & 3' -- 0x80 for 0, 0x55 for 1, 0x20 for
   2, 1 otherwise -- to a running total that is NEVER RESET between
   descriptors, publishing the total into D_8001F6F8[i] after each descriptor
   and once more at D_8001F6F8[0x13] after the loop (through `i', not the
   literal 0x13: the listing recomputes `lui'/`addiu'/`sll'/`addu' rather
   than folding 0x26 into the %lo).

   SPELLINGS MEASURED (candidate words / aligned rows):

     `(v & 0x10) == 0 && ((v & 0x40) == 0 ||
       D_80012090 == 2)' as one gate, int division         54w / 53
     + a `Unk80073414 *s = &D_80073414[i]' local inside the
       inner loop for the second read, `(s16)' on the AND   58w / 48
     + the gate split into two `continue' statements and a
       `volatile u16' second read                          61w / 48  <- kept
       -- the `volatile' is a DEVICE and it is what finally makes the second
          read survive; it should be replaced, see below

   WHAT TO TRY NEXT.  The `volatile' in the parked body is a device and must
   not ship.  The real question is why cse forwards the first read across the
   `.A10' CODE_LABEL at all, when a cse basic block ends at a CODE_LABEL
   (cse.c:8086) -- the candidate's gate DOES branch to that label.  Read
   `-da' `.cse' against `.jump' on this body: if the second read is already
   gone in `.cse' the block boundary is not where it looks, and
   `cse_end_of_basic_block's skip-blocks arm is the thing to read.  Separately,
   for fault 2: the D_80012090 load must be in a conditional arm at loop.c
   time, so check `.loop' for whether the `||' collapsed the arm into
   straight-line code before loop.c ran.

   DECLARATIONS
   extern s16 D_8001F6F8[];
   END DECLARATIONS

   Unk80073414 (with unk_02 the +0x0C entry count and unk_0C the table
   pointer) and D_80012090 both come from include/structs.h.
*/
void func_lo16_8001E994(void)
{
    Unk80073414 *s;
    s32 i;
    s32 j;
    s32 acc;
    s32 k;
    s32 a;
    u16 v;

    acc = 0;
    for (i = 1; i < 0x13; i++)
    {
        for (j = 1; j < D_80073414[i].unk_02; j++)
        {
            s = &D_80073414[i];
            v = *(u16 *)(D_80073414[i].unk_0C + j * 0x14);
            if ((v & 0x10) != 0)
            {
                continue;
            }
            if ((v & 0x40) != 0)
            {
                if (D_80012090 != 2)
                {
                    continue;
                }
            }
            {
                k = (((s16)(*(volatile u16 *)(s->unk_0C + j * 0x14) & 0x3000)) / 0x1000) & 3;
                if (k == 0)
                {
                    a = 0x80;
                }
                else if (k == 1)
                {
                    a = 0x55;
                }
                else if (k == 2)
                {
                    a = 0x20;
                }
                else
                {
                    a = 1;
                }
                acc += a;
            }
        }
        D_8001F6F8[i] = acc;
    }
    D_8001F6F8[i] = acc;
}
