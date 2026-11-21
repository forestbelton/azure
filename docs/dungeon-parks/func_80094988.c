/* func_80094988, ovl_dungeon, agent j47, 2026-11-16.

   296 of 299 words, 81 aligned differences (aligndiff). The STRUCTURE is
   settled -- every block, every branch, every loop bound and every callee
   argument matches; what is left is three missing words and a register
   rotation that follows from them.

   NEEDS ONE NEW DECLARATION, in the block at the top of csrc/ovl_dungeon.c:

       extern s32 D_800E3D74[];

   (D_800E3D74 is not reachable through D_800E3D40, which is declared as a
   1-byte scalar; provision.py says so.)

   WHAT IT DOES. Restore the party from the save-RAM mirror at 0x80010000.
   Set bit 1 of D_80013714 (remembering whether it was already set),
   func_800A6A9C(), hand the 0x80012194 record to func_80041E28, copy two
   scalars into the caller's +0xF8/+0xFC, then a 20-iteration loop that
   copies each 4-byte name record 0x800121E8[i] -> 0x80010248[i], publishes a
   pointer or NULL at 0x8001029C[i] from the 0x80012238[i] index byte, copies
   the 0x8C-byte Unk800E3E48 record 0x80012260[i] -> D_800E3E48[i], and
   publishes D_800E3DF0[i] from the 0x8001224C[i] index byte. Then a
   2-iteration loop doing the same for the caller's +0xD0 pair out of
   0x80012D52, two single-slot publishes into func_800982A8/func_80098614,
   and a second 20-iteration loop that, for every slot whose +1 byte is 0x13
   and whose +3 byte has 0x20, finds which of the caller's two +0xD0 slots
   holds it, builds the actor through func_800A1618 + a function pointer
   returned by func_800A0B94, and registers it. Finally restore D_80013714's
   bit 1 and func_80035208(D_800E045C).

   FOUR SPELLINGS THAT ARE SETTLED AND SHOULD NOT BE RE-DERIVED:

   * `b = (u8 *)0x80010000;` AS A POINTER LOCAL for the head and the first
     loop. All of the head's accesses are `%lo` displacements off one
     `lui $s0, 0x8001`, and the 0x80012194 argument is `ori`-ed onto the same
     register -- the constant-base idiom docs/matching-levers.md:2115
     records. Writing every one of them as an absolute constant instead is
     320 words of 299.
   * THE SECOND LOOP WALKS A POINTER, NOT AN INDEX. `slot = (u8 *)0x80010248`
     incremented by 4 is what gives the target's `$s2` and, through
     loop.c's combine_givs, its `$s4` = slot+3 with `lbu $v1, -0x2($s4)` for
     slot+1. Spelled `*((u8 *)0x80010249 + k * 4)` and friends, loop.c
     reduces only `k * 4` and rebuilds the constant inside the loop: three
     extra words per site, 320 words / 151.
   * `u8 v`, NOT `s32 v`, for the two index bytes in the first loop: 296/81
     against 296/92.
   * THE 0x8C-BYTE COPY IS `D_800E3E48[i] = *(Unk800E3E48 *)(...)`. The
     struct's alignment of 4 is what picks the 0x10-byte-chunk loop plus the
     0xC tail that the target has.

   THE RESIDUE, and it is one cause with three symptoms. Every remaining
   difference is that the target has NO callee-saved register left for a
   0x80010000 base in the second half of the function, so reload
   rematerialises `lui $reg, 0x8001` at each reference and the loads become
   `lui` + a `%lo` displacement. In this candidate that base is still live in
   `$s0`, so each of those reads is one word shorter:

     - word 78: the target's `addu $v1, $v0, $zero` ahead of the 0x8C
       multiply chain is missing; the loaded byte lands in `$v1` here and in
       `$v0` there, so no copy is needed.
     - words 119/120 and 134/135: the target reads 0x800121E0 and 0x800121E1
       as `lui` + `lbu %lo(...)($reg)`; this candidate materialises the whole
       address with `lui`+`ori` and loads `0($a0)` twice, because cse hoists
       the lo_sum when the same constant address is read twice in one
       extended block.
     - word 96: the target's `lui $v1, 0x8001` for the post-loop
       `*(u32 *)(0x8001029C + i * 4) = 0` store, which also feeds the second
       loop's base and the 0x80010248 constant.

   MEASURED AND REJECTED, all against the 296/81 body above:

     every 0x80010000 access as an absolute constant       320 / 151
     ditto but keeping `slot`                              303 / 103
     the post-loop 0x29C store as an absolute              299 / 120  (it
       breaks the IN-loop 0x29C store's giv: 4 words there instead of 1)
     a second base local `c` for the post-loop half        297 / 121
     `c` for post-loop + second loop, ternaries absolute   300 / 97
     0x800121E0/E1 read through `b`                        292 / 116
     D_800121E0/D_800121E1 as declared symbols             294 / 77
     D_80013714 as a declared symbol in the tail           295 / 78
     both of those as symbols                              293 / 74
     `s32 v`                                               296 / 92
     `s16 v`                                               296 / 81  (same)
     separate `v2`/`v3` per index-byte site                295 / 114
     the 0x224C test and use inlined, no local             296 / 81  (same)
     `s32 idx` with `(s16)` casts at the test and decrement 296 / 110
     `s32 sv`                                              296 / 89
     `u16 sv`                                              296 / 81  (same)
     `b` declared first / last, `i` and `j` swapped        296 / 81  (same)
     `__asm__("" ::"r"(q))` once or twice                  297 / 97
     `__asm__("" ::"r"(p))`                                293 / 118
     accumulator locals instead of pointer walks in loop 1 304 / 140

   The register rotation is the whole of the remaining 81, so by
   docs/dungeon-sweep.md's triage rule this is NOT pure-allocation permuter
   food -- the word counts do NOT match (296 against 299) and the three
   missing words are in three different blocks, which is the shape that
   wins. Queue it.

   The body below needs the ~1500 lines of declarations at the top of
   csrc/ovl_dungeon.c plus the one new extern above.    ====================================== 2026-11-21: 296/299 words -> 297/299
                                          at the same 81 rows

   `for (i = 0; i + 1 <= 0x14; i++)' in place of `for (i = 0; i < 0x14; i++)'.
   One of the three missing words, and it costs nothing.  Three other
   spellings of the same loop were measured: `i <= 0x13' is 296 words (no
   change) and `i != 0x14' is 295 and 85 rows.  So it is not "any equivalent
   test" -- it is specifically the `i + 1' form, which makes the increment
   feed the comparison.

   REJECTED, and worth recording because the permuter keeps proposing it:
   hoisting `b' into a `u8 *bb' inside the loop.  With one, two or three of
   the six `b' uses converted it is 297 words / 81 rows -- no change -- and
   with all six it reaches 298 words but 93 rows.  The permuter scored that
   as its best find here; measured, it trades two rows for one word and then
   twelve more rows.
*/

