/* PARK: func_s21_80025BA0, csrc/ovl_dng_s21.c.

   THIS PARK IS THE WHOLE FAMILY'S, AND IT ABSORBED A SECOND ONE. Until batch
   12 this body was parked TWICE -- here and as func_s20_80026010 (agent-h13,
   23 aligned differences) -- by two agents who each recorded that the other
   member was "still an INCLUDE_ASM, so there was no landed sibling to read"
   and neither of whom looked for a park on it. The two files were kept
   because they reached their figures by DIFFERENT C shapes, and the ladder
   below is why that mattered: h13's integer address form plus this park's
   index locals is the hybrid neither agent tried, and it is what took the
   residue from 16 to 5. h13's file is deleted now that its content is here;
   `git log` has it in full. Landing either member lands both (dngfam: 2
   members, 170 words each).
   agent-h38, 2026-11-23, batch 12.  WAS 16 ALIGNED DIFFERENCES; NOW 5.

   TARGET 170 words.  BEST BUILT 170 words, 5 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py; ovlverify also reports 5 masked, 37 raw).
   2 members, 170 words each, 340 words total (tools/decomp/dngfam.py:
   func_s20_80026010 in ovl_dng_s20, func_s21_80025BA0).  Landing either
   member lands both via dngclone.py.

   THE BODY IN THIS FILE IS A NEW SHAPE AND IT SUPERSEDES BOTH EARLIER ONES.
   It is the HYBRID that neither of the two earlier parks tried:

     * POINTER locals for the three corner addresses, in the INTEGER form
       `((index) << 3) + (u32)D_800265C8', which is h13's shape from
       the now-merged func_s20_80026010 park (agent-h13) -- that is what
       gives the target's
       strictly serial index block and the `addu <index>, <base>' operand
       order, and it is what the first s21 park (index locals plus array refs
       off a pointer local `t') could not reach: its own note records three
       pointer locals measuring 172 words / 153, because it tried them with
       ARRAY REFS rather than the integer form.
     * INDEX locals `ia', `ib', `ic' for the index arithmetic, which is the
       FIRST s21 park's shape.  h13's body wrote the indices inline and `fold'
       then reassociated `(i & -0x10) + (((i + 1) & 0xF) + 0x10)' into
       `((i & -0x10) + 0x10) + ((i + 1) & 0xF)' -- putting the `+ 0x10' on the
       WRONG term.  That is the whole of h13's 23-difference residue.

   WHAT IT DOES.  The 0x202 spawner for the trail ring: for every live entry
   of D_800265C8 take a node, park func_s21_800255E8 as its step and
   func_s21_800259DC as its draw, seed the mover at the average of four ring
   entries (entry i, the next entry in its 0x10-wide ring, and the two
   matching entries of the second bank at D_80026648 = D_800265C8 + 0x80),
   jitter the three velocities by `(rand() & 0xF) - 8' in the high halfword,
   jitter the slot's +0x16/+0x18 by `(rand() & 0x1FF) - 0x100', set the
   payload's +0x1A life to 0x20, copy the four ring entries into the corner
   records at +0x34..+0x53, pick the colour pair by `i %% 0x20 < 0x10', and
   clear D_80026476 on the way out.

   NO TWIN outside the family.  tools/decomp/dngtwin.py --unit ovl_dng_s21
   scores func_s20_80026010 at 1.000 (the clone) and nothing else above 0.45.

   THE RESIDUE IS FIVE ROWS AND ALL FIVE ARE ONE REGISTER.  `ib'/`ic' -- the
   `(i + 1) & 0xF' chain -- lands in $v0 where the target has $v1.  Nothing
   else differs: word count, instruction selection, instruction order, the
   $s0/$s1 assignment that was h13's whole residue, and every other register
   agree exactly.  The five rows are target words 32, 33, 34, 40 and 41:

     addiu v0,s5,1     / addiu $v1, $s5, 0x1
     andi  v0,v0,0xf   / andi  $v1, $v1, 0xF
     addu  s3,s1,v0    / addu  $s3, $s1, $v1
     addiu v0,v0,16    / addiu $v1, $v1, 0x10
     addu  s1,s1,v0    / addu  $s1, $s1, $v1

   THE MECHANISM, read out of `-da'.  `tools/cdkgcc -da -O2 -G0 -Iinclude
   -Ipsyq/include -Icsrc -S' on this body gives, for func_s21_80025BA0:

     ;; 7 regs to allocate: 87 209 88 80 237 109 196
     ;; Register dispositions: ... 81 in 16  83 in 20  84 in 18  85 in 17
                                   86 in 16  92 in 17  93 in 2  94 in 2
                                   105 in 17  106 in 2  111 in 18  112 in 18
                                   115 in 17  116 in 17

   81 = m -> $s0, 86 = e -> $s0, 85 = c -> $s1, 83 = a -> $s4, 84 = b -> $s2:
   ALL CORRECT, which is the part this file fixes.  93 = `(i + 1) & 0xF',
   94 = `ib + 0x10' and 106 = `i + 1' are one local-alloc quantity (each
   link's source dies at the next, so `combine_regs' ties them) and it is
   BLOCK-LOCAL with `qty_n_calls_crossed == 0'.  local-alloc.c's
   `find_free_reg' then scans plain ascending hard-register order -- cdk-gcc's
   mips.h defines NO `REG_ALLOC_ORDER', confirmed in
   scratch/gcc-src/cdk-gcc-b18/config/mips/mips.h -- and takes the first free
   call-used register, which is $v0.  For the target to use $v1 instead,
   SOMETHING ELSE MUST HOLD $v0 LIVE ACROSS TARGET WORDS 30..43, and no value
   in the emitted target touches $v0 between the `sw $v0, 0x10($s6)' in the
   func_8004491C delay slot (word 29) and the `lh $v0, 0x0($s4)' ring load
   (word 45).  Whatever it is produces no instruction in that window.

   AGENT-h130, 2026-11-24, BATCH 30.  STILL 5 ALIGNED DIFFERENCES.  The body
   below is unchanged: I reproduced agent-h38's 170 words / 5 aligned rows
   exactly, byte for byte, with the same five rows at the same target words,
   and 13 further spellings are priced below.  (My dispatch brief said this
   park stood at 23 rows; that is the figure of the DELETED h13 park.  The
   file is right and the brief was wrong -- 5 is what this shape measures.)

   THE DIAGNOSIS IS NARROWED FROM `find the missing $v0 consumer' TO A PROOF
   THAT THERE IS NO SUCH CONSUMER REACHABLE AT 170 WORDS.  I read sched1's
   output (`probe.c.sched') rather than only `.lreg'/`.greg', which names the
   pseudos and, more usefully, the TIES.  In sched1 order the window is

     insn 53   pseudo 105 = -0x10                  (li)
     insn 54   pseudo  92 = ia = i & -0x10         (and)   <- chain's rival
     insn 57   pseudo 106 = i + 1
     insn 58   pseudo  93 = ib = (i + 1) & 0xF
     insn 61   pseudo 107 = ia + ib
     insn 62   pseudo 108 = 107 << 3
     insn 66   pseudo  83 = a = 108 + base
     insn 69   pseudo 111 = i + 0x10
     insn 70   pseudo 112 = 111 << 3
     insn 74   pseudo  84 = b = 112 + base
     insn 77   pseudo  94 = ic = ib + 0x10         (REG_DEAD 93 here)
     insn 80   pseudo 115 = ia + ic                (REG_DEAD 94 here)

   so the chain 106 -> 93 -> 94 is one `combine_regs' quantity whose range is
   insns 57..80, and that range CONTAINS six other values: 107, 108, 83, 111,
   112, 84.  Those are the only candidates to hold $v0 across the chain, and
   every one of them is unavailable, for a reason that is a fact about the
   TARGET and not about this body:

     * 107/108 are tied into one quantity with each other (each dies at the
       next) and 108 IS USED AGAIN AFTER THE rand() CALLS -- it is the scaled
       `a' index the target re-adds D_80026648 to with `addu $s3, $s3, $t3'.
       So that quantity crosses five calls, `find_free_reg' starts from
       `call_used_reg_set', and it can only have a callee-saved register.  It
       gets $s3 in both bodies.
     * 111/112/84 are one quantity ending in `b', which crosses the same five
       calls: $s2 in both bodies.
     * 83 is `a' and 92 is `ia' (tied to `c'): $s4 and $s1, both call-crossing.

   I CONFIRMED 108's call-crossing IS FORCED, not a choice of this body: the
   second-bank copy written off the pointer instead, `*(u32 *)((u8 *)a +
   0x80)', comes out 168 words / 62 -- TWO WORDS SHORT, which is the
   `lui'/`addiu D_80026648' pair the target really emits.  So there is no
   spelling that frees 108 from the calls while keeping 170 words, and with
   all six rivals callee-saved nothing marks $v0 in `regs_live_at' over insns
   57..80.  `find_free_reg' then scans ascending (cdk-gcc's mips.h has no
   `REG_ALLOC_ORDER') and hands the chain $v0.

   WHAT THIS MEANS FOR THE NEXT AGENT.  Do not look for another statement
   order and do not look for a reference bump: the quantity's priority is
   irrelevant here, because NO rival wants $v0 at all, so the chain takes it
   whatever its rank.  The only two things that can move it are
     (a) a value that is born before insn 57, dies after insn 80, does NOT
         cross a call, and costs no instruction -- which I believe is
         impossible, since a quantity needs a set and a use; or
     (b) the chain itself crossing a call, which I MEASURED for the first time
         (it was listed as untried): the whole index block moved above
         func_8004491C is 170 words / 24 rows, and it does hold the word
         count, so the register IS reachable from the quantity's own
         properties -- just in the wrong direction (callee-saved, not $v1).
   My own reading is that these five rows are not a C-level fact at all and
   that the next real move is the permuter on the index block, or accepting
   the park.  They are 5 rows out of 170 words and the whole rest of the body,
   including the $s0..$s7/$fp assignment, is exact.

   EVERY SPELLING agent-h130 MEASURED, all 170 words unless stated, all
   against the 5-row body:

     `ic = ((i + 1) & 0xF) + 0x10' recomputed instead of `ib + 0x10'      5
       (cse unifies it back to `ib + 0x10'; identical code)
     `a's index as `(ib + ia)' rather than `(ia + ib)'                    5
     `c's index as `(ic + ia)' rather than `(ia + ic)'                   23
     a separate local `j' for `i + 1', then `ib = j & 0xF'                5
     `(ia + ib) * 8' for the `a' index instead of `<< 3'                  5
     `(ia + ic) * 8' for the `c' index instead of `<< 3'                  5
     `a = &D_800265C8[ia + ib]' (array ref) with b and c left integer     5
     `b's scaled index hoisted into its own local used twice             27
     the second-bank `a' copy as `*(u32 *)((u8 *)a + 0x80)'
         -- TWO WORDS SHORT, so the target really emits the
            lui/addiu D_80026648 pair                     168 words,     62
     the second-bank `a' copy indexed `(ic - 0x10 + ia)'    176 words,    68
     the second-bank `a' copy indexed `(ia + ic - 0x10)'                 32
     the whole index block moved ABOVE func_8004491C
         (the chain then crosses the call)                               24
     `n->unk_10 = step' moved BELOW the index block                      13

   The last two are the park's own two "what I would try next" shapes, now
   priced.  Neither helps, and the 13-row one is the closest miss in this
   file after the kept body.

   WHAT I WOULD TRY NEXT.  Find the missing $v0 consumer, not another
   statement order -- order is exhausted below.  Two concrete shapes:
     * a value computed before the index block and used after it that
       `combine' folds away to nothing, so it costs no word but still holds a
       register at local-alloc time;
     * getting the `ib' quantity to CROSS A CALL, which makes
       `find_free_reg' exclude every call-used register -- that is the wrong
       direction for $v1, but it proves whether the register choice is
       reachable at all from the quantity's own properties.
   The one thing I did NOT measure is a device that spends a word: every row
   below holds the word count at 170, and a 171-word body is not a landing.

   EVERY SPELLING MEASURED IN THIS RUN, all 170 words unless stated.
   The first block is the shape search; the second is this shape's details.

     POINTER locals, integer form, indices inline (h13's body
         translated: the untested combination the dispatcher flagged)      23
     ... with `c's index written `(((i + 1) & 0xF) + 0x10 + (i & -0x10))'
         so fold cannot move the constant onto the `i & -0x10' term         7
     ... with `ia'/`ib'/`ic' index locals and `ic = ib + 0x10' AFTER the
         `a' and `b' statements                      (THIS BODY)            5
     ... `ic = ib + 0x10' before `a'                                       17
     ... `ic = ib + 0x10' between `a' and `b'                               7
     ... `ib += 0x10' in place of a separate `ic'                          28
     ... `c's index as `(ia + (ib + 0x10))' with no `ic' local             23
     ... `ib' assigned before `ia'                                          9
     ... the `b' statement before the `a' statement        174 words,      36

     STATEMENT ORDER OF THE EIGHT HEAD STATEMENTS (ia, ib, a, b, ic, c, w, m)
     -- ten orders, every one that keeps `ic' after `a' measures 5 and the
     two that move it measure 9 and 11, so NOTHING ELSE IN THE HEAD IS
     ORDER-SENSITIVE:
       ia ib a b ic c w m   (this body)                                     5
       ia ib w a b ic c m / ia ib m a b ic c w / m ia ib a b ic c w
         / w m ia ib a b ic c / ia ib a b w ic c m / ia ib a b ic c m w
         / ia ib a b m ic c w                                         5 each
       ia ib a ic c b w m                                                  11
       ib ia a b ic c w m                                                   9

     MEASURED AND FREE on this shape, so NOT reasons it reaches 5:
       the `>> 2' inside the three sums instead of in the three stores       5
       `x'/`y'/`z' declared in the order z, y, x                            5
       `m' as a `u8 *' with `*(u32 *)(m + 0xC)' casts                       5
       `ia'/`ib'/`ic' declared as `u32'                                     5
       `ic' declared `s16'                                                  5
       `ia'/`ib'/`ic' declared before `i'/`x'/`y'/`z', and `ib'/`ic'
           before `ia'                                                      5
       a local for the `a' sum index, for `i + 0x10', or for `ia + ic'       5
       `((s16 *)m)[1]' array refs for the three mover stores                5

     RE-MEASURED FROM h13's PARK ON THIS SHAPE and still negative, so its
     conclusion carries:
       `do { } while (0)' around the `m->unk_14' store      175 words,     42
       `do { } while (0)' around the `m->unk_0C' store      175 words,     42
       `m = n->unk_08' moved after the three sums                         24

   WHY THE EARLIER s21 BODY'S FOUR "LOAD-BEARING" FACTS NO LONGER APPLY.
   That body addressed the ring through a pointer local `t = D_800265C8'
   assigned inside the loop and indexed it `t[i]', `t[j]', `t[k]', `t[l]'.
   This body has no `t': the first corner is `&D_800265C8[i]' (which loop.c
   strength-reduces into the target's `$s7' giv, incremented by 8 in the
   loop's branch delay slot) and the other three are integer addresses off
   the hoisted `$fp' base.  Its three other facts survive in a changed form
   and are visible in the body: the three averages still go into `x'/`y'/`z'
   locals and are stored afterwards, the second bank D_80026648 is still its
   own symbol for the two word copies (`(i << 3) + (u32)D_80026648' and
   `((ia + ib) << 3) + (u32)D_80026648', which is where the target's two
   `lui/addiu D_80026648' pairs and its `addu $s3, $s3, $t3' reuse of the
   scaled `a' index come from), and `i + 0x10' is still a separate index
   rather than a displacement off the biv.

   `rand' IS func_80069EF8 (splat/common_syms.txt), so the five `jal
   func_80069EF8' in the listing are the five `rand()' calls here; the real
   name needs no declaration.

   raw 37 == the 5 masked rows plus 32 relocation and local-label words:
   R_MIPS_26 on func_8003FC64, func_8004491C and rand five times, HI16/LO16
   pairs on D_80026476 three times, D_800265C8, func_s21_800255E8,
   func_s21_800259DC, D_80026648 twice, D_800265C0 twice and D_800265C4
   twice, and the body's own `j'/`beqz'/`bnez' targets.

   func_s21_800255E8 and func_s21_800259DC both need NO declaration: both are
   defined above this function in csrc/ovl_dng_s21.c.  Of the block below,
   only D_80026648 is new to that unit as of this run.

DECLARATIONS
typedef struct UnkDng26478
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng26478;
extern UnkDng26478 D_800265C8[];
extern UnkDng26478 D_80026648[];
extern s16 D_80026476;
extern u32 D_800265C0;
extern u32 D_800265C4;
UnkNode *func_8003FC64(s32);
END DECLARATIONS */
void func_s21_80025BA0(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkDng26478 *w;
    UnkDng26478 *a;
    UnkDng26478 *b;
    UnkDng26478 *c;
    u8 *e;
    u8 *q;
    s32 i;
    s32 x;
    s32 y;
    s32 z;
    s32 ia;
    s32 ib;
    s32 ic;

    for (i = 0; i < D_80026476; i++)
    {
        n = func_8003FC64(0x202);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s21_800255E8;
            func_8004491C(n, (u32)func_s21_800259DC);
            ia = i & -0x10;
            ib = (i + 1) & 0xF;
            a = (UnkDng26478 *)(((ia + ib) << 3) + (u32)D_800265C8);
            b = (UnkDng26478 *)(((i + 0x10) << 3) + (u32)D_800265C8);
            ic = ib + 0x10;
            c = (UnkDng26478 *)(((ia + ic) << 3) + (u32)D_800265C8);
            w = &D_800265C8[i];
            m = n->unk_08;
            x = w->unk_00 + a->unk_00 + b->unk_00 + c->unk_00;
            y = w->unk_02 + a->unk_02 + b->unk_02 + c->unk_02;
            z = w->unk_04 + a->unk_04 + b->unk_04 + c->unk_04;
            *(s16 *)((u8 *)m + 0x2) = x >> 2;
            *(s16 *)((u8 *)m + 0x6) = y >> 2;
            *(s16 *)((u8 *)m + 0xA) = z >> 2;
            m->unk_0C = ((rand() & 0xF) - 0x8) << 16;
            m->unk_10 = ((rand() & 0xF) - 0x8) << 16;
            m->unk_14 = ((rand() & 0xF) - 0x8) << 16;
            e = (u8 *)n->unk_0C;
            *(s16 *)(e + 0x16) = (rand() & 0x1FF) - 0x100;
            *(s16 *)(e + 0x18) = (rand() & 0x1FF) - 0x100;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0x1A) = 0x20;
            *(u32 *)(q + 0x34) = *(u32 *)w;
            *(s16 *)(q + 0x38) = w->unk_04;
            *(u32 *)(q + 0x3C) = *(u32 *)a;
            *(s16 *)(q + 0x40) = a->unk_04;
            *(u32 *)(q + 0x44) = *(u32 *)((i << 3) + (u32)D_80026648);
            *(s16 *)(q + 0x48) = b->unk_04;
            *(u32 *)(q + 0x4C) = *(u32 *)(((ia + ib) << 3) + (u32)D_80026648);
            *(s16 *)(q + 0x50) = c->unk_04;
            if (i % 0x20 < 0x10)
            {
                *(u32 *)(q + 0x54) = D_800265C0;
                *(u32 *)(q + 0x58) = D_800265C4;
            }
            else
            {
                *(u32 *)(q + 0x58) = D_800265C0;
                *(u32 *)(q + 0x54) = D_800265C4;
            }
        }
    }
    D_80026476 = 0;
}
