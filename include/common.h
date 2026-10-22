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
 */
#define INCLUDE_ASM(FOLDER, NAME)                                 \
    void __maspsx_include_asm_hack_##NAME(void) {                 \
        __asm__(                                                  \
            ".text # maspsx-keep\n"                               \
            "\t.align\t2 # maspsx-keep\n"                         \
            "\t.set noreorder # maspsx-keep\n"                    \
            "\t.set noat # maspsx-keep\n"                         \
            ".include \"" FOLDER "/" #NAME ".s\" # maspsx-keep\n" \
            "\t.set reorder # maspsx-keep\n"                       \
            "\t.set at # maspsx-keep\n"                            \
        );                                                         \
    }

#include "types.h"
#include "structs.h"
#include "include_asm.h"

#endif /* COMMON_H */
