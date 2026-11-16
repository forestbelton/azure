/* PARK: func_800B23F8, 81 of 81 words, 21 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  A 0x112 spawner: allocates a node,
   installs func_800B274C as its handler and func_800B2A60 as the payload's
   +0x8C step, and returns the payload or NULL.

   NEW DECLARATION NEEDED (func_800B274C is defined BELOW this function and
   its address is taken, so an implicit declaration will not do -- the unit's
   own comment at the func_800B2A60 block says as much):

       void func_800B274C(u8 *, UnkVec *, u8 *);

   Everything else it touches is already declared: func_8003FD64,
   func_8004491C, func_800B22F0, func_800B2A60, D_80083498, D_80045340.

   ------------------------------------------------------------------ residue

   A SINGLE REGISTER PERMUTATION, at exactly the right word count.  The target
   has `n` in $s0 and the payload pointer in $s1; this body has them the other
   way round, and the same swap runs through `a` ($s4 against $s3) and `c`
   ($s3 against $s4).  Nothing else differs: every opcode, immediate and
   displacement matches, and the only other row is WHERE the second pointer is
   created -- the target copies it into a callee-saved register at word 37,
   before the branches, while this body recomputes it in the late block.

   By the triage rule in docs/dungeon-sweep.md this is `.greg` work, not
   permuter food: the word counts match and what is left is register numbers.

   -------------------------------------------------------------- what paid

   79/49 -> 81/21, and the two words are both worth recording:

   * `*(s8 *)(p + 0x9C) = -1;` and NOT `p[0x9C] = -1;`.  Through a `u8 *`
     lvalue gcc folds the -1 to 0xFF, which is the same constant the
     neighbouring `p[0x9A] = 0xFF` already holds, so cse shares one register
     and the `addiu $v0, $zero, -0x1` disappears.  The signed lvalue keeps the
     two constants distinct.  Worth 1 word.
   * A SECOND POINTER LOCAL, recomputed rather than copied.  The target keeps
     two registers holding `n + 0x20`, one for the early stores and the return
     and one for the +0x9A/+0x9C/+0x8C group.  Written `w = p;` the copy is
     propagated away wherever it is placed -- I swept all ten statement
     boundaries in the body and every one of them measured 81 words and 34
     differing, with the single exception of the position after `k = a & 3;`
     (47).  Written `w = (u8 *)n + 0x20;` INSIDE the late block, cse cannot
     reach it across the if/else-if's labels, the address is rebuilt, and the
     body is 81 words and 21 differing.  Worth 1 word and 13 differences.

   Also measured, no better: `s32 b, c` (25); `s8 b, c` (21, byte-identical to
   the `u8` form); `s32 a` with an explicit `(s16)a` at the func_800B22F0 call
   (78 words, 30 -- the `s16` parameter's two pseudos are load-bearing here,
   as docs/matching-levers.md's entry on that says). */

u8 *func_800B23F8(s16 a, u8 b, u8 c, s16 d, s32 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    s32 k;
    u8 *w;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B274C;
        p[0x13] = 0x1E;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *((u8 *)r + 0x24) = b;
        k = a & 3;
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
        func_800B22F0(n, m, r, a);
        w = (u8 *)n + 0x20;
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_800B2A60;
        *(s32 *)((u8 *)r + 0x8) = e;
    }
    return p;
}
