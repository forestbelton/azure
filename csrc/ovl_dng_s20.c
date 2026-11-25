#include "common.h"

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
typedef struct UnkDng24EF4SP
{
    u8 pad_00[0x18];
    u8 *unk_18;
    u8 pad_1C[0x4];
    u8 *unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
} UnkDng24EF4SP;
typedef struct UnkDng26478
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng26478;
extern UnkDng26478 D_80026478[];
extern u32 D_80026470;
extern u32 D_80026474;
extern s16 D_s20_8002632A;
void func_80067F20(u32 *, s32, s32, s32, s32);

typedef struct UnkDng255E8Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng255E8Vec;
typedef struct UnkDng255E8SP
{
    u8 pad_00[0x70];
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng255E8Vec unk_98[4];
} UnkDng255E8SP;
typedef struct UnkDng24ED0SP
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
} UnkDng24ED0SP;
typedef struct UnkDng265C8
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDng265C8;
UnkNode *func_8003FC64(s32);
s32 func_800CEEFC();
void func_s20_80024D70(u8 *p, UnkVec *v, u8 *c);

extern u16 D_s20_80026326;

extern u8 D_800E1D01[];
void func_80099844(u8 *, u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");

extern u8 D_800263F8[];
extern u8 D_80045340[];
void func_8003DB94(void *, void *, s32);
void func_s20_80025868(u8 *p, UnkVec *v, u8 *c);

void func_800478B8(u8 *);

UnkNode *func_s20_8002569C(u8 *p, s16 ry, s16 rx, s16 rz, void *tmpl);
void func_s20_80025160(u8 *p, UnkVec *v, u8 *c);
extern u16 D_80026878;
UnkNode *func_8003FD64(s32, void *);
s32 func_s20_80025340(u8 *p, UnkVec *v, UnkSlot *s);

extern u8 D_80026344[];
extern u8 D_s20_8002632C[];
extern s16 D_s20_80026324;
extern s16 D_s20_80026328;
extern s16 D_s20_80026326_i __asm__("D_s20_80026326");
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
void func_800B835C(u8 *, s32 *, s32, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_8004491C(UnkNode *, u32);
s32 func_800644B8(s32);
s32 func_80065F90(s32, s32);
s32 func_8009FCF0w(u8, u8, u8, u8) __asm__("func_8009FCF0");
s32 func_800A45D8w(s32, s32, s32) __asm__("func_800A45D8");
s32 func_800A56E0(s32);
UnkNode *func_s20_80024DE8(u8 *v, u8 *q);
UnkNode *func_s20_80024C80(u8 *p, UnkVec *v, s16 z, u8 *t);
void func_s20_800262B8_3(u8 *, u8, u8 *) __asm__("func_s20_800262B8");
/* ARITY NOT WITNESSED.  func_s20_80024804 is referenced only as
   `(u32)func_s20_80024804' -- parked as a node's +0x10 step -- so no call site
   constrains its signature and no codegen here can depend on it.  Its prologue
   reads $a0 and $a1 only ($a2/$a3 are written-only throughout).  This is the
   unit's +0x10-step shape, guessed from what every landed step handler here
   takes and what the dispatcher calls the slot with.  WHOEVER LANDS IT MUST
   RE-DERIVE THIS LINE rather than trust it. */
void func_s20_80024804(u8 *p, UnkVec *v, u8 *c);
s32 func_s20_80024EF4(void);

/* The master step handler of this bank's spell script, and IT OWNS A JUMP
   TABLE.  jtbl_s20_80024008: 18 `.word's, dispatch bound `sltiu $v0, $v1,
   0x12' with no minval subtraction, 8 distinct targets.  The entry list, in
   table order:

     case 0  .Ls20_800240C8   case 9  .Ls20_800247C8 (default)
     case 1  .Ls20_80024160   case 10 .Ls20_800247C8 (default)
     case 2  .Ls20_800243C8   case 11 .Ls20_800247C8 (default)
     case 3  .Ls20_80024640   case 12 .Ls20_800247C8 (default)
     case 4  .Ls20_8002467C   case 13 .Ls20_800247C8 (default)
     case 5  .Ls20_800247C8 (default)   case 14 .Ls20_800247C8 (default)
     case 6  .Ls20_800247C8 (default)   case 15 .Ls20_800247C8 (default)
     case 7  .Ls20_800247C8 (default)   case 16 .Ls20_800246F0
     case 8  .Ls20_800247C8 (default)   case 17 .Ls20_80024790

   ELEVEN of the eighteen entries carry the DEFAULT label -- the same
   .Ls20_800247C8 that the range-check `beqz $v0' at 800240A0 jumps to -- so
   there are SEVEN real arms, written 0, 1, 2, 3, 4, 16, 17 with no
   `default:', and cases 5..15 are gcc's pad to maxval and NOT empty `break'
   arms.  Writing them out would be eleven phantom labels.  The arms' entry
   addresses are already ascending in case order, case 0 falls through into
   case 1 and case 3 into case 4.  18 is an EVEN `.word' count with NO pad
   word, because the case count is 18 as well -- read the `sltiu' bound, not
   the word count.

   WHAT IT IS.  `p' is this node's payload, `o = *(u8 **)p' the caster's
   payload, `n' the caster's node (`o - 0x20') and `m' its mover.  0
   publishes the ring buffer through func_800B835C, initialises the
   renderable and this bank's four counters and falls into 1; 1 reads the
   caster's hand through func_8003DF74, mirrors its position into the mover,
   tags the node with D_80045340, picks the target tile off the caster's
   +0x60 target (or its own mover when there is none) and seeds the tile-walk
   step count and the D_8006CCD8/E8 facing pair; 2 integrates and clamps the
   mover's two horizontal velocities at +-0x200000, eases z, spins by
   func_800644B8, spawns a func_s20_80024DE8 marker a frame, plays the
   proximity cue once func_8009FCF0 says the caster is within 5, and either
   arrives (state 16) or steps one tile when func_800A45D8 says the next tile
   is clear; 3 rings func_80065F90 and spawns the func_s20_80024C80 trail
   ring; 4 fades the renderable and hands off at state 0x11 once the ring
   dies; 16 drifts and fades; 17 waits for D_s20_80026326 to empty.

   TWO TWINS, AND IT NEEDED BOTH.  func_s21_80024050 (dngtwin 0.930, also
   CALLEE.txt's 14-of-23 best overlap with a clean gap to 10/23) is the
   skeleton: byte-identical prologue, -0x40 frame, same six saves in the same
   order, so every one of its facts is in scope.  func_s19_80024050 (0.877)
   supplied the one block s21 has not got -- case 2's `if (*(s16 *)(p + 0x58)
   == 0) { if ((s16)func_8009FCF0w(...) < 5) ... }' proximity gate nested
   inside the `if (*(u8 **)(o + 0x60) != NULL)' whose SECOND arm re-tests the
   same pointer -- and its `func_8009FCF0w(u8, u8, u8, u8)' / `func_800A45D8w
   (s32, s32, s32)' s32 aliases.  s21's case-3 `func_800A56E0(0x300)' moved
   into case 2 here; s21's case-4 kill tail is gone, case 4 only parks state
   0x11, and the kill lives in case 17 alone.

   FIRST COMPILE, so the nine rows below were measured AFTER the fact by
   flipping each spelling on a body already at 0.  All figures are aligndiff
   against this body at 488 words / 0:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | case 0's arr[] fill and the func_800B835C call written |       |      |
   |   FIRST, before the renderable and bank stores  KEPT   |  488  |   0  |
   |   ... the s19/s21 order, stores first                  |  489  |  21  |
   | case 0's +0x16 facing read as an ARRAY REF,            |       |      |
   |   `((u16 *)o)[0x15]'                            KEPT   |  488  |   0  |
   |   ... `*(u16 *)(o + 0x2A)', a plain cast               |  488  |   4  |
   | case 1's two join stores DUPLICATED INTO BOTH ARMS,    |       |      |
   |   +0xA then +0x2E, with A LOCAL PER ARM         KEPT   |  488  |   0  |
   |   ... duplicated but sharing one local                 |  488  |   3  |
   |   ... written ONCE after the if/else, one local        |  487  |   9  |
   | case 1's D_800DDC40 index read through a SECOND inline |       |      |
   |   `*(u8 **)(o + 0x60)'                          KEPT   |  488  |   0  |
   |   ... that pointer hoisted into a local                |  486  |   9  |
   | case 2's D_80026878 store written BEFORE the +0xA      |       |      |
   |   bump                                          KEPT   |  488  |   0  |
   |   ... after it                                         |  490  |  12  |
   | case 2's +0xA bump as an ARRAY REF,                    |       |      |
   |   `((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1'       KEPT   |  488  |   0  |
   |   ... `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1'      |  486  |  12  |
   | `(s16)' on func_800A45D8w's s32 result          KEPT   |  488  |   0  |
   |   ... no cast                                          |  487  |   6  |
   | `(s16)' on func_8009FCF0w's s32 result          KEPT   |  488  |   0  |
   |   ... no cast                                          |  486  |   8  |

   SIX OF THESE REPRODUCE A TWIN'S ROW AND ONE DOES NOT.  s21 prices the two
   join-store alternatives at 3 and 9 and this body gives 3 and 9 exactly;
   its D_800269C8-before-the-bump row is 11 against my 12 and its array-ref
   bump row is 11 against my 12.  Its case-0 array-ref row is 16 here and
   only 4, which is the weak row in that otherwise exact table: the twin's
   `lhu' hoists over TWO D_80026484 stores where this bank's case 0 has only
   the one D_80026474 store for it to hop, so the displacement wake is
   shorter.  The mechanism (`true_dependence', sched.c:817 -- an ARRAY_REF
   sets MEM_IN_STRUCT_P at a varying address, which is exempt against a plain
   cast at a SYMBOL address, so sched1 may hoist the load above the store) is
   the same in both; only the number of displaced words differs.

   THE 21-ROW ROW IS THIS BANK'S OWN, AND IT IS NOT AN ALLOCATION FACT.  A
   call is a barrier for the stores, so case 0's emitted order IS its source
   order: the two `sw' to the 0x18/0x1C argument block sit in and beside the
   `jal func_800B835C' delay slot at target words 22..24, ahead of every
   `c + ...' store.  Both twins publish after the stores.

   NOT MEASURABLE: which of cases 5..15 the table's eleven shared entries
   name (all are the default), and the `&&'-versus-nested spelling of case
   2's `*(u8 **)(o + 0x60)' re-test, whose arms are identical after
   cross-joining.

   SIGNATURES.  func_8009FCF0 and func_800A45D8 are declared `s16'-returning
   in csrc/ovl_dungeon_8009F644.c and csrc/ovl_dungeon_800A4B88.c; this body
   needs the `sll 16'/`sra 16' of an s32 result cast to s16 at both sites, so
   it carries func_s19_80024050's unit-local s32 aliases and leaves both
   landed declarations alone.  func_800A45D8w's first two parameters must be
   `s32' and not the landed `u16' so that `& 0xFFE0' stays SImode.
   func_s20_800262B8 is called with THREE arguments here while its landed
   definition lower in this file takes two, so this unit carries a
   three-argument alias for that one site.  func_s20_80024C80 takes FOUR
   arguments -- its own listing opens `addu $s5, $a3, $zero' -- and its third
   is never read.

   488 words, 0 differing; relcand ok (15 data symbols, 13 calls). */
void func_s20_80024064(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    s32 arr[2];
    UnkNode *n;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *u;
    s32 a;
    s32 b;
    s32 w;
    s32 a2;
    s32 b2;
    s32 w2;
    s32 gx;
    s32 gy;
    s32 z;
    s32 z2;
    s32 f;

    o = *(u8 **)p;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_s20_8002632C, arr, 1, 0);
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x555;
        *(s16 *)(c + 0x1C) = 0x555;
        *(u32 *)(c + 0x8) = (u32)D_80026344;
        D_s20_80026324 = 0;
        D_s20_80026328 = 0;
        D_s20_8002632A = 0;
        D_80026470 = 0;
        D_80026474 = 0;
        *(s16 *)(p + 0x16) = (((u16 *)o)[0x15] >> 9) & 0x7;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 1:
        if (func_8003DF74((u8 *)((UnkSlot *)n->unk_0C)->unk_08, (UnkSlot *)n->unk_0C, (s16 *)out,
                          0) == NULL)
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
            *(s16 *)((u8 *)v + 0xA) = z;
            *(s16 *)(p + 0x2E) = z;
        }
        else
        {
            z2 = *(u16 *)(m + 0xA) + out[2];
            *(s16 *)((u8 *)v + 0xA) = z2;
            *(s16 *)(p + 0x2E) = z2;
        }
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
        if (*(u8 **)(o + 0x60) != NULL)
        {
            u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
            *(s16 *)(p + 0xC) = *(u16 *)(u + 0x2);
            *(s16 *)(p + 0xE) = *(u16 *)(u + 0x6);
            *(s16 *)(p + 0x10) = *(u16 *)(u + 0xA) - D_800DDC40[(*(u8 **)(o + 0x60))[0x13]];
            e = *(u8 **)(o - 0x14);
            p[0x20] = p[0x1E] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x16)];
            p[0x21] = p[0x1F] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x16)];
            *(s16 *)(p + 0x14) = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24]
                                                   ? *(s8 *)(o + 0x73) - e[0x25]
                                                   : *(s8 *)(o + 0x72) - e[0x24]) +
                                 1;
        }
        else
        {
            *(s16 *)(p + 0x14) = 0x8;
            *(s16 *)(p + 0xC) = *(u16 *)((u8 *)v + 0x2);
            *(s16 *)(p + 0xE) = *(u16 *)((u8 *)v + 0x6);
            *(s16 *)(p + 0x10) = *(u16 *)(o + 0x88) - 0x50;
        }
        *(s16 *)((u8 *)v + 0xE) = D_8006CCD8[*(s16 *)(p + 0x16)] * 8;
        *(s16 *)((u8 *)v + 0x12) = D_8006CCE8[*(s16 *)(p + 0x16)] * 8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        v->unk_00 = v->unk_00 + v->unk_0C;
        a = v->unk_0C + (v->unk_0C >> 4);
        v->unk_0C = a;
        b = __builtin_abs(a);
        if (b > 0x200000)
        {
            w = (a > 0) ? 0x200000 : -0x200000;
            v->unk_0C = w;
        }
        v->unk_04 = v->unk_04 + v->unk_10;
        a2 = v->unk_10 + (v->unk_10 >> 4);
        v->unk_10 = a2;
        b2 = __builtin_abs(a2);
        if (b2 > 0x200000)
        {
            w2 = (a2 > 0) ? 0x200000 : -0x200000;
            v->unk_10 = w2;
        }
        *(s32 *)(p + 0x2C) += ((*(s16 *)(p + 0x10) << 16) - *(s32 *)(p + 0x2C)) >> 4;
        v->unk_08 += ((*(s16 *)(p + 0x10) << 16) - v->unk_08) >> 4;
        v->unk_08 += func_800644B8(*(s16 *)(p + 0x18) << 7) << 6;
        func_s20_80024DE8((u8 *)v, c);
        p[0x1E] = *(s16 *)((u8 *)v + 0x2) / 0x40;
        p[0x1F] = *(s16 *)((u8 *)v + 0x6) / 0x40;
        if (*(s16 *)(p + 0x20) == *(s16 *)(p + 0x1E))
        {
            break;
        }
        if (*(u8 **)(o + 0x60) != NULL)
        {
            if (*(s16 *)(p + 0x58) == 0)
            {
                if ((s16)func_8009FCF0w(p[0x1E], p[0x1F], o[0x72], o[0x73]) < 5)
                {
                    *(s16 *)(p + 0x58) = 1;
                    func_800A56E0(0x300);
                }
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
                func_s20_80024DE8((u8 *)v, c);
                D_80026878 = *(u16 *)(*(u8 **)(o + 0x60) + 0x88);
                ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
                break;
            }
        }
        p[0x20] = p[0x1E];
        p[0x21] = p[0x1F];
        if (--*(s16 *)(p + 0x14) != 0)
        {
            if ((s16)func_800A45D8w(((*(s8 *)(p + 0x1E) << 6) + 0x20) & 0xFFE0,
                                    ((*(s8 *)(p + 0x1F) << 6) + 0x20) & 0xFFE0,
                                    *(s16 *)(p + 0x2E)) == 0)
            {
                break;
            }
        }
        *(s16 *)(p + 0xA) = 0x10;
        break;
    case 3:
        func_80065F90(*(s16 *)((u8 *)v + 0xE), *(s16 *)((u8 *)v + 0x12));
        *(UnkNode **)(p + 0x28) =
            func_s20_80024C80(p, v, *(s16 *)(*(u8 **)(o + 0x60) + 0x88), *(u8 **)(o + 0x60));
        if (*(UnkNode **)(p + 0x28) == NULL)
        {
            break;
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 4:
        c[0xC] -= c[0xC] >> 2;
        c[0xD] -= c[0xD] >> 2;
        c[0xE] -= c[0xE] >> 2;
        if (((*(UnkNode **)(p + 0x28))->unk_1E & 0x8000) == 0)
        {
            break;
        }
        if (*(u8 **)(o + 0x60) != NULL)
        {
            func_s20_800262B8_3(*(u8 **)(o + 0x60), p[0x9], o);
        }
        *(s16 *)(p + 0xA) = 0x11;
        break;
    case 16:
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += ((*(s16 *)(p + 0x10) << 16) - v->unk_08) >> 4;
        c[0xC] -= c[0xC] >> 1;
        c[0xD] -= c[0xD] >> 1;
        c[0xE] -= c[0xE] >> 1;
        func_s20_80024DE8((u8 *)v, c);
        if (c[0xC] < 2)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 17:
        if (D_s20_80026326_i != 0)
        {
            break;
        }
        D_8008346C = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
    f = *(u16 *)(p + 0x18);
    D_s20_80026326 = 0;
    *(s16 *)(p + 0x18) = f + 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80024804);

