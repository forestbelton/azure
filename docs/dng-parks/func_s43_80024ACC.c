/* PARK: func_s43_80024ACC, csrc/ovl_dng_s43.c, agent-h184, 2026-11-25.

   TARGET 496 words. BEST BUILT 500 words, 23 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 410 differing, which is the
   four-word shift wake the brief warns about -- price this body with
   aligndiff only).
   1 member, 496 words (tools/decomp/dngfam.py: func_s43_80024ACC alone, no
   replication value beyond its own body).

   THE OTHER NINE FUNCTIONS OF ovl_dng_s43 ALL LANDED at 0 differing in the
   same run, so this park is the only stub left in the unit. Two of those
   landings are the donors for this one: func_s43_800246C0 (259 words) and
   func_s43_8002429C (265 words) are the same per-segment primitive emitter
   with a different primitive, and BOTH landed at 0. This body is
   func_s43_800246C0's loop with TWO POLY_G4 per iteration instead of one,
   the row index 409 * (2i + 1) instead of i * 0x333, and the x/y centres
   carried in two locals across the halves instead of read from the payload.

   WHAT IT DOES. p is the 0x212 node payload func_s43_80025874 spawns (the
   layout is in that function's landing: +0x40..+0x42 the RGB, +0xA the
   angle, +0xE the radius, +0x10 the z offset, +0x12 the texture page,
   +0x18 + i*4 and +0x2C + i*4 two five-entry 16.16 world x/y rings) and
   `b' is the node's UnkVec. Five iterations, two semi-transparent
   gouraud quads each (len 8, code 0x3A, 0x24 bytes off
   D_80083160.unk_00's +0x8D0 cursor), each quad spanning ring entry i
   (first half) and (i + 1) % 5 (second half) against the per-iteration
   centre (cx, cy) = b->unk_00/unk_04 plus rsin/rcos of 409 * (2i + 1) +
   p+0xA scaled by p+0xE << 8. The four SVECTORs go to scratchpad
   0x1F800064/6C/74/7C, RotAverage4 projects them into 0x1F8000D8..E7, and
   when the returned otz is below 0x1E0 the quad and a 0xC-byte DR_MODE
   carrying GetTPage(0, p+0x12, 0, 0) are addPrim'd onto
   sp->unk_18 + otz * 4. `e' and `f' are the dispatcher's p+0x6 / p+0x8
   frame counter and limit; while e < f the two 16.16 deltas are scaled by
   e/f, which is what opens the quad out over the animation.

   THE WHOLE 23-ROW RESIDUE IS IN ONE REGION -- rows 16..161 of 496, the
   prologue plus the first half up to the first RotAverage4. Rows 162..485
   are byte-identical. Two symptoms, and they are ONE fault:

     1. MY BUILD HOISTS THE ADDRESS 0x1F80006C (RotAverage4's second
        argument) OUT OF THE LOOP as a full `lui'/`ori' constant pair and
        spills it in the prologue (+3 words at rows 16-18). The target
        hoists ONLY 0x1F800064 that way and computes 0x6C in the loop as
        `addiu $t0, $s2, %lo(D_1F80006C)' -- twice, because reorg fills the
        second `if (e < f)' gate's beqz delay slot with it (target rows 139
        and 147) -- then spills it to $sp+0x38 for the second half.
     2. `cx' AND `cy' BOTH LOSE THEIR REGISTERS. The target keeps cx in $s0
        across the func_80064584 call (`addu $s0, $s0, $v0' in the jal's
        delay slot) and cy in $a2 until `subu $v0, $a2, $v0'; my build
        computes cx into $v1 BEFORE the jal and spills it in the delay slot,
        then reloads both cx and cy to form the two deltas (+1 word each for
        the early spill and the extra cy reload).

     Rows 0..46 are byte-identical, so the prologue's $s0..$s7/$fp
     assignment is already the target's and $s0 is provably free where the
     target puts cx in it. loop.c runs before local-alloc and global-alloc
     (toplev.c:3548/3585), so (1) is the CAUSE and (2) the consequence: the
     extra loop-invariant allocno is what shifts global.c's assignment
     order. **Anything that stops 0x1F80006C being hoisted should land this
     body**, and I could not find the spelling.

   MEASURED AND REJECTED, all from the 500/23 state with aligndiff
   (words given because several spellings change them):

   | spelling                                                  | words | rows |
   |-----------------------------------------------------------|-------|------|
   | a SEPARATE `r2' for the second half's ring cursor   KEPT  |  500  |  23  |
   |   ... one `r' shared by both halves                       |  501  |  35  |
   | `g = &D_80083160' ABOVE the loop                    KEPT  |  500  |  23  |
   |   ... `g' assigned INSIDE the loop                        |  495  |  29  |
   |   ... no `g', `D_80083160.unk_00' at all four sites       |  495  |  29  |
   | `if (e < f)' for the two delta scalings             KEPT  |  500  |  23  |
   |   ... `if (f > e)'                                        |  500  |  29  |
   | `cx = b->unk_00 + (...)' as ONE statement           KEPT  |  500  |  23  |
   |   ... `(...) + b->unk_00' (both)                          |  500  |  25  |
   |   ... `(...) + b->unk_04' (cy only)                       |  500  |  25  |
   |   ... split into `cx = (...); cx = cx + b->unk_00;'       |  500  |  86  |
   |   ... `do { cx = ...; } while (0)'                        |  501  |  25  |
   |   ... `do { cx = ...; cy = ...; } while (0)'              |  509  |  84  |
   | `s32 i'                                             KEPT  |  500  |  23  |
   |   ... `s16 i'                                             |  508  | 123  |
   | `s16 e, s16 f' parameters                           KEPT  |  500  |  23  |
   |   ... `s32 e = e0; s32 f = f0;' widened in the body       |  506  |  55  |
   | the RotAverage4 argument addresses as `&sp->unk_XX' KEPT  |  500  |  23  |
   |   ... all ten as `(SVECTOR *)0x1F8000XX' literals         |  499  | 174  |
   |   ... the four SVECTORs as `((u32)sp | 0xXX)'             |  500  |  25  |
   | `r = p + i * 4;' AFTER the colour block             KEPT  |  500  |  23  |
   |   ... before the `cx' statement                           |  507  | 136  |

   NOT MEASURABLE -- identical 500 words / 23 rows, so these say nothing
   about the target and should not be re-measured:
   every permutation of the declaration order tried (cx/cy first, i first);
   `q' and `m' merged into one local; the four SVECTOR arguments and the
   four sxy arguments as plain `((u8 *)sp + 0xXX)' casts; the p/flag pair as
   plain casts; `b->unk_00'/`b->unk_04' as plain `*(s32 *)((u8 *)b + 0x0)'
   casts; the whole scratchpad reached through a `u8 *sp' with plain casts
   instead of the typedef; `<< 8' written `* 0x100'; `(2 * i + 1)' for
   `(i * 2 + 1)'; `u32 cx/cy'; `r' and `r2' exchanged between the halves;
   `sp->unk_18 = g->unk_00 + 0xB0' for the direct symbol; `g = &D_80083160'
   placed after that store.
   Per the brief, declaration order measuring identically in every
   permutation rules OUT allocation priority as the thing to lever and puts
   the cause in the arithmetic or the statement order -- which is why I
   swept the statement positions above rather than reaching for a device.

   FACTS THAT CARRY FROM THE TWO LANDED SIBLINGS and are already in this
   body (all three bodies share the scratchpad typedef and the emitter):
     * `Unk80083160 *g = &D_80083160' ABOVE the loop with `g->unk_00' at
        every in-loop site and the DIRECT `D_80083160.unk_00' at the one
        pre-loop `sp->unk_18' store. On func_s43_800246C0 this is the whole
        2-word/7-row difference: `g' inside the loop is 257 words / 18 and
        no `g' at all is 257 / 18, against 259 / 0 this way.
     * the chains emit the INNER lvalue first, so the target's
        `sh 0x74' before `sh 0x64' is `sp->unk_64 = sp->unk_74 = v' and its
        `sh 0x78' before `sh 0x80' is `sp->unk_80 = sp->unk_78 = v'.
        Getting the last one backwards is 4 rows on func_s43_800246C0.
     * the SVECTOR and sxy slots must be u16 FIELDS, not `SVECTOR'/`DVECTOR'
        members: func_s43_8002429C read-modify-writes them and the target's
        loads are `lhu'. The typedef below is shared by all three and is
        what both landed bodies use.
     * `m', a SECOND pointer local for the DR_MODE, is load-bearing on
        func_s43_800246C0 (one `q' for both primitives is 259 words / 7
        rows there, all of them the last addPrim's `and $s0, $s0, $s4'
        dying-input tie) and NOT MEASURABLE here.

   DECLARATIONS
   typedef union UnkDng2429CW
   {
       s32 w;
       u16 h[2];
   } UnkDng2429CW;
   typedef struct UnkDng2429CSP
   {
       u8 pad_00[0x18];
       u8 *unk_18;
       u8 pad_1C[0x48];
       u16 unk_64;
       u16 unk_66;
       u16 unk_68;
       u8 pad_6A[0x2];
       u16 unk_6C;
       u16 unk_6E;
       u16 unk_70;
       u8 pad_72[0x2];
       u16 unk_74;
       u16 unk_76;
       u16 unk_78;
       u8 pad_7A[0x2];
       u16 unk_7C;
       u16 unk_7E;
       u16 unk_80;
       u8 pad_82[0x2];
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
       u8 pad_E8[0xC];
       s32 unk_F4;
       s32 unk_F8;
       u8 pad_FC[0xC];
       UnkDng2429CW unk_108;
       UnkDng2429CW unk_10C;
       s32 unk_110;
   } UnkDng2429CSP;
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   void func_80067F20(u32 *, s32, s32, s32, s32);
   END DECLARATIONS
*/
void func_s43_80024ACC(u8 *p, UnkVec *b, s16 e, s16 f)
{
    UnkDng2429CSP *sp = (UnkDng2429CSP *)0x1F800000;
    Unk80083160 *g;
    u8 *q;
    u8 *m;
    u8 *r;
    u8 *r2;
    s32 cx;
    s32 cy;
    s32 i;

    g = &D_80083160;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < 5; i++)
    {
        cx = b->unk_00 +
             ((func_800644B8((i * 2 + 1) * 0x199 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE)
              << 8);
        cy = b->unk_04 +
             ((func_80064584((i * 2 + 1) * 0x199 + *(s16 *)(p + 0xA)) >> 4) * *(s16 *)(p + 0xE)
              << 8);
        q = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0x24);
        q[0x3] = 8;
        q[0x7] = 0x3A;
        q[0x4] = p[0x40];
        q[0x5] = p[0x41];
        q[0x6] = p[0x42];
        q[0xC] = p[0x40];
        q[0xD] = p[0x41];
        q[0xE] = p[0x42];
        q[0x14] = p[0x40];
        q[0x15] = p[0x41];
        q[0x16] = p[0x42];
        q[0x1C] = p[0x40];
        q[0x1D] = p[0x41];
        q[0x1E] = p[0x42];
        r = p + i * 4;
        sp->unk_64 = sp->unk_74 = *(u16 *)(r + 0x1A);
        sp->unk_108.w = cx - *(s32 *)(r + 0x18);
        if (e < f)
        {
            sp->unk_108.w = sp->unk_108.w / f * e;
        }
        sp->unk_6C = sp->unk_7C = *(u16 *)(r + 0x1A) + sp->unk_108.h[1];
        sp->unk_66 = sp->unk_76 = *(u16 *)(r + 0x2E);
        sp->unk_10C.w = cy - *(s32 *)(r + 0x2C);
        if (e < f)
        {
            sp->unk_10C.w = sp->unk_10C.w / f * e;
        }
        sp->unk_6E = sp->unk_7E = *(u16 *)(r + 0x2E) + sp->unk_10C.h[1];
        sp->unk_68 = sp->unk_70 = *(u16 *)((u8 *)b + 0xA) - *(u16 *)(p + 0x10);
        sp->unk_80 = sp->unk_78 = *(u16 *)((u8 *)b + 0xA);
        sp->unk_B4 = RotAverage4((SVECTOR *)&sp->unk_64, (SVECTOR *)&sp->unk_6C,
                                 (SVECTOR *)&sp->unk_74, (SVECTOR *)&sp->unk_7C,
                                 (long *)&sp->unk_D8, (long *)&sp->unk_DC, (long *)&sp->unk_E0,
                                 (long *)&sp->unk_E4, (long *)&sp->unk_84, (long *)&sp->unk_88);
        *(s16 *)(q + 0x8) = sp->unk_D8;
        *(s16 *)(q + 0xA) = sp->unk_DA;
        *(s16 *)(q + 0x10) = sp->unk_DC;
        *(s16 *)(q + 0x12) = sp->unk_DE;
        *(s16 *)(q + 0x18) = sp->unk_E0;
        *(s16 *)(q + 0x1A) = sp->unk_E2;
        *(s16 *)(q + 0x20) = sp->unk_E4;
        *(s16 *)(q + 0x22) = sp->unk_E6;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            m = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(m + 0xC);
            func_80067F20((u32 *)m, 0, 0, GetTPage(0, *(s16 *)(p + 0x12), 0, 0) & 0xFFFF, 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, m);
        }
        q = *(u8 **)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0x24);
        q[0x3] = 8;
        q[0x7] = 0x3A;
        q[0x4] = p[0x40];
        q[0x5] = p[0x41];
        q[0x6] = p[0x42];
        q[0xC] = p[0x40];
        q[0xD] = p[0x41];
        q[0xE] = p[0x42];
        q[0x14] = p[0x40];
        q[0x15] = p[0x41];
        q[0x16] = p[0x42];
        q[0x1C] = p[0x40];
        q[0x1D] = p[0x41];
        q[0x1E] = p[0x42];
        r2 = p + (i + 1) % 5 * 4;
        sp->unk_64 = sp->unk_74 = *(u16 *)(r2 + 0x1A);
        sp->unk_108.w = cx - *(s32 *)(r2 + 0x18);
        if (e < f)
        {
            sp->unk_108.w = sp->unk_108.w / f * e;
        }
        sp->unk_6C = sp->unk_7C = *(u16 *)(r2 + 0x1A) + sp->unk_108.h[1];
        sp->unk_66 = sp->unk_76 = *(u16 *)(r2 + 0x2E);
        sp->unk_10C.w = cy - *(s32 *)(r2 + 0x2C);
        if (e < f)
        {
            sp->unk_10C.w = sp->unk_10C.w / f * e;
        }
        sp->unk_6E = sp->unk_7E = *(u16 *)(r2 + 0x2E) + sp->unk_10C.h[1];
        sp->unk_B4 = RotAverage4((SVECTOR *)&sp->unk_64, (SVECTOR *)&sp->unk_6C,
                                 (SVECTOR *)&sp->unk_74, (SVECTOR *)&sp->unk_7C,
                                 (long *)&sp->unk_D8, (long *)&sp->unk_DC, (long *)&sp->unk_E0,
                                 (long *)&sp->unk_E4, (long *)&sp->unk_84, (long *)&sp->unk_88);
        *(s16 *)(q + 0x8) = sp->unk_D8;
        *(s16 *)(q + 0xA) = sp->unk_DA;
        *(s16 *)(q + 0x10) = sp->unk_DC;
        *(s16 *)(q + 0x12) = sp->unk_DE;
        *(s16 *)(q + 0x18) = sp->unk_E0;
        *(s16 *)(q + 0x1A) = sp->unk_E2;
        *(s16 *)(q + 0x20) = sp->unk_E4;
        *(s16 *)(q + 0x22) = sp->unk_E6;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
            m = *(u8 **)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(m + 0xC);
            func_80067F20((u32 *)m, 0, 0, GetTPage(0, *(s16 *)(p + 0x12), 0, 0) & 0xFFFF, 0);
            addPrim(sp->unk_18 + sp->unk_B4 * 4, m);
        }
    }
}
