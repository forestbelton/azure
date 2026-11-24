#include "common.h"

extern s16 D_80083228;
extern u8 D_800E2348[];
extern u8 D_800E23A8[];
s32 func_800AC82C();
s32 func_800AD9B4(u8 *, u8 *);
void func_80047784(u8 *, s32, s32);

extern u16 D_80083462;
s32 func_800A2BDC(u8 *);
void func_800A9A0C(u8 *);
s16 func_800ADDA0(void *, void *, u8 *, s16, s16, s32);
void func_b25_801726EC(u8 *p, void *b, void *c, u8 *d);

s32 func_800AC010();

s32 func_800AB378();
s32 func_800AD9B4();

s32 func_800AC7EC();

s32 func_800ACE34();

s32 func_800AC5C8();

s32 func_800AC480();

s32 func_800ABDBC();
void func_b25_80171F1C();

s32 func_800AB778();

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
s32 func_800AB464();

void func_800A4ACC(u8 *p);
s32 func_800AB1C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_800AD594(u8 *p, s32 a);

s32 func_800AB538();

s32 func_8009A180(u8 *a, u8 *b);
extern u8 D_800E23B0[];
s32 func_800A2C34(u8 *);
void func_800A9A04(u8 *);
void func_800AA258(u8 *, void *, u8 *, u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA888(u8 *, void *, u8 *, u8 *);
void func_b25_8017531C(u8 *p, u8 *b, u8 *c, u8 *d);

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E2390[];
void func_800A2B04(UnkVec *, s32, s32);
void func_800AAA54(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD4D0(u8 *);

u16 func_800A6D30(void);
extern u8 D_80082E80[];
s32 func_80042900(s8 *, s8);
void func_80042B68(s8 *, s8);
s16 func_8009FD40(u8 *, u8 *);
void func_800AA79C(u8 *, void *, u8 *, u8 *);

s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out);
u8 *func_800A02AC(u8 *p, u16 x, u16 y);
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f);
u8 *func_800A04F0_r(u8 *p, s32 x, s32 y, s16 dir) __asm__("func_800A04F0");
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y);
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2);
s16 func_800A0134(u8 *a, u8 *b);
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z);
s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d);
void func_8009A3D0(s32 x, s32 y, s32 f);
void func_8009A21C(s32 a, s32 b, s32 c);
s32 func_800BCB04(s32, s32, s16);
UnkNode *func_8003FD64(s32, void *);
void func_800D707C(u8 *p, UnkVec *v);
s32 func_800D68F4(u8 *p, UnkVec *v);
extern s16 D_800E2468[][2];
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y);
void func_b25_80172E0C(u8 *, UnkVec *, u8 *, u8 *);
s32 func_b25_80172E80(u8 *p, void *b, void *c, u8 *d);
extern s16 D_8006CD00[];
extern u16 D_80082EA4;
extern UnkDngRoom2970 D_800E2970_s[] __asm__("D_800E2970");

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170A44);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170BB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80170D34);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171010);

/* The banked room script's 0x211 burst spawner: the resident overlay's
   func_800D5DCC (csrc/ovl_dungeon_800D50FC.c) relinked, with the same
   `n->unk_08' RE-DEREFERENCED AT EVERY STATEMENT and the same `b' kept
   NARROW so the table index shifts logically -- both of those facts are the
   twin's and its comment carries the measurements. func_b15_80170F2C
   (csrc/ovl_dng_b15.c) is the nearer landed relink by word count but it is
   the WRONG twin: it takes its position offsets from the caller and has no
   D_800E2468 row at all.

   Seven deltas from func_800D5DCC, every one read off the listing: the z
   seed is `- 0x58' not `- 0x14'; the row entries are `* 18', which is the
   `sll 3'/`addu'/`sll 1' triple; there is NO `*(s16 *)((u8 *)n->unk_0C +
   0x6) = 6' write at all; the three velocities come from rand
   rather than rand() and are `<< 4' on ALL THREE axes where the twin is
   6/6/5; the constant drift is `t[k] * 0x1C0000' (`sll 3'/`subu'/`sll 18')
   not 0x160000; the +0x32/+0x34 life is 0xC; and BOTH hung functions are
   RESIDENT -- the step is func_800D707C and func_8004491C is handed
   func_800D68F4 -- where every other relink in this block substitutes
   bank-local ones.

   106 words, 0 differing, on the first compile. Measured afterwards and all
   equally 0, so nothing here is claimed to be load-bearing: the row entries
   as `(t[k] * 9) << 1', the velocities as `* 16' instead of `<< 4', the
   drift as `(t[k] * 7) << 18', and the row index hoisted into a local of its
   own ahead of the D_800E2468 subscript. No jump table and no rodata of its
   own -- the listing opens at `glabel'. */
void func_b25_801711DC(UnkNode *a, s16 b, u8 *c)
{
    UnkNode *n;
    s16 *t;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D707C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) - 0x58;
        t = D_800E2468[(b >> 9) & 7];
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)n->unk_08 + 0x2) + (t[0] * 18);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)n->unk_08 + 0x6) + (t[1] * 18);
        *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0x7FFF) - 0x4000) << 4;
        *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0x7FFF) - 0x4000) << 4;
        *(s32 *)((u8 *)n->unk_08 + 0x14) = ((rand() & 0x7FFF) - 0x4000) << 4;
        *(s32 *)((u8 *)n->unk_08 + 0xC) += t[0] * 0x1C0000;
        *(s32 *)((u8 *)n->unk_08 + 0x10) += t[1] * 0x1C0000;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = 0xC;
        *(s16 *)(p + 0x34) = 0xC;
        func_8004491C(n, (u32)func_800D68F4);
        *(u8 **)((u8 *)n + 0x20) = c;
        *(u8 **)(p + 0x8) = c;
    }
}

