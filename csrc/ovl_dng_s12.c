#include "common.h"

void func_800478B8(u8 *);
s32 func_8004491C(UnkNode *, u32);
UnkNode *func_8003FD64(s32, void *);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
s32 func_800A56E0(s32);
s32 func_800BCB04(s32, s32, s16);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
void func_s12_80024850(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_s12_800249BC(u8 *p);
void func_s12_80024C40(u8 *p);
void func_s12_80024F40(u8 *p);
extern s32 D_8008346C;
extern u8 D_80045340[];
extern u8 D_80083498[];
extern u8 D_800DEC50[];
extern u8 D_800DEC70[];
extern u8 D_800DEE38[];

/* The bank's six-state cutscene director, a JUMP TABLE on *(s16 *)(p + 0xA)
   over cases 0..5 (jtbl_s12_80024008, 6 words = 6 cases + 0 pad, dispatch
   bound 6, 6 distinct targets). THIS FUNCTION OWNS THAT TABLE. The range
   check's `beqz' lands on the function's EPILOGUE, which is NOT one of the
   six entries, so there is no default among them and no `default:' label is
   written -- an explicit `default: break;' measured FREE at 524 words / 0
   differing, so the target does not constrain it either way.

   Before the switch, every state but 0 and 5 spawns eight 0x312 smoke nodes
   that run func_s12_80024850, jittered +-0xFF<<13 around the caller's vector
   and dressed from D_800DEC70 (odd index) or D_800DEC50 (even). State 0
   publishes the party record's projected position into b, seeds the +0x50
   timer at 8 and divides the three velocity words by it; state 1 advances b
   by its own velocity half and reloads the timer; state 2 spawns three 0x201
   nodes running func_s12_80024C40; state 3 spawns the 0x201 node that gets
   func_s12_80024F40 as its step and func_s12_800249BC as its renderer and
   seeds its four-segment trail; state 4 spawns one more 0x312; state 5 tears
   the actor down. 524 words, 0 differing.

   NO TWIN AT ANY RATIO -- dngtwin reports nothing at 0.60 for this stub.
   Written from the listing, from the four bodies below it in this same unit,
   and from csrc/ovl_dng_s63.c func_s63_8002401C for the
   `s = *(u8 **)p; o = *(u8 **)(s - 0x14); w = s - 0x20;' dispatcher skeleton
   and the 0x302-spawn dress block. func_80069EF8, func_80066460 and
   func_8006649C are rand, GetTPage and GetClut out of
   splat/common_syms.txt, so none of them needs a declaration.

   SIX FACTS, all priced from this body:

   * THE +0x72/+0x73 BYTES MUST BE SCALED WITH A MULTIPLY, NOT A SHIFT.
     `*(s8 *)(s + 0x72) * 0x40' is 0; `*(s8 *)(s + 0x72) << 6' is 525 words /
     20 aligned differences. The multiply keeps the `sign_extend' and expands
     to `lb' + `sll 6'; the shift reaches combine's `simplify_shift_const',
     which rewrites `(ashift (sign_extend (mem:QI)) 6)' as
     `lbu' + `sll 24' + `sra 18' -- three words for two, at both sites. So
     the lever "a plain-constant multiply by a power of two is the same tree
     as the shift" HOLDS ONLY FOR AN ALREADY-WIDE OPERAND; on a sign-extended
     byte the two spellings are different trees.
   * STATE 3's TRAIL LOOP NEEDS BOTH A WRITE CURSOR AND AN OFFSET LOCAL, IN
     THAT ORDER: `e = q + i * 8; j = (i - 1) * 8;' at the top of the body is
     0, the two assignments SWAPPED is 2 aligned differences, and folding the
     read into the write cursor (`*(u16 *)(q + (i - 1) * 8 + 0x4)', which
     combine_givs merges into one register with -4/-2/0 displacements) is
     134. The order matters because loop.c's `record_giv' PREPENDS to
     `bl->giv', so reduced-giv initialisers are emitted in REVERSE discovery
     order and the assignment written first lands LAST in the preheader.
   * THE ODD DRESS ARM WANTS +0x10 BEFORE +0xC in source order -- the other
     way is 6 aligned differences, every row the scheduler putting the
     0x404040 store ahead of the 0x20 materialisation and so freeing $v0
     where the target spends $v1.
   * CASE 0's INDIRECT RECORD MUST REUSE `q', NOT GET A LOCAL OF ITS OWN --
     a separate local is 3 aligned differences, because `q' is the allocno
     that already owns $s1 there.
   * CASES 2 AND 3 MUST NAME THEIR NODE PAYLOAD `q' -- the SAME local the
     first loop uses for `n->unk_0C' -- and not a local of their own. A fresh
     local for them is 82 aligned differences, because the hoisted
     step-function pointer then takes the register the target spends on the
     payload and every $s1/$s2 pair in both arms exchanges.
   * THE LOCAL DECLARATION ORDER IS MEASURED AND FREE ON THIS BODY, AND I HAD
     IT DOWN AS A LEVER UNTIL I RE-PRICED IT: `n' declared after `q' is 524
     words / 0 differing. It was worth 74 aligned differences at the
     intermediate state where cases 2 and 3 still had a payload local of
     their own, which is the brief's "worse alone, necessary in combination"
     shape read the other way round -- a fix that was load-bearing until the
     fault it was compensating for was removed. Do not preserve it.

   The body went 149 -> 75 -> 63 -> 32 -> 12 -> 2 -> 0 aligned differences;
   the step from 2 to 0 is the giv-order fact above. */
void func_s12_80024020(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    UnkNode *n;
    u8 *q;
    u8 *e;
    u8 *t;
    s32 i;
    s32 j;
    s32 r;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = *(u8 **)(s - 0x14);
    w = s - 0x20;
    if (*(s16 *)(p + 0xA) != 0 && *(s16 *)(p + 0xA) != 5)
    {
        for (i = 7; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                e = (u8 *)n + 0x20;
                q = (u8 *)n->unk_0C;
                n->unk_10 = (u32)func_s12_80024850;
                *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x1FF) - 0xFF) << 13);
                *(s32 *)((u8 *)n->unk_08 + 0x8) =
                    b->unk_08 - (((rand() & 0x1FF) - 0xFF) << 14) - 0x200000;
                *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
                *(u16 *)(q + 0x14) |= 0xC;
                if ((i & 1) != 0)
                {
                    *(s16 *)(q + 0x10) = 0x20;
                    *(s32 *)(q + 0xC) = 0x404040;
                    *(u8 **)(q + 0x0) = D_800DEC70;
                    *(s16 *)(e + 0x4A) = 0x10;
                }
                else
                {
                    *(s16 *)(q + 0x10) = 0x60;
                    *(u8 **)(q + 0x0) = D_800DEC50;
                    *(s32 *)(q + 0xC) = 0x282828;
                    *(s16 *)(e + 0x4A) = 0x8;
                }
                *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4);
                *(u8 *)(q + 0x4) = 0;
                *(u8 *)(q + 0x5) = 0;
                *(u8 **)(e + 0x0) = p;
                *(s16 *)(e + 0x48) = rand() & 3;
                *(s16 *)(e + 0x4C) = 0;
            }
        }
    }
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        t = *(u8 **)(s + 0x60);
        if (t == NULL)
        {
            *(u8 *)(s + 0x72) = *(u8 *)(o + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(o + 0x25);
        }
        else
        {
            q = *(u8 **)(t - 0x14);
            *(u8 *)(s + 0x72) = *(u8 *)(q + 0x24);
            *(u8 *)(s + 0x73) = *(u8 *)(q + 0x25);
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(w + 0xC) + 0x8), (UnkSlot *)*(u8 **)(w + 0xC), out, 0) ==
            0)
        {
            out[0] = out[1] = out[2] = 0;
        }
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(w + 0x8) + 0x2) + out[0];
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(w + 0x8) + 0x6) + out[1];
        *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(w + 0x8) + 0xA) + out[2];
        *(s16 *)(p + 0x50) = 8;
        *(s16 *)((u8 *)b + 0xE) = (*(s8 *)(s + 0x72) * 0x40) - (*(u16 *)((u8 *)b + 0x2) - 0x20);
        b->unk_0C /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x12) = (*(s8 *)(s + 0x73) * 0x40) - (*(u16 *)((u8 *)b + 0x6) - 0x20);
        b->unk_10 /= *(s16 *)(p + 0x50);
        *(s16 *)((u8 *)b + 0x16) =
            func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(*(u8 **)(w + 0x8) + 0xA) - 0x30) -
            *(u16 *)((u8 *)b + 0xA);
        b->unk_14 /= *(s16 *)(p + 0x50);
        func_800A56E0(0x300);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 1:
        b->unk_00 += b->unk_0C;
        b->unk_04 += b->unk_10;
        b->unk_08 += b->unk_14;
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0x50) = 0xA;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) == 0)
            {
                *(s16 *)(p + 0xA) = 5;
            }
            else
            {
                for (i = 8; i >= 0; i -= 4)
                {
                    n = func_8003FD64(0x201, D_80083498);
                    if (n != NULL)
                    {
                        q = (u8 *)n + 0x20;
                        n->unk_10 = (u32)func_s12_80024C40;
                        *(s16 *)(q + 0x48) = i;
                        *(s16 *)(q + 0x4) = *(u16 *)((u8 *)b + 0x2);
                        *(s16 *)(q + 0x6) = *(u16 *)((u8 *)b + 0x6);
                        *(s16 *)(q + 0x8) = *(u16 *)((u8 *)b + 0xA);
                        *(s16 *)(q + 0x46) = 3;
                        *(u8 **)((u8 *)n + 0x20) = p;
                        *(s16 *)(q + 0x4A) = 0;
                    }
                }
                *(s16 *)(p + 0x50) = 8;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            }
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x50) < 6)
        {
            n = func_8003FD64(0x201, D_80083498);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                n->unk_10 = (u32)func_s12_80024F40;
                func_8004491C(n, (u32)func_s12_800249BC);
                *(s16 *)(q + 0x4) = *(u16 *)((u8 *)b + 0x2) + (rand() & 0x3F) - 0x20;
                *(s16 *)(q + 0x6) = *(u16 *)((u8 *)b + 0x6) + (rand() & 0x3F) - 0x20;
                *(s16 *)(q + 0x8) = *(u16 *)((u8 *)b + 0xA);
                *(s16 *)(q + 0x40) = GetTPage(0, 3, 0x2C0, 0x100);
                *(s32 *)(q + 0x34) = 0x707070;
                if ((*(u16 *)(p + 0x50) & 1) != 0)
                {
                    *(s16 *)(q + 0x38) = 0xA0;
                    *(s16 *)(q + 0x3A) = 0;
                }
                else
                {
                    *(s16 *)(q + 0x38) = 0x80;
                    *(s16 *)(q + 0x3A) = 0x20;
                }
                *(s16 *)(q + 0x3C) = 0xBF;
                *(s16 *)(q + 0x3E) = 0x1F;
                *(s16 *)(q + 0x42) = GetClut(0xA0, 0x1F7);
                *(s16 *)(q + 0x46) = 6;
                for (i = 1; i < 5; i++)
                {
                    e = q + i * 8;
                    j = (i - 1) * 8;
                    *(s16 *)(e + 0x4) = *(u16 *)(q + j + 0x4) + rand() % 159 - 0x50;
                    *(s16 *)(e + 0x6) = *(u16 *)(q + j + 0x6) + rand() % 159 - 0x50;
                    r = (rand() & 0x1F) + 0x14;
                    *(s16 *)(e + 0x8) = *(u16 *)(q + j + 0x8) - r;
                }
                *(u8 **)(q + 0x0) = p;
                *(s16 *)(q + 0x4A) = 0;
            }
        }
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            if (*(s32 *)(s + 0x60) != 0)
            {
                func_8009CE1C(*(u8 **)(s + 0x60), 0xA, *(u8 *)(p + 0x9), 4, *(s16 *)(s + 0x2A),
                              (u32)s, 2);
            }
            *(s16 *)(p + 0x50) = 0xA;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            e = (u8 *)n + 0x20;
            q = (u8 *)n->unk_0C;
            n->unk_10 = (u32)func_s12_80024850;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x1FF) - 0xFF) << 13);
            *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x1FF) - 0xFF) << 13);
            *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 - ((rand() & 0x1FF) << 12) - 0x200000;
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0xC00;
            *(s16 *)(q + 0x10) = 0x20;
            *(u8 **)(q + 0x0) = D_800DEE38;
            *(s32 *)(q + 0xC) = 0x808080;
            *(u16 *)(q + 0x14) |= 0xC;
            *(s16 *)(e + 0x4A) = 0x8;
            *(s32 *)(q + 0x8) = *(s32 *)(*(u8 **)(q + 0x0) + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)(e + 0x48) = rand() & 3;
            *(s16 *)(e + 0x4C) = 0;
        }
        *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
        if (*(s16 *)(p + 0x50) <= 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 5:
        if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
        }
        else
        {
            D_8008346C = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The 0x312 smoke node's per-frame step, installed as the +0x10 step by
   func_s12_80024020's spawn loops and by its case 4, so the four-argument
   signature is fixed by those call sites. Sets bit 0x8000 in the +0x52
   halfword of the record at the actor's +0x0, decays the caller's +0x14
   velocity component by 0x20000 unless it has already reached zero, advances
   the vector by its own velocity half, then runs a two-case compare chain on
   +0x4C: state 0 counts the +0x48 timer down and re-registers the renderable
   through func_8004491C when it expires, state 1 hands the renderable to
   func_800478B8, clears its +0x4/+0x5 once +0x14 reports 0x6000, and fades
   its +0xC/+0xD/+0xE colour bytes by the +0x4A step until the red byte would
   pass the actor's own level, at which point it clears the colour word and
   retires. 91 words, 0 differing. 2-member family with
   func_s09_80024C84 (dngfam); I did not touch the sibling.

   NO TWIN AT ANY RATIO. Landed off the IN-UNIT route plus one donor row:
   every idiom but the fade came from func_s12_80024F40 below (the +0x52 flag,
   the retire pair, the two-case compare chain), and the
   `b->unk_00 += b->unk_0C' triple from csrc/ovl_dng_s23.c
   func_s23_80024024, which the callee-set overlap on
   D_80045340 + D_800814A0 + func_8004491C + func_800478B8 named.

   FOUR FACTS, all priced from this body:

   * THE FADE/RETIRE GATE'S ARMS WERE THE WRONG WAY ROUND and that was 17 of
     the 19 aligned differences this body started at -- one character, and the
     residue was the whole subu block emitted in the wrong position with an
     inverted branch, exactly as the brief describes.
   * THE COMPARE PUTS THE STRUCT FIELD ON THE LEFT:
     `*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A)' is 0 and
     `*(s16 *)(p + 0x4A) >= *(u8 *)(c + 0xC)' is 2 aligned differences, a pure
     transposition of the two loads, because operand 0 of a comparison is
     expanded first.
   * THE +0x14 DECAY NEEDS A LOCAL: read into `v', test, decay, store back
     unconditionally. Written in place as
     `if (b->unk_14 != 0) { b->unk_14 -= 0x20000; }' it is 5 aligned
     differences, because the target's store at the join is unconditional.
   * THE COLOUR FADE IS ONE CHAINED ASSIGNMENT, not three statements -- three
     statements are 92 words / 3 aligned differences. Chained, the stores come
     out +0xE, +0xD, +0xC, which is a chained assignment's right-to-left
     order. */
void func_s12_80024850(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 v;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    v = b->unk_14;
    if (v != 0)
    {
        v -= 0x20000;
    }
    b->unk_14 = v;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        func_800478B8(c);
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
        }
        if (*(u8 *)(c + 0xC) <= *(s16 *)(p + 0x4A))
        {
            *(s32 *)(c + 0xC) = 0;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(u8 *)(c + 0xC) = *(u8 *)(c + 0xD) = *(u8 *)(c + 0xE) -= *(u8 *)(p + 0x4A);
        }
        break;
    }
}

/* The trail renderer func_s12_80024020's state 3 registers through
   func_8004491C: walk the tag chain, project the five SVECTORs at the
   record's +0x4 (stride 8) through RotTransPers into a five-element DVECTOR
   array, widen each of the four segments by half the vertical span of the
   first and last projections divided by the +0x46 width, and hang four
   semi-transparent POLY_FT4 ribbon quads per node on the OT row given by the
   AVERAGE of the five depths when that is under 0x1E0. The prim slot is
   0x28 bytes of D_80083160's +0x8D0 cursor, which is exactly a POLY_FT4.
   161 words, 0 differing.

   TWIN: csrc/ovl_dng_s70.c func_s70_80024440 (dngtwin 0.693, 139 words), the
   same +0x8D0-cursor POLY_FT4 tag-chain walk. ALL THREE OF ITS PRICED FACTS
   CARRIED VERBATIM and are not re-priced here: `DVECTOR xy[]' rather than
   `s32' read with plain casts, `(long *)&xy[i]' rather than a cursor local,
   and `t->u1 = t->u3 = t->u0 + *(u8 *)(q + 0x3C)' rather than a local holding
   the first byte. My deltas from it are five projections instead of two, the
   averaged OT row, and the four-segment ribbon over xy[i]/xy[i+1].

   ONE FACT, priced from this body and worth 21 aligned differences:

   * THE TWO LOOPS SHARE ONE INDEX VARIABLE. With a separate `i' and `j' the
     body is 161 words / 21 aligned differences, every row an $s1/$s2/$s3
     permutation and not one other word wrong; with one `i' for both it is 0.
     `-da' names it before the compile: z is 18 refs over 33 insns, so
     `allocno_compare' scores it 4*18/33 = 2.18 against i's 11/14 = 2.36 and
     i takes $s2 first. Merging the two counters pushes the shared index to
     ~28 refs over ~80 insns, it drops below z, and the permutation resolves.
     THE TELL IS IN THE LISTING: the target's two loop counters are BOTH $s3,
     which two live-disjoint variables are not obliged to be.
     Declaration order was measured and FREE -- 21 aligned differences with
     `z' declared before `i' and with `i' before `z' -- so this is a priority,
     not an allocno-number tie. */
