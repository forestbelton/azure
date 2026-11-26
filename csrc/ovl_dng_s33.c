#include "common.h"

typedef struct UnkDngRoom2970
{
    u8 pad_00[0xC];
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDngRoom2970;
typedef struct UnkDng73E94SP
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x12];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x6C];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng73E94SP;

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with the tile's colour word
   copied off the payload at +0x8 and then faded: each of the three bytes is
   scaled by the payload's s16 life counter at +0x32 over 0x100. The twin is
   csrc/ovl_dungeon_800D50FC.c's func_800D5AC0 (120 words, resident), which is
   this function without the fade; the relink moves every scratchpad slot down
   by 4 -- the input SVECTOR to +0x00, the OT cursor to +0x18, the OT base to
   +0x20, RotTransPers' two out-parameters to +0x90/+0x94 and the OT index to
   +0xC0 -- so the struct is this unit's own, and the input vector is the
   scratchpad base itself, which is why $a0 is a bare register copy.

   THE FADE REPLACES THE TWIN'S DEAD setRGB0 ECHO. func_800D5AC0 needs
   setRGB0(t, t->r0, t->g0, t->b0) to keep three loads whose stores cse
   strips; here the three lbu/mult/sb runs are the real work and the echo must
   NOT be written -- added in front of the tag setup it is 151 words and 76
   differing.

   `/ 0x100' AND NOT `>> 8': the bgez/addiu 0xFF pair ahead of each sra is the
   signed division's correction, and `>> 8' drops it -- 139 words, 102
   differing.

   THE SCALE FACTOR IS RE-READ AT ALL THREE CHANNELS. A local for
   *(s16 *)(p + 0x32) is 145 words and 109 differing; the target has three
   separate `lh 0x32($s3)'.

   Three orderings, every one re-measured here rather than taken from the
   twin: the colour word and its three scalings come BEFORE the tag setup
   (setTile1 first is 147 words and 111 differing); `sp->unk_18' is assigned
   BEFORE `sp->unk_20' even though the target STORES them the other way round
   (swapped, 11 differing, against the twin's 20 for the same pair); and
   `p = (u8 *)n + 0x20;' comes before `v = n->unk_08;' (swapped, 17
   differing, against the twin's 4).

   setTile1(t) plus setSemiTrans(t, 1) is the whole `sb 2, 0x3' /
   `sb 0x6A, 0x7' pair: cse folds the getcode back to the 0x68 setTile1 just
   stored and drops the dead store. `(SVECTOR *)&sp->unk_00' written for
   `(SVECTOR *)sp' is a tie at 0.

   148 words, 0 differing. raw 7 == relocdump's 7, none on a function symbol:
   1 HI16 + 2 LO16 on D_80083160, R_MIPS_26 on RotTransPers, GetTPage and
   func_80067F20, and one on .text for the loop's own backward `j'. The
   +0x90/+0x94 addresses that splat auto-names D_1F800090/D_1F800094 carry no
   relocation on this side and come out byte-identical, so seqcheck counts one
   extra entry in the target and reports seq=FAIL on that bookkeeping alone. */
extern s16 D_80025118;
extern u8 D_s33_80024004[];
extern u8 D_80025100[];
extern u8 D_8002510C[];
extern u8 D_80045340[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E3D68;
extern s32 D_8008346C;
s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
s32 func_800A6D30(void);
s32 func_800C8B84(u8 *, s16, s16);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;

s32 func_s33_80024044(u8 *p, UnkVec *v)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_18;
        sp->unk_18 = (u8 *)(t + 1);
        sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            t->r0 = t->r0 * *(s16 *)(p + 0x32) / 0x100;
            t->g0 = t->g0 * *(s16 *)(p + 0x32) / 0x100;
            t->b0 = t->b0 * *(s16 *)(p + 0x32) / 0x100;
            setTile1(t);
            setSemiTrans(t, 1);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, t);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* The payload's node tick: integrate the +0x3C..+0x44 velocity into the
   caller's vector, accelerate it by +0x48..+0x50, poke D_80025118, retire the
   actor when its screen-x at +0x34 comes within 0x10 of the vector's integer
   part, and retire it again when the +0x32 fuse runs out.  64 words, 0
   differing.

   No carved twin at 0.70 for the whole body; dngtwin's 0.758 row,
   csrc/ovl_dng_s72.c's func_s72_8002615C, is the same six-statement integrate
   with a different tail, and the teardown pair
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000;' is that family's
   verbatim.  The third parameter is unused here -- $a2 is a scratch register
   for D_80025118's %hi in the target -- and is kept because the +0x10 handler
   slot func_s33_80024394 installs is called with three.

   BOTH HALFWORD LOADS IN THE abs() MUST BE ARRAY REFS, AND IT IS A
   CONJUNCTION: NEITHER ALONE DOES ANYTHING.

   | spelling                                        | words | aligned rows |
   | both operands as array refs, as written         |  64   |      0       |
   | `((s16 *)p)[0x1A]' with `*(s16 *)((u8 *)b + 2)' |  66   |     15       |
   | `*(s16 *)(p + 0x34)' with `((s16 *)b)[0x1]'     |  66   |     15       |
   | both as plain casts                             |  66   |     15       |

   All three failures give the bit-identical 15 rows.  The target fills the
   `beqz' delay slot with the `sh' of `D_80025118 = 1', which means BOTH
   `lh's were hoisted ABOVE that store; a plain cast anywhere in the
   subtraction pins the whole expression below it, because true_dependence
   will not excuse an anonymous halfword load against a fixed-address scalar
   store, and one surviving dependence is enough to hold the sum down.  So the
   store stays where it is, the `lh 0x2($a1)' load delay becomes a `nop', and
   the body is two words long.  As array refs MEM_IN_STRUCT_P is set on both
   and the hoist is allowed.  Same lever and same direction as
   func_s72_800246B0's +0x2A fact, but that body states it on ONE load and
   this one shows the fact is over every load in the hoisted expression --
   a one-at-a-time sweep reads all three failing spellings as the same number
   and concludes nothing.

   AND THE DISCRIMINATOR IS NOT "ARRAY REF versus CAST", IT IS WHETHER A CAST
   SITS BETWEEN THE ADDITION AND THE DEREFERENCE.  `expr.c's INDIRECT_REF case
   sets MEM_IN_STRUCT_P when `TREE_OPERAND (exp, 0)' is itself a PLUS_EXPR --
   "If address was computed by addition, mark this as an element of an
   aggregate".  `((s16 *)p)[0x1A]' is `*((s16 *)p + 0x1A)', so the operand IS
   the PLUS and the flag is set; `*(s16 *)(p + 0x34)' wraps the PLUS in a
   NOP_EXPR for the cast, so the operand is the CAST and the flag is NOT set.
   That predicts the equivalences without a compile: move the cast inside, and
   `*(s16 *)((u8 *)b + 0x2)' and `((s16 *)b)[0x1]' differ for this reason
   alone, not because one is subscripted. */
void func_s33_80024294(u8 *p, UnkVec *b, u8 *r)
{
    b->unk_00 += *(s32 *)(p + 0x3C);
    b->unk_04 += *(s32 *)(p + 0x40);
    b->unk_08 += *(s32 *)(p + 0x44);
    *(s32 *)(p + 0x3C) += *(s32 *)(p + 0x48);
    *(s32 *)(p + 0x40) += *(s32 *)(p + 0x4C);
    *(s32 *)(p + 0x44) += *(s32 *)(p + 0x50);
    D_80025118 = 1;
    if (__builtin_abs(((s16 *)p)[0x1A] - ((s16 *)b)[0x1]) < 0x10)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if ((*(s16 *)(p + 0x32) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x211 spawner for this unit: seed the new node's mover from the parent
   `a's, offset each axis by the caller's `dx'/`dy'/`dz', keep the parent's
   un-offset position at the payload's +0x34/+0x36/+0x38, give the payload
   three velocities that undo the offset over `f'/8 steps divided by 16, and
   park func_s33_80024294 as the step and func_s33_80024044 as the draw.
   109 words, 0 differing, first compile.

   The twin is csrc/ovl_dng_b19.c's func_b19_80170D28 (dngtwin 0.849, 129
   words, +20 against this body); b15 carries an identical copy.  The 20-word
   gap is three ABSENT STATEMENTS, not missing references -- that body stores
   SIX velocities (halved and quartered) where this one stores three, has a
   further `- 0x64' on the z offset, and writes `*(u32 *)p = c' as well as
   `*(u32 *)(p + 0x8) = c'.  Its frame is `-0x38' with s0..s7 and ra, exactly
   this one's, so its allocation facts were safe to carry; both are re-priced
   here rather than inherited.

   | spelling                                   | words | aligned rows | twin |
   | as written below                           |  109  |      0       | 129/0|
   | `s32 dv = f / 8' hoisted ahead of the three|  104  |     39       |123/49|
   | `dx'/`dy'/`dz' declared `s32'              |  109  |     19       | n/a  |

   THE DIVISOR INLINE AT EACH SITE: HOLDS, SAME DIRECTION, AND COSTS FIVE
   WORDS HERE.  With `dv' hoisted, the whole `f / 8' expansion (`sll'/`sra
   16', `bgez', `addiu 7', `sra 3') is emitted before the first numerator, so
   `sll $v0, $s2, 16' can no longer sit in front of that `bgez' and its `negu'
   can no longer take the branch's delay slot.  Written at each site the
   numerator expands first and cse reuses the one quotient in $a1 for
   divisions two and three -- so "inline at each site" buys the sharing that
   the local was supposed to buy, plus the two delay slots.

   `dx'/`dy'/`dz' ARE `s16': HOLDS AS A FACT, BUT NOT FOR THE TWIN'S STATED
   REASON.  The twin attributes it to the three stack parameters keeping their
   incoming MEMs and being rematerialised at each use.  That cannot be what is
   happening here: `s32' is 109 words, the SAME length, and all 19 rows are a
   save-slot and callee-saved-register permutation across the prologue with no
   extra load anywhere.  Each of the three has only two uses in this body
   against the twin's three, so the rematerialisation threshold is not reached
   -- re-measure this one before carrying it further, and do not carry the
   reason. */
void func_s33_80024394(UnkNode *a, s16 b, u32 c, s16 f, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s33_80024294;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2) + dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6) + dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA) + dz;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x34) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)(p + 0x36) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)(p + 0x38) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        *(s32 *)(p + 0x48) = -(dx << 16) / (f / 8) / 16;
        *(s32 *)(p + 0x4C) = -(dy << 16) / (f / 8) / 16;
        *(s32 *)(p + 0x50) = -(dz << 16) / (f / 8) / 16;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = f;
        func_8004491C(n, (u32)func_s33_80024044);
        *(u32 *)(p + 0x8) = c;
    }
}

/* A second node tick for the same payload shape: poke D_80025118, burn one
   off the +0x2 fuse, fade the slot's three colour bytes at +0xC..+0xE by
   10/13 a tick, advance its +0x1A angle by 0x300 modulo 0x1000, and retire
   the actor when either the fuse runs out or the slot raises 0x8000 at +0x14.
   72 words, 0 differing.  No carved sibling at ratio 0.70 and none from
   xsibs; dngcallee's two symbols (D_80025118, D_800814A0) reach 588
   functions, so the body was read off the listing.

   `r[K] * 10 / 13' IS THE WHOLE `mult'/`mfhi'/`sra 2' RUN AND NEEDS NO SIGN
   CORRECTION.  0x4EC4EC4F is ceil(2^34 / 13), and cc1psx emits the signed
   highpart (`mult' + `mfhi' + `sra 2') with NO `srl 31'/`addu' fixup when the
   dividend came from an `lbu' and the quotient is stored back through a `sb'
   -- verified on a standalone probe, where `r[0xC] * 10 / 13',
   `(u8)(r[0xC] * 10 / 13)' and a two-statement `s32 t = r[0xC] * 10; t / 13'
   all emit the identical seven instructions.  Writing it unsigned
   (`* 10u / 13u') is the one spelling that does NOT: that is `multu'/`srl'.
   So do not read the missing fixup as evidence of a hand-written
   multiply-shift.

   THE FUSE DECREMENT MUST BE AN ARRAY REF -- 33 ALIGNED ROWS.

   | spelling                            | words | aligned rows |
   | `((s16 *)p)[0x1] -= 1', as written  |  72   |      0       |
   | `*(s16 *)(p + 0x2) -= 1'            |  72   |     33       |

   Identical mechanism to func_s33_80024294 above, and this is the body that
   shows it is the unit's house rule rather than one expression's accident:
   every body here opens with the fixed-address scalar store `D_80025118 = 1',
   and the target hoists the NEXT halfword load above it so the `sh' fills no
   delay slot of its own.  A plain cast leaves the load anonymous,
   true_dependence refuses to excuse it against that store, and the load stays
   below -- the word count is unchanged (72 both ways) and the whole body
   rotates by one scheduling slot instead, which is why this one presents as
   33 rows rather than as two extra words.  The LATER read of the same
   address, the `if (*(s16 *)(p + 0x2) <= 0)' gate, is a plain cast and
   measures 0: it sits after the fade work with nothing to hoist past. */
void func_s33_80024548(u8 *p, UnkVec *b, u8 *r)
{
    D_80025118 = 1;
    ((s16 *)p)[0x1] -= 1;
    r[0xC] = r[0xC] * 10 / 13;
    r[0xD] = r[0xD] * 10 / 13;
    r[0xE] = r[0xE] * 10 / 13;
    *(u16 *)(r + 0x1A) += 0x300;
    if (*(u16 *)(r + 0x1A) > 0x1000)
    {
        *(u16 *)(r + 0x1A) -= 0x1000;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* This unit's master step handler, and IT OWNS A JUMP TABLE.
   jtbl_s33_80024028: 7 `.word's, dispatch bound `sltiu $v0, $v1, 0x7' with NO
   minval subtraction (so the case set starts at 0), 7 DISTINCT targets, no pad
   word.  The range-check `beqz $v0' targets .Ls33_800250D4, which is the first
   word of the EPILOGUE, so there is NO `default:' statement -- an out-of-range
   state falls straight out of the switch.  Entry list in table order, which is
   also strictly ascending by address:

     case 0  .Ls33_80024764      case 4  .Ls33_80024E9C
     case 1  .Ls33_800247E4      case 5  .Ls33_80024FC4
     case 2  .Ls33_80024A48      case 6  .Ls33_80025070
     case 3  .Ls33_80024C8C

   ARM ORDER VERIFIED BY STANDALONE COMPILE, not by the verifier.
   `tools/cdkgcc -O2 -G0 -S' emits 7 `.word's at `.rdata'/`.align 3' in the
   order $L33 $L34 $L45 $L53 $L76 $L86 $L90, and those labels sit at
   increasing text offsets in that same order, so entry k is the k-th arm
   block by address on both sides and the sequence resolves to the ROM's.
   Case 0 FALLS THROUGH into case 1 (no `j' at .Ls33_800247E0).

   WHAT IT IS.  `p' is this node's payload, `o = *(u8 **)p' the caster's,
   `n = o - 0x20' its node and `m = n->unk_08' its mover; the +0x82 frame
   counter is bumped before the dispatch.  0 paints the renderable white,
   parks the 0xC-byte D_8002510C template at p+0x94 as the primitive, picks
   the facing out of the caster's +0x2A and pokes D_80025118, then falls into
   1; 1 reads the caster's hand through func_8003DF74, mirrors its position
   into the mover (less 0x40 when the slot is flagged), tags the node with
   D_80045340, picks the target height and step count off the caster's +0x60
   target and seeds the mover's three velocities out of the 0x20-byte
   D_s33_80024004 eight-direction table; 2 waits for func_800A4778's arrival,
   spawns the 0x212 impact node (func_s33_80024548 step) and otherwise
   integrates; 3 ramps the primitive's scale through a six-step ladder on
   c+0x1C and sprays two func_s33_80024394 sparks a frame; 4 keeps spraying
   and runs the func_8009D218/func_800C8B84 hit; 5 winds the scale back down;
   6 waits 0x15 frames on D_80025118 and then kills the node.
   678 words, 0 differing, 0 aligned differences.

   DONOR: csrc/ovl_dng_s38.c's LANDED func_s38_80024FA4, which dngtwin cannot
   see at any floor (it finds no carved sibling at 0.70 for this body at all)
   and which `dngcallee.py' names alone at 14 of 20 shared symbols with a
   clean gap to the next tier.  Same `(u8 *p, UnkVec *v, u8 *c)' signature,
   same `o'/`n'/`m' head, same `u16 out[3]' stack local for func_8003DF74,
   same D_80045340 tag block, same `D_8006CCD8[facing]'/`D_8006CCE8[facing]'
   corner pair, same `UnkDngS58Blk32'/`UnkDngS58Rec'/`UnkDngS58Name12'
   typedefs for the lwl/lwr block moves and the 4-byte table records.  Six of
   its ten recorded facts carried unchanged (the table copy before `n'/`m',
   case 0's +0xA bump as an array ref, `z' assigned after the +0x6 store, the
   0x8000 gate's arm order, the 0x7B gate as `<= 0' with the integrate block
   as the ELSE, and the SIX-save frame's `arr[2]' absence -- this frame is
   -0x68 against its -0x70 and has no `s32 arr[2]', which the outgoing
   7-argument area plus `out[3]' plus `tbl[8]' accounts for exactly).
   First compile: 679 words / 23 aligned rows.  Four faults, all source-level.

   | spelling                                               | words | rows |
   |--------------------------------------------------------|-------|------|
   | as written below                                       |  678  |   0  |
   | p+0x7B's abs/scale as ONE statement with the ternary    |      |      |
   |   inside __builtin_abs (the donor's own spelling)      |  678  |   3  |
   |   ... that ternary hoisted into an `s32 k' first       |  678  |  16  |
   |   ... the ternary inline plus `do { } while (0)'       |  679  |   5  |
   | p+0x8A's 4/1 choice as a ternary, `>= 0x3C ? 1 : 4'    |  678  |   3  |
   |   ... as a ternary the other way, `< 0x3C ? 4 : 1'     |  678  |   5  |
   | `*(s16 *)(c + 0x6) = 0x64' BEFORE the +0x84/+0xA pair  |  679  |  23  |
   | the abs ternary written `!=' with its arms exchanged   |  678  |   0  |

   BOTH REMAINING FAULTS WERE ONE MECHANISM: A VALUE CHOSEN IN TWO ARMS MUST
   BE WRITTEN AS TWO STATEMENTS, NOT AS A `?:'.  This is the body's result and
   it is worth 3 rows at each of the two sites.

   For p+0x8A it is `jump.c:927' -- "Simplify if (...) { x = a; goto l; } x =
   b; to x = a; if (...) goto l; x = b;".  A `?:' whose arms are two bare
   constant sets matches that pattern exactly, so jump_optimize HOISTS one set
   ABOVE the comparison and inverts the branch.  The value pseudo is then live
   across the `sltu', conflicts with the hard register local-alloc gave the
   comparison result, and lands in $a0 where the target has $v0 -- `.greg'
   prints `;; 376 conflicts: ... 2 ...' for exactly that reason.  Written as
   two `if'/`else' stores the then-arm is TWO insns (set plus store), so
   `temp3's `SET_DEST' is a MEM rather than a REG and the hoist is refused;
   each arm's pseudo stays local to its block, local-alloc gives both $v0, and
   jump2's cross_jump merges the two identical `sh $v0, 0x8A($s4)' tails for
   free.  Same word count either way.

   For p+0x7B it is `fold-const.c:3736' -- fold distributes any unary op, and
   any binary op against a constant, INTO both arms of a COND_EXPR.  So
   `__builtin_abs(c ? A : B) * 4 - 3' duplicates the whole abs/`sll'/`addiu'
   chain into both arms and the store's QImode pseudo is SET TWICE.  That
   makes it a global allocno, so `combine_regs' cannot tie it to the dying
   `sll' result, and because sched1 hoists the following `lh 0x7E($s4)' above
   the `sb' the hard register is already taken: 3 rows of `addiu $v1, $v0, -3'
   / `sb $v1' against the target's `addiu $v0, $v0, -3' / `sb $v0'.  Proof of
   the mechanism rather than a guess: a `do { } while (0)' round that one
   statement supplies the CODE_LABEL that stops the hoist and fixes the
   register outright -- but the label also stops `addiu $v1, $sp, 0x28' from
   filling the `lh's load delay, so it is 679 words with a `nop'.  Hoisting
   the ternary into an `s32 k' also undistributes it and also fixes the
   register, and is WORSE at 16 rows, because the extra global allocno inverts
   the two feeder loads: `allocno_compare' ranks on
   floor_log2(refs)*refs/live_length, the two loads have 3 refs each, the
   o+0x72 load is born one insn earlier so its live_length is one longer, and
   with `k' in the pool it loses $v0 to the e+0x24 load.  Writing the two
   stores out separately keeps the arithmetic LOCAL to each arm -- tied to the
   dying operand, so `subu $v0, $v0, $v1' -- while adding no allocno, and
   jump2 cross-joins from the `bgez' onward, which is why the word count does
   not move.

   NOT MEASURABLE, not free: the abs ternary's arm order (`==' with
   0x73/0x25 first against `!=' with 0x72/0x24 first) is 678 / 0 both ways,
   because the two spellings are the same tree after canonicalisation.  Also
   not measurable: `*(s8 *)(p + 0x7B)' for `p[0x7B]' as the store's lvalue,
   and `((s16 *)p)[0x45]' for `*(s16 *)(p + 0x8A)' -- both 0 once the two
   statement-splits above are in place, and both 3 before them, so neither
   is the lever it looks like.

   `rand' is spelled `rand', not func_80069EF8: `splat/common_syms.txt' has
   `rand = 0x80069ef8' and the two spray loops call it four times each. */
void func_s33_80024668(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    u8 *u;
    u8 *r;
    u8 *w;
    s32 i;
    s32 z;
    s32 j;
    s32 h;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s33_80024004;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(UnkDngS58Name12 *)(p + 0x94) = *(UnkDngS58Name12 *)D_8002510C;
        *(u32 *)(c + 0x8) = (u32)(p + 0x94);
        D_80025118 = 1;
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
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
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = z;
        if ((((UnkSlot *)n->unk_0C)->unk_14 & 0x8000) == 0)
        {
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)v + 0x2) + out[0];
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)v + 0x6) + out[1];
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)v + 0xA) + out[2];
        }
        else
        {
            *(s16 *)((u8 *)v + 0xA) = z - 0x40;
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x7A] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x40;
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            *(s16 *)(c + 0x1E) = 0x800;
            *(s16 *)(c + 0x1C) = 0x800;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - 0x40;
        }
        else
        {
            *(s16 *)(p + 0x78) = *(u16 *)(o + 0x88) - 0x50;
        }
        e = *(u8 **)(o - 0x14);
        p[0xA0] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
        p[0xA1] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
        if (*(s8 *)(o + 0x72) == e[0x24])
        {
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x73) - e[0x25]) * 4 - 3;
        }
        else
        {
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) - e[0x24]) * 4 - 3;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            func_80044A50((UnkNode *)(p - 0x20));
            *(s16 *)(p + 0xA) = 6;
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0x82) = 0;
            break;
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        n2 = func_8003FC64(0x212);
        if (n2 != NULL)
        {
            *(s16 *)((u8 *)n2 + 0x22) = 0x10;
            n2->unk_10 = (u32)func_s33_80024548;
            func_8004491C(n2, (u32)D_80045340);
            r = (u8 *)n2->unk_0C;
            *(s16 *)(r + 0x10) = 0x40;
            *(u16 *)(r + 0x14) |= 0xC;
            w = (u8 *)n2->unk_08;
            *(s16 *)(w + 0x2) = *(u16 *)((u8 *)v + 0x2);
            *(s16 *)(w + 0x6) = *(u16 *)((u8 *)v + 0x6);
            *(s16 *)(w + 0xA) = *(u16 *)((u8 *)v + 0xA);
            r = (u8 *)n2->unk_0C;
            *(s16 *)(r + 0x1E) = 0x800;
            *(s16 *)(r + 0x1C) = 0x800;
            r[0xE] = 0x78;
            r[0xD] = 0x78;
            r[0xC] = 0x78;
            *(UnkDngS58Name12 *)((u8 *)n2 + 0x40) = *(UnkDngS58Name12 *)D_8002510C;
            *(u32 *)(r + 0x8) = (u32)((u8 *)n2 + 0x40);
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(p + 0x78);
                *(u32 *)(c + 0x8) = (u32)D_80025100;
                *(s16 *)(c + 0x1E) = 0x400;
                *(s16 *)(c + 0x1C) = 0x400;
                *(s16 *)(c + 0x10) = 0x40;
                v->unk_08 += 0x80000;
                c[0xC] = 0x20;
                c[0xD] = 0x20;
                c[0xE] = 0x20;
                *(s16 *)(p + 0x88) = 0x258;
                *(s16 *)(p + 0x8A) = 2;
                *(s16 *)(p + 0x8C) = 0x190;
                *(s16 *)(p + 0x84) = 0;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
                *(s16 *)(c + 0x6) = 0x64;
                func_800A56E0(0x300);
            }
            else
            {
                func_80044A50((UnkNode *)(p - 0x20));
                *(s16 *)(p + 0xA) = 6;
                *(s16 *)(p + 0x84) = 0;
                *(s16 *)(p + 0x82) = 0;
            }
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) + 1;
        if (*(s16 *)(p + 0x84) < 5)
        {
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0xBB8;
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        }
        else if (*(u16 *)(c + 0x1C) > 0x1000)
        {
            if (*(u16 *)(c + 0x1C) > 0x2EE0)
            {
                *(s16 *)(p + 0x88) = 0x2BC;
            }
            if (*(u16 *)(c + 0x1C) > 0x2710)
            {
                *(s16 *)(p + 0x88) = 0x1F4;
            }
            else if (*(u16 *)(c + 0x1C) > 0x2000)
            {
                *(s16 *)(p + 0x88) = 0xC8;
            }
            else if (*(u16 *)(c + 0x1C) > 0x1800)
            {
                *(s16 *)(p + 0x88) = 0x78;
            }
            else if (*(u16 *)(c + 0x1C) > 0x1000)
            {
                *(s16 *)(p + 0x88) = 0x3C;
            }
            else if (*(u16 *)(c + 0x1C) > 0x800)
            {
                *(s16 *)(p + 0x88) = 0x28;
            }
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x88);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x88);
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + *(u16 *)(p + 0x8C);
        *(s16 *)(p + 0x8C) = *(u16 *)(p + 0x8C) + 0x14;
        if (c[0xC] < 0x78)
        {
            if (c[0xC] < 0x3C)
            {
                *(s16 *)(p + 0x8A) = 4;
            }
            else
            {
                *(s16 *)(p + 0x8A) = 1;
            }
            c[0xC] = c[0xC] + p[0x8A];
            c[0xD] = c[0xD] + p[0x8A];
            c[0xE] = c[0xE] + p[0x8A];
        }
        if (*(s16 *)(p + 0x84) >= 0x51)
        {
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        for (i = 0; i < 2; i++)
        {
            func_s33_80024394((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xC0C0C0,
                              (rand() & 0xFF) | 0x80, (rand() & 0x7F) - 0x40, (rand() & 0x7F) - 0x40,
                              (rand() & 0x7F) - 0x40);
        }
        break;
    case 4:
        for (i = 0; i < 2; i++)
        {
            func_s33_80024394((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xC0C0C0,
                              (rand() & 0xFF) | 0x80, (rand() & 0x7F) - 0x40, (rand() & 0x7F) - 0x40,
                              (rand() & 0x7F) - 0x40);
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + *(u16 *)(p + 0x8C);
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) + 2;
        if (*(s16 *)(p + 0x84) >= 0x3D)
        {
            *(s16 *)(p + 0x84) = 0x28;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            if (func_8009D218(*(u8 **)(o + 0x60), 4, o) == 0)
            {
                j = (func_800A6D30() & 3) + 4;
                h = (p[0x9] >> 2) + j;
                func_800C8B84(*(u8 **)(o + 0x60), D_800E3D68 == 0xFF ? 0xFF : 0x10, h);
            }
        }
        break;
    case 5:
        if (*(u16 *)(c + 0x1C) > 0x32)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(s16 *)(p + 0x88) * 2;
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(s16 *)(p + 0x88) * 2;
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + *(u16 *)(p + 0x8C);
        if (c[0xC] > 6)
        {
            c[0xC] = c[0xC] - 4;
            c[0xD] = c[0xD] - 4;
            c[0xE] = c[0xE] - 4;
        }
        *(s16 *)(p + 0x84) = *(u16 *)(p + 0x84) - 2;
        if (*(s16 *)(p + 0x84) <= 0)
        {
            *(s16 *)(p + 0x84) = 0;
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x15)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_80025118 == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_80025118 = 0;
            }
        }
        break;
    }
}