/* One frame of the fading burst func_b25_801711DC spawns, and the resident
   func_800D5CA0's body (csrc/ovl_dungeon_800D50FC.c) with a THIRD PARAMETER:
   integrate the mover, halve each of its three velocities -- the three
   `srl 31'/`addu'/`sra 1' triples are a signed `/= 2', and `>>= 1' would
   drop the correction -- then fade the colour and count down to the kill.
   A leaf with no frame at all; the `addu $t0, $a0, $zero' is `p' moved out
   of $a0 so the mover loads can use it.

   Four deltas from the twin, all read off the listing. The fade is ONE value
   written to three bytes of the THIRD parameter at +0xE, +0xD and +0xC in
   that order, not three per-channel multiplies: `x' has to be a local,
   because gcc 2.7.2 has no alias analysis and each `c[...]' store kills the
   cached p + 0xC and p + 0xE, so writing the expression out three times is
   66 words and 12 aligned differences -- three divides. The life pair is
   p + 0xC / p + 0xE where the twin uses p + 0x32 / p + 0x34, so the same
   halfword the fade divides is also the countdown. There is no
   `*(s32 *)(p + 0x8) = *(s32 *)(p + 0x4)' colour-forward copy. And the kill
   test is `if (--*(s16 *)(p + 0xC) <= 0)' -- `lhu', `addiu -1', `sh', then
   `sll 16'/`bgtz' -- on that same halfword.

   54 words, 0 differing, on the first compile. `* 128' is written rather
   than `<< 7' only because the load is `lh' and the twin's own note makes
   that the safer form; measured here both are 0, as is
   `c[0xC] = c[0xD] = c[0xE] = x;' in place of the three statements. No jump
   table and no rodata of its own -- the listing opens at `glabel'. */
