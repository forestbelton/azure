/* PARK -- func_s63_80024D58, WORD-EXACT: 339 words target, 339 words built,
   46 aligned differences, re-measured by the second agent in batch 34 -- see
   the SECOND AGENT section below for the four changes that took it there and
   for the one contradiction left.  THE ORIGINAL HEADER FOLLOWS UNCHANGED; its
   local map, phase breakdown and index arithmetic are all still correct and
   are what made the second run possible.

   PARK -- func_s63_80024D58, 339 words target, 333 words built, 109 aligned
   differences. Batch 33, agent h142. ovl_dng_s63; one-member family. It is
   the renderer of the 0x302 node that func_s63_8002401C (LANDED) spawns in
   its state 3, and func_s63_80024B20 (LANDED) is that nodes step, so the
   record this reads at p+0x10..p+0x3F is the EIGHT SIX-BYTE TRAIL ROWS that
   func_s63_80024B20 shifts and settles every frame. Read that function first;
   it fixes the record layout and the row stride.

   WHAT IT DOES, derived entirely from the listing. A tag-chain walk, same
   shape as the landed func_s63_8002441C: the body runs on p, then
   `n = *(UnkNode **)(p - 8)` and `p = (u8 *)n + 0x20` until the chain ends,
   returning 0. Each node is drawn as a 15x15 mesh of semi-transparent
   POLY_FT4 quads whose screen vertices are built in three phases.

   PHASE A seeds the input vector arrays y column, `a[i].vy = i * 0x60 -
   0x2D0` for sixteen i, and fills the four invariant fields of the
   func_800DBA90 descriptor.

   PHASE B is the eight-step ring. For i = 7 down to 0 it points the
   descriptor at `&a[i]` and `&b[i]`, sets its count field to `(8 - i) * 2`
   and its angle field to the trail rows `*(u16 *)(p + i * 6 + 0x12)`, fills
   `a[k].vx`/`a[k].vz` for the counts worth of entries, and then runs the
   descriptor through func_800DBA90 FOUR TIMES, adding 0x400 to the angle
   field after each, projecting the rotated `b[k]` into a different EDGE of
   the 16x16 screen-vertex grid each time:

       j == 0   c[i][k]
       j == 1   c[i * 2 + r - (k + 1)][i]
       j == 2   c[i + r - 1][i * 2 + r - (k + 1)]
       j == 3   c[k][i + r - 1]

   with `r` the descriptors count field read back as
   `(s16) *(u16 *)(rec + 0x18)`. The four arms are a COMPARE CHAIN over
   {0,1,2,3} -- this function owns no jump table and the listing has no
   .rodata block.

   PHASE C walks j and k from 0 to 14 and emits one POLY_FT4 per cell off
   D_80083160s +0x8D0 cursor, 0x28 bytes of cursor each, with the four
   corners `c[k][j]`, `c[k][j+1]`, `c[k+1][j]`, `c[k+1][j+1]` written as WORDS
   into +0x8/+0x10/+0x18/+0x20, a fixed 0xC0/0xDF u and 0/0x1F v box, and
   `addPrim((u32 *)(g->unk_00 + 0xB0), t)` -- OT row 0, no depth index.

   THE FRAME IS THE MOST USEFUL THING HERE. -0x570 with s0..s7, fp and ra,
   and gcc has run out of registers: four values live in SPILL SLOTS
   (sp+0x538 = p, sp+0x53C = the `a` base, sp+0x540 = `&c[i][0]`, sp+0x544 =
   the `i * 0x60 - 0x2D0` giv), and `p` itself is reloaded from its incoming
   home slot at sp+0x570 at the top of every walk iteration. The local layout
   that produces it, in declaration order:

       SVECTOR a[16];        sp+0x010 .. 0x08F
       SVECTOR b[16];        sp+0x090 .. 0x10F
       s32     c[16][16];    sp+0x110 .. 0x50F
       u8      rec[0x20];    sp+0x510 .. 0x52F
       s32     w0;           sp+0x530
       s32     w1;           sp+0x534

   THE DESCRIPTOR IS func_800DBA90s AND ITS LAYOUT IS ALREADY LANDED, which
   is the donor nothing in LEADS.txt, CALLEE.txt or NEAR.txt points at. Three
   landed bodies call func_800DBA90 -- csrc/ovl_dng_b04.c func_b04_801743EC,
   csrc/ovl_dng_b38.c and csrc/ovl_dng_b33.c -- and b04s comment states the
   type outright: `u8 rec[0x20]`, +0x0 the input vector pointer, +0x4 the
   output vector pointer, two eight-byte `memcpy` payloads, and two s16 tail
   fields. It also states WHY the type matters: a `u8` array is alignment 1,
   so an eight-byte `memcpy(rec + 0x10, p + 0x4, 8)` with both pointers `u8 *`
   expands to one unaligned load pair and one unaligned store pair per word,
   which is the `lwl`/`lwr` and `swl`/`swr` run at 0x80024DDC..0x80024DF8.
   No other spelling reaches it.

   MEASURED, the one fact that is priced:

     | fact                                                  | words | rows |
     | PHASE C REUSES PHASE Bs TWO COUNTERS -- its outer     |  333  |  109 |
     |   loop is the same local as the four-way switchs      |       |      |
     |   counter and its inner loop the same local as the    |       |      |
     |   `a[k]` fill loops (as written)                      |       |      |
     |   ... phase C given two counters of its own           |  334  |  194 |

   That is 85 aligned rows for two variable names and it is the brief s
   shared-register rule paying off: the target holds the outer counter in $s4
   and the inner in $s1 across BOTH phases, so they are two locals for the
   whole function, not four. Count the callee-saved saves against the number
   of long-lived values before writing the locals -- ten saves minus one
   parameter is nine, and four counters do not fit.

   THE THREE FAULTS I DID NOT CLOSE, in the order I would attack them.

   1. THE `rec + 0x18` COUNT FIELD IS RE-READ BY THE TARGET INSIDE THE `a[k]`
      FILL LOOP (`lh $v0, 0x528($sp)` at 0x80024E98, inside the loop) and is
      HOISTED in this body. As a plain cast off a `u8` array the read is a
      fixed-address scalar, the `a[k]` stores are varying struct references,
      and `fixed_scalar_and_varying_struct_p` excuses the pair, so nothing
      invalidates it. The untried fix is to make the read a COMPONENT_REF --
      give the descriptor a struct typedef whose +0x18 is an `s16` member and
      whose +0x10 is a `u8 [8]` member, so the memcpys pointer stays
      alignment 1 while the field read becomes in_struct and is killed by the
      stores. That is the MEM_IN_STRUCT_P lever in the brief, used for its
      DEPENDENCE half rather than its allocation half.

   2. THE -0x2D0 ADDITIVE TERM IS A HOISTED REGISTER IN THE TARGET AND A
      FOLDED CONSTANT HERE. The target materialises it with
      `addiu $t0, $zero, -0x2D0` and then derives BOTH giv initialisers from
      it (`addiu $v1, $t0, 0x5A0` for the phase A loop at i = 15, and
      `addiu $t0, $t0, 0x2A0` for the phase B giv at i = 7); this body folds
      each to a single `li`. `emit_iv_add_mult` only fails to fold when
      `add_val` reaches it as a REGISTER, so the constant is a movable that
      `scan_loop` hoisted out of the WALK loop -- which needs the two giv
      expressions to share it. Worth about four words of the six-word
      deficit. Try writing the term so the two sites are textually one value.

   3. EVERYTHING ELSE IS THE PHASE B HEAD`S STATEMENT ORDER. The target
      computes `&a[i]`, `&b[i]`, the count field, the angle field and
      `a[0].vx = (*(s16 *)(p + 0x4) + i) * 0x40` in one straight-line run
      ending with the `a[k]` loops entry test, with the `a[0].vx` store in
      the `beqz`s delay slot. This body emits the same insns in a different
      order and the register names rotate through the rest of the arm.
      Re-derive that order against 0x80024E20..0x80024E7C before touching
      anything else; the four switch arms and all of phase C are already
      register-rename-only against the target.

   NOTE ON `a[0].vx`. The store at 0x80024E78 is to a FIXED frame address,
   sp+0x10, which is `a[0]` -- not `a[i]` -- while the descriptor is handed
   `&a[i]`. I could find no reading that makes it `a[i]`, and the listing is
   the authority, so it is written as `a[0].vx` here. If the next agent finds
   a layout in which sp+0x10 is something other than `a[0]`, that changes the
   whole local map above and should be checked first.


   ================  SECOND AGENT, batch 34 (h150).  WORD-EXACT NOW.  ================

   NEW FIGURE: 339 built words against 339 target, 46 aligned differences
   (ovlverify: 77 differing masked).  The park arrived at 333 / 109.  The
   six-word deficit is CLOSED and 63 rows are gone.  The body below is the new
   one.  All three of the author's named faults were real and all three are
   priced below; two are fixed outright.  The local map, the phase breakdown,
   the four index-arithmetic arms and the a[0].vx reading were all correct and
   were not re-derived -- reading them instead of re-deriving them is what made
   this run possible.

   FAULT 1 IS FIXED AND THE AUTHOR'S UNTRIED FIX IS THE ONE THAT WORKED.
   Giving the descriptor a struct typedef -- UnkDngS63Rec below, +0x10 a
   u8 [8] member so memcpy keeps its alignment-1 source and still expands to
   lwl/lwr + swl/swr, +0x18 an s16 member so the count read becomes a
   COMPONENT_REF and in_struct -- is worth two words and sixteen rows on its
   own.  The memcpy is written memcpy(rec.unk_10, q + 0x4, 8) and the pair at
   0x80024DDC..0x80024DF8 is unchanged, so the park's warning about the u8
   array was about the SOURCE's alignment, which q + 0x4 still supplies.
           u8 rec[0x20], the park's spelling                   333 / 109
           UnkDngS63Rec, s16 unk_18, r via *(s16 *)&rec.unk_18 335 /  93
           UnkDngS63Rec, u16 unk_18, bound via (s16)rec.unk_18 330 / 118
       The u16 variant is the trap: the target reads rec+0x18 BOTH ways and the
       listing shows the pair, lh $v0, 0x528($sp) for the loop bound and
       lhu $a1, 0x528($sp) for r, at 0x80024F60/64, FB8/BC, FDC/E0, 5030/34,
       5050/54 and 50A4/A8.  Keep the s16 member for the bound and the
       *(s16 *)&rec.unk_18 cast for r; one member cannot serve both.

   FAULT 2 IS FIXED AND THE AUTHOR'S MECHANISM WAS EXACTLY RIGHT.  The -0x2D0
   term has to reach emit_iv_add_mult as a REGISTER, and the way to do that is
   a plain s32 local assigned ONCE BEFORE the walk loop:
       s32 d;  ...  d = -0x2D0;  while (1) { ... k * 0x60 + d ... i * 0x60 + d }
   d is set outside the loop so loop.c sees an invariant REG, the fold does not
   happen, and reload -- which never gives a pseudo with a constant REG_EQUIV a
   stack slot -- rematerialises it as addiu $t0, $zero, -0x2D0 immediately
   before each giv initialiser.  That is exactly the target's words 19/20
   (addiu $t0, $zero, -0x2D0 / addiu $v1, $t0, 0x5A0) and 47/48
   (addiu $t0, $zero, -0x2D0 / addiu $t0, $t0, 0x2A0), and $t0 being a reload
   register is the tell.  Worth TWO words, not the four the park estimated.
           d as a local, from the 335-word body                 337 /  84

   THE THIRD AND LARGEST FIX IS ONE THE PARK DID NOT NAME: p NEEDS A SECOND
   PSEUDO.  The target has FOUR spill slots and the built had THREE.  Target
   words 15..18 are sw $t0, 0x53C($sp) in the prologue and then, at the walk
   loop's own label .Ls63_80024D98, lw $t0, 0x570($sp) / nop /
   sw $t0, 0x538($sp) -- it copies p out of its incoming home slot into a
   SECOND spill slot at the top of every iteration, and the loop's bottom
   (0x8002524C..0x80025268) does the chain step off 0x570.  So the parameter is
   the WALK CURSOR and a separate local is what the BODY reads.  Written that
   way -- u8 *q; q = p; at the top of the while, every body reference through q,
   p left for n = *(UnkNode **)(p - 0x8) and p = (u8 *)n + 0x20 -- it is worth
   THREE words, which is the rest of the deficit, and six rows.
           q = p, a separate body cursor, from the 336-word body 339 /  61
   This is the brief's parameter-copy lever RUN BACKWARDS.  The lever says a
   parameter that dies at its copy gets folded and the cure is to reuse the
   parameter as the cursor; here the target wants the copy to EXIST, and
   reusing the parameter as the body's pointer is what hid three words.  Both
   directions are now measured on the same block.

   FAULT 3 IS A PURE SOURCE-ORDER FAULT IN THE PHASE B HEAD AND IT IS WORTH 15
   ROWS.  The author said to re-derive the order against
   0x80024E20..0x80024E7C and was right to, but the STORE ORDER IN THE LISTING
   IS NOT THE SOURCE ORDER: the listing stores rec+0x4 at word 58 and rec+0x0
   at word 64, and the source has rec+0x0 FIRST.  Twelve orders of the five
   statements measured, all word-exact at 339:
           unk_00, unk_04, unk_0C, unk_18, a[0].vx    339 / 46   <-- kept
           unk_00, unk_0C, unk_04, unk_18, a[0].vx    339 / 48
           unk_00, unk_04, unk_18, unk_0C, a[0].vx    339 / 49
           unk_04, unk_00, unk_0C, unk_18, a[0].vx    339 / 54
           unk_00, unk_18, unk_04, unk_0C, a[0].vx    339 / 56
           unk_18, unk_00, unk_04, unk_0C, a[0].vx    339 / 58
           unk_00, unk_04, unk_18, a[0].vx, unk_0C    339 / 52
           unk_18, unk_04, unk_00, unk_0C, a[0].vx    339 / 60
           unk_04, unk_00, unk_18, unk_0C, a[0].vx    339 / 61   (the park's)
           unk_18, unk_0C, unk_04, unk_00, a[0].vx    339 / 63
           unk_04, unk_00, unk_18, a[0].vx, unk_0C    339 / 64
           unk_00, unk_04, unk_0C, a[0].vx, unk_18    339 / 68
       Hoisting k = i out of the fill loop's for-init is MUCH worse and costs a
       word: 340 / 102 with it first, 340 / 100 with it after unk_18.  Do not
       try that again.

   MEASURED AND REJECTED -- do not re-derive these.
       phase A's loop counter on j instead of i                  335 /  93
       phase A's loop counter on k, sharing phase B's inner      335 /  90
   Phase A's counter IS phase B's inner counter: the target's phase A loop is
   addiu $s1, $zero, 0xF / addiu $s1, $s1, -0x1 / bgez $s1 in $s1, which is
   the k register, while $s3 holds i.  So the park's rule extends: phase A,
   phase B's fill and switch loops and phase C are THREE uses of two locals,
   not five.  Three rows, kept in the body below.

   THE CASE 2 CONTRADICTION, WHICH IS THE MOST USEFUL THING LEFT HERE.
   The target computes k + 1 ONCE into $s0 and uses it both for the index and
   as the loop increment, in BOTH case 1 and case 2:
       case 1   0x80024F8C  addiu $s0, $s1, 0x1 / 0x80024F90 subu $a1, $a1, $s0
                0x80024FB0  addu  $s1, $s0, $zero
       case 2   0x80025010  addiu $s0, $s1, 0x1 / 0x80025014 subu $a1, $a1, $s0
                0x8002502C  addu  $s1, $s0, $zero
   and in both arms $s0 is therefore NOT available as a strength-reduced
   address giv, which is why the target recomputes sll $a0, $s1, 3 in the
   loop-closing delay slot instead of carrying addiu $s0, $s0, 8.  Cases 0 and
   3 have no k + 1, keep the plain k++ and ARE reduced (0x80024F50 and
   0x800250B8).  The spelling that produces it is an explicit shared local:
       for (k = i; k < i + rec.unk_18; k = m) { r = ...; m = k + 1; ... - m ... }
   In case 1 that is worth six rows.  IN CASE 2 IT MEASURES WORSE AND COSTS A
   WORD, against a listing that plainly has it:
           m in case 1 only                                      339 /  46
           m in case 1 and case 2                                338 /  51
           m in case 2 only, before the other fixes              (not measured)
   So case 2 is one spelling away from correct in a direction the measurement
   disagrees with, and the word it loses is somewhere else in the function.
   That is the single most promising thread left: find the word that case 2's
   m form gives up and the eleven rows of case 2 come with it.

   WHAT THE 46 ROWS ARE NOW, priced from the 339-word body.

     * 11 rows, case 2: the m shape above, plus the strength reduction it
       suppresses.  Word-neutral only in the current spelling.
     * 8 rows, case 1: the target's sll $a0, $s1, 3 / addu $s2, $s5, $zero /
       addu $a0, $s6, $a0 run sits two words EARLIER than the built's, so the
       whole arm's sign-extend pair reads one register later.  $s2 is a copy of
       $s5 (i * 2) that case 2 does not make.  No word delta.
     * 7 rows, the phase C tail at 0x800251B8..0x800251F4: the target's
       lw $v0, 0x0($v1) / addiu $t0, $t0, %lo(D_80083160) / sw $v0, 0x18($s0) /
       lw $v1, 0x4($v1) run is three words earlier than the built's.  The
       %lo is a relocation the verifier masks, so this is position only.
     * 5 rows, the phase B head: addu $s1, $s3, $zero (k = i) one word later
       than the target's and sw $a1, 0x510($sp) three words earlier.  The
       twelve orders above are exhausted; this is what the best of them leaves.
     * 4 rows, case 3: addu $a0, $s0, $zero two words later than the target's.
     * 4 rows, the hoisted lhu $a1, 0x528($sp) in cases 1, 2 and 3 landing in
       $v1 where the target has $a1, and one beq's offset.
     * 7 rows across the three sign-extend pairs: the target reuses $a1 for
       sll/sra where the built goes through $v0.

   NEW NAMED NEXT STEP.  Word-exact, 46 rows, and every row is a POSITION or a
   REGISTER NAME -- there is no missing or extra instruction anywhere in the
   function.  Two things, in this order:
     1. The case 2 contradiction above.  Hand the permuter the case 2 arm
        alone, with the question "which spelling gives addiu $s0, $s1, 0x1
        without losing a word".  That is 11 of the 46.
     2. The three position groups -- case 1's two-word lead, case 3's two-word
        lag and phase C's three-word lead -- are all the same kind of fault and
        all in bodies whose insn sets already match exactly.  They are sched1
        decisions, so they are reachable from statement order inside the arms,
        which is the cheapest thing left to sweep and has not been touched:
        every order experiment so far was in the phase B HEAD, none inside an
        arm or inside phase C's cell body.

   DECLARATIONS
   typedef struct UnkDngS63Rec
   {
       u8 *unk_00;
       u8 *unk_04;
       s16 unk_08;
       s16 unk_0A;
       u16 unk_0C;
       u8 pad_0E[0x2];
       u8 unk_10[0x8];
       s16 unk_18;
       s16 unk_1A;
       u8 pad_1C[0x4];
   } UnkDngS63Rec;
   void func_800DBA90(u8 *);
   END DECLARATIONS

   D_80083160, Unk80083160, UnkNode and memcpy need no declaration; all four
   are reachable from common.h and include/structs.h. RotTransPers,
   SetPolyFT4, SetSemiTrans, GetTPage, GetClut and addPrim are the psyq
   names of the listings func_80065420, func_800666F4, func_80066640,
   func_80066460 and func_8006649C -- splat/common_syms.txt names all five
   and none needs declaring. */
