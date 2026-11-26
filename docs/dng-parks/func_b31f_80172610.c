/* PARK: func_b31f_80172610 (ovl_dng_b31f) -- 152 words / 153 words /
   24 ALIGNED DIFFERENCES.  Agent h186, batch 35.  One-member family.
   OWNS NO JUMP TABLE.

   WHAT IT DOES: the per-frame publish for this bank's puppet actor.  Take
   the node at p + 0xAC, bail out if p[0x9A] is nonzero, hand its payload's
   +0x1C flags and c + 0xC to func_800A020C, call func_800478B8(c) once when
   bit 5 of those flags is clear and TWICE when the slot's table is
   D_b31f_80174C6C, copy the payload's +0x2A heading into p + 0x2A and
   re-dress c when it changed, recompute the facing index off
   D_80083160 + 0xC8 and re-dress again when p + 0x94 disagrees, then copy
   the node's slot +0x14/+0x1C/+0x1E/+0x12 into c, the node's mover's
   +0x2/+0x6/+0xA into the caller's, and D_800DCECC's entry times 4 into
   c + 0x6; finally raise the 0x8000 pair when the node's +0x1E carries it,
   and again, with p[0x9A] bumped and +0x96 cleared, when the payload's
   +0x9A has reached 0x13.

   xsibs offers the resident func_800D5594 at 0.800; dngtwin finds no carved
   sibling at any ratio down to 0.45.  This body was written straight off
   the listing.

   THE RESIDUE IS TWO MISSING PSEUDOS, both of them a SECOND REGISTER HOLDING
   A VALUE THE BODY ALREADY HAS, and ONE WORD short:
   1. `addiu $s0, $s5, 0x20' AND `addu $fp, $s0, $zero' -- the target keeps
      n + 0x20 in two registers, $s0 for the early +0x1C and +0x2A reads and
      $fp for the late `lbu 0x9A($fp)'.  That copy is the missing word.
   2. `andi $s0, $v0, 0x7' then `addu $v1, $s0, $zero' -- the facing index
      likewise in two, $s0 (callee-saved, for the `sh 0x94($s1)' AFTER the
      func_80047784 call) and $v1 (for the comparison and the table index).
   EVERY SPELLING THAT COULD PRODUCE EITHER WAS MEASURED AND ALL ARE FLAT:
   | q2 placement                                        | words | aligned |
   | `q2 = q' just before the +0x9A gate                 |  152  |   38    |
   | `q2 = n + 0x20' just before the +0x9A gate          |  152  |   38    |
   | `q2 = n + 0x20' right after the p[0x9A] return      |  152  |   38    |
   | `q2 = q' right after the p[0x9A] return             |  152  |   38    |
   | `q2 = n + 0x20' before the facing block             |  152  |   38    |
   | `q2 = q' before the facing block                    |  152  |   38    |
   This is the same mechanism recorded in this batch's
   docs/dng-parks/func_b31f_80171A10.c: cse's make_regs_eqv makes whichever
   partner has the LATER last use the canonical register, canon_reg then
   rewrites the other's uses to it, and global-alloc coalesces what is left.
   No plain-C copy or recomputation survives it.  WHAT I WOULD TRY NEXT: the
   permuter, or an `__asm__' barrier between the two use groups.

   THREE SHAPE FACTS MEASURED AND LOAD-BEARING, all kept below:
   | spelling                                            | words | aligned |
   | the full body as written below                      |  152  |   24    |
   | the +0x9A bump written AFTER the -0x2 raise in the
     0x13 arm (both loads are off $s1, so their order is
     readable and the bump comes first)                  |  152  |   37    |
   | the heading read NOT in a `j' local, so the index
     re-loads q + 0x2A after the store to p + 0x2A
     killed cse's entry                                  |  153  |   50    |
   | `i2' and `j' locals for both sides of the heading
     comparison, which is what puts p's `lh' first       |  152  |   24    |
   | without `i2' (q's load first)                       |  152  |   25    |

   DECLARATIONS
   extern s16 D_80083228;
   extern s8 D_800DCECC[];
   extern u8 D_b31f_80174C6C[];
   void func_800A020C(s32, u8 *);
   void func_800478B8(u8 *);
   void func_80047784(u8 *, s32, s32);
   END DECLARATIONS */
void func_b31f_80172610(u8 *p, UnkVec *b, u8 *c)
{
    u8 *n;
    u8 *q;
    u8 *q2;
    u8 *r;
    UnkVec *m;
    u8 *g;
    s32 k;
    s32 j;
    s32 i2;

    n = *(u8 **)(p + 0xAC);
    q = n + 0x20;
    r = *(u8 **)(n + 0xC);
    m = *(UnkVec **)(n + 0x8);
    if (p[0x9A] != 0)
    {
        return;
    }
    func_800A020C(*(s32 *)(q + 0x1C), c + 0xC);
    if (!(*(s32 *)(q + 0x1C) & 0x20))
    {
        func_800478B8(c);
    }
    if (*(u8 **)(r + 0x2C) == D_b31f_80174C6C)
    {
        func_800478B8(c);
        func_800478B8(c);
    }
    i2 = *(s16 *)(p + 0x2A);
    j = *(s16 *)(q + 0x2A);
    if (i2 != j)
    {
        *(s16 *)(p + 0x2A) = *(u16 *)(q + 0x2A);
        func_80047784(c, (*(u8 **)(c + 0x2C))[((D_80083228 + j + 0x100) >> 9) & 7], 0);
    }
    g = (u8 *)&D_80083160;
    k = ((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
    if (*(s16 *)(p + 0x94) != k)
    {
        func_80047784(c, (*(u8 **)(c + 0x2C))[k], 0);
        *(s16 *)(p + 0x94) = k;
    }
    *(s16 *)(c + 0x14) = *(u16 *)(r + 0x14);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)m + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)m + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)m + 0xA);
    *(s16 *)(c + 0x6) = *(s8 *)((((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7) + (s32)D_800DCECC) * 4;
    *(s16 *)(c + 0x1C) = *(u16 *)(r + 0x1C);
    *(s16 *)(c + 0x1E) = *(u16 *)(r + 0x1E);
    *(s16 *)(c + 0x12) = *(u16 *)(r + 0x12);
    if (*(u16 *)(n + 0x1E) & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    q2 = n + 0x20;
    if (q2[0x9A] == 0x13)
    {
        p[0x9A] += 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        *(s16 *)(p + 0x96) = 0;
        D_800814A0 |= 0x8000;
    }
}
