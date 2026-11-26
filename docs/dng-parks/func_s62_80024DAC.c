/* PARK: func_s62_80024DAC -- ovl_dng_s62 -- agent-h208, batch 35.
   target 1130 words, candidate 1135 words, 550 aligned differences
   (ovlverify: 1130 / 1135 / 1117 differing masked -- read the ALIGNDIFF
   figure, the positional one is meaningless at a 5-word offset).
   relcand: ok, 14 data symbols and 44 calls all correct.

   WHAT IT IS. The step func_s62_80025F54 parks on every link of its
   nine-node chain, so this is the chain-link brain for the whole bank. It is
   ONE function with TWO top-level halves, selected by the payload's +0x20
   parent link:

     link == 0  -- this is the HEAD link, and it runs a 17-case state machine
                   on the payload's +0x30 (states 0..6 and 16 are real; 7..15
                   are jump-table padding to the default), then a shared
                   dress/step tail;
     link != 0  -- this is a FOLLOWER, and it just copies its parent's mover
                   and slot rotation, with the parent reached by ORing the top
                   bit back onto the stored link.

   The link is stored by func_s62_80025F54 masked with 0x7FFFFFFF when it
   comes from the D_80026BBC table and unmasked otherwise, and THAT BIT IS THE
   DISCRIMINATOR in the follower half: `link | 0x80000000' is the node, and
   `link & 0x80000000' chooses between reading the parent payload's +0x50
   trail slot and reading the parent mover directly.  Every call to
   func_8003DE58 in the head half projects the player (D_80082E80) through
   D_800E3D18 indexed by `((D_80083228 + player bearing + 0x100) >> 7) & 0x1C'.

   FRAME MAP -- read this before touching the body.
     target:   addiu $sp, $sp, -0x48      ra,s6,s5,s4,s3,s2,s1,s0   (8 saves)
               args 0x00..0x1F (eight outgoing words: func_8009CE1C takes 7)
               s16 sp20[3] at 0x20..0x25, pad, saves 0x28..0x44
               $s4 = p, $s5 = v, $s6 = s
     mine:     addiu $sp, $sp, -0x48      ra,s4,s3,s2,s1,s0         (6 saves)
               $s2 = p, $s3 = v, $s4 = s
   THE WHOLE REMAINING RESIDUE IS THAT TWO-REGISTER DEFICIT.  The target
   assigns FOUR non-parameter callee-saved registers ($s0..$s3) and mine
   assigns TWO, so every parameter name is shifted by two and ~500 of the 550
   rows are that rotation, not local faults.  I could not find the missing
   two long-lived locals.  What I ruled out, and it is worth not repeating:
     - it is NOT the follower half.  There the target holds o/$s0, r/$s1 and
       m/$s3 across its one call and so does mine; its $s2 is a ONE-INSN
       `* 10' temp that local-alloc put in an already-saved register, and the
       same pattern appears in the tail (`sll $s2, $v0, 2' then
       `addiu $v0, $s2, 0x400'), so $s2 is evidence of a save that exists for
       another arm, not of a source local.
     - it is NOT case 1 either, for the same reason: the target's bearing
       value lives in $s2 with NO call in its range.
   So the save count comes from whichever arm genuinely needs four globals at
   once, and the next agent should find it by compiling ONE ARM AT A TIME with
   the others stubbed, which is the experiment I did not have budget for.

   JUMP TABLE -- jtbl_s62_80024020, and this is checked.
   17 .word = 17 cases + 0 pad (odd case count, so no pad: the corrected pad
   rule).  Dispatch is a bare `sltiu $v0, $v1, 0x11' with NO minval
   subtraction, so the case set is {0..16}.  Address 0x80024020 mod 8 == 0.
   8 distinct targets:
     case  0 -> .Ls62_80024E74     case  1 -> .Ls62_80024FAC
     case  2 -> .Ls62_80025234     cases 3,4 -> .Ls62_8002542C
     case  5 -> .Ls62_800255DC     case  6 -> .Ls62_800257E8
     cases 7..15 -> .Ls62_80025C6C cases 16 -> .Ls62_80025B78
   .Ls62_80025C6C IS THE DEFAULT: it is also the target of the range-check
   `beqz', so cases 7..15 are gcc padding the table to maxval and MUST NOT be
   written as empty `break' arms.  It is ALSO the code that physically follows
   the switch, so the body needs NO `default:' label at all -- the shared tail
   after the switch serves as the default, which is why case 16 ends in a
   `return' on both its paths instead of a `break'.
   jtcheck agrees: bound 17 == label count 17, no disagreement.

   CASE 0 FALLS THROUGH INTO CASE 1 and that is worth 6 rows if you add a
   `break' (measured on the smaller func_s62_80024064 in the same unit at
   175 words / 6 rows; not re-measured here).

   MEASURED FACTS, in the order they were worth words.  Every figure is
   `built words / aligned differences' for the WHOLE body, so they are only
   comparable to each other, not to a clean body -- this park never reached 0
   and all of its prices were taken with the register deficit live.

   | change                                                  | words | rows |
   | the body as parked                                      | 1135  | 550  |
   | the three bearing clamps as if/else, not a ternary      | 1141  | 575  |
   | D_80082E80[0x24] and [0x25] read twice, no locals       | 1141  | 568  |
   | `s32 n' for the six -1 countdowns instead of `s16 n'    | 1139  | 620  |
   | the trail shift with a `c += 8' cursor and c+0x48/0x50  | 1143  | 687  |
   | the trail shift counting UP (for i = 0; i < 1; i++)     | 1143  | 695  |
   | case 0 divisor read back as *(s16 *)(p + 0x34)          | 1136  | 553  |
   | case 0 `- 0x50' in one expression, either addend order  | 1136  | 553  |
   | ... with a (s32) cast round the subtraction             | 1147  | 587  |
   | func_8003DE58's player base through a u8 * local        | 1137  | 563  |
   | ... written fully inline                                | 1137  | 561  |
   | one shared u8 * local for both D_80083780 sites in 3/4  | 1146  | 596  |

   SIX FACTS THAT ARE SHAPE, NOT PRESSURE -- carry these, they each bought
   words:

   (1) THE TRAIL SHIFT IS A ONE-ITERATION LOOP, AND IT IS A LOOP.
   `addu $a3, $zero, $zero' then `addiu $a3, $a3, -0x1' / `bgez $a3' runs the
   body EXACTLY ONCE, and the 8 bytes move from +0x48 to +0x50 through two
   lwl/lwr pairs and two swl/swr pairs, i.e. an 8-byte struct of ALIGN 2.
   The spelling that reproduces all eight words plus the three loop words is
     for (i = 0; i >= 0; i--)
         *(UnkDngS62Trail *)(p + 0x50 + i * 8) = *(UnkDngS62Trail *)(p + 0x48 + i * 8);
   with UnkDngS62Trail = struct { s16 h[4]; }.  A POINTER CURSOR IS WRONG:
   `c = p; ... *(T *)(c + 0x50) = *(T *)(c + 0x48); c -= 8;' biases the biv to
   `p + 0x50' and the displacements come out -8/0 instead of 0x48/0x50
   (1143 / 687).  Counting up is also wrong (1143 / 695) -- an up loop emits
   `addiu +1' / `blez', NOT the reversed form, because the trip count is 1 and
   check_dbra_loop has nothing to gain.  The one-iteration bound is almost
   certainly a macro, and that is unrecoverable; write the 1.

   (2) EVERY `-1' COUNTDOWN LOCAL IS `s16', not `s32'.  There are six of them
   (+0x34 three times, +0x46, +0x8A twice, +0x9A) and all six tests emit
   `sll $vX, $vX, 16' before the `bgtz'/`bnez', which is the HImode promotion
   of the value just stored.  `s32' loses that word at every site: 1139 / 620
   against 1135 / 550.

   (3) EACH BEARING CLAMP IS A TERNARY WITH ONE STORE.  The pattern
       if (abs(b - *(s16 *)(p + 0x38)) > 0x80)
           *(s16 *)(p + 0x38) = (*(s16 *)(p + 0x96) != 0) ? (... + 0x80) : (... - 0x80);
       else
           *(s16 *)(p + 0x38) = b;
   appears in cases 1, 2 and 6.  Written as a nested if/else with a store in
   each arm it is 1141 / 575 -- two words per site, because the target reaches
   ONE `sh 0x38($s4)' through a `j' from the `+ 0x80' arm and jump2 cannot
   merge two stores of different registers.

   (4) THE DIVISIONS BY 16 ARE DIVISIONS BY A VARIABLE WHOSE VALUE cse KNOWS.
   `addiu $v1, $zero, 0x10' followed by `div' is NOT a literal divisor --
   expand_divmod would have strength-reduced a literal four passes before cse.
   Both sites divide by the +0x34 counter that was set to 0x10 two statements
   earlier.  In case 0 the reload survives when you write
   `/ *(s16 *)(p + 0x34)' (1136 / 553) and the constant appears when you write
   `dv = 0x10; *(s16 *)(p + 0x34) = dv; ... / dv' (1135 / 550), because the
   intervening store to +0x2C invalidates cse's entry for the MEM but not the
   local.  Case 6's site still has the memory form and has not been swept.

   (5) THE THREE BASE ADDRESSES NEED u8 * LOCALS, ONE PER SITE, AND SHARING
   THEM COSTS.  D_80083780 is reached four times in three arms and the target
   materialises a FULL `lui'+`addiu' address each time (`addiu $s1, $s1,
   %lo(D_80083780)' etc), never a %lo folded into the load.  Case 3/4 has TWO
   such sites and they must be TWO LOCALS: one shared local is 1146 / 596
   against 1137 / 563.  Same for the two D_80082E80 byte reads at the end of
   case 3/4 -- read into locals `b' and `e' once each (1137 / 563) rather than
   twice from the array (1141 / 568).

   (6) func_800A07D0 IS TAKEN TWO DIFFERENT WAYS IN THIS UNIT, and this body
   needs the `s16' one.  Cases 1, 2 and 6 all sign-extend the result
   (`sll $v0, 16' / `sra $sN, $v0, 16') before masking it, which is the plain
   `s16 func_800A07D0(s16, s16, s16, s16)' declaration.  The landed
   func_s62_80025F54 in the same unit needs the OPPOSITE -- its site has no
   extension and wants the `s32' alias, priced there at 188 words / 3 rows for
   the s16 form.  Both declarations therefore have to coexist, which is why
   the DECLARATIONS block below has `func_800A07D0' and the unit already has
   `func_800A07D0i'.

   THREE THINGS THE NEXT AGENT MUST NOT MISS.

   A. func_s62_80024938 IS CALLED WITH FIVE ARGUMENTS FROM THIS BODY, twice
   (case 5 and case 6), the fifth being `(rand() & 3) | 4' passed at
   0x10($sp).  The landed func_s62_80024938 in csrc/ovl_dng_s62.c has FOUR
   parameters and does not read a fifth, so landing this body REQUIRES
   widening that prototype with an unread `s32' fifth parameter.  I widened it
   while working, then reverted it, so csrc is untouched -- but the 0x10($sp)
   store is in the listing at 80025770 and 80025A04 and it is real.

   B. THE FOLLOWER HALF'S `D_8000001E' IS NOT A SYMBOL.  splat names
   `lui $a0, 0x8000' as `%hi(D_8000001E)' and the following `lhu 0x1E($s0)' as
   `%lo(D_8000001E)'; the instruction pair is `or $s0, $v1, $a0' which is the
   0x80000000 bit being ORed back onto the stored link, and the loads at
   `%lo(D_8000000C)' / `%lo(D_80000008)' / `addiu %lo(D_80000020)' are plain
   +0xC / +0x8 / +0x20 displacements off that pointer.  Do not declare any of
   them.

   C. `rand' IS func_80069EF8 AND IT TAKES NO ARGUMENT HERE.  m2c renders one
   site as `func_80069EF8(temp_a0_4)' because `$a0' happens to hold the +0x42
   byte; that value is stored to +0x99 afterwards, so by the dead-after-call
   test it is a scheduling artefact, not an argument.  The same test clears
   `func_800A56E0(0x300, temp_a1)' in case 3/4 -- `$a1' is never set there.

   HOW THIS BODY WAS GOT. tools/m2c on the listing gave a structurally
   complete draft in one shot (the whole switch, both halves, all eight arms);
   dngtwin finds NO carved sibling at 0.70 and dngcallee's best tier is 8 of
   26 symbols reached by one function (func_s69_800245A0), which was not
   opened.  The draft needed: the five-argument calls restored, UnkVec's
   missing +0x16 field spelled as a cast, the two `D_8000001E' symbols read as
   the sign bit, and `rand'.

DECLARATIONS
extern u8 D_80026BD4[];
extern u8 D_80082E80[];
extern u8 *D_800E3D7C;
extern u8 *D_800E3D18;
extern s16 D_80083228;
s16 func_800A07D0(s16, s16, s16, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
void func_800B8D64(s32, s32, s32);
void func_s62_800262F4(u8 *s, s16 *b, s16 lo, s16 hi);
void func_s62_800263C0(u8 *s, s16 d);
u8 *func_s62_80026444(u8 *p);
s32 func_s62_80026694(void *a, u8 *b, s16 c, s32 d);
typedef struct UnkDngS62Trail
{
    s16 h[4];
} UnkDngS62Trail;
END DECLARATIONS
   Everything else this body needs is already in csrc/ovl_dng_s62.c's one
   declaration block after batch 35: D_80083780, D_80026BC8, D_80027C94,
   D_80027C96, D_80027C98, D_800269D0, func_800A56E0, func_8003DE58,
   func_8004491C and the forward declarations of the in-unit callees.
   func_s62_80024938 must additionally gain its fifth parameter -- see A. */
