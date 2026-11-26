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
extern u8 D_800E3D68;
extern u8 *D_800E3D7C;

/* 75 words, 0 differing. Walks the resident actor ring at D_800E3D7C, and
   for each actor the cursor admits whose grid cell is within 2 of (x, y)
   calls the 0x800C87C4 report with a 0x10 chance -- 0xFF when D_800E3D68
   is saturated -- and a rank of (d >> 2) + 0x10 plus two random bits.
   THIS COPY'S OWN CONSTANT: 0x10 -> 0x20 at 1 of 2 sites.
   The rest of this comment is the family representative's and its other
   numbers are its own.
*/
UnkNode *func_8003FC64(s32);
void func_8003DB94(void *, void *, s32);
void func_800478B8(u8 *);
extern u8 D_800DEDB0[];
extern u8 D_80045340[];
extern u8 D_800DEAE0[];
s32 func_800644B8(s32);
s32 func_80064584(s32);
void func_s13_80024330(u8 *p, UnkVec *b, u8 *c);
void func_s13_800244D4(u8 *, UnkVec *, u8 *);
s32 func_s13_800247D4(u8 *);
s32 func_s13_800247D4_2(u8 *, u8 *) __asm__("func_s13_800247D4");
s32 func_800A44E0(u16, u16, s16, u32);
s32 func_800BCB04(s32, s32, s16);
s32 func_800A56E0(s32);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s32 D_8008346C;

void func_s13_80024060(s16 x, s16 y, u8 *c, u8 d)
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
                    chance = 0x20;
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

/* The 0x212 flash's own step function: bump the +0x14 counter on the node at
   the actor's +0x0, grow the renderable's +0x1E scale by 0x800 and copy that
   value across to +0x1C, hand it to func_800478B8, and when its +0x14 reports
   0x6000 mark the owning actor retired and set the same bit in D_800814A0.
   36 words, 0 differing, first compile.

   TWIN: the landed func_s10_80024224 in csrc/ovl_dng_s10.c (dngtwin 0.896,
   31 words). Deltas off this listing: the counted node is at the actor's +0x0
   and its counter at +0x14 where the twin uses +0xC and +0x16, and the five
   extra words are the scale pair this copy adds. The twin's note on
   D_800814A0 being the PLAIN scalar holds here and gives the `lui'/`lw %lo'.

   THE SCALE PAIR IS A READ-BACK, NOT A SECOND SUM -- measured, aligned
   differences at 36 words unless noted:

     `*(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);'          36 w,  0   kept
       ... `*(u16 *)(c + 0x1C) + 0x800' again      37 w,  6
       ... the two stores swapped, +0x1C summed first    36 w,  4

   The 37-word row is the tell: written as a second sum the +0x1C load cannot
   be cse'd against the value just stored at +0x1E, so it costs its own `lhu'.
   The target has ONE `lhu 0x1E' and two `sh' of the same register. */
void func_s13_8002418C(u8 *p, UnkVec *b, u8 *c)
{
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0x800;
    *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212 node that animates this flash at q: white renderable, the
   0x7E0B sprite at 0x1800 scale, the +0x10 0x60 and +0x14 0x10C bits, the
   caller's record at +0x20, func_s13_8002418C as the per-frame step, and a
   position jittered down by rand() % 0x10 under q's own.
   69 words, 0 differing, first compile.

   TWIN: the landed func_s14_8002425C in csrc/ovl_dng_s14.c (dngtwin 0.978,
   70 words). The one missing word is the twin's `*((u8 *)n + 0x24) = 0' which
   this copy does not have; the scale is 0x1800 not 0x1000 and the z jitter
   is -0x20 not -0x40.

   THE TWIN'S ONE STATED FACT IS INVERTED HERE. The twin insists the +0x20
   record store be the FIRST statement inside the gate and prices it third at
   9 aligned differences. On this copy, measured:

     the +0x20 store THIRD, after the step pointer and `m = n->unk_0C'
                                                        69 w,  0   kept
       ... FIRST inside the gate (the twin's spelling)   69 w,  9

   Same price, opposite sign -- and the kept spelling is also the listing's
   own emission order (`sw $v0,0x10($s2)' and `lw $s0,0xC($s2)' ahead of
   `sw $s3,0x20($s2)'), which is how it was spotted before the first compile.
   Both bodies save $s0..$s3 with the same -0x28 frame, so the save count
   predicts nothing here; the residue is the two parameters exchanging
   $s2/$s3, i.e. a REFERENCE-COUNT/ORDER fact in the brief's taxonomy, the
   one class where inversion is expected.

   The twin's discarded first `rand()' holds: removing it is 67 words /
   3 aligned differences. */
void func_s13_8002421C(u8 *v, u8 *q)
{
    UnkNode *n;
    u8 *m;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_s13_8002418C;
        m = (u8 *)n->unk_0C;
        *(u8 **)((u8 *)n + 0x20) = v;
        m[0xE] = 0x80;
        m[0xD] = 0x80;
        m[0xC] = 0x80;
        func_8003DB94(m, D_800DEDB0, 0);
        *(s16 *)(m + 0x1E) = 0x1800;
        *(s16 *)(m + 0x1C) = 0x1800;
        *(s16 *)(m + 0x12) = 0x7E0B;
        *(u16 *)(m + 0x10) |= 0x60;
        *(u16 *)(m + 0x14) |= 0x10C;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        rand();
        *(s16 *)(m + 0x2) = *(u16 *)(q + 0x2);
        *(s16 *)(m + 0x6) = *(u16 *)(q + 0x6);
        *(s16 *)(m + 0xA) = *(u16 *)(q + 0xA) - 0x20 - rand() % 0x10;
    }
}