s32 func_s12_800249BC(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    s32 z;
    s32 i;
    DVECTOR xy[5];
    s32 w;

    while (1)
    {
        q = p;
        z = 0;
        for (i = 4; i >= 0; i--)
        {
            z += RotTransPers((SVECTOR *)(q + 0x4 + i * 8), (long *)&xy[i], (long *)&w, (long *)&w) - 8;
        }
        w = __builtin_abs(xy[0].vy - xy[4].vy) / (*(s16 *)(q + 0x46) + 2);
        z /= 5;
        if ((u32)z < 0x1E0)
        {
            for (i = 0; i < 4; i++)
            {
                t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                *(u32 *)&t->r0 = *(u32 *)(q + 0x34);
                SetPolyFT4(t);
                SetSemiTrans((u8 *)t, 1);
                t->tpage = *(u16 *)(q + 0x40);
                t->clut = *(u16 *)(q + 0x42);
                t->x0 = xy[i].vx - w;
                t->x1 = xy[i + 1].vx - w;
                t->x2 = xy[i].vx + w;
                t->x3 = xy[i + 1].vx + w;
                t->y0 = t->y2 = xy[i].vy;
                t->y1 = t->y3 = xy[i + 1].vy;
                t->u0 = t->u2 = *(u8 *)(q + 0x38);
                t->u1 = t->u3 = t->u0 + *(u8 *)(q + 0x3C);
                t->v2 = t->v3 = *(u8 *)(q + 0x3A);
                t->v0 = t->v1 = t->v2 + *(u8 *)(q + 0x3E);
                addPrim((u32 *)(g->unk_00 + 0xB0) + z, t);
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

/* The 0x201 node's per-frame step that func_s12_80024020's state 2 installs.
   Sets bit 0x8000 in the +0x52 halfword of the record at the actor's +0x0,
   then runs a three-case compare chain on +0x4A. State 0 waits out the +0x48
   timer, then re-registers the renderable, seeds the 0xC0C0C0 colour word,
   the GetTPage/GetClut pair into +0x40/+0x42, the 0x1F pair into +0x3C/+0x3E,
   0x80 into +0x38 and -0x1C into +0x44, and stacks the four trail SVECTORs
   at +0x4 (stride 8) straight down from the first by k * +0x44. State 1
   re-jitters the trail's x and y by rand() % 95 - 0x30 while the +0x48
   counter is under 6, re-stacks the z column, advances the counter, pulls
   +0x44 down by twice it, republishes the +0x38/+0x3A UV pair from its low
   bits, and fades the colour word by 0x101010 a frame until the counter
   passes 8. State 2 tears the actor down. 192 words, 0 differing.

   NO TWIN AT ANY RATIO; written from the listing. func_80066460 and
   func_8006649C are GetTPage and GetClut out of splat/common_syms.txt and
   need no declaration, and the two-case-chain and retire idioms came from
   func_s12_80024F40 below, which landed first.

   THREE FACTS, all priced from this body:

   * STATE 1's GUARDED JITTER NEEDS AN EXPLICIT EMPTY `else' LOOP. gcc 2.7.2
     HAS NO UNSWITCHING PASS -- there is no `unswitch' in
     scratch/gcc-src/cdk-gcc-b18/loop.c -- so
     `for (k...) { if (inv) {...} }' keeps the invariant test INSIDE the loop
     and comes out 186 words / 9 aligned differences, six words SHORT, where
     the target evaluates the test once and spins a second, body-less copy of
     the loop on the false side. Written as
     `if (inv) { for (k...) {...} } else { for (k...) { } }' it is 192 words
     and 0. I am calling that MACRO RESIDUE rather than a device: a loop whose
     body is empty in one configuration is what a configuration macro
     collapses to, and nothing else reaches the word count.
   * THE +0x48 BUMP MUST GO THROUGH A LOCAL THAT THE +0x44 STATEMENT THEN
     DOUBLES: `c = *(u16 *)(p + 0x48) + 1; *(s16 *)(p + 0x48) = c; ...
     - c * 2;' is 0, and re-reading the field as
     `- (*(u16 *)(p + 0x48) * 2)' is 16 aligned differences. A `(u16)'
     re-read cannot use the truncating `sh' store's forwarded SImode value
     and reloads with `lhu', which hands the forwarded value to the +0x38
     statement instead and reverses the whole tail's schedule.
   * A SIGNED RE-READ IS WORSE STILL: `- (*(s16 *)(p + 0x48) * 2)' is 193
     words / 23 aligned differences, because the sign extension of the
     just-stored halfword costs the `sll 16'/`sra 15' pair on top of the
     reload. `c * 2' on the local is a plain `sll 1'.

   The body measured 189 words / 40 aligned differences at first compile; the
   three fixes above are independent and account for all of it. */
void func_s12_80024C40(u8 *p)
{
    s32 k;
    s32 c;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    switch (*(s16 *)(p + 0x4A))
    {
    case 0:
        *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            func_8004491C((UnkNode *)(p - 0x20), (u32)func_s12_800249BC);
            *(s32 *)(p + 0x34) = 0xC0C0C0;
            *(s16 *)(p + 0x40) = GetTPage(0, 1, 0x2C0, 0x100);
            *(s16 *)(p + 0x42) = GetClut(0xA0, 0x1F7);
            *(s16 *)(p + 0x3C) = *(s16 *)(p + 0x3E) = 0x1F;
            *(s16 *)(p + 0x38) = 0x80;
            *(s16 *)(p + 0x48) = 0;
            *(s16 *)(p + 0x3A) = 0;
            *(s16 *)(p + 0x44) = -0x1C;
            for (k = 1; k < 5; k++)
            {
                *(s16 *)(p + k * 8 + 0x4) = *(u16 *)(p + 0x4);
                *(s16 *)(p + k * 8 + 0x6) = *(u16 *)(p + 0x6);
                *(s16 *)(p + k * 8 + 0x8) = *(u16 *)(p + 0x8) + *(s16 *)(p + 0x44) * k;
            }
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x48) < 6)
        {
            for (k = 1; k < 5; k++)
            {
                *(s16 *)(p + k * 8 + 0x4) = *(u16 *)(p + 0x4) + rand() % 95 - 0x30;
                *(s16 *)(p + k * 8 + 0x6) = *(u16 *)(p + 0x6) + rand() % 95 - 0x30;
            }
        }
        else
        {
            for (k = 1; k < 5; k++)
            {
            }
        }
        for (k = 1; k < 5; k++)
        {
            *(s16 *)(p + k * 8 + 0x8) = *(u16 *)(p + 0x8) + *(s16 *)(p + 0x44) * k;
        }
        c = *(u16 *)(p + 0x48) + 1;
        *(s16 *)(p + 0x48) = c;
        *(s16 *)(p + 0x44) = *(u16 *)(p + 0x44) - c * 2;
        *(s16 *)(p + 0x38) = ((*(u16 *)(p + 0x48) & 3) << 5) + 0x80;
        *(s16 *)(p + 0x3A) = ((s16) * (u16 *)(p + 0x48) >> 2) << 5;
        if (*(s16 *)(p + 0x48) >= 8)
        {
            *(s32 *)(p + 0x34) = 0;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
        else
        {
            *(s32 *)(p + 0x34) -= 0x101010;
        }
        break;
    case 2:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* The 0x201 trail node's per-frame step that func_s12_80024020's state 3
   installs alongside func_s12_800249BC as its renderer. Sets bit 0x8000 in
   the +0x52 halfword of the record at the actor's +0x0, re-rolls the CLUT
   into +0x42 from a random 4-bit page, and runs a two-case compare chain on
   +0x4A: state 0 fades the three colour bytes at +0x34 by 0x30 each a frame
   as one word subtraction and advances the state when the red byte would
   pass 0x30, state 1 retires the actor. 52 words, 0 differing, FIRST COMPILE.

   NO TWIN WORTH READING -- dngtwin's best is func_s23_80024370 at 0.632 and
   it was not used. This is the IN-UNIT route's first body: the two idioms it
   needed came from the callee-set row csrc/ovl_dng_s63.c func_s63_80024648
   (`*(u16 *)(*(u8 **)p + 0x52) |= 0x8000' and the
   `*(u16 *)(p - 0x2) |= 0x8000; D_800814A0 |= 0x8000' retire pair), and
   splat/common_syms.txt resolved func_8006649C to GetClut and func_80069EF8
   to rand, so neither needs a declaration. Every other body in this unit was
   then built out of this one.

   It landed first compile, so I priced the four spellings that looked
   load-bearing. ONLY ONE IS:

   * THE TWO-ARM GATE MUST BE A `switch', NOT AN if/else-if CHAIN -- the
     chain is 50 words / 9 aligned differences. gcc emits the switch's
     range-ordered `beqz'/`beq' pair with the `lui %hi(D_800814A0)' hoisted
     into the second branch's delay slot; the chain re-loads +0x4A for the
     second test.

   MEASURED AND FREE at 52 words / 0 differing, so none of these is the reason
   it lands:
     the fade written `+= 0xFFCFCFD0' instead of `-= 0x303030'
     the +0x34 clear and the +0x4A bump in the other order
     the gate written `*(u8 *)(p + 0x34) <= 0x30' instead of `< 0x31' */
void func_s12_80024F40(u8 *p)
{
    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    *(s16 *)(p + 0x42) = GetClut((rand() & 0xF) << 4, 0x1F8);
    switch (*(s16 *)(p + 0x4A))
    {
    case 0:
        if (*(u8 *)(p + 0x34) < 0x31)
        {
            *(s32 *)(p + 0x34) = 0;
            *(s16 *)(p + 0x4A) = *(u16 *)(p + 0x4A) + 1;
        }
        else
        {
            *(s32 *)(p + 0x34) -= 0x303030;
        }
        break;
    case 1:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}
