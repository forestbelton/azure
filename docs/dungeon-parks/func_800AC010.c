/* func_800AC010 -- state 4's per-frame step for the four-argument installer
   family. 284 target words.
   BEST: 284 words, FOUR DIFFERING (ovlverify), 3 aligned rows. THE WORD
   COUNTS MATCH and every register, immediate and displacement in the
   function is already right. The whole residue is ONE SCHEDULE
   TRANSPOSITION of two independent two-insn chains at 0x8009AC04C:

       target     0x800AC04C  lbu $v0, 0x24($s1)      <- c[0x24]
                  0x800AC050  lh  $v1, 0x2($s3)       <- b's +0x2
                  0x800AC054  sll $v0, $v0, 6
                  0x800AC058  addiu $v1, $v1, -0x20
       candidate  lh / lbu / addiu / sll  -- the two chains swapped.

   WHY IT IS STUCK. The subtrahend MUST go through its own `s32` local:
   written inline, `fold`'s associate path rewrites `A - (B - 0x20)` into
   `(A + 0x20) - B` and the `addiu` comes out `+32` on the shifted byte
   instead of `-0x20` on the loaded halfword. But the local's assignment is
   then the EARLIER statement, so its two insns get the lower INSN_LUID, and
   sched.c's `rank_for_schedule` falls through to `INSN_LUID (tmp) -
   INSN_LUID (tmp2)` on the tie (both chains have equal priority and equal
   class) -- so the halfword chain is emitted first. Reading `c[0x24]` into
   a local first puts the chains in the target's order but costs a word:
   every two-local spelling measured is 285 words.

   SPELLINGS MEASURED (all against 284 target words):

   * `x = *(s16*)((u8*)b+2) - 0x20;` then `((c[0x24]<<6) - x)` ....... 284 / 4   (this body)
   * fully inline, no local ......................................... 284 / 4, but the
     differing words are the folded `addiu $v0,$v0,32` pair, not a transposition
   * `(u32)` cast as a fold barrier ................................. 285 / 7  (and the
     division comes out `divu`)
   * `*(volatile s16 *)` as a fold barrier .......................... 289 / 30
   * `x = c[0x24] << 6; y = <field> - 0x20;` (two locals) ........... 285 / 261
   * `x = c[0x24]; y = <field> - 0x20;` (two locals) ................ 285 / 269
   * `b->unk_0C = ((x = c[0x24]<<6) - (<field> - 0x20)) ...` ......... 285 / 261
   * `b->unk_0C = ((c[0x24]<<6) - (x = <field> - 0x20)) ...` ......... 284 / 4 (same residue:
     gcc still expands the side-effecting operand first)
   * `0x20 - <field>` then `+` ...................................... 285 / 36
   * `c[0x24] * 64` instead of `<< 6` ............................... 284 / 4 (no change)
   * a shared `s16` divisor local for both divides .................. 283 / 6
   * declaring `x` first / last among the locals .................... 284 / 4 (no change)

   THE THREE DEVICES THAT GOT IT FROM 283/50 TO 284/4, each measured:

   1. `goto fail;` IN THE SECOND func_8009B88C ARM. Both arms end in the same
      `func_800A2B04` / `func_800AA5E4` / `return 0` tail. Written out twice,
      jump.c's forward scan always deletes the FIRST copy -- `do_cross_jump`
      deletes the insns before the jump it is currently scanning, and the
      forward scan reaches arm 1's `j` first -- so the shared tail lands at
      the END of the function where the target has it at 0x800AC1C0. Both
      arms spelled `== 0`: 282 words / 38. Arm 1 `== 0`, arm 2 `!= 0`:
      282 / 33. With the label in arm 1 and `goto fail` in arm 2: 282 / 12.
   2. A TRAILING `out: return 0;`. The `*(s16 *)(a + 0x96) = 2` arm's
      `return 0` must cross-jump onto a one-insn `v0 = 0` block that falls
      into the epilogue; that is what makes the final `return 1` need its own
      `j .L800AC458`. Worth the last TWO WORDS: 282 -> 284.
   3. `s32 func_8009B164s(s16, UnkVec *, u8 *) __asm__("func_8009B164");`
      The landed definition takes `u16` (its own `(a >> 9) & 7` needs the
      logical shift), and through it the argument load is `lhu` where the
      target has `lh`. The alias is worth one word-position.

   NEW DECLARATION THIS BODY NEEDS (top of file, see the report):
       s32 func_8009B164s(s16, UnkVec *, u8 *) __asm__("func_8009B164");

   MUST STAY K&R: `s32 func_800AC010();` is declared at the top of the unit
   and two landed call sites (func_800AF2xx and func_800D-something, the
   `if (func_800AC010() != 0)` pair) pass NO arguments, so an ANSI
   four-parameter definition makes both "too few arguments" -- the same
   constraint func_800AC82C records.

   PERMUTER TRIAGE: word counts MATCH, and the four differing words are an
   INSTRUCTION ORDER transposition, not register numbers -- the permuter
   reorders statements, so this is worth a run. */

