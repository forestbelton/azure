/* PARK: func_b38_80172494, csrc/ovl_dng_b38.c, agent-h225, 2026-11-26.

   TARGET 363 words. BEST BUILT 363 words, 92 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py), 97 differing under ovlverify.
   1 member, 363 words (dngfam.py).
   VERDICT: ALLOC, and it is a THREE-WAY PERMUTATION of p, b and the spawn
   loop's counter over $s2/$s3/$s4. Nothing is moved, nothing is missing, the
   frame and the save COUNT are the target's, every arm is present and the
   word count is exact.

   WHAT THIS BODY IS. The banked room script's SIX-state step on p + 0x9B
   over {0, 1, 2, 3, 4, 0xFF}, with the slot's +0x96 countdown decremented
   once before the dispatch and the D_8006CCD8 / D_8006CCE8 facing pair read
   into x and y at the top. State 0 either retires straight to 0xFF (when
   c + 0x14 already has 0x8000, raising 0x6000 and cueing func_8009C12C) or
   seeds the mover from the NEGATED facing pair shifted 18 and clears two
   d + 0x1C flags; state 1 grows b + 0x14 by a quarter; state 2 decays all
   three mover words by an eighth, re-dresses off the bank-local D_80174AE4
   table and re-seeds the mover at three halves of the facing pair; state 3
   and state 4 advance p + 0x90 by 0x80000; state 0xFF eases the mover onto
   the tile centre, spawns TEN 0x312 nodes that func_b38_80173F0C draws, and
   hands the actor back to func_b38_80171138 through func_800A2B04 /
   func_800AD594 / func_800A4ACC.

   NO JUMP TABLE: six cases, but 0xFF puts the range at 255 so
   expand_end_case takes the compare chain, and the arms emit in ascending
   source order. Same as the donor.

   THE RESIDUE, EXACTLY. 92 rows, of which about 85 are the rename. Mine
   assigns p = $s3, b = $s2, i = $s4; the target assigns p = $s4, b = $s3,
   i = $s2. c = $s5, d = $s7, x = $s0, y = $s6, n = $s1 and
   &D_800DEC28 = $fp agree on both sides, as do the frame (-0x40), the ten
   saved slots and the two spill slots at 0x10/0x14($sp). So exactly one
   quantity -- the spawn loop's counter -- has to outrank p and b, and does
   not. The rest: a `lbu'/`lh'/`sll' transposition in state 0xFF's two
   position expressions (rows 212-223), the `lui $a0, 0xC0C0C0' slot
   (rows 279-283), and the `lui'/`addiu' pair for D_8006CCE8 at the top
   (rows 20-21).

   WHAT THE PERMUTATION IS NOT. Three independent perturbations give the
   IDENTICAL 92 rows, which by the levers file's rule puts the cause outside
   the arithmetic and outside allocation priority:
     the declaration order permuted so `i' is declared FIRST   363 / 92
     every `b->unk_0C' / `unk_10' / `unk_14' rewritten as the donor's
       plain cast `*(s32 *)((u8 *)b + 0xC)' etc. (19 sites), which flips
       MEM_IN_STRUCT_P on all of them                          363 / 92
     (and the body as first written, before the four fixes below) 362 / 105
   `-da' `.greg' prints the allocation order as
   `;; 13 regs to allocate: 85 90 341 81 80 84 82 89 83 88 299 272 268',
   i.e. pseudo 81 is placed before pseudo 80, which is the b-before-p order
   both sides agree on -- so the ORDER of p and b is not in question, only
   where the counter slots into it. ASSUMED, not measured: that the counter
   needs one more weighted reference to cross a `floor_log2' power-of-two
   cliff in `allocno_compare'. I did not find a natural spelling that adds
   one, and the declaration-order result argues against priority being the
   mechanism at all. NOT RESOLVED.

   FOUR FACTS MEASURED AND KEPT, each of which moved the body:

   (1) THE SPAWN LOOP COUNTS DOWN IN THE SOURCE, `for (i = 9; i >= 0; i--)'.
       Written UP as `for (i = 0; i < 10; i++)' it is 363 words / 101 rows
       and the loop control comes out THREE insns (`move/addiu/slti/bnez')
       where the target has TWO (`addiu $s2, $zero, 9' / `addiu -1' /
       `bgez'): check_dbra_loop does NOT reverse it, because `i' is read by
       the loop test in a form it will not rewrite. The levers file's
       "an emitted count-DOWN loop means the source counts UP" does not
       apply when the body is a plain 10-iteration spawn loop.

   (2) STATE 0xFF's POSITION EXPRESSION NEEDS THE `- 0x20' IN ITS OWN
       STATEMENT, and this is worth 2 words. Written as one expression,
       `(((c[0x24] << 6) - (*(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) >> 1',
       fold does BOTH of the things the target does not: it reassociates
       `a - (b - 0x20)' into `a + 0x20 - b' (the target keeps
       `addiu $v1, $v1, -0x20' on the b operand) and it merges `<< 15 >> 1'
       into a single `sll 14'. Split as
           u = *(s16 *)((u8 *)b + 0x2) - 0x20;
           b->unk_0C = (((c[0x24] << 6) - u) << 15) >> 1;
       both survive, because fold works per expression and cannot see across
       the statement boundary while combine will not merge an ASHIFT into an
       ASHIFTRT. Also measured on the same site: `/ 2' instead of `>> 1' is
       362 words / 132 rows AND grows the frame to -0x48 (the signed
       power-of-two divide needs its correction sequence and a pseudo); and
       splitting at the OTHER boundary, `u = (...) << 15; x = u >> 1', is
       NOT MEASURABLE -- fold still sees one tree through the single-use
       local, so the split has to be INSIDE the shifted expression.

   (3) STATE 2's RE-SEED NEEDS THE EXPLICIT SHIFT PAIR, `(x << 18) +
       (x << 17)', NOT the plain constant `x * 0x60000'. The plain constant
       is 9 rows and emits `sll 1 / addu / sll 17' where the target has
       `sll 18 / sll 17 / addu' -- same three insns, different three. THIS
       INVERTS the levers file's "a multiply the listing shows as
       sll/addu/sll wants the plain constant": that rule is about a listing
       whose pattern IS sll/addu/sll, and this listing's is sll/sll/addu.
       READ WHICH THREE INSNS THE LISTING HAS BEFORE APPLYING IT.

   (4) THE SPAWNED NODE'S LAST FOUR STORES GO
       `+0x8, +0xC, +0x4, +0x5' IN THE SOURCE, which is worth 1 word and
       4 rows against the transcribed order `+0xC, +0x8' with the two `sb's
       before them. With the colour word written first the `lw $v0, 0x4($fp)'
       load delay has nothing left to fill it and a `nop' survives; moving
       the two byte zeros to the end lets the scheduler put the
       `ori $a0, $a0, 0xC0C0' there, which is what the target does. The
       EMITTED order is `+0xC' then `+0x8' either way, so it is the order you
       would transcribe and it is the wrong one -- the same trap the donor
       records as its own fact (1).

   DONOR: csrc/ovl_dng_b12.c func_b12_80172654 (297 words, landed), named by
   dngcallee.py at 14 of 19 symbols with a clean gap to the 12/19 tier. It is
   the right donor and the shape is its shape: the same six-case set, the
   same `x = D_8006CCD8[(*(u16 *)(d + 0x2A) >> 9) & 7]' facing pair, the same
   `*(u16 *)(p + 0x96) -= 1' before the dispatch, the same
   `D_80083228 + d + 0x2A' facing index, and its state 0xFF teardown
   (func_800A2B04 / func_800AD594 / D_8008346C = 0 / func_800A4ACC / the
   d + 0x6D two-arm on D_800E3DE8) is this body's teardown statement for
   statement. Its frame is -0x38 with seven callee-saved where this is -0x40
   with nine, so by the three-kinds rule its SHAPE facts are the
   transferable ones and its allocation facts are not -- which is consistent
   with the shape carrying completely and the residue being allocation.
   Its own fact (2), "the facing pair is negated BEFORE the shift", holds
   here: state 0's `-x << 18' is the target's `negu'/`sll 18'.
   Its position arithmetic does NOT carry: b12 works on the whole 32-bit
   mover word (`(((c[0x24] << 6) + 0x20) << 16) - *(s32 *)(b + 0x0)) >> 1'),
   this body works on the HIGH HALFWORD (b + 0x2) and shifts 15 then 1.

   RESOLVED NAMES: func_80069EF8 is rand (splat/common_syms.txt), and the
   unit already declares it. func_8004491C's second argument here is
   `(u32)D_80045340', the usual byte-table, not a function pointer.
   D_800DEC28 appears NOWHERE ELSE in the corpus -- it is this body's own
   0x312-node template and `*(s32 *)(m + 0x8) = *(s32 *)(D_800DEC28 + 0x4)'
   is the only read of it.

   relcand.py: ok, 10 data symbols and 14 calls -- so every `%hi' table and
   every `jal' target is the listing's. declcheck.py: clean.
   NOT a table owner.

   DECLARATIONS
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s16 D_80083228;
   extern s32 D_8008346C;
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   extern u8 D_80174AD4[];
   extern u8 D_80174AE4[];
   extern u8 D_800DEC28[];
   extern u8 *D_800E3DE8;
   s32 rand(void);
   UnkNode *func_8003FD64(s32, void *);
   u8 *func_8009C12C(u8 *, u8 *, s16, s16);
   void func_80047784(u8 *, s32, s32);
   void func_800A2B04(UnkVec *, s32, s32);
   void func_800A4ACC(u8 *p);
   void func_800AD594(u8 *p, s32 a);
   s32 func_800A56E0(s32);
   void func_b38_80173F0C(u8 *p, u8 *b, u8 *c);
   void func_b38_80171138();
   END DECLARATIONS

   func_8004491C comes from include/structs.h. func_b38_80173F0C is DEFINED
   LOWER IN THE UNIT than this function, so the forward declaration above is
   required -- declcheck.py flags its absence. */