/* The 0x202 spawner for the ring of sixteen trail nodes, called from case 3
   of func_s20_80024064 above: take a node off func_8003FD64 with this
   payload's own node as the template, bump this bank's D_s20_80026324 live
   count, park func_s20_80024804 as the new node's +0x10 step and
   func_s20_80024EF4 through func_8004491C as its draw, copy the caller's
   mover position into the new mover's +0x2/+0x6/+0xA, stash the fourth
   argument in the node's +0x20, then call func_s20_8002569C sixteen times
   with the ring angle `i << 8', a 0x40 radius and a -0x100 z.  The node is
   returned.

   THE THIRD PARAMETER IS NEVER READ -- `$a2' is dead on entry and the first
   thing to touch it is `lui $a2, %hi(D_s20_80026324)' -- but it has to be
   declared, because `$a3' IS read.  func_s20_80024064's case 3 passes
   `*(s16 *)(*(u8 **)(o + 0x60) + 0x88)' there.

   TWIN func_s21_80024804 (dngtwin 0.948), which is this body MINUS the
   D_s20_80026324 bump: byte-identical for all 56 of its words, same -0x38
   frame, same six saves in the same order, so its ONE fact is a BUDGET fact
   with a matching frame and it carries unchanged.  That fact is the brief's
   parameter-reuse lever run in reverse: the extra word `addu $s3, $s4,
   $zero' in the `lhu 0xA($s3)' load-delay slot is a copy of the `p - 0x20'
   template into the register `v' has just vacated, and gcc 2.7.2 has no
   coalescing between two global pseudos, so the copy survives only if the
   source assigns one variable from another -- and it must be the PARAMETER's
   own pseudo, since a fresh local is folded away.  The twin prices all three
   alternatives (a separate local in either position, or `d' passed directly)
   at 54 words / 54 rows, two words short because the frame drops to five
   saves at -0x30.  Not re-priced here: the twin's rows are from a body at 0
   and the construct is identical.

   THE ONE THING THIS BODY HAD TO FIND is the bump's POSITION.  Written after
   the `q->unk_10 = func_s20_80024804' store it is 60 words / 2 aligned rows,
   and both rows are the two `lui's exchanged -- the target loads
   %hi(D_s20_80026324) into $a2 before %hi(func_s20_80024804) into $v0.  A
   `symbol_ref' costs no insn at expand time, so the order the two addresses
   are forced into registers is the order the source mentions them, and
   writing `D_s20_80026324++;' FIRST is 60 / 0.  The emitted order then puts
   the `lhu' before the `sw' anyway, which is sched1 taking the
   `true_dependence' exemption for a fixed-address scalar against the
   in-struct `q->unk_10'.

   60 words, 0 differing; relcand ok (1 data symbol, 3 calls). */
