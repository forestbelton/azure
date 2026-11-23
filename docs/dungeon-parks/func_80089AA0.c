/* func_80089AA0 LANDS: 1165/1165 words, 0 differing, 0 aligned rows.
   2026-11-22. VERIFIED in a real unit copy of csrc/ovl_dungeon.c and in a
   whole-unit sweep -- all 204 landed definitions in the unit read 0 differing
   with this body and its declarations in place. The park is kept only until
   the merge lands, then delete it.

   ===================================== THE LAST THREE ROWS, CLOSED
   The park's residue was one register over three rows at 80089DD4
       ours    sra $s6, $v0, 16 / bgtz $s6 / bgez $s6
       target  sra $v1, $v0, 16 / bgtz $v1 / bgez $v1
   and the park had it diagnosed as two things that had to be true at once
   with no spelling doing both. ONE CHANGE DOES BOTH, and it is a partition
   move, not a device

       n1 = func_80042900((s8 *)e, 7) << 16
       r1 = n1 >> 16
       n1 = 0x80

   in place of the park's
       if (r1) { r1 = (s16)func_80042900((s8 *)e, 7) } else { same }
       n1 = 0x80

   MAKE n1 BE THE SIGN-EXTENSION TEMP. Three consequences, all read off the
   dumps of the landed body and all three needed

   1. THE li GAINS A REG_DEP_OUTPUT ON THE sll, so sched1 cannot hoist it.
      The sll now writes reg 116, which is n1, and the later `li n1, 0x80'
      writes the same pseudo, so sched.c records
          insn 417 (set (reg/v:SI 116) (const_int 128))
              (insn_list:REG_DEP_ANTI 409 (insn_list:REG_DEP_OUTPUT 412 ...))
      priority (sched.c:1456) is the max over LOG_LINKS of
      `priority (x) + insn_cost (x, prev, insn) - 1', so the output
      dependence on the sll raises the li from the call-anti-dependence value
      P(call) to P(sll), which is P(call) plus the call result latency minus
      one. The sll and the sra tie with it, the LUID tie-break at sched.c:2470
      then prefers the HIGHEST LUID, and the li is the latest of the three, so
      it is scheduled FIRST in sched.c's bottom-up order and lands LAST, after
      the sra. WHAT IT USED TO DO -- with a plain `r1 = (s16)call' the li was
      the only insn in the block that was ready while the call result was
      still in flight, so sched1 put it in the stall slot between the jal and
      the sll. reorg then moved it back into the bgtz delay slot, so the
      EMITTED TEXT was already right and only the live range was wrong.
   2. n1 THEREFORE NO LONGER CONFLICTS WITH HARD $v0. `.greg' for the park
      body reads `;; 116 conflicts: ... 2 ...' and for the landed body
      `;; 116 conflicts: 80 81 82 84 89 115 116 29' with no 2, because n1 is
      no longer live across the sll where the call result occupies $v0.
   3. n1 GAINS A $v0 PREFERENCE OF ITS OWN, which is the escape hatch the
      park had already identified at global.c:879 and could not reach.
      set_preference (global.c:1545) strips the first operand of an
      expression source, and the sll's source is `(ashift (reg:SI 2 v0) 16)',
      whose first operand is hard $v0. So `.greg' now prints
          ;; 116 preferences: 2
      and prune_preferences' merge loop removes a register the allocno itself
      prefers from the merged set when the sizes are equal, so $v0 never
      enters regs_someone_prefers[n1]. n1 takes $v0 in find_reg pass 0.
      And r1 LOSES its own $v0 preference at the same time, because the first
      operand of its source `(ashiftrt (reg/v:SI 116) 16)' is now a pseudo
      rather than the hard $v0 the old temp had been renumbered to -- so r1
      no longer competes for $v0, has no preference at all, and takes $v1 as
      the next register in order once n1 has $v0.

   THE DUPLICATED ARM IS GONE, AND WITH IT THE PARK'S ONLY UB. The park's
   `if (r1) { r1 = call } else { r1 = call }' read r1 before its only
   assignment, which is what made flow mark r1 live from function entry
   (`.lreg': used 5 times across 144 insns, crosses 8 calls) and what forced
   it to a callee-saved register. The landed body has no duplicated arm on r1
   and no uninitialised read in this region.

   MEASURED AROUND THE WINNING CHANGE, all 1165 words, on the park body
     n1 as the sign-extension temp, no arm                           0
     n1 as the temp spelt `(s32)((u32)call << 16)'                   0
     n1 as the temp but `one = 1' moved ahead of `n1 = 0x80'        12
     r1 as its own temp, `r1 = call << 16' then `r1 = r1 >> 16'      7
       -- this is the park's old `t6' result and the proof the win is the
       OUTPUT DEPENDENCE on n1 and not the split shift. A split shift into
       any other name leaves the li hoistable.
     the park body, `if (r1)' duplicated arm                         3
     no duplicated arm at all                                        7
     `if (v2)' duplicated arm                                       10
     `one = 1' moved after `n1 = 0x80', no arm                      11
     `one = 1' moved before `n1 = 0x80', no arm                     11

   THE OTHER THREE DEVICES ALL STILL EARN THEIR KEEP, re-ablated on the
   landed body, and every one of their conditions is definitely assigned
     `w2++ / w2--' after `w2 = D_800E3D7C'       removed: 108 rows
     `r7++ / r7--' after the func_800A2C34 call  removed:   7 rows
     `if (f13) { s3 = D_800E3DE8 } else { same }' removed:  64 rows and one
       word short -- f13 is assigned before the loop on every path
     w2 pair and r7 pair removed together       108 rows
     all three removed together                166 rows and one word short

   EVERYTHING ELSE IN THE PARK'S 543 -> 3 RECONSTRUCTION IS UNCHANGED and is
   recorded in git history at docs/dungeon-parks/func_80089AA0.c, including
   the fifteen settled facts, the ARRAY_REF / MEM_IN_STRUCT_P lever that was
   the biggest single step, the sentinel and walker partition, and the
   `s32 flagB[1]' frame.

   DECLARATIONS
   extern s32 D_800E4948[];
   extern s32 D_800DD160;
   extern s32 D_800E3D70;
   extern u16 D_800DD258;
   s32 func_80096100_4(u8 *, UnkVec *, u8 *, u8 *) __asm__("func_80096100");
   s32 func_8009FB34_i(u16, u16) __asm__("func_8009FB34");
   END DECLARATIONS

   AND ONE MORE, WHICH CANNOT GO IN THE BLOCK ABOVE BECAUSE
   tools/decomp/parkresidue.py CORRUPTS THE UNIT WHEN IT SEES IT. It is
   written below this comment instead, behind an empty comment

       extern u16 D_800DD25A __asm__("D_800DD258 + 2");

   NO FORWARD DECLARATION OF func_80089AA0 IS NEEDED. parksplice appends one
   and it is harmless here -- the unit has no C call site for this function,
   only the D_800DD168 dispatch table -- but the whole-unit sweep was run
   BOTH ways and both read 204 of 204 at 0 differing, so the merge should
   leave it out.

   PROTOTYPE CHANGE ON LANDING: none. func_80096100 is declared in the unit
   with ONE parameter and func_8009FB34 with an s16 return, and both call
   sites go through the `__asm__' aliases above, so neither landed definition
   is touched.
*/
extern u16 D_800DD25A __asm__("D_800DD258 + 2");
void func_80089AA0(u8 *p, UnkVec *c, u8 *d)
{
    s32 one;
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *v1;
    u8 *f1;
    u8 *k;
    u8 *f2;
    u8 *f3;
    u8 *f4;
    u8 *f5;
    u8 *f6;
    u8 *f7;
    u8 *f8;
    u8 *f9;
    u8 *f10;
    u8 *f11;
    u8 *f12;
    u8 *fA;
    u8 *f13;
    u8 *v2;
    u8 *q1;
    u8 *q2;
    u8 *q3;
    u8 *s1;
    u8 *s3;
    u8 *w2;
    void (*fn1)(u8 *, s32, s32);
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 r1;
    s32 n1;
    s32 n2;
    s32 r2;
    s32 n3;
    s32 fi;
    s32 r3;
    s32 mm;
    s32 r6;
    s32 r7;
    s32 i;
    s32 cnt2;
    s32 t1;
    s32 t2;
    s32 t3;
    u8 *ta;
    s32 t4;
    s32 flagB[1];

    g = (u8 *)&D_80083160;
    e = p;
    h = g + 0x18;
    if ((*(u16 *)0x80013714 & 4) != 0 && D_800E3CD0 == 0 && *(s32 *)(g + 0x10) != 0)
    {
        D_800E3CD0 = 1;
        func_80040AA0(3);
        return;
    }
    if (D_80080AA0 != 0 && D_800E3CD0 == 0 && func_80053EF0(1) == 0)
    {
        func_80053DA8(0x200);
        D_80080AA0 = 0;
        D_80080A88 = 1;
    }
    if ((*(u16 *)0x80013714 & 4) != 0)
    {
        v1 = D_800DCE60;
        *(s16 *)(v1 + 4) = *(s16 *)(v1 + 4) + (((D_80081468[0] & 1) != 0) ? 8 : -8);
        D_800832B4 = 0;
        func_8004D7A8(one);
        func_8004D294(NULL, D_800DCE60, 2);
    }
    *(s16 *)(p + 0x9E) = (*(u16 *)(h + 0xB0) >> 9) & 7;
    f1 = (u8 *)&D_80083460;
    *(u16 *)(f1 + 2) &= 0xFFCF;
    x0 = *(s32 *)(f1 + 8);
    if (x0 == D_800E4948[0] && (x1 = *(s32 *)(f1 + 0xC)) == D_800E4948[1] &&
        (x2 = *(s32 *)(f1 + 0x10)) == D_800E4948[2] &&
        (x3 = *(s16 *)(f1 + 0x14)) == *(s16 *)&D_800E4948[3] &&
        (*(u16 *)0x80013714 & 8) == 0 && p[0x9A] != 0x17 && p[0x9A] != 0x27 &&
        p[0x9A] != 0x29 && D_800DCF4F == 0 &&
        (x1 != 0 || x2 != 0 || x3 != 0 || x0 != 0))
    {
        D_800DD160++;
    }
    else
    {
        D_800DD160 = 0;
    }
    k = (u8 *)&D_80083460 + 8;
    func_8003DB6C(D_800E4948, (int *)k, 4);
    f2 = (u8 *)&D_80083460;
    if (*(s32 *)(f2 + 0xC) == 0 && *(s32 *)(f2 + 0x10) == 0 && (*(u16 *)(f2 + 2) & 8) == 0 &&
        *(s16 *)(f2 + 8) == 0 && *(s16 *)(f2 + 0x14) == 0 && (D_800E296C & 0x100000) == 0 &&
        *(s16 *)(f2 + 0xA) == 0 && (*(u16 *)(f2 + 2) & 0x2E01) == 0)
    {
        if ((s16)func_8009F5CC() != 0)
        {
            func_800C6AB0();
        }
        if ((*(u16 *)(f2 + 2) & 2) != 0)
        {
            *(u16 *)(f2 + 2) = (*(u16 *)(f2 + 2) & 0xFFFD) | 0x20;
            func_800A0DD8();
            n1 = func_80042900((s8 *)e, 7) << 16;
            r1 = n1 >> 16;
            n1 = 0x80;
            if (r1 <= 0)
            {
                n1 = 0x100;
                if (r1 < 0)
                {
                    n1 = 0x200;
                }
            }
            one = 1;
            *(s16 *)(k - 8) = n1;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                f3 = (u8 *)&D_80083460;
                *(u16 *)(f3 + 0x1E) = *(u16 *)(f3 + 0x1E) + 1;
                D_800E3D70 += e[0x29];
                n2 = e[0x28];
                while ((u32)D_800E3D70 > 0x100)
                {
                    D_800E3D70 -= 0x100;
                    n2++;
                }
                if (e[0x29] < n2)
                {
                    n2 = e[0x29];
                }
                e[0x28] = n2;
                for (i = 1; i >= 0; i--)
                {
                    q1 = *(u8 **)(p + 0xAC + i * 4);
                    if (q1 != NULL && q1[0x25] != 0)
                    {
                        D_800E3D74[i] += q1[0x29];
                        n2 = q1[0x28];
                        while ((u32)D_800E3D74[i] > 0x100)
                        {
                            D_800E3D74[i] -= 0x100;
                            n2++;
                        }
                        if (q1[0x29] < n2)
                        {
                            n2 = q1[0x29];
                        }
                        q1[0x28] = n2;
                    }
                }
            }
            if ((*(s32 *)(e + 0x1C) & 0x800) != 0)
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 8) + 0x30;
            }
            else
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 7) + 0x40;
            }
            e[3] = n3;
            *(s32 *)(p + 0x128) = func_800A6D30() & 0xFFFF;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                *(s16 *)(p + 0x118) = *(u16 *)(p + 0x118) + one;
            }
            func_800A4ACC(e);
            if (*(s32 *)(p + 0x8C) == (s32)func_80096384)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) == 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
                    f4 = (u8 *)&D_80083460;
                    *(u16 *)(f4 + 2) |= 4;
                }
            }
            else if (*(s32 *)(p + 0x8C) == (s32)func_8008ACDC)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) != 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_80096384;
                    f5 = (u8 *)&D_80083460;
                    *(u16 *)(f5 + 2) |= 4;
                }
            }
        }
        f6 = (u8 *)&D_80083460;
        *(u16 *)(f6 + 2) &= 0xBFFB;
        *(s32 *)(e + 0x14) &= 0x7FFE7FFF;
    }
    else
    {
        f7 = (u8 *)&D_80083460;
        *(u16 *)(f7 + 2) |= 4;
    }
    if ((*(u16 *)0x80013714 & 8) == 0)
    {
        if (p[0x9A] != 0x17 && D_800DCF4F == 0 && (u32)D_800DD160 >= 0x385)
        {
            q2 = D_800E3D7C;
            D_800DD160 = 0;
            do
            {
                q2[0x71] &= 0x7F;
                *(s16 *)(q2 + 0x6C) = 0;
                q2 = *(u8 **)(q2 + 0x5C) + 0x20;
            } while (q2 != D_800E3D7C);
            f8 = (u8 *)&D_80083460;
            *(s16 *)(f8 + 8) = 0;
            *(s16 *)(f8 + 0xA) = 0;
            *(s16 *)(f8 + 0x14) = 0;
            *(s32 *)(f8 + 0xC) = 0;
            *(s32 *)(f8 + 0x10) = 0;
            *(u16 *)(f8 + 2) = 0;
        }
        if ((*(u16 *)0x80013714 & 8) == 0)
        {
            if ((D_80083462 & 4) == 0 || (D_800E296C & 0x40) != 0 || p[0x9A] == 0x17)
            {
                D_800E296C &= ~0x40;
                v2 = D_800DCE60;
                if ((s16)func_800C77D0(e - 0x20, c, 8, *(s16 *)(v2 + 6)) != 0)
                {
                    D_800832B4 = 0;
                    func_8004D7A8(1);
                    func_8004D294(NULL, v2, 2);
                }
                else
                {
                    D_800E3D20 = 0;
                }
            }
        }
    }
    if ((*(s32 *)(e + 0x1C) & 0x20) == 0)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_800478B8(d);
        }
        else
        {
            func_80048AC8(d, one);
        }
    }
    else
    {
        *(u16 *)(d + 0x14) |= 0x6000;
    }
    if (*(s32 *)(p + 0x8C) != 0)
    {
        (*(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C))(p, c, d, e);
    }
    D_800DD168[p[0x9A]](p, c, d, e);
    fi = (((D_80083228 + *(s16 *)(e + 0x2A)) + 0x100) >> 9) & 7;
    if (*(s16 *)(p + 0x94) != fi)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_8003DB94(d, *(void **)(*(s32 *)(d + 0x2C) + fi * 4), *(s8 *)(d + 4));
        }
        else
        {
            func_800489F4(d, *(*(u8 **)(d + 0x2C) + fi), *(s8 *)(d + 4), one);
        }
        *(s16 *)(p + 0x94) = fi;
        if (p[0x9A] != 0x22 && D_8006CCF8[fi] != 0)
        {
            *(u16 *)(d + 0x14) |= one;
        }
        else
        {
            *(u16 *)(d + 0x14) &= 0xFFFE;
        }
    }
    r3 = func_8009FB34_i(d[0x24], d[0x25]);
    d[0x26] = r3;
    mm = (s8)r3;
    if (mm >= 0)
    {
        D_800E2970_s[mm].unk_0C |= one;
    }
    func_800A020C(*(s32 *)(e + 0x1C), d + 0xC);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        c->unk_00 += c->unk_0C;
        c->unk_04 += c->unk_10;
        if ((*(u16 *)(p + 0x98) & 8) == 0)
        {
            c->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
            p[0x9D] = p[0x9D] + one;
        }
        *(s32 *)(p + 0x90) += c->unk_14;
        r6 = (s16)func_800BCB04(*(u16 *)((u8 *)c + 2), *(u16 *)((u8 *)c + 6),
                                (s16)(*(u16 *)(e + 0x88) - 0x20));
        if (r6 < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(e + 0x88) >= r6)
            {
                if ((*(u16 *)(p + 0x98) & 4) != 0 || c->unk_14 < 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                else
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
                    {
                        if ((D_800E296C & 0x80000) == 0)
                        {
                            func_800A56E0(0x508);
                        }
                        func_800B66C8((u8 *)c);
                    }
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
                *(u16 *)(p + 0xA2) &= 0xF7FF;
            }
            else
            {
                if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                *(u16 *)(p + 0xA2) |= 0x800;
            }
            if ((*(s32 *)(e + 0x1C) & 0x40000000) != 0)
            {
                *(s32 *)(e + 0x1C) &= 0xBFFFFFFF;
                *(s16 *)(e + 0x8A) = *(u16 *)(e + 0x88);
                r6 = (s16)func_800BCB04((d[0x24] << 6) | 0x20, (d[0x25] << 6) | 0x20,
                                        (s16)(*(u16 *)(e + 0x88) - 0x20));
                *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + (*(u16 *)(e + 0x88) - r6);
                *(s16 *)(e + 0x88) = r6;
                if ((D_80083462 & 0x80) != 0)
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
            }
        }
        else
        {
            if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFEF;
            }
            *(u16 *)(p + 0xA2) |= 0x800;
        }
        *(s16 *)((u8 *)c + 0xA) = *(u16 *)(e + 0x88) + *(u16 *)(p + 0x92);
    }
    func_80096100_4(p, c, d, e);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        if (D_800DD258 != d[0x24] || D_800DD25A != d[0x25])
        {
            func_8009D3B0();
            if ((s16)func_800A2C34(NULL) == 0)
            {
                if ((s16)func_800A4474(d[0x24], d[0x25]) != 0)
                {
                    *(u16 *)(p + 0xA2) |= 0x200;
                }
                else
                {
                    *(u16 *)(p + 0xA2) &= 0xFDFF;
                }
            }
        }
        q3 = func_8009B25C(e, d[0x24], d[0x25], *(s16 *)(e + 0x88));
        if (q3 != NULL)
        {
            func_8009A180(q3, (u8 *)D_800814A8);
            *(s32 *)(q3 + 0x1C) &= 0x7FFFFFFF;
        }
        D_800DD258 = d[0x24];
        D_800DD25A = d[0x25];
    }
    q3 = *(u8 **)(e + 0x5C) + 0x20;
    ta = e;
    while (q3 != ta)
    {
        *(u16 *)(*(u8 **)(q3 - 0x14) + 0x14) &= 0xFFBF;
        q3 = *(u8 **)(q3 + 0x5C) + 0x20;
    }
    f9 = (u8 *)&D_80083460;
    if ((*(u16 *)(f9 + 2) & 0x800) != 0 && (s16)func_800A2C34(NULL) == 0 &&
        *(s16 *)(f9 + 8) == 0 && *(s16 *)(f9 + 0x14) == 0)
    {
        *(u16 *)(f9 + 2) = (*(u16 *)(f9 + 2) & 0xF7FF) | 0x2000;
    }
    f10 = (u8 *)&D_80083460;
    if ((*(u16 *)(f10 + 2) & 0x400) == 0)
    {
        if ((*(u16 *)(f10 + 2) & 0x2000) != 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    *(s16 *)(q3 + 0x8A) = *(u16 *)(q3 + 0x88);
                    *(s16 *)(q3 + 0x6A) = *(u16 *)(q3 + 0x2A);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                ta = e;
                do
                {
                    s1 = *(u8 **)(e + 0x5C);
                    q3 = s1 + 0x20;
                    i = 0;
                    if (q3 != e)
                    {
                        do
                        {
                            D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                            if (((s32 *)s1)[4] > 0)
                            {
                                fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                                fn1(s1 + 0x20, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                                if ((*(u16 *)(s1 + 0x1E) & 0x8000) == 0 && *(s8 *)(q3 + 0x71) < 0 &&
                                    *(s8 *)(q3 + 0x6D) != 0)
                                {
                                    i++;
                                }
                            }
                            s1 = D_800E3DE8;
                            q3 = s1 + 0x20;
                        } while (q3 != ta);
                    }
                } while (i != 0);
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    func_800A9AFC(*(u8 **)(q3 - 0x14), q3);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                f11 = (u8 *)&D_80083460;
                *(u16 *)(f11 + 2) = (*(u16 *)(f11 + 2) & 0xDFFF) | 0x1000;
            }
            else
            {
                *(u16 *)(f10 + 2) &= 0xDFFF;
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                while (q3 != e)
                {
                    s1 = q3 - 0x20;
                    D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                    if (((s32 *)s1)[4] > 0)
                    {
                        fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                        fn1(q3, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                    }
                    q3 = D_800E3DE8 + 0x20;
                }
                f12 = (u8 *)&D_80083460;
                *(u16 *)(f12 + 2) |= 0x2000;
            }
        }
    }
    fA = (u8 *)&D_80083460;
    i = 0;
    if (*(s32 *)(fA + 0x10) > 0)
    {
        *(s32 *)(fA + 0x10) = 0;
    }
    flagB[0] = 0;
    t4 = *(u16 *)(fA + 2);
    f13 = fA;
    *(u16 *)(f13 + 2) = t4 | one;
    for (;;)
    {
        if ((D_800E296C & 0x100000) != 0 && (s16)func_800A2C34(NULL) == 0)
        {
            e = D_800E3D7C;
            do
            {
                n2 = (e[0x41] << 8) | e[0x42];
                e[0x42] = 0;
                e[0x41] = 0;
                *(s32 *)(e + 0x18) += n2;
                if ((s16)func_800A1C58(e) != 0)
                {
                    *(u8 **)(f13 + 0xC) = e;
                    goto walk;
                }
                e = *(u8 **)(e + 0x5C) + 0x20;
            } while (e != D_800E3D7C);
            D_800E296C &= 0xFFEFFFFF;
        }
    walk:
        w2 = D_800E3D7C;
        w2++;
        w2--;
        s3 = *(u8 **)(w2 + 0x5C);
        e = s3 + 0x20;
        cnt2 = 0;
        while (e != w2)
        {
            D_800E3DE8 = *(u8 **)(e + 0x5C);
            if (flagB[0] != 0 && *(s8 *)(e + 0x6D) == 0 && *(s16 *)(f13 + 0x14) == 0)
            {
                goto next;
            }
            if (*(s32 *)(s3 + 0x10) <= 0)
            {
                goto next;
            }
            r7 = (s16)func_800A2C34(NULL);
            r7++;
            r7--;
            if (*(s16 *)(f13 + 0x14) != 0)
            {
                if (r7 == 0)
                {
                    if ((*(u32 *)(s3 + 0xB8) & 0xFFFF0000) != 0x150000)
                    {
                        goto next;
                    }
                    *(u8 **)(f13 + 0xC) = s3 + 0x20;
                }
            }
            fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s3 + 0x10) | 0x80000000);
            fn1(s3 + 0x20, *(s32 *)(s3 + 8), *(s32 *)(s3 + 0xC));
            if ((*(u16 *)(s3 + 0x1E) & 0x8000) != 0)
            {
                break;
            }
            if (*(s8 *)(e + 0x6D) != 0)
            {
                cnt2++;
            }
            if (r7 != 0 && (s16)func_800A2C34(NULL) == 0)
            {
                break;
            }
        next:
            if (f13)
            {
                s3 = D_800E3DE8;
            }
            else
            {
                s3 = D_800E3DE8;
            }
            e = s3 + 0x20;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x1000) != 0)
        {
            if (*(s16 *)(f13 + 8) == 0)
            {
                if (*(s16 *)(f13 + 4) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = (t1 & 0xEFF7) | 0x4000;
                if (cnt2 != 0)
                {
                    flagB[0] = one;
                    continue;
                }
            }
        }
        else if ((t1 & 0x400) != 0)
        {
            if ((t1 & 0x800) == 0 && (s16)func_800A2C34(NULL) == 0 &&
                (s16)func_800A2B28() == 0)
            {
                if (i == 0)
                {
                    i = one;
                    continue;
                }
                t2 = *(u16 *)(f13 + 2);
                t3 = t2 & 0xFBFF;
                *(u16 *)(f13 + 2) = t3;
                if ((t2 & 0x4000) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = t3 | 0x800;
                if (func_800A5C70() != 0)
                {
                    *(u16 *)(f13 + 2) |= 0x80;
                }
                return;
            }
        }
        if (e == D_800E3D7C && cnt2 == 0)
        {
            *(u16 *)(f13 + 2) &= 0xFFFE;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x80) == 0 || *(s32 *)(f13 + 0xC) != 0 || *(s32 *)(f13 + 0x10) != 0 ||
            *(s16 *)(f13 + 0xA) != 0 || *(s16 *)(f13 + 8) == 0)
        {
            break;
        }
        *(u16 *)(f13 + 2) = t1 & 0xFFF7;
    }
}
