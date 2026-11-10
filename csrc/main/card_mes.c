#include "common.h"
#include "main.h"
#include "strings.h"

/* card_mes.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 46..87. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine".

   Its rodata is the four bytes at 0x800200A0. The unit owns no other
   rodata and no data. */

/* The space-skipping width measurer: for each of the a+1 segments it skips a
   run of spaces (one unit each), then walks the glyphs two bytes at a time
   (two units each), reporting the run start in *x, the glyph count in *y and
   the accumulated glyph total in *z.

   The space skip has to be a plain `while`, not a guarded `do`/`while`. Both
   compile to the same guard-plus-rotated-test shape, but written as
   `if (...) do ... while (...)` gcc shares ONE register for the 0x20 it
   compares against, where the target has three: one hoisted out of the outer
   loop, one hoisted into the skip loop by loop rotation, and one for the
   glyph walk. Writing the skip as a loop in its own right is what makes
   loop.c hoist that second copy. */
void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z)
{
    s32 total = 0;
    s32 n = 0;
    s32 i = 0;

    *z = 0;
    if (a < 0)
    {
        return;
    }
    do
    {
        *z += n;
        while (*s == ' ')
        {
            s++;
            total++;
        }
        *x = total;
        n = 0;
        while (*s != ' ' && *s != 0)
        {
            s += 2;
            n++;
        }
        total += n * 2;
        *y = n;
    } while (++i <= a);
}

/* The statement order is load-bearing, and only one thing about it is: the
   `p[0xA]` store has to come SECOND, before the 0x13 and the word store.
   sched1 schedules this block backward and fills the `lbu p[0]` load delay
   with whatever insn pair is still unplaced; written here, the `(n << 3) + 4`
   pair is consumed early and the 0x13 constant becomes the filler, which puts
   its one-insn live range INSIDE the p[0] quantity's. local-alloc then prices
   the 0x13 pair at 2 refs / 1 insn = 1.0 against the p[0] quantity's 4 refs /
   5 insns = 0.8, so 0x13 takes $v0 and p[0] is pushed to $v1; the 0xA04010
   constant, now spanning insns 1..11, is blocked from $v0 by both byte
   constants and from $v1 by p[0], and lands in $a0, free because the first
   parameter is dead. With p[0] in $v1 the return value no longer collides with
   it, so sched2 hoists `addiu $v0, $a1, 0xC` to word 9 and reorg takes the
   `sb $v1, 0($a1)` for the `jr` delay slot instead of the return value. All
   three differences in the old park were that one local-alloc flip.

   The four stores other than `p[0] |= 0x80` must be in the order p[1], p[0xA],
   p[0xB], p+4; `p[0] |= 0x80` matches at any of the five positions. Measured
   (all 15 words): all 120 permutations of the five statements scored, five
   match, the next best is 3 differing. The parked order (p[1], p[0xB], p+4,
   p[0xA], p[0]) was 12, as were an explicit `u8`/`s32` temp for p[0] first, a
   local for the return value, and `u32 *` for the word store. */
u8 *func_80022300(s32 *a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xA] = (n << 3) + 4;
    p[0xB] = 0x13;
    *(s32 *)(p + 4) = 0xA04010;
    p[0] |= 0x80;
    return p + 0xC;
}

void func_8002233C(u8 *p, s32 idx, s32 n)
{
    s32 i;

    func_8004E884(p, 1);
    p += idx * 12;
    for (i = 0; i < n; i++)
    {
        func_8004E264(p, 0);
        p += 12;
    }
}

u8 *func_80022300(s32 *a, u8 *p, s32 n);

/* p[569] is ptrs[6] of the 0x8B0 group (ptrs starts at 0x8CC) and p[563 + n]
   is ptrs[n]; the two s16 stores are the sprite's x and y, each a position
   minus half the corresponding extent. `x * 4` has to be a local declared
   BEFORE the other two: the target computes it at word 3, ahead of the $s4
   save and the `p + 0xE4` address, and inline it is computed after both. */
