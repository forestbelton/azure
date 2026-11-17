/* PARK: func_8009A540, 75 of 75 words, 6 differing (masked) / 5 aligned rows.
   WAS 75/23 when this park was opened.  ovl_dungeon.  Measured 2026-11-16 by
   agent j78; `parksplice.py` then `ovlverify.py` reproduce the number exactly.
   clang-format leaves the body untouched.

   NO NEW DECLARATIONS.  `D_800DCEAC`/`D_800DCEBC` are already
   `extern u16 ...[]`, `func_800BCB04h` is already the unit's halfword alias,
   and the prototype `s16 func_8009A540(s32, s32, s32, s16);` is pinned by
   eight landed call sites -- do not change it.

   ------------------------------------------------------------- THE RESIDUE

   SIX WORDS, TWO INDEPENDENT CAUSES, both pure permutation/register:

   (A) words 3,4,5 -- the entry block's last three insns are permuted.

          candidate                    target
        2 addu  $s2, $a0, $zero     addu  $s2, $a0, $zero   (same)
        3 andi  $a0, $a0, 0x1       sw    $s6, 0x30($sp)
        4 sw    $s6, 0x30($sp)      addu  $s6, $a3, $zero
        5 addu  $s6, $a3, $zero     andi  $a0, $a0, 0x1

       Every opcode and operand is right, including the `andi $a0, $a0, 0x1`
       that reads AND writes the incoming argument register; only the `andi`
       sits two words early.

   (B) words 54,59,60 -- the post-loop `i < 2` test is in $v0, the target has
       it in $v1:  `slti $v0,$s0,2` / `slti $v0,$s0,2` / `bnez $v0` against
       `slti $v1,$s0,2` / `slti $v1,$s0,2` / `bnez $v1`.  The loop-back test
       at words 56-57 is `$v0` in both.  The two `slti`s are reorg's
       duplication of one join-block insn into the break branch's delay slot;
       it is ONE block-local pseudo and local-alloc gives it the first free
       register, i.e. $v0.  The target's conflicts must exclude $v0 there and
       `.greg` does not say why: in this body the pseudo is block-local with
       `Registers live at start: 29 30 94`, nothing in $v0.

   (A) and (B) are not coupled -- (A) was closed to 3 words by the levers
   below while (B) never moved, and one rejected spelling (`y5`, see below)
   closes (B) and reopens (A) at 5 words instead of 3.

   ----------------------------------------------------- WHAT PAID, 23 -> 6

   Four levers, measured in this order.  Each is independently verifiable by
   reverting just that one.

   (1) `b = a;` WITH THE LOOP READING `b` AND ONLY THE TEST READING `a`.
       23 -> 19 on its own.  Eleven rows of the old residue were a straight
       $s2/$s3 swap (`a` and `xx` the wrong way round, in eleven places
       including both stack slots); the copy renumbers the allocnos and fixes
       all of them, and it is also what produces the target's
       `andi $a0, $a0, 0x1`: `a`'s pseudo then dies at the test, local-alloc
       ties it to the incoming $a0 through the parameter copy, that copy
       becomes `addu $a0,$a0,$zero` and is deleted, and the AND's destination
       is tied to the same dying $a0.

   (2) `a &= 1;` AS A SEPARATE STATEMENT, WITH THE TEST `if (a != 0)`.
       This is the lever that keeps the copy, and the mechanism is
       **`optimize_reg_copy_1`, local-alloc.c:704**, called from
       `update_equiv_regs` (local-alloc.c:1028) for EVERY reg-to-reg copy
       whose source is not dead in the copy.  From `b = a` it scans forward,
       finds `a`'s REG_DEAD on the `and`, and REPLACES `a` WITH `b` there --
       so the test reads $s2 and the copy is then tied away entirely: one
       pseudo, no `addu $s2,$a0,$zero`, 16 differing.  Its forward scan breaks
       on `CODE_LABEL`, `JUMP_INSN`, a `NOTE_INSN_LOOP_BEG/END`, a `USE` of
       the source, or `reg_set_p (src, p) || reg_set_p (dest, p)`.
       `a &= 1` IS that `reg_set_p (src, p)`, and it costs nothing because it
       is the `andi` the function needs anyway.
       (The same scan is why `local-alloc.c:716`'s SMALL_REGISTER_CLASSES
       guard being inactive matters here -- see the catalogue entry "an
       INCOMING ARGUMENT REGISTER can turn up far from the prologue".)

   (3) `b = a;` ONCE MORE AT THE END, as the last statement before
       `return 1`.  A DEAD STORE, deleted by flow long before allocation,
       worth 8 rows.  It is a cse-canonicality device, `make_regs_eqv`,
       cse.c:838-868: at `b = a` cse promotes whichever pseudo LIVES LONGER
       to canonical, and a canonical `b` makes every later mention of `a` --
       including the test -- read `b` instead.  The trailing store makes
       `regno_last_uid[b] == regno_last_uid[a]`, and the comparison is
       STRICTLY greater, so `a` stays canonical.  Without it: 13 differing.

   (4) A SECOND SET ON THE D_800DCEBC POINTER LOCAL, plus `s32 ax`/`s32 az`
       LOCALS FOR THE TWO SUMS with `u = D_800DCEBC;` BETWEEN THEM.
       Together 19 -> 6; the second set alone is 22 and the locals alone do
       nothing, because the locals only pay once the address is materialised
       inside the loop.

       THE SECOND SET IS WHY loop.c DOES NOT HOIST THE BASE.  A movable needs
       one of the three arms at loop.c:786-800.  Arm (2) is
       `! REG_USERVAR_P (SET_DEST)`, which is why the compiler temp that
       `D_800DCEBC[k]` generates is ALWAYS hoisted -- that is the 77-word
       `$fp` case this park's first header documents, and no respelling of
       the reference escapes it; the base has to be a USER VARIABLE.  Arm (3)
       fails because the set is inside the conditional (`maybe_never`).  Arm
       (1) is `reg_in_basic_block_p`, and a second set kills it two ways at
       once: that function's first line is
       `if (regno_first_uid[regno] != INSN_UID (insn)) return 0;`, so the
       LATER set is not the register's first mention, and the EARLIER one
       needs `n_times_set == 1` (or consecutive sets) to be a movable at all.
       The result is the target's own two words, `lui %hi(D_800DCEBC)` /
       `addiu $v0, $v0, %lo(D_800DCEBC)` at the point of use, where the old
       park body's raw constant `0x800DCEBC` gave `lui 0x800D` / `ori 0xCEBC`
       -- right count, right place, wrong two words.

       The dead `u = t;` costs nothing and its position does not matter:
       after the inner `if`, at the head of the loop body, `u = t;` in the
       preheader and `u = D_800DCEAC;` in the preheader all measure the same
       6.  **That is the one place this body is least likely to be the
       original source**, and it is where a permuter run should look: any
       spelling that gives `u` a mention before its in-loop set will do.

   ----------------------------------------------- MEASURED AND REJECTED

   All at 75 words unless stated.  Numbers are aligned rows.

   For (A), the entry-block order.  The order is decided in sched1 and then
   re-decided in sched2, and sched2's tie-break is `INSN_LUID`, i.e. sched1's
   output order (`rank_for_schedule`, sched.c).  In sched1 the ready list at
   T-2 is `12 (7f000001) 21 (1)`: the `z` parameter copy carries
   LAUNCH_PRIORITY because `adjust_priority` -> `birthing_insn_p` (sched.c)
   bumps any insn whose destination is live and has `reg_n_sets == 1`, and
   this body's `andi` writes `a`, whose `reg_n_sets` is 2.  **Give the `andi`
   a once-set destination and it ties with the `z` copy on priority, the
   LUID tie-break prefers it, and sched1 emits 19/12/21 -- the target order.**
   Everything tried for that re-opened (2):
     * `c = a & 1; if (c != 0)` with `b = a` before it -- 16 (the copy is
       tied away by optimize_reg_copy_1, as above).
     * the same plus a dead `a = 0;` after the AND -- 16.  The `.combine`
       dump of this one is CORRECT (`(set reg88 reg80)` survives and
       `(set reg89 (and reg80 1))` has reg89 once-set); it is local-alloc
       that undoes it, so do not re-measure it from the RTL dumps.
     * `c = a & 1;` BEFORE `b = a;`, so `a` dies at the copy and
       optimize_reg_copy_1 is not called at all (`y5`) -- 6, and a DIFFERENT
       6: it fixes residue (B) completely, and costs 5 words in the entry
       block plus `andi $v0,$a0,1` for the one word (A) costs here.  Worth
       a permuter seed of its own.
     * `do { } while (0);` between `b = a;` and the test -- 8.  The
       LOOP_BEG/LOOP_END notes DO block optimize_reg_copy_1 (the copy
       survives and `addu $s2,$a0,$zero` appears), but `sched_analyze` makes
       the insn carrying a loop note depend on everything before it, so the
       `andi` cannot be scheduled early and four other words move.
     * `while ((a & 1) != 0) { ... break; }` instead of the `if` -- 16.
     * `b = a;` before the `if` with no second set of `a` -- 25 with the old
       body, 13 with this one; also 13 for: `b = a;` after `i = -1;`, after
       `t = D_800DCEAC;`, just before the `for`, a second `b = a;` inside the
       `if`, and a trailing `c = a;` with a fresh `c` instead of `b = a;`.
     * `a` read at the k site and `b` at the func_8009A350 site -- 78 words
       (two long-lived copies of the same value is a ninth callee-saved
       quantity, which is the $fp/77-word trap again); the other way round is
       12.

   For (B), the $v0/$v1 of the post-loop test.  NOTHING MOVED IT.  All 6:
   `if (i != 3)`, `if (i <= 1)`, `for (; 2 > i; i += 2)`, the loop as an
   explicit `do/while` and as a `while`, `i = -1` lifted into the `for`,
   `if (i >= 2) return 1; return 0;`, the test written twice, `a = a & 1` for
   `a &= 1`, the trailing dead store moved into the `return 0` arm / to the
   end of the `if` / written `a = b;`, and three placements of the dead
   `u = t;`.  `u16 ax`/`u16 az` instead of `s32` + `(u16)` casts is 16, so
   that pair is load-bearing; only `az` as a local is also 6 and only `ax` is
   24.  `u16 wx`/`u16 wz` is 77 words.

   -------------------------------------------------------------- PERMUTER

   The run recorded in this park's first header (2026-11-16, 20 minutes,
   6 jobs, base 725 -> best 320 on the permuter's own metric, output measured
   at 73 words -- two SHORT -- with the only idea being the undefined
   `k * (k = 2)`) was against the OLD 23-row body with the raw constant
   address.  THIS body is a different and much narrower residue -- six words,
   three of them a pure three-insn permutation in the entry block and three a
   single register number -- and is worth a fresh run.  Seed it with the body
   below; `y5` (swap `b = a;` and the AND into `c`, drop the trailing
   `b = a;`) is a second, disjoint 6-word seed that has residue (B) already
   closed. */