void func_b25_80171384(u8 *p, UnkVec *v, u8 *c)
{
    s32 x;

    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    x = *(s16 *)(p + 0xC) * 128 / *(s16 *)(p + 0xE);
    c[0xE] = x;
    c[0xD] = x;
    c[0xC] = x;
    if (--*(s16 *)(p + 0xC) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_8017145C);

/* The bank's "random value below n": six masked draws from rand,
   picked by the size of n, each retried until it falls under n. The mask and
   shift pairs are read straight off the listing -- 0xC >> 2, 0xF0 >> 4,
   0xFC >> 2, 0xFF0 >> 4, then 0xFFF and 0xFFFF unshifted -- and the six
   bounds are the `slti' constants 5, 0x11, 0x41, 0x101, 0x1001 and the
   fall-through. xsibs found no twin at ratio 0.70 and nothing in the project
   reads rand this way, so this is plain decompilation.

   Two shape facts read off the listing rather than guessed. The n == 0 test
   shares its `addu $v0, $v1, $zero' with the LAST arm's fall-through at
   .Lb25_80171768, so `r = 0' is initialised before the test and the last
   `do' loop has NO `return' of its own -- the five earlier arms do, which is
   what puts the return-value copy in each of their `beqz' delay slots and a
   bare `nop' in the following `j'. The retry test is a signed `slt', so n and
   r are both s32.

   73 words, 0 differing, on the first compile; no alternative spelling was
   measured, so nothing here is claimed to be load-bearing. No jump table and
   no rodata of its own -- the listing opens at `glabel'. */
s32 func_b25_8017165C(s32 n)
{
    s32 r;

    r = 0;
    if (n != 0)
    {
        if (n < 5)
        {
            do
            {
                r = (rand() & 0xC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x11)
        {
            do
            {
                r = (rand() & 0xF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x41)
        {
            do
            {
                r = (rand() & 0xFC) >> 2;
            } while (r >= n);
            return r;
        }
        if (n < 0x101)
        {
            do
            {
                r = (rand() & 0xFF0) >> 4;
            } while (r >= n);
            return r;
        }
        if (n < 0x1001)
        {
            do
            {
                r = rand() & 0xFFF;
            } while (r >= n);
            return r;
        }
        do
        {
            r = rand() & 0xFFFF;
        } while (r >= n);
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171780);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171964);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80171F1C);

/* The banked room script's step-the-walk handler: advance the actor one cell
   of its queued path, swap in the resident D_800E2348 facing table, re-aim
   the sprite, and either stop at state 0xF or hand on to the bank-local
   func_b25_80172E0C. The twin is the 145-word func_b07_80171570
   (csrc/ovl_dng_b07.c) and not the resident func_800D904C: like b07 this copy
   calls func_b25_80172E0C on the INVERTED gate, `(s16)t == 3 &&
   !(D_80083462 & 0x80) && !(c[0x14] & 0x8000)', with 0xF as the else arm.
   Read b07's comment for the rest of the shape and for its own measurements.

   Three deltas from that twin, all read off the listing, and the third is
   the only one that took work.

   1. The table is the resident D_800E2348 rather than a bank-local array,
      and the swap arm gains a trailing `*(s16 *)(p + 0x9E) = 0;' after
      func_80047784 -- the `sh $zero, 0x9E($s4)' the twin does not have.
      Without it: 142 words, 4 aligned differences.

   2. The func_8009A66C result is cast AT THE ASSIGNMENT, `t =
      (s16)func_8009A66C(...)', and the gate then tests a plain `t == 3'.
      The twin's cast-at-the-use form is 145 words / 8 aligned differences
      here, which is where two of the three words go. The `(s16)' on `r' at
      that call is still real: dropping it is 142 words / 4 differences.

   3. THE DIVISOR IN THE TAIL IS `t' REUSED, NOT A SEPARATE LOCAL. b07's own
      `s32 n; n = d[0x71]; ... 8 / n' is 144 words and 18 aligned differences
      here: `n' takes $v1, which leaves func_8009A66C's return sitting in $v0
      until the gate, so the sign-extension is scheduled late, the +0x8A
      halfword lands in $v1 instead of $v0 and the second `(d + 0x1C &
      0x2000)' load can no longer fill the +0x8A load-delay slot -- one stray
      `nop', and the whole 18-difference $v0/$v1 rotation around it. Sharing
      `t' puts the divisor in $s0 with `x' and `t' and the schedule falls out.

   AND b07'S PHANTOM-ZERO DEVICE IS NOT NEEDED HERE. It is load-bearing on
   b07 (28 differing without it there) but once the divisor shares `t' this
   body is 143 words / 0 differing with no device at all, which is how it is
   written. Measured and all equally 0 on this body, so none is claimed:
   `x + ((x + x) - x - x)' on func_800A0818's first argument as b07 has it,
   declaring `x' and `y' before `r', and `s16 x, y'. With the separate `n'
   still in place the device mattered -- 41 aligned differences without it,
   23 with it moved to func_8009A3D0's first argument, 42 applied to `y' as
   well, 22 applied to `r' at func_8009A66C, 20 as its own statement -- so
   the device and the merged divisor are two routes to the same allocation
   and the merge is the one the source almost certainly had.

   143 words, 0 differing, 0 aligned differences. No jump table and no rodata
   of its own -- the listing opens at `glabel'. The second parameter is only
   passed on to func_b25_80172E0C. */
void func_b25_801724B0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 r;
    s32 t;
    s32 x;
    s32 y;

    if (*(s8 *)(d + 0x71) > 0 && d[0x71] > *(s16 *)(d + 0x8A))
    {
        if (*(u8 **)(c + 0x2C) != D_800E2348)
        {
            *(u8 **)(c + 0x2C) = D_800E2348;
            func_80047784(c, D_800E2348[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s16 *)(p + 0x9E) = 0;
        }
        x = c[0x24];
        y = c[0x25];
        func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        r = func_800A0818(x, y, *(d + *(s16 *)(d + 0x8A) + 0x74), *(d + *(s16 *)(d + 0x8A) + 0x7C), (u16 *)(p + 0x98));
        t = (s16)func_8009A66C((s16)r, c, d, 0x20);
        c[0x24] = *(d + *(s16 *)(d + 0x8A) + 0x74);
        c[0x25] = *(d + *(s16 *)(d + 0x8A) + 0x7C);
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s16 *)(d + 0x2A) = r;
        if (t == 3 && (D_80083462 & 0x80) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            func_b25_80172E0C(p, b, c, d);
        }
        else
        {
            p[0x9A] = 0xF;
        }
        *(s32 *)(p + 0x8C) = 0;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        if ((D_80083462 & 0x80) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            t = d[0x71];
            if (t > 0)
            {
                *(s16 *)(p + 0x96) = 8 / t;
            }
        }
    }
}

/* The banked room script's walk step: the 456-WORD RELINK of the resident
   overlay's func_800D92C0 (csrc/ovl_dungeon_800D50FC.c), and a DIFFERENT
   SOURCE from the 466-word relink that 46 other stubs in this block share
   (func_b01_80171C34, func_b14_80171410, ...). Read func_b01_80171C34's
   comment first: everything about the three extra pre-loop arms, the
   func_800BCB04 register-assignment fact, the `e'-sharing of the loop's
   heading value, and the D_800E2970 0x14-byte indexing is its, holds here
   verbatim, and is not repeated.

   FOUR SOURCE DELTAS FROM THE 466-WORD BODY, AND THEY ACCOUNT FOR ALL TEN
   WORDS. Every number below was measured on THIS body, one change at a time.

   1. `s32 i', not `s16 i'. That is the whole 456-vs-466 story: an s32 index
      lets loop.c strength-reduce the D_8006CD00 lookup to a pointer walk, so
      the two arms become a bare `lh $v0, 0x0($s1)' with an `addiu $s1, $s1,
      0x2' in the tail, and the `i >= 3', `i == 0' and `i < 8' tests lose
      their `sll 16'/`sra 16' pairs. `s16 i' is 468 words and 144 aligned
      differences.

   2. THE SWEEP TABLE IS NAMED INLINE AS `D_8006CD00[i]', WITH NO `s16 *tbl'
      LOCAL. This is the one spelling fact that is not visible in the
      listing's arithmetic, and it is why the 466-word body cannot simply be
      retyped. b01's `tbl = D_8006CD00;' local is 457 words and 21 aligned
      differences HERE: naming the table through a local takes four RTL insns
      out of the loop, the loop's insn_count drops below gcc 2.7.2's
      invariant-motion threshold (loop.c:532 computes
      `(loop_has_call ? 1 : 2) * (1 + n_non_fixed_regs)'; loop.c:1631 moves a
      movable when `threshold * savings * lifetime >= insn_count'), and the
      halfword extension of `f' in `i >= 3 && f != 0' is then hoisted into
      the loop preheader as an extra `move' -- costing the one word and
      rotating $s1/$s5/$s6. Written inline the index arithmetic is
      strength-reduced away AFTER that count is taken, so the final code is
      two words SHORTER while the count stays above the threshold. The
      preheader's single `lui'/`addiu' pair for D_8006CD00 is the giv's
      initial value, not a source assignment.

   3. D_8006CCD8 IS HOISTED, THROUGH A `u8 *tb' SET BETWEEN `i = 0;' AND THE
      LOOP, AND INDEXED `*(u8 *)(kk + (s32)tb)' WITH kk FIRST. This is the
      exact inverse of b01, whose `t1 = (s32)D_8006CCD8; t1 += kk;' device
      exists to give the `%lo' destination reg_n_sets != 1 and PIN the
      address inside the loop. Here the target carries it in $s7 across the
      whole loop, which is also why this copy saves nine registers' worth of
      frame plus $s7 (sp -0x40, $ra at 0x38) where b01 saves eight (sp -0x38).
      Measured: inline `*(u8 *)(kk + (s32)D_8006CCD8)' with no local is 454
      words / 20 differences; `tb[kk]' instead of `kk + (s32)tb' is 456 words
      / 1 difference, the `addu $v1, $a0, $s7' coming out `addu $v1, $s7,
      $a0'; putting `tb = ...' BEFORE `i = 0' is 456 words / 8 differences,
      because `i = 0' must be the first insn of the pre-loop join block for
      reorg to copy it into the seven branch delay slots that reach it. The
      SECOND lookup stays inline as `*(u8 *)(kk + (s32)D_8006CCE8)' -- it
      must, for the reason b01 gives.

   4. `i = (s16)func_800BCB04(...)' INTO THE s32 `i', not b01's s16-returning
      func_800BCB04h alias. The target's `sll $v0, $v0, 16' / `sra $s3, $v0,
      16' sign-extends straight into `i'; dropping the `(s16)' is 455 words /
      5 differences.

   TWO OF b01'S DEVICES DIVERGE. Its `do { return; } while (0)' in the
   `i == 0' arm must be REMOVED -- 459 words / 14 differences with it, because
   with an s32 index reorg does not want `i++' in those delay slots and fills
   them with the `lui %hi(D_80082EA4)' and a `nop' instead. Its `cc2 =
   0x80000000' do-while(0) reference bump is KEPT verbatim: spelled plainly as
   `*(s32 *)(d + 0x14) |= 0x80000000;' this body is 456 words / 4 differences,
   the same swap of $v0 and $v1 b01 measures.

   456 words, 0 differing, and 0 aligned differences. No jump table and no
   rodata of its own -- the listing opens at `glabel'. The second parameter is
   never read and still has to be declared and passed on, as in b01. */
void func_b25_801726EC(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    s32 cc2;
    u8 *g2;
    s32 e;
    u8 *t;
    u8 *t80;
    u16 *w;
    s16 f;
    s32 i;
    s16 m;
    s32 s;
    s32 v;
    s32 j;
    s32 k;
    s32 kk;
    s32 x;
    s32 y;
    u8 *tb;

    g = (u8 *)&D_80083460;
    f = 0;
    if ((*(u16 *)(g + 0x2) & 0x4000) != 0 || *(s8 *)(d + 0x71) >= 0)
    {
        if (d[0x12] >= 2 || (s16)func_b25_80172E80(p, b, c, d) == 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u8 **)(g + 0xC) == d)
        {
            *(u16 *)(d + 0x46) = 0xC008;
        }
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        return;
    }
    func_800A19E4((s32)c, d, 3, 6, p + 0x9C);
    v = *(s32 *)(d + 0x1C);
    if ((v & 0x410) != 0)
    {
        if ((v & 0x400) != 0)
        {
            e = (s32)func_800A02AC(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25));
            if (e != 0)
            {
                t = *(u8 **)(e - 0x14);
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t[0x24], t[0x25], (u16 *)(p + 0x98));
                d[0x71] &= 0x7F;
                return;
            }
            cc2 = 0x80000000;
            if (*(s32 *)(d + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(d + 0x14) |= cc2;
                } while (0);
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
            }
        }
        else
        {
            if (func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A)) != NULL)
            {
                d[0x71] &= 0x7F;
                return;
            }
        }
    }
    else if ((v & 0x2000) != 0)
    {
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((v & 0x20000) != 0)
            {
                k = ((d[0x45] + ((s16)(*(u16 *)(D_800814A8 + 0x2A)) >> 9)) & 7) * 2;
                x = D_80082E80[0x24] + *(u16 *)(k + (s32)D_8006CCD8);
                y = D_80082E80[0x25] + *(u16 *)(k + (s32)D_8006CCE8);
                if (*(u8 *)(c + 0x24) == (u16)x && *(u8 *)(c + 0x25) == (u16)y)
                {
                    d[0x71] &= 0x7F;
                    return;
                }
                w = (u16 *)(p + 0x98);
                m = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), x, y, w);
                *(s16 *)(d + 0x2A) = m;
                if ((s16)func_8009A8C0(m, c, d, 0x20) <= 0)
                {
                    *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25], w);
                }
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), D_80082E80[0x24], D_80082E80[0x25]) != 0)
                {
                    f = 1;
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    else
    {
        j = *(s8 *)(c + 0x26);
        if (j >= 0 && (D_800E2970_s[j].unk_0C & 2) != 0)
        {
            func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
        }
        else if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            e = (s32)func_800A04F0_r(d, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25), *(s16 *)(d + 0x2A));
            if (e != 0 && (*(s32 *)(e + 0x1C) & 0x2000) != 0 && func_800A0134((u8 *)e, d) < 0x81 &&
                func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                              (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            if ((*(s32 *)(d + 0x1C) & 0x20000) != 0)
            {
                t80 = D_80082E80;
                *(s16 *)(d + 0x2A) = func_800A0818(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25],
                                                   (u16 *)(p + 0x98));
                if (func_8009FD7C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), t80[0x24], t80[0x25]) != 0)
                {
                    if (func_800A0134((u8 *)D_800814A8, d) < 0x81 &&
                        func_8009A540(((s16)(*(u16 *)(d + 0x2A)) >> 9) & 0xFFFF, *(u8 *)(c + 0x24), *(u8 *)(c + 0x25),
                                      (s16)(*(u16 *)(d + 0x88) - 0x20)) != 0)
                    {
                        d[0x71] &= 0x7F;
                        return;
                    }
                }
            }
            else
            {
                func_800A0E6C((u8 *)c, *(s8 *)(p + 0x9C), d, p + 0x98);
            }
        }
    }
    i = 0;
    tb = (u8 *)D_8006CCD8;
    do
    {
        e = *(s16 *)(d + 0x2A);
        if ((*(u16 *)(p + 0x98) & 2) != 0)
        {
            s = e - D_8006CD00[i];
        }
        else
        {
            s = e + D_8006CD00[i];
        }
        if ((s16)func_8009A8C0((s16)s, (u8 *)c, d, 0x20) > 0)
        {
            if (i >= 3 && f != 0)
            {
                d[0x71] &= 0x7F;
                return;
            }
            *(s16 *)(d + 0x2A) = s;
            *(d + (d[0x71] & 0x7F) + 0x74) = *(u8 *)(c + 0x24);
            *(d + (d[0x71] & 0x7F) + 0x7C) = *(u8 *)(c + 0x25);
            d[0x71]++;
            func_8009A3D0(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            kk = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            *(u8 *)(c + 0x24) += *(u8 *)(kk + (s32)tb);
            *(u8 *)(c + 0x25) += *(u8 *)(kk + (s32)D_8006CCE8);
            func_8009A21C(*(u8 *)(c + 0x24), *(u8 *)(c + 0x25), (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            break;
        }
        if (i == 0 && D_80082EA4 != *(u16 *)(c + 0x24))
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
        return;
    }
    *(u16 *)(d + 0x46) &= 0x7FFF;
    p[0x9C] = *(u8 *)(c + 0x26);
    d[0x6D]--;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0x8) += 1;
    if (*(s8 *)(d + 0x6D) == 0)
    {
        d[0x71] &= 0x7F;
        return;
    }
    i = (s16)func_800BCB04((*(u8 *)(c + 0x24) << 6) | 0x20, (*(u8 *)(c + 0x25) << 6) | 0x20, (s16)(*(u16 *)(d + 0x88) - 0x20));
    if (i < 0x200)
    {
        *(s16 *)(d + 0x88) = i;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80172E0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80172E80);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801730A4);

/* THE PARAMETER COPY HAS TO LEAVE THE PROLOGUE'S UNSCHEDULABLE PREFIX. The
   target saves and copies its four parameters in the order $s1, $s2, $s3,
   $s0 -- parameter 0 LAST -- and the leading run of (set pseudo hard_reg)
   insns is pinned by sched.c and emitted in LUID order, so no spelling
   inside those four copies can reorder them. A plain `p = a;` in the body
   is the cure: combine folds parameter 0's prologue copy into the body copy,
   the surviving move is a user insn inside sched's region, and sched2's
   entry block (all priority 1, ranked on INSN_LUID) therefore puts it after
   the other three. Used directly, with no local, the function is 8
   differing; the same copy made of parameter 1 instead is also 8.

   Two other measured facts. `if ((s16)d != 0)` is the halfword test the
   target's `sll $v0, $s3, 16` / `bnez` is: a plain `if (d != 0)` is 80 words
   and 59 differing. The case arms are in the target's own order -- case 0,
   case 2, then case 1 falling into `default` -- and putting case 2 first is
   79 words and 47 differing. `s32 r` with `(s16)` casts at its three uses
   also measures 0; `s16 r` is kept because it needs none of them.
   81 words, 0 differing. */
s32 func_b25_801731DC(u8 *a, void *b, void *c, s32 d)
{
    u8 *p;
    s16 r;

    p = a;
    r = func_800ADDA0(b, c, p, 3, 6, (s32)(p + 0x9C));
    if (r < 0)
    {
        return 0;
    }
    if ((s16)d != 0)
    {
        func_b25_801726EC(p, b, c, p);
        return 0;
    }
    switch (r)
    {
    case 0:
        p[0x9A] = 0xE;
        func_800A9A0C(p);
        return 0;
    case 2:
        func_b25_801726EC(p, b, c, p);
        return 0;
    case 1:
        p[0x71] &= 0x7F;
        if ((s16)func_800A2BDC(p) != 0)
        {
            *(u16 *)(p + 0x46) &= 0x7FFF;
            return 0;
        }
    default:
        p[0x71] &= 0x7F;
        if ((D_80083462 & 8) == 0)
        {
            return 1;
        }
        *(u16 *)(p + 0x46) &= 0x7FFF;
        return 0;
    }
}

/* The banked room script's two-gate installer, the relinked twin of the
   resident overlay's func_800D9884 (csrc/ovl_dungeon_800D50FC.c): the same
   func_800AB1C0 gate, the same func_800AD594 / func_800A4ACC pair, the same
   `blez'-to-the-epilogue inner gate, and the same D_80083462 & 0x80 tail.
   Two deltas, both read off the listing: this copy does NOT clear p + 0x90
   after installing the handler, and its flag arm writes -0x20 to p + 0x92
   where the resident writes 0. The handler is the bank-local
   func_b25_80171F1C. 37 words, 0 differing, on the first compile -- no
   alternative spelling was measured, so nothing here is claimed to be
   load-bearing beyond the `(s16)' cast on func_800AD9B4, which the twin's
   own note measures at 38 differing when dropped. */
void func_b25_80173320(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_b25_80171F1C;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = -0x20;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801733B4);

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b25_80173794(u8 *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801737F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80173B08);

/* The banked room script's three-state glide, switching on p + 0x9B:
   state 0 seeds the velocity from the D_8006CCD8 / D_8006CCE8 facing pair,
   state 1 decays it, state 2 steers onto the tile centre and installs
   func_b25_80171F1C. The resident overlay's func_800D9C7C
   (csrc/ovl_dungeon_800D50FC.c) is the same machine -- the same three cases
   FALLING THROUGH into one another, the same `stop:' label inside case 1
   that case 0's `else' reaches by `goto', the same func_800AAA54 call -- and
   func_800D991C in the same unit supplies the divide form of case 2. Both
   of those comments carry the measured spelling facts this body relies on:
   the three-statement numerator `u = c[0x24] << 6; w = b2 - 0x20; u -= w;'
   (nine other spellings 1 to 4 differing on func_800D991C), the second
   numerator left two-statement (1 word and 145 differing when split), and
   `if (x > 0) x--;' in case 1 against `if (--x > 0)' in case 2, which
   func_800D9C7C measures as not interchangeable.

   Four deltas from func_800D9C7C, all read off the listing: the
   D_8006CCD8 index is NOT a local here -- state 0 recomputes
   `(*(u16 *)(d + 0x6A) >> 9) & 7' for each of the two tables and state 1
   never uses it, so there is no $s4 to hold it; the seeds are negated and
   scaled by 15 (`negu' then `sll 15') rather than shifted by 18; state 1
   decays by a quarter (`b->unk_0C -= b->unk_0C / 4', the
   `bgez'/`addiu 3'/`sra 2' triple) where func_800D9C7C subtracts
   `D_8006CCD8[k] << 14'; and func_800AAA54's fourth argument is the
   bank-local D_800E2390 rather than 0. 181 words, 0 differing, on the first
   compile. */
void func_b25_80174238(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p[0x9B] += 1;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] = 2;
                return;
            }
            *(s16 *)(p + 0x96) = (*(s32 *)(d + 0x1C) & 0x228) ? 8 : -1;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (*(s16 *)(p + 0x96) > 0)
        {
            *(s16 *)(p + 0x96) -= 1;
        }
        else if (*(u16 *)(c + 0x14) & 0x6000)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(p, b, c, D_800E2390);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            p[0x9B] += 1;
        }
        return;
    case 2:
        if (*(s16 *)(p + 0x96) != 0)
        {
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 15) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 15) / *(s16 *)(p + 0x96);
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_8017450C. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_8017088C[] = {
    0x10,
    0x00,
    0x00,
    0x00,
    0x0D,
    0x00,
    0x0D,
    0x00,
    0x00,
    0x00,
    0x10,
    0x00,
    0xF3,
    0xFF,
    0x0D,
    0x00,
    0xF0,
    0xFF,
    0x00,
    0x00,
    0xF3,
    0xFF,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0xF0,
    0xFF,
    0x0D,
    0x00,
    0xF3,
    0xFF,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_8017450C);

