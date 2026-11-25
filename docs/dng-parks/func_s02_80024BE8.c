/* PARK: func_s02_80024BE8 (ovl_dng_s02) -- 834 words target, 834 words
   candidate, 5 ALIGNED DIFFERENCES (and 5 under ovlverify too, so no stale
   build: the two tools agree at equal word counts).  1 member, 834 words
   (dngfam.py), so nothing replicates off it.

   This was NOT on agent h140's dispatched list; it was handed over as extra
   work after the five listed bodies landed, on the strength of this unit
   being four-fifths decompiled.  That bet paid: the body went from "no twin
   at any ratio" to word-exact and five rows in one sitting.

   OWNS A JUMP TABLE: jtbl_s02_80024028, 9 .word entries, NO PAD.
   jtcheck agrees with the dispatch bound (`sltiu $v0, $v1, 0x9'): 9 cases,
   9 distinct targets.  The entries, in order:
       0 -> .Ls02_80024CE8    5 -> .Ls02_800256E4
       1 -> .Ls02_80024D54    6 -> .Ls02_800257E8
       2 -> .Ls02_80024F94    7 -> .Ls02_80025880   <-- gcc's DEFAULT label
       3 -> .Ls02_8002540C    8 -> .Ls02_8002581C
       4 -> .Ls02_80025490
   ENTRY 7 IS THE DEFAULT, not an arm: .Ls02_80025880 is also the target of
   the range-check `beqz $v0, .Ls02_80025880' at 80024CC4 and of every arm's
   closing `j', which is the brief's one-grep discriminator.  So the switch is
   written with cases 0,1,2,3,4,5,6,8 and NO `case 7' -- gcc pads the table to
   maxval 8 with the default label.  Writing `case 7: break;' would give a
   phantom ninth arm.  Note also that CASE 0 FALLS THROUGH into case 1 (no
   branch between 80024D50 and .Ls02_80024D54).

   WHAT IT IS.  The summon-beam driver, state 0..8 on *(s16 *)(p + 0xA).
   `p' is this node's payload, `v' its mover, `s' its renderable;
   `r = *(u8 **)p' is the CASTER payload, `g = (UnkNode *)(r - 0x20)' the
   caster node and `m = g->unk_08' the caster mover.  The prologue also
   copies the 32-byte direction table D_s02_80024004 onto the stack and bumps
   the +0x82 frame counter; the shared tail after the switch refreshes the
   renderable through func_800478B8 and re-registers it against D_800DE870
   every sixth frame.
     0  dress white, 0x400 scales, bind D_800DE870, latch the caster facing
     1  publish the caster hand through func_8003DF74, then install the beam
        step and compute the per-frame delta (+0xC/+0x10 from the direction
        table, +0x14 as the climb divided by the +0x7B tile count)
     2  probe with func_800A4778, spawn four func_s02_80024758 motes, then
        either walk the tile counter down (six func_s02_800240C0 sparks per
        frame) or land the beam: func_80044A50, raise 0x10000000 on the
        target's +0x1C, white its +0x0C..0x0E, func_800419EC/func_800A56E0,
        and spawn the 0x212 impact node running func_s02_80024B14
     3  fade the target's colour triple down over 0x24 frames
     4  two func_s02_800249A0 bursts plus one 0x212 node running
        func_s02_8002443C, jittered by the +0x86 counter & 3
     5  two more bursts, then clear 0x10000000 and fire func_8009CE1C
     6  wait 0x3D frames, then jump to state 8
     8  wait 0x1F frames, then release the node (D_8008346C = 0 and the
        0x8000 redraw pair) unless D_80025914 is still raised

   THE DONOR, and it is the whole reason this got close: the landed
   func_s01_80024CFC (csrc/ovl_dng_s01.c, 399 words).  dngtwin cannot see it
   (ratio below the floor at this size difference) and dngcallee NAMED IT'S
   SIBLING instead -- its single 9-of-21 row is func_s01_800246DC, which is
   the function s01's own case 2 spawns.  Reading 800246DC's unit then put
   func_s01_80024CFC in front of me, and it is the same driver with FOUR
   states where this has NINE: same prologue (`r = *(u8 **)p',
   `d = dd.unk_00', `dd = *(UnkDngS01Dirs *)D_s01_80024004',
   `g = (UnkNode *)(r - 0x20)', `m = g->unk_08', `*(u16 *)(p + 0x82) += 1'),
   same case 0, same case 1 func_8003DF74 gate with the `|| flags & 0x8000'
   short circuit and the cross-joined `v+0xA' store, same `__builtin_abs' of
   a ternary for the tile count, same D_8006CCD8/D_8006CCE8 byte reads, same
   `((u16 *)p)[n] += 1' / `-= 1' undo in the terminal state.  s01's case 2
   and 3 correspond to nothing here; this body's states 2..8 are its own.
   ovl_dng_s01 is this unit's sibling bank throughout -- s02/s01 are 1.000
   clone pairs at 8002443C/80024280 and 80024758/800245BC -- which is why the
   donor transfers as well as it does.

   THE RESIDUE, all five rows, ONE CAUSE, no words missing:

       R  172  172 80024E98  lw a0,-20(s5)       lw $a2, -0x14($s5)
       R  174  174 80024EA0  lbu v0,36(a0)       lbu $v0, 0x24($a2)
       R  183  183 80024EC4  lbu v0,37(a0)      lbu $v0, 0x25($a2)
       R  189  189 80024EDC  lbu v1,36(a0)      lbu $v1, 0x24($a2)
       R  194  194 80024EF0  lbu v1,37(a0)      lbu $v1, 0x25($a2)

   The pointer local `u = *(u8 **)(r - 0x14)' in case 1 gets $a0 here and
   $a2 in the target.  Nothing is moved, no word is gained or lost, and every
   other instruction in that block matches.  Since MIPS gcc 2.7.2 defines no
   REG_ALLOC_ORDER (checked in config/mips/mips.h), `find_reg' scans hard
   registers ASCENDING, so $a0 is the natural pick and the target getting $a2
   means $a0 AND $a1 both conflict with `u' live range there.  Nothing inside
   that basic block writes either, so the occupant must be a pseudo defined
   before .Ls02_80024E6C and used after .Ls02_80024F08 -- and the only
   candidates visible in the listing are the two arguments of the
   func_8004491C call in the `(p[0x7A] & 4) == 0' block immediately above,
   which is exactly the $a0/$a1 pair, and which gcc should consider dead at
   the call.

   THE UNTRIED NEXT STEP, stated plainly because the brief says that is the
   most valuable thing to leave: FIND THE SOURCE CONSTRUCT THAT KEEPS THE
   func_8004491C ARGUMENT PAIR LIVE PAST ITS CALL, or that puts two other
   short-lived pointers in $a0/$a1 across case 1's `*(s32 *)(r + 0x60) != 0'
   arm.  Everything I tried moved `u' itself and that is the wrong end of the
   problem -- the mechanism that solved the body's two OTHER caller-saved
   faults (see the table) was always a pseudo elsewhere occupying the low
   register, never a change at the site that reads wrong.  `-da' `.greg's
   conflict list for `u' would name the occupant in one compile; I did not
   run it because the brief's own warning about -da corroborating false
   stories applied and I had no source change in hand to confirm.

   TWO FAULTS THAT WERE SOLVED BY EXACTLY THAT MECHANISM, recorded because
   they are the pattern to re-use:
   * func_800419EC TAKES TWO ARGUMENTS, NOT THREE.  Written
     `func_800419EC(4, 8, e)' -- which is what the listing looks like, since
     `e' sits in $a2 at the call -- the body is 834 words / 19 aligned
     differences; `func_800419EC(4, 8)' with `e' merely a local used for the
     three preceding byte stores is 834 / 14.  `e' is in $a2 in the TARGET
     because $a0 and $a1 are taken by the two argument hard-register sets
     that sched1 hoists to the top of the block, and with only two arguments
     the SECOND one's hoist is blocked by the 0x1C pointer that takes $a1,
     so reorg lifts `$a0 = 4' into the earlier branch's delay slot -- which
     is what the target does.  THE CORPUS PROTOTYPE IS RIGHT:
     `void func_800419EC(u16, u32);' already exists in two other units and I
     had widened it on the strength of a register that was incidental.
   * A CALL ARGUMENT IN $a2 IS NOT EVIDENCE OF A THIRD ARGUMENT.  That is the
     general form, and it is a new way for a listing to mislead.

   THE FULL PRICED TABLE.  Every figure is from the body at 834 words except
   where marked, and the four marked rows were re-priced from the 5-row body
   afterwards because the brief is right that a mid-fight figure prices the
   remaining faults too:

   | spelling                                                        | rows |
   |-----------------------------------------------------------------|------|
   | KEPT: case 4 index reuses the loop local `i'                    |   0  |
   | its own local `w' instead                                       | 198  |
   | KEPT: case 0 facing has its OWN local `j'                       |   0  |
   | `j' shared with case 4's index as one local                     | 823  |
   | KEPT: `n' is ONE local across cases 2 and 4                     |   0  |
   | split into `n' and `nn', one per case                           | 209  |
   | KEPT: func_800419EC(4, 8) -- two arguments                      |   0  |
   | func_800419EC(4, 8, e) -- three                                 |  19  |
   | KEPT: case 4 slot block order 1C, 1E, 0C[2], 0C[1], 0C[0]       |   0  |
   | 1C, 0C[2], 0C[1], 0C[0], 1E                                     |  14  |
   | 1E, 0C[2], 0C[1], 0C[0], 1C                                     |  28  |
   | 1C, 0C[0], 0C[1], 0C[2], 1E                                     |  16  |
   | KEPT: `d = dd.b' inside case 1, just before its two uses        |   0  |
   | in the prologue, as func_s01_80024CFC has it      (re-priced)   |  11  |
   | at the top of case 1's 0x80 gate                  (re-priced)   |  14  |
   | immediately before case 1's `r + 0x60' gate       (re-priced)   |  14  |
   | KEPT: `*(s32 *)(h + 0x8) = v->unk_08' BEFORE the k reload       |   0  |
   | after it                                                        |  41  |
   | KEPT: ternary condition `*(s8 *)(r + 0x72) == u[0x24]'          |   0  |
   | operands swapped, `u[0x24] == *(s8 *)(r + 0x72)'                |  17  |
   | KEPT: p[0xA2] written before p[0xA3]                            |   0  |
   | p[0xA3] first                                                   |  16  |
   | KEPT: the abs expression inline in the p[0x7B] store            |   0  |
   | hoisted into `j' first, `p[0x7B] = j * 2 - 1'                   |  17  |
   | KEPT: one local `u' for all four `r - 0x14' reads               |   0  |
   | `u' plus an alias `q = u' for the byte reads                    |  11  |
   | the expression inline at each of the four sites                 |  27  |
   | KEPT: tail gate `(s16)(*(u16 *)(p + 0x84))++ >= 5'              |   0  |
   | `*(u16 *)(p + 0x84) += 1;' then `>= 6' as a separate gate       |   6  |
   | KEPT: case 1 else arm writes +0x78 BEFORE p[0x7B] = 8           |   0  |
   | p[0x7B] = 8 first                                               |   4  |
   | KEPT: case 4's dz is `-(*(s16 *)(p + 0x86) * 2) + 0x10'         |   0  |
   | `0x10 - *(s16 *)(p + 0x86) * 2'                                 |   1  |
   | KEPT: case 4's two jitters wear `(s16)' on the SUBTRACTION      |   0  |
   | `*(u16 *)(...) - 0x8 + (rand() & 0xF)' plain                    |   4  |
   | NOT MEASURABLE: declaration order of the 14 locals              |   -  |

   Two of those rows are the brief's `reg_n_sets' lever and they are the
   whole reason this body got anywhere, so they are worth restating: a local
   SHARED between two arms hundreds of words apart loses its
   `adjust_priority' birthing boost and rotates the ENTIRE callee-saved set.
   `j' doing double duty as case 0's facing and case 4's index was 823
   differing out of 839 words; splitting it was 836/84.  And the inverse is
   just as sharp -- case 4's index MUST reuse the loop counter `i' (0) rather
   than take a local of its own (198), because the target shares $s2 between
   `i', the 0x80 constant and that index, and a separate local pushes `n' out
   of $s3.  Count the sets, then look at which registers the target SHARES.

   The `(s16)' jitter fact is func_s01_800246DC's, carried verbatim and
   re-priced here at 4 rows against its 33: convert.c narrows the sum to
   `unsigned short' because the first operand is a `u16' load, `- 0x8'
   becomes 0xFFF8, and cc1psx materialises it with `li $s0, 0xfff8' plus
   `addu' where the target has `addiu $v1, $v1, -0x8'.

   SYMBOLS: relcand.py reports `ok (10 data symbols, 51 calls)'.  Every
   `%hi'/`%lo' pair and every `jal' in the listing was checked by hand
   against the source as well -- func_80045C34 is passed BY ADDRESS to
   func_8004491C in case 2 (not called), and func_80069EF8 is `rand' out of
   splat/common_syms.txt.

   DECLARATIONS
   extern s16 D_80025914;
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u8 D_80045340[];
   extern u8 D_800DE870[];
   extern u8 D_800DE9D0[];
   extern u8 D_80025908[];
   extern u8 D_800258FC[];
   extern u8 D_s02_80024004[];
   extern s32 D_8008346C;
   (UnkDngS58Blk32 comes from the unit's own block now -- it was declared
    here when this park was written and is a REDEFINITION today.)
   UnkNode *func_8003FC64(s32);
   void func_8003DB94(void *, void *, s32);
   u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
   s32 func_800A4778(u16, u16, s16, s32);
   s32 func_800A56E0(s32);
   void func_800419EC(u16, u32);
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
   void func_800478B8(u8 *);
   void func_s02_800240C0(u8 *, UnkVec *, u8 *, s16, s16, s16);
   void func_s02_80024758(UnkNode *, s16, u32, s16, s16, s16, s16);
   void func_s02_800249A0(UnkNode *, s16, s16, s16, s16);
   void func_s02_8002443C(u8 *, UnkVec *, u8 *);
   void func_s02_80024B14(u8 *, UnkVec *, u8 *);
   END DECLARATIONS

   func_8004491C, func_80044A50, D_800814A0 and UnkSlot come from
   include/structs.h; `rand' comes from common.h via the psyq headers. */
