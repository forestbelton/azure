/* func_800B1768 (ovl_dungeon) -- PARK, best candidate so far.
   target 234 words, candidate 234 words, 55 aligned differences
   (ovlverify masked: 101 differing -- read aligndiff, not that number).

   SIGNATURE, for whoever takes func_800B6AC4 (which is the only `jal` to it
   anywhere and is itself parked at docs/dungeon-parks/func_800B6AC4.c):
       void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f);
   The two landed callers already pass six arguments
   (`func_800B1768(0x21, 0xE0, 0x84, 0, 0, 0)` at csrc/ovl_dungeon.c:3178 and
   `func_800B1768(0, 0x27, 0x40, 0x209, 0, 0)` at :25179), all constants, so
   adding this prototype leaves both at 0 differing -- checked by a whole-unit
   sweep of all 803 landed names.
   The fifth parameter is the stack word at 0x58($sp) copied to the local
   0x10($sp) and tested twice as a halfword; the sixth is the halfword at
   0x5C($sp) copied to 0x18($sp) and stored to q+0x26 at the end.

   NEEDS THESE DECLARATIONS at the top of the unit:
       s32 func_80069E98(u8 *);
       s8 *func_8004E298(u8 *, u8 *, s32);
       void func_800B1768(s16, s16, s16, s16, s16, s16);
       extern u8 *D_80073618[];
       extern u8 *D_800DF03C[];

   TEMPLATE: the landed func_800B1B10 (csrc/ovl_dungeon.c, "The dungeon's
   message panel") is this function one layer over -- same func_8003FD64(0x12,
   D_80083498) node, same func_800B14FC step, same func_800B06F0 tag, the same
   three func_800B12F4 chains parked in the +0x50 array off the +0x20 view,
   and the same `func_800B1320(t, e != 0 ? A : B, (s16)(-*(s8 *)(t+3) - K))`
   tail.  Its four recorded spellings all transferred, in particular that the
   +0x50 slots are written off the NODE and that the two negative bytes go
   through `s8 *`.

   RESIDUE, in three clusters:
   (1) words 107-113, 2 words misplaced.  The target computes the ternary's
       ELSE value in the branch delay slot (`addiu $a1, $v1, 0x80`); the
       candidate puts `move $a1, $v1` there, falls through the if-arm and
       needs a `j` plus `ori $a1, $a1, 0x80` after it.  The `ori` is the tell:
       the candidate's `has` is provably 0 or 1, so combine knows bit 7 of
       `has * 10` is clear.  In the target it is not provable, so some set of
       `has` has an unbounded source there.  Nothing tried reproduces it.
   (2) words 166-167 and 180-181: the target copies the value into a second
       register before subtracting 2 (`addu $v1, $a0, $zero` /
       `addiu $v1, $v1, -0x2` in and after the delay slot); the candidate
       subtracts in place.  Separate `v2`/`z2` locals for the two results give
       the copies back but cost 2 words elsewhere: 236 words / 50 aligned.
   (3) the rest is a saved-register rotation:
         candidate  d=$s7 col=$s6 j=$s5 w=$s7 (s16)e=$s5 t=$s0
         target     d=$s6 col=$s5 j=$s7 w=$s6 (s16)e=$s0 t=$s1

   MEASURED AND REJECTED
   * ALL 182 single-move permutations of the 14 local declarations were swept:
     every one is 234 words / 56 aligned or worse, so declaration order is NOT
     the tie-break here.  (That sweep ran against `s32 j`; `s16 j` is 55.)
   * `s32 j` for `d & 0xFF`: 234/56.  `s16 j`: 234/55 (adopted -- it is what
     puts the `andi` at word 52 instead of 62).  `u8 j`: 238/102.
   * `s16 base`: 232/59.  `s16 has`: 232/114.  `s16 w`: 237/62.
     `u32 has`: identical bytes to `s32 has`.
   * `has = k != 0;` 234/90; `has = i;` identical bytes (cse folds it back to
     the constant); `base = has * 5 * 2`, `0x80 + base`, and dropping the cast
     on the else arm are all identical bytes.
   * the func_800B1320 argument as a statement (`arg = ...; if (e) arg = ...`)
     is 230/75; swapping the ternary's arms 234/57; `base - w + 0x76` 234/57;
     a `b2 = base + 0x76` local 234/57.
   * one variable for the second and third chains (`u = tmp;` after
     func_800B1434 instead of a separate `t`): 235 words but only 41 aligned
     differences -- it fixes the whole $s0/$s1 rotation in cluster (3) and
     costs one extra `move`.  That is the shape to aim a permuter at.
   * `j = d & 0xFF;` at five different source positions: identical bytes at
     four of them, 236/61 when hoisted above func_8003FD64.
   * cluster (2) in LEGAL C -- separate `v2`/`z2` locals holding the `- 2`
     results -- is 236 words / 49 aligned: it restores both copies and every
     row from word 165 to the end becomes register-only, at the cost of two
     words.  A zero-width `__asm__("" : "+r"(base));` after `base = has * 10;`
     takes those two words back by blocking combine's `nonzero_bits` on
     `base`, so the else arm comes out `addiu $v0, $v1, 0x80` instead of
     `move`/`ori`; the two together are **234 words / 50 aligned**, the best
     measured.  That body is NOT the one below, because the `asm` is a device
     whose own cost is 2 words (232/60 on its own) and the clean 234/55 is
     the better permuter base.  The device line and the two locals are:

         s32 v2; s32 z2;
         base = has * 10;
         __asm__("" : "+r"(base));
         ...
         v = has == 0 ? -0x2 : -0xA;
         v2 = v;
         if (e != 0) { v2 = v - 2; }
         *(s8 *)(t + 0x2) = v2;
         ...
         z2 = z;
         if (e != 0) { z2 = z - 2; }
         t[0xA] = z2;
   * `__asm__("" : "+r"(has))` before the multiply is 235/60; after
     the func_8004E298 store it is 234/56. */
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *t;
    u32 *col;
    s32 i;
    s16 j;
    s32 has;
    s32 w;
    s32 v;
    s32 base;
    s32 z;

    i = 0;
    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    if (e != 0)
    {
        *(u16 *)(r + 0x14) |= 1;
    }
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0xC) = y;
    *(s16 *)((u8 *)m + 0xA) = 0;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    j = d & 0xFF;
    *(s16 *)(q + 0x24) = d >> 8;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    col = (u32 *)((u8 *)n + 0x58);
    u = func_800B12F4();
    *(u8 **)(q + 0x28) = u;
    if (u != NULL)
    {
        if (k != 0)
        {
            i = 1;
            col = (u32 *)((u8 *)n + 0x5C);
            *(u32 *)((u8 *)n + 0x70) = (u32)u;
            u = func_800B1434(u, D_80073618[k]);
            has = 1;
            *(u32 *)((u8 *)n + 0x58) = 0x808080;
        }
        else
        {
            has = 0;
        }
        *(s32 *)(q + i * 4 + 0x50) = (s32)func_8004E298(u, D_800DF03C[j], 0);
        i++;
        w = func_80069E98(D_800DF03C[j]) * 8;
        base = has * 10;
        func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : (s16)(base + 0x80),
                      (s16)(-*(s8 *)(u + 0x3) - 0x9));
        func_800B135C(u, e != 0 ? 4 : 8);
        func_800B13CC(u, 0x20);
        func_800B1400(u, 2);
        *col++ = 0x808080;
        u += 0x60;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        t = func_800B1434(u, u - 0x60);
        func_800B1320(u, 1, 1);
        *col = 0x80808;
        *(s32 *)(q + i * 4 + 0x50) = (s32)t;
        i++;
        t[0x1] = 0x38;
        v = has == 0 ? -0x2 : -0xA;
        if (e != 0)
        {
            v -= 2;
        }
        *(s8 *)(t + 0x2) = v;
        *(s8 *)(t + 0x3) = -0xE;
        t[0x9] = 0;
        t[0x8] = 0;
        z = w + has * 24;
        if (has == 0)
        {
            z += 4;
        }
        if (e != 0)
        {
            z -= 2;
        }
        t[0xA] = z;
        t[0xB] = 0x10;
        t[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(t + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(t + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(t + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(t + 0x14) = *(s32 *)(q + 0x1C);
        t[0x1] |= 2;
        t += 0x18;
        *(s32 *)(q + i * 4 + 0x50) = (s32)t;
        t[0x1] = 0x81;
        *(s16 *)(t + 0x4) = 0x20;
        *(s16 *)(t + 0x6) = 0;
        t[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}
