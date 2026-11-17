/* PARKED 2026-11-16, j72.  target 741 words, candidate 741 words,
   **2 aligned differences** (ovlverify masked: 2).  Every instruction is
   positionally identical; both rows are register names only.

   NOTE FOR THE DISPATCHER: this function was NOT untouched before today.
   An agent parked it on 2026-11-15 at 739/741 words with 19 aligned rows
   and its candidate was lost in the 2026-11-16 /tmp cleanup; seven entries
   in docs/matching-levers.md are written against it and
   docs/dungeon-sweep.md records that residue.  This body is a fresh write
   from the listing plus docs/dungeon-parks/func_8008EAC8.c, and it does
   NOT have that residue: the two words the old candidate was missing at
   0x8008AFB0 (the `andi $v0, $v0, 0xFF` and the `addu $v0, $a0, $zero` of
   the facing compare) are both present here, out of
   `want = (m[0x1] & 7) << 9;` with `s16 want` and
   `*(s16 *)(d + 0x2A) &= 0xFFF;` as the compare's left operand -- i.e.
   func_8008EAC8's spelling, unchanged.  Please UPDATE the
   "Open parks" bullet in docs/dungeon-sweep.md, which still says 739/19
   and "the saved body is `u8 i; want = i << 9`".

   WHAT IS LEFT, AND IT IS ONLY THIS:
     200  sll v0,a0,0x10    vs   sll  $v1, $a0, 16
     201  sra a1,v0,0x10    vs   sra  $a1, $v1, 16
   The SImode temp that holds `want << 16` on the way to its sign extension
   gets $v0 where the target has $v1.  This is now the SAME two-row
   residue, instruction for instruction, in THREE functions of this unit:
   docs/dungeon-parks/func_80096384.c (rows 136-137),
   docs/dungeon-parks/func_8008EAC8.c (rows 202-203) and this one.
   func_80096384's park carries j69's full local-alloc derivation of why it
   is stuck -- the block is `move / sll / sra / lh / lhu / subu / bgez` and
   closing it needs an insn scheduled strictly between the `sll` and the
   `lh` with the `subu` immediately after the `sra`, and the only movable
   insn in the block is the `lhu`, which is self-defeating.  Whatever
   closes one of the three should close all three; this one is the longest
   of them and is otherwise byte-exact.

   TEN OF j69's AND j50's SPELLINGS WERE RE-MEASURED HERE and behave
   exactly as their parks record, which is itself a datum -- the residue is
   a property of the construct, not of the function:
     cur read after `delta`, both inside the do-while      2  (a tie)
     an `s32 wext` local for the sign-extended value       2  (a tie)
     `(s32) *(s16 *)(d + 0x2A) - want`                     2  (a tie)
     `delta = -(want - *(s16 *)(d + 0x2A))`                2  (a tie)
     `u16 cur`                                             2  (a tie)
     `delta = (s16)cur - want`                             2  (a tie)
     `delta = *(s16 *)(d + 0x2A); delta -= want;`          swaps $a1/$a2
     cur read OUTSIDE (before) the do-while                8
     cur read OUTSIDE (after) the do-while                 6
     a nested do-while round `cur` + `delta`              10

   ===================== WHAT WAS LOAD-BEARING HERE ======================

   (1) `p[0x9A] = 0xE;` BEFORE `*(u16 *)(p + 0xA2) &= 0xFEFF;` -- the
   catalogue entry "A BYTE STORE TO A NEIGHBOURING FIELD KEEPS cse FROM
   FORWARDING A HALFWORD LOAD ACROSS A FOLLOWED BRANCH" (:27787) is written
   against this very function and is worth 2 words and 9 rows.  The emitted
   order is `lhu / sb / andi / sh`; do not "fix" it back to source order.

   (2) THE SWITCH ARMS ARE IN SOURCE ORDER 0x8/0xE0/0xE8/0xF0/0xF8 (one
   stacked group), then 0x10, 0x18, 0x20, 0x30, 0x48, 0x50, 0x58, 0x68,
   **0x88**, 0x70, 0x78, 0x80, 0x90, 0x98, 0xA0, 0xC8, 0xD8 -- NOT value
   order.  Read it off the listing's body addresses (0x8008B1B4 first, then
   B1D0, B1EC, B208, B220, B23C, B270, B2BC, B2E4, B310, B340, B360, ...).
   Case 0x88 must sit between 0x68 and 0x70 or jump.c will not cross-jump
   the `jal func_80094270` / `sw <idx>, 0x10($sp)` tail the two arms share.

   (3) *** CORRECTION TO docs/matching-levers.md :27987 ***, "TWO SWITCH
   ARMS CROSS-JUMP ONLY IF THEIR INDEX LANDS IN THE SAME REGISTER".  That
   entry's func_8008ACDC bullet says "one shared index local fixes it --
   worth 20 aligned differences".  With case 0x88 ADJACENT to case 0x68 and
   its pointer argument hoisted, THE OPPOSITE IS TRUE: case 0x68 writes its
   index inline in the call as `j = (m[0x0] & 0x60) >> 5` and case 0x88
   needs its OWN local `jj`.  Measured against this body, all 741 words:
       0x88 with its own local `jj`                        **7**
       0x88 sharing `j` with 0x68 (the entry's advice)      10
       0x88 as `j = m[0] & 0x60; j >>= 5;`                  10
       0x88 as `u = ...[(j = (m[0] & 0x60) >> 5)]`          10
       0x88 as `u = ((u8 **)(p + 0xD0))[j]`                 10
       0x88 as `j = (m[0] >> 5) & 3`                        11
       0x88 via a separate mask temp `n`                    13
   The entry's correction note for func_8008EAC8 (dated 2026-11-16) already
   says its seven index spellings were measured against the wrong arm
   order; the same applies to this bullet.

   (4) *** NEW MECHANISM, worth 5 rows, not in docs/matching-levers.md ***
   CASE 0x50's INDEX MUST BE A **GLOBAL** ALLOCNO, AND THE WAY TO BUY THAT
   IS TO REUSE THE SAME LOCAL IN A SECOND SWITCH ARM.
   The target is
       lbu $a0, 0x0($s0) / andi $v0, $a0, 0x60 / srl $s0, $v0, 5
       ... jal func_8009FADC ...
       sll $v0, $s0, 2 / addu $v0, $v0, $s1 / lw $a0, 0xAC($v0)
   i.e. the mask result and the whole address chain in $v0 and only the
   shifted index in $s0.  An index local used ONLY in this arm comes out
   with all four pseudos coalesced into $s0.  `local-alloc.c:1844`:
   combine_regs refuses to tie when `reg_qty[ureg] < 0`, which is exactly
   "UREG is not local to this block".  With the index local to one block,
   block_alloc ties the chain `mask -> i -> i*4 -> i*4+p` link by link into
   one quantity, that quantity crosses the `jal`, and so it is forced into
   a call-saved register and drags the three temporaries with it.  Give the
   local a reference in a SECOND basic block and local-alloc skips it
   entirely; global-alloc puts it in $s0 on its own and all three temps
   fall to $v0.
   Here the second home is case 0xC8, where `i = m[0x0] & 0x7F;` followed
   by `*(s16 *)(p + 0x96) = i;` is byte-identical to the inline store.
   Three second homes were measured and ALL THREE close the arm (741 words,
   7 -> 2 rows): case 0xC8's `& 0x7F` store, case 0x48's `m[0] & 0x1F`
   argument and case 0x70's `m[0] & 7` argument.  Measured and rejected,
   all 741 words, against the 7-row body:
       `s16 i` local to case 0x50 only (the obvious spelling)   7
       `u8 i`, `u16 i`, `((u8 **)(p + 0xAC))[i]`,
       `p + i * 4 + 0xAC`, `(s32)i * 4`, `(i << 2)`             7 each
       `i = m[0] & 0x60; i >>= 5;`                             13
       a separate mask temp, `s32 i`, `s8 i`                   13 each
       `i = (m[0] >> 5) & 3`                                    8
       sharing case 0x68's `j`                                 10 (right
           registers, wrong schedule: the `srl` lands after the `jal`)
       sharing case 0x88's `jj`                                breaks 0x88
       `i` reused at the function's own `*(s16 *)(p + 0x96) = 0`
           (same block count, still one block)                  7
   docs/dungeon-parks/func_8008EAC8.c has the IDENTICAL five-row residue in
   its own case 0x50 (its rows 321-329) and lists eight spellings that did
   not move it -- none of them made the index global.  This should close it.

   (5) THE FIVE func_8008B9FC CASES ARE ONE STACKED GROUP at the top of the
   switch.  Cross-jumping then produces the target's two entry points at
   0x8008B1B4 / 0x8008B1B8 by itself, and the same happens for the
   func_8008C7B4 body shared by case 0x10 and the earlier
   `(0x80013714 & 1) == 0 && (g->unk_08 & 0x80)` gate at 0x8008B1D0/B1D4.

   (6) `0x80013714` IS THE RAW CONSTANT ADDRESS at all three sites, as
   "WHY A RAW CONSTANT ADDRESS AND THE DECLARED SYMBOL ARE BYTE-IDENTICAL
   BUT NOT INTERCHANGEABLE" (:27853) already records against this function.

   (7) `Unk80083160 *g = &D_80083160;` AS A STRUCT POINTER, not the unit's
   majority `u8 *g = (u8 *)&D_80083160;`.  The target re-loads `g->unk_08`
   at five sites with a register base and a displacement of 8, which is
   what a varying-address struct ref gives once a store through `p` kills
   it (:26056).

   (8) THE ZERO-TRIP `do { ... } while (0)` around the `want` ternary, the
   `cur` read and the `delta` subtraction ONLY, with the halfword
   sign-extension if/else OUTSIDE it -- j69's lever, carried over from
   func_80096384.c and func_8008EAC8.c unchanged.  `want`'s own sign
   extension is the TERNARY (the `addu $a0, $v0, $zero` at 0x8008AFF8 is
   the `safe_from_p` copy of :27811); `cur`'s is an if/else.

   (9) `s16 e = *(u16 *)(p + 0xA4) + 1; *(s16 *)(p + 0xA4) = e;
   if (e >= 0x3D)` for the +0xA4 counter: the compare runs on the stored
   value through `sll`/`sra`, not on a reload.

   (10) `if (p[0x9A] != 0xE) { if (p[0x9A] != 0x17) { ... } ... }` as a
   NESTED pair, not an `&&`: the 0x17 test branches past the inner block
   onto the shared `*(s16 *)(p + 0x96) = 0;` / func_800A4300 tail.

   DECLARATIONS
   void func_8008B9FC(u8 *, void *, u8 *, u8 *);
   void func_8008C468(u8 *, void *, u8 *, u8 *);
   void func_8008C514(u8 *, void *, u8 *, u8 *);
   s32 func_8008C5C4(u8 *, UnkVec *, u8 *, u8 *);
   void func_8008C7B4(u8 *, void *, u8 *, u8 *);
   s16 func_8008C8BC(u8 *, UnkVec *, u8 *, u8 *);
   void func_8008CAA0(u8 *, void *, u8 *, u8 *);
   void func_8008CBA0(void *, void *, void *, u8 *);
   void func_8008CCB0(u8 *, void *, u8 *, u8 *);
   void func_8008CF6C(UnkDungeonActor *, void *, u8 *, u8 *);
   s32 func_8008D024(u8 *, void *, u8 *, s16, s16);
   s32 func_8008D1D0(u8 *, void *, u8 *, u8 *);
   void func_8008D7D0(u8 *, void *, u8 *, u8 *);
   void func_8008D94C(u8 *, void *, u8 *, u8 *);
   void func_80090200(u8 *, void *, u8 *, u8 *);
   s32 func_8009074C(s16, u16 *, u16 *);
   void func_80091B5C(u8 *, void *, u8 *, u8 *);
   s32 func_80094270(u8 *, void *, u8 *, u8 *, s16);
   void func_80094548(s16, s32);
   void func_8009456C(s16, u8);
   void func_8009458C(s16, u8);
   void func_800945C4(s16, s32);
   s32 func_80094EA4(void);
   s16 func_80095538(u8 *, s16, s16);
   void func_800956B8(u8 *, void *, void *, u8 *);
   s32 func_80095854(UnkDungeonActor *, void *, u8 *, u8 *);
   UnkNode *func_80098CF8(u8 *, void *, u8 *, u8 *);
   u8 *func_8009F868(void);
   u8 *func_8009F988(void);
   void func_8009FAAC(void);
   void *func_8009FADC(s16);
   s16 func_800A1C58(u8 *);
   s32 func_800A2C34(u8 *);
   void func_800A4300(u8 *, u8 *);
   s32 func_800A4474(s16, s16);
   extern u8 D_800245A8[];
   extern u8 D_80050CAC[];
   extern void (*D_800DD830[])(void);
   END DECLARATIONS

   Every one of those 35 prototypes is for a function DEFINED LOWER IN THE
   UNIT than func_8008ACDC's slot at csrc/ovl_dungeon.c:2515, so without
   them the body builds on implicit declarations and the `void` returns
   conflict.  Each prototype is copied from that function's own definition
   line.  D_800245A8 and D_800DD830 are also wanted by
   docs/dungeon-parks/func_8008EAC8.c and func_80096384.c, and D_80050CAC
   by func_8008EAC8.c; same spelling, so they merge.

   NO PROTOTYPE CHANGE: `void func_8008ACDC();` at the top of the unit is
   unprototyped and compatible with this definition.

   THE 35 PROTOTYPES BECOME VISIBLE TO EARLIER CALL SITES THAT CURRENTLY
   RELY ON AN IMPLICIT DECLARATION -- sixteen of them, in func_8008B870,
   func_8008D9F0, func_8008E504, func_800909A0, func_80090C10, func_80092018,
   func_800924F0, func_80094F74, func_80095538 and func_8009E038.  ALL TEN
   WERE RE-MEASURED with the declarations in place, and so was every other
   landed function in the unit: ovlverify over all 820 definitions in
   csrc/ovl_dungeon.c reports 0 differing for every one of them.
 */
