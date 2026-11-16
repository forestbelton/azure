/* func_800D92C0 (ovl_dungeon) -- 343 of 344 words, 62 ALIGNED DIFFERENCES.
   Agent j49, 2026-11-16.  Written cold; the 2026-11-15 candidate that reached
   15 was lost with /tmp and nothing of it survived but the three
   docs/matching-levers.md entries, all three of which are applied below.

   SIGNATURE: void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d);  `b` is dead.
   The one landed caller (csrc/ovl_dungeon.c:41344, func_800D8C64) rides an
   IMPLICIT declaration today, so landing this needs
       void func_800D92C0(u8 *, void *, u8 *, u8 *);
   in the declaration block.  func_800D8C64 re-measures at 250/250, 0
   differing with it.  func_800A0E6C is left on its implicit declaration on
   purpose -- csrc/ovl_dungeon.c:33865 already calls it that way.

   FOUR of the 62 are the PROVED-UNREACHABLE `|= 0x80000000` gate
   (docs/matching-levers.md:27186), at words 42/44/45/46.  Nothing was spent
   on them.

   THE DEVICE FROM docs/matching-levers.md:27214 IS IN THE BODY AND IS WORTH
   79 ALIGNED DIFFERENCES HERE, not the 57 that entry records: without
   `__asm__("" : "+r"(i));` after the func_800BCB04 assignment the function is
   344 words / 142 differing, with EVERY callee-saved register one lower than
   the target's ($s1 holds `d` where the target holds `g`/`i`, $s2 holds `c`
   where the target holds `d`, and so on down).  With it the whole register
   assignment -- $s0 w/s, $s1 g then i, $s2 d, $s3 c, $s4 tbl, $s5 p, $s6 f --
   is the target's.  This is the single most valuable line in the function and
   it is the first thing to restore if the body is ever re-derived.

   THE ONE MISSING WORD IS A `nop` THE TARGET KEEPS AND THE CANDIDATE FILLS.
   At 0x800D9458..0x800D9460 the target spends the `bne`'s delay slot on
   `andi $v0, $a1, 0xFFFF` (the second comparison's 16-bit window) and then
   cannot fill the `lbu $v1, 0x25($s3)` load delay, so word 104 is a `nop`;
   reorg here steals `sll $a2, $a2, 16` -- the (s16) conversion that belongs to
   the func_800A0818 call BELOW the `beq` -- into the delay slot instead and
   fills word 104 with the `andi`.  Measured and rejected, all 343 words /
   62 differing and byte-identical to each other:
     the compare nested (`if (c[0x24]==x) { if (c[0x25]==y) {...} }`)
     the two (s16) conversions hoisted into `s16 x2`/`s16 y2` locals
     `t80` dropped and `D_80082E80[0x24]` written inline at both reads
     `t80 = D_80082E80;` moved AFTER the `k =` statement
     `u8 *t1 = (u8 *)D_8006CCD8;` added as a third local before `k`
     `x = t80[0x24]; y = t80[0x25];` split from the two table adds   345 / 64

   ONE SPELLING SCORES 344 / 61 AND IS WRONG, so it is NOT the body here.
   The loop's second test is `i == 0`, not `r == 0`: at 0x800D95D4 the
   `blez`'s delay slot holds `sll $v0, $s1, 16`, which CLOBBERS $v0 before
   .L800D96C4's `bnez $v0` -- so that `bnez` can only be reading `i << 16`,
   and reorg placed the `sll` there because it is the first insn of BOTH
   threads.  Writing the test as `(r << 16) == 0` (with `s32 r` holding
   func_8009A66C's result) makes the candidate keep its own `sll` AND a `nop`
   in the `blez` slot, which pays for the missing word above: 344 / 61.  It
   tests the wrong value.  `(i << 16) == 0` and `i == 0` are byte-identical
   to each other at 343 / 62.

   WHAT GOT IT FROM 184 TO 62 BESIDES THE DEVICE, in the order it paid:

   * THE LOOP'S `*(s16 *)(d + 0x2A)` IS READ ONCE INTO AN `s32` LOCAL before
     the `p + 0x98` test, not inside both arms: read inline it is duplicated
     into each arm (2 words) and comes out `lhu` where the target has `lh`.
     Worth 6 differences and the `lh`/`lhu` pair.
   * THE TWO ROW STORES ARE BASE-FIRST: `*(d + (d[0x71] & 0x7F) + 0x74)`, not
     `d[(d[0x71] & 0x7F) + 0x74]`.  The subscript form is `addu $v0, $v0, $s2`
     where the target has `addu $v0, $s2, $v0` -- 2 differences, and it is
     func_800D9820's rule (csrc/ovl_dungeon.c, "The row address is
     BASE-FIRST") applied to a different row pair.
   * ALL FOUR D_8006CCD8/D_8006CCE8 LOOKUPS ARE INTEGER ARITHMETIC WITH THE
     INDEX FIRST: `*(u16 *)(k + (s32)D_8006CCD8)`.  `(u8 *)D_8006CCD8 + k`
     gives the transposed `addu`.
   * `if (i >= 8) {...} else {...}`, NOT the other way round: the target's
     `bnez $v0, .L800D975C` puts the i<8 arm at the branch target and the
     i>=8 arm on the fall-through, and swapping the arms is 37 differences.
   * func_8009FD7C's and func_8009A66C's results are tested through an
     explicit `(s16)` cast; without it on the func_8009A66C inside the
     0x20000 arm the `sll $v0, $v0, 16` at 0x800D94AC disappears.

   THE TWO BLOCKS THAT STILL CARRY MOST OF THE RESIDUE are both pure sched1
   placement of a symbol's `lui`/`addiu`, and no source spelling moved either:

   * 0x800D93F4..0x800D9474 (14 rows).  The target materialises BOTH
     D_80082E80 and D_8006CCD8 before the D_800814A8 load; the candidate
     materialises D_80082E80 there and D_8006CCD8 after the index multiply.
     Six placements of the base locals were measured (see the rejected list
     above) and every one is byte-identical.
   * 0x800D9650..0x800D9698 (12 rows).  The target materialises D_8006CCD8
     before `lhu 0x2A($s2)` and D_8006CCE8 after the first `sb`; the
     candidate materialises each at its use.  A `u8 *` local for the first
     table ADDS three words (347 / 67), which is the opposite of what the
     pointer-local hoist does elsewhere in this unit.

   TRIAGE: one word short, in one identified block, with the structure either
   side of it proved -- permuter food by docs/dungeon-sweep.md's rule. */
