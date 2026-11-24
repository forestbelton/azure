/* PARK: func_b09_80170A84, csrc/ovl_dng_b09.c, agent-h11, 2026-11-23.

   TARGET 123 words. BEST BUILT 123 words, 6 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py). 2 members, 123 words each, 246 words total
   (tools/decomp/dngfam.py: func_b09_80170A84, func_b10_80170B0C).

   WHAT IT DOES. The +0x10 step of the 0x212 particle node that the landed
   func_b09_80170DDC spawns: integrate the payload's velocity at +0x40..+0x48
   into the mover and its acceleration at +0x4C..+0x54 into the velocity;
   kill the node when the mover has come within 0x10 of the spawn point
   recorded at +0x36/+0x38; fade the three colour bytes by
   p[i] * p + 0x32 / p + 0x34 into the slot's +0xC..+0xE, wind the slot's
   +0x1C/+0x1E scale down by 0xC8 with a floor of 0, drop the +0x32 life by
   8, re-register through func_800478B8 and kill the node when the life runs
   out. NO TWIN: xsibs.py's best offer is dungeon.bin:func_800D50FC at ratio
   0.658, which is a different function.

   THE RESIDUE IS SIX ROWS AND THEY ARE ALL ONE THING: the target emits
   `lh $v0, 0x36($s0)' BEFORE `lh $v1, 0x2($a3)' and this body emits them the
   other way round, in both abs-difference blocks. Rows, target index first:
   29/30 `sw $v0, 0x44($s0)' and `lh $v0, 0x36($s0)' against this body's
   30/33, and 42/43 `lh $v0, 0x38($s0)' / `lh $v1, 0x6($a3)' against 43/42.
   Every other word, including both `subu' operand orders and all of the
   velocity integration, matches.

   THE COUPLING THAT MAKES IT HARD, measured: the b + 0x2 load is
   alias-dependent on the three p stores (different base register, gcc 2.7.2
   cannot separate them) and the p + 0x36 load is not (same base, constant
   offsets, memrefs_conflict_p separates them). So the two loads' SOURCE
   ORDER decides both their output order and whether the p + 0x44 and
   p + 0x48 integration groups come out in source order:

     `u = *(s16 *)((u8 *)b + 0x2); t = *(s16 *)(p + 0x36) - u;'
         groups in source order, `subu' operands right, loads SWAPPED:  6
     `t = *(s16 *)(p + 0x36) - *(s16 *)((u8 *)b + 0x2);' inline
         loads right, GROUPS SWAPPED and `subu' operands swapped:      13
     `u = *(s16 *)(p + 0x36); t = u - *(s16 *)((u8 *)b + 0x2);'        16
     both operands in locals, p first                                 16
     `t = *(s16 *)(p + 0x36); u = ...b + 0x2; t -= u;'                24
     `t = *(s16 *)(p + 0x36); t -= *(s16 *)((u8 *)b + 0x2);'          24
     b hoisted for the first block, inline for the second              7
     inline for the first block, b hoisted for the second             17
     b hoisted and a separate t2 for the second block                  7
     inline and a separate t2 for the second block                    13
     `u' declared s16 instead of s32                                  13
     the b + 0x2 load hoisted above the integration group  120 words, 35
     the b + 0x2 load hoisted to the top of the body       120 words, 35
     both b loads hoisted to the top                       119 words, 39
     both b loads hoisted above the integration group      119 words, 31

   ONE ROW WAS BOUGHT BY A STATEMENT SPLIT AND IT IS WORTH RECORDING:
   `k = *(u16 *)(c + 0x1C); k -= 0xC8;' is 13 against the single statement
   `k = *(u16 *)(c + 0x1C) - 0xC8;'s 15, because it gives the loaded value
   and `k' one quantity -- the target's `lhu $a0' / `addiu $v0, $a0' /
   `addu $a0, $v0' instead of `lhu $v0' / `addiu $v0, $v0' / `move $a0, $v0'.
   `k' must stay s16: an s32 `k' with an `(s16)' cast on the test is 25, and
   `(s16)(... - 0xC8)' into an s16 is 15.

   MEASURED AND REJECTED on the integration group, ALL 123 words and ALL
   BYTE-IDENTICAL to each other, which is why none of them is the lever:
   both triples through `((UnkVec *)(p + 0x40))->unk_00' style members; the
   destination side only; the source side only; the first group's p reads
   only; an `((s32 *)p)[0x10]' array view on either group; a local
   `UnkVec *q = (UnkVec *)(p + 0x40)' (124 words, 30); `d0/d1/d2' locals for
   the three accelerations (17); `a0/a1/a2' accumulators (6, no change);
   the accumulators split into load-then-add (27); the three statements
   written longhand; `*(s32 *)((u8 *)b + 0x0)' casts for the mover. All six
   permutations of the integration group's statement order were measured:
   source order 6, the other five 16 to 24. `do { } while (0)' on each of
   the four candidate statements: 11, 34, 17 and 6.

   WHAT I WOULD TRY NEXT. Everything here is ONE scheduling choice inside a
   single basic block, which is what decomp-permuter's statement and
   temporary passes are for. The body below is the 6-difference one.


   PERMUTER RUN, 2026-11-23, 45 minutes / 333,405 iterations / 8 jobs:
   score 455 -> 20, which measures as 6 ALIGNED DIFFERENCES -> 3. Not a
   landing, but the residue is halved and the remaining three rows are one
   window: the target has `lh $v0, 0x38($s0)' / `lh $v1, 0x6($a3)' /
   `subu $v0, $v0, $v1' and the candidate has the same three loads in the same
   ORDER with $v0 and $v1 exchanged, so what is left is register allocation in
   a three-instruction window, not scheduling and not instruction selection.

   WHAT IT FOUND, and it is worth reading before trying again. Two changes
   from the body above:

     * a pointer local for ONE of the three velocity stores --
       `new_var = p + 0x44;' then `*(s32 *)new_var += ...' -- which is what
       fixes the FIRST abs block's load pair;
     * a DUPLICATED ARM on the first abs, under an always-true guard:
       `if (p || k) { t = -t; } else { t = -t; }'. `p' is a non-null
       parameter, so the condition is a predictable true and both arms are
       identical; this is the duplicated-arm device, not a semantic change.

   Removing that device is a syntax error as the permuter left it (its `if
   (t < 0x10)' lost a brace), so the device has NOT been priced on its own --
   that is the first thing to measure next.

   WHAT I TRIED AFTER IT, all on the candidate, none better than 3:
     the second block as `t = p+0x38; t -= b+0x6;'              13
     the second block through its own `u' local, then `t = u - b+0x6'  3
     both halves of the second block in locals, `t = u - k'           3

   So the second block does not respond to the treatment that fixed the
   first, which is the same asymmetry the table above records -- the b + 0x2
   load is alias-dependent on the three p stores and the b + 0x6 load is not.

   DISPATCHER PASS, 2026-11-23, no permuter -- just ovlverify on a ladder.
   TWO CLAIMS ABOVE ARE WRONG and the mechanism is now known exactly.

   (A) THE POINTER LOCAL IS WORTH NOTHING. The note above credits
   `new_var = p + 0x44' with fixing the first abs block's load pair. It does
   not. With the duplicated arm present, the body measures 3 WITH the pointer
   local and 3 WITHOUT it; with the arm removed, 13 either way. The device was
   doing all ten rows and the pointer local none of them.

   (B) THE DEVICE IS A REFERENCE BUMP ON `k' AND NOTHING ELSE:

       if (p || k) { t = -t; } else { t = -t; }     3   (the permuter's)
       if (k)     { t = -t; } else { t = -t; }      3   <- k alone suffices
       if (p)     { t = -t; } else { t = -t; }     13   <- p is irrelevant
       if (1)     { t = -t; }                      13
       do { t = -t; } while (0);                   13
       t = 0 - t;                                  13
       t = -t + (k - k);                           13

   So it is not the duplicated arm, not the predictable branch and not the
   block structure: it is ONE MORE FLOW-COUNTED USE OF `k', and only of `k'.
   `if (1)' and `do { } while (0)' -- the two macro-residue shapes -- add no
   reference and buy nothing, which is a useful negative result for a body
   whose residue is an allocno competition.

   NATURAL SPELLINGS THAT ADD THAT USE, all measured, none as good:

       t = k - *(s16 *)(p + 0x36);                 10
       t = k; t -= *(s16 *)(p + 0x36);             10
       q = p+0x36 named, t = q - k, abs t = k - q  10
       the same with q named after k               10
       t = t < 0 ? k - *(s16 *)(p + 0x36) : t;     15

   The reversed subtraction is the semantically honest way to write the abs
   and it does add the use, but it changes the INSTRUCTION -- a real `subu'
   with swapped operands where the target negates -- so it trades ten rows
   for three. Naming the `p + 0x36' load as well does not recover them, which
   rules out a reload as the cause.

   (C) THE SECOND BLOCK DOES RESPOND, which the note above denies. It was
   tried with locals only. The same guard on the second abs takes the body
   from 3 to 2:

       first block guarded, second plain                3
       second block given its own `s16 m'               3   (m vs k: no effect)
       first and second BOTH guarded, shared k          2
       first and second BOTH guarded, own m             2

   WHERE THAT LEAVES IT. Two aligned differences, and the price is TWO
   duplicated-arm guards. docs/landing-a-function.md's rule is that a device
   is a last resort, and two of them in a 123-word body for a result that is
   still not a landing is not a trade worth carrying -- so THE BODY BELOW IS
   STILL THE CLEAN 6-DIFFERENCE ONE, deliberately, and the 2-difference
   version is this table rather than code.

   WHAT I WOULD TRY NEXT, now that the axis is known: the goal is one more
   flow-counted use of `k' that costs no instruction. Nothing in the source
   wants one, so the remaining candidates are on the OTHER side of the
   competition -- lower the pseudo `k' is losing to rather than raise `k',
   which is the lever docs/matching-levers.md records for func_s56_80024388
   (merging two returns to DROP a reference). Read `-da`'s .lreg/.greg to
   name that pseudo first; I did not, and that is the next step rather than
   another sweep of spellings.

   The candidate's source is nonmatchings/func_b09_80170A84/output-20-1/.
   I stopped here deliberately: this is a 123-word function with one sibling,
   246 words of yield, and 1,433 stubs remain. It is a better park than it
   was, not a landing.

   DECLARATIONS
   END DECLARATIONS

   It needs nothing new: void func_800478B8(u8 *) is already in
   csrc/ovl_dng_b09.c's block, D_800814A0 is in include/structs.h, and
   void func_b09_80170A84(u8 *, UnkVec *, u8 *) is already declared there for
   the landed func_b09_80170DDC. */

