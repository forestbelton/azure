/* PARK: func_s30_800241D4, the ovl_dng_s30 slot renderer registered by
   func_s30_80024640 through func_8004491C.

   STATUS: 283 words against the target's 283, and 2 ALIGNED DIFFERENCES
   (ovlverify reports 21 differing, which is the masked count downstream of the
   one fault -- aligndiff is the number of PLACES wrong). This function owns NO
   jump table: its listing has no .section .rodata.

   AGENT-H232, 2026-11-26: THE CAUSE IS NOW NAMED AND THE RESIDUE HAS BEEN
   MOVED, BUT NOT YET AT A PROFIT.  Read this before anything below it.

   THE MECHANISM, read out of the compiler and confirmed by measurement.
   sched.c's `adjust_priority' is called the moment an insn becomes ready.
   Because the `??? This code has no effect' loop over REG_DEAD notes always
   counts n_deaths == 0 (the notes are gone by then), EVERY ready insn falls
   into `case 0:' and is passed to `birthing_insn_p'.  That returns
   `reg_n_sets[i] == 1' for an insn whose destination pseudo is live, and the
   insn's priority is then raised to `max_priority', which
   (sched.c, `max_priority = MAX (INSN_PRIORITY (ready[0]), ...)') is the
   current frontier's best -- so a boosted insn is picked immediately by the
   backward scan and therefore lands LATE in the emitted block.  The `sll3'
   that computes w times 8 is exactly such an insn here, and that is why it
   sits at word 123 instead of 103.

   REMOVING THE BOOST PUTS THE sll3 AT WORD 103.  The gate is ONE SET versus
   TWO SURVIVING SETS of the destination pseudo.  Two spellings do it:

     w = (t % 4) * 8; ... ; w += 0x3F; with setUV4 expanded
       to its eight field stores                         283 w, 19 rows
       -- sll3 at 103, CORRECT.  What breaks instead: the `addiu w + 0x3F'
          lands at 104 instead of 127, and the mod-4 chain's registers rotate
          to $a0/$t6/$v1 where the ROM has $v1/$v0/$v0
     d = (t % 4) * 8; reusing `d', which this body already sets twice
                                                          283 w, 36 rows
       -- sll3 at 103, CORRECT, but in $t5, which rotates the whole $t
          register wall AND rotates `d's two earlier uses (rows 25..46)

   SO THIS IS THE BRIEF'S "worse alone, necessary in combination" SHAPE: the
   change that fixes the park's one stated fault measures 19 rows against 2.
   What is still missing is a shape in which the sll3's destination has TWO
   sets (no boost, so it lands at 103) while the `+ 0x3F' destination is a
   DISTINCT SINGLE-set pseudo (so it IS boosted and lands at 127) that
   combine_regs then ties to the sll3's destination, giving both $v0 -- which
   is exactly what the ROM's `sll $v0,$v0,3' ... `addiu $v0,$v0,0x3F' pair is.
   I could not spell that: a two-set `w' makes both sets the same pseudo, and
   a separate temp for the `+ 0x3F' makes `w' single-set again.

   FOUR THINGS MEASURED ALONG THE WAY, two of them corrections to this header:

   * THE "two sets of w" ROW BELOW IS WRONG.  `w = t % 4; w = w * 8;'
     measures 283 words / 2 aligned rows with the SAME `sll3 at 123' residue
     as the one-set form, NOT the `sb zero, 0xC(q) at 104' residue recorded
     below.  It is not a two-set form at all: the first value has a single
     use, so combine merges the two sets into one and reg_n_sets is 1 again.
     Any "give it two sets" remedy has to put a READ of the first value
     between the two sets, or combine undoes it.
   * `w += 0x3F' and `w = w + 0x3F' are identical (19 rows each), and the
     addiu's emitted position is INSENSITIVE to where the statement is written
     in source -- before or after `q->u2 = 0x3F' is 19 rows either way.  The
     position is sched's, not the source's.
   * A DEAD SECOND SET DOES NOT WORK.  `w = (t % 4) * 8;' plus a trailing
     `w = 0;' just before the `return 0;' measures 283 / 8 -- identical to
     `w = (t % 4) * 8;' on its own -- because flow deletes the dead insn
     before reg_n_sets is what birthing_insn_p reads.
   * Expanding setUV4 into its eight field stores in source order remains
     free, as the 2x2 below found: 283 / 2 with the one-set `w'.

   WHAT IT DOES. Builds one semi-transparent POLY_GT4 billboard quad for the
   caller's UnkVec in the 0x1F800000 scratchpad: the four SVECTORs at +0x64 are
   two columns at b+0x2 -+ d and two rows at b+0x6 +- e, where
   d = func_80064584(D_80083160.unk_C8) >> 4 times 36 >> 8 and e is the same
   through func_800644B8, with the near pair's z at b+0xA - 0x80 and the far
   pair's at b+0xA. The primitive takes black at vertices 0 and 1, the payload's
   +0xA byte at vertices 2 and 3, the payload's +0x8 halfword as its clut, a
   0x3F-square UV patch whose v origin is the payload's +0x4 counter mod 4 times
   8, projects through RotAverage4 and, when the averaged z is under 0x1E0,
   chains a DR_MODE with texture window 0,0,0xFF,0xFF, then the quad, then a
   second DR_MODE with window 0,0x80,0x40,0x40 into D_80083160's +0x8D0 ring
   with addPrim.

   THE DONOR, and no ratio tool could reach it. dngtwin reports no carved
   sibling at 0.70 for this stub and xsibs finds nothing; what found the donor
   was ONE grep on the only callee that is not library-common,
   grep -ln func_80065590 over csrc/ovl_dng_*.c, which hits exactly one file.
   That is csrc/ovl_dng_s38.c, whose comment names func_80065590 as RotAverage4
   -- and re-grepping the REAL name plus the scratchpad field names turns up
   csrc/ovl_dng_s22.c's func_s22_8002445C, 241 words, which has THIS EXACT
   scratchpad layout: unk_18 the OT base, unk_64/6C/74 the SVECTORs, unk_84/88
   RotAverage3's two outputs, unk_B4 the averaged z, unk_D8.. the projected sxy
   pairs. UnkDngS30PolySP below is that file's UnkDngS22LineSP with a FOURTH
   vertex and a fourth sxy pair. The quad-versus-triangle shape and the
   setPolyGT4 head came from csrc/ovl_dng_s60.c's func_s60_80024A30.

   THE ONE REMAINING FAULT, and it is a scheduling choice inside the basic block
   that starts at .Ls30_80024364. Target words 100..123 are
   sra/sll/subu/sll3 (the p+0x4 mod-4 chain and its times-8) followed by the
   twenty lui/ori that materialise RotAverage4's ten scratchpad addresses. The
   candidate emits those two groups the other way round. Every other word in the
   body, including the whole UV group, the three addPrim expansions and all
   seven callee-saved registers, is byte-identical.

   TWO INDEPENDENT SPELLINGS REACH 2 ROWS AND NEITHER REACHES 0, which is the
   brief's signature for a second fault rather than a floor -- except that here
   the two 2-row states have DIFFERENT residues:

     w = t % 4 with w times 8 inline in setUV4        283 w, 2   KEPT
       the one row is `sll v0,v0,3' at 123 not 103
     w = t % 4; w = w * 8;  two sets of w             283 w, 2
       the one row is `sb zero,0xC(q)' at 104 not 128
       -- REFUTED above by agent-h232: this is NOT a two-set form, combine
          merges it, and it re-measures as the sll3-at-123 residue, not this
     do { w = (t % 4) * 8; } while (0);               283 w, 2
       same residue as the two-set form
     w = (t % 4) * 8;  one set, no device             283 w, 8
       the whole four-insn chain at 120..123 not 100..103

   MECHANISM, and it is sched.c's adjust_priority. birthing_insn_p
   (sched.c, the reg_n_sets[i] == 1 arm) lifts an insn that SETS a single-set
   live pseudo to max_priority, and gcc 2.7.2's list scheduler is BACKWARD, so a
   boosted insn is placed LATE. With `w = (t % 4) * 8' the boosted insn is the
   sll3 that sets w and it drags its three predecessors past the lui block; with
   `w = t % 4' the boost lands on the subu instead and only the times-8
   (now a cse temp) is displaced; with two sets of w nothing in the chain is
   boosted and the chain lands correctly, but the slot the chain vacated is then
   filled by the lowest-LUID priority-1 store, which is the u0 zero store. I
   could not find a spelling that satisfies both at once.

   EVERY SPELLING MEASURED AND REJECTED, all with aligndiff, all from the 283-word
   state unless the word count is given:

     clut read AFTER the six zero colour stores        284 w, 85
       -- the extra word is an unfilled load delay; the target hoists
          `lhu 0x8(p)' seven slots above the stores, which only happens when
          the clut assignment is BEFORE them in source and the stores sink
     clut as ((u16 *)p)[4] instead of a plain cast      284 w, 85  no change
     ONE DR_MODE local for both dress sites             283 w, 85
       -- this is the single largest fact in the body, 72 rows. Two separate
          locals give each a short live range and the allocator then packs
          dm into p's $s0; one local spans both sites, loses the priority
          contest and takes $s4, which rotates five callee-saved registers
          across the whole second half
     s16 ang for D_80083160.unk_C8                      287 w, 113
       -- unk_C8 is s16, so an s16 local costs a redundant sll16/sra16 pair;
          s32 ang takes the lh straight
     (*(s16 *)(p + 0x4) % 4) * 8 with no int local       284 w, 88
       -- shorten_binary_op narrows the MULT to HImode and emits sll16/sra13
          where the target has a bare sll3. An int local for the loaded
          halfword is what keeps the mod in SImode
     (s32) cast on the load instead of the int local     284 w, 9
     setRECT(&tw, ...) for both texture windows         283 w, 8
       -- the target writes each window as TWO CHAINS, `tw.x = tw.y = 0' and
          `tw.w = tw.h = 0xFF', which emit y-then-x and h-then-w. setRECT's
          four separate stores emit in source order and are 6 rows
     t read as ((s16 *)p)[2] rather than a plain cast   283 w, 8   no change
     the ten RotAverage4 addresses as literal
       (SVECTOR *)0x1F800064 constants                  283 w, 2   no change
     the eight UV stores in the order the listing
       emits them, v0,v1,u0,u1,u2,v2,u3,v3             283 w, 8   no change
     the UV stores as u-group then v-group             283 w, 30
     the UV stores as v-group then u-group             283 w, 38
     q->u0 and q->u3 through a u8 * cursor             283 w, 2   no change
     q->u2 = q->u1 = 0x3F as a chain                   283 w, 2   no change
     q->u3 = q->u0 = 0 as a chain                      283 w, 4
     the t/w pair hoisted to the top of the function    276 w, 83
     the t/w pair after setRGB1                         277 w, 74
     the t/w pair after setRGB2                         280 w, 62
     do { } while (0) round setUV4                      283 w, 44
     if (1) round the w assignment                      283 w, 8
       -- the brief's rule holds: if (1) is not a loop and buys nothing here
     d split into two locals for the two 36-multiplies  283 w, 8   no change

   MEASURED AND FREE, so none of these is why the body is as close as it is:
   the ten RotAverage4 addresses as literal constants rather than
   (u32)sp | 0xNN (283 / 2, bit-identical); q->u0 and q->u3 written through a
   u8 * cursor (283 / 2, bit-identical); and reading t through an array ref
   (283 / 8, bit-identical with the plain cast). Note these are bit-identical
   residues and therefore NOT MEASURABLE rather than free.

   DIRECTION (1) IS NOW CLOSED (dispatcher, 2026-11-25).  I ran it as a 2x2
   and every cell is 283 words / 2 aligned rows -- the same figure as the kept
   body:

       setUV4 expanded to eight field stores, `w' one set     283 /  2
       ... expanded, `w' TWO sets                             283 /  2
       ... u0 and u3 PINNED as plain casts
           (`*(u8 *)((u8 *)q + 0xC)' and `+ 0x30', which clear
            MEM_IN_STRUCT_P where a COMPONENT_REF sets it),
           `w' one set                                        283 /  2
       ... u0 and u3 pinned AND `w' two sets                   283 /  2

   So neither half of the suggested conjunction moves it and nor does the
   combination.  Two things worth having from that: EXPANDING `setUV4' INTO ITS
   EIGHT FIELD STORES IS FREE (the macro is a comma expression, so this was
   never a shape question), and the two-set `w' residue is now pinned down
   precisely -- it is the `sll $v0, $v0, 3' at TARGET WORD 103 against word 123
   here, a 20-slot displacement, with nothing else differing.

   Pinning the zero stores was the specific thing direction (1) asked for, on
   the theory that the u0 store was taking the slot the `sll' needs.  It is
   not: clearing MEM_IN_STRUCT_P on both zero stores changes nothing at all,
   so whatever holds that slot is not those stores' mobility.

   ONLY DIRECTION (2) REMAINS -- the primitive not being reached as a POLY_GT4
   struct pointer at all -- and the author's reason for not trying it still
   stands: it rewrites the whole body, and the colour and position groups are
   already byte-identical as struct fields.  Anyone attempting it should expect
   to re-measure everything, not just the UV group.

   WHAT A NEXT AGENT SHOULD TRY. The body is one insn out, so this is not an
   allocation problem and -da will corroborate whatever story it is given. The
   question is narrow: what source shape puts the p+0x4 mod-4-times-8 chain at
   words 100..103 WITHOUT freeing a slot for the u0 zero store. Two untried
   directions: (1) a spelling in which the times-8's destination is a pseudo
   with two sets AND the u0 store is not the lowest-LUID priority-1 insn in the
   block -- i.e. combine the two-set w with a UV group whose zero stores both
   carry a block-internal dependence; (2) the possibility that the primitive is
   not reached as a POLY_GT4 struct pointer at all, which would change every
   store's MEM_IN_STRUCT_P and so which of them sched is allowed to move. I did
   not test (2) because it rewrites the whole body and the colour and position
   groups are already byte-identical as struct fields.

   dngfam.py reports this function as a ONE-MEMBER family, so nothing is
   replicated off it.

   DECLARATIONS
   typedef struct UnkDngS30PolySP
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
       u8 pad_72[0x2];
       u16 unk_74;
       u16 unk_76;
       u16 unk_78;
       u8 pad_7A[0x2];
       u16 unk_7C;
       u16 unk_7E;
       u16 unk_80;
       u8 pad_82[0x2];
       s32 unk_84;
       s32 unk_88;
       u8 pad_8C[0x28];
       s32 unk_B4;
       u8 pad_B8[0x20];
       u16 unk_D8;
       u16 unk_DA;
       u16 unk_DC;
       u16 unk_DE;
       u16 unk_E0;
       u16 unk_E2;
       u16 unk_E4;
       u16 unk_E6;
   } UnkDngS30PolySP;
   END DECLARATIONS

   Everything else the body names comes from include/structs.h already:
   D_80083160 with its Unk80083160 type, func_80064584, func_800644B8 and
   func_80067F20. RotAverage4, GetTPage, setPolyGT4, setSemiTrans, setRGB0..3,
   setUV4, setRECT, addPrim, POLY_GT4, DR_MODE and RECT all come from
   psyq/include/libgpu.h through common.h. */
