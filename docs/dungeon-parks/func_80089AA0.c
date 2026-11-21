/* RECONSTRUCTION of func_80089AA0 (1165 words), batch j76, 2026-11-16.

   ====================================== 2026-11-21: 75 rows -> 69

   Two duplicated-arm devices from tools/decomp/devsweep.py, out of 12265
   candidates a round (223 statements x 54 names -- the largest sweep this
   block has run):
     * `if (r1)` around `r1 = (s16)func_80042900((s8 *)...);`, 75 -> 71.  The
       condition reads r1 before the call that sets it, which is the same
       indeterminate-condition question func_800A94A0's park raises: the
       device's free reference lands on the CONDITION's name, and here the
       name wanted is the one the statement assigns.
     * `if (ta)` around the `fn1(q3, ...)` call, 71 -> 69.

   ============================== 2026-11-20: 87 rows -> 81

   The body below carries one device: a duplicated-arm `if (f13)` around `s3 = D_800E3DE8;`.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same lever from two directions -- flow.c:2128 weights each
   reference by `loop_depth`, and jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  SWEPT, NOT REASONED:
   every statement position times both devices over every local, on a
   `tools/decomp/onefunc.py` file where a candidate is 0.09 s.  Nothing in
   this park's diagnosis points at the winning statement, and that is the
   normal case rather than the exception.
   The 2026-11-15 body that reached 95 aligned rows died in the /tmp cleanup;
   this is a fresh reconstruction from the listing plus the four catalogue
   entries written against this function, and it is now BETTER than that
   number.

   RESIDUE: target 1165 words, candidate 1165 words -- the word count is
   EXACT -- and 87 aligned differences (tools/decomp/aligndiff.py: 69 R,
   7 +, 6 -, 5 X). ovlverify reports 259 masked word differences on the same
   body, because three shifted regions move every branch offset inside them;
   aligndiff's 87 is the honest figure, per docs/town-sweep.md.

   WHAT IS SETTLED, AND WHAT EACH STEP WAS WORTH (543 -> 87)
   1. `s32 flagB[2]` used as `flagB[0]` IS the retry flag of the final loop,
      and it gives the target's 0x40 frame with the 8-byte local area at
      sp+0x10. All twenty sw/lw offsets match. (:28057.)
   2. ONE LOCAL PER REGION, NOT ONE REUSED LOCAL PER ROLE: 543 -> 298.
      Reusing five pointer locals (f/q/s/w/k) across the whole function made
      each ONE pseudo with a 1200-1300-insn live range crossing 36-43 calls,
      and the whole callee-saved assignment came out permuted -- p in $s5
      instead of $s1, d in $s7 instead of $s2, c in $fp instead of $s5.
      Thirteen separate &D_80083460 locals, one per point where the target
      rematerialises the base, is what fixes it.
   3. EACH `while (q != e)` WALK NEEDS ITS OWN SENTINEL LOCAL `t = e;`:
      200 -> 169 and +4 words. The target's `addu $s2, $s4, $zero` in the
      entry branch's delay slot is that copy sitting in the loop preheader.
      Without it the loop test reads `e` directly, `e` collects the
      loop-depth weight of every walk in REG_N_REFS (flow.c:2122), and `e`
      and `h` swap $s3/$s4 over the whole function -- about 65 rows.
   4. THE FINAL RETRY LOOP MUST BE A REAL `for (;;)`, NOT A BACKWARD `goto`:
      200 -> 169, and it is what puts a `high` in a register for the whole
      loop. gcc only emits NOTE_INSN_LOOP_BEG for a loop STATEMENT, so a
      `goto` loop gets no preheader and loop.c hoists neither the two
      `%hi` bases nor the base copy. The two inner back edges are `continue`;
      the last one stores the flag halfword first and cross-jumping merges
      that store with the loop's entry store, which is what .L8008A9A4 is.
   5. WALKER GROUPING, by exhaustive measurement over all 52 partitions of
      the five list walkers (:28106 is right that it has to be measured):
      merging the 0xFFBF walk, the 0x8A/0x6A walk, the inner callback walk
      and the second-arm callback walk into ONE local, with the
      func_800A9AFC walk left SEPARATE, is 169 -> 152. All-five-merged is
      176, all-separate 169, and no other partition beats 152.
      Then, each measured on top of the last: all 15 partitions of the four
      SENTINELS -> {entry walk, 0x8A walk, func_800A9AFC walk} as one local
      and the nested loop's own sentinel separate, 130 -> 125; the three
      function-pointer temps as ONE local, 125 -> 114 (the single largest
      step after the frame); the two func_800BCB04 results as one local,
      114 -> 105; {nz, i} as one counter and the func_8009B25C result
      sharing the walker's local, -> 103.
   6. THE TWO BIT TESTS AT 8008A79C AND THE TWO AT 8008ABA4 MUST BE NESTED
      `if`s, NOT `&&`. `(x & 0x400) == 0 && (x & 0x2000) != 0` folds to one
      `andi 0x2400` against a materialised 0x2000 and is three words short;
      the same for `(x & 0x400) != 0 && (x & 0x800) == 0` -> `andi 0xC00`.
      Nested ifs give the target's two separate `andi`. 152 -> 147 and
      103 -> 94.
   7. The 0x80/0x100/0x200 fork is NOT an if/else-if chain -- that gives
      `bgtz` then `bltz`, because expand branches on the negation of each
      test. The target's `bgtz` / `bgez` pair is the early-exit form:
          n = 0x80; if (r <= 0) { n = 0x100; if (r < 0) { n = 0x200; } }
   8. THE RETRY LOOP'S BASE IS TWO LOCALS, `fA` before the loop and
      `f13 = fA` for the loop body: 94 -> 92 AND it is the last two words.
      The copy is the target's `addu $s3, $v1, $zero` at 8008A99C; with one
      local the 0x10 test, the 0x2 read and every loop reference share one
      register and the copy never appears.
   9. THE 0xFBFF VALUE IN THE TAIL NEEDS ITS OWN LOCAL (92 -> 87):
          t3 = t2 & 0xFBFF;  *(f+2) = t3;  if (t2 & 0x4000) return;
          *(f+2) = t3 | 0x800;
      Recomputing `t2 & 0xFBFF` for the second store costs five rows of
      register assignment in that block.
  10. `s32 func_8009FB34_i(u16, u16) __asm__("func_8009FB34");` -- the unit's
      declaration returns s16, which costs a `sll 16`/`sra 16` pair the
      target does not have, because the result is stored as a byte and then
      re-extended with `sll 24`/`sra 24`. Two words.
  11. `*(u16 *)0x80013714`, the raw constant address, at all six sites and
      not the declared D_80013714: the `lui` temp is the load's own
      destination at all six (the lever at :14092), and the unit's other
      sixteen sites spell it that way.
  12. The func_8003DB6C destination is `(u8 *)&D_80083460 + 8`, NOT the
      D_80083468 symbol -- which does exist in
      splat/dungeon.bin_undefined_syms_auto.txt, so splat NAMES it, but the
      relocation has to be against D_80083460 with +8 in the addend for
      cse's related_value to give `addiu $s0, $s3, -0x8` at 80089CFC and
      `sh $v0, -0x8($s3)` at 80089DEC off the same base.
  13. D_800E4948 is the 4-word SNAPSHOT that call writes, and the four
      compares just above it test the live record against it. They must be
      `D_800E4948[0..2]` plus `*(s16 *)&D_800E4948[3]` -- written
      symbolically, so the first is `lw %lo(D_800E4948)($v0)` and the other
      three ride a cse base at 0x4/0x8/0xC -- and NOT a pointer local, which
      would make the first `lw 0x0($a1)`.
  14. `addiu $s3, $a0, 0x18` at 80089AD8 is a base local plus 0x18, not
      `&D_80083178`: `g = (u8 *)&D_80083160;` (read at g+0x10) and
      `h = g + 0x18;` (read once, at `*(u16 *)(h + 0xB0)` = 0x80083228).
      Writing `&D_80083160 + 0x18` as one expression folds to a CONST and
      relocates against D_80083178.
  15. `if ((*(s32 *)(e + 0x1C) & 0x20) == 0) { ... } else { |= 0x6000; }` --
      the 0x6000 arm is the BRANCH TARGET, so it is the else.

   WHAT IS STILL DIFFERENT -- 87 rows in six groups, largest first
   * ~30 rows, 8008A9D4..8008AC2C: THE FINAL LOOP'S THREE CALLEE-SAVED
     LOCALS ARE A 3-CYCLE PERMUTATION. walker $s1 (target $s4), sentinel
     $s4 (target $s2), `(s16)func_800A2C34`'s result $s2 (target $s1). The
     ONE fix worth trying next; everything in those rows is otherwise
     identical. Measured and rejected: the walker merged with `e` (158),
     with the main walker group (87, inert), the sentinel merged with `d`
     (157) or with a walk sentinel (124/145), the result local merged with
     any other s32 (132-136), the walker split into one local per sub-loop
     (87, inert), `s16` for the result (117).
   * ~17 rows, 8008A700..8008A8CC: the four walk SENTINELS land in $s0/$s3
     where the target has all of them in $s2; `cnt` is in $s2 where the
     target has $s6 (the same register as the `i`/`nz` counter); and
     `%hi(D_800E3DE8)` is in $s5 where the target has $s3. The D_800E3DE8
     store is also not in the `blez`'s delay slot.
   * ~11 rows, 80089DD4..80089E00: the `sll 16`/`sra 16` pair writes ONE
     register where the target writes two ($v0 then $v1) -- the residue
     docs/dungeon-sweep.md records as blocking three other parks -- and the
     0x80/0x100/0x200 constants take $v1 where the target has $v0. All
     adjacent declaration-order swaps of those two locals are inert, so
     they do not tie and :28106's rule says order cannot reach it.
     The `sh $v0, -0x8($s3)` also ends up in the `bnez`'s delay slot instead
     of ahead of the D_80013714 load.
   * ~7 rows, 8008A514..8008A548: `*(s32 *)(e + 0x1C)` is loaded into $v1
     where the target holds it in $a0 across the branch, and the
     `*(u16 *)(e + 0x88)` copy is missing -- the target has
     `addu $a2, $v1, $zero` and then stores 0x8A from $v1. A local for the
     0x1C word and a hoisted `(s16)(r5 - 0x20)` were both measured inert.
   * ~6 rows, 8008A98C..8008A99C: `%hi(D_800E3D7C)` is in $s7 and
     `%hi(D_800E3DE8)` in $fp where the target has $fp and $s7, and the
     `f13 = fA` copy is emitted before the 0x2 read rather than after it.
   * ~4 rows, 8008A918/8008A954: the second callback loop's node pointer
     `addiu $s0, $s1, -0x20` is at the loop top where the target has it in
     the preheader and in the closing branch's delay slot. Writing it as
     part of the increment instead measured 131, so it is not that.

   OTHER MEASURED AND REJECTED
     all five walkers in one local                       176
     every walker separate                               169
     {i, cnt, nz} as one counter                         190
     {cnt, nz} 188   {i, cnt} 185   {cnt, cnt2} 149
     func_8009B25C's result reusing the `p` parameter     233
     the retry loop as a backward `goto`                  169 and 5 words short
     `(x | 0x4000) & 0xEFF7` instead of `(x & 0xEFF7) | 0x4000`   90
     `s16 cnt`  88 and one word long
     every adjacent declaration-order swap of the 40 locals: all inert at 87,
       which is :28106's rule holding again -- nothing in this function ties.

   DECLARATIONS
   extern s32 D_800E4948[];
   extern s32 D_800DD160;
   extern s32 D_800E3D70;
   extern u16 D_800DD258;
   s32 func_80096100_4(u8 *, UnkVec *, u8 *, u8 *) __asm__("func_80096100");
   s32 func_8009FB34_i(u16, u16) __asm__("func_8009FB34");
   END DECLARATIONS

   AND ONE MORE, WHICH CANNOT GO IN THE BLOCK ABOVE BECAUSE
   tools/decomp/parkresidue.py CORRUPTS THE UNIT WHEN IT SEES IT. It is
   written below this comment instead, behind an empty comment:

       extern u16 D_800DD25A __asm__("D_800DD258 + 2");

   `adopt()`'s PROTO regex reads `__asm__` as the declared NAME on an
   `extern ... __asm__("...")` line; it then searches the unit for that
   "name", matches
   `extern UnkDungeon82EB0 D_80082EB0_m __asm__("D_80082EB0");`
   and REPLACES it, and the compile fails with `D_80082EB0_m undeclared`
   7000 lines away. A leading comment on the line stops PROTO matching and
   the declaration still reaches the compiler. Worth fixing in the tool:
   PROTO should not match a line that starts with `extern`.

   PROTOTYPE CHANGE ON LANDING: none. func_80096100 is declared in the unit
   with ONE parameter and func_8009FB34 with an s16 return; both call sites
   go through the `__asm__` aliases above, so neither landed definition is
   touched.    ====================================== 2026-11-21: 81 rows -> 75 at
                                          1165/1165

   A PERMUTER FIND: the literal `1' at TWELVE sites now goes through one
   `s32 one', set `one = 1;' immediately before `*(s16 *)(k - 8) = n1;'.
   Nothing else changed.

   The placement is load-bearing to within one statement.  Putting the
   assignment inside the `if (r1 < 0)' block just above it -- which is where
   a careless transcription of the permuter's output lands it -- measures 79
   rows, not 75.  It has to sit after that block closes.

   WHAT IT IS DOING: `1' is wanted in a register twelve times across a
   1165-word function, and the target plainly keeps it in one.  With the
   literal, each site re-materialises it.  This is the same lever as
   func_8009F644's `qo = 0xC', which was worth that park's missing word --
   a constant through a local escapes being folded into an instruction.
*/
extern u16 D_800DD25A __asm__("D_800DD258 + 2");
void func_80089AA0(u8 *p, UnkVec *c, u8 *d)
{
    s32 one;
    u8 *e;
    u8 *g;
    u8 *h;
    u8 *v1;
    u8 *f1;
    u8 *k;
    u8 *f2;
    u8 *f3;
    u8 *f4;
    u8 *f5;
    u8 *f6;
    u8 *f7;
    u8 *f8;
    u8 *f9;
    u8 *f10;
    u8 *f11;
    u8 *f12;
    u8 *fA;
    u8 *f13;
    u8 *v2;
    u8 *q1;
    u8 *q2;
    u8 *q3;
    u8 *q8;
    u8 *s1;
    u8 *s3;
    u8 *w2;
    void (*fn1)(u8 *, s32, s32);
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 r1;
    s32 n1;
    s32 n2;
    s32 r2;
    s32 n3;
    s32 i;
    s32 fi;
    s32 r3;
    s32 mm;
    s32 r6;
    s32 r5;
    s32 r7;
    s32 cnt;
    s32 cnt2;
    s32 t1;
    s32 t2;
    s32 t3;
    u8 *ta;
    u8 *tc;
    s32 flagB[2];

    g = (u8 *)&D_80083160;
    e = p;
    h = g + 0x18;
    if ((*(u16 *)0x80013714 & 4) != 0 && D_800E3CD0 == 0 && *(s32 *)(g + 0x10) != 0)
    {
        D_800E3CD0 = 1;
        func_80040AA0(3);
        return;
    }
    if (D_80080AA0 != 0 && D_800E3CD0 == 0 && func_80053EF0(1) == 0)
    {
        func_80053DA8(0x200);
        D_80080AA0 = 0;
        D_80080A88 = 1;
    }
    if ((*(u16 *)0x80013714 & 4) != 0)
    {
        v1 = D_800DCE60;
        *(s16 *)(v1 + 4) = *(s16 *)(v1 + 4) + (((D_80081468[0] & 1) != 0) ? 8 : -8);
        D_800832B4 = 0;
        func_8004D7A8(one);
        func_8004D294(NULL, D_800DCE60, 2);
    }
    *(s16 *)(p + 0x9E) = (*(u16 *)(h + 0xB0) >> 9) & 7;
    f1 = (u8 *)&D_80083460;
    *(u16 *)(f1 + 2) &= 0xFFCF;
    x0 = *(s32 *)(f1 + 8);
    if (x0 == D_800E4948[0] && (x1 = *(s32 *)(f1 + 0xC)) == D_800E4948[1] &&
        (x2 = *(s32 *)(f1 + 0x10)) == D_800E4948[2] &&
        (x3 = *(s16 *)(f1 + 0x14)) == *(s16 *)&D_800E4948[3] &&
        (*(u16 *)0x80013714 & 8) == 0 && p[0x9A] != 0x17 && p[0x9A] != 0x27 &&
        p[0x9A] != 0x29 && D_800DCF4F == 0 &&
        (x1 != 0 || x2 != 0 || x3 != 0 || x0 != 0))
    {
        D_800DD160++;
    }
    else
    {
        D_800DD160 = 0;
    }
    k = (u8 *)&D_80083460 + 8;
    func_8003DB6C(D_800E4948, (int *)k, 4);
    f2 = (u8 *)&D_80083460;
    if (*(s32 *)(f2 + 0xC) == 0 && *(s32 *)(f2 + 0x10) == 0 && (*(u16 *)(f2 + 2) & 8) == 0 &&
        *(s16 *)(f2 + 8) == 0 && *(s16 *)(f2 + 0x14) == 0 && (D_800E296C & 0x100000) == 0 &&
        *(s16 *)(f2 + 0xA) == 0 && (*(u16 *)(f2 + 2) & 0x2E01) == 0)
    {
        if ((s16)func_8009F5CC() != 0)
        {
            func_800C6AB0();
        }
        if ((*(u16 *)(f2 + 2) & 2) != 0)
        {
            *(u16 *)(f2 + 2) = (*(u16 *)(f2 + 2) & 0xFFFD) | 0x20;
            func_800A0DD8();
            if (r1)
            {
                r1 = (s16)func_80042900((s8 *)e, 7);
            }
            else
            {
                r1 = (s16)func_80042900((s8 *)e, 7);
            }
            n1 = 0x80;
            if (r1 <= 0)
            {
                n1 = 0x100;
                if (r1 < 0)
                {
                    n1 = 0x200;
                }
            }
            one = 1;
            *(s16 *)(k - 8) = n1;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                f3 = (u8 *)&D_80083460;
                *(u16 *)(f3 + 0x1E) = *(u16 *)(f3 + 0x1E) + 1;
                D_800E3D70 += e[0x29];
                n2 = e[0x28];
                while ((u32)D_800E3D70 > 0x100)
                {
                    D_800E3D70 -= 0x100;
                    n2++;
                }
                if (e[0x29] < n2)
                {
                    n2 = e[0x29];
                }
                e[0x28] = n2;
                for (i = 1; i >= 0; i--)
                {
                    q1 = *(u8 **)(p + 0xAC + i * 4);
                    if (q1 != NULL && q1[0x25] != 0)
                    {
                        D_800E3D74[i] += q1[0x29];
                        n2 = q1[0x28];
                        while ((u32)D_800E3D74[i] > 0x100)
                        {
                            D_800E3D74[i] -= 0x100;
                            n2++;
                        }
                        if (q1[0x29] < n2)
                        {
                            n2 = q1[0x29];
                        }
                        q1[0x28] = n2;
                    }
                }
            }
            if ((*(s32 *)(e + 0x1C) & 0x800) != 0)
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 8) + 0x30;
            }
            else
            {
                r2 = *(u16 *)(p + 0xF8);
                *(s16 *)(p + 0xF8) = r2 + one;
                n3 = (func_800644B8((s16)r2) >> 7) + 0x40;
            }
            e[3] = n3;
            *(s32 *)(p + 0x128) = func_800A6D30() & 0xFFFF;
            if ((*(u16 *)0x80013714 & 8) == 0)
            {
                *(s16 *)(p + 0x118) = *(u16 *)(p + 0x118) + one;
            }
            func_800A4ACC(e);
            if (*(s32 *)(p + 0x8C) == (s32)func_80096384)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) == 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
                    f4 = (u8 *)&D_80083460;
                    *(u16 *)(f4 + 2) |= 4;
                }
            }
            else if (*(s32 *)(p + 0x8C) == (s32)func_8008ACDC)
            {
                if ((s16)func_80042900((s8 *)e, 0xA) != 0)
                {
                    *(s32 *)(p + 0x8C) = (s32)func_80096384;
                    f5 = (u8 *)&D_80083460;
                    *(u16 *)(f5 + 2) |= 4;
                }
            }
        }
        f6 = (u8 *)&D_80083460;
        *(u16 *)(f6 + 2) &= 0xBFFB;
        *(s32 *)(e + 0x14) &= 0x7FFE7FFF;
    }
    else
    {
        f7 = (u8 *)&D_80083460;
        *(u16 *)(f7 + 2) |= 4;
    }
    if ((*(u16 *)0x80013714 & 8) == 0)
    {
        if (p[0x9A] != 0x17 && D_800DCF4F == 0 && (u32)D_800DD160 >= 0x385)
        {
            q2 = D_800E3D7C;
            D_800DD160 = 0;
            do
            {
                q2[0x71] &= 0x7F;
                *(s16 *)(q2 + 0x6C) = 0;
                q2 = *(u8 **)(q2 + 0x5C) + 0x20;
            } while (q2 != D_800E3D7C);
            f8 = (u8 *)&D_80083460;
            *(s16 *)(f8 + 8) = 0;
            *(s16 *)(f8 + 0xA) = 0;
            *(s16 *)(f8 + 0x14) = 0;
            *(s32 *)(f8 + 0xC) = 0;
            *(s32 *)(f8 + 0x10) = 0;
            *(u16 *)(f8 + 2) = 0;
        }
        if ((*(u16 *)0x80013714 & 8) == 0)
        {
            if ((D_80083462 & 4) == 0 || (D_800E296C & 0x40) != 0 || p[0x9A] == 0x17)
            {
                D_800E296C &= ~0x40;
                v2 = D_800DCE60;
                if ((s16)func_800C77D0(e - 0x20, c, 8, *(s16 *)(v2 + 6)) != 0)
                {
                    D_800832B4 = 0;
                    func_8004D7A8(1);
                    func_8004D294(NULL, v2, 2);
                }
                else
                {
                    D_800E3D20 = 0;
                }
            }
        }
    }
    if ((*(s32 *)(e + 0x1C) & 0x20) == 0)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_800478B8(d);
        }
        else
        {
            func_80048AC8(d, one);
        }
    }
    else
    {
        *(u16 *)(d + 0x14) |= 0x6000;
    }
    if (*(s32 *)(p + 0x8C) != 0)
    {
        (*(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C))(p, c, d, e);
    }
    D_800DD168[p[0x9A]](p, c, d, e);
    fi = (((D_80083228 + *(s16 *)(e + 0x2A)) + 0x100) >> 9) & 7;
    if (*(s16 *)(p + 0x94) != fi)
    {
        if ((s16)func_80042900((s8 *)e, 0xA) != 0)
        {
            func_8003DB94(d, *(void **)(*(s32 *)(d + 0x2C) + fi * 4), *(s8 *)(d + 4));
        }
        else
        {
            func_800489F4(d, *(*(u8 **)(d + 0x2C) + fi), *(s8 *)(d + 4), one);
        }
        *(s16 *)(p + 0x94) = fi;
        if (p[0x9A] != 0x22 && D_8006CCF8[fi] != 0)
        {
            *(u16 *)(d + 0x14) |= one;
        }
        else
        {
            *(u16 *)(d + 0x14) &= 0xFFFE;
        }
    }
    r3 = func_8009FB34_i(d[0x24], d[0x25]);
    d[0x26] = r3;
    mm = (s8)r3;
    if (mm >= 0)
    {
        D_800E2970_s[mm].unk_0C |= one;
    }
    func_800A020C(*(s32 *)(e + 0x1C), d + 0xC);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        c->unk_00 += c->unk_0C;
        c->unk_04 += c->unk_10;
        if ((*(u16 *)(p + 0x98) & 8) == 0)
        {
            c->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
            p[0x9D] = p[0x9D] + one;
        }
        *(s32 *)(p + 0x90) += c->unk_14;
        r6 = (s16)func_800BCB04(*(u16 *)((u8 *)c + 2), *(u16 *)((u8 *)c + 6),
                                (s16)(*(u16 *)(e + 0x88) - 0x20));
        if (r6 < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(e + 0x88) >= r6)
            {
                if ((*(u16 *)(p + 0x98) & 4) != 0 || c->unk_14 < 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                else
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
                    {
                        if ((D_800E296C & 0x80000) == 0)
                        {
                            func_800A56E0(0x508);
                        }
                        func_800B66C8((u8 *)c);
                    }
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
                *(u16 *)(p + 0xA2) &= 0xF7FF;
            }
            else
            {
                if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
                {
                    *(u16 *)(p + 0xA2) &= 0xFFEF;
                }
                *(u16 *)(p + 0xA2) |= 0x800;
            }
            if ((*(s32 *)(e + 0x1C) & 0x40000000) != 0)
            {
                *(s32 *)(e + 0x1C) &= 0xBFFFFFFF;
                r5 = *(u16 *)(e + 0x88);
                *(s16 *)(e + 0x8A) = r5;
                r6 = (s16)func_800BCB04((d[0x24] << 6) | 0x20, (d[0x25] << 6) | 0x20,
                                        (s16)(r5 - 0x20));
                *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + (*(u16 *)(e + 0x88) - r6);
                *(s16 *)(e + 0x88) = r6;
                if ((D_80083462 & 0x80) != 0)
                {
                    *(s32 *)(p + 0x90) = 0;
                    c->unk_14 = 0;
                    p[0x9D] = 0;
                    *(u16 *)(p + 0xA2) |= 0x10;
                }
            }
        }
        else
        {
            if ((*(u16 *)(p + 0xA2) & 0x800) != 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFEF;
            }
            *(u16 *)(p + 0xA2) |= 0x800;
        }
        *(s16 *)((u8 *)c + 0xA) = *(u16 *)(e + 0x88) + *(u16 *)(p + 0x92);
    }
    func_80096100_4(p, c, d, e);
    if ((*(s32 *)(e + 0x14) & 0x100000) == 0)
    {
        if (D_800DD258 != d[0x24] || D_800DD25A != d[0x25])
        {
            func_8009D3B0();
            if ((s16)func_800A2C34(NULL) == 0)
            {
                if ((s16)func_800A4474(d[0x24], d[0x25]) != 0)
                {
                    *(u16 *)(p + 0xA2) |= 0x200;
                }
                else
                {
                    *(u16 *)(p + 0xA2) &= 0xFDFF;
                }
            }
        }
        q3 = func_8009B25C(e, d[0x24], d[0x25], *(s16 *)(e + 0x88));
        if (q3 != NULL)
        {
            func_8009A180(q3, (u8 *)D_800814A8);
            *(s32 *)(q3 + 0x1C) &= 0x7FFFFFFF;
        }
        D_800DD258 = d[0x24];
        D_800DD25A = d[0x25];
    }
    q3 = *(u8 **)(e + 0x5C) + 0x20;
    ta = e;
    while (q3 != ta)
    {
        *(u16 *)(*(u8 **)(q3 - 0x14) + 0x14) &= 0xFFBF;
        q3 = *(u8 **)(q3 + 0x5C) + 0x20;
    }
    f9 = (u8 *)&D_80083460;
    if ((*(u16 *)(f9 + 2) & 0x800) != 0 && (s16)func_800A2C34(NULL) == 0 &&
        *(s16 *)(f9 + 8) == 0 && *(s16 *)(f9 + 0x14) == 0)
    {
        *(u16 *)(f9 + 2) = (*(u16 *)(f9 + 2) & 0xF7FF) | 0x2000;
    }
    f10 = (u8 *)&D_80083460;
    if ((*(u16 *)(f10 + 2) & 0x400) == 0)
    {
        if ((*(u16 *)(f10 + 2) & 0x2000) != 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    *(s16 *)(q3 + 0x8A) = *(u16 *)(q3 + 0x88);
                    *(s16 *)(q3 + 0x6A) = *(u16 *)(q3 + 0x2A);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                tc = e;
                do
                {
                    s1 = *(u8 **)(e + 0x5C);
                    q3 = s1 + 0x20;
                    cnt = 0;
                    while (q3 != tc)
                    {
                        D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                        if (*(s32 *)(s1 + 0x10) > 0)
                        {
                            fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                            fn1(s1 + 0x20, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                            if ((*(u16 *)(s1 + 0x1E) & 0x8000) == 0 && *(s8 *)(q3 + 0x71) < 0 &&
                                *(s8 *)(q3 + 0x6D) != 0)
                            {
                                cnt++;
                            }
                        }
                        s1 = D_800E3DE8;
                        q3 = s1 + 0x20;
                    }
                } while (cnt != 0);
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                ta = e;
                while (q3 != ta)
                {
                    func_800A9AFC(*(u8 **)(q3 - 0x14), q3);
                    q3 = *(u8 **)(q3 + 0x5C) + 0x20;
                }
                f11 = (u8 *)&D_80083460;
                *(u16 *)(f11 + 2) = (*(u16 *)(f11 + 2) & 0xDFFF) | 0x1000;
            }
            else
            {
                *(u16 *)(f10 + 2) &= 0xDFFF;
                q3 = *(u8 **)(e + 0x5C) + 0x20;
                while (q3 != e)
                {
                    s1 = q3 - 0x20;
                    D_800E3DE8 = *(u8 **)(q3 + 0x5C);
                    if (*(s32 *)(s1 + 0x10) > 0)
                    {
                        fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s1 + 0x10) | 0x80000000);
                        if (ta)
                        {
                            fn1(q3, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                        }
                        else
                        {
                            fn1(q3, *(s32 *)(s1 + 8), *(s32 *)(s1 + 0xC));
                        }
                    }
                    q3 = D_800E3DE8 + 0x20;
                }
                f12 = (u8 *)&D_80083460;
                *(u16 *)(f12 + 2) |= 0x2000;
            }
        }
    }
    fA = (u8 *)&D_80083460;
    i = 0;
    if (*(s32 *)(fA + 0x10) > 0)
    {
        *(s32 *)(fA + 0x10) = 0;
    }
    flagB[0] = 0;
    f13 = fA;
    *(u16 *)(f13 + 2) |= one;
    for (;;)
    {
        if ((D_800E296C & 0x100000) != 0 && (s16)func_800A2C34(NULL) == 0)
        {
            q8 = D_800E3D7C;
            do
            {
                n2 = (q8[0x41] << 8) | q8[0x42];
                q8[0x42] = 0;
                q8[0x41] = 0;
                *(s32 *)(q8 + 0x18) += n2;
                if ((s16)func_800A1C58(q8) != 0)
                {
                    *(u8 **)(f13 + 0xC) = q8;
                    goto walk;
                }
                q8 = *(u8 **)(q8 + 0x5C) + 0x20;
            } while (q8 != D_800E3D7C);
            D_800E296C &= 0xFFEFFFFF;
        }
    walk:
        w2 = D_800E3D7C;
        s3 = *(u8 **)(w2 + 0x5C);
        q8 = s3 + 0x20;
        cnt2 = 0;
        while (q8 != w2)
        {
            D_800E3DE8 = *(u8 **)(q8 + 0x5C);
            if (flagB[0] != 0 && *(s8 *)(q8 + 0x6D) == 0 && *(s16 *)(f13 + 0x14) == 0)
            {
                goto next;
            }
            if (*(s32 *)(s3 + 0x10) <= 0)
            {
                goto next;
            }
            r7 = (s16)func_800A2C34(NULL);
            if (*(s16 *)(f13 + 0x14) != 0)
            {
                if (r7 == 0)
                {
                    if ((*(u32 *)(s3 + 0xB8) & 0xFFFF0000) != 0x150000)
                    {
                        goto next;
                    }
                    *(u8 **)(f13 + 0xC) = s3 + 0x20;
                }
            }
            fn1 = (void (*)(u8 *, s32, s32))(*(u32 *)(s3 + 0x10) | 0x80000000);
            fn1(s3 + 0x20, *(s32 *)(s3 + 8), *(s32 *)(s3 + 0xC));
            if ((*(u16 *)(s3 + 0x1E) & 0x8000) != 0)
            {
                break;
            }
            if (*(s8 *)(q8 + 0x6D) != 0)
            {
                cnt2++;
            }
            if (r7 != 0 && (s16)func_800A2C34(NULL) == 0)
            {
                break;
            }
        next:
            if (f13)
            {
                s3 = D_800E3DE8;
            }
            else
            {
                s3 = D_800E3DE8;
            }
            q8 = s3 + 0x20;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x1000) != 0)
        {
            if (*(s16 *)(f13 + 8) == 0)
            {
                if (*(s16 *)(f13 + 4) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = (t1 & 0xEFF7) | 0x4000;
                if (cnt2 != 0)
                {
                    flagB[0] = one;
                    continue;
                }
            }
        }
        else if ((t1 & 0x400) != 0)
        {
            if ((t1 & 0x800) == 0 && (s16)func_800A2C34(NULL) == 0 &&
                (s16)func_800A2B28() == 0)
            {
                if (i == 0)
                {
                    i = one;
                    continue;
                }
                t2 = *(u16 *)(f13 + 2);
                t3 = t2 & 0xFBFF;
                *(u16 *)(f13 + 2) = t3;
                if ((t2 & 0x4000) != 0)
                {
                    return;
                }
                *(u16 *)(f13 + 2) = t3 | 0x800;
                if (func_800A5C70() != 0)
                {
                    *(u16 *)(f13 + 2) |= 0x80;
                }
                return;
            }
        }
        if (q8 == D_800E3D7C && cnt2 == 0)
        {
            *(u16 *)(f13 + 2) &= 0xFFFE;
        }
        t1 = *(u16 *)(f13 + 2);
        if ((t1 & 0x80) == 0 || *(s32 *)(f13 + 0xC) != 0 || *(s32 *)(f13 + 0x10) != 0 ||
            *(s16 *)(f13 + 0xA) != 0 || *(s16 *)(f13 + 8) == 0)
        {
            break;
        }
        *(u16 *)(f13 + 2) = t1 & 0xFFF7;
    }
}
