/* ovl_dng_b57, the banked room script's per-frame actor driver and the LARGEST
   BODY LEFT IN THE PROJECT. 1366 words. PARKED at
   1366 words / 95 differing (ovlverify) / 4 ALIGNED DIFFERENCES (aligndiff).

   THE WORD COUNT IS EXACT AND THE SHAPE IS SETTLED: 4 aligned rows out of
   1366 words, and every remaining ovlverify row is a REGISTER choice, not an
   instruction. The first measurement of this body was 1351 words / 1304
   differing; the route from there to here is the table below.

   WHAT THIS BODY IS. p + 0xB8 is a "a message is queued" latch and
   *(s32 *)(p + 0xB0) is a bitmask of "this line has already been said".
   The head syncs D_800FBE22/D_800FBE20 against D_800E3D7C's +0x9A, services
   the p + 0xB8 == 2 / == 3 states, and arms or tears down the D_800FBE54
   helper node. Then ONE outer gate (no cutscene, no queued message, the
   party standing exactly on the actor's tile) guards THIRTEEN trigger
   blocks, each `if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & BIT) == 0)' over a
   room id and the same tile test, each calling func_800353F4 with its own
   string and raising its own bit. The tail is the shared actor driver:
   func_800A9E70, the +0x8C continuation, the D_80175CC8 handler table, the
   facing re-dress, the ground-height tracking and the +0x92 lean.

   DONORS. dngcallee named func_b31f_8016F2E0 and func_b31f_8016B0E8
   (csrc/ovl_dng_b31f.c) at 12 of 48 symbols with a clean gap; dngtwin finds
   nothing at 0.70. func_b31f_8016F2E0 (299 words) is the TAIL donor and
   every spelling it states carries here: the `h'/`h2' handler locals, the
   `v'/`kk' narrowing copies of the `& 7' facing index, `s16 dir = (s8)w[0x6D]',
   `(*(s8 *)(p + 0x9D) * 5) << 14', the func_800BCB04 argument forms and the
   `q'-style alias. csrc/ovl_dng_b32.c:220-270 is the donor for the
   0x80010248 record walk and its "the base is the integer constant, never
   the declared symbol" rule holds here too.

   SIX FACTS THAT MOVED THIS BODY, measured one at a time:

   | spelling                                              | words | aligned |
   | as written below                                       | 1366 |    4    |
   | the three item-pool loops with a `tbl += 4' CURSOR     | 1366 |   13    |
   | ... with ONE cursor local shared by all three loops    | 1369 |   13    |
   | ... written as `goto' loops                            | 1358 |  103    |
   | ... indexed `*(u8 *)(0x80010249 + i * 4)'              | 1379 |  100    |
   | arm2 of the c + 0x14 & 0x8000 gate NOT inverted        | 1372 |   23    |
   | the +0x92 lean written `< -0x1F' first                 | 1369 |   13    |
   | block M's six room ids with a `>=/<=' range on the
     first three                                           | 1370 |   85    |
   | the three loop payloads INSIDE their loops             | 1360 |  107    |
   | the outer gate as ONE `&&' chain (e/g hoisted above it)| 1370 |   85    |

   (1) THE ITEM-POOL LOOPS MUST RECOMPUTE THE ADDRESS FROM THE COUNTER INTO A
   PER-LOOP LOCAL, NOT WALK A CURSOR, AND THE MECHANISM IS loop.c's giv
   rebasing. `tbl = (u8 *)(0x80010000 + i * 4);' inside the loop body gives
   `lui $v1, 0x8001' in the preheader -- ONE word, because 0x80010000's low
   half is zero -- with the two reads at MEM offsets 0x249 and 0x248 and
   `addiu $v1, $v1, 4' at the bottom, which is the target exactly. A cursor
   biv (`tbl = (u8 *)0x80010000' outside, `tbl += 4' inside) is ONE WORD
   LONGER PER LOOP: find_mem_givs records both MEM addresses as DEST_ADDR
   givs, combine_givs merges them, and the surviving giv's initial value
   becomes `init + min(offset)' = 0x80010248, which needs `lui' PLUS `ori'.
   The -da `.loop' dump says it in four lines:
       Insn 1509: dest address src reg 93 ... mult 1 add 585
       Insn 1515: dest address src reg 93 ... mult 1 add 584
       giv at 1509 combined with giv at 1515
       giv at 1515 reduced to (reg:SI 1162)
   AND THE LOCAL MUST BE PER-LOOP. With ONE `tbl' shared by all three loops
   the giv is not `replaceable' (the variable is read after the loop), so
   `benefit -= copy_cost' fires at loop.c:3955 and the dump prints
   `giv of insn 1515 not worth while, -10292 vs 14' -- no reduction at all,
   and the address is recomputed with `sll'/`addu' every iteration: 1369
   words. Three separate locals, 1366.

   (2) THE TWO ARMS OF THE `c + 0x14 & 0x8000' GATE HAVE OPPOSITE if/else
   POLARITY, worth 19 aligned rows and the placement of a 34-word block.
   Both arms end in the same `*(s32 *)(w + 0x1C) &= 0xF7FFFFFF' plus the same
   `if (& 0x40000)' two-way split, written out TWICE, and jump2 cross-joins
   the two copies of the no-0x40000 arm into one. Which copy survives, and
   where it lands, is decided by the polarity: the 0x8000-CLEAR arm must be
   `if (!= 0) { arc } else { ground }' and the 0x8000-SET arm must be
   `if (== 0) { ground } else { arc }'. Written the same way in both arms the
   merged block lands after the lean tails instead of between the second
   arm's test and its arc block: 1372 words / 23 rows.

   (3) THE +0x92 LEAN IS `>= -0x1F' FIRST, not `< -0x1F' first.
   `if (x >= -0x1F) { -= 8 } else if (x < -0x28) { += 8 }' is 0; the same
   three outcomes written `if (x < -0x1F) { if (x < -0x28) { += 8 } } else
   { -= 8 }' is 1369 words / 13 rows, because the `-= 8' store has to be the
   `then' arm for `slti'/`bnez' to come out in the target's order.

   (4) BLOCK M's SIX ROOM IDS MUST ALL BE WRITTEN AS EQUALITIES.
   `D_80082EA4 == 0x2C1E || == 0x2C1F || == 0x2C20 || == 0x2D1E || ...' lets
   fold merge the first three into `addiu -0x2C1E' / `sltiu 0x3' by itself.
   Spelling those three as `>= 0x2C1E && <= 0x2C20' produces the SAME range
   check plus a spurious `andi $v1, $v1, 0xFFFF' before the remaining `beq's,
   because the range test leaves the u16 read as a HImode value that then
   needs zero-extending: 1 word, and the `m = D_80082EA4' int local in the
   body below is the spelling that avoids it.

   (5) THE THREE ITEM-POOL PAYLOADS SIT ABOVE THE WHOLE TRIGGER CHAIN, as
   `goto' labels between the two arms of the `(p[0xB0] & 0x20000)' if.
   Written inside their loops (the natural `break' shape) the body is 1360
   words / 107 rows, with all three blocks ~2KB from where the ROM has them.
   The shape that reproduces the ROM is `if (...) { ...; goto after; }
   goto elsearm; hit1: ... goto done1; ... elsearm: ...; after:' -- jump.c
   collapses the `jump_if_not cond -> L1; L1: jump L_else' pair, which is what
   puts the three labelled blocks between the two arms.

   (6) THE OUTER GATE SPLITS AFTER THE func_800A2C34 TEST. As one `&&' chain
   with `e'/`g' assigned above it, the two symbol materialisations come out
   before the call instead of after it: 1370 words / 85 rows.

   WHAT IS LEFT, AND IT IS 95 REGISTER WORDS IN FOUR GROUPS. Every one is a
   register choice; `aligndiff' sees only 4 rows of it because it masks
   register fields.
     a. 2 rows, the prologue: `addu $s3, $s1, $zero' (w = p) and
        `addiu $s5, $sp, 0x10' (&t[0]) are TRANSPOSED. Both are independent
        single-set insns in the entry block and the order is sched1's.
        MEASURED FLAT: `w = p' first, `gg' first, `w = p' after either
        D_800FBE2x block, and `t' declared last -- all 1366 / 4.
     b. 63 rows, 9 of the 11 tile gates: the `%hi(D_80083780)' temp is $v0
        here and $v1 in the ROM, and the $v0/$v1 roles swap through the whole
        comparison with every instruction in place. The two gates that are
        CORRECT (blocks L and M, the two whose room test is on D_80082EA4
        rather than on `e + 0x24') are the ones where `e' and `g' are both
        assigned INSIDE the room test. In the ROM the `lui' lands in the room
        test's branch DELAY SLOT, which is why it cannot be $v0 there.
        MEASURED AND FLAT: nesting the two tile tests instead of `&&'
        (1366/4, NOT MEASURABLE), declaring `g' before `e', and moving the
        e/g declarations to the end of the block (both 1366/4).
        MEASURED AND WORSE: flipping the comparison operands (101), an `s32'
        temp for the e-side product (133), hoisting `g' next to `e' (90
        differing but 26 aligned rows), `g' written inline (1355 words / 58),
        both written inline (1364 / 90), `e' shared with `g' per-block (127),
        and the L/M pattern applied to all nine (1375 / 36).
     c. 11 rows in the +0x10000 block (func_800F6DFC / D_800FBD96): the `x'
        and `y' byte reads and the `sll 6' are one slot early.
     d. 8 rows in the two +0x9E/+0xA0 arc copies: `addiu $v0, $v1, 1' where
        the ROM has `addiu $v1, $v1, 1'.

   A NOTE FOR WHOEVER BUILDS THIS: the 16-byte `u16 t[8]' initializer is
   gcc's OWN CONSTANT POOL, emitted to `.rdata' as `$LC0'; splat calls it
   D_b57_80170804 and puts it in `.data' at 0x80170804, right after
   D_b57_80170800 (= &func_b57_801708AC). THE BODY NEEDS NO FILE-SCOPE RODATA
   OBJECT -- `relcand' reports `DATA in the listing, not in your code:
   D_b57_80170804' and that report is correct and expected. The initializer
   and its unaligned `lwl'/`lwr' + `swl'/`swr' block copy were confirmed
   byte-for-byte against a standalone probe before the body was written.

   DECLARATIONS
   extern u8 D_8006CCF8[];
   extern u8 D_8006EE9C[];
   extern u8 D_8006F47A[];
   extern u8 D_8006F50D[];
   extern u8 D_8006F6B9[];
   extern u8 D_8006F9D0[];
   extern u8 D_8006FD01[];
   extern u8 D_8006FFB7[];
   extern u8 D_800703D9[];
   extern u8 D_80070684[];
   extern u8 D_80070AF8[];
   extern u8 D_80070C11[];
   extern u8 D_80070E63[];
   extern u8 D_800FB3C8[];
   extern u8 D_800FB8DF[];
   extern u8 D_800FBAD7[];
   extern u8 D_800FBD96[];
   extern s16 D_800FBE20;
   extern s16 D_800FBE22;
   extern u8 *D_800FBE54;
   extern u8 *D_800E3D7C;
   extern u32 D_800E296C;
   extern void (*D_80175CC8[])(u8 *, UnkVec *, u8 *, u8 *);
   s32 func_800353F4(u32 v);
   s16 func_800A48F0(s8 *p, s8 kind, s8 c);
   u8 *func_800F6DFC(u8 *);
   s32 func_800A9E70(u8 *, void *, u8 *, u8 *);
   void func_80047738(u8 *, s16, s16);
   void func_800A020C(s32, u8 *);
   void func_800478B8(u8 *);
   s32 func_800BCB04w(s32, s32, s16) __asm__("func_800BCB04");
   END DECLARATIONS
   The unit already declares D_80082E80, D_80082EA4, D_80083462, D_8008346C,
   D_80083228, D_80175C30, func_8009C12C, func_80047784, func_800A2B04,
   func_800AD594, func_800A4ACC, func_800A56E0, func_800A2C34, func_800A9A0C,
   func_800AA36C, func_800F6D28, func_80042B68 and func_b57_80171FA4;
   D_80013714, D_80083160, D_80083780 and func_800644B8 come from common.h. */