s16 func_8009A540(s32 a, s32 x, s32 y, s16 z)
{
    u16 w;
    s32 ax;
    s32 az;
    s32 b;
    s32 xx;
    s32 yy;
    s32 wx;
    s32 wz;
    s32 i;
    s32 k;
    u16 *t;
    u16 *u;

    b = a;
    a &= 1;
    if (a != 0)
    {
        i = -1;
        xx = (s16)x;
        wx = (xx << 6) + 0x20;
        t = D_800DCEAC;
        yy = (s16)y;
        wz = (yy << 6) + 0x20;
        for (; i < 2; i += 2)
        {
            if (func_8009A350(xx, yy, (b + i) & 7, &w) != 0)
            {
                k = ((u16)b + i) & 7;
                ax = t[k] + wx;
                u = D_800DCEBC;
                az = u[k] + wz;
                if (func_800BCB04h((u16)ax, (u16)az, z) >= 0x201)
                {
                    break;
                }
                u = t;
            }
        }
        if (i < 2)
        {
            return 0;
        }
    }
    b = a;
    return 1;
}

#if 0
/* SECOND SEED, also 75 words / 6 differing, with a DISJOINT residue: the
   post-loop `i < 2` test comes out in $v1 as the target has it, and the cost
   is five words in the entry block plus `andi $v0,$a0,1` where the target has
   `andi $a0,$a0,1`.  Only the first three statements differ from the body
   above; the loop is identical. */
s16 func_8009A540(s32 a, s32 x, s32 y, s16 z)
{
    u16 w;
    s32 ax;
    s32 az;
    s32 b;
    s32 c;
    s32 xx;
    s32 yy;
    s32 wx;
    s32 wz;
    s32 i;
    s32 k;
    u16 *t;
    u16 *u;

    c = a & 1;
    b = a;
    if (c != 0)
    {
        i = -1;
        xx = (s16)x;
        wx = (xx << 6) + 0x20;
        t = D_800DCEAC;
        yy = (s16)y;
        wz = (yy << 6) + 0x20;
        for (; i < 2; i += 2)
        {
            if (func_8009A350(xx, yy, (b + i) & 7, &w) != 0)
            {
                k = ((u16)b + i) & 7;
                ax = t[k] + wx;
                u = D_800DCEBC;
                az = u[k] + wz;
                if (func_800BCB04h((u16)ax, (u16)az, z) >= 0x201)
                {
                    break;
                }
                u = t;
            }
        }
        if (i < 2)
        {
            return 0;
        }
    }
    return 1;
}
#endif
