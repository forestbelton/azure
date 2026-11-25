/* func_s67_80024E54 -- PARKED.  1227 target words, 1224 built, 44 ALIGNED
   differences (ovlverify's unaligned figure is 806; always size this one with
   aligndiff.py).  dngfam.py: 1 MEMBER, 1227 words, so this is not a
   replication representative -- it is simply the largest single function left
   in DUNGEON.BIN.

   Progress across this run, every figure from aligndiff.py:

       first compile                                  1231 w / 371 rows
       + four D_80083780 pointer locals               1227 w / 342
       + nested ifs, three D_80082E80 locals          1226 w / 332
       + `u = i * 2' hoisted, rows base local         1224 w / 162
       + `kj = kk + 1' local                          1225 w / 151
       + tail-loop arguments through s32 locals       1225 w / 114
       + 0x2A read hoisted into a local               1223 w / 106
       + 0x1C..0x28 stored in ASCENDING order         1225 w /  70
       + `u = i << 1' for `u = i * 2'                 1224 w /  61
       + row stores through rw/rw2, dv local          1224 w /  57
       + w2 split off w, dv2 local                    1224 w /  44

   WHAT THIS FUNCTION IS.  The bank's five-state cutscene dispatcher on
   *(s16 *)(p + 0xA), a JUMP TABLE: it OWNS jtbl_s67_80024078, 5 words = 5
   cases + 0 pad, dispatch bound 5 (`sltiu $v0, $v1, 0x5'), 5 DISTINCT
   targets, and the range check's `beqz' lands on the function's COMMON TAIL
   so there is NO default among the five and no `default:' label is written.
   Entries in table order:

       case 0 -> .Ls67_80024F4C     case 3 -> .Ls67_800260B8
       case 1 -> .Ls67_80024FA8     case 4 -> .Ls67_800260F4
       case 2 -> .Ls67_80025014

   The five entry addresses are strictly ascending, so the arms are written
   ascending 0..4, which is their source order.  Case 0 FALLS THROUGH into
   case 1 and case 3 FALLS THROUGH into case 4; both fallthroughs are visible
   in the listing as a table target sitting immediately after the previous
   arm's last store with no intervening branch.

   State 0 clears D_800814A8's +0xF4, arms its +0x96, sets D_80082E86 to 6,
   bumps the state and publishes D_80083780's three position halfwords into
   the payload's +0x84..+0x88.  State 1 waits for the view's +0x0 to carry
   0x80, then arms a 0x21 countdown at +0x32, drops D_800814A8's +0xA6, and
   plants a 3-bit facing from D_800814A8's +0x2A.  State 2 is the body of the
   function: it spends the countdown, fires func_800A56E0 at 0x12, and at
   specific countdown values spawns up to SEVEN different 0x212 particle
   nodes (0x18 twice -- once ten times in a loop -- then 4, 2, and 1 three
   times), writes two six-byte rows into the payload at +0x54, runs a
   sixteen-iteration func_s67_80024C50 sweep, and finally spawns the
   func_s67_800245B8 node off D_800814A8's +0x60 list.  State 3 counts +0x32
   down unless D_80082E94 carries 0x8000 and FALLS THROUGH into state 4,
   which either tears the actor down (dropping D_80083460's +0xA, clearing
   its +0xC and D_80082E86) or just clears D_800261B0.

   NO TWIN AT ANY RATIO -- dngtwin finds no carved sibling at 0.60 or better.
   The whole body was read off the listing.  The two donors that made it
   tractable are both ALREADY IN THIS UNIT: func_s67_80024C50 (landed, 129
   words) gave the entire 0x212 spawn skeleton, the `dd = *(UnkDngS67Dirs *)
   D_s67_80024054' struct-assignment table copy and the `d + (index << 2)'
   addressing, and csrc/ovl_dng_s63.c's func_s63_8002401C plus
   csrc/ovl_dng_s55.c's func_s55_80024078 gave the dispatcher shape and the
   cue gate.

   THE REMAINING 44 ROWS ARE FOUR GROUPS, and 34 of them are ONE CAUSE.

   GROUP 1 -- 34 rows and ALL THREE MISSING WORDS, the rows block at
   0x80025378..0x800254EC.  Two six-byte rows are written at
   `p + k * 12 + 0x54' and `p + kj * 6 + 0x54'; each of the six elements is
   `<first term> + (*(s16 *)(D_80083780 + N) - *(s16 *)(p + 0x84 + M)) / 2'.
   The TARGET loads D_80083780 + 0x2/0x6/0xA SIX times, three in each row,
   off a single base register $a3 that it materialises once
   (`addiu $a3, $v0, %lo(D_80083780)').  Every spelling I tried loads them
   THREE times and carries the values in $t1/$t0/$a2 across both rows, which
   is the three missing words plus the register displacement that follows.

   The mechanism is cse's memory disambiguation, and I PROVED it with an
   alias experiment rather than guessing: declaring
   `extern u8 D_80083780_b[] __asm__("D_80083780");' and reading the second
   row through that name makes the two rows' loads different EXPRESSIONS to
   cse, and the three loads come straight back -- 1228 words / 40 rows with
   the second row direct, 1229 / 36 with it through a second pointer local.
   So the loads are being folded by cse, not by the scheduler, and the row
   stores (base `p + k * 12', a varying register) are NOT invalidating a
   load whose base cse knows to be a symbol address.  Both alias forms also
   cost a SECOND `lui'/`addiu' base pair, which is why they overshoot 1227;
   the spelling that is actually wanted keeps ONE base and still reloads, and
   I did not find it.  NOTE FOR WHOEVER PICKS THIS UP: do not spend the
   budget on more pointer-local shapes -- six were measured (table below) and
   all six fold identically.  The question is what makes cse treat
   `(mem (plus x3 2))' as killed by `(mem (plus (p + 12k) 0x54))'.

   GROUP 2 -- 6 rows, case 0 at 0x80024F50..0x80024F5C.  The target emits
   `sw $zero, 0xF4($v1)' BEFORE `sh $v0, 0x96($v1)'; every source order emits
   them the other way round, and the D_800814A8 pointer lands in $a0 where
   the target has $v1.  BOTH SOURCE ORDERS MEASURE THE SAME 44, so the order
   is free and this is not a statement-order fact -- it is the same
   allocation question as group 3 one arm earlier.  s55's `+0xF4 as an array
   ref' lever (`((s32 *)w)[0x3D] = 0') was measured here and is ALSO free:
   44 either way.  So neither of s55's two candidate levers applies.

   GROUP 3 -- 2 rows, case 1 at 0x80024FC0.  %hi(D_800814A8) and the loaded
   pointer are exchanged ($v1/$a0 against the target's $a0/$v1).  This was
   13 rows until the `w2' split below; what is left is the last pair.

   GROUP 4 -- 4 rows in blocks F and G, 0x80025B80 and 0x80025DC8.  The
   second of the two dirs-table reads per block is `lhu' where the target has
   `lh'.  The FIRST one was fixed by taking the load into an s32 local
   (`dv', `dv2'), which forces the sign-extending form because the local is
   a full int; the second site still folds to `lhu' through the shift.  The
   `addu $v1, $v1, $v0' / `addu $v0, $v1, $v0' pair beside it is the same two
   rows.

   WHAT A PERMUTER RUN SHOULD BE POINTED AT.  One statement:

       *(s16 *)(rw2 + 0x54) = *(s16 *)(p + 0x2C) / 4 +
                              (*(s16 *)(x3 + 0x2) - *(s16 *)(p + 0x84)) / 2;

   and its two siblings at +0x56/+0x58.  Everything else in the body is at 0.

   MEASURED AND REJECTED -- every spelling I tried, with its aligndiff score.
   All scores are against the baseline stated in the row, so read the
   progress table above for what each one was added to.

     ROWS BLOCK (all six fold the loads identically):
       x3 pointer local, both rows                     1224 w / 44  KEPT
       direct D_80083780 refs, both rows               1224 w / 44  base
                                                       folds to symbol+2
       x3 row 1, direct refs row 2                     1224 w / 44
       direct refs row 1, x3 row 2                     1224 w / 44
       x3 row 1, x4 = D_80083780 row 2                 1225 w / 70 (at the
                                                       70 baseline: free)
       x3 assigned a second time between the rows      1225 w / 70 (free)
       alias symbol, row 2 direct                      1228 w / 40
       alias symbol, row 2 via x4                      1229 w / 36
       row stores through rw/rw2 pointer locals        -2 rows  KEPT

     THINGS THAT COST REAL ROWS IF SPELLED THE OTHER WAY:
       `(diff) * (i * 2) / 16' written inline          1227 w / 220
         -- fold reassociates to `(diff * 2) * i', the `i * 2' stops being a
         shared subexpression, and this ALSO rotates p from $s3 to $s4 across
         all 1227 words.  This one change was 162 rows down to 61 at the
         time.  It is the largest single fact on this body.
       `u = i * 2' instead of `u = i << 1'            1224 w / 61 -> 70, +9
       tail-loop arguments written inline in the call  1225 w / 151 (vs 114)
       `x < 0xA && x < 6 && x != 3' as an && chain     the front end's
         fold_truthop collapses it to `x < 6' and the target's
         `slti $v0, $v1, 0xA' / `beqz' pair disappears: 2 words short.
         NESTED IFS ARE REQUIRED.
       `(kk + 1) * 6' instead of the `kj' local        1224 w / 154 (vs 151)
         -- fold distributes it to `kk * 6 + 6' and folds the 6 into the
         store displacement, so the stores come out at +0x5A/+0x5C/+0x5E.
       0x1C/0x24/0x28/0x20 in the LISTING'S order      1225 w / 106 (vs 70)
         -- 36 rows across the four dress blocks.  The four colour words must
         be stored in ASCENDING source order; the listing's
         0x1C,0x24,0x28,0x20 is the scheduler's doing.  Chained
         (`*(q+0x1C) = *(q+0x20) = A') is 78, and reading +0x14/+0x18 off
         `(u8 *)n + 0x44/0x48' instead of `q + 0x24/0x28' is 160.
       0xC..0x18 as ONE 16-byte struct assignment      1227 w / 192
         -- tried twice, at the 154 and the 114 baselines, on the theory that
         the target's two `addu' plus two `lw' is a block move with cse
         folding half its loads.  192 and 234.  It is NOT a block move.
       one `w' shared by case 0 and case 1             1224 w / 57 (vs 44)
         -- s55's fact (4) HOLDS verbatim: D_800814A8 needs a SEPARATE
         pointer local per arm.
       case 1's +0x2A read through `w2'                1222 w / 77 (vs 44)
         -- it must be the inline `*(u16 *)((u8 *)D_800814A8 + 0x2A)', which
         is what makes the target reload D_800814A8 a second time.  This is
         the other half of s55's fact (4) and it also HOLDS.
       case 1's +0x2A value not hoisted into a local   1225 w / 114 (vs 106)
       `u8 *d = dd.unk_00' as a function-scope local    1231 w / 371
         -- func_s67_80024C50 in this same unit NEEDS that local; here it
         spans the whole body, takes the last callee-saved register and
         SPILLS parameter 3 to its incoming slot at sp+0x68.  `d' must be
         assigned INSIDE blocks F and G instead.  THIS IS AN INVERSION of a
         landed in-unit donor's spelling, and the brief's rule predicted it:
         80024C50 has a -0x50 frame with six saves, this has -0x60 with nine.
       `*(u16 *)(D_80083780 + 0x2)' without a pointer local
         -- combine materialises the base as `symbol + 2' with displacements
         0/4/8 where the target has the plain symbol and 2/6/A.  Four
         separate pointer locals, one per site, are required.  Same disease
         on `*(u8 **)(D_80082E80 + 0x8)', where it additionally costs an
         `addiu $a1, $a1, -0x8' to recover the base for argument 2.

     MEASURED AND FREE -- do NOT build on these as facts:
       `((s32 *)w)[0x3D] = 0' for `*(s32 *)(w + 0xF4) = 0'    44 either way
       0xF4 store before or after the 0x96 store              44 either way
       x4 as a second D_80083780 pointer local                free
       x3 assigned twice                                      free

   THE FOUR THINGS THE LISTING LIES ABOUT ON THIS BODY, for the next reader:
     1. The 0x1C..0x28 store order is the scheduler's, not the source's.
     2. `addiu $s1, $zero, 0x4' in a branch delay slot 430 words before its
        only use is a long-lived local holding 4 (`j'), not a materialised
        immediate; the gate at 0x800254F8 is `*(s16 *)(p + 0x32) == j'.
     3. An emitted `sll $a2, $s4, 1' shared by three `mult's means the source
        has `i << 1' in a LOCAL; written inline the shift is distributed into
        each multiplicand and the whole register file rotates.
     4. `addu $a1, $s3, $a1' versus `addu $a1, $a1, $s3' is pointer-first
        order and follows the same `<< 2' / `* 4' rule
        func_s67_80024C50's comment records.

DECLARATIONS
typedef struct UnkDngS67Name12
{
    u8 b[0xC];
} UnkDngS67Name12;
extern const u8 D_s67_80024054[];
extern u8 D_80026180[];
extern u8 D_8002618C[];
extern u8 D_80026198[];
extern u32 D_800814A8;
extern u8 D_80082E80[];
extern s16 D_80082E86;
extern u16 D_80082E94;
extern Unk80083462 D_80083460;
extern u8 D_80083780[];
extern u8 D_800DDC40[];
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_8004491C(UnkNode *, u32);
s32 func_80053EF0(s32);
s32 func_800A56E0(s32);
END DECLARATIONS

   The body also calls func_s67_80024564, func_s67_800245B8,
   func_s67_80024834, func_s67_80024918, func_s67_80024B3C,
   func_s67_80024BD0, func_s67_80024C50 and func_s67_800246F0, all of which
   are DEFINED ABOVE IT in csrc/ovl_dng_s67.c, so they need no declaration
   there; `rand' and `func_8003FC64' come from the unit's existing block. */
