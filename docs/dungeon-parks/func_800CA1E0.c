/* PARK func_800CA1E0 -- 151 of 153 words, 17 aligned differences (j40, 2026-11-16).

   Shape: func_8009A66C's twin (csrc/ovl_dungeon.c, 149 words, landed) with a
   map-bounds pre-check in front of it and a 1/-1 tail instead of 1/2/3.
   Probe the tile one step along the facing in `f`: reject the step if the
   target cell is 0 or past the map's 1<<unk_1F0 / 1<<unk_1F2 extent, ask
   func_8009A540 whether the step is legal, func_8009A350 for the tile's flag
   word, func_8009FB34/D_800E2970 for the room gate, and func_800BCB04 for
   the floor height.  Returns -1 blocked, 0 cannot, 1 ok.

   RESIDUE -- exactly two words, both in the prologue's handling of `k`:
     * 0x800CA1F8  addu $s5, $a0, $zero   (the target computes `(f >> 9) & 7`
                   into a SHORT-LIVED pseudo in $a0 and copies it into the
                   long-lived $s5; the candidate's `andi` writes the
                   long-lived register directly)
     * 0x800CA238  addu $a0, $s5, $zero   (that value copied BACK into $a0 in
                   the ENTRY basic block, 40 instructions before the
                   func_8009A540 call it feeds -- so the insn is in block 1 in
                   the RTL, not hoisted; the candidate emits its one
                   `move $a0, k` in the call's own block, where it also fills
                   the delay slot the target leaves as a `nop` at 0x800CA2B0)
   Every other row aligns and 15 of the 17 are register-number-only.

   MEASURED AND REJECTED (aligndiff rows at 150-152 words):
     * THE BOUNDS CHECK AND THE STEP TARGET ARE THE SAME TWO LOCALS.  Writing
       `tx`/`tz` for the pre-check and `ax`/`az` for `D_800DCEAC[k] + wx` is
       81 rows; reusing `ax`/`az` for both is 26.  That is the single biggest
       lever here: it is what puts the two sums in CALLEE-SAVED registers
       ($s1/$s0) and forces the `s16 y` parameter out to the `sh $a3,
       0x18($sp)` spill slot the target has, which is 2 of the 4 words the
       first draft was missing.
     * THE TAIL IS `if (t >= 0x201) return -1; return 1;`, NOT
       `if (t < 0x201) return 1; return -1;`.  With the `return -1` last,
       cross-jumping merges all five `return -1` bodies into one block at the
       end of the function, test 4's branch is inverted, and the target's
       shared `.L800CA2B4: j .L800CA410 / addiu $v0, $zero, -1` block
       disappears: 150 words / 26 rows.  With `return -1` first it is
       151 / 17.  `r = -1; if (t < 0x201) r = 1; return r;` is 152 / 30 and
       `return t < 0x201 ? 1 : -1;` is 152 / 30 (both leave a `move $v0,$v1`).
     * the two bounds tests as FOUR separate `if`s: 28 rows (vs 17 for two
       `if`s with `||`).  As ONE `if` with four `||` terms and the `az`
       assignment inside the condition: 83 rows.
     * `m` must be a LOCAL `u8 *m = (u8 *)D_8008333C;`.  Spelling the two
       reads `*(s16 *)((u8 *)D_8008333C + 0x14)` or `((s16 *)D_8008333C)[0xA]`
       folds the displacement into the relocation and costs the shared base
       register: 152 words / 24 rows.
     * `h = *(u16 *)(d + 0x88)` hoisted above the bounds checks: 150 / 34.
     * for the two missing words, all of these were measured and are
       BYTE-IDENTICAL to the 17-row body (cse collapses every one):
       `kk = (f >> 9) & 7; k = kk;`;  `dir = k;` with `dir` passed to
       func_8009A540 and/or func_8009A350;  `k = f >> 9; k &= 7;` split;
       `k` declared `u32` / `s16`;  `f = (f >> 9) & 7; k = f;`;
       `k = (f >> 9) & 7; f = k;`;  no `k` at all with `(f >> 9) & 7`
       written out at all five use sites;  and swapping the order of the
       `k` / `m` / `ax` statements.  Reusing the parameter outright
       (`f = (f >> 9) & 7;` and `f` everywhere) is 45 rows.

   Two words of allocator noise inside one 153-word function whose every
   other word aligns -- good permuter food. */
s32 func_800CA1E0(u32 f, u8 *c, u8 *d, s16 y)
{
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 h;
    s32 t;
    s16 n;
    u8 *m;

    k = (f >> 9) & 7;
    m = (u8 *)D_8008333C;
    ax = c[0x24] + (u16)D_8006CCD8[k];
    if (ax == 0 || (1 << *(s16 *)(m + 0x14)) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[k];
    if (az == 0 || (1 << *(s16 *)(m + 0x16)) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if (func_8009A540(k, px >> 6, pz >> 6, h - y) == 0)
    {
        return 0;
    }
    ax = D_800DCEAC[k] + wx;
    az = D_800DCEBC[k] + wz;
    func_8009A350(c[0x24], c[0x25], k, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k]);
        if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 && (*(s32 *)(d + 0x1C) & 0x2000) == 0)
        {
            return 0;
        }
    }
    t = func_800BCB04(ax, az, h - y);
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}
