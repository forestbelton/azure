/* PARK: func_800AD6FC, 174 of 174 words, 31 aligned differences.
   ovl_dungeon.  Measured 2026-11-16.  Builds one of four message strings for
   a record, adds a score to its +0x24 counter and clamps it.

   SIX NEW DECLARATIONS, all message/rodata blobs the unit does not yet name
   (insert beside the existing `extern u8 D_800E0683[];`):

       extern u8 D_800E0CAE[];
       extern u8 D_800E0CDC[];
       extern u8 D_800E0CF3[];
       extern u8 D_800E0D1B[];
       extern u8 D_800E0D36[];
       extern u8 D_80089084[];

   D_80089084 is this unit's own rodata (there is an INCLUDE_RODATA for it),
   the other five are out-of-segment externs.

   The prototype `s32 func_800AD6FC(u8 *, s32, s32);` at line 361 is pinned by
   eight landed call sites, all of which pass 0 as the third argument -- but
   the body INDEXES that argument (`lbu $v1, 0x1($s4)`), so it is a record
   pointer declared `s32`.  Hence the `p = (u8 *)arg;` at the top; do not
   widen the declaration.

   The tail -- the 0x10000 clamp, the `c[0x25]` / `c+0x1C & ~8` pair and the
   `c[0x66] < c[0x25]` swap -- is the same block j34 decoded in
   func_800ADE74; see docs/dungeon-parks/func_800ADE74.c.  The three
   func_80099194 / func_80099734 chains are written UN-NESTED, which is the
   shape csrc/ovl_dungeon.c:3841 already uses and landed.

   ------------------------------------------------------------------ residue

   31 rows, no structural difference.  The bulk is a `$s2`/`$s4` swap between
   the `mode` parameter and the record pointer; the rest is delay-slot
   filling around the double condition, where the target duplicates
   `addiu $v0, $zero, 0xC` into TWO branch delay slots and this body
   materialises the constant once after the reload.

   -------------------------------------------------------------- what paid

   The function compiled at 172 words and 27 differences on the first try --
   structurally right everywhere -- and the whole 2-word deficit was in one
   place: the target RE-LOADS `p[1]` before the second half of

       (p[1] == 2 && p[0] == 1) || (p[1] == 0xC && p[0] == 6)

   and leaves the following load-delay slot as a `nop`, while this body kept
   the first load's value live in `$a0` across the `p[0]` test.

   **A DEVICE IS WHAT GETS THE RELOAD.**  `*(volatile u8 *)(p + 1) == 0xC`
   for the SECOND test only is worth exactly the 2 words.  The reason no
   plain spelling reaches it: the block holding the second test is entered
   both by the first `bne` and by fall-through, and with `-fcse-skip-blocks`
   cse extends its run across that branch anyway, so the two textual `p[1]`
   reads become one pseudo however they are written.  Measured and rejected,
   all 172 words / 27 differing: the condition as nested `if`s with a `goto`,
   as doubly-nested `if`s, and with the second read spelled
   `*(u8 *)(arg + 1)` to give it a different-looking base.

   Measured with the device in place: volatile on the second test only, 31;
   on a separate `volatile u8 *pv` local used for that read, 31 (identical);
   on BOTH the first-pair and the case-2 test, 31; on the whole `p` pointer,
   37; with the operands of the second test swapped so `p[0]` is read first,
   38.  So the narrowest placement is also the best. */

s32 func_800AD6FC(u8 *c, s32 mode, s32 arg)
{
    u8 *p;
    u8 *m;
    u8 *h;
    s32 t;
    s32 t0;

    p = (u8 *)arg;
    m = func_800990FC();
    h = m;
    if ((s16)func_80042900((s8 *)c, 0x19) != 0)
    {
        h = func_80099194(D_800E0CAE, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        func_80099290(h);
        func_800A5720(m);
        return 0;
    }
    if (mode == 3 && p != NULL && ((p[1] == 2 && p[0] == 1) || (*(volatile u8 *)(p + 1) == 0xC && p[0] == 6)))
    {
        mode = 2;
    }
    t = *(u16 *)(c + 0x24);
    t0 = t;
    switch (mode)
    {
    case 0:
        h = func_80099194(D_800E0CDC, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        break;
    case 1:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0CF3, h);
        t += 0x300;
        break;
    case 2:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D1B, h);
        if (p != NULL && p[1] == 0xC && p[0] == 6)
        {
            t += 0xA00;
        }
        else
        {
            t += 0x3200;
        }
        break;
    case 3:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D36, h);
        t += 0x6400;
        break;
    }
    if (p != NULL || mode == 0)
    {
        func_80099290(h);
        func_800A5720(m);
    }
    if (t0 != t)
    {
        func_800A56E0(0x51E);
    }
    if (t > 0x10000)
    {
        t = 0xFFFF;
    }
    *(u16 *)(c + 0x24) = t;
    if (c[0x25] != 0)
    {
        *(s32 *)(c + 0x1C) &= ~0x8;
    }
    if (c[0x66] < c[0x25])
    {
        c[0x25] = c[0x66];
        c[0x24] = 0xFF;
    }
    return mode;
}