/* The ring frame's own step function, the step func_s13_80024628 installs:
   bump the +0x14 counter on the node at the actor's +0x0, advance the +0x22
   frame index modulo 8 and respawn the flash through func_s13_8002421C when
   it comes round to the +0x24 slot, ramp the +0x1E angular speed up to 0xC0
   by 8 a frame, spin the +0x1C angle by it modulo 0x1000, grow the +0x20
   radius by 4, and place the mover at the payload position offset by the
   angle's sine and cosine scaled by that radius -- with the renderable's
   +0x1A rotation set from the same angle relative to the camera's
   D_80083160 +0xC8 heading. Finally hand the renderable to func_800478B8
   and re-dress it from D_800DEAE0 when its +0x14 reports 0x6000.
   105 words, 0 differing, first compile.

   NO TWIN BY EITHER TOOL -- dngtwin reports no carved sibling at 0.60 for
   this stub and xsibs is useless on an s-unit. The route in was that
   func_s13_80024628, landed above, builds the payload this reads: its
   +0x1C/+0x20/+0x22/+0x24 stores are this function's reads, so the two
   listings interpret each other and the offsets needed no guessing. The sine
   idiom is func_s13_80024628's own.

   FACTS MEASURED ON THIS BODY, both spellings priced (aligned differences):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `Unk80083160 *g' SET IN THE ENTRY BLOCK                  |  105  |   0  |
   |   ... `D_80083160.unk_C8' named directly at the use      |  103  |  24  |
   |   ... the same local assigned just before its one use    |  103  |  24  |
   | the +0x1C spin as TWO statements, sum then `% 0x1000'    |  105  |   0  |
   |   ... folded into one statement                          |  103  |  20  |
   | the +0x1A rotation as `... * -0x100 >> 8'                |  105  |   0  |
   |   ... `* 0x100 >> 8' (no negation)                       |  102  |   4  |
   | the +0x22 wrap as `*(u16 *)(p + 0x22) - 8'               |  105  |   0  |
   |   ... `= 0'                                              |  104  |   5  |

   THE POINTER LOCAL IS THE BIG ONE AND csrc/gprel_10308.c ALREADY NAMES THE
   MECHANISM: a ONE-SET `Unk80083160 *g = &D_80083160;' is the only way found
   to get a large in-image object's `%hi' materialised BEFORE the first
   conditional branch. Here it buys TWO words -- the `lui $v1,%hi' lands in
   the entry block and the `addiu $s3,$v1,%lo' lands in the first `bnez's
   delay slot, which is what keeps $s3 live across the three calls. Note the
   second row: the local alone is not enough, it must be set in the entry
   block, so this is a STATEMENT-POSITION fact and not just a shape one.

   The +0x22 wrap row is the cheap diagnostic for the same cse effect one
   level down: `*(u16 *)(p + 0x22) - 8' re-reads a field cse knows equals
   `old + 1', so it folds to `addiu $v0,$a0,-0x7' off the ORIGINAL load and
   keeps that load live. `= 0' would be correct arithmetic and is 5 rows. */
void func_s13_80024330(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g;

    g = &D_80083160;
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s16 *)(p + 0x22) = *(u16 *)(p + 0x22) + 1;
    if (*(s16 *)(p + 0x22) >= 8)
    {
        *(s16 *)(p + 0x22) = *(u16 *)(p + 0x22) - 8;
    }
    if (*(s16 *)(p + 0x22) == *(s16 *)(p + 0x24))
    {
        func_s13_8002421C(*(u8 **)p, (u8 *)b);
    }
    if (*(s16 *)(p + 0x1E) < 0xC1)
    {
        *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) + 8;
    }
    *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + *(u16 *)(p + 0x1E);
    *(s16 *)(p + 0x1C) = *(s16 *)(p + 0x1C) % 0x1000;
    *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) + 4;
    *(s16 *)((u8 *)b + 0x2) =
        *(u16 *)(p + 0x6) + ((func_800644B8(*(s16 *)(p + 0x1C)) >> 4) * *(s16 *)(p + 0x20) >> 8);
    *(s16 *)((u8 *)b + 0x6) =
        *(u16 *)(p + 0xA) + ((func_80064584(*(s16 *)(p + 0x1C)) >> 4) * *(s16 *)(p + 0x20) >> 8);
    *(s16 *)(c + 0x1A) = (func_80064584(*(s16 *)(p + 0x1C) - g->unk_C8) >> 4) * -0x100 >> 8;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        func_8003DB94(c, D_800DEAE0, 0);
    }
}

/* The ring frame's LATE step, the variant func_s13_80024330 turns into once
   the flash stops respawning: bump the +0x14 counter on the node at the
   actor's +0x0, fade the renderable's packed +0xC colour word by 0x80808 a
   frame, spin the +0x1C angle by the +0x1E speed modulo 0x1000, grow the
   +0x20 radius by 4, place the mover at the payload position offset by the
   angle's sine and cosine scaled by that radius, set the renderable's +0x1A
   rotation from the same angle relative to the camera's D_80083160 +0xC8
   heading, hand it to func_800478B8 and re-dress it from D_800DEAE0 when its
   +0x14 reports 0x6000.
   85 words, 0 differing, FIRST COMPILE.

   DONOR: the landed func_s13_80024330 twenty listings up in this same file,
   found by dngcallee at 6 of 6 symbols with a CLEAN GAP to the 4/6 tier --
   and with a byte-identical frame (-0x28, s0/ra/s3/s2/s1 in that order), so
   its allocation facts were in budget before any compile. dngtwin and xsibs
   were never run. This body is that one minus the +0x22/+0x24 frame-index
   block and the +0x1E ramp gate, plus the +0xC colour fade; all three of the
   donor's facts HELD, and the figures below are this body's own.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced
   (aligned differences):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `Unk80083160 *g' SET IN THE ENTRY BLOCK                  |   85  |   0  |
   |   ... `D_80083160.unk_C8' named directly at the use      |   82  |  15  |
   | the +0x1C spin as TWO statements, sum then `% 0x1000'    |   85  |   0  |
   |   ... folded into one statement                          |   82  |   9  |
   | the +0xC fade SECOND, right after the +0x14 bump         |   85  |   0  |
   |   ... moved below the +0x20 radius bump                   |   87  |  27  |
   | the fade as `-= 0x80808'                                 |   85  |   0  |
   |   ... `+= 0xFFF7F7F8'                                     |   85  |   0  |

   THE FADE'S POSITION IS THE BIG ONE and it is a fact the donor cannot have,
   because the donor has no fade. Second in the body it costs nothing; below
   the radius bump it is 87 words and 27 rows, because `c' then has to stay
   live across the `% 0x1000' block and takes the callee-saved register the
   payload pointer wants. The two constant spellings are FREE -- both are one
   `lui'/`ori' pair, so the C89 unsignedness of 0xFFF7F7F8 never reaches an
   operator that could see it. `-= 0x80808' is kept because it says what the
   code does.

   The `g' and `% 0x1000' rows reproduce the donor's DIRECTION and neither
   its magnitude (24 and 20 there, 15 and 9 here), which is the usual pattern
   for this family. */
