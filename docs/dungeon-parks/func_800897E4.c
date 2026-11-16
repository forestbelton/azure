/* PARK: func_800897E4, 165 of 175 words, 103 aligned differences.
   ovl_dungeon.  2026-11-16.  **This is a first compile of a complete reading,
   not a worked candidate** -- the decode below is the expensive part and is
   worth trusting; the loop SHAPES are not yet right and that is what the 12
   words and most of the 125 rows are.

   TWO NEW DECLARATIONS (beside the existing `extern u32 D_800E296C;`):

       extern u32 D_800DCF80[];
       extern u32 D_800DCFA4[];

   `D_80083360[0x20]` and `D_800833E0[0x20]` are already declared with their
   sizes in include/structs.h:1814-1815, and the size matters: the loops bound
   themselves at `+0x80`.

   **DO NOT ADD A PROTOTYPE.**  csrc/ovl_dungeon.c:2075 calls `func_800897E4()`
   seven lines ABOVE the INCLUDE_ASM, so the call rides an implicit `int`
   declaration.  The unit already carries that pattern elsewhere
   (func_800DC888), and docs/matching-levers.md records that silencing the
   warning can break a match.

   ------------------------------------------------------------- WHAT IT IS

   One function, three alternative loops over the same pair of arrays:
   `D_80083360[i]` holds a handler pointer and `D_800833E0[i]` the object it
   runs on.  Each loop body is the same four steps --

       h = D_80083360[i];            if (h == NULL)            -> next
       s = (u8 *)D_800833E0[i];      if (s == NULL)  D_80083360[i] = 0, next
       if (*(u16 *)(s + 0x1E) & 0x800)                         -> next
       h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));

   -- and they differ only in which handlers are allowed to run and what
   happens after the call:

   * `(D_800E296C & 0x2000000) != 0` -- the handler must appear in
     `D_800DCFA4[]`, searched in step with `D_800DCF80[]`, stopping when the
     D_800DCF80 entry is 0.  Two pointers walk the two tables together.
   * else `(*(s32 *)(D_800814A8 + 0x1C) & 0x10) != 0` -- if the handler is
     `D_800DCF80[0]` it is run against **D_80083498** (not the object), gated
     on that record's +0x14, with its +0x18 saved, zeroed across the call and
     restored.  Otherwise the handler is searched for in `D_800DCF80[1..]`,
     stopping at a 0 entry, and run against the object.
   * else -- run every handler unconditionally, and after each call
     `if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0) return;`

   The third loop counts with an INDEX (`slti $v0, $s3, 0x20`); the first two
   bound a POINTER against `base + 0x80`.  That is a real difference in the
   source, not scheduling.

   -------------------------------------------------- WHAT IS WRONG, AND WHY

   FIXED ALREADY, and it is in the body below: in the first two loops the
   target recomputes the bound INSIDE the loop --

       addiu $v0, $s3, 0x80
       slt   $v0, $s0, $v0

   -- keeping `&D_80083360` in `$s3` across the whole loop, while the literal
   `D_80083360 + 0x20` in the exit test lets loop.c hoist it into the
   preheader.  **A `base` local that the bound expression references is what
   denies the hoist**: `base = D_80083360; f = base;` with
   `while (f < base + 0x20)`.  Applied to the first loop it measures
   164/106 against 163/125, and to both 165/103.  Measured and worse: a `for`
   loop with the literal bound in its condition, 166/128; `!=` instead of
   `<`, 162/125.

   WHAT IS LEFT, 10 words across the three loops, and it is the same thing
   three times.  The target emits the handler call's tail TWICE per loop --

       lw   $a1, 0x8($s1)
       lw   $a2, 0xC($s1)
       jalr $a3
       addiu $a0, $s1, 0x20
       j    .L800898BC
       addiu $s0, $s0, 0x4        <- its own copy of the increment

   -- so the call path jumps PAST the shared `addiu $s0, $s0, 0x4` at
   `.L800898B8` and does its own in the delay slot, while the refusal paths
   fall through that one.  A plain `break` out of the search loop merges the
   two and emits the increment once, which is this body.  Finding the source
   shape that keeps them separate is the remaining job; `docs/matching-
   levers.md`'s "A DUPLICATED FAIL BODY IS WHAT PUTS IT IN cse1's PATH" is
   the nearest relative, but note that here the target does NOT cross-jump
   the copies, so whatever the source does has to defeat that merge too.

   The prologue also saves two fewer callee-saved registers than the target,
   which follows from the same thing.

   Nothing else in the reading is in doubt: every call, every field offset,
   every table and every gate above was read off the listing. */

void func_800897E4(void)
{
    u32 *f;
    u32 *base;
    u32 *o;
    u32 *a;
    u32 *b;
    u8 *s;
    u8 *cam;
    void (*h)(u8 *, s32, s32);
    s32 save;
    s32 i;

    if ((D_800E296C & 0x2000000) != 0)
    {
        b = D_800DCFA4;
        base = D_80083360;
        f = base;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s == NULL)
                {
                    *f = 0;
                }
                else if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                {
                    a = D_800DCF80;
                    b = D_800DCFA4;
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
            f++;
            o++;
        } while (f < base + 0x20);
    }
    else if ((*(s32 *)(D_800814A8 + 0x1C) & 0x10) != 0)
    {
        base = D_80083360;
        f = base;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s == NULL)
                {
                    *f = 0;
                }
                else if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                {
                    b = D_800DCF80 + 1;
                    if (D_800DCF80[0] == (u32)h)
                    {
                        cam = D_80083498;
                        if (*(s32 *)(cam + 0x14) != 0)
                        {
                            save = *(s32 *)(cam + 0x18);
                            *(s32 *)(cam + 0x18) = 0;
                            h(cam + 0x20, *(s32 *)(cam + 0x8), *(s32 *)(cam + 0xC));
                            *(s32 *)(cam + 0x18) = save;
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
            f++;
            o++;
        } while (f < base + 0x20);
    }
    else
    {
        i = 0;
        f = D_80083360;
        o = D_800833E0;
        do
        {
            h = (void (*)(u8 *, s32, s32))*f;
            if (h != NULL)
            {
                s = *(u8 **)o;
                if (s == NULL)
                {
                    *f = 0;
                }
                else if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                {
                    h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                    if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
                    {
                        return;
                    }
                }
            }
            f++;
            i++;
            o++;
        } while (i < 0x20);
    }
}
