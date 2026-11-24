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
void func_s19_800255DC(u8 *p, UnkVec *v, u8 *c);
/* Forward declaration added in batch 26 by the dispatcher: this is called at
 * two sites in func_s19_80024050 and DEFINED ~300 lines lower in this file,
 * so it had no declaration in scope and K&R C gave it an implicit `int'
 * against a definition returning UnkNode *.  Both call sites discard the
 * result and pass pointers, so the emitted code is identical and ovlverify
 * read 0 -- tools/decomp/declcheck.py is what sees it. */
UnkNode *func_s19_80025654(u8 *v, u8 *q);

extern u16 D_s19_800257CE;

extern u8 D_800E1CD5[];
void func_80099844(u8 *, u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");

extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800DDC40[];
extern u8 D_80045340[];
extern u8 D_800257D0[];
extern u8 D_800257E8[];
extern s32 D_8008346C;
void func_800B835C(u8 *, s32 *, s32, s32);
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A56E0(s32);
s32 func_8009FCF0w(u8, u8, u8, u8) __asm__("func_8009FCF0");
s32 func_800A45D8w(s32, s32, s32) __asm__("func_800A45D8");
u8 *func_s19_80024968(u8 *, UnkVec *, s16, u8 *);
void func_s19_80025760_3(u8 *, u8, u8 *) __asm__("func_s19_80025760");
extern s16 D_s19_800257CC;
void func_s19_80024A98(u8 *);
void func_800478B8(u8 *);

/* The unit's root state machine, and it OWNS A JUMP TABLE.
   jtbl_s19_80024008: 18 `.word's = 18 cases + 0 pad, dispatch bound
   `sltiu $v0, $v1, 0x12' with no minval subtraction, 8 distinct targets
   (`python3 tools/decomp/jtcheck.py --func func_s19_80024050' agrees).
   The entry list, in table order:

     case 0  .Ls19_800240B4   case 9  .Ls19_800246F8 (default)
     case 1  .Ls19_80024128   case 10 .Ls19_800246F8 (default)
     case 2  .Ls19_80024394   case 11 .Ls19_800246F8 (default)
     case 3  .Ls19_8002459C   case 12 .Ls19_800246F8 (default)
     case 4  .Ls19_800245D8   case 13 .Ls19_800246F8 (default)
     case 5  .Ls19_800246F8 (default)  case 14 .Ls19_800246F8 (default)
     case 6  .Ls19_800246F8 (default)  case 15 .Ls19_800246F8 (default)
     case 7  .Ls19_800246F8 (default)  case 16 .Ls19_8002461C
     case 8  .Ls19_800246F8 (default)  case 17 .Ls19_800246BC

   SO THERE IS NOTHING TO HAND-SETTLE ABOUT ARM ORDER. All ten shared
   entries carry the DEFAULT label -- the same .Ls19_800246F8 the range check
   `beqz' jumps to -- so the seven real arms have seven distinct labels and
   no two of them are word-identical after cross-joining. The arms emit in
   source order and their entry addresses are already ascending in case
   order (0x240B4 < 0x24128 < 0x24394 < 0x2459C < 0x245D8 < 0x2461C <
   0x246BC), so the source is written 0, 1, 2, 3, 4, 16, 17 with no
   `default:' label: cases 5..15 are the gcc-filled default entries, NOT
   empty `break' arms, and writing them out would be seven phantom labels.
   With 7 case values the set is above gcc 2.7.2's `case_values_threshold'
   (4) and the range 17 is well under 10 * 7, so `expand_end_case' takes the
   table and not a comparison tree.

   NO TWIN. dngtwin's best is func_s20_80024064 at 0.875 and
   func_s21_80024050 at 0.843, both still INCLUDE_ASM (and the latter is
   PARKED at docs/dng-parks/func_s21_80024050.c -- this landing should let
   that park be re-derived). xsibs finds nothing at 0.70. What got it written
   is the callee-set grep: func_s25_80024028_3's three-argument call site
   (`*(u8 **)(s + 0x60), p[0x9], s') is in csrc/ovl_dng_s25.c's landed
   func_s25_800247D4, whose case 0/case 1 head is this function's line for
   line -- the func_8003DE58/+0x14-0x8000 `&&' gate, the `**(u16 **)(p+0x4)
   & 0x80' retire and the `u16 out[4]' with a `(s16 *)' cast at the call.
   Its arms are inverted here: this body takes `-0x40' on the flag SET and
   `+ out[2]' on the clear, s25 the other way round.

   SIX FACTS MEASURED, both spellings priced (aligned differences at 438
   words unless noted; `ovlverify' inflates every row that shifts a word, so
   price with aligndiff):

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | case 2's accumulate as `b->unk_00 = b->unk_00 +       |       |      |
   |   b->unk_0C;' THEN `k = b->unk_0C + (b->unk_0C >> 4)' |  438  |   0  |
   |   ... `k = b->unk_0C; b->unk_00 += k; k = k+(k>>4)'   |  438  |  94  |
   |   ... accumulate first, then `k = b->unk_0C;          |       |      |
   |         k = k + (k >> 4);' as TWO statements          |  438  |  37  |
   | a SEPARATE local for each of the two clamps           |  438  |   0  |
   |   ... one `k' shared with case 1's distance           |  438  |  20  |
   | case 1's distance as ONE `__builtin_abs(cond ? a : b)'|  438  |   0  |
   |   ... an if/else into `k' then `k = __builtin_abs(k)' |  438  |   9  |
   | the clamp value through a local, `t = (k > 0) ?        |       |      |
   |   0x200000 : -0x200000; b->unk_0C = t;'               |  438  |   0  |
   |   ... the ternary stored straight into `b->unk_0C'    |  442  |  57  |
   | `D_8006CCD8[i] * 8' at the +0xE/+0x12 velocity stores |  438  |   0  |
   |   ... `D_8006CCD8[i] << 3'                            |  442  |  59  |
   | the +0x18 countdown as its own statement, then        |       |      |
   |   `if (*(s16 *)(p + 0x18) == 0 || ...)'               |  438  |   0  |
   |   ... `if ((s16)(--*(u16 *)(p + 0x18)) == 0 || ...)'  |  438  |   1  |
   | func_s19_80024968 called with FOUR arguments          |  438  |   0  |
   |   ... three, dropping the `*(u8 **)(s + 0x60)' repeat |  438  |   2  |

   THE TWO BIG ONES ARE BOTH reg_n_sets, NOT allocation priority, and the
   diagnostic is the same in both: a chain scheduled at the WRONG END of its
   basic block with a register pair exchanged behind it.
   `k = X + (X >> 4)' sets `k' ONCE, so adjust_priority's birthing_insn_p
   boost applies and sched1 defers the chain; `k = X; k = k + (k >> 4)' is
   two sets, loses the boost, the chain comes out first and the
   `b->unk_00' store slides past the `sra'. Sharing one `k' with case 1 does
   the same thing by the back door -- `k' then has THREE sets across the
   function and the boost is gone from both arms. The 94-row row is that
   fault plus its wake: it rotates $s1/$s2/$s3 over the whole body, so
   `count distinct causes, not rows'.

   `D_8006CCD8[i] << 3' is convert.c: the LSHIFT_EXPR arm of the narrowing
   distribution is guarded on TREE_UNSIGNED and MULT_EXPR's is not, so the
   shift makes the array read `lhu' where the target has `lh', with nothing
   else moving. `* 8' is the same tree otherwise.

   THE FOUR-ARGUMENT CALL IS READABLE OFF THE CALLEE: func_s19_80024968's
   own listing opens `addu $s3, $a3, $zero', so it takes four, and the fourth
   is the `*(u8 **)(s + 0x60)' the third argument is read through -- which is
   why the pointer lands in $a3 rather than $v0. Two rows, and no tool sees
   it: relcand compares the call SET.

   MEASURED AND FREE, so none of these is why this lands (all 438 / 0):
   `e[0x24] == *(s8 *)(s + 0x72)' for the written order and the `!=' form
   with the arms swapped; `p[0x22] = p[0x20]' against writing the whole
   `e[0x24] + *(u8 *)&D_8006CCD8[...]' expression twice; `q = (u8 *)(n + 1)'
   for `(u8 *)n + 0x20'; `(u16)out[2]' with `out' declared `s16 out[4]' and
   the cast at the read; and declaring the two clamp locals in either order.
   NOT MEASURABLE: which of cases 5..15 the table's shared entries name
   (they are all the default), and the `&&' versus nested-`if' spelling of
   the `*(u8 **)(s + 0x60)' re-test, whose arms are identical after
   cross-joining.

   SIGNATURES. func_8003DF74 is undeclared anywhere in the tree and is given
   csrc/ovl_dng_s55.c's func_8003DE58 shape, `(u8 *, UnkSlot *, s16 *, s16)',
   which is what s25's twin call site uses. func_8009FCF0 and func_800A45D8
   are declared `s16'-returning in csrc/ovl_dungeon_8009F644.c and
   csrc/ovl_dungeon_800A4B88.c; this body needs the `sll 16'/`sra 16' of an
   s32 result cast to s16 at both sites, so it carries unit-local s32
   aliases (`func_8009FCF0w', `func_800A45D8w') and leaves both landed
   declarations alone. func_800A45D8w's first two parameters are `s32' and
   not the landed `u16' so that `& 0xFFE0' stays SImode: through the `u16'
   prototype convert.c narrows the mask to HImode -32 and cc1psx materialises
   it, which is 2 words and the whole func_800A45D8 argument block.
   func_s19_80025760 is called with THREE arguments here while its landed
   definition below takes two, so this unit carries a three-argument alias
   for that one site, exactly as csrc/ovl_dng_s25.c does for
   func_s25_80024028.

   438 words, 0 differing; relcand ok (9 data symbols, 10 calls). */
void func_s19_80024050(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *g;
    u8 *e;
    u8 *m;
    u8 *n;
    s32 k;
    s32 d;
    s32 k2;
    s32 d2;
    s32 t;
    s32 t2;
    u16 out[4];
    s32 arr[2];

    s = *(u8 **)(p + 0x0);
    o = s - 0x20;
    w = *(u8 **)(o + 0x8);
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x800;
        *(s16 *)(c + 0x1C) = 0x800;
        *(u32 *)(c + 0x8) = (u32)D_800257E8;
        *(s16 *)(p + 0x1A) = (*(u16 *)(s + 0x2A) >> 9) & 7;
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C(D_800257D0, arr, 1, 0);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if (func_8003DF74(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(w + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(w + 0x6);
        if ((*(u16 *)(*(u8 **)(o + 0xC) + 0x14) & 0x8000) != 0)
        {
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(w + 0xA) - 0x40;
        }
        else
        {
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(w + 0xA) + out[2];
        }
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        if ((p[0x16] & 0x4) == 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x16] |= 0x4;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            g = *(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            *(s16 *)(p + 0x10) = *(u16 *)(g + 0x2);
            *(s16 *)(p + 0x12) = *(u16 *)(g + 0x6);
            *(s16 *)(p + 0x14) =
                *(u16 *)(g + 0xA) - (D_800DDC40[(*(u8 **)(s + 0x60))[0x13]] + 0x20);
            e = *(u8 **)(s - 0x14);
            p[0x20] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x1A)];
            p[0x22] = p[0x20];
            p[0x21] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x1A)];
            p[0x23] = p[0x21];
            *(s16 *)(p + 0x18) = __builtin_abs(*(s8 *)(s + 0x72) == e[0x24]
                                                   ? *(s8 *)(s + 0x73) - e[0x25]
                                                   : *(s8 *)(s + 0x72) - e[0x24]) +
                                 1;
        }
        else
        {
            *(s16 *)(p + 0x18) = 8;
            *(s16 *)(p + 0x10) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(p + 0x12) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(p + 0x14) = *(u16 *)(s + 0x88) - 0x50;
        }
        *(s16 *)((u8 *)b + 0xE) = D_8006CCD8[*(s16 *)(p + 0x1A)] * 8;
        *(s16 *)((u8 *)b + 0x12) = D_8006CCE8[*(s16 *)(p + 0x1A)] * 8;
        *(s16 *)(p + 0x1C) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        b->unk_00 = b->unk_00 + b->unk_0C;
        k = b->unk_0C + (b->unk_0C >> 4);
        b->unk_0C = k;
        d = __builtin_abs(k);
        if (0x200000 < d)
        {
            t = (k > 0) ? 0x200000 : -0x200000;
            b->unk_0C = t;
        }
        b->unk_04 = b->unk_04 + b->unk_10;
        k2 = b->unk_10 + (b->unk_10 >> 4);
        b->unk_10 = k2;
        d2 = __builtin_abs(k2);
        if (0x200000 < d2)
        {
            t2 = (k2 > 0) ? 0x200000 : -0x200000;
            b->unk_10 = t2;
        }
        b->unk_08 = b->unk_08 + (((*(s16 *)(p + 0x14) << 16) - b->unk_08) >> 3);
        func_s19_80025654((u8 *)b, c);
        p[0x20] = *(s16 *)((u8 *)b + 0x2) / 0x40;
        p[0x21] = *(s16 *)((u8 *)b + 0x6) / 0x40;
        if (*(s16 *)(p + 0x22) == *(s16 *)(p + 0x20))
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            if (*(s16 *)(p + 0x28) == 0)
            {
                if ((s16)func_8009FCF0w(p[0x20], p[0x21], s[0x72], s[0x73]) < 5)
                {
                    *(s16 *)(p + 0x28) = 1;
                    func_800A56E0(0x300);
                }
            }
            if (*(u8 **)(s + 0x60) != NULL && *(s16 *)(s + 0x72) == *(s16 *)(p + 0x20))
            {
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)(p + 0x10);
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)(p + 0x12);
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x14);
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
                break;
            }
        }
        p[0x22] = p[0x20];
        p[0x23] = p[0x21];
        *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - 1;
        if (*(s16 *)(p + 0x18) == 0 ||
            (s16)func_800A45D8w(((*(s8 *)(p + 0x20) << 6) + 0x20) & 0xFFE0,
                                ((*(s8 *)(p + 0x21) << 6) + 0x20) & 0xFFE0,
                                *(s16 *)((u8 *)b + 0xA)) != 0)
        {
            *(s16 *)(p + 0xA) = 0x10;
        }
        break;
    case 3:
        n = func_s19_80024968(p, b, *(s16 *)(*(u8 **)(s + 0x60) + 0x88), *(u8 **)(s + 0x60));
        *(u32 *)(p + 0xC) = (u32)n;
        if (n == NULL)
        {
            break;
        }
        *(u16 *)(c + 0x14) |= 0x80;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 4:
        if ((*(u16 *)(*(u8 **)(p + 0xC) + 0x1E) & 0x8000) == 0)
        {
            break;
        }
        if (*(u8 **)(s + 0x60) != NULL)
        {
            func_s19_80025760_3(*(u8 **)(s + 0x60), p[0x9], s);
        }
        *(s16 *)(p + 0xA) = 0x11;
        break;
    case 16:
        m = (u8 *)b;
        *(s32 *)(m + 0x0) = *(s32 *)(m + 0x0) + *(s32 *)(m + 0xC);
        *(s32 *)(m + 0x4) = *(s32 *)(m + 0x4) + *(s32 *)(m + 0x10);
        *(s32 *)(m + 0x8) =
            *(s32 *)(m + 0x8) + (((*(s16 *)(p + 0x14) << 16) - *(s32 *)(m + 0x8)) >> 3);
        c[0xC] -= c[0xC] >> 1;
        c[0xD] -= c[0xD] >> 1;
        c[0xE] -= c[0xE] >> 1;
        func_s19_80025654(m, c);
        if (c[0xC] < 2)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 17:
        if (*(s16 *)&D_s19_800257CE == 0)
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    D_s19_800257CE = 0;
}

