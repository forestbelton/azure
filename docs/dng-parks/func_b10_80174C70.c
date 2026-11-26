/* PARK func_b10_80174C70 -- ovl_dng_b10, 173 words target.
   BEST: 173 words candidate -- WORD-EXACT -- 32 aligned differences, of
   which 31 are register-only (`R') and exactly ONE is a real instruction
   difference. 1 member (one-member family, no replication).

   THIS FUNCTION OWNS A JUMP TABLE. jtbl_b10_80170898 at 0x80170898
   (address mod 8 == 0), 28 `.word's = 27 cases + 1 pad, dispatch bound
   `sltiu $v0, $v1, 0x1B' = 27 with NO minval subtraction, so the case set is
   0..26 and every value below the lowest real arm is a real case. FIVE
   distinct targets, and the `beqz' discriminator sends out-of-range to
   .Lb10_80174DE8, which is the code AFTER the switch, so there is NO
   `default:' arm -- the range check is the `break'. Entry classes, in
   ENTRY-ADDRESS order (which here is also ascending case order):
     cases 0,1,2,3 -> .Lb10_80174D48 (falls THROUGH into case 4's block)
     case  4       -> .Lb10_80174D60
     cases 5..16   -> .Lb10_80174D74
     cases 17..25  -> .Lb10_80174D90
     case  26      -> .Lb10_80174DC4 (returns, does not break)
   All 27 case labels are written out; the body below reproduces the table.

   WHAT IT IS: the +0x10 step of the 0x212 decoration node that
   func_b10_80174F24 spawns. The 27-way switch on the payload's +0x1A frame
   counter fades the renderable's three colour bytes up by 0x18 over frames
   0..3, then sets a +0x62 speed and an angular velocity at the renderable's
   +0x1A for three phases, fades the colour back down by 0xE over frames
   17..25, and on frame 26 raises 0x8000 in the node's -0x2 and in
   D_800814A0 and returns. The tail bumps the frame counter, wraps the
   renderable's angle modulo 0x1000, biases it by a quarter turn into
   `ang', and integrates the mover: +0x8 by p[0x62]*func_800644B8(ang) << 4,
   and +0xC/+0x10 from this bank's facing pair at D_b10_80170874 scaled by
   p[0x62]*func_80064584(ang) << 4 >> 4, then +0x0/+0x4 by those.

   DONOR: none by ratio -- dngtwin reports no carved sibling at 0.70 and
   dngcallee's best tier is 3-of-4 across 38 resident functions, i.e. noise.
   The body was written off its own listing plus the in-unit
   func_b10_80174898 (the `UnkDngB09A28Tbl t = *(UnkDngB09A28Tbl *)D_b10_...'
   local-copy idiom, same rodata, same `& 0x1C' byte index) and
   func_b10_80174F24 above (the same `e[2]' pair and `g' pointer base).

   FACTS MEASURED ON THIS BODY (aligned differences):

   | spelling                                                   | words | rows |
   |------------------------------------------------------------|-------|------|
   | the facing-pair reads AFTER the two sin/cos calls and       |       |      |
   |   BEFORE the +0x8 integration                               |  173  |  32  |
   |   ... +0x8 integration before the pair reads                |  176  |  62  |
   | func_800644B8's product in a `s32 w' local, `b->unk_08 +=   |       |      |
   |   (w << 4)'                                                 |  173  |  32  |
   |   ... the call written inline in the +0x8 statement         |  174  |  49  |
   |   ... only the CALL in `w', the multiply left inline        |  176  |  47  |
   | the quarter-turn bias through TWO locals, `v2' then `ang',  |       |      |
   |   with an if/else assigning `ang' in both arms              |  173  |  32  |
   |   ... one local, `ang = ...; if (ang < 0) ang += 0x1000'    |  174  |  33  |
   |   ... one local with `ang = ang + 0x1000'                   |  174  |  33  |
   |   ... `ang = v2; if (ang < 0) ang += 0x1000'                |  174  |  33  |
   |   ... `ang = v2 + 0x1000; if (v2 >= 0) ang = v2'            |  174  |  34  |
   |   ... the ternary `ang = (v2 < 0) ? v2 + 0x1000 : v2'       |  174  |  33  |

   THE ONE REAL ROW, and it is almost certainly the root of the other 31.
   The candidate emits `addiu $v0, $a2, 0xC00' where the target has
   `addiu $a2, $v1, 0x1000': cse folds `(v2 - 0x400) + 0x1000' into
   `load + 0xC00' through `fold_rtx's PLUS arm (`lookup_as_function' on the
   register's equivalence class), which keeps the LOADED value live one
   instruction longer than the target does. In the target the load dies at
   `addiu $v1, $a2, -0x400' and $a2 is then free to carry `ang'; in the
   candidate it is not, and the whole tail rotates. The 31 register rows are
   that rotation: the parameter `c' sits in $a3 where the target has $s0
   (15 rows), and the four `mflo' temporaries sit in $t0 where the target has
   $a3 (8 rows). Note local_alloc runs BEFORE global_alloc (toplev.c), so
   global.c's choice for `c' is downstream of which low registers
   local-alloc has already spent -- it is not an allocno-priority question
   about `c' itself.

   MEASURED AND RULED OUT for the fold (all still 32 rows with the same one
   real row): `0x1000 + v2' operand order; the gate inverted to
   `if (v2 >= 0) { ang = v2; } else { ang = v2 + 0x1000; }'; `s32 v2' with
   the test written `(s16)v2 < 0'; a `do { } while (0)' round the whole
   if/else (174/36), round the `+ 0x1000' arm alone (173/32), or round a
   single-variable form (174/33). Also FLAT at 32: the byte-fade temporary
   `v' as `s16' or `u8'; a THREE-parameter signature (the function is only
   ever address-taken, so its arity is free) and a FIVE-parameter one.
   The byte-fade written without the `v' temporary was not measurable here --
   the edit must be scoped to this function, a file-wide text replace of
   `s32 v;' breaks func_b10_8017092C.

   The declarations this body needs are the same block as
   func_b10_80174F24's, listed in that park.
DECLARATIONS
extern u8 D_801759C0[];
extern u8 D_80175A0C[];
UnkNode *func_8003FC64(s32);
s32 rand(void);
void func_b10_80174C70(u8 *, UnkVec *, u8 *, u8 *);
s32 func_80064584(s32);
s32 func_800644B8(s32);
extern const u8 D_b10_80170874[];
extern u32 D_800814A0;
typedef struct UnkDngB09A28
{
    s16 unk_00;
    s16 unk_02;
} UnkDngB09A28;
typedef struct UnkDngB09A28Tbl
{
    UnkDngB09A28 unk_00[8];
} UnkDngB09A28Tbl;
END DECLARATIONS
*/
void func_b10_80174C70(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 e[2];
    UnkDngB09A28Tbl t;
    u8 *g;
    s32 spd;
    s32 w;
    s32 v;
    s16 ang;
    s16 v2;

    t = *(UnkDngB09A28Tbl *)D_b10_80170874;
    switch (*(s16 *)(p + 0x1A))
    {
    case 0:
    case 1:
    case 2:
    case 3:
        v = c[0xE] + 0x18;
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        /* fall through */
    case 4:
        *(s16 *)(p + 0x62) = 0x20;
        *(s16 *)(c + 0x1A) = 0xA00;
        break;
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
    case 16:
        *(s16 *)(p + 0x62) = 0x18;
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x180;
        break;
    case 17:
    case 18:
    case 19:
    case 20:
    case 21:
    case 22:
    case 23:
    case 24:
    case 25:
        v = c[0xE] - 0xE;
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        *(s16 *)(p + 0x62) = 0x18;
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x30;
        break;
    case 26:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    *(s16 *)(p + 0x1A) = *(u16 *)(p + 0x1A) + 1;
    if (*(u16 *)(c + 0x1A) >= 0x1000)
    {
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
    }
    v2 = *(u16 *)(c + 0x1A) - 0x400;
    if (v2 < 0)
    {
        ang = v2 + 0x1000;
    }
    else
    {
        ang = v2;
    }
    spd = (*(s16 *)(p + 0x62) * func_80064584(ang)) << 4;
    w = *(s16 *)(p + 0x62) * func_800644B8(ang);
    g = (u8 *)&t;
    e[0] = *(u16 *)(g + (((*(s16 *)(p + 0x18) - 0x400) >> 7) & 0x1C));
    e[1] = *(u16 *)(g + (((*(s16 *)(p + 0x18) - 0x400) >> 7) & 0x1C) + 0x2);
    b->unk_08 = b->unk_08 + (w << 4);
    b->unk_0C = (e[0] * spd) >> 4;
    b->unk_10 = (e[1] * spd) >> 4;
    b->unk_00 = b->unk_00 + b->unk_0C;
    b->unk_04 = b->unk_04 + b->unk_10;
}
