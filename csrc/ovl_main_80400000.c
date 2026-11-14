#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_80407E08(s32 *p);
extern void func_80402BB8(s32 *p);
extern void func_80403738(s32 *p);
extern void func_804038C8(s32 *p);
extern void func_80403CD4(s32 **pp);
extern void func_80404314(s32 *p, s32 a, s32 b);
extern void func_804044D8(s32 *p);
extern void func_8040454C(s32 **p);
extern void func_80404688(s32 *p);
extern void func_8040553C(s32 *p);
extern void func_804057B0(s32 *p, s32 v);
extern void func_80405900(s32 *p);
extern void func_80407FD4(s32 *p);
extern void func_8040801C(s32 *p);
extern void func_80408060(s32 *p);

/* CARVED 2026-11-13: this file is now the TAIL of the block, not the block.
 * The nine original translation units are in csrc/main_twin/, ten files
 * because c_server.c is split at func_804016D0 (see that directory and
 * docs/overlays.md, "Carving the twin"). What is left here is everything from
 * 0x8040855C -- the statically linked PSY-Q library objects, still asm stubs,
 * documented in docs/twin-block-libcard.md -- plus the whole 0x3C48 `.data`
 * window, whose per-unit partition is known but not yet applied.
 *
 * The notes below are about the BLOCK and still apply to all ten units; the
 * Makefile gives csrc/main_twin/%.o every override this file has.
 *
 * main.bin's DEBUG TWIN of the 0x80020000 card overlay: the same source built
 * at -G8 with its asserts compiled in, linked against a different engine
 * build, and dead on retail (0x80400000 is 8 MB dev-board RAM). It is in the
 * container, so it still has to be reproduced. See docs/overlays.md.
 *
 * THIS UNIT IS THE ONE THE PROJECT BUILDS WITH A DIFFERENT COMPILER. The
 * Makefile points it at tools/psx272gcc -- native FSF gcc 2.7.2 for
 * mips-sony-psx -- because this block is a PSY-Q 4.0 build and everything
 * else is 4.1. Two passes separate them, reload_cse_regs and split
 * addresses, and both are Cygnus/SN additions over FSF 2.7.2. The triage
 * below has the evidence; tools/psx272gcc's header has the provenance.
 * EVERY MEASUREMENT OF THIS UNIT MUST USE THAT COMPILER -- asmlib resolves
 * it from the Makefile, so ovlverify/verify/advise/permimport/twinalign all
 * do it automatically; a hand-rolled `tools/cdkgcc` command does not.
 *
 * THE PLAN IS NOT A SECOND DECOMPILATION. The live overlay's own source
 * rebuilt at -G8 already reproduces 86 of this block's 263 functions
 * instruction-for-instruction, 231 of 263 pair 1:1 with a function it
 * defines, and the 32 that do not are engine code this build linked
 * statically (memory.c, the PSY-Q stubs) which is decompiled in the resident
 * units. tools/assertmap.py reads the missing asserts straight out of here.
 *
 * WHY THE SYMBOLS ARE SPELLED 0x8040xxxx RATHER THAN SHARED. Both blocks sit
 * in ONE container, so a name cannot resolve to two addresses, and the two
 * builds do not agree on any constant offset: card_slot is D_800287CC live
 * and D_804094EC here, a delta of 0x3E0D20, while the data bases differ by
 * 0x3E1320. The external calls are worse than renamed -- this block was
 * linked against a different engine build, where printf is at 0x8007C040,
 * which in the shipped executable is inside libgte's rcossin_tbl.
 *
 * THE LAST 23 FUNCTIONS ARE NOT DECOMPILATION WORK. Everything above
 * func_804084DC -- 23 functions, 289 words, 3.5% of the block -- is PSY-Q
 * library code, and this project keeps PSY-Q library code as ASSEMBLY
 * everywhere, including for the resident executable. Two kinds of evidence,
 * and they agree:
 *
 *   * Nine of them are BIOS call stubs: `addiu $t2, $zero, 0xB0; jr $t2;
 *     addiu $t1, $zero, <call>`, a jump to the BIOS entry point with the
 *     call number in $t1. That is not C in any revision.
 *   * func_804087C8 matches `_ExitCard` (0x80069D78) at 0.90 masked, and
 *     `_ExitCard`'s own listing is `asm/psyq/libcard/END.s` -- the resident
 *     copy of this same code is assembly in this tree too. The `jalr $t2`
 *     BIOS calls are right there in both. The eleven neighbours below it are
 *     named per module in `docs/twin-block-libcard.md`.
 *
 * So they stay on INCLUDE_ASM, as the asm/psyq listings do, and the block's
 * real remaining work is the 239 functions below them.
 *
 * HOW A FUNCTION GETS HERE. `tools/decomp/twinalign.py --todo` pairs each of
 * this block's functions with the live function it came from and sorts by how
 * much they differ; --symmap gives the live -> twin rename. A body is moved
 * over with its references renamed, then measured with
 * `tools/decomp/ovlverify.py --G 8` -- note the --G 8, which is the opposite
 * of every other overlay unit.
 *
 * WHAT THE FIRST 86 FUNCTIONS COST, AND WHAT THEY TAUGHT. Every function
 * whose masked body was already identical to the live source at -G8 was moved.
 * 71 landed at 0 differing words with nothing changed but their symbol names.
 * The other 15 differed ONLY in immediates, and all 15 now land too, after
 * adjustments that fall into four families -- none of which is a different
 * SPELLING of the body, and all of which say something about this build:
 *
 *   * THE WORD INDICES THROUGH ITS STRUCTURES DIFFER, by a constant per
 *     structure: -0x91 words in func_80402BB8, -0x90 in func_80402EBC and
 *     func_804033EC, -0x21 in func_80404314/func_804044D8/func_8040454C,
 *     -0xD in func_8040553C/func_80405900/func_804057B0, +6 in
 *     func_804038C8/func_80403738/func_80403B84/func_80403CD4. These bodies
 *     index a plain `s32 *p` by word, not an UNK_CARD_GROUP struct, so the
 *     fix is the index and the deltas are a direct read-out of how this
 *     build's data is laid out against the live one's.
 *
 *   * FOUR OF ITS TABLES HAVE ONE MORE ENTRY. func_804038C8, func_80403738,
 *     func_80403B84 and func_80403CD4 each need a loop bound of 4 where the
 *     live source has 3, and the +6-word index shift beside it is 24 bytes --
 *     one UnkStrSet record. func_804038C8 also wants 0x22 where the live
 *     source writes 0x1A. A TABLE WITH ONE MORE ENTRY IS NOT SOMETHING AN
 *     ASSERT DOES: this is the first hard evidence for the open question in
 *     docs/where-we-are.md, that part of this block is a DIFFERENT REVISION
 *     of the source rather than the same source with asserts compiled in.
 *
 *   * ONE FUNCTION LOOKED LIKE IT HAD A LOCAL THE LIVE SOURCE DOES NOT
 *     (func_804025F8, 8 bytes, never read). IT DOES NOT, and the device is
 *     gone: the 8 bytes were the COMPILER, and FSF 2.7.2 reserves them by
 *     itself. Kept in this list because a frame 8 bytes too small still
 *     looks like a missing local, and now there is a second thing to check
 *     first.
 *
 *   * ONE CONSTANT IS LOWER BY ONE (func_80402DC8 passes 6 where the live
 *     source passes 7).
 *
 * AND THEN THE SECOND COMPILER RESTARTED IT, 2026-11-11. With
 * tools/psx272gcc in place, `twinalign --todo` went from 1 mask-identical
 * pair to 33, and 32 of those 33 landed in one pass: 21 with nothing but
 * their symbol names changed, 11 after an adjustment. The adjustments were
 * all old families plus TWO NEW ONES:
 *
 *   * A WHOLE STRUCTURE 0x84 BYTES EARLIER. func_80404668, func_804045C8,
 *     func_80404618 and func_80403E54 reach UnkCardGrp240's fields at
 *     0x1C0/0x21C/0x1DC/0x1EC/0x1F4 where the live struct has
 *     0x244/0x2A0/0x260/0x270/0x278 -- a flat -0x84 on every field. Written
 *     as raw word indices off `s32 *p`, as func_804040C8 already was. This
 *     is the first-bullet family at a larger scale, and the cleanest
 *     evidence yet that the two builds' structures genuinely differ.
 *
 *   * FOUR REVISION DIFFERENCES IN ONE FUNCTION. func_80403698 needs the
 *     ptrs array 0x18 later, a loop bound of 4 for 3, func_80056F14 called
 *     with 4 for 3, and a 0x3C stride for 0x48. Nothing in it is a spelling.
 *
 *   * AND THE MIRROR OF func_804065CC's ONE-HANDLER-FOR-TWO:
 *     func_80405F24 has TWO handlers where the live source has one
 *     (func_80406368 and func_80405AB4 for the single live func_80024FFC),
 *     which is why twinmove refuses that symbol -- a real tie, not a weak
 *     vote.
 *
 * THE ONE TRAP IN THAT BATCH IS WORTH MORE THAN THE BATCH. Six of the 32
 * linked against the RESIDENT engine and every one of them measured 0
 * differing. The live source names three objects through `__asm__` ALIASES
 * rather than their symbols -- `card_file_count`, `card_free_blocks`,
 * `D_800814A0_a` -- twinmove only resolved `#define` aliases, so the names
 * survived the rename, csrc/main.h's own declarations resolved them at
 * compile time, and the relocations pointed at the resident executable
 * (D_80084128 where this build wants D_80136140, a different engine build
 * entirely). ovlverify masks relocation targets, so it saw nothing. ONLY THE
 * CONTAINER LINK CAUGHT IT, 33 bytes in 2.5 MB. twinmove now resolves
 * `__asm__` aliases too, carries an alias SUFFIX across the rename when the
 * twin declares one, and reports any alias name that survives. The general
 * rule stands: a clean ovlverify sweep is a reason to build, never a
 * substitute for building.
 *
 * THE SECOND PASS, same day: the 24 `near` rows. 14 more landed, 141 -> 155.
 *
 *   * TWELVE WERE THE reload_cse_regs WRAPPERS and needed no thought at all
 *     once the compiler was right -- see their own comment. Their IDs, not
 *     twinalign, are what pairs them: every wrapper masks to the same ten
 *     words, so the DP had the whole run shifted by one.
 *   * func_804067BC moved over untouched.
 *   * func_80401ED4 NEEDED THE LIVE SOURCE'S OWN DEVICE REVERSED, which is
 *     the most interesting thing in this batch -- see its comment. Without
 *     split addresses a MEM address FOLDS, so the spelling that costs four
 *     words under 4.1 costs three here, and the pointer-local goes on the
 *     OTHER access.
 *
 * THE OTHER TEN ARE REAL MATCHING WORK and are back on INCLUDE_ASM, but their
 * SYMBOL MAPS are not: those are in csrc/main_twin.h now, and they are the
 * expensive part to re-derive. Three of them are findings rather than renames:
 *
 *   * func_80405DE0 (49 differing). func_80024FFC maps to TWO twin functions,
 *     func_80405AB4 after the loop and func_80406368 in the fail arm.
 *     func_80024298's symmap vote is func_80404C84 and is WRONG (a 0.26
 *     pairing); the listing says func_80404688. AND func_80025358 IS NOT
 *     CALLED AT ALL -- the live source calls it between the read and the
 *     write and it is two calls deep (set_save_title + set_save_icon), where
 *     this build has one store, `sw $zero, %lo(0x80010208)`. An earlier
 *     revision of the function, not an assert difference.
 *   * func_804012B0 and func_804010F0, the two state machines, LANDED
 *     2026-11-12. Their two listing-settled renames are below; the devices
 *     that closed them are on func_804010F0 itself.
 *   * D_80084118 -> D_8009E390 for BOTH of those, but a different twin
 *     function genuinely uses the resident D_80084118. Check the listing per
 *     function; the name is not enough.
 *
 * WHERE THE MECHANICAL PHASE ENDED THE FIRST TIME. Everything above the
 * second-compiler batch was a move plus an immediate. The 33 functions of
 * twinalign's `near` and `inserts-only` classes that carried no assert were
 * moved the same way and measured, and only ONE of them (func_80407E08)
 * landed; the rest went back on INCLUDE_ASM. EVERY DIFFERING-WORD COUNT IN
 * THE TRIAGE BELOW WAS TAKEN UNDER THE 4.1 COMPILER AND IS STALE. It is kept
 * because the reasoning is still worth reading, and because two of its
 * verdicts -- the reload_cse_regs cluster and the folded indexed load --
 * are what identified the compiler in the first place.
 *
 *   14 fn, 1 word   NOT A SPELLING -- THIS BLOCK'S COMPILER LACKS
 *   each            reload_cse_regs. RESOLVED 2026-11-11; the park below was
 *                   left open as "one lever here is worth 14 functions" and
 *                   the answer is that there is no source lever at all.
 *
 *                   func_804030F4, func_8040311C, func_80403144,
 *                   func_8040320C, func_80403234, func_8040325C,
 *                   func_80403284, func_804032AC, func_804032D4,
 *                   func_804032FC, func_8040334C, func_80403374,
 *                   func_804033C4 -- the card_mes.c wrappers main.h
 *                   documents -- plus func_804013F8. The live build copies a
 *                   hard register that already holds the value; this block
 *                   materialises it again:
 *
 *                     live  addu $a3,$a2,$zero   twin  addu  $a3,$zero,$zero
 *                     live  addu $a2,$a1,$zero   twin  addiu $a2,$zero,0x2
 *                     live  addu $v1,$v0,$zero   twin  addiu $v1,$zero,imm
 *
 *                   14 for 14, one word each, same position each time, and
 *                   NO function differs the other way round. The pass is
 *                   reload_cse_simplify_set (reload1.c:8142) -- not cse: the
 *                   operand is (const_int 0) in every dump through .lreg and
 *                   (reg a2) in .greg. It has no cost test, takes the
 *                   lowest-numbered hard register holding the value, is
 *                   gated only on `optimize > 0`, and IS ABSENT FROM VANILLA
 *                   gcc-2.7.2.3 -- an SN addition that cdk-gcc-b18 has. So
 *                   this block was built by a cc1psx WITHOUT it, which is
 *                   also why the footprint is so narrow: the pass only bites
 *                   where a hard register already holds a wanted constant.
 *                   docs/matching-levers.md has the full heading and the
 *                   swept-and-failed spellings; do not retry them.
 *
 *                   RESOLVED 2026-11-11 BY A SECOND COMPILER, and it needed
 *                   no wine. THIS BLOCK IS A 4.0 BUILD: two unrelated passes
 *                   4.1 has are missing from it -- reload_cse_regs, above,
 *                   and split addresses, which is the very discriminator
 *                   docs/compiler-version.md uses to prove the GAME code is
 *                   4.1. SN never released 4.0's source (decompals/old-gcc
 *                   has one CDK tarball, b18, and that IS 4.1 -- its
 *                   version.c is `cygnus-2.7.2-970404 SN32.3.7.0004`, the
 *                   string 4.1's cc1psx.exe reports). But 4.0's reports
 *                   `2.7.2.SN32.3.7 Build 0001`, so 4.0 is SN's build of FSF
 *                   gcc 2.7.2 where 4.1 is SN's build of a Cygnus snapshot
 *                   -- and reload_cse, split_addresses, -mfix4300 and
 *                   -ffunction-sections are all in b18 and all absent from
 *                   FSF 2.7.2 AND from 4.0's binary. So FSF 2.7.2 stands in,
 *                   this unit builds with tools/psx272gcc, and both classes
 *                   closed at once.
 *
 *                   IT IS A STAND-IN, NOT THE COMPILER. Where it and the
 *                   real Build 0001 differ the bytes will say so. What is
 *                   measured: 105 of the 106 functions landed under 4.1 are
 *                   byte-identical under it untouched, the 106th
 *                   (func_804025F8) matches once its frame-padding device is
 *                   deleted, and FOUR devices came out of this unit and
 *                   csrc/main_twin.h altogether.
 *
 *   LANDED 2026-11-12  func_80406720 and func_804070C4, which were a
 *                   register race plus a delay slot and stood at 2 words
 *                   differing for three sessions. BOTH halves had a lever.
 *                   The race (the candidate gives $s0 to the counter and
 *                   $s1 to the pointer, the target the reverse, and the four
 *                   prologue/epilogue saves follow) is a priority race: cse
 *                   folds every `q[n]` outside the loop to p-relative, so
 *                   `q` has NO reference outside the loop and loses to `i`.
 *                   A degenerate `do { } while (0)` over the body buys it
 *                   one -- flow.c:440 weights a reference by loop_depth.
 *                   The delay slot is reorg scanning BACKWARD out of a
 *                   CALL_INSN; an empty `asm` between `q++` and the call
 *                   stops it (stop_search_p) and the forward scan takes
 *                   `i++`. Both are in docs/matching-levers.md.
 *
 *   LANDED 2026-11-11, all three by the SAME lever -- a declared size of 8
 *   or less takes the -G8 small-data path, which costs the address ONE
 *   register and leaves the %hi/%lo pair ADJACENT so nothing schedules
 *   between them. See docs/matching-levers.md, "A global's DECLARED SIZE
 *   also decides how many registers its address costs".
 *
 *     func_804078D8   was "two words transposed, a lui/addiu scheduling
 *                     order". D_80408D6C's real size is 8 and it was
 *                     declared `[]`; declaring `[8]` is both correct and the
 *                     fix. Four more twin objects had the same wrong
 *                     declaration (D_804005F0, D_80408C84, D_80408CE8,
 *                     D_80408CF0, D_80408D64); all now carry their real
 *                     size, with the container byte-identical either way.
 *     func_804037F4   needed the lever on a table whose real size is 16, so
 *                     under 4.1 it used a D_80408C8C_a[1] alias. That alias
 *                     is gone -- tools/psx272gcc needs no help here. Its two
 *                     revision differences stand: stride 0x3C for 0x48,
 *                     bound 4 for 3.
 *     func_804040C8   needed no declaration change once D_804005F0 was
 *                     sized -- only the -0x21 word indices and four revision
 *                     constants.
 *
 *   AND ONE ON A SECOND LEVER, THE INCREMENT ORDER. func_80405B98 walks a
 *   5-element loop with a moving pointer `off` and a counter `i`. Written the
 *   live way -- `off++;` then `while (++i < 5)` -- the POINTER increment
 *   fills the two `j` delay slots and the `bnez` takes a nop: 18 differing.
 *   Incrementing the COUNTER first, with a plain `while (i < 5)`, puts `i++`
 *   in those slots and pushes `off++` into the `bnez`'s, which is this
 *   build's shape: 0 differing. Its other difference is an arity one --
 *   func_80405A00 takes TWO arguments here where the live func_80024F3C
 *   takes three, and its listing never reads $a2.
 *
 *   func_80405AE8 was THE SAME TWO LEVERS AND THEN ONE reload_cse_regs
 *   WORD, which made the cluster 15 rather than 14. It was parked at exactly
 *   one differing word with a note saying it would land unchanged the day a
 *   cc1psx without that pass existed. LANDED 2026-11-11 on tools/psx272gcc,
 *   body unchanged, park deleted.
 *
 *   LANDED 2026-11-11 ON A THIRD LEVER, THEN UN-LEVERED THE SAME DAY.
 *   func_80406BEC (12 -> 0), func_804065CC and func_80406F70 (11 -> 0 each)
 *   were landed with a sized DATA ALIAS of a FUNCTION symbol -- `extern u8
 *   F_a[8] __asm__("F");` gives a function a declared size, which puts its
 *   address on the -G8 small-data path and makes it ONE insn, so reorg has
 *   no `lui` to steal into the preceding branch's delay slot where this
 *   build keeps a nop. All three were EMULATING a compiler without split
 *   addresses, one site at a time. This unit now has one, so the plain
 *   spellings match and the three aliases are deleted. The revision
 *   difference they also carry is real and stays: one handler where the live
 *   source has two.
 *
 *   The lever itself is still true and still documented in
 *   docs/matching-levers.md -- it is the right tool for a 4.1-built unit.
 *
 *   func_80406274 and func_80404DEC were THE FOLDED INDEXED LOAD, and both
 *   LANDED 2026-11-11 by moving the live body over with its symbols renamed
 *   and nothing else. Between them they had 33 swept spellings recorded
 *   against them. See their own comments.
 *
 *   others          THE WHOLE LIST IS STALE as of the compiler switch and
 *                   must be re-measured, not read. Every count in it was
 *                   taken under 4.1's cc1. `twinalign.py --todo` on
 *                   tools/psx272gcc now reports 33 of the 156 remaining
 *                   stubs as MASK-IDENTICAL to their live counterparts
 *                   (against 1 before) and 24 more as `near`, so the
 *                   remaining work is mostly moving live bodies over with
 *                   the symbol map rather than finding levers.
 *
 *                   What is known to still be real: func_804018FC is 42
 *                   words against a live counterpart of 2, so that pairing
 *                   is wrong or the live source lost the body entirely.
 *                   func_804012B0's block layout was characterised under
 *                   4.1 and the characterisation was RIGHT: its last arm
 *                   falls through to the shared return in the live build and
 *                   jumps to it here. It landed 2026-11-12 by making that
 *                   jump real -- see the note on the function.
 *
 *                   RE-MEASURE BEFORE BELIEVING ANY OF IT. `rm -rf
 *                   build/twinalign` first: that cache keys on the source
 *                   mtime only and does not notice the compiler changing.
 */

