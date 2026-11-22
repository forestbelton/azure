/* PARK: func_800AFBFC, 238 of 238 words, 2 aligned differences.

   ====================================== 2026-11-21: 116 rows -> 2

   Measured with tools/decomp/aligndiff.py and confirmed in a real unit copy:
   `ovlverify.py ... func_800AFBFC' -> target 238 words, candidate 238 words,
   2 differing (masked).  All four sweeps are CLEAN on this body (widthsweep,
   stmtsweep, refsweep, devsweep, each re-run after the last change).

   THE WHOLE 116-ROW RESIDUE WAS THREE SOURCE FACTS AND FIVE EARLIER DEVICES.
   The five devices this park carried in (two `do{}while(0)'s, two duplicated
   `if' arms and an `m++; m--;') were all DELETED; the body below has one
   device left (see "the one device" at the bottom).  Starting point was 238
   words / 116 rows; the device-free version of that body was 227/149.

   ------------------------------------------------------ cluster breakdown

   AT 116 ROWS, four clusters, every row of them an allocation or schedule
   consequence -- the control flow, store offsets, branches and both loop
   shapes already matched:
     (1) ~60 rows   a permutation of $s0..$s6 over {sp, k, x, y, x2, y2, ang}
     (2) ~25 rows   four missing `addu' copies (x/y written through a temp in
                    the target's second halves) and the missing seed copy
     (3) ~12 rows   the backward loop's `lhu'/schedule and a missing `nop'
     (4) ~15 rows   the two duplicated-arm devices' own extra words
   AT 2 ROWS, one cluster: 800AFE54/800AFE5C, `lhu $s0,0x18($sp)' +
   `addiu $s1,$s0,-0x80' where the target has `$t0'.  Same two instructions,
   same operands, one register name.  See "what is left".

   ------------------------------------------- the changes, and what each paid

   Each number is rows on the body that existed when the change was measured.

   a) `s16 x, y, x2, y2;' (were `s32').  227/149 -> 236/93 on the DEVICE-FREE
      body, and it is what makes the five devices unnecessary.
      MECHANISM, and it is worth knowing because it reappears everywhere in
      this function: with an SImode RHS and a HImode destination,
      `store_expr' (expr.c:3221, `temp = expand_expr (exp, target, GET_MODE
      (target), 0);') cannot compute into TARGET -- `expand_binop' sees a
      target whose mode is not the operation's mode and allocates its own
      pseudo -- so the value lands in an SImode temp and the tail of
      store_expr copies it into the HImode variable.  The target's four
      `addu $s4,$s0,$zero' / `addu $s3,$v1,$zero' at 800AFDB8, 800AFDD4,
      800AFF14 and 800AFF30 ARE those copies; cse then rewrites the `sh's
      that follow to read `(subreg:HI temp)' rather than the variable, which
      is why they name a different register from the one the copy wrote.
      In the FIRST half of each loop the same copy exists and is invisible:
      temp and variable get one hard register and final.c drops the no-op
      move.  s32 x/y/x2/y2 cannot produce those four words at all.
      Per-variable: `s16 x' alone 234/114, `s16 y' 229/152, `s16 x2' 227/140,
      `s16 y2' 227/138, and every PAIR measured exactly 227/149 (= base).
      All four together is the only win.  u16 scores identically to s16.

   b) A SEPARATE `s32 bs' forward-loop accumulator, seeded `bs = ang;'.
      236/93 -> 237/83.  This is the park's old rejected line "separate `s32
      bs' seed local does not produce it (227 words)" -- TRUE of the s32
      x/y body it was measured on, FALSE here.  It is the target's
      `addu $s6,$s0,$zero' at 800AFCC4.

   c) A REDUNDANT `bs = k;' IMMEDIATELY BEFORE `k += 0x80;'.  237/83 -> 35,
      the single biggest change in this run, and it is a cse fact, not a
      pressure fact:
        cse.c:5621 `from_plus:' associates `(plus REG C2)' into the inner
        `(plus X C1)' whenever `lookup_as_function (folded_arg0, PLUS)'
        finds one, and the one exemption -- cse.c:5655-5669, equal constants
        that are powers of two -- is guarded at cse.c:5662 by
        `#if defined(HAVE_PRE_INCREMENT) || defined(HAVE_POST_INCREMENT)',
        and config/mips/mips.h:2381 and :2385 leave both COMMENTED OUT.  So on this compiler `k = bs + 0x80; ...; k += 0x80;'
        ALWAYS folds to `k = bs + 0x100', and the park's body duly emitted
        `addiu $s0,$s6,0x100' where the target has `addiu $s1,$s1,0x80'.
        That fold also kills k's live range across the calls, which is what
        put k at the TOP of the allocation order (18 refs / 9 insns ->
        priority 80000) and rotated every other callee-saved register.
      Writing to `bs' first invalidates the table entry `(plus bs 0x80)', so
      the association cannot happen and the increment stays in place.  flow
      then deletes the store, so it costs NOTHING: 237 words before and after.

   d) THE SAME TRICK FOR THE BACKWARD LOOP, as `ang = k;' before `k -= 0x80;'
      (35 -> 34, and it is the `addiu $s1,$s1,-0x80' at 800AFED8).  A dead
      store to `m' does NOT work (measured, 35 rows, no change): `m' is u16,
      so the loop top is `Z = zero_extend(m); k = Z - 0x80' and k's class
      names the UNNAMED pseudo Z, not m.  The association breaker has to
      write the same C variable the subtraction reads.

   e) `do { ang = m; } while (0);' at the backward loop top.  From
      tools/decomp/devsweep.py.  12 -> 2 on the final body; see "the one
      device".

   f) `ang <<= 16; ang >>= 16;' in the head instead of `(s16)ang' at the two
      trig calls, and `k <<= 16; k >>= 16;' in the forward loop's second half
      instead of `(s16)k'.  10 -> 5 and 5 -> 3 respectively.  Two different
      mechanisms:
        * In the head it makes the NARROWING WRITE `ang' ITSELF, so ang dies
          at the `sll' and the target's `sll $s0,$s0,16' is in place.  As
          `ang = (s16)ang;' it is 10 rows; as the shift pair, 5.
        * In the loop it gives the `sll'/`sra' a LOWER INSN_LUID than the two
          quad stores that follow, which is the whole fix for the one store
          the scheduler was hoisting above them at 800AFD94 (sched.c ranks
          within a priority group, and these are all priority 1 off the
          block head).

   g) `ang = a[6]; ang += 0xC00;' as two statements (was `ang = a[6] +
      0xC00;').  8 -> ... it is worth 2 rows: the target's `lbu $s0,0x6($t0)'
      + `addiu $s0,$s0,0xC00' is an IN-PLACE update of one pseudo; the single
      statement gives `lbu $v1' + `addiu $s0,$v1,0xC00'.

   h) REUSING `ang' AS THE BACKWARD LOOP'S TEMP (it was a separate `s32 z').
      3 -> 2.  MECHANISM: cse.c:838 `make_regs_eqv' only makes the NEW
      register the canonical one (`qty_first_reg[q] = new') when, at
      cse.c:866-869, `uid_cuid[regno_last_uid[new]] >
      uid_cuid[regno_last_uid[firstr]]'.
      With `bs = ang;' in the head and bs last used inside the forward loop,
      bs outlived ang and cse rewrote the head's `sll' to read BS -- the
      target reads ANG.  Giving `ang' a reference after the forward loop
      flips that test back.  A dead `ang = 0;' placed after the forward loop
      does it just as well (also 2 rows) and keeps `z'; both leave the same
      two rows, so this spelling was kept because it is one local fewer.

   ------------------------------------------------- what is left, and why

   800AFE54  lhu s0,24(sp)        vs  lhu $t0, 0x18($sp)
   800AFE5C  addiu s1,s0,-128     vs  addiu $s1, $t0, -0x80

   `ang' is the pseudo that reads the spilled `m' at the backward loop top.
   Every other register in the function now agrees with the target:
   `.greg' gives 83(sp) in 18, 85(p) in 21, 88(cx) in 30, 89(cy) in 23,
   92(ang) in 16, 93(k) in 17, 94(bs) in 22, 96(x) in 20, 97(y) in 19, and
   95(m) spilled -- all nine identical to the listing.

   THEORY: in the target that temp is a DIFFERENT pseudo from the head's
   `ang'.  The head's ang is $s0 there too (`lbu $s0,0x6($t0)'), so the
   backward temp cannot also be $s0; it is $t0, i.e. a short-lived quantity
   that conflicts with $v0..$a3 but not with $t0.  Our `ang' spans the head,
   where $t0 is live across the `lui/addiu %hi/%lo(D_80083160)' + `lhu
   0xC8' sequence, so $t0 is excluded for it and it takes $s0.  Splitting the
   two roles is exactly what the `z' spelling does -- and then `z' is
   block-local, local-alloc scans hard regs from 0 upward (no REG_ALLOC_ORDER
   on MIPS, local-alloc.c `find_free_reg') and hands it $v0.  So the target's
   temp is neither block-local nor as long-lived as our ang.  What would do
   it is a pseudo referenced in the backward loop top AND in the block right
   after the first call (where $v0 is live), and every spelling I tried for
   that second reference was deleted before local-alloc ran.

   MEASURED AND REJECTED FOR THAT LAST PAIR, all on the 2/3-row body:
     separate `s32 z' + dead `ang = 0;' after the forward loop     2 (same)
     `z' + dead `ang = k;' in the backward second half             2 (same)
     `z = k; k -= 0x80; z = k; m = z;'                             2 (same)
     `z = k; k -= 0x80; m = k; z = m;'                             2 (same)
     `z = k; k = z - 0x80; m = k;'                                 2 (same)
     `z = k - 0x80; k = z; m = z;'                                16
     backward temp = `bs'                                         24
     backward temp = `f'                                           9
     backward temp = `v'                                          10
     `u16 z' / `s16 z'                                     23 / 14 at 240 words
     no temp at all, `k = m - 0x80;' + `k <<= 16; k >>= 16;'      14 at 237
     same, wrapped in do-while                                     7
     `k <<= 16; k >>= 16;' in the backward FIRST half too          6
     `k <<= 16; k >>= 16;' in the forward FIRST half too           6

   -------------------------------------------------------- the one device

   `do { ang = m; } while (0);' at the backward loop top is the only device
   left.  Removing it: 12 rows at 237 words (it is worth 10 rows and the
   238th word).  It is a FULL SCHEDULING BARRIER and weights every reference
   inside it by loop_depth (flow.c:2128); what it buys here is the target's
   `nop' at 800AFE58 -- without it the scheduler fills the `lhu's load-delay
   slot with the first of the four quad stores and the whole block shifts.
   Device-free alternatives measured on this body:
     `if (p) {ang = m;} else {ang = m;}'    89 rows, 245 words
     `if (m) {ang = m;} else {ang = m;}'    61 rows, 241 words
     `ang = m; ang++; ang--;'               59 rows, 237 words
     do-while around `ang = m;' AND `k = ang - 0x80;' together   4 rows
   devsweep re-run on the final body reports "no wrapper helps"; refsweep
   reports "no cancelled pair helps"; widthsweep and stmtsweep are clean too.

   The two REDUNDANT ASSIGNMENTS (`bs = k;' before `k += 0x80;' and
   `ang = k;' before `k -= 0x80;') are not devices in the cancelled-pair
   sense -- they are real stores that flow deletes, they cost zero words, and
   removing them is not an option: without the first the body is 3 rows and
   without the second it is 148 rows at 248 words.  But they ARE a shape I am
   not happy about, because the original source surely wrote something that
   happens to invalidate cse's table entry rather than a redundant store.  I
   could not find what: every in-place/narrowing/reordering spelling of the
   two accumulators that does not write the accumulator name scores worse
   (see the rejected list above and item (d)).

   ------------------------------------------------------------- unchanged

   Everything the earlier header called settled is still settled and still
   measured on THIS body unless listed above: `s32 cx'/`s32 cy' (u16 is
   234 words), `u16 m' with `(s16)m' at the backward seed, a separate `q' for
   the call result with `p = q;' after the test, `u8 *g = (u8 *)&D_80083160;'
   as a pointer local, the `(u16)' read of unk_C8, the texture byte read once
   into `v', and the two-two-segment loop shape.

   DECLARATIONS: none are required.  `func_800AFFB4' is defined BELOW this
   function in the unit and the call rides its implicit `int' declaration;
   adding the honest prototype
       POLY_GT4 *func_800AFFB4(u8 *, void *, UnkDungeonSP5 *, POLY_GT4 *, s16);
   measures IDENTICALLY (238 words, 2 rows), so it can be added or not.  The
   unit no longer carries a `void func_800AFBFC();' forward declaration, so
   there is no prototype change to make. */

