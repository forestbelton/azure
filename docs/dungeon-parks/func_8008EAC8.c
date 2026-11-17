/* PARKED 2026-11-16, j58; j71 took it from 20 aligned rows to 2 on
   2026-11-16.  target 600 words, candidate 600 words, **2 aligned
   differences** (ovlverify masked: 2).  Every instruction is positionally
   identical; both rows are register names only.

   WHAT IS LEFT (and only this):
     202  sll v0,a0,0x10    vs   sll $v1, $a0, 16
     203  sra a1,v0,0x10    vs   sra $a1, $v1, 16

   The SImode temp that holds `want << 16` on the way to the sign extension
   gets $v0 where the target has $v1.  This is now LITERALLY the same
   two-row residue as docs/dungeon-parks/func_80096384.c, instruction for
   instruction, and that park's own two rows are the same temp; whatever
   closes one should close the other.  From the dumps: that temp is a LOCAL
   quantity with 2 refs and a 2-insn range, so `QTY_CMP_PRI`
   (local-alloc.c:1693) gives it the top priority in its block and
   `find_free_reg` hands it the first register free over [birth, death];
   it has no copy- or arithmetic-suggestion to steer it, and nothing holds
   $v0 across those two insns in either build, so the difference is in
   `regs_live_at` over that window and not in anything the surrounding
   statements say.  Eighteen spellings of the `want` block were measured
   against it without moving it (list below).  PERMUTER FOOD.

   The old fifteen-row $a0/$a1 exchange and the five-row case 0x50 block
   are both GONE -- see j71's two levers below.

   HOW IT GOT HERE: 80 -> 67 -> 58 -> 29 -> 23 -> 20 (j58), -> 7 -> 2 (j71).

   *** J71'S FIRST LEVER, worth 13 rows (20 -> 7), ONE COMPILE: ***
   MOVE THE `*(s16 *)(d + 0x2A)` SIGN-EXTENSION IF/ELSE **OUT** OF THE
   `do { } while (0)`, leaving only the `want` ternary, the `cur` read and
   the `delta` subtraction inside it.  j58's header said the wrapper should
   cover "exactly the block func_80096384.c wraps" -- true when it was
   written, but j69 subsequently changed what that park wraps, and this
   function wanted the same change.  The mechanism is j69's: `flow` adds
   `loop_depth` per reference, so the zero-trip loop's LOOP notes were
   DOUBLE-COUNTING the two references to the raw `*(s16 *)(d + 0x2A)` read
   that sat inside it.  Pulling the if/else out drops that pseudo's weight
   and flips the $a0/$a1 pair that was 15 of the 20 rows.

   THE `do { } while (0)` IS STILL LOAD-BEARING: it is what keeps the three
   references to `want` weighted at depth 2.  Do not delete it, and do not
   put the halfword if/else back inside it.

   (1) THE SWITCH ARMS ARE IN SOURCE ORDER 0x8, 0x10, 0x28, 0x30, 0x48,
   0x50, 0x68, **0x88**, 0x70, 0x78, 0x80, 0x90, 0x98, 0xA0, 0xD8 -- NOT in
   value order.  gcc emits the arm bodies in SOURCE order under the sparse
   switch's binary decision tree, and the target's bodies run
   0x68 (8008F010), 0x88 (8008F03C), 0x70 (8008F06C), ...  Moving case 0x88
   up beside case 0x68 is worth **29 rows** (58 -> 29) on its own, because
   those two arms share the `jal func_80094270` / `sw <idx>, 0x10($sp)` tail
   and jump.c will only cross-jump them when they are ADJACENT.  Read the
   emitted arm order off the listing's label addresses before writing the
   switch; value order is a guess, not a reading.

   (2) THE 0x88 ARM'S FOURTH ARGUMENT MUST BE HOISTED INTO A LOCAL, worth
   **3 words and 9 rows** (603/67 -> 600/58).
       u = *(u8 **)(p + 0xD0 + j * 4);
       func_80094270(p, b, c, u, j);
   NOTE THIS CONTRADICTS the catalogue entry "TWO SWITCH ARMS CROSS-JUMP
   ONLY IF THEIR INDEX LANDS IN THE SAME REGISTER" (:27886), whose corollary
   says hoisting the pointer argument here "changes nothing" and that the
   merged-tail shape is "a reorg outcome, not a source one".  It is a source
   one: with the hoist AND the adjacency the merge fires and both arms are
   byte-exact.  The index `j` is an ordinary arm-local `s32`; no shared
   index local is needed, and the seven index spellings that entry lists
   were all measured against the WRONG ARM ORDER.

   (3) `s16` FOR CASE 0x50's HOISTED BYTE.  j58 measured this as the
   INDEX's type and it was worth 6 rows (29 -> 23) there; in j71's shape the
   same narrow local holds `m[0]` instead, and it is still load-bearing --
   `s32 j` in its place is 9 rows and turns the `srl` into an `sra`.

   (4) THE ZERO-TRIP `do { ... } while (0)`, now around the `want` ternary,
   the `cur` read and the `delta` subtraction ONLY -- see j71's lever above.

   (5) `(((u32)w >> 1) ^ 1) & 1` for func_8008D024's fourth argument: the
   target has `srl $a3, $a3, 1`, and `w` as a plain `s32` gives `sra`.

   ALSO LOAD-BEARING, each read off the listing and each measured:
   * `u16 k = *(u16 *)(p + 0xA2);` read into a local BEFORE `D_800E3544 = 0;`
     so that the store can be the `beqz`'s delay slot -- 1 word.
   * `if ((w2 & 0x20) == 0) { func_8008F6EC(...); return; } else { ... }`,
     NOT the `!= 0` form: the target's fall-through is the func_8008F6EC
     arm and the 0x96/func_8008FA14 arm is out of line.
   * `goto tail;` with one `tail:` label carrying the D_800DD0B8 skin, as
     func_80096384's park does; the D_800DCFB0 site is written out in full
     at its own `return` and jump.c merges the common five-instruction tail.
   * `(*(u16 *)0x80013714 & 1)` as the RAW CONSTANT ADDRESS at both gates,
     the unit's majority spelling.
   * One pointer local per skin site (`t` in the tail, `t2` at the
     D_800DCFB0 site).

   MEASURED AND REJECTED BY j58, against the 20-row body (all at 600 words
   / 20 unless the row says otherwise; the number is aligned differences).
   THE CASE-0x50 ROWS BELOW ARE SUPERSEDED: they were all measured with a
   local index, which j71's second lever replaced -- keep them only as
   evidence that no address spelling fixes a local index.
     case 0x50, share one index local with case 0x88      603 / 45
     case 0x50, hoist the slot pointer before the call    607 / 69
     case 0x50, `n = i * 4;` then `p + 0xAC + n`          599 / 42
     case 0x50, `((u8 **)(p + 0xAC))[i]`                  20
     case 0x50, `p + i * 4 + 0xAC`                        20
     case 0x50, `(s32)i * 4`                              20
     case 0x50, `u8 i` / `u16 i`                          20
     case 0x50, `s8 i` / `s32 i`                          26
     case 0x50, `i = i;` second assignment                20
     case 0x50, m[0] hoisted into an `s32 v`              20 (and `sra` for
                                                          the >> 5, so worse
                                                          before lever (3))
     case 0x68, hoist func_8009FADC's result              603 / 67
     case 0x68, dedicated index local                     603 / 184
     case 0x68 + 0x88 sharing one index local             603 / 184
     `s32 want`                                           596 / 27  (4 SHORT)
     `want = m[1] & 7; want <<= 9;`                       599 / 19  (1 SHORT)
     `*(s16 *)(d + 0x2A) &= 0xFFF;` before `want = ...`   601 / 30
     `want != *(s16 *)(d + 0x2A)` for the compare         21
     an explicit `cur` local for the `& 0xFFF` store      23
     `cur` read after the `delta` subtraction             20
     `want`'s sign extension as if/else, not the ternary  601 / 21
     the 0x88 arm left in value order (after 0x80)        600 / 58
   The `want` rows above were all measured with the halfword if/else still
   INSIDE the `do { } while (0)`, so their scores are not comparable to the
   7-row body.

   *** J71'S SECOND LEVER, worth 5 more rows (7 -> 2): CASE 0x50's INDEX
   LIVES IN `m` ITSELF, AND THE BYTE IS HOISTED INTO THE `s16` LOCAL. ***
       case 0x50:
           i = m[0x0];
           m = (u8 *)((i & 0x60) >> 5);
           if (func_80098920(*(u8 **)(p + 0xAC + (s32)m * 4),
                             func_8009FADC(i & 0x1F), 0x15, 0) < 0)
   WHY, measured from the RTL dumps (tools/decomp/dump.sh, -G0):
   the five rows were ONE local-alloc quantity.  `combine_regs`
   (local-alloc.c:1791) ties the destination of an insn to a dying source,
   so `t = m[0] & 0x60` -> `i = t >> 5` -> `i * 4` -> `+ p` all became one
   quantity; that quantity crosses the `jal func_8009FADC`, so it must be
   call-saved and every member came out `$s0`.  The target has the index in
   `$s0` and the three temps in `$v0`, i.e. the index is a SEPARATE,
   GLOBALLY allocated allocno.  `local_alloc` only hands a pseudo a local
   quantity when `reg_basic_block[i] >= 0 && reg_n_deaths[i] == 1`
   (local-alloc.c:472), so the index has to appear in TWO basic blocks --
   and then `combine_regs` refuses both ties (`reg_qty[sreg] >= -1`,
   local-alloc.c:1911) and the temps fall back to `$v0`.
   Confirmed directly: ANY function-scope variable already used elsewhere,
   substituted for `i` here, removes all five rows.  What is left is then
   whatever that variable's OTHER site loses, and `m` is the one that loses
   nothing, because in the target `$s0` holds `q`, then `m`, then this
   index, on three disjoint paths -- one hard register, three values.
   Scores for the other holders, all measured against the 7-row body
   (aligned differences; case 0x50 itself is correct in every one of them):
       `m` + byte hoisted into `s16 i`        **2**  <- the body below
       `m` + byte hoisted into `u16`-cast i     2    (identical output)
       `n`                                      5    (3 rows at n's own site)
       `n` for the index, `q` as a copy         5    (same; `n` does the work)
       `m` + byte hoisted into `w`              5    (byte lands in $a3)
       `cur`                                    6    (4 rows at cur's site)
       `w` / `delta`                            7    (5 rows at their sites)
       `w2`                                     8
       `m` + byte hoisted into `j`              9
       pointer `q` directly                 599 / 8  (q's D_80083460 init
                                                      rematerialises; one
                                                      word short)
       `k`                                     22
       `r` / `e`                        602 / 28, 602 / 27
       `j`                                 603 / 29
       `want`                                 178
   The byte MUST be hoisted into a narrow local (`s16`): `m` is reassigned
   before the `& 0x1F` argument is read, so `m[0]` cannot be read twice, and
   a wide/global holder (`w`, `j`) puts the loaded byte in the wrong
   register.  `(i & 0x60) >> 5` with `s16 i` also keeps `srl`; with `s32 j`
   it comes out `sra`.

   ALSO MEASURED AND REJECTED BY J71, against the 7-row body (600 words /
   7 unless stated):
     all 16 positions for the `s16 i` declaration          7 (no effect)
     hoisting func_8009FADC's result into `n`              7
     `i = 0;` dead store after / inside the `if`           7  (a dead store
                                                            is deleted
                                                            BEFORE
                                                            `mark_set_1`
                                                            records
                                                            reg_basic_block,
                                                            flow.c:1541, so
                                                            it cannot make a
                                                            pseudo global)
     inverted `if` with an early return                    7
     `register` + block-scope index                        7
     the index written inline, no local             599 / 26
     `p + (i * 4 + 0xAC)`                                  7
     `(m[0] & 0x60) / 32`                                  7
     `((u8 **)(p + 0xAC))[i]`                              7
     `do { } while (0)` around the arm / the index         7
     cur after delta, INSIDE the do-while                  7
     cur read before the do-while                         13
     cur read after the do-while                          11
     delta outside the do-while                           15
     only the ternary inside the do-while                 15
     no do-while at all                                   23
     a second, nested do-while                            15
     `want = m[1] & 7; want <<= 9;`                 599 / 19
     `want = (m[1] << 9) & 0xE00`                   599 / 20
     `(s16)((m[1] & 7) << 9)`                              7
     compare operands swapped                              8
     three other spellings of the `want` ternary           7
     abs via a ternary / `delta >= 0x801` / `-0x1000`      7
     `(cur & 0xFFF) | (want & ~0xFFF)` store order        11
     `delta <= 0` for the abs                              8
     final compare as `want <` / `<=` with arms swapped    7
     final compare through `*(u16 *)(d + 0x2A) -= 0x200`   7

   MEASURED AND REJECTED BY J71 AGAINST THE 2-ROW BODY (all 600 words / 2,
   i.e. no effect on the surviving pair, unless stated):
     `(s32)want` / `(s32)(s16)want` in the `delta` subtraction      2
     `(s32)` cast on the `*(s16 *)(d + 0x2A)` load                  2
     `delta = -(want - *(s16 *)(d + 0x2A));`                        2
     `delta = *(s16 *)(d + 0x2A); delta -= want;`                  20
     `cur` as u16 / s16 / u32                                       2
     `delta` as s16                                           603 / 40
     `want` as s32 / u16 / s8                   596 / 14, 596 / 15, 592 / 28
   The pair is insensitive to every spelling of the statements around it.
   Both of its insns are emitted as one unit by the sign-extension expander,
   so no source reordering can put a basic-block boundary between them, and
   that is what it would take to make the temp a GLOBAL allocno (which is
   what pseudo 181 is here, and it gets $v1).

   DECLARATIONS THIS BODY NEEDS (new; nothing else in the unit declares
   them):
       extern void (*D_800DD830[])(void);
       extern u8 D_800245A8[];
       extern u8 D_80050CAC[];
   D_800DD830 and D_800245A8 are also wanted by docs/dungeon-parks/
   func_80096384.c; same spelling, so they merge.
   NO PROTOTYPE CHANGE: `void func_8008EAC8();` at the top of the unit is
   unprototyped and compatible with this definition.  func_8008D94C,
   func_8008CAA0, func_8008CBA0, func_8008C468, func_80090200, func_8008C7B4,
   func_8008C13C, func_8008F6EC, func_80095538, func_800956B8, func_80095854,
   func_8009FADC and func_8009F868 are all reached through declarations the
   unit already has.
 */