void func_800223B4(UnkCardGrp8B0 *c, s32 x, s32 y, s32 n, s32 z)
{
    s32 sx = x * 4;
    UnkCardPair *e = c->ptrs[6];
    s32 *p = (s32 *)c;
    u8 *q = (u8 *)&p[57];

    e->unk_04->unk_08 = sx - (p[6] / 2) + 0xA2;
    e->unk_04->unk_0A = (n * 17) - (p[7] / 2) + 0x7A;
    func_80022300(p, q, y);
    e->unk_00 = (s32)q;
    func_8002233C((u8 *)c->ptrs[n]->unk_00, z, y);
}

void func_80022488(s32 *p)
{
    UnkStrSet *t = gCardMessages;
    UnkStrSet *r = &t[p[2]];
    s32 n = r->unk_17 - 1;
    s32 x;
    s32 y;
    s32 z;

    if (r->unk_14 != 0)
    {
        func_8002225C(r->strs[n], p[0], &x, &y, &z);
        func_800223B4((UnkCardGrp8B0 *)p, x, y, n, z);
    }
}

/* The same input-handler shape as func_800265B8, and the same two levers:
   the `p[4] >= 3` arm comes first with the increment as the `else`, and the
   0x8000/0x2000 pair is written out in BOTH arms so jump.c cross-jumps the
   two `dir` assignments into one tail. func_800231E4(1) is likewise written
   in both arms of the p[8] test -- the target calls it either way but only
   stores the result when p[8] is non-zero, and the store needs p[8] reloaded
   after the call, so the test has to precede it. */
void func_80022524(s32 *p)
{
    s32 dir = 0;
    s32 n;

    if (D_80083160.unk_08 == 0)
    {
        return;
    }
    if (D_80083160.unk_10 & 0x20)
    {
        func_80053DA8(0x515);
        func_800231E4(1);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = 1;
        }
    }
    else if (D_80083160.unk_10 & 0x40)
    {
        func_80053DA8(0x514);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = func_800231E4(1);
        }
        else
        {
            func_800231E4(1);
        }
    }
    else if (D_80083160.unk_08 & 0xA000)
    {
        if (D_80083160.unk_10 & 0xA000)
        {
            p[4] = 0;
            if (D_80083160.unk_10 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_10 & 0x2000)
            {
                dir = 1;
            }
        }
        else if (p[4] >= 3)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_08 & 0x2000)
            {
                dir = 1;
            }
        }
        else
        {
            p[4]++;
        }
    }
    if (dir != 0)
    {
        n = func_80049E1C(p[0], dir, gCardMessages[p[2]].unk_14);
        if (n != p[0])
        {
            p[0] = n;
            func_80053DA8(0x502);
        }
        func_80022488(p);
    }
}

void func_800226A8(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_800231E4(0);
    }
}

void func_800226E0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -72;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* func_80022A30's bigger sibling: the same unaligned D_800200A0 copy, the
   same four iterations, the same `p[-0x18] |= 0x80` and return, but THREE
   primitives per iteration (0xC, 0xC, 0x18 bytes), the last a D_80027F48
   template copied through a 24-byte alignment-1 struct and then patched with
   four 0x90D0E0 words. Width and height come from gCardMessages[q[2]].unk_16 * 4
   and .unk_17 * 17, stored to q[6]/q[7] with +8 and halved for the sprite
   bytes; `(w + 0xA) >> 1` is the one signed halving.

   Shape notes (each measured, none a device): `i = 0` before the loop rather
   than in the `for` (4); `bh = h + 0xA` AFTER the `(w + 0xA) >> 1` store, the
   natural order -- written before it the `h + 0xA` movable is hoisted ahead
   of the `w + 0xA`/`sra` pair and those three preheader words rotate (3);
   `h = (hh << 4) + hh` rather than `hh * 17` (11, see the pin below). */
