/* PARK -- func_s63_80024648, 279 words target, 279 words built, 12 aligned
   differences. Batch 33, agent h142. ovl_dng_s63; one-member family, so
   landing this lands nothing else, but func_s63_8002401C (LANDED) installs it
   as the +0x10 step of the 0x302 node it spawns in state 1, so the signature
   here is fixed by that call site.

   WHAT IT DOES. The 0x302 node's per-frame step. Bumps the +0x48 frame
   counter, sets bit 0x8000 in the +0x52 halfword of the record at the actor's
   +0x0, then runs a FOUR-CASE COMPARE CHAIN on *(s16 *)(p + 0x4C) -- no
   .rodata block, no jump table, this function owns none. State 0 seeds the
   GetTPage/GetClut pair into +0x50/+0x52, the 0x1F pair into +0x44/+0x46,
   clears the counter, sets +0x4E to 0x30, advances, and FALLS THROUGH into
   state 1. State 1 drifts the +0x1C/+0x20/+0x24 position triple one 0x4000th
   of the way toward the party record D_80083780 offset by func_8003DE58s
   projection of D_80082E80s slot, pulls +0x24 down by
   func_800644B8((counter << 11) / 10) << 9, and at counter 0xA arms a 0x10
   countdown and advances. State 2 is the same drift at 0x8000, clamps the
   counter back to 8 past 0xD, bleeds the countdown and advances when it and
   the counter agree. State 3 tears the actor down at counter 0xF. After the
   chain it publishes the counters low two bits and its signed quarter into
   +0x40/+0x42 and spawns `c + 1` particle nodes, c being the per-state count
   (-1, 0xE, 9 or 4, 5) the chain left behind.

   NO TWIN AT ANY RATIO -- dngtwin reports nothing at 0.60 for any stub in
   this unit, and LEADS.txt is empty for it. CALLEE.txt gave func_800D7A78 at
   8 of 13, which is NOT this shape. What this was written from instead:
   csrc/ovl_dng_s69.c func_s69_800245A0 (271 words), which the callee-set grep
   over func_8003DE58 + D_80083780 + D_80082E80 finds, and whose measured
   table is worth reading in full -- six of its rows reproduced here without
   re-measuring and its `sl = D_80082E80; ... g = D_80083780;` pointer-local
   idiom is the whole of the drift blocks addressing. The spawn loops
   skeleton is csrc/ovl_dng_s10.c func_s10_80024224s family.

   THE WHOLE RESIDUE IS ONE WORD IN THE SPAWN LOOPS LAST TWO STATEMENTS, and
   it is a FOLD, not an allocation. The target ends each iteration with

       lw    $v0, 0x0($s2)
       nop
       sw    $v0, 0x20($s1)
       addiu $v0, $s1, 0x20
       sh    $zero, 0x4C($v0)

   -- the payload base n+0x20 materialised into a register and then a 0x4C
   displacement off it. Every spelling of that store I tried lets gcc fold the
   two constants into one `sh $zero, 0x6C($s1)`, which is one word short, and
   the word the body is missing then displaces the four-insn
   ori/sh/lw/ori block above it. MEASURED AND REJECTED, all against the
   279-word shape unless the words column moves:

     | spelling                                              | words | rows |
     | the 0x4C store through `e` FIRST, then the 0x20 store  |  279  |  12  |
     |   (as written -- the only 279-word shape found)        |       |      |
     | 0x20 store first, then `e` and the 0x4C store          |  277  |  10  |
     | both stores through `e`, 0x20 first                    |  278  |  15  |
     | `*(s16 *)(n->payload + 0x4C) = 0`                      |  277  |  10  |
     | `*(s16 *)((u8 *)(n + 1) + 0x4C) = 0`                   |  277  |  10  |
     | a do-while-0 round the 0x4C store (DEVICE, rejected)   |  278  |   9  |
     | the payload pointer shared with case 1s `t`            |  278  |  23  |

   NAMED NEXT STEP. cse canonicalises a `reg + const` address back to its
   base whenever the local has one or two uses, and combine then merges the
   `addiu` into the MEM. func_s63_8002401Cs case 1 -- LANDED in the same
   unit -- keeps the identical `addiu $a0, $s0, 0x20` because its payload
   pointer has THREE uses at +0x1C/+0x20/+0x24. So the question for the next
   agent is not how to spell this store but WHAT ELSE in the target reaches
   the payload through that register: find a third reference and the word
   appears. The two candidates I could not place are the `sw $s5, 0x10($s1)`
   and `lw $s0, 0xC($s1)` node-field accesses, which the target writes off the
   NODE base, not the payload base.

   SECOND, SMALLER FAULT, probably the same one. The 0x808080 constants `ori`
   half sits at index 256 in the target, filling the `lw $v0, 0x4($s4)`
   load-delay slot, and at 239 in this body with a `nop` left at 260. Moving
   the `*(s32 *)(q + 0xC) = 0x808080` store after the +0x8 store cures that
   and costs the word instead -- 278 words / 10 rows -- so the two are
   coupled through the same slot. Do not treat them as independent.

   MEASURED AND LOAD-BEARING, all from this body:

     | fact                                                  | words | rows |
     | SEPARATE pointer locals per switch arm for both       |  279  |  12  |
     |   D_80082E80 and D_80083780 (as written)              |       |      |
     |   ... one `r` and one `t` shared across states 1+2    |  278  |  23  |
     | the (counter << 11) / 10 division written INLINE      |  279  |  12  |
     |   ... hoisted into its own `s32` local first          |  278  |  23  |
     | state 2s count gate as `>= 0xB -> 4 else 9`           |  279  |  12  |
     |   ... as `< 0xB -> 9 else 4`                          |  278  |  35  |
     | the two `sb $zero` at q+0x4 and q+0x5 as TWO          |  279  |  12  |
     |   statements in ascending order                       |       |      |
     |   ... as one chained assignment                       |  279  |  13  |

   WHY THE PER-ARM LOCALS MATTER, which is the reusable finding. One local
   shared between two switch arms is referenced from two basic blocks, so
   local-alloc hands it to global-alloc, the `(high sym)` pseudo is no longer
   tied to the `(lo_sum)` that consumes it, and the address comes out as
   `lui $v0, %hi(X)` / `addiu $a1, $v0, %lo(X)` where the target has
   `lui $a1` / `addiu $a1, $a1`. Two locals per symbol, one per arm, ties each
   pair inside its own block. This is the register-count half of the
   declared-size lever in azure-addressing-shapes, reached from the C side
   rather than the declaration side, and it is live at -G0 where the
   symbolic-form half is not. A SIZED ALIAS was tried first and changes
   nothing -- `extern u8 D_80082E80_s __asm__("D_80082E80")` measures
   identically to `extern u8 D_80082E80[]`.

   THE FRAME IS -0x38 WITH s0..s5 AND ra, and `out` has FOUR elements so that
   the four-word outgoing-argument area leaves it at sp+0x10 with the saves
   starting at sp+0x18. Every read of `out` in the listing is `lh`, so it is
   `s16` here and takes no cast at the func_8003DE58 call -- the opposite of
   ovl_dng_s69, whose arrays are `u16` and whose calls need `(s16 *)`.

   DECLARATIONS
   s32 func_800644B8(s32);
   UnkNode *func_8003FD64(s32, void *);
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   s32 func_8004491C(UnkNode *, u32);
   void func_s63_80024AA4(u8 *, UnkVec *, u8 *, u8 *);
   extern u8 D_80083498[];
   extern u8 D_80082E80[];
   extern u8 D_800DEC00[];
   extern u8 D_80045340[];
   END DECLARATIONS

   D_80083780, D_800814A0 and UnkNode need no declaration; all three are in
   include/structs.h. GetTPage, GetClut and rand come from common.h -- the
   listings func_80066460, func_8006649C and func_80069EF8 are those three
   and splat/common_syms.txt names them. */
