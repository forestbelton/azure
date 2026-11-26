/* PARK: func_s34_80024714, csrc/ovl_dng_s34.c, agent-h242, 2026-11-26.

   TARGET 677 words. BEST BUILT 678 words, 151 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py, R rows INCLUDED); ovlverify reports 527 masked
   on the same body, which is the word-against-word count and is noise here.
   1 member, 677 words (tools/decomp/dngfam.py) -- no replication value.

   IT OWNS NO TABLE. jtcheck reports one table for this unit and it belongs to
   func_s34_80024020 (LANDED). There is no aggregate initialiser in this body
   and a standalone compile emits no $LC pool, so there is no container risk
   of the kind that refused a 1366-word body on 2026-11-25.

   WHAT IT IS. A shadow/decal renderer registered through func_8004491C by
   func_s34_80024020's second spawn (the 0x201 node), whose step is the landed
   func_s34_80024684. It walks the tag chain off p - 0x8, and per node:
   builds four SVECTORs at sp+0x18 (stride 8) as the corners of a quad
   (vx = +-0x30 on i < 2, vy = 0, vz = -0x80 on odd i else 0x10); hands
   func_800DBA90 a 0x20-byte record whose +0xC is D_80083160's +0xB8 minus its
   +0xC8 and whose +0x10 is an 8-byte copy of q + 0x4; projects the four
   corners through RotTransPers accumulating (depth - 8) and the x/y min and
   max; turns the min/max into a RECT at sp+0x10; and then, only while the
   mean depth is under 0x1E0, hangs four bands of semi-transparent POLY_FT4
   quads (one band per `w' in 4..1, each band one quad per OT row from `lo' to
   `hi') plus -- when `lo' is 0 -- five jittered quads, two func_80067EF4
   packets, two SetDrawMove packets and one more plain quad, all linked with
   addPrim onto D_80083160's +0xB0 OT row given by the mean depth.

   VERDICT: ALLOC-DOMINATED BUT NOT PURE ALLOC. The word count is one over
   and the five spill slots do not line up: the target spills to 0x5C (`w',
   which is ADDRESS-TAKEN because RotTransPers gets it twice), 0x60 (`q'),
   0x64 (`acc'), 0x68 (the `vecs' base) and 0x6C (`acc * 4'), with a frame of
   0x98 and ten saves (s0..s7, fp, ra). This body builds a frame of 0xA0, so
   it has two slots too many. Everything else -- every opcode, immediate and
   displacement in the four dress blocks -- matches.

   THE MEASUREMENT TABLE. Each row is one change applied to the row above it,
   measured with aligndiff (words / aligned rows, R included):

     first draft from the listing                    639 / 461
     + quad pairing u0=u1,u2=u3,v0=v2,v1=v3          641 / 426
       and y0=y2,y1=y3 (NOT the s68 twin's pairing)
     + mn/mx as a 16-bit SIGNED BITFIELD PAIR        678 / 293   <- worth 37
       instead of DVECTOR                                           words
     + locals declared in stack-slot order and      677 / 148
       the mn/mx init moved AFTER func_800DBA90
     + divisor 224 (not 7), `q = p' at the loop     678 / 151
       top, colour as (d<<16)+(d<<8)+d                              <- KEPT
     - colour reverted to `* 0x10101'                678 / 151
       (the (d<<16)+(d<<8)+d form with a shared
        local `d' was 678 / 162, so 11 rows)
     + mn.vy read ONCE into a local and shared       678 / 182   <- REJECTED,
       between box.y and box.h                                      31 rows

   THE 148-ROW ROW IS A TRAP AND IS NOT THE BEST BODY. It scores better than
   the 151 kept below but it divides by 7 where the target divides by 224, so
   its shape is provably wrong. The discriminator is the SHIFT after
   `mult'/`mfhi'/`addu': magic 0x92492493 with `sra 7' is / 224 (= 7 * 2^5),
   and the same magic with `sra 2' is / 7. Everywhere the listing shows
   `sra 7' this body now writes `% 224' or `/ 224 * 224'. Score is not
   correctness -- see docs/landing-a-function.md 4b.

   FIVE FACTS, MEASURED ON THIS BODY.

   * THE MIN/MAX ACCUMULATORS ARE A SIGNED BITFIELD PAIR, NOT A DVECTOR, AND
     IT IS WORTH 37 WORDS AND 133 ROWS. The target keeps each of them wholly
     in one register ($s7 and $fp) with `sll 16'/`sra 16' to read .vx,
     `sra 16' to read .vy, `andi 0xFFFF'+`or' to write .vy and
     `and 0xFFFF0000'+`ori' to write .vx -- that is gcc's store_field and
     extract_bit_field acting on a REG. `struct { s32 vx : 16; s32 vy : 16; }'
     reproduces it instruction for instruction, including the hoisting of the
     0xFFFF0000 mask into a callee-saved register. A two-field `short' struct
     -- which is what DVECTOR is -- is forced to the stack by this compiler;
     verified with a 23-line standalone probe (`vars= 8', every access
     $sp-relative). This is now in docs/matching-levers.md.

   * THE STACK SLOTS COME OUT IN DECLARATION ORDER, and getting that order
     wrong is worth 145 rows on this body. The target's addressable locals
     ascend box(0x10), vecs(0x18), rec(0x38), out(0x58), w(0x5C), so they
     must be DECLARED in exactly that order. Declaring `out' before `box'
     put `out' at 0x10 and shifted every $sp displacement in the body.

   * `w' IS ONE VARIABLE DOING THREE JOBS and it is address-taken, which is
     why it is in memory for all of them. It holds 0x30 for the corner
     builder (read back as `lhu 0x5C($sp)'), is passed BY ADDRESS to
     RotTransPers as both scratch arguments, and is then the 4..1 band
     counter. An address-taken local cannot share a spill slot with a
     spilled pseudo, so the three uses at 0x5C(sp) are necessarily the same
     declared variable -- that is evidence, not a guess.

   * THE TARGET MAKES A SECOND COPY OF `p' AT THE TOP OF EVERY ITERATION.
     0x98(sp) is the parameter (reassigned at the chain walk) and 0x60(sp) is
     written from it at the head of the while(1) and is what every q-relative
     read inside the body goes through. That is the s68 twin's `q = p;' and
     it is kept here, although in combination with the other two changes of
     its batch it measured 3 rows worse than the wrong-divisor body.

   * THE QUAD PAIRING IS NOT THE s68 TWIN'S. Here vertices 0,1 are the LEFT
     edge and 2,3 the RIGHT (x0=x1, x2=x3, u0=u1, u2=u3) while 0,2 are the
     TOP and 1,3 the BOTTOM (y0=y2, y1=y3, v0=v2, v1=v3).
     csrc/ovl_dng_s68.c func_s68_80024B20 uses the other convention. Worth
     35 rows and 2 words to get wrong.

   WHERE TO START. The residue is five spill slots and two too many of them.
   The ladder in scratch/azure-dng/BRIEF-TEMPLATE.md says to look for one
   local too many first: the candidates are `d', `i', `col', `ang' and `y',
   several of which this body reuses across arms, and the two rejected rows
   above show this body punishes sharing. `.lreg'/`.greg' on a single-function
   probe (a whole-unit -da compile dumps only the FIRST function) will say
   which pseudos the target spilled.

   PROVENANCE. No carved sibling at ratio >= 0.70 (dngtwin). Written from the
   listing plus csrc/ovl_dng_s68.c func_s68_80024B20 (the tag-chain walk, the
   +0x8D0 cursor, the `u8 rec[0x20]' func_800DBA90 record and the
   `RotTransPers(...) - 8' depth accumulation) and csrc/ovl_dng_s41.c
   func_s41_* (the `D_80083160.unk_00 != D_801C9E40' lean test).
   func_80065420 is RotTransPers, func_800666F4 SetPolyFT4, func_80066640
   SetSemiTrans, func_80066460 GetTPage, func_80066890 SetDrawMove and
   func_80069EF8 rand, all out of splat/common_syms.txt. func_80067EF4,
   func_800644B8 (a trig lookup: its result is used `>> 10') and
   func_800DBA90 are not in common_syms and keep their addresses.

   DECLARATIONS THIS BODY NEEDS that csrc/ovl_dng_s34.c does NOT already
   carry (they were removed from the unit when the stub was restored, so a
   future landing must add them back):

     s32 rand(void);
     void func_800DBA90(u8 *);
     s32 func_800644B8(s32);
     void func_80067EF4(void *, s32, s32);
     extern u8 D_801C9E40[];

   plus the typedef below. */

