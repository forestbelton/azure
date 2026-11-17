/* PARK -- func_8009C93C (ovl_dungeon).  263 of 263 words, 15 ALIGNED
   DIFFERENCES (ovlverify reports 16 masked; one row is a transposed pair and
   misses twice).  Agent j55, 2026-11-16, rewriting j49's park: the word
   count is now EXACT and the residue is down from 33 aligned rows.

   SIGNATURE, corrected here:
     u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
   The FOURTH parameter is a u16, not the s32 j49 assumed -- see the prologue
   lever below, which is worth 5 rows.  The fifth argument arrives at
   0x40($sp).  The five call sites ride an IMPLICIT declaration today, and
   every one passes a constant 0 or 1 for `dist'.  THE DECLARATION HAS BEEN
   MEASURED AGAINST ALL FIVE ENCLOSING FUNCTIONS and none of them moves:
   with `u8 *func_8009C93C(u8 *, u8 *, s32, u16, u8 *);' added to the
   declaration block, func_8008C8BC 121/121, func_800973F0 59/59,
   func_80098864 47/47, func_800AC010 284/284 and func_800CA788 109/109 are
   all still 0 differing (2026-11-16, agent j55, against csrc at f196f77).

   ---------------------------------------------------------------- residue

   FOUR OF THE 15 ARE THE PROVED-UNREACHABLE `|= 0x80000000' GATE, words
   19/21/22/23 -- docs/matching-levers.md, "A PROVED-UNREACHABLE RESIDUE:
   THE `|= 0x80000000' GATE'S `$v0'/`$v1' SWAP".  Re-confirmed here, nothing
   spent beyond four spellings that are all byte-identical to this body:
   `0x80000000 | *(s32 *)(a + 0x14)', `*(u32 *)(a + 0x14) |= 0x80000000',
   `*(s32 *)(a + 0x14) &= ~0x7FFFFFFF', and the two-condition `if' flattened
   into one.  Hoisting the load into a local (`(t3 = *(s32 *)(a + 0x14)) >= 0'
   then `t3 | 0x80000000') is 265 words / 263 differing.

   THREE are words 33-35, a sched1 transposition: the target emits
   `lui %hi(D_800DCEAC)' / `addiu %lo' and THEN `andi $s0, $v0, 0xE', this
   emits the `andi' first.  Byte-identical spellings tried: `i' inlined into
   its first use inside the call's argument list, `(s32)(w >> 8) & 0xE',
   `u32 i', `i' declared first.  `i = (w & 0xE00) >> 8' is 22 differing and
   a separate `g = (u8 *)D_800DCEAC' pointer local is 23.

   THREE are words 140/144/145: the target's mask lands in `$v1' and its
   `k = t' copy fills the `beqz $v0, .L8009CB94' delay slot, where this body
   leaves a `nop' and puts the mask in `$a0'.  The target has THREE copies
   around `k'/`t' (`lbu $s1, 0x84($s3)', `addu $s0, $s1, $zero' in the jal
   slot, `addu $s1, $s0, $zero' in the beqz slot); this body produces the
   first two exactly.  Every placement of a redundant third `k = t;' was
   measured: before `r = func_800A6D30()' 28, after it 28, with the load
   written `t = a[0x84]; k = t;' 28 -- all WORSE than the 16 below.
   `if (a[3])' for `if (a[3] != 0)' is byte-identical.

   SIX are words 175-181, the second draw.  Target:
     addu $s1, $v0, $zero / addu $a0, $s3, $zero / andi $s0, $s1, 0x1 /
     andi $v0, $s1, 0x2 / sltu $v0, $zero, $v0 / addu $s0, $s0, $v0
   i.e. the call result is copied into a callee-saved register with a PLAIN
   MOVE -- no `andi 0xFFFF' -- and `(r & 2) != 0' stays a `sltu'.  With
   `u16 func_800A6D30()' (the unit's declaration, pinned by 62 call sites)
   assigned to an `s32', the zero-extension is a real insn, and combine then
   rewrites `(r & 2) != 0' as `(r >> 1) & 1' because the masked value's
   nonzero_bits make the ZERO_EXTRACT form cheaper.  Measured on this base:
   a separate `u16'/`s16 r2' DOES delete the mask but also coalesces the
   move away -- 262 words / 105 differing; `s32'/`u32 r2' and `!!(r & 2)',
   `(0 != (r & 2))', `((r & 2) != 0 ? 1 : 0)', `((r & 2) != 0) + (r & 1)'
   are all byte-identical to this body; reusing `k' 30, reusing `t' 30,
   `r = func_800A6D30() & 3' 262/99, computing the func_800981F8 half first
   85 and 266 words, `(r & 2) >> 1' / `(r >> 1) & 1' / a split `t +=' 38.
   WHAT IS WANTED IS A SECOND PSEUDO that the mask-free move cannot be
   coalesced into -- the same shape as the `w' lever below, but the HImode
   trick does not work here because the HImode copy IS the mask.

   --------------------------------------------------- WHAT J55 ADDED, 33 -> 15

   * `u16 w;' AND NOT `u32 w;'.  Worth THE TWO MISSING WORDS, and it is the
     general lever.  The target keeps the re-aimed angle in a SECOND pseudo:
     `$s0' stays the parameter `ang' and is read at word 28 as the addend,
     while `$a1' is `w' -- written at word 9 (`addu $a1, $s0, $zero') and at
     word 29 (`addu $a1, $v0, $zero') and read at word 32 by
     `srl $v0, $a1, 8'.  With `u32 w' (or any SImode `w') cse.c's
     `insert_regs' finds a same-mode REG in the equivalence class, calls
     `make_regs_eqv', and the two share one register: `addu $s0, $s0, $v0'
     then `srl $v0, $s0, 8', and both copies vanish -- 261 words.  An `s16'
     or `u16' `w' is a DIFFERENT MODE, `insert_regs' finds no same-mode REG,
     `w' gets a quantity of its own and both copies survive.  `(w >> 8) & 0xE'
     still compiles to the target's bare `srl'/`andi' because the `& 0xE'
     kills everything the truncation would have.  (The same mechanism landed
     func_800BB2E4 on 2026-11-16; see its comment in csrc/ovl_dungeon.c.)
     j49 measured a plain `s32' temp instead and got 263/47, with the two
     words coming back as a tail nop and a stray `move'.

   * `u16 dist' IN THE SIGNATURE.  Worth 5 rows -- the whole prologue
     cluster, words 7-11.  With `s32 dist' the scheduler emits the `$s4'
     save-and-copy pair BEFORE the `$s2' pair, so the stack argument is
     loaded fifth instead of fourth and `addu $a1, $s0, $zero' lands two
     words late; with `u16 dist' the order is the target's exactly.
     `u8 dist' is 25, `s16 dist' is 264 words / 190, `s16 ang' and
     `u32 ang' are byte-identical to this body.

   * `k = a[0x84]; t = k;' AND NOT `t = a[0x84]; k = t;'.  Worth 2 rows
     (words 137 and 139): the target loads into `k's register and copies
     DOWN to `t', not the other way round.

   * A SEPARATE `s32 t3' FOR THE FINAL `t3 = k; if (t3 >= ...)' COMPARE.
     Worth 4 rows (words 198-203, the whole `sll'/`sra'/`slt'/`bnez' block):
     sharing `t' with the two earlier roles is one global allocno and one
     hard register, and the target plainly uses `$s0' throughout there.
     Splitting the MIDDLE role (`t2' for the `(r & 1) + ...' block) instead
     is 36, and splitting both is 96 -- it is this role specifically.

   ------------------------------------- what j49 established and still holds

   * TWO SCRATCH LOCALS, `s16 k' and `s32 t', reused across the blocks: the
     s16->s32 direction is what emits `sll $s0,$s1,16' / `sra $s0,$s0,16'.
   * `k = t;' BEFORE the `if', not an `else' arm (an `if'/`else' emits a `j').
   * `g = D_800E3D7C;' READ INTO A LOCAL BETWEEN the two `&=' stores of the
     tail.  Read inline in the compare it is 14 aligned rows across words
     232-249; read before BOTH stores, 261/47.

   TRIAGE: 15 rows, 4 of them documented permanent, and the three live
   clusters are all register-name ties inside blocks whose instruction
   streams match.  Permuter food. */
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e)
{
    s16 k;
    s32 i;
    s32 r;
    s32 t;
    s32 t3;
    u8 *g;
    u16 w;

    w = ang;
    if (e == NULL)
    {
        if ((*(s32 *)(a + 0x1C) & 0x400) != 0 && *(s32 *)(a + 0x14) >= 0)
        {
            *(s32 *)(a + 0x14) |= 0x80000000;
            w = ang + ((func_800A6D30() & 7) << 9);
            *(s16 *)(a + 0x2A) = w;
        }
    }
    i = (w >> 8) & 0xE;
    func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)((u8 *)D_800DCEAC + i) >> 1) + 0x20,
                  b[0x25] * 64 + (*(s16 *)((u8 *)D_800DCEBC + i) >> 1) + 0x20,
                  *(s16 *)(a + 0x88), 8, 0x300);
    *(u8 **)(a + 0x60) = e;
    *(s32 *)(a + 0x14) &= ~0x4000000;
    if (e == NULL)
    {
        if (func_8009A540((*(u16 *)(a + 0x2A) >> 9) & 7, b[0x24], b[0x25],
                          *(u16 *)(a + 0x88) - 0x20) == 0)
        {
            return NULL;
        }
        e = func_8009B25C(a, b[0x24] + *(u16 *)((u8 *)D_8006CCD8 + i) * dist,
                          b[0x25] + *(u16 *)((u8 *)D_8006CCE8 + i) * dist,
                          *(s16 *)(a + 0x88));
        *(u8 **)(a + 0x60) = e;
        if (e == NULL)
        {
            return NULL;
        }
    }
    *(u16 *)(e + 0x6A) = *(u16 *)(a + 0x2A);
    if (e[0x13] != 0)
    {
        if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && (*(u16 *)(e + 0x46) & 0x8000) == 0)
        {
            *(u16 *)(e + 0x2A) = *(u16 *)(a + 0x2A) + 0x800;
        }
    }
    *(s32 *)(a + 0x14) |= 0x4000000;
    *(s32 *)(e + 0x14) = (*(s32 *)(e + 0x14) | 0x20000) & ~0x1000000;
    k = a[0x84];
    t = k;
    r = func_800A6D30();
    if (a[3] != 0)
    {
        k = t + (r % a[3]) * 4;
    }
    if (e[0x13] == 0 && *(u8 **)(e + 0x50) != NULL && **(u8 **)(e + 0x50) == 2)
    {
        k -= k >> 2;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && k < 0x80)
    {
        return e;
    }
    r = func_800A6D30();
    t = (r & 1) + ((r & 2) != 0);
    t += (a[0x26] + func_800981F8(a)) * 2;
    *(s16 *)(a + 0x20) = t;
    if ((*(s32 *)(a + 0x1C) & 0x1000) != 0)
    {
        k = a[3] >> 1;
    }
    else
    {
        k = a[3] >> 6;
    }
    t3 = k;
    if (t3 >= (s32)(func_800A6D30() & 0x7F))
    {
        *(s32 *)(a + 0x1C) |= 0x1000000;
    }
    else
    {
        *(s32 *)(a + 0x1C) &= ~0x1000000;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) != 0)
    {
        r = func_800A6D30();
        if (e[3] != 0)
        {
            k = r % e[3];
        }
        else
        {
            k = 0;
        }
        if ((s16)(k + e[0x70]) >> 7 > 0)
        {
            return e;
        }
    }
    *(s32 *)(a + 0x14) &= ~0x4000000;
    g = D_800E3D7C;
    *(s32 *)(e + 0x14) &= ~0x20000;
    if (a == g)
    {
        func_8003E188(e[0x13], 0);
    }
    return e;
}
