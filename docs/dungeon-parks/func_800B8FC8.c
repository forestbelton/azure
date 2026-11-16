/* PARK: func_800B8FC8, 95 of 95 words, 19 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  The five-argument sibling of
   func_800B835C -- `nearsibs.py` scores the pair at 0.891 and that is an
   understatement: the two are the same function with a different middle
   callee and one more parameter.  **Read docs/dungeon-parks/func_800B835C.c
   first**; every lever in its header applies here and was carried over
   rather than re-derived.

   NO DECLARATION NEEDED, and no call site in this unit, so the signature
   below is a free choice rather than something pinned:

       void func_800B8FC8(u8 *p, s32 *b, s32 *b2, s32 c, s32 e)

   It calls func_800B9144, which landed on 2026-11-16 and is declared in the
   unit; the first argument is the `s16 w[2]` pair cast to `u8 *`.

   Note the two SEPARATE record pointers, which is what makes this function
   longer than its sibling: `b2` feeds the `w[2]` pair handed to
   func_800B9144, and `b` feeds the 0x60000000 rectangle and the tail's
   SetDrawArea.  func_800B835C uses one record for both.

   ------------------------------------------------------------------ residue

   A THREE-WAY REGISTER ROTATION plus the prologue scheduling that follows
   from it.  The `%hi(D_80083160)` pseudo is in $s5 here and $s4 in the
   target; `bb` is in $s4 here and $s6 there; `ee` is in $s6 here and $s5
   there.  Every instruction, immediate and displacement matches.  Same class
   as the sibling's residue and the same verdict: `.greg` work, not the
   permuter.

   -------------------------------------------------------------- what paid

   93/23 -> 95/19:

   * `ee = e;` AS THE FIRST STATEMENT IN THE FUNCTION, and nothing else
     before it.  Worth the 2 missing words.  Without it the fifth argument is
     re-loaded from the caller's frame at the call site and never occupies a
     register, so the function is two words short and the target's $fp
     save/restore pair has nothing to hold.  The position is not negotiable:
     all six orderings of `{ee = e; g = &D_80083160; lean = ...}` were
     measured and the three with `ee` first are 95 words while the three with
     anything ahead of it are 93.  Declaring the parameter `s16 e` instead
     reaches the same 95/22, so either spelling gets the word -- what matters
     is that the value is bound before the first call.
   * `bb = b;` right after it.  22 -> 19.  A plain copy of the second
     parameter, used for all four of `b`'s reads.  Copies of the other
     parameters do nothing: `cc = c;` is 22, `pp = p;` is 22, a second copy
     of `b2` is 19, and adding any of them to `bb` is still 19.
   * `s16 lean;` and not `s32` -- 19 against 26 -- carried over from the
     sibling, where it is explained.
   * the `g = &D_80083160;` pointer local for the tail block, and a separate
     local for the middle block's halfword temp, both carried over from the
     sibling.

   Measured and byte-identical, so free to spell either way: `lean` and `g`
   in either order after `ee`/`bb`, and `bb` as a `u8 *` with byte offsets
   instead of an `s32 *`. */

void func_800B8FC8(u8 *p, s32 *b, s32 *b2, s32 c, s32 e)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s16 lean;
    s32 t;
    s32 ee;
    s32 *bb;
    s32 t2;

    ee = e;
    bb = b;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b2;
    t = *((s16 *)b2 + 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_800B9144((u8 *)w, p, r, (s16)ee);
    if ((u16)c != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)bb;
        t2 = *((s16 *)bb + 1);
        if (lean)
        {
            t2 -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t2;
        *(u32 *)(u + 0xC) = bb[1];
        AddPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, bb);
    AddPrim(r, q);
}