/* The twin of set_card_slot, and the pair that measures the -G difference:
 * 24 words at -G0 in the live block, 26 here at -G8, where a global access
 * comes out as the assembler-expanded `lui $at` + `%lo` pair instead of the
 * hoisted-lui split. Same body as the live one, including the pointer local,
 * which is the in-place flag update device. */

void func_8040855C(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 1);
}

void func_8040859C(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 2);
}

void func_804085DC(s32 *p)
{
    D_8008CA34 = 1;
    func_80400E34();
    func_804083FC((s32)p, 0);
}

/* The body is just `return func_8004DBDC();` with a `short` return type. The
 * marker is a DEVICE, worth one word and five differing (5 -> 0).
 *
 * mips.h's `mips_epilogue_delay_slots` gives the epilogue ONE delay slot when
 * the only saved register is $ra, and reorg's backward scan at the end of
 * `fill_simple_delay_slots` then walks back from the last insn looking for
 * something to put in it. Here it finds the `sra` and the epilogue comes out
 * interleaved and noreorder --
 *     lw $31,16($sp) / sra $2,$2,16 / j $31 / addu $sp,$sp,24
 * -- four words where the target has the plain reorder-mode five:
 *     sra $2,$2,16 / lw $ra / addiu $sp / jr $ra / nop
 *
 * `stop_search_p` (reorg.c) ends that scan at an `asm`, so a marker AFTER the
 * truncation blocks it. The cast is the whole device: without it gcc sinks the
 * sign-extension past the asm to the return and the scan reaches it anyway --
 * `short v = g(); __asm__ volatile(""); return v;` is still 9 words. `"r"((s32)v)`
 * forces the sll/sra before the marker, and then there is nothing behind it.
 *
 * THIS IS A COMPILER-VERSION DIFFERENCE AND THE PROOF IS IN THE TREE.
 * csrc/main/memory.c's `s16 func_80027DD0(void) { return func_8004CAA0(); }`
 * is the same two lines in the live block, it is landed byte-identical under
 * the 4.1 compiler, and its epilogue IS interleaved -- 9 words, exactly what
 * gcc gives here. 4.1 fills that slot, this block's 4.0 build does not, and
 * FSF 2.7.2 and the Cygnus b18 tree carry an identical
 * `mips_epilogue_delay_slots`, so the clause is not where they differ.
 * Across the twin's 49 framed functions that save only $ra, not one fills the
 * jr slot; across the live overlay's 48, two do, and both of those are
 * compiled C. Measured 2026-11-12.
 *
 * Tried and rejected, all still 9 words: `short`/`s32` return, a short
 * callee, a short local, an explicit (short) cast, and an empty volatile asm
 * before the return, between the call and the return, and at the top. */
