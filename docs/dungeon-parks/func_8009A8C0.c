/* func_8009A8C0 (ovl_dungeon) -- PARK, best candidate so far.
   target 184 words, candidate 184 words, 43 aligned differences
   (ovlverify masked: 60 differing).

   Template: the landed func_8009A66C (csrc/ovl_dungeon.c), which is the same
   probe one layer in.  Structure is believed right: every opcode and
   displacement matches except the ones listed below.

   RESIDUE
   * Register assignment: candidate k=$s4 h=$s5 wz=$s6, target k=$s5 h=$s6
     wz=$s4.  Everything downstream of that is an R (register-only) row.
   * words 24/28: the candidate materialises `%lo(D_8008333C)` one slot early
     and fills the first `beqz`'s delay slot with `addiu $v0,-1`; the target
     fills it with the `addiu %lo` and sends that branch to a shared
     `j epilogue / addiu $v0,-1` block (.L8009AB30) instead.
   * words 55-66: the candidate reloads `d` into $a2 once and reads
     `0x88($a2)`; the target reloads `d` into $t0 at the point of use.
   * words 155/156: the target duplicates `sll $v1, $s2, 16` into the
     `beqz`'s delay slot; the candidate cross-jumps it into the shared tail
     and pays a `nop`.

   MEASURED AND REJECTED
   * x/ax and z/az as SEPARATE locals: 180 words (4 short), 102 aligned.
     Sharing one local for x/ax and one for z/az is what gives the target's
     $s1 / $s0 reuse and -- because x then needs a callee-saved register --
     what pushes `y` out of a register and into the 0x18($sp) spill the
     target has.  Worth 4 words.
   * without `(void)&d;`: 180 words / 102 aligned, with `d` living in $s8
     instead of its incoming home slot 0x50($sp).  Worth 2 words (the
     `sw $a2, 0x50($sp)` plus the reloads).
   * 4th argument of func_8009B25C as an `s16 hh` local assigned in an `if`:
     182 words, 48 aligned -- the load comes out `lhu` + `sll`/`sra` instead
     of the target's bare `lh`.  The ternary IN the argument is what gives
     `lh $a3, 0x88($t0)`.
   * tail written `if ((s16)t < 0x201) return 1; return -1;`: 182 words,
     48 aligned.  Inverted (`>= 0x201 -> return -1` first) it is 184/43.

   A BROAD PLATEAU AT 43: every one of the following compiles to the SAME
   bytes (md5 of the emitted word column identical to the base), so none of
   them is the variable that matters --
     the position of `h = *(u16 *)(d + 0x88);` (before px / after pz /
     after wz); `h` as u16 instead of s32; `h` read as `((u16 *)d)[0x44]`;
     the position of `(void)&d;` (top / before the h read / at the end);
     `u8 **pd = &d;` instead of `(void)&d;`; `w` declared last;
     `u32 k`; `(f >> 9) & 7` written inline at the two call sites;
     `s32 kk = (f >> 9) & 7; k = kk;`; `u16 f` for the first parameter.
   Also measured: `k = f >> 9; k &= 7;` is 60 aligned, `k = (f & 0xE00) >> 9`
   is 45, `sh` declared at its first use is 45, `((s16 *)D_8008333C)[10]`
   inline is 45.  All 24 permutations of the `wx/wz/k/h` declaration order
   were measured: every order with `wx` before `wz` is 43, every order with
   `wz` before `wx` is 182 words / 81 aligned.

   WHAT THE RESIDUE REDUCES TO, and it is ONE cause.  The target evaluates
   func_8009A540's first argument (`addu $a0, $s5, $zero`) in the FIRST basic
   block, at word 22, twenty-odd words and four conditional branches before
   the `jal`; the candidate emits it at word 53, in the call's own block.
   That makes $a0 busy for the whole head of the target, so the constant 1
   lands in $a1 and the `(s16 *)D_8008333C` base in $a2 -- which clobbers the
   incoming $a2 and forces the target to RELOAD `d` from 0x50($sp) at the
   `0x88` read, where the candidate still has it in $a2 and cse forwards it.
   Every remaining R row downstream is that one register rotation
   (candidate k=$s4 h=$s5 wz=$s6 against target k=$s5 h=$s6 wz=$s4).
   The target also carries an extra `addu $s5, $a0, $zero` copy at word 6
   that the candidate coalesces away.  Nothing tried reproduces the hoist.
   PERMUTER FOOD: this is a placement, not a pure allocation tie.

   ALSO MEASURED AND REJECTED, all aimed at that one placement:
   * `s16 k2 = k;` (and u16/s8/u8/s32) used as func_8009A540's first argument,
     to give the copy a source position of its own: identical bytes to the
     base for s16/s8/s32, 43 with a different allocation for u16/u8.  cse
     propagates the copy away in every case.
   * `register s32 k2 __asm__("$4");` for that same local DOES put the
     `addu $a0, $sN, $zero` at word 21 where the target has it at 22 -- but
     the `(s16 *)D_8008333C` base then lands in $t0 rather than $a2, `d` is
     still live in $a2, and the function is still 184/43.  So $a0 being busy
     is NOT what pushes the base into $a2, and the one-cause story above is
     incomplete.
   * `do { } while (0);` after `k = (f >> 9) & 7;` (the loop-NOTE lever that
     blocks optimize_reg_copy_1) is 184/56; wrapping the copy in one is 56 too.
   * `u8 *volatile d` as the third parameter -- which is the other
     `assign_parms` condition for the home slot, and which also blocks cse's
     forwarding -- is 185 words / 46, and 186/46 with `(void)&d;` as well.
     Every read of `d` becomes a load, which is one more than the target. */
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y)
{
    s16 *sh;
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

    (void)&d;
    k = (f >> 9) & 7;
    sh = (s16 *)D_8008333C;
    ax = c[0x24] + (u16)D_8006CCD8[k];
    if (ax == 0)
    {
        return -1;
    }
    if ((1 << sh[10]) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[k];
    if (az == 0)
    {
        return -1;
    }
    if ((1 << sh[11]) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if ((s16)func_8009A540(k, px >> 6, pz >> 6, h - y) == 0)
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
    if ((w & 0x3300) != 0)
    {
        if ((w & 0x40) == 0)
        {
            return -1;
        }
        t = func_800BCB04(ax, az, h - y);
        if (func_8009B25C(d, ax >> 6, az >> 6,
                          (s16)t < 0x201 ? (s16)t : *(s16 *)(d + 0x88)) != NULL)
        {
            return -1;
        }
    }
    else
    {
        t = func_800BCB04(ax, az, h - y);
    }
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}
