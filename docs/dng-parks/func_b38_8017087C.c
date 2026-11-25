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

   SECOND INDEPENDENT CONFIRMATION, 2026-11-24, agent-h66, batch 18. THE BODY
   BELOW IS CORRECT AND THE BLOCKER IS THE BOUNDARY, NOT THE C. I spliced it
   into my own copy of the unit unchanged and `aligndiff.py' gives

       built 124 words, target 125 words, 1 aligned differences
       -         0 8017087C      sll $at, $zero, 16

   -- the single difference is the target's word 0, which is the rodata word
   and which no C can emit. `ovlverify' gives `target 125 words, candidate
   124 words, 125 differing', the whole-stream shift, exactly as the park
   says. I also RE-PRICED the park's first measured spelling on my copy: `w =
   p' collapsed into plain `p' at the six tail sites is `built 123 words,
   target 125 words, 17 aligned differences', i.e. 16 with the phantom word
   discounted -- the park's recorded figure to the digit. The table is sound.

   TWO NEW PIECES OF EVIDENCE, BOTH FROM THE REPO AND NEITHER NEEDING A BUILD.

   (1) THE ROM'S OWN HANDLER PAIR NAMES 0x80170880 AS THE ENTRY, AND SPLAT
   CANNOT NAME IT. Every carved object opens with an 8-byte `data'
   subsegment holding two code pointers. For ovl_dng_b38 splat has already
   written them out, in asm/data/FA7000.data.s:

       dlabel D_b38_80170800
           .word 0x80170880
           .word func_b38_80170A70
       enddlabel D_b38_80170800

   The SECOND word resolved to a symbol. The FIRST did not, because under the
   current boundary no symbol exists at 0x80170880 -- splat put the first
   `glabel' at 0x8017087C. Across all 116 carved objects' data listings
   exactly TWO have a bare literal in that slot instead of a symbol:
   asm/data/FA7000.data.s (this one, 0x80170880) and asm/data/E2D000.data.s
   (ovl_dng_b29, 0x80170890, where splat's `c' start is 0xe2d08c and the
   swallowed word is 0x2E004981). Every other carved object's first handler
   resolves. So the defect is a one-word-early `c' start, it is detectable
   WITHOUT any ROM reading, and ovl_dng_b29 has it too.

   (2) THE LINKER SCRIPT DOES NOT PIN .text, SO EDIT (2) IS WHAT MOVES IT AND
   EDIT (1) DOES NOT. build/dungeon.bin.ld:1718-1734 places the segment at
   0x80170800 and then emits, in order and with no address of its own,
   `.rodata', `.bss', `.text', `.data'. `nm build/dungeon.bin.elf' on the
   committed build gives

       ovl_dng_b38_RODATA_START = 0x80170808
       ovl_dng_b38_RODATA_SIZE  = 0x74
       ovl_dng_b38_RODATA_END   = 0x8017087c
       ovl_dng_b38_TEXT_START   = 0x8017087c

   so TEXT_START is a CONSEQUENCE of the emitted rodata size, not of the yaml.
   Today the container matches because the stray word is carried as the first
   `instruction' of func_b38_8017087C's INCLUDE_ASM listing. Growing
   D_b38_80170874 from 0x8 to 0xC makes RODATA_SIZE 0x78 and TEXT_START
   0x80170880, and the 124-word body then occupies 0x80170880..0x80170A70 --
   the ROM's own extent -- with the container the same size as now. THE TWO
   EDITS MUST THEREFORE BE MADE TOGETHER WITH THE BODY SPLICED IN, AND IN THAT
   combination nothing moves:

       rodata 0x74 + stub 125 words  = today, matches
       rodata 0x78 + body  124 words = also matches
       rodata 0x78 + stub  125 words = every label +4   <- the failed attempt
       rodata 0x74 + body  124 words = container 4 bytes short

   The recorded failure (`every label +4, image grew') is the THIRD row. That
   is what you get from edit (2) while the text still has the 125-word
   listing -- i.e. if the body was not spliced, or if splat had not
   regenerated the listing at its new 124-word length. It is NOT evidence
   against the park. Edit (1) is needed for the regenerated listing's length
   and start and so that 0xfa7000's first word resolves to a symbol; it has no
   effect on the link by itself.

   A SPLICED COPY READY FOR THAT TEST is at
   /home/case/scratch/azure-dng/agent-h66/ovl_dng_b38.with_8017087C_park.c
   (this body plus the three functions agent-h66 landed this batch). It still
   has D_b38_80170874 at 0x8 bytes; edit (2) is one four-byte addition there.

   THE BODY BELOW IS BYTE-EXACT. aligndiff.py reports `built 124 words,
   target 125 words, 1 aligned differences' and that ONE difference is the
   target's word 0, which no C can emit. ovlverify prints
   `target 125 words, candidate 124 words, 125 differing' purely because it
   compares word N against word N and the whole stream is shifted by one.

   THE THREE EDITS, VERIFIED AGAINST THE CONTAINER BYTES, 2026-11-25.  The
   dispatcher read the bytes rather than reasoning about them, and the park's
   reading is confirmed exactly.  From `xxd' on build/dungeon.bin:

       0xfa7068  0010 000c 0010 0018 0014 0000   D_b38_80170868, 0xC bytes,
                                                 six halfwords ending 0x0000
       0xfa7074  0010 0018 0010 0008             D_b38_80170874, 8 bytes
       0xfa707c  000c 0000                       THE DISPUTED WORD -- two more
                                                 halfwords ending in the same
                                                 0x0000 TERMINATOR, so the
                                                 array is six halfwords too
       0xfa7080  c8ff bd27  = addiu $sp,$sp,-0x38  the real first instruction

   (A caution for whoever repeats this: read `build/dungeon.bin', NOT `az.bin'.
   az.bin is the 298MB disc image and the same offset in it lands somewhere
   unrelated -- it gave `0800 4000' here and briefly looked like a refutation.)

   SO THE EDITS ARE:

     (i)   csrc/ovl_dng_b38.c, D_b38_80170874: append `0x00, 0x0C, 0x00, 0x00'
           so the array is 0xC bytes.  This is the edit that moves TEXT_START
           from 0x8017087C to 0x80170880, because the linker script does not
           pin .text.
     (ii)  splat/dungeon.bin.yaml:794
             - [0xfa707c, c, ovl_dng_b38]   ->   - [0xfa7080, c, ovl_dng_b38]
     (iii) splice the body below in place of the INCLUDE_ASM stub AND RENAME IT
           `func_b38_80170880'.  Its address changes, so splat will expect that
           name, and the handler word at 0xfa7000 can only resolve to a symbol
           that exists at 0x80170880 -- which is the defect this whole park is
           about.

   All three together, then `make splat' (it regenerates asm/ and the data
   listing), then the build gate.  Nothing else in the unit moves: the body
   ends at 0x80170A70 exactly as the 125-word stub listing does today.

   WHY THIS IS NOT DONE YET, 2026-11-25: `make splat' does `rm -rf asm
   build/asm' and regenerates every container's listings.  Four agents were
   measuring against those listings when this was prepared, and deleting them
   mid-run is the one thing every agent brief forbids -- the rule applies to
   the dispatcher too.  The edits are small, verified and independent of any
   agent's work; they need a quiet tree, not more analysis.

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
   void func_b38_80170A70(u8 *, UnkVec *, u8 *);
        -- THREE parameters, corrected in batch 25.  This line used to say
        four, which would have been a `conflicting types' error the moment
        this park was spliced back into the unit: func_b38_80170A70 landed
        in batch 25 as (u8 *p, UnkVec *b, u8 *c), its listing's prologue
        consumes only $a0/$a1/$a2, and all twenty landed members of the
        func_800D8728 family are three-parameter.  Nothing in this park's
        C depends on the arity -- it only takes the address, at the
        `n->unk_10 = (u32)func_b38_80170A70' store below -- so the fix is
        the declaration alone and the parked body is unaffected.
   void func_b38_80171138();
   END DECLARATIONS    THE BOUNDARY MOVE WAS TRIED AND IT DOES NOT WORK, 2026-11-24. This settles
   the question the park has been waiting on, and the answer is that the yaml
   is the wrong lever. What was done and what happened:

     1. `[0xfa707c, c, ovl_dng_b38]' -> `[0xfa7080, c, ovl_dng_b38]', then
        `make splat'. The data word at 0xfa7000 IMMEDIATELY resolved from the
        bare `.word 0x80170880' to `.word func_b38_80170880' -- so that half of
        agent-h66's evidence is exactly right, and the one-word-early `c' start
        is real.
     2. The stub had to be renamed in csrc too (`func_b38_8017087C' ->
        `func_b38_80170880'), because the Makefile keeps only the listings an
        INCLUDE_ASM names; without that rename splat emits the listing and the
        build deletes it, which looks like splat failing to disassemble the
        region.
     3. The regenerated listing is **124 instruction words**, and THIS PARK'S
        BODY MEASURES 124 words / 0 differing against it, both `ovlverify' and
        `aligndiff'. **The body is correct. That is now proved, not argued.**
     4. AND THE CONTAINER STILL FAILED: dungeon.bin came out 59f4c929 against
        the reference 27f9fcc3, with every entry of the 0xfa7000 handler table
        FOUR LOWER than the ROM's (built 0x8017087c / 0x80170a6c / 0x8017156c,
        ref 0x80170880 / 0x80170a70 / 0x80171570).

   WHY, AND IT IS THE THING TO REMEMBER: `.rodata, ovl_dng_b38' TAKES ITS SIZE
   FROM THE COMPILER'S OUTPUT, NOT FROM THE YAML RANGE. The yaml range only
   says which ROM bytes splat disassembles. `build/dungeon.bin.ld' does not pin
   `.text'; TEXT_START is RODATA_END, and RODATA_END is wherever the 0x74 of
   jump tables the C file emits happens to end. Moving the yaml boundary
   therefore changed NOTHING about the link, and the text stayed at
   0x8017087c.

   So agent-h66's four-row table is wrong in its premise: there is no
   combination of yaml edits that produces `rodata 0x78', and its explanation
   of the earlier "+4 on every label" failure (the listing not regenerated) is
   wrong too. BOTH failures have the same single cause -- the emitted rodata
   size never moved.

   WHAT WOULD ACTUALLY LAND IT: the unit's `.rodata' must grow by 4 bytes, so
   the C file has to DEFINE the halfword table that occupies 0x8017087c. That
   is `D_b38_80170874' -- the earlier ROM reading found the bytes there make it
   six halfwords with a terminator, matching `D_b38_80170868'. Defining a const
   array in the unit is a different and larger change from a boundary move, it
   affects how every other bank-local table in the unit is declared, and it
   should be decided deliberately rather than as a side effect of landing one
   function. ovl_dng_b29 has the identical defect (its first word disassembles
   as the nonsense `sltiu $zero, $s0, 0x4981', the next is a real
   `addiu $sp, $sp, -0x40' prologue, and its data word `.word 0x80170890'
   points at that prologue), so whatever is decided here applies there too.

   THE PARK THEREFORE STAYS, with its body KNOWN-GOOD and the blocker named
   precisely: not the C, not the boundary, but the unit's rodata size.

*/

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
