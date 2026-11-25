#include "common.h"

s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s32 func_800A6870(s16);
void func_800AD4D0(u8 *);
s32 func_800AD568(u8 *);
UnkNode *func_800B4C7C(u16, u8 *, s16, s16);
void func_800478B8(u8 *);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
void func_s28_80024024_3(u8 *, u8, u8 *) __asm__("func_s28_80024024");
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
s32 func_8004491C(UnkNode *, u32);
s32 func_80045C34(u8 *, UnkVec *, UnkSlot *);
extern u8 D_80025EE4[];
void func_s28_80024FD4(u8 *, UnkVec *);
UnkNode *func_s28_800253D0(u8 *, UnkVec *);

/* Raise the actor's +0x64 pool by func_800A6870's scale of the second
   parameter plus 8, doubled when the actor's +0x28 has bit 0, then republish
   it with func_800AD568, float the 0x8004 number and refresh with
   func_800AD4D0. All of it gated on func_8009D218 coming back zero.
   37 words, 0 differing.

   TWIN: the landed func_s30_80024024 in csrc/ovl_dng_s30.c (dngtwin 0.974,
   39 words; func_s25_80024028 is identical to it at the same ratio and
   width). Deltas, all off this listing: the gate argument is 1 not 4, the
   scale bonus is +8 not +16, and this copy's fraction arm is a straight
   DOUBLING where the twin's is `t + (t >> 1)' -- that one arm is the whole
   two-word difference, since the twin spends `sll 16'/`sra 17'/`addu' where
   this has a bare `sll $a1, $v1, 1'.

   Of the twin's facts, the `u8' second parameter and the TWO-argument
   func_8009D218 alias both HOLD. Its `s16' pair HOLDS, and more strongly
   than on the twin, which found the pair free: here the widths carry the
   `addiu $v1, $v0, 0x8' ahead of the +0x28 gate and the `addu $a1, $v1,
   $zero' in the branch's delay slot, and `s32' for BOTH drops that move
   (36 words, 26 differing) because the two pseudos coalesce.

   THE DOUBLING IDIOM IS NOT FREE AT A SUB-WORD DESTINATION, which
   contradicts the "`i * 0x400' and `i << 10' are the same tree" lever --
   that rule holds for an `int' destination, not here. Measured:

     `s16 t; s16 u;' with `u = t << 1;'                 37 w,  0   kept
       ... `u = t + t;'                                 37 w,  0   also free
       ... `u = t * 2;'                                 38 w, 22
       ... `u = (s16)(t * 2);'                          38 w, 22
       ... `u = u * 2;'                                 38 w, 22
     `u16 t; u16 u;' with `u = t * 2;'                  37 w,  7
     `u16 t; s32 u;' with `u = t * 2;'                  37 w,  6
     `s16 t; s32 u;' with `u = t * 2;'                  38 w, 27
     `s32 t; s32 u;' with `u = t * 2;'                  36 w, 26
     `s32 t; s16 u;' with `u = t * 2;'                  37 w,  0
     `s32 t; u16 u;' with `u = t * 2;'                  37 w,  0

   The 38-word rows are `*2' narrowing through `convert_to_integer' to the
   `s16' destination and costing `sll 16'/`srl 15' where `<< 1' and `+ t'
   do not narrow at all. FOUR of the eleven spellings measure 0, so the
   target does NOT pin the pair's widths on its own -- it pins only that the
   destination of the doubling is sub-word OR the idiom is not `*2'. The
   twin's `s16'/`s16' is kept because it is the twin's and it is one of the
   four. */
void func_s28_80024024(u8 *p, u8 k)
{
    s16 t;
    s16 u;

    if (func_8009D218_2(p, 1) == 0)
    {
        t = func_800A6870(k) + 8;
        u = t;
        if ((p[0x28] & 1) != 0)
        {
            u = t << 1;
        }
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) + u;
        func_800AD568(p);
        func_800B4C7C(0x8004, p, *(s16 *)(p + 0x64), 1);
        func_800AD4D0(p);
    }
}

