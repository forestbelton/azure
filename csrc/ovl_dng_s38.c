#include "common.h"

s32 rand(void);

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
typedef struct UnkDng74334SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    u8 pad_1C[0x8];
    u8 *unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDng74334SP;
void func_s38_80024B6C(u8 *p);
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk64
{
    u8 b[0x40];
} UnkDngS58Blk64;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
extern s16 D_800259AC;

s32 func_s38_800248EC(u8 *p, UnkVec *v);

void func_s38_8002404C(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3);
typedef struct UnkDng264D4Vec
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
} UnkDng264D4Vec;
typedef struct UnkDng264D4SP
{
    u8 pad_00[0x30];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x28];
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    SVECTOR unk_70[4];
    u8 pad_90[0x8];
    UnkDng264D4Vec unk_98[4];
} UnkDng264D4SP;

typedef struct UnkDng24094SP
{
    u8 pad_00[0x8];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8 pad_3C[0x34];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8 pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8 pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
    u16 unk_100;
    u16 unk_102;
    u16 unk_104;
    u16 unk_106;
    u16 unk_108;
    u16 unk_10A;
} UnkDng24094SP;

UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800644B8(s32);
s32 func_800BCB04(s32, s32, s16);
extern u8 D_s38_80024004[];
extern u8 D_80025970[];
extern u8 D_80025988[];
extern u8 D_800259A0[];
extern u8 D_80045340[];
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_800E3D68;
extern s32 D_8008346C;
void func_800B835C(u8 *, s32 *, s32, s32);
u8 *func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
s32 func_800A4778(u16, u16, s16, s32);
s32 func_800A56E0(s32);
s32 func_8009D218(u8 *, s32, u8 *);
s32 func_800A6D30(void);
s32 func_800C8A3C(u8 *, s16, s16);

/* The bank's slot renderer: builds the slot's rotation composed with the
   camera matrix in the 0x1F800000 scratchpad, then per 0xC-byte element either
   calls the element's own callback (first byte bit 0x20) or draws a RIPPLED
   QUAD STRIP -- for every row from the element's +0x08 to +0x08 + +0x0A, two
   0x28-byte primitives whose left and right x offsets come from
   func_800644B8(ang) * the payload's +0x38 >> 16 with `ang' advanced by +0x3C
   and wrapped at 0x1000, and whose top two vertices are the PREVIOUS row's
   bottom two, carried in the 0x40-byte stack array `mm'.
   531 words, 0 differing, 0 aligned differences.
   This function owns NO jump table: its listing has no `.section .rodata'.

   THE DONOR IS NOT IN ANY OF THE THREE LEAD FILES AND COULD NOT HAVE BEEN.
   dngtwin finds no carved sibling at 0.60; dngnear finds no landed in-unit
   neighbour; and dngcallee's best tier was 2-of-17 reached by 31 unrelated
   functions, because NINE of this body's eleven callees are libgte routines
   that every landed body spells by their REAL NAMES, so the address form the
   tool searched for occurs nowhere in csrc/ at all:

     func_800649A0 PushMatrix   func_80065820 RotMatrix
     func_80064AE0 ReadRotMatrix func_80064840 CompMatrix
     func_80064BC0 ScaleMatrix  func_80064D80 SetTransMatrix
     func_80064CF0 SetRotMatrix func_80065590 RotAverage4
     func_80064A40 PopMatrix

   Re-running the callee-set intersection on the REAL NAMES puts fourteen
   functions at 9-of-9, and the one whose job this is turned out to be already
   named in include/structs.h: `Unk45CC4SP' -- "the FIFTH typedef for the
   0x1F800000 scratchpad" -- is this object's layout FIELD FOR FIELD
   (unk_08..unk_14 the four Unk3DBD0W, unk_20 the OT base, unk_24 the flags,
   unk_30/34/38 the scale, unk_70..unk_8C the four SVECTORs, unk_90/94
   RotAverage4's outputs, unk_C0 the otz, unk_D0 + unk_E4/E8/EC the rotation
   MATRIX, unk_100/102/104 the euler vector), and `Unk45CC4E' is the 0xC-byte
   element and `Unk45CC4P' the 0x28-byte primitive.  Its witness is the LANDED
   RESIDENT func_80045CC4 in csrc/unk_19434.c, 383 words against this 531.
   That body supplied the whole head, the element walk, the two sign-arm
   blocks, the four on-screen tests, the clamp pairs, the +0x0F flag gate and
   the handler tail; what is NEW here is the row loop, the two-primitive inner
   loop, the func_800644B8 ripple and the `mm' strip cache.

   First compile was 522 words / 109 aligned rows; it took eight.  THIRTEEN
   FACTS, all priced from this body AFTER it reached 0 (aligndiff):

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | a `do { } while (0)' round the +0x14 store      KEPT  |  531  |   0  |
   |   ... no wrapper at all                               |  530  |   6  |
   |   ... `if (1)' round the same statement                |  530  |   6  |
   | `e' RELOADED from `s->unk_08' at the top of the        |       |      |
   |   inner loop, the same local the outer walk            |       |      |
   |   advances by 0xC                               KEPT  |  531  |   0  |
   |   ... one cursor, no reload                            |  541  | 137  |
   | the two +0x88/+0x78 accumulator stores DUPLICATED      |       |      |
   |   into both sign arms                           KEPT  |  531  |   0  |
   |   ... written once after the if/else                   |  533  |   8  |
   | the four +0x8A/+0x82 stores duplicated into ALL        |       |      |
   |   FOUR arms of the second sign block            KEPT  |  531  |   0  |
   |   ... written once after the four arms                 |  527  |  62  |
   | `s16 a' for the accumulator                     KEPT  |  531  |   0  |
   |   ... `s32 a'                                          |  526  |  51  |
   | `s16 t' for the row-walked sign byte            KEPT  |  531  |   0  |
   |   ... `s32 t'                                          |  529  |  18  |
   | `s32 ang'                                       KEPT  |  531  |   0  |
   |   ... `s16 ang'                                        |  534  | 125  |
   | `s16 nrows' / `s16 row'                         KEPT  |  531  |   0  |
   |   ... both `s32'                                       |  530  |  88  |
   | `nrows' DECLARED BEFORE `row'                   KEPT  |  531  |   0  |
   |   ... `row' first                                      |  531  |   9  |
   | the clear loop's cursor recomputed INSIDE it as        |       |      |
   |   `b = &mm[i]'                                  KEPT  |  531  |   0  |
   |   ... `b = mm' outside with `b++' inside               |  530  |  25  |
   | the strip cursor as `(s16 *)(i * 2 + (s32)mm)'  KEPT  |  531  |   0  |
   |   ... `&mm[i]'                                         |  531  |   1  |
   | the two arms write +0x0C BEFORE +0x10           KEPT  |  531  |   0  |
   |   ... +0x10 first                                      |  530  |  17  |
   | the ripple chains written                              |       |      |
   |   `p->unk_74 = p->unk_84 = ...'                 KEPT  |  531  |   0  |
   |   ... `p->unk_84 = p->unk_74 = ...'                    |  531  |   4  |
   | `q->unk_0E' as an IF/ELSE                       KEPT  |  531  |   0  |
   |   ... as a ternary                                     |  531  |   2  |
   | `t' read off `s->unk_08' rather than off `e'    KEPT  |  531  |   0  |
   |   ... off `e'                                          |  530  |  14  |

   THE `do { } while (0)' IS A DEVICE AND I COULD NOT FIND THE SPELLING.  Its
   one job is a SCHEDULING barrier: without it sched2 lifts the call's
   `addu $a0, $s3, $zero' into the `lbu 0xB($s2)' load-delay slot that the
   target leaves as a `nop', which is the whole of the 6 rows and the one
   missing word.  `if (1)' round the identical statement is 6 -- the same as no
   wrapper -- so what is load-bearing is the LOOP NOTE and not the block, the
   same discriminator as the reference-bump case.  Most likely macro residue:
   the statement it wraps is the one scratchpad field the row loop sets per
   primitive, which is exactly the shape a configuration macro collapses to.

   THE CLEAR LOOP IS THE SUBTLER FACT.  `b = mm' outside the loop makes `b's
   initial value a known frame address, and loop.c then REBASES the biv onto
   `mm + 0x20' (the lowest of its four constant offsets), so the clear stores
   come out `0/8/0x10/0x18' off an `addiu $a1, $sp, 0x48' where the target has
   `0x20/0x28/0x30/0x38' off `mm' itself.  Recomputing `b = &mm[i]' inside the
   loop gives loop.c the SAME expression the strip cursor 200 words later
   uses, so cse makes one `&mm' pseudo (the target's `sw $t1, 0x84($sp)' /
   two `lw $t1, 0x84($sp)'), strength-reduces it to the biv the target has,
   and the rebase cannot happen.  An explicit `s16 *mb = mm;' long-lived
   local also fixes the rebase but costs an extra spill slot and 8 bytes of
   frame.

   TWO OF func_80045CC4'S OWN FACTS MEASURE FREE ON THIS RELINK:
   * it prices `q->unk_16' as ONE store (a ternary) against an if/else at
     126 WORDS, calling it "the subtler lever in the function".  Here the
     if/else is 531 words / 0 aligned differences -- FREE, not unmeasurable:
     the two source trees differ and the output does not, because jump2
     cross-joins the two `sh' and this body's q+4 giv has reference-count
     slack the twin's did not.  Note its sibling fact DOES hold here:
     `q->unk_0E' must stay an if/else, 2 rows as a ternary.
   * it prices the OT link as two `Unk45CC4T' BITFIELD stores against a
     hand-written `|' at 4 words plus two register fields.  Here libgpu's own
     `addPrim(p->unk_20 + p->unk_C0 * 4, q)' and the two bitfield stores both
     measure 531 / 0, so `addPrim' is kept -- it is what the original wrote.
   MEASURED AND FREE, so neither is why this lands: `((u8 *)e)[0xB] >> 1' for
   `e->unk_0B >> 1' inside the barrier (531 / 0), and the declaration order of
   `i' against `f' (531 / 0, three permutations).

   THE `e' RELOAD IS NOT A TRANSCRIPTION ERROR.  `lw $s2, 0x8($s6)' sits at
   the top of the two-primitive loop and `addiu $s2, $s2, 0xC' at the bottom
   of the element walk, on the same register, so the ROM really re-reads the
   slot's element list head inside the inner loop and the element cursor only
   ever advances past the first drawable element.  Writing it the coherent way
   -- one cursor, no reload -- is 541 words / 137 rows, so this is what shipped;
   the reload survives loop.c because the scratchpad and primitive stores in
   the loop are memrefs `invariant_p' cannot excuse against `s->unk_08'. */