short func_8040861C(void)
{
    short v = func_8004DBDC();

    __asm__("" :: "r"((s32)v));
    return v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408644);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408654);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408664);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408674);

/* Live func_80027D0C. TWO REVISION DIFFERENCES: func_804086BC takes THREE
 * arguments here and the second is 0x3F, not 1. It is a BIOS 0xB0:0x4E stub,
 * so the listing is the only authority on its arity. */
long func_80408684(long chan)
{
    func_804086CC();
    return func_804086BC(chan, 0x3F, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804086CC);

/* libcard's InitCARD, not game code: ChangeClearPAD, then init_card /
 * start_card / B0:0x55 inside a critical section. The live func_80027D40 the
 * alignment pairs it with is something else entirely. */
void func_804086DC(long pad_enable)
{
    func_8007CB08(0);
    func_8007CA38();
    func_80408798(pad_enable);
    func_804088A0();
    func_80408958();
    func_8007CA48();
}

void func_80408730(void)
{
    func_8007CA38();
    func_804087A8();
    func_8007CB08(0);
    func_8007CA48();
}

void func_80408768(void)
{
    func_804087B8();
    func_80408958();
    func_804087C8();
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408798);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087A8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804087C8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804088A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408928);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_80408958);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80400000", func_804089C4);

/* ---------------------------------------------------------------------------
   THE TWIN'S DATA, 0x804089E0..0x8040C628.

   Everything below is the `.data` subsegment splat hands this unit, 0x3C48
   bytes of it, and the unit has to define every one of them: a dotted `.data`
   subsegment emits no assembly and no stub, so a byte nobody writes here is a
   byte that vanishes from the container. docs/owning-data.md has the
   procedure; four rules decide the shape of this block.

   EVERY OBJECT NEEDS AN EXPLICIT INITIALISER, even the all-zero ones -- and
   89.8% of this region is zero. Without one the compiler emits `.comm`, the
   object lands in this unit's `.bss`, and splat's generated `.bss`
   input-section line sits between the segment's .rodata and its .text, so the
   mistake does not shorten the data: it moves every function in the overlay.

   AND THIS UNIT IS THE ONE BUILT AT -G8, WHERE EVERY OBJECT OF EIGHT BYTES OR
   LESS GOES TO `.sdata`. There is not one $gp-relative reference in this
   block, and the linker script does not collect `.sdata`, so each of the 46
   objects at or below 8 bytes carries an explicit
   `__attribute__((section(".data")))`. It has to name `.data`; an invented
   name is taken literally by gas and the linker silently drops the section.
   The same rule is why neither template below is `const`: a const object goes
   to `.rdata`, which is at the other end of the segment.

   AND EVERY FILE-SCOPE OBJECT GETS `.align 2`, so an object cannot start off
   the 4-byte grid. Nine of splat's 107 labels do -- D_80408ADD..D_80408ADF,
   D_80409526, D_80409CAE, D_8040B1EE, D_8040B516, D_8040C336, D_8040C43E and
   D_8040C5DE, every one of them a byte or halfword read inside something
   larger -- so each rides the 4-aligned object before it and the 107 labels
   come out as 98 objects. Nothing references the nine names.

   DEFINITION ORDER IS ADDRESS ORDER, and the block is at the foot of the file
   so that the functions above keep reaching these through main_twin.h's
   declarations -- several of which are deliberately incomplete, and a
   definition earlier in the file would complete the type at those sites.

   The nineteen cel records at 0x80409140 are forward-declared because the two
   animation scripts and D_80408CFC point at them from above.

   ONLY 29 WORDS IN THE REGION ARE RELOCATIONS, in five places: the two
   animation scripts (9 + 2, into the cel records and each into itself),
   D_80408CFC (11 cel records), D_80408D4C (the two scripts) and D_804090E0
   (five handler addresses, four of them distinct functions of this unit).
   Every other 0x8040xxxx-looking word in here is a plain constant that splat
   did NOT relocate, and is spelled as the address it is -- there is no symbol
   at any of those addresses to name, since they point into the middle of the
   rodata string pool this unit does not own yet. */
