/* func_80041284 (unk_14934) -- partial. Arm 2 and both register copies are
 * exact; arm 1 has the store and the `*p` reload in the wrong order.
 *
 * Only the fragment that matters is reproduced here, not a compilable
 * function: see docs/open-matching-problems.md for the full write-up of what
 * is left and the list of shapes already ruled out.
 *
 * The mechanism this fragment exists to record: writing the assignment as a
 * chained store whose value is wanted sends it through store_expr's
 * copy_to_reg, so the RTL is load-to-temp / store / copy. combine cannot fold
 * the load into the copy because the store sits between them
 * (use_crosses_set_p via mem_last_set), sched1 then lifts the copy above the
 * store, and local-alloc rewrites the store's source to x. That is what
 * produces the target's `lw $v1 / or $s0 / addu $s1,$v1,$zero / lui $at /
 * sw $s1` in arm 2 and the empty `jal func_8003F320` delay slot.
 *
 * Note that csrc/unk_14934.c already uses `D_80081480 = D_8008148C;` in four
 * matched functions, so a declaration change for either symbol would risk
 * those.
 */

    if (x == 0) {
        x = D_80081480 = D_8008148C;
        y = x + ((*p & 0xFF800000) >> 12);
    } else {
        x |= 0x80000000;
        y = D_80081480 = D_8008148C;
    }
