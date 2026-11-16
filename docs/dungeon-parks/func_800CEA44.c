/* PARK: func_800CEA44, 240 of 242 words, 128 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  The kind-2 node's per-frame handler,
   parked at the node's +0x10 by the landed func_800CEE0C
   (`*(u32 *)(n + 0x10) = (u32)func_800CEA44;`, csrc/ovl_dungeon.c:35836), so
   `p` is `(u8 *)n + 0x20` and the `lhu $v0, -0x2($s7)` near the tail is
   UnkNode.unk_1E reached backwards off that pointer -- the same +0x20
   convention docs/town-sweep.md records for func_80040190's cast.

   WHAT IT DOES.  A three-state machine on the halfword at `p + 0x6`.
   State 0 spends the D_800E3648 slot for room `*(s16 *)(p + 0xC)`: for each
   of its `*(s8 *)(e + 2)` charges it rolls a random pair out of
   D_80083460's +0x18 table, asks func_800A1618 for a template, then tries up
   to sixteen random tiles inside the D_800E2970 room record for one that
   func_8009A350 reports with no 0xB700 flag, and spawns there through the
   function pointer func_800A0B94 RETURNS -- the `jalr $v0` at 0x800CEBF0,
   called with (0, (u8)a, (u8)c, -0x400).  It then drains the spawn's +0x18
   budget through func_800A1D4C and stamps +0x9A/+0x8C/+0x1C/+0x92.  State 1
   counts down +0x4 and plays the message; the default state counts down,
   clears the slot and sets the two 0x8000 request bits.

   DECLARATIONS THIS NEEDS -- EXACTLY TWO:
     extern u8 D_800E1C3A[];
     extern u32 D_800835E4[];
   D_800E1C3A is a message string: its four neighbours D_800E1BF3, D_800E1C03,
   D_800E1C14 and D_800E1C25 are all declared `extern u8 D_XXXX[];` in this
   unit and all four are passed to func_80099844 as strings, so it is its own
   symbol and not `D_800E1C25 + 0x15`.  D_800835E4 is a resident u32 table,
   indexed twice -- once at the roll's +0x1 byte and once at base+1 -- and the
   two comparisons against it are `sltu`, which is what fixes it as `u32`.
   func_800A1D4C needs NOTHING: it is still an INCLUDE_ASM stub (:14185) with
   no declaration anywhere, and the unit's four landed call sites (:3257,
   :19390, :23882, :28107) all ride the implicit one.  Declaring it measured
   the same, so this body rides it too.
   NO PROTOTYPE CHANGE.  `void func_800CEA44();` at :1044 already fits, and
   the only other mention of the name is the install, which takes its address.

   ------------------------------------------------------------------ residue

   TWO MISSING WORDS, with four spurious ones and six missing ones that do not
   fully cancel in `aligndiff`'s alignment.  The control flow, all three
   states, both loops and every field offset already match.  What is left:

   (1) FOUR SPURIOUS WORDS this body emits.
       * `sll $v0, $v0, 16` after the `sh` of the D_80082EA6 byte, because the
         `if` tests the HImode LOCAL where the target tests the SImode
         expression.  Feeding both from an `s32 kk` intermediate removes it
         and costs three words elsewhere: 237 words / 229 masked.  The target
         also reads the byte with `lbu` + `sll 24` + `sra 24` where every
         spelling here folds to `lb`; `k = D_80082EA6;` (this body),
         `(s8)*(u8 *)&D_80082EA6`, `*(s8 *)&D_80082EA6`, `(s8)` of a
         `u8`-aliased symbol and `(x << 24) >> 24` all measure 240 / 217.
       * `lui`/`addiu %lo(D_800835E4)` a SECOND time, because this body keeps
         `tb` live into the guard (see (3)) so two symbol pseudos are
         rematerialised where the target has one and derives the second
         pointer destructively with `addiu $v1, $v1, 0x4`.
       * one extra `lw` of `tb[y]`, same cause.

   (2) SIX MISSING WORDS.  The identified ones are the `nop` in
       `jal func_800A6D30`'s delay slot at 0x800CEAD8 -- the target
       materialises `&D_80083460` AFTER the call into the CALLER-saved $t0, so
       nothing can fill the slot, while this body's `g` is a function-wide
       local and lands in $s0 where the `addiu %lo` fills it -- and
       `addu $s1, $v1, $zero` + `addu $a0, $s0, $zero` + `lw $v0, 0x18($s0)`
       around the func_800A1D4C loop.

   (3) THE ONE DEVICE-SHAPED SPELLING, AND WHAT IT BUYS.  `(void)&k;` is worth
       FOUR words (236 -> 240).  Without it gcc's PROMOTE_MODE keeps the
       `(s8)D_80082EA6` value in an SImode pseudo and spills it with `sw`/`lw`;
       the target stores it with `sh $t0, 0x18($sp)` and reloads it with
       `lhu` + `sll 16` + `sra 16`, which is an HImode STACK SLOT, and on this
       target only an addressable local gets one.  I could not find a
       non-addressable spelling that produces the HImode slot: `s16 k`,
       `u16 k` with `(s16)k` at the use, the `(x << 24) >> 24` form and a
       `u8`-aliased symbol all compile to `lb` + an SImode spill.
       The remaining frame difference follows from it: the target puts `w` at
       0x10 and this k at 0x18 with six bytes between them, while this body
       gets 0x10/0x12 and a 0x40 frame against the target's 0x48.  Swapping
       the declaration order of `w` and `k` moves which one is first but never
       opens the gap, so something else occupies 0x12..0x17 in the original.

   MEASURED AND REJECTED:
     plain `while (*(u32 *)(o + 0x18) >= tb1[o[0x11]])`      230 / 235 masked
     `if (u >= ...) do { } while (...)`                      230 / 235
     `if (*(u32 *)(o + 0x18) >= ...) do { } while (...)`     230 / 235
     `tb1` computed before the store                         230 / 235
     THE GUARD RE-READING `tb[y]` (this body)                236 / 234
     `tb++` instead of `tb1 = tb + 1`, guard on `u`          232 / 224
     `(void)&k` on top of the re-read guard                  240 / 217
     `s32 kk` intermediate for the byte test                 237 / 229
     `(u8 *)&D_80083460` inlined at the use                  240 / 220
     `k = (s8)D_80082EA6_u` with the test on the expression  239 / 222
     `g = (u8 *)&D_80083460;` moved after func_800A6D30      236 / 234

   THE GUARD RE-READ IS THE LOAD-BEARING ONE AND IT IS WORTH SIX WORDS.
   Written as a plain `while`, or as a guarded `do`/`while` whose guard uses
   the value just stored, gcc emits `j <test>` into the loop's own test and
   the duplicated exit test the target has (0x800CEC50..0x800CEC68, seven
   words) never appears.  Re-reading `tb[y]` in the guard makes the two tests
   different expressions so they cannot be unified.  This is the same shape
   docs/dungeon-sweep.md records for func_800B7774 -- "the guard's `sltu`
   reads the stored register while the bottom test reloads, the two tails
   differ BY CONSTRUCTION" -- except that here the difference has to be put in
   by hand, which is why the re-read is probably NOT the original spelling and
   is where the next attempt should start.

   WHAT THIS CANDIDATE HOLDS FIXED: the three-state if/else-if chain (a
   `switch` was not measured), the `b = D_800E3648; e = b + i * 4;`
   two-statement form at all three sites against the inline
   `D_800E2970 + k * 0x14` at the room record (both read off the listing: the
   symbol's lui/addiu is BEFORE the index chain for D_800E3648 and AFTER it
   for D_800E2970 -- see func_8009FCAC's comment at csrc/ovl_dungeon.c:13125),
   `s16 i` for the sixteen-try counter with `s32 n` for the charge counter,
   and `u16 w` with `&w` as func_8009A350's fourth argument, which is the
   unit's established spelling (csrc/ovl_dungeon.c:4268). */

