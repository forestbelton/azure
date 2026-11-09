#ifndef GTE_INLINE_H
#define GTE_INLINE_H

/* GTE (coprocessor 2) operations as inline asm.
 *
 * `include/gte.inc` is the assembler-side macro set and cannot be used from C.
 * These are the C-side equivalents, and they do work through this toolchain:
 * `__asm__ volatile` survives cc1psx, maspsx and gas, and assembles to the
 * target's exact bytes. func_80044D24 is the first function in the project to
 * need them.
 *
 * `c2 0x480012` is how gas spells MVMVA 1,0,0,0,0.
 *
 * WHY NOT `psyq/include/inline_c.h`, which has all 241 of these? MEASURED
 * 2026-10-27, by compiling a function that uses it through this toolchain:
 * its data-movement macros come out EXACTLY right -- `gte_ldv3`, `gte_ldv0`,
 * `gte_stsxy`, `gte_stsxy3` all assemble to the same `lwc2 $0, 0(%0)` /
 * `swc2 $12, 0(%0)` words as the definitions below -- but its GTE OPERATIONS
 * do not. It spells them as a bare `.word`: `.word 0x000000bf` for RTPT and
 * `.word 0x0000007f` for RTPS, which is ASPSX's encoding, where the assembler
 * supplied the COP2 prefix itself. Modern gas emits that literal word, so the
 * object gets 0x000000bf where the game has 0x4A280030. **It assembles
 * silently**, which is the dangerous part: 130 of the header's 241 `gte_*`
 * macros use that `.word` form, so every operation is wrong and nothing
 * complains. The register loads and stores are the safe half.
 *
 * So inline_c.h cannot replace this file. It could supply the data movement
 * if this file kept the operations, but then a `gte_rtpt()` written without
 * thinking picks up inline_c.h's broken one instead of an undeclared-function
 * error, and the loss is silent. Not worth it until there is much more GTE
 * code than this.
 *
 * One real difference, also measured: inline_c.h's store macros carry a
 * `: "memory"` clobber that these do not. Adding it to `gte_stsxy`/`gte_stsxy3`
 * changed nothing in func_8004C36C -- same word count, same diff.
 *
 * A listing marked "Handwritten function" by splat is marked only because of
 * the coprocessor instructions; it is not a sign that the function cannot be
 * decompiled. */

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" :: "r"(r0))

#define gte_rtv0()   __asm__ volatile ("nop\n\tnop\n\tc2 0x480012")

#define gte_stsv(r0) __asm__ volatile ("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\t" \
                                       "sh $12, 0(%0)\n\tsh $13, 2(%0)\n\tsh $14, 4(%0)" \
                                       :: "r"(r0) : "$12", "$13", "$14")

/* The RTPT group, added for func_8004C36C: three vertices in, three screen
   coordinates out. `c2 0x280030` is RTPT and `c2 0x180001` is RTPS, read out
   of that function's own listing (0x4A280030 and 0x4A180001) rather than
   guessed. The two `nop`s are the GTE's own latency and are part of the
   listing, not padding. */

#define gte_ldv3(r0, r1, r2) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)\n\t" \
                                               "lwc2 $2, 0(%1)\n\tlwc2 $3, 4(%1)\n\t" \
                                               "lwc2 $4, 0(%2)\n\tlwc2 $5, 4(%2)" \
                                               :: "r"(r0), "r"(r1), "r"(r2))

#define gte_rtpt()   __asm__ volatile ("nop\n\tnop\n\tc2 0x280030")

#define gte_rtps()   __asm__ volatile ("nop\n\tnop\n\tc2 0x180001")

#define gte_stsxy3(r0, r1, r2) __asm__ volatile ("swc2 $12, 0(%0)\n\t" \
                                                 "swc2 $13, 0(%1)\n\t" \
                                                 "swc2 $14, 0(%2)" \
                                                 :: "r"(r0), "r"(r1), "r"(r2))

#define gte_stsxy(r0) __asm__ volatile ("swc2 $14, 0(%0)" :: "r"(r0))

/* The func_800AD138 group, added 2026-11-04 when that 432-word park landed:
   a depth-cued, normal-coloured POLY_FT4 pass over the 0x1F800000 scratchpad.
   Every COP2 word below is read off func_800AD138's own listing rather than
   taken from psyq/include/inline_c.h, for the reason this file's header gives
   -- that header spells its OPERATIONS as a bare `.word` and assembles to the
   wrong bytes through modern gas, silently.

     nclip  0x4B400006      avsz3  0x4B58002D      ncds  0x4AE80413

   `gte_ncds_b()` is the NO-nop form (inline_c.h:863 names the convention):
   func_800AD138's listing has mtc2/addiu/sb where gte_ncds()'s two `nop`s
   would be, so the nop-free spelling is the one that matches. The two `nop`s
   in gte_nclip() and gte_avsz3() ARE in the listing and are the GTE's own
   latency.

   gte_stszotz() needs its `$12` scratch declared clobbered and carries the
   `sra $12, $12, 2` the listing has; the store macros take `: "memory"`
   because func_800AD138's scratchpad stores must stay ordered against them --
   without it sched2 sinks them past the following loads. */

#define gte_ldrgb(r0) __asm__ volatile("lwc2 $6, 0(%0)" ::"r"(r0))

#define gte_lddp(r0) __asm__ volatile("mtc2 %0, $8" ::"r"(r0))

#define gte_nclip() __asm__ volatile("nop\n\tnop\n\tc2 0x1400006")

#define gte_avsz3() __asm__ volatile("nop\n\tnop\n\tc2 0x158002D")

#define gte_ncds_b() __asm__ volatile("c2 0xE80413")

#define gte_stopz(r0) __asm__ volatile("swc2 $24, 0(%0)" ::"r"(r0) : "memory")

#define gte_stdp(r0) __asm__ volatile("swc2 $8, 0(%0)" ::"r"(r0) : "memory")

#define gte_stotz(r0) __asm__ volatile("swc2 $7, 0(%0)" ::"r"(r0) : "memory")

#define gte_strgb(r0) __asm__ volatile("swc2 $22, 0(%0)" ::"r"(r0) : "memory")

#define gte_stszotz(r0) __asm__ volatile("mfc2 $12, $19\n\tnop\n\t" \
                                         "sra $12, $12, 2\n\tsw $12, 0(%0)" ::"r"(r0) : "$12", "memory")

#define gte_stsxy3_ft4(r0) __asm__ volatile("swc2 $12, 8(%0)\n\t"  \
                                            "swc2 $13, 16(%0)\n\t" \
                                            "swc2 $14, 24(%0)" ::"r"(r0) : "memory")

/* The func_800AC4C4 group, added 2026-11-09 when that 797-word park landed:
   the town's map-cell POLY_FT4/LINE_F4 pass, which calls func_800AD138 above
   to finish the frame.  Two of the three are NO-nop forms of operations this
   file already has with their nops (inline_c.h:863 names the convention):
   func_800AC4C4's listing fills both latency slots with its own stores, so
   `rtpt` at 0x4A280030 and `rtps` at 0x4A180001 come out bare.  `gte_nccs`
   (0x4B08041B) keeps the two nops, because the listing has them.  Every COP2
   word is read off func_800AC4C4's own listing, not inline_c.h.

     rtpt   0x4A280030      rtps   0x4A180001      nccs   0x4B08041B */

#define gte_rtpt_b() __asm__ volatile("c2 0x280030")

#define gte_rtps_b() __asm__ volatile("c2 0x180001")

#define gte_nccs() __asm__ volatile("nop\n\tnop\n\tc2 0x108041B")

#endif