void func_b09_80170A84(u8 *p, UnkVec *b, u8 *c)
{
    s32 t;
    s16 k;
    s32 u;

    b->unk_00 += *(s32 *)(p + 0x40);
    b->unk_04 += *(s32 *)(p + 0x44);
    b->unk_08 += *(s32 *)(p + 0x48);
    *(s32 *)(p + 0x40) += *(s32 *)(p + 0x4C);
    *(s32 *)(p + 0x44) += *(s32 *)(p + 0x50);
    *(s32 *)(p + 0x48) += *(s32 *)(p + 0x54);
    u = *(s16 *)((u8 *)b + 0x2);
    t = *(s16 *)(p + 0x36) - u;
    if (t < 0)
    {
        t = -t;
    }
    if (t < 0x10)
    {
        u = *(s16 *)((u8 *)b + 0x6);
        t = *(s16 *)(p + 0x38) - u;
        if (t < 0)
        {
            t = -t;
        }
        if (t < 0x10)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
    c[0xC] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    c[0xD] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    c[0xE] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    k = *(u16 *)(c + 0x1C);
    k -= 0xC8;
    if (k < 0)
    {
        k = 0;
    }
    *(s16 *)(c + 0x1C) = k;
    *(s16 *)(c + 0x1E) = k;
    *(s16 *)(p + 0x32) = *(u16 *)(p + 0x32) - 8;
    func_800478B8(c);
    if (*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
