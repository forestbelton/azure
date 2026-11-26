/* func_s66_800242D4 -- ovl_dng_s66 -- PARK, agent-h237, batch 35.

   264 words target, 264 words candidate, 70 ALIGNED DIFFERENCES.
   dngfam: 1 member, 264 words -- no replication, this body lands once.
   Owns no jump table; its three-case switch is a compare chain.

   WHAT IS ALREADY RIGHT: every opcode, every immediate, every displacement,
   every branch target and the whole control flow, INCLUDING the awkward
   layout where state 2's loop-exit block sits physically BEFORE the loop at
   .Ls66_80024474, between state 1's code and state 2's entry. That falls out
   of writing the exit code inline inside the `if' that breaks the loop; it
   needed no device and no goto.

   ALL 70 ROWS ARE ONE REGISTER PERMUTATION of three callee-saved values.
   Filtering the diff for any row whose two sides differ in anything but a
   register name leaves exactly three, and those three are the prologue saves
   of the same three registers to their own slots:

     candidate                       target
       $s0 = p                         $s0 = sl (state 0) / i (state 2)
       $s1 = b                         $s1 = p
       $s2 = sl (state 0) / i (st 2)   $s2 = b

   A ROTATION BY ONE, not a swap. $s3/$s4/$s5 (x, y, z) and $s6/$s7 (the
   cycle end and base) match the target exactly, so the priority order of
   everything below the top three is already right.

   CAUSE: UNKNOWN, AND THE OBVIOUS DIAGNOSIS IS PROVABLY WRONG. STATED AS A
   HYPOTHESIS ONLY. Measured from a `-da' probe of the parked body, the
   allocation order is

     ;; 13 regs to allocate: 87 84 269 80 81 83 105 82 88 89 90 85 86

   with 80 = p (43 RTL occurrences), 81 = b (36), 83 = sl (6), 82 = i (4),
   88/89/90 = x/y/z, 85/86 = cend/cyc. So p is allocated FIRST and sl third.
   For the target's order, `sl' would have to outrank `p' under
   `allocno_compare's floor_log2(n_refs) * n_refs / live_length. It cannot:
   sl has 5 references over roughly 130 RTL lines while p has around 40
   (more once loop-weighted) over roughly 680, which is a factor of four the
   wrong way. No spelling of this body changes those counts, because every
   p reference and every sl reference is visible in the listing. Whatever
   gives the target's $s0 to sl is therefore NOT priority, and the next agent
   should not spend measurements on reference bumps. The remaining candidates
   I did not reach: `prune_preferences's regs_someone_prefers set, and
   local-alloc reserving a hard register across the relevant range before
   global-alloc runs.

   WHAT IS RULED OUT, MEASURED:

     allocation PRIORITY -- see above, and the declaration-order permutation
     below measures identically, which docs/matching-levers.md says means the
     cause is in the arithmetic or the statement order rather than priority.

   EVERY SPELLING MEASURED, with its score. aligndiff rows, word count given
   where it moves:

     the body as parked                                          264 /  70
     x, y, z declared `s16' instead of `s32'                 268 words /  92
     the +0x28 state bumps as the plain cast
       `*(s16 *)(p + 0x28) += 1' instead of the u16
       ARRAY REF `((u16 *)p)[0x14]'                         265 words /  74
     local declaration order with `sl' first instead of `i'      264 /  70

   TWO FACTS THAT ARE SETTLED AND ARE IN THE PARKED BODY:

   (1) x, y and z MUST BE `s32', and so must the matching parameters of
   func_s66_80024924. As `s16' the three arguments at the func_s66_80024924
   call site get `sll'/`sra' promotion pairs where the target has a plain
   `addu' -- FOUR WORDS LONG and 22 extra rows. This is the same fact that
   was worth 16 rows inside func_s66_80024924 itself; see that body's comment
   in csrc/. It is a SELECTION fact and it reads as an allocation fault from
   either side of the call.

   (2) The +0x28 state bump must be the u16 ARRAY REF
   `((u16 *)p)[0x14] = ((u16 *)p)[0x14] + 1'. As a plain cast the state-2
   exit block is ONE WORD LONG with a load-delay nop, because
   MEM_IN_STRUCT_P is false on the store and `true_dependence's
   fixed-scalar-versus-varying-struct arm then refuses to let the
   fixed-address D_800814A0 load move above it. The target issues
   `lw $v1, %lo(D_800814A0)($a0)' between the +0x28 load and the bump, i.e.
   above the +0x28 store. This is csrc/ovl_dng_s55.c's func_s55_80024078
   fact (1) holding on a different offset, and the same mechanism as
   func_s66_800246F4's fact (1) in csrc/.

   OTHER SPELLINGS IN THE PARKED BODY, ASSUMED AND NOT PRICED -- flagged so
   the next agent does not inherit them as facts:

     `off' is `s16 off[3]' at sp+0x28 and `w' is `s32 w[3]' at sp+0x30; the
     declaration order fixes those offsets and they match, but the TYPES were
     chosen from the load widths (`lhu' in state 0, `lh' in state 2) and not
     measured against alternatives.

     the func_8003DE58 table index is written as the integer sum
     `(((... ) >> 7) & 0x1C) + (s32)D_800E3D18' rather than an array ref,
     because the target's `addu $v0, $v0, $v1' puts the INDEX first and
     `pointer_int_sum' always builds PLUS(ptr, intop). Assumed from that
     lever, not priced here.

     `*(s16 *)(p + 0x2E) * 170' is the plain constant, not the
     sll/addu/sll/addu/sll decomposition the listing shows. Assumed from the
     existing lever, not priced here.

     the state-0 `||' gate and the shared `sh $v0, 0xA($s2)' tail of its
     inner if/else are jump2 cross-joins of the natural spelling and were
     right on the first compile, so nothing about them is measured.

   DECLARATIONS
   extern s16 D_80024A70;
   extern s8 D_80024AE0[];
   extern u32 D_800814A0;
   extern u32 D_800814A8;
   extern s16 D_80083228;
   extern u8 *D_800E3D18;
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   void func_800B8D64(s16, s16, s16);
   s32 func_800644B8(s32);
   UnkNode *func_s66_80024924(u8 *tmpl, s32 x, s32 y, s32 z, u8 *m, u8 *c, s16 k, u8 *d, s32 e);
   END DECLARATIONS
*/
void func_s66_800242D4(u8 *p, UnkVec *b)
{
    s32 i;
    u8 *sl;
    u8 *vv;
    u8 *cend;
    u8 *cyc;
    UnkNode *nn;
    s32 x;
    s32 y;
    s32 z;
    s16 off[3];
    s32 w[3];

    D_80024A70 += 1;
    switch (*(s16 *)(p + 0x28))
    {
    case 0:
        sl = *(u8 **)(*(u8 **)(p + 0x0) + 0xC);
        if ((*(u16 *)(sl + 0x14) & 0x8000) != 0 ||
            func_8003DE58(*(u8 **)(sl + 0x8), (UnkSlot *)sl, off, 0) != NULL)
        {
            vv = *(u8 **)(*(u8 **)(p + 0x0) + 0x8);
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(vv + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(vv + 0x6);
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(vv + 0xA);
            if ((*(u16 *)(sl + 0x14) & 0x8000) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + (u16)off[0];
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + (u16)off[1];
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + (u16)off[2];
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) - 0x20;
            }
        }
        if ((**(u16 **)(p + 0xC) & 0x80) != 0)
        {
            *(s16 *)(p + 0x2E) = 8;
            *(s16 *)(p + 0x30) = 9;
            ((u16 *)p)[0x14] = ((u16 *)p)[0x14] + 1;
        }
        break;
    case 1:
        if ((*(s16 *)(p + 0x30) -= 1) <= 0)
        {
            ((u16 *)p)[0x14] = ((u16 *)p)[0x14] + 1;
        }
        break;
    case 2:
        i = 0;
        cend = (u8 *)D_80024AE0;
        cyc = cend - 0x54;
        while (1)
        {
            x = *(s16 *)((u8 *)b + 0x2);
            y = *(s16 *)((u8 *)b + 0x6);
            z = *(s16 *)((u8 *)b + 0xA);
            vv = *(u8 **)(*(u8 **)(p + 0x4) + 0x8);
            w[0] = *(s32 *)(vv + 0x0);
            w[1] = *(s32 *)(vv + 0x4);
            w[2] = (*(s16 *)(p + 0x38) - 0x50) << 16;
            if (func_8003DE58(
                    *(u8 **)((((D_80083228 + *(s16 *)((u8 *)D_800814A8 + 0x2A) + 0x100) >> 7) &
                              0x1C) +
                             (s32)D_800E3D18),
                    *(UnkSlot **)(*(u8 **)(p + 0x4) + 0xC), off, 0) != NULL)
            {
                w[0] += off[0] << 16;
                w[1] += off[1] << 16;
                w[2] += off[2] << 16;
            }
            *(s32 *)((u8 *)b + 0x0) += (w[0] - *(s32 *)((u8 *)b + 0x0)) / *(s16 *)(p + 0x2E);
            *(s32 *)((u8 *)b + 0x4) += (w[1] - *(s32 *)((u8 *)b + 0x4)) / *(s16 *)(p + 0x2E);
            *(s32 *)((u8 *)b + 0x8) +=
                (w[2] - *(s32 *)((u8 *)b + 0x8)) / *(s16 *)(p + 0x2E) -
                (func_800644B8(*(s16 *)(p + 0x2E) * 170) << 7);
            nn = func_s66_80024924(*(u8 **)(p + 0x8), x, y, z, (u8 *)b, *(u8 **)(p + 0x10), 4,
                                   *(u8 **)(p + 0x14), i);
            if (nn != NULL)
            {
                *(UnkNode **)(p + 0x8) = nn;
                *(u8 **)(p + 0x14) = (u8 *)nn->unk_08;
            }
            if ((*(s16 *)(p + 0x2E) -= 1) <= 0)
            {
                *(s32 *)((u8 *)b + 0x0) = w[0];
                *(s32 *)((u8 *)b + 0x4) = w[1];
                *(s32 *)((u8 *)b + 0x8) = w[2];
                func_800B8D64(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6),
                              *(s16 *)((u8 *)b + 0xA));
                *(u16 *)(p - 0x2) |= 0x8000;
                ((u16 *)p)[0x14] = ((u16 *)p)[0x14] + 1;
                D_800814A0 |= 0x8000;
                break;
            }
            *(u8 **)(p + 0x10) += 0xC;
            if (*(u8 **)(p + 0x10) >= cend)
            {
                *(u8 **)(p + 0x10) = cyc;
            }
            i++;
        }
        break;
    }
}
