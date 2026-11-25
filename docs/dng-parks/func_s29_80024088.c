/* func_s29_80024088 -- PARKED at 81 words / 81 words / 3 aligned differences.
   dngfam: 1 member, 81 words, so this park is worth exactly 81 words.

   WHAT IT IS. The +0x10 step of the 0x212 node func_s29_800247B0 spawns (the
   payload is that body's q at n+0x20): bump the frame counter at +0x4, set
   the +0x8 amplitude to the payload's +0xA swing scaled by a triangle in the
   counter -- rising for the first 0x20 frames, falling after -- retire once
   the counter passes 0x40, otherwise ramp the renderable's +0xE byte up over
   the first 0x10 frames, hold it at 0x80 to 0x30 and run it negative after,
   and cycle the +0xC sprite number through 0x7DCF/0x7E00/0x7E01/0x7E02 on
   the counter mod 4.  A leaf: no frame, no saved registers.

   THE RESIDUE IS THREE ROWS AND ONE REGISTER NAME, with the word count and
   the whole instruction multiset already right:

     word 20   mflo $v0         target  mflo $a2
     word 21   lh   $v1, 0x4($a0)       srl  $v0, $a2, 16
     word 22   srl  $v0, $v0, 16       lh   $v1, 0x4($a0)

   i.e. the target keeps the mult's result (the local `t') in $a2 and shifts
   it into $v0, where this body coalesces the two into $v0; and the reload of
   p+0x4 for the retire gate sits one slot earlier here (filling the
   mflo -> srl latency) than in the target (which fills it with the +0x8
   store instead).  Everything before word 20 and after word 23 is identical.

   DIAGNOSIS, and it is the part worth keeping.  The target's allocation is
   w -> $v1, the sign-extended counter -> $a1, `t' -> $a2, the shifted value
   -> $v0.  This body gets w -> $v1, counter -> $a1, t AND the shifted value
   both -> $v0.  For `t' to be pushed out to $a2 the allocator must find $v0,
   $v1, $a0 ($a0 is the live parameter p) and $a1 all unavailable when it
   reaches t, which means t must be assigned AFTER the block-local temps that
   use $v0 -- i.e. t must have LOWER allocno priority than it has here.
   `allocno_compare' is floor_log2(n_refs) * n_refs / live_length:

     t written as ONE expression per arm (`t = (0x40 - x) * w;')
        gives t 3 refs and a range born at the mult -- the target's shape --
        but then reorg steals arm B's single `mult' into the branch delay
        slot, arm A no longer needs its `j', and the body comes out
        80 words / 11 rows.
     t written SPLIT in arm A (`t = 0x40 - x; t = t * w;')
        gives t 5 refs and a range covering arm A's `subu', which keeps the
        `j' and the 81 words -- but also keeps t's priority above the $v0
        temps, so t stays in $v0.

   So the two halves of the fault are coupled through t's reference count and
   NO ONE-AT-A-TIME SPELLING REACHES BOTH.  The missing piece is a spelling
   that gives t three references AND leaves arm A three instructions long, or
   that lowers w's/raises the counter's priority so reorg can take the `sll'
   for the delay slot in the three-reference shape.  (In the 80-word shape the
   registers come out w -> $a1 and counter -> $v1, exactly swapped from the
   target, and that swap is WHY reorg cannot use the `sll': the sll reads the
   register the intervening sign-extension writes.)

   MEASURED TABLE, all with aligndiff, both columns real compiles.  The first
   block is the 81-word family (3 rows, all of them the mflo register and the
   reload slot above); everything in it is FLAT at 3, which is the signature
   of a fault no source spelling in this family can move:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | KEPT: `w' local, arm A split, `(u32)t >> 16'          |   81  |   3  |
   |   ... t declared u32, no cast on the shift            |   81  |   3  |
   |   ... `t *= w;' for `t = t * w;'                      |   81  |   3  |
   |   ... arm A `t = w * t;'                              |   81  |   3  |
   |   ... `*(u16 *)(p + 0x8)' destination                 |   81  |   3  |
   |   ... `t' declared before `w'                         |   81  |   3  |
   |   ... `(u16)((u32)t >> 16)'                           |   81  |   3  |
   |   ... a separate local for the shifted value          |   81  |   3  |
   |       (tried s32 / u32 / s16 / u16, all 81 / 3)       |   81  |   3  |
   |   ... BOTH arms split (`t = x; t = t * w;')           |   81  |   3  |
   |   ... arm A as `t = 0x40; t = (t - x) * w;'           |   81  |   3  |
   |   ... gate written `> 0x1F'                           |   81  |   3  |
   |   ... the counter hoisted into its own `s16 k'        |   81  |   3  |
   |   ... the shift written `/ 0x10000'                   |   81  |   3  |
   |   ... `if (1)' round the +0x8 store                   |   81  |   3  |
   | arm A NOT split (one expression)                      |   80  |  11  |
   |   ... plus a separate local for the shifted value     |   80  |  11  |
   |   ... plus a separate local for the subtraction       |   80  |  11  |
   |   ... arm B operands swapped                          |   80  |  11  |
   |   ... the shift arithmetic (`t >> 16')                 |   80  |  11  |
   | arm A split, product in a SECOND local                |   80  |  11  |
   | gate polarity inverted (`< 0x20' first)               |   81  |   9  |
   |   (arms laid out in the other order, otherwise right)  |       |      |
   | no `w' local, the `<< 11' inline in both arms          |   85  |  14  |
   | `w' holding only the LOAD, `(w << 11)' in both arms    |   82  |   6  |
   | `w' split into two statements                          |   82  |  13  |
   | the +0x8 store inside BOTH arms, no `t' at all          |   82  |   3  |
   |   (the 3 rows are a DIFFERENT fault -- see below)      |       |      |
   | do { } while (0) round the `w' assignment              |   82  |  19  |
   | do { } while (0) round the +0x8 store                   |   82  |   5  |
   | do { } while (0) round the whole if/else                |   82  |  13  |
   | do { } while (0) round the retire gate                  |   85  |  13  |

   THE 82-WORD SHAPE IS A SEPARATE, FULLY DIAGNOSED FAULT and it is the one
   to read if this is picked up again.  With the +0x8 store written inside
   both arms (no `t'), every register matches the target and the only residue
   is that the target emits

        lh $v1, 0x4($a0)   /   sh $v0, 0x8($a0)

   in that order while this body emits the `sh' first and then needs a `nop'
   for the `lh''s load delay -- one word long, three rows.  The cause is
   PASS ORDER, not aliasing: cross-jumping runs in jump2, AFTER both
   schedulers, so with the store inside the arms the `sh' is in the ARM's
   basic block and the `lh' in the merge block and sched can never order them.
   Making either MEM an array ref is measured FREE here (`((s16 *)p)[2]' for
   the gate, `((s16 *)p)[4]' for the store, and both together: 82 / 3 in all
   five combinations), which rules MEM_IN_STRUCT_P out -- the two MEMs are
   `p + 4' and `p + 8' and memrefs_conflict_p already separates them.  Moving
   the store out of the arms into a local is what puts both in one block, and
   that is the 80/81-word family above.

   TWO FACTS FROM THIS BODY THAT DID LAND AND ARE WORTH CARRYING:

   * THE GATE ARMS ARE INVERTED relative to the obvious reading.  Writing
     `if (x < 0x20) rising else falling' lays the arms out the other way:
     20 rows in the 82-word shape, 9 rows in the 81-word shape with
     everything else already right.  The kept form is
     `if (x >= 0x20) falling else rising'.
   * `p[0xE] = p[0x4] * 8;' AND NOT `*(s16 *)(p + 0x4) * 8'.  The target's
     load is `lbu', which is convert.c narrowing the MULT to the byte
     destination through the UNSIGNED short type; the `s16' spelling keeps
     the `lh' and costs 3 words (83/20 against 86/15 in the same column --
     the two spellings were crossed with the gate polarity, and the byte form
     wins in both columns).
   * `u = *(s16 *)(p + 0x4) % 4 << 3;' AND NOT `% 4 * 8'.  Same convert.c
     rule with the opposite sign: the MULT narrows to the `s16' destination
     and emits `sll 16' / `srl 13' where `<< 3' is one `sll 3'.  Worth
     2 words.  (This is the levers file's "the LSHIFT_EXPR arm is guarded on
     TREE_UNSIGNED and MULT_EXPR's is not", confirmed on a third body.)
   * `s32 k' for the camera angle read, NOT `s16'.  `g->unk_C8' is an `s16'
     field; converting it to `s32' is the single `lh' the target has, while
     an `s16' local adds `lhu' + `sll 16' + `sra 16'.  (That one is from
     func_s29_800241CC in the same unit, which landed.)

   DECLARATIONS
   extern u32 D_800814A0;
   END DECLARATIONS

   D_800814A0 comes from common.h and needs no declaration in the unit; the
   line above is only so this file can be compiled on its own.  The body uses
   no other symbol: it is a leaf with no calls. */