void func_s02_80024BE8(u8 *p, UnkVec *v, UnkSlot *s)
{
    s16 sp10[3];
    UnkDngS58Blk32 dd;
    u8 *d;
    u8 *r;
    UnkNode *g;
    UnkVec *m;
    UnkNode *n;
    u8 *q;
    u8 *u;
    u8 *h;
    u8 *e;
    UnkSlot *k;
    s32 i;
    s32 j;

    r = *(u8 **)(p + 0x0);
    dd = *(UnkDngS58Blk32 *)D_s02_80024004;
    g = (UnkNode *)(r - 0x20);
    m = g->unk_08;
    *(u16 *)(p + 0x82) += 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)s->unk_0C = 0x808080;
        s->unk_1E = 0x400;
        s->unk_1C = 0x400;
        s->unk_12 = 0x7DCF;
        s->unk_14 |= 0x100;
        *(s16 *)(p + 0x84) = 0;
        func_8003DB94(s, D_800DE870, 2);
        j = (*(u16 *)(r + 0x2A) >> 9) & 7;
        D_80025914 = 1;
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
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                if ((p[0x7A] & 0x4) == 0)
                {
                    func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
                    s->unk_10 = 0x60;
                    s->unk_14 |= 0xC;
                    p[0x7A] |= 0x4;
                }
                if (*(s32 *)(r + 0x60) != 0)
                {
                    *(s16 *)(p + 0x78) = *(u16 *)(*(s32 *)(r + 0x60) + 0x88);
                    u = *(u8 **)(r - 0x14);
                    p[0xA2] = u[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
                    p[0xA3] = u[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
                    p[0x7B] = __builtin_abs(*(s8 *)(r + 0x72) == u[0x24]
                                                ? *(s8 *)(r + 0x73) - u[0x25]
                                                : *(s8 *)(r + 0x72) - u[0x24]) *
                                  2 -
                              1;
                }
                else
                {
                    *(s16 *)(p + 0x78) = *(u16 *)(r + 0x88) - 0x50;
                    p[0x7B] = 8;
                }
                d = dd.b;
                *(s32 *)((u8 *)v + 0xC) = *(s16 *)(d + (*(s16 *)(p + 0x7E) << 2)) << 16;
                *(s32 *)((u8 *)v + 0x10) = *(u16 *)(d + (*(s16 *)(p + 0x7E) << 2) + 0x2) << 16;
                *(s32 *)((u8 *)v + 0x14) =
                    ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
                *(s16 *)(p + 0x82) = 0;
                *(u16 *)(p + 0xA) += 1;
            }
        }
        break;
    case 2:
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(r + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            s->unk_14 |= 0x80;
            break;
        }
        for (i = 0; i < 4; i++)
        {
            func_s02_80024758((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xF04040,
                              (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        if ((s8)(p[0x7B] -= 1) <= 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            if (*(s32 *)(r + 0x60) != 0)
            {
                *(s16 *)(p + 0x86) = 0;
                *(u16 *)(p + 0xA) += 1;
                q = *(u8 **)(*(s32 *)(r + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(q + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(q + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                *(s32 *)(*(s32 *)(r + 0x60) + 0x1C) |= 0x10000000;
                e = *(u8 **)(*(s32 *)(r + 0x60) - 0x14);
                e[0xC] = 0x80;
                e[0xD] = 0x80;
                e[0xE] = 0x80;
                func_800419EC(4, 8);
                func_800A56E0(0x300);
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s02_80024B14;
                    func_8004491C(n, (u32)func_80045C34);
                    k = (UnkSlot *)n->unk_0C;
                    k->unk_10 = 0;
                    k->unk_14 |= 0xC;
                    h = (u8 *)n->unk_08;
                    *(s16 *)(h + 0x2) = *(u16 *)((u8 *)v + 0x2);
                    *(s16 *)(h + 0x6) = *(u16 *)((u8 *)v + 0x6);
                    *(s32 *)(h + 0x8) = v->unk_08;
                    k = (UnkSlot *)n->unk_0C;
                    k->unk_08 = (u32)D_800258FC;
                    k->unk_0C[2] = 0x80;
                    k->unk_0C[1] = 0x80;
                    k->unk_0C[0] = 0x80;
                    k->unk_1E = 1;
                    k->unk_1C = 1;
                    k->unk_14 ^= 0xC;
                    *(s16 *)(h + 0xA) = *(u16 *)(*(s32 *)(r + 0x60) + 0x88);
                }
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(*(s32 *)(r + 0x60) + 0x88);
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
                s->unk_0C[2] = 0;
                s->unk_0C[1] = 0;
                s->unk_0C[0] = 0;
            }
        }
        else
        {
            v->unk_00 += *(s32 *)((u8 *)v + 0xC);
            v->unk_04 += *(s32 *)((u8 *)v + 0x10);
            v->unk_08 += *(s32 *)((u8 *)v + 0x14);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
            func_s02_800240C0(p, v, (u8 *)s, (rand() & 0xF) - 8, (rand() & 0xF) - 8,
                              (rand() & 0xF) - 8);
        }
        break;
    case 3:
        *(u16 *)(p + 0x86) += 2;
        e = *(u8 **)(*(s32 *)(r + 0x60) - 0x14);
        e[0xE] = *(s16 *)(p + 0x86) * 3 - 0x70;
        e[0xD] = 0x70 - *(s16 *)(p + 0x86) * 3;
        e[0xC] = 0x70 - *(s16 *)(p + 0x86) * 3;
        if (*(s16 *)(p + 0x86) >= 0x24)
        {
            *(s16 *)(p + 0x86) = 0;
            *(s16 *)(p + 0x82) = 0;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 4:
        for (i = 0; i < 2; i++)
        {
            func_s02_800249A0((UnkNode *)(p - 0x20), (rand() & 0x3F) - 0x20,
                              (rand() & 0x3F) - 0x20, -(*(s16 *)(p + 0x86) * 2) + 0x10, 0x1E);
        }
        i = *(u16 *)(p + 0x86) & 0x3;
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            *(s16 *)((u8 *)n + 0x22) = 0x1E;
            n->unk_10 = (u32)func_s02_8002443C;
            func_8004491C(n, (u32)D_80045340);
            k = (UnkSlot *)n->unk_0C;
            k->unk_10 = 0;
            k->unk_14 |= 0xC;
            h = (u8 *)n->unk_08;
            *(s16 *)(h + 0x2) = (s16)(*(u16 *)((u8 *)v + 0x2) - 0x8) + (rand() & 0xF);
            *(s16 *)(h + 0x6) = (s16)(*(u16 *)((u8 *)v + 0x6) - 0x8) + (rand() & 0xF);
            if ((rand() & 0x1) != 0)
            {
                k->unk_14 |= 0x1;
            }
            switch (i)
            {
            case 0:
                *(s16 *)(h + 0x6) = *(u16 *)((u8 *)v + 0x6) + 0x8;
                k->unk_06 = 0x7;
                break;
            case 1:
                *(s16 *)(h + 0x2) = *(u16 *)((u8 *)v + 0x2) - 0x8;
                k->unk_06 = 0x7;
                break;
            case 2:
                *(s16 *)(h + 0x2) = *(u16 *)((u8 *)v + 0x2) + 0x8;
                k->unk_06 = 0x7;
                break;
            case 3:
                *(s16 *)(h + 0x6) = *(u16 *)((u8 *)v + 0x6) - 0x8;
                k->unk_06 = -0x7;
                break;
            }
            *(s32 *)(h + 0x8) = v->unk_08;
            k = (UnkSlot *)n->unk_0C;
            k->unk_1C = *(s16 *)(p + 0x86) * 170;
            k->unk_1E = *(s16 *)(p + 0x86) * 204;
            k->unk_0C[2] = 0x80;
            k->unk_0C[1] = 0x80;
            k->unk_0C[0] = 0x80;
            func_8003DB94(k, D_800DE9D0, 0);
        }
        *(u16 *)(p + 0x86) += 2;
        if (*(s16 *)(p + 0x86) >= 0x3D)
        {
            *(s16 *)(p + 0x86) = 0x28;
            *(u16 *)(p + 0xA) += 1;
        }
        break;
    case 5:
        for (i = 0; i < 2; i++)
        {
            func_s02_800249A0((UnkNode *)(p - 0x20), (rand() & 0x3F) - 0x20,
                              (rand() & 0x3F) - 0x20, -0x20 - (rand() & 0x3F), 0x1E);
        }
        *(u16 *)(p + 0x86) -= 2;
        if (*(s16 *)(p + 0x86) <= 0)
        {
            *(s16 *)(p + 0x86) = 0;
            *(s16 *)(p + 0x82) = 0;
            *(u16 *)(p + 0xA) += 1;
            *(s32 *)(*(s32 *)(r + 0x60) + 0x1C) ^= 0x10000000;
            e = *(u8 **)(*(s32 *)(r + 0x60) - 0x14);
            e[0xC] = 0x80;
            e[0xD] = 0x80;
            e[0xE] = 0x80;
            func_8009CE1C(*(u8 **)(r + 0x60), 0x10, p[0x9], 2,
                          (s16)(*(u16 *)(p + 0x7E) << 9), (u32)r, 2);
        }
        break;
    case 6:
        *(u16 *)(p + 0x82) += 1;
        if (*(s16 *)(p + 0x82) >= 0x3D)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        ((u16 *)p)[0x41] += 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((u16 *)p)[0x41] -= 1;
            if (D_80025914 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025914 = 0;
            }
        }
        break;
    }
    func_800478B8((u8 *)s);
    if ((s16)(*(u16 *)(p + 0x84))++ >= 5)
    {
        func_8003DB94(s, D_800DE870, 2);
        *(s16 *)(p + 0x84) = 0;
    }
}
