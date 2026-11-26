#include "common.h"

s32 rand(void);

typedef union UnkDngS14Word
{
    s32 w;
    s16 h[2];
} UnkDngS14Word;
typedef struct UnkDngS14LineSP
{
    u8 pad_00[0x18];
    u8 *unk_18;
    u8 pad_1C[0x48];
    u16 unk_64;
    u16 unk_66;
    u16 unk_68;
    u8 pad_6A[0x2];
    u16 unk_6C;
    u16 unk_6E;
    u16 unk_70;
    u8 pad_72[0x12];
    s32 unk_84;
    s32 unk_88;
    u8 pad_8C[0x28];
    s32 unk_B4;
    u8 pad_B8[0x20];
    u16 unk_D8;
    u16 unk_DA;
    u16 unk_DC;
    u16 unk_DE;
    u8 pad_E0[0x14];
    s32 unk_F4;
    s32 unk_F8;
} UnkDngS14LineSP;
s16 func_800A2CB8(u8 *, u8 *);
s16 func_800A41F0(u8 *);
u16 func_800A6D30();
s32 func_800C87C4(u8 *, s16, s16);
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_s14_8002418C(u8 *p, UnkVec *b, u8 *c);
extern u8 D_800DEDB0[];
extern u8 D_80045340[];
extern u8 D_800E3D68;
extern u8 *D_800E3D7C;

/* 75 words, 0 differing. Walks the resident actor ring at D_800E3D7C, and
   for each actor the cursor admits whose grid cell is within 2 of (x, y)
   calls the 0x800C87C4 report with a 0x10 chance -- 0xFF when D_800E3D68
   is saturated -- and a rank of (d >> 2) + 0x10 plus two random bits. */