extern u8 D_80409140[];
extern u8 D_8040914C[];
extern u8 D_80409158[];
extern u8 D_80409164[];
extern u8 D_80409170[];
extern u8 D_8040917C[];
extern u8 D_80409188[];
extern u8 D_80409194[];
extern u8 D_804091A0[];
extern u8 D_804091AC[];
extern u8 D_804091B8[];
extern u8 D_804091C4[];
extern u8 D_804091D0[];
extern u8 D_804091DC[];
extern u8 D_804091E8[];
extern u8 D_804091F4[];
extern u8 D_80409200[];
extern u8 D_8040920C[];
extern u8 D_80409218[];
/* ------------------------------------------------------------------------- */

/* 0x804089E0 */
/* Eight zero bytes below the first animation script; nothing reads them. */
u8 D_804089E0[0x8] __attribute__((section(".data"))) = {0};

/* 0x804089E8 */
/* The 8-frame animation script, D_80027E20's twin: eight op-2 steps and
   the op-1 jump back to the top. Reached through D_80408D4C[1]. */
UnkAnimStep D_804089E8[9] = {
    {0x6, 2, D_80409140},
    {0x4, 2, D_8040914C},
    {0x2, 2, D_80409158},
    {0x4, 2, D_80409164},
    {0x6, 2, D_80409170},
    {0x4, 2, D_8040917C},
    {0x2, 2, D_80409188},
    {0x4, 2, D_80409194},
    {0x0, 1, D_804089E8},
};

