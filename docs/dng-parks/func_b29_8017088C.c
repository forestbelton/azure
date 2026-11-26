/* PARK: func_b29_8017088C  --  csrc/ovl_dng_b29.c  (DUNGEON.BIN carved bank 29)

   THIS FUNCTION CANNOT VERIFY AT 0 UNTIL THE UNIT'S FIRST .rodata BOUNDARY
   MOVES, AND THAT IS THE HEADLINE.  splat labels the word at 0x8017088C as
   this function's first instruction, which is why the listing opens with the
   phantom `sltiu $zero, $s0, 0x4981'.  That word is 0x2E004981 = the bytes
   81 49 00 2E, a Shift-JIS "!" followed by NUL, and it is already documented
   as rodata by this unit's own landed func_b29_80176330, which references it
   through the file-scope alias `D_8017088C_str'.  So:

       splat's target for func_b29_8017088C  151 words
       the real function                     150 words
       the body below                        150 words

   `ovlverify' therefore reports `target 151 words, candidate 150 words' for a
   byte-correct body and the landing gate can never be met.  When the
   subsegment boundary is corrected -- the word becomes a real rodata object
   and func_b29_80176330's alias goes -- the target becomes 150 words and the
   table below applies unchanged.

   RESIDUE, from aligndiff.py: built 150, target 151, 5 aligned differences,
   of which ONE is the phantom word itself.  The other FOUR are a single
   register choice:

       target   beqz $v0, .Lb29_80170A54 / sll $a2, $a1, 1   (delay slot)
                ...  addiu $a0, $sp, 0x10 ; addu $v0, $a2, $a1
       here     beqz $v0, ... / addiu $v1, $v1, 0xC
                ...  addiu $a0, $sp, 0x10 ; sll $v0, $a1, 1 ; addu $v0, $v0, $a1

   i.e. the `i * 2' subexpression of the final `i * 0xC' index gets $v0 here
   and $a2 in the target.  The two rows that follow are consequences, not
   causes: because $v0 is the `beqz's own test register, reorg's
   `fill_slots_from_thread' refuses to lift the `sll' into the exit branch's
   delay slot (an insn that SETS a branch's input is not a legal filler), so
   the `addiu $v1, $v1, 0xC' fills it instead and the `sll' stays in the
   post-loop block.  Word count is unaffected; it is four rows of position.

   WHY IT IS NOT A PRIORITY SWEEP.  mips.h defines no REG_ALLOC_ORDER, so both
   local-alloc and global_alloc walk the hard regs in ascending number and $v0
   (2) is always tried before $a2 (6).  For the target's assignment the `i * 2'
   quantity must CONFLICT with whatever holds $v0, and in a body where it is
   born and dies two insns apart it cannot.  The unmeasured hypothesis is
   therefore that in the original the `sll' is NOT block-local at all -- it
   spans the loop-exit edge, which makes it a global allocno conflicting with
   the loop's own $v0 test temp.  Nothing tried below moves it there, and the
   two ways it could be true are both UNMEASURED:
     - the loop test itself indexing `base[i * 0xC]' (so cse has the product
       inside the loop) WITH a real loop note, i.e. a `while' plus loop
       strength reduction.  The plain `while' is 153 words (see table), so this
       needs the `while' AND the indexed test together -- the batch-34
       "a fix can be worse alone and necessary in combination" shape.
     - a macro whose expansion references `i' once more across the exit edge.

   ONE THING THAT DID MOVE, AND IT IS THE REUSABLE FACT.  The body was 14
   aligned differences until the `r[0x24] = b' store was written FIRST of the
   four slot stores instead of last.  That is nine rows of a $s5/$s6 exchange
   between the `b' parameter and the `m = n->unk_08' local, and it is
   allocno_compare arithmetic read straight off `-da':

       .lreg  reg 82 (b)  2 refs / 27 insns ->  1 * 2 / 27 = 0.074
              reg 89 (m)  4 refs / 96 insns ->  2 * 4 / 96 = 0.083

   so `m' outranks `b' by one part in a hundred, is allocated first and takes
   $s5.  Moving b's only use up shortens its live range to 19 insns
   (2 / 19 = 0.105) and it wins.  A `do { r[0x24] = b; } while (0)' reference
   bump measures the SAME 5 differences -- it takes b to 3 refs, 1 * 3 / 27 =
   0.111 -- so the mechanism is confirmed twice and the legal spelling is kept.
   THE TWIN DOES NOT HAVE THIS: csrc/ovl_dng_b41.c's func_b41_8017088C stores
   0x25, 0x2C, then 0x24 and lands at 0 differing, because it has no loop and
   no RECT and so no competing local.  The store order is priced per body.

   DONOR: csrc/ovl_dng_b41.c's func_b41_8017088C (123 words, landed), found by
   dngcallee.py -- 8 of 12 symbols, 42 bodies, one clean family.  dngtwin's
   best row for this stub is func_b01_80170BB0 at 0.830 and is the same family
   seen less well.  b41's whole shape is reproduced: the `p = NULL' early
   return value, the `w = p' alias for the five tail stores, `k = a & 3' with
   the three-way flag arm, and the func_800A6D30 recolour nest.  Deltas:
   p[0x13] is 0x1D, the handler is func_b29_80170AE8, the tables are the
   bank-local D_80176460 / D_80176470, the installed step is
   func_b29_80171094, the +0x92 slide is -0x20, b41's `*(u32 *)(p + 0x1C) |=
   0x200' in the recolour arm is ABSENT here, and -- the 27 words b41 has not
   got -- this body then walks the slot's +0x8 record list for the first entry
   WITHOUT 0x20 in its first byte and uploads a 256x1 strip with LoadImage.

   dngfam.py: 1 copy.  No replication to lose.

   MEASURED AND REJECTED (all with the b-store-first fix in place unless the
   row says otherwise; `built/target/aligned' from aligndiff.py):

   | spelling                                              | words | aligned |
   | the body below                                        |  150  |    5    |
   | r[0x24] = b written LAST (b41's order)                 |  150  |   14    |
   | do { r[0x24] = b; } while (0), b41's order kept        |  150  |    5    |
   | k = a & 3 computed before the r[0x25] store            |  150  |   14    |
   | k inlined at both tests (free on b41, NOT here)        |  150  |   14    |
   | the record walk as a plain `while' loop                |  153  |   11    |
   | the record walk as a `for' loop                        |  153  |   11    |
   | `if (...== 0) goto done;' inverted goto loop           |  150  |    5    |
   | `i += 1' before `q += 0xC' in the loop body            |  150  |    5    |
   | do { q += 0xC; i += 1; } while (0) in the loop body    |  150  |    5    |
   | s16 i                                                  |  152  |   17    |
   | no `t' local, rect.y and `rect.y << 6' at both uses    |  150  |   14    |
   | s16 t                                                  |  150  |   14    |
   | u32 t                                                  |  150  |   14    |
   | t split into two sets (t = load; t = (t >> 6) - 1)     |  150  |    6    |
   | a u16 `v' temp for the load                            |  150  |    5    |
   | `off = i * 0xC' hoisted into a local                   |  150  |   16    |
   | `base = *(u8 **)(r + 0x8)' hoisted into a local        |  149  |   23    |
   | `s = base + i * 0xC' pointer local                     |  150  |    7    |
   | array-ref ((u16 *)(base + i * 0xC))[3]                 |  150  |   14    |
   | (i * 2 + i) * 4 written out                            |  150  |    5    |
   | (i + i + i) * 4 written out                            |  150  |    5    |
   | i * 2 * 6                                              |  150  |   14    |
   | *(s16 *)(w + 0xB0) written before rect.y               |  150  |   18    |
   | *(s16 *)(w + 0xB0) written after rect.w/x/h            |  150  |   30    |
   | rect.w/x/h stored before the index computation         |  150  |   16    |
   | rect.h/w/x stored before the index computation         |  150  |   16    |
   | rect.x, y, w, h in field order                         |  150  |   24    |
   | *(s16 *)(p + 0xB0) instead of w                        |  150  |    6    |
   | func_800AA36C(w, m, r, w) instead of (w, m, r, p)      |  150  |    6    |
   | rect.y = (s16)t                                        |  150  |    5    |
   | `i' declared before `q'                                |  150  |    5    |
   | do { } while (0) round the index statement             |  151  |   14    |

   NOTE THE FLAT BLOCK AT 5.  Eleven spellings measure 5 with a
   bit-identical residue, so they are ONE fact and not eleven, and the four
   rows are not reachable by changing the index expression, the loop body
   order or the local set.  Per the brief's sweep rule -- flat under both
   order and set -- what is left is a dependence or pass-boundary question,
   and the two unmeasured candidates are named above.

   UNMEASURED, in the order worth trying:
     1. a real `while' loop whose TEST indexes `base[i * 0xC]' (both changes
        together, since the `while' alone is 153 words).
     2. the same with `i' as the only cursor and no `q' at all.
     3. the permuter, which I am not allowed to run.

DECLARATIONS
UnkNode *func_8003FD64(s32, void *);
s32 func_800A9C18(UnkNode *, UnkVec *, u8 *, s16);
s32 func_800AA36C(u8 *, void *, u8 *, u8 *);
extern u8 D_80083498[];
extern u8 D_801764B8[];
extern u8 D_80045340[];
extern u8 D_80176460[];
extern u8 D_80176470[];
u16 func_800A6D30(void);
s16 func_800A48F0(s8 *p, s8 kind, s8 c);
void func_b29_80170AE8(u8 *p, UnkVec *v, u8 *c, u8 *t);
void func_b29_80171094();
END DECLARATIONS */
u8 *func_b29_8017088C(s16 a, u16 b, u16 c, s16 d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;
    u8 *w;
    u8 *q;
    s32 k;
    s32 i;
    s32 t;
    RECT rect;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_b29_80170AE8;
        p[0x13] = 0x1D;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (u8 *)n->unk_0C;
        r[0x24] = b;
        r[0x25] = c;
        *(u8 **)(r + 0x2C) = D_80176460;
        k = a & 3;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        else if ((a & ~3) == 0)
        {
            if ((*(u32 *)(p + 0x14) & 0x200) == 0)
            {
                if ((func_800A6D30() & 1) != 0)
                {
                    func_800A48F0((s8 *)p, 1, (func_800A6D30() & 0x3F) | 0x20);
                    *(u8 **)(r + 0x2C) = D_80176470;
                }
            }
        }
        func_800A9C18(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_b29_80171094;
        *(u32 *)(p + 0x1C) |= 0x40000;
        *(s16 *)(w + 0x92) = -0x20;
        i = 0;
        q = *(u8 **)(r + 0x8);
    loop:
        if ((*q & 0x20) != 0)
        {
            q += 0xC;
            i += 1;
            goto loop;
        }
        t = (*(u16 *)(*(u8 **)(r + 0x8) + i * 0xC + 0x6) >> 6) - 1;
        rect.y = t;
        *(s16 *)(w + 0xB0) = t << 6;
        rect.w = 0x100;
        rect.x = 0;
        rect.h = 1;
        LoadImage(&rect, (u_long *)D_801764B8);
        func_800AA36C(w, m, r, p);
    }
    return p;
}
