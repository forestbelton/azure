#include "common.h"

/* ovl_dng_s07's six-state cutscene director, a JUMP TABLE on
   *(s16 *)(p + 0xA) over cases 0..5.  THIS FUNCTION OWNS
   jtbl_s07_80024008 (6 words = 6 cases + 0 pad, dispatch bound 6, 6 distinct
   targets; tools/decomp/jtcheck.py agrees).  1 member (dngfam.py), so no
   replication.

   STATUS: target 566 words, candidate 562 words, 232 ALIGNED DIFFERENCES
   (ovlverify reports 459 differing position-by-position; aligndiff is the
   number to read).  216 of the 232 rows are REGISTER-NAME-ONLY rows and the
   remaining ~16 are all accounted for by one allocation decision -- see THE
   CAUSE.  The body is believed structurally complete: every opcode,
   immediate and displacement outside the spill set matches, and the 4-word
   deficit reconciles EXACTLY against that one decision (see the arithmetic
   below).  Do not go looking for a missing statement.

   WHAT IT DOES: p is a node payload (node = p - 0x20), b the caller's
   UnkVec, c an UnkSlot whose +0x14 flags gate the state-3 escape.  Before
   the switch, states 1..3 each spawn ten 0x312 nodes running
   func_s07_80024B58, jittered +-0x1FF<<9 around b, dressed from D_800DEA68 at
   0x3030C0 with +0x1C/+0x1E = 0x1000 and +0x10 = 0x60, and in state 2 given a
   per-direction displacement off the D_8006CCD8/D_8006CCE8 pair.  Then the
   +0x50 timer is decremented and the switch runs: state 0 publishes the
   party record's projected position into b, installs func_s07_800248F8 as
   the renderable, walks the direction ray with func_800A4688 until it is
   blocked and divides the three velocity words by the step count; states 1
   and 3 run +0xC/+0x50 countdowns; state 2 advances b and stops it on the
   tile it was aiming at; state 4 unlinks with func_80044A50; state 5 tears
   the actor down.

   DONORS: csrc/ovl_dng_s12.c func_s12_80024020 (same vram, 524 words,
   dngcallee 10/17, the SAME six-case table skeleton -- dngtwin finds nothing
   at any ratio for either body) and csrc/ovl_dng_b37.c func_b37_80172840
   (10/17, 434 words).  Nine of s12's spellings carried unchanged: the
   `s = *(u8 **)p; w = s - 0x20; o = *(u8 **)(s - 0x14);' head, the
   `q = (u8 *)n->unk_0C' / `e = (u8 *)n + 0x20' pair, the
   `*(s16 *)(X) = *(u16 *)(X) + 1' state bump idiom, the `(UnkNode *)(p -
   0x20)' casts, `*(s8 *)(s + 0x72) * 0x40' as a MULTIPLY (s12 prices the
   shift at 20 rows; it is `lb' + `sll 6' here too), case 0's reuse of `q'
   for the indirect record, the ascending case order, and the
   `(**(u16 **)(p + 0x4) & 0x80)' gate.

   THE CAUSE -- MEASURED, not hypothesised.  The target SPILLS the pointer
   local `o' (= *(u8 **)(s - 0x14)) to 0x44($sp) and keeps the
   loop-invariant `k1 << 21' in $s5.  This candidate does the opposite: `o'
   wins $fp and BOTH `k0 << 21' and `k1 << 21' spill (to 0x4C and 0x50).
   Everything else about the allocation agrees -- same frame (-0x80, vars 56,
   args 32), same 10 saves, same FIVE spill slots, same nine callee-saved
   values in the same roles.  The arithmetic closes exactly:

       566  target
       -  6  the four `lw $t0, 0x44($sp)' o-reloads plus two load-delay
             `nop's that this candidate does not need (o is in $fp)
       +  2  the `sw'/`lw' pair for the k1 << 21 spill that the target
             does not need (it is in $s5)
       ----
       562  candidate

   The register ROTATION follows from the same decision and nothing else:
   target $fp/$s7/$s6 = p/b/s with `o' in memory, candidate
   $s7/$s6/$s5/$fp = p/b/s/o, and every later callee-saved name shifts one
   position.  That is the 216 register-only rows.  The ~16 structural rows
   are the six o-reload words, the two k1h spill words, the four
   D_8006CCD8/D_8006CCE8 `lui'/`addiu' materialisations that move with the
   pressure, and a four-row schedule difference where the target computes
   both `dx' and `dy' before the two `__builtin_abs' macros and this
   candidate interleaves them.

   SO THE WHOLE JOB IS: make `o' lose its register to `k1 << 21'.
   `allocno_compare' is floor_log2(n_refs) * n_refs / live_length, and `o'
   has EIGHT unweighted references here (1 set, 1 at the +0x14 gate, 2 at the
   func_800A05A4 call, 2 inside the j-loop, 2 at the loop exit) which is ten
   loop-weighted -- just over the floor_log2 cliff at 8.  Seven weighted
   references would drop floor_log2 from 3 to 2 and, by that arithmetic
   alone, hand the register to k1h.  I could not find an honest spelling that
   removes a reference: all four o-use SITES are visible in the listing as
   reloads of 0x44($sp), so none of them is a re-read of
   *(u8 **)(s - 0x14) and none is a read through `w + 0xC'.  Note also that
   the register NUMBERS are NOT a readout of priority order here -- find_reg
   never allocates a register for the first time in pass 0, so an allocno
   that conflicts with everything already placed jumps to $fp regardless of
   its rank.  Two attempts to compute the margin from the printed
   `;; N regs to allocate' order contradicted each other; treat the ORDER as
   data and the margin as unknown.

   MEASUREMENT TABLE.  Every row is this body with ONE change, measured with
   aligndiff (register-only rows INCLUDED in the counts):

   | spelling                                              | words | aligned |
   | as written below                                      |  562  |   232   |
   | `*(s32 *)((u8 *)n->unk_08 + N)' plain casts instead of|  562  |   232   |
   |   the `n->unk_08->unk_NN' struct fields               |       |         |
   | `k0 * 0x200000 - k0 * pC * 0x100000 / 12' instead of  |  562  |   232   |
   |   `(k0 << 21) - ((k0 * pC) << 20) / 12'               |       |         |
   | declaration order `n' before `q' swapped              |  562  |   232   |
   | declaration order `s, o, w' instead of `s, w, o'      |  562  |   232   |
   | `sx = 0; sy = 0;' as statements instead of in the     |  562  |   232   |
   |   `for' initialiser comma list                        |       |         |
   | e+0x48 store before e+0x4C instead of after           |  562  |   234   |
   | `q + 0xC' colour store before the `q + 0x8' record    |  563  |   234   |
   |   load (adds a `move' and loses the forwarded base)   |       |         |
   | `do { } while (0)' round the k1 displacement statement|  562  |   214   |
   | `do { } while (0)' round the whole state-2 block      |  562  |   242   |
   | out[2] as two `if'/`else' stores instead of `?:'      |  563  |   236   |
   | `u16 tx; u16 ty;'                                     |  562  |   231   |
   | `s32 tx' plus `u16 txs = tx' at the call sites        |  562  |   229   |

   TWO OF THOSE ROWS SCORE BETTER AND ARE PROVABLY WRONG; THE KEPT SPELLING
   IS THE ONE BELOW.  `u16 tx' (231) and the separate `u16 txs' truncation
   local (229) both let combine fold the `+ 0x20' through the 16-bit
   truncation and emit `ori $reg, $reg, 0x20' where the target has
   `addiu $s3, $v0, 0x20' -- a different opcode, so those three rows are
   cheaper only because the register rotation happens to line up better.
   `s32 tx' with a `(u16)' cast at each use site is the only form that emits
   `addu'/`andi' as the target does.  The 214-row `do { } while (0)' is a
   DEVICE that does not touch the cause: it changes a schedule, leaves the
   word count at 562 and leaves `o' in $fp, so it is a mid-fight improvement
   and not evidence (checked: the prologue still reads `lw $fp, -20($21)').

   NOT THE LEVER HERE: MEM_IN_STRUCT_P.  Writing the spawned node's vector
   stores as `n->unk_08->unk_00' struct fields and as
   `*(s32 *)((u8 *)n->unk_08 + 0x0)' plain casts measures IDENTICALLY, 562
   words and 232 rows both ways.  That is a NON-WITNESS for the flag that
   func_b37_80172840 names as the lever that landed it, so the flag is not
   free here -- it is simply not the axis.

   WHAT TO TRY NEXT, in order.  (1) Find an eighth o-reference to remove, or
   a ninth to add -- the cliff is at 8 and it cuts both ways, so test BOTH
   directions before concluding.  (2) Raise k1h's weighted reference count to
   4 by a spelling rather than a device; the one-statement `do' wrapper is
   worth 20 rows but does not flip the spill, so the bump needs to land on
   k1h specifically and a wider scope made it worse.  (3) The pseudo count in
   the j-loop is the one place where this candidate provably differs from the
   target in KIND and not just in names: the target keeps THREE quantities
   there (the accumulator in $v0, `tx' in $s3, `(u16)tx' in $s1) where this
   candidate merges all three into one register, and func_b37_80172840's
   comment records that any change to the pseudo count in that position
   throws the PARAMETER allocation for 101 rows -- which is exactly this
   body's symptom.  Both ways of splitting them that I tried produce the
   wrong `ori', so the split wanted is one I have not found.

   THE TABLE IS SETTLED AND REPORTED.  A standalone tools/cdkgcc -S of this
   candidate emits the six `.word' entries in ascending-case order, whose
   addresses ascend in the same sequence as the listing's
   .Ls07_80024310 / 800246B8 / 800246DC / 800247D0 / 80024844 / 8002486C for
   cases 0..5.  All six arms are distinguishable (different calls, different
   lengths), so the order is measurable and ascending is correct; there is no
   pad word and no `default:' among the entries -- the range check's `beqz'
   lands on the epilogue.

   DECLARATIONS
   s32 rand(void);
   void func_800478B8(u8 *);
   s32 func_8004491C(UnkNode *, u32);
   UnkNode *func_8003FD64(s32, void *);
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   void func_80044A50(UnkNode *);
   s32 func_800A3820(s16);
   s32 func_800A05A4(u8 *, s32, s32, s16, s32);
   s32 func_800A4688(s32, s32, s16, s16, s32);
   s32 func_800A56E0(s32);
   s32 func_800BCB04(s32, s32, s16);
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   s32 func_s07_800248F8(u8 *p, UnkVec *v);
   void func_s07_80024B58(u8 *p, UnkVec *b, UnkSlot *c);
   extern s16 D_8006CCD8[];
   extern s16 D_8006CCE8[];
   extern s32 D_8008346C;
   extern u8 D_80045340[];
   extern u8 D_80083498[];
   extern u8 D_800DEA68[];
   extern u8 D_800DEC70[];
   extern u8 D_800DED28[];
   extern u32 D_800814A0;
   END DECLARATIONS

   func_800A4688 is a NEW declaration -- no other unit in the tree declares
   it.  Its arity and widths are read off the call site: five arguments with
   the fifth in 0x10($sp), $a2 sign-extended from a halfword and $a3 loaded
   with `lh', which is the same shape as the already-declared
   func_800A05A4(u8 *, s32, s32, s16, s32) two calls earlier.  Treat the
   first two parameters' s32 as read-from-the-call rather than known. */
