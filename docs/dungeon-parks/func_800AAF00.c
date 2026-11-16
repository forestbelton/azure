/* func_800AAF00 (ovl_dungeon) -- 176 of 176 words, 21 ALIGNED DIFFERENCES.
   Agent j42, 2026-11-16.

   FOUR of the 21 are the PROVED-UNREACHABLE `|= 0x80000000` gate
   (docs/dungeon-sweep.md, "A PROVED-UNREACHABLE RESIDUE EXISTS IN THIS
   BLOCK"): at 0x800AB064..0x800AB074 the candidate loads into $v1 and builds
   the mask in $v0 where the target does the reverse.  Nothing was spent on
   those four.

   THE REAL RESIDUE IS SEVENTEEN WORDS AND IT IS ONE MISSING LOAD.
   At 0x800AB0A8 the target RE-READS `*(u16 *)(p + 0x46)`:

     801AB094  lhu  $v0, 0x46($s0)      <- the & 0x4000 test
     800AB09C  andi $v0, $v0, 0x4000
     800AB0A0  bnez $v0, .L800AB130
     800AB0A4  addiu $v0, $zero, 0x12   <- clobbers $v0 in the delay slot,
     800AB0A8  lhu  $v0, 0x46($s0)      <- so this load really is a re-read
     800AB0AC  lui  $v1, %hi(D_8006DE24)
     800AB0B0  andi $v0, $v0, 0x3FFF

   Every spelling reachable from C commons the two reads (cse crosses the
   conditional branch here), so the candidate carries ONE `lhu` whose value
   lives in $v1 across the branch, and that one missing word pulls the whole
   0x800AB094..0x800AB0D8 block out of register-step with the target: the
   D_8006DE24 `lui`/`addiu` pair lands AFTER the stride-0x14 multiply instead
   of straddling it, the load-delay `nop` at 0x800AB0C8 survives where the
   target fills it with the `addiu %lo`, and ten more words differ only in
   which of $v0/$v1/$a0 they use.  The word COUNT still closes at 176 because
   the target's extra `lhu` is paid for by the `nop` the candidate keeps.

   MEASURED AND REJECTED FOR THE RE-READ (all at 176 words unless noted):
     `((u16 *)p)[0x23]` for either read, or both       identical md5
     `*(u16 *)&p[0x46]`, `((u16 *)(p + 0x46))[0]`       identical md5
     `*(s16 *)` for either read, or both (8 combos)     identical md5
     `!(x & 0x4000)`, `(x & 0x4000) == 0 && 1`          identical md5
     `*(volatile u16 *)` on either read                 178 words, 35 diff
     reading the test through `(u8 *)a` (the other
       pointer view) -- DOES break the cse, but the
       two views then need two registers              178 words, 41 diff
     a `do { } while (0)` around the whole arm          175 words, 46 diff
     duplicating `a->unk_9A = 0x12` into both arms      178 words, 51 diff
     splitting the mask and the -1 into two statements  178 words, 35 diff
   and for the TABLE POINTER (which decides where the `lui` lands; see
   func_800A35A4's comment at csrc/ovl_dungeon.c:14683):
     `tbl = (UnkDungeonName10 *)D_8006DE24; e = &tbl[...]`   175 words, 21
     the same with `p += i * 3;` first                       175 words, 21(30
                                                             before the other
                                                             levers landed)
     the symbol ADDED to the index (the body below)          176 words, 21
   The pointer-local form is the target's shape and fills the `nop`, but it
   is a word SHORT -- it needs the re-read to pay for it.  Combining the two
   (pointer local + volatile) is 177 words and 19 aligned, which is the
   closest shape seen and the right lead for a permuter run: the answer is a
   non-volatile device that makes gcc emit the second `lhu`.

   WHAT GOT IT FROM 37 TO 21, and these DID generalise:

   * `i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;` AS ITS OWN STATEMENT.  Inlined
     as `p[((... & 0x3FFF) - 1) * 3 + 8]`, fold distributes the `-1` through
     the `* 3` and the subscript becomes `p[k * 3 + 5]` -- one word short at
     175 and 37 aligned.  With `i` a variable there is no constant to
     distribute and the target's `addiu $a0, $v0, -0x1` survives.
   * `(*(s16 *)(p + 0x46) & 0x3FFF) >= 5` for the FIRST gate, not `u16`.
     Through `u16` combine proves the masked value non-negative and the
     compare becomes `sltiu`; the target has `slti`.  (The signedness of the
     OTHER two reads of the same field changes nothing -- 8 combinations
     measured, one md5.)  Worth 1.
   * `k = 0;` BEFORE `p[0x71] &= 0x7F;`, not after the D_80083460 gate.
     Written after, sched1 sinks the `addu $s1, $zero, $zero` past the
     parameter copies, reorg then fills the gate's delay slot with
     `addu $s5, $a3, $zero` instead, and the whole prologue comes out in a
     different order -- 32 aligned against 23.  Seven of the nine rows it
     fixed were `sw $sN` saves in the prologue.
   * `k = 1;` BEFORE the `(q + 0x98) & 0x3000` test, not inside its arm.
     Inside the arm it is in the same basic block as the `lhu 0xA6` that
     follows, sched1 sinks it below that load, and reorg can then only put it
     in the SECOND branch's delay slot (the first gets a `nop`); the target
     has it in the first branch's slot and the `nop` in the second.  Setting
     it before the test puts it in the block that ENDS at that branch, where
     sched1 cannot move it.  Worth 2.  It is dead on the arm that returns, so
     the early write changes nothing observable.

   WHAT THE CANDIDATE HOLDS FIXED: the five-parameter signature
   `(UnkDungeonActor *a, UnkVec *b, u8 *c, s32 t, void (*h)())` with the fifth
   read from its home slot at 0x48($sp); the two views `a` ($s3, for +0x8C,
   +0x9A, +0x9B) and `p = (u8 *)a` ($s0, for everything else) -- taking `p` as
   the parameter and deriving `a` is 46 aligned; `s32 t` with `((u8 *)t)[...]`
   (a `u8 *t` is byte-identical); `s16 k` and `u8 k` are both 177 words;
   `UnkDungeonName10` as the 0x14-byte record face, which is the only one of
   the unit's three D_8006DE24 typedefs that names both +0x12 and +0x13.

   NEW DECLARATIONS: NONE.  Every symbol and callee this body names is already
   declared in csrc/ovl_dungeon.c or include/structs.h, including
   `s32 func_800A05A4(u8 *, s32, s32, s16, s32)` at :1483 (five parameters,
   which is what the `sw $v0, 0x10($sp)` fifth argument needs) and
   `UnkDungeonName10` at :557.  func_800C77D0 rides its implicit declaration
   exactly as the three landed call sites at :3202, :3293 and :19349 do.

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_800AAF00
     python3 tools/decomp/ovlverify.py <unit copy> func_800AAF00
   -> 176 of 176 words, 19 differing masked, 21 aligned (4 of them the gate). */

