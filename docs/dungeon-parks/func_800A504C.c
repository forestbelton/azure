/* PARK func_800A504C -- agent j51, 2026-11-16.

   RESIDUE: target 401 words, candidate 401 words, 64 differing (masked);
   aligndiff 64. The STRUCTURE is exact: every instruction, every schedule
   slot and every stack displacement agrees. ~58 of the 64 are ONE register
   rotation -- the target has `p` in $s3 and the `n == 1` gate pseudo in $s2,
   this body has them the other way round. The rest:

   * 1 word at 0x800A52D4 / 0x800A54C4: `lui %hi(D_800814A8)` is one slot
     later here than in the target (it sits after the `sll $v0, 16` instead
     of before it), in both case-2 blocks.
   * 1 word at 0x800A544C: the shared `D_800DCF5E++` tail fills its own
     load delay with `addu $v0, $zero, $zero` where the target leaves a
     `nop` and reaches the shared `return 0` block instead.
   * 2 words at 0x800A54AC/0x800A54B4: same cause -- the `q == NULL` early
     exit threads its own `v0 = 0` into the delay slot.

   THE REGISTER ROTATION, DIAGNOSED FROM .greg/.lreg (tools/decomp/dump.sh):
     allocnos  83 q (71 refs/190 insns, pri 2.242) -> $s0
               84 w (19/138, 0.551)               -> $s1
               81 p (38/612, 0.3105)              -> $s2   <-- target: $s3
               85 d (7/63,   0.222)               -> $s3
               82 g (11/199, 0.166)               -> $s4
               98 the gate 1 (5/137, 0.073)       -> $s3 (shares with d)
   global.c's allocno_compare is floor_log2(refs)*refs/live_length, and the
   target needs an allocno with priority in (0.3105, 0.551) that conflicts
   with `p` to take $s2 ahead of it. `d` is the only candidate -- 8 refs over
   63 insns would be 0.381 -- but every spelling measured leaves it at 7,
   because cse2 canonicalises `d` back to `q` for every use BEFORE the block
   move (make_regs_eqv keeps `q` as qty_first_reg: q's last use is later),
   so only the six field stores after the loop survive as references to `d`.
   In the TARGET those pre-loop uses are NOT canonicalised: 0x800A5194
   `addu $a3, $s2, $zero` and 0x800A51A0/0x800A51A4 `lw $t1/$t2, 0x5C/0x58($s2)`
   all read `d`, not `q`. Finding the spelling that keeps `d` canonical there
   is the whole remaining landing.

   SPELLINGS MEASURED AND REJECTED (all at 401 words unless stated):
     baseline (first draft, u8* casts throughout)          409 / 299
     + `q -= 0x20` instead of a separate `e` local         404 / 132
       (a separate local for q-0x20 is hoisted early by
        sched and buys a SIXTH callee-saved register)
     + `id = q[0x13]` local for the two 0x80010000 stores  (-2 words)
     + `m = *(u16 *)(p + 0x2A)` local                      (-1 word)
     + tail order `((u16*)w)[0xA] &= 0xFFFE;` BEFORE
       `D_80080A90 = D_8014A000;`, and `D_800DCF5C = 1`
       AFTER `((u16*)g)[5] -= 1`                            401 / 70
     + reuse the `g` local as the 0x8C-field source
       instead of a separate `s`                            401 / 64  <-- this body
     `s16 n` gate local + `D_800DCF5C = n`                  402 / 397
     `p = arg` parameter-copy device                        401 / 91
     `d = q; s = p;` order swapped                          401 / 64 (identical)
     4-parameter signature; `((u32 *)p)[5]` for the 0x4000
       test; `((s16 *)g)[5]` for the gate; `s32 a` first
       parameter; `s32 k`; splitting `q`/`w` into two
       locals each                                          401 / 64 (all identical)
     `d`-based pre-loop reads/block move (3 combinations)   401 / 64 (cse undoes them)
     `g = p; d = q;` moved down to the field copies         399 / 336 (copies die)

   NEW DECLARATIONS THIS BODY NEEDS:
     extern u8 D_800DCF5C;
     extern s16 D_800DCF5E;
     extern u8 D_8008149C[];
*/

