/* PARK (BLOCKED, NOT A FAILING BODY): func_b38_8017087C,

   DISPATCHER ATTEMPT, 2026-11-23, batch 15: THE TWO EDITS THIS PARK PROPOSES
   DO NOT WORK AS WRITTEN. I made both, ran the gate, and dungeon.bin came out
   +0x10 BYTES with 32.84% of it differing, first at 0xfa7004. Reverted; the
   containers are byte-identical again. Recording it so the next attempt does
   not repeat it.

   THE PARK'S EVIDENCE IS CORRECT. I checked the ROM independently:
   `xxd -s 0xfa7070 -l 32 data/dungeon/dungeon.bin' gives

       00fa7070: 0014 0000 0010 0018 0010 0008 000c 0000
       00fa7080: c8ff bd27 1c00 b3af 2198 8000 1000 b0af

   0xfa7080 really is `addiu $sp, $sp, -0x38', the four bytes at 0xfa707c
   really are 0x0C00/0x0000 as halfwords, and that really does make
   D_b38_80170874 six halfwords ending in a terminator -- the same shape as
   the already-landed D_b38_80170868, which is 0xC bytes ending in 0x0000.
   None of that is in doubt.

   WHAT I GOT WRONG IS WHAT MOVING THE `c' START DOES. I read
   `- [0xfa707c, c, ovl_dng_b38]' -> `- [0xfa7080, ...]' as "stop counting
   that word as code". It is not: the subsegment start is where the LINKER
   PLACES the unit's .text, so moving it four bytes later moves every
   instruction in the unit four bytes later. The failure shows it exactly --
   the jump-table entries at 0xfa7004 come out 0x80170874/0x80171570 against
   the ROM's 0x80170870/0x8017156C, every label +4 -- and the image grows
   instead of staying the same size.

   SO THE NEXT ATTEMPT NEEDS TO MOVE THE BOUNDARY WITHOUT MOVING THE TEXT.
   The word belongs to `.rodata', so the `.rodata' subsegment has to end at
   0xfa7080 while `c' still begins where the code does. Whether splat can
   express that for a segment whose `.rodata' and `c' are adjacent, and
   whether `dngrodata.py' has a mode for it, is the open question -- not
   whether the word is rodata, which is settled.

   Until then this function stays parked and its body is CORRECT: 124 built
   words, every one matching, one aligned difference which is the listing's
   leading non-instruction word. `ovlverify's 125 differing is that one-word
   shift and nothing else.

   csrc/ovl_dng_b38.c, agent-h51, 2026-11-23.

   THE BODY BELOW IS BYTE-EXACT. aligndiff.py reports `built 124 words,
   target 125 words, 1 aligned differences' and that ONE difference is the
   target's word 0, which no C can emit. ovlverify prints
   `target 125 words, candidate 124 words, 125 differing' purely because it
   compares word N against word N and the whole stream is shifted by one.

   WHAT IS ACTUALLY WRONG IS THE SEGMENT BOUNDARY, NOT THE FUNCTION.
   splat/dungeon.bin.yaml has

       - [0xfa7008, .rodata, ovl_dng_b38]
       - [0xfa707c, c, ovl_dng_b38]

   so the `c' subsegment starts one word BEFORE the first instruction and the
   generated listing swallows it into this function. The ROM at 0xfa7074:

       00fa7070: 0014 0000 0010 0018 0010 0008 000c 0000
       00fa7080: c8ff bd27 1c00 b3af 2198 8000 1000 b0af

   0xfa7080 is `addiu $sp, $sp, -0x38', the real entry. The four bytes at
   0xfa707c read as halfwords are 0x0C00, 0x0000, which turns the landed
   const u8 D_b38_80170874[] -- today 0x1000, 0x1800, 0x1000, 0x0800, four
   values and no terminator -- into 0x1000, 0x1800, 0x1000, 0x0800, 0x0C00,
   0x0000: SIX halfwords ending in a zero terminator, which is exactly the
   shape of its neighbour D_b38_80170868 (0x1000, 0x0C00, 0x1000, 0x1800,
   0x1400, 0x0000). Both are read by func_b38_80174260 and both are tables of
   0x400 multiples. D_b38_80170874 is 0xC bytes, not 0x8.

   TWO EDITS LAND THIS, AND NEITHER IS MINE TO MAKE (csrc/ is read-only for
   this run and splat/ is the dispatcher's):

     1. splat/dungeon.bin.yaml: `- [0xfa707c, c, ovl_dng_b38]' becomes
        `- [0xfa7080, c, ovl_dng_b38]'.
     2. csrc/ovl_dng_b38.c: const u8 D_b38_80170874[] gains four bytes,
        0x00, 0x0C, 0x00, 0x00, taking it from 0x8 to 0xC. That cannot
        disturb the landed func_b38_80174260, which only takes the array's
        address.

   After (1) the target is 124 words and this body measures 124/124 at
   0 differing. INTEGRATING THE BODY WITHOUT (1) AND (2) MAKES THE CONTAINER
   FOUR BYTES SHORT, because splat currently attributes that rodata word to
   the text subsegment, so do them first or leave the INCLUDE_ASM in place.

   1 copy (dngfam.py: func_b38_8017087C alone).

   MEASURED SPELLINGS. Aligned differences, with the one phantom pad word
   discounted, everything else as in the body below:

     `w = p' collapsed into plain `p' at the six tail sites
                                                  123 words /  16 aligned
     the `|= 0x200' written AFTER the func_800A48F0 call
                                                  125 words /  12 aligned
     the `w + 0xA0' zero written AFTER the +0x92 slide rather than before
                                                  124 words /   4 aligned
   AND ONE MEASURED FREE, so it is NOT why this body is right: `k = a & 3'
   hoisted into a local. Inline as `(a & 3) == 1' / `(a & 3) >= 2' it is also
   124 words / 0 aligned. The twin is the landed 123-word func_b41_8017088C
   (csrc/ovl_dng_b41.c), same frame, and the first two rows above are its two
   recorded facts holding at this body's own prices.

   DECLARATIONS
   UnkNode *func_8003FD64(s32, void *);
   s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
   s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
   u16 func_800A6D30(void);
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   extern u8 D_80174AD4[];
   extern u8 D_80174AFC[];
   void func_b38_80170A70(u8 *, UnkVec *, u8 *, u8 *);
   void func_b38_80171138();
   END DECLARATIONS */

