/* PARKED at 529 words / 529 words, 56 ALIGNED DIFFERENCES -- the right
   length, and every differing row is one consistent TWO-REGISTER TRANSPOSITION
   plus the scheduling that follows from it.  agent-h157, batch 36.
   1 member (dngfam.py): this function only.  No family member was parked.

   THE RESIDUE IS $s0 <-> $s1, TWICE, AND IT IS ONE DECISION.  Four allocnos
   compete for those two hard registers in two blocks that never overlap:
     case 1  m  (the spawned node's mover, n->unk_08)   target $s0, mine $s1
     case 1  dx (the first rand jitter)                 target $s1, mine $s0
     case 2  xx (the probed cell's x, u16-truncated)    target $s0, mine $s1
     case 2  yy (the probed cell's y)                   target $s1, mine $s0
   m and xx do not conflict (different cases) so they share one register, and
   dx and yy share the other.  Whichever of {m, xx} or {dx, yy} global-alloc
   reaches first takes $s0; the target reaches m/xx first and this body
   reaches dx/yy first.  Every one of the 54-56 rows is either that swap or
   an instruction the scheduler moved because of it -- the two clusters are
   case 1's velocity block (candidate rows 118-130) and case 2's xx/yy
   computation (rows 238-256).  THE WORD COUNT IS EXACT AND NOTHING IS
   STRUCTURALLY MISSING: there is no extra or absent instruction anywhere
   outside those two clusters.

   ONE CONSEQUENCE WORTH NAMING, because it is what makes the rows visible
   rather than silent: in the target `dx << 12' goes to a TEMP ($v0) and so
   the stored q+0x50 value is not live when `*(s32 *)(q + 0x50) +=' reloads
   it -- the target emits `lw $v1, 0x50($s2)'.  With dx in $s0 the shift
   reuses dx's own register, the value stays live, and cse forwards it as
   `move $v1, $s0' instead.  So the sw/lw pair in the target is EVIDENCE for
   the register assignment, not an independent fact.

   `-da' NUMBERS (probe.c.flow on this body, loop-weighted as flow.c does):
   the mover `m' is reg 86 at 28 refs / 40 insns / 3 calls, priority 2.80 --
   the HIGHEST call-crossing allocno in the function -- so the formula says
   it should take $s0 first and it does not.  That means `find_reg's
   preference machinery and not `allocno_compare' is deciding here, which is
   why none of the reference-count levers below moved it.  THIS IS GOOD
   PERMUTER INPUT: the shape is settled, the word count is exact, and the
   search space is which of two pseudos is allocated first.

   EVERYTHING MEASURED AND REJECTED, all at 529 words unless stated:

   | spelling                                                    | words | aligned |
   | as written below                                             |  529  |   56    |
   | dy computed BEFORE dx (semantically wrong -- the first       |  529  |   54    |
   |   rand must feed the x axis, so rejected despite the gain)   |       |         |
   | `do { } while (0)' round the m + 0xA out-add (bump on m)      |  529  |   56    |
   | `do { } while (0)' round the m + 0xA mover seed               |  529  |   56    |
   | `do { } while (0)' round `*(s32 *)(q + 0x50) = dx << 12'      |  529  |   60    |
   | the two q+0x50/q+0x54 stores BEFORE the two mover adds        |  529  |   56    |
   | `dx * 0x1000' for `dx << 12'                                  |  529  |   56    |
   | dy's rand interleaved between the two mover adds              |  529  |   59    |
   | yy computed before xx                                         |  529  |   56    |
   | the +0x2A wrap as `k = ... + 0x200; if (k >= 0x1000) k -= ..' |  529  |   62    |
   | the +0x2A wrap with the store duplicated in both arms         |  531  |   94    |
   | the +0x2A wrap as a `?:'                                      |  529  |   85    |
   | xx/yy passed inline to both calls, no locals                 |  540  |   69    |
   | `s16 k' for the +0x2A wrap                                    |  535  |   79    |
   | `u16 xx/yy' instead of `s32' with u16 parameters              |  531  |   59    |
   | the two +0x96 countdowns as `(s16)(x)-- <= 1'                 |  531  |   59    |

   FOUR FACTS THAT ARE SETTLED AND SHOULD NOT BE RE-SEARCHED, since each was
   measured on this body and each closed its own cluster:

   * THE +0x2A ANGLE WRAP MUST BE A THREE-STATEMENT LOCAL, `k = *(s16 *)(d +
     0x2A); k += 0x200; if (k >= 0x1000) { k -= 0x1000; }' and then one
     store.  Folded into two statements (`k = *(s16 *)(d + 0x2A) + 0x200')
     combine merges the two constants and emits `addiu $v1, $a0, -0xE00' off
     the ORIGINAL load where the target has `addiu $v1, $v1, -0x1000' off the
     incremented value: 62 rows.  Splitting the load from the add is what
     keeps the original out of a register.  Both case 3 and case 7 have this
     block and both need the split.
   * `xx' AND `yy' MUST BE `s32' PASSED TO `u16' PARAMETERS, with
     func_800BCB04 reached through the unit's `func_800BCB04u(u16, u16, s16)'
     alias.  As `u16' locals every operand load in the sum narrows to `lhu'
     where the target has `lh' (the cell fields and both b+0x2/b+0x6 reads),
     for 59 rows at 531 words.  This is the same lever func_b42_80174AA4
     needed and it reproduces here.
   * THE TWO +0x96 COUNTDOWNS ARE PRE-DECREMENTS, `(s16)(--*(u16 *)(p +
     0x96)) <= 0'.  As a post-decrement compared against 1 the target's
     `sll'/`blez' pair becomes `sll'/`sra'/`slti'/`bnez' and the store sinks:
     531 words, 59 rows.
   * `tbl' IS A POINTER LOCAL IN CASE 2 AND THE CASE-1 CELL READS ARE NOT.
     Case 2 reads the D_8017610C record at three sites either side of two
     calls, so the `%lo' is materialised once into $s2; case 1's two reads
     are in one block and re-materialise the `%hi'/`%lo' pair each time,
     which is what the listing shows (two `lui $t0' and two `addiu $t0').

   IT OWNS jtbl_b42_80170890: 17 words = 17 cases + 0 pad, dispatch bound 17,
   12 DISTINCT targets, and MINVAL 0 -- the dispatch reads p[0x9B] with
   `sltiu $v0, $v1, 0x11' and no subtraction, so entry k IS case k.  Written
   arms: 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 and 16, with case 9 FALLING THROUGH
   into case 16.  CASES 10 THROUGH 15 ARE NOT WRITTEN -- their six entries
   all hold .Lb42_80175FF0, which is the range check's own `beqz' target and
   the function's return, so they are gcc's pad to maxval.  Seventeen cases
   is odd and there is no pad word, which is consistent with this being the
   last table in the unit's .rodata (0x80170890 is the highest jtbl here).
   Eleven case nodes clears the five-node threshold.

   NO TWIN AT ANY RATIO.  dngtwin's rows for this body are 0.792
   func_b43_80170F68, 0.765 func_b15_801719DC, 0.764 func_b31_801716F4,
   0.758 func_b09_80171728 and 0.751 func_b28_80170E5C -- all five are the
   +0x8C handler family and NONE of them is this function, which is a +0x9B
   state machine.  xsibs finds nothing for this unit at any ratio.  Written
   off the listing on top of the ten bodies landed in ovl_dng_b42 in batch
   36, which is where every callee signature and the whole payload layout
   came from.

   DECLARATIONS
   extern u8 D_800DE870[];
   s16 func_800BCB04u(u16, u16, s16) __asm__("func_800BCB04");
   s16 func_8009A350(s16, s16, s32, u16 *);
   END DECLARATIONS

   The other symbols this body needs are already declared in
   csrc/ovl_dng_b42.c after batch 36: func_8003FC64, func_8004491C,
   func_8003DE58, func_8003DB94, func_800A45D8, func_800A2B04, func_80047784,
   func_800A56E0, func_800AD594, func_b42_80170B48, func_b42_80174A00,
   func_b42_80175494, func_b42_801714B8, D_80045340, D_8017610C (as
   `UnkDngB09A28 []'), D_801760A4, D_b42_8017609C, D_80083228, D_8008346C and
   rand. */
