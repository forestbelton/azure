/* PARK: func_b28_80173A20, csrc/ovl_dng_b28.c, agent-h81, 2026-11-24.

   TARGET 176 words. BEST BUILT 176 words, 2 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports 4 differing masked, which is
   the same fault counted position-by-position. 1 member, 176 words
   (tools/decomp/dngfam.py), so there is no replication value beyond its own
   body. This function owns NO jump table: its listing has no
   `.section .rodata' block and no jtbl_ symbol, and its three-state dispatch
   is a compare chain.

   THE WHOLE RESIDUE IS ONE INSTRUCTION SCHEDULED THREE SLOTS EARLY. The
   instruction MULTISET is identical to the target's, word for word; the only
   disagreement is that `andi $v1, $v1, 0xE' -- the second half of the facing
   index -- comes out at word 84 here and at word 87 in the target:

     word   built                            target
      83    srl   $v1, $v1, 8                srl   $v1, $v1, 8
      84    andi  $v1, $v1, 0xE              sw    $v0, %lo(D_800814A0)($a0)
      85    sw    $v0, %lo(D_800814A0)($a0)  lui   $v0, %hi(D_8006CCD8)
      86    lui   $v0, %hi(D_8006CCD8)       addiu $v0, $v0, %lo(D_8006CCD8)
      87    addiu $v0, $v0, %lo(D_8006CCD8)  andi  $v1, $v1, 0xE
      88    addu  $s4, $v1, $v0              addu  $s4, $v1, $v0

   MECHANISM, as far as it was traced: this is sched1's ready-list choice, not
   allocation and not a missing statement. After the `srl', the ready insns are
   the `andi', the D_800814A0 `sw' and the `lui'. INSN_PRIORITY is the critical
   path to the end of the block, and the `andi' path (andi -> addu $s4 ->
   lh 0($s4) -> addu $a0 -> sll -> andi 0xFFC0 -> jal) is exactly ONE longer
   than the `sw' path (sw -> lh 0($s4) -> ... -> jal, through the memory
   dependence on the *t1 load), so the `andi' always wins the tie here. The
   target picked the `sw'. Nothing reachable from the C changes that ranking:
   fourteen spellings were tried (table below) and every one that kept the
   word count landed on either 2 or 4.

   RELOCATIONS AUDITED. The listing's complete symbol set is five `%hi'/`%lo'
   pairs -- D_8006CCD8, D_8006CCE8, D_800814A0, D_8008346C and
   func_b28_80170E5C -- and ten `jal' targets: func_80042640, func_800A2B04,
   func_800A56E0, func_800A6D30, func_800AD594, func_800BCB04, func_800D3F74,
   func_b28_80174084, func_b28_801742AC and func_b28_80174460. The body below
   spells exactly those and no others; tools/decomp/relcand.py agrees.
   `--raw' is 26 differing and all 26 are those relocations.

   WHAT IT IS. The actor's +0x9B three-state hand-off for the 0x80C cue.
   State 0 either arms c + 0x14's 0x6000 and jumps to state 2 or steps to
   state 1; state 1, on the `(c[4] == 8 && 0x1000) || 0xE000' gate, shows the
   bank-local message through func_b28_80174460, rings func_800A56E0(0x80C),
   runs func_b28_80174084 and parks func_b28_801742AC's result in p + 0xA4;
   state 2 raises 0x8000 on that record's +0x1E and in D_800814A0, spawns a
   func_800D3F74 node on the cell the D_8006CCD8/D_8006CCE8 facing pair picks,
   dresses it through func_80042640 and a clamped +0x28/+0x29 pair, and hands
   the actor back to func_b28_80170E5C.

   NO TWIN OF ANY KIND. dngtwin finds no carved sibling at 0.70 or better and
   dngfam gives one member. The callee-set intersection on func_800D3F74 /
   func_80042640 / D_800814A0 over csrc/ovl_dungeon*.c and csrc/ovl_dng_*.c
   returns no landed body that calls func_800D3F74 at all -- the only landed
   definition is func_800D3F74 itself in csrc/ovl_dungeon_800CABA8.c:5413.
   The whole body below was derived from the listing. Its tail (func_800AD594,
   func_800A2B04, the func_b28_80170E5C install, D_8008346C = 0 and the
   d + 0x46 0x7FFF mask) is the same teardown as the landed
   func_b28_801728B4 in the same unit, whose comment carries its own measured
   statement-order fact for it.

   MEASURED AND LOAD-BEARING. Every row is aligndiff on this body; the
   starting point was 177 words / 42 aligned differences and each row below
   is the step that spelling was worth. Read top to bottom as a sequence of
   independent facts, each measured against the body as it now stands.

   | spelling                                        | words | aligned diffs |
   | as written below                                |  176  |      2        |
   |                                                 |       |               |
   | `s16 *t1' DECLARED BEFORE `s16 *t2'             |  177  |     39->42    |
   |   The two pointers hold each other's            |       |               |
   |   callee-saved register ($s0/$s4 exchanged at   |       |               |
   |   both of their two reads each). Declaring t2   |       |               |
   |   first is what gives t1 $s4, as the target     |       |               |
   |   has. Equal refs and near-equal live length,   |       |               |
   |   so the tie falls to the allocno number.       |       |               |
   |                                                 |       |               |
   | the two func_800BCB04 arguments written inline  |  175  |     34->25    |
   |   instead of through the `x1'/`x2' locals       |       |               |
   |   (9 rows): with locals the source order is     |       |               |
   |   t1, x1, t2, x2 and sched keeps it, which is   |       |               |
   |   the target's interleave; inline, both table   |       |               |
   |   bases are materialised adjacently.            |       |               |
   |                                                 |       |               |
   | the third argument inline, `*(u16 *)(d + 0x88)  |  175  |     25->17    |
   |   - 0x20', instead of the `x3' local placed     |       |               |
   |   BEFORE the D_800814A0 store (8 rows). The     |       |               |
   |   target reads d + 0x88 at word 81, above the   |       |               |
   |   store at 84, and a load cannot cross a store  |       |               |
   |   to a fixed scalar unless the source already   |       |               |
   |   has it earlier. `x3' must hold the RAW load   |       |               |
   |   with the `- 0x20' left at the call site:      |       |               |
   |   `x3 = ... - 0x20' as the local is 19.         |       |               |
   |                                                 |       |               |
   | the third d + 0x88 read left INSIDE the `h ='   |  175  |     17->12    |
   |   arm instead of hoisted into `hx' before the   |       |               |
   |   `if' (5 rows): the target's `lhu $v0' sits    |       |               |
   |   above the `beqz' and the arm is the copy      |       |               |
   |   `addu $a3, $v0, $zero'.                       |       |               |
   |                                                 |       |               |
   | the clamp as one pseudo, `if (t >= 0x100)       |  175  |     12->4     |
   |   t = 0xFF;' (8 rows AND the missing word):     |       |               |
   |   the target's `addu $v0, $v1, $zero' is the    |       |               |
   |   copy `r = t', and it only survives when `r'   |       |               |
   |   is a SUB-WORD local -- cse's make_regs_eqv    |       |               |
   |   never equates a subreg set, so with `u8 r'    |       |               |
   |   the copy is invisible to cse and is not       |       |               |
   |   folded. With `s32 r' the copy is deleted and  |       |               |
   |   r and t share $v1: 175 words, 12.             |       |               |
   |                                                 |       |               |
   | the D_800814A0 read-modify-write left as        |  176  |      4->2     |
   |   `D_800814A0 |= 0x8000;' instead of split      |       |               |
   |   into `w = D_800814A0; ...; D_800814A0 =       |       |               |
   |   w | 0x8000;' with the two d loads BETWEEN     |       |               |
   |   the halves (2 rows): the target's `lw' comes  |       |               |
   |   out before the d + 0x2A load and its `ori'/   |       |               |
   |   `sw' after both d loads, which only the       |       |               |
   |   split form gives.                             |       |               |
   |                                                 |       |               |
   | `(*(s32 *)(d + 0x1C) & 0x2000) != 0' as         |  175  |      2 extra  |
   |   func_800D3F74's first argument instead of     |       |               |
   |   `> 0': do_store_flag's single-bit case        |       |               |
   |   rewrites the `!= 0' form to `srl 13'/`andi 1' |       |               |
   |   where the target has `andi 0x2000'/`sltu'.    |       |               |
   |   `> 0' takes the plain NE expansion.           |       |               |
   |                                                 |       |               |
   | `((c[0x24] + *t1) << 6) & 0xFFC0' written       |  177  |     +0 words  |
   |   through a `u16'-parameter alias of            |       |  lh -> lhu    |
   |   func_800BCB04 instead of the explicit mask:   |       |               |
   |   the narrowing makes combine turn the table's  |       |               |
   |   `lh' into `lhu', which the target has only at |       |               |
   |   the SECOND call. The explicit `& 0xFFC0' with |       |               |
   |   the unit's plain `s32 func_800BCB04' keeps    |       |               |
   |   `lh' at the first call and `lhu' at the       |       |               |
   |   second (there the `s16' parameter truncates). |       |               |

   MEASURED AND REJECTED, each on the body as written below unless stated:

   | spelling                                              | words | diffs |
   | `k' as s16, u16 or u8 instead of s32                  |  176  |   2   |
   | the index as a byte offset, `k = (x >> 8) & 0xE' with |  176  |   2   |
   |   `(s16 *)((u8 *)D_8006CCD8 + k)'                     |       |       |
   | `kk' holding the raw d + 0x2A load with the index     |  176  |   2   |
   |   inline in both array refs                           |       |       |
   | the same, with `k = (kk >> 9) & 7' after the store    |  176  |   2   |
   | the split store moved after `t1 = &D_8006CCD8[k]'     |  176  |   2   |
   | `if (1)' around the split store                       |  176  |   2   |
   | `x3' assigned BEFORE `k'                              |  176  |   4   |
   | `if (1) { D_800814A0 |= 0x8000; }', unsplit           |  176  |   4   |
   | `do { D_800814A0 |= 0x8000; } while (0);', unsplit    |  178  |  19   |
   | the split store moved after `x1'                      |  176  |  19   |
   | `w = D_800814A0' read before the e + 0x1E update      |  174  |  24   |
   | the e + 0x1E update moved after `k' and `x3'          |  174  |  28   |
   | `t2 = &D_8006CCE8[k]' before the store                |  174  |  28   |
   | the clamp as `if (t >= 0x100) { q[0x28] = 0xFF;       |  177  |  30   |
   |   q[0x29] = 0xFF; } else { q[0x28] = t; q[0x29] = t;}'|       |       |
   | the clamp into `h' (reused, multi-set) instead of     |  176  |  10   |
   |   `u8 r' -- right SHAPE, wrong register: the copy     |       |       |
   |   appears but the value takes $a3, h's argument tie,  |       |       |
   |   where the target has $v0                            |       |       |
   | the clamp into `x1', `x2' (reused)                    |  175  |  12   |
   | the clamp into `hx' (reused)                          |  176  |  15   |
   | the clamp into `k' (reused)                           |  177  |  25   |
   | `h' holding the sum and `t' the clamped value         |  175  |  17   |
   | `r = t; if (t > 0xFF) r = 0xFF;' with `s32 r'         |  175  |  17   |
   | the two addends swapped, `d[0x11] * 2 + rand +        |  175  |  16   |
   |   q[0x29]'                                            |       |       |
   | `hh' holding `*(u16 *)(d + 0x88) - 0x20' before the   |  175  |  37   |
   |   store (the pre-x1/x2 shape)                         |       |       |

   WHAT TO TRY NEXT, in order:
   1. The residue is ONE sched1 tie. The thing that would settle it is a
      `-da' dump pair: `.sched'/`.sched2' would print INSN_PRIORITY for the
      `andi' and the `sw' and say whether the target's ranking is reachable at
      all from this basic block's dependence graph. That was not run here.
   2. If it is not reachable, the fault is upstream: the block's dependence
      graph differs because ONE memory dependence differs. The candidate is
      the `*t1' load's dependence on the D_800814A0 store. Breaking it would
      shorten the `sw' path and could flip the tie -- which is the
      MEM_IN_STRUCT_P lever (docs/matching-levers.md) used in the HOISTING
      direction: a struct-typed access for the table load, so
      fixed_scalar_and_varying_struct_p proves it independent of the fixed
      scalar D_800814A0. No struct type for the facing tables exists in this
      unit's context, so that was not tried.
   3. A permuter run on this body alone. It is 2 rows from landing and the
      whole fault is in one 10-word window.

   DECLARATIONS
   u8 *func_800D3F74(s16, s16, s16, s16);
   void func_80042640(Unk42710 *, s16);
   void func_b28_80174460(u8 *p);
   void func_b28_80174084(u8 *, UnkVec *, u8 *);
   u8 *func_b28_801742AC(u8 *, UnkVec *, u8 *);
   END DECLARATIONS

   These five are the only declarations this body adds to ovl_dng_b28.c as it
   stands after agent-h81's three landings; everything else it names
   (func_800A56E0, func_800AD594, func_800A2B04, func_800A6D30,
   func_800BCB04, D_8006CCD8, D_8006CCE8, D_8008346C, D_800814A0 and
   func_b28_80170E5C) is already declared in that unit or comes from
   common.h. Unk42710 is include/structs.h:644.
*/
void func_b28_80173A20(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *q;
    s16 *t2;
    s16 *t1;
    s32 h;
    s32 t;
    s32 k;
    s32 hx;
    s32 w;
    u8 r;
    s32 x3;
    s32 x1;
    s32 x2;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(u16 *)(c + 0x14) |= 0x6000;
            p[0x9B] = 2;
            return;
        }
        p[0x9B] = 1;
    case 1:
        if ((*(s8 *)(c + 0x4) == 8 && (*(u16 *)(c + 0x14) & 0x1000) != 0) ||
            (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_b28_80174460(d);
            func_800A56E0(0x80C);
            func_b28_80174084(p, b, c);
            *(u8 **)(p + 0xA4) = func_b28_801742AC(p, b, c);
            p[0x9B] += 1;
        }
        return;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            return;
        }
        e = *(u8 **)(p + 0xA4);
        *(u16 *)(e + 0x1E) |= 0x8000;
        w = D_800814A0;
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        x3 = *(u16 *)(d + 0x88);
        D_800814A0 = w | 0x8000;
        t1 = &D_8006CCD8[k];
        x1 = ((c[0x24] + *t1) << 6) & 0xFFC0;
        t2 = &D_8006CCE8[k];
        x2 = ((c[0x25] + *t2) << 6) & 0xFFC0;
        h = func_800BCB04(x1, x2, x3 - 0x20);
        hx = *(u16 *)(d + 0x88);
        if (*(s16 *)(d + 0x88) < (s16)h)
        {
            h = hx;
        }
        q = func_800D3F74((*(s32 *)(d + 0x1C) & 0x2000) > 0, c[0x24] + *t1, c[0x25] + *t2, h);
        func_80042640((Unk42710 *)q, 0x36);
        t = q[0x29] + (d[0x11] * 2 + (func_800A6D30() & 3));
        r = t;
        if (t >= 0x100)
        {
            r = 0xFF;
        }
        q[0x28] = r;
        q[0x29] = r;
        func_800AD594(d, 0x400);
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u32 *)(p + 0x8C) = (u32)func_b28_80170E5C;
        D_8008346C = 0;
        *(u16 *)(d + 0x46) &= 0x7FFF;
        return;
    }
}