/* A per-frame step function: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x4 counter, drag the mover's +0x8 down
   by 0xA0000, hand the renderable to func_800478B8, and when its +0x14
   reports 0x6000 mark the owning actor retired and set the same bit in
   D_800814A0.
   39 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.933,
   36 words). Its head statement, its gate and its whole retire block carry
   verbatim; the three extra words are this copy's own middle, where the
   twin's `+0x1E' scale pair is replaced by the actor's +0x4 counter bump and
   the `b->unk_08 -= 0xA0000'. The twin's note on D_800814A0 being the PLAIN
   `u32' scalar (structs.h, no declaration needed) holds here too and gives
   the `lui %hi'/`lw %lo' pair, with the `lui' hoisted into the gate's delay
   slot. The twin's own priced fact -- the scale pair as a read-back, not a
   second sum -- does not apply: this copy has no scale pair.

   ONLY THE STATEMENT ORDER IS LOAD-BEARING. Priced on this body, all at
   39 words:

     the +0x4 counter bump BEFORE the +0x8 drag               0   kept
       ... the two swapped, the drag first                    8
     `b->unk_08 -= 0xA0000;'                                  0   kept
       ... `+= -0xA0000;'                                     0   free
       ... `*(s32 *)((u8 *)b + 0x8) -= 0xA0000;'              0   free
       ... second parameter `u8 *b', `*(s32 *)(b + 0x8)'      0   free
     the +0x4 bump read through `u16'                         0   kept
       ... read through `s16'                                 0   free
     the head reached through `*(u8 **)p' twice               0   kept
       ... hoisted into a `u8 *n' local                       0   free
     the gate read through `u16', the retire store `u16'      0   kept
       ... either one through `s16'                           0   free

   So the UnkVec second parameter is the twin's spelling rather than a
   measured one, and the only thing this listing actually constrains is that
   the +0x8 drag is emitted last of the three. The drag is a `lui 0xFFF6' plus
   `addu' because 0xFFF60000 is outside `addiu' range; the `-=' and `+= -'
   forms are the same tree. */
void func_s28_800240B8(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    b->unk_08 -= 0xA0000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_80024154);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_800242FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_80024790);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_80024A64);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_80024FD4);

