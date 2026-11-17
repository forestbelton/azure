/* PARKED 2026-11-16, j50; j69 took it from 15 aligned rows to 2 the same day.
   target 397 words, candidate 397 words, **2 aligned differences**.

   WHAT IS LEFT, AND IT IS ONLY THIS:
     136  sll v0,a0,0x10   vs   sll $v1, $a0, 16
     137  sra a1,v0,0x10   vs   sra $a1, $v1, 16
   The SImode temp that holds `want << 16` on the way to its sign extension
   (`.lreg` pseudo 181) gets $v0 where the target has $v1.  Everything else --
   including the $a0/$a1 pair that was the whole of the old residue -- is
   byte-identical, and the function is the right length.

   ================= HOW THE 15 ROWS WENT (j69, ONE COMPILE) =================

   j50's diagnosis named the right pass and the wrong cure.  `allocno_compare`
   scores `floor_log2(refs) * refs / live_length`, and with j50's permuted
   `do { } while (0)` in place the real numbers (tools/decomp/dump.sh, -G0, on
   a standalone file of the unit's declaration block + this body) were:

     98  = want                      12 refs / 28 insns -> 12857   got $a1
     154 = the raw *(u16 *)(d + 0x2A) 6 refs /  9 insns -> 13333   got $a0
     155 = the `& 0xFFF` result       5 refs /  7 insns -> 14285   got $v1

   so `want` had to land strictly between 154 and 155, i.e. in [13333, 14285)
   -- a tie at 13333 goes to `want` on `allocno_compare`'s `v1 - v2`
   allocno-number tie-break.

   The cure is NOT to buy `want` a reference.  **`flow` adds `loop_depth`, not
   1, per reference**, so the `do { } while (0)`'s LOOP notes were DOUBLE-
   COUNTING the two references to the raw halfword that happened to sit inside
   them:

       *(s16 *)(d + 0x2A) |= 0xF800;   /   *(s16 *)(d + 0x2A) &= 0x7FF;

   Moving that if/else OUT of the `do { } while (0)` -- leaving only the
   `want` ternary, `cur` and `delta` inside -- takes 154 from 6 refs to 4
   (8888) and leaves 98 at 12 refs / 28 (12857).  The pair flips, and with it
   all thirteen of the rows that fell out of it.  15 -> 2, first compile.

   **THE `do { } while (0)` IS STILL LOAD-BEARING**: it is what keeps the
   three references to `want` inside it weighted at depth 2, which is what
   holds 98 at 12 refs rather than 7.  Do not delete it, and do not put the
   halfword if/else back inside it.

   ============== WHAT IS LEFT IS A local-alloc QTY INVERSION ===============

   Both survivors are LOCAL quantities of the basic block that starts at
   .L800965A0 (the ternary's join copy) and ends at the `bgez`:

     181 = want << 16   4 refs, born at the `sll`, dies at the `sra`
     177 = the `lh`     4 refs, born at the `lh`,  dies at the `subu`

   `local-alloc.c`'s `QTY_CMP_PRI` is
   `floor_log2(n_refs) * n_refs * size / (qty_death - qty_birth)`, with
   birth = `2*insn + 1` for a set and death = `2*insn` for a last use
   (local-alloc.c:382, :2126, :1693).  In the sched1 order this body produces
   -- `join, sll, sra, lh, lhu, subu, bgez` -- 181's span is 1 and 177's is 3,
   so 181 sorts first, takes the first free register ($v0), and 177 reuses
   $v0 after it dies.  The target needs the opposite: 177 allocated FIRST
   (taking $v0) with 181 CONFLICTING with it, so that 181 is pushed to $v1.

   THE ARITHMETIC OF WHAT WOULD DO IT.  With a = pos(sll), b = pos(lh),
   c = pos(sra), d = pos(subu) in the post-sched1 insn order, the two
   requirements are

       overlap:        b < c
       177 sorts first: d - b < c - a

   which together force **b >= a + 2 and d = c + 1**: at least one insn
   strictly between the `sll` and the `lh`, and the `subu` immediately after
   the `sra`.  The only other insn available in the block is the `lhu` that
   loads `cur`, and putting it there is self-defeating: `cur` is a global
   allocno, `global_conflicts` would then see it live across 181's $v1, and
   the target's own `lhu $v1` at word 139 proves it is not.  Every other
   ordering either ties (and the tie goes to 181, which is born first and so
   has the lower qty number) or removes the overlap.

   So the lever, if there is one, is a ZERO-WORD INSN SCHEDULED BETWEEN THE
   `sll` AND THE `lh`, with the `lhu` after the `subu`.  Note which half of
   that is already reachable: `__asm__("" : "+r"(delta))` after the
   subtraction moves the `lhu` to the end of the block all by itself (order
   becomes sll, lh, sra, subu, lhu) -- it just also puts the `lh` at a+1, so
   the two spans tie at 3 and 181 keeps $v0.  That spelling is 6 rows, not 2,
   but it is the half of the mechanism that works.

   A no-op `use` cannot be that insn: it carries priority 1 while the `sll`
   carries `adjust_priority`'s birthing boost (0x7f000001), so the scheduler
   always emits it ahead of the `sll`, never between.

   ====================== DECLARATIONS THIS BODY NEEDS ======================
       extern u8 D_800245A8[];
       extern void (*D_800DD830[])(void);
   NO prototype change: `void func_80096384();` in csrc/ovl_dungeon.c is
   unprototyped and compatible with this definition.

   ============== SPELLINGS THAT ARE ALREADY LOAD-BEARING HERE ==============
   (all measured by j50; do not "simplify" any of them)
   (1) `s16 want;` -- the catalogue's `:25519` entry.  As `s32` the byte folds
       and the function is 396 words; the narrow destination keeps the
       surviving `andi 0xFF`, buys the `addu $v0, $a0, $zero` copy, and raises
       nonzero_bits(want) past 0xFFFF so that `want & ~0xFFF` comes out as the
       target's `addiu $v0, $zero, -0x1000` + `and` instead of one `andi`.
   (2) THE SWITCH ARMS IN VALUE ORDER 8, 0x10, 0x18, 0xD8 -- `:27053`, worth
       10 differing.
   (3) THE `want` SIGN EXTENSION IS A TERNARY, NOT AN IF/ELSE (398 words as an
       if/else).  The SAME construct on `*(s16 *)(d + 0x2A)` must stay an
       if/else, because there the common store is the join.
   (4) `*(s16 *)(d + 0x2A) &= 0xFFF;` as a COMPOUND ASSIGNMENT, not through an
       `n` local: 26 -> 23 differing.
   (5) ONE POINTER LOCAL PER D_800DD274 ARM (`t` at the head, `t2` in the
       tail): 6 differing.
   (6) `cur = *(u16 *)(d + 0x2A);` read BEFORE the `delta` subtraction: 3
       differing.

   ==================== MEASURED AND REJECTED (j50) =========================
   (all 397 words / 17 differing at the time unless stated)
     `want = m[1] & 7; want <<= 9;`                       396 words
     `(s16)((m[0x1] & 7) << 9)`                           no change
     `want` and the compare operands swapped              18
     `(*(u16 *)(d + 0x2A) & 0xFFF) != want`               no change
     `want < *(s16 *)(d + 0x2A)` for the `>` test         no change
     a `wext` local for the sign-extended value           no change
     an explicit `cur0` local for the raw halfword        no change
     `cur0` reused for the `& 0xFFF` in the `> 0x800` arm 396 words
     `cur` placed after `delta`, or after the abs         17 / 20
     `delta >= 0x801` instead of `> 0x800`                no change
     `u16 cur`, `s16 delta`, six declaration orders       no change

   ============ MEASURED AND REJECTED (j69, against the 2-row body) =========
   Each of these is a whole-function aligndiff against the body below.
     cur read after `delta`, both still inside the do-while      2  (a tie)
     `delta = -(want - *(s16 *)(d + 0x2A))`                      2  (a tie)
     `delta = *(s16 *)(d + 0x2A) - (s32)want`                    2  (a tie)
     `u16 cur`                                                   2  (a tie)
     `__asm__("" : "+r"(want))` after the ternary                2  (a tie)
     `__asm__("" : "+r"(want))` before the subtraction           2  (a tie)
     `__asm__("" : "+r"(delta))` after the subtraction           6
     ... with either `+r(want)` or `"r"(want)` as well           6
     `__asm__("" : "+r"(cur))` after the cur read                10
     `__asm__("" : "+r"(cur))` + `+r(delta)`                     10
     `"r"(cur)` before the subtraction + `+r(delta)`             13, 398 words
     cur read OUTSIDE (before) the do-while                      8
     `delta` computed OUTSIDE (after) the do-while               10
     do-while around the `want` ternary ONLY                     10
     do-while nested round `cur`+`delta`                         10
     do-while nested round `delta` only                          10
     the `if (delta < 0)` abs pulled INSIDE the do-while         20, 403 words
     `delta = *(s16 *)(d + 0x2A); delta -= want;`                20
     `s16 delta`                                                 41, 400 words
   The ones marked "a tie" leave exactly the two rows above; they are
   alternative spellings of the same allocation, not progress.
 */

