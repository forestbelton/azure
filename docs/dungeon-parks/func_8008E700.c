/* func_8008E700 (ovl_dungeon) -- 208 of 208 words, 6 ALIGNED DIFFERENCES.
   Agent j42, 2026-11-16.  The whole residue is ONE sched1 tie in a ten-word
   window at 0x8008E924..0x8008E938; words 0..134 and 143..207 are byte-exact.

   THE RESIDUE, exactly:

     cand 136 lui  v0, %hi(D_80081484)   target 138 lui  a1, %hi(D_80081484)
     cand 139 lw   t0, %lo(..)(v0)       target 139 lw   v0, 0x1C(s0)
     cand 141 sw   zero, %lo(..)(v0)     target 140 lw   t0, %lo(..)(a1)
     cand 142 lw   v0, 0x1C(s0)          target 142 sw   zero, %lo(..)(a1)

   i.e. the candidate hoists `lui %hi(D_80081484)` ONE SLOT EARLIER, into the
   gap between `lui $v1, 0xFFEF` and `ori $v1, 0xFFFF`, where the target emits
   the `ori` first.  Because the %hi then dies before `*(u32 *)(d + 0x1C)` is
   loaded, local-alloc gives it $v0 (the target's $a1) and the d+0x1C load
   slides from word 139 to 142.  No instruction is missing, wrong or extra:
   it is a priority tie between `ori $v1` and `lui %hi(D_80081484)`, both with
   a three-insn chain to a store, broken by RTL order.  ORDERING residue ->
   permuter.

   WHAT GOT IT TO 6, and these are the levers worth keeping:

   * `*(u16 *)0x80013714`, NOT the declared `D_80013714` (include/structs.h
     :2129).  The declared symbol lets reorg lift its `lui` into the delay
     slot of the `(d+0x1C & 0x300000) == 0x100000` test, where the target
     leaves a `nop`: 207 of 208 words.  The constant address is a
     plus(REG, CONST_INT) split, the `lui` is not a separate movable insn,
     and the function reaches 208/208.  Six other spellings of that test --
     nested ifs, `!(x & 1)`, `x % 2`, `(u32)x & 1`, `!= 1`, and a `- 0x100000
     == 0` form -- are all byte-identical to each other at 207 words, so it
     is the SYMBOL spelling and nothing else.  (Worth 1 word + ~10 diffs.)
   * HOISTING THE SKIN INDEX INTO A LOCAL `v`, placed between the +0x2C store
     and `D_800E3540 = s`.  Inline in the call argument the function is 15-16
     aligned; as `v` computed at that point it is 6.  Every other placement of
     the same hoist is worse: before the D_80081484 pair 43, after `t2` 12,
     after the g increment 8, last 15.
   * THREE SEPARATE POINTER LOCALS (`t`, `t2`, `g`, `g2`) -- one per symbol
     materialisation site, per the "N materialisations mean N locals" rule.
   * `*(u32 *)(d + 0x1C) &= ~0x100000` as a CAST, not `((u32 *)d)[7]`.  The
     array ref costs 9 aligned differences here (25 vs 16 at the time it was
     measured), which is the opposite sign to func_8008E504's note -- there
     the array ref is what hoists the load over the %lo(D_80081484) store; in
     this function the hoist is already there and the array ref over-schedules.
   * CASE 0 STORES THE SKIN TABLE IN BOTH ARMS, and the ternary
     `*(u8 **)(c+0x2C) = cond ? D_800DD0C8 : D_800DD048;` is wrong: the
     ternary puts the store in the merge block where cse then forwards it into
     `(*(u8 **)(c + 0x2C))[...]`, costing the target's `lw $v1, 0x2C($a0)`
     reload and two words.  Written as two stores in two arms, cross-jumping
     unifies the `sw` (which is where the target's `j .L8008E798` + delay-slot
     `addiu %lo` comes from) and the reload survives.
   * CASE 1 USES THE LOCAL-BOUND FORM `t = D_800DD060; ...; t[...]`, case 0
     the RE-READ form.  Both are in the listing and they are not interchangeable.

   MEASURED AND REJECTED (aligned differences, all at 208/208 words unless
   noted):
     tail statement order, all 60 legal permutations of
       A=`&= ~0x100000`  B=`s = D_80081484; D_80081484 = NULL`
       C=`D_800E3540 = s`  D=`g; *(u16*)(g+0xA)++`  E=`t2; c+0x2C = t2`
       with the index INLINE: best 17 (AEBDC, BDECA), worst 38, median ~25.
     with the index hoisted to `v` and B split into P/Q:
       PQAXDYVC 6, PAQXDYVC 6, PQADXYVC 6, PQXADYVC 6, PAXQDYVC 6,
       APQXDYVC 8, PQAXDVYC 8, PQAVXDYC 12, LPQXMDYVC 13 (A split), 
       PQAXYDVC 16, PQVAXDYC 27, VPQAXDYC 43.
     splitting `s = *(void**)D_80081484` from the NULL store: no change.
     a `void **q` local for the D_80081484 base: no change (same md5).
     `u32 s` instead of `void *s`, `u8 *s`, `void **s`: no change.
     declaration ORDER of the seven locals, nine permutations: no change --
       gcc 2.7.2 numbers pseudos by first use, not by declaration.
     `((u32 *)d)[7] &= ~0x100000`: 25 where the cast is 16.
     naming D_800DCFD0 twice instead of binding `t2`: no change.
     re-reading `(*(u8 **)(c + 0x2C))[...]` in the tail call: no change
       (no call between the store and the read, so cse forwards either way).
     a `u32 m = ~0x100000` local for the mask: no change.
     `&= 0xFFEFFFFF` written out instead of `&= ~0x100000`: no change.

   WHAT THE CANDIDATE HOLDS FIXED (so the next reader knows the frame):
   the signature `(u8 *p, UnkVec *b, u8 *c, u8 *d)`; `func_8009AF18` riding its
   IMPLICIT declaration (it is defined below at :10174 as
   `s16 func_8009AF18(u16, UnkVec *, u8 *, s16)` -- a real prototype makes the
   first argument `lhu` where the target has `lh`, and makes the result
   pre-sign-extended where the target has `addu $a2, $v0, $zero` + `sll 16`);
   `n = ++*(u16 *)(p + 0x96)` as an s16 local; and the four +0x124 reads being
   written out inline rather than cached, which is what the target's four
   `lw 0x124($s1)` are.

   NEW DECLARATIONS this body needs at the top of csrc/ovl_dungeon.c:
     extern u8 D_800DD048[];
     extern u8 D_800DD060[];
     extern u8 D_800DCFD0[];
   (D_800DD0C8, D_80081484, D_800E3540, D_80083228, D_80083460, func_80048A44,
   func_8009F644, func_800A56E0, func_80099F04, func_80099F70, func_8008ACDC
   and func_8008EAC8 are all already declared.)

   Measured with
     python3 tools/decomp/aligndiff.py <unit copy> func_8008E700
     python3 tools/decomp/ovlverify.py <unit copy> func_8008E700
   -> 208 of 208 words, 7 differing masked, 6 aligned. */