UnkNode *func_s20_80024C80(u8 *p, UnkVec *v, s16 z, u8 *t)
{
    UnkNode *d;
    UnkNode *q;
    UnkVec *r;
    s32 i;

    d = (UnkNode *)(p - 0x20);
    q = func_8003FD64(0x2, d);
    if (q != NULL)
    {
        D_s20_80026324++;
        q->unk_10 = (u32)func_s20_80024804;
        func_8004491C(q, (u32)func_s20_80024EF4);
        r = q->unk_08;
        *(s16 *)((u8 *)r + 0x2) = *(u16 *)((u8 *)v + 0x2);
        *(s16 *)((u8 *)r + 0x6) = *(u16 *)((u8 *)v + 0x6);
        *(s16 *)((u8 *)r + 0xA) = *(u16 *)((u8 *)v + 0xA);
        v = (UnkVec *)d;
        *(u32 *)((u8 *)q + 0x20) = (u32)t;
        for (i = 0; i < 0x10; i++)
        {
            func_s20_8002569C((u8 *)r, i << 8, 0x40, -0x100, v);
        }
    }
    return q;
}

/* The +0x10 step of the 0x202 node func_s21_8002495C below spawns: bump the
   object's own u16 counter, fade the renderable's RGB triple at +0xC..+0xE by
   one eighth each, and raise the kill bit on the node flag at -0x2 and on
   D_800814A0 once the red channel drops under 8. csrc/ovl_dungeon_800CABA8.c's
   func_800CC744 is the closest resident shape -- the same two-arm fade and the
   same kill tail -- but this one has a single arm and the >> 3 in place of a
   division, so the body is written from the listing.

   The three fades are SEPARATE statements, not a chain: each reads its own
   byte, so there is no common value to carry. `>> 3' and not `/ 8': the
   bytes are u8 and the shift comes out `srl' with no sign correction. The
   guard reads the byte BACK (`lbu 0xC' a second time after the store), which
   is what `c[0xC] < 8' spelled on the array gives.

   30 words, 0 differing on the first compile. raw 6 == the 6 relocations:
   1 HI16 + 2 LO16 on D_s20_80026326 and the same three on D_800814A0. */
