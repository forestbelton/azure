/* func_s10_80024930 -- PARKED at 243 words / 243 words / 14 aligned
   differences (ovlverify reports 14 differing; aligndiff agrees, so no word
   is displaced).  Single-member family (dngfam.py: 1 member, 243 words), so
   landing it lands only itself.  relcand.py: ok, 1 data symbol, 6 calls.

   WHAT IT IS.  The sixteen-segment semi-transparent Gouraud-textured ring
   this effect is drawn as: one 0x34-byte POLY_GT4 per 1/16 of a turn taken
   off D_80083160.unk_00 + 0x8D0, its four corners staged as SVECTORs in the
   0x1F800000 scratchpad at +0x64 (stride 8), projected with RotAverage4 into
   the scratchpad pairs at +0xD8..+0xE7, and hung on the OT row RotAverage4
   returns when that row is below 0x1E0.  Eight parameters; the first and the
   third are never read.

   NO TWIN BY EITHER TOOL.  dngtwin.py --unit ovl_dng_s10 reports no carved
   sibling at ratio 0.70 for this stub, and xsibs.py is useless on an s-unit
   (the fifth consecutive one).  The route that supplied every spelling was
   the callee-set grep: GetTPage / func_80065590 / D_1F800018 / D_1F8000D8
   intersected to csrc/ovl_dng_s17.c, csrc/ovl_dng_s46.c and
   csrc/ovl_dungeon_800D50FC.c, and the two bodies actually read for idiom
   are csrc/ovl_dng_s17.c:func_s17_80024188 (the scratchpad struct at
   0x1F800000, RotAverage4 out-parameters, the `(u32)x < 0x1E0' OT gate) and
   csrc/ovl_dungeon_800A4B88.c:func_800AFBFC (the chained
   `sp->unk_70[0].vx = sp->unk_70[2].vx = ...' corner assignment).
   csrc/ovl_dng_b02.c:func_b02_801750E4 is the nearest shape -- the same
   POLY_GT4 ribbon off the same +0x8D0 ring -- but it calls SetPolyGT4 and
   AddPrim as functions where this one has setPolyGT4 and addPrim inlined
   from psyq/include/libgpu.h.

   THE RESIDUE, exactly.  All 14 rows are two instances of ONE fault: in each
   of the two signed `% 0x1000' expansions the SUM pseudo and the CORRECTION
   pseudo hold each other's register.

     rows 84-98   target  addu $a0,$v0,$t1 / addu $a1,$a0,$zero /
                          bgez $a0 / addiu $a1,$a0,0xFFF /
                          sra $v0,$a1,12 / sll $v0,$v0,12 / subu $s3,$a0,$v0
                  mine    addu $a1,$v0,$t1 / addu $a0,$a1,$zero /
                          bgez $a1 / addiu $a0,$a1,0xFFF /
                          sra $s3,$a0,12 / sll $v0,$s3,12 / subu $s3,$a1,$v0
     rows 118-124 target  addu $v0,$v0,$t1 / bgez $v0 /
                          addu $s0,$v0,$zero / addiu $s0,$v0,0xFFF /
                          sra $s0,$s0,12 / sll $s0,$s0,12 / subu $s0,$v0,$s0
                  mine    addu $s0,$v0,$t1 / bgez $s0 /
                          addu $v0,$s0,$zero / addiu $v0,$s0,4095 /
                          sra $v0,$v0,12 / sll $v0,$v0,12 / subu $s0,$s0,$v0

   Nothing else differs: the frame is -0x58 with the same nine callee-saved
   saves in the same order and the same slots, the whole prologue, the
   prim-dressing block, the RotAverage4 argument build, the eight screen-pair
   copies, the inlined addPrim and the epilogue are word-identical, and q and
   the loop counter now hold the registers the target gives them.

   TWO FACTS WERE MEASURED AND BOTH ARE LOAD-BEARING.  They were found in
   this order and each reproduces when the other is removed.

   1. THE +0xA SCRATCHPAD READ-BACK ORDER -- the byte-dressing block must be
      clut, then the u/v pairs in setUV4 order, then the twelve colour bytes.
      This is the setUV4 macro's own order from psyq/include/libgpu.h
      (u0,v0,u1,v1,u2,v2,u3,v3) with setRGB0..3 after it, not u-bytes
      followed by v-bytes.  Measured, all other spellings at 243 words:

      | byte-dressing order                              | aligned |
      | clut, setUV4 pairs, colours   (kept)             |   14    |
      | setUV4 pairs, colours, clut                      |   19    |
      | setUV4 pairs, clut, colours                      |   19    |
      | u0..u3, colours, clut, v0..v3                    |   21    |
      | u0..u3, colours, v0..v3, clut                    |   24    |
      | u0..u3, v0..v3, colours, clut                    |   24    |
      | u0..u3, v0..v3, clut, colours                    |   24    |
      | colours, setUV4 pairs, clut                      |   27    |
      | colours, u0..u3, v0..v3, clut                    |   28    |

      The mechanism is sched1, not allocation.  With the u-bytes grouped and
      the v-bytes separate, the `li 0x3F' feeding u1 and u3 is scheduled
      AFTER the whole colour run and after the first modulo sum, so it has to
      share $v0 with the 0x7E00/0x80/0xBF chain; the target has it in $v1 at
      word 67 with its two stores at 69 and 71.

   2. `t2 = <sum>; t2 = t2 % 0x1000;' AS TWO STATEMENTS, and `t' as ONE.
      This is the asymmetric one and it is worth 41 rows:

      | t spelling              | t2 spelling             | aligned |
      | one expression (kept)   | two statements (kept)   |   14    |
      | one expression          | one expression          |   58    |
      | two statements          | two statements          |   98    |
      | two statements          | one expression          |   54    |
      | one expression          | three statements        |   22    |

      With t2 as one expression the exchange is not just inside the modulo:
      the cursor pointer q and the sign-extended loop counter swap $s0 and
      $s1 across the WHOLE body, which is 62 rows at 243 words.  q has 78
      references over 171 insns against the counter's 8 over 23, so
      allocno_compare ranks q first and it takes $s0; splitting t2 is what
      changes the counter allocno enough to reverse that.

   SPELLINGS MEASURED AND REJECTED, all at 243 words unless noted
   (aligned differences):

   | spelling                                                   | rows |
   | `Unk80083160 *gg = &D_80083160' and `gg->unk_00' INSIDE the
     loop, with `D_80083160.unk_00' inline outside it (kept)    |  14  |
   |   ... `D_80083160.unk_00' inline inside the loop too       | 242 words, 69 |
   | `q->tpage' etc. as POLY_GT4 FIELDS (kept)                  |  14  |
   |   ... `u8 *q' with plain casts at every site               |  14  |
   | `i << 8' for the first angle                               |  14  |
   |   ... `i * 0x100'                                          |  14  |
   | `((i + 1) % 0x10) * 0x100 + d' with d second               |  14  |
   |   ... `d + ((i + 1) % 0x10) * 0x100'                       |  14  |
   | `(i << 8) + d' with d second                               |  14  |
   |   ... `d + (i << 8)'                                       |  14  |
   | `t2 = t2 % 0x1000;'                                        |  14  |
   |   ... `t2 %= 0x1000;'                                      |  14  |
   | `s16 i' with the loop `for (i = 0; i < 0x10; i++)'         |  14  |
   |   ... `s32 i' with `(s16)i' at all three uses              | 241 words, 31 |
   | `s16 d' and `s16 f' PARAMETERS                             |  14  |
   |   ... `s32 d' parameter with an `s16 d' local              |  14  |
   |   ... `s32 f' parameter with an `s16 f' local              | 241 words, 34 |
   |   ... both as s32 parameters with s16 locals               | 241 words, 34 |
   | no device                                                  |  14  |
   |   ... `if (1)' around the `t' statement                    |  14  |
   |   ... `do { } while (0)' around the `t' statement          | 242 words, 104 |
   |   ... `do { } while (0)' around the two `t2' statements    | 242 words, 59 |
   |   ... `do { } while (0)' around `t2 = t2 % 0x1000;' alone  |  14  |
   | declaration order sp, gg, q, i, t, t2                      |  14  |
   |   ... q after t2                                           |  14  |
   |   ... gg last                                              |  14  |
   |   ... t2 before t                                          |  14  |
   |   ... i last                                               |  14  |
   | separate scratch local for the t2 sum                      |  62  |
   | separate scratch local for the t sum                       |  14  |
   | `t2' reused as the scratch for `t' as well                 | 245 words, 91 |
   | the t2 block written BEFORE the t block                    |  82  |

   The eleven rows that measure 14 unchanged are FREE, not evidence: the
   residue is bit-identical in every one of them, so none of those spellings
   is why the body is as close as it is.

   WHAT TO TRY NEXT.  The fault is `allocno_compare' inside two
   compiler-generated `%' expansions, so it is not reachable by respelling
   the modulo -- every form of it was priced above.  Run the permuter; this
   is exactly the residue it is for.  Failing that, `-da' on the two sums'
   allocnos: in this build the first modulo's sum is live 84..98 and its
   correction 85..95, and the target gives the LONGER-lived sum the lower
   register, which is the reverse of local-alloc's shorter-first rule, so
   something is adding a conflict on $a0 in the real source.

   DECLARATIONS
   typedef struct UnkDngS10QuadSP
   {
       u8 pad_00[0x18];
       u8 *unk_18;
       u8 pad_1C[0x48];
       SVECTOR unk_64[4];
       s32 unk_84;
       s32 unk_88;
       u8 pad_8C[0x28];
       s32 unk_B4;
       u8 pad_B8[0x20];
       u16 unk_D8;
       u16 unk_DA;
       u16 unk_DC;
       u16 unk_DE;
       u16 unk_E0;
       u16 unk_E2;
       u16 unk_E4;
       u16 unk_E6;
   } UnkDngS10QuadSP;
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   END DECLARATIONS */
s32 func_s10_80024930(u8 *a, UnkVec *b, u8 *c, s16 d, s32 e, s16 f, u8 g, u8 h)
{
    UnkDngS10QuadSP *sp = (UnkDngS10QuadSP *)0x1F800000;
    Unk80083160 *gg = &D_80083160;
    POLY_GT4 *q;
    s16 i;
    s32 t;
    s32 t2;

    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    sp->unk_64[0].vz = sp->unk_64[1].vz = *(u16 *)((u8 *)b + 0xA) - e + 1;
    sp->unk_64[3].vz = sp->unk_64[2].vz = *(u16 *)((u8 *)b + 0xA);
    for (i = 0; i < 0x10; i++)
    {
        q = *(POLY_GT4 **)(gg->unk_00 + 0x8D0);
        *(u32 *)(gg->unk_00 + 0x8D0) = (u32)q + 0x34;
        setPolyGT4(q);
        if (g == 4)
        {
            q->tpage = GetTPage(0, 0, 0x280, 0x100);
        }
        else
        {
            setSemiTrans(q, 1);
            q->tpage = GetTPage(0, g, 0x280, 0x100);
        }
        q->clut = 0x7E00;
        q->u0 = 0;
        q->v0 = 0x80;
        q->u1 = 0x3F;
        q->v1 = 0x80;
        q->u2 = 0;
        q->v2 = 0xBF;
        q->u3 = 0x3F;
        q->v3 = 0xBF;
        q->r0 = 0;
        q->g0 = 0;
        q->b0 = 0;
        q->r1 = 0;
        q->g1 = 0;
        q->b1 = 0;
        q->r2 = h;
        q->g2 = h;
        q->b2 = h;
        q->r3 = h;
        q->g3 = h;
        q->b3 = h;
        t = ((i << 8) + d) % 0x1000;
        sp->unk_64[0].vx = sp->unk_64[2].vx =
            *(u16 *)((u8 *)b + 0x2) + ((func_800644B8(t) >> 4) * f >> 8);
        t2 = ((i + 1) % 0x10) * 0x100 + d;
        t2 = t2 % 0x1000;
        sp->unk_64[1].vx = sp->unk_64[3].vx =
            *(u16 *)((u8 *)b + 0x2) + ((func_800644B8(t2) >> 4) * f >> 8);
        sp->unk_64[0].vy = sp->unk_64[2].vy =
            *(u16 *)((u8 *)b + 0x6) + ((func_80064584(t) >> 4) * f >> 8);
        sp->unk_64[1].vy = sp->unk_64[3].vy =
            *(u16 *)((u8 *)b + 0x6) + ((func_80064584(t2) >> 4) * f >> 8);
        sp->unk_B4 = RotAverage4(&sp->unk_64[0], &sp->unk_64[1], &sp->unk_64[2], &sp->unk_64[3],
                                 (long *)&sp->unk_D8, (long *)&sp->unk_DC, (long *)&sp->unk_E0,
                                 (long *)&sp->unk_E4, (long *)&sp->unk_84, (long *)&sp->unk_88);
        q->x0 = sp->unk_D8;
        q->y0 = sp->unk_DA;
        q->x1 = sp->unk_DC;
        q->y1 = sp->unk_DE;
        q->x2 = sp->unk_E0;
        q->y2 = sp->unk_E2;
        q->x3 = sp->unk_E4;
        q->y3 = sp->unk_E6;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
        }
    }
    return 0;
}
