/* PARK: func_s27_80024E64, csrc/ovl_dng_s27.c, agent-h40, 2026-11-23.

   TARGET 650 words. BEST BUILT 644 words, 152 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify reports 436 unaligned on the same
   body, which is the same body with the stream shifted). 1 member, 650 words
   (tools/decomp/dngfam.py), so no replication value -- it is worth its own
   650 words.

   THE FUNCTION OWNS jtbl_s27_80024028, TEN ENTRIES, one per case 0..9, and
   case 0 FALLS THROUGH into case 1. Entries, in order:
   .Ls27_80024F34 .Ls27_80024FA4 .Ls27_8002551C .Ls27_80025580 .Ls27_8002566C
   .Ls27_800256C0 .Ls27_800256E0 .Ls27_80025790 .Ls27_800257C8 .Ls27_80025834

   WHAT IT DOES. The +0xA step of the scene's 10-state ten-node effect. Every
   frame it first walks the eight halfwords at p+0x1A..p+0x28 adding the step
   at p+0x18 and folding each modulo 0x80, then bumps the two counters at
   p+0x10 and p+0x16, then dispatches on p+0xA, and every path ends in the
   shared `*(s16 *)(p + 0x14) = 0;'.
   State 0 seeds: zero both counters, set the step to 4, bump the state, clear
   the eight phase halfwords and the ten pointers at p+0x2C..p+0x50, call
   func_s27_800240EC, take the caster's facing octant out of its +0x2A angle
   and set the renderable white; falls into state 1.
   State 1 re-reads the caster's node, asks func_8003DE58 for the muzzle
   offset, writes the mover to the muzzle (dropping 0x40 instead when the slot
   has 0x8000), registers func_s27_80024DF4 with func_8004491C once (guarded
   by bit 0 of p+0xC), and -- only when the caster's +0x4 record has bit 0x80
   -- picks a target. With a locked-on actor at the caster's +0x60 it takes
   the three absolute axis deltas (the Z one offset by 0x10 when the actor's
   +0x1C has 0x40000), divides the largest by 32 to get a frame count in
   p+0x12, writes the three HALFWORD velocities at b+0xE/0x12/0x16 and bumps
   the state. Otherwise it WALKS UP TO EIGHT TILES along the octant with
   func_800A44E0 and func_800BCB04 and takes the last tile that was not
   blocked, is below 0x201 and is within 0x3F of the caster, builds a target
   vector on the stack, takes the same three deltas and frame count, writes
   the three WORD velocities at b+0xC/0x10/0x14 and sets the state to 8.
   States 2 and 8 integrate the halfword velocity and call
   func_s27_800241F8; 2 also plays func_800A56E0(0x300).
   State 3 re-seeds from the actor, writes the 0x10..0x70 phase ramp and
   builds the ten nodes with func_s27_8002443C.
   States 4 and 6 add 0x40404 / 0xFFFBFBFC to each node's +0xC colour; 6 also
   fires func_s27_80024050 and then releases all ten nodes.
   State 5 waits. State 7 is the kill: clear D_8008346C, set 0x8000 on the
   node and on D_800814A0. State 9 re-arms back to state 7.

   NO TWIN ANYWHERE. xsibs.py over ovl_dng_s27 (--min-words 18 --ratio 0.70,
   3366 landed functions) finds nothing for it, and at --min-words 500
   --ratio 0.30 it still finds nothing. dngtwin.py --unit ovl_dng_s27 (2035
   carved listings, 830 landed) reports nothing above 0.21 anywhere. The
   callee-set recipe does not reach it either: func_800A44E0 is the only rare
   symbol and its two landed callers (func_800A05A4, func_800A36B4 in
   csrc/ovl_dungeon_8009F644.c) are 139- and 91-word plain line walks.
   THE BODY ABOVE WAS DERIVED FROM docs/dng-parks/func_s14_80024AE4.c, the
   438-word six-state sibling in ovl_dng_s14 -- also parked, also on this
   residue. That park's measured facts were re-measured here and three of them
   hold on this body (see below); the rest of the shape is new work.

   FIVE MEASURED FACTS THAT TOOK THIS BODY FROM 482 TO 152. Each was
   re-measured on this body, not inherited.

     EVERY LOOP IS WRITTEN `base + i * stride', NOT AS A WALKING POINTER.
     The six counted loops all index off p: `*(s16 *)(p + 0x1A + i * 2)' and
     `*(s32 *)(p + 0x2C + i * 4)'. gcc then makes the giv `p + 2*i' (or
     `p + 4*i') and leaves 0x1A / 0x2C in the load's displacement, which is
     exactly what the target does -- `addiu $a1, $s3, 0xE' then
     `lhu $v0, 0x1A($a1)'. Written as the walking pointers the s14 park uses
     (`u = (s16 *)(p + 0xE); for (...; u--) u[0xD] = ...') gcc folds the
     constant into the base instead (`addiu $a1, $s3, 0x28' / offset 0) and
     every such loop also forces ONE pseudo for all six loops, so the whole
     callee-saved file shifts: 641 words, 254 aligned.

     func_s27_8002443C's THIRD ARGUMENT IS `(s16)(i * -0x19)', NOT A RAMP
     VARIABLE. The target holds the value PRE-SHIFTED -- `lui $s1, 0xFF1F'
     before the loop, `sra $a2, $s1, 16' at the call and
     `addu $s1, $s1, $v0' with `lui $v0, 0x190000' inside it. That is gcc
     strength-reducing `(i * -25) << 16' into its own biv. Written as a ramp
     variable (`g = -0xE1; ...; g += 0x19;') gcc keeps the plain value and
     sign-extends at the call (`li $s2,-225; sll; sra; addiu $s2,$s2,25'):
     162 aligned against 153, and 2 words shorter.

     THE STACK TARGET VECTOR IS REACHED THROUGH A POINTER. `tt = (u8 *)&targ;'
     and then `*(s16 *)(tt + 0x2)' / `*(s32 *)(tt + 0x0)'. The target keeps
     `$a2 = $sp + 0x10' live into the whole .390 block and addresses every
     field off it. Written as `targ.unk_00' / `*(s16 *)((u8 *)&targ + 0x2)'
     gcc addresses them off $sp directly (`lw $v0, 16($sp)'): 170 aligned
     against 162.

     THE m+0xA VALUE IS A `u16' LOCAL, THE b+0xA READ IS NOT CSE'd FROM IT.
     `zz = *(u16 *)(m + 0xA); *(s16 *)((u8 *)b + 0xA) = zz;' and the 0x8000
     arm is `*(s16 *)((u8 *)b + 0xA) = zz - 0x40;'. Without the local, the
     arm reloads m+0xA (`lhu' + `nop') because the earlier load's register is
     dead: 170 aligned against 162. With the local spelled `s16' the OTHER
     arm then CSEs `*(u16 *)((u8 *)b + 0xA)' out of it and loses the target's
     `lhu $v0, 0xA($s6)'.

     THE 0x10 Z OFFSET IS A SEPARATE STATEMENT, NOT `- (x - 0x10)'. Written
     `aa = *(s16 *)((u8 *)b + 0xA) - 0x10;' and then used. As one expression
     `*(s16 *)(r + 0xA) - (*(s16 *)((u8 *)b + 0xA) - 0x10)' the tree folder
     reassociates it to `+ 0x10 - x' and the target's `addiu $v1, $v1, -0x10'
     disappears from both the out[2] block and the b+0x16 division.

   EVERY ONE OF THE 152 REMAINING ROWS IS THE SAME REGISTER EXCHANGE, AND IT
   IS THE s14 PARK'S WINDOW 1 VERBATIM. Instruction selection, the switch,
   the jump table, the loop trip counts, the giv shapes and all nine arms'
   control flow agree. What differs is that the six abs blocks

       d = A - B; if (d < 0) { d = -d; } out[n] = d;

   put the difference in $v1 where the target puts it in $v0, and `r' (the
   actor record) in $a1 where the target puts it in $a2. The six words of
   shortfall are all downstream of that: two of the arms' tails become
   cross-jumpable in this body and are not in the target, because
   cross-jumping needs two IDENTICAL insns and only matching registers give
   it them (jump.c's find_cross_jump is called with minimum 2).

   THE ARITHMETIC, FROM THE `-da' DUMP. probe.c.greg says `d' is pseudo 90,
   allocated to hard reg 3 ($v1), and `;; 90 conflicts: ... 2 29' -- it
   conflicts with $v0 and so cannot have it. The two operand temps are local-
   alloc quantities, and local-alloc.c's qty_compare_1 ranks on
   floor_log2(n_refs) * n_refs * size / (death - birth) DESCENDING, so with
   equal reference counts the SHORTER-LIVED operand is allocated first and
   takes $v0. The minuend is expanded first (expand_expr's `binop:' always
   does operand 0 then operand 1), so the minuend is the longer-lived one and
   loses $v0 to the subtrahend. The target has it the other way round, which
   means the target's RTL loads the SUBTRAHEND first -- and no spelling I
   found makes gcc expand operand 1 first. `d' conflicts with $v0 because the
   WALK branch keeps the next target component live in $v0 across `d's range;
   the two allocations are therefore coupled and cannot be fixed separately.

   EVERYTHING I MEASURED, ALL ON THIS BODY:

     this body (the park's body)                                 644 w, 152
     abs written `A - B' instead of `B - A' (abs is symmetric)    644 w, 153
     three separate temps for the walk branch's abs              643 w, 153
     the same, each temp also carrying its target component       644 w, 158
     ONE extra temp shared by all three walk abs blocks           645 w, 234
     `d' declared last of the locals                              644 w, 153
     `r' declared after `tt'                                      644 w, 153
     a separate `rr' for case 3's actor record                    644 w, 153
     `s16 out[4]' instead of `s16 out[3]'                         644 w, 153
     the abs as macro residue -- the ternary with the difference
       spelled out three times, `(x<0) ? -(x) : (x)'              661 w, 161

   The four declaration-order and variable-splitting spellings are INERT to
   the row, which is the same verdict the s14 park reached after twelve
   spellings on the same window: the exchange is not decided by the order or
   the number of the C variables. What has NOT been tried here, and is the
   next move, is forcing the subtrahend to be expanded first -- it needs a
   construct that makes operand 1 of the MINUS the one gcc reaches first, and
   `-(B - A)' is not it (fold rewrites `-(a - b)' to `b - a' before expand).

   RECONCILED PROTOTYPES. Three of this body's callees landed in the same unit
   after it was parked and their real signatures are now known, so the
   declarations below are the reconciled ones: func_s27_800241F8 RETURNS the
   node it allocated (`UnkNode *', not `void'), func_s27_8002417C takes the
   dispatcher's FOUR arguments and func_s27_80024DF4 takes four and returns
   `s32'. THE BODY WAS RE-MEASURED AGAINST THEM AND DID NOT MOVE: still 644
   words, 152 aligned differences, 436 unaligned.


   DISPATCHER PASS, 2026-11-23 (batch 12): abs() ON THE FIRST TWO DELTAS IS
   WORTH 7 ROWS, AND THIS PARK'S WINDOW IS THE SAME ONE func_s14_80024AE4 HAS.

   This park says its remaining 152 rows are one register exchange and are
   "the s14 park's unsolved window verbatim". Confirmed, and the same lever
   moves both. Five `d = A - B; if (d < 0) { d = -d; }' blocks here; all 32
   combinations measured, the interesting ones:

       this park's body, all five manual          644/650 words,  152
       abs() on blocks 0 and 1                    644/650 words,  145  <-- BEST
       abs() on 0, 1 and 3                        646/650 words,  151
       abs() on 0, 1, 2                           644/650 words,  155
       abs() on all five                          646/650 words,  155

   Blocks 0 and 1 are the x and y deltas against `r'; take those to
   `out[0] = abs(...)' / `out[1] = abs(...)' and leave the other three. The
   same pattern holds on the s14 park -- abs on x and y, manual on z -- which
   is the useful part: `abs' is two words (mips.md's in-place `abssi2' arm:
   `bgez' / real `nop' / `negu') where the manual block is three, so a
   substitution the target did not make shows up as a word short, and x and y
   are the two it did make in both bodies.

   NOT SOLVED, and the route this park proposed is closed. It suggests one
   permuter run or one `-da' experiment for both bodies at once, on the theory
   that the target's RTL loads the subtrahend first. The direct test of that
   is the operand swap -- `|A - B| == |B - A|' makes it free in the source,
   and `expand_expr's `binop:' expands operand 0 first -- and on the s14 park
   all eight swap combinations are inert or worse (53 -> 55, 55, 57), never
   better. So the exchange is not reachable by operand order in either body,
   and a permuter run would be searching the same closed space.

   A WARNING ABOUT MEASURING: the figure in a park header is `aligndiff.py's
   SUMMARY LINE (`built N words, target N words, K aligned differences'), not
   a count of its `R' rows. On this park the two metrics rank the candidates
   DIFFERENTLY -- R-rows alone prefer the 0,1,3 combination, which the real
   figure scores at 151 against the 145 that R-rows rank lower. I ranked this
   sweep the wrong way round before noticing.

   DECLARATIONS
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   s32 func_8004491C(UnkNode *, u32);
   s32 func_800A44E0(s32, s32, s16, u32);
   s32 func_800A56E0(s32);
   s32 func_800BCB04(s32, s32, s16);
   void func_s27_80024050(u8 *, u8);
   void func_s27_800240EC(void);
   void func_s27_8002417C(u8 *, UnkVec *, u8 *, u8 *);
   UnkNode *func_s27_800241F8(u8 *, UnkVec *);
   u8 *func_s27_8002443C(u8 *, UnkVec *, s16, s16);
   s32 func_s27_80024DF4(u8 *, UnkVec *, u8 *, u8 *);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern u32 D_800814A0;
   extern s32 D_8008346C;
   END DECLARATIONS
*/
void func_s27_80024E64(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *n;
    u8 *m;
    u8 *e;
    u8 *r;
    u8 *t;
    u8 *tt;
    UnkVec targ;
    s16 out[3];
    s32 d;
    s32 i;
    s32 x;
    s32 y;
    s32 k;
    s32 aa;
    s16 tx;
    s16 ty;
    u16 bx;
    u16 by;
    s16 h;
    u16 zz;

    q = *(u8 **)(p + 0x0);
    for (i = 7; i >= 0; i--)
    {
        *(s16 *)(p + 0x1A + i * 2) = *(u16 *)(p + 0x1A + i * 2) + *(u16 *)(p + 0x18);
        *(s16 *)(p + 0x1A + i * 2) = *(s16 *)(p + 0x1A + i * 2) % 0x80;
    }
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x16) = *(u16 *)(p + 0x16) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0x16) = 0;
        *(s16 *)(p + 0x18) = 4;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        for (i = 7; i >= 0; i--)
        {
            *(s16 *)(p + 0x1A + i * 2) = 0;
        }
        for (i = 9; i >= 0; i--)
        {
            *(s32 *)(p + 0x2C + i * 4) = 0;
        }
        func_s27_800240EC();
        *(s16 *)(p + 0xE) = (*(u16 *)(q + 0x2A) >> 9) & 7;
        *(u32 *)(c + 0xC) = 0x808080;
        /* fallthrough */
    case 1:
        n = q - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(n + 0xC) + 0x8), (UnkSlot *)*(u8 **)(n + 0xC), out,
                          0) == 0 &&
            (*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        m = *(u8 **)(n + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        zz = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)b + 0xA) = zz;
        if ((*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) += out[0];
            *(s16 *)((u8 *)b + 0x6) += out[1];
            *(s16 *)((u8 *)b + 0xA) += out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = zz - 0x40;
        }
        if ((*(u16 *)(p + 0xC) & 1) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)func_s27_80024DF4);
            *(s16 *)(p + 0xC) = *(u16 *)(p + 0xC) | 1;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (*(u8 **)(q + 0x60) != NULL)
        {
            r = *(u8 **)(*(u8 **)(q + 0x60) - 0x18);
            d = *(s16 *)((u8 *)b + 0x2) - *(s16 *)(r + 0x2);
            if (d < 0)
            {
                d = -d;
            }
            out[0] = d;
            d = *(s16 *)((u8 *)b + 0x6) - *(s16 *)(r + 0x6);
            if (d < 0)
            {
                d = -d;
            }
            out[1] = d;
            if ((*(u32 *)(*(u8 **)(q + 0x60) + 0x1C) & 0x40000) != 0)
            {
                aa = *(s16 *)((u8 *)b + 0xA) - 0x10;
                d = aa - *(s16 *)(r + 0xA);
            }
            else
            {
                aa = *(s16 *)((u8 *)b + 0xA);
                d = aa - *(s16 *)(r + 0xA);
            }
            if (d < 0)
            {
                d = -d;
            }
            out[2] = d;
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if (*(s16 *)(p + 0x12) < out[i])
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = *(s16 *)(p + 0x12) >> 5;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            *(s16 *)((u8 *)b + 0xE) =
                (*(s16 *)(r + 0x2) - *(s16 *)((u8 *)b + 0x2)) / *(s16 *)(p + 0x12);
            *(s16 *)((u8 *)b + 0x12) =
                (*(s16 *)(r + 0x6) - *(s16 *)((u8 *)b + 0x6)) / *(s16 *)(p + 0x12);
            if ((*(u32 *)(*(u8 **)(q + 0x60) + 0x1C) & 0x40000) != 0)
            {
                aa = *(s16 *)((u8 *)b + 0xA) - 0x10;
                *(s16 *)((u8 *)b + 0x16) = (*(s16 *)(r + 0xA) - aa) / *(s16 *)(p + 0x12);
            }
            else
            {
                *(s16 *)((u8 *)b + 0x16) =
                    (*(s16 *)(r + 0xA) - *(s16 *)((u8 *)b + 0xA)) / *(s16 *)(p + 0x12);
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
        }
        else
        {
            i = 0;
            m = *(u8 **)(n + 0xC);
            tx = m[0x24];
            ty = m[0x25];
            bx = tx;
            by = ty;
            do
            {
                x = tx;
                y = ty;
                if ((s16)func_800A44E0((x << 6) & 0xFFC0, (y << 6) & 0xFFC0, *(s16 *)(q + 0x88),
                                       (s16)(*(u16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                k = *(s16 *)(p + 0xE) * 2;
                h = func_800BCB04((((x + *(s16 *)(k + (s32)D_8006CCD8)) << 6) + 0x20) & 0xFFE0,
                                  (((y + *(s16 *)(k + (s32)D_8006CCE8)) << 6) + 0x20) & 0xFFE0,
                                  (s16)(*(u16 *)(q + 0x88) - 0x20));
                if (h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(q + 0x88)) < -0x3F)
                {
                    break;
                }
                i++;
                k = *(s16 *)(p + 0xE) * 2;
                tx = tx + *(u16 *)(k + (s32)D_8006CCD8);
                ty = ty + *(u16 *)(k + (s32)D_8006CCE8);
                bx = tx;
                by = ty;
            } while (i < 8);
            tt = (u8 *)&targ;
            k = *(s16 *)(p + 0xE) * 2;
            *(s16 *)(tt + 0x2) =
                ((s32)(bx << 16) >> 10) + ((*(s16 *)(k + (s32)D_8006CCD8) + 1) << 5);
            k = *(s16 *)(p + 0xE) * 2;
            *(s16 *)(tt + 0x6) =
                ((s32)(by << 16) >> 10) + ((*(s16 *)(k + (s32)D_8006CCE8) + 1) << 5);
            *(s16 *)(tt + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            d = *(s16 *)((u8 *)b + 0x2) - *(s16 *)(tt + 0x2);
            if (d < 0)
            {
                d = -d;
            }
            out[0] = d;
            d = *(s16 *)((u8 *)b + 0x6) - *(s16 *)(tt + 0x6);
            if (d < 0)
            {
                d = -d;
            }
            out[1] = d;
            d = *(s16 *)((u8 *)b + 0xA) - *(s16 *)(tt + 0xA);
            if (d < 0)
            {
                d = -d;
            }
            out[2] = d;
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if (*(s16 *)(p + 0x12) < out[i])
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = *(s16 *)(p + 0x12) >> 5;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (*(s32 *)(tt + 0x0) - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (*(s32 *)(tt + 0x4) - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (*(s32 *)(tt + 0x8) - b->unk_08) / *(s16 *)(p + 0x12);
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
        }
        break;
    case 2:
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + *(u16 *)((u8 *)b + 0xE);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + *(u16 *)((u8 *)b + 0x12);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + *(u16 *)((u8 *)b + 0x16);
        func_s27_800241F8(p, b);
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 8)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            r = *(u8 **)(*(u8 **)(q + 0x60) - 0x18);
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(r + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(r + 0x6);
            if ((*(u32 *)(*(u8 **)(q + 0x60) + 0x1C) & 0x40000) != 0)
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r + 0xA) + 0x10;
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(r + 0xA);
            }
            k = 0x10;
            for (i = 7; i > 0; i--)
            {
                *(s16 *)(p + 0x1A + i * 2) = k;
                k += 0x10;
            }
            *(s16 *)(p + 0x1A) = 0;
            for (i = 9; i >= 0; i--)
            {
                *(s32 *)(p + 0x2C + i * 4) =
                    (s32)func_s27_8002443C(p, b, (s16)(i * -0x19), (s16)i);
            }
        }
        break;
    case 4:
        for (i = 9; i >= 0; i--)
        {
            t = *(u8 **)(*(u8 **)(p + 0x2C + i * 4) + 0xC);
            *(u32 *)(t + 0xC) = *(u32 *)(t + 0xC) + 0x40404;
        }
        if (*(s16 *)(p + 0x16) >= 0x20)
        {
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x16) >= 0x20)
        {
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        for (i = 9; i >= 0; i--)
        {
            t = *(u8 **)(*(u8 **)(p + 0x2C + i * 4) + 0xC);
            *(u32 *)(t + 0xC) = *(u32 *)(t + 0xC) + 0xFFFBFBFC;
        }
        if (*(s16 *)(p + 0x16) >= 0x20)
        {
            func_s27_80024050(*(u8 **)(q + 0x60), p[0x9]);
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            for (i = 9; i >= 0; i--)
            {
                e = *(u8 **)(p + 0x2C + i * 4);
                if (e != NULL)
                {
                    *(s16 *)(e + 0x1E) = *(u16 *)(e + 0x1E) | 0x8000;
                    D_800814A0 |= 0x8000;
                    *(s32 *)(p + 0x2C + i * 4) = 0;
                }
            }
        }
        break;
    case 7:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(s16 *)(p - 0x2) = *(u16 *)(p - 0x2) | 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    case 8:
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + *(u16 *)((u8 *)b + 0xE);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + *(u16 *)((u8 *)b + 0x12);
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + *(u16 *)((u8 *)b + 0x16);
        func_s27_800241F8(p, b);
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 9:
        if (*(s16 *)(p + 0x10) >= 8)
        {
            *(s16 *)(p + 0xA) = 7;
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0x16) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