/* Spawn the kind-0x112 node this bank's scripts use for a thrown or placed
   actor: arm it with the bank-local func_b38_80170A70 handler and the
   D_80174AD4 facing table, dress its slot from the b/c/d arguments, pick one
   of three flag pairs off `a & 3', and hand it to func_800A9C18 and
   func_800AA36C.

   124 words of body, 0 aligned differences (aligndiff.py), ON THE FIRST
   COMPILE. `ovlverify' prints `target 125 words, candidate 124 words' for
   this one function and that extra TARGET word is not a missing instruction:
   splat's ovl_dng_b38 `c' subsegment starts at 0xfa707c, one word BEFORE the
   function, so the listing's glabel swallows the 4 bytes at 0x8017087C that
   actually belong to the rodata ahead of it. Read as halfwords they are
   0x0C00, 0x0000, which continues D_b38_80170874's 0x1000, 0x1800, 0x1000,
   0x0800 into the same six-halfword, 0-terminated shape D_b38_80170868 has.
   So D_b38_80170874 is 0xC bytes, not 0x8, and the boundary is 0xfa7080.
   Nothing in this function can emit that word.

   THE TWIN IS THE LANDED 123-word func_b41_8017088C (csrc/ovl_dng_b41.c).
   Its frame is IDENTICAL -- `addiu $sp, $sp, -0x38' and all eight of
   $s0..$s7 saved -- so its allocation facts are the ones that transfer, and
   both of the two it priced HOLD here (aligned differences, with the phantom
   pad word above discounted):
     `w = p' collapsed into plain `p' at the six tail sites
                                                  123 words /  16 aligned
     the `|= 0x200' written AFTER the func_800A48F0 call
                                                  125 words /  12 aligned
   Its one fact measured FREE there is free here too, so it is NOT why either
   body lands: `k = a & 3' hoisted into a local. Inline as `(a & 3) == 1' /
   `(a & 3) >= 2' this is also 124 words / 0 aligned.

   Deltas from that twin, all read off the listing: p[0x13] is 0x26, the
   handler is func_b38_80170A70, the two facing tables are D_80174AD4 and
   D_80174AFC, the installed step is func_b38_80171138, the +0x92 slide is
   -0x30 rather than b41's -0x18, and -- the one extra word against b41's 123
   -- the tail also zeroes `w + 0xA0'. That store's POSITION is this body's
   own third measured fact and it is NOT free: it has to precede the +0x92
   slide, because the two share the `sh' delay slot of the func_800AA36C
   call. Written after it, 124 words / 4 aligned. 1 copy (dngfam.py). */
u8 *func_b38_8017087C(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b38_80170A70;
        p[0x13] = 0x26;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_80174AD4;
        k = a & 3;
        *((u8 *)r + 0x24) = b;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    *(u32 *)(p + 0x1C) |= 0x200;
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)((u8 *)r + 0x2C) = D_80174AFC;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b38_80171138;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0xA0) = 0;
        *(s16 *)(w + 0x92) = -0x30;
        func_800AA36C(w, m, (u8 *)r, p);
    }
    return p;
}
