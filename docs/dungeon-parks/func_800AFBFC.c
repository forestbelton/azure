/* PARK: func_800AFBFC, 227 of 238 words, 149 aligned differences.

   ====================================== 2026-11-21: 117 rows -> 116

   Cancelled increment pairs from tools/decomp/refsweep.py, run on the
   body the devsweep round before it produced.  The two sweeps alternate:
   each one changes which name is contested, so the other finds something
   new on the result.  devsweep re-run after this found nothing further.

   ====================================== 2026-11-21: 123 rows -> 117

   Two more do-while wrappers from tools/decomp/devsweep.py, around
   `m = ang;` and `sp->unk_70[0].vy = sp->unk_70[2].vy = y;`: 123 rows -> 117.
   refsweep in the same batch reached 121 with `m++; m--;` before the
   `while (1)`, so the wrappers win and the cancelled pair should be re-swept
   on this body.

   ====================================== 2026-11-21: 149 rows at 227/238 words
                                          -> 123 AT 238/238

   TWO duplicated-arm devices, found by tools/decomp/devsweep.py, and the
   first one is the park's ELEVEN MISSING WORDS:

     * `if (p)` around `q = func_800AFFB4(a, c, sp, p, 1);` -- 227 words and
       149 rows -> 238 words and 138.  The eleven words come back because the
       wrapper's free references keep the call's arguments live past the point
       where this body had been letting gcc fold them away.
     * `if (x)` around `sp->unk_70[1].vy = sp->unk_70[3].vy = y2;` -- 138 ->
       123.

   Two more were found and then superseded inside the same sweep (`if (y)`
   around `t = *(u8 **)(c + 0x8);` at 136, a do-while around `m = ang;` at
   135), so this body is not finished: re-running devsweep on it is the next
   thing to do.
   ovl_dungeon.  Measured 2026-11-16.  The kind-0x12 panel's render handler,
   installed by func_800B0544's `func_8004491C(n, (u32)func_800AFBFC)`, so the
   signature is the engine's three-argument handler shape
   `s32 f(u8 *p, UnkVec *v, UnkSlot *q)` -- compare csrc/unk_1811C.c:186's
   `s32 func_80044BB0(u8 *p, UnkVec *v, UnkSlot *s)`.  The second parameter is
   never read ($a1 is never stored).

   WHAT IT DOES.  It sweeps a ring of quads outward from the panel's current
   bearing in BOTH directions, 0x80 of PSX angle per segment, handing each
   segment to the landed func_800AFFB4 (csrc/ovl_dungeon.c:20831) and stopping
   the moment that returns NULL.  Each loop body emits TWO segments, with the
   fifth argument alternating 0 and 1 -- which is exactly what func_800AFFB4's
   own comment records ("func_800AFBFC is the only caller and passes 0 or 1 as
   the fifth argument").  The four SVECTORs at sp->unk_70[0..3] are the quad:
   the previous spoke's (x, y) goes to [0] and [2] and the new one to [1] and
   [3] going forward, and the other way round coming back.

   DECLARATIONS THIS NEEDS (both are prototype CHANGES, see below):
     s32 func_800AFBFC();                                  (was `void`)
     POLY_GT4 *func_800AFFB4(u8 *, void *, UnkDungeonSP5 *, POLY_GT4 *, s16);
   The second one must go AFTER `} UnkDungeonFT4W;`, not in the declaration
   block at the top of the file: UnkDungeonSP5 is typedef'd at :1792 and the
   declaration block ends at :1540.

   ------------------------------------------------------------------ residue

   ELEVEN MISSING WORDS, no extra ones, and every one of them is a register
   allocation consequence rather than a wrong statement.  The whole control
   flow, every store offset, every branch and both loop shapes already match;
   `aligndiff` puts 149 of 238 rows apart and all of them are downstream of
   the eleven.  The three identified groups:

   (1) THE THIRD PARAMETER IS IN A REGISTER HERE AND IN ITS HOME SLOT IN THE
       TARGET.  The target reloads `c` with `lw $t0, 0x50($sp)` at 0x800AFC48,
       0x800AFC68 and 0x800AFC90 and with `lw $a1, 0x50($sp)` at each of the
       four func_800AFFB4 call sites; this body keeps it in $fp/$s8 and pays
       one `move s8,a2` instead.  Net +3 words for the target.

   (2) FOUR `addu` COPIES IN EACH LOOP'S SECOND HALF.  The target computes
       the second half's x and y through a CALLEE-SAVED register and then
       copies: `subu $s0, $s0, $fp` / `addu $s4, $s0, $zero` at 0x800AFDB4 and
       0x800AFF10, and `subu $v1, $v1, $s7` / `addu $s3, $v1, $zero` at
       0x800AFDD0 and 0x800AFF2C.  The reason the chain is in a callee-saved
       register at all is that x has to survive the func_800644B8 call that
       follows it, which is true in the first half too -- there the `subu`
       lands in $s3 directly, in the jal's delay slot.  Every spelling tried
       here writes the `subu` straight into x's own register.

   (3) `addu $s6, $s0, $zero` at 0x800AFCC4 -- the target copies the seed
       angle into the forward loop's accumulator; here the accumulator IS the
       seed's register.  A separate `s32 bs` seed local does not produce it
       (227 words, 230 masked).

   The same pressure argument covers all three: the target holds NINE
   callee-saved pseudos (sp, p, cx, cy, ang, k, the sign-extended angle, x, y)
   and puts a, c and m in memory, while this body fits `c` into $fp because
   its `k` and its sign-extended angle share one register.

   WHAT IS ALREADY SETTLED, and should not be re-derived:

     * `s32 cx`/`s32 cy`, NOT `u16`.  As `u16` the two `lhu`s get an
       `andi $v0, 0xFFFF` apiece and `cy` spills to a halfword stack slot:
       234 words / 220 masked for the first draft that had them narrow.
     * `u16 m` for the backward loop's accumulator, with `(s16)m` at the two
       trig calls.  That is what produces the target's `lhu $t0, 0x18($sp)` +
       `sll`/`sra` pair; `s16 m` is 226 words / 233 masked.
     * A SEPARATE `q` FOR THE CALL RESULT, with `p = q;` after the test.  The
       target's `addu $s5, $v0, $zero` sits AFTER the `beqz`, i.e. on the
       success path only, and the loop-back is `beqz` + `nop` + `j`.  Writing
       `p = func_800AFFB4(...); if (p == NULL) break;` moves the move above
       the branch and turns the back edge into a single `bnez`: 221 words.
     * `u8 *g = (u8 *)&D_80083160;` A POINTER LOCAL, not `D_80083160.unk_AC`
       &c.  Worth FOUR words (223 -> 227).  It is spilled, so reload
       rematerialises its REG_EQUIV at each site -- `lui`+`addiu`+`lhu 0xC8`
       where the direct struct reference folds the offset into the `%lo` and
       is one word shorter, and `lui`+`addiu`+`lw 0x0` in the tail.  At
       offset 0 in the head reload folds it anyway and emits
       `lui` + `lw %lo(D_80083160)`, which is what the target has.
       Same lever as func_8009B88C's note (5) for the 0x1F800000 pointer.
     * `(u16)` ON unk_C8.  The field is declared `s16`, and the target reads
       it with `lhu`; the mask `& 0xF00` makes the two equivalent.
     * The texture byte is read ONCE into a local: `v = t[1]; c[0xF] = v;`
       then `c[0xF] = (f & 4) ? (v | 2) : (v & 0xFD);` inside the `f & 8`
       test.  Re-reading `t[1]` in each arm costs three `lbu`s and an extra
       `j`, because the `sb` to c[0xF] invalidates it.
     * `ang = a[6] + 0xC00;` as its OWN STATEMENT before the `-=`.  Written
       `(a[6] + 0xC00) - ((...) & 0xF00)` fold reassociates it into
       `a[6] - (X - 0xC00)` and the `addiu $s0, $s0, 0xC00` disappears.

   MEASURED AND REJECTED (all with the rest of the body as below):
     base (this file)                                   227 / 230 masked
     `(void)&c;`                                        228 / 236
     `(void)&a;` + `(void)&c;`                          227 / 236
     `u8 *sp` with `*(s16 *)(sp + 0x70)` casts          227 / 230
     `u8 *sp` + `(void)&c;`                             228 / 236
     separate `s32 bs` seed local                       227 / 230
     `ang += 0x80; k = ang;` for both halves            223 / 224
     `ang = k + 0x80; k = ang;` in the second half      223 / 232
     a third angle local `k2` for the second half       224 / 231
     one angle variable only (no `k`)                   223 / 224
     one x/y pair (no x2/y2)                            223 / 232
     an explicit `s16 e = k;` for the trig argument     223 / 232
     `s16 m`                                            226 / 233
     `u16 cx`/`u16 cy`                                  234 / 220

   WHAT THIS CANDIDATE HOLDS FIXED: the three-argument signature, the two
   two-segment loops, the store ORDER inside each quad (`[0] = [2] = x` as a
   chained assignment, x before y), and the fact that the seed trig pair is
   computed once before each loop rather than inside it.

   PROTOTYPE CHANGES THIS LANDING NEEDS, and who else is affected:
     * csrc/ovl_dungeon.c:1420 `void func_800AFBFC();` -> `s32 ...`.  The body
       ends in `addu $v0, $zero, $zero`, so it returns 0 and cannot be void.
       The only other mention is `func_8004491C(n, (u32)func_800AFBFC)` at
       :20965, which takes the address and constrains nothing, so no landed
       caller changes.
     * func_800AFFB4 has NO declaration in the unit and is defined BELOW this
       call site, so the call currently rides an implicit declaration that
       returns `int`.  Adding the real prototype is what lets `q` be a
       POLY_GT4 *.  It changes no other call site: func_800AFBFC is its only
       caller. */

