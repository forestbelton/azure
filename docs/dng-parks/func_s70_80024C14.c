/* PARK -- func_s70_80024C14, ovl_dng_s70. 266 words target, 261 words built,
   101 aligned differences. Batch 45, agent h224. dngfam: ONE MEMBER, 266
   words, so nothing replicates off it.

   The other three stubs of this unit LANDED in the same run (func_s70_8002401C
   265/0, func_s70_8002466C 280/0, func_s70_80024B48 51/0), so this is the only
   thing between ovl_dng_s70 and 100%.

   NO DONOR EXISTS. dngtwin --unit reports no carved sibling at ratio >= 0.70.
   dngcallee's best overlap is 5 of 10 symbols reached by SEVEN functions, i.e.
   the noise tier, and NO landed body in the tree touches the PSX scratchpad at
   0x1F800000 at all -- a `grep -l' for D_1F80001C over every C file in csrc
   and in docs/dng-parks comes back EMPTY, which is why there is no donor row
   anywhere above. The whole body below was derived from the listing; the walk
   skeleton
   and the D_80083160 +0x8D0 cursor idiom come from the LANDED in-unit sibling
   func_s70_80024440 (csrc after integration; 139 words, 0 differing), which is
   the same tag-chain walk.

   NOT the same body as docs/dng-parks/func_s63_80024D58.c. That park is the
   s63 bank's 339-word mesh renderer with a -0x570 frame and its 16x16 grid in
   the FRAME; this one is 266 words, -0x458, and keeps its colour grid in the
   SCRATCHPAD. They are installed by the same state-3 spawn in their banks'
   dispatchers (func_s63_8002401C / func_s70_8002401C) but they are not a
   family and dngfam agrees. Its local map is still worth reading for the
   descriptor-versus-array question; its phase layout is not this one's.

   WHAT IT DOES, derived entirely from the listing. A tag-chain walk: run the
   body on p, then `n = *(UnkNode **)(p - 8)', `p = (u8 *)n + 0x20', until the
   chain ends, returning 0. The parameter is SPILLED to its incoming home slot
   at sp+0x458 and reloaded there at the top of every walk iteration and again
   in the tail, which is how you can tell `p' and the body cursor `q' are two
   locals and not one. Three phases per node:

   PHASE A seeds eight words of the scratchpad with a grey ramp:
   `(func_800644B8((i + *(s16 *)(q + 0x48)) * 1200) >> 7) + 0x20' splatted into
   all three colour bytes, i counting DOWN from 7 (the giv decrements, and i is
   used in the value so check_dbra_loop cannot have reversed it).

   PHASE B is a 16x16 projection: for j then i, build the SVECTOR
   (q+0xC + i*64, q+0xE + j*64, q+0x10), RotTransPers it into vtx[i][j],
   subtract q+0x4 / q+0x6 from the input's x and y IN PLACE, take
   func_80064710 of the sum of their squares, and write a second grey ramp into
   the scratchpad cell. The scratchpad row stride is 64 and the column stride 4,
   so the grid is u32 [16][16] at 0x1F800000 and phase A fills the first eight
   words of row 0 -- which phase B then overwrites. That is the ROM's
   behaviour, not a transcription error: both loops address 0x1F800000 + i*4
   and phase B's j = 0 pass covers phase A's range.

   PHASE C emits one semi-transparent POLY_G4 per cell over the inset square
   `m .. 14 - m', where m is `7 - *(s16 *)(q + 0x48)' clamped at 0 from below.
   Each prim takes 0x24 bytes of D_80083160's +0x8D0 cursor (sizeof POLY_G4),
   its four colours from the scratchpad corners [j][i] [j+1][i] [j][i+1]
   [j+1][i+1], its four vertices from vtx[i][j] [i][j+1] [i+1][j] [i+1][j+1] as
   WORDS, and goes on OT row 0 through addPrim with no depth index. After the
   loops one DR_MODE of 0xC bytes is cut from the same cursor and handed to
   func_80067F20 with GetTPage(0, 2, 0, 0).

   FOUR FACTS READ OFF THE LISTING THAT ARE NOT GUESSES

   * `sv' IS UNSIGNED 16-BIT, NOT AN SVECTOR. Every read of sp+0x18/0x1A is
     `lhu', and SVECTOR's vx is `short' so it would be `lh'. The sign
     extension the listing does show -- `sll 16' / `sra 16' before each
     `mult' -- is cse forwarding the stored subtraction result and widening it,
     not a reload, which is why the FIRST read after the RotTransPers call is a
     real `lhu' (the call is a cse barrier) and the second is not.
   * THE TWO `long *' OUT PARAMETERS ARE ONE REUSED LOCAL. sp+0x420 is both
     RotTransPers' third argument and the phase B scratch that gets written
     twice (`sw $v0, 0x420($sp)' then `sw $v1, 0x420($sp)'); it is
     address-taken, so each assignment is a real store and each use is the
     forwarded register. A non-address-taken local would emit neither store.
   * EVERY `* 1200' AND `* 100' IS A PLAIN CONSTANT MULTIPLY. The listing's
     sll/addu/subu/sll chains are cc1psx's synthesis of them; writing the
     decomposition is the mistake the levers file prices at 17 rows.
   * THE 0xFFFFFF addPrim MASK IS HOISTED INTO A CALLEE-SAVED REGISTER in the
     prologue (`lui'/`ori' before any save completes), as in the landed
     func_s70_80024440. That is free, not a lever.

   THE ONE FAULT, AND IT IS A loop.c GIV-COMBINATION FAULT, NOT ALLOCATION

   All 101 rows are a uniform whole-body register rename -- candidate
   s2/s3/s1/s8 against target s3/s2/s3/s6 and so on -- plus five missing words,
   and both have ONE cause. The target leaves its scratchpad addresses PARTLY
   UNREDUCED and this body reduces them completely, which costs it two
   long-lived values; with two fewer allocnos competing, every callee-saved
   name after the first sits one position early.

   Precisely, from the target listing:

     phase B store   target:  sll $a0, $s1, 2 / lui $t0, 0x1F80 /
                              addu $a0, $a0, $t0 / addu $a0, $s3, $a0
                              -- the whole address recomputed, and a `nop'
                              left in the back branch's delay slot
                     built:   one `addu' off a reduced giv plus
                              `addiu $s1, $s1, 4' in that delay slot
     phase C reads   target:  $s3 = i*4 + 0x1F800000 reduced on i only,
                              $fp = j*64 recomputed per outer iteration,
                              `addu $v1, $fp, $s3' per inner iteration, four
                              loads at 0x0/0x40/0x4/0x44 off $v1
                     built:   ONE doubly-reduced giv, +4 inner and +64 outer
     and the target SPILLS its outer loop bound to sp+0x428 (8 more bytes of
     frame, 0x458 against 0x450) and REMATERIALISES `g' as
     `lui %hi / addiu %lo / lw 0x0(reg)' at all SIX of its uses, where this
     body keeps &D_80083160 in a callee-saved register. Three symptoms, one
     pressure difference.

   THE MECHANISM, CONFIRMED FROM THE `-da' `.loop' DUMP of this body:

     Insn 401: giv reg 218 src reg 88 benefit 2 used 1 lifetime 1 mult 4 add 0
     Insn 403: giv reg 87  src reg 88 benefit 4 used 1 lifetime 7 mult 4
               add (reg:SI 215)
     Insn 406: dest address src reg 88 benefit 5 used 1 lifetime 1 mult 4
               add (reg:SI 215)
     giv at 403 combined with giv at 406
     giv of insn 401 not worth while, 0 vs 60
     giv at 406 reduced to (reg:SI 308)

   The `c' pointer giv (403) and the MEM-address giv (406) carry the SAME
   add_val, reg 215, so loop.c COMBINES them and reduces one register for
   both; in the target they must differ, so they cannot combine and the
   MEM givs are left to the runtime `addu'. reg 215 is itself
   `j*64 + reg 216', and the outer pass then reduces THAT:

     Insn 713: giv reg 215 src reg 89 benefit 4 used 1 lifetime 65 mult 64
               add (reg:SI 216)
     giv at 713 reduced to (reg:SI 312)
     giv of insn 710 not worth while, 0 vs 74      -- plain j*64, add 0
     giv of insn 714 not worth while, 0 vs 74      -- plain j*4,  add 0

   So the discriminator is ADD_VAL, not cost: a giv whose add_val is 0 is
   priced at 0 benefit and left alone (which is why the target's `sll $fp' and
   `sll $s4' survive in BOTH bodies), and a giv whose add_val is a register is
   reduced. The register in question is the hoisted 0x1F800000:

     Insn 127: regno 122 (life 65), savings 1  moved to 748

   -- `move_movables' lifts the `lui' out of BOTH loop nests, and once the
   constant is a register it becomes an add_val and the address becomes
   reducible. THE TARGET DOES NOT HOIST IT: its `lui $t0, 0x1F80' sits inside
   the phase B inner loop at 0x80024DA4. The test is loop.c:1730,
   `(threshold * savings * m->lifetime) >= insn_count' with
   `threshold = (loop_has_call ? 1 : 2) * (1 + n_non_fixed_regs)' at
   loop.c:631; the giv side is loop.c:3996,
   `v->lifetime * threshold * benefit < insn_count' with threshold from
   loop.c:3359. Both cited against scratch/gcc-src/cdk-gcc-b18/loop.c.

   SO THE THING TO TRY NEXT, and I did not reach it: a spelling in which the
   0x1F800000 constant is NOT a loop-invariant movable, so that no address giv
   acquires a register add_val. Four ways in were measured and all four made it
   worse (table below) because every one of them still leaves fold free to
   reassociate the constant out to the front of the sum. The untried route is
   to stop fold reassociating at all -- a VAR_DECL holding the grid base that
   is WRITTEN somewhere inside the walk loop, so `invariant_p' fails on it in
   the j and i loops and `simplify_giv_expr' cannot use it as an add_val. That
   is the opposite polarity of docs/matching-levers.md's "A CONSTANT INSIDE A
   HOISTED ADDRESS IS UNREACHABLE BY ANY CAST", and this park is the witness
   that the polarity exists.

   EVERY SPELLING MEASURED, all with aligndiff, all against this body

     | the scratchpad grid spelled                        | words | rows |
     | `c = (u32 *)(j * 64 + (i * 4 + 0x1F800000))' and   |  261  |  101 |
     |   c[0] c[0x10] c[1] c[0x11]              (KEPT)    |       |      |
     | the same with the addends reversed,                |  261  |  101 |
     |   `(i * 4 + 0x1F800000 + j * 64)'                  |       |      |
     |   -- NOT MEASURABLE, fold canonicalises it         |       |      |
     | no `c' at all, four inline                         |  257  |  111 |
     |   `*(u32 *)(j * 64 + (i * 4 + 0x1F8000NN))'        |       |      |
     |   -- four separate `lui'/`ori' pairs, unshared     |       |      |
     | `c = (u32 *)(i * 4 + 0x1F800000)' then             |  253  |  105 |
     |   c[j * 16] c[j * 16 + 16] c[j * 16 + 1] ...       |       |      |
     | `c = (u32 *)(i * 4 + 0x1F800000)' then             |  253  |  105 |
     |   `d = (u32 *)((u8 *)c + j * 64)', d[0] d[0x10]... |       |      |
     | phase B's store ALSO routed through `c'            |  261  |  100 |
     | `volatile' on phase B's scratchpad store           |  261  |  101 |
     |   -- NOT MEASURABLE here; it does not stop the     |       |      |
     |   reduction, because the giv is on the pointer     |       |      |
     |   and not on the MEM                               |       |      |
     | `DVECTOR vtx[16][16]' with the phase C reads as    |  258  |  119 |
     |   `*(u32 *)&vtx[i][j]'                             |       |      |

   READ THE 253/105 ROW AS THE IMPORTANT ONE. Two structurally independent
   perturbations of the phase C address -- the j term moved into the index, and
   the j term moved into a second pointer local -- give the IDENTICAL residue.
   By the brief's own rule that means the cause is not in that block, and it is
   the reason I stopped sweeping spellings there and went to the dump instead.

   The `DVECTOR' row is worth noting because it INVERTS the landed in-unit
   sibling's measured fact. func_s70_80024440's comment prices `DVECTOR xy[2]'
   against `s32 xy[2]' at 21 aligned rows in favour of DVECTOR, on a
   MEM_IN_STRUCT_P argument. Here the plain `s32' array is 101 rows and
   DVECTOR is 119, and it also comes out THREE WORDS SHORTER -- so the fact
   does not transfer from a 139-word two-element array to a 266-word 16x16
   one, and I would not try it again before the giv fault is closed.

   DECLARATIONS
   s32 func_800644B8(s32);
   s32 func_80064710(s32);
   END DECLARATIONS

   func_80067F20 is already declared in include/structs.h:2067 as
   `void func_80067F20(u32 *, s32, s32, s32, s32)' -- do NOT add a local
   declaration for it, a `void *' last parameter is a conflicting type and the
   unit will not compile. GetTPage, RotTransPers, SetPolyG4 and SetSemiTrans
   come from common.h and are the listing's func_80066460, func_80065420,
   func_80066708 and func_80066640, resolved through splat/common_syms.txt.
   `0x1F800000' and `0x1F80001C' are PLAIN CONSTANTS, not symbols: splat's
   `%hi(D_1F80001C)' / `%lo(D_1F800004)' renderings are its lui/addiu pairing
   heuristic, and the giveaway is that it calls the giv's own `addiu $s3, $s3,
   4' increment a `%lo'. Do not declare an extern for them. */
