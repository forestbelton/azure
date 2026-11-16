/* func_8009DAD8 -- the dungeon minimap/radar draw. 269 target words.
   BEST: 267 words, 34 aligned differences (aligndiff). THE WORD COUNTS DO
   NOT MATCH: the candidate is TWO WORDS SHORT, and both missing words are
   register-to-register COPIES that this build coalesces away and the target
   keeps. Every opcode, immediate and displacement elsewhere is right; the
   remaining 32 rows are a permutation of the callee-saved registers that
   falls out of those two copies.

   THE TWO MISSING WORDS, by address:

   1. 0x8009DC20  `addu $fp, $s2, $zero` -- the preheader copy of
      &D_80082E80. In the target `n` (&D_80082E80, $s2) DIES at this copy,
      so it is live only across the first func_8009DA70 call and scores a
      short live range; the loop body then reads `0x26($fp)` off a second
      pseudo that loop.c hoisted into the preheader. Every spelling measured
      keeps ONE pseudo for both uses, which is why `ot` lands in $fp here and
      in $s7 in the target.

   2. 0x8009DCE8  `addu $s0, $a0, $zero` -- the copy of the 0/1/2 kind value
      into `mode`. The target computes the if-chain into $a0 (call-clobbered,
      dead before the call) and copies it into $s0; reorg then duplicates
      that copy into the 0x8009DCDC branch's delay slot, where this candidate
      has a `nop`. One pseudo gets $s0 directly here and the copy disappears.

   SPELLINGS MEASURED AND REJECTED (all on the 269-word target):

   * `n` local for both the pre-loop reads and the in-loop +0x26 ...... 267 / 34  (this body)
   * `D_80082E80[0x26]` in the loop, `n` outside ...................... 266 / 35
   * array refs with no `n` local at all ............................. 266 / 35
   * second local `m = n;` before the loop, used in the loop ......... 267 / 43
   * second local `m = n;` as the first statement INSIDE the loop ..... 267 / 34 (coalesced, no change)
   * `k = 0; ...chain...; mode = k;` (two locals for the kind) ........ 267 / 34 (coalesced, identical output)
   * `col[1] = 0;` BEFORE the for-init ............................... 267 / 37
     (that one is fixed in this body: the store has to sit AFTER
      `p = *(u8 **)(head + 0x5C) + 0x20;` so reorg fills the guard's delay
      slot with it instead of stealing the post-loop `i = 0`; worth 3 rows
      and the loop-bottom `nop`.)
   * `if (p[0xAD] == 0) mode = 1; else mode = 2;` (arms the other way) . 267 / 37
   * col[] stores written ascending 0..3 ............................. 267 / 37

   WHAT IS ESTABLISHED AND SHOULD NOT BE RE-DERIVED:

   * The `s16 func_8009FD40(u8 *, u8 *);` FORWARD DECLARATION IS REQUIRED and
     is worth TWO WORDS. func_8009FD40 is defined BELOW this function, so the
     call here otherwise rides an implicit `int f()` and the `sll 16`/`sra 16`
     pair in front of the `slti` is absent. Add it to the declaration block.
   * The second parameter is UNUSED -- $a1 is never read. func_8009DF0C (landed,
     just below) calls `func_8009DAD8(r, p)` with two arguments, so the
     definition must take two or that call stops compiling.
   * The ring walk is a `for` with the advance in the THIRD CLAUSE, not a
     `while` with the advance in the condition (which is what the sibling
     func_8009F0B4 needs): the target has the duplicated exit test and a
     preheader, and the `continue`s land on the advance.
   * `g = (u8 *)&D_80083160;` is a local of its own, assigned at the top,
     before the five flag gates -- the `lhu 0x4($s1)` base shape.
   * `head = D_800E3D7C;` is read into a local BEFORE the five flag gates;
     those five call out and would otherwise force a reload.
   * The four sweeps are SUBSCRIPTED (`D_800E3548_s[i]`, `D_800E36C8[i]`,
     `D_800E3CD8[i]`, `D_800E3648_s[i]`, `D_800E39C8[i]`), the same shape
     func_8009F020/func_8009EF78 needed; the pointer-stride givs are loop.c's.
   * The D_800E3CD8 sweep passes `(u16)` casts to func_8009DA70 (`lhu`) and the
     plain `s16` fields to func_8009EE4C (`lh`).

   PERMUTER TRIAGE: the counts do NOT match -- two words short, both inside
   proved blocks -- so by docs/dungeon-sweep.md's rule this is permuter food
   rather than .greg work. */

