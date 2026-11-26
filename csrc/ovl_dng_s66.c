#include "common.h"

UnkNode *func_8003FD64(s32, void *);
extern u8 D_80083498[];
extern u8 D_80024A8C[];
extern u32 D_800814A8;
void func_s66_800242D4(u8 *p, UnkVec *b);
s32 func_8004491C(UnkNode *, u32);
s32 func_800CEEFC(u8 *, UnkVec *, UnkSlot *);
void func_s66_80024784(u8 *p, UnkVec *b, UnkSlot *s);
UnkNode *func_s66_80024924(u8 *tmpl, s32 x, s32 y, s32 z, u8 *m, u8 *c, s16 k, u8 *d, s32 e);
s32 rand(void);
extern s16 D_80024A70;
extern s8 D_80024AE0[];
extern u32 D_800814A0;
void func_s66_800246F4(u8 *p, u8 *b);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s66", func_s66_8002401C);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s66", func_s66_800242D4);

/* Spawn this bank's kind-0x10 node off the shared D_80083498 template and
   dress its payload: func_s66_800242D4 as the node's step, D_80024A8C (the
   seven-entry 0xC-byte cycle table func_s66_80024784 and func_s66_800242D4
   both walk) at the payload's +0x10, the calling actor's node at +0x0, the
   template at +0x4, the node itself at +0x8, the caller's view at +0xC, and
   the player's +0x88 halfword snapshotted at +0x38. Its one caller is
   func_s66_8002401C's state 0. 36 words, 0 differing.

   No twin: dngtwin finds no carved sibling at ratio >= 0.70, and dngcallee's
   3-of-4 tier is 13 functions wide (noise). The spawn skeleton is
   csrc/ovl_dng_s55.c's func_s55_80024578 idiom -- func_8003FD64, the step
   into n->unk_10, the payload reached as `(u8 *)n + 0x20'.

   TWO FACTS, AND THEY ARE A CONJUNCTION. Priced from this body at 0, in
   aligned rows unless a word count is given:

     plain casts `*(u8 **)(q + N)', +0x38 store last      38 words / 23 diff
     ARRAY REFS `q[N]', +0x38 store last                  37 words / 22 diff
     array refs, +0x38 store after q[1], no `r' local     36 /  15
     array refs, +0x38 store after q[0], no `r' local     37 /  16
     array refs, +0x38 store after q[3], no `r' local     36 /  16
     q[0] spelled `*(u8 **)((u8 *)n + 0x20) = p'          36 /  15
     `q = ...' moved above the n->unk_10 store            36 /  15
     `r' local placed BEFORE the q[4] store               36 /  12
     `r' local placed at the top of the if                36 /  11
     `r' local placed AFTER the q[4] store (kept)         36 /   0

   (1) The payload stores must be ARRAY REFS. As plain casts the body is two
   words LONG, because MEM_IN_STRUCT_P is false on them and
   `true_dependence's fixed-scalar-versus-varying-struct arm then refuses to
   let the fixed-address D_800814A8 load move above them -- the load lands
   next to its own use and costs two load-delay nops. As array refs the load
   hoists and both nops go.

   (2) D_800814A8 must be read into a POINTER LOCAL whose assignment sits
   between the q[4] and q[0] stores. The figures are measured; the mechanism
   below is INFERRED from the two listings' register use and was not
   confirmed from a `-da' dump. It appears to be what makes its value a
   quantity distinct from the symbol-address temps: in the target $v1 holds it across
   words 19..25 while $v0 is still live with D_80024A8C, and with the read
   inline in the +0x38 statement the two never overlap, share $v0, and push
   `q' and `n' down into $v1/$a0 where the target has $a0/$a1. The whole
   15-row residue of the no-`r' forms is that one rotation. Note the local
   alone is not the fact and neither is its position: at the top of the if it
   is 11 and one statement earlier it is 12.

   NOT MEASURABLE here: whether the first payload store is q[4] or q[0].
   They share a base with a provably disjoint offset, so sched1 may transpose
   them and the listing's order is not evidence either way. */
void func_s66_800246F4(u8 *p, u8 *b)
{
    UnkNode *n;
    u8 **q;
    u8 *t;
    u8 *r;

    t = D_80083498;
    n = func_8003FD64(0x10, t);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s66_800242D4;
        q = (u8 **)((u8 *)n + 0x20);
        q[4] = D_80024A8C;
        r = (u8 *)D_800814A8;
        q[0] = p;
        q[1] = t;
        ((s16 *)q)[0x1C] = *(u16 *)(r + 0x88);
        q[3] = b;
        q[2] = (u8 *)n;
    }
}

