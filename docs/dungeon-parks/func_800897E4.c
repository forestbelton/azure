/* 175 words, 0 differing.  Three alternative loops over the same pair of
   arrays: D_80083360[i] is a handler pointer and D_800833E0[i] the object it
   runs on; the two flag tests pick which handlers are allowed to run and what
   happens after each call.  Four spellings were worth 157 of the 175 words:

   1. EACH LOOP INDEXES THE ARRAYS; IT DOES NOT WALK POINTERS.  loop.c turns
      `D_80083360[i]` into the pointer bivs the listing shows, and in the
      first two loops it then ELIMINATES `i`, rewriting the exit test as
      `addiu $v0, $s3, 0x80` / `slt $v0, $s0, $v0` -- a SIGNED compare, which
      a pointer compare (`sltu`) can never give.  maybe_eliminate_biv puts
      that `addiu` inside the loop rather than the preheader exactly when
      `threshold < insn_count`, and the pre-strength-reduction insns are also
      what keep `insn_count` above move_movables' threshold, so the
      `lui %hi(D_800DCF80)` and `lui %hi(D_80083160)` stay inside their loops.
      Hand-written pointer walks measured 174 words / 34 aligned rows: the
      bodies were right and every one of those rows was a hoisted `lui` or
      the register numbering that followed from it.

   2. `D_80083360[i] = 0` IS THE `else` OF `if (s != NULL)`, NOT THE `if` OF
      `if (s == NULL)`.  Writing it the other way puts the store inline and
      costs a `j` over it -- and, because the store then no longer sits
      between the call and the shared `addiu $s0, $s0, 0x4`, the call path
      reaches that increment by fall-through instead of by `j .L800898B8`,
      which is where ten of the words went.  reorg steals the increment into
      the jump's delay slot, and that -- not a failure to cross-jump -- is why
      the target seems to emit the increment twice per loop.  165 words.

   3. `cam` AND `s` ARE ONE VARIABLE.  Loop 2 overwrites the object pointer
      with `&D_80083498` on the match path, so the single pseudo is live
      across a call and takes a callee-saved register ($s1) in all three
      loops.  A separate `cam` local leaves `s` in $a0 everywhere: 16 rows.

   4. LOOP 2'S `a2` AND LOOP 1'S `a` MUST BE DIFFERENT LOCALS, and `a2` must
      be set at the top of loop 2's body.  loop.c then hoists it as $s5
      (%hi) / $s6 (the address), cse2 folds the `D_800DCF80[0]` load's own
      `lui` onto $s5, and $s5 is the SEVENTH callee-saved register the target
      saves -- the last two words.  Sharing one local costs 18 rows; setting
      it beside its use costs the hoist and 2 words.

   No prototype for this function: csrc/ovl_dungeon.c:2075 calls it seven
   lines above, on an implicit `int` declaration, and that is load-bearing. */
void func_800897E4(void)
{
    u32 *a;
    u32 *a2;
    u32 *b;
    u32 *tbl;
    u8 *s;
    void (*h)(u8 *, s32, s32);
    s32 save;
    s32 i;

    if ((D_800E296C & 0x2000000) != 0)
    {
        tbl = D_800DCFA4;
        for (i = 0; i < 0x20; i++)
        {
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        a = D_800DCF80;
                        b = tbl;
                        for (;;)
                        {
                            if (*b == (u32)h)
                            {
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                break;
                            }
                            if (*a == 0)
                            {
                                break;
                            }
                            a++;
                            b++;
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
    else if ((*(s32 *)(D_800814A8 + 0x1C) & 0x10) != 0)
    {
        for (i = 0; i < 0x20; i++)
        {
            a2 = D_800DCF80;
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        b = a2 + 1;
                        if (D_800DCF80[0] == (u32)h)
                        {
                            s = D_80083498;
                            if (*(s32 *)(s + 0x14) != 0)
                            {
                                save = *(s32 *)(s + 0x18);
                                *(s32 *)(s + 0x18) = 0;
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                *(s32 *)(s + 0x18) = save;
                            }
                        }
                        else
                        {
                            for (;;)
                            {
                                if (*b == (u32)h)
                                {
                                    h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                    break;
                                }
                                if (*b == 0)
                                {
                                    break;
                                }
                                b++;
                            }
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
    else
    {
        for (i = 0; i < 0x20; i++)
        {
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                        if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
                        {
                            return;
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
}
