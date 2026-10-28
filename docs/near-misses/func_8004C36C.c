/* func_8004C36C (unk_1C540) -- 464 of 461 words, 171 DIFF. COMPILES.
 *
 * FIRST ATTEMPT, 2026-10-27, written the same day as func_80033D54's and
 * func_800345B8's parks; it is the third of the three giants that had never
 * been read. The GTE model draw: for each node on the `UnkNode.unk_18` chain
 * it walks a linked list of parts (`list->unk_0C`), builds each part's matrix
 * with RotMatrix/TransMatrix/CompMatrix/ScaleMatrix, and then walks that
 * part's 0xC-byte element records projecting four corners through the GTE's
 * RTPT/RTPS and emitting one primitive each -- either inline (the 0x2C code
 * path) or through func_8004C080, which returns a count in a stack slot.
 *
 * THREE THINGS CARRY THE SHAPE, and the first was worth 265 differing words:
 *
 *  - (*) THE STACK OBJECT IS REACHED THROUGH A POINTER LOCAL. `Unk4C36C pp;`
 *    with `Unk4C36C *p = &pp;` and every access as `p->unk_NN` gives the
 *    listing's `addiu $s1, $sp, 0x18` and one base register for all 0x110
 *    bytes of it. Accessed directly as `pp.unk_NN`, gcc addresses every field
 *    off `$sp` instead: the same word count, but 436 differing words and
 *    three more `nop`s, because nothing shares a base. This is section 5's
 *    "struct POINTER local gets both a materialised base and
 *    MEM_IN_STRUCT_P", and it is what the survey meant by "the stack objects'
 *    bases are the thing to get right first".
 *  - (*) EVERY NODE FIELD IS RE-DEREFERENCED, not cached. The listing reloads
 *    `list->unk_08` (and `list->unk_04`) before each of the three or four
 *    words it copies out of them -- `lw $v0, 0x8($s3)` appears before every
 *    single one. A `q = *(u8 **)(list + 8);` local instead is TWENTY words
 *    short. matching-levers section 4.
 *  - (*) The third element word goes through a LOCAL (`w = *(s32 *)(e + 8);`
 *    then the store and the test), because the listing loads it ONCE and puts
 *    the store in the `beqz`'s delay slot. Read twice -- once for the store,
 *    once for the test -- the store to the stack object invalidates cse's
 *    entry and it reloads: four words.
 *
 * THE GTE OPERATIONS NEEDED NEW MACROS. `include/gte_inline.h` now has
 * `gte_ldv3`, `gte_rtpt`, `gte_rtps`, `gte_stsxy3` and `gte_stsxy`, with the
 * RTPT/RTPS encodings read out of this function's own listing (0x4A280030 and
 * 0x4A180001). Do NOT reach for `psyq/include/inline_c.h` instead: its
 * data-movement macros are right but its operations are `.word 0x000000bf`
 * and `.word 0x0000007f`, which assemble SILENTLY to the wrong bytes through
 * modern gas. The measurement is in that header's comment.
 *
 * WHAT IS LEFT: 3 words and the register/schedule positions that follow them.
 * All three are `nop`s in the primitive-field block, where the listing fills
 * each store's load-delay slot with the NEXT statement's load and this build
 * cannot: those loads are of the stack object and the stores are through
 * `prim`, and `true_dependence` makes them conflict because BOTH are QImode
 * (`lbu`/`sb`), which is the one case its MEM_IN_STRUCT_P exception cannot
 * reach -- see the note added to matching-levers section 5 for func_80033D54.
 * Measured and not working: the stack-object reads in that block spelled as
 * casts rather than struct fields (no change at all).
 *
 * STILL UNVERIFIED, and the places to look first when picking this up:
 *   * the two `|| x != y` guards around the u/v decrements. The listing
 *     branches to the decrement when `prim->x1 < prim->x0` and past it when
 *     `prim->x0 == prim->x2`, which is what `a < b || b != c` gives, but
 *     nothing has tested the other groupings.
 *   * `p->unk_02`/`unk_03` are read through `*(volatile u8 *)` to keep the
 *     listing's `lbu` + `sll 24` + `sra 24`; without it combine folds the
 *     sign-extension into an `lb` and four words differ. The volatile is a
 *     stand-in for whatever the original actually wrote.
 *   * the field names and widths of Unk4C36CW: the union's halves are read in
 *     both signednesses at different sites and only the `lhu`/`lh` in the
 *     listing says which is which.
 */
