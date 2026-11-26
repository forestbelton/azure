/* func_s24_800244F0 -- ovl_dng_s24, 482 words.  PARKED at
   482 words / 482 candidate / 13 differing (ovlverify) / 14 ALIGNED ROWS.
   Every one of the 14 rows is a REGISTER NAME; no word is missing, no word is
   extra, no instruction is in the wrong place, and no opcode or immediate
   differs.  The residue sits in exactly two spots, both named below.

   THE TABLE: this body owns jtbl_s24_80024008 -- 6 words = 6 cases + 0 pad,
   dispatch bound 6, 6 distinct targets (jtcheck).  ARM ORDER IS SETTLED BY
   STANDALONE COMPILE, not by the verifier: `tools/cdkgcc -O2 -G0 -S' on this
   body emits [$L29, $L30, $L54, $L56, $L61, $L63] at strictly ascending arm
   addresses, which is 1:1 positional against the ROM's
   [.Ls24_80024554, .Ls24_80024600, .Ls24_80024A74, .Ls24_80024AE8,
   .Ls24_80024BC8, .Ls24_80024C00].  Ascending by case value 0..5 IS entry
   order here.  The range-check `beqz' lands on the common tail, so there is no
   default among the six and no `default:' label is written.

   SHAPE, read off the listing and all of it confirmed by the 0-row regions:
   a six-state dispatcher on *(s16 *)(p + 0xA); state 0 FALLS THROUGH into
   state 1; the tail `*(s16 *)(p + 0x14) = 0;' is after the switch.  The
   four-armed chain in state 1 is an `else if' ladder with its body written out
   FOUR TIMES, not an `||' -- the ROM has four copies of
   `w+0x8 = D_800814A8+0x88 << 16; p+0xA = 5' and gcc emits ONE copy for a
   `||'.  jump2 cross-joins only the final `sh $v0, 0xA($s0)'.

   THE ONE FACT THAT PAID THE MOST, and it is a new lever: see
   MEM_IN_STRUCT_P below.  Worth 2 words and 6 rows.

   PRICED TABLE -- every cell measured with aligndiff on this unit copy.
   Rows are ALIGNED rows; ovlverify's differing count is noise once a word
   moves, so it is not quoted per cell.

   state 1's D_800814A8 facing lookup, index spelling:
     `((*(u16 *)(D_800814A8 + 0x2A) >> 8) & 0xE) + (s32)D_8006CCD8'
                                                     477 words / 50 rows
     `((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCD8'
                                                     483 words / 28 rows  KEPT
   Both spellings emit the SAME two instructions (`srl 8' / `andi 0xE') because
   combine folds `((x >> 9) & 7) << 1'.  They are NOT equivalent to the
   allocator: the `>>9 & 7 then *2' form has FOUR pseudos in the index chain
   where `>>8 & 0xE' has three, the index quantity then outranks the table
   address quantity, and the two arms of the gate stop being word-identical --
   so jump2 can no longer cross-join their six-word tails and the 6 missing
   words come back.  NOTE that state 0's copy of the same lookup matches with
   EITHER spelling, because there the two tables share one cse'd index.

   MEM_IN_STRUCT_P on the stack vector's three integer-half stores:
     `*(s16 *)(w + 0x2) = ...' with `u8 *w'         484 words / 24 rows
     `w->unk_02 = ...' with a struct pointer        482 words / 16 rows  KEPT
   This is the brief's fixed-scalar-versus-varying-struct rule used in the
   direction the lever does not yet record: the ROM SCHEDULES the `lui'/`lw' of
   the file-scope scalar D_800814A8 INTO the two load-delay slots of the
   `lh 0x16($s0)' / `lh 0x18($s0)' pair.  With the stores written as plain
   casts the MEM is not in_struct, `true_dependence' refuses to let the scalar's
   load move above them, and the two slots come out as `nop' -- exactly 2 extra
   words.  A COMPONENT_REF sets MEM_IN_STRUCT_P, the rule fires, and both nops
   are filled.  A pointer local is required as well: with the frame object
   addressed directly (`(u8 *)&v + 0x2') gcc folds the frame offset into every
   MEM, the `addiu $s1, $sp, 0x18' pseudo never exists, and the body loses a
   callee-saved register (477 words / 166 rows).

   the `*(u8 *)(p + 0x1C) = <1 or 2>' choice:
     `((...| 0x2000) != 0) ? 1 : 2'                 483 words / 28 rows
     `((...| 0x2000) == 0) ? 2 : 1'                 483 words / 28 rows
     two `if'/`else' stores of 1 and 2              484 words / 24 rows  KEPT
   The two `?:' spellings put the constants in the other order; the if/else
   pair reproduces the ROM's `bnez' + hoisted `addiu $v0, 1' and is worth one
   word.  (The brief's `a ?: is not a neutral spelling' entry, confirmed.)

   the loop's comparison operand order:
     `if (*(s16 *)(p + 0x12) < out[i])'             482 words / 16 rows
     `if (out[i] > *(s16 *)(p + 0x12))'             482 words / 14 rows  KEPT
   Two rows, the two `lh' in the other order, nothing else moving.

   state 1's `*(s16 *)((u8 *)b + 0xA)' in the 0x8000-set arm:
     `= *(u16 *)(u + 0xA) - 0x40'                   479 words / 59 rows
     `= *(u16 *)((u8 *)b + 0xA) - 0x40'             479 words / 52 rows  KEPT
   Reading it back off b lets cse forward the value just stored there; reading
   u again costs a reload and a nop, because the three stores through b may
   alias u.

   the two scale statements' `<< 6' versus `* 0x40':
     `(*(s16 *)(p + 0x16) << 6) + 0x20'             479 words / 59 rows
     `*(s16 *)(p + 0x16) * 0x40 + 0x20'             479 words / 52 rows  KEPT
   The shift form emits `lhu' where the ROM has `lh' -- convert.c's
   LSHIFT_EXPR arm is guarded on TREE_UNSIGNED and MULT_EXPR's is not.

   state 0's statement order:
     0x1C/0x20 zeroed BEFORE the p+0xE statement    479 words / 59 rows
     0x1C/0x20 zeroed AFTER it                      477 words / 50 rows  KEPT

   the byte triple in the colour arms (this is func_s24_80024264's fact, kept
   here because the same idiom appears across the unit):
     `c[0xE] = c[0xD] = c[0xC] = c[0xE] + 1'        86 words / 6 differing
     `c[0xC] = c[0xD] = c[0xE] = c[0xE] + 1'        86 words / 0 differing
   A chained assignment stores RIGHT TO LEFT, so the innermost lvalue is the
   one emitted first.

   WHAT REMAINS -- 14 rows, in two places, both pure register naming.

   (1) 2 rows.  `else if (w->unk_0A > 0x200)' is `lh $v0, 0xA($s1)' /
       `slti $v0, $v0, 0x201' where the ROM has `lh $a2, 0xA($s1)' /
       `slti $v0, $a2, 0x201'.  The value is short-lived and block-local and
       the ROM puts it in an ARGUMENT register although $v0 is free; MIPS
       defines no REG_ALLOC_ORDER in this tree (checked:
       scratch/gcc-src/cdk-gcc-b18/config/mips/mips.h has none), so
       find_free_reg scans from $v0 and something must have outranked it.
       Measured and NOT the cause: the plain cast
       `*(s16 *)((u8 *)w + 0xA) > 0x200' is the identical 14 rows.

   (2) 12 rows, all inside the 13-word clamp in state 3.  The ROM is
         lbu $v1, 0x9($s0) / srl $a0, $v1, 1 / sll $v1, $v1, 1 /
         addu $a0, $a0, $v1 / addu $a0, $a0, $v0 / addiu $a0, $a0, 0x10 /
         addu $v0, $a0, $zero / slti $a0, $a0, 0x100 / bnez / nop /
         addiu $v0, 0xFF / sb $v0, 0x28($s1) / sb $v0, 0x29($s1)
       -- ONE load of p+0x9 and a SURVIVING `kk = k' copy.  This body gets
       TWO loads of p+0x9 (cse never unifies them: the `.cse', `.combine',
       `.regmove', `.sched' and `.lreg' dumps all still show two
       `(zero_extend:SI (mem/s:QI (plus (reg/v 80) (const_int 9))))' insns),
       reload_cse turns the second into `move $a0, $v1', and the ternary's
       copy is then coalesced away.  Same 13 words, 12 names wrong.
       MEASURED, each at 482 words unless stated:
         two reads + `m[0x29] = m[0x28] = (k < 0x100) ? k : 0xFF'   14  KEPT
         two reads + `kk = (k<0x100)?k:0xFF' into an s32 local      14
         two reads + the same into a `u8 kk' local                  14
         two reads + `if (k < 0x100) kk = k; else kk = 0xFF;'       16
         two reads + `(k > 0xFF) ? 0xFF : k'               485 words / 119
         two reads + `k + 0x10' split out of the sum                14
         `p[0x9]' array-ref instead of the cast                     14
         ONE read via `kb = *(u8 *)(p + 0x9);' with the
           func_800A6D30() call hoisted to its own statement
           (so kb does not cross the call)               481 words / 22
         ONE read with `s32 kb' rather than `u32 kb'      481 words / 22,
           and `sra' instead of `srl' -- the shift must be unsigned
         ONE read + a separate `kc' for the clamp         481 words / 22
         ONE read + duplicated stores in both arms        483 words / 119
         ONE read with the call left inside the sum
           statement (kb then crosses the call and takes a
           callee-saved register)                        483 words / 122
       So the one-read form is EXACTLY ONE WORD SHORT -- it loses the copy as
       well -- and no spelling tried keeps both.  The next thing to try is a
       conjunction, in the brief's sense: the one-read form PLUS whatever makes
       the ternary's copy survive local-alloc's combine_regs.  combine_regs
       (scratch/gcc-src/cdk-gcc-b18/local-alloc.c:1791) refuses to tie when
       `reg_qty[sreg] == -1', i.e. when the DESTINATION is not block-local, so
       a clamp variable that is live in a further basic block should keep the
       copy.  THAT IS A HYPOTHESIS, NOT A MEASUREMENT.

   NOT MEASURABLE, so do not re-run these:
     * the jump table's arm order under ovlverify/aligndiff -- settled by the
       standalone compile above instead;
     * `(s32)D_8006CCD8 + idx' versus `idx + (s32)D_8006CCD8' -- fold swaps a
       constant address to the second operand, so both are one tree (477
       words / 50 rows either way, bit-identical);
     * `*(u16 *)((u8 *)D_800814A8 + 0x88)' versus
       `*(u16 *)(D_800814A8 + 0x88)' -- identical, 484 words / 24 rows at the
       time it was priced.

   LOCAL COUNT IS LOAD-BEARING.  Twelve locals.  Adding a thirteenth that
   CROSSES the func_800A6D30 call rotates $s0/$s1 across the whole body (483
   words / 122 rows); adding one that does not cross a call is free.

   DECLARATIONS
   typedef struct UnkDngS24Fxv
   {
       u16 unk_00;
       s16 unk_02;
       u16 unk_04;
       s16 unk_06;
       u16 unk_08;
       s16 unk_0A;
       s32 unk_0C;
       s32 unk_10;
       s32 unk_14;
   } UnkDngS24Fxv;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 D_80082E80[];
   extern s32 D_8008346C;
   extern u8 D_800DEF08[];
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   void func_8003DB94(u8 *, u8 *, s16);
   s32 func_800A0818(s16, s16, s16, s16, u16 *);
   s16 func_800BCAD0(u8 *);
   s16 func_8009A350(s16, s16, s32, u16 *);
   s16 func_800A45D8(u16, u16);
   s16 func_800A5690(void);
   s32 func_800A56E0(s32);
   s32 func_800A6D30(void);
   u8 *func_800D2CB4(s16, s16, s16, s16);
   s32 func_800D36D0(u8 *, UnkVec *, UnkSlot *);
   UnkNode *func_s24_80024134(u8 *p, UnkVec *q, s16 d);
   UnkNode *func_s24_800243BC(u8 *p, UnkVec *q);
   END DECLARATIONS
*/
void func_s24_800244F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkDngS24Fxv v;
    s16 out[4];
    u16 q;
    u16 z;
    u8 *s;
    u8 *o;
    u8 *u;
    UnkDngS24Fxv *w;
    u8 *m;
    s32 i;
    s32 k;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(s + 0x2A) = func_800A0818(
            *(u8 *)(*(u8 **)(s - 0x14) + 0x24), *(u8 *)(*(u8 **)(s - 0x14) + 0x25),
            D_80082E80[0x24] +
                *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 8) & 0xE) + (s32)D_8006CCD8),
            D_80082E80[0x25] +
                *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 8) & 0xE) + (s32)D_8006CCE8),
            &q);
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        *(u8 *)(p + 0x1C) = 0;
        *(s32 *)(p + 0x20) = 0;
        *(u32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC), out,
                          0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        u = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(u + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(u + 0x6);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(u + 0xA);
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        w = &v;
        if ((*(u16 *)(o + 0x1E) | 0x2000) != 0)
        {
            *(s16 *)(p + 0x16) =
                D_80082E80[0x24] +
                *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCD8);
            *(s16 *)(p + 0x18) =
                D_80082E80[0x25] +
                *(u16 *)(((*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7) * 2 + (s32)D_8006CCE8);
        }
        else
        {
            *(s16 *)(p + 0x16) =
                D_80082E80[0x24] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCD8);
            *(s16 *)(p + 0x18) =
                D_80082E80[0x25] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCE8);
        }
        w->unk_02 = *(s16 *)(p + 0x16) * 0x40 + 0x20;
        w->unk_06 = *(s16 *)(p + 0x18) * 0x40 + 0x20;
        w->unk_0A = *(u16 *)(D_800814A8 + 0x88) - 0x20;
        *(s16 *)(p + 0x1A) = w->unk_0A = func_800BCAD0((u8 *)w);
        func_8009A350(*(u16 *)(p + 0x16) - 1, *(s16 *)(p + 0x18), 0, &z);
        if ((z & 0x3300) != 0)
        {
            *(s32 *)((u8 *)w + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 5;
        }
        else if (w->unk_0A > 0x200)
        {
            *(s32 *)((u8 *)w + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 5;
        }
        else if (func_800A45D8(*(u16 *)((u8 *)w + 0x2), *(u16 *)((u8 *)w + 0x6)) != 0)
        {
            *(s32 *)((u8 *)w + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 5;
        }
        else if (func_800A5690() == 0)
        {
            *(s32 *)((u8 *)w + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 5;
        }
        else
        {
            if ((*(u16 *)(o + 0x1E) | 0x2000) != 0)
            {
                *(u8 *)(p + 0x1C) = 1;
            }
            else
            {
                *(u8 *)(p + 0x1C) = 2;
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        out[0] = __builtin_abs((s16)*(u16 *)((u8 *)w + 0x2) - *(s16 *)((u8 *)b + 0x2));
        out[1] = __builtin_abs((s16)*(u16 *)((u8 *)w + 0x6) - *(s16 *)((u8 *)b + 0x6));
        out[2] = __builtin_abs(*(s16 *)((u8 *)w + 0xA) - *(s16 *)((u8 *)b + 0xA));
        *(s16 *)(p + 0x12) = out[0];
        for (i = 1; i < 3; i++)
        {
            if (out[i] > *(s16 *)(p + 0x12))
            {
                *(s16 *)(p + 0x12) = out[i];
            }
        }
        *(s16 *)(p + 0x12) = ((s16)*(u16 *)(p + 0x12) >> 4) + ((s16)*(u16 *)(p + 0x12) >> 5);
        if (*(s16 *)(p + 0x12) == 0)
        {
            *(s16 *)(p + 0x12) = 1;
        }
        b->unk_0C = (*(s32 *)((u8 *)w + 0x0) - b->unk_00) / *(s16 *)(p + 0x12);
        b->unk_10 = (*(s32 *)((u8 *)w + 0x4) - b->unk_04) / *(s16 *)(p + 0x12);
        b->unk_14 = (*(s32 *)((u8 *)w + 0x8) - b->unk_08) / *(s16 *)(p + 0x12);
        func_s24_80024134(p, b, *(s16 *)(p + 0x12));
        func_s24_80024134(p, b, *(s16 *)(p + 0x12));
        func_s24_80024134(p, b, *(s16 *)(p + 0x12));
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            *(s32 *)(p + 0x20) = (s32)func_s24_800243BC(p, b);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) > 0x30)
        {
            if (*(u8 *)(p + 0x1C) != 0)
            {
                m = func_800D2CB4(*(u8 *)(p + 0x1C), *(s16 *)(p + 0x16), *(s16 *)(p + 0x18),
                                  *(s16 *)(p + 0x1A));
                func_80042640((Unk42710 *)m, 0x35);
                k = (*(u8 *)(p + 0x9) >> 1) + (*(u8 *)(p + 0x9) << 1) + (func_800A6D30() & 3) +
                    0x10;
                m[0x29] = m[0x28] = (k < 0x100) ? k : 0xFF;
                func_8003DB94(*(u8 **)(m - 0x14), D_800DEF08,
                              *(s8 *)(*(u8 **)(*(u8 **)(p + 0x20) + 0xC) + 0x4));
            }
            if (*(UnkNode **)(p + 0x20) != NULL)
            {
                (*(UnkNode **)(p + 0x20))->unk_1E |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 5:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 4;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
