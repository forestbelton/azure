/* PARK: func_s72_800247C0, ovl_dng_s72, agent h178, batch 35.

   BEST: 202 words / 8 ALIGNED DIFFERENCES with the `do { } while (0)' below.
   Without it: 201 words (ONE SHORT) / 10 aligned differences.
   dngfam: 1 member, 202 words, so nothing replicates off it.

   WHAT IT DOES.  The 0x27 flash's own driver.  Poke D_8002992E, draw three
   `(rand() & 0x3F) - 0x20' offsets, copy the owning node's vector into the
   caller's, decay the owning slot's +0x1C and +0x1E scales by 0x50 with a
   0x800 floor, and -- while the payload's +0x28 fuse is 0xB or more -- spawn
   FOUR 0x212 sparks driven by func_s72_800246B0, each at the caller's point
   plus the offsets (0x10 lower in z), each with the offsets negated and
   scaled by 0x200 as its velocity, each with one of three colour bytes set to
   0x10 on a func_s72_8002458C(3) draw, 0x7DCF at +0x12 and 0x100 raised in
   +0x14, registered with func_8003DB94 against D_800DE870.  Then decrement
   the fuse and, at zero, restore it, publish the owning node in
   D_80083460 +0x10 if that slot is empty, hand the payload to
   func_800A48F0(..., 0x18, 0x14), clear its +0x28, write -1 at +0x64 and
   D_800E3D7C at +0x60, and retire the actor.

   THE RESIDUE IS ONE INSTRUCTION'S POSITION AND NOTHING ELSE.  Every other
   word in the body agrees.  The target emits

       87  lw    $s1, 0xC($s2)
       88  nop                          <- maspsx load-delay nop
       89  lhu   $v1, 0x14($s1)
       90  addiu $v0, $zero, 0x20
       ...
       96  lw    $a0, 0x8($s2)
       97  addu  $v0, $v0, $a3
       98  sh    $v0, 0x2($a0)
       99  addiu $a0, $zero, 0x3        <- the func_s72_8002458C argument
      ...
      122  jal   func_s72_8002458C
      123  sh    $v0, 0x1C($s1)          (delay slot)

   so the argument constant is hoisted to row 99 and no further -- exactly one
   slot after the FIRST vector store, whose base then gets $a0.  With no
   device the same constant hoists all the way to row 90, fills the `lw $s1'
   load-delay slot, the nop disappears (201 words), and the first vector
   store's temp takes $v1 instead of $a0: that is the whole 10-row residue,
   two register rows plus the displaced constant and its wake.  With the
   `do { } while (0)' the constant is pinned into the `jal's own delay slot
   instead, the nop comes back (202 words) and the residue is 8 rows -- the
   same two register rows plus the constant now LATE rather than early.  So
   the device overshoots by as much as no device undershoots, and the answer
   is whatever makes sched1 stop at 99.

   EVERY SPELLING MEASURED, all through aligndiff on this body (words / rows):

   | spelling                                                 | words | rows |
   | the body below, with the do-while                         |  202  |   8  |
   | the same with no device at all                             |  201  |  10  |
   | `if (1)' round the draw instead of the do-while            |  201  |  10  |
   | do-while round the draw + the three zero colour bytes      |  202  |   8  |
   | do-while round the whole colour pick (call, zeros, 3 ifs)  |  202  |  37  |
   | do-while round the three vector stores                     |  204  |  49  |
   | do-while round the FIRST vector store only                  |  203  |  22  |
   | if (1) round the three vector stores                        |  201  |  10  |
   | if (1) round vector stores 2 and 3                          |  201  |  10  |
   | if (1) round the second slot read + the two 0x1000 stores    |  201  |  10  |
   | do-while + if(1) round vector stores 2 and 3                 |  202  |   8  |
   | do-while + the +0x14 flag before the +0x10 store              |  202  |   8  |
   | the +0x14 flag before the +0x10 store, no device                |  201  |  10  |
   | `m = n->unk_08' hoisted into a local for the vector stores      |  201  |  21  |
   | the three velocity stores before the three vector stores         |  201  |  32  |
   | the slot dressing moved after the vector stores                   |  201  |  19  |
   | `*(s16 *)(q + 0x28) = 0x14' after the n->unk_10 store              |  201  |  23  |

   FOUR FACTS THAT DID REACH 0 ON THEIR OWN ROWS and are already in the body
   below -- they took it from 48 rows to 10, so do not undo them:

   (1) THE PROLOGUE NODE AND SLOT MUST BE THE SAME LOCALS THE LOOP USES.
   Writing `n' and `s' once and reassigning them inside the loop is what puts
   them in $s2 and $s1; with separate prologue locals they get $a1/$a2 and
   the body is 30 aligned differences instead of 12 (and the whole prologue
   rotates).  The pseudos cross a call in the loop, so global.c gives them
   callee-saved registers, and the prologue's dead-before-the-loop values
   ride along.

   (2) THE TAIL RE-READS `*(UnkNode **)(p + 0x20)' INTO THE SAME `n'.  Written
   inline at the func_800A48F0 argument it is 2 rows: the target has
   `lw $s2, 0x20($s5)' then `addiu $a0, $s2, 0x20', i.e. a callee-saved
   pseudo, not a $a0 add-in-place.

   (3) `*(s16 *)(g + 0x64) = -1' MUST BE AN ARRAY REF, `((s16 *)g)[0x32]'.
   The target hoists the `lw %lo(D_800E3D7C)' ABOVE that store and fills its
   load delay with the `sh'; a plain cast keeps true_dependence's dependence
   against the fixed-address scalar and costs 3 rows plus a nop.  This is the
   SAME MEM_IN_STRUCT_P hoist that landed func_s72_80024BA0, func_s72_800246B0
   and func_s72_80024398 in this unit.

   (4) D_80083460 NEEDS A POINTER LOCAL (`w').  The target has
   `lui'/`addiu'/`lw 0x10($v1)'; inline it folds to `lw %lo+0x10' and loses
   the `addiu'.

   AND THREE WIDTHS, all read off the prologue rather than guessed: dx, dy and
   dz are `u16' (dx is spilled at sp+0x18 and re-read with `lhu', and the two
   register copies are used raw in the `sh'-truncated position adds while the
   velocity multiply re-extends them with `sll 16'/`sra 7'); `i' and `k' are
   `s16'; the three parameters are the handler triple.

   WHAT I WOULD TRY NEXT.  The question is narrow enough to answer with `-da':
   dump `.sched' and read why `(set (reg $a0) (const_int 3))' stops at row 99
   in the target's input but at 90 in mine.  It is a `rank_for_schedule'
   question about ONE insn with no flow dependence, so the answer is either an
   anti-dependence I have not created (something that READS $a0 between the
   func_8004491C call and row 99 -- note the target's first vector-store temp
   IS $a0, which would be exactly that if the source made it a hard-register
   value) or an INSN_PRIORITY difference from a dependence chain I have
   mis-shaped around the second slot read.  A permuter run over the statement
   order inside the `if (n != NULL)' arm is the cheap mechanical answer.

DECLARATIONS
void func_s72_800247C0(u8 *p, UnkVec *v, u8 *c);
void func_s72_800246B0(u8 *p, UnkVec *b, u8 *r);
s32 func_s72_8002458C(s32 n);
UnkNode *func_8003FC64(s32);
extern u8 D_80045340[];
void func_8003DB94(void *, void *, s32);
extern u8 D_800DE870[];
extern s16 D_8002992E;
s16 func_800A48F0(s8 *, s8, s8);
extern u8 *D_800E3D7C;
END DECLARATIONS

   `func_8004491C', `D_80083460', `D_800814A0', `rand' and `UnkNode' all come
   from include/structs.h and common.h and need nothing. */