s32 func_800AFBFC(u8 *a, void *b, u8 *c)
{
    UnkDungeonSP5 *sp;
    u8 *g;
    POLY_GT4 *p;
    POLY_GT4 *q;
    u8 *t;
    s32 cx;
    s32 cy;
    s32 f;
    s32 v;
    s32 ang;
    s32 k;
    u16 m;
    s32 x;
    s32 y;
    s32 x2;
    s32 y2;

    g = (u8 *)&D_80083160;
    sp = (UnkDungeonSP5 *)0x1F800000;
    p = *(POLY_GT4 **)(*(u8 **)g + 0x8D0);
    sp->unk_20 = (u32)(*(u8 **)g + 0x8B0);
    t = *(u8 **)(c + 0x8);
    sp->unk_118 = (u32)t;
    cx = *(u16 *)(g + 0xAC);
    cy = *(u16 *)(g + 0xAE);
    v = t[1];
    c[0xF] = v;
    f = *(u16 *)(c + 0x14);
    if (f & 8)
    {
        c[0xF] = (f & 4) ? (v | 2) : (v & 0xFD);
    }
    ang = a[6] + 0xC00;
    ang -= (*(u16 *)(g + 0xC8) + 0x80) & 0xF00;
    do
    {
        m = ang;
    } while (0);
    x = func_80064584((s16)ang) * 6 - cx;
    y = func_800644B8((s16)ang) * 6 - cy;
    while (1)
    {
        k = ang + 0x80;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        do
        {
            sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        } while (0);
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        if (x)
        {
            sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        }
        else
        {
            sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        }
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
        k += 0x80;
        ang = k;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        x = func_80064584((s16)k) * 6 - cx;
        y = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        q = func_800AFFB4(a, c, sp, p, 1);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    x = func_80064584((s16)m) * 6 - cx;
    y = func_800644B8((s16)m) * 6 - cy;
    while (1)
    {
        k = m - 0x80;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        if (p)
        {
            q = func_800AFFB4(a, c, sp, p, 1);
        }
        else
        {
            q = func_800AFFB4(a, c, sp, p, 1);
        }
        if (q == NULL)
        {
            break;
        }
        p = q;
        k -= 0x80;
        m = k;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        x = func_80064584((s16)k) * 6 - cx;
        m++;
        m--;
        y = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)p;
    return 0;
}