void func_s13_800244D4(u8 *p, UnkVec *b, u8 *c)
{
    Unk80083160 *g;

    g = &D_80083160;
    *(s16 *)(*(u8 **)p + 0x14) = *(u16 *)(*(u8 **)p + 0x14) + 1;
    *(s32 *)(c + 0xC) -= 0x80808;
    *(s16 *)(p + 0x1C) = *(u16 *)(p + 0x1C) + *(u16 *)(p + 0x1E);
    *(s16 *)(p + 0x1C) = *(s16 *)(p + 0x1C) % 0x1000;
    *(s16 *)(p + 0x20) = *(u16 *)(p + 0x20) + 4;
    *(s16 *)((u8 *)b + 0x2) =
        *(u16 *)(p + 0x6) + ((func_800644B8(*(s16 *)(p + 0x1C)) >> 4) * *(s16 *)(p + 0x20) >> 8);
    *(s16 *)((u8 *)b + 0x6) =
        *(u16 *)(p + 0xA) + ((func_80064584(*(s16 *)(p + 0x1C)) >> 4) * *(s16 *)(p + 0x20) >> 8);
    *(s16 *)(c + 0x1A) = (func_80064584(*(s16 *)(p + 0x1C) - g->unk_C8) >> 4) * -0x100 >> 8;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        func_8003DB94(c, D_800DEAE0, 0);
    }
}

/* Spawn the 0x212 node for one of the ring's eight frames, three steps of
   the eight-way circle per call: the caller's record at +0x20, then the
   payload at +0x20 gets the `k * 3 % 8' frame index at +0x24 and its
   0x200-per-step angle at +0x1C, the 0x20/0/-8 radius triple, and q's
   position; the renderable is dressed white from D_800DEAE0 as the 0x7E0B
   sprite at 0x1800 scale with the +0x14 0xC/0x100 and +0x10 0x20 bits;
   then the mover at n->unk_08 is placed at the payload position offset by
   the angle's sine and cosine scaled by the +0x20 radius and lowered by 8.
   func_s13_80024330 is its per-frame step. Returns the node, or NULL.
   107 words, 0 differing.

   TWIN: the landed func_s10_80024790 in csrc/ovl_dng_s10.c (dngtwin 0.645,
   104 words) -- a 0.6x lead, so a body to READ rather than edit. What
   transferred from it: the `s16 k' parameter used unqualified, the
   `sra'/`sll'/`subu' signed-modulo shape, the `(f(angle) >> 4) * radius >> 8'
   placement idiom, and the whole declaration block. What did not: this copy's
   frame index is `k * 3 % 8' against its `(k << 8) % 0x1000', and the
   renderable here is plain white rather than scaled by k.

   FACTS MEASURED ON THIS BODY, both spellings priced (aligned differences at
   107 words unless noted):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `k * 3 % 8' written INLINE AT BOTH SITES                 |  107  |   0  |
   |   ... an `s32 j' local holding it, named twice           |  107  |   9  |
   | the +0x1C angle store FIRST of the five payload stores   |  107  |   0  |
   |   ... the +0x24 index store moved ahead of it            |  107  |  10  |
   | the +0x14 bits as TWO `|=' statements with +0x10 between |  107  |   0  |
   |   ... merged into one `|= 0x10C'                         |  104  |   7  |

   THE `j' LOCAL IS THE ONE THAT MATTERED and it is the opposite of what the
   twin's table suggests (the twin keeps a pointer local and prices inlining
   at 21 rows). The residue is a pure $a0/$v1 exchange across the nine
   instructions of the modulo block with IDENTICAL instruction order: with
   the local, `k * 3' is tied to the dying `k << 1' and takes $v1, and the
   adjusted dividend takes $a0; inline, cse unifies the second occurrence
   against the first and the pair comes out the other way. Nothing is moved
   in the 9-row residue, which is the brief's "uniform two-register exchange
   with nothing moved" signature -- and here the cure was a LOCAL COUNT, not
   a comparison flip or a reference bump.

   MEASURED AND FREE, so neither is why this lands: `(k * 3 % 8) * 0x200' in
   place of `<< 9' (107 / 0 either way -- an unsigned-safe shift of a value
   already widened, so convert.c's narrowing rule has nothing to bite on),
   and the +0x20 record store written second rather than first (107 / 0),
   which is the fact that is load-bearing 60 words up in
   func_s13_8002421C and is simply absent here. */
