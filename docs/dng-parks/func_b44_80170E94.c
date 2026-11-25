/* PARKED at 520 words / 520 words / 4 aligned differences (aligndiff.py).
   func_b44_80170E94, ovl_dng_b44. 1 member (dngfam.py), so this park is
   worth its own 520 words and nothing else.

   THE RESIDUE IS FOUR WORDS IN ONE FIVE-INSTRUCTION WINDOW, inside `case 9'
   of the (d + 0x46) & 0x3FFF switch. Target against candidate:

     801713A0  lw   $v0, 0x14($s2)      candidate: lw   v1, 20(s2)
     801713A8  bltz $v0, .Lb44_801713D8 candidate: bltz v1, ...
     801713AC  lui  $v1, 0x8000         candidate: lui  v0, 0x8000
     801713B0  or   $v0, $v0, $v1       candidate: or   v0, v1, v0

   Every other word of the 520 is identical, every callee-saved register
   matches, the frame is 0x40 with the same six saves, and the jump table is
   resolved (see below). The two quantities that hold the loaded
   d + 0x14 word and the 0x80000000 constant are EXCHANGED, nothing else.

   THE MECHANISM, read out of local-alloc.c rather than guessed.
   The loaded word is referenced in two basic blocks (the `lw'/`bltz' block
   and the `or'/`sw' block), so local_alloc gives it no quantity
   (local-alloc.c:1762: "If UREG is a pseudo-register that hasn't already
   been assigned a quantity number, it means that it is not local to this
   block"), combine_regs returns 0 for it, and block_alloc's operand loop
   (local-alloc.c:1241) falls through to operand 2 and ties the `or' result
   to the CONSTANT instead. The constant therefore takes the first free
   caller-saved register, $v0, and global_alloc has to give the loaded word
   $v1. In the target the tie is the other way round: the constant is in
   $v1 and the loaded word shares $v0 with the `or' result.

   To reproduce the target the 0x80000000 constant must NOT be a
   block-local, single-set pseudo at local-alloc time. A plain
   `cc2 = 0x80000000;' local does not achieve that: local_alloc calls
   update_equiv_regs() first, which promotes the single set of a constant to
   a REG_EQUIV and rematerialises it at the use, so the pseudo is block-local
   again. I could not find a plain-C spelling that keeps it alive across the
   `bltz'. This is a permuter-shaped residue: four words, one window, a named
   cause.

   EVERY SPELLING MEASURED ON THIS STATEMENT, all at 520 words:

   | spelling of the case-9 read-modify-write              | aligned |
   | `*(s32 *)(d + 0x14) |= 0x80000000;' (the body below)  |    4    |
   | `= *(s32 *)(d + 0x14) | 0x80000000'                   |    4    |
   | `|= 0x80000000u', `|= -0x80000000', `|= 1 << 31'      |    4    |
   | `|= ~0x7FFFFFFF'                                      |    4    |
   | `cc2 = 0x80000000;' then `|= cc2', flat               |    4    |
   | `cc2 = 0x80000000;' then `|= cc2', inside the 0x400 if|    4    |
   | the 0x400/sign gate as nested ifs instead of `&&'     |    4    |
   | a `q = *(s32 *)(d + 0x1C)' local for the 0x400 test   |    4    |
   | `*(s32 *)(d + 0x14) > -1' for `>= 0'                  |    4    |
   | `q14 = (s32 *)(d + 0x14)' pointer local, either form  |    4    |
   | the RMW written twice (cse deletes the second)        |    4    |
   | `if (1) { RMW }'                                      |    4    |
   | `m = func_800A6D30();' hoisted out of the 0x2A store  |    4    |
   | `(func_800A6D30() % 8 * 0x200)' for `(& 7) << 9'      |    4    |
   | `*(s16 *)(d + 0x2A) += ...' for the explicit add      |    4    |
   | a `v2A = (func_800A6D30() & 7) << 9' local            |    4    |
   | `t14 = *(s32 *)(d + 0x14); t14 |= 0x80000000;' and    |    5    |
   |   store back -- the `or' becomes in-place as in the   |         |
   |   target, but on $v1                                  |         |
   | `cc2 = 0x80000000' + the RMW in `do { } while (0)'    |   68    |
   |   (this is the landed func_b09_80171728's recorded    |         |
   |   device; INVERTED here)                              |         |
   | the RMW in `do { } while (0)' alone                   |   71    |
   | the 0x2A store in `do { } while (0)'                  |   71    |
   | a `do { } while (0)' round the whole 0x400 gate       |  121    |
   | the 0x2A store BEFORE the RMW                         | 523w/88 |
   | `v2A = *(u16 *)(d + 0x2A)' hoisted above the RMW      |   75    |
   | case 9's two arms inverted (`p[0xAE] == 0' first)     |   17    |

   Every `do { } while (0)' bump moves `d' from $s2 to $s1 across the whole
   body (71 rows) and does NOT fix the four words, so the fault is not a
   reference count. Local declaration ORDER is free: five different orders
   all measure 4.

   WHAT IS ALREADY RIGHT, and should not be re-derived:

   THE TABLE. jtbl_b44_80170808, 12 words = 12 cases + 0 pad, dispatch bound
   12, MINVAL 1 (`addiu $v1, $v0, -0x1' before the `sltiu $v0, $v1, 0xC'), so
   entry k is case k+1. Entries 3 and 9 (cases 4 and 10) hold
   .Lb44_80171490, which is ALSO the range check's own `beqz' target, so they
   are gcc's default padding and get no `case' label. Written arms:
   1,2,3 -> func_800AAF00; 5,6,7 -> func_800A0818 / D_800814A8[0x9A];
   8 -> func_b44_80172050; 9 -> the arm above; 11 -> func_b44_801718F8;
   12 -> func_800A9A0C; default -> func_b44_801718F8. Ten case nodes, well
   over the five cc1psx needs for a table.

   THE 5/6/7 ARM FALLS THROUGH INTO CASES 1/2/3 and `case 12:' sits inside
   the `w[0x9A] != 0x11' block's exit -- func_800A9A0C is written twice and
   jump2 cross-joins it. Dropping the `m' local costs 78 aligned
   differences; dropping the `w = (u8 *)D_800814A8' local costs 521 words /
   20. Both facts are the landed func_b09_80171728's and both held here.

   THE FOUR `*(u8 **)(c + 0x2C)' GATES EACH NEED THEIR OWN POINTER LOCAL.
   Sharing one local between case 12's D_80175F10 dress and the final tail's
   gives the dress a two-set pseudo, which goes to global_alloc and can no
   longer tie with its own `lui', costing 2 aligned differences at that site.
   Merging e5 into e2 (the two D_80175F38 sites) is 71; merging e4 into e3
   is 7; merging both is 74.

   THE FINAL D_80175F10 GATE READS c + 0x2C BEFORE MATERIALISING THE
   ADDRESS: with `e4 = D_80175F10;' ahead of the `if' the `lui' comes out one
   word early, 2 aligned differences. Assigning it inside the `if' (as
   below) or writing the symbol inline at all three uses both measure the
   same.

   THE TWIN is the landed func_b09_80171728 (csrc/ovl_dng_b09.c, 363 words),
   found by its callee set (func_800AA924 / func_800A1C58 / func_800AAB10 /
   func_8009FB34 / func_800AAF00). dngtwin scored no carved sibling at all
   for this function. Deltas this body carries over b09: a `p[0xAE] == 0'
   conjunct on the opening D_80083462 0x1000 gate; a func_b44_80175ED4 call
   at six sites; a two-armed state block (0x19 with func_b44_80174EB4 and
   the D_80175F38 dress, 0xE with the D_80175F10 dress) in place of b09's
   single 0xE arm; func_b44_80174698 instead of a dress in the 0x80000 arm;
   separate case 11 and case 12 arms; a func_80042900 / D_800814A8[0x9A]
   0x17 / func_b44_80172334 else-arm on the `v & 0x2000' test; and a
   two-armed final tail.

   NOTE A PROTOTYPE CHANGE THIS BODY NEEDS: func_b44_80175ED4's second
   parameter is `u8 *', not `UnkVec *', because every call here passes `d'.
   The landed definition in csrc/ovl_dng_b44.c already has that signature
   and is 15 words / 0 differing with it.

   DECLARATIONS
   s32 func_800AA924(u8 *, void *, u8 *, u8 *);
   s16 func_8009FB34(u16, u16);
   s16 func_800A1C58(u8 *);
   s32 func_800AAB10(u8 *, void *, u8 *, u8 *);
   void func_800AAF00(u8 *, UnkVec *, u8 *, s32, void (*)());
   extern u8 D_80175F50[];
   extern u8 D_80175F60[];
   void func_b44_801716B4(u8 *, UnkVec *, u8 *, u8 *);
   void func_b44_80172218(u8 *, UnkVec *, u8 *, u8 *);
   void func_b44_80172334(u8 *, UnkVec *, u8 *, u8 *);
   s32 func_b44_801723D8(u8 *, UnkVec *, u8 *, u8 *);
   s32 func_b44_80172504(u8 *, UnkVec *, u8 *, u8 *);
   s32 func_b44_801727D8(u8 *, void *, void *, s32);
   UnkNode *func_b44_80174EB4(u8 *, UnkVec *, u8 *);
   s32 func_b44_80175ED4(u8 *, u8 *);
   END DECLARATIONS
*/
void func_b44_80170E94(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;
    s32 v;
    s32 m;
    u16 x;
    u8 *e;
    u8 *e2;
    u8 *e3;
    u8 *e4;
    u8 *e5;
    u8 *e6;
    u8 *w;

    if (p[0xAE] == 0 && (D_80083462 & 0x1000) != 0)
    {
        p[0x9A] = 0xE;
        func_b44_801716B4(p, b, c, d);
        return;
    }
    if (d[0x25] == 0)
    {
        func_b44_80175ED4(p, d);
        func_800AA79C(p, b, c, d);
        if (*(u8 **)(c + 0x2C) != D_80175F28)
        {
            e = D_80175F20;
            *(u8 **)(c + 0x2C) = e;
            func_80047784(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        if (*(u8 **)(c + 0x2C) == D_80175F28)
        {
            p[0x9A] = 0xD;
            p[0x9B] = 1;
            *(u32 *)(p + 0x8C) = 0;
            *(s32 *)(d + 0x1C) &= ~0x40000;
            return;
        }
        func_b44_80175ED4(p, d);
        if (func_800AA924(p, b, c, D_80175F20) != 0)
        {
            return;
        }
    }
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0x19 && p[0xAE] != 0)
        {
            e2 = D_80175F38;
            if (*(u8 **)(c + 0x2C) != e2)
            {
                func_b44_80174EB4(p, b, c);
                *(u8 **)(c + 0x2C) = e2;
                func_80047784(c, e2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0x19;
        }
        else if (p[0x9A] != 0xE && p[0xAE] == 0)
        {
            e3 = D_80175F10;
            if (*(u8 **)(c + 0x2C) != e3)
            {
                *(u8 **)(c + 0x2C) = e3;
                func_80047784(c, e3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= ~0xC;
        if (*(s16 *)(d + 0x64) != 0 && func_800AA6B4(p, b, c, D_80175F50) != 0)
        {
            return;
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_b44_80174698(p, (u8 *)b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            func_800AAB10(p, b, c, d);
        }
    }
    n = func_8009FB34(c[0x24], c[0x25]);
    c[0x26] = n;
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
        {
            func_800A9A0C(d);
            return;
        }
        if (*(u16 *)(c + 0x24) == D_80082EA4 && p[0xAE] == 0)
        {
            func_b44_80175ED4(p, d);
            func_b44_801718F8(p, b, c, d);
            return;
        }
        if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
        {
            if ((D_80083462 & 0x2000) != 0 &&
                (s16)func_8009A180(d, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                return;
            }
            if (p[0xAE] == 0 && (s16)func_b44_801727D8(p, b, c, 0) == 0)
            {
                return;
            }
            *(u16 *)(d + 0x46) |= 0x4000;
            if ((*(u16 *)(d + 0x46) & 0x8000) == 0)
            {
                func_b44_801718F8(p, b, c, d);
                if (p[0xAE] != 0)
                {
                    func_b44_80175ED4(p, d);
                }
                return;
            }
        }
        switch (*(u16 *)(d + 0x46) & 0x3FFF)
        {
        case 12:
            if (func_b44_80175ED4(p, d) != 0)
            {
                e6 = D_80175F10;
                *(u8 **)(c + 0x2C) = e6;
                func_80047784(c, e6[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            func_800A9A0C(d);
            return;
        case 11:
            func_b44_801718F8(p, b, c, d);
            func_b44_80175ED4(p, d);
            return;
        case 8:
            func_b44_80175ED4(p, d);
            if ((s16)func_b44_80172050(p, b, c, d) != 0)
            {
                return;
            }
            func_b44_80172218(p, b, c, d);
            return;
        case 9:
            if (p[0xAE] != 0)
            {
                if ((*(s32 *)(d + 0x1C) & 0x400) != 0 && *(s32 *)(d + 0x14) >= 0)
                {
                    *(s32 *)(d + 0x14) |= 0x80000000;
                    *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + ((func_800A6D30() & 7) << 9);
                }
                func_b44_80172504(p, b, c, d);
                return;
            }
            func_b44_801723D8(p, b, c, d);
            return;
        case 5:
        case 6:
        case 7:
            m = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24], D_80082E80[0x25], &x);
            w = (u8 *)D_800814A8;
            *(s16 *)(d + 0x2A) = m;
            if (w[0x9A] != 0x11)
            {
                func_800A9A0C(d);
                return;
            }
        case 1:
        case 2:
        case 3:
            func_b44_80175ED4(p, d);
            func_800AAF00(p, b, c, (s32)D_80175F60, func_b44_80170E94);
            return;
        default:
            func_b44_801718F8(p, b, c, d);
            if (p[0xAE] != 0)
            {
                func_b44_80175ED4(p, d);
            }
            return;
        }
    }
    if (p[0xAE] == 0)
    {
        v = *(s32 *)(d + 0x1C);
        if ((v & 0x2000) == 0)
        {
            if ((s8)n < 0 || (D_800E2970_s[(s8)n].unk_0C & 2) == 0)
            {
                if ((v & 0x430) == 0)
                {
                    if ((s16)func_8009FD7C(c[0x24], c[0x25], D_80082E80[0x24],
                                           D_80082E80[0x25]) != 0)
                    {
                        *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], D_80082E80[0x24],
                                                           D_80082E80[0x25], &x);
                    }
                }
            }
        }
        else if ((s16)func_80042900((s8 *)d, 4) == 0 && *(u8 *)(D_800814A8 + 0x9A) == 0x17)
        {
            func_b44_80172334(p, b, c, d);
        }
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return;
    }
    if ((*(u16 *)(c + 0x14) & 0x40) != 0)
    {
        return;
    }
    if (p[0xAE] != 0)
    {
        e5 = D_80175F38;
        if (*(u8 **)(c + 0x2C) != e5)
        {
            func_b44_80174EB4(p, b, c);
            *(u8 **)(c + 0x2C) = e5;
            func_80047784(c, e5[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        return;
    }
    if (*(u8 **)(c + 0x2C) != D_80175F10)
    {
        e4 = D_80175F10;
        *(u8 **)(c + 0x2C) = e4;
        func_80047784(c, e4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    }
}