void func_800478B8(u8 *);
s32 func_800A45D8w(s32, s32, s32) __asm__("func_800A45D8");
s16 func_800BCAD0(u8 *);
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
s32 func_s14_800246D8_2(u8 *, u8 *) __asm__("func_s14_800246D8");
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern u8 D_800DE9D0[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u32 D_800814A0;
extern s32 D_8008346C;

void func_s14_80024060(s16 x, s16 y, u8 *c, u8 d)
{
    u8 *p;
    u8 *e;
    s32 t;
    s32 t2;
    s16 chance;
    s16 rank;

    p = D_800E3D7C;
    do
    {
        if ((s16)func_800A2CB8(c, p) != 0)
        {
            e = *(u8 **)(p - 0x14);
            t = e[0x24] - x;
            if (t < 0)
            {
                t = -t;
            }
            if (t < 3)
            {
                t2 = e[0x25] - y;
                if (t2 < 0)
                {
                    t2 = -t2;
                }
                if (t2 < 3 && (s16)func_800A41F0(p) != 0)
                {
                    rank = (d >> 2) + 0x10 + (func_800A6D30() & 3);
                    chance = 0x10;
                    if (D_800E3D68 == 0xFF)
                    {
                        chance = 0xFF;
                    }
                    func_800C87C4(p, chance, rank);
                }
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != D_800E3D7C);
}

/* The marker's per-frame step, installed as the +0x10 handler on the 0x212
   node: bump the +0x14 hit counter on the node at the owner's +0x0, raise the
   renderable's +0x1C/+0x1E scale by 0x400 and fade its three colour bytes by
   4, and when the renderable's +0x14 reports 0x6000 either retire the owning
   actor (+0x20 bit 0x8000 and the same bit in D_800814A0) or, the first time
   round, re-dress it from D_800DEDB0 with mode 4 and set the +0x4 latch.
   52 words, 0 differing, first compile.

   TWIN: the landed func_s15_8002418C in csrc/ovl_dng_s15.c (dngtwin 0.679,
   54 words). Its one big stated fact HOLDS unmeasured here because it is the
   order this body was written in from the start: the whole +0x1E/+0x1C
   read-modify-write -- load, add and BOTH stores -- precedes the
   +0xE/+0xD/+0xC byte block. Its `(u8)a == 0' retirement gate and its
   `b + 0xA' step do not exist in this copy; what replaces them is the +0x4
   one-shot latch, which the s15 body has no analogue of. */
void func_s14_8002418C(u8 *p, UnkVec *b, u8 *c)
{
    s32 a;

    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x400;
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
    a = c[0xE] - 4;
    c[0xE] = a;
    c[0xD] = a;
    c[0xC] = a;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        if (p[0x4] != 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            func_8003DB94(c, D_800DEDB0, 4);
            p[0x4] = 1;
        }
    }
}

/* Spawn the 0x212 node that animates the hit flash at q: white renderable,
   the 0x7E0B sprite, the +0x10/+0x14 flag bits and a position jittered down
   by rand() % 0x10 under q's own. 70 words, 0 differing.

   THE ONE SPELLING FACT: `*(u8 **)((u8 *)n + 0x20) = v;' must be the FIRST
   statement inside the gate. Written third, after the +0x10 step pointer and
   the `m = n->unk_0C' load -- which is the order every landed sibling of this
   shape uses -- the two parameters exchange $s2 and $s3 and it is 9 aligned
   differences; nothing else in the body moves.

   The FIRST rand() call has its result discarded: it is a real call that the
   target makes and gcc cannot delete, so it is written as a bare `rand();'.
   Read as macro residue -- the x and y jitters collapsed to nothing and only
   the z one survived. */
void func_s14_8002425C(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = v;
        n->unk_10 = (u32)func_s14_8002418C;
        m = (u8 *)n->unk_0C;
        *((u8 *)n + 0x24) = 0;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DEDB0, 0);
        *(s16 *)(m + 0x1E) = 0x1000;
        *(s16 *)(m + 0x1C) = 0x1000;
        *(s16 *)(m + 0x12) = 0x7E0B;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x10C;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        rand();
        *(s16 *)(m + 0x2) = *(u16 *)(q + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(q + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(q + 0xA) - 0x40 - rand() % 0x10;
    }
}

/* The flash frame's per-frame step, the handler func_s14_80024428 installs:
   bump the +0x14 counter on the node at the owner's +0x0, republish the
   renderable through func_800478B8, and count the node's own +0x4 frame
   counter up -- respawning the hit flash through func_s14_8002425C on frame
   0xA -- then retire the owning actor when the renderable's +0x14 reports
   0x6000. 45 words, 0 differing, first compile.

   TWIN: the landed func_s13_8002418C in csrc/ovl_dng_s13.c (dngtwin 0.741,
   36 words). What transferred: the `*(s16 *)(*(u8 **)p + 0x14) = ... + 1'
   counter bump through the double indirection, the one-argument
   func_800478B8 call, and the whole 0x6000 / +0x20 / D_800814A0 retirement
   tail verbatim. What is new here is the +0x4 counter and its respawn, which
   is the s13 body's +0x1E/+0x1C scale step replaced.

   The +0x4 counter is the in-place flag-update idiom: the compare re-reads
   `*(s16 *)(p + 0x4)' rather than carrying the stored value in a local,
   which is where the target's `sll 16'/`sra 16' pair after the `sh' comes
   from. */
void func_s14_80024374(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    func_800478B8(c);
    *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
    if (*(s16 *)(p + 0x4) == 0xA)
    {
        func_s14_8002425C(*(u8 **)p, (u8 *)b);
    }
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The flash spawner. With r == 0 it allocates ONE 0x212 node at b's position
   dressed from D_800DE9D0 at 0x80 white, 0x1000/0x3000 scale and the 0x7E0B
   sprite; otherwise it walks the eight-way ring D_8006CCD8/D_8006CCE8 and for
   each direction whose f[i] slot is still clear converts b's position to the
   grid, offsets it by the direction times r, converts back to the 0x20-centred
   tile coordinate and asks func_800A45D8 whether that cell is blocked --
   marking f[i] when it is, and otherwise spawning the same node there at 0x40
   white and 0x1800 scale with func_800BCAD0 supplying its final z. Both
   variants install func_s14_80024374 as the +0x10 step and the caller's record
   at +0x20. 172 words, 0 differing.

   NO TWIN AT ANY RATIO. The route in was this unit's own two landed bodies:
   func_s14_80024374 (landed an hour earlier, immediately above) is the step
   this installs, so its reads name these stores' offsets, and
   func_s14_8002425C gave the 0x212 dress skeleton. The callee-set
   intersection then confirmed csrc/ovl_dng_s13.c's func_s13_80024628 (6 of 9
   symbols, the only function at that score) as the structural donor for the
   spawn block; nothing of its arithmetic was needed.

   FACTS MEASURED ON THIS BODY, all re-priced FROM A BODY AT 0 (aligned
   differences at 172 words unless noted):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `s16 r' with the multiplier spelled `(u16)r'             |  172  |   0  |
   |   ... plain `r' (so the s16 parameter sign-extends)      |  173  |   7  |
   | `(u16)D_8006CCD8[i]', an unsigned halfword read          |  172  |   0  |
   |   ... `D_8006CCD8[i]' off the `s16' declaration          |  172  |  11  |
   | the grid value and the scaled value as SEPARATE          |  172  |   0  |
   |   statements, both grid values before either scaling     |       |      |
   |   ... each axis folded into one expression               |  172  |  11  |
   | the two scale stores as +0x1C then +0x1E                 |  172  |   0  |
   |   ... +0x1E first (the order func_s14_8002425C uses)     |  172  |   8  |
   | `*(s16 *)(m + 0xA)' stored from b AND AGAIN from         |  172  |   0  |
   |   func_800BCAD0's result                                 |       |      |
   |   ... only the call's result stored                      |  170  |   9  |

   THE PARAMETER IS `s16' AND THE MULTIPLIER IS `(u16)r', AND THAT PAIR IS THE
   WHOLE FIGHT. The target copies the RAW incoming $a2 to $s7 and then
   truncates $a2 in place for the gate (`addu $s7,$a2,$zero' / `sll
   $a2,$a2,16'), which no 32-bit parameter can produce: with `s32 r' and
   `if ((s16)r == 0)' the body is 172 words and 2 rows, the `sll' reading $s7
   instead, because one pseudo serves both uses. An `s16' parameter is the
   only declaration that makes assign_parms create TWO pseudos -- a raw
   SImode temp from the hard register and the converted parm -- and `(u16)r'
   is what lets the multiply use the raw one, so the converted parm's `sra'
   dies. `s32 r'/2, `u32 r'/2, `u16 r'/2 and `s16 r' with a plain multiplier
   /173-7 were all measured.

   NOT MEASURABLE: `(u16)D_8006CCE8[i]' against
   `*(u16 *)(i * 2 + (s32)D_8006CCE8)' -- both 172/0 and the emitted words are
   identical, so the index-first `addu' is not evidence of the cast form here.
   MEASURED AND FREE, so neither is why this lands: `g = f + i' as a pointer
   local against `f[i]' inline at both sites (172/0 both ways); and the +0x20
   record store written FIRST inside the gate instead of third, which the
   landed func_s14_8002425C above prices at 9 differences -- that fact is
   simply free on this body, in both arms. */
void func_s14_80024428(u8 *p, UnkVec *b, s16 r, u8 *f)
{
    UnkNode *n;
    u8 *m;
    u8 *g;
    s32 i;
    s32 sx;
    s32 sy;
    s16 gx;
    s16 gy;

    if (r == 0)
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_s14_80024374;
            m = (u8 *)n->unk_0C;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)((u8 *)n + 0x24) = 0;
            m[0xE] = 0x80;
            m[0xD] = 0x80;
            m[0xC] = 0x80;
            func_8003DB94(m, D_800DE9D0, 0);
            *(s16 *)(m + 0x1C) = 0x1000;
            *(s16 *)(m + 0x1E) = 0x3000;
            *(s16 *)(m + 0x12) = 0x7E0B;
            *(u16 *)(m + 0x10) |= 0x20;
            *(u16 *)(m + 0x14) |= 0x10C;
            func_8004491C(n, (u32)D_80045340);
            m = (u8 *)n->unk_08;
            *(s16 *)(m + 0x2) = *(u16 *)((u8 *)b + 0x2);
            *(s16 *)(m + 0x6) = *(u16 *)((u8 *)b + 0x6);
            *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA);
        }
    }
    else
    {
        for (i = 0; i < 8; i++)
        {
            g = f + i;
            if (*g != 0)
            {
                continue;
            }
            gx = *(s16 *)((u8 *)b + 0x2) / 0x40 + (u16)D_8006CCD8[i] * (u16)r;
            gy = *(s16 *)((u8 *)b + 0x6) / 0x40 + (u16)D_8006CCE8[i] * (u16)r;
            sx = (gx << 6) + 0x20;
            sy = (gy << 6) + 0x20;
            if ((s16)func_800A45D8w(sx & 0xFFE0, sy & 0xFFE0, *(s16 *)((u8 *)b + 0xA)) != 0)
            {
                *g = 1;
                continue;
            }
            n = func_8003FC64(0x212);
            if (n == NULL)
            {
                continue;
            }
            n->unk_10 = (u32)func_s14_80024374;
            m = (u8 *)n->unk_0C;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)((u8 *)n + 0x24) = 0;
            m[0xE] = 0x40;
            m[0xD] = 0x40;
            m[0xC] = 0x40;
            func_8003DB94(m, D_800DE9D0, 0);
            *(s16 *)(m + 0x1C) = 0x1000;
            *(s16 *)(m + 0x1E) = 0x1800;
            *(s16 *)(m + 0x12) = 0x7E0B;
            *(u16 *)(m + 0x10) |= 0x20;
            *(u16 *)(m + 0x14) |= 0x10C;
            func_8004491C(n, (u32)D_80045340);
            m = (u8 *)n->unk_08;
            *(s16 *)(m + 0x2) = sx;
            *(s16 *)(m + 0x6) = sy;
            *(s16 *)(m + 0xA) = *(u16 *)((u8 *)b + 0xA);
            *(s16 *)(m + 0xA) = func_800BCAD0(m);
        }
    }
}

