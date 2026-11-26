/* PARK: func_b29_80172438  --  csrc/ovl_dng_b29.c  (DUNGEON.BIN carved bank 29)

   188 words against a 189-word target, 8 aligned differences, AND THE WHOLE
   RESIDUE IS ONE MISSING INSTRUCTION: the target copies func_8009B4B0's
   return value out of $v0 before using it, and this body uses $v0 directly.

       target   jal func_8009B4B0 ; addu $a1, $v0, $zero
                bnez $a1, .Lb29_801725D4 / sw $a1, 0x60($s4)
       here     jal func_8009B4B0
                bnez $v0, ...        / sw $v0, 0x60($s4)

   The other seven rows are the branch displacements that one missing word
   shifts.  So this is a LENGTH residue of exactly one word with no shape
   question left in it.

   MECHANISM, as far as I got.  cse records `(set (reg t) (reg $v0))' as a
   register equivalence; `make_regs_eqv' makes the HARD register the quantity's
   first register, every later use of `t' is canonicalised to $v0, and flow
   then deletes the dead copy.  The copy therefore survives only when the
   assignment is NOT a plain reg-to-reg SET.  That is the same mechanism this
   unit's landed func_b29_801761AC records for a modulo ("A MISSING move CAN BE
   A NARROW DECLARATION"), and it is also why the `s16 h = func_800BCB04(...)'
   and `s16 g = func_800BCB04(...)' copies in THIS body and in the landed
   func_b29_80170AE8 DO survive -- a HImode destination makes the SET_SRC a
   `subreg', not a REG.  func_8009B4B0 returns a POINTER, so the narrow-
   declaration cure is not available: every spelling of a 32-bit pointer
   assignment is a plain REG copy.

   CONFIRMED BY PERTURBATION: wrapping the store in `do { } while (0)' puts a
   NOTE_INSN_LOOP_END between the copy and its uses, the copy survives, and the
   body is 189 words -- the right length -- at 42 aligned differences, because
   the loop note also re-weights every reference inside the enclosing `for' and
   loop.c then hoists BOTH facing-table addresses instead of one (`&D_8006CCD8[g]'
   AND `&D_8006CCE8[g]' in $s7/$s6, where the target hoists only the first and
   recomputes the second inside the loop).  So the mechanism is proven and the
   device's side effect is what makes it unusable.  The remaining route is a
   spelling that breaks the cse equivalence without a loop note, or the
   permuter.

   WHAT DID MOVE: the 0x17 dress block's store order.  This body's tail is this
   unit's own landed func_b29_8017272C verbatim, and THAT BODY'S STATED FACT
   INVERTS HERE.  func_b29_8017272C records `*(s32 *)(p + 0x8C) = 0' between the
   p+0x9A and p+0x9B byte stores (and measures the other order at 2 differing).
   Here the only order that works is p+0x9B FIRST:

   | 0x9B = 0 ; 0x8C = 0 ; 0x9A = 0x17   (as below)        |  188 |   8 |
   | 0x8C = 0 ; 0x9B = 0 ; 0x9A = 0x17                     |  188 |  10 |
   | 0x9B = 0 ; 0x9A = 0x17 ; 0x8C = 0                     |  188 |  10 |
   | 0x9A = 0x17 ; 0x9B = 0 ; 0x8C = 0                     |  188 |  10 |
   | 0x8C = 0 ; 0x9A = 0x17 ; 0x9B = 0                     |  188 |  10 |
   | 0x9A = 0x17 ; 0x8C = 0 ; 0x9B = 0   (8017272C's fact) |  188 |  12 |

   DONOR: csrc/ovl_dng_b44.c's func_b44_80172504 (181 words, landed), named by
   dngcallee.py at 9 of 13 symbols -- dngtwin returns nothing at 0.70 for this
   stub.  All four of b44's named facts hold here unchanged and are NOT
   re-measured: the `s32 x = tx; s32 y = ty;' copies at the top of the walk
   (b44 priced the absence at 131 rows), plain `x << 6' with the mask left to
   func_800A44E0's `u16' parameters, `D_8006CCD8[g] & 0xFFFF' at the unsigned
   sites and `lh' at the func_800BCB04 site, and `g'/`dir' as two locals.
   Deltas from b44, all read off this listing: the walk is FOUR tiles not eight;
   there is no p+0xA8/p+0xAA publish, and in its place a p+0xB2 counter that is
   zeroed before the walk, incremented at the top of each iteration and
   DECREMENTED again on each of the three early exits; `*(u8 **)(d + 0x60) = t'
   is unconditional here where b44 stores it inside the `if'; the three early
   exits return 2, 3 and 3 and the normal fall-out returns 3; after the walk
   comes a func_800A2CB8 gate, a second D_80083462 read with the 0x2000 and 8
   tests, and then func_b29_8017272C's 0x17 dress tail returning 1.

   PROTOTYPE NOTE, SAID LOUDLY: this body calls func_800A2CB8 WITH ONE
   ARGUMENT.  csrc/ovl_dng_b29.c already declares `s16 func_800A2CB8(u8 *a,
   u8 *b);' and its landed func_b29_80171FFC calls it with two, so the
   declaration cannot be retyped.  The park uses a unit-local alias,
   `s16 func_800A2CB8_1(u8 *) __asm__("func_800A2CB8");', in the same style as
   the unit's existing func_800BCB04h alias.  No landed body changes.

   dngfam.py: 1 copy.

   MEASURED AND REJECTED, all with the dress order above in place
   (`built / target / aligned' from aligndiff.py; target is 189):

   | spelling                                               | words | aligned |
   | the body below                                         |  188  |    8    |
   | `*(u8 **)(d + 0x60) = t' INSIDE the `if (t != NULL)'    |  190  |   46    |
   | `if ((*(u8 **)(d + 0x60) = t) != NULL)' one expression  |  188  |    8    |
   | `u8 **q60' pointer local for the store                  |  188  |    8    |
   | re-read `*(u8 **)(d + 0x60)' in the test                |  188  |    8    |
   | `void *t'                                               |  188  |    8    |
   | `s32 tn' with (s32)/(s32 *) casts throughout            |  188  |    8    |
   | `*(s32 *)(d + 0x60) = (s32)t'                           |  188  |    8    |
   | an extra `u8 *u = t' alias for the store and the test   |  188  |    8    |
   | `if ((s16)(s32)t != 0)'                                 |  190  |   11    |
   | `t = NULL' before the walk (second set; cse drops it)   |  188  |    8    |
   | gate inverted, accumulate in the `== NULL' arm          |  188  |    8    |
   | `continue' in the `== NULL' arm                         |  188  |    8    |
   | do { *(u8 **)(d + 0x60) = t; } while (0)                |  189  |   42    |
   | `s16 i' for the walk counter                            |  193  |   35    |
   | `if (i == 4)' after the walk instead of `i >= 4'        |  188  |   10    |
   | `while (i < 4)' with `i++' at the end of the body       |  188  |    8    |
   | `for (i = 0; i < 4;)' with `i++' at the end of the body |  188  |    8    |
   | the same with `i++' before the two accumulates          |  188  |    8    |

   TWELVE SPELLINGS MEASURE 188/8 WITH A BIT-IDENTICAL RESIDUE, so they are one
   fact and not twelve, and no rearrangement of the local, its type, the store
   or the loop reaches the missing copy.

   UNMEASURED, in the order worth trying:
     1. the permuter, which I am not allowed to run -- this is a one-word
        LENGTH residue in a 189-word body and is exactly its shape.
     2. a spelling that makes the assignment a non-REG SET without a loop note:
        a bitfield or union member as the destination, or an aggregate copy.
     3. a `do { } while (0)' round the store COMBINED with something that pins
        `&D_8006CCE8[g]' inside the walk (loop.c's threshold is a control: b41's
        comment and today's func_b29_80170AE8 both show one more successful
        move changing what else hoists).  Batch 34's "a fix can be worse alone
        and necessary in combination" is the shape of this.

DECLARATIONS
s32 func_800A44E0(u16, u16, s16, u32);
u8 *func_8009B4B0(u8 *, u16, u16);
s32 func_800BCB04(s32, s32, s16);
s16 func_800A2CB8_1(u8 *) __asm__("func_800A2CB8");
s32 func_800A2B5C(void *d);
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 e);
void func_80047784(u8 *, s32, s32);
void func_800A4ACC(u8 *p);
extern u16 D_80083462;
extern s16 D_80083228;
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_801764A0[];
END DECLARATIONS */
s32 func_b29_80172438(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 i;
    s16 tx;
    s16 ty;
    u16 dir;
    s32 g;
    s16 h;
    s32 x;
    s32 y;

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        return -1;
    }
    g = (*(u16 *)(d + 0x2A) >> 9) & 7;
    dir = g;
    tx = c[0x24];
    ty = c[0x25];
    *(s16 *)(p + 0xB2) = 0;
    for (i = 0; i < 4; i++)
    {
        x = tx;
        y = ty;
        *(s16 *)(p + 0xB2) = *(u16 *)(p + 0xB2) + 1;
        if ((s16)func_800A44E0(x << 6, y << 6, *(s16 *)(d + 0x88), dir << 9) != 0)
        {
            *(s16 *)(p + 0xB2) = *(u16 *)(p + 0xB2) - 1;
            return 2;
        }
        h = func_800BCB04((((x + D_8006CCD8[g]) << 6) + 0x20) & 0xFFE0,
                          (((y + D_8006CCE8[g]) << 6) + 0x20) & 0xFFE0,
                          (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (h > 0x200)
        {
            *(s16 *)(p + 0xB2) = *(u16 *)(p + 0xB2) - 1;
            return 3;
        }
        if ((u16)(h - *(u16 *)(d + 0x88) + 0x3F) >= 0x7F)
        {
            *(s16 *)(p + 0xB2) = *(u16 *)(p + 0xB2) - 1;
            return 3;
        }
        t = func_8009B4B0(d, tx + (D_8006CCD8[g] & 0xFFFF), ty + (D_8006CCE8[g] & 0xFFFF));
        *(u8 **)(d + 0x60) = t;
        if (t != NULL)
        {
            break;
        }
        tx += D_8006CCD8[g] & 0xFFFF;
        ty += D_8006CCE8[g] & 0xFFFF;
    }
    if (i >= 4)
    {
        return 3;
    }
    if ((s16)func_800A2CB8_1(d) == 0)
    {
        return 0;
    }
    if ((D_80083462 & 0x2000) != 0)
    {
        return -1;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 8) != 0)
    {
        return -1;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    func_800C7930(d - 0x20, (u8 *)b, 8, 0x300);
    if ((s16)func_800A2B5C(d) != 0)
    {
        return -1;
    }
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x17;
    *(u8 **)(c + 0x2C) = D_801764A0;
    func_80047784(c, D_801764A0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    d[0x6D] -= 1;
    return 1;
}