void func_8008EAC8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    u8 *m;
    u8 *t;
    u8 *t2;
    u8 *u;
    s32 e;
    s32 w;
    s32 w2;
    s32 r;
    s32 n;
    s16 i;
    s32 j;
    s32 cur;
    s32 delta;
    s16 want;
    u16 k;

    if (p[0x9A] != 0xE)
    {
        if ((*(u16 *)(p + 0xA2) & 0x100) != 0)
        {
            func_8008D94C(p, b, c, d);
            return;
        }
    }
    p[0x9A] = 0xE;
    if ((*(s32 *)(d + 0x14) & 0x100000) == 0)
    {
        func_800A4300(c, d);
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
    }
    *(u16 *)(c + 0x14) &= 0xF7FF;
    q = (u8 *)&D_80083460;
    p[0x9B] = 0;
    *(u16 *)(q + 0x2) &= 0xFF7F;
    r = *(s16 *)(d + 0x64);
    if (r < 0 || (*(u16 *)(p + 0x10C) & 1) != 0)
    {
        func_8008CAA0(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if ((*(s32 *)(d + 0x14) & 0x20000) != 0)
    {
        *(s32 *)(d + 0x14) &= 0xFFFDFFFF;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        func_8008C468(p, b, c, d);
        return;
    }
    e = *(s32 *)(d + 0x1C);
    if ((e & 0x200) != 0)
    {
        func_80090200(p, b, c, d);
        return;
    }
    if ((e & 0x100000) == 0)
    {
        *(void **)(p + 0x8C) = func_8008ACDC;
        t2 = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        return;
    }
    if (*(s32 *)(p + 0x124) == 0)
    {
        return;
    }
    if ((s16)func_800A1C58(d) != 0)
    {
        if (func_8008D1D0(p, b, c, d) != 0)
        {
            return;
        }
    }
    if ((*(u16 *)(q + 0x2) & 0x4) != 0)
    {
        goto tail;
    }
    if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
    {
        if ((*(u16 *)0x80013714 & 1) == 0 && (g->unk_08 & 0x80) != 0)
        {
            *(s16 *)(d + 0x8A) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = NULL;
            *(s32 *)(p + 0xC8) = 0;
            *(s32 *)(p + 0x104) = 0;
            return;
        }
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)0x80013714 & 1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        if ((*(u16 *)(p + 0xA2) & 0x200) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFDFF;
        }
        m = func_8009F868();
        if (m == NULL)
        {
            return;
        }
        want = (m[0x1] & 7) << 9;
        *(s16 *)(d + 0x2A) &= 0xFFF;
        if (*(s16 *)(d + 0x2A) != want)
        {
            if ((*(u16 *)(d + 0x2A) & 0x800) != 0)
            {
                *(s16 *)(d + 0x2A) |= 0xF800;
            }
            else
            {
                *(s16 *)(d + 0x2A) &= 0x7FF;
            }
            do
            {
                want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                cur = *(u16 *)(d + 0x2A);
                delta = *(s16 *)(d + 0x2A) - want;
            } while (0);
            if (delta < 0)
            {
                delta = -delta;
            }
            if (delta > 0x800)
            {
                *(s16 *)(d + 0x2A) = (want & ~0xFFF) | (cur & 0xFFF);
            }
            if (*(s16 *)(d + 0x2A) > want)
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) - 0x200;
            }
            else
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            func_8009F988();
            goto tail;
        }
        D_800E3544 = m[0x1] & 0xF8;
        switch (D_800E3544)
        {
        case 0x8:
            func_8008C13C(p, b, c, d);
            return;
        case 0x10:
            func_8008C7B4(p, b, c, d);
            return;
        case 0x28:
            func_8008F6EC(p, b, c, d);
            return;
        case 0x30:
            *(s16 *)(p + 0x96) = 6;
            func_8008FA14(p, b, c, d);
            return;
        case 0x48:
            if ((s16)func_80095538(p, m[0x0] & 0x1F, m[0x2] & 0x1F) < 0)
            {
                func_8009F988();
            }
            else
            {
                func_8009FAAC();
            }
            return;
        case 0x50:
            i = m[0x0];
            m = (u8 *)((i & 0x60) >> 5);
            if (func_80098920(*(u8 **)(p + 0xAC + (s32)m * 4), func_8009FADC(i & 0x1F), 0x15, 0) < 0)
            {
                func_8009F988();
            }
            return;
        case 0x68:
            func_80094270(p, b, c, func_8009FADC(m[0x0] & 0x1F), (m[0x0] & 0x60) >> 5);
            return;
        case 0x88:
            j = (m[0x0] & 0x60) >> 5;
            u = *(u8 **)(p + 0xD0 + j * 4);
            func_80094270(p, b, c, u, j);
            return;
        case 0x70:
            func_80094548((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x78:
            func_8009458C((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x80:
            func_800945C4((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x90:
            func_8009456C((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x98:
            u = func_8009FADC(m[0x0] & 0x1F);
            if ((u[0x3] & 0x20) != 0)
            {
                func_800956B8(p, b, c, u);
            }
            else
            {
                func_80095854(p, b, c, u);
            }
            return;
        case 0xA0:
            func_8002534C(p, b, c, d);
            return;
        case 0xD8:
            D_800DD830[m[0x0] & 0x7F]();
            return;
        }
        return;
    }
    k = *(u16 *)(p + 0xA2);
    D_800E3544 = 0;
    if ((k & 0x200) != 0 && (s16)func_800A2C34(d) == 0 && (*(u16 *)(q + 0x2) & 0x4) == 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFDFF;
        if ((s16)func_800A4474(c[0x24], c[0x25]) != 0)
        {
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
    }
    w = g->unk_08;
    if ((w & 0x80) != 0)
    {
        func_8008CF6C((UnkDungeonActor *)p, b, c, D_80050CAC);
        return;
    }
    if ((w & 0x10) == 0 && (w & 3) != 0)
    {
        if (func_8008D024(p, b, c, (((u32)w >> 1) ^ 1) & 1, 0) != 0)
        {
            return;
        }
    }
    n = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
    if ((s16)n != 0xFFF)
    {
        *(s16 *)(d + 0x2A) = n;
        if ((g->unk_08 & 0x10) == 0 && (*(u16 *)(p + 0xA2) & 0x400) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((*(s32 *)(d + 0x1C) & 0x400) != 0)
            {
                *(u16 *)(d + 0x2A) += func_800A6D30() & 0xE00;
            }
            func_8008C13C(p, b, c, d);
            return;
        }
    }
    w2 = g->unk_08;
    if ((w2 & 0x30) == 0x30)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x40) != 0)
    {
        if ((w2 & 0x40) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFBF;
        }
    }
    else if ((w2 & 0x40) != 0)
    {
        if ((w2 & 0x20) == 0)
        {
            func_8008F6EC(p, b, c, d);
            return;
        }
        else
        {
            *(s16 *)(p + 0x96) = 6;
            func_8008FA14(p, b, c, d);
            return;
        }
    }
tail:
    t = D_800DD0B8;
    *(u8 **)(c + 0x2C) = t;
    func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
}