void func_s38_8002404C(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3)
{
    Unk45CC4SP *p = (Unk45CC4SP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    s16 mm[0x20];
    Unk45CC4E *e;
    Unk45CC4P *q;
    s16 *b;
    s32 i;
    s32 ang;
    s16 nrows;
    s16 row;
    s16 t;
    s16 a;
    u8 f;

    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_E4 = *(s16 *)((u8 *)v + 0x2);
    p->unk_E8 = *(s16 *)((u8 *)v + 0x6);
    p->unk_EC = *(s16 *)((u8 *)v + 0xA);
    q = *(Unk45CC4P **)(D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    PushMatrix();
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38 = 0x2000;
    p->unk_100 = s->unk_16;
    p->unk_104 = s->unk_1A;
    p->unk_102 = s->unk_18;
    RotMatrix((SVECTOR *)&p->unk_100, (MATRIX *)p->unk_D0);
    ReadRotMatrix((MATRIX *)mm);
    CompMatrix((MATRIX *)mm, (MATRIX *)p->unk_D0, (MATRIX *)&p->unk_50);
    ScaleMatrix((MATRIX *)&p->unk_50, (VECTOR *)&p->unk_30);
    SetTransMatrix((MATRIX *)&p->unk_50);
    SetRotMatrix((MATRIX *)&p->unk_50);
    e = (Unk45CC4E *)s->unk_08;
    p->unk_24 = s->unk_14;
    for (;;)
    {
        if ((e->unk_00 & 0x20) == 0)
        {
            ang = *(s16 *)(arg0 + 0x3A);
            nrows = e->unk_08 + e->unk_0A;
            row = e->unk_08;
            t = (s8)((u8 *)s->unk_08)[2];
            for (i = 0; i < 2; i++)
            {
                b = &mm[i];
                b[0x10] = b[0x14] = b[0x18] = b[0x1C] = 0x7D00;
            }
            do
            {
                for (i = 0; i < 2; i++)
                {
                    e = (Unk45CC4E *)s->unk_08;
                    if (i == 0)
                    {
                        p->unk_08.w = row;
                        p->unk_0C.w = e->unk_09;
                        p->unk_10.w = 1;
                    }
                    else
                    {
                        p->unk_08.w = row;
                        p->unk_0C.w = e->unk_09 + (e->unk_0B >> 1);
                        p->unk_10.w = 1;
                    }
                    do
                    {
                        p->unk_14.w = e->unk_0B >> 1;
                    } while (0);
                    p->unk_74 = p->unk_84 = (func_800644B8(ang) * *(s16 *)(arg0 + 0x38)) >> 16;
                    ang += *(s16 *)(arg0 + 0x3C);
                    if (ang > 0x1000)
                    {
                        ang -= 0x1000;
                    }
                    p->unk_7C = p->unk_8C = (func_800644B8(ang) * *(s16 *)(arg0 + 0x38)) >> 16;
                    ang -= *(s16 *)(arg0 + 0x3C);
                    if (ang < 0)
                    {
                        ang += 0x1000;
                    }
                    if ((e->unk_00 ^ p->unk_24) & 1)
                    {
                        a = -t;
                        p->unk_80 = a;
                        p->unk_70 = a;
                        a = a - p->unk_10.h;
                        p->unk_88 = a;
                        p->unk_78 = a;
                    }
                    else
                    {
                        a = t;
                        p->unk_80 = a;
                        p->unk_70 = a;
                        a = a + p->unk_10.h;
                        p->unk_88 = a;
                        p->unk_78 = a;
                    }
                    if ((e->unk_00 ^ p->unk_24) & 2)
                    {
                        if (i == 0)
                        {
                            a = -(s8)((u8 *)e)[3];
                            p->unk_7A = a;
                            p->unk_72 = a;
                            a = a - p->unk_14.h;
                            p->unk_8A = a;
                            p->unk_82 = a;
                        }
                        else
                        {
                            a = -(s8)((u8 *)e)[3] - p->unk_14.h;
                            p->unk_7A = a;
                            p->unk_72 = a;
                            a = a - p->unk_14.h;
                            p->unk_8A = a;
                            p->unk_82 = a;
                        }
                    }
                    else
                    {
                        if (i == 0)
                        {
                            a = (s8)((u8 *)e)[3];
                            p->unk_7A = a;
                            p->unk_72 = a;
                            a = a + p->unk_14.h;
                            p->unk_8A = a;
                            p->unk_82 = a;
                        }
                        else
                        {
                            a = (s8)((u8 *)e)[3] + p->unk_14.h;
                            p->unk_7A = a;
                            p->unk_72 = a;
                            a = a + p->unk_14.h;
                            p->unk_8A = a;
                            p->unk_82 = a;
                        }
                    }
                    p->unk_C0 =
                        RotAverage4((SVECTOR *)&p->unk_70, (SVECTOR *)&p->unk_78,
                                    (SVECTOR *)&p->unk_80, (SVECTOR *)&p->unk_88,
                                    (long *)&q->unk_08, (long *)&q->unk_10, (long *)&q->unk_18,
                                    (long *)&q->unk_20, (long *)&p->unk_90, (long *)&p->unk_94) -
                        arg3 - 6;
                    b = (s16 *)(i * 2 + (s32)mm);
                    if (b[0x10] != 0x7D00)
                    {
                        q->unk_08 = b[0x10];
                        q->unk_0A = b[0x18];
                        q->unk_18 = b[0x14];
                        q->unk_1A = b[0x1C];
                    }
                    b[0x10] = q->unk_10;
                    b[0x14] = q->unk_20;
                    b[0x18] = q->unk_12;
                    b[0x1C] = q->unk_22;
                    if ((u32)p->unk_C0 < 0x1E0)
                    {
                        if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                             ((u16)(q->unk_10 + 0x20) < 0x181 && (u16)(q->unk_12 + 0x20) < 0x121) |
                             ((u16)(q->unk_18 + 0x20) < 0x181 && (u16)(q->unk_1A + 0x20) < 0x121) |
                             ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121)) != 0)
                        {
                            ((u8 *)&q->unk_00)[3] = 9;
                            s->unk_14 &= 0x7FFF;
                            p->unk_10.w = p->unk_10.w + p->unk_08.w;
                            if (p->unk_10.w & 0x100)
                            {
                                p->unk_10.w = p->unk_10.w - 1;
                            }
                            p->unk_14.w = p->unk_14.w + p->unk_0C.w;
                            if (p->unk_14.w & 0x100)
                            {
                                p->unk_14.w = p->unk_14.w - 1;
                            }
                            p->unk_0C.w = p->unk_0C.w << 8;
                            p->unk_14.w = p->unk_14.w << 8;
                            if (p->unk_24 & 0x100)
                            {
                                q->unk_0E = s->unk_12;
                            }
                            else
                            {
                                q->unk_0E = s->unk_12 + e->unk_06;
                            }
                            q->unk_0C.h = p->unk_0C.h + p->unk_08.h;
                            q->unk_14.h = p->unk_0C.h + p->unk_10.h;
                            q->unk_16 = (s->unk_10 != 0) ? s->unk_10 + (e->unk_04 & 0xFF9F) : e->unk_04;
                            q->unk_1C.h = p->unk_14.h + p->unk_08.h;
                            q->unk_24.h = p->unk_14.h + p->unk_10.h;
                            if ((s16)q->unk_08 > (s16)q->unk_20)
                            {
                                q->unk_14.b[0] = q->unk_14.b[0] - 1;
                                q->unk_24.b[0] = q->unk_24.b[0] - 1;
                            }
                            if ((s16)q->unk_0A > (s16)q->unk_22)
                            {
                                q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                                q->unk_24.b[1] = q->unk_24.b[1] - 1;
                            }
                            f = e->unk_01;
                            s->unk_0C[3] = f;
                            if (p->unk_24 & 8)
                            {
                                if (p->unk_24 & 4)
                                {
                                    s->unk_0C[3] = f | 2;
                                }
                                else
                                {
                                    s->unk_0C[3] = f & 0xFD;
                                }
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            addPrim(p->unk_20 + p->unk_C0 * 4, q);
                            q++;
                        }
                    }
                }
                ang += *(s16 *)(arg0 + 0x3C);
                if (ang > 0x1000)
                {
                    ang -= 0x1000;
                }
                row++;
                t++;
            } while (row != nrows);
        }
        else
        {
            void (*h)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *);

            h = *(void (**)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *))&e->unk_08;
            if (h != NULL)
            {
                h(arg0, v, s, e);
            }
        }
        if ((s8)e->unk_00 < 0)
        {
            break;
        }
        e++;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