s32 func_800A504C(u8 *a, u8 *p)
{
    u8 *g;
    u8 *q;
    u8 *w;
    u8 *d;
    u8 *e;
    u8 *z;
    s16 k;
    s16 i;
    u16 m;
    s32 r;
    u8 id;
    s32 x;
    s32 y;
    s32 f;

    g = (u8 *)&D_80083460;
    if (*(s16 *)(g + 0xA) != 1)
    {
        return 0;
    }
    if (func_8003FA44(8) == 0)
    {
        return 0;
    }
    if ((*(u32 *)(p + 0x14) & 0x4000) != 0)
    {
        switch (D_800DCF5E)
        {
        case 0:
            if (func_80053EF0(4) != 0)
            {
                return 0;
            }
            q = func_800A1618(p[0x13], 2);
            if (q == NULL)
            {
                return 0;
            }
            D_80081488 = (u32)func_80047DB8(p[0x13]);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)&D_800E3E40);
            D_800DCF5E++;
            return 0;
        case 1:
            if (D_800E3E40 == 0)
            {
                return 0;
            }
            func_80047E30();
            D_800DCF5E++;
            return 0;
        case 2:
            q = func_800A1618(p[0x13], 2);
            if (q == NULL)
            {
                return 0;
            }
            w = *(u8 **)(p - 0x14);
            q = ((u8 * (*)(s32, s32, s32, s32))func_800A0B94(p[0x13], q, 1))(
                6, w[0x24], w[0x25], (s16)(*(u16 *)(p + 0x88) - 0x20));
            if (q == NULL)
            {
                return 0;
            }
            g = p;
            d = q;
            x = *(s32 *)(q + 0x5C);
            y = *(s32 *)(q + 0x58);
            *(Unk42710 *)q = *(Unk42710 *)p;
            *(s32 *)(q + 0x4C) = 0;
            *(s32 *)(q + 0x50) = 0;
            *(s32 *)(q + 0x5C) = x;
            *(s32 *)(q + 0x58) = y;
            func_80042640((Unk42710 *)q, q[0x13]);
            w = *(u8 **)(q - 0x14);
            *(s16 *)(w + 0x1E) = 0x1000;
            *(s16 *)(w + 0x1C) = 0x1000;
            *(u32 *)(q + 0x14) &= ~7;
            *(u32 *)(q + 0x1C) &= ~7;
            *(u32 *)(q + 0x14) |= *(u32 *)(p + 0x14) & 7;
            *(u32 *)(q + 0x1C) |= *(u32 *)(p + 0x1C) & 7;
            func_800424E0(q, q[0x13], NULL);
            func_80042710((Unk42710 *)q, (Unk42710 *)p);
            *(u32 *)(q + 0x14) &= ~0x4000;
            *(u32 *)(d + 0x8C) = *(u32 *)(g + 0x8C);
            *(u32 *)(d + 0x90) = *(u32 *)(g + 0x90);
            *(u16 *)(d + 0x94) = *(u16 *)(g + 0x94);
            *(u16 *)(d + 0x98) = *(u16 *)(g + 0x98);
            d[0x9A] = g[0x9A];
            d[0x9B] = g[0x9B];
            k = (s16)func_800A1BD0(p);
            *(s32 *)(D_800814A8 + k * 4 + 0xAC) = (s32)q;
            func_8009A3D0(w[0x24], w[0x25], 0x300);
            func_8009A028(q);
            q -= 0x20;
            func_80044A50((UnkNode *)q);
            *(u32 *)(q + 0x10) |= 0x80000000;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800DCF5E = 0;
            D_800814A0 |= 0x8000;
            return 0;
        }
        return 0;
    }
    switch (D_800DCF5E)
    {
    case 0:
        q = func_800A1618(p[0x13] - 1, 3);
        if (q != NULL)
        {
            p[0x13]--;
            func_8003E188(p[0x13], 0);
            D_80080A90 = D_80023000;
            D_80081488 = func_80048118(p[0x13], D_8008149C);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)&D_800E3E40);
        }
        D_800DCF5E++;
        return 0;
    case 1:
        if (D_800E3E40 == 0)
        {
            return 0;
        }
        if (D_80081488 != 0)
        {
            func_80047FF4(p[0x13], D_80081488);
        }
        D_800DCF5E++;
        return 0;
    case 2:
        q = func_800A1618(p[0x13], 3);
        if (q == NULL)
        {
            return 0;
        }
        w = *(u8 **)(p - 0x14);
        q = ((u8 * (*)(s32, s32, s32, s32))func_800A0B94(p[0x13], q, 1))(
            1, w[0x24], w[0x25], (s16)(*(u16 *)(p + 0x88) - 0x20));
        if (q == NULL)
        {
            return 0;
        }
        *(u32 *)(q + 0x14) |= 0x4000;
        k = (s16)func_800A1BD0(p);
        e = *(u8 **)(D_800814A8 + k * 4 + 0xD0);
        *(s32 *)(D_800814A8 + k * 4 + 0xAC) = (s32)q;
        i = e[3] & 0x1F;
        e[0] = p[0x13];
        D_800E3DF0[i] = (s32)q;
        w = *(u8 **)(q - 0x14);
        *(s16 *)(w + 0x1E) = 0x1000;
        *(s16 *)(w + 0x1C) = 0x1000;
        func_80042640((Unk42710 *)q, q[0x13]);
        *(u32 *)(q + 0x14) &= ~7;
        *(u32 *)(q + 0x1C) &= ~7;
        *(u32 *)(q + 0x14) |= *(u32 *)(p + 0x14) & 7;
        *(u32 *)(q + 0x1C) |= *(u32 *)(p + 0x1C) & 7;
        func_800424E0(q, q[0x13], NULL);
        func_80042710((Unk42710 *)q, (Unk42710 *)p);
        if (q[0x43] < 0x40)
        {
            z = (u8 *)0x80010000;
            r = q[0x43];
            id = q[0x13];
            *(u8 *)(z + r * 0x54 + 0xA93) = id;
            *(u8 *)(z + r * 4 + 0x980) = id;
        }
        func_800A152C(q[0x13], 3);
        func_80042560((s8 *)q);
        m = *(u16 *)(p + 0x2A);
        *(u16 *)(q + 0x2A) = m;
        func_80047738(w, (*(u8 **)(w + 0x2C))[((D_80083228 + (s16)m + 0x100) >> 9) & 7],
                      *(s8 *)(w + 0x4));
        ((u16 *)w)[0xA] &= 0xFFFE;
        D_80080A90 = D_8014A000;
        ((u16 *)g)[5] -= 1;
        D_800DCF5C = 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        f = D_800814A0;
        *(s32 *)(p + 0x90) = 0;
        D_800814A0 = f | 0x8000;
        break;
    }
    return 0;
}
