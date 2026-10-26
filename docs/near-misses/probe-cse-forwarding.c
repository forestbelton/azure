/* PROBE, not a candidate. nmcheck: multi
 *
 * Does MEM_IN_STRUCT_P change whether cse forwards a stored value to a
 * following load? NO -- all four of these emit lhu / addu / sh / andi with no
 * reload, so the flag that decides sched1's alias calls and cse's
 * INVALIDATION (matching-levers section 5) does not decide FORWARDING.
 *
 * It matters because func_80036F24 and func_80037090 are each two words short
 * of their target for exactly one reason: the target RELOADS a u16 field after
 * the increment stores it, and every spelling here forwards instead. Section 5
 * points straight at the struct flag; this rules it out.
 *
 *     tools/ccpsx -O2 -G8 -Iinclude -Ipsyq/include -S -o out.s <this file>
 *
 * Compare pr_a (cast pointer, non-struct reference) with pr_b and pr_c (real
 * struct fields, MEM_IN_STRUCT_P set) and pr_d (store and load through
 * different base expressions).
 */

#include "common.h"

typedef struct Probe
{
    u8 pad_00[0x64];
    u16 f;
    u16 g;
} Probe;

/* A: a cast pointer -- a NON-struct reference. */
s32 pr_a(u8 *p) {
    *(u16 *) (p + 0x64) = *(u16 *) (p + 0x64) + 1;
    return *(u16 *) (p + 0x64) & 1;
}

/* B: a real struct field -- MEM_IN_STRUCT_P is set. */
s32 pr_b(Probe *p) {
    p->f = p->f + 1;
    return p->f & 1;
}

/* C: struct field, post-increment. */
s32 pr_c(Probe *p) {
    p->f++;
    return p->f & 1;
}

/* D: cast pointer, but the store and the load go through DIFFERENT bases. */
s32 pr_d(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c + 1;
    return *(u16 *) (p + 0x64) & 1;
}