/* The ring-of-four flyer's own step function: bump the node's +0x14, the
   actor's +0x10 frame counter and its +0x12 slot counter, spin the
   renderable's +0x1A by 0x300, and then dispatch on the actor's +0xE phase.
   Phase 0 integrates the mover's velocity until the +0x14 budget runs out;
   phases 1 and 2 place the renderable's three colour bytes from the slot
   counter's residue modulo 7 -- the /4, the (/2)%2 and the %2 bits each
   scaled by 128 -- with phase 1 additionally fading them by the frame
   counter, driving the +0x1C/+0x1E scale down from 0x19 and handing off to
   func_s28_80024FD4; phase 3 waits for the renderable's +0x14 to report
   0x6000 and then retires the actor into D_800814A0.
   186 words, 0 differing.

   NO TWIN AT ANY RATIO -- LEADS.txt reports nothing at the 0.60 floor -- and
   the callee-set intersection could not help either: its only distinctive
   symbols are D_800814A0 and func_800478B8, which 96 landed functions share,
   so the recipe returns a tier with no gap. dngnear returns nothing for this
   unit before OR after three landings, because its landed neighbours are in
   my private copy and not in csrc/. What landed it instead was THIS FILE:
   func_s28_800240B8 and func_s28_8002553C were already at 0 differing above
   it and are the same step-function family, so the head bumps, the +0x1A
   spin, the 32-bit velocity integration and the whole phase-3 retire block
   are their measured spellings transplanted. Only the modulo-7 chain, the
   colour arithmetic and the phase tree were derived from the listing.

   The 4-case switch is a BRANCH TREE, not a table: the dispatch tests `== 1',
   then `< 2', then `== 0', then `== 2' and `== 3', with the default falling
   to the epilogue. There is no jtbl symbol and nothing for this unit's rodata
   -- gcc only builds a table from more cases than this.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced:

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `s16 k' for the modulo-7 residue                         |  186  |   0  |
   |   ... `s32 k'                                           |  184  |  91  |
   | `s16 h' for `k / 2'                                      |  186  |   0  |
   |   ... `s32 h'                                           |  184  |  62  |
   | `h = k / 2;' written AFTER the +0xC colour store         |  186  |   0  |
   |   ... ahead of it, at the top of the arm                |  190  |  58  |
   | the +0x10 bump BEFORE the +0x12 bump                     |  186  |   0  |
   |   ... the other order                                   |  187  |  17  |
   | `(k % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80)'        |  186  |   0  |
   |   ... `- *(s16 *)(p + 0x10) * 8 + 0x80' unbracketed     |  186  |  29  |
   | the +0x1C scale as a READ-BACK of the +0x1E store        |  186  |   0  |
   |   ... the same sum written out twice                    |  190  |  10  |
   | case 0's gate `p+0x10 >= p+0x14'                         |  186  |   0  |
   |   ... `p+0x14 <= p+0x10'                                |  186  |   2  |

   THE TWO WIDTH ROWS ARE `shorten_binary_op' AND THEY ARE THE BODY. With
   `s16' operands gcc narrows TRUNC_MOD_EXPR to HImode, so `(h % 2) * 128'
   collapses to one `sll 16'/`sra 9' pair; with `s32' it is a full SImode mod
   and the arm is two words shorter and 62 rows out. `/' is NOT narrowed the
   same way, which is why `(k / 4) * 128' keeps its plain `sra 2'/`sll 7' in
   the same statement -- the asymmetry inside one expression is the tell, and
   it is what names the locals' width before any compile.

   `* 128' AND `<< 7' ARE FREE HERE (186 / 0 either way), which is the lever
   file's power-of-two rule holding for an `int'-typed product. Note that it
   does NOT hold at a sub-word destination: see func_s28_80024024 above in
   this same file, where `t * 2' and `t << 1' are 22 aligned differences
   apart. The precondition is the DESTINATION's width, not the operand's.

   MEASURED AND FREE, so none of these is why this lands: `h' dropped and
   written inline as `((k / 2) % 2)' (186 / 0 -- so the local is a readability
   choice and only its WIDTH matters); `k' taken from the un-stored sum
   `(s16)(*(u16 *)(p + 0x12) + 1) % 7 + 1' with the +0x12 store moved after it,
   rather than read back out of +0x12 (186 / 0, cse forwards either way);
   `*(u16 *)(c + 0x14) &= ~0x100' for `&= 0xFEFF'; and `(k / 4) << 7' for
   `(k / 4) * 128'. The +0x1C read-back is also NOT the only spelling that
   works -- a chained `*(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = ...' and an
   `s16' or `s32' temp holding the sum both measure 186 / 0; the read-back was
   kept because it is this family's idiom, measured as such on
   func_s13_8002418C, and the 10-row row above is only against writing the sum
   TWICE. */
void func_s28_800250E8(u8 *p, UnkVec *b, u8 *c)
{
    s16 k;
    s16 h;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    *(s16 *)(p + 0x12) = *(u16 *)(p + 0x12) + 1;
    k = *(s16 *)(p + 0x12) % 7 + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    switch (*(s16 *)(p + 0xE))
    {
    case 0:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x14))
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 1:
        *(s16 *)(c + 0x12) = 0;
        *(u16 *)(c + 0x14) &= 0xFEFF;
        c[0xC] = (k / 4) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        h = k / 2;
        c[0xD] = (h % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        c[0xE] = (k % 2) * 128 - (*(s16 *)(p + 0x10) * 8 - 0x80);
        *(s16 *)(c + 0x1E) = (0x19 - *(s16 *)(p + 0x10)) * 256;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            func_s28_80024FD4(p, b);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 2:
        c[0xC] = (k / 4) * 128;
        h = k / 2;
        c[0xD] = (h % 2) * 128;
        c[0xE] = (k % 2) * 128;
        if (*(s16 *)(p + 0x10) >= 0x20)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        }
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s28", func_s28_800253D0);