void func_800D92C0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *w;
    s16 *tbl;
    s16 f;
    s16 i;
    s16 m;
    s32 s;
    s32 n;
    u8 *t80;
    s32 j;
    s32 k;
    s32 r;
    s32 v;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        func_800A9A0C(d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    w = p + 0x9C;
    func_800A19E4((s32)c, d, 3, 6, w);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0 && *(s32 *)(d + 0x14) >= 0)
        {
            *(s32 *)(d + 0x14) |= 0x80000000;
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
        }
    }
    else
    {
        if (*(s8 *)(d + 0x71) >= 0)
        {
            return;
        }
        if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
        {
            return;
        }
        func_800A19E4((s32)c, d, 3, 6, w);
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x2000) != 0)
        {
            if ((v & 0x20000) != 0)
            {
                t80 = D_80082E80;
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = t80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = t80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (c[0x24] == (u16)x && c[0x25] == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                m = func_800A0818(c[0x24], c[0x25], x, y, (u16 *)(p + 0x98));
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A66C(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                       D_80082E80[0x25], (u16 *)(p + 0x98));
                }
                if (func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
        else
        {
            j = *(s8 *)(c + 0x26);
            if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
            else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_800A0E6C(c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tbl = D_8006CD00;
    do
    {
        n = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = n - tbl[i];
        }
        else
        {
            s = n + tbl[i];
        }
        r = func_8009A66C((s16)s, c, d, 0x20);
        if ((r << 16) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = c[0x24];
            *(d + (d[0x71] & 0x7F) + 0x7C) = c[0x25];
            d[0x71]++;
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            k = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            c[0x24] += *(u8 *)(k + (s32)D_8006CCD8);
            c[0x25] += *(u8 *)(k + (s32)D_8006CCE8);
            func_8009A21C(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && *(u16 *)(D_80082E80 + 0x24) != *(u16 *)(c + 0x24))
        {
            if ((s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
        }
        i++;
    } while (i < 8);
    if (i >= 8)
    {
        d[0x71] &= 0x7F;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        func_800A9A0C(d);
    }
    else
    {
        p[0x9C] = c[0x26];
        d[0x6D]--;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0x8) = *(u16 *)(g2 + 0x8) + 1;
        if (*(s8 *)(d + 0x6D) == 0)
        {
            d[0x71] &= 0x7F;
            return;
        }
        i = func_800BCB04(c[0x24] * 64 | 0x20, c[0x25] * 64 | 0x20,
                          *(u16 *)(d + 0x88) - 0x20);
        __asm__("" : "+r"(i));
        if (i < 0x200)
        {
            *(s16 *)(d + 0x88) = i;
        }
    }
}