/* The unit's second state machine, over the same +0xA selector but as a
   COMPARISON TREE and not a table: cases 0, 1 and 2 only, and three case
   labels is below gcc 2.7.2's `case_values_threshold' (4), so
   `expand_end_case' emits the balanced tree on 1 (`beq 1', then `slti 2'
   splitting 0 from 2) that the listing opens with. Case 0 raises 0x10000000
   in the acting record's +0x1C flags, case 1 ramps the slot's RGB towards
   0xC0/0x40/0x40 until this bank's D_s19_800257CC reaches 0xE, case 2 ramps
   it back towards 0x80/0x80/0x80 and retires the node once D_s19_800257CC
   is empty; every path then advances the caller's mover by its own +0x14
   velocity, drops that velocity by 0x80000, calls func_s19_80024A98 and
   bumps +0x1C.

   NO TWIN: dngtwin reports no carved sibling at 0.70 for this stub and
   xsibs none at all. Written from the listing; it landed on the first
   compile, so the two facts below were measured AFTER the fact by flipping
   them, and the four free rows with them.

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | case 0 FALLS THROUGH into case 1               KEPT   |  144  |   0  |
   |   ... `break;' at the end of case 0                   |  141  |   7  |
   | `if (D_s19_800257CC < 0xE)' read SIGNED        KEPT   |  144  |   0  |
   |   ... `if ((u16)D_s19_800257CC < 0xE)'                |  144  |   2  |

   The 141-word row is the diagnostic shape for a missing fall-through: three
   words SHORT, not longer, because the `break' lets jump.c fold case 0's
   tail into the common exit.
   D_s19_800257CC is `extern s16' here and that is what gives the `lh'/`slti'
   pair at 80024824 and the `lh'/`bnez' at 800248D4; the `(u16)' cast turns
   both into `lhu'/`sltiu' and costs the 2. Note that
   docs/dng-parks/func_s19_80024D90.c reads the SAME symbol with `lhu',
   because `D_s19_800257CC++' on an `s16' still loads unsigned -- one
   declaration serves both.

   MEASURED AND FREE, so none of these is why this lands (all 144 / 0): a
   SEPARATE `u8 *' local per arm for `*(u8 **)(*(u8 **)p - 0x14)' against one
   shared local -- func_800974DC's one-local-per-arm rule is vacuous here;
   `>= 0xC1' written for `> 0xC0' (fold makes them one tree); and
   `b->unk_14 + 0xFFF80000' for `- 0x80000' -- the C89 signedness trap needs
   a division, a shift or a comparison downstream and this value feeds a
   plain store.

   144 words, 0 differing; relcand ok (3 data symbols, 1 call). */
