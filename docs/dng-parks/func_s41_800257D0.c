/* PARKED by agent h158, batch 36. 996 target words, 997 built words,
   119 aligned differences. One family member (dngfam: 1). THIS IS THE
   LARGEST BODY IN THE UNIT and the whole shape is derived -- what is left
   is a register map and four small scheduling clusters, not structure.

   IT OWNS A JUMP TABLE: jtbl_s41_80024038, 9 words = 9 cases + 0 pad,
   dispatch bound 9, minval 0. ENTRY 7 IS THE DEFAULT: the range check's
   `beqz' and table entry 7 both target .Ls41_80026730, which is the
   function's epilogue, so the source writes `case 0' through `case 6' and
   `case 8' with NO `case 7' and NO `default:' label at all -- falling out
   of the switch is the default. `case 0' FALLS THROUGH into `case 1'.

   WHAT IT IS. The 0x212 spawner-and-sequencer for the room's shutter
   effect, switching on the payload's +0xA state:
     0  paint the renderable white (0x808080), copy D_800267A8's 0xC bytes
        to the payload's +0xA2 and publish that as the renderable's name,
        tick D_800267B8, latch the room's facing at +0x7E from the slot's
        +0x2A, bump the state -- then FALL INTO 1;
     1  func_8003DF74 the slot's +0xC object, and unless it reports and the
        object's 0x8000 flag is clear, copy the mover's three axes from the
        +0x8 object, bias them by func_8003DF74's three out-parameters (or
        drop +0xA by 0x40 when the 0x8000 flag is set), then -- gated on the
        +0x4 object's first halfword 0x80 bit -- arm the effect once
        (D_80045340 as the script, 0x20 dimensions, white, 0x7D0 clip),
        derive the travel distance at +0x78 and the step count at +0x7B
        from D_800DDC40 and the two D_8006CC.. tables, seed the mover's
        three deltas from the +0x38 table indexed by the facing, and set
        the 5/0xC8 wobble parameters;
     2  spawn four func_s41_80025080 sparks a frame, and while
        func_800A4778 reports clear, run the wobble, spend +0x7B, and at
        zero either hand over to state 3 (raising the target's 0x800 and
        0x100000 bits and snapping the mover to it) or, with no target,
        jump to state 8 and black the renderable;
     3  run the wobble, and on the first frame draw the two 0x340-wide
        panels, then on the fourth frame spawn the four node chain --
        func_s41_80025294, func_s41_800252E4, func_s41_800251A0 and
        func_s41_80025408, the last one carrying the other three's payload
        pointers at +0x44/+0x48/+0x4C -- and advance to state 4;
     4  run the wobble for 0x78 frames, dimming the renderable by 3 a frame
        over frames 0xB..0x1E and by 4 again past 0x64;
     5  once +0x9C is set, call this unit's func_s41_8002405C with
        D_800E3D68's byte or the payload's +0x9, then advance;
     6  after 0xB frames clear the target's 0x800/0x100000/0x10000000 bits,
        whiten it and go to state 8 with +0x82 preloaded to 0x1E;
     8  after 0x1F frames, if D_800267B8 is still raised, clear it and wait
        another pass; otherwise clear D_8008346C and raise both 0x8000 kill
        bits.

   NINE SPELLINGS MEASURED ON THIS BODY, each one a step down from 566:

   | change                                                      | aligned |
   |-------------------------------------------------------------|---------|
   | twin-style first draft (ovl_dng_s37 head + m2c structure)    |   493  |
   | + the `goto L35' shared state-8 exit in case 2               |   471  |
   | + `wd = 0x60' before case 3's gate                           |   328  |
   | + descending colour bytes, one local per node mover          |   306  |
   | + `s32 wd' for group 1 only, literal for group 2             |   295  |
   | + a short-lived local for the bearing difference             |   160  |
   | + the +0x10 store BETWEEN the two +0x14 flag updates         |   158  |
   | + the +0x6 zero store written AFTER the flag block (n1, n3)  |   127  |
   | + case 0's facing folded into one local, colours after +0x22 |   119  |

   THE FIVE MECHANISMS BEHIND THOSE ROWS, each measured here:

   * A SHARED STATE EXIT IS A `goto' INTO THE ARM THAT CONTAINS IT. Case 2
     reaches the state-8 block from two places -- func_800A4778 reporting
     non-zero and the +0x60 target being NULL -- and the target emits that
     block AFTER the state-3 block and BEFORE the mover-advance block. The
     only arrangement that produces that order is the label living inside
     the `if (k <= 0)' arm with the func_800A4778 test jumping into it; as
     an `else' arm the mover-advance block comes out first, 22 rows.
   * A CONSTANT WHOSE DEF IS IN A DIFFERENT BASIC BLOCK FROM ITS USES LOSES
     ITS REGISTER. Case 3's RECT width 0x60 is stored twice. Written as a
     literal in both groups it is ONE cse quantity (insert_regs merges two
     same-mode registers equivalent to one constant), it crosses the
     func_800B8FC8 call, takes a ninth callee-saved register -- $fp -- and
     every long-lived value in the function shifts one register along: 143
     rows. Assigning it to a local BEFORE the `if (+0x82 == 1)' gate makes
     it a global allocno whose live length update_equiv_regs has DOUBLED,
     so it loses the competition and reload rematerialises it at each use,
     which is what the target does. Hoisting the same local further, to
     before the switch, is 69 rows on the sibling body -- the fix is priced
     per block, not per unit.
   * THE +0x10 DIMENSION STORE MUST SIT BETWEEN THE TWO +0x14 FLAG STORES.
     With it before them, flow.c's `last_mem_set' test (flow.c insn_dead_p)
     sees two stores to +0x14 with no intervening memory reference and
     DELETES the first, which both loses a word and lets combine fold the
     two ors into one `ori 0x8C'. One store of a different field between
     them is all it takes to keep both.
   * THE +0x6 ZERO STORE GOES AFTER THE FLAG BLOCK, NOT BEFORE IT. Written
     first, the whole +0x10 constant/store pair schedules up to the top of
     the block, the flag value stays in one register and the target's
     `addu $v1, $v0, $zero' copy never appears: one word per spawn and 3
     rows each.
   * A LONG-LIVED LOCAL REUSED AS A SCRATCH IS A REGISTER TOO MANY. Case
     1's bearing difference was written into the same `s32 i' the case-2
     spawn loop counts with; that put it in a callee-saved register, let
     reorg steal the following `sll' into a `bgez' delay slot the target
     leaves empty, and cost 9 rows on its own.

   WHAT IS LEFT, named: the 119 rows are (a) caller-saved register choices
   in case 0 and case 1 -- the facing temp takes $a0 where the target has
   $v0, and the constant 1 takes $v0 where the target has $v1 -- (b) the
   evaluation order of the two loads in case 1's +0x78 statement, where the
   target reads the -0x18 object before the +0x13 byte and this body reads
   them the other way round, (c) one word over: the four spawns want the
   `addu' copy in some and not others and I have not separated which, and
   (d) the position of the three colour-byte stores relative to the +0x22
   division in each spawn. None of those is structural; the body's control
   flow, its arity, its table and all nine arms are settled.

   Also measured and NOT the answer: the mover pointer load written before
   the two payload zero stores in all four spawns is 4 extra words (it is
   the scheduler's hoist, not a source order); `t2 == 1' and `+0x82 == 4'
   as the two gates are both plain `==' tests and neither wants an s16
   local; and the two `0x1004'/`0x1000' clip pairs are free in either order.

DECLARATIONS
extern u8 D_s41_80024004[];
extern u8 D_s41_80024014[];
extern u8 D_800267A8[];
extern u8 D_80026760[];
extern u8 D_8002676C[];
extern u8 D_80026778[];
extern u8 D_80026784[];
extern u8 D_80045340[];
extern u8 D_800DDC40[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_800267B8;
extern s32 D_8008346C;
extern u8 D_800E3D68;
s32 func_8003DF74(u8 *, UnkSlot *, s16 *, s16);
UnkNode *func_8003FC64(s32);
s32 func_8004491C(UnkNode *, u32);
s16 func_800A4778(u16, u16, s16, u8 *);
s32 func_800A56E0(s32);
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
void func_s41_80025080(UnkNode *a, s16 b, u32 c, s16 d, s16 dx, s16 dy, s16 dz);
void func_s41_8002405C(u8 *p, u8 b, u8 *q);
void func_s41_800251A0(u8 *p);
void func_s41_800252E4(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s41_80025408(u8 *p, UnkVec *b, u8 *c);
void func_s41_80025294(u8 *p);
typedef struct UnkDngS58Rec
{
    s16 a;
    u16 b;
} UnkDngS58Rec;
typedef struct UnkDngS58Blk32
{
    u8 b[0x20];
} UnkDngS58Blk32;
typedef struct UnkDngS58Name12
{
    u8 b[0xC];
} UnkDngS58Name12;
END DECLARATIONS
*/
void func_s41_800257D0(u8 *p, UnkVec *b, u8 *c)
{
    s16 out[3];
    RECT r;
    DVECTOR pos;
    UnkDngS58Rec tab[8];
    u8 *g;
    u8 *nd;
    u8 *m;
    u8 *e;
    u8 *q;
    UnkNode *n1;
    UnkNode *n2;
    UnkNode *n3;
    UnkNode *n4;
    u8 *s0;
    u8 *s2;
    s32 i;
    s16 t;
    s16 t2;
    u16 w;
    s8 k;
    u8 kk;
    s32 wd;
    UnkVec *mv;
    s32 d;

    g = *(u8 **)p;
    r = *(RECT *)D_s41_80024004;
    *(UnkDngS58Blk32 *)tab = *(UnkDngS58Blk32 *)D_s41_80024014;
    nd = g - 0x20;
    m = *(u8 **)(nd + 0x8);
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(u32 *)(c + 0xC) = 0x808080;
        *(UnkDngS58Name12 *)(p + 0xA2) = *(UnkDngS58Name12 *)D_800267A8;
        *(u8 **)(c + 0x8) = p + 0xA2;
        t = (*(u16 *)(g + 0x2A) >> 9) & 7;
        D_800267B8 = 1;
        *(s16 *)(p + 0x7E) = t;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
    case 1:
        if (func_8003DF74(*(u8 **)(*(u8 **)(nd + 0xC) + 0x8), *(UnkSlot **)(nd + 0xC), out, 0) != 0 ||
            (*(u16 *)(*(u8 **)(nd + 0xC) + 0x14) & 0x8000) != 0)
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(m + 0x2);
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(m + 0x6);
            t = *(u16 *)(m + 0xA);
            *(s16 *)((u8 *)b + 0xA) = t;
            if ((*(u16 *)(*(u8 **)(nd + 0xC) + 0x14) & 0x8000) == 0)
            {
                *(s16 *)((u8 *)b + 0x2) = *(u16 *)((u8 *)b + 0x2) + out[0];
                *(s16 *)((u8 *)b + 0x6) = *(u16 *)((u8 *)b + 0x6) + out[1];
                *(s16 *)((u8 *)b + 0xA) = *(u16 *)((u8 *)b + 0xA) + out[2];
            }
            else
            {
                *(s16 *)((u8 *)b + 0xA) = t - 0x40;
            }
            if ((**(u16 **)(p + 0x4) & 0x80) != 0)
            {
                if ((p[0x7A] & 4) == 0)
                {
                    func_8004491C((UnkNode *)(p - 0x20), (u32)D_80045340);
                    *(s16 *)(c + 0x10) = 0x20;
                    c[0xE] = 0x80;
                    c[0xD] = 0x80;
                    c[0xC] = 0x80;
                    *(s16 *)(c + 0x1E) = 0x7D0;
                    *(s16 *)(c + 0x1C) = 0x7D0;
                    *(u16 *)(c + 0x14) |= 0xC;
                    p[0x7A] |= 4;
                }
                if (*(u8 **)(g + 0x60) != NULL)
                {
                    *(s16 *)(p + 0x78) = *(u16 *)(*(u8 **)(*(u8 **)(g + 0x60) - 0x18) + 0xA) -
                                         (D_800DDC40[(*(u8 **)(g + 0x60))[0x13]] + 0x40);
                    q = *(u8 **)(g - 0x14);
                    p[0xBA] = q[0x24] + *(u8 *)&D_8006CCD8[*(s16 *)(p + 0x7E)];
                    p[0xBB] = q[0x25] + *(u8 *)&D_8006CCE8[*(s16 *)(p + 0x7E)];
                    if (*(s8 *)(g + 0x72) == q[0x24])
                    {
                        d = *(s8 *)(g + 0x73) - q[0x25];
                    }
                    else
                    {
                        d = *(s8 *)(g + 0x72) - q[0x24];
                    }
                    if (d < 0)
                    {
                        d = -d;
                    }
                    p[0x7B] = d * 2 - 1;
                }
                else
                {
                    *(s16 *)(p + 0x78) = *(u16 *)(g + 0x88) - 0x50;
                    p[0x7B] = 0x20;
                }
                b->unk_0C = tab[*(s16 *)(p + 0x7E)].a << 16;
                b->unk_10 = tab[*(s16 *)(p + 0x7E)].b << 16;
                b->unk_14 = ((*(s16 *)(p + 0x78) << 16) - b->unk_08) / *(s8 *)(p + 0x7B);
                *(s16 *)(p + 0x82) = 0;
                *(s16 *)(p + 0x90) = 0;
                *(s16 *)(p + 0x92) = 0;
                *(s16 *)(p + 0x94) = 5;
                *(s16 *)(p + 0x96) = 0xC8;
                *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
                *(s16 *)(p + 0x88) = *(s8 *)(p + 0x7B);
                *(s16 *)(p + 0x8A) = *(s8 *)(p + 0x7B) * *(s8 *)(p + 0x7B);
            }
        }
        break;
    case 2:
        for (i = 0; i < 4; i++)
        {
            func_s41_80025080((UnkNode *)(p - 0x20), *(s16 *)(p + 0x7E), 0xE02020, (rand() & 0xFF) | 0x80, 0, 0, 0);
        }
        if (func_800A4778(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA),
                          *(u8 **)(g + 0x60)) != 0)
        {
            goto L35;
        }
        {
            w = *(u16 *)(c + 0x1A) + 0x190;
            *(s16 *)(c + 0x1A) = w;
            if (w > 0x1000)
            {
                *(s16 *)(c + 0x1A) = w - 0x1000;
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
            t2 = *(u16 *)(p + 0x92) + 1;
            *(s16 *)(p + 0x92) = t2;
            if (t2 >= *(s16 *)(p + 0x94))
            {
                *(s16 *)(p + 0x92) = 0;
                *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
            }
            k = *(u8 *)(p + 0x7B) - 1;
            *(s8 *)(p + 0x7B) = k;
            if (k <= 0)
            {
                if (*(u8 **)(g + 0x60) != NULL)
                {
                    *(s16 *)(p + 0xA) = 3;
                    *(s16 *)(p + 0x82) = 0;
                    func_800A56E0(0x300);
                    q = *(u8 **)(*(u8 **)(g + 0x60) - 0x14);
                    if ((*(u8 **)(g + 0x60))[0x13] < 0x33 || (*(u8 **)(g + 0x60))[0x13] > 0x36)
                    {
                        *(u16 *)(q + 0x14) |= 0x800;
                        *(s32 *)(*(u8 **)(g + 0x60) + 0x14) |= 0x100000;
                    }
                    e = *(u8 **)(*(u8 **)(g + 0x60) - 0x18);
                    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(e + 0x2);
                    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(e + 0x6);
                    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(p + 0x78);
                    break;
                }
            L35:
                *(s16 *)(p + 0xA) = 8;
                *(s16 *)(p + 0x82) = 0;
                c[0xE] = 0;
                c[0xD] = 0;
                c[0xC] = 0;
                break;
            }
            b->unk_00 += b->unk_0C;
            b->unk_04 += b->unk_10;
            b->unk_08 += b->unk_14;
        }
        break;
    case 3:
        w = *(u16 *)(c + 0x1A) + 0x190;
        *(s16 *)(c + 0x1A) = w;
        if (w > 0x1000)
        {
            *(s16 *)(c + 0x1A) = w - 0x1000;
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
        t2 = *(u16 *)(p + 0x92) + 1;
        *(s16 *)(p + 0x92) = t2;
        if (t2 >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        t2 = *(u16 *)(p + 0x82) + 1;
        *(s16 *)(p + 0x82) = t2;
        if (t2 == 1)
        {
            r.y = 0x154;
            wd = 0x60;
            r.w = wd;
            r.x = 0x340;
            r.h = 0x54;
            pos.vx = 0x370;
            pos.vy = 0x19A;
            func_800B8FC8(*(u8 **)(g + 0x60), (s32 *)&r, (s32 *)&pos, 1, t2);
            r.y = 0x1A8;
            r.w = 0x60;
            r.x = 0x340;
            r.h = 0x54;
            pos.vx = 0x370;
            pos.vy = 0x1EF;
            func_800B8FC8(*(u8 **)(g + 0x60), (s32 *)&r, (s32 *)&pos, 1, t2);
        }
        if (*(s16 *)(p + 0x82) == 4)
        {
            n1 = func_8003FC64(0x212);
            if (n1 != NULL)
            {
                s2 = (u8 *)n1 + 0x20;
                *(s16 *)(s2 + 0x2) = 0x78;
                n1->unk_10 = (u32)func_s41_80025294;
                func_8004491C(n1, (u32)D_80045340);
                s0 = *(u8 **)((u8 *)n1 + 0xC);
                *(u16 *)(s0 + 0x14) |= 0xC;
                *(s16 *)(s0 + 0x10) = 0x40;
                *(u16 *)(s0 + 0x14) |= 0x80;
                *(s16 *)(s0 + 0x6) = 0;
                *(s16 *)(s2 + 0xA) = 0;
                *(s16 *)(s2 + 0xC) = 0;
                mv = n1->unk_08;
                e = *(u8 **)(*(u8 **)(g + 0x60) - 0x18);
                mv->unk_00 = *(s32 *)(e + 0x0);
                mv->unk_04 = *(s32 *)(e + 0x4);
                mv->unk_08 = *(s32 *)(e + 0x8);
                s0 = *(u8 **)((u8 *)n1 + 0xC);
                *(s16 *)(s0 + 0x1C) = 0x1004;
                *(s16 *)(s0 + 0x1E) = 0x1004;
                *(s16 *)(s0 + 0x22) = -D_800DDC40[(*(u8 **)(g + 0x60))[0x13]] / 2;
                s0[0xE] = 0x80;
                s0[0xD] = 0x80;
                s0[0xC] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n1 + 0x40) = *(UnkDngS58Name12 *)D_8002676C;
                *(u8 **)(s0 + 0x8) = (u8 *)n1 + 0x40;
                *(s16 *)(*(u8 **)(s0 + 0x8) + 0x4) = GetTPage(2, 1, 0x340, 0x154);
            }
            n2 = func_8003FC64(0x212);
            if (n2 != NULL)
            {
                s2 = (u8 *)n2 + 0x20;
                *(s16 *)(s2 + 0x2) = 0x8C;
                n2->unk_10 = (u32)func_s41_800252E4;
                func_8004491C(n2, (u32)D_80045340);
                s0 = *(u8 **)((u8 *)n2 + 0xC);
                *(u16 *)(s0 + 0x14) &= 0xFFF3;
                *(s16 *)(s0 + 0x10) = 0x20;
                *(u16 *)(s0 + 0x14) |= 0x80;
                *(u8 **)(s2 + 0x38) = g;
                *(u8 **)(s2 + 0x40) = p;
                *(u8 **)(s2 + 0x3C) = *(u8 **)(g + 0x60);
                *(s16 *)(s0 + 0x6) = 0;
                *(s16 *)(s2 + 0xA) = 0;
                *(s16 *)(s2 + 0xC) = 0;
                mv = n2->unk_08;
                e = *(u8 **)(*(u8 **)(g + 0x60) - 0x18);
                mv->unk_00 = *(s32 *)(e + 0x0);
                mv->unk_04 = *(s32 *)(e + 0x4);
                mv->unk_08 = *(s32 *)(e + 0x8);
                s0 = *(u8 **)((u8 *)n2 + 0xC);
                *(s16 *)(s0 + 0x1E) = 0x1000;
                *(s16 *)(s0 + 0x1C) = 0x1000;
                s0[0xE] = 0x80;
                s0[0xD] = 0x80;
                s0[0xC] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n2 + 0x40) = *(UnkDngS58Name12 *)D_80026778;
                *(u8 **)(s0 + 0x8) = (u8 *)n2 + 0x40;
                *(s16 *)(*(u8 **)(s0 + 0x8) + 0x4) = GetTPage(2, 1, 0x340, 0x1A8);
            }
            n3 = func_8003FC64(0x212);
            if (n3 != NULL)
            {
                s2 = (u8 *)n3 + 0x20;
                *(s16 *)(s2 + 0x2) = 0x78;
                n3->unk_10 = (u32)func_s41_800251A0;
                func_8004491C(n3, (u32)D_80045340);
                s0 = *(u8 **)((u8 *)n3 + 0xC);
                *(u16 *)(s0 + 0x14) &= 0xFFF3;
                *(s16 *)(s0 + 0x10) = 0x20;
                *(u16 *)(s0 + 0x14) |= 0x80;
                *(s16 *)(s0 + 0x6) = 0;
                *(s16 *)(s2 + 0xA) = 0;
                *(s16 *)(s2 + 0xC) = 0;
                mv = n3->unk_08;
                e = *(u8 **)(*(u8 **)(g + 0x60) - 0x18);
                mv->unk_00 = *(s32 *)(e + 0x0);
                mv->unk_04 = *(s32 *)(e + 0x4);
                mv->unk_08 = *(s32 *)(e + 0x8);
                s0 = *(u8 **)((u8 *)n3 + 0xC);
                *(s16 *)(s0 + 0x1C) = 0x1000;
                *(s16 *)(s0 + 0x1E) = 0x1000;
                s0[0xE] = 0x80;
                s0[0xD] = 0x80;
                s0[0xC] = 0x80;
                for (i = 0; i < 2; i++)
                {
                    *(UnkDngS58Name12 *)(s2 + i * 0xC + 0x20) = *(UnkDngS58Name12 *)D_80026784;
                }
                for (i = 1; i < 9; i++)
                {
                    *(s16 *)(s2 + i * 2 + 0x50) = i << 9;
                }
                *(u8 **)(s0 + 0x8) = s2 + 0x20;
            }
            n4 = func_8003FC64(0x212);
            if (n4 != NULL)
            {
                s2 = (u8 *)n4 + 0x20;
                *(u8 **)(s2 + 0x38) = g;
                *(u8 **)(s2 + 0x44) = (u8 *)n1 + 0x20;
                *(u8 **)(s2 + 0x48) = (u8 *)n3 + 0x20;
                *(u8 **)(s2 + 0x4C) = (u8 *)n2 + 0x20;
                *(u8 **)(s2 + 0x40) = p;
                *(u8 **)(s2 + 0x3C) = *(u8 **)(g + 0x60);
                n4->unk_10 = (u32)func_s41_80025408;
                func_8004491C(n4, (u32)D_80045340);
                s0 = *(u8 **)((u8 *)n4 + 0xC);
                *(u16 *)(s0 + 0x14) |= 0xC;
                *(s16 *)(s0 + 0x10) = 0x20;
                *(u16 *)(s0 + 0x14) |= 0x80;
                *(s16 *)(s0 + 0x6) = 0;
                *(s16 *)(s2 + 0xA) = 0;
                *(s16 *)(s2 + 0xC) = 0;
                mv = n4->unk_08;
                e = *(u8 **)(*(u8 **)(g + 0x60) - 0x18);
                mv->unk_00 = *(s32 *)(e + 0x0);
                mv->unk_04 = *(s32 *)(e + 0x4);
                mv->unk_08 = *(s32 *)(e + 0x8);
                s0 = *(u8 **)((u8 *)n4 + 0xC);
                *(s16 *)(s0 + 0x1E) = 0x1000;
                *(s16 *)(s0 + 0x1C) = 0x1000;
                s0[0xE] = 0x80;
                s0[0xD] = 0x80;
                s0[0xC] = 0x80;
                *(UnkDngS58Name12 *)((u8 *)n4 + 0x40) = *(UnkDngS58Name12 *)D_80026760;
                *(u8 **)(s0 + 0x8) = (u8 *)n4 + 0x40;
            }
            if (*(s16 *)(p + 0x82) == 4)
            {
                *(s16 *)(p + 0xA) = *(s16 *)(p + 0x82);
                *(s16 *)(p + 0x82) = 0;
                *(s16 *)(p + 0x9C) = 0;
            }
        }
        break;
    case 4:
        *(s16 *)(p + 0x82) = *(u16 *)(p + 0x82) + 1;
        for (i = 0; i < 4; i++)
        {
        }
        w = *(u16 *)(c + 0x1A) + 0x190;
        *(s16 *)(c + 0x1A) = w;
        if (w > 0x1000)
        {
            *(s16 *)(c + 0x1A) = w - 0x1000;
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
        t2 = *(u16 *)(p + 0x92) + 1;
        *(s16 *)(p + 0x92) = t2;
        if (t2 >= *(s16 *)(p + 0x94))
        {
            *(s16 *)(p + 0x92) = 0;
            *(s16 *)(p + 0x90) = *(u16 *)(p + 0x90) ^ 1;
        }
        if (*(u16 *)(p + 0x82) >= 0xB && *(u16 *)(p + 0x82) <= 0x1E)
        {
            kk = c[0xE] - 3;
            c[0xE] = kk;
            c[0xD] = kk;
            c[0xC] = kk;
        }
        if (*(s16 *)(p + 0x82) >= 0x64 && c[0xC] >= 6)
        {
            kk = c[0xE] - 4;
            c[0xE] = kk;
            c[0xD] = kk;
            c[0xC] = kk;
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
            if (D_800E3D68 == 0xFF)
            {
                func_s41_8002405C(*(u8 **)(g + 0x60), 0xFF, g);
            }
            else
            {
                func_s41_8002405C(*(u8 **)(g + 0x60), p[0x9], g);
            }
            *(s16 *)(p + 0x82) = 0;
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        }
        break;
    case 6:
        t2 = *(u16 *)(p + 0x82) + 1;
        *(s16 *)(p + 0x82) = t2;
        if (t2 >= 0xB)
        {
            q = *(u8 **)(*(u8 **)(g + 0x60) - 0x14);
            *(u16 *)(q + 0x14) &= 0xF7FF;
            *(s32 *)(*(u8 **)(g + 0x60) + 0x14) &= 0xFFEFFFFF;
            *(s32 *)(*(u8 **)(g + 0x60) + 0x1C) &= 0xEFFFFFFF;
            q[0xC] = 0x80;
            q[0xD] = 0x80;
            q[0xE] = 0x80;
            *(s16 *)(p + 0xA) = 8;
            *(s16 *)(p + 0x82) = 0x1E;
        }
        break;
    case 8:
        t = *(u16 *)(p + 0x82);
        t2 = t + 1;
        *(s16 *)(p + 0x82) = t2;
        if (t2 >= 0x1F)
        {
            *(s16 *)(p + 0x82) = t;
            if (D_800267B8 != 0)
            {
                D_800267B8 = 0;
            }
            else
            {
                D_8008346C = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}