/* 0x80408A30 */
/* The 1-frame script, D_80027E68's twin. */
UnkAnimStep D_80408A30[2] = {
    {0x2, 2, D_80409140},
    {0x0, 0, D_80408A30},
};

/* 0x80408A40 */
/* Three 16-entry 5551 CLUTs, one 0x20-byte row each -- the live block's
   SAVE_ICON_CLUTS has five. Only the first row is written; nothing in
   this unit names the object, so it rides D_80408A30's splat label. */
u8 D_80408A40[3][0x20] = {
    {0x00, 0x00, 0xFF, 0x7F, 0xBE, 0x77, 0x5C, 0x6B, 0xFA, 0x5E, 0xB9, 0x56,
     0x74, 0x4E, 0x30, 0x46, 0x0C, 0x42, 0xCA, 0x35, 0x88, 0x29, 0x46, 0x1D,
     0x25, 0x15, 0xC3, 0x0C, 0x62, 0x08, 0x21, 0x04},
    {0},
    {0},
};

/* 0x80408AA0 */
/* MEM_CARD_NAMES: "bu00:" and "bu10:" in this block's rodata, which is
   not owned yet, so the two words are spelled as the addresses they are. */
const char *D_80408AA0[2] __attribute__((section(".data"))) = {
    (const char *)0x804000EC,
    (const char *)0x804000E4,
};

/* 0x80408AA8 */
/* MEM_CARD_PREFIX. */
char D_80408AA8[16] = "BISLPS-00001";

/* 0x80408AB8 */
/* MEM_CARD_FILENAMES. */
const char *D_80408AB8[6] = {
    (const char *)0x80400110,
    (const char *)0x8040010C,
    (const char *)0x80400108,
    (const char *)0x80400104,
    (const char *)0x80400100,
    (const char *)0x804000FC,
};

/* 0x80408AD0 */
/* The message constructors, in the engine build this block was linked
   against -- 0x80060A1C and friends are not addresses in the shipped
   executable, so there is no symbol to name. */
void (*D_80408AD0[3])(s32 *) = {
    (void (*)(s32 *))0x80060A1C,
    (void (*)(s32 *))0x80060A3C,
    (void (*)(s32 *))0x80060A5C,
};

/* 0x80408ADC */
/* The 20 message records, four bytes each. splat cuts a label at the
   first three bytes (D_80408ADD..D_80408ADF) because the unit reads them
   with lb; nothing references those names. */
UnkTwinMsg D_80408ADC[20] = {
    {0x15, 0x03, 0x02, 0x01},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0C, 0x01, 0x00, 0x00},
    {0x14, 0x02, 0x00, 0x00},
    {0x12, 0x02, 0x00, 0x00},
    {0x0D, 0x02, 0x02, 0x00},
    {0x0E, 0x02, 0x00, 0x00},
    {0x0D, 0x01, 0x00, 0x00},
    {0x10, 0x02, 0x00, 0x00},
    {0x15, 0x04, 0x00, 0x00},
    {0x13, 0x04, 0x00, 0x00},
    {0x10, 0x01, 0x00, 0x00},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0E, 0x02, 0x00, 0x00},
    {0x0C, 0x02, 0x00, 0x00},
    {0x0B, 0x01, 0x00, 0x00},
    {0x14, 0x02, 0x00, 0x00},
    {0x12, 0x02, 0x02, 0x01},
    {0x0E, 0x01, 0x00, 0x00},
};

/* 0x80408B2C */
/* The 20 records' strings, four per record. They live in this block's
   rodata pool, which is not owned yet, so they are addresses. */
