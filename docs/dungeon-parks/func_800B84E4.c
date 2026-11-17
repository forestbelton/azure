/* PARK: func_800B84E4, 211 of 211 words, 5 aligned differences (ovlverify
   reports 4 masked).  ovl_dungeon.  Measured 2026-11-16 by batch j56, on top
   of the 211/10 j48 left.  WAS 10 aligned; the two STRUCTURAL residues below
   are now closed and what is left is one register-free ordering.

   THE HEADER OF THIS FILE WAS NOT VALID C BEFORE j56.  An earlier repair
   removed an inner comment opener but left its closer, so everything from the
   old line 24 to line 154 sat at top level as bare text, and clang-format had
   already mangled parts of it (`docs / dungeon - parks /').  parkcheck.py
   only looks for an inner comment opener, so it passed.  The whole header is now ONE
   comment and every inner comment marker is written (* ... *).

   The fan/arc emitter func_800B835C calls: one POLY_GT4 per step of a
   `*(u16 *)(p + 0xE)'-step sweep, each quad spanning the previous step's
   screen point and the current one, with the u/v pair walked by rsin/rcos
   of the step angle.

   SIGNATURE, pinned by docs/dungeon-parks/func_800B835C.c:
   `void func_800B84E4(s16 *, u8 *, u8 *, s32);'.  The first argument is read
   as `*(u16 *)a' / `*((u16 *)a + 1)' / `*(s32 *)a' -- all three loads in the
   listing are `lhu'/`lw', never `lh', so the reads are UNSIGNED even though
   the caller hands it an `s16 w[2]'.

   NEW DECLARATIONS: three typedefs and one prototype, all at the bottom of
   this file above the body, to be pasted into the unit's top block after
   UnkDungeonSPXY.  NAME COLLISION RESOLVED 2026-11-16: UnkDungeonSPF was
   called UnkDungeonSP9 until func_800BC4D4 landed with a DIFFERENT struct
   under that name.  Both describe the 0x1F800000 scratchpad as their own
   function lays it out, so both are right and only the name was wrong.
   UnkDungeonSP9 belongs to func_800BC4D4 and is in csrc/ovl_dungeon.c.
   Check the SP-name list in csrc before inventing another: SP, SP2..SP9,
   SPA, SPC, SPD, SPW, SPXY are taken.

   ---------------------------------------------------- WHAT j56 ADDED: 10 -> 5

   TWO LEVERS, both of them scheduling, both free of instructions.

   (1) `__asm__("");' IMMEDIATELY AFTER THE LOOP-TOP COPY
       `sp->unk_70.w = sp->unk_78.w;'.  Worth 3 of the 10 rows and the one
       word that paid for them.  A traditional asm -- no operands at all, so
       ASM_INPUT rather than ASM_OPERANDS -- makes sched.c flush every
       pending read and write list and depend on all of memory and all
       registers (sched.c:1943-1970).  The copy therefore comes out as the
       target's serial `lw' / `nop' / `sw' instead of being interleaved into
       the first UV chain's load-delay slots, and it emits NOTHING.  This is
       the lever the old header called unreachable: it concluded, correctly,
       that no ALIAS route exists for two members of one struct, and then
       stopped.  The barrier is not an alias route.
       Measured alone on the old body: 212 words / 7 aligned -- one word OVER,
       because it exposes residue (2) below as a `nop'.

   (2) `dd = (u16)d;' HOISTED OUT OF THE `if' TO JUST BEFORE IT, with `i = 0'
       and `ang = 0' left inside.  Worth the other 2 rows and the word back.
       The old body left the `ot' parameter copy (`addu $fp, $a2, $zero',
       emitted by assign_parms at the top of the function with no dependent
       and the smallest LUID in the block) as the LAST insn of block 0, so
       sched2 used it for the `lhu 0xE' -> `beqz' stall and reorg then took
       it for the guard's delay slot -- leaving a `nop' at word 37, which is
       where the target puts the copy.  Hoisting `dd' gives block 0 one more
       ready, dependent-free insn with a LARGER LUID; sched2 takes the `andi'
       for that last slot instead, the `ot' copy drops back into the
       `lbu $v0, 0xB($s3)' load-delay slot at word 37, and reorg fills the
       delay slot from the `andi'.

   ---------------------------------------------- THE RESIDUE, 5 aligned rows

   All five are in the guard's delay slot and the three-insn preheader, and
   the register assignment is now CORRECT everywhere (i = $s5, dd = $s6,
   ang = $s1, as the target has them; words 66/159/162/179/190 match).

     built   42 andi $s6,$a3,0xffff   (delay)   target 42 addu $s5,$zero,$zero
            43 addu $s5,$zero,$zero          43 andi $s6,$a3,0xffff
            44 addu $s1,$s5,$zero            44 addiu $s0,$s4,0x7
            45 addiu $s0,$s4,0x7             45 addu $s1,$s5,$zero

   Two independent facts are left:
     a) the target's delay slot is `i = 0', taken from the FALL-THROUGH,
        which means its reorg found nothing usable before the branch at all.
        Ours finds the `andi'.  The only way to deny reorg the `andi' is to
        put something unusable after it, and every such thing measured costs
        a word (see the asm-device rows below).
     b) the target emits the loop.c-hoisted invariant `addiu $s0, $s4, 0x7'
        BEFORE `ang = i', i.e. that invariant has a SMALLER LUID than the
        `ang = 0' statement.  loop.c inserts invariants immediately before
        NOTE_INSN_LOOP_BEG, which is after all three preheader statements, so
        no ordering of {dd, i, ang} can produce it.  `ang' being a derived
        induction variable of `i' in the original source -- so that loop.c
        emits its initialisation itself, after the invariants -- is the only
        story that fits, and it was not tried.

   ------------------------------------- what paid, in the order it paid (j48)

   175 -> 23 -> 16 -> 11 -> 10 -> 5 differing.

   * THE LOOP IS A `do'/`while' INSIDE AN EXPLICIT `if', with `i' and `ang'
     as locals initialised INSIDE the if.  Worth 152 of the 175.  Written as
     `for (i = 0; i < *(u16 *)(p + 0xE); i++)' with `(u16)d' inline, gcc
     hoists the `andi' as a loop invariant and the parameter `d' survives in
     its own callee-saved register -- ten values for nine registers, so `ot'
     spills and the body carries an extra `lw $a0, 0x40($sp)' before
     func_8006658C.  (* j56 note: `dd' is now hoisted out of the `if' as a
     STATEMENT, which is not the same thing -- the `andi' still reads $a3
     directly and `d' still dies there. *)
   * A POINTER LOCAL `Unk80083160 *g = &D_80083160;' USED ONLY IN THE TAIL.
     23 -> 16 and the last missing word.  The target's tail is
     `lui $t0, %hi / addiu $t0, %lo / lw $v0, 0x0($t0)' -- three words, which
     is reload REMATERIALISING a pseudo it could not give a hard register to
     (nine callee-saved registers are already spoken for).  Spelled
     `D_80083160.unk_00' the tail folds to `lui $v0 / lw %lo($v0)', two
     words.  The head still reads `D_80083160.unk_00' directly: cse folds
     that one because the def is in the same block.
   * HOISTING `*(u16 *)(p + 0x4)' INTO A LOCAL before the tpage store.  Read
     twice (once for the unconditional store, once inside the `if (dd)'
     arm), the second read is re-emitted as its own `lhu' and the function
     is one word long.
   * `i = 0;' AS THE SECOND STATEMENT INSIDE THE `if' was worth 16 -> 11 on
     the PRE-BARRIER body, because `ang = 0' written after `i = 0' is
     copy-propagated to `ang = i' (`addu $s1, $s5, $zero'), which is what the
     target has.  That is still true; with `dd' hoisted out, `i = 0' is now
     simply the first of the two statements left inside.

   ----------------------------------- measured and rejected by j48, no better

   All at 211 words against the 10-aligned pre-barrier base:

   * declaration order of the locals: dd,i,ang / i,dd,ang / ang,dd,i all 10.
     (* j56 re-ran all SIX declaration orders on top of the barrier + the
     `i'-hoist variant: all 10, identical.  Declaration order is dead here. *)
   * `ot' copied into a local at the top: 10.  `ot' declared `void *': 10.
   * `q' declared before `sp': 10.  `sp' assigned as a statement rather than
     an initialiser: 10.  Reading `q' AFTER the scratchpad stores instead of
     before: 208 words / 62.
   * the loop-top copy as a union AGGREGATE assignment (`sp->unk_70 =
     sp->unk_78;'): 10, byte-identical to the `.w' form.
   * the loop-top copy through `*(volatile s32 *)': 10.  A volatile store
     does not serialise against a non-volatile load -- sched.c's
     `true_dependence' needs BOTH sides volatile.
   * the loop-top copy via a temp local: 14.  Moved after the first UV pair:
     212 words / 11.  As a 4-byte STRUCT assignment, both `struct { u16
     h[2]; }' and `struct { u16 a, b; }': 214 words / 12 -- gcc expands both
     as two halfword moves, not one word move.
   * a SECOND pointer local `u8 *b = (u8 *)0x1F800070;' for the 0x70/0x78
     region, to break the alias disambiguation: 209 words / 98.
   * the whole scratchpad through `u8 *sp' with casts instead of the struct:
     206 words / 34.  The struct spelling is load-bearing.
   * `dd = d & 0xFFFF;' for `dd = (u16)d;': 10, identical.
   * the guard as `if (*(u16 *)(p + 0xE))': 10, identical.
   * `g' assigned as a STATEMENT before the `q' read: 10; after it: 10; just
     before the tail store: 210 words / 17 -- the local must be live across
     the loop for reload to rematerialise it.
   * the four `sp->unk_08..unk_14' scratchpad stores moved ABOVE the two
     `sp->unk_78' stores: 211 / 26.  The two `sp->unk_74 = 0; sp->unk_7C = 0;'
     zero stores moved BELOW the `sp->unk_78' stores: 209 / 18.

   The alias route into sched1 is a dead end and the reason is in the
   compiler: both references are MEM_IN_STRUCT_P at `(plus (reg) (const_int))'
   off the SAME pseudo, so `memrefs_conflict_p' proves them distinct, and the
   MEM_IN_STRUCT_P exemption in `true_dependence' (sched.c:835-857) only ever
   CLEARS a dependence.  The barrier in lever (1) is what creates one.

   ------------------------------------------- measured and rejected by j56

   On top of the loop-top barrier unless stated.  Base for comparison:
   barrier alone = 212 words / 7 aligned; barrier + `dd' hoist = 211 / 5.

   * `__asm__("" : "=r"(ot) : "0"(ot));' -- a non-volatile self-copy device
     that re-LUIDs the `ot' copy.  It DOES put the copy at word 37, from any
     of NINE source positions (before the `q' read, after each of the eight
     prologue stores): all 212 words / 6 aligned, byte-identical.  The word
     it costs is reorg's: the `#APP' insn is then the last thing before the
     branch, reorg cannot use it, and `fill_eager_delay_slots' DUPLICATES the
     branch target's `lui $t0, %hi(D_80083160)' into the delay slot and
     redirects the branch past it.  A plain `__asm__("");' in block 0 does
     the same thing: after `sp->unk_14.w = p[0xB];' 212 / 6, after any of the
     three earlier UV stores 212 / 8.
   * a second `__asm__("");' at the top of the TAIL, to deny reorg that
     `lui': 212 / 6 and 212 / 7.  It does not stop the duplication.
   * `u8 *o = ot;' as a statement after the `unk_10' or `unk_14' store, with
     `o' passed to func_8006658C: 212 / 7 both.  gcc folds the copy away.
   * ALL SIX orderings of {dd, i, ang} inside the `if', on the barrier body:
     dd,i,ang 7; dd,ang,i 7; ang,dd,i 9; i,ang,dd 12; ang,i,dd 12;
     i,dd,ang 14.  All 212 words.  On the device body: 6 / 6 / 6 / 11 / 11
     / 12, all 212.
   * swapping the `sp->unk_10' and `sp->unk_14' stores: 212 / 12.
   * `i = 0' hoisted out of the `if' instead of `dd' (211 words / 10), and
     with the inner pair reordered to `ang = 0; dd = (u16)d;' (211 / 9).
     Both get the `ot' copy to word 37 and reorg to the fall-through, but
     they SWAP the registers: `i' becomes $s6 and `dd' $s5, against the
     target's $s5/$s6, and that one allocation decision is seven of the rows
     (42, 43, 66, 159, 162, 179, 190).  The cause is global.c's priority,
     which divides by live length: hoisting `i' lengthens its range, drops
     its priority below `dd''s, and `dd' is then allocated first into the
     lower-numbered register (MIPS has no REG_ALLOC_ORDER, so allocation runs
     $16, $17, ... in order).  Hoisting `dd' instead lengthens the range of
     the one the target also allocates second.  Declaration order does not
     move it (all six measured, above).
   * BOTH `dd' and `i' hoisted before the `if', either order, and with `dd'
     at any of four earlier positions: 210 words / 13 -- one word SHORT, and
     the registers are still swapped.
   * `dd' hoisted and a barrier placed between it and the `if' (to deny
     reorg the `andi'): 211 / 7 -- the barrier pins the `andi' at word 37
     where the copy belongs, and reorg duplicates the tail `lui' again.
   * `dd' hoisted to four earlier positions in block 0 (before the `q' read,
     before `sp->unk_74', before `sp->unk_08', after `sp->unk_14'): all
     211 / 5, byte-identical to the version below.  sched2 sinks the `andi'
     to the end of block 0 from anywhere.

   WHAT TO TRY NEXT.  Residue (b) -- `ang' as a derived induction variable
   rather than a second counter -- is the one untried story, and it would
   also explain why the target's `addu $s1, $s5, $zero' sits after loop.c's
   invariant.  Residue (a) needs reorg to find NOTHING before the branch
   while block 0 still has an insn to displace the `ot' copy; every device
   tried so far is itself the thing reorg trips over. */