/* The beam line: one semi-transparent LINE_G2 from the payload's +0x22/+0x26
   /+0x2A origin to its +0x3A/+0x3E/+0x42 endpoint, interpolated by the +0x10
   phase and coloured from a 15-entry table, projected through the 0x1F800000
   scratchpad and chained into D_80083160's +0x8D0 primitive ring with the
   addPrim macro. 259 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s14_80024008, 15 entries: cases 0..6 share the
   forward arm, case 7 is the flat middle, cases 8..14 the reverse arm.

   FOUR SPELLING FACTS, each re-measured on this body, all 259 words:

   THE SWITCH INDEX HAS TO BE AN INT-TYPED TREE. `switch (*(s16 *)(p + 0x10))'
   with cases 1..15 does the minval subtraction in HImode and costs two words
   (`sll 16' / `sra 16' after the `addiu -1'): gcc 2.7.2's
   c_expand_start_case runs get_unwidened over the promoted s16 and strips the
   promotion back off, so index_type stays `short'. Subtracting in the source
   and numbering the cases 0..14 keeps it `int' and gives the target's
   `lh' / `addiu -1' / `sltiu 0xF'. 260 words, 215 differing spelled the
   other way.

   THE INTERPOLATOR IS A 16.16 UNION, AND THAT IS THE `andi 0xFFFF' / `or' /
   `and 0xFFFF0000' TRIPLE. `u.h[1] = delta; u.h[0] = 0;' is two
   store_bit_field inserts into one register-resident word -- bitpos 16 then
   bitpos 0 -- and the first axis collapses to a bare `sll 16' only because
   the previous value is undefined there. Without the union the triple is
   simply absent: 255 words, 139 differing. The shift and the multiply must
   then be SEPARATE statements (`u.w >>= 3; u.w *= k;'): as one expression the
   shift lands in a different register from `u.w' and it is 107 differing.

   THE TWO COLOUR MULTIPLIERS ARE VARIABLES. `(st + 1) * 0xF' synthesises at
   expand time (`sll 4' / `subu'); `c = 0xF; (st + 1) * c' gives the target's
   `addiu $v0, $zero, 0xF' and `mult $v1, $v0', and TWO separate locals are
   needed because cse has to keep the 0xF register alive for the reverse
   arm's `0xF - st'. Read as macro residue.

   THE THREE COLOURS ARE COMPUTED INTO LOCALS BEFORE ANY OF THEM IS STORED,
   so the single `lh 0x10($t0)' serves both multiplies and the green ramp; a
   store to the primitive invalidates cse's memory table and each later read
   reloads. And in the forward arm `l->g1 = 0x40 - g;' comes BEFORE the three
   0x40 stores: after them, 0x40 is materialised twice and the function is
   260 words / 37 differing. That one statement move is the last 37.

   AND THE TWO SCRATCHPAD ARGUMENT ADDRESSES ARE `(u32)sp | 0x64', NOT THE
   INTEGER CONSTANT. Written `(SVECTOR *)0x1F800064' the four addresses each
   get their own `lui' instead of the target's `ori $aN, $s3, imm' off the
   pointer local: 264 words, 91 differing. The two OUTPUT addresses stay
   `&sp->unk_84' / `&sp->unk_88', which is where the target's two `addiu'
   come from. */
