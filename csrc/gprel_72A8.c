#include "common.h"

/* The first fourteen functions of the old unk_72A8, carved off for ONE WORD.
   func_80033D44 and func_80033D54 -- the last two functions in this file --
   reach D_8008099C $gp-relatively, while func_80034EF8 and
   func_8003C0C8, further down the old unit, reach the same word ABSOLUTELY
   and are matched that way. maspsx's size record is file-scope, so no single
   translation unit can give one symbol both forms; the __asm__-alias trick
   that gives one symbol two spellings elsewhere in this project does not lift
   it either (measured 2026-10-27, see the note at the top of csrc/unk_72A8.c).
   So the unit is cut immediately after func_80033D54, which is also the
   earliest cut that separates the two groups, and this piece asks for the
   size record the stubs need.

   This unit stays at the project default -G0, like gprel_11B4C and unlike the
   other gprel_* units: the one access it wants is asked for by the size
   record below, and nothing here reaches a small object absolutely.

   The twelve functions above them came along because they are on this
   side of the cut, not because they need anything from it -- none of them
   touches D_8008099C or any of the old unit's other size-recorded symbols,
   which is why the carve moves no matched code. See
   docs/addressing-and-translation-units.md. */
__asm__(".sdata\n\t.size D_8008099C,4\n\t.text");

/* The bitmap set, clear and test on the out-of-image words at 0x80012D70,
   whose base 0x80010000 has no low half, so 0x2D70 stays in the load and store
   displacements. The base is assigned AFTER the two guards so its `lui` is the
   first insn of the following block and reorg lifts it into the second guard's
   delay slot; at the top of the function the index chain is emitted first and
   the `lui` lands after the `sll` (3 DIFF). Adding the index to the base in a
   separate statement (`p += ...`) also hands the `addu`'s destination to the
   pointer's quantity instead of the index chain's, which permutes $a0/$v0
   through the whole function (7 DIFF). */
void func_80033AA8(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return;
    }
    if (k == 1) {
        return;
    }
    p = (u8 *) 0x80010000;
    *(u32 *) (p + (k >> 5) * 4 + 0x2D70) |= 1 << (id & 0x1F);
}

/* The bitmap CLEAR, func_80033AA8's twin with `&= ~` for `|=`.

   ITS PARAMETER IS `u16`, and that is the whole difference from the twin: the
   listing's `andi $a0, $v1, 0xFFFF` is the PARAMETER's own truncation, which
   leaves the raw argument alive in $v1 for the `& 0x1F` and lets the `sllv`
   take the constant 1 out of $a1 before the `lw`. Declared `s32 id` with the
   mask written in the source, the raw value and the constant 1 hold each
   other's registers and six words differ. Nothing is lost by narrowing: both
   spellings use only the low sixteen bits of the argument.

   It is DEFINED UNDER AN __asm__ ALIAS so that this unit's own
   `void func_80033AE8(s32);` declarations and their call sites keep the
   32-bit view they are matched against -- func_80039258's
   `func_80033AE8((s16) v)` needs the `sll`/`sra` pair that a u16 prototype
   in scope would replace with a truncation. In the original sources the
   definition and those callers were in different files; the alias is how a
   carved unit says that (see docs/carved-sweep.md). */
void func_80033AE8w(u16) __asm__("func_80033AE8");

void func_80033AE8w(u16 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return;
    }
    if (k == 1) {
        return;
    }
    p = (u8 *) 0x80010000;
    *(u32 *) (p + (k >> 5) * 4 + 0x2D70) &= ~(1 << (id & 0x1F));
}

s32 func_80033B2C(s32 id) {
    u32 k = id & 0xFFFF;
    u8 *p;

    if (k == 0) {
        return 0;
    }
    if (k == 1) {
        return 1;
    }
    p = (u8 *) 0x80010000;
    return (*(u32 *) (p + (k >> 5) * 4 + 0x2D70) & (1 << (id & 0x1F))) != 0;
}

void func_80033AE8(s32);

