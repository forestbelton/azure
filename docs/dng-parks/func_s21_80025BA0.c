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
