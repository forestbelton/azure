/* PARKED: func_s15_80024CD4, ovl_dng_s15, 421 target words.
   BEST: 420 candidate words, 20 ALIGNED differences (aligndiff); ovlverify
   reports 412 differing position-by-position, which is the usual inflation
   from being one word short -- quote the aligned figure.

   dngfam.py: 1 member, 421 words. No other member, no other park.
   dngtwin: no carved sibling at any ratio. xsibs: 0 resident siblings at
   0.70. dngnear/NEAR.txt: 0 donors (ovl_dng_s15 has no landed in-unit
   neighbour to invert).

   HOW IT WAS FOUND, because that part worked and is worth reusing: the
   callee-set intersection grep over the seven distinctive symbols
   func_800BCB04, func_800A56E0, func_800A44E0, func_8003DE58, D_8006CCD8,
   D_8006CCE8 and D_8008346C put csrc/ovl_dng_s25.c at 7 of 7, and
   func_s25_800247D4 in that file is the line-for-line donor -- same 0x212
   actor state machine, same func_8003DE58 gate, same abs() triple into a
   stack array, same eight-step D_8006CCD8/D_8006CCE8 walk, same func_800BCB04
   ceiling tests, same D_8008346C teardown. The whole 421-word shape came out
   of that one grep. ovl_dng_s25 is also the unit holding the 0.906 twin of
   this unit's func_s15_800247DC, so it is the sibling bank to read next time
   anything in ovl_dng_s15 needs a donor.

   WHAT THIS BODY IS. The actor driver: bumps p+0x10, dispatches
   *(s16 *)(p + 0xA) through jtbl_s15_80024048 (6 cases, no default arm), and
   every arm falls out through a shared *(s16 *)(p + 0x14) = 0.

   THIS FUNCTION OWNS jtbl_s15_80024048. 6 .word entries = 6 cases + 0 pad,
   dispatch bound `sltiu $v0, $v1, 0x6', 6 DISTINCT targets, confirmed by
   jtcheck.py (which agrees with the listing). Entry order, which is the arm
   order this body is written in: case 0 .L80024D48, case 1 .L80024D78,
   case 2 .L8002521C, case 3 .L80025290, case 4 .L800252C4,
   case 5 .L800252FC. Case 0 FALLS THROUGH into case 1. The pad rule holds as
   stated in the brief: 6 cases is EVEN and the table is last in .rodata, so
   no pad word -- read off the bound, not the word count.

   WHAT THE RESIDUE IS. Four independent causes, every one of them a REGISTER
   AVAILABILITY difference with the instruction stream otherwise identical.
   Nothing is missing or extra except two words, named below.

   1. THE PROLOGUE, 3 rows and the missing word. The target copies the THIRD
      parameter out of its incoming register -- `addu $a1, $a2, $zero' in the
      beqz delay slot -- and stores 0x808080 through $a1. This body leaves it
      in $a2, so the copy is deleted, `addu $s5, $a1, $zero' sinks into the
      delay slot instead of sitting at word 4 beside its own `sw $s5', and the
      store reads `sw $v1, 0xC($a2)'. c is dead at the end of case 0 and
      nothing uses $a2 before that point, so no conflict explains the target's
      choice; local-alloc simply gave the pseudo $a1 there and $a2 here.
      Rejected: a `cc = c' local for the store, 20 rows, no change. A
      three-parameter signature, 20 rows, no change. Moving the c store to the
      top of case 0, 26 rows.

   2. `g' IN $a0 WHERE THE TARGET HAS $s0, 5 rows, plus 4 more from the
      knock-on. In the target $a0 is occupied across g's whole range by the
      high half of the /15 magic 0x88888889, which reorg has pulled all the
      way back into the `beqz' delay slot at word 112; the `ori' that completes
      it is at word 158. Here the magic's high half sits at word 158 next to
      its `ori', so $a0 is free over g's range, g takes it (find_reg ascends
      from $v0, and $a0 is regno 4 against $s0's 16), and reorg then has to
      fill the delay slot with `i = 0' instead. The target packs o, g, cx and v
      all into $s0.
      THE DIAGNOSTIC WORTH REUSING: reusing the `o' local for `g' DOES force
      $s0 and removes all 5 of those rows -- and the identical fault
      immediately reappears on `q', which then takes $a0 where the target has
      $v1, for 6 rows instead of 5 (21 total). So the cause is upstream of
      which local is involved: $a0 is simply free here and busy there.
      Rejected: `g' reusing `o' 21; `g' reusing `w' 30; `g' reusing `o' and
      `q' reusing `w' 25; `q' reusing `w' alone 20; declaring `g' first 20;
      `do { g = ...; } while (0)' 421 words / 25 rows -- that one does buy the
      missing word back, so if a later pass fixes causes 1 and 2 properly the
      word count is already accounted for; a `do { } while (0)' round the abs
      triple 421 words / 25; round the then-arm divisions 145; round the whole
      then-arm 158; `if (1)' round the divisions 20; `/ 0xF' spelled instead
      of `/ 15' 20.

   3. THE SECOND MISSING WORD IS A DEAD HOISTED `%hi'. The target carries a
      `lui $v0, 0x8007' in the else-arm preheader that nothing reads --
      0x8007 is `%hi(D_8006CCD8)' and `%hi(D_8006CCE8)', and splat renders it
      as a bare constant because the hoisted `high' has no paired `%lo' left
      to symbolise against. loop.c hoisted the invariant and the four uses
      inside the loop re-materialise their own `lui'/`addiu' pairs anyway. No
      spelling tried reproduced it. Two more rows sit beside it: the loop's
      `addiu $s7, $s7, 1' is scheduled at word 255 in the target and 266 here,
      and the `%lo(D_8006CCD8)' one slot earlier.

   4. CASE 2 TAKES $a1/$a2 FOR ITS TWO TEMPORARIES WHERE THE TARGET TAKES
      $a2/$a3, 4 rows. Same shape as cause 1: $a1 is free here and not there.
      Rejected: `+=' instead of `x = x + y' 20; plain casts off `(u8 *)b'
      instead of the UnkVec members 20.

   FACTS THAT ARE ALREADY SETTLED AND SHOULD NOT BE RE-SWEPT. All measured on
   this body, and the four that reached 0 for their own row are real.

     The func_800A44E0 cell masks must be `& 0xFFFFFFC0', not `& 0xFFC0'.
     This is the convert.c narrowing lever in the brief: into a `u16'
     parameter the short mask becomes HImode -64 and cc1psx emits
     `li $t0, -64' plus two `and' where the target has two `andi 0xFFC0'.
     Worth 4 rows and 3 words.

     `*(s16 *)((u8 *)v + 0x2) = ox * 64 + 0x20' must be a MULTIPLY, not
     `(ox << 6)'. The shift form lets convert.c narrow the whole expression
     because the LSHIFT_EXPR arm is guarded on TREE_UNSIGNED and MULT_EXPR's
     is not, so the `s16' sign extension vanishes: the target's
     `sll 16'/`sra 10' pair collapses to a bare `sll 6' and the body comes out
     2 words short with 3 extra rows. Same for `oy * 64'.

     TWO STATEMENT SWAPS, BOTH THE SAME MECHANISM, and this is the most
     transferable thing in this file. A byte or halfword store written AFTER a
     may-alias load is that load's SUCCESSOR, and sched2 then promotes it over
     the ALU insns that separate it from its own value -- which is how the
     target gets a store emitted between a load and the arithmetic that feeds
     the next store. Both of these are one line moved:
       case 0: `p[0x16] = 0' must come AFTER the +0xE statement that reads
       s+0x2A, not before it. Before it: 5 rows.
       case 1 then-arm: `*(s16 *)(p + 0x3A) = ...' must come BEFORE
       `*(s16 *)(p + 0x38) = 0', so that the 0x60/0x88 load precedes the 0x38
       store. The other way the 0x38 store fills a load-delay slot the target
       leaves as a `nop': 2 rows.

     THE 16.16 `oy' MUST OUTRANK `ox' AND THIS BODY NEEDS A DEVICE FOR IT,
     48 rows down to 20 -- the single largest fact here. The target keeps `oy'
     in $fp and SPILLS `ox' to 0x18($sp); written plainly this body does the
     reverse, and because the two are perfectly symmetric -- 5 weighted
     references each, with `ox' assigned second and used first so its live
     range is strictly the shorter -- `ox' wins `allocno_compare' every time.
     `do { oy = yy; } while (0)' inside the walk is what reverses it, and
     flow.c:2122 counting a reference once per enclosing loop is the
     mechanism. Rejected: all four combinations of the `ox'/`oy' declaration
     order and of their assignment order, 48 or 107 -- 107 because assigning
     `ox' first in the preheader rotates b off $s5 and s off $s6 through the
     whole body, so that direction is not a near miss but a different
     allocation. Spelling `ox' as a fifth element of the `out' array does
     force it to memory and reaches 45 rows without a device, but then the
     load-plus-cast combines into `lh' where the target has `lhu' with a
     separate sign extension, so it is the wrong shape. I would call the
     `do { } while (0)' MACRO RESIDUE rather than a device: the two
     assignments it brackets are the tail of a coherent step-the-cursor
     block, which is exactly what a macro body looks like. It is the one
     device in this body and it is load-bearing.

     `yy = q[0x25]' must be written BEFORE `xx = q[0x24]', which is what puts
     xx in $s3 and yy in $s4; the other order is 78 rows. The two `lbu'
     emit in the opposite order from the source because they share a base
     register and sched2 is free to swap them, so do not read the order off
     the listing.

     Locals confirmed against the frame: `addiu $sp, $sp, -0x48' with saves
     s0 s1 s2 s3 s4 s5 s6 s7 fp ra, so 9 saves minus 2 callee-saved parameters
     is 7 long-lived locals, and the target has exactly 7 -- s, the $s0 pack
     (o/g/cx/v), cy, xx, yy, i and oy, with ox spilled and out[4] on the
     stack at 0x10($sp).

     The three divisions really are by the literal 15: the
     `lui 0x8888'/`ori 0x8889'/`mult'/`mfhi'/`addu'/`sra 3'/`sra 31'/`subu'
     sequence is expand_divmod's magic multiply, so no variable divisor is
     involved anywhere in this body.

   WHAT I WOULD DO NEXT. All four remaining causes are the same question --
   why $a0 and $a1 are free in this build and busy in the target -- and none
   of the twenty-odd source-level spellings above moves it. The next step is
   `-da' on this body and reading `.greg's allocation order for the /15 magic
   pseudo against `g', since the whole chain starts with where that constant's
   `high' insn sits, or a permuter run, which is yours.

   DECLARATIONS
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   extern u32 D_800814A0;
   s32 func_800A44E0(u16, u16, s16, u32);
   s32 func_800BCB04(s32, s32, s16);
   s32 func_800A56E0(s32);
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   s32 func_s15_800248C8(u8 *);
   void func_s15_800247DC(u8 *, u8 *);
   void func_s15_80024684(u8 *, u8 *);
   void func_s15_80024060(s16 x, s16 y, u8 *c, u8 d);
   END DECLARATIONS
*/
void func_s15_80024CD4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *g;
    u8 *q;
    UnkVec *v;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 yy;
    s16 xx;
    s16 ox;
    s16 oy;
    u16 z;
    u16 out[4];

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        p[0x16] = 0;
        *(s32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        o = s - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        w = *(u8 **)(o + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        z = *(u16 *)(w + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        *(UnkVec *)(p + 0x18) = *b;
        if (*(u8 **)(s + 0x60) != NULL)
        {
            g = *(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)(g + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)(g + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            q = *(u8 **)(*(u8 **)(s + 0x60) - 0x14);
            *(s16 *)(p + 0x48) = q[0x24];
            *(s16 *)(p + 0x4A) = q[0x25];
            *(s32 *)(p + 0x30) = *(s32 *)g;
            *(s32 *)(p + 0x34) = *(s32 *)(g + 0x4);
            *(s16 *)(p + 0x3A) = *(u16 *)(*(u8 **)(s + 0x60) + 0x88);
            *(s16 *)(p + 0x38) = 0;
            b->unk_0C = (*(s32 *)(p + 0x30) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x34) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x38) - b->unk_08) / 15;
        }
        else
        {
            q = *(u8 **)(o + 0xC);
            yy = q[0x25];
            xx = q[0x24];
            oy = yy;
            ox = xx;
            for (i = 0; i < 8; i++)
            {
                cx = xx;
                cy = yy;
                if ((s16)func_800A44E0((cx << 6) & 0xFFFFFFC0, (cy << 6) & 0xFFFFFFC0,
                                       *(s16 *)(s + 0x88),
                                       (s16)(*(s16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((cx + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((cy + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(s + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(s + 0x88)) < -0x3F)
                {
                    break;
                }
                xx = xx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                yy = yy + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                do
                {
                    oy = yy;
                } while (0);
                ox = xx;
            }
            v = (UnkVec *)(p + 0x30);
            v->unk_08 = 0;
            v->unk_04 = 0;
            v->unk_00 = 0;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + 0x20;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + 0x20;
            *(s16 *)((u8 *)v + 0xA) = -0x400;
            *(s16 *)((u8 *)v + 0xA) = func_800BCB04(*(u16 *)((u8 *)v + 0x2),
                                                    *(u16 *)((u8 *)v + 0x6),
                                                    *(s16 *)((u8 *)v + 0xA));
            if (*(s16 *)((u8 *)v + 0xA) >= 0x201)
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            }
            b->unk_0C = (*(s32 *)(p + 0x30) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x34) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x38) - b->unk_08) / 15;
            *(s16 *)(p + 0x48) = ox;
            *(s16 *)(p + 0x4A) = oy;
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        b->unk_00 = b->unk_00 + b->unk_0C;
        b->unk_04 = b->unk_04 + b->unk_10;
        b->unk_08 = b->unk_08 + b->unk_14;
        func_s15_800248C8(p);
        if (*(s16 *)(p + 0x10) >= 0xF)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            func_s15_800247DC(p, (u8 *)b);
            func_800A56E0(0x300);
        }
        break;
    case 3:
        if (p[0x16] != 0)
        {
            *(s16 *)(p + 0x10) = 0;
            p[0x16] = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            func_s15_80024684(p, (u8 *)b);
        }
        break;
    case 4:
        if (p[0x16] != 0)
        {
            func_s15_80024060(*(s16 *)(p + 0x48), *(s16 *)(p + 0x4A), s, p[0x9]);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x14) == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