#include "common.h"

u8 *func_8004C010(u8 *, u8 *);
u8 *func_8004C080(u8 *, u8 *, u8 *, u8 *, u8 *, s32);

typedef union Unk4C36CW
{
    s32 w;
    struct
    {
        u16 lo;
        u16 hi;
    } u;
    struct
    {
        s16 lo;
        s16 hi;
    } h;
    u8 b[4];
} Unk4C36CW;

typedef struct Unk4C36C
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
    u16 unk_04;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
    u8 pad_0C[0xC];
    s32 unk_18;
    u8 pad_1C[0x4];
    Unk4C36CW unk_20;
    Unk4C36CW unk_24;
    s32 unk_28;
    u8 pad_2C[0x4];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x4];
    s32 unk_40;
    s32 unk_44;
    u8 pad_48[0x8];
    s32 unk_50;
    s32 unk_54;
    s16 unk_58;
    s16 unk_5A;
    s16 unk_5C;
    u8 pad_5E[0x2];
    s16 unk_60;
    s16 unk_62;
    s16 unk_64;
    u8 pad_66[0x2];
    s16 unk_68;
    s16 unk_6A;
    s16 unk_6C;
    u8 pad_6E[0x2];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s32 unk_78;
    MATRIX unk_7C;
    u8 pad_9C[0xC];
    s32 unk_A8;
    Unk4C36CW unk_AC;
    Unk4C36CW unk_B0;
    Unk4C36CW unk_B4;
    s32 unk_B8;
    Unk4C36CW unk_BC;
    Unk4C36CW unk_C0;
    MATRIX unk_C4;
    MATRIX unk_E4;
    u8 pad_104[0x8];
    s32 unk_10C;
} Unk4C36C;

