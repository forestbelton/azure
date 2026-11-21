/* ============================== 2026-11-21: 71 rows -> 70

   ====================================== 2026-11-21: 60 rows -> 55

   Two cancelled pairs on `r` from tools/decomp/refsweep.py, before
   `r[0xD] = r[0x15] += 0x40;` and before `r[0x2D] >>= 1;`: 60 rows -> 55.
   Note both are INSERTIONS ahead of those statements and neither moves them --
   this park has twice rejected a permuter find that moved `r[0x2C]`/`r[0x2D]`
   ahead of the store that initialises that word.

   ====================================== 2026-11-21: THE SAME REJECTION
                                          TWICE, ON A DIFFERENT BYTE

   The second run (base 6375, best 2530) found 63 rows, and its change is
   `r[0x2D] >>= 1;` moved ahead of `*(u32 *)(r + 0x2C) = 0x606060;` -- the
   same defect as the first run's `r[0x2C] >>= 1;`, one byte over.  The shift
   reads the previous frame's packet buffer and the store then overwrites the
   word, so the POLY_F4's green channel ends 0x60 where the target's is 0x30.

   TWO INDEPENDENT RUNS CONVERGED ON THE SAME ILLEGAL MOVE, which is what to
   expect: the permuter scores the object, the object does not care which value
   reaches the GPU, and the move happens to shorten a live range.  Any find
   here that touches the three `>>= 1' shifts needs reading before measuring.

   ====================================== 2026-11-21: 70 rows -> 60

   A duplicated-arm `if (flag)` around the cursor write-back
   `*(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x58);`, found by
   tools/decomp/devsweep.py.  Ten rows on one wrapper, and `flag' is the right
   condition because it is the one name in the function whose reference count
   this residue is about -- the sweep tried all fourteen.

   A `do { } while (0);` around `*(u32 *)(r + 0x2C) = 0x606060;` measures 68,
   so there are two independent wrappers here and the sweep kept the better.
   Worth re-running devsweep on this body for the second one.

   `u8 *dr = r + 0x4C;` computed between `ry = *(u16 *)(p + 0x6);` and
   `rc.y = ry;`, and passed to the final func_80067E2C instead of the inline
   `r + 0x4C`.  One row; the `addiu` moves up out of the call sequence.

   REJECTED FROM THE SAME PERMUTER RUN -- and this one is a correctness
   rejection, not a score one.  Its best find (64 rows, 7 better) moves
   `r[0x2C] >>= 1;` to BEFORE `*(u32 *)(r + 0x2C) = 0x606060;`.  The shift
   then reads whatever the previous frame left in the packet buffer and the
   store overwrites it, so the first colour channel ends at 0x60 where the
   target's is 0x30: the POLY_F4's red is wrong and the whole 7 rows is that
   dead statement.  The permuter cannot see it because the object it diffs
   does not care which value reaches the GPU.  Checking what a find MEANS, not
   just what it scores, is the second gate after measuring it.
   target 358 words, candidate 358 words, 70 aligned differences

   PARKED 2026-11-16, j50.  target 358 words, candidate 358 words,
   87 differing (ovlverify masked); aligndiff 71 aligned differences, EVERY
   instruction positionally aligned and most rows register-name-only.

   WHAT THIS FUNCTION IS: the renderable callback func_8009ED30 installs
   (csrc/ovl_dungeon.c:12997 hands it to func_8004491C).  It emits, off
   D_80083160.unk_00's +0x8D0 cursor: a DR_AREA set to the draw environment's
   own clip RECT, one POLY_FT4 projected through four func_80065320 calls, a
   COPY of that POLY_FT4 0x28 bytes later with its two u/v bytes biased by
   0x40 and 0x100 ORed into the second tpage, a POLY_F4 whose colour is
   0x606060 halved per channel and whose four screen pairs are copied out of
   the first quad, a 0xC-byte func_80067F20 primitive, and a second DR_AREA
   from the caller's own +0x04..+0x0B rect -- biased +0xE0 in y whenever
   D_80083160.unk_00 is not D_801C9E40.  All six hang on the OT row at
   sp->unk_20 + b->unk_0A * 4.

   DECLARATIONS THIS BODY NEEDS: the UnkDungeonSPB typedef (in the agent's
   report) and a PROTOTYPE CHANGE -- csrc/ovl_dungeon.c:1515 declares
   `void func_8009E798();`, but the function returns 0 and takes three
   arguments, so that line must become
   `s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);`.  Its only use is
   `func_8004491C(n, (u32)func_8009E798);` inside the landed func_8009ED30,
   which re-measures 71 words / 0 differing with the change.

   THE RESIDUE is one callee-saved slot: `r` lands in $s1 where the target
   has $s2, and the target keeps a separate $s1 as a rolling cursor for the
   four later primitives (r+0x28, r+0x40, r+0x4C, r+0x58) while mine reuses
   $s0 after `q` dies.  `p` and `flag` are also exchanged ($s3/$s4).  Three
   smaller clusters follow from that: the `*(u16 *)(r + 0x16) |= 0x100` pair
   lands after the +0x25 byte update instead of between the two byte updates;
   the POLY_F4 copy loads three source words before its first store where the
   target loads four; and the second AddPrim passes `q` where the target
   rematerialises `r - 0x28`.

   THE TWO `__asm__` LINES ARE LOAD-BEARING AND EACH IS MEASURED.  Without
   them the function is 358 words / 223 differing.  cse folds `r = q + 0x28`
   into every later `r + N` as `q + (0x28 + N)` -- the .lreg dump shows the
   tail computing `(plus (reg/v:SI 89) (const_int 116))` and `(const_int 128)`
   straight off `q`, so `r` never reaches the allocator at all and `q` stays
   live to the end of the function, which makes it a GLOBAL allocno; the
   globals are then handed $s4..$s7 after local-alloc has already taken
   $s0..$s3, and the whole callee-saved file permutes.
     `__asm__("" : "=r"(r) : "0"(r));`   -- the matched-operand form from the
   catalogue's "A MATCHED-OPERAND `asm` IS A SECOND SET THAT IS *NOT* A
   SCHEDULING BARRIER" -- makes `r` opaque to cse, which alone takes it to
   357 words / 122 differing: right, but ONE WORD SHORT, because `r` now has
   two sets, loses `adjust_priority`'s single-set boost, and sched1 hoists the
   `addiu` into a load-delay nop 12 words earlier.
     `__asm__("" : "=r"(q) : "0"(q));`   -- the same device on `q`, placed
   immediately before `r = q + 0x28;` -- puts the `addiu` back where the
   target has it and the function is 358 words / 87 differing.  An
   `__asm__ __volatile__("")` barrier in the same place measures identically
   (358 / 87); an input-form `__asm__("" ::"r"(q))` is 359 / 331.

   MEASURED AND REJECTED:
     one `s16 a` shared by the x and y sign blocks    223 -> 87 is partly this:
       a shared local is live across both blocks, so it is a global allocno and
       cannot coalesce with the sign-extended byte; two locals give the target's
       `sll`/`sra`/`sh`/`sh`/`addu`/`sh`/`sh` in $v0 exactly (5 differing)
     `sp->unk_24 = s->unk_14;` before `e = s->unk_08;`  359 words / 309 --
       with `e` first the `lw` fills the `lhu 0x14` load delay and PushMatrix's
       delay slot takes the `sh 0x24`; the other way the `sh` is emitted before
       the call and `q += 0xC` (a callee-saved pseudo add, which sched2 moves
       freely across a call) steals the delay slot
     rc.y written `*(u16 *)(p + 0x6) + 0xE0` in the arm   361 words; the three
       stores to the address-taken `rc` kill cse's entry and the load returns
       with its own nop.  The `ry` local must be assigned AFTER `rc.x`, not
       before, or its load fills the first `lhu`'s delay slot (357 words)
     an explicit rolling cursor `c` for the four tail primitives   361 words
     `z = r + 0x28` as its own local                      362 words
     `r = q; r += 0x28;` (two sets, no asm)               223 (r still folds)
     a `do { } while (0)` after `r = q + 0x28;`           223 (ditto)
     second AddPrim written `r - 0x28`                    no change
     `q[0x7] |= 2;` moved before the memcpy               224
     dropping the `base` local                            no change
     declaration orders q/e/r, e/q/r, q/r/e               no change
     s32 for the sign-block locals                        356 words (`lb`)
     chained `sp->unk_80 = sp->unk_70 = a = ...`          no change
     `u8 flag`                                            359 words
     the `|= 0x100` moved first or last in its group      no change 
   ---------------------------------------------------------------- 2026-11-16

   THE UnkDungeonSPB TYPEDEF WAS LOST.  It existed only in its author's
   hand-back, so this park had not compiled from the repo at all and was
   invisible to parkresidue's ranking.  Recovered below from the target's own
   listing, which writes down every offset and every width:

     python3 - <<'EOF'
     grep -oE '(lw|lh|lhu|lb|lbu|sw|sh|sb) +\$[a-z0-9]+, \(0x1F800[0-9A-F]{3} & 0xFFFF\)' \
         asm/nonmatchings/ovl_dungeon/func_8009E798.s
     EOF

   offset  instructions seen        conclusion
     0x00  lh, lhu, sh              halfword, read BOTH ways -- see below
     0x02  lhu, sh                  u16
     0x08  lw, sw, lhu              UnkDungeonSPW (word written, halfword read)
     0x0C  lw, sw, lhu              UnkDungeonSPW
     0x10  lw, sw, lhu              UnkDungeonSPW
     0x14  lw, sw, lhu              UnkDungeonSPW
     0x20  lw, sw                   the OT base, a pointer
     0x24  sh                       written only; signedness unobservable
     0x28, 0x2A, 0x2C   sh          written only
     0x30, 0x34, 0x38   sw          s32
     0x48  sw                       s32
     0x70..0x8C         sh          the six screen pairs, written only
     0x98, 0x9A, 0x9C   sh          written only
     0xE4, 0xE8, 0xEC   sw          s32
     0x100, 0x102       lhu, sh     u16

   DECLARATIONS
   typedef struct UnkDungeonSPB { s16 unk_00; u16 unk_02; u8 pad_04[0x4]; UnkDungeonSPW unk_08; UnkDungeonSPW unk_0C; UnkDungeonSPW unk_10; UnkDungeonSPW unk_14; u8 pad_18[0x8]; u8 *unk_20; s16 unk_24; u8 pad_26[0x2]; s16 unk_28; s16 unk_2A; s16 unk_2C; u8 pad_2E[0x2]; s32 unk_30; s32 unk_34; s32 unk_38; u8 pad_3C[0xC]; s32 unk_48; u8 pad_4C[0x24]; s16 unk_70; s16 unk_72; s16 unk_74; u8 pad_76[0x2]; s16 unk_78; s16 unk_7A; s16 unk_7C; u8 pad_7E[0x2]; s16 unk_80; s16 unk_82; s16 unk_84; u8 pad_86[0x2]; s16 unk_88; s16 unk_8A; s16 unk_8C; u8 pad_8E[0xA]; s16 unk_98; s16 unk_9A; s16 unk_9C; u8 pad_9E[0x46]; s32 unk_E4; s32 unk_E8; s32 unk_EC; u8 pad_F0[0x10]; u16 unk_100; u16 unk_102; } UnkDungeonSPB;
   s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);
   END DECLARATIONS

   (Written on one line because the DECLARATIONS block is read a line at a
   time.  Total size 0x104; every pad is derived, not guessed -- check the
   arithmetic before changing a field width.)

   WHAT IS STILL OPEN, and it is only the SPELLING, not the layout:

   `unk_00` is read `lh` at 8009E8A0 and `lhu` at 8009E914, so one face of it
   is wrong whichever way it is declared.  As `s16` the second read needs a
   `(u16)` cast; as `u16` the first needs `(s16)`.  Note that the body already
   writes `(s16)sp->unk_02` for a field the listing only ever reads `lhu`, so
   the cast does NOT fold to `lh` in this compiler -- which argues for `s16`
   here and a `(u16)` cast at the subtraction.  ONE COMPILE SETTLES IT; do
   that before anything else and record which way it went.

   The fields marked "written only" have no observable signedness.  s16 is the
   assumption; if a residue lands on one of them, that is the first thing to
   vary and it costs nothing.
*/
s32 func_8009E798(u8 *p, UnkVec *b, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPB *sp = (UnkDungeonSPB *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    u8 *q;
    u8 *r;
    u8 *e;
    s32 flag;
    s32 ry;
    s16 a;
    s16 bb;
    RECT rc;
    u8 *dr;

    q = *(u8 **)(base + 0x8D0);
    sp->unk_20 = base + 0xB0;
    sp->unk_38 = 0x1000;
    flag = base != D_801C9E40;
    sp->unk_48 = 0;
    func_80067E2C((DR_AREA *)q, (RECT *)base);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    e = (u8 *)s->unk_08;
    sp->unk_24 = s->unk_14;
    PushMatrix();
    sp->unk_98 = 0;
    sp->unk_9A = 0;
    sp->unk_9C = g->unk_C8;
    RotMatrix((SVECTOR *)0x1F800098, (MATRIX *)0x1F8000D0);
    ScaleMatrix((MATRIX *)0x1F8000D0, (VECTOR *)0x1F800030);
    sp->unk_E4 = sp->unk_00;
    sp->unk_E8 = (s16)sp->unk_02;
    sp->unk_EC = 0;
    func_80064CF0((MATRIX *)0x1F8000D0);
    func_80064D80((MATRIX *)0x1F8000D0);
    sp->unk_28 = 0x20 - s->unk_20;
    sp->unk_2A = 0x20 - s->unk_22;
    sp->unk_2C = 0;
    func_80065320((SVECTOR *)0x1F800028, (s32 *)0x1F800100, (s32 *)0x1F800094);
    q += 0xC;
    sp->unk_100 = sp->unk_100 - sp->unk_00;
    sp->unk_102 = sp->unk_102 - sp->unk_02;
    sp->unk_08.w = e[0x8];
    sp->unk_0C.w = e[0x9];
    sp->unk_10.w = e[0xA];
    sp->unk_14.w = e[0xB];
    a = (s8)e[0x2];
    sp->unk_80 = a;
    sp->unk_70 = a;
    a = a + sp->unk_10.h;
    sp->unk_88 = a;
    sp->unk_78 = a;
    bb = (s8)e[0x3];
    sp->unk_7A = bb;
    sp->unk_72 = bb;
    bb = bb + sp->unk_14.h;
    sp->unk_8A = bb;
    sp->unk_82 = bb;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    func_80065320((SVECTOR *)0x1F800070, (s32 *)(q + 0x8), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800078, (s32 *)(q + 0x10), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800080, (s32 *)(q + 0x18), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800088, (s32 *)(q + 0x20), (s32 *)0x1F800094);
    *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + sp->unk_100;
    *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + sp->unk_100;
    *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + sp->unk_100;
    *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + sp->unk_100;
    *(s16 *)(q + 0xA) = *(u16 *)(q + 0xA) + sp->unk_102;
    *(s16 *)(q + 0x12) = *(u16 *)(q + 0x12) + sp->unk_102;
    *(s16 *)(q + 0x1A) = *(u16 *)(q + 0x1A) + sp->unk_102;
    *(s16 *)(q + 0x22) = *(u16 *)(q + 0x22) + sp->unk_102;
    sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
    sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
    sp->unk_0C.w = sp->unk_0C.w << 8;
    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
    *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
    *(u32 *)(q + 0x14) =
        sp->unk_0C.w + sp->unk_10.w + ((s->unk_10 | *(s16 *)(e + 0x4)) << 16);
    *(s16 *)(q + 0x1C) = sp->unk_14.h + sp->unk_08.h;
    *(s16 *)(q + 0x24) = sp->unk_14.h + sp->unk_10.h;
    *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
    func_800666F4((POLY_FT4 *)q);
    __asm__("" : "=r"(q) : "0"(q));
    r = q + 0x28;
    __asm__("" : "=r"(r) : "0"(r));
    func_8003DB6C((int *)r, (int *)q, 0xA);
    q[0x7] |= 2;
    r[0xD] = r[0x15] += 0x40;
    *(u16 *)(r + 0x16) |= 0x100;
    r++;
    r--;
    r[0x1D] = r[0x25] += 0x40;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    *(u32 *)(r + 0x2C) = 0x606060;
    r[0x2C] >>= 1;
    r++;
    r--;
    r[0x2D] >>= 1;
    r[0x2E] >>= 1;
    *(s32 *)(r + 0x30) = *(s32 *)(r + 0x8);
    *(s32 *)(r + 0x34) = *(s32 *)(r + 0x10);
    *(s32 *)(r + 0x38) = *(s32 *)(r + 0x18);
    *(s32 *)(r + 0x3C) = *(s32 *)(r + 0x20);
    func_800666E0((POLY_F4 *)(r + 0x28));
    r[0x2F] |= 2;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x28);
    func_80067F20((u32 *)(r + 0x40), 0, 0, 0x40, 0);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x40);
    rc.x = *(u16 *)(p + 0x4);
    ry = *(u16 *)(p + 0x6);
    dr = r + 0x4C;
    rc.y = ry;
    rc.w = *(u16 *)(p + 0x8);
    rc.h = *(u16 *)(p + 0xA);
    if (flag != 0)
    {
        rc.y = ry + 0xE0;
    }
    func_80067E2C((DR_AREA *)dr, &rc);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r + 0x4C);
    PopMatrix();
    if (flag)
    {
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x58);
    }
    else
    {
        *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x58);
    }
    return 0;
}