/* The engine's node walk, func_80046398 of csrc/unk_19434.c, relinked here
   with func_s38_8002404C as the visitor: it is variant A -- the slot's
   +0x06 as the visitor's fourth argument -- exactly as
   csrc/ovl_dungeon_800B6AC4.c's func_800BC054 has it, and xsibs scores it
   1.000 against slus_006.14's func_80046398. Nothing was changed.
   21 words, 0 differing, first compile. */
s32 func_s38_80024898(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_s38_8002404C(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* One semi-transparent 1x1 TILE_1 and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad, with each node's colour word
   scaled by the payload's +0x32 life over 128.  148 words, 0 differing, first
   compile.  The twin is func_800D68F4 (csrc/ovl_dungeon_800D50FC.c), which is
   this walk with a constant colour; that function's long note on the shape --
   `addPrim` as a bitfield store, the two loop-hoisted masks in $s2/$s4,
   `setTile1` plus `setSemiTrans` folding to one `sb 0x6A` -- applies here
   unchanged.  The only relink is the scratchpad struct: this object's fields
   sit at +0x00/+0x02/+0x04, +0x18, +0x20, +0x90/+0x94 and +0xC0 where the
   resident original has them at +0x04/+0x06/+0x08, +0x1C, +0x24, +0xD0/+0xD4
   and +0x100, which is what UnkDng73E94SP already describes.  The twin's three
   dead colour `lbu`s are live here: they are `setRGB0`'s three operands, and
   the three `mult`/`bgez`/`addiu 0x7F`/`sra 7` runs are the signed `/ 128`. */
s32 func_s38_800248EC(u8 *p, UnkVec *v)
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
        sp->unk_C0 = RotTransPers((SVECTOR *)&sp->unk_00, (s32 *)&t->x0, &sp->unk_90, &sp->unk_94);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setRGB0(t, t->r0 * *(s16 *)(p + 0x32) / 128, t->g0 * *(s16 *)(p + 0x32) / 128,
                    t->b0 * *(s16 *)(p + 0x32) / 128);
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

/* A random sign: rand() & 1, with the zero case turned into -1. 12 words,
   0 differing. ONE LOCAL CARRIES BOTH VALUES, not two `return's: the target
   reuses the `andi' result on the taken path and only materialises
   `addiu $v0, $zero, -0x1' on the fall-through. As
   `if ((rand() & 1) == 0) return -1; return 1;' it is 12 words and 4
   differing -- the taken path gets its own `li 1'. 0x80069EF8 is `rand' in
   splat/common_syms.txt, so the call needs no declaration.
   raw 1 == relocdump's 1, the R_MIPS_26 on rand. */
s32 func_s38_80024B3C(void)
{
    s32 r;

    r = rand() & 1;
    if (r == 0)
    {
        r = -1;
    }
    return r;
}

/* Spend 8 of the payload's +0x32 life counter, raise the engine's D_800259AC
   tick, and when the counter runs out set the 0x8000 kill bit in the node's
   -0x2 flags and in D_800814A0.  20 words, 0 differing.  The shape is
   func_800999D4 (csrc/ovl_dungeon.c) plus the D_800259AC store.

   THE DECREMENT MUST BE READ INTO AN `s16` LOCAL *BEFORE* THE D_800259AC
   STORE.  Every folded spelling -- `if ((*(s16 *)(p + 0x32) -= 8) <= 0)`
   with the store first, the early-return form, the counter behind a
   `s16 *` local, a `do { } while (0)` around the store, and the
   decrement/test split into two statements -- is 20 words and 11 differing,
   and the same local written AFTER the store is 11 too.  The eleven rows are
   one allocation: with the load after the store, the constant 1 is the only
   block-local quantity alive over $v0, so local-alloc hands it $v0 and
   global-alloc then gives `p` its incoming $a0; reading the counter first
   keeps $v0/$v1 busy across the constant's range, so the constant takes $a0
   and `p` is copied to $a1 by the target's entry `addu`.  That also buys the
   schedule: `addiu $a0, $zero, 1` fills the `lhu`'s load-delay slot, where
   the store-first forms spend a `nop` there.  Hoisting the result into an
   `s32` instead loses the `sll 16`/`bgtz` pair: 19 words, 19 differing.
   THIS COPY'S OWN CONSTANT: -0x8 -> -0xc.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
void func_s38_80024B6C(u8 *p)
{
    s16 t;

    t = *(s16 *)(p + 0x32) - 12;
    D_800259AC = 1;
    *(s16 *)(p + 0x32) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The same 0x211 spawner as csrc/ovl_dng_s58.c's func_s58_800243C4 (dngtwin
   0.973), relinked: seed the new node's mover from the parent `a`'s, jitter
   each axis by a fresh rand() & 0x1F and the caller's own offset less 0x10,
   park func_s38_80024B6C as the step and func_s38_800248EC as the draw, with
   `b` at the payload's +0x14, `d` at +0x32 and `c` at +0x8.  74 words,
   0 differing, first compile.

   The two words over the twin are THIS copy's extra `*(s32 *)(p + 0x50) =
   -0x2000;`, and its POSITION IS LOAD-BEARING: after the func_8004491C call
   it is 0, before the call 74 words / 3 aligned rows (the -0x2000 `addiu`
   moves out of the shared block and the `sw $s7, 0x8($s0)` loses its slot).

   BOTH OF THE TWIN'S FACTS HOLD HERE, re-measured from this body at 0:
   * `s32 dx/dy/dz` instead of `s16` is 70 words / 42 aligned rows -- the four
     missing words are two of the three save/restore pairs the promoted `s16`
     pseudos need.  (The twin priced it 66/71 on its own body; the mechanism
     reproduces, the figure does not.)
   * dropping the `(s16)` cast on the subtraction is 76 words / 20 aligned
     rows for all three sites, which is the twin's recorded 20 TO THE DIGIT,
     and 75 words / 7 rows for the dx site alone.  fold's associate step
     splits `(X + jitter) + (dx + -0x10)` into `(X + jitter + dx) + -0x10`
     unless the cast's NOP_EXPR blocks it. */
void func_s38_80024BBC(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s38_80024B6C;
        *(s16 *)((u8 *)n->unk_08 + 0x2) =
            *(u16 *)((u8 *)a->unk_08 + 0x2) + (rand() & 0x1F) + (s16)(dx - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0x6) =
            *(u16 *)((u8 *)a->unk_08 + 0x6) + (rand() & 0x1F) + (s16)(dy - 0x10);
        *(s16 *)((u8 *)n->unk_08 + 0xA) =
            *(u16 *)((u8 *)a->unk_08 + 0xA) + (rand() & 0x1F) + (s16)(dz - 0x10);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x14) = b;
        *(s16 *)(p + 0x32) = d;
        func_8004491C(n, (u32)func_s38_800248EC);
        *(s32 *)(p + 0x50) = -0x2000;
        *(u32 *)(p + 0x8) = c;
    }
}

