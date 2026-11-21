/* PARK: func_800D50FC -- **102 of 102 words, 0 DIFFERING**, 2026-11-20.
   NOT LANDED, AND NOT BECAUSE OF THE BODY.

   This function owns `jtbl_80089454`, which sits at rodata offset 0xCF4 --
   4 mod 8 -- and `cc1psx` writes `.align 3` before every table it compiles.
   Compiled inside one big ovl_dungeon.c the table therefore gains four bytes
   of padding the original build does not have, `.rodata` grows and
   `build/dungeon.bin` comes out eight bytes long.  See
   docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8", and
   the worked carve in splat/town.bin.yaml.

   **So this body is correct and complete and must not be merged until the
   unit is carved in the same change.**  It is parked here so the carve is a
   purely structural job with no decompilation left in it.

   The body came out of `tools/m2c` plus retyping, at 0 differing on the
   FIRST compile -- which is the same thing that happened on 2026-11-15 when
   a batch was handed this function by mistake.  Nothing was fought for:

     * the switch is on `*(u16 *)(p + 0x1E) & 7` with the eight table entries
       grouping as 0/4 -> the random-velocity arm, 3/7 -> the zero arm, and
       1/2/5/6 -> an empty arm that falls through to the shared tail.  The
       empty `case` labels have to be written out: without them the compiler
       builds a different table.
     * the three `+=` statements on b+0x0/+0x4/+0x8 are each their own
       statement; the target's four loads bunched ahead of the three adds are
       sched1's doing, not the source's.
     * `n` is `s16` and the test is `n <= 0`, which is what gives
       `sll $v0, $v0, 16` + `bgtz` rather than a compare.

   DECLARATIONS
   s32 func_80069EF8();
   END DECLARATIONS
   (D_800814A0 is already declared.  The unit also already carries
   `void func_800D50FC();` at the top, which C89 makes compatible with this
   three-parameter definition, and the only use of the name is
   `n->unk_10 = (u32)func_800D50FC;` -- an address, so the arity does not
   matter to it.)

   Measured with
     python3 tools/decomp/parkresidue.py func_800D50FC
*/
void func_800D50FC(u8 *p, u8 *b, u8 *c)
{
    s32 i;
    s16 n;

    i = *(u16 *)(p + 0x1E) & 7;
    switch (i)
    {
    case 1:
    case 2:
    case 5:
    case 6:
        break;
    case 0:
    case 4:
        *(s32 *)(b + 0xC) = ((func_80069EF8() & 0x3FFF) - 0x2000) << 5;
        *(s32 *)(b + 0x10) = ((func_80069EF8() & 0x3FFF) - 0x2000) << 5;
        break;
    case 3:
    case 7:
        *(s32 *)(b + 0x10) = 0;
        *(s32 *)(b + 0xC) = 0;
        break;
    }
    *(s32 *)(b + 0x0) += *(s32 *)(b + 0xC);
    *(s32 *)(b + 0x4) += *(s32 *)(b + 0x10);
    *(s32 *)(b + 0x8) += *(s32 *)(b + 0x14);
    c[0xC] = p[0xC] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xD] = p[0xD] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xE] = p[0xE] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    n = *(u16 *)(p + 0x1E) - 1;
    *(u16 *)(p + 0x1E) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
