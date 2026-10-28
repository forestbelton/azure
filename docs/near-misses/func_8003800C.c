/* func_8003800C (unk_72A8) -- 70 of 71 words, 29 DIFF. NOT compiled.
 *
 * The D_800809C4 reset for one actor's +0x8C sub-struct: three func_80043FB8
 * calls, the owner's +3/+4 byte pair synchronised, then -- if the owner's
 * +0x34 handle is set -- the state function driven round to func_80038A10,
 * the sub-struct re-pointed at its own +0x3C and the owner's handle taken
 * over and cleared. Finally the state function is called, defaulting to
 * func_80038A10 if null.
 *
 * `q` is the SUB-STRUCT, not the actor: every offset here is one of
 * func_80037D9C's `q = p + 0x8C` fields, and `*(u8 **)(q + 0x80)` is the
 * owner pointer that function stores there. `q + 0x3C` is the actor's
 * +0xC8, which is what func_80037D9C writes to the same +0x7C slot.
 *
 * WHAT IS PAID FOR -- 70 words on the first draft too, but 36 DIFF then
 *
 * * The owner byte is read ONCE into a local: `v = r[3]; r[4] = v;
 *   q[0x28] = v;`, exactly as func_80037D9C spells the same three lines.
 *   Written as `r[4] = r[3]; q[0x28] = r[3];` the second `r[3]` is a separate
 *   pseudo -- the first `sb` is a QImode store, and `note_mem_written` sets
 *   cse's `all` flag for any QImode store (ANSI lets a char pointer alias
 *   anything), so the read is not shared -- and reload turns it into
 *   `addu $v0, $a1, $zero`. Same word count, but the copy occupies the `beq`
 *   delay slot the target leaves as a nop, and seven words differ. **An extra
 *   reg-reg copy right after an `lbu` whose value is stored twice means the
 *   byte is a LOCAL in the source.**
 *
 * * The three reads of the owner in the +3/+4 block go through one pointer
 *   local; the comparison casts to `s8` (two `lb`) and the stored value does
 *   not (one `lbu`). Three loads of the owner POINTER would be wrong: the
 *   block has one `lw 0x80`, so the pointer is cached and only the byte is
 *   re-spelled.
 *
 * * The `+0x34` guard and the `+0x34` uses each re-read `*(u8 **)(q + 0x80)`
 *   rather than reusing the block-1 local: the two `sb`s in block 1 set cse's
 *   `all` flag, so the pointer is reloaded, and the listing has a fresh
 *   `lw 0x80` at 80038074 to prove it.
 *
 * * `t` -- the owner's +0x34 -- is read as the FIRST statement of the guarded
 *   block, before the `+0x10` store. That is what puts `lw $a0, 0x34($v0)`
 *   ahead of the `lui`: read after the store, the load picks up a true
 *   dependence on it (its base is a loaded pointer, so memrefs_conflict_p
 *   cannot disambiguate) and sched1 cannot lift it. Worth the whole tail
 *   order plus the `bne` delay-slot nop -- see below.
 *
 * * **A `nop` in a branch delay slot can mean the next insn is a LOAD.**
 *   dbr will not speculate a `may_trap_p` insn into a slot that executes on
 *   both paths, so when the fall-through block starts with a `lw` the slot
 *   stays empty; when it starts with a `lui` dbr fills it. The nop after
 *   `bne $v0, $s0` is therefore a statement about what sched2 put first, not
 *   about dependences.
 *
 * WHAT IS LEFT: one word -- the SECOND `lw 0x80($s1)` in the guarded block.
 *
 * The target reads the owner twice there, once for `t` and once as the base
 * of the clearing store, and keeps both in registers ($v0 and $v1). cse does
 * keep two loads: `invalidate_memory` drops every varying-address MEM on any
 * store, so the `+0x10` store between them is enough, and all four loads of
 * q+0x80 survive to `lreg`. **greg deletes the second one**, because at
 * sched1 time the two pseudos' live ranges do not overlap -- sched1 emits
 * readB, lui, lw 0x34 (readB dead), ..., readA -- so local-alloc gives both
 * the same hard register and the load becomes `$v1 = $v1`.
 *
 * So this is a SCHEDULING problem, not a cse one: the fix has to make sched1
 * place the second read before the `lw 0x34`. Measured and not working, all
 * at 70 words: both reads through named locals in either order; the second
 * read placed in all four slots between the five stores (`r2@0..3` x
 * `t@0..3`, 16 combinations); the final store's base inline vs a local; the
 * +0x1C value through `t` vs re-read; `u32` vs `u8 *` for the cached owner;
 * three reads instead of two. A duplicated `q[0x2E] = 0;` does give 71 words
 * and 11 DIFF, but flow does NOT drop the dead first store -- the output has
 * two `sb`s and still one load -- so that number is an alignment accident.
 *
 * sched1's own trace (tools/decomp/sched.py --sched1, block 6) is the place to
 * start: the two reads and the `lui` are all priority 1, the ready list at
 * T-4 holds the sb, the +0x10 store, the +0x3C addiu and readA, and it picks
 * the addiu. The target's order needs the sb at T-4 and readA at T-10.
 *
 * SINCE PARKED (advise.py): the missing word is localised. advise.py's branch
 * spans put it inside the `beq` at 80038084 (span 80038088..800380E4), and
 * the listing there reads `*(u8 **) (q + 0x80)` TWICE in adjacent
 * instructions:
 *
 *     lw $v0, 0x80($s1)      ; one pointer
 *     lw $v1, 0x80($s1)      ; and a second, same address
 *     lw $a0, 0x34($v0)      ; +0x34 through the first
 *
 * cse shares the two reads here however they are spelled, so one load is all
 * this gets -- and the second pointer is what the block's last store
 * (`*(u32 *) (r2 + 0x34) = 0`) goes through, so it is live across the whole
 * block alongside the first. Forcing the reload with the repo's `volatile`
 * device -- `(u8 *) *(volatile u32 *) (q + 0x80)` -- does produce 71 words
 * and takes the function to 4 DIFF, but sched then puts the `+0x34` load
 * BETWEEN the two pointer loads (gcc 2.7.2's sched orders a volatile MEM
 * against stores and other volatiles, not against plain loads), so the two
 * never come out adjacent and the pair lands in one register instead of two.
 * `volatile` on both is the same 4. So the original is reading that field
 * twice through something cse cannot equate, and finding what that is is the
 * whole remaining job.
 *
 * A STORE BETWEEN THE TWO READS DOES NOT DO IT -- measured 2026-10-28. The
 * reasoning was that cse's memory table is flushed by a store it cannot prove
 * disjoint, and the `*(u32 *) (q + 0x10) = func_80038A10` store sits AFTER the
 * `t = ...` read in this source while the only thing between the two `+0x34`
 * reads is a CONDITIONAL call -- so on the path that skips the call, nothing
 * invalidates and cse equates them. Moving that store earlier does not help:
 *
 *     store before the `t = ...` read        69 of 71, 29 DIFF
 *     store after `r2 = ...`                 70 of 71, 29 DIFF
 *     `r2` read first, `t` read off `r2`     70 of 71, 29 DIFF
 *
 * all against the base's 70 of 71. So the invalidation is not reachable by
 * moving the stores that are already there, and the next thing to try is the
 * OTHER half of what decides cse's equating: MEM_IN_STRUCT_P (section 5). Two
 * reads of the same address differ to cse if one carries it and the other does
 * not, which in source terms is one read through a struct field and the other
 * through a plain pointer cast. That needs a declared type for the +0x80
 * object, which this source does not have.
 */