/* ---- the new declarations this body needs, verbatim ---- */
/* A scratchpad word written whole and read back as its low BYTE and as the
   signed word -- the byte-wide face of the slot UnkDungeonSPW gives a
   halfword face to. */
typedef union UnkDungeonSPUV
{
    s32 w;
    u8 b;
} UnkDungeonSPUV;
/* The 0x1F800000 scratchpad as func_800B84E4 lays it out: the four UV
   scalars at +0x08..+0x17 and two packed screen pairs at +0x70 and +0x78,
   each the XY half of an 8-byte slot whose Z halfword is zeroed. */
typedef struct UnkDungeonSPF
{
    u8 pad_00[0x8];
    UnkDungeonSPUV unk_08;
    UnkDungeonSPUV unk_0C;
    UnkDungeonSPUV unk_10;
    UnkDungeonSPUV unk_14;
    u8 pad_18[0x58];
    UnkDungeonSPXY unk_70;
    s16 unk_74;
    u8 pad_76[0x2];
    UnkDungeonSPXY unk_78;
    s16 unk_7C;
} UnkDungeonSPF;
/* The same 0x34-byte POLY_GT4 as UnkDungeonGT4 above with its four screen
   pairs under their WORD face and ALL FOUR u/v pairs byte-addressed:
   func_800B84E4 copies each screen pair in with one `sw' and reads its own
   u0/v0 back a byte at a time. */
