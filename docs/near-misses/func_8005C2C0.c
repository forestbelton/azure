/* func_8005C2C0 (unk_1C540) -- 132 of 132 words, 10 DIFF. NOT compiled.
 *
 * Starts a voice: claims the D_80085458 entry for channel `b`, fills it in,
 * and keys the note on through func_8005EC0C with a stack SpuVoiceAttr.
 *
 * The struct is the real Psy-Q `SpuVoiceAttr`, which is why `libspu.h` is now
 * in include/common.h. Two witnesses, not one: the offsets written match the
 * type field for field (0x00 voice, 0x04 mask, 0x08/0x0A volume, 0x0C/0x0E
 * volmode, 0x16 note, 0x18 sample_note, 0x1C addr, 0x24/0x28/0x2C the three
 * ADSR modes, 0x30..0x38 ar/dr/sr/rr/sl), AND the mask 0x1FFEF is a valid
 * SPU_VOICE_* bitmask: everything from VOLL to LSAX except bit 4, PITCH --
 * which is exactly the field this function leaves alone because it sets
 * `note` instead.
 *
 * EVERY instruction matches. What is left is where sched2 puts ten of them in
 * the entry block; see the last section.
 *
 * Four things are paid for here, three of them reusable:
 *
 *   * `j = a; a = a << 16; a = a >> 16;` -- the in-place sign-extension of
 *     matching-levers section 7, but with `j` declared `s16`, not `s32`.
 *     That matters: `s32 j = a` is a COPY, so cse makes j and a one quantity
 *     and canon_reg rewrites `a << 16` into `j << 16` -- the extension then
 *     reads the callee-saved copy and `a` picks up a fourth reg_n_refs, which
 *     is enough to outrank `c` and swap their hard registers. `s16 j = a` is
 *     a TRUNCATION, a different expression, so cse leaves the shift on the
 *     parameter and it extends in place in $a0 while $s5 keeps the raw.
 *
 *   * `q = D_80085458; p = q + b * 0x78;` -- a pointer VARIABLE for the base,
 *     not the symbol inline. `D_80085458 + b * 0x78` has an ADDR_EXPR
 *     operand, which is TREE_CONSTANT, so fold swaps it to the right and the
 *     index chain expands FIRST. sched1 then lifts the three index shifts
 *     into the shadow of the `lw 0x10($v0)` that loads st.addr, so the
 *     combined index/p quantity is born while $v0 still holds the addr and
 *     local-alloc gives p $v1 instead of $v0 -- 26 words of register swap
 *     through the whole entry block. With `q` the symbol is a variable, fold
 *     leaves it first, and the symbol's lui/addiu fills that shadow instead.
 *
 *   * `st.volume.left = *(u16 *) (p + 0x10);` -- read BACK from the entry,
 *     not `f << 7` spelled again. cse replaces the load with the register
 *     that was stored, and that replacement is a reg-to-reg MOVE: the
 *     listing's `addu $a0, $v1, $zero`, a word this function is otherwise
 *     short. Written as the expression, cse shares the shift's pseudo and
 *     there is no move at all. Note the asymmetry the listing shows and this
 *     source reproduces: +0x12 is NOT forwarded and keeps its `lhu`.
 *
 *   * The four constant bytes go in CHANNEL-INTERLEAVED order --
 *     0x14, 0x16, 0x15, 0x17, not 0x14, 0x15, 0x16, 0x17 -- which is worth
 *     16 words. It decides which of the two constants sched1 hoists: with
 *     0x7F's pseudo born first, sched1 lifts it all the way into the
 *     st.addr load's shadow, its live range then crosses the 0x11 and 0x40
 *     pseudos in $v1 and local-alloc gives it $a0, which in turn frees $v1
 *     for `f << 7` early. Written in address order the 0x7F lands in $v1 at
 *     its use and everything after it shifts by one slot. This is the least
 *     convincing of the four as original source; the next person should
 *     treat it as a symptom and look for whatever else makes 0x7F's pseudo
 *     the oldest.
 *
 * Two shapes worth keeping even though they read oddly:
 *   * `(u16) d != 0` with `d` declared `s32`. A `u16` parameter is assumed
 *     already zero-extended in its incoming register, so `d != 0` is a plain
 *     `bnez` (29 DIFF) -- the listing's `andi $v0, $s3, 0xFFFF` needs the
 *     cast on a wide parameter. `d & 0xFFFF` is identical; `(s16) d` is one
 *     word worse.
 *   * `return b;` from an `s16` function, which is the epilogue's
 *     `sll`/`sra` on $s2.
 *
 * WHAT IS LEFT -- ten words, all of them sched2's entry block, no instruction
 * different. The listing interleaves
 *
 *     sw $s2 / move $s2 | lui %hi(D_80085F98) | sw $s6 / lhu $s6 | li 1
 *       | sw $s5 / move $s5 | sw $s4 / move $s4 | sw $s3 / move $s3
 *       | sw $s7 / lhu $s7 | sll | sw $fp / lhu $fp | sra | sw
 *
 * and this source gives the same 25 insns with {lui, $s6 pair, li 1} and
 * {$s4 pair, $s3 pair} swapped, and the two shifts six slots earlier. The
 * cause is in the dump: sched1 raises insn 14 (`lhu $s6`, the first stack
 * parameter), insn 28 (the %hi) and insn 31 (the constant 1) to
 * LAUNCH_PRIORITY as BIRTHING INSNS -- destination live, reg_n_sets == 1 --
 * and the backward pass then places them as late as it can. In the listing
 * they are early, so in the original at least one of those three pseudos is
 * set more than once. Nothing tried moves it: statement order does not
 * (sched1 normalises it), nor the three-variable extension, nor reading
 * st.note back from the entry, nor `d`'s type.
 *
 *   python3 tools/decomp/sched.py cand.c func_8005C2C0            # sched2
 *   python3 tools/decomp/sched.py cand.c func_8005C2C0 --insn 14
 */


