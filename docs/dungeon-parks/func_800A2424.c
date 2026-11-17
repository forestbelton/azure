/* PARK -- func_800A2424, the dungeon "level down" twin of func_800A1D4C.
   432 of 432 words, 10 differing (ovlverify, masked); aligndiff reports no
   opcode, immediate or displacement difference -- all ten are register
   names, and they are ONE swap:

     257  addiu $a0, $s1, 6     vs  addiu $a1, $s1, 0x6     (induction init)
     260  lbu   $a1, 8($a0)     vs  lbu   $a0, 0x8($a1)
     262  beqz  $a1             vs  beqz  $a0
     263  sll   $v0, $a1, 2     vs  sll   $v0, $a0, 2
     264  addu  $v0, $v0, $a1   vs  addu  $v0, $v0, $a0
     273  lbu   $v1, 10($a0)    vs  lbu   $v1, 0xA($a1)
     278  sb    $v0, 10($a0)    vs  sb    $v0, 0xA($a1)
     282  addiu $a0, $a0, -3    vs  addiu $a1, $a1, -0x3

   i.e. the class local `c` (reused as the loop's rule index) and loop.c's
   induction pointer have exchanged $a0 and $a1; plus words 369/370, where
   `addu $s0, $zero, $zero` and `addiu $s2, $v0, %lo(D_8006DE24)` come out
   in the opposite order.

   Three levers were worth 26 of the 36 differing this started from, and all
   three are allocation, not shape:

     1. THE `t[x] * q[x]` PRODUCT HAS TO SHARE THE MESSAGE LOOP'S COUNTER.
        The target keeps it in $s0, a callee-saved register, although it
        crosses no call. Written as its own `s32 r` it is a short-lived
        allocno, global.c gives it $a0/$v1 from the call-clobbered end of
        reg_alloc_order, and that is 36 differing. Written into `i` -- the
        variable that is also the message loop's counter, and therefore
        crosses twelve calls -- find_reg is forced to the callee-saved end
        and it lands in $s0: 36 -> 17.
     2. THE SPELL LOOP'S RULE INDEX HAS TO BE THE CLASS LOCAL `c`.
        A fresh local is 17; `hi` is 14; `c` is 10. `u`, `n`, `w`, `hp` and
        a `u8`/`s16`/`u32` `c` were all measured and are 17-21 (a `u8` `c`
        is 433 words).
     3. `(*(u16 *)0x80013714 & 1) == 0` FOR THE FIRST OF THE TWO TESTS.
        The declared `D_80013714` lets reorg lift the `lui %hi` into the
        `beqz $v0, .L800A2AB8` delay slot; the target's slot holds the
        `addiu $v0, $zero, 0x1` that reorg COPIES out of the shared
        `return 1` block, which a constant address's single rtl insn leaves
        it free to take. 431 words -> 432, and 149 -> 36 differing. The
        SECOND test measures identically either way, so it is left as the
        symbol. Five spellings of the `return 1` itself (early return,
        return inside the arm, a `goto`, a value held in a local) are all
        431 words -- the delay slot is the only control there is.

   Also measured and rejected: the p[0..3] loads read after the `hi` line
   (431/202 before lever 3); `lo` used as the product with `hi` computed
   first (432/56); the product fully inlined as `t[x] * q[x] * ...` twice
   (432/42); explicit `r1`/`r2` product temporaries (448 words); the whole
   difference as one expression with no `lo`/`hi` locals (432/86).

   New declarations it needs are in the agent's report; note that this
   file's `s32 func_800A2424(u8 *, s32);` is wrong -- the second parameter
   is a SHORT (`sll $v0, $s6, 16` + `beqz`). 
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
   PROTOTYPE CHANGE ON LANDING: `s32 func_800A2424(u8 *, s32);' narrows to
   `s32 func_800A2424(u8 *, s16);'. Six landed callers re-measured at 0.
*/
s32 func_800A2424(u8 *p, s16 f)
{
    u8 *t;
    u8 *q;
    u8 *g;
    void *m;
    void *h;
    UnkDungeonName0 *d;
    s32 lvl;
    s32 c;
    s32 i;
    s32 w;
    s32 u;
    s32 lo;
    s32 hi;
    s32 n;
    u32 hp;
    u8 e[3];

    if (p[0x11] < 2)
    {
        return 0;
    }
    p[0x11] -= 1;
    lvl = p[0x11];
    c = p[0x13];
    *(u32 *)(p + 0x18) = D_800835E8[lvl] - 1;
    t = D_800DDCBC[c];
    q = (u8 *)&D_8006D168[c];
    lo = q[5] + t[5] * (lvl - 1) / 16 + t[5] * func_800647A0(((lvl - 1) * t[5]) << 11) / 32768;
    hi = q[5] + t[5] * lvl / 16 + t[5] * func_800647A0((lvl * t[5]) << 11) / 32768;
    n = p[5] + (lo - hi);
    if (n == 0)
    {
        n = 1;
    }
    p[5] = n;

    i = t[4] * q[4];
    lo = q[4] + i * (lvl - 1) / 1024;
    hi = q[4] + i * lvl / 1024;
    n = p[4] + (lo - hi);
    if (n == 0)
    {
        n = 1;
    }
    p[4] = n;

    i = t[0] * q[0];
    n = p[0];
    lo = q[0] + i * (lvl - 1) / 64;
    hi = q[0] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[0] = n;

    i = t[1] * q[1];
    n = p[1];
    lo = q[1] + i * (lvl - 1) / 64;
    hi = q[1] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[1] = n;

    i = t[2] * q[2];
    n = p[2];
    lo = q[2] + i * (lvl - 1) / 64;
    hi = q[2] + i * lvl / 64;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
    }
    p[2] = n;

    i = t[3] * q[3];
    n = p[3];
    lo = q[3] + i * (lvl - 1) / 1024;
    hi = q[3] + i * lvl / 1024;
    n += lo - hi;
    if (n == 0)
    {
        n = 1;
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
        for (i = 2; i >= 0; i--)
        {
            e[i] = 0;
            c = p[i * 3 + 8];
            if (c != 0)
            {
                u = D_8006DE24[c].unk_10 >> 4;
                if ((u & *(u32 *)(p + 0x14)) != 0)
                {
                    if (p[i * 3 + 0xA] >= 2)
                    {
                        p[i * 3 + 0xA] -= 1;
                        e[i] = 1;
                    }
                }
            }
        }
    }
    func_80041E70(p);
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
    if (p[0x66] < p[0x25])
    {
        p[0x25] = p[0x66];
        p[0x24] = 0xFF;
    }
    if (p[0x68] < p[0x26])
    {
        p[0x26] = p[0x68];
    }
    if (p[0x69] < p[0x27])
    {
        p[0x27] = p[0x69];
    }
    if (f != 0)
    {
        if ((*(u16 *)0x80013714 & 1) == 0)
        {
            D_800DCF4F = 1;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
        }
        func_800B4C7C(0x8003, p, -3, 1);
        func_800A56E0(0x615);
        m = func_800990FC();
        h = func_80099194(D_800E098B, m);
        h = (void *)func_8009929C(0xA, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800E099A, h);
        h = func_8003AD08(p[0x11], h);
        h = func_80099194(D_80089000, h);
        if (p[0x13] != 0 && (*(u32 *)(p + 0x14) & 0x4000) != 0)
        {
            d = (UnkDungeonName0 *)D_8006DE24;
            for (i = 0; i < 3; i++)
            {
                if (e[i] != 0)
                {
                    h = (void *)func_8009929C(0xA, h);
                    h = func_80099194(d[p[i * 3 + 8]].unk_00, h);
                    h = func_80099194(D_800E09B2, h);
                    h = func_80099194(D_800E09BB, h);
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
    return 1;
}
