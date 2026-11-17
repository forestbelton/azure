/* PARK: func_800AD6FC, 174 of 174 words, 31 aligned differences.
   ovl_dungeon.  Measured 2026-11-16 (j34/j42); re-measured unchanged by j57
   the same day, who DIAGNOSED the residue exactly and could not close it.
   Builds one of four message strings for a record, adds a score to its +0x24
   counter and clamps it.

   SIX NEW DECLARATIONS, all message/rodata blobs the unit does not yet name
   (insert beside the existing `extern u8 D_800E0683[];`):

       extern u8 D_800E0CAE[];
       extern u8 D_800E0CDC[];
       extern u8 D_800E0CF3[];
       extern u8 D_800E0D1B[];
       extern u8 D_800E0D36[];
       extern u8 D_80089084[];

   D_80089084 is this unit's own rodata (there is an INCLUDE_RODATA for it),
   the other five are out-of-segment externs.

   The prototype `s32 func_800AD6FC(u8 *, s32, s32);` is pinned by eight
   landed call sites, all of which pass 0 as the third argument -- but the
   body INDEXES that argument (`lbu $v1, 0x1($s4)`), so it is a record
   pointer declared `s32`.  Hence the `p = (u8 *)arg;` at the top; do not
   widen the declaration.

   The tail -- the 0x10000 clamp, the `c[0x25]` / `c+0x1C & ~8` pair and the
   `c[0x66] < c[0x25]` swap -- is the same block j34 decoded in
   func_800ADE74.  The three func_80099194 / func_80099734 chains are written
   UN-NESTED, the shape csrc/ovl_dungeon.c:3841 already landed.

   ============================================== THE RESIDUE IS ONE SWAP

   Twenty-five of the 31 rows are a single `$s2`/`$s4` exchange between
   `mode` and `p`.  Every opcode, immediate and displacement in those rows
   already matches.  Read out of `-da` (`tools/cdkgcc -O2 -G8 -da -S` on the
   declaration block + this body; see docs/matching-levers.md, "READ
   `allocno_compare`'s SORT OUT OF `-da`"), the eight global allocnos are

     pseudo  role   refs/live   priority   gets   TARGET WANTS
       85     h      26/ 66      1.576     $s0      $s0  ok
       86     t      14/ 78      0.538     $s1      $s1  ok
       83     p      10/ 89      0.337     $s2      $s4  <-- swap
       80     c      16/240      0.267     $s3      $s3  ok
       81     mode   10/116      0.259     $s4      $s2  <-- swap
       84     m       5/106      0.094     $s5      $s5  ok
       87     t0      2/ 70      0.029     $s6      $s6  ok

   `c` is ALREADY in the right place, so the target's order is
   `mode > c > p` and this body's is `p > c > mode`: the two NEIGHBOURS OF
   `c` have to exchange places around a fixed middle, which is much harder
   than a plain adjacent swap.  The arithmetic, with `c` at 0.2667:

     * `mode` must rise above 0.2667 -- only +3.1%.  Either 11 references
       (0.2845) or a live length of 112 instead of 116.
     * `p` must fall below 0.2667 -- -21%.  At 10 references that needs a
       live length of 113 (it is 89, and 89 + the whole tail is about 116);
       at length 89 it needs SEVEN references, because 8 is still 0.2697 and
       only the `floor_log2` step at 8 -> 7 is big enough (2*7/89 = 0.157).

   `p` HAS NINE USES IN THE TARGET ITSELF and they are all needed:
   `beqz $s4` x2 and `bnez $s4` x1 for the three null tests, `lbu 0x1($s4)`
   x3 and `lbu 0x0($s4)` x3 for the two copies of the double condition.
   Plus the parameter copy that is ten.  So the seven-reference route is not
   reachable by any spelling, and the 113-insn route needs `p` live through
   the whole tail, which it is not: its last use is word 128 of 174.

   MEASURED, with `;; N regs to allocate:` read each time (device = the
   catalogued zero-instruction `asm`, "AN `asm` THAT WRITES A VARIABLE ALSO
   RESTORES ITS REFERENCE COUNT" / "A MATCHED-OPERAND `asm` IS A SECOND SET
   THAT IS *NOT* A SCHEDULING BARRIER"):

     base                                         31   p>c>mode
     `__asm__("" : "=r"(mode) : "0"(mode))` x1    47   p>mode>c
     the same x2                                  36   mode>p>c
     the same x3                                  59   mode first
     `__asm__("" ::"r"(p))` before the return     --   p 11/116, still > c
     p-device + one mode-device                   --   p>mode>c, 0.282/0.280
     p-device + mode-device + one c-device        47   c jumps to SECOND
     p-device + mode-device + two c-devices       47   same
     the final `p != NULL || mode == 0` moved
       after the `t0 != t` test                   --   p 10/92, no reorder
     the whole first `if` moved below `t0 = t;`   --   identical dump
     `p = (u8 *)arg;` moved after `h = m;`        --   p 10/85, no reorder

   **The c-device result is the interesting one and worth recording on its
   own: one matched-operand `asm` on `c` takes its REG_LIVE_LENGTH from 240
   to 122.** `c` "dies in 2 places" (the early `return 0` path and the tail),
   and the 240 is the two ranges summed; a second SET splits them and the
   number halves. So a length read off `.lreg` for a pseudo that DIES IN TWO
   PLACES is not a span, and comparing it with a single-death pseudo's length
   is comparing two different things. That is very likely why `c` sits where
   it does here at all, and it is the one number in the table above that
   should be distrusted.

   ------------------------------------------- the other six rows

   Around the double condition the target duplicates `addiu $v0, $zero, 0xC`
   into TWO branch delay slots (0x800AD7B0 and 0x800AD7C0) and leaves the
   shared block's head as the `lbu`'s load-delay `nop`; this body puts a
   `nop` in the second slot and materialises the constant once, after the
   reload.  The cause is that the constant has to sit BEFORE the volatile
   re-read in the fall-through block for `dbr` to steal it, and nothing can
   move across a volatile MEM.  Measured: the constant written first
   (`0xC == *(volatile u8 *)(p + 1)`) is 31 and byte-identical -- fold puts
   the constant back on the right; the volatile moved to the FIRST `p[1]`
   test (the func_800AAF00 lever, where that placement was worth everything)
   is 37 here; volatile on BOTH 37.

   ------------------------------------------- what paid, and still holds

   The function compiled at 172 words / 27 on the first try and the whole
   2-word deficit was the target RE-LOADING `p[1]` for the second half of

       (p[1] == 2 && p[0] == 1) || (p[1] == 0xC && p[0] == 6)

   `*(volatile u8 *)(p + 1) == 0xC` for the SECOND test only is worth exactly
   those 2 words; see "-fcse-skip-blocks COMMONS TWO READS ACROSS A BRANCH
   EVEN WHEN THE JOIN BLOCK HAS TWO PREDECESSORS".  Rejected at 172/27: the
   condition as nested `if`s with a `goto`, as doubly-nested `if`s, and the
   second read spelled `*(u8 *)(arg + 1)`.  With the device: on the second
   test only 31, a separate `volatile u8 *` local 31, on the first-pair AND
   the case-2 test 31, on the whole `p` pointer 37, operands of the second
   test swapped 38.

   ------------------------------------------- for a permuter run

   The body below is the 31-row candidate.  What it needs is a +3% lift on
   `mode`'s allocno priority together with a -21% drop on `p`'s, with `c`
   held between them -- i.e. the permuter should be allowed to move
   statements that change those two live ranges, not the expressions.

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_800AD6FC
     python3 tools/decomp/ovlverify.py <unit copy> func_800AD6FC
   -> 174 of 174 words, 27 differing masked, 31 aligned. */

