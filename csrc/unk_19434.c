/* The remainder of unk_1811C after the gprel_18B40 carve-out. Named for its
 * start offset. */
/* Another piece of the original unk_14934 chain, carved off when the
 * gprel_17B7C $gp cluster was taken out of the middle of unk_176B8. Named for
 * its start offset. See docs/addressing-and-translation-units.md.
 */
/* The tail of the original unk_14934 split, carved off when gprel_17504 was
 * taken out of the middle of it: a $gp cluster cannot be compiled with the
 * same -G as its neighbours, so the unit had to be cut in three. Named for its
 * start offset, like the other provisional units. See
 * docs/addressing-and-translation-units.md.
 */
#include "common.h"

void func_800423C0(u8 *, s32, u8 *);
void func_80041E70(void *);
void func_80047338(Unk800777AC *, u32, u32);
void func_80047468(Unk800777AC *, u32, u32);
void func_800463EC(u8 *, UnkVec *, UnkSlot *, s16);
void func_80045CC4(u8 *, UnkVec *, UnkSlot *, s16);
void func_80044D24(u8 *, UnkSlot *, s16);
void func_80041344(u8 *, u32 *);
void func_8003F6D4(s32, s32, s32 *, s32);
void func_80040B88(void);
void func_80042984(u8 *);
void func_80041AB0(void);
void func_80041AE4(void);
void func_80043CD0(void);
void func_80043D04(void);
s32 func_80042518(s8 *, s8);
UnkNode *func_8003FC64(s32);
void func_80043FD0(Unk44144 *);
s32 func_8003F794(s16, s16);
void func_80040454(void);

/* Links the node into one of the 0x20 tag slots at D_80083360/D_800833E0,
 * pushing any existing head down, or claims a free slot. Returns 0 only when
 * every slot is taken.
 *
 * Three shapes are load-bearing:
 *  - `D_800833E0[i] = (u32) n;` is duplicated into both arms of the null
 *    check. That gives the store its own basic block instead of putting it in
 *    the join block, so sched1 never has it and the `return 1` constant in one
 *    ready list, where the hazard heuristic always hoists the constant past
 *    it. jump2's cross-jumping then merges the two copies back and plants the
 *    join label in front of the store, which is why .L80044964 sits before it
 *    and the word count is unchanged.
 *  - the free-slot bookkeeping is `if (first == -1) { first = i; } last = i;`
 *    in that order. With `last = i` first, cse folds the second assignment to
 *    a copy of last's register; in this order the `last = i` insn is instead
 *    stolen into the bne's delay slot. The test has to be against -1, not
 *    `< 0`: the target compares against a loop-hoisted -1 in a register.
 *  - `return 0` is the last statement, so its block lands just before the
 *    epilogue and the test is a single bltz. An early return costs a bgez, a
 *    `j` and a move.
 *
 * The two tails of the D_8006E7B4 walk are genuinely duplicated in the target
 * -- they differ only in which index they use -- so they are written out
 * twice here as well. */