UnkNode *func_s13_80024628(u8 *v, UnkVec *q, s16 k)
{
    UnkNode *n;
    u8 *m;
    u8 *r;
    u8 *w;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        *(u8 **)((u8 *)n + 0x20) = v;
        n->unk_10 = (u32)func_s13_80024330;
        m = (u8 *)n + 0x20;
        *(s16 *)(m + 0x1C) = (k * 3 % 8) << 9;
        *(s16 *)(m + 0x20) = 0x20;
        *(s16 *)(m + 0x1E) = 0;
        *(s16 *)(m + 0x22) = -0x8;
        *(s16 *)(m + 0x24) = k * 3 % 8;
        *(s16 *)(m + 0x6) = *(u16 *)((u8 *)q + 0x2);
        *(s16 *)(m + 0xA) = *(u16 *)((u8 *)q + 0x6);
        *(s16 *)(m + 0xE) = *(u16 *)((u8 *)q + 0xA);
        r = (u8 *)n->unk_0C;
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        func_8003DB94(r, D_800DEAE0, 0);
        *(s16 *)(r + 0x1E) = 0x1800;
        *(s16 *)(r + 0x1C) = 0x1800;
        *(s16 *)(r + 0x12) = 0x7E0B;
        *(u16 *)(r + 0x14) |= 0xC;
        *(u16 *)(r + 0x10) |= 0x20;
        *(u16 *)(r + 0x14) |= 0x100;
        func_8004491C(n, (u32)D_80045340);
        w = (u8 *)n->unk_08;
        *(s16 *)(w + 0x2) = *(u16 *)(m + 0x6) + ((func_800644B8(*(s16 *)(m + 0x1C)) >> 4) * *(s16 *)(m + 0x20) >> 8);
        *(s16 *)(w + 0x6) = *(u16 *)(m + 0xA) + ((func_80064584(*(s16 *)(m + 0x1C)) >> 4) * *(s16 *)(m + 0x20) >> 8);
        *(s16 *)(w + 0xA) = *(u16 *)(m + 0xE) - 0x8;
        return n;
    }
    return NULL;
}

