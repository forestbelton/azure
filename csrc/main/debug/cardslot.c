#include "common.h"
#include "main_twin.h"

/* Data objects this unit's own initialisers point at but another
   unit defines; main_twin.h does not declare them. */
extern UnkAnimStep D_804089E8[9];

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_80400908(void);
extern void func_80400EF8(void);
extern void func_804023C4(void);
extern void func_80403194(s32 a, s32 p);
extern void func_804031BC(s32 a, s32 p);
extern void func_80404688(s32 *p);

/* THE 0x80400000 DEBUG TWIN -- cardslot.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x804007A4..0x804007D8   .text from 0x804069FC

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main/debug/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- cardslot.c's assert pair, and nothing else. --- */
/* 0x804007A4 */
const char D_804007A4[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x804007CC */
const char D_804007CC[12] __attribute__((section(".rodata"))) = "cardslot.c\0<";


void func_804069FC(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80406A24(s32 *p)
{
    func_804069FC(p);
    if (func_804016D0() >= 4)
    {
        func_8040343C(0);
        p[-4] = (s32)func_80406DA0;
    }
}

/* No live counterpart: D_8008CA34 is written by six functions in this block
 * and by nothing in the live one. It is a BYTE -- the store is `sb`. */
void func_80406A74(void)
{
    D_8008CA34 = 2;
    func_80400EF8();
    func_8003D548(3);
}

/* Live func_800262BC, with func_80403144 where it has func_804030F4, two of
 * its calls absent, func_80404688 taking one argument, and the 0x80010000
 * read a WORD here where the live one reads ((s16 *)0x80010000)[0x104]. The
 * handler goes through a LOCAL: three stores would cost three %hi pairs. */
void func_80406AA8(s32 *p)
{
    func_80406844((u8 *)0x80010000, p[11], p[9]);
    if (func_80402084(p[11], (u8 *)0x80010000) == 0)
    {
        p[13] = (s32)func_80406DA0;
        func_80403144((s32)(p - 8));
        p[-4] = (s32)func_804069FC;
    }
    else
    {
        func_80405A00((s32 *)p[1], p[11]);
        func_80404688((s32 *)p[p[11] + 3]);
        if (D_80010208.unk_00 != 0)
        {
            p[-4] = (s32)func_80406A74;
        }
        else
        {
            p[-4] = (s32)func_80406DA0;
        }
    }
    func_80400908();
    p[16] = 0;
}

void func_80406B74(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80406AA8;
    }
}

void func_80406B9C(s32 *p)
{
    p[15] = 1;
    func_804031BC((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80406B74;
}

/* Live func_80026410, same body and the same p[14]. The ONE difference is a
 * delay slot: the live build steals the function address's `lui` into the
 * `beqz`'s slot, this build leaves a nop there. Under 4.1's compiler that
 * needed a sized DATA alias of the function symbol to make the address one
 * insn, so reorg had nothing to take -- 12 differing plain, 0 with the alias.
 * Under tools/psx272gcc the address is one insn to begin with and the plain
 * spelling matches, so the alias is gone. */
void func_80406BEC(s32 *p)
{
    if (p[14] != 0)
    {
        p[-4] = (s32)func_80406DA0;
    }
    else
    {
        func_80406B9C(p);
    }
}

void func_80406C2C(s32 *p)
{
    p[13] = (s32)func_80406BEC;
    func_80403194((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_80406A24;
}

void func_80406C74(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_8040320C((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

void func_80406CB8(s32 *p)
{
    p[13] = (s32)func_80406DA0;
    func_80403234((s32)(p - 8));
    p[-4] = (s32)func_80406A24;
}

void func_80406CFC(s32 *p)
{
    if (D_8009DDD8[p[11]][0] != 0)
    {
        p[-4] = (s32)func_80406C2C;
    }
    else if (func_80402268() >= 3)
    {
        p[-4] = (s32)func_80406B9C;
    }
    else if (func_80402290() != 0)
    {
        func_80406C74(p);
    }
    else
    {
        func_80406CB8(p);
    }
}

/* Live func_800265B8, the sibling of func_80405FC4 above: the 0x40 arm goes
 * straight to the close tail, p[12] loses 4 here rather than 2, and the
 * state word is p[11].
 *
 * DEVICE: that first arm REPEATS the close tail instead of a `goto` to it,
 * and gcc's cross-jumping merges the two into the one `j`. The four extra
 * references to `p` are what the device is for -- with a `goto`, `dir`
 * outranks `p` in global.c's allocno order and the two take $s0 and $s1 the
 * other way round, 22 differing with every other word already right. The
 * live func_800265B8 uses the goto, and at -G0 either spelling matches. */
void func_80406DA0(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_801379A8.unk_00 != 0)
    {
        if (D_801379B0.unk_00 & 0x40)
        {
            func_80063FF8(0x515);
            func_804070C4(p - 8);
            func_804083FC(p[8], p[9]);
            return;
        }
        if (D_801379B0.unk_00 & 0x20)
        {
            func_80063FF8(0x514);
            func_80406CFC(p);
        }
        else if (D_801379A8.unk_00 & 0x5000)
        {
            if (D_801379B0.unk_00 & 0x5000)
            {
                p[12] = 0;
                if (D_801379B0.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379B0.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_801379A8.unk_00 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_801379A8.unk_00 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            p[11] = func_80058FF0(p[11], dir, 5);
            func_804067BC(p);
        }
    }
    r = func_804016D0();
    func_804018FC();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_804070C4(p - 8);
    func_804083FC(p[8], p[9]);
}

void func_80406F0C(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80406F14(s32 *p, s32 v)
{
    func_80406F0C(p, v);
}

void func_80406F34(s32 *p)
{
    func_804067BC(p);
    func_80400908();
    p[-4] = (s32)func_80406DA0;
}

/* Live func_800267C0, and func_804065CC's twin in every sense: the same body
 * one slot over (p[10] and p[n+3] where that one has p[9] and p[n+1]), the
 * same revision collapse -- func_80406F34 at both the p[13] and the trailing
 * p[-4] site, where the live source has func_80026784 and func_80026748 --
 * and the same delay slot, which was 11 differing under 4.1's compiler and
 * needed a sized alias at the last site; it is 0 plain under
 * tools/psx272gcc. */
void func_80406F70(s32 *p)
{
    s32 n = p[10];

    if (n < 5)
    {
        if (func_80401E2C(n) != 0)
        {
            p[n + 3] = func_80404364((s32)(p - 8), n);
            p[10] = p[10] + 1;
        }
        else
        {
            p[13] = (s32)func_80406F34;
            func_8040325C((s32)(p - 8));
            p[-4] = (s32)func_804069FC;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80406F34;
    }
}

s32 *func_8040701C(s32 *p, s32 a)
{
    s32 *q = func_8003C480(0);
    s32 *r;

    if (q != 0)
    {
        r = q + 8;
        r[9] = a;
        func_80406F14(r, (s32)p);
        func_804023C4();
        func_80402214();
        q[8] = func_80404C84(q, 0);
        r[1] = (s32)func_804057F0(q, r[11]);
        r[2] = (s32)func_80407C10(q);
        r[10] = 0;
        q[4] = (s32)func_80406F70;
    }
    return q;
}

/* The same two devices as func_80406720 above. */
void func_804070C4(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_80404DBC((u8 *)q[0]);
        func_804059A8((s32 *)q[1]);
        func_80407D38((s32 *)q[2]);
        i = 0;
        do
        {
            do
            {
                s32 *v = (s32 *)q[3];

                q++;
                __asm__("");
                func_80404570(v);
            } while (0);
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
}

/* Live func_800269B4, with the group's ptrs reached through p[0x18] -- the
 * same extra indirection as func_804072CC and func_80407458. */
void func_80407160(s32 *p)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        s32 k = D_80408D40[p[9]][i];
        u32 *e = ((u32 **)p[0x18])[k + 2];

        if (e[0] != 0)
        {
            func_8004DDBC((u32 *)((u8 *)p + (k * 12 + 4)), D_80408D4C[i], e);
        }
        ((UnkCardPair **)p[0x18])[i]->unk_04->unk_00 =
            D_80408D44[D_80408D40[p[9]][i]];
        ((UnkCardPair **)p[0x18])[i + 2]->unk_04->unk_00 =
            D_80408D44[D_80408D40[p[9]][i]];
    }
}

void func_80407298(s32 *p)
{
    func_8004DCCC(p + 1);
    func_8004DCCC(p + 4);
}

/* Live func_80026B18, with ONE MORE INDIRECTION: the pair comes through
 * p[0x18] here, where the live one reads the group's own ptrs array. Same
 * difference as func_80407458 and func_804074C4. */
void func_804072CC(u8 *p, s32 *b, s32 *c)
{
    s32 any = 0;
    s32 i;

    for (i = 0; i < 2; i++)
    {
        if (c[i] != 0)
        {
            UnkCardPair *q = ((UnkCardPair **)((s32 *)p)[0x18])[2 + i];

            any = 1;
            if (b[i] != 0)
            {
                func_8004DDBC((u32 *)(p + (i * 0xC + 4)), (u8 *)D_80408A30, (u32 *)q);
            }
            else
            {
                q->unk_00 = 0;
                *(s32 *)(p + i * 0xC + 8) = 0;
            }
        }
    }
    if (any != 0)
    {
        func_80407160((s32 *)p);
    }
}

void func_804073B0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002D690_a;
    u32 *s2 = D_8002D6A0_a;

    a[0] = s1[0];
    a[1] = s1[1];
    a[2] = s1[2];
    a[3] = s1[3];
    b[0] = s2[0];
    b[1] = s2[1];
    b[2] = s2[2];
    p->unk_04 = (UnkCardPrim *)a;
    p->unk_08 = (UnkCardPrim12 *)b;
    p->unk_00 = 0;
    p->unk_04->unk_08 = -160;
    p->unk_04->unk_0A = -120;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

/* Live func_80026C94. REVISION: one more INDIRECTION. The live struct has
 * `ptrs` inline at +0x60; here +0x60 holds a POINTER to the pair array, so
 * each access is `lw 0x60($a0)` then `lw N($v0)`. And it is re-read for the
 * second pair rather than held in a local, because the `sw` through the
 * first pair invalidates cse's entry for it. */
void func_80407458(s32 *p)
{
    UnkCardPair *q;

    q = ((UnkCardPair **)p[0x18])[0];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80409110;
    q = ((UnkCardPair **)p[0x18])[1];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80409128;
}

/* Live func_80026CF0, func_80407458's sibling and the same extra
 * indirection through +0x60. */
void func_804074C4(s32 *p)
{
    UnkCardPair *q;

    q = ((UnkCardPair **)p[0x18])[2];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    func_8004DDBC((u32 *)(p + 1), (u8 *)D_80408A30, (u32 *)q);
    q = ((UnkCardPair **)p[0x18])[3];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    func_8004DDBC((u32 *)(p + 4), (u8 *)D_80408A30, (u32 *)q);
}

void func_80407558(UnkCardGrp044 *c)
{
    func_80407458(c);
    func_804074C4(c);
    func_804072CC((u8 *)c, (s32 *)D_80408D54, (s32 *)D_80408D5C);
    func_80407160((s32 *)c);
}

s32 func_804075A8(s32 a)
{
    return func_80047358(a, D_80408D64, 4);
}

/* The sixth of the family, and the one whose ptrs is a POINTER at c[0x18]
 * rather than an inline array -- same revision difference as func_804072CC,
 * func_80407458 and func_804074C4. Raw word indices: shared at c[0x11],
 * ents at c[0x19], prims at c[0x29]. */
void func_804075D0(s32 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        ((UnkCardPair **)c[0x18])[i] = (UnkCardPair *)&c[0x19 + i * 4];
        c[0x29 + i * 4] = D_8002D690_a[0];
        c[0x2A + i * 4] = D_8002D690_a[1];
        c[0x2B + i * 4] = D_8002D690_a[2];
        c[0x2C + i * 4] = D_8002D690_a[3];
        c[0x1A + i * 4] = (s32)&c[0x29 + i * 4];
        c[0x1B + i * 4] = (s32)&c[0x11];
    }
    c[0x11] = D_8002D6A0.unk_00;
    c[0x12] = D_8002D6A4.unk_00;
    c[0x13] = D_8002D6A8.unk_00;
}

void func_80407688(s32 *p, s32 v)
{
    p[0] = v;
    func_804073B0((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80407558(p);
}

/* Live func_80026ED0. ONE argument, not two -- the live `r[9] = a;` has no
 * counterpart -- and, as in func_804057F0, the allocation failure is an
 * assert this build carries and the live one does not.  Raw word indices for
 * the UnkCardGrp044: root at r[0x14], its unk_0C at r[0x17], ptrs at r[0x18]. */
s32 *func_804076C8(s32 *p)
{
    s32 *q = (s32 *)D_8040C318;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_8040C318, 0x42);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804007A4, D_804007CC, 0x136);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_804077D4(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0xE4);
    }
    func_804075D0(r, 4);
    q[3] = (s32)(r + 0x14);
    r[0x17] = func_804075A8(r[0x18]);
    func_80407688(r, (s32)p);
    q[4] = (s32)func_80407298;
    return q;
}

/* Live func_80026FB4 plus an assert on the pointer. */
void func_804077D4(s32 *p)
{
    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804007A4, D_804007CC, 0x14B);
        func_8007BEF0(1);
    }
    ((u16 *)p)[15] |= 0x8000;
    D_8008DAB4 |= 0x8000;
}

void func_80407830(s32 *p, s32 a)
{
    s32 *q = p + 8;

    if (p == 0)
    {
        __asm__ volatile("");
        func_8007C040(D_804007A4, D_804007CC, 0x153);
        func_8007BEF0(1);
    }
    p[0x11] = a;
    func_80407160(q);
}

void func_8040787C(s32 *p, s32 *b, s32 *c)
{
    func_804072CC((u8 *)(p + 8), b, c);
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S `.data`, 0x80408D40..0x80408D6C.

   The twin's data is two runs: the TABLES, 0x804089E0..0x8040922C, which the
   nine units own, and the STATE run behind it, which one object has to hold
   because one object contributes one `.data` run. That is the same shape and
   the same forced split as the live 0x80020000 overlay -- docs/overlays.md,
   "The data IS carved, and a unit can only own the FIRST data run".

   Ownership is by DEFINITION, not by use: link order makes a unit's objects
   contiguous and in unit order, so the referencing units read down the
   address list come out non-decreasing, and where they do not, the object is
   merely USED by the later unit. Applying this partition moves no bytes, so
   byte-identity cannot check it -- it is a claim about the original source.
   ------------------------------------------------------------------------- */

/* 0x80408D40 */
/* D_80028068's twin. */
u8 D_80408D40[2][2] __attribute__((section(".data"))) = {
    {0x01, 0x00},
    {0x00, 0x01},
};

/* 0x80408D44 */
/* D_8002806C's twin. */
s32 D_80408D44[2] __attribute__((section(".data"))) = {0x00404040, 0x00808080};

/* 0x80408D4C */
/* D_80028074's twin: the two animation scripts above. */
u8 *D_80408D4C[2] __attribute__((section(".data"))) = {
    (u8 *)D_80408A30,
    (u8 *)D_804089E8,
};

/* 0x80408D54 */
/* D_8002807C's twin. */
u8 D_80408D54[0x8] __attribute__((section(".data"))) = {0};

/* 0x80408D5C */
/* D_80028084's twin. */
u8 D_80408D5C[0x8] __attribute__((section(".data"))) = {
    0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

/* 0x80408D64 */
/* D_8002808C's twin. */
u8 D_80408D64[0x8] __attribute__((section(".data"))) = {
    0x00, 0x01, 0x02, 0x03, 0x04, 0x00, 0x00, 0x83};