void func_80094988(u8 *p, u8 *q, s16 x, s16 y)
{
    s16 sx;
    s16 sy;
    u8 bx;
    u8 by;
    u8 *b;
    u8 *e;
    s16 sv;
    s16 idx;
    s32 i;
    s32 j;
    s32 k;
    u8 v;
    u8 *slot;

    b = (u8 *)0x80010000;
    sv = *(u16 *)(b + 0x3714) & 2;
    *(u16 *)(b + 0x3714) |= 2;
    func_800A6A9C();
    func_80041E28(q, (u8 *)0x80012194);
    *(u16 *)(p + 0xF8) = *(u16 *)(b + 0x2D50);
    *(u32 *)(p + 0xFC) = *(u32 *)(b + 0x2D58);
    for (i = 0; i + 1 <= 0x14; i++)
    {
        *(UnkDungeonName4 *)(b + 0x248 + i * 4) = *(UnkDungeonName4 *)(b + 0x21E8 + i * 4);
        v = *(u8 *)(b + 0x2238 + i);
        if (v != 0xFF)
        {
            *(u32 *)(b + 0x29C + i * 4) = (u32)((u8 *)0x80010248 + v * 4);
        }
        else
        {
            *(u32 *)(b + 0x29C + i * 4) = 0;
        }
        D_800E3E48[i] = *(Unk800E3E48 *)(b + 0x2260 + i * 0x8C);
        v = *(u8 *)(b + 0x224C + i);
        if (v != 0xFF)
        {
            D_800E3DF0[i] = (s32)&D_800E3E48[v];
        }
        else
        {
            D_800E3DF0[i] = 0;
        }
    }
    *(u32 *)(b + 0x29C + i * 4) = 0;
    for (j = 0; j < 2; j++)
    {
        v = *((u8 *)0x80012D52 + j);
        if (v != 0xFF)
        {
            *(u32 *)(p + j * 4 + 0xD0) = (u32)((u8 *)0x80010248 + v * 4);
        }
        else
        {
            *(u32 *)(p + j * 4 + 0xD0) = 0;
        }
    }
    func_800982A8(q, (*(u8 *)0x800121E0 == 0xFF) ? NULL : (u8 *)0x80010248 + *(u8 *)0x800121E0 * 4);
    func_80098614(q, (*(u8 *)0x800121E1 == 0xFF) ? NULL : (u8 *)0x80010248 + *(u8 *)0x800121E1 * 4);
    slot = (u8 *)0x80010248;
    for (k = 0; k < 0x14; k++)
    {
        if (slot[1] == 0x13 && (slot[3] & 0x20) != 0)
        {
            idx = 0;
            for (j = 0; j < 2; j++)
            {
                if (*(u32 *)(p + j * 4 + 0xD0) == (u32)slot)
                {
                    idx = j + 1;
                }
            }
            if (idx != 0)
            {
                idx--;
                e = func_800A1618(slot[0], 3);
                if (e != NULL)
                {
                    if (func_8009B88C(NULL, x, y, &sx, &sy) == 0)
                    {
                        while ((s16)func_800A4E2C(&bx, &by) < 0)
                        {
                        }
                        sx = bx;
                        sy = by;
                    }
                    e = ((u8 * (*)(s32, s16, s16, s16)) func_800A0B94(slot[0], e,
                                                                      1))(1, sx, sy,
                                                                          *(s16 *)(q + 0x88));
                    *(u8 **)(p + idx * 4 + 0xAC) = e;
                    if (e != NULL)
                    {
                        func_80042640((Unk42710 *)e, e[0x13]);
                        func_800424E0(e, e[0x13], slot);
                        *(u32 *)(p + idx * 4 + 0xD0) = (u32)slot;
                        slot[3] |= 0x20;
                        D_800E3D74[idx] = 0;
                        if (e[0x25] == 0)
                        {
                            *(u32 *)(e + 0x1C) |= 8;
                        }
                        D_800E3DF0[slot[3] & 0x1F] = (s32)e;
                        func_80042984(e);
                        func_800A152C(slot[0], 3);
                        func_80042560((s8 *)e);
                        func_800C542C(e, D_800DCED4[func_800429E4(e)], idx, 1);
                    }
                }
            }
        }
        slot += 4;
    }
    if (sv == 0)
    {
        *(u16 *)0x80013714 &= 0xFFFD;
    }
    func_80035208(D_800E045C);
}