void func_80033B78(s16 a) {
    func_80033AE8(a);
}

void func_80033AA8(s32);

void func_80033B9C(s16 a) {
    func_80033AA8(a);
}

s32 func_80033B2C(s32);

void func_80033BC0(s16 a) {
    func_80033B2C(a);
}

u8 *func_80033BE4(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        if (p[0] == 0) {
            return p;
        }
        p += 0x64;
    }
    return 0;
}

void func_80033C1C(u8 *p, u32 a) {
    *(u32 *) (p + 0x00) = 1;
    *(u32 *) (p + 0x04) = a;
    *(u16 *) (p + 0x08) = 0;
    *(u16 *) (p + 0x0A) = 0;
    *(u16 *) (p + 0x0C) = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    *(u32 *) (p + 0x18) = 0;
    *(u32 *) (p + 0x20) = 0x1000;
    *(u32 *) (p + 0x24) = 0x1000;
    *(u32 *) (p + 0x28) = 0x1000;
    *(u32 *) (p + 0x30) = 0;
    *(u32 *) (p + 0x34) = 0;
    *(u32 *) (p + 0x38) = 0;
    *(u32 *) (p + 0x40) = 0;
    *(u32 *) (p + 0x44) = 0;
    *(u32 *) (p + 0x48) = 0;
    *(u32 *) (p + 0x4C) = 0;
    *(u32 *) (p + 0x50) = 0;
    *(u32 *) (p + 0x54) = 0;
    *(u32 *) (p + 0x58) = 0;
    *(u32 *) (p + 0x5C) = 0;
    *(u32 *) (p + 0x60) = 0;
}

void func_80033C84(u32 *p) {
    *p = 0;
}

void func_80033C8C(void) {
    u8 *p = D_800816C0;
    s32 i;

    for (i = 0; i < 0x14; i++) {
        func_80033C84((u32 *) p);
        p += 0x64;
    }
}

void func_80033CD8(u8 *p, u32 tag) {
    if (*(u32 *) (p - 0xC) == 0) {
        func_8004491C((UnkNode *) (p - 0x20), tag);
    }
}

void func_80033D08(u8 *p) {
    if (*(u32 *) (p - 0xC) != 0) {
        func_80044A50((UnkNode *) (p - 0x20));
    }
}

s32 func_80033D38(u32 *p) {
    return p[-3] != 0;
}

/* Records the argument in the $gp word and hands it back. The copy to $v0
   ahead of the store is the return value; the two-statement spelling
   (`D_8008099C = a; return a;`) gives the same four words, measured. First
   draft, 4 of 4 -- the first thing the carve bought. */
s32 func_80033D44(s32 a)
{
    return D_8008099C = a;
}