void func_b57_80170A4C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u16 t[8] = {4, 5, 6, 7, 0, 1, 2, 3};
    u8 *gg;
    u8 *e1;
    u8 *g1;
    u8 *e2;
    u8 *g2;
    u8 *e3;
    u8 *g3;
    u8 *e4;
    u8 *g4;
    u8 *e5;
    u8 *g5;
    u8 *e6;
    u8 *g6;
    u8 *e7;
    u8 *g7;
    u8 *e8;
    u8 *g8;
    u8 *e9;
    u8 *g9;
    u8 *e10;
    u8 *g10;
    u8 *e11;
    u8 *g11;
    u8 *e12;
    u8 *g12;
    u8 *e13;
    u8 *g13;
    u8 *n;
    u8 *e;
    u8 *tbl1;
    u8 *tbl2;
    u8 *tbl3;
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    void (*h2)(u8 *, UnkVec *, u8 *, u8 *);
    s32 k;
    s32 v;
    u8 *w;
    s32 r;
    s32 i;
    s32 x;
    s32 y;
    s16 dir;
    s16 kk;
    s16 dy;
    u16 nn;
    s32 m;

    w = p;
    gg = (u8 *)&D_80083160;
    if (D_800FBE22 != D_800FBE20)
    {
        D_800FBE22 = D_800FBE20;
    }
    if (D_800FBE20 != D_800E3D7C[0x9A])
    {
        D_800FBE20 = D_800E3D7C[0x9A];
    }
    if (p[0xB8] == 2)
    {
        k = func_800F6D28(b);
        *(s16 *)(p + 0x2A) = k << 9;
        if (p[0xB8] == 2)
        {
            e = D_800E3D7C;
            if (e[0x9A] == 0xE)
            {
                *(s16 *)(e + 0x2A) = t[k & 7] << 9;
            }
        }
    }
    if (p[0xB8] == 3)
    {
        p[0xB8] = 0;
        func_800AA36C(p, b, c, w);
    }
    if (p[0xB8] != 0)
    {
        func_800A9A0C(w);
        if ((D_80083462 & 0x2000) != 0)
        {
            w[0x71] &= 0x7F;
        }
    }
    if (D_80082EA4 == 0x2536 || D_80082EA4 == 0x2537)
    {
        *(s32 *)(p + 0xB0) |= 0x20000;
    }
    if ((*(s32 *)(p + 0xB0) & 0x20000) == 0)
    {
        if (D_800FBE54 != NULL)
        {
            func_800A48F0((s8 *)(D_800FBE54 + 0x20), 1, 0xA);
        }
        goto after20000;
    }
    goto else20000;