s32 func_s30_800241D4(u8 *p, UnkVec *b, UnkSlot *s)
{
    UnkDngS30PolySP *sp = (UnkDngS30PolySP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    POLY_GT4 *q;
    DR_MODE *dm;
    DR_MODE *dm2;
    RECT tw;
    s32 ang;
    s32 d;
    s32 t;
    s32 w;

    ang = g->unk_C8;
    sp->unk_18 = D_80083160.unk_00 + 0xB0;
    d = (func_80064584(ang) >> 4) * 36 >> 8;
    sp->unk_64 = sp->unk_74 = *(u16 *)((u8 *)b + 0x2) - d;
    sp->unk_6C = sp->unk_7C = *(u16 *)((u8 *)b + 0x2) + d;
    d = (func_800644B8(ang) >> 4) * 36 >> 8;
    sp->unk_66 = sp->unk_76 = *(u16 *)((u8 *)b + 0x6) + d;
    sp->unk_6E = sp->unk_7E = *(u16 *)((u8 *)b + 0x6) - d;
    sp->unk_68 = sp->unk_70 = *(u16 *)((u8 *)b + 0xA) - 0x80;
    sp->unk_80 = sp->unk_78 = *(u16 *)((u8 *)b + 0xA);
    q = *(POLY_GT4 **)(D_80083160.unk_00 + 0x8D0);
    *(POLY_GT4 **)(D_80083160.unk_00 + 0x8D0) = q + 1;
    setPolyGT4(q);
    setSemiTrans(q, 1);
    q->tpage = GetTPage(0, 1, 0x280, 0x100);
    q->clut = *(u16 *)(p + 0x8);
    setRGB0(q, 0, 0, 0);
    setRGB1(q, 0, 0, 0);
    setRGB2(q, p[0xA], p[0xA], p[0xA]);
    setRGB3(q, p[0xA], p[0xA], p[0xA]);
    t = *(s16 *)(p + 0x4);
    w = t % 4;
    setUV4(q, 0, w * 8, 0x3F, w * 8, 0x3F, w * 8 + 0x3F, 0, w * 8 + 0x3F);
    sp->unk_B4 = RotAverage4((SVECTOR *)((u32)sp | 0x64), (SVECTOR *)((u32)sp | 0x6C),
                             (SVECTOR *)((u32)sp | 0x74), (SVECTOR *)((u32)sp | 0x7C),
                             (long *)((u32)sp | 0xD8), (long *)((u32)sp | 0xDC),
                             (long *)((u32)sp | 0xE0), (long *)((u32)sp | 0xE4),
                             (long *)((u32)sp | 0x84), (long *)((u32)sp | 0x88));
    q->x0 = sp->unk_D8;
    q->y0 = sp->unk_DA;
    q->x1 = sp->unk_DC;
    q->y1 = sp->unk_DE;
    q->x2 = sp->unk_E0;
    q->y2 = sp->unk_E2;
    q->x3 = sp->unk_E4;
    q->y3 = sp->unk_E6;
    if ((u32)sp->unk_B4 < 0x1E0)
    {
        tw.x = tw.y = 0;
        tw.w = tw.h = 0xFF;
        dm = *(DR_MODE **)(D_80083160.unk_00 + 0x8D0);
        *(DR_MODE **)(D_80083160.unk_00 + 0x8D0) = dm + 1;
        func_80067F20((u32 *)dm, 0, 0, GetTPage(0, 1, 0x280, 0x100), (s32)&tw);
        addPrim(sp->unk_18 + sp->unk_B4 * 4, dm);
        addPrim(sp->unk_18 + sp->unk_B4 * 4, q);
        tw.x = 0;
        tw.y = 0x80;
        tw.w = tw.h = 0x40;
        dm2 = *(DR_MODE **)(D_80083160.unk_00 + 0x8D0);
        *(DR_MODE **)(D_80083160.unk_00 + 0x8D0) = dm2 + 1;
        func_80067F20((u32 *)dm2, 0, 0, GetTPage(0, 1, 0x280, 0x100), (s32)&tw);
        addPrim(sp->unk_18 + sp->unk_B4 * 4, dm2);
    }
    return 0;
}