u8 *D_80408B2C[20][4] = {
    {(u8 *)0x80400518, (u8 *)0x804004F4, (u8 *)0x804004D4, (u8 *)0x804004D0},
    {(u8 *)0x804004B0, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400474, (u8 *)0x80400448, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400420, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400404, (u8 *)0x804003E8, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804003C8, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804003AC, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400388, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x8040035C, (u8 *)0x80400330, (u8 *)0x80400308, (u8 *)0x804002F4},
    {(u8 *)0x804002D8, (u8 *)0x804002B4, (u8 *)0x80400308, (u8 *)0x804002F4},
    {(u8 *)0x80400290, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400270, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400250, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400230, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400218, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400200, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804001E4, (u8 *)0x804001B8, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400190, (u8 *)0x80400168, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400148, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
};

/* 0x80408C6C */
/* The 24-byte primitive template, D_80027F48's twin. */
UnkBlock24 D_80408C6C = {{0x00, 0x38, 0x00, 0x00, 0xBC, 0xBC, 0x9C, 0x00, 0x00, 0x00, 0xA0, 0x70,
                          0xBC, 0xBC, 0x9C, 0x00, 0xBC, 0xBC, 0x9C, 0x00, 0xBC, 0xBC, 0x9C, 0x00}};

/* 0x80408C84 */
/* D_80027F60's twin. */
u8 D_80408C84[0x8] __attribute__((section(".data"))) = {
    0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x06, 0x35};

/* 0x80408C8C */
/* D_800282BC's twin. */
u8 *D_80408C8C[4] = {
    (u8 *)0x80400598,
    (u8 *)0x80400590,
    (u8 *)0x80400588,
    (u8 *)0x8040057C,
};

/* 0x80408C9C */
/* D_80027F68's twin: a func_80047358 index list, terminated by the
   first byte that is not below the count the call passes. */
u8 D_80408C9C[0x14] = {
    0x00, 0x01, 0x0A, 0x0B, 0x0C, 0x0D, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x31};

/* 0x80408CB0 */
/* D_800282A4's twin. */
u8 *D_80408CB0[2] __attribute__((section(".data"))) = {
    (u8 *)0x804005D8,
    (u8 *)0x804005D4,
};

/* 0x80408CB8 */
/* D_80027FC4's twin, another index list. */
u8 D_80408CB8[0xC] = {
    0x07, 0x08, 0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x09, 0x0A, 0xE1, 0x06};

/* 0x80408CC4 */
/* Nine words the unit reads one at a time, D_80071660..D_80071680 in the
   engine build this block was linked against. Each one is its own object
   because each is read as a scalar, which at -G8 is also why each needs the
   section attribute. */
u32 D_80408CC4 __attribute__((section(".data"))) = 0xB8B8AC00;

/* 0x80408CC8 */
u32 D_80408CC8 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CCC */
u32 D_80408CCC __attribute__((section(".data"))) = 0x0070D0D8;

/* 0x80408CD0 */
u32 D_80408CD0 __attribute__((section(".data"))) = 0xD8D80000;

/* 0x80408CD4 */
u32 D_80408CD4 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CD8 */
u32 D_80408CD8 __attribute__((section(".data"))) = 0x0080B0B8;

/* 0x80408CDC */
u32 D_80408CDC __attribute__((section(".data"))) = 0xD8D80000;

/* 0x80408CE0 */
u32 D_80408CE0 __attribute__((section(".data"))) = 0x00000000;

/* 0x80408CE4 */
u32 D_80408CE4 __attribute__((section(".data"))) = 0x0080B0B8;

/* 0x80408CE8 */
/* D_800717BC's twin. */
u8 D_80408CE8[0x8] __attribute__((section(".data"))) = {
    0x05, 0x06, 0x01, 0x02, 0x03, 0x04, 0x07, 0x30};

/* 0x80408CF0 */
/* D_80027FD0's twin. */
u8 D_80408CF0[0x4] __attribute__((section(".data"))) = {
    0x00, 0x01, 0x02, 0x03};

/* 0x80408CF4 */
/* Live D_800283A8[2] and [3]: two separate pointer words into the
   rodata string pool. Only the first has a name the unit uses. */
u8 *D_80408CF4 __attribute__((section(".data"))) = (u8 *)0x804006A0;

/* 0x80408CF8 */
u8 *D_80408CF8 __attribute__((section(".data"))) = (u8 *)0x80400694;

/* 0x80408CFC */
/* Live D_80027FD4: eleven cel records, [10] being the "clear" one
   func_80404EA0 indexes as D_80408CFC[i]. */
u8 *D_80408CFC[11] = {
    D_804091A0,
    D_804091AC,
    D_804091B8,
    D_804091C4,
    D_804091D0,
    D_804091DC,
    D_804091E8,
    D_804091F4,
    D_80409200,
    D_8040920C,
    D_80409218,
};

/* 0x80408D28 */
/* D_80027FF8's twin. */
u8 D_80408D28[0x18] = {
    0x03, 0x00, 0x01, 0x02, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B,
    0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x51, 0x9D};

/* 0x80408D40 */
/* D_80028068's twin. */
u8 D_80408D40[2][2] __attribute__((section(".data"))) = {
    {0x01, 0x00},
    {0x00, 0x01},
};

/* 0x80408D44 */
/* D_8002806C's twin. */
s32 D_80408D44[2] __attribute__((section(".data"))) = {0x00404040, 0x00808080};

/* 0x80408D4C */
/* D_80028074's twin: the two animation scripts above. */
u8 *D_80408D4C[2] __attribute__((section(".data"))) = {
    (u8 *)D_80408A30,
    (u8 *)D_804089E8,
};

/* 0x80408D54 */
/* D_8002807C's twin. */
u8 D_80408D54[0x8] __attribute__((section(".data"))) = {0};

/* 0x80408D5C */
/* D_80028084's twin. */
u8 D_80408D5C[0x8] __attribute__((section(".data"))) = {
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* 0x80408D64 */
/* D_8002808C's twin. */
u8 D_80408D64[0x8] __attribute__((section(".data"))) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x83};

/* 0x80408D6C */
/* D_800282CC's twin, a Shift-JIS fragment. */
u8 D_80408D6C[0x8] __attribute__((section(".data"))) = {
    0x83, 0x5A, 0x81, 0x5B, 0x83, 0x75, 0x00, 0x00};

/* 0x80408D74 */
/* D_80028094's twin. */
u8 D_80408D74[0x154] = {
    0x02, 0x00, 0x01, 0x03, 0x04, 0x10, 0x00, 0x01, 0xA2, 0x00, 0x00, 0x10,
    0x11, 0x9B, 0x0B, 0x01, 0x50, 0x12, 0x82, 0x00, 0x10, 0x22, 0x03, 0x85,
    0x49, 0x23, 0x43, 0x09, 0x14, 0x03, 0x80, 0x2C, 0x99, 0x02, 0xF4, 0x03,
    0x85, 0x60, 0x34, 0x28, 0x12, 0x32, 0x64, 0x03, 0xF0, 0x2B, 0x03, 0x8D,
    0xE7, 0x20, 0x35, 0xE2, 0x32, 0xE5, 0x07, 0x03, 0xF0, 0x29, 0x60, 0x46,
    0x35, 0x38, 0x65, 0x03, 0x88, 0x58, 0x80, 0xFE, 0x4E, 0x02, 0x8E, 0x81,
    0x04, 0x00, 0x29, 0x67, 0x34, 0x11, 0x31, 0x64, 0x00, 0x03, 0x88, 0xE0,
    0xD0, 0xEE, 0xFF, 0xEF, 0xDE, 0xE0, 0x79, 0x03, 0xF0, 0x28, 0xA0, 0x40,
    0xA7, 0x01, 0x03, 0x88, 0xED, 0x1F, 0xFE, 0x1E, 0xEF, 0x0D, 0x99, 0x04,
    0x00, 0x28, 0x90, 0x40, 0xB5, 0xF8, 0x97, 0x38, 0xB8, 0x7F, 0xFE, 0x7E,
    0x38, 0x8C, 0x04, 0x00, 0x2E, 0x8C, 0x40, 0x22, 0x01, 0x40, 0x87, 0x0C,
    0x6F, 0xFE, 0xEF, 0x86, 0x2E, 0x82, 0x6E, 0x0F, 0x62, 0x00, 0x86, 0x04,
    0x00, 0x2A, 0x23, 0x33, 0x01, 0x40, 0x8E, 0xE6, 0xEF, 0x22, 0xE2, 0xEF,
    0x86, 0x0A, 0x0E, 0x00, 0x86, 0x04, 0x00, 0x27, 0xAD, 0x68, 0x45, 0x60,
    0x33, 0x43, 0x65, 0xA8, 0x0D, 0x8C, 0x08, 0x4E, 0x08, 0x6E, 0x02, 0x00,
    0x88, 0x04, 0x00, 0x27, 0xBE, 0x79, 0x56, 0x44, 0x54, 0x28, 0x76, 0xB9,
    0x0E, 0x00, 0x8E, 0x04, 0x00, 0x27, 0xC0, 0x40, 0x9A, 0x67, 0x55, 0x65,
    0x97, 0xCA, 0x00, 0x8F, 0x01, 0x00, 0x10, 0x27, 0xE0, 0xAC, 0x89, 0x88,
    0x88, 0xA9, 0x0A, 0xEC, 0x00, 0x8F, 0x00, 0x10, 0x28, 0xDE, 0xAB, 0x99,
    0x6C, 0xA9, 0xDB, 0xAF, 0x00, 0x8E, 0xF1, 0x04, 0x10, 0x29, 0xCC, 0xDC,
    0x2F, 0x55, 0x00, 0x8E, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00, 0x10,
    0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00,
    0x10, 0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF,
    0x00, 0x10, 0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00, 0x10,
    0xFF, 0x00, 0x10, 0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x00,
    0x10, 0xFF, 0x00, 0x10, 0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF,
    0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x55, 0x00, 0x10, 0xFF, 0x00, 0x10,
    0xFF, 0x00, 0x10, 0xFF, 0x00, 0x10, 0xFF, 0x15, 0x00, 0x10, 0xFF, 0x00,
    0x10, 0x25, 0x00, 0x00};

/* 0x80408EC8 */
/* The 0x200-byte save-header template func_80406844 block-copies. */
UnkTwinBlk200 D_80408EC8 = {{0x53, 0x43, 0x13, 0x03, 0x82, 0xA0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x1F, 0x00, 0xE0, 0x03, 0x00, 0x7C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                             0x00, 0x00, 0x00, 0x00, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11, 0x11,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22,
                             0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22, 0x22}};

/* 0x804090C8 */
/* The card-state names func_804080A4 prints. */
char *D_804090C8[6] = {
    (char *)0x8040083C,
    (char *)0x80400834,
    (char *)0x80400828,
    (char *)0x8040081C,
    (char *)0x80400814,
    (char *)0x8040080C,
};

/* 0x804090E0 */
/* The handler func_804080A4 picks by state. It is read as a word and
   stored into a word field, so the declared type is s32. */
s32 D_804090E0[6] = {
    0,
    (s32)func_80407E08,
    (s32)func_80407E08,
    (s32)func_80407FD4,
    (s32)func_8040801C,
    (s32)func_80408060,
};

/* 0x804090F8 */
/* The same six names, printed by func_804081AC. */
char *D_804090F8[6] = {
    (char *)0x8040083C,
    (char *)0x80400834,
    (char *)0x80400828,
    (char *)0x8040081C,
    (char *)0x80400814,
    (char *)0x8040080C,
};

/* 0x80409110 */
/* D_80028418's twin: two cel records. */
u8 D_80409110[0x18] = {
    0x40, 0x2C, 0x2C, 0x04, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0x80, 0x10, 0x10,
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x40, 0x60};

/* 0x80409128 */
/* D_80028430's twin. */
u8 D_80409128[0x18] = {
    0x40, 0x2C, 0x2C, 0x04, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0x90, 0x10, 0x10,
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x40, 0x60};

/* 0x80409140 */
/* The eighteen 12-byte cel records the two animation scripts and
   D_80408CFC point at. Nothing in the unit reads one -- each is
   stored by address into a primitive field and consumed outside --
   so they stay u8[], as the live block's do. */
u8 D_80409140[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x00, 0x80, 0x30, 0x40};

/* 0x8040914C */
u8 D_8040914C[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x30, 0x80, 0x30, 0x40};

/* 0x80409158 */
u8 D_80409158[0xC] = {
    0xC0, 0x2C, 0x10, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x60, 0x80, 0x20, 0x40};

/* 0x80409164 */
u8 D_80409164[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x00, 0xC0, 0x30, 0x40};

/* 0x80409170 */
u8 D_80409170[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x30, 0xC0, 0x30, 0x40};

/* 0x8040917C */
u8 D_8040917C[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9E, 0x00, 0x40, 0x7D, 0x00, 0x80, 0x30, 0x40};

/* 0x80409188 */
u8 D_80409188[0xC] = {
    0xC0, 0x2C, 0x10, 0x18, 0x9D, 0x00, 0x40, 0x7D, 0x60, 0xC0, 0x20, 0x40};

/* 0x80409194 */
u8 D_80409194[0xC] = {
    0xC0, 0x2C, 0x08, 0x18, 0x9E, 0x00, 0x40, 0x7D, 0x00, 0xC0, 0x30, 0x40};

/* 0x804091A0 */
u8 D_804091A0[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0xE0, 0x10, 0x10};

/* 0x804091AC */
u8 D_804091AC[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x30, 0xF0, 0x10, 0x10};

/* 0x804091B8 */
u8 D_804091B8[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xB0, 0x10, 0x10};

/* 0x804091C4 */
u8 D_804091C4[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xA0, 0x10, 0x10};

/* 0x804091D0 */
u8 D_804091D0[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xC0, 0x10, 0x10};

/* 0x804091DC */
u8 D_804091DC[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x70, 0xD0, 0x10, 0x10};

/* 0x804091E8 */
u8 D_804091E8[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x40, 0xE0, 0x10, 0x10};

/* 0x804091F4 */
u8 D_804091F4[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x50, 0xE0, 0x10, 0x10};

/* 0x80409200 */
u8 D_80409200[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x60, 0xE0, 0x10, 0x10};

/* 0x8040920C */
u8 D_8040920C[0xC] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x50, 0xF0, 0x10, 0x10};

