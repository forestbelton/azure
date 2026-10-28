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

#endif