void func_s19_80024728(u8 *p, UnkVec *b, u8 *c)
{
    u8 *r;
    u8 *r2;

    D_s19_800257CE++;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(*(u8 **)p + 0x1C) |= 0x10000000;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        r = *(u8 **)(*(u8 **)p - 0x14);
        r[0xC] = r[0xC] + 1;
        if (r[0xC] > 0xC0)
        {
            r[0xC] = 0xC0;
        }
        r[0xD] = r[0xD] - 1;
        if (r[0xD] < 0x40)
        {
            r[0xD] = 0x40;
        }
        r[0xE] = r[0xE] - 1;
        if (r[0xE] < 0x40)
        {
            r[0xE] = 0x40;
        }
        if (D_s19_800257CC < 0xE)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        r2 = *(u8 **)(*(u8 **)p - 0x14);
        r2[0xC] = r2[0xC] - 4;
        if (r2[0xC] < 0x80)
        {
            r2[0xC] = 0x80;
        }
        r2[0xD] = r2[0xD] + 4;
        if (r2[0xD] > 0x80)
        {
            r2[0xD] = 0x80;
        }
        r2[0xE] = r2[0xE] + 4;
        if (r2[0xE] > 0x80)
        {
            r2[0xE] = 0x80;
        }
        if (D_s19_800257CC == 0)
        {
            *(u32 *)(*(u8 **)p + 0x1C) &= 0xEFFFFFFF;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    b->unk_08 = b->unk_08 + b->unk_14;
    b->unk_14 = b->unk_14 - 0x80000;
    func_s19_80024A98((u8 *)b);
    *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024968);

/* The per-frame step for this unit's +0x1C-gated hit effect: bump
   D_s19_800257CE, and on the low bit of the payload's own +0x1C counter hand
   the renderable to func_800478B8 and retire the node when its +0x14 flags
   come back with 0x4000 set; the counter is bumped either way.
   TWIN: the landed func_s10_80024224 in csrc/ovl_dng_s10.c (dngtwin 0.783,
   31 words), which is the same func_800478B8/retire pair with no +0x1C gate
   and a 0x6000 mask; xsibs offers town.bin's func_800BF15C at 0.831.

   THE ONE FACT, and it is a DEPENDENCE not a shape: the +0x1C gate must be
   an ARRAY REF, `((u16 *)p)[0xE]'. Written `*(u16 *)(p + 0x1C)' the MEM
   loses MEM_IN_STRUCT_P, gcc's fixed-scalar-versus-varying-struct rule in
   `true_dependence' no longer excuses it against the fixed-address
   D_s19_800257CE beside it, the `sh %lo(D_s19_800257CE)' cannot sink into
   the `beqz' delay slot, the load-delay slot takes a `nop' instead and the
   body is 39 words / 9 aligned differences (34 by ovlverify). Same mechanism
   as csrc/ovl_dng_s55.c's func_s55_80025764 and csrc/ovl_dng_b02.c:2402.

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | gate `((u16 *)p)[0xE] & 0x1'                   KEPT   |   38  |   0  |
   |   ... `*(u16 *)(p + 0x1C) & 0x1'                      |   39  |   9  |
   |   ... `((UnkSlot *)p)->unk_1C & 0x1'                  |   38  |   0  |
   | the tail bump as `((u16 *)p)[0xE] = ... + 1'   KEPT   |   38  |   0  |
   |   ... as `*(u16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + 1'|   38  |   0  |

   The UnkSlot row is a THIRD spelling that also reaches 0: any
   MEM_IN_STRUCT_P form works, so the array ref is kept because it claims no
   struct this function cannot witness. The tail bump is FREE in both forms,
   so only the GATE's load carries the dependence -- the counter store is
   already past the tail by then. And the counter spelling is not the lever:
   `D_s19_800257CE = D_s19_800257CE + 1', `+= 1' and a single-set local all
   measure 39 / 34 with the plain-cast gate.

   38 words, 0 differing; relcand ok (2 data symbols, 1 call). */
void func_s19_80024A00(u8 *p, UnkVec *v, u8 *c)
{
    D_s19_800257CE++;
    if ((((u16 *)p)[0xE] & 0x1) != 0)
    {
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x4000) != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
    ((u16 *)p)[0xE] = ((u16 *)p)[0xE] + 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024A98);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024B98);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024D90);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024EE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_80024FD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_800250E8);

