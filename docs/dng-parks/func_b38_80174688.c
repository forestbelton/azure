/* PARK: func_b38_80174688, csrc/ovl_dng_b38.c, agent-h225, 2026-11-26.

   TARGET 245 words. BEST BUILT 244 words, 33 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py). 1 member, 245 words (dngfam.py).
   VERDICT: ALLOC is wrong and SHAPE is wrong; this is a SCHEDULE. The frame,
   the save list, the save order, every register and all 27 insns of the
   block in question are the target's. The residue is sched1 choosing a
   different slot for ONE insn.

   WHAT THIS BODY IS. The draw/step callback that func_b38_80174260 installs
   on its 0x201 node, registered through func_8004491C. It walks the node tag
   chain (`n = *(UnkNode **)(p - 0x8); p = (u8 *)n + 0x20'), and per node:
   seeds a two-element SVECTOR array with (+-w, 0, 0), rotates it about the
   record's +0xC centre by the negated D_80083160.unk_C8 camera angle through
   func_800DBA90's 0x20-byte record, projects both vectors with RotTransPers,
   accumulates a screen-space min/max box, and hangs TWO semi-transparent
   POLY_G4 gradient quads (0x24 bytes each off D_80083160.unk_00 + 0x8D0) on
   the OT row at the last projection's depth, plus one DR_MODE. It returns 0.

   THE RESIDUE, EXACTLY. 33 rows: (1) ~26 rows are the 8-instruction
   `movstrsi' block move for `*(SVECTOR *)(rec + 0x10) = *(SVECTOR *)(q+0xC)'
   being emitted FIRST in its basic block where the target emits it SIXTH
   FROM LAST, after `sh $v0, 0x34($sp)'; (2) 2 rows are the `sra $s2,16' and
   `lh/lhu 0x4A($sp)' pair transposed in the min/max loop; (3) 1 row is the
   `addiu $s1, $zero, 0x1' slot before the emit loop; (4) 1 word SHORT -- the
   target loads `xy.vy' TWICE, `lh $a0, 0x4A($sp)' for the comparison and
   `lhu $v1, 0x4A($sp)' for the insert, and this body reuses the signed load.

   ON (1), AND THIS IS THE USEFUL PART: FOUR INDEPENDENT PERTURBATIONS OF
   THAT BLOCK GIVE THE IDENTICAL 40-ROW RESIDUE, so by the levers file's own
   rule the cause is NOT in the block.
     the six rec statements in source order A B C D E F     245 / 40
     the +0xC angle store moved FIRST (C A B D E F)         245 / 40
     the block move moved LAST (A B C E F D)                245 / 40
     the angle read hoisted into an `s16 a' local, stored
       later (s68's spelling)                               245 / 40
     the destination as an ARRAY REF `((SVECTOR *)rec)[2]',
       which sets MEM_IN_STRUCT_P on it                     245 / 40
   `-da' `.sched' explains WHAT happens and not why: the move is insn 116,
   `(parallel [(set (mem/s:BLK (plus $fp 56)) ...)])', it enters the ready
   list at T-10 with priority 1 and ref_count 1, loses every
   `rank_for_schedule' tie to insns 98/100/105/107/121/124 (each reported as
   having `a greater potential hazard'), and is taken last at T-24, which is
   the FIRST emitted slot. For the target to emit it late, either its
   priority or its hazard class must differ, and no source spelling tried
   moves either. NOT RESOLVED. The next thing to try is reading the target's
   own dependence structure: if the target's block move is anti-dependent on
   the preceding rec scalar stores it cannot be hoisted at all, and the
   question is then what makes `memrefs_conflict_p' refuse to separate
   frame+56 from frame+48 there and allow it here.

   THE TYPE DISCOVERY IS THE REUSABLE RESULT, and it cost most of the run.
   The body keeps TWO 4-byte packed screen-coordinate pairs in callee-saved
   REGISTERS, with `sll/sra' extractions and `and'/`or' inserts, and a third
   in memory as RotTransPers' out-parameter. Priced, all from this body:
     `DVECTOR' (i.e. `struct { s16 vx, vy; }')      250 words /  94 aligned
        -- IMPOSSIBLE, not merely wrong: stor-layout.c forces that struct to
        BLKmode because STRICT_ALIGNMENT holds and TYPE_ALIGN 16 is below
        TYPE_SIZE 32, so it can never sit in a register. Both pairs landed in
        8-aligned frame slots and the frame grew 8 bytes.
     plain `s32' with the halves masked by hand      237 words / 136 aligned
        -- combine folds the two half-inserts of the 30000 / -30000 seed into
        one `lui'/`ori' pair, so the seed comes out 6 words short.
     `s32' BIT-FIELDS (`struct { s32 vx : 16; s32 vy : 16; }')
                                                    247 words /  81 aligned
        -- the bit-field carries the base type's 32-bit alignment, so the
        pair gets SImode and an insv/extv per access. This reproduces the
        seed AND the min/max inserts exactly. Its cost is that
        `t->x2 = mx.vx' is then an extv that loop.c hoists out of the emit
        loop, which costs two `move's and takes the registers the target
        spends on `mx.vy' and the 0xFF000000 constant.
     a UNION of `s32 w' and the bit-field struct    236 words / 116 aligned
        alone, and 245 words / 40 aligned once `hy' and `g' are added
        -- KEPT. The whole-word view serves the sites that store a half into
        a `short' (`sh $s2', no extraction at all) and the bit-field view
        serves the seed, the comparisons and the inserts.
   So the discriminator is per SITE, not per type, and a 4-byte pair that is
   both compared half-wise and stored whole needs both views.

   THREE MORE MEASURED FACTS, each from a body at the right word count:
     `hy = mx.h.vy' AS AN EXPLICIT LOCAL before the emit loop is worth
       6 words and 50 rows (236/116 without it, 242/44 with it). The target
       has NINE callee-saved quantities and `q' is NOT one of them -- it is
       spilled to 0x50($sp) and reloaded in each arm. Without `hy' loop.c
       leaves `mx.vy' inside the loop, `q' keeps the register, and the two
       `lw'/`nop' reloads the target pays are missing. `saves minus
       parameters' names the missing value before any compile.
     `Unk80083160 *g = &D_80083160' AS A VAR_DECL is worth exactly 3 words
       (242 -> 245). Written inline as `D_80083160.unk_00 + 0x8D0' the `%lo'
       folds into each load's displacement and the three `addiu's vanish;
       only a VAR_DECL makes cc1psx materialise the full address. This is the
       levers file's `a constant inside a hoisted address' entry with the
       sign that matters here: the local is what KEEPS the addiu, and because
       all nine callee-saved registers are spoken for it is rematerialised at
       each use rather than hoisted.
     the emit loop's arm 1 must write the COLOUR pair before the ZERO pair
       (`*(u32 *)&t->r0 = *(u32 *)&t->r1 = *(u32 *)(q + 0x58)' then
       `*(u32 *)&t->r2 = *(u32 *)&t->r3 = 0'), which is 7 rows, AND ARM 0
       MUST KEEP THE OPPOSITE ORDER -- swapping both arms is 243 / 39, worse
       than swapping neither. Two arms of one loop wanting opposite statement
       orders is the `two bodies in one unit can want opposite answers'
       pattern one level down. With arm 1 swapped the `nop' the target fills
       with those two stores disappears, which is what exposed fault (4).

   MEASURED AND FREE, so NOT why this body is where it is:
     the loop-1 conditional as `v[i].vx = w; if (i != 0) v[i].vx = -w;'
       rather than `if/else' -- both reach the target's
       `sh +w; beqz; sh -w' shape via jump.c's
       `if (c) x = a; else x = b' -> `x = b; if (c) x = a' rewrite.
     a `(u16)' cast on the vy insert value -- folded away, 244 / 33 either
       way, so it is NOT the way to buy fault (4)'s second load.
     an `s32 u' intermediate holding the pre-shift value -- not measurable.

   AND ONE THING THAT MAKES IT WORSE: giving the union a THIRD view with
   `u32' halves and reading the insert value through it is 248 / 41. It does
   buy the second load but costs three `andi' masks.

   DONORS. dngtwin finds NO carved sibling at 0.70 for any stub in this unit.
   The body came from csrc/ovl_dng_s68.c func_s68_80024B20 (203 words,
   landed), found by intersecting SetPolyG4 / SetSemiTrans / GetTPage /
   func_800DBA90 / 0x8D0 / RotTransPers over the landed side -- s68 scored
   5 of 6. Its `while (1) { ... n = *(UnkNode **)(p - 0x8); if (n == NULL)
   break; p = (u8 *)n + 0x20; } return 0' chain walk, its `u8 rec[0x20]'
   record type for func_800DBA90 (which it attributes to
   csrc/ovl_dng_b04.c func_b04_801743EC) and its `q = p' alias split all
   carry verbatim. Its own priced fact -- `*(s16 *)(rec + 0x1A) = 0' before
   `*(s16 *)(rec + 0x18) = 1', ascending being 2 rows -- is NOT MEASURABLE
   here, because reorg puts whichever store is last into the `jal's delay
   slot either way. csrc/ovl_dungeon_800D50FC.c func_800D5AC0 supplied the
   `func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0)' DR_MODE idiom
   verbatim.

   RESOLVED NAMES (splat/common_syms.txt): func_80065420 is RotTransPers,
   func_80066708 is SetPolyG4, func_80066640 is SetSemiTrans, func_80066460
   is GetTPage. func_80067F20 has no library name and is declared in
   include/structs.h:2067. `addPrim' is psyq/include/libgpu.h:246 and expands
   to the target's two and/or tag splices.

   `q = p' IS LOAD-BEARING: dropping it and using `p' at both sites is
   243 words / 86 aligned.

   relcand.py: ok, 1 data symbol and 6 calls. declcheck.py: clean.
   NOT a table owner (MYTABLES.txt and JTCHECK.txt both say the unit's three
   stubs own no jump table), so there is no arm order to settle.

   DECLARATIONS
   extern u8 D_80083498[];
   extern u8 D_80045340[];
   void func_800DBA90(u8 *);
   s32 func_b38_80174688(u8 *);
   typedef union UnkDngB38XY
   {
       s32 w;
       struct
       {
           s32 vx : 16;
           s32 vy : 16;
       } h;
   } UnkDngB38XY;
   END DECLARATIONS

   func_8004491C, func_80067F20 and D_80083160 all come from
   include/structs.h and need no unit-local declaration. SetPolyG4,
   SetSemiTrans, GetTPage and RotTransPers come from the psyq headers through
   common.h. POLY_G4, DR_MODE and SVECTOR likewise. */
