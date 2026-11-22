/* PARK func_800D13B8 -- 161 of 164 words, 66 aligned differences (j40, 2026-11-16).

   Shape: the minimap's spoke redraw, called once a frame from
   func_800894A0.  Bail unless func_80044724() says the camera moved; take
   the camera-relative facing `t = 2 - ((D_80083160.unk_C8 + 0x100) >> 9)`,
   run func_800D1338(), read the cell (x, y) out of D_80083178's third
   SVECTOR (`(u8 *)&D_80083160 + 0x18`, +0xA4/+0xA6, each >> 6), and paint
   spokes with func_800D112C: four on an ODD facing ((k+-1) at the cell,
   (k+-3) one 10-unit step along D_8006CCD8/E8) and six on an EVEN one
   (k, k+-2 at the cell, then k+4 and k+-2 one step out).

   RESIDUE -- three words, all of them copies the allocator makes and cse
   will not let a source copy reproduce:
     * 0x800D142C  addu $s0, $s2, $zero   and
     * 0x800D1430  addu $a1, $s0, $zero   -- the ODD arm moves `x` into $s0
       and `y` into $s2, which frees $s3 for the D_8006CCD8 index; the
       candidate keeps x/y in $s5/$s6 in both arms.
     * 0x800D14F8  addu $s1, $s4, $zero   -- the EVEN arm copies `k` into
       $s1 so $s4 can be recycled for `(k + 2) & 7`.
   Those two copies are also why the target needs only SIX callee-saved
   registers (frame 0x30, $s0..$s5) where the candidate needs seven
   ($s0..$s6): with x/y/k pinned, `r`, `u` and the two table pointers each
   want their own.  All 66 differences are register numbers that follow
   from it; no opcode, immediate or displacement differs.

   THE ONE LEVER THAT IS LOAD-BEARING, AND IT IS WORTH TWELVE WORDS:
   THE EVEN ARM'S LAST CALL UPDATES x AND y IN PLACE.  Written
       func_800D112C(u, (s16)(x + D_8006CCD8[k] * 10),
                        (s16)(y + D_8006CCE8[k] * 10));
   the two arms' last calls compile to byte-identical 13-instruction tails,
   jump.c's cross_jump merges the whole thing, the odd arm ends `j` into the
   even arm's tail and the function is 149 words / 75 rows.  Written
       x += D_8006CCD8[k] * 10;
       y += D_8006CCE8[k] * 10;
       func_800D112C(u, (s16)x, (s16)y);
   the tails differ and only the target's shared `jal func_800D112C` + `nop`
   at 0x800D1618 is merged: 161 words / 66 rows.  The SAME edit on the odd
   arm instead is 161 / 68; on BOTH arms the tails match again and it is
   back to 149 / 67.  The target's own code is destructive in both arms, but
   on different registers -- which is the residue above, not a source fact.

   MEASURED AND REJECTED (all BYTE-IDENTICAL to the body above, i.e. cse's
   canon_reg rewrites every one of them back):
     * `px = x; py = y;` at the head of the odd arm, with px/py used in its
       four calls; and the same written as a RECOMPUTATION
       `px = *(u16 *)(e + 0xA4) >> 6;`.
     * `kk = k;` or `kk = t & 7;` at the head of the even arm with `kk` used
       throughout it.
     * `(k + 2) & 7` / `(k - 2) & 7` written out at both of their use sites
       instead of held in `r` / `u`.
     * all six orders of the `x`/`y` declarations against `t`/`k`/`r`/`u`.
     * adding `s16 func_800D112C(s16, s32, s32);` and
       `void func_800D1338(void);` to the declaration block (the calls
       otherwise ride implicit `int func()` declarations, as func_8009EF04's
       callers do).
   And: inverting the test to `if ((t & 1) == 0)` with the arms swapped is
   149 words / 136 rows -- the arm order is fixed.

   Three register-copy words in a 164-word function whose every opcode
   already matches.

   ====================================== 2026-11-21: 161/164 words, 66 rows
                                          -> 164/164, 70 rows

   `s32 x' -> `s16 x'.  THE WORD GAP IS CLOSED; this park is no longer the
   one with an unexplained word count.  The declaration is honest: `x' is
   `*(u16 *)(e + 0xA4) >> 6', at most 0x3FF, and every use is already cast to
   `(s16)'.

   WHY IT WAS NEVER FOUND, AND IT WAS A TOOL BUG RATHER THAN A HARD PROBLEM.
   widthsweep.py accepted a candidate only when its ROW COUNT improved, so on
   a park that is word-short it threw this one away for being four rows worse
   -- while being three words better, which is the thing that actually
   matters.  Fixed in the same commit: the objective is now (word gap, rows),
   which is what measure_finds.py has always used and for exactly this
   reason.  On the first run after the fix it was the first candidate found.

   The neighbours, all measured serially against the real unit:
       s16 x                 164/164, 70 rows   <- this body
       s32 x (was)           161/164, 66
       s16 x and s16 y       157/164, 52        fewer rows, SEVEN words short
       u16 x                 166/164, 76
       s16 y alone           155/164, 82
       s16 x with u16 y      160/164, 69
   `s16 x' with `s16 y' is the trap: 52 rows reads like the best result on
   the page and it is seven words from the target, i.e. a different function.

   ALSO MEASURED AND REJECTED on the 161-word body (the park's residue says
   the target makes three register copies the candidate does not, so these
   were attempts to force them):
     * both arms' last call destructive with a SEPARATE pair of locals per
       arm, to stop jump2 cross-jumping the tails: 153/83 with the odd arm's
       own pair, 153/95 with the even arm's, 153/91 with both.  cse's
       canon_reg propagates the copies away and the merge happens anyway, so
       all three land on the 149-word merged shape plus four words.
     * hoisting `r' and `u' to the top of the even arm (159/76), only `u'
       (161/67), only `r' (159/74), and both above the `if' (161/75).
   widthsweep is clean on the new body.

   Measured with
     python3 tools/decomp/parkresidue.py func_800D13B8

   ====================================== 2026-11-21: 164/164, 70 -> 67 -> 63

   stmtsweep: `k = t & 7;' moves between the two coordinate loads.  70 -> 67.
   refsweep on that body: clean (8 names, 252 insertion points).

   devsweep: an `if (e)' DUPLICATED ARM around `func_800D112C(k, x, y);'.
   67 -> 63.  THIS IS A DEVICE AND I AM NOT HAPPY WITH IT -- `e' is `g + 0x18'
   and can never be null, so the condition is a pure reference-adder.  It is
   recorded here rather than defended: the device-free body is 67 rows at the
   same 164 words, and if a later change makes the residue go the other way
   this is the first thing to delete.  See the floor_log2 entry in
   docs/matching-levers.md -- a device can only move a quantity UP the
   allocation queue, so if the arithmetic ever says this one needs to be
   cheaper, no spelling of this device will do it.

   NOT a second SET: the wrapped statement is a void call with no assignment,
   so this buys the extra reference on `e' and nothing else.

   Measured with
     python3 tools/decomp/parkresidue.py func_800D13B8
 */