void func_800CEA44(u8 *p, UnkVec *v, u32 q)
{
    u8 *b;
    u8 *e;
    u8 *t;
    u8 *r;
    u8 *o;
    u8 *g;
    u32 *tb;
    u32 *tb1;
    u32 u;
    s32 n;
    s32 j;
    s16 i;
    u16 w;
    u16 k;
    s32 a;
    s32 c;
    s32 x;
    s32 y;
    u8 *s;

    (void)&k;
    if (*(s16 *)(p + 0x6) == 0)
    {
        k = D_80082EA6;
        if ((s16)k >= 0)
        {
            b = D_800E3648;
            e = b + *(s16 *)(p + 0xC) * 4;
            n = *(s8 *)(e + 2);
            while (n >= 0)
            {
                if (func_8003FA44(3) == 0)
                {
                    b = D_800E3648;
                    e = b + *(s16 *)(p + 0xC) * 4;
                    e[2] = e[2] - n;
                    return;
                }
                j = func_800A6D30() & 0x1E;
                g = (u8 *)&D_80083460;
                r = *(u8 **)(g + 0x18) + j;
                x = r[0];
                y = r[1];
                s = func_800A1618(x, 1);
                if (s != NULL)
                {
                    i = 0xF;
                    t = D_800E2970 + (s16)k * 0x14;
                    while (1)
                    {
                        a = func_800A6DA4(0, (u16)(*(u16 *)(t + 4) - 1)) + t[0];
                        c = func_800A6DA4(0, (u16)(*(u16 *)(t + 6) - 1)) + t[2];
                        if (func_8009A350((u8)a - 1, (u8)c, 0, &w) != 0 && (w & 0xB700) == 0)
                        {
                            break;
                        }
                        i--;
                        if (i < 0)
                        {
                            break;
                        }
                    }
                    if (i >= 0)
                    {
                        D_800E2968 = 1;
                        o = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(x, s, 1))(0, (u8)a, (u8)c,
                                                                                    -0x400);
                        if (o != NULL)
                        {
                            func_800A152C(x, 1);
                            func_80042640((Unk42710 *)o, x);
                            o[0x43] = func_800A6D30();
                            tb = D_800835E4;
                            o[0x12] = 0;
                            u = tb[y];
                            *(u32 *)(o + 0x18) = u;
                            tb1 = tb + 1;
                            if (tb[y] >= tb1[o[0x11]])
                            {
                                do
                                {
                                    func_800A1D4C(o, 0);
                                } while (*(u32 *)(o + 0x18) >= tb1[o[0x11]]);
                            }
                            o[0x9A] = 0x16;
                            *(u32 *)(o + 0x1C) &= 0xF7FBFFFF;
                            *(u32 *)(o + 0x8C) = 0;
                            *(u16 *)(o + 0x92) -= 0x200;
                        }
                    }
                }
                n--;
            }
        }
        *(s16 *)(p + 0x4) = 0xA;
        *(u16 *)(p + 0x6) += 1;
    }
    else if (*(s16 *)(p + 0x6) == 1)
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_800997FC(D_800E1C3A);
            func_800419EC(0xC, 0x18);
            func_800A56E0(0x602);
            *(s16 *)(p + 0x4) = 0x10;
            *(u16 *)(p + 0x6) += 1;
        }
    }
    else
    {
        *(u16 *)(p + 0x4) -= 1;
        if (*(s16 *)(p + 0x4) < 0)
        {
            func_8004437C(0, 1);
            func_80053DA8(0x200);
            b = D_800E3648;
            e = b + *(s16 *)(p + 0xC) * 4;
            g = (u8 *)&D_80083460;
            *(u32 *)e = 0;
            *(u16 *)(g + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}