s32 func_s14_800246D8(u8 *p)
{
    UnkDngS14LineSP *sp = (UnkDngS14LineSP *)0x1F800000;
    u8 *base;
    LINE_G2 *l;
    UnkDngS14Word u;

    base = D_80083160.unk_00;
    sp->unk_18 = base + 0xB0;
    l = *(LINE_G2 **)(base + 0x8D0);
    *(LINE_G2 **)(base + 0x8D0) = l + 1;
    setLineG2(l);
    sp->unk_64 = *(u16 *)(p + 0x22);
    sp->unk_66 = *(u16 *)(p + 0x26);
    sp->unk_68 = *(u16 *)(p + 0x2A);
    sp->unk_6C = *(u16 *)(p + 0x3A);
    sp->unk_6E = *(u16 *)(p + 0x3E);
    sp->unk_70 = *(u16 *)(p + 0x42);
    switch (*(s16 *)(p + 0x10) - 1)
    {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
        u.h[1] = sp->unk_6C - sp->unk_64;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_6C = sp->unk_64 + (u.w >> 16);
        u.h[1] = sp->unk_6E - sp->unk_66;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_6E = sp->unk_66 + (u.w >> 16);
        u.h[1] = sp->unk_70 - sp->unk_68;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) + 1;
        sp->unk_70 = sp->unk_68 + (u.w >> 16);
        {
            s32 c;
            s32 c2;
            s32 r;
            s32 g;
            s32 b;

            c = 0xF;
            r = (*(s16 *)(p + 0x10) + 1) * c + 0x40;
            c2 = 0x17;
            b = (*(s16 *)(p + 0x10) + 1) * c2 + 0x40;
            g = (7 - *(s16 *)(p + 0x10)) << 3;
            l->g1 = 0x40 - g;
            l->r0 = 0x40;
            l->g0 = 0x40;
            l->b0 = 0x40;
            l->r1 = r;
            l->b1 = b;
        }
        break;
    case 7:
        l->r0 = 0x40;
        l->g0 = 0x40;
        l->b0 = 0x40;
        l->r1 = 0x7F;
        l->g1 = 0;
        l->b1 = 0xFF;
        break;
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        u.h[1] = sp->unk_6C - sp->unk_64;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_64 = sp->unk_64 + (u.w >> 16);
        u.h[1] = sp->unk_6E - sp->unk_66;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_66 = sp->unk_66 + (u.w >> 16);
        u.h[1] = sp->unk_70 - sp->unk_68;
        u.h[0] = 0;
        u.w >>= 3;
        u.w *= *(s16 *)(p + 0x10) - 7;
        sp->unk_68 = sp->unk_68 + (u.w >> 16);
        {
            s32 c;
            s32 c2;
            s32 r;
            s32 g;
            s32 b;

            c = 0xF;
            r = (*(s16 *)(p + 0x10) - 7) * c + 0x40;
            c2 = 0x17;
            b = (*(s16 *)(p + 0x10) - 7) * c2 + 0x40;
            g = (c - *(s16 *)(p + 0x10)) << 3;
            l->g1 = 0;
            l->g0 = g;
            l->r1 = 0x7F;
            l->b1 = 0xFF;
            l->r0 = r;
            l->b0 = b;
        }
        break;
    }
    sp->unk_F4 = RotTransPers((SVECTOR *)((u32)sp | 0x64), (s32 *)((u32)sp | 0xD8), &sp->unk_84, &sp->unk_88);
    sp->unk_F8 = RotTransPers((SVECTOR *)((u32)sp | 0x6C), (s32 *)((u32)sp | 0xDC), &sp->unk_84, &sp->unk_88);
    l->x0 = sp->unk_D8;
    l->y0 = sp->unk_DA;
    l->x1 = sp->unk_DC;
    l->y1 = sp->unk_DE;
    sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
    if ((u32)sp->unk_B4 < 0x1E0)
    {
        addPrim(sp->unk_18 + sp->unk_B4 * 4, l);
    }
    return 0;
}