/* The bank's fade-out/retire step for the 0x211 node the spawner above makes.
   On the first frame (+0x0 still 0) it bumps that counter, copies the mover's
   whole UnkVec into the payload's +0x44 and -- only while the engine's camera
   record still looks at the party record D_80083780 AND this node owns
   D_800814A8 -- repoints the record's look-at slot (D_80083178 + 0xB8, +0x20,
   i.e. Unk80083178.unk_D8) at that copy and marks the payload +0xA with 9.
   Every frame: reset +0x3C to 0x40, run +0x2 down, advance the +0x3A phase by
   0x96, ramp the +0x38 amplitude up by 4 over 0x50 and down by 5 under 0x32
   with a zero floor, fade `c`'s three colour bytes over the last 0x14 frames,
   push `c`'s +0x1A/+0x1C/+0x1E, spin the phase 0x20 more steps through the
   0x1000 wrap to get this frame's angle, drive the mover's +0x8 height from
   func_800644B8 of it, clamp +0xA to func_800BCB04's ground height, and at
   +0x2 <= 0 release the camera, raise +0x9C on the slot at +0x30 and set the
   0x8000 kill bit.  176 words, 0 differing, FIRST COMPILE.

   NO TWIN AT ANY RATIO.  Written off the listing.  csrc/ovl_dungeon_800B6AC4's
   func_800B9A78 -- CALLEE.txt's only 4-of-7 row -- supplied the
   `(u8 *p, UnkVec *v, u8 *c)` signature (the third parameter is read in $a2
   with no entry copy, so it is a parameter) and the house idiom
   `*(s16 *)(c + K) = *(u16 *)(c + K) + N`.  The +0xB8 record came from
   `grep -rn D_80083178`, which names csrc/ovl_dng_s44.c and
   csrc/ovl_dungeon_800C2824.c; Unk80083178.unk_D8 at 0xB8 + 0x20 is the
   look-at pointer func_8004D09C (csrc/unk_1C540.c) reads as `q[1]/q[3]/q[5]`,
   which is why a bare UnkVec can be parked there.

   SIX FACTS, all priced from this body AFTER it reached 0:
   * ONE `m` LOCAL, ASSIGNED TWICE, shared between the first-frame block and
     the tail.  Two locals is 177 words / 28 aligned rows and inlining the
     first site is 177 / 28 as well -- the same 28, because both spellings cost
     the single global allocno its $s0.
   * THE THREE COLOUR BYTES MUST STORE DESCENDING (+0xE, +0xD, +0xC).  The
     chain `c[0xC] = c[0xD] = c[0xE] = ...` and an explicit local with three
     descending stores are byte-identical (0 either way, so the chain is NOT
     MEASURABLE against them, not free); the same local with the stores
     ASCENDING is 4 aligned rows.
   * THE MOVER COPY IS A WHOLE-STRUCT ASSIGNMENT.  `*(UnkVec *)(p + 0x44) =
     *(UnkVec *)m` gives the target's 4+4 / 2+2 block move; six explicit word
     stores are 182 words / 22 rows.
   * TWO POINTER LOCALS FOR THE RECORD, `g = (u8 *)&D_80083178` and
     `q = g + 0xB8`, with `g` dying at the copy.  One local spelled
     `q = (u8 *)&D_80083178 + 0xB8` is 175 words / 1 row: fold reassociates the
     0xB8 onto the `%lo` and the separate `addiu $s3, $v0, 0xB8` disappears.
     (Same fact, same shape, as func_s44_8002416C's; its own price was 25.)
   * `s16 r` FOR func_800BCB04's RESULT.  `s32 r` is 174 words / 6 rows -- the
     target keeps the raw result for the `sh` and a sign-extended copy for the
     `slt`, which is the narrow-signed-call-result lever.
   * `s16 x`/`s16 step` FOR THE WRAP LOOP, and the +0x1C gate read UNSIGNED.
     `s32 x`/`s32 step` is 172 words / 15 rows (the `sll 16`/`sra 16` pairs
     round each clamp go); `*(s16 *)(c + 0x1C) < 0x1000` for the `sltiu` is
     177 / 7.
   * THE TWO +0x8 STORES ARE TWO STATEMENTS.  Folded into one expression it is
     174 words / 4 rows; the target stores the product, then reloads it out of
     its own register and adds +0x40.

   MEASURED AND FREE, so NOT the reason this lands: `(x << 7) / 20` for
   `x * 0x80 / 20` (0 either way, the int destination makes them one tree);
   `>= 0x51` for `> 0x50`; the two-test gate NESTED instead of `&&` (the
   operands are different symbols, so fold_truthop never had a merge to make);
   `(*(s16 *)p)++` for `*(s16 *)p = *(u16 *)p + 1`; reading x and step through
   `*(s16 *)` instead of `*(u16 *)` (the s16 local truncates either way); and
   the `*(s16 *)(p + 0x3C) = 0x40` store placed after the +0x2 decrement
   instead of before it. */