typedef struct UnkDng24714MinMax
{
    s32 vx : 16;
    s32 vy : 16;
} UnkDng24714MinMax;

s32 func_s34_80024714(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    UnkNode *n;
    POLY_FT4 *t;
    UnkDng24714MinMax mn;
    UnkDng24714MinMax mx;
    RECT box;
    SVECTOR vecs[4];
    u8 rec[0x20];
    DVECTOR out;
    s32 w;
    s32 acc;
    s32 ac4;
    s32 i;
    s32 lo;
    s32 hi;
    s32 col;
    s32 row;
    s32 ang;
    s32 y;
    s32 d;

    acc = 0;
    while (1)
    {
        q = p;
        w = 0x30;
        for (i = 3; i >= 0; i--)
        {
            vecs[i].vx = w;
            if (i < 2)
            {
                vecs[i].vx = -w;
            }
            vecs[i].vy = 0;
            if ((i & 1) != 0)
            {
                vecs[i].vz = -0x80;
            }
            else
            {
                vecs[i].vz = 0x10;
            }
        }
        *(u8 **)rec = *(u8 **)(rec + 0x4) = (u8 *)vecs;
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        *(s16 *)(rec + 0xC) = g->unk_B8 - g->unk_C8;
        memcpy(rec + 0x10, q + 0x4, 8);
        *(s16 *)(rec + 0x18) = 4;
        *(s16 *)(rec + 0x1A) = 0;
        func_800DBA90(rec);
        mn.vx = mn.vy = 0x7530;
        mx.vx = mx.vy = -0x7530;
        for (i = 3; i >= 0; i--)
        {
            acc += RotTransPers(&vecs[i], (long *)&out, (long *)&w, (long *)&w) - 8;
            if (mx.vx < out.vx)
            {
                mx.vx = out.vx;
            }
            else if (out.vx < mn.vx)
            {
                mn.vx = out.vx;
            }
            if (mx.vy < out.vy)
            {
                mx.vy = out.vy;
            }
            else if (out.vy < mn.vy)
            {
                mn.vy = out.vy;
            }
        }
        box.x = mn.vx;
        box.w = mx.vx - mn.vx;
        acc >>= 2;
        y = mn.vy;
        if (g->unk_00 != D_801C9E40)
        {
            y += 0xE0;
        }
        box.y = y;
        box.h = mx.vy - mn.vy;
        if ((u32)acc < 0x1E0)
        {
            hi = box.y + box.h;
            if (box.y / 224 != hi / 224)
            {
                hi = (hi / 224) * 224;
            }
            if (*(s16 *)&g->unk_B8 != 0)
            {
                i = 0x28 - *(s16 *)(q + 0x10);
                if (i >= 9)
                {
                    i = 8;
                }
                lo = box.y;
                hi = (box.h * i) / 8 + lo;
            }
            else
            {
                i = *(s16 *)(q + 0x10) - 0x20;
                if (i < 0)
                {
                    i = 0;
                }
                lo = (box.h * i) / 8 + box.y;
            }
            w = 4;
            ac4 = acc * 4;
            do
            {
                ang = (*(s16 *)(q + 0x10) + w) << 7;
                if ((w & 1) == 0)
                {
                    ang += 0x800;
                }
                if (*(s32 *)(q + 0xC) > 0)
                {
                    col = *(s32 *)(q + 0xC) * 0x10101;
                }
                else
                {
                    col = 0;
                }
                for (row = lo; row <= hi; row++)
                {
                    t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                    ang += 0x50 + w * 4;
                    if (*(s16 *)(q + 0x10) >= 9)
                    {
                        col += 0x80808;
                        if (col > 0x484848)
                        {
                            col = 0x484848;
                        }
                    }
                    *(u32 *)&t->r0 = col;
                    SetPolyFT4(t);
                    SetSemiTrans((u8 *)t, 1);
                    t->tpage = GetTPage(2, 3, 0x340, 0x100);
                    t->u0 = t->u1 = ((s16)box.w >> 3) + ((s16)box.w >> 4);
                    t->u2 = t->u3 = (u8)box.w - t->u0;
                    t->v0 = t->v2 = row - (u8)box.y;
                    t->v1 = t->v3 = t->v0 + 2;
                    d = func_800644B8(ang) >> 10;
                    t->x0 = t->x1 = box.x + d + t->u0;
                    t->x2 = t->x3 = t->x0 + box.w - t->u0;
                    t->y0 = t->y2 = row % 224 - w;
                    t->y1 = t->y3 = t->y0 + 2;
                    addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
                }
            } while (--w > 0);
            if (lo == 0)
            {
                for (i = 4; i >= 0; i--)
                {
                    t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
                    *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
                    *(u32 *)&t->r0 = 0x808080;
                    SetPolyFT4(t);
                    SetSemiTrans((u8 *)t, 1);
                    t->tpage = GetTPage(2, 0, 0x340, 0x100);
                    t->x0 = t->x1 = box.x + rand() % 5 - 2;
                    t->x2 = t->x3 = t->x0 + box.w;
                    t->y0 = t->y2 = (s16)box.y % 224 + rand() % 5 - 2;
                    t->u0 = t->u1 = 0;
                    t->y1 = t->y3 = t->y0 + box.h;
                    t->u2 = t->u3 = (u8)box.w;
                    t->v0 = t->v2 = 0;
                    t->v1 = t->v3 = (u8)box.h;
                    addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
                }
            }
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0xC;
            func_80067EF4(t, 0, 0);
            addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x18;
            SetDrawMove((DR_MOVE *)t, (RECT *)&box, 0x340, 0x100);
            addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x28;
            *(u32 *)&t->r0 = 0x808080;
            SetPolyFT4(t);
            t->tpage = GetTPage(2, 0, 0x340, 0x100);
            t->x0 = t->x1 = box.x;
            t->x2 = t->x3 = box.x + box.w;
            t->y0 = t->y2 = (s16)box.y % 224;
            t->u0 = t->u1 = 0;
            t->y1 = t->y3 = t->y0 + box.h;
            t->u2 = t->u3 = (u8)box.w;
            t->v0 = t->v2 = 0;
            t->v1 = t->v3 = (u8)box.h;
            addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0xC;
            func_80067EF4(t, 0, 1);
            addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
            t = (POLY_FT4 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x18;
            SetDrawMove((DR_MOVE *)t, (RECT *)&box, 0x340, 0x100);
            addPrim((u32 *)(g->unk_00 + 0xB0) + acc, t);
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
