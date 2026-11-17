/* PARKED 2026-11-16, j58.  target 600 words, candidate 600 words,
   20 aligned differences (ovlverify masked: 20) -- EVERY INSTRUCTION
   POSITIONALLY IDENTICAL, and all twenty rows are register names only.

   This is a fresh decompilation; no earlier park of this function survived
   the 2026-11-16 cleanup.  Previous measured baseline on record (lever
   catalogue :27899) was 602 words / 63 differences, so this is 2 words and
   43 rows better and the word count is now exact.

   THE RESIDUE IS TWO REGISTER PAIRS AND NOTHING ELSE.

   (A) FIFTEEN ROWS, words 184..219: the angle block's two quantities are
   EXCHANGED.  `want` -- the s16 holding `(m[1] & 7) << 9` -- lands in $a1
   where the target has $a0, and the raw `*(u16 *)(d + 0x2A)` read lands in
   $a0 where the target has $a1.  Every instruction is the same instruction
   in the same slot: the three `andi`s on each, the join `addu` the ternary
   produces, the `sll 16`/`sra 16`, the `and` against -0x1000 and the final
   `slt`.
   THIS IS THE SAME RESIDUE, IN THE SAME CODE, AS docs/dungeon-parks/
   func_80096384.c -- read that header first.  Its diagnosis (global_alloc's
   `allocno_compare`, `(floor_log2(refs)*refs - live_length)/live_length`,
   `want` scoring -0.500 against the read's -0.111 because `floor_log2(7)`
   is 2) applies here unchanged, and its long "measured and rejected" list
   is a list of things NOT to retry here.

   (B) FIVE ROWS, words 321..329, switch case 0x50: the target keeps
   `m[0] & 0x60` and the `i * 4 + p` address in $v0 and only the shifted
   index `i` in $s0; this candidate coalesces all three into $s0.  Same
   instructions, same order.

   HOW IT GOT HERE: 80 -> 67 -> 58 -> 29 -> 23 -> 20, on five levers.

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

   (3) `s16 i` FOR CASE 0x50's INDEX, worth **6 rows** (29 -> 23).  As `s32`
   the whole address chain coalesces one register further.  `u8`, `u16` and
   a `(s32)` cast on the subscript are all identical to `s16`; `s8` and
   `s32` are both 26.

   (4) THE PERMUTER'S `do { ... } while (0)` FROM func_80096384's PARK
   TRANSFERS, worth **3 rows** (23 -> 20).  It wraps the two sign-extension
   arms, the `want` ternary, the `cur` read and the `delta` subtraction --
   exactly the block func_80096384.c wraps -- and it is the catalogued
   zero-trip device "A ZERO-TRIP `do { } while (0)` AROUND AN `if`".

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

   MEASURED AND REJECTED (all at 600 words / 20 unless the row says
   otherwise; the number is aligned differences):
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

   NOTE THE TWO 599/596-WORD ROWS: `s32 want` and the two-statement `want`
   are each SHORTER than the target, so they are not simply worse -- they
   remove an instruction the target has.  `want = m[1] & 7; want <<= 9;` at
   599 words / 19 rows is the closest thing to a lead left in this function
   and is where I would start a permuter run.

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
            do
            {
                if ((*(u16 *)(d + 0x2A) & 0x800) != 0)
                {
                    *(s16 *)(d + 0x2A) |= 0xF800;
                }
                else
                {
                    *(s16 *)(d + 0x2A) &= 0x7FF;
                }
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
            i = (m[0x0] & 0x60) >> 5;
            if (func_80098920(*(u8 **)(p + 0xAC + i * 4), func_8009FADC(m[0x0] & 0x1F), 0x15, 0) < 0)
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