void func_s20_80024D70(u8 *p, UnkVec *v, u8 *c)
{
    D_s20_80026326++;
    c[0xC] -= c[0xC] >> 3;
    c[0xD] -= c[0xD] >> 3;
    c[0xE] -= c[0xE] >> 3;
    if (c[0xC] < 8)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s20_80024D70's fading marker.
   csrc/ovl_dungeon_800CABA8.c's func_800CC5F0 is the line-for-line template:
   the same func_8003FC64(0x202), the same `n->unk_10 = step' ahead of
   func_8004491C(n, (u32)func_800CEEFC), and the same mover-then-renderable
   fill. The difference is that nothing here is a literal -- the new node's
   mover is SEEDED FROM AN EXISTING ONE (`v') and its renderable's scale,
   flags, colour word and +0x8 pointer are COPIED FROM AN EXISTING ONE (`q'),
   which is why it returns the node.

   The three velocity halfwords at +0xE/+0x12/+0x16 are the source's position
   MINUS its own +0xE/+0x12/+0x16, and each re-reads the position halfword it
   already stored (two `lhu 0x2' in the stream) -- written out rather than
   carried in a local, which is what keeps the reload.

   Both parameters are `u8 *': the first is read at the mover's halfword
   offsets and the second at the renderable's, and neither is witnessed as a
   struct here beyond those offsets.

   67 words, 0 differing on the first compile. raw 6 == the 6 relocations:
   R_MIPS_26 on func_8003FC64 and func_8004491C, and HI16/LO16 pairs on
   func_s20_80024D70 and func_800CEEFC. */
UnkNode *func_s20_80024DE8(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s20_80024D70;
        func_8004491C(n, (u32)func_800CEEFC);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(v + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(v + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(v + 0xA);
        *(s16 *)(m + 0xE) = *(u16 *)(v + 0x2) - *(u16 *)(v + 0xE);
        *(s16 *)(m + 0x12) = *(u16 *)(v + 0x6) - *(u16 *)(v + 0x12);
        *(s16 *)(m + 0x16) = *(u16 *)(v + 0xA) - *(u16 *)(v + 0x16);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x1C) = *(u16 *)(q + 0x1C);
        *(s16 *)(r + 0x1E) = *(u16 *)(q + 0x1E);
        *(u32 *)(r + 0xC) = *(u32 *)(q + 0xC);
        *(u32 *)(r + 0x8) = *(u32 *)(q + 0x8);
        *(s16 *)(r + 0x14) = *(u16 *)(q + 0x14);
    }
    return n;
}

/* The trail ring's renderer: one semi-transparent POLY_G4 and one DR_MODE per
   live entry of D_80026478, each quad spanning entry `i', the next entry in
   its 0x10-wide ring, and the two matching entries of the second bank at
   &D_80026478[0x10].  155 words, 0 differing, FIRST COMPILE.

   Written against func_s20_80025340's landed body in this same unit (the
   D_80083160/+0x8D0 cursor prologue, the `sp->unk_18' before `sp->unk_20'
   order, the GetTPage/func_80067F20/AddPrim tail) plus
   func_s20_80026010's index arithmetic, so nothing here was fought for.

   Note the contrast with func_s20_80025340's trail-ring store, which needs an
   integer address and an explicit `<< 3': here the PLAIN ARRAY REF is right.
   `D_80026478[i + 0x10]' folding to `i * 8 + (base + 0x80)' is exactly the
   target's `addiu $a3, $s4, 0x80', and an array ref off a loop-invariant
   SYMBOL also gives the `addu $v0, $v1, $s4' operand order that an array ref
   off a pointer LOCAL does not.

   The two colour pairs are CHAINED assignments: `*(u32 *)&q->r0 =
   *(u32 *)&q->r1 = D_80026470' emits +0x0C before +0x04, which is the order
   the target has in both arms.  RotTransPers4 is RotTransPers4 and
   SetPolyG4 is SetPolyG4 -- a real call, not the setPolyG4 macro, which
   is why the tag and code bytes are written after the four colour words. */
s32 func_s20_80024EF4(void)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_G4 *q;
    DR_MODE *dm;
    s32 i;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < D_s20_8002632A; i++)
    {
        q = (POLY_G4 *)sp->unk_18;
        sp->unk_18 = (u8 *)(q + 1);
        *(u32 *)&sp->unk_70[0] = *(u32 *)&D_80026478[i];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&D_80026478[(i & -0x10) + ((i + 1) & 0xF)];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&D_80026478[i + 0x10];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&D_80026478[(i & -0x10) + ((i + 1) & 0xF) + 0x10];
        sp->unk_70[1].vz = sp->unk_70[0].vz = D_80026478[i].unk_04;
        sp->unk_70[3].vz = sp->unk_70[2].vz = D_80026478[i + 0x10].unk_04;
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            if (i % 0x20 < 0x10)
            {
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_80026470;
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_80026474;
            }
            else
            {
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_80026470;
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_80026474;
            }
            SetPolyG4(q);
            setSemiTrans(q, 1);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* The step handler of the 0x202 node func_s20_80024C80 above parks on
   func_s20_8002569C: a four-state machine on the payload's +0xA counter that
   ramps the renderable's +0x6 scale by 0x80 a frame.  State 0 ramps up, caps
   at 0x100, spawns one func_s20_8002569C trail node per frame while the
   facing at +0x22 is under 0x61, advances the state and raises
   D_s20_80026328 at facing 0x6F; state 1 waits for D_s20_80026328 and flips
   the scale to -0x100; state 2 counts +0x1A down and falls into state 3,
   which ramps the negative scale back toward 0 and kills the node when it
   reaches it.  Then every path writes the 0x20/0x40/0xE0 colour triple.
   It is a COMPARE CHAIN and not a table -- four case labels, and
   `expand_end_case' emits the balanced tree on 1 that the listing opens with
   -- so the arms are order-sensitive and they are written ascending, which
   is the order their blocks appear in.

   120 words, 0 differing, FIRST COMPILE off the 0.899 twin
   func_s21_80024CD4.  All four of that twin's stated facts carried and none
   is re-priced here, because its figures come from a body at 0 and the
   constructs are textually identical:

     * the colour triple is AFTER the `switch', not in each arm -- every arm
       reaches the same `sb $v0, 0xC($s1)';
     * CASE 2 FALLS THROUGH INTO CASE 3, with `if (--*(s16 *)(p + 0x1A) > 0)
       break;' as its guard -- the `bgtz' to the shared tail with the
       increment on the fall-through, and the two arms sharing one `sh 0x6';
     * the two func_s20_8002569C argument fields are stored +0x18 THEN +0x1C
       and read back in the same order: the +0x1C argument is cse-forwarded
       from its own store (`sll'/`sra' on the stored register) while +0x18 is
       a real `lh' reload, because the +0x1C store wipes cse's memory table
       between the +0x18 store and the read;
     * `>= 0x101' needs the full `sll 16'/`sra 16'/`slti' while state 3's
       `>= 0' is just `sll 16'/`bltz' -- both free from reading the stored
       halfword back.

   THE 7-WORD DEFICIT AGAINST THE TWIN IS TWO MISSING STATEMENTS, NOT MISSING
   REFERENCES: this bank has no trailing func_s21_800262AC spawn after the
   colour triple, which is why `v' can sit in $a0 from the prologue to the
   one call that uses it instead of needing a callee-saved register.  Its
   triple is 0x20/0x40/0xE0 where the twin's is 0x20/0xE0/0x40.

   The constant 1 in `$s2' across the whole body is cse sharing the `case 1:'
   comparison value with `D_s20_80026328 = 1;'; nothing in the source says
   so.  D_s20_80026326 is the unit's `extern u16' and `D_s20_80026326++'
   still gives the `lhu'; D_s20_80026328 is `extern s16', which is what gives
   case 1's `lh'/`beqz'.

   120 words, 0 differing; relcand ok (3 data symbols, 1 call). */
void func_s20_80025160(u8 *p, UnkVec *v, u8 *c)
{
    D_s20_80026326++;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(c + 0x6) = *(u16 *)(c + 0x6) + 0x80;
        if (*(s16 *)(c + 0x6) >= 0x101)
        {
            *(s16 *)(c + 0x6) = 0x100;
            if (*(s16 *)(p + 0x22) < 0x61)
            {
                *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - ((s16)(*(u16 *)(p + 0x18)) >> 3);
                *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + 0x80;
                func_s20_8002569C((u8 *)v, *(s16 *)(p + 0x16), *(s16 *)(p + 0x18),
                                  *(s16 *)(p + 0x1C), p - 0x20);
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            if (*(s16 *)(p + 0x22) == 0x6F)
            {
                D_s20_80026328 = 1;
            }
        }
        break;
    case 1:
        if (D_s20_80026328 != 0)
        {
            *(s16 *)(c + 0x6) = -0x100;
            *(s16 *)(p + 0x1A) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        if (--*(s16 *)(p + 0x1A) > 0)
        {
            break;
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fallthrough */
    case 3:
        *(s16 *)(c + 0x6) = *(u16 *)(c + 0x6) + 0x80;
        if (*(s16 *)(c + 0x6) >= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    c[0xC] = 0x20;
    c[0xD] = 0x40;
    c[0xE] = 0xE0;
}

/* One semi-transparent gouraud LINE_G2 and one DR_MODE per node of the tag
   chain: project the mover's own origin and the point it is interpolated
   towards through the 0x1F800000 scratchpad, average the two depths, and --
   on the forward arm -- record the interpolated point in this bank's trail
   ring at D_80026478.  215 words, 0 differing.

   The twin is csrc/ovl_dng_s16.c's func_s16_8002416C (148 words, landed),
   which is the same walk with a TILE_1 in place of the LINE_G2 and no
   interpolation; its three measured orderings were re-used and hold here --
   `sp->unk_18' is assigned BEFORE `sp->unk_20' even though the target STORES
   them the other way round, and `p = (u8 *)n + 0x20;' comes before
   `v = n->unk_08;'.

   THE ONE SPELLING FACT: the trail-ring index is `(u32)t', an INTEGER
   address, with an explicit `<< 3' -- not `t[... + 0x10]'.  Two things ride
   on that and both were measured here:

     `t[*(s16 *)(p + 0x22) + 0x10].unk_00 = ...'  folds (i + 0x10) * 8 into
         i * 8 + 0x80 and puts the 0x80 in the store's displacement, which
         DELETES the target's three `addiu $v0, $v0, 0x10': 212 words,
         99 aligned differences.
     `*(s16 *)((u8 *)t + ((... + 0x10) << 3))'    keeps the three `addiu' but
         `pointer_int_sum' always puts the pointer operand FIRST, so the
         address comes out `addu $v0, $s5, $v0' where the target has
         `addu $v0, $v0, $s5': 215 words, 3 aligned differences, and writing
         the sum the other way round in the source does not move it.
     `*(s16 *)(((... + 0x10) << 3) + (u32)t)'     both ints, so the written
         order survives: 0.  `(u8 *)((... ) << 3) + (u32)t' is also 0.

   The arm order is load-bearing: `if (s->unk_06 < 0)' first is 212 words and
   185 differing -- the target falls through on `>= 0' and `bltz's to the
   negative arm.  RotTransPers is RotTransPers; the `func_' spellings are
   what splat/dungeon.bin_undefined_funcs_auto.txt links. */
s32 func_s20_80025340(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkDng26478 *t = D_80026478;
    UnkNode *n;
    LINE_G2 *l;
    DR_MODE *dm;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        l = (LINE_G2 *)sp->unk_18;
        sp->unk_18 = (u8 *)(l + 1);
        if (s->unk_06 >= 0)
        {
            sp->unk_00 = *(u16 *)((u8 *)v + 0xE);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x12);
            sp->unk_04 = *(u16 *)((u8 *)v + 0x16);
            sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0xE) +
                         ((*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)v + 0xE)) * s->unk_06 >> 8);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x12) +
                         ((*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)v + 0x12)) * s->unk_06 >> 8);
            sp->unk_04 = *(u16 *)((u8 *)v + 0x16) +
                         ((*(s16 *)((u8 *)v + 0xA) - *(s16 *)((u8 *)v + 0x16)) * s->unk_06 >> 8);
            if (D_s20_8002632A < *(s16 *)(p + 0x22) + 0x10)
            {
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t) = sp->unk_00;
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t + 0x2) = sp->unk_02;
                *(s16 *)(((*(s16 *)(p + 0x22) + 0x10) << 3) + (u32)t + 0x4) = sp->unk_04;
            }
        }
        else
        {
            sp->unk_00 = *(u16 *)((u8 *)v + 0x2) -
                         ((*(s16 *)((u8 *)v + 0xE) - *(s16 *)((u8 *)v + 0x2)) * s->unk_06 >> 8);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x6) -
                         ((*(s16 *)((u8 *)v + 0x12) - *(s16 *)((u8 *)v + 0x6)) * s->unk_06 >> 8);
            sp->unk_04 = *(u16 *)((u8 *)v + 0xA) -
                         ((*(s16 *)((u8 *)v + 0x16) - *(s16 *)((u8 *)v + 0xA)) * s->unk_06 >> 8);
            sp->unk_C0 = RotTransPers((SVECTOR *)sp, (s32 *)&l->x0, &sp->unk_90, &sp->unk_94);
            sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
            sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
            sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        }
        sp->unk_C0 =
            (sp->unk_C0 + RotTransPers((SVECTOR *)sp, (s32 *)&l->x1, &sp->unk_90, &sp->unk_94)) >>
            1;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&l->r0 = *(u32 *)s->unk_0C;
            setLineG2(l);
            setSemiTrans(l, 1);
            l->r1 = l->r0 >> 1;
            l->g1 = l->g0 >> 1;
            l->b1 = l->b0 >> 1;
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, l);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

/* The 0x202 spawner for func_s20_80025160's step and func_s20_80025340's
   renderer.  It seeds the new mover's position from the caller's own position
   offset by rcos/rsin of `ry' scaled by `rx' (>> 15) and by `rz >> 3' in z,
   clamps the z component at D_80026878, copies the unoffset position into the
   mover's +0xE/+0x12/+0x16 interpolation target, writes 0x40E020 into the
   renderable's colour word, records the three angles and the current ring
   cursor in the payload, and -- while the cursor is under 0x71 -- writes the
   interpolation target into BOTH trail-ring banks at D_80026478[cursor] and
   D_80026478[cursor + 0x10] and bumps the cursor.  115 words, 0 differing.

   LANDED OUT OF docs/dng-parks/func_s20_8002569C.c, WHICH STOOD AT 1
   DIFFERING.  Its residue was one word: target word 82, the `sh' in the
   `beqz' delay slot, `sh $a3, 0x22($v1)' where the park's body had
   `sh $a2, ...'.  $a3 is the raw `lhu' of D_s20_8002632A and $a2 its sign
   extension -- the same sixteen bits, so a register choice, not a value.
   The park had measured the catch-22 exactly: `*(s16 *)(q + 0x22) = c'
   stores the raw register the target stores but sched1 then leaves the store
   AHEAD of the `sll', `q' dies before the shift temp is born, local-alloc
   gives both $v0 and reorg can no longer sink the store into the delay slot
   (9 differing); `= e' defers the store but stores the sign extension
   (1 differing).

   THE DEVICE THAT CLOSES IT, AND IT COSTS NO WORD:

       do
       {
           e = (s16)c;
       } while (0);
       *(s16 *)(q + 0x22) = c;

   The `do { } while (0)' is a pure scheduling barrier -- loop NOTEs, no
   instruction -- so it pins the sign extension ahead of the store without
   making the store depend on it.  The store then keeps the RAW value and is
   still the last insn before the branch, which is what reorg sinks into the
   delay slot.  MEASURED, all 115 words unless stated:

     do-while around `e = (s16)c;', store `= c'   (this body)     0
     do-while around BOTH that and the store                      0
     one do-while containing both statements                      0
     ... the same device one statement earlier (before the +0x18
         store instead of after it)                               2
     ... the device before all three other `q' stores  114 words, 12
     do-while around the STORE only, `= c'                        9
     `if (1) { *(s16 *)(q + 0x22) = c; }'                         7
     no device, `= e'                       (the park's body)     1
     no device, `= c'                                             9
     `e = (s16)c; g = e < 0x71;' then store `= c', `if (g)'       7
     ... with `g' computed before `e', or after the store         7
     `e = c; e = (s16)e;' then store `= c'                        7
     `e = c << 16; e >>= 16;' then store `= c'                    9
     `(s16)c' hoisted above all four `q' stores     114 words,   10

   PROBABLY MACRO RESIDUE, not a hand-written `do { } while (0)': narrowing
   the ring cursor to a signed halfword is exactly the body of a one-line
   macro, and a macro body in this tree is a `do { } while (0)'.  I could not
   find a device-free spelling and the park's own list rules out the obvious
   ones.

   THE PARK'S FOUR SPELLING FACTS ALL HOLD and are still in the body: the two
   ring banks are CHAINED assignments (two separate statements reload all
   three values for the second bank: 121 words, 44 aligned differences),
   `f = e + 0x10' is an INDEX LOCAL (inline, loop.c folds the 0x80 into the
   base and deletes the `addiu/sll/addu'), `c = (u16)D_s20_8002632A;' is what
   turns the unit's `extern s16 D_s20_8002632A' into the target's `lhu', and
   D_80026878 is read twice -- `lh' for the compare and `lhu' for the store --
   which is why it is declared `u16' with an explicit `(s16)' on the compare.

   The five-argument signature is the one func_s20_80025160 calls with (the
   fifth argument on the stack at 0x10($sp)); func_s20_80025160 re-verified
   at 127 words / 0 differing with this body in place.

   raw 16 == the 13 relocations plus three local branch targets: R_MIPS_26 on
   func_8003FD64, func_8004491C, func_80064584 and func_800644B8, and
   HI16/LO16 pairs on func_s20_80025160, func_s20_80025340, D_80026878 (one
   HI, two LO), D_s20_8002632A (one HI, two LO) and D_80026478. 
   THIS COPY'S OWN CONSTANT: 0x40e020 -> 0xe04020.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
