/* func_8005A608 (unk_2CDC0) -- 92 of 92 words, 13 DIFF. NOT compiled.
 * WAS 88 of 92, 85 DIFF. The four missing words are now paid for and what is
 * left is one register chain.
 *
 * Claims a D_80086A40 slot for `p` -- the caller's own index when it gives
 * one, otherwise the first free slot of sixteen -- fills it, and asks
 * func_80059E94 for a handle. Returns the slot index, or -1 if there was no
 * free slot or no handle.
 *
 * THE LEVER, and it is new: the index parameter is `s32` and the function
 * RE-ASSIGNS IT ITS OWN TRUNCATION,
 *
 *     j = id;              // s16 -- the raw value, a plain copy
 *     id = (s16) id;       // the sign-extension, IN PLACE
 *     if (id == -1) { ...scan... }
 *     i = j;               // the else arm uses the raw
 *
 * which buys three of the four words at once:
 *
 *   1. `addu $v1, $a1, $zero`. `j = id` is a real copy because the next
 *      statement overwrites `id`, so the raw has to be saved first. Walking
 *      the parameter straight through (`s16 id` with `i = id`) lets the raw
 *      stay in $a1 and no copy is emitted.
 *   2+3. `sll $v0, $s1, 16` / `sra $a0, $v0, 16` in the ELSE arm. Assigning
 *      to `id` INVALIDATES cse's equivalences for that pseudo, so the else's
 *      `(s16) i` -- with `i` copied from the raw `j` -- is no longer the same
 *      expression as the entry's `(s16) id` and has to be recomputed. With a
 *      plain `s16 id`, cse forwards the entry's extension into the else (it
 *      follows the conditional jump: the label has one use and is preceded by
 *      a barrier) and those two words go missing.
 *
 * Three variables do NOT work: `j = id; t = (s16) id; if (t == -1)` is 88
 * words again, because the extension then lands in its own pseudo and the
 * parameter keeps $a1 with no copy. The assignment has to be back to `id`.
 *
 * THE IN-PLACE EXTENSION IS NOW REPRODUCED, and it took writing the shifts
 * out by hand:
 *
 *     id = id << 16;
 *     id = id >> 16;
 *
 * rather than `id = (s16) id`. MIPS's `extendhisi2` expander always allocates
 * a FRESH pseudo for the intermediate, so the cast gives `sll $v0, $a1, 16` /
 * `sra $a1, $v0, 16` -- two registers -- and the two shifts written as
 * statements are one pseudo set twice, which comes out as the listing's
 * `sll $a1, $a1, 16` / `sra $a1, $a1, 16`. That is a general lever and is now
 * in matching-levers section 7.
 *
 * WHAT IS LEFT with the hand-written shifts: 13 words, and they are ONE
 * SCHEDULING FACT. The listing emits `sw $zero, %lo(D_8007382C)($v0)` AFTER
 * the two shifts; this source emits it before. That one position decides
 * everything else: with the store late, $v0 is still holding the symbol's
 * high half across the extension, so `j = id` falls to $v1 (the listing's
 * `addu $v1, $a1, $zero`) and the -1 then reuses $v0 once the store frees it;
 * with the store early, $v0 is free, `j` takes it and the -1 is pushed to
 * $a2, and the then-arm's own -1 becomes a copy instead of a fresh
 * materialisation.
 *
 * The store has priority 1 in sched2 (nothing in the block depends on it) and
 * the `sra` has 2, so the usual ordering puts the store first. In the listing
 * it is the other way round, which is `schedule_select`'s `potential_hazard`
 * and not priority. All twelve orders of the four entry statements were
 * measured with the hand-written shifts: 13 is the floor, 15, 16 and 18 the
 * alternatives, and nothing in the source moves the store.
 *
 * CORRECTED 2026-10-27, twice over.
 *
 * (1) THOSE PRIORITIES ARE THE SCHED1 DUMP'S. In sched2 -- the pass that
 * emits this order -- EVERY insn in block 0 is priority 1, the store and both
 * shifts included; the dump's own list says so. They are therefore all in one
 * group, which is the only place `potential_hazard` can act: `schedule_select`
 * (sched.c:2616) walks the ready list in groups of EQUAL priority, picks the
 * largest potential hazard within the first group, and BREAKS -- it moves to a
 * lower group only if every insn in the higher one was queued by
 * `actual_hazard`. So potential_hazard can never promote a lower-priority insn
 * over a higher one, and an explanation that needs it to is wrong on its face.
 *
 * (2) WHAT ACTUALLY PINS THE STORE is an anti-dependence, and it is the CAST's
 * fault. With `id = (s16) id` the extension's intermediate takes $v0 -- the
 * same register as the symbol's high half -- so the `sll` CLOBBERS it and the
 * store has to come first. The dump shows it: insn 12 is
 * `(set (reg $v0) (high D_8007382C))`, insn 23 is
 * `(set (reg $v0) (ashift (reg $a1) 16))`, and 23 therefore depends on 15.
 * The in-place form removes that WAR entirely, and on the sibling
 * func_8005A778 the store then DOES move -- store-last puts it after both
 * shifts. So "nothing in the source moves the store" is a property of the cast
 * form, not of the function.
 *
 * (3) BUT THE SWEEP DOES NOT PAY HERE. All five store positions were measured
 * against both extension spellings on this function: cast gives 11 / 14 / 14 /
 * 18 and in-place gives 13 / 16 / 16 / 17 / 18, so `cast_slot0` -- the shape
 * in this file -- is still the floor, and 13 is still the structurally closer
 * one. The same sweep is worth THREE WORDS on func_8005A778 (21 to 18), which
 * has a third parameter and a bound check competing for the same registers.
 * Whatever is left here is not the store's position.
 *
 * FOR REFERENCE, the `(s16)` cast version is 11 DIFF -- fewer differing words
 * but structurally further away, since its extension is not in place.
 *
 * THE OLD ANALYSIS, which the above supersedes: 11 words, one chain:
 *
 *   the extension's intermediate pseudo takes $v0 here, so the entry's -1
 *   is pushed to $a2; the listing's extension is IN PLACE in $a1, which
 *   leaves $v0 for the -1 -- and then the then-arm's own
 *   `lui $v0, %hi(D_80086A40)` CLOBBERS it, so reload's `find_equiv_reg`
 *   cannot find the value and materialises `addiu $a0, $zero, -1` fresh,
 *   which is what the listing has. With the -1 parked in $a2 instead,
 *   find_equiv_reg finds it and emits `addu $a0, $a2, $zero`, and the knock-on
 *   runs through the else's index registers as well.
 *
 *   find_equiv_reg stops at a CODE_LABEL, which is why only the then-arm --
 *   the fall-through, with no label between it and the branch -- is affected.
 *
 *   So the whole residue is: get the extension's intermediate into $a1. It
 *   cannot share with `id`'s own pseudo, because `id = (s16) id` makes that
 *   pseudo's live range continuous across the shift, and local-alloc
 *   therefore sees a conflict. All 12 orders of the four entry statements
 *   were measured: 11 DIFF is the floor, 14 and 18 the alternatives.
 *
 * func_8005A778 is the same function with a third argument and a bound check
 * and is at 101/101 words with the same residue; func_8005A90C is a fourth
 * member. One register chain unlocks three functions.
 *
 * 2026-10-28: THIS FILE NOW HOLDS THE IN-PLACE EXTENSION, 13 DIFF, and the
 * 11-DIFF cast version is recorded rather than kept. The note above already
 * said "13 is still the structurally closer one" and then kept the 11, which
 * is the opposite of the call func_800563B0 makes on the same trade ("the
 * better NUMBER is not the better SOURCE ... this file keeps the 14 and
 * records the 11"). The listing decides it in two instructions:
 *
 *     sll $a1, $a1, 16
 *     sra $a1, $a1, 16
 *
 * SAME register, which is what `id = id << 16; id = id >> 16;` emits. The
 * `(s16)` cast gives `sll $v0, $a1, 16` / `sra $a1, $v0, 16` -- two registers,
 * which the listing does not contain anywhere. Both spellings are 92 of 92
 * words, so the two differing words the cast saves are bought with an
 * extension the target does not have.
 *
 * Everything else in this file stands, AND THE STORE SWEEP DOES NOT NEED
 * RE-RUNNING -- I wrote that it did and the file already had it. The paragraph
 * above measured all five store positions against BOTH extension spellings:
 * "cast gives 11 / 14 / 14 / 18 and in-place gives 13 / 16 / 16 / 17 / 18".
 * The 13 this file now holds is the best of the five in-place positions, so
 * the sweep is complete for this base. What the switch does change is the
 * EXPLANATION: "nothing in the source moves the store" was derived from the
 * cast form's anti-dependence, where the extension's intermediate takes $v0
 * and clashes with the symbol's high half, and the in-place form has no such
 * WAR. So the store's position is still 13's, but it is no longer explained --
 * and the sibling func_8005A778, which reaches the same wall from the in-place
 * side, says store-last DOES move there. That difference between the two
 * functions is the open question.
 */

#include "common.h"

extern s32 D_8007382C[];

void func_8005A1D0(u32);
s32 func_80059E94(u32);

s32 func_8005A608(u8 *p, s32 id)
{
    s16 i;
    s16 j;
    s32 k;
    Unk80086A40 *e;
    s32 h;

    D_8007382C[0] = 0;
    i = 0;
    j = id;
    id = id << 16;
    id = id >> 16;
    if (id == -1)
    {
        do
        {
            if (D_80086A40[i].unk_00 == -1)
            {
                goto found;
            }
            i++;
        } while (i < 0x10);
        return -1;
    }
    i = j;
    if (D_80086A40[i].unk_00 != -1)
    {
        func_8005A1D0(D_80086A40[i].unk_10);
    }
found:
    k = i;
    e = &D_80086A40[k];
    e->unk_00 = i;
    e->unk_04 = p;
    e->unk_08 = (*(u16 *) (p + 0x12) << 9) + 0xA20;
    e->unk_14 = *(u32 *) (p + 0xC) - e->unk_08;
    e->unk_18 = p[0x18];
    e->unk_1B = p[0x19];
    h = func_80059E94(e->unk_14);
    e->unk_10 = h;
    if (h == -1)
    {
        return -1;
    }
    return k;
}
