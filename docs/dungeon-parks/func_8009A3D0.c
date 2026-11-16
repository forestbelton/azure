/* func_8009A3D0 (ovl_dungeon) -- PARK, best so far.

   target 92 words, candidate 91 words, 77 differing (masked).
   The residue is ONE missing word plus a rotation of the five callee-saved
   registers.  Everything structural is settled and measured below.

   WHAT THE FUNCTION IS.  (x, y, flags) over the 6-byte map cells at
   `*(s32 *)(&D_80083160 + 0x1DC)`, row shift at +0x1F0.  `flags & 0x8832`
   non-zero CLEARS flags out of the cell's +0x04 halfword -- and when
   `flags & 0x800` only if func_800A6E10(x, y) < 2.  Otherwise it clears them
   AND ors back the saturated `(cell & flags) - (flags & 0x1100)`.

   ================================================================
   LEVER 1, worth 13 words: THE ROW SHIFT MUST BE A STRUCT MEMBER
   ================================================================
   `cse.c`'s invalidate_memory drops an entry only on
   `all || (nonscalar && p->in_struct) || cse_rtx_addr_varies_p (p->exp)`.
   The halfword store `*(u16 *)(c + 0x4) = ...` has a PLUS address, so
   note_mem_written sets only `nonscalar`; and the shift's own address folds
   to a constant, so `cse_rtx_addr_varies_p` is 0.  Read through a `u8 *` its
   in_struct is clear, the entry SURVIVES the store, cse keeps the whole cell
   index in a register and the SECOND cell access disappears entirely:
     *(s16 *)(w + 0x14), u8 *w ............ 78 words, 88 differing
     ((UnkDungeonMapHdr *)w)->unk_14 ...... 88 words, 86 differing
   `*(volatile s16 *)(w + 0x14)` also reaches 88, which is what proves the
   mechanism is the reload and not the struct type.
   (The same lever landed func_8008DBE8 -- see docs/dungeon-parks for its
   write-up, where it is worth 4 differing on a divide.)

   ===========================================================
   LEVER 2, worth 2 words: THE CALL RESULT NEEDS AN s32 + CAST
   ===========================================================
   func_800A6E10 is landed as `s16`, so `if (func_800A6E10(a, b) >= 2)` emits
   NO sign extension at all -- gcc trusts the SImode-promoted return.
   `s32 r = func_800A6E10(a, b); if ((s16)r >= 2)` is what puts the target's
   `sll 16`/`sra 16` back in front of the `slti`.

   =====================================================
   LEVER 3, worth 1 word: THE CLAMP IS A TWO-ARM if/else
   =====================================================
   `if ((s16)n < 0) m = 0; else m = n;` gives the target's
   `addu $a2, $a0, $zero` copy; the one-armed `if ((s16)n < 0) n = 0;` does
   not and is 90 words.

   OTHER SPELLINGS MEASURED AND REJECTED (all on top of lever 1):
     `n = ((s16)n < 0) ? 0 : n;` .................. 79 words (pre-struct base)
     duplicating `*(u16 *)(c + 0x4) &= ~f;` per arm  98 words, 90 differing
       -- cross-jumping does NOT merge the two tails here, because the two
       arms' registers already differ
     a second pointer local for the second access .. no change
     f declared u16/u32/s16 ....................... conflicting types; the
       unit declares `void func_8009A3D0(s32, s32, s32);` and the three
       parameters' widths are therefore fixed
     x/y declared s16 instead of (s16) casts ...... identical allocation

   ==========================================================
   THE RESIDUE IS AN ALLOCATION ONE, AND IT IS QUANTIFIED
   ==========================================================
     target: $s0=flags  $s1=(s16)y  $s2=(s16)x  $s3=cellbase  $s4=&hdr
     here:   $s0=(s16)y $s1=(s16)x  $s2=cellbase $s3=flags    $s4=&hdr
   i.e. `flags` is LAST of the five instead of FIRST, and the other four keep
   their relative order.  `flags` is a parameter pseudo, so
   update_equiv_regs doubles its live_length (local-alloc.c:1094) and
   global.c's allocno_compare divides by it.  `.greg`'s own priority line,
   with pseudo 82 = flags, measured by adding N zero-instruction
   `__asm__("" ::"r"(f))` references purely as a DIAGNOSTIC:

     | extra refs on flags | priority rank | flags gets | differing |
     |---|---|---|---|
     | 0 (the saved body)  | 10 of 11 | $s3 | 77 |
     | 1                   |  7 of 11 | $s2 | 75 |
     | 3                   |  -       | $s1 | 74 |
     | 6                   |  -       | $s0 | 69 |
     | `q = f;` local only |  8 of 12 | $s2 | 82 |
     | `q = f;` + 3        |  -       | $s0 | 71 |

   So SIX extra references are needed to take $s0, which is far more than any
   legal spelling can supply, and -- the part worth knowing -- EVEN WITH THE
   TARGET'S EXACT CALLEE-SAVED ASSIGNMENT the function is still 91 words and
   32 ALIGNED differences, because the caller-saved temporaries in the two
   arms are then allocated the other way round.  So the callee-saved rotation
   is NOT the whole residue and chasing it alone cannot close this function.

   The one missing word: with these registers the then-arm's
   `sh <reg>, 0x4(<reg>)` and the else arm's final store are the SAME insn,
   so jump2 cross-jumps them (`j` + `and` in the delay slot, 2 words, against
   the target's `and` / `j` / `sh`, 3).  The target's two stores use opposite
   register pairs (`sh $v1, 0x4($v0)` against `sh $v0, 0x4($v1)`) and the
   merge cannot fire.

   Also still wrong: the `beqz $v0, .L8009A454` delay slot holds a COPY of the
   branch target's first `sll` (reorg did not own the taken thread) where the
   target MOVES the fall-through's first `sll` into it.

   PERMUTER TRIAGE: the word counts differ by one and the residue is a
   register permutation plus one cross-jump, which docs/dungeon-sweep.md
   calls NOT permuter food.  But the one-word deficit is inside a single
   4-insn block whose structure is proved, which is the shape the rule says
   DOES win, so it is worth one short search.

   NEEDS the typedef below in the declaration block. */