UnkNode *func_s20_8002569C(u8 *p, s16 ry, s16 rx, s16 rz, void *tmpl)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    u32 c;
    s32 e;
    s32 f;

    n = func_8003FD64(0x202, tmpl);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s20_80025160;
        func_8004491C(n, (u32)func_s20_80025340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(p + 0x2) + ((func_80064584(ry) * rx) >> 15);
        *(s16 *)(m + 0x6) = *(u16 *)(p + 0x6) + ((func_800644B8(ry) * rx) >> 15);
        *(s16 *)(m + 0xA) = *(u16 *)(p + 0xA) + (rz >> 3);
        if ((s16)D_80026878 < *(s16 *)(m + 0xA))
        {
            *(s16 *)(m + 0xA) = D_80026878;
        }
        *(s16 *)(m + 0xE) = *(u16 *)(p + 0x2);
        *(s16 *)(m + 0x12) = *(u16 *)(p + 0x6);
        *(s16 *)(m + 0x16) = *(u16 *)(p + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0xe04020;
        q = (u8 *)n + 0x20;
        c = (u16)D_s20_8002632A;
        *(s16 *)(q + 0x16) = ry;
        *(s16 *)(q + 0x1C) = rz;
        *(s16 *)(q + 0x18) = rx;
        do
        {
            e = (s16)c;
        } while (0);
        *(s16 *)(q + 0x22) = c;
        if (e < 0x71)
        {
            f = e + 0x10;
            D_80026478[f].unk_00 = D_80026478[e].unk_00 = *(u16 *)(m + 0xE);
            D_80026478[f].unk_02 = D_80026478[e].unk_02 = *(u16 *)(m + 0x12);
            D_80026478[f].unk_04 = D_80026478[e].unk_04 = *(u16 *)(m + 0x16);
            D_s20_8002632A = c + 1;
        }
    }
    return n;
}

