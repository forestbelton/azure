/* PARK func_80089588 -- 148 of 151 words, 80 aligned differences (j40, 2026-11-16).

   Shape: the overlay's HUD/letterbox frame builder.  `D_80083160.unk_00` is
   the live draw buffer; `lean = unk_00 != D_801C9E40` picks between y=0xE0
   and y=0x1C0 for the two semi-transparent POLY_F4 bars; everything else is
   ten primitives taken off the +0x8D0 cursor and hung on the +0x7B0 / +0x830
   OT rows, with the cursor written back once at the end.

   RESIDUE -- three missing words, all in the two POLY_F4 blocks:
     * 0x80089638  addu $s3, $s0, $zero      (a SECOND long-lived pseudo for
                   `lean`; the candidate keeps one and tests it directly)
     * 0x800896A8  addu $v0, $s3, $zero      (that pseudo copied again)
     * 0x800896B4  addiu $v0, $zero, 0x1C0   (the second block's if is
                   one-armed in the candidate, two-armed in the target)
     * the target materialises `addiu $v1, $zero, 0x5` ONCE PER BLOCK; the
       candidate's two `q[3] = 5` stores are cse'd into one callee-saved
       pseudo, which is also what shifts every register by one.
   Everything else aligns: 71 of the 80 differences are register-number-only
   rows (`R` in aligndiff), caused by that one extra callee-saved pseudo.

   MEASURED AND REJECTED (ovlverify words / aligndiff rows):
     * `y = lean ? 0xE0 : 0x1C0;` with ONE store after the if, in either
       block: 150 words / 146 positional.  All four spellings of that if --
       if/else, ternary, `y = 0x1C0; if (lean) y = 0xE0;`, and
       `y = 0xE0; if (!lean) y = 0x1C0;` -- give BYTE-IDENTICAL output, so
       the branch shape here is not a source lever.  jump.c:789
       ("Simplify if (...) x = a; else x = b; by converting it to
       x = b; if (...) x = a;") fires on every one of them, which is what
       hoists the else constant above the branch and leaves one label.
     * THE STORE MUST BE INSIDE BOTH ARMS.  Writing
       `if (lean) { *(s16 *)(q + 0xE) = 0xE0; } else { ... = 0x1C0; }`
       makes each arm two insns, so jump.c:789's `temp3` is a MEM-destination
       set and the transform cannot fire; the two-arm form survives and
       cse stops commoning the two blocks' constants.  That alone is worth
       65 aligned rows: 150w/146 -> 148w/81.  Without it the constants
       0x2A000000, 1 and 0x100 are commoned into THREE extra callee-saved
       registers (s2/s3/s6, frame 0x40 not 0x38) and the function is 150-152
       words with 146-148 differing.
     * THE D_80083160 BASE IS A LOCAL OF ITS OWN: `Unk80083160 *g =
       &D_80083160;` with `g->unk_00` throughout is what produces the target's
       mid-function `addiu $s1, $s1, %lo(D_80083160)` at 0x80089634 and the
       switch from `lw %lo(D_80083160)($s1)` to `lw 0x0($s1)` after the fifth
       access.  Spelling every access `D_80083160.unk_00` loses that word and
       re-emits `lui %hi` in the later cse regions: 149w / 91 rows vs
       148w / 81.
     * `q[0x3] = 5;` moved to the head of each block, or to just after the
       `*(u32 *)(q + 4)` store: 147w / 90 rows.  It must stay after the
       `lhu` re-read.
     * `lean2 = lean;` with the second block testing `lean2`: cse collapses
       the copy, output byte-identical to one local.  Does not buy the
       `addu $s3, $s0, $zero`.
     * reading `*(u16 *)(q + 0xE)` three times instead of once into `h`:
       the first read is forwarded out of the `sh` (no `lhu` at all) and the
       `+ 0xE` is computed twice -- 152w / 148.  `s32 h` is what makes the
       load a real zero_extend that cse cannot forward.
     * x-store order: the target's source order is 0x10, 0x8, 0x14, 0xC
       (confirmed -- the candidate preserves source order exactly), not
       0x8, 0xC, 0x10, 0x14.

   NOT TRIED: anything that produces a second live `lean` pseudo without a
   dead copy.  The permuter wants this one -- three missing words inside two
   40-word blocks whose every other word already aligns. */
void func_80089588(void)
{
    Unk80083160 *g = &D_80083160;
    u32 w[2];
    u8 *q;
    s32 lean;
    s32 y;
    s32 h;

    lean = g->unk_00 != D_801C9E40;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    SetDrawArea(q, g->unk_00);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, g->unk_00);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    func_80067EF4(q, 0, 0);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0xC) = 0x100;
    if (lean)
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    q[0x3] = 5;
    h = *(u16 *)(q + 0xE);
    *(s16 *)(q + 0xA) = h;
    *(s16 *)(q + 0x16) = h + 0xE;
    *(s16 *)(q + 0x12) = h + 0xE;
    AddPrim(g->unk_00 + 0x830, q);
    q += 0x18;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0xC) = 0x100;
    if (lean == 0)
    {
        y = 0x1C0;
    }
    else
    {
        y = 0xE0;
    }
    *(s16 *)(q + 0xE) = y;
    q[0x3] = 5;
    h = *(u16 *)(q + 0xE);
    *(s16 *)(q + 0xA) = h;
    *(s16 *)(q + 0x16) = h + 0xE;
    *(s16 *)(q + 0x12) = h + 0xE;
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0x18;
    func_80067EF4(q, 0, 1);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    w[0] = 0x01C00000;
    w[1] = 0x00100100;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}