typedef struct UnkDungeonMapHdr
{
    u8 *unk_00;
    u8 pad_04[0x10];
    s16 unk_14;
} UnkDungeonMapHdr;

void func_8009A3D0(s32 x, s32 y, s32 f)
{
    u8 *v;
    UnkDungeonMapHdr *w;
    s32 t;
    u8 *c;

    v = (u8 *)&D_80083160;
    w = (UnkDungeonMapHdr *)(v + 0x1DC);
    t = *(s32 *)(v + 0x1DC);
    if (f & 0x8832)
    {
        if (f & 0x800)
        {
            s32 a;
            s32 b;
            s32 r;

            a = (s16)x;
            b = (s16)y;
            r = func_800A6E10(a, b);
            if ((s16)r >= 2)
            {
                return;
            }
            c = (u8 *)((a + (b << w->unk_14)) * 6 + t);
        }
        else
        {
            c = (u8 *)(((s16)x + ((s16)y << w->unk_14)) * 6 + t);
        }
        *(u16 *)(c + 0x4) &= ~f;
    }
    else
    {
        s32 px;
        s32 py;
        u16 o;
        s32 n;
        s32 m;

        px = (s16)x;
        py = (s16)y;
        c = (u8 *)((px + (py << w->unk_14)) * 6 + t);
        o = *(u16 *)(c + 0x4);
        n = (o & f) - (f & 0x1100);
        *(u16 *)(c + 0x4) = o & ~f;
        if ((s16)n < 0)
        {
            m = 0;
        }
        else
        {
            m = n;
        }
        c = (u8 *)((px + (py << w->unk_14)) * 6 + t);
        *(u16 *)(c + 0x4) |= m;
    }
}
