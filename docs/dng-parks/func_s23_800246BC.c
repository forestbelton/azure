/* func_s23_800246BC -- PARKED at 459 words against a 467-word target,
   88 aligned differences (tools/decomp/aligndiff.py). agent h152, batch 35.

   dngfam.py: 1 member, 467 words. No replication, and no other member is
   parked. The other SEVEN functions of ovl_dng_s23 all LANDED this batch at
   0 differing, so every symbol, signature and idiom below is corroborated by
   a landed body in the same unit.

   WHAT IT IS. The 0x212 sprite family's state machine, and the owner of
   jtbl_s23_80024008. It bumps the instance's +0x10 frame counter and
   dispatches on the +0xA state.

   THE TABLE. 7 entries, 7 distinct targets, no pad word. The dispatch is
   `sltiu $v0, $v1, 0x7' with NO minval subtraction, so the case set is
   0..6 with every value present, and the range check's `beqz' goes to
   .Ls23_80024DE0 -- which is the function's COMMON TAIL, `sh $zero,
   0x14($s0)' plus the epilogue, and NOT one of the seven table entries.
   So this is the brief's shape 1: seven real arms, no default among the
   entries, nothing to hand-settle, and an explicit `default:' is unneeded.
   I verified this with the `beqz' grep before writing any C.
   CASE 0 FALLS THROUGH INTO CASE 1 -- it has no `break'. That is visible in
   the listing as .Ls23_80024720 running off its end into .Ls23_800247C8.

   FRAME. `addiu $sp, $sp, -0x58', saves s0,s2,s3,ra,s4,s1 -- five
   callee-saved plus ra. Three live parameters (p, b, c), so TWO long-lived
   locals, which is what the body below has. Register map read off the
   landing attempts: $s0 = p, $s1 = the shared scratch pointer, $s2 = b,
   $s3 = c and then %hi(D_800814A8), $s4 = u.

   Stack locals, and the frame arithmetic that fixes them. The argument save
   area is 0x14 bytes for the one five-argument call, rounded to 0x18, so the
   first local sits at sp+0x18. gcc 2.7.2 gives a BLKmode local
   BIGGEST_ALIGNMENT, which is 8 bytes on this target, so:
       UnkVec t   at sp+0x18, 0x18 bytes, leaving frame_offset at 0x30
       s16 v[3]   at sp+0x30, 6 bytes,    leaving frame_offset at 0x36
       u16 e[2]   at sp+0x38, rounded up from 0x36 by the 8-byte alignment
   That alignment round is why `e' is a TWO-ELEMENT ARRAY rather than two
   scalars: two `u16' scalars would land at 0x36 and 0x38, and the listing
   needs 0x38 and 0x3A. &e[0] is func_800A0818's fifth argument and &e[1] is
   func_8009A350's fourth; only e[1] is ever read back.

   THE ONE FACT THAT IS WORTH THE WHOLE PARK -- 80 aligned rows.
   $s1 holds THREE different pointers in disjoint ranges: the owning actor
   read in the prologue, then (u8 *)&t from the middle of case 1 onward, then
   func_800D24A8's result in case 4. They must be ONE local in the source.
   Measured, all on this body:
       all three as one `u8 *q'                       459 w,  88 aligned
       case 4's node split into its own local         459 w, 173 aligned
       &t split into its own local                    461 w, 107 aligned
       &t not a local at all, t addressed off sp      456 w, 199 aligned
   With &t addressed sp-relative there is no `addiu $s1, $sp, 0x18' at all
   and the whole body's callee-saved assignment rotates: `p' loses $s0 to
   case 4's pointer, because that pointer crosses two calls, has four
   references and a short live range, and so outranks `p' in
   allocno_compare. Merging the three collapses it to one allocno and `p'
   gets $s0. This is the brief's `one local too many' rung reached from the
   other side -- the fix was merging THREE of mine into one.

   OTHER FACTS THAT HOLD, each measured on this body.
     THE THREE INVERTED GATES, 80 aligned rows between them. The four guards
     before the success arm each have the FAIL block as the FALL-THROUGH, not
     as the taken arm. Written the other way round -- `<= 0x200' for `>
     0x200', `== 0' for `!= 0' on func_800A45D8, `!= 0' for `== 0' on
     func_800A5690 -- it is 463 w / 168 aligned. The first guard, `(e[1] &
     0x3300) != 0', is the one that is NOT inverted.
     THE FOUR IDENTICAL FAIL BLOCKS ARE REALLY FOUR. Each is `lw
     %lo(D_800814A8)($s3) / lh 0x88 / sll 16 / sw 0x8($s1) / li 6 / j', they
     are byte-identical, and jump2 does NOT cross-join them; only the final
     `sh $v0, 0xA($s0)' is shared, at .Ls23_80024AE4, because the FAIL
     blocks reach it with 6 and the success arm with the incremented value.
     Writing them as one block under an `&&' chain cannot reproduce that.
     `m + 0xA' NEEDS A LOCAL, 2 words. The else arm of the 0x8000 gate uses
     the register the earlier `lhu $v1, 0xA($v1)' left, so the value is a
     local; read again it is 461 w and two extra words.
     THE 0xA0 AND 0xA00000 ADDENDS EACH NEED A LOCAL. `abs(t_A - (b_A +
     0xA0))' and `(t_8 - (b_8 + 0xA00000)) / n' both get the constant folded
     through the subtraction into `addiu -0xA0' and `lui 0xFF60'; with the
     sum assigned to an `s32' local first, the target's separate `addiu $v1,
     $v1, 0xA0' and `lui $v0, 0xA00000 >> 16' survive. Worth 2 rows each.
     THE ABS IS THE TWO-WORD IN-PLACE abssi2 FORM in all three cases -- same
     register in and out, `bgez' with a `nop' delay slot -- so each is a
     plain `abs()' whose operand dies there, per the matching-levers entry.

   SPELLINGS MEASURED AND FREE at 459 w / 88 aligned, so NONE of them is
   load-bearing and the next agent should not spend budget preserving them:
     `q = *(u8 **)(p + 0x0)' before or after the +0x10 counter bump
     the then-arm index through a reassigned `k' local, or inline
     the table address on either side of the index sum
     the lookup reached as `(u8 *)D_8006CCD8 + k'
     the abs operands via `s16' locals, or as `(s16)*(u16 *)(q + K)'
     an `(s32)' cast on the two `<< 6' reads
     the two `<< 6' results through the multi-set `s32 h' local (90, so
       marginally WORSE -- not free, but not a fix either)
     the case 4 clamp as `j = (i < 0x100) ? i : 0xFF', or as a copy then an
       `if' into a second variable, or in place on one variable
     the 0xA00000 sum hoisted above the unk_10 store
     reading D_800814A8 inline in the then arm instead of through `g'

   WHAT IS STILL WRONG, in descending size. All four resisted every
   one-at-a-time change above, which is the brief's signature for a fix that
   is only correct IN COMBINATION -- so the next agent should HOLD one of
   these and vary another on top of it rather than sweeping singly.

   1. 20 rows and 6 of the 8 missing words, rows 132..154. In the `(u16 at
      u+0x1E | 0x2000) != 0' arm the target's table-base and index registers
      are $v1 and $v0; mine are $v0 and $v1, with EVERY INSTRUCTION IN THE
      SAME ORDER. Because the exchange makes my two arms' tails
      byte-identical, jump2 cross-joins the last six words of the second
      lookup into a shared label and the body comes out six words short. The
      ELSE arm matches the target exactly, so this is one arm's local-alloc
      tie-break: qty_compare_1 breaks on quantity number, so the index pseudo
      must be born before the table-base pseudo in that block and in mine it
      is not. Nine spellings of the two lookups were measured and all nine
      score 88 or worse. Fixing the pair should recover the six words for
      free, because the tails stop being identical.
   2. 5 rows, rows 25..34. In case 0 the target loads `w = *(u8 **)(q -
      0x14)' and both `lbu' of w BEFORE `lui %hi(D_800814A8)'; mine starts
      the D_800814A8 load first. sched1, not allocation.
   3. 5 rows, rows 171..187. `(*(s16 *)(p + 0x16) << 6) + 0x20' emits `lhu'
      in mine and `lh' in the target. combine rewrites the sign_extend to a
      zero_extend through force_to_mode because only the low 16 bits reach
      the `sh'. The target did not, so something keeps the full width live
      there. The displaced `lui $s3, %hi(D_800814A8)' at row 178 is a
      consequence: in the target it fills the `lh' delay slot, in mine a
      `nop' does.
   4. 1 row each: `p[0x1C] = 0' sits four rows early in case 0 (row 58
      against 62), `q = *(u8 **)(p + 0x0)' two rows early in the prologue,
      and the `sw $v0, 0x10($s2)' of the second division one row late.

   I DID NOT RUN THE PERMUTER -- the brief forbids it. If item 1 is to go to
   a search, the statement to permute is the pair of lookups inside the
   `(u16 at u+0x1E | 0x2000) != 0' arm, which is the two lines assigning
   `*(s16 *)(p + 0x16)' and `*(s16 *)(p + 0x18)'.

   A NOTE ON `| 0x2000'. Two gates in this body read `*(u16 *)(u + 0x1E)',
   OR in 0x2000 and branch on the result, which can never be zero. It is
   written as `|' here because that is what the ROM does -- `lhu / ori
   0x2000 / beqz' -- and gcc 2.7.2 does not fold `(x | C) != 0' for a
   nonzero C the way it folds `(x & C)'. It reads like a `&' that was
   mistyped in the original source; do not `fix' it.

   DECLARATIONS
   u8 *func_8003DE58(u8 *, u8 *, void *, s16);
   void func_80042640(Unk42710 *, s16);
   s16 func_8009A350(s16, s16, s32, u16 *);
   s32 func_800A0818(s16, s16, s16, s16, u16 *);
   s16 func_800A45D8(u16, u16, s32);
   s32 func_800A5690(void);
   s32 func_800A56E0(s32);
   u16 func_800A6D30(void);
   s16 func_800BCAD0(u8 *);
   u8 *func_800D24A8(s16, s16, s16, s16);
   UnkNode *func_s23_800240CC(u8 *p, UnkVec *b, s16 k);
   UnkNode *func_s23_8002441C(u8 *p, UnkVec *b);
   UnkNode *func_s23_800245CC(u8 *p, UnkVec *b);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 D_80082E80[];
   extern s32 D_8008346C;
   extern u32 D_800814A0;
   extern u32 D_800814A8;
   END DECLARATIONS */