#include "common.h"

extern u8 D_800809C4[];
extern u8 D_8002D030[];

s32 func_80043FB8(s32 arg0, ...);
s32 func_80053EF0(s32);
void func_800387D0(u8 *);
void func_80038A10(u8 *, u8 *, s32);

void func_8003800C(u8 *q)
{
    u8 *r;
    u8 v;

    func_80043FB8((s32)D_800809C4);
    func_80043FB8((s32)D_8002D030, func_80053EF0(4));
    func_80043FB8((s32)D_800809C4);
    r = *(u8 **)(q + 0x80);
    if (*(s8 *)(r + 3) != *(s8 *)(r + 4))
    {
        v = r[3];
        r[4] = v;
        q[0x28] = v;
    }
    if (*(u32 *)(*(u8 **)(q + 0x80) + 0x34) != 0)
    {
        u8 *r2;
        u32 t;

        if (*(u32 *)(q + 0x10) == (u32)func_800387D0)
        {
            do
            {
                func_800387D0(q);
            } while (*(u32 *)(q + 0x10) != (u32)func_80038A10);
        }
        t = *(u32 *)(*(u8 **)(q + 0x80) + 0x34);
        *(u32 *)(q + 0x10) = (u32)func_80038A10;
        r2 = *(u8 **)(q + 0x80);
        q[0x2E] = 0;
        *(u32 *)(q + 0x7C) = (u32)(q + 0x3C);
        *(u32 *)(q + 0x1C) = t;
        *(u32 *)(r2 + 0x34) = 0;
    }
    if (*(u32 *)(q + 0x10) == 0)
    {
        *(u32 *)(q + 0x10) = (u32)func_80038A10;
    }
    (*(void (**)(u8 *))(q + 0x10))(q);
}
