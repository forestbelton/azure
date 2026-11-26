#include "common.h"

s32 rand(void);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s03", func_s03_80024020);

INCLUDE_ASM("asm/nonmatchings/ovl_dng_s03", func_s03_800245B4);

/* 153 words, 0 differing. Walks a linked list of nodes at `arg0' (next at
   e-8, the successor's cursor being that pointer + 0x20) and for each one
   emits four semi-transparent POLY_F3 triangles -- one per SVECTOR in the
   0x28+i*8 array -- into D_80083160's OT, each with its own DR_MODE.
   Each triangle's three corners are the single projected point jittered by
   rand(): +1/+1, -1/+1, (rand()&4)-2 / -1.

   DERIVED FROM func_800A2AF8 (ovl_town.c), which dngcallee found at 6/7
   symbols; dngtwin found nothing at all for this unit.  That donor's frame
   (-0x38, 8 saves) is not this one's (-0x48, 10 saves), so only its
   STATEMENTS carried over: the Unk80083160 allocator pair, SetSemiTrans,
   the func_80067F20/GetTPage pair and the two addPrims.

   Three facts, each priced ALONE against the otherwise-matching body:

   * **`sxy' MUST BE A SCALAR, NOT `u16 sxy[2]'.** 36 aligned differences as
     an array.  gcc 2.7.2's assign_stack_local takes alignment from
     BIGGEST_ALIGNMENT for a BLKmode local, so u16[2] occupies EIGHT bytes
     and `vars' comes out 16 against the target's 8 -- frame -0x50, every
     save displacement shifted, and RotTransPers's third argument at
     sp+0x20 instead of sp+0x1C.  A two-field struct (DVECTOR) is BLKmode
     too and would cost the same.  Read through `((u16 *)&sxy)[k]' the slot
     is SImode, four bytes, and the frame closes.

   * **`e = arg0;' BEFORE `g = &D_80083160;' -- worth 4 rows** (7 against
     3).  Written the other way round the `lui %hi(D_80083160)' is the first
     real insn, so sched2 interleaves the prologue saves starting at $s5 and
     `sw $fp'/`addu $fp, $a0' land fourth and fifth instead of first and
     second.  Nothing else in the body moves.

   * **THE LOOP EXIT IS A `break' BEFORE THE ADVANCE, NOT A `do/while' AFTER
     IT -- worth the last 3 rows.**  Spelled
     `nxt = *(u32 *)(e - 8); e = (u8 *)(nxt + 0x20); } while (nxt != 0);'
     the load, the add and the test are ONE basic block, `nxt' is a
     block-local quantity, and local-alloc hands it $v0 -- MIPS defines no
     REG_ALLOC_ORDER, so the default ascending order reaches reg 2 before
     reg 4.  With the `break' between the test and the advance the add sits
     in its own block, `nxt' is live out of the test block, so it is a
     GLOBAL allocno and global.c gives it $a0, which is what the target has.
     dbr then pulls the advance back into the branch's delay slot either
     way, so the emitted word order is identical and only the register
     differs.  The three rows are `lw'/`bnez'/`addiu'.

   MEASURED AND FREE, so NOT reasons this lands: `nxt' declared first among
   the locals rather than last (3 rows either way); `nxt' as `u8 *' with
   pointer arithmetic rather than `u32' (3 rows either way); and reusing the
   parameter `arg0' as the temp instead of a separate `nxt', which the
   "suggested register" ladder predicts should claim $a0 and does NOT -- 7
   rows, identical to not doing it, because `arg0' is a global allocno whose
   prologue copy is in another block.

   func_80067F20 rides an implicit declaration here, as it does at the eight
   other carved call sites (ovl_dng_b06/b09/b19/b20/b23); that is what makes
   GetTPage's `u_short' return arrive as `andi $a3, $v0, 0xFFFF'. */
s32 func_s03_80024A1C(u8 *arg0)
{
    Unk80083160 *g;
    POLY_F3 *p;
    DR_MODE *dm;
    u8 *e;
    u8 *q;
    s32 i;
    s32 otz;
    s32 sxy;
    s32 pz;
    u32 nxt;

    e = arg0;
    g = &D_80083160;
    for (;;)
    {
        q = e;
        for (i = 3; i >= 0; i--)
        {
            dm = (DR_MODE *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(dm + 1);
            p = (POLY_F3 *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)(p + 1);
            *(u32 *)&p->r0 = *(u32 *)(q + 0x48);
            SetPolyF3(p);
            SetSemiTrans(p, 1);
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, (*(s16 *)(q + 0x54) > 0) ? 1 : 3, 0, 0), 0);
            otz = RotTransPers((SVECTOR *)(q + 0x28 + i * 8), (long *)&sxy, (long *)&pz,
                               (long *)&pz) -
                  8;
            p->x0 = ((u16 *)&sxy)[0] + (rand() & 1) + 1;
            p->y0 = ((u16 *)&sxy)[1] + (rand() & 1) + 1;
            p->x1 = ((u16 *)&sxy)[0] - (rand() & 1) - 1;
            p->y1 = ((u16 *)&sxy)[1] + (rand() & 1) + 1;
            p->x2 = ((u16 *)&sxy)[0] + (rand() & 4) - 2;
            p->y2 = ((u16 *)&sxy)[1] - (rand() & 1) - 1;
            if ((u32)otz < 0x1E0)
            {
                addPrim((u32 *)(g->unk_00 + 0xB0) + otz, p);
                addPrim((u32 *)(g->unk_00 + 0xB0) + otz, dm);
            }
        }
        nxt = *(u32 *)(e - 8);
        if (nxt == 0)
        {
            break;
        }
        e = (u8 *)(nxt + 0x20);
    }
    return 0;
}