s32 func_800AD6FC(u8 *c, s32 mode, s32 arg)
{
    u8 *p;
    u8 *m;
    u8 *h;
    s32 t;
    s32 t0;

    p = (u8 *)arg;
    m = func_800990FC();
    h = m;
    if ((s16)func_80042900((s8 *)c, 0x19) != 0)
    {
        h = func_80099194(D_800E0CAE, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        func_80099290(h);
        func_800A5720(m);
        return 0;
    }
    if (mode == 3 && p != NULL && ((p[1] == 2 && p[0] == 1) || (*(volatile u8 *)(p + 1) == 0xC && p[0] == 6)))
    {
        mode = 2;
    }
    t = *(u16 *)(c + 0x24);
    t0 = t;
    switch (mode)
    {
    case 0:
        h = func_80099194(D_800E0CDC, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        break;
    case 1:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0CF3, h);
        t += 0x300;
        break;
    case 2:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D1B, h);
        if (p != NULL && p[1] == 0xC && p[0] == 6)
        {
            t += 0xA00;
        }
        else
        {
            t += 0x3200;
        }
        break;
    case 3:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D36, h);
        t += 0x6400;
        break;
    }
    if (p != NULL || mode == 0)
    {
        func_80099290(h);
        func_800A5720(m);
    }
    if (t0 != t)
    {
        func_800A56E0(0x51E);
    }
    if (t > 0x10000)
    {
        t = 0xFFFF;
    }
    *(u16 *)(c + 0x24) = t;
    if (c[0x25] != 0)
    {
        *(s32 *)(c + 0x1C) &= ~0x8;
    }
    if (c[0x66] < c[0x25])
    {
        c[0x25] = c[0x66];
        c[0x24] = 0xFF;
    }
    return mode;
}