void func_8009DAD8(u8 *ot, u8 *unused)
{
    u8 col[4];
    u8 *g;
    u8 *n;
    u8 *p;
    u8 *q;
    u8 *head;
    s32 c;
    s32 i;
    s32 t;
    s32 k;
    s32 mode;

    g = (u8 *)&D_80083160;
    head = D_800E3D7C;
    if ((D_800E296C & 0x1000) != 0)
    {
        D_800E296C &= ~0x1000;
        func_8009F13C();
    }
    if ((D_800E296C & 0x400) != 0)
    {
        D_800E296C &= ~0x400;
        func_8009F0B4();
    }
    if ((D_800E296C & 0x200) != 0)
    {
        D_800E296C &= ~0x200;
        func_8009F020();
    }
    if ((D_800E296C & 0x800) != 0)
    {
        D_800E296C &= ~0x800;
        func_8009EF78();
    }
    if ((D_800E296C & 0x100) != 0)
    {
        D_800E296C &= ~0x100;
        func_8009EF04();
    }
    c = (func_800644B8(*(u16 *)(g + 0x4) << 8) >> 6) + 0x80;
    if (c >= 0x100)
    {
        c = 0xFF;
    }
    n = D_80082E80;
    col[3] = 0x68;
    col[2] = 0;
    col[1] = c;
    col[0] = c;
    func_8009DA70(n[0x24], n[0x25], (s32 *)col, (s32)ot);
    p = *(u8 **)(head + 0x5C) + 0x20;
    col[1] = 0;
    for (; p != head; p = *(u8 **)(p + 0x5C) + 0x20)
    {
        if (*(s8 *)(p + 0x13) > 0)
        {
            q = *(u8 **)(p - 0x14);
            if ((D_800E296C & 4) == 0)
            {
                if (*(s8 *)(q + 0x26) < 0 || *(s8 *)(q + 0x26) != *(s8 *)(n + 0x26))
                {
                    if (func_8009FD40(q, D_80082E80) >= 4)
                    {
                        continue;
                    }
                }
            }
            k = 0;
            t = p[0x13];
            if (t == 0x1E)
            {
                if (p[0xAD] != 0)
                {
                    k = 2;
                }
                else
                {
                    k = 1;
                }
            }
            else if (t != 0x23 || *(s16 *)(p + 0xA6) == 0 || (D_800E296C & 4) != 0)
            {
                k = 1;
            }
            mode = k;
            if (mode == 0)
            {
                continue;
            }
            if (mode == 2)
            {
                col[2] = c;
                col[0] = 0;
            }
            func_8009DA70(q[0x24], q[0x25], (s32 *)col, (s32)ot);
            if (mode == 2)
            {
                col[2] = 0;
                col[0] = c;
            }
        }
    }
    col[2] = c;
    col[0] = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if ((D_800E296C & 2) != 0 ||
                func_8009EE4C(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01) != 0)
            {
                func_8009DA70(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01, (s32 *)col, (s32)ot);
            }
        }
    }
    col[2] = 0;
    col[1] = c;
    for (i = 0; i < 4; i++)
    {
        if (D_800E3CD8[i].unk_00 != 2)
        {
            break;
        }
        if ((D_800E296C & 1) != 0 ||
            func_8009EE4C(D_800E3CD8[i].unk_02, D_800E3CD8[i].unk_04) != 0)
        {
            func_8009DA70((u16)D_800E3CD8[i].unk_02, (u16)D_800E3CD8[i].unk_04, (s32 *)col,
                          (s32)ot);
        }
    }
    col[1] = 0x20;
    col[2] = c;
    col[0] = c;
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
            (D_800E3648_s[i].unk_03 & 0x40) == 0)
        {
            if ((D_800E296C & 8) != 0 ||
                ((D_800E3648_s[i].unk_03 & 0x80) == 0 &&
                 func_8009EE4C(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07) != 0))
            {
                func_8009DA70(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, (s32 *)col, (s32)ot);
            }
        }
    }
}