void func_80096384(u8 *p, void *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    void **t;
    void **t2;
    s32 cur;
    u8 *m;
    u16 k;
    s32 e;
    s32 f;
    s32 w;
    s32 n;
    s32 r;
    s32 delta;
    s16 want;
    if (p[0x9A] != 0x32)
    {
        t = D_800DD274;
        *((void ***)(c + 0x2C)) = t;
        func_8003DB94(c, t[(((g->unk_C8 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
        p[0x9A] = 0x32;
        *((u16 *)(p + 0xA2)) &= 0xFEFF;
        *((u16 *)(c + 0x14)) &= 0xF7FF;
        p[0x9B] = 0;
        *((s16 *)(p + 0x96)) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)(&D_80083460);
    *((u16 *)(q + 0x2)) &= 0xFF7F;
    r = *((s16 *)(d + 0x64));
    if (r < 0)
    {
        func_80097898(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if (((*((s32 *)(d + 0x14))) & 0x20000) != 0)
    {
        *((s32 *)(d + 0x14)) &= 0xFFFDFFFF;
    }
    if (((*((u16 *)(p + 0xA2))) & 0x10) == 0)
    {
        func_80096AC0(p, b, c, d);
        return;
    }
    e = *((s32 *)(d + 0x1C));
    if ((e & 0x200) != 0)
    {
        func_80097C50((UnkDungeonActor *)p, b, c, d);
        return;
    }
    if (((*((u16 *)(q + 0x2))) & 0x4) == 0)
    {
        if ((e & 0x20) != 0)
        {
            func_800969B8(p, b, c, d);
            return;
        }
        k = *((u16 *)0x80013714);
        if ((k & 1) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((*((u16 *)(p + 0xA2))) & 0x200) != 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFDFF;
            }
            m = func_8009F868();
            if (m == 0)
            {
                return;
            }
            want = (m[0x1] & 7) << 9;
            *((s16 *)(d + 0x2A)) &= 0xFFF;
            if ((*((s16 *)(d + 0x2A))) != want)
            {
                if (((*((u16 *)(d + 0x2A))) & 0x800) != 0)
                {
                    *((s16 *)(d + 0x2A)) |= 0xF800;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) &= 0x7FF;
                }
                do
                {
                    want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                    cur = *((u16 *)(d + 0x2A));
                    delta = (*((s16 *)(d + 0x2A))) - want;
                } while (0);
                if (delta < 0)
                {
                    delta = -delta;
                }
                if (delta > 0x800)
                {
                    *((s16 *)(d + 0x2A)) = (want & (~0xFFF)) | (cur & 0xFFF);
                }
                if ((*((s16 *)(d + 0x2A))) > want)
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) - 0x200;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) + 0x200;
                }
                func_8009F988();
                goto tail;
            }
            D_800E3544 = m[0x1] & 0xF8;
            switch (D_800E3544)
            {
            case 8:
                func_80096C24(p, b, c, d);
                return;

            case 0x10:
                func_800969B8(p, b, c, d);
                return;

            case 0x18:
                func_800973F0(p, b, c, d);
                goto tail;

            case 0xD8:
                D_800DD830[m[0x0] & 0x7F]();
                goto tail;
            }

            goto tail;
        }
        D_800E3544 = 0;
        if ((k & 4) != 0)
        {
            func_80040AA0(3);
            return;
        }
        if (((s16)func_80094EA4()) != 0)
        {
            *((s16 *)(p + 0xA4)) = 0;
        }
        if ((((((*((u16 *)(p + 0xA2))) & 0x200) != 0) && (((s16)func_800A2C34(d)) == 0)) && (((*((u16 *)(q + 0x2))) & 0x4) == 0)) && (((s16)func_800A4474(c[0x24], c[0x25])) != 0))
        {
            *((u16 *)(p + 0xA2)) &= 0xFDFF;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
        f = g->unk_08;
        if ((f & 0x80) != 0)
        {
            *((s16 *)(d + 0x8A)) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = 0;
            *((s32 *)(p + 0xC8)) = 0;
            *((s32 *)(p + 0x104)) = 0;
            return;
        }
        if ((f & 3) != 0)
        {
            func_800A56E0(0x506);
            return;
        }
        n = func_8009074C(*((s16 *)(p + 0x9E)), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
        if (((s16)n) != 0xFFF)
        {
            *((s16 *)(d + 0x2A)) = n;
            if (((g->unk_08 & 0x10) == 0) && (((*((u16 *)(p + 0xA2))) & 0x400) == 0))
            {
                *((u16 *)(p + 0xA2)) &= 0xFFFE;
                if (((*((s32 *)(d + 0x1C))) & 0x400) != 0)
                {
                    *((u16 *)(d + 0x2A)) += func_800A6D30() & 0xE00;
                }
                func_80096C24(p, b, c, d);
                if (p[0x9A] != 0x32)
                {
                    return;
                }
            }
            else
            {
                *((s16 *)(p + 0x96)) = 0;
            }
        }
        w = g->unk_08;
        if ((w & 0x30) == 0x30)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            func_800969B8(p, b, c, d);
            return;
        }
        if (((*((u16 *)(p + 0xA2))) & 0x40) != 0)
        {
            if ((w & 0x40) == 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFFBF;
            }
        }
        else if ((w & 0x40) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((s16)func_800973F0(p, b, c, d)) != 0)
            {
                return;
            }
        }
    }
tail:
    if (((*((u16 *)(p + 0xA2))) & 0x1) != 0)
    {
        *((u16 *)(p + 0xA2)) &= 0xFFFE;
        t2 = D_800DD274;
        *((void ***)(c + 0x2C)) = t2;
        func_8003DB94(c, t2[(((D_80083228 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
    }
}