void func_s62_80024DAC(u8 *p, UnkVec *v, UnkSlot *s)
{
    u8 *g;
    u8 *c;
    u8 *o;
    u8 *q;
    UnkVec *m;
    UnkSlot *r;
    s32 i;
    s32 b;
    s32 e;
    s32 t;
    s16 n;
    s16 dv;
    s32 n2;
    u8 *w;
    u8 *w2;
    u8 *w3;
    u8 *w4;
    s16 sp20[3];

    D_800269F8++;
    for (i = 0; i >= 0; i--)
    {
        *(UnkDngS62Trail *)(p + 0x50 + i * 8) = *(UnkDngS62Trail *)(p + 0x48 + i * 8);
    }
    *(s16 *)(p + 0x48) = *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x4A) = *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x4C) = *(u16 *)((u8 *)v + 0xA);
    if (*(s32 *)(p + 0x20) == 0)
    {
        switch (*(s16 *)(p + 0x30))
        {
        case 0:
            w = D_80083780;
            func_8003DE58(
                *(u8 **)(D_800E3D18 +
                         (((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 7) & 0x1C)),
                (UnkSlot *)D_80082E80, (s16 *)(p + 0x28), 0);
            e = func_80064584(*(s16 *)(D_800E3D7C + 0x2A));
            *(s16 *)(p + 0x28) =
                *(u16 *)(p + 0x28) +
                ((*(u16 *)(w + 0x2) - (e >> 4)) +
                 (func_80064584(*(s16 *)(D_800E3D7C + 0x2A) - 0x400) >> 4));
            e = func_800644B8(*(s16 *)(D_800E3D7C + 0x2A));
            *(s16 *)(p + 0x2A) =
                *(u16 *)(p + 0x2A) +
                ((*(u16 *)(w + 0x6) - (e >> 4)) +
                 (func_800644B8(*(s16 *)(D_800E3D7C + 0x2A) - 0x400) >> 4));
            dv = 0x10;
            *(s16 *)(p + 0x34) = dv;
            t = *(u16 *)(p + 0x2C) - 0x50;
            *(s16 *)(p + 0x2C) = *(u16 *)(D_800E3D7C + 0x88) + t;
            v->unk_14 = ((*(s16 *)(p + 0x2C) - *(s16 *)((u8 *)v + 0xA)) << 16) / dv;
            *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) + 1;
            /* fall through */
        case 1:
            b = func_800A07D0(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6), *(s16 *)(p + 0x28),
                              *(s16 *)(p + 0x2A));
            t = *(u16 *)(p + 0x38);
            *(s16 *)(p + 0x38) = ((t & 0x800) != 0) ? (t | 0xF800) : (t & 0x7FF);
            b = ((b & 0x800) != 0) ? (b | -0x800) : (b & 0x7FF);
            if (__builtin_abs(*(s16 *)(p + 0x38) - b) > 0x800)
            {
                *(s16 *)(p + 0x38) = (b & -0x1000) | (*(u16 *)(p + 0x38) & 0xFFF);
            }
            if (__builtin_abs(b - *(s16 *)(p + 0x38)) > 0x80)
            {
                *(s16 *)(p + 0x38) = (*(s16 *)(p + 0x96) != 0) ? (*(s16 *)(p + 0x38) + 0x80)
                                                                : (*(s16 *)(p + 0x38) - 0x80);
            }
            else
            {
                *(s16 *)(p + 0x38) = b;
            }
            v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
            v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
            n = *(u16 *)(p + 0x34) - 1;
            *(s16 *)(p + 0x34) = n;
            if (n <= 0)
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x2C);
                *(s16 *)(p + 0x34) = 0;
                v->unk_14 = 0;
            }
            *(s16 *)((u8 *)D_800814A8 + 0x96) = 2;
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            if (__builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)(p + 0x28)) < 0x40 &&
                __builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)(p + 0x2A)) < 0x40 &&
                __builtin_abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)(p + 0x2C)) < 0x40)
            {
                func_8003DE58(
                    *(u8 **)(D_800E3D18 +
                             (((D_80083228 + *(s16 *)(D_800E3D7C + 0x2A) + 0x100) >> 7) & 0x1C)),
                    (UnkSlot *)D_80082E80, (s16 *)(p + 0x28), 0);
                w = D_80083780;
                *(s16 *)(p + 0x28) = *(u16 *)(p + 0x28) + *(u16 *)(w + 0x2);
                *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x2A) + *(u16 *)(w + 0x6);
                t = *(u16 *)(p + 0x2C) - 0x50;
                *(s16 *)(p + 0x2C) = *(u16 *)(D_800E3D7C + 0x88) + t;
                *(s16 *)((u8 *)D_800814A8 + 0xA6) = *(u16 *)((u8 *)D_800814A8 + 0xA6) - 1;
                *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) + 1;
            }
            break;
        case 2:
            b = func_800A07D0(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6), *(s16 *)(p + 0x28),
                              *(s16 *)(p + 0x2A));
            t = *(u16 *)(p + 0x38);
            *(s16 *)(p + 0x38) = ((t & 0x800) != 0) ? (t | 0xF800) : (t & 0x7FF);
            b = ((b & 0x800) != 0) ? (b | -0x800) : (b & 0x7FF);
            if (__builtin_abs(*(s16 *)(p + 0x38) - b) > 0x800)
            {
                *(s16 *)(p + 0x38) = (b & -0x1000) | (*(u16 *)(p + 0x38) & 0xFFF);
            }
            if (__builtin_abs(b - *(s16 *)(p + 0x38)) > 0x80)
            {
                *(s16 *)(p + 0x38) = (*(s16 *)(p + 0x96) != 0) ? (*(s16 *)(p + 0x38) + 0x80)
                                                                : (*(s16 *)(p + 0x38) - 0x80);
            }
            else
            {
                *(s16 *)(p + 0x38) = b;
            }
            v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
            v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
            n = *(u16 *)(p + 0x34) - 1;
            *(s16 *)(p + 0x34) = n;
            if (n <= 0)
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x2C);
                *(s16 *)(p + 0x34) = 0;
                v->unk_14 = 0;
            }
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            *(s16 *)((u8 *)D_800814A8 + 0x96) = 2;
            if (__builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)(p + 0x28)) < 0x20 &&
                __builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)(p + 0x2A)) < 0x20)
            {
                *(s16 *)(p + 0x34) = 4;
                *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) + 1;
                *(s16 *)((u8 *)D_800814A8 + 0x96) = 0;
                *(s16 *)(p + 0x38) = *(u16 *)(D_800E3D7C + 0x2A);
                D_80026B28 = 0;
                *(s16 *)(p + 0x44) = (*(u16 *)(D_800E3D7C + 0x2A) >> 9) & 7;
            }
            break;
        case 3:
        case 4:
            if (func_8003DE58((u8 *)((UnkSlot *)D_80082E80)->unk_08, (UnkSlot *)D_80082E80, sp20,
                              0) != NULL)
            {
                if (*(s16 *)(p + 0x30) == 3)
                {
                    w2 = D_80083780;
                    func_800B8D64(*(s16 *)(w2 + 0x2) + sp20[0], *(s16 *)(w2 + 0x6) + sp20[1],
                                  *(s16 *)(w2 + 0xA) + sp20[2]);
                    *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) + 1;
                }
                w3 = D_80083780;
                func_s62_80024B2C(*(u16 *)(w3 + 0x2) + *(u16 *)&sp20[0],
                                  *(u16 *)(w3 + 0x6) + *(u16 *)&sp20[1],
                                  *(u16 *)(w3 + 0xA) + *(u16 *)&sp20[2],
                                  *(s16 *)(D_800E3D7C + 0x2A), 0);
            }
            v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
            v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
            *(s16 *)((u8 *)v + 0x16) = 0x16;
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            if (*(s16 *)(D_800E3D7C + 0x88) < *(s16 *)((u8 *)v + 0xA))
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(D_800E3D7C + 0x88);
            }
            n = *(u16 *)(p + 0x34) - 1;
            *(s16 *)(p + 0x34) = n;
            if (n <= 0)
            {
                w4 = D_80082E80;
                b = w4[0x24];
                *(s16 *)(p + 0x40) = b;
                *(u8 *)(p + 0x98) = b;
                e = w4[0x25];
                *(s16 *)(p + 0x30) = 5;
                *(s16 *)(p + 0x46) = 3;
                *(s16 *)(p + 0x34) = 0;
                *(s16 *)(p + 0x42) = e;
                *(u8 *)(p + 0x99) = e;
                func_800A56E0(0x300);
            }
            break;
        case 5:
            v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
            v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
            v->unk_14 = 0;
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            b = *(s16 *)((u8 *)v + 0x2);
            if (b < 0)
            {
                b += 0x3F;
            }
            *(s16 *)(p + 0x40) = b >> 6;
            e = *(s16 *)((u8 *)v + 0x6);
            if (e < 0)
            {
                e += 0x3F;
            }
            e >>= 6;
            *(s16 *)(p + 0x42) = e;
            if (*(u8 *)(p + 0x98) != *(s16 *)(p + 0x40) || *(u8 *)(p + 0x99) != e)
            {
                g = (u8 *)D_800814A8;
                c = *(u8 **)(g + 0x5C) + 0x20;
                while (c != (u8 *)D_800814A8)
                {
                    o = *(u8 **)(c - 0x14);
                    if (*(u8 *)(o + 0x24) == *(s16 *)(p + 0x40) &&
                        *(u8 *)(o + 0x25) == *(s16 *)(p + 0x42) &&
                        __builtin_abs(*(s16 *)(c + 0x88) - *(s16 *)((u8 *)v + 0xA)) < 0x80)
                    {
                        q = c - 0x20;
                        if ((*(u16 *)(q + 0x1E) & 0x2000) == 0)
                        {
                            func_8009CE1C(c, 0xC, D_80027C96, 0xA,
                                          (s16)(*(u16 *)(p + 0x44) << 9), (u32)D_80027C98, 2);
                            m = *(UnkVec **)(c - 0x18);
                            func_s62_80024938(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6),
                                              *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x38),
                                              (rand() & 3) | 4);
                            *(u16 *)(q + 0x1E) |= 0x2000;
                        }
                    }
                    c = *(u8 **)(c + 0x5C) + 0x20;
                }
                n = *(u16 *)(p + 0x46) - 1;
                *(s16 *)(p + 0x46) = n;
                *(u8 *)(p + 0x98) = *(u8 *)(p + 0x40);
                *(u8 *)(p + 0x99) = *(u8 *)(p + 0x42);
                if (n <= 0)
                {
                    *(s16 *)(p + 0x8A) = (rand() & 0x1F) + 0x10;
                    *(u32 *)(p + 0x9C) = 0;
                    *(s16 *)(p + 0x30) = *(u16 *)(p + 0x30) + 1;
                }
            }
            break;
        case 6:
            if (*(u32 *)(p + 0x9C) != 0)
            {
                m = *(UnkVec **)(*(u8 **)(p + 0x9C) - 0x18);
                b = func_800A07D0(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6),
                                  *(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6));
                t = *(u16 *)(p + 0x38);
                *(s16 *)(p + 0x38) = ((t & 0x800) != 0) ? (t | 0xF800) : (t & 0x7FF);
                b = ((b & 0x800) != 0) ? (b | -0x800) : (b & 0x7FF);
                if (__builtin_abs(*(s16 *)(p + 0x38) - b) > 0x800)
                {
                    *(s16 *)(p + 0x38) = (b & -0x1000) | (*(u16 *)(p + 0x38) & 0xFFF);
                }
                n = *(u16 *)(p + 0x9A) - 1;
                *(s16 *)(p + 0x9A) = n;
                if (n <= 0)
                {
                    *(s16 *)(p + 0x9A) = 0x1E;
                    *(s16 *)(p + 0x38) = b;
                }
                if (__builtin_abs(b - *(s16 *)(p + 0x38)) > 0x80)
                {
                    *(s16 *)(p + 0x38) = (*(s16 *)(p + 0x96) != 0)
                                             ? (*(s16 *)(p + 0x38) + 0x80)
                                             : (*(s16 *)(p + 0x38) - 0x80);
                }
                else
                {
                    *(s16 *)(p + 0x38) = b;
                }
                if (*(s16 *)(p + 0x34) != 0)
                {
                    n = *(u16 *)(p + 0x34) - 1;
                    *(s16 *)(p + 0x34) = n;
                    if (n == 0)
                    {
                        v->unk_14 = 0;
                        *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)m + 0xA) - 0x40;
                        *(s16 *)(p + 0x8A) = 0;
                    }
                }
                else
                {
                    n = *(u16 *)(p + 0x8A) + 1;
                    *(s16 *)(p + 0x8A) = n;
                    *(s16 *)((u8 *)v + 0x16) = func_800644B8((n << 16) >> 9) >> 9;
                }
                if (__builtin_abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)m + 0x2)) < 0x20 &&
                    __builtin_abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)m + 0x6)) < 0x20)
                {
                    q = *(u8 **)(p + 0x9C) - 0x20;
                    func_8009CE1C(*(u8 **)(p + 0x9C), 0xC, D_80027C96, 0xA,
                                  (s16)(*(u16 *)(p + 0x44) << 9), (u32)D_80027C98, 2);
                    func_800A56E0(0x300);
                    func_s62_80024938(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6),
                                      *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x38),
                                      (rand() & 3) | 4);
                    *(u16 *)(q + 0x1E) |= 0x2000;
                    *(u32 *)(p + 0x9C) = 0;
                    *(s16 *)(p + 0x8A) = (rand() & 0x1F) + 0x10;
                }
                v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
                v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
                if (*(s16 *)(p + 0x38) == b)
                {
                    v->unk_0C += v->unk_0C >> 1;
                    v->unk_10 += v->unk_10 >> 1;
                }
            }
            else
            {
                v->unk_14 = 0;
                n = *(u16 *)(p + 0x8A) - 1;
                *(s16 *)(p + 0x8A) = n;
                if (n <= 0)
                {
                    *(u8 **)(p + 0x9C) = func_s62_80026444(D_80082E80);
                    if (*(u32 *)(p + 0x9C) == 0)
                    {
                        *(s16 *)(p + 0x30) = 0x10;
                        *(s16 *)(p + 0x8A) = 8;
                    }
                    else
                    {
                        m = *(UnkVec **)(*(u8 **)(p + 0x9C) - 0x18);
                        *(s16 *)(p + 0x34) = 0x10;
                        v->unk_14 = ((*(s16 *)((u8 *)m + 0xA) - (*(s16 *)((u8 *)v + 0xA) + 0x40))
                                     << 16) /
                                    *(s16 *)(p + 0x34);
                        *(s16 *)(p + 0x9A) = 0x3C;
                    }
                }
                else
                {
                    *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) + 0x80;
                }
                v->unk_0C = func_80064584(*(s16 *)(p + 0x38)) << 8;
                v->unk_10 = func_800644B8(*(s16 *)(p + 0x38)) << 8;
            }
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            break;
        case 16:
            v->unk_0C -= v->unk_0C >> 3;
            v->unk_10 -= v->unk_10 >> 3;
            if (*(s16 *)(p + 0x3E) - 0x10 < *(s16 *)((u8 *)v + 0xA))
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x3E) - 0x10;
            }
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
            s->unk_0C[0] -= s->unk_0C[0] / *(s16 *)(p + 0x8A);
            s->unk_0C[1] -= s->unk_0C[1] / *(s16 *)(p + 0x8A);
            s->unk_0C[2] -= s->unk_0C[2] / *(s16 *)(p + 0x8A);
            n = *(u16 *)(p + 0x8A) - 1;
            *(s16 *)(p + 0x8A) = n;
            if (n <= 0)
            {
                D_80027C94 = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            return;
        }
        s->unk_1A = func_800A07D0(0, 0, *(s16 *)((u8 *)v + 0xE), *(s16 *)((u8 *)v + 0x12)) - 0x400;
        t = *(u16 *)((u8 *)s + 0x1C);
        s->unk_16 = *(s16 *)((u8 *)v + 0x16) * 4 + 0x400;
        t += (0x2000 - t) >> 2;
        n2 = s->unk_0C[0] + 8;
        s->unk_0C[0] = n2;
        s->unk_1C = t;
        s->unk_20 = t;
        s->unk_1E = t;
        if ((u32)(n2 & 0xFF) >= 0x81)
        {
            *(u32 *)s->unk_0C = 0x808080;
        }
        else
        {
            s->unk_0C[1] += 8;
            s->unk_0C[2] += 8;
        }
        func_s62_80026694(p - 0x20, v, 8, 0x300);
        func_s62_800262F4((u8 *)s, (s16 *)(p + 0x36), *(s16 *)(p + 0x3C), *(s16 *)(p + 0x3A));
        func_s62_80026240(p, (u8 *)s->unk_00);
        if (s->unk_04 == *(s16 *)(p + 0x3A))
        {
            *(s16 *)(p + 0x36) = -1;
        }
        else if (s->unk_04 == *(s16 *)(p + 0x3C))
        {
            *(s16 *)(p + 0x36) = 1;
        }
        func_s62_8002470C(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6),
                          *(s16 *)((u8 *)v + 0xA), *(s16 *)(p + 0x38),
                          D_80026BD4[s->unk_04]);
    }
    else
    {
        o = (u8 *)(*(s32 *)(p + 0x20) | 0x80000000);
        if ((*(u16 *)(o + 0x1E) & 0x8000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        r = *(UnkSlot **)(o + 0xC);
        m = *(UnkVec **)(o + 0x8);
        if ((*(s32 *)(p + 0x20) & 0x80000000) != 0)
        {
            q = o + 0x20;
            s->unk_1A = func_800A07D0(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6),
                                      *(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6)) -
                        0x400;
            s->unk_16 = (*(s16 *)((u8 *)m + 0xA) - *(s16 *)((u8 *)v + 0xA)) * 10 + 0x400;
            s->unk_14 = r->unk_14;
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)(q + 0x50);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)(q + 0x52);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(q + 0x54);
        }
        else
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)m + 0x2);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)m + 0x6);
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)m + 0xA);
            s->unk_16 = r->unk_16;
            s->unk_18 = r->unk_18;
            s->unk_1A = r->unk_1A;
            s->unk_14 = r->unk_14;
        }
        s->unk_1C = r->unk_1C;
        s->unk_1E = r->unk_1E;
        s->unk_20 = r->unk_20;
        *(u32 *)s->unk_0C = *(u32 *)r->unk_0C;
        if (D_80026BC8[*(s16 *)(p + 0x88)] != 0)
        {
            func_s62_800263C0((u8 *)s, r->unk_04);
            func_s62_80026240(p, (u8 *)s->unk_00);
        }
    }
    *(s16 *)(p + 0x94) = *(u16 *)(p + 0x94) + 1;
}

