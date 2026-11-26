/* func_b22_8017086C -- PARKED at 287 words against 288, 5 ALIGNED DIFFERENCES.
   1 member (dngfam), 288 words, so this is worth exactly its own count.

   WHAT IT IS: the bank actor's TRAIL renderer.  It composes the slot's
   rotation with the camera matrix in the 0x1F800000 scratchpad, works out how
   many 16-unit segments the payload's +0x5A/+0x5E displacement needs, and for
   each segment allocates a 0x10-byte LINE_F2 off the resident OT cursor at
   g->unk_00 + 0x8D0, interpolates the two endpoints from +0x5A/+0x5E/+0x62
   scaled by i and i+1 over n, RotAverage4s them straight into the primitive
   and links it at sp->unk_24 + sp->unk_100 * 4.  After the loop one more
   0xC-byte DR_MODE closes the run.

   NO TWIN.  dngtwin finds nothing at 0.70.  dngcallee gives 12 of 18 symbols
   over six functions, all of them the GTE call set, and the best of them
   (func_b20_80173E40, csrc/ovl_dng_b20.c, 312 words) supplied the ENTIRE
   HEAD -- the UnkDng74334SP scratchpad, the PushMatrix / RotMatrix /
   ReadRotMatrix / CompMatrix / ScaleMatrix / SetTransMatrix / SetRotMatrix
   chain, `sp->unk_28 = s->unk_14', the `- d - 6', the sp->unk_0C[3] flag
   fiddle and the closing func_80067F20 / GetTPage(0, 1, 0, 0) / PopMatrix.
   Everything from the segment count down was read off the listing.

   THE RESIDUE IS TWO CLUSTERS, AND ONE OF THEM IS THE WHOLE MISSING WORD.

   CLUSTER 1 -- ONE WORD, A `nop' THE TARGET HAS AND THIS BODY DOES NOT.
   The target emits, at 0x80170984..0x8017099C:

       bgez  $v0, .Lb22_80170990
        nop
       negu  $v0, $v0
       addu  $a0, $v0, $zero          .Lb22_80170990
       bgez  $v1, .Lb22_801709A0
        nop
       negu  $v1, $v1

   so BOTH branch delay slots are nops: reorg filled neither.  This body
   emits the same insns with `addu $a0, $v0, $zero' stolen into the SECOND
   bgez delay slot and the first bgez redirected one word past the label, so
   it is 287 words.  The two insn streams are otherwise identical, including
   the registers.  Every spelling of the abs/max block that was tried either
   keeps the steal (287 words) or changes the stream somewhere else; the
   table below has all of them.  WHAT IS UNMEASURED: nothing was tried that
   makes the first bgez target label have more than one use, which is the one
   way reorg could be prevented from redirecting it, and no reorg.c citation
   was established for why the target did not take the insn.

   CLUSTER 2 -- THREE ROWS, THE +0x7 TERNARY IN $a0 WHERE THE TARGET HAS $v0.
   The target has `addiu $v0, 0x42' / `addiu $v0, 0x40' / `sb $v0, 0x7($s2)';
   this body has the same three insns in $a0.  Both are caller-saved temps, so
   this is local-alloc, not global, and it is almost certainly downstream of
   cluster 1 -- it is the last three words of the loop body and nothing else
   differs between them.

   THE TWO THINGS THAT GOT THE BODY FROM 84 ROWS TO 5, both worth recording:

   (1) `s->unk_0C[3] |= 0x2' AND `s->unk_0C[3] &= 0xFD' IN TWO ARMS, NOT A
   VALUE TERNARY.  As
   `s->unk_0C[3] = (sp->unk_28 & 4) != 0 ? (s->unk_0C[3] | 2) : (...& 0xFD)'
   the body is 288 words / 31 aligned differences, and all of it is a THREE-WAY
   ROTATION of `s', `p' and the 0xFFFFFF mask over $s4/$s5/$s6.  The reason is
   allocno_compare and it is measured off the `.lreg' dump: the ternary gives
   `s' 15 references over 352 insns (3*15/352 = 0.1278) which TIES `p' at
   15/352 and loses to the hoisted mask at 7/105 = 0.1333; the two `|=' arms
   give `s' 17 references, floor_log2 crosses to 4, the score becomes
   4*17/352 = 0.193 and `s' is allocated first.  jump2 cross-joins the two
   `sb $v0, 0xF($s4)' back into the one the listing shows, so the two arms
   cost no word.  This is the "references buy nothing until one crosses a
   power of two" rule with the power-of-two crossing doing all the work.

   (2) THE SEGMENT LOOP IS A `while (i < n)' AND NOT `if (n > 0) { do ... }'.
   The do-while form is 288 words / 7 aligned differences: it emits an extra
   `move $a0, $zero' ahead of the loop and loses the abs steal, which lands on
   the same word count by accident.  The `while' form is 287 / 5.  The loop IS
   a real loop in both -- loop.c hoists the 0xFFFFFF and 0xFF000000 addPrim
   masks into the preheader, which is what $s5 and $s7 are -- so the
   goto-back-edge lever does NOT apply here and was not tried.

   MEASURED AND REJECTED, one change at a time, with aligndiff:

   | spelling                                          | words | aligned |
   | as parked below                                   |  287  |    5    |
   | `n = y / 16' with no `t' temp                     |  287  |    5    |
   | `if (x > y)' for `if (y < x)'                     |  287  |    5    |
   | `/ 0x10' for `/ 16'                               |  287  |    5    |
   | the two +0x5A/+0x5E loads in the other order      |  287  |    5    |
   | `if (n < 1)' for `if (n == 0)'                    |  287  |    6    |
   | `t = w' inserted before the second abs            |  287  |    6    |
   | `if (n > 0) { do ... } while (i < n); }'          |  288  |    7    |
   | `for (i = 0; i < n; i++)'                         |  287  |    5    |
   | +0x7 ternary as `(x & 4) != 0 ? 0x42 : 0x40'      |  287  |    7    |
   | +0x7 ternary through an `s32 f' local             |  287  |    8    |
   | +0x7 as two separate `q[7] =' stores              |  290  |   80    |
   | `w = __builtin_abs(...)' then `y = w'             |  287  |    8    |
   | `y = __builtin_abs(...)', no `w'                  |  287  |    8    |
   | `y = w < 0 ? -w : w'                              |  287  |    9    |
   | `if (w < 0) { y = -w; } else { y = w; }'          |  287  |    9    |
   | `y = w' then `if (y < 0) y = -y'                  |  287  |    9    |
   | explicit ifs with no `w' at all                   |  287  |   11    |
   | `do { y = w; } while (0)'                         |  287  |   12    |
   | `s16 w'                                           |  288  |   16    |
   | abs(+0x5A) before abs(+0x5E)                      |  288  |   17    |
   | `y = w' moved after the second abs                |  287  |   14    |
   | `if (1) { y = w; }'                               |  287  |    5    |

   `if (1) { y = w; }' is FREE, so the construct is not a barrier reorg can
   see -- do not reach for it again here.

   SIX SPELLINGS THAT ARE LOAD-BEARING AND WERE PRICED ON THE WAY DOWN, all
   from the 84-row first compile: the +0x7 ternary at all (two stores is
   290/80); `*(s32 *)(q + 0x4) = *(s32 *)((u8 *)s + 0xC)' BEFORE `q[3] = 3'
   (the other order strands the `lw 0xC($s4)' behind a load-delay `nop');
   `q = *(u8 **)(g->unk_00 + 0x8D0)' re-read from `g' inside the loop rather
   than hoisted; the six interpolated stores written
   `sp->unk_B0[0].vx = sp->unk_B0[2].vx = <expr>' with the [2] lvalue
   INNERMOST, because the innermost lvalue emits first; RotAverage4's fifth
   through eighth arguments being `q + 0x8, q + 0xC, q + 0x8, q + 0xC' (the
   primitive is a 0x10-byte two-point line, not the twin's four-point quad);
   and `(u32)q + 0x10' for the cursor bump.

   OWNS NO JUMP TABLE and no rodata of its own.  The listing opens at
   `glabel'.  All eleven `jal' targets were resolved through
   splat/common_syms.txt and spelled by their real names -- CompMatrix,
   PushMatrix, PopMatrix, ReadRotMatrix, ScaleMatrix, SetRotMatrix,
   SetTransMatrix, RotAverage4, RotMatrix, GetTPage -- which need no
   declaration; only func_80067F20 does.

DECLARATIONS
void func_80067F20(u32 *, s32, s32, s32, s32);
END DECLARATIONS
*/
void func_b22_8017086C(u8 *p, UnkVec *v, UnkSlot *s, s16 d)
{
    UnkDng74334SP *sp = (UnkDng74334SP *)0x1F800000;
    Unk80083160 *g;
    MATRIX m;
    u8 *q;
    u8 *q2;
    s32 i;
    s32 n;
    s32 x;
    s32 y;
    s32 t;
    s32 w;

    g = &D_80083160;
    sp->unk_24 = g->unk_00 + 0xB0;
    sp->unk_74.t[0] = *(s16 *)((u8 *)v + 0x2);
    sp->unk_74.t[1] = *(s16 *)((u8 *)v + 0x6);
    sp->unk_74.t[2] = *(s16 *)((u8 *)v + 0xA);
    s->unk_14 |= 0x8000;
    PushMatrix();
    sp->unk_34.vx = sp->unk_34.vy = sp->unk_34.vz = 0x2000;
    sp->unk_A4.vx = s->unk_16;
    sp->unk_A4.vz = s->unk_1A;
    sp->unk_A4.vy = s->unk_18;
    RotMatrix(&sp->unk_A4, &sp->unk_74);
    ReadRotMatrix(&m);
    CompMatrix(&m, &sp->unk_74, &sp->unk_54);
    ScaleMatrix(&sp->unk_54, &sp->unk_34);
    SetTransMatrix(&sp->unk_54);
    SetRotMatrix(&sp->unk_54);
    sp->unk_28 = s->unk_14;
    w = *(s16 *)(p + 0x5E);
    x = *(s16 *)(p + 0x5A);
    if (w < 0)
    {
        w = -w;
    }
    y = w;
    if (x < 0)
    {
        x = -x;
    }
    if (y < x)
    {
        y = x;
    }
    t = y;
    n = t / 16;
    if (n == 0)
    {
        n = 1;
    }
    i = 0;
    while (i < n)
    {
        {
            q = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)q + 0x10;
            sp->unk_B0[0].vx = sp->unk_B0[2].vx = *(s16 *)(p + 0x5A) * i / n;
            sp->unk_B0[1].vx = sp->unk_B0[3].vx = *(s16 *)(p + 0x5A) * (i + 1) / n;
            sp->unk_B0[0].vy = sp->unk_B0[2].vy = *(s16 *)(p + 0x5E) * i / n;
            sp->unk_B0[1].vy = sp->unk_B0[3].vy = *(s16 *)(p + 0x5E) * (i + 1) / n;
            sp->unk_B0[0].vz = sp->unk_B0[2].vz = *(s16 *)(p + 0x62) * i / n;
            sp->unk_B0[1].vz = sp->unk_B0[3].vz = *(s16 *)(p + 0x62) * (i + 1) / n;
            sp->unk_100 = RotAverage4(&sp->unk_B0[0], &sp->unk_B0[1], &sp->unk_B0[2],
                                      &sp->unk_B0[3], (s32 *)(q + 0x8), (s32 *)(q + 0xC),
                                      (s32 *)(q + 0x8), (s32 *)(q + 0xC), &sp->unk_D0,
                                      &sp->unk_D4) -
                           d - 6;
            if ((sp->unk_28 & 0x8) != 0)
            {
                if ((sp->unk_28 & 0x4) != 0)
                {
                    s->unk_0C[3] |= 0x2;
                }
                else
                {
                    s->unk_0C[3] &= 0xFD;
                }
            }
            *(s32 *)(q + 0x4) = *(s32 *)((u8 *)s + 0xC);
            q[3] = 3;
            q[7] = 0x40;
            if ((sp->unk_28 & 0x8) != 0)
            {
                q[7] = (sp->unk_28 & 0x4) == 0 ? 0x40 : 0x42;
            }
            i++;
            addPrim(sp->unk_24 + sp->unk_100 * 4, q);
        }
    }
    q2 = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q2 + 0xC;
    func_80067F20((u32 *)q2, 0, 0, GetTPage(0, 1, 0, 0), 0);
    addPrim(sp->unk_24 + sp->unk_100 * 4, q2);
    PopMatrix();
}