void func_800D13B8(void)
{
    u8 *g;
    u8 *e;
    s32 t;
    s32 k;
    s32 r;
    s32 u;
    s16 x;
    s32 y;

    g = (u8 *)&D_80083160;
    e = g + 0x18;
    if (func_80044724() == 0)
    {
        return;
    }
    t = 2 - ((*(s16 *)(g + 0xC8) + 0x100) >> 9);
    func_800D1338();
    x = *(u16 *)(e + 0xA4) >> 6;
    k = t & 7;
    y = *(u16 *)(e + 0xA6) >> 6;
    if ((t & 1) != 0)
    {
        func_800D112C((k + 1) & 7, x, y);
        func_800D112C((k - 1) & 7, x, y);
        func_800D112C((k + 3) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        func_800D112C((k - 3) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
    }
    else
    {
        if (e)
        {
            func_800D112C(k, x, y);
        }
        else
        {
            func_800D112C(k, x, y);
        }
        r = (k + 2) & 7;
        func_800D112C(r, x, y);
        u = (k - 2) & 7;
        func_800D112C(u, x, y);
        func_800D112C((k + 4) & 7, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        func_800D112C(r, (s16)(x + D_8006CCD8[k] * 10), (s16)(y + D_8006CCE8[k] * 10));
        x += D_8006CCD8[k] * 10;
        y += D_8006CCE8[k] * 10;
        func_800D112C(u, (s16)x, (s16)y);
    }
}
