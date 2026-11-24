/* PARK: func_s01_80024CFC, csrc/ovl_dng_s01.c.  agent-h102, 2026-11-24.

   TARGET 399 words.  BEST BUILT 399 words, 12 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), and ovlverify agrees at 12 masked -- nothing
   is displaced, every one of the 12 rows is a caller-saved REGISTER NAME in
   one 25-word region.  1 member, 399 words, no replication
   (tools/decomp/dngfam.py).

   OWNS NO JUMP TABLE.  Its switch on *(s16 *)(p + 0xA) is a compare chain
   (beq 1 / slti 2 / beqz 0 / beq 2 / beq 3); the listing has no
   .section .rodata and ovl_dng_s01 has no .rodata subsegment at all, only a
   data one -- see the 0x24 bytes at 0x1834800 in splat/dungeon.bin.yaml.

   NO TWIN, EITHER TOOL.  dngtwin --unit ovl_dng_s01 gives func_s31_800247D0
   at 0.899 (333 words) and func_s32_80025374 at 0.785, BOTH STILL
   INCLUDE_ASM.  The shape came out of the LANDED SPAWNER in its own unit --
   func_s01_800245BC names csrc/ovl_dungeon_800D50FC.c's func_800D6AD4 as its
   twin, which is how this family was identified -- plus the callee-set grep
   on D_800DEC00, which lands on csrc/ovl_dng_s67.c's func_s67_80024C50.
   That body supplied the ONE idiom this function could not be guessed from
   the listing: the bank-local 0x20-byte direction table is copied into the
   frame by a STRUCT ASSIGNMENT off a one-byte-aligned extern array, which is
   what gives the lwl/lwr + swl/swr block move at words 14..44.

   RELATED PARK, SAME FAMILY, READ IT: docs/dng-parks/func_s21_80024050.c.
   It is the 471-word master step handler of the same func_8003DF74 caller
   family (ovl_dng_b27, s01, s02, s16..s21, s31..s33, s37..s42, s51, s58,
   s59, s65, s67) and its header already diagnoses THIS EXACT RESIDUE off
   -da: local-alloc combine_regs ties a subtraction destination to its dying
   source operand, and when the two byte loads feeding it have been given
   $v0/$v1 in the other order, record_one_conflict marks both and every later
   caller-saved name shifts.  That park reached 61 rows on 471 words; this
   one reaches 12 on 399, and the 12 are the same mechanism.

   WHAT IT IS.  State 0..3 of the summon beam.  `p` is this node payload,
   `v` its mover, `s` its renderable.  `r = *(u8 **)p` is the CASTER payload,
   `g = (UnkNode *)(r - 0x20)` the caster node and `m = g->unk_08` the
   caster mover.  Every frame bumps the +0x82 tick.
     0: dress the renderable white (0x808080 into the 3 colour bytes as one
        word) at scale 0x1000/0x1000, bind the model with
        func_8003DB94(s, D_800DEC00, 0), latch the caster facing
        ((*(u16 *)(r + 0x2A) >> 9) & 7) at +0x7E, raise the bank tick and
        advance -- then FALL THROUGH into 1.
     1: read the caster hand through func_8003DF74 into a 3-halfword frame
        slot; mirror the caster mover into our own, and either add the hand
        offset or (when the caster slot already carries 0x8000) drop the z by
        0x40; publish the result at +0xB0..+0xB8 and, when the script word at
        **(u16 **)(p + 0x4) has bit 7, zero +0x84, advance and ring
        func_800A56E0(0x300).
     2: every OTHER frame (+0x84 odd) spawn a 0x12 node: seed its +0x58/+0x5C
        velocity from the direction table, point its +0x2C back at the caster,
        copy the caster target at +0x30, set the frame index, the facing and
        the grid step (caster +0x24/+0x25 plus the table byte) and, when the
        caster has a target, set a tile distance at +0x2 of twice the larger
        axis delta and on the first frame divide the remaining height by it
        into the mover +0x14.  Install func_s01_800246DC as its step, copy
        our published position into its mover, dress it white, and on frame 1
        remember it at +0x90/+0x94 with +0x88 = 0x63.  Then, while +0x88 is
        0x63, follow the remembered node: if it has died clear +0x88, else
        take its mover as ours.  At frame 0xB reset and advance.
     3: the same follow block AGAIN, written out, then count +0x84 up to
        +0x86 and -- unless the bank tick is still set, in which case clear
        the tick instead -- zero D_8008346C and raise the 0x8000 kill bit.

   THE 12 ROWS, by target word:

     226/227, 229..234   lb 0x72($s1) is in $v1 here and $v0 in the target,
                         lbu 0x24($a0) the other way round, so the shared
                         `subu` writes the lbu register (`subu $v0,$v1,$v0`)
                         where the target writes the dying lb one
                         (`subu $v0,$v0,$v1`).  7 rows.  This is
                         func_s21_80024050's documented combine_regs tie.
     240/241             the constant 1 for the `+0x84 == 1` gate is
                         materialised one slot late and in $a0 where the
                         target has $v0.  2 rows.
     242, 244            consequently *(s16 *)(p + 0x84) is in $v0 here and
                         $a0 there.  2 rows.
     246                 the `!= 0` reload is `lh $a0, 0x22($a2)` here and
                         `lh $a0, 0x2($a1)` in the target: the SAME ADDRESS
                         off a different base, because of the spelling in the
                         next paragraph.  1 row.

   EVERY SPELLING MEASURED ON THIS BODY.  All figures are aligned
   differences at 399 words unless the word count is given.

   LOAD-BEARING, and each of these was found by measurement:

   * THE `+0x2` TILE DISTANCE MUST BE RE-READ OFF A BASE THE COMPILER CANNOT
     EQUATE WITH `w`.  Written the natural way -- store `*(s16 *)(w + 0x2)`,
     then test and divide by `*(s16 *)(w + 0x2)` -- cse forwards the stored
     value across the `+0x84 == 1` branch and reconstructs it as
     `sll 17`/`sra 16` off the abs pseudo, which keeps that pseudo live and
     displaces `w` and `n` by a register each: 32 rows.  Spelled
     `*(s16 *)((u8 *)n + 0x22)` the reload survives and it is 12.  The array
     form `((s16 *)w)[1]`, on either the store or the read or both, forwards
     exactly like the plain cast (32) -- MEM_IN_STRUCT_P does not separate
     two varying MEMs at one address.  I DO NOT BELIEVE `n + 0x22` IS THE
     ORIGINAL SPELLING; it buys 20 rows and costs 1, and the row it costs is
     the base register.  The spelling that makes the target reload off `w` is
     the one thing left to find here.
   * THE TWO FOLLOW BLOCKS MUST REUSE `n` AND `h`.  `*(UnkNode **)(p + 0x90)`
     and `*(u8 **)(p + 0x94)` assigned into the SAME locals the node block
     used ($a2 and $a0) is 12; fresh locals for them is 20, the eight extra
     rows being those two pointers in $a0/$v1 instead.  Same allocno, same
     register.
   * THE +0x7E FACING MUST GO THROUGH AN `s32` LOCAL WRITTEN BEFORE THE BANK
     TICK STORE.  `j = (*(u16 *)(r + 0x2A) >> 9) & 7; D_s01_80025338 = 1;
     *(s16 *)(p + 0x7E) = j;` is 12; computing it inline after the tick store
     is 17, the three rows being the `lhu 0x2A($s1)` two slots late; moving
     the whole statement before the tick store is 42 at 401 words.
   * THE +0x84 DECREMENT IN STATE 3 MUST BE AN ARRAY REF.
     `((u16 *)p)[0x42] -= 1;` is 12; `*(u16 *)(p + 0x84) -= 1;` is 15.  The
     three rows are the `sh $a0, 0x84($s0)` the target puts in the `bnez`
     delay slot: as a plain cast the store is a varying MEM that is not
     in_struct, memrefs_conflict_p refuses to separate it from the
     fixed-address `D_s01_80025338` load, and reorg cannot sink it past that
     load.  As an ARRAY_REF it is in_struct, the fixed-scalar-versus-varying-
     struct rule applies, and the sink is allowed.  Making the matching `+= 1`
     an array ref too is FREE (12 either way); it is written that way for
     consistency.
   * THE TWO-ARM TILE DELTA MUST BE AN `if`/`else`, NOT A PRE-ASSIGNMENT.
     The `if`/`else` above is 12.  `t = <else expr>; if (cond) { t = <then
     expr>; }` -- which is what the target's fall-through LOOKS like, with the
     else arm's `subu` in the branch delay slot -- is 17, and the extra rows
     are the same combine_regs tie one register further out.  `if (t == 0)`
     on the pre-assigned difference does NOT fold back into the two-operand
     compare: 37, with a `bnez`/`nop` pair where the target has
     `bne $v0,$v1` plus the `subu` in its delay slot.
   * THE THIRD-ARM CLIMB IS A SIGNED `- 0x200000`, NOT `+ 0xFFE00000`.  Both
     give one `lui 0xFFE0` and an `addu`, but `0xFFE00000` is an unsigned
     constant in C89, which makes the whole numerator unsigned and emits
     `divu` where the target has `div`.  1 row, and it is a correctness bug
     rather than a spelling.
   * A `do { } while (0)` AROUND THE `+0x2` STORE IS WORTH 5 ROWS: 17 without
     it, 12 with.  `if (1)` around the same statement is 17, i.e. exactly as
     good as nothing, which is flow.c:2122 again -- `if (1)` is not an
     enclosing loop and buys no reference.  I CALL THIS MACRO RESIDUE rather
     than a device: the statement is the whole "publish the tile distance"
     step and a macro body is the natural reading, but I could not find a
     plain-C spelling with the same effect.  Doubling the wrapper, splitting
     the abs out inside it, and wrapping the `w + 0x8` store instead all
     measure 12 as well, so the target does not constrain the scope.

   MEASURED AND FREE, so NONE of these is why the body is as close as it is:

   * The declaration order of the sixteen locals.  Four permutations --
     `t` third, `hh` before `h`, `r` before `d`, `t`/`k` hoisted above
     `n`/`w` -- all measure 12.
   * `u[0x24]` against `*(u8 *)(u + 0x24)`, and `*(s8 *)(r + 0x72)` against
     `((s8 *)r)[0x72]`: 12 either way.
   * `__builtin_abs(t) * 2` against `__builtin_abs(t) << 1`, against
     `t = __builtin_abs(t); ... t * 2`, and against the two-statement
     `if (t < 0) { t = -t; }`: 12 for all four.
   * Two `s32` locals for the compared bytes (`aa = *(s8 *)(r + 0x72);
     bb = u[0x24]; if (aa == bb) ... else t = aa - bb;`): 12.
   * `*(u8 **)(r + 0x60)` against `*(s32 *)(r + 0x60)` in the `hh` chain: 12.
   * Separate locals for the three `u8 *` roles I had merged (the -0x18
     pointer, n->unk_08, p+0x94) are REQUIRED, not free: one shared `h` is
     20 and splitting the -0x18 pointer out into `hh` is 12.

   REJECTED, WITH ITS PRICE:

     the `+0x88 == 0x63` follow block shared between states 2 and 3
                                                   not measurable -- it must
                                                   be written out twice, the
                                                   target has both copies
     `*(s16 *)(w + 0x2)` read plainly              32
     `((s16 *)w)[1]` read                          32
     fresh locals for p+0x90 / p+0x94              20
     one `h` for all three pointer roles           20
     no do-wrapper on the +0x2 store               17
     `if (1)` instead of the do-wrapper            17
     `t = A; if (cond) t = B;` for the tile delta  17
     `*(u16 *)(p + 0x84) -= 1;` in state 3         15
     +0x7E facing computed inline                  17
     +0x7E statement moved above the tick store    42  (401 words)
     `hh` computed before the +0x2 store           41
     `hh` computed before the abs block            34
     state-3 inner gate as `if (tick != 0)` first  32  (the big arm must be
                                                   the `then`; this is the
                                                   brief's which-arm check)
     `if (+0x84 != 1) {...} else if (...)`         23
     `u[0x24] == *(s8 *)(r + 0x72)`                19  (400 words)
     `s16 t`                                       32  (406 words)
     `*(u8 **)(w + 0x2C) = r` before the +0x58
       store rather than after the +0x5C store     17  (and at 397 words it
                                                   also hid the two missing
                                                   words)
     the five `n->unk_0C` dress stores written
       out instead of through one `k` local        81  (404 words -- five
                                                   extra `lw 0xC`)

   RELOCATIONS HAND-CHECKED against the listing and clean under
   tools/decomp/relcand.py: 7 data symbols (D_s01_80024004, D_800DEC00,
   D_s01_80025338, D_8006CCD8, D_8006CCE8, D_8008346C, D_800814A0), the
   %hi/%lo pair on func_s01_800246DC, and 4 calls (func_8003DB94,
   func_8003DF74, func_800A56E0, func_8003FC64).  D_s01_80024004 is NOT a
   rodata array to define in C -- splat already emits it, as
   D_s01_80024004 in asm/data/1834800.data.s, from the segment's `data`
   subsegment.  Declare it extern and do not add a definition.

   WHAT I WOULD TRY NEXT.  Not statement order, not declaration order and not
   another device -- all three are exhausted above, and func_s21_80024050
   records the same dead ends on the same mechanism.  The one open question
   is the spelling that makes the `+0x2` tile distance RELOAD off `w` rather
   than be forwarded from its own store.  Find that and the `n + 0x22` row
   goes, and with it very likely the $v0/$v1 tie, because the forwarded
   pseudo is what lengthens the live range that creates the conflict.

   DECLARATIONS
   typedef struct UnkDngS01Dirs
   {
       u8 unk_00[0x20];
   } UnkDngS01Dirs;
   extern u8 D_s01_80024004[];
   extern u8 D_800DEC00[];
   extern s16 D_s01_80025338;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   void func_s01_800246DC();
   void func_8003DB94(void *, void *, s32);
   u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
   UnkNode *func_8003FC64(s32);
   s32 func_800A56E0(s32);
   END DECLARATIONS */
