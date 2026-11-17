/* PARK: func_800AAF00, 176 of 176 words, 4 ALIGNED DIFFERENCES -- and all
   four are the PROVED-UNREACHABLE `|= 0x80000000` gate.
   ovl_dungeon.  j42 parked this at 21 on 2026-11-16; j57 took it to 4 the
   same day.  Every other opcode, immediate, displacement and register in the
   function now matches.

   THE RESIDUE IS THE CATALOGUED FLOOR.  docs/matching-levers.md, "A
   PROVED-UNREACHABLE RESIDUE: THE `|= 0x80000000` GATE'S `$v0`/`$v1` SWAP"
   (and "RECOGNISING THE..." after it) says to subtract exactly these four
   words from any residue before deciding whether a function can close:

       89  lw   $v1, 0x14($s0)      target  lw   $v0, 0x14($s0)
       91  bltz $v1, .L800AB094             bltz $v0, .L800AB094
       92  lui  $v0, 0x8000                 lui  $v1, 0x8000
       93  or   $v0, $v1, $v0               or   $v0, $v0, $v1

   Four more spellings of that gate were measured here and none beats 4:
   hoisting the `|` out of the inner `if` (`v = load; w = v | 0x80000000;
   if (v >= 0) store w;`) 175 words / 11; `__asm__("" : "=r"(v) : "0"(v))`
   inside the arm 177 / 11; `v |= 0x80000000;` in place through a local
   176 / 5; the two tests merged into one `&&` 176 / 4 (byte-identical to the
   body below, so the nesting is free).  Treat as permanent.

   NEW DECLARATIONS: NONE.  Every symbol and callee is already declared in
   csrc/ovl_dungeon.c or include/structs.h, including
   `s32 func_800A05A4(u8 *, s32, s32, s16, s32)` and `UnkDungeonName10`.
   func_800C77D0 rides its implicit declaration exactly as the three landed
   call sites at :3202, :3293 and :19349 do.

   ------------------------------------------- what took it from 21 to 4

   THREE LEVERS, all in the `(x & 0x4000) == 0` arm, and all three are needed.

   1. `*(volatile u16 *)` ON THE **FIRST** READ -- THE 0x4000 TEST -- NOT ON
      THE RE-READ.  The target reads `*(u16 *)(p + 0x46)` twice across the
      branch and cse commons them however they are spelled (that is
      docs/matching-levers.md's "-fcse-skip-blocks COMMONS TWO READS ACROSS A
      BRANCH ... SO A RE-READ NEEDS A DEVICE", written against func_800AD6FC).
      That entry puts the device on the SECOND read, which is right for
      func_800AD6FC and WRONG here, and the difference is worth stating:

        * volatile on the second read forbids scheduling AT THE RE-READ, so
          the `lui %hi(D_8006DE24)` cannot move into its load-delay slot and
          a `nop` appears -- two words for a one-word deficit;
        * volatile on the FIRST read keeps that read out of cse's table (so
          the second read is a fresh `lhu`) and puts the barrier where the
          TARGET ALREADY HAS AN UNFILLED DELAY SLOT, the `nop` at 0x800AB098.
          One word, which is exactly the deficit.

      **The rule: when the target's two reads have a filled delay slot at the
      re-read and an empty one at the first read, the volatile goes on the
      FIRST read.  Read the two delay slots before choosing the site.**
      Measured on the pointer-local body: volatile on the first read 176
      words / 16 aligned, on the second read 176 / 16 (identical here),
      neither 175 / 25, on the j42 body (symbol added to the index) 178 / 25.

   2. THE TABLE AS A POINTER LOCAL (func_800A35A4's lever, csrc:15817):
          tbl = (UnkDungeonName10 *)D_8006DE24;
          e = &tbl[i];
      which is what splits `lui %hi` / `addiu %lo` around the stride-0x14
      multiply.  The symbol added to the index puts both halves after it.

   3. `i` IS REUSED FOR THE TABLE INDEX IT LOADS -- 16 aligned rows:

          i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
          gp = p + i * 3;
          i = gp[8];            <- the SAME local, not a new one

      With a fresh local for the loaded byte, `i` is a one-set block-local
      pseudo, `combine_regs` ties it to the masked value and it stays in the
      lhu's register (`addiu $v0, $v0, -0x1`); the target has
      `addiu $a0, $v0, -0x1`.  A SECOND SET makes `i` fail
      local-alloc.c:472's `reg_n_deaths == 1`, so local-alloc skips it,
      `global_alloc` places it, it crosses no call, and `find_reg` hands it
      $a0 from the call-clobbered end -- which is also the register the
      target re-uses for the loaded byte, because they are now the same
      pseudo.  That one edit fixed ten of the twelve table rows.

      This is the allocation counterpart of "`adjust_priority`'s BIRTHING
      BOOST": there a second set is a COST, here it is the lever.  **When a
      residue is a register permutation inside ONE basic block and the target
      moves a value to a register the block has not used, ask whether the
      target's variable has two sets -- a two-set local leaves local-alloc
      entirely.**  Rejected on the way (all 176 words): `__asm__("" : "+r"(i))`
      after the subtraction 14; the same on `tbl` 177 / 23; on the loaded byte
      177 / 23; the mask as its own local 14; `(i << 1) + i` 14; `s16 i` 178;
      `tbl` hoisted above `i` 16; `e = tbl + ...` 16.

   4. `gp = p + i * 3;` AS ITS OWN POINTER LOCAL is the last row, worth 1.
      Inline -- `p[i * 3 + 8]`, `(p + i * 3)[8]`, `*(p + i * 3 + 8)`,
      `p[8 + i * 3]`, `p[3 * i + 8]`, `*(u8 *)(i * 3 + (s32)p + 8)`, all six
      measured at 5 -- the address PLUS comes out `addu $v0, $v0, $s0` with
      the index first, and the target has `addu $v0, $s0, $v0`.  Binding the
      sum to a pointer local is the only spelling of the seven that emits the
      operands in the source order.  `i *= 3;` as its own statement is 6.

   ------------------------------------------- j42's levers, all still load-bearing

   * `i = (... & 0x3FFF) - 1;` as its own statement (inlined, fold
     distributes the -1 through the `* 3` and the subscript becomes
     `p[k * 3 + 5]`).
   * `(*(s16 *)(p + 0x46) & 0x3FFF) >= 5` for the FIRST gate, not `u16`:
     through `u16` combine proves the masked value non-negative and the
     compare becomes `sltiu` where the target has `slti`.
   * `k = 0;` BEFORE `p[0x71] &= 0x7F;`, and `k = 1;` BEFORE the
     `(q + 0x98) & 0x3000` test rather than inside its arm.
   * the five-parameter signature with the fifth read from 0x48($sp), and the
     two views `a` ($s3) and `p = (u8 *)a` ($s0).

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_800AAF00
     python3 tools/decomp/ovlverify.py <unit copy> func_800AAF00
   -> 176 of 176 words, 4 differing masked, 4 aligned, all four the gate. */

void func_800AAF00(UnkDungeonActor *a, UnkVec *b, u8 *c, s32 t, void (*h)())
{
    u8 *p;
    u8 *g;
    u8 *g2;
    u8 *q;
    u8 *gp;
    UnkDungeonName10 *e;
    UnkDungeonName10 *tbl;
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
        if ((*(volatile u16 *)(p + 0x46) & 0x4000) == 0)
        {
            i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
            gp = p + i * 3;
            i = gp[8];
            tbl = (UnkDungeonName10 *)D_8006DE24;
            e = &tbl[i];
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