s32 func_s70_80024C14(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_G4 *t;
    u32 *dm;
    u32 *c;
    s32 i;
    s32 j;
    s32 k;
    s32 m;
    u16 sv[4];
    s32 vtx[16][16];
    s32 w0;
    s32 w1;

    while (1)
    {
        q = p;
        for (i = 7; i >= 0; i--)
        {
            k = (func_800644B8((i + *(s16 *)(q + 0x48)) * 1200) >> 7) + 0x20;
            *(u32 *)(i * 4 + 0x1F800000) = k + ((k << 8) + (k << 16));
        }
        for (j = 0; j < 16; j++)
        {
            for (i = 0; i < 16; i++)
            {
                sv[0] = *(u16 *)(q + 0xC) + i * 64;
                sv[1] = *(u16 *)(q + 0xE) + j * 64;
                sv[2] = *(u16 *)(q + 0x10);
                RotTransPers((SVECTOR *)sv, (long *)&vtx[i][j], (long *)&w0, (long *)&w1);
                sv[0] = sv[0] - *(u16 *)(q + 0x4);
                sv[1] = sv[1] - *(u16 *)(q + 0x6);
                w0 = (func_800644B8((func_80064710((s16)sv[0] * (s16)sv[0] +
                                                   (s16)sv[1] * (s16)sv[1]) >>
                                    2) *
                                       100 -
                                   (*(s16 *)(q + 0x48) << 8)) >>
                      7) +
                     0x20;
                w0 = w0 * (0x20 - *(s16 *)(q + 0x48)) / 32;
                *(u32 *)(j * 64 + (i * 4 + 0x1F800000)) = w0 + (w0 << 8) + (w0 << 16);
            }
        }
        if (*(s16 *)(q + 0x48) < 7)
        {
            m = 7 - *(s16 *)(q + 0x48);
        }
        else
        {
            m = 0;
        }
        for (j = m; j < 15 - m; j++)
        {
            for (i = m; i < 15 - m; i++)
            {
                t = (POLY_G4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x24;
                c = (u32 *)(j * 64 + (i * 4 + 0x1F800000));
                *(u32 *)&t->r0 = c[0];
                *(u32 *)&t->r1 = c[0x10];
                *(u32 *)&t->r2 = c[1];
                *(u32 *)&t->r3 = c[0x11];
                SetPolyG4(t);
                SetSemiTrans((u8 *)t, 1);
                *(u32 *)&t->x0 = vtx[i][j];
                *(u32 *)&t->x1 = vtx[i][j + 1];
                *(u32 *)&t->x2 = vtx[i + 1][j];
                *(u32 *)&t->x3 = vtx[i + 1][j + 1];
                addPrim((u32 *)(g->unk_00 + 0xB0), t);
            }
        }
        dm = (u32 *)*(u32 *)(g->unk_00 + 0x8D0);
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)dm + 0xC;
        func_80067F20(dm, 0, 0, GetTPage(0, 2, 0, 0), 0);
        addPrim((u32 *)(g->unk_00 + 0xB0), dm);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}