/* The step for the kind-0x212 nodes func_s66_80024924 spawns: bump this
   bank's D_80024A70 live counter, re-seat the mover on the payload's +0x14
   source if it still has one, jitter the three view components at +0x18 by
   rand() & 0xF minus 8 scaled by D_80024AE0's signed byte at the payload's
   +0x32, advance the slot's +0x8 cursor one 0xC-byte entry round the
   D_80024AE0-0x54 ring, ease its +0x1E/+0x1C scale down by a 24th, fade the
   payload's +0x3C brightness by 8 less half its +0x32, publish that byte into
   the slot's three colour bytes, and retire the node once the brightness
   reaches 0. 104 words, 0 differing.

   No twin at any dngtwin floor and dngcallee's best tier for this body is
   2 of 4 symbols across 73 functions (noise), so this is written from the
   listing. func_80069EF8 IS `rand' (splat/common_syms.txt), so the call reads
   as a call.

   D_80024AE0 is ONE symbol used two ways and that is load-bearing. It is the
   signed-byte scale table (the index comes from a `lh', so it can be
   negative) AND the END of the 0x54-byte / seven-entry ring the slot cursor
   walks. The ring base must be spelled `D_80024AE0 - 0x54', not as the
   separate symbol D_80024A8C that func_s66_800246F4 uses for the same
   address: the target computes it with `addiu $v0, $v1, -0x54' off the
   register already holding D_80024AE0, which cse reaches only from one
   symbol. Two distinct SYMBOL_REFs cannot be related by the compiler, which
   does not know either address.

   FIVE FACTS, every one of them measured, priced from this body at 0 in
   aligned rows (word counts given where they move):

     all five wrong, as first drafted                     111 words / 22
     (a) `for (i = 0; i < 3; i++)'                        +2 words / 2
     (b) p+0x14 read as the plain cast `*(s32 *)(p+0x14)' +0 / 3
     (c) `t[*(s16 *)(p + 0x32)]'                          +0 / 1
     (d) `*(u16 *)(p+0x3C) - 8 + <div>'                   +1 word / 2
         `(s32)(*(u16 *)(p+0x3C) - 8) + <div>'            +1 word / 2
         `(s16)(*(u16 *)(p+0x3C) - 8) + <div>'            +0 / 2  [see note]
     (e) three separate `*(u8 *)(p+0x3C)' colour stores   +4 words / 4
     (f) `t = D_80024AE0' as its own statement before
         the loop instead of in the `for' initialiser     +0 / 2

   (a) THE COUNTER MUST BE WRITTEN AS A COUNTDOWN. `for (i = 0; i < 3; i++)'
   is 2 words long with `addiu $s0, $s0, 1' / `slti' / `bnez' where the target
   has `addiu $s0, $s0, -0x1' / `bgtz'. check_dbra_loop does NOT reverse this
   loop -- the lever that says an emitted countdown means the source counted
   up does not apply when the body has two strength-reduced pointer givs of
   different stride (+2 and +4) and the counter has no other use.

   (b) THE p+0x14 READ MUST BE AN ARRAY REF, `((u8 **)p)[5]'. As a plain cast
   the load cannot move above the D_80024A70 store: D_80024A70 is a
   fixed-address scalar and `true_dependence's fixed-scalar-versus-varying-
   STRUCT arm needs MEM_IN_STRUCT_P on the other reference, which only the
   array ref sets. The target issues `lw $a0, 0x14($s3)' two words BEFORE the
   `sh' that stores the bumped counter.

   (c) THE SCALE INDEX MUST COME FIRST IN THE ADDRESS SUM.
   `t[*(s16 *)(p + 0x32)]' emits `addu $v1, $s5, $v1' -- `pointer_int_sum'
   always builds PLUS(ptr, intop) -- where the target has
   `addu $v1, $v1, $s5'. The integer-sum spelling
   `*(s8 *)(*(s16 *)(p + 0x32) + (s32)t)' puts the index first.

   (d) THE -8 MUST NOT SIT ON THE UNSIGNED LOAD. This is convert.c's
   narrowing distribution: the destination is `s16', the first operand is a
   `u16' load, so the subtraction is redone in unsigned short, -8 becomes
   0xFFF8, that is no `addiu' immediate, and you get
   `ori $v0, $zero, 0xFFF8' + `addu' where the target has
   `addiu $a0, $a0, -0x8'. An `(s32)' cast around the subtraction does NOT
   help -- still 0xFFF8. Moving the constant onto the other addend,
   `*(u16 *)(p+0x3C) + (*(s16 *)(p + 0x32) / 2 - 8)', is what lands.
   NOT CONSTRAINED, measured: `(s16)(*(u16 *)(p+0x3C) - 8) + <div>' is also
   104 words and reaches the same 2 remaining rows as the kept form did
   before fact (f) was found, so the explicit narrowing cast is an equally
   good cure and the choice between them is not measurable here.

   (e) THE THREE COLOUR BYTES MUST BE ONE CHAINED ASSIGNMENT,
   `s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = *(u8 *)(p + 0x3C)'. Written
   as three statements each reading p+0x3C the body is FOUR words long: the
   `sb' into the slot invalidates cse's record of the load every time, so
   each store reloads behind a nop. The chain evaluates the load once, and it
   also explains the target's DESCENDING store order +0xE, +0xD, +0xC for
   free, because C assigns the rightmost lvalue first. A `u8' local would do
   the same job; the chain is the spelling that also produces the order.

   (f) `t' MUST BE SET IN THE `for' INITIALISER, after `i'. As a separate
   statement before the loop, `addiu $s0, $zero, 0x3' lands two words later
   than the target has it, behind the D_80024AE0 lui/addiu pair. The comma
   initialiser `for (i = 3, t = D_80024AE0; i > 0; i--)' is the last 2 rows
   of this body and it is the same lever as the `for (i = 7, t = p + 7; ...)'
   case already in docs/matching-levers.md. */
