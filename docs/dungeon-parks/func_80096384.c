/* PARKED 2026-11-16, j50.  target 397 words, candidate 397 words,
   17 differing (ovlverify masked); aligndiff 17 aligned differences, every
   instruction positionally identical.

   THE WHOLE RESIDUE IS ONE REGISTER PAIR, EXCHANGED.  `want` -- the `s16`
   holding `(m[1] & 7) << 9` -- lands in $a1 where the target has $a0, and
   the raw `*(u16 *)(d + 0x2A)` read lands in $a0 where the target has $a1.
   Everything else (the three `andi`s on each, the `addu` copy the compare
   needs, the join copy, the `sll 16`/`sra 16`, the `and` against -0x1000 and
   the final `slt`) is byte-identical apart from those two register names, and
   the only ordering difference -- `sra`/`lh`/`lhu`/`subu` against
   `lh`/`sra`/`subu`/`lhu` at words 137..140 -- falls out of it.

   DIAGNOSIS FROM THE DUMPS (tools/decomp/dump.sh, -G0).  Both are GLOBAL
   allocnos and `global_alloc` assigns in `allocno_compare` order, which is
   `(floor_log2(refs) * refs - live_length) / live_length`:

     want      = pseudo 98,  7 refs across 28 insns -> (2*7 - 28)/28 = -0.500
     the read  = pseudo 154, 4 refs across  9 insns -> (2*4 -  9)/9  = -0.111

   so 154 is assigned first (7th of 28) and takes $a0 while 98 is 16th and
   takes $a1.  `floor_log2(7)` is 2, not 3, which is what costs `want` the
   slot: at EIGHT references and a live_length of 26 or less it wins, and at
   NINE it wins at the current length.  Nothing I could spell gave it an
   eighth reference without also changing the instruction stream -- the eighth
   reference the target must have is most likely the compare's operand, which
   in my build is a SEPARATE pseudo (159, a re-materialised `ashift` that
   regmove later collapses into the `addu $v0, $a0, $zero` copy) rather than a
   use of `want`.

   DECLARATIONS THIS BODY NEEDS (in the agent's report, and both are the only
   new names):
       extern u8 D_800245A8[];
       extern void (*D_800DD830[])(void);
   NO prototype change: `void func_80096384();` at csrc/ovl_dungeon.c:670 is
   unprototyped and compatible with this definition.

   SPELLINGS THAT ARE ALREADY LOAD-BEARING HERE, all measured:
   (1) `s16 want;` -- the catalogue's `:25519` entry.  As `s32` the byte folds
       and the function is 396 words; the narrow destination keeps the
       surviving `andi 0xFF`, buys the `addu $v0, $a0, $zero` copy, and raises
       nonzero_bits(want) past 0xFFFF so that `want & ~0xFFF` comes out as the
       target's `addiu $v0, $zero, -0x1000` + `and` instead of one `andi`.
       Using a sign-extended copy for that `and` is 396 words.
   (2) THE SWITCH ARMS IN VALUE ORDER 8, 0x10, 0x18, 0xD8 -- the catalogue's
       `:27053` entry, worth 10 differing.
   (3) THE `want` SIGN EXTENSION IS A TERNARY, NOT AN IF/ELSE.  Written as
       `if (want & 0x800) want |= 0xF800; else want &= 0x7FF;` each arm stores
       into `want` itself, so there is no join copy and the if-arm needs a
       `j`/`nop`: 398 words.  The ternary gives the target's
       `bnez`/`ori`(delay)/`andi`/`addu $a0, $v0, $zero`.  The SAME construct
       on `*(s16 *)(d + 0x2A)` must stay an if/else, because there the common
       store is the join.
   (4) `*(s16 *)(d + 0x2A) &= 0xFFF;` as a COMPOUND ASSIGNMENT, not through an
       `n` local: 26 -> 23 differing, because the compound form lets cse fold
       the two following masks back onto the raw halfword the way the target
       does.
   (5) ONE POINTER LOCAL PER D_800DD274 ARM (`t` at the head, `t2` in the
       tail).  A single `t` assigned in both places is a global allocno and its
       `%hi`/`%lo` pair splits across two registers -- the catalogue's "A
       POINTER LOCAL ASSIGNED IN TWO ARMS" entry.  Worth 6 differing.
   (6) `cur = *(u16 *)(d + 0x2A);` read BEFORE the `delta` subtraction, so the
       `lhu` sits beside the `lh` in the same block and fills its load-delay
       slot; without it the reload lands after the `subu`.  Worth 3 differing.

   ALSO MEASURED AND REJECTED (all 397 words / 17 differing unless stated):
     `want = m[1] & 7; want <<= 9;`                       396 words
     `(s16)((m[0x1] & 7) << 9)`                           no change
     `want` and the compare operands swapped              18
     `(*(u16 *)(d + 0x2A) & 0xFFF) != want`               no change
     `want < *(s16 *)(d + 0x2A)` for the `>` test         no change
     a `wext` local for the sign-extended value           no change (396 if it
                                                          also takes the `and`)
     an explicit `cur0` local for the raw halfword        no change
     `cur0` reused for the `& 0xFFF` in the `> 0x800` arm 396 words (it removes
                                                          the target's reload)
     `cur` placed after `delta`, or after the abs         17 / 20
     `delta >= 0x801` instead of `> 0x800`                no change
     `u16 cur`, `s16 delta`, and six declaration orders   no change */
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
        *(void ***)(c + 0x2C) = t;
        func_8003DB94(c, t[((g->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        p[0x9A] = 0x32;
        *(u16 *)(p + 0xA2) &= 0xFEFF;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        p[0x9B] = 0;
        *(s16 *)(p + 0x96) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) &= 0xFF7F;
    r = *(s16 *)(d + 0x64);
    if (r < 0)
    {
        func_80097898(p, b, c, d);
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
        func_80096AC0(p, b, c, d);
        return;
    }
    e = *(s32 *)(d + 0x1C);
    if ((e & 0x200) != 0)
    {
        func_80097C50((UnkDungeonActor *)p, b, c, d);
        return;
    }
    if ((*(u16 *)(q + 0x2) & 0x4) == 0)
    {
        if ((e & 0x20) != 0)
        {
            func_800969B8(p, b, c, d);
            return;
        }
        k = *(u16 *)0x80013714;
        if ((k & 1) != 0)
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
                want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                cur = *(u16 *)(d + 0x2A);
                delta = *(s16 *)(d + 0x2A) - want;
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
        if ((s16)func_80094EA4() != 0)
        {
            *(s16 *)(p + 0xA4) = 0;
        }
        if ((*(u16 *)(p + 0xA2) & 0x200) != 0 && (s16)func_800A2C34(d) == 0 &&
            (*(u16 *)(q + 0x2) & 0x4) == 0 &&
            (s16)func_800A4474(c[0x24], c[0x25]) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFDFF;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
        f = g->unk_08;
        if ((f & 0x80) != 0)
        {
            *(s16 *)(d + 0x8A) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = NULL;
            *(s32 *)(p + 0xC8) = 0;
            *(s32 *)(p + 0x104) = 0;
            return;
        }
        if ((f & 3) != 0)
        {
            func_800A56E0(0x506);
            return;
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
                func_80096C24(p, b, c, d);
                if (p[0x9A] != 0x32)
                {
                    return;
                }
            }
            else
            {
                *(s16 *)(p + 0x96) = 0;
            }
        }
        w = g->unk_08;
        if ((w & 0x30) == 0x30)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            func_800969B8(p, b, c, d);
            return;
        }
        if ((*(u16 *)(p + 0xA2) & 0x40) != 0)
        {
            if ((w & 0x40) == 0)
            {
                *(u16 *)(p + 0xA2) &= 0xFFBF;
            }
        }
        else if ((w & 0x40) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((s16)func_800973F0(p, b, c, d) != 0)
            {
                return;
            }
        }
    }
tail:
    if ((*(u16 *)(p + 0xA2) & 0x1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        t2 = D_800DD274;
        *(void ***)(c + 0x2C) = t2;
        func_8003DB94(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}