s32 func_800AFBFC(u8 *a, void *b, u8 *c)
{
    UnkDungeonSP5 *sp;
    u8 *g;
    POLY_GT4 *p;
    POLY_GT4 *q;
    u8 *t;
    s32 cx;
    s32 cy;
    s32 f;
    s32 v;
    s32 ang;
    s32 k;
    s32 bs;
    u16 m;
    s16 x;
    s16 y;
    s16 x2;
    s16 y2;

    g = (u8 *)&D_80083160;
    sp = (UnkDungeonSP5 *)0x1F800000;
    p = *(POLY_GT4 **)(*(u8 **)g + 0x8D0);
    sp->unk_20 = (u32)(*(u8 **)g + 0x8B0);
    t = *(u8 **)(c + 0x8);
    sp->unk_118 = (u32)t;
    cx = *(u16 *)(g + 0xAC);
    cy = *(u16 *)(g + 0xAE);
    v = t[1];
    c[0xF] = v;
    f = *(u16 *)(c + 0x14);
    if (f & 8)
    {
        c[0xF] = (f & 4) ? (v | 2) : (v & 0xFD);
    }
    ang = a[6];
    ang += 0xC00;
    ang -= (*(u16 *)(g + 0xC8) + 0x80) & 0xF00;
    m = ang;
    bs = ang;
    ang <<= 16;
    ang >>= 16;
    x = func_80064584(ang) * 6 - cx;
    y = func_800644B8(ang) * 6 - cy;
    while (1)
    {
        k = bs + 0x80;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
        bs = k;
        k += 0x80;
        bs = k;
        k <<= 16;
        k >>= 16;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        x = func_80064584(k) * 6 - cx;
        y = func_800644B8(k) * 6 - cy;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        q = func_800AFFB4(a, c, sp, p, 1);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    x = func_80064584((s16)m) * 6 - cx;
    y = func_800644B8((s16)m) * 6 - cy;
    while (1)
    {
        do
        {
            ang = m;
        } while (0);
        k = ang - 0x80;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y;
        x2 = func_80064584((s16)k) * 6 - cx;
        y2 = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x2;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y2;
        q = func_800AFFB4(a, c, sp, p, 1);
        if (q == NULL)
        {
            break;
        }
        p = q;
        ang = k;
        k -= 0x80;
        m = k;
        sp->unk_70[1].vx = sp->unk_70[3].vx = x2;
        sp->unk_70[1].vy = sp->unk_70[3].vy = y2;
        x = func_80064584((s16)k) * 6 - cx;
        y = func_800644B8((s16)k) * 6 - cy;
        sp->unk_70[0].vx = sp->unk_70[2].vx = x;
        sp->unk_70[0].vy = sp->unk_70[2].vy = y;
        q = func_800AFFB4(a, c, sp, p, 0);
        if (q == NULL)
        {
            break;
        }
        p = q;
    }
    *(u32 *)(*(u8 **)g + 0x8D0) = (u32)p;
    return 0;
}
