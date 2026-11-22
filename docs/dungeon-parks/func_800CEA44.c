/* PARK: func_800CEA44, 242 of 242 words, 3 aligned differences.

   ====================================== 2026-11-21 (session 2): 112 rows -> 3

   The body was REWRITTEN.  WORD COUNT IS NOW EXACT and all nineteen remaining
   rows are REGISTER NAMES -- no instruction is missing, spurious or in the
   wrong order except one `addu` operand pair.  Every device the park carried
   is gone except `c++; c--;`.  Each finding below was measured on the body the
   one before it produced, on a tools/decomp/onefunc.py standalone that
   reproduces the real unit exactly (re-checked against the unit at 112, 240
   and 242 words).

   (1) THE FRAME IS 0x48 BECAUSE THE LOCAL AREA IS 16 BYTES, NOT 4.  The park
       had `w` and `k` as two adjacent 2-byte locals -> vars=8 -> frame 0x40,
       which cost ~23 rows of prologue/epilogue offsets on its own.  The
       target puts `w` at 0x10 and `k` at 0x18 with six bytes between them.
       That gap is reload's: `k` is an HImode PSEUDO that gets SPILLED, and
       its only use is `(s16)k`, which mips.md's `extendhisi2`
       (config/mips/mips.md:2559-2577) expands as
       `gen_lowpart (SImode, operands[1])` -- a PARADOXICAL subreg.
       scan_paradoxical_subregs (reload1.c:3772) sets reg_max_ref_width[k] = 4,
       so alter_reg (reload1.c:2495) calls `assign_stack_local (GET_MODE (k),
       total_size, inherent_size == total_size ? 0 : -1)` with align == -1,
       and assign_stack_local (function.c) turns align == -1 into
       BIGGEST_ALIGNMENT (mips.h:1252 = 64 bits) AND rounds the SIZE up to 8:
         w at 0x10, 2 bytes -> frame_offset 2 -> CEIL_ROUND(2,8) = 8
         -> k at 0x18, size 8 -> frame_offset 16 -> vars 16 -> frame 0x48.
       `(void)&k` -- what the park had -- makes k an ordinary addressable
       2-byte local at align 2 and the gap never opens.

   (2) `(void)&k` ALSO HID THE SPILL FORM.  A spilled HImode pseudo under a
       paradoxical subreg reloads as `lhu` + `sll 16` + `sra 16`; an
       addressable `u16 k` read as `(s16)k` is one `lh`, because combine
       re-forms extendhisi2_internal.  The target has the three-insn form.
       Same at the store: `sra $t0,$v0,24` + `sh $t0,0x18($sp)` is reload
       writing the spill, which is why BOTH ends use $t0.

   (3) THE `if (q)` DEVICES WERE THE BUG, NOT THE FIX.  `q` is never read by
       the target -- $a2 appears nowhere in the listing -- and the park's two
       duplicated `if (q)` arms cost `sw $a2` in the prologue plus two `lw`
       reloads.  Removing the one around `t = D_800E2970 + k * 0x14` is also
       what lets the k spill happen at all.

   (4) ONE LOCAL PER SITE FOR D_800E3648 AND D_80083460.  The park reused one
       `b`/`e` pair at all three D_800E3648 sites and one `g` at both
       D_80083460 sites.  One variable is ONE pseudo spanning three basic
       blocks, so global_alloc gives it a callee-saved register and the
       symbol's `addiu` writes $s0 at every site; the target materialises the
       symbol into a dead temp at each site.  Separate locals (b/e, b2/e2,
       b3/e3, g/g2): 81 -> 69 rows, and the word count back to 242.  Same
       finding csrc/ovl_dungeon_800CABA8.c:211 records for func_800CB170.

   (5) BOTH func_800A6DA4 CALLS BEFORE EITHER `t[...]` LOAD.  The target
       loads t[0] and t[2] AFTER the second call; `a = call() + t[0];
       c = call() + t[2];` loads t[0] between the two calls, because a load
       cannot be scheduled across a call.  Splitting into
       `a = call(); c = call(); a = a + t[0]; c = t[2] + c;` is 69 -> 52.

   (6) `b3 = D_800E3648;` BELONGS AFTER THE TWO CALLS in the default state.
       The park had moved it above them (worth a row on the old body); with a
       per-site local it costs a callee-saved register.  52 -> 45.

   (7) THE do/while GUARD: REVERSE THE COMPARISON, DO NOT RE-READ.  The park
       re-read `tb[y]` in the guard to stop gcc unifying the guard with the
       loop test.  The real reason they do not unify in the target is that
       the loop test loads `*(u32 *)(o + 0x18)` AFTER the limit: written as
       `tb[o[0x11]] <= *(u32 *)(o + 0x18)` the limit is evaluated first, so
       the reload of o+0x18 lands between the limit load and the `sltu`, and
       jump2's cross-jumping has no common tail to merge.  Written the other
       way round gcc emits `j` into the loop's own test and the target's
       seven-word duplicated exit test never appears.  45 -> 35.

   (8) `g = (u8 *)&D_80083460;` BEFORE `j = func_800A6D30() & 0x1E;`.  Worth
       SIX WORDS and 10 rows (234/44 -> 240/34), and it REPLACED a `g++; g--;`
       device that had been doing the same job.  With `g` written after the
       call the symbol+0x18 folds to `lui %hi(D_80083460+0x18)` /
       `lw %lo(...)`, two words where the target has three
       (`lui`/`addiu`/`lw 0x18($t0)`) -- and because that is two fewer insns
       inside the `while (n >= 0)` loop, loop.c's desirability test
       `threshold * savings * m->lifetime >= insn_count` (loop.c:1730) flips
       and `(s16)k * 0x14` gets HOISTED out of the loop, which undoes (1) and
       (2) as well.  THE TWO ARE ONE LEVER: the k spill only survives while
       the loop body is big enough that loop.c declines to hoist.

   (9) TWO SECOND PSEUDOS, which were the last two words.  A value computed
       in one block and used inside the following loop is TWO pseudos in the
       target and one here, and a plain `t2 = t;` copy is deleted by cse.
       * `tb`: `if (...) { tb1 = tb; do { } while (tb1[...] <= ...); }`.  The
         copy survives because the `do` body's label has two predecessors, so
         cse's table is reset there and `tb1` is never replaced by `tb`.
         35 -> 20 rows, and 242/242 words.
       * `x`: `(s16)x` at the three calls after the spawn.  `x` holds
         `r[0]`, a `lbu`, so num_sign_bit_copies is 24 and combine's
         simplify_shift_const folds `(ashiftrt (ashift x 16) 16)` to `x`
         itself -- turning the sll/sra pair into a PLAIN MOVE, which survives
         because NOTHING AFTER COMBINE DELETES COPIES.  `(u8)x` leaves an
         `andi` (1 row worse), `x3 = x;` is deleted outright (no effect).
         20 -> 19.
       The honest reading of the second is probably that func_800A0B94,
       func_800A152C and func_80042640 take `s16`/`short` in that position
       and the conversion is implicit at the call; none of the three is
       landed in this unit, so the cast is written out rather than changing a
       prototype other functions share.

   (10) `j = tb[y]; *(u32 *)(o + 0x18) = j;` -- SPLIT THE STORE THROUGH `j`,
       THE ROLL VARIABLE, not through a fresh local.  19 -> 13 rows.  Found by
       decomp-permuter (output-105-1 of a 25-minute 4-job run, the only real
       change in that find; everything else it had was reparenthesisation).
       `j` is dead at that point -- it is `func_800A6D30() & 0x1E` from the
       top of the same iteration and is reassigned every iteration -- so this
       is safe, and the value round-trips bit-exactly through s32.  IT HAS TO
       BE `j`: a fresh `u32 u` or `s32 n2` measures 19, and reusing `a` 25,
       `n` 24, `i` 19, `x` 29/243, `c` 47, `y` 46.  What it buys is `j`'s
       extra references and longer live range, which is what finally gets the
       guard's `tb` off $a0.

   (11) `rm = D_800E2970; t = rm + k * 0x14;` -- THE SYMBOL IS ITS OWN LOCAL,
       exactly like (4) and (7).  13 -> 5 rows.  It closes cluster (A) whole:
       with the address in its own variable the chain comes out k in $v1, the
       *20 in $v0 and the symbol in a third register, which is what the target
       has.  Found by decomp-permuter (run 2, output-30-1) and SPLIT OUT of
       that find: the find also carried `new_var = (s16 *)(p + 0xC)` hoisted
       above the `if (k >= 0)` and an `if (1)` wrapper, and each of those
       measures 13 on its own -- i.e. worth nothing.  The whole find and this
       one statement both measure 5.

   (12) `j = func_800A6D30(); ... + (j & 0x1E)` -- MASK AT THE USE, not at the
       assignment.  5 -> 3 rows.  `j = func_800A6D30() & 0x1E;` puts the
       masked value in $a0 where the target reuses $v0 (`andi $v0, $v0, 0x1E`
       straight on the return register).  `j = func_800A6D30(); j = j & 0x1E;`
       as two statements measures 5, and dropping `j` entirely --
       `r = *(u8 **)(g + 0x18) + (func_800A6D30() & 0x1E);` -- measures 11,
       because `j` is also the store temp from (10).

   ------------------------------------------------------------------ residue

   THREE ROWS, 242/242 words, and all three are REGISTER NAMES:

     0x800CEB78  addu $s0, $v0, $a0   vs  addu $s0, $a0, $v0
     0x800CEC58  sltu $v0, $a0, $v0   vs  sltu $a0, $a0, $v0
     0x800CEC5C  bnez $v0, ...        vs  bnez $a0, ...

   The second and third are one decision: the guard's `sltu` destination.
   Both source registers already match ($a0 = the value stored at o+0x18,
   $v0 = the limit); gcc reuses a dying source for the result and picks the
   lower-numbered one, $v0, while the target reuses $a0.  The first is the
   operand order of `c = t[2] + c;`, which does not follow the source: eight
   spellings (`c = c + t[2]`, `c += t[2]`, `a` and `c` swapped, `(s32)t[2]`,
   `d2 = t[2]` into its own local, `d0`/`d2` for both) all measure 3.

   ALL FOUR SWEEPS ARE CLEAN ON THE 3-ROW BODY, re-run after every change
   above with a private fast harness on the onefunc standalone (~0.15 s a
   candidate -- this unit is 5745 lines, so the repo tools compile the whole
   unit at ~25 s a candidate and a full refsweep does not finish):
     cancelled pairs, every local x every statement      1403 cands, none < 3
     do-while(0) + duplicated arm, every local x stmt    1334 cands, none < 3
     declared widths, every local and the parameter        46 cands, none < 3
     adjacent-statement swaps, every same-indent pair      37 cands, none < 3
     declaration ORDER, every local to every position     506 cands (run on
       the 19-row body; pseudo number is the LAST tie-break in both
       allocno_compare and qty_compare_1, and it changed nothing anywhere)

   MEASURED AND REJECTED (rows / words, on the body named):
     on the 112-row park body:
       plain `while (*(u32 *)(o + 0x18) >= tb1[o[0x11]])`   235 rows / 230 w
       `tb1` computed before the store                      235 / 230
       `tb++` with the guard on `u`                         224 / 232
       `s32 kk` intermediate for the byte test              229 / 237
       `(u8 *)&D_80083460` inlined at the use               220 / 240
     on the 81-row body (w[4] + addressable k):
       `s16 k; k = D_80082EA6; if (k >= 0)`                  82 / 242
       `u16 k; if ((s16)k >= 0)` (the park's)                82 / 242
       `kk = D_80082EA6; k = kk; if (kk >= 0)`               83 / 239
       `k = D_80082EA6; kk = k; if (kk >= 0)`                85 / 244
       no `(void)&k`, `s16 k`, `if (k >= 0)`                 90 / 241
     on the 52-row body:
       `a = c1; c = c2; a = a + t[0]; c = c + t[2];`         63 / 233
       `a++; a--;` instead of `c++; c--;`                    62 / 233
       `x2`/`y2` separate locals for the two call results    59 / 233
     on the 34-row body:
       deleting `g++; g--;` WITHOUT moving `g` up            44 / 234
       `r = *(u8 **)(g + 0x18); r = r + j;`                  44 / 234
       `g = *(u8 **)((u8 *)&D_80083460 + 0x18); r = g + j;`  44 / 234
       `y = r[1];` before `x = r[0];`                        44 / 234
       `x3 = x;` before the func_800A0B94 call               34 / 240 (the
         copy is deleted by cse -- byte-identical to no copy)
       `if (x) { x3 = x; } else { x3 = x; }`                 34 / 240
       `x3 = x; x3++; x3--;`                                 34 / 240
       `s = func_800A1618(r[0], 1); x = r[0];`               40 / 240
       `s = func_800A1618(r[0], 1);` after `y = r[1];`       37 / 240
       `s++; s--;` after func_800A1618 (x DOES get $s5, but
         `s` overshoots $s4 and takes $s3 off `i`)           37 / 240
     on the 3-row body:
       `c = c + t[2]`, `c += t[2]`, `(s32)t[2] + c`, the two adds
         swapped, `d2 = t[2]` and `d0`/`d2` into their own locals   all 3
       `if (*(u32 *)(o + 0x18) >= tb[o[0x11]])`, `if (j >= ...)`,
         `if (tb[o[0x11]] <= j)` for the guard                      all 3
       `lim = tb[o[0x11]];` into its own local                          3
     on the 5-row body:
       `j = func_800A6D30(); j = j & 0x1E;` as two statements            5
       `r = j + *(u8 **)(g + 0x18);`                                     5
       `r = *(u8 **)(g + 0x18) + (func_800A6D30() & 0x1E);` (no `j`)     11
       `a`/`n`/`i`/`x`/`c`/`y`/a fresh local as the store temp
         instead of `j`                            18/16/11/21/39/38/11
       no store temp at all                                             11
       deleting `c++; c--;`                                             10
       `x` instead of `(s16)x`                                     20 / 241
       the permuter's `new_var = (s16 *)(p + 0xC)` hoist alone            13
       the permuter's `if (1)` wrapper alone                             13
     on the 13-row body:
       deleting `c++; c--;`                                  18 / 242
       `x` instead of `(s16)x` at the three calls            28 / 241
       one `tb` instead of `tb` + `tb1`                      27 / 241
       `g = (u8 *)&D_80083460;` after the func_800A6D30 call 59 / 236
       `u = tb[y];` / `n2 = tb[y];` with a FRESH local       19 / 242
       `a`/`n`/`i`/`x`/`c`/`y` as the store temp instead of
         `j`                                      25/24/19/29/47/46
     on the 20/19-row body:
       `x3 = (u8)x` / `x3 = (u16)x`                          20 / 242 (`andi`)
       `tb = &D_800835E4_s;` with a scalar extern alias      20 / 242
       `tb1 = tb + 1;` without the `if` guard                32 / 243
       `tb1 = tb;` outside the guard (plain `while`)         22 / 241
       `o[0x12] = 0;` before `tb = D_800835E4;`              20 / 242
       `u = tb[y]; tb++; *(u32 *)(o + 0x18) = u;`            20 / 242
       `tb = D_800835E4;` before `o[0x43] = func_800A6D30()` 31 / 240
       `o[0x12] = 0;` first in the block                     29 / 241
       `o[0x12] = 0;` last in the block                      31 / 243
       `tb = D_800835E4 + 1; ... tb[y - 1]`                  22 / 242
       `t = D_800E2970;` then `t = t + k * 0x14;`            22 / 241
       `t = &D_800E2970[k * 0x14]`, `k * 0x14 + D_800E2970`,
         and `t = ...` before `i = 0xF;`                     19 / 242 (same)
       deleting `c++; c--;`                                  24 / 242

   THE ONE DEVICE LEFT: `c++; c--;` after `c = t[2] + c;`, worth 7 rows
   (3 -> 10) at the same word count.  It is four more references to `c` and compiles to
   nothing; without it the two `lbu`s and `addiu $a3, $sp, 0x10` schedule in
   the wrong order (0x800CEB68..0x800CEB78).  I did not find the honest
   statement that supplies those references.

   `u16 w` WITH `&w` IS THE SPELLING, and it is the unit's established one:
   the landed func_8009A350 (csrc/ovl_dungeon.c:9896) takes `u16 *out` and
   writes exactly one halfword through it.  An intermediate body in this
   session used `u16 w[4]` to force eight bytes of frame, and on the 81-row
   body the scalar measured 90 rows against the array's 81 -- but that was a
   stale negative the moment (8) landed: on THIS body the scalar and the array
   produce a byte-identical object (same aligndiff md5, 242 words / 19 rows),
   because the eight bytes are k's spill slot, not w.  The scalar is what is
   written below.

   DECLARATIONS THIS NEEDS -- EXACTLY TWO (unchanged):
     extern u8 D_800E1C3A[];
     extern u32 D_800835E4[];
   NO PROTOTYPE CHANGE.

   WHAT IT DOES: a three-state machine on the halfword at `p + 0x6`.
   State 0 spends the D_800E3648 slot for room `*(s16 *)(p + 0xC)`: for each
   of its `*(s8 *)(e + 2)` charges it rolls a random pair out of
   D_80083460's +0x18 table, asks func_800A1618 for a template, then tries up
   to sixteen random tiles inside the D_800E2970 room record for one that
   func_8009A350 reports with no 0xB700 flag, and spawns there through the
   function pointer func_800A0B94 RETURNS -- the `jalr $v0` at 0x800CEBF0,
   called with (0, (u8)a, (u8)c, -0x400).  It then drains the spawn's +0x18
   budget through func_800A1D4C and stamps +0x9A/+0x8C/+0x1C/+0x92.  State 1
   counts down +0x4 and plays the message; the default state counts down,
   clears the slot and sets the two 0x8000 request bits. */