void func_s38_80024CE4(u8 *p, UnkVec *v, u8 *c)
{
    u8 *g;
    u8 *q;
    u8 *m;
    s16 x;
    s16 step;
    s16 i;
    s16 r;

    g = (u8 *)&D_80083178;
    q = g + 0xB8;
    D_800259AC = 1;
    if (*(s16 *)p == 0)
    {
        *(s16 *)p = *(u16 *)p + 1;
        m = *(u8 **)(*(u8 **)(p + 0x2C) - 0x18);
        *(UnkVec *)(p + 0x44) = *(UnkVec *)m;
        if (*(u32 *)(q + 0x20) == (u32)D_80083780 && *(u32 *)(p + 0x2C) == D_800814A8)
        {
            *(u32 *)(q + 0x20) = (u32)(p + 0x44);
            *(s16 *)(p + 0xA) = 9;
        }
    }
    *(s16 *)(p + 0x3C) = 0x40;
    *(s16 *)(p + 0x2) = *(u16 *)(p + 0x2) - 1;
    *(s16 *)(p + 0x3A) = *(u16 *)(p + 0x3A) + 0x96;
    if (*(s16 *)(p + 0x2) > 0x50)
    {
        *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) + 4;
    }
    else if (*(s16 *)(p + 0x2) < 0x32)
    {
        *(s16 *)(p + 0x38) = *(u16 *)(p + 0x38) - 5;
        if (*(s16 *)(p + 0x38) < 0)
        {
            *(s16 *)(p + 0x38) = 0;
        }
    }
    if (*(s16 *)(p + 0x2) < 0x14)
    {
        c[0xC] = c[0xD] = c[0xE] = *(s16 *)(p + 0x2) * 0x80 / 20;
    }
    *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x10;
    if (*(u16 *)(c + 0x1C) < 0x1000)
    {
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0x100;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x100;
    }
    x = *(u16 *)(p + 0x3A);
    step = *(u16 *)(p + 0x3C);
    for (i = 0; i < 0x20; i++)
    {
        x += step;
        if (x > 0x1000)
        {
            x -= 0x1000;
        }
    }
    m = *(u8 **)(*(u8 **)(p + 0x2C) - 0x18);
    *(s32 *)(m + 0x8) = func_800644B8(x) * *(s16 *)(p + 0x38) * 2;
    *(s32 *)(m + 0x8) += *(s32 *)(p + 0x40);
    r = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6), *(s16 *)((u8 *)v + 0xA));
    if (r < *(s16 *)(m + 0xA))
    {
        *(s16 *)(m + 0xA) = r;
    }
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(s16 *)(*(u8 **)(p + 0x30) + 0x9C) = 1;
        if (*(s16 *)(p + 0xA) == 9)
        {
            *(u32 *)(q + 0x20) = (u32)D_80083780;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The bank's master step handler for the 0x213 caster script, and IT OWNS A
   JUMP TABLE.  jtbl_s38_80024028: 9 `.word's, dispatch bound
   `sltiu $v0, $v1, 0x9' with no minval subtraction, 9 DISTINCT targets, no
   pad.  The entry list in table order:

     case 0  .Ls38_80025090      case 5  .Ls38_80025874
     case 1  .Ls38_80025150      case 6  .Ls38_800258B0
     case 2  .Ls38_80025418      case 7  .Ls38_80025948  (NO ARM)
     case 3  .Ls38_80025798      case 8  .Ls38_800258E4
     case 4  .Ls38_800257CC

   THE DEFAULT IS THE FUNCTION'S EPILOGUE AND CASE 7 IS WRITTEN AS NOTHING AT
   ALL.  `.Ls38_80025948' is both the range-check `beqz $v0' target and the
   first word of the epilogue, so there is no `default:' statement and no
   `case 7:' label: case 7 simply falls out of the switch.  Eight real arms,
   written 0, 1, 2, 3, 4, 5, 6, 8 in ascending order, which is the entry
   order; case 0 FALLS THROUGH into case 1 (no `j' at .Ls38_8002514C), and
   cases 4 and 5 share a cross-jumped two-store tail at .Ls38_8002589C that
   jump2 merged -- both arms write it out.

   WHAT IT IS.  `p' is this node's payload, `o = *(u8 **)p' the caster's
   payload, `n' its node (`o - 0x20') and `m' its mover.  0 paints the
   renderable white, parks the 0xC-byte D_800259A0 template at p+0xA2 as the
   primitive, picks the facing out of the caster's +0x2A and arms the
   D_80025988 ring through func_800B835C, then falls into 1; 1 reads the
   caster's hand through func_8003DF74, mirrors its position into the mover
   (less 0x40 when the slot is flagged), tags the node with D_80045340, picks
   the target tile's height and the step count off the caster's +0x60 target
   and seeds the mover's three velocities out of the 0x20-byte
   D_s38_80024004 table; 2 sprays sixteen func_s38_80024BBC sparks a frame,
   spins the ring, ticks the +0x92/+0x94 blink and on arrival either spawns
   the 0x212 impact node (func_s38_80024898 step, func_s38_80024CE4 handler)
   or gives up to state 8; 3 waits four frames; 4 runs 0x64 frames to the
   func_8009D218/func_800C8A3C hit and 0x78 to the next state; 5 waits for
   +0x9C; 6 waits 0xB; 8 waits 0x1F and then either consumes the bank's
   D_800259AC tick or kills the node.
   627 words, 0 differing, 0 aligned differences.

   DONOR: csrc/ovl_dng_s21.c's LANDED func_s21_80024050, which dngtwin cannot
   see at any floor (it is 471 words against this 627 with eighteen table
   entries against nine) and which CALLEE.txt's 10-of-22 tier names together
   with its s19 and s20 relinks.  Same `(u8 *p, UnkVec *v, u8 *c)' signature,
   same `o'/`n'/`m' head, same case-0 renderable-and-ring opening, same
   func_8003DF74 hand read with the same `u16 out[3]'/`s32 arr[2]' stack
   locals, same D_80045340 tag block and the same
   `D_8006CCD8[facing]'/`D_8006CCE8[facing]' corner pair.  The 0x20-byte
   table copy came from this unit's own ALREADY-PRESENT BUT UNUSED
   `UnkDngS58Blk32'/`UnkDngS58Rec'/`UnkDngS58Name12' typedefs: the lwl/lwr
   block moves are 1-aligned struct assignments and `tbl[k].a'/`tbl[k].b' is
   exactly `UnkDngS58Rec'.  Three of the lead files were useless here --
   LEADS.txt reports no sibling at 0.60, NEAR.txt no landed in-unit
   neighbour -- and the callee-set row was the whole answer.

   FIRST COMPILE WAS 633 WORDS / 81 ALIGNED ROWS; it took four.  TEN FACTS,
   all priced from this body AFTER it reached 0 (aligndiff):

   | spelling                                              | words | rows |
   |-------------------------------------------------------|-------|------|
   | the D_s38_80024004 copy BEFORE `n'/`m'         KEPT   |  627  |   0  |
   |   ... after them                                      |  628  |   6  |
   | case 0's +0xA bump as an ARRAY REF,                   |       |      |
   |   `((s16 *)p)[0x5] = ((u16 *)p)[0x5] + 1'      KEPT   |  627  |   0  |
   |   ... `*(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1'    |  628  |   7  |
   | m+0xA through `z', assigned AFTER the +0x6 store KEPT |  627  |   0  |
   |   ... no `z', `*(u16 *)(m + 0xA)' inline both times  |  629  |   7  |
   |   ... `z' assigned before the +0x2 store             |  624  |   8  |
   | the 0x8000 gate with the out[] arm as the `then' KEPT |  627  |   0  |
   |   ... arms the other way round                       |  626  |   5  |
   | o+0x88 through `k' in the t == NULL arm         KEPT  |  627  |   0  |
   |   ... the two statements with no local               |  628  |   8  |
   | case 1's +0xA store BEFORE the +0x88 store      KEPT  |  627  |   0  |
   |   ... +0x88 first                                    |  627  |  11  |
   | the 0x7B gate as `<= 0' with the integrate block      |       |      |
   |   as the ELSE                                   KEPT  |  627  |   0  |
   |   ... `> 0' with the integrate block as the then     |  627  |  27  |
   | case 4's argument as TWO locals, `j' for the call     |       |      |
   |   result + 2 and `h' for the sum                KEPT  |  627  |   0  |
   |   ... one local for the whole expression             |  627  |   3  |
   |   ... `h' spelled `k', i.e. SHARED with the o+0x88    |       |      |
   |       local 300 words away                           |  628  |  13  |
   | the dead `*(s32 *)(w + 0x8) = *(s32 *)(u + 0x8)       |       |      |
   |   - 0x140000' store KEPT (it is overwritten two       |       |      |
   |   statements later and the ROM still has it)          |  627  |   0  |
   |   ... removed                                        |  623  |  29  |

   THE TWO ARRAY-REF ROWS ARE `true_dependence' (sched.c:817), the same rule
   func_s21_80024050 prices at 11 for the same sentence: `((u16 *)p)[0x5]' is
   an ARRAY_REF so MEM_IN_STRUCT_P is set and its address varies, while the
   `sh %lo(D_800259AC)' beside it is a plain cast at a FIXED address, so the
   exemption fires and the `lhu 0xA($s1)' hoists ABOVE the D_800259AC store
   into its load-delay slot.  As a plain cast neither memref is in-struct, the
   hoist is illegal, and the `nop' plus a whole-block $v0/$v1/$a0 rotation
   come back.  That is also the inverse of func_s38_80024B6C's note above,
   which needs a LOCAL rather than an array ref for the same hoist -- so the
   two spellings are alternatives for one mechanism and the choice is per
   body.

   THE TWO LOCALS `j'/`h' ARE ONE FACT WITH TWO HALVES.  `j' exists to stop
   fold's associate step moving the `+ 2' off the call result and onto the
   running sum (the target has `addiu $v0, $v0, 0x2' on the `andi' result,
   before the `addu'); `h' exists because the third argument must be a VALUE
   by the time the D_800E3D68 ternary is expanded, which is what keeps the
   `addu $a2' ahead of the ternary instead of in the `jal's delay slot.  The
   13-row row is the brief's `reg_n_sets' trap arriving sideways: sharing one
   local between case 1's +0x88 block and case 4's argument puts two sets on
   it, makes it a global allocno, and rotates $v0/$v1/$a2 in BOTH places.

   RETRACTED, AND I NEARLY SHIPPED IT: while `h' was still the SHARED local a
   `sll 16'/`sra 16' pair appeared at the func_800C8A3C call and I read it as
   the `s16' third parameter narrowing an `s32' argument.  It was the shared
   local's own 13 rows.  With `h' separate, the landed
   `s32 func_800C8A3C(u8 *, s16, s16)' prototype measures 627 / 0 and
   widening the third parameter to `s32' also measures 627 / 0, so the width
   is FREE here and NO prototype anywhere is changed by this body.

   NOT MEASURABLE, not free: the clamp written `-0x140000 < *(s32 *)(w + 0x8)'
   instead of `*(s32 *)(w + 0x8) > -0x140000' -- 627 / 0 both ways because
   gcc canonicalises `>' into `<' with the operands swapped, so the two source
   forms are the SAME tree and the tools have nothing to see.
   Also NOT MEASURABLE: which arm order the table's nine entries carry, since
   both tools mask a `j's target and neither reads `.rodata'; the arms are
   written in the entry order read off the listing.

   `__builtin_abs(...) * 2 - 1' is s21's `__builtin_abs(...) + 1' with this
   copy's constants; the `sll 1'/`addiu -1' pair is what the listing shows. */
void func_s38_80024FA4(u8 *p, UnkVec *v, u8 *c)
{
    u16 out[3];
    s32 arr[2];
    UnkDngS58Rec tbl[8];
    UnkNode *n;
    UnkNode *n2;
    u8 *o;
    u8 *m;
    u8 *e;
    u8 *t;
    u8 *u;
    u8 *q;
    u8 *r;
    u8 *w;
    u8 *g;
    s32 i;
    s32 k;
    s32 z;
    s32 j;
    s32 h;

    o = *(u8 **)p;
    *(UnkDngS58Blk32 *)tbl = *(UnkDngS58Blk32 *)D_s38_80024004;
    n = (UnkNode *)(o - 0x20);
    m = (u8 *)n->unk_08;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0xA2) = *(UnkDngS58Name12 *)D_800259A0;
        *(u32 *)(c + 0x8) = (u32)(p + 0xA2);
        *(s16 *)(p + 0x7E) = (((u16 *)o)[0x15] >> 9) & 0x7;
        D_80025988[0x10] = 0xE0;
        D_80025988[0x11] = 0xE0;
        D_80025988[0x12] = 0xE0;
        D_80025988[0x14] = 0;
        D_80025988[0x15] = 0;
        D_80025988[0x16] = 0;
        arr[0] = 0x1400340;
        arr[1] = 0x400040;
        func_800B835C(D_80025988, arr, 1, 0);
        D_800259AC = 1;
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
            *(s16 *)(c + 0x10) = 0x20;
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            *(s16 *)(c + 0x1E) = 0x7D0;
            *(s16 *)(c + 0x1C) = 0x7D0;
            *(u16 *)(c + 0x14) |= 0xC;
            p[0x7A] |= 0x4;
        }
        t = *(u8 **)(o + 0x60);
        if (t != NULL)
        {
            u = *(u8 **)(t - 0x18);
            *(s16 *)(p + 0x78) = *(u16 *)(u + 0xA) - (D_800DDC40[t[0x13]] >> 1);
            e = *(u8 **)(o - 0x14);
            p[0xBA] = e[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
            p[0xBB] = e[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
            p[0x7B] = __builtin_abs(*(s8 *)(o + 0x72) == e[0x24] ? *(s8 *)(o + 0x73) - e[0x25]
                                                                 : *(s8 *)(o + 0x72) - e[0x24]) *
                          2 -
                      1;
        }
        else
        {
            k = *(u16 *)(o + 0x88) - 0x50;
            p[0x7B] = 0x20;
            *(s16 *)(p + 0x78) = k;
        }
        v->unk_0C = tbl[*(s16 *)(p + 0x7E)].a << 16;
        v->unk_10 = tbl[*(s16 *)(p + 0x7E)].b << 16;
        v->unk_14 = ((*(s16 *)(p + 0x78) << 16) - v->unk_08) / *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x8A) = *(s8 *)(p + 0x7B) * *(s8 *)(p + 0x7B);
        *(s16 *)(p + 0x82) = 0;
        *(s16 *)(p + 0x90) = 0;
        *(s16 *)(p + 0x92) = 0;
        *(s16 *)(p + 0x94) = 5;
        *(s16 *)(p + 0x96) = 0xC8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
        break;
    case 2:
        for (i = 0; i < 0x10; i++)
        {
            func_s38_80024BBC((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xFF4040,
                              (rand() & 0x3F) | 0x40, 0, 0, 0);
        }
        if ((s16)func_800A4778(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                               *(s16 *)((u8 *)v + 0xA), *(s32 *)(o + 0x60)) != 0)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0;
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            break;
        }
        *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) + 0x190;
        if (*(u16 *)(c + 0x1A) > 0x1000)
        {
            *(s16 *)(c + 0x1A) = *(u16 *)(c + 0x1A) - 0x1000;
        }
        if (*(s16 *)(p + 0x90) == 0)
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + *(u16 *)(p + 0x96);
        }
        else
        {
            *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - *(u16 *)(p + 0x96);
            *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - *(u16 *)(p + 0x96);
        }
        *(s16 *)(p + 0x92) = *(u16 *)(p + 0x92) + 1;
        if (*(s16 *)(p + 0x92) >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        p[0x7B] = p[0x7B] - 1;
        if (*(s8 *)(p + 0x7B) <= 0)
        {
            if (*(u8 **)(o + 0x60) != NULL)
            {
                *(s16 *)(p + 0xA) = 3;
                *(s16 *)(p + 0x82) = 0;
                *(u32 *)(*(u8 **)(o + 0x60) + 0x14) |= 0x100000;
                func_800A56E0(0x300);
                u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)(u + 0x2);
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)(u + 0x6);
                g = D_800DDC40;
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)(u + 0xA) - g[(*(u8 **)(o + 0x60))[0x13]];
                n2 = func_8003FC64(0x212);
                if (n2 != NULL)
                {
                    q = (u8 *)n2 + 0x20;
                    *(s16 *)(q + 0x2) = 0x78;
                    *(u32 *)(q + 0x28) = (u32)o;
                    *(u32 *)(q + 0x2C) = *(u32 *)(o + 0x60);
                    *(u32 *)(q + 0x30) = (u32)p;
                    n2->unk_10 = (u32)func_s38_80024CE4;
                    func_8004491C(n2, (u32)func_s38_80024898);
                    r = (u8 *)n2->unk_0C;
                    *(s16 *)(r + 0x10) = 0x20;
                    *(s16 *)(r + 0x6) = 0;
                    *(u16 *)(r + 0x14) |= 0xC;
                    u = *(u8 **)(*(u8 **)(o + 0x60) - 0x18);
                    w = (u8 *)n2->unk_08;
                    *(s32 *)(w + 0x0) = *(s32 *)(u + 0x0);
                    *(s32 *)(w + 0x4) = *(s32 *)(u + 0x4);
                    *(s32 *)(w + 0x8) = *(s32 *)(u + 0x8) - 0x140000;
                    *(s32 *)(w + 0x8) = -((g[(*(u8 **)(o + 0x60))[0x13]] >> 1) << 16);
                    if (*(s32 *)(w + 0x8) > -0x140000)
                    {
                        *(s32 *)(w + 0x8) = -0x140000;
                    }
                    *(s32 *)(w + 0x8) += *(s32 *)(u + 0x8);
                    *(s32 *)(q + 0x40) = *(s32 *)(u + 0x8);
                    r = (u8 *)n2->unk_0C;
                    r[0xE] = 0x80;
                    r[0xD] = 0x80;
                    r[0xC] = 0x80;
                    *(s16 *)(r + 0x1E) = 0;
                    *(s16 *)(r + 0x1C) = 0;
                    *(UnkDngS58Name12 *)((u8 *)n2 + 0x3A) = *(UnkDngS58Name12 *)D_80025970;
                    *(u32 *)(r + 0x8) = (u32)((u8 *)n2 + 0x3A);
                }
            }
            else
            {
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
            }
            c[0xE] = 0;
            c[0xD] = 0;
            c[0xC] = 0;
            *(s16 *)(c + 0x1E) = 0;
            *(s16 *)(c + 0x1C) = 0;
        }
        else
        {
            v->unk_00 += v->unk_0C;
            v->unk_04 += v->unk_10;
            v->unk_08 += v->unk_14;
        }
        break;
    case 3:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) == 4)
        {
            *(s16 *)(p + 0xA) = 4;
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0x9C) = 0;
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) == 0x64)
        {
            if (func_8009D218(*(u8 **)(o + 0x60), 2, o) == 0)
            {
                j = (func_800A6D30() & 3) + 2;
                h = (p[0x9] >> 2) + j;
                func_800C8A3C(*(u8 **)(o + 0x60), D_800E3D68 == 0xFF ? 0xFF : 0x10, h);
            }
            *(u32 *)(*(u8 **)(o + 0x60) + 0x14) &= 0xFFEFFFFF;
        }
        if (*(s16 *)(p + 0x82) >= 0x78)
        {
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if (*(s16 *)(p + 0x9C) != 0)
        {
            *(u32 *)(*(u8 **)(o + 0x60) + 0x14) &= 0xFFEFFFFF;
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0xB)
        {
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        if (*(s16 *)(p + 0x82) >= 0x1F)
        {
            ((s16 *)p)[0x41] = ((u16 *)p)[0x41] - 1;
            if (D_800259AC == 0)
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
            else
            {
                D_800259AC = 0;
            }
        }
        break;
    }
}