void func_8008ACDC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    u8 *m;
    u8 *u;
    u8 *t;
    u8 *t2;
    s32 r;
    s32 n;
    s32 w;
    s32 w2;
    s32 j;
    s32 jj;
    s16 i;
    s16 want;
    s16 e;
    s32 cur;
    s32 delta;

    if (p[0x9A] != 0xE)
    {
        if (p[0x9A] != 0x17)
        {
            if ((*(u16 *)(p + 0xA2) & 0x100) != 0)
            {
                func_8008D94C(p, b, c, d);
                return;
            }
            p[0x9A] = 0xE;
            *(u16 *)(p + 0xA2) &= 0xFEFF;
            *(u16 *)(c + 0x14) &= 0xF7FF;
            p[0x9B] = 0;
            *(s16 *)(p + 0xA4) = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
        }
        *(s16 *)(p + 0x96) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)&D_80083460;
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
        func_80091B5C(p, b, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        func_8008C468(p, b, c, d);
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        func_80090200(p, b, c, d);
        return;
    }
    if ((s16)func_800A1C58(d) != 0)
    {
        if (func_8008D1D0(p, b, c, d) != 0)
        {
            return;
        }
    }
    if ((*(u16 *)(p + 0xA2) & 0x80) != 0)
    {
        func_8008D7D0(p, b, c, d);
        return;
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
        case 0xE0:
        case 0xE8:
        case 0xF0:
        case 0xF8:
            func_8008B9FC(p, b, c, d);
            return;
        case 0x10:
            func_8008C7B4(p, b, c, d);
            return;
        case 0x18:
            func_8008C8BC(p, b, c, d);
            return;
        case 0x20:
            func_8008C5C4(p, b, c, d);
            return;
        case 0x30:
            func_8008C514(p, b, c, d);
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
        case 0x58:
            func_80098CF8(p, b, c, func_8009FADC(m[0x0] & 0x1F));
            return;
        case 0x68:
            func_80094270(p, b, c, func_8009FADC(m[0x0] & 0x1F), j = (m[0x0] & 0x60) >> 5);
            return;
        case 0x88:
            jj = (m[0x0] & 0x60) >> 5;
            u = *(u8 **)(p + 0xD0 + jj * 4);
            func_80094270(p, b, c, u, jj);
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
                func_80095854((UnkDungeonActor *)p, b, c, u);
            }
            return;
        case 0xA0:
            func_8002534C(p, b, c, d);
            return;
        case 0xC8:
            i = m[0x0] & 0x7F;
            *(s16 *)(p + 0x96) = i;
            func_8008D368(p, b, c, D_800DD148, 2);
            return;
        case 0xD8:
            D_800DD830[m[0x0] & 0x7F]();
            return;
        }
        return;
    }
    D_800E3544 = 0;
    if ((*(u16 *)0x80013714 & 4) != 0)
    {
        if (D_800E3CD0 != 0)
        {
            return;
        }
        D_800E3CD0 = 1;
        func_80040AA0(3);
        return;
    }
    if ((s16)func_80094EA4() != 0)
    {
        *(s16 *)(p + 0xA4) = 0;
    }
    if ((*(u16 *)(p + 0xA2) & 0x200) != 0 && (s16)func_800A2C34(d) == 0 && (*(u16 *)(q + 0x2) & 0x4) == 0)
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
            func_8008B9FC(p, b, c, d);
            if (p[0x9A] != 0xE)
            {
                return;
            }
        }
        else
        {
            *(s16 *)(p + 0x96) = 0;
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
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        if ((g->unk_08 & 0x20) == 0)
        {
            if ((s16)func_8008C8BC(p, b, c, d) == 0)
            {
                return;
            }
            goto tail2;
        }
        else
        {
            if (func_8008C5C4(p, b, c, d) != 0)
            {
                return;
            }
        }
    }
    if ((*(u16 *)0x80013714 & 9) != 0)
    {
        *(s16 *)(p + 0xA4) = 0;
    }
    if (p[0x9A] == 0xE)
    {
        e = *(u16 *)(p + 0xA4) + 1;
        *(s16 *)(p + 0xA4) = e;
        if (e >= 0x3D)
        {
            *(s16 *)(p + 0xA4) = 0x3C;
            func_8008CCB0(p, b, c, d);
            return;
        }
    }
tail:
    if ((*(u16 *)(p + 0xA2) & 1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        t = D_800DD058;
        *(u8 **)(c + 0x2C) = t;
        func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    }
tail2:
    if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
    {
        t2 = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    }
}