void func_8008E700(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *t;
    u8 *t2;
    void *s;
    s16 n;
    s32 r;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            b->unk_14 = -0x160000;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u8 **)(c + 0x2C) = D_800DD0C8;
            }
            else
            {
                *(u8 **)(c + 0x2C) = D_800DD048;
            }
            func_80048A44(
                c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        n = ++*(u16 *)(p + 0x96);
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0 && n >= 4)
        {
            b->unk_14 = 0;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            }
            else
            {
                t = D_800DD060;
                *(u8 **)(c + 0x2C) = t;
                func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            p[0x9B]++;
        }
        else if ((*(u32 *)(d + 0x1C) & 0x300000) == 0x100000 && (*(u16 *)0x80013714 & 1) == 0)
        {
            r = func_8009AF18(*(s16 *)(d + 0x2A), b, c, 8);
            if ((s16)r != 0)
            {
                ((u32 *)*(u8 **)(p + 0x124))[7] |= 0x200000;
                ((u32 *)d)[7] |= 0x200000;
                *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
                (*(u8 **)(p + 0x124))[0x84] = 0x78;
                (*(u8 **)(p + 0x124))[0x85] = 2;
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                *(u32 *)(d + 0x1C) &= ~0x100000;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(c + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);
            }
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0xA) == 0)
            {
                if ((*(u32 *)(d + 0x1C) & 0x200000) != 0)
                {
                    *(u32 *)(d + 0x1C) &= 0xFFDFFFFF;
                    *(u16 *)(g2 + 0x2) |= 0x412;
                    func_80099F70(*(void **)(d + 0x5C));
                    func_80099F04(*(void **)(d + 0x5C));
                }
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}
