/* PARK: func_s53_80024B54, csrc/ovl_dng_s53.c, agent-h194, 2026-11-25.

   TARGET 506 words. BEST BUILT 507 words, 65 ALIGNED DIFFERENCES
   (tools/decomp/aligndiff.py). 1 member, 506 words (tools/decomp/dngfam.py:
   func_s53_80024B54 alone -- no replication value beyond its own body).

   THIS IS THE UNIT'S ROOT DISPATCHER and the only stub left in it; the other
   six in batch 35's list all landed. OWNS jtbl_s53_80024008: 5 words, 5
   cases, NO pad, dispatch bound `sltiu $v0, $v1, 0x5' with NO subtraction
   before it, so minval 0 and the arms are 0..4. The range-check branch
   `beqz $v0, .Ls53_80025308' targets the common tail, which is NOT one of
   the five entries, so there is no `default:' label and all five entries are
   real arms. Entries in order: 0 .Ls53_80024BBC, 1 .Ls53_80024BF0,
   2 .Ls53_80024C7C, 3 .Ls53_800250E8, 4 .Ls53_800252C0. Case 0 FALLS THROUGH
   into case 1 (its last insn is the func_s53_80024494 jal and case 1's label
   is the next word), and case 3's retire tail is cross-joined with case 4's.

   NO TWIN. dngtwin --unit ovl_dng_s53 reports no carved sibling at >= 0.70
   for this stub. The whole body was written from the listing plus the six
   bodies that landed beside it in this unit, which is where the spawner and
   step offsets come from: func_s53_80024494's payload stores ARE the fields
   case 0 seeds, and func_s53_800249E4's by-value MATRIX + SVECTOR prototype
   is what case 2's 14-argument call site is.

   WHAT IT DOES. Bumps the +0x10 frame counter, then dispatches on the +0xA
   state. State 0 zeroes +0x10, advances, clears D_800814A8 + 0xF4 and
   spawns the bolt through func_s53_80024494. State 1 waits for the room
   record's 0x80 bit, arms a 0xA-frame countdown at +0x18, drops
   D_800814A8 + 0xA6, publishes p[0x8] to D_800814A8 + 0xA8, copies the
   camera heading to +0xE, advances, and plays 0x300 or 0x4300 depending on
   whether func_80053EF0(4) returns 2. State 2 counts the countdown down,
   then picks a random phase, takes D_80083780 as the origin plus the
   D_800E3D18 facing entry for the camera octant, asks func_800A05A4 for a
   target: on a hit the aim point is that record's own vector at -0x18, on a
   miss it walks up to eight cells out along the camera heading through the
   D_8006CCD8 / D_8006CCE8 step tables, backing the last step off by half
   when func_800A45D8 refuses the cell. Either way it builds a bearing with
   func_s53_8002401C, a MATRIX with RotMatrix, and fires THREE
   func_s53_800249E4 nodes 0x555 apart in phase, arms 0x18 frames and
   advances. State 3 pumps the hit record's colour -- +8/-8/-8 per frame
   clamped to 0xE0/0x20 while the countdown is >= 0xC, then -8/+8/+8 clamped
   to 0x80 after -- plays func_8009CE1C once the countdown underflows or
   D_80082E94's 0x8000 bit is up, and either advances or retires. State 4
   retires. The common tail clears +0x1A.

   WHERE IT IS. Words 0..25, 34..82, 100..356 and 426..455 are EXACT. The
   residue is three sites and one of them costs the single extra word:

   SITE A, case 0, words 26..33: the same eight instructions in a different
   order. The target materialises both symbol addresses FIRST
   (`lui $a2, %hi(D_800DE5DC)' then `lui $v1, %hi(D_800814A8)') and fills the
   load delays with the +0xA bump; this build does the +0xA bump first and
   the symbols last. Both schedules are nop-free, so this costs 8 rows and
   NO words.

   SITE B, case 1, word 54: THE ONE EXTRA WORD. The target fills the load
   delay after `lbu $v0, 0x8($s2)' with `addiu $a0, $zero, 0x4' -- the
   func_80053EF0 argument -- and this build emits a nop there and materialises
   the 4 at word 61 instead. The chain is: sched1 places the argument setup
   late here, so local-alloc is free to give the `high(D_800814A8)' pseudo
   $a0 (the target has it in $a1, because there the argument setup is already
   above the pseudo's last use), and sched2 then cannot hoist `$a0 = 4' past
   it. Words 45..53 and 55..63 are otherwise exact, including the nop the
   target DOES have at word 50.

   SITE C, case 2, words 83..99: the rand-modulo and D_80083780 region, same
   instructions, different schedule and a temp-register rotation. The target
   computes `sra $v0, $a1, 12' / `sll $v0, $v0, 12' / `subu $s7, $a0, $v0'
   with $v0 as the one temp; this build reuses `ang' itself ($s7) as the
   first temp and $v1 as the second, and the rand value and its biased copy
   sit in the opposite registers. 16 rows.

   SITE D, case 3, words 357..425 and 456..465: the renderable pointer `m'
   gets $a0 where the target has $a2, and the two colour constants are one
   register low. 34 rows. THIS ONE IS A KNOCK-ON OF SITE B, PROVEN: any
   variant that builds 506 words makes every one of these rows disappear
   (measured on two separate 506-word variants). It is allocno numbering, not
   a spelling -- so SITE B is worth 35 rows, not one.

   MEASURED AND REJECTED, all against this body, aligndiff rows:

     starting point, obvious spelling of everything        513 w, 101

     THE TABLE LOCALS ARE LOOP-HOISTED INVARIANTS, NOT SOURCE LOCALS.
     This was the single biggest step and it is the generalisable one.
       `((s16 *)D_8006CCD8)[i]' written inline in the loop   507 w,  73
       ... through `s16 *tx' / `s16 *ty' locals assigned in
           the preheader                                    510 w,  96
     The two `addiu $sN, $v1, %lo(D_8006CCxx)' in the preheader are loop.c
     hoisting the invariant itself; writing them as C locals adds a ninth
     callee-saved pseudo (a SECOND `high(D_800814A8)' quantity appears,
     copied with `move $s3, $s8'), which costs the $s8 save/restore pair and
     four scheduling nops.

     `* 0x40' / `* 0x20' on the table element, NOT `<< 6' / `<< 5'
                                                            507 w,  69
       ... `<< 6' / `<< 5'                                  507 w,  73
     The shift lets combine narrow the element load from `lh' to `lhu'
     (force_to_mode sees only the low bits are needed); the multiply does
     not. Four rows, two per table, and `(s32)' casts, a byte-offset
     `*(s16 *)(tbl + ((x >> 8) & 0xE))' spelling and declaring the symbols
     `extern s16 ...[]' are ALL equal to the shift form -- the operator is
     the only thing that moves it.

     `b->unk_00 = ((s32 *)D_80083780)[0]' array refs          507 w,  65
       ... `*(s32 *)(D_80083780 + 0x0)'                       507 w,  67
       ... through a `u8 *' pointer local                     513 w,  83

     case 1's `h' pointer local for the D_800814A8 block IS load-bearing:
       `h' for the +0xA6/+0xA8 pair and `h2' for the +0x2A read
                                                             507 w,  65  kept
       ... one `h' for all three                              509 w,  69
       ... no local at all, D_800814A8 written out each time  510 w,  72
       ... `h' defined BEFORE the gate                        505 w,  69
           (it then fills the two nops the target HAS at words 40 and 42,
            so the local belongs after the gate -- this is a real fact
            about where the statement sits, not just a score)

     case 1's gate is `!= 2' with the 0x300 arm FIRST
                                                             507 w,  65  kept
       ... `== 2' with the arms swapped                       507 w,  70
       ... `func_800A56E0(cond ? 0x300 : 0x4300)'             504 w,  75

     case 2's loop is a `for (k = 0; k < 8; k++)' with the hit test and the
     back-off inside the body
                                                             507 w,  65  kept
       ... `k++' written BEFORE the test inside a
           `do { } while (k < 8)'                            506 w, 104-ish
           (measured 512 w / 104 on the pre-fix base; it puts the `slti' in
            the branch delay slot where the target has the increment, so the
            loop-continue block loses its own test)
     `for', `while (k < 8) { ... k++; }' and
     `do { ... k++; } while (k < 8)' are IDENTICAL (507 w, 65) -- gcc
     normalises all three.

     case 3's `m' / `w' pointer-local structure is NOT MEASURABLE. All of
     these are 507 w / 65 or 67: `w' as a local vs `*(u8 **)(p + 0x14)'
     written out, a separate `w2'/`m2' for the teardown, swapping the
     declaration order of `w' and `m', declaring them first or last in the
     block, `m[0xC]' array refs vs `*(u8 *)(m + 0xC)', and a second `m2' in
     the second arm. The one variant that reaches 506 words,
     `m = *(u8 **)(w - 0x14)' without the re-read of p + 0x14, is
     STRUCTURALLY WRONG -- the listing re-reads `lw $v1, 0x14($s2)' before
     `lw $a2, -0x14($v1)' -- and is 71 rows.

     EXHAUSTIVE ORDER SWEEPS, all run and all flat or worse:
       case 0's three statements, 6 orders x {global, pointer local}:
         A-B-C and B-A-C both 507 w / 65-69, C first 507 w / 79.
       case 1's five statements, all 60 orders with B before C:
         ABCDE is the ONLY one that is both 507 w and 65 rows; the twelve
         orders that reach 506 words are 69..75 rows and all of them break
         case 1 internally.
       case 2's six head statements, all 180 orders with B before C and D
         before F: ABCDEF..AEBCDF are 507 w / 67 (65 after the array-ref
         change); every order with B before A is 505 w / 71+ and introduces
         an `addiu $s0, $v1, %lo(D_80083780)' the target does not have.
       the four preheader statements, 12 orders: completely flat at 510/96
         (before the inline-table change), which is what said the table
         pointers were not locals at all.

     THE LABEL-PLUS-GOTO LOOP IS WRONG HERE, and the reason is diagnostic:
       `for (k = 0; k < 8; k++)'                              507 w,  65  kept
       ... `k = 0; again: { ... goto again; } out:;'           505 w, 115
       ... the same with `if (++k < 8)'                        505 w, 115
     The new lever says to write a loop as label-plus-goto when the target
     has NO invariant hoisting. This target HAS it -- the two
     `addiu $sN, $v1, %lo(D_8006CCxx)' in the preheader at words 150..158
     ARE loop.c hoists -- so killing the loop note costs exactly those two
     words and rotates the whole loop body. The precondition for that lever
     is absent here; read the preheader before reaching for it.

     UNMEASURED, and these are where I would look next:
       -- anything that changes sched1's priority for the func_80053EF0
          argument setup in case 1 (SITE B). I tried moving the constant
          into a local assigned early (three positions), a `n = f(4)'
          temporary, an early-break framing, `u16' vs `s16' widths on the
          +0x18 store, `-=' vs `= x - 1' on the +0xA6 bump, array-ref forms
          on all four D_800814A8 accesses, and `u16 *' types for `h'/`h2':
          every one is exactly 507 w / 65. I did NOT read the .sched dump,
          and that is the one thing that would settle it -- the question is
          purely why `set $a0 = 4' is scheduled last here and seventh from
          last in the target.
       -- a device. Nothing in this body was tried with a
          `do { } while (0)', an `if (1)' or a duplicated arm.
       -- the permuter. Never run on this function.

   DECLARATIONS
   UnkNode *func_8003FD64(s32, void *);
   extern u8 D_80083498[];
   void func_s53_800240F0(u8 *p, UnkVec *b, u8 *c, u8 *d);
   s32 func_8004491C(UnkNode *, u32);
   extern u8 D_80045340[];
   u8 *func_8003DE58(u8 *, UnkSlot *, s16 *, s16);
   extern u32 D_800814A8;
   extern u8 D_80082E80[];
   extern s16 D_80083228;
   extern u8 *D_800E3D18;
   s32 func_800644B8(s32);
   extern u32 D_800814A0;
   UnkNode *func_8003FC64(s32);
   void func_s53_80024918(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_s53_80024700(u8 *p);
   void func_s53_8002455C(u8 *p, UnkVec *b, u8 *c, u8 *d);
   extern u8 D_800DE938[];
   void func_8003DB94(void *, void *, s32);
   s32 func_80064584(s32);
   void func_800478B8(u8 *);
   extern u8 D_800DE5DC[];
   s32 func_80053EF0(s32);
   void func_800A56E0(s32);
   s32 func_800A05A4(u8 *, s32, s32, s16, s32);
   extern u8 D_8006CCD8[];
   extern u8 D_8006CCE8[];
   s16 func_800BCAD0(u8 *);
   s16 func_800A45D8(u16, u16, s32);
   extern u16 D_80082E94;
   void func_8009CE1C(u8 *, s16, s16, s16, s16, u32, s16);
   void func_s53_80024B54(u8 *p, UnkVec *b, u8 *c, u8 *d);
   void func_s53_8002401C(s16 x0, s16 y0, s16 z0, s16 x1, s16 y1, s16 z1, s16 *out);
   void func_s53_80024494(u8 *p, u8 *q, u32 r);
   void func_s53_800249E4(u8 *a, UnkVec *b, UnkVec *c, s16 d, MATRIX m, SVECTOR r);
   END DECLARATIONS
   D_80083460 is `extern Unk80083462 D_80083460;' in include/structs.h and is
   reached as `(u8 *)&D_80083460' the way csrc/ovl_dng_b08.c spells it;
   D_80083780 and D_800814A8 are already in include/structs.h too. */
