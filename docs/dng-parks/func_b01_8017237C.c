/* PARK: func_b01_8017237C (ovl_dng_b01), 115 words.
   BEST: 115 words, 3 ALIGNED DIFFERENCES (aligndiff), 1 family member
   (dngfam.py), so landing it lands only itself.

   THE RESIDUE IS EXACTLY ONE MISSING REG-REG COPY AND ITS TWO CONSEQUENCES:

       X  82  82 801724C4  nop            vs  addu $s1, $s2, $zero
       R  86  86 801724D4  beqz s2,...    vs  beqz $s1, .Lb01_80172518
       R 103 103 80172518  move v0,s2     vs  addu $v0, $s1, $zero

   The target holds the return flag in TWO pseudos: $s2 is set 0 at entry and
   1 inside the 0x80 block and is what the `<= 0x80'-false path returns at
   8017245C; $s1 is a COPY of it made in the delay slot at 801724C4, and it
   is what the +0x9B gate tests and what .Lb01_80172518 returns. Every
   spelling of a second local that was tried is COALESCED away by regmove
   (the copy`s source dies in it and the destination has one set), so the
   body comes out with one pseudo, a `nop' in that delay slot and $s2 at
   both later sites.

   THE TWIN IS THE LANDED func_b26_80171D80 (csrc/ovl_dng_b26.c, 132 words,
   dngtwin 0.794) and its shape carries completely. Deltas from it, all read
   off the listing:
     * the 0x8000/0x17 arm and the D_b26_8017386C facing-table dress are
       ABSENT (this copy has only the 0x11 arm and no dress),
     * func_8009C93C is called UNCONDITIONALLY (b26 guards it with
       `if (p[0x9A] == 0x11)'),
     * there is an EXTRA `*(s16 *)(p + 0x96) = 0;' beside `p[0x9B] = 0',
     * the 0x8000/`& 8' gate occurs TWICE -- once before func_800A0134
       through `g', and again after `r = 1' reading D_80083462 as its own
       symbol (a fresh `lui'/`lhu' at 80172474, not `lhu 0x2($s3)'),
     * and the whole 0x11 install is wrapped in a gate on the return flag,
       which b26 does not have.

   THE ONE MECHANISM THIS PARK ESTABLISHES, measured and traced in the
   dumps: WITHOUT A cse BARRIER BETWEEN `r = 1' AND THE FLAG GATE, cse1
   DELETES THE GATE. With the gate written plainly the body is 113 words /
   14 aligned differences and `probe.c.cse' has already dropped both the
   copy and the `beqz' (they are present in `probe.c.jump'); cse reports
   `;; Processing block from 2 to 252' -- one extended block spanning both
   `r = 1' and the test -- because the last gate`s `jump_if_false' targets a
   ONE-USE label preceded by the BARRIER of the `fail:' body, so
   cse_end_of_basic_block`s follow-jumps arm (cse.c:8139) hops over the
   `fail:' CODE_LABEL and carries r == 1 into the test.

   A `do { } while (0)' around THE LAST GATE supplies the NOTE_INSN_LOOP_END
   that cse_end_of_basic_block breaks on (cse.c:8054), and that alone takes
   the body from 113 / 14 to 115 / 3. It is a device, not a spelling: the
   real source presumably has a construct there that is a cse boundary and
   also keeps the two flag pseudos apart, and those are probably the SAME
   fault -- which is why neither half has been found.

   EVERY SPELLING MEASURED, with its score:

   | spelling                                              | words | aligned |
   | as below (do-while round the last gate)               |  115  |    3    |
   | plain gate, no device                                 |  113  |   14    |
   | plain `return r' instead of b26's do-while return     |  115  |    3    |
   | do-while round func_800C7930                          |  113  |   14    |
   | do-while round the p+0x9B / p+0x96 stores             |  113  |   20    |
   | `r = 1' moved INSIDE a nested 0x8000 gate (no device) |  114  |   13    |
   | `r = 1' before a NESTED 0x8000 gate                   |  109  |   37    |
   | second local `q = r', gate+return on q, s32           |  115  |    3    |
   | the same with `s16 q' / `u8 q' / `u32 q'              |  115  |    3    |
   | `q = r' plus `return q' inside, `return r' outside    |  112  |   24    |
   | `q = r' right after `r = 1' (two returns)             |  112  |   24    |
   | `do { q = r; } while (0)'                             |  115  |    3    |
   | the dead `t' reused as the flag, `t = (u8 *)r'        |  115  |    3    |
   | the same with `return (s32)t' inside                  |  112  |   24    |
   | gate written `if (r > 0)'                             |  115  |    3    |
   | inner `if (q == 0) return q;' then the block flat     |  114  |   32    |

   The three 112/24 rows all drop a callee-saved register (six saves against
   the target`s seven) because the inner `return q' lets regmove tie the two
   flags; that is the signature to avoid.

   NEXT THING TO TRY: a construct between `r = 1' and the gate that is both a
   cse boundary AND forces the two flag allocnos to conflict -- the
   matching-levers entry "A REG-REG COPY SURVIVES WHEN ITS TWO ENDS ARE READ
   IN DIFFERENT ARMS OF AN if/else" is the shape to aim for, with the copy`s
   SOURCE read in the fall-through arm. Nothing tried here reads $s2 after
   801724C4, so no conflict was ever created.

DECLARATIONS
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
void func_b01_80170DA8();
s16 func_800A2CB8(u8 *, u8 *);
END DECLARATIONS
*/
s32 func_b01_8017237C(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    u8 *t;
    s32 r;

    r = 0;
    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
    {
        goto fail;
    }
    t = func_800A04F0_r(d, ((u8 *)c)[0x24], ((u8 *)c)[0x25], *(s16 *)(d + 0x2A));
    if ((s16)func_800A2CB8(d, t) != 0)
    {
        if ((*(u16 *)(g + 0x2) & 0x2000) != 0)
        {
            goto fail;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (*(u16 *)(g + 0x2) & 0x8) != 0)
        {
            goto fail;
        }
        if ((u16)(0x40 + -func_800A0134(t, d)) <= 0x80)
        {
            r = 1;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 0x8) != 0)
            {
                goto fail;
            }
            if ((s16)func_800A2B5C(d) != 0)
            {
                goto fail;
            }
            func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
            do
            {
                if ((s16)func_800A2B5C(d) != 0)
                {
                fail:
                    return -1;
                }
            } while (0);
            p[0x9B] = 0;
            *(s16 *)(p + 0x96) = 0;
            if (r != 0)
            {
                *(s32 *)(p + 0x8C) = 0;
                p[0x9A] = 0x11;
                d[0x84] = 0x7C;
                d[0x85] = 0;
                d[0x6D] -= 1;
                func_8009C93C(d, (u8 *)c, *(s16 *)(d + 0x2A), 1, 0);
            }
        }
        do
        {
            return r;
        } while (0);
    }
    return 0;
}