/* A one-call forwarder with a tail store: func_800AB538 takes this
   function's own incoming $a0..$a3 untouched and p + 0xA2 is cleared after
   it returns. The empty-paren `s32 func_800AB538();' declaration already in
   this unit is what allows the argument-free call -- func_800AB538's landed
   definition is K&R with four parameters
   (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in the project
   disagrees with it. 11 words, 0 differing, on the first compile. */
void func_b25_80174B80(u8 *p)
{
    func_800AB538();
    *(s16 *)(p + 0xA2) = 0;
}

/* A bare forwarder: one call, nothing in the delay slot, no argument set up
   at all. The empty-paren declaration is what lets it pass its own incoming
   $a0..$a3 through untouched -- func_800AB778's landed definition is K&R with
   four parameters (csrc/ovl_dungeon_800A4B88.c:5506) and every call site in
   the project disagrees with it. 8 words, 0 differing. */
void func_b25_80174BAC(void)
{
    func_800AB778();
}

/* 14 words, 0 differing. */
void func_b25_80174BCC(u8 *p)
{
    if (func_800ABDBC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_800E23A8 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b25_80171F1C. 241 words,
   0 differing, on the first compile.

   THE TWIN IS func_b03_80173358 (csrc/ovl_dng_b03.c), not the 213-word
   func_b01_80173AAC / func_b16_80173114 pair: like func_b03_80173358 this
   copy opens state 1 with a func_80042900 test rather than the d[0x25] one,
   carries the func_800AA79C arm behind the g + 0x2 0x2008 gate, closes state
   1 with the D_80082E80 / func_8009FD40 / func_800A6D30 window and the second
   func_80042900 test, and writes the dress/flag/bump tail out TWICE -- once
   in the first test's `== 0' arm and once after `again:' -- which gcc
   cross-jumps into the single block at the target's .Lb02_80174134. Read that
   comment for the mechanism: `&D_80083460' as separate pointer locals per
   arm, and the facing table named directly at every site.

   Four deltas from that twin, all read off the listing. The tail's flag
   update is `*(s32 *)(d + 0x1C) |= 0x40000' and it sits BEFORE the c + 0x14
   0x8000 gate, unconditionally, where func_b03_80173358 has `&= ~0x200'
   inside the gate -- that is func_b01_80173AAC's constant in
   func_b03_80173358's position. The 0x80000 arm has the same six-word block
   func_b01_80173AAC has between func_800AA888 and the bank-local tail call,
   p + 0x92 reduced by p + 0xA2 as halfwords then p + 0xA2 and p + 0x9E
   cleared, and the tail call is this unit's already-landed func_b25_8017531C.
   State 2 changes no flag word at all. And in the tail the two bumps are in
   the opposite order to the twin's -- `p[0x9B] += 1' first, then
   `*(u16 *)(k + 0xA) += 1' -- which is the order the `sb 0x9B' / `lhu 0xA'
   pair appears in at .Lb02_80174194. */
void func_b25_80174C04(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;
    u8 *e;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23A8;
        func_80047784(c, D_800E23A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        p[0x9B] += 1;
        return;
    case 1:
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(u8 **)(c + 0x2C) = D_800E23B0;
            func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
                return;
            }
            p[0x9B] += 1;
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0xA) += 1;
            return;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if (d[0x25] == 0)
        {
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            func_800AA79C(p, b, c, d);
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b25_8017531C(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            e = D_80082E80;
            if (*(s8 *)(c + 0x26) != *(s8 *)(e + 0x26) || *(s8 *)(c + 0x26) < 0)
            {
                if ((s16)func_8009FD40(e, c) >= 2)
                {
                    goto again;
                }
            }
            if ((func_800A6D30() & 7) == 0)
            {
                func_80042B68((s8 *)d, 1);
            }
        }
    again:
        if ((s16)func_80042900((s8 *)d, 1) != 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23B0;
        func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
            return;
        }
        p[0x9B] += 1;
        k2 = (u8 *)&D_80083460;
        *(u16 *)(k2 + 0xA) += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0xA) -= 1;
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* The banked room script's three-state tail of the handler installed at
   p + 0x8C, switching on the state index at p + 0x9B: state 0 re-dresses the
   renderable from the D_800E23A8 facing table, state 1 is the long refusal
   chain, state 2 hands the actor back to func_b25_80171F1C. The landed
   func_b05_80173BEC (csrc/ovl_dng_b05.c) is the twin -- like this copy it
   opens state 1 with the same d[0x25] test that closes it, so the
   dress/flag/bump tail is written out twice, and it carries the same
   `*(s32 *)(d + 0x1C) |= 0x40000' at both dress sites. Read its comment for
   the mechanism: the facing table named directly at all three sites rather
   than held in a `u8 *' local (6 differing there), `&D_80083460' as separate
   pointer locals per arm (160 differing shared there), and the `p[0x9B] += 1'
   bump as a `goto' label inside the switch (177 differing after the switch
   there).

   Two deltas from that twin, both read off the listing: state 2 clears only
   bit 3 of d + 0x1C (`addiu $v1, $zero, -0x9') where func_b05_80173BEC
   clears 0x208, and the 0x80000 arm has a six-word block between
   func_800AA888 and the bank-local func_b25_8017531C -- p + 0x92 reduced by
   p + 0xA2 as halfwords (both reads `lhu', so both are unsigned), then
   p + 0xA2 and p + 0x9E cleared. p + 0x92 is READ BEFORE p + 0xA2 IS
   CLEARED in the listing, which is the order the three statements are
   written in. 213 words, 0 differing, on the first compile; the six-word
   block and the `~8' account for the 213 against the twin's 207. */
void func_b25_80174FC8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *j;
    u8 *k;
    s32 v;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(u8 **)(c + 0x2C) = D_800E23A8;
        func_80047784(c, D_800E23A8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        goto bump;
    case 1:
        if (d[0x25] != 0)
        {
            *(u8 **)(c + 0x2C) = D_800E23B0;
            func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            *(s32 *)(d + 0x1C) |= 0x40000;
            j = (u8 *)&D_80083460;
            *(u16 *)(j + 0xA) += 1;
            goto bump;
        }
        g = (u8 *)&D_80083460;
        if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
        {
            return;
        }
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, NULL) != 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0)
        {
            return;
        }
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if ((v & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            *(u16 *)(p + 0x92) -= *(u16 *)(p + 0xA2);
            *(s16 *)(p + 0xA2) = 0;
            *(s16 *)(p + 0x9E) = 0;
            func_b25_8017531C(p, (u8 *)b, c, d);
            return;
        }
        if (*(s8 *)(d + 0x6D) == 0)
        {
            return;
        }
        if ((s16)func_800A2C34(d) != 0 &&
            (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
        {
            return;
        }
        func_800A9A0C(d);
        func_800A9A04(d);
        if (d[0x25] == 0)
        {
            return;
        }
        *(u8 **)(c + 0x2C) = D_800E23B0;
        func_80047784(c, D_800E23B0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(d + 0x1C) |= 0x40000;
        *(u16 *)(g + 0xA) += 1;
    bump:
        p[0x9B] += 1;
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        k = (u8 *)&D_80083460;
        *(u16 *)(k + 0xA) -= 1;
        *(s32 *)(d + 0x1C) &= ~8;
        *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        return;
    }
}

/* The four-argument installer's banked twin: func_800DA660 in the resident
   overlay is its first half verbatim, and this one adds the gate's `else`.
   50 words, 0 differing.
   THE `(s16)` CAST ON func_800AD9B4 IS THE ONE LOAD-BEARING SPELLING: the
   target tests the result as a halfword, `sll $v0, 16` then `blez` with no
   `sra`. Written `func_800AD9B4(c, d) > 0` it is 49 words / 38 differing.
   Measured and equal at 50 words / 0 differing, so none of these is the
   reason it lands: passing the four arguments explicitly to func_800AC82C
   (they are already in $a0..$a3, and the declaration is empty-paren, so the
   call emits the bare `jal` either way); hoisting D_800E2348 into a local as
   ovl_dungeon_800B348C.c does; splitting the `&&` into nested `if`s; reading
   the +0x1C flag word as `u32` instead of `s32`. */
void func_b25_8017531C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
    else if (*(u8 **)(c + 0x2C) == D_800E23A8 && (*(s32 *)(d + 0x1C) & 0x208) == 0)
    {
        *(u8 **)(c + 0x2C) = D_800E2348;
        func_80047784(c, D_800E2348[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}

/* A two-gate handler installer: the outer gate is called with no arguments at
   all -- no instruction sets up $a0..$a3 before it -- so the four incoming
   registers reach it untouched, and the inner gate is handed the third and
   fourth parameters. The second parameter is never mentioned. The inner
   result is read as a halfword: `sll 16` with no `sra` and a `blez` is
   `(s16)x > 0`. Same shape as the landed func_800D9C18 in
   csrc/ovl_dungeon_800D50FC.c. 25 words, 0 differing.

   The handler field is written through a `u8 *` rather than UnkDungeonActor,
   whose typedef lives in csrc/ovl_dungeon_decls.h and is not in this unit's
   context; `*(u32 *)(p + 0x8C) = (u32)f` is the spelling the landed
   func_800D991C tail already uses for the same store. */
void func_b25_801753E4(u8 *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_b25_80171F1C;
        }
    }
}

/* 14 words, 0 differing. */
void func_b25_80175448(u8 *p)
{
    if (func_800AC480(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_80175480(u8 *p)
{
    if (func_800AC5C8(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_801754B8(u8 *p)
{
    if (func_800ACE34(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_801754F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175594);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175BE0);

/* 0x10 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170940[] = {
    0x82,
    0x61,
    0x82,
    0x95,
    0x82,
    0x94,
    0x81,
    0x44,
    0x81,
    0x44,
    0x81,
    0x44,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0x38 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170950[] = {
    0x82,
    0x68,
    0x82,
    0x94,
    0x81,
    0x66,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x8D,
    0x82,
    0x90,
    0x82,
    0x8F,
    0x82,
    0x93,
    0x82,
    0x93,
    0x82,
    0x89,
    0x82,
    0x82,
    0x82,
    0x8C,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x94,
    0x82,
    0x8F,
    0x81,
    0x40,
    0x82,
    0x8D,
    0x82,
    0x81,
    0x82,
    0x8B,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x89,
    0x82,
    0x94,
    0x00,
    0x00,
    0x00,
    0x00,
};

/* 0xc bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170988[] = {
    0x82,
    0x97,
    0x82,
    0x89,
    0x82,
    0x94,
    0x82,
    0x88,
    0x81,
    0x40,
    0x00,
    0x00,
};

/* 0x24 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_80170994[] = {
    0x81,
    0x66,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x83,
    0x82,
    0x95,
    0x82,
    0x92,
    0x82,
    0x92,
    0x82,
    0x85,
    0x82,
    0x8E,
    0x82,
    0x94,
    0x81,
    0x40,
    0x82,
    0x8C,
    0x82,
    0x85,
    0x82,
    0x96,
    0x82,
    0x85,
    0x82,
    0x8C,
    0x81,
    0x44,
    0x00,
    0x00,
};

/* 0x38 bytes of this bank's own rodata, between its jump tables,
   read by func_b25_80175CD8. Spelled as BYTES because that is all the
   reference witnesses, and `const' because without it the array
   goes to .data and the linker puts it outside this segment's
   declared .rodata. */
const u8 D_b25_801709B8[] = {
    0x82,
    0x6D,
    0x82,
    0x8F,
    0x81,
    0x40,
    0x82,
    0x8D,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x86,
    0x82,
    0x8C,
    0x82,
    0x8F,
    0x82,
    0x8F,
    0x82,
    0x92,
    0x82,
    0x93,
    0x81,
    0x40,
    0x82,
    0x81,
    0x82,
    0x82,
    0x82,
    0x8F,
    0x82,
    0x96,
    0x82,
    0x85,
    0x81,
    0x40,
    0x82,
    0x99,
    0x82,
    0x8F,
    0x82,
    0x95,
    0x81,
    0x49,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

INCLUDE_ASM("asm/nonmatchings/ovl_dng_b25", func_b25_80175CD8);

/* 14 words, 0 differing. */
void func_b25_801765DC(u8 *p)
{
    if (func_800AB464(p) > 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}

/* 14 words, 0 differing. */
void func_b25_80176614(u8 *p)
{
    if (func_800AC7EC(p) != 0)
    {
        *(s32 *)(p + 0x8C) = (s32)func_b25_80171F1C;
    }
}