void func_80056DB4(s32);
void func_8005E97C(s32, u32);
s32 func_8005EB78(u32);
void func_8005EC0C(SpuVoiceAttr *);

s16 func_8005C2C0(s32 a, s32 b, s32 c, s32 d, u16 e, u16 f, u16 g)
{
    SpuVoiceAttr st;
    u8 *q;
    u8 *p;
    s32 t;
    s16 j;

    D_80085F98[0] = 1;
    j = a;
    a = a << 16;
    a = a >> 16;
    if (a != -1 && b < 0x18)
    {
        func_80056DB4(b);
        do
        {
            func_8005E97C(0, D_80073740[b]);
            t = func_8005EB78(D_80073740[b]);
        } while (t != 2 && t != 0);

        st.mask = 0x1FFEF;
        st.a_mode = 1;
        st.s_mode = 1;
        st.r_mode = 3;
        st.ar = 0;
        st.dr = 0;
        st.sr = 0;
        st.rr = 0;
        st.sl = 0;
        st.volmode.left = 0;
        st.volmode.right = 0;
        st.voice = D_80073740[b];
        st.addr = D_80086A40[j].unk_10;

        q = D_80085458;
        p = q + b * 0x78;
        *(u16 *) (p + 0x06) = 0x11;
        *(u8 *) (p + 0x14) = 0x7F;
        *(u8 *) (p + 0x16) = 0x40;
        *(u8 *) (p + 0x15) = 0x7F;
        *(u8 *) (p + 0x17) = 0x40;
        *(u16 *) (p + 0x10) = f << 7;
        *(u16 *) (p + 0x00) = b;
        *(u16 *) (p + 0x1A) = 1;
        *(u8 *) (p + 0x22) = c;
        *(u8 *) (p + 0x23) = d;
        *(u16 *) (p + 0x0A) = e;
        *(u16 *) (p + 0x5C) = j;
        *(u16 *) (p + 0x12) = g << 7;

        st.volume.left = *(u16 *) (p + 0x10);
        st.volume.right = *(u16 *) (p + 0x12);
        st.note = e;
        if ((u16) d != 0)
        {
            st.sample_note = ((c - 1) << 8) | (0x7F - d);
        }
        else
        {
            st.sample_note = d | (c << 8);
        }
        func_8005EC0C(&st);
    }
    D_80085F98[0] = 0;
    return b;
}
