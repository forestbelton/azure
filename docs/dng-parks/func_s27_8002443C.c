/* PARK: func_s27_8002443C, csrc/ovl_dng_s27.c, agent-h196, 2026-11-25.

   TARGET 117 words. BEST BUILT 117 words, 2 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), 3 differing by ovlverify. 1 member, 117 words
   (dngfam.py), so no replication value.

   THE RESIDUE IS A THREE-INSTRUCTION ROTATION IN ONE BASIC BLOCK AND NOTHING
   ELSE. Every other word of the 117 is exact, including the %hi/%lo pair, the
   register assignment and the frame. The block is the head of the non-NULL
   arm:

     target     15 lui   $v0, %hi(func_s27_800242E0)   (beqz delay slot)
                16 addiu $v0, $v0, %lo(func_s27_800242E0)
                17 sw    $s1, 0x20($s3)
                18 addiu $s1, $s3, 0x20
                19 sw    $v0, 0x10($s3)

     built      15 lui   $v0, %hi(...)                 (beqz delay slot)
                16 sw    $s1, 0x20($s3)
                17 addiu $s1, $s3, 0x20
                18 addiu $v0, $v0, %lo(...)
                19 sw    $v0, 0x10($s3)

   So the `addiu' that completes the symbol address is scheduled LAST here and
   FIRST in the target; the set of instructions is identical. This is a sched
   priority/tie-break question, not a shape question.

   WHAT MADE THIS BODY, measured on this body:

   * THE PAYLOAD CURSOR MUST REUSE THE PARAMETER `v'. A separate `u8 *w'
     local is 120 words / 31 aligned rows -- it takes a SIXTH callee-saved
     register (frame 0x30 against the target's 0x28) because with two
     pseudos there is no anti-dependence forcing `sw $s1, 0x20($s3)' ahead of
     `addiu $s1', sched interleaves them and their live ranges overlap. With
     `v' reused the anti-dependence exists and global.c shares $s1.
     Variants measured: `w' with the step store first 118 w / 103; `w' third
     118 w / 103.

   * `return n' INSIDE THE then-ARM AND `return NULL' AFTER THE GATE.
     This is the shape; the other two cost:
       `if (n != NULL) { ...; return n; } return NULL;'  117 w,  2 rows  <-- kept
       `if (n != NULL) { ... } else { return NULL; } return n;'  118 w,  9 rows
       `if (n == NULL) { return NULL; } ...; return n;'   117 w,  6 rows
     The else-arm form is the landed func_s27_800241F8's shape in this same
     unit, so the fact INVERTS between two bodies 0x244 apart.

   * `func_8003DB94(m, D_800DEC70, (s16)(a % 16))' -- the third argument is
     sign-extended at the call (`sll 16' / `sra 16' after the `subu'), which
     with the unit's `s32' third parameter has to be an explicit cast. The
     same code comes from declaring that parameter `s16'; this body cannot
     tell them apart and the cast was preferred because it leaves the two
     landed callers of func_8003DB94 in this unit untouched.

   SPELLINGS MEASURED AND REJECTED, all at 117 words unless stated:
     statement order unk_10 / +0x20 / cursor, all 6 permutations      3
     `*(u32 *)((u8 *)n + 0x10) = (u32)func_s27_800242E0'              3
     `u32 fptr = (u32)func_s27_800242E0; n->unk_10 = fptr;'           3
     `if (1) { ... }' round the +0x20 store                           3
     `do { } while (0)' round the +0x20 store / the cursor / both    19
     `do { } while (0)' round the unk_10 store                       18/19
     cursor store before the +0x4 store, unk_10 last                  5
     the rand()%5 store before the k store                           17
     `k' as s32                                                      11
     `k' as u16                                                       3
     `a' as s32 with `(s16)((s16)a % 16)'                             3
     `v' as UnkNode * throughout                                      3
   UNMEASURED: anything that changes the number of RTL insns in the arm (the
   sched priority is a critical-path length, so an extra or absent insn in
   this block is the lever that was not tried); and -da on .sched/.sched2,
   which is where the tie-break actually is.

   DECLARATIONS
   void func_s27_800242E0(u8 *p, UnkVec *b, u8 *c, u8 *d);
   s32 func_800644B8(s32);
   s32 func_80064584(s32);
   END DECLARATIONS
*/
UnkNode *func_s27_8002443C(u8 *v, UnkVec *q, s16 k, s16 a)
{
    UnkNode *n;
    u8 *m;
    u8 *m2;
    s32 r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = v;
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s27_800242E0;
        *(s16 *)(v + 0x4) = k;
        *(s16 *)(v + 0x6) = -6 - rand() % 5;
        *(s16 *)(v + 0x8) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(v + 0xA) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(v + 0xC) = *(u16 *)((u8 *)q + 0xA);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0;
        m[0xD] = 0;
        m[0xC] = 0;
        *(s16 *)(m + 0x12) = 0x7E09;
        *(u16 *)(m + 0x14) |= 0xC;
        *(u16 *)(m + 0x10) |= 0x20;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_800DEC70, (s16)(a % 16));
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        func_8004491C(n, (u32)D_80045340);
        m2 = (u8 *)n->unk_08;
        r = rand() % 4096;
        *(s16 *)(m2 + 0x2) = *(u16 *)(v + 0x8) + ((func_800644B8(r) << 1) >> 8);
        *(s16 *)(m2 + 0x6) = *(u16 *)(v + 0xA) + ((func_80064584(r) << 1) >> 8);
        *(s16 *)(m2 + 0xA) = *(u16 *)(v + 0xC);
        return n;
    }
    return NULL;
}
