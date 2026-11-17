/* func_8009DAD8 -- the dungeon minimap/radar draw. 269 target words.
   BEST: 269 words, 9 aligned differences. THE WORD COUNT MATCHES AND THE
   ENTIRE RESIDUE IS ONE REGISTER SWAP, which is arithmetic, not spelling:

       head                        is in $s6; the target has $s5
       the hoisted D_800E296C base is in $s5; the target has $s6

   Every opcode, every immediate, every displacement and every other
   register in all 269 words is identical. Rows: 10, 11, 14, 76, 79, 81,
   88, 126, 150 -- all of them one of those two registers.

   REQUIRED DECLARATION (parksplice ships 0 declaration lines for this park,
   so ADD THIS BY HAND to the unit's top declaration block; without it the
   body measures 265 words / 38 rows, and that -- not drift -- is why the
   previous header's 267 / 34 did not reproduce):

       s16 func_8009FD40(u8 *, u8 *);

   func_8009FD40 is DEFINED below func_8009DAD8, so the call otherwise rides
   an implicit `int f()` and loses the `sll 16` / `sra 16` pair in front of
   the `slti`. Worth exactly TWO WORDS (265 -> 267), re-confirmed.

   =====================================================================
   WHAT MOVED THIS PARK: 265/38 -> 267/34 -> 268/25 -> 269/9
   =====================================================================

   (1) +2 words: the func_8009FD40 forward declaration above.

   (2) +1 word, -9 rows: ONE LINE, `mode = 0;` immediately above `k = 0;`.

   A DEAD EARLIER SET MAKES cse2 PICK `mode` AS THE CANONICAL REG OF THE
   COPY'S QUANTITY, SO `addu $s0, $a0, $zero` SURVIVES. In cse.c,
   make_regs_eqv promotes the NEW reg of `(set mode k)` to qty_first_reg
   only when

       (uid_cuid[regno_last_uid[new]]  > cse_basic_block_end
     || uid_cuid[regno_first_uid[new]] < cse_basic_block_start)
     && uid_cuid[regno_last_uid[new]]  > uid_cuid[regno_last_uid[firstr]]

   With `mode` first touched inside the same cse2 block as the copy, both
   clauses of the first test fail, `k` stays canonical, every use of `mode`
   is rewritten to `k`, the copy goes dead and FLOW deletes it. Traced:
   `(set (reg/v:SI 91) (reg/v:SI 90))` is present in .cse2 and .bp and gone
   in .flow. A dead `mode = 0;` in an EARLIER cse2 block drags
   regno_first_uid[mode] below cse_basic_block_start, the second clause
   fires, `mode` becomes canonical, the copy survives -- and flow then
   deletes the dead `mode = 0;` itself, so it costs no word. It brought in
   BOTH 0x8009DCE0 (reorg's duplicate of the copy into the delay slot) and
   0x8009DCE8 (the copy). Four placements score 268/25 and are
   byte-identical: above `k = 0;`, first statement in the loop body, before
   the `for`, and above `q = *(u8 **)(p - 0x14);`.

   (3) +1 word, -16 rows: the comma expression

       if (*(s8 *)(q + 0x26) < 0
           || (m = D_80082E80, *(s8 *)(q + 0x26) != *(s8 *)(m + 0x26)))

   THE TARGET KEEPS TWO PSEUDOS FOR &D_80082E80 -- $s2 before the loop
   (`lbu 0x24/0x25`), which then DIES so $s2 is reused for `q`, and $fp
   across the loop body for the single `lb $v0, 0x26($fp)` -- and the
   preheader copy `addu $fp, $s2, $zero` at 0x8009DC20 is loop.c hoisting
   the in-loop materialisation, which cse2 then collapses from lui+addiu to
   one copy of the pre-loop pseudo. Getting that copy is in direct tension
   with keeping func_8009FD40's own `lui $t0`/`addiu $a1` at
   0x8009DC6C/0x8009DC74, and THREE compiler conditions have to hold at
   once:

     * loop.c scan_loop's three-case gate (loop.c:794). `m` is a user
       variable, so case (2) `! REG_USERVAR_P` never applies; case (3)
       needs `! maybe_never`, i.e. the set must be at the very top of the
       loop body, BEFORE the first conditional jump; case (1)
       `reg_in_basic_block_p` needs the set to be the reg's FIRST uid and
       its last use to be in the SAME basic block.
     * move_movables' desirability (loop.c:1727): `threshold * savings *
       m->lifetime >= insn_count`. An address materialised at its point of
       use has life 1 and prints `move-insn savings 1 not desirable`.
     * cse1 must not substitute the hoisted value into the call's second
       argument. It will: -fcse-skip-blocks is on at -O2, so
       cse_end_of_basic_block follows the `||`'s `bltz` over the skipped
       block, and invalidate_skipped_block only kills regs SET in the
       SKIPPED block. Putting `m`'s set INSIDE that skipped block (which is
       exactly the `||`'s right operand) is what saves the argument.

   The comma expression satisfies all three: the set is a statement-like
   expression in the `||`'s right-operand block, so it is `m`'s first uid
   and its only use is in the same basic block (case 1), its lifetime is
   the whole-function span so it is desirable, and invalidate_skipped_block
   drops it before the argument is materialised. It is a SCAFFOLD -- the
   original presumably got the same RTL from a shape I could not find -- but
   it is byte-exact apart from the two registers.

   =====================================================================
   THE LAST 9 ROWS ARE AN ALLOCATION PRIORITY, AND I COULD NOT MOVE IT
   =====================================================================

   global.c's allocno_compare sorts by

       pri = floor_log2(n_refs) * n_refs / live_length * 10000

   and find_reg then takes the lowest-numbered free hard reg (MIPS defines
   no REG_ALLOC_ORDER). Measured from `.lreg` on this body:

       head                 (allocno 87)  refs 5, live_length 112, pri 893
       D_800E296C base copy (allocno 165) refs 5, live_length 108, pri 926
       the constant 2       (allocno 195) refs 5, live_length 102, pri 980
       &D_80082E80 high     (allocno 156) refs 3, live_length  32, pri 937

   Order 195, 156, 165, 87 -> $s4, $s4, $s5, $s6. The target needs
   195, 156, 87, 165 -> $s4, $s4, $s5, $s6, i.e. `head` must be allocated
   BEFORE the base copy, which needs pri(head) > 926, i.e. live_length
   <= 107 or n_refs >= 6.

   Both counts are depth-weighted (flow.c:2128, `reg_n_refs[regno] +=
   loop_depth`): head = set(1) + `lw 0x5C`(1) + guard `beq`(1) + bottom
   `bne`(2) = 5; the base copy = preheader set(1) + two in-loop reads(2+2)
   = 5. The base copy's live range is a SUBSET of head's (head is live from
   its load, through the five flag gates, to the loop's bottom test), so its
   live_length can never exceed head's -- 112 vs 108, four apart -- and with
   equal ref counts the base copy therefore always wins. Everything I tried
   left both at exactly 5 refs / 112 and 108 insns:

     * every placement of the dead `mode = 0;` (6 placements) ..... 269 / 9
     * every placement and every value of a dead pre-loop `m = X;` for
       X in {p, head, ot, col, (u8 *)0, g, n} at 5 positions ...... 269 / 33
       (that family is the earlier 33-row basin, before the comma)
     * every declaration position for `u8 *head;` (12) ............ 269 / 9
     * every declaration position for `u8 *m;` (6) ................ 269 / 33
     * `t` inlined as `p[0x13]` ................................... 269 / 9
     * `if (c > 0xFF)` instead of `if (c >= 0x100)` ............... 269 / 9
     * `if (*(s8 *)(p + 0x13) <= 0) continue;` guard ............... 269 / 9
     * the whole `||` rewritten as nested ifs plus a `goto` ....... 269 / 9
     * `head != p` in the loop condition .......................... 269 / 9
     * `while (p != head)` with the advance at the bottom .......... 269 / 11
     * `for (;;)` with `if (p == head) break;` at the top .......... 269 / 11
     * `while (1)` with `if (p == head) break;` at the top ......... 266 / 25
     * `if (p != head) { do { ... } while (...); }` ................ 269 / 9
     * `for (p = ...; p != head; ...)` (init in the for) ........... 270 / 11
     * `head = D_800E3D7C;` swapped above/below `g = ...` .......... 269 / 18
     * `head = D_800E3D7C;` moved after the five flag gates ....... 269 / 20
     * `head = D_800E3D7C;` moved just above `p = ...` ............ 270 / 21
     * `col[1] = 0;` moved above `p = *(u8 **)(head + 0x5C) + 0x20;` 270 / 11
     * col[] stores written ascending 0..3 ........................ 269 / 19
     * `if (p[0xAD] == 0) k = 1; else k = 2;` (arms the other way) . 269 / 14

   SO THE REMAINING LEVER IS NOT A STATEMENT ORDER: it has to change
   `head`'s ref count to 6 (one more depth-1 reference that is live at FLOW
   time and removed by COMBINE, which runs after flow and does not update
   reg_n_refs) or shorten `head`'s live range below the base copy's.
   This is exactly the shape decomp-permuter searches well.

   REJECTED WHILE HUNTING THE &D_80082E80 COPY (all with `mode = 0;` in):
     * `m = D_80082E80;` first statement in the loop body ......... 268 / 44
       (hoisted -- 0x8009DC20 MATCHES -- but cse1 eats the argument's
        lui/addiu at 0x8009DC6C, so the count does not move)
     * same with the argument written `&D_80082E80[0]` ............. 268 / 44
     * same with the argument written `n` ......................... 268 / 37
     * `m = D_80082E80;` inside `if ((D_800E296C & 4) == 0)` ...... 264 / 36
       (life 1: loop.c prints `move-insn savings 1 not desirable`)
     * `m = D_80082E80;` above `q = *(u8 **)(p - 0x14);` .......... 264 / 39
     * `m = D_80082E80;` first inside `if (p[0x13] > 0)` .......... 264 / 39
     * `n = D_80082E80;` re-set at the loop top (one variable) .... 266 / 55
     * `m = n;` at the loop top (no dead earlier set) ... 268 / 25, BYTE-
       IDENTICAL to the body without it (cse1 propagates m->n, flow deletes)
     * `m = n;` before the `for` only ............................. 268 / 34
     * `m = n;` before the `for` AND at the loop top .............. 268 / 44
     * pre-loop call written `D_80082E80[0x24], D_80082E80[0x25]` .. no change,
       BYTE-IDENTICAL
     * in-loop `*(s8 *)&D_80082E80[0x26]`, `*(s8 *)(D_80082E80 + 0x26)`,
       `((s8 *)D_80082E80)[0x26]` -- all three fold the 0x26 INTO the
       symbol, so cse1 leaves the argument alone but the movable has life 1
       and stays in the loop ..................................... 267 / 28
     * `*(s8 *)((m = D_80082E80) + 0x26)` (assignment inside the address,
       no comma) -- gcc folds `(&sym) + 0x26` to a constant and the
       separate pseudo never exists .............................. 267 / 28
     * `g` reused as the in-loop base instead of a new `m` ........ 269 / 43

   WHAT IS ESTABLISHED AND SHOULD NOT BE RE-DERIVED:

   * The second parameter is UNUSED -- $a1 is never read. func_8009DF0C
     (landed, just below) calls `func_8009DAD8(r, p)` with two arguments, so
     the definition must take two or that call stops compiling.
   * The ring walk is a `for` with the advance in the THIRD CLAUSE (the
     alternatives are measured above; `while` and `for(;;)` both cost 2 rows
     in the `continue` branch targets).
   * `g = (u8 *)&D_80083160;` is a local of its own, assigned FIRST, before
     `head` and before the five flag gates -- the `lhu 0x4($s1)` base shape,
     and swapping it with `head` costs 9 rows.
   * `head = D_800E3D7C;` is read into a local BEFORE the five flag gates.
   * `col[1] = 0;` must sit AFTER `p = *(u8 **)(head + 0x5C) + 0x20;` so
     reorg fills the guard's delay slot with it instead of stealing the
     post-loop `i = 0`.
   * The four sweeps are SUBSCRIPTED (`D_800E3548_s[i]`, `D_800E36C8[i]`,
     `D_800E3CD8[i]`, `D_800E3648_s[i]`, `D_800E39C8[i]`), the same shape
     func_8009F020/func_8009EF78 needed; the pointer-stride givs are loop.c's.
   * The D_800E3CD8 sweep passes `(u16)` casts to func_8009DA70 (`lhu`) and
     the plain `s16` fields to func_8009EE4C (`lh`).

   PERMUTER TRIAGE: counts MATCH, 9 rows, and all 9 are the same two
   registers exchanged. Ideal permuter food. */