s32 func_b38_80174688(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    SVECTOR v[2];
    u8 rec[0x20];
    UnkDngB38XY xy;
    s32 w;
    u8 *q;
    UnkNode *n;
    POLY_G4 *t;
    DR_MODE *dm;
    UnkDngB38XY mn;
    UnkDngB38XY mx;
    s32 hy;
    s32 dep;
    s32 i;

    while (1)
    {
        q = p;
        w = 8;
        for (i = 1; i >= 0; i--)
        {
            v[i].vx = w;
            if (i != 0)
            {
                v[i].vx = -w;
            }
            v[i].vy = v[i].vz = 0;
        }
        *(s16 *)(rec + 0x8) = *(s16 *)(rec + 0xA) = 0;
        *(u8 **)(rec + 0x4) = *(u8 **)rec = (u8 *)v;
        *(s16 *)(rec + 0xC) = -g->unk_C8;
        *(SVECTOR *)(rec + 0x10) = *(SVECTOR *)(q + 0xC);
        *(s16 *)(rec + 0x18) = 2;
        *(s16 *)(rec + 0x1A) = 0;
        func_800DBA90(rec);
        mn.h.vx = mn.h.vy = 30000;
        mx.h.vx = mx.h.vy = -30000;
        for (i = 1; i >= 0; i--)
        {
            dep = RotTransPers(&v[i], (long *)&xy, (long *)&w, (long *)&w) - 4;
            if (mx.h.vx < xy.h.vx)
            {
                mx.h.vx = xy.h.vx;
            }
            if (xy.h.vx < mn.h.vx)
            {
                mn.h.vx = xy.h.vx;
            }
            if (mx.h.vy < xy.h.vy)
            {
                mx.h.vy = xy.h.vy;
            }
            if (xy.h.vy < mn.h.vy)
            {
                mn.h.vy = xy.h.vy;
            }
        }
        if ((u32)dep < 0x1E0)
        {
            w = (mx.h.vx + mn.h.vx) >> 1;
            hy = mx.h.vy;
            for (i = 1; i >= 0; i--)
            {
                t = (POLY_G4 *)*(u32 *)(g->unk_00 + 0x8D0);
                *(u32 *)(g->unk_00 + 0x8D0) = (u32)t + 0x24;
                if (i != 0)
                {
                    *(u32 *)&t->r0 = *(u32 *)&t->r1 = *(u32 *)(q + 0x58);
                    *(u32 *)&t->r2 = *(u32 *)&t->r3 = 0;
                    t->x2 = t->x3 = mx.w;
                    t->x0 = t->x1 = w;
                }
                else
                {
                    *(u32 *)&t->r0 = *(u32 *)&t->r1 = 0;
                    *(u32 *)&t->r2 = *(u32 *)&t->r3 = *(u32 *)(q + 0x58);
                    t->x0 = t->x1 = mn.w;
                    t->x2 = t->x3 = w;
                }
                SetPolyG4(t);
                SetSemiTrans((u8 *)t, 1);
                t->y0 = t->y2 = 0;
                t->y1 = t->y3 = hy;
                addPrim((u32 *)(g->unk_00 + 0xB0) + dep, t);
            }
            dm = (DR_MODE *)*(u32 *)(g->unk_00 + 0x8D0);
            *(u32 *)(g->unk_00 + 0x8D0) = (u32)dm + 0xC;
            func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0, 0), 0);
            addPrim((u32 *)(g->unk_00 + 0xB0) + dep, dm);
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
