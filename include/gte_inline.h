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
 * A listing marked "Handwritten function" by splat is marked only because of
 * the coprocessor instructions; it is not a sign that the function cannot be
 * decompiled. */

#define gte_ldv0(r0) __asm__ volatile ("lwc2 $0, 0(%0)\n\tlwc2 $1, 4(%0)" :: "r"(r0))

#define gte_rtv0()   __asm__ volatile ("nop\n\tnop\n\tc2 0x480012")

#define gte_stsv(r0) __asm__ volatile ("mfc2 $12, $9\n\tmfc2 $13, $10\n\tmfc2 $14, $11\n\t" \
                                       "sh $12, 0(%0)\n\tsh $13, 2(%0)\n\tsh $14, 4(%0)" \
                                       :: "r"(r0) : "$12", "$13", "$14")

#endif