void func_s66_80024784(u8 *p, UnkVec *b, UnkSlot *s)
{
    s32 i;
    u16 *src;
    u8 *w;
    s8 *t;

    D_80024A70 += 1;
    if (((u8 **)p)[5] != NULL)
    {
        *(s16 *)((u8 *)b + 0xE) = *(u16 *)(((u8 **)p)[5] + 0x2);
        *(s16 *)((u8 *)b + 0x12) = *(u16 *)(((u8 **)p)[5] + 0x6);
        *(s16 *)((u8 *)b + 0x16) = *(u16 *)(((u8 **)p)[5] + 0xA);
    }
    w = (u8 *)b;
    src = (u16 *)(p + 0x18);
    for (i = 3, t = D_80024AE0; i > 0; i--)
    {
        *(s16 *)(w + 0x2) =
            *src + ((rand() & 0xF) - 8) * *(s8 *)(*(s16 *)(p + 0x32) + (s32)t);
        src++;
        w += 4;
    }
    s->unk_08 += 0xC;
    if (s->unk_08 >= (u32)D_80024AE0)
    {
        s->unk_08 = (u32)(D_80024AE0 - 0x54);
    }
    s->unk_1E -= s->unk_1E / 24;
    s->unk_1C = s->unk_1E;
    *(s16 *)(p + 0x3C) = *(u16 *)(p + 0x3C) + (*(s16 *)(p + 0x32) / 2 - 8);
    s->unk_0C[0] = s->unk_0C[1] = s->unk_0C[2] = *(u8 *)(p + 0x3C);
    if (*(s16 *)(p + 0x3C) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn one kind-0x212 trail node off the previous one as its template and
   dress it: func_s66_80024784 as the step, func_800CEEFC installed through
   func_8004491C, the mover's three position halfwords copied into the node's
   view, the caller's x/y/z written into both the view (+0xE/+0x12/+0x16) and
   the payload (+0x18/+0x1A/+0x1C), the slot pointed at the caller's cycle
   entry with 0x1000 scales and a 0x808080 tint, an optional
   (k-1) << 5 rotation at +0x10 with 0xC raised at +0x14, and +0x2E/+0x3C/
   +0x14/+0x32 seeded from 8, 0x80 and the caller's last two arguments. The
   node is returned so func_s66_800242D4 can chain the next one off it.
   80 words, 0 differing. NINE arguments, five of them on the stack.

   The caller is func_s66_800242D4's state 2, which calls this in a loop.

   THE BIG FACT, AND IT COST ME THE WHOLE BODY: x, y and z must be `s32'
   PARAMETERS, not `s16'. As `s16' this body is 80 words / 16 ALIGNED ROWS and
   the residue is a clean whole-body rotation of the four callee-saved
   registers -- x/y/z/k sit in $s2/$s3/$s4/$s5 where the target has
   $s3/$s4/$s5/$s2 -- which reads exactly like an `allocno_compare' priority
   fault and is not one. gcc promotes a `short' parameter to SImode in the
   ABI (PROMOTE_FUNCTION_ARGS) and then re-extends it at each use, so the
   declared narrow type creates an extra conversion pseudo per parameter and
   reshuffles the whole ranking. I spent eleven compiles on reference-count
   devices against that residue and all of them were attacking the wrong
   diagnosis; the arithmetic never even applied. The tell I missed: the
   CALLER's residue, not this one's -- in func_s66_800242D4 the `s16'
   prototype forces `sll'/`sra' pairs on the three arguments at the call
   site, which is visible as X rows against the target's plain `addu'.
   Corollary, measured here: a narrow parameter is only free when the listing
   shows it copied once and used in a store that truncates anyway.

   THE REST, ALL RE-PRICED FROM THIS BODY AT 0 (the figures below replace an
   earlier table measured while the x/y/z width was still wrong, where the
   same two facts read 15 and 16 rows -- an improvement is not evidence):

     x, y, z declared `s16' instead of `s32'          80 words / 16 rows
     no `kk' local, `(k - 1) << 5' straight off k     79 words /  3 rows
     the three mover copies interleaved with the
       x/y/z stores instead of written as a group     80 words /  2 rows

   (1) `k' MUST BE READ INTO AN `s32' LOCAL BEFORE THE GATE. Written
   `if (k != 0) { ... (k - 1) << 5 ... }' the body is ONE WORD SHORT, missing
   the `sra $v0, $v0, 16' the target has between the `sll' and the `beqz':
   with `k' declared `s16', `(k - 1) << 5' stored through an `sh' lets
   convert.c narrow the whole expression and drop the sign extension, and
   `combine' then folds the remaining `sll' straight into the compare. The
   `s32 kk = k' assignment makes the extension a real SImode value with two
   uses, so combine cannot delete it. `(s16)k' on an `s32' parameter does the
   same thing and is also one word short -- it is the ASSIGNMENT TO A WIDER
   LOCAL that matters, not the cast.

   (2) THE THREE MOVER COPIES MUST BE WRITTEN AS A GROUP, +0x2, +0x6, +0xA,
   before `q' and the x/y/z pairs. Interleaved the way the listing reads them
   (+0x2, +0x6, q, +0xE, +0xA, +0x18, ...) it is 2 rows, with the +0xE store
   emitted three words early. The listing's order is NOT the source's here:
   sched1 hoists the +0xE store to cover the +0xA load's delay.

   NOT RE-PRICED, so treat it as weaker than the two above: `e' (argument 9)
   is `s32'. As `s16' it was 80 words / 17 rows when measured against the
   body with the x/y/z fault live, the extra row being a SEVENTH callee-saved
   register -- gcc builds a prologue conversion pseudo for a narrow parameter
   and it lives from entry to its use. The direction is almost certainly
   right for the same reason as the x/y/z fact, but the figure is not.

   The `*(u32 *)(s + 0xC) = 0x808080' store sits before the `if (k != 0)'
   gate because the target fills the `beqz's delay slot with it; that is
   sched, not a source fact, and it is not measurable the other way round. */
UnkNode *func_s66_80024924(u8 *tmpl, s32 x, s32 y, s32 z, u8 *m, u8 *c, s16 k, u8 *d, s32 e)
{
    UnkNode *n;
    UnkVec *v;
    u8 *s;
    u8 *q;
    s32 kk;

    n = func_8003FD64(0x212, tmpl);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s66_80024784;
        func_8004491C(n, (u32)func_800CEEFC);
        v = n->unk_08;
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0xA) = *(u16 *)(m + 0xA);
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)v + 0xE) = x;
        *(s16 *)(q + 0x18) = x;
        *(s16 *)((u8 *)v + 0x12) = y;
        *(s16 *)(q + 0x1A) = y;
        *(s16 *)((u8 *)v + 0x16) = z;
        *(s16 *)(q + 0x1C) = z;
        s = (u8 *)n->unk_0C;
        *(u8 **)(s + 0x8) = c;
        *(s16 *)(s + 0x1E) = 0x1000;
        *(s16 *)(s + 0x1C) = 0x1000;
        *(u32 *)(s + 0xC) = 0x808080;
        kk = k;
        if (kk != 0)
        {
            *(s16 *)(s + 0x10) = (kk - 1) << 5;
            *(u16 *)(s + 0x14) |= 0xC;
        }
        *(s16 *)(q + 0x2E) = 8;
        *(s16 *)(q + 0x3C) = 0x80;
        *(u8 **)(q + 0x14) = d;
        *(s16 *)(q + 0x32) = e;
    }
    return n;
}
