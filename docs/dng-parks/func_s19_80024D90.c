/* func_s19_80024D90, ovl_dng_s19. PARKED at 84 words / 84 words /
   7 aligned differences (7 by ovlverify too -- nothing moves, so the two
   agree). dngfam: 1 member, 84 words, no other copy and no other park.

   WHAT IT IS: the sixteen-spoke spawner for the ovl_dng_s19 ring effect.
   Sixteen 0x202 nodes off func_8003FC64, each driven by func_s19_80024B98
   with func_80045C34 through func_8004491C, placed on a circle of radius
   (func_80064584/func_800644B8 of i << 8) >> 7 around the caller's mover and
   0x100 below it, dressed from D_80025854 with 0x800/0x1CCC scales, a 0x400
   at +0x16 and a per-spoke -0x400 + i * 0x100 at +0x18, and given the spoke
   index at payload +0x24, the caller's s16 at +0x14 and i * 2 at +0x1E.
   Each successful spawn bumps this unit's own D_s19_800257CC. Returns the
   LAST node allocated (NULL if the sixteenth allocation failed).

   NO TWIN ANYWHERE. dngtwin --unit ovl_dng_s19 reports no carved sibling at
   0.70 or better; xsibs (--min-words 18 --ratio 0.70 --top 200) lists five
   of the unit's twelve stubs and not this one. The callee-set grep
   (func_80045C34 + func_80064584 + func_800644B8) intersects on
   csrc/ovl_dungeon_800C2824.c, csrc/ovl_dungeon_800B6AC4.c and
   csrc/ovl_dungeon_800CABA8.c; the closest body there is func_800C7380
   (csrc/ovl_dungeon_800C2824.c:2762), same allocate/install/dress/payload
   skeleton with an inner do-while, and its plain-cast node-field idiom was
   tried here and is FREE (see the table). Written from the listing.

   THE RESIDUE IS A TWO-REGISTER EXCHANGE WITH THE INSTRUCTION ORDER EXACT.
   All 84 words are in the right positions and 77 of 84 are byte-identical.
   The seven rows are the whole of it:

     row  built                              target
      56  lhu   $a0, %lo(D_257CC)($s6)       lhu   $v1, %lo(D_257CC)($s6)
      57  addiu $v1, $s2, 0x20               addiu $a0, $s2, 0x20
      61  sh    $s3, 0x24($v1)               sh    $s3, 0x24($a0)
      62  sh    $s7, 0x14($v1)               sh    $s7, 0x14($a0)
      63  sh    $v0, 0x1E($v1)               sh    $v0, 0x1E($a0)
      64  addiu $a0, $a0, 0x1                addiu $v1, $v1, 0x1
      65  sh    $a0, %lo(D_257CC)($s6)       sh    $v1, %lo(D_257CC)($s6)

   i.e. the counter value wants $v1 and the payload pointer $a0, and this
   body gives them the other way round. Both are block-local quantities in
   one basic block (the whole if-body plus the loop test is a single block
   under gcc 2.7.2, since a CALL_INSN does not end one), so this is
   local-alloc, not global-alloc, and local-alloc.c:1693 QTY_CMP_PRI is
   floor_log2(n_refs) * n_refs * size / (death - birth) with ties broken by
   qty NUMBER. The payload pointer has 4 references over 6 insns (1.333) and
   the counter 2 over 9 (0.25) -- or 4 over 9 (0.889) when written as one
   `D_s19_800257CC++' -- so the pointer sorts first on every spelling tried
   and takes $v1, the earlier register in the default allocation order
   (mips.h defines no REG_ALLOC_ORDER). For the target to put the counter in
   $v1 the counter qty has to sort FIRST, which needs either nine references
   on it or fewer than four on the pointer; neither is reachable from any
   source shape found. -da .lreg confirms the two pseudos: `Register 86 in 3'
   (the pointer, $v1) and `Register 88 in 4' (the counter, $a0).

   WHAT THE SPLIT COUNTER BUYS, and why it is kept. The counter MUST be read
   early and stored last. Written as the single statement
   `D_s19_800257CC++' in any of the twelve possible positions in the if-body
   the best is 10 aligned differences and the `lhu' lands two insns before
   its own `addiu' with a `nop' between -- 85 words, one too many. Split into
   `cc = D_s19_800257CC;' before the +0x16 store and
   `D_s19_800257CC = cc + 1;' as the last statement, the schedule is exact
   at 84 words and only the registers are left. That split is the one
   DEVICE-shaped thing in this body; everything else is plain C.

   EVERY SPELLING MEASURED, aligned differences at 84 words unless noted:

   | spelling                                                    | w  | rows |
   |-------------------------------------------------------------|----|------|
   | `cc' read before the +0x16 store, stored last      KEPT     | 84 |   7  |
   |   ... read before `r = n->unk_0C'                           | 84 |  11  |
   |   ... read before `*(s16 *)(r + 0x1C) = 0x800'              | 84 |  11  |
   |   ... read before the func_8003DB94 call                    | 84 |  15  |
   |   ... read before the +0x18 store                           | 84 |   8  |
   |   ... read before `q = (u8 *)n + 0x20'                      | 84 |  10  |
   | one `D_s19_800257CC++', before the +0x18 store (best)       | 84 |  10  |
   | one `D_s19_800257CC++', last statement                      | 85 |  11  |
   | one `D_s19_800257CC++', before the +0x16 store              | 84 |  11  |
   | one `D_s19_800257CC++', before the func_8003DB94 call       | 84 |  15  |
   | one `D_s19_800257CC++', before the +0x14 payload store      | 85 |  13  |
   | the +0x18 value as a giv, `i * 0x100 - 0x400'      KEPT     | 84 |   7  |
   |   ... as an explicit `j = -0x400; ... j += 0x100;' counter  | 84 |  15  |
   | `cc' declared `s16'                                KEPT     | 84 |   7  |
   |   ... `u16'                                                 | 84 |   7  |
   |   ... `s32' -- gives `lh' for the target's `lhu'             | 84 |   7  |
   | the counter store spelled `D_s19_800257CC = cc + 1'  KEPT   | 84 |   7  |
   |   ... `cc = cc + 1; D_s19_800257CC = cc;'                   | 84 |   7  |
   |   ... wrapped in `do { } while (0)'                         | 84 |   7  |
   | `q = (u8 *)n + 0x20'                               KEPT     | 84 |   7  |
   |   ... `q = (u8 *)(n + 1)'                                   | 84 |   7  |
   |   ... no `q' at all, three `(u8 *)n + 0x44/0x34/0x3E' sites | 83 |   7  |
   | `q' declared after `r', before `i'                 KEPT     | 84 |   7  |
   |   ... declared after `cc'                                   | 84 |   7  |
   |   ... `cc' declared first of all                            | 84 |   7  |
   | node fields as `n->unk_10' / `n->unk_08' / `n->unk_0C' KEPT | 84 |   7  |
   |   ... all three as plain casts, func_800C7380's idiom       | 84 |   7  |
   | payload stores written 0x24, 0x14, 0x1E            KEPT     | 84 |   7  |
   |   ... 0x14, 0x24, 0x1E                                      | 84 |   8  |
   | the counter store after all three payload stores   KEPT     | 84 |   7  |
   |   ... between the +0x14 and +0x1E stores                    | 84 |   8  |
   | `i * 2' at +0x1E                                   KEPT     | 84 |   7  |
   |   ... `i << 1'                                              | 84 |   7  |
   | statement order CC, +0x16, +0x18, payload, store   KEPT     | 84 |   7  |
   |   ... CC, payload, +0x16, +0x18, store                      | 84 |  12  |
   |   ... CC, +0x18, +0x16, payload, store                      | 84 |   7  |
   |   ... +0x16, CC, payload, +0x18, store                      | 84 |  10  |
   |   ... CC, +0x16, payload, +0x18, store                      | 84 |   9  |

   The last five rows are the evidence that the SCHEDULE is already settled:
   three of the eleven free-looking spellings reproduce the identical seven
   rows bit for bit, which per docs/matching-levers.md is one fact, not
   eleven.

   WHAT WOULD SETTLE IT: a permuter run on this body alone. The shape is
   right, the schedule is right, and the search space is one local-alloc
   priority. I am not allowed to run it.

   TWO FACTS THIS BODY ESTABLISHES FOR THE UNIT, both re-measured here:
   the +0x18 per-spoke value is a loop.c giv and NOT a source counter (an
   explicit counter is 15 rows, and the tell is the PROLOGUE: a source
   counter's `addiu $s4, $zero, -0x400' is born before loop.c's hoisted
   `%hi(D_s19_800257CC)' and `%lo(func_s19_80024B98)', where the target has
   it after -- givs are initialised in the preheader, after the hoists); and
   `i << 8' is written inline at BOTH func_80064584 and func_800644B8 rather
   than carried in a local, which is what keeps it in $s0 across the first
   call.

   DECLARATIONS
   extern u8 D_80025854[];
   extern s16 D_s19_800257CC;
   s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
   void func_8003DB94(void *, void *, s32);
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   void func_s19_80024B98(u8 *, UnkVec *, u8 *);
   UnkNode *func_8003FC64(s32);
   s32 func_8004491C(UnkNode *, u32);
   END DECLARATIONS
*/
UnkNode *func_s19_80024D90(UnkVec *v, s16 a)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s32 i;
    s16 cc;

    for (i = 0; i < 0x10; i++)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s19_80024B98;
            func_8004491C(n, (u32)func_80045C34);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)v + 0x2) + (func_80064584(i << 8) >> 7);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)v + 0x6) + (func_800644B8(i << 8) >> 7);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)v + 0xA) - 0x100;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1C) = 0x800;
            *(s16 *)(r + 0x1E) = 0x1CCC;
            *(u32 *)(r + 0xC) = 0;
            func_8003DB94(r, D_80025854, 0);
            cc = D_s19_800257CC;
            *(s16 *)(r + 0x16) = 0x400;
            *(s16 *)(r + 0x18) = i * 0x100 - 0x400;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x24) = i;
            *(s16 *)(q + 0x14) = a;
            *(s16 *)(q + 0x1E) = i * 2;
            D_s19_800257CC = cc + 1;
        }
    }
    return n;
}
