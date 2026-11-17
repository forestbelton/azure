/* PARK -- func_800C5FA8, agent j59, 2026-11-16.
   target 706 words; best candidate 705 words, 61 aligned differences
   (ovlverify reports a much larger masked word-diff: almost all of it is the
   one-word shortfall sliding every later word by one, so read aligndiff).

   STRUCTURE IS BELIEVED CORRECT.  Everything from the state-1 marking loops
   (listing offset 0x77C onward) to the epilogue is byte-identical, as are
   the prologue, the whole gate chain, the three message arms and the
   D_80013714/D_80010234 tail.

   THE WHOLE RESIDUE IS ONE MISSING WORD AND ITS FALLOUT, in the random-room
   search between 800C62E8 and 800C634C.  The target has TWO register COPY
   insns in that preheader that no spelling tried here reproduces:

       800C62F0  lh   $v1, 0x6($a0)
       800C62F8  addu $s0, $v1, $zero      <-- MISSING: n gets its own pseudo
       800C62FC  div  $zero, $v0, $s0
       ...
       800C630C  addiu $a2, $v0, %lo(D_800E2970)
       800C6310  addu  $a1, $a0, $zero     <-- present only as the same reg

   The candidate instead emits `lh $s0, 0x6($a2)` straight into the counter
   and reuses one register for the D_80081468 base inside and outside the
   loop.  Because the candidate has one fewer live pseudo there, local-alloc
   shifts the whole search loop down one register -- k lands in $a0 where the
   target has $s3, the room pointer in $a1 where the target has $a0, the
   D_800E2970 base in $a3 where the target has $a2 -- which is where ~27 of
   the 61 aligned rows come from, plus 11 more from every `-> epilogue`
   branch being one word short.  FIX THE COPY AND MOST OF THE REST GOES.

   REJECTED SPELLINGS, each measured on this body (words / aligned diffs):

     * `n = *(s16 *)(t + 0x6); k = u % n;`            705 / 61   (this body)
     * `c = *(s16 *)(t+6); n = c; k = u % n;` with
       `s32 c`                                        705 / 61   (copy folded)
     * same with `s16 c`                              705 / 61   (copy folded)
     * `k = (k+1) % *(s16 *)((u8 *)D_80081468 + 6)`
       inline in the loop instead of `t + 6`          704 / 60   (hoists a
       fresh `lui` into the preheader, not a copy)
     * `t2 = t;` as the first statement of the loop
       body, modulus read through `t2`                705 / 61   (regmove
       folds the copy away)

   OTHER MEASURED LEVERS THAT ARE ALREADY IN THIS BODY (do not re-derive):

     * The +0x6C reset fires when `*(s16 *)(p + 0x12) != 0`, not `== 0`
       (800C6650 is `beqz` to the SKIP label).  Inverted it is 2 worse.
     * `snd = 0x3000; q = *(u8 **)(r + 0x58) + 0x20; if (...) snd = 0x300;`
       as three statements.  As the ternary
       `snd = (...) ? 0x300 : 0x3000;` with `q` after it: 704 words /
       78 differing -- 17 rows and a word, because the 0x3000 has to be
       materialised after the two loads instead of before them.
     * The random-room draw is its own statement AHEAD of the D_80081468
       base: `u = func_800A6D30() & 0xFFFF; t = (u8 *)D_80081468;`.  With the
       base first the `jal` loses `sh $v0, 0x4($s6)` from its delay slot.
     * The +0x10 countdown needs an `s16` local (`tc`), as in func_800C5D80:
       with an `s32` local and an explicit `(s16)` cast the `sh` sinks into
       the `bgtz` delay slot and the function is a word shorter again.
     * The four `D_80013714 & 1` tests are the DECLARED SYMBOL (one shared
       `lui $s2`); the `(g + 0x8) & 0x800` gate is the RAW CONSTANT
       `*(u16 *)0x80013714`; the state>=2 arm uses `lo = (u8 *)0x80010000`
       for both 0x3714 and 0x234.  All three already measured by the
       2026-11-15 run -- see the four docs/matching-levers.md entries titled
       against this function.
     * The found-block's two halfword temps are ONE short-lived `s32 u`, not
       the `x`/`y` loop counters: with x/y they take callee-saved registers
       and cost 9 rows.
     * Both marking-loop nests are written out TWICE (once per arm of the
       `*(s16 *)(p + 0x6) >= 0` test), not shared.

   New declarations this body needs:
       extern u8 D_800E0B65[];
       extern u8 D_800E0B73[];
       extern u8 D_800E0BA1[];
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
    s32 y;
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
        c = *(s16 *)(t + 0x6);
        n = c;
        k = u % n;
        while (n > 0)
        {
            t2 = t;
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
            k = (k + 1) % *(s16 *)(t2 + 0x6);
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
        f2 = (u8 *)&D_80083460;
        kk = *(s16 *)(p + 0x6);
        q = D_800E3D7C;
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
                x++;
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