void func_800AAF00(UnkDungeonActor *a, UnkVec *b, u8 *c, s32 t, void (*h)())
{
    u8 *p;
    u8 *g;
    u8 *g2;
    u8 *q;
    UnkDungeonName10 *e;
    u16 f;
    s32 k;
    s32 i;

    p = (u8 *)a;
    k = 0;
    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    f = *(u16 *)(g + 0x2);
    if ((f & 0x2008) != 0)
    {
        return;
    }
    if ((*(u32 *)(p + 0x1C) & 0x2000) != 0 && (*(s16 *)(p + 0x46) & 0x3FFF) >= 5)
    {
        q = D_800E3D7C;
        k = 1;
        if ((*(u32 *)(q + 0x98) & 0x3000) == 0x2000)
        {
            if (*(u16 *)(q + 0xA6) == 0)
            {
                *(u16 *)(q + 0x98) |= 0x1000;
                *(u16 *)(q + 0xA6) += 1;
                *(u16 *)(g + 0xA) += 1;
                goto join;
            }
        }
        func_800A4ACC(p);
        p[0x6D] -= 1;
        a->unk_8C = h;
        return;
    }
    if ((f & 8) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
    func_800C77D0(p - 0x20, b, 8, 0x300);
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
join:
    if (k == 0)
    {
        if ((*(u32 *)(p + 0x1C) & 0x400) != 0)
        {
            if (*(s32 *)(p + 0x14) >= 0)
            {
                *(u32 *)(p + 0x14) |= 0x80000000;
                *(u16 *)(p + 0x2A) += (func_800A6D30() & 7) << 9;
            }
        }
        if ((*(u16 *)(p + 0x46) & 0x4000) == 0)
        {
            i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
            e = (UnkDungeonName10 *)(p[i * 3 + 8] * 0x14 + (u8 *)D_8006DE24);
            if (e->unk_12 == 2 && *(s32 *)(p + 0x60) == 0)
            {
                *(s32 *)(p + 0x60) =
                    func_800A05A4(p, c[0x24], c[0x25], *(s16 *)(p + 0x2A), e->unk_13);
            }
        }
    }
    else
    {
        *(u8 **)(D_800E3D7C + 0x11C) = p;
    }
    a->unk_9A = 0x12;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    if (t != 0)
    {
        *(s32 *)(c + 0x2C) = t;
        func_80047784(c, ((u8 *)t)[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u16 *)(c + 0x14) |= 0x800;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0xA) += 1;
}
