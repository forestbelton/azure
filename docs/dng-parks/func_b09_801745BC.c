/* PARK: func_b09_801745BC, csrc/ovl_dng_b09.c, agent-h11, 2026-11-23.

   TARGET 145 words. BEST BUILT 144 words, 19 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py). 2 members, 145 words each, 290 words total
   (tools/decomp/dngfam.py: func_b09_801745BC, func_b10_8017442C).

   WHAT IT DOES. Walk the actor's own circular room list (e = p + 0x5C
   chain, payloads at node + 0x20) and, for every other actor standing on
   cell (x, y) within 0x40 of height z, either knock it over -- clear its
   +0x28, lay -1 in its +0x64, func_800419EC(6, 0xC), the 0x80B/0x601 cue and
   a func_80099C58 flash whose x/y come from the eight {dx, dy} pairs at
   D_b09_80170884 indexed by (p + 0x2A >> 7) & 0x1C -- or, when its
   e[0x13] is outside 0x33..0x36, re-aim it through func_8009C12C.

   NO TWIN. xsibs.py reports no landed near-sibling at ratio >= 0.60, and no
   resident or carved unit calls func_80099C58 and func_8009C12C together
   except csrc/ovl_dungeon.c's func_8009CE1C, which is a different function
   (255 words, no loop). This is plain decompilation; the shape above is
   mine, read off the listing.

   THE WHOLE RESIDUE IS ONE MISSING REGISTER-TO-REGISTER COPY, plus the 18
   renames it causes. The target holds `p' in THREE pseudos: $s0 from $a0 at
   entry (dead at the preheader, then reused for the cursor `e'), $s2 for the
   three pre-loop loads and both loop compares, and $s1 for the four body
   uses -- preheader insns `addu $s2, $s0, $zero' then
   `addu $s1, $s2, $zero'. Every spelling I measured collapses to TWO
   pseudos and one copy, so the build is one word short and the eight
   callee-saved registers all land one place over. The exact rows are: at
   target index 47 `addu $s1, $s2, $zero' is absent; indices 1, 12, 46, 47,
   48, 49, 50, 51, 53, 54, 59, 63, 94, 97, 98, 123, 130 are the same
   instructions with $s1/$s2/$s3/$s6/$s7/$fp permuted. There is no
   instruction-selection row and no scheduling row left.

   WHAT LANDED THE REST, measured on this body:

   * THE HEIGHT TESTS MUST PUT m + 0xA ON THE LEFT.
     `*(s16 *)(m + 0xA) <= z + 0x40 && *(s16 *)(m + 0xA) > z - 0x40' keeps
     `z' itself in a callee-saved register and recomputes `z + 0x40' inside
     the loop, which is the target. Written `z + 0x40 >= *(s16 *)(m + 0xA) &&
     z - 0x40 < *(s16 *)(m + 0xA)' loop.c hoists BOTH offsets into
     callee-saved registers and `z' dies in the preheader: same 144 words,
     26 aligned differences. A `zz' local for the m + 0xA load is 140 words
     and 37.
   * THE func_80099C58 INDEX AND ITS FIFTH ARGUMENT SHARE THE SIGN
     EXTENSION. `(((s16)*(u16 *)(p + 0x2A) >> 7) & 0x1C)' for the index and
     `(s16)*(u16 *)(p + 0x2A)' for the argument give the target's one
     `lhu'/`sll 16' pair with `sra 23'/`andi 0x1C' off it and a second
     `sra 16' for the argument. Writing the index on the unsigned value,
     `(*(u16 *)(p + 0x2A) >> 7) & 0x1C', emits a separate `srl 7': 29
     aligned differences against 26.
   * func_80099C58 TAKES FIVE ARGUMENTS, as csrc/ovl_dungeon.c's
     func_8009CE1C header already records; `sw $t0, 0x10($sp)' is the fifth.
   * THE 0x80B/0x601 CUE IS A TERNARY, not a local.
     `func_800A56E0(e[0x13] == 0x34 ? 0x80B : 0x601)' is what puts the 0x601
     in the `bne's delay slot, which the target has.

   MEASURED AND REJECTED for the missing copy, all 144 words / 19 to 20
   aligned differences unless stated. The 145-word column is the interesting
   one: every shape that reaches 145 words puts the second copy in the wrong
   register.

     a local `h = p' for the loop control only (this body)            19
     a local `g = p' for the four body uses only                      20
     `g = p' assigned after the v/w loads                             20
     `g = p' as the loop body's FIRST statement (levers :29402)
                                                        141 words,    24
     `g = p' inside the then arm                                      27
     no alias at all, body and control both on `p'       141 words,    24
     `h = p' for control and `g = p' for the body        144 words,    39
     `h = p; g = h;' with the body on `g'                145 words,    44
     `g = p; h = g;' with the control on `h'             145 words,    44
     `h = p; g = p;' both from the parameter             145 words,    44
     the same with `h' declared first                    145 words,    44
     `h = p' for control, `g = p' for the body, v/w on `p'
                                                        144 words,    39
     `g = p' for the body and `g2 = p' for the else arm  145 words,    49
     `g2 = p' assigned inside the else arm                            21
     the e advance split into `e = *(u8 **)(e + 0x5C); e += 0x20;'    22
     the loop head split the same way                                 21
     `for (e = ...; e != p; e = ...)' instead of `while'               20
     `do { } while' with an `if' guard                                 20
     the four gate tests nested instead of one `&&' chain             20
     v and w loaded in the other order                                20
     v and w declared in the other order                              20
     v written inline at its one use                     141 words,    21
     w written inline at its three uses                  142 words,    52
     both written inline                                 140 words,    58
     the table copy moved below the pointer setup        145 words,    41

   WHAT I WOULD TRY NEXT. The copy is a global-alloc outcome, not a
   selection one: in the target `p' shares $s0 with the cursor `e' and the
   control alias keeps $s2, where every spelling here makes `p' BE the
   control alias and gives the cursor its own register. A permuter run on
   this body is the obvious next move -- it is one `move' away and the
   oracle is monotone. The park below is the 19-difference body.

   DECLARATIONS
   void func_800419EC(u16, u32);
   void func_80099C58(s16, s16, s16, s16, s16);
   u8 *func_8009C12C(u8 *, u8 *, s16, s16);
   END DECLARATIONS

   Everything else it needs is already in csrc/ovl_dng_b09.c's block:
   s32 func_800A56E0(s32), extern const u8 D_b09_80170884[], and the
   UnkDngB09A28 / UnkDngB09A28Tbl typedefs. */

