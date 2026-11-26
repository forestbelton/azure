/* PARK func_b10_80174F24 -- ovl_dng_b10, 167 words target.
   BEST: 168 words candidate, 3 ALIGNED DIFFERENCES, with one
   `do { } while (0)' reference bump marked DEVICE at its site below.
   Without that device the same body is 168 words / 10 aligned rows, and a
   one-statement reorder of the tail reaches 167 words -- WORD-EXACT -- at 20
   rows. ovlverify reads 57 differing on the 10-row body, which is the
   position-by-position inflation the extra word causes: price with
   aligndiff, not ovlverify. 1 member (dngfam.py), so no replication.
   THE OTHER FOUR FUNCTIONS OF THIS BATCH LANDED, INCLUDING ITS OWN STEP
   FUNCTION func_b10_80174C70, which is the best reference for this body --
   it carries the identical `UnkDngB09A28Tbl t' / `u8 *g' / `s16 e[2]' block
   and that block measures 0 there.

   WHAT IT IS: this bank's spawner for the 0x212 decoration node. Copy the
   bank's 0x20-byte facing table D_b10_80170874 into a local, allocate the
   node with func_8003FC64(0x212), zero the payload's +0x1A, scan the eight
   0x200 steps of a full turn for the one whose
   ((D_80083160.unk_C8 + i + 0x100) >> 9) & 7 lands on sector 2 and park that
   angle in +0x18, install func_b10_80174C70 as the node's +0x10 step,
   register it with func_8004491C(n, D_80045340), raise 0xC and set +0x10 to
   0x20 on the renderable, jitter the three mover channels off the caller's
   vector with rand() & 0x1F, seed the mover's +0xC/+0x10 velocity from the
   facing pair scaled << 19 and integrate it once, then publish 0x200 scale
   halves, a 0x20/0x20/0x20 colour, 0x7DCF at +0x12 and 0x100 in the flags,
   and copy the 12 bytes at D_80175A0C into the payload at +0x28 as the
   renderable's +0x8 vector.

   DONOR: func_s46_80024D34 (csrc/ovl_dng_s46.c, 74 words) is the same
   func_8003FC64(0x212) / +0x10 install / func_8004491C(n, D_80045340)
   skeleton; it was reached through func_s02_800240C0
   (csrc/ovl_dng_s02.c), which names it. dngtwin finds NO carved sibling at
   0.70 for this stub; dngcallee's 5-of-7 tier (func_s10_80024550,
   func_b20_80174D14, func_b14_801749A8) is the same family but all three
   are -0x28 frames against this body's -0x50.

   THE RESIDUE, STATED EXACTLY. It is ONE two-register exchange plus the one
   copy it forces. `g' (the `(u8 *)&t' pointer) comes out in $a0 where the
   target has $v1, and the e[0] lhu/sll chain in $v1 where the target has
   $a0; the extra word is then the `move a0,v1' that has to carry the
   shifted value out of $v1 before `lw $v1, 0x4($s0)' reuses it. Every
   opcode, immediate and displacement in those rows already matches -- they
   are all `R' rows -- and rows 120..130 are content-identical, shifted one
   slot by that copy.

   THE ARITHMETIC IS KNOWN AND SO IS THE REMEDY'S COST. local-alloc's
   QTY_CMP_PRI (local-alloc.c:1693) is
   floor_log2(n_refs) * n_refs * size / (death - birth). `-da' `.lreg' on
   this body reports `g' as `Register 88 used 3 times across 12 insns', i.e.
   1*3/12 = 0.25, while the e[0] chain is 6 references over about 19 insns,
   2*6/19 = 0.63, so the chain is allocated first and takes the lower
   register. `g' needs a FOURTH reference to win (2*4/12 = 0.67). A
   `do { } while (0)' round the SECOND table read supplies exactly that and
   takes the body to 3 rows -- the mechanism is confirmed -- but its
   NOTE_INSN_LOOP_END then stops sched moving `sll $a0, $a0, 19' into the
   following load-delay slot, so a `nop' appears where the target has the
   shift. WHAT IS MISSING IS A LEGAL FOURTH REFERENCE TO `g' WITH NO
   SCHEDULING BARRIER, or a reason the target's `g' wins at 3 references.

   FACTS MEASURED ON THIS BODY (aligned differences):

   | spelling                                                   | words | rows |
   |------------------------------------------------------------|-------|------|
   | the body below (device on the 2nd table read)               |  168  |   3  |
   | the same with NO device                                     |  168  |  10  |
   | ... plus the last two mover statements SWAPPED (0x4 then    |       |      |
   |     0x0) -- kills the extra copy, breaks the tail order     |  167  |  20  |
   | channels 1/2 as `load + ((rand() & 0x1F) - 0x10)'           |  168  |  10  |
   | channel 3's -0x70 through an `s32 k' LOCAL, `load + k'      |  168  |  10  |
   |   ... `load - 0x70 + (rand() & 0x1F)'                       |  169  |  13  |
   |   ... `(s32)(load - 0x70) + (rand() & 0x1F)'                |  169  |  13  |
   |   ... `((s32)load - 0x70) + (rand() & 0x1F)'                |  169  |  13  |
   |   ... `(rand() & 0x1F) + (load - 0x70)'                     |  169  |  14  |
   |   ... `-0x70 + load + (rand() & 0x1F)'                      |  169  |  13  |
   |   ... `load + ((rand() & 0x1F) - 0x70)'                     |  168  |  13  |
   |   ... `load + (rand() & 0x1F) - 0x70'                       |  168  |  13  |
   |   ... `(rand() & 0x1F) - 0x70 + load'                       |  168  |  15  |
   |   ... two statements, `u = load - 0x70' then `u + rand()'   |  168  |  55  |
   | the table base through a `u8 *g = (u8 *)&t' POINTER LOCAL    |  168  |  10  |
   |   ... `(u8 *)&t' written out at both reads                   |  169  |  13  |
   | device on the SECOND read only                               |  168  |   3  |
   |   ... on the FIRST read only                                 |  169  |  13  |
   |   ... on BOTH reads                                          |  169  |  23  |
   |   ... on `m->unk_0C = e[0] << 19'                            |  170  |  13  |
   |   ... on the second read + that shift together               |  169  |  12  |
   |   ... on `g = (u8 *)&t' alone                                |  169  |  13  |
   |   ... on `g = (u8 *)&t' and the first read together          |  169  |  13  |
   |   ... on the second read, with the two shifts swapped        |  168  |  15  |
   |   ... on the second read, with the 0xC shift moved up        |  168  |  11  |

   CHANNEL 3's JITTER IS THE ONE FACT THAT IS FULLY SETTLED, and it is the
   reason the local is there. func_s46_80024D34 records that
   `load + ((rand() & 0x1F) - K)' puts the `addiu K' on the LOADED halfword,
   because fold-const.c's `associate:' arm moves a split-out constant to the
   OTHER addend; the target's channel 3 has the `addiu -0x70' on the RAND
   value instead, so the source grouping must be `(load - K) + rand'. Every
   spelling of that grouping is narrowed to UNSIGNED SHORT by convert.c's
   PLUS case (0x70 becomes 0xFF90 and cc1psx emits `ori $a0, $zero, 0xff90'
   plus a second `addu'), and NO cast prevents it -- `(s32)' round the load,
   round the subtraction, or round both all measure identically. An `s32'
   local holding `(rand() & 0x1F) - 0x70' is the only form found that gets
   the grouping AND the mode: the tree is then `PLUS(load, k_var)' with no
   constant for `associate:' to move and nothing for convert.c to narrow.

   MEASURED AND FLAT at 10 rows (so the exchange is not reachable from
   these): every permutation of the declaration order (`g' first, `g' last,
   `g' between); `e' as a `UnkDngB09A28' struct instead of `s16 e[2]';
   `u16 e[2]' with `(s16)' casts at the shifts; the four mover words as
   plain `*(s32 *)((u8 *)m + N)' casts instead of UnkVec fields;
   `g + 0x2 + idx' and `g + (idx + 0x2)' for the second read;
   `(UnkDngB09A28 *)(g + idx)' field refs at both reads; `(s32)e[0] << 19';
   a second pointer local `g2' for the second read. Per the brief's rule, a
   flat declaration-order sweep means allocation PRIORITY is not reachable
   from the declaration side. WORSE: `g' assigned at the top of the function
   (171/17); the two reads interleaved with their shifts (168/11);
   `m->unk_0C + m->unk_00' operand order (168/15).

   NOT A FAULT, measured: the mover's +0xC load is cse-FORWARDED and its
   +0x10 load is RELOADED in both target and candidate, so the
   MEM_IN_STRUCT_P asymmetry there is already right. The 0x20-byte table
   copy is the in-unit func_b10_80174898 idiom (a `UnkDngB09A28Tbl' struct
   assignment, alignment 2, which is what gives the `lwl'/`lwr' + `swl'/`swr'
   group); the 12-byte copy to the payload's +0x28 is `memcpy(..., 0xC)',
   which at 12 bytes and alignment 1 takes expr.c's movstrsi path and gives
   the same unaligned group.

DECLARATIONS
extern u8 D_801759C0[];
extern u8 D_80175A0C[];
UnkNode *func_8003FC64(s32);
s32 rand(void);
void func_b10_80174C70(u8 *, UnkVec *, u8 *, u8 *);
extern const u8 D_b10_80170874[];
extern u8 D_80045340[];
s32 func_8004491C(UnkNode *, u32);
void *memcpy(void *, const void *, u32);
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
void func_b10_80174F24(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 e[2];
    UnkDngB09A28Tbl t;
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    u8 *g;
    s32 k;
    s16 i;

    t = *(UnkDngB09A28Tbl *)D_b10_80170874;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x1A) = 0;
        for (i = 0; i < 0x1000; i += 0x200)
        {
            if ((((D_80083160.unk_C8 + i + 0x100) >> 9) & 7) == 2)
            {
                *(s16 *)(q + 0x18) = i;
            }
        }
        n->unk_10 = (u32)func_b10_80174C70;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_14 |= 0xC;
        r->unk_10 = 0x20;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)b + 0x2) + ((rand() & 0x1F) - 0x10);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)b + 0x6) + ((rand() & 0x1F) - 0x10);
        k = (rand() & 0x1F) - 0x70;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)b + 0xA) + k;
        g = (u8 *)&t;
        e[0] = *(u16 *)(g + (((*(s16 *)(q + 0x18) - 0x400) >> 7) & 0x1C));
        /* DEVICE: the `do { } while (0)' is a REFERENCE BUMP on `g', not a
           macro body -- I cannot see a macro boundary round one half of a
           two-component read. flow.c:2122 counts a reference once per
           ENCLOSING LOOP, so this takes `g' from 3 references to 4 and
           local-alloc's QTY_CMP_PRI (local-alloc.c:1693,
           floor_log2(n_refs) * n_refs * size / (death - birth)) from
           1*3/12 = 0.25 to 2*4/12 = 0.67, past the e[0] lhu/sll chain's
           2*6/19 = 0.63, so `g' is allocated first and takes $v1 as the
           target does. Worth 7 of the 10 aligned rows: without it the body
           is 168 words / 10 rows, with it 168 / 3. It is NOT a landing --
           its NOTE_INSN_LOOP_END then stops sched moving
           `sll $a0, $a0, 19' into the next load-delay slot, so a `nop'
           appears where the target has the shift. Warns under -Wall? No.
           Rejected plain spellings are priced in the header. */
        do
        {
            e[1] = *(u16 *)(g + (((*(s16 *)(q + 0x18) - 0x400) >> 7) & 0x1C) + 0x2);
        } while (0);
        m->unk_0C = e[0] << 19;
        m->unk_10 = e[1] << 19;
        m->unk_00 = m->unk_00 + m->unk_0C;
        m->unk_04 = m->unk_04 + m->unk_10;
        r = (UnkSlot *)n->unk_0C;
        r->unk_1E = 0x200;
        r->unk_1C = 0x200;
        *((u8 *)r + 0xE) = 0x20;
        *((u8 *)r + 0xD) = 0x20;
        *((u8 *)r + 0xC) = 0x20;
        r->unk_12 = 0x7DCF;
        r->unk_14 |= 0x100;
        memcpy(q + 0x28, D_80175A0C, 0xC);
        r->unk_08 = (u32)(q + 0x28);
    }
}