void func_s63_80024648(u8 *p)
{
    u8 *q;
    u8 *e;
    u8 *r;
    u8 *t;
    u8 *r2;
    u8 *t2;
    UnkNode *n;
    s32 c;
    s32 i;
    s16 out[4];

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) + 1;
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    c = -1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x50) = GetTPage(0, 1, 0x2C0, 0x100);
        *(s16 *)(p + 0x52) = GetClut(0x80, 0x1F7);
        *(s16 *)(p + 0x44) = *(s16 *)(p + 0x46) = 0x1F;
        *(s16 *)(p + 0x48) = 0;
        *(s16 *)(p + 0x4E) = 0x30;
        *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        /* fall through */
    case 1:
        c = 0xE;
        r = D_80082E80;
        if (func_8003DE58(*(u8 **)(r + 0x8), (UnkSlot *)r, out, 0) != 0)
        {
            t = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t + 0x2) - *(s16 *)(p + 0x1E)) << 14;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t + 0x6) - *(s16 *)(p + 0x22)) << 14;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t + 0xA) - *(s16 *)(p + 0x26)) << 14;
            *(s32 *)(p + 0x24) -= func_800644B8((*(s16 *)(p + 0x48) << 11) / 10) << 9;
        }
        if (*(s16 *)(p + 0x48) >= 0xA)
        {
            *(s16 *)(p + 0x4A) = 0x10;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x48) >= 0xB)
        {
            c = 4;
        }
        else
        {
            c = 9;
        }
        *(s16 *)(p + 0x4E) = *(u16 *)(p + 0x4E) + 2;
        r2 = D_80082E80;
        if (func_8003DE58(*(u8 **)(r2 + 0x8), (UnkSlot *)r2, out, 0) != 0)
        {
            t2 = D_80083780;
            *(s32 *)(p + 0x1C) += (out[0] + *(s16 *)(t2 + 0x2) - *(s16 *)(p + 0x1E)) << 15;
            *(s32 *)(p + 0x20) += (out[1] + *(s16 *)(t2 + 0x6) - *(s16 *)(p + 0x22)) << 15;
            *(s32 *)(p + 0x24) += (out[2] + *(s16 *)(t2 + 0xA) - *(s16 *)(p + 0x26)) << 15;
        }
        if (*(s16 *)(p + 0x48) >= 0xD)
        {
            *(s16 *)(p + 0x48) = 8;
        }
        *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) - 1;
        if (*(s16 *)(p + 0x4A) <= 0 && *(s16 *)(p + 0x48) == 0xC)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 3:
        c = 5;
        if (*(s16 *)(p + 0x48) == 0xF)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x40) = (*(u16 *)(p + 0x48) & 3) << 5;
    *(s16 *)(p + 0x42) = (((s16)*(u16 *)(p + 0x48) >> 2) << 5) + 0x80;
    for (i = c; i >= 0; i--)
    {
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s63_80024AA4;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = *(s32 *)(p + 0x1C) + (((rand() & 0x3FF) - 0x1FF) << 12);
            *(s32 *)((u8 *)n->unk_08 + 0x4) = *(s32 *)(p + 0x20) + (((rand() & 0x3FF) - 0x1FF) << 12);
            *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)(p + 0x24) + (((rand() & 0x3FF) - 0x1FF) << 11);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
            *(s16 *)(q + 0x10) = 0x60;
            *(u8 **)(q + 0x0) = D_800DEC00;
            *(u16 *)(q + 0x14) |= 0xC;
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0x808080;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC00 + 0x4);
            e = (u8 *)n + 0x20;
            *(s16 *)(e + 0x4C) = 0;
            *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
        }
    }
}