void func_s23_800246BC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkVec t;
    s16 v[3];
    u16 e[2];
    u8 *q;
    u8 *u;
    u8 *w;
    u8 *m;
    u8 *g;
    u16 n;
    s32 h;
    s16 k;
    s16 x;
    s16 y;
    s16 r;
    s32 j;
    s32 i;

    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    q = *(u8 **)(p + 0x0);
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        w = *(u8 **)(q - 0x14);
        k = (*(u16 *)(D_800814A8 + 0x2A) >> 8) & 0xE;
        x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
        y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
        *(s16 *)(q + 0x2A) = func_800A0818(w[0x24], w[0x25], x, y, e);
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        p[0x1C] = 0;
        *(s16 *)(p + 0xE) = (*(u16 *)(q + 0x2A) >> 9) & 0x7;
        *(u32 *)(c + 0xC) = 0x808080;
        /* fallthrough */
    case 1:
        u = q - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(u + 0xC) + 0x8), *(u8 **)(u + 0xC), v, 0) == NULL &&
            (*(u16 *)(*(u8 **)(u + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        m = *(u8 **)(u + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        n = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)b + 0xA) = n;
        if ((*(u16 *)(*(u8 **)(u + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + v[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + v[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + v[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = n - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        q = (u8 *)&t;
        if ((*(u16 *)(u + 0x1E) | 0x2000) != 0)
        {
            g = (u8 *)D_800814A8;
            k = (*(u16 *)(g + 0x2A) >> 8) & 0xE;
            *(s16 *)(p + 0x16) = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
            k = (*(u16 *)(g + 0x2A) >> 8) & 0xE;
            *(s16 *)(p + 0x18) = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
        }
        else
        {
            *(s16 *)(p + 0x16) = D_80082E80[0x24] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCD8);
            *(s16 *)(p + 0x18) = D_80082E80[0x25] + *(u16 *)(*(s16 *)(p + 0xE) * 2 + (s32)D_8006CCE8);
        }
        *(s16 *)(q + 0x2) = (*(s16 *)(p + 0x16) << 6) + 0x20;
        *(s16 *)(q + 0x6) = (*(s16 *)(p + 0x18) << 6) + 0x20;
        *(s16 *)(q + 0xA) = *(u16 *)(D_800814A8 + 0x88) - 0x20;
        r = func_800BCAD0(q);
        *(s16 *)(q + 0xA) = r;
        *(s16 *)(p + 0x1A) = r;
        func_8009A350(*(u16 *)(p + 0x16) - 1, *(s16 *)(p + 0x18), 0, &e[1]);
        if ((e[1] & 0x3300) != 0)
        {
            *(s32 *)(q + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 6;
        }
        else if (*(s16 *)(q + 0xA) > 0x200)
        {
            *(s32 *)(q + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 6;
        }
        else if ((s16)func_800A45D8(*(u16 *)(q + 0x2), *(u16 *)(q + 0x6), *(s16 *)(q + 0xA)) != 0)
        {
            *(s32 *)(q + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 6;
        }
        else if ((s16)func_800A5690() == 0)
        {
            *(s32 *)(q + 0x8) = *(s16 *)(D_800814A8 + 0x88) << 16;
            *(s16 *)(p + 0xA) = 6;
        }
        else
        {
            if ((*(u16 *)(u + 0x1E) | 0x2000) != 0)
            {
                p[0x1C] = 1;
            }
            else
            {
                p[0x1C] = 2;
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        x = *(u16 *)(q + 0x2);
        v[0] = abs(x - *(s16 *)((u8 *)b + 0x2));
        y = *(u16 *)(q + 0x6);
        v[1] = abs(y - *(s16 *)((u8 *)b + 0x6));
        h = *(s16 *)((u8 *)b + 0xA) + 0xA0;
        v[2] = abs(*(s16 *)(q + 0xA) - h);
        *(s16 *)(p + 0x12) = v[0];
        for (i = 1; i < 3; i++)
        {
            if (*(s16 *)(p + 0x12) < v[i])
            {
                *(s16 *)(p + 0x12) = v[i];
            }
        }
        *(s16 *)(p + 0x12) = (*(s16 *)(p + 0x12) >> 4) + (*(s16 *)(p + 0x12) >> 5);
        if (*(s16 *)(p + 0x12) == 0)
        {
            *(s16 *)(p + 0x12) = 1;
        }
        b->unk_0C = (*(s32 *)(q + 0x0) - b->unk_00) / *(s16 *)(p + 0x12);
        b->unk_10 = (*(s32 *)(q + 0x4) - b->unk_04) / *(s16 *)(p + 0x12);
        h = b->unk_08 + 0xA00000;
        b->unk_14 = (*(s32 *)(q + 0x8) - h) / *(s16 *)(p + 0x12);
        func_s23_800240CC(p, b, *(s16 *)(p + 0x12));
        func_s23_800240CC(p, b, *(s16 *)(p + 0x12));
        func_s23_800240CC(p, b, *(s16 *)(p + 0x12));
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_s23_8002441C(p, b);
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            func_s23_800245CC(p, b);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x11)
        {
            if (p[0x1C] != 0)
            {
                q = func_800D24A8(p[0x1C], *(s16 *)(p + 0x16), *(s16 *)(p + 0x18), *(s16 *)(p + 0x1A));
                func_80042640((Unk42710 *)q, 0x34);
                j = (p[0x9] >> 1) + (p[0x9] << 1) + (func_800A6D30() & 0x3) + 0x10;
                if (j >= 0x100)
                {
                    j = 0xFF;
                }
                q[0x28] = j;
                q[0x29] = j;
            }
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 6:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 5;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
