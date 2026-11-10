#include "common.h"
#include "main.h"
#include "strings.h"

/* cardbk.c -- one of the nine translation units the 0x80020000 overlay was built
   from, carved out of csrc/ovl_main_80020000.c on 2026-11-10. Live-block
   function indices 122. How the boundaries were established, and which of
   them are inferences, is in docs/overlays.md under "Carving the nine".

   ONE FUNCTION. cardbk.c contributes exactly one function to the retail
   build, plus the four rodata bytes at 0x800200A4 and D_80027FD0 in the
   shared data file. The twin has fourteen functions here with no live
   counterpart, which is why this unit never paired an anchor. */

/* A 0xC-byte header, four 0x18-byte sprite records and a 0xC-byte trailer.
   Two spellings in here are load-bearing and both are measured:

   - the halvings `w / 2` and `h / 2` are written INLINE at the two stores,
     not in locals. They are loop-invariant either way, so loop.c hoists
     them, but written inline they are hoisted AFTER the three constant
     movables (0x38, 0x9CBCBC, 0x8C9C9C), which is where the target has
     them. In locals assigned inside the loop is 22 DIFF; in locals before
     the loop, 24.

   - `i` is incremented INSIDE the body, as `D_80027FD0[i++]`, and the `for`
     has an empty third clause. This is not about the top of the body: it is
     about being before `p += 0x18`. loop.c emits the reduced giv's own
     update (`$v1 += 0x18`, for the `p + 0xB` the seven record stores are
     addressed off) immediately before the biv update `p += 0x18`, so the
     giv update's LUID is fixed relative to that statement. sched1 here is a
     backward list scheduler, and the slot after the `sb $v0, 0($a0)` is a
     load-delay filler with exactly two candidates of priority 1, the `i`
     increment and the giv update; `rank_for_schedule` breaks that tie on
     INSN_LUID and prefers the HIGHER one (sched.c:2425 -- it returns
     `INSN_LUID (y) - INSN_LUID (x)`, and ready[0] is taken). With `i++` in
     the `for` clause it lands after `p += 0x18` and outranks the giv update,
     which rotates three insns and costs 10 DIFF. Moved before `p += 0x18`,
     the giv update takes the slot and the loop is exact.

   Measured, all 46 words: `D_80027FD0[i++]` 0; `p[0] = D_80027FD0[i]; i++;`
   before `p += 0x18` 0; the same with `i = 0` lifted out of the `for` 0; an
   address local `q = &D_80027FD0[i]; i++; p[0] = *q;` 0; `i++` AFTER
   `p += 0x18` 10 (the parked shape); `i++` first with `D_80027FD0[i - 1]` 2;
   `for (i = 0; i < 4; i++)` with an address local at the top of the body 23,
   and with the local declared in the body, spelled `D_80027FD0 + i`, or with
   `i = 0` lifted out, 23 each; `while (i < 4)` / `do ... while (i < 4)` with
   `i++` at the top of the body 14, as is `&D_80027FD0[i++]` in a `while`.
   Reading the byte (not the address) into a local at the top of the body is
   17, as `u8` or as `s32`. The permuter was not used. */
u8 *func_800242D8(u8 *p, s32 w, s32 h)
{
    s32 i;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 0;
    p += 0xC;
    for (i = 0; i < 4;)
    {
        p[1] = 0x38;
        *(s32 *)(p + 4) = 0x9CBCBC;
        *(s32 *)(p + 0xC) = 0x9CBCBC;
        *(s32 *)(p + 0x10) = 0x9CBCBC;
        *(s32 *)(p + 0x14) = 0x8C9C9C;
        p[0xA] = w / 2;
        p[0xB] = h / 2;
        p[0] = D_80027FD0[i++];
        p += 0x18;
    }
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x20;
    *(s16 *)(p + 6) = 1;
    p[0] = 0x80;
    return p + 0xC;
}

/* splat migrated this one-character string into func_80024390's listing, so
   landing that function loses the symbol. It belongs to this unit: the rodata
   stream has it at 0x800200A4, between card_mes.c's D_800200A0 and
   cardinfo.c's run, which is the position docs/overlays.md records for
   cardbk.c. */
const u8 D_800200A4[] = " ";