void func_s01_80024CFC(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 sp10[3];
    UnkDngS01Dirs dd;
    u8 *d;
    u8 *r;
    UnkNode *g;
    UnkVec *m;
    UnkNode *n;
    u8 *w;
    u8 *u;
    u8 *h;
    u8 *hh;
    s32 t;
    s32 j;
    UnkSlot *k;

    r = *(u8 **)(p + 0x0);
    d = dd.unk_00;
    dd = *(UnkDngS01Dirs *)D_s01_80024004;
    g = (UnkNode *)(r - 0x20);
    m = g->unk_08;
    *(u16 *)(p + 0x82) += 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)s->unk_0C = 0x808080;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        func_8003DB94(s, D_800DEC00, 0);
        j = (*(u16 *)(r + 0x2A) >> 9) & 7;
        D_s01_80025338 = 1;
        *(s16 *)(p + 0x7E) = j;
        *(u16 *)(p + 0xA) += 1;
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)g->unk_0C)->unk_08, (UnkSlot *)g->unk_0C, sp10, 0) != 0 ||
            (((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) != 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)m + 0xA);
            if ((((UnkSlot *)g->unk_0C)->unk_14 & 0x8000) == 0)
            {
                *(s16 *)((u8 *)v + 0x2) += sp10[0];
                *(s16 *)((u8 *)v + 0x6) += sp10[1];
                *(s16 *)((u8 *)v + 0xA) += sp10[2];
            }
            else
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) - 0x40;
            }
            *(s32 *)(p + 0xB0) = v->unk_00;
            *(s32 *)(p + 0xB4) = v->unk_04;
            *(s32 *)(p + 0xB8) = v->unk_08;
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                *(s16 *)(p + 0x84) = 0;
                *(u16 *)(p + 0xA) += 1;
                func_800A56E0(0x300);
            }
        }
        break;
    case 2:
        *(u16 *)(p + 0x84) += 1;
        if ((*(u16 *)(p + 0x84) & 1) != 0)
        {
            n = func_8003FC64(0x12);
            if (n != NULL)
            {
                w = (u8 *)n + 0x20;
                *(s32 *)(w + 0x58) = *(s16 *)(d + (*(s16 *)(p + 0x7E) << 2)) << 16;
                *(s32 *)(w + 0x5C) = *(u16 *)(d + (*(s16 *)(p + 0x7E) << 2) + 0x2) << 16;
                *(u8 **)(w + 0x2C) = r;
                *(s32 *)(w + 0x30) = *(s32 *)(r + 0x60);
                *(s16 *)(w + 0xA) = (*(s16 *)(p + 0x84) - 1) / 2;
                w[0x15] = p[0x9];
                *(s16 *)(w + 0x16) = *(u16 *)(p + 0x7E);
                u = *(u8 **)(r - 0x14);
                p[0xA8] = u[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
                p[0xA9] = u[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
                if (*(s32 *)(r + 0x60) != 0)
                {
                    *(s16 *)(w + 0x8) = 1;
                    if (*(s8 *)(r + 0x72) == u[0x24])
                    {
                        t = *(s8 *)(r + 0x73) - u[0x25];
                    }
                    else
                    {
                        t = *(s8 *)(r + 0x72) - u[0x24];
                    }
                    do
                    {
                        *(s16 *)(w + 0x2) = __builtin_abs(t) * 2;
                    } while (0);
                    hh = *(u8 **)(*(s32 *)(r + 0x60) - 0x18);
                    if (*(s16 *)(p + 0x84) == 1)
                    {
                        if (*(s16 *)((u8 *)n + 0x22) != 0)
                        {
                            v->unk_14 = *(s32 *)(w + 0x60) =
                                (*(s32 *)(hh + 0x8) - v->unk_08 - 0x200000) / (*(s16 *)((u8 *)n + 0x22) - 1);
                        }
                    }
                    else
                    {
                        *(s32 *)(w + 0x60) = v->unk_14;
                    }
                }
                else
                {
                    *(s16 *)(w + 0x2) = 8;
                    *(s16 *)(w + 0x8) = 0;
                }
                if (*(s16 *)(p + 0x84) == 9)
                {
                    if (*(s32 *)(r + 0x60) != 0)
                    {
                        *(s16 *)(p + 0x86) = *(u16 *)(w + 0x2) + 0x28;
                    }
                    else
                    {
                        *(s16 *)(p + 0x86) = 0x1E;
                    }
                }
                n->unk_10 = (u32)func_s01_800246DC;
                h = (u8 *)n->unk_08;
                *(s32 *)(h + 0x0) = *(s32 *)(p + 0xB0);
                *(s32 *)(h + 0x4) = *(s32 *)(p + 0xB4);
                *(s32 *)(h + 0x8) = *(s32 *)(p + 0xB8);
                k = (UnkSlot *)n->unk_0C;
                k->unk_0C[2] = 0x80;
                k->unk_0C[1] = 0x80;
                k->unk_0C[0] = 0x80;
                k->unk_1E = 0x1000;
                k->unk_1C = 0x1000;
                if (*(s16 *)(p + 0x84) == 1)
                {
                    *(u32 *)(p + 0x90) = (u32)n;
                    *(u32 *)(p + 0x94) = (u32)h;
                    *(s16 *)(p + 0x88) = 0x63;
                }
            }
        }
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0x90);
            h = *(u8 **)(p + 0x94);
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = *(s32 *)(h + 0x0);
                v->unk_04 = *(s32 *)(h + 0x4);
                v->unk_08 = *(s32 *)(h + 0x8);
            }
        }
        if (*(s16 *)(p + 0x84) >= 0xB)
        {
            *(s16 *)(p + 0x84) = 0;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x88) == 0x63)
        {
            n = *(UnkNode **)(p + 0x90);
            h = *(u8 **)(p + 0x94);
            if ((n->unk_1E & 0x8000) != 0)
            {
                *(s16 *)(p + 0x88) = 0;
            }
            else
            {
                v->unk_00 = *(s32 *)(h + 0x0);
                v->unk_04 = *(s32 *)(h + 0x4);
                v->unk_08 = *(s32 *)(h + 0x8);
            }
        }
        ((u16 *)p)[0x42] += 1;
        if (*(s16 *)(p + 0x84) > *(s16 *)(p + 0x86))
        {
            ((u16 *)p)[0x42] -= 1;
            if (D_s01_80025338 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_s01_80025338 = 0;
            }
        }
        break;
    }
}