s32 func_80045C34(u8 *p, UnkVec *v, UnkSlot *s)
{
    for (;;)
    {
        UnkNode *n;

        if (!(s->unk_14 & 0x80))
        {
            func_80045CC4(p, v, s, s->unk_06);
            if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
            {
                return 0;
            }
        }
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

/* Emits one slot's elements: builds the matrix in the 0x1F800000 scratchpad,
 * then per 0xC-byte element either calls the element's own callback or clips,
 * projects and links one 0x28-byte primitive into the OT.
 *
 * LOAD-BEARING SHAPES, each measured by reverting it:
 *
 *  - The two OT-linking statements are BITFIELD stores through Unk45CC4T
 *    (`u32 addr:24; u32 len:8`), libgpu's P_TAG -- the original wrote
 *    `addPrim(ot, q)`. A hand-written `|` cannot match: the target needs
 *    `q->unk_00 & 0xFF000000` as the or's operand 0 AND 0xFFFFFF hoisted by
 *    loop.c, and `expand_binop` materialises operand 0's chain first, so one
 *    spelling cannot give both. See the landed func_800453E0 in
 *    csrc/gprel_18B40.c for the full mechanism and the loop-dump arithmetic.
 *    Worth 4 words plus the two lw/and register fields.
 *
 *  - `q->unk_16` is written as ONE store (a ternary), not an if/else with a
 *    store in each arm. This is worth 126 words and it is the subtler lever in
 *    the function: `reg_n_refs` is counted by flow, which runs BEFORE jump2
 *    cross-jumps the arms' two identical `sh` into one, so the if/else form
 *    charges the q+4 giv two loop-weighted refs for a store that exists once
 *    in the output. That inflates the giv's global-alloc priority past the
 *    scratchpad pointer's and exchanges $s0/$s1 across the whole function.
 *    With one store the giv drops from 77 refs to 75 -- 6*75/319 = 1.4107
 *    against the scratchpad's 7*148/724 = 1.4309 -- and the scratchpad is
 *    allocated first, as the target has it. A `u16` temp set in both arms and
 *    stored once after the join is byte-identical. NOTE the same change at the
 *    other two-arm q store, `q->unk_0E`, costs 6 words instead: the target has
 *    that site's two `sh 0xA($s1)` in branch delay slots and they must stay
 *    two stores. So the lever applies only where the target itself
 *    cross-jumped.
 *
 *  - `s->unk_0C` is reached in the array form (`s->unk_0C[3]` and
 *    `*(u32 *) s->unk_0C`), matching the declaration in include/structs.h and
 *    the spellings func_800463EC already uses. A `u32 unk_0C` field with
 *    `((u8 *) &s->unk_0C)[3]` also matches here, but would mean retyping a
 *    shared field and rewriting an already-matching function, so it was not
 *    taken.
 *
 *  - The accumulator in the sign arms is `s16`, not `s32`: a HImode consumer
 *    keeps the `lbu`/`sll 24`/`sra 24` in each arm instead of letting combine
 *    refold to `lb`. See matching-levers section 7.
 *
 *  - A function-scope `Unk80083160 *g = &D_80083160;` used ONLY at the tail
 *    store. One set with one use in another block lets update_equiv_regs
 *    delete the set so reload rematerialises the address at the use; it also
 *    fixes a 16-word prologue rotation, since the %hi scratch is then not a
 *    pseudo competing for $v0. Section 6.
 *
 *  - A separate `u8 f` for the flag byte rather than reusing the clamp temp,
 *    worth 7 words. */
void func_80045CC4(u8 *arg0, UnkVec *v, UnkSlot *s, s16 arg3)
{
    Unk45CC4SP *p = (Unk45CC4SP *)0x1F800000;
    Unk45CC4E *e;
    Unk45CC4P *q;
    Unk80083160 *g = &D_80083160;

    p->unk_20 = D_80083160.unk_00 + 0xB0;
    p->unk_E4 = *(s16 *)((u8 *)v + 2);
    p->unk_E8 = *(s16 *)((u8 *)v + 6);
    p->unk_EC = *(s16 *)((u8 *)v + 0xA);
    p->unk_8C = 0;
    p->unk_84 = 0;
    p->unk_7C = 0;
    p->unk_74 = 0;
    q = *(Unk45CC4P **)(D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    PushMatrix();
    p->unk_108 = s->unk_20;
    p->unk_10A = s->unk_22;
    p->unk_30.w = s->unk_1C << 1;
    p->unk_34.w = s->unk_1E << 1;
    p->unk_38 = 0x2000;
    p->unk_100 = s->unk_16;
    p->unk_104 = s->unk_1A;
    p->unk_102 = s->unk_18;
    RotMatrix((void *)0x1F800100, (void *)0x1F8000D0);
    ReadRotMatrix((void *)0x1F80011C);
    CompMatrix((void *)0x1F80011C, (void *)0x1F8000D0, (void *)0x1F800050);
    ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
    SetTransMatrix((void *)0x1F800050);
    SetRotMatrix((void *)0x1F800050);
    e = (Unk45CC4E *)s->unk_08;
    p->unk_24 = s->unk_14;
    for (;;)
    {
        if ((e->unk_00 & 0x20) == 0)
        {
            s32 u;
            s32 w;
            s16 a;
            s32 otz;

            u = e->unk_08;
            p->unk_08.w = u;
            w = e->unk_0A;
            p->unk_10.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0)
            {
                p->unk_10.w = w - 1;
            }
            u = e->unk_09;
            p->unk_0C.w = u;
            w = e->unk_0B;
            p->unk_14.w = w;
            if (u + w >= 0x100 || s->unk_1A != 0)
            {
                p->unk_14.w = w - 1;
            }
            if ((e->unk_00 ^ p->unk_24) & 1)
            {
                a = -(s8)((u8 *)e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a - p->unk_10.h;
                p->unk_88 = a;
                p->unk_78 = a;
            }
            else
            {
                a = (s8)((u8 *)e)[2] - p->unk_108;
                p->unk_80 = a;
                p->unk_70 = a;
                a = a + p->unk_10.h;
                p->unk_88 = a;
                p->unk_78 = a;
            }
            if ((e->unk_00 ^ p->unk_24) & 2)
            {
                a = -(s8)((u8 *)e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a - p->unk_14.h;
                p->unk_8A = a;
                p->unk_82 = a;
            }
            else
            {
                a = (s8)((u8 *)e)[3] - p->unk_10A;
                p->unk_7A = a;
                p->unk_72 = a;
                a = a + p->unk_14.h;
                p->unk_8A = a;
                p->unk_82 = a;
            }
            otz = RotAverage4(&p->unk_70, &p->unk_78, &p->unk_80, &p->unk_88,
                              (s32 *)&q->unk_08, (s32 *)&q->unk_10,
                              (s32 *)&q->unk_18, (s32 *)&q->unk_20,
                              &p->unk_90, &p->unk_94) -
                  arg3 - 8;
            p->unk_C0 = otz;
            if ((u32)otz < 0x1E0)
            {
                u8 t;
                u8 f;
                if ((((u16)(q->unk_08 + 0x20) < 0x181 &&
                      (u16)(q->unk_0A + 0x20) < 0x121) |
                     ((u16)(q->unk_10 + 0x20) < 0x181 &&
                      (u16)(q->unk_12 + 0x20) < 0x121) |
                     ((u16)(q->unk_18 + 0x20) < 0x181 &&
                      (u16)(q->unk_1A + 0x20) < 0x121) |
                     ((u16)(q->unk_20 + 0x20) < 0x181 &&
                      (u16)(q->unk_22 + 0x20) < 0x121)) != 0)
                {
                    ((u8 *)&q->unk_00)[3] = 9;
                    s->unk_14 &= 0x7FFF;
                    p->unk_10.w = p->unk_10.w + p->unk_08.w;
                    p->unk_14.w = p->unk_14.w + p->unk_0C.w;
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
                    if ((s16)p->unk_50 >= 0x1800)
                    {
                        t = q->unk_24.b[0];
                        q->unk_24.b[0] = t + 0xFF;
                        q->unk_14.b[0] = t;
                    }
                    if ((s16)p->unk_58 >= 0x1800)
                    {
                        t = q->unk_24.b[1];
                        q->unk_24.b[1] = t + 0xFF;
                        q->unk_1C.b[1] = t;
                    }
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
                    ((Unk45CC4T *)q)->addr = ((Unk45CC4T *)(p->unk_20 + p->unk_C0 * 4))->addr;
                    ((Unk45CC4T *)(p->unk_20 + p->unk_C0 * 4))->addr = (u32)q;
                    q++;
                }
            }
        }
        else
        {
            void (*f)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *);

            f = *(void (**)(u8 *, UnkVec *, UnkSlot *, Unk45CC4E *))&e->unk_08;
            if (f != NULL)
            {
                f(arg0, v, s, e);
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

s32 func_80046398(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800463EC(p, v, s, s->unk_06);
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

/* Transforms and emits one model's faces: builds the matrix, then per face
 * loads four vertices into the scratchpad, projects them with RotAverage4,
 * back-face-culls with NormalClip and adds the primitive.
 *
 * The load-bearing shape is that the scratchpad vertex and the vertex table
 * are both 4-halfword SVECTORs, and the x/y pair is copied as one u32 through
 * a cast while vz is copied as a struct field. That asymmetry pipelines the
 * x/y group exactly one deep and leaves the z group serial -- five stall nops,
 * which is the target. Which MEMs are casts and which are struct fields is
 * what decides it, and the four combinations are measured below; the mechanism
 * is NOT established, though. The obvious reading is
 * true_dependence's MEM_IN_STRUCT_P exception, but that needs the non-struct
 * side's address not to vary and here every base is a register, so something
 * else is feeding in -- most likely memrefs_conflict_p comparing constant
 * offsets off a shared base. See docs/matching-levers.md section 5. It is
 * also what the original would naturally write, since the word store spans two
 * fields and needs a cast while the vz store does not. Measured alternatives:
 * both sides fields 297 words with no pipelining at all, both sides casts 293,
 * stores as casts with loads as fields 286, the reverse 300, and literal
 * constant scratchpad addresses 294 words but 138 differing (every store
 * rebuilds `lui $at, 0x1F80`, so the matching count is a coincidence).
 *
 * unk_114 is volatile: the target stores NormalClip's result and reloads it
 * before the blez, and without volatile cse folds the reload away (3 words).
 *
 * Also established and worth not re-deriving: one pointer local for every
 * scratchpad field access but literal `(void *) 0x1F8000XX` constants for the
 * libgte calls, which is the mixing func_8003D0F0 and func_80044724 also need;
 * RotAverage4's arguments through the pointer local instead; D_80083160
 * reached both ways in one function, with the pointer local declared first so
 * cse shares the one `lui` (the same ordering constraint as func_80043A68);
 * tbl and tex read before PushMatrix; a three-case switch plus default, which
 * is below gcc's jump-table threshold so it builds the decision tree the
 * target has; and `break` out of `for (;;)` with the termination test at the
 * bottom. */
void func_800463EC(u8 *arg0, UnkVec *v, UnkSlot *s, s16 depth)
{
    Unk80083160 *g = &D_80083160;
    Unk463ECSP *sp = (Unk463ECSP *)0x1F800000;
    SVECTOR *tbl = (SVECTOR *)g->unk_1E4;
    u8 *tex = (u8 *)g->unk_1E8;
    u8 m[0x20];
    Unk463ECF *f;
    POLY_FT4 *p;

    PushMatrix();
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_38 = s->unk_20;
    sp->unk_40 = ((s16 *)v)[1];
    sp->unk_44 = ((s16 *)v)[3];
    sp->unk_48 = ((s16 *)v)[5];
    ReadRotMatrix(m);
    TransMatrix((void *)0x1F800050, (void *)0x1F800040);
    RotMatrix(&s->unk_16, (void *)0x1F800050);
    if (s->unk_14 & 1)
    {
        sp->unk_30 = -sp->unk_30;
    }
    if (s->unk_14 & 2)
    {
        sp->unk_34 = -sp->unk_34;
    }
    ScaleMatrix((void *)0x1F800050, (void *)0x1F800030);
    CompMatrix(m, (void *)0x1F800050, (void *)0x1F8000D0);
    SetTransMatrix((void *)0x1F8000D0);
    SetRotMatrix((void *)0x1F8000D0);
    sp->unk_20 = (u32)(D_80083160.unk_00 + 0xB0);
    p = (POLY_FT4 *)*(u32 *)(D_80083160.unk_00 + 0x8D0);
    s->unk_14 |= 0x8000;
    f = ((Unk463ECF **)g->unk_1E0)[s->unk_08];
    for (;;)
    {
        long sxy0, sxy1, sxy2;

        *(u32 *)&sp->unk_70[0] = *(u32 *)&tbl[f->unk_00];
        *(u32 *)&sp->unk_70[1] = *(u32 *)&tbl[f->unk_02];
        *(u32 *)&sp->unk_70[2] = *(u32 *)&tbl[f->unk_04];
        *(u32 *)&sp->unk_70[3] = *(u32 *)&tbl[f->unk_06];
        sp->unk_70[0].vz = tbl[f->unk_00].vz;
        sp->unk_70[1].vz = tbl[f->unk_02].vz;
        sp->unk_70[2].vz = tbl[f->unk_04].vz;
        sp->unk_70[3].vz = tbl[f->unk_06].vz;
        sp->unk_C0 = RotAverage4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2],
                                 &sp->unk_70[3], &p->x0, &p->x1,
                                 &p->x2, &p->x3, &sp->unk_90, &sp->unk_94);
        switch (s->unk_14 & 3)
        {
        case 0:
            sxy0 = *(s32 *)&p->x0;
            sxy1 = *(s32 *)&p->x1;
            sxy2 = *(s32 *)&p->x2;
            break;
        case 1:
            sxy0 = *(s32 *)&p->x1;
            sxy1 = *(s32 *)&p->x0;
            sxy2 = *(s32 *)&p->x3;
            break;
        case 2:
            sxy0 = *(s32 *)&p->x3;
            sxy1 = *(s32 *)&p->x2;
            sxy2 = *(s32 *)&p->x1;
            break;
        default:
            sxy0 = *(s32 *)&p->x2;
            sxy1 = *(s32 *)&p->x3;
            sxy2 = *(s32 *)&p->x0;
            break;
        }
        sp->unk_114 = NormalClip(sxy0, sxy1, sxy2);
        if (sp->unk_114 > 0)
        {
            sp->unk_C0 = sp->unk_C0 - depth;
            if ((u32)sp->unk_C0 < 0x1E0)
            {
                u16 t;

                func_80065034(tex + f->unk_10 * 8, s->unk_0C, (u8 *)&p->r0);
                *(u32 *)&p->u0 = f->unk_08;
                p->clut = p->clut + s->unk_12;
                *(u32 *)&p->u1 = f->unk_0C;
                t = s->unk_10;
                if (t != 0)
                {
                    p->tpage = t + (p->tpage & 0xFF9F);
                }
                *(u16 *)&p->u2 = f->unk_12;
                *(u16 *)&p->u3 = f->unk_14;
                SetPolyFT4(p);
                if (s->unk_14 & 8)
                {
                    if (s->unk_14 & 4)
                    {
                        p->code |= 2;
                    }
                }
                AddPrim((void *)(sp->unk_20 + sp->unk_C0 * 4), p);
                p++;
                s->unk_14 &= 0x7FFF;
            }
        }
        if (f->unk_16 == 1 && f->unk_17 < 0)
        {
            break;
        }
        f++;
    }
    PopMatrix();
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)p;
}

/* Projects the four corners of a rect through func_80046A5C, clips each
 * against the reference point in out[5] with func_80046AFC, and if any corner
 * was clipped replaces all four with a ring of offsets around the transformed
 * camera position; then copies out[0] into out[4].
 *
 * Load-bearing shapes, with the cost of reverting each:
 *  - the third parameter is s32, not s16 (8 DIFF). With s16, gcc splits the
 *    parameter copy into an SImode copy plus a HImode subreg copy, and that
 *    second insn is schedulable, so it drifts to the end of the prologue
 *    block; the target has the plain SImode copy pinned right after its
 *    register save. The sll/sra in the loop is func_80046AFC's own s16
 *    conversion, not a cast here.
 *  - func_80064584/func_800644B8 take s32 (120 words otherwise): `ang` is s32,
 *    so an s16 parameter makes gcc sign-extend at both call sites.
 *  - `origin` is two explicit word stores into a u32[2]. Every aggregate
 *    initialiser spelling calls bzero instead (126 words), which also forces
 *    `r` out of $a0 into a callee-saved register; three `sh` field zeroes on an
 *    SVECTOR cost 99 words.
 *  - the origin zeroing sits before the ReadGeomScreen statement, not just
 *    before the first func_80046A5C call (22 DIFF): the two stores have to be
 *    in the prologue block so sched1 can use them to cover the
 *    `lhu $v1,0x6($a0)` load delay.
 *  - the corner stores are chained assignments, in this direction. Eight
 *    separate statements cost 128 words; `A = B = v` stores B first, and the
 *    target stores the higher address of each pair first, so reversing any
 *    pair is 8 DIFF. r[0] and r[1] really are reloaded for the second and
 *    fourth statements -- the intervening stack stores invalidate cse's memory
 *    table -- so they must not be hoisted into locals.
 *  - the z store is one four-deep chain (119 words otherwise); it evaluates
 *    innermost-first, which gives the target's descending 0x2C/0x24/0x1C/0x14.
 *  - D_80083160 is reached through a pointer local declared at the top (116
 *    words otherwise), as in func_8003D5A4: the whole address is materialised
 *    in ReadGeomScreen's delay slot and carried in $s6 across every call even
 *    though its first use is inside the if.
 *  - the tail copy is field-by-field; `out[4] = out[0];` is an 8-byte block
 *    move where the target has three lhu/sh pairs.
 *
 * arg0 is a four-halfword x/y/w/h rect, the same shape func_800404F8 and
 * LoadImage take; if the project ever grows a RECT type those three belong
 * together. */
void func_80046884(RECT *r, SVECTOR *out, s32 t)
{
    SVECTOR c[4];
    SVECTOR xf[4];
    u32 origin[2];
    u16 v58[4];
    u16 v60[4];
    s32 i;
    s32 flag = 0;
    Unk80083160 *g = &D_80083160;

    c[0].vx = c[3].vx = r->x;
    c[1].vx = c[2].vx = r->x + r->w;
    c[0].vy = c[1].vy = r->y;
    c[3].vy = c[2].vy = r->y + r->h;
    origin[0] = 0;
    origin[1] = 0;
    c[0].vz = c[1].vz = c[2].vz = c[3].vz = ReadGeomScreen();
    func_80046A5C((u16 *)origin, &out[5]);
    for (i = 3; i >= 0; i--)
    {
        func_80046A5C((u16 *)&c[i], &xf[i]);
        if (func_80046AFC(&out[5], &xf[i], &out[i], t) != 0)
        {
            flag = 1;
        }
    }
    if (flag != 0)
    {
        s32 ang = g->unk_C8 + 0x600;

        v58[0] = g->unk_AC;
        v58[1] = g->unk_AE;
        v58[2] = g->unk_B0;
        func_80046A5C(v58, v60);
        for (i = 3; i >= 0; i--)
        {
            s32 cc = func_80064584(ang);
            s32 ss = func_800644B8(ang);

            ang += 0x400;
            out[i].vx = v60[0] + (cc >> 3) + (cc >> 4);
            out[i].vy = v60[1] + (ss >> 3) + (ss >> 4);
            out[i].vz = v60[2];
        }
    }
    out[4].vx = out[0].vx;
    out[4].vy = out[0].vy;
    out[4].vz = out[0].vz;
}

void func_80046A5C(u16 *arg0, void *out)
{
    u16 m[0x10];
    u16 t[0x10];
    s16 v[4];

    ReadRotMatrix(m);
    PushMatrix();
    v[0] = arg0[0] - m[10];
    v[1] = arg0[1] - m[12];
    v[2] = arg0[2] - m[14];
    TransposeMatrix(m, t);
    ApplyMatrixSV(t, v, out);
    PopMatrix();
}

/* Interpolates a/b at height t into out, clamps both coordinates to
 * +/-0x7FF0, and returns 1 if the result lies more than 0x1000 from a in
 * either axis.
 *
 * Three shapes are load-bearing:
 *  - `dx` is hoisted into its own local one statement before `f`. That costs
 *    no instruction (sched1 still issues f's subtract first) but it pushes
 *    the last use of the CSE'd b->vz load one slot later, so its live
 *    range grows from 11 insns to 12 and its global.c priority drops just
 *    below b's own. b is then allocated first and takes $a0, the load is
 *    pushed to $a1, and a/out/t slide down to $t1/$t2/$t3 as in the target.
 *  - cx's clamp is the preset-then-nested form and cy's is the if/else form
 *    with the -0x7FF0 arm in the else. They are not interchangeable: that
 *    asymmetry is what makes both clamps' branch layout and delay slots come
 *    out right.
 *  - cx and cy have to be separate variables; one shared variable is
 *    coalesced with nx and costs a `j`. */
s16 func_80046AFC(SVECTOR *a, SVECTOR *b, SVECTOR *out, s16 t)
{
    s32 dz = b->vz - a->vz;
    s32 f;
    s32 nx;
    s32 ny;
    s32 cx;
    s32 cy;
    s32 dx;

    if (dz < 0x40)
    {
        return 1;
    }
    dx = b->vx - a->vx;
    f = t - b->vz;
    nx = dx * f / dz + b->vx;
    ny = (b->vy - a->vy) * f / dz + b->vy;
    cx = -0x7FF0;
    if (nx >= -0x7FF0)
    {
        cx = 0x7FF0;
        if (nx < 0x7FF1)
        {
            cx = nx;
        }
    }
    out->vx = cx;
    if (ny >= -0x7FF0)
    {
        cy = 0x7FF0;
        if (ny < 0x7FF1)
        {
            cy = ny;
        }
    }
    else
    {
        cy = -0x7FF0;
    }
    out->vy = cy;
    out->vz = t;
    if (out->vx < a->vx - 0x1000)
    {
        return 1;
    }
    if (a->vx + 0x1000 < out->vx)
    {
        return 1;
    }
    if (out->vy < a->vy - 0x1000)
    {
        return 1;
    }
    return a->vy + 0x1000 < out->vy;
}

/* Builds four per-edge records from five points, clamping each y to a
 * multiple of 0x40 and returning the lowest y seen, likewise clamped.
 * Degenerate pairs are marked with unk_1C = -1 and decrement *cnt.
 *
 * Load-bearing shapes:
 *  - `dx = abs(dx);` rather than `if (dx < 0) { dx = -dx; }`. The MIPS abssi2
 *    pattern is a single insn whose template contains its own `bgez`/`subu`,
 *    so it introduces no basic-block boundary and reorg cannot fill the branch
 *    delay slot inside it -- which is why the target has a bare nop there.
 *    With the abs as one insn the whole else-arm tail is a single block, so
 *    sched1 can hoist the out->unk_14 reload (emitted at the `> 0` test, where
 *    cse genuinely cannot fold it because the unk_20/unk_08 stores invalidated
 *    the entry) up to immediately after the store. Written as an if, the
 *    reload is stuck in the join block. It also settles the $v0/$v1 choice on
 *    the two loads feeding dx. Same pattern as func_8003F6F4.
 *  - `out` is a pointer incremented by the loop, with every field reached as
 *    out->field. That is what gives the target's two induction registers: $a1
 *    is the biv and $t0 = $a1 + 0x1C the one surviving address giv. Indexing
 *    as out[i].field makes gcc use a single giv at +0 with displacements
 *    0..0x24.
 *  - `out->unk_1C = 0;` is the LAST out statement in the else arm even though
 *    the emitted code shows it in the middle. gcc builds the giv list by
 *    prepending as it discovers them, so the last access becomes
 *    combine_givs' representative and sets the surviving pointer's +0x1C base;
 *    written earlier the base comes out +0x24.
 *  - `out->unk_04 = out->unk_0C << 7;` reads the field back rather than using
 *    `dy << 7`, which is what produces the copy-then-shift pair instead of a
 *    single `sll`, and it also settles min/base into $t3/$t4.
 *  - `s32 m = -0x40;` is a local declared in the loop BODY, used for both the
 *    mask and the -0x40 store but not for the return. The mask has to reach
 *    the `and` as a loop-invariant register: as a literal in both places gcc
 *    keeps the constant inside the loop and combine narrows it to an
 *    `andi 0xffc0` immediate using nonzero_bits of the lhu. loop.c hoists the
 *    local's single set into the preheader, which sits after `i = 0`, giving
 *    the target's register order. At function scope the set is emitted ahead
 *    of `i = 0`; in the else arm it is not a movable of the loop proper and
 *    costs 26 words.
 *  - `min` is s32; as s16 both the compare and the return need their own
 *    sll/sra pair. */
s32 func_80046C20(Unk46C20A *a, Unk46C20B *out, u16 *cnt)
{
    s32 min = 0x7FFF;
    s32 i;

    for (i = 0; i < 4; i++, out++)
    {
        s32 d = a[i].unk_02 - a[i + 1].unk_02;
        s32 m = -0x40;

        if (d == 0)
        {
            out->unk_1C = -1;
            *cnt = *cnt - 1;
        }
        else
        {
            s32 lo;
            s32 hi;
            s32 y;
            s32 dy;
            s32 dx;

            if (d < 0)
            {
                lo = i;
                hi = i + 1;
            }
            else
            {
                lo = i + 1;
                hi = i;
            }
            out->unk_00 = a[lo].unk_00;
            y = (s16)((u16)a[lo].unk_02 & m);
            out->unk_18 = y;
            if (y < min)
            {
                min = y;
            }
            dy = a[hi].unk_02 - y;
            out->unk_0C = dy;
            out->unk_10 = dy;
            out->unk_04 = out->unk_0C << 7;
            dx = a[hi].unk_00 - out->unk_00;
            out->unk_14 = dx;
            dx = abs(dx);
            out->unk_20 = dx;
            out->unk_08 = dx << 7;
            if (out->unk_14 > 0)
            {
                out->unk_14 = 0x40;
            }
            else if (out->unk_14 < 0)
            {
                out->unk_14 = m;
            }
            out->unk_20 = out->unk_20 + 0x20;
            out->unk_24 = out->unk_20;
            out->unk_1C = 0;
        }
    }
    return min & ~0x3F;
}

void func_80046D64(u8 *base, s16 arg1)
{
    Unk80083160 *s = &D_80083160;
    u32 *p = &s->unk_1DC;
    u32 *q;
    u32 v;
    s32 i;

    /* v is used twice, so combine cannot fold the copy into q and q is a loop
     * variable (a global allocno) so local-alloc cannot tie them: the
     * `addu $a2,$v0,$zero` survives, which renumbers the allocnos enough that
     * p takes $a0 and base is pushed to $t0. */
    v = (u32)(base + D_8006E7F0[arg1][4] * 4);
    p[1] = v;
    q = (u32 *)v;
    for (i = D_8006E7F0[arg1][3]; i > 0; i--)
    {
        *q += (u32)base;
        q++;
    }
    p[2] = (u32)(base + D_8006E7F0[arg1][5] * 4);
    p[3] = (u32)(base + D_8006E7F0[arg1][6] * 4);
    p[4] = (u32)(base + D_8006E7F0[arg1][7] * 4);
}

/* Streams a run of compressed blocks through the unpacker, then hands the
 * result to func_80046D64.
 *
 * Load-bearing shapes, each measured by reverting it:
 *  - D_80080A7C is written by two separate stores in an if/else, not from a
 *    variable. Every single-store spelling costs 30 words: with one store the
 *    jump optimizer hoists the 0x8000 assignment above the lbu, so the pseudo
 *    is live across the comparison, conflicts with $v0/$v1 and lands in $a0,
 *    and sched2 is then free to hoist the (base+3)&~3 chain in front of the
 *    store. As two stores, cross-jumping merges the common `sw` and leaves
 *    each arm's constant in its own block, so the value is defined after the
 *    comparison, takes $v0, and the anti-dependence pins the store ahead of
 *    the addiu. It also recovers the word that the duplicated
 *    `addiu $a0,$zero,0x6` costs, which reorg steals from the loop top into
 *    the bgtz's delay slot.
 *  - `base = D_80081480 = D_8008148C;` as a chained store (78 words
 *    otherwise). Same store_expr-with-want_value pattern as func_80041284:
 *    load to temp, store, copy -- and the copy fills DrawSync's delay slot.
 *  - `n--` after the func_8004068C call, not before it (4 words otherwise):
 *    it has to be the insn immediately before the jal so reorg takes it for
 *    the delay slot instead of the $a1 argument setup.
 *  - the table entry is re-dereferenced at each use rather than cached in a
 *    local (55 words otherwise). The two stores to the stack local invalidate
 *    cse's memory table, so those loads are genuinely reloaded; only the
 *    address arithmetic survives in a register.
 *  - `buf = *...->unk_00;` as a whole-struct assignment, which is gcc's
 *    8-byte movstrsi (lw/lw/sw/sw); two field copies cost 6 words.
 *  - arg0 is s16 (76 words otherwise): the raw parameter stays in $s0 and each
 *    use sign-extends it separately, once by 14 for the *4 index and once by
 *    16 for the func_80046D64 argument.
 *  - the parameter arg1 is what the loop walks and a separate `orig` keeps the
 *    original (12 words the other way round).
 *  - D_80082E60.unk_0A, not a scalar at D_80082E6A -- the struct-field read is
 *    what puts the %hi in a different register from the lbu, as recorded for
 *    func_8003E14C.
 *  - `while (n > 0)`, not `!= 0`, with n a plain s32 holding the lhu-loaded
 *    count, so the test is blez/bgtz and the decrement needs no masking. */
void func_80046E38(s16 arg0, u8 *arg1)
{
    Unk46E38A buf;
    u8 *orig = arg1;
    u32 base;
    s32 n;
    s32 addr;

    base = D_80081480 = D_8008148C;
    DrawSync(0);
    buf = *((Unk8006E7F0 *)D_8006E7F0[arg0])->unk_00;
    n = ((Unk8006E7F0 *)D_8006E7F0[arg0])->unk_04;
    addr = ((Unk8006E7F0 *)D_8006E7F0[arg0])->unk_00->unk_04;
    while (n > 0)
    {
        func_8003E4FC(6, (u8 *)&buf, 0);
        addr += 0x10;
        func_8003F320();
        arg1 = func_8004068C((u8 *)D_80081480, arg1);
        n--;
        func_8003F6D4(0x10, 0, (s32 *)&buf, addr);
    }
    func_80046D64(orig, arg0);
    if (D_80082E60.unk_0A == 2)
    {
        D_80080A7C = 0x4000;
    }
    else
    {
        D_80080A7C = 0x8000;
    }
    D_80081480 = (base + 3) & ~3;
}

s32 func_80046F88(u32 *p)
{
    s32 delta = 0;
    s32 i;
    u32 *q;

    /* Single exit returning delta, and 0xC updated before 0x14: the
     * scheduler emits that pair in the opposite order to the source. */
    if (p != (u32 *)p[9])
    {
        delta = (s32)((u8 *)p - (u8 *)p[9]);
        p[9] = (u32)p;
        p[0] += delta;
        p[2] += delta;
        p[3] += delta;
        p[5] += delta;
        p[7] += delta;
        q = (u32 *)p[0];
        for (i = 0; i < (s32)p[1]; i++)
        {
            *q += delta;
            q++;
        }
        while ((u32)q < p[2])
        {
            q[1] += delta;
            q += 2;
        }
    }
    return delta;
}

INCLUDE_ASM("asm/nonmatchings/unk_19434", func_80047054);

/* Uploads `arg2` 0x2000-byte tiles to VRAM, walking a 2x4 grid of slots: the
 * x coordinate steps by 0x20 after the first pair and the y coordinate
 * alternates by 0x80, with both offsets based on the page number in arg1.
 *
 * THE BLOCKER THIS USED TO BE. D_80080B08 is reached BOTH ways: the two
 * coordinate stores are `sh %gp_rel(D_80080B08)` / `sh %gp_rel(D_80080B0A)`
 * while the RECT address handed to LoadImage is an absolute `lui %hi` /
 * `addiu %lo` pair. No unit-level setting can express that -- a `-G` value
 * decides per symbol SIZE and an `.extern` override per SYMBOL, and both apply
 * to every access alike -- so this unit could not simply be carved at -G8.
 * maspsx can, because its %gp_rel rewrite fires on a load or store and leaves
 * an address materialisation alone, and the bare size record below asks for it
 * while DEFINING NOTHING: .sdata stays 0 bytes and both symbols stay UND, so
 * the linker script's own assignments resolve them and there is no placement
 * to arrange. See the longer note on func_8003F624 in csrc/unk_12DEC.c; these
 * two symbols are the only $gp references anywhere in this unit, so no carve
 * was needed at all.
 *
 * THE ONE LOAD-BEARING SHAPE is where `r` is declared, and it is worth 4 words
 * plus a register. Three things in the body are loop-invariant -- the RECT
 * address, `arg1 << 4` and `arg1 << 6` -- and loop.c hoists only two of them
 * into the preheader. Which two depends on the order it meets them, so:
 *  - with `r` declared OUTSIDE the loop it is an ordinary source statement and
 *    lands before the loop GUARD, not in the preheader, leaving both shifts to
 *    be hoisted: 47 words, 44 DIFF.
 *  - with no `r` at all the address is rebuilt per iteration inside the loop
 *    and both shifts are still hoisted: 45 words, 45 DIFF.
 *  - with `r` declared as the loop body's FIRST statement it is a loop.c hoist
 *    like the others, it is met first, and `arg1 << 6` is the one left behind.
 *    That is the target: the `sll`/`andi` pair for x stays in the loop, which
 *    is why the first `beqz` has an empty delay slot, while the hoisted
 *    `arg1 << 4` lets sched1 drop y's `andi` into the second `beqz`'s slot.
 *
 * So this extends section 6's pointer-local entry: the lever is not only
 * whether a pointer local exists but WHERE it is declared, because that decides
 * whether its address materialisation is a statement or a hoist, and a hoist
 * competes with the loop's other invariants. Declaring `y` separately and
 * early, to try to lose `arg1 << 6` that way instead, gives 49 words and
 * 7 DIFF -- the right word count with the wrong registers. */
__asm__(".sdata\n\t.size D_80080B08,2\n\t.size D_80080B0A,2\n\t.text");

void func_8004713C(u32 *arg0, u16 arg1, u16 arg2)
{
    s32 i;

    for (i = 0; i < arg2; i++)
    {
        RECT *r = (RECT *)&D_80080B08;
        s32 x = (arg1 << 6) & 0x3C0;

        if (i / 2 != 0)
        {
            x += 0x20;
        }
        D_80080B08 = x;
        x = (arg1 << 4) & 0x100;
        if (i & 1)
        {
            x += 0x80;
        }
        D_80080B0A = x;
        LoadImage(r, arg0);
        arg0 += 0x800;
    }
}

void func_80047200(u16 *p, s32 rows, s32 flag)
{
    s32 i;
    s32 j;

    if (flag == 0)
    {
        for (i = 0; i < rows; i++)
        {
            /* p++ first in the body, not last: the back-edge bnez then finds
             * `addiu $a0,$a0,2` at its target, and reorg's
             * fill_slots_from_thread steals it into the delay slot, redirects
             * the branch past it and emits the compensating addiu -2. */
            p++;
            for (j = 1; j < 0x10; j++)
            {
                *p |= 0x8000;
                p++;
            }
        }
        return;
    }
    for (j = 1; j < 0x100; j++)
    {
        *p |= 0x8000;
        p++;
    }
}

void func_80047270(u8 *arg0, RECT *rect)
{
    s32 t = rect->h * 2;
    s32 n = rect->w * t;

    if (D_8008148C + D_80080A7C <= D_80081480 + n)
    {
        D_80081480 = D_8008148C;
        DrawSync(0);
    }
    func_800407C0(arg0, (u32 *)D_80081480);
    LoadImage(rect, (u32 *)D_80081480);
    t = rect->h * 2;
    D_80081480 = D_80081480 + rect->w * t;
}
