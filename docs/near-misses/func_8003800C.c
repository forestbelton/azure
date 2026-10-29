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
 * 2026-10-28, SECOND PASS: THE MISSING WORD IS NOT A SCHEDULING PROBLEM. Read
 * the sections below for their measurements, but not for the mechanism -- they
 * conclude "this is a SCHEDULING problem, not a cse one: the fix has to make
 * sched1 place the second read before the `lw 0x34`", and that is wrong at the
 * top. Walk the second load through the dumps and it is a `(mem ...)` in
 * `.cse2`, in `.combine` AND in `.lreg`, and gone in `.greg`:
 *
 *   * local-alloc's `update_equiv_regs` (local-alloc.c:1044-1056) gives it a
 *     REG_EQUIV note, because its destination lives in ONE basic block and
 *     `validate_equiv_mem` proves `(mem (plus q 128))` unchanged for the
 *     register's life;
 *   * RELOAD then satisfies the load from the register the FIRST load put the
 *     value in, and the resulting same-register copy is deleted.
 *
 * So combine never sees it and sched1's order is not what deletes it. The
 * general rule is matching-levers, "A REG-TO-REG MOVE IN THE LISTING IS A LOAD
 * cse DID *NOT* EQUATE". This park's claim that cse keeps both loads is RIGHT
 * and now has a citation -- `note_mem_written` always sets `writes->var`, and
 * `invalidate_memory`'s last clause `cse_rtx_addr_varies_p (p->exp)` ignores
 * the flags, so one store of any width clears every register-plus-constant
 * entry. What was wrong is the conclusion drawn from it.
 *
 * WHY `volatile` BUYS THE LOAD BACK, named at last: `validate_equiv_mem`
 * returns 0 for `side_effects_p (memref)`. Its four exits are the four ways to
 * deny the note, and three of them are closed here:
 *
 *   * a CALL_INSN in the register's live range -- not available, because the
 *     listing holds the second pointer in $v1, a CALLER-saved register, so it
 *     cannot span the do-while's call;
 *   * a store `true_dependence` cannot prove disjoint -- every store in the
 *     range is off `q` with a constant displacement, so all are disambiguated;
 *   * a REG mentioned in the MEM's address being SET in the range
 *     (`reg_overlap_mentioned_p`, in `validate_equiv_mem_from_store`) -- `q`
 *     is the parameter and is never reassigned.
 *
 * THE LISTING'S SHAPE IS NOW REACHED, and what it costs is a different word.
 * `volatile` on BOTH reads, with the second going into this function's
 * EXISTING outer pointer local `r` (so its pseudo is multi-block and
 * multi-set), gives the two `lw 0x80($s1)` ADJACENT -- which nothing before
 * this had. 70 words, 30 DIFF: two loads gained, and the listing's
 * branch-delay `nop` lost, because sched2 puts the `lui %hi(func_80038A10)`
 * FIRST in the block and dbr then speculates it into the preceding `bne`'s
 * delay slot. That is this file's own DELAY-NOPS bullet seen from the other
 * side: a load first in the block keeps the slot empty, a `lui` first does not.
 *
 * AND THE REMAINING TIE IS EXACT. Block 6's sched2 trace for that variant
 * matches the listing PICK FOR PICK except at one slot:
 *
 *     ;; ready list at T-8: 118 (1) 113 (1) 116 (2), now 116 118 113
 *
 * where 118 is the `lui`, 113 the second `lw 0x80` and 116 the `lw 0x34`. 116
 * has priority 2 and wins; the listing picks 118 there, which puts the `lui`
 * fourth and leaves a LOAD first in the block, where dbr cannot speculate it
 * (`may_trap_p`). Everything else is already identical -- both loads, the
 * `lw 0x34`, the addiu/sw pair, `sb 0x2E` before `addiu q+0x3C`, and the three
 * closing stores. **The whole remaining job is to make the `lui` outrank the
 * `lw 0x34` at T-8, or to shorten the `lw 0x34`'s path to the block end so its
 * priority falls to 1.** That 2 comes from its single dependent, the
 * `*(u32 *) (q + 0x1C) = t` store, which sched2 places at T-2.
 *
 * ONE REGISTER DIFFERENCE RIDES ALONG, and it is `combine_regs`. In that
 * variant `t` reuses readA's register (`lw $v1, 0x34($v1)`) because readA DIES
 * in the 0x34 load and `combine_regs` ties an insn's destination into the
 * quantity of a source that dies in it; the listing gives `t` its own ($a0).
 * The tie is refused when the SOURCE pseudo is not block-local or dies more
 * than once (`reg_qty[ureg] < 0`, local-alloc.c:1762) or when the DEST is one
 * local-alloc is not allocating. Buying the second death by reusing `ra` for
 * `q + 0x3C` loses the second load again.
 *
 * MEASURED 2026-10-28 AND NOT WORKING, all 70 words unless noted:
 *
 *     second read through the outer `r`, NO volatile            70,  29
 *     that, with a fresh block-local for the first read         71,   6   (one load + a `move`)
 *     volatile on ONE read only, either position                71,   4   (the long-standing number)
 *     volatile on both, second into `r`  <- THE SHAPE           70,  30
 *     all three reads volatile, including the +0x34 load        70,  30
 *     the two reads' roles swapped                              70,  29
 *     `*(u32 *) (q + 0x1C) = t;` moved above the other stores   70,  30
 *     `ra` reused for `q + 0x3C`, two positions                 70,  29
 *     an ARRAY REF on the pointer, `((u8 **) q)[0x20]`, as
 *       either read or both: does NOT set MEM_IN_STRUCT_P,
 *       because the C front end builds INDIRECT_REF(PLUS) for
 *       a pointer and not ARRAY_REF                             70,  29
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
 * moving the stores that are already there.
 *
 * THAT PARAGRAPH USED TO END by proposing MEM_IN_STRUCT_P as the next thing to
 * try -- two reads of one address differing to cse because one is a struct
 * field and the other a plain cast. **THAT IS CLOSED AND IT WAS WRONG, read
 * out of cse.c on 2026-10-28.** cse does NOT consult MEM_IN_STRUCT_P when it
 * equates. cse.c:1951 sets `hash_arg_in_struct` while hashing, but the flag
 * never enters the hash VALUE; it is stored on the table element
 * (cse.c:1488, `new->in_struct`) and read in exactly one place,
 * `invalidate_memory` at cse.c:1715 -- `p->in_memory && (all || (nonscalar &&
 * p->in_struct) || cse_rtx_addr_varies_p (p->exp))`. So the flag makes an
 * entry EASIER TO INVALIDATE and never harder to equate, and `exp_equiv_p`
 * ignores it entirely. No declared type for the +0x80 object is needed for
 * this, and one would not have helped. (The flag DOES decide sched's memory
 * dependences -- that is matching-levers' "A STRUCT TYPE IS WHAT GIVES A
 * STORE A MEMORY DEPENDENCE AT ALL", which landed three functions the same
 * day -- but sched is not what equates these two loads.) Also measured while
 * closing it: an ARRAY REFERENCE ON A POINTER does not set the flag either.
 * `((u8 **) q)[0x20]` is the same address as `*(u8 **) (q + 0x80)` and the C
 * front end builds INDIRECT_REF(PLUS) for it, not ARRAY_REF, so it dumps as
 * plain `mem`; as `t`'s read, as `r2`'s, and as both, all 70 of 71 and 29.
 *
 * 71 WORDS IS REACHABLE WITHOUT `volatile`, and the extra word is the wrong
 * one. Reusing the function's EXISTING pointer local for the inner read --
 * `r = *(u8 **) (q + 0x80);` in place of the block-local `r2`, and the
 * clearing store through `r` -- is **71 of 71 words and 6 DIFF**, against this
 * file's 70 and 29. But the word is a copy, not the second load:
 *
 *     built   lw $a1, 0x80($s1) ; lui $v0 ; move $v1, $a1 ; lw $v1, 0x34($v1)
 *     target  lw $v0, 0x80($s1) ; lw $v1, 0x80($s1) ; lw $a0, 0x34($v0)
 *
 * so cse equated the two reads after all, which also corrects this file's
 * claim that "`invalidate_memory` drops every varying-address MEM on any
 * store": the `*(u32 *) (q + 0x10)` store between them does NOT invalidate
 * `(mem (plus q 0x80))`, because both are varying addresses off the SAME
 * register with different constant displacements and `memrefs_conflict_p`
 * disambiguates them. A fresh inner local for the first read (`ra = ...;
 * t = *(u32 *) (ra + 0x34);`) with `r` reused for the second is the same 71
 * and the same 6. `volatile` on the reused read is 70 and 30.
 *
 * THE REQUIREMENT IS NOW EXACT, read from block 6's own sched1 trace
 * (tools/decomp/dump.sh, `.sched`, "basic block number 6 from 107 to 135").
 * The eleven insns are 110 readA, 112 `lw 0x34`, 114/116 the func_80038A10
 * address, 118 the +0x10 store, 121 readB, 124 `sb 0x2E`, 127 `addiu q+0x3C`,
 * 129 `sw 0x7C`, 132 `sw 0x1C`, 135 `sw 0x34(readB)`. sched1 emits
 *
 *     built   110 114 112 116 118 124 121 127 129 132 135
 *     target  110 121 112 114 116 118 127 124 129 132 135
 *
 * and readB is the only insn that has to move: everything after it follows by
 * one slot. **readB IS BUMPED** -- `;; ready list at T-5: 124 (2) 118 (2)
 * 121 (7f000001)` -- and its only dependent is the clearing store scheduled at
 * T-1, so it is ready at T-2 and wins by T-5 however the stores are arranged.
 * Un-bumping it is the whole requirement.
 *
 * AND THE TWO HALVES OF THAT FIGHT EACH OTHER, which is why neither device
 * above reaches it. Un-bumping needs `reg_n_sets != 1`, i.e. a second
 * assignment to readB's variable; every second set this function has makes
 * readB the SAME VARIABLE as some other read, and then it is the same pseudo
 * and cse hands back one load and a copy. But the target holds the two values
 * in $v0 and $v1 AT ONCE, which one pseudo with two sets cannot do -- so they
 * must stay two pseudos. `volatile` buys the two pseudos (71 words, 4 DIFF)
 * and leaves readB single-set and bumped, so the `lui` lands between the two
 * loads; local reuse buys the second set and loses the second pseudo. Six
 * `volatile` placements measured 2026-10-28 -- on `r2`, on `t`'s pointer, on
 * both, and each of those with the `+0x34` read volatile too -- and five are
 * 4 DIFF while `volatile` on the +0x34 read alone is 70 and 29.
 *
 * THIS FILE KEEPS THE 70, as the number-against-shape rule says it should:
 * the 71-word version reaches the right COUNT with an instruction the listing
 * does not contain (`move $v1, $a1`), while the 70-word base contains nothing
 * the listing lacks and is simply one short. Do not adopt the 6 without the
 * second load.
 *
 * SO WHAT A SOLUTION LOOKS LIKE: two reads of `q + 0x80` that cse will not
 * equate, where the SECOND one's pseudo has more than one set, and without a
 * `volatile`. Nothing in this source's vocabulary does both at once. The thing
 * to look for is a second assignment to the clearing store's pointer that is
 * NOT a read of q+0x80 -- a value it holds earlier for another purpose -- or a
 * reason the two reads differ that is not volatility.
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
