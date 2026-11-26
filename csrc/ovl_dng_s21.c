#include "common.h"

s32 rand(void);

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
extern UnkDng26478 D_800265C8[];
extern s16 D_80026476;
void func_80067F20(u32 *, s32, s32, s32, s32);

extern u32 D_800265C0;
extern u32 D_800265C4;

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
extern u16 D_80026472;
extern u8 D_80026544[];
extern u8 D_80045340[];
extern u8 D_800E1D28[];
void func_8003DB94(void *, void *, s32);
UnkNode *func_8003FC64(s32);
void func_800478B8(u8 *);
void func_80099844(u8 *, u8 *);
s32 func_8009D218_2(u8 *, s32) __asm__("func_8009D218");
s16 func_800A48F0_i(u8 *, s32, s32) __asm__("func_800A48F0");
s32 func_800CEEFC();
void func_s21_800248E4(u8 *p, UnkVec *v, u8 *c);
void func_s21_800253F8(u8 *p, UnkVec *v, u8 *c);

extern u8 D_80026478[];
void func_800B835C(u8 *, s32 *, s32, s32);
extern s16 D_80026474;
UnkNode *func_s21_8002522C(u8 *p, s16 ry, s16 rx, s16 rz, void *tmpl);
UnkNode *func_s21_800262AC(s16 x, s16 y, s16 z);
void func_s21_80024CD4(u8 *p, UnkVec *v, u8 *c);

extern u8 D_80026490[];
extern u16 D_800269C8;
s32 func_8003FA44(s32);
UnkNode *func_8003FD64(s32, void *);

extern s16 D_80026470;
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;
extern s16 D_80026472_i __asm__("D_80026472");
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
s32 func_80065F90(s32, s32);
s32 func_800A45D8w(s32, s32, s32) __asm__("func_800A45D8");
s32 func_800A56E0(s32);
void func_s21_80026404_3(u8 *, u8, u8 *) __asm__("func_s21_80026404");
UnkNode *func_s21_8002495C(u8 *v, u8 *q);
s32 func_s21_80024A68(void);