void func_800CEA44(u8 *p, UnkVec *v, u32 q)
{
    u8 *b;
    u8 *b2;
    u8 *b3;
    u8 *e2;
    u8 *e3;
    u8 *g2;
    u8 *e;
    u8 *t;
    u8 *rm;
    u8 *r;
    u8 *o;
    u8 *g;
    u32 *tb;
    u32 *tb1;
    s32 n;
    s32 j;
    s16 i;
    u16 w;
    s16 k;
    s32 a;
    s32 c;
    s32 x;
    s32 y;
    u8 *s;

    if (*(s16 *)(p + 0x6) == 0)
    {
        k = D_80082EA6;
        if (k >= 0)
        {
            b = D_800E3648;
            e = b + *(s16 *)(p + 0xC) * 4;
            n = *(s8 *)(e + 2);
            while (n >= 0)
            {
                if (func_8003FA44(3) == 0)
                {
                    b2 = D_800E3648;
                    e2 = b2 + *(s16 *)(p + 0xC) * 4;
                    e2[2] = e2[2] - n;
                    return;
                }
                g = (u8 *)&D_80083460;
                j = func_800A6D30();
                r = *(u8 **)(g + 0x18) + (j & 0x1E);
                x = r[0];
                y = r[1];
                s = func_800A1618(x, 1);
                if (s != NULL)
                {
                    i = 0xF;
                    rm = D_800E2970;
                    t = rm + k * 0x14;
                    while (1)
                    {
                        a = func_800A6DA4(0, (u16)(*(u16 *)(t + 4) - 1));
                        c = func_800A6DA4(0, (u16)(*(u16 *)(t + 6) - 1));
                        a = a + t[0];
                        c = t[2] + c;
                        c++;
                        c--;
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
                        o = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94((s16)x, s, 1))(
                            0, (u8)a, (u8)c, -0x400);
                        if (o != NULL)
                        {
                            func_800A152C((s16)x, 1);
                            func_80042640((Unk42710 *)o, (s16)x);
                            o[0x43] = func_800A6D30();
                            tb = D_800835E4;
                            o[0x12] = 0;
                            j = tb[y];
                            *(u32 *)(o + 0x18) = j;
                            tb++;
                            if (tb[o[0x11]] <= *(u32 *)(o + 0x18))
                            {
                                tb1 = tb;
                                do
                                {
                                    func_800A1D4C(o, 0);
                                } while (tb1[o[0x11]] <= *(u32 *)(o + 0x18));
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
            b3 = D_800E3648;
            e3 = b3 + *(s16 *)(p + 0xC) * 4;
            g2 = (u8 *)&D_80083460;
            *(u32 *)e3 = 0;
            *(u16 *)(g2 + 0xA) -= 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}
