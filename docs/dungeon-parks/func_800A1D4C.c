/* PARK -- func_800A1D4C, the dungeon "level up" twin of func_800A2424.
   438 of 438 words, 45 differing (ovlverify, masked); aligndiff shows NO
   opcode, immediate or displacement difference anywhere -- every remaining
   word is a register name. Residue, by region:

     * stat 5's SECOND half (target words 55-71, 14 words): t[5] is $v1 in
       the target and $a0 here, the two `mflo`s are swapped ($a0/$v1 against
       $v1/$a0), and `lbu $v1, 0x5($s2)` (the p[5] read) is scheduled TWO
       words early here (69 against the target's 71). The FIRST half of the
       same statement pair (the `lo` computation, words 35-54) is exact.
     * the spell loop (words 274-313, ~14 words): the class local `c` and
       loop.c's induction pointer have swapped registers -- target has the
       pointer in $a1 and `c` in $a0, this has the pointer in $a2 and `c` in
       $a1. Same swap, one register further out, as func_800A2424's residue.
     * the `0x64 - lvl` preheader (words 261-273, 4 words): the target
       computes `addiu $v0, $zero, 0x64` + `subu $a3, $v0, $s1` INSIDE the
       `if (p[0x13] != 0)` arm; this computes it before the test, and still
       hoists `li $t1, 0x63` out of the loop where the target rematerialises
       `addiu $v0, $zero, 0x63` at the use (word-count neutral, 2 words).
     * words 375/376: `addu $s0, $zero, $zero` and the `addiu %lo(D_8006DE24)`
       are in the opposite order.

   Spellings measured and rejected (ovlverify words/differing):

     `(D_80013714 & 1) == 0` for the FIRST of the two tests   437/236
        -- the declared symbol lets reorg lift its `lui` into the
        `beqz $v0, .L800A23F8` delay slot where the target has a `nop`.
        `(*(u16 *)0x80013714 & 1) == 0` is the whole word (lever
        "reorg WILL LIFT A DECLARED SYMBOL'S %hi ... AND WILL NOT LIFT A
        CONSTANT ADDRESS'S"). The SECOND test measures the same either way.
     `o = p[x]; n = o + (hi - lo);`                            437/335
        -- no `addu $a0, $v1, $zero` copy. `o = n = p[x];` + `n += hi - lo;`
        is what produces it (4 words).
     stat loads left after the `hi` line (stats 0..3)          439/337
        -- the target reads p[0..3] BEFORE the `lo` division, i.e. in the
        block that ends at the division's first `bgez`; stats 4 and 5 read
        theirs late. Moving all six early is 443/424, moving only 0/1 is
        439/344.
     no `u2 = 0x64 - lvl;` hoist (let loop.c do it)            438/165
        -- the hoist shortens `lvl`'s live range enough to flip the
        allocation priority against `p`: without it p is $s1 / lvl $s2,
        with it p is $s2 / lvl $s1 as the target has. Placed inside the
        `if (p[0x13] != 0)` arm it is 439/185.
     product in its own `r` rather than in `i`                 438/117
        -- see func_800A2424's park: sharing the message loop's counter is
        what puts the product in the callee-saved $s0.
     loop index local `k` of its own rather than `c`           438/?? (the
        same +7-differing step func_800A2424 measured, 17 against 10).
     division result in `n`/`hp` 46, in `hi` 118, in `lo` 440/433.
     `o = p[5]; n = p[5] + (hi - lo);` for stat 5 only         437/355.

   New declarations it needs are in the agent's report (func_800647A0,
   void func_800A1D4C(u8 *, s16), eight D_800E09xx/D_800E09xx strings, and
   func_800A2424's second parameter corrected from s32 to s16). 
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
        o = n = p[5];
        n += hi - lo;
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
        u2 = 0x64 - lvl;
        if (p[0x13] != 0)
        {
            for (i = 2; i >= 0; i--)
            {
                e[i] = 0;
                c = p[i * 3 + 8];
                if (c != 0)
                {
                    u = D_8006DE24[c].unk_10 >> 4;
                    if ((u & *(u32 *)(p + 0x14)) != 0 || (u & 7) == 0)
                    {
                        if (p[i * 3 + 0xA] < 0x63)
                        {
                            p[i * 3 + 0xA] += 1;
                            e[i] = 1;
                            r = (0x63 - p[i * 3 + 0xA]) / u2 + p[i * 3 + 0xA];
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
                d = (UnkDungeonName0 *)D_8006DE24;
                for (i = 0; i < 3; i++)
                {
                    if (e[i] != 0)
                    {
                        h = (void *)func_8009929C(0xA, h);
                        h = func_80099194(d[p[i * 3 + 8]].unk_00, h);
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
