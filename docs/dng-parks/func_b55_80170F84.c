/* PARKED: func_b55_80170F84 -- ovl_dng_b55, 106 target words, 105 candidate
   words, 8 ALIGNED DIFFERENCES. 1 copy (dngfam.py: one-member family).
   No jump table and no rodata of its own -- the listing opens at `glabel'.

   WHAT IT IS. The banked room script's SPAWNER for this bank's 0x112 actor:
   allocate the node off D_80083498, publish it in D_80174710, install
   func_b55_8017112C as the node's +0x10 driver, stamp the kind byte 0x37,
   hand it to func_8004491C/D_80045340, seed the mover's +0xA heading from
   the fourth parameter and the slot's +0x24/+0x25 cell from the second and
   third, point the slot's +0x2C dress at the bank-local D_b55_80173E8C,
   raise the actor through func_800A9C18, arm +0x8C with
   func_b55_80171514, add 0x40000 to +0x1C, publish through func_800AA36C,
   then reseat the mover from D_8008378A, stash and zero the slot's
   +0x1C/+0x1E scale pair across two func_b55_80170B50 calls (1 then 2),
   restore the pair and reset the engine through func_b55_80173C30.
   Returns the payload pointer, NULL when the allocation fails.

   THE DONOR is the landed func_b57_801708AC (csrc/ovl_dng_b57.c, 104 words),
   found by the CALLEE-SET grep on func_8003FD64 / func_8004491C /
   func_800A9C18 / func_800AA36C / D_80083498 -- dngtwin finds NOTHING for
   this body at ratio 0.70. b57's own donor chain reaches func_b09_80171158.
   Three of b57's four stated facts were taken over unmeasured and the body
   is at 0 everywhere except the D_8008378A block, so they are not in doubt:
   `s16 a' as the first parameter, `u16 b, u16 c', and
   `*(s8 *)(p + 0x9C) = -1' rather than `p[0x9C] = 0xFF'.

   THE WHOLE RESIDUE IS ONE MISSING WORD IN THE D_8008378A BLOCK, and it is
   an absent register-to-register COPY, not an absent statement. Target:

       801710A4  sh    $v0, 0xA($s1)
       801710A8  addu  $v1, $v0, $zero
       801710AC  addiu $v0, $v0, -0x20
       801710B0  addiu $v1, $v1, -0x190
       801710B4  sh    $v0, 0x16($s1)
       801710B8  sh    $v1, 0xA($s1)

   Candidate:

       addiu $v1, $v0, -0x20
       sh    $v0, 0xA($s1)
       addiu $v0, $v0, -0x190
       sh    $v1, 0x16($s1)
       sh    $v0, 0xA($s1)

   MECHANISM, read off `tools/cdkgcc -da'. The three statements give RTL
   pseudo 94 (reg/v:HI, the `lhu') with three uses: the unpromoted store to
   m+0xA, and `plus (subreg:SI 94) -32' and `plus (subreg:SI 94) -400'.
   local-alloc.c's `combine_regs' ties an output to a DYING input, and 94
   dies at the -400 insn (it is the last use), so the -400 result is tied in
   place in 94's register and the -32 result takes a fresh one. The target
   has the OPPOSITE tie -- -0x20 in place on 94's register -- which is only
   reachable if 94 DIES AT THE -0x20 INSN, i.e. the -0x190 operand is a
   THIRD pseudo copied from 94 earlier. That copy is the missing word.

   EVERY ATTEMPT TO MAKE THAT COPY SURVIVE, with the pass that removes it:

   | spelling                                                 | words | rows |
   | the three statements as written below                     |  105  |  8   |
   | `s16 h' instead of `u16 h'                                |  105  |  8   |
   | `u16 g = h;' before the stores, `g - 0x190' last          |  105  |  8   |
   | the same, with `g = h;' after the m+0xA store             |  105  |  8   |
   | `u16 g = h;' plus `h -= 0x20;' and `g -= 0x190;' in place |  105  |  8   |
   | `g = h - 0x190;' precomputed before the stores            |  105  |  8   |
   | `s32 g = h;' before the stores, `g - 0x190' last          |  105  |  8   |
   | the same, with `g = h;' after the m+0xA store             |  105  |  8   |
   | `s32 g = h;' plus `g -= 0x190;' -- two SETS on g          |  105  |  6   |
   | `s32 g = h;' with BOTH subtractions off g (two USES)      |  106  |  8   |
   | `g = h - 0x20;' then `g = h - 0x190;' (one local, 2 sets) |  106  |  9   |
   | the two subtraction statements swapped                     |  104  |  7   |

   READ THE LAST THREE ROWS TOGETHER -- THEY BRACKET THE ANSWER. Two USES of
   an `s32 g' do keep the promotion insn alive (106 words, the right count),
   but it comes out `andi $v0, $a3, 0xffff' -- the zero_extend pattern -- not
   `addu'. One use lets combine substitute the promotion into its single
   consumer and delete it (confirmed in the dumps: insn 158 is present in
   `.cse' and NOTE_INSN_DELETED in `.combine'), which is the 105-word column.
   Same mode on both locals (`u16 g') is folded earlier still, by cse's
   `canon_reg', because the two pseudos land in one quantity.
   So the target's word is a promotion that combine kept AND simplified to a
   plain move. Neither two-sets nor two-uses reaches that, and the swapped
   order loses a store to flow instead.

   WHAT I WOULD TRY NEXT. The surviving-copy class is real -- the same
   mechanism landed the `addu $a0, $v0, $zero' in func_b55_8017112C in this
   unit, where a NARROW declaration on a CALL RESULT (`s16 r' rather than
   `s32 r' with an `(s16)' cast) keeps the raw result beside the
   sign-extended one. The difference here is that the value comes from an
   `lhu' of a symbol rather than from `$v0' at a call, so there is no
   hard-register copy to anchor the second pseudo. The two unexplored
   directions are: a spelling that makes `nonzero_bits' of the loaded value
   NOT provably 16-bit, so that combine cannot rewrite
   `plus (zero_extend ...)' into `plus (subreg ...)' and must keep the
   promotion as a move; and a cse basic-block break between the copy and its
   use, which needs a CODE_LABEL this straight-line block does not have.

   THREE MORE SPELLINGS MEASURED, and READ THEM WITH THE CAVEAT that this
   body never reaches 0, so each figure prices the spelling AND the live
   copy fault together and must be re-priced from a clean body:

   | the first parameter as `s32 a' with `(s16)a' at func_800A9C18 |  8  |
   | `u' and `v' as `s16' rather than `u16'                        |  8  |
   | `*(s16 *)(p + 0x9E) = 0' moved AFTER the func_800A9C18 call   | 11  |

   The first two do not move the row count and the residue is the same eight
   rows, so they look free; the +0x9E store's POSITION is load-bearing at 3
   rows and belongs before the call, which is where the `sh' reaches that
   call's delay slot.

   STUB STATE: reverted to INCLUDE_ASM in the working copy, and the other
   twelve functions of this batch's b55 list all verify at 0 differing with
   it stubbed.

DECLARATIONS
extern u8 D_80083498[];
extern u8 D_80045340[];
extern u8 D_b55_80173E8C[];
extern u8 *D_80174710;
extern u16 D_8008378A;
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800A9C18(UnkNode *, UnkVec *, UnkSlot *, s16);
s32 func_800AA36C(u8 *, void *, void *, u8 *);
void func_b55_80171514(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_b55_8017112C(u8 *p, UnkVec *b, u8 *c);
void func_b55_80170B50(u8 *p, u8 *b, u8 *c, s16 d);
void func_b55_80173C30(void);
END DECLARATIONS
*/
u8 *func_b55_80170F84(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u16 u;
    u16 v;
    u16 h;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        D_80174710 = (u8 *)n;
        n->unk_10 = (u32)func_b55_8017112C;
        p[0x13] = 0x37;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = b;
        *((u8 *)r + 0x25) = c;
        *(u8 **)((u8 *)r + 0x2C) = D_b55_80173E8C;
        *(s16 *)(p + 0x9E) = 0;
        func_800A9C18(n, m, r, a);
        p[0x9A] = 0xFF;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x8C) = (u32)func_b55_80171514;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)((u8 *)r + 0x10) = 0x20;
        *(u16 *)((u8 *)r + 0x14) |= 0xC;
        func_800AA36C(p, m, r, p);
        h = D_8008378A;
        *(s16 *)((u8 *)m + 0xA) = h;
        *(s16 *)((u8 *)m + 0x16) = h - 0x20;
        *(s16 *)((u8 *)m + 0xA) = h - 0x190;
        u = *(u16 *)((u8 *)r + 0x1C);
        v = *(u16 *)((u8 *)r + 0x1E);
        *(s16 *)((u8 *)r + 0x1C) = 0;
        *(s16 *)((u8 *)r + 0x1E) = 0;
        *(u16 *)((u8 *)r + 0x14) |= 0x80;
        func_b55_80170B50(p, (u8 *)m, (u8 *)r, 1);
        func_b55_80170B50(p, (u8 *)m, (u8 *)r, 2);
        *(s16 *)((u8 *)r + 0x1C) = u;
        *(s16 *)((u8 *)r + 0x1E) = v;
        func_b55_80173C30();
    }
    return p;
}