void func_s29_80024088(u8 *p, UnkVec *b, u8 *c)
{
    s32 w;
    s32 t;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    w = *(s16 *)(p + 0xA) << 11;
    if (*(s16 *)(p + 0x4) >= 0x20)
    {
        t = 0x40 - *(s16 *)(p + 0x4);
        t = t * w;
    }
    else
    {
        t = *(s16 *)(p + 0x4) * w;
    }
    *(s16 *)(p + 0x8) = (u32)t >> 16;
    if (*(s16 *)(p + 0x4) >= 0x40)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    if (*(s16 *)(p + 0x4) < 0x10)
    {
        p[0xE] = p[0x4] * 8;
    }
    else if (*(s16 *)(p + 0x4) < 0x30)
    {
        p[0xE] = 0x80;
    }
    else
    {
        p[0xE] = -0x80 - (*(s16 *)(p + 0x4) - 0x30) * 8;
    }
    switch (*(s16 *)(p + 0x4) % 4)
    {
    case 0:
        *(s16 *)(p + 0xC) = 0x7DCF;
        break;
    case 1:
        *(s16 *)(p + 0xC) = 0x7E00;
        break;
    case 2:
        *(s16 *)(p + 0xC) = 0x7E01;
        break;
    case 3:
        *(s16 *)(p + 0xC) = 0x7E02;
        break;
    }
}
