/* PARKED: func_b24_80174C64 (ovl_dng_b24), 530 target words.
   BEST: 530 candidate words, 3 differing (ovlverify), 3 aligned differences
   (aligndiff, R rows INCLUDED).  The shape is fully settled -- the word count
   is exact, every opcode, immediate and displacement matches, and the whole
   residue is three consecutive words in ONE basic block.  1 copy (dngfam.py).
   agent h280 (dir agent-h280), batch 35.

   THIS FUNCTION OWNS jtbl_b24_80170888, five entries at 0x80170888:
   .Lb24_80175374, .Lb24_80175388, .Lb24_801753AC, .Lb24_801753C0,
   .Lb24_801753F0 -- cases 0..4 of p[0xB1] under p[0xB3] == 3, MINVAL 0 (a
   bare `sltiu $v0, $v1, 0x5' with no subtraction), five distinct targets, and
   NO DEFAULT AMONG THEM: the range check's `beqz' goes to .Lb24_80175468,
   the shared func_800BCB04 tail, which is not a table entry.  The standalone
   compile of the body below emits the five entries in exactly that order
   (tools/cdkgcc -S, the .rdata table reads $L605 $L606 $L608 $L609 $L611 in
   strictly increasing address order) and aligndiff reports ZERO differences
   anywhere in the 0x80175334..0x80175468 region, so the arm order is right.
   The one word at 0x8017088C of padding in the listing is the table's own
   trailing 0x00000000 and is not a case.

   WHAT IT IS.  The banked room script's four-stage p[0xB3] push/pull
   sequence, called from this unit's landed func_b24_8017558C case 3.  It is
   FOUR SEQUENTIAL `if' TESTS on p[0xB3] -- 0, 1, 2, 3 -- not a switch: the
   listing reloads `lbu $v1, 0xB3($s0)' before each one and the first test's
   failure jumps into the SECOND test's body at .Lb24_80174ED8, reusing the
   already-loaded byte.  Stage 0 and stage 1 each run a p[0xB1] sub-machine
   with a p[0xB2] sub-sub-machine inside it; stage 2 is the abort publish
   (D_800E296C 0x800000, func_8009D8A4, func_800A56E0(0x80F)); stage 3 is the
   five-case jump table.  func_800D5F80(b, d) is the resident clearance test
   in csrc/ovl_dungeon_800D50FC.c and its result is taken as (s16) at all four
   call sites.  d + 0x2A is the facing angle, b + 0x2 / + 0x6 the position,
   and `k' at the top is the eight-way facing index that selects the
   D_800E2468 (dx, dz) row.

   STRUCTURE FACTS THAT ARE NOT NEGOTIABLE, each read off the listing:

   - stage 1's p[0xB1] machine is a LOOP: four backward jumps re-enter case
     bodies after a state store (.L801750D8 to .L80175080, .L801750E4 to
     .L80175030, .L8017529C to .L80175240, .L801752AC to .L801750EC).  It is
     written here as `switch' + labels inside the arms + `goto', NOT as
     `for (;;)' + `continue'.  THAT IS A MEASURED FACT, not a style choice:
     the `for (;;)' spelling makes loop.c hoist the three switch comparison
     constants 1, 2 and 3 into callee-saved registers, which costs THREE extra
     prologue saves -- frame -0x38 against the target's -0x28 -- and it
     measured 520 words / 215 aligned differences.  A goto-formed loop gets no
     NOTE_INSN_LOOP_BEG, so loop.c never sees it.
   - the three `p[0xB3] = 3; p[0xB1] = 0;' sites cross-join to .L801752B8 and
     that block JUMPS OVER the p[0xB3] == 2 test to .Lb24_80175334.  The
     `goto past2' after the shared store is what produces that jump; falling
     out of the if-chain instead leaves the p[0xB3] == 2 test in the way and
     is one word short.  cse cannot fold it away for us: cse_end_of_basic_block
     stops at every CODE_LABEL and .L801752C4 has many uses.
   - `p[0xB2] += 1' and NOT `p[0xB2] = 1' in stage 1 / p[0xB1] case 2 /
     p[0xB2] case 0.  cse follows the dispatch's own `beqz' into that arm
     (LABEL_NUSES == 1, BARRIER in front), so it knows p[0xB2] == 0 there,
     folds the increment to the constant 1 and reuses the register the
     compare chain already put 1 in.  Spelled `p[0xB2] = 1' it materialises
     the constant again: 531 words / 43 aligned differences against 530 / 34.

   MEASUREMENT TABLE.  Every row was measured on the body below with one
   change, restored to byte-identical in between, and every figure is
   `aligndiff' with R rows included.

   | spelling                                                 | words | aligned |
   | the body below                                           |  530  |    3    |
   | stage 1's p[0xB1] machine as `for (;;)' + `continue'      |  520  |  215    |
   | ONE `t' local for both D_800E2468 row sites               |  530  |   29    |
   | ONE `q' local for block2 and the history shift loop       |  531  |   30    |
   | ONE `tb' local for both D_800E2468 base sites             |  529  |   44    |
   | no `tb' at all, inline `t = D_800E2468[k]'                |  530  |   48    |
   | `p[0xB2] = 1' in stage 1 / B1 case 2 / B2 case 0          |  531  |   43    |
   | history shift via `r + 0x62 + i * 4' subscripts, no       |       |         |
   |   `dst'/`g' pointers (lsr then COMBINES the two givs)     |  525  |   51    |
   | separate loop counters `i' and `j'                        |  529  |   40    |
   | node-seed stores in the order +0x62 +0x68 +0x66 +0x64     |  530  |    7    |
   | block2 scan subscripted off `q + 0x82 + i * 4'            |  530  |   48    |
   | block2 loop invariants read as `q + 0x86' / `q + 0x88'    |  531  |   14    |
   | block2 `r' recomputed inside the loop body                |  530  |    6    |
   | block2 hand-strength-reduced pointer `gp = q + 0x28'      |  530  |    6    |
   | block2 with a SECOND `r' local of its own                 |  530  |   29    |
   | an explicit local for the hoisted +0x66 invariant         |  530  |    7    |
   | declaration order permuted (q, h first)                   |  530  |    3    |
   | block2 as `if (X != NULL) { q = X; ... }'                 |  530  |    3    |
   | block2 as `q = X; if (q != NULL) { ... }'                 |  530  |    3    |

   The last three rows are MEASURED AND FREE, so none of them is the reason
   this body scores 3, and declaration order is not a lever here either.

   WHAT IS LEFT: THREE WORDS, ONE CAUSE, AND THE ARITHMETIC IS KNOWN.

       word  target                          candidate
       198   lw    $v0, 0xAC($s0)            lw   $v1, 0xAC($s0)
       200   beqz  $v0, .Lb24_80174FEC       beqz $v1, .Lb24_80174FEC
       201   addu  $v1, $v0, $zero           nop

   The target holds the node pointer in TWO pseudos in that block -- a short
   one in $v0 that only the `beqz' reads, and a long one in $v1 that feeds
   both `addiu $a2, $v1, 0x20' and the loop preheader's `addiu $a0, $v1, 0x28'
   -- and pays one reg-to-reg copy for it, which reorg then puts in the
   branch's delay slot.  Every spelling of `q = <the load>' collapses the two,
   because cse1's make_regs_eqv keeps the LOAD's pseudo as qty_first_reg
   unless the copy's destination either lives beyond the cse basic block or
   was first referenced before it starts, and cse1's block here runs from
   block2's label THROUGH the `||' test and the preheader (flag_cse_skip_blocks
   lets it skip the second `||' arm), so `r = q + 0x20' and even a repeated
   `r = q + 0x20' in the preheader are both inside it.

   TWO VARIANTS DO PRODUCE THE COPY, AND BOTH ARE 6 ROWS, NOT 0:

   | variant                                                  | words | aligned |
   | symmetric `h = X; if (h) { q = h; ... }' in BOTH 0xAC     |       |         |
   |   blocks -- copy present, in the delay slot, block1       |       |         |
   |   byte-exact, ONLY q and the hoisted +0x66 temp swap      |  530  |    6    |
   | a dead `g = q' inside the scan loop body                  |  530  |    6    |
   | a dead `q = <the load>' as the arm's first statement       |  530  |    3    |
   |   (block2 byte-EXACT, block1's pointer takes $v1)          |       |         |

   The symmetric variant is saved at scratch/azure-dng/agent-h280/sym6.c and
   is the better LEAD even though its number is worse: its whole residue is
   q and the loop.c-hoisted +0x66 invariant exchanging $v1 and $a0, both
   caller-saved, i.e. a wrong register WITHIN a class.  The arithmetic, from
   global.c's allocno_compare and find_reg:

       q             3 refs / 12 insns  ->  floor_log2(3)*3/12 = 0.25
       +0x66 temp    3 refs /  9 insns  ->  floor_log2(3)*3/9  = 0.33

   so the temp sorts first and takes $v1, and q falls to $a0.  The target
   needs q first, which wants q at 4 or more refs or the temp at 12 or more
   insns.  MEASURED AND NOT IT: a fourth q reference bought by reading the
   `||' second arm as `q + 0x88' (530 / 7 -- the ref count rose and the
   registers did NOT move), and an explicit local for the invariant, which
   lengthens the temp but costs a word elsewhere (530 / 7).

   CAUSE: ARITHMETIC.  One reg-to-reg copy that cse1 deletes, and the
   allocation margin that decides its register, both quantified above.

   NOT RUN: relocheck.py (it needs `build/csrc/ovl_dng_b24.s', i.e. a build).
   relcand.py on this body is `ok (2 data symbols, 9 calls), 0 mismatches'.
   declcheck b24 is `0 with an undeclared or mistyped callee'.

DECLARATIONS
s32 func_800D5F80(u8 *, u8 *);
extern s16 D_800E2468[][2];
extern u32 D_800E296C;
void func_8009D8A4(void);
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
END DECLARATIONS
*/
void func_b24_80174C64(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 *t;
    s16 *t2;
    s16(*tb)[2];
    s16(*tb2)[2];
    u8 *q;
    u8 *dst;
    u8 *r;
    u8 *g;
    s32 k;
    s32 i;
    s32 ang;
    s32 e;

    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    if (p[0xB3] == 0)
    {
        switch (p[0xB1])
        {
        case 0:
            switch (p[0xB2])
            {
            case 0:
                if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
                {
                    p[0xB3] = 2;
                }
                else if (p[0xB4] >= 7)
                {
                    if ((*(u16 *)(d + 0x2A) & 0x3FF) != 0)
                    {
                        p[0xB2] = 2;
                    }
                    else
                    {
                        p[0xB1] = 1;
                        p[0xB2] = 0;
                        *(s16 *)(p + 0x96) = 0;
                    }
                }
                else if ((*(u16 *)(d + 0x2A) & 0x3FF) != 0)
                {
                    p[0xB2] = 2;
                }
                else if ((s16)func_800D5F80((u8 *)b, d) != 0)
                {
                    *(s16 *)(p + 0x96) = 0x1E;
                    p[0xB2] = 0;
                    p[0xB1] = 0;
                    p[0xB3] += 1;
                    if (*(u8 **)(p + 0xAC) != NULL)
                    {
                        r = *(u8 **)(p + 0xAC) + 0x20;
                        *(s16 *)(r + 0x62) = *(u16 *)((u8 *)b + 0x2);
                        *(s16 *)(r + 0x64) = *(u16 *)((u8 *)b + 0x6);
                        *(s16 *)(r + 0x68) = 0;
                        *(s16 *)(r + 0x66) = 0;
                    }
                }
                else
                {
                    p[0xB2] = 2;
                }
                break;
            case 1:
                if ((*(s16 *)(p + 0x96))-- <= 0)
                {
                    p[0xB2] = 0;
                }
                break;
            case 2:
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
                if (*(s16 *)(d + 0x2A) >= 0x1000)
                {
                    *(s16 *)(d + 0x2A) = 0;
                }
                p[0xB2] = 1;
                *(s16 *)(p + 0x96) = 1;
                p[0xB4] += 1;
                break;
            }
            break;
        case 1:
            switch (p[0xB2])
            {
            case 0:
                if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
                {
                    p[0xB3] = 2;
                    break;
                }
                if ((s16)func_800D5F80((u8 *)b, d) != 0)
                {
                    *(s16 *)(p + 0x96) = 0x1E;
                    p[0xB2] = 0;
                    p[0xB1] = 0;
                    p[0xB3] += 1;
                    break;
                }
                *(s16 *)(p + 0x96) = 3;
                p[0xB2] += 1;
                /* fallthrough */
            case 1:
                if ((*(s16 *)(p + 0x96))-- <= 0)
                {
                    p[0xB2] = 0;
                }
                tb = D_800E2468;
                t = tb[k];
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + t[0] * 16;
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + t[1] * 16;
                break;
            }
            break;
        }
    }
    if (p[0xB3] == 1)
    {
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            p[0xB3] = 2;
        }
        else if ((*(s16 *)(p + 0xA8))++ < 0x12C)
        {
            if (*(u8 **)(p + 0xAC) != NULL)
            {
                r = *(u8 **)(p + 0xAC) + 0x20;
                if ((*(s16 *)(r + 0x66) != 0 || *(s16 *)(r + 0x68) != 0) &&
                    *(s16 *)(r + 0x62) == *(s16 *)((u8 *)b + 0x2) &&
                    *(s16 *)(r + 0x64) == *(s16 *)((u8 *)b + 0x6))
                {
                    goto hit;
                }
                q = *(u8 **)(p + 0xAC);
                if (q != NULL)
                {
                    r = q + 0x20;
                    if (*(s16 *)(r + 0x66) != 0 || *(s16 *)(r + 0x68) != 0)
                    {
                        for (i = 2; i < 6; i++)
                        {
                            if (*(s16 *)(r + 0x66) == *(s16 *)(r + 0x62 + i * 4) &&
                                *(s16 *)(r + 0x68) == *(s16 *)(r + 0x64 + i * 4))
                            {
                                goto hit;
                            }
                        }
                    }
                }
            }
            switch (p[0xB1])
            {
            case 0:
            s0:
                if ((s16)func_800D5F80((u8 *)b, d) == 0)
                {
                    p[0xB1] = 2;
                    p[0xB2] = 0;
                    goto s2;
                }
                p[0xB1] = 1;
                p[0xB2] = 0;
                /* fallthrough */
            case 1:
                switch (p[0xB2])
                {
                case 0:
                s1a:
                    *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
                    if (*(s16 *)(d + 0x2A) >= 0x1000)
                    {
                        *(s16 *)(d + 0x2A) = 0;
                    }
                    *(s16 *)(p + 0x96) = 0;
                    p[0xB2] += 1;
                    break;
                case 1:
                    if ((*(s16 *)(p + 0x96))-- > 0)
                    {
                        break;
                    }
                    if ((*(u16 *)(d + 0x2A) & 0x3FF) != 0)
                    {
                        p[0xB2] = 0;
                        goto s1a;
                    }
                    p[0xB1] = 0;
                    p[0xB2] = 0;
                    goto s0;
                }
                break;
            case 2:
            s2:
                switch (p[0xB2])
                {
                case 0:
                    p[0xB2] += 1;
                    *(s16 *)(p + 0x96) = 3;
                    /* fallthrough */
                case 1:
                    tb2 = D_800E2468;
                    t2 = tb2[k];
                    *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + t2[0] * 16;
                    *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + t2[1] * 16;
                    if ((*(s16 *)(p + 0x96))-- > 0)
                    {
                        break;
                    }
                    if (*(u8 **)(p + 0xAC) != NULL)
                    {
                        r = *(u8 **)(p + 0xAC) + 0x20;
                        for (i = 5; i >= 2; i--)
                        {
                            dst = r + i * 4;
                            g = r + (i - 1) * 4;
                            *(s16 *)(dst + 0x62) = *(u16 *)(g + 0x62);
                            *(s16 *)(dst + 0x64) = *(u16 *)(g + 0x64);
                        }
                        *(s16 *)(r + 0x66) = *(u16 *)((u8 *)b + 0x2);
                        *(s16 *)(r + 0x68) = *(u16 *)((u8 *)b + 0x6);
                    }
                    ang = *(u16 *)(d + 0x2A);
                    *(s16 *)(d + 0x2A) = ang - 0x400;
                    if (*(s16 *)(d + 0x2A) < 0)
                    {
                        *(s16 *)(d + 0x2A) = ang + 0xC00;
                    }
                    if ((s16)func_800D5F80((u8 *)b, d) != 0)
                    {
                        p[0xB2] = 0;
                        p[0xB1] = 0;
                        *(s16 *)(d + 0x2A) = ang;
                    }
                    else
                    {
                        p[0xB1] = 3;
                        p[0xB2] = 0;
                        *(s16 *)(d + 0x2A) = ang;
                    }
                    break;
                }
                break;
            case 3:
                switch (p[0xB2])
                {
                case 0:
                s3a:
                    e = *(u16 *)(d + 0x2A);
                    *(s16 *)(d + 0x2A) = e - 0x200;
                    if (*(s16 *)(d + 0x2A) < 0)
                    {
                        *(s16 *)(d + 0x2A) = e + 0xE00;
                    }
                    *(s16 *)(p + 0x96) = 0;
                    p[0xB2] += 1;
                    break;
                case 1:
                    if ((*(s16 *)(p + 0x96))-- > 0)
                    {
                        break;
                    }
                    if ((*(u16 *)(d + 0x2A) & 0x3FF) != 0)
                    {
                        p[0xB2] = 0;
                        goto s3a;
                    }
                    p[0xB1] = 2;
                    p[0xB2] = 0;
                    goto s2;
                }
                break;
            }
        }
        else
        {
        hit:
            p[0xB3] = 3;
            p[0xB1] = 0;
            goto past2;
        }
    }
    if (p[0xB3] == 2)
    {
        *(u16 *)(d + 0x2A) &= 0xFC00;
        p[0x9B] += 1;
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0x92) = 0;
        p[0xB1] = 0;
        *(s16 *)(p + 0x96) = 0;
        *(s16 *)(p + 0xA2) = 0;
        func_8009D8A4();
        D_800E296C |= 0x800000;
        func_800A56E0(0x80F);
    }