/* 0x80409218 */
/* The "clear" record, D_80408CFC[10], with eight bytes of slop after
   it that no reader reaches. */
u8 D_80409218[0x14] = {
    0xC0, 0x2C, 0x00, 0x00, 0x9E, 0x00, 0x40, 0x7D, 0x40, 0xF0, 0x10, 0x10,
    0x50, 0x73, 0x03, 0x0C, 0xDA, 0x99, 0x37, 0x00};

/* 0x8040922C */
/* func_804087C8 (a PSY-Q libcard routine, still an asm stub) stashes
   its return address in the first word. */
u8 D_8040922C[0x10] = {0};

/* 0x8040923C */
/* The same stash for func_804088A0 and func_80408958. */
s32 D_8040923C __attribute__((section(".data"))) = 0;

/* 0x80409240 */
/* NOT a jump table: splat named it one, but func_80408848 and
   func_804088A0 read and write the first word as data. The name has
   to stay, because those two are still asm stubs that reference it. */
u8 jtbl_80409240[0x10] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xF6, 0x52, 0x55, 0x00};

/* 0x80409250 */
s32 D_80409250[2] __attribute__((section(".data"))) = {0, 0};

/* 0x80409258 */
s32 D_80409258[2] __attribute__((section(".data"))) = {0, 0};