/* The beam's root step, a six-state machine on *(s16 *)(p + 0xA); the +0x10
   timer is bumped on entry and every arm falls out through one shared
   `*(s16 *)(p + 0x14) = 0;'. State 0 reseeds the run: zero the timer, advance
   the state, take the caster's facing octant out of its +0x2A angle, clear the
   eight node slots at +0x16 and whiten the renderable, then fall through.
   State 1 projects the caster's slot through func_8003DE58, bails unless that
   or the slot's +0x14 0x8000 flag admits it, copies the slot position into b
   (plus func_8003DE58's three deltas, or z - 0x40 under the flag), gates on
   the room record's 0x80 bit, saves b into the +0x20 payload, and aims: WITH a
   lock-on actor at the caster's +0x60 it takes the three absolute axis deltas,
   the actor's +0x0/+0x4 and its +0x88 height as the +0x38 destination and the
   actor's grid cell into +0x50/+0x52; with NO target it walks up to eight tiles
   along the octant through func_800A44E0 / func_800BCB04 and keeps the last
   tile that was passable, below 0x201 and within 0x3F of the caster. Either way
   the three b velocities are (destination - position) / 15, the state advances
   and func_800A56E0(0x300) plays. State 2 integrates the velocity, draws the
   line through func_s14_800246D8 and hands off to func_s14_80024428 after 0xF
   frames; state 3 pulses func_s14_80024428 every eighth frame until 0x18;
   state 4 fires func_s14_80024060 at the recorded tile at 0x24; state 5 clears
   D_8008346C and retires the actor. 438 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s14_80024048, SIX ENTRIES AND NO PAD -- confirmed by
   tools/decomp/jtcheck.py and by the dispatch bound `sltiu $v0, $v1, 0x6':
   0 .Ls14_80024B58, 1 .Ls14_80024B9C, 2 .Ls14_8002505C, 3 .Ls14_800250D0,
   4 .Ls14_80025114, 5 .Ls14_80025150. All six labels are distinct, so all six
   are real cases and none is a default pad; their addresses ascend, so the arms
   are written ascending 0..5. Case 0 falls through into case 1.

   THIS WAS A 65-ROW PARK (docs/dng-parks/func_s14_80024AE4.c, agent-h33). What
   cleared it was the landed func_s13_80024C88 in csrc/ovl_dng_s13.c (dngtwin
   0.926, 465 words), which did not exist when the park was written: it is the
   same root step one bank over, and it supplies BOTH of the two faults the park
   could not see. The park's own five standing facts all held and are re-priced
   below; two of its figures INVERTED once the body reached 0.

   FACTS MEASURED ON THIS BODY, every figure RE-PRICED FROM THE 0 STATE
   (aligned differences at 438 words unless noted):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `i = 7' EVALUATED BEFORE `t = p + 7'                     |  438  |   0  |
   |   ... `t = p + 7;' as its own statement before the loop  |  438  |  16  |
   | `func_s14_800246D8_2(p, c)' -- TWO arguments             |  438  |   0  |
   |   ... one argument, through the landed prototype         |  437  |   7  |
   | all THREE deltas as `abs()'                              |  438  |   0  |
   |   ... the z delta written out as `if (d < 0) d = -d;'    |  439  |  14  |
   |   ... x and y written out as well                        |  438  |  16  |
   | `bx = tx; by = ty;' at the BOTTOM of the walk loop       |  438  |   0  |
   |   ... `by = ty; bx = tx;' (the park's standing fact)     |  438  |  21  |
   | the payload save as `*(UnkVec *)(p + 0x20) = *b;'        |  438  |   0  |
   |   ... six word-at-a-time field stores                    |  444  |  91  |
   | the walk-arm zero stores DESCENDING 0x8 / 0x4 / 0x0      |  438  |   0  |
   |   ... ascending 0x0 / 0x4 / 0x8                          |  438  |   4  |
   | `& 0xFFFFFFC0' into func_800A44E0's `u16' prototype      |  438  |   0  |
   |   ... `& 0xFFC0' against the same prototype              |  441  |   7  |
   | the `p + 0x42' store BEFORE `p + 0x40 = 0'               |  438  |   0  |
   |   ... the other order                                    |  438  |   2  |
   | ONE loop counter shared by state 0's clear and the walk  |  438  |   0  |
   |   ... a separate `j' for the walk                        |  438  | 142  |
   | ONE `UnkVec *v' shared by both aiming arms               |  438  |   0  |
   |   ... a separate `UnkVec *v2' in the walk arm            |  438  |  24  |

   THE LAST TWO ROWS WERE A STATEMENT-ORDER FAULT WEARING AN ALLOCATION FAULT'S
   CLOTHES, and it is the whole reason this park stood at 65. With `t = p + 7;'
   written as its own statement ahead of the loop the residue is 16 rows, EVERY
   ONE of them a clean $s0/$s1 exchange: `i' takes $s0 and `n'/`x' take $s1
   where the target has them the other way round, with no instruction moved and
   no word gained. `-da' confirms the mechanism -- `i' is reg 91 with 8 textual
   references weighted to 14 by flow.c's loop_depth, so `floor_log2(n_refs) *
   n_refs' is 42 against `n''s 30, and global.c allocates it eleventh where the
   target allocates it after `n'. None of that is reachable: twelve spellings in
   the park's own table, five declaration permutations (ALL FREE, so the
   pseudo-number tie-break is not involved) and both loop shapes
   (`for'/`do-while', measured 0 either way) leave it at 16. What moves it is
   `i = 7' reaching expand FIRST, which is one comma in the `for'-init and
   costs no word -- the two insns are five apart in one basic block, so the
   listing cannot be read for their order, only priced.

   THE SECOND FAULT IS THE ARITY OF func_s14_800246D8, and the twin names the
   diagnostic outright. func_s14_800246D8 is LANDED IN THIS UNIT taking one
   argument and that definition is untouched; this call site passes two, through
   the unit-local alias `func_s14_800246D8_2'. The tell is a missing parameter
   copy in the prologue plus a skipped argument register in the calling block:
   with one argument `c' stays in its incoming $a2, the entry copy
   `addu $a1, $a2, $zero' is absent (the word this body is short), and state 2's
   four mover temps sit in $v0/$v1/$a1/$a2 where the target has $v0/$v1/$a2/$a3.
   `c' is live only before the first call on every path, so $a1 costs it
   nothing. No tool reports this: relcand compares the call SET, not the arity.

   TWO OF THE PARK'S FIGURES INVERTED, both because they were taken while the
   statement-order fault was still live. The park prices `by = ty; bx = tx;' as
   the right order at 63 against 113; from 0 it is 21 rows WRONG and the twin's
   `bx = tx; by = ty;' is right -- and 21 is the twin's own figure for its
   rejected spelling, to the digit. The park also prices `abs()' on the z delta
   as two words SHORT; from 0 the manual form is a word LONG and 14 rows, so all
   three deltas are `abs()' exactly as the twin has them.

   MEASURED AND FREE, so none of these is why this lands: merging `n' into the
   shared `UnkVec *v' (438/0 both ways -- it reads as a 14-row fact from the
   broken body because it perturbs the same allocation the statement order
   decides, which is the clearest case of `an improvement is not evidence' in
   this unit); the walk-arm zero stores written as plain casts off `v' rather
   than as `v->unk_08' field refs, which the twin prices at 73 rows (438/0 both
   ways here); `((s32)(bx << 16) >> 10) + 0x20' in place of `bx * 64 + 0x20'
   (438/0 both ways, so the park's insistence on the shift form is a free fact);
   and passing -0x400 directly to the second func_800BCB04 instead of reading
   back the `v + 0xA' pre-store, which the twin prices at 6 rows (438/0 here).

   THE DEAD `lui $v0, 0x8007' in the walk preheader at 80024E08 IS NOT SOURCE --
   it is the table HIGH hoisted out of a loop whose body then rematerialised its
   own `lui'/`addiu' pairs, and this body emits it without being asked, exactly
   as the twin's comment records for its own copy. */