typedef struct UnkDungeonGT4W
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    Unk453E0UV unk_0C;
    u16 unk_0E;
    u32 unk_10;
    s32 unk_14;
    Unk453E0UV unk_18;
    u16 unk_1A;
    u32 unk_1C;
    s32 unk_20;
    Unk453E0UV unk_24;
    u16 unk_26;
    u32 unk_28;
    s32 unk_2C;
    Unk453E0UV unk_30;
    u16 unk_32;
} UnkDungeonGT4W;
/* func_800B84E4's per-primitive hand-off, beside func_8006658C's. */
void func_8006671C(void *);


void func_800B84E4(s16 *a, u8 *p, u8 *ot, s32 d)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPF *sp = (UnkDungeonSPF *)0x1F800000;
    UnkDungeonGT4W *q;
    s32 i;
    s32 ang;
    s32 dd;
    s32 t;
    s32 tp;

    q = *(UnkDungeonGT4W **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_74 = 0;
    sp->unk_7C = 0;
    sp->unk_78.h[0] = *(u16 *)a + p[0xC];
    sp->unk_78.h[1] = *((u16 *)a + 1);
    sp->unk_08.w = p[0x8];
    sp->unk_0C.w = p[0x9];
    sp->unk_10.w = p[0xA];
    sp->unk_14.w = p[0xB];
    dd = (u16)d;
    if (*(u16 *)(p + 0xE) != 0)
    {
        i = 0;
        ang = 0;
        do
        {
            sp->unk_70.w = sp->unk_78.w;
            __asm__("");
            t = sp->unk_08.b + (sp->unk_10.w >> 1);
            q->unk_18.b[0] = t;
            q->unk_0C.b[0] = t;
            t = sp->unk_0C.b + (sp->unk_14.w >> 1);
            q->unk_18.b[1] = t;
            q->unk_0C.b[1] = t;
            i++;
            q->unk_30.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_30.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 13);
            ang += 0x1000;
            sp->unk_78.h[0] = *(u16 *)a + ((p[0xC] * func_80064584(ang / *(u16 *)(p + 0xE))) >> 12);
            sp->unk_78.h[1] = *((u16 *)a + 1) + ((p[0xD] * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 12);
            q->unk_08 = q->unk_14 = *(s32 *)a;
            q->unk_20 = sp->unk_78.w;
            q->unk_2C = sp->unk_70.w;
            q->unk_24.b[0] = q->unk_0C.b[0] + ((sp->unk_10.w * func_80064584(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_24.b[1] = q->unk_0C.b[1] + ((sp->unk_14.w * func_800644B8(ang / *(u16 *)(p + 0xE))) >> 13);
            q->unk_0E = *(u16 *)(p + 0x6);
            tp = *(u16 *)(p + 0x4);
            q->unk_1A = tp;
            if (dd != 0)
            {
                q->unk_1A = (tp & 0xFF9F) | ((dd - 1) << 5);
            }
            q->unk_04 = *(u32 *)(p + 0x10);
            q->unk_10 = *(u32 *)(p + 0x10);
            q->unk_1C = *(u32 *)(p + 0x14);
            q->unk_28 = *(u32 *)(p + 0x14);
            func_8006671C(q);
            if (dd != 0)
            {
                *((u8 *)q + 0x7) |= 2;
            }
            func_8006658C(ot, q);
            q++;
        } while (i < *(u16 *)(p + 0xE));
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}