/* 0x80409260 */
s32 D_80409260[2] __attribute__((section(".data"))) = {0, 0};

/* 0x80409268 */
s32 D_80409268[2] __attribute__((section(".data"))) = {0, 0};

/* 0x80409270 */
/* The eight card event descriptors. */
long D_80409270 __attribute__((section(".data"))) = 0; /* gSwCardEOFEvent */

/* 0x80409274 */
long D_80409274 __attribute__((section(".data"))) = 0; /* gSwCardErrorEvent */

/* 0x80409278 */
long D_80409278 __attribute__((section(".data"))) = 0; /* gSwCardTimeoutEvent */

/* 0x8040927C */
long D_8040927C __attribute__((section(".data"))) = 0; /* gSwCardNewEvent */

/* 0x80409280 */
long D_80409280 __attribute__((section(".data"))) = 0; /* gHwCardEOFEvent */

/* 0x80409284 */
long D_80409284 __attribute__((section(".data"))) = 0; /* gHwCardErrorEvent */

/* 0x80409288 */
long D_80409288 __attribute__((section(".data"))) = 0; /* gHwCardTimeoutEvent */

/* 0x8040928C */
long D_8040928C __attribute__((section(".data"))) = 0; /* gHwCardNewEvent */

/* 0x80409290 */
/* card_dir: the 15-row directory buffer firstfile2/nextfile walk. */
struct DIRENTRY D_80409290[15] = {{{0}}};

/* 0x804094E8 */
s32 D_804094E8 __attribute__((section(".data"))) = 0; /* card_st */

/* 0x804094EC */
s32 D_804094EC __attribute__((section(".data"))) = 0; /* card_slot */

/* 0x804094F0 */
s32 D_804094F0[2] __attribute__((section(".data"))) = {0, 0};

/* 0x804094F8 */
s32 D_804094F8[2] __attribute__((section(".data"))) = {0, 0};

/* 0x80409500 */
s32 D_80409500 __attribute__((section(".data"))) = 0; /* card_evflg */

/* 0x80409504 */
CARD_STATE D_80409504 __attribute__((section(".data"))) = CARD_STATE_OK;

/* 0x80409508 */
/* D_800287E8's twin, the head of the first work buffer: func_8003C714
   is handed this address and a word count. splat cuts a label at
   +0x1E, off the 4-byte grid, so the head is one object. */
u8 D_80409508[0x20] = {0};

/* 0x80409528 */
/* The message group this block keeps. */
u8 D_80409528[0xC] = {0};

/* 0x80409534 */
u8 D_80409534[0x8] __attribute__((section(".data"))) = {0};

/* 0x8040953C */
/* A pointer, read and then flagged (D_8040953C[15] |= 0x2000). */
u16 *D_8040953C __attribute__((section(".data"))) = (u16 *)0;

/* 0x80409540 */
/* The rest of the buffer. */
u8 D_80409540[0x750] = {0};

/* 0x80409C90 */
/* D_800291D0's twin. */
u8 D_80409C90[0x460] = {0};

/* 0x8040A0F0 */
/* Five 0x360-byte groups, one per save-slot row. */
u8 D_8040A0F0[5][0x360] = {{0}};

/* 0x8040B1D0 */
/* The save-slot group. The three bytes at 0x8040B4F5 are the alignment
   slop between this group and the next, not zeros -- see the last
   bullet of docs/owning-data.md SS6. */
u8 D_8040B1D0[0x328] = {
    [0x325] = 0x01,
    0xC0,
    0xD0,
};

/* 0x8040B4F8 */
/* D_8002A990's twin, with one byte of slop at 0x8040C317. */
u8 D_8040B4F8[0xE20] = {
    [0xE1F] = 0x89,
};

/* 0x8040C318 */
/* D_8002B850's twin. */
u8 D_8040C318[0x108] = {0};

/* 0x8040C420 */
/* D_8002B968's twin. */
u8 D_8040C420[0x1A0] = {0};

/* 0x8040C5C0 */
/* The message-pair group, and the last object in the block: the four
   bytes at 0x8040C624 are slop, and 0x8040C628 is where the pad to the
   0x275628 sector boundary begins. */
u8 D_8040C5C0[0x68] = {
    [0x64] = 0x3C,
    0x78,
    0xDA,
    0x42,
};