/* THE BEAM FAN: eight semi-transparent LINE_G2 primitives per call, one per
   eighth of the circle, each drawn from the payload's +0x3A/+0x3E/+0x42
   origin to its +0x52/+0x56/+0x5A endpoint swung round by the
   `(i * 3 % 8) * 0x200' angle, interpolated and coloured by the +0x10 phase,
   projected through the 0x1F800000 scratchpad with RotTransPers and chained
   into D_80083160's +0x8D0 primitive ring with the addPrim macro. Returns 0.
   301 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s13_80024008. 15 cases, 3 distinct targets: cases
   0..6 (phase 1..7) share the forward arm, case 7 (phase 8) is the flat
   middle, cases 8..14 (phase 9..15) the reverse arm. A standalone
   `tools/cdkgcc -S' of this unit emits the entries in exactly that order --
   7 x forward, then the middle, then 7 x reverse -- which is entry-address
   order and the listing's order. The ROM's 16th `.word 0' is the pad, which
   a standalone compile cannot show.

   ITS ARITY IS ONE ARGUMENT AND THE CALLER PASSES TWO. This body takes one
   `u8 *p' and returns `s32'; func_s13_80024C88 calls it with `(p, c)', and
   that two-argument call is priced 465/0 in that function's own comment. So
   the call goes through the unit-local alias `func_s13_800247D4_2', exactly
   as csrc/ovl_dng_s14.c and csrc/ovl_dng_s15.c do for their copies. The
   stale `void func_s13_800247D4(u8 *, u8 *);' forward declaration this unit
   carried was replaced; func_s13_80024C88 re-verified at 465/0 afterwards.

   DONORS: the landed func_s14_800246D8 (csrc/ovl_dng_s14.c) for the SHAPE
   and the landed func_s22_8002445C (csrc/ovl_dng_s22.c) for the two
   scratchpad facts. Neither dngtwin nor xsibs was needed -- the route in was
   this unit's TYPES.txt flagging `UnkDngS14LineSP' and `UnkDngS14Word' as
   declared-but-unused here, and both typedefs name exactly the scratchpad
   layout and the 16.16 union this body uses. s14 is a ONE-SHOT version of
   this routine (no loop, 259 words); everything structural came from it.

   WHAT TRANSFERRED FROM s14, UNCHANGED AND AT ITS PRICE:
     - the int-typed switch index (`- 1' in the source, cases numbered from
       0), because c_expand_start_case strips the s16 promotion back off;
     - the 16.16 union interpolator, `u.h[1] = delta' then the mask then
       `u.w >>= 3; u.w *= k;' as SEPARATE statements;
     - the two colour multipliers as VARIABLES, not literals, so the
       multiply is a real `mult' instead of expand-time `sll'/`subu'.
   WHAT INVERTED: s14's `(u32)sp | 0x64' argument form. Here all four
   RotTransPers addresses are `&sp->unk_XX' and the ior form is 4 rows --
   the same inversion func_s22_8002445C recorded, so that is now 2 for 2 and
   s14's ior is the outlier.
   WHAT s22 SUPPLIED: `g = &D_80083160' as a pointer local for the two
   in-loop +0x8D0 reads while the ONE pre-loop use stays direct. That gives
   the target's `lui'/`addiu'/`lw 0($t0)' triple inside the loop and the
   folded `lw %lo(D_80083160)($v0)' outside it, and it is the whole reason
   the loop body reads the ring pointer the way it does.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced
   (aligned differences; EVERY figure below was re-measured after the body
   reached 0, and four of them had read differently mid-fight -- see the
   retractions at the end):

   | spelling                                                 | words | rows |
   |----------------------------------------------------------|-------|------|
   | `msk = -0x10000;' BEFORE `w = 0x40;' at the loop top      |  301  |   0  |
   |   ... the two statements swapped                          |  301  |   4  |
   |   ... `u.h[0] = 0' at all six sites, no `msk' at all      |  301  |   4  |
   | `w = 0x40;' as the FIRST statement of the loop body       |  301  |   0  |
   |   ... moved below the two trig calls, before the switch    |  298  |  61  |
   |   ... literal 0x40 at all seven sites, no local           |  300  |  67  |
   | `rr = 0x7F' a local in the REVERSE arm                     |  301  |   0  |
   |   ... the literal 0x7F at its store                        |  301  |   7  |
   |   ... plus a `bb = 0xFF' local beside it                   |  301  |  12  |
   | `switch (*(s16 *)(p + 0x10) - 1)', cases 0..14             |  301  |   0  |
   |   ... `switch (*(s16 *)(p + 0x10))', cases 1..15           |  303  |   5  |
   | `g = &D_80083160' for the two in-loop +0x8D0 reads         |  301  |   0  |
   |   ... `D_80083160.unk_00' named directly at both           |  300  |   4  |
   | `for (i = 7; i >= 0; i--)' with `i * 3' in the angle        |  301  |   0  |
   |   ... `for (i = 21; i >= 0; i -= 3)' with `i % 8'          |  297  |  45  |
   | `&sp->unk_64' / `&sp->unk_D8' at RotTransPers              |  301  |   0  |
   |   ... s14's `(SVECTOR *)((u32)sp | 0x64)' ior form         |  301  |   4  |

   MEASURED AND FREE, so none of these is why this lands:
     - `c = 0xF; c2 = 0x17;' both assigned before their uses, against the
       interleaved `c; r; c2; b' order (301/0 either way);
     - an `s32 st' local for the colour block's one `p + 0x10' read, against
       naming the cast three times (301/0 either way);
     - the angle as an `s32 a' local, against writing `(i * 3 % 8) << 9'
       inline at BOTH trig calls (301/0 either way -- cse unifies the two
       occurrences and the value still survives the first call in $s0). Note
       func_s13_80024628 in this unit prices the same inline-versus-local
       question at 9 rows in the opposite direction; it is free here.

   THE TWO HOISTED CONSTANTS ARE AN ORDER FACT, AND IT IS WHY THIS BODY
   NEEDS AN EXPLICIT MASK LOCAL. The target keeps 0x40 in $s7 and 0xFFFF0000
   in $fp, both initialised in the prologue, and 0xFFFF0000 is initialised
   FIRST. Both are loop.c hoists out of the loop body, and move_movables
   emits each moved insn immediately before loop_start, so the hoists come
   out in the order their SETS appear inside the loop -- and a hoist always
   lands AFTER any statement the source already wrote before the loop. With
   `u.h[0] = 0' the 0xFFFF0000 pseudo is created by store_bit_field inside
   the forward ARM, i.e. after `w = 0x40' at the loop top, so 0x40 is hoisted
   first and the two prologue pairs come out swapped: 4 rows. Writing the
   mask as a source local assigned one line ABOVE `w' puts both sets at the
   loop top in the order I choose. Spelling it `-0x10000' rather than
   `0xFFFF0000' keeps the expression signed; the value only feeds an `and',
   so both spellings are the same word, and the signed one is kept so a later
   reader is not relying on that.

   AND `w = 0x40' HAS TO BE ABOVE THE TWO TRIG CALLS, not merely inside the
   loop. loop.c will not move a set it reached after a CALL_INSN, so placed
   just before the switch the constant is not hoisted at all, the frame drops
   to -0x38 with one fewer callee-saved register, and the body is 298 words /
   61 rows. That is a STATEMENT-POSITION fact, not a shape one, and it is the
   same class as func_s13_80024330's `g' row twenty listings up.

   THE REVERSE ARM'S `rr' LOCAL IS A ONE-WORD FACT. The target materialises
   0x7F and 0xFF into $t0 and $v0 BEFORE the first colour store, so both are
   live at once, and $t0 is then clobbered -- which forces reload to
   rematerialise the 0xF for `(c - phase)' a second time. That second
   `addiu $t0, $zero, 0xF' IS the 301st word. Written as a bare literal the
   two constants go sequentially through one register, nothing is clobbered,
   the 0xF survives, and the body is one word short. Making 0xFF a local as
   well overshoots: both then share $t0 and it is 12 rows.

   FOUR FIGURES RETRACTED -- each was measured mid-fight and each was wrong:
     - `c = 0xF; c2 = 0x17' up front read 24 rows (35 -> 11) while the `rr'
       and `msk' faults were live. It is FREE from the clean body; it was
       compensating. KEPT anyway, for symmetry with the reverse arm.
     - `w' before the switch read 204 rows; the true figure is 61.
     - the literal 0x40 read 93 rows / 298 words; the true figure is 67 / 300.
     - `rr'/`bb' as locals read 0 -- FREE -- on the 35-row body. From 0, `rr'
       alone is worth 7 and the pair is worth 12. That one INVERTED.
   An `s32 k' local for `phase + 1' and a literal `0xF' in place of `c' in
   the reverse arm were both measured FREE on the 35-row body and have NOT
   been re-priced from 0; treat them as unmeasured. */