/* The sprite/billboard draw of the object tree: for each node on the
   `UnkNode.unk_18` chain it builds the object's world matrix in the
   scratchpad, projects the four corners of every element record through
   RotTransPers4 and emits a POLY_FT4 per element. Fourth member of the
   `(u8 *p, UnkVec *v, UnkSlot *s)` family (func_80044BB0, func_80044C54,
   func_80045C34, func_80046398, func_800463EC); the inner loop is
   func_80044D24's (csrc/unk_1811C.c) with the scratchpad offsets shifted by
   +4 -- the element record (Unk44D24E), the +0x100 UV wrap clamps, the sign
   arms, `setlen`, the `s->unk_0C[3]` flag byte and the `AddPrim` tail are the
   same code. 537 of 537 words.

   It was parked at 46 DIFF with EVERY instruction right and four callee-saved
   register names wrong (two adjacent pairs in global-alloc's priority order,
   e/m1 and pl/m2), plus two scheduler tie-breaks. Three things landed it,
   each measured on its own:

   SYMPTOM: phantom bit test -- dead code whose only purpose is +2 weighted
   references on m1 and m2. Replace with anything that spends those references
   for a reason a person would have had; the other 535 words do not depend on
   which. The likely reading is that a declared type, a struct layout or the
   unit split is still wrong and is costing those two references elsewhere.

   1. THE PHANTOM TEST at the top of the outer loop body,
      `if ((((u32)m1 | (u32)m2) & 3) != 0) { p->unk_C4 = 0; }`, which EMITS
      NOTHING and is there only to be counted. global-alloc ranks by
      floor_log2(refs) * refs / live_length, and the two matrix pointers sat
      exactly one outer-loop reference (+2 weighted) below `e` and `pl` --
      0.235 against 0.248 and 0.064 against 0.077 -- with no instruction left
      that could carry one. flow counts references BEFORE combine runs and
      never recounts, and combine knows the nonzero bits of every once-set
      pseudo (`reg_nonzero_bits`, valid across blocks), so a bit test that is
      always false by the pointers' constants (0x1F800054 | 0x1F8000C8 has its
      low two bits clear) folds there, AFTER the count, into an always-taken
      jump; jump2 deletes the jump and the dead store after reload. Three
      details keep it alive until flow: loop.c sees the `ior`/`and` as
      invariant but refuses to hoist them ("savings 2 not desirable" in the
      .loop dump -- a register with a lifetime of 2 insns is not worth moving
      out of a 444-insn loop by scan_loop's threshold test), so they stay
      where cse2, which knows the constants only in the preheader, cannot
      fold them; inside the loop cse knows nothing about m1 and m2 (they are
      set in another block); and the dead store is what stops jump1 deleting
      a branch around nothing. The condition must fold to an always-TAKEN
      jump: the other way round the branch becomes (set pc pc), no pattern
      recognises it, and the compare stays (3 extra words, measured). m1 goes
      to 27 refs and m2 to 11, all four registers fall into place: 46 -> 10
      DIFF. AND-ing a loop-varying value into the mask (`& (3 & (u32)o)`)
      matches as well and is the form to reach for if a smaller loop ever
      lets loop.c hoist the test; any placement in the loop body works.
   2. `g` IS DECLARED BEFORE `p`. The prologue's `lw $v0, gp` / `lui $v1` /
      `sw $s1` / `lui $s1` are four priority-1 insns in sched2, the tie goes
      to ready-list order, which is LUID, which is declaration order: with
      `p` first its `lui $s1` is picked first and placed last. 10 -> 3 DIFF.
   3. `a` IS A VOLATILE READ THROUGH `q`, AFTER `p->unk_1C = prim + 1`, and
      `b` is a plain read. The listing loads e->unk_08 twice back to back
      (`lbu $v0` for the +0x0C store, `lbu $v1` for the clamp test) after the
      +0x1C store and before the +0x0C one. A plain `a` there is merged with
      the +0x0C reload by cse (a word short); read before the +0x1C store it
      is hoisted into the `lw $s0` delay slot (the park's 6 words); volatile
      and after it, cse leaves both loads, the QImode load cannot cross the
      scratchpad store, and the plain reload is still free to be scheduled
      ahead of it. `b` must NOT be volatile: two volatile loads carry an
      anti-dependence that turns the sched1 tie the other way, and b's
      zero-extension, used in the next block, then cannot fold (an `andi`).
      Through `e` instead of `q` it reads off $s5 (23 DIFF): a volatile MEM is
      not replaced by the reduced giv. 3 -> MATCH. `u8` for a and b matches
      as well; it is not load-bearing.

   Everything the park established is still in force: every element field is
   a struct read off `e` and loop.c's reduced giv is the +1 register ($s2),
   so there is no e+1 pointer in the source except for the volatile reads;
   the `(s8)x * 2` cast on a plain read folds to `lb`/`sll 1`, so exactly the
   two sign bytes and `a` are volatile; the matrix copy is eight word
   assignments (a struct copy is a four-register block move); arm B ends in
   `TransMatrix(m2, &p->unk_44)` and arm A in `TransMatrix(m2, tv)` or
   cross-jumping merges the 30-word tails; each sign arm has its own
   single-set locals (x0..x3, t0..t3) so sched1 keeps `subu`/`addu` after the
   two `sh`; the clip test is one expression of four `&&` pairs joined by `|`;
   the CLUT ternary is inside the u0/v0 store; `ot` is its own local so the
   prologue read lands in $v1; the three matrix pointers and `base` are
   assigned after the D_8008099C guard; the guard is `!= 0` with its own
   `return 0`; and `e = s->unk_08` sits before `p->unk_28 = s->unk_14` to fill
   the `lhu` delay slot. */
