/* PARK -- func_800A1D4C, the dungeon "level up" twin of func_800A2424.
   438 of 438 words, **4 ALIGNED DIFFERENCES** (7 differing by ovlverify's
   masked word compare).  Was 45 differing / 43 aligned rows when this park
   was handed to j88.  The residue is now ONE statement's placement, diagnosed
   down to the arithmetic -- a good permuter target.

   FIVE LEVERS GOT IT HERE.  All five are in the body below; do not undo them.

   1. THE SPELL LOOP'S RULE INDEX IS A FRESH LOCAL `k' THAT IS *ALSO* READ IN
      THE MESSAGE LOOP: `k = p[i * 3 + 8];' in the spell loop AND
      `k = p[i * 3 + 8]; h = func_80099194(d[k].unk_00, h);' in the message
      loop, in place of the inlined subscript.  Lifted verbatim from the
      landed twin func_800A2424.  **The park's old note "loop index local `k'
      of its own rather than `c' -- the same +7-differing step" measured the
      SPELL LOOP ONLY; the message-loop read is the half that pays.**  Fixes
      the whole spell-loop rotation (induction ptr $a2->$a1, k $a1->$a0,
      &e[i] $a0->$a2).  13 rows.

   2. `i = 0;' BEFORE `d = (UnkDungeonName0 *)D_8006DE24;', with the message
      loop written `for (; i < 3; i++)'.  Also from the twin.  The two insns
      are independent, so sched1 orders them by INSN_LUID and nothing
      downstream recovers the swap.  Words 375/376, 2 rows.

   3. `hi -= lo;' AS ITS OWN STATEMENT for STAT 5 ONLY:
          hi -= lo;
          o = n = p[5];
          n += hi;
      in place of `o = n = p[5]; n += hi - lo;'.  **15 rows, and it is the
      whole of the stat-5 residue this park was stuck on.**  It fixes both
      halves at once: the `lbu $v1, 0x5($s2)' stops being scheduled two words
      early (69 instead of 71), AND the $a0/$v1 rotation in the `hi' block
      (the t[5] reload, the two `mflo's and the partial sum) comes out the
      target's way.  The other five stat blocks keep `n += hi - lo;'.
      Note the direction: `hi -= lo' (the level-DOWN twin has `lo - hi'), and
      `n += hi' after.

   4. `r = 0x63;' SPLIT OUT AS ITS OWN SET, then
      `r = (hi - p[i*3+0xA]) / u2 + p[i*3+0xA];'.  Lever "DENY loop.c A
      CONSTANT HOIST BY PUTTING THE CONSTANT IN A *USER* VARIABLE": the
      holder is then a user variable, set three times, live across basic
      blocks, so all three scan_loop arms fail, no movable is made, and the
      target's `addiu $v0, $zero, 0x63' is rematerialised at the use (word
      300) instead of hoisted to `li $t1, 0x63' in the preheader.  9 rows
      (it also unshifts $t1/$t2/$t3 and un-nops word 261).

   5. THE `0x63' IS HELD IN `hi', NOT IN `r'.  WHICH user variable holds it
      decides whether the constant and the subtraction share a register.  The
      target has `addiu $v0, $zero, 0x63' + `subu $v0, $v0, $a0'; with the
      constant in `r' it is `li $v1, 99' + `subu $v0, $v1, $a0'.  2 rows.
      Measured holders (aligned rows): hi 4, u 6, w 6, r 6, hp 7, n 7, c 17,
      k 23, o 436 words/70, lo 440 words/104.

   THE RESIDUE -- 4 rows, one statement's placement
   ------------------------------------------------
   Ours computes `li v0,100' + `subu a3,v0,s1' at words 267/268, at the TOP of
   the `if (p[0x13] != 0)' arm.  The target computes `addiu $v0, $zero, 0x64'
   + `subu $a3, $v0, $s1' at words 272/273, at the END of the loop preheader:

     267 addiu $s0, $zero, 0x2        i = 2            (arm)
     268 addiu $t0, $sp, 0x10         &e[0]            (loop.c hoist)
     269 lui   $v0, %hi(D_8006DE24)                    (loop.c hoist)
     270 addiu $t2, $v0, %lo(...)                      (loop.c hoist)
     271 addiu $t1, $zero, 0x1        the 1 of e[i]=1  (loop.c hoist)
     272 addiu $v0, $zero, 0x64   <-- u2, LAST hoist
     273 subu  $a3, $v0, $s1      <--
     274 addiu $a1, $s2, 0x6          biv init         (strength_reduce)

   THIS POSITION CAN ONLY BE A loop.c HOIST, and that is proved, not guessed:
   sched1 leaves this block in INSN_LUID order (read out of the .sched dump --
   all eight insns have priority 1, the ready list is sorted by descending
   insn number and the backward scheduler emits them in chain order), and
   `move_movables' does `emit_insn_before (..., loop_start)', so a hoist's
   position in the preheader is its SOURCE position among the movables.  For
   the u2 pair to land after the `1' hoist, `u2 = 0x64 - lvl;' must sit in the
   loop body AFTER `e[i] = 1;'.  Arm insns are always emitted before every
   hoist, so no placement inside the `if' arm can reach word 272.

   AND THAT HOIST FLIPS THE p/lvl ALLOCATION, BY 0.2%.  global.c's
   `allocno_compare' priority is floor_log2(n_refs) * n_refs / live_length:

     u2 in the `if' arm   lvl: 16 refs / 197 insns = 0.3249  > p 0.3188  OK
     u2 hoisted by loop.c lvl: 16 refs / 202 insns = 0.3168  < p 0.3188  FLIP
                          p:   38 refs / 596 insns = 0.3188

   lvl must be allocated before p to get lvl=$s1 / p=$s2 as the target has;
   the hoist adds exactly the 5 preheader insns to lvl's live range and loses
   the race.  Any ONE of these would win it:
       lvl live_length <= 200   (4*16/200 = 0.3200)
       lvl n_refs      >= 17    (4*17/202 = 0.3366)
       p   n_refs      <= 37    (5*37/596 = 0.3104)
       p   live_length >= 601   (5*38/601 = 0.3161)
   `flow_analysis' runs ONCE (toplev.c:3463) and BEFORE combine (:3488), so a
   reference that combine later deletes would be free -- that is the shape of
   the fix, and j88 could not find a spelling with one.

   PROOF THAT THIS IS THE WHOLE STORY: with the hoist in place AND the spell
   loop gated on `if (c != 0)' instead of `if (p[0x13] != 0)' -- which takes
   one reference off p -- the preheader comes out EXACTLY right, words 267-274
   all match, and the only residue left is `c' becoming callee-saved ($s6,
   pushing f to $s7 and the frame to 0x40): 438 words, 16 aligned rows.  So
   the diagnosis above is confirmed by measurement, and all that is missing is
   a way to spend that one reference without a long-lived local.

   Spellings measured and rejected
   -------------------------------
   `u2 = 0x64 - lvl;' POSITION (with levers 1-5 in place; aligned rows):
     first statement inside the `if (p[0x13] != 0)' arm       4   <-- kept
     before the `if', after `*(u16 *)(p + 6) = hp;'     437 words/25
     `u2 = 0x64; u2 -= lvl;' split, in the arm                4  (identical)
     `i = 2;' hoisted before or after `u2' in the arm         4  (identical)
     top of the loop body (hoisted, but to the WRONG slot --
        before the D_8006DE24 base -- and the `1' hoist is
        then LOST into the loop)                            14
     after `e[i] = 0;' (same, hoisted too early)             14
     after `e[i] = 1;' (hoisted to the RIGHT slot)     437 words/62, p/lvl flip
     before `e[i] = 1;'                                437 words/86, p/lvl flip
     after `u = D_8006DE24[k]...;'                     439 words/94
     `(0x64 - lvl)' inline, no u2 local                437 words/62, p/lvl flip
     `u2 = -lvl; u2 += 0x64;' in the loop              437 words/63, flip
   Tried against the p/lvl margin, all on the hoisted base, all 62-65 rows:
     `p[0x11] = 1 + lvl;'; `if (p[0x11] <= 0x62)'; four regroupings of the
     `hp' expression (`w * lvl', `lvl * t[6]', nested `lvl * (lvl * ...)');
     `lvl * t[5] / 16' in the hi line; `(t[5] * (lvl - 1)) << 11' in the lo
     argument; `lvl * i' in the five stat products; moving `lvl = p[0x11];'
     after `c', after `t', after `q' (lreg confirms lvl's live length does NOT
     move: 16/202 in all four); and all eight 0x63 holders again.
   stat-5 `hi' statement (measured at the 21-row intermediate):
     terms reversed (`.../32768 + t[5]*lvl/16')               20
     `(t[5]*lvl)/16 + (...)/32768' explicit parens            21 (identical)
     `lvl * t[5] / 16'                                        21 (identical)
     `hi = q[5] + t[5]*lvl/16;' then `hi += .../32768;'      151
     the call hoisted into `u' first                          77
     `q[5]' moved to the end of the sum                       85
   stat-5 `n' statement:
     `n = p[5]; o = n; n += hi - lo;'                         21
     `u = hi - lo; o = n = p[5]; n += u;'                     89
   `r' statement shape (with `r' as the 0x63 holder):
     `r = 0x63; r -= x; r = r / u2 + x;'                      22
     `r = 0x63; r = (r - x) / u2; r += x;'                    27

   Older measurements, from before j88 (ovlverify words/differing):
     `(D_80013714 & 1) == 0` for the FIRST of the two tests   437/236
        -- the declared symbol lets reorg lift its `lui` into the
        `beqz $v0, .L800A23F8` delay slot where the target has a `nop`.
        `(*(u16 *)0x80013714 & 1) == 0` is the whole word. The SECOND test
        measures the same either way.
     `o = p[x]; n = o + (hi - lo);`                            437/335
        -- no `addu $a0, $v1, $zero` copy.
     stat loads left after the `hi` line (stats 0..3)          439/337
        -- the target reads p[0..3] BEFORE the `lo` division. Moving all six
        early is 443/424, moving only 0/1 is 439/344.
     product in its own `r` rather than in `i`                 438/117
        -- sharing the message loop's counter is what puts the product in
        the callee-saved $s0.
     division result in `n`/`hp` 46, in `hi` 118, in `lo` 440/433.
     `o = p[5]; n = p[5] + (hi - lo);` for stat 5 only         437/355.

   THE STAT-5 ALLOCATOR ARITHMETIC, in case it is wanted again: before lever 3
   the hi block's pseudos were 138 (the t[5] reload, 3 refs / 7 insns), 141
   (the first `mflo', 5/7), 146 (the second `mflo', 5/7), 137 (q[5], 2/6) and
   142 (the partial sum, 2/6).  141 therefore sorted before 138 (1.43 against
   0.43) and took $v1, forcing 138 to $a0 -- the whole 14-row rotation.
   `hi -= lo;' changes the block's LUIDs and the order comes out right.

   DECLARATIONS
   s32 func_800647A0(s32);
   extern u8 D_800E0953[];
   extern u8 D_800E095F[];
   extern u8 D_800E0970[];
   extern u8 D_800E0979[];
   extern u8 D_800E098B[];
   extern u8 D_800E099A[];
   extern u8 D_800E09B2[];
   extern u8 D_800E09BB[];
   void func_800A1D4C(u8 *, s16);
   END DECLARATIONS
   PROTOTYPE ALREADY DONE: `s32 func_800A2424(u8 *, s16);' is in the unit as
   of 2026-11-16, landed with its twin func_800A2424. Nothing to change.
*/
void func_800A1D4C(u8 *p, s16 f)
{
    u8 *t;
    u8 *q;
    u8 *g;
    void *m;
    void *h;
    UnkDungeonName0 *d;
    s32 lvl;
    s32 c;
    s32 k;
    s32 i;
    s32 r;
    s32 w;
    s32 u;
    s32 u2;
    s32 lo;
    s32 hi;
    u32 n;
    u32 o;
    u32 hp;
    u8 e[3];

    if (p[0x11] < 0x63)
    {
        lvl = p[0x11];
        c = p[0x13];
        p[0x11] = lvl + 1;
        t = D_800DDCBC[c];
        q = (u8 *)&D_8006D168[c];
        lo = q[5] + t[5] * (lvl - 1) / 16 + t[5] * func_800647A0(((lvl - 1) * t[5]) << 11) / 32768;
        hi = q[5] + t[5] * lvl / 16 + t[5] * func_800647A0((lvl * t[5]) << 11) / 32768;
        hi -= lo;
        o = n = p[5];
        n += hi;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[5] = n;
        o = n - o;
        p[0x28] += o;

        i = t[4] * q[4];
        lo = q[4] + i * (lvl - 1) / 1024;
        hi = q[4] + i * lvl / 1024;
        o = n = p[4];
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[4] = n;
        o = n - o;
        p[0x25] += o;

        i = t[0] * q[0];
        o = n = p[0];
        lo = q[0] + i * (lvl - 1) / 64;
        hi = q[0] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[0] = n;
        o = n - o;
        p[0x26] += o;

        i = t[1] * q[1];
        o = n = p[1];
        lo = q[1] + i * (lvl - 1) / 64;
        hi = q[1] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[1] = n;
        o = n - o;
        p[0x27] += o;

        i = t[2] * q[2];
        n = p[2];
        lo = q[2] + i * (lvl - 1) / 64;
        hi = q[2] + i * lvl / 64;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[2] = n;

        i = t[3] * q[3];
        n = p[3];
        lo = q[3] + i * (lvl - 1) / 1024;
        hi = q[3] + i * lvl / 1024;
        n += hi - lo;
        if (n > 0xFF)
        {
            n = 0xFF;
        }
        p[3] = n;

        w = *(s16 *)(q + 6);
        hp = w + lvl * w + lvl * lvl * (w + t[6] * lvl) / 512;
        if (hp > 0xFFFF)
        {
            hp = 0xFFFF;
        }
        *(u16 *)(p + 6) = hp;
        if (p[0x13] != 0)
        {
            u2 = 0x64 - lvl;
            for (i = 2; i >= 0; i--)
            {
                e[i] = 0;
                k = p[i * 3 + 8];
                if (k != 0)
                {
                    u = D_8006DE24[k].unk_10 >> 4;
                    if ((u & *(u32 *)(p + 0x14)) != 0 || (u & 7) == 0)
                    {
                        if (p[i * 3 + 0xA] < 0x63)
                        {
                            p[i * 3 + 0xA] += 1;
                            e[i] = 1;
                            hi = 0x63;
                            r = (hi - p[i * 3 + 0xA]) / u2 + p[i * 3 + 0xA];
                            if (r >= 0x64)
                            {
                                r = 0x63;
                            }
                            p[i * 3 + 0xA] = r;
                        }
                    }
                }
            }
        }
        func_80041E70(p);
        if (f != 0)
        {
            if ((*(u16 *)0x80013714 & 1) == 0)
            {
                D_800DCF4F = 1;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
            }
            func_800B4C7C((*(u32 *)(p + 0x14) & 0x2000) ? 0x8002 : 0x8003, p, -2, 1);
            m = func_800990FC();
            h = func_80099194(D_800E0953, m);
            h = (void *)func_8009929C(0xA, h);
            h = func_80099734(p, h);
            h = func_80099194(D_800E095F, h);
            h = func_8003AD08(p[0x11], h);
            h = func_80099194(D_80089000, h);
            if (p[0x13] != 0 && (*(u32 *)(p + 0x14) & 0x4000) != 0)
            {
                i = 0;
                d = (UnkDungeonName0 *)D_8006DE24;
                for (; i < 3; i++)
                {
                    if (e[i] != 0)
                    {
                        h = (void *)func_8009929C(0xA, h);
                        k = p[i * 3 + 8];
                        h = func_80099194(d[k].unk_00, h);
                        h = func_80099194(D_800E0970, h);
                        h = func_80099194(D_800E0979, h);
                    }
                }
            }
            if ((D_80013714 & 1) == 0)
            {
                h = (void *)func_8009929C(0x11, h);
                h = (void *)func_8009929C(0x4C, h);
                h = (void *)func_80099254(D_800E0458, h);
            }
            func_80099290(h);
            func_800A5720(m);
        }
    }
}
