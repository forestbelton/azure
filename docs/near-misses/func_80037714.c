/* func_80037714 (unk_72A8) -- 346 of 346 words, 7 DIFF. NOT compiled.
 *
 * The two-quad draw for one of these actors: projects the +0x0C rect's four
 * corners with RotTransPers4, and if any corner lands on screen builds a
 * textured POLY_FT4 for it; then does the same for the +0x28 rect, which gets
 * an untextured POLY_G4 plus a tpage primitive instead.
 *
 * EVERY instruction matches. What is left is one register name and the six
 * words that follow it; see the last section.
 *
 * Seven things paid for, 355 DIFF down to 7:
 *
 *   * **The stack frame is four SVECTORs and then FOUR 8-BYTE screen-coord
 *     slots**, not `long sxy[4]`. RotTransPers4's four `long *` outputs land
 *     at sp+0x48, 0x50, 0x58 and 0x60 -- stride 8, not 4 -- so whatever the
 *     original declared there was 8 bytes wide. `u8 s[0x20]` with the four
 *     slots cast reproduces the layout, the `lw` for the vertex copies and the
 *     `lhu` pairs for the bound tests exactly.
 *
 *   * **`a = p + 0xC` is ONE pointer, and cse is what makes `a[0]` print as
 *     `0xC($t0)`.** The listing reads the first field through the parameter
 *     and every other field through `$s1`, which looks like two source
 *     pointers and is not: for a MEM whose address is a bare register, cse
 *     substitutes that register's known equivalence `(plus p 12)`, and for
 *     `(plus a 4)` it cannot, because the equivalence is an expression and
 *     `equiv_constant` only folds constants. Write both through `a`.
 *
 *   * **The four `||` terms are `|` in the source, with the `&&`s inline.**
 *     Four locals OR'd afterwards emit the three `or`s at the end and cost
 *     seven words of branches; the listing interleaves each `or` into the
 *     NEXT term's delay slot, which is what `((A0 && B0) | (A1 && B1) | ...)`
 *     gives, because left-associative `|` folds the running value before the
 *     next term is evaluated.
 *
 *   * **Every value stored twice is a CHAINED assignment.** A store to the
 *     stack invalidates cse's memory table, so `v[0].vx = x; v[2].vx = x;`
 *     re-loads `x`; `v[0].vx = v[2].vx = x;` loads once and stores twice, in
 *     the listing's order (rightmost target first). Same for the three colour
 *     bytes, and the ten zero bytes of the POLY_G4 come out in the listing's
 *     exact descending order from one ten-deep chain.
 *
 *   * **`% 64 * 4` is NARROWED and `% 256` must not be.** `build_binary_op`
 *     sets `shorten` for TRUNC_MOD, so `*(s16 *)(a + 8) % 64` is computed in
 *     HImode and converted back, which is the listing's `sll 16`/`sra 14`
 *     (the extension combined with the `* 4`). The v coordinate has no such
 *     pair, so its operand has to be genuinely `int`: a cast does not do it
 *     (`get_narrower` strips NOP_EXPRs), an `s32` local does.
 *
 *   * **The u sum is `a[0x18] + u` with the mod in its own local.** Written
 *     as one expression the add's operands come out (mod, byte); as two
 *     statements they come out (byte, mod) like the listing, and the `lbu`
 *     still lands in the mod's join block -- which matters, because a `lbu`
 *     emitted ahead of the `bgez` is in another basic block and local-alloc
 *     can then no longer tie the sum's destination to it.
 *
 *   * **The ot index is INLINE in AddPrim, not a local.** 15 words. Both
 *     spellings emit the same five instructions, but with `o = n * 4 + 0x70;`
 *     first, `block_alloc` ties the sum's hard `$a0` to its FIRST operand,
 *     the loaded ot base, so the index chain gets `$v0`; inline, the tie goes
 *     to the index chain and it gets `$a0`, which is the listing. Note this
 *     is the opposite of func_800369C8/func_80036A70 in this same unit, where
 *     the index HAS to be a local -- there the base is a local too, so there
 *     is no hard register in the add.
 *
 *   * `g = &D_80083160` as a struct pointer, used in the SECOND half only.
 *     That is the listing's `addiu $s5, $s3, %lo(D_80083160)` in the entry
 *     region: `$s3` keeps the `high` for the first half's two `%lo(...)($s3)`
 *     loads and `$s5` the whole address for the second half's four `0($s5)`
 *     loads. Without it the function is five words short -- one callee-saved
 *     register's save and restore, the `addiu`, and two load-delay nops.
 *
 * WHAT IS LEFT -- 7 words, one tie. The v coordinate's running value lives in
 * `$v0` in the listing and `$v1` here, and the five stores after it follow.
 * `t` is ONE variable across both coordinates (two locals is 343 DIFF), so
 * the u chain pins it to `$v1` and the listing's second chain somehow does
 * not: there `addu $v0, $v0, $v1` ties the destination to operand 0, the
 * `subu`'s result, while here operand 0's quantity is not local -- the `subu`
 * tied its own destination back to the shifted temp, which is set in both
 * `bgez` arms and so crosses blocks. Every spelling of the v sum measured
 * (inline, mod in `u`, mod back into `y`, byte in a local, byte first,
 * chained stores, a separate `w`) is 7 or worse, so the next lever is
 * probably in `combine_regs`'s refusal conditions rather than in the source.
 */