void func_b42_801757E0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    SVECTOR out;
    u16 sp18;
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *r0;
    u8 *e;
    u8 *tbl;
    s32 xx;
    s32 yy;
    s32 hh;
    s32 dx;
    s32 dy;
    s32 k;
    s32 i;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            p[0x9B] = 2;
            *(u16 *)(c + 0x14) |= 0x6000;
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s16 *)(p + 0x96) = 0xE;
        p[0xAC] = 0;
        p[0xAD] = 0;
        p[0x9B] += 1;
        func_800A56E0(0x80A);
        break;
    case 1:
        i = 0;
        do
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x1E) = 0x28;
                n->unk_10 = (u32)func_b42_80170B48;
                func_8004491C(n, (u32)D_80045340);
                r = (u8 *)n->unk_0C;
                *(u16 *)(r + 0x14) |= 0xC;
                *(s16 *)(r + 0x10) = 0x60;
                *(u16 *)(r + 0x14) |= 0x2;
                r0 = *(u8 **)(p - 0x14);
                m = n->unk_08;
                if (func_8003DE58(*(u8 **)(r0 + 0x8), r0, &out, 1) != NULL)
                {
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2);
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6);
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA);
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + (u16)out.vx;
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + (u16)out.vy;
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)m + 0xA) + (u16)out.vz;
                }
                dx = (rand() & 0x1F) - 0x10;
                dy = (rand() & 0x1F) - 0x10;
                *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + dx;
                *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + dy;
                *(s32 *)(q + 0x50) = dx << 12;
                *(s32 *)(q + 0x54) = dy << 12;
                *(s32 *)(q + 0x50) +=
                    ((UnkDngB09A28 *)((u8 *)D_8017610C + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))
                        ->unk_00
                    << 16;
                *(s32 *)(q + 0x54) +=
                    ((UnkDngB09A28 *)((u8 *)D_8017610C + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))
                        ->unk_02
                    << 16;
                *(s32 *)(q + 0x58) = -(rand() + 0x50000);
                *(s32 *)(q + 0x64) = 0x6000;
                r = (u8 *)n->unk_0C;
                *(s16 *)(r + 0x1C) = 0x800;
                *(s16 *)(r + 0x1E) = 0x800;
                r[0xE] = 0x80;
                r[0xD] = 0x80;
                r[0xC] = 0x80;
                *(s16 *)(r + 0x12) = 0x7DCF;
                *(u16 *)(r + 0x14) |= 0x100;
                func_8003DB94(r, D_800DE870, 0);
            }
            i++;
        } while (i < 2);
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0x78;
            p[0x9B] += 1;
        }
        break;
    case 2:
        if ((*(s8 *)(c + 0x4) == 2 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0x23;
        }
        if ((s16)(--*(u16 *)(p + 0x96)) <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            *(u16 *)(c + 0x14) &= 0xF7FF;
        }
        if (*(s16 *)(p + 0x96) == 0xA || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            tbl = (u8 *)D_8017610C;
            p[0xAD] = 0;
            xx = *(s16 *)((u8 *)b + 0x2) +
                 (((UnkDngB09A28 *)(tbl + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_00 << 6);
            yy = *(s16 *)((u8 *)b + 0x6) +
                 (((UnkDngB09A28 *)(tbl + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)))->unk_02 << 6);
            if ((s16)func_800A45D8(xx, yy, *(s16 *)((u8 *)b + 0xA)) != 0)
            {
                p[0xAD] = 1;
            }
            if (*(s8 *)(p + 0xAD) == 0)
            {
                func_8009A350(c[0x24], c[0x25], (*(u16 *)(d + 0x2A) >> 9) & 7, &sp18);
                if ((sp18 & 0x8400) != 0)
                {
                    p[0xAD] = 1;
                }
            }
            if (*(s8 *)(p + 0xAD) == 0)
            {
                if (func_b42_80174A00(
                        d, c[0x24] + *(u16 *)(tbl + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C)),
                        c[0x25] + *(u16 *)(tbl + ((*(u16 *)(d + 0x2A) >> 7) & 0x1C) + 2),
                        *(s16 *)((u8 *)b + 0xA)) != NULL)
                {
                    p[0xAD] = 1;
                }
            }
            if (*(s8 *)(p + 0xAD) == 0)
            {
                hh = func_800BCB04u(xx, yy, *(u16 *)((u8 *)b + 0xA) - 0x20);
                if (hh >= 0x200 || hh > *(s16 *)((u8 *)b + 0xA) + 0x20 ||
                    hh < *(s16 *)((u8 *)b + 0xA) - 0x20)
                {
                    p[0xAD] = 1;
                }
            }
            if (*(s8 *)(p + 0xAD) == 0)
            {
                func_b42_80175494(p, b, c);
            }
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            if (*(u8 **)(c + 0x2C) != D_b42_8017609C)
            {
                *(u8 **)(c + 0x2C) = D_b42_8017609C;
                *(u16 *)(c + 0x14) &= 0xF7FF;
                func_80047784(c,
                              (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) &
                                                   7],
                              0);
            }
            *(s16 *)(p + 0x96) = 0;
            if (*(s8 *)(p + 0xAD) == 0)
            {
                p[0x9B] += 1;
            }
            else
            {
                p[0x9B] = 0x10;
            }
        }
        break;
    case 3:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if ((*(u16 *)(p + 0x96) & 3) == 0)
        {
            k = *(s16 *)(d + 0x2A);
            k += 0x200;
            if (k >= 0x1000)
            {
                k -= 0x1000;
            }
            *(s16 *)(d + 0x2A) = k;
        }
        if (*(s16 *)(p + 0x96) == 0x13)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 4:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) >= 0xA || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            e = D_801760A4;
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        break;
    case 5:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            e = D_b42_8017609C;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) >= 0x14 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 7:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if ((*(u16 *)(p + 0x96) & 3) == 0)
        {
            k = *(s16 *)(d + 0x2A);
            k += 0x200;
            if (k >= 0x1000)
            {
                k -= 0x1000;
            }
            *(s16 *)(d + 0x2A) = k;
        }
        if (*(s16 *)(p + 0x96) == 0x13)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 8:
        if (*(s8 *)(p + 0xAC) == 0x4D)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 9:
        func_800AD594(d, 0x800);
        *(s32 *)(p + 0x8C) = (s32)func_b42_801714B8;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
    case 16:
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
        if (*(s16 *)(p + 0x96) >= 0x1E)
        {
            p[0x9B] = 9;
            *(s16 *)(p + 0x96) = 0;
        }
        break;
    }
}