/* The THREE-PARAMETER member of func_800D5AC0's family (csrc/ovl_dungeon_
   800D50FC.c, which carries the long note): one semi-transparent 1x1 TILE_1
   and one DR_MODE per node of the tag chain, projected through the
   0x1F800000 scratchpad. dngtwin ranks four landed carved copies at 0.929
   (func_b34_80170ED8, func_b31f_80171704, func_b23_80170D2C,
   func_b19_80170884, all 120 words); xsibs names the resident func_800D68F4
   and func_800D5AC0 at 0.946.

   THREE RELINK DELTAS, all additive. (1) The colour word comes from a THIRD
   parameter's renderable at +0xC where the 120-word family reads `p + 0x8',
   and the loop refreshes it from `n->unk_0C' -- that `lw $s3, 0xC($a2)' is
   the whole one-word difference. (2) The scratchpad is the +4-shifted
   UnkDng73E94SP layout of csrc/ovl_dng_s39.c's landed func_s39_80024EDC
   (+0x18 cursor, +0x20 OT base, +0x90/+0x94 out-parameters, +0xC0 index),
   not the twin's UnkDng708A4SP. (3) There is NO setRGB0 echo.

   FOUR FACTS MEASURED ON THIS BODY, both spellings priced:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | `sp->unk_18' assigned BEFORE `sp->unk_20'      KEPT   |  121  |   0  |
   |   ... swapped (the order the target STORES them)      |  121  |  11  |
   | the colour word copied BEFORE the tag setup    KEPT   |  121  |   0  |
   |   ... after setTile1/setSemiTrans                     |  121  |   8  |
   | `v = n->unk_08' before `c = (u8 *)n->unk_0C'   KEPT   |  121  |   0  |
   |   ... the two swapped                                 |  121  |   5  |
   | no setRGB0(t, t->r0, t->g0, t->b0) echo        KEPT   |  121  |   0  |
   |   ... the twin's echo added                           |  124  |  79  |

   The 11 reproduces func_s39_80024EDC's figure for the same pair to the
   digit and NOT the resident original's 20, which is the shape-versus-frame
   split: s39 has this body's scratchpad type and its six saves plus $s6.

   THE RESIDENT TWIN'S THIRD ORDERING FACT IS FREE HERE. It insists
   `p = (u8 *)n + 0x20' come before `v = n->unk_08' and prices the swap at 4;
   on this body `p' first, `v'/`c' first and `p', `c', `v' are 0, 0 and 5 --
   so what is load-bearing is `v' before `c', not `p' before `v'.

   121 words, 0 differing; relcand ok (1 data symbol, 3 calls). */