extern SVECTOR D_80080994;

typedef union Unk33D54XY
{
    s32 w;
    struct
    {
        u16 x;
        u16 y;
    } h;
} Unk33D54XY;

typedef struct Unk33D54O
{
    u8 pad_00[0x4];
    struct Unk33D54O *unk_04;
    SVECTOR unk_08;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    u8 pad_1C[0x4];
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    u8 pad_2C[0x4];
    VECTOR unk_30;
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    s32 unk_4C;
    s32 unk_50;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
} Unk33D54O;

typedef struct Unk33D54SP
{
    u8 pad_00[0xC];
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    Unk3DBD0W unk_18;
    POLY_FT4 *unk_1C;
    u8 pad_20[0x4];
    s32 unk_24;
    u16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    VECTOR unk_44;
    s32 unk_54;
    s32 unk_58;
    s32 unk_5C;
    s32 unk_60;
    s32 unk_64;
    s32 unk_68;
    s32 unk_6C;
    s32 unk_70;
    s16 unk_74;
    s16 unk_76;
    s16 unk_78;
    u8 pad_7A[0x2];
    s16 unk_7C;
    s16 unk_7E;
    s16 unk_80;
    u8 pad_82[0x2];
    s16 unk_84;
    s16 unk_86;
    s16 unk_88;
    u8 pad_8A[0x2];
    s16 unk_8C;
    s16 unk_8E;
    s16 unk_90;
    u8 pad_92[0x2];
    s32 unk_94;
    s32 unk_98;
    Unk33D54XY unk_9C;
    u8 pad_A0[0x4];
    Unk33D54XY unk_A4;
    u8 pad_A8[0x4];
    Unk33D54XY unk_AC;
    u8 pad_B0[0x4];
    Unk33D54XY unk_B4;
    u8 pad_B8[0xC];
    s32 unk_C4;
    MATRIX unk_C8;
} Unk33D54SP;

