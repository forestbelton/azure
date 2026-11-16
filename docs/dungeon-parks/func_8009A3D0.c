/* func_8009A3D0 (ovl_dungeon) -- PARK, best so far.

   target 92 words, candidate 91 words, 77 differing (masked).
   aligndiff: see below.  The residue is ONE missing word plus a rotation of
   the five callee-saved registers.

   WHAT THE FUNCTION IS.  (x, y, flags) over the 6-byte map cells at
   `*(s32 *)(&D_80083160 + 0x1DC)`, row shift at +0x1F0.  `flags & 0x8832`
   non-zero clears `flags` out of the cell's +0x04 halfword (and when
   `flags & 0x800` only if func_800A6E10(x, y) < 2); otherwise it clears them
   AND ors back the saturated `(cell & flags) - (flags & 0x1100)`.

   THE ONE MEASURED MECHANISM, worth 13 words: the row shift must be read
   through a STRUCT MEMBER, not `*(s16 *)(w + 0x14)` off a `u8 *`.
   `cse.c`'s invalidate_memory drops an entry only on
   `all || (nonscalar && in_struct) || cse_rtx_addr_varies_p (exp)`, and the
   halfword store `*(u16 *)(c + 0x4) = ...` is a PLUS address, so it sets only
   `nonscalar`.  With the cast spelling the shift's MEM has in_struct clear
   and a constant-foldable base, so it survives the store, cse keeps the whole
   index in a register and the second cell access vanishes:
     *(s16 *)(w + 0x14), u8 *w ............ 78 words, 88 differing
     ((UnkDungeonMapHdr *)w)->unk_14 ...... 88 words, 86 differing
   (`*(volatile s16 *)(w + 0x14)` also reaches 88 -- same mechanism, confirms
   it is the reload and not the struct type.)

   OTHER SPELLINGS MEASURED, all on top of the struct read:
     one-armed `if ((s16)n < 0) n = 0;` ........... 90 words (no $a2 copy)
     `if ((s16)n < 0) m = 0; else m = n;` ......... 91 words, 77 differing  <-- best
     `n = ((s16)n < 0) ? 0 : n;` .................. 79 words (pre-struct base)
     duplicating `*(u16 *)(c+4) &= ~f;` per arm ... 98 words, 90 differing
     `g = f;` local ahead of the first branch ..... 91 words, 82 differing
     second cell access through a second pointer .. no change
     f declared u16/u32/s16 ....................... conflicting types (the
       unit already declares `void func_8009A3D0(s32, s32, s32);` at :609)

   THE RETURN-VALUE EXTENSION IS WORTH 2 WORDS.  func_800A6E10 is landed as
   `s16`, so `if (func_800A6E10(a, b) >= 2)` emits no sll/sra at all (gcc
   trusts the SImode-promoted return).  `s32 r = func_800A6E10(a, b);
   if ((s16)r >= 2)` is what puts the target's `sll 16`/`sra 16` back.

   WHAT IS LEFT, and it is an ALLOCATION residue, not a shape one:
     target: $s0=flags  $s1=(s16)y  $s2=(s16)x  $s3=cellbase  $s4=&hdr
     here:   $s0=(s16)y $s1=(s16)x  $s2=cellbase $s3=flags    $s4=&hdr
   i.e. `flags` is LAST of the five instead of FIRST.  `.greg` says
   `;; 11 regs to allocate: 86 135 80 91 81 90 85 132 131 82 84` with 82 =
   flags at rank 10 of 11.  flags is a parameter pseudo, so
   update_equiv_regs doubles its live_length (local-alloc.c:1094) and
   global.c's allocno_compare divides by it: 2*7/L_f against 1*3/L_b for the
   two short extension pseudos.  Copying it into a local before the first
   branch (`g = f;`) halves the live length and moves it from rank 10 to rank
   8 -- $s2 instead of $s3 -- but not past the two extension pseudos.  An
   EIGHTH reference on flags would cross the floor_log2 step (2*7 -> 3*8) and
   is the thing to look for; no legal spelling found that adds an insn
   mentioning it which survives flow.

   The one missing word is a consequence of the rotation: with these registers
   the then-arm's `sh $v0, 0x4($a1)` and the else arm's final store are the
   SAME insn, so jump2 cross-jumps them (`j` + `and` in the delay slot, 2
   words, against the target's `and` / `j` / `sh`, 3).  With the target's
   registers the two stores differ and the merge cannot fire.

   Also still wrong: the `beqz $v0, .L8009A454` delay slot holds a COPY of the
   branch target's first `sll` (reorg did not own the taken thread) where the
   target MOVES the fall-through's first `sll` into it -- one extra word here,
   which is why the count is 91 and not 90.

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
