/* PARK: func_b39_80174B98, csrc/ovl_dng_b39.c, agent-h211, 2026-11-26.

   TARGET 375 words. BEST BUILT 375 WORDS, 5 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports 19 differing unaligned.
   Read the aligned figure. 1 member (dngfam.py) -- no replication value.
   relcand: ok, 3 data symbols and 6 calls all correct. declcheck clean.

   VERDICT: ALLOC, at the right word count, with EXACTLY ONE remaining fault
   and that fault is named, measured and traced to the line of loop.c that
   makes it. Every opcode, immediate, displacement and store site below came
   off the listing and 370 of the 375 words are byte-exact.

   WHAT IT DOES. The 0x201 node renderer, drawn once per node of the tag
   chain off p - 8. Per node: seed four SVECTORs as a 0x60-wide, 0xB0-deep
   quad (vx +/-0x30, vy 0, vz 0x10 or -0xA0 by parity), rotate all four in
   place through func_800DBA90 with -D_80083160+0xC8 as the yaw and the
   node payload's +0xA2 eight bytes as the translation, RotTransPers each
   corner while accumulating (result - 8) and tracking a packed min/max xy
   box in two s32, average the depth by 4, turn the box into an x/y/w/h
   rect at sp+0x10 raising y by 0xE0 off the D_801C9E40 lean flag, and if
   the averaged depth is under 0x1E0 emit, for each of four sub-passes and
   each scanline from rect.y + taper to the 224-clipped bottom, one
   semi-transparent POLY_FT4 whose colour word is the slot's +0xC, whose
   u-span is the rodata byte at D_b39_8017089C[4], whose v-span is
   D_b39_8017089C[6..7] scaled by the scanline fraction, whose x comes from
   func_800644B8 of the per-pass angle and whose OT row is the averaged
   depth.

   DONORS. The resident func_800DB660 (csrc/ovl_dungeon_800D50FC.c, 268
   words) gave the chain-walk skeleton, the +0x8D0 primitive cursor, the
   inlined addPrim and the `u8 rec[0x20]` record type. The real donor is the
   PARKED func_b05_80174228 (docs/dng-parks/func_b05_80174228.c, 352 words,
   agent-h202) which is the same machine with a 23-segment beam instead of
   this scanline fan; FIVE of its stated facts carried here verbatim and are
   re-priced below. func_b04_801743EC (csrc/ovl_dng_b04.c) gave the
   `memcpy(dst, src, 8)` spelling for the two alignment-1 eight-byte copies.

   FRAME, read off the listing and reproduced exactly (0xB0, ten saves at
   0x88..0xAC): 0x10 outgoing args, `u16 rc[4]` at 0x10, `u8 oct[8]` at
   0x18, `SVECTOR sv[4]` at 0x20, `u8 rec[0x20]` at 0x40, `s32 xy` at 0x60,
   `s32 w` at 0x64 -- `xy` and `w` are address-taken (RotTransPers' sxy, p
   and flag) which is why they are in memory -- then SEVEN reload spill
   slots at 0x68 (`e`), 0x6C (`bot`), 0x70 (`start`), 0x74 (`z`), 0x78
   (`svp`), 0x7C (the hoisted `bot - start + 1`) and 0x80 (the hoisted
   `z * 4`).

   THE SPILL SLOT ORDER IS DECLARATION ORDER AND IT IS WORTH 17 ROWS. This
   is the single largest fact on this body and it is general. reload's
   `alter_reg` walks pseudos in REGNO order and `assign_stack_local` hands
   out slots as it goes, and a VAR_DECL`s pseudo number is fixed at
   expand_decl time, i.e. by DECLARATION ORDER. So the five spilled locals
   must be declared in the order the listing`s slots run: `e` (0x68),
   `bot` (0x6C), `start` (0x70), `z` (0x74), `svp` (0x78). Declared in the
   order a person would write them -- svp, e, z, start, bot -- the slots
   come out 0x68=svp, 0x6C=e, 0x70=z, 0x74=start, 0x78=bot and the body is
   375 words / 38 aligned differences. Reordering them alone took it to 21.
   The tell is that `aligndiff` prints rows whose two sides read IDENTICAL:
   `lw t0,112(sp)` against `lw $t0, 0x74($sp)` is 112 == 0x70 != 0x74, so a
   row that looks like a tool artifact is really a different stack offset.
   THREE different orders satisfying the five constraints all measure 21, so
   only the relative order of the SPILLED five matters.

   MEASURED FACTS, every one re-measured on this body, each from the body as
   written below with one thing changed:

   | spelling                                                 | words | aligned |
   | as written below                                          |  375  |    5    |
   | the five spilled locals in natural declaration order       |  375  |   38    |
   | `s32 my` instead of `s16 my`                               |  376  |   12    |
   | `s32 ux` / `s32 uy` instead of `u16`                       |  375  |   20    |
   | `s32 uy` only (`u16 ux` kept)                              |  375  |   14    |
   | DVECTOR mn/mx instead of two s32 with half masks           | (b05: 354/198 -- not re-run, the frame moves) |
   | `ang = ang + w * 4 + 0x50`                                 |  375  |   16    |
   | `ang = ang + 0x50 + w * 4`                                 |  375  |   28    |
   | the `ang +=` AFTER the primitive-cursor fetch              |  376  |   47    |
   | `i - i / 7 * 224 - w` (the /7 magic read as a divide by 7) |  375  |   50+   |
   | `ax` read before `ux` in the X half                        |  375  |   27    |
   | `uy` read before `ay` in the Y half                        |  376  |   37    |
   | ay/uy merged into ax/ux                                    |  376  |   59    |
   | `t->v0` inlined in both arms of the +0x25 gate             |  375  |   10    |
   | the +0x25 gate as assign-then-overwrite (b05's fact)       |  375  |   13    |
   | `mx = ux | (mx & 0xFFFF0000)` as one statement             |  374  |   25    |
   | `m2 = mx & 0xFFFF0000; mx = ux | m2`                       |  374  |   25    |
   | `memcpy(oct, D_b39_8017089C, 8)` with the array operand    |  375  |  200+   |
   | no `e = p` cursor copy at the top of the `while`           |  375  |   16    |
   | `svp[i]` as `sv[i]`, no pointer local                      | (b05: 360/150 -- not re-run) |
   | `vz` if/else written as assign-then-overwrite              |  374  |    5    |
   | `vz` if/else inverted                                      |  375  |    6    |
   | `vz` as a ternary                                          |  375  |    5    |
   | the `vz` if/else FIRST in the loop body                    |  375  |   13    |
   | `z >>= 2` in any of four slots around the rc stores        |  375  |    5    |

   FIVE OF THOSE GENERALISE AND ARE NEW:

   (1) `s16 my` IS WORTH 7 ROWS AND IT IS A MODE FACT, NOT AN ALLOCATION
   ONE. `my = mn >> 16` feeds `rc[1]` (a u16 store) and `rc[3] = (mx >> 16)
   - my`. As `s32` the pseudo is SImode and sched1 orders the whole
   post-loop block differently -- the `z` reload lands first with a `nop` in
   its load-delay slot and the `rc[0]` store at the end; as `s16` the
   `sra $a1, $s2, 16` is scheduled first exactly as the target has it and
   six schedule rows plus a word vanish. The companion fact is that the
   value must be a LOCAL at all: without it, `mn >> 16` is computed twice
   because the lean-flag `if` ends the cse block between the two uses.

   (2) `u16` ON THE UNSIGNED HALF-READS IS WORTH 8 ROWS. `ux`/`uy` hold
   `*(u16 *)&xy` purely to be OR-ed into the packed box. As `s32` the
   pseudos are SImode and the Y pair's two registers come out EXCHANGED
   against the X pair's ($a0/$v1 crossed) with every instruction in place --
   the textbook "allocation fault" residue. As `u16` they are HImode, which
   is what the target's `lhu` pair says they are, and the exchange goes
   away. Note the asymmetry is real: `ax`/`ay` stay `s32` (as `s16` the body
   is 376/25), because those feed `slt` comparisons that want the full
   sign-extended word. SO: TYPE THE HALF-READ BY WHAT CONSUMES IT, and when
   a two-register exchange is confined to ONE of two structurally identical
   blocks, suspect a MODE difference before an allocation one.

   (3) THE `0x92492493` MAGIC IS A DIVIDE BY 224 HERE, NOT BY 7, AND THE
   SHIFT TELLS YOU WHICH. The same multiplier serves every divisor of the
   form 7 * 2^k; `mulhi(x, 0x92492493) + x` then `>> s` then subtract the
   sign is d = 7 for s = 2 and d = 224 for s = 7. Read the `sra` AMOUNT
   after the `addu`, not the constant. Writing the three sites as `/ 7` and
   `* 224` gives the right multiplier, the wrong shift and 50+ rows; `/ 224`
   and `i % 224` are exact, and `i % 224` is what produces the target's
   quotient / `sll 3` / `subu` / `sll 5` / `subu` tail.

   (4) `ang += 0x50 + w * 4` IS THE ONLY ONE OF SIX SPELLINGS THAT GROUPS
   THE WAY THE TARGET DOES. The target computes `(ang + 0x50) + w * 4`
   (`addiu $a0, $s4, 0x50` then `addu $s4, $a0, $v0`). `ang = ang + 0x50 +
   w * 4` gives `ang + (w * 4 + 0x50)`, `ang = ang + w * 4 + 0x50` gives
   `(ang + w * 4) + 0x50`, and two separate `+=` statements collapse to one
   accumulate. Only the compound-assignment form with the constant first
   inside the operand reaches the target's grouping. This is the additive
   lever the levers file warns about, with a fourth witness.

   (5) THE `ang` UPDATE IS THE FIRST STATEMENT OF THE INNER LOOP BODY,
   BEFORE THE PRIMITIVE-CURSOR FETCH. Written after it the body is 376 words
   / 47 aligned differences; the `addiu $a0, $s4, 0x50` is the target's
   first insn in the block and the whole cursor/colour dress schedules
   around it.

   THE REMAINING FAULT, named and priced: THE -0xA0 CONSTANT IS HOISTED OUT
   OF THE OUTER LOOP AND LOSES ITS REGISTER. The target keeps both quad
   constants in the outer loop's head (`addiu $a3, $zero, -0xA0` and
   `addiu $a2, $zero, 0x10`); this body keeps only the 0x10 there and
   rematerialises `-0xA0` inside the build loop as `li $t0, -160`. Same word
   count, 5 aligned rows, no shape difference.

   The mechanism, read out of the `-da` `.loop` dump and then out of
   loop.c, and it is the useful part of this park:

     Loop from 51 to 136: 18 real insns.
     Insn 105: regno 130 (life 1), move-insn savings 1  moved to 961
     Insn 120: regno 134 (life 1), move-insn savings 1  moved to 963
     Loop from 34 to 900: 305 real insns.
     Insn 961: regno 130 (life 30), savings 1 halved since already moved  moved to 974
     Insn 963: regno 134 (life 30), savings 1 halved since already moved  not desirable

   Both constants are hoisted out of the BUILD loop into its preheader by
   the inner pass, which sets `moved_once` on both. The outer pass then
   applies `move_movables`'s desirability test at
   scratch/gcc-src/cdk-gcc-b18/loop.c:1722,

       if (already_moved[regno]
           || (threshold * savings * m->lifetime) >= insn_count
           || ...)

   and the preceding `if (moved_once[regno]) insn_count *= 2;`. INSN_COUNT
   IS A RUNNING LOCAL: it is doubled once per already-moved movable the
   chain reaches, so the FIRST of the two sees 305 * 2 = 610 and passes
   while the SECOND sees 1220 and fails. Both have savings 1 and lifetime
   30, so the order in the chain -- which is their order in the build loop
   body -- decides which one is hoisted out of the outer loop, and the one
   that is hoisted then spans 538 insns across 6 calls (`.lreg`: `Register
   130 used 4 times across 538 insns; crosses 6 calls; 2 bytes`), needs a
   callee-saved register, finds none because all ten are taken, and reload
   rematerialises it from its REG_EQUIV constant. The 0x10 stays at 38 insns
   and 0 calls and takes $a2 unopposed.

   SO THE TARGET'S BUILD LOOP MUST FAIL THE TEST FOR *BOTH*, which needs
   either `threshold * 30 < 610` or one more already-moved movable ahead of
   them in the chain. Bounds measured from the two outcomes:
   threshold * 30 is in [610, 1220). Inverting the two arms just moves the
   fault to the 0x10 (375/6). Putting the whole `vz` if/else first in the
   loop body shortens both lifetimes and still does not flip it (375/13), so
   threshold >= 28. The two candidates NOT tried, and this is where the next
   agent should start:
     * a spelling whose build loop carries a THIRD outer-invariant movable
       that the inner pass also moved, which would double insn_count once
       more before either constant is tested;
     * a spelling that stops the INNER pass moving them at all, which makes
       `moved_once` false and leaves insn_count at 305 with lifetime ~1, far
       under the threshold -- but the inner gate is
       `2 * (1 + n_non_fixed_regs) * 1 * 1 >= 18`, which no source change
       reaches.
   Eight spellings of the two arms were measured (if/else both polarities,
   assign-then-overwrite both polarities, a ternary, `i % 2`, the arm
   duplicated, the arm inside `do { } while (0)`) and NONE improves on 5;
   four of them measure exactly 5, so the construct is NOT what decides it.

   NOT MEASURABLE, recorded so nobody re-runs it: the position of
   `z >>= 2` among the four slots around the `rc[0]`/`rc[2]` stores is
   375 words / 5 rows in all four, and so is `z = z >> 2` against `z >>= 2`.
   A flat sweep means the position is not a lever here.

   THE X AND Y HALVES OF THE BOX ARE NOT SYMMETRIC IN THE SOURCE, which is
   worth stating because it looks like a transcription error. The X insert
   is `mx = ux | mx` and the Y insert is `mx |= uy << 16`; the target emits
   `or $s3, $v1, $s3` for the first and `or $s3, $s3, $v0` for the second.
   The Y form is exact as written. The X form is NOT reachable from a
   two-statement spelling at all: expand_binop swaps a commutative
   operation's operands whenever `target == op1`
   (scratch/gcc-src/cdk-gcc-b18/optabs.c, the `commutative_op` block), so
   both `mx = ux | mx` and `mx |= ux` emit `or $s3, $s3, $v1`. The one-
   statement `mx = ux | (mx & 0xFFFF0000)` DOES emit `or $s3, $v1, $s3` --
   confirmed -- but routes the `and` through a temporary and loses a
   duplicated `sra` in a delay slot, for 374 words / 25 rows. Those 2 rows
   are the second-smallest fault on the body and they are a separate
   question from the -0xA0.

   DECLARATIONS
   void func_800DBA90(u8 *);
   s32 func_800644B8(s32);
   extern const u8 D_b39_8017089C[];
   END DECLARATIONS
*/
s32 func_b39_80174B98(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g;
    u16 rc[4];
    u8 oct[8];
    SVECTOR sv[4];
    u8 rec[0x20];
    s32 xy;
    s32 w;
    POLY_FT4 *t;
    UnkNode *n;
    u8 *e;
    u8 *s;
    s32 mn;
    s32 mx;
    s32 i;
    s16 j;
    s32 k;
    s32 m;
    s32 y;
    s32 y2;
    s16 my;
    s32 ax;
    u16 ux;
    s32 ay;
    u16 uy;
    s32 ang;
    s32 bot;
    s32 top;
    s32 start;
    s32 z;
    SVECTOR *svp;

    s = (u8 *)D_b39_8017089C;
    memcpy(oct, s, 8);
    g = &D_80083160;
    z = 0;
    svp = sv;
    while (1)
    {
        e = p;
        w = 0x30;
        for (i = 3; i >= 0; i--)
        {
            svp[i].vx = w;
            if (i < 2)
            {
                svp[i].vx = -w;
            }
            svp[i].vy = 0;
            if ((i & 1) != 0)
            {
                svp[i].vz = -0xA0;
            }
            else
            {
                svp[i].vz = 0x10;
            }
        }
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        *(u8 **)rec = (u8 *)svp;
        *(u8 **)(rec + 0x4) = (u8 *)svp;
        *(s16 *)(rec + 0xC) = -*(u16 *)((u8 *)g + 0xC8);
        memcpy(rec + 0x10, e + 0xA2, 8);
        *(s16 *)(rec + 0x18) = 4;
        *(s16 *)(rec + 0x1A) = 0;
        func_800DBA90(rec);
        mn &= 0xFFFF;
        mn |= 0x75300000;
        mn &= 0xFFFF0000;
        mn |= 0x7530;
        mx &= 0xFFFF;
        mx |= 0x8AD00000;
        mx &= 0xFFFF0000;
        mx |= 0x8AD0;
        for (i = 3; i >= 0; i--)
        {
            z += RotTransPers(&svp[i], (long *)&xy, (long *)&w, (long *)&w) - 8;
            ux = *(u16 *)&xy;
            ax = *(s16 *)&xy;
            if ((s16)mx < ax)
            {
                mx &= 0xFFFF0000;
                mx = ux | mx;
            }
            else if (ax < (s16)mn)
            {
                mn &= 0xFFFF0000;
                mn = ux | mn;
            }
            ay = *(s16 *)((u8 *)&xy + 0x2);
            uy = *(u16 *)((u8 *)&xy + 0x2);
            if ((mx >> 16) < ay)
            {
                mx &= 0xFFFF;
                mx |= uy << 16;
            }
            else if (ay < (mn >> 16))
            {
                mn &= 0xFFFF;
                mn |= uy << 16;
            }
        }
        my = mn >> 16;
        rc[0] = mn;
        rc[2] = mx - mn;
        z >>= 2;
        y = my;
        if (g->unk_00 != D_801C9E40)
        {
            y += 0xE0;
        }
        rc[1] = y;
        rc[3] = (mx >> 16) - my;
        if ((u32)z < 0x1E0)
        {
            k = *(s16 *)(e + 0x96) - 0x34;
            if (k < 0)
            {
                k = 0;
            }
            k = *(s16 *)&rc[3] * k / 8;
            bot = *(s16 *)&rc[1] + *(s16 *)&rc[3];
            top = bot;
            if (*(s16 *)&rc[1] / 224 != bot / 224)
            {
                top = bot / 224 * 224;
            }
            for (w = 4; w > 0; w--)
            {
                ang = (*(s16 *)(e + 0x96) + w) * 128;
                if ((w & 1) == 0)
                {
                    ang += 0x800;
                }
                start = *(s16 *)&rc[1] + k;
                for (i = start; i <= top; i++)
                {
                    ang += 0x50 + w * 4;
                    t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                    *(u32 *)&t->r0 = *(s32 *)(c + 0xC);
                    SetPolyFT4(t);
                    SetSemiTrans((u8 *)t, 1);
                    t->tpage = GetTPage(2, (c[0xC] ^ 0x80) != 0, 0x340, 0x100);
                    t->u0 = t->u1 = 0;
                    t->u2 = t->u3 = oct[4];
                    t->x0 = bot - start;
                    if (t->x0 == 0)
                    {
                        t->x0 = bot - start + 1;
                    }
                    t->v0 = t->v2 = *(s16 *)(oct + 0x6) * (i - start) / t->x0;
                    t->v3 = *(s16 *)(oct + 0x6) / t->x0;
                    y2 = t->v0;
                    if (t->v3 >= 2)
                    {
                        m = y2 + t->v3;
                    }
                    else
                    {
                        m = y2 + 2;
                    }
                    t->v1 = t->v3 = m;
                    j = rc[0] + (func_800644B8(ang) >> 9);
                    t->x0 = t->x1 = j;
                    j += rc[2];
                    t->x2 = t->x3 = j;
                    j = i % 224 - w;
                    t->y0 = t->y2 = j;
                    j += 2;
                    t->y1 = t->y3 = j;
                    addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
                }
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        c = (u8 *)n->unk_0C;
    }
    return 0;
}