void func_s14_80024AE4(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *n;
    u8 *m;
    u8 *w;
    u8 *r;
    u8 *t;
    UnkVec *v;
    u16 z;
    s32 i;
    s32 h;
    s32 x;
    s32 y;
    s16 tx;
    s16 ty;
    s16 bx;
    u16 out[4];
    s16 by;

    q = *(u8 **)(p + 0x0);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = (*(u16 *)(q + 0x2A) >> 9) & 7;
        for (i = 7, t = p + 7; i >= 0; i--, t--)
        {
            t[0x16] = 0;
        }
        *(u32 *)(c + 0xC) = 0x808080;
        /* fall through */
    case 1:
        n = q - 0x20;
        if (func_8003DE58(*(u8 **)(*(u8 **)(n + 0xC) + 0x8), (UnkSlot *)*(u8 **)(n + 0xC),
                          (s16 *)out, 0) == 0 &&
            (*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
        {
            break;
        }
        m = *(u8 **)(n + 0x8);
        *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
        z = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)b + 0xA) = z;
        if ((*(u16 *)(*(u8 **)(n + 0xC) + 0x14) & 0x8000) == 0)
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
        *(UnkVec *)(p + 0x20) = *b;
        if (*(u8 **)(q + 0x60) != NULL)
        {
            v = (UnkVec *)*(u8 **)(*(u8 **)(q + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(q + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            r = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
            *(s16 *)(p + 0x50) = r[0x24];
            *(s16 *)(p + 0x52) = r[0x25];
            *(s32 *)(p + 0x38) = *(s32 *)((u8 *)v + 0x0);
            *(s32 *)(p + 0x3C) = *(s32 *)((u8 *)v + 0x4);
            *(s16 *)(p + 0x42) = *(u16 *)(*(u8 **)(q + 0x60) + 0x88);
            *(s16 *)(p + 0x40) = 0;
            b->unk_0C = (*(s32 *)(p + 0x38) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x3C) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x40) - b->unk_08) / 15;
        }
        else
        {
            w = *(u8 **)(n + 0xC);
            tx = w[0x24];
            ty = w[0x25];
            bx = tx;
            by = ty;
            for (i = 0; i < 8; i++)
            {
                x = tx;
                y = ty;
                if ((s16)func_800A44E0((x << 6) & 0xFFFFFFC0, (y << 6) & 0xFFFFFFC0,
                                       *(s16 *)(q + 0x88), (s16)(*(s16 *)(p + 0xE) << 9)) != 0)
                {
                    break;
                }
                h = func_800BCB04(((x + D_8006CCD8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  ((y + D_8006CCE8[*(s16 *)(p + 0xE)]) << 6) + 0x20 & 0xFFE0,
                                  (s16)(*(u16 *)(q + 0x88) - 0x20));
                if ((s16)h >= 0x201)
                {
                    break;
                }
                if ((s16)(h - *(u16 *)(q + 0x88)) < -0x3F)
                {
                    break;
                }
                tx = tx + (D_8006CCD8[*(s16 *)(p + 0xE)] & 0xFFFF);
                ty = ty + (D_8006CCE8[*(s16 *)(p + 0xE)] & 0xFFFF);
                bx = tx;
                by = ty;
            }
            v = (UnkVec *)(p + 0x38);
            v->unk_08 = 0;
            v->unk_04 = 0;
            v->unk_00 = 0;
            *(s16 *)((u8 *)v + 0x2) = bx * 64 + 0x20;
            *(s16 *)((u8 *)v + 0x6) = by * 64 + 0x20;
            *(s16 *)((u8 *)v + 0xA) = -0x400;
            *(s16 *)((u8 *)v + 0xA) = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                                    *(s16 *)((u8 *)v + 0xA));
            if (*(s16 *)((u8 *)v + 0xA) >= 0x201)
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            }
            b->unk_0C = (*(s32 *)(p + 0x38) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x3C) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x40) - b->unk_08) / 15;
            *(s16 *)(p + 0x50) = bx;
            *(s16 *)(p + 0x52) = by;
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        func_800A56E0(0x300);
        break;
    case 2:
        b->unk_00 = b->unk_00 + b->unk_0C;
        b->unk_04 = b->unk_04 + b->unk_10;
        b->unk_08 = b->unk_08 + b->unk_14;
        func_s14_800246D8_2(p, c);
        if (*(s16 *)(p + 0x10) >= 0xF)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            func_s14_80024428(p, b, 0, p + 0x16);
        }
        break;
    case 3:
        if ((*(u16 *)(p + 0x10) & 7) == 0)
        {
            func_s14_80024428(p, b, (s16)(*(u16 *)(p + 0x10)) >> 3, p + 0x16);
        }
        if (*(s16 *)(p + 0x10) >= 0x18)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x24)
        {
            func_s14_80024060(*(s16 *)(p + 0x50), *(s16 *)(p + 0x52), q, p[0x9]);
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
    }
    *(s16 *)(p + 0x14) = 0;
}