s32 func_s19_800252E0(u8 *p, UnkVec *v, u8 *c)
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
            *(u32 *)&t->r0 = *(u32 *)(c + 0xC);
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
        c = (u8 *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_18;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s19", func_s19_800254C4);

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
   1 HI16 + 2 LO16 on D_s19_800257CE and the same three on D_800814A0. */
void func_s19_800255DC(u8 *p, UnkVec *v, u8 *c)
{
    D_s19_800257CE++;
    c[0xC] -= c[0xC] >> 3;
    c[0xD] -= c[0xD] >> 3;
    c[0xE] -= c[0xE] >> 3;
    if (c[0xC] < 8)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s19_800255DC's fading marker.
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
   func_s19_800255DC and func_800CEEFC. */
UnkNode *func_s19_80025654(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s19_800255DC;
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

/* Apply status kind 0x17 at a strength scaled off the second parameter, then
   print the D_800E1CD5 message, both gated on func_8009D218 coming back zero.
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
   D_800E1CD5.
   THIS COPY'S OWN CONSTANT: 0x4 -> 0x1, 0x17 -> 0x15.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s19_80025760(u8 *p, u8 d)
{
    u32 e;

    if (func_8009D218_2(p, 1) == 0)
    {
        e = d;
        e >>= 2;
        if ((s16)func_800A48F0_i(p, 0x15, e + 2) != 0)
        {
            func_80099844(p, D_800E1CD5);
        }
    }
}
