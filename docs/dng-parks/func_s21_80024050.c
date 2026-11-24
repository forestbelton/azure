/* PARK: func_s21_80024050, csrc/ovl_dng_s21.c.  agent-h38, 2026-11-23, batch 12.

   TARGET 471 words.  BEST BUILT 471 words, 61 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py).  ovlverify reports 265 masked / 293 raw and
   BOTH OF THOSE NUMBERS ARE MEANINGLESS HERE: one instruction sits one slot
   early, so every word after it compares against its neighbour.  Use
   aligndiff on this function, as the brief says for anything over 200 words.

   1 member, 471 words, no replication (tools/decomp/dngfam.py).

   THIS FUNCTION OWNS A JUMP TABLE.  jtbl_s21_80024008, 18 entries, and the
   body below writes it as a plain `switch (*(s16 *)(p + 0xA))' with cases
   0, 1, 2, 3, 4, 16, 17; entries 5..15 all point at the shared exit, i.e.
   `default'.  CHECK THE CONTAINER when this lands -- a clean ovlverify
   cannot see the table.

   NO TWIN.  tools/decomp/dngtwin.py --unit ovl_dng_s21 gives
   func_s20_80024064 at 0.928 and func_s19_80024050 at 0.843, and BOTH ARE
   STILL INCLUDE_ASM, as are all 22 members of the `func_8003DF74' caller
   family (ovl_dng_b27, s01, s02, s16..s21, s31..s33, s37..s42, s51, s58,
   s59, s65, s67 -- grep asm/nonmatchings for func_8003DF74).  xsibs at
   --ratio 0.70 --min-words 18 finds nothing for it; the callee-set
   intersection recipe over csrc/ovl_dungeon*.c and csrc/ovl_dng_*.c on
   func_800A45D8 / func_8003DF74 / func_800A56E0 / func_80065F90 /
   D_8006CCD8 / D_800DDC40 / D_8006CCE8 finds no file carrying four of them
   in one function.  So this body is written from the listing, and the shape
   is believed correct: the word count is exact and the residue is registers.

   WHAT IT IS.  The master step handler of the spell script.  `p' is the
   payload of its own node; `o = *(u8 **)p' is the CASTER's payload, `n' is
   the caster's node (`o - 0x20') and `m' the caster's mover.  Eighteen
   states: 0 initialises the renderable, the ring banks and the facing and
   falls into 1; 1 reads the caster's hand through func_8003DF74, mirrors its
   position into the mover, tags the node with D_80045340, picks the target
   tile off the caster's +0x60 target (or its own mover when there is none),
   seeds the tile-walk step count and the D_8006CCD8/E8 facing pair; 2
   integrates and clamps the mover's two horizontal velocities at +-0x200000,
   eases z, spins by func_800644B8, spawns a func_s21_8002495C marker a frame,
   and either arrives (advancing to state 16) or steps one tile when
   func_800A45D8 says the next tile is clear; 3 rings func_80065F90, spawns
   the func_s21_800262AC burst and stores it at p+0x2C; 4 fades the
   renderable by a quarter a frame and, once the burst's node raises its kill
   bit, applies func_s21_80026404 to the caster's target and kills the node;
   16 drifts and fades by a half until the red channel drops under 2;
   17 waits for D_80026472 to fall to zero and kills the node.

   THE RESIDUE IS 61 ROWS AND 54 OF THEM ARE REGISTER NAMES IN CALLER-SAVED
   REGISTERS.  Word count, instruction selection and -- except for the seven
   rows listed next -- instruction order all agree.  The seven non-register
   rows, by target word:

     36  / 54..57  the `addu $a3, $zero, $zero' that sets func_800B835C's
                   FOURTH ARGUMENT sits at target word 36, with the other
                   three, and in this body at word 57.  The cause is visible:
                   this body puts the full `&D_80026484' in $a3 (word 32,
                   `addiu $a3, $v1, 12') so $a3 cannot be the argument until
                   that value dies at word 55, while the target puts it in
                   $t1 and uses $t0..$t3 for all four case-0 temporaries.
                   4 rows.
     103 / 105    the target emits `sh $v0, 0x32($s0)' immediately after
                   `sh $v0, 0xA($s1)' and then takes a load-delay `nop'; this
                   body sinks the +0x32 store into that nop.  sched1 is
                   allowed to because `*(s16 *)(p + 0x32)' and
                   `**(u16 **)(p + 0x4)' are both constant offsets off `p'
                   and memrefs_conflict_p separates them.  2 rows.
     182          the `bgez' delay slot: the target has a `nop', this body
                   duplicates `addiu $v0, $a0, 1' into it, which it can only
                   do because `d' is in $a0 and the result in $v0 where the
                   target has both in $v0.  1 row.

   Everything else is a rename, and the renames are systematic: wherever the
   target holds a value in $v0 or $v1, this body holds it in $a0 or $a1.  The
   register histogram over the whole function is

       reg  this  target        reg  this  target
       v0    325   326          a3     5     2
       v1    114   121          t0     4     3
       a0     70    64          t1     4     3
       a1     27    22          t2     0     4
       a2     12    12          t3     0     4

   -- the same total pressure, one register's worth of displacement, and
   $s0..$s5 identical (p=$s0, v=$s1, c=$s3, o=$s2, n=$s4, m=$s5, all correct
   and never in dispute).

   WHERE THE DISPLACEMENT COMES FROM, read out of `-da' (tools/cdkgcc -da -O2
   -G0 -Iinclude -Ipsyq/include -Icsrc -S, dumps moved out of the repo).
   `probe.c.greg' for this body gives

     ;; 19 regs to allocate: 93 92 281 287 88 90 91 94 87 382 89 80 86 450 81 84 82 83 85
     ;; 90 conflicts: 80 81 84 86 90 2 3 29
     ;; 86 conflicts: 80 81 84 86 90 2 3 29
     ;; Register dispositions: ... 86 in 5  87 in 2  88 in 4  89 in 5
                                   90 in 4  91 in 2  92 in 4  93 in 2  94 in 3

   with `probe.c.lreg' giving each allocno's `used N times across M insns'.
   global.c's `allocno_compare' priority, floor_log2(refs) * refs /
   live_length, then sorts exactly as the dump prints:

       93 = gy (case 2)   10 refs /  7 insns  ->  4.29
       92 = gx (case 2)   10 refs /  9 insns  ->  3.33
       281, 287           5 refs / 4 insns    ->  2.50 each
       88  = a (clamp)    18 refs / 32 insns  ->  2.25
       90  = d (case 1)    6 refs /  6 insns  ->  2.00
       91                  6 refs /  8 insns  ->  1.50
       94  = z (case 1)    4 refs /  6 insns  ->  1.33
       87  = t (case 1)    3 refs /  3 insns  ->  1.00
       89  = b (clamp)     6 refs / 16 insns  ->  0.75
       86  = e (case 1)    5 refs / 23 insns  ->  0.43

   `d' (90) is allocated before `e' (86), both conflict with hard regs 2 and
   3, so `d' takes 4 ($a0) and `e' takes 5 ($a1).  THE TARGET HAS `d' IN $v0
   AND `e' IN $a0, i.e. `d' does NOT conflict with hard reg 2 there.  In this
   body the conflict is created at the `subu' that defines `d': the two byte
   loads it consumes have already been given $v0 and $v1 by local-alloc and
   are still live at the set, so `record_one_conflict' marks both.  In the
   target the `subu' writes the register of the dying `lb' -- `subu $v0, $v0,
   $v1' -- so no conflict is recorded.  That is one local-alloc
   `combine_regs' tie this body does not get, and the same pattern explains
   the clamp's `b' and case 1's `z'.

   WHAT I WOULD TRY NEXT.  Not statement order and not declaration order --
   both are exhausted below.  The thing to find is the spelling that lets
   local-alloc TIE a subtraction's destination to its dying source operand
   across a basic-block boundary, or a way to give `d' and `e' live ranges
   that do not both reach hard regs 2 and 3.  The zero-word `do { } while (0)'
   pin found for func_s21_8002522C in this same run IS effective here (it is
   what took this body from 81 rows to 61) but only on ordering, never on a
   rename: every pin that changed a register changed the word count too.

   TWO PINS ARE IN THE BODY AND BOTH ARE MEASURED.  `do { a += a >> 4; }
   while (0);' around EACH of the two velocity clamps.  Without them sched1
   hoists the shift pair above the `v->unk_00 += v->unk_0C' store, `b' is
   born before that store instead of in the `bgez' delay slot, and the clamp
   blocks come out in the wrong order:

     both clamps pinned                 (this body)   471 words,   61
     only the first clamp pinned                      471 words,   73
     only the second clamp pinned                     471 words,   73
     neither pinned                                   471 words,   81

   They are probably MACRO RESIDUE rather than hand-written braces: the two
   clamps are the same five-statement idiom written twice, which is what a
   `CLAMP_VELOCITY(v, field)' macro collapses to, and a macro body is exactly
   a `do { } while (0)'.

   EVERY OTHER PIN MEASURED, all `do { <stmt>; } while (0);' and all against
   the 61-row body:

     around `v->unk_00 += v->unk_0C;'                 472 words,   79
     around `a = v->unk_0C;'                          472 words,   79
     around `*(s16 *)(p + 0x32) = z;'                 472 words,   59
     around `e = *(u8 **)(o - 0x14);'                 472 words,   69
     around `arr[0] = 0x1000340;'                     472 words,   94
     around `arr[1] = 0x200020;'                      471 words,   72
     around `func_800B835C(D_80026478, arr, 1, 0);'   471 words,   91
     around `*(s16 *)(p + 0x16) = ...;'               472 words,   74
     around `*(s16 *)&D_80026478[0xC] = 0x1010;'      472 words,   73
     around `*(u32 *)&D_80026478[0x14] = 0;'          471 words,   72
     around `*(u32 *)(c + 0x8) = (u32)D_80026490;'    471 words,   77
     around `d = *(s8 *)(o + 0x72) - e[0x24];'        471 words,   61
     around `*(s16 *)(p + 0x14) = d + 1;'             471 words,   61
     around `d = -d;'                                 471 words,   61
     around `b = -b;' in both clamps                  471 words,   61
     around `b = a;' in both clamps                   471 words,   61

   THE SPELLINGS THAT ARE LOAD-BEARING AND ARE IN THE BODY.  Each was
   measured against the then-current body, so the deltas are relative, but
   each is the better of its pair:

   * `v->unk_00 += v->unk_0C; a = v->unk_0C;' -- TWO READS, not one read into
     a local.  `q = v->unk_0C; v->unk_00 += q; a = q;' is 472 words;
     `a = q + (q >> 4)' is 472 words / 109; `a = v->unk_0C;
     v->unk_00 += v->unk_0C;' is 471 / 83.  The two-read form is the only one
     that is 471 words AND keeps the target's `addu $a0, $v1, $zero' copy.
   * EVERY VALUE GETS ITS OWN LOCAL.  Sharing `a'/`b' between case 1 and
     case 2 is 97 rows against 81 for per-use locals (`z', `k1', `k2', `f',
     `gx', `gy', `w').  That was the single biggest step in the shape search.
   * `d = x - y; if (x == y) { d = z - w; }' -- the comparison is written on
     the two LOADS, not on `d'.  `if (d == 0)' gives `subu' then `bnez' and
     costs a word; combine folds the written form to the target's `bne $v0,
     $v1' with the `subu' in the delay slot.
   * `gx = *(s8 *)(o + 0x72) << 6; gx += 0x20;' -- TWO STATEMENTS, at both
     case-2 sites and both func_800A45D8 arguments.  In ONE expression
     combine knows the low six bits are zero, folds `+ 0x20' into `ori' and
     drops the `& 0xFFE0' entirely: 474 words.  A local written more than
     once blocks that substitution and gives the target's
     `lb' / `sll 6' / `addiu 0x20' / `andi 0xFFE0'.
   * `a = D_8006CCD8[i]; a <<= 3;' -- two statements again, for `lh' instead
     of `lhu': in one expression force_to_mode converts the sign_extend to a
     zero_extend.  It costs no word either way (2 rows against 6), so it is
     the weakest of these four and the one to drop first if it gets in the
     way; `D_8006CCD8[i] * 8' measures the same 61 as the shift.
   * `f = *(u16 *)(p + 0x18); D_80026472 = 0; *(s16 *)(p + 0x18) = f + 1;' --
     the exit block's counter read is SPLIT around the `D_80026472 = 0' store.
     `D_80026472 = 0;' first is 472 words and six stolen branch-delay slots
     (reorg lifts the `lui %hi' out of the exit block into every `beqz' that
     reaches it); the increment first is 472 words / 107.
   * `extern s16 D_80026472_i __asm__("D_80026472");' for case 17's test.
     The unit's own `extern u16 D_80026472' gives `lhu' where the target has
     `lh', and `(s16)D_80026472' does not recover it.
   * `u16 out[3]' for func_8003DF74's output buffer, with `(s16 *)out' at the
     call: the +0x4 halfword is read `lhu'.  `s16 out[3]' gives `lh'.

   MEASURED AND FREE, so NOT reasons this body is at 61:
     `((s16 *)p)[0x19] = z;' for the p+0x32 store                        61
     `v' declared `u8 *' with `*(s32 *)(v + 0xC)' casts throughout       61
     `*(s32 *)((u8 *)v + 0x0) += *(s32 *)((u8 *)v + 0xC);' (either or
         both of the clamp's two accesses as cast-derefs)                61
     `d += 1; *(s16 *)(p + 0x14) = d;' split in two                      61
     one shared local for `k1'/`k2'                                      61
     `(s16)(*(s16 *)(p + 0x32))' at the func_800A45D8 call               61
     ALL FOUR declaration orders tried: reversed, pointers last,
         `t'/`u'/`d' first, arrays last                                  61
     `w = (a > 0) ? 0x200000 : -0x200000; v->unk_0C = w;' vs the
         ternary assigned straight to the field  (the local is needed:
         inline it is 473 words)                                    61 / 473

   MEASURED AND REJECTED:
     `D_800269C8 = ...' before the state bump in case 2    467 words,   83
     `t' and `u' as ONE local for the +0x60 target                       81
     `t' as an expression instead of a local                             81

   raw 293 == the 265 masked words plus the function's relocations; with the
   stream one word out of step the raw count is not a reloc count here and
   `--raw' tells you nothing about this body.  The `%hi/%lo' pairs the body
   needs are on D_80026484 (shared with the `&D_80026478' argument via one
   `addiu ..., -0xC', which is the target's shape), D_80026490, D_80026470,
   D_80026474, D_80026476, D_800265C0, D_800265C4, D_80045340, D_800DDC40,
   D_8006CCD8 twice, D_8006CCE8 twice, D_800269C8, D_8008346C, D_80026472
   and D_800814A0.

   func_s21_8002495C, func_s21_80026404 and func_s21_800262AC are all defined
   in csrc/ovl_dng_s21.c -- the first two BELOW this function, so they need
   forward declarations; func_s21_800262AC's is already in the unit because
   this run landed it.  func_s21_80026404 is defined with TWO parameters and
   this function calls it with THREE (the third is the caster's payload in
   $a2), which is why the alias below exists; it leaves the landed definition
   alone.

   func_8003DF74's signature is copied verbatim from its definition at
   csrc/unk_113D0.c:128.  func_800A45D8's is copied from
   csrc/ovl_dungeon_8009F644.c:4133 (`u16' parameters, which is what absorbs
   the `& 0xFFE0').  func_800A56E0's is csrc/ovl_dungeon_decls.h:976 and
   func_80065F90's is csrc/unk_72A8.c:45.

DECLARATIONS
extern s16 D_80026470;
extern s16 D_80026474;
extern s16 D_80026476;
extern u32 D_800265C0;
extern u32 D_800265C4;
extern u8 D_80026478[];
extern u8 D_80026490[];
extern u16 D_800269C8;
extern s32 D_8008346C;
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80045340[];
extern s16 D_80026472_i __asm__("D_80026472");
void func_800B835C(u8 *, s32 *, s32, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_80065F90(s32, s32);
s16 func_800A45D8(u16, u16, s32);
s32 func_800A56E0(s32);
UnkNode *func_s21_8002495C(u8 *v, u8 *q);
UnkNode *func_s21_800262AC(s16 x, s16 y, s16 z);
void func_s21_80026404_3(u8 *, u8, u8 *) __asm__("func_s21_80026404");
END DECLARATIONS */
void func_s21_80024050(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    s32 arr[2];
    UnkNode *n;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    s32 a;
    s32 b;
    s32 d;
    s32 w;
    s32 gx;
    s32 gy;
    s32 z;
    s32 k1;
    s32 k2;
    s32 f;
    u8 *u;

    o = *(u8 **)p;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x555;
        *(s16 *)(c + 0x1C) = 0x555;
        *(u32 *)(c + 0x8) = (u32)D_80026490;
        D_80026470 = 0;
        D_80026474 = 0;
        D_80026476 = 0;
        D_800265C0 = 0;
        D_800265C4 = 0;
        *(s16 *)&D_80026478[0xC] = 0x1010;
        *(u32 *)&D_80026478[0x14] = 0;
        *(s16 *)(p + 0x16) = (*(u16 *)(o + 0x2A) >> 9) & 0x7;
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_80026478, arr, 1, 0);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out, 0) ==
            NULL)
        {
            if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
            {
                break;
            }
        }
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(m + 0x6);
        if (((UnkSlot *)n->unk_0C)->unk_14 & 0x8000)
        {
            z = *(u16 *)(m + 0xA) - 0x40;
        }
        else
        {
            z = *(u16 *)(m + 0xA) + out[2];
        }
        *(s16 *)((u8 *)v + 0xA) = z;
        *(s16 *)(p + 0x32) = z;
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x12] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x12] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0xC) = *(u16 *)(u + 0x2);
            *(s16 *)(p + 0xE) = *(u16 *)(u + 0x6);
            *(s16 *)(p + 0x10) = *(u16 *)(u + 0xA) - D_800DDC40[0];
            e = *(u8 **)(o - 0x14);
            p[0x20] = p[0x1E] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x16)];
            p[0x21] = p[0x1F] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x16)];
            d = *(s8 *)(o + 0x72) - e[0x24];
            if (*(s8 *)(o + 0x72) == e[0x24])
            {
                d = *(s8 *)(o + 0x73) - e[0x25];
            }
            if (d < 0)
            {
                d = -d;
            }
            *(s16 *)(p + 0x14) = d + 1;
        }
        else
        {
            *(s16 *)(p + 0x14) = 0x8;
            *(s16 *)(p + 0xC) = *(u16 *)((u8 *)v + 0x2);
            *(s16 *)(p + 0xE) = *(u16 *)((u8 *)v + 0x6);
            *(s16 *)(p + 0x10) = *(u16 *)(o + 0x88) - 0x50;
        }
        k1 = D_8006CCD8[*(s16 *)(p + 0x16)];
        k1 <<= 3;
        *(s16 *)((u8 *)v + 0xE) = k1;
        k2 = D_8006CCE8[*(s16 *)(p + 0x16)];
        k2 <<= 3;
        *(s16 *)((u8 *)v + 0x12) = k2;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        v->unk_00 += v->unk_0C;
        a = v->unk_0C;
        do
        {
            a += a >> 4;
        } while (0);
        b = a;
        if (b < 0)
        {
            b = -b;
        }
        v->unk_0C = a;
        if (b > 0x200000)
        {
            w = (a > 0) ? 0x200000 : -0x200000;
            v->unk_0C = w;
        }
        v->unk_04 += v->unk_10;
        a = v->unk_10;
        do
        {
            a += a >> 4;
        } while (0);
        b = a;
        if (b < 0)
        {
            b = -b;
        }
        v->unk_10 = a;
        if (b > 0x200000)
        {
            w = (a > 0) ? 0x200000 : -0x200000;
            v->unk_10 = w;
        }
        *(s32 *)(p + 0x30) += ((*(s16 *)(p + 0x10) << 16) - *(s32 *)(p + 0x30)) >> 4;
        v->unk_08 += ((*(s16 *)(p + 0x10) << 16) - v->unk_08) >> 4;
        v->unk_08 += func_800644B8(*(s16 *)(p + 0x18) << 7) << 6;
        func_s21_8002495C((u8 *)v, c);
        p[0x1E] = *(s16 *)((u8 *)v + 0x2) / 0x40;
        p[0x1F] = *(s16 *)((u8 *)v + 0x6) / 0x40;
        if (*(s16 *)(p + 0x20) == *(s16 *)(p + 0x1E))
        {
            break;
        }
        if (*(u8 **)(o + 0x60) != NULL &&
            (*(u32 *)(o + 0x70) & 0xFFFF0000) == (*(u32 *)(p + 0x1C) & 0xFFFF0000))
        {
            gx = *(s8 *)(o + 0x72) << 6;
            gx += 0x20;
            *(s16 *)((u8 *)v + 0x2) = gx;
            gy = *(s8 *)(o + 0x73) << 6;
            gy += 0x20;
            *(s16 *)((u8 *)v + 0x6) = gy;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x10);
            func_s21_8002495C((u8 *)v, c);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            D_800269C8 = *(u16 *)(*(u8 **)(o + 0x60) + 0x88);
            break;
        }
        p[0x20] = p[0x1E];
        p[0x21] = p[0x1F];
        if (--*(s16 *)(p + 0x14) != 0)
        {
            gx = *(s8 *)(p + 0x1E) << 6;
            gx += 0x20;
            gx &= 0xFFE0;
            gy = *(s8 *)(p + 0x1F) << 6;
            gy += 0x20;
            gy &= 0xFFE0;
            if (func_800A45D8(gx, gy, *(s16 *)(p + 0x32)) == 0)
            {
                break;
            }
        }
        *(s16 *)(p + 0xA) = 0x10;
        break;
    case 3:
        func_80065F90(*(s16 *)((u8 *)v + 0xE), *(s16 *)((u8 *)v + 0x12));
        *(UnkNode **)(p + 0x2C) = func_s21_800262AC(
            *(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6), *(s16 *)((u8 *)v + 0xA));
        if (*(UnkNode **)(p + 0x2C) == NULL)
        {
            break;
        }
        if (*(u8 **)(o + 0x60) != NULL)
        {
            func_800A56E0(0x300);
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 4:
        c[0xC] -= c[0xC] >> 2;
        c[0xD] -= c[0xD] >> 2;
        c[0xE] -= c[0xE] >> 2;
        if (((*(UnkNode **)(p + 0x2C))->unk_1E & 0x8000) == 0)
        {
            break;
        }
        if (*(u8 **)(o + 0x60) != NULL)
        {
            func_s21_80026404_3(*(u8 **)(o + 0x60), p[0x9], o);
        }
        D_8008346C = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    case 16:
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += ((*(s16 *)(p + 0x10) << 16) - v->unk_08) >> 4;
        c[0xC] -= c[0xC] >> 1;
        c[0xD] -= c[0xD] >> 1;
        c[0xE] -= c[0xE] >> 1;
        func_s21_8002495C((u8 *)v, c);
        if (c[0xC] < 2)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 17:
        if (D_80026472_i != 0)
        {
            break;
        }
        D_8008346C = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    f = *(u16 *)(p + 0x18);
    D_80026472 = 0;
    *(s16 *)(p + 0x18) = f + 1;
}
