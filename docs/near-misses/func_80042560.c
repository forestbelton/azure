/* func_80042560 (unk_14934) -- partial. The mixed addressing is solved; loop
 * 1's store/load order and three $s registers are not.
 *
 * Only the fragment that matters is reproduced here, not a compilable
 * function: see docs/open-matching-problems.md for the full write-up and the
 * list of shapes already ruled out.
 *
 * The addressing result is the part worth keeping, and it is genuinely mixed
 * within the one function: a constant base pointer for the first two accesses
 * to 0x80013714 and the plain `extern u16 D_80013714` scalar for the third.
 * That gives `lui $v1,0x8001 / lhu $s4,0x3714($v1) / ... / sh $v0,0x3714($v1)`
 * for the first two and the assembler-expanded `lui $at / sh $s4,0x3714($at)`
 * for the third, matching all five instructions.
 *
 * `*(u16 *) 0x80013714` instead gives lui+ori plus a `0($v1)` displacement,
 * and declaring `extern u16 D_80013714[]` splits all three accesses and
 * rematerialises the third into $v0.
 */

    u8 *base = (u8 *) 0x80010000;

    saved = *(u16 *) (base + 0x3714);
    *(u16 *) (base + 0x3714) = saved | 2;
    /* ... */
    D_80013714 = saved;
