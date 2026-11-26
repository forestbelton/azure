#include "common.h"

void func_800478B8(u8 *);
s32 rand(void);
UnkNode *func_8003FD64(s32, void *);
s32 func_8004491C(UnkNode *, u32);
s32 func_800BCB04(s32, s32, s16);
u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
void func_s61_80024BB8(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s61_800246C0(u8 *p, UnkVec *b, u8 *c, u8 *d);
void func_s61_80024C68(u8 *p, UnkVec *b, u8 *c, u8 *d);
s32 func_80053EF0(s32);
s32 func_800A56E0(s32);
void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
extern u32 D_800814A0;
extern u16 D_8008378A;
extern u8 D_80045340[];
extern u8 D_80083498[];
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern u8 D_80082E80[];
extern u16 D_80082E94;
extern u8 D_80083780[];
extern u8 D_800DE9D0[];
extern u8 D_800DEC00[];
extern u8 D_800DEDB0[];
extern u8 D_800DE990[];
extern u8 D_800DED70[];

/* The bank's five-state cutscene dispatcher and the OWNER OF
   jtbl_s61_80024008 (5 words = 5 cases + 0 pad, dispatch bound 5, 5 distinct
   targets, no minval subtraction so the case set is 0..4). A JUMP TABLE on
   *(s16 *)(p + 0xA). ARM ORDER verified by standalone `tools/cdkgcc -O2 -G0
   -S': the table emits [$L3, $L4, $L8, $L12, $L31] at strictly ascending arm
   block addresses, which is the ROM's ascending entry sequence
   [80024090, 800240EC, 80024248, 80024284, 800245E0], 1:1 positional. No
   `$LC' pool is emitted (the two local arrays have no initialiser).

   State 0 clears D_800814A8's +0xF4, publishes the record's position triple
   into b and FALLS THROUGH into state 1, which decrements D_800814A8's +0xA6
   counter, parks p[8] at its +0xA8 and spawns the 0x312 node that gets
   func_s61_800246C0 as its step, offsetting b and the node's mover by
   func_8003DE58's projection and dressing the renderable from D_800DEDB0 at
   0x808080. State 2 fires func_800A56E0 with 0x4300 or 0x300 off
   func_80053EF0(4). State 3, while +0x50 is under 3, spawns TWO batches off
   one func_800BCB04 bearing -- two func_s61_80024BB8 nodes from D_800DE9D0 at
   0x606060, then ten func_s61_80024C68 nodes from D_800DEC00 at 0x101010 --
   scattering each around D_8006CCD8/D_8006CCE8's facing offsets. State 4
   retires the actor through func_8009CE1C.

   425 words, 0 differing. Donor: the landed func_s70_8002401C in
   csrc/ovl_dng_s70.c (dngcallee: 14 of 21 symbols, clean gap to the 13/21
   tier; dngtwin found NO carved sibling at 0.70). Its state 0/1/2/4 shapes
   carried verbatim, including the `(**(u16 **)(p + 0x4) & 0x80) == 0' gate,
   the `if (f() != 2)' equal-last form in state 2, the chained `out' zeroing,
   and state 4's `(D_80082E94 & 0x8000) != 0 || *(s16 *)(p + 0x50) < 0' pair.

   FOUR MEASURED FACTS, all of them worth words or rows on this body.

   1. A `(s16)' CAST ON `rand() % 32 + 0x10' IS WHAT BLOCKS fold's
      REASSOCIATION, AND IT IS FREE. Written
      `... * 64 + (rand() % 32 + 0x10)' the body is 425 words / 18 aligned
      rows: fold-const.c's `split_tree' (fold-const.c, the `if (!wins)'
      associate block) splits the inner `PLUS(mod, 16)' into var=mod, con=16
      and rebuilds the expression as `(MULT + 16) + mod', so the `addiu 16'
      lands on the multiply where the ROM has it on the modulo, and the
      rand result and its modulo copy exchange $a0/$a1 across both sites.
      `split_tree' strips a NOP_EXPR only while the MACHINE MODE is unchanged,
      so `(s16)(rand() % 32 + 0x10)' is not stripped, the split fails, and the
      tree survives as `PLUS(MULT, PLUS(mod, 16))' -- which expand_expr emits
      operand-0-first, exactly the ROM. It costs NOTHING because the value is
      stored through `*(s16 *)' anyway, so convert.c absorbs the truncation:
      425 words / 0 rows. Rejected, measured: `+ (rand() % 32 + 0x10)' 18
      rows; `+ (0x10 + rand() % 32)' 18 rows (FREE against the first, the
      operand order is not the fact); `+ rand() % 32 + 0x10' 81 rows; a
      `k = rand() % 32 + 0x10;' local 89 rows; `rand() % 32 + 0x10 + ...*64'
      reorders the whole block.
   2. THE SPRITE-TABLE STORE MUST COME AFTER THE `|= 0xC', NOT BEFORE, AND IT
      IS WORTH 46 ROWS. The dress order `0x10; |= 0xC; 0x0; 0x8' is 18 rows;
      `0x10; 0x0; |= 0xC; 0x8' -- the order five landed bodies in this family
      use -- is 64. MECHANISM, read off loop.c:1730
      (`threshold * savings * m->lifetime >= insn_count', with
      `threshold -= 3' per move): loop.c hoists exactly THREE invariants out
      of each spawn loop here, and with the `|= 0xC' between the two
      D_800DE9D0 references that symbol's lifetime is long enough to win a
      slot, which pushes the HANDLER address back into the loop as an extra
      `lui'/`addiu' pair. Moving the `|= 0xC' above both references shortens
      the table's lifetime, the handler is hoisted instead (into $fp, as the
      ROM has it), and the table is recomputed in-loop -- same word count,
      and every callee-saved name falls into place. The ROM keeps the `|= 0xC'
      between them in the EMITTED stream; that is sched, not source order.
   3. `* 64' AND `<< 6' ARE NOT THE SAME TREE HERE, worth 4 rows: the shift
      form reads the facing table with `lhu' where the ROM has `lh', because
      convert.c's LSHIFT_EXPR arm is guarded on `TREE_UNSIGNED (type)' and
      MULT_EXPR's is not. 76 rows with `<< 6', 72 with `* 64'.
   4. NO `r = rand();' LOCAL -- 12 rows. With the four `rand()' results
      inlined the body is 60 rows where a shared `s32 r' local is 72, because
      the extra pseudo re-ranks the whole spawn loop. Measured FREE on its
      own, so it is NOT the reason this lands: whether `r' exists in the first
      spawn loop only (72 either way). It is removing it from BOTH loops that
      pays.

   `(s16) * (u16 *)(D_800814A8 + 0x2A) >> 9' is the index with NO `& 7' mask
   and `s16 D_8006CCD8[]'/`s16 D_8006CCE8[]' give the ROM's `lh'; the donor
   masks and reads `u16' because its own listing does. */
void func_s61_8002401C(u8 *p, UnkVec *b)
{
    u8 *s;
    u8 *o;
    u8 *w;
    u8 *e;
    u8 *q;
    u8 *t;
    u8 *d;
    UnkNode *n;
    s32 i;
    s32 z;
    s16 out[4];

    s = *(u8 **)(p + 0x0);
    o = s - 0x20;
    *(s16 *)(p + 0x50) = *(u16 *)(p + 0x50) - 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        *(s32 *)((u8 *)b + 0x0) = *(s32 *)(*(u8 **)(o + 0x8) + 0x0);
        *(s32 *)((u8 *)b + 0x4) = *(s32 *)(*(u8 **)(o + 0x8) + 0x4);
        *(s32 *)((u8 *)b + 0x8) = *(s32 *)(*(u8 **)(o + 0x8) + 0x8);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        /* fall through */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        w = (u8 *)D_800814A8;
        *(s16 *)(p + 0x50) = 0xA;
        *(s16 *)(w + 0xA6) = *(u16 *)(w + 0xA6) - 1;
        *(u8 *)(w + 0xA8) = *(u8 *)(p + 0x8);
        n = func_8003FD64(0x312, D_80083498);
        if (n != NULL)
        {
            if (func_8003DE58(*(u8 **)(*(u8 **)(o + 0xC) + 0x8), (UnkSlot *)*(u8 **)(o + 0xC),
                              out, 0) == 0)
            {
                out[0] = out[1] = out[2] = 0;
            }
            e = (u8 *)n + 0x20;
            n->unk_10 = (u32)func_s61_800246C0;
            func_8004491C(n, (u32)D_80045340);
            q = (u8 *)n->unk_0C;
            *(s32 *)((u8 *)n->unk_08 + 0x0) = *(s32 *)((u8 *)b + 0x0) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x0) + (out[0] << 16);
            *(s32 *)((u8 *)n->unk_08 + 0x4) = *(s32 *)((u8 *)b + 0x4) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x4) + (out[1] << 16);
            *(s32 *)((u8 *)n->unk_08 + 0x8) = *(s32 *)((u8 *)b + 0x8) =
                *(s32 *)(*(u8 **)(o + 0x8) + 0x8) + (out[2] << 16);
            *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
            *(s16 *)(q + 0x10) = 0x20;
            *(u8 **)(q + 0x0) = D_800DEDB0;
            *(u16 *)(q + 0x14) |= 0xC;
            *(s32 *)(q + 0x8) = *(s32 *)(D_800DEDB0 + 0x4);
            *(u8 *)(q + 0x4) = 0;
            *(u8 *)(q + 0x5) = 0;
            *(s32 *)(q + 0xC) = 0x808080;
            *(u8 **)((u8 *)n + 0x20) = p;
            *(s16 *)(e + 0x4C) = 0;
        }
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 2:
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        if (func_80053EF0(4) != 2)
        {
            func_800A56E0(0x300);
        }
        else
        {
            func_800A56E0(0x4300);
        }
        *(s16 *)(p + 0x50) = 0x10;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if (*(s16 *)(p + 0x50) < 3)
        {
            t = D_80083780;
            z = (s16)func_800BCB04(*(u16 *)(t + 0x2), *(u16 *)(t + 0x6),
                                   *(u16 *)(t + 0xA) - 0x80);
            for (i = 1; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s61_80024BB8;
                    func_8004491C(n, (u32)D_80045340);
                    q = (u8 *)n->unk_0C;
                    e = (u8 *)n + 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) =
                        (D_80082E80[0x24] + D_8006CCD8[(s16) * (u16 *)(D_800814A8 + 0x2A) >> 9]) *
                            64 +
                        (s16)(rand() % 32 + 0x10);
                    *(s16 *)((u8 *)n->unk_08 + 0x6) =
                        (D_80082E80[0x25] + D_8006CCE8[(s16) * (u16 *)(D_800814A8 + 0x2A) >> 9]) *
                            64 +
                        (s16)(rand() % 32 + 0x10);
                    *(s16 *)((u8 *)n->unk_08 + 0xA) = z;
                    *(s16 *)(q + 0x1C) = 0x1800;
                    *(s16 *)(q + 0x1E) = 0x2000;
                    *(s16 *)(q + 0x10) = 0x60;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(u8 **)(q + 0x0) = D_800DE9D0;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DE9D0 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x606060;
                    *(u8 **)((u8 *)n + 0x20) = p;
                    *(s16 *)(e + 0x4C) = 0;
                }
            }
            for (i = 9; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s61_80024C68;
                    func_8004491C(n, (u32)D_80045340);
                    q = (u8 *)n->unk_0C;
                    e = (u8 *)n + 0x20;
                    *(s16 *)((u8 *)n->unk_08 + 0x2) =
                        (D_80082E80[0x24] + D_8006CCD8[(s16) * (u16 *)(D_800814A8 + 0x2A) >> 9]) *
                            64 +
                        rand() % 64;
                    *(s16 *)((u8 *)n->unk_08 + 0x6) =
                        (D_80082E80[0x25] + D_8006CCE8[(s16) * (u16 *)(D_800814A8 + 0x2A) >> 9]) *
                            64 +
                        rand() % 64;
                    *(s16 *)((u8 *)n->unk_08 + 0xA) = z - (rand() & 0x1F);
                    *(s32 *)((u8 *)n->unk_08 + 0x14) = -0x1A0000 - (rand() << 2);
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0x800;
                    *(s16 *)(q + 0x10) = 0x20;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(u8 **)(q + 0x0) = D_800DEC00;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DEC00 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0x101010;
                    *(u8 **)((u8 *)n + 0x20) = p;
                    *(s16 *)(e + 0x4C) = 0;
                }
            }
        }
        if (*(s16 *)(p + 0x50) > 0)
        {
            break;
        }
        *(s16 *)(p + 0x50) = 0x20;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 4:
        if ((D_80082E94 & 0x8000) != 0 || *(s16 *)(p + 0x50) < 0)
        {
            if ((*(s16 *)(p + 0x52) & 0x8000) != 0)
            {
                *(s16 *)(p + 0x52) = *(u16 *)(p + 0x52) & 0x7FFF;
            }
            else
            {
                func_8009CE1C(*(u8 **)(D_800814A8 + 0x60), 8, *(u8 *)(p + 0x9), 0xA,
                              *(s16 *)(s + 0x2A), (u32)s, 2);
                d = (u8 *)&D_80083460;
                *(s16 *)(d + 0xA) = *(u16 *)(d + 0xA) - 1;
                *(s32 *)(d + 0xC) = 0;
                *(u16 *)(p - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s61", func_s61_800246C0);

/* The 0x312 node step func_s61_8002401C's state 3 spawns two of and
   func_s61_80024C68's state 2 spawns five of: advance the mover by its own
   three velocity words, raise the actor's +0x52 flag, hand the renderable to
   func_800478B8, and retire the actor when its +0x14 reports 0x6000.

   44 words, 0 differing, ON THE FIRST COMPILE. Two donors, both landed:
   func_b33_8017406C (csrc/ovl_dng_b33.c, dngtwin 0.780) for the three-word
   velocity advance, and func_s23_800241D8 (csrc/ovl_dng_s23.c, dngtwin 0.734)
   for the `(*(u16 *)(c + 0x14) & 0x6000) != 0' retire gate. The delta from
   b33 is that the flag goes to the actor at `*(u8 **)p' + 0x52 instead of
   b33's `p + 0x48' countdown, and the gate is s23's rather than b33's.

   EVERY SPELLING IN THIS BODY IS UNMEASURED -- it landed first compile, so
   b33's recorded fact that the mover bumps must precede the call and s23's
   that the `p + 0x0' chain must come first were simply obeyed, not re-priced
   here. Do not read them as confirmed on this body. */
void func_s61_80024BB8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    *(u16 *)(*(u8 **)(p + 0x0) + 0x52) |= 0x8000;
    func_800478B8(c);
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The three-state step of the 0x312 node func_s61_8002401C's state 3 spawns
   ten of: raise the actor's +0x52 flag, consume the renderable's +0x14
   0x6000, and then in state 0 fling the mover up by 0x1A000 plus half a
   random, swap in D_800DE990 and arm a 4..7 frame countdown; state 1 climbs
   the renderable's +0x1C/+0x1E by 0xC8 a frame until the countdown expires;
   state 2 re-dresses, climbs by 0x1C000, and when the mover's +0xA clears
   func_800BCB04's bearing minus 0x20 spawns five more 0x312 nodes with
   func_s61_80024BB8 as their step before retiring the actor.

   261 words, 0 differing. Donors: func_s49_8002445C (csrc/ovl_dng_s49.c) for
   the switch/spawn skeleton -- dngcallee's 7-of-11 row, dngtwin found nothing
   at 0.70 -- and the landed func_s70_8002466C (csrc/ovl_dng_s70.c) for the
   renderable dress and the payload tail.

   TWO MEASURED FACTS.

   1. THE PAYLOAD'S +0x4C STORE NEEDS `volatile' AND THE +0x20 STORE MUST BE
      SPELLED OFF `n' -- 2 words and 2 rows. The ROM has
      `sw $v0, 0x20($s1)' / `addiu $v0, $s1, 0x20' / `sh $zero, 0x4C($v0)':
      the pointer store folds to an offset off the node, the halfword store
      keeps a computed base. Every non-volatile spelling loses the `addiu',
      because cse.c's `find_best_addr' rewrites the zero-offset MEM's bare
      register to the equivalent `(plus n 32)' -- that drops the base pseudo
      to ONE use -- and combine then folds `(plus (plus n 32) 76)' into
      `(plus n 108)' and deletes the `addiu' (watched in the `-da' `.cse2' and
      `.combine' dumps: insn 503 survives cse2 and is a NOTE_INSN_DELETED in
      combine). `volatile' sets `addr_volatile' in `find_best_addr' and
      `volatile_ok == 0' in combine, so both passes leave it alone. Rejected,
      all measured at 259 words / 12 rows: `a = n->payload;' with both stores
      through `a'; `a = (u8 *)n + 0x20;' with both stores through `a';
      the +0x4C store first (260 / 12); the +0x00 store off `n' with a
      non-volatile `a' (259 / 12). The spelling kept is the one the landed
      func_s70_8002466C uses.
   2. THE 0xC06060 STORE GOES LAST IN THE DRESS, worth 4 rows. The kept order
      is `1C/1E; 0x10; 0x0; |= 0xC; 0x8; q4; q5; 0xC'. Rejected, measured:
      `...; |= 0xC; 0xC; 0x8; q4; q5' is 259 words / 12 rows, and
      `...; |= 0xC; q4; q5; 0xC; 0x8' is 260 / 17.

      NOTE FOR WHOEVER READS BOTH BODIES: func_s61_8002401C in this unit needs
      the `|= 0xC' BEFORE its `0x0' sprite-table store and this body needs it
      AFTER. That is not an inversion of one fact -- this body's spawn loop has
      room for loop.c to hoist BOTH the handler and D_800DED70, so the table's
      lifetime does not cost anything; 8002401C's has room for three
      invariants out of four and the lifetime decides which. Read the register
      budget, not the house style.

   Every other spelling in this body is UNMEASURED -- it landed on the second
   compile and only the two facts above were ever flipped. */
void func_s61_80024C68(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *q;
    u8 *a;
    s32 i;
    s32 h;

    *(u16 *)(*(u8 **)p + 0x52) |= 0x8000;
    if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
    {
        *(u8 *)(c + 0x4) = 0;
        *(u8 *)(c + 0x5) = 0;
        *(u16 *)(c + 0x14) &= 0x9FFF;
    }
    func_800478B8(c);
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        b->unk_08 += b->unk_14;
        b->unk_14 += 0x1A000 + (rand() >> 1);
        if (c[0xC] < 0x40)
        {
            *(s32 *)(c + 0xC) += 0x40404;
        }
        if (b->unk_14 > 0)
        {
            *(u8 **)(c + 0x0) = D_800DE990;
            *(s32 *)(c + 0x8) = *(s32 *)(D_800DE990 + 0x4);
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
            *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) = 0x400;
            *(s16 *)(c + 0x10) = 0;
            *(u16 *)(c + 0x14) |= 0x2;
            *(s16 *)(p + 0x48) = (rand() & 0x3) + 4;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        *(s32 *)(c + 0xC) = 0x606060;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + 0xC8;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + 0xC8;
        if ((s16)(--*(u16 *)(p + 0x48)) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u8 **)(c + 0x0) = D_800DE990;
            *(s32 *)(c + 0x8) = *(s32 *)(D_800DE990 + 0x4);
            *(u8 *)(c + 0x4) = 0;
            *(u8 *)(c + 0x5) = 0;
        }
        b->unk_08 += b->unk_14;
        b->unk_14 += 0x1C000 + (rand() >> 1);
        if (c[0xC] < 0x80)
        {
            *(s32 *)(c + 0xC) += 0x40404;
        }
        h = (s16)func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), D_8008378A - 0x80);
        if (*(s16 *)((u8 *)b + 0xA) >= h - 0x20)
        {
            for (i = 4; i >= 0; i--)
            {
                n = func_8003FD64(0x312, D_80083498);
                if (n != NULL)
                {
                    n->unk_10 = (u32)func_s61_80024BB8;
                    func_8004491C(n, (u32)D_80045340);
                    q = (u8 *)n->unk_0C;
                    *(s32 *)((u8 *)n->unk_08 + 0x0) = b->unk_00 + (((rand() & 0x3FF) - 0x1FF) << 10);
                    *(s32 *)((u8 *)n->unk_08 + 0x4) = b->unk_04 + (((rand() & 0x3FF) - 0x1FF) << 10);
                    *(s32 *)((u8 *)n->unk_08 + 0x8) = b->unk_08 + (((rand() & 0x3FF) - 0x1FF) << 10);
                    *(s32 *)((u8 *)n->unk_08 + 0xC) = ((rand() & 0x3FF) - 0x1FF) << 10;
                    *(s32 *)((u8 *)n->unk_08 + 0x10) = ((rand() & 0x3FF) - 0x1FF) << 10;
                    *(s32 *)((u8 *)n->unk_08 + 0x14) = -((rand() & 0x3FF) << 8);
                    *(s16 *)(q + 0x1C) = *(s16 *)(q + 0x1E) = 0xC00;
                    *(s16 *)(q + 0x10) = 0x20;
                    *(u8 **)(q + 0x0) = D_800DED70;
                    *(u16 *)(q + 0x14) |= 0xC;
                    *(s32 *)(q + 0x8) = *(s32 *)(D_800DED70 + 0x4);
                    *(u8 *)(q + 0x4) = 0;
                    *(u8 *)(q + 0x5) = 0;
                    *(s32 *)(q + 0xC) = 0xC06060;
                    *(u8 **)((u8 *)n + 0x20) = *(u8 **)p;
                    a = (u8 *)n + 0x20;
                    *(volatile s16 *)(a + 0x4C) = 0;
                }
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}