#include "common.h"

void func_80067F20(u32 *, s32, s32, s32, s32);

void func_80037714(u8 *p)
{
    Unk80083160 *g;
    u8 *a;
    u8 *b;
    u8 *q;
    u8 *e;
    SVECTOR v[4];
    u8 s[0x20];
    s32 pz;
    s32 flag;
    s32 t;
    s32 u;
    s32 y;

    g = &D_80083160;
    q = *(u8 **) p;
    a = p + 0xC;
    b = p + 0x28;

    v[0].vx = v[2].vx = *(u16 *) a;
    v[1].vx = v[3].vx = *(u16 *) a + *(u16 *) (a + 4);
    v[0].vy = v[1].vy = *(u16 *) (a + 2);
    v[2].vy = v[3].vy = *(u16 *) (a + 2) + *(u16 *) (a + 6);
    v[0].vz = v[1].vz = v[2].vz = v[3].vz = 0;
    RotTransPers4(&v[0], &v[1], &v[2], &v[3], (s32 *) (s + 0x00), (s32 *) (s + 0x08),
                  (s32 *) (s + 0x10), (s32 *) (s + 0x18), &pz, &flag);
    if (((u16) (*(u16 *) (s + 0x00) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x02) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x08) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x0A) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x10) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x12) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x18) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x1A) + 0x20) < 0x121))
    {
        e = *(u8 **) (D_80083160.unk_00 + 0x8D0);
        *(u8 **) (D_80083160.unk_00 + 0x8D0) = e + 0x28;
        SetPolyFT4(e);
        *(s16 *) (e + 0xE) = GetClut(*(s16 *) (a + 0x14), *(s16 *) (a + 0x16));
        *(s16 *) (e + 0x16) = GetTPage(0, 0, *(s16 *) (a + 0x8), *(s16 *) (a + 0xA));
        e[4] = e[5] = e[6] = a[0x10];
        u = *(s16 *) (a + 0x8) % 64 * 4;
        t = a[0x18] + u;
        e[0x1C] = t;
        e[0xC] = t;
        t = t + a[0xC];
        e[0x24] = t;
        e[0x14] = t;
        y = *(s16 *) (a + 0xA);
        t = y % 256 + a[0x1A];
        e[0x15] = t;
        e[0xD] = t;
        t = t + a[0xE];
        e[0x25] = t;
        e[0x1D] = t;
        *(s32 *) (e + 0x08) = *(s32 *) (s + 0x00);
        *(s32 *) (e + 0x10) = *(s32 *) (s + 0x08);
        *(s32 *) (e + 0x18) = *(s32 *) (s + 0x10);
        *(s32 *) (e + 0x20) = *(s32 *) (s + 0x18);
        AddPrim(D_80083160.unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);
    }

    v[0].vx = v[2].vx = *(u16 *) b;
    v[1].vx = v[3].vx = *(u16 *) b + *(u16 *) (b + 4);
    v[0].vy = v[1].vy = *(u16 *) (b + 2);
    v[2].vy = v[3].vy = *(u16 *) (b + 2) + *(u16 *) (b + 6);
    v[0].vz = v[1].vz = v[2].vz = v[3].vz = 0;
    RotTransPers4(&v[0], &v[1], &v[2], &v[3], (s32 *) (s + 0x00), (s32 *) (s + 0x08),
                  (s32 *) (s + 0x10), (s32 *) (s + 0x18), &pz, &flag);
    if (((u16) (*(u16 *) (s + 0x00) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x02) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x08) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x0A) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x10) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x12) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x18) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x1A) + 0x20) < 0x121))
    {
        e = *(u8 **) (g->unk_00 + 0x8D0);
        *(u8 **) (g->unk_00 + 0x8D0) = e + 0x24;
        SetPolyG4(e);
        SetSemiTrans(e, 1);
        e[4] = e[5] = e[6] = e[0xC] = e[0xD] = e[0x14] = e[0x15] = e[0x16] = e[0x1C] = e[0x1D] = 0;
        e[0xE] = e[0x1E] = 0x40;
        *(s32 *) (e + 0x08) = *(s32 *) (s + 0x00);
        *(s32 *) (e + 0x10) = *(s32 *) (s + 0x08);
        *(s32 *) (e + 0x18) = *(s32 *) (s + 0x10);
        *(s32 *) (e + 0x20) = *(s32 *) (s + 0x18);
        AddPrim(g->unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);

        e = *(u8 **) (g->unk_00 + 0x8D0);
        *(u8 **) (g->unk_00 + 0x8D0) = e + 0xC;
        func_80067F20((u32 *) e, 0, 0, GetTPage(0, 0, 0x140, 0), 0);
        AddPrim(g->unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);
    }
}