/* Another of this bank's per-frame step functions, the one func_s28_80025614
   installs on its 0x212 node: bump the +0x14 counter on the node at the
   actor's +0x0 and the actor's own +0x10 frame counter, hand the renderable
   to func_800478B8 and spin its +0x1A rotation by 0x300, integrate the
   mover's three velocity words, and once the frame counter passes the actor's
   +0x14 budget mark the actor retired and set the same bit in D_800814A0.
   54 words, 0 differing, first compile.

   NO EXTERNAL DONOR WAS USED OR NEEDED. dngtwin's best is func_s67_800246F0
   at 0.755 and it was never opened: by the time this came up, THIS FILE
   already held func_s28_800240B8 at 0 differing (the same head bump, the same
   retire block) and func_s28_80025738 at 0 differing (the same 32-bit
   velocity integration, as its states 2 and 6). Every statement here is one
   of those two bodies' measured spellings with this listing's offsets. That
   is the brief's "land the small siblings in your own unit first" paying off
   for the second time in this unit -- make it eight for eight.

   FACTS MEASURED ON THIS BODY, both spellings priced, all at 54 words:

     the gate as `*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14)'     0   kept
       ... as `*(s16 *)(p + 0x14) < *(s16 *)(p + 0x10)'        2
     the three velocity adds as `b->unk_00 += b->unk_0C'       0   kept
       ... as plain `*(s32 *)' casts off `b'                   0   free

   The gate's 2 rows are the LOAD ORDER, not the compare: both spellings emit
   `slt $v0, [0x14], [0x10]', and only the kept one loads +0x10 into $v1
   before +0x14 into $v0, which is the order the target has. A comparison
   whose two sides are both memory is a statement about which side the source
   names FIRST, and it is worth reading off the listing rather than deriving.
   Same fact, same price, same direction as func_s28_800250E8's case 0 gate. */
void func_s28_8002553C(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    func_800478B8(c);
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x300;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    if (*(s16 *)(p + 0x10) > *(s16 *)(p + 0x14))
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that carries this bank's flyer trail: a renderable
   dressed from D_80025EE4 at white 0x80, the 0x7E00 sprite with the +0x14
   0x100 bit set, a random starting rotation and 0x400 scale on both axes,
   func_80045C34 as the per-slot walk, func_s28_8002553C as the per-frame
   step, the caller's record at the node's +0x20 with the caller's +0x12
   budget copied to +0x34, and the caller's mover copied wholesale into the
   node's own. Returns the node, or NULL when the pool is empty.
   73 words, 0 differing.

   DONORS, two, and NEITHER is a dngtwin hit (this body is not in LEADS.txt at
   all): the skeleton is the landed func_s13_8002421C in csrc/ovl_dng_s13.c --
   the same 0x212 spawn one bank over, 69 words -- and the one thing it does
   not have came from func_800C4944 in csrc/ovl_dungeon_800C2824.c, whose
   comment names `q = (u8 *)n + 0x20' as a node-payload view. That is the
   whole of this body's fight.

   THE PAYLOAD VIEW IS THE ONLY LOAD-BEARING SPELLING, and it is worth the
   frame. Priced from the 0 state:

     `q = (u8 *)n + 0x20;' with the +0x30/+0x34 stores
     written `*(s16 *)(q + 0x10)' / `*(s16 *)(q + 0x14)'    73 w,  0   kept
       ... both named off `n' as +0x30 and +0x34            72 w, 10
     the mover copy as `*n->unk_08 = *b;'                   73 w,  0   kept
       ... six field stores through `n->unk_08->unk_NN'     83 w, 27
     `m[0xE] = m[0xD] = m[0xC] = 0x80' DESCENDING           73 w,  0   kept
       ... ascending 0xC / 0xD / 0xE                        73 w,  4
     `*(s16 *)(m + 0x1E) = 0x400;' BEFORE the +0x1C store   73 w,  0   kept
       ... the other order                                  73 w,  4
     the NULL return written as the function's LAST line    73 w,  0   kept
       ... an early `if (n == NULL) return NULL;'           72 w,  3
     a separate `u8 *m' for the renderable cursor           73 w,  0   kept
       ... reusing the parameter `p' as the cursor          73 w,  0   free
     `q' assigned ahead of the step-pointer store           73 w,  0   kept
       ... after it, after the +0x20 store, or all three
           payload stores named off `q' instead of two      73 w,  0   free

   THE PARAMETER-REUSE FACT IS REAL BUT CONJOINED, and the honest reading is
   that `q' subsumes it. On the FIRST compile, with a separate `m' and no `q',
   this was 74 words / 40 rows and the fault was a FOURTH callee-saved
   register: the target saves s0/s1/s2 only, so there are three long-lived
   values (p, b, n) and `m' must share p's register once p dies. Reusing the
   parameter as the cursor -- the brief's own free fix -- took it to 72 words /
   10 rows and fixed the frame. But once `q' is in place the coalescing
   happens by itself and the separate `m' is byte-identical, so the kept body
   uses the natural `m'. Price a frame fault against the OTHER facts still
   missing before writing it down as the mechanism; this one would have been
   recorded as load-bearing by any run that stopped at the first 0.

   The brief's own `-da'/allocno route was not needed for the frame fault and
   would have misled: counting saves against the parameter list is what named
   it, in one reading of the prologue, which is what the brief's "saves minus
   parameters is the number of long-lived locals" line is for.

   SIGNATURE NOTES. func_80069EF8 is `rand' (splat/common_syms.txt), so no
   declaration and no address. func_80045C34's landed definition is in
   csrc/unk_19434.c and its real `s32 (u8 *, UnkVec *, UnkSlot *)' prototype
   is used rather than an invented one, with the `(u32)' cast at the
   func_8004491C site that the rest of the tree uses. D_80025EE4 is this
   object's OWN rodata at 0x80025EE4 (splat's
   dungeon.bin_undefined_syms_auto.txt), declared `extern u8 []' the way
   csrc/ovl_dng_s25.c declares its in-unit D_80025238. */
UnkNode *func_s28_80025614(u8 *p, UnkVec *b)
{
    UnkNode *n;
    u8 *q;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_s28_8002553C;
        *(u8 **)q = p;
        *(s16 *)(q + 0x10) = 0;
        *(s16 *)(q + 0x14) = *(u16 *)(p + 0x12);
        m = (u8 *)n->unk_0C;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        *(s16 *)(m + 0x12) = 0x7E00;
        *(u16 *)(m + 0x14) |= 0x100;
        func_8003DB94(m, D_80025EE4, 0);
        *(s16 *)(m + 0x1A) = rand() % 0x1000;
        *(s16 *)(m + 0x1E) = 0x400;
        *(s16 *)(m + 0x1C) = 0x400;
        func_8004491C(n, (u32)func_80045C34);
        *n->unk_08 = *b;
        return n;
    }
    return NULL;
}