/* The +0x10 step of func_s21_800254FC's 0x202 node: bump the counter,
   integrate the mover's velocity into its position, damp the two horizontal
   velocity words by an eighth while adding a constant 0x4000 to the third,
   fade the renderable's RGB triple by a quarter, hand the renderable to
   func_800478B8 and kill the node when the +0x1A countdown runs out.
   csrc/ovl_town.c's func_800A86BC is the twin -- the same three vector adds,
   the same func_800478B8 hand-off and the same kill tail -- and its two
   measured notes both hold here: `--*(s16 *)(p + 0x1A) <= 0' gives the
   `lhu'/`addiu -1'/`sh'/`sll 16'/`bgtz' run with no `sra', and the vector
   updates are order-pinned.

   `>> 3' on the two velocity words is SIGNED (`sra'): UnkVec's fields are
   s32. `>> 2' on the three colour bytes is UNSIGNED (`srl'): they are u8.
   The three colour fades are separate statements, as in func_s21_800248E4.

   65 words, 0 differing on the first compile. raw 7 == the 7 relocations:
   1 HI16 + 2 LO16 on D_s20_80026326, the same three on D_800814A0, and
   R_MIPS_26 on func_800478B8. */
void func_s20_80025868(u8 *p, UnkVec *v, u8 *c)
{
    D_s20_80026326++;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C -= v->unk_0C >> 3;
    v->unk_10 -= v->unk_10 >> 3;
    v->unk_14 += 0x4000;
    c[0xC] -= c[0xC] >> 2;
    c[0xD] -= c[0xD] >> 2;
    c[0xE] -= c[0xE] >> 2;
    func_800478B8(c);
    if (--*(s16 *)(p + 0x1A) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s20_80025868's puff.
   csrc/ovl_dungeon_800B6AC4.c's func_800B8D64 is the line-for-line template
   -- the same three s16 position parameters stored straight to the mover's
   +0x2/+0x6/+0xA, the same func_8004491C(n, (u32)D_80045340) tag, the same
   `0x1E' then `0x1C' scale pair and `|= 0xC' on the flag halfword -- with
   0x202 for its 0x212, a func_8003DB94 template in place of the bare +0x8
   pointer, and a random spin.

   THE THREE PARAMETERS ARE `s16' AND GO STRAIGHT INTO THE `sh': no masking
   appears in the target, which is what a narrow parameter stored to a
   halfword gives.

   `*(u32 *)(r + 0xC) = 0x808080;' COMES BEFORE func_8003DB94, which is the
   same ordering fact func_800CC5F0 records: the store rides in the call's
   delay slot. The two 0x1000 scales are a literal written twice, from one
   `addiu', and NOT the read-back func_800CC5F0 and func_800CC88C use -- the
   target has no `lhu 0x1E' between them. The spin is
   `rand() & 0x7'.

   59 words, 0 differing on the first compile. raw 10 == the 10 relocations:
   R_MIPS_26 on func_8003FC64, func_8004491C, rand and
   func_8003DB94, and HI16/LO16 pairs on D_80045340, func_s20_80025868 and
   D_800263F8. */
void func_s20_8002596C(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s20_80025868;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = x;
        *(s16 *)(m + 0x6) = y;
        *(s16 *)(m + 0xA) = z;
        *(s16 *)(m + 0xE) = 0;
        *(s16 *)(m + 0x12) = 0;
        *(s16 *)(m + 0x16) = rand() & 0x7;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        func_8003DB94(r, D_800263F8, 0);
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x10) = 0x20;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)((u8 *)n + 0x3A) = 0x10;
    }
}