void func_8009DAD8(u8 *ot, u8 *unused)
{
    u8 col[4];
    u8 *g;
    u8 *n;
    u8 *m;
    u8 *p;
    u8 *q;
    u8 *head;
    s32 c;
    s32 i;
    s32 t;
    s32 k;
    s32 mode;

    g = (u8 *)&D_80083160;
    head = D_800E3D7C;
    if ((D_800E296C & 0x1000) != 0)
    {
        D_800E296C &= ~0x1000;
        func_8009F13C();
    }
    if ((D_800E296C & 0x400) != 0)
    {
        D_800E296C &= ~0x400;
        func_8009F0B4();
    }
    if ((D_800E296C & 0x200) != 0)
    {
        D_800E296C &= ~0x200;
        func_8009F020();
    }
    if ((D_800E296C & 0x800) != 0)
    {
        D_800E296C &= ~0x800;
        func_8009EF78();
    }
    if ((D_800E296C & 0x100) != 0)
    {
        D_800E296C &= ~0x100;
        func_8009EF04();
    }
    c = (func_800644B8(*(u16 *)(g + 0x4) << 8) >> 6) + 0x80;
    if (c >= 0x100)
    {
        c = 0xFF;
    }
    n = D_80082E80;
    col[3] = 0x68;
    col[2] = 0;
    col[1] = c;
    col[0] = c;
    func_8009DA70(n[0x24], n[0x25], (s32 *)col, (s32)ot);
    p = *(u8 **)(head + 0x5C) + 0x20;
    col[1] = 0;
    for (; p != head; p = *(u8 **)(p + 0x5C) + 0x20)
    {
        if (*(s8 *)(p + 0x13) > 0)
        {
            q = *(u8 **)(p - 0x14);
            if ((D_800E296C & 4) == 0)
            {
                if (*(s8 *)(q + 0x26) < 0 || (m = D_80082E80, *(s8 *)(q + 0x26) != *(s8 *)(m + 0x26)))
                {
                    if (func_8009FD40(q, D_80082E80) >= 4)
                    {
                        continue;
                    }
                }
            }
            mode = 0;
            k = 0;
            t = p[0x13];
            if (t == 0x1E)
            {
                if (p[0xAD] != 0)
                {
                    k = 2;
                }
                else
                {
                    k = 1;
                }
            }
            else if (t != 0x23 || *(s16 *)(p + 0xA6) == 0 || (D_800E296C & 4) != 0)
            {
                k = 1;
            }
            mode = k;
            if (mode == 0)
            {
                continue;
            }
            if (mode == 2)
            {
                col[2] = c;
                col[0] = 0;
            }
            func_8009DA70(q[0x24], q[0x25], (s32 *)col, (s32)ot);
            if (mode == 2)
            {
                col[2] = 0;
                col[0] = c;
            }
        }
    }
    col[2] = c;
    col[0] = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if ((D_800E296C & 2) != 0 ||
                func_8009EE4C(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01) != 0)
            {
                func_8009DA70(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01, (s32 *)col, (s32)ot);
            }
        }
    }
    col[2] = 0;
    col[1] = c;
    for (i = 0; i < 4; i++)
    {
        if (D_800E3CD8[i].unk_00 != 2)
        {
            break;
        }
        if ((D_800E296C & 1) != 0 ||
            func_8009EE4C(D_800E3CD8[i].unk_02, D_800E3CD8[i].unk_04) != 0)
        {
            func_8009DA70((u16)D_800E3CD8[i].unk_02, (u16)D_800E3CD8[i].unk_04, (s32 *)col,
                          (s32)ot);
        }
    }
    col[1] = 0x20;
    col[2] = c;
    col[0] = c;
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
            (D_800E3648_s[i].unk_03 & 0x40) == 0)
        {
            if ((D_800E296C & 8) != 0 ||
                ((D_800E3648_s[i].unk_03 & 0x80) == 0 &&
                 func_8009EE4C(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07) != 0))
            {
                func_8009DA70(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, (s32 *)col, (s32)ot);
            }
        }
    }
}
