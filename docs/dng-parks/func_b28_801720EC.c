/* PARK: func_b28_801720EC, csrc/ovl_dng_b28.c, agent-h132, 2026-11-24.

   TARGET 160 words. BEST BUILT 160 words, 24 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py); ovlverify reports the same 24. 1 member, 160
   words (tools/decomp/dngfam.py), so there is no replication value beyond its
   own body. This function owns NO jump table: its listing has no
   `.section .rodata' block and the three failure shapes are a compare chain.
   tools/decomp/relcand.py: ok, 5 data symbols and 10 calls all agreeing with
   the listing.

   THE WHOLE RESIDUE IS ONE MISSING LONG-LIVED PSEUDO, and I could not build
   it from C. The target allocates NINE callee-saved values and the ninth is a
   DEAD ZERO:

     80172144  bnez  $v0, .Lb28_801721AC
     80172148   addu $s7, $zero, $zero      <- set, saved, restored, NEVER READ

   $s7 is written exactly once with 0, is saved at 0x44($sp) and restored, and
   nothing in the 160 words reads it. The mechanism is almost certainly
   reload_cse_regs (reload1.c; it IS present in this compiler): a long-lived
   local holding 0 whose every use reload_cse replaced with $zero, leaving the
   set dead with no dead-code pass after it to remove it.

   WHY IT MATTERS MORE THAN ONE WORD. With that ninth value present the target
   has no register left for `t' (= d - 0x20), so `t' goes to the caller-saved
   $t0 and is read back from the frame; `y2' takes $s6 instead of $s7; and
   reorg has one spare insn, so the `lbu $v0, 0x71($s1)' LOAD-DELAY slot at
   word 15 is filled by the `c' parameter copy instead of a `nop'. Those three
   consequences are 22 of the 24 rows. The body below gets the word count, the
   frame, the save list, the save ORDER, every save/restore offset, every
   relocation and every call exactly right; the residue is entirely the
   register map downstream of the absent pseudo.

   EIGHT SHAPES TRIED FOR THE ZERO, ALL 160 words / 24 rows -- i.e. the
   pseudo was folded away in every one, so none of these is the answer:
     `s32 r = 0;' with the five `return 0' written `return r'
     the same plus `r = 1' before the final `return r' (two definitions)
     `s32 z = 0;' used as func_80047784's third argument
     `s32 z = 0;' used at `p[0x9B] = z' and `*(s32 *)(p + 0x8C) = z'
     the same, plus the five returns and the func_80047784 argument
     `u8 *w = NULL;' carried to `p[0x9B] = (s32)w' with func_8009B4B0's
       result assigned into `w' first
     a THIRD copy of each c byte (`x3 = x2; y3 = y2') to raise pressure
     `k = 0' before the gates and reassigned after them
   WHAT I WOULD TRY NEXT: the pseudo has to survive cse's constant
   propagation, so it needs a definition cse cannot see through. The two
   candidates left are (a) a value cse proves zero only through
   `record_jump_equiv' -- some test earlier in the body whose failing arm
   makes a local provably 0 -- and (b) a permuter run, which is 24 rows on a
   body whose every other property already matches.

   THE FRAME NEEDED 0x28 BYTES BELOW THE SAVES AND THE SPLIT IS A GUESS. The
   target is `addiu $sp, $sp, -0x50' with ten saves at 0x28..0x4C, so
   args_size + var_size = 0x28. The body below reaches that as a FIVE-ARGUMENT
   func_800C7930 (0x18 of outgoing args) plus a 16-byte local (`zz'), and that
   combination reproduces the frame, every save offset AND the
   `sw $t0, 0x10($sp)' at exactly word 42. It is not proven:
     * func_800C7930's real arity is FOUR -- csrc/ovl_dungeon_800C2824.c:3015
       defines `s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 d)' -- so the fifth
       argument below is a device, declared through a unit-local `__asm__'
       alias so the landed definition is untouched.
     * Written with FOUR arguments the body is 160 - 5 = 155 words and 87
       rows: the five missing words are `sw $a0, 0x50($sp)',
       `sw $a1, 0x54($sp)', the two `x2'/`y2' copies and `sw $t0, 0x10($sp)'.
       Adding a 24-byte local instead gets the frame right but NOT the
       `sw $t0', because a stack local adds no register pressure and `t' then
       stays in $s6.
     * So the honest reading of 0x10($sp) is either the fifth outgoing
       argument (args 0x18, local 0x10) or `t's SPILL SLOT with a 4-argument
       call (args 0x10, locals 0x18) -- and the second needs `t' to spill,
       which needs the missing ninth pseudo. FINDING THE ZERO MAY DELETE THE
       FIVE-ARGUMENT DEVICE AND THE `zz' LOCAL TOGETHER. Do not inherit
       either of them as facts.

   WHAT IT IS. The actor's "can I step onto this tile" test, the relinked and
   extended cousin of the resident func_800CA788
   (csrc/ovl_dungeon_800C2824.c:4893, 109 words, 0 differing -- the pair the
   brief already records as func_b03_80171DA0 <- func_800CA788). It clears
   d[0x71]'s 0x80, refuses on D_80083462's 0x2000, on D_80083462's 8 unless
   d + 0x46 has 0x8000, and on func_800A2B5C either side of
   func_800C7930 -- all four with -1. Then it asks func_800A44E0 whether the
   cell at the facing index (d + 0x2A bits 9..11) is passable, func_8009B4B0
   whether anything occupies it, func_800A5690 whether the move is allowed at
   all, and func_800BCB04 for the floor height there, refusing with 0 unless
   the height is under 0x201 and within 0x3F of d + 0x88. On success it puts
   the actor into state 0x17 with a null +0x8C, points the slot's +0x2C at
   this bank's D_80174558 facing table, re-dresses through func_80047784,
   runs func_800A4ACC and func_800C77D0 and drops d[0x6D].

   MEASURED ON THIS BODY. Every figure is aligndiff on the body below unless a
   word count is given. NONE of these is a price from a clean body -- the
   24-row fault is live in all of them, so re-price anything you keep.

   | spelling                                              | words | rows |
   | as written below                                       |  160  |  24  |
   | `s16 *t1' DECLARED BEFORE `s16 *t2'                    |  160  |  29  |
   |   (t1/t2 then hold each other's $s0/$s2)               |       |      |
   | x and y declared before their copies, in the other     |  160  |  32  |
   |   order (y, x, y2, x2)                                 |       |      |
   | ONE local per c byte -- no `x2'/`y2' at all            |  160  |  68  |
   |   (with the 5-arg call and `zz' still in place)        |       |      |
   | `s32 x2'/`s32 y2' instead of `u8'                      |  155  |  87  |
   |   -- a WIDE copy is propagated away entirely, which    |       |      |
   |   is the whole reason the copies are `u8': cse's       |       |      |
   |   make_regs_eqv never equates a subreg set, so only    |       |      |
   |   a NARROWING copy survives to the allocator           |       |      |
   | `s16 x2'/`s16 y2'                                      |  155  |  87  |
   | `u16 x2'/`u16 y2'                                      |  159  |  93  |
   | `u8 x'/`u8 y' with `s32 x2'/`s32 y2' (copy the other   |  158  |  77  |
   |   way round -- the copy comes out `andi 0xFF', not     |       |      |
   |   the target's `addu')                                 |       |      |
   | `zz' as 24 bytes instead of 16                         |  160  |  60  |
   | `zz' as 8 bytes                                        |  160  |  64  |
   | `d[0x6D] -= 1' moved BEFORE func_800C77D0              |  161  |  26  |
   |   -- it costs a WORD, so the decrement really is after |       |      |
   |   the call and reorg lifts its `sb' into the delay slot|       |      |
   | NOT MEASURABLE: `t' dropped and `d - 0x20' written     |  160  |  24  |
   |   inline at both calls, or inline at the second only   |       |      |
   |   -- cse keeps one pseudo either way                   |       |      |
   | NOT MEASURABLE: func_8009B4B0's addends swapped to     |  160  |  24  |
   |   `*t1 + x' -- the emitted `addu' operand order does   |       |      |
   |   not follow the source here, and the target's         |       |      |
   |   `addu $a1, $s3, $a1' is still 2 of the 24 rows       |       |      |

   DECLARATIONS
   s32 func_800A44E0(u16, u16, s16, u32);
   u8 *func_8009B4B0(u8 *, u16, u16);
   s32 func_800A5690(void);
   s32 func_800C77D0(void *, u8 *, s16, s32);
   s32 func_800C7930_5(u8 *, u8 *, s16, s32, u8 *) __asm__("func_800C7930");
   END DECLARATIONS

   Everything else it names (func_800A2B5C, func_800BCB04, func_80047784,
   func_800A4ACC, D_80083462, D_80083228, D_80174558, D_8006CCD8, D_8006CCE8)
   is already declared in ovl_dng_b28.c or comes from common.h.
*/
s32 func_b28_801720EC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    s16 *t2;
    s16 *t1;
    s32 x;
    s32 y;
    u8 x2;
    u8 y2;
    s32 k;
    s32 h;
    s32 zz[4];

    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) != 0)
    {
        goto fail;
    }
    if ((*(u16 *)(d + 0x46) & 0x8000) == 0 && (D_80083462 & 8) != 0)
    {
        goto fail;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        goto fail;
    }
    t = d - 0x20;
    func_800C7930_5(t, (u8 *)b, 8, 0x300, t);
    if ((s16)func_800A2B5C(d) != 0)
    {
    fail:
        return -1;
    }
    x = c[0x24];
    y = c[0x25];
    x2 = x;
    y2 = y;
    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    if ((s16)func_800A44E0(x2 << 6, y2 << 6, *(s16 *)(d + 0x88), k << 9) != 0)
    {
        return 0;
    }
    t1 = &D_8006CCD8[k];
    t2 = &D_8006CCE8[k];
    if (func_8009B4B0(d, x + *t1, y + *t2) != NULL)
    {
        return 0;
    }
    if ((s16)func_800A5690() == 0)
    {
        return 0;
    }
    h = func_800BCB04((((x2 + *t1) << 6) + 0x20) & 0xFFE0, (((y2 + *t2) << 6) + 0x20) & 0xFFE0,
                      *(u16 *)(d + 0x88) - 0x20);
    if ((s16)h >= 0x201)
    {
        return 0;
    }
    if ((s16)(h - *(u16 *)(d + 0x88)) < -0x3F)
    {
        return 0;
    }
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    p[0x9A] = 0x17;
    *(u8 **)(c + 0x2C) = D_80174558;
    func_80047784(c, D_80174558[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
    func_800A4ACC(d);
    func_800C77D0(t, (u8 *)b, 8, 0x300);
    d[0x6D] -= 1;
    return 1;
}