s32 func_s13_800247D4(u8 *p)
{
    UnkDngS14LineSP *sp = (UnkDngS14LineSP *)0x1F800000;
    s32 w;
    LINE_G2 *l;
    UnkDngS14Word u;
    Unk80083160 *g;
    s32 i;
    s32 a;
    s32 msk;

    g = &D_80083160;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    for (i = 7; i >= 0; i--)
    {
        msk = -0x10000;
        w = 0x40;
        l = *(LINE_G2 **)(g->unk_00 + 0x8D0);
        *(LINE_G2 **)(g->unk_00 + 0x8D0) = l + 1;
        setLineG2(l);
        sp->unk_64 = *(u16 *)(p + 0x3A);
        sp->unk_66 = *(u16 *)(p + 0x3E);
        sp->unk_68 = *(u16 *)(p + 0x42);
        a = (i * 3 % 8) << 9;
        sp->unk_6C = *(u16 *)(p + 0x52) + (func_800644B8(a) * 2 >> 8);
        sp->unk_6E = *(u16 *)(p + 0x56) + (func_80064584(a) * 2 >> 8);
        sp->unk_70 = *(u16 *)(p + 0x5A);
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
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) + 1;
            sp->unk_6C = sp->unk_64 + (u.w >> 16);
            u.h[1] = sp->unk_6E - sp->unk_66;
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) + 1;
            sp->unk_6E = sp->unk_66 + (u.w >> 16);
            u.h[1] = sp->unk_70 - sp->unk_68;
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) + 1;
            sp->unk_70 = sp->unk_68 + (u.w >> 16);
            {
                s32 c;
                s32 c2;
                s32 r;
                s32 gr;
                s32 b;

                c = 0xF;
                c2 = 0x17;
                r = (*(s16 *)(p + 0x10) + 1) * c + 0x40;
                b = (*(s16 *)(p + 0x10) + 1) * c2 + 0x40;
                gr = (7 - *(s16 *)(p + 0x10)) << 3;
                l->r0 = w;
                l->g0 = w;
                l->b0 = w;
                l->g1 = w - gr;
                l->r1 = r;
                l->b1 = b;
            }
            break;
        case 7:
            l->r0 = w;
            l->g0 = w;
            l->b0 = w;
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
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) - 7;
            sp->unk_64 = sp->unk_64 + (u.w >> 16);
            u.h[1] = sp->unk_6E - sp->unk_66;
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) - 7;
            sp->unk_66 = sp->unk_66 + (u.w >> 16);
            u.h[1] = sp->unk_70 - sp->unk_68;
            u.w &= msk;
            u.w >>= 3;
            u.w *= *(s16 *)(p + 0x10) - 7;
            sp->unk_68 = sp->unk_68 + (u.w >> 16);
            {
                s32 c;
                s32 c2;
                s32 r;
                s32 gr;
                s32 b;
                s32 rr;

                c = 0xF;
                c2 = 0x17;
                rr = 0x7F;
                r = (*(s16 *)(p + 0x10) - 7) * c + 0x40;
                b = (*(s16 *)(p + 0x10) - 7) * c2 + 0x40;
                gr = (c - *(s16 *)(p + 0x10)) << 3;
                l->g1 = 0;
                l->r1 = rr;
                l->b1 = 0xFF;
                l->g0 = gr;
                l->r0 = r;
                l->b0 = b;
            }
            break;
        }
        sp->unk_F4 = RotTransPers((SVECTOR *)&sp->unk_64, (long *)&sp->unk_D8, (long *)&sp->unk_84,
                                  (long *)&sp->unk_88);
        sp->unk_F8 = RotTransPers((SVECTOR *)&sp->unk_6C, (long *)&sp->unk_DC, (long *)&sp->unk_84,
                                  (long *)&sp->unk_88);
        l->x0 = sp->unk_D8;
        l->y0 = sp->unk_DA;
        l->x1 = sp->unk_DC;
        l->y1 = sp->unk_DE;
        sp->unk_B4 = (sp->unk_F4 + sp->unk_F8) / 2;
        if ((u32)sp->unk_B4 < 0x1E0)
        {
            addPrim(sp->unk_18 + sp->unk_B4 * 4, l);
        }
    }
    return 0;
}