/* The master step handler of the spell script, and IT OWNS A JUMP TABLE.
   jtbl_s21_80024008: 18 `.word's, dispatch bound `sltiu $v0, $v1, 0x12' with
   no minval subtraction, 8 distinct targets.  The entry list, in table order:

     case 0  .Ls21_800240B4   case 9  .Ls21_80024770 (default)
     case 1  .Ls21_80024160   case 10 .Ls21_80024770 (default)
     case 2  .Ls21_800243B4   case 11 .Ls21_80024770 (default)
     case 3  .Ls21_800245D4   case 12 .Ls21_80024770 (default)
     case 4  .Ls21_80024628   case 13 .Ls21_80024770 (default)
     case 5  .Ls21_80024770 (default)   case 14 .Ls21_80024770 (default)
     case 6  .Ls21_80024770 (default)   case 15 .Ls21_80024770 (default)
     case 7  .Ls21_80024770 (default)   case 16 .Ls21_80024698
     case 8  .Ls21_80024770 (default)   case 17 .Ls21_80024738

   ELEVEN of the eighteen entries carry the DEFAULT label -- the same
   .Ls21_80024770 the range-check `beqz $v0' jumps to -- so there are SEVEN
   real arms with seven distinct labels and NOTHING TO HAND-SETTLE about arm
   order: the source is written 0, 1, 2, 3, 4, 16, 17 with no `default:' and
   cases 5..15 are gcc's pad to maxval, NOT empty `break' arms.  Writing them
   out would be eleven phantom labels.  The arms' entry addresses are already
   ascending in case order.  18 is an EVEN `.word' count and there is no pad
   word, because the case count is 18 as well -- read the `sltiu' bound, not
   the word count.

   WHAT IT IS.  `p' is this node's payload; `o = *(u8 **)p' is the CASTER's
   payload, `n' the caster's node (`o - 0x20') and `m' its mover.  0
   initialises the renderable, the ring banks and the facing and falls into 1;
   1 reads the caster's hand through func_8003DF74, mirrors its position into
   the mover, tags the node with D_80045340, picks the target tile off the
   caster's +0x60 target (or its own mover when there is none) and seeds the
   tile-walk step count and the D_8006CCD8/E8 facing pair; 2 integrates and
   clamps the mover's two horizontal velocities at +-0x200000, eases z, spins
   by func_800644B8, spawns a func_s21_8002495C marker a frame, and either
   arrives (state 16) or steps one tile when func_800A45D8 says the next tile
   is clear; 3 rings func_80065F90 and spawns the func_s21_800262AC burst;
   4 fades the renderable and kills the node once the burst dies; 16 drifts and
   fades; 17 waits for D_80026472 to empty.

   LANDED OUT OF docs/dng-parks/func_s21_80024050.c, WHICH STOOD AT 61 ALIGNED
   ROWS.  The park's shape, its word count and its declaration block were all
   correct and are kept verbatim; what it was missing is a twin.  Its header
   records "NO TWIN ... func_s19_80024050 at 0.843 ... STILL INCLUDE_ASM", and
   that body landed in batch 26.  Three of its six facts carried straight over
   and were worth 47 of the 61 rows; the other 14 came off my own listing.
   Both of the park's `do { } while (0)' pins are GONE -- see the clamp row.

   SEVEN FACTS, every one priced BOTH WAYS from this body at 0 differing
   (aligndiff; ovlverify inflates any row that shifts a word):

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | case 0's +0x16 facing read as an ARRAY REF,           |       |      |
   |   `((u16 *)o)[0x15]'                           KEPT   |  471  |   0  |
   |   ... `*(u16 *)(o + 0x2A)', a plain cast             |  471  |  16  |
   | case 1's two join stores DUPLICATED INTO BOTH ARMS,   |       |      |
   |   +0xA then +0x32, with A LOCAL PER ARM        KEPT   |  471  |   0  |
   |   ... duplicated but sharing one local               |  471  |   3  |
   |   ... written ONCE after the if/else, one local      |  470  |   9  |
   | case 1's tile distance as ONE `__builtin_abs' of a    |       |      |
   |   ternary                                      KEPT   |  471  |   0  |
   |   ... the park's `d = x - y; if (x == y) d = z - w;   |       |      |
   |         if (d < 0) d = -d;'                          |  471  |  14  |
   | both velocity clamps as `k = X + (X >> 4)' then       |       |      |
   |   `__builtin_abs(k)', NO DEVICE                KEPT   |  471  |   0  |
   |   ... the park's `a = X; do { a += a >> 4; }          |       |      |
   |         while (0); b = a; if (b < 0) b = -b;'        |  471  |   2  |
   | func_800A45D8 through an `s32'-parameter alias with   |       |      |
   |   the masks written INLINE and `(s16)' on the result  |  471  |   0  |
   |   ... the park's `s16 f(u16, u16, s32)' prototype     |       |      |
   |         with three statements into gx/gy locals      |  470  |  19  |
   | case 2's D_800269C8 store written BEFORE the +0xA     |       |      |
   |   bump                                         KEPT   |  471  |   0  |
   |   ... after it, as the park has it                   |  473  |  11  |
   | case 2's +0xA bump as an ARRAY REF,                   |       |      |
   |   `((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1'      KEPT   |  471  |   0  |
   |   ... `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1'    |  469  |  11  |

   THE LAST TWO ARE A CONJUNCTION, and each alone reads 11.  They are also the
   only pair here that touches one instruction, which is the brief's test for
   when to cross two facts rather than add them.

   THE TWO BIGGEST ROWS ARE BOTH `true_dependence' (sched.c:817), not
   allocation, and they run in OPPOSITE directions on the same rule -- "a
   MEM_IN_STRUCT reference at a varying address never conflicts with a
   non-MEM_IN_STRUCT reference at a FIXED address":
     * case 0 wants the dependence GONE.  `((u16 *)o)[0x15]' is an ARRAY_REF,
       so MEM_IN_STRUCT_P is set and its address varies, while the
       `*(s16 *)&D_80026478[0xC]' store beside it is a plain cast at a
       SYMBOL address; the exemption fires and sched1 hoists the `lhu' above
       both D_80026484 stores.  That one hoist is the whole of case 0's
       14-row register displacement: with the `lhu' up there $v0 is taken, the
       0x1010 constant is pushed to $v1, and the D_80026484 base is pushed out
       of $v1/$a3 into the target's $t0/$t1 -- which frees $a3 for
       func_800B835C's fourth argument at target word 36 instead of word 57.
       As a plain cast the exemption does not fire, because then NEITHER memref
       is in-struct, and 16 rows come back.  The park spent sixteen `do { }
       while (0)' pins on this symptom; none of them could reach it.
     * case 2's +0xA bump wants the dependence GONE too, so the D_800269C8
       store can sink past it into the `j's delay slot.
   Case 1's duplicated stores are the same pass seen from the other side: with
   both stores inside the arms, cross-jumping merges them into the join block
   AFTER sched1 has run, so the `lw $v0, 0x4($s0)' that follows never gets the
   chance to hop over `sh $v0, 0x32($s0)'.  Written once in the join block the
   hop is legal (`p + 0x32' and `p + 0x4' are provably disjoint off one base)
   and sched1 takes it -- which is also what moves `z' out of $v0 and lets the
   store sink into the load-delay slot the target leaves as a `nop'.

   MEASURED AND FREE, so none of these is why this lands (all 471 / 0):
   the dead `s32 d' declaration left over from the park; `k1'/`k2' index
   locals for the two +0xE/+0x12 velocity stores against writing
   `D_8006CCD8[i] * 8' inline (the park's `<< 3' form and `* 8' both land);
   and `out' declared `u16 out[4]' or `u16 z' / `s16 z' for the join local.
   NOT MEASURABLE: which of cases 5..15 the table's shared entries name.

   `(s16)func_800A45D8w(...)' needs the s32 alias for two reasons at once: the
   `s32' parameters keep `& 0xFFE0' in SImode (through the landed `u16'
   prototype convert.c narrows the mask to HImode and cc1psx materialises it),
   and the s32 RESULT is what gives the target's `sll $v0, $v0, 16' before the
   `beqz'.  Both are func_s19_80024050's facts, reproduced here.

   relcand reports `D_80026484' in the listing against `D_80026478' in this
   code.  That is the park's documented -0xC alias and it is BYTE-IDENTICAL:
   the two symbols share a `%hi' and `addiu $t1, $t0, %lo(D_80026478) + 0xC'
   assembles to the same `0x6484' the target shows.  Verified at words 31/32
   under `ovlverify --raw'.

   471 words, 0 differing; raw 61 == the function's relocated and local-label
   fields. */
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
    s32 w;
    s32 a2;
    s32 b2;
    s32 w2;
    s32 gx;
    s32 gy;
    s32 z;
    s32 z2;
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
        *(s16 *)(p + 0x16) = (((u16 *)o)[0x15] >> 9) & 0x7;
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
            *(s16 *)((u8 *)v + 0xA) = z;
            *(s16 *)(p + 0x32) = z;
        }
        else
        {
            z2 = *(u16 *)(m + 0xA) + out[2];
            *(s16 *)((u8 *)v + 0xA) = z2;
            *(s16 *)(p + 0x32) = z2;
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
            D_800269C8 = *(u16 *)(*(u8 **)(o + 0x60) + 0x88);
            ((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1;
            break;
        }
        p[0x20] = p[0x1E];
        p[0x21] = p[0x1F];
        if (--*(s16 *)(p + 0x14) != 0)
        {
            if ((s16)func_800A45D8w(((*(s8 *)(p + 0x1E) << 6) + 0x20) & 0xFFE0,
                                    ((*(s8 *)(p + 0x1F) << 6) + 0x20) & 0xFFE0,
                                    *(s16 *)(p + 0x32)) == 0)
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

/* BEGIN func_s21_800247AC */
/* The simplest step handler in the unit: bump D_80026472, count the payload's
   own +0x1A halfword up, and retire the node at 0x101.  22 words, 0 differing.

   csrc/ovl_dungeon_800A4B88.c's func_800A5D5C is the twin xsibs names at
   0.783 and its `if (++*(s16 *)p >= 0x10)' idiom carries over unchanged --
   `lhu'/`addiu 1'/`sh'/`sll 16'/`sra 16'/`slti' with the guard reading the
   stored halfword back.

   THE ONE SPELLING FACT: the payload counter is `((s16 *)p)[0xD]', AN ARRAY
   REFERENCE, not `*(s16 *)(p + 0x1A)'.  The cast-dereference form leaves
   MEM_IN_STRUCT_P clear on that MEM, sched1 then has to assume it may alias
   D_80026472, and the two read-modify-writes serialise: the counter's `sh'
   stays in source position instead of sinking into the `bnez' delay slot and
   both `lhu's get an unfilled load-delay `nop'.  Measured:

     `((s16 *)p)[0xD]' with D_80026472++ first   (this body)  22 words,  0
     `*(s16 *)(p + 0x1A)' with D_80026472++ first            24 words, 23
     `*(s16 *)(p + 0x1A)' with D_80026472++ last             23 words, 23
     the counter split into `t = D; ... ; D = t + 1;'        21 words, 22

   raw 6 == the 6 relocations: 1 HI16 + 2 LO16 on D_80026472 and the same
   three on D_800814A0. */
void func_s21_800247AC(u8 *p)
{
    D_80026472++;
    if (++((s16 *)p)[0xD] >= 0x101)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for the ring of sixteen trail nodes: take a node off
   func_8003FD64 with this payload's own node as the template, park
   func_s21_800247AC as its +0x10 step and func_s21_80024A68 through
   func_8004491C as its draw, copy the caller's mover position into the new
   mover's +0x2/+0x6/+0xA, stash the fourth argument in the payload's first
   word, then call func_s21_8002522C sixteen times with the ring angle
   `i << 8', a 0x40 radius and a -0x100 z.  The node is returned.

   THE THIRD PARAMETER IS NEVER READ -- `$a2' is dead on entry and the first
   thing to touch it is the `addiu $a2, $zero, 0x40' inside the loop -- but it
   has to be declared, because `$a3' IS read.  No caller exists in this
   container, so the arity comes from the prologue alone.

   dngtwin's best is func_s69_80025FCC at 0.639; what actually wrote it is the
   LISTING'S PROLOGUE plus this unit's own two landed spawners
   (func_s21_800254FC for the `n->unk_10 = step' / func_8004491C order and
   func_s21_8002522C, the callee, for the five-argument shape and the fact
   that its fifth argument is the func_8003FD64 template).  func_s69_80025FCC
   contributed nothing beyond confirming the order.

   THE ONE FACT, and it is the brief's parameter-reuse lever run in reverse.
   The target saves SIX callee-saved registers where the obvious body saves
   five, and the extra word is `addu $s3, $s4, $zero' sitting in the
   `lhu 0xA($s3)' load-delay slot: a register-to-register COPY of the
   `p - 0x20' template into the register `v' has just vacated.  gcc 2.7.2 has
   no copy coalescing between two GLOBAL pseudos, so the copy survives only if
   the source really assigns one variable from another -- and it must be the
   PARAMETER's own pseudo, because a fresh local gets folded away:

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | `v = (UnkVec *)d;' before the +0x20 store, then `v'   |       |      |
   |   passed as func_s21_8002522C's template       KEPT   |   56  |   0  |
   | a separate `UnkNode *e = d;' in the same place        |   54  |  54  |
   | the same separate local at the top of the `if'        |   54  |  54  |
   | `d' passed directly, no second variable               |   54  |  54  |

   All three rejected forms come out TWO WORDS SHORT in the same way -- one
   `sw'/`lw' pair, because the frame drops to five saves at -0x30 and `i'
   takes over `v's register once `v' dies.  The 54 masked differences are the
   whole body shifting, not 54 faults.

   56 words, 0 differing; relcand ok (0 data symbols, 3 calls). */
UnkNode *func_s21_80024804(u8 *p, UnkVec *v, s32 arg2, u32 q)
{
    UnkNode *n;
    UnkNode *d;
    UnkVec *m;
    s32 i;

    d = (UnkNode *)(p - 0x20);
    n = func_8003FD64(0x2, d);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s21_800247AC;
        func_8004491C(n, (u32)func_s21_80024A68);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)v + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)v + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)v + 0xA);
        v = (UnkVec *)d;
        *(u32 *)((u8 *)n + 0x20) = q;
        for (i = 0; i < 0x10; i++)
        {
            func_s21_8002522C((u8 *)m, i << 8, 0x40, -0x100, v);
        }
    }
    return n;
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
   1 HI16 + 2 LO16 on D_80026472 and the same three on D_800814A0. */
void func_s21_800248E4(u8 *p, UnkVec *v, u8 *c)
{
    D_80026472++;
    c[0xC] -= c[0xC] >> 3;
    c[0xD] -= c[0xD] >> 3;
    c[0xE] -= c[0xE] >> 3;
    if (c[0xC] < 8)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_s21_800248E4's fading marker.
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
   func_s21_800248E4 and func_800CEEFC. */
UnkNode *func_s21_8002495C(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s21_800248E4;
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
   live entry of D_800265C8, each quad spanning entry `i', the next entry in
   its 0x10-wide ring, and the two matching entries of the second bank at
   &D_800265C8[0x10].  155 words, 0 differing, FIRST COMPILE.

   Written against func_s20_80025340's landed body in this same unit (the
   D_80083160/+0x8D0 cursor prologue, the `sp->unk_18' before `sp->unk_20'
   order, the GetTPage/func_80067F20/AddPrim tail) plus
   func_s20_80026010's index arithmetic, so nothing here was fought for.

   Note the contrast with func_s20_80025340's trail-ring store, which needs an
   integer address and an explicit `<< 3': here the PLAIN ARRAY REF is right.
   `D_800265C8[i + 0x10]' folding to `i * 8 + (base + 0x80)' is exactly the
   target's `addiu $a3, $s4, 0x80', and an array ref off a loop-invariant
   SYMBOL also gives the `addu $v0, $v1, $s4' operand order that an array ref
   off a pointer LOCAL does not.

   The two colour pairs are CHAINED assignments: `*(u32 *)&q->r0 =
   *(u32 *)&q->r1 = D_800265C0' emits +0x0C before +0x04, which is the order
   the target has in both arms.  RotTransPers4 is RotTransPers4 and
   SetPolyG4 is SetPolyG4 -- a real call, not the setPolyG4 macro, which
   is why the tag and code bytes are written after the four colour words. */
s32 func_s21_80024A68(void)
{
    UnkDng24EF4SP *sp = (UnkDng24EF4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_G4 *q;
    DR_MODE *dm;
    s32 i;

    sp->unk_18 = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = D_80083160.unk_00 + 0xB0;
    for (i = 0; i < D_80026476; i++)
    {
        q = (POLY_G4 *)sp->unk_18;
        sp->unk_18 = (u8 *)(q + 1);
        *(u32 *)&sp->unk_70[0] = *(u32 *)&D_800265C8[i];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&D_800265C8[(i & -0x10) + ((i + 1) & 0xF)];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&D_800265C8[i + 0x10];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&D_800265C8[(i & -0x10) + ((i + 1) & 0xF) + 0x10];
        sp->unk_70[1].vz = sp->unk_70[0].vz = D_800265C8[i].unk_04;
        sp->unk_70[3].vz = sp->unk_70[2].vz = D_800265C8[i + 0x10].unk_04;
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            if (i % 0x20 < 0x10)
            {
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_800265C0;
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_800265C4;
            }
            else
            {
                *(u32 *)&q->r2 = *(u32 *)&q->r3 = D_800265C0;
                *(u32 *)&q->r0 = *(u32 *)&q->r1 = D_800265C4;
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

/* BEGIN func_s21_80024CD4 */
/* The step handler of the 0x202 node func_s21_800262AC-family code parks on
   func_s21_8002522C: a four-state machine on the payload's +0xA counter that
   ramps the renderable's +0x6 scale by 0x80 a frame.  State 0 ramps up, caps
   at 0x100, spawns one func_s21_8002522C trail node per frame while the
   facing at +0x22 is under 0x61, advances the state and raises D_80026474 at
   facing 0x6F; state 1 waits for D_80026474 and flips the scale to -0x100;
   state 2 counts +0x1A down and falls into state 3, which ramps the negative
   scale back toward 0 and kills the node when it reaches it.  Then every path
   writes the 0x20/0xE0/0x40 colour triple and calls func_s21_800262AC with
   the mover's position.  127 words, 0 differing, FIRST COMPILE.

   Four facts were read off the listing rather than guessed, and all four held:

   * `c[0xC] = 0x20;' AND THE COLOUR TRIPLE ARE AFTER THE `switch', not in
     each arm -- every arm reaches the same `sb $v0, 0xC($s1)'.
   * CASE 2 FALLS THROUGH INTO CASE 3, with `if (--*(s16 *)(p + 0x1A) > 0)
     break;' as its guard.  That is the `bgtz' to the shared tail with the
     increment on the fall-through, and the two arms share one `sh 0x6' ramp.
   * THE TWO ARGUMENT FIELDS ARE STORED IN THE ORDER +0x18 THEN +0x1C AND READ
     BACK IN THE SAME ORDER.  The +0x1C argument is cse-forwarded from its own
     store (`sll`/`sra` on the stored register) while the +0x18 argument is a
     real `lh' reload, because the +0x1C store lands between the +0x18 store
     and the read and wipes cse's memory table.  The other store order loses
     that asymmetry.
   * `>= 0x101' NEEDS THE FULL `sll 16`/`sra 16`/`slti' while state 3's
     `>= 0' is just `sll 16`/`bltz' -- both come free from reading the stored
     halfword back.

   The constant 1 in `$s3' across the whole body is cse sharing the `case 1:'
   comparison value with `D_80026474 = 1;'; nothing in the source says so.

   raw 16 == the 16 relocations: 1 HI16 + 2 LO16 on D_80026472, the same three
   on D_80026474 (HI16 + LO16 for the `lh', HI16 + LO16 for the `sh' -- 4),
   1 HI16 + 2 LO16 on D_800814A0, and R_MIPS_26 on func_s21_8002522C and
   func_s21_800262AC. */
void func_s21_80024CD4(u8 *p, UnkVec *v, u8 *c)
{
    D_80026472++;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(c + 0x6) = *(u16 *)(c + 0x6) + 0x80;
        if (*(s16 *)(c + 0x6) >= 0x101)
        {
            *(s16 *)(c + 0x6) = 0x100;
            if (*(s16 *)(p + 0x22) < 0x61)
            {
                *(s16 *)(p + 0x18) =
                    *(u16 *)(p + 0x18) - ((s16)(*(u16 *)(p + 0x18)) >> 3);
                *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + 0x80;
                func_s21_8002522C((u8 *)v, *(s16 *)(p + 0x16), *(s16 *)(p + 0x18),
                                  *(s16 *)(p + 0x1C), p - 0x20);
            }
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            if (*(s16 *)(p + 0x22) == 0x6F)
            {
                D_80026474 = 1;
            }
        }
        break;
    case 1:
        if (D_80026474 != 0)
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
    c[0xD] = 0xE0;
    c[0xE] = 0x40;
    func_s21_800262AC(*(s16 *)((u8 *)v + 0x2), *(s16 *)((u8 *)v + 0x6),
                      *(s16 *)((u8 *)v + 0xA));
}

/* One semi-transparent gouraud LINE_G2 and one DR_MODE per node of the tag
   chain: project the mover's own origin and the point it is interpolated
   towards through the 0x1F800000 scratchpad, average the two depths, and --
   on the forward arm -- record the interpolated point in this bank's trail
   ring at D_800265C8.  215 words, 0 differing.

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
s32 func_s21_80024ED0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng73E94SP *sp = (UnkDng73E94SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkDng26478 *t = D_800265C8;
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
            if (D_80026476 < *(s16 *)(p + 0x22) + 0x10)
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

/* The 0x202 spawner for func_s21_80024CD4's step and func_s21_80024ED0's
   renderer.  It seeds the new mover's position from the caller's own position
   offset by rcos/rsin of `ry' scaled by `rx' (>> 15) and by `rz >> 3' in z,
   clamps the z component at D_800269C8, copies the unoffset position into the
   mover's +0xE/+0x12/+0x16 interpolation target, writes 0x40E020 into the
   renderable's colour word, records the three angles and the current ring
   cursor in the payload, and -- while the cursor is under 0x71 -- writes the
   interpolation target into BOTH trail-ring banks at D_800265C8[cursor] and
   D_800265C8[cursor + 0x10] and bumps the cursor.  115 words, 0 differing.

   LANDED OUT OF docs/dng-parks/func_s21_8002522C.c, WHICH STOOD AT 1
   DIFFERING.  Its residue was one word: target word 82, the `sh' in the
   `beqz' delay slot, `sh $a3, 0x22($v1)' where the park's body had
   `sh $a2, ...'.  $a3 is the raw `lhu' of D_80026476 and $a2 its sign
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
   base and deletes the `addiu/sll/addu'), `c = (u16)D_80026476;' is what
   turns the unit's `extern s16 D_80026476' into the target's `lhu', and
   D_800269C8 is read twice -- `lh' for the compare and `lhu' for the store --
   which is why it is declared `u16' with an explicit `(s16)' on the compare.

   The five-argument signature is the one func_s21_80024CD4 calls with (the
   fifth argument on the stack at 0x10($sp)); func_s21_80024CD4 re-verified
   at 127 words / 0 differing with this body in place.

   raw 16 == the 13 relocations plus three local branch targets: R_MIPS_26 on
   func_8003FD64, func_8004491C, func_80064584 and func_800644B8, and
   HI16/LO16 pairs on func_s21_80024CD4, func_s21_80024ED0, D_800269C8 (one
   HI, two LO), D_80026476 (one HI, two LO) and D_800265C8. */
UnkNode *func_s21_8002522C(u8 *p, s16 ry, s16 rx, s16 rz, void *tmpl)
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
        n->unk_10 = (u32)func_s21_80024CD4;
        func_8004491C(n, (u32)func_s21_80024ED0);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = *(u16 *)(p + 0x2) + ((func_80064584(ry) * rx) >> 15);
        *(s16 *)(m + 0x6) = *(u16 *)(p + 0x6) + ((func_800644B8(ry) * rx) >> 15);
        *(s16 *)(m + 0xA) = *(u16 *)(p + 0xA) + (rz >> 3);
        if ((s16)D_800269C8 < *(s16 *)(m + 0xA))
        {
            *(s16 *)(m + 0xA) = D_800269C8;
        }
        *(s16 *)(m + 0xE) = *(u16 *)(p + 0x2);
        *(s16 *)(m + 0x12) = *(u16 *)(p + 0x6);
        *(s16 *)(m + 0x16) = *(u16 *)(p + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x40E020;
        q = (u8 *)n + 0x20;
        c = (u16)D_80026476;
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
            D_800265C8[f].unk_00 = D_800265C8[e].unk_00 = *(u16 *)(m + 0xE);
            D_800265C8[f].unk_02 = D_800265C8[e].unk_02 = *(u16 *)(m + 0x12);
            D_800265C8[f].unk_04 = D_800265C8[e].unk_04 = *(u16 *)(m + 0x16);
            D_80026476 = c + 1;
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
   1 HI16 + 2 LO16 on D_80026472, the same three on D_800814A0, and
   R_MIPS_26 on func_800478B8. */
void func_s21_800253F8(u8 *p, UnkVec *v, u8 *c)
{
    D_80026472++;
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

/* The 0x202 spawner for func_s21_800253F8's puff.
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
   func_8003DB94, and HI16/LO16 pairs on D_80045340, func_s21_800253F8 and
   D_80026544. */
void func_s21_800254FC(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s21_800253F8;
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
        func_8003DB94(r, D_80026544, 0);
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
   of image (D_80026472); the best landed non-family neighbour is 0.363.
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
   OF THE LAST SEVEN WORDS. Written as a plain `D_80026472++' anywhere in the
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
     `D_80026472++' after the 0x48 corner (h13's park body)            7
     `D_80026472++' before it                                         13
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
   D_80026472, the same three on D_800814A0, and R_MIPS_26 on PushMatrix,
   RotMatrix, SetTransMatrix, SetRotMatrix, RotTransSV four times and
   PopMatrix. */
void func_s21_800255E8(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDng255E8SP *sp = (UnkDng255E8SP *)0x1F800000;
    SVECTOR rot;
    MATRIX m;
    s32 t;
    s16 n;
    u16 c;

    sp->unk_70[0].vx = *(u16 *)(p + 0x34) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[1].vx = *(u16 *)(p + 0x3C) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[2].vx = *(u16 *)(p + 0x44) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[3].vx = *(u16 *)(p + 0x4C) - *(u16 *)((u8 *)v + 0x2);
    sp->unk_70[0].vy = *(u16 *)(p + 0x36) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[1].vy = *(u16 *)(p + 0x3E) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[2].vy = *(u16 *)(p + 0x46) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[3].vy = *(u16 *)(p + 0x4E) - *(u16 *)((u8 *)v + 0x6);
    sp->unk_70[0].vz = *(u16 *)(p + 0x38) - *(u16 *)((u8 *)v + 0xA);
    sp->unk_70[1].vz = *(u16 *)(p + 0x40) - *(u16 *)((u8 *)v + 0xA);
    c = D_80026472;
    t = *(u16 *)(p + 0x48) - *(u16 *)((u8 *)v + 0xA);
    D_80026472 = c + 1;
    sp->unk_70[2].vz = t;
    sp->unk_70[3].vz = *(u16 *)(p + 0x50) - *(u16 *)((u8 *)v + 0xA);
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
    *(s16 *)(p + 0x34) = sp->unk_98[0].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x3C) = sp->unk_98[1].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x44) = sp->unk_98[2].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x4C) = sp->unk_98[3].unk_00 + *(u16 *)((u8 *)v + 0x2);
    *(s16 *)(p + 0x36) = sp->unk_98[0].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x3E) = sp->unk_98[1].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x46) = sp->unk_98[2].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x4E) = sp->unk_98[3].unk_02 + *(u16 *)((u8 *)v + 0x6);
    *(s16 *)(p + 0x38) = sp->unk_98[0].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x40) = sp->unk_98[1].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x48) = sp->unk_98[2].unk_04 + *(u16 *)((u8 *)v + 0xA);
    *(s16 *)(p + 0x50) = sp->unk_98[3].unk_04 + *(u16 *)((u8 *)v + 0xA);
    p[0x54] -= p[0x54] / *(s16 *)(p + 0x1A);
    p[0x55] -= p[0x55] / *(s16 *)(p + 0x1A);
    p[0x56] -= p[0x56] / *(s16 *)(p + 0x1A);
    p[0x58] -= p[0x58] / *(s16 *)(p + 0x1A);
    p[0x59] -= p[0x59] / *(s16 *)(p + 0x1A);
    p[0x5A] -= p[0x5A] / *(s16 *)(p + 0x1A);
    n = *(u16 *)(p + 0x1A) - 1;
    *(s16 *)(p + 0x1A) = n;
    if (n <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* BEGIN func_s21_800259DC */
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
s32 func_s21_800259DC(u8 *p, UnkVec *v, UnkSlot *s)
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
        *(u32 *)&sp->unk_70[0] = *(u32 *)(p + 0x34);
        *(u32 *)&sp->unk_70[1] = *(u32 *)(p + 0x3C);
        *(u32 *)&sp->unk_70[2] = *(u32 *)(p + 0x44);
        w = *(u32 *)(p + 0x4C);
        sp->unk_70[0].vz = *(u16 *)(p + 0x38);
        sp->unk_70[1].vz = *(u16 *)(p + 0x40);
        sp->unk_70[2].vz = *(u16 *)(p + 0x48);
        *(u32 *)&sp->unk_70[3] = w;
        sp->unk_70[3].vz = *(u16 *)(p + 0x50);
        sp->unk_C0 = RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                   (s32 *)&q->x0, (s32 *)&q->x1, (s32 *)&q->x2, (s32 *)&q->x3,
                                   &sp->unk_90, &sp->unk_94) -
                     0x8;
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&q->r0 = *(u32 *)&q->r1 = *(u32 *)(p + 0x54);
            *(u32 *)&q->r2 = *(u32 *)&q->r3 = *(u32 *)(p + 0x58);
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

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s21", func_s21_80025BA0);

/* BEGIN func_s21_80025E48 */
/* The step handler of the 0x202 node that func_s21_8002522C spawns: a
   four-state machine on the payload's +0xA counter.  Every frame it bumps
   D_80026472, aims the mover at an angle built from the payload's +0x22 facing
   and its +0x18 sweep (added on the +0x24 arm, subtracted on the other) and
   writes the rcos/rsin pair into the mover's position AND its +0xE/+0x12
   interpolation target scaled by +0x66; then state 0 grows +0x66 toward +0x64
   and fades the renderable's +0xD alpha toward 0xF0 or 0x20, state 1 drifts
   the mover's +0x16 and spins +0x26, state 2 drives D_80026478's +0xC/+0xD
   pair toward 0x14 and rings func_800B835C, and state 3 does the same toward
   0x10, converges +0x66 on half of +0x64, advances the sweep, and kills the
   node when +0x1A runs out.  281 words, 0 differing.

   NO TWIN: dngfam.py gives this body one copy and xsibs finds nothing at
   0.70.  It is written from the listing, against the unit's own landed
   neighbours for the idioms (func_s21_800253F8's `--*(s16 *)(p + 0x1A) <= 0',
   func_s21_8002522C's rcos/rsin-times-scale pair, func_s21_800248E4's kill
   tail).  The four switch arms are written as a `switch'; with four cases
   cc1psx emits the comparison tree and NO .rodata jump table, and the listing
   has no `.section .rodata'.

   SIX SPELLINGS WERE MEASURED, each worth between 1 and 11 words:

   * `a <<= 16; a >>= 16;' AS TWO STATEMENTS for the angle's (s16) narrowing.
     `a = (s16)a;' puts the `sll' in a temporary and the `sra' back into `a'
     (2 differing); declaring `a' as `s16' instead narrows the whole
     computation and turns both `lh 0x18' into `lhu' (20 aligned differences);
     `(s16)a' at the four call sites is 15.
   * EVERY DIVIDEND AND EVERY `c[0xD]' READ HAS ITS OWN LOCAL (d0, d2, d3, d4,
     d5).  One shared `d' reused at all five sites is 76 aligned differences:
     the five quantities merge, `lbu' lands in $t0 instead of $v1 at both
     case-3 tails and `mflo' moves from $t2 to $t1 in all four prologue
     multiplies.
   * THE TWO `arr' STORES COME AFTER the D_80026478 read-modify-write, not
     before: ahead of it the `lui/ori' pairs are emitted before the `div' and
     the whole call block is 12 words out of order.
   * `*(s16 *)(p + 0x64) > *(s16 *)(p + 0x66)' AND NOT `0x66 < 0x64'.  The
     comparison is canonicalised to the same `slt' either way, but the written
     operand order fixes which `lh' is emitted first: 2 differing the other
     way round.
   * THE +0x22 BASE IS COMPUTED BEFORE `D_80026472++', not after: 6 differing
     the other way, because the `lh 0x22'/`sll 8' pair then lands after the
     counter's `addiu`/`sh` and the $s3 parameter copy is pushed out of the
     prologue's run of copies.
   * THE ALPHA TARGET IS AN INLINE CONDITIONAL EXPRESSION, NOT A LOCAL.
     `e = (... == 0) ? 0x20 : 0xF0;' then `(e - d0)' measures 5 differing --
     `e' takes a register of its own instead of reusing the condition's $v0
     and everything in case 0 shifts one register.  Written inline it is 0.
     `if/else' in place of the conditional is 6, and the arms the other way
     round (`!= 0 ? 0xF0 : 0x20') is also 6.

   THE ONE DEVICE: `b2 = b;'.  The target computes the +0x22 base once, before
   the branch, and the ELSE arm reads a COPY of it ($v1) while the then arm
   reads the original ($a0) -- `addu $v1, $a0, $zero', which reorg then sinks
   into the `beqz' delay slot.  A single local is coalesced away and the body
   comes out 280 words:

     b, b2 = b, then uses b and else uses b2   (this body)   281 words,  0
     b, b2 = b, then uses b2 and else uses b                 280 words, 16
     one local b, `a = b +/- x' per arm                      280 words, 17
     one local a, `a = base' then `a +/- = x' per arm        280 words, 16
     `a = b; ...' ahead of the if                            280 words, 16
     the base inlined in both arms                           280 words, 26
     a ternary over the whole thing                          280 words, 17
     `else { a = b; a -= x; }'                               280 words, 17

   Probably MACRO RESIDUE rather than a hand-written second name: a macro that
   names the base and is used in both arms gives the second evaluation for
   free, and cse1 turns the second one into exactly this copy.  I could not
   find a device-free spelling.

   raw 23 == the 23 relocations: 1 HI16 + 2 LO16 on D_80026472, R_MIPS_26 on
   func_80064584 and func_800644B8 twice each, HI16/LO16 pairs on D_80026478
   four times (two per call block, one for the byte pair and one for the
   argument), R_MIPS_26 on func_800B835C twice, and 1 HI16 + 2 LO16 on
   D_800814A0. */
void func_s21_80025E48(u8 *p, UnkVec *v, u8 *c)
{
    s32 arr[2];
    s32 a;
    s32 b;
    s32 b2;
    s32 d0;
    s32 d2;
    s32 d3;
    s32 d4;
    s32 d5;
    s32 n;

    b = *(s16 *)(p + 0x22) << 8;
    b2 = b;
    D_80026472++;
    if (*(s16 *)(p + 0x24) != 0)
    {
        a = b + (*(s16 *)(p + 0x18) << 4);
    }
    else
    {
        a = b2 - (*(s16 *)(p + 0x18) << 4);
    }
    a <<= 16;
    a >>= 16;
    *(s16 *)((u8 *)v + 0x2) = *(u16 *)(p + 0x5C) + ((*(s16 *)(p + 0x66) * func_80064584(a)) >> 10);
    *(s16 *)((u8 *)v + 0x6) = *(u16 *)(p + 0x5E) + ((*(s16 *)(p + 0x66) * func_800644B8(a)) >> 10);
    *(s16 *)((u8 *)v + 0xE) = *(u16 *)(p + 0x5C) + ((*(s16 *)(p + 0x66) * func_80064584(a)) >> 10);
    *(s16 *)((u8 *)v + 0x12) = *(u16 *)(p + 0x5E) + ((*(s16 *)(p + 0x66) * func_800644B8(a)) >> 10);
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if (*(s16 *)(p + 0x64) > *(s16 *)(p + 0x66))
        {
            *(s16 *)(p + 0x66) = *(u16 *)(p + 0x66) + 1;
        }
        d0 = c[0xD];
        n = *(s16 *)(p + 0x1A);
        c[0xD] = d0 + (((*(s16 *)(p + 0x24) == 0) ? 0x20 : 0xF0) - d0) / n;
        if (--*(s16 *)(p + 0x1A) <= 0)
        {
            *(s16 *)(p + 0x1A) = 0x18;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 1:
        *(s16 *)((u8 *)v + 0x16) =
            *(u16 *)((u8 *)v + 0x16) + ((s16)(*(u16 *)(p + 0x26)) >> 1);
        *(s16 *)(p + 0x26) = *(u16 *)(p + 0x26) + 1;
        if (--*(s16 *)(p + 0x1A) <= 0)
        {
            *(s16 *)(p + 0x1A) = 0x10;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x24) == 0)
        {
            if (*(s16 *)(p + 0x22) == 0)
            {
                d2 = D_80026478[0xC];
                D_80026478[0xD] = D_80026478[0xC] = d2 + (0x14 - d2) / *(s16 *)(p + 0x1A);
                arr[0] = 0x1000340;
                arr[1] = 0x200020;
                func_800B835C(D_80026478, arr, 1, 0);
            }
        }
        if (--*(s16 *)(p + 0x1A) <= 0)
        {
            *(s16 *)(p + 0x1A) = 0x8;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 3:
        *(s16 *)((u8 *)v + 0xA) =
            *(u16 *)((u8 *)v + 0xA) +
            (*(s16 *)((u8 *)v + 0x16) - *(s16 *)((u8 *)v + 0xA)) / *(s16 *)(p + 0x1A);
        *(s16 *)(p + 0x66) =
            *(u16 *)(p + 0x66) +
            (((s16)(*(u16 *)(p + 0x64)) >> 1) - *(s16 *)(p + 0x66)) / *(s16 *)(p + 0x1A);
        *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) + (*(s16 *)(p + 0x1C))++;
        if (*(s16 *)(p + 0x24) == 0)
        {
            if (*(s16 *)(p + 0x22) == 0)
            {
                d3 = D_80026478[0xC];
                D_80026478[0xD] = D_80026478[0xC] = d3 + (0x10 - d3) / *(s16 *)(p + 0x1A);
                arr[0] = 0x1000340;
                arr[1] = 0x200020;
                func_800B835C(D_80026478, arr, 1, 0);
            }
        }
        if (*(s16 *)(p + 0x24) != 0)
        {
            d4 = c[0xD];
            c[0xD] = d4 - d4 / (*(s16 *)(p + 0x1A) - 0x6);
            if (--*(s16 *)(p + 0x1A) < 0x7)
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        else
        {
            d5 = c[0xD];
            c[0xD] = d5 - d5 / *(s16 *)(p + 0x1A);
            if (--*(s16 *)(p + 0x1A) <= 0)
            {
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}

/* The 0x202 spawner for func_s21_80025E48's three-ring sweep: 48 nodes in a
   3 x 16 nest, all at the same position, each tagged with its ring index j at
   the payload's +0x24 and its slot index i at +0x22, and each pointing its
   renderable's +0x8 at D_80026490.  Returns the LAST node allocated, which is
   what func_s21_80024050's case 3 stores at p+0x2C and case 4 polls for the
   kill bit.  86 words, 0 differing.

   NO TWIN: dngtwin's best sibling is func_s69_80025FCC at 0.684 and it is
   still an INCLUDE_ASM.  Written from the listing against the unit's own
   landed spawners (func_s21_8002495C, func_s21_800254FC) for the
   func_8003FC64 / `n->unk_10 = step' / func_8004491C(n, tag) idiom.

   THE ONE SPELLING FACT, worth 6 differences: BOTH CONDITIONAL CONSTANTS ARE
   WRITTEN `!= 0' FIRST.  `(i != 0) ? 0x202 : 2' and `(j != 0) ? 0x100 : 0x800'
   give the target's `beqz' with the SECOND arm's constant in the delay slot;
   written `(i == 0) ? 2 : 0x202' and `(j == 0) ? 0x800 : 0x100' both come out
   `bnez' with the arms swapped -- 86 words, 6 aligned differences, 3 per site.

   THREE MORE FACTS, ALL MEASURED:

   * THE SIX MOVER STORES ARE PAIRED, FAR FIELD FIRST: +0xE then +0x2, +0x12
     then +0x6, +0x16 then +0xA.  Position-first (+0x2 then +0xE, ...) is 4
     aligned differences and so is grouping the three +0xE/+0x12/+0x16 stores
     ahead of the three position stores -- the pairing is what lets one
     register serve both stores of a pair with no reload.
   * `*(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);' IS A READ-BACK.  Writing the
     conditional expression a second time instead duplicates the whole
     `beqz'/`j' selection: 92 words, 34 aligned differences.  (Contrast
     func_s21_800254FC, whose two 0x1000 scales ARE the literal written twice.)
   * `q = (u8 *)n + 0x20;' IS A POINTER LOCAL.  The target's `addiu $v1, $s0,
     0x20' is that local; spelled as seven `*(s16 *)((u8 *)n + 0x3A)'-style
     offsets off the node the addiu folds into each store's displacement and
     the body is 85 words, 11 aligned differences.

   MEASURED AND FREE, so NOT the reason this lands: where the +0x1C read-back
   sits.  Moved up to directly after the +0x1E store (ahead of +0x14, +0x10
   and +0x8) it is still 86 words, 0 differing.

   raw 11 == 9 relocations plus 2 local `j' targets the mask does not cover:
   R_MIPS_26 on func_8003FA44, func_8003FC64 and func_8004491C, and HI16/LO16
   pairs on func_s21_80025E48, D_80026490 and func_800CEEFC.

   The declaration had to CHANGE: this unit already declared
   `void func_s21_800262AC(s16, s16, s16)' for func_s21_80024CD4.  It returns
   the node.  func_s21_80024CD4 re-verified at 127 words / 0 differing with
   the new prototype. */
UnkNode *func_s21_800262AC(s16 x, s16 y, s16 z)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *q;
    s32 i;
    s32 j;

    if (func_8003FA44(0x30) == 0)
    {
        return NULL;
    }
    for (j = 2; j >= 0; j--)
    {
        for (i = 0xF; i >= 0; i--)
        {
            n = func_8003FC64((i != 0) ? 0x202 : 2);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_s21_80025E48;
                func_8004491C(n, (u32)func_800CEEFC);
                m = (u8 *)n->unk_08;
                *(s16 *)(m + 0xE) = x;
                *(s16 *)(m + 0x2) = x;
                *(s16 *)(m + 0x12) = y;
                *(s16 *)(m + 0x6) = y;
                *(s16 *)(m + 0x16) = z;
                *(s16 *)(m + 0xA) = z;
                r = (u8 *)n->unk_0C;
                *(s16 *)(r + 0x6) = 0x10;
                *(s16 *)(r + 0x1E) = (j != 0) ? 0x100 : 0x800;
                *(s16 *)(r + 0x14) = 0xC;
                *(s16 *)(r + 0x10) = 0x20;
                *(u32 *)(r + 0x8) = (u32)D_80026490;
                *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x1A) = 0x8;
                *(s16 *)(q + 0x64) = 0x8;
                *(s16 *)(q + 0x22) = i;
                *(s16 *)(q + 0x24) = j;
                *(s16 *)(q + 0x5C) = x;
                *(s16 *)(q + 0x5E) = y;
                *(s16 *)(q + 0x60) = z;
            }
        }
    }
    return n;
}

/* Apply status kind 0x17 at a strength scaled off the second parameter, then
   print the D_800E1D28 message, both gated on func_8009D218 coming back zero.
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
   D_800E1D28. */
void func_s21_80026404(u8 *p, u8 d)
{
    u32 e;

    if (func_8009D218_2(p, 4) == 0)
    {
        e = d;
        e >>= 2;
        if ((s16)func_800A48F0_i(p, 0x17, e + 2) != 0)
        {
            func_80099844(p, D_800E1D28);
        }
    }
}