void func_s53_80024B54(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkVec loc;
    SVECTOR rot;
    MATRIX mtx;
    s16 out[4];
    UnkVec *q;
    u8 *r;
    u8 *w;
    u8 *m;
    u8 *g;
    u8 *h;
    u8 *h2;
    u8 *g2;
    s32 ang;
    s32 n;
    s32 k;
    s32 i;

    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        *(s16 *)(p + 0x10) = 0;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s32 *)(D_800814A8 + 0xF4) = 0;
        func_s53_80024494(*(u8 **)p - 0x20, *(u8 **)(p + 0x4), (u32)D_800DE5DC);
        /* fallthrough */
    case 1:
        if ((**(u16 **)(p + 0x4) & 0x80) == 0)
        {
            break;
        }
        h = (u8 *)D_800814A8;
        *(s16 *)(p + 0x18) = 0xA;
        *(s16 *)(h + 0xA6) = *(u16 *)(h + 0xA6) - 1;
        h[0xA8] = p[0x8];
        h2 = (u8 *)D_800814A8;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        *(s16 *)(p + 0xE) = *(u16 *)(h2 + 0x2A);
        if (func_80053EF0(4) != 2)
        {
            func_800A56E0(0x300);
        }
        else
        {
            func_800A56E0(0x4300);
        }
        break;
    case 2:
        *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - 1;
        if (*(s16 *)(p + 0x18) > 0)
        {
            break;
        }
        ang = rand() % 0x1000;
        b->unk_00 = ((s32 *)D_80083780)[0];
        b->unk_04 = ((s32 *)D_80083780)[1];
        b->unk_08 = (*(s16 *)(D_800814A8 + 0x88) - 0x50) << 16;
        r = D_80082E80;
        i = ((D_80083228 + *(s16 *)(D_800814A8 + 0x2A) + 0x100) >> 9) & 7;
        if (func_8003DE58(((u8 **)D_800E3D18)[i], (UnkSlot *)r, out, 0) != 0)
        {
            b->unk_00 += out[0] << 16;
            b->unk_04 += out[1] << 16;
            b->unk_08 += out[2] << 16;
        }
        n = func_800A05A4((u8 *)D_800814A8, r[0x24], r[0x25], *(s16 *)(D_800814A8 + 0x2A), 8);
        *(s32 *)(p + 0x14) = n;
        if (n != 0)
        {
            q = *(UnkVec **)((u8 *)n - 0x18);
        }
        else
        {
            q = &loc;
            *(s16 *)((u8 *)q + 0x2) = (r[0x24] * 0x40) + 0x20;
            *(s16 *)((u8 *)q + 0x6) = (r[0x25] * 0x40) + 0x20;
            for (k = 0; k < 8; k++)
            {
                *(s16 *)((u8 *)q + 0x2) =
                    *(u16 *)((u8 *)q + 0x2) + (((s16 *)D_8006CCD8)[(*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7] * 0x40);
                *(s16 *)((u8 *)q + 0x6) =
                    *(u16 *)((u8 *)q + 0x6) + (((s16 *)D_8006CCE8)[(*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7] * 0x40);
                *(s16 *)((u8 *)q + 0xA) = *(u16 *)(D_800814A8 + 0x88);
                *(s16 *)((u8 *)q + 0xA) = func_800BCAD0((u8 *)q);
                if (*(s16 *)((u8 *)q + 0xA) > 0x200)
                {
                    *(s16 *)((u8 *)q + 0xA) = *(u16 *)(D_800814A8 + 0x88);
                }
                if (func_800A45D8(*(u16 *)((u8 *)q + 0x2), *(u16 *)((u8 *)q + 0x6),
                                  *(s16 *)((u8 *)q + 0xA)) != 0)
                {
                    *(s16 *)((u8 *)q + 0x2) =
                        *(u16 *)((u8 *)q + 0x2) - (((s16 *)D_8006CCD8)[(*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7] * 0x20);
                    *(s16 *)((u8 *)q + 0x6) =
                        *(u16 *)((u8 *)q + 0x6) - (((s16 *)D_8006CCE8)[(*(u16 *)(D_800814A8 + 0x2A) >> 9) & 7] * 0x20);
                    *(s16 *)((u8 *)q + 0xA) = *(u16 *)(D_800814A8 + 0x88);
                    *(s16 *)((u8 *)q + 0xA) = func_800BCAD0((u8 *)q);
                    if (*(s16 *)((u8 *)q + 0xA) > 0x200)
                    {
                        *(s16 *)((u8 *)q + 0xA) = *(u16 *)(D_800814A8 + 0x88);
                    }
                    break;
                }
            }
        }
        func_s53_8002401C(*(s16 *)((u8 *)b + 0x2), *(s16 *)((u8 *)b + 0x6), *(s16 *)((u8 *)b + 0xA),
                          *(s16 *)((u8 *)q + 0x2), *(s16 *)((u8 *)q + 0x6),
                          *(s16 *)((u8 *)q + 0xA), (s16 *)&rot);
        RotMatrix(&rot, &mtx);
        func_s53_800249E4(p, b, q, ang, mtx, rot);
        func_s53_800249E4(p, b, q, ang + 0x555, mtx, rot);
        func_s53_800249E4(p, b, q, ang + 0xAAA, mtx, rot);
        *(s16 *)(p + 0x18) = 0x18;
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
        break;
    case 3:
        if (*(u8 **)(p + 0x14) != NULL)
        {
            *(u32 *)(*(u8 **)(p + 0x14) + 0x1C) |= 0x10000000;
            m = *(u8 **)(*(u8 **)(p + 0x14) - 0x14);
            if (*(s16 *)(p + 0x18) >= 0xC)
            {
                m[0xC] += 8;
                m[0xD] -= 8;
                m[0xE] -= 8;
                if (m[0xC] > 0xE0)
                {
                    m[0xC] = 0xE0;
                }
                if (m[0xD] < 0x20)
                {
                    m[0xD] = 0x20;
                }
                if (m[0xE] < 0x20)
                {
                    m[0xE] = 0x20;
                }
            }
            else
            {
                m[0xC] -= 8;
                m[0xD] += 8;
                m[0xE] += 8;
                if (m[0xC] < 0x80)
                {
                    m[0xC] = 0x80;
                }
                if (m[0xD] > 0x80)
                {
                    m[0xD] = 0x80;
                }
                if (m[0xE] > 0x80)
                {
                    m[0xE] = 0x80;
                }
            }
        }
        if ((D_80082E94 & 0x8000) == 0)
        {
            *(s16 *)(p + 0x18) = *(u16 *)(p + 0x18) - 1;
            if (*(s16 *)(p + 0x18) >= 0)
            {
                break;
            }
        }
        if (*(s32 *)(p + 0x14) != 0)
        {
            func_8009CE1C(*(u8 **)(p + 0x14), 0xA, p[0x9], 9,
                          (*(u16 *)(p + 0xE) << 9) + 0x800, (u32) * (u8 **)p, 1);
            w = *(u8 **)(p + 0x14);
            *(u32 *)(w + 0x1C) &= 0xEFFFFFFF;
            *(u32 *)(*(u8 **)(w - 0x14) + 0xC) = 0x808080;
        }
        if (*(s16 *)(p + 0x1A) != 0)
        {
            *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
            break;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u32 *)(g + 0xC) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    case 4:
        if (*(s16 *)(p + 0x1A) != 0)
        {
            break;
        }
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0xA) -= 1;
        *(u32 *)(g2 + 0xC) = 0;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    *(s16 *)(p + 0x1A) = 0;
}
