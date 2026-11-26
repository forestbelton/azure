/* PARK: func_b31f_80171A10 (ovl_dng_b31f) -- 171 words / 172 words /
   40 ALIGNED DIFFERENCES.  Agent h186, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   WHAT IT DOES: this bank's 0x211 burst spawner, five parameters.  Copy the
   0x20-byte direction table D_b31f_8016A894 into a local, allocate the node
   with func_8003FD64(0x211, p), install func_b31f_801718E4 as its +0x10
   step, copy the caller's mover's +0x2/+0x6/+0xA halfwords (the +0xA one
   pulled back by 0x14), nudge +0x2 and +0x6 by the table entry times 16,
   put 6 in the slot's +0x6, seed the mover's +0xC/+0x10 velocities from
   rand with the shift picked by the fifth parameter (7 when it is zero,
   6 otherwise) and +0x14 always at shift 5, add the table entry times
   0x160000 into +0xC and +0x10, then store the second parameter at the
   payload's +0x14, 7 at +0x32 and +0x34, hand the node to func_8004491C
   with func_b31f_80171704 as its step, and store the third parameter at the
   payload's +0x0 and +0x8.

   THE TWIN IS IN THIS FILE: func_b31f_80171F30 (landed, 104 words), the
   other 0x211 spawner in this bank.  Every one of its statement shapes
   carried verbatim -- the mover copies through
   *(s16 *)((u8 *)n->unk_08 + K) = *(u16 *)(*(u8 **)(p + 0x8) + K), the
   UnkSlot cast on n->unk_0C, q = (u8 *)n + 0x20, and both of the tail's
   word stores off q.

   ARITY, SETTLED FROM THE CALLER: FIVE parameters.  func_b31f_801730AC
   calls it at four sites and every one sets $a0..$a3 AND stores the fifth
   argument with `sw' at 0x10($sp) in the jal's delay slot (two sites pass
   $zero, two pass a register).  The fourth parameter is NEVER READ in the
   body -- $a3 is not copied anywhere -- and still has to be declared.  The
   fifth is `s16': the body loads it with `lw $s4, 0x60($sp)' and tests it
   `sll 16' / `bnez', which is what PROMOTE_MODE gives a short parm, and
   `s32 e' with an (s16) cast is 169 words / 48 rows.

   THE WHOLE RESIDUE IS ONE MISSING PSEUDO.  The target saves SEVEN
   callee-saved registers and this body saves six:

     target  s0=n  s1=p then q  s2=b  s3=&t  s4=e  s5=b-ALIAS  s6=c
     mine    s0=n  s1=p then q  s2=&t  s3=e   s4=b  s5=c

   The target holds the second parameter in TWO registers -- `addu $s5, $s2,
   $zero' at 0x80171AD8, in the entry block right after the func_8003FD64
   call -- with $s2 feeding the FIRST table index (0x80171AF0) and $s5 the
   SECOND index (0x80171C0C) and the +0x14 store (0x80171C7C).  One word of
   the deficit is that copy; the other 39 rows are its wake.

   I COULD NOT PRODUCE THAT SECOND PSEUDO FROM ANY PLAIN-C SPELLING, and the
   mechanism is now understood and recorded, so do not re-sweep it:
   -da on this body shows the alias pseudo (reg 89) SURVIVING cse, loop,
   flow, combine, regmove, sched and local-alloc and then being COALESCED
   with the parameter pseudo (reg 81) at global-alloc -- because cse's
   make_regs_eqv picked the ALIAS as qty_first_reg (its last use is after
   cse_basic_block_end and later than the parameter's), so canon_reg
   rewrote both of the parameter's uses to the alias and left the parameter
   dead at the copy.  Reversing the roles loses it the other way: the
   partner whose last use is earlier always becomes the one rewritten.

   EVERY SPELLING MEASURED, one change at a time, all 171 words unless
   stated:
   | spelling                                            | words | aligned |
   | no alias at all, `b' at all three sites (below)     |  171  |   40    |
   | `u32 f = b' after the table copy, f post-merge      |  171  |   59    |
   | `f = b' after the func_8003FD64 call                |  171  |   57    |
   | `f = b' as the very first statement                 |  171  |   59    |
   | roles swapped (f pre-merge, b post-merge)           |  171  |   40    |
   | `f = b' assigned in BOTH arms of the if/else        |  171  |   39    |
   | alias + do { } while (0) round the if/else arms     |  171  |   59    |
   | alias + if (1) round the if/else arms               |  171  |   59    |
   | no alias + do { } while (0) round the arms          |  171  |   40    |
   | `g = &t.bb[...]' before the three mover copies      |  171  |   40    |
   | `g = &t.bb[...]' before the n->unk_10 store         |  171  |   40    |
   | `g = &t.bb[...]' hoisted before the call            |  171  |   51    |
   | an `m' local for n->unk_08 used at every site       |  161  |   59    |
   | a `w = t.bb' local feeding the first index          |  171  |   40    |
   | an `r' local for (UnkSlot *)n->unk_0C               |  171  |   40    |
   | `s32 e' with `if ((s16)e == 0)'                     |  169  |   48    |
   | `s32 b' with `(((u32)b >> 9) & 7)'                  |  171  |   40    |
   | `q = (u8 *)n + 0x20' at the top of the if-body      |  171  |   40    |
   | `q = (u8 *)n + 0x20' just before the +0x14 store    |  171  |   68    |
   FLAT AT 40 ACROSS EIGHT DECLARATION AND STATEMENT PERMUTATIONS, so
   allocation priority and statement order are both ruled out; the fault is
   the pseudo COUNT.  WHAT I WOULD TRY NEXT: the permuter, which can reach
   spellings (a dead test, a duplicated arm) that create a second pseudo
   cse cannot fold.  A `__asm__' barrier on the parameter between its two
   use groups is the other candidate and was not tried.

   TWO SHAPE FACTS WORTH KEEPING, both forced by the listing:
   1. THE TABLE LOCAL MUST BE A BYTE STRUCT.  The copy is four groups of
      `lwl'/`lwr' plus `swl'/`swr' over $sp+0x10..0x2F, i.e. alignment 1, so
      it needs its own 0x20-byte `u8 bb[0x20]' type (UnkDngB31fName32 here,
      beside this unit's existing Name12 and Name24).  An
      `UnkDngB09A28Tbl'-style s16 array would be alignment 2 and emit
      `lh'/`sh' pairs.
   2. THE TABLE'S TWO HALFWORDS ARE READ DIFFERENTLY AT EVERY SITE: offset
      0 with a plain `lh' (`*(s16 *)g') and offset 2 with `lhu' followed by
      `sll 16'/`sra 16', which is what an (s16) cast on a u16 lvalue gives.
      Writing both as `*(s16 *)' is not what the target has.

   DECLARATIONS
   typedef struct UnkDngB31fName32 { u8 bb[0x20]; } UnkDngB31fName32;
   extern const u8 D_b31f_8016A894[];
   UnkNode *func_8003FD64(s32, void *);
   s32 func_8004491C(UnkNode *, u32);
   void func_b31f_801718E4(u8 *p, UnkVec *v);
   s32 func_b31f_80171704(u8 *p, UnkVec *v);
   END DECLARATIONS */