void func_s72_800247C0(u8 *p, UnkVec *v, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *g;
    u8 *s;
    u8 *q;
    u8 *w;
    u16 dx;
    u16 dy;
    u16 dz;
    s16 i;
    s16 k;

    D_8002992E = 1;
    dx = (rand() & 0x3F) - 0x20;
    dy = (rand() & 0x3F) - 0x20;
    dz = (rand() & 0x3F) - 0x20;
    n = *(UnkNode **)(p + 0x20);
    m = n->unk_08;
    s = (u8 *)n->unk_0C;
    g = (u8 *)n + 0x20;
    v->unk_00 = m->unk_00;
    v->unk_04 = m->unk_04;
    v->unk_08 = m->unk_08;
    if (*(u16 *)(s + 0x1C) > 0x800)
    {
        *(s16 *)(s + 0x1C) -= 0x50;
        if (*(u16 *)(s + 0x1C) < 0x800)
        {
            *(s16 *)(s + 0x1C) = 0x800;
        }
    }
    if (*(u16 *)(s + 0x1E) > 0x800)
    {
        *(s16 *)(s + 0x1E) -= 0x50;
        if (*(u16 *)(s + 0x1E) < 0x800)
        {
            *(s16 *)(s + 0x1E) = 0x800;
        }
    }
    if (*(s16 *)(p + 0x28) >= 0xB)
    {
        for (i = 0; i < 4; i++)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x28) = 0x14;
                n->unk_10 = (u32)func_s72_800246B0;
                func_8004491C(n, (u32)D_80045340);
                s = (u8 *)n->unk_0C;
                *(s16 *)(s + 0x10) = 0x20;
                *(u16 *)(s + 0x14) |= 0xC;
                *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)v + 0x2) + dx;
                *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)v + 0x6) + dy;
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)v + 0xA) + dz - 0x10;
                *(s32 *)(q + 0x88) = -((s16)dx * 0x200);
                *(s32 *)(q + 0x8C) = -((s16)dy * 0x200);
                *(s32 *)(q + 0x90) = -((s16)dz * 0x200);
                s = (u8 *)n->unk_0C;
                *(s16 *)(s + 0x1E) = 0x1000;
                *(s16 *)(s + 0x1C) = 0x1000;
                /* DEVICE, and it does not reach 0: see the header. */
                do
                {
                    k = func_s72_8002458C(3);
                } while (0);
                *(s8 *)(s + 0xE) = 0;
                *(s8 *)(s + 0xD) = 0;
                *(s8 *)(s + 0xC) = 0;
                if (k == 0)
                {
                    *(s8 *)(s + 0xC) = 0x10;
                }
                if (k == 1)
                {
                    *(s8 *)(s + 0xD) = 0x10;
                }
                if (k == 2)
                {
                    *(s8 *)(s + 0xE) = 0x10;
                }
                *(s16 *)(s + 0x12) = 0x7DCF;
                *(u16 *)(s + 0x14) |= 0x100;
                func_8003DB94(s, D_800DE870, 0);
            }
        }
    }
    *(s16 *)(p + 0x28) = *(u16 *)(p + 0x28) - 1;
    if (*(s16 *)(p + 0x28) <= 0)
    {
        *(s16 *)(p + 0x28) = *(u16 *)(p + 0x28) + 1;
        w = (u8 *)&D_80083460;
        if (*(u32 *)(w + 0x10) == 0)
        {
            *(u32 *)(w + 0x10) = *(u32 *)(p + 0x20);
        }
        n = *(UnkNode **)(p + 0x20);
        func_800A48F0((s8 *)((u8 *)n + 0x20), 0x18, 0x14);
        g[0x28] = 0;
        ((s16 *)g)[0x32] = -1;
        *(u32 *)(g + 0x60) = (u32)D_800E3D7C;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