/* This bank's root step for the lock-on flyer: bump the +0x10 frame counter,
   then dispatch on the +0xA state. State 0 seeds the heading out of the
   actor's +0x2A and whitens the renderable; state 1 re-reads the slot through
   func_8003DE58, publishes the mover's position, and then AIMS -- at the
   locked-on target's record if the actor has one at +0x60, otherwise by
   walking up to eight cells along the heading with func_800A44E0 and
   func_800BCB04 and aiming at the last cell that admitted it. Either way the
   per-axis velocity is the span over a /16 step count floored at 1, it hands
   off to func_s28_800253D0 or func_s28_80025614, and states 2 and 6 integrate
   that velocity until the step count runs out. States 3, 4 and 5 are the
   timed teardown, with state 4 calling this unit's own func_s28_80024024 and
   state 5 clearing D_8008346C and retiring the actor.
   449 words, 0 differing.

   OWNS jtbl_s28_80024008: 7 words, 7 cases, NO pad, dispatch bound
   `sltiu $v0, $v1, 0x7' -- so all 7 entries are real arms and none is a
   default pad. Entries in order: 0 .Ls28_800257A8, 1 .Ls28_800257D4,
   2 .Ls28_80025CD0, 3 .Ls28_80025D28, 4 .Ls28_80025D44, 5 .Ls28_80025D7C,
   6 .Ls28_80025DB4. All 7 distinct, their addresses ascend, so the arms are
   written ascending 0..6; case 0 falls through into case 1. The range-check
   `beqz $v0, .Ls28_80025E04' lands on the function's COMMON TAIL (the
   `*(s16 *)(p + 0x14) = 0' after the switch), which is not a table entry, so
   there is no default label to write -- confirmed by the grep the brief asks
   for, and an explicit `default: break;' measures 449 / 0, i.e. free.

   DONOR, AND THE PROLOGUE PICKED IT OVER EVERY RATIO. dngtwin's three leads
   are func_s14_80024AE4 (0.694), func_s15_80024CD4 (0.683) and
   func_s13_80024C88 (0.654) and ALL THREE have a -0x48 frame. This body's is
   -0x60 with s0..s7/fp/ra and a 40-byte local area, which is byte-for-byte
   func_s25_800247D4's in csrc/ovl_dng_s25.c -- 656 words, eight states, the
   same walk loop, and the body the CALLEE-SET INTERSECTION names at 8 of 8
   symbols with a clean gap to the next tier. It is 207 words larger than this
   one and no ratio at any floor can see that pair. The shape came from s25;
   the last 5 rows came from func_s13_80024C88's table, which is the same root
   step in a third bank and prices the facts a second time.

   WHAT THIS COPY DROPS relative to s25: the two `% 0x1000' angle bumps in the
   head, state 0's +0x20 clear and its ten-slot clearing loop, the whole
   D_800DDC40 height correction (this bank aims at `*(u8 **)(s + 0x60)'s own
   +0x88 instead), the child-spawn state and the four retire sites. What it
   ADDS: `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60)', the `+ 0x20' on the
   aim point's +0xA, and the lift of the whole velocity to 32-bit `UnkVec'
   field arithmetic in states 1, 2 and 6 where s25 works in 16-bit halves.

   THE DEAD `lui $v0, (0x80070000 >> 16)' at 80025A0C IS NOT SOURCE. 0x8007 is
   %hi(D_8006CCD8); it is the walk loop's hoisted table high left behind once
   the loop body's uses were rematerialised as full `lui'/`addiu' pairs, and
   $v0 is overwritten by the loop's first instruction. func_s13_80024C88
   records the same artefact at its own 80024FAC and the same conclusion.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced
   (aligned differences; words given because several spellings change them):

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | ONE `UnkVec *v' shared by the lock-on and the walk arm    |  449  |   0  |
   |   ... a separate `u8 *g' in the lock-on arm               |  449  |   5  |
   | `(cx << 6) & 0xFFFFFFC0' into func_800A44E0               |  449  |   0  |
   |   ... `& 0xFFC0' against the same u16 prototype           |  452  |  61  |
   | `ox * 64' / `oy * 64' into the aim point                  |  449  |   0  |
   |   ... `(ox << 6)' / `(oy << 6)'                           |  447  |  79  |
   | the aim point's +0xA as `*(u16 *)(b + 0xA) + 0x20'        |  449  |   0  |
   |   ... without the `+ 0x20'                                |  448  |  58  |
   | `*(s32 *)(p + 0x18) = (s32)*(u8 **)(s + 0x60);'           |  449  |   0  |
   |   ... the statement absent                                |  446  |  61  |
   | case 6's `p+0xA = 5' BEFORE `p+0x10 = 0'                  |  449  |   0  |
   |   ... the other order                                     |  454  |  20  |
   | `u16 z' for w+0xA across state 1's flag gate              |  449  |   0  |
   |   ... naming `*(u16 *)(w + 0xA)' in both arms             |  451  |   7  |
   | state 1's `p+0x10 = 0' ONCE after the if/else             |  449  |   0  |
   |   ... duplicated into both arms                           |  448  |   6  |
   | the arm-B `i = 1' hoisted ahead of the three vv stores    |  449  |   0  |
   |   ... left in the `for' init                              |  449  |   2  |
   | `(D_8006CCD8[dir] + 1) * 32'                              |  449  |   0  |
   |   ... `((D_8006CCD8[dir] + 1) << 5)'                      |  449  |   2  |
   | func_s28_80024024 reached through a THREE-argument alias  |  449  |   0  |
   |   ... the landed two-argument name at that site           |  449  |   1  |

   THE TWIN'S DEVICE IS FREE HERE AND THE TWO FACTS ARE CONJOINED, which is
   the thing to carry forward. s25 prices `do { v = &vv; } while (0);' at 5
   rows and explains it as a scheduling barrier that lets reorg steal
   `addiu $a3, $sp, 0x10' into the `beqz .Ls28_80025B3C' delay slot. On the
   FIRST compile here, with a separate `u8 *g', the residue was exactly 10
   rows: 5 for the barrier and 5 for `g' holding $a2 where the target holds
   $a3. Adding the wrapper took it to 5; sharing ONE `v' across both arms took
   it to 0 -- and with the shared `v' the wrapper is worth NOTHING (449 / 0
   with it, without it, and with `if (1)'). So the shared allocno subsumes the
   barrier: `v' picks up the lock-on arm's set and four references, which is
   what gives it $a3 and what gives the pointer assignment a scheduling anchor
   of its own. The device is NOT in the landed body. s13 prices the same
   shared-local fact at 9 rows on its body and reaches the same conclusion by
   a different route ($s0 rather than $a3), so this is three bodies agreeing.

   WHERE THE DONOR'S PRICES DIVERGE FROM MINE, all in the same direction --
   this copy is far more sensitive to the walk block's arithmetic than either
   of the other two banks: `& 0xFFFFFFC0' is 61 rows here against s25's 7 and
   s13's 7, and `* 64' is 79 against s25's 29 and s13's 7. Both are SELECTION
   facts in the brief's taxonomy and both HOLD in sign; it is only the
   magnitude that does not transfer, because this body's 32-bit velocity
   arithmetic puts the whole aim block downstream of the mask's `andi' pair.
   Treat a donor's magnitude as a lead and its SIGN as the fact.

   MEASURED AND FREE, so none of these is why this lands: `oy = yy; ox = xx;'
   at the bottom of the walk loop in either order (s25 records it free,
   s13 records its own order at 21 rows -- s25's reading is the one that
   reproduces here); `(u16)D_8006CCD8[dir]' for `D_8006CCD8[dir] & 0xFFFF';
   states 2 and 6's three adds as plain casts off `b' rather than
   `b->unk_00 += b->unk_0C'; cases 3 and 4's two stores in either order;
   case 5's `*(u16 *)(p - 0x2) |= 0x8000' through an `s16 *' instead;
   `(s16)h > 0x200' for `>= 0x201'; an explicit `default: break;'; and BOTH
   declaration-order permutations tried (`v' ahead of `w', and `ox'/`oy'
   exchanged around `out[4]'), which rules out the pseudo-number tie-break
   and leaves the statement order as the whole of the ordering facts above. */