s32 func_s63_80024D58(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    u8 *q;
    POLY_FT4 *t;
    s32 i;
    s32 j;
    s32 k;
    s32 r;
    s32 d;
    s32 m;
    SVECTOR a[16];
    SVECTOR b[16];
    s32 c[16][16];
    UnkDngS63Rec rec;
    s32 w0;
    s32 w1;

    d = -0x2D0;
    while (1)
    {
        q = p;
        for (k = 15; k >= 0; k--)
        {
            a[k].vy = k * 0x60 + d;
        }
        rec.unk_08 = rec.unk_0A = 0;
        memcpy(rec.unk_10, q + 0x4, 8);
        rec.unk_1A = 0;
        for (i = 7; i >= 0; i--)
        {
            rec.unk_00 = (u8 *)&a[i];
            rec.unk_04 = (u8 *)&b[i];
            rec.unk_0C = *(u16 *)(q + i * 6 + 0x12);
            rec.unk_18 = (8 - i) * 2;
            a[0].vx = (*(s16 *)(q + 0x4) + i) * 0x40;
            for (k = i; k < i + rec.unk_18; k++)
            {
                a[k].vx = i * 0x60 + d;
                a[k].vz = *(u16 *)(q + i * 6 + 0x10);
            }
            for (j = 0; j < 4; j++)
            {
                func_800DBA90((u8 *)&rec);
                switch (j)
                {
                case 0:
                    for (k = i; k < i + rec.unk_18; k++)
                    {
                        RotTransPers(&b[k], &c[i][k], &w0, &w1);
                    }
                    break;
                case 1:
                    for (k = i; k < i + rec.unk_18; k = m)
                    {
                        r = *(s16 *)&rec.unk_18;
                        m = k + 1;
                        RotTransPers(&b[k], &c[i * 2 + r - m][i], &w0, &w1);
                    }
                    break;
                case 2:
                    for (k = i; k < i + rec.unk_18; k++)
                    {
                        r = *(s16 *)&rec.unk_18;
                        RotTransPers(&b[k], &c[i + r - 1][i * 2 + r - (k + 1)], &w0, &w1);
                    }
                    break;
                case 3:
                    for (k = i; k < i + rec.unk_18; k++)
                    {
                        r = *(s16 *)&rec.unk_18;
                        RotTransPers(&b[k], &c[k][i + r - 1], &w0, &w1);
                    }
                    break;
                }
                rec.unk_0C = rec.unk_0C + 0x400;
            }
        }
        for (j = 0; j < 15; j++)
        {
            for (k = 0; k < 15; k++)
            {
                t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                *(u32 *)&t->r0 = *(s32 *)(q + 0xC);
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = GetTPage(0, 3, 0x300, 0x100);
                t->clut = GetClut(0x10, 0x1F8);
                *(s32 *)&t->x0 = c[k][j];
                *(s32 *)&t->x1 = c[k][j + 1];
                *(s32 *)&t->x2 = c[k + 1][j];
                t->u0 = t->u1 = 0xC0;
                t->u2 = t->u3 = 0xDF;
                t->v0 = t->v2 = 0;
                t->v1 = t->v3 = 0x1F;
                *(s32 *)&t->x3 = c[k + 1][j + 1];
                addPrim((u32 *)(g->unk_00 + 0xB0), t);
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}
