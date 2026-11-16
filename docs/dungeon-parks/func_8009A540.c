/* PARK: func_8009A540, 75 of 75 words, 23 aligned differences.
   ovl_dungeon.  Measured 2026-11-16; `aligndiff.py` on a copy of the unit with
   this body spliced over the stub reproduces the number exactly.

   NO NEW DECLARATIONS.  Every symbol it uses is already declared in the unit,
   and the prototype `s16 func_8009A540(s32, s32, s32, s16);` at line 572 is
   pinned by eight landed call sites -- do not change it.  The body gets its
   sign extensions from explicit `(s16)` casts instead.

   ------------------------------------------------------------------ residue

   (1) TWO WRONG WORDS: the D_800DCEBC base, plus its position and the order of
       the two `lhu`s after it.  Inside the loop the target has

           lui   $v0, %hi(D_800DCEBC)
           addiu $v0, $v0, %lo(D_800DCEBC)

       in a CALL-CLOBBERED register, materialised at the point of use, AFTER
       the `lhu` of the D_800DCEAC element.  This body uses the raw constant
       address, which assembles to `lui 0x800D` / `ori 0xCEBC` -- the right
       NUMBER of words in the right place, the wrong two words.

       Why the symbol spelling cannot be used here.  As `D_800DCEBC[k]` the
       address is a loop invariant and `scan_loop` admits it under arm (1) --
       `reg_in_basic_block_p`, the set and its single use being in one basic
       block (loop.c:794-800) -- so it is hoisted into the preheader.  That
       makes it a NINTH quantity live across the loop's two calls; mips.h
       defines no `REG_ALLOC_ORDER`, so `find_reg` takes plain ascending
       register number, hands it $fp, and the prologue grows a tenth
       save/restore pair: 77 words.  The target has exactly EIGHT such
       quantities -- i, a, xx, yy, wx, wz, z and the D_800DCEAC base, in
       $s0..$s7 -- and does not touch $fp, so in the real source this address
       is not hoisted.

       Measured and rejected, all at 77 words: the plain symbol (42
       differing); `u16 *tb = D_800DCEBC;` as a user variable inside the
       conditional (42); the same with `__asm__("" : "+r"(tb))` to break its
       invariance (47); that device on the D_800DCEAC side instead (47); an
       independent `extern u16 D_800DCEBCi[] __asm__("D_800DCEBC");` alias
       (42); and `*(u16 *)((u8 *)D_800DCEBC + k * 2)` (42).  The raw constant
       is the only spelling found that keeps the count at 75.

   (2) A STRAIGHT $s2/$s3 SWAP: the target holds `a` in $s2 and `xx` in $s3,
       this body has them the other way round, which shows at eleven rows
       including the two stack slots they are saved in.  The target also reads
       the incoming `$a0` for the `a & 1` test where this body reads the copy.
       Swept with no improvement: the y block before the x block (28); an early
       `if ((a & 1) == 0) return 1;` (30); `i = -1` lifted out of the `if`
       (28); the loop as an explicit `do`/`while` (23, byte-identical codegen).

   -------------------------------------------------------------- what paid

   79/47 -> 75/23, in order:

   * `s32 xx = (s16)x;` NOT `s16 xx = x;`.  An s16 local is re-extended at
     every use, so `(xx << 6)` folds to one `sll 16`/`sra 10` and the call
     argument needs its own pair; an s32 local holding the truncated value is
     sign-extended once and shared.  79 -> 77.
   * the raw constant address for D_800DCEBC, above.  77 -> 75.
   * the two sums written INLINE in the call, with no `u16 ax`/`u16 az`
     locals.  36 -> 29.
   * `i = -1;` as its own statement BEFORE the xx/wx block, the loop written
     `for (; i < 2; i += 2)`, and the D_800DCEAC base in a user variable `t`
     assigned BETWEEN the x block and the y block.  That is where the target's
     preheader puts all three -- and since `move_movables` emits a loop.c
     hoist at the END of the preheader, the middle position is itself the
     evidence that the real source has a user variable there.  29 -> 23.

   Word-exact with the whole residue in two words in one place: permuter food
   by the triage rule in docs/dungeon-sweep.md. */

s16 func_8009A540(s32 a, s32 x, s32 y, s16 z)
{
    u16 w;
    s32 xx;
    s32 yy;
    s32 wx;
    s32 wz;
    s32 i;
    s32 k;
    u16 *t;

    if ((a & 1) != 0)
    {
        i = -1;
        xx = (s16)x;
        wx = (xx << 6) + 0x20;
        t = D_800DCEAC;
        yy = (s16)y;
        wz = (yy << 6) + 0x20;
        for (; i < 2; i += 2)
        {
            if (func_8009A350(xx, yy, (a + i) & 7, &w) != 0)
            {
                k = ((u16)a + i) & 7;
                if (func_800BCB04h((u16)(t[k] + wx), (u16)(*(u16 *)(0x800DCEBC + k * 2) + wz),
                                   z) >= 0x201)
                {
                    break;
                }
            }
        }
        if (i < 2)
        {
            return 0;
        }
    }
    return 1;
}