void func_b38_80172494(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *w;
    u8 *m;
    s32 x;
    s32 y;
    s32 i;
    s32 u;

    x = D_8006CCD8[(*(u16 *)(d + 0x2A) >> 9) & 7];
    y = D_8006CCE8[(*(u16 *)(d + 0x2A) >> 9) & 7];
    *(u16 *)(p + 0x96) -= 1;
    switch (p[0x9B])
    {
    case 0:
        if (*(u16 *)(c + 0x14) & 0x8000)
        {
            p[0x9B] = 0xFF;
            *(u16 *)(c + 0x14) |= 0x6000;
            func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
            return;
        }
        b->unk_0C = -x << 18;
        b->unk_10 = -y << 18;
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x8000000;
        *(s32 *)(d + 0x1C) &= ~0x40000;
        *(s16 *)(p + 0x96) = 4;
        b->unk_14 = 0xFFFD0000;
        p[0x9B] += 1;
        return;
    case 1:
        b->unk_14 += b->unk_14 >> 2;
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 8;
        p[0x9B] += 1;
        return;
    case 2:
        b->unk_0C -= b->unk_0C >> 3;
        b->unk_10 -= b->unk_10 >> 3;
        b->unk_14 -= b->unk_14 >> 3;
        if (*(s16 *)(p + 0x96) == 4 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            b->unk_0C = 0;
            b->unk_10 = 0;
            b->unk_14 = 0;
            *(u8 **)(c + 0x2C) = D_80174AE4;
            func_80047784(c, D_80174AE4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 4;
        b->unk_0C = (x << 18) + (x << 17);
        b->unk_10 = (y << 18) + (y << 17);
        func_800A56E0(0x80E);
        p[0x9B] += 1;
        return;
    case 3:
        *(s32 *)(p + 0x90) += 0x80000;
        b->unk_0C += x << 18;
        b->unk_10 += y << 18;
        if (*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        func_800A56E0(0x808);
        func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
        p[0x9B] += 1;
        return;
    case 4:
        *(s32 *)(p + 0x90) += 0x80000;
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_14 = 0;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(s32 *)(d + 0x1C) |= 0x8000000;
        *(s16 *)(p + 0x96) = 4;
        p[0x9B] = 0xFF;
        return;
    case 0xFF:
        u = *(s16 *)((u8 *)b + 0x2) - 0x20;
        b->unk_0C = (((c[0x24] << 6) - u) << 15) >> 1;
        u = *(s16 *)((u8 *)b + 0x6) - 0x20;
        b->unk_10 = (((c[0x25] << 6) - u) << 15) >> 1;
        if ((*(u16 *)(c + 0x14) & 0x8000) == 0 && *(s16 *)(p + 0x96) != 0)
        {
            for (i = 9; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    func_8004491C(n, (u32)D_80045340);
                    n->unk_10 = (u32)func_b38_80173F0C;
                    w = (u8 *)n + 0x20;
                    m = (u8 *)n->unk_0C;
                    *(s16 *)(w + 0xC) = *(u16 *)((u8 *)b + 0x2) - x * 40;
                    *(s16 *)(w + 0xE) = *(u16 *)((u8 *)b + 0x6) - y * 40;
                    *(s16 *)(w + 0x10) = *(u16 *)((u8 *)b + 0xA);
                    *(s16 *)(w + 0x4) = rand() & 0xF;
                    *(s16 *)(w + 0x6) = 0;
                    *(s16 *)(w + 0x8) = (rand() & 0xF) + 8;
                    *(s16 *)(w + 0x14) = rand();
                    *(s16 *)(w + 0x48) = 5;
                    *(s16 *)(w + 0x18) = *(u16 *)(d + 0x2A);
                    *(s16 *)(m + 0x1C) = *(s16 *)(m + 0x1E) = 0x1000;
                    *(s16 *)(m + 0x10) = 0x60;
                    *(s32 *)m = (s32)D_800DEC28;
                    *(u16 *)(m + 0x14) |= 0xC;
                    *(s32 *)(m + 0x8) = *(s32 *)(D_800DEC28 + 0x4);
                    *(s32 *)(m + 0xC) = 0xC0C0C0;
                    m[0x4] = 0;
                    m[0x5] = 0;
                }
            }
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        func_800AD594(d, 0x100);
        *(s32 *)(p + 0x8C) = (s32)func_b38_80171138;
        D_8008346C = 0;
        *(u8 **)(c + 0x2C) = D_80174AD4;
        func_80047784(c, D_80174AD4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        func_800A4ACC(d);
        if (*(s8 *)(d + 0x6D) == 0)
        {
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        else
        {
            D_800E3DE8 = d - 0x20;
        }
        return;
    }
}
