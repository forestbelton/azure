/* PARK: func_800A31D0, 52 of 53 words, 21 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  Walks the engine's node list and, for
   every kind-0x1E node whose +0xA4 owner is this record, bumps two counters
   and sets a request bit.

   NO NEW DECLARATIONS.  D_800E3D7C, D_80083460 and D_800DCE68 are all already
   in the unit, and the function has no forward declaration or call site to
   disturb.  csrc/ovl_dungeon.c's func_80099F04 and func_80099F70 (~line 9400)
   are the landed templates for the list walk itself, including the note that
   the head is re-read each turn with the `%hi` held across the loop.

   ------------------------------------------------------------------ residue

   ONE MISSING WORD, and it is an UNFILLED LOAD-DELAY SLOT.  At 0x800A325C the
   target has a bare `nop` after `lhu $v0, 0xA($a3)`; this body fills it with
   the `lhu` of D_800DCE68.  Everything else is register names that follow
   from the one-word shift.

   The mechanism is alias analysis, and it is why no spelling here reaches it.
   For the slot to stay empty, NOTHING independent may be available at that
   point -- in particular the D_800DCE68 load and the `lw 0x1C($a1)` must both
   be unable to move above the `sh $v0, 0xA($a3)` that follows.  In the target
   they cannot, which means the store's base is opaque to gcc.  Here
   `g = (u8 *)&D_80083460;` is propagated back into the MEM, so the store has
   a known base object, D_800DCE68 is a different known object, and the
   scheduler is free to hoist its load into the slot.

   Measured and rejected:
     * `__asm__("" : "+r"(g));` after the assignment -- it DOES make g opaque,
       but costs two words materialising the address: 54 words, 25.
     * `*(volatile u16 *)(g + 0xA) += 1;` -- 52 words, 21, no change.
     * D_800DCE68 reached through its own `s16 *` pointer local -- 52/21.
     * the two increments swapped -- 52 words, 33.
     * the n-stores moved ahead of both increments -- 52/30; interleaved
       between them -- 54/19 and 55/27.
     * `g` pointing straight at +0xA with `*g += 1` -- 51 words, 27.
     * the three guard conditions as early returns instead of an `&&` chain,
       and `*(s8 *)(n + 0x6D) = 0` instead of `n[0x6D] = 0` -- both 52/21,
       byte-identical.

   A candidate one word short, where the missing word is a specific nop in a
   specific block and everything else matches, is exactly the shape the
   permuter closes -- see docs/dungeon-sweep.md's triage rule.  QUEUE IT.

   -------------------------------------------------------------- what paid

   51/28 -> 52/21: `g = (u8 *)&D_80083460;` assigned INSIDE the loop body,
   immediately before its use, rather than before the loop.  Worth 1 word.
   Before the `if` it is 51/28, after `d = c - 0x20;` 51/28, after the `n`
   initialisation 51/28 -- all three hoist the `lui` out of the branch delay
   slot the target fills it from. */

void func_800A31D0(u8 *c)
{
    u8 *g;
    u8 *d;
    u8 *n;

    if (c[0x13] == 0x1E && (*(s32 *)(c + 0x14) & 0x4000) != 0 && *(s16 *)(c + 0xAA) != 0)
    {
        d = c - 0x20;
        n = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
        while (n != D_800E3D7C)
        {
            if (n[0x13] == 0x1E && *(u8 **)(n + 0xA4) == d)
            {
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                D_800DCE68 += 1;
                *(s16 *)(n + 0x6E) = 0;
                n[0x6D] = 0;
                *(s32 *)(n + 0x1C) |= 0x400000;
            }
            n = *(u8 **)(n + 0x5C) + 0x20;
        }
    }
}