/* The +0x10 step of the 0x202 node func_s21_80025BA0 spawns: rebase the four
   corner records at the payload's +0x34..+0x53 on the mover's own origin into
   the scratchpad quad at 0x1F800070, build a rotation-only matrix from the
   slot's +0x16/+0x18 on the stack, push it, run the four corners through
   RotTransSV, pop, integrate the mover's velocity, write the rotated
   corners back onto the payload biased by the mover again, fade the six
   colour bytes at +0x54..+0x5A by one 1/(+0x1A) step each, and kill the node
   when the +0x1A counter runs out.

   NO TWIN OUTSIDE ITS OWN FAMILY. tools/decomp/dngfam.py's only sibling is
   func_s20_80025A58, which is the identical body with every payload offset
   4 lower and its counter inside the object (D_s20_80026326) instead of out
   of image (D_s20_80026326); the best landed non-family neighbour is 0.363.
   docs/dng-parks/func_s20_80025A58.c, agent-h13's park on that sibling, is
   where the addressing convention and six of the spellings below come from:
   a scratchpad STORE is `sp->unk_NN' through a struct pointer at
   0x1F800000 and a scratchpad ADDRESS PASSED AS AN ARGUMENT is the integer
   constant `(SVECTOR *)0x1F800070' off the same hoisted `lui' (stores as
   constant addresses too: 275 words, 258 differing), the stack MATRIX's
   translation is zeroed t[2], t[1], t[0] DESCENDING (ascending, 2 rows), and
   the colour fade re-reads `*(s16 *)(p + 0x1A)' at all six sites so the
   six lbu/lh/div/mflo runs hoist into one block.

   THE COUNTER BUMP IS SPLIT AROUND THE 0x48 CORNER, AND THAT IS THE WHOLE
   OF THE LAST SEVEN WORDS. Written as a plain `D_s20_80026326++' anywhere in the
   vz block the body is 253 words and never better than 7 aligned
   differences, all of them in one window where the counter's lhu/addiu/sh
   and the 0x48 corner's lhu/lhu/subu/sh interleave in the wrong order:
   sched.c's backward list scheduler ties both stores on INSN_PRIORITY (a
   store's destination is a MEM, so birthing_insn_p can never boost it) and
   falls to INSN_LUID, i.e. source order, which a single statement cannot
   split finely enough. Reading the counter BEFORE the corner, computing the
   corner into a local, storing the counter BETWEEN them and storing the
   corner LAST puts the four LUID groups in the order the target wants and is
   0 differing. Measured on this body, all 253 words:

     this spelling                                                     0
     `D_s20_80026326++' after the 0x48 corner (h13's park body)            7
     `D_s20_80026326++' before it                                         13
     the counter's read hoisted, its STORE after the corner's store     2
     `c = D; ...corner...; D = c + 1;' with the corner stored in place  7
     `c = D + 1; ...corner...; D = c;'                                  7
     the counter inside the corner statement as a comma expression,
         in any of four positions                                     13
     one shared s32 local for the 0x48 and 0x50 subtractions          11
     the same for all four vz corners / for all twelve corners    19 / 51

   The last three rows are the boost-removal experiment h13's park proposes
   and they are all negative, which is the evidence that the tie is decided
   before any boost: a local written twice does have reg_n_sets > 1 at flow
   time, and it makes the window worse, not better. THE SAME SPLIT SHOULD
   LAND func_s20_80025A58, whose residue is these same seven words.

   253 words, 0 differing. raw 15 == the 15 relocations: 1 HI16 + 2 LO16 on
   D_s20_80026326, the same three on D_800814A0, and R_MIPS_26 on PushMatrix,
   RotMatrix, SetTransMatrix, SetRotMatrix, RotTransSV four times and
   PopMatrix.
   THIS COPY'S OWN CONSTANT: 0x34 -> 0x30 at all 2 sites, 0x3c -> 0x38 at all 2 sites, 0x44 -> 0x40 at all 2 sites, 0x4c -> 0x48 at all 2 sites, 0x36 -> 0x32 at all 2 sites, 0x3e -> 0x3a at all 2 sites, 0x46 -> 0x42 at all 2 sites, 0x4e -> 0x4a at all 2 sites, 0x38 -> 0x34 at all 2 sites, 0x40 -> 0x3c at all 2 sites, 0x48 -> 0x44 at all 2 sites, 0x50 -> 0x4c at all 2 sites, 0x54 -> 0x50 at all 2 sites, 0x55 -> 0x51 at all 2 sites, 0x56 -> 0x52 at all 2 sites, 0x58 -> 0x54 at all 2 sites, 0x59 -> 0x55 at all 2 sites, 0x5a -> 0x56 at all 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s20_80025A58(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng255E8SP *sp = (UnkDng255E8SP *)0x1F800000;
    SVECTOR rot;
    MATRIX m;
    s32 t;
    s16 n;
    u16 c;

    sp->unk_70[0].vx = *(u16 *)(p + 0x30) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[1].vx = *(u16 *)(p + 0x38) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[2].vx = *(u16 *)(p + 0x40) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[3].vx = *(u16 *)(p + 0x48) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[0].vy = *(u16 *)(p + 0x32) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[1].vy = *(u16 *)(p + 0x3a) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[2].vy = *(u16 *)(p + 0x42) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[3].vy = *(u16 *)(p + 0x4a) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[0].vz = *(u16 *)(p + 0x34) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[1].vz = *(u16 *)(p + 0x3c) - *(u16 *)((u8 *)v + 0xA);
    c = D_s20_80026326;
    t = *(u16 *)(p + 0x44) - *(u16 *)((u8 *)v + 0xA);
    D_s20_80026326 = c + 1;
    sp->unk_70[2].vz = t;
    sp->unk_70[3].vz = *(u16 *)(p + 0x4c) - *(u16 *)((u8 *)v + 0xA);
    PushMatrix();
    rot.vx = s->unk_16;
    rot.vy = s->unk_18;
    rot.vz = 0;
    m.t[2] = 0;
    m.t[1] = 0;
    m.t[0] = 0;
    RotMatrix(&rot, &m);
    SetTransMatrix(&m);
    SetRotMatrix(&m);
    RotTransSV((SVECTOR *)0x1F800070, (s32 *)0x1F800098, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800078, (s32 *)0x1F8000A0, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800080, (s32 *)0x1F8000A8, (s32 *)0x1F800094);
    RotTransSV((SVECTOR *)0x1F800088, (s32 *)0x1F8000B0, (s32 *)0x1F800094);
    PopMatrix();
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    *(s16 *)(p + 0x30) = sp->unk_98[0].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x38) = sp->unk_98[1].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x40) = sp->unk_98[2].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x48) = sp->unk_98[3].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x32) = sp->unk_98[0].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x3a) = sp->unk_98[1].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x42) = sp->unk_98[2].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x4a) = sp->unk_98[3].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x34) = sp->unk_98[0].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x3c) = sp->unk_98[1].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x44) = sp->unk_98[2].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x4c) = sp->unk_98[3].unk_04 + *(u16 *)((u8 *)v + 0xA);
    p[0x50] -= p[0x50] / *(s16 *)(p + 0x1A);
    p[0x51] -= p[0x51] / *(s16 *)(p + 0x1A);
    p[0x52] -= p[0x52] / *(s16 *)(p + 0x1A);
    p[0x54] -= p[0x54] / *(s16 *)(p + 0x1A);
    p[0x55] -= p[0x55] / *(s16 *)(p + 0x1A);
    p[0x56] -= p[0x56] / *(s16 *)(p + 0x1A);
    n = *(u16 *)(p + 0x1A) - 1;
    *(s16 *)(p + 0x1A) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* BEGIN func_s20_80025E4C
   THIS COPY'S OWN CONSTANT: 0x34 -> 0x30, 0x3c -> 0x38, 0x44 -> 0x40, 0x4c -> 0x48, 0x38 -> 0x34, 0x40 -> 0x3c, 0x48 -> 0x44, 0x50 -> 0x4c, 0x54 -> 0x50, 0x58 -> 0x54.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
/* The renderer for func_s21_800255E8's 0x202 node: one semi-transparent
   POLY_G4 and one DR_MODE per node of the tag chain, with the quad's four
   corners read straight out of the node payload's +0x34..+0x53 record and its
   two colour words out of +0x54/+0x58.  113 words, 0 differing.

   Written against two already-landed neighbours in this same unit, so only
   one thing had to be found: func_s21_80024A68 supplies the
   D_80083160/+0x8D0 cursor prologue, the `sp->unk_18' before `sp->unk_20'
   order, the RotTransPers4 argument list, the CHAINED colour pairs
   (`q->r0 = q->r1 = ...' emits +0x0C before +0x04) and the
   GetTPage/func_80067F20/AddPrim tail; func_s21_80024ED0 supplies the
   `while (1) { ... n = *(UnkNode **)(p - 0x8); if (n == NULL) break;
   p = (u8 *)n + 0x20; }' walk.

   THE ONE SPELLING FACT: THE FOURTH CORNER'S WORD IS HOISTED INTO A LOCAL AND
   STORED SEVENTH.  Written in place as the fourth of eight stores, its `lw'
   and `sw' sit next to each other, the `lhu 0x38' cannot cover the `lw
   0x44' load delay, and the body is 114 words with a third `nop'.  Read into
   `w' after the third word and stored after the `lhu 0x48', the 0x4C value
   lives in $v1 across nine insns and the block costs two `nop's instead of
   three.  Measured, all at 113 or 114 words:

     w0 w1 w2 w=0x4C v0 v1 v2 w3=w v3   (this body)   113 words,  0
     w0 w1 w2 w=0x4C v0 v1 w3=w v2 v3   113 words,  2 aligned differences
     w0 w1 w2 w=0x4C v0 v1 v2 v3 w3=w   113 words,  4
     w0 w1 w2 w3 v0 v1 v2 v3            114 words,  6
     w3 w0 w1 w2 v0 v1 v2 v3            114 words,  6
     w0 w1 w2 v0 v1 w3 v2 v3            114 words,  9
     w0 w1 w2 v0 v1 v2 v3 w3            114 words, 12
     w0 w1 v0 v1 w2 v2 w3 v3            114 words, 15
     w0 v0 w1 v1 w2 v2 w3 v3            114 words, 20
     v0 v1 v2 v3 w0 w1 w2 w3            114 words, 29

   raw 9 == the 9 relocations: a HI16/LO16 pair on D_80083160 plus a second
   LO16 for the `lw', and R_MIPS_26 on RotTransPers4, SetPolyG4, AddPrim,
   GetTPage, func_80067F20 and AddPrim again. */
