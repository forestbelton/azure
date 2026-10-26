#ifndef COMMON_H
#define COMMON_H

/* maspsx-compatible INCLUDE_ASM.
 *
 * With a non-zero -G, gcc 2.7.2 emits all functions *after* data definitions,
 * which also pushes them after bare __asm__ statements. splat's default
 * INCLUDE_ASM is a bare __asm__, so the included assembly ends up ahead of
 * the C functions in the object and the link comes out in the wrong order.
 *
 * maspsx recognises an __asm__ wrapped in a function named
 * __maspsx_include_asm_hack_* whose lines are tagged "# maspsx-keep", and
 * emits it in place. Defining INCLUDE_ASM here wins because include_asm.h
 * guards its own definition with #ifndef.
 *
 * Guarded like include_asm.h's own definition: under M2CTX or PERMUTER the
 * macro has to expand to nothing, because m2c and decomp-permuter parse this
 * with pycparser and an __asm__ statement is a syntax error to it. Defining
 * it unconditionally here silently defeated that.
 */
#if !defined(M2CTX) && !defined(PERMUTER)
#define INCLUDE_ASM(FOLDER, NAME)                                 \
    void __maspsx_include_asm_hack_##NAME(void)                   \
    {                                                             \
        __asm__(                                                  \
            ".text # maspsx-keep\n"                               \
            "\t.align\t2 # maspsx-keep\n"                         \
            "\t.set noreorder # maspsx-keep\n"                    \
            "\t.set noat # maspsx-keep\n"                         \
            ".include \"" FOLDER "/" #NAME ".s\" # maspsx-keep\n" \
            "\t.set reorder # maspsx-keep\n"                      \
            "\t.set at # maspsx-keep\n");                         \
    }
#endif /* !M2CTX && !PERMUTER */

#include "sys/types.h"
#include "libcd.h"
#include "libgte.h"
#include "libgpu.h"

#include "types.h"
#include "structs.h"
#include "gte_inline.h"
#include "include_asm.h"

#endif /* COMMON_H */
