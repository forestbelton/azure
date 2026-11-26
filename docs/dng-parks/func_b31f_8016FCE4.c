/* PARK: func_b31f_8016FCE4 (ovl_dng_b31f) -- 332 words / 332 words /
   3 ALIGNED DIFFERENCES.  Agent h186, batch 35; agent h230, 2026-11-26.
   One-member family.  OWNS NO JUMP TABLE.

   THE WORD COUNT IS EXACT AND THE ONLY RESIDUE IS ONE REGISTER, three rows:

     173 8016FF98   lh a0,42(s2)   against  lh $v1, 0x2A($s2)
     179 8016FFB0   subu s0,a0,v0  against  subu $s0, $v1, $v0
     182 8016FFBC   addu s0,a0,v0  against  addu $s0, $v1, $v0

   The loop's heading value lands in $a0 where the target has $v1.  Nothing
   else in 332 words differs.

   h230: THE CAUSE IS NOW MEASURED END TO END, AND IT IS A CONFLICT, NOT A
   PREFERENCE-BREAK. Four facts, each read off a `-da' dump of a standalone
   onefunc probe (0.1 s a candidate):

   1. `e' is allocno 86 (`;; 86 conflicts: 80 82 83 86 89 90 98 380 2 29'
      plus `;; 86 preferences: 4'), and it is the FIRST allocno allocated
      (`;; 23 regs to allocate: 86 134 ...'), so allocation ORDER and
      priority are not levers -- nothing else can take $a0 first.
   2. THE $a0 PREFERENCE IS SEEDED BY LOCAL-ALLOC, NOT BY GLOBAL. `.lreg'
      says `;; Register 273 in 4': 273 is the `sll' temp of `(s16)s', tied
      to the hard $a0 of `(set (reg:SI 4 a0) (ashiftrt (reg 273) 16))' by
      combine_regs. global.c:1545 `set_preference' then runs on
      `(set (reg 273) (ashift (reg 92) 16))', and because
      `GET_RTX_FORMAT(code)[0] == 'e'' it takes the FIRST OPERAND of the
      expression (`src = XEXP (src, 0)', copy = 0), so `s' (allocno 92) gets
      hard_reg_preferences bit 4 from a reg whose hard number comes out of
      `reg_renumber' -- i.e. out of local-alloc's result.
   3. global.c:783 `expand_preferences' then copies it to `e', because `e'
      dies in the insn that sets `s' (insns 517 and 536,
      `REG_DEAD (reg/v:SI 86)') and the two do not conflict.
      global.c:836 `prune_preferences' strips bit 4 from `s' (it crosses a
      call, so $a0 is in its conflicts) and KEEPS it on `e' (which crosses
      none). global.c:1038 then overrides the scan's $v1 with $a0.
   4. SO EVERY SPELLING OF THE ARGUMENT RE-SEEDS IT. The target's own
      listing has `sll $a0, $s0, 16 / sra $a0, $a0, 16' at 8016FFC0, so the
      $a0-tied shift temp exists in the ORIGINAL too and `s' has the
      preference there as well. The park's "WHAT I WOULD TRY NEXT -- break
      `s's $a0 preference" axis is therefore CLOSED: the seed is the
      instruction stream, which is already byte-correct.

   AND THE `regs_someone_prefers' ROUTE IS CLOSED BY SOURCE. It would let a
   conflicting allocno's preference push $a0 out of `used' in pass 0, but
   prune_preferences does `AND_COMPL (temp, hard_reg_full_preferences
   [allocno])' when the other allocno's size is <= this one's -- and `e'
   prefers $a0 itself, at the same size, so $a0 can never enter
   regs_someone_prefers[86].

   WHAT IS LEFT IS THE ONE THING THE LANDED TWIN HAS: a hard-register
   CONFLICT with $a0, which makes prune_preferences delete the preference
   outright. Measured on the twin func_b31f_8016BF74 (landed, 457 words, 0
   differing) by compiling it standalone: it emits `lh $3, 42($18)' -- $v1,
   our target -- and its heading allocno reads
   `;; 87 conflicts: 80 82 83 87 91 92 101 499 2 4 5 29' WITH NO
   PREFERENCES LINE AT ALL. The twin gets 4 and 5 into that list because its
   `e' is shared with TWO pre-loop actor lookups whose ranges are live while
   $a0/$a1 are being loaded. The twin also PAYS a `move $3,$2' for that
   sharing (visible in its own compile) and its target has that word; this
   body's target has none, which is why the park's e2-sharing row is 333.

   h230's NEW LEVER, AND IT IS THE FIRST THING THAT HAS EVER MOVED THESE
   THREE ROWS: share `e' with `t' -- the `*(u8 **)(e2 - 0x14)' pointer in the
   0x400 arm, which is live across func_800A0818's argument setup.

       spelling                                            words | aligned
       the body below (separate `e', `e2', `t')              332  |   3
       `e' used for `t': e = (s32)*(u8 **)(e2 - 0x14), then
         *(u8 *)(e + 0x24) / *(u8 *)(e + 0x25) as the args   332  |   5
       `e' used for the 5th argument, (u16 *)e               331  |   7

   THE t-SHARING ROW IS WORTH READING EVEN THOUGH 5 > 3. All three heading
   rows are GONE -- `lh $v1, 0x2A($s2)', `subu $s0, $v1, $v0' and
   `addu $s0, $v1, $v0' all match -- and `;; 86 conflicts: ... 2 4 5 6 29'
   with NO preferences line confirms the mechanism transferred. The five rows
   it leaves are all one register, in the arm:

     -        47 8016FDA0                       lbu $a0, 0x24($s3)
     R   47   48 lw v1,-20(v0)                  lw $v0, -0x14($v0)
     +   48      lbu a0,36(s3)
     R   50   50 lbu a2,36(v1)                  lbu $a2, 0x24($v0)
     R   51   51 lbu a3,37(v1)                  lbu $a3, 0x25($v0)

   i.e. the target wants `t' in $v0 (reusing the dying call result) while the
   merged allocno has to be $v1 for the loop. ONE allocno cannot be both, so
   `t' is not the final partner -- but a partner that conflicts with $a0 and
   whose own range the target ALSO holds in $v1 would land this body.

   SO THE NEXT MOVE IS A PARTNER SEARCH WITH A STATED TEST, not a spelling
   sweep: a local whose live range (a) overlaps some call's argument setup,
   so $a0 enters hard_reg_conflicts, and (b) is held in $v1 in the target
   listing. I read the target listing for $v1 and the only pre-loop locals
   there are `v' (`lw $v1, 0x1C($s2)', which does NOT overlap an argument
   setup -- hence the park's 7 rows) and `k' (`lbu $v1, 0x45($s2)' ...
   `addu $a0, $v1, $a0', which DOES, and is the park's 331/35 row). Neither
   works as written, which leaves the possibility that the original carries a
   local this body does not have.

   WHAT IT DOES: the bank's walk step -- a 332-word REDUCTION of the landed
   func_b31f_8016BF74 in this same file (457 words; READ THAT COMMENT AND
   THROUGH IT func_b25_801726EC'S AND THE RESIDENT func_800D92C0'S).  The
   deltas, all read off this listing:
   1. the opening gate reads the SCALAR D_80083462 twice (one `lhu', the
      0x4000 and 0x2000 tests off the same register), not
      `g = (u8 *)&D_80083460' with +0x2 -- while the TAIL still needs the
      pointer local `g2' for D_80083460 + 0x8;
   2. the first gate's failure is a bare func_800A9A0C(d) + return, with
      none of 8016BF74's d[0x12] / func_b31f_8016C720 interrogation and no
      `*(u8 **)(g + 0xC) == d' arm;
   3. the second top-level arm is NOT 8016BF74's p[0xB2] test: this listing
      goes straight from the 0x410 gate to `(*(u16 *)(d + 0x46) & 0x8000)
      == 0' and has NO `j = *(s8 *)(c + 0x26)' / D_800E2970 arm at all --
      that is 125 of the 125 missing words;
   4. func_8009A66C at both sites, as in 8016BF74;
   5. the ELSE arm of state 3 adds `*(u16 *)(d + 0x46) &= 0x7FFF' before
      `p[0x9C] = c[0x26]'.
   Everything else is 8016BF74's text, including its `cc2' do-while(0)
   reference bump, `tb' for D_8006CCD8 with `*(u8 *)(kk + (s32)tb)' index
   first, `s32 i', D_8006CD00[i] inline with no `tbl' local, and
   `i = (s16)func_800BCB04(...)'.

   TWO FIXES THIS BODY NEEDED OVER THE TWIN'S TEXT, both measured:
   | spelling                                            | words | aligned |
   | the func_800A02AC result in its OWN `e2' local      |  332  |    3    |
   | that result reusing `t' (t = ...; t = *(u8 **)(t-0x14)) | 332 |   5    |
   | that result SHARING `e' with the loop heading       |  333  |    7    |
   The twin shares `e' between the actor lookups and the loop heading; here
   that costs a word (`move v1,v0') because this body has only ONE pre-loop
   lookup, so the shared local is long-lived where the target's is not.

   EVERY CANDIDATE PARTNER TRIED BY h186 (and none reaches 0):
   | the heading value shares...                         | words | aligned |
   | its own `e' local (as written below)                |  332  |    3    |
   | `cc2'                                               |  332  |    5    |
   | `v'                                                 |  332  |    7    |
   | `e2' (the func_800A02AC result)                     |  333  |    7    |
   | `y'                                                 |  332  |   11    |
   | `m'                                                 |  333  |   12    |
   | `x'                                                 |  331  |   14    |
   | `kk'                                                |  333  |   18    |
   | `k'                                                 |  331  |   35    |
   | `s' itself (s = load; s = s -/+ tbl[i])             |  332  |    3    |
   | `f' (the s16 flag)                                  |  329  |  128    |
   | `t' (h230)                                          |  332  |    5    |
   | the 5th argument pointer (h230)                     |  331  |    7    |
   And off the SET/shape side, all still 3: `u32 e' (3), `s16 e' (333/12),
   the gate inverted with the arms swapped (4), `s16 s' with no (s16) cast
   at the func_8009A66C call (3), the ternary form (3), `e' moved to any of
   eight declaration positions (3, 3, 3, 3, 3, 3, 3, 3 -- FLAT, so
   allocation priority is ruled out, which fact 1 above now explains: `e' is
   allocated first in every variant), the heading read inline in both arms
   (334/7), an explicit `s16 *tbl' local (333/22), a do { } while (0) round
   the heading read (333/9), and `&& e == e' after the call (3).

   NOT WORTH RE-RUNNING: the permuter. h186 put it next, but the residue is
   which hard register an existing pseudo gets, decided by a preference
   seeded in local-alloc off a byte-correct instruction stream; a spelling
   search cannot address it (and a 376,000-candidate run on exactly this
   shape moved nothing elsewhere in the project).

   DECLARATIONS
   extern u16 D_80083462;
   extern u8 D_80082E80[];
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s16 D_8006CD00[];
   extern u16 D_80082EA4;
   extern u8 *func_800A02AC(u8 *p, u16 x, u16 y);
   u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
   s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
   s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
   s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
   s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
   s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
   void func_8009A3D0(s32 x, s32 y, s32 f);
   void func_8009A21C(s32 a, s32 b, s32 c);
   s32 func_8009A180(u8 *a, u8 *b);
   void func_800A9A0C(u8 *);
   u16 func_800A6D30(void);
   s32 func_800BCB04(s32, s32, s16);
   END DECLARATIONS */
void func_b31f_8016FCE4(u8 *p, void *b, void *c, u8 *d)
{
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u16 *w;
    s16 f;
    s32 i;
    s16 m;
    s32 s;
    s32 v;
    s32 k;
    s32 kk;
    s32 x;
    s32 y;
    u8 *tb;
    s32 e2;

    f = 0;
    if ((D_80083462 & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e2 = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e2 != 0)
            {
                t = *(u8 **)(e2 - 0x14);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
    {
        if ((v & 0x20000) != 0)
        {
            k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
            x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
            y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
            if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
            {
                d[0x71] &= 0x7F;
                return;
            }
            w = (u16 *)(p + 0x98);
            m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
            *(s16 *)(d + 0x2A) = m;
            if ((s16)func_8009A66C(m, (u8 *)c, d, 0x20) <= 0)
            {
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
            }
            if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
            {
                f = 1;
            }
        }
        else
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
    }
    i = 0;
    tb = (u8 *)D_8006CCD8;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - D_8006CD00[i];
        }
        else
        {
            s = e + D_8006CD00[i];
        }
        if ((s16)func_8009A66C((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            *(u8 *)(c + 0x24) += *(u8 *)(kk + (s32)tb);
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = (s16)func_800BCB04((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}
