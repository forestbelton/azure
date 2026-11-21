/* PARK -- func_800C5FA8, agent j59, 2026-11-16.

   ====================================== 2026-11-20: 239 rows at 701/706 words -> 175 at 706/706

   The body below carries one device: a duplicated-arm `if (u)` around `x++;` -- which is also the missing FIVE words.

   A `do { } while (0);` wrapper and a duplicated-arm `if (v) { s } else { s }`
   are the same free-reference lever from two directions -- flow.c:2128 weights
   each reference by `loop_depth`, jump2's cross-jumping merges identical arms
   -- so both cost nothing and both are ordinary C.  Found by sweeping every
   statement position times both devices over every local on a
   `tools/decomp/onefunc.py` file, where a candidate is 0.09 s.  Nothing in
   this park's own diagnosis points at the winning statement.
   TARGET 706 WORDS, CANDIDATE 706 WORDS, 49 ALIGNED DIFFERENCES.
   (ovlverify's masked word count is much larger and misleading here; the
   honest figure is aligndiff's 49.)

   STRUCTURE IS CORRECT.  Words 0-155, 173-208, 234-339 and 475-705 are
   byte-identical: the prologue, the whole D_80083460 gate chain, the three
   message arms, BOTH state-0 marking-loop nests, the whole state-1 half and
   the D_80013714 / D_80010234 tail all match exactly.

   ALL 49 ROWS ARE REGISTER ASSIGNMENT IN ONE REGION -- the random-room
   search at 800C62EC..800C6258 and the four %hi(D_800E3D7C) sites of the
   node walk.  Nothing is mis-ordered and no instruction is wrong; the
   candidate simply gives the same quantities different registers.  THIS IS
   PERMUTER FOOD.

   THE ONE MECHANICAL FACT STILL MISSING is the register copy at 800C6310:

       800C62EC  addiu $a0, $v1, %lo(D_80081468)   <- t
       800C62F0  lh    $v1, 0x6($a0)
       800C62F8  addu  $s0, $v1, $zero             <- n   (REPRODUCED, see below)
       800C62FC  div   $zero, $v0, $s0
       800C6300  mfhi  $s3                         <- k   (candidate: $a0)
       800C6304  blez  $s0, .L800C6350
       800C630C  addiu $a2, $v0, %lo(D_800E2970)
       800C6310  addu  $a1, $a0, $zero             <- MISSING: loop.c hoists a
                                                      SECOND pseudo holding t,
                                                      used only by the in-loop
                                                      `% *(s16 *)(t + 6)`
       800C6314  sll   $v0, $s3, 2

   With that copy absent the candidate has one fewer live pseudo across the
   loop, so local-alloc shifts everything: k -> $a0 (target $s3), the room
   record pointer -> $v1 (target $a0), t -> $a1 (target $a0).  That is 25 of
   the 49 rows (156-172 and 209-233).  A further 11 rows (342-474) are the
   mirror-image swap in the node walk: the candidate puts kk in $s4 and the
   shared %hi(D_800E3D7C) pseudo in $s3, the target the other way round, and
   the candidate's first `lw D_800E3D7C` reads the raw `lui` result instead
   of the shared pseudo.  The last 4 rows are at 800C6660..800C666C: the
   target schedules `lbu $a0, 0x24($s1)` / `lbu $a1, 0x25($s1)` into the
   load-delay slot before the 0x2000 test, the candidate leaves a `nop`
   there and emits them after the branch.

   SPELLINGS MEASURED AND REJECTED (words / aligned differences):

   For `addu $s0, $v1, $zero` (the `n` copy) -- SOLVED, keep this body's form:
     * `n = *(s16 *)(t+6); k = u % n;`                     705 / 61  no copy
     * `s32 c = *(s16 *)(t+6); n = c; k = u % n;`          705 / 61  folded
     * same with `s16 c`                                   705 / 61  folded
     * `k = u % *(s16 *)(t+6); n = *(s16 *)(t+6);`         706 / 49  <-- WINS
       (cse commons the second read, so `n` gets a copy of the first read's
       pseudo instead of being the `lh`'s own destination)

   For `addu $a1, $a0, $zero` (the `t` copy) -- STILL OPEN:
     * `k = (k+1) % *(s16 *)((u8 *)D_80081468 + 6)` inline
       in the loop instead of `t + 6`                      705 / 60
       (hoists a bare `lui`, not a copy)
     * `t2 = t;` as the first statement of the LOOP BODY   705 / 61 (folded)
     * `t2 = t;` just BEFORE the loop                      708 / 65
     * `t2 = (u8 *)D_80081468;` as the first statement of
       the loop body, modulus through t2                   707 / 61
       (DOES produce the copy -- `move $a2,$a1` -- but costs a word
       elsewhere and does not move k into $s3)

   For the 800C6660 lbu scheduling -- STILL OPEN:
     * `snd` as a ternary in the call's argument list      705 / 54
     * `ex = e[0x24]; ey = e[0x25];` hoisted before the
       0x2000 test, call takes ex/ey                       705 / 54
     * `snd = 0x3000; q = ...; if (...) snd = 0x300;`      706 / 49  <-- best

   For the kk / %hi(D_800E3D7C) swap -- STILL OPEN:
     * moving `f2 = (u8 *)&D_80083460;` after `q = D_800E3D7C;`  no change
     * moving `s32 k;` to the head of the declaration list        no change

   OTHER LEVERS ALREADY BAKED INTO THIS BODY (do not re-derive):
     * The +0x6C reset fires when `*(s16 *)(p + 0x12) != 0`, NOT `== 0`:
       800C6650 is a `beqz` to the SKIP label.  Inverted it is 2 rows worse.
     * `snd = 0x3000;` then `q = *(u8 **)(r + 0x58) + 0x20;` then the
       `if`.  As a ternary with `q` after it: 704 / 78 -- 17 rows and a word,
       because the 0x3000 is then materialised after the two loads.
     * The random draw is its own statement AHEAD of the D_80081468 base:
       `u = func_800A6D30() & 0xFFFF; t = (u8 *)D_80081468;`.  With the base
       first the `jal` loses `sh $v0, 0x4($s6)` from its delay slot.
     * The +0x10 countdown needs an `s16` local (`tc`), exactly as in the
       landed func_800C5D80.  With `s32` plus an explicit `(s16)` cast the
       `sh` sinks into the `bgtz` delay slot and the function is a word short.
     * The found block's two halfword temps are ONE short-lived `s32 u`, not
       the `x`/`y` loop counters -- with x/y they take callee-saved registers
       and cost 9 rows.
     * Both marking-loop nests are written out TWICE, one per arm of the
       `*(s16 *)(p + 0x6) >= 0` test, not shared.
     * The four `D_80013714 & 1` tests are the DECLARED SYMBOL (one shared
       `lui $s2`); the `(g + 0x8) & 0x800` gate is the RAW CONSTANT
       `*(u16 *)0x80013714`; the state>=2 arm uses `lo = (u8 *)0x80010000`
       for both 0x3714 and 0x234.  These three are the 2026-11-15 run's
       measured levers -- see the four docs/matching-levers.md entries
       titled against this function -- and all three are confirmed here.
     * `g`, `m = *(u8 **)(g + 0x1DC)` and `h = g + 0x1DC` all three at the
       top, the func_8009F13C template.  `g` stays in $s3 across the whole
       function, so the 2026-11-15 "rematerialised base" problem is gone.

   New declarations this body needs:
       extern u8 D_800E0B65[];
       extern u8 D_800E0B73[];
       extern u8 D_800E0BA1[];
   ====================================== 2026-11-21: 175 rows -> 172, on one
                                          DECLARED WIDTH

   `s32 y;' became `u32 y;', found by tools/decomp/widthsweep.py, which tries every
   width for every local greedily.  The word count is unchanged at the
   target's.  Two parks today were closed most of the way by exactly this kind
   of change (func_800B6AC4 s32->s16, 46 rows -> 39 and a word;
   func_8009F644 s32->u8, 81 -> 42 and a word), which is why the sweep exists.
*/
void func_800C5FA8(u8 *p)
{
    u8 *g;
    u8 *h;
    u8 *m;
    u8 *f;
    u8 *f2;
    u8 *f3;
    u8 *lo;
    u8 *t;
    u8 *q;
    u8 *e;
    u8 *r;
    s16 *tbl;
    void *msg;
    void *txt;
    UnkDungeonRoom *rm;
    u16 a;
    s32 st;
    s32 i;
    s32 n;
    s32 k;
    s32 kk;
    s32 x;
    u32 y;
    s32 x1;
    s32 y1;
    s32 snd;
    s32 u;
    s16 tc;
    s16 c;
    u8 *t2;

    g = (u8 *)&D_80083160;
    m = *(u8 **)(g + 0x1DC);
    h = g + 0x1DC;
    st = *(s16 *)(p + 0x0);
    if (st == 0)
    {
        f = (u8 *)&D_80083460;
        if ((*(u16 *)(f + 0x2) & 0x10) != 0)
        {
            *(u16 *)(f + 0x2) |= 0x200;
        }
        a = *(u16 *)(f + 0x2);
        if ((a & 0x200) == 0)
        {
            return;
        }
        if ((*(u32 *)(g + 0x8) & 0x800) != 0 && (*(u16 *)0x80013714 & 0x8) == 0)
        {
            return;
        }
        if (*(s32 *)(f + 0xC) != 0)
        {
            return;
        }
        if (*(s32 *)(f + 0x10) != 0)
        {
            return;
        }
        if ((a & 0x8) != 0)
        {
            return;
        }
        if (*(s16 *)(f + 0x8) != 0)
        {
            return;
        }
        if (*(s16 *)(f + 0x14) != 0)
        {
            return;
        }
        if ((D_800E296C & 0x100000) != 0)
        {
            return;
        }
        if (*(s16 *)(f + 0xA) != 0)
        {
            return;
        }
        if ((a & 0x2C01) != 0)
        {
            return;
        }
        *(u16 *)(f + 0x2) = a & 0xFDFF;
        if (*(s16 *)(p + 0x16) != 0)
        {
            *(s16 *)(p + 0x16) = *(u16 *)(p + 0x16) - 1;
            return;
        }
        func_800A56E0(0x818);
        func_800419EC(0x20, 0x8);
        *(s16 *)(p + 0x14) = *(u16 *)(p + 0x14) + 1;
        if ((D_80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            *(u16 *)(f + 0xA) += 1;
        }
        msg = func_800990FC();
        i = *(s16 *)(p + 0x14);
        if (i == 1)
        {
            txt = func_80099194(D_800E0B65, msg);
            if ((D_80013714 & 1) == 0)
            {
                txt = func_8009929C(0x11, txt);
                txt = func_8009929C(0x4C, txt);
                txt = func_80099254(D_800E0458, txt);
            }
            func_80099290(txt);
            func_800A5720(msg);
            *(s16 *)(p + 0x16) = 0x80;
            return;
        }
        if (i == 2)
        {
            txt = func_80099194(D_800E0B73, msg);
            if ((D_80013714 & 1) == 0)
            {
                txt = func_8009929C(0x11, txt);
                txt = func_8009929C(0x4C, txt);
                txt = func_80099254(D_800E0458, txt);
            }
            func_80099290(txt);
            func_800A5720(msg);
            *(s16 *)(p + 0x16) = 0x40;
            return;
        }
        txt = func_80099194(D_800E0BA1, msg);
        *(s16 *)(p + 0x16) = 0;
        if ((D_80013714 & 1) == 0)
        {
            txt = func_8009929C(0x11, txt);
            txt = func_8009929C(0x4C, txt);
            txt = func_80099254(D_800E0458, txt);
        }
        func_80099290(txt);
        func_800A5720(msg);
        *(s16 *)(p + 0x0) = 1;
        *(u16 *)(f + 0xA) += 1;
        *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
        u = func_800A6D30() & 0xFFFF;
        t = (u8 *)D_80081468;
        k = u % *(s16 *)(t + 0x6);
        n = *(s16 *)(t + 0x6);
        while (n > 0)
        {
            rm = &D_800E2970_s[k];
            if (rm->unk_0A != 0)
            {
                *(s16 *)(p + 0x6) = k;
                u = rm->unk_00 - 1;
                *(s16 *)(p + 0x8) = u;
                *(s16 *)(p + 0xC) = u + rm->unk_04 + 2;
                u = rm->unk_02 - 1;
                *(s16 *)(p + 0xA) = u;
                *(s16 *)(p + 0xE) = u + rm->unk_06 + 2;
                break;
            }
            n--;
            k = (k + 1) % *(s16 *)(t + 0x6);
        }
        if (n == 0)
        {
            *(s16 *)(p + 0x6) = -1;
        }
        *(s16 *)(p + 0x2) = 0x20;
        *(s16 *)(p + 0x10) = 1;
        *(s16 *)(p + 0x4) = 1;
        if (*(s16 *)(p + 0x6) >= 0)
        {
            x1 = *(s16 *)(p + 0xC);
            y1 = *(s16 *)(p + 0xE);
            for (x = *(s16 *)(p + 0x8); x < x1; x++)
            {
                for (y = *(s16 *)(p + 0xA); y < y1; y++)
                {
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) |= 0x8000;
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) &= 0xFF73;
                    func_800B6008(x, y);
                    func_800A7030(x, y);
                }
            }
        }
        else
        {
            x1 = 0x3F;
            y1 = x1;
            for (x = 1; x < x1; x++)
            {
                for (y = 1; y < y1; y++)
                {
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) |= 0x8000;
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) &= 0xFF73;
                    func_800B6008(x, y);
                    func_800A7030(x, y);
                }
            }
        }
        tbl = D_800DCED4;
        kk = *(s16 *)(p + 0x6);
        q = D_800E3D7C;
        f2 = (u8 *)&D_80083460;
        *(s16 *)(p + 0x12) = 0;
        do
        {
            e = *(u8 **)(q - 0x14);
            if (kk == func_8009FBF0(e[0x24], e[0x25]))
            {
                if (q[0x13] == 0)
                {
                    D_800E296C |= 0x2000;
                    func_800C542C(q, tbl[func_800429E4(q)], 0, 0);
                    r = *(u8 **)(D_800E3D7C + 0xAC);
                    if (r != NULL)
                    {
                        func_800C542C(r, tbl[func_800429E4(r)], 0, 0);
                    }
                    r = *(u8 **)(D_800E3D7C + 0xB0);
                    if (r != NULL)
                    {
                        func_800C542C(r, tbl[func_800429E4(r)], 1, 0);
                    }
                    *(s16 *)(p + 0x12) = 1;
                }
                else if ((*(u32 *)(q + 0x14) & 0x4000) != 0)
                {
                    if (*(s16 *)(p + 0x12) == 0)
                    {
                        func_800C1F28(q, *(u8 **)(D_800E3D7C + 0xAC) != q);
                    }
                }
                else
                {
                    r = q;
                    if (*(s16 *)(p + 0x12) != 0)
                    {
                        *(s16 *)(r + 0x6C) = 0;
                    }
                    snd = 0x3000;
                    q = *(u8 **)(r + 0x58) + 0x20;
                    if ((*(u32 *)(r + 0x1C) & 0x2000) != 0)
                    {
                        snd = 0x300;
                    }
                    func_8009A3D0(e[0x24], e[0x25], snd);
                    if ((*(u32 *)(r + 0x14) & 0x20000000) != 0 && r[0x13] == 0x1E)
                    {
                        *(u16 *)(f2 + 0xA) += 1;
                    }
                    func_800A32A4(r);
                    func_8009A028(r);
                    *(u16 *)(r - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                }
            }
        } while ((q = *(u8 **)(q + 0x5C) + 0x20) != D_800E3D7C);
        if (kk >= 0)
        {
            D_800E2970_s[kk].unk_0A = 0;
        }
    }
    else if (st == 1)
    {
        if (*(s16 *)(p + 0x6) >= 0)
        {
            x = *(s16 *)(p + 0x8);
            x1 = *(s16 *)(p + 0xC);
            y1 = *(s16 *)(p + 0xE);
            n = 0;
            while (n < *(s16 *)(p + 0x4))
            {
                for (y = *(s16 *)(p + 0xA); y < y1; y++)
                {
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x2) -=
                        (8 - (func_800A6D30() & 7)) * (*(s16 *)(p + 0x4) + (-1 - n));
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) &= 0xFF73;
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) |= 0x8000;
                }
                if (u)
                {
                    x++;
                }
                else
                {
                    x++;
                }
                if (x >= x1)
                {
                    break;
                }
                n++;
            }
        }
        else
        {
            x = 1;
            x1 = 0x3F;
            y1 = x1;
            n = 0;
            while (n < *(s16 *)(p + 0x4))
            {
                for (y = 1; y < y1; y++)
                {
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x2) -=
                        (8 - (func_800A6D30() & 7)) * (*(s16 *)(p + 0x4) + (-1 - n));
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) &= 0xFF73;
                    *(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m + 0x4) |= 0x8000;
                }
                x++;
                if (x >= x1)
                {
                    break;
                }
                n++;
            }
        }
        tc = *(u16 *)(p + 0x10) - 1;
        *(s16 *)(p + 0x10) = tc;
        if (tc <= 0)
        {
            *(s16 *)(p + 0x10) = 1;
            *(s16 *)(p + 0x4) = *(u16 *)(p + 0x4) + 1;
            func_8009D6F4();
            if (x1 + 0x10 < *(s16 *)(p + 0x4))
            {
                if (*(s16 *)(p + 0x12) == 0)
                {
                    f3 = (u8 *)&D_80083460;
                    *(s16 *)(p + 0x0) = 0;
                    *(u16 *)(f3 + 0xA) -= 1;
                }
                else
                {
                    *(s32 *)(g + 0xCC) = 0;
                    *(s32 *)(g + 0x154) = 0;
                    *(s16 *)(p + 0x0) = *(u16 *)(p + 0x0) + 1;
                }
            }
        }
    }
    else
    {
        lo = (u8 *)0x80010000;
        if ((*(u16 *)(lo + 0x3714) & 0x4) != 0)
        {
            func_80040AA0(3);
            return;
        }
        func_800945E8(D_800E3D7C);
        func_800948BC();
        func_800A6780();
        *(s32 *)(lo + 0x234) += 1;
        D_8008146C = *(u16 *)(lo + 0x234);
        func_80040AA0(D_80082E6B);
        func_800481E0();
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}