void func_s07_80024020(u8 *p, UnkVec *b, UnkSlot *c)
{
    u8 *s;
    u8 *w;
    u8 *o;
    UnkNode *n;
    u8 *q;
    u8 *e;
    s32 i;
    s32 j;
    s32 sx;
    s32 sy;
    s32 k0;
    s32 k1;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    s32 t;
    s32 tx;
    s32 ty;
    UnkVec v;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    w = s - 0x20;
    o = *(u8 **)(s - 0x14);
    k0 = D_8006CCD8[(*(u16 *)(s + 0x2A) >> 9) & 7];
    k1 = D_8006CCE8[(*(u16 *)(s + 0x2A) >> 9) & 7];
    if (*(s16 *)(p + 0xA) > 0 && *(s16 *)(p + 0xA) < 4)
    {
        for (i = 9; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                q = (u8 *)n->unk_0C;
                e = (u8 *)n + 0x20;
                n->unk_10 = (u32)func_s07_80024B58;
                n->unk_08->unk_00 = b->unk_00 + (((rand() & 0x3FF) - 0x1FF) << 9);
                n->unk_08->unk_04 = b->unk_04 + (((rand() & 0x3FF) - 0x1FF) << 9);
                n->unk_08->unk_08 = b->unk_08 + (((rand() & 0x3FF) - 0x1FF) << 9);
                if (*(s16 *)(p + 0xA) == 2)
                {
                    n->unk_08->unk_0C = b->unk_0C >> 2;
                    n->unk_08->unk_10 = b->unk_10 >> 2;
                    n->unk_08->unk_00 += (k0 << 21) - ((k0 * *(s32 *)(p + 0xC)) << 20) / 12;
                    n->unk_08->unk_04 += (k1 << 21) - ((k1 * *(s32 *)(p + 0xC)) << 20) / 12;
                }
                n->unk_08->unk_14 = -rand() * 2;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x1000;
                *(u16 *)(q + 0x14) |= 0xC;
                if ((rand() & 1) != 0)
                {
                    *(u16 *)(q + 0x14) |= 0x1;
                }
                *(s16 *)(q + 0x10) = 0x60;
                *(u8 **)(q + 0x0) = D_800DEA68;
                *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4);
                *(s32 *)(q + 0xC) = 0x3030C0;
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)(e + 0x0) = p;
                *(s16 *)(e + 0x4C) = 0;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4A) = i + 0x28;
                *(s16 *)(e + 0x4) = (s8)s[0x72];
                *(s16 *)(e + 0x6) = (s8)s[0x73];
            }
        }
    }
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out, 0) == 0)
        {
            out[0] = out[1] = 0;
            out[2] = (*(u16 *)(o + 0x14) & 0x8000) ? -0x30 : 0;
        }
        b->unk_00 = *(s32 *)(*(u8 **)(w + 0x8) + 0x0) + (out[0] << 16);
        b->unk_04 = *(s32 *)(*(u8 **)(w + 0x8) + 0x4) + (out[1] << 16);
        b->unk_08 = *(s32 *)(*(u8 **)(w + 0x8) + 0x8) + (out[2] << 16);
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_s07_800248F8);
        *(s32 *)(s + 0x60) =
            func_800A05A4(s, o[0x24], o[0x25], *(s16 *)(s + 0x2A), (s16)func_800A3820(7));
        if (*(s32 *)(s + 0x60) == 0)
        {
            for (j = 0, sy = 0, sx = 0; j < func_800A3820(7); j++)
            {
                tx = (o[0x24] + sx) * 0x40 + 0x20;
                ty = (o[0x25] + sy) * 0x40 + 0x20;
                if ((s16)func_800A4688(
                        (u16)tx, (u16)ty,
                        (s16)func_800BCB04((u16)tx, (u16)ty,
                                           *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x80),
                        *(s16 *)(s + 0x2A), *(s32 *)(s + 0x60)) != 0)
                {
                    break;
                }
                sy += k1;
                sx += k0;
            }
            s[0x72] = o[0x24] + k0 * j;
            s[0x73] = o[0x25] + k1 * j;
        }
        else
        {
            q = *(u8 **)(*(u8 **)(s + 0x60) - 0x14);
            if ((*(u16 *)(q + 0x14) & 0x8000) != 0 && (c->unk_14 & 0x8000) != 0)
            {
                *(s16 *)(p + 0xA) = 3;
                break;
            }
            s[0x72] = q[0x24];
            s[0x73] = q[0x25];
        }
        *(s16 *)((u8 *)&v + 0x2) = *(s8 *)(s + 0x72) * 0x40 + 0x20;
        *(s16 *)((u8 *)&v + 0x6) = *(s8 *)(s + 0x73) * 0x40 + 0x20;
        *(s16 *)((u8 *)&v + 0xA) =
            func_800BCB04(*(u16 *)((u8 *)&v + 0x2), *(u16 *)((u8 *)&v + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30);
        if (*(s16 *)((u8 *)&v + 0xA) >= 0x200)
        {
            *(s16 *)((u8 *)&v + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA);
        }
        *(s16 *)((u8 *)&v + 0xA) = *(u16 *)((u8 *)&v + 0xA) - 0x30;
        dx = *(s16 *)((u8 *)&v + 0x2) - *(s16 *)((u8 *)b + 0x2);
        dy = *(s16 *)((u8 *)&v + 0x6) - *(s16 *)((u8 *)b + 0x6);
        ax = __builtin_abs(dx);
        ay = __builtin_abs(dy);
        if (ax < ay)
        {
            ax = ay;
        }
        t = ax / 8 + 1;
        *(s16 *)(p + 0x50) = t;
        b->unk_0C = (dx << 16) / *(s16 *)(p + 0x50);
        b->unk_10 = (dy << 16) / *(s16 *)(p + 0x50);
        b->unk_14 = (v.unk_08 - b->unk_08) / *(s16 *)(p + 0x50);
        *(s32 *)(p + 0xC) = 8;
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) + *(u16 *)(p + 0xC);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        *(s32 *)(p + 0xC) = *(s32 *)(p + 0xC) - 1;
        if (*(s32 *)(p + 0xC) <= 0)
        {
            *(s32 *)(p + 0xC) = 0xC;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        *(s32 *)(p + 0xC) = *(s32 *)(p + 0xC) - 1;
        if (*(s32 *)(p + 0xC) > 0)
        {
            b->unk_0C += k0 << 16;
            b->unk_10 += k1 << 16;
        }
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)((u8 *)b + 0x2) / 64 == *(s8 *)(s + 0x72) &&
            *(s16 *)((u8 *)b + 0x6) / 64 == *(s8 *)(s + 0x73))
        {
            *(s16 *)(p + 0x50) = 0;
        }
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            b->unk_0C = k0 << 16;
            b->unk_10 = k1 << 16;
        }
        break;
    case 3:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s32 *)(s + 0x60) != 0)
        {
            func_8009CE1C(*(u8 **)(s + 0x60), 0xA, p[0x9], 1, *(s16 *)(s + 0x2A), (u32)s, 2);
        }
        *(s16 *)(p + 0x50) = 0x10;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        func_80044A50((UnkNode *)(p - 0x20));
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 5:
        if ((*(s32 *)(p + 0x10) & 0x8000) != 0)
        {
            *(s32 *)(p + 0x10) &= ~0x8000;
        }
        else if (*(s16 *)(p + 0x50) <= 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