past2:
    if (p[0xB3] == 3)
    {
        switch (p[0xB1])
        {
        case 0:
            *(s16 *)(p + 0x96) = 0xA;
            p[0xB1] += 1;
            /* fallthrough */
        case 1:
            if ((*(s16 *)(p + 0x96))-- > 0)
            {
                break;
            }
            p[0xB1] += 1;
            break;
        case 2:
            *(u16 *)(c + 0x14) |= 0x80;
            *(s16 *)(p + 0x96) = 0x14;
            p[0xB1] += 1;
            break;
        case 3:
            if ((*(s16 *)(p + 0x96))-- > 0)
            {
                break;
            }
            *(s16 *)(p + 0x96) = 0x14;
            p[0xB1] += 1;
            break;
        case 4:
            if ((*(s16 *)(p + 0x96))-- > 0)
            {
                break;
            }
            *(u16 *)(d + 0x2A) &= 0xFC00;
            p[0x9B] += 1;
            *(u16 *)(c + 0x14) |= 0x80;
            *(s16 *)(p + 0x92) = 0;
            p[0xB1] = 0;
            *(s16 *)(p + 0x96) = 0;
            *(s16 *)(p + 0xA2) = 0;
            func_8009D8A4();
            D_800E296C |= 0x800000;
            func_800A56E0(0x80F);
            break;
        }
    }
    *(s16 *)(d + 0x88) = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                       (s16)(*(u16 *)((u8 *)b + 0xA) - 0x20));
}