s32 func_s20_80025E4C(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    POLY_G4 *q;
    DR_MODE *dm;
    u32 w;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        q = (POLY_G4 *)sp->unk_18;
        sp->unk_18 = (u8 *)(q + 1);
        *(u32 *)&sp->unk_70[0] = *(u32 *)(p + 0x30);
        *(u32 *)&sp->unk_70[1] = *(u32 *)(p + 0x38);
        *(u32 *)&sp->unk_70[2] = *(u32 *)(p + 0x40);
        w = *(u32 *)(p + 0x48);
        sp->unk_70[0].vz = *(u16 *)(p + 0x34);
        sp->unk_70[1].vz = *(u16 *)(p + 0x3c);
        sp->unk_70[2].vz = *(u16 *)(p + 0x44);
        *(u32 *)&sp->unk_70[3] = w;
        sp->unk_70[3].vz = *(u16 *)(p + 0x4c);
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&q->r0 = *(u32 *)&q->r1 = *(u32 *)(p + 0x50);
            *(u32 *)&q->r2 = *(u32 *)&q->r3 = *(u32 *)(p + 0x54);
            SetPolyG4(q);
            setSemiTrans(q, 1);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
            dm = (DR_MODE *)sp->unk_18;
            sp->unk_18 = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            AddPrim(sp->unk_20 + sp->unk_C0 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s20", func_s20_80026010);

/* Apply status kind 0x17 at a strength scaled off the second parameter, then
   print the D_800E1D01 message, both gated on func_8009D218 coming back zero.
   csrc/ovl_dungeon_800C2824.c's func_800A48F0/func_80099844 pairs are the
   shape; func_8009D218 takes TWO arguments here, not the three
   csrc/ovl_dungeon_decls.h declares, so it rides a unit-local alias.

   THE SECOND PARAMETER IS `u8' AND THE SHIFT IS ITS OWN STATEMENT, and both
   halves are load-bearing -- together they are 9 of the 27 words:

   * `s32 d' with `((d & 0xFF) >> 2) + 2' is 9 differing. It gives `sra' for
     the target's `srl' (1 word) and, worse, swaps the two callee-saved
     registers: `p' lands in $s0 and `d' in $s1 where the target has them the
     other way round, and the argument setup then comes out in the other
     order too (8 words). The register order is global_alloc's priority
     `floor_log2 (n_refs) * n_refs / live_length': `p' is 4 refs over 21
     insns and `d' 2 over 7, so `p' sorts first and takes $s0. A `u8'
     parameter costs an extra flow-counted set for the truncation, which
     carries `d' past it. `u32 d' fixes only the `srl' -- still 9.
   * `e = d; e >>= 2;' as two statements for `e = d >> 2' is worth 4 words on
     top of that. With the shift in the argument expression the and/shift
     chain's temporaries are each set ONCE, so adjust_priority's
     birthing_insn_p boost lifts them to max_priority and sched.c's backward
     scan places them after the $a0/$a1 argument moves; a temporary written
     by two statements has reg_n_sets > 1, loses the boost, and the chain
     comes out first, which is the target. A device-free spelling: the `andi
     0xFF' in the target IS the u8 parameter's own truncation.
     `u32 d' plus a redundant `d &= 0xFFFF;' that combine deletes measures
     0 as well, and is the same two mechanisms bought with a device instead.

   27 words, 0 differing. raw 5 == the 5 relocations: R_MIPS_26 on
   func_8009D218, func_800A48F0 and func_80099844, and a HI16/LO16 pair on
   D_800E1D01.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x2, 0x17 -> 0x16, 0x2 -> 0x4 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s20_800262B8(u8 *p, u8 d)
{
    u32 e;

    if (func_8009D218_2(p, 2) == 0)
    {
        e = d;
        e >>= 2;
        if ((s16)func_800A48F0_i(p, 0x16, e + 4) != 0)
        {
            func_80099844(p, D_800E1D01);
        }
    }
}
