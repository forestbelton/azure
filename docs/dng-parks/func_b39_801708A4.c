/* PARK: func_b39_801708A4, csrc/ovl_dng_b39.c, agent-h198, 2026-11-25.

   TARGET 167 words.  BEST BUILT 167 words, 4 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py).  1 member, 167 words (dngfam.py).
   VERDICT: ALLOC, and the whole residue is ONE SHORT-LIVED TEMPORARY'S
   REGISTER.  The four rows are:

     target   beqz $v0, exit / sll $a2, $a1, 1   (the sll IN THE DELAY SLOT)
     here     beqz $v0, exit / addiu $a0, $a0, 0xC
     target   addu $v0, $a2, $a1
     here     sll $v0, $a1, 0x1 ; addu $v0, $v0, $a1

   Everything else, including both scan-loop registers ($a0 = the cursor,
   $a1 = the index) and the whole 0x112 node dress, is exact.

   THE MECHANISM, and it is worth more than this function.  The tail computes
   `i * 0xC' as `(i*2 + i) * 4'.  Here local-alloc's combine_regs TIES the
   `i*2' temporary to the `addu' that consumes it -- the temp dies in that
   insn, so the whole chain temp -> sum -> scaled -> address is one quantity
   in $v0.  $v0 is also the scan loop's byte temp and the exit branch's own
   input, so reorg REFUSES to put the `sll' in the branch's delay slot
   (resource_conflict_p: the delay insn may not set a register the branch
   reads) and fills the slot from the fall-through with `q += 0xC' instead.
   In the target the temp is $a2, nothing in the loop reads $a2, and reorg
   takes the `sll'.  So the one question is how to break that tie.
   combine_regs (local-alloc.c) declines only when the used pseudo has no
   quantity -- not block-local, or dying more than once -- and neither is
   arrangeable from the source spellings tried below.

   THE SAME RESIDUE IS IN func_b39_8017382C's park, at two sites, with the
   temp wanting $a1 there instead of $a2.  Both bodies contain the SAME
   0xC-stride scan loop over `*(u8 **)(c + 0x8)' followed by
   `*(u16 *)(base + i * 0xC + 0x6)', so this is ONE source idiom appearing
   twice and whoever finds it closes both.

   DONOR.  csrc/ovl_dng_b45.c's func_b45_80170854 (the 42-member
   dngcallee family at 8 of 11 symbols; also b43/b44/b38/b37/b36/b40/b41).
   Its body is this one's head verbatim.  Four deltas: `p[0x13] = 0x27'
   where b45 writes 45, this bank's D_80175258 and D_80175298 for its
   D_b45_80173C6C and D_80173C9C, `func_b39_80170B40' as the node's step,
   and a 45-WORD TAIL b45 has not got -- the +0x9A/+0x9C/+0x8C/+0xAE dress,
   the scan loop, and two MoveImage calls (func_800673A0 resolves to
   MoveImage in splat/common_syms.txt).

   MEASURED SPELLINGS -- words / aligned rows.

     | spelling                                                | words | rows |
     |---------------------------------------------------------|-------|------|
     | THE BODY BELOW                                          |  167  |   4  |
     | the +0x12 read inline at the +0xAE store                |  167  |   9  |
     | ... as `s32 v' read BEFORE the three w stores (ADOPTED)  |  167  |   4  |
     | ... with `i = 0' after that read instead of before it    |  167  |   4  |
     | the scan loop as a plain `while'                         |  170  |  10  |
     | the scan loop as `while' over `base[i * 0xC]'            |  170  |  10  |
     | the scan loop as `goto' over `base[i * 0xC]'             |  163  |  17  |
     | the cursor re-derived as `base + i * 0xC' in the body    |  171  |  17  |
     | `do { } while (0)' around the scan loop                  |  167  |   4  |
     | ... around the tail `t = ' statement                     |  168  |  29  |
     | ... around both                                          |  168  |  22  |
     | `s16 i' instead of `s32 i'                               |  169  |   7  |
     | `u32 i' instead of `s32 i'                               |  167  |   4  |
     | `u16 t' instead of `s32 t'                               |  167  |   4  |
     | the index as `(i * 2 + i) * 4' written out               |  167  |   4  |
     | ... as `(i + i + i) * 4'                                 |  167  |   4  |
     | ... with `j = i * 2' as its own local                    |  167  |   4  |
     | ... as `(i << 3) + (i << 2)'                             |  167  |  11  |
     | ... as `((u16 *)(base + i * 0xC))[3]'                    |  167  |   4  |
     | the base hoisted into a local for the tail read          |  167  |   4  |
     | the base AND the index folded into one `u8 *e'           |  167  |   6  |
     | `rect.y' used directly instead of the `t' local          |  167  |   4  |
     | ... and `MoveImage(&rect, 0, rect.y - 1)'                |  167  |   4  |
     | the `t = ' statement moved below `rect.x = 0'            |  167  |  12  |

   Eleven of those are FLAT at 4 rows across both axes the brief names --
   the set (locals, widths, casts) and the order -- which is why the
   diagnosis above comes from local-alloc.c and reorg rather than from more
   sweeping.

   WHY THE SCAN LOOP IS A `goto'.  Measured here as well as on
   func_b39_8017382C: a `while' costs 3 words on this body (170 against 167)
   because stmt.c's expand_end_loop rotates a loop that opens with a
   conditional exit and jump.c:661's duplicate_loop_exit_test then copies the
   test above it.  A `goto' back edge emits no NOTE_INSN_LOOP_BEG, so neither
   pass fires.  DEVICE, or macro residue.

   UNMEASURED:
     * anything that gives the `i*2' temporary two deaths or a live range
       crossing a block boundary, which is the only thing combine_regs
       declines on.
     * the MoveImage rect's four field stores in any order but the one
       below (x, y, w, h for the first call).
     * `rect.y -= 1' as `rect.y--' or `rect.y = rect.y - 1'.

   DECLARATIONS
   extern u8 D_80045340[];
   s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
   void func_b39_80170B40(u8 *p, UnkVec *b, u8 *c);
   UnkNode *func_8003FD64(s32, void *);
   extern u8 D_80083498[];
   END DECLARATIONS
*/
u8 *func_b39_801708A4(s16 a, u16 b, u16 c, s16 d)
{
    RECT rect;
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    u8 *q;
    s32 k;
    s32 i;
    s32 t;
    s32 v;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b39_80170B40;
        p[0x13] = 0x27;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80175258;
        k = a & 3;
        *((u8 *)r + 0x24) = b;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    *(u32 *)(p + 0x1C) |= 0x200;
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80175298;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        i = 0;
        v = *(u16 *)((u8 *)r + 0x12);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b39_80170F6C;
        *(s16 *)(w + 0xAE) = v;
        q = *(u8 **)((u8 *)r + 0x8);
    scan708:
        if ((q[0] & 0x20) != 0)
        {
            q += 0xC;
            i++;
            goto scan708;
        }
        t = *(u16 *)(*(u8 **)((u8 *)r + 0x8) + i * 0xC + 0x6) >> 6;
        rect.x = 0;
        rect.y = t;
        rect.w = 0x100;
        rect.h = 1;
        MoveImage(&rect, 0, t - 1);
        rect.w = 0x10;
        rect.x = 0x30;
        rect.y -= 1;
        do
        {
            MoveImage(&rect, rect.x - 0x30, rect.y);
            rect.x += 0x40;
        } while (rect.x < 0x100);
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}
