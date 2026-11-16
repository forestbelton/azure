/* PARK: func_800B835C, 98 of 98 words, 8 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  The three-primitive tint overlay:
   a draw-area primitive, func_800B84E4's output, an optional 0x60000000
   rectangle, and a second draw-area primitive, all hung on the +0x8B0 OT row.

   NEW DECLARATION NEEDED (func_800B84E4 is defined below this function):

       void func_800B84E4(s16 *, u8 *, u8 *, s32);

   The prototype `void func_800B835C(u8 *, s32 *, s32, s32);` at line 518 is
   already pinned by four landed call sites -- do not change it.  The `u16`
   narrowings on the third and fourth parameters are written out in the body.

   `csrc/ovl_dungeon.c`'s func_8009DF0C (line ~11525) is the line-for-line
   template: same +0x8D0 cursor, same +0x8B0 OT row, same `lean` test, same
   0x60000000 / `u[3] = 3` rectangle.  Read its comment first.

   ------------------------------------------------------------------ residue

   TWO PARAMETER REGISTERS, SWAPPED, AND NOTHING ELSE.  `p` is in $s6 here and
   $s7 in the target; `c` is in $s7 here and $s6 in the target.  That is all 8
   rows -- the two prologue saves, the two parameter copies, the `addu $a1` at
   the func_800B84E4 call and the `andi` before the gate.  Every other word in
   the function, including the whole frame, matches.

   It is a genuine `allocno_compare` tie.  Both are parameters with two
   references; in the target `c`'s live range is 42 insns against `p`'s 44, so
   `c` wins the lower register, and here both measure 44 and the tie falls
   through to `return v1 - v2` -- declaration order -- which gives it to `p`.
   The documented cure (docs/matching-levers.md, "TWO PARAMETERS THAT TIE IN
   PRIORITY allocate in DECLARATION ORDER -- copy one") does not reach it:
   `cc = c;` before the `dd` copy is 10, after it 8, `pp = p;` is 8, and
   `(c & 0xFFFF)`, `u16 dd` and `dd = (u16)d` are 8, 8 and 10.  The copies are
   coalesced because neither parameter is live past its single use, so there
   is nothing for the copy to conflict with.  This is `.greg` work, not the
   permuter.

   -------------------------------------------------------------- what paid

   99/19 -> 98/8, in order, and all three are worth knowing:

   * `s16 lean;` and NOT `s32 lean;`.  99 -> 98 words and 19 -> 18.  As an
     `s32` the boolean is computed into $s0 before the two calls and then
     COPIED to $s1 -- a `move` the target does not have.  The narrower local
     lands where it is wanted first time.  `u8 lean` is 99/18, so it is the
     s32 that is wrong rather than the s16 that is right.
   * A POINTER LOCAL `g = &D_80083160;` for the tail block only.  18 -> 13.
     The target holds `&D_80083160` in $s1 from word 30 to word 75 and reads
     the tail's base with a one-word `lw $v1, 0x0($s1)`; spelled
     `D_80083160.unk_00` the tail rebuilds the address (`lui` + `lw`) and the
     extra word has to come out of somewhere else.  Keeping the address live
     also stops the MIDDLE block rebuilding its own `%hi`, which is the `nop`
     the target has at word 52.  NOTE the ordering: this only pays once `lean`
     is `s16` -- added to the `s32 lean` body it measures 101 words.  Reading
     `g->unk_00` twice in the tail and reading it once into a temp are
     byte-identical.
   * A SEPARATE LOCAL for the middle block's halfword temp.  13 -> 8.  Sharing
     `t` with the one in the first block makes the middle block read into $v1
     where the target uses $v0, and that renames three rows.
   * `dd = d;` as a PLAIN copy placed immediately before `g = &D_80083160;`.
     Worth 2.  The narrowing form `dd = (u16)d;` is 10 -- the `andi` has to
     stay at the call site, where the target has it.  Moving the copy anywhere
     later measures 13: after `g`, after `r`, after the first `AddPrim`, after
     `w[1] = t`, all 13.

   Also measured, no better: hoisting BOTH narrowings into locals (`cc` before
   `dd` and `dd` before `cc`) is 97 words -- one SHORT -- because the hoisted
   `andi` replaces the parameter copy and the gate's own `andi` folds away;
   the target keeps both.  Declaring `w[2]` first, last, or between the
   pointers is byte-identical. */

void func_800B835C(u8 *p, s32 *b, s32 c, s32 d)
{
    Unk80083160 *g;
    u8 *bp;
    u8 *q;
    u8 *r;
    u8 *u;
    s16 w[2];
    s32 dd;
    s16 lean;
    s32 t;
    s32 t2;

    dd = d;
    g = &D_80083160;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    w[0] = *(u16 *)b + ((s16) * ((u16 *)b + 2) >> 1);
    t = *((s16 *)b + 1) + ((s16) * ((u16 *)b + 3) >> 1);
    if (lean)
    {
        t -= 0xE0;
    }
    w[1] = t;
    func_800B84E4(w, p, r, (u16)dd);
    if ((u16)c != 0)
    {
        u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
        *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
        *(u32 *)(u + 0x4) = 0x60000000;
        u[0x3] = 3;
        *(u16 *)(u + 0x8) = *(u16 *)b;
        t2 = *((s16 *)b + 1);
        if (lean)
        {
            t2 -= 0xE0;
        }
        *(s16 *)(u + 0xA) = t2;
        *(u32 *)(u + 0xC) = b[1];
        AddPrim(r, u);
    }
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, b);
    AddPrim(r, q);
}
