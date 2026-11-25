/* PARKED at 492 target words / 492 candidate words / 7 ALIGNED DIFFERENCES.
   ovl_dng_b29, 1 member (dngfam.py), so landing it lands one body -- but it
   also OWNS TWO JUMP TABLES, jtbl_b29_80170850 (8 words = 7 cases + 1 pad,
   dispatch bound 7, states 0..6, all seven real arms) and jtbl_b29_80170870
   (7 words = 7 cases + 0 pad, bound 7, dispatch on
   ((d + 0x46) & 0x3FFF) - 1 so the case set is 1..7, with entry index 3
   (case 4) carrying the DEFAULT label -- the range-check `beqz' targets
   .Lb29_801730DC, which is that entry).  Together with the landed
   func_b29_80171094 these four tables are the whole of this unit's first
   .rodata subsegment, so this park is what still keeps that subsegment in
   assembly.

   WHAT IT IS: the actor's +0x9B seven-state machine for the three-slot carry.
   State 0 picks the slot triple out of (d + 0x46) & 0x3FFF and hands it to
   func_800A94A0; state 1 waits on func_8003F270; states 2..6 run a +0xAE
   countdown each with its own payload.

   SOURCE: the landed func_b21_80175480 in csrc/ovl_dng_b21.c (286 words),
   found through CALLEE.txt at 13-of-19 symbols, not through dngtwin (whose
   best row for this body is func_b55_8017256C at 0.641).  Same frame class:
   six callee-saved registers plus ra, -0x40 here against b21's -0x38 because
   this body has an `s16 out[4]' at sp+0x18.  Every one of b21's/b03's stated
   facts HELD unchanged:
     - the two sign fixes are `__builtin_abs', not two `if (x < 0) x = -x;'
       (the `sw $v0, 0x60($s3)' keeps its own word here, exactly as b21 says
       it does with __builtin_abs);
     - `k' is `s16';
     - the inner compare-chain arms are written 3, 2, 1 (descending);
     - the D_8006DE24 record through `tbl'/`i'/`e' locals.

   DELTAS FROM b21, all read off this listing: four more states (2..6); the
   0xAE countdown; D_80083160 reached through an Unk80083160 POINTER LOCAL
   whose unk_C8 is the facing index (the SAME ADDRESS as D_80083228, which
   state 6 spells directly -- two different source expressions for one
   address, and the listing proves it: state 2 emits
   `lui/addiu $s0, %lo(D_80083160)' once and indexes +0xC8 off it twice,
   state 6 emits `lui $v0, %hi(D_80083228)'); the bank-local tables
   D_80176490 / D_80176498 / D_80176460; and the func_8003DE58 projection
   block with `s16 out[4]' zeroed DESCENDING.

   FIVE MEASURED FACTS, each priced on this body:

   1. THE D_80176490 FACING TABLE MUST BE NAMED DIRECTLY AT BOTH SITES, not
      held in a `u8 *f' local.  With the local it is 492 words / 26 aligned
      differences and the whole 0xBFFFFFFF/table block re-schedules; named
      twice it is 8.  That is the OPPOSITE of b25's and b41's "one table
      local per dress site", so the fact does not carry from them -- which
      the brief predicts, because those bodies have a different frame.

   2. `*(u16 *)(c + 0x14) |= 0xC;' MUST BE WRITTEN BEFORE
      `*(u16 *)(c + 0x10) |= 0x20;' in case 5, although the listing emits
      the 0x10 store first: source order 0x10-then-0x14 is 155 aligned
      differences against 145 for 0x14-then-0x10 on the same baseline.
      Delay-slot/load scheduling inverts the apparent order.

   3. THE c + 0x28 CHAIN IS FOUR LOADS DEEP, not three:
      ((u8 **)*(u8 **)*(u8 **)*(u8 **)(c + 0x28))[D_80176498[...]].  Three
      derefs is 174 differences and a missing `lw'.

   4. `out[2]' MUST GO THROUGH AN s16 LOCAL (`z') before the p + 0xB2 store,
      because the store to p + 0xB2 forces a reload of out[2] for the
      p + 0x92 divide otherwise: 155 differences with out[2] read twice.

   5. `n->unk_1E |= 0x8000' MUST BE THE UnkNode STRUCT FIELD.  As
      `*(u16 *)((u8 *)n + 0x1E) |= 0x8000' it is 494 words / 19 -- two words
      LONGER, both load-delay `nop's, because MEM_IN_STRUCT_P on that load
      is what lets sched1 interleave it with the D_800814A0 chain
      (sched.c:817 anti_dependence's fixed-scalar-versus-varying-struct
      escape).  This is the same clause the brief records for the b + 0x14
      case, used in the opposite direction.

   THE ONE DEVICE, AND IT IS A REFERENCE BUMP I COULD NOT SPELL AWAY.
   Without it the body is the right 492 words with 132 aligned differences:
   `p' and `d' exchange $s2/$s3 from the prologue down, with every
   instruction in place -- a WRONG PRIORITY, not a wrong pseudo count.
   From `-da' on this unit, global.c's allocno_compare
   (floor_log2(n_refs) * n_refs / live_length * 10000) gives

       c (reg 82)   51 refs / 622 insns   ->  4099
       d (reg 83)   51 refs / 630 insns   ->  4047
       p (reg 80)   62 refs / 778 insns   ->  3984

   so the sort is c, d, p and `d' takes $s2 before `p' can.  `p' needs one
   more reference to reach 5*63/778 = 4048, which is above `d' and still
   below `c', and flow.c:2122 counts a reference once per ENCLOSING LOOP --
   so a `do { } while (0)' round a single-p-reference statement buys exactly
   that one reference and emits no instruction.  Two scopes measured, both
   492 words / 7 differences: round `*(s32 *)(p + 0xA0) = 0;' (kept, since
   the whole 0xA0 release reads as one macro) and round
   `*(s16 *)(p + 0xB2) = z;'.  Scopes that OVERSHOOT, because they carry
   two p references or three: round `*(u16 *)(p + 0x98) |= 0x80;' 133,
   round the case-2 `*(s16 *)(p + 0xAE) = 8; p[0x9B] += 1;' pair 495 words /
   125, round the case-2 A0 publish 131, round case 6's handler install 493
   words / 18, round case 4's `*(s16 *)(p + 0xAE) = 9;' 494 words / 36.
   Macro residue rather than a device: the released-node block is one
   coherent unit of work.

   THE REMAINING 7 DIFFERENCES ARE ALL ONE THING and I could not reach it.
   In case 6's released-node block the target schedules the D_800814A0 chain
   FIRST and this body schedules the n + 0x1E chain first, so dbr fills the
   `beqz $v0, .Lb29_80173624' delay slot with `lui $a0, %hi(D_800814A0)'
   where this body puts `addu $a1, $v0, $zero' (the `n' copy) there, and the
   two chains' `ori'/store pairs then interleave the other way round:

       target                      this body
       397 lui  $a0, %hi(D)  [ds]  397 move a1, v0        [ds]
       398 addu $a1, $v0, 0        398 lui  v0, %hi(D)
       399 lw   $v0, %lo(D)($a0)   399 sh   zero, 0xAC(s2)
       400 sh   zero, 0xAC($s2)    400 sw   zero, 0xA4(s2)
       401 sw   zero, 0xA4($s2)    401 sw   zero, 0x90(s2)
       402 sw   zero, 0x90($s2)    402 lhu  v1, 0x1E(a1)
       403 lhu  $v1, 0x1E($a1)     403 lw   a0, 0(v0)
       404 ori  $v0, $v0, 0x8000   404 ori  v1, v1, 0x8000
       405 ori  $v1, $v1, 0x8000   405 ori  a0, a0, 0x8000
       406 sw   $v0, %lo(D)($a0)   406 sh   v1, 0x1E(a1)
       407 sh   $v1, 0x1E($a1)     407 sw   a0, 0(v0)

   It is a sched2 tie: both chains have the same critical path, so
   rank_for_schedule falls through to INSN_LUID, i.e. to which statement's
   RTL comes first -- and the `n' copy's insn is created by cse where the
   SECOND read of p + 0xA0 sits, so it is always ahead of the D_800814A0
   address `lui'.  TWENTY-TWO SPELLINGS MEASURED AND REJECTED, and because
   they all leave the same seven rows they are ONE fact and not twenty-two:

     statement order inside the released-node block (8 orderings of
       {three p clears, D_800814A0 |= 0x8000, n + 0x1E |= 0x8000}): 7..158
     `n' read at the top of the block from the test's own load (kept)   7
     `n' read after the D statement (a RELOAD, cse blocked by the
       D store): 495 words / 143
     `m = *(u8 **)(p + 0xA0); if (m != NULL) { n = (UnkNode *)m; ... }'
       with the copy before the clears 8, after them 7, after D 16
     no local at all, `*(u16 *)(*(u8 **)(p + 0xA0) + 0x1E) |= 0x8000'
       494 words / 143
     the three p clears as COMPONENT_REFs through an invented struct, which
       DOES buy the sched1 hoist of the D load (the `lw' moves from 402 to
       399) and still measures 7 or 8 -- so MEM_IN_STRUCT_P on the CLEARS is
       MEASURED FREE here and the invented typedef is not in this body
     `if (1)' round the n + 0x1E statement 7, round the D statement 7,
       round the D statement with D first 8
     `do { } while (0)' round the D statement 494 words / 19
     splitting D_800814A0 into a load local and a late store 500 words / 32

   WHAT I DID NOT TRY, and what I would try next: the copy only loses the
   LUID tie, so anything that makes the D_800814A0 address `lui' older than
   the copy insn wins.  Two candidates neither of which this unit can
   express today -- a second reference to D_800814A0 earlier in case 6 (cse
   would hoist one address pseudo for both), and p as a struct pointer
   THROUGHOUT so the test's load is a COMPONENT_REF and cse's forwarding
   across the clears changes.  A permuter run on the released-node block
   alone is the cheap answer; I am not allowed to spend one.

   TWO FACTS THAT ARE *NOT* LEVERS HERE, measured free, so do not preserve
   them: `&&' against nested `if's for case 6's
   `*(s16 *)(p + 0xAE) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0' gate, and
   `*(s32 *)(d + 0x1C) &= 0xBFFFFFFF' against `&= ~0x40000000' (both 26 on
   the baseline that measured 26).

   DECLARATIONS
   extern u8 D_80176460[];
   extern u8 D_80176490[];
   extern u8 D_80176498[];
   extern u32 D_800814A0;
   extern s32 D_8008346C;
   extern s16 D_80083228;
   extern Unk80083462 D_80083460;
   s32 func_8003F270(void);
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   s32 func_800A05A4(u8 *, s32, s32, s16, s32);
   s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
   void func_800BB044(u8 *);
   s32 func_800A56E0(s32);
   void func_800A2B04(UnkVec *, s32, s32);
   void func_800A4ACC(u8 *p);
   void func_80047784(u8 *, s32, s32);
   s32 func_b29_80175858(u8 *, UnkVec *, u8 *);
   void func_b29_80171094(u8 *, UnkVec *, u8 *, u8 *);
   END DECLARATIONS
*/
void func_b29_80172FC0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk8006DE24 *tbl;
    Unk80083160 *h;
    u8 *g;
    u8 *e;
    u8 *m;
    UnkNode *n;
    u8 *q;
    u8 *f;
    u8 *g2;
    s16 k;
    s16 z;
    s32 i;
    s32 x;
    s32 y;
    s16 out[4];

    k = 0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 7:
                k = 1;
            case 3:
                g = d + 0xE;
                break;
            case 6:
                k = 1;
            case 2:
                g = d + 0xB;
                break;
            case 5:
                k = 1;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        else
        {
            switch (*(u16 *)(d + 0x46) & 0x3FFF)
            {
            case 3:
                g = d + 0xE;
                break;
            case 2:
                g = d + 0xB;
                break;
            case 1:
                g = d + 0x8;
                break;
            default:
                g = NULL;
                break;
            }
        }
        if (g[0] != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFF7F;
            if (k != 0)
            {
                m = (u8 *)D_800814A8;
                *(s32 *)(d + 0x60) = (s32)m;
                q = *(u8 **)(m - 0x14);
                d[0x72] = q[0x24];
                d[0x73] = q[0x25];
            }
            else
            {
                tbl = D_8006DE24;
                i = g[0];
                e = (u8 *)&tbl[i];
                if (e[0x12] == 2)
                {
                    m = *(u8 **)(d + 0x60);
                    if (m != NULL)
                    {
                        q = *(u8 **)(m - 0x14);
                        d[0x72] = q[0x24];
                        d[0x73] = q[0x25];
                    }
                }
                else
                {
                    *(s32 *)(d + 0x60) =
                        func_800A05A4(d, c[0x24], c[0x25], *(s16 *)(d + 0x2A), 0x10);
                    x = *(s8 *)(d + 0x72);
                    y = *(s8 *)(d + 0x73);
                    d[0x72] = __builtin_abs(x);
                    d[0x73] = __builtin_abs(y);
                }
            }
            if (func_800A94A0(d, g, k, p + 0x98) != 0)
            {
                func_800BB044(d);
                p[0x9B] += 1;
            }
        }
        else
        {
            *(s32 *)((u8 *)b + 0x14) = 0;
            *(s32 *)((u8 *)b + 0x10) = 0;
            *(s32 *)((u8 *)b + 0xC) = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            D_8008346C = 0;
            *(u16 *)(D_800814A8 + 0xA6) -= 1;
            func_800A4ACC(d);
            d[0x6D] -= 1;
            *(void **)(p + 0x8C) = func_b29_80171094;
            d[0x73] = 0;
            d[0x72] = 0;
            *(u16 *)(d + 0x46) &= 0x7FFF;
        }
        break;
    case 1:
        if (func_8003F270() != 0)
        {
            *(u16 *)(c + 0x14) |= 0x800;
            break;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s16 *)(p + 0xAE) = 8;
        p[0x9B] += 1;
    case 2:
        *(s16 *)(p + 0xAE) -= 1;
        if (*(s16 *)(p + 0xAE) > 0)
        {
            *(u16 *)(c + 0x1E) -= 0x100;
            *(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
            break;
        }
        n = (UnkNode *)func_b29_80175858(p, b, c);
        *(UnkNode **)(p + 0xA0) = n;
        h = &D_80083160;
        e = ((u8 **)*(u8 **)*(u8 **)*(u8 **)(c + 0x28))
            [D_80176498[((h->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7]];
        *(u8 **)(c + 0x0) = e;
        *(u8 **)(c + 0x8) = *(u8 **)(e + 0x4);
        out[2] = 0;
        out[1] = 0;
        out[0] = 0;
        if (func_8003DE58(*(u8 **)(c + 0x8), (UnkSlot *)c, out, 0) != NULL)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
            z = out[2];
            *(s16 *)(p + 0xB2) = z;
            *(s16 *)(p + 0x92) = (*(u16 *)((u8 *)b + 0xA) - *(u16 *)(d + 0x88)) + z / 2;
        }
        *(u16 *)(p + 0x98) |= 8;
        *(s32 *)(d + 0x1C) &= ~0x40000000;
        *(u8 **)(c + 0x2C) = D_80176490;
        func_80047784(c, D_80176490[((h->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(u16 *)(c + 0x1E) = 0x800;
        *(u16 *)(c + 0x1C) = 0x800;
        *(s16 *)(p + 0xAE) = 8;
        p[0x9B] += 1;
        break;
    case 3:
        *(s16 *)(p + 0xAE) -= 1;
        if (*(s16 *)(p + 0xAE) >= 0)
        {
            *(u16 *)(c + 0x1E) += 0x100;
            *(u16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
            *(s16 *)(p + 0x92) += (s16)*(u16 *)(p + 0xB2) >> 4;
            break;
        }
        *(s16 *)(p + 0xAE) = 4;
        *(u16 *)(p + 0x98) |= 0x80;
        p[0x9B] += 1;
        break;
    case 4:
        *(s16 *)(p + 0xAE) -= 1;
        if (*(s16 *)(p + 0xAE) < 0)
        {
            *(s32 *)(p + 0xB4) = *(s32 *)(c + 0xC);
            *(s16 *)(p + 0xBA) = *(u16 *)(c + 0x10);
            *(s16 *)(p + 0xB8) = *(u16 *)(c + 0x12);
            *(u16 *)(c + 0x12) -= 0x80;
            *(s16 *)(p + 0xAE) = 9;
            p[0x9B] += 1;
        }
        break;
    case 5:
        *(s16 *)(p + 0xAE) -= 1;
        if (*(s16 *)(p + 0xAE) > 0)
        {
            *(s32 *)(d + 0x1C) |= 0x10000000;
            *(u16 *)(c + 0x14) |= 0xC;
            *(u16 *)(c + 0x10) |= 0x20;
            c[0xC] -= c[0xC] / *(s16 *)(p + 0xAE);
            c[0xD] -= c[0xD] / *(s16 *)(p + 0xAE);
            c[0xE] -= c[0xE] / *(s16 *)(p + 0xAE);
            if (*(s16 *)(p + 0xAE) == 1)
            {
                *(s16 *)(p + 0xAE) = 8;
                p[0x9B] += 1;
            }
        }
        break;
    case 6:
        *(s16 *)(p + 0xAE) -= 1;
        if (*(s16 *)(p + 0xAE) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        if (*(s32 *)(p + 0xA0) != 0)
        {
            n = *(UnkNode **)(p + 0xA0);
            *(s16 *)(p + 0xAC) = 0;
            *(s32 *)(p + 0xA4) = 0;
            *(s32 *)(p + 0x90) = 0;
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            do
            {
                *(s32 *)(p + 0xA0) = 0;
            } while (0);
        }
        *(s32 *)(d + 0x1C) &= ~0x10000000;
        *(s16 *)(c + 0x12) = *(u16 *)(p + 0xB8);
        *(s32 *)(c + 0xC) = *(s32 *)(p + 0xB4);
        *(s16 *)(c + 0x10) = *(u16 *)(p + 0xBA);
        *(s32 *)((u8 *)b + 0x14) = 0;
        *(s32 *)((u8 *)b + 0x10) = 0;
        *(s32 *)((u8 *)b + 0xC) = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(s32 *)(d + 0x1C) |= 0x40000000;
        f = D_80176460;
        if (*(u8 **)(c + 0x2C) != f)
        {
            *(u8 **)(c + 0x2C) = f;
            func_80047784(c, f[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        }
        g2 = (u8 *)&D_80083460;
        if (*(u32 *)(g2 + 0xC) == 0)
        {
            *(u16 *)(g2 + 0xA) -= 1;
            *(void **)(p + 0x8C) = func_b29_80171094;
            *(u16 *)(c + 0x1E) = 0x1000;
            *(u16 *)(c + 0x1C) = 0x1000;
            func_800A4ACC(d);
            d[0x73] = 0;
            d[0x72] = 0;
            d[0x6D] -= 1;
            *(u16 *)(d + 0x46) &= 0x7FFF;
            func_800A56E0(0xB4);
        }
        break;
    }
}
