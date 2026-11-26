/* func_b25_8017450C (csrc/ovl_dng_b25.c) -- PARKED at 415 words against the
   target's 413, 14 aligned differences (aligndiff), by agent h188 on
   2026-11-25. One-member family (dngfam: no other copy), so this parks 413
   words and nothing more.

   VERDICT: LENGTH + SCHEDULE. The body is otherwise exact: every opcode,
   immediate, displacement and register in the other 401 words matches, and
   the two surplus words are both `nop's in ONE TWELVE-INSTRUCTION WINDOW at
   the end of the switch's case 0, between `sh $zero, 0x88($s1)' (0x801746D0)
   and `sw $v1, %lo(D_800E296C)' (0x80174704). Nothing else differs.

   THE RESIDUE, EXACTLY. Target 0x801746C4..0x80174704:

       addiu $a0, $zero, 0x807          <- the func_800A56E0 argument, 15
       lhu   $a1, 0x92($s4)                insns before its own `jal'
       lui   $a2, %hi(D_800E296C)
       sw    $zero, 0xA0($s4)
       addiu $v0, $v0, 0x1
       sb    $v0, 0x9B($s4)
       lhu   $v0, 0xA($fp)
       lw    $v1, %lo(D_800E296C)($a2)   <- fills the load-delay slot
       addu  $v0, $v0, $a1
       ori   $v1, $v1, 0x40
       sh    $v0, 0xB6($s4)
       jal   func_800A56E0
       sw    $v1, %lo(D_800E296C)($a2)

   The candidate emits the same thirteen instructions plus two `nop's, because
   it allocates DIFFERENT REGISTERS for two short-lived values: the p + 0x92
   reload lands in `$v1' rather than `$a1', and the `%hi(D_800E296C)' address
   therefore cannot be materialised until after the `addu' that consumes
   `$v1', which in turn leaves the two load-delay slots empty. In the target
   `$a0' holds the 0x807 constant across the whole window and `$a1'/`$a2'
   hold the reload and the address, which is only possible if the 0x807
   materialisation is EARLY IN THE RTL -- i.e. its live range is long before
   sched runs, not after. Nothing in the C reached that.

   MECHANISM I BELIEVE BUT COULD NOT REACH: the constant's pseudo needs the
   call's copy preference (`find_reg's round-1 suggested register) while its
   live range is already long, so `$a0' is reserved across the window and the
   two cheap values fall into `$a1'/`$a2'. What I could not find is a source
   spelling that puts the `li $a0, 0x807' twelve instructions earlier.

   WHAT I WOULD TRY NEXT, in order:
   1. `-da' on this one function and read `.sched' for the ready list at
      0x801746C4 and `.lreg' for the p + 0x92 quantity's `qty_compare' score
      against the D_800E296C address quantity. This is the one place in this
      body where the diagnostic ladder has not been run, and the residue is
      two registers with nothing moved, which is exactly what `.lreg' answers.
   2. A `do { } while (0)' around the `func_800A56E0(0x807)' call alone: the
      macro-residue reading says a cue call IS a macro, and `flow.c:2122'
      would double the constant's references, which is the one lever that
      moves `qty_compare' for a value with a 2-reference range.
   3. Splitting `*(u16 *)(p + 0x92) += *(u16 *)(d + 0x88);' so the reload in
      the +0xB6 statement is a different pseudo from the +=.

   EVERY SPELLING MEASURED, with aligndiff figures. The rows above the line
   are the ones that are now in the body; the rows below are rejected.

   | spelling                                       | words | aligned |
   | as written below                               |  415  |   14    |
   | first draft (no v2 array, no sx/sy, flat gates)|  405  |  184    |
   | `nx < 0x1020 && nx > 0' not nested             |  410  |  123    |
   | two `s16 dx, dy' locals instead of `s16 v2[2]' |  410  |  123    |
   | no `sx'/`sy' locals, `ox * 0x40' written twice |  410  |  123    |
   | `w' not holding the product (just the call)    |  417  |   34    |
   | `*(s32 *)(p + 0x90) +=' BEFORE the v2 stores   |  419  |   59    |
   | `t = dd.unk_00' at the top of the function     |  419  |   46    |
   | no `t' local, `dd.unk_00 + ...' at both sites  |  419  |   43    |
   | `k = *(u16 *)(c + 0x1A) - 0x400' in one step   |  419  |   43    |
   | `s32 k' with `(s16)' on the test and the arg   |  416  |   42    |
   | `j'/`k' split with an explicit else arm        |  419  |   46    |
   | `b->unk_14 = 0x280000' BEFORE the +0xA store   |  415  |   20    |
   | +0xB6 addends as `p + 0x92' first              |  415  |   22    |
   | `D_800E296C = D_800E296C | 0x40'               |  415  |   20    |
   | a `q' pointer local for D_800E296C, either pos |  415  |   14    |
   | a `v2[0]' local for the p + 0x92 reload        |  416  |   15    |
   | `*(s32 *)((u8 *)b + 0x14)' plain cast          |  415  |   14    |
   | `*(s32 *)(p + 0xA0) = 0' at slots 0..7         |  414-415 | 14-17 |
   | `D_800E296C |= 0x40' at slots 0..6             |  414-415 | 17-22 |

   FOUR FACTS THIS BODY ESTABLISHED, all worth carrying:

   1. THE TWO HALFWORDS AT 0x10/0x12($sp) ARE A LOCAL ARRAY, NOT SPILLS.
      `s16 v2[2]' gives `sh 0x10($sp)' / `sh 0x12($sp)' / `lh 0x10($sp)'
      directly; two `s16' scalars get registers instead and the frame comes
      out -0x58 against the target's -0x60. Worth 5 words and ~100 rows. A
      pair of stack halfwords written and read back with no call between them
      is an ARRAY, because gcc 2.7.2 never puts an array in a register.
   2. `nx < 0x1020 && nx > 0' IS FOLDED INTO ONE UNSIGNED RANGE TEST.
      `fold_truthop' rewrites it as `(unsigned)(nx - 1) < 0x101F' --
      `addiu $v0, $v1, -1' / `sltiu $v0, $v0, 0x101F' -- where the target has
      `slti'/`beqz' then `blez'. Nesting the two `if's is 2 words per site and
      there are two sites. This is the RANGE-MERGE sibling of the documented
      two-bit-tests case, and it fires on ONE local with TWO relational tests,
      which the existing lever does not say.
   3. A VALUE USED BEFORE AND AFTER A FLAG ASSIGNMENT NEEDS ITS OWN LOCAL.
      `ox' is the jitter AND the success flag (set to 0xB), so
      `*(u16 *)((u8 *)b + 0x2) += ox * 0x40' constant-folds to
      `addiu $v0, $v0, 0x2C0' where the target has `addu $v0, $v0, $s5'.
      Separate `sx = ox * 0x40' / `sy = oy * 0x40' locals are what the two
      extra callee-saved registers ($s5 and $s3) in the target's TEN saves
      are for.
   4. THE +0xB6 LOAD HOISTS ABOVE THE `b->unk_14' STORE ONLY IF THE 0xA STORE
      IS WRITTEN FIRST. `*(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0xB6);' before
      `b->unk_14 = 0x280000;' is 14 rows; the other order is 20, and the two
      stores still EMIT in the 0x14-then-0xA order because `true_dependence'
      sees no conflict between a COMPONENT_REF and a plain cast off the same
      pointer. Reading the emitted store order as the source order costs 6
      rows here.

   THE TABLE SHAPES, for the integrator -- this function owns TWO:

   jtbl_b25_801708B0: 6 .words = 5 CASES + 1 PAD, minval 0
   (`sltiu $v0, $v1, 0x5' with no subtraction), cases 0..4, five distinct
   targets, NO default among the entries -- the range check's `beqz $v0' goes
   to .Lb25_80174B4C, which is the function's EPILOGUE. Entry order:
   .Lb25_80174600 / .Lb25_80174708 / .Lb25_80174728 / .Lb25_80174A50 /
   .Lb25_80174A84. CASE 0 FALLS THROUGH INTO CASE 1 AND CASE 1 INTO CASE 2:
   case 0's last instruction is the `jal func_800A56E0' delay slot one word
   above .Lb25_80174708, and case 1's `sh $v0, 0x1C($s7)' one word above
   .Lb25_80174728, with no `j' between either pair.

   jtbl_b25_801708C8: 22 .words = 22 CASES + 0 PAD, minval 0
   (`sltiu $v0, $v1, 0x16' with no subtraction), cases 0..21, SEVEN distinct
   targets, and this one DOES HAVE A DEFAULT: the range check's
   `beqz $v0, .Lb25_801747D0' goes to a real block (`lhu $v0, 0x1A($s7)' /
   `addiu $v0, $v0, 0x20') that no table entry points at, so `default:' is
   written out. Entry grouping, in source order:
       0            -> .Lb25_801747E0  (an EMPTY arm: that label is the code
                                        AFTER the inner switch)
       1,2          -> .Lb25_80174758  (+0xAE = 0x30, +0x1A += 0x100)
       3..8         -> .Lb25_8017476C  (+0xAE = 0x28, +0x1A += 0x200)
       9,10         -> .Lb25_80174780  (+0xAE = 0x30, +0x1A += 0x100)
       11..14       -> .Lb25_80174794  (+0xAE = 0x70, +0x1A += 0x100)
       15,16,17     -> .Lb25_801747A8  (+0xAE = 0x60, +0x1A += 0x200)
       18..21       -> .Lb25_801747BC  (+0xAE = 0x50, +0x1A += 0x80)
       default      -> .Lb25_801747D0  (+0x1A += 0x20, no +0xAE store)
   Note that the 1,2 group and the 9,10 group have IDENTICAL arms (0x30 and
   +0x100) and SEPARATE table targets, so they must be written as two arm
   groups and not merged -- merging them would permute the table.

   The 0x20-byte copy at the top is the eight-direction unit-vector table
   D_b25_8017088C (already landed in this unit as `const u8 []') read into a
   local through a byte-array struct, exactly the landed
   func_s31_800247D0 idiom: `lwl'/`lwr' and `swl'/`swr' in 3+3+2 word chunks
   is `emit_block_move' with DECL_ALIGN 1, which is what the struct of `u8'
   gives and what a `u32 w[8]' struct would NOT give.

   ADDRESSES A CONSTANT BASE: no. Every global here is a declared symbol
   (D_80083160, D_80083460, D_800E296C, D_800E2368, D_800814A0,
   D_b25_8017088C) and every one is reached through a `%hi'/`%lo' pair.

DECLARATIONS
typedef struct UnkDngB25Dirs
{
    u8 unk_00[0x20];
} UnkDngB25Dirs;
extern s32 D_8008346C;
extern u32 D_800E296C;
extern u8 D_800E2368[];
s16 func_800A45D8(u16, u16, s32);
void func_800ACF88(u8 *);
void func_800A2FE0(u8 *);
void func_800A32A4(u8 *);
void func_8009A028(u8 *);
s32 func_80064584(s32);
s32 func_800A56E0(s32);
END DECLARATIONS
*/
void func_b25_8017450C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 v2[2];
    u8 *t;
    UnkDngB25Dirs dd;
    u8 *g;
    u8 *g2;
    u8 *h;
    s16 x;
    s16 k;
    s32 m;
    s32 i;
    s32 w;
    s32 ox;
    s32 oy;
    s32 sx;
    s32 sy;
    s32 nx;
    s32 ny;

    dd = *(UnkDngB25Dirs *)D_b25_8017088C;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            g2 = (u8 *)&D_80083460;
            if (*(s32 *)(g2 + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g2 + 0x10) &= 0x7FFFFFFF;
            }
            func_800A2FE0(d);
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            break;
        }
        h = (u8 *)&D_80083160;
        for (x = 0; x < 0x1000; x += 0x200)
        {
            if ((((*(s16 *)(h + 0xC8) + x + 0x100) >> 9) & 7) == 2)
            {
                *(s16 *)(d + 0x2A) = x;
            }
        }
        *(u8 **)(c + 0x2C) = D_800E2368;
        func_80047784(c, D_800E2368[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(p + 0x98) |= 0x8;
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        *(u16 *)(p + 0x92) += *(u16 *)(d + 0x88);
        *(s16 *)(d + 0x88) = 0;
        *(s32 *)(p + 0xA0) = 0;
        p[0x9B] += 1;
        *(u16 *)(p + 0xB6) = *(u16 *)((u8 *)b + 0xA) + *(u16 *)(p + 0x92);
        D_800E296C |= 0x40;
        func_800A56E0(0x807);
        /* fallthrough */
    case 1:
        *(s16 *)(p + 0x96) = 0;
        p[0x9B] += 1;
        *(u16 *)(c + 0x22) = 0xFFEC;
        *(s16 *)(c + 0x1C) = 0x800;
        /* fallthrough */
    case 2:
        switch (*(s16 *)(p + 0x96))
        {
        case 0:
            break;
        case 1:
        case 2:
            *(s16 *)(p + 0xAE) = 0x30;
            *(u16 *)(c + 0x1A) += 0x100;
            break;
        case 3:
        case 4:
        case 5:
        case 6:
        case 7:
        case 8:
            *(s16 *)(p + 0xAE) = 0x28;
            *(u16 *)(c + 0x1A) += 0x200;
            break;
        case 9:
        case 10:
            *(s16 *)(p + 0xAE) = 0x30;
            *(u16 *)(c + 0x1A) += 0x100;
            break;
        case 11:
        case 12:
        case 13:
        case 14:
            *(s16 *)(p + 0xAE) = 0x70;
            *(u16 *)(c + 0x1A) += 0x100;
            break;
        case 15:
        case 16:
        case 17:
            *(s16 *)(p + 0xAE) = 0x60;
            *(u16 *)(c + 0x1A) += 0x200;
            break;
        case 18:
        case 19:
        case 20:
        case 21:
            *(s16 *)(p + 0xAE) = 0x50;
            *(u16 *)(c + 0x1A) += 0x80;
            break;
        default:
            *(u16 *)(c + 0x1A) += 0x20;
            break;
        }
        if (*(u16 *)(c + 0x1A) >= 0x1000)
        {
            *(u16 *)(c + 0x1A) -= 0x1000;
        }
        k = *(u16 *)(c + 0x1A);
        k -= 0x400;
        if (k < 0)
        {
            k += 0x1000;
        }
        m = *(s16 *)(p + 0xAE) * func_80064584(k) << 4;
        w = *(s16 *)(p + 0xAE) * func_800644B8(k) << 4;
        t = dd.unk_00;
        v2[0] = *(u16 *)(t + ((((*(s16 *)(d + 0x2A) - 0x400) >> 9) & 7) << 2));
        v2[1] = *(u16 *)(t + ((((*(s16 *)(d + 0x2A) - 0x400) >> 9) & 7) << 2) + 0x2);
        *(s32 *)(p + 0x90) += w;
        b->unk_0C = v2[0] * m >> 4;
        b->unk_10 = v2[1] * m >> 4;
        if ((*(s16 *)(p + 0x96))++ >= 0x2D)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
            *(s16 *)(c + 0x1A) = 0x800;
            b->unk_10 = 0;
            b->unk_0C = 0;
            *(s16 *)((u8 *)b + 0x2) = (c[0x24] << 6) + 0x20;
            *(s16 *)((u8 *)b + 0x6) = (c[0x25] << 6) + 0x20;
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0xB6);
            b->unk_14 = 0x280000;
            *(s32 *)(p + 0xA0) = 0;
            *(u16 *)(p + 0x92) = *(u16 *)(p + 0xB6) - 0x12C;
            *(s16 *)(c + 0x1C) = 0x600;
            for (i = 0; i < 0xA; i++)
            {
                ox = func_b25_8017165C(7) - 3;
                oy = func_b25_8017165C(6) - 2;
                sx = ox * 0x40;
                nx = *(s16 *)((u8 *)b + 0x2) + sx;
                if (nx < 0x1020)
                {
                    if (nx > 0)
                    {
                        sy = oy * 0x40;
                        ny = *(s16 *)((u8 *)b + 0x6) + sy;
                        if (ny < 0x1020)
                        {
                            if (ny > 0)
                            {
                                if (func_800A45D8(nx, ny, -0x200) == 0)
                                {
                                    if ((s16)func_800BCB04((u16)nx, (u16)ny, -0x200) >= 0x200)
                                    {
                                        ox = 0xB;
                                        i = 0xB;
                                        *(u16 *)((u8 *)b + 0x2) += sx;
                                        *(u16 *)((u8 *)b + 0x6) += sy;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            if (ox != 0xB)
            {
                p[0x9B] = 4;
                b->unk_14 = 0;
                *(u16 *)(c + 0x14) |= 0x80;
            }
        }
        break;
    case 3:
        if ((*(s16 *)(p + 0x96))++ >= 0x1E)
        {
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] += 1;
        }
        break;
    case 4:
        if ((*(s16 *)(p + 0x96))++ >= 0xA)
        {
            if ((*(s32 *)(d + 0x14) & 0x4000) != 0)
            {
                if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
                {
                    func_800ACF88(d);
                }
            }
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            func_800A2FE0(d);
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