void func_8004C36C(u8 *pl, u8 *list)
{
    Unk4C36C pp;
    Unk4C36C *p = &pp;
    s32 count;
    MATRIX *m1;
    MATRIX *m2;
    u8 *base;
    POLY_FT4 *prim;
    u8 *e;
    UnkNode *nd;
    u32 c0;
    u32 c1;
    s32 v0;
    s32 v1;
    s32 v2;
    s32 v3;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t;
    s32 w;

    m1 = &p->unk_E4;
    m2 = &p->unk_7C;
    c0 = 0xFFFFFF;
    c1 = 0xFF000000;
    base = D_80083160.unk_00;
    prim = *(POLY_FT4 **)(base + 0x8D0);
    p->unk_18 = (s32)(base + 0x70);
    PushMatrix();
    p->unk_38 = 0;
    p->unk_74 = 0;
    p->unk_6C = 0;
    p->unk_64 = 0;
    p->unk_5C = 0;
    for (;;)
    {
        while (list != NULL)
        {
            p->unk_B8 = *(s32 *)(*(u8 **)(list + 8) + 0);
            p->unk_BC.w = *(s32 *)(*(u8 **)(list + 8) + 4);
            p->unk_C0.w = *(s32 *)(*(u8 **)(list + 8) + 8);
            p->unk_20.w = *(s16 *)(*(u8 **)(list + 4) + 8) + p->unk_BC.h.hi;
            p->unk_24.w = *(s16 *)(*(u8 **)(list + 4) + 0xA) + p->unk_C0.h.lo;
            p->unk_28 = *(s16 *)(*(u8 **)(list + 4) + 0xC) + p->unk_C0.h.hi;
            p->unk_50 = p->unk_B8;
            p->unk_54 = p->unk_BC.w;
            RotMatrix((SVECTOR *)&p->unk_50, &p->unk_C4);
            TransMatrix(&p->unk_C4, (VECTOR *)&p->unk_20);
            p->unk_40 = *(s16 *)(*(u8 **)(list + 4) + 4);
            p->unk_44 = *(s16 *)(*(u8 **)(list + 4) + 6);
            p->unk_78 = *(s32 *)(*(u8 **)(list + 4) + 0);
            p->unk_10C = (*(u8 **)(list + 4))[0xF];
            do
            {
                list = *(u8 **)(list + 0xC);
                if (list == NULL)
                {
                    goto done;
                }
                e = *(u8 **)list;
            } while (e == NULL);
            p->unk_A8 = *(s32 *)(*(u8 **)(list + 4) + 0);
            p->unk_AC.w = *(s32 *)(*(u8 **)(list + 4) + 4);
            p->unk_B0.w = *(s32 *)(*(u8 **)(list + 4) + 8);
            p->unk_B4.w = *(s32 *)(*(u8 **)(list + 4) + 0xC);
            p->unk_50 = *(s32 *)(*(u8 **)(list + 8) + 0);
            p->unk_54 = *(s32 *)(*(u8 **)(list + 8) + 4);
            RotMatrix((SVECTOR *)&p->unk_50, m1);
            p->unk_20.w = *(s16 *)(*(u8 **)(list + 8) + 6);
            p->unk_24.w = *(s16 *)(*(u8 **)(list + 8) + 8);
            p->unk_28 = *(s16 *)(*(u8 **)(list + 8) + 0xA) + p->unk_B4.h.lo;
            TransMatrix(m1, (VECTOR *)&p->unk_20);
            CompMatrix(&p->unk_C4, m1, m2);
            p->unk_30 = p->unk_AC.h.lo;
            p->unk_34 = p->unk_AC.h.hi;
            ScaleMatrix(m2, (VECTOR *)&p->unk_30);
            ScaleMatrix(m2, (VECTOR *)&p->unk_40);
            SetRotMatrix(m2);
            SetTransMatrix(m2);
            for (;;)
            {
                *(s32 *)&p->unk_00 = *(s32 *)(e + 0);
                *(s32 *)&p->unk_04 = *(s32 *)(e + 4);
                w = *(s32 *)(e + 8);
                *(s32 *)&p->unk_08 = w;
                if (w != 0)
                {
                    if (p->unk_00 & 1)
                    {
                        v0 = p->unk_B0.u.lo - (s8) * (volatile u8 *)&p->unk_02 -
                             p->unk_20.u.lo - p->unk_BC.u.hi;
                        p->unk_68 = v0;
                        p->unk_58 = v0;
                        t0 = v0 - p->unk_0A;
                        p->unk_70 = t0;
                        p->unk_60 = t0;
                    }
                    else
                    {
                        v1 = p->unk_B0.u.lo + (s8) * (volatile u8 *)&p->unk_02 -
                             p->unk_20.u.lo - p->unk_BC.u.hi;
                        p->unk_68 = v1;
                        p->unk_58 = v1;
                        t1 = v1 + p->unk_0A;
                        p->unk_70 = t1;
                        p->unk_60 = t1;
                    }
                    if (p->unk_00 & 2)
                    {
                        v2 = p->unk_B0.u.hi - (s8) * (volatile u8 *)&p->unk_03 -
                             p->unk_24.u.lo - p->unk_C0.u.lo;
                        p->unk_62 = v2;
                        p->unk_5A = v2;
                        t2 = v2 - p->unk_0B;
                        p->unk_72 = t2;
                        p->unk_6A = t2;
                    }
                    else
                    {
                        v3 = p->unk_B0.u.hi + (s8) * (volatile u8 *)&p->unk_03 -
                             p->unk_24.u.lo - p->unk_C0.u.lo;
                        p->unk_62 = v3;
                        p->unk_5A = v3;
                        t3 = v3 + p->unk_0B;
                        p->unk_72 = t3;
                        p->unk_6A = t3;
                    }
                    gte_ldv3(&p->unk_58, &p->unk_60, &p->unk_68);
                    gte_rtpt();
                    gte_stsxy3(&prim->x0, &prim->x1, &prim->x2);
                    gte_ldv0(&p->unk_70);
                    gte_rtps();
                    gte_stsxy(&prim->x3);
                }
                if ((p->unk_01 & 0xFC) == 0x2C)
                {
                    if (p->unk_08 + p->unk_0A >= 0x100)
                    {
                        p->unk_0A = p->unk_0A - 1;
                    }
                    if (p->unk_09 + p->unk_0B >= 0x100)
                    {
                        p->unk_0B = p->unk_0B - 1;
                    }
                    setlen(prim, 9);
                    *(s32 *)&prim->r0 = p->unk_A8;
                    func_8004C010((u8 *)&prim->r0, (u8 *)&p->unk_78);
                    prim->code = p->unk_01;
                    prim->u0 = p->unk_08;
                    prim->clut = p->unk_06;
                    prim->v0 = p->unk_09;
                    if (prim->x1 < prim->x0 || prim->x0 != prim->x2)
                    {
                        p->unk_0A = p->unk_0A - 1;
                    }
                    t = p->unk_08 + p->unk_0A;
                    prim->u3 = t;
                    prim->u1 = t;
                    prim->v1 = p->unk_09;
                    prim->tpage = p->unk_04;
                    prim->u2 = p->unk_08;
                    if (prim->y2 < prim->y0 || prim->y0 != prim->y1)
                    {
                        p->unk_0B = p->unk_0B - 1;
                    }
                    t = p->unk_09 + p->unk_0B;
                    prim->v3 = t;
                    prim->v2 = t;
                    *(u32 *)prim =
                        (*(u32 *)prim & c1) |
                        (*(u32 *)(p->unk_B4.b[3] * 4 +
                                  (p->unk_10C * 4 + p->unk_18)) &
                         c0);
                    *(u32 *)(p->unk_B4.b[3] * 4 + (p->unk_10C * 4 + p->unk_18)) =
                        (*(u32 *)(p->unk_B4.b[3] * 4 +
                                  (p->unk_10C * 4 + p->unk_18)) &
                         c1) |
                        ((u32)prim & c0);
                    e = e + 0xC;
                    prim = (POLY_FT4 *)((u8 *)prim + ((u8 *)prim)[3] * 4 + 4);
                }
                else
                {
                    count = 1;
                    e = func_8004C080((u8 *)&p->unk_78, (u8 *)&p->unk_A8,
                                      (u8 *)prim, e, (u8 *)p, (s32)&count);
                    count = count - 1;
                    while (count != -1)
                    {
                        *(u32 *)prim =
                            (*(u32 *)prim & c1) |
                            (*(u32 *)(p->unk_B4.b[3] * 4 +
                                      (p->unk_10C * 4 + p->unk_18)) &
                             c0);
                        *(u32 *)(p->unk_B4.b[3] * 4 +
                                 (p->unk_10C * 4 + p->unk_18)) =
                            (*(u32 *)(p->unk_B4.b[3] * 4 +
                                      (p->unk_10C * 4 + p->unk_18)) &
                             c1) |
                            ((u32)prim & c0);
                        prim = (POLY_FT4 *)((u8 *)prim + ((u8 *)prim)[3] * 4 +
                                            4);
                        count = count - 1;
                    }
                    e = e + 0xC;
                }
                if (p->unk_00 & 0x80)
                {
                    break;
                }
            }
        }
    done:
        nd = *(UnkNode **)(pl - 8);
        if (nd == NULL)
        {
            break;
        }
        pl = (u8 *)nd + 0x20;
        list = (u8 *)nd->unk_0C;
    }
    PopMatrix();
    *(POLY_FT4 **)((u8 *)D_80083160.unk_00 + 0x8D0) = prim;
}