void func_b09_801745BC(u8 *p, u16 x, u16 y, s16 z)
{
    UnkDngB09A28Tbl t;
    u8 *e;
    u8 *q;
    u8 *m;
    u8 *v;
    u8 *w;
    u8 *h;

    t = *(UnkDngB09A28Tbl *)D_b09_80170884;
    v = *(u8 **)(p - 0x14);
    w = *(u8 **)(p - 0x18);
    h = p;
    e = *(u8 **)(h + 0x5C) + 0x20;
    while (e != h)
    {
        q = *(u8 **)(e - 0x14);
        m = *(u8 **)(e - 0x18);
        if (q[0x24] == x && q[0x25] == y && *(s16 *)(m + 0xA) <= z + 0x40 &&
            *(s16 *)(m + 0xA) > z - 0x40)
        {
            if ((u32)(e[0x13] - 0x33) < 4)
            {
                e[0x28] = 0;
                *(s16 *)(e + 0x64) = -1;
                func_800419EC(6, 0xC);
                func_800A56E0(e[0x13] == 0x34 ? 0x80B : 0x601);
                func_80099C58((s16)(*(u16 *)(w + 0x2) +
                                    ((UnkDngB09A28 *)((u8 *)&t +
                                                      (((s16)*(u16 *)(p + 0x2A) >> 7) & 0x1C)))
                                            ->unk_00 *
                                        0x20),
                              (s16)(*(u16 *)(w + 0x6) +
                                    ((UnkDngB09A28 *)((u8 *)&t +
                                                      (((s16)*(u16 *)(p + 0x2A) >> 7) & 0x1C)))
                                            ->unk_02 *
                                        0x20),
                              (s16)(*(u16 *)(w + 0xA) - 0x20), *(s16 *)(p + 0x14),
                              (s16)*(u16 *)(p + 0x2A));
            }
            else
            {
                func_8009C12C(p, v, *(s16 *)(p + 0x2A), 1);
            }
        }
        e = *(u8 **)(e + 0x5C) + 0x20;
    }
}