/* THE EFFECT'S ROOT STEP, a six-state machine on *(s16 *)(p + 0xA); every
   frame bumps the +0x10 timer first. State 0 resets the timer, clears the
   eight node slots at +0x18 and whitens the renderable, then falls through.
   State 1 projects the owning slot through func_8003DE58 and bails unless
   either that or the slot's +0x14 0x8000 flag admits it, copies the slot's
   position into b (adding func_8003DE58's three deltas, or dropping z by
   0x40 when the flag is set), gates on the room record's 0x80 bit, saves b
   into the +0x38 mover copy, and then aims: WITH a lock-on target it takes
   the target's +0x0/+0x4 and the slot's +0x88 height as the destination at
   +0x50 and the target's grid cell into +0x68/+0x6A; with NO target it walks
   up to eight tiles along the +0xE heading through func_800A44E0 /
   func_800BCB04, lands on the last passable tile, and builds the same +0x50
   destination from the tile centre with func_800BCB04's floor height.
   Either way the three b velocities are (destination - position) / 15 and
   the state advances. State 2 advances the mover, republishes it through
   func_s13_800247D4 and after 0xF frames spawns the eight
   func_s13_80024628 ring frames and plays 0x300. State 3 waits 0x21 frames
   and re-points every child at func_s13_800244D4. State 4 waits 0x11 frames,
   reports through func_s13_80024060 and retires every child. State 5 clears
   D_8008346C and retires the actor. 465 words, 0 differing.

   THIS FUNCTION OWNS jtbl_s13_80024048, SIX ENTRIES AND NO PAD WORD --
   confirmed by tools/decomp/jtcheck.py and by the dispatch bound
   `sltiu $v0, $v1, 0x6': 0 .Ls13_80024CFC, 1 .Ls13_80024D40,
   2 .Ls13_800251F4, 3 .Ls13_80025288, 4 .Ls13_800252E0, 5 .Ls13_80025360.
   All six are distinct labels, so all six are real cases, and their
   addresses ascend, so the arms are written ascending 0..5. Case 0 falls
   through into case 1; no arm is a default pad.

   NO TWIN AT ANY RATIO by dngtwin, and dngnear.py returns ZERO donors for
   this unit because the inference needs a landed in-unit neighbour to invert
   and ovl_dng_s13 had only one. THE ROUTE IN WAS THE CALLEE-SET
   INTERSECTION, and it was decisive: grepping the seven distinctive symbols
   one at a time --

     func_800A44E0 func_800BCB04 D_8006CCD8 D_8006CCE8 func_800A56E0
     func_8003DE58 D_8008346C

   -- over csrc/ovl_dungeon*.c and csrc/ovl_dng_*.c leaves csrc/ovl_dng_s25.c
   as the ONLY file in all seven lists, and its func_s25_800247D4 is the same
   root step one bank over: 656 words, eight states, the same walk loop, the
   same dress skeleton and one of the richest fact tables in the tree.
   func_800A44E0 alone narrows the corpus to three files. Shape matching
   cannot see a 656-against-465 pair at all.

   WHICH OF THE DONOR'S FACTS CARRIED, PRICED HERE:
     - `(cx << 6) & 0xFFFFFFC0' into func_800A44E0: HOLDS, 7 rows the other
       way against its 7. Its own reasoning reproduces exactly.
     - `*(UnkVec *)q = *b' for the mover copy: HOLDS and is the largest
       single fact here, 86 rows and six words as field stores against its
       117 and twelve.
     - the `u16 z' local across state 1's gate: HOLDS, 7 rows against its 7.
     - the retire site as `r->unk_1E |= 0x8000': HOLDS, 22 rows as a plain
       cast against its 63, and case 5's `*(u16 *)(p - 0x2) |= 0x8000' is
       the other polarity here too.
     - the two narrowing stores as `* 64': HOLDS, 7 rows as `<< 6'.
     - `oy = yy; ox = xx;' at the bottom of the walk loop: INVERTED. The
       donor records it MEASURED FREE either way; here the other order is
       21 rows, the third-largest fact on this body. It decides which of
       ox/oy keeps $fp and which spills to 0x18($sp).
     - a SEPARATE local per pointer temp: INVERTED. The donor prices one
       shared pointer at 27 rows; here ONE `UnkVec *v' shared by the lock-on
       arm and the walk arm is what lands and a separate `u8 *g' is 9 rows,
       because the shared allocno is what puts the target in $s0 and leaves
       $a0 to the /15 magic across the gate.

   FACTS MEASURED ON THIS BODY FROM THE 0 STATE, both spellings priced
   (aligned differences; every figure below was re-measured after the body
   reached 0, and three of them had read differently mid-fight):

   | spelling                                                | words | rows |
   |---------------------------------------------------------|-------|------|
   | `func_s13_800247D4(p, c)' -- TWO arguments               |  465  |   0  |
   |   ... one argument                                      |  464  |   7  |
   | the mover copy as `*(UnkVec *)(p + 0x38) = *b;'          |  465  |   0  |
   |   ... six word-at-a-time field stores                   |  471  |  86  |
   | the walk-arm zero stores DESCENDING 0x8 / 0x4 / 0x0      |  465  |   0  |
   |   ... ascending 0x0 / 0x4 / 0x8                         |  465  |   4  |
   |   ... as plain casts off p rather than through `v'      |  465  |  73  |
   | `ox = xx; oy = yy;' at the BOTTOM of the walk loop       |  465  |   0  |
   |   ... the donor's `oy = yy; ox = xx;'                    |  465  |  21  |
   | ONE `UnkVec *v' shared by both aiming arms               |  465  |   0  |
   |   ... a separate `u8 *g' in the lock-on arm             |  465  |   9  |
   | the `*(s16 *)(p + 0x5A)' store BEFORE `+0x58 = 0'        |  465  |   0  |
   |   ... the other order                                   |  465  |   2  |
   | the retire site as `r2->unk_1E |= 0x8000'                |  465  |   0  |
   |   ... a plain cast through `(u8 *)r2 + 0x1E'            |  467  |  22  |
   | `u16 z' for w+0xA across state 1's flag gate             |  465  |   0  |
   |   ... naming `*(u16 *)(w + 0xA)' in both arms           |  467  |   7  |
   | `& 0xFFFFFFC0' into func_800A44E0                        |  465  |   0  |
   |   ... `& 0xFFC0' against the same u16 prototype         |  468  |   7  |
   | `ox * 64 + 0x20' / `oy * 64 + 0x20'                      |  465  |   0  |
   |   ... `(ox << 6) + 0x20' / `(oy << 6) + 0x20'           |  463  |   7  |
   | the `v+0xA = -0x400' pre-store, read back as arg 3       |  465  |   0  |
   |   ... -0x400 passed directly with no pre-store          |  464  |   6  |

   THE LAST SEVEN ROWS WERE A CALL'S ARITY, and the diagnostic is worth
   writing down because no tool reports it. The residue was the incoming
   parameter copy `addu $a1, $a2, $zero' MISSING (one word) plus state 2's
   four mover temps sitting in $v0/$v1/$a1/$a2 where the target has
   $v0/$v1/$a2/$a3. Those are one fault: the target needs `c' in $a1 at the
   `jal func_s13_800247D4', so global.c takes the outgoing-argument copy
   preference over the incoming one, `c' is allocated $a1 instead of $a2, the
   entry copy out of $a2 survives as a real word, and $a1 is no longer free
   for state 2's block-local temps. **A skipped argument register in a
   call's block, together with a missing parameter copy in the prologue, is
   an ARITY diagnostic** -- the second argument is whichever value is already
   sitting in that register with no instruction setting it.

   THE ORDER-OF-ASSIGNMENT ROWS ARE WHY `ox' SPILLS. ox and oy have
   identical reference counts and identical uses, so `allocno_compare'
   separates them only on live length, and the length is set by which one is
   written first at the bottom of the loop. Declaration order is FREE here
   (all four permutations of `ox'/`oy' around `out[4]' measure the same),
   which rules out the pseudo-number tie-break and leaves the statement
   order as the whole fact.

   AND TWO OF THESE ROWS WERE FALSE MID-FIGHT, which is the brief's warning
   firing: the ascending zero stores read 42 against 45 while the ox/oy order
   was still wrong, and the folded block copy likewise looked cheap. Once the
   body reached 0 the ascending order is 4 rows, i.e. the improvement had
   been the other fault moving. Only the re-priced column above is evidence.

   MEASURED AND FREE, so neither is why this lands: `(u16)D_8006CCD8[dir]' in
   place of `D_8006CCD8[dir] & 0xFFFF' for the unsigned step (465 / 0 both
   ways -- the donor's own caveat says to price this per body and on this one
   it is genuinely free), and reading the heading as `*(u16 *)(p + 0xE)'
   rather than `*(s16 *)(p + 0xE)' inside func_800A44E0's fourth argument
   (465 / 0 -- `(s16)(x << 9)' discards the top seven bits, so force_to_mode
   rewrites the sign extension to the `lhu' either way).

   THE DEAD `lui $v0, %hi(D_8006CCD8)' at 80024FAC IS NOT SOURCE. It is the
   walk loop's hoisted table HIGH, left behind when the loop body's uses were
   rematerialised as full `lui'/`addiu' pairs; this body emits it without
   being asked and no spelling here was found to affect it. */