u8 *func_80022774(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    u32 bh;
    /* DEVICE: a hard-register pin on the unk_17 byte, worth 11 words
       (11 -> 0). Without it `hh` is a three-reference block-0 quantity that
       local-alloc prices (`qty_compare_1`, local-alloc.c:1598,
       `floor_log2(refs)*refs/(death-birth)` over the SCHED1 order) below the
       seven-reference unk_16/w chain, so w takes $v1 and hh $a0: 11 differing
       words, all register names in the preheader. Three references cannot win
       at any live length this block admits; four can, but the only free fourth
       reference is a `__asm__("" ::"r"(hh))` marker, and that marker is then
       the load's last consumer in sched1's backward schedule: it is what
       launches the `lbu` (sched.c `adjust_priority`, LAUNCH_PRIORITY), and to
       launch it late enough to sit above the `sll`/`addiu` of w the marker
       must take sched1's one priority-1 hole at T-13 -- the hole the giv
       initialiser `p + 0xB` needs in order to end up above the `lo_sum` in
       sched2's LUID tie-break (`rank_for_schedule`, sched.c:2425). The two
       cannot share it: every marker form measured is 3 at best (two markers),
       and 0 only with a THIRD marker holding the `lo_sum` back
       (`"r"((u8 *)&gCardMessages[q[2]] - q[2] * 24)`, which cse folds to the
       lo_sum pseudo), declined as a third device. The pin removes the pricing
       question entirely, leaves the load's launch to the `sll` so the giv gets
       the hole, and sched2 then orders the two `lbu` by priority on its own.
       `(hh << 4) + hh` is required with it: `hh * 17` goes through
       expand_mult, which copies $3 into a pseudo that combine substitutes back
       into both uses, and the pin is lost (11, the same words).
       Legal spellings ruled out: no pin at all 11; `hh * 17` with the pin 11;
       `register u8 hh` 11; `__asm__("" ::"r"(hh))` alone 3 (giv permutation);
       `"r"(hh), "r"(w + 8)` 3 (the rotation); `"r"(hh), "r"(w)` 11;
       `__asm__("" : "+r"(p) : "r"(hh), "r"(hh), "r"(hh))` 8; two markers
       `"r"(hh)` + `"r"(w + 8)` 7; the volatile and `"memory"` forms 28..62.
       Warns under -Wall? No. */
    register u32 hh __asm__("$3");
    s32 g;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    i = 0;
    w = gCardMessages[q[2]].unk_16 * 4;
    hh = gCardMessages[q[2]].unk_17;
    h = (hh << 4) + hh;
    q[6] = w + 8;
    q[7] = h + 8;
    for (; i < 4; i++)
    {
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = (s32)(w + 0xA) >> 1;
        bh = h + 0xA;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x104058;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = D_80027F48;
        g = 0x90D0E0;
        /* DEVICE: one marker doing three jobs, worth 85 words (85 -> 0).
           (a) `"+r"(g)` is a SECOND set of g in the loop whose source is not
           invariant, so `consec_sets_invariant_p` (loop.c) fails and
           `g = 0x90D0E0` never becomes a movable: the lui/ori stays in the
           loop where the target has it. The threshold cannot do this job --
           cse1 merges the four stores onto one pseudo of lifetime 4 and
           `threshold*1*4 >= insn_count` always holds. `"r"(i)` is what makes
           the asm variant; without it (`"+r"(g)` alone) loop.c moves BOTH
           insns out as a two-insn consec movable (84), and `volatile`
           instead is 68.
           (b) `"r"(bh)` makes the asm the last use of `h + 0xA`, so
           force_movables finds no movable at `regno_last_uid[bh]` and does
           not link the `srl` to the `addiu`; the addiu is hoisted on its own
           merit and the shift is left in the loop on the threshold, as the
           target has it. Written `p[0xB] = (h + 0xA) >> 1;` the shift rides
           out free on the addiu's link (36 on the park body).
           (c) `"r"(bh)` is also bh's THIRD reference, which keeps its
           global-alloc priority above the two hoisted constants so it takes
           $t4; a dead second assignment `g = bh ^ i;` does (a) and (b) for
           free but is deleted by flow before the references are counted, bh
           drops to $t8 and t5..t8 shift down (15).
           Re-measured on the landed body: dropping `"r"(bh)` with the shift
           inline 27, dropping the marker entirely 85.
           Legal spellings ruled out for (a): four separate `s32` locals, one
           per store (85 -- cse1 collapses them anyway); `g` assigned 0 before
           the loop (85 -- the gate also needs maybe_never, which a rotated
           for-loop never gives); the dead assignment above (15).
           Warns under -Wall? No -- g is initialised. */
        __asm__("" : "+r"(g) : "r"(i), "r"(bh));
        *(s32 *)(p + 4) = g;
        *(s32 *)(p + 0xC) = g;
        *(s32 *)(p + 0x10) = g;
        *(s32 *)(p + 0x14) = g;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_8002290C(UnkCardGrp8B0 *c)
{
    UnkCardPair *q = c->ptrs[5];

    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0x78;
    q->unk_00 = (s32)((s32 *)c + 9);
}

/* Five sprites in a row, 0x180 bytes of buffer apart, each label rendered by
   func_8004DA74 into `(u8 *)p + 0x114 + i * 0x180`. gCardMessages must be
   indexed directly: hoisting it into a `UnkStrSet *t` local costs 5 DIFF
   even though gcc hoists the base address either way, and giving the row its
   own local inside the loop costs 21. */
void func_80022934(s32 *p)
{
    s32 i;

    for (i = 0; i < 5; i++)
    {
        UnkCardPair *e = ((UnkCardGrp8B0 *)p)->ptrs[i];

        e->unk_04->unk_08 = 0x124 - (p[6] / 2);
        e->unk_04->unk_0A = (i * 17) - (p[7] / 2) + 0xFC;
        e->unk_00 = (s32)func_8004DA74((u8 *)p + 0x114 + i * 0x180,
                                       gCardMessages[p[2]].strs[i], 1);
    }
}

/* The second parameter is unused by the body but func_80022AE8 sets $a1
   before the call and sets nothing else. */
u8 *func_80022A30(u8 *p, s32 *q)
{
    u8 buf[4];
    u8 w1;
    u8 h1;
    u8 c;
    s32 d;
    u8 w2;
    u8 h2;
    s32 i;

    /* Every one of these seven assignments is its own pre-loop statement, and
       that order is the whole function. The target's preheader is
       `i = 0`, 5, a COPY of the 5, 0x48, 0x104058, 6, a COPY of the 6, and
       then the giv `addiu $a0, $a3, 4`. Two things it is NOT:

       - the pairs are not hoisted out of the loop. Written inside the body
         they ARE hoisted in body order, which looks like the listing, but
         cse1 collapses the two pseudos holding 5 before loop.c ever sees
         them (confirmed in the .cse dump: one `(set (reg:QI N) (const_int 5))`
         serves both stores) and the function comes out 44 words / 36 DIFF.
         Widening the source so the copy is mode-invisible to cse does not
         help either -- `u32 w1; u8 h1 = w1;` is still 44/36 -- and even if a
         pair survived cse1, loop.c's combine_movables would match the two
         movables (equal mode, and `matched_regs[w1]` makes the copy's REG
         source count as the same value) and replace one register with the
         other. A hoisted pair can never keep its copy.
       - 0x48 and 0x104058 are not literals at their use sites. As literals
         they are hoisted by loop.c, which puts them AFTER the for-init
         instead of between the two pairs: 46 words, 8 DIFF.

       So all seven are statements, `i = 0` is lifted out of the `for` to put
       it first, and the pairs survive because the copy and its only use sit
       in different cse basic blocks -- the copy is before the loop, the use
       inside it, so canon_reg never rewrites the use. Measured alternatives,
       all on the same body: pairs assigned before the loop with 0x48/0x104058
       left as literals 46w/8; pairs inside the loop body 44w/36; pairs inside
       with w1 `u32`/`u16` and h1 `u8` 44w/36; h1 `u32` and w1 `u8` 44w/36;
       all four `u32` 44w/36; `h1 = 5` instead of `h1 = w1` 44w/36; a dead
       pre-loop `h1 = 0` to make the movable global 44w/36. */
    *(UnkFour *)buf = *(UnkFour *)D_800200A0;
    i = 0;
    w1 = 5;
    h1 = w1;
    c = 0x48;
    d = 0x104058;
    w2 = 6;
    h2 = w2;
    for (; i < 4; i++)
    {
        p[0xA] = w1;
        p[0xB] = h1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = w2;
        p[0xB] = h2;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80022AE8(UnkCardGrp8B0 *c)
{
    s32 *p = (s32 *)c;

    func_80022A30((u8 *)(p + 9), p);
    func_8002290C(c);
}

s32 func_80022B20(s32 a)
{
    return func_800491F4(a, D_80027F60, 7);
}

/* The arrayed primitive initialiser, six copies of it at six base offsets.
   The lever, found by decomp-permuter after two long searches on the wrong
   idea: D_8002E5D8[0] must be read into a LOCAL before the `ptrs[i]` store,
   not fetched where it is used. That one reordering is the whole difference
   between 9 differing words and zero, in all six functions.

   What it buys: with the load first, gcc keeps the base in $t3 (copied from
   $a0 at word 0) and leaves $a0 free as the scratch for the prims address,
   which is the register assignment the targets have. Fetched in place, the
   base stays in $a0 and the prims address takes $v1 -- and no amount of
   restructuring around a second base pointer reaches it, which is what the
   earlier 9-DIFF park was trying. Both UnkCardPair fields must also go
   through `c->ents[i]` rather than a live `e` local, so they stay on the
   stride-16 induction variable at displacements 0x8EC/0x8F0. */
void func_80022B48(UnkCardGrp8B0 *c, s32 n)
{
    s32 i;
    u32 v;

    for (i = 0; i < n; i++)
    {
        v = D_8002E5D8[0];
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = v;
        c->prims[i][1] = D_8002E5D8[1];
        c->prims[i][2] = D_8002E5D8[2];
        c->prims[i][3] = D_8002E5D8[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002E5E8[0];
    c->shared[1] = D_8002E5E8[1];
    c->shared[2] = D_8002E5E8[2];
}

/* The pointer bump belongs in the TEST, not the body. `duplicate_loop_exit_test`
   (jump.c:2131) copies the inner loop's exit test above the loop in either
   spelling, so the three extra words of `while (*s != 0) { s++; ... }` are not
   a guard that failed to appear -- they are a guard that failed to be merged
   back. `jump2`'s conditional cross-jump (jump.c:1923-1950) collapses the guard
   into the plain `j` at 0x80022BFC only if the guard copy and the bottom test
   end in an identical tail of at least `minimum` = 2 insns (find_cross_jump,
   jump.c:2371, which scans BACKWARD from each conditional jump and stops at the
   first mismatch). With `s++` in the test both tails are `lbu` + `addiu`, so two
   insns match and the merge fires. With `s++` in the body, sched1 fills the load
   delay after the `lbu` with `last = i` at the bottom and with `len = 0` in the
   guard, so the very first backward pair already differs and nothing merges.
   Measured: `while (*s != 0) { s++; ... }` 27 words / 23 DIFF;
   `for (len = 0; *s != 0; s++)` 28 words / 23 DIFF; this spelling 24 / 0. */
void func_80022BE8(UnkStrSet *q)
{
    s32 maxlen = 0;
    s32 last = 0;
    s32 i;

    for (i = 0; i < 5; i++)
    {
        u8 *s = q->strs[i];
        s32 len = 0;

        while (*s++ != 0)
        {
            len++;
            if (maxlen < len)
            {
                maxlen = len;
            }
            last = i;
        }
    }
    q->unk_16 = maxlen;
    q->unk_17 = last + 1;
}

void func_80022C48(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    p[5] = a;
    p[2] = b;
    p[3] = c;
    p[8] = d;
    func_80022BE8(&gCardMessages[b]);
}

void func_80022C90(UnkCardGrp8B0 *g, s32 a, s32 b, s32 c, s32 d)
{
    func_80022C48((s32 *)g, a, b, c, d);
    func_800226E0(&g->root, g->root_prim, g->root_shared);
    func_80022AE8(g);
}

void func_80022CD8(s32 *p)
{
    func_80022774((u8 *)(p + 9), p);
    func_80022934(p);
    p[0] = gCardMessages[p[2]].unk_15;
    func_80022488(p);
    if (p[3] == 2)
    {
        p[-4] = (s32)func_800226A8;
    }
    else
    {
        p[-4] = (s32)func_80022524;
    }
}

void func_80022B48(UnkCardGrp8B0 *c, s32 n);

/* The same constructor idiom as func_80023FF0 and func_800238D4, at the
   singleton D_800287E8. It RETURNS q even though every caller ignores the
   value: the target copies $s0 into $v0 and stores the func_80022CD8 pointer
   through $v0, which as a void function is three words instead of four.
   func_80022B20 likewise had to become `s32` with a `return`, since its
   value lands in r[0x232]; its own ten words are unchanged. */
s32 open_card_message(s32 a, s32 id, s32 kind, s32 p)
{
    s32 *q = (s32 *)D_800287E8;
    s32 *r = q + 8;
    UnkCardGrp8B0 *c;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800287E8, 0x27A);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_800231E4(r[3] != 2);
        ((u16 *)q)[15] &= 0x7FFF;
        bzero((unsigned char *)r, 0x9C8);
    }
    c = (UnkCardGrp8B0 *)r;
    func_80022B48(c, 7);
    q[3] = (s32)&c->root;
    c->root.unk_0C = func_80022B20((s32)c->ptrs);
    func_80022C90(c, a, id, kind, p);
    q[4] = (s32)func_80022CD8;
    return (s32)q;
}

void msg_format_confirm(s32 a, s32 p)
{
    open_card_message(a, 0, 1, p);
}

void msg_no_card(s32 a)
{
    open_card_message(a, 1, 0, 0);
}

void msg_please_wait(s32 a, s32 p)
{
    open_card_message(a, 2, 2, p);
}

void msg_overwrite_failed(s32 a)
{
    open_card_message(a, 8, 0, 0);
}

void msg_formatting(s32 a, s32 p)
{
    open_card_message(a, 4, 2, p);
}

void msg_overwrite_confirm(s32 a, s32 p)
{
    open_card_message(a, 5, 1, p);
}

void msg_saving(s32 a, s32 p)
{
    open_card_message(a, 6, 2, p);
}

void msg_loading(s32 a, s32 p)
{
    open_card_message(a, 14, 2, p);
}

void msg_not_enough_blocks_overwrite(s32 a)
{
    open_card_message(a, 9, 0, 0);
}

void msg_not_enough_blocks(s32 a)
{
    open_card_message(a, 10, 0, 0);
}

void msg_load_failed(s32 a)
{
    open_card_message(a, 11, 0, 0);
}

void msg_format_failed(s32 a)
{
    open_card_message(a, 12, 0, 0);
}

void msg_format_done(s32 a)
{
    open_card_message(a, 13, 0, 0);
}

void msg_card_damaged(s32 a)
{
    open_card_message(a, 3, 0, 0);
}

void msg_no_saved_game(s32 a)
{
    open_card_message(a, 7, 0, 0);
}

void msg_copying(s32 a, s32 p)
{
    open_card_message(a, 15, 2, p);
}

void msg_copy_failed(s32 a)
{
    open_card_message(a, 16, 0, 0);
}

void msg_cannot_overwrite(s32 a)
{
    open_card_message(a, 17, 0, 0);
}

void msg_delete_confirm(s32 a, s32 p)
{
    open_card_message(a, 18, 1, p);
}

void msg_delete_failed(s32 a)
{
    open_card_message(a, 19, 0, 0);
}

/* The local copy of the parameter is load-bearing, and so is declaring the
   counter first: the target saves $s1 (the counter) before $s0 (the cursor)
   and fills the load-delay slot with the cursor's own increment. Walking the
   parameter directly puts $s0 first and leaves a nop there -- 6 differing. */
void func_80023194(UnkCardGrp8B0 *c)
{
    s32 i = 0;

    do
    {
        func_8004E994(c->ptrs[i]->unk_00);
    } while (++i < 5);
}

s32 func_800231E4(s32 flag)
{
    s32 *p = &D_80028808;

    if (flag != 0)
    {
        ((u16 *)p[5])[15] |= 0x2000;
    }
    func_80023194((UnkCardGrp8B0 *)p);
    ((u16 *)p)[-1] |= 0x8000;
    D_800814A0_a[0] |= 0x8000;
    return D_80028808;
}

const u8 D_800200A0[4] = {0, 1, 2, 3};