void func_s67_80024E54(u8 *p, s32 a, s32 b)
{
    UnkDngS67Dirs dd;
    u8 *x0;
    u8 *x1;
    u8 *x2;
    u8 *x3;
    u8 *rw;
    u8 *rw2;
    u8 *d;
    u8 *e;
    u8 *y0;
    u8 *y1;
    u8 *y2;
    UnkNode *n;
    UnkSlot *r;
    u8 *q;
    u8 *m;
    u8 *w;
    u8 *w2;
    u8 *h;
    u8 *g;
    s32 i;
    s32 k;
    s32 j;
    u16 t;
    s32 u;
    s32 kk;
    s32 kj;
    s32 xx;
    s32 yy;
    s32 zz;
    s32 v2;
    s32 dv;
    s32 dv2;

    dd = *(UnkDngS67Dirs *)D_s67_80024054;
    *(s16 *)(p + 0x3A) = *(u16 *)(p + 0x3A) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        w = (u8 *)D_800814A8;
        *(s32 *)(w + 0xF4) = 0;
        *(s16 *)(w + 0x96) = 0x14;
        D_80082E86 = 6;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        x0 = D_80083780;
        *(s16 *)(p + 0x84) = *(u16 *)(x0 + 0x2);
        *(s16 *)(p + 0x86) = *(u16 *)(x0 + 0x6);
        D_800261B0 = 1;
        *(s16 *)(p + 0x88) = *(u16 *)(x0 + 0xA);
        /* fall through */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        w2 = (u8 *)D_800814A8;
        *(s16 *)(p + 0x32) = 0x21;
        *(s16 *)(w2 + 0xA6) = *(u16 *)(w2 + 0xA6) - 1;
        w2[0xA8] = p[0x8];
        v2 = (*(u16 *)((u8 *)D_800814A8 + 0x2A) >> 9) & 7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x3A) = 0;
        *(s16 *)(p + 0x36) = v2;
        break;
    case 2:
        t = *(u16 *)(p + 0x32) - 1;
        *(s16 *)(p + 0x32) = t;
        if ((s16)t <= 0)
        {
            *(s16 *)(p + 0x32) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            break;
        }
        if ((s16)t == 0x12)
        {
            if (func_80053EF0(4) != 2)
            {
                func_800A56E0(0x300);
            }
            else
            {
                func_800A56E0(0x4300);
            }
        }
        if (*(s16 *)(p + 0x32) == 0x18)
        {
            y0 = D_80082E80;
            if (func_8003DF74(*(u8 **)(y0 + 0x8), (UnkSlot *)y0, (s16 *)(p + 0x2C), 0) !=
                NULL)
            {
                for (i = 0; i < 10; i++)
                {
                    n = func_8003FC64(0x212);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        *(s16 *)(q + 0x3A) = 0x14;
                        *(s16 *)(q + 0x3C) = 0x14;
                        *(s16 *)(q + 0x40) = (rand() & 0xFF) + 0xC8;
                        n->unk_10 = (u32)func_s67_80024B3C;
                        func_8004491C(n, (u32)D_80045340);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_10 = 0x20;
                        r->unk_14 |= 0xC;
                        x1 = D_80083780;
                        m = (u8 *)n->unk_08;
                        *(s16 *)(m + 0x2) = *(u16 *)(p + 0x2C) + *(u16 *)(x1 + 0x2);
                        *(s16 *)(m + 0x6) = *(u16 *)(p + 0x2E) + *(u16 *)(x1 + 0x6);
                        *(s16 *)(m + 0xA) = *(u16 *)(p + 0x30) + *(u16 *)(x1 + 0xA);
                        r = (UnkSlot *)n->unk_0C;
                        r->unk_1E = 0x1000;
                        r->unk_1C = 0x1000;
                        r->unk_0C[2] = 0x80;
                        r->unk_0C[1] = 0x80;
                        r->unk_0C[0] = 0x80;
                        r->unk_16 = (rand() & 0xF) << 8;
                        r->unk_18 = (rand() & 0xF) << 8;
                        r->unk_1A = (rand() & 0xF) << 8;
                        *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_80026180;
                        r->unk_08 = (u32)((u8 *)n + 0xAA);
                    }
                }
            }
        }
        if (*(s16 *)(p + 0x32) == 0x18)
        {
            y1 = D_80082E80;
            if (func_8003DF74(*(u8 **)(y1 + 0x8), (UnkSlot *)y1, (s16 *)(p + 0x2C), 0) !=
                NULL)
            {
                n = func_8003FC64(0x212);
                if (n != NULL)
                {
                    q = (u8 *)n + 0x20;
                    *(s16 *)(q + 0x3A) = 0x14;
                    *(s16 *)(q + 0x3C) = 0x14;
                    n->unk_10 = (u32)func_s67_80024BD0;
                    func_8004491C(n, (u32)D_80045340);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_10 = 0x60;
                    r->unk_14 |= 0xC;
                    x2 = D_80083780;
                    m = (u8 *)n->unk_08;
                    *(s16 *)(m + 0x2) = *(u16 *)(p + 0x2C) + *(u16 *)(x2 + 0x2);
                    *(s16 *)(m + 0x6) = *(u16 *)(p + 0x2E) + *(u16 *)(x2 + 0x6);
                    *(s16 *)(m + 0xA) = *(u16 *)(p + 0x30) + *(u16 *)(x2 + 0xA);
                    r = (UnkSlot *)n->unk_0C;
                    r->unk_1E = 0x1000;
                    r->unk_1C = 0x1000;
                    r->unk_0C[2] = 0x80;
                    r->unk_0C[1] = 0x80;
                    r->unk_0C[0] = 0x80;
                    *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                    r->unk_08 = (u32)((u8 *)n + 0xAA);
                }
            }
        }
        if (*(s16 *)(p + 0x32) < 0xA)
        {
            if (*(s16 *)(p + 0x32) < 6)
            {
                if (*(s16 *)(p + 0x32) != 3)
                {
                    k = *(s16 *)(p + 0x32) == 4;
                    j = 4;
                    if (*(s16 *)(p + 0x32) == 2)
                    {
                        k = 2;
                    }
                    if (*(s16 *)(p + 0x32) == 1)
                    {
                        k = 3;
                    }
                    y2 = D_80082E80;
                    if (func_8003DF74(*(u8 **)(y2 + 0x8), (UnkSlot *)y2, (s16 *)(p + 0x2C), 0) !=
                        NULL)
                    {
                        kk = k * 2;
                        kj = kk + 1;
                        x3 = D_80083780;
                        rw = p + k * 12;
                        *(s16 *)(rw + 0x54) =
                            (s16) * (u16 *)(p + 0x2C) / 2 +
                            (*(s16 *)(x3 + 0x2) - *(s16 *)(p + 0x84)) / 2;
                        *(s16 *)(rw + 0x56) =
                            (s16) * (u16 *)(p + 0x2E) / 2 +
                            (*(s16 *)(x3 + 0x6) - *(s16 *)(p + 0x86)) / 2;
                        *(s16 *)(rw + 0x58) =
                            (s16) * (u16 *)(p + 0x30) / 2 +
                            (*(s16 *)(x3 + 0xA) - *(s16 *)(p + 0x88)) / 2;
                        rw2 = p + kj * 6;
                        *(s16 *)(rw2 + 0x54) =
                            *(s16 *)(p + 0x2C) / 4 +
                            (*(s16 *)(x3 + 0x2) - *(s16 *)(p + 0x84)) / 2;
                        *(s16 *)(rw2 + 0x56) =
                            *(s16 *)(p + 0x2E) / 4 +
                            (*(s16 *)(x3 + 0x6) - *(s16 *)(p + 0x86)) / 2;
                        *(s16 *)(rw2 + 0x58) =
                            *(s16 *)(p + 0x30) / 4 +
                            (*(s16 *)(x3 + 0xA) - *(s16 *)(p + 0x88)) / 2;
                        if (*(s16 *)(p + 0x32) == j)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x3A) = 0x14;
                                *(s16 *)(q + 0x3C) = 0x14;
                                n->unk_10 = (u32)func_s67_80024834;
                                func_8004491C(n, (u32)func_s67_80024564);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x60);
                                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x62);
                                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x64);
                                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x54);
                                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x56);
                                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x58);
                                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x66);
                                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x68);
                                *(s16 *)(q + 0x64) = *(u16 *)(p + 0x6A);
                                *(s16 *)(q + 0x66) = *(u16 *)(p + 0x5A);
                                *(s16 *)(q + 0x68) = *(u16 *)(p + 0x5C);
                                *(s16 *)(q + 0x6A) = *(u16 *)(p + 0x5E);
                                m = (u8 *)n->unk_08;
                                *(s16 *)(m + 0x2) = *(u16 *)(p + 0x84);
                                *(s16 *)(m + 0x6) = *(u16 *)(p + 0x86);
                                *(s16 *)(m + 0xA) = *(u16 *)(p + 0x88);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                r->unk_0C[2] = 0x80;
                                r->unk_0C[1] = 0x80;
                                r->unk_0C[0] = 0x80;
                                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                                r->unk_08 = (u32)((u8 *)n + 0xAA);
                            }
                        }
                        if (*(s16 *)(p + 0x32) == 2)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x3A) = 0x14;
                                *(s16 *)(q + 0x3C) = 0x14;
                                *(s32 *)(q + 0x1C) = 0x804020;
                                *(s32 *)(q + 0x20) = 0x804020;
                                *(s32 *)(q + 0x24) = 0x201008;
                                *(s32 *)(q + 0x28) = 0x201008;
                                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                                n->unk_10 = (u32)func_s67_80024834;
                                func_8004491C(n, (u32)func_s67_80024564);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x70);
                                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x60);
                                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x62);
                                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x64);
                                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x72);
                                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x74);
                                *(s16 *)(q + 0x64) = *(u16 *)(p + 0x76);
                                *(s16 *)(q + 0x66) = *(u16 *)(p + 0x66);
                                *(s16 *)(q + 0x68) = *(u16 *)(p + 0x68);
                                *(s16 *)(q + 0x6A) = *(u16 *)(p + 0x6A);
                                m = (u8 *)n->unk_08;
                                *(s16 *)(m + 0x2) = *(u16 *)(p + 0x84);
                                *(s16 *)(m + 0x6) = *(u16 *)(p + 0x86);
                                *(s16 *)(m + 0xA) = *(u16 *)(p + 0x88);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                r->unk_0C[2] = 0x80;
                                r->unk_0C[1] = 0x80;
                                r->unk_0C[0] = 0x80;
                                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                                r->unk_08 = (u32)((u8 *)n + 0xAA);
                            }
                        }
                        if (*(s16 *)(p + 0x32) == 1)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x3A) = 0x14;
                                *(s16 *)(q + 0x3C) = 0x14;
                                *(s32 *)(q + 0x1C) = 0x804020;
                                *(s32 *)(q + 0x20) = 0x804020;
                                *(s32 *)(q + 0x24) = 0x201008;
                                *(s32 *)(q + 0x28) = 0x201008;
                                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                                n->unk_10 = (u32)func_s67_80024834;
                                func_8004491C(n, (u32)func_s67_80024564);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x78);
                                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x7A);
                                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x7C);
                                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x70);
                                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x7E);
                                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x80);
                                *(s16 *)(q + 0x64) = *(u16 *)(p + 0x82);
                                *(s16 *)(q + 0x66) = *(u16 *)(p + 0x72);
                                *(s16 *)(q + 0x68) = *(u16 *)(p + 0x74);
                                *(s16 *)(q + 0x6A) = *(u16 *)(p + 0x76);
                                m = (u8 *)n->unk_08;
                                *(s16 *)(m + 0x2) = *(u16 *)(p + 0x84);
                                *(s16 *)(m + 0x6) = *(u16 *)(p + 0x86);
                                *(s16 *)(m + 0xA) = *(u16 *)(p + 0x88);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                r->unk_0C[2] = 0x80;
                                r->unk_0C[1] = 0x80;
                                r->unk_0C[0] = 0x80;
                                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                                r->unk_08 = (u32)((u8 *)n + 0xAA);
                            }
                        }
                        if (*(s16 *)(p + 0x32) == 1)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x3A) = 0x14;
                                *(s16 *)(q + 0x3C) = 0x14;
                                *(s32 *)(q + 0x1C) = 0x10000;
                                *(s32 *)(q + 0x20) = 0x10000;
                                *(s32 *)(q + 0x24) = 0x804020;
                                *(s32 *)(q + 0x28) = 0x804020;
                                *(s16 *)(q + 0x3E) = 2;
                                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                                n->unk_10 = (u32)func_s67_80024918;
                                func_8004491C(n, (u32)func_s67_80024564);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x70);
                                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x60);
                                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x62);
                                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x64);
                                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x64) = *(u16 *)(p + 0x70);
                                *(s16 *)(q + 0x66) = *(u16 *)(p + 0x60);
                                *(s16 *)(q + 0x68) = *(u16 *)(p + 0x62);
                                *(s16 *)(q + 0x6A) = *(u16 *)(p + 0x64);
                                d = dd.unk_00;
                                dv = *(s16 *)(d + (*(s16 *)(p + 0x36) << 2));
                                *(s16 *)(q + 0x6C) = dv << 3;
                                *(s16 *)(q + 0x6E) = *(u16 *)(d + (*(s16 *)(p + 0x36) << 2) + 2) << 3;
                                *(s16 *)(q + 0x70) = 0;
                                dv2 = *(s16 *)(d + (*(s16 *)(p + 0x36) << 2));
                                *(s16 *)(q + 0x72) = dv2 << 3;
                                *(s16 *)(q + 0x74) = *(u16 *)(d + (*(s16 *)(p + 0x36) << 2) + 2) << 3;
                                *(s16 *)(q + 0x76) = 0;
                                m = (u8 *)n->unk_08;
                                *(s16 *)(m + 0x2) = *(u16 *)(p + 0x84);
                                *(s16 *)(m + 0x6) = *(u16 *)(p + 0x86);
                                *(s16 *)(m + 0xA) = *(u16 *)(p + 0x88);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                r->unk_0C[2] = 0x80;
                                r->unk_0C[1] = 0x80;
                                r->unk_0C[0] = 0x80;
                                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                                r->unk_08 = (u32)((u8 *)n + 0xAA);
                            }
                        }
                        if (*(s16 *)(p + 0x32) == 1)
                        {
                            n = func_8003FC64(0x212);
                            if (n != NULL)
                            {
                                q = (u8 *)n + 0x20;
                                *(s16 *)(q + 0x3A) = 0x14;
                                *(s16 *)(q + 0x3C) = 0x14;
                                *(s32 *)(q + 0x1C) = 0x10000;
                                *(s32 *)(q + 0x20) = 0x10000;
                                *(s32 *)(q + 0x24) = 0x804020;
                                *(s32 *)(q + 0x28) = 0x804020;
                                *(s16 *)(q + 0x3E) = 2;
                                *(s32 *)(q + 0xC) = *(s32 *)(q + 0x1C);
                                *(s32 *)(q + 0x10) = *(s32 *)(q + 0x20);
                                *(s32 *)(q + 0x14) = *(s32 *)(q + 0x24);
                                *(s32 *)(q + 0x18) = *(s32 *)(q + 0x28);
                                n->unk_10 = (u32)func_s67_80024918;
                                func_8004491C(n, (u32)func_s67_80024564);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_10 = 0x20;
                                r->unk_14 |= 0xC;
                                *(s16 *)(q + 0x54) = *(u16 *)(p + 0x78);
                                *(s16 *)(q + 0x56) = *(u16 *)(p + 0x7A);
                                *(s16 *)(q + 0x58) = *(u16 *)(p + 0x7C);
                                *(s16 *)(q + 0x5A) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x5C) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x5E) = *(u16 *)(p + 0x70);
                                *(s16 *)(q + 0x60) = *(u16 *)(p + 0x78);
                                *(s16 *)(q + 0x62) = *(u16 *)(p + 0x7A);
                                *(s16 *)(q + 0x64) = *(u16 *)(p + 0x7C);
                                *(s16 *)(q + 0x66) = *(u16 *)(p + 0x6C);
                                *(s16 *)(q + 0x68) = *(u16 *)(p + 0x6E);
                                *(s16 *)(q + 0x6A) = *(u16 *)(p + 0x70);
                                d = dd.unk_00;
                                dv = *(s16 *)(d + (*(s16 *)(p + 0x36) << 2));
                                *(s16 *)(q + 0x6C) = dv << 3;
                                *(s16 *)(q + 0x6E) = *(u16 *)(d + (*(s16 *)(p + 0x36) << 2) + 2) << 3;
                                *(s16 *)(q + 0x70) = 0;
                                dv2 = *(s16 *)(d + (*(s16 *)(p + 0x36) << 2));
                                *(s16 *)(q + 0x72) = dv2 << 3;
                                *(s16 *)(q + 0x74) = *(u16 *)(d + (*(s16 *)(p + 0x36) << 2) + 2) << 3;
                                *(s16 *)(q + 0x76) = 0;
                                m = (u8 *)n->unk_08;
                                *(s16 *)(m + 0x2) = *(u16 *)(p + 0x84);
                                *(s16 *)(m + 0x6) = *(u16 *)(p + 0x86);
                                *(s16 *)(m + 0xA) = *(u16 *)(p + 0x88);
                                r = (UnkSlot *)n->unk_0C;
                                r->unk_1E = 0x1000;
                                r->unk_1C = 0x1000;
                                r->unk_0C[2] = 0x80;
                                r->unk_0C[1] = 0x80;
                                r->unk_0C[0] = 0x80;
                                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_8002618C;
                                r->unk_08 = (u32)((u8 *)n + 0xAA);
                            }
                        }
                        if (*(s16 *)(p + 0x32) == 1)
                        {
                            for (i = 0; i < 0x10; i++)
                            {
                                u = i << 1;
                                xx = *(s16 *)(p + 0x78) * 2 +
                                     (*(s16 *)(p + 0x60) - *(s16 *)(p + 0x78)) * u / 16;
                                yy = *(s16 *)(p + 0x7A) * 2 +
                                     (*(s16 *)(p + 0x62) - *(s16 *)(p + 0x7A)) * u / 16;
                                zz = *(s16 *)(p + 0x7C) * 2 +
                                     (*(s16 *)(p + 0x64) - *(s16 *)(p + 0x7C)) * u / 16;
                                func_s67_80024C50(p, a, b, xx, yy, zz);
                            }
                        }
                    }
                }
            }
        }
        h = *(u8 **)((u8 *)D_800814A8 + 0x60);
        if (h != NULL && *(s16 *)(p + 0x32) == 1)
        {
            n = func_8003FC64(0x212);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x3A) = 0xD;
                *(s16 *)(q + 0x3C) = 0xD;
                n->unk_10 = (u32)func_s67_800245B8;
                func_8004491C(n, (u32)D_80045340);
                r = (UnkSlot *)n->unk_0C;
                r->unk_06 = 6;
                r->unk_14 &= 0xFFF3;
                e = *(u8 **)(h - 0x18);
                m = (u8 *)n->unk_08;
                *(s16 *)(m + 0x2) = *(u16 *)(e + 0x2);
                *(s16 *)(m + 0x6) = *(u16 *)(e + 0x6);
                *(s16 *)(m + 0xA) = *(u16 *)(e + 0xA) - (D_800DDC40[h[0x13]] >> 1);
                r = (UnkSlot *)n->unk_0C;
                r->unk_1E = 0x1000;
                r->unk_1C = 0x1000;
                r->unk_0C[2] = 0x80;
                r->unk_0C[1] = 0x80;
                r->unk_0C[0] = 0x80;
                *(UnkDngS67Name12 *)((u8 *)n + 0xAA) = *(UnkDngS67Name12 *)D_80026198;
                *(UnkDngS67Name12 *)((u8 *)n + 0xB6) = *(UnkDngS67Name12 *)(D_80026198 + 0xC);
                r->unk_08 = (u32)((u8 *)n + 0xAA);
            }
        }
        break;
    case 3:
        if ((D_80082E94 & 0x8000) != 0 ||
            (s16)(*(s16 *)(p + 0x32) = *(u16 *)(p + 0x32) - 1) < 0)
        {
            *(s16 *)(p + 0xA) = 4;
        }
        /* fall through */
    case 4:
        if (D_800261B0 == 0)
        {
            g = (u8 *)&D_80083460;
            *(s32 *)(g + 0xC) = 0;
            D_80082E86 = 0;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            D_800261B0 = 0;
        }
        break;
    }
}