s32 func_800AC010(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *d;
    u8 *g;
    u8 *h;
    s32 x;
    s16 o0;
    s16 o1;
    s16 t;

    if (*(s16 *)(a + 0x96) != 0)
    {
        x = *(s16 *)((u8 *)b + 0x2) - 0x20;
        b->unk_0C = (((c[0x24] << 6) - x) << 16) / *(s16 *)(a + 0x96);
        x = *(s16 *)((u8 *)b + 0x6) - 0x20;
        b->unk_10 = (((c[0x25] << 6) - x) << 16) / *(s16 *)(a + 0x96);
    }
    *(s16 *)(a + 0x96) = *(u16 *)(a + 0x96) - 1;
    if (*(s16 *)(a + 0x96) > 0)
    {
        return 0;
    }
    d = (u8 *)func_8009C93C(p, c, *(s16 *)(p + 0x6A), 0, 0);
    if (d != NULL)
    {
        func_8009CE1C(p, 4, 1, 8, (s16)(*(u16 *)(p + 0x6A) + 0x800), 0, 1);
        func_8009CE1C(d, 8, 1, 8, *(s16 *)(p + 0x6A), 0, 1);
        *(u16 *)(a + 0x98) &= 0xFFF7;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s32 *)(p + 0x1C) |= 0x40000000;
        a[0x9C] = c[0x26];
        c[0x26] = func_8009FB34(c[0x24], c[0x25]);
        func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
        if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
        {
        fail:
            func_800A2B04(b, c[0x24], c[0x25]);
            func_800AA5E4(a, b, c, p);
            return 0;
        }
        c[0x24] = o0;
        c[0x25] = o1;
        func_800A2B04(b, c[0x24], c[0x25]);
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s32 *)(p + 0x1C) &= 0xFFDFFFFF;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(s16 *)(a + 0x96) = 0;
        p[0x71] = 0;
        return 1;
    }
    if ((s16)func_8009B164s(*(s16 *)(p + 0x6A), b, c) != 0)
    {
        *(s16 *)(p + 0x8A) = *(u16 *)(p + 0x8A) - 1;
        if (*(s16 *)(p + 0x8A) > 0)
        {
            c[0x24] += *(u8 *)&D_8006CCD8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            c[0x25] += *(u8 *)&D_8006CCE8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            *(s16 *)(a + 0x96) = 2;
            goto out;
        }
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    *(u16 *)(a + 0x98) &= 0xFFF7;
    a[0x9C] = c[0x26];
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
    t = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                      (s16)(*(u16 *)((u8 *)b + 0xA) - 0x20));
    if ((s16)t < 0x200)
    {
        *(s16 *)(a + 0x92) = 0;
        *(s16 *)(p + 0x88) = t;
    }
    h = (u8 *)&D_80083460;
    *(u16 *)(h + 0xA) -= 1;
    *(s16 *)(a + 0x96) = 0;
    *(s32 *)(p + 0x1C) = (*(s32 *)(p + 0x1C) | 0x40000000) & 0xFFDFFFFF;
    if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
    {
        goto fail;
    }
    c[0x24] = o0;
    c[0x25] = o1;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0x71] = 0;
    return 1;
out:
    return 0;
}