s32 func_80033D54(u8 *pl, UnkVec *v, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    Unk33D54SP *p = (Unk33D54SP *)0x1F800000;
    MATRIX *m1;
    MATRIX *m2;
    VECTOR *tv;
    u8 *base;
    Unk33D54O *o;
    Unk44D24E *e;
    POLY_FT4 *prim;
    POLY_FT4 *ot;
    u8 *q;
    UnkNode *n;
    void (*f)(u8 *);
    s32 x0;
    s32 x1;
    s32 x2;
    s32 x3;
    s32 t0;
    s32 t1;
    s32 t2;
    s32 t3;
    s32 t;
    s32 a;
    s32 b;
    u8 c;

    if (D_8008099C != 0)
    {
        return 0;
    }
    m1 = (MATRIX *)&p->unk_54;
    tv = &p->unk_44;
    m2 = &p->unk_C8;
    base = D_80083160.unk_00;
    ot = *(POLY_FT4 **)(base + 0x8D0);
    p->unk_24 = (s32)(base + 0x70);
    p->unk_34.vz = 0x1000;
    p->unk_44.vz = 0;
    p->unk_90 = 0;
    p->unk_88 = 0;
    p->unk_80 = 0;
    p->unk_78 = 0;
    p->unk_1C = ot;
    PushMatrix();
    for (;;)
    {
        o = *(Unk33D54O **)pl;
        f = *(void (**)(u8 *))(pl + 8);
        if ((((u32)m1 | (u32)m2) & 3) != 0)
        {
            p->unk_C4 = 0;
        }
        p->unk_C4 = o->unk_60;
        e = (Unk44D24E *)s->unk_08;
        p->unk_28 = s->unk_14;
        p->unk_34.vx = o->unk_20;
        p->unk_34.vy = o->unk_24;
        p->unk_34.vz = o->unk_28;
        if (o->unk_04 == NULL)
        {
            p->unk_44.vx = ((s16 *)v)[1];
            p->unk_44.vy = ((s16 *)v)[3];
            p->unk_44.vz = ((s16 *)v)[5];
            TransMatrix(m1, tv);
            RotMatrix(&o->unk_08, m1);
            ScaleMatrix(m1, &p->unk_34);
            o->unk_40 = p->unk_54;
            o->unk_44 = p->unk_58;
            o->unk_48 = p->unk_5C;
            o->unk_4C = p->unk_60;
            o->unk_50 = p->unk_64;
            o->unk_54 = p->unk_68;
            o->unk_58 = p->unk_6C;
            o->unk_5C = p->unk_70;
            p->unk_44.vx = o->unk_10;
            p->unk_44.vy = o->unk_14;
            p->unk_44.vz = o->unk_18;
            TransMatrix(m2, tv);
        }
        else
        {
            TransMatrix(m1, &o->unk_30);
            RotMatrix(&o->unk_08, m1);
            ScaleMatrix(m1, &p->unk_34);
            CompMatrix((MATRIX *)&o->unk_04->unk_40, m1, m1);
            o->unk_40 = p->unk_54;
            o->unk_44 = p->unk_58;
            o->unk_48 = p->unk_5C;
            o->unk_4C = p->unk_60;
            o->unk_50 = p->unk_64;
            o->unk_54 = p->unk_68;
            o->unk_58 = p->unk_6C;
            o->unk_5C = p->unk_70;
            p->unk_44.vx = o->unk_10;
            p->unk_44.vy = o->unk_14;
            p->unk_44.vz = o->unk_18;
            TransMatrix(m2, &p->unk_44);
        }
        RotMatrix(&D_80080994, m2);
        CompMatrix(m1, m2, m1);
        SetRotMatrix(m1);
        SetTransMatrix(m1);
        while (e != NULL)
        {
            prim = p->unk_1C;
            q = (u8 *)&e->unk_01;
            p->unk_1C = prim + 1;
            a = ((volatile u8 *)q)[7];
            b = e->unk_09;
            p->unk_0C.w = e->unk_08;
            p->unk_10.w = e->unk_09;
            p->unk_14.w = e->unk_0A;
            p->unk_18.w = e->unk_0B;
            if (a + e->unk_0A >= 0x100)
            {
                p->unk_14.w = p->unk_14.w - 1;
            }
            if (b + e->unk_0B >= 0x100)
            {
                p->unk_18.w = p->unk_18.w - 1;
            }
            if (((e->unk_00 ^ p->unk_28) & 1) != 0)
            {
                x0 = -((s8)((volatile u8 *)q)[1] * 2);
                p->unk_84 = x0;
                p->unk_74 = x0;
                t0 = x0 - p->unk_14.h;
                p->unk_8C = t0;
                p->unk_7C = t0;
            }
            else
            {
                x1 = (s8)((volatile u8 *)q)[1] * 2;
                p->unk_84 = x1;
                p->unk_74 = x1;
                t1 = x1 + p->unk_14.h;
                p->unk_8C = t1;
                p->unk_7C = t1;
            }
            if (((e->unk_00 ^ p->unk_28) & 2) != 0)
            {
                x2 = -((s8)((volatile u8 *)q)[2] * 2);
                p->unk_7E = x2;
                p->unk_76 = x2;
                t2 = x2 - p->unk_18.h;
                p->unk_8E = t2;
                p->unk_86 = t2;
            }
            else
            {
                x3 = (s8)((volatile u8 *)q)[2] * 2;
                p->unk_7E = x3;
                p->unk_76 = x3;
                t3 = x3 + p->unk_18.h;
                p->unk_8E = t3;
                p->unk_86 = t3;
            }
            RotTransPers4((SVECTOR *)&p->unk_74, (SVECTOR *)&p->unk_7C,
                          (SVECTOR *)&p->unk_84, (SVECTOR *)&p->unk_8C,
                          &p->unk_9C.w, &p->unk_A4.w, &p->unk_AC.w,
                          &p->unk_B4.w, &p->unk_94, &p->unk_98);
            if ((((u16)(p->unk_9C.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_9C.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_A4.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_A4.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_AC.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_AC.h.y + 0x20) < 0x121) |
                 ((u16)(p->unk_B4.h.x + 0x20) < 0x181 &&
                  (u16)(p->unk_B4.h.y + 0x20) < 0x121)) != 0)
            {
                *(s32 *)&prim->x0 = p->unk_9C.w;
                *(s32 *)&prim->x1 = p->unk_A4.w;
                *(s32 *)&prim->x2 = p->unk_AC.w;
                *(s32 *)&prim->x3 = p->unk_B4.w;
                setlen(prim, 9);
                s->unk_14 &= 0x7FFF;
                t = p->unk_14.w + p->unk_0C.w;
                p->unk_14.w = t;
                if (t & 0x100)
                {
                    p->unk_14.w = t - 1;
                }
                t = p->unk_18.w + p->unk_10.w;
                p->unk_18.w = t;
                if (t & 0x100)
                {
                    p->unk_18.w = t - 1;
                }
                p->unk_18.w = p->unk_18.w << 8;
                p->unk_10.w = p->unk_10.w << 8;
                *(s32 *)&prim->u0 =
                    p->unk_10.w + p->unk_0C.w +
                    ((s->unk_12 != 0 ? s->unk_12 : e->unk_06) << 16);
                *(u16 *)&prim->u1 = p->unk_10.h + p->unk_14.h;
                if (s->unk_10 != 0)
                {
                    prim->tpage = s->unk_10 + (e->unk_04 & 0xFF9F);
                }
                else
                {
                    prim->tpage = e->unk_04;
                }
                *(u16 *)&prim->u2 = p->unk_18.h + p->unk_0C.h;
                *(u16 *)&prim->u3 = p->unk_18.h + p->unk_14.h;
                if (prim->x0 > prim->x3)
                {
                    prim->u1 = prim->u1 - 1;
                    prim->u3 = prim->u3 - 1;
                }
                if (prim->y0 > prim->y3)
                {
                    prim->v2 = prim->v2 - 1;
                    prim->v3 = prim->v3 - 1;
                }
                if (p->unk_34.vx >= 0x1800)
                {
                    prim->u1 = prim->u1 - 1;
                    prim->u3 = prim->u3 - 1;
                }
                if (p->unk_34.vy >= 0x1800)
                {
                    prim->v2 = prim->v2 - 1;
                    prim->v3 = prim->v3 - 1;
                }
                c = e->unk_01;
                s->unk_0C[3] = c;
                if (p->unk_28 & 8)
                {
                    if (p->unk_28 & 4)
                    {
                        s->unk_0C[3] = c | 2;
                    }
                    else
                    {
                        s->unk_0C[3] = c & 0xFD;
                    }
                }
                if (p->unk_28 & 0x10)
                {
                    s->unk_0C[3] = s->unk_0C[3] & 0xFE;
                }
                else
                {
                    s->unk_0C[3] = s->unk_0C[3] | 1;
                }
                *(u32 *)&prim->r0 = *(u32 *)s->unk_0C;
                AddPrim((void *)(p->unk_24 + p->unk_C4 * 4), prim);
            }
            else
            {
                s->unk_14 |= 0x8000;
            }
            if ((s8)e->unk_00 < 0)
            {
                break;
            }
            e++;
        }
        *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = p->unk_1C;
        if (f != NULL)
        {
            f(pl);
        }
        p->unk_1C = *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0);
        n = *(UnkNode **)(pl - 8);
        if (n == NULL)
        {
            break;
        }
        pl = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    PopMatrix();
    *(POLY_FT4 **)((u8 *)g->unk_00 + 0x8D0) = p->unk_1C;
    return 0;
}