void func_s28_80025738(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q4;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
    UnkVec vv;
    u16 out[4];
    s16 oy;

    s = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(s + 0x2A) >> 9) & 7;
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
        if (*(u8 **)(s + 0x60) != NULL)
        {
            *(s32 *)(p + 0x18) = (s32) * (u8 **)(s + 0x60);
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (i = 1; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = ((*(s16 *)(*(u8 **)(s + 0x60) + 0x88) << 16) - b->unk_08) /
                        *(s16 *)(p + 0x12);
            func_s28_800253D0(p, b);
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        else
        {
            q4 = *(u8 **)(o + 0xC);
            xx = q4[0x24];
            yy = q4[0x25];
            ox = xx;
            oy = yy;
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
                oy = yy;
                ox = xx;
            }
            v = &vv;
            i = 1;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + (D_8006CCD8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + (D_8006CCE8[*(s16 *)(p + 0xE)] + 1) * 32;
            *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)b + 0xA));
            *(s16 *)(p + 0x12) = out[0];
            for (; i < 3; i++)
            {
                if ((s16)out[i] > *(s16 *)(p + 0x12))
                {
                    *(s16 *)(p + 0x12) = out[i];
                }
            }
            *(s16 *)(p + 0x12) = (s16) * (u16 *)(p + 0x12) >> 4;
            if (*(s16 *)(p + 0x12) == 0)
            {
                *(s16 *)(p + 0x12) = 1;
            }
            b->unk_0C = (v->unk_00 - b->unk_00) / *(s16 *)(p + 0x12);
            b->unk_10 = (v->unk_04 - b->unk_04) / *(s16 *)(p + 0x12);
            b->unk_14 = (v->unk_08 - b->unk_08) / *(s16 *)(p + 0x12);
            func_s28_80025614(p, b);
            *(s16 *)(p + 0xA) = 6;
        }
        *(s16 *)(p + 0x10) = 0;
        break;
    case 2:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            func_800A56E0(0x300);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x10)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x30)
        {
            func_s28_80024024_3(*(u8 **)(s + 0x60), p[0x9], s);
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
    case 6:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        if (*(s16 *)(p + 0x10) >= *(s16 *)(p + 0x12))
        {
            *(s16 *)(p + 0xA) = 5;
            *(s16 *)(p + 0x10) = 0;
        }
        break;
    }
    *(s16 *)(p + 0x14) = 0;
}