hit1:
    func_800353F4((u32)D_8006F6B9);
    w[0x6D] = 0;
    p[0x9B] = 0;
    p[0xB8] = 1;
    *(s32 *)(p + 0xB0) |= 0x10;
    goto done1;
hit2:
    func_800353F4((u32)D_8006FFB7);
    w[0x6D] = 0;
    p[0x9B] = 0;
    p[0xB8] = 1;
    *(s32 *)(p + 0xB0) |= 0x80;
    goto done2;
hit3:
    func_800353F4((u32)D_800703D9);
    w[0x6D] = 0;
    p[0x9B] = 0;
    p[0xB8] = 1;
    *(s32 *)(p + 0xB0) |= 0x100;
    goto done3;
else20000:
    if (D_800FBE54 != NULL)
    {
        func_80042B68((s8 *)(D_800FBE54 + 0x20), 1);
        D_800FBE54 = NULL;
    }
after20000:
    if ((D_80013714 & 0x1) == 0 && p[0xB8] == 0 && (D_80083462 & 0x2000) == 0 &&
        (s16)func_800A2C34(w) == 0)
    {
        e1 = D_80082E80;
        g1 = D_80083780;
        if (e1[0x24] * 0x40 + 0x20 == *(s16 *)(g1 + 0x2) &&
            e1[0x25] * 0x40 + 0x20 == *(s16 *)(g1 + 0x6) && D_800E3D7C[0x9A] != 0x19 &&
            D_800E3D7C[0x9A] != 0x1B && (*(s32 *)(w + 0x1C) & 0x80000) == 0)
        {
        if (p[0xB9] == 0 && (D_80013714 & 0x1) == 0)
        {
            p[0xB9] = 1;
            func_800353F4((u32)D_8006EE9C);
            w[0x6D] = 0;
            p[0x9B] = 0;
            p[0xB8] = 2;
            k = func_800F6D28(b);
            e = D_800E3D7C;
            *(s16 *)(w + 0x2A) = k << 9;
            if (e1[0x9A] == 0xE)
            {
                *(s16 *)(e + 0x2A) = t[k & 7] << 9;
            }
            D_800FBE54 = func_800F6DFC(w);
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x20) == 0)
        {
            e2 = D_80082E80;
            if (*(u16 *)(e2 + 0x24) == 0x519 || *(u16 *)(e2 + 0x24) == 0x619)
            {
                g2 = D_80083780;
                if (e2[0x24] * 0x40 + 0x20 == *(s16 *)(g2 + 0x2) && e2[0x25] * 0x40 + 0x20 == *(s16 *)(g2 + 0x6))
                {
                    func_800353F4((u32)D_8006F9D0);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x20;
                }
            }
            if (D_800E3D7C[0x9A] != 0x19 && D_800E3D7C[0x9A] != 0x1B &&
                (D_800FBE22 == 0x1B || D_800FBE22 == 0x1F || D_800FBE22 == 0x20 ||
                 D_800FBE20 == 0x1B || D_800FBE20 == 0x1F || D_800FBE20 == 0x20))
            {
                func_800353F4((u32)D_8006F9D0);
                w[0x6D] = 0;
                p[0x9B] = 0;
                p[0xB8] = 1;
                *(s32 *)(p + 0xB0) |= 0x20;
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x4000) == 0)
        {
            e3 = D_80082E80;
            if (*(u16 *)(e3 + 0x24) == 0x2B2D || *(u16 *)(e3 + 0x24) == 0x2C2D)
            {
                g3 = D_80083780;
                if (e3[0x24] * 0x40 + 0x20 == *(s16 *)(g3 + 0x2) && e3[0x25] * 0x40 + 0x20 == *(s16 *)(g3 + 0x6))
                {
                    func_800353F4((u32)D_800FB8DF);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x4000;
                }
            }
            if (D_800E3D7C[0x9A] == 0x1C)
            {
                func_800353F4((u32)D_800FB8DF);
                w[0x6D] = 0;
                p[0x9B] = 0;
                p[0xB8] = 1;
                *(s32 *)(p + 0xB0) |= 0x4000;
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x200) == 0)
        {
            e4 = D_80082E80;
            if (*(u16 *)(e4 + 0x24) >= 0xB36 && *(u16 *)(e4 + 0x24) <= 0xB37)
            {
                g4 = D_80083780;
                if (e4[0x24] * 0x40 + 0x20 == *(s16 *)(g4 + 0x2) && e4[0x25] * 0x40 + 0x20 == *(s16 *)(g4 + 0x6))
                {
                    func_800353F4((u32)D_80070684);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x200;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x8000) == 0)
        {
            e5 = D_80082E80;
            if (*(u16 *)(e5 + 0x24) >= 0x1236 && *(u16 *)(e5 + 0x24) <= 0x1237)
            {
                g5 = D_80083780;
                if (e5[0x24] * 0x40 + 0x20 == *(s16 *)(g5 + 0x2) && e5[0x25] * 0x40 + 0x20 == *(s16 *)(g5 + 0x6))
                {
                    func_800353F4((u32)D_800FBAD7);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x8000;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x2) == 0)
        {
            e6 = D_80082E80;
            if (*(u16 *)(e6 + 0x24) >= 0x1D07 && *(u16 *)(e6 + 0x24) <= 0x1D08)
            {
                g6 = D_80083780;
                if (e6[0x24] * 0x40 + 0x20 == *(s16 *)(g6 + 0x2) && e6[0x25] * 0x40 + 0x20 == *(s16 *)(g6 + 0x6))
                {
                    func_800353F4((u32)D_8006F47A);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x2;
                }
            }
            if (D_800E3D7C[0x9A] != 0x19 && (*(s32 *)(gg + 0x10) & 0x20) != 0)
            {
                func_800353F4((u32)D_8006F47A);
                w[0x6D] = 0;
                p[0x9B] = 0;
                p[0xB8] = 1;
                *(s32 *)(p + 0xB0) |= 0x2;
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x4) == 0)
        {
            e7 = D_80082E80;
            if (*(u16 *)(e7 + 0x24) >= 0x291E && *(u16 *)(e7 + 0x24) <= 0x2920)
            {
                g7 = D_80083780;
                if (e7[0x24] * 0x40 + 0x20 == *(s16 *)(g7 + 0x2) && e7[0x25] * 0x40 + 0x20 == *(s16 *)(g7 + 0x6))
                {
                    func_800353F4((u32)D_8006F50D);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x4;
                }
            }
            if (D_800E3D7C[0x9A] != 0x19 && (*(s32 *)(gg + 0x8) & 0x10) != 0)
            {
                func_800353F4((u32)D_8006F50D);
                w[0x6D] = 0;
                p[0x9B] = 0;
                p[0xB8] = 1;
                *(s32 *)(p + 0xB0) |= 0x4;
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x10) == 0)
        {
            if (D_800E3D7C[0x9A] != 0x23)
            {
                for (i = 0; i < 0x14; i++)
                {
                    tbl1 = (u8 *)(0x80010000 + i * 4);
                    if (tbl1[0x249] == 0xF && tbl1[0x248] == 2)
                    {
                        goto hit1;
                    }
                }
            }
        }
    done1:
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x80) == 0)
        {
            if (D_800E3D7C[0x9A] != 0x23)
            {
                for (i = 0; i < 0x14; i++)
                {
                    tbl2 = (u8 *)(0x80010000 + i * 4);
                    if (tbl2[0x249] == 0x12 && tbl2[0x248] == 0x15)
                    {
                        goto hit2;
                    }
                }
            }
        }
    done2:
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x100) == 0)
        {
            if (D_800E3D7C[0x9A] != 0x23)
            {
                for (i = 0; i < 0x14; i++)
                {
                    tbl3 = (u8 *)(0x80010000 + i * 4);
                    if (tbl3[0x249] == 0x4 && tbl3[0x248] == 0x1)
                    {
                        goto hit3;
                    }
                }
            }
        }
    done3:
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x40) == 0)
        {
            e8 = D_80082E80;
            if (*(u16 *)(e8 + 0x24) >= 0x161C && *(u16 *)(e8 + 0x24) <= 0x1622)
            {
                g8 = D_80083780;
                if (e8[0x24] * 0x40 + 0x20 == *(s16 *)(g8 + 0x2) && e8[0x25] * 0x40 + 0x20 == *(s16 *)(g8 + 0x6))
                {
                    func_800353F4((u32)D_8006FD01);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x40;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x400) == 0)
        {
            e9 = D_80082E80;
            if (*(u16 *)(e9 + 0x24) >= 0x2036 && *(u16 *)(e9 + 0x24) <= 0x2037)
            {
                g9 = D_80083780;
                if (e9[0x24] * 0x40 + 0x20 == *(s16 *)(g9 + 0x2) && e9[0x25] * 0x40 + 0x20 == *(s16 *)(g9 + 0x6))
                {
                    func_800353F4((u32)D_80070AF8);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x400;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x800) == 0)
        {
            if (D_80082EA4 == 0x31E || D_80082EA4 == 0x41E || D_80082EA4 == 0x51E ||
                D_80082EA4 == 0x61E || D_80082EA4 == 0x71E || D_80082EA4 == 0x81E)
            {
                e10 = D_80082E80;
                g10 = D_80083780;
                if (e10[0x24] * 0x40 + 0x20 == *(s16 *)(g10 + 0x2) && e10[0x25] * 0x40 + 0x20 == *(s16 *)(g10 + 0x6))
                {
                    func_800353F4((u32)D_80070C11);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x800;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x1000) == 0)
        {
            m = D_80082EA4;
            if (m == 0x2C1E || m == 0x2C1F || m == 0x2C20 || m == 0x2D1E || m == 0x2D1F ||
                m == 0x2D20)
            {
                e11 = D_80082E80;
                g11 = D_80083780;
                if (e11[0x24] * 0x40 + 0x20 == *(s16 *)(g11 + 0x2) && e11[0x25] * 0x40 + 0x20 == *(s16 *)(g11 + 0x6))
                {
                    func_800353F4((u32)D_80070E63);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x1000;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x2000) == 0)
        {
            e12 = D_80082E80;
            if (*(u16 *)(e12 + 0x24) >= 0x121C && *(u16 *)(e12 + 0x24) <= 0x1222)
            {
                g12 = D_80083780;
                if (e12[0x24] * 0x40 + 0x20 == *(s16 *)(g12 + 0x2) && e12[0x25] * 0x40 + 0x20 == *(s16 *)(g12 + 0x6))
                {
                    func_800353F4((u32)D_800FB3C8);
                    w[0x6D] = 0;
                    p[0x9B] = 0;
                    p[0xB8] = 1;
                    *(s32 *)(p + 0xB0) |= 0x2000;
                }
            }
        }
        if (p[0xB8] == 0 && (*(s32 *)(p + 0xB0) & 0x10000) == 0)
        {
            if (D_800FBE54 == NULL)
            {
                D_800FBE54 = func_800F6DFC(w);
                if (D_800FBE54 == NULL)
                {
                    goto tail;
                }
            }
            e13 = D_80082E80;
            n = *(u8 **)(D_800FBE54 + 0xC);
            x = e13[0x24];
            if (__builtin_abs(x - n[0x24]) < 4)
            {
                y = e13[0x25];
                if (__builtin_abs(y - n[0x25]) < 4)
                {
                    g13 = D_80083780;
                    if (x * 0x40 + 0x20 == *(s16 *)(g13 + 0x2) && y * 0x40 + 0x20 == *(s16 *)(g13 + 0x6))
                    {
                        func_800353F4((u32)D_800FBD96);
                        w[0x6D] = 0;
                        p[0x9B] = 0;
                        p[0xB8] = 1;
                        *(s32 *)(p + 0xB0) |= 0x10000;
                    }
                }
            }
        }
        }
    }
tail:
    if (p[0xB8] != 0)
    {
        D_800E296C |= 0x200000;
    }
    else
    {
        D_800E296C &= 0xFFDFFFFF;
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
        if (h == (void (*)(u8 *, UnkVec *, u8 *, u8 *))func_b57_80171FA4)
        {
            h(p, b, c, w);
        }
        else
        {
            w[0x71] &= 0x7F;
        }
        return;
    }
    dir = (s8)w[0x6D];
    if (func_800A9E70(p, b, c, w) != 0)
    {
        return;
    }
    h2 = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h2 != NULL)
    {
        h2(p, b, c, w);
    }
    D_80175CC8[p[0x9A]](p, b, c, w);
    if (dir != *(s8 *)(w + 0x6D))
    {
        func_800AA36C(p, b, c, w);
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(s32 *)(w + 0x1C) & 0x40000) == 0 && (*(u16 *)(p + 0x98) & 0x8) == 0)
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    else
    {
        p[0x9D] = 0;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    v = ((D_80083228 + *(s16 *)(w + 0x2A) + 0x100) >> 9) & 7;
    kk = v;
    if (*(s16 *)(p + 0x94) != kk)
    {
        func_80047738(c, (*(u8 **)(c + 0x2C))[kk], *(s8 *)(c + 0x4));
        *(s16 *)(p + 0x94) = v;
    }
    if (D_8006CCF8[kk] != 0)
    {
        *(u16 *)(c + 0x14) |= 0x1;
    }
    else
    {
        *(u16 *)(c + 0x14) &= 0xFFFE;
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        func_800A020C(*(s32 *)(w + 0x1C), c + 0xC);
        if ((*(s32 *)(w + 0x1C) & 0x20) == 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0)
            {
                func_800478B8(c);
            }
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
            *(s32 *)(w + 0x1C) &= 0xFFFBFFFF;
        }
        *(s32 *)(w + 0x1C) &= 0xF7FFFFFF;
        if ((*(s32 *)(w + 0x1C) & 0x40000) != 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_80175C30)
            {
                if (*(u16 *)(c + 0x4) == 0x100)
                {
                    *(s16 *)(p + 0x9E) = 0;
                    *(s32 *)(p + 0xA0) = 0;
                }
                if (c[0x4] >= 1 && c[0x4] <= 4)
                {
                    nn = *(u16 *)(p + 0x9E);
                    *(s16 *)(p + 0x9E) = nn + 1;
                    *(s32 *)(p + 0xA0) += func_800644B8((s16)nn * 0xAA) * 0x20;
                }
                else
                {
                    nn = *(u16 *)(p + 0x9E);
                    *(s16 *)(p + 0x9E) = nn + 1;
                    *(s32 *)(p + 0xA0) += func_800644B8((s16)nn * 0xAA) * 0x40;
                }
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                if (*(s16 *)(p + 0x92) >= -0x1F)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 0x8;
                }
                else if (*(s16 *)(p + 0x92) < -0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 0x8;
                }
            }
        }
        else
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
            *(s16 *)(p + 0x9E) = 0;
            *(s32 *)(p + 0xA0) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04w(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(w + 0x88) - 0x20);
                dy = r - *(u16 *)(w + 0x88);
                if (dy < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = dy;
                    p[0x9D] = 0;
                    b->unk_14 = 0;
                    *(s32 *)(w + 0x1C) |= 0x8000000;
                }
            }
        }
    }
    else
    {
        if ((*(u16 *)(c + 0x14) & 0x800) != 0)
        {
            *(u16 *)(c + 0x14) &= 0x8FFF;
        }
        else
        {
            *(u16 *)(c + 0x14) |= 0x7000;
        }
        *(s32 *)(w + 0x1C) &= 0xF7FFFFFF;
        if ((*(s32 *)(w + 0x1C) & 0x40000) == 0)
        {
            *(s32 *)(p + 0x90) -= *(s32 *)(p + 0xA0);
            *(s16 *)(p + 0x9E) = 0;
            *(s32 *)(p + 0xA0) = 0;
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                r = func_800BCB04w(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(w + 0x88) - 0x20);
                dy = r - *(u16 *)(w + 0x88);
                if (dy < *(s16 *)(p + 0x92))
                {
                    *(s16 *)(p + 0x92) = dy;
                    p[0x9D] = 0;
                    b->unk_14 = 0;
                    *(s32 *)(w + 0x1C) |= 0x8000000;
                }
            }
        }
        else
        {
            if ((*(u16 *)(c + 0x14) & 0x40) == 0 && *(u8 **)(c + 0x2C) == D_80175C30)
            {
                if (*(u16 *)(c + 0x4) == 0x100)
                {
                    *(s16 *)(p + 0x9E) = 0;
                    *(s32 *)(p + 0xA0) = 0;
                }
                if (c[0x4] >= 1 && c[0x4] <= 4)
                {
                    nn = *(u16 *)(p + 0x9E);
                    *(s16 *)(p + 0x9E) = nn + 1;
                    *(s32 *)(p + 0xA0) += func_800644B8((s16)nn * 0xAA) * 0x20;
                }
                else
                {
                    nn = *(u16 *)(p + 0x9E);
                    *(s16 *)(p + 0x9E) = nn + 1;
                    *(s32 *)(p + 0xA0) += func_800644B8((s16)nn * 0xAA) * 0x40;
                }
            }
            if ((*(u16 *)(p + 0x98) & 0x8) == 0)
            {
                if (*(s16 *)(p + 0x92) >= -0x1F)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) - 0x8;
                }
                else if (*(s16 *)(p + 0x92) < -0x28)
                {
                    *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 0x8;
                }
            }
        }
    }
    if ((*(s32 *)(w + 0x1C) & 0x40000000) != 0)
    {
        *(s32 *)(w + 0x1C) &= 0xBFFFFFFF;
        r = func_800BCB04w((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(w + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            *(u16 *)(p + 0x92) += *(u16 *)(w + 0x88) - r;
            *(u16 *)(w + 0x88) = r;
        }
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(w + 0x88) + *(u16 *)(p + 0x92) - *(u16 *)(p + 0xA2);
    *(u16 *)(c + 0x14) |= 0x40;
}