void func_b31f_80171A10(u8 *p, u32 b, s32 c, s32 d, s16 e)
{
    UnkDngB31fName32 t;
    UnkNode *n;
    u8 *q;
    u8 *g;

    t = *(UnkDngB31fName32 *)D_b31f_8016A894;
    n = func_8003FD64(0x211, p);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_b31f_801718E4;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(*(u8 **)(p + 0x8) + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(*(u8 **)(p + 0x8) + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(*(u8 **)(p + 0x8) + 0xA) - 0x14;
        g = &t.bb[((b >> 9) & 7) * 4];
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)n->unk_08 + 0x2) + *(s16 *)g * 16;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)n->unk_08 + 0x6) + (s16) * (u16 *)(g + 2) * 16;
        ((UnkSlot *)n->unk_0C)->unk_06 = 6;
        q = (u8 *)n + 0x20;
        if (e == 0)
        {
            n->unk_08->unk_0C = ((rand() & 0x7FFF) - 0x4000) << 7;
            n->unk_08->unk_10 = ((rand() & 0x7FFF) - 0x4000) << 7;
        }
        else
        {
            n->unk_08->unk_0C = ((rand() & 0x7FFF) - 0x4000) << 6;
            n->unk_08->unk_10 = ((rand() & 0x7FFF) - 0x4000) << 6;
        }
        n->unk_08->unk_14 = ((rand() & 0x7FFF) - 0x4000) << 5;
        g = &t.bb[((b >> 9) & 7) * 4];
        n->unk_08->unk_0C += *(s16 *)g * 0x160000;
        n->unk_08->unk_10 += (s16) * (u16 *)(g + 2) * 0x160000;
        *(s16 *)(q + 0x14) = b;
        *(s16 *)(q + 0x32) = 7;
        *(s16 *)(q + 0x34) = 7;
        func_8004491C(n, (u32)func_b31f_80171704);
        *(s32 *)q = c;
        *(s32 *)(q + 0x8) = c;
    }
}