void func_s13_80024C88(u8 *p, UnkVec *b, u8 *c)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *q;
    u8 *q2;
    UnkVec *v;
    UnkNode *r;
    UnkNode *r2;
    u16 z;
    s32 i;
    s32 h;
    s32 cx;
    s32 cy;
    s16 xx;
    s16 yy;
    s16 ox;
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
        for (i = 7; i >= 0; i--)
        {
            *(s32 *)(p + 0x18 + i * 4) = 0;
        }
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
        *(UnkVec *)(p + 0x38) = *b;
        if (*(u8 **)(s + 0x60) != NULL)
        {
            v = (UnkVec *)*(u8 **)(*(u8 **)(s + 0x60) - 0x18);
            out[0] = abs(*(s16 *)((u8 *)v + 0x2) - *(s16 *)((u8 *)b + 0x2));
            out[1] = abs(*(s16 *)((u8 *)v + 0x6) - *(s16 *)((u8 *)b + 0x6));
            out[2] = abs(*(s16 *)(*(u8 **)(s + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA));
            q = *(u8 **)(*(u8 **)(s + 0x60) - 0x14);
            *(s16 *)(p + 0x68) = q[0x24];
            *(s16 *)(p + 0x6A) = q[0x25];
            *(s32 *)(p + 0x50) = *(s32 *)((u8 *)v + 0x0);
            *(s32 *)(p + 0x54) = *(s32 *)((u8 *)v + 0x4);
            *(s16 *)(p + 0x5A) = *(u16 *)(*(u8 **)(s + 0x60) + 0x88);
            *(s16 *)(p + 0x58) = 0;
            b->unk_0C = (*(s32 *)(p + 0x50) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x54) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x58) - b->unk_08) / 15;
        }
        else
        {
            q2 = *(u8 **)(o + 0xC);
            xx = q2[0x24];
            yy = q2[0x25];
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
                ox = xx;
                oy = yy;
            }
            v = (UnkVec *)(p + 0x50);
            v->unk_08 = 0;
            v->unk_04 = 0;
            v->unk_00 = 0;
            *(s16 *)((u8 *)v + 0x2) = ox * 64 + 0x20;
            *(s16 *)((u8 *)v + 0x6) = oy * 64 + 0x20;
            *(s16 *)((u8 *)v + 0xA) = -0x400;
            *(s16 *)((u8 *)v + 0xA) = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                                                    *(s16 *)((u8 *)v + 0xA));
            if (*(s16 *)((u8 *)v + 0xA) >= 0x201)
            {
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)b + 0xA) + 0x20;
            }
            b->unk_0C = (*(s32 *)(p + 0x50) - b->unk_00) / 15;
            b->unk_10 = (*(s32 *)(p + 0x54) - b->unk_04) / 15;
            b->unk_14 = (*(s32 *)(p + 0x58) - b->unk_08) / 15;
            *(s16 *)(p + 0x68) = ox;
            *(s16 *)(p + 0x6A) = oy;
        }
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        b->unk_00 = b->unk_00 + b->unk_0C;
        b->unk_04 = b->unk_04 + b->unk_10;
        b->unk_08 = b->unk_08 + b->unk_14;
        func_s13_800247D4_2(p, c);
        if (*(s16 *)(p + 0x10) >= 0xF)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            for (i = 7; i >= 0; i--)
            {
                *(s32 *)(p + 0x18 + i * 4) = (s32)func_s13_80024628(p, b, i);
            }
            func_800A56E0(0x300);
        }
        break;
    case 3:
        if (*(s16 *)(p + 0x10) >= 0x21)
        {
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            for (i = 7; i >= 0; i--)
            {
                r = *(UnkNode **)(p + 0x18 + i * 4);
                if (r != NULL)
                {
                    r->unk_10 = (u32)func_s13_800244D4;
                }
            }
        }
        break;
    case 4:
        if (*(s16 *)(p + 0x10) >= 0x11)
        {
            func_s13_80024060(*(s16 *)(p + 0x68), *(s16 *)(p + 0x6A), s, p[0x9]);
            *(s16 *)(p + 0x10) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            for (i = 7; i >= 0; i--)
            {
                r2 = *(UnkNode **)(p + 0x18 + i * 4);
                if (r2 != NULL)
                {
                    r2->unk_1E |= 0x8000;
                    D_800814A0 |= 0x8000;
                    *(s32 *)(p + 0x18 + i * 4) = 0;
                }
            }
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
