/* PARK: func_800C2824 -- 280 words against a target of 278, 105 aligned
   differences.  FIRST DRAFT, 2026-11-20.

   One of the seven 4-mod-8 jump-table functions, and since j96's carve it is
   the only thing standing between csrc/ovl_dungeon_800C2824.c and a complete
   unit: its table jtbl_8008938C is now at offset 0 of that unit's rodata, so
   **this function can be landed the moment its body is right** -- no further
   splat work, and `make build/dungeon.bin' is byte-identical today with the
   unit carved and func_800D50FC and func_800D81D4 compiled.

   WHAT IT IS.  An 18-case state machine on `*(s16 *)(p + 0x4)' with six real
   arms (0, 1, 2, 3, 16, 17) and twelve that fall to the default return.  It
   sets up a 0x1800340/0x400040 rectangle and a 0x360/0x1B8 pair on the stack
   for func_800B8FC8, installs func_800CEF54 as the node's handler, fades
   c[0xC..0xE] down over 0x10 frames, and on the way out raises 0x8000 in both
   `*(u16 *)(p - 2)' and D_800814A0.

   STRUCTURE THAT IS ALREADY RIGHT (read off the listing, not guessed):
     * case 0 FALLS THROUGH into case 1, and case 1 falls through to the
       default `return';
     * cases 3 and 17 SHARE the `|= 0x8000' tail (m2c's block_26), and cases
       1, 2 and 16 share the `*(u16 *)(p + 0x4) += 1; return;' block
       (block_19) -- both are jump2 cross-jumps, so writing each out is right;
     * `(s16)func_80042900(...) == 0' for the 0x1B arm, which is what gives
       the `sll 16' before the test;
     * the 0x3000/0x300 pick for func_8009A3D0's third argument is a plain
       `if' on `*(u32 *)(q + 0x1C) & 0x2000', in both case 2 and case 17.

   THE RESIDUE STARTS AT THE FRAME, AND THAT IS THE THING TO ATTACK FIRST.
   The target's prologue is `addiu $sp, $sp, -0x48' and saves s0..s3 and ra at
   0x30..0x40; this body's is -0x40 with the saves at 0x28..0x38.  So the
   target reserves EIGHT MORE BYTES than the locals and the call area need:
   five arguments to func_800B8FC8 make the outgoing area 0x00..0x17, the four
   stack locals are 12 bytes at 0x20..0x2B, and 0x18..0x1F is unaccounted for.
   That is docs/matching-levers.md's "A FRAME LARGER THAN THE FUNCTION'S CALLS
   NEED IS AN UNREFERENCED STACK LOCAL, AND THE ARITHMETIC NAMES ITS SIZE",
   and every row below the prologue follows from the save slots being wrong.

   MEASURED SO FAR (all on a `tools/decomp/onefunc.py` file, which reproduces
   the unit exactly for this function):
     four scalars `s32 sp20, sp24; s16 sp28, sp2A;' .... 280 words / 105 rows
     `s32 w2[2]; s16 v2[2];' passed as arrays ........... 285 / 84
     the same plus an unreferenced `s32 pad2[2];' ....... 285 / 103
     `s32 w2[4]; s16 v2[2];' ............................ 285 / 106
     `s32 w2[2]; s16 v2[6];' ............................ 285 / 103
   So the ARRAY form is worth 21 rows and costs 5 words, and padding it out
   does not recover the frame -- the eight bytes have to come from somewhere
   that does not also add spill slots.  **Do not sweep devices on this body
   until the frame is 0x48**; the device sweep is cheap and will chase the
   knock-on rows instead of the cause.

   DECLARATIONS
   void func_800C27F0();
   void func_800C0180();
   void func_800CEF54();
   extern u8 D_80010A93;
   extern u8 D_800E1640[];
   END DECLARATIONS
   (func_800C27F0 is in the HEAD unit and func_800CEF54 in
   ovl_dungeon_800CABA8, so both are cross-unit references now and need a
   declaration where a single-unit build needed none.  Everything else --
   D_800DF45C, D_800DF4A4, D_800DF55C, D_800DF560, D_800E3D7C, D_80082E80,
   D_800814A0 -- is already in csrc/ovl_dungeon_decls.h.)

   Measured with
     python3 tools/decomp/parkresidue.py func_800C2824
*/
void func_800C2824(u8 *p, u8 *b, u8 *c)
{
    u8 *q;
    u8 *r;
    s32 i;
    s32 j;
    s32 sp20;
    s32 sp24;
    s16 sp28;
    s16 sp2A;
    s32 a;
    u32 f;
    u8 v;
    u16 w;

    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        if ((*(u32 *)(*(u8 **)p + 0x14) & 0x4000) == 0)
        {
            *(s16 *)(p + 0x4) = 0x10;
            return;
        }
        sp24 = 0x400040;
        sp28 = 0x360;
        sp2A = 0x1B8;
        sp20 = 0x1800340;
        func_800B8FC8(*(u8 **)p, &sp20, (s32 *)&sp28, 1, 2);
        q = *(u8 **)p - 0x20;
        func_80044A50(q);
        func_800BC318(q);
        r = *(u8 **)(q + 0x8);
        w = *(u16 *)(r + 0x2);
        *(u16 *)(b + 0x2) = w;
        *(u16 *)(b + 0xE) = w;
        w = *(u16 *)(r + 0x6);
        *(u16 *)(b + 0x6) = w;
        *(u16 *)(b + 0x12) = w;
        w = *(u16 *)(r + 0xA);
        *(u16 *)(b + 0xA) = w;
        *(u16 *)(b + 0x16) = w;
        func_8004491C(p - 0x20, (u32)func_800CEF54);
        *(u16 *)(c + 0x10) = 0x20;
        *(u8 **)(c + 0x8) = (u8 *)&D_800DF4A4;
        *(s32 *)(c + 0xC) = 0x808080;
        *(u16 *)(c + 0x1E) = 0x1000;
        *(u16 *)(c + 0x1C) = 0x1000;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u16 *)(p + 0x6) = 0x10;
        func_800A56E0(1);
        *(u16 *)(p + 0x4) += 1;
    case 1:
        v = c[0xC];
        v = v - (s32)v / *(s16 *)(p + 0x6);
        c[0xC] = v;
        c[0xE] = v;
        c[0xD] = v;
        w = *(u16 *)(p + 0x6) - 1;
        *(u16 *)(p + 0x6) = w;
        if ((s32)(w << 16) <= 0)
        {
            *(u16 *)(p + 0x4) += 1;
        }
        return;
    default:
        return;
    case 2:
        i = func_800A32A4(*(u8 **)p);
        if (i != 0)
        {
            j = func_800A6620(i, 0);
            if (j < 0x40)
            {
                *(u8 *)(j * 0x54 + (s32)&D_80010A93) = 0;
                *(u32 *)(0x80010980 + j * 4) = 0;
            }
        }
        if ((s16)func_80042900(*(u8 **)p, 0x1B) == 0)
        {
            q = *(u8 **)p;
            r = *(u8 **)(q - 0x14);
            a = 0x3000;
            if ((*(u32 *)(q + 0x1C) & 0x2000) != 0)
            {
                a = 0x300;
            }
            func_8009A3D0(r[0x24], r[0x25], a);
        }
        func_8009A028(*(u8 **)p);
        func_80099844(*(u8 **)p, &D_800E1640);
        *(u16 *)(p + 0x4) += 1;
        return;
    case 3:
        D_800DF55C = *(u8 **)&D_800E3D7C;
        if (func_800BBA40(D_80082E80[0x24], D_80082E80[0x25], *(u16 *)(D_800E3D7C + 0x88),
                          &D_800DF45C, 0x2800, 0x208020, (u32)func_800C0180) != 0)
        {
            q = *(u8 **)p;
            *(u16 *)(q - 0x2) |= 0x8000;
            f = D_800814A0 | 0x8000;
            D_800814A0 = f;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 = f;
        }
        break;
    case 16:
        q = *(u8 **)p;
        r = *(u8 **)(q - 0x14);
        D_800DF55C = q;
        if (func_800BBA40(r[0x24], r[0x25], *(u16 *)(q + 0x88), &D_800DF45C, 0x2800, 0x208020,
                          (u32)func_800C27F0) != 0)
        {
            *(u16 *)(p + 0x6) = 0x3C;
            *(u16 *)(p + 0x4) += 1;
            return;
        }
        break;
    case 17:
        w = *(u16 *)(p + 0x6) - 1;
        *(u16 *)(p + 0x6) = w;
        if ((s32)(w << 16) <= 0)
        {
            func_800A32A4(*(u8 **)p);
            if ((s16)func_80042900(*(u8 **)p, 0x1B) == 0)
            {
                q = *(u8 **)p;
                r = *(u8 **)(q - 0x14);
                a = 0x3000;
                if ((*(u32 *)(q + 0x1C) & 0x2000) != 0)
                {
                    a = 0x300;
                }
                func_8009A3D0(r[0x24], r[0x25], a);
            }
            func_8009A028(*(u8 **)p);
            q = *(u8 **)p;
            *(u16 *)(q - 0x2) |= 0x8000;
            f = D_800814A0 | 0x8000;
            D_800814A0 = f;
            *(u16 *)(D_800DF560 + 0x1E) |= 0x8000;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 = f;
        }
        break;
    }
}
