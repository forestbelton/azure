#include "common.h"
#include "ovl_dungeon_decls.h"

extern u8 D_800E0CAE[];
extern u8 D_800E0CDC[];
extern u8 D_800E0CF3[];
extern u8 D_800E0D1B[];
extern u8 D_800E0D36[];
extern u8 D_80089084[];

extern u8 D_80089080[];
extern u8 D_800DD880[];
extern u8 D_800DD8B4[];
extern u8 D_800E1C58[];
s32 func_80069EF8(void);

extern u8 *D_80073618[];
extern u8 *D_800DF03C[];
s32 func_80069E98(u8 *);
s8 *func_8004E298(u8 *, u8 *, s32);
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f);

void func_800894A0(void)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    func_80040190();
    if ((D_800814A0 & 0x8000) != 0)
    {
        func_800401FC();
    }
    D_800814A0 = 0;
    if (D_80080A86 == 0)
    {
        func_8004027C();
        if ((D_80013714 & 2) != 0)
        {
            return;
        }
        if (*(u32 *)(g + 0x1DC) != 0)
        {
            func_8004D70C();
            func_800D13B8();
            if ((*(u32 *)(D_800814A8 + 0x1C) & 0x10) == 0)
            {
                func_800CF8E4();
            }
        }
    }
    func_800897E4();
    func_80044B48();
    func_80089588();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80089588);

/* 175 words, 0 differing.  Three alternative loops over the same pair of
   arrays: D_80083360[i] is a handler pointer and D_800833E0[i] the object it
   runs on; the two flag tests pick which handlers are allowed to run and what
   happens after each call.  Four spellings were worth 157 of the 175 words:

   1. EACH LOOP INDEXES THE ARRAYS; IT DOES NOT WALK POINTERS.  loop.c turns
      `D_80083360[i]` into the pointer bivs the listing shows, and in the
      first two loops it then ELIMINATES `i`, rewriting the exit test as
      `addiu $v0, $s3, 0x80` / `slt $v0, $s0, $v0` -- a SIGNED compare, which
      a pointer compare (`sltu`) can never give.  maybe_eliminate_biv puts
      that `addiu` inside the loop rather than the preheader exactly when
      `threshold < insn_count`, and the pre-strength-reduction insns are also
      what keep `insn_count` above move_movables' threshold, so the
      `lui %hi(D_800DCF80)` and `lui %hi(D_80083160)` stay inside their loops.
      Hand-written pointer walks measured 174 words / 34 aligned rows: the
      bodies were right and every one of those rows was a hoisted `lui` or
      the register numbering that followed from it.

   2. `D_80083360[i] = 0` IS THE `else` OF `if (s != NULL)`, NOT THE `if` OF
      `if (s == NULL)`.  Writing it the other way puts the store inline and
      costs a `j` over it -- and, because the store then no longer sits
      between the call and the shared `addiu $s0, $s0, 0x4`, the call path
      reaches that increment by fall-through instead of by `j .L800898B8`,
      which is where ten of the words went.  reorg steals the increment into
      the jump's delay slot, and that -- not a failure to cross-jump -- is why
      the target seems to emit the increment twice per loop.  165 words.

   3. `cam` AND `s` ARE ONE VARIABLE.  Loop 2 overwrites the object pointer
      with `&D_80083498` on the match path, so the single pseudo is live
      across a call and takes a callee-saved register ($s1) in all three
      loops.  A separate `cam` local leaves `s` in $a0 everywhere: 16 rows.

   4. LOOP 2'S `a2` AND LOOP 1'S `a` MUST BE DIFFERENT LOCALS, and `a2` must
      be set at the top of loop 2's body.  loop.c then hoists it as $s5
      (%hi) / $s6 (the address), cse2 folds the `D_800DCF80[0]` load's own
      `lui` onto $s5, and $s5 is the SEVENTH callee-saved register the target
      saves -- the last two words.  Sharing one local costs 18 rows; setting
      it beside its use costs the hoist and 2 words.

   No prototype for this function: csrc/ovl_dungeon.c:2075 calls it seven
   lines above, on an implicit `int` declaration, and that is load-bearing. */
void func_800897E4(void)
{
    u32 *a;
    u32 *a2;
    u32 *b;
    u32 *tbl;
    u8 *s;
    void (*h)(u8 *, s32, s32);
    s32 save;
    s32 i;

    if ((D_800E296C & 0x2000000) != 0)
    {
        tbl = D_800DCFA4;
        for (i = 0; i < 0x20; i++)
        {
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        a = D_800DCF80;
                        b = tbl;
                        for (;;)
                        {
                            if (*b == (u32)h)
                            {
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                break;
                            }
                            if (*a == 0)
                            {
                                break;
                            }
                            a++;
                            b++;
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
    else if ((*(s32 *)(D_800814A8 + 0x1C) & 0x10) != 0)
    {
        for (i = 0; i < 0x20; i++)
        {
            a2 = D_800DCF80;
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        b = a2 + 1;
                        if (D_800DCF80[0] == (u32)h)
                        {
                            s = D_80083498;
                            if (*(s32 *)(s + 0x14) != 0)
                            {
                                save = *(s32 *)(s + 0x18);
                                *(s32 *)(s + 0x18) = 0;
                                h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                *(s32 *)(s + 0x18) = save;
                            }
                        }
                        else
                        {
                            for (;;)
                            {
                                if (*b == (u32)h)
                                {
                                    h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                                    break;
                                }
                                if (*b == 0)
                                {
                                    break;
                                }
                                b++;
                            }
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
    else
    {
        for (i = 0; i < 0x20; i++)
        {
            h = (void (*)(u8 *, s32, s32))D_80083360[i];
            if (h != NULL)
            {
                s = (u8 *)D_800833E0[i];
                if (s != NULL)
                {
                    if ((*(u16 *)(s + 0x1E) & 0x800) == 0)
                    {
                        h(s + 0x20, *(s32 *)(s + 0x8), *(s32 *)(s + 0xC));
                        if (func_80045310(*(u32 *)(D_80083160.unk_00 + 0x8D0)) != 0)
                        {
                            return;
                        }
                    }
                }
                else
                {
                    D_80083360[i] = 0;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80089AA0);

void func_8008ACD4(void)
{
}

void func_8008ACDC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    u8 *m;
    u8 *u;
    u8 *t;
    u8 *t2;
    s32 r;
    s32 n;
    s32 w;
    s32 w2;
    s32 j;
    s32 jj;
    s16 i;
    s16 want;
    s16 e;
    s32 cur;
    s32 delta;

    if (p[0x9A] != 0xE)
    {
        if (p[0x9A] != 0x17)
        {
            if ((*(u16 *)(p + 0xA2) & 0x100) != 0)
            {
                func_8008D94C(p, b, c, d);
                return;
            }
            p[0x9A] = 0xE;
            *(u16 *)(p + 0xA2) &= 0xFEFF;
            *(u16 *)(c + 0x14) &= 0xF7FF;
            p[0x9B] = 0;
            *(s16 *)(p + 0xA4) = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
        }
        *(s16 *)(p + 0x96) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) &= 0xFF7F;
    r = *(s16 *)(d + 0x64);
    if (r < 0 || (*(u16 *)(p + 0x10C) & 1) != 0)
    {
        func_8008CAA0(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if ((*(s32 *)(d + 0x14) & 0x20000) != 0)
    {
        func_80091B5C(p, b, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        func_8008C468(p, b, c, d);
        return;
    }
    if ((*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        func_80090200(p, b, c, d);
        return;
    }
    if ((s16)func_800A1C58(d) != 0)
    {
        if (func_8008D1D0(p, b, c, d) != 0)
        {
            return;
        }
    }
    if ((*(u16 *)(p + 0xA2) & 0x80) != 0)
    {
        func_8008D7D0(p, b, c, d);
        return;
    }
    if ((*(u16 *)(q + 0x2) & 0x4) != 0)
    {
        goto tail;
    }
    if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
    {
        if ((*(u16 *)0x80013714 & 1) == 0 && (g->unk_08 & 0x80) != 0)
        {
            *(s16 *)(d + 0x8A) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = NULL;
            *(s32 *)(p + 0xC8) = 0;
            *(s32 *)(p + 0x104) = 0;
            return;
        }
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)0x80013714 & 1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        if ((*(u16 *)(p + 0xA2) & 0x200) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFDFF;
        }
        m = func_8009F868();
        if (m == NULL)
        {
            return;
        }
        want = (m[0x1] & 7) << 9;
        *(s16 *)(d + 0x2A) &= 0xFFF;
        if (*(s16 *)(d + 0x2A) != want)
        {
            if ((*(u16 *)(d + 0x2A) & 0x800) != 0)
            {
                *(s16 *)(d + 0x2A) |= 0xF800;
            }
            else
            {
                *(s16 *)(d + 0x2A) &= 0x7FF;
            }
            do
            {
                want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                cur = *(u16 *)(d + 0x2A);
                r = (u16)want;
                r <<= 16;
                delta = *(s16 *)(d + 0x2A) - (r >> 16);
            } while (0);
            if (delta < 0)
            {
                delta = -delta;
            }
            if (delta > 0x800)
            {
                *(s16 *)(d + 0x2A) = (want & ~0xFFF) | (cur & 0xFFF);
            }
            if (*(s16 *)(d + 0x2A) > (r >> 16))
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) - 0x200;
            }
            else
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            func_8009F988();
            goto tail;
        }
        D_800E3544 = m[0x1] & 0xF8;
        switch (D_800E3544)
        {
        case 0x8:
        case 0xE0:
        case 0xE8:
        case 0xF0:
        case 0xF8:
            func_8008B9FC(p, b, c, d);
            return;
        case 0x10:
            func_8008C7B4(p, b, c, d);
            return;
        case 0x18:
            func_8008C8BC(p, b, c, d);
            return;
        case 0x20:
            func_8008C5C4(p, b, c, d);
            return;
        case 0x30:
            func_8008C514(p, b, c, d);
            return;
        case 0x48:
            if ((s16)func_80095538(p, m[0x0] & 0x1F, m[0x2] & 0x1F) < 0)
            {
                func_8009F988();
            }
            else
            {
                func_8009FAAC();
            }
            return;
        case 0x50:
            i = (m[0x0] & 0x60) >> 5;
            if (func_80098920(*(u8 **)(p + 0xAC + i * 4), func_8009FADC(m[0x0] & 0x1F), 0x15, 0) < 0)
            {
                func_8009F988();
            }
            return;
        case 0x58:
            func_80098CF8(p, b, c, func_8009FADC(m[0x0] & 0x1F));
            return;
        case 0x68:
            func_80094270(p, b, c, func_8009FADC(m[0x0] & 0x1F), j = (m[0x0] & 0x60) >> 5);
            return;
        case 0x88:
            jj = (m[0x0] & 0x60) >> 5;
            u = *(u8 **)(p + 0xD0 + jj * 4);
            func_80094270(p, b, c, u, jj);
            return;
        case 0x70:
            func_80094548((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x78:
            func_8009458C((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x80:
            func_800945C4((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x90:
            func_8009456C((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x98:
            u = func_8009FADC(m[0x0] & 0x1F);
            if ((u[0x3] & 0x20) != 0)
            {
                func_800956B8(p, b, c, u);
            }
            else
            {
                func_80095854((UnkDungeonActor *)p, b, c, u);
            }
            return;
        case 0xA0:
            func_8002534C(p, b, c, d);
            return;
        case 0xC8:
            i = m[0x0] & 0x7F;
            *(s16 *)(p + 0x96) = i;
            func_8008D368(p, b, c, D_800DD148, 2);
            return;
        case 0xD8:
            D_800DD830[m[0x0] & 0x7F]();
            return;
        }
        return;
    }
    D_800E3544 = 0;
    if ((*(u16 *)0x80013714 & 4) != 0)
    {
        if (D_800E3CD0 != 0)
        {
            return;
        }
        D_800E3CD0 = 1;
        func_80040AA0(3);
        return;
    }
    if ((s16)func_80094EA4() != 0)
    {
        *(s16 *)(p + 0xA4) = 0;
    }
    if ((*(u16 *)(p + 0xA2) & 0x200) != 0 && (s16)func_800A2C34(d) == 0 && (*(u16 *)(q + 0x2) & 0x4) == 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFDFF;
        if ((s16)func_800A4474(c[0x24], c[0x25]) != 0)
        {
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
    }
    w = g->unk_08;
    if ((w & 0x80) != 0)
    {
        func_8008CF6C((UnkDungeonActor *)p, b, c, D_80050CAC);
        return;
    }
    if ((w & 0x10) == 0 && (w & 3) != 0)
    {
        if (func_8008D024(p, b, c, (((u32)w >> 1) ^ 1) & 1, 0) != 0)
        {
            return;
        }
    }
    n = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
    if ((s16)n != 0xFFF)
    {
        *(s16 *)(d + 0x2A) = n;
        if ((g->unk_08 & 0x10) == 0 && (*(u16 *)(p + 0xA2) & 0x400) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((*(s32 *)(d + 0x1C) & 0x400) != 0)
            {
                *(u16 *)(d + 0x2A) += func_800A6D30() & 0xE00;
            }
            func_8008B9FC(p, b, c, d);
            if (p[0x9A] != 0xE)
            {
                return;
            }
        }
        else
        {
            *(s16 *)(p + 0x96) = 0;
        }
    }
    w2 = g->unk_08;
    if ((w2 & 0x30) == 0x30)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x40) != 0)
    {
        if ((w2 & 0x40) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFBF;
        }
    }
    else if ((w2 & 0x40) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        if ((g->unk_08 & 0x20) == 0)
        {
            if ((s16)func_8008C8BC(p, b, c, d) == 0)
            {
                return;
            }
            goto tail2;
        }
        else
        {
            if (func_8008C5C4(p, b, c, d) != 0)
            {
                return;
            }
        }
    }
    if ((*(u16 *)0x80013714 & 9) != 0)
    {
        *(s16 *)(p + 0xA4) = 0;
    }
    if (p[0x9A] == 0xE)
    {
        e = *(u16 *)(p + 0xA4) + 1;
        *(s16 *)(p + 0xA4) = e;
        if (e >= 0x3D)
        {
            *(s16 *)(p + 0xA4) = 0x3C;
            func_8008CCB0(p, b, c, d);
            return;
        }
    }
tail:
    if ((*(u16 *)(p + 0xA2) & 1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        t = D_800DD058;
        *(u8 **)(c + 0x2C) = t;
        func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    }
tail2:
    if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
    {
        t2 = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    }
}

/* func_80097C78's longer twin: the same +0x64 fork, the same
   0x80013714 / D_80083160 gate and the same +0x9A tail, with a mover reset
   and a func_800A2B04 placement in front, an extra D_80083462 term in the
   gate, 0xD for 0x3A as the state the tail refuses, and func_8008CAA0 /
   func_80090200 for func_80097898 / func_80097C50. 99 words, 0 differing,
   first compile from that template.

   The negative-counter arm is an `||`: the `bltz` branches INTO the
   func_8008CAA0 call and the `a + 0x10C` bit test branches PAST it, so the
   two tests share one body.

   0x80013714 is a CONSTANT ADDRESS here, as func_800969B8 and func_80097C78
   both record -- the declared symbol is in include/structs.h but the raw
   address is the spelling that matches. */
void func_8008B870(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    if (*(s16 *)(d + 0x64) < 0 || (*(u16 *)(a + 0x10C) & 1) != 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_8008CAA0(a, b, c, d);
        return;
    }
    if (*(s16 *)(d + 0x64) > 0)
    {
        func_8008CBA0(a, b, c, d);
    }
    if ((*(u16 *)0x80013714 & 1) == 0 && (D_80083462 & 4) == 0 && (*(s32 *)(g + 0x8) & 0x80) != 0)
    {
        *(s16 *)(d + 0x8A) = 2;
        D_800E4940 = 2;
        func_8008CF6C((UnkDungeonActor *)a, b, c, D_8004F5F4);
        D_80082EB0 = NULL;
        *(s32 *)(a + 0xC8) = 0;
        *(s32 *)(a + 0x104) = 0;
        return;
    }
    if (a[0x9A] != 0xD && (*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_80090200(a, b, c, d);
    }
}

/* The entry handler of the same dispatcher func_8008C13C (:1858) serves, and
   that function is its line-for-line template: the same func_8009ABA0 probe,
   the same D_8006CCD8/D_8006CCE8 eight-way step, the same f/h/q
   materialisations of D_80083460 and the same r>0 / r<0 / r==0 fork, with a
   longer D_800E3544 tail. 464 words, 0 differing.

   D_80013714 IS THE DECLARED SYMBOL HERE, not the raw address the unit's
   other sixteen sites use. Both spellings are 0 differing -- the linked
   bytes are identical -- but the raw address emits no relocation and costs
   57 aligned differences: the symbol's `high` needs a register, which
   lengthens `d`'s live range past `p`'s and flips the one allocno_compare
   that swaps $s3 and $s4 over the whole function. The numbers out of .lreg
   are p 21 refs / 504 insns against d 26 refs / 625 insns, i.e. priorities
   0.16667 against 0.16640. An `__asm__("" ::"r"(d))` reference buys the same
   flip, which is what func_8008C13C's own DEVICE does; the symbol makes it
   unnecessary here.

   THE `*(s16 *)(q + 0x4)` HALVING IS DUPLICATED INTO THE 0xF0, 0xF8 AND 0xE8
   ARMS. Cross-jumping merges all three back onto the last one, which is what
   .L8008C048 is. Written once as a shared tail after the chain, the 0xF0
   arm's whole swap block is cross-jumped away too and the function comes out
   449 words, fifteen short.

   THE INDEX MUST BE INLINE IN THE func_80048A44 ARGUMENT at the three
   unconditional table stores, and each of those stores needs its OWN pointer
   local: one local shared between two stores splits that symbol's `%hi` and
   `%lo` across two registers, and a named index local puts the table base in
   $v1 where the target has it in $a1 (27 aligned differences). The two
   subtracting arms need their own index locals k2/k3 for the same reason --
   the function-scope `k` is 465 words with a nop in the 0xE8 arm, and
   writing that index inline as well is 467.

   Relocations: the two `*(s16 *)((u8 *)&D_80083460 + 0x4) = 8` stores
   relocate against D_80083460 with +4 in the displacement where splat names
   D_80083464, exactly as func_8008E264, func_8008FF18 and func_800971A8
   already do. seqcheck's four mismatches are all that pair; raw == relocdump
   at 104 == 104. */
void func_8008B9FC(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *f;
    u8 *h;
    u8 *q;
    u8 *t;
    u8 *t2;
    u8 *t3;
    u8 *u;
    u8 *u2;
    u8 *cur;
    s32 r;
    s32 k;
    s32 k2;
    s32 k3;
    s32 v;
    s32 w;

    g = (u8 *)&D_80083160;
    r = func_8009ABA0(*(s16 *)(d + 0x2A), b, c, *(s16 *)(d + 0x88), 0x20);
    if ((s16)r > 0)
    {
        p[0x9B] = 0;
        func_8009A3D0(c[0x24], c[0x25], 0x300);
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        c[0x24] += *(u8 *)&D_8006CCD8[k];
        c[0x25] += *(u8 *)&D_8006CCE8[k];
        func_8009A21C(c[0x24], c[0x25], 0x300);
        f = (u8 *)&D_80083460;
        *(u16 *)(f + 0x2) |= 0x8;
        *(s32 *)(p + 0x8C) = 0;
        if ((s16)r != 4)
        {
            if ((D_80013714 & 2) != 0 ||
                ((*(s32 *)(g + 0x8) & 0x20) != 0 && (*(u16 *)(p + 0xA2) & 0x100) == 0 && func_800A5C70() != 0))
            {
                *(u16 *)(f + 0x2) |= 0x80;
                cur = *(u8 **)(c + 0x2C);
                t = D_800DCFC0;
                if (cur != t)
                {
                    *(u8 **)(c + 0x2C) = t;
                    v = ((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                    func_80048A44(c, t[v], 0, 1);
                }
            }
        }
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x80) != 0 || (s16)r == 1)
        {
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            if ((*(u16 *)(h + 0x2) & 0x80) == 0 && (D_800E3544 == 8 || D_800E3544 == 0))
            {
                cur = *(u8 **)(c + 0x2C);
                t = D_800DCFB8;
                if (cur != t)
                {
                    *(u8 **)(c + 0x2C) = t;
                    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                    func_80048A44(c, t[v], 0, 1);
                }
            }
            p[0x9A] = 0xF;
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 8;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0xC;
            if ((s16)r < 4)
            {
                *(u16 *)(p + 0xA2) |= 1;
            }
            else if ((s16)r == 2 || (s16)r == 4)
            {
                p[0x9B] = 8;
            }
            func_80094ED4(p, b, c, d);
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 8;
            if (*(u8 **)(c + 0x2C) == D_800DCFB8)
            {
                *(u16 *)(c + 0x14) |= 0x6000;
                func_8008E264(p, b, c, d);
            }
            else
            {
                u = D_800DD030;
                *(u8 **)(c + 0x2C) = u;
                func_80048A44(c, u[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            p[0x9A] = 0x10;
            *(s32 *)(d + 0x1C) |= 0x40000000;
            func_8009F644(d, 8, 0, 0);
            return;
        }
        func_8009F644(d, 8, 0, 0);
    }
    else if ((s16)r < 0)
    {
        if ((D_80013714 & 1) != 0 || (s16)(++*(u16 *)(p + 0x96)) >= 0x15)
        {
            func_8008CD4C(p, b, c, d, (s16)r);
            if (p[0x9B] >= 0x10)
            {
                *(s32 *)(d + 0x1C) |= 0x40000000;
            }
        }
        return;
    }
    else
    {
        u2 = D_800DCFB0;
        *(u8 **)(c + 0x2C) = u2;
        w = ((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, u2[w], 0, 1);
        *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
        return;
    }
    *(s32 *)(d + 0x1C) |= 0x40000000;
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) |= 0x812;
    func_800A67F4();
    func_80094ED4(p, b, c, d);
    if ((D_80013714 & 8) != 0)
    {
        if (D_800E3544 == 0xF0)
        {
            cur = *(u8 **)(c + 0x2C);
            t = D_800DCFC0;
            if (cur != t)
            {
                *(u8 **)(c + 0x2C) = t;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, t[v], 0, 1);
            }
            *(s16 *)(q + 0x4) = (s16)(*(u16 *)(q + 0x4)) >> 1;
        }
        else if (D_800E3544 == 0xF8)
        {
            *(u16 *)(p + 0xA2) |= 0x100;
            *(u16 *)(q + 0xA) += 1;
            cur = *(u8 **)(c + 0x2C);
            t = D_800DCFC0;
            if (cur != t)
            {
                *(u8 **)(c + 0x2C) = t;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, t[v], 0, 1);
            }
            *(s16 *)(q + 0x4) = (s16)(*(u16 *)(q + 0x4)) >> 1;
        }
        else if (D_800E3544 == 0xE8)
        {
            t2 = D_800DCFE8;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 1, 1);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            k2 = (*(u16 *)(d + 0x2A) >> 9) & 7;
            c[0x24] -= D_8006CCD8[k2] * 2;
            c[0x25] -= D_8006CCE8[k2] * 2;
            func_8009A21C(c[0x24], c[0x25], 0x300);
            *(s16 *)(q + 0x4) = (s16)(*(u16 *)(q + 0x4)) >> 1;
        }
        else if (D_800E3544 == 0xE0)
        {
            t3 = D_800DD0D0;
            *(u8 **)(c + 0x2C) = t3;
            func_80048A44(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 1, 1);
            func_8009A3D0(c[0x24], c[0x25], 0x300);
            k3 = (*(u16 *)(d + 0x2A) >> 9) & 7;
            c[0x24] -= *(u8 *)&D_8006CCD8[k3];
            c[0x25] -= *(u8 *)&D_8006CCE8[k3];
            func_8009A21C(c[0x24], c[0x25], 0x300);
        }
    }
}

void func_8008C13C(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *f;
    u8 *h;
    u8 *q;
    u8 *t;
    u8 *u;
    u8 *cur;
    s32 r;
    s32 k;
    s32 v;
    s32 w;

    g = (u8 *)&D_80083160;
    /* DEVICE: worth 25 differing.  The else arm must run its store BEFORE the
       index expression -- that is the target's instruction order and the only
       order sched2 can produce -- but that order shortens `c`'s
       reg_live_length by 3 (232 -> 229) and flips the one allocno_compare
       that decides the whole function's callee-saved rotation:
       c = 16 refs / 229 -> 2794 against f = 5 refs / 36 -> 2777, so `c` takes
       $s0 and `f` $s1 where the target has them the other way round.  This
       reference costs no instruction and no word; it adds exactly the one
       insn to `c`'s live range (229 -> 231, pri 2770) that puts `f` back in
       front.  Measured and rejected as legal spellings: both statement orders
       in the arm, 6 adjacent-statement swaps elsewhere in the function
       (p+0x98/c+0x14, the two D_80083460 faces, the d+0x1C tail pair, the
       cur/t pair, dropping the `w` temporary), and `h = f`; none of them
       moves c's length by the two insns needed without also moving a word. */
    __asm__("" ::"r"(d));
    r = func_8009ABA0(*(s16 *)(d + 0x2A), b, c, *(s16 *)(d + 0x88), 0x20);
    if ((s16)r > 0)
    {
        func_8009A3D0(c[0x24], c[0x25], 0x300);
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        c[0x24] += *(u8 *)&D_8006CCD8[k];
        c[0x25] += *(u8 *)&D_8006CCE8[k];
        func_8009A21C(c[0x24], c[0x25], 0x300);
        f = (u8 *)&D_80083460;
        *(u16 *)(f + 0x2) |= 0x8;
        *(s32 *)(p + 0x8C) = 0;
        if ((s16)r != 4)
        {
            if ((*(u16 *)0x80013714 & 2) != 0 || ((*(s32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0))
            {
                *(u16 *)(f + 0x2) |= 0x80;
            }
        }
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x80) != 0 || (s16)r == 1)
        {
            cur = *(u8 **)(c + 0x2C);
            t = D_800DCFD8;
            if (cur != t)
            {
                *(u8 **)(c + 0x2C) = t;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, t[v], 0, 1);
            }
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            *(s16 *)(h + 0x4) = 8;
            p[0x9A] = 0x1D;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0xC;
            *(u16 *)(c + 0x14) |= 0x4000;
            func_80094ED4(p, b, c, d);
            if ((s16)r == 2)
            {
                p[0x9B] = 8;
            }
            *(s16 *)(h + 0x4) = 8;
            p[0x9A] = 0x1E;
            *(s32 *)(d + 0x1C) |= 0x40000000;
            func_8009F644(d, 8, 0, 0);
            return;
        }
        func_8009F644(d, 8, 0, 0);
    }
    else
    {
        u = D_800DD0B8;
        *(u8 **)(c + 0x2C) = u;
        w = ((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, u[w], 0, 1);
        *(s32 *)(p + 0x8C) = (s32)func_8008EAC8;
        return;
    }
    func_800A67F4();
    func_80094ED4(p, b, c, d);
    *(s32 *)(d + 0x1C) |= 0x40000000;
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) |= 0x812;
}

/* State 0x16, with the eight-way facing table picked by the view's 0x100000
   flag and installed only when it is not already the renderable's.

   THE TEST AND ITS BODY ARE DUPLICATED INTO BOTH ARMS; cross-jumping merges
   them back from the `beq` onwards, which is why the +0x2C load appears
   twice. Picking `t` in the arms and testing once afterwards leaves that load
   in the join block: 43 words, 12 differing. AND THE FIELD READ MUST BE ITS
   OWN STATEMENT AHEAD OF THE TABLE ADDRESS -- with `t = SYM;` first the else
   arm schedules its `lui` before the `lw` and it is 2 differing.
   43 words, 0 differing. */
void func_8008C468(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    u8 *cur;
    s32 v;

    if ((*(u16 *)(p + 0x98) & 0xC) == 0)
    {
        p[0x9A] = 0x16;
        p[0x9B] = 0;
        *(s32 *)(p + 0x8C) = 0;
        *(u16 *)(p + 0xA2) &= ~1;
        if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
        {
            cur = *(u8 **)(c + 0x2C);
            t = D_800DD0E0;
            if (cur != t)
            {
                *(u8 **)(c + 0x2C) = t;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, t[v], 0, 1);
            }
        }
        else
        {
            cur = *(u8 **)(c + 0x2C);
            t = D_800DD050;
            if (cur != t)
            {
                *(u8 **)(c + 0x2C) = t;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, t[v], 0, 1);
            }
        }
    }
}

/* State 0x1A. With the view's 0x100000 flag set only the renderable's 0x4000
   bit is raised; otherwise the D_800DD030 facing table is installed the
   func_8008CCB0 way. 44 words, 0 differing. */
void func_8008C514(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;

    p[0x9A] = 0x1A;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
    {
        *(u16 *)(c + 0x14) |= 0x4000;
    }
    else
    {
        t = D_800DD030;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
    }
    func_800A56E0(0x50A);
    *(s16 *)(p + 0x96) = 0;
    *(u16 *)(p + 0xA2) &= ~0x10;
}

/* The 0x1B state: ask func_8009B5AC what stands on the tile ahead, park it
   in the actor's +0x124 and fork on what came back. A nonzero POSITIVE
   result is an error code and returns -1; a negative one is a record
   pointer, and whether its +0x14 carries 0x80000 decides between remembering
   it at +0x11C and claiming it -- raising 0x80000 on its +0x1C, 0x100000 on
   the mover's, writing the mover into its +0x60 and firing the 0x20 effect.
   The 0x39/2 kinds under the engine's 0x80012090 gate and D_80083170's 0x40
   bit refuse the claim instead and hand the actor back to func_8008ACDC.

   THE `r == NULL` ARM IS NOT AN EARLY RETURN. Written
   `if (r == NULL) { p->unk_8C = func_8008ACDC; return 0; }` right after the
   call it is laid out INLINE and the whole function shifts: 124 words,
   86 differing, with `bnez` for the target's `beqz`. Wrapping the body in
   `if (r != NULL)` and leaving that arm as the function's last two
   statements puts it out of line at 0x8008C788, which is where the target
   has it. The `(s32)r > 0` early return next door IS an early return and
   stays one -- its `bgtz` carries the -1 in its delay slot.

   0x80012090 IS THE CONSTANT ADDRESS, as func_80092DFC and func_80094208
   record: the load reuses its own destination as the `lui` temp.

   124 words, 0 differing. */
s32 func_8008C5C4(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *r;
    u8 *t;
    s32 k;
    s32 v;

    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    r = (u8 *)func_8009B5AC(d, (s16)(c[0x24] + (u16)D_8006CCD8[k]),
                            (s16)(c[0x25] + (u16)D_8006CCE8[k]));
    *(u8 **)(a + 0x124) = r;
    if (r != NULL)
    {
        if ((s32)r > 0)
        {
            return -1;
        }
        if ((*(s32 *)(r + 0x14) & 0x80000) == 0)
        {
            *(u8 **)(a + 0x11C) = r;
            *(s32 *)(a + 0x124) = 0;
        }
        else
        {
            if (*(s32 *)0x80012090 == 1)
            {
                if (r[0x13] == 0x39 || r[0x13] == 2)
                {
                    if ((D_80083170[0] & 0x40) != 0)
                    {
                        func_800A56E0(0x506);
                        func_80099844(*(u8 **)(a + 0x124), D_800E0495);
                    }
                    *(s32 *)D_80081484 = 0;
                    *(s32 *)(a + 0x124) = 0;
                    *(s32 *)(a + 0x8C) = (s32)func_8008ACDC;
                    return 0;
                }
            }
            *(s32 *)(*(u8 **)(a + 0x124) + 0x1C) |= 0x80000;
            *(s32 *)(d + 0x1C) |= 0x100000;
            *(u8 **)(*(u8 **)(a + 0x124) + 0x60) = d;
            func_8009F644(d, 0x20, 0, 0);
            func_800A56E0(0x511);
        }
        a[0x9A] = 0x1B;
        a[0x9B] = 0;
        *(s32 *)(a + 0x8C) = 0;
        t = D_800DCFC8;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
        return 1;
    }
    *(s32 *)(a + 0x8C) = (s32)func_8008ACDC;
    return 0;
}

/* Installer: state 0x1C. TWO SPELLINGS, both already measured on
   func_800969B8 next door and both worth a word here. `*(u16 *)0x80013714`
   must be the CONSTANT ADDRESS, not the structs.h symbol: as the symbol
   sched2 hoists the `lui` into the second prologue slot and the three stores
   shift, 3 differing. And func_800A5C70 takes NO arguments, so passing `d`
   emits an extra `addu $a0, $s0, $zero` that reorg puts in the `jal`'s delay
   slot where the target has a `nop`, 1 differing. 66 words, 0 differing. */
void func_8008C7B4(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *f;
    u8 *t;
    s32 v;

    p[0x9A] = 0x1C;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    g = (u8 *)&D_80083160;
    if ((*(u16 *)0x80013714 & 2) != 0 || ((*(s32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0))
    {
        f = (u8 *)&D_80083460;
        *(u16 *)(f + 0x2) |= 0x80;
    }
    func_8009F644(d, 0x10, 0, 0);
    if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
    {
        t = D_800DD0B8;
    }
    else
    {
        t = D_800DCFB0;
    }
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
}

/* The 0x11 state: probe the tile ahead with func_8009A540, and on a hit ask
   func_8009B25C what is standing there and park it in the record's +0x60.
   A hit that the engine's 0x80013186 flag protects (and that is not already
   0x410-marked) leaves +0x60 clear and returns early; otherwise the turn is
   taken through func_8009C93C and the actor drops into state 0x11 with the
   func_800973F0 tail -- D_80083460's +0xC owner, the +0x2 flag word, the
   +0x5C message close, the 0x18 effect and the func_800A5C70 gate.

   0x80013714 AND 0x80013186 SHARE ONE `lui`, so they are displacements off
   `h = (u8 *)0x80010000` rather than the two declared symbols -- the
   one-base-pointer lever, and the same constant-address fact func_8008E0C4
   and func_80094208 record for 0x80013714 alone.

   121 words, 0 differing on the first compile, with the tail written in
   func_800973F0's order (csrc/ovl_dungeon.c:3164) and the direction index
   in func_8008F6EC's `(*(u16 *)(d + 0x2A) >> 9) & 7` / `(u16)D_8006CCD8[k]`
   spelling. */
s16 func_8008C8BC(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *q;
    s32 k;

    k = (*(u16 *)(d + 0x2A) >> 9) & 7;
    if (func_8009A540(k, c[0x24], c[0x25], *(u16 *)(d + 0x88) - 0x20) != 0)
    {
        q = func_8009B25C(d, c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k],
                          *(s16 *)(d + 0x88));
        *(u8 **)(d + 0x60) = q;
        h = (u8 *)0x80010000;
        if ((*(u16 *)(h + 0x3714) & 1) == 0 && q != NULL && (*(u32 *)(q + 0x14) & 0x4000) != 0)
        {
            if ((*(u8 *)(h + 0x3186) & 1) != 0 && (*(u32 *)(d + 0x1C) & 0x410) == 0)
            {
                *(s32 *)(d + 0x60) = 0;
                return 0;
            }
        }
        func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    }
    else
    {
        *(s32 *)(d + 0x60) = 0;
    }
    a[0x9A] = 0x11;
    g = (u8 *)&D_80083460;
    a[0x9B] = 0;
    *(s32 *)(a + 0x8C) = 0;
    *(u8 **)(g + 0xC) = d;
    *(s16 *)(a + 0xA6) = 0;
    *(u16 *)(a + 0x98) = (*(u16 *)(a + 0x98) | 0x2000) & 0xEFFF;
    *(u16 *)(g + 0x2) |= 0x400;
    func_80099F70(*(u32 *)(d + 0x5C));
    func_8009F644(d, 0x18, 0, 0);
    if (func_800A5C70() != 0)
    {
        *(u16 *)(g + 0x2) |= 0x80;
    }
    *(s16 *)(a + 0x96) = 6;
    a[0x102] = 0;
    return 0;
}

/* Installer: state 7, the carrier's +0x10C bit 0 dropped, the eight-way
   facing picked out of whichever of D_800DCFE0/D_800DCFE8 the view's
   0x20000000 flag selects, and on an odd draw three func_80042B68 pulses.
   The 0x20000000 flag is re-read for the second test. 64 words,
   0 differing. */
void func_8008CAA0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;

    *(u16 *)(p + 0x10C) &= 0xFFFE;
    if (*(s32 *)(d + 0x1C) & 0x20000000)
    {
        t = D_800DCFE0;
    }
    else
    {
        t = D_800DCFE8;
    }
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
    p[0x9A] = 7;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_80094E34();
    if ((func_800A6D30() & 1) == 0)
    {
        func_80042B68((s8 *)d, 1);
        func_80042B68((s8 *)d, 4);
        func_80042B68((s8 *)d, 2);
    }
    if (*(s32 *)(d + 0x1C) & 0x20000000)
    {
        *(s16 *)(p + 0x96) = D_800E3E41;
    }
}

/* FOUR PARAMETERS, only the LAST read -- the record arrives in $a3. The
   `andi 0xFF` after the `sb` is cse reusing the stored byte for the
   unsigned compare, and +0x64 is read as a BYTE and cleared as a HALFWORD.
   13 words, 0 differing. */
void func_8008CBA0(void *a, void *b, void *c, u8 *p)
{
    p[0x28] += p[0x64];
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
    *(u16 *)(p + 0x64) = 0;
}

/* Installer: state 8, the eight-way facing picked out of D_800DD120 exactly
   as func_8008CCB0 and func_80090200 do it. 55 words, 0 differing. */
void func_8008CBD4(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    u8 *g;

    t = D_800DD120;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
    p[0x9A] = 8;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_80094E34();
    *(s32 *)(d + 0x1C) &= ~0x1638;
    *(s32 *)(b + 0x10) = 0;
    *(s32 *)(b + 0xC) = 0;
    func_800A2B04((UnkVec *)b, c[0x24], c[0x25]);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* THE WHOLE INDEX IN ONE EXPRESSION. Split into `v = D_80083228 + d[0x2A];`
   and `v = ((v + 0x100) >> 9) & 7;` the table base lands in $v1 and the two
   `lh` results swap registers: 8 differing at the right length. Folded into
   one expression the base takes $a0 and the sum's operands come out in the
   target's order. 39 words, 0 differing. */
void func_8008CCB0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    u8 *g;

    if (p[0x9A] != 0x17)
    {
        t = D_800DD000;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x2) |= 0x100;
        p[0x9A] = 0x17;
        p[0x9B] = 0;
        *(s32 *)(p + 0x8C) = 0;
    }
}

/* State 0x18: unless the fifth argument is -2, look at the tile one step
   ahead in the actor's facing, and if something is standing there and
   func_8009ADB8 agrees, park it in +0x124, step the view onto that tile and
   hand over to func_80094ED4. Either way the slot's +0x2C table is re-sent
   to func_80048A44. 136 words, 0 differing.

   FIVE PARAMETERS; the fifth arrives on the stack at 0x50($sp) and is a
   PROMOTED s16 (`lw` then `sll 16`/`sra 16`). As `s32` with a `(s16)` cast at
   the test it is 134 words.

   THE FIRST STORE MUST BE AN ARRAY REFERENCE, NOT A CAST. `((u8 **)c)[0xB]`
   sets MEM_IN_STRUCT_P (expr.c's INDIRECT_REF arm takes a top-level
   PLUS_EXPR; a cast puts a NOP_EXPR on top and loses it), so the store is
   provably clear of the incoming argument slot and the fifth parameter's
   `lw` moves down to its use, where it is scheduled after the store.
   Written `*(u8 **)(c + 0x2C)` the load stays in the prologue, sched2 ranks
   it above the D_800DD018 `lui`, and it is 9 differing. The sibling
   `*(s32 *)(p + 0x8C) = 0;` is inert either way (measured both).

   `dx` AND `x` ARE INTERLEAVED WITH `dy` AND `y`. With both bases computed
   first, x's live range starts five insns later, its global-alloc priority
   beats the parameter `c`'s and the two swap $s2 and $s3 -- 36 differing. */
void func_8008CD4C(u8 *p, UnkVec *b, u8 *c, u8 *d, s16 e)
{
    u8 *q;
    u8 *g;
    s16 *dx;
    s16 *dy;
    s32 k;
    s32 x;
    s32 y;

    ((u8 **)c)[0xB] = D_800DD018;
    p[0x9A] = 0x18;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (e != -2)
    {
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        dx = &D_8006CCD8[k];
        x = c[0x24] + (u16)*dx;
        dy = &D_8006CCE8[k];
        y = c[0x25] + (u16)*dy;
        q = func_8009B25C(d, x, y, *(s16 *)(d + 0x88));
        if (q != NULL)
        {
            if ((s16)func_8009ADB8(d, q, (s16)x, (s16)y, *(s16 *)(d + 0x88)) != 0)
            {
                *(u8 **)(p + 0x124) = q;
                func_8009A3D0(c[0x24], c[0x25], 0x300);
                c[0x24] += *(u8 *)dx;
                c[0x25] += *(u8 *)dy;
                func_8009A21C(c[0x24], c[0x25], 0x300);
                *(u32 *)(d + 0x1C) |= 0x40000000;
                g = (u8 *)&D_80083460;
                *(s16 *)(g + 0x4) = 0x20;
                *(u16 *)(g + 0x2) |= 0x8;
                p[0x9B] = 0x10;
                *(u8 **)(c + 0x2C) = D_800DCFB8;
                *(s16 *)(p + 0x96) = 4;
                func_8009F644(d, 8, 0, 0);
                func_800A56E0(0x50B);
                func_80094ED4(p, b, c, d);
            }
        }
    }
    func_80048A44(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
}

/* THE RECORD HAS TO ARRIVE AS `UnkDungeonActor *`: through a `u8 *` cast the
   +0x8C store may alias D_800E296C, so sched1 cannot lift that global's `lw`
   and the `lui %hi(D_80082EB8)` above it -- 46 words, 3 differing, in every
   statement order measured. MEM_IN_STRUCT_P on the COMPONENT_REF is what
   frees the load, and a single cast at that one store is enough. Unlike
   func_800969B8 next door, 0x80013714 is wanted here as structs.h's SYMBOL.
   46 words, 0 differing. */
void func_8008CF6C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    if ((D_80013714 & 1) != 0)
    {
        return;
    }
    if ((D_800E296C & 0x200000) != 0)
    {
        if ((D_80083170[0] & 0x80) != 0)
        {
            func_800A56E0(0x506);
        }
        return;
    }
    p->unk_9A = 0x19;
    func_800A56E0(0x514);
    D_800E4938 = d;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    D_80082EB8 = 0;
    D_800E296C |= 0x2000;
}

/* State 0x27: adopt the carrier parked at +0xAC + 4*d, put up the four
   func_800B1768 panels on the first entry, and re-skin through
   func_8008C7B4's D_800DD0B8/D_800DCFB0 pair.

   `func_800B0F50(q)` TAKES THE CARRIER, and that is the whole residue of the
   first six compiles. Called with no arguments the function is still 107
   words but `q` and the +0x1C flags swap registers -- $v1/$a0 against the
   target's $a0/$v1, 6 differing -- because nothing then prefers $a0 for `q`.
   The call emits no argument move either way, so the only witness that the
   argument exists is those six words; five other spellings (dropping the
   flags local, three declaration orders, `u32` flags) all stay at 6.
   `func_800B0F50(p)` is 108 / 89.

   The three failures are ONE nested `if`, not three early returns: written
   as `if (q == NULL) return 0;` guards the second one gets its own `j` to an
   inline `return 0` and the function is 104 / 32. The +0x2A read goes
   through a SECOND local holding the same parameter, func_800CA93C's shape;
   with one variable it is 104 / 98. 107 words, 0 differing. */
s32 func_8008D024(u8 *p, void *b, u8 *c, s16 d, s16 e)
{
    u8 *q;
    u8 *r;
    u8 *t;
    s32 f;
    s32 v;

    r = p;
    q = *(u8 **)(p + d * 4 + 0xAC);
    if (q != NULL)
    {
        f = *(s32 *)(q + 0x1C);
        if ((f & 0x20000) != 0)
        {
            if ((f & 0x80000) != 0)
            {
                return 0;
            }
            *(u8 **)(p + 0x104) = q;
            p[0x9A] = 0x27;
            p[0x9B] = 0;
            *(s32 *)(p + 0x8C) = 0;
            func_800B0F50(q);
            if (e == 0)
            {
                func_800B1768(0x21, 0xE0, 0x84, 0, 0, 0);
                func_800B1768(0x22, 0xF2, 0x98, 1, 0, 2);
                func_800B1768(0x24, 0xCE, 0x98, 2, 1, 4);
                func_800B1768(0x23, 0xE0, 0xAC, 3, 1, 6);
                D_800E296C |= 0x2000;
            }
            if ((*(s32 *)(p + 0x1C) & 0x100000) != 0)
            {
                t = D_800DD0B8;
            }
            else
            {
                t = D_800DCFB0;
            }
            *(u8 **)(c + 0x2C) = t;
            v = ((D_80083228 + *(s16 *)(r + 0x2A) + 0x100) >> 9) & 7;
            func_80048A44(c, t[v], 0, 1);
            *(s16 *)(p + 0x120) = e;
            return 1;
        }
    }
    return 0;
}

/* State 9, gated on the D_80083460 record naming THIS view. The second
   parameter is never read by the body: it is FORWARDED into func_800C77D0's
   second argument, which is why the `jal` sets only $a0/$a2/$a3 and leaves
   $a1 as it arrived. Both ASM callers (func_8008ACDC at 0x8008AEA8 and
   func_8008EAC8 at 0x8008ECBC) load $a0..$a3, so the arity is four.
   The facing table at +0x2C and the `((D_80083228 + d[0x2A] + 0x100) >> 9)
   & 7` index are func_8008C514's idiom, local `t` and all. 88 words,
   0 differing on the first compile. */
s32 func_8008D1D0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *t;
    u8 *w;
    s32 v;

    if ((D_800E296C & 0x100000) == 0)
    {
        return 0;
    }
    g = (u8 *)&D_80083460;
    w = *(u8 **)(g + 0xC);
    if (w == NULL)
    {
        return 0;
    }
    if (w != d)
    {
        return 1;
    }
    if (*(s16 *)(g + 0xA) != 0)
    {
        return 1;
    }
    if ((*(u16 *)(g + 0x2) & 8) != 0)
    {
        return 1;
    }
    p[0x9A] = 9;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_800C77D0(p - 0x20, b, 8, 0x300);
    if ((*(s32 *)(d + 0x1C) & 0x100000) == 0)
    {
        t = D_800DD0F8;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 1, 1);
    }
    func_800A56E0(0x201);
    if ((D_80013714 & 2) == 0)
    {
        D_80080AA0 = 1;
        D_80080A88 = 0;
    }
    *(s16 *)(p + 0x96) = 0x78;
    func_800A1D4C(d, 1);
    return 1;
}

/* Reset an actor: a state byte, the byte after it, and the handler slot.
   The store order is the source order -- three independent stores, nothing
   for reorg to reassociate.

   FOUR PARAMETERS, THREE OF THEM UNUSED, and the witness is a CALL SITE:
   func_800BD45C below reaches it with $a1, $a2 and $a3 all loaded. Unused
   arguments cost nothing on this target -- they arrive in $a1..$a3 and are
   never touched -- so this landed at 0 differing with one parameter too,
   and the extra three are the call site's evidence rather than the body's.
   It is a four-argument step of the same shape as UnkDungeonStep; the other
   installers in this unit are very likely the same, but only this one has a
   call site that says so. */
void func_8008D330(UnkDungeonActor *p, void *b, void *c, void *d)
{
    p->unk_9A = 0x21;
    p->unk_9B = 0;
    p->unk_8C = NULL;
}

/* func_8008D330 one state over, with a fourth-argument gate. THE SECOND
   STATE IS AN INCREMENT OF THE FIELD, not the constant 0x2D: spelled
   `p[0x9A] = 0x2D;` the second store is `li $v0, 45` where the target reuses
   the 0x2C still in $v0 with `addiu $v0, $v0, 1`. `p[0x9A]++` costs no load
   -- cse knows the byte from the store four lines up. `u8 *` rather than
   UnkDungeonActor is the call sites': every one of them passes a `u8 *`.
   9 words, 0 differing. */
void func_8008D344(u8 *p, void *b, void *c, void *d)
{
    p[0x9A] = 0x2C;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (d == NULL)
    {
        p[0x9A]++;
    }
}

/* The five-argument installer: state 0x2E, the handler slot cleared, and the
   last two arguments parked at +0xCC and +0x10E. Like func_8008D330 it is
   reached with more arguments than its body reads. 8 words, 0 differing. */
void func_8008D368(u8 *p, void *b, void *c, void *d, s32 e)
{
    p[0x9A] = 0x2E;
    p[0x9B] = 0;
    *(void **)(p + 0x8C) = NULL;
    *(void **)(p + 0xCC) = d;
    *(s16 *)(p + 0x10E) = e;
}

/* Installer for state 0x22. THE REFUSAL IS WRITTEN FIRST, as one `||`. With
   the main body first the body is laid out inline and the function is 59
   words against 58 with 48 differing; the `||` form puts the body BETWEEN
   the two tests, which is the target's layout. 58 words, 0 differing. */
s32 func_8008D388(u8 *p, u8 *b, void *c, u8 *d)
{
    s32 r;

    if (D_80081485[0] != 0 || D_80081471[0] != 0)
    {
        if (D_80081471[0] != 0)
        {
            func_800997FC(D_800E04BA);
            return 0;
        }
        func_80099844(D_800E3D7C, D_800E04E6);
        return 0;
    }
    *(s32 *)(d + 0x1C) |= 0x100000;
    *(s32 *)(p + 0x124) = 0;
    p[0x9A] = 0x22;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_800C77D0(p - 0x20, b, 8, 0x300);
    r = func_80098C80(*(u8 **)(p + 0xBC));
    func_8009F644(d, 0x48, (s16)r, 0);
    return 1;
}

/* The 0x110 node's +0x10 step: ease the mover toward func_8003DE58's
   projection of the slot at +0x90 over the +0xA4 countdown, fade the
   renderable's three colour bytes toward func_800644B8's curve, and when the
   +0xA8 countdown runs out either spawn the follow-on node (+0xAC == 1) or
   just raise the two 0x8000 flags.

   THE "A MOVE HAPPENED" LATCH IS `s16`, AND THAT IS THE WHOLE RESIDUE.
   As `s32` the function is 216/216 with 2 differing: the target widens the
   latch into $v0 before `bnez` (`addu $v0, $s3, $zero`, which reorg then
   puts in the previous branch's delay slot) and an `s32` tests $s3 directly,
   leaving a `nop` there. `u8` is 1 differing, `register s32` and three tail
   if-chain spellings (||-joined, split `else if`, nested) are all 2.
   216 words, 0 differing. */
void func_8008D470(u8 *p, UnkVec *v, UnkSlot *r)
{
    UnkNode *n;
    UnkSlot *s;
    u8 *q;
    s16 flag;
    s32 g;
    s16 c;
    s16 out[4];

    flag = 0;
    switch (*(s16 *)(p + 0xA2))
    {
    case 0:
        *(s16 *)(p + 0xA4) = 6;
        *(s16 *)(p + 0xA2) = *(s16 *)(p + 0xA2) + 1;
    case 1:
        s = *(UnkSlot **)(p + 0x90);
        if (func_8003DE58((u8 *)s->unk_08, s, out, 0) != 0 && *(s32 *)(D_800E3D7C + 0x124) != 0)
        {
            r->unk_1C = 0x1000;
            *(s16 *)(p + 0xA4) = *(s16 *)(p + 0xA4) - 1;
            if (*(s16 *)(p + 0xA4) > 0)
            {
                v->unk_00 += ((*(UnkVec **)(p + 0x8C))->unk_00 + (out[0] << 16) - v->unk_00) / *(s16 *)(p + 0xA4);
                v->unk_04 += ((*(UnkVec **)(p + 0x8C))->unk_04 + (out[1] << 16) - v->unk_04) / *(s16 *)(p + 0xA4);
                v->unk_08 += ((*(UnkVec **)(p + 0x8C))->unk_08 + (out[2] << 16) - (*(s16 *)(p + 0xA4) << 21) - v->unk_08) / *(s16 *)(p + 0xA4);
            }
            else
            {
                *(s16 *)(p + 0xA4) = 0;
                *(s16 *)((u8 *)v + 0x2) = *(u16 *)((u8 *)*(UnkVec **)(p + 0x8C) + 0x2) + out[0];
                *(s16 *)((u8 *)v + 0x6) = *(u16 *)((u8 *)*(UnkVec **)(p + 0x8C) + 0x6) + out[1];
                *(s16 *)((u8 *)v + 0xA) = *(u16 *)((u8 *)*(UnkVec **)(p + 0x8C) + 0xA) + out[2];
            }
            if (*(s16 *)(p + 0xAA) == 0)
            {
                *(s16 *)(p + 0xAA) = 1;
            }
            flag = 1;
        }
        else if (*(s16 *)(p + 0xAA) == 0)
        {
            r->unk_1C = 0;
        }
        else if (*(s16 *)(p + 0xAA) == 1)
        {
            if (*(s16 *)(p + 0xAE) == 0)
            {
                *(u16 *)(p - 2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        break;
    }
    if (*(s16 *)(p + 0xAE) != 0)
    {
        c = (func_800644B8(*(s16 *)(p + 0xB4) << 8) >> 6) + 0x80;
        g = r->unk_0C[0];
        g = g + ((c - g) >> 2);
        r->unk_0C[0] = g;
        r->unk_0C[1] = g;
        r->unk_0C[2] = g;
    }
    *(s16 *)(p + 0xB4) = *(s16 *)(p + 0xB4) + 1;
    *(s16 *)(p + 0xA8) = *(s16 *)(p + 0xA8) - 1;
    if (*(s16 *)(p + 0xA8) > 0)
    {
        return;
    }
    *(s16 *)(p + 0xA8) = 0;
    if (*(s16 *)(p + 0xAC) == 1)
    {
        n = func_800A8E74(p, (u8 *)v, (s32)r, (u8 *)D_800814A8, *(u8 **)(p + 0x9C), -1);
        if (n == NULL)
        {
            return;
        }
        q = (u8 *)n + 0x20;
        *(u8 **)(D_800E3D7C + 0x124) = q;
        *(u8 **)(q + 0x9C) = D_80081484;
        *(s32 *)(q + 0x1C) |= 0x80000;
        *(u8 **)(*(u8 **)(D_800E3D7C + 0x124) + 0x60) = D_800E3D7C;
        *(s32 *)(D_800E3D7C + 0x1C) |= 0x100000;
    }
    else if (*(s16 *)(p + 0xAC) == 2)
    {
    }
    else if (*(s16 *)(p + 0xAC) != 0 || flag != 0)
    {
        return;
    }
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

/* Installer for state 0x23: the facing table at +0x2C and the eight-way
   func_80048A44 pick, then a node from func_800A8608 whose +0x10 handler is
   func_8008D470 and whose actor view gets the carrier and the mover; when the
   node cannot be had, func_8008ACDC is parked at +0x8C instead.
   95 words, 0 differing, on the first compile from func_8008CAA0's table
   idiom (:1296) and func_800A7700's node idiom (:7634).
   The two func_800A56E0 ids are ONE call with a ternary argument: the target
   sets $a0 = 0x505 in the `bne`'s delay slot and 0x516 on the fall-through. */
void func_8008D7D0(u8 *p, void *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *t;
    s32 v;

    p[0x9A] = 0x23;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(p + 0xA2) &= 0xFF7F;
    t = D_800DD0B8;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
    n = (UnkNode *)func_800A8608(p - 0x20, *(void **)(p + 0xBC), 0, 0, 0);
    *(UnkNode **)(p + 0x124) = n;
    if (n != NULL)
    {
        n->unk_10 = (u32)func_8008D470;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = (c[0x24] << 6) + 0x20;
        *(s16 *)((u8 *)m + 0x6) = (c[0x25] << 6) + 0x20;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(d + 0x88);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0xAA) = 1;
        *(s16 *)(q + 0xA8) = 0x1E;
        *(u8 **)(q + 0x90) = c;
        *(void **)(q + 0x8C) = b;
        *(u8 **)(q + 0x9C) = (u8 *)n + 0xB8;
        func_800A56E0(*(u8 *)(*(u8 **)(p + 0xBC) + 1) == 0xE ? 0x516 : 0x505);
        *(s16 *)(q + 0xAE) = *(u16 *)(d + 0x46) == 1;
        *(s16 *)(p + 0x46) = 0;
    }
    else
    {
        *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
    }
}

/* State 0x24 -- func_80090200 one state over, with the same
   whole-index-in-one-expression rule func_8008CCB0 records. 41 words,
   0 differing. */
void func_8008D94C(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    u8 *g;

    p[0x9A] = 0x24;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(p + 0xA2) &= 0xFEFF;
    func_80094E34();
    g = (u8 *)&D_80083460;
    *(s16 *)(g + 0x4) *= 2;
    t = D_800DCFF0;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
}

/* State 0x25: re-skin the actor from D_800DCFB0 unless func_80042900 says
   the walker is busy, then branch on what func_800A4474 finds under the
   actor's tile. D_80012090 is the CONSTANT ADDRESS and the `lui $at` at the
   store is the decisive tell. 71 words, 0 differing on the first compile. */
void func_8008D9F0(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    s16 r;
    s16 h;

    p[0x9A] = 0x25;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_80094E34();
    if ((s16)func_80042900((s8 *)d, 0xA) == 0)
    {
        t = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
    }
    r = func_800A4474(c[0x24], c[0x25]);
    if (r == 3)
    {
        h = func_8003F794(6, 0x20);
        D_800814E8 = h;
        D_80083120[h].unk_06 = 1;
        p[0x9B] = 0x10;
    }
    else if (r == 4)
    {
        *(s32 *)0x80012090 = 2;
        func_800A9024(2);
    }
}

/* Put the actor into state 0x26, point its renderable at the direction table
   and report. FIVE PARAMETERS, the second never read; the fifth is `u16` --
   the entry load is `lhu $s1, 0x30($sp)`, and a narrow stack parameter is
   what keeps that load at all. 55 words, 0 differing. */
void func_8008DB0C(UnkDungeonActor *p, void *b, u8 *c, s16 d, s16 e)
{
    u8 *t;

    p->unk_9A = 0x26;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    t = D_800DD130;
    *(u8 **)(c + 0x2C) = t;
    func_80048A44(c, t[((D_80083228 + *(s16 *)((u8 *)p + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    *((u8 *)p + 0x72) = d;
    *((u8 *)p + 0x73) = e;
    if (D_80082EB0 != NULL)
    {
        func_8009F644((u8 *)p, 0x68, *(s16 *)((u8 *)p + 0x8A), (s16)func_80098C80(D_80082EB0));
    }
    else
    {
        func_8009F644((u8 *)p, 0x88, *(s16 *)((u8 *)p + 0x8A), 0);
    }
}

/* State 0x23's per-frame step: re-aim the mover at the record's tile over the
   D_80083460 +0x04 countdown, hand off to func_8008D94C when the countdown
   hits the frame the 0x80013714 flag selects, re-skin from D_800DD050, then
   run the countdown down and close on func_800A2B04 / func_80094F74.

   0x80013714 IS THE RAW CONSTANT ADDRESS, not the declared D_80013714 --
   the same choice func_8008B870 records. A constant address is a two-word
   assembler MACRO inside ONE rtl insn, so reorg cannot steal its `lui' into
   the preceding `beqz $v0''s delay slot and that slot stays the target's
   `nop'; the declared symbol is a `high' + `lo_sum' pair whose `lui' reorg
   does steal, and the function comes out 127 words.

   D_80083460 IS A STRUCT HERE, worth 4 differing. The second divide must
   RELOAD the +0x04 halfword; through a `u8 *' cast it does not, because the
   `*(s32 *)(b + 0xC)' store is a PLUS address (only `nonscalar') and the
   halfword's own address folds to a constant, so cse's entry survives and the
   divisor stays in the register the guard left it in. Reading it as a
   COMPONENT_REF sets MEM_IN_STRUCT_P and the target's `lh $v1, 0x4($a1)'
   comes back -- which also frees $a0 for the `lw $a0, 0x4($s0)' the target
   puts in the `mflo' delay slot. `*(volatile s16 *)' forces the same reload
   but costs an `lhu' plus its own `sll 16'/`sra 16': 130 words.

   THE 3-OR-6 TEST IS A TERNARY. Written as two `if's with the call and
   `return' duplicated per arm, cross-jumping merges the COMPARES as well as
   the call -- the constant becomes a conditional `li 3'/`li 6' and a single
   `bne', 123 words. The ternary is what produces the target's asymmetric
   `beq -> call' / `bne -> continue' pair with the call block out of line.

   `r' is `s32': an `s16' local gets an HImode pseudo and pays `lhu' plus
   `sll 16'/`sra 16' at the comparison, 130 words. And `t = D_800DD050;' sits
   INSIDE the arm -- hoisted above the test its `lui'/`addiu' pair precedes
   the `lw $v1, 0x2C($s1)' and that is 2 differing. 128 words, 0 differing. */
void func_8008DBE8(u8 *p, void *b, u8 *c, u8 *d)
{
    UnkDungeonGlobal *g;
    UnkDungeonGlobal *g2;
    u8 *t;
    s32 r;
    s32 v;

    g = (UnkDungeonGlobal *)&D_80083460;
    if (g->unk_02 & 0x80)
    {
        g->unk_04 = 0;
    }
    if (g->unk_04 != 0)
    {
        *(s32 *)((u8 *)b + 0xC) = ((((c[0x24] << 6) + 0x20) << 16) - *(s32 *)b) / g->unk_04;
        *(s32 *)((u8 *)b + 0x10) =
            ((((c[0x25] << 6) + 0x20) << 16) - *(s32 *)((u8 *)b + 0x4)) / g->unk_04;
    }
    if (*(u16 *)(p + 0xA2) & 0x100)
    {
        r = g->unk_04;
        if ((*(u16 *)0x80013714 & 8) ? r == 3 : r == 6)
        {
            func_8008D94C(p, b, c, d);
            return;
        }
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        if (*(u8 **)(c + 0x2C) != D_800DD050)
        {
            t = D_800DD050;
            *(u16 *)(p + 0xA2) |= 1;
            *(u8 **)(c + 0x2C) = t;
            v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            func_80048A44(c, t[v], 0, 1);
        }
    }
    g2 = (UnkDungeonGlobal *)&D_80083460;
    g2->unk_04 = (u16)g2->unk_04 - 1;
    if (g2->unk_04 <= 0)
    {
        g2->unk_04 = 0;
        *(s32 *)((u8 *)b + 0x14) = 0;
        *(s32 *)((u8 *)b + 0x10) = 0;
        *(s32 *)((u8 *)b + 0xC) = 0;
        func_800A2B04((UnkVec *)b, c[0x24], c[0x25]);
        *(u16 *)(c + 0x14) |= 0x4000;
        if ((s16)func_80094F74(p, b, c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
    }
}

/* State handler reached with (actor, mover, renderable, record). The first
   visit primes the mover's +0x0C/+0x10 from func_80064584/func_800644B8 of
   the record's +0x6A angle and sets a 0xC countdown; after that the body
   eases the mover onto the renderable's tile over the remaining frames,
   halving the two deltas each step, and finally re-installs D_800DCFB0's
   facing table and hands the actor back to func_8008ACDC.

   THE SECOND TABLE'S INDEX MUST NOT SHARE `v` WITH THE FIRST ONE. With one
   function-scope `s32 v` assigned in both arms the D_800DCFB0 base takes
   $v1 and the `lh 0x2A` takes $a1 -- the target has them the other way
   round, 7 differing at the right length. Inlining EITHER of the two index
   expressions (so that `v` is set in one block only) reaches 0, and both
   spellings emit the identical word stream. 183 words, 0 differing. */
void func_8008DDE8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    u8 *g;

    if (p[0x9B] == 0)
    {
        func_800AD4D0(d);
        p[0x9B] += 1;
        b->unk_0C = func_80064584(*(s16 *)(d + 0x6A)) << 9;
        b->unk_10 = func_800644B8(*(s16 *)(d + 0x6A)) << 9;
        *(s16 *)(p + 0x96) = 0xC;
    }
    if ((*(u16 *)(p + 0x10C) & 1) != 0)
    {
        func_8008CAA0(p, b, c, d);
        return;
    }
    if (*(u8 **)(c + 0x2C) == D_800DCFE0)
    {
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            if ((*(s16 *)(p + 0x96) -= 1) > 0)
            {
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                func_80048A44(c, (*(u8 **)(c + 0x2C))[v], 0, 1);
            }
        }
    }
    if (*(s16 *)(p + 0x96) > 0)
    {
        b->unk_00 += ((((c[0x24] << 6) + 0x20) << 16) - b->unk_00) / *(s16 *)(p + 0x96);
        b->unk_04 += ((((c[0x25] << 6) + 0x20) << 16) - b->unk_04) / *(s16 *)(p + 0x96);
        b->unk_0C /= 2;
        b->unk_10 /= 2;
        if ((*(s16 *)(p + 0x96) -= 1) == 0)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
        }
    }
    if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
    {
        t = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t;
        func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        if (d[0x28] == 0)
        {
            if ((D_800E296C & 0x8000000) != 0)
            {
                d[0x28] = 1;
            }
            else
            {
                d[0x28] = 0;
                func_8008CBD4(p, (u8 *)b, c, d);
                return;
            }
        }
        *(s16 *)(p + 0x96) = 8;
        *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
    }
}

/* The warp/stair state handler, a sibling of func_8008F878 above. Four cases
   -- 0, 1, an EMPTY 2 (its `beq $v1, 2` jumps straight to the break label)
   and 0x10 -- so the switch tree is `beq ==1`, `slti <2`, then the 2/0x10
   pair. Case 0 announces D_800E0672 once the renderable's 0xE000 gate is up
   and arms the 0x80 countdown; case 1 runs that countdown out and then
   either latches D_800E3CD0 and pokes func_80040AA0(3), or tears the floor
   down through func_800945E8/func_800948BC and the 0xC000 fade; case 0x10
   copies the view's +0x29 byte down to +0x28, points it at the quadrant
   D_80083160+0xC8 selects, reinstalls func_8008ACDC and drops the engine's
   +0xA population count.

   0x80013714 MUST BE THE CONSTANT ADDRESS, not include/structs.h's
   D_80013714, and it is worth exactly one word -- the same fact
   func_8008C7B4 and func_80094208 record for the same global. As the symbol
   the `lui`/`lhu` pair is `high` + `lo_sum` in two separate insns and reorg
   steals the `lui` into the preceding `bgtz`'s delay slot, where the target
   has a `nop`: 103 words against 104, 67 differing. As a constant address
   the pair is one assembler macro using the destination as its own temp and
   nothing can be scheduled into it.

   `D_800E3CD0 = 1;` comes out as `sb $v1` off the switch subject's own
   register, because cse knows $v1 == 1 inside the `case 1` arm.

   104 words, 0 differing. */
void func_8008E0C4(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            func_80099844(d, D_800E0672);
            *(s16 *)(p + 0x96) = 0x80;
            p[0x9B]++;
        }
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) <= 0)
        {
            if (*(u16 *)0x80013714 & 0x4)
            {
                if (D_800E3CD0 == 0)
                {
                    D_800E3CD0 = 1;
                    func_80040AA0(3);
                }
            }
            else
            {
                func_800945E8(p);
                func_800948BC();
                D_80082E76 = 0xC000;
                func_80041094(6, 0, 0, 0, 0xC000);
                p[0x9B]++;
            }
        }
        break;
    case 2:
        break;
    case 0x10:
        if (*(u16 *)(c + 0x14) & 0xE000)
        {
            d[0x28] = d[0x29];
            *(s16 *)(d + 0x2A) = 0x400 - ((*(u16 *)(g + 0xC8) + 0x100) & 0xE00);
            *(void **)(p + 0x8C) = func_8008ACDC;
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0xA) -= 1;
        }
        break;
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088760);

/* The three-way view stepper's sibling one state table over, and
   func_800971A8 (:3949) is its line-for-line template: the same 0/8/10,
   1/9/11 and 12 case groups over the same D_80083460 countdown, the same
   `-0x100000 / -0x80000 / -0x140000` fall rates and the same
   `*((s16 *)g + 2)` divisor. States 0/8/10 wait for the renderable's 0x6000
   bits, set the fall rate from the state, tear the carrier down and re-skin
   from D_800DD040; 1/9/11 glide the mover onto the tile and then either
   re-skin from D_800DD058 or hand over to func_80094F74; 12 clears the
   countdown and does the same hand-over. The jump table is jtbl_80088810 at
   rodata offset 0xB0, 0 mod 8, so cc1psx's `.align 3` is a no-op.

   THE SKIN TABLE MUST BE NAMED TWICE, NOT BOUND TO A LOCAL. With
   `t = D_800DD040; *(u8 **)(c + 0x2C) = t; func_80048A44(c, t[...], ...)` --
   the spelling func_8008D7D0 and func_8008E504 use -- the symbol's `high`
   takes $a0 and its `lo_sum` $a1, and the `addu $a0, $s1, $zero` that the
   target emits BEFORE the flag update is pushed six insns later: 168 words,
   6 differing, three in each of the two case groups. Writing the symbol out
   at both sites is 0.

   The case-12 store reaches D_80083460 + 4, so the candidate relocates
   against D_80083460 where splat's listing names D_80083464; both resolve to
   the same `lui`/`sh` pair, and func_800971A8's landed case 12 does the same.
   168 words, 0 differing. */
void func_8008E264(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    s16 n;
    s32 x;
    s32 y;

    switch (p[0x9B])
    {
    case 0:
    case 8:
    case 10:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            if (p[0x9B] == 0)
            {
                b->unk_14 = -0x100000;
            }
            else if (p[0x9B] == 8)
            {
                b->unk_14 = -0x80000;
            }
            else
            {
                b->unk_14 = -0x140000;
            }
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 0x812;
            *(u16 *)(p + 0x98) &= 0xFFF3;
            *(u8 **)(c + 0x2C) = D_800DD040;
            func_80048A44(c, D_800DD040[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
            func_800A56E0(0x50A);
        }
        break;
    case 1:
    case 9:
    case 11:
        g = (u8 *)&D_80083460;
        if (*((s16 *)g + 2) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *((s16 *)g + 2);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *((s16 *)g + 2);
        }
        n = *(u16 *)(g + 0x4) - 1;
        *(s16 *)(g + 0x4) = n;
        if (n <= 0)
        {
            *(s16 *)(g + 0x4) = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            if (p[0x9B] >= 0xA)
            {
                *(u8 **)(c + 0x2C) = D_800DD058;
                func_80048A44(c, D_800DD058[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0,
                              1);
                p[0x9B]++;
                *(s16 *)(g + 0x4) = 1;
            }
            else if ((s16)func_80094F74(p, b, c, d) > 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    case 12:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 0;
            if ((s16)func_80094F74(p, b, c, d) > 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}

/* State machine for the stairs/warp confirm, off the actor's +0x9B. State 0
   waits for the renderable's 0x10 flag, stops the mover, and either waits
   one frame (when the record's +0x88/+0x8A pair is within 0x40) or rumbles,
   releases the engine's parked +0x124 record exactly as the landed
   func_80094E34 does, and waits 0xC; it falls through into state 1, which
   either hands the actor to func_8008EAC8 or re-skins it from D_800DD058 and
   raises the renderable's 0x800; state 2 runs the +0x96 countdown down,
   drops the 0x800 again and installs func_8008ACDC when either the
   renderable still has a 0xE000 bit or func_80094EA4 agrees.

   BOTH +0x1C FLAG WORDS HAVE TO BE ARRAY REFS, not `u8 *` casts -- the rule
   func_80094E34's own comment records, and it is worth one word plus thirteen
   here. Only an array ref is MEM_IN_STRUCT_P and only a MEM_IN_STRUCT_P load
   is hoisted over the `%lo(D_80081484)` store, which is what lets the two
   masks and D_80081484's own load materialise ahead of the +0x124
   dereference and removes its load-delay `nop`. Measured: both as casts is
   128 words against 127 with 90 differing; `((u32 *)q)[7]` alone 127 / 15;
   `((u32 *)d)[7]` alone 127 / 13; both 127 / 0.

   `sh $a0, 0x96($s0)` in the short arm is the constant 1 left in $a0 by the
   switch's own `== 1` compare, not a fourth argument. 127 words,
   0 differing. */
void func_8008E504(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    u8 *q;
    void *s;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        if (*(s16 *)(d + 0x88) - *(s16 *)(d + 0x8A) >= 0x41)
        {
            func_800419EC(8, 0x10);
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                q = *(u8 **)(p + 0x124);
                ((u32 *)q)[7] &= ~0x80000;
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                D_800E3540 = s;
                ((u32 *)d)[7] &= ~0x100000;
            }
            *(s16 *)(p + 0x96) = 0xC;
        }
        else
        {
            *(s16 *)(p + 0x96) = 1;
        }
        p[0x9B]++;
        /* fall through */
    case 1:
        if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            break;
        }
        t = D_800DD058;
        *(u8 **)(c + 0x2C) = t;
        func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u16 *)(c + 0x14) |= 0x800;
        p[0x9B]++;
        break;
    case 2:
        if (--*(s16 *)(p + 0x96) <= 0)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
            if ((*(u16 *)(c + 0x14) & 0xE000) != 0 || (s16)func_80094EA4() != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}

/* 208 words, 0 differing.

   THE SHAPE IS FORCED, AND THE PROOF IS WORTH KEEPING.  The target emits the
   `+0x1C` flag READ before the `*D_80081484 = NULL` store and the `+0x1C`
   WRITE after it:

       H = lw  $v0, 0x1C($s0)      (139)
       G = sw  $zero, %lo(D_80081484)($a1)  (142)
       I = sw  $v0, 0x1C($s0)      (147)

   `*(u32 *)(d + 0x1C)` is `(mem (plus (reg) 28))` -- non-struct, varying --
   and `*(void **)D_80081484` is `(mem (lo_sum (reg) (symbol_ref)))`, which
   `rtx_varies_p` calls NON-varying and which is also non-struct.  Neither
   side gets the `MEM_IN_STRUCT_P && rtx_addr_varies_p` escape in
   `true_dependence`/`anti_dependence`/`output_dependence`
   (sched.c:846-911), and `memrefs_conflict_p` cannot prove two non-constant
   addresses apart, so BOTH edges exist.  A single
   `*(u32 *)(d + 0x1C) &= ~0x100000;` statement therefore puts H and I on the
   same side of the store and one edge always points the wrong way: written
   AFTER the store, true_dependence pins H after G and H can never reach 139;
   written BEFORE it, output_dependence pins I before G and G can never reach
   142.  **So the read-modify-write has to be SPLIT ACROSS the store**, which
   is the `m = ...; s = ...; store NULL; ... = m & k;` below.

   `((u32 *)d)[7] &= ~0x100000;` DOES set MEM_IN_STRUCT_P (expr.c:5343-5351 --
   the operand is a PLUS_EXPR, which `*(u32 *)(d + 0x1C)` is not) and so
   deletes both edges, and it does not help: with no edge every placement
   collapses to the same bad schedule with the +0x1C load still AFTER the
   store, 19-23 rows.  The registers agree -- the landed sibling
   func_8008E504, which the array-ref rule was written against, puts its two
   masks in $a2/$a3, every array-ref spelling here puts the mask in $a3, and
   THIS target has it in $v1, which is what the cast spelling gives.

   TWO DEVICES CLOSE THE REST, and both are register placement.

   1. `register u32 k __asm__("$3");` for the 0xFFEFFFFF mask -- worth
      ELEVEN rows.  local_alloc sorts quantities by
      `QTY_CMP_PRI = floor_log2(n_refs) * n_refs * size / (death - birth)`
      (local-alloc.c:1693) and hands each the lowest-numbered free register,
      and off this body's `.lreg` the `%hi(D_80081484)` pseudo is 3 refs over
      10 insns (0.30) against the mask's 2 over 8 (0.25) -- so the `%hi` is
      priced first and takes $v1 while the mask gets $a1.  The target has
      them the other way round.  Winning that comparison from the source
      needs the mask at 3 references, or its range at 6 insns, or the `%hi`'s
      at 13, and none is reachable: cse2 merges the `(high (symbol_ref))`
      however the pair is written and rematerialises a bare `~0x100000`
      whatever local you bind it to.  So do not fight the priority -- name
      the register.  Re-checked with the pin removed (13 rows): a
      `do { } while (0);` wrapper at every statement is 13, a duplicated-arm
      `if (v) { s } else { s }` for each of k/m/s/d/c at every statement is
      13, and a greedy second round over both finds nothing.  No plain-C
      equivalent exists here.
   2. `g2 = c;` -- a copy of the `c` parameter, with the `+0x2C` store made
      through it.  This is the last TWO rows and decomp-permuter found it
      after 143000 candidates; every one of the 540 legal source orders of
      the split basin had measured 2, and the pin could only put the
      offending `addu $a0, $s2, $zero` at word 135 or 138 and never at the
      target's 137.  `g2` already exists as a local for case 2, so the copy
      costs no declaration -- and because its uses are in two different
      blocks it survives cse rather than being propagated away.

   ORDERING, measured over ALL 540 legal source orders of this basin (L
   before or after P, the mask and write-back in each slot after the NULL
   store, times the 30 legal permutations of the five tail statements with
   g before the 0xA bump and t2 before its store): the floor is 2 before
   device 2 and eight orders reach it, the worst is 36.  The one below is
   `L P Q K M X G Y W V E`.

   Also measured and rejected: the comma trick that would split the RMW
   without a temp -- `*(u32 *)(d + 0x1C) &= (*(void **)D_80081484 = NULL,
   ~0x100000);` -- is byte-identical to the unsplit body, because expand_expr
   evaluates the side-effecting operand of a commutative binop FIRST; mixed
   forms that put the MEM_IN_STRUCT escape on ONE HALF of the clear only
   (worth knowing: it is the only way to break one edge without the other)
   best 14 over all 180 of their legal orders; the temp's TYPE makes no
   difference at all (u32/s32/int/unsigned long); and reusing an existing
   local for it instead of a fresh `m` is 13-20. */
void func_8008E700(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *t;
    u8 *t2;
    void *s;
    s16 n;
    s32 r;
    s32 v;
    u32 m;
    register u32 k __asm__("$3");

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            b->unk_14 = -0x160000;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u8 **)(c + 0x2C) = D_800DD0C8;
            }
            else
            {
                *(u8 **)(c + 0x2C) = D_800DD048;
            }
            func_80048A44(
                c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        n = ++*(u16 *)(p + 0x96);
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0 && n >= 4)
        {
            b->unk_14 = 0;
            if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            }
            else
            {
                t = D_800DD060;
                *(u8 **)(c + 0x2C) = t;
                func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            p[0x9B]++;
        }
        else if ((*(u32 *)(d + 0x1C) & 0x300000) == 0x100000 && (*(u16 *)0x80013714 & 1) == 0)
        {
            r = func_8009AF18(*(s16 *)(d + 0x2A), b, c, 8);
            if ((s16)r != 0)
            {
                ((u32 *)*(u8 **)(p + 0x124))[7] |= 0x200000;
                ((u32 *)d)[7] |= 0x200000;
                *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
                (*(u8 **)(p + 0x124))[0x84] = 0x78;
                (*(u8 **)(p + 0x124))[0x85] = 2;
                func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
                m = *(u32 *)(d + 0x1C);
                s = *(void **)D_80081484;
                *(void **)D_80081484 = NULL;
                k = ~0x100000;
                *(u32 *)(d + 0x1C) = m & k;
                g2 = c;
                t2 = D_800DCFD0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                *(u8 **)(g2 + 0x2C) = t2;
                v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
                D_800E3540 = s;
                func_80048A44(c, t2[v], 5, 1);
                func_800A56E0(0x512);
            }
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0xA) == 0)
            {
                if ((*(u32 *)(d + 0x1C) & 0x200000) != 0)
                {
                    *(u32 *)(d + 0x1C) &= 0xFFDFFFFF;
                    *(u16 *)(g2 + 0x2) |= 0x412;
                    func_80099F70(*(void **)(d + 0x5C));
                    func_80099F04(*(void **)(d + 0x5C));
                }
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}

/* The same two-handler install as func_80096088 below, behind two teardown
   calls on the fourth argument's +0x5C and the 0x812 flag set. The second
   and third parameters are never read. 34 words, 0 differing. */
void func_8008EA40(UnkDungeonActor *a, void *b, void *c, u8 *d)
{
    u8 *g;

    func_80099F70(*(void **)(d + 0x5C));
    func_80099F04(*(void **)(d + 0x5C));
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x2) |= 0x812;
    if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
    {
        a->unk_8C = func_8008EAC8;
    }
    else
    {
        a->unk_8C = func_8008ACDC;
    }
}

void func_8008EAC8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    u8 *m;
    u8 *t;
    u8 *t2;
    u8 *u;
    s32 e;
    s32 w;
    s32 w2;
    s32 r;
    s32 n;
    s16 i;
    s32 j;
    s32 cur;
    s32 delta;
    s16 want;
    u16 k;

    if (p[0x9A] != 0xE)
    {
        if ((*(u16 *)(p + 0xA2) & 0x100) != 0)
        {
            func_8008D94C(p, b, c, d);
            return;
        }
    }
    p[0x9A] = 0xE;
    if ((*(s32 *)(d + 0x14) & 0x100000) == 0)
    {
        func_800A4300(c, d);
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
    }
    *(u16 *)(c + 0x14) &= 0xF7FF;
    q = (u8 *)&D_80083460;
    p[0x9B] = 0;
    *(u16 *)(q + 0x2) &= 0xFF7F;
    r = *(s16 *)(d + 0x64);
    if (r < 0 || (*(u16 *)(p + 0x10C) & 1) != 0)
    {
        func_8008CAA0(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if ((*(s32 *)(d + 0x14) & 0x20000) != 0)
    {
        *(s32 *)(d + 0x14) &= 0xFFFDFFFF;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        func_8008C468(p, b, c, d);
        return;
    }
    e = *(s32 *)(d + 0x1C);
    if ((e & 0x200) != 0)
    {
        func_80090200(p, b, c, d);
        return;
    }
    if ((e & 0x100000) == 0)
    {
        *(void **)(p + 0x8C) = func_8008ACDC;
        t2 = D_800DCFB0;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        return;
    }
    if (*(s32 *)(p + 0x124) == 0)
    {
        return;
    }
    if ((s16)func_800A1C58(d) != 0)
    {
        if (func_8008D1D0(p, b, c, d) != 0)
        {
            return;
        }
    }
    if ((*(u16 *)(q + 0x2) & 0x4) != 0)
    {
        goto tail;
    }
    if ((*(s32 *)(d + 0x1C) & 0x20) != 0)
    {
        if ((*(u16 *)0x80013714 & 1) == 0 && (g->unk_08 & 0x80) != 0)
        {
            *(s16 *)(d + 0x8A) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = NULL;
            *(s32 *)(p + 0xC8) = 0;
            *(s32 *)(p + 0x104) = 0;
            return;
        }
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)0x80013714 & 1) != 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        if ((*(u16 *)(p + 0xA2) & 0x200) != 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFDFF;
        }
        m = func_8009F868();
        if (m == NULL)
        {
            return;
        }
        want = (m[0x1] & 7) << 9;
        *(s16 *)(d + 0x2A) &= 0xFFF;
        if (*(s16 *)(d + 0x2A) != want)
        {
            if ((*(u16 *)(d + 0x2A) & 0x800) != 0)
            {
                *(s16 *)(d + 0x2A) |= 0xF800;
            }
            else
            {
                *(s16 *)(d + 0x2A) &= 0x7FF;
            }
            do
            {
                want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                cur = *(u16 *)(d + 0x2A);
                e = (u16)want;
                e <<= 16;
                delta = *(s16 *)(d + 0x2A) - (e >> 16);
            } while (0);
            if (delta < 0)
            {
                delta = -delta;
            }
            if (delta > 0x800)
            {
                *(s16 *)(d + 0x2A) = (want & ~0xFFF) | (cur & 0xFFF);
            }
            if (*(s16 *)(d + 0x2A) > (e >> 16))
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) - 0x200;
            }
            else
            {
                *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
            }
            func_8009F988();
            goto tail;
        }
        D_800E3544 = m[0x1] & 0xF8;
        switch (D_800E3544)
        {
        case 0x8:
            func_8008C13C(p, b, c, d);
            return;
        case 0x10:
            func_8008C7B4(p, b, c, d);
            return;
        case 0x28:
            func_8008F6EC(p, b, c, d);
            return;
        case 0x30:
            *(s16 *)(p + 0x96) = 6;
            func_8008FA14(p, b, c, d);
            return;
        case 0x48:
            if ((s16)func_80095538(p, m[0x0] & 0x1F, m[0x2] & 0x1F) < 0)
            {
                func_8009F988();
            }
            else
            {
                func_8009FAAC();
            }
            return;
        case 0x50:
            i = m[0x0];
            m = (u8 *)((i & 0x60) >> 5);
            if (func_80098920(*(u8 **)(p + 0xAC + (s32)m * 4), func_8009FADC(i & 0x1F), 0x15, 0) < 0)
            {
                func_8009F988();
            }
            return;
        case 0x68:
            func_80094270(p, b, c, func_8009FADC(m[0x0] & 0x1F), (m[0x0] & 0x60) >> 5);
            return;
        case 0x88:
            j = (m[0x0] & 0x60) >> 5;
            u = *(u8 **)(p + 0xD0 + j * 4);
            func_80094270(p, b, c, u, j);
            return;
        case 0x70:
            func_80094548((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x78:
            func_8009458C((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x80:
            func_800945C4((m[0x0] & 0x60) >> 5, m[0x0] & 0x1F);
            return;
        case 0x90:
            func_8009456C((m[0x0] & 0x60) >> 5, m[0x0] & 7);
            return;
        case 0x98:
            u = func_8009FADC(m[0x0] & 0x1F);
            if ((u[0x3] & 0x20) != 0)
            {
                func_800956B8(p, b, c, u);
            }
            else
            {
                func_80095854(p, b, c, u);
            }
            return;
        case 0xA0:
            func_8002534C(p, b, c, d);
            return;
        case 0xD8:
            D_800DD830[m[0x0] & 0x7F]();
            return;
        }
        return;
    }
    k = *(u16 *)(p + 0xA2);
    D_800E3544 = 0;
    if ((k & 0x200) != 0 && (s16)func_800A2C34(d) == 0 && (*(u16 *)(q + 0x2) & 0x4) == 0)
    {
        *(u16 *)(p + 0xA2) &= 0xFDFF;
        if ((s16)func_800A4474(c[0x24], c[0x25]) != 0)
        {
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
    }
    w = g->unk_08;
    if ((w & 0x80) != 0)
    {
        func_8008CF6C((UnkDungeonActor *)p, b, c, D_80050CAC);
        return;
    }
    if ((w & 0x10) == 0 && (w & 3) != 0)
    {
        if (func_8008D024(p, b, c, (((u32)w >> 1) ^ 1) & 1, 0) != 0)
        {
            return;
        }
    }
    n = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
    if ((s16)n != 0xFFF)
    {
        *(s16 *)(d + 0x2A) = n;
        if ((g->unk_08 & 0x10) == 0 && (*(u16 *)(p + 0xA2) & 0x400) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFFE;
            if ((*(s32 *)(d + 0x1C) & 0x400) != 0)
            {
                *(u16 *)(d + 0x2A) += func_800A6D30() & 0xE00;
            }
            func_8008C13C(p, b, c, d);
            return;
        }
    }
    w2 = g->unk_08;
    if ((w2 & 0x30) == 0x30)
    {
        *(u16 *)(p + 0xA2) &= 0xFFFE;
        func_8008C7B4(p, b, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x40) != 0)
    {
        if ((w2 & 0x40) == 0)
        {
            *(u16 *)(p + 0xA2) &= 0xFFBF;
        }
    }
    else if ((w2 & 0x40) != 0)
    {
        if ((w2 & 0x20) == 0)
        {
            func_8008F6EC(p, b, c, d);
            return;
        }
        else
        {
            *(s16 *)(p + 0x96) = 6;
            func_8008FA14(p, b, c, d);
            return;
        }
    }
tail:
    t = D_800DD0B8;
    *(u8 **)(c + 0x2C) = t;
    func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
}

/* RETIRES THE func_8008F428 PARK (177 of 177 words, 15 differing, thirteen
   of them the two masks in the `p[0x9B] == 1` arm). The park's residue was
   the one func_8008E504's comment had already solved three hundred lines
   away: BOTH +0x1C FLAG WORDS HAVE TO BE ARRAY REFS. Only `((u32 *)q)[7]`
   is MEM_IN_STRUCT_P, and only a MEM_IN_STRUCT_P load is hoisted over the
   `%lo(D_80081484)` store, which is what lets the two masks materialise
   ahead of the +0x124 dereference. No statement order and no `u32` local
   per constant reaches it, which is exactly what the park had measured.

   THREE MORE SPELLINGS WERE EACH WORTH WORDS, all measured from the
   177-word base:
   * The two bit tests on the record's +0x1C are NESTED IFS, not an `&&`
     chain: `(w & 0x2000) == 0 && (w & 0x228) == 0` is combined into one
     `andi 0x2228` and is 174 words.
   * The two facing tables must be NAMED TWICE, not bound to a `u8 *t`
     local -- func_8008E264's rule, and here it is worth 13 differing
     (17 against 4): with the local, the `addu $a0, $s2, $zero` the target
     emits ahead of the handler store is pushed past it in both arms.
   * `D_800E3DA8` must be declared `extern s16 D_800E3DA8[]` and written as
     `D_800E3DA8[0]` / `D_800E3DA8[1]`. As a scalar with
     `*(s16 *)((u8 *)&D_800E3DA8 + 2)` for the second the lo_sum lands
     AFTER the second rand call instead of in its delay slot: 2
     differing. A `u8 *` base local and `(&D_800E3DA8)[1]` are both 2 as
     well; only the array declaration reaches 0.
   * The message handle is `h = func_800990FC(); m = h;` -- func_800956B8's
     shape. As `m = func_800990FC();` with `m` passed to func_80099734 the
     result goes to $s0 first and is copied to $a1; the target copies the
     call result into $a1 and saves $a1 into $s0. 2 differing.

   177 words, 0 differing. */
void func_8008F428(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *q;
    u8 *g;
    void *s;
    void *h;
    void *m;
    u32 w;

    if (*(u8 **)(p + 0x124) != NULL)
    {
        if ((*(u16 *)(c + 0x14) & 0x6000) == 0)
        {
            return;
        }
        if (p[0x9B] == 0)
        {
            D_800E3DA8[0] = 0;
            D_800E3DA8[1] = 0;
            w = *(u32 *)(*(u8 **)(p + 0x124) + 0x1C);
            if ((w & 0x2000) == 0)
            {
                if ((w & 0x228) == 0)
                {
                    if ((s16)func_800A6DA4(0, 0x10) < 6)
                    {
                        *(s16 *)(p + 0x96) = 0x10;
                        p[0x9B]++;
                        return;
                    }
                }
            }
            *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            *(u8 **)(c + 0x2C) = D_800DD0B8;
            func_80048A44(c, D_800DD0B8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            return;
        }
        if (p[0x9B] == 1)
        {
            D_800E3DA8[0] = (rand() & 0xF) - 8;
            D_800E3DA8[1] = (rand() & 0xF) - 8;
            if ((*(s16 *)(p + 0x96) -= 1) > 0)
            {
                return;
            }
            func_800A56E0(0x510);
            func_80099844(*(u8 **)(p + 0x124), D_800E0523);
            q = *(u8 **)(p + 0x124);
            ((u32 *)q)[7] &= ~0x80000;
            s = *(void **)D_80081484;
            *(void **)D_80081484 = NULL;
            D_800E3540 = s;
            ((u32 *)d)[7] &= ~0x100000;
            p[0x9B]++;
            return;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x2) |= 0x412;
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
    }
    else
    {
        if (p[0x9B] == 0)
        {
            if (*(s8 *)(c + 0x4) < 2)
            {
                return;
            }
            *(u16 *)(c + 0x14) |= 0x800;
            *(s16 *)(p + 0x96) = 0x10;
            p[0x9B]++;
            return;
        }
        if ((*(s16 *)(p + 0x96) -= 1) > 0)
        {
            return;
        }
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_800A56E0(0x506);
        h = func_800990FC();
        m = h;
        h = func_80099734(*(u8 **)(p + 0x11C), h);
        h = func_80099194(D_800E0683, h);
        func_80099290(h);
        func_800A5720(m);
    }
    *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
    *(u8 **)(c + 0x2C) = D_800DCFB0;
    func_80048A44(c, D_800DCFB0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
}

/* The step that puts the actor into state 0x1F: pick the gate by whether
   D_80081485 holds 0x13 -- func_8009A66C's halfword result, or a
   func_800A44E0 refusal followed by func_800A7234 over the direction
   tables -- and on a pass re-skin the renderable off D_800DD0D0 and report
   with func_8009F644. 99 words, 0 differing.

   The gate is ONE `s32 ok` spanning both arms: the first arm's
   `slt $s1, $zero, $v0` is jump.c's store-flag of `(s16)r > 0` and the
   second arm's is a plain `li 1`, and the shared `beqz $s1` is what the
   body hangs off.

   The direction index is `(*(u16 *)(d + 0x2A) >> 9) & 7` with the entries
   read as `(u16)D_8006CCD8[k]`, the spelling func_8009B164 records: combine
   folds the index's `* 2` back into `srl 8` / `andi 0xE`.

   D_80081485 is declared `extern u8 D_80081485[]`, so the test has to be
   `D_80081485[0]`: written as a bare scalar the compare takes the table's
   ADDRESS and the three words at 0x8008F714 come out as an `addiu` plus a
   swapped `bne` pair, 3 differing. */
void func_8008F6EC(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    u8 *g;
    s32 ok;
    s32 k;
    s32 v;
    s16 o0;
    s16 o1;
    s16 o2;

    ok = 0;
    if (D_80081485[0] == 0x13)
    {
        ok = (s16)func_8009A66C(*(s16 *)(d + 0x2A), c, d, 0x20) > 0;
    }
    else if ((s16)func_800A44E0(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                                *(s16 *)(d + 0x88), *(s16 *)(d + 0x2A)) == 0)
    {
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        if ((s16)func_800A7234((s16)(c[0x24] + (u16)D_8006CCD8[k]),
                               (s16)(c[0x25] + (u16)D_8006CCE8[k]), *(s16 *)(d + 0x88), &o0,
                               &o1, &o2) != 0)
        {
            ok = 1;
        }
    }
    if (ok)
    {
        a[0x9A] = 0x1F;
        a[0x9B] = 0;
        *(s32 *)(a + 0x8C) = 0;
        t = D_800DD0D0;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        func_8009F644(d, 0x28, 0, 0);
    }
}

/* The cut-scene state handler. Case 0 arms the scene -- bail out if
   func_8004CAE8 claims the node's +0x8 object, park the engine record from
   +0x124 in D_800DD25C (dropping it again when its +0x13 counter is not
   positive), clear the two request bits, ask func_8009A350 for the tile the
   facing at d+0x2A selects and play 0x517 or 0x50C by its 0x400 terrain bit,
   then hand D_80081484's word to D_800E3540 exactly as the landed
   func_80094E34 does. Case 1 waits for the renderable's 0x6000 gate and
   closes: raise the engine's 0x412 flags, run the two func_80099Fxx closers
   over d+0x5C, clear the parked record's +0x6D, install func_8008ACDC at
   +0x8C and take the +0xA population counter down.

   THERE IS A THIRD, EMPTY CASE, and it is worth seven words. With only
   `case 0` and `case 1` gcc leaves the two nodes as a right-chain and emits
   `beqz` for 0 then `beq 1` -- 103 words, 7 differing, including a swapped
   `sw $ra`/`sw $s2` pair in the prologue. A third node makes
   `balance_case_nodes` pick 1 as the root, which is the target's
   `beq ==1` / `slti <2` / `bnez !=0` tree; the empty arm's own test is then
   deleted by jump.c because both of its edges reach the break label. The
   number of trailing empty cases is NOT witnessed -- `case 2: case 3:`
   measures identically -- only that there is at least one.

   103 words, 0 differing. */
void func_8008F878(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    void *t;
    u16 w;

    switch (p[0x9B])
    {
    case 0:
        if (func_8004CAE8(*(u8 **)(c + 0x8), 0) != 0)
        {
            break;
        }
        D_800DD25C = *(u8 **)(p + 0x124);
        if (*(s8 *)(D_800DD25C + 0x13) <= 0)
        {
            D_800DD25C = NULL;
        }
        *(u32 *)(*(u8 **)(p + 0x124) + 0x1C) &= ~0x80000;
        *(u32 *)(d + 0x1C) &= ~0x100000;
        func_8009A350(c[0x24], c[0x25], (*(u16 *)(d + 0x2A) >> 9) & 7, &w);
        func_800A56E0((w & 0x400) ? 0x517 : 0x50C);
        t = *(void **)D_80081484;
        *(void **)D_80081484 = NULL;
        D_800E3540 = t;
        p[0x9B]++;
        break;
    case 1:
        if (*(u16 *)(c + 0x14) & 0x6000)
        {
            p[0x9B] = 2;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x412;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            if (D_800DD25C != NULL)
            {
                D_800DD25C[0x6D] = 0;
            }
            *(void **)(p + 0x8C) = func_8008ACDC;
            *(u16 *)(g + 0xA) -= 1;
        }
        break;
    case 2:
        break;
    }
}

/* Installer: state 0x20, handler slot cleared, a sound, one flag bit dropped
   at +0xA2 and the remaining life handed to func_8009F644.
   26 words, 0 differing. */
void func_8008FA14(u8 *p, void *b, void *c, u8 *d)
{
    p[0x9A] = 0x20;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    func_800A56E0(0x512);
    *(u16 *)(p + 0xA2) &= 0xFFEF;
    func_8009F644(d, 0x30, *(s16 *)(p + 0x96), 0);
}

/* State machine for the stairs/warp confirm's second half, off the actor's
   +0x9B: state 0 skins from D_800DD0C8, state 1 runs the +0x96 countdown down
   and on a non-zero func_8009AF18 result raises the renderable's 0x200000,
   writes its +0x8A/+0x84/+0x85 trio, drops d's +0x100000, parks D_80081484's
   record in D_800E3540, bumps D_80083460's +0xA counter and re-skins from
   D_800DCFD0; state 2 re-skins from D_800DD060 and state 3 hands the actor to
   func_8008EAC8 or func_8008ACDC.  181 words, 0 differing.

   STATE 1's +0xA BUMP NEEDS ITS OWN LOCAL *AND* AN ARRAY REF, and those are
   two INDEPENDENT requirements worth 13 rows between them.  Both +0xA accesses
   are the same halfword, but state 1's must be `((s16 *)g1)[5]' on a local
   that only state 1 writes:

   * ITS OWN LOCAL.  Sharing state 3's `g' gives the pseudo TWO sets, so
     `adjust_priority' (sched.c:2583) never sees `birthing_insn_p' -- which
     gates on `reg_n_sets == 1' -- and the `lo_sum' keeps priority 1 instead of
     being bumped to LAUNCH_PRIORITY the cycle it becomes ready.  Shared, with
     no device: 180 words / 31 rows.
   * AN ARRAY REF, so MEM_IN_STRUCT_P is set on the `lhu'/`sh' pair.  Only then
     does true_dependence's escape clause (sched.c:880) fire against
     `%lo(D_80081484)' and `0x1C(d)', and only then is the whole D_80081484
     group free to float BELOW the `and' that kills the 0xFFEFFFFF mask.  That
     is what lets `lui %hi(D_80081484)' reuse $a1 at word 75 instead of taking
     $a0 at word 63 -- and $a0 staying free is the target's
     `addu $a0, $s2, $zero' at word 63 and its load-delay `nop' at 67.  On its
     own local but through a cast, `*(s16 *)(g1 + 0xA) += 1;' is 182 / 32.

   NEITHER HALF MOVES ANYTHING ALONE, and the park had a measured NEGATIVE on
   each of them separately -- "separate local 182/32" and "array ref 180/31" --
   each true of the body it was measured on.  Two correct halves of one answer,
   filed as two independent failures.

   The signedness and the struct type are irrelevant: `((u16 *)g1)[5]',
   `((s16 *)g1)[5]' and a `UnkDungeonGlobal *' local's `gs->unk_0A' are all
   181/0, while `*(u16 *)((u8 *)g1 + 0xA)' is 182/32.  It is the ARRAY REF, not
   the width and not the typedef.

   DO NOT ADD A `do { } while (0)' AROUND THE BUMP.  It was worth 18 rows while
   the increment was a cast on the shared `g' (31 -> 13), because it forced the
   occupied cycle the missing dependence edge would have supplied; on this body
   it re-breaks what the array ref fixes, 183 / 30.

   STILL LOAD-BEARING, with what the cast costs here:
   * the call's angle must be `((s16 *)d)[0x15]', not `*(s16 *)(d + 0x2A)':
     184 / 36;
   * `((u32 *)d)[7] &= ~0x100000' must be the array ref: 181 / 2;
   * the `s16'-returning alias below is required -- the real definition's `u16'
     first parameter gives `lhu' where the target has `lh'.
   NO LONGER load-bearing: `((u32 *)q)[7] |= 0x200000' may be written
   `*(u32 *)(q + 0x1C) |=' (181/0).  State 1's statement order is the best of
   all 60 orderings of its five groups; the next best is 15 rows. */
/* func_8009AF18 is defined below with a `u16' first parameter, which gives
   `lhu' where this call site needs `lh'.  The alias declares the same symbol
   with the parameter type this caller needs; the other caller keeps the
   implicit int it has today, and the definition re-measures 147/0. */
s16 func_8009AF18_s(s16, UnkVec *, u8 *, s16) __asm__("func_8009AF18");
void func_8008FA7C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g1;
    u8 *q;
    u8 *t0;
    u8 *t1;
    u8 *t2;
    void *s;
    s16 r;

    switch (p[0x9B])
    {
    case 0:
        b->unk_14 = (s32)0xFFEA0000;
        t0 = D_800DD0C8;
        *(u8 **)(c + 0x2C) = t0;
        func_80048A44(c, t0[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        p[0x9B]++;
        break;
    case 1:
        if (--*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        r = func_8009AF18_s(*(s16 *)(d + 0x2A), b, c, 8);
        if (r != 0)
        {
            q = *(u8 **)(p + 0x124);
            ((u32 *)q)[7] |= 0x200000;
            *(s16 *)(*(u8 **)(p + 0x124) + 0x8A) = r;
            (*(u8 **)(p + 0x124))[0x84] = 0x78;
            (*(u8 **)(p + 0x124))[0x85] = 2;
            ((u32 *)d)[7] &= ~0x100000;
            s = *(void **)D_80081484;
            *(void **)D_80081484 = NULL;
            g1 = (u8 *)&D_80083460;
            ((s16 *)g1)[5] += 1;
            t1 = D_800DCFD0;
            *(u8 **)(c + 0x2C) = t1;
            D_800E3540 = s;
            func_80048A44(c, t1[((D_80083228 + ((s16 *)d)[0x15] + 0x100) >> 9) & 7], 5, 1);
        }
        p[0x9B]++;
        break;
    case 2:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        if ((*(u32 *)(d + 0x1C) & 0x100000) == 0)
        {
            t2 = D_800DD060;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        }
        p[0x9B]++;
        break;
    case 3:
        g = (u8 *)&D_80083460;
        if (*(s16 *)(g + 0xA) != 0)
        {
            break;
        }
        *(u16 *)(g + 0x2) |= 0x412;
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
        if ((*(u32 *)(d + 0x1C) & 0x100000) != 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
            *(u8 **)(c + 0x2C) = D_800DD0B8;
        }
        else
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            *(u8 **)(c + 0x2C) = D_800DCFB0;
        }
        func_80048A44(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        p[0x9B]++;
        break;
    }
}

/* The state-0x22 glide: ease the mover onto the tile at c[0x24]/c[0x25] over
   the engine's shared countdown at D_80083460 + 4, re-skin from D_800DD0E0
   unless the view's +0xA2 says not to, and on the last frame stop the mover,
   place it and hand over to func_80094F74 (which may park func_8008EAC8).

   THE RESET IS WRITTEN AS A DUPLICATED `if` ARM, and that is a DEVICE. The
   whole residue was four words: the target fills the first division's
   `mflo` delay with the SECOND division's `lw 0x4($s0)`, and every spelling
   reachable from the source leaves a `nop` there -- struct or cast on both
   the loads and the stores, the subtrahend hoisted into a local, the
   numerator as its own statement, all four at 114/4. The duplicated arm
   puts a CODE_LABEL where the block has to split, and the load is then free
   to rise. jump2 cross-jumps the arms back together and deletes the test,
   so the emitted code carries no trace of it.

   This is the THIRD independent sighting of this device in this unit --
   func_8009704C (a cse-region boundary against an unwanted common
   subexpression) and func_800C0230 (a reorg boundary against a stolen
   delay slot) are the other two, and all three were found by
   decomp-permuter after hand sweeps ran out. Here it took 211 candidates.
   114 words, 0 differing. */
void func_8008FD50(u8 *p, UnkVec *v, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *t;
    s32 k;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        if (c != NULL)
        {
            *(s16 *)(g + 0x4) = 0;
        }
        else
        {
            *(s16 *)(g + 0x4) = 0;
        }
    }
    if (*(s16 *)(g + 0x4) != 0)
    {
        v->unk_0C = ((((c[0x24] << 6) + 0x20) << 16) - v->unk_00) / *(s16 *)(g + 0x4);
        v->unk_10 = ((((c[0x25] << 6) + 0x20) << 16) - v->unk_04) / *(s16 *)(g + 0x4);
    }
    if ((*(u16 *)(p + 0xA2) & 0x100) != 0 && *(s16 *)(g + 0x4) == 6)
    {
        func_8008D94C(p, v, c, d);
        return;
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        if (*(u8 **)(c + 0x2C) != D_800DD0E0)
        {
            t = D_800DD0E0;
            *(u8 **)(c + 0x2C) = t;
            k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            func_80048A44(c, t[k], 0, 1);
        }
    }
    g2 = (u8 *)&D_80083460;
    *(s16 *)(g2 + 0x4) = *(u16 *)(g2 + 0x4) - 1;
    if (*(s16 *)(g2 + 0x4) <= 0)
    {
        *(s16 *)(g2 + 0x4) = 0;
        v->unk_14 = 0;
        v->unk_10 = 0;
        v->unk_0C = 0;
        func_800A2B04(v, c[0x24], c[0x25]);
        if ((s16)func_80094F74(p, v, c, d) > 0)
        {
            *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
        }
    }
}

/* The three-way view stepper's THIRD state table, and func_8008E264 (:2408)
   is its line-for-line template: the same 0/8/10, 1/9/11 and 12 case groups
   over the same D_80083460 countdown, the same -0x100000 / -0x80000 /
   -0x140000 fall rates, the same `*((s16 *)g + 2)` divisor and the same
   case-12 clear. 186 words, 0 differing, FIRST COMPILE from that template.

   What differs from func_8008E264, read off the listing: the skin table in
   0/8/10 is picked by the record's +0x1C 0x100000 flag (D_800DD0C8 against
   D_800DD040) and so HAS to be bound to a local, with the func_80048A44
   argument re-reading `*(u8 **)(c + 0x2C)` -- which is what the target's
   `lw $v1, 0x2C($a0)` after the two func_80099F.. calls is; there is no
   closing func_800A56E0; the 1/9/11 group tests the same 0x100000 flag and
   hands over to func_8008EAC8 before the `p[0x9B] >= 0xA` re-skin; and the
   re-skin writes `*(s16 *)(g + 0x4) = 1` BEFORE `p[0x9B]++`, because the
   increment is the tail cross-jumping shares with the 0/8/10 group's.

   The jump table is jtbl_80088848 at 0x80088848, 0 mod 8, so cc1psx's
   `.align 3` is a no-op -- it is NOT one of the seven blocked tables.
   seqcheck reports the two documented benign relocation differences and
   nothing else: the table moving from `jtbl_80088848` to a `.rodata`
   section-relative reloc, and the case-12 store relocating against
   D_80083460 with +4 in the displacement where splat names D_80083464,
   exactly as func_8008E264 and func_800971A8 already do. */
void func_8008FF18(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *t;
    s16 n;
    s32 x;
    s32 y;

    switch (p[0x9B])
    {
    case 0:
    case 8:
    case 10:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            if (p[0x9B] == 0)
            {
                b->unk_14 = -0x100000;
            }
            else if (p[0x9B] == 8)
            {
                b->unk_14 = -0x80000;
            }
            else
            {
                b->unk_14 = -0x140000;
            }
            if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
            {
                t = D_800DD0C8;
            }
            else
            {
                t = D_800DD040;
            }
            *(u8 **)(c + 0x2C) = t;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 0x812;
            *(u16 *)(p + 0x98) &= 0xFFF3;
            func_80048A44(c, (*(u8 **)(c + 0x2C))[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
    case 9:
    case 11:
        g = (u8 *)&D_80083460;
        if (*((s16 *)g + 2) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *((s16 *)g + 2);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *((s16 *)g + 2);
        }
        n = *(u16 *)(g + 0x4) - 1;
        *(s16 *)(g + 0x4) = n;
        if (n <= 0)
        {
            *(s16 *)(g + 0x4) = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
            {
                if ((s16)func_80094F74(p, b, c, d) > 0)
                {
                    *(u32 *)(p + 0x8C) = (u32)func_8008EAC8;
                }
            }
            else if (p[0x9B] >= 0xA)
            {
                *(u8 **)(c + 0x2C) = D_800DD058;
                func_80048A44(c, D_800DD058[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
                *(s16 *)(g + 0x4) = 1;
                p[0x9B]++;
            }
            else if ((s16)func_80094F74(p, b, c, d) > 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    case 12:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 0;
            if ((s16)func_80094F74(p, b, c, d) > 0)
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    }
}

/* Installer: state 0xD, handler and life cleared, one flag bit dropped at the
   view's +0x1C, and the eight-way facing picked out of D_800DD0E8. THE WHOLE
   INDEX IN ONE EXPRESSION, exactly as func_8008CCB0 records it -- split in
   two the table base lands in $v1 and the two `lh` results swap registers.
   38 words, 0 differing. */
void func_80090200(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;

    p[0x9A] = 0xD;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 0;
    func_80094E34();
    *(s32 *)(d + 0x1C) &= ~0x20;
    t = D_800DD0E8;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_80048A44(c, t[v], 0, 1);
}

/* The stair-descent state machine off the actor's +0x9B, three cases and a
   shared `*(s16 *)(p + 0x96) += 1` after the switch. State 0 waits for the
   renderable's 0xE000 bits, re-skins from D_800DD0F0 -- indexed off
   D_80083160's +0xC8 bearing, not D_80083228 -- and installs func_8008B870;
   state 1 announces 0x50E or 0x50F every 0x20 frames, and once the engine's
   +0x02 flags are clear and the view's +0xA2 has 0x10 either re-skins from
   D_800DD0F8 and clears the handler, or tears the carrier down when
   func_80042900(d, 1) refuses; state 2 drops the +0x200 flag, tears the
   carrier down and parks func_8008ACDC.

   THE +0x96 INCREMENT IS AFTER THE SWITCH, not in each arm: .L80090514 is
   the `lhu $v0, 0x96($s2)` at word 159 and every `j` in the body targets it.
   Without it the function is 169 words of 172.

   Each of the four D_80083460 bases is ITS OWN LOCAL -- one shared local is a
   single global-alloc quantity in one register, which is the shape
   func_800CBDB4's comment block records for the same symbol.
   172 words, 0 differing. */
void func_80090298(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *m;
    u8 *n;
    s32 t;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u8 **)(c + 0x2C) = D_800DD0F0;
            func_80048A44(c, D_800DD0F0[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          0, 1);
            *(u32 *)(p + 0x8C) = (u32)func_8008B870;
            p[0x9B]++;
        }
        break;
    case 1:
        t = *(u16 *)(p + 0x96);
        if ((t & 0x1F) == 0)
        {
            func_800A56E0((t & 0x20) ? 0x50E : 0x50F);
        }
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x4) == 0 && (*(u16 *)(p + 0xA2) & 0x10) != 0)
        {
            if ((*(u32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0)
            {
                *(u16 *)(h + 0x2) |= 0x80;
            }
            if ((s16)func_80042900((s8 *)d, 1) == 0)
            {
                *(u8 **)(c + 0x2C) = D_800DD0F8;
                func_80048A44(c, D_800DD0F8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0,
                              1);
                *(u32 *)(p + 0x8C) = 0;
                k = (u8 *)&D_80083460;
                *(u16 *)(k + 0xA) += 1;
                p[0x9B]++;
            }
            else
            {
                func_80099F70(*(u8 **)(d + 0x5C));
                func_80099F04(*(u8 **)(d + 0x5C));
                m = (u8 *)&D_80083460;
                *(u16 *)(m + 0x2) |= 0x812;
            }
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u32 *)(d + 0x1C) &= ~0x200;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            n = (u8 *)&D_80083460;
            *(u16 *)(n + 0x2) |= 0x812;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            *(u16 *)(n + 0xA) -= 1;
        }
        break;
    }
    *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) + 1;
}

/* State 0x21's four-step switch on the view's +0x9B, and case 0 FALLS THROUGH
   into case 1 -- case 0's body ends on the `sb` and case 1's label is the
   next word, with no jump between, exactly as func_80091958 records.

   THE TWO HALFWORD ARGUMENTS OF func_800BCB04 MUST BE READ INTO LOCALS
   BEFORE THE D_800DCF58 STORE. Written inline in the call, the `sb` is
   scheduled to the head of the inter-call region and reorg puts the
   `addiu $a2, -0x400` in the `jal`'s delay slot instead of the store:
   129 words, 6 differing (words 57-63 permuted). With `s32 x`/`s32 y`
   assigned ahead of `D_800DCF58 = 1`, the store is the last insn before
   the call and takes the slot. Moving the store before func_800A2B04
   instead is 128 words / 79 differing; a `do {} while (0)`, a read-back,
   an `__asm__` memory barrier and a pointer local for the flag are all
   byte-identical to the inline form (same md5), so the lever is where the
   ARGUMENTS are read, not how the store is spelled.
   129 words, 0 differing. */
void func_80090548(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 x;
    s32 y;

    switch (p[0x9B])
    {
    case 0:
        *(u16 *)(d + 0x6A) = *(u16 *)(d + 0x2A) & 0xFFF;
        *(u32 *)(d + 0x1C) &= 0xFFFEFFFF;
        p[0x9B]++;
    case 1:
        *(u16 *)(d + 0x2A) += 0x200;
        b->unk_14 -= 0x10000;
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) <= 0)
        {
            b->unk_14 = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            x = *(u16 *)((u8 *)b + 0x2);
            y = *(u16 *)((u8 *)b + 0x6);
            D_800DCF58 = 1;
            *(s16 *)(d + 0x88) = func_800BCB04(x, y, -0x400);
            *(s16 *)(p + 0x92) = -0x200;
            *(s16 *)(p + 0x96) = 0x10;
            *(u16 *)(p + 0x98) &= 0xFFF7;
            p[0x9B]++;
        }
        break;
    case 2:
        *(u16 *)(d + 0x2A) += 0x200;
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0)
        {
            *(s16 *)(p + 0x96) = 0x20;
            p[0x9B]++;
        }
        break;
    case 3:
        *(u16 *)(d + 0x2A) += 0x200;
        if ((*(u16 *)(d + 0x2A) & 0xFFF) == (*(u16 *)(d + 0x6A) & 0xFFF))
        {
            func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            *(u16 *)(d + 0x2A) = *(u16 *)(d + 0x6A);
            *(u32 *)(d + 0x1C) &= 0xFFFEFFFF;
            func_80096088((UnkDungeonActor *)p, d);
        }
        break;
    }
}

/* Turn the engine's D_80083168 top-nibble command into a facing delta for the
   caller's +0x2A word: eight of the twelve commands set the delta to
   `(K - a) * 0x200` with K running 0..7, four of those only when the word
   does not already carry the command's own nibble, and the other four just
   clear the nibble. When a reference angle is given, the delta is folded
   into its 12-bit range and clamped to one 0x200 step per call, with bit
   0x400 of the caller's word marking that the turn is still running.
   149 words, 0 differing.

   THE COMMAND WORD IS A `u32` LOCAL: `f >> 12` on an `s32` is `sra` where
   the target has `srl`, which is this function's only difference under every
   other spelling (149 words, 1 differing).
   THE CASES ARE WRITTEN IN LAYOUT ORDER -- 2, 6, 4, 12, 8, 9, 1, 3, default
   -- because cc1psx emits a switch's bodies in SOURCE order, and cases 1, 4
   and 8 then cross-jump onto case 1's `subu`/`sll` tail.
   THIS FUNCTION OWNS jtbl_80088880 AT 0x80088880, WHICH IS 0 MOD 8, so
   cc1psx's `.align 3` is a no-op and the table lands where splat's copy did
   -- the same screen func_80095538 passed. `seqcheck` reports
   `HI16 jtbl_80088880` against `HI16 .rodata`, which is the documented
   benign relocation class for a compiled jump table. */
s32 func_8009074C(s16 a, u16 *b, u16 *c)
{
    u32 f;
    s32 t;
    s32 r;
    s32 cv;
    s32 cu;
    s32 w;

    r = -1;
    f = *(u16 *)D_80083168;
    t = f & 0xF000;
    if ((f & 0x10) == 0)
    {
        *b &= 0xFFF;
    }
    switch (f >> 12)
    {
    case 2:
        if ((*b & t) == 0)
        {
            r = (0 - a) << 9;
        }
        break;
    case 6:
        r = (1 - a) << 9;
        *b = (*b & 0xFFF) | t;
        break;
    case 4:
        if ((*b & t) == 0)
        {
            r = (2 - a) << 9;
        }
        break;
    case 12:
        r = (3 - a) << 9;
        *b = (*b & 0xFFF) | t;
        break;
    case 8:
        if ((*b & t) == 0)
        {
            r = (4 - a) << 9;
        }
        break;
    case 9:
        r = (5 - a) << 9;
        *b = (*b & 0xFFF) | t;
        break;
    case 1:
        if ((*b & t) == 0)
        {
            r = (6 - a) << 9;
        }
        break;
    case 3:
        r = (7 - a) << 9;
        *b = (*b & 0xFFF) | t;
        break;
    default:
        *b &= 0xFFF;
        break;
    }
    *b &= 0xFBFF;
    if (c != NULL && (s16)r != -1)
    {
        w = *c;
        *c = (w & 0x800) ? (w | 0xF800) : (w & 0x7FF);
        r = (r & 0x800) ? (r | 0xF800) : (r & 0x7FF);
        cv = *(s16 *)c;
        cu = *c;
        if (abs(cv - (s16)r) > 0x800)
        {
            r = (cu & 0xF000) | (r & 0xFFF);
        }
        if (cv < (s16)r)
        {
            *b |= 0x400;
            r = cu + 0x200;
        }
        else if ((s16)r < cv)
        {
            *b |= 0x400;
            r = cu - 0x200;
        }
    }
    return r & 0xFFF;
}

/* The door step: state 0 and state 1 re-skin the renderable off D_800DD008
   and D_800DD010 at the camera-relative facing, state 1 then clears
   D_80083460's 0x100 bit and -- if func_80094EA4 agrees -- runs the actor's
   own +0x9A step out of D_800DD168, and state 2 does the same and otherwise
   parks the actor in state 0xE back on func_8008ACDC. 156 words, 0 differing.

   THE SKIN TABLE IS WRITTEN AT BOTH SITES, NOT THROUGH A LOCAL. With
   `t = D_800DD008; *(u8 **)(c + 0x2C) = t; ... t[...]` the `lui`/`addiu` pair
   is split across two registers and the facing sum takes $v1 where the target
   has $a1 -- 18 differing, every one of them a register. Written out twice,
   cse shares the one address, local-alloc ties the `high` and the `lo_sum`
   into $v1, and the facing lands in the argument register it is about to
   become. An index local, the reversed operand order and a `u8 *` base for
   D_80083160 are all 18 differing as well.
   TWO `u8 *` LOCALS FOR &D_80083460, one per state, for the usual reason:
   state 1 holds it across func_80094EA4 and state 2 does not. */
void func_800909A0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    Unk80083160 *g;
    u8 *h;
    u8 *h2;

    g = &D_80083160;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u8 **)(c + 0x2C) = D_800DD008;
            func_80048A44(c, D_800DD008[((g->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        if (*(u16 *)((u8 *)g + 0x8) == 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_800DD010;
        func_80048A44(c, D_800DD010[((g->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        p[0x9B]++;
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0x2) &= 0xFEFF;
        if ((s16)func_80094EA4() == 0)
        {
            break;
        }
        if (*(s16 *)(h + 0xA) != 0)
        {
            *(u16 *)(h + 0x2) |= 4;
        }
        func_8008ACDC(p, b, c, d);
        if (p[0x9A] != 0x17)
        {
            D_800DD168[p[0x9A]](p, b, c, d);
        }
        break;
    case 2:
        if ((s16)func_80094EA4() != 0)
        {
            h2 = (u8 *)&D_80083460;
            if (*(s16 *)(h2 + 0xA) != 0)
            {
                *(u16 *)(h2 + 0x2) |= 4;
            }
            func_8008ACDC(p, b, c, d);
            if (p[0x9A] != 0x17)
            {
                D_800DD168[p[0x9A]](p, b, c, d);
                break;
            }
        }
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            p[0x9A] = 0xE;
            *(s16 *)(p + 0xA4) = 0;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
        break;
    }
}

/* The 0x9B state machine's dispatcher: an outer `switch (p[0x9B])` whose
   cases 0 and 1 fall through into 2, and inside case 2 a second switch over
   D_80082EB8 (dense 1..21; 11, 13 and 15 are absent from the source, so their
   jtbl_800888B0 slots point at the default). The arms are written in the
   order the listing lays them out -- 1, 2, 21, 3/4/20, 5, 8/9, 10, 14, 12,
   16, 6/7, 17/18, 19 -- because cc1psx emits switch bodies in SOURCE order.

   EVERY ARM THAT HOLDS A VALUE NEEDS ITS OWN FUNCTION-SCOPE LOCAL. Disjoint
   arms sharing one local are one global allocno with one hard register, and
   here that is most of the residue: letting case 1's dispatch result share
   `r` with case 3/4/20's cost 521 words and 29 aligned differences (case 1
   gained a pointless `move s1,v0`); letting case 12's two record loads share
   case 0's `e` cost 6 and 7 more. Split, they land in $a0/$v1 as the target
   has them.

   The other three that were measured one at a time, each from this body:
   * `t = (u8 *)&D_80082EB0;` then `*(u16 *)(t + 8)` in case 6/7. Spelled
     `((u16 *)&D_80082EB0)[4]` the address folds to the constant
     `D_80082EB0 + 8` and gcc biases one register by 8 and reads the triple's
     first word back at -8: 5 aligned differences. A pointer local forces
     &D_80082EB0 itself into a register, which is what `addiu $v0, $v1,
     %lo(D_80082EB0)` / `lhu 0x8($v0)` / `lw %lo(D_80082EB0)($v1)` is.
   * `((UnkDungeonActor *)p)->unk_C8 = NULL;` -- the ONE store in case 2 that
     is spelled through the struct. sched.c's true_dependence drops the
     conflict only when the varying-address side has MEM_IN_STRUCT_P and the
     symbol side does not, so through `*(s32 *)(p + 0xC8)` the D_800E296C load
     cannot be lifted over it: 2 aligned differences. (Putting the bit on
     D_800E296C instead, via an `__asm__` array alias, changes nothing -- it
     is the wrong side of the test.)
   * `extern u8 D_800DDCBC[][8]` with `D_800DDCBC[g[0x13]][7]`: as a flat
     array `[g[0x13] * 8 + 7]` folds the +7 into the %lo and the `lbu` loses
     its 0x7 displacement, 1 aligned difference.

   Case 8/9's two message arms are written out in full and cc1psx cross-jumps
   their shared tail to .L800911E4 by itself; the test is spelled `== 0` so
   the D_800E069D arm is emitted first, which is the layout the target has
   (`!= 0` with the arms the other way round costs 14). Case 3/4/20 really
   does test the sign of the same value twice, `bgtz` then `bgez`.
   520 words, 0 differing. */
void func_80090C10(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *g;
    u8 *m;
    u8 *t;
    u8 *h;
    u8 *h0;
    u8 f;
    s32 r;
    s32 q;
    s32 i;
    s32 k;
    s32 v;
    s32 x;
    s16 sx;
    s16 sy;

    switch (p[0x9B])
    {
    case 0:
        D_800DCF4D = -1;
        func_8004B568();
        if (D_800E4938 != D_8004F5F4)
        {
            if ((s16)func_80042900((s8 *)d, 0xA) != 0)
            {
                *(void **)(c + 0x2C) = D_800DD274;
                func_8003DB94(c,
                              *(void **)((u8 *)D_800DD274 +
                                         (((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 7) & 0x1C)),
                              0);
            }
            else
            {
                e = ((*(s32 *)(d + 0x1C) & 0x100000) != 0) ? D_800DD0B8 : D_800DCFB0;
                *(u8 **)(c + 0x2C) = e;
                func_80048A44(c, e[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
        }
        p[0x9B]++;
        /* fallthrough */
    case 1:
        q = ((s32 (*)(u32, u32))D_800E4938_a[0])(D_800E4938_a[1], D_800E4938_a[2]);
        *(s32 *)(p + 0xC8) = q;
        if (q == 0)
        {
            return;
        }
        *(s16 *)(p + 0x96) = 0;
        D_80082EB0 = NULL;
        ((u32 *)&D_80082EB0)[1] = 0;
        ((u32 *)&D_80082EB0)[2] = 0;
        p[0x9B]++;
        /* fallthrough */
    case 2:
        if ((*(u16 *)(*(u8 **)(p + 0xC8) + 0x1E) & 0x8000) != 0)
        {
            func_800BA810(NULL, 0);
            ((UnkDungeonActor *)p)->unk_C8 = NULL;
            D_800E296C &= ~0x2000;
        }
        if (*(s32 *)(p + 0xC8) != 0)
        {
            return;
        }
        switch (D_80082EB8)
        {
        case 1:
            func_8009F644(d, 0x48, (s16)func_80098C80(D_80082EB0), 0);
            if (D_80082EB0[1] == 0x11)
            {
                func_80098614(d, D_80082EB0);
            }
            else
            {
                func_800982A8(d, D_80082EB0);
            }
            break;
        case 2:
            if (D_80082EB0[1] == 0x11)
            {
                func_8009F644(d, 0x48, (s16)func_80098C80(*(u8 **)(d + 0x50)), 0);
                func_80098614(d, NULL);
            }
            else
            {
                func_8009F644(d, 0x48, (s16)func_80098C80(*(u8 **)(d + 0x4C)), 0);
                func_800982A8(d, NULL);
            }
            break;
        case 21:
            if ((D_80082EB0[3] & 0x20) != 0)
            {
                func_800997FC(D_800E0542);
                break;
            }
            if (func_800BA33C(*(u8 **)(p + 0xAC)) != 0 || func_800BA33C(*(u8 **)(p + 0xB0)) != 0)
            {
                func_80091920((UnkDungeonActor *)p, b, c, d);
                return;
            }
            func_800997FC(D_800E0571);
            break;
        case 3:
        case 4:
        case 20:
            r = func_80098920(d, D_80082EB0, 3, 0);
            if (r >= 0)
            {
                x = (s16)func_80098C80(D_80082EB0);
                func_8009F644(d, 0x48, x, (s16)func_80098C80(((u8 **)&D_80082EB0)[1]));
                if (r > 0)
                {
                    break;
                }
                if (r >= 0)
                {
                    return;
                }
            }
            if (D_80082EB0[1] == 0x15)
            {
                p[0x9B] = 3;
            }
            return;
        case 5:
            func_8009F644(d, 0x58, 0, (s16)func_80098C80(D_80082EB0));
            func_80098CF8(p, b, c, D_80082EB0);
            break;
        case 8:
        case 9:
            func_8009F644(d, 0x98, 0, (s16)func_80098C80(D_80082EB0));
            if ((D_80082EB0[3] & 0x20) != 0)
            {
                func_800956B8(p, b, c, D_80082EB0);
                break;
            }
            i = (*(u16 *)(d + 0x2A) >> 8) & 0xE;
            if (func_8009B88C(NULL, (s16)(c[0x24] + *(u16 *)((u8 *)D_8006CCD8 + i)),
                              (s16)(c[0x25] + *(u16 *)((u8 *)D_8006CCE8 + i)), &sx, &sy) == 0)
            {
                h0 = func_800990FC();
                h = func_80099194(D_800E069D, h0);
                h = func_80099734((u8 *)D_800E3DF0[D_80082EB0[3] & 0x1F], h);
                h = func_80099194(D_800E06BD, h);
            }
            else
            {
                if (func_80095854((UnkDungeonActor *)p, b, c, D_80082EB0) != 0)
                {
                    return;
                }
                h0 = func_800990FC();
                h = func_80099194(D_800E06C0, h0);
                h = func_80099734((u8 *)D_800E3DF0[D_80082EB0[3] & 0x1F], h);
                h = func_80099194(D_800E06E0, h);
            }
            func_80099290(h);
            func_800A5720(h0);
            break;
        case 10:
            func_8002534C(p, b, c, d);
            *(s32 *)(p + 0xC8) = 0;
            return;
        case 14:
            func_8008D330((UnkDungeonActor *)p, b, c, d);
            *(s32 *)(p + 0xC8) = 0;
            break;
        case 12:
            m = ((u8 **)(p + 0xAC))[(s32)D_80082EB0];
            if ((*(s32 *)(m + 0x1C) & 0x80000) == 0)
            {
                *(s32 *)(m + 0x60) = 0;
            }
            if (D_800E4938 == D_8001EF2C)
            {
                break;
            }
            g = ((u8 **)(p + 0xAC))[(s32)D_80082EB0];
            k = *(u16 *)(g + 0x46) & 0x3FFF;
            if (k >= 5 && k <= 7)
            {
                break;
            }
            if (k >= 9 && k <= 10)
            {
                f = D_800DDCBC[g[0x13]][7];
                if ((f & ((k == 9) ? 1 : 2)) != 0)
                {
                    break;
                }
            }
            if (func_8008D024(p, b, c, (s16)(s32)D_80082EB0, 1) != 0)
            {
                return;
            }
            break;
        case 16:
            *(u8 **)(p + 0xBC) = D_80082EB0;
            if (func_8008D388(p, b, c, d) != 0)
            {
                return;
            }
            break;
        case 6:
        case 7:
            t = (u8 *)&D_80082EB0;
            v = *(u16 *)(t + 8) - 6;
            *(s16 *)(d + 0x8A) = v;
            if (func_80094270(p, b, c, D_80082EB0, (s16)v) == 0)
            {
                return;
            }
            break;
        case 17:
        case 18:
            func_80097F84(D_80082EB0, D_800E0462, D_800E0472, 1);
            break;
        case 19:
            func_8008D9F0(p, b, c, d);
            return;
        }
        D_80082EB8 = 0;
        if (*(s32 *)(p + 0xC8) == 0)
        {
            *(u16 *)(p + 0xA2) |= 0x40;
            func_80096088((UnkDungeonActor *)p, d);
        }
        break;
    }
}

/* The engine-dialogue state machine on +0x9B: state 0 opens the window
   (func_800B9964 off +0x104, the mode beep, and one of three rodata scripts),
   state 1 runs it -- re-aims the record's +0x2A facing through func_8009074C,
   then either closes on D_80083160+0x10's 0x80/0x10/0x20/0x40 bits, re-targets
   from the engine's +0xAC/+0xB0 slot pair on D_80083160+8, or (when +0x120 is
   set) tears the window down and parks func_8008EAC8 / func_8008ACDC -- and
   state 2 is the bare teardown. 316 words, 0 differing, with one device.

   THE SIXTH ARGUMENT'S READ OF D_80082EB0 IS A `volatile` CAST, AND THAT IS A
   DEVICE worth the function's last word. The target clears the global and
   RELOADS it one insn later into the outgoing argument slot; cse1 records the
   MEM dest with the stored constant's class (cse.c:7402) and forwards it, so
   every ordinary spelling is 315 words / 9 aligned differences with
   `sw $zero, 0x14($sp)` where the target has `lw` + `sw $v1`. A volatile READ
   is refused a hash lookup, so it is never substituted, while the address is
   still an ordinary pseudo and the `lui %hi(D_80082EB0)` stays shared between
   the store and the load -- which is why this and not a cse-region boundary:
   the target materialises that `high` ONCE.
   docs/matching-levers.md:26412 records that `extern u8 *volatile D_80082EB0;`
   also buys the word (314 of 316) but takes the landed func_8008DB0C to
   56 words / 24 differing; the cast is the same lever confined to one read, and
   func_8008DB0C, func_8009402C, func_80090C10 and func_80094270 all re-measure
   at 0 differing beside it.
   MEASURED AND REJECTED: moving `D_80082EB0 = NULL;` ABOVE the +0x8A store so
   that store invalidates the entry does not work and costs 4 more -- 315 words
   / 13 differing, with the `lui`/`sw` pair six words early. The reason is in
   `note_mem_written` (cse.c:7606): a varying-address store whose address is a
   PLUS and whose mode is not QImode sets only `nonscalar`, and
   `invalidate_memory` drops an in-memory entry only when it is `in_struct` or
   its address varies -- D_80082EB0's is neither.

   THE THREE SCRIPTS ARE THREE SEPARATE CALLS to func_8004DD2C, not one call on
   a selected pointer. Through a local assigned in three arms each symbol's
   `high` lands in $v0 and only its `lo_sum` in $a0; three calls let
   cross-jumping merge at the `jal` and leave `lui $a0` / `addiu $a0` in every
   arm, which is the target. One call: 315 words, 11 differing.
   (docs/matching-levers.md:26384, read for an argument rather than a ternary.)

   THE +0x104 CLEAR IN THE 0x80 ARM IS AN ARRAY REF, `((void **)p)[0x41]`.
   `a[i]` on a pointer is an INDIRECT_REF of a PLUS_EXPR, which expand_expr
   marks MEM_IN_STRUCT_P; `*(s32 *)(p + 0x104)` is an INDIRECT_REF of the CAST,
   which is not marked. Only with the bit set does true_dependence let the
   `lw %lo(D_800E296C)` rise over the store, and the store land between the
   `addiu $v1, $zero, -0x2001` and the `and` -- 316 words, 2 differing without
   it. It is func_80090C10's `((UnkDungeonActor *)p)->unk_C8` rule reached
   without extending the struct; the other three arms need no such spelling
   because nothing is scheduled across them.

   func_800B9964's result NEEDS ITS OWN LOCAL, separate from the +0xAC/+0xB0
   slot read in the retarget tail: one shared local is one allocno, it takes
   $a0, and state 0 gains an `addu $a0, $v0, $zero` -- 317 words, 8 differing.

   The record pointer is re-spelled `*(u8 **)(p + 0x104)` at every site rather
   than taken into a local: cse hands the three uses before func_80094208 one
   `lw $t0` and re-reads it after each call, which is what the target does and
   what a local in a callee-saved register would not. */
void func_80091430(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *q;
    u8 *n;
    u8 **s;
    s16 r;
    s16 x;
    s16 y;
    s16 k;

    g = (u8 *)&D_80083160;
    switch (p[0x9B])
    {
    case 0:
        n = (u8 *)func_800B9964(p + 0x104);
        *(u8 **)(p + 0xC8) = n;
        if (n == NULL)
        {
            return;
        }
        func_8004DCE0(2);
        func_8004DCEC();
        if (*(s16 *)(p + 0x120) == 0)
        {
            if (D_80012D6D == 0)
            {
                func_8004DD2C(D_80088904);
            }
            else
            {
                func_8004DD2C(D_80088934);
            }
        }
        else
        {
            func_8004DD2C(D_80088970);
        }
        p[0x9B]++;
        /* fallthrough */
    case 1:
        r = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2),
                          (u16 *)(*(u8 **)(p + 0x104) + 0x2A));
        if (r != 0xFFF)
        {
            *(s16 *)(*(u8 **)(p + 0x104) + 0x2A) = r;
            func_8009F644(d, 0x70, *(u8 **)(p + 0x104) == *(u8 **)(p + 0xB0), (r >> 9) & 7);
        }
        if (*(s16 *)(p + 0x120) == 0)
        {
            if (*(u8 **)(p + 0x104) != NULL)
            {
                if ((*(s32 *)(g + 0x10) & 0x80) != 0)
                {
                    if (func_80094208(0) != 0)
                    {
                        return;
                    }
                    if ((*(s32 *)(d + 0x1C) & 0x100000) != 0)
                    {
                        return;
                    }
                    *(s16 *)(d + 0x8A) = *(u8 **)(p + 0x104) != *(u8 **)(p + 0xAC);
                    D_80082EB0 = NULL;
                    /* DEVICE: the `volatile` cast on this one read stops cse1
                       forwarding the zero just stored; worth the function's
                       last word (315 of 316 without it) and it emits nothing.
                       The head comment has the mechanism and the rejected
                       spellings. */
                    if (func_8009402C(p, (s32)b, c, &x, &y, *(u8 *volatile *)&D_80082EB0) != 0)
                    {
                        func_800997FC(D_800E2004);
                    }
                    else
                    {
                        func_8004E130();
                    }
                    *(s32 *)(p + 0xC8) = 0;
                    func_8008DB0C((UnkDungeonActor *)p, b, c, x, y);
                    ((void **)p)[0x41] = NULL;
                    D_800E296C &= ~0x2000;
                    return;
                }
                else if ((*(s32 *)(g + 0x10) & 0x10) != 0)
                {
                    func_800C77D0(*(u8 **)(p + 0x104) - 0x20,
                                  *(u8 **)(*(u8 **)(p + 0x104) - 0x18), 8, D_800DCE66);
                    k = *(u8 **)(p + 0x104) != *(u8 **)(p + 0xAC);
                    *(s16 *)(d + 0x8A) = k;
                    D_800E4940 = k;
                    func_8008CF6C((UnkDungeonActor *)p, b, c, D_8001EF2C);
                    D_80082EB0 = NULL;
                    *(s32 *)(p + 0xC8) = 0;
                    *(s32 *)(p + 0x104) = 0;
                    func_8004E130();
                    return;
                }
                else if ((*(s32 *)(g + 0x10) & 0x20) != 0)
                {
                    k = *(u8 **)(p + 0x104) != *(u8 **)(p + 0xAC);
                    *(s16 *)(d + 0x8A) = k;
                    D_800E4940 = k;
                    func_8008CF6C((UnkDungeonActor *)p, b, c, D_80023C58);
                    D_80082EB0 = NULL;
                    *(s32 *)(p + 0xC8) = 0;
                    *(s32 *)(p + 0x104) = 0;
                    func_8004E130();
                    return;
                }
                else if ((*(s32 *)(g + 0x10) & 0x40) != 0)
                {
                    func_800C77D0(*(u8 **)(p + 0x104) - 0x20,
                                  *(u8 **)(*(u8 **)(p + 0x104) - 0x18), 8, D_800DCE66);
                    k = *(u8 **)(p + 0x104) != *(u8 **)(p + 0xAC);
                    *(s16 *)(d + 0x8A) = k;
                    D_800E4940 = k;
                    func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
                    D_80082EB0 = NULL;
                    *(s32 *)(p + 0xC8) = 0;
                    *(s32 *)(p + 0x104) = 0;
                    func_8004E130();
                    return;
                }
            }
            if (*(u8 **)(p + 0x104) == *(u8 **)(p + 0xAC))
            {
                if ((*(s32 *)(g + 0x8) & 0x2) != 0)
                {
                    return;
                }
            }
            else if (*(u8 **)(p + 0x104) == *(u8 **)(p + 0xB0))
            {
                if ((*(s32 *)(g + 0x8) & 0x1) != 0)
                {
                    return;
                }
            }
            if ((*(s32 *)(g + 0x8) & 0x3) != 0)
            {
                if ((*(s32 *)(g + 0x8) & 0x2) != 0)
                {
                    s = (u8 **)(p + 0xAC);
                }
                else
                {
                    s = (u8 **)(p + 0xB0);
                }
                q = *s;
                if (q == NULL)
                {
                    return;
                }
                if (*(u8 **)(p + 0x104) == q)
                {
                    return;
                }
                if ((*(s32 *)(q + 0x1C) & 0x20000) == 0)
                {
                    return;
                }
                if ((*(s32 *)(q + 0x1C) & 0x80000) != 0)
                {
                    return;
                }
                *(u8 **)(p + 0x104) = q;
                func_800B0F50(q);
            }
            else
            {
                *(s32 *)(p + 0x104) = 0;
                p[0x9B]++;
            }
        }
        else
        {
            func_800BA810(*(u8 **)(p + 0x104), *(u16 *)(*(u8 **)(p + 0x104) + 0x46));
            if ((*(s32 *)(g + 0x10) & 0x60) != 0)
            {
                if ((*(s32 *)(g + 0x10) & 0x20) != 0)
                {
                    func_800A56E0(0x515);
                    *(u16 *)(*(u8 **)(p + 0x104) + 0x46) &= 0x7FFF;
                }
                else
                {
                    func_800A56E0(0x503);
                }
                *(s32 *)(p + 0x104) = 0;
                func_8004E130();
                D_800E296C &= ~0x2000;
                *(void **)(p + 0x8C) =
                    ((*(s32 *)(d + 0x1C) & 0x100000) != 0) ? func_8008EAC8 : func_8008ACDC;
                func_800BA810(NULL, 0);
            }
        }
        break;
    case 2:
        *(s32 *)(p + 0xC8) = 0;
        func_8004E130();
        D_800E296C &= ~0x2000;
        *(void **)(p + 0x8C) =
            ((*(s32 *)(d + 0x1C) & 0x100000) != 0) ? func_8008EAC8 : func_8008ACDC;
        break;
    }
}

void func_80091920(UnkDungeonActor *p)
{
    p->unk_9A = 0x29;
    p->unk_9B = 0;
    p->unk_8C = NULL;
}

/* State 0x2B; the fourth argument picks the sub-state byte.
   9 words, 0 differing. */
void func_80091934(UnkDungeonActor *p, void *b, void *c, void *d)
{
    p->unk_9A = 0x2B;
    if (d == NULL)
    {
        p->unk_9B = 0x10;
    }
    else
    {
        p->unk_9B = 0;
    }
    p->unk_8C = NULL;
}

/* A three-case switch on +0x9B whose case 0 FALLS THROUGH into case 1 --
   case 0's body ends on the `sb` and case 1's label is the next word, with no
   jump between them. 56 words, 0 differing. */
void func_80091958(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    switch (p[0x9B])
    {
    case 0:
        *(u32 *)(p + 0xC8) = func_800BA074(D_80082EB0);
        if (*(u32 *)(p + 0xC8) == 0)
        {
            break;
        }
        p[0x9B]++;
    case 1:
        if ((*(u16 *)(*(u8 **)(p + 0xC8) + 0x1E) & 0x8000) != 0)
        {
            *(u32 *)(p + 0xC8) = 0;
            p[0x9B]++;
        }
        break;
    case 2:
        D_80082EB8 = 0;
        *(u32 *)(p + 0x8C) =
            (*(u32 *)(d + 0x1C) & 0x100000) ? (u32)func_8008EAC8 : (u32)func_8008ACDC;
        break;
    }
}

/* States 0 and 0x10 run the handler word out of
   D_800DD6B8[s[1] - 1][s[0] - 1]. THE TWO INDICES NEED THEIR OWN LOCALS:
   written inline the `- 1` folds into the address as a -4 displacement and
   the function is 71 words against 73 with 60 differing; bound to `i` and `j`
   first, the target's `addiu -1` / `sll 2` pair survives for each.
   73 words, 0 differing. */
void func_80091A38(u8 *p, void *b, void *c, u8 *d)
{
    u8 *s;
    u32 w;
    s32 n;
    s32 i;
    s32 j;
    s32 (*fn)(u32, u8 *, s32, s32);
    u8 *g;

    if (p[0x9B] == 0 || p[0x9B] == 0x10)
    {
        s = *(u8 **)(p + 0x110);
        i = s[1] - 1;
        j = s[0] - 1;
        w = D_800DD6B8[i][j];
        n = w >> 24;
        fn = (s32 (*)(u32, u8 *, s32, s32))((w & 0xFFFFFF) | 0x80000000);
        if (n != 0)
        {
            func_800BCFBC(n - 1);
        }
        if (fn((u32)d | 0xA0000000, s, 0, 0) != 0)
        {
            *(u32 *)(p + 0x110) = 0;
            p[0x9B]++;
        }
    }
    else if (p[0x9B] == 1)
    {
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x2) |= 0x812;
        func_80096088((UnkDungeonActor *)p, d);
    }
}

/* State 0x2A behind the renderable's 0x8000 gate. THE ENGINE RECORD IS READ
   BEFORE THE +0x11A COPY, not after -- written the other way round the
   `lhu 0x2A($s3)` has nothing but its own `addiu` behind it, cc1psx emits the
   load-delay `nop` and the function is 67 words against 66 with 20 differing.
   66 words, 0 differing. */
void func_80091B5C(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;
    u8 *g;
    u8 *e;
    u16 x;

    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(s32 *)(d + 0x14) &= ~0x20000;
    }
    else
    {
        p[0x9A] = 0x2A;
        p[0x9B] = 0;
        *(s32 *)(p + 0x8C) = 0;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        *(s16 *)(p + 0x96) = 0x10;
        t = D_800DD030;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_80048A44(c, t[v], 0, 1);
        e = *(u8 **)(*(u8 **)(g + 0xC) - 0x14);
        *(s16 *)(p + 0x11A) = *(s16 *)(d + 0x2A);
        *(s16 *)(d + 0x2A) = func_800A0818(c[0x24], c[0x25], e[0x24], e[0x25], &x);
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088904);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088934);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088970);

/* Five states of the camera's pull-back-and-return: 0 runs the +0x96 timer
   down to 9 and re-skins the renderable off D_800DD038, 1 drives the mover
   out along the engine record's facing until the timer expires and re-skins
   off D_800DD058, 2 waits on D_8008346C and re-arms the timer at 8, 3 glides
   the mover back onto the renderable's (+0x24, +0x25) tile, and 4 restores
   the saved facing from +0x11A and hands the actor to func_8008EAC8 or
   func_8008ACDC on the record's 0x100000 bit. jtbl_800889A8 is 0 mod 8, so
   cc1psx's `.align 3` is a no-op. 237 words, 0 differing.

   THE TWO SKIN TABLES ARE NAMED TWICE AT EACH SITE, NOT BOUND TO A LOCAL --
   func_8008E264's rule, and the cost here is the same shape it records:
   with `t = D_800DD038; *(u8 **)(c + 0x2C) = t; func_80048A44(c, t[...])`
   the symbol's `high` takes $v0 and only its `lo_sum` takes $a1, and the
   `addu $a0, $s2, $zero` the target puts in the gate's delay slot is pushed
   two insns later. 237 words, 12 differing -- exactly three in each of the
   four re-skin blocks. Writing the symbol out at both sites is 0. */
void func_80091C64(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    switch (p[0x9B])
    {
    case 0:
        if (--*(s16 *)(p + 0x96) < 9)
        {
            *(u8 **)(c + 0x2C) = D_800DD038;
            func_80048A44(c, D_800DD038[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(p + 0x98) |= 0xC;
            p[0x9B]++;
            func_800A56E0(0x701);
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_00 += func_80064584(*(s16 *)(d + 0x2A) + 0x400) << 6;
            b->unk_04 += func_800644B8(*(s16 *)(d + 0x2A) + 0x400) << 6;
            *(s16 *)(p + 0x92) = -func_800644B8(*(s16 *)(p + 0x96) << 8) >> 8;
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_800DD058;
        func_80048A44(c, D_800DD058[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s16 *)(p + 0x92) = 0;
        p[0x9B]++;
        break;
    case 2:
        if (D_8008346C != 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_800DD038;
        func_80048A44(c, D_800DD038[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s16 *)(p + 0x96) = 8;
        p[0x9B]++;
        break;
    case 3:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_00 += (((c[0x24] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x2)) << 16) / *(s16 *)(p + 0x96);
            b->unk_04 += (((c[0x25] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x6)) << 16) / *(s16 *)(p + 0x96);
            *(s16 *)(p + 0x92) = -func_800644B8(*(s16 *)(p + 0x96) << 8) >> 8;
        }
        if (--*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        func_800A2B04(b, c[0x24], c[0x25]);
        *(s16 *)(p + 0x92) = 0;
        p[0x9B]++;
        *(u8 **)(c + 0x2C) = D_800DD058;
        func_80048A44(c, D_800DD058[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        break;
    case 4:
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(s16 *)(d + 0x2A) = *(s16 *)(p + 0x11A);
        *(u32 *)(p + 0x8C) = (*(u32 *)(d + 0x1C) & 0x100000) ? (u32)func_8008EAC8 : (u32)func_8008ACDC;
        *(s32 *)(d + 0x14) &= ~0x20000;
        *(u16 *)(p + 0x98) &= 0xFFF3;
        *(u16 *)(p + 0xA2) |= 0x10;
        break;
    }
}

/* State machine on the actor's +0x9B for the lift/platform ride: state 0
   either re-skins the slot from D_800DD028 (once) or, with the renderable's
   0xE000 bits clear, asks func_8009074C for a destination and hands the
   actor back to func_8008ACDC when it is a new facing and func_80094EA4
   agrees; state 16 sets the mover's two integer velocities from the engine's
   sine/cosine tables and runs the +0x96 countdown down into the engine's
   +0x124 record; state 17 re-skins from D_800DD020 and falls through into
   state 18, which glides the mover onto the tile over D_80083460+4, pulses
   func_800B653C every fourth frame and lands in state 0x13; state 19 runs the
   same glide off +0x96 and closes on func_800A2B04/func_80099F70/func_80099F04
   and D_80083460+2's 0x812 bits. jtbl_800889C0 is 0 mod 8, so cc1psx's
   `.align 3` is a no-op. 310 words, 0 differing.

   THE TWO GLIDE EXPRESSIONS TAKE DIFFERENT BIAS SPELLINGS, and a 20-point
   grid over the two measured every combination:

   - STATE 18's TABLE TERM AND ITS +0x20 MUST BE ONE LOCAL ASSIGNED INSIDE
     THE EXPRESSION, `(x = D_8006CCD8[k] * 16 + 0x20)`. Written flat as
     `A + T * 16 + 0x20 - B`, fold's associate moves the 0x20 onto the
     loaded position (`addiu $v1, -0x20` where the target has
     `addiu $v1, 0x20` on the shifted table entry) -- 148 differing.
     Assigning B to the local instead is 14.
   - STATE 19 HAS NO LOCAL: `A + 0x20 - B` is the func_80092E90 rule, and
     fold reassociates it into the target's `A - (B - 0x20)` with the
     subtrahend as a scratch register. The func_800AB1C0 spelling
     `A - (x = B - 0x20)` keeps the bias right but gives the local its own
     callee-saved register, which costs a word and 105 differing here,
     because the divisor `*(s16 *)(p + 0x96)` is already live in $a0 from
     the gate above it.

   D_80083460+4 IS `((s16 *)g)[2]`, AN ARRAY REF -- func_80092E90's rule.
   Through `*(s16 *)(g + 4)` the read is a plain MEM, the `b->unk_0C` store
   does not invalidate it, the second divisor is not reloaded and the
   function is a word short.

   STATE 16's TWO VELOCITIES USE `* 8`, NOT `<< 3`. A shift between the
   sign-extended table load and the narrowing `sh` lets combine turn the
   `lh` into `lhu`; the multiply does not. Two words. */
void func_80092018(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    s32 r;
    s32 x;
    s32 y;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            if (*(u8 **)(c + 0x2C) != D_800DD028)
            {
                *(u8 **)(c + 0x2C) = D_800DD028;
                func_80048A44(c, D_800DD028[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            }
            else
            {
                *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
            }
            break;
        }
        r = func_8009074C(*(s16 *)(p + 0x9E), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
        if ((s16)r == 0xFFF)
        {
            break;
        }
        if (((*(u16 *)(d + 0x2A) >> 9) & 7) == (((s16)r >> 9) & 7))
        {
            break;
        }
        if ((s16)func_80094EA4() == 0)
        {
            break;
        }
        *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        break;
    case 16:
        *(s16 *)((u8 *)b + 0xE) = D_8006CCD8[(*(u16 *)(d + 0x2A) >> 9) & 7] * 8;
        *(s16 *)((u8 *)b + 0x12) = D_8006CCE8[(*(u16 *)(d + 0x2A) >> 9) & 7] * 8;
        if (--*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        *(u8 **)(c + 0x2C) = D_800DD018;
        func_80048A44(c, D_800DD018[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u32 *)(*(u8 **)(p + 0x124) + 0x1C) |= 0x100;
        *(u16 *)(*(u8 **)(p + 0x124) + 0x6A) = *(u16 *)(d + 0x2A);
        *(u8 **)(*(u8 **)(p + 0x124) + 0x60) = d;
        p[0x9B]++;
        break;
    case 17:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            *(u8 **)(c + 0x2C) = D_800DD020;
            func_80048A44(c, D_800DD020[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        /* fallthrough */
    case 18:
        g = (u8 *)&D_80083460;
        if (((s16 *)g)[2] != 0)
        {
            b->unk_0C = (((c[0x24] << 6) + (x = D_8006CCD8[(*(u16 *)(d + 0x2A) >> 9) & 7] * 16 + 0x20) - *(s16 *)((u8 *)b + 0x2)) << 16) / ((s16 *)g)[2];
            b->unk_10 = (((c[0x25] << 6) + (y = D_8006CCE8[(*(u16 *)(d + 0x2A) >> 9) & 7] * 16 + 0x20) - *(s16 *)((u8 *)b + 0x6)) << 16) / ((s16 *)g)[2];
        }
        if ((((u16 *)g)[2] & 3) == 0)
        {
            func_800B653C((u8 *)b, *(s16 *)(d + 0x2A));
        }
        if (--((s16 *)g)[2] > 0)
        {
            break;
        }
        ((s16 *)g)[2] = 0;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(u8 **)(c + 0x2C) = D_800DD028;
        func_80048A44(c, D_800DD028[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s16 *)(p + 0x96) = 2;
        p[0x9B] = 0x13;
        break;
    case 19:
        if (*(s16 *)(p + 0x96) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x2)) << 16) / *(s16 *)(p + 0x96);
            b->unk_10 = (((c[0x25] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x6)) << 16) / *(s16 *)(p + 0x96);
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            if (--*(s16 *)(p + 0x96) == 0)
            {
                b->unk_10 = 0;
                b->unk_0C = 0;
            }
        }
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0 ||
            ((*(u16 *)(c + 0x14) & 0x6000) != 0 && *(s16 *)(p + 0x96) == 0))
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 0x812;
            *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        }
        break;
    }
}

/* Pick the actor's next +0x8C handler from the fourth argument's 0x100000
   flag, with the third argument's 0xE000 bits as the gate on the other side.

   THE ARMS ARE WRITTEN `== 0` FIRST. With the 0x100000 test positive the
   func_8008EAC8 arm is laid out inline and the `beqz` goes to the gate: 27
   words, 8 differing. AND THE HANDLER GOES IN A LOCAL: storing into
   `a->unk_8C` from inside each arm is 26 words / 26 differing -- the two
   parameter copies at the top disappear. 27 words, 0 differing. */
void func_800924F0(UnkDungeonActor *a, UnkVec *m, u8 *r, u8 *s)
{
    void (*h)();

    if ((*(u32 *)(s + 0x1C) & 0x100000) == 0)
    {
        if ((*(u16 *)(r + 0x14) & 0xE000) == 0)
        {
            return;
        }
        h = func_8008ACDC;
    }
    else
    {
        h = func_8008EAC8;
    }
    a->unk_8C = h;
    if ((s16)func_800A1C58(s) == 0)
    {
        D_8008346C = 0;
    }
}

/* State 0x2D: case 0 turns the mover 0x200 at a time until the
   camera-relative facing reaches 2 and then re-skins the renderable off
   D_800DD100, case 1 waits for func_8004CAE8 to claim the node's +0x8
   object, spawns the func_8008D470 node over the actor's cell and prints
   the two-part line, case 2 hands the actor to func_80091934 or parks
   func_8008ACDC.

   ITS TEMPLATE IS func_800927E4 two functions above, and its three measured
   spellings transfer verbatim: `q = &D_80083160;` with the heading read as
   the COMPONENT_REF `q->unk_C8` (so the `*(u8 **)(c + 0x2C)` store between
   the gate and the index kills cse's entry and BOTH `lh 0xC8($a1)` survive);
   the handler store written `*(u32 *)((u8 *)p + 0x8C)` and not `p->unk_8C`;
   and the facing index written out in full at both sites rather than bound
   to a local.

   THE MESSAGE HANDLE IS DRAWN INTO `s` AND COPIED TO `h`, NOT THE OTHER WAY
   ROUND. `h = func_800990FC(); s = func_8009929C(8, h);` gives
   `addu $s0, $v0, $zero` / `addu $a1, $s0, $zero` -- the long-lived copy
   first -- which is 2 differing at the same 162 words; drawing into `s`
   lets it die at the call, so it coalesces with the argument register and
   the target's `addu $a1, $v0, $zero` / `addu $s0, $a1, $zero` pair comes
   out. A separate `void *t` temporary, swapping the `h`/`s` declarations
   and `s = func_8009929C(8, h = func_800990FC());` all score 2.
   162 words, 0 differing. */
void func_8009255C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *a;
    Unk80083160 *q;
    void *h;
    void *s;

    switch (p->unk_9B)
    {
    case 0:
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            *(u8 **)(c + 0x2C) = D_800DD100;
            func_80048A44(c, D_800DD100[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p->unk_9B++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if (func_8004CAE8(*(u8 **)(c + 0x8), 0) == NULL)
        {
            break;
        }
        if (*(u8 **)((u8 *)p + 0x110) == D_80081484)
        {
            *(u32 *)((u8 *)p + 0x124) = 0;
            *(u32 *)((u8 *)p + 0x1C) = *(u32 *)((u8 *)p + 0x1C) & 0xFFEFFFFF;
        }
        n = (UnkNode *)func_800A8608((u8 *)p - 0x20, *(void **)((u8 *)p + 0x110), 0, 0, 0);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_8008D470;
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = (c[0x24] << 6) + 0x20;
            *(s16 *)((u8 *)m + 0x6) = (c[0x25] << 6) + 0x20;
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(d + 0x88) - 0x30;
            a = (u8 *)n + 0x20;
            *(s16 *)(a + 0xA8) = 0x1E;
            *(s16 *)(a + 0xAA) = 2;
            *(u8 **)(a + 0x90) = c;
            *(void **)(a + 0x8C) = b;
            *(u8 **)(a + 0x9C) = (u8 *)p + 0x108;
            s = func_800990FC();
            h = s;
            s = func_8009929C(8, h);
            s = func_80099734(d, s);
            s = func_80099194(D_800E06E0 + 3, s);
            s = func_80099368(*(u8 **)((u8 *)p + 0x110), s);
            s = func_80099194(D_800E06EB, s);
            s = func_8009965C(*(u8 **)((u8 *)p + 0x110), s);
            func_80099290(s);
            func_800A5720(h);
            func_800A56E0(0x509);
        }
        p->unk_9B++;
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        if (*(void **)((u8 *)p + 0x110) != NULL)
        {
            func_80091934(p, b, c, d);
            break;
        }
        *(u32 *)((u8 *)p + 0x8C) = (u32)func_8008ACDC;
        break;
    }
}

/* State 0x2C: case 0 re-skins the renderable at the camera-relative facing
   (on the 0x2C sub-state off D_80083228, otherwise off the camera record's
   +0xC8 and only at facing 2), case 1 projects the slot, spawns the
   func_8008D470 node and prints the pick-up line, case 2 runs the +0x96 timer
   down and either hands the actor to func_80091934 or closes the window and
   parks func_8008ACDC.

   THE CAMERA HEADING IN CASE 0's SECOND ARM MUST BE A STRUCT MEMBER --
   `q = &D_80083160; q->unk_C8` -- and that is worth two words. Through a
   `u8 *` base and `*(s16 *)(g + 0xC8)` the read is a FIXED-ADDRESS SCALAR,
   which cse's `invalidate_memory` does not kill on the `*(u8 **)(c + 0x2C)`
   store between the gate and the index, so one load serves both and combine
   folds the %lo into its displacement: 243 of 245 words, 191 differing. As a
   COMPONENT_REF the store kills it, the pointer gets two uses, and the
   target's `addiu $a1, $v0, %lo(D_80083160)` plus its two `lh 0xC8($a1)`
   appear. `D_80083160.unk_C8` with no pointer local scores the same.

   AND THE HANDLER STORE IN CASE 2 MUST *NOT* BE `p->unk_8C` -- the same
   lever from the other side. As a COMPONENT_REF it is a varying-address
   struct reference, which sched.c's `true_dependence` excuses against the
   fixed-address non-struct `*(u16 *)(g + 0x2)` read-modify-write beside it,
   so the handler's `lui` fills that `lhu`'s load-delay slot and the function
   is 244 words, 13 differing. Written `*(u32 *)((u8 *)p + 0x8C)` the two
   conflict, the order holds and the `nop` survives.

   The facing table is NAMED TWICE in each arm, not bound to a `u8 *t` --
   func_80091C64's rule: with the local the symbol's `high` takes $v0 and
   only its `lo_sum` takes $a1, and the `addu $a0, $s1, $zero` the target
   puts ahead of it is pushed two insns later. 17 differing with the local.
   245 words, 0 differing. */
void func_800927E4(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *a;
    u8 *e;
    u8 *g;
    Unk80083160 *q;
    void *h;
    void *s;
    u16 out[4];

    switch (p->unk_9B)
    {
    case 0:
        if (p->unk_9A == 0x2C)
        {
            *(u8 **)(c + 0x2C) = D_800DD0B8;
            func_80048A44(c, D_800DD0B8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p->unk_9B++;
            break;
        }
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            *(u8 **)(c + 0x2C) = D_800DD0B8;
            func_80048A44(c, D_800DD0B8[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p->unk_9B++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) == 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        if ((*(u32 *)(d + 0x1C) & 0x100000) == 0)
        {
            n = (UnkNode *)func_800A8608((u8 *)p - 0x20, *(void **)((u8 *)p + 0x110), 0, 0, 0);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_8008D470;
                m = n->unk_08;
                *(s16 *)((u8 *)m + 0x2) = (c[0x24] << 6) + 0x20;
                *(s16 *)((u8 *)m + 0x6) = (c[0x25] << 6) + 0x20;
                a = (u8 *)n + 0x20;
                if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
                {
                    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(d + 0x88) - 0x50;
                }
                else
                {
                    *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)m + 0x2) + out[0];
                    *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)m + 0x6) + out[1];
                    *(s16 *)((u8 *)m + 0xA) = out[2] + *(u16 *)(d + 0x88);
                }
                *(s16 *)(a + 0xA8) = 0x1E;
                *(s16 *)(a + 0xAA) = 2;
                *(u8 **)(a + 0x90) = c;
                *(void **)(a + 0x8C) = b;
                *(u8 **)(a + 0x9C) = (u8 *)p + 0x108;
                e = *(u8 **)((u8 *)p + 0x110);
                if (e != NULL && e[1] == 4 && *(s8 *)(e + 0x2) != 0)
                {
                    *(s16 *)(a + 0xAE) = 1;
                    *(s16 *)(a + 0xA8) = 0x3C;
                }
            }
            h = func_800990FC();
            s = func_8009929C(8, h);
            s = func_80099734(d, s);
            s = func_80099194(D_800E06EE, s);
            s = func_80099368(*(u8 **)((u8 *)p + 0x110), s);
            s = func_80099194(D_800E06F7, s);
            e = *(u8 **)((u8 *)p + 0x110);
            if (e[1] != 4)
            {
                s = func_8009965C(e, s);
            }
            func_80099290(s);
            func_800A5720(h);
        }
        p->unk_96 = 0x1E;
        p->unk_9B++;
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x8000) == 0 && (p->unk_96 -= 1) > 0)
        {
            break;
        }
        if (*(void **)((u8 *)p + 0x110) != NULL)
        {
            func_80091934(p, b, c, d);
            break;
        }
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x2) |= 0x812;
        *(u32 *)((u8 *)p + 0x8C) = (u32)func_8008ACDC;
        break;
    }
}

/* State 0x2B: case 0 re-skins the renderable off the actor's own +0xCC table
   once the camera-relative facing reaches 2 (and spins the mover's facing by
   0x200 until it does), case 1 waits for the projection or the 0x8000 flag
   and hands the +0x114 record to func_800D7A14 -- unless D_80013714's bit 3
   is up, where it either re-arms at sub-state 0x10 or parks func_8008ACDC --
   case 2 closes on func_80091934 or on the D_800DD150 skin, and case 0x10
   runs the +0x96 timer down into func_8008D368. 145 words, 0 differing on
   the first compile.

   ITS TEMPLATE IS THE FUNCTION ABOVE IT, func_800927E4, and three of that
   one's measured spellings transferred verbatim: `q = &D_80083160;` with the
   heading read as the COMPONENT_REF `q->unk_C8` (a `u8 *` base and
   `*(s16 *)(g + 0xC8)` is a fixed-address scalar that cse does not kill on
   the `*(u8 **)(c + 0x2C)` store between the gate and the index, so one load
   would serve both); the handler store written `*(u32 *)((u8 *)p + 0x8C)` and
   not `p->unk_8C`; and the facing index written out in full at both sites
   rather than bound to a local.


   The +0xCC skin table IS bound to a local here, unlike func_800927E4's
   named symbols -- it is a pointer LOAD, and the target loads it once for
   the `*(u8 **)(c + 0x2C)` store and the index both. */
void func_80092BB8(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    Unk80083160 *q;
    u8 *t;
    u16 out[4];

    switch (p->unk_9B)
    {
    case 0:
        q = &D_80083160;
        if ((((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) == 2)
        {
            t = *(u8 **)((u8 *)p + 0xCC);
            *(u8 **)(c + 0x2C) = t;
            func_80048A44(c, t[((q->unk_C8 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                          *(s16 *)((u8 *)p + 0x10E), 1);
            p->unk_9B++;
        }
        else
        {
            *(s16 *)(d + 0x2A) = *(u16 *)(d + 0x2A) + 0x200;
        }
        break;
    case 1:
        if ((D_80013714 & 8) != 0)
        {
            if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
            {
                if (*(u8 **)(c + 0x2C) == D_800DD148)
                {
                    p->unk_9B = 0x10;
                }
                else
                {
                    *(u32 *)((u8 *)p + 0x8C) = (u32)func_8008ACDC;
                }
            }
        }
        else if (func_8003DE58(*(u8 **)(c + 0x8), c, out, 0) != 0 ||
                 (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            func_800D7A14(*(u8 **)((u8 *)p + 0x114));
            p->unk_9B++;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0 || D_8008346A < 2)
        {
            if (*(void **)((u8 *)p + 0x110) != NULL)
            {
                func_80091934(p, b, c, d);
            }
            else
            {
                *(u8 **)(c + 0x2C) = D_800DD150;
                func_80048A44(c, D_800DD150[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7],
                              2, 1);
                *(u32 *)((u8 *)p + 0x8C) = (u32)func_8008ACDC;
            }
        }
        break;
    case 0x10:
        if ((p->unk_96 -= 1) <= 0)
        {
            func_8008D368((u8 *)p, b, c, D_800DD150, 0);
        }
        break;
    }
}

/* Two states: 0 arms a 0x10-frame timer and advances, 1 runs it down and
   hands the actor to func_8008ACDC when it expires.

   D_80012090 IS A CONSTANT ADDRESS, NOT THE DECLARED SYMBOL. Spelled
   `D_80012090` its `lui`/`lw` is a two-insn `high` + `lo_sum`, so reorg
   steals the `lui` into the preceding `beqz`'s delay slot and the function
   is 36 words against 37 with 13 differing. As `*(s32 *)0x80012090` the pair
   is one assembler macro nothing can be lifted out of, and the `beqz` keeps
   its `nop`. 37 words, 0 differing. */
void func_80092DFC(UnkDungeonActor *p)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    switch (p->unk_9B)
    {
    case 0:
        p->unk_96 = 0x10;
        p->unk_9B = p->unk_9B + 1;
        break;
    case 1:
        p->unk_96--;
        if (p->unk_96 < 0 || (*(s32 *)(g + 0x10) != 0 && *(s32 *)0x80012090 != 1))
        {
            *(s32 *)((u8 *)p + 0x124) = 0;
            p->unk_8C = func_8008ACDC;
        }
        break;
    }
}

/* The recentre step: drive the mover's two deltas at D_80083460+4 frames'
   worth of the distance to the renderable's (+0x24, +0x25) tile, then run the
   +0x9B state machine -- state 0 re-skins the renderable off D_800DCFF8,
   state 1 sweeps the node ring, un-sleeps every member whose kind byte
   matches D_80082E80's, reports the outcome and hands the actor back to
   func_8008ACDC. 152 words, 0 differing.

   FOUR SPELLINGS ARE LOAD-BEARING, each measured on this function:
   (1) D_80083460+4 IS `((s16 *)g)[2]`, AN ARRAY REF. Through `*(s16 *)(g + 4)`
   the read is a plain MEM, the `b->unk_0C` store does not invalidate cse's
   entry for it, the second divisor is not reloaded and the `%lo` folds into
   the load instead of staying in the pointer's `addiu` -- 149 words against
   152. An array ref is MEM_IN_STRUCT_P and both doors shut.
   (2) `(c[0x24] << 6) + 0x20 - *(s16 *)(b + 2)`, with the bias written as
   `A + K - B`. fold reassociates that into the target's `A - (B - K)`;
   written `A - (B - K)` it comes back out as `(A + K) - B`, which puts the
   `addiu` on the shift instead of the load and costs the second group's load
   a scheduling slot (152 / 75).
   (3) 0x80013714 IS A CONSTANT ADDRESS, `*(u16 *)0x80013714`. As the declared
   symbol the `lui` is schedulable and reorg puts it in the `& 0xE000` gate's
   delay slot where the target has a `nop` -- 151 words.
   (4) `s16 ok`. As `s32` the flag is tested in $s1 directly; narrow, the
   compare reads a COPY (`addu $v0, $s1, $zero`), which is the target's word.
   THREE SEPARATE `u8 *` LOCALS FOR &D_80083460, one per site: one variable is
   a global-alloc quantity and gets one register in all three blocks (the
   target uses $a1, $a0 and $v1), and the ring walk's node pointer is its own
   local too -- sharing it with the skin table is 5 differing. */
void func_80092E90(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *t;
    u8 *w;
    u8 *q;
    u8 *n;
    s16 ok;

    g = (u8 *)&D_80083460;
    if (((s16 *)g)[2] != 0)
    {
        b->unk_0C = (((c[0x24] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x2)) << 16) / ((s16 *)g)[2];
        b->unk_10 = (((c[0x25] << 6) + 0x20 - *(s16 *)((u8 *)b + 0x6)) << 16) / ((s16 *)g)[2];
    }
    else
    {
        b->unk_0C = 0;
        b->unk_10 = 0;
    }
    g2 = (u8 *)&D_80083460;
    if (((s16 *)g2)[2] != 0)
    {
        ((u16 *)g2)[2] -= 1;
    }
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            func_800419EC(0x10, 8);
            t = D_800DCFF8;
            *(u8 **)(c + 0x2C) = t;
            func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 1:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        if ((*(u16 *)0x80013714 & 8) == 0)
        {
            ok = 0;
            q = *(u8 **)(d + 0x5C) + 0x20;
            n = D_80082E80;
            do
            {
                w = *(u8 **)(q - 0x14);
                if (*(s8 *)(n + 0x26) == *(s8 *)(w + 0x26))
                {
                    if ((s16)func_80042900((s8 *)q, 1) != 0)
                    {
                        func_80042B68((s8 *)q, 1);
                        ok = 1;
                    }
                }
                q = *(u8 **)(q + 0x5C) + 0x20;
            } while (q != d);
            func_800997FC(ok ? D_800E0597 : D_800E05C3);
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u32 *)(p + 0x8C) = (u32)func_8008ACDC;
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) -= 1;
        break;
    }
}

/* The 0x19 state machine func_8008CF6C arms: case 0 reports both carriers
   through func_800C542C, case 1 counts to 0x3D and then either tears the
   floor down and installs D_80021268 at D_800E4938, or latches D_800E3CD0
   and pokes func_80040AA0(3); cases 2 to 4 run the installed handler and
   wait for its node's 0x8000 bit; case 0x10 repeats case 1's fork without
   the countdown. jtbl_80088A10 is at rodata offset 0x2B0, 0 MOD 8, so
   cc1psx's `.align 3` before it is a no-op (docs/dungeon-sweep.md, "SEVEN of
   the 28 jump tables are at 4 mod 8" -- this is not one of them).

   TWO EMPTY CASES ARE REAL. The table has EIGHTEEN entries, so the switch
   range runs to 0x11, not to the 0x10 the live cases stop at: without
   `case 0x11: break;` the dispatch is `sltiu $v0, $v1, 0x11` and the
   function is 217 words. The same shape func_8008E0C4's empty case 2 has.

   THE 0x80012090 FORK IS A SWITCH ON {0, 1} WITH A DEFAULT, not
   `if (x == 1)`. The target tests `beqz` and then `bne 1`, both to the same
   out-of-line block: that is case 0 going out of line because
   balance_case_nodes is gated on `i > 2`, with case 1 inline and the
   identical case-0 and default bodies cross-jumped into one copy. As
   `if (x == 1) {...} else {...}` it is one `bne` and 217 words.

   0x80010000 IS ONE BASE POINTER inside case 1 -- six accesses (0x3714,
   0x3716, 0x3718, 0x371A, 0x2090, 0x234) share its single `lui`, the lever
   func_8008C8BC records -- while case 4's 0x80010234 and case 0x10's
   0x80013714 are plain constant addresses, each a two-insn macro using its
   own destination as the temp. The 12-byte 0x80013710 -> 0x80012080 copy is
   a struct assignment at alignment 2, which is where the lwl/lwr + swl/swr
   pairs come from.
   219 words, 0 differing. */
void func_800930F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *e;
    s32 i;
    s32 r;

    switch (p[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x96) = 0;
        func_800B2074((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20);
        *(u32 *)(d + 0x1C) |= 0x40000000;
        for (i = 1; i >= 0; i--)
        {
            e = *(u8 **)(i * 4 + p + 0xAC);
            if (e != NULL)
            {
                func_800C542C(e, D_800DCED4[func_800429E4(e)], (s16)i, 0);
            }
        }
        p[0x9B] += 1;
    case 1:
        *(s16 *)(p + 0x96) += 1;
        if (*(s16 *)(p + 0x96) < 0x3D)
        {
            break;
        }
        h = (u8 *)0x80010000;
        if ((*(u16 *)(h + 0x3714) & 4) == 0)
        {
            func_800945E8(p);
            func_800948BC();
            *(UnkDungeon12080 *)0x80012080 = *(UnkDungeon12080 *)0x80013710;
            *(u16 *)(h + 0x3714) = 0;
            *(u16 *)(h + 0x3716) = 0;
            *(u16 *)(h + 0x3718) = 0;
            *(u16 *)(h + 0x371A) = 0;
            func_800A6780();
            switch (*(s32 *)(h + 0x2090))
            {
            case 0:
                *(s32 *)(h + 0x234) += 1;
                break;
            case 1:
                *(s32 *)(h + 0x234) += 1;
                func_80043568();
                *(s32 *)(h + 0x2090) = 0;
                break;
            default:
                *(s32 *)(h + 0x234) += 1;
                break;
            }
            func_800A56E0(0x514);
            D_800E4938 = D_80021268;
            D_80082EB8 = 0;
            D_800E296C |= 0x2000;
            p[0x9B] += 1;
        }
        else if (D_800E3CD0 == 0)
        {
            D_800E3CD0 = 1;
            func_80040AA0(3);
        }
        break;
    case 2:
        D_800DCF64 = (u8 *)func_800A613C();
        if (D_800DCF64 == NULL)
        {
            break;
        }
        p[0x9B] += 1;
        break;
    case 3:
        r = ((s32 (*)(u32, u32))D_800E4938_a[0])(D_800E4938_a[1], D_800E4938_a[2]);
        *(s32 *)(p + 0xC8) = r;
        if (r == 0)
        {
            return;
        }
        p[0x9B] += 1;
    case 4:
        if ((*(u16 *)(*(u8 **)(p + 0xC8) + 0x1E) & 0x8000) != 0)
        {
            func_80040AA0(D_80082E6B);
            D_8008146C = *(u16 *)0x80010234;
            func_800481E0();
            p[0x9B] += 1;
        }
        break;
    case 0x10:
        if ((*(u16 *)0x80013714 & 4) != 0)
        {
            if (D_800E3CD0 == 0)
            {
                D_800E3CD0 = 1;
                func_80040AA0(3);
            }
        }
        else
        {
            func_800945E8(p);
            func_800948BC();
            D_80082E76 = 0x8000;
            func_80041094(6, 0, 0, 0, 0x8000);
            p[0x9B] += 1;
        }
        break;
    case 0x11:
        break;
    }
    g = (u8 *)&D_80083160;
    *(s32 *)(g + 0xCC) = 0;
    *(s32 *)(g + 0x154) = 0;
    D_800E296C |= 0x40000;
}

/* The ten-state spawn/handover machine over +0x9B: state 0 drops the
   D_800DD138 skin, runs func_80093C70/func_80093D8C and latches the view's
   +0x8A slot in D_800DD262, state 1 runs a 0x10 countdown and either retires
   the slot through func_80093E74 or skips two states, state 2 announces
   D_800E06FA, clears the +0xAC slot and either closes (D_80082EB0 == NULL)
   or carries on, states 3 and 4 load a resource through func_800A1618 /
   func_800A0B94 and wait on the D_800E3E40 handshake, state 5 builds the
   entity, dresses it, announces D_800E0714 and arms a 0x10 countdown, state
   7 re-skins from D_800DD140 and state 8 parks func_8008ACDC.  jtbl_80088A58
   is 0 mod 8, so cc1psx's `.align 3` is a no-op.  398 words, 0 differing.

   EVERY CASE ADVANCES +0x9B ITSELF.  Written as one `p[0x9B]++` after the
   switch the out-of-range arm has to fall into it; written per case,
   cross-jumping merges the three tails into the listing's
   `lbu` / `addiu 1` / `sb` ladder, which is how states 1 and 2 can reach it
   with +2 and +4 and state 5 with the +0x96 store already done.

   D_80082EB0 IS READ AS A STRUCT MEMBER HERE, AND IT IS WORTH THREE WORDS.
   As the file's plain `extern u8 *D_80082EB0` the target's two re-reads in
   state 5 do not appear: cse.c's `note_mem_written` leaves `all` clear for a
   store through `(plus (reg) (const_int))`, so the fixed-address scalar
   survives the +0xD0 store and the `[3] |= 0x20` read is forwarded (2 words,
   with the target's `nop` after it), and sched.c's `true_dependence` excuses
   a fixed-address load against a varying-address store unless exactly one of
   them is a struct reference, so the load after `D_800E3D74[...] = 0` is
   hoisted into the `lh`'s load-delay slot (1 word).  Measured: plain, 397
   words / 70 differing; `(*(u8 *volatile *)&D_80082EB0)[3]` at the one site
   restores the reload only, 399 / 70; the one-member struct alias restores
   both, 400 / 48.  Same lever as docs/matching-levers.md's "A FIXED-ADDRESS
   SCALAR DOES NOT CONFLICT WITH A VARYING-ADDRESS STRUCT".

   THE +0x1C FLAG IS A STRUCT MEMBER FOR THE SAME REASON, worth the last 9
   differing.  As `*(s32 *)(q2 + 0x1C) |= 0x2000000;` the store is not
   MEM_IN_STRUCT_P, so true_dependence refuses to let the fixed-address
   `lh %lo(D_800DD262)` of the next statement move across it and the index
   arithmetic cannot fill the store's slots.  `((Unk42710 *)q2)->unk_1C` is
   the same instruction and 0 differing.

   ONE `h` LOCAL FOR BOTH func_800990FC RESULTS, and that is the function's
   register allocation.  With a local per case the state-5 one is 4 refs over
   29 insns, does NOT conflict with `d`, and shares $s1 with it; the state-5
   `%hi(D_80082EB0)` (8 refs / 132 insns, priority 3*8/132 = 0.182 against
   `d`'s 4*18/308 = 0.234) is then allocated after `d`, takes $s3, and the
   `%hi(D_800DD262)` is pushed to a SIXTH callee-saved register -- 400 words
   of 398, 48 differing, all of them renames.  One local spans state 2 as
   well, where it DOES conflict with `d`; `d` loses $s1 to the `%hi`, and the
   `%hi(D_800DD262)` then shares `d`'s $s3 exactly as the target does.
   398 words, 9 differing.

   THREE SPLIT STATEMENTS, ONE WORD EACH, all the same shape -- a global
   store wants to sit between a load and its use:
   `fl = *(u16 *)(c + 0x14); D_800DD260 = 0; *(u16 *)(c + 0x14) = fl | 0x200;`
   (either order of a plain `|= 0x200` and the `sb` leaves the `lhu`'s
   load-delay slot a `nop`), the same split for the +0xAC word against
   `D_800DD262 =`, and `r = *(u8 **)(d + 0x60);` ahead of `D_800DD260 = 1;`
   so that the `sb` is the insn in front of the `jal` reorg takes its delay
   slot from.  Inlined, func_800A31D0's call gets a `nop`.

   `k = h; if (D_800DD260 == 0) { k = func_8009929C(8, h); }` and not a
   ternary: the ternary builds both arms with a `j` and a second copy, 3
   words. */
void func_8009345C(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u8 *q;
    u8 *q2;
    u8 *r;
    s32 fl;
    s32 v;
    u8 *h;
    u8 *t;
    u8 *t2;
    u8 *w;
    u8 *g;
    u8 *k;
    s16 n;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            t = D_800DD138;
            *(u8 **)(c + 0x2C) = t;
            func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            func_80093C70(p, b, c);
            func_80093D8C(p, b, c);
            fl = *(u16 *)(c + 0x14);
            D_800DD260 = 0;
            *(u16 *)(c + 0x14) = fl | 0x200;
            *(s16 *)(p + 0x96) = 0x10;
            v = *(s32 *)(p + *(s16 *)(d + 0x8A) * 4 + 0xAC);
            D_800DD262 = *(u16 *)(d + 0x8A);
            *(s32 *)(d + 0x60) = v;
            p[0x9B]++;
        }
        break;
    case 1:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            if (*(s32 *)(d + 0x60) != 0)
            {
                func_80093E74(p, b, c, d);
                p[0x9B]++;
            }
            else
            {
                p[0x9B] += 2;
            }
        }
        break;
    case 2:
        if ((*(s32 *)(*(u8 **)(d + 0x60) + 0x1C) & 0x800000) != 0)
        {
            h = func_800990FC();
            func_80099290(func_80099194(
                D_800E06FA, func_80099734(*(u8 **)(d + 0x60),
                                          func_80099194(D_800DD262 ? D_800E05F0 : D_800E05E1,
                                                        func_8009929C(8, h)))));
            func_800A5720(h);
            r = *(u8 **)(d + 0x60);
            D_800DD260 = 1;
            func_800A31D0(r);
            *(s32 *)(p + D_800DD262 * 4 + 0xAC) = 0;
            func_800A18E8((*(u8 **)(d + 0x60))[0x13], 3);
            func_8009A028(*(u8 **)(d + 0x60));
            *(u16 *)(*(u8 **)(d + 0x60) - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            if (D_80082EB0_m.unk_00 == NULL)
            {
                *(s16 *)(p + 0x96) = 0;
                p[0x9B] += 4;
            }
            else
            {
                p[0x9B]++;
            }
        }
        break;
    case 3:
        if ((s16)func_800A2BDC(NULL) == 0)
        {
            q = func_800A1618(*D_80082EB0_m.unk_00, 3);
            if (q != NULL)
            {
                D_80081488 = (u32)func_80048118(*D_80082EB0_m.unk_00, D_8008149C);
                func_800A0B94(*D_80082EB0_m.unk_00, q, 0);
                D_800E3E40 = 0;
                func_8003E4FC(0xFF, D_8003E140, (s32)&D_800E3E40);
            }
            p[0x9B]++;
        }
        break;
    case 4:
        if (D_800E3E40 != 0)
        {
            if (D_80081488 != 0)
            {
                func_80047FF4(*D_80082EB0_m.unk_00, D_80081488);
            }
            p[0x9B]++;
        }
        break;
    case 5:
        q2 = func_800A1618(*D_80082EB0_m.unk_00, 3);
        if (q2 != NULL)
        {
            func_80043914(D_80082EB0_m.unk_00);
            q2 = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(*D_80082EB0_m.unk_00, q2, 1))(
                1, *(s8 *)(d + 0x72), *(s8 *)(d + 0x73), (s16)(*(u16 *)(d + 0x88) - 0x20));
            if (q2 != NULL)
            {
                *(s32 *)(p + D_800DD262 * 4 + 0xAC) = (s32)q2;
                D_800E3DF0[D_80082EB0_m.unk_00[3] & 0x1F] = (s32)q2;
                w = *(u8 **)(q2 - 0x14);
                *(s16 *)(w + 0x1E) = 0x800;
                *(s16 *)(w + 0x1C) = 0x800;
                w = *(u8 **)(q2 - 0x18);
                *(s16 *)(w + 0x2) = *(u16 *)(b + 0x2);
                *(s16 *)(w + 0x6) = *(u16 *)(b + 0x6);
                *(s16 *)(w + 0xA) = *(u16 *)(b + 0xA) - 0x10;
                func_80042640((Unk42710 *)q2, q2[0x13]);
                func_800424E0(q2, q2[0x13], D_80082EB0_m.unk_00);
                ((Unk42710 *)q2)->unk_1C |= 0x2000000;
                *(s32 *)(p + D_800DD262 * 4 + 0xD0) = (s32)D_80082EB0_m.unk_00;
                D_80082EB0_m.unk_00[3] |= 0x20;
                h = func_800990FC();
                k = h;
                if (D_800DD260 == 0)
                {
                    k = func_8009929C(8, h);
                }
                func_80099290(func_80099194(
                    D_800E0714, func_80099734(q2, func_80099194(D_800DD262 ? D_800E05F0
                                                                           : D_800E05E1,
                                                                k))));
                func_800A5720(h);
                D_800E3D74[D_800DD262] = 0;
                func_800A152C(*D_80082EB0_m.unk_00, 3);
                func_80042560((s8 *)q2);
                func_800A56E0(0x704);
            }
        }
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B]++;
        break;
    case 6:
        p[0x9B]++;
        break;
    case 7:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            t2 = D_800DD140;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 8:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x812;
            *(u16 *)(c + 0x14) &= 0xFDFF;
            p[0x9B]++;
        }
        break;
    case 9:
        break;
    }
}

/* Four states of the cut-scene camera fade: ease the renderable's +0x1C
   towards 0x1000, hold while the parked record at +0x2C is still
   D_800DD140, ease back to 0, then kill the node. 119 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING.
   (1) THE +0xC BYTE GOES IN A LOCAL. Written `c[0xC] >= 0x81` and then
   `c[0xC] - 8`, cse does not common the two reads across the comparison and
   the function is 122 words -- a second `lbu` plus its load-delay `nop` plus
   the `addiu -8` losing the branch's delay slot. One read into a `u8` local
   gives the target's `addiu $v0, $v1, -0x8` in the `bnez`'s delay slot.
   (2) THE THREE BYTE STORES ARE 0xC, 0xE, 0xD IN SOURCE ORDER. 0xC, 0xD, 0xE
   is 119 words at 2 differing: they are three stores of one value at disjoint
   displacements off one base, so sched does not reorder them and the source
   order is visible.

   The `*(s16 *)(p + 0x4) += 1` tail is written out in BOTH case 0 and case 2
   and cross-jumping merges them; case 1 reaches the same tail one insn in,
   because its own `lhu 0x4($s1)` is already in $v0 (the target's
   `j .L80093C28` lands past the shared load). The kill body is case 3 with a
   `goto` label, which is how case 1's early exit reaches it -- the switch has
   no default, so a shared tail has to be a label inside it. */
void func_80093A94(u8 *p, u8 *b, u8 *c)
{
    s16 t;
    s32 v;
    u8 d;

    func_80048AC8(c, *(s16 *)(p + 0x8) + 2);
    if (D_80082E94 & 1)
    {
        *(u16 *)(c + 0x14) |= 1;
    }
    else
    {
        *(u16 *)(c + 0x14) &= ~1;
    }
    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(c + 0x1C) += (0x1000 - *(u16 *)(c + 0x1C)) / 4;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t <= 0)
        {
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x8) == 0)
        {
            d = c[0xC];
            if (d >= 0x81)
            {
                v = d - 8;
                c[0xC] = v;
                c[0xE] = v;
                c[0xD] = v;
            }
        }
        if (*(u8 **)(*(u8 **)p + 0x2C) == D_800DD140)
        {
            if (*(s16 *)(p + 0x8) != 0)
            {
                goto kill;
            }
            *(s16 *)(p + 0x6) = 8;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 2:
        *(s16 *)(c + 0x1C) += (0 - *(u16 *)(c + 0x1C)) / 4;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t <= 0)
        {
            *(s16 *)(c + 0x1C) = 0;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 3:
    kill:
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* The func_80093D8C recipe with a different renderable seed and an option
   bit. THE FIRST ACCESS TO THE RENDERABLE'S +0x14 FLAGS IS `volatile` AND THE
   SECOND IS NOT; that one qualifier is worth four words. Written plain, cse
   forwards the stored halfword into the `D_80082E94 & 1` arm and the hoisted
   `lhu` also fills a load-delay slot the target leaves as a `nop`: 67 words
   against 71, 41 differing. Volatile on the second instead is 72 / 27, and on
   BOTH is also 72 / 27. A pointer local for `n + 0x20` is the remaining word.
   71 words, 0 differing. */
void func_80093C70(u8 *a, u8 *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FD64(0x310, a - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_80093A94;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(b + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x28) = *(u32 *)(c + 0x28);
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        /* DEVICE: the `volatile` here, and only here, is worth four words. */
        *(volatile u16 *)(r + 0x14) |= 0x200;
        *(s16 *)(r + 0x6) = 4;
        if ((D_80082E94 & 1) != 0)
        {
            *(u16 *)(r + 0x14) |= 1;
        }
        func_80048A44(r, 0xCE, 0, 3);
        func_8004491C(n, (u32)D_80045340);
        p = (u8 *)n + 0x20;
        *(u8 **)p = c;
        *(s16 *)(p + 0x6) = 0x10;
        *(u32 *)(r + 0xC) = 0x2C808080;
        *(s16 *)(p + 0x8) = 1;
    }
}

/* A 0x310 spawner parented on the CALLER's own node: the second argument to
   func_8003FD64 is `a - 0x20`, the node header behind the actor view the
   first parameter points at. 58 words, 0 differing. */
void func_80093D8C(u8 *a, u8 *b, u8 *c)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;

    n = func_8003FD64(0x310, a - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_80093A94;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(b + 0xA);
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0x28) = *(u32 *)(c + 0x28);
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x14) = 0xC;
        *(s16 *)(r + 0x10) = 0x20;
        *(u32 *)(r + 0xC) = 0xF8F8F8;
        *(s16 *)(r + 0x1C) = 0;
        *(s16 *)(r + 0x6) = 4;
        *(u16 *)(r + 0x14) |= 0x200;
        func_80048A44(r, 0xD0, 0, 2);
        func_8004491C(n, (u32)D_80045340);
        *(u8 **)((u8 *)n + 0x20) = c;
        *(s16 *)((u8 *)n + 0x26) = 0x10;
    }
}

/* Retire the party slot the view's +0x8A names: copy the live record into
   the D_800E3E48 shadow, point D_800E3DF0 at the copy, clear the slot's
   0x4000 flag and the +0xD0 pointer, mark the owner and announce 0x704.

   THE SLOT INDEX IS `s16`, AND IT IS THE WHOLE FUNCTION. As `u8 k` the
   widened value lives in ONE pseudo and the target's `addu $s0, $s3, $zero`
   -- a copy of the widened index, kept so that one register feeds the three
   early uses and the other the late re-read after the inline struct
   assignment ends cse's region -- is never emitted: 109 words of 110.
   An `s16` is an HImode pseudo whose int uses each want a conversion, which
   is where the copy comes from. `u16` is 1 differing (the extension comes
   out `andi` rather than `sll`/`sra`), `s8` is 109 words again, and any
   plain `s32` index is 106 -- an int index lets reload_cse reuse the earlier
   `&D_800E3DF0[i]` across the block move and costs four more words.

   Three placements of an `__asm__("" : "=r"(j) : "0"(j))` copy device were
   measured first and all stayed at 109: gcc coalesces the copy away.
   decomp-permuter found the type in 434 candidates. 110 words, 0 differing. */
void func_80093E74(u8 *p, void *b, void *c, u8 *d)
{
    u8 *q;
    u8 *r;
    u8 *e;
    u8 *w;
    s16 k;
    s32 i;

    q = *(u8 **)(p + *(s16 *)(d + 0x8A) * 4 + 0xD0);
    q[3] &= 0xDF;
    k = q[3] & 0x1F;
    i = k;
    r = (u8 *)D_800E3DF0[i];
    if ((s16)func_80042900((s8 *)r, 0xA) != 0)
    {
        r[0x13] = r[0xA8];
    }
    D_800E3DF0[i] = (s32)&D_800E3E48[i];
    D_800E3E48[i] = *(Unk800E3E48 *)r;
    *(u32 *)((u8 *)D_800E3DF0[k] + 0x14) &= ~0x4000;
    *(s32 *)(p + *(s16 *)(d + 0x8A) * 4 + 0xD0) = 0;
    *(u32 *)(*(u8 **)(d + 0x60) + 0x1C) |= 0x400000;
    e = *(u8 **)(d + 0x60);
    w = *(u8 **)(e - 0x14);
    func_8009A3D0(w[0x24], w[0x25], (*(u32 *)(e + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    func_800A56E0(0x704);
}

/* The dungeon's "can the party step here" probe, called from two stubs with
   SIX arguments: the leader record, an unread second, the tile record whose
   +0x24/+0x25 are biased by the engine's sine/cosine tables, the two `s16 *`
   out-cells func_8009B88C fills, and a trailing record whose +0x3 byte names
   a D_800E3DF0 slot. It returns 3, 1, 2 or 0 and both callers sign-extend
   the result, which is why the return type is `s16`.

   ONE `r` FOR BOTH func_8009B88C CALLS IS THE WHOLE LANDING. With a second
   variable (or with the first call tested inline) the second result dies at
   its own `sll`, local-alloc folds the comparison temporary into its
   register and the function is `sll $s0, $s0, 16` / `bnez $s0` where the
   target has `sll $v0, $s0, 16` / `bnez $v0` -- 119 words, 2 differing.
   Sharing the variable makes it a multi-block quantity, so global-alloc
   gives it $s0 and the temporary takes $v0. Measured and ruled out at 2
   differing: `s16`/`u16`/`short` r, r declared first/last, `(s16)r == 0`,
   `!r`, `(r & 0xFFFF) == 0`, `if (r != 0) goto tail`, a `do { } while (0)`
   around the test, and an s32-returning cast of func_8009B88C. An
   `__asm__("" ::"r"(r))` after the test also reaches 0, which is what
   identified the cause. `s32 r` is 4 differing (it buys a sign-extension).

   `w = a;` IS REAL: the target copies the first parameter into a second
   callee-saved register at word 9 and reads the +0x8A index off THAT while
   the +0xAC slot table is indexed off the original. One pointer for both is
   one register and one word short.

   func_8009A3D0's result is read nowhere, so its `void` declaration stands,
   and the second parameter is never read -- its `s32` is a placeholder.
   119 words, 0 differing. */
s16 func_8009402C(u8 *a, s32 b, u8 *c, s16 *d, s16 *e, u8 *f)
{
    u8 *w;
    u8 *t;
    u8 *q;
    s32 k;
    s32 x;
    s32 y;
    s16 r;
    s32 x0;
    s32 y0;

    w = a;
    if (f != NULL)
    {
        if ((*(u32 *)((u8 *)D_800E3DF0[f[3] & 0x1F] + 0x1C) & 0x4000000) != 0)
        {
            return 3;
        }
        k = (*(u16 *)(a + 0x2A) >> 9) & 7;
        x = (s16)(c[0x24] + (u16)D_8006CCD8[k]);
        y = (s16)(c[0x25] + (u16)D_8006CCE8[k]);
        r = func_8009B88C(NULL, x, y, d, e);
        if (r == 0)
        {
            return 1;
        }
        if ((*(u32 *)(a + 0x1C) & 0x100000) != 0)
        {
            t = *(u8 **)(a + 0x124);
            if (t != NULL)
            {
                if (*(s8 *)(t + 0x13) > 0)
                {
                    x0 = *d;
                    y0 = *e;
                    func_8009A21C(x0, y0, 0x8000);
                    r = func_8009B88C(NULL, x, y, d, e);
                    func_8009A3D0(x0, y0, 0x8000);
                    if (r == 0)
                    {
                        return 1;
                    }
                }
            }
        }
    }
    q = *(u8 **)(a + *(s16 *)(w + 0x8A) * 4 + 0xAC);
    if (q != NULL && (*(u32 *)(q + 0x1C) & 0x20000) == 0)
    {
        return 2;
    }
    func_80094E34();
    return 0;
}

/* Announce D_800E0600 against the record in the engine's +0xAC slot table,
   but only while the second slot's owner is 1.

   D_80012090 IS A CONSTANT ADDRESS, and that is the whole landing. As the
   declared symbol the `lui`/`lw` pair is `high` + `lo_sum` in TWO insns, so
   sched1 slips the comparison's `addiu $v0, $zero, 1` between the load and
   the `bne` to cover the load delay and reorg then fills the earlier
   `beqz`'s slot with the `lui` instead: 26 words, 4 differing, with the
   constant and the load exactly swapped. As `*(s32 *)0x80012090` the pair is
   ONE assembler macro nothing can be scheduled into. The nesting is
   load-bearing too: with the guard as a plain early `return 0` both exits
   cross-jump onto one and the function is 24 words. 26 words, 0 differing. */
s32 func_80094208(s16 k)
{
    u8 *base = D_800E3D7C;
    u8 *q;

    q = *(u8 **)(base + k * 4 + 0xAC);
    if (q != NULL)
    {
        if (*(s32 *)0x80012090 != 1)
        {
            return 0;
        }
        func_80099844(q, D_800E0600);
        return 1;
    }
    return 0;
}

/* The "you cannot do that" reporter: hand func_8009402C the intended move and,
   on any refusal, build the matching message out of D_800E3DF0's name table
   and send it.  182 words, 0 differing, with one device.

   THE FIFTH PARAMETER OF func_8008DB0C HAD TO BE WIDENED from `u16 e` to
   `s16 e`.  With `u16` this function's call site emits `lhu 0x1A($sp)` where
   the target has `lh`.  Both of func_8008DB0C's callers agree with `s16`:
   func_80091430's listing loads the same argument with `lh $v0, 0x1A($sp)`.
   func_8008DB0C itself still measures 55 of 55 words, 0 differing.

   SETTLED, with numbers: the index must reuse `r`, func_8009402C's own s32
   result, so sched1's `birthing_insn_p` does not defer it past the second
   call -- a separate `s32 i` is 5 differing and `s16 r` is 7; the record slot
   is addressed inline twice, a `q` local costs 1; `*(p + k + 0xFA)` not
   `p[k + 0xFA]`, 1 word; and the two `return 0` paths share one
   `move $v0, $zero`, so the first branch falls into an `else` and the
   function ends with a single `return 0;` -- two separate `return 0;` is 181
   words. */
s32 func_80094270(u8 *p, void *b, u8 *c, u8 *d, s16 k)
{
    s16 x;
    s16 y;
    s32 r;
    u8 *h;
    u8 *m;
    s32 t;

    *(s16 *)(p + 0x8A) = k;
    if (func_80094208(0) != 0)
    {
        return 1;
    }
    if (*(p + k + 0xFA) == 2)
    {
        func_800997FC(D_800E0633);
        return 1;
    }
    if (*(u8 **)(p + *(s16 *)(p + 0x8A) * 4 + 0xD0) == d)
    {
        if ((*(u32 *)(*(u8 **)(p + *(s16 *)(p + 0x8A) * 4 + 0xAC) + 0x1C) & 0x20000) == 0)
        {
            m = func_800990FC();
            h = func_80099194(D_800E0726, m);
            h = func_80099194(*(s16 *)(p + 0x8A) != 0 ? D_800E05F0 : D_800E05E1, h);
            h = func_80099194(D_800E0739, h - 3);
            func_80099290(h);
            func_800A5720(m);
            func_800A56E0(0x506);
            return 1;
        }
        func_80094E34();
        D_80082EB0 = NULL;
        func_8008DB0C((UnkDungeonActor *)p, b, c, 0, 0);
        *(s32 *)(p + 0x60) = 0;
    }
    else
    {
        r = func_8009402C(p, (s32)b, c, &x, &y, d);
        if (r != 0)
        {
            m = func_800990FC();
            if (r == 1)
            {
                t = d[3] & 0x1F;
                /* DEVICE: this reference stops combine folding the 0x1F mask and
                   the (u8) truncation into one `andi`.  A CALL between the two
                   statements is the only legal barrier (combine.c:937) and it puts
                   the zero-extension on the wrong side; the reference pins both
                   before the call, which is where the target has them.  Without it
                   the function is 182 words, 4 differing: mine loads into $s0 and
                   masks in place, the target loads into $v0, masks there, and
                   takes `andi $s0, $v0, 0xFF` as the jal's delay slot.  Both
                   statements moved before the call without the device fold to one
                   `andi` and the function is 181 words / 84.  It emits nothing. */
                __asm__("" ::"r"(t));
                r = (u8)t;
                h = func_80099194(D_800E0747, m);
                h = func_8009929C(0xA, h);
                h = func_80099734((u8 *)D_800E3DF0[r], h);
                h = func_80099194(D_800E0766, h);
            }
            else if (r == 2)
            {
                h = func_80099194(D_800E0769, m);
                h = func_80099194(k != 0 ? D_800E05F0 : D_800E05E1, h);
                h = func_80099194(D_800E077C, h - 3);
            }
            else
            {
                h = func_80099734((u8 *)D_800E3DF0[d[3] & 0x1F], m);
                h = func_80099194(D_800E078A, h);
            }
            func_80099290(h);
            func_800A5720(m);
            func_800A56E0(0x506);
            return 1;
        }
        *(s32 *)(p + 0xC8) = 0;
        D_80082EB0 = d;
        func_8008DB0C((UnkDungeonActor *)p, b, c, x, y);
        func_80094E34();
    }
    return 0;
}

/* The same +0xAC slot table and the same pointer-local hoist func_8009456C
   documents. 9 words, 0 differing. */
void func_80094548(s16 k, s32 v)
{
    u8 *base = D_800E3D7C;

    *(s16 *)(*(u8 **)(base + k * 4 + 0xAC) + 0x2A) = v << 9;
}

/* Write a byte into +0x45 of the record in the engine's +0xAC slot table.

   THE POINTER LOCAL IS LOAD-BEARING. Spelled inline --
   `(*(u8 **)(k * 4 + D_800E3D7C + 0xAC))[0x45] = v;` -- the `sll` for the
   index lands ahead of the `lui` and the function is 2 differing; only
   taking the global into a local first hoists its `high` above the
   sign-extension. That is docs/matching-levers.md's pointer-local hoist
   (:3628) applied to a LOAD of a pointer global, which the section records
   only for a store. 8 words, 0 differing. */
void func_8009456C(s16 k, u8 v)
{
    u8 *base = D_800E3D7C;

    (*(u8 **)(base + k * 4 + 0xAC))[0x45] = v;
}

/* NO POINTER-LOCAL HOIST HERE, unlike func_8009456C next door: this
   target's sign-extension comes BEFORE the `lui`, which is the inline
   spelling.

   AND D_80012098 IS NOT A SYMBOL. As `extern u16 D_80012098[]` the address
   is its own quantity and costs an `addiu %lo` the target does not have; as
   `((u16 *)0x80012098)[k]` gcc folds the whole constant with `lui`/`ori` at
   the top; a scalar plus `(&D_80012098)[k]` is the array shape again. It is
   the constant base 0x80010000 -- low half zero, so a bare `lui` -- with the
   field offset inside the index expression (docs/matching-levers.md:2314),
   and the base must be a POINTER LOCAL or the `+ 0x2098` folds back into the
   constant. 14 words, 0 differing. */
void func_8009458C(s16 k, u8 v)
{
    u8 *b;

    (*(u8 **)(k * 4 + D_800E3D7C + 0xAC))[0x12] = v;
    b = (u8 *)0x80010000;
    *(u16 *)(b + k * 2 + 0x2098) = v;
}

/* The same table at +0x46, with bit 15 forced on. 9 words, 0 differing. */
void func_800945C4(s16 k, s32 v)
{
    u8 *base = D_800E3D7C;

    *(s16 *)(*(u8 **)(base + k * 4 + 0xAC) + 0x46) = v | 0x8000;
}

/* The save: hand the view to func_800422DC, fold every live D_800E3DF0
   record into its D_800E3E48 shadow, then write the whole lot out of the
   0x80010000 page through func_800422A8 -- the twenty 0x80010248 entries
   and their shadows, the two +0xD0 slots, and finally the six scalars off
   the view. 181 words, 0 differing.

   EVERY 0x8001xxxx ADDRESS HERE IS A CONSTANT ADDRESS, and the spellings
   are NOT interchangeable:
   * loop 1's cursor and func_800422A8's record argument are plain constant
     addresses (`lui`/`ori`), func_8009FADC's shape;
   * loops 2 and 3 and the tail each want their OWN `u8 *` page-base local
     holding 0x80010000 with the table offset inside the index,
     func_80098FF8's shape. ONE shared base local for all three is 182
     words: it stays live in a callee-saved register to the end and the
     tail's own `lui` never happens. Three separate locals is 181/21.

   FOUR MORE SPELLINGS WERE EACH WORTH WORDS, measured from that 21:
   * D_800E3DF0 and D_800E3E48 are SOURCE LOCALS assigned before loop 1,
     not expressions loop.c hoists: a source statement in the preheader
     sits ahead of a hoist, so with the cursor written before the loop and
     the two tables left as hoists the cursor's `lui`/`ori` comes out FIRST
     where the target emits it LAST, 6 words of preheader order. With
     `i = 0x13; tbl = ...; sh = ...; q = ...;` and a `do/while` the
     preheader is exact. Making the cursor a giv instead -- either
     `(0x13 - i) * 4` or a second biv -- is 185 and 183 words.
   * `tbl[k] = k * 0x8C + (s32)sh;`, written as INTEGER arithmetic so the
     base is added last. `(s32)&sh[k]` and `(s32)(sh + k)` are pointer
     arithmetic and come out `addu $v0, $s4, $v0`; the target has
     `addu $v0, $v0, $s4`, matching the block move's own address.
   * loop 3's counter is initialised BEFORE the page base (1 word of
     preheader order) and its index is written
     `*(u8 *)(i + (s32)g2 + 0x2D52)` -- integer arithmetic keeps source
     order, so the counter is the first `addu` operand as the target has it.
   * THE PARAMETER IS COPIED INTO A LOCAL. Used directly, `p` and loop 2's
     hoisted `high` of D_800E3E48 swap $s6 and $s7 -- 12 differing, every
     one a register number. `p = arg;` as the first statement costs no
     instruction and produces the target's assignment; the declaration
     position of the local does not matter (measured both ends).

   The two struct copies are block moves: `sh[k] = *(Unk800E3E48 *)r;` is
   the 0x8C-byte one func_80093E74 already uses, and the 4-byte
   `Unk80010248` record copies as `lwl`/`lwr` + `swl`/`swr` because that
   type's alignment is 1.

   seqcheck is the constant-address subsequence case: eleven `%hi`/`%lo`
   entries the listing shows have no relocation in the candidate, all of
   them on DATA symbols in the 0x8001xxxx page, and `raw == relocdump` at
   15 == 15 proves those words byte for byte. */
void func_800945E8(u8 *arg)
{
    u8 *p;
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *q;
    u8 *r;
    s32 i;
    s32 k;
    s32 *tbl;
    Unk800E3E48 *sh;

    p = arg;
    func_800422DC((u8 *)0x80012194, (s32)p);
    i = 0x13;
    tbl = D_800E3DF0;
    sh = D_800E3E48;
    q = (u8 *)0x8001024B;
    do
    {
        if (q[-2] == 0x13 && (q[0] & 0x20) != 0)
        {
            k = q[0] & 0x1F;
            r = (u8 *)tbl[k];
            if ((s16)func_80042900((s8 *)r, 0xA) != 0)
            {
                r[0x13] = r[0xA8];
            }
            sh[k] = *(Unk800E3E48 *)r;
            tbl[k] = k * 0x8C + (s32)sh;
        }
        q += 4;
    } while (--i >= 0);
    g = (u8 *)0x80010000;
    for (i = 0x13; i >= 0; i--)
    {
        *(Unk80010248 *)(g + i * 4 + 0x21E8) = *(Unk80010248 *)(g + i * 4 + 0x248);
        *(u8 *)(g + i + 0x2238) = func_800422A8(*(u8 **)(g + i * 4 + 0x29C), (u8 *)0x80010248, 4, 0x14);
        *(Unk800E3E48 *)(g + i * 0x8C + 0x2260) = D_800E3E48[i];
        *(u8 *)(g + i + 0x224C) = func_800422A8((u8 *)D_800E3DF0[i], D_800E3E48, 0x8C, 0x14);
    }
    i = 1;
    g2 = (u8 *)0x80010000;
    do
    {
        *(u8 *)(i + (s32)g2 + 0x2D52) = func_800422A8(*(u8 **)(p + i * 4 + 0xD0), (u8 *)0x80010248, 4, 0x14);
    } while (--i >= 0);
    g3 = (u8 *)0x80010000;
    *(u16 *)(g3 + 0x3624) = *(u16 *)(g3 + 0x3626);
    *(u16 *)(g3 + 0x209C) = *(u16 *)(g3 + 0x209E);
    *(u16 *)(g3 + 0x2D50) = *(u16 *)(p + 0xF8);
    *(s32 *)(g3 + 0x2D58) = *(s32 *)(p + 0xFC);
    *(u8 *)(g3 + 0x2D6C) = p[0xFA];
    *(u8 *)(g3 + 0x2D6D) = p[0xFB];
    func_800A9160();
}

/* Clear +0x13 of twenty 0x54-byte records at 0x800102F0, then hand each live
   0x13-kind entry its D_800E3DF0 value.

   BOTH 0x800102F0 AND 0x8001024B ARE CONSTANT ADDRESSES, and 0x800102F0 is
   materialised twice because the second use is inside the loop.

   THE SECOND TABLE'S CURSOR MUST BE A BASE LOCAL PLUS AN INDEX COMPUTED
   INSIDE THE LOOP, so that loop.c owns the init: a giv's init is emitted
   AFTER loop.c's hoists, where a source statement sits ahead of them. A
   hand-walked `q` is 51 words / 6 differing with its `lui`/`ori` ahead of the
   0x13 and D_800E3DF0 hoists instead of behind them, and 5 with the init in
   the `for`'s first clause. 51 words, 0 differing. */
void func_800948BC(void)
{
    s32 i;
    s32 k;
    u8 *p;
    u8 *b;
    u8 *q;

    p = (u8 *)0x800102F0;
    for (i = 0x13; i >= 0; i--)
    {
        p[0x13] = 0;
        p += 0x54;
    }
    b = (u8 *)0x8001024B;
    for (i = 0; i < 0x14; i++)
    {
        q = b + i * 4;
        if (q[-2] == 0x13)
        {
            k = q[0] & 0x1F;
            if (D_800E3DF0[k] != 0)
            {
                func_800422DC((u8 *)(k * 0x54 + 0x800102F0), D_800E3DF0[k]);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094988);

/* THE +0x1C FLAG WORD HAS TO BE AN ARRAY REF, not a `u8 *` cast: only an
   ARRAY_REF is MEM_IN_STRUCT_P, and only a MEM_IN_STRUCT_P load is hoisted
   over the `%lo(D_80081484)` store, which is what puts the reload of +0x1C
   ahead of D_80081484's own load. Through the cast it is 28 words at 8
   differing, and the clear/save pair has to be written AFTER the mask or it
   is 12. An array ref off a `u8 *` base is enough -- no struct typedef is
   needed to get the flag. 28 words, 0 differing. */
void func_80094E34(void)
{
    u8 *p;
    u8 *q;
    void *t;

    p = D_800E3D7C;
    if (((u32 *)p)[7] & 0x100000)
    {
        q = *(u8 **)(p + 0x124);
        if (q != NULL)
        {
            *(u32 *)(q + 0x1C) &= ~0x80000;
        }
        ((u32 *)p)[7] &= ~0x100000;
        t = *(void **)D_80081484;
        *(void **)D_80081484 = NULL;
        D_800E3540 = t;
    }
}

/* Two bit tests over one engine flag word. THE INNER TEST IS NESTED rather
   than a second early return: as two consecutive `if (c) return 1; return
   0;` statements jump.c's store-flag transform fires on the 0/1 pair and
   the second branch collapses to `sltu $v0, $zero, $v0` -- 10 words, 8
   differing (docs/matching-levers.md:2019). 12 words, 0 differing. */
s32 func_80094EA4(void)
{
    u32 f;

    f = D_80083168[0];
    if ((f & 0x10) == 0)
    {
        if (f & 0xF0A3)
        {
            return 1;
        }
    }
    return 0;
}

/* Probes func_80095464 with the actor's 0x88 field temporarily replaced, and
   puts it back. 40 words, 0 differing. */
s16 func_80094ED4(u8 *a, void *b, u8 *c, u8 *d)
{
    u16 save;
    s16 r;

    save = *(u16 *)(d + 0x88);
    *(s16 *)(d + 0x88) =
        func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, save - 0x20);
    r = func_80095464(a, b, c, d);
    *(u16 *)(d + 0x88) = save;
    return r;
}

extern u8 D_800E0A76[];
extern u8 D_800E0A83[];
extern u8 D_800E0A90[];
extern u8 D_800E0AA1[];
extern u8 D_800E0ABC[];
extern u8 D_800E0AD8[];
extern u8 D_800E0B07[];
extern u8 D_800E0AFA[];
extern u8 D_80088A80[];
extern u8 D_80088A84[];
extern u16 D_80012094;
extern s16 D_80012096;

s32 func_80094F74(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *m;
    u8 *h;
    u8 *q;
    u8 *w;
    u8 *w2;
    u8 *bb;
    s16 *zz;
    s32 r;
    s32 a;
    s32 n;
    s32 v;

    g = (u8 *)&D_80083160;
    r = func_800A70E4(c[0x24], c[0x25], *(s16 *)(d + 0x88));
    if ((s16)r < 0)
    {
        if (*(s16 *)(p + 0x100) < 0)
        {
            return 1;
        }
        func_80095DD0(p, b, c, d);
        return 0;
    }
    m = func_800990FC();
    h = func_8009929C(8, m);
    if (D_80081485[0] != 0)
    {
        func_80099290(func_80099194(
            D_80088A80, func_80099368((u8 *)&D_800E3548_s[(s16)r], func_80099194(D_800E0A76, h))));
        func_800A5720(m);
        return 1;
    }
    a = func_80098FB0();
    n = func_80098FF8();
    if ((D_80013714 & 1) == 0)
    {
        if ((*(s32 *)(g + 0x8) & 0x20) != 0)
        {
            func_8009F644(d, 0x40, 0, 0);
            func_80099290(func_80099194(
                D_80088A80, func_80099368((u8 *)&D_800E3548_s[(s16)r], func_80099194(D_800E0A83, h))));
            func_800A5720(m);
            return 1;
        }
    }
    else
    {
        q = func_8009F868();
        if (q != NULL)
        {
            if ((q[1] & 0xF8) == 0x40)
            {
                return 1;
            }
            func_8009F988();
        }
    }
    if (D_800E3548_s[(s16)r].unk_01 == 0xE ||
        (D_800E3548_s[(s16)r].unk_01 == 0xC && D_800E3548_s[(s16)r].unk_00 == 4) ||
        (D_800E3548_s[(s16)r].unk_01 == 0x12 && D_800E3548_s[(s16)r].unk_00 == 1))
    {
        h = func_80099368((u8 *)&D_800E3548_s[(s16)r], h);
        if (D_800E3548_s[(s16)r].unk_01 == 0xE)
        {
            h = func_80099194(D_80088A84, h);
        }
        else
        {
            h = func_80099194(D_800E0A90, h);
        }
        *(u16 *)(p + 0xA2) |= 0x80;
        if (D_800E3548_s[(s16)r].unk_01 == 0xC)
        {
            h = func_80099194(D_800E0ABC,
                              func_8009929C(0xA, func_80099194(D_800E0AA1, func_8009929C(0xA, h))));
            p[0xFB] = 1;
            zz = &D_80012096;
            if (D_80012094 != 3)
            {
                *zz = 3;
            }
            else
            {
                *zz = 5;
                /* DEVICE: worth 5 words (0x80095248..0x80095260). %hi of
                   D_80012096 must be allocated before the constant 3, and
                   allocno_compare gives it 1*3/16 against the constant's
                   1*3/6 (both live lengths already doubled by
                   update_equiv_regs). This asm emits nothing but adds two
                   references to the symbol's high -- 2*5/18 = 5555 against
                   5000 -- without adding an instruction, because
                   update_equiv_regs deletes the lo_sum it keeps alive.
                   No legal spelling reaches it: three references is all two
                   stores can produce. Measured, all 7 differing unless noted:
                   `D_80012096 = 3/5` with no pointer local (317 words, 142);
                   constant addresses for both (318, 141); a page-base local
                   (316, 11); `__asm__("" ::"r"(zz))` (317, 147 -- the "r"
                   forces the whole address into a register); the arms swapped
                   (9); `zz` assigned earlier (7); `zz[0] =` (7). The same asm
                   anywhere but the end of the else arm perturbs the delay-slot
                   fill and costs words or order (11 to 144 differing). */
                __asm__("" : "=m"(D_80012096) : "m"(D_80012096));
            }
            *(s16 *)0x8001209A = 0;
        }
        else if (D_800E3548_s[(s16)r].unk_01 == 0xE)
        {
            /* DEVICE: worth 2 words (the `addu`/`lbu` pair at 0x800951E8).
               The slot address is a block-local pseudo here, so local-alloc's
               combine_regs ties it to the `sra` that feeds it and it inherits
               $v0; the target keeps them apart in $a0. This reference in a
               second basic block makes reg_basic_block -1, which is exactly
               the `reg_qty[sreg] == -1` leg of combine_regs that refuses the
               tie, and the address then conflicts with the 0xC/0xE constant
               in $v0 and the kind byte in $v1 and lands in $a0. No legal
               spelling reaches it: the target's 0xE arm touches nothing in
               the slot, so nothing in C can reference the address there.
               Measured alternatives, all 7 differing: the kind byte in a u8
               local compared twice; `((u16 *)p)[0x51] |= 0x80`;
               `*(u16 *)(p+0xA2) = *(u16 *)(p+0xA2) | 0x80`; 11 differing for
               `((UnkDungeonSlot *)&D_800E3548_s[(s16)r])->unk_01`. */
            __asm__("" ::"m"(D_800E3548_s[(s16)r].unk_01));
            v = func_800438E4();
            *(s32 *)(p + 0xFC) += v;
            func_800B4C7C(0x97, d, (s16)v, 1);
        }
        w = (u8 *)0x80010298;
        *(s32 *)w = *(s32 *)&D_800E3548_s[(s16)r];
        D_800E3548_s[(s16)r].unk_00 = 0;
        D_800E3548_s[(s16)r].unk_01 = 0;
        func_8009A3D0(D_800E36C8[(s16)r].unk_00, D_800E36C8[(s16)r].unk_01, 0x800);
        *(u8 **)(p + 0xBC) = w;
    }
    else if ((s16)a < 0 || (s16)n < 0)
    {
        h = func_80099194(D_80088A80,
                          func_80099368((u8 *)&D_800E3548_s[(s16)r],
                                        func_80099194(D_800E0AFA,
                                                      func_8009929C(0xA, func_80099194(D_800E0AD8, h)))));
    }
    else
    {
        h = func_80099194(D_800E0B07, func_80099368((u8 *)&D_800E3548_s[(s16)r], h));
        w2 = (u8 *)0x80010248 + (s16)a * 4;
        *(s32 *)w2 = *(s32 *)&D_800E3548_s[(s16)r];
        bb = (u8 *)0x80010000;
        *(u8 **)(bb + (s16)n * 4 + 0x29C) = w2;
        *(u16 *)(p + 0xA2) |= 0x80;
        D_800E3548_s[(s16)r].unk_00 = 0;
        D_800E3548_s[(s16)r].unk_01 = 0;
        func_8009A3D0(D_800E36C8[(s16)r].unk_00, D_800E36C8[(s16)r].unk_01, 0x800);
        *(u8 **)(p + 0xBC) = w2;
    }
    func_80099290(h);
    func_800A5720(m);
    return 1;
}

/* Probe the tile the second argument's (0x24, 0x25) and the fourth's facing
   pick, park the index at +0x100 and report whether the caller may go on.

   THREE `return`s, AND THE `return 0` IS LAST. Sharing one `return 1` for
   both success paths is 30 words against 31 with 19 differing -- the target
   materialises the 1 twice, once in the `bltz`'s delay slot and once in the
   fall-through tail -- and writing the `return 0` as the middle arm's early
   exit is 32 words / 10 differing, because the extra `j` to the epilogue
   survives. An accumulator with one `return v` is a genuine tie at 31 / 0.
   31 words, 0 differing. */
s16 func_80095464(u8 *a, void *b, u8 *c, u8 *d)
{
    s16 r;

    r = func_800B500C(c[0x24], c[0x25], *(s16 *)(d + 0x88));
    *(s16 *)(a + 0x100) = r;
    if (r < 0)
    {
        return 1;
    }
    if ((D_800E3648_s[r].unk_03 & 0x80) == 0)
    {
        *(s16 *)(a + 0x100) = -1;
        return 1;
    }
    return 0;
}

/* 22 words, 0 differing. */
s32 func_800954E0(u8 *p, void *b, void *c, u8 *d)
{
    s16 i;
    u8 f;

    i = *(s16 *)(p + 0x100);
    if (i < 0)
    {
        return 1;
    }
    f = D_800E3648_s[i].unk_03;
    if (!(f & 0x80))
    {
        return 1;
    }
    if (f & 0x40)
    {
        return 0;
    }
    D_800E3648_s[i].unk_03 = f & 0x7F;
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088A80);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088A84);

/* The 21-case item dispatcher: draw two records out of func_8009FADC into
   D_80082EB0 and its neighbour, then act on the item's type byte.
   THE CASE BODIES ARE EMITTED IN SOURCE ORDER, so 15/16 and 17 must be
   written BEFORE 1..14 -- with 1..14 first the three arms come out in the
   wrong places and it is 96 words, 28 differing. 96 words, 0 differing.
   The `sltiu 0x15` bound is what fixes cases 19..21 as real (empty) case
   labels rather than a shorter table plus a default.
   `p` and `q` are two pointers to the same record: the copy survives because
   the case-18 arm reads one and every other arm reads the other.
   This function OWNS jtbl_80088AA0, whose address is 0 mod 8, so cc1psx's
   `.align 3` is a no-op; the 4 bytes splat shows after the 21 entries are
   regenerated by func_80095A10's own `.align 3` for jtbl_80088AF8. */
s16 func_80095538(u8 *p, s16 b, s16 c)
{
    u8 *g;
    u8 *q;
    UnkVec *m;
    u32 k;
    s32 r;

    r = 1;
    q = p;
    m = *(UnkVec **)(q - 0x18);
    k = *(u32 *)(q - 0x14);
    g = (u8 *)&D_80082EB0;
    D_80082EB0 = func_8009FADC(b);
    if (c != 0)
    {
        *(u8 **)(g + 4) = func_8009FADC(c);
    }
    else
    {
        *(u8 **)(g + 4) = NULL;
    }
    switch (D_80082EB0[1])
    {
    case 15:
    case 16:
        func_800982A8(q, (D_80082EB0 == *(u8 **)(q + 0x4C)) ? NULL : D_80082EB0);
        break;
    case 17:
        func_80098614(q, (D_80082EB0 == *(u8 **)(q + 0x50)) ? NULL : D_80082EB0);
        break;
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
    case 9:
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
        r = func_80098920(q, D_80082EB0, 3, 0);
        break;
    case 18:
        *(u8 **)(p + 0xBC) = D_80082EB0;
        func_8008D388(p, m, k, q);
        break;
    case 19:
    case 20:
    case 21:
        break;
    }
    return r;
}

/* The engine's +0x1C 0x20000 flag picks between announcing the record
   D_800E3DF0[q[3] & 0x1F] names and re-arming it. The announce half builds
   the message out of func_800990FC's handle: with a key the three-part
   D_800E0B18 / D_800E05E1-or-D_800E05F0 / D_800E0B2B chain, without one the
   single D_800E202D line, and the two halves CROSS-JOIN onto the last
   func_80099194 call, which is why that call is written in both arms.

   TWO THINGS WERE WORTH WORDS:
   * the `key >= 0` arm must be written FIRST. As `if (key < 0)` the target's
     `bltz` becomes a `bgez` and the arms swap out of line: 102 words,
     54 differing.
   * `e` IS REASSIGNED, not a second variable. The 0x20000000 gate's record
     goes back into the same local, so when that gate fails func_800A1BD0
     runs on the RECORD and not on the table entry -- which is what keeps the
     record in $s0 and makes reorg fill the `beqz`'s slot with
     `addu $a0, $s0, $zero`. With a separate `r` the load lands straight in
     $a0, the copy disappears and it is 102 words, 38 differing.

   103 words, 0 differing. */
void func_800956B8(u8 *p, void *b, void *c, u8 *q)
{
    u8 *e;
    u8 *r;
    void *h;
    void *m;
    s16 k;

    e = (u8 *)D_800E3DF0[q[3] & 0x1F];
    if ((*(u32 *)(e + 0x1C) & 0x20000) == 0)
    {
        k = func_800A1BD0(e);
        h = func_800990FC();
        m = h;
        if (k >= 0)
        {
            h = func_80099194(D_800E0B18, h);
            h = func_80099194(k != 0 ? D_800E05F0 : D_800E05E1, h);
            h = func_80099194(D_800E0B2B, (u8 *)h - 3);
        }
        else
        {
            h = func_80099194(D_800E202D, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    else
    {
        k = func_800A1C14(e);
        if (k >= 0)
        {
            e = *(u8 **)(p + k * 4 + 0xAC);
            if (*(u32 *)(e + 0x14) & 0x20000000)
            {
                *(u32 *)(e + 0x1C) |= 0x10000;
                func_800ACB98(e, *(u8 **)(e - 0x18), *(u8 **)(e - 0x14), e);
                return;
            }
        }
        k = func_800A1BD0(e);
        if (k >= 0)
        {
            r = *(u8 **)(p + k * 4 + 0xAC);
        }
        else
        {
            r = *(u8 **)(p + 0x60);
        }
        r[0x9A] = 2;
        r[0x9B] = 0;
        *(u32 *)(r + 0x8C) = 0;
        *(u8 **)(p + 0x110) = q;
    }
}

/* Put the actor into state 0x31, point its renderable at the direction table,
   aim it at the tile the mover's +0x2A facing picks off the engine's sine and
   cosine tables, and probe that tile with func_8009B88C -- twice, with a
   func_8009A21C/func_8009A3D0 bracket, when the +0x124 record's +0x13 byte is
   positive. 111 words, 0 differing.

   ONE SHARED TRAILING `return 0`, WITH BOTH SUCCESS ARMS RETURNING 1.
   Written as two `if (...) { return 0; }` early exits the two zeroing blocks
   cross-jump into ONE block in the middle of the function and the second test
   inverts to a `bnez` plus a `j`: 111 words / 18 differing. Nested inside one
   `if (func_8009B88C(...) != 0)` with a single `return 0` at the bottom, the
   second test is a `beqz` straight to the epilogue with the zero in its delay
   slot, and the two `p[0x73]` tails cross-jump as the target has them.

   The flag is `s16` (the tests are `sll 16`/`beqz`), the table index is read
   off the mover's facing as `(*(u16 *)(p + 0x2A) >> 9) & 7`, and combine
   narrows both table loads to `lbu` because only the low byte of the sum is
   stored. */
s32 func_80095854(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *t;
    s16 f;
    s32 k;
    s16 x;
    s16 y;
    s16 x2;
    s16 y2;

    f = 0;
    e = *(u8 **)((u8 *)p + 0x124);
    p->unk_9A = 0x31;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    if (e != NULL)
    {
        f = *(s8 *)(e + 0x13) > 0;
    }
    func_80094E34();
    t = D_800DD130;
    *(u8 **)(c + 0x2C) = t;
    func_80048A44(c, t[((D_80083228 + *(s16 *)((u8 *)p + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    D_80082EB0 = d;
    k = (*(u16 *)((u8 *)p + 0x2A) >> 9) & 7;
    *((u8 *)p + 0x72) = c[0x24] + D_8006CCD8[k];
    *((u8 *)p + 0x73) = c[0x25] + D_8006CCE8[k];
    if (func_8009B88C(NULL, *(s8 *)((u8 *)p + 0x72), *(s8 *)((u8 *)p + 0x73), &x, &y) != 0)
    {
        if (f != 0)
        {
            func_8009A21C(x, y, 0x8000);
            f = func_8009B88C(NULL, *(s8 *)((u8 *)p + 0x72), *(s8 *)((u8 *)p + 0x73), &x2, &y2);
            func_8009A3D0(x, y, 0x8000);
            if (f != 0)
            {
                *((u8 *)p + 0x72) = x2;
                *((u8 *)p + 0x73) = y2;
                return 1;
            }
        }
        else
        {
            *((u8 *)p + 0x72) = x;
            *((u8 *)p + 0x73) = y;
            return 1;
        }
    }
    return 0;
}

/* State machine over +0x9B for the treasure/item reveal: state 0 drops the
   D_800DD138 skin and the 0x200 flag and runs func_80093C70/func_80093D8C,
   states 1 and 3 load a resource through func_800A1618 and hand it to
   func_800A0B94, state 2 waits on the D_800E3E40 handshake byte, state 3
   builds the entity and dresses it, states 4 and 5 re-skin from D_800DD140
   and D_800DCFB0, and state 6 closes on func_800956B8 and parks
   func_8008ACDC.  jtbl_80088AF8 is at rodata offset 0x398, 0 mod 8, so
   cc1psx's `.align 3` is a no-op.  240 words, 0 differing.

   STATE 3 CALLS func_800A0B94's RESULT.  func_800A0B94 is declared `s32`
   here and returns `*(s32 *)t`, the first word of the image buffer it just
   loaded; state 3 casts that word to a function pointer and calls it with
   (0x6, the two signed bytes at d+0x72/+0x73, and `(s16)(*(u16 *)(d + 0x88)
   - 0x20)`).  The listing's `jalr $v0` immediately after the `jal` is the
   whole witness.

   ONE TABLE POINTER LOCAL PER CASE.  Cases 0, 4 and 5 each park a facing
   table at the renderable's +0x2C and index it.  Sharing ONE local across
   the three is 240/240 with 10 differing -- all ten of them the three
   `lui`/`addiu` pairs, which become a global-alloc quantity and stop sharing
   a register with the `lo_sum`.  Three locals make each a local-alloc
   quantity and each pair lands in $a1, which is the target.  Same rule as
   func_80097DB8's note on D_80083460 and func_800974DC's on D_800DD274.

   THE TWO `|=` ON +0x1C ARE ONE LOAD, TWO `or`s AND ONE STORE, so they go
   through an `s32` local: written as two `*(s32 *)(q + 0x1C) |= C;`
   statements the second would reload, and written as one `|= 0x2020000` the
   constants fold into a single `lui`/`or`. */
void func_80095A10(u8 *p, u8 *b, u8 *c, u8 *d)
{
    u8 *e;
    u8 *q;
    u8 *t;
    u8 *t2;
    u8 *t3;
    u8 *w;
    u8 *g;
    s32 f;
    s16 n;

    e = D_80082EB0;
    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            t = D_800DD138;
            *(u8 **)(c + 0x2C) = t;
            func_80048A44(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u16 *)(c + 0x14) |= 0x200;
            func_80093C70(p, b, c);
            func_80093D8C(p, b, c);
            p[0x9B]++;
        }
        break;
    case 1:
        q = func_800A1618(*e, 2);
        if (q != NULL)
        {
            D_80081488 = (u32)func_80047DB8(*e);
            func_800A0B94(*e, q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)&D_800E3E40);
        }
        p[0x9B]++;
        break;
    case 2:
        if (D_800E3E40 != 0)
        {
            func_80047DF0();
            p[0x9B]++;
        }
        break;
    case 3:
        q = func_800A1618(*e, 2);
        if (q == NULL)
        {
            return;
        }
        func_80043914(D_80082EB0);
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(*e, q, 1))(
            6, *(s8 *)(d + 0x72), *(s8 *)(d + 0x73), (s16)(*(u16 *)(d + 0x88) - 0x20));
        if (q != NULL)
        {
            *(u8 **)(d + 0x60) = q;
            D_800E3DF0[e[3] & 0x1F] = (s32)q;
            w = *(u8 **)(q - 0x14);
            *(s16 *)(w + 0x1E) = 0x800;
            *(s16 *)(w + 0x1C) = 0x800;
            w = *(u8 **)(q - 0x18);
            *(s16 *)(w + 0x2) = *(u16 *)(b + 0x2);
            *(s16 *)(w + 0x6) = *(u16 *)(b + 0x6);
            *(s16 *)(w + 0xA) = *(u16 *)(b + 0xA) - 0x10;
            func_80042640((Unk42710 *)q, q[0x13]);
            func_800424E0(q, q[0x13], e);
            f = *(s32 *)(q + 0x1C);
            f |= 0x2000000;
            f |= 0x20000;
            *(s32 *)(q + 0x1C) = f;
            func_80042560((s8 *)q);
            func_800A56E0(0x704);
            func_80042B68((s8 *)q, 3);
            func_80042B68((s8 *)q, 1);
            if (q[0x25] == 0)
            {
                q[0x25] = 1;
            }
        }
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B]++;
        break;
    case 4:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0)
        {
            t2 = D_800DD140;
            *(u8 **)(c + 0x2C) = t2;
            func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 5:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(u16 *)(c + 0x14) &= 0xFDFF;
            t3 = D_800DCFB0;
            *(u8 **)(c + 0x2C) = t3;
            func_80048A44(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            p[0x9B]++;
        }
        break;
    case 6:
        if ((*(s32 *)(*(u8 **)(d + 0x60) + 0x14) & 0x100000) == 0)
        {
            func_800956B8(p, b, c, e);
            *(s32 *)(p + 0x8C) = (s32)func_8008ACDC;
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x812;
        }
        break;
    }
}

/* State 0x15, a global counter bump and the fourth argument parked at
   D_80083460 + 0xC.

   THE COUNTER BUMP COMES FIRST. With the `sw` written ahead of it the load
   delay after the `lhu` can only be a nop; with the bump first, reorg drops
   the `sw` into that slot, which is the target's order.
   11 words, 0 differing. */
void func_80095DD0(u8 *p, void *b, void *c, void *d)
{
    u8 *g;

    p[0x9A] = 0x15;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x14) += 1;
    *(void **)(g + 0xC) = d;
}

void func_80095DFC(u8 *p, void *b, void *c, u8 *d)
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *e;
    void *m;
    u8 *h;
    u8 *q;
    u8 *t;
    u8 *tbl;
    u8 *tbl2;
    s16 k;
    s32 st;

    st = p[0x9B];
    if (st == 1)
    {
        goto state1;
    }
    if (st > 1)
    {
        return;
    }
    if (st != 0)
    {
        return;
    }
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(d) != 0)
    {
        return;
    }
    tbl = D_800E3648;
    e = &tbl[*(s16 *)(p + 0x100) * 4];
    if (e[0] == 0)
    {
        func_80096088((UnkDungeonActor *)p, d);
        *(s32 *)(g + 0xC) = 0;
        *(s16 *)(g + 0x14) = *(u16 *)(g + 0x14) - 1;
        *(u16 *)(g + 0x2) &= 0xFFF7;
        return;
    }
    if ((e[3] & 0x40) == 0)
    {
        h = func_800990FC();
        m = h;
        h = func_8009929C(8, h);
        h = func_80099194(D_800E0B39, h);
        h = func_80099978(h);
        q = h - 2;
        h = func_80099194(D_80088B14, h);
        t = h;
        h = func_80099368(&tbl[*(s16 *)(p + 0x100) * 4], h);
        func_800998C0(q, t);
        h = func_8009929C(0xA, h);
        h = func_80099194(D_800E0B4C, h);
        h = func_800999B0(h);
        func_80099290(h);
        func_800A5720(m);
    }
    p[0x9B] += 1;
    func_800954E0(p, b, c, d);
state1:
    tbl2 = D_800E3648;
    e = &tbl2[*(s16 *)(p + 0x100) * 4];
    if (e[0] == 0)
    {
        func_80096088((UnkDungeonActor *)p, d);
        g2 = (u8 *)&D_80083460;
        *(s32 *)(g2 + 0xC) = 0;
        *(s16 *)(g2 + 0x14) = *(u16 *)(g2 + 0x14) - 1;
        *(u16 *)(g2 + 0x2) &= 0xFFF7;
        return;
    }
    k = (s16)func_800B627C(e[0], d, *(s16 *)(p + 0x100), 0);
    if (k != 0)
    {
        g3 = (u8 *)&D_80083460;
        *(s32 *)(g3 + 0xC) = 0;
        *(s16 *)(g3 + 0x14) = *(u16 *)(g3 + 0x14) - 1;
        *(u16 *)(g3 + 0x2) &= 0xFFF7;
        if (k > 0)
        {
            func_80096088((UnkDungeonActor *)p, d);
        }
        p[0x9B] += 1;
    }
}

/* One of three handlers into the record's +0x8C, picked by a halfword
   predicate and then by the +0x1C flag word. The `sll $v0, 16` with no `sra`
   is the `(s16)` on func_80042900's result: only its low half is tested.
   30 words, 0 differing. */
void func_80096088(UnkDungeonActor *a, u8 *p)
{
    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        a->unk_8C = func_80096384;
    }
    else if ((*(s32 *)(p + 0x1C) & 0x100000) != 0)
    {
        a->unk_8C = func_8008EAC8;
    }
    else
    {
        a->unk_8C = func_8008ACDC;
    }
}

/* The dungeon's once-per-frame wall/floor retint: refuse while D_800E296C
   carries 0x40000, while 0x80013714 has 0xC or 1, in actor state 0x19 or
   while func_800A2BDC says something is busy; otherwise walk D_80083160's
   +0x8 and +0x10 input words and nudge the three colour registers, then set
   D_80080A84 from the two flag classes.  161 words, 0 differing.

   FOUR SPELLINGS ARE LOAD-BEARING.
   (1) THE ENTRY TEST IS A CHAIN OF `else if` ARMS THAT EACH SET `skip`, not
   an `&&` or `||` chain.  As one `||` condition gcc folds the two
   0x80013714 masks into a single `andi 0xD` (the two bit tests are two arms
   of one fold) and the function is 156 words / 143 differing; as a nested
   `if` pair with a `goto` it is 158 / 130.  And the LAST arm has to be
   written `else if (... == 0) { r = 1; } else { skip = 1; }` -- with the
   test the other way round the arms swap sides of the branch and it is
   161 / 7.
   (2) `skip` AND `r` ARE NARROW LOCALS and `dirty` IS `s32`.  `skip`'s read
   is the target's `addu $v0, $s0, $zero` before the `beqz` -- a mode
   conversion whose extension combine drops, leaving a copy cse cannot
   propagate (the func_80098614 lever).  `r` must share `skip`'s mode so
   that `r = 0` comes out as a COPY of skip's zero, which is the target's
   `addu $s3, $s0, $zero`.  All of `s32 skip` (160/136), `s32 r` (161/5),
   `s16 dirty` and `u8 dirty` (162/80) are worse.
   (3) 0x80013714 IS A CONSTANT ADDRESS, not the declared `D_80013714`: the
   symbol spelling puts the `lui` in its own register where the target uses
   the load's own destination (161/3, the lever at
   docs/matching-levers.md "THE `lui` TEMP REGISTER TELLS SYMBOL FROM
   CONSTANT ADDRESS").
   (4) THE TWO 0x200 ARMS EACH NEED THEIR OWN POINTER LOCAL for D_800DCE60,
   because each materialises the base once for a load and a store at +4;
   reaching it as `*(u16 *)(D_800DCE60 + 4)` folds the %lo into the
   displacement and is two words short (159 words).  The two TABLE reads are
   the opposite: `D_800DD264[D_800120A2]` written INLINE, because a pointer
   local there materialises the base before the index's store instead of
   after it -- 26 differing. */
s32 func_80096100(u8 *p)
{
    u8 *g;
    u8 *q;
    u8 *q2;
    s16 skip;
    s16 r;
    s32 dirty;

    skip = 0;
    r = 0;
    dirty = 0;
    g = (u8 *)&D_80083160;
    if ((D_800E296C & 0x40000) != 0)
    {
        skip = 1;
    }
    else if ((*(u16 *)0x80013714 & 0xC) != 0)
    {
        skip = 1;
    }
    else if ((*(u16 *)0x80013714 & 1) != 0)
    {
        skip = 1;
    }
    else if (p[0x9A] == 0x19)
    {
        skip = 1;
    }
    else if ((s16)func_800A2BDC(NULL) == 0)
    {
        r = 1;
    }
    else
    {
        skip = 1;
    }
    if (skip != 0)
    {
        return r;
    }
    if (D_800832B4 == 0)
    {
        if ((*(s32 *)(g + 0x8) & 0x10) == 0)
        {
            if ((*(s32 *)(g + 0x8) & 0x8) != 0)
            {
                q = D_800DCE60;
                *(s16 *)(q + 4) = *(u16 *)(q + 4) - 0x200;
                dirty = 1;
            }
            else if ((*(s32 *)(g + 0x8) & 0x4) != 0)
            {
                q2 = D_800DCE60;
                *(s16 *)(q2 + 4) = *(u16 *)(q2 + 4) + 0x200;
                dirty = 1;
            }
        }
        if ((*(s32 *)(g + 0x8) & 0x10) != 0 && (*(s32 *)(g + 0x10) & 0x1) != 0)
        {
            D_800120A2 = (D_800120A2 + 1) & 3;
            dirty = 1;
            *(s16 *)D_800DCE60 = D_800DD264[D_800120A2];
        }
        if (dirty != 0)
        {
            D_800832B4 = 0;
            func_8004D7A8(1);
            func_8004D294(NULL, D_800DCE60, 1);
        }
    }
    if ((*(s32 *)(g + 0x8) & 0x10) != 0 && (*(s32 *)(g + 0x10) & 0x4) != 0)
    {
        D_800120A0 = (D_800120A0 + 1) & 3;
        D_800E296C |= 0x40;
        D_800DCE66 = D_800DD26C[D_800120A0];
    }
    if ((*(s32 *)(g + 0x8) & 0x10) != 0 && (*(s32 *)(g + 0x10) & 0x2) != 0)
    {
        func_800DC888();
    }
    if (func_80033BC0(0xA2) != 0)
    {
        if ((*(s32 *)(g + 0x10) & 0xC0000) != 0)
        {
            D_80080A84 = 1;
        }
        else if ((*(s32 *)(g + 0x8) & 0x30000) != 0)
        {
            D_80080A84 = 2;
        }
    }
    return r;
}

void func_80096384(u8 *p, void *b, u8 *c, u8 *d)
{
    Unk80083160 *g = &D_80083160;
    u8 *q;
    void **t;
    void **t2;
    s32 cur;
    u8 *m;
    u16 k;
    s32 e;
    s32 f;
    s32 w;
    s32 n;
    s32 r;
    s32 delta;
    s16 want;
    if (p[0x9A] != 0x32)
    {
        t = D_800DD274;
        *((void ***)(c + 0x2C)) = t;
        func_8003DB94(c, t[(((g->unk_C8 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
        p[0x9A] = 0x32;
        *((u16 *)(p + 0xA2)) &= 0xFEFF;
        *((u16 *)(c + 0x14)) &= 0xF7FF;
        p[0x9B] = 0;
        *((s16 *)(p + 0x96)) = 0;
        func_800A4300(c, d);
    }
    q = (u8 *)(&D_80083460);
    *((u16 *)(q + 0x2)) &= 0xFF7F;
    r = *((s16 *)(d + 0x64));
    if (r < 0)
    {
        func_80097898(p, b, c, d);
        return;
    }
    if (r > 0)
    {
        func_8008CBA0(p, b, c, d);
    }
    if (((*((s32 *)(d + 0x14))) & 0x20000) != 0)
    {
        *((s32 *)(d + 0x14)) &= 0xFFFDFFFF;
    }
    if (((*((u16 *)(p + 0xA2))) & 0x10) == 0)
    {
        func_80096AC0(p, b, c, d);
        return;
    }
    e = *((s32 *)(d + 0x1C));
    if ((e & 0x200) != 0)
    {
        func_80097C50((UnkDungeonActor *)p, b, c, d);
        return;
    }
    if (((*((u16 *)(q + 0x2))) & 0x4) == 0)
    {
        if ((e & 0x20) != 0)
        {
            func_800969B8(p, b, c, d);
            return;
        }
        k = *((u16 *)0x80013714);
        if ((k & 1) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((*((u16 *)(p + 0xA2))) & 0x200) != 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFDFF;
            }
            m = func_8009F868();
            if (m == 0)
            {
                return;
            }
            want = (m[0x1] & 7) << 9;
            *((s16 *)(d + 0x2A)) &= 0xFFF;
            if ((*((s16 *)(d + 0x2A))) != want)
            {
                if (((*((u16 *)(d + 0x2A))) & 0x800) != 0)
                {
                    *((s16 *)(d + 0x2A)) |= 0xF800;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) &= 0x7FF;
                }
                do
                {
                    want = (want & 0x800) ? (want | 0xF800) : (want & 0x7FF);
                    cur = *((u16 *)(d + 0x2A));
                    f = (u16)want;
                    f <<= 16;
                    delta = (*((s16 *)(d + 0x2A))) - (f >> 16);
                } while (0);
                if (delta < 0)
                {
                    delta = -delta;
                }
                if (delta > 0x800)
                {
                    *((s16 *)(d + 0x2A)) = (want & (~0xFFF)) | (cur & 0xFFF);
                }
                if ((*((s16 *)(d + 0x2A))) > (f >> 16))
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) - 0x200;
                }
                else
                {
                    *((s16 *)(d + 0x2A)) = (*((u16 *)(d + 0x2A))) + 0x200;
                }
                func_8009F988();
                goto tail;
            }
            D_800E3544 = m[0x1] & 0xF8;
            switch (D_800E3544)
            {
            case 8:
                func_80096C24(p, b, c, d);
                return;

            case 0x10:
                func_800969B8(p, b, c, d);
                return;

            case 0x18:
                func_800973F0(p, b, c, d);
                goto tail;

            case 0xD8:
                D_800DD830[m[0x0] & 0x7F]();
                goto tail;
            }

            goto tail;
        }
        D_800E3544 = 0;
        if ((k & 4) != 0)
        {
            func_80040AA0(3);
            return;
        }
        if (((s16)func_80094EA4()) != 0)
        {
            *((s16 *)(p + 0xA4)) = 0;
        }
        if ((((((*((u16 *)(p + 0xA2))) & 0x200) != 0) && (((s16)func_800A2C34(d)) == 0)) && (((*((u16 *)(q + 0x2))) & 0x4) == 0)) && (((s16)func_800A4474(c[0x24], c[0x25])) != 0))
        {
            *((u16 *)(p + 0xA2)) &= 0xFDFF;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_800245A8);
            return;
        }
        f = g->unk_08;
        if ((f & 0x80) != 0)
        {
            *((s16 *)(d + 0x8A)) = 2;
            D_800E4940 = 2;
            func_8008CF6C((UnkDungeonActor *)p, b, c, D_8004F5F4);
            D_80082EB0 = 0;
            *((s32 *)(p + 0xC8)) = 0;
            *((s32 *)(p + 0x104)) = 0;
            return;
        }
        if ((f & 3) != 0)
        {
            func_800A56E0(0x506);
            return;
        }
        n = func_8009074C(*((s16 *)(p + 0x9E)), (u16 *)(p + 0xA2), (u16 *)(d + 0x2A));
        if (((s16)n) != 0xFFF)
        {
            *((s16 *)(d + 0x2A)) = n;
            if (((g->unk_08 & 0x10) == 0) && (((*((u16 *)(p + 0xA2))) & 0x400) == 0))
            {
                *((u16 *)(p + 0xA2)) &= 0xFFFE;
                if (((*((s32 *)(d + 0x1C))) & 0x400) != 0)
                {
                    *((u16 *)(d + 0x2A)) += func_800A6D30() & 0xE00;
                }
                func_80096C24(p, b, c, d);
                if (p[0x9A] != 0x32)
                {
                    return;
                }
            }
            else
            {
                *((s16 *)(p + 0x96)) = 0;
            }
        }
        w = g->unk_08;
        if ((w & 0x30) == 0x30)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            func_800969B8(p, b, c, d);
            return;
        }
        if (((*((u16 *)(p + 0xA2))) & 0x40) != 0)
        {
            if ((w & 0x40) == 0)
            {
                *((u16 *)(p + 0xA2)) &= 0xFFBF;
            }
        }
        else if ((w & 0x40) != 0)
        {
            *((u16 *)(p + 0xA2)) &= 0xFFFE;
            if (((s16)func_800973F0(p, b, c, d)) != 0)
            {
                return;
            }
        }
    }
tail:
    if (((*((u16 *)(p + 0xA2))) & 0x1) != 0)
    {
        *((u16 *)(p + 0xA2)) &= 0xFFFE;
        t2 = D_800DD274;
        *((void ***)(c + 0x2C)) = t2;
        func_8003DB94(c, t2[(((D_80083228 + (*((s16 *)(d + 0x2A)))) + 0x100) >> 9) & 7], 0);
    }
}

/* 0x80013714 IS A CONSTANT ADDRESS, NOT THE SYMBOL. `D_80013714 & 2` -- the
   declaration structs.h:2129 gives it, and the spelling every other landed
   caller in this unit uses -- emits the same two words but as
   `(high (symbol_ref))` + `(mem (lo_sum ...))`, and sched2 then hoists the
   `lui` to the second slot of the prologue: 3 differing, in every store
   order and both condition spellings. `*(u16 *)0x80013714` makes the `lui`
   a plain high-constant move, which loses the tie to the two remaining
   stores, and the prologue comes out sb/sb/sw/lui. A `--raw` run confirms
   the reading: words 7 and 8 are byte-identical unmasked, so there is no
   relocation there at all -- 0x80013714 is one of structs.h:2190's
   out-of-image addresses. 39 words, 0 differing. */
void func_800969B8(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 0x33;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if ((*(u16 *)0x80013714 & 2) || ((D_80083168[0] & 0x20) && func_800A5C70()))
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x2) |= 0x80;
    }
    func_8009F644(d, 0x10, 0, 0);
}

/* 27 words, 0 differing. */
void func_80096A54(u8 *a, UnkVec *v, u8 *b, u8 *c)
{
    u8 *g;

    func_80099F70(*(u8 **)(c + 0x5C));
    func_80099F04(*(u8 **)(c + 0x5C));
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x2) |= 0x812;
    *(s32 *)(a + 0x8C) = (s32)func_80096384;
}

/* Installer: state 0x34, handler slot cleared, bit 0 of the +0xA2 halfword
   dropped. 8 words, 0 differing. */
void func_80096AC0(u8 *p, void *b, u8 *c, u8 *d)
{
    p[0x9A] = 0x34;
    p[0x9B] = 0;
    *(void **)(p + 0x8C) = NULL;
    *(u16 *)(p + 0xA2) &= ~1;
}

/* A three-case switch whose case 0 FALLS THROUGH into case 1, like
   func_80091958 above. THE +0x88/+0x8A TEST IS WRITTEN `>= 0x41`, NOT
   `< 0x41`: neither arm returns, so the arms are laid out in SOURCE order,
   and with the short arm written first the call arm lands at the far label
   and the function is 8 differing. The 1 stored at +0x96 in the short arm is
   the switch's own `addiu $a0, $zero, 1` reused. 81 words, 0 differing. */
void func_80096AE0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 t;
    u16 f;

    switch (p[0x9B])
    {
    case 0:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        if (*(s16 *)(d + 0x88) - *(s16 *)(d + 0x8A) >= 0x41)
        {
            func_800419EC(8, 0x10);
            *(s16 *)(p + 0x96) = 0xC;
        }
        else
        {
            *(s16 *)(p + 0x96) = 1;
        }
        p[0x9B]++;
    case 1:
        *(u16 *)(c + 0x14) |= 0x800;
        p[0x9B]++;
        break;
    case 2:
        t = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = t;
        if (t <= 0)
        {
            f = *(u16 *)(c + 0x14);
            *(u16 *)(c + 0x14) = f & 0xF7FF;
            if ((f & 0xE000) != 0 || (s16)func_80094EA4() != 0)
            {
                *(s32 *)(p + 0x8C) = (s32)func_80096384;
            }
        }
        break;
    }
}

/* The entry probe of the dispatcher func_8008B9FC and func_8008C13C serve,
   and both are line-for-line templates for it: the same func_8009ABA0 probe
   on +0x2A/+0x88 with 0x20 on the stack, the same D_8006CCD8/D_8006CCE8
   eight-way step, the same f/h/q materialisations of D_80083460 and the same
   `r > 0` / `r <= 0` fork, with a D_800DD274 or D_800DD294 re-skin in each
   arm instead of func_8008B9FC's D_800E3544 tail. 266 words, 0 differing.

   THE FIRST FLAG RMW IS THE ARRAY-REF SPELLING, `*((u16 *)f + 1) |= 0x8`,
   AND IT IS WORTH THREE WORDS. Written `*(u16 *)(f + 0x2) |= 0x8` the
   address expression's top node is a cast, so expand_expr leaves
   MEM_IN_STRUCT_P clear; `f` holds a SYMBOL address so cse_rtx_addr_varies_p
   is 0, and `in_struct` is then the only clause left in invalidate_memory by
   which `*(s32 *)(p + 0x8C) = 0` could kill the cached load. It does not, so
   the D_80013714 arm reuses the pre-or value as one `ori $v0, $v1, 0x88` and
   the function comes out 263 words of 266 with 32 aligned differences. The
   array-ref form is in-struct, the store kills it, the reload appears, and
   the arm is the target's four words. docs/matching-levers.md, "WITH A
   CONSTANT-ADDRESS BASE, MEM_IN_STRUCT_P IS THE ONLY WAY A STORE CAN KILL A
   LOAD IN cse". The two `|= 0x80` arms are left as casts; only the cached
   side of the pair decides this.

   `h` IS ITS OWN LOCAL AND MUST STAY BELOW THE `(s16)r != 4` BLOCK. Assigned
   beside `f`, cse turns it into a copy of `f`, the second
   `lui`/`addiu` of D_80083460 at .L80096E08 disappears and it is 264 words
   with 38 aligned differences.

   Each D_800DD274 arm has its own pointer local, the rule func_800974DC
   records, and each facing index is ONE expression inline in the subscript so
   fold collapses the `* 4` into `sra 7` / `andi 0x1C`.

   The two `*(s16 *)(...+ 0x4) = 8` stores are spelled against D_80083460
   where splat's listing names D_80083464, exactly as func_8008B9FC,
   func_8008E264 and func_800971A8 already do, so D_80083464 needs no
   declaration. */
void func_80096C24(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *f;
    u8 *h;
    u8 *q;
    void **t;
    void **t2;
    void **t3;
    u8 *t4;
    void **t5;
    void **cur;
    s32 r;
    s32 k;

    /* DEVICE: worth 25 aligned differences and no words. Without it `p` and
       the D_80083460 base `h` come out with each other's callee-saved
       registers -- p in $s4 and h in $s3 where the target has p in $s3 and h
       in $s4 -- and every one of the 25 differences is that one swap, at 266
       of 266 words. `h` is 4 references over a ~40-insn range (pri 2*4/40)
       against p's 15 over the whole function (3*15/250); one more reference
       to p crosses the floor_log2 step at 16 and puts it in front in
       allocno_compare. This is the same device func_8008C13C carries on its
       own fourth parameter for the same $s3/$s4 rotation.
       Measured and rejected as legal spellings, all 266 words / 25 aligned
       differences: `*(u16 *)0x80013714` for the declared symbol; `cur`
       dropped so the +0x2C load is tested inline; a `pa = p` copy feeding
       `p[0x9B] = 8`, and another feeding `*(u16 *)(p + 0x98) |= 0xC`; an
       `h2 = h` copy so `h` carries 3 references instead of 4; `UnkVec *b`
       for `void *b`; and `*(s16 *)((u8 *)&D_80083460 + 0x4) = 8` for the
       `h + 0x4` store, which cse folds straight back onto `h`. Inlining the
       `k` facing index at both D_8006CCD8/E8 reads is 269 words / 45.
       It does not warn under -Wall. */
    g = (u8 *)&D_80083160;
    __asm__("" ::"r"(p));
    func_800A67F4();
    r = func_8009ABA0(*(s16 *)(d + 0x2A), b, c, *(s16 *)(d + 0x88), 0x20);
    if ((s16)r > 0)
    {
        p[0x9B] = 0;
        func_8009A3D0(c[0x24], c[0x25], 0x300);
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        c[0x24] += *(u8 *)&D_8006CCD8[k];
        c[0x25] += *(u8 *)&D_8006CCE8[k];
        func_8009A21C(c[0x24], c[0x25], 0x300);
        f = (u8 *)&D_80083460;
        *((u16 *)f + 1) |= 0x8;
        *(s32 *)(p + 0x8C) = 0;
        if ((s16)r != 4)
        {
            if ((D_80013714 & 2) != 0)
            {
                *(u16 *)(f + 0x2) |= 0x80;
                cur = *(void ***)(c + 0x2C);
                t = D_800DD274;
                if (cur != t)
                {
                    *(void ***)(c + 0x2C) = t;
                    func_8003DB94(c, t[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                }
            }
            else if ((*(s32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0)
            {
                t2 = D_800DD274;
                *(u16 *)(f + 0x2) |= 0x80;
                *(void ***)(c + 0x2C) = t2;
                func_8003DB94(c, t2[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            else
            {
                r = 5;
                p[0x9B] = 0xA;
            }
        }
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 0x80) != 0 || (s16)r == 1)
        {
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            if ((*(u16 *)(h + 0x2) & 0x80) == 0)
            {
                t3 = D_800DD274;
                *(void ***)(c + 0x2C) = t3;
                func_8003DB94(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            }
            p[0x9A] = 0x35;
            *(s16 *)(h + 0x4) = 8;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0xC;
            t4 = D_800DD294;
            *(u8 **)(c + 0x2C) = t4;
            func_8003DB94(c, *(void **)(t4 + (((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7) * 4), 0);
            if ((s16)r < 4)
            {
                *(u16 *)(p + 0xA2) |= 1;
            }
            else if ((s16)r == 2 || (s16)r == 4)
            {
                p[0x9B] = 8;
            }
            func_80094ED4(p, b, c, d);
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 8;
            p[0x9A] = 0x36;
            *(s16 *)(p + 0x96) = 2;
            *(s32 *)(d + 0x1C) |= 0x40000000;
            func_8009F644(d, 8, 0, 0);
            return;
        }
        func_8009F644(d, 8, 0, 0);
    }
    else
    {
        t5 = D_800DD274;
        *(void ***)(c + 0x2C) = t5;
        func_8003DB94(c, t5[((*(s16 *)(g + 0xC8) + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        *(s32 *)(p + 0x8C) = (s32)func_80096384;
        return;
    }
    func_80094ED4(p, b, c, d);
    *(s32 *)(d + 0x1C) |= 0x40000000;
    q = (u8 *)&D_80083460;
    *(u16 *)(q + 0x2) |= 0x812;
}

/* Slide the mover onto the record's tile over the engine's shared countdown
   at D_80083460 + 4, then close on func_80095DD0 or park func_80096384.

   THE DUPLICATED ARM IN THE FIRST `if` IS LOAD-BEARING, and it is worth the
   whole residue: 87 words / 4 differing without it, 87 / 0 with it. Written
   plainly, the `beqz` branches around a single store, so cse1's
   `skip_blocks` path extension (cse.c, `cse_end_of_basic_block`) carries the
   ENTRY block's table into the division block -- `g` still has its constant
   there, the divisor's address therefore does not vary, and the
   `b->unk_0C` store cannot invalidate it (`invalidate_memory` drops an
   in-memory entry only when it is in_struct or its address varies). The
   second division then reuses the guard's `lh` and the slot the target fills
   with its reload comes out as a `nop`. The inner `if`/`else` puts a
   CODE_LABEL inside the skipped block, the extension is refused, the division
   block starts a fresh cse run, and the divisor is re-read. jump2's
   cross-jumping merges the two identical arms back into one store and deletes
   the test, so the emitted code is unchanged -- every one of
   `c != NULL`, `b != NULL`, `p != NULL` and `d != NULL` gives the same 87
   words. Found by decomp-permuter (470 -> 0 in one minute) after fifteen
   hand spellings sat flat on 4; the ones that measured IDENTICAL CODE, i.e.
   changed nothing at all, were: the stores and the dividend reads respelled
   through `u8 *` casts, the divisor as an ARRAY_REF, a second base pointer
   local for the second division, the dividends hoisted into locals, the
   guard taken through a local, and `do { } while (0)` around the store.
   DEVICE: the duplicated arm is not what the original author wrote; it is a
   cse-region boundary. 87 words, 0 differing. */
void func_8009704C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s16 t;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        if (c != NULL)
        {
            *(s16 *)(g + 0x4) = 0;
        }
        else
        {
            *(s16 *)(g + 0x4) = 0;
        }
    }
    if (*(s16 *)(g + 0x4) != 0)
    {
        b->unk_0C = (((c[0x24] * 64 + 0x20) << 16) - b->unk_00) / *(s16 *)(g + 0x4);
        b->unk_10 = (((c[0x25] * 64 + 0x20) << 16) - b->unk_04) / *(s16 *)(g + 0x4);
    }
    if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
    {
        *(u16 *)(p + 0xA2) |= 1;
    }
    t = *(u16 *)(g + 0x4) - 1;
    *(s16 *)(g + 0x4) = t;
    if (t <= 0)
    {
        *(s16 *)(g + 0x4) = 0;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(c + 0x14) |= 0x4000;
        if (*(s16 *)(p + 0x100) >= 0)
        {
            func_80095DD0(p, b, c, d);
        }
        else
        {
            *(s32 *)(p + 0x8C) = (s32)func_80096384;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B14);

/* The three-way view stepper off the actor's +0x9B: states 0/8/10 run the
   +0x96 countdown down and then set the mover's fall rate from the state,
   states 1/9/11 glide it onto the tile over the engine's shared countdown at
   D_80083460 + 4, and state 12 waits for the renderable's 0x6000 bits. The
   jump table is `jtbl_80088B18` at rodata offset 0x3B8, 0 mod 8, so
   cc1psx's `.align 3` is a no-op and the table costs nothing.

   THE DIVISOR IS READ `*((s16 *)g + 2)`, NOT `*(s16 *)(g + 0x4)`, and that
   one word is the whole residue. `expand_expr` marks an INDIRECT_REF's MEM
   `MEM_IN_STRUCT_P` only when the address expression's TOP node is a
   PLUS_EXPR, so the pointer-add spelling is in-struct and the cast-over-the-
   addition spelling is not (docs/matching-levers.md :16982). `g` is a
   CONSTANT address, so cse's `cse_rtx_addr_varies_p` is 0 for it and
   `invalidate_memory`'s only remaining clause is `nonscalar && in_struct` --
   which means the `b->unk_0C` store between the two divisions kills the
   guard's `lh` for the in-struct spelling and leaves it alive for the cast.
   Cast spelling: 145 words against 146, the target's `lh $v1, 0x4($s3)`
   missing, every other word exact. Measured and identical to the cast (same
   md5 of the emitted word column, so these changed no code at all): the
   second divisor via `(u8 *)&D_80083460 + 4`, via `((s16 *)g)[2]`, with and
   without a guard local. `*(volatile s16 *)(g + 0x4)` is 149 -- it costs an
   `lhu`/`sll`/`sra` triple instead of one `lh`. Declaring D_80083464 and
   dividing by it is 147.

   CASE A'S D_80083460 BASE IS ITS OWN LOCAL. Sharing `g` with case B gives
   it the saved register case B needs: 146 words, 53 differing, every one a
   rename, because the shared pseudo outranks the three parameters in
   local-alloc and takes $s0.

   EACH AXIS' `- 0x20` NEEDS ITS OWN LOCAL ASSIGNED INSIDE THE SUBTRACTION,
   the rule func_800AB1C0 records: written plainly, fold's `associate:` moves
   the constant onto the shifted tile (`addiu $v0, 0x20`) where the target
   has `addiu $v1, -0x20` on the loaded position. 146 words, 0 differing. */
void func_800971A8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    s16 t;
    s32 x;
    s32 y;

    switch (p[0x9B])
    {
    case 0:
    case 8:
    case 10:
        t = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = t;
        if (t <= 0)
        {
            if (p[0x9B] == 0)
            {
                b->unk_14 = -0x100000;
            }
            else if (p[0x9B] == 8)
            {
                b->unk_14 = -0x80000;
            }
            else
            {
                b->unk_14 = -0x140000;
            }
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            h = (u8 *)&D_80083460;
            *(u16 *)(h + 0x2) |= 0x812;
            *(u16 *)(p + 0x98) &= 0xFFF3;
            p[0x9B]++;
            func_800A56E0(0x50A);
        }
        break;
    case 1:
    case 9:
    case 11:
        g = (u8 *)&D_80083460;
        if (*((s16 *)g + 2) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *((s16 *)g + 2);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *((s16 *)g + 2);
        }
        t = *(u16 *)(g + 0x4) - 1;
        *(s16 *)(g + 0x4) = t;
        if (t <= 0)
        {
            *(s16 *)(g + 0x4) = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            if (p[0x9B] >= 0xA)
            {
                *(s16 *)(g + 0x4) = 1;
                p[0x9B]++;
            }
            else if (*(s16 *)(p + 0x100) >= 0)
            {
                func_80095DD0(p, b, c, d);
            }
            else
            {
                *(s32 *)(p + 0x8C) = (s32)func_80096384;
            }
        }
        break;
    case 12:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            *(s16 *)((u8 *)&D_80083460 + 0x4) = 0;
            if (*(s16 *)(p + 0x100) >= 0)
            {
                func_80095DD0(p, b, c, d);
            }
            else
            {
                *(s32 *)(p + 0x8C) = (s32)func_80096384;
            }
        }
        break;
    }
}

/* The return is `s16` on the CALLER's evidence, not the callee's:
   func_80096384's second call site does `sll $v0, 16` / `bnez` on the result,
   and the body -- which only ever returns 0 -- measures the same either way.
   59 words, 0 differing. */
s16 func_800973F0(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    *(u16 *)(p + 0x98) |= 0x2000;
    func_8009C93C(d, c, *(s16 *)(d + 0x2A), 1, 0);
    if ((*(u32 *)(d + 0x1C) & 0x400) != 0)
    {
        *(u16 *)(d + 0x2A) += func_800A6D30() & 0xE00;
    }
    p[0x9A] = 0x37;
    g = (u8 *)&D_80083460;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u8 **)(g + 0xC) = d;
    *(s16 *)(p + 0xA6) = 0;
    func_80099F70(*(u32 *)(d + 0x5C));
    func_8009F644(d, 0x18, 0, 0);
    if (func_800A5C70() != 0)
    {
        *(u16 *)(g + 0x2) |= 0x80;
    }
    return 0;
}

/* State machine over +0x9B for the cast that parks itself in front of the
   player: state 0 drops the D_800DD294 skin and a 3-frame +0x96 countdown,
   states 1 and 2 run the countdown out and re-skin from D_800DD274, state 3
   divides the remaining distance by the countdown into the mover's per-frame
   deltas, and state 4 closes on func_800A2B04/func_80099F04 and parks
   func_80096384.  jtbl_80088B50 is at rodata offset 0x3F0, which is 0 mod 8,
   so cc1psx's `.align 3` is a no-op -- this is NOT one of the seven tables
   docs/dungeon-sweep.md lists at 4 mod 8.  239 words, 0 differing.

   CASE 1 FALLS THROUGH INTO CASE 2.  Its two exits -- the +0x96 countdown
   still positive, and the 0xE000 gate closed -- both branch to case 2's own
   label, which is what makes the fall-through the only reading.

   TWO SPELLING FACTS, and each was worth the whole match.

   (1) THE FIRST NUMERATOR IN CASE 3 IS THREE STATEMENTS AND THE SECOND IS
   TWO.  `(c[0x24] << 6) - (*(s16 *)((u8 *)b + 0x2) - 0x20)` is reassociated
   by fold() into `((c[0x24] << 6) + 0x20) - b2`, emitting `addiu $v0, $v0,
   32` where the target has `addiu $v1, $v1, -0x20`, and the function comes
   out 240 words -- one long -- with 65 differing.  func_800D991C
   (csrc/ovl_dungeon.c, "191 words, 0 differing") records the cure and it
   transfers verbatim: `u = c[0x24] << 6; w = b2 - 0x20; u -= w;` for the
   first, and the plain two-statement form for the second, where the `lh`
   has the `mflo` bubble to fall into.

   (2) ONE POINTER LOCAL PER ARM FOR D_800DD274.  Case 1 and case 2 each
   store the table at the renderable's +0x2C and index it; with a single
   local shared between them it is a global-alloc quantity, the `high` gets
   its own register and the `addiu` another, and it is 239/239 with 4
   differing -- all four of them that `lui`/`addiu` pair.  One local per arm
   makes each a local-alloc quantity and the pair shares $a1, which is what
   the target has.  Same rule as func_80097DB8's note on D_80083460. */
void func_800974DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;
    void **t2;
    void **t3;
    u8 *q;
    u8 *g;
    s32 v;
    s32 k;
    s16 n;
    s32 u;
    s32 w;

    switch (p[0x9B])
    {
    case 0:
        func_800C77D0(d - 0x20, b, 8, 0x300);
        t = D_800DD294;
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        func_8003DB94(c, *(void **)(t + v * 4), 0);
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_0C = D_8006CCD8[k] << 17;
        b->unk_10 = D_8006CCE8[k] << 17;
        b->unk_14 = -0x144000;
        *(s16 *)(p + 0x96) = 3;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
        break;
    case 1:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        if (*(s16 *)(p + 0x96) == 0)
        {
            if (p[0x102] == 0)
            {
                *(s16 *)(d + 0x20) = 1;
                if (func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1) == 0)
                {
                    q = *(u8 **)(d + 0x60);
                    if (q != NULL)
                    {
                        *(s32 *)(d + 0x60) = 0;
                        *(u32 *)(q + 0x1C) &= 0xEFFFFFFF;
                        *(s32 *)(*(u8 **)(q - 0x14) + 0xC) = 0x808080;
                    }
                }
            }
        }
        if (*(s16 *)(p + 0x96) <= 0 && (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            t2 = D_800DD274;
            *(void ***)(c + 0x2C) = t2;
            func_8003DB94(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
            p[0x9B]++;
        }
        /* fallthrough */
    case 2:
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            if (*(s16 *)(p + 0x96) <= 0)
            {
                t3 = D_800DD274;
                *(void ***)(c + 0x2C) = t3;
                func_8003DB94(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
                p[0x9B] = 3;
            }
        }
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            b->unk_14 = 0;
            *(s16 *)(p + 0x96) = 2;
            u = c[0x24] << 6;
            w = *(s16 *)((u8 *)b + 0x2) - 0x20;
            u -= w;
            b->unk_0C = (u << 16) / *(s16 *)(p + 0x96);
            w = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
            p[0x9B]++;
        }
        break;
    case 4:
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n <= 0 || (*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            func_80099F04(*(u8 **)(d + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x412;
            *(s32 *)(p + 0x8C) = (s32)func_80096384;
            *(s32 *)(g + 0xC) = 0;
            *(s16 *)(p + 0x96) = 0;
        }
        break;
    }
}

/* func_8008CCB0's one-expression index again, on a word table: the compiler
   folds `(((x + 0x100) >> 9) & 7) * 4` into `sra 7` / `andi 0x1C`, which is
   what the listing shows. Split into two statements it is 10 differing and
   the `lui` for D_800DD294 steals the branch delay slot from the mask
   constant. 39 words, 0 differing. */
void func_80097898(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;

    p[0x9A] = 0x39;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (!(func_800A6D30() & 3))
    {
        *(s32 *)(d + 0x1C) &= ~0x620;
    }
    t = D_800DD294;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_8003DB94(c, *(void **)(t + v * 4), 0);
}

/* State step: the D_800DD274 descriptor for the current eight-way facing,
   then either a reset deferring to func_8008CBD4 or the func_80096384
   continuation. 77 words, 0 differing. */
void func_80097934(u8 *p, u8 *b, u8 *c, u8 *d)
{
    void **t;
    u8 *g;

    if (p[0x9B] == 0)
    {
        func_800AD4D0(d);
        p[0x9B]++;
    }
    if ((*(u16 *)(c + 0x14) & 0xE000) != 0)
    {
        t = D_800DD274;
        *(void ***)(c + 0x2C) = t;
        func_8003DB94(c, t[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        if (d[0x28] == 0)
        {
            d[0x28] = 0;
            *(s32 *)(b + 0x14) = 0;
            *(s32 *)(b + 0x10) = 0;
            *(s32 *)(b + 0xC) = 0;
            func_80042B68((s8 *)d, 0xA);
            func_8008CBD4(p, b, c, d);
        }
        else
        {
            *(s16 *)(p + 0x96) = 8;
            *(s32 *)(p + 0x8C) = (s32)func_80096384;
        }
    }
}

/* State 0x38: park the D_800DD294 animation for the facing and start the
   0x50A sound. func_80097898's one-expression index verbatim. 36 words,
   0 differing. */
void func_80097A68(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *t;
    s32 v;

    p[0x9A] = 0x38;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    t = D_800DD294;
    *(u8 **)(c + 0x2C) = t;
    v = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
    func_8003DB94(c, *(void **)(t + v * 4), 0);
    func_800A56E0(0x50A);
    *(s16 *)(p + 0x96) = 2;
    *(u16 *)(p + 0xA2) &= 0xFFEF;
}

/* State 0..2 of a +0x8C handler: wait out the +0x96 countdown, give the mover
   a -0x160000 lean, then gate on the renderable's 0x6000 bits and the engine's
   +0xA counter before clearing +0x200000 and parking func_80096384.
   The +0x96 counter is the unit's established `t = *(u16 *)(p + 0x96) +/- 1;
   *(s16 *)(p + 0x96) = t;` pair with an `s16` local, so the `>= 4` test uses
   the computed value rather than a reload. Cases 0 and 1 share the
   `p[0x9B]++` tail by cross-jumping. 86 words, 0 differing on the first
   compile. */
void func_80097AF8(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s16 t;
    s32 f;

    switch (p[0x9B])
    {
    case 0:
        t = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = t;
        if (t <= 0)
        {
            b->unk_14 = -0x160000;
            *(s16 *)(p + 0x96) = 0;
            p[0x9B]++;
        }
        break;
    case 1:
        t = *(u16 *)(p + 0x96) + 1;
        *(s16 *)(p + 0x96) = t;
        if ((*(u16 *)(p + 0xA2) & 0x10) != 0 && t >= 4)
        {
            b->unk_14 = 0;
            p[0x9B]++;
        }
        break;
    case 2:
        if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            g = (u8 *)&D_80083460;
            if (*(s16 *)(g + 0xA) == 0)
            {
                f = *(s32 *)(d + 0x1C);
                if ((f & 0x200000) != 0)
                {
                    *(s32 *)(d + 0x1C) = f & ~0x200000;
                    *(u16 *)(g + 0x2) |= 0x412;
                    func_80099F70(*(u8 **)(d + 0x5C));
                    func_80099F04(*(u8 **)(d + 0x5C));
                }
                *(s32 *)(p + 0x8C) = (s32)func_80096384;
            }
        }
        break;
    }
}

/* State 0x3A, the step counter cleared, and bit 5 dropped from the fourth
   argument's +0x1C flags. 10 words, 0 differing. */
void func_80097C50(UnkDungeonActor *p, void *b, void *c, u8 *d)
{
    p->unk_9A = 0x3A;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    p->unk_96 = 0;
    *(s32 *)(d + 0x1C) &= ~0x20;
}

/* The +0x64 counter picks the frame. 0x80013714 is a CONSTANT ADDRESS, as
   func_800969B8 records. 80 words, 0 differing. */
void func_80097C78(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (*(s16 *)(d + 0x64) < 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_80097898(a, b, c, d);
        return;
    }
    if (*(s16 *)(d + 0x64) > 0)
    {
        func_8008CBA0(a, b, c, d);
    }
    if ((*(u16 *)0x80013714 & 1) == 0 && (*(s32 *)(g + 0x8) & 0x80) != 0)
    {
        *(s16 *)(d + 0x8A) = 2;
        D_800E4940 = 2;
        func_8008CF6C((UnkDungeonActor *)a, b, c, D_8004F5F4);
        D_80082EB0 = NULL;
        *(s32 *)(a + 0xC8) = 0;
        *(s32 *)(a + 0x104) = 0;
        return;
    }
    if (a[0x9A] != 0x3A && (*(s32 *)(d + 0x1C) & 0x200) != 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_80097C50((UnkDungeonActor *)a, b, c, d);
    }
}

/* State machine over +0x9B: install the +0x8C continuation, then the
   gated flag/counter work, then the teardown. 115 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, and each is worth the whole match.
   (1) ONE POINTER LOCAL PER BLOCK for D_80083460. With a single `k` shared
   between the two arms of the func_80042900 test it is a global-alloc
   quantity and gets ONE hard register in both; the target has the `lui` and
   the `addiu` sharing $v0 in the first arm and $v1 in the second, which only
   happens when each arm's pointer is its own local-alloc quantity.
   Shared: 115/115, 6 differing, all of them that lui/addiu pair.
   (2) `*(s32 *)((u8 *)a + 0x8C) = 0;` rather than `a->unk_8C = NULL;`.
   The struct store is MEM_IN_STRUCT_P and sched1 sinks it past the counter
   read-modify-write: with the split above but the field spelling it is
   115/114, 59 differing, the `sw $zero` landing in the `lbu 0x9B`'s load
   delay. Writing it through the cast restores the dependence and pins it
   between the `addiu` and the `lhu`, which is the target's order. The same
   cast is needed on case 2's install (field spelling: 115/113, 33).
   Statement order inside the arm is NOT the lever -- all four permutations
   of the three statements measure 114 or 112 with the field spelling.
   (3) func_800A5C70 takes NO argument here. Passing `d` fills the call's
   delay slot with a redundant `addu $a0, $s0, $zero` where the target has a
   `nop`: 115/115, 7 differing. With no argument the only `$a0` setup is
   func_80042900's, which reorg copies into BOTH preceding branch delay
   slots, which is what words 45 and 49 are. */
void func_80097DB8(UnkDungeonActor *a, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    u8 *k2;
    u8 *m;

    g = (u8 *)&D_80083160;
    switch (a->unk_9B)
    {
    case 0:
        a->unk_8C = func_80097C78;
        a->unk_9B++;
        break;
    case 1:
        h = (u8 *)&D_80083460;
        if ((*(u16 *)(h + 0x2) & 4) != 0)
        {
            break;
        }
        if ((*(u16 *)((u8 *)a + 0xA2) & 0x10) == 0)
        {
            break;
        }
        if ((*(s32 *)(g + 0x8) & 0x20) != 0 && func_800A5C70() != 0)
        {
            *(u16 *)(h + 0x2) |= 0x80;
        }
        if ((s16)func_80042900((s8 *)d, 1) == 0)
        {
            *(s32 *)((u8 *)a + 0x8C) = 0;
            k2 = (u8 *)&D_80083460;
            *(u16 *)(k2 + 0xA) += 1;
            a->unk_9B++;
        }
        else
        {
            func_80099F70(*(u8 **)(d + 0x5C));
            func_80099F04(*(u8 **)(d + 0x5C));
            k = (u8 *)&D_80083460;
            *(u16 *)(k + 0x2) |= 0x812;
        }
        break;
    case 2:
        *(s32 *)(d + 0x1C) &= ~0x200;
        func_80099F70(*(u8 **)(d + 0x5C));
        func_80099F04(*(u8 **)(d + 0x5C));
        m = (u8 *)&D_80083460;
        *(u16 *)(m + 0x2) |= 0x812;
        *(s32 *)((u8 *)a + 0x8C) = (s32)func_80096384;
        *(u16 *)(m + 0xA) -= 1;
        break;
    }
    a->unk_96 += 1;
}

/* Release a message handle and hand back a fresh one: free slot `a` in the
   0x80010248 table and free slot `n` in the 0x8001029C one, with a sentence
   built on the failure path. 157 words, 0 differing.

   THREE THINGS ARE LOAD-BEARING.
   (1) `a` and `n` are `s32` holding the IMPLICITLY-declared callees' `int`
   results, with an `(s16)` cast at every use. As `s16` locals the HImode
   pseudo costs a copy of the first result into its own register
   (`addu $s6, $s0, $zero`) and pushes `c` down to $s5 where the target has
   $s7: 156 words, 37 differing.
   (2) The 0x1C read is an ARRAY REFERENCE, `((s32 *)D_800814A8)[7]`.
   Spelled `*(s32 *)(D_800814A8 + 0x1C)` the load is not MEM_IN_STRUCT_P, so
   sched records a dependence against the fixed-address store to D_80081484
   above it and the load cannot be hoisted over it: 3 differing, the same
   mechanism func_80098B38's comment records for func_800422A8's first
   argument.
   (3) The two 0x8001xxxx bases take DIFFERENT spellings, and both are
   measured elsewhere in this unit: the 0x80010248 table is a plain constant
   address (`lui`/`ori` and a zero displacement, func_8009FADC's shape) and
   the 0x8001029C one is a page-base LOCAL with the offset inside the index
   (one bare `lui` and a `%lo` displacement, func_80098FF8's shape). */
void *func_80097F84(u8 *p, u8 *q, u8 *c, s16 f)
{
    u8 *b;
    u8 *m;
    u8 *w;
    s32 a;
    s32 n;
    s16 r;

    if (p == D_80081484 || p == D_80081470 || (u32)p == *(u32 *)(D_800814A8 + 0xF0))
    {
        a = func_80098FB0();
        n = func_80098FF8();
        if ((s16)a < 0 || (s16)n < 0)
        {
            m = func_800990FC();
            func_80099290(func_80099194(c, func_8009955C(p, func_80099194(q, func_8009929C(8, m)))));
            func_800A5720(m);
            return NULL;
        }
        m = func_800990FC();
        func_80099290(func_80099194(D_800E07EF, func_80099368(p, m)));
        if (f != 0)
        {
            func_800A5720(m);
        }
        func_800A56E0(0x508);
        if (p == D_80081484)
        {
            *(void **)((u8 *)0x80010248 + (s16)a * 4) = *(void **)D_80081484;
            *(s32 *)D_80081484 = 0;
            *(s32 *)(D_800814A8 + 0x124) = 0;
            ((s32 *)D_800814A8)[7] &= ~0x100000;
        }
        else
        {
            r = (s16)func_800422A8(((u8 **)D_800E3D7C)[0x3C], D_800E3548, 4, 0x40);
            if (r >= 0)
            {
                func_8009A3D0(D_800E36C8[r].unk_00, D_800E36C8[r].unk_01, 0x800);
            }
            *(void **)((u8 *)0x80010248 + (s16)a * 4) = *(void **)D_80081470;
            **(s32 **)(D_800814A8 + 0xF0) = 0;
        }
        w = (u8 *)0x80010248 + (s16)a * 4;
        *(s32 *)p = 0;
        /* DEVICE: a zero-instruction reference on `w`, worth the last word.
           The target computes the new handle into a scratch register BEFORE
           `*(s32 *)p = 0` and copies it into p's register after
           (`addu $v0, $v0, $v1` / `sw $zero, 0($s1)` / `addu $s1, $v0,
           $zero`). Written `p = <expr>` directly, the set of p's own pseudo
           carries an anti-dependence on the store's address register, so
           sched1 cannot lift it and the addu lands after the store as ONE
           word: 156 of 157, 22 differing. Binding the expression to `w`
           first does not help on its own -- combine folds the copy away --
           so the marker is what keeps `w` a separate quantity. It emits
           nothing. Legal spellings measured and all on the same 156/22
           plateau (identical emitted words, md5 d25a3017): the plain
           assignment; `b` hoisted above the store; a `u32` temp; a
           `do { } while (0)` around the store; `&((s32 *)0x80010248)[a]`;
           `*(...) = p = <expr>` chained; `w` read by the 0x29C store
           instead of p; the phantom zero `w + ((w + 1) - w - 1)`; and the
           early-`return p` inversion of the entry test (156/22, md5
           b2403062). Duplicating `*(s32 *)p = 0` into both arms reaches 157
           words but 44 differing. */
        __asm__("" ::"r"(w));
        p = w;
        b = (u8 *)0x80010000;
        *(void **)(b + (s16)n * 4 + 0x29C) = p;
    }
    return p;
}

/* Clamp a stat to 0..0x63. The running value is an s16, which is what makes
   every use re-sign-extend -- the five `sll`/`sra` pairs are the type, not
   four separate casts -- and the optional term is a SIGNED byte off a
   record the actor may not have. */
s16 func_800981F8(u8 *p)
{
    s16 v;
    u8 *q;

    q = *(u8 **)(p + 0x4C);
    v = p[0x85];
    if (q != NULL)
    {
        v += (s8)q[2];
    }
    if (v < 0)
    {
        v = 0;
    }
    if (v >= 0x64)
    {
        v = 0x63;
    }
    return v;
}

s16 func_80098250(u8 *p)
{
    s16 v;
    u8 *q;

    q = *(u8 **)(p + 0x50);
    v = p[0x86];
    if (q != NULL)
    {
        v += (s8)q[2];
    }
    if (v < 0)
    {
        v = 0;
    }
    if (v >= 0x64)
    {
        v = 0x63;
    }
    return v;
}

/* Swap the record's +0x4C occupant for `p`, the +0x50 swap func_80098614
   does one field over: drop the old entry (reporting it, or bailing out
   through the 0x70A chime if it still holds its 0x40 bit), announce the new
   one, park the matching record at D_800E3D18, and finish by copying the
   per-kind +0x84/+0x85 byte pair out of whichever of the two table pairs the
   entry's kind picks.

   BOTH LOCALS ARE `s16`, for the reason func_80098614's comment records: a
   narrow local's read is a mode conversion whose extension combine drops,
   leaving a plain register copy. `s32 i` is 218 words / 31 differing.

   func_80048590 AND func_80048568 TAKE THE INDEX. Their `jal`s have a `nop`
   delay slot and no argument setup of their own, which reads like a
   zero-arity call; the `addu $a0, $s2, $zero` six words earlier, in the
   `bne $v1, 0x10` delay slot, is the argument, and cse then reuses $a0 for
   both table reads and both `beqz`. Spelled `func_80048590()` the index copy
   lands in $v1 instead and the function is 7 differing; passing `q` is 9.
   `i = 0` must also be written AFTER the pair of D_800DD2C4/D_800DD2B4
   stores in the `p == NULL` arm -- before them it is 10.
   219 words, 0 differing. */
void func_800982A8(u8 *q, u8 *p)
{
    u8 *r;
    s16 f;
    s16 i;
    void *h;

    f = 0;
    if (p != NULL)
    {
        if (p[1] == 0xF && p[0] >= 0xD)
        {
            func_800A56E0(0x506);
            h = func_800990FC();
            func_80099290(func_80099194(D_80088B64, func_80099368(p, func_80099194(D_800E080A, func_80099734(q, func_8009929C(8, h))))));
            func_800A5720(h);
            return;
        }
        p = (u8 *)func_80097F84(p, D_800E07C0, D_800E07D3, 0);
        if (p == NULL)
        {
            return;
        }
    }
    r = *(u8 **)(q + 0x4C);
    if (r != NULL)
    {
        if ((r[3] & 0x40) != 0)
        {
            func_800A56E0(0x70A);
            h = func_800990FC();
            func_80099290(func_80099194(D_800E081C, func_80099368(r, func_8009929C(8, h))));
            func_800A5720(h);
            return;
        }
        r[3] &= 0xDF;
        D_800E3D18 = D_800E3CF8;
        if (r == p)
        {
            p = NULL;
        }
        f = 1;
    }
    if (p != NULL)
    {
        i = p[0];
        p[3] &= 0x7F;
        h = func_800990FC();
        func_80099290(func_80099194(D_800E0844, func_80099368(p, func_8009929C(8, h))));
        func_800A5720(h);
        D_800E3D18 = D_800E3D80;
        if ((p[3] & 0x40) != 0)
        {
            *(u32 *)(q + 0x1C) |= 0x800;
            func_800A56E0(0x70A);
            h = func_800990FC();
            func_80099290(func_80099194(D_800E0853, func_80099368(p, h)));
            func_800A5720(h);
        }
        p[3] |= 0x20;
    }
    else
    {
        if (f != 0)
        {
            h = func_800990FC();
            func_80099290(func_80099194(D_800E0862, func_80099368(*(u8 **)(q + 0x4C), func_8009929C(8, h))));
            func_800A5720(h);
        }
        q[0x84] = D_800DD2C4[0];
        q[0x85] = D_800DD2B4[0];
        i = 0;
    }
    *(u8 **)(q + 0x4C) = p;
    func_800A56E0(0x508);
    if (p[1] == 0x10)
    {
        q[0x84] = D_800DD2E0[i];
        q[0x85] = D_800DD2D4[i];
        if (i != 0)
        {
            func_80048590(i);
        }
    }
    else
    {
        q[0x84] = D_800DD2C4[i];
        q[0x85] = D_800DD2B4[i];
        if (i != 0)
        {
            func_80048568(i);
        }
    }
    if (i == 0)
    {
        func_800483AC(1);
    }
}

/* Swap the record's +0x50 occupant for `p`: drop the old one (reporting it,
   or bailing out through the 0x70A chime if it still holds its 0x40 bit),
   announce the new one, and finally copy D_800DD2EC's per-kind byte into
   +0x86 and poke the engine.

   BOTH LOCALS ARE `s16`, AND THAT IS THE WHOLE LANDING. As `s32` the flag's
   `if (f != 0)` is a bare `beqz $s2` and the index's `D_800DD2EC[i]` uses
   $s1 directly; the target copies each into another register first
   (`addu $v0, $s2, $zero` in the `beqz $s0` delay slot, `addu $a0, $s1,
   $zero` ahead of the table read). A narrow local's read is a mode
   conversion whose extension combine drops -- the value is provably in range
   -- leaving a plain register copy cse cannot propagate away; the same lever
   func_800A982C's comment records. Measured: both `s32` 147 words /
   25 differing, `s16 f` alone 147/22, `u8 f` alone 147/23, `u8 i` alone
   148/4, `s16 f` + `u8 i` 148/1, `u8 f` + `u8 i` 148/2, both `s16` 148/0.
   The flag's width is also what stops cse handing `f`'s zero to
   func_80097F84's fourth argument (`move $a3, $s2` against the target's
   `addu $a3, $zero, $zero`).

   The forward declaration added at the top of the file is required: the
   landed func_80095538 (:3251) calls this on an implicit declaration today,
   and that function still measures 96 words, 0 differing with it in place.
   148 words, 0 differing. */
void func_80098614(u8 *q, u8 *p)
{
    u8 *r;
    s16 f;
    s16 i;
    void *h;

    f = 0;
    if (p != NULL)
    {
        p = (u8 *)func_80097F84(p, D_800E07C0, D_800E07D3, 0);
        if (p == NULL)
        {
            return;
        }
    }
    r = *(u8 **)(q + 0x50);
    if (r != NULL)
    {
        if ((r[3] & 0x40) != 0)
        {
            func_800A56E0(0x70A);
            h = func_800990FC();
            func_80099290(func_80099194(D_800E0874, func_80099368(r, func_8009929C(8, h))));
            func_800A5720(h);
            return;
        }
        r[3] &= 0xDF;
        if (r == p)
        {
            p = NULL;
        }
        f = 1;
    }
    if (p != NULL)
    {
        i = p[0];
        p[3] &= 0x7F;
        h = func_800990FC();
        func_80099290(func_80099194(D_800E089C, func_80099368(p, func_8009929C(8, h))));
        func_800A5720(h);
        if ((p[3] & 0x40) != 0)
        {
            *(u32 *)(q + 0x1C) |= 0x800;
            func_800A56E0(0x70A);
            h = func_800990FC();
            func_80099290(func_80099194(D_800E08AB, func_80099368(p, h)));
            func_800A5720(h);
        }
        p[3] |= 0x20;
    }
    else
    {
        i = 0;
        if (f != 0)
        {
            h = func_800990FC();
            func_80099290(func_80099194(D_800E08BA, func_80099368(*(u8 **)(q + 0x50), func_8009929C(8, h))));
            func_800A5720(h);
        }
    }
    *(u8 **)(q + 0x50) = p;
    func_800A56E0(0x508);
    q[0x86] = D_800DD2EC[i];
    if (i != 0)
    {
        func_800485B8(i);
    }
    else
    {
        func_800483AC(0);
    }
}

/* The shared close of the kill handlers. THE SECOND PARAMETER IS THE ACTOR
   AND IS DECLARED `s32`: ten landed call sites pass their own `s32 r`
   straight through, so widening it to `u8 *` would warn at every one of
   them. 47 words, 0 differing. */
s32 func_80098864(u8 *q, s32 r)
{
    u8 *g;
    u8 *e;

    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    e = *(u8 **)(r - 0x14);
    func_800990C8((u8 *)r, q);
    func_8009C93C((u8 *)r, e, *(s16 *)(r + 0x2A), 0, 0);
    if (func_8009C12C((u8 *)r, e, *(s16 *)(r + 0x2A), 0) == 0)
    {
        return 0;
    }
    if (*(u8 **)(r + 0x60) != NULL)
    {
        *(u8 **)(*(u8 **)(r + 0x60) + 0x60) = D_800E3D7C;
    }
    func_80098B38(q);
    return 1;
}

/* The message dispatcher: look the handler word up in
   D_800DD6B8[q[1] - 1][q[0] - 1], play the sound its top byte names and jump
   to the address its low 24 bits hold. Two descriptor kinds are special --
   0x12 goes to func_800B7B60 unless the caller already passed 0x15, and 0x15
   runs the D_800E3648 slot lookup instead. `c == 0x15` also tears the
   dialogue window down (func_80099F70/func_80099F04 on the engine's +0x5C)
   before the handler runs. func_80091A38 at :2665 is the same lookup out of
   the same table and its note on the two index locals holds here too.

   THREE MEASURED FACTS.

   (1) func_800B7B60 IS CALLED WITH THREE ARGUMENTS HERE, and that is the last
   three words. Its landed definition below declares TWO (`UnkNode
   *func_800B7B60(u8 *p, u32 *q)`), but the `jal` has a `nop` delay slot and no
   argument setup at all -- $a0, $a1 and $a2 are already live -- so the arity
   is invisible at the call. Passing only `p`, only `(p, q)` or nothing is 134
   words and 3 differing: the `(s16)c` the third argument needs is what puts
   the sign extension in $a2 and the 0x15 constant in $v0 rather than the other
   way round, and it also moves the `beq` one word. This call rides an implicit
   declaration, so nothing diagnoses the mismatch -- see the report.

   (2) THE TABLE LOOKUP MUST NOT BE AN ARRAY REF. `D_800DD6B8[i][j]` is
   MEM_IN_STRUCT_P, so its two loads hoist over the `sh` of the D_80083460
   counter and the whole block reschedules: 134 words, 25 differing. Through
   the cast chain the load is not in a struct, the counter's
   read-modify-write completes first, and the two `lui`s come out in the
   target's order.

   (3) D_80083460 IS MATERIALISED TWICE, once per `if`, and `i`/`j` ARE
   COMPUTED BEFORE THE `q[1] == 0x15` TEST. One `g` local shared by the +0xA
   increment and the +0x2 or is 135 words; computing `i`/`j` inside the tail
   instead leaves the two `addiu -1` out of the entry block. The three reads of
   `q[1]` are real source: one local for all of them is 133 words, one short.

   134 words, 0 differing. */
s32 func_80098920(u8 *p, u8 *q, s16 c, s32 d)
{
    u8 *g;
    u8 *g2;
    u8 *e;
    u32 w;
    s32 n;
    s32 i;
    s32 j;
    s32 (*fn)(u8 *, u8 *, s16, s32);
    s16 r;
    s16 t;

    if (q[1] == 0x12)
    {
        if (c != 0x15)
        {
            return func_800B7B60(p, q, c);
        }
    }
    i = q[1] - 1;
    j = q[0] - 1;
    if (q[1] == 0x15)
    {
        e = *(u8 **)(p - 0x14);
        r = func_800B500C(e[0x24], e[0x25], *(s16 *)(p + 0x88));
        if (r < 0)
        {
            return -1;
        }
        return func_800B627C(*(u8 *)&D_800E3648_s[r], p, r, 1);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    w = *(*(u32 **)((u8 *)D_800DD6B8 + i * 4) + j);
    n = w >> 24;
    fn = (s32 (*)(u8 *, u8 *, s16, s32))((w & 0xFFFFFF) | 0x80000000);
    if (n != 0)
    {
        func_800BCFBC(n - 1);
    }
    if (c != 0x15)
    {
        if (D_80082EB4 == NULL)
        {
            t = func_80098C80(q);
            if (t != 0x15)
            {
                if (t == 0x16)
                {
                    **(u32 **)(D_800E3D7C + 0xF0) = 0;
                }
                else
                {
                    func_80094E34();
                }
            }
        }
    }
    if (c == 0x15)
    {
        func_80099F70(*(u8 **)(D_800E3D7C + 0x5C));
        func_80099F04(*(u8 **)(D_800E3D7C + 0x5C));
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0x2) |= 0x812;
    }
    return fn(p, q, c, d);
}

/* Releasing a message handle.

   THE FIRST ARGUMENT OF func_800422A8 IS AN ARRAY REFERENCE, and that is the
   last three words. Spelled `*(u8 **)(D_800E3D7C + 0xF0)` the load is a plain
   cast -- not MEM_IN_STRUCT_P, so sched records a dependence against the
   non-struct store to D_80081470 above it and the store can never sink past
   the load: it lands three insns before the `jal` and reorg fills the delay
   slot with `li $a3, 0x40` (68 words, 3 differing). As
   `((u8 **)D_800E3D7C)[0x3C]` the load is in-struct with a varying address
   against a fixed-address non-struct store, the dependence is dropped and the
   store sinks into the slot. Binding the load to a local ahead of the store
   reaches 2 -- the right store placement, but D_800E3D7C's `lui` then beats
   D_800E3548's `addiu`. The func_800A86DC arm must be written FIRST: as the
   `else` of `if (r < 0)` it is a `bgez` with the arm out of line, 58
   differing. 68 words, 0 differing. */
void func_80098B38(u8 *p)
{
    s16 r;

    if (p != NULL)
    {
        *(s32 *)p = 0;
        r = (s16)func_8009904C(p);
        if (r >= 0)
        {
            func_800A86DC(r);
        }
        else if (p == D_80081484)
        {
            *(s32 *)(D_800E3D7C + 0x1C) &= ~0x100000;
            *(s32 *)(D_800E3D7C + 0x124) = 0;
        }
        else if (p == D_80081470 || p == *(u8 **)(D_800E3D7C + 0xF0))
        {
            *(s32 *)D_80081470 = 0;
            r = (s16)func_800422A8(((u8 **)D_800E3D7C)[0x3C], D_800E3548, 4, 0x40);
            if (r >= 0)
            {
                func_8009A3D0(D_800E36C8[r].unk_00, D_800E36C8[r].unk_01, 0x800);
            }
            **(s32 **)(D_800E3D7C + 0xF0) = 0;
        }
    }
}

/* Index of an address in the engine's twenty-word table, scanned downwards.

   THE COUNTER IS SET FIRST and the `while` is load-bearing: initialised in
   a `for`, the table pointer's `lui`/`ori` is emitted ahead of the `li
   0x13`; as `do { ... } while (--i >= 0)` the hit test comes out `beq` with
   its body at the end. 14 words, 0 differing. */
s16 func_80098C48(u8 *p)
{
    s32 i;
    u8 *q;

    i = 0x13;
    q = (u8 *)0x80010294;
    while (i >= 0)
    {
        if (q == p)
        {
            return i;
        }
        i--;
        q -= 4;
    }

    return -1;
}

/* func_80098C48's index plus one, or one of two fixed answers for two known
   descriptors. `s32 t = (s16)call();` and the `(s16)` written on `t + 1`:
   an `s16 t` local adds to the UN-extended value, costs a `move` and saves
   the `sra` (27 words, 3 differing). The return stays s32 because all five
   ASM callers re-narrow with their own `sll 16`/`sra 16`.
   27 words, 0 differing. */
s32 func_80098C80(u8 *p)
{
    s32 t;

    t = func_80098C48(p);
    if (t >= 0)
    {
        return (s16)(t + 1);
    }
    if (p == D_80081484)
    {
        return 0x15;
    }
    if (p == D_80081470)
    {
        return 0x16;
    }
    return 0;
}

/* 3 words, 0 differing. */
s32 func_80098CEC(u8 *p)
{
    return p[0x3] & 0x20;
}

/* The key-use step: refuse the door outright when the held key is not
   D_80081484's and that slot is armed, print the mismatch pair and bail;
   otherwise, for a consumable key, announce it, spend it through
   func_80098614 or func_800982A8, then spawn the unlock node, aim its mover
   at the renderable's tile, record the key in the actor's +0xBC and in
   D_80081484, re-skin the renderable at the camera-relative facing and arm
   state 0x1B. 174 words, 0 differing -- a decomp-permuter win on top of a
   16-differing hand floor.

   THE WHOLE RESIDUE WAS ONE LOAD'S POSITION. `*(u32 *)d` must be read into a
   local BETWEEN the `+0xBC` store and the `+0x9C` store; at that point the
   target's copy of d's word has an extra consumer, so sched1 lifts it 18
   insns earlier into the load-delay slot after `lhu $v0, 0x88($s4)`, and the
   `addu $a0, $s5, $zero` that fills word 114 appears. Written inline at the
   D_80081484 store the copy has only the store and the `jal` to feed, stays
   next to the call, and word 114 is a `nop` instead -- 16 differing, with
   knock-on register renames through words 118-134 and the two constant
   argument setups at 143/151 swapped. A `w` local assigned anywhere ELSE is
   byte-identical to the inline form; it is the position, not the local.

   MEASURED AND REJECTED BY HAND FIRST, all 174 or 175 words: all 120 orders
   of the five r-stores (floor 16, reached by every order putting 0x98
   first); the table store moved before the r-block, after 0xAC and before
   0x90 (173 words, 50-52); `((u8 **)r)[0x24] = c`, `c` as `void *`, an index
   local, a `do { } while (0)` before the r-stores and `__asm__("" ::"r"(c))`
   before the 0x90 store, all byte-identical; `((u8 **)c)[0xB] = D_800DD0B8`
   22; x/y locals for the two byte reads 173/71; the dead-`if` reference
   device 91; p[0x9A]/p[0x9B] above the D_80081484 store 21; the +0x88,
   +0x2A and +0x1C reads moved off q onto p, 60/59/59. */
UnkNode *func_80098CF8(u8 *p, void *b, u8 *c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *g;
    u8 *r;
    void *h;
    void *s;
    u32 w;

    q = p;
    g = D_80081484;
    if (d != g && g[1] != 0)
    {
        h = func_800990FC();
        func_80099290(func_80099194(D_800E08E5,
                                    func_80099368(g, func_80099194(D_800E08CC,
                                                                   func_80099368(d, func_8009929C(8, h))))));
        func_800A5720(h);
        return NULL;
    }
    if ((d[3] & 0x20) != 0)
    {
        h = func_800990FC();
        s = func_8009929C(8, h);
        if ((d[3] & 0x40) != 0)
        {
            func_800A56E0(0x70A);
            func_80099290(func_80099194(D_800E08F1, func_80099368(d, s)));
            func_800A5720(h);
            return NULL;
        }
        func_80099290(func_80099194(D_800E0919, func_80099368(d, s)));
        func_800A5720(h);
        if (d[1] == 0x11)
        {
            func_80098614(q, d);
        }
        else
        {
            func_800982A8(q, d);
        }
    }
    n = func_800A8608(p - 0x20, d, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    n->unk_10 = (u32)func_8008D470;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = (c[0x24] << 6) + 0x20;
    *(s16 *)((u8 *)m + 0x6) = (c[0x25] << 6) + 0x20;
    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(q + 0x88) - 0x30;
    r = (u8 *)n + 0x20;
    *(u32 *)(r + 0x98) = *(u32 *)d;
    *(s16 *)(r + 0xAC) = 1;
    *(u8 **)(r + 0x90) = c;
    *(void **)(r + 0x8C) = b;
    *(s16 *)(r + 0xA8) = 6;
    *(u8 **)(p + 0xBC) = d;
    w = *(u32 *)d;
    *(u8 **)(r + 0x9C) = D_80081484;
    *(u32 *)(p + 0x8C) = 0;
    *(u32 *)(p + 0x124) = 0;
    *(u32 *)D_80081484 = w;
    p[0x9A] = 0x1B;
    p[0x9B] = 0;
    *(u8 **)(c + 0x2C) = D_800DD0B8;
    func_80048A44(c, D_800DD0B8[((D_80083228 + *(s16 *)(q + 0x2A) + 0x100) >> 9) & 7], 0, 1);
    func_800A56E0(0x511);
    *(u32 *)(q + 0x1C) |= 0x100000;
    func_80098B38(d);
    return n;
}

/* THE 0x80010000 BASE AGAIN: splat renders these as %hi/%lo(D_8001xxxx) but
   a --raw run shows no relocations. `return ret` inside the loop is 19
   words / 16 differing; `break` with one return at the end makes the
   `ret = i` arm the fall-through and its `j` carries the assignment in its
   delay slot. 18 words, 0 differing. */
s16 func_80098FB0(void)
{
    s32 i;
    s32 ret;
    u8 *b;

    b = (u8 *)0x80010000;
    ret = -1;
    for (i = 0x13; i >= 0; i--)
    {
        if (b[i * 4 + 0x249] == 0)
        {
            ret = i;
            continue;
        }
        if (ret >= 0)
        {
            break;
        }
    }
    return (s16)ret;
}

/* THE BASE IS 0x80010000 AND THE TABLE OFFSET RIDES IN THE INDEX. Every
   spelling that names 0x8001029C as the base -- as an array, as a cast, as
   a pointer plus index -- materialises the whole address with `lui`/`ori`
   and loads at displacement 0: 23 words against 21. Only a pointer local
   holding the page base, with 0x29C written inside the index expression,
   gives the target's bare `lui` and its displacement
   (docs/matching-levers.md:2314). splat prints those displacements as
   `%lo(D_8001029C)` even though there is no relocation.
   21 words, 0 differing. */
s16 func_80098FF8(void)
{
    u8 *b;
    s32 i;
    s32 n;

    b = (u8 *)0x80010000;
    n = -1;
    for (i = 0; i < 0x14; i++)
    {
        if (*(void **)(b + i * 4 + 0x29C) == NULL)
        {
            n = i;
            break;
        }
    }
    if (n >= 0)
    {
        *(void **)(b + n * 4 + 0x2A0) = NULL;
    }
    return n;
}

/* 17 words, 0 differing. */
s16 func_8009904C(void *x)
{
    s32 i;
    void **tbl;

    i = 0;
    tbl = (void **)0x80010000;
loop:
    if (tbl[0xA7] != NULL)
    {
        if (tbl[0xA7] == x)
        {
            return (s16)i;
        }
        i++;
        if (i < 0x14)
        {
            tbl++;
            goto loop;
        }
    }
    return -1;
}

/* The first free slot of the engine's twenty-record 0x8C-stride table. The
   counter is assigned before the pointer, as in func_80098C48.
   14 words, 0 differing. */
s32 func_80099090(void)
{
    s32 i;
    u8 *p;

    i = 0;
    p = (u8 *)D_800E3E48;
    do
    {
        if (p[0x13] == 0)
        {
            return i;
        }
        i++;
        p += 0x8C;
    } while (i < 0x14);

    return -1;
}

/* 13 words, 0 differing. */
void func_800990C8(u8 *p, u8 *q)
{
    p[3] = *(u8 *)(D_800814A8 + 3);
    p[0x26] = D_800DD704[q[1]];
}

/* Hand out the next of six message buffers, skipping any buffer the engine's
   five-slot queue at D_80082A38+0x48 is still showing.

   THE RETRY IS A `goto` TO THE TOP OF THE FUNCTION: the target's `beq` jumps
   to this function's own first instruction, so the modulo, the buffer address
   and the D_800DD718 store are all inside the loop.

   `k = D_800DD718 + 1; k %= 6;` -- the in-place remainder is what makes the
   dividend and the remainder ONE pseudo, which is the target's
   `addiu $a0, $a0, 1` ... `subu $a0, $a0, $v0` off one register. Written as
   the single expression `(D_800DD718 + 1) % 6` they are two pseudos and the
   function is 35 words / 13 differing, every one a register number.
   35 words, 0 differing. */
void *func_800990FC(void)
{
    s32 i;
    s32 k;
    u8 *r;

restart:
    k = D_800DD718 + 1;
    k %= 6;
    r = &D_800E4958[k * 312];
    D_800DD718 = k;
    for (i = 0xA; i < 0xF; i++)
    {
        u8 *q = (u8 *)&D_80082A38w[i];

        if (*(u8 **)(q + 0x48) == r)
        {
            goto restart;
        }
    }
    return r;
}

/* A one-word publish, twice: the overlay's own slot and an engine one. */
void func_80099188(s32 v)
{
    D_800DCF60 = v;
}

/* Copy a Shift-JIS string into the message buffer, translating through
   D_800DCF60 while the 0x51 escape is active.

   ONE EXIT. Spelled with the NUL case as `return d;` inside the loop the
   return is laid out inline and the function is 50 words against 48 with 32
   differing. THE TABLE INDEX IS WRITTEN INDEX-FIRST -- `i + D_800DCF60 - 2`;
   base-first costs 3 differing, because both operands are integers and
   integer arithmetic keeps source order. 48 words, 0 differing. */
void *func_80099194(u8 *s, u8 *d)
{
    s32 f;
    u8 c;
    s32 i;

    f = 0;
    for (;;)
    {
        if (f == 0 && *s == 0x51)
        {
            f = 1;
            s++;
            continue;
        }
        c = *s;
        if (c == 0)
        {
            if (f == 0)
            {
                break;
            }
            f = 0;
            s++;
            continue;
        }
        if (f != 0)
        {
            i = *s * 2;
            *d++ = *(u8 *)(i + D_800DCF60 - 2);
            s++;
            *d++ = *(u8 *)(i + D_800DCF60 - 1);
        }
        else
        {
            if (c >= 0x80)
            {
                *d++ = *s++;
            }
            *d++ = *s++;
        }
    }

    return d;
}

/* 15 words, 0 differing. */
u8 *func_80099254(u8 *s, u8 *d)
{
    *d++ = *s++;
    *d++ = *s++;
    *d++ = *s++;
    *d++ = *s++;

    return d;
}

/* The string builder's terminator: write the NUL and hand back the advanced
   cursor. 3 words, 0 differing. */
u8 *func_80099290(u8 *p)
{
    *p = 0;
    return p + 1;
}

/* Append one byte. 3 words, 0 differing. */
u8 *func_8009929C(s32 c, u8 *p)
{
    *p = c;
    return p + 1;
}

/* 16 words, 0 differing. */
u8 *func_800992A8(s32 n, u8 *d)
{
    u8 *s;

    if (n != 0)
    {
        s = D_800DD71C;
        if (n <= 0)
        {
            s += 2;
        }
        *d++ = s[0];
        *d++ = s[1];
    }

    return d;
}

/* Append a string to `dst` and hand back the new cursor.

   THEY ARE SPELLED `strlen` / `memcpy`, NOT `func_80069E98` /
   `func_80069ED8`. Both names link, now that build/dungeon.bin passes
   --script=splat/common_syms.txt like town's and main's rules, and both
   measure the same bytes; the library spellings are the readable ones.
   seqcheck canonicalises a PSY-Q name to its `func_` spelling before
   comparing, so the ordered symbol check stays clean either way.
   32 words, 0 differing. */
u8 *func_800992E8(u8 *p, u8 *dst)
{
    u8 *src;
    s32 n;
    s32 len;

    if (*(u32 *)(D_800E3D7C + 0x1C) & 0x10)
    {
        src = D_800DD724;
    }
    else
    {
        src = (u8 *)func_8004AC3C(p, &n);
    }
    len = strlen(src);
    memcpy(dst, src, len);
    return dst + len;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B64);

/* func_8009955C's body -- the same two optional strings around
   func_800992E8, gated on the same engine flag and the same (entry, kind)
   pair -- plus a switch on the entry's +0x1 kind that appends the trailing
   quantity. jtbl_80088C60 is at 0 mod 8, so `.align 3` before it is a
   no-op and the table costs nothing (see docs/dungeon-sweep.md, "SEVEN of
   the 28 jump tables are at 4 mod 8"); seqcheck reports the table's `%hi`
   as `.rodata` rather than `jtbl_80088C60`, which is the known-benign
   stub-to-C relocation class.

   Kind 4 copies two bytes out of D_800DD720, writes the signed +0x2 count
   through func_8003AD08 and copies two more; kinds 0xE, 0x12, 0x13 and 0x15
   append nothing; every other kind writes the sign pair through
   func_800992A8 and then the magnitude. The `bgez`/`negu` pair with a live
   instruction in its delay slot is `if (c < 0) { c = -c; }`, not an abs
   ternary -- the ternary's trio carries its own `nop`.

   RETURN AND SECOND PARAMETER RETYPED from `void *` to `u8 *` so the
   `*out++ = *q++;` chain compiles; the unit's own declaration block already
   records that `u8 *` is the right spelling for this family (see
   func_80099978 at the top of the file). func_8009965C and func_8009955C
   both re-measured at 0 differing after the change.

   125 words, 0 differing on the first compile. */
u8 *func_80099368(u8 *p, u8 *out)
{
    u8 *s;
    u8 *q;
    u32 k;
    s32 c;

    if ((*(u32 *)(D_800E3D7C + 0x1C) & 0x10) == 0)
    {
        k = p[1];
        s = D_800DD72C[k];
        if (s != NULL && (k != 0xF || p[0] < 0xE))
        {
            out = func_80099194(s, out);
        }
    }
    out = func_800992E8(p, out);
    if ((*(u32 *)(D_800E3D7C + 0x1C) & 0x10) == 0)
    {
        k = p[1];
        s = D_800DD784[k];
        if (s != NULL && (k != 0xF || p[0] < 0xE))
        {
            out = func_80099194(s, out);
        }
        switch (p[1])
        {
        case 4:
            if ((p[3] & 0x80) == 0)
            {
                q = D_800DD720;
                c = *(s8 *)(p + 2);
                *out++ = *q++;
                *out++ = *q++;
                out = func_8003AD08(c, out);
                *out++ = *q++;
                *out++ = *q++;
            }
            break;
        case 0xE:
        case 0x12:
        case 0x13:
        case 0x15:
            break;
        default:
            if ((p[3] & 0x80) == 0)
            {
                c = *(s8 *)(p + 2);
                if (c != 0)
                {
                    out = func_800992A8(c, out);
                    if (c < 0)
                    {
                        c = -c;
                    }
                    out = func_8003AD08(c, out);
                }
            }
            break;
        }
    }
    return out;
}

/* Two optional prefix/suffix strings around func_800992E8's body, each gated
   on the same engine flag and the same (entry, kind) pair. `k` is `u32`: the
   0xE bound is `sltiu`, and an `s32` local gives `slti`. 64 words,
   0 differing. */
void *func_8009955C(u8 *p, void *out)
{
    u8 *s;
    u32 k;

    if ((*(u32 *)(D_800E3D7C + 0x1C) & 0x10) == 0)
    {
        k = p[1];
        s = D_800DD72C[k];
        if (s != NULL && (k != 0xF || p[0] < 0xE))
        {
            out = func_80099194(s, out);
        }
    }
    out = func_800992E8(p, out);
    if ((*(u32 *)(D_800E3D7C + 0x1C) & 0x10) == 0)
    {
        k = p[1];
        s = D_800DD784[k];
        if (s != NULL && (k != 0xF || p[0] < 0xE))
        {
            out = func_80099194(s, out);
        }
    }
    return out;
}

/* `func_80042A80(p)` compiles to a bare `jal` with a `nop` delay slot because
   the incoming $a0 still holds p -- written `func_80042A80()` the slot takes
   the next call's `addiu $a0, $zero, 0xA` instead and the function is 2
   differing. A missing argument move is not evidence of a nullary call.
   54 words, 0 differing. */
u8 *func_8009965C(u8 *p, u8 *q)
{
    if ((*(u32 *)(D_800E3D7C + 0x1C) & 0x10) == 0)
    {
        if ((p[3] & 0x80) != 0 || func_80042A80(p) != 0)
        {
            q = func_80099194(D_800E18C0, func_80099368(p, func_8009929C(0xA, q)));
            p[3] &= 0x7F;
            func_800A90E8(p);
            q = func_80099194(D_80088CA8, func_80099368(p, q));
            func_800A90E8(p);
        }
    }
    return q;
}

/* Append the record's display name. `p[0x13]` IS WRITTEN OUT AT BOTH USES,
   not taken into a local: a local gets a callee-saved-class register and the
   whole D_8007359C index reallocates around it -- 50 words, 10 differing,
   with s32, s16 and u8 all identical. 50 words, 0 differing. */
void *func_80099734(u8 *p, u8 *d)
{
    u8 *s;

    if ((*(s32 *)(D_800E3D7C + 0x1C) & 0x10) && p[0x13] != 0)
    {
        s = D_800DD728;
    }
    else if (*(s32 *)(p + 0x14) & 0x4000)
    {
        s = p + 0x34;
    }
    else
    {
        if (p[0x13] == 0x23 && *(s16 *)(p + 0xA6) != 0)
        {
            s = D_800DD728;
        }
        else
        {
            s = (u8 *)D_8007359C[0][p[0x13]].unk_04;
        }
    }
    while (*s != 0)
    {
        *d++ = *s++;
    }

    return d;
}

/* 18 words, 0 differing. */
void func_800997FC(u8 *s)
{
    void *h;

    h = func_800990FC();
    func_80099290(func_80099194(s, h));
    func_800A5720(h);
}

/* The idiom with no skeleton at all and the string as a PARAMETER: the
   overlay's one-line "X does Y" reporter, which the kill handlers call
   instead of spelling the chain out. 24 words, 0 differing. */
void func_80099844(u8 *p, u8 *s)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099734(p, h);
    h = func_80099194(s, h);
    func_80099290(h);
    func_800A5720(m);
}

/* Append the two-byte 0x81 0x21 control pair. 7 words, 0 differing. */
u8 *func_800998A4(u8 *p)
{
    *p++ = 0x81;
    *p++ = 0x21;
    return p;
}

/* Append the two-byte glyph at D_800E045F when the source pair is 0x82
   followed by one of ten second bytes. IT RETURNS THE ADVANCED CURSOR, and
   the declaration above was `void` -- both call sites drop the value and both
   still measure 0 differing. THE TEN SECOND BYTES ARE A `||` CHAIN, not a
   switch: nine `beq` to the shared body and a tenth `bne` to the exit is
   exactly what `||` lays out. 46 words, 0 differing. */
u8 *func_800998C0(u8 *p, u8 *s)
{
    u8 *t;
    s32 c;

    t = D_800E045F;
    if (s[0] == 0x82)
    {
        c = s[1];
        if (c == 0x81 || c == 0x89 || c == 0x95 || c == 0x85 || c == 0x8F || c == 0x60 ||
            c == 0x68 || c == 0x74 || c == 0x64 || c == 0x6E)
        {
            *p++ = t[0];
            *p++ = t[1];
        }
    }
    return p;
}

/* THE PARAMETER IS THE CURSOR. Taken into a local off a `void *` parameter,
   combine collapses all four increments and the stores come out at
   displacements 0..3; with the parameter itself advanced, three `addiu`
   survive and the fourth folds into the return. 14 words, 0 differing. */
u8 *func_80099978(u8 *p)
{
    *p++ = D_800E045D[0];
    *p++ = D_800E045D[1];
    *p++ = 7;
    *p++ = 7;

    return p;
}

/* Append the 0x12 0x10 control pair, terminate, hand back the cursor.
   9 words, 0 differing. */
u8 *func_800999B0(u8 *p)
{
    *p++ = 0x12;
    *p++ = 0x10;
    *p++ = 0;
    return p;
}

/* 18 words, 0 differing. */
void func_800999D4(u8 *p)
{
    *(s32 *)(p + 0x8) += 1;
    if (--*(s16 *)(p + 0x10) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Walk the node chain from `p`'s own node, projecting each one through
   RotTransPers into the scratchpad and emitting its tag. The scratchpad base
   is the CONSTANT 0x1F800000, whose low half is zero, so it is a one-word
   `lui` held in $s0 across every call. 63 words, 0 differing. */
s32 func_80099A1C(u8 *p, UnkVec *m, u8 *r)
{
    u8 *sp;
    UnkNode *n;

    sp = (u8 *)0x1F800000;
    do
    {
        *(s16 *)(sp + 0x0) = *(u16 *)((u8 *)m + 0x2);
        *(s16 *)(sp + 0x2) = *(u16 *)((u8 *)m + 0x6);
        *(s16 *)(sp + 0x4) = *(u16 *)((u8 *)m + 0xA);
        *(s32 *)(sp + 0xC0) = RotTransPers((SVECTOR *)sp, (s32 *)(sp + 0xB8),
                                           (s32 *)(sp + 0x90), (s32 *)(sp + 0x94)) -
                              *(s16 *)(r + 0x6);
        *(s16 *)(sp + 0xB8) = *(u16 *)(sp + 0xB8) - ((s16)(*(u16 *)p) >> 1);
        *(s16 *)(sp + 0xB8) =
            *(u16 *)(sp + 0xB8) +
            ((func_800644B8((*(s16 *)(p + 0xC) << 8) + (*(s32 *)(p + 0x8) << 7)) *
              (*(s32 *)(p + 0x8) >> 2)) >>
             9);
        func_800C96E8(p, sp);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        m = n->unk_08;
        r = (u8 *)n->unk_0C;
    } while (1);
    return 0;
}

/* The flashing-flag step: bail out with the node's redraw bit raised while
   the renderer's +0x14 carries any of 0xE000, otherwise tick +0x10, drop
   bit 7 of the renderer's flags once it goes positive, and -- when +0x0E is
   still set -- advance the mover by its delta, bleed a quarter off each
   delta and halve the three colour bytes. 80 words, 0 differing, first
   compile. The colour bytes are `srl`, so UnkSlot.unk_0C is the u8[4] it is
   already declared as; `sra` there would mean a signed field. */
void func_80099B18(u8 *p, UnkVec *v, UnkSlot *r)
{
    if ((r->unk_14 & 0xE000) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    *(s16 *)(p + 0x10) += 1;
    if (*(s16 *)(p + 0x10) > 0)
    {
        r->unk_14 &= 0xFF7F;
        func_800478B8((u8 *)r);
    }
    if (*(s16 *)(p + 0xE) == 0)
    {
        return;
    }
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C -= v->unk_0C >> 2;
    v->unk_10 -= v->unk_10 >> 2;
    v->unk_14 -= v->unk_14 >> 2;
    r->unk_0C[0] -= r->unk_0C[0] >> 1;
    r->unk_0C[1] -= r->unk_0C[1] >> 1;
    r->unk_0C[2] -= r->unk_0C[2] >> 1;
}

/* Spawn the 0x212 flash at (a, b, c) and then sixteen more behind it on a
   descending radius, all on func_80099B18, with the element table chosen by
   two bits of `d`. The first spawn and the loop body are the same block
   written twice; there is no call between them to share.

   `r->unk_08 = r->unk_00->unk_04;` IS THE FIRST OF THE SLOT STORES, not the
   last. Written last -- which is where it reads naturally, after the five
   constants -- sched1 cannot hoist its two dependent loads far enough and
   the record pointer lands in $a2 with the 0x808080 constant in $v1, where
   the target has $a3 and $a2: 147 words, 36 differing, every one of them a
   consequence of that. Second position is 10 differing; first is 0.

   The element table is chosen by THREE SEPARATE STORES to `r->unk_00`, one
   per arm, which jump2 cross-jumps back into the single `sw $v0, 0x0($a3)`
   at the join -- that is why the next statement's `lw $v1, 0x0($a3)` is a
   real reload and not the value cse would have had from one conditional
   expression. 147 words, 0 differing. */
void func_80099C58(s16 a, s16 b, s16 c, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    s32 i;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        q = (u8 *)n + 0x20;
        m = n->unk_08;
        n->unk_10 = (u32)func_80099B18;
        *(s16 *)((u8 *)m + 0x2) = a;
        *(s16 *)(q + 0x12) = a;
        *(s16 *)((u8 *)m + 0x6) = b;
        *(s16 *)(q + 0x14) = b;
        *(s16 *)((u8 *)m + 0xA) = c;
        *(s16 *)(q + 0x16) = c;
        *(s16 *)(q + 0x10) = 0;
        r = (UnkSlot *)n->unk_0C;
        if ((d & 2) != 0)
        {
            r->unk_00 = (UnkElem *)D_800DEDB0;
        }
        else if ((d & 4) != 0)
        {
            r->unk_00 = (UnkElem *)D_800DEE38;
        }
        else
        {
            r->unk_00 = (UnkElem *)D_800DE870;
        }
        r->unk_08 = r->unk_00->unk_04;
        r->unk_14 |= 0x8C;
        r->unk_10 = 0x20;
        *(u32 *)((u8 *)r + 0xC) = 0x808080;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_8004491C(n, (u32)D_80045340);
    }
    i = 0x10;
    do
    {
        n = func_8003FC64(0x212);
        if (n != NULL)
        {
            q = (u8 *)n + 0x20;
            m = n->unk_08;
            n->unk_10 = (u32)func_80099B18;
            *(s16 *)(q + 0xE) = 1;
            *(s16 *)((u8 *)m + 0x2) = a;
            *(s16 *)(q + 0x12) = a;
            *(s16 *)((u8 *)m + 0x6) = b;
            *(s16 *)(q + 0x14) = b;
            *(s16 *)((u8 *)m + 0xA) = c;
            *(s16 *)(q + 0x16) = c;
            m->unk_14 = func_80064584(i << 8) << 7;
            m->unk_0C = func_800644B8(i << 8) << 7;
            *(s16 *)(q + 0x10) = 0;
            r = (UnkSlot *)n->unk_0C;
            if ((d & 2) != 0)
            {
                r->unk_00 = (UnkElem *)D_800DEDB0;
            }
            else if ((d & 4) != 0)
            {
                r->unk_00 = (UnkElem *)D_800DEE38;
            }
            else
            {
                r->unk_00 = (UnkElem *)D_800DE870;
            }
            r->unk_08 = r->unk_00->unk_04;
            r->unk_14 |= 0x8C;
            r->unk_10 = 0x20;
            *(u32 *)((u8 *)r + 0xC) = 0x808080;
            r->unk_1E = 0x1000;
            r->unk_1C = 0x1000;
            func_8004491C(n, (u32)D_80045340);
        }
        i--;
    } while (i > 0);
}

/* THE +0x6C UPDATE IS THE SECOND STATEMENT, NOT THE LAST. All 120 orders of
   the other five were measured with the 0x71 store held first: the six that
   put the multiply second are the only ones that reach 0 differing, and every
   order leaving it last is 25 words -- the `lh` of D_80083460 then has nothing
   to fill its load-delay slot. The relative order of the remaining four is a
   measured TIE, so the listing does not witness it. D_80083460's first
   halfword is read with `lh`, so it is reached as `*(s16 *)&D_80083460`
   rather than through this unit's usual base pointer, which would cost the
   `addiu`. 24 words, 0 differing. */
void func_80099EA4(u8 *p)
{
    p[0x71] = 0x80;
    *(s16 *)(p + 0x6C) += (*(s16 *)(p + 0x6E) * *(s16 *)&D_80083460) >> 8;
    p[0x42] = 0;
    p[0x41] = 0;
    *(s32 *)(p + 0x1C) &= 0x7FFFFFFF;
    *(s32 *)(p + 0x14) &= 0x7FFE7FFF;
}

/* Walk the ring from the node's own payload until it comes back to the head
   D_800814A8 holds, running func_80099EA4 on every record whose +0x14 lacks
   0x4000. The engine's head is RE-READ each turn -- the `%hi` stays in $s1
   across the loop and the `lw` is inside it. 27 words, 0 differing. */
void func_80099F04(u8 *n)
{
    u8 *p;

    p = n + 0x20;
    while (p != (u8 *)D_800814A8)
    {
        if (!(*(u32 *)(p + 0x14) & 0x4000))
        {
            func_80099EA4(p);
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* Walk the engine's node list from p and run func_80099EA4 on every view
   whose +0x14 carries 0x4000. D_800814A8 HOLDS the list terminator's address,
   so the comparison is against the value, not against `&D_800814A8`.
   27 words, 0 differing. */
void func_80099F70(u8 *p)
{
    u8 *q;

    q = p + 0x20;
    while ((u32)q != D_800814A8)
    {
        if (*(u32 *)(q + 0x14) & 0x4000)
        {
            func_80099EA4(q);
        }
        q = *(u8 **)(q + 0x5C) + 0x20;
    }
}

/* 19 words, 0 differing. */
void func_80099FDC(UnkNode *n)
{
    u8 *p;
    u8 *q;

    n->unk_10 &= 0x7FFFFFFF;
    q = *(u8 **)(D_800814A8 + 0x58) + 0x20;
    p = (u8 *)n + 0x20;
    *(u8 **)(p + 0x5C) = *(u8 **)(q + 0x5C);
    *(u8 **)(p + 0x58) = *(u8 **)(D_800814A8 + 0x58);
    *(UnkNode **)(D_800814A8 + 0x58) = n;
    *(UnkNode **)(q + 0x5C) = n;
}

/* 20 words, 0 differing. */
void func_8009A028(u8 *p)
{
    u8 *a;
    u8 *b;

    if (D_800E3DE8 == p - 0x20)
    {
        D_800E3DE8 = *(u8 **)(p + 0x5C);
    }
    a = *(u8 **)(p + 0x58);
    b = *(u8 **)(p + 0x5C);
    if (a != NULL && b != NULL)
    {
        *(u8 **)(a + 0x7C) = b;
        *(u8 **)(b + 0x78) = a;
        *(u8 **)(p + 0x5C) = NULL;
        *(u8 **)(p + 0x58) = NULL;
    }
}

/* Set the 0x1000 flag on every visible node of the engine's ring and give
   each one to func_8009A028, then raise D_800E296C's 0x10000000.

   THE NODE HEADER NEEDS ITS OWN POINTER. Written
   `((UnkNode *)(p - 0x20))->unk_1E |= 0x1000;` the -0x20 folds into the
   displacement and the pair becomes `lhu -0x2($a0)` / `sh -0x2($a0)` -- 32
   words against 33, 21 differing. A named local keeps the
   `addiu $v1, $a0, -0x20` the target has. Contrast func_800BC388 below,
   whose target genuinely wants the folded form: it is a per-site question,
   decided by whether the listing materialises the base. The ring head is NOT
   cached -- the loop test re-reads D_800E3D7C. 33 words, 0 differing. */
void func_8009A078(void)
{
    u8 *p;
    u8 *n;
    UnkNode *e;

    p = D_800E3D7C;
    do
    {
        n = *(u8 **)(p + 0x5C) + 0x20;
        if ((*(s32 *)(p + 0x14) & 0x4000) == 0)
        {
            e = (UnkNode *)(p - 0x20);
            e->unk_1E |= 0x1000;
            func_8009A028(p);
        }
        p = n;
    } while (p != D_800E3D7C);
    D_800E296C |= 0x10000000;
}

/* Unlink every node in D_80081498's list whose +0x1E carries 0x1000, then
   drop the 0x10000000 bit unless D_80012090 is 1. D_80012090 IS A CONSTANT
   ADDRESS, not the declared symbol: the target's `lui` destination IS the
   load's destination, which is what a PLUS gives where a LO_SUM gets the
   `high` its own register -- the symbol spelling is 2 differing.
   33 words, 0 differing. */
void func_8009A0FC(void)
{
    UnkNode *n;

    for (n = D_80081498; n != NULL; n = n->unk_00)
    {
        if (n->unk_1E & 0x1000)
        {
            func_80099FDC(n);
        }
    }
    if (*(s32 *)0x80012090 != 1)
    {
        D_800E296C &= ~0x10000000;
    }
}

/* Move a payload record out of one chain and into b's.

   THE FLAG TEST IS SPELLED AS A BIT TEST, NOT AS A SIGN TEST. With
   `(*(u32 *)(a + 0x1C) & 0x80000000) != 0` the loaded word takes $v0 and the
   constant $v1 exactly as the target has them; `*(s32 *)(a + 0x1C) < 0` is 4
   differing at the right length and every one of the four is those two
   registers swapped. The `(s32 *)` cast with the same `&` measures 0 too, so
   it is the bit test and not the cast. The trailing `return 0` also has to be
   the function's LAST statement: the `if (clear) { ...; return 1; } return 0;`
   form hoists the zero above the branch and is 38 words of 39, 24 differing.
   39 words, 0 differing. */
s32 func_8009A180(u8 *a, u8 *b)
{
    u8 *q;
    u8 *n;

    if (a == b)
    {
        *(s32 *)(b + 0x1C) |= 0x80000000;
        return 0;
    }
    if ((*(u32 *)(a + 0x1C) & 0x80000000) != 0)
    {
        return 0;
    }
    *(u32 *)(a + 0x1C) |= 0x80000000;
    func_8009A028(a);
    q = *(u8 **)(b + 0x5C);
    *(u8 **)(a + 0x5C) = q;
    q += 0x20;
    n = a - 0x20;
    *(u8 **)(a + 0x58) = *(u8 **)(q + 0x58);
    *(u8 **)(q + 0x58) = n;
    *(u8 **)(b + 0x5C) = n;
    return 1;
}

/* THE HALFWORD READ OF THE THIRD PARAMETER NEEDS ITS OWN `u16` LOCAL. The
   parameter is read raw for the mask work and as a halfword for the
   `== 0x300` test; `(c & 0xFFFF) == 0x300` is 39 words / 3 differing -- no
   copy, the `andi 0x1100` rises into the delay slot and the load-delay nop
   goes. `u16 d = c;` is an HImode copy cse cannot fold, which is the
   `addu $t0, $a2, $zero` in the first branch's delay slot. The parameters
   stay s32 with `(s16)` casts; an s16 prototype would cost every landed
   caller a sll/sra pair on a u8 argument. 39 words, 0 differing. */
void func_8009A21C(s32 a, s32 b, s32 c)
{
    Unk80083160 *g = &D_80083160;
    u16 *p;
    s32 w;
    s32 m;
    u16 d = c;

    p = (u16 *)((u8 *)g->unk_1DC + ((s16)a + ((s16)b << (s16)g->unk_1F0)) * 6);
    if ((c & 0xC832) != 0)
    {
        p[2] |= c;
        return;
    }
    w = p[2];
    m = ((w & c) + (c & 0x1100)) & 0x3300;
    p[2] = w & ~c;
    if (m == 0)
    {
        m = 0x3000;
        if (d == 0x300)
        {
            m = 0x300;
        }
    }
    p[2] |= m;
}

/* Is the cell one step in direction `d` from (x, y) passable?

   THE WHOLE INDEX IN ONE EXPRESSION, like func_8008CCB0's. Split into
   `i = x + ...;` then `i += ...;` the D_80083160 pointer is scheduled
   seventh instead of first and nine words shift: 38 words, 11 differing.
   AND THE TAIL IS ONE `||`, NOT TWO `if`s: the two-if form keeps the branch
   but costs the second load $v0 (38 / 2), and collapsing the pair to
   `return *(u16 *)r == 0;` is jump.c's store-flag transform (37 / 17).
   38 words, 0 differing. */
s32 func_8009A2B8(s16 x, s16 y, s32 d)
{
    u8 *g;
    u8 *r;
    s32 i;

    g = (u8 *)&D_80083160;
    i = (x + D_8006CCD8[(s16)d]) + ((y + D_8006CCE8[(s16)d]) << *(s16 *)(g + 0x1F0));
    r = (u8 *)(*(s32 *)(g + 0x1DC) + i * 6);
    if ((*(u16 *)(r + 0x4) & 0xF320) != 0 || *(u16 *)r == 0)
    {
        return 1;
    }
    return 0;
}

/* Look a 6-byte cell up by (x, z) stepped one square in direction `d`, hand
   its +0x04 halfword back through the out-parameter and return its first.

   TWO POSITION FACTS, 13 differing between them. `g` must be assigned
   BETWEEN the two table reads, not as the first statement: written first its
   `lui`/`addiu` lead the function instead of sitting after D_8006CCD8's
   `addu`. And the row address is INDEX-FIRST (`i * 6 + base`): written
   base-first the final `addu` commutes and lands in the base's register,
   which moves the two reads off it as well. 32 words, 0 differing. */
s16 func_8009A350(s16 x, s16 y, s32 d, u16 *out)
{
    u8 *g;
    u8 *r;
    s32 i;

    i = x + D_8006CCD8[(s16)d];
    g = (u8 *)&D_80083160;
    i += (y + D_8006CCE8[(s16)d]) << *(s16 *)(g + 0x1F0);
    r = (u8 *)(i * 6 + *(s32 *)(g + 0x1DC));
    *out = *(u16 *)(r + 0x4);
    return *(s16 *)r;
}

/* DEVICES, both load-bearing and neither with a reason in the source:

   `f++; f--;` gives `f` a SECOND SET for free.  Without it update_equiv_regs
   (local-alloc.c:1094) doubles f's reg_live_length -- the doubling is gated at
   :1043 on `reg_n_sets[regno] != 1' -- f then loses the callee-saved register
   it needs and four of them rotate.  The park spent two days paying for that
   second set with a real statement (`f = cell | m; cell = f;'), which costs
   the two words of an `or'/`sh' pair in $s0 where the target has $v0.  The
   cancelled increment pair compiles to NOTHING (combine folds the two addiu)
   and buys the same second set, so the tail can be the read-modify-write the
   target plainly has.  Found by tools/decomp/refsweep.py.

   `m = n + z0;` with `register s32 z0 __asm__("$0")` is a plain `m = n;` copy
   that local-alloc's optimize_reg_copy_1 cannot eat: local-alloc.c:1025-1028
   requires SET_SRC to be a REG, and a PLUS is not, while `addu $a2, $a0,
   $zero' is the word the target has. See docs/matching-levers.md, "A
   REDUNDANT REGISTER COPY SURVIVES jump2 IF YOU SPELL IT `x + z0'". */

/* WHAT IT IS: (x, y, flags) over the 6-byte map cells at `&D_80083160 +
   0x1DC', row shift at +0x14 of that header.  `flags & 0x8832' non-zero
   CLEARS flags out of the cell's +0x04 halfword -- and when `flags & 0x800'
   only if func_800A6E10(x, y) < 2.  Otherwise it clears them AND ors back the
   saturated `(cell & flags) - (flags & 0x1100)'.

   FOUR MORE SPELLINGS HERE ARE LOAD-BEARING, each measured against its
   alternatives when this was a park:
   * the row shift through UnkDungeonMapHdr, not a `u8 *' -- 13 words, and the
     reason is on the typedef in csrc/ovl_dungeon_decls.h.
   * `r = func_800A6E10(a, b);` into an s32 and then `(s16)r >= 2` -- 2 words.
     The callee is landed as `s16`, so testing the call directly emits NO sign
     extension at all; gcc trusts the SImode-promoted return.
   * the one-armed clamp with the copy FIRST (`m = n + z0;` then
     `if ((s16)n < 0) m = 0;`).  The two-armed `if ((s16)n < 0) m = 0; else
     m = n;` is 22 rows and the other one-arming (`m = 0;` then
     `if ((s16)n >= 0) m = n;`) is 22 as well.
   * `i = index * 6` joined between the two inner arms with `c = (u8 *)(i + t)`
     computed ONCE after the join.  `i = index` with the `* 6` outside is ONE
     WORD SHORT: both arms then start with the same `sll $v0, $a0, 16` into the
     same hard register, and reorg.c fills the `beq`'s delay slot from both
     threads at once and deletes the copy at the branch target.  The target
     keeps two slls precisely because their destinations differ.  The else-arm
     must write all three cell accesses inline, with no pointer local, for the
     same reason. */
void func_8009A3D0(s32 x, s32 y, s32 f)
{
    u8 *v;
    UnkDungeonMapHdr *w;
    s32 t;

    f++;
    f--;
    v = (u8 *)&D_80083160;
    w = (UnkDungeonMapHdr *)(v + 0x1DC);
    t = *(s32 *)(v + 0x1DC);
    if (f & 0x8832)
    {
        s32 i;
        u8 *c;

        if (f & 0x800)
        {
            s32 a;
            s32 b;
            s32 r;

            a = (s16)x;
            b = (s16)y;
            r = func_800A6E10(a, b);
            if ((s16)r >= 2)
            {
                return;
            }
            i = (a + (b << w->unk_14)) * 6;
        }
        else
        {
            i = ((s16)x + ((s16)y << w->unk_14)) * 6;
        }
        c = (u8 *)(i + t);
        *(u16 *)(c + 0x4) &= ~f;
    }
    else
    {
        s32 px;
        s32 py;
        u16 o;
        s32 n;
        s32 m;
        register s32 z0 __asm__("$0");

        px = (s16)x;
        py = (s16)y;
        o = *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4);
        n = (o & f) - (f & 0x1100);
        *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4) = o & ~f;
        m = n + z0;
        if ((s16)n < 0)
        {
            m = 0;
        }
        *(u16 *)((px + (py << w->unk_14)) * 6 + t + 4) |= m;
    }
}

s16 func_8009A540(s32 a, s32 x, s32 y, s16 z)
{
    u16 w;
    s16 zz;
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
    __asm__("" : "=r"(b) : "0"(b));
    zz = z;
    if ((a & 1) != 0)
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
                if (func_800BCB04h((u16)ax, (u16)az, zz) >= 0x201)
                {
                    break;
                }
                u = t;
            }
        }
        {
            register s32 h __asm__("$3");

            h = (i < 2);
            if (h != 0)
            {
                return 0;
            }
        }
    }
    b = a;
    return 1;
}

/* Probe the tile one step along the facing in `f`: ask func_8009A540 whether
   the step is legal at all, func_8009A350 what the tile's flag word is, and
   -- for a tile the 0x3300 bits mark -- func_800BCB04 how high the floor is
   there. Returns 0 for "cannot", -1 for "blocked", and 1/2/3 for a floor
   level, equal to, above or below the walker's own +0x88. 149 words,
   0 differing on the first compile.

   THE FACING WORD IS `u32`: `(f >> 9) & 7` is the unit's established
   direction idiom, and on an `s32` parameter the shift is `sra`.
   The two world coordinates are `u32` locals too, so the `(x << 6) >> 6`
   pair func_8009A540's arguments need comes out as `srl`. */
s32 func_8009A66C(u32 f, u8 *c, u8 *d, s16 y)
{
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 h;
    s32 t;
    s16 n;

    k = (f >> 9) & 7;
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if (func_8009A540(k, px >> 6, pz >> 6, h - y) != 0)
    {
        ax = D_800DCEAC[k] + wx;
        az = D_800DCEBC[k] + wz;
        if (func_8009A350(c[0x24], c[0x25], k, &w) != 0)
        {
            if ((s8)c[0x26] < 0)
            {
                n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k]);
                if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 &&
                    (*(s32 *)(d + 0x1C) & 0x2000) == 0)
                {
                    return 0;
                }
            }
            if ((w & 0x8402) != 0)
            {
                return 0;
            }
            if ((w & 0x3300) != 0)
            {
                if ((w & 0x40) == 0)
                {
                    return -1;
                }
                t = func_800BCB04(ax, az, h - y);
                if (func_8009B25C(d, ax >> 6, az >> 6, t) != 0)
                {
                    return -1;
                }
            }
            else
            {
                t = func_800BCB04(ax, az, h - y);
            }
            if ((s16)t < 0x200)
            {
                if ((s16)t == (s16)h)
                {
                    return 1;
                }
                if ((s16)h < (s16)t)
                {
                    return 2;
                }
                return 3;
            }
        }
    }
    return 0;
}

/* The landed func_8009A66C's probe, one layer in.  184 words, 0 differing.

   Parked at 12 rows -- two `andi`s that should be moves, plus a three-way
   rotation of $s4/$s5/$s6 -- and BOTH halves were diagnosed in the park to
   the compiler pass and the line of compiler source.  Each had exactly one
   missing fact, and both facts turn out to be the same two devices.

   ============================ (A) THE TWO COPIES: `kx + z0` WITH z0 ON `$0`

   The target computes `(f >> 9) & 7` into $a0, copies it to $s5 at word 6,
   and copies THAT back to $a0 at word 22, where it waits twenty-odd words
   and four conditional branches for `jal func_8009A540`.  Three pseudos, two
   surviving MOVES -- and a plain `k = kx;` / `k2 = k;` can never survive,
   because cse.c's make_regs_eqv puts the destination of a reg-to-reg copy
   into the SOURCE's quantity and canon_reg rewrites every use of the
   destination back to qty_first_reg, so the copy goes dead and flow deletes
   it.  Its one escape -- NEW becomes canonical when it will outlive every
   other reg of the quantity, beyond the current basic block -- is true for
   `k = kx` but not for `k2 = k`, so at most one of the two can be a plain
   copy.  The park's best was a WIDENED recomputation (`k = (u8)kx; k2 =
   (u16)kx;`), which cse cannot hash into kx's class, at the cost of emitting
   `andi` where the target has `addu`.

   The spelling that is both is an addsi3 with `$zero` as the literal second
   operand:

       register s32 z0 __asm__("$0");
       k = kx + z0;
       k2 = k + z0;

   `addu $s5, $a0, $zero` is the same word as `move $s5, $a0`, and cse cannot
   hash a PLUS of a hard register into kx's class, so both copies survive as
   real insns.  See docs/matching-levers.md, "A REDUNDANT REGISTER COPY
   SURVIVES jump2 IF YOU SPELL IT `x + z0`" -- written for func_800CA1E0,
   where it defeats jump2's noop-move deletion; here it defeats cse's copy
   propagation, which is a different pass and the same spelling.  Note the
   second copy reads `k`, not `kx`: the target's word 22 is
   `addu $a0, $s5, $zero`.

   =================== (B) THE ROTATION: TWO FREE REFERENCES, PRICED EXACTLY

   global.c:587 allocno_compare is
   `pri = floor_log2(n_refs) * n_refs / live_length * 10000`, and find_reg
   then takes the lowest-numbered free hard register (MIPS defines no
   REG_ALLOC_ORDER).  Off this body's `.lreg` after (A):

       az  5/44 = 2272   ax 5/46 = 2173   c 10/152 = 1973   k<<1 5/56 = 1785
       k   4/50 = 1600   wz 2/18 = 1111   h 4/68 = 1176     wx 2/18 = 1111

   which allocates ..., k<<1 = $s3, k = $s4, h = $s5, wz = $s6.  The target
   needs the order k<<1, **wz, k, h** -- wz = $s4, k = $s5, h = $s6.

   flow_analysis runs ONCE (toplev.c:3463) and BEFORE combine (:3488), so a
   reference costs nothing in the assembly and still counts here.  Two of
   them do it, and the arithmetic says where each must go:

   1. `__asm__("" ::"r"(wz));` AFTER `az = D_800DCEBC[k] + wz;` -- wz goes to
      3 refs.  The park had predicted 3/18 = 1666, which would sit between
      k's 1600 and k<<1's 1785; the asm is a scheduling barrier and lengthens
      wz's range to 21, so it actually lands at 1428 -- still short of k, but
      it lifts wz over `h` and that is four of the twelve rows (words 61, 63,
      137, 160 go).  Eight rows left, k and wz simply swapped.
   2. `__asm__("" ::"r"(k));` AFTER the `n = func_8009FB34(...)` line, deep
      in the `(s8)c[0x26] < 0` arm.  This is the other side of the same
      comparison: rather than lift wz to 1600+, it LOWERS k, because the
      reference extends k's live range far past its last real use at
      func_8009A350's third argument.  With 5 refs over the longer range k
      falls below wz's 1428 and above h's 1176, and the order becomes
      k<<1, wz, k, h.  Zero rows.

      **The position is the whole lever and it was found by sweeping every
      insertion point in the body** -- one measurement per line, which costs
      0.09 s each on a `tools/decomp/onefunc.py` file.  The next-best
      position is 1 row and everything outside that arm is 8.  A free
      reference is a dial, not a switch: the park's own rejected list has
      five attempts at wz's third reference that overshot (`wz &= 0xFFFF;`
      gives 4 refs / 19 = 4210 and takes $s0) or undershot
      (`do { wz = pz + 0x20; } while (0);` gives 3 refs but length 25, so
      1200).  Price the window, then sweep the position.

      THESE TWO `asm`s ARE A LAST RESORT AND THE PLAIN-C ALTERNATIVES WERE
      SWEPT FIRST.  A `do { stmt } while (0);` wrapper buys the same
      loop_depth-weighted reference in ordinary C (flow.c:2128) and is what
      landed func_800A1D4C, so it was tried here exhaustively and it does
      not reach 0 on this function:

        * every single-statement wrapper, all 40 positions ....... best 16
          (and every one of them is WORSE than the 12-row base, which the
           asm pair improves on, so there is no greedy path either)
        * every PAIR of wrappers, 1024 combinations .............. best 16
          (`n = func_8009FB34(...)` plus `t = func_800BCB04(...)`)
        * a duplicated-arm `if (v) { s } else { s }` -- the device that
          landed func_8009DAD8, whose references flow counts twice and
          jump2's cross-jumping then merges -- for each of
          wz/k/wx/h/k2/t at every statement ..................... best 25

      The reason is in the mechanism: the loop NOTEs are a full scheduling
      barrier on the insn that CARRIES them (sched.c:2088), so a do-while
      can only be placed where a statement already is, while the asm can go
      between two statements.  On this function the reference `k` needs is
      in the middle of the `(s8)c[0x26] < 0` arm and there is no statement
      boundary there that also keeps the schedule.

   ================================= THE FIVE LEVERS UNDER ALL OF THAT (j87)

   1. `u8 *d` PLAIN -- no `volatile`, no `(void)&d;`.  The target's
      `lw $t0, 0x50($sp)` at words 55, 118, 148 and `lw $a0, 0x50($sp)` at
      8009AB1C are RELOAD reloads of a SPILLED PSEUDO, not reads of an
      addressable variable: reload picks $t0, local-alloc would have picked
      $v0/$v1.  `d` spills because this body puts nine pseudos in
      $s0..$s7/$fp, leaving `d` and `y` as the two lowest-priority allocnos
      with nowhere to go -- and `y` spills to 0x18($sp) exactly as the target
      does.  Measured: plain 183w/14, volatile 183w/25, volatile +
      `(void)&d` 184w/26, plain + `(void)&d` 182w/33.
   2. THREE pseudos for the facing index, with func_8009A540's argument
      taken from its OWN copy in the FIRST basic block.  That is what puts a
      copy of k in $a0 at word 22; it is not a hoist, it is a source-level
      copy in block 1 whose pseudo global_alloc gives $a0 (free, no call in
      its range), after which the call's own `(set (reg a0) k2)` is a no-op
      move flow deletes.
   3. `if (ax == 0 || (1 << sh[10]) - 1 < ax) return -1;` -- the first two
      tests JOINED with `||`, and the same for the az pair.  10 rows and 2
      words.  With separate `if`s reorg's fill_slots_from_thread OWNS the
      first test's `return -1` block, steals `addiu $v0, -1` into the
      branch's delay slot and redirects the branch; joining them makes the
      -1 block shared, so reorg cannot own it and fill_simple_delay_slots
      puts `addiu $a2, %lo(D_8008333C)` there instead -- the target's word 28.
   4. The ternary IN func_8009B25C's fourth argument, and the inverted tail
      (`(s16)t >= 0x201 -> return -1` first).  The tail the other way round
      is 183 words / 43 rows.
   5. x/ax and z/az SHARE one local each; separate locals are 180w/102.

   And from the park's rejected list, so it is not re-derived: ONLY ax/az's
   width matters -- `s32`/`u32` for them is 178w/70, so they stay 16-bit --
   while u16/u32 on wx, wz, h, k, kx, k2, t and s32 px/pz are all
   byte-identical; all 24 declaration orders of wx/wz/k/h with wx before wz
   are identical bytes and every order with wz first is 183w/92; all 30 legal
   statement orders of the {h, px, pz, wx, wz} group are identical bytes;
   and cc1psx's SN-only `shorten_lifetimes` pass is NOT behind the word-22
   copy -- `.shorten` has the same 126 insns in the same order as `.sched`. */
s32 func_8009A8C0(u32 f, u8 *c, u8 *d, s16 y)
{
    s16 *sh;
    u16 w;
    u16 ax;
    u16 az;
    u32 px;
    u32 pz;
    s32 wx;
    s32 wz;
    s32 k;
    s32 h;
    s32 t;
    s16 n;
    s32 k2;
    s32 kx;
    register s32 z0 __asm__("$0");

    kx = (f >> 9) & 7;
    k = kx + z0;
    k2 = k + z0;
    sh = (s16 *)D_8008333C;
    ax = c[0x24] + (u16)D_8006CCD8[k];
    if (ax == 0 || (1 << sh[10]) - 1 < ax)
    {
        return -1;
    }
    az = c[0x25] + (u16)D_8006CCE8[k];
    if (az == 0 || (1 << sh[11]) - 1 < az)
    {
        return -1;
    }
    h = *(u16 *)(d + 0x88);
    px = c[0x24] << 6;
    pz = c[0x25] << 6;
    wx = px + 0x20;
    wz = pz + 0x20;
    if ((s16)func_8009A540(k2, px >> 6, pz >> 6, h - y) == 0)
    {
        return 0;
    }
    ax = D_800DCEAC[k] + wx;
    az = D_800DCEBC[k] + wz;
    __asm__("" ::"r"(wz));
    func_8009A350(c[0x24], c[0x25], k, &w);
    if ((w & 0x8002) != 0)
    {
        return 0;
    }
    if ((s8)c[0x26] < 0)
    {
        n = func_8009FB34(c[0x24] + (u16)D_8006CCD8[k], c[0x25] + (u16)D_8006CCE8[k]);
        __asm__("" ::"r"(k));
        if (n >= 0 && (D_800E2970_s[n].unk_0C & 2) != 0 && (*(s32 *)(d + 0x1C) & 0x2000) == 0)
        {
            return 0;
        }
    }
    if ((w & 0x3300) != 0)
    {
        if ((w & 0x40) == 0)
        {
            return -1;
        }
        t = func_800BCB04(ax, az, h - y);
        if (func_8009B25C(d, ax >> 6, az >> 6,
                          (s16)t < 0x201 ? (s16)t : *(s16 *)(d + 0x88)) != NULL)
        {
            return -1;
        }
    }
    else
    {
        t = func_800BCB04(ax, az, h - y);
    }
    if ((s16)t >= 0x201)
    {
        return -1;
    }
    return 1;
}

s32 func_8009ABA0(u16 a, UnkVec *b, u8 *c, s16 d, s16 e)
{
    s32 k;
    u16 x;
    u16 y;
    s16 z;
    u16 m;
    u8 *q;
    u32 f;

    k = (a >> 9) & 7;
    x = *(u16 *)((u8 *)b + 0x2);
    y = *(u16 *)((u8 *)b + 0x6);
    z = d - e;
    if ((s16)func_8009A540(k, x >> 6, y >> 6, z) == 0)
    {
        return -2;
    }
    x += D_8006CCD8[k] * 64;
    y += D_8006CCE8[k] * 64;
    if ((s16)func_8009A350(c[0x24], c[0x25], k, &m) == 0)
    {
        return 0;
    }
    if ((m & 0x8000) != 0)
    {
        return -2;
    }
    if ((m & 0x400) != 0)
    {
        return 0;
    }
    if ((m & 0x3300) != 0)
    {
        z = func_800BCB04(x, y, z);
        /* DEVICE: 4 differing (words 65, 66, 69, 71), worth 0 words -- the
           asm emits nothing.  `z` is `s16`, so the assignment above is
           `(set (reg:HI z) (subreg:HI (reg:SI result)))` and cse records the
           equivalence; the 4th argument's promotion back to int is then
           expanded as `sll`/`sra` off the SImode RESULT pseudo rather than
           off `z`, the result stays live past the copy, and the
           `%hi(D_800814A8)` is pushed out of $v0 into $v1.  The target
           extends out of `z`'s own register ($s2).  An asm that writes `z`
           kills the equivalence without emitting an instruction.  The ~30
           legal spellings in the park header -- every cast on the argument,
           a `w = z` copy, a second `z2` local, block scopes, the duplicated
           `if` arm in four placements -- are all 134/4, because cse re-forms
           the equivalence from the copy each time.  The permuter's own answer
           was to narrow func_800BCB04's return type to `short`, which breaks
           seven landed functions. */
        __asm__("" : "+r"(z));
        q = func_8009B25C((u8 *)D_800814A8, x >> 6, y >> 6, z);
        if (q != NULL)
        {
            f = *(u32 *)(q + 0x1C);
            if ((f & 0x2000) != 0)
            {
                if ((f & 0x628) == 0)
                {
                    if ((*(u32 *)(q + 0x14) & 0x40000000) == 0)
                    {
                        if (*(s16 *)(q + 0x6C) +
                                ((*(s16 *)(q + 0x6E) * *(s16 *)&D_80083460) >> 8) >=
                            0x100)
                        {
                            goto ok;
                        }
                    }
                }
            }
            return -1;
        }
    ok:
    }
    else
    {
        z = func_800BCB04(x, y, z);
    }
    if ((s16)z < 0x200)
    {
        if ((s16)z == d)
        {
            return 1;
        }
        if (d < (s16)z)
        {
            if ((s16)z - d >= 0x41)
            {
                return 4;
            }
            return 2;
        }
        return 3;
    }
    return -2;
}
/* Probe the cell one step along `k` from (c, d): the blocked test, then the
   map cell's own flags, then a second probe at the stepped coordinate.

   FOUR MEASURED LEVERS, and the first is the whole landing.

   THE `do { } while (0)` IS LOAD-BEARING, worth 11 differing at an unchanged
   88 words -- a clean rotation of three callee-saved registers, with `c`/`d`
   in $s2/$s3 and the map base in $s1 where the target has $s1/$s2 and $s3.
   It is `allocno_compare`'s priority, `floor_log2(R) * R / live_length`, and
   the degenerate loop weights every reference inside it by two. Without it
   `.lreg` reads c = 3 refs / 72 insns = 0.0417, d = 3/74 = 0.0405 and
   m = 2/39 = 0.0513, so the map base sorts ABOVE both parameters; with it
   c and d reach FOUR refs, which is where `floor_log2` steps from 1 to 2,
   and the three become 8/72 = 0.1111, 8/74 = 0.1081 and 3/39 = 0.0769 --
   the target's order. k goes 2/23 -> 3/23 and stays on top either way.
   Nothing reachable from the statement structure moves those numbers: six
   positions for the `m` assignment, `*(u8 **)h` for it, three declaration
   orders, an `Unk80083160 *` for g, `u16 *` for m and r, inlining the index,
   early returns in place of the nesting, `&&` for the two cell tests and
   computing x/y before the call all left `.lreg` at exactly 3/72, 3/74 and
   2/39.

   `y` MUST BE `s32` WITH AN EXPLICIT `(s16)` AT EACH USE. As `s16 y` the
   truncation distributes into the PLUS, the add happens in HImode and comes
   out `addu $v1, $v1, $s2` against the target's `addu $v1, $s2, $v1`
   (1 differing, source order does not reach it).

   THE ROW ADDRESS MUST BE WRITTEN OUT AT BOTH USES. Through a `u8 *r` local
   its `addu` commutes to base-first, `addu $v1, $s3, $v1` for
   `addu $v1, $v1, $s3` (1 differing).

   AND c AND d ARE `s32` PARAMETERS WITH `(s16)` CASTS, not `s16` ones:
   as `s16` the sign extension is shared between the first call and the
   x/y adds and the function is 28 differing. The one ASM caller
   (func_8008CD4C at 0x8008CE2C) sign-extends $a2 and $a3 in front of the
   `jal`, so whoever lands it needs the casts at the call site.
   88 words, 0 differing. */
s32 func_8009ADB8(u8 *a, u8 *b, s32 c, s32 d, s16 e)
{
    u8 *g;
    u8 *h;
    u8 *m;
    s32 k;
    s32 i;
    s32 x;
    s32 y;
    s16 t;

    g = (u8 *)&D_80083160;
    h = g + 0x1DC;
    m = *(u8 **)(g + 0x1DC);
    k = (*(u16 *)(a + 0x2A) >> 9) & 7;
    if ((*(s32 *)(b + 0x14) & 0x100) != 0)
    {
        t = func_800BCB04((((s16)c << 6) + 0x20) & 0xFFE0, (((s16)d << 6) + 0x20) & 0xFFE0, e);
        do
        {
            if (t < 0x200)
            {
                x = c + ((u16 *)D_8006CCD8)[k];
                y = d + ((u16 *)D_8006CCE8)[k];
                i = (s16)(x + ((s16)y << *(s16 *)(h + 0x14)));
                if ((*(u16 *)((u8 *)(i * 6 + m) + 0x4) & 0xB300) == 0)
                {
                    if (*(u16 *)((u8 *)(i * 6 + m)) != 0)
                    {
                        if ((s16)func_800BCB04((((s16)x << 6) + 0x20) & 0xFFE0,
                                               (((s16)y << 6) + 0x20) & 0xFFE0, t) < 0x200)
                        {
                            return 1;
                        }
                    }
                }
            }
        } while (0);
    }
    return 0;
}

/* Walk up to `n` tiles along the direction in `ang` and report how many were
   clear: the count is returned one SHORT when func_800A0548, func_8009A350 or
   func_800BCB04 stops the walk, and in full when a stepped-on tile holds a
   0x1F occupant (which is then flagged 0x800000 and announced through
   D_800DD7DC). 147 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING.

   (1) `dir` IS A COPY MADE INSIDE THE LOOP of a value computed outside it:
   `d0 = (ang >> 9) & 7;` before the loop and `dir = d0;` as the loop body's
   first statement. loop.c hoists that invariant copy into the preheader, and
   it is the target's `addu $fp, $a2, $zero` -- the `andi` stays in the entry
   block in a call-clobbered register and only the copy gets $fp. With one
   `dir` assigned before the loop the function is 146 words, one short, with 3
   aligned differences; computing `dir` inside the loop and dropping `d0`
   entirely is 144/142; `d0` and `dir` both assigned before the loop is
   146/123, as is an `s32 ang` with `((u32)ang >> 9)`.

   (2) THE func_8009A350 TEST IS AN if/else WITH `return k - 1` IN THE ELSE
   ARM, not an early `if (... == 0) return k - 1;`. The else arm is emitted
   after the then arm, which is where the target's own 2-word `j`/`sll` block
   sits -- just before the loop tail. As an early return it is laid out inline
   and the function is 148 words with 55 aligned differences. The
   func_800A0548 test above it IS a plain early return and stays one.

   `u16 dir` instead of `s32 dir` is 147 words, 20 differing; hoisting
   `&D_8006CCD8[dir]` into a `u16 *t` by hand before the loop is 148/144 --
   the hoist is loop.c's to make, not the source's. */
s16 func_8009AF18(u16 ang, UnkVec *b, u8 *c, s16 n)
{
    s16 k;
    s32 dir;
    s32 d0;
    s32 x;
    s32 y;
    s32 xx;
    s32 yy;
    s32 wx;
    s32 wz;
    u16 w;
    u8 *r;

    D_800DD7DC = 0;
    d0 = (ang >> 9) & 7;
    x = c[0x24];
    y = c[0x25];
    wx = (x << 6) | 0x20;
    wz = (y << 6) | 0x20;
    for (k = 1; k <= n; k++)
    {
        dir = d0;
        xx = (s16)x;
        yy = (s16)y;
        if (func_800A0548(xx, yy) != 0)
        {
            return k - 1;
        }
        if (func_8009A350(xx, yy, dir, &w) != 0)
        {
            x += ((u16 *)D_8006CCD8)[dir];
            y += ((u16 *)D_8006CCE8)[dir];
            if ((w & 0x3300) != 0)
            {
                r = func_8009B25C(D_800E3D7C, x, y, *(s16 *)(D_800E3D7C + 0x88));
                if (r != NULL && r[0x13] == 0x1F && (*(s32 *)(r + 0x1C) & 0x228) == 0 &&
                    *(s8 *)(*(u8 **)(D_800E3D7C + 0x124) + 0x13) < 0)
                {
                    D_800DD7DC = 1;
                    *(s32 *)(r + 0x14) |= 0x800000;
                }
                return k;
            }
            wx += ((u16 *)D_800DCEAC)[dir];
            wz += ((u16 *)D_800DCEBC)[dir];
            if (func_800BCB04h((u16)wx, (u16)wz, *(s16 *)((u8 *)b + 0xA)) >= 0x200)
            {
                return k - 1;
            }
        }
        else
        {
            return k - 1;
        }
    }
    return k;
}

/* A reachability probe. THREE SPELLINGS, ALL MEASURED. (1) The two sums must
   be s32 LOCALS of their own, assigned before either shift: inline in the
   call arguments the second table's address chain is scheduled after the
   first value's `sll`/`sra`, 5 differing, and the two sums in one expression
   each is 42. (2) The sine/cosine entries are read with `lhu`, so they take a
   `(u16)` cast -- as the declared s16 the load is `lh`, 2 differing.
   (3) `* 64` rather than `<< 6`: the shift form lets the final `(u16)` mask
   propagate back and delete the sign extension, two words. The tail is nested
   `if`s with ONE trailing `return 1`; three flat early returns let jump.c
   store-flag the last pair and the function is two words short.
   62 words, 0 differing. */
s32 func_8009B164(u16 a, UnkVec *b, u8 *c)
{
    s32 k;
    u16 w;
    u16 x;
    u16 y;
    s32 u0;
    s32 u1;

    k = (a >> 9) & 7;
    if ((s16)func_8009A350(c[0x24], c[0x25], k, &w) != 0)
    {
        if ((w & 0x8000) == 0)
        {
            u0 = c[0x24] + (u16)D_8006CCD8[k];
            u1 = c[0x25] + (u16)D_8006CCE8[k];
            x = (u16)(((s16)u0 * 64) | 0x20);
            y = (u16)(((s16)u1 * 64) | 0x20);
            if ((s16)func_800BCB04(x, y, (s16)((b->unk_08 + b->unk_14 * 2) >> 16)) >= 0x200)
            {
                return 0;
            }
        }
        else
        {
            return 0;
        }
    }
    return 1;
}

/* Walk the record's own ring looking for a neighbour standing on the given
   tile within 0x40 of the given facing, with func_8009A350's terrain mask
   as the gate. func_8009B390 below is this source reading the ring head
   from D_800E3D7C instead and without the empty-ring test.

   THE PARAMETER IS THE WALKER. Writing the walk through a separate local
   and keeping `p` as the head costs a word and moves four registers: the
   target copies `p` into the head variable in the `bnez` delay slot of the
   mask test and then advances `p` itself, which is what reusing the
   parameter gives. 77 words, 0 differing. */
u8 *func_8009B25C(u8 *p, u16 x, u16 y, s16 ang)
{
    u8 *h;
    u8 *e;
    u16 m;
    s16 a;
    s32 d;

    if (func_8009A350(x - 1, y, 0, &m) == 0)
    {
        return NULL;
    }
    if (!(m & 0x3300))
    {
        return NULL;
    }
    h = p;
    p = *(u8 **)(p + 0x5C) + 0x20;
    if (p != h)
    {
        a = ang;
        do
        {
            e = *(u8 **)(p - 0x14);
            if (e[0x24] == x && e[0x25] == y)
            {
                d = *(s16 *)(p + 0x88) - a;
                if (d < 0)
                {
                    d = -d;
                }
                if (d < 0x40)
                {
                    if (func_800A41F0(p) != 0)
                    {
                        return p;
                    }
                }
            }
            p = *(u8 **)(p + 0x5C) + 0x20;
        } while (p != h);
    }
    return NULL;
}

/* func_8009B25C over the engine's own ring: three parameters instead of
   four, the head out of D_800E3D7C, and no empty-ring test because that
   ring always has the player in it. 72 words, 0 differing. */
u8 *func_8009B390(u16 x, u16 y, s16 ang)
{
    u8 *p;
    u8 *h;
    u8 *e;
    u16 m;
    s16 a;
    s32 d;

    if (func_8009A350(x - 1, y, 0, &m) == 0)
    {
        return NULL;
    }
    if (!(m & 0x3300))
    {
        return NULL;
    }
    p = D_800E3D7C;
    h = p;
    a = ang;
    do
    {
        e = *(u8 **)(p - 0x14);
        if (e[0x24] == x && e[0x25] == y)
        {
            d = *(s16 *)(p + 0x88) - a;
            if (d < 0)
            {
                d = -d;
            }
            if (d < 0x40)
            {
                if (func_800A41F0(p) != 0)
                {
                    return p;
                }
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != h);
    return NULL;
}

/* func_8009B25C's ring walk with no angle test and the tile gate the other
   way round. `h = p;` MUST BE THE FIRST STATEMENT, ahead of the gate: written
   after it the copy is emitted inside the loop preheader, fills the
   `lw 0x5C` load delay that the target leaves as a `nop`, and the function is
   62 words against 63 with 53 differing. 63 words, 0 differing. */
u8 *func_8009B4B0(u8 *p, u16 x, u16 y)
{
    u8 *h;
    u8 *e;
    u16 m;

    h = p;
    if (func_8009A350(x - 1, y, 0, &m) != 0 && (m & 0x3300) == 0)
    {
        return NULL;
    }
    p = *(u8 **)(p + 0x5C) + 0x20;
    if (p != h)
    {
        do
        {
            e = *(u8 **)(p - 0x14);
            if (e[0x24] == x && e[0x25] == y)
            {
                if (func_800A41F0(p) != 0)
                {
                    return p;
                }
            }
            p = *(u8 **)(p + 0x5C) + 0x20;
        } while (p != h);
    }
    return NULL;
}

/* Step one tile along the actor's own facing and decide what is standing
   there: func_8009A540 vetoes the step, func_8009B25C hands back whatever
   occupies the tile, and when nothing does, func_800A70E4 picks a free slot
   and func_800A8E74 spawns the node for it. Returns the occupant, (u8 *)1
   when the spawn failed, or NULL. 142 words, 0 differing.

   FOUR SPELLINGS WERE LOAD-BEARING, in the order they paid.

   (1) ONE TRAILING `return NULL`, with everything nested inside the
   func_8009A540 gate. Both NULL exits then branch to the one `addu $v0,
   $zero, $zero` block the compiler puts after all the code. Written as two
   early returns -- `if (func_8009A540(...) == 0) return NULL;` and later
   `if (n < 0) return NULL;` -- gcc keeps the shared stub in the MIDDLE of the
   function, the second test inverts to `bgez`, and it is 142 words with 34
   aligned differences. Same for the single `return r`: with two early
   `return r`s gcc builds a `j`/`move` stub and the two tests branch to it,
   where the target branches straight to the epilogue with `addu $v0, $s0,
   $zero` in each delay slot (51 aligned differences).

   (2) `hp = &D_800814A8;` -- A POINTER LOCAL TO THE SYMBOL, read as `*hp` at
   both sites. That is what gives ONE `lui $s6, %hi(D_800814A8)` in a
   callee-saved register with the `%lo` folded into each `lw`'s displacement.
   Named directly as `(u8 *)D_800814A8` each site materialises its own
   `lui`/`lw` pair, $s6 is never saved, and the function is 141 words -- one
   short -- with 24 aligned differences. An array alias
   `extern u32 D_800814A8_a[] __asm__("D_800814A8");` read as `[0]`,
   `*(u32 *)&D_800814A8` and `(&D_800814A8)[0]` all measure identically to the
   plain scalar, 141/116; only the pointer local moves it.

   (3) `h2 = (u8 *)*hp;` READ BEFORE `*(u8 **)(q + 0x80) = p;`. The load has
   to precede the store or reorg cannot put the store in the `bne`'s delay
   slot: with the store first it fills the slot from the join block instead
   and the function is 143 words. Paired with (4) it is the difference
   between 14 and 1 aligned differences.

   (4) `w = *(u32 *)s;` bound to a local ahead of the two stores, with the
   stores left in target order. Without the local the `lw 0($s1)` cannot be
   scheduled into the `lui`'s shadow and a load-delay `nop` appears; swapping
   the two stores instead removes the nop but puts the word store first (11
   aligned differences).

   (5) `s32 v` with the test written `(s16)v < 0`. As `s16 v` with `v < 0` the
   `sll` reads the call result where the target reads the truncating copy --
   one register row, 142/1. `s8 v`, an `s32` temporary feeding `v`, and an
   `if (v >= 0) {} else` all score the same 1; `(s16)` applied at the
   assignment instead is 142/3. */
u8 *func_8009B5AC(u8 *p, s16 x, s16 y)
{
    u8 *r;
    u8 *q;
    s32 d;
    s32 v;
    s32 n;
    UnkDungeonSlot4 *s;
    u8 *h2;
    u32 w;
    u32 *hp;

    hp = &D_800814A8;
    d = (*(u16 *)(p + 0x2A) >> 9) & 7;
    if (func_8009A540(d, (s16)(x - ((u16 *)D_8006CCD8)[d]), (s16)(y - ((u16 *)D_8006CCE8)[d]),
                      *(u16 *)(p + 0x88) - 0x20) != 0)
    {
        r = func_8009B25C(p, x, y, *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if ((*(s32 *)(r + 0x14) & 0x80000) != 0)
            {
                if (p == (u8 *)*hp)
                {
                    D_80081484[0] = r[0x13];
                    D_80081484[1] = 0x13;
                    v = 0x14;
                    if ((*(s32 *)(r + 0x14) & 0x4000) != 0)
                    {
                        v = func_800A4298(r);
                        if ((s16)v < 0)
                        {
                            v = 0x14;
                        }
                    }
                    D_80081484[3] = v;
                    D_80081484[2] = 0;
                }
            }
            return r;
        }
        n = (s16)func_800A70E4(x, y, *(s16 *)(p + 0x88));
        if (n >= 0)
        {
            s = &D_800E3548_t[n];
            q = (u8 *)func_800A8E74(p, *(u8 **)(p - 0x18), *(s32 *)(p - 0x14), p, (u8 *)s, n);
            if (q == NULL)
            {
                return (u8 *)1;
            }
            h2 = (u8 *)*hp;
            *(u8 **)(q + 0x80) = p;
            if (p == h2)
            {
                w = *(u32 *)s;
                *(u8 **)(q + 0xBC) = D_80081484;
                *(u32 *)D_80081484 = w;
            }
            s->unk_01 = 0;
            s->unk_00 = 0;
            func_8009A3D0(x, y, 0x800);
            *(s32 *)(q + 0x34) |= 0x80000;
            *(s32 *)(q + 0x3C) |= 0x2000;
            return q + 0x20;
        }
    }
    return NULL;
}

/* THE TWO FAILURE TESTS ARE ONE `&&`, NOT TWO EARLY RETURNS. Written as
   separate `return 0;` guards the second one becomes a skip-over with its
   own `j` and the function is 41 words against 42, 26 differing. Joined,
   both land on the tail's single `return 0` and reorg duplicates the
   `addu $v0, $zero, $zero` into the second branch's delay slot, which is
   the two zeroing sites the target has. 42 words, 0 differing. */
s32 func_8009B7E4(u8 *a, u8 *b)
{
    s16 x;
    s16 y;
    u8 *q;

    q = func_8009B25C(b, a[0x24], a[0x25], *(s16 *)(b + 0x88));
    if (q != NULL && (s16)func_80042900((s8 *)q, 0x1B) == 0)
    {
        if (func_8009B88C(b, a[0x24], a[0x25], &x, &y) == 0)
        {
            return 2;
        }
        a[0x24] = x;
        a[0x25] = y;
        return 1;
    }
    return 0;
}

/* "Find a free cell at or next to (x, y)" for the dungeon. Stage one walks the
   actor ring -- `a`'s ring, or D_800E3D7C's when `a` is NULL, which is what the
   `first` flag is for: it lets the D_800E3D7C walk run the FULL ring even though
   the first step lands back on the head -- and marks a 25-byte claim table in the
   scratchpad at 0x1F800000: slot 0 is the tile itself, 1..8 the eight
   D_8006CCD8/E8 neighbours, 9..24 the sixteen D_800DCE6C/8C ones. An actor within
   Chebyshev distance 2 claims the slot func_80065F90's angle picks. Stages two
   and three then hand back the first UNclaimed cell that func_800A0548 says is on
   the map, func_8009A350 says exists without the 0x8000/0x400 flags (0x400 is
   waived when the ring head's +0x1C has 0x40000), and func_800BCB04 puts below
   0x200. 360 words, 0 differing.

   THE SHAPE WAS RIGHT AT 372 WORDS / 228 ALIGNED; EVERY ONE OF THE SEVEN
   LOAD-BEARING SPELLINGS BELOW IS AN ALLOCATION OR AN RTL-OPERAND-ORDER FACT.

   (1) `xx = x; yy = y;` -- THE TWO s32 PARAMETERS MUST BE COPIED INTO LOCALS.
   A never-reassigned parameter gets about DOUBLE the REG_LIVE_LENGTH of an
   identical mid-function pseudo (x: 16 refs / 552 against xx: 16 refs / 276), so
   global.c's floor_log2(refs)*refs/live_length ranks the raw parameters BELOW
   `h` and the (s16)x preheader copies, and both parameters spill: 372 words,
   228 aligned. The copies cost nothing -- they coalesce into the
   `addu $s5,$a1,$zero` the prologue has anyway. Dropping either one is 370
   words and 132 (yy) or 169 (xx) aligned.

   (2) `ax = dx >= 0 ? dx : -dx;` INTO A SECOND VARIABLE. abssi2 emits its
   three-word `bgez/move/subu` form only when REGNO(dst) != REGNO(src); the
   in-place `if (dx < 0) { dx = -dx; }` is the two-word form and the `move` the
   target puts in the `bgez` delay slot never appears. The func_800A0134 abs just
   below IS the in-place one-variable form -- that is why it has a `nop` delay
   slot. (`dx < 0 ? -dx : dx` does not fold to ABS_EXPR at all.)

   (3) `s16 first`, `s16 j`, `s16 j2`, `s16 gx2/gy2/gx3/gy3`. Each narrow copy
   survives cse because the modes differ, and each is a real word: `first` is the
   `addu $v0,$s0,$zero` ahead of the loop test, j/j2 the `addu $s0,$s1,$zero` and
   `addu $v1,$s1,$zero` at the loop heads. Narrowing gx2/gy2 and gx3/gy3 took
   359 words / 154 aligned to 360 / 73 -- as s32 they outrank `yy` and it spills.

   (4) ONE `k1` FOR ALL THREE INDEX ROLES: the ring walk's claim slot, loop one's
   direction and loop two's. Separate `k`/`k1`/`k2` are three allocnos whose
   priorities land between the table pointers and `p`, and the whole callee-saved
   file comes out permuted: 73 aligned. Merging k1/k2 is 55; folding the ring
   walk's slot variable in too is 17.

   (5) `u8 *sp = (u8 *)0x1F800000;` A POINTER LOCAL, NOT A CONSTANT ADDRESS.
   Reload rematerialises its REG_EQUIV constant as a bare `lui` at each of the
   four sites, which is what the target has; reaching 0x1F800001 as a constant
   goes through LEGITIMIZE_ADDRESS instead, which builds `PLUS(index, int_reg)`
   and puts the `lui` in the result register: 17 aligned against 12.

   (6) `(D_8006CCD8[j] & 0xFFFF)`, NOT `(u16)D_8006CCD8[j]`. Nine `addu`s come
   out with their operands transposed with the cast and right with the mask --
   combine's "put complex operands first" swaps `PLUS(reg, zero_extend(mem))`
   and the split puts the swap back into the insn stream. 12 aligned to 3.

   (7) THE LAST TWO WORDS ARE OPERAND ORDER INSIDE THE SCRATCHPAD ADDRESS.
   `*(sp + j + 1)` associates as `PLUS(PLUS(sp, j), 1)` and gives
   `addu $v0,$t1,$s0`; `sp[1 + j]` gives the transpose. The claim store is
   `*(u8 *)(k1 | (s32)sp) = 1;` -- an explicit `|` with the INDEX FIRST is the
   only spelling that reaches the target's `or $v1,$s1,$t1`; `sp[k1]`,
   `*(sp + k1)` and `(s32)sp | k1` are all the `addu` with the operands the
   other way round. 3 aligned to 0. */
s16 func_8009B88C(u8 *a, s32 x, s32 y, s16 *d, s16 *e)
{
    u8 *p;
    u8 *h;
    u8 *q;
    u8 *sp;
    u16 m;
    s32 xx;
    s32 yy;
    s16 first;
    s32 dx;
    s32 dy;
    s32 ax;
    s32 ay;
    s32 n;
    s32 gx;
    s32 gy;
    s32 sx;
    s32 sy;
    s16 cnt;
    s32 k1;
    s16 j;
    s16 gx2;
    s16 gy2;
    s32 bx;
    s32 by;
    s16 j2;
    s16 gx3;
    s16 gy3;
    s32 cx;
    s32 cy;

    sp = (u8 *)0x1F800000;
    xx = x;
    yy = y;
    p = a;
    if (p != NULL)
    {
        h = p;
        first = 0;
    }
    else
    {
        h = D_800E3D7C;
        p = *(u8 **)(h + 0x58) + 0x20;
        first = 1;
    }
    func_8003DB4C((int *)sp, 7);
    while ((p = *(u8 **)(p + 0x5C) + 0x20) != h || first)
    {
        first = 0;
        q = *(u8 **)(p - 0x14);
        dx = (s16)xx - q[0x24];
        ax = dx >= 0 ? dx : -dx;
        dy = (s16)yy - q[0x25];
        ay = dy >= 0 ? dy : -dy;
        if (ax < ay)
        {
            ax = ay;
        }
        if (ax < 3)
        {
            if (ax == 0)
            {
                k1 = 0;
            }
            else if (ax == 1)
            {
                k1 = (((func_80065F90(q[0x25] - (s16)yy, q[0x24] - (s16)xx) + 0x100) >> 9) & 7) + 1;
            }
            else
            {
                k1 = (((func_80065F90(q[0x25] - (s16)yy, q[0x24] - (s16)xx) + 0x80) >> 8) & 0xF) + 9;
            }
            n = (s16)func_800A0134(h, p);
            if (n < 0)
            {
                n = -n;
            }
            if (n < 0x40)
            {
                *(u8 *)(k1 | (s32)sp) = 1;
            }
        }
    }
    if (sp[0] == 0)
    {
        gx = (s16)xx;
        gy = (s16)yy;
        if (func_800A0548(gx, gy) == 0)
        {
            if ((s16)func_8009A350(xx - 1, gy, 0, &m) != 0)
            {
                if ((m & 0x8000) == 0)
                {
                    if ((m & 0x400) == 0 || (*(u32 *)(p + 0x1C) & 0x40000) != 0)
                    {
                        sx = gx << 6;
                        sx = (sx + 0x20) & 0xFFE0;
                        sy = gy << 6;
                        sy = (sy + 0x20) & 0xFFE0;
                        if ((s16)func_800BCB04(sx, sy, *(u16 *)(h + 0x88) - 0x20) < 0x200)
                        {
                            *d = xx;
                            *e = yy;
                            return 1;
                        }
                    }
                }
            }
        }
    }
    cnt = 0;
    gx2 = (s16)xx;
    gy2 = (s16)yy;
    k1 = D_8008347E & 7;
    while (cnt < 8)
    {
        j = k1;
        if (*(sp + j + 1) == 0)
        {
            if (func_800A0548((s16)(xx + (D_8006CCD8[j] & 0xFFFF)),
                              (s16)(yy + (D_8006CCE8[j] & 0xFFFF))) == 0)
            {
                if ((s16)func_8009A350(gx2, gy2, j, &m) != 0)
                {
                    if ((m & 0x8000) == 0)
                    {
                        if ((m & 0x400) == 0 || (*(u32 *)(p + 0x1C) & 0x40000) != 0)
                        {
                            bx = (gx2 + D_8006CCD8[j]) << 6;
                            bx = (bx + 0x20) & 0xFFE0;
                            by = (gy2 + D_8006CCE8[j]) << 6;
                            by = (by + 0x20) & 0xFFE0;
                            if ((s16)func_800BCB04(bx, by, *(u16 *)(h + 0x88) - 0x20) < 0x200)
                            {
                                *d = xx + (D_8006CCD8[j] & 0xFFFF);
                                *e = yy + (D_8006CCE8[j] & 0xFFFF);
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        cnt++;
        k1 = (k1 + 1) & 7;
    }
    cnt = 0;
    gx3 = (s16)xx;
    gy3 = (s16)yy;
    k1 = D_8008347E & 0xF;
    while (cnt < 0x10)
    {
        j2 = k1;
        if (*(sp + j2 + 9) == 0)
        {
            if (func_800A0548((s16)(xx + (D_800DCE6C[j2] & 0xFFFF)),
                              (s16)(yy + (D_800DCE8C[j2] & 0xFFFF))) == 0)
            {
                if ((s16)func_8009A350(xx + (D_800DCE6C[j2] & 0xFFFF) - 1,
                                       yy + (D_800DCE8C[j2] & 0xFFFF), 0, &m) != 0)
                {
                    if ((m & 0x8000) == 0)
                    {
                        if ((m & 0x400) == 0 || (*(u32 *)(p + 0x1C) & 0x40000) != 0)
                        {
                            cx = (gx3 + D_800DCE6C[j2]) << 6;
                            cx = (cx + 0x20) & 0xFFE0;
                            cy = (gy3 + D_800DCE8C[j2]) << 6;
                            cy = (cy + 0x20) & 0xFFE0;
                            if ((s16)func_800BCB04(cx, cy, *(u16 *)(h + 0x88) - 0x20) < 0x200)
                            {
                                *d = xx + (D_800DCE6C[j2] & 0xFFFF);
                                *e = yy + (D_800DCE8C[j2] & 0xFFFF);
                                return 1;
                            }
                        }
                    }
                }
            }
        }
        cnt++;
        k1 = (k1 + 1) & 0xF;
    }
    return 0;
}

/* A node's +0x10 step for the damage number: append a 0x10-byte primitive to
   D_80083160.unk_00's +0x8D0 cursor, tint it by the remaining frames, hang it
   and a 0xC-byte tail primitive on the +0xB0 OT row, and retire the node when
   the +0x5 counter reaches zero.

   TWO SPELLINGS ARE LOAD-BEARING. (1) `Unk80083160 *g = &D_80083160;` -- a
   POINTER LOCAL. Reaching the global as `D_80083160.unk_00` at all four sites
   keeps only the `lui` in a callee-saved register and reloads with a `%lo`
   displacement: 83 words of 84 and 36 aligned differences. The pointer local
   is the `addiu $s1, $a0, %lo(D_80083160)` the target computes once.
   (2) THE PRODUCT IS A `u16` LOCAL, not an `s32` one. With `s32 v` the
   function is 84 words and 8 differing: the truncation to u16 is a register
   copy cse cannot see through, and it is the `addu $v1, $a1, $zero` the
   target fills the `beqz` delay slot with, as well as the reason the
   else arm's `sll 16`/`sra 16` reads the untruncated `$a1`. Found by
   decomp-permuter in 76 candidates after eleven hand spellings (chained
   assignment, a separate s32/u8/volatile temp, ascending store order, an
   inverted test, a two-step product and a four-argument signature) all sat
   flat at 8. 84 words, 0 differing. */
void func_8009BE2C(u8 *p, UnkVec *m, u8 *c)
{
    u8 *q;
    u16 v;
    Unk80083160 *g;

    g = &D_80083160;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0x10);
    *(s16 *)(q + 0xC) = 0x140;
    *(s16 *)(q + 0x8) = 0;
    *(s16 *)(q + 0xA) = 0;
    *(s16 *)(q + 0xE) = 0xE0;
    v = (0xC0 / *(s16 *)(c + 0x6)) * *(s8 *)(c + 0x5);
    if (*(s8 *)(c + 0x4) != 0)
    {
        q[0x6] = v;
        q[0x5] = v;
        q[0x4] = v;
    }
    else
    {
        *(s32 *)(q + 0x4) = (s16)v;
    }
    SetTile(q);
    q[0x7] |= 2;
    AddPrim(g->unk_00 + 0xB0, q);
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    q[0x3] = 1;
    *(u32 *)(q + 0x4) = 0xE1000020;
    AddPrim(g->unk_00 + 0xB0, q);
    if (--*(s8 *)(c + 0x5) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn a kind-0x212 node on the engine's own record, park func_8009BE2C at
   its +0x10 and write the two flags into the renderable at +0x0C. The second
   parameter is `s8`: it is stored raw by `sb` at +5 and sign-extended
   (`sll 24`/`sra 24`) for the halfword at +6, which is the narrow type's
   extension sitting at its first int use. The +0x4 store RE-READS the
   renderable rather than reusing the local. 31 words, 0 differing. */
void func_8009BF7C(s16 a, s8 b)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0x212, D_800E3D7C - 0x20);
    if (n != NULL)
    {
        q = (u8 *)n->unk_0C;
        n->unk_10 = (u32)func_8009BE2C;
        q[0x5] = b;
        *(s16 *)(q + 0x6) = b;
        ((u8 *)n->unk_0C)[0x4] = a != 0;
    }
}

/* The counter-attack report. `s16 t = d;` IS THE NARROW COPY -- the test and
   func_8003AD08's argument read `t` while the add and func_800B4C7C's read
   `d`, which is the second callee-saved register the listing spends. One
   variable fuses the two and the function is two words short; a same-width
   `s32 t` is copy-propagated away and measures the same two words short.
   77 words, 0 differing. */
void func_8009BFF8(u8 *p, u8 *q)
{
    void *m;
    void *h;
    s32 d;
    s16 t;
    u8 *e;
    u8 *a;
    u8 *b;
    s32 x;

    e = *(u8 **)(p + 0x50);
    if (e != NULL && e[0] == 0xB && (func_800A6D30() & 3) == 0)
    {
        d = (s16) * (u16 *)(p + 0x64) >> 2;
        t = d;
        *(u16 *)(q + 0x64) += d;
        if (t != 0)
        {
            a = *(u8 **)(p - 0x14);
            b = *(u8 **)(q - 0x14);
            *(u8 **)(q + 0x60) = p;
            *(s16 *)(q + 0x6A) = func_800A0818(a[0x24], a[0x25], b[0x24], b[0x25], &x);
            func_800B4C7C(3, q, -d, 0);
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E0D51, h);
            h = func_8003AD08(-t, h);
            h = func_80099194(D_800E0D6D, h);
            h = func_80099734(q, h);
            h = func_80099194(D_80088CAC, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
}

/* Hand the actor over to the record it is standing on: refuse with a message
   when there is nothing there, build the travel halfword +0x22 and the flag
   drift the same way func_8009CE1C does, lay the result in +0x64, relink the
   two +0x60 slots, publish the node in D_80083470 and fire the 0x601 cue plus
   the func_80099C58 flash. 516 words, 0 differing.

   THE WHOLE FUNCTION IS AN ALLOCATION RACE BETWEEN `h` AND `o`, and nothing
   else in it was ever more than three words. Target order of the five
   callee-saved allocnos is h $s0, o $s1, n $s2, fl $s3, p $s4; the obvious
   spelling gives o $s0, n $s1, fl $s2, p $s3, h $s4 -- 189 aligned
   differences at 515 words, every one of them a register number.
   `allocno_compare`'s `floor_log2(refs) * refs / live_length` from `.lreg`:
   o is 46/385 = 0.597 and h must beat it. `live_length` cannot be moved here
   (h is live across the whole middle on the path that reaches the `r == 0`
   block), so only the REFERENCE COUNT is available, and the floor_log2 step
   at 32 means h needs exactly 32. THREE FREE REFERENCES BUY THEM, and all
   three are byte-identical to the nested form they replace:

   * the refusal path's chain on `h`/`h0` instead of its own local (+6);
   * `h = func_80099734(p, h0); h = func_80099194(D_800E0D92, h);
      h = func_8009929C(0xA, h);` instead of one nested call (+4);
   * the same un-nesting of the D_800E0DB8 pair (+2).

   20 refs -> 32 refs rotates all five registers at once. Measured with
   16 x `__asm__("" ::"r"(h))` first, which reaches the same 0; the three
   un-nestings are the device-free spelling of it.

   THE `i < 0` REFUSAL MUST NEST, NOT RETURN EARLY. `if (i >= 0) { ... }`
   with one `return NULL` at the end of the block is 0; `if (i < 0) return
   NULL;` lets jump.c fold the two `j .L8009C908` / `move $v0,$zero` tails
   together, reorg then cannot steal the move into the `bltz`'s delay slot,
   and it is 3 aligned differences at the same word count.

   `*((s32 *)o + 7) &= 0xDFFFFFFF;` -- the POINTER-ADD form, not
   `*(s32 *)(o + 0x1C)`. The cast form is not MEM_IN_STRUCT_P, so sched
   cannot hoist D_800E3D7C's `lw` over the store and the load-delay nop
   stands: 518 words, 19 differing.

   The 0x20 in the func_80099C58 arguments sits on the `* 0x40` operand
   (`... * 0x40 + 0x20 + D_8006CCD8[j] * 0x20`), the OPPOSITE side from
   func_8009CE1C's call, because `fold`'s additive-constant swap moves it
   across; on the table term it is 6 differing at the same word count. The
   second table index is inlined as `(i1 + 4) & 7` -- as its own local the
   two `sll`/`addu` index pairs come out in the other order, 8 differing.

   func_8003AD08's argument is an if/else over two whole calls, not a
   ternary: the ternary is two words short (no `j`, and the `h` copy is not
   duplicated into both arms). Same for the D_800E0DD0/D_800E0DDD pick,
   which as a ternary builds the `high` in $v0 instead of $a0, 4 differing.

   `(s8)p[0x13] >= 0` and `*(s16 *)(o + 0x64) >= 0` are the `bltz`-to-skip
   tests; `ang` is read as `(u16)ang >> 9` (`lhu` + `srl`). func_800B4C7C,
   func_800B5ED0, func_800C82B8, func_800C7DEC, func_800A2D68 and
   func_8009CD58 are all still on their implicit declarations here -- a
   prototype for func_800B5ED0 would zero-extend its third argument where
   the target sign-extends it. */
u8 *func_8009C12C(u8 *p, u8 *q, s16 ang, s16 k)
{
    u8 *o;
    void *h0;
    u8 *h;
    s16 n;
    s16 r;
    s16 d;
    s32 w;
    s32 e;
    s32 fl;
    s32 j;
    s32 a;
    s32 v;
    s32 t;
    s32 kk;
    s32 i;
    s32 i1;

    p[0x73] = 0;
    p[0x72] = 0;
    o = *(u8 **)(p + 0x60);
    n = 0;
    r = 0;
    if (o == NULL)
    {
        if (p != D_800E3D7C)
        {
            return NULL;
        }
        i = (s16)func_800B5ED0(q[0x24], q[0x25], ang, *(s16 *)(p + 0x88));
        if (i >= 0)
        {
            h0 = func_800990FC();
            h = func_8009929C(8, h0);
            h = func_8009955C(&D_800E3648[i * 4], h);
            h = func_80099194(D_800E0D7B, h);
            func_80099290(h);
            func_800A5720(h0);
            func_800A56E0(0x700);
            func_800A2D68(p, 8);
            func_800B4C7C(0x82, p, 8, 1);
        }
        return NULL;
    }
    h0 = func_800990FC();
    h = h0;
    if ((s8)p[0x13] >= 0)
    {
        if ((*(u16 *)(q + 0x14) & *(u16 *)(*(u8 **)(o - 0x14) + 0x14) & 0x8000) == 0 || (p[0x13] == 0x23 && *(s16 *)(p + 0xA6) != 0 && (*(u16 *)(*(u8 **)(o - 0x14) + 0x14) & 0x8000) == 0))
        {
            h = func_80099734(p, h0);
            h = func_80099194(D_800E0D92, h);
            h = func_8009929C(0xA, h);
        }
        else
        {
            a = *(s32 *)(p + 0x14) & 0x4000;
            if (a == 0)
            {
                r = -1;
            }
            if (a != 0 || (*(s32 *)(o + 0x14) & 0x4000) != 0)
            {
                h = func_80099194(D_800E0D9E, func_80099734(a != 0 ? p : o, h));
                r = 1;
            }
        }
    }
    *(s32 *)(o + 0x14) |= 0x1000000;
    if ((*(s32 *)(p + 0x14) & 0x4000000) != 0)
    {
        func_800B4C7C(3, o, -1, 0);
        if ((s8)p[0x13] >= 0)
        {
            w = r;
            if (w == 0)
            {
                h = func_80099734(o, h);
                h = func_80099194(D_800E0DB8, h);
            }
            if (w < 0)
            {
                return NULL;
            }
            func_80099290(h);
            func_800A5720(h0);
        }
        return NULL;
    }
    if (*(u32 *)(p + 0x4C) != 0 && *(u16 *)*(u32 *)(p + 0x4C) == 0xF0D)
    {
        *(s16 *)(o + 0x22) = o[0x27];
    }
    else
    {
        *(s16 *)(o + 0x22) = o[0x27] + func_80098250(o);
    }
    if ((s16)func_80042900((s8 *)o, 0xA) != 0)
    {
        *(s16 *)(o + 0x22) >>= 1;
    }
    if (*(s16 *)(o + 0x22) == 0)
    {
        (*(s16 *)(o + 0x22))++;
    }
    e = func_8009CD58(p, 7, 1);
    fl = func_8009CD58(o, 7, 0);
    if ((e & 1) != 0)
    {
        if ((s16)func_80042900((s8 *)o, 0x13) != 0)
        {
            *(s16 *)(p + 0x20) >>= 2;
        }
        else
        {
            if ((fl & 4) != 0)
            {
                n++;
            }
            if ((fl & 2) != 0)
            {
                n -= 2;
            }
        }
    }
    if ((e & 2) != 0)
    {
        if ((s16)func_80042900((s8 *)o, 0x14) != 0)
        {
            *(s16 *)(p + 0x20) >>= 2;
        }
        else
        {
            if ((fl & 1) != 0)
            {
                n++;
            }
            if ((fl & 4) != 0)
            {
                n -= 2;
            }
        }
    }
    if ((e & 4) != 0)
    {
        if ((s16)func_80042900((s8 *)o, 0x12) != 0)
        {
            *(s16 *)(p + 0x20) >>= 2;
        }
        else
        {
            if ((fl & 2) != 0)
            {
                n++;
            }
            if ((fl & 1) != 0)
            {
                n -= 2;
            }
        }
    }
    d = *(u16 *)(p + 0x88) - *(u16 *)(o + 0x88);
    if (d >= 0x20)
    {
        n--;
    }
    else if (d < -0x1F)
    {
        n++;
    }
    *(s16 *)(p + 0x20) += (*(s16 *)(p + 0x20) * n) >> 3;
    v = func_800C82B8(p);
    if (v != 0)
    {
        *(s16 *)(p + 0x20) += (*(s16 *)(p + 0x20) >> 3) * v;
    }
    if ((*(s32 *)(p + 0x1C) & 0x1000000) != 0)
    {
        t = (*(s16 *)(o + 0x22) - *(s16 *)(p + 0x20)) / 2;
        *(s16 *)(o + 0x64) = t + (s16)t / 2;
    }
    else
    {
        *(s16 *)(o + 0x64) = (*(s16 *)(o + 0x22) - *(s16 *)(p + 0x20)) / 2;
    }
    if (*(s16 *)(o + 0x64) >= 0)
    {
        *(s16 *)(o + 0x64) = -1;
    }
    j = func_80042900((s8 *)o, 0x1D);
    if (r == 0)
    {
        h = func_80099734(o, h);
        if ((*(s32 *)(p + 0x14) & 0x2000) != 0)
        {
            h = func_80099194(D_800E0DD0, h);
        }
        else
        {
            h = func_80099194(D_800E0DDD, h);
        }
        if ((s16)j != 0)
        {
            h = func_8003AD08(0, h);
        }
        else
        {
            h = func_8003AD08(-*(s16 *)(o + 0x64), h);
        }
        h = func_80099194(D_800E0DEA, h);
    }
    if (r >= 0)
    {
        func_80099290(h);
        func_800A5720(h0);
    }
    if (k == 1)
    {
        func_800C7DEC(p, o);
    }
    *((s32 *)o + 7) &= 0xDFFFFFFF;
    if (o == D_800E3D7C)
    {
        func_8009BFF8(o, p);
    }
    if ((s16)j != 0)
    {
        kk = 0;
    }
    else
    {
        kk = (s16) - *(u16 *)(o + 0x64);
    }
    func_800B4C7C(3, o, kk, 0);
    if (o == D_800E3D7C)
    {
        func_80094E34();
    }
    *(u8 **)(o + 0x60) = p;
    *(u8 **)(p + 0x60) = o;
    D_80083470 = o - 0x20;
    if ((*(s32 *)(p + 0x1C) & 0x1000000) != 0)
    {
        func_800A56E0(0x700);
        func_8009BF7C(*(u16 *)(p + 0x14) & 0x2000, 4);
    }
    func_800A56E0(0x601);
    if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
    {
        func_800419EC(8, 0x10);
    }
    if (p == D_800E3D7C && p[0xA8] != 0)
    {
        return o;
    }
    i1 = ((u16)ang >> 9) & 7;
    func_80099C58((s16)((q[0x24] + D_8006CCD8[i1] * k) * 0x40 + 0x20 + D_8006CCD8[(i1 + 4) & 7] * 0x20),
                  (s16)((q[0x25] + D_8006CCE8[i1] * k) * 0x40 + 0x20 + D_8006CCE8[(i1 + 4) & 7] * 0x20),
                  (s16)(*(u16 *)(*(u8 **)(p + 0x60) + 0x88) - 0x30), e, *(s16 *)(p + 0x2A));
    return o;
}

u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e)
{
    s16 k;
    s32 i;
    s32 sh;
    s32 cc2;
    s32 r;
    s32 r1;
    s32 t;
    s32 t3;
    s32 t2;
    s32 m;
    register u8 *zz __asm__("$4");
    u8 *g;
    u8 *p;
    u16 w;

    w = ang;
    if (e == NULL)
    {
        if ((*(s32 *)(a + 0x1C) & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(a + 0x14) >= 0)
            {
                do
                {
                    *(s32 *)(a + 0x14) |= cc2;
                } while (0);
                w = ang + ((func_800A6D30() & 7) << 9);
                *(s16 *)(a + 0x2A) = w;
            }
        }
    }
    sh = w >> 8;
    p = (u8 *)D_800DCEAC;
    i = sh & 0xE;
    func_800C78A0(a - 0x20, b[0x24] * 64 + (*(s16 *)(p + i) >> 1) + 0x20,
                  b[0x25] * 64 + (*(s16 *)((u8 *)D_800DCEBC + i) >> 1) + 0x20,
                  *(s16 *)(a + 0x88), 8, 0x300);
    *(u8 **)(a + 0x60) = e;
    *(s32 *)(a + 0x14) &= ~0x4000000;
    if (e == NULL)
    {
        if (func_8009A540((*(u16 *)(a + 0x2A) >> 9) & 7, b[0x24], b[0x25],
                          *(u16 *)(a + 0x88) - 0x20) == 0)
        {
            return NULL;
        }
        e = func_8009B25C(a, b[0x24] + *(u16 *)((u8 *)D_8006CCD8 + i) * dist,
                          b[0x25] + *(u16 *)((u8 *)D_8006CCE8 + i) * dist,
                          *(s16 *)(a + 0x88));
        *(u8 **)(a + 0x60) = e;
        if (e == NULL)
        {
            return NULL;
        }
    }
    *(u16 *)(e + 0x6A) = *(u16 *)(a + 0x2A);
    if (e[0x13] != 0)
    {
        if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && (*(u16 *)(e + 0x46) & 0x8000) == 0)
        {
            *(u16 *)(e + 0x2A) = *(u16 *)(a + 0x2A) + 0x800;
        }
    }
    *(s32 *)(a + 0x14) |= 0x4000000;
    *(s32 *)(e + 0x14) = (*(s32 *)(e + 0x14) | 0x20000) & ~0x1000000;
    k = a[0x84];
    t = k;
    __asm__("" ::"r"(k));
    r1 = func_800A6D30();
    __asm__("" ::"r"(r1));
    k = t;
    if (a[3] != 0)
    {
        k = t + (r1 % a[3]) * 4;
    }
    if (e[0x13] == 0 && *(u8 **)(e + 0x50) != NULL && **(u8 **)(e + 0x50) == 2)
    {
        k -= k >> 2;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) == 0 && k < 0x80)
    {
        return e;
    }
    k = func_800A6D30();
    zz = a;
    __asm__("" : "=r"(zz) : "0"(zz));
    t2 = (k & 1) + ((m = k & 2) != 0);
    t2 += (a[0x26] + func_800981F8(zz)) * 2;
    *(s16 *)(a + 0x20) = t2;
    if ((*(s32 *)(a + 0x1C) & 0x1000) != 0)
    {
        k = a[3] >> 1;
    }
    else
    {
        k = a[3] >> 6;
    }
    t3 = k;
    if (t3 >= (s32)(func_800A6D30() & 0x7F))
    {
        *(s32 *)(a + 0x1C) |= 0x1000000;
    }
    else
    {
        *(s32 *)(a + 0x1C) &= ~0x1000000;
    }
    if ((*(s32 *)(e + 0x1C) & 0x238) != 0)
    {
        r = func_800A6D30();
        if (e[3] != 0)
        {
            k = r % e[3];
        }
        else
        {
            k = 0;
        }
        if ((s16)(k + e[0x70]) >> 7 > 0)
        {
            return e;
        }
    }
    *(s32 *)(a + 0x14) &= ~0x4000000;
    g = D_800E3D7C;
    *(s32 *)(e + 0x14) &= ~0x20000;
    if (a == g)
    {
        func_8003E188(e[0x13], 0);
    }
    __asm__("" ::"r"(k));
    return e;
}

/* Mask the renderable's +0x14 flags with the caller's and add one more bit
   when the actor's held record names the right kind. The nine
   `r = t | (b & K)` statements cross-jump onto one `or`.

   TWO DEVICES, STACKED ON THE SAME COPY, and neither emits an instruction.
   `return r;` alone is 48 words -- one short -- because cse's
   merge_equiv_classes makes `r` canonical (it outlives `t`), so the `r = t`
   copy coalesces and the `and` writes straight into the result register.
   Returning THROUGH `t` gives `t` the later last mention and the copy
   survives: 49 words, but 4 differing, all of them $a3 against $t0, because
   `t` then outranks `r` on global-alloc's `floor_log2(refs)*refs/live_length`.
   Four free references on `r` via `+ ((r + r) - r - r)` flip the order back.
   Measured inert: swapping the two declarations, an `__asm__("" ::"r"(r))`
   reference, and writing the mask first in the `or`. 49 words,
   0 differing. */
s32 func_8009CD58(u8 *a, s32 b, s16 c)
{
    u8 *q;
    s32 t;
    s32 r;

    t = *(u16 *)(a + 0x14) & b;
    r = t;
    if (c != 0)
    {
        q = *(u8 **)(a + 0x4C);
        if (q != NULL)
        {
            if (q[1] == 0xF)
            {
                if (q[0] == 5)
                {
                    r = t | (b & 1);
                }
                else if (q[0] == 6)
                {
                    r = t | (b & 2);
                }
                else if (q[0] == 7)
                {
                    r = t | (b & 4);
                }
            }
            else
            {
                if (q[0] == 6)
                {
                    r = t | (b & 1);
                }
                else if (q[0] == 7)
                {
                    r = t | (b & 2);
                }
                else if (q[0] == 8)
                {
                    r = t | (b & 4);
                }
            }
        }
    }
    else
    {
        q = *(u8 **)(a + 0x50);
        if (q != NULL)
        {
            if (q[0] == 8)
            {
                r = t | (b & 1);
            }
            else if (q[0] == 9)
            {
                r = t | (b & 2);
            }
            else if (q[0] == 0xA)
            {
                r = t | (b & 4);
            }
        }
    }
    /* DEVICE: the return-through and the phantom zero; see the header. */
    t = r + ((r + r) - r - r);
    return t;
}

/* Open a door/lift: refuse if func_8009D218 does, set the actor's travel
   halfword +0x22 from func_80098250 and the three D_8006CCD8 facing bits,
   accumulate a per-flag drift in `n`, lay the result in +0x64, raise
   0x20000000 on the view, publish the node in D_80083470 and fire the
   0x601/0x602 cue plus the func_80099C58 flash. 255 words, 0 differing.

   SEVEN PARAMETERS; the last three arrive on the stack at 0x50/0x54/0x58.

   `w` IS AN `s16` LOCAL SEEDED FROM `c`, and the `move $v0, $s2` before the
   `slti` is what that costs: an HImode pseudo's int use is a copy here, not
   a `sll 16`/`sra 16` pair. `s32 w` deletes that word.

   THE DRIFT CORRECTION IS NOT A DIVISION. `s16 t = m * n;` then
   `if (t < 0) { t = (t + 3) >> 2; }` is the target: the HImode destination
   makes the shift `srl` and leaves it INSIDE the arm. Written `t = t / 4`
   gcc emits the branchy sdiv_pow2 form, whose `sra` sits AFTER the label --
   one word short and a different shape (measured on a one-function probe).

   `if (*(s16 *)(p + 0x64) >= 0) ... = -1;` -- the test is `>= 0`, which is
   the `bltz`-to-skip the target has; `< 0` gives `bgez` and 3 differing.

   THE CUE ID NEEDS A LOCAL: `snd = 0x601; if (q != 0 && (q & 0x20000000) == 0)
   { snd = 0x602; }`. The ternary `(q == 0 || ...) ? 0x601 : 0x602` puts 0x602
   in the second branch's delay slot and is ONE WORD SHORT at 65 differing.

   THE 0x20 GOES ON THE `r[0x24] * 0x40` OPERAND, not on the table term:
   `fold`'s additive-constant swap is an involution and moves it across, so
   `(D_8006CCD8[i] * 0x20 + 0x20)` is the wrong side -- same word count, 19
   differing.

   func_800B4C7C'S THIRD PARAMETER IS `s32` AND THE `(s16)` NARROWING BELONGS
   IN THE ARM. With `s16 k` the sign extension lands after the join, where the
   target has `sra $a2, $v0, 16` inside the else arm: 255 words, 6 differing.

   `*(u32 *)(p + 0x60) = q;` comes BEFORE `D_80083470 = p - 0x20;`. The other
   order lets the +0x60 store fill the `lw -0x14($s0)` load delay and the
   function is 2 words short.

   func_80099C58 TAKES FIVE ARGUMENTS, not the four its landed definition
   declares -- `sw $t0, 0x10($sp)` is the angle. The fifth is unread there, so
   it is unwitnessed in the callee and func_80099C58 stays 147/0 with it
   added. */
void func_8009CE1C(u8 *p, s16 b, s16 c, s16 e, s16 ang, u32 q, s16 g2)
{
    u8 *r;
    s32 fl;
    s16 w;
    s16 n;
    s32 snd;
    s32 k;
    s16 t;
    s32 m;
    s32 i;

    w = c;
    n = 0;
    if (p == NULL)
    {
        return;
    }
    if (func_8009D218(p, (u16)e, (u8 *)q) != 0)
    {
        return;
    }
    if (c == 0xFF)
    {
        w = D_800E3D7C[0x11] * 4;
        if (w >= 0x64)
        {
            w = 0x63;
        }
    }
    m = (b + w) * 2;
    *(s16 *)(p + 0x6A) = ang;
    if (p[0x13] != 0 && (*(s32 *)(p + 0x1C) & 0x238) == 0 && (*(u16 *)(p + 0x46) & 0x8000) == 0)
    {
        *(s16 *)(p + 0x2A) = ang + 0x800;
    }
    *(s16 *)(p + 0x22) = p[0x27] + func_80098250(p);
    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        *(s16 *)(p + 0x22) >>= 1;
    }
    if (*(s16 *)(p + 0x22) == 0)
    {
        (*(s16 *)(p + 0x22))++;
    }
    fl = func_8009CD58(p, 7, 0);
    if ((e & 1) != 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x13) != 0)
        {
            m >>= 2;
        }
        else
        {
            if ((fl & 4) != 0)
            {
                n++;
            }
            if ((fl & 2) != 0)
            {
                n--;
            }
        }
    }
    if ((e & 2) != 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x14) != 0)
        {
            m >>= 2;
        }
        else
        {
            if ((fl & 1) != 0)
            {
                n++;
            }
            if ((fl & 4) != 0)
            {
                n--;
            }
        }
    }
    if ((e & 4) != 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x12) != 0)
        {
            m >>= 2;
        }
        else
        {
            if ((fl & 2) != 0)
            {
                n++;
            }
            if ((fl & 1) != 0)
            {
                n--;
            }
        }
    }
    t = m * n;
    if (t < 0)
    {
        t = (t + 3) >> 2;
    }
    m += t;
    *(s16 *)(p + 0x64) = (*(s16 *)(p + 0x22) - m) / 2;
    if (*(s16 *)(p + 0x64) >= 0)
    {
        *(s16 *)(p + 0x64) = -1;
    }
    *(s32 *)(p + 0x1C) |= 0x20000000;
    if ((s16)func_80042900((s8 *)p, 0x1D) != 0)
    {
        k = 0;
    }
    else
    {
        k = (s16) - *(u16 *)(p + 0x64);
    }
    func_800B4C7C(3, p, k, 0);
    if (p == D_800E3D7C)
    {
        D_800E3E41 = g2;
        func_80094E34();
    }
    *(u32 *)(p + 0x60) = q;
    D_80083470 = p - 0x20;
    if ((*(u16 *)(*(u8 **)(p - 0x14) + 0x14) & 0x8000) == 0)
    {
        func_800419EC(8, 0x10);
    }
    snd = 0x601;
    if (q != 0 && (q & 0x20000000) == 0)
    {
        snd = 0x602;
    }
    func_800A56E0(snd);
    r = *(u8 **)(p - 0x14);
    i = (*(u16 *)(p + 0x2A) >> 9) & 7;
    func_80099C58((s16)(r[0x24] * 0x40 + 0x20 + D_8006CCD8[i] * 0x20),
                  (s16)(r[0x25] * 0x40 + 0x20 + D_8006CCE8[i] * 0x20),
                  (s16)(*(u16 *)(p + 0x88) - 0x30), e, ang);
}

/* Should the actor react to `q`? Three mutually exclusive request bits pick
   which pair of func_80042900 probes to run, and a common fallback gate
   follows them.

   THE HIT ACTION IS WRITTEN OUT IN FULL IN ALL FOUR ARMS -- a shared
   `goto hit` collapses to 74 words / 79 differing, because cross-jumping
   then folds the three arms' `lb 0x13` / `bltz` / `j` tails AND their first
   `func_80042900(p, 0x16)` calls into one. Duplicating the tail in the
   source is what keeps them apart (74 -> 91 words).
   AND THE TWO ENTRY TESTS ARE ONE `&&` AROUND THE WHOLE BODY: as two
   separate `return 0`s they are 91 words / 81 differing, with the `f & 1`
   test displaced out of the `beqz $s1` delay slot by a `move $v0, $zero`
   stolen from the return-0 block; as one `||` early return, 90 / 21.
   90 words, 0 differing. */
s32 func_8009D218(u8 *p, s32 f, u8 *q)
{
    if ((f & 8) == 0 && q != NULL)
    {
        if ((f & 1) != 0)
        {
            if ((s16)func_80042900((s8 *)p, 0x16) != 0)
            {
                if (*(s8 *)(q + 0x13) >= 0)
                {
                    func_800CB82C(*(void **)(p - 0x18), p, q);
                    return 1;
                }
            }
        }
        else if ((f & 2) != 0)
        {
            if ((s16)func_80042900((s8 *)p, 0x16) != 0 || (s16)func_80042900((s8 *)p, 0x17) != 0)
            {
                if (*(s8 *)(q + 0x13) >= 0)
                {
                    func_800CB82C(*(void **)(p - 0x18), p, q);
                    return 1;
                }
            }
        }
        else if ((f & 4) != 0)
        {
            if ((s16)func_80042900((s8 *)p, 0x16) != 0 || (s16)func_80042900((s8 *)p, 0x15) != 0)
            {
                if (*(s8 *)(q + 0x13) >= 0)
                {
                    func_800CB82C(*(void **)(p - 0x18), p, q);
                    return 1;
                }
            }
        }
        if (q != NULL && *(u8 **)(p + 0x50) != NULL && **(u8 **)(p + 0x50) == 3 &&
            *(s8 *)(q + 0x13) >= 0 && (func_800A6D30() & 3) != 0)
        {
            func_800CB82C(*(void **)(p - 0x18), p, q);
            return 1;
        }
    }
    return 0;
}

/* 12 words, 0 differing. */
void func_8009D380(void)
{
    func_8003DB4C((int *)D_800E50A8, 0x200);
    func_8009D3B0();
}

/* Repaint the packed 4-bit minimap at D_800E50A8 and hand it to the engine's
   LoadImage. D_80082E80[0x26] is the party's room index: when it is >= 0 the
   whole D_800E2970 room rectangle (grown by one cell on every side) is
   repainted, otherwise only the eight neighbours of the party tile at
   D_80082E80[0x24]/[0x25]. Each painted cell's nibble -- even x low, odd x
   high -- takes the cell's +0x02 height biased by 0x200 and divided by 0x40,
   clamped to 1..0xF, exactly as func_8009D8A4 does it. 209 words, 0 differing.

   THE TWO ARMS SHARE `j` AND `jEnd`. The second arm's x coordinate is the
   same variable as the first arm's inner loop counter, and its direction
   counter is the same variable as the first arm's inner bound. That is not
   cosmetic: with separate `x` and `d` locals the function is 209 words and
   18 differing, every one of them a register. Merging doubles those two
   allocnos' reference counts, which lifts them over the loop's
   sign-extension temporaries in allocno_compare, and the whole file falls
   into place -- `j` moves from $a3 to $a1, `jEnd` from $t8 to $a3, `m` from
   $t9 to $t8, and the cell pointer stops needing a register of its own and
   shares $a2 with the map pointer.

   D_80082E80 IS A BASE LOCAL FOR THE FLAG AND A SECOND LOCAL IN THE LOOP.
   `c` carries the address across the branch so the flag is `lb 0x26($v0)`;
   the second arm then takes its own `c2 = D_80082E80;` INSIDE the loop,
   which loop-invariant motion turns into the target's `addu $t0, $v0,
   $zero`. One shared local is 208 words -- that copy is the missing word.
   Reading the flag as `*(s8 *)&D_80082E80[0x26]` instead makes the shared
   value `&D_80082E80[0x26]` and the hoisted copy an `addiu $t0, $v0, -38`:
   22 differing. `c2 = D_80082E80;` written above the `for` is emitted ahead
   of `jEnd = 7`: 2 differing.

   `p = &D_800E50A8[k / 2];` MUST SIT ABOVE THE `/ 64` STATEMENT. The signed
   halving is three words that have to land in the block the division's
   `bgez` ends; reading +0x02 into a temporary first, or splitting `k / 2`
   into its own local, pushes them past it and costs 31 differing.

   `m = &g[0x1DC]` off a base local keeps the three-word lui/addiu/addiu the
   target has, the same spelling func_8009D8A4 records. */
void func_8009D3B0(void)
{
    UnkDungeonRect r;
    u8 *c;
    u8 *c2;
    u8 *g;
    u8 *m;
    u8 *p;
    UnkDungeonRoom *e;
    s32 nx;
    u8 b;
    s32 k;
    s32 t;
    s16 i;
    s16 j;
    s16 iEnd;
    s16 jEnd;
    s16 n;
    s16 y;

    r = D_80088CB0;
    g = (u8 *)&D_80083160;
    m = &g[0x1DC];
    c = D_80082E80;
    t = *(s8 *)(c + 0x26);
    if (t >= 0)
    {
        e = &D_800E2970_s[t];
        i = e->unk_02 - 1;
        jEnd = e->unk_00 + e->unk_04 + 1;
        iEnd = e->unk_06 + i + 2;
        for (; i < iEnd; i++)
        {
            for (j = e->unk_00 - 1; j < jEnd; j++)
            {
                k = (i << *(s16 *)(m + 0x14)) + j;
                if (D_800EA000_s[k].unk_00 != 0 && D_800EA000_s[k].unk_00 != 3)
                {
                    p = &D_800E50A8[k / 2];
                    n = (s16)(D_800EA000_s[k].unk_02 + 0x200) / 64;
                    if (n >= 0x10)
                    {
                        n = 0xF;
                    }
                    else if (n <= 0)
                    {
                        n = 1;
                    }
                    nx = n;
                    b = p[0];
                    if (j & 1)
                    {
                        p[0] = b | (nx << 4);
                    }
                    else
                    {
                        p[0] = b | nx;
                    }
                }
            }
        }
    }
    else
    {
        for (jEnd = 7; jEnd >= 0; jEnd--)
        {
            c2 = D_80082E80;
            j = c2[0x24] + (u16)D_8006CCD8[jEnd];
            y = c2[0x25] + (u16)D_8006CCE8[jEnd];
            k = (y << *(s16 *)(m + 0x14)) + j;
            if (D_800EA000_s[k].unk_00 != 0 && D_800EA000_s[k].unk_00 != 3)
            {
                p = &D_800E50A8[k / 2];
                n = (s16)(D_800EA000_s[k].unk_02 + 0x200) / 64;
                if (n >= 0x10)
                {
                    n = 0xF;
                }
                else if (n <= 0)
                {
                    n = 1;
                }
                nx = n;
                b = p[0];
                if (j & 1)
                {
                    p[0] = b | (nx << 4);
                }
                else
                {
                    p[0] = b | nx;
                }
            }
        }
    }
    LoadImage(r.v, D_800E50A8);
}

/* Rebuild the packed 4-bit minimap at D_800E50A8 from the height map and hand
   it to the engine's LoadImage. Each cell's nibble -- even x low, odd x high --
   is cleared, and if it was set it is replaced by the cell's +0x02 height
   biased by 0x200 and divided by 0x40, clamped to 1..0xF.

   THE D_80083160 BASE IS A LOCAL OF ITS OWN, the same shape func_800C3D3C
   records: `w = (u8 *)&D_80083160 + 0x1DC` in one go folds the displacement
   into the relocation and the `addiu` disappears.

   D_800E50A8 MUST BE ONE LOCAL FEEDING BOTH THE WALK AND THE CALL. Written
   `p = D_800E50A8` with `D_800E50A8` again at the call, gcc rematerialises the
   symbol in the epilogue -- 105 words of 108, and the `addu $a3, $a1, $zero`
   the target copies the cursor with is gone.

   THE CLAMP IS AN s16 AND ITS WIDENING IS ITS OWN STATEMENT. With `s16 c` used
   directly in both arms no sign extension is emitted at all (106 words, 42
   differing); with a separate `s32 d` for the quotient the divide keeps an
   extra pseudo (104 / 53). `g = c;` ahead of the byte read is the one spelling
   that puts the single `sll`/`sra` pair at the join, where reorg then copies
   the `sll` into the `bgtz`'s delay slot. Putting it AFTER `e = *p` is 43
   differing.

   AND `g` MUST BE DECLARED BEFORE `e`: the two tie in priority and allocate in
   declaration order, so `e` first gives them each other's registers -- 108
   words, 5 differing, every one of them a register. 108 words, 0 differing. */
void func_8009D6F4(void)
{
    UnkDungeonRect r = D_80088CB0;
    u8 *v;
    u8 *w;
    u8 *p;
    u8 *q;
    u8 b;
    u8 n;
    s32 g;
    u8 e;
    s16 c;
    s16 i;
    s16 j;

    b = 0;
    q = D_800E50A8;
    p = q;
    v = (u8 *)&D_80083160;
    w = v + 0x1DC;
    for (i = 0; i < (1 << *(s16 *)(w + 0x16)); i++)
    {
        for (j = 0; j < (1 << *(s16 *)(w + 0x14)); j++)
        {
            if (j & 1)
            {
                n = b & 0xF0;
            }
            else
            {
                b = *p;
                *p = 0;
                n = b & 0xF;
            }
            if (n != 0)
            {
                c = (s16)(D_800EA000_s[(i << *(s16 *)(w + 0x14)) + j].unk_02 + 0x200) / 64;
                if (c >= 0x10)
                {
                    c = 0xF;
                }
                else if (c <= 0)
                {
                    c = 1;
                }
                g = c;
                e = *p;
                if (j & 1)
                {
                    *p = e | (g << 4);
                }
                else
                {
                    *p = e | g;
                }
            }
            if (j & 1)
            {
                p++;
            }
        }
    }
    LoadImage(r.v, q);
}

/* Repack the floor's tile types into the two-nibble-per-cell map at
   D_800E50A8, then blit the 0x10 x 0x40 tile sheet. 115 words, 0 differing.

   FIVE SPELLINGS ARE LOAD-BEARING, each measured against the whole function.
   (1) THE MAP BASE IS ITS OWN LOCAL, COPIED INTO THE CURSOR. The target
   materialises D_800E50A8 once (words 12-13) and copies it into the cursor
   (`addu $a3, $a1, $zero`), keeping $a1 live across both loops for the
   LoadImage argument. Writing `p = D_800E50A8;` and passing
   `D_800E50A8` again at the call rematerialises it after the loops --
   cse restarts at the join -- which is 118 words.
   (2) `m = &g[0x1DC]` OFF A BASE LOCAL, NOT `(u8 *)&D_80083160 + 0x1DC`.
   The second form folds 0x1DC into the relocation and the `addiu` is gone:
   one word short. `g = ...; m = g + 0x1DC;` also folds, because `g` is
   single-use; the subscript form keeps the three-word
   lui/addiu/addiu the target has (docs/matching-levers.md :22941).
   (3) `if (type != 0 && type != 3) {paint} else {clear}`, not
   `if (type == 0 || type == 3) {clear} else {paint}`. The `&&` form emits
   both tests as jumps to the ELSE arm and lays the paint arm inline, which
   is the target's layout; the `||` form inverts the second test and swaps
   the two arms' positions.
   (4) THE CLAMPED NIBBLE NEEDS AN s32 COPY OF THE s16. `n` is `s16`, so the
   clamp's three assignments are HImode moves and nothing extends; the
   target's `sll 16`/`sra 16` at the arms' join is the conversion of that
   HImode value to int, and it only appears if the source asks for it.
   Without `nx` the function is 113 words; `n * 16` instead of `n << 4`
   gets an extension folded into the shift (`sll 16`/`sra 12`) and is 114.
   (5) THE INDEX SHIFT READS THE FIELD AS `s16`, LIKE THE BOUND. With
   `*(u16 *)(m + 0x14)` there the `lhu` is emitted inside the loop body
   instead of beside the bound's `lh` in the test block: 115 words,
   68 differing. The same field is read `lh` for the bound and `lh` for the
   index; the target's `lhu` at words 28/92 is cc1psx's own narrowing of
   the shift count, not a second declaration.

   DECLARATION ORDER IS WORTH THE LAST FIVE WORDS: `s32 nx;` must be
   declared BEFORE the two byte temporaries. With it after `s16 n`, the
   extension's result and the loaded byte come out in each other's
   registers -- 115/115, 5 differing, every one of them that pair.
   The two byte temporaries are also SEPARATE variables (`c` and `c2`):
   one shared temporary across both arms is 8 differing, because it becomes
   one quantity and the `lbu` is then scheduled ahead of the extension. */
void func_8009D8A4(void)
{
    UnkDungeonRect r;
    u8 *g;
    u8 *m;
    s32 nx;
    u8 c;
    u8 c2;
    u8 *p;
    u8 *q;
    s16 i;
    s16 j;
    s16 n;
    s32 k;

    r = D_80088CB0;
    q = D_800E50A8;
    p = q;
    g = (u8 *)&D_80083160;
    m = &g[0x1DC];
    for (i = 0; i < (1 << *(s16 *)(m + 0x16)); i++)
    {
        for (j = 0; j < (1 << *(s16 *)(m + 0x14)); j++)
        {
            k = (i << *(s16 *)(m + 0x14)) + j;
            if (D_800EA000_s[k].unk_00 != 0 && D_800EA000_s[k].unk_00 != 3)
            {
                n = (s16)(D_800EA000_s[k].unk_02 + 0x200) / 64;
                if (n >= 0x10)
                {
                    n = 0xF;
                }
                else if (n <= 0)
                {
                    n = 1;
                }
                nx = n;
                c = p[0];
                if (j & 1)
                {
                    p[0] = c | (nx << 4);
                }
                else
                {
                    p[0] = c | nx;
                }
            }
            else
            {
                c2 = p[0];
                if (j & 1)
                {
                    p[0] = c2 & 0xF;
                }
                else
                {
                    p[0] = c2 & 0xF0;
                }
            }
            if (j & 1)
            {
                p++;
            }
        }
    }
    LoadImage(r.v, q);
    func_8009BF7C(1, 0x20);
}

/* Append a 0xC-byte primitive to the list at D_80083160's +0x8D0 and hand it
   to AddPrim.

   THE SECOND PARAMETER IS `s32` WITH TWO `(u16)` CASTS, not a `u16`. A u16
   parameter arrives already zero-extended, so the callee emits no `andi` and
   the two biased values are computed off a plain parameter copy -- 27 words,
   25 differing. The casts produce the target's one shared
   `andi $t1, $a1, 0xFFFF`, and the ONE-ARMED `if` is what puts the +0xC0 form
   above the branch and the -0x20 form in its shadow; a ternary gives the
   two-arm `bne`/`j` pair instead. 26 words, 0 differing. */
void func_8009DA70(s32 x, s32 y, s32 *src, s32 tag)
{
    u8 *b;
    u8 *q;
    s32 t;

    b = *(u8 **)&D_80083160;
    q = *(u8 **)(b + 0x8D0);
    *(u8 **)(b + 0x8D0) = q + 0xC;
    q[0x3] = 2;
    *(s16 *)(q + 0x8) = x + 0x300;
    t = (u16)y + 0xC0;
    if (b != D_801C9E40)
    {
        t = (u16)y - 0x20;
    }
    *(s16 *)(q + 0xA) = t;
    *(s32 *)(q + 0x4) = *src;
    AddPrim(tag, q);
}

/* The dungeon minimap/radar draw.  269 words, 0 differing.

   Walks the actor ring plus three static tables and calls func_8009DA70 once
   per blip with a four-byte colour in `col'.

   FIVE THINGS GOT IT HERE, and the last one is a scaffold.

   1. THE `s16 func_8009FD40(u8 *, u8 *);' PROTOTYPE, added to the unit's
      declaration block above.  func_8009FD40 is defined BELOW this function,
      so without it the call rides an implicit `int f()' and loses the
      `sll 16' / `sra 16' pair in front of the `slti'.  Exactly two words
      (265 -> 267).

   2. `mode = 0;' IMMEDIATELY ABOVE `k = 0;' -- a DEAD earlier set, and it
      costs no word because flow deletes it.  +1 word, -9 rows.
      cse.c's make_regs_eqv promotes the NEW reg of `(set mode k)' to
      qty_first_reg only when

          (uid_cuid[regno_last_uid[new]]  > cse_basic_block_end
        || uid_cuid[regno_first_uid[new]] < cse_basic_block_start)
        && uid_cuid[regno_last_uid[new]]  > uid_cuid[regno_last_uid[firstr]]

      With `mode' first touched inside the same cse2 block as the copy, both
      clauses of the first test fail, `k' stays canonical, every use of
      `mode' is rewritten to `k', the copy goes dead and flow deletes it --
      traced: `(set (reg/v:SI 91) (reg/v:SI 90))' present in .cse2 and .bp,
      gone in .flow.  A dead `mode = 0;' in an EARLIER cse2 block drags
      regno_first_uid[mode] below cse_basic_block_start, the second clause
      fires, `mode' becomes canonical and the copy survives.  It brings in
      BOTH 0x8009DCE0 (reorg's duplicate of the copy into the delay slot)
      and 0x8009DCE8.  Four placements are byte-identical: above `k = 0;',
      first statement in the loop body, before the `for', and above
      `q = *(u8 **)(p - 0x14);'.

   3. THE COMMA EXPRESSION in the 0x26 test:

          if (*(s8 *)(q + 0x26) < 0
              || (m = D_80082E80, *(s8 *)(q + 0x26) != *(s8 *)(m + 0x26)))

      +1 word, -16 rows.  The target keeps TWO pseudos for &D_80082E80 --
      $s2 before the loop, which dies so $s2 is reused for `q', and $fp
      across the loop body for the single `lb $v0, 0x26($fp)' -- and the
      preheader copy `addu $fp, $s2, $zero' is loop.c hoisting the in-loop
      materialisation, which cse2 then collapses from lui+addiu to one copy.
      Three conditions have to hold at once: loop.c scan_loop's gate
      (loop.c:794) needs the set to be `m''s FIRST uid with its last use in
      the SAME basic block, since `m' is a user variable so the
      `! REG_USERVAR_P' arm never applies; move_movables' desirability
      (loop.c:1727) needs a lifetime bigger than 1, which an address
      materialised at its point of use does not have (`move-insn savings 1
      not desirable'); and cse1 must not substitute the hoisted value into
      func_8009FD40's second argument -- it will, because -fcse-skip-blocks
      makes cse_end_of_basic_block follow the `||''s `bltz' over the skipped
      block, and invalidate_skipped_block only kills regs SET in the SKIPPED
      block.  A set INSIDE that skipped block -- which is exactly the `||''s
      right operand -- satisfies all three.

   4. THE RING WALK IS A `for' WITH THE ADVANCE IN THE THIRD CLAUSE, `g' is a
      local of its own assigned FIRST, `head = D_800E3D7C;' is read into a
      local BEFORE the five flag gates, and `col[1] = 0;' sits AFTER
      `p = *(u8 **)(head + 0x5C) + 0x20;'.  All four measured; `while' and
      `for(;;)' each cost 2 rows in the `continue' branch targets, swapping
      `g' and `head' costs 9, and moving `head' later costs 18-21.

   5. THE LAST NINE ROWS WERE AN ALLOCATION PRIORITY AND THE CURE IS A
      DUPLICATED-ARM `if (p)' AROUND THE SECOND `col' RESET.  Found by
      decomp-permuter in one minute after thirty-odd hand spellings had left
      the park at nine rows; the search's own answer also carried a
      `do { } while (0);' in the third flag gate, which re-measures as
      unnecessary (that one alone is still 9 rows).

      The residue it removes was `head' in $s6 where the target has $s5 and
      the hoisted D_800E296C base in $s5 where the target has $s6 -- nine
      rows, every opcode, immediate and displacement already identical.
      global.c's allocno_compare sorts by
      floor_log2(n_refs) * n_refs / live_length, and off this body's .lreg:

          head                 refs 5, live_length 112, pri 893
          D_800E296C base copy refs 5, live_length 108, pri 926
          the constant 2       refs 5, live_length 102, pri 980
          &D_80082E80 high     refs 3, live_length  32, pri 937

      which allocates 195, 156, 165, 87 -> $s4, $s4, $s5, $s6 where the
      target needs 195, 156, 87, 165.  Both counts are depth-weighted
      (flow.c:2128), and the base copy's live range is a strict SUBSET of
      head's, so with equal ref counts it always wins -- which is why no
      statement order could move it.  `if (p)' with identical arms does.

      THIS IS A SCAFFOLD, not a reconstruction of the original source: the
      arms are the same two stores and jump2 merges them.  `if (q)' in its
      place measures identically at 0.  A plausible-looking natural shape --
      the call duplicated into both arms of `if (mode == 2)' -- is 267 words
      / 66 rows, and a `goto skip' past the two stores is 269 / 9.

      DO NOT "simplify" the duplicated arms away.

   Established and not to be re-derived: the second parameter is UNUSED, $a1
   is never read, but func_8009DF0C (just below) calls `func_8009DAD8(r, p)'
   with two arguments, so the definition must take two. */
void func_8009DAD8(u8 *ot, u8 *unused)
{
    u8 col[4];
    u8 *g;
    u8 *n;
    u8 *m;
    u8 *p;
    u8 *q;
    u8 *head;
    s32 c;
    s32 i;
    s32 t;
    s32 k;
    s32 mode;

    g = (u8 *)&D_80083160;
    head = D_800E3D7C;
    if ((D_800E296C & 0x1000) != 0)
    {
        D_800E296C &= ~0x1000;
        func_8009F13C();
    }
    if ((D_800E296C & 0x400) != 0)
    {
        D_800E296C &= ~0x400;
        func_8009F0B4();
    }
    if ((D_800E296C & 0x200) != 0)
    {
        D_800E296C &= ~0x200;
        func_8009F020();
    }
    if ((D_800E296C & 0x800) != 0)
    {
        D_800E296C &= ~0x800;
        func_8009EF78();
    }
    if ((D_800E296C & 0x100) != 0)
    {
        D_800E296C &= ~0x100;
        func_8009EF04();
    }
    c = (func_800644B8(*(u16 *)(g + 0x4) << 8) >> 6) + 0x80;
    if (c >= 0x100)
    {
        c = 0xFF;
    }
    n = D_80082E80;
    col[3] = 0x68;
    col[2] = 0;
    col[1] = c;
    col[0] = c;
    func_8009DA70(n[0x24], n[0x25], (s32 *)col, (s32)ot);
    p = *(u8 **)(head + 0x5C) + 0x20;
    col[1] = 0;
    for (; p != head; p = *(u8 **)(p + 0x5C) + 0x20)
    {
        if (*(s8 *)(p + 0x13) > 0)
        {
            q = *(u8 **)(p - 0x14);
            if ((D_800E296C & 4) == 0)
            {
                if (*(s8 *)(q + 0x26) < 0 || (m = D_80082E80, *(s8 *)(q + 0x26) != *(s8 *)(m + 0x26)))
                {
                    if (func_8009FD40(q, D_80082E80) >= 4)
                    {
                        continue;
                    }
                }
            }
            mode = 0;
            k = 0;
            t = p[0x13];
            if (t == 0x1E)
            {
                if (p[0xAD] != 0)
                {
                    k = 2;
                }
                else
                {
                    k = 1;
                }
            }
            else if (t != 0x23 || *(s16 *)(p + 0xA6) == 0 || (D_800E296C & 4) != 0)
            {
                k = 1;
            }
            mode = k;
            if (mode == 0)
            {
                continue;
            }
            if (mode == 2)
            {
                col[2] = c;
                col[0] = 0;
            }
            func_8009DA70(q[0x24], q[0x25], (s32 *)col, (s32)ot);
            if (mode == 2)
            {
                if (p)
                {
                    col[2] = 0;
                    col[0] = c;
                }
                else
                {
                    col[2] = 0;
                    col[0] = c;
                }
            }
        }
    }
    col[2] = c;
    col[0] = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if ((D_800E296C & 2) != 0 ||
                func_8009EE4C(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01) != 0)
            {
                func_8009DA70(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01, (s32 *)col, (s32)ot);
            }
        }
    }
    col[2] = 0;
    col[1] = c;
    for (i = 0; i < 4; i++)
    {
        if (D_800E3CD8[i].unk_00 != 2)
        {
            break;
        }
        if ((D_800E296C & 1) != 0 ||
            func_8009EE4C(D_800E3CD8[i].unk_02, D_800E3CD8[i].unk_04) != 0)
        {
            func_8009DA70((u16)D_800E3CD8[i].unk_02, (u16)D_800E3CD8[i].unk_04, (s32 *)col,
                          (s32)ot);
        }
    }
    col[1] = 0x20;
    col[2] = c;
    col[0] = c;
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
            (D_800E3648_s[i].unk_03 & 0x40) == 0)
        {
            if ((D_800E296C & 8) != 0 ||
                ((D_800E3648_s[i].unk_03 & 0x80) == 0 &&
                 func_8009EE4C(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07) != 0))
            {
                func_8009DA70(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, (s32 *)col, (s32)ot);
            }
        }
    }
}

/* Two primitives off D_80083160.unk_00's +0x8D0 cursor, hung on the +0x8B0
   OT row either side of func_8009DAD8's own output. The cursor is read
   through the COMPONENT_REF every time, which is what makes each store
   invalidate the next read. The 8-byte local is an INITIALISED aggregate
   whose address is taken, so its two constants are built once in the
   prologue; `*(s16 *)(u + 0xA)` is then read out of the word just stored,
   because a halfword read cannot be forwarded out of a word store. The lean
   test is a BOOLEAN local computed before the three calls, since the
   comparison's operand does not survive them. 75 words, 0 differing. */
void func_8009DF0C(u8 *p)
{
    u32 w[2];
    u8 *q;
    u8 *r;
    u8 *u;
    s32 lean;
    s32 t;

    w[0] = 0x00C00300;
    w[1] = 0x00400040;
    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    r = D_80083160.unk_00 + 0x8B0;
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(r, q);
    func_8009DAD8(r, p);
    u = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(u + 0x10);
    *(u32 *)(u + 0x4) = 0x60000000;
    u[0x3] = 3;
    *(u32 *)(u + 0x8) = w[0];
    t = *(s16 *)(u + 0xA);
    if (lean)
    {
        t -= 0xE0;
    }
    *(s16 *)(u + 0xA) = t;
    *(u32 *)(u + 0xC) = w[1];
    AddPrim(r, u);
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    SetDrawArea(q, w);
    AddPrim(r, q);
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CA8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CAC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CB0);

/* The kind-0x12 node's per-frame handler: func_8009ED30 installs it at the
   node's +0x10, so `p` is `(u8 *)n + 0x20`, `v` is `n->unk_08` and `q` is
   `n->unk_0C`. A five-state machine on the halfword at +0x1A -- 0 fades in,
   1 holds, 0x10 eases toward the target pose, 0x20 decays the renderable's
   scale, 0x30 rearms -- with +0x1C the remaining frame count that every
   `x += (target - x) / n` step divides by.

   THE WHOLE RESIDUE WAS gcc's MEM_IN_STRUCT_P ALIAS TEST, and eight of this
   function's halfword accesses on `p` have to be spelled `((s16 *)p)[i]` for
   it. `*(s16 *)(p + N)` is an INDIRECT_REF and gets MEM_IN_STRUCT_P = 0, so
   it conflicts with the plain scalar `D_800E296C` and neither can be
   scheduled across the other; an ARRAY_REF sets the bit and the conflict is
   dropped. Measured by reverting exactly one group out of the landed body and
   re-running aligndiff.py (the landed body reads 9 there, all of them the
   masked constant-address `lui`s below; `--raw` is clean):

     +0x1A in the `>= 2` test          473 words, 18  -- ONE WORD: without it
       the D_800E296C `sw` cannot reach the branch delay slot, because the
       `lh` of +0x1A is forced after it and takes the same register, and a
       load-delay `nop` appears where the target has the `and`.
     the two +0x2C clears              472 words, 13
     the +0x4 / +0x6 clears            472 words, 13
     the +0x1C clear in case 0         472 words, 12
     the three stores in the two
       `D_800E296C &= mask` arms       470 words, 31  -- TWO WORDS SHORT

   That last one is the interesting half. With those stores left as
   `*(s16 *)`, both arms end with the `lw`/`ori` adjacent to the `and`, so
   jump2 cross-jumps FIVE instructions into .L8009E728 where the target shares
   only three. Forcing the shape instead with two `u32` locals
   (`m = D_800E296C;` before the stores, `D_800E296C = m & mask;` after)
   restores the two words but loses the register assignment: 473 words, 44.
   The alias spelling gets both. Plain `*(s16 *)` everywhere and no locals is
   471 words, 52.

   0x80013630..0x80013637 ARE ONE CONSTANT-ADDRESS BASE IN A LOCAL whose low
   half is zero -- `b = (u8 *)0x80010000;` assigned just before its first use,
   with the offsets in the displacement -- the device func_8009ED30's block
   already records. 0x8001363C stays an INLINE constant address, which is
   where its `lui $at` comes from. `--raw` confirms neither emits a
   relocation.

   q's +0x1C..+0x22 counters are read `*(u16 *)` and p's and v's `*(s16 *)`.
   The eases divide `(target - x)` by the frame count, so the sign of the load
   is load-bearing and the target shows which is which: q's fields take ONE
   `lhu` for both the subtraction and the `+=`, p's and v's take an `lh` for
   the subtraction and a separate `lhu` for the `+=`.
   472 words, 0 differing. */
void func_8009E038(u8 *p, u8 *v, u8 *q)
{
    u8 *g;
    u8 *e;
    u8 *b;
    u32 f;

    g = (u8 *)&D_80083160;
    if ((D_800E296C & 0x2000) != 0)
    {
        if (*(s16 *)(p + 0x1A) < 0x20)
        {
            *(s16 *)(p + 0x1A) = 0x20;
        }
    }
    f = D_800E296C;
    if ((f & 0x800000) != 0)
    {
        D_800E296C = f & ~0x2000;
        if (((s16 *)p)[0xD] >= 2)
        {
            *(s16 *)(p + 0x1A) = 0;
            *(s16 *)(p + 0x1C) = 8;
        }
        ((s16 *)p)[0x16] = 0;
        D_800E296C &= 0xFEFFFFFF;
    }
    else if (*(u16 *)(p + 0x1A) >= 0x20 && *(u16 *)(p + 0x1A) < 0x30 && (f & 0x2000) == 0)
    {
        *(s16 *)(p + 0x1A) = 0x30;
    }
    if (*(s16 *)(p + 0x1A) != 0 || (*(s32 *)(D_800E3D7C + 0x1C) & 0x10) != 0)
    {
        ((s16 *)p)[0x16] = 0;
        D_800E296C &= 0xFEFFFFFF;
    }
    switch (*(s16 *)(p + 0x1A))
    {
    case 0:
        f = D_800E296C;
        if ((f & 0x800000) == 0)
        {
            *(s16 *)(p + 0x2C) = *(u16 *)(p + 0x2C) + 1;
            if (*(s16 *)(p + 0x2C) == 0x1F)
            {
                D_800E296C = f | 0x1000000;
                func_800A634C(0);
            }
            else if (*(s16 *)(p + 0x2C) >= 0x20)
            {
                *(s16 *)(p + 0x2C) = 0x20;
            }
        }
        if (*(s16 *)(p + 0x1C) > 0)
        {
            q[0xC] = q[0xD] = q[0xE] = q[0xE] + (0x80 - q[0xE]) / *(s16 *)(p + 0x1C);
            *(u16 *)(q + 0x1C) = *(u16 *)(q + 0x1E) =
                *(u16 *)(q + 0x1E) + (0x3000 - *(u16 *)(q + 0x1E)) / *(s16 *)(p + 0x1C);
            *(u16 *)(q + 0x20) += (0x20 - *(u16 *)(q + 0x20)) / *(s16 *)(p + 0x1C);
            *(u16 *)(q + 0x22) += (0x20 - *(u16 *)(q + 0x22)) / *(s16 *)(p + 0x1C);
            *(s16 *)(v + 0x2) += (0xA0 - *(s16 *)(v + 0x2)) / *(s16 *)(p + 0x1C);
            *(s16 *)(v + 0x6) += (0x78 - *(s16 *)(v + 0x6)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x4) += (0 - *(s16 *)(p + 0x4)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x6) += (0 - *(s16 *)(p + 0x6)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x8) += (0x140 - *(s16 *)(p + 0x8)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0xA) += (0xE0 - *(s16 *)(p + 0xA)) / *(s16 *)(p + 0x1C);
        }
        if (--*(s16 *)(p + 0x1C) > 0)
        {
            break;
        }
        *(s16 *)(v + 0x2) = 0xA0;
        *(s16 *)(v + 0x6) = 0x78;
        *(u16 *)(q + 0x1E) = 0x3000;
        *(u16 *)(q + 0x1C) = 0x3000;
        *(u16 *)(q + 0x20) = 0x20;
        *(u16 *)(q + 0x22) = 0x20;
        *(u32 *)(q + 0xC) = 0x808080;
        *(s16 *)(p + 0x8) = 0x140;
        *(s16 *)(p + 0xA) = 0xE0;
        ((s16 *)p)[0x2] = 0;
        ((s16 *)p)[0x3] = 0;
        ((s16 *)p)[0xE] = 0;
        if ((D_800E296C & 0x800000) != 0)
        {
            *(s16 *)(p + 0x1C) = 0x1E;
            *(s16 *)(p + 0x1A) = *(u16 *)(p + 0x1A) + 1;
            break;
        }
        if ((*(s32 *)(g + 0x8) & 0x100) != 0)
        {
            break;
        }
        *(s16 *)(p + 0x1C) = 4;
        *(s16 *)(p + 0x1A) = 0x10;
        break;
    case 1:
        if (--*(s16 *)(p + 0x1C) > 0)
        {
            break;
        }
        ((s16 *)p)[0xE] = 8;
        ((s16 *)p)[0xD] = 0x10;
        D_800E296C &= 0xFF7FFFFF;
        break;
    case 0x10:
        if (*(s16 *)(p + 0x1C) != 0)
        {
            *(u16 *)(q + 0x1C) = *(u16 *)(q + 0x1E) =
                *(u16 *)(q + 0x1E) + (*(s16 *)(p + 0x1E) - *(u16 *)(q + 0x1E)) / *(s16 *)(p + 0x1C);
            *(s16 *)(v + 0x2) += (*(s16 *)(p + 0x14) - *(s16 *)(v + 0x2)) / *(s16 *)(p + 0x1C);
            *(s16 *)(v + 0x6) += (*(s16 *)(p + 0x16) - *(s16 *)(v + 0x6)) / *(s16 *)(p + 0x1C);
            e = D_80082E80;
            *(u16 *)(q + 0x20) += (e[0x24] - *(u16 *)(q + 0x20)) / *(s16 *)(p + 0x1C);
            *(u16 *)(q + 0x22) += (e[0x25] - *(u16 *)(q + 0x22)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x4) += (*(s16 *)(p + 0xC) - *(s16 *)(p + 0x4)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x6) += (*(s16 *)(p + 0xE) - *(s16 *)(p + 0x6)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0x8) += (*(s16 *)(p + 0x10) - *(s16 *)(p + 0x8)) / *(s16 *)(p + 0x1C);
            *(s16 *)(p + 0xA) += (*(s16 *)(p + 0x12) - *(s16 *)(p + 0xA)) / *(s16 *)(p + 0x1C);
            if (--*(s16 *)(p + 0x1C) <= 0)
            {
                *(s16 *)(v + 0x2) = *(s16 *)(p + 0x14);
                *(s16 *)(v + 0x6) = *(s16 *)(p + 0x16);
                *(u16 *)(q + 0x1C) = *(u16 *)(q + 0x1E) = *(s16 *)(p + 0x1E);
                *(u16 *)(q + 0x20) = e[0x24];
                *(u16 *)(q + 0x22) = e[0x25];
                b = (u8 *)0x80010000;
                *(s16 *)(b + 0x3630) = *(s16 *)(p + 0x4) = *(u16 *)(p + 0xC);
                *(s16 *)(b + 0x3632) = *(s16 *)(p + 0x6) = *(u16 *)(p + 0xE);
                *(s16 *)(b + 0x3634) = *(s16 *)(p + 0x8) = *(u16 *)(p + 0x10);
                *(s16 *)(b + 0x3636) = *(s16 *)(p + 0xA) = *(u16 *)(p + 0x12);
            }
        }
        else
        {
            *(u16 *)(q + 0x20) = D_80082E80[0x24];
            *(u16 *)(q + 0x22) = D_80082E80[0x25];
        }
        if ((*(s32 *)(g + 0x8) & 0x10) != 0 && (*(s32 *)(g + 0x10) & 0x8) != 0)
        {
            if (*(s16 *)(p + 0x1E) < 0x5000)
            {
                *(s16 *)(p + 0x1E) = *(u16 *)(p + 0x1E) + 0x1000;
                *(s32 *)0x8001363C = *(s16 *)(p + 0x1E);
            }
            else
            {
                *(s16 *)(p + 0x1E) = 0;
                *(s32 *)0x8001363C = 0;
            }
            *(s16 *)(p + 0x1C) = 4;
        }
        if ((*(s32 *)(g + 0x8) & 0x100) != 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                func_800A67F4();
                ((s16 *)p)[0x16] = 0;
                ((s16 *)p)[0xD] = 0;
                ((s16 *)p)[0xE] = 1;
                D_800E296C &= 0xFEFFFFFF;
            }
        }
        break;
    case 0x20:
        *(u16 *)(q + 0x1E) -= *(u16 *)(q + 0x1E) >> 2;
        if (*(u16 *)(q + 0x1E) < 0x100)
        {
            *(u16 *)(q + 0x1E) = 0;
        }
        break;
    case 0x30:
        *(s16 *)(p + 0x1A) = 0x10;
        *(s16 *)(p + 0x1C) = 4;
        break;
    }
    func_8009DF0C(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009E798);

/* The one-shot spawner for the kind-0x12 node: raise bit 0x80 of D_800E296C
   so it never runs twice, install func_8009E038 at the node's +0x10 and hand
   func_8009E798 to func_8004491C, then fill the mover, the renderable at
   +0x0C and the actor view at +0x20 out of the record at 0x80013630.

   THREE SPELLINGS ARE LOAD-BEARING, each measured.
   (1) 0x80013638/0x8001363A/0x8001363C ARE ONE CONSTANT-ADDRESS BASE IN A
       LOCAL whose low half is ZERO -- `b = (u8 *)0x80010000;` with the
       offsets in the displacement, assigned just before its first use. The
       three halfwords then share one bare `lui`. Written as three inline
       `*(u16 *)0x8001363C` constants each gets its own `lui`: 74 words of 71
       and 53 differing. Assigned at the top of the block instead, `b` takes a
       CALLEE-SAVED register and the three loads come off it: 74 words, 73
       differing. 0x80013630 and 0x80013634 stay INLINE constants -- each has
       its own `lui`/`lw` pair in the target, which is what the inline form
       gives.
   (2) THE TWO WORD LOADS GO INTO A LOCAL. Written as six inline
       `*(s32 *)0x80013630` reads, cse commons them but the scheduler lifts
       the loads above the four zero stores and fills the second load's delay
       slot with the +0x18 store: 71 words, 5 differing plus a stray `nop`.
   (3) THE `|=` ON D_800E296C IS SPLIT around the +0x1C store, the idiom
       func_800A6104's block already records: unsplit it is 72 words with a
       `nop` in the final load-delay slot.

   DEVICE: the trailing `do { } while (0)`. It is a BASIC-BLOCK BOUNDARY and
   nothing else; the four statements inside it are unchanged. Without it the
   function is 71 words and 5 differing, the whole residue being that the
   D_800E296C read takes $v0 and the constant 4 takes $v1 where the target has
   them the other way round: the two quantities tie in local-alloc's
   QTY_CMP_PRI (4 refs over 4 insns against 2 over 1, both 2.0) and the tie
   breaks on quantity number, which is birth order, so the earlier-born flag
   wins $v0. The boundary must start at `*(s32 *)(p + 0x10) = w;` exactly:
   one statement later it is 2 differing, and wrapping more costs nothing.
   Ruled out at 5 differing, all measured: `f` declared first/last/`s32`, a
   separate `g` for the or-result, `f |= 0x80` in place, `+ 0x80` for `| 0x80`,
   an `s16`/`s32`/`u16` local for the constant 4, a `u32 *fp = &D_800E296C`
   pointer local, a `volatile` read, and read-backs
   `*(s32 *)(p + 0x10) = *(s32 *)(p + 0x8)` and the +0xC/+0x4 pair; the store
   written before the read is 72 words. decomp-permuter scored 50,389
   candidates and its only improvement was this wrapper -- its score floor is
   not 0 here because the five constant addresses emit no relocation.
   71 words, 0 differing. */
void func_8009ED30(void)
{
    UnkNode *n;
    UnkVec *v;
    u8 *q;
    u8 *p;
    u8 *b;
    u32 f;
    s32 w;

    if ((D_800E296C & 0x80) == 0)
    {
        n = func_8003FC64(0x12);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_8009E038;
            func_8004491C(n, (u32)func_8009E798);
            v = n->unk_08;
            *(s16 *)((u8 *)v + 0xA) = -1;
            q = (u8 *)n->unk_0C;
            *(s16 *)(q + 0x14) = 0xC;
            *(u8 **)(q + 0x8) = D_800DD7E0;
            *(s16 *)(q + 0x1E) = 0;
            *(s16 *)(q + 0x1C) = 0;
            p = (u8 *)n + 0x20;
            b = (u8 *)0x80010000;
            *(s16 *)(p + 0x1E) = *(u16 *)(b + 0x363C);
            *(s16 *)((u8 *)v + 0x2) = *(u16 *)(b + 0x3638);
            *(s16 *)(p + 0x14) = *(u16 *)(b + 0x3638);
            *(s16 *)((u8 *)v + 0x6) = *(u16 *)(b + 0x363A);
            *(s16 *)(p + 0x16) = *(u16 *)(b + 0x363A);
            *(s16 *)(p + 0x1A) = 0x10;
            *(s16 *)(p + 0x1C) = 8;
            *(s32 *)(q + 0xC) = 0x808080;
            w = *(s32 *)0x80013630;
            *(s16 *)(q + 0x18) = 0;
            *(s16 *)(q + 0x16) = 0;
            *(s16 *)(q + 0x18) = 0;
            *(s16 *)(q + 0x16) = 0;
            *(s32 *)(p + 0x4) = w;
            *(s32 *)(p + 0xC) = w;
            w = *(s32 *)0x80013634;
            *(s16 *)(p + 0x18) = 0;
            *(s32 *)(p + 0x8) = w;
            do
            {
                *(s32 *)(p + 0x10) = w;
                f = D_800E296C;
                *(s16 *)(p + 0x1C) = 4;
                D_800E296C = f | 0x80;
            } while (0);
        }
    }
}

/* One nibble of the packed D_800E50A8 map: even x is the low nibble, odd x
   the high one.

   THE SHIFT COUNT GOES IN ITS OWN LOCAL. Written `(y << D_80083350) + x`
   inline, the parameter's `sll $a1, 16` is emitted BEFORE the symbol's `lui`
   and the first two words come out swapped (24 words, 2 differing); reading
   the global into an s32 local first puts the `lui` first. Writing the sum
   the other way round, `x + (y << ...)`, is 17 differing. The `sll 15`/
   `sra 16` pair is the /2 and the narrowing to the s16 index folded
   together. 24 words, 0 differing. */
s32 func_8009EE4C(s16 x, s16 y)
{
    s16 i;
    s32 s;
    u8 v;

    s = D_80083350;
    i = ((y << s) + x) / 2;
    v = D_800E50A8[i];
    if (x & 1)
    {
        v &= 0xF0;
    }
    else
    {
        v &= 0xF;
    }
    return v;
}

/* 22 words, 0 differing. */
void func_8009EEAC(void)
{
    s32 v[2];

    v[0] = 0x1000340;
    v[1] = 0x200020;
    func_800B835C(D_800DD80C, v, 1, 0);
    D_800E296C |= 0x800000;
}

/* Replay up to four queued marks through func_8009F3D4, stopping at the first
   whose type is not 2.

   THE TABLE MUST BE SUBSCRIPTED, NOT WALKED. With `u8 *p` and `p += 8` -- or
   with `s16 *p` and `p[0]/p[1]/p[2]` -- loop.c makes a SECOND address giv
   biased by 4 for the +2/+4 reads and hoists the constant 2 into a
   callee-saved register: 36 words against 29, 36 differing, with $s2 and $s3
   saved as well. 29 words, 0 differing. */
void func_8009EF04(void)
{
    s32 i;

    func_8009EEAC();
    for (i = 0; i < 4; i++)
    {
        if (D_800E3CD8[i].unk_00 != 2)
        {
            break;
        }
        func_8009F3D4(D_800E3CD8[i].unk_02, D_800E3CD8[i].unk_04, 0x208020, 1, 0);
    }
}

/* The D_800E3648/D_800E39C8 sweep, the same shape as func_8009F020 next door
   over the other table pair, plus the +0x03 bit-0x40 gate. 42 words,
   0 differing. */
void func_8009EF78(void)
{
    s32 i;

    func_8009EEAC();
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
            (D_800E3648_s[i].unk_03 & 0x40) == 0)
        {
            func_8009F3D4(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, 0x802080, 8, i);
        }
    }
}

/* 37 words, 0 differing. */
void func_8009F020(void)
{
    s32 i;

    func_8009EEAC();
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && *(u8 *)&D_800E3548_s[i] != 0)
        {
            func_8009F3D4(D_800E36C8[i].unk_00, D_800E36C8[i].unk_01, 0x802020, 2, i);
        }
    }
}

/* Run func_8009EEAC, then walk the engine's ring handing each record's tile
   to func_8009F3D4 with a running index. The advance is IN THE CONDITION --
   the target jumps straight to the bottom test on entry, which is what a
   `while ((p = ...) != head)` gives and a guarded `while (p != head)` does
   not. 34 words, 0 differing. */
void func_8009F0B4(void)
{
    u8 *p;
    u8 *e;
    s32 i;

    p = D_800E3D7C;
    func_8009EEAC();
    i = 0;
    while ((p = *(u8 **)(p + 0x5C) + 0x20) != D_800E3D7C)
    {
        if (*(s8 *)(p + 0x13) > 0)
        {
            e = *(u8 **)(p - 0x14);
            func_8009F3D4(e[0x24], e[0x25], 0x202080, 4, i);
            i++;
        }
    }
}

/* Mark every 6-byte map cell whose first halfword is 3, over the 1..0x3E
   square. TWO POINTERS OFF D_80083160, NOT ONE: the map base is read with a
   displacement off the symbol while the row shift is read off a SECOND
   pointer at +0x1DC, and both are needed -- either alone is one word short
   (55 against 56). 56 words, 0 differing. */
void func_8009F13C(void)
{
    u8 *g;
    u8 *h;
    u8 *m;
    s32 x;
    s32 y;
    s32 n;

    g = (u8 *)&D_80083160;
    m = *(u8 **)(g + 0x1DC);
    h = g + 0x1DC;
    func_8009EEAC();
    n = 0;
    for (y = 1; y < 0x3F; y++)
    {
        for (x = 1; x < 0x3F; x++)
        {
            if (*(u16 *)((x + (y << *(s16 *)(h + 0x14))) * 6 + m) == 3)
            {
                func_8009F3D4((s16)x, (s16)y, 0x808080, 0x10, n);
                n++;
            }
        }
    }
}

/* The UnkNode +0x10 handler of the three-stage fade func_8009F3D4 installs:
   stage 0 waits out the +0x1C countdown and re-arms it, stage 1 eases the
   renderable's three colour bytes toward the record's own triple one
   1/(count)th per frame, and the default stage shrinks the +0x1C/+0x1E
   extents and then kills the node. 110 words, 0 differing.

   THE +0x2E READ NEEDS ITS OWN `s32` LOCAL, ASSIGNED BEFORE THE D_800814A0
   STATEMENT. Written inline in the `if` it is 112 words / 21 differing -- the
   `lh` cannot be scheduled into the D_800814A0 load's delay slot because it
   reuses $v0, so the slot stays a `nop` and the store cannot reach the
   branch's. An `s16` local is 110 / 1. */
void func_8009F21C(u8 *a, UnkVec *b, u8 *c)
{
    s32 t;

    if (*(s16 *)(a + 0x1A) == 0)
    {
        *(s16 *)(a + 0x1C) = *(u16 *)(a + 0x1C) - 1;
        if (*(s16 *)(a + 0x1C) <= 0)
        {
            func_8004491C((UnkNode *)(a - 0x20), (u32)D_80044BB0);
            *(s16 *)(a + 0x1C) = 8;
            *(s16 *)(a + 0x1A) = *(u16 *)(a + 0x1A) + 1;
            func_800A56E0(0x80F);
        }
    }
    else if (*(s16 *)(a + 0x1A) == 1)
    {
        c[0xC] += (a[0] - c[0xC]) / *(s16 *)(a + 0x1C);
        c[0xD] += (a[1] - c[0xD]) / *(s16 *)(a + 0x1C);
        c[0xE] += (a[2] - c[0xE]) / *(s16 *)(a + 0x1C);
        *(s16 *)(a + 0x1C) = *(u16 *)(a + 0x1C) - 1;
        if (*(s16 *)(a + 0x1C) <= 0)
        {
            *(s32 *)(c + 0xC) = *(s32 *)a;
            *(s16 *)(a + 0x1C) = 0x10;
            *(s16 *)(a + 0x1A) = *(u16 *)(a + 0x1A) + 1;
        }
    }
    else
    {
        *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C) =
            *(u16 *)(c + 0x1C) - *(u16 *)(c + 0x1C) / *(s16 *)(a + 0x1C);
        *(s16 *)(a + 0x1C) = *(u16 *)(a + 0x1C) - 1;
        if (*(s16 *)(a + 0x1C) <= 0)
        {
            D_800E296C |= *(u32 *)(a + 0x28);
            *(u16 *)(a - 0x2) |= 0x8000;
            t = *(s16 *)(a + 0x2E);
            D_800814A0 |= 0x8000;
            if (t == 0)
            {
                D_800E296C &= 0xFFFBFFFF;
            }
        }
    }
}

/* Spawn one map marker at (x, y) and project it: allocate a 0x2 node (0x202
   when the fifth argument is non-zero), install func_8009F21C, build a
   scale-0x1800 rotation from the engine's +0xB0 bearing in the 0x1F800000
   scratchpad, push it, project (x, y) biased by half a map cell through
   RotTransPers and park the screen pair in the node's mover. The last
   argument doubles as "quiet": when it is zero the engine's 0x40000 request
   bit goes up instead.

   D_80083178 IS REACHED AS `(u8 *)D_8008333C - 0x1C4`, the one `lui`/`addiu`
   plus one `addiu` the target has, and the map-width shifts at D_80083350/
   D_80083352 are then +0x14/+0x16 off the same register -- the rule
   func_800D175C records.

   THREE SPELLINGS ARE LOAD-BEARING, each measured on this function:
   (1) THE MOVER IS READ THROUGH A CAST, NOT A COMPONENT_REF.
       `v = n->unk_08;` is 124 words against 126 with 62 differing: the
       COMPONENT_REF is MEM_IN_STRUCT_P, sched1 schedules its `lw` after the
       +0x9C store, and `q` and `v` then stop overlapping -- which frees a
       callee-saved register, so the fourth parameter keeps one instead of
       spilling to its incoming home slot at 0x44($sp) and the two reload
       words vanish. Through `*(UnkVec **)((u8 *)n + 0x8)` the load lands
       above `q`'s last use, nine values compete for nine registers, and the
       spill is the target's. 126 words / 11 differing.
   (2) `sp[0x34]` IS WRITTEN BEFORE `sp[0x30]`. 126 / 9 the other way.
   (3) `sp` IS ASSIGNED LAST of the three prologue pointers. The prologue
       initialises the fifth parameter, then D_8008333C, then D_80083178,
       then the scratchpad base; with `sp` first it is 126 / 9 and in the
       middle 126 / 4. Moving the assignment inside the `if` is 126 / 43.

   NOT NEEDED: the `(void)&d` address-take that forces a register parameter
   into its home slot (the func_800D5460 device). Here the spill falls out of
   (1) on its own, and the device instead drops a callee-saved register --
   123 words.

   OLD-STYLE DEFINITION, with no declaration added: the five landed callers
   in func_8009EF04 and its four relatives ride an implicit `int func()`
   declaration, and the K&R parameter promotions are what the target's raw
   `subu $v0, $s6, $v0` on the incoming x and y asks for. 126 words,
   0 differing. */
s32 func_8009F3D4(x, y, colour, d, e)
s32 x;
s32 y;
s32 colour;
s32 d;
s32 e;
{
    u8 *sp;
    u8 *g;
    u8 *q;
    u8 *p;
    u8 *r;
    UnkVec *v;
    UnkNode *n;

    g = (u8 *)D_8008333C;
    q = g - 0x1C4;
    sp = (u8 *)0x1F800000;
    n = func_8003FC64(e != 0 ? 0x202 : 2);
    if (n != NULL)
    {
        v = *(UnkVec **)((u8 *)n + 0x8);
        n->unk_10 = (u32)func_8009F21C;
        *(s16 *)(sp + 0x98) = 0;
        *(s16 *)(sp + 0x9A) = 0;
        *(s32 *)(sp + 0x34) = 0x1800;
        *(s32 *)(sp + 0x30) = 0x1800;
        *(s32 *)(sp + 0x38) = 0;
        *(s16 *)(sp + 0x9C) = *(u16 *)(q + 0xB0);
        RotMatrix((SVECTOR *)(sp + 0x98), (MATRIX *)(sp + 0xD0));
        ScaleMatrix((MATRIX *)(sp + 0xD0), (VECTOR *)(sp + 0x30));
        *(s32 *)(sp + 0xE4) = 0;
        *(s32 *)(sp + 0xE8) = 0;
        *(s32 *)(sp + 0xEC) = 0;
        SetRotMatrix((MATRIX *)(sp + 0xD0));
        SetTransMatrix((MATRIX *)(sp + 0xD0));
        *(s16 *)(sp + 0x74) = 0;
        *(s16 *)(sp + 0x70) = x - (1 << *(s16 *)(g + 0x14)) / 2;
        *(s16 *)(sp + 0x72) = y - (1 << *(s16 *)(g + 0x16)) / 2;
        RotTransPers((SVECTOR *)(sp + 0x70), (s32 *)(sp + 0xF0), (s32 *)(sp + 0x90), (s32 *)(sp + 0x94));
        *(s16 *)((u8 *)v + 0x2) = *(u16 *)(sp + 0xF0);
        *(s16 *)((u8 *)v + 0x6) = *(u16 *)(sp + 0xF2);
        r = (u8 *)n->unk_0C;
        *(s16 *)(r + 0x14) = 0xC;
        *(u8 **)(r + 0x8) = D_800DD824;
        *(s16 *)(r + 0x1E) = 0x2000;
        *(s16 *)(r + 0x1C) = 0x2000;
        p = n->payload;
        *(s16 *)(p + 0x1C) = 8;
        *(u32 *)n->payload = colour;
        *(u32 *)(p + 0x28) = d;
        *(s16 *)(p + 0x2E) = e;
        if (e == 0)
        {
            D_800E296C |= 0x40000;
        }
    }
    return (s32)n;
}

/* Claim the 0x400000 bit of D_800E296C unless it or 0x10000000 is already
   up, or both the engine's +0x118 counter and 0x80013716 are still under
   their limits. THE STORE BLOCK MUST BE THE FALL-THROUGH OF THE SECOND TEST:
   written as `&&` with two early returns the store's block has the `%hi`
   re-materialised and the function is 30 words at 12 differing, all of them
   that second `lui` and the registers it displaces; written as `||` with the
   body nested, cse reaches the store across the one-use label and the single
   `%hi` in $a1 serves both the load and the store. 0x80013716 is a constant
   address -- its `lui` destination is the load's. 30 words, 0 differing. */
s32 func_8009F5CC(void)
{
    u32 f;

    f = D_800E296C;
    if (f & 0x10000000)
    {
        return 0;
    }
    if (*(u16 *)(D_800E3D7C + 0x118) > 0x400 || *(u16 *)0x80013716 > 0xC00)
    {
        if (!(f & 0x400000))
        {
            D_800E296C = f | 0x400000;
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F644);

/* Step the text cursor forward. The record is the fixed RAM address
   0x80013710, a constant address rather than a `D_` symbol.

   (1) THE BACK EDGE IS A `goto`, which loop.c cannot see, so the `lui`/`ori`
   pair is re-materialised at the top of every iteration where a `while (1)`
   would have let it be hoisted into a preheader.
   (2) THE LAST `return q` NEEDS ITS OWN BLOCK. Written as a plain tail the
   `move $v0, $a0` is scheduled into the `lhu 0xA`'s load-delay slot and the
   counter is pushed to $v1 -- 72 words, 5 differing. Sharing one exit through
   `goto out` puts the copy in a JOIN BLOCK where sched1 cannot reach it,
   which also lets the counter have $v0. An if/else with the tail duplicated
   is 75 words: cross-jumping does not merge them, because the two copies
   schedule differently. 72 words, 0 differing. */
u8 *func_8009F868(void)
{
    UnkDungeon13710 *p;
    u8 *q;

loop:
    p = (UnkDungeon13710 *)0x80013710;
    q = p->unk_0C + p->unk_0A * 2;
    if (q[1] == 0)
    {
        if (D_80013714 & 8)
        {
            *(u16 *)((u8 *)p + 0x4) |= 0x10;
            return NULL;
        }
        *(u16 *)((u8 *)p + 0x4) &= 0xC;
        p->unk_0A = 0;
        func_8004D0C8((u32)D_80083780);
        D_800832B4 = 0;
        func_8004D7A8(1);
        func_8004D294(NULL, D_800DCE60, 8);
        return NULL;
    }
    if (q[1] == 0xD0)
    {
        p->unk_08 = 0;
        p->unk_0A += *(s8 *)q;
        goto loop;
    }
    if (*(s8 *)q >= 0)
    {
        p->unk_08++;
        if (p->unk_08 < q[0])
        {
            goto out;
        }
    }
    p->unk_0A++;
    p->unk_08 = 0;
out:
    return q;
}

/* Step the text cursor back one entry and hand back where it now points.
   The record lives at the fixed RAM address 0x80013710, which arrives as a
   `lui`/`ori` pair -- the constant-address spelling, not a `D_` symbol.
   24 words, 0 differing. */
u8 *func_8009F988(void)
{
    UnkDungeon13710 *p;
    u8 *q;
    s32 c;

    p = (UnkDungeon13710 *)0x80013710;
    c = p->unk_0A;
    q = p->unk_0C + c * 2;
    if (p->unk_08 == 0)
    {
        q -= 2;
        p->unk_0A = c - 1;
        if (*(s8 *)q >= 0)
        {
            p->unk_08 = *q - 1;
        }
        else
        {
            p->unk_08 = 0;
        }
    }
    else
    {
        p->unk_08 = p->unk_08 - 1;
    }
    return q;
}

/* Search the 0x80013720 table downward for the entry whose type nibble is
   0x70, 0x78, 0x80 or 0x90 and whose +0x01 byte and 0x20 bit match (a, b).

   TWO SEPARATE DEVICES KEEP THE ARM'S READ WHERE IT IS, both ablated from
   this body. (1) The dead `c = 0;` ahead of the loop: without it loop.c
   hoists the arm's whole `base + n * 2` out and cse2 folds it onto the
   preheader load -- 45 words of 49, 36 differing. loop.c ~695 clause (3),
   `reg_in_basic_block_p`, opens on `regno_first_uid`, and the dead set moves
   that off the in-loop set. (2) The second `extern` under the same `__asm__`
   name: with the plain name in the arm the two `%hi(D_80013716)` are one
   pseudo and the arm gets `move $t0, $v1` where the target has its own `lui`
   -- 49 words, 3 differing.

   The type test must also be the `||` chain with the early return in the
   `else`: an `&&` chain of `!=` makes the return arm the fall-through, 47
   words of 49, 31 differing. 49 words, 0 differing. */
u8 *func_8009F9E8(u8 a, u8 b)
{
    s32 i;
    u8 *base;
    u8 *q;
    s32 t;
    u16 c;

    base = (u8 *)0x80013720;
    /* DEVICE: a dead set, so that `c`'s first mention is not the one inside
       the loop. flow deletes it and it costs no word. */
    c = 0;
    for (i = D_80013716 - 1; i >= 0; i--)
    {
        q = base + i * 2;
        t = q[1] & 0xF8;
        if (t == 0x70 || t == 0x78 || t == 0x80 || t == 0x90)
        {
            if (q[1] == a && (q[0] & 0x20) == b)
            {
                return q;
            }
        }
        else
        {
            c = D_80013716_b;
            return base + c * 2;
        }
    }
    return base;
}

/* TWO out-of-image accesses, so a BASE LOCAL. One access written as a plain
   constant address stays absolute and the assembler expands it with
   `lui $at`; two want the constant-base idiom, which gives one bare `lui`
   in an allocatable register and two displacements. `$at` in a listing is
   the tell for the first shape. 6 words, 0 differing. */
void func_8009FAAC(void)
{
    u8 *base = (u8 *)0x80010000;

    *(u16 *)(base + 0x3718) = 0;
    *(u16 *)(base + 0x371A) += 1;
}

/* ONE out-of-image access, so a plain constant address -- the assembler's
   `lui $at` expansion; see func_8009FAAC. 6 words, 0 differing. */
void func_8009FAC4(void)
{
    *(void **)0x8001371C = (void *)0x80013720;
}

/* 22 words, 0 differing. */
void *func_8009FADC(s16 x)
{
    s16 i;

    i = x - 1;
    if (i < 0x14)
    {
        return (u8 *)0x80010248 + i * 4;
    }
    if (i == 0x14)
    {
        return D_80081484;
    }
    return *(void **)(D_800E3D7C + 0xF0);
}

/* Find the D_800E2970 region containing tile (x, y). The lower-bound tests
   are `sltu` because both operands are u16 and shorten_compare keeps the
   comparison narrow; the upper-bound tests are `slt` because the sum is an
   int.

   THE BOUND IS READ IN THE `for` CONDITION, NOT INTO A LOCAL AHEAD OF IT. A
   local loads straight into the bound's register and the function is 46 words
   of 47 -- the target loads into $v0 for the guard and COPIES it to $t0,
   which is what the inline read gives. AND THE RECORD MUST BE INDEXED AS AN
   ARRAY OF STRUCTS: a hand-walked `u8 *` makes loop.c build two address givs,
   one biased by +6, and that is 48 words / 24 differing.
   47 words, 0 differing. */
s16 func_8009FB34(u16 x, u16 y)
{
    s32 i;

    for (i = 0; i < D_8008146E; i++)
    {
        if (D_800E2970_s[i].unk_0A != 0 && x >= D_800E2970_s[i].unk_00 &&
            x < D_800E2970_s[i].unk_00 + D_800E2970_s[i].unk_04 && y >= D_800E2970_s[i].unk_02 &&
            y < D_800E2970_s[i].unk_02 + D_800E2970_s[i].unk_06)
        {
            return i;
        }
    }
    return -1;
}

/* The room-rectangle lookup over D_800E2970's 0x14-byte records.

   TWO THINGS ARE FORCED. The record address is `D_800E2970 + i * 0x14` INSIDE
   the body, so loop.c makes it one giv; walking a `u8 *r` by hand gives TWO
   induction variables -- a second biased by 6, with the later fields read at
   -4/-2/0 -- 48 words against 47 and 22 differing. And the bound must be read
   in the loop CONDITION: a local is a second quantity and costs the whole
   prologue, 46 words / 42 differing. 47 words, 0 differing. */
s16 func_8009FBF0(u16 x, u16 y)
{
    s32 i;
    u8 *r;

    for (i = 0; i < D_8008146E; i++)
    {
        r = D_800E2970 + i * 0x14;
        if (*(s16 *)(r + 0xA) != 0 && x >= *(u16 *)r - 1 && *(u16 *)r + *(u16 *)(r + 4) >= x &&
            y >= *(u16 *)(r + 2) - 1 && *(u16 *)(r + 2) + *(u16 *)(r + 6) >= y)
        {
            return i;
        }
    }
    return -1;
}

/* `b = D_800E2970;` AS ITS OWN STATEMENT: both `&D_800E2970[i * 0x14]` and
   `D_800E2970 + i * 0x14` put the symbol's lui/addiu AFTER the index chain
   and sit at 6 differing. 17 words, 0 differing. */
s32 func_8009FCAC(s16 i)
{
    u8 *b;
    u8 *p;

    if (i < 0)
    {
        return 0;
    }
    b = D_800E2970;
    p = b + i * 0x14;
    if (*(s16 *)(p + 0xA) == 0)
    {
        return 0;
    }
    return *(s32 *)(p + 0x10);
}

/* Chebyshev distance between two tiles, and `abs()` IS THE WHOLE FUNCTION.
   The `bgez`/nop/`negu` trio plus the surviving `sll 16`/`sra 16` on the
   return is the abs() tell at docs/matching-levers.md:4615. A conditional
   negate loses BOTH -- reorg fills the trio's nop and combine's
   reg_sign_bit_copies then proves the s16 return extension redundant and
   deletes it: 17 words against 20. Nineteen other spellings were swept at
   15 differing; one near-miss, `s32` parameters with the operands masked in
   place, reproduces the target's register allocation and its cross-jumped
   tail exactly and still sits at 2 -- a right diagnosis of the allocation
   and a wrong one of the negate. 20 words, 0 differing. */
s16 func_8009FCF0(u8 x0, u8 y0, u8 x1, u8 y1)
{
    s32 dx;
    s32 dy;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (dx < dy)
    {
        return dy;
    }
    return dx;
}

/* 15 words, 0 differing. */
s16 func_8009FD40(u8 *a, u8 *b)
{
    return func_8009FCF0(a[0x24], a[0x25], b[0x24], b[0x25]);
}

/* Are the two tiles adjacent, non-identical and connected?

   ONE SHARED `return 0`, NOT FIVE EARLY ONES. Written as five early returns
   the first two tests still branch to the shared tail but the third gets its
   own `j`/`move v0,zero` pair: 70 words, 21 differing. Nesting the tests so
   every failure falls out of the bottom gives the target's three
   `beqz`. `w` is both func_800A0818's out-parameter and the variable the
   shifted result is parked in -- the address-take is why the store survives.
   70 words, 0 differing. */
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                w = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((w & 1) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* func_8009FD7C one argument wider: on a connected pair it asks
   func_8009A540 whether the step is legal rather than testing the wall bit.
   THE SHIFTED RESULT IS ITS OWN LOCAL HERE, which is the one place the two
   differ: reusing `w` for it, as func_8009FD7C does, costs the `sh`/`lhu`
   pair that spills it to the out-parameter's slot and reloads it for the call
   -- 81 words against 80, 34 differing. 80 words, 0 differing. */
/* SIX PARAMETERS, the sixth unread: func_800A3D18 sets $a0..$a3 plus
   0x10($sp) and 0x14($sp), and its frame starts the saved registers at 0x18,
   so the outgoing-argument area is six words wide. 80 words, 0 differing
   either way. */
s32 func_8009FE94(s16 x1, s16 y1, s16 c, s16 x2, s16 y2, s16 y3)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;
    s32 t;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                t = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((s16)func_8009A540((u16)t, x1, y1, (s16)(c - 0x20)) != 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Walk the eight neighbouring tiles and report the first that passes:
   D_8006CCD8 and D_8006CCE8 are the engine's sine and cosine tables, read
   here as u16 and added to the record's tile, and the index is handed back
   through the caller's halfword as `i << 9` -- the same facing encoding
   csrc/ovl_town.c decodes with `(ang >> 9) & 7`.

   func_8009FB34 RETURNS s16 AND TAKES u16: the `andi $a0, $a0, 0xFFFF`
   before the call is a u16 parameter truncating, and the `sll $v0, 16`
   after it is a halfword result being tested signed. The spawners above
   store its result with `sb` and so witness neither.
   44 words, 0 differing. */
s32 func_8009FFD4(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) < 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* func_8009FFD4 with the test inverted -- `bltz` for `bgez`, the first
   neighbour that FAILS. 44 words, 0 differing. */
s32 func_800A0084(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) >= 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* 7 words, 0 differing. */
s16 func_800A0134(u8 *a, u8 *b)
{
    return *(u16 *)(a + 0x88) - *(u16 *)(b + 0x88);
}

/* Tint three colour bytes by the engine's low-five-bit frame counter.

   TWO DECLARATIONS CARRY THE WHOLE FUNCTION, both about width.
   - THE THREE QUOTIENTS ARE `s16`. As s32 the signed divide-by-16 ends in
     `sra` where the target has `srl`: combine converts the ASHIFTRT to an
     LSHIFTRT only when it can see the result narrowed, and an s16
     destination is what narrows it. `u8` quotients, a `(u8)` cast at the
     store and an `s8 *` destination all leave the three `sra`.
   - `t` IS `s16` AND IS FIRST ASSIGNED THE UNMASKED HALFWORD, which is what
     keeps the `sll 16`/`sra 16` pair: with `t` only ever assigned masked
     values, combine's reg_nonzero_bits proves the top bits clear and elides
     the extension, one word short. The masked value lives in its own `s32 u`,
     which is also where the `addu $a2, $v1, $zero` copy comes from.
   47 words, 0 differing. */
void func_800A0150(u8 *p, u8 *d)
{
    s16 t;
    s32 u;
    s16 r;
    s16 g;
    s16 b;

    t = D_80083164[0];
    u = t & 0x1F;
    t = u;
    if (u & 0x10)
    {
        t = -u & 0x1F;
        if (t == 0)
        {
            t = 0x10;
        }
    }
    r = (0x80 - p[0]) * t / 16;
    g = (0x80 - p[1]) * t / 16;
    b = (0x80 - p[2]) * t / 16;
    d[0] = -0x80 - r;
    d[1] = -0x80 - g;
    d[2] = -0x80 - b;
}

/* Six bit tests in priority order, each arm a string address, and the NULL
   test after the chain is real: cc1psx does not fold `&array != 0`.
   40 words, 0 differing. */
void func_800A020C(s32 f, u8 *d)
{
    u8 *s;

    if (f & 0x10000000)
    {
        return;
    }
    if (f & 0x8)
    {
        s = D_800DD844;
    }
    else if (f & 0x20)
    {
        s = D_800DD850;
    }
    else if (f & 0x40)
    {
        s = D_800DD848;
    }
    else if (f & 0x400)
    {
        s = D_800DD840;
    }
    else if (f & 0x10)
    {
        s = D_800DD84C;
    }
    else
    {
        s = D_800DD83C;
    }
    if (s != NULL)
    {
        func_800A0150(s, d);
    }
}

/* Try the eight neighbouring tiles starting from the engine's current
   facing (D_8008347E & 7) and hand back the first that both func_8009B25C
   accepts and func_8009A540 confirms. The direction wraps with `& 7` while
   a separate counter runs 0..7, which is why there are two induction
   variables. 70 words, 0 differing. */
u8 *func_800A02AC(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
            {
                return r;
            }
        }
    }
    return NULL;
}

/* func_800A02AC with a func_800A2CB8 gate between the two.
   75 words, 0 differing. */
u8 *func_800A03C4(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_800A2CB8(p, r) != 0)
            {
                if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
                {
                    return r;
                }
            }
        }
    }
    return NULL;
}

/* `dir` is UNSIGNED -- as s32 the shift is `sra` where the target has
   `srl`. 22 words, 0 differing. */
void func_800A04F0(u8 *p, s32 x, s32 y, u16 dir)
{
    s32 d;

    d = (dir >> 9) & 7;
    func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                  *(s16 *)(p + 0x88));
}

/* Is (x, y) outside the map? 1 when it is. TWO EARLY RETURNS, NOT AN `&&`
   CHAIN: the positive form is the same 23 words and 8 differing, with the
   four branches' delay slots and the two constant materialisations
   redistributed. 23 words, 0 differing. */
s32 func_800A0548(s16 x, s16 y)
{
    s16 *sh = (s16 *)D_8008333C;

    if (x < 0 || x >= (1 << sh[10]))
    {
        return 1;
    }
    if (y < 0 || y >= (1 << sh[11]))
    {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A05A4);

/* 18 words, 0 differing. */
s16 func_800A07D0(s16 a, s16 b, s16 c, s16 d)
{
    return func_80065F90(d - b, c - a);
}

/* func_800A07D0's angle with a quadrant flag written back through the fifth
   (stack) argument.

   THE RESULT IS KEPT IN A `u16` COPY, AND THE RETURN EXPRESSION IS NARROWED
   TO MATCH. The target masks the CALL'S OWN $v0 while a copy of it lives in
   $v1 for the tail. A single `s32 ang` cannot do that: cse's make_regs_eqv
   short-circuits on `firstr < FIRST_PSEUDO_REGISTER`, so the pseudo always
   becomes canonical and every later use is rewritten to it -- nine spellings
   all sit at 1 differing. A NARROWER copy is invisible to cse, because
   `(set (reg:HI r) (subreg:HI ...))` is not a same-mode REG, and MIPS emits
   it as a plain `addu`. The widening back has to be free too:
   `(r + 0x100) & 0xE00` on a u16 is 35 words, one `andi 0xFFFF` too many.
   34 words, 0 differing. */
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f)
{
    s32 ang;
    u16 r;

    ang = func_80065F90(d - b, c - a);
    r = ang;
    if ((ang & 0x1FF) >= 0x100)
    {
        *f |= 2;
    }
    else
    {
        *f &= 0xFFFD;
    }
    r += 0x100;
    return r & 0xE00;
}

/* Up to 16 attempts to place a spawn.  189 words, 0 differing.

   Each attempt asks func_800A4E2C for a tile and rejects it if func_8009FB34
   says no; when the argument is 0 it also rejects any tile within a
   Chebyshev-ish 0x20 of D_80082E80+0x24/+0x25 (for a nonzero argument the
   distance stays at its initial 0x100 and the test always passes).  It then
   picks a row of the D_80083478 table -- the argument's own row for n >= 2, a
   random one for n < 2 -- publishes its second byte in D_800E2968, loads the
   resource with func_800A1618, and builds the entity by CALLING
   func_800A0B94's RESULT, the same idiom the landed func_80095A10 uses.
   n >= 2 then re-centres the camera; n < 2 paces the spawn against
   D_800835E4 with func_800A1D4C.

   ====================== THE LAST SIX ROWS WERE ONE FOUR-INSN WINDOW, AND THE
   ====================== CAUSE IS `birthing_insn_p`, NOT A SPELLING

   The park sat at 189/189 words with six rows, all in 800A0990..800A09B0:
   the target interleaves the table load with the sign extension of `off`

       lui  $v0, %hi(D_80083478)
       sll  $v1, $v1, 16        <- the sign extension's first half
       lw   $v0, %lo(D_80083478)($v0)
       sra  $v1, $v1, 16
       addu $v1, $v1, $v0

   and `t = D_80083478; t += (s16)off;` always emitted the `lw` before the
   `sll`.  Read out of the .sched dump rather than guessed: in that block
   every insn has INSN_PRIORITY 1 except the `addu` at 2, and the ready list
   at the deciding step prints `171 (7f000001) 177 (1)` -- the sign
   extension's `sll` carries `max_priority` and the `lw` does not.  That is
   `adjust_priority` (sched.c:2551): REG_DEAD notes are gone by then so
   n_deaths is always 0, and case 0 sets INSN_PRIORITY to max_priority when
   `birthing_insn_p` holds -- dest is a REG, live before the insn, and
   **reg_n_sets == 1**.  The `sll`'s destination is a fresh temp, so it is
   boosted; `t` is written TWICE (`t = ...` then `t += ...`), so the `lw` is
   not.  sched1 runs backward, and a boosted insn is taken earlier in the
   backward pass, i.e. placed LATER -- hence `lw` then `sll`.

   THREE EDITS CLOSE IT, and all three are needed:

   1. **A SEPARATE BASE LOCAL** `tz`, so the `lw`'s destination has ONE set
      and is boosted too.  Both then carry max_priority, the class test ties
      (the `sll` feeds the `sra` at cost 1; the `lw` has no link to it), and
      rank_for_schedule falls through to INSN_LUID -- which prefers the
      HIGHER LUID, so the `lw` must come SECOND in the source.  Hence `tz`
      is assigned BEFORE the use of `off`, not after.
   2. **`tz` IS PINNED TO `$v0`** -- `register u8 *tz __asm__("$2");`.
      Without it `off` lands in $v0, reorg may not steal the else arm's
      `move` into a branch delay slot that reads $v0, and the arm costs a
      `nop`, a `j` and a duplicated `sll`: 191 words / 14-18 rows.  That is
      the same tension park lever 7 recorded for the one-expression form.
   3. **THE OFFSET IS THE FIRST ADDEND, AND THE ADD IS AN INTEGER ADD:**
      `t = (u8 *)((s16)off + (s32)tz);`.  This is the whole last five rows.
      With `t = tz + (s16)off;` block_alloc gives the destination a copy
      suggestion for the FIRST operand's register, so `t` takes $v0 (the
      dying pinned base) and the %hi base of the D_800E2968 store takes $v1;
      the target has them the other way round.  Writing the offset first --
      and casting both sides so it is an integer add rather than pointer
      arithmetic, which gcc canonicalises back to `tz + off` -- suggests
      $v1, the register `off` is dying in, and all five rows go.
      The pointer-arithmetic spelling `t = (s16)off + tz;` measures 189/5.

   Also measured on the pinned base: keeping `tz` alive past the add with
   `__asm__("" ::"r"(tz))` 189/7 (it moves the `sll` back); a free `t`
   reference in the `h == 0` arm to push `t` out of local-alloc 191/13, in
   the `e == 0` arm 192/42; `t` pinned to $v1 as well, no change (the
   suggestion already decides it).  And without the pinned base: the sign
   extension hoisted into its own local `so`, 189/6 byte-identical to the
   unsplit form; `so` or the sign extension routed through the existing `e`
   189/7 or `v` 191/44 (both are live across calls, so the shared allocno
   goes callee-saved); the extension spelled `(u16)off << 16` then `>> 16`
   191/18 (the cast is its own sll/sra pair).

   ========================================= THE EARLIER LEVERS, ALL STILL IN

   5. `c` IS A `u8` LOCAL THE STORE GOES THROUGH, NOT A SECOND `t[1]`:
      `cb = t[1]; D_800E2968 = cb; c = cb;` with `u8 cb` keeps the loaded
      byte in a QImode pseudo, so the store is `sb $a0` and the widening to
      `c` is zero_extendqisi2 on that register -- the target's
      `andi $s3, $a0, 0xFF`.  `D_800E2968 = t[1]; c = (u8)t[1];` reloads
      instead: cse does not forward the second read across the store to the
      global.  2 rows.
   6. `off` IS `s16`, WHICH IS WHAT MAKES loop.c HOIST THE WHOLE `(n - 2)*2`.
      With `s32 off` the shift writes `off` directly, `off` has two sets, and
      only `addiu $v0, $s2, -0x2` hoists.  With `s16 off` the assignment
      expands as `temp = (n-2)*2; off = (HI)temp`, `temp` is a non-user
      pseudo with ONE set, and force_movables (loop.c:1320) hands the
      movable that is the LAST USE of `(s32)n` the free move at loop.c:1731
      -- it is NOT the desirability test, which prints `not desirable` here.
   7. THE TABLE POINTER IS LOADED BEFORE THE INDEX IS ADDED -- two
      statements, never `t = D_80083478 + (s16)off;` (191 words / 14 rows).
   8. THE PACING BLOCK INDEXES THE ARRAY DIRECTLY AND STARTS THE POINTER AT
      [1], AND THE DO-WHILE TEST IS WRITTEN WITH THE TABLE ON THE LEFT.
      Fifteen rows, and two separate facts: reading the first element off the
      ARRAY and giving the variable the +1 address makes the base a
      block-local temp so `g` loses the local-alloc tie and takes $a0 while
      `tb` gets $v1; and `while (tp[q[0x11]] <= *(u32 *)(q + 0x18))` puts the
      TABLE load first in the RTL, where the `q + 0x18` reload then writes
      $v0 and sched2 cannot float it into the `lbu`'s load-delay slot.
   1. THE DISTANCE IS `abs()`, NOT `if (d < 0) d = -d;`.  `abssi2` emits its
      OWN delay slot, so it expands to `bgez / nop / negu` and the two
      `nop`s at 800A0958 and 800A0968 are part of the pattern.
   2. THE PACING LOOP NEEDS ITS OWN POINTER LOCAL FOR THE INNER TEST, and
      `tp = tb;` must stay INSIDE the guarded arm: moved above the `if` it is
      in the same basic block as its source, optimize_reg_copy_1
      (local-alloc.c:704) rewrites the guard's `tb` to `tp` and the copy
      evaporates.  187 words. */
void func_800A08A0(s32 a)
{
    u8 x;
    u8 y;
    u8 *t;
    u8 *q;
    u8 *w;
    u32 *tb;
    u32 *tp;
    u32 g;
    s32 e;
    s32 v;
    s32 d1;
    s32 d2;
    s16 dist;
    s16 n;
    s16 k;
    s16 off;
    s16 h;
    s16 c;
    u8 cb;
    register u8 *tz __asm__("$2");

    dist = 0x100;
    if ((D_800E296C & 0x10000000) != 0)
    {
        return;
    }
    if (D_8008347C >= 0x20)
    {
        return;
    }
    n = a;
    for (k = 0; k < 0x10; k++)
    {
        func_800A4E2C(&x, &y);
        if (func_8009FB34(x, y) < 0)
        {
            continue;
        }
        if (n == 0)
        {
            dist = abs(D_80082E80[0x24] - x) + abs(D_80082E80[0x25] - y);
        }
        if (dist < 0x21)
        {
            continue;
        }
        if (n < 2)
        {
            off = (func_800A6D30() & 0xF) * 2;
        }
        else
        {
            off = (n - 2) * 2;
        }
        tz = D_80083478;
        t = (u8 *)((s16)off + (s32)tz);
        cb = t[1];
        D_800E2968 = cb;
        c = cb;
        h = t[0];
        if (h == 0)
        {
            return;
        }
        e = (s32)func_800A1618(h, 1);
        if (e == 0)
        {
            continue;
        }
        v = func_800BCB04((x << 6) | 0x20, (y << 6) | 0x20, -0x400);
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(h, (u8 *)e, 1))(0, x, y, (s16)v);
        if (q == NULL)
        {
            return;
        }
        func_800A152C(h, 1);
        func_80042640((Unk42710 *)q, h);
        q[0x43] = func_800A6D30();
        q[0x12] = 0;
        if (n >= 2)
        {
            w = *(u8 **)(q - 0x14);
            func_8009A3D0(w[0x24], w[0x25], 0x3000);
            func_800A32A4(q);
            func_8009A028(q);
            *(u16 *)(q - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        g = D_800835E4[c];
        *(u32 *)(q + 0x18) = g;
        tb = &D_800835E4[1];
        if (g >= tb[q[0x11]])
        {
            tp = tb;
            do
            {
                func_800A1D4C(q, 0);
            } while (tp[q[0x11]] <= *(u32 *)(q + 0x18));
        }
        if (q[0x49] != 0)
        {
            return;
        }
        *(s32 *)(q + 0x48) = func_800A9230(q);
        return;
    }
}

/* Upload one of three HUD image buffers for kind `a` and report the word the
   buffer starts with: slot 0x38 and 0x31 have fixed buffers and fixed
   descriptors, everything else indexes D_80170800 downwards by the slot byte
   at s[3] and computes its descriptor from (a - 1) and that byte.

   THE abs IS A TERNARY OVER A SECOND LOCAL, and that is one word. mips.md's
   `abssi2` emits `bgez %1,1f / move %0,%1 / subu %0,$0,%0` -- three words,
   with the copy in the delay slot -- only when operands 0 and 1 are
   DIFFERENT registers; `if (v < 0) { v = -v; }` and `v = v < 0 ? -v : v;`
   over ONE variable both give the two-word same-register form and the
   function is 143 against 145. `u = D_800E3DA0[s[3]]; v = u >= 0 ? u : -u;`
   fires it (and is byte-identical to `__builtin_abs(u)`). Measured and
   rejected at the two-word form: v as s8 (147, the QImode compare costs a
   `sll`/`sra 24` pair), v as s16 (147), v as u8 (140), a plain copy
   `v = u; if (u < 0) v = -v;` with u as s32/s16/s8 (all 144, gcc coalesces
   the copy away), and `__asm__("" : "=r"(v) : "0"(v))` (146).

   THE DESCRIPTOR'S 0x4340 MUST RIDE THE s[3] TERM, assigned INSIDE the sum.
   `(a - 1) * 84 + s[3] * 12 + 0x4340` lets fold factor the common 4 out of
   both products: 143 words, two short. Grouped `(a - 1) * 84 + (s[3] * 12 +
   0x4340)` the arithmetic is right but the constant still migrates onto the
   (a-1) chain, 144/124; hoisted into its own statement first the chains
   schedule the wrong way round, 144/125. `n = (a - 1) * 84 + (m = s[3] * 12
   + 0x4340);` is 0. 145 words, 0 differing. */
s32 func_800A0B94(s16 a, u8 *s, s16 c)
{
    u8 *t;
    s32 r;
    s32 v;
    s32 u;
    s32 w;
    s32 n;
    s32 m;

    r = 0;
    if (s[0] != 2)
    {
        u = D_800E3DA0[s[3]];
        v = u >= 0 ? u : -u;
        if (v == a && v != 0x39)
        {
            s[1] = a;
        }
    }
    if (s[1] == 0)
    {
        if (a == 0x38)
        {
            t = D_80164800;
            n = 0x56B1;
            w = 0x24;
        }
        else if (a == 0x31)
        {
            t = D_8016A800;
            n = 0x569C;
            w = 0x18;
        }
        else
        {
            w = 0xC;
            t = D_80170800 - s[3] * 0x6000;
            n = (a - 1) * 84 + (m = s[3] * 12 + 0x4340);
        }
        func_8003F6D4(w, t, D_800E58A8, n);
        func_8003E4FC(6, D_800E58A8, 0);
        if (c != 0)
        {
            func_8003F320();
            if (s[0] != 2)
            {
                D_800E3DA0[s[3]] = a;
            }
            r = *(s32 *)t;
            *(s32 *)(s + 4) = r;
        }
        else
        {
            if (s[0] != 2)
            {
                D_800E3DA0[s[3]] = -a;
            }
        }
    }
    else
    {
        if (s[0] != 2)
        {
            D_800E3DA0[s[3]] = a;
        }
        if (a == 0x38)
        {
            t = D_80164800;
        }
        else if (a == 0x31)
        {
            t = D_8016A800;
        }
        else
        {
            t = D_80170800 - s[3] * 0x6000;
        }
        r = *(s32 *)t;
    }
    return r;
}

/* Run the D_80083460 + 0x6 countdown down and, when it goes negative, reseed
   it from the LCG with 0x20 forced on. 27 words, 0 differing. */
void func_800A0DD8(void)
{
    u8 *g;
    s16 t;

    if (D_8008146E != 0)
    {
        g = (u8 *)&D_80083460;
        t = *(u16 *)(g + 0x6) - 1;
        *(s16 *)(g + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(g + 0x6) = (func_800A6D30() & 0x1F) | 0x20;
            func_800A08A0(0);
        }
    }
}

/* Park a random nibble in the engine's D_80083466.

   THE CALL IS ITS OWN STATEMENT AND THE MASK IS NOT. Spelled
   `D_80083466 = func_800A6D30() & 0xF;` the assignment expands its LHS
   address FIRST, so the `%hi` lands in the jal's delay slot in a
   callee-saved register and the function is two words longer; with the
   mask folded into the call's own statement the `lui` comes out after the
   `andi`. 10 words, 0 differing. */
void func_800A0E44(void)
{
    s32 v;

    v = func_800A6D30();
    D_80083466 = v & 0xF;
}

s32 func_800A0E6C(u8 *b, s16 f, u8 *c, u8 *d)
{
    u8 seen[8];
    u8 *g;
    u8 *r;
    u8 *e;
    u8 *e2;
    u8 *e3;
    s16 i;
    s16 j;
    s16 k;
    s16 t;
    s32 v;
    s32 q;
    s32 w;
    s16 m;

    if (*(s8 *)(b + 0x26) >= 0)
    {
        if ((c[0x72] | c[0x73]) == 0 || *(s8 *)(b + 0x26) != f)
        {
            do
            {
                c[0x73] = 0;
            } while (0);
            c[0x72] = 0;
            c[0x67] = 0;
            b[0x27] = 0xFF;
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                g = D_800E2970;
                for (i = 0; i < *(s16 *)(g + *(s8 *)(b + 0x26) * 0x14 + 0xE); i++)
                {
                    e = (u8 *)(i * 4 + (s32)r);
                    if (e[0] != 0)
                    {
                        v = b[0x24] - e[0];
                        v = v >= 0 ? v : -v;
                        if (v < 2)
                        {
                            v = b[0x25] - e[1];
                            v = v >= 0 ? v : -v;
                            if (v < 2)
                            {
                                goto hit;
                            }
                        }
                    }
                }
            again:
                j = func_800A6D30();
                for (i = 0; i < D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E; i++)
                {
                    j = j % D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E;
                    e2 = (u8 *)(j * 4 + (s32)r);
                    if (e2[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e2 + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e2 + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e2 + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            hit:
                b[0x27] = i;
                c[0x67] = e[2];
                goto again;
            }
        }
        else if (b[0x24] == c[0x72] && b[0x25] == c[0x73])
        {
            r = (u8 *)func_8009FCAC(*(s8 *)(b + 0x26));
            if (r != NULL)
            {
                j = D_8008347E;
                for (i = 0; i < D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E; i++)
                {
                    j = j % D_800E2970_s[*(s8 *)(b + 0x26)].unk_0E;
                    e2 = (u8 *)(j * 4 + (s32)r);
                    if (e2[0] != 0 && b[0x27] != j)
                    {
                        t = 0;
                        switch (c[0x67])
                        {
                        case 0:
                            t = *(s16 *)(e2 + 2) < 2;
                            break;
                        case 1:
                            t = *(s16 *)(e2 + 2) == 2;
                            break;
                        case 2:
                            t = *(s16 *)(e2 + 2) == 0;
                            break;
                        }
                        if (t != 0)
                        {
                            goto found;
                        }
                    }
                    j++;
                }
                return 0;
            }
        }
        else
        {
            k = (((s16) * (u16 *)(c + 0x2A) >> 9) - 4) & 7;
            *(s32 *)&seen[0] = 0;
            *(s32 *)&seen[4] = 0;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            w = (s16)(func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73),
                                    (u16 *)d) &
                      0xFFF);
            k = ((w >> 9) - 4) & 7;
            seen[k] = 1;
            seen[(k + 1) & 7] = 1;
            seen[(k - 1) & 7] = 1;
            for (i = 0; i < 8; i++)
            {
                if ((*(u16 *)d & 2) != 0)
                {
                    m = ((w - D_8006CD00[i]) >> 9) & 7;
                }
                else
                {
                    m = ((w + D_8006CD00[i]) >> 9) & 7;
                }
                if (seen[m] == 0)
                {
                    if ((s16)func_800BCB04((u16)(D_800DCEAC[m] + (b[0x24] << 6)),
                                           (u16)(D_800DCEBC[m] + (b[0x25] << 6)),
                                           (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
                    {
                        *(s16 *)(c + 0x2A) = m << 9;
                        return 0;
                    }
                }
            }
            return 0;
        }
        goto out;
    found:
        e3 = (u8 *)(j * 4 + (s32)r);
        c[0x72] = e3[0];
        c[0x73] = e3[1];
        c[0x67] = e3[2];
        *(s16 *)(c + 0x2A) =
            func_800A0818(b[0x24], b[0x25], *(s8 *)(c + 0x72), *(s8 *)(c + 0x73), (u16 *)d);
        b[0x27] = j;
    }
    else
    {
        b[0x27] = 0xFF;
        q = ((*(s16 *)(c + 0x2A) - D_8006CD02) >> 9) & 7;
        if ((s16)func_800BCB04((u16)(D_800DCEAC[q] + (b[0x24] << 6)),
                               (u16)(D_800DCEBC[q] + (b[0x25] << 6)),
                               (s16)(*(u16 *)(c + 0x88) - 0x20)) < 0x200)
        {
            *(s16 *)(c + 0x2A) = q << 9;
        }
    }
out:
    return 0;
}

/* The `q == NULL` arm is laid out INLINE here, so it is the second arm of the
   `if`, not an early return: `if (q == NULL) { return 0; }` puts the
   `return 0` at the END and measures 33 words / 12 differing. Both spellings
   are 33 words, so only the differing count separates them.
   33 words, 0 differing. */
s32 func_800A152C(s16 a, s16 b)
{
    u8 *q;
    u8 *g;

    if (a == 0x39)
    {
        a = 2;
    }
    q = func_800A1618(a, b);
    if (q != NULL)
    {
        q[1] = a;
        q[2] += 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x1C) += 1;
        return 1;
    }
    return 0;
}

/* Is the given kind (0x39 read as 2) in the six-entry table at
   D_800E3DB0 + 1, stride 8?

   THE TABLE POINTER IS ASSIGNED BEFORE THE `if`. Left in the `for`
   initialiser it is 26 words with 16 differing: the `lui`/`addiu` pair lands
   after the 0x39 test instead of at the top, and every register in the
   function moves with it. `i = 0` stays in the `for` -- hoisting that too is
   6 differing. 26 words, 0 differing. */
s32 func_800A15B0(s16 k)
{
    s16 i;
    u8 *p;

    p = D_800E3DB0;
    if (k == 0x39)
    {
        k = 2;
    }
    for (i = 0; i < 6; i++, p += 8)
    {
        if (p[1] == k)
        {
            return 1;
        }
    }
    return 0;
}

/* Find the table slot for kind `a` in mode `b`: the 0x38 and 0x31 kinds reset
   their counters and hand back a fixed slot, b == 3 searches D_800E3DD0 from
   index 4, b == 2 hands back D_800E3DE0, and otherwise the search starts in
   D_800E3DB0 at whichever of index 3, 2 or 0 the two stored kinds select.
   This is the long form of the landed func_800A17E8 above -- same dispatch
   chain, same six-entry stride-8 table, same `i` allocation race.
   116 words, 0 differing.

   `v = p[1];` FOR THE FIRST TEST IS WORTH 13 WORDS OF DIFFERENCE. Written
   `if (p[1] == 0 || p[1] == a)` the giv reading p[1] gains a reference, wins
   the allocation race against `p` itself, and the two come out in each
   other's registers: 116 words, 16 differing, every one of them that pair.
   The THIRD read of p[1], inside the arm, must stay a read -- it is the
   target's second `lbu 0x0($a0)`, which cse cannot common because the arm is
   reached from two predecessors. */
u8 *func_800A1618(s16 a, s16 b)
{
    u8 *found;
    u8 *p;
    s32 i;
    u8 v;

    /* DEVICE: a zero-instruction reference that makes `i` live at function
       entry, exactly as the landed func_800A17E8 above needs it -- that
       function is this one's shorter sibling and its comment block has the
       `.lreg`/`.greg` reading. It emits nothing: `i` is assigned on every
       path that reaches the loop, so the read is dead in the abstract
       machine and gcc expands no code for it. It warns under `-Wall`
       (uninitialised use); the default CFLAGS has no `-Wall`, which is why
       the build is silent.
       WORTH 3 OF 116 WORDS: without it this body is 116 words and 3
       differing, the whole residue being the `b == 3` arm, where
       `addiu $a2, $zero, 0x4` and `addiu $v1, $v0, %lo(D_800E3DD0)` come out
       in each other's slots. Ruled out, all at 116 words:
         every arm writing `i` before `p`              17 differing
         only the 0x38 and 0x39-table arms doing so     3
         `i = 4` hoisted above the chain               13
         `i = 0` hoisted above the chain               27 (dead, same md5)
         `s16 i` with the device                      123 words
         `u32 i`                                       27
         declaration order, all 6 permutations         27 (same md5)
         `p = &D_800E3DB0[0x18]` for `p += 0x18`      117 words
         a separate `g` base as func_800A17E8 uses    117 words
         a second walked pointer for `p[1]`            22, and it moves the
           giv's initialiser out of the hoist block
       func_800A17E8 records two permuter runs, 352 and 82,594 candidates,
       agreeing that no legal spelling reaches 0 on its own copy of this
       allocation race. */
    __asm__("" ::"r"(i));
    found = NULL;
    if (a == 0x38)
    {
        D_800E3DB0[0x11] = 0;
        D_800E3DA0[2] = 0;
        D_800E3DA0[1] = 0;
        D_800E3DA0[0] = 0;
        return &D_800E3DB0[0x10];
    }
    if (a == 0x31)
    {
        D_800E3DB0[9] = 0;
        D_800E3DA0[1] = 0;
        D_800E3DA0[0] = 0;
        return &D_800E3DB0[8];
    }
    if (a == 0x39)
    {
        a = 2;
    }
    if (b == 3)
    {
        p = D_800E3DD0;
        i = 4;
    }
    else if (b == 2)
    {
        return D_800E3DE0;
    }
    else
    {
        p = D_800E3DB0;
        if (p[0x11] == 0x38)
        {
            p += 0x18;
            i = 3;
        }
        else if (p[9] == 0x31)
        {
            p += 0x10;
            i = 2;
        }
        else
        {
            i = 0;
        }
    }
    for (; i < 6; i++, p += 8)
    {
        if (b == 1 || b == 3)
        {
            if (p[0] != b)
            {
                break;
            }
        }
        v = p[1];
        if (v == 0 || v == a)
        {
            if (b == 2)
            {
                D_800E3DA6 = 0;
                return p;
            }
            found = p;
            if (p[1] == a)
            {
                break;
            }
            if (D_800E3DA0[i] == a)
            {
                break;
            }
        }
    }
    if (found != NULL)
    {
        func_80048224(a);
    }
    return found;
}

/* CARRIES A DEVICE, and the device-free floor is 5 differing.

   The whole residue is an ALLOCATION one: every opcode, immediate and
   displacement already matches and only three registers are wrong. The target
   puts the loop counter in $a2, the sign-extended `k` in $a1 and the raw `k`
   copy in $a3; the plain source puts the counter in $a1 and both `k`s in $a2.
   Read off `.lreg`/`.greg`: global.c's `floor_log2(n_refs) * n_refs /
   live_length` scores the counter 9 refs over 30 insns (0.90) against the
   extended `k`'s 7 over 18 (0.78), so the counter is allocated first and
   takes the lower register. The counter has to LOSE that race, and the only
   thing that moves it is making it live at FUNCTION ENTRY: that stretches its
   live length to 48, scores it 0.69, and every register falls into place.

   Measured, all 64 words: plain 13 differing; `i = 4` before the chain 5;
   every arm writing `i` before `p` 9 (all-four-or-nothing -- the other
   fifteen subsets are 15..19); `i = 0` before the chain 25; a third `s16 i`
   parameter 13 (its incoming copy is dead on every path, so flow deletes it);
   a two-case switch 24; a `do`/`while` 13; `s32 i` 63 words / 22. Two
   permuter runs, 352 and 82,594 candidates, agree that no legal spelling
   reaches 0.

   THE DEVICE READS AN UNINITIALISED LOCAL. It emits no instruction and exists
   only to make `i` live-in at entry. The function has no callers and no
   address-takers anywhere in asm/, so nothing downstream depends on it. If
   this is not wanted, delete the two device lines and write `i = 4;` before
   the `if` chain with the k==3 arm reduced to `p = D_800E3DD0;` -- that is 64
   words, 5 differing, all inside that one arm. 64 words, 0 differing. */
u8 *func_800A17E8(s16 x, s16 k)
{
    u8 *g;
    u8 *p;
    s16 i;

    /* DEVICE: a zero-instruction reference that makes `i` live at entry. */
    __asm__("" ::"r"(i));
    if (k == 3)
    {
        p = D_800E3DD0;
        i = 4;
    }
    else if (k == 2)
    {
        return D_800E3DE0;
    }
    else
    {
        g = D_800E3DB0;
        if (g[0x11] == 0x38)
        {
            p = g + 0x18;
            i = 3;
        }
        else if (g[0x9] == 0x31)
        {
            p = g + 0x10;
            i = 2;
        }
        else
        {
            p = g;
            i = 0;
        }
    }
    while (i < 6)
    {
        if (k == 1 || k == 3)
        {
            if (p[0] != k)
            {
                return NULL;
            }
        }
        if (p[1] == x)
        {
            return p;
        }
        i++;
        p += 8;
    }
    return NULL;
}

/* Run down one 8-byte D_800E3DB0 entry whose kind and owner match.

   THE OWNER LOCAL IS `s16`. As `s32 m` the whole register assignment rotates
   -- the value is loaded into $a0 and copied into $s0 at the call where the
   target loads it into $s0 and copies it into $a0: 63 words, 16 differing,
   every one a register name, with no extra instruction anywhere. As `u8 m`
   it also costs an `andi` (64 words). 63 words, 0 differing. */
void func_800A18E8(s16 a, s16 b)
{
    s16 i;
    u8 *p;
    s16 m;

    p = D_800E3DB0;
    if (a == 0x39)
    {
        a = 2;
    }
    if (b == 1 || b == 3)
    {
        for (i = 0; i < 6; i++, p += 8)
        {
            if (p[0] == b)
            {
                m = p[1];
                if (m == a)
                {
                    if (*(s8 *)(p + 2) <= 0)
                    {
                        return;
                    }
                    if (p[0] == 1)
                    {
                        return;
                    }
                    p[2]--;
                    if ((s8)p[2] > 0)
                    {
                        return;
                    }
                    p[1] = 0;
                    p[2] = 0;
                    if ((s16)func_800A15B0(m) == 0)
                    {
                        func_80048190(m);
                    }
                    return;
                }
            }
        }
    }
}

/* Raise or clear the view's 0x20000 "this one is interesting" bit from the
   distance to D_80082E80, and hand the caller back 0 when the bit ends up
   clear. THREE THINGS THE LISTING FORCES.
   THE RETURN TYPE IS NOT `void`: the declaration at the top of the unit said
   so, but the tail reads +0x1C again and shifts it into $v0, which a void
   function would have deleted -- so it is now `s32`. Both landed callers
   (func_800A1B44, func_800AA53C) discard the result and both still measure
   0 differing.
   THE SHIFT IS UNSIGNED: through `*(s32 *)` it is `sra 17` (1 differing);
   the field has to be read as `u32` for the `srl`.
   AND THE OUT-PARAMETER'S -1 IS SIGNED: `*out = -1` through the declared
   `u8 *` is `li $v0, 255` where the target has `addiu $v0, $zero, -0x1`
   (2 differing, one per arm), so the store casts to `s8 *`.
   D_80082EA6 is its own symbol and not `D_80082E80[0x26]`: the `lb` carries
   its own `lui`, in a block the call's `addiu $a0, %lo(D_80082E80)` does not
   reach. 88 words, 0 differing. */
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out)
{
    s32 fl;

    fl = *(s32 *)(b + 0x1C);
    if ((fl & 0x20000) != 0)
    {
        if ((fl & 0x10) != 0)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
        else if ((*(s8 *)(a + 0x26) < 0 || *(s8 *)(a + 0x26) != D_80082EA6) &&
                 func_8009FD40(D_80082E80, (u8 *)a) >= d)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
    }
    else if ((fl & 0x10) == 0)
    {
        if ((*(s8 *)(a + 0x26) >= 0 && *(s8 *)(a + 0x26) == D_80082EA6) ||
            func_8009FD40(D_80082E80, (u8 *)a) < c)
        {
            *(s32 *)(b + 0x1C) |= 0x20000;
        }
    }
    return (*(u32 *)(b + 0x1C) >> 17) & 1;
}

/* Walk the engine's own ring and run func_800A19E4 on every member. The head
   is RE-READ out of D_800E3D7C at the bottom of each turn rather than held in
   a local, which is the second `lw %lo(D_800E3D7C)($s3)` inside the loop; the
   two s16 parameters have their `sll` hoisted into the preheader and their
   `sra` left in the body. 35 words, 0 differing. */
void func_800A1B44(s16 x, s16 y)
{
    u8 *p;

    p = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
    while (p != D_800E3D7C)
    {
        func_800A19E4(*(s32 *)(p - 0x14), p, x, y, p + 0x9C);
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* The same search over the engine's four-entry table, keyed on a POINTER
   field rather than a byte: +0xAC in one and +0xE4 in the other. The base is
   read from D_800E3D7C ONCE and walked, which is what keeps the `lw` out of
   the loop.

   func_800A1BD0 IS DECLARED IN include/structs.h, as `s32 func_800A1BD0(u8
   *)`, because a resident engine unit calls this address -- and the engine
   reaches whichever overlay is loaded, so that declaration is about this
   function. It is adopted here rather than guessed at, and it is what fixes
   the key as a `u8 *`: csrc/unk_154BC.c writes the same +0xAC field with
   `*(u8 **)(((s16)k) * 4 + D_800E3D7C + 0xAC) = nw;`. The s32 return then
   makes the index's `sll`/`sra` an explicit cast rather than a consequence
   of the return type. */
s32 func_800A1BD0(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xAC) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

s32 func_800A1C14(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xE4) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* A five-gate predicate returning 0 or 1 as an s16 -- the shared exit is
   `sra $v0, $v0, 16`, and each path narrows with its own `sll 16`. The last
   comparison is written TABLE-FIRST: the other order emits the two `lw`s
   reversed and swaps $v0/$v1 across six words. 61 words, 0 differing. */
s16 func_800A1C58(u8 *p)
{
    s16 r;

    r = 0;
    if (*(s8 *)(p + 0x13) < 0)
    {
        return 0;
    }
    if (p[0x11] >= 0x63)
    {
        return 0;
    }
    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        return 0;
    }
    if ((*(u32 *)(p + 0x1C) & 0x80000) != 0)
    {
        return 0;
    }
    if (p[0x13] == 0)
    {
        if (D_800E3D7C[0x9A] == 0x22)
        {
            return 0;
        }
    }
    if ((*(u32 *)(p + 0x1C) & 0x228) == 0 && D_800835E8[p[0x11]] <= *(u32 *)(p + 0x18))
    {
        r++;
    }
    return r;
}

/* The dungeon "level up" twin of func_800A2424.  438 words, 0 differing.

   SIX LEVERS, and the sixth is one free reference.

   1. THE SPELL LOOP'S RULE INDEX IS A FRESH LOCAL `k' THAT IS *ALSO* READ IN
      THE MESSAGE LOOP -- `k = p[i * 3 + 8];' in both, in place of the inlined
      subscript, lifted verbatim from the landed twin.  The message-loop read
      is the half that pays; the spell loop alone was measured and is not
      enough.  Fixes the whole spell-loop rotation (induction ptr $a2->$a1,
      k $a1->$a0, &e[i] $a0->$a2).  13 rows.
   2. `i = 0;' BEFORE `d = (UnkDungeonName0 *)D_8006DE24;', with the message
      loop written `for (; i < 3; i++)'.  The two insns are independent, so
      sched1 orders them by INSN_LUID and nothing downstream recovers the
      swap.  2 rows.
   3. `hi -= lo;' AS ITS OWN STATEMENT for STAT 5 ONLY (`hi -= lo; o = n =
      p[5]; n += hi;' rather than `o = n = p[5]; n += hi - lo;').  15 rows.
      It fixes both halves at once: the `lbu $v1, 0x5($s2)' stops being
      scheduled two words early, AND the $a0/$v1 rotation in the `hi' block
      comes out the target's way, because the statement changes the block's
      LUIDs and with them global.c's priority order over the t[5] reload and
      the two `mflo's.  The other five stat blocks keep `n += hi - lo;'.
      Note the direction: `hi -= lo' -- the level-DOWN twin has `lo - hi'.
   4. `hi = 0x63;' SPLIT OUT AS ITS OWN SET inside the loop, then
      `r = (hi - p[i*3+0xA]) / u2 + p[i*3+0xA];'.  Lever "DENY loop.c A
      CONSTANT HOIST BY PUTTING THE CONSTANT IN A *USER* VARIABLE": the holder
      is a user variable, set three times, live across basic blocks, so all
      three scan_loop arms fail, no movable is made, and the target's
      `addiu $v0, $zero, 0x63' is rematerialised at the use instead of
      hoisted to `li $t1, 0x63' in the preheader.  9 rows.
   5. THE 0x63 IS HELD IN `hi', NOT IN `r'.  Which user variable holds it
      decides whether the constant and the subtraction share a register.
      Measured holders, aligned rows: hi 4, u 6, w 6, r 6, hp 7, n 7, c 17,
      k 23, o 436 words/70, lo 440 words/104.

   6. `u2 = 0x64 - lvl;' GOES IN THE LOOP BODY AFTER `e[i] = 1;' AND IS
      HOISTED BY loop.c -- AND THE HOIST IS PAID FOR WITH ONE FREE REFERENCE
      TO `lvl'.  This is what took the park's last 4 rows to 0.

      The target's `addiu $v0, $zero, 0x64' / `subu $a3, $v0, $s1' sit at
      words 272/273, at the END of the loop preheader, after four other
      loop.c hoists.  That position can ONLY be a loop.c hoist: sched1 leaves
      the preheader in INSN_LUID order (all eight insns have priority 1), and
      `move_movables' does `emit_insn_before (..., loop_start)', so a hoist's
      position in the preheader is its SOURCE position among the movables --
      and arm insns are always emitted before every hoist, so no placement
      inside the `if (p[0x13] != 0)' arm can reach word 272.  In the arm the
      pair comes out at 267/268 instead: 438 words, 4 rows, which is where
      this park sat.

      But the hoist adds the five preheader insns to lvl's live range and
      that LOSES THE p/lvl ALLOCATION RACE BY 0.2%.  global.c's
      `allocno_compare' priority is floor_log2(n_refs) * n_refs / live_length:

        u2 in the arm      lvl: 4*16/197 = 0.3249  >  p 0.3188   OK
        u2 hoisted         lvl: 4*16/202 = 0.3168  <  p 0.3188   FLIP
                           p:   5*38/596 = 0.3188

      lvl must be allocated before p to get the target's lvl=$s1 / p=$s2.
      Hoisted and unpaid-for the whole register file rotates: 437 words,
      62 rows.

      THE CURE IS TO WRAP `lvl = p[0x11];' IN `do { } while (0);'.  That
      buys lvl a 17th reference for nothing: flow.c:2128 weights each
      reference by `loop_depth', and the inner loop's NOTEs raise the depth
      over the statement without emitting an instruction.  The priority
      becomes 4*17/202 = 0.3366, well clear of p, and the margin is wide
      rather than marginal -- 17 references stay ahead of p until lvl's live
      length reaches 213 against the 202 it has.

      `__asm__("" ::"r"(lvl));' as the first statement of the arm does the
      same thing and is how this first landed, but it is a scaffold where the
      do-while is ordinary C, so the do-while is what is in the tree.  Of the
      twelve statements mentioning `lvl', exactly three wrappers reach 0 or
      near it -- `lvl = p[0x11];' 0 rows, the `hp = ...' line 0 rows,
      `p[0x11] = lvl + 1;' 2 rows -- and the rest run 29 to 72, so the
      position is a real choice and not a formality.

      That is the general shape of this fix -- `flow_analysis' runs ONCE
      (toplev.c:3463) and BEFORE combine (:3488), so a reference that costs
      no instruction is free in the allocator's arithmetic.  The alternative
      route, taking one reference OFF p by gating the spell loop on
      `if (c != 0)', does produce the exact preheader but makes `c'
      callee-saved ($s6, pushing f to $s7 and the frame to 0x40): 438 words,
      16 rows.

      `u2' POSITIONS MEASURED (with levers 1-5 in place, aligned rows):
      first statement in the arm 4; split as `u2 = 0x64; u2 -= lvl;' in the
      arm 4 (identical); `i = 2;' before or after it in the arm 4
      (identical); before the `if' 437 words/25; top of the loop body 14
      (hoisted to the WRONG slot, before the D_8006DE24 base, and the `1'
      hoist is then LOST into the loop); after `e[i] = 0;' 14 (same);
      before `e[i] = 1;' 437/86; `(0x64 - lvl)' inline with no u2 local
      437/62; `u2 = -lvl; u2 += 0x64;' in the loop 437/63.

   Also measured and kept: `(*(u16 *)0x80013714 & 1) == 0' as a literal
   address, not through a declared symbol -- the symbol lets reorg lift its
   `lui' into the `beqz $v0, .L800A23F8' delay slot where the target has a
   `nop', and that is the whole word.  The second test measures the same
   either way. */
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
        do
        {
            lvl = p[0x11];
        } while (0);
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
                            u2 = 0x64 - lvl;
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

/* The dungeon "level down" twin of func_800A1D4C. 432 words, 0 differing.
   The second parameter is a SHORT (`sll $v0, $s6, 16' + `beqz'), not the
   s32 the unit declared.

   FOUR LEVERS, all allocation and scheduling rather than shape:

   * THE `t[x] * q[x]' PRODUCT HAS TO SHARE THE MESSAGE LOOP'S COUNTER `i'.
     The target holds it in `$s0' although it crosses no call; as its own
     `s32 r' it is a short allocno and global.c's find_reg takes $a0/$v1
     from the call-clobbered end of reg_alloc_order. Written into `i', which
     crosses twelve calls, find_reg goes to the callee-saved end. 36 -> 17
     aligned differences. Six rewrites of the expression could not move it
     (fully inlined 42, product and high half reordered 56, explicit r1/r2
     temporaries +16 words, one whole-difference expression 86).

   * THE SPELL LOOP'S RULE INDEX IS A LOCAL OF ITS OWN THAT IS ALSO READ IN
     THE MESSAGE LOOP (`k = p[i * 3 + 8]; h = func_80099194(d[k].unk_00, h);'
     in place of the inlined subscript). 2 -> 0. This is the whole residue
     the park was left on: the target loads it into `$a0' and loop.c's
     induction pointer into `$a1', and this candidate had them the other way
     round. The reason is a hard-register conflict, read out of `.greg':
     with the index written into the class local `c' the allocno is live
     across the `sll $a0, $v1, 11' that sets up func_800647A0's argument at
     word 38, so `c' conflicts with hard reg 4 and find_reg skips to $a1;
     with a fresh local used only in the spell loop the allocno conflicts
     with $v0 alone and find_reg takes $v1, which shifts every register in
     the loop. Reading the SAME local in the message loop adds the $v1
     conflict without adding an $a0 one -- `k' dies building the argument
     address, so it crosses no call -- and $a0 is the first register left.
     Measured with the index in `c' 10, `hi' 14 (and `hi' then takes $a0 in
     all six stat blocks, 12 rows), a fresh local used only in the spell
     loop 17, `u' 17 / 30 asm lines, `w'/`n'/`hp'/a second fresh local 17,
     `lvl' and `lo' over 100.

   * `i = 0;' BEFORE `d = (UnkDungeonName0 *)D_8006DE24;', with the message
     loop then written `for (; i < 3; i++)'. 2 -> 0, and the last two words.
     `addu $s0, $zero, $zero' and `addiu $s2, $v0, %lo(D_8006DE24)' are
     independent, so sched1 orders them by INSN_LUID and nothing downstream
     can recover the swap. Assigning `d' inside the loop body and letting
     loop.c hoist it measures the same.

   * `(*(u16 *)0x80013714 & 1) == 0' FOR THE FIRST OF THE TWO TESTS. The
     declared `D_80013714' lets reorg lift the `lui %hi' into the
     `beqz $v0, .L800A2AB8' delay slot; the target's slot holds the
     `addiu $v0, $zero, 0x1' reorg COPIES out of the shared `return 1'
     block, which a constant address's single rtl insn leaves it free to
     take. 431 -> 432 words and 149 -> 36 differing. The SECOND test
     measures identically either way, so it is left as the symbol.

   Also measured and rejected: the p[0..3] loads read after the `hi' line
   (431/202), `lo' used as the product with `hi' computed first (432/56),
   the product fully inlined twice (432/42), explicit product temporaries
   (448 words), the whole difference as one expression with no lo/hi locals
   (432/86), and five spellings of the `return 1' itself (all 431 words --
   the delay slot is the only control there is). */
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
    s32 k;
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
            k = p[i * 3 + 8];
            if (k != 0)
            {
                u = D_8006DE24[k].unk_10 >> 4;
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
            i = 0;
            d = (UnkDungeonName0 *)D_8006DE24;
            for (; i < 3; i++)
            {
                if (e[i] != 0)
                {
                    h = (void *)func_8009929C(0xA, h);
                    k = p[i * 3 + 8];
                    h = func_80099194(d[k].unk_00, h);
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

/* 8 words, 0 differing. */
void func_800A2AE4(void)
{
    u8 *g;

    D_800DCF4F = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

/* Tile (x, z) to the halfword world coordinates at +0x2 and +0x6. The
   `(s16)` casts are what give the `sll 16`/`sra 10` pairs, and neither
   field is an UnkVec member, so both go in as offsets.
   9 words, 0 differing. */
void func_800A2B04(UnkVec *p, s32 x, s32 z)
{
    *(s16 *)((u8 *)p + 0x2) = ((s16)x) * 64 + 0x20;
    *(s16 *)((u8 *)p + 0x6) = ((s16)z) * 64 + 0x20;
}

/* func_80094EA4's shape over two different globals, nested for the same
   reason. 13 words, 0 differing. */
s32 func_800A2B28(void)
{
    if ((D_80013714 & 2) == 0)
    {
        if (D_800E3D20 != 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Claim D_80083460 + 0xC for the caller's object unless anything else is
   pending, then report func_800A2B28's answer. ONE `||` CHAIN, NOT FOUR
   `if`s, for the same reason func_800A2C34 above records: each test's
   `addiu $v0, $zero, 1` is reorg duplicating ONE `return 1` into four delay
   slots. 32 words, 0 differing. */
s32 func_800A2B5C(void *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(void **)(g + 0xC) != d)
    {
        if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0 ||
            (*(u16 *)(g + 0x2) & 8))
        {
            return 1;
        }
        *(void **)(g + 0xC) = d;
    }
    return (s16)func_800A2B28();
}

/* Four engine flags, any of which blocks whatever the caller is about to do.

   A COMMON RESULT VARIABLE, NOT FOUR EARLY `return 1`s. Written as four
   `if (...) return 1;` and a trailing `return 0`, jump.c store-flags the LAST
   pair into `j` + `sltu $v0, $zero, $v0` and the fourth `bnez` is gone: 22
   words, 5 differing. An `&&` chain whose fall-through returns 0 is the same
   5, and so is a `goto`-shared `return 1`. Only the explicit if/else over one
   variable keeps all four branches -- which CORRECTS the escape the
   store-flag lever records. 22 words, 0 differing. */
/* ONE PARAMETER, unread. The body never touches $a0, which is why `void`
   had never been falsified -- but all five call sites set it in the `jal`'s
   delay slot (func_80096100 and func_8009345C pass $zero, func_800A9E70
   passes $s1, func_800B253C passes $s0, func_800AA36C passes its fourth
   argument). 22 words, 0 differing either way. */
s32 func_800A2BDC(u8 *a)
{
    u8 *g;
    s32 r;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s32 *)(g + 0x8) != 0 ||
        (*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        r = 1;
    }
    else
    {
        r = 0;
    }
    return r;
}

/* ONE `||` CHAIN, NOT THREE `if`s: separate returns are 17 words / 8
   differing because jump.c's store-flag transform turns the last pair into
   `sltu`. Sharing one `return 1` lets reorg duplicate the `addiu $v0, 1`
   into all three delay slots. 17 words, 0 differing. */
/* ONE PARAMETER, unread. Across its four ASM callers the setup is four
   `addu $a0, $zero, $zero` and two `addu $a0, $sN, $zero`, so it is real and
   it is a pointer. 17 words, 0 differing either way. Every future landing of
   one of those callers has to pass it. */
s32 func_800A2C34(u8 *q)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0)
    {
        return 1;
    }
    return 0;
}

/* Two accesses off D_80083460, so a base pointer. The result accumulating
   in $v1 and moving to $v0 at the `jr` is the `||`. 13 words, 0 differing. */
/* ONE PARAMETER, unread: func_800CB170 sets $a0 in the `jal`'s delay slot
   and the now-landed func_800AB778 does too. 13 words, 0 differing
   either way. */
s32 func_800A2C78(u8 *p)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    return *(s32 *)(g + 0xC) != 0 || *(s16 *)(g + 0xA) != 0;
}

/* 3 words, 0 differing. */
s32 func_800A2CAC(u8 *p)
{
    return p[0x25] < 0x40;
}

/* THE TAIL'S XOR MUST LAND IN AN `s32` LOCAL FIRST. Written as one
   expression, the s16 return's truncation is folded back into both loads and
   they come out `lhu` -- 44 words, 2 differing. The local keeps the two `lw`
   and leaves the narrowing as the trailing `sll`/`srl` pair. 44 words,
   0 differing. */
s16 func_800A2CB8(u8 *a, u8 *b)
{
    s32 t;

    if (b == NULL)
    {
        return 0;
    }
    if ((*(u32 *)(a + 0x1C) & 0x410) != 0)
    {
        return 1;
    }
    if (a[0x13] == 0x2B && (*(u16 *)(a + 0x98) & 0x8000) != 0)
    {
        return 1;
    }
    if ((*(u32 *)(b + 0x14) & 0x4000) != 0 && (*(u32 *)(a + 0x14) & 0x4000) != 0)
    {
        return (s16)func_80042900((s8 *)a, 0xC) != 0;
    }
    t = *(u32 *)(a + 0x1C) ^ *(u32 *)(b + 0x1C);
    return t & 0x2000;
}

/* The read and the add are SEPARATE STATEMENTS: folded into one expression
   the `or`'s result takes $v0 instead of reusing the low byte's $v1.
   20 words, 0 differing. */
void func_800A2D68(u8 *p, u16 n)
{
    u32 v;

    v = (p[0x41] << 8) | p[0x42];
    v += n;
    if (v > 0xFFFF)
    {
        v = 0xFFFF;
    }
    p[0x41] = v >> 8;
    p[0x42] = v;
    D_800E296C |= 0x100000;
}

/* Split the actor's +0x06 damage figure across the party: double it once per
   flag on the target's +0x54 and once per flag on its own, count the live
   D_800E3D7C +0xAC slots, divide rounding up, and then either hand each live
   slot its share through func_800B4C7C/func_800A2D68 or -- when the +0x60
   occupant is not 0x4000-flagged -- give the whole figure to that occupant.
   138 words, 0 differing.

   ONE TRAILING `return v`, WITH THE WHOLE BODY NESTED. Written as two early
   `return v;` exits the two zero-returns compile to `move $v0, zero` in
   their own delay slots instead of sharing .L800A2FB8's `addu $v0, $s3`,
   and `v = 0` is constant-propagated away: 138 words, 3 differing.

   THE VARIABLE SHIFT NEEDS ITS OWN LOCAL. `w = v + (v >> (d > 1));` written
   inline is 141 words / 76 differing -- gcc specialises the boolean shift
   count into a branch and two shifts where the target has
   `slti`/`xori`/`srav`. Bound to an `s32 sh` first it is 138 / 4, and that
   one change also stops D_800E3D7C being loaded twice in the tail.

   `n = 1;` sits AFTER the +0x40 test, not before it: in the same basic block
   as the loop's `i = 1` cse turns the second constant into
   `addu $s1, $a1, $zero`, which is 1 differing the other way. `i = n` and
   `i = 1` are a tie. */
s32 func_800A2DB8(u8 *p)
{
    u8 *q;
    u8 *s;
    s32 v;
    s32 n;
    s32 i;
    s32 d;
    s32 w;
    s32 t;
    s32 sh;

    v = 0;
    q = *(u8 **)(p + 0x60);
    if (q != NULL && *(s8 *)(q + 0x13) >= 0)
    {
        v = *(u16 *)(p + 0x6);
        if (*(u32 *)(q + 0x54) & 0x20)
        {
            v *= 2;
        }
        if (*(u32 *)(p + 0x54) & 0x40)
        {
            v *= 2;
        }
        n = 1;
        for (i = 1; i >= 0; i--)
        {
            s = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
            if (s != NULL && s[0x28] != 0)
            {
                n++;
            }
        }
        v = (v + n - 1) / n;
        if (*(u32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x4000)
        {
            for (i = 1; i >= 0; i--)
            {
                s = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
                if (s != NULL && s[0x28] != 0)
                {
                    d = s[0x11] - p[0x11];
                    w = v;
                    if (d < 0)
                    {
                        sh = d > 1;
                        w = v + (v >> sh);
                    }
                    func_800B4C7C(0x82, s, (s16)w, 1);
                    func_800A2D68(s, w);
                }
            }
            if (D_800E3D7C[0x28] == 0)
            {
                return 0;
            }
            d = D_800E3D7C[0x11] - p[0x11];
            w = v;
            if (d < 0)
            {
                sh = d > 1;
                w = v + (v >> sh);
            }
            func_800A2D68(D_800E3D7C, w);
            func_800B4C7C(0x82, D_800E3D7C, (s16)w, 1);

            return w;
        }
        v = *(u16 *)(p + 0x6);
        func_800A2D68(*(u8 **)(p + 0x60), v);
        t = (*(u32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x2000) ? 0x82 : 0x83;
        func_800B4C7C(t, *(u8 **)(p + 0x60), *(s16 *)(p + 0x6), 1);
    }

    return v;
}

/* The "what did I just bump into" announcer. Run func_800A2DB8 unless the
   record and its +0x60 occupant are both 0x4000-flagged, then post up to two
   messages off func_800990FC's handle: the plain one when the record itself
   is not 0x4000-flagged, and -- if func_800A2DB8 returned a count -- either
   the D_8007359C[0] name line or the +0x60 occupant's own, each closing on
   func_8003AD08 and a trailing string. The two arms CROSS-JOIN onto the last
   func_80099194, which is why that call is written in both.

   TWO SPELLINGS ARE LOAD-BEARING, 10 differing between them.

   (1) The two EARLY +0x60 reads are written INLINE, not through the `q`
   local the later ones use. A single named local for all three is one
   quantity and takes $v1 everywhere; the target has $v0 for the first two
   and $v1 for the third. Worth 8 differing.

   (2) THE 0x4000 MASK NEEDS ITS OWN STATEMENT. `ok = (flags & 0x4000) != 0;`
   lets combine rewrite the test to `(flags >> 14) & 1` -- `srl $a0, $v0,
   0xE` / `andi $a0, $a0, 1` where the target has `andi $v0, $v0, 0x4000` /
   `sltu $v1, $zero, $v0`. A `(u32)` mask and a `? 1 : 0` are both still
   2 differing; only binding the masked value to its own local first keeps
   the `andi`. That is lever 16, "`flags & (1 << i)` needs the mask in a
   local", on a test rather than on a loop. `ok = flags & 0x4000;` without
   the comparison is 123 words / 108 differing.

   124 words, 0 differing. */
void func_800A2FE0(u8 *p)
{
    u8 *q;
    void *m;
    u8 *h;
    s32 r;
    s32 ok;
    s32 v;

    r = 0;
    ok = 0;
    if (*(u8 **)(p + 0x60) != NULL && (*(s32 *)(*(u8 **)(p + 0x60) + 0x14) & 0x4000) != 0)
    {
        v = *(s32 *)(p + 0x14) & 0x4000;
        ok = v != 0;
    }
    if (!ok)
    {
        r = func_800A2DB8(p);
    }
    if ((*(s32 *)(p + 0x14) & 0x20000000) != 0)
    {
        return;
    }
    if ((*(s32 *)(p + 0x14) & 0x4000) == 0)
    {
        if (*(u8 **)(p + 0x60) != NULL && *(s8 *)(*(u8 **)(p + 0x60) + 0x13) >= 0)
        {
            m = func_800990FC();
            h = func_80099194(D_800E09CD, m);
            h = func_80099734(p, h);
            h = func_80099194(D_80089000, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    if (r == 0)
    {
        return;
    }
    q = *(u8 **)(p + 0x60);
    if (q == NULL)
    {
        return;
    }
    if ((*(s32 *)(q + 0x14) & 0x4000) != 0)
    {
        if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            return;
        }
        m = func_800990FC();
        h = func_80099194((u8 *)D_8007359C[0]->unk_04, m);
        h = func_80099194(D_800E09D9, h);
        h = func_8003AD08(r, h);
        h = func_80099194(D_800E09E6, h);
    }
    else
    {
        if (*(s8 *)(q + 0x13) < 0)
        {
            return;
        }
        m = func_800990FC();
        h = func_80099734(*(u8 **)(p + 0x60), m);
        h = func_80099194(D_800E09EE, h);
        h = func_8003AD08(r, h);
        h = func_80099194(D_800E09FB, h);
    }
    func_80099290(h);
    func_800A5720(m);
}

/* Walks the engine's node list and, for every kind-0x1E node whose +0xA4
   owner is this record, bumps two counters and sets a request bit.
   53 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, and they fix OPPOSITE halves of one
   scheduling problem -- the 52-word plateau two earlier attempts and a
   permuter run all sat on.

   (1) `__asm__("")' BETWEEN THE TWO COUNTER INCREMENTS -- the missing word.
   A traditional asm (ASM_INPUT, no operands) makes sched.c flush every
   pending read/write list and depend on all of memory (sched.c:1943-1970),
   so the D_800DCE68 load cannot be hoisted into the load-delay slot of
   `lhu $v0, 0xA($a3)' and the target's bare `nop' survives.  Nothing else
   reaches it: `*(u16 *)(g + 0xA) += 1' and `D_800DCE68 += 1' are both
   non-struct refs whose addresses canon_rtx resolves to distinct symbols
   (a once-set `g = (u8 *)&D_80083460' carries a REG_EQUIV), so
   memrefs_conflict_p proves them disjoint and true_dependence's
   MEM_IN_STRUCT_P clauses -- which only ever CLEAR a dependence -- never
   get the chance to fire.  It emits no instruction.  Without it, with
   everything else below: 52 words, 9 aligned.

   (2) THE THREE RECORD STORES AS STRUCT MEMBERS, not `*(s16 *)(n + 0x6E)'
   casts -- the second half.  COMPONENT_REF sets MEM_IN_STRUCT_P, and with
   the varying `n' base that is exactly true_dependence's exemption against
   the fixed, non-struct `sh %lo(D_800DCE68)', so `lw $v1, 0x1C($a1)' moves
   up into that store's own delay slot as the target has it.  As casts the
   load is pinned below the store and the two counter `lhu's interleave
   instead: 52 words, 17 aligned.

   The two together are what land it; the barrier ALONE (casts kept) is
   54 words / 16 aligned, because the record load then cannot fill the
   second slot either and a second `nop' appears.

   Also load-bearing, from the park and re-confirmed here: `c - 0x20'
   written INLINE in the comparison rather than through a local (a `d'
   local is 4 differences worse at the same length), and
   `g = (u8 *)&D_80083460' assigned INSIDE the loop body immediately before
   its use -- before the loop or before the `if' it is 51 words, because
   the `lui' leaves the branch delay slot the target fills it from.  loop.c
   hoists the pair to the preheader by itself. */
void func_800A31D0(u8 *c)
{
    u8 *g;
    u8 *n;

    if (c[0x13] == 0x1E && (*(s32 *)(c + 0x14) & 0x4000) != 0 && *(s16 *)(c + 0xAA) != 0)
    {
        n = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
        while (n != D_800E3D7C)
        {
            if (n[0x13] == 0x1E && *(u8 **)(n + 0xA4) == c - 0x20)
            {
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) += 1;
                __asm__("");
                D_800DCE68 += 1;
                ((UnkDungeonA31D0 *)n)->unk_6E = 0;
                ((UnkDungeonA31D0 *)n)->unk_6D = 0;
                ((UnkDungeonA31D0 *)n)->unk_1C |= 0x400000;
            }
            n = *(u8 **)(n + 0x5C) + 0x20;
        }
    }
}

/* Retire the record `p` from the party: drop its 0x1A/0x1C/0x1D attributes,
   hand it to func_800A31D0, and if it still owns one of the four +0xAC slots,
   clear the slot, copy the live record into its D_800E3E48 shadow and point
   D_800E3DF0 at the copy -- func_80093E74 at :3068 does the same shadow copy
   from the other side. The 0/1/2/3 code the walk ends with is what
   func_800A18E8 is told, and the slot record it found is returned.

   THE RETURN VALUE IS REAL: the two `addu $v0, $s2, $zero` at the tail are
   `return r;`, so the unit's `void func_800A32A4(u8 *);` declaration had to
   become `u8 *`. All seven landed call sites drop the result and are
   unchanged.

   TWO ALLOCATION FACTS, 12 differing between them, and neither is reachable
   by moving statements.

   (1) THE TWO IDENTICAL D_80083460 ARMS NEED TWO DIFFERENT LOCALS. With one
   `g` written in both arms the base lands in $v1 and the `lhu` result in $a0,
   where the target has $a0 and $v1 -- 10 differing across the ten words. A
   second local for the second arm changes the quantity numbering and both
   arms come out right. (The base itself must still be re-assigned per arm:
   the target has one `lui` in the `beqz`'s delay slot and an `addiu %lo` in
   each arm.)

   (2) `f` IS `s16`. As `s32` the flag and `r` tie, reload_cse rewrites
   `r = NULL` into `addu $s2, $s1, $zero` and the loop's `i = 0` then copies
   $s1 instead of $s2: 2 differing. The width is invisible at the one use --
   func_800A18E8's second argument is a plain `addu $a1, $s1, $zero` either
   way.

   Measured and neutral: the decrement as `-= 1`, `--`, or written out with
   the `lhu` read spelled explicitly; `g` as `s16 *` with `g[14]`; every
   declaration order of the six locals; `r = NULL` before `f = 0` (15
   differing) and either of them moved between the func_80042B68 calls (16,
   129, 131).

   134 words, 0 differing. */
u8 *func_800A32A4(u8 *p)
{
    u8 *g;
    u8 *g2;
    u8 *r;
    s32 i;
    s32 k;
    s16 f;
    u32 w;

    f = 0;
    r = NULL;
    func_80042B68((s8 *)p, 0x1A);
    func_80042B68((s8 *)p, 0x1C);
    func_80042B68((s8 *)p, 0x1D);
    if ((*(u32 *)(p + 0x14) & 0x20000000) == 0)
    {
        func_800A31D0(p);
        for (i = 0; i < 4; i++)
        {
            if (*(u8 **)(D_800814A8 + i * 4 + 0xAC) == p)
            {
                *(u32 *)(p + 0x14) &= ~0x4000;
                *(u32 *)(D_800814A8 + i * 4 + 0xAC) = 0;
                w = *(u32 *)(p + 0x1C);
                w |= 0x4000000;
                w |= 0x20000;
                *(u32 *)(p + 0x1C) = w;
                r = *(u8 **)(D_800814A8 + i * 4 + 0xD0);
                r[3] &= 0xDF;
                f = 3;
                *(u32 *)(D_800814A8 + i * 4 + 0xD0) = 0;
                k = r[3] & 0x1F;
                D_800E3DF0[k] = (s32)&D_800E3E48[k];
                D_800E3E48[k] = *(Unk800E3E48 *)p;
                break;
            }
        }
    }
    if (f == 0)
    {
        if ((*(u32 *)(p + 0x14) & 0x4000) != 0)
        {
            f = 3;
        }
        else if ((*(u32 *)(p + 0x14) & 0x2000) != 0)
        {
            g = (u8 *)&D_80083460;
            if (*(s16 *)(g + 0x1C) != 0)
            {
                *(s16 *)(g + 0x1C) -= 1;
            }
            f = 2;
        }
        else
        {
            g2 = (u8 *)&D_80083460;
            if (*(s16 *)(g2 + 0x1C) != 0)
            {
                *(s16 *)(g2 + 0x1C) -= 1;
            }
            f = 1;
        }
    }
    if ((*(u32 *)(p + 0x14) & 0x20000000) == 0)
    {
        func_800A18E8(p[0x13], f);
    }
    return r;
}

/* Copy the twelve-byte name the D_8007359C record at +0x04 points at into
   the actor's +0x34, then drop a NUL on its last word's first byte. The
   copy's type has to have ALIGNMENT 1 -- see UnkDungeonName12 -- which is
   what gives the `lwl`/`lwr` + `swl`/`swr` pairs rather than aligned `lw`.
   23 words, 0 differing. */
void func_800A34BC(u8 *p)
{
    *(UnkDungeonName12 *)(p + 0x34) = *(UnkDungeonName12 *)D_8007359C[0][p[0x13]].unk_04;
    p[0x3C] = 0;
}

/* A capacity predicate: true once +0x29 over half of +0x11 plus two has
   caught up with +0x28.

   THE DIVISOR IS ITS OWN STATEMENT -- the target loads +0x11 before +0x29,
   which is the divisor's expression first; inline, the dividend goes first.
   And `>> 1`, not `/ 2`: the value comes out of an `lbu`, so combine knows
   the sign bit is clear and the shift stays the `srl` the target has, where
   a signed divide by two costs three words. 11 words, 0 differing. */
s32 func_800A3518(u8 *p)
{
    s32 d;

    d = (p[0x11] >> 1) + 2;
    return p[0x29] / d >= p[0x28];
}

/* func_800A35A4's record lookup as a scan over three stride-3 slots.

   TWO PREHEADER FACTS, each worth measuring: the subscript `p[i * 3 + 8]` is
   what loop.c reduces to the target's `addiu $a0, $a0, 0x3` biv, while
   walking `p += 3` myself leaves the giv's own `addiu $a0, $a0, 8` in the
   preheader and is 25 words / 24 differing; and `t` has to be written INSIDE
   the loop, because the target's preheader sets `$a2` (i) before the
   `%hi(D_8006DE24)`, which the hoist reproduces and an assignment ahead of
   the `for` does not (24 words / 3 differing, the three being just those two
   insns and the `j`). Hoisting `p[8]` into a local is 24 / 10.
   24 words, 0 differing. */
s16 func_800A3544(u8 *p, u8 k)
{
    UnkDungeonName *t;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        t = (UnkDungeonName *)D_8006DE24;
        if (p[i * 3 + 8] != 0 && t[p[i * 3 + 8]].unk_11 == k)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* func_800A3820's +0x13 read with the index taken out of a record. THE
   TABLE ARRIVES AS A POINTER LOCAL, not as a symbol added to the index:
   added, it expands SECOND and the `lui` lands after the multiply, which is
   what func_800A3820 wants and this one does not (9 differing). `p += i *
   3;` as its own statement is what keeps the update in $a0.
   13 words, 0 differing. */
u8 func_800A35A4(u8 *p, s32 i)
{
    UnkDungeonName *t;

    t = (UnkDungeonName *)D_8006DE24;
    p += i * 3;
    return t[p[8]].unk_13;
}

/* A three-bit cross-product: for each bit of `a`, the bit one place round the
   3-cycle of `b` adds and the bit one place the other way subtracts.

   THE PARAMETER WIDTH IS WORTH TWO WORDS. Declared `s32`, gcc keeps both
   parameters in $a0/$a1, the first block's `sltu` writes straight into the
   counter, and the function is 31 words. Declared sub-word, the two incoming
   copies `addu $a3, $a0, $zero` / `addu $a2, $a1, $zero` survive (a sub-word
   copy of a word register is not the no-op jump2 deletes) and all 33 words
   match. u8, s8, u16 and s16 all measure 33 / 0 here, so the callee cannot
   pick between them; u16 is what the CALLERS witness -- func_800A40AC passes
   an `lhu` result with no mask, which rules out u8, and with no `sll`/`sra`,
   which rules out s16. 33 words, 0 differing. */
s16 func_800A35D8(u16 a, u16 b)
{
    s16 r;

    r = 0;
    if (a & 1)
    {
        if (b & 4)
        {
            r++;
        }
        if (b & 2)
        {
            r--;
        }
    }
    if (a & 2)
    {
        if (b & 1)
        {
            r++;
        }
        if (b & 4)
        {
            r--;
        }
    }
    if (a & 4)
    {
        if (b & 2)
        {
            r++;
        }
        if (b & 1)
        {
            r--;
        }
    }
    return r;
}

/* Two tile positions are "in line" when their x and y distances are equal or
   either one is zero.

   BOTH abs() ARE THE `bgez`/nop/`negu` TRIO, and the spelling matters: the
   TERNARY form `dx = dx >= 0 ? dx : -dx;` writing back into its own variable
   is 22 words / 14 differing, every difference a register. Written into a
   SEPARATE variable the ternary is 0, and `abs()` inline is 0 and the same
   bytes. Computing both differences before either abs is one word SHORT: the
   second pair of `lbu` fills the first subtraction's load-delay slot, which
   the target leaves as a `nop`. 22 words, 0 differing. */
s32 func_800A365C(u8 *a, u8 *b)
{
    s32 dx;
    s32 dy;
    s32 r;

    dx = abs(a[0x24] - b[0x24]);
    dy = abs(a[0x25] - b[0x25]);
    r = 0;
    if (dx == dy || dx == 0 || dy == 0)
    {
        r = 1;
    }
    return r;
}

/* Walk the tile line from a's record to b's one step at a time and ask
   func_800A44E0 whether each step is blocked; the running ceiling at +0x88
   is lowered by whatever func_800BCB04 reports under each tile.
   91 words, 0 differing on the first compile.

   The three spellings it needed are all func_800A44E0's own, recorded at its
   landing: the third argument is `s16` and is sign-extended at the call (the
   note there names this function as one of its two witnesses), `z` is the
   `u16` read of +0x88 held in an `s16`, and the `(s16)(z - 0x20)` cast comes
   from func_800BCB04's prototype. */
s32 func_800A36B4(u8 *a, u8 *b)
{
    u8 *sa;
    u8 *sb;
    s32 dx;
    s32 dy;
    s32 x;
    s32 y;
    s16 z;
    s16 w;
    s16 lim;
    s16 h;

    sa = *(u8 **)(a - 0x14);
    sb = *(u8 **)(b - 0x14);
    w = *(u16 *)(a + 0x2A);
    dx = sa[0x24] - sb[0x24];
    if (dx != 0)
    {
        if (dx > 0)
        {
            dx = 1;
        }
        else
        {
            dx = -1;
        }
    }
    dy = sa[0x25] - sb[0x25];
    if (dy != 0)
    {
        if (dy > 0)
        {
            dy = 1;
        }
        else
        {
            dy = -1;
        }
    }
    x = sa[0x24];
    y = sa[0x25];
    z = *(u16 *)(a + 0x88);
    while (x != sb[0x24] || y != sb[0x25])
    {
        lim = z;
        if ((s16)func_800A44E0((x << 6) & 0xFFC0, (y << 6) & 0xFFC0, lim, w) != 0)
        {
            return 0;
        }
        x -= dx;
        y -= dy;
        h = func_800BCB04(((x << 6) + 0x20) & 0xFFE0, ((y << 6) + 0x20) & 0xFFE0,
                          (s16)(z - 0x20));
        if (h < lim)
        {
            z = h;
        }
    }
    return 1;
}

/* Read +0x13 out of the D_8006DE24 record; see UnkDungeonName above. The
   index is a separate statement, or fold hands the symbol to expand first
   and the `lui` comes out ahead of the multiply. 11 words, 0 differing. */
u8 func_800A3820(s16 i)
{
    s32 off;

    off = (s16)i * 0x14;
    return ((UnkDungeonName *)(off + (u8 *)D_8006DE24))->unk_13;
}

/* Pick which of the actor's three 3-byte D_8006DE24 rules it should react to
   `b` with: gate on the two +0x88 heights being within 0x40, aim through
   func_800A0818, latch the +0x14/+0x98 "noticed" flags once, then scan the
   three slots from D_8008347E's low two bits, keeping the rule whose
   func_800A35D8 score beats the best so far -- and, for rule kind 1, only if
   an eight-way walk of `dist` steps from the actor reaches `b` without
   crossing a blocking tile. 307 words, 0 differing.

   EVERY FAILED TEST IS A `continue`, AND THE TAIL IS WRITTEN ONCE. The
   target's two dispatch arms both reach `bestt = t; best = i + 1;` at
   .L800A3C40, and the copy in the `j`'s delay slot is reorg's
   fill_slots_from_thread, not a second source statement. Written with the
   tail duplicated in both arms the extra reference lifts `t`'s allocno above
   `dist`'s, `t` takes a callee-saved register, `dist` spills to the frame and
   the function is 315 words / 126 aligned differences; with the shared tail
   it is 307 / 11.

   THE FIRST PARAMETER IS COPIED INTO A LOCAL USED FOR -0x14 AND +0x98 ONLY.
   The target loads 0x68($sp) twice in a row, into $t1 for the +0x88 read and
   into $s1 for `*(u8 **)(h - 0x14)` and the five +0x98 accesses; all the
   other uses reload from the home slot. Without the local the parameter wins
   $fp outright, `e` and `dist` both spill and it is 212 aligned differences
   against 163.

   THE FLAG WORD MUST BE A LOCAL DECLARED **BEFORE** `mm`, AND THAT ONE LINE
   IS THE LAST THREE WORDS. `fl2`, `mm` and `w` all measure refs 9 / length 4,
   so `allocno_compare` ties on all three and global.c falls through to
   `return v1 - v2` -- the allocno INDEX, which follows the pseudo number,
   which follows DECLARATION ORDER. Read as `*(u32 *)(a + 0x1C)` twice the
   value is a cse-created pseudo (242 here) and loses the tie to `mm` (94), so
   `mm` is allocated first and takes $v1; `fl2` declared above `mm` wins the
   tie, takes $v1, and `mm` gets the target's $a0. Thirty-one other spellings
   -- every local type, every declaration position, block scope, swapped arms,
   ternaries, a pointer read -- all sit at exactly 3 aligned differences.
   `fl = fl2 & 0x2000;` must also precede `mm = m;`, or the `lh` is scheduled
   ahead of the `andi` and reorg fills the gate's delay slot from the wrong
   thread (10 differences).

   `kind` IS AN `s32` LOCAL, WORTH ONE OPCODE. With `d->unk_12` read directly
   at the `>= 4` test combine folds the `lbu`'s zero-extension into the
   comparison and emits `sltiu`; the target has the signed `slti`.

   `j` IS `s16` AND STARTS AT `n`. The guard is `slt $s3, $fp` with no
   sign-extension because combine proves `n` already has 17 sign bits (it is
   `(s16)func_80042900(...)`), while the bottom test re-extends; `j = 0` gives
   `blez $fp` and `s32 j` loses the two `sll`/`sra` pairs. */
s32 func_800A384C(u8 *a, u8 *b, u8 *c, s32 s)
{
    UnkDungeonName10 *d;
    u8 *h;
    u8 *e;
    u8 *f;
    u8 *g;
    s16 m;
    s32 k;
    s32 n;
    s16 j;
    s16 w;
    s32 fl2;
    s32 mm;
    s32 fl;
    s32 off;
    s32 kind;
    s32 dist;
    s16 best;
    s16 bestt;
    s16 ang;
    s16 i;
    s16 t;
    s16 x;
    s16 y;
    s16 dir;

    best = -1;
    bestt = -0x100;
    *(s16 *)c = *(u16 *)(a + 0x2A);
    h = a;
    e = *(u8 **)(h - 0x14);
    f = *(u8 **)(b - 0x14);
    if (abs(*(s16 *)(a + 0x88) - *(s16 *)(b + 0x88)) >= 0x40)
    {
        return -1;
    }
    ang = func_800A365C(e, f);
    dist = func_8009FD40(f, e);
    *(s16 *)c = func_800A0818(e[0x24], e[0x25], f[0x24], f[0x25], (u16 *)&m);
    if (s == 0 && dist == 1)
    {
        if (func_8009A540((u16)(*(s16 *)c >> 9), e[0x24], e[0x25], *(u16 *)(a + 0x88) - 0x20) != 0)
        {
            return 8;
        }
    }
    if ((*(u32 *)(a + 0x14) & 0x10000) == 0)
    {
        *(u32 *)(a + 0x14) |= 0x10000;
        if (func_8009FD40(D_80082E80, e) < 0x11 &&
            ((*(u32 *)(a + 0x14) & 0x6000) != 0 || (*(u16 *)(h + 0x98) & 0x10) == 0 ||
             (func_800A6D30() & 0xF) == 0) &&
            (func_800A6D30() & 3) == 0 && (s16)func_80042900((s8 *)a, 6) == 0)
        {
            *(u16 *)(h + 0x98) |= 0x11;
        }
        else
        {
            *(u16 *)(h + 0x98) &= 0xFFFE;
        }
    }
    if (*(u16 *)(h + 0x98) & 1)
    {
        i = D_8008347E & 3;
        for (k = 0; k < 3; k++, i++)
        {
            if (i == 3)
            {
                i = 0;
            }
            g = a + i * 3;
            if (g[8] == 0)
            {
                continue;
            }
            off = g[8] * 0x14;
            d = (UnkDungeonName10 *)(off + (u8 *)D_8006DE24);
            if (d->unk_11 >= 0x12)
            {
                continue;
            }
            n = (s16)func_80042900((s8 *)b, d->unk_11);
            if (n != 0)
            {
                continue;
            }
            if (d->unk_13 < dist)
            {
                continue;
            }
            t = func_800A35D8(d->unk_10, *(u16 *)(a + 0x14));
            if (bestt >= t)
            {
                continue;
            }
            kind = d->unk_12;
            if (kind != 1)
            {
                if (kind == 0)
                {
                    continue;
                }
                if (kind >= 4)
                {
                    continue;
                }
            }
            else
            {
                if (ang == 0)
                {
                    continue;
                }
                x = e[0x24];
                y = e[0x25];
                dir = (*(u16 *)c >> 9) & 7;
                for (j = n; j < dist; j++)
                {
                    func_8009A350(x, y, dir, (u16 *)&m);
                    fl2 = *(u32 *)(a + 0x1C);
                    if ((fl2 & 0x410) == 0)
                    {
                        mm = m;
                        fl = fl2 & 0x2000;
                        if (fl != 0)
                        {
                            w = mm & 0x300;
                        }
                        else
                        {
                            w = mm & 0x3000;
                        }
                        if (w != 0)
                        {
                            break;
                        }
                    }
                    x += (u16)D_8006CCD8[dir];
                    y += (u16)D_8006CCE8[dir];
                }
                if (j < dist)
                {
                    continue;
                }
            }
            bestt = t;
            best = i + 1;
        }
    }
    if (s != 0 && dist == 1)
    {
        if (func_8009A540((u16)(*(s16 *)c >> 9), e[0x24], e[0x25], *(u16 *)(a + 0x88) - 0x20) != 0)
        {
            if (best < 0)
            {
                best = 8;
            }
        }
    }
    return best;
}

/* Pick the ring member `a` should react to: walk `b`'s ring, keep the nearest
   candidate that func_800A2CB8 and func_800A41F0 both admit, and score it two
   ways -- same `+0x26` class members by func_8009FD40's distance, different
   ones by func_8009FE94's line-of-sight test, which breaks the walk. Two
   candidates are tracked, `r4` for the one that IS the resident's current
   actor (D_800E3D7C) and `r3` for everything else, and the tail chooses
   between them.

   FOUR MEASURED FACTS.

   (1) `b` IS THE WALKER, as in func_8009B25C. `h = b; b = *(u8 **)(b + 0x5C)
   + 0x20;` gives the target's `addu $s0, $a1, $zero` / `addu $s2, $s0, $zero`
   pair; a separate `p` local for the walk and `b` kept as the head is 130
   words of 132 and 129 differing, because the parameter's own prologue copy
   disappears.

   (2) THE TAIL IS ONE `return r3` WITH FOUR `r3 = r4` SITES, not seven
   returns. Written as early `return r3` / `return r4` statements it is 132
   words and 35 differing: cross-jumping then produces a bare `addu $v0, $s4`
   where the target has `.L800A3EF0: addu $s3, $s4` falling into
   `.L800A3EF4: addu $v0, $s3`, and reorg copies that second insn into every
   early exit's delay slot.

   (3) THE `d6 == d1` ARM MUST BE WRITTEN BEFORE THE `d6 < d1` ONE. With
   `else if (d6 != d1) { if (d6 < d1) ... }` first, the equal case goes out of
   line and the compare comes out `beq` where the target has `bne`:
   17 differing at the same 132 words.

   (4) func_8009FE94 TAKES SIX ARGUMENTS HERE, not the five its landed
   definition declared -- the sixth is `*(s16 *)(b + 0x88)`, stored at
   0x14($sp), and the frame's 0x18 start proves the outgoing area is six words
   wide. The definition was widened with an unread sixth `s16`; it still
   measures 80 words, 0 differing, since an unread parameter costs nothing.

   132 words, 0 differing. */
u8 *func_800A3D18(u8 *a, u8 *b, s16 c)
{
    u8 *h;
    u8 *e;
    u8 *r4;
    u8 *r3;
    s16 d6;
    s16 d1;
    s16 t;

    r3 = NULL;
    r4 = r3;
    d6 = 0x100;
    h = b;
    if (c < 0)
    {
        d1 = -c;
    }
    else if (*(s8 *)(a + 0x26) < 0)
    {
        d1 = c;
    }
    else
    {
        d1 = 0x100;
    }
    b = *(u8 **)(b + 0x5C) + 0x20;
    if (b != h)
    {
        do
        {
            if ((s16)func_800A2CB8(h, b) != 0 && (s16)func_800A41F0(b) != 0)
            {
                e = *(u8 **)(b - 0x14);
                if (*(s8 *)(a + 0x26) == *(s8 *)(e + 0x26))
                {
                    t = func_8009FD40(e, a);
                    if (t < d1)
                    {
                        if (b == D_800E3D7C)
                        {
                            r4 = b;
                            d6 = t;
                        }
                        else
                        {
                            r3 = b;
                            d1 = t;
                        }
                    }
                }
                else if ((s16)func_8009FE94(a[0x24], a[0x25], *(s16 *)(h + 0x88), e[0x24], e[0x25],
                                            *(s16 *)(b + 0x88)) != 0)
                {
                    if (b == D_800E3D7C)
                    {
                        r4 = b;
                        d6 = 1;
                    }
                    else
                    {
                        r3 = b;
                        d1 = 1;
                    }
                    break;
                }
            }
            b = *(u8 **)(b + 0x5C) + 0x20;
        } while (b != h);
    }
    if (r4 != NULL)
    {
        if (r3 == NULL)
        {
            r3 = r4;
        }
        else if (d6 == d1)
        {
            if (*(u32 *)(r3 + 0x1C) & 8)
            {
                r3 = r4;
            }
            else if (((*(u32 *)(D_800E3D7C + 0x128) + b[0x43]) & 3) == 0)
            {
                r3 = r4;
            }
        }
        else if (d6 < d1)
        {
            r3 = r4;
        }
    }
    return r3;
}

/* Walk the ring from q looking for a neighbour that is either in the same
   room as (x, y) or standing within one tile of it; func_8009B25C is the
   same walk against a facing instead of a room. 73 words, 0 differing.

   THREE SPELLINGS WERE MEASURED, and two of them are not guessable.
   (1) The comparison is `r == e[0x26]`, NOT `e[0x26] == r`: with the operands
   the other way round every callee-saved register rotates by one -- 24
   differing with the whole structure already exact -- because the HImode
   extension of `r` is born in the other order.
   (2) `r` IS `s16`, so the call result's `sll`/`sra` sits at the first INT
   use, inside the `if`, where reorg can take the `sll` into the `beq`'s
   delay slot. As `s32` the extension is at the assignment and lands ahead of
   the `lw` -- 5 differing, no word change.
   (3) THE 1 IS A VARIABLE. `abs(d) > 1` folds to `d >= 2` and comes out as
   `slti $v0, $v0, 2`; the target's `slt $v0, $s2, $v0` needs the constant in
   a register, which only a local gives, and the local must be `s16`: as
   `s32` it takes a seventh callee-saved register and the function is 75
   words. */
u8 *func_800A3F28(u16 x, u16 y, u8 *h, u8 *q)
{
    u8 *p;
    u8 *e;
    s16 r;
    s16 lim;
    s16 xx;
    s16 yy;
    s32 d;

    r = func_8009FB34(x, y);
    p = *(u8 **)(q + 0x5C) + 0x20;
    if (p != h)
    {
        xx = x;
        lim = 1;
        yy = y;
        do
        {
            if (func_800A41F0(p) != 0)
            {
                e = *(u8 **)(p - 0x14);
                if (r == *(s8 *)(e + 0x26) && r >= 0)
                {
                    return p;
                }
                d = xx - e[0x24];
                d = abs(d);
                if (d <= lim)
                {
                    d = yy - e[0x25];
                    d = abs(d);
                    if (d <= lim)
                    {
                        return p;
                    }
                }
            }
            p = *(u8 **)(p + 0x5C) + 0x20;
        } while (p != h);
    }
    return NULL;
}

/* Is either of the engine's two slots holding a record whose +0x46 is flagged
   and whose low 14 bits sit in 5..7?

   THE RANGE TEST MUST BE PARENTHESISED. Written
   `(t & 0x8000) && (t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7` the `&&` chain
   associates left, fold never sees the two bounds as one range, and it is 25
   words against 24 with 15 differing -- two `sltiu`s instead of the target's
   `addiu -5`/`sltiu 3`. 24 words, 0 differing. */
s32 func_800A404C(void)
{
    s32 i;
    u8 *p;
    u8 *q;
    u16 t;

    for (i = 0, p = (u8 *)D_800814A8; i < 2; i++, p += 4)
    {
        q = *(u8 **)(p + 0xAC);
        if (q != NULL)
        {
            t = *(u16 *)(q + 0x46);
            if ((t & 0x8000) && ((t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7))
            {
                return 1;
            }
        }
    }
    return 0;
}

/* Pick the best of the record's three 3-byte slots, starting from
   D_8008347E's low two bits and wrapping at 3; the winner is the one with
   the highest +0x09, ties broken by func_800A35D8's score, and the answer
   is the slot index plus 5. 81 words, 0 differing.

   `q[9]` IS READ BEFORE THE CALL. It is live across it in $s0, and gcc will
   not move a load across a call, so reading it after is 11 differing. The
   two score locals are `s16`, which is what gives the tie-break its
   `sll`/`sll`/`slt` with no `sra`. */
s16 func_800A40AC(u8 *p, u16 arg)
{
    s32 n;
    s16 i;
    s16 best;
    s16 bestt;
    s16 ans;
    s16 t;
    s16 q9;
    u8 *q;

    ans = -1;
    bestt = -4;
    best = 0;
    i = D_8008347E & 3;
    for (n = 0; n < 3; n++, i++)
    {
        if (i == 3)
        {
            i = 0;
        }
        q = p + i * 3;
        if (q[8] != 0)
        {
            q9 = q[9];
            t = func_800A35D8(D_8006DE24[q[8]].unk_10, arg);
            if (best < q9)
            {
                best = q9;
                ans = i + 5;
                bestt = t;
            }
            else if (best == q9 && bestt < t)
            {
                bestt = t;
                ans = i + 5;
            }
        }
    }
    return ans;
}

/* Can this actor act? THE LAST TEST IS NARROW: `sll $v0, 16` / `sltiu $v0, 1`
   is the low halfword of func_80042900's result compared against zero, so the
   result takes an `(s16)` cast at the point of use rather than a changed
   prototype. 42 words, 0 differing. */
s16 func_800A41F0(u8 *p)
{
    u8 *q;
    u16 r;

    if (p[0x28] + *(s16 *)(p + 0x64) <= 0)
    {
        return 0;
    }
    if (*(s32 *)(p + 0x1C) & 0x2000)
    {
        q = *(u8 **)(p - 0x14);
        func_8009A350(q[0x24] - 1, q[0x25], 0, &r);
        if (r & 2)
        {
            return 0;
        }
    }
    if (*(s32 *)(p + 0x1C) & 0x290000)
    {
        return 0;
    }
    return (s16)func_80042900((s8 *)p, 0x1B) == 0;
}

/* Find the entry of the 0x80010248 record array whose +1 byte is 0x13 and
   whose +3 byte indexes a D_800E3DF0 word equal to the key; return that
   index. THE CURSOR MUST BE A GIV, not a pointer walked from a statement
   before the loop: as `q = (u8 *)0x8001024B;` ahead of the `for` it is a
   SOURCE statement and sits at the head of the preheader, where the target
   emits it LAST, after the counter, the 0x13 and the table base -- 26 words
   at 5 differing, all of them preheader order. 26 words, 0 differing. */
s32 func_800A4298(s32 key)
{
    s32 i;
    u8 *q;
    s32 k;

    for (i = 0; i < 0x14; i++)
    {
        q = (u8 *)0x8001024B + i * 4;
        if (q[-2] == 0x13)
        {
            k = q[0] & 0x1F;
            if (D_800E3DF0[k] == key)
            {
                return k;
            }
        }
    }
    return -1;
}

/* Pick what the cursor is standing on. The `sw $zero, 0xF0($v1)` tail is
   shared by the first and last arms, which is cross-jumping putting the
   duplicated statement back. 75 words, 0 differing. */
void func_800A4300(u8 *a, u8 *b)
{
    s16 r;
    s16 i;
    s16 j;

    r = func_800A4474(a[0x24], a[0x25]);
    if (r != 0)
    {
        *(u16 *)D_80081470 = r | 0x1400;
        *(u32 *)(D_800814A8 + 0xF0) = 0;
    }
    else
    {
        i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
        if (i >= 0)
        {
            if ((D_800E3648_s[i].unk_03 & 0x40) == 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3648_s[i];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3648_s[i];
            }
            else
            {
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
        else
        {
            j = func_800A70E4(a[0x24], a[0x25], *(s16 *)(b + 0x88));
            if (j >= 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3548_s[j];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3548_s[j];
            }
            else
            {
                *(u32 *)D_80081470 = 0;
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
    }
}

/* 18 words, 0 differing. */
u8 *func_800A442C(void)
{
    u8 *r;

    if (D_80081470[1] < 0x14)
    {
        r = *(u8 **)(D_800814A8 + 0xF0);
        if (r != NULL)
        {
            return r;
        }
        *(s32 *)D_80081470 = 0;
    }
    return D_80081470;
}

/* Find (x, y) among the four D_800E3CD8 marks. SUBSCRIPT THE STRUCT ARRAY;
   DO NOT WALK A `s16 *`: the walk makes TWO address givs -- a base walked by
   8 and a second biased by 4, read at -2 and 0 -- and the counter moves to
   $t0, 29 words / 29 differing. 27 words, 0 differing. */
s32 func_800A4474(s16 x, s16 y)
{
    s32 i;
    s16 n;

    for (i = 0; i < 4; i++)
    {
        n = D_800E3CD8[i].unk_00;
        if (n < 2)
        {
            return 0;
        }
        if (x == D_800E3CD8[i].unk_02 && y == D_800E3CD8[i].unk_04)
        {
            return n;
        }
    }
    return 0;
}

/* TWO ALLOCATION FACTS, and nothing else in the function is free. The
   residue of the obvious spelling is 11 to 23 differing with every opcode,
   immediate and displacement already right and only $s1/$s2/$s3 permuted.

   (1) THE TWO SUMS GO THROUGH `u16` LOCALS, so the truncation happens at the
   ASSIGNMENT and not inside the argument expression. Written inline, `u`
   scores 3 refs over 24 insns against `v`'s 3 over 30 and takes the lower
   register, which is the wrong way round; through the two `u16` temporaries
   the lengths become 28 and 26 and `v` wins. Hoisting the same sums into
   `s32` temporaries changes NOTHING -- cse folds them back -- so it is the
   TYPE of the temporary, not the statement split.

   (2) THE THIRD PARAMETER IS `s16`, and that is what puts it in $s1: as s32
   it is live over 34 insns and scores 0.059, below both coordinates; as s16
   the pseudo is live over 17 and scores 0.118. Two callers confirm it --
   func_800A36B4 and func_800A05A4 both sign-extend $a2 immediately before the
   `jal`. `dir` is the other way: func_800A05A4 passes `$t0 << 9` with no
   truncation, so it is u32. 62 words, 0 differing. */
s32 func_800A44E0(u16 x, u16 y, s16 c, u32 dir)
{
    s16 t;
    s32 u;
    s32 v;
    s32 i;
    u16 aa;
    u16 bb;

    u = x >> 6;
    v = y >> 6;
    i = (dir >> 9) & 7;
    if (func_8009A350(u, v, i, (u16 *)&t) == 0)
    {
        return 0;
    }
    if ((t & 0x8000) != 0)
    {
        return 1;
    }
    aa = ((u16 *)D_8006CCD8)[i] + u;
    bb = ((u16 *)D_8006CCE8)[i] + v;
    t = func_800BCB04((((s32)aa << 6) + 0x20) & 0xFFE0, (((s32)bb << 6) + 0x20) & 0xFFE0,
                      c - 0x20);
    return t > 0x200;
}

/* Is the tile ahead blocked? THE TWO TILE INDICES ARE `u16` LOCALS. As s32
   the shift lands straight in the callee-saved register and the function is
   43 words against 44 with 41 differing; an HImode pseudo cannot be the
   `srl`'s destination, so the shift goes to a temp and the copy into the
   callee-saved register is the missing word. 44 words, 0 differing. */
s16 func_800A45D8(u16 x, u16 y, s32 c)
{
    u16 tx;
    u16 ty;
    s16 r;

    tx = x >> 6;
    ty = y >> 6;
    if (func_8009A350(tx - 1, ty, 0, (u16 *)&r) == 0)
    {
        return 0;
    }
    if (r & 0x8000)
    {
        return 1;
    }
    r = func_800BCB04((tx << 6) | 0x20, (ty << 6) | 0x20, (s16)(c - 0x20));
    return r > 0x200;
}

/* Is the tile one step along `ang` from (px, py) blocked? func_8009B164 is
   the same probe one record out and its three spellings transfer.

   THREE THINGS ARE SPECIFIC. (1) `face` is a NARROW parameter: as s32 with
   the `(s16)` cast at the call it is 60 words and 11 differing, every
   difference a callee-saved register -- the sign-extension of a 16-bit
   parameter is one more reference on its allocno, which lifts it past
   `gx`/`gy` in global-alloc's priority. Nothing calls this function anywhere
   in the project, so the width is unwitnessed. (2) `k` must be s16 as well:
   as s32 the index is computed straight into $s0 and copied to $a2 at the
   call, where the target computes it into $a2 and copies it into $s0, 17
   differing. (3) The `+ 0x20` must be its own set of an ALREADY-SET variable:
   in one expression combine proves the low bits clear and turns the add into
   `ori`, 21-32 differing. 60 words, 0 differing. */
s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)
{
    u16 m;
    s32 gx;
    s32 gy;
    s16 k;
    u16 u0;
    u16 u1;
    s32 x;
    s32 y;
    s16 r;

    gx = px >> 6;
    gy = py >> 6;
    k = (ang >> 9) & 7;
    if (flag == 0)
    {
        if ((s16)func_8009A350(gx, gy, k, &m) != 0)
        {
            u0 = (u16)D_8006CCD8[k] + gx;
            u1 = (u16)D_8006CCE8[k] + gy;
            x = u0 * 64;
            x = (x + 0x20) & 0xFFE0;
            y = u1 * 64;
            y = (y + 0x20) & 0xFFE0;
            r = func_800BCB04(x, y, (s16)(face - 0x20));
            m = r;
            return r > 0x200;
        }
    }
    return 0;
}

/* func_800A45D8's probe one tile further: when the tile west of (x, y) is
   clear and its centre is above 0x200, nudge the sample point by a tile in
   whichever axis is more than 0x15 off centre and probe again.

   FOUR TYPE AND SPELLING FACTS, all measured:
   (1) `gx`/`gy` ARE `u16`, exactly as in func_800A45D8 -- as `s32` the `srl`
   lands straight in the callee-saved register and the two `addu` copies the
   target has go missing: 89 words of 94, 79 differing.
   (2) `nx`/`ny` ARE `u16` TOO. As `s32` the second call's two arguments come
   out `andi 0xFFE0` instead of `andi 0xFFFF`, because combine can still see
   that `(gx << 6) + 0x20` has five clear low bits, and the two copies into
   the callee-saved registers disappear again: 90 words, 68 differing.
   (3) THE X STEP GOES THROUGH A TEMPORARY AND THE Y STEP DOES NOT -- that
   asymmetry is in the target, not a guess. Spelled the same way as the x step
   the y block loses its out-of-line zero and its `j`: 93 words, 32 differing.
   Spelled the same way as the y step the x block loses the `addu $a0, $v0`
   copy at its join: 93 / 45.
   (4) THE Y STEP STILL HAS TO PASS THROUGH `t`, as the assignment inside the
   conditional expression. Without it the y argument is `andi 0xFFE0` against
   the target's `andi 0xFFFF` -- one word, and the only difference left after
   everything above. Measured and rejected at 1 differing: both plain-`if` and
   plain-`?:` forms of the y step, `u16`/`s16`/`s8` for `sy`, `(u16)(sy + ny)`,
   `(ny + sy) & 0xFFFF`, `(u16)(ny + (u16)sy)`, a second temporary `u` holding
   the ternary, and `u16` locals for the two sums (3 differing). An `if/else`
   with `t` in the else arm is 6. 94 words, 0 differing. */
s32 func_800A4778(u16 x, u16 y, s16 c, s32 flag)
{
    u16 gx;
    u16 gy;
    u16 nx;
    u16 ny;
    s32 dx;
    s32 dy;
    s32 t;
    s32 sx;
    s32 sy;
    s16 r;

    if (flag == 0)
    {
        gx = x >> 6;
        gy = y >> 6;
        if (func_8009A350(gx - 1, gy, 0, (u16 *)&r) != 0)
        {
            nx = gx * 64;
            nx = nx + 0x20;
            ny = gy * 64;
            ny = ny + 0x20;
            r = func_800BCB04(nx & 0xFFE0, ny & 0xFFE0, (s16)(c - 0x20));
            if (r > 0x200)
            {
                dx = (s16)(nx - x);
                sx = 0;
                if (abs(dx) >= 0x16)
                {
                    if (dx > 0)
                    {
                        t = -0x40;
                    }
                    else
                    {
                        t = 0x40;
                    }
                    sx = t;
                }
                dy = (s16)(ny - y);
                sy = abs(dy) < 0x16 ? 0 : (t = dy > 0 ? -0x40 : 0x40);
                r = func_800BCB04((u16)(nx + sx), (u16)(ny + sy), (s16)(c - 0x20));
                return r > 0x200;
            }
        }
    }
    return 0;
}

/* The four-slot status table at +0x2C: refresh an existing entry of the same
   kind, otherwise take the first free slot, otherwise evict the weakest one
   and retry. The recursion is the retry, and the engine's own declaration of
   this function lives in include/structs.h.

   THE XOR IS ON `(u8)c`, NOT ON `c`. The sign-extended copy of c is already
   live in $a2 for the `c == 0` test, so `(c ^ byte) & 0x80` reuses it and the
   whole register assignment shifts behind it -- 67 differing, with p/kind/c
   landing in $s2/$s4/$s5 where the target has $s3/$s5/$s2. The target xors
   the RAW parameter out of its callee-saved home. `(u8)c` and
   `(c ^ *(s8 *)...)` both give 50; `(u32)c` stays at 67.

   AND THE LAST TEST IS `if (best > kind) { evict; retry; } return -1;`.
   Written as the early return -- `if (kind >= best) return -1;` -- gcc lays
   the `return -1` out at the END of the function instead of in the branch's
   shadow, the two loops' `func_800A4B88(p, kind); return i;` tails then
   cross-jump into one, and it is 46 differing; `best <= kind` fixes the
   `slt`'s operand order but not the layout, and `kind < best` as the if test
   is 5. The `else` forms of both are 46 and 0. 107 words, 0 differing. */
s16 func_800A48F0(s8 *p, s8 kind, s8 c)
{
    s32 i;
    s16 best;
    s8 v;

    best = 0x100;
    for (i = 3; i >= 0; i--)
    {
        if (*(s8 *)(p + i * 2 + 0x2C) == kind)
        {
            if ((((u8)c ^ *(u8 *)(p + i * 2 + 0x2D)) & 0x80) != 0 || c == 0)
            {
                func_80042BDC(p, *(s8 *)(p + i * 2 + 0x2C));
                *(s8 *)(p + i * 2 + 0x2C) = 0;
                *(s8 *)(p + i * 2 + 0x2D) = 0;
            }
            else
            {
                *(s8 *)(p + i * 2 + 0x2D) = c;
            }
            func_800A4B88(p, kind);
            return i;
        }
    }
    for (i = 3; i >= 0; i--)
    {
        v = *(s8 *)(p + i * 2 + 0x2C);
        if (v == 0)
        {
            if (kind == 1)
            {
                func_80042B68(p, 4);
            }
            else if (kind == 4)
            {
                func_80042B68(p, 1);
            }
            *(s8 *)(p + i * 2 + 0x2C) = kind;
            *(s8 *)(p + i * 2 + 0x2D) = c;
            func_800A4B88(p, kind);
            return i;
        }
        if (v < best)
        {
            best = v;
        }
    }
    if (best > kind)
    {
        func_80042B68(p, best);
        return func_800A48F0(p, kind, c);
    }
    return -1;
}

/* Four 2-byte records scanned DOWNWARD from +0x32; the counter and the -2
   cursor are the target's `bgez`/`addiu -2` pair. 12 words, 0 differing. */
s32 func_800A4A9C(u8 *p)
{
    s32 i;

    for (i = 3; i >= 0; i--)
    {
        if (*(s8 *)(p + i * 2 + 0x2C) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Run the four two-byte slots at +0x2C/+0x2D down from the top.

   THE SUBSCRIPT IS WRITTEN AT EVERY SITE, NOT WALKED. `s = p + 6;` with
   `s -= 2` in the loop's third clause folds the +0x2C into the biv's
   initialiser, so the cursor starts at p + 0x32 and the two accesses come out
   at displacements 0 and 1 where the target keeps p + 6 and +0x2C/+0x2D: 47
   words, 8 differing, every one a displacement. The degenerate `bgez` after
   the `blez` is the `else if (d < 0)` arm's own test, not a redundancy.
   47 words, 0 differing. */
void func_800A4ACC(u8 *p)
{
    s32 i;
    s32 c;
    s32 d;
    u8 v;

    for (i = 3; i >= 0; i--)
    {
        c = *(s8 *)(p + i * 2 + 0x2C);
        if (c != 0)
        {
            if (c == 5)
            {
                func_800A5DFC(p, *(s32 *)(p - 0x18));
            }
            d = *(s8 *)(p + i * 2 + 0x2D);
            if (d != 0)
            {
                if (d > 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))--;
                }
                else if (d < 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))++;
                }
                if (*(s8 *)(p + i * 2 + 0x2D) == 0)
                {
                    v = *(u8 *)(p + i * 2 + 0x2C);
                    *(u8 *)(p + i * 2 + 0x2C) = 0;
                    func_80042BDC((s8 *)p, (s8)v);
                }
            }
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089000);

/* The status-kind dispatcher func_800A48F0 calls after it writes a slot:
   twelve of twenty-seven kinds set a bit in the view's +0x1C flags, kinds
   11/12/27 flash the tile through func_8009A3D0/func_8009A21C with the
   0x2000 bit choosing 0x300 or 0x3000, and kind 10 is the petrify path --
   func_8003E188(0x2E, 0), then either func_800ACB98 and an early return or
   func_800D8590 plus a full re-skin from D_800DD274 and a four-word clear of
   D_80083110. Every other kind falls straight through to func_80041E70(p).
   The jump table is jtbl_80089008 at rodata offset 0x8A8, 0 mod 8, so
   cc1psx's `.align 3` is a no-op.

   `s16 kind` IS THE SWITCH SUBJECT'S OWN TYPE, not a cast: the listing biases
   first (`addiu $a1, -1`) and sign-extends after (`sll 16`/`sra 16`), which
   is what `stmt.c` does when index_type is HImode -- docs/matching-levers.md,
   "A `switch` ON A `short` SUBJECT PUTS THE SIGN EXTENSION *AFTER* THE RANGE
   BIAS". func_800A48F0 passes an `s8` and re-measures at 107 words /
   0 differing against the forward declaration, so the narrowing costs its
   caller nothing.

   D_80082E80 NEEDS A POINTER LOCAL. Written out at both the +0x2C store and
   the func_8003DB94 call it is still 169 words but 7 differing: the
   `addiu %lo` is biased by +0x2C for the store and then un-biased with
   `addiu $a0, $a0, -0x2C` for the call, where the target keeps the bare
   symbol in $a0 and puts 0x2C in the store's displacement.
   169 words, 0 differing. */
void func_800A4B88(s8 *p, s16 kind)
{
    void **t;
    u8 *c;
    u8 *g;
    s32 i;

    switch (kind)
    {
    case 1:
        *(u32 *)(p + 0x1C) |= 0x200;
        break;
    case 2:
        *(u32 *)(p + 0x1C) |= 0x400;
        break;
    case 3:
        *(u32 *)(p + 0x1C) = (*(u32 *)(p + 0x1C) & ~0x20000) | 0x10;
        break;
    case 4:
        *(u32 *)(p + 0x1C) |= 0x20;
        break;
    case 5:
        *(u32 *)(p + 0x1C) |= 0x40;
        break;
    case 6:
        *(u32 *)(p + 0x1C) |= 0x80;
        break;
    case 8:
        *(u32 *)(p + 0x1C) |= 0x800;
        break;
    case 9:
        *(u32 *)(p + 0x1C) |= 0x1000;
        break;
    case 27:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 11:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *)(p + 0x1C) |= 0x2000;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 12:
        c = *(u8 **)(p - 0x14);
        func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(u32 *)(p + 0x1C) &= ~0x2000;
        func_8009A21C(c[0x24], c[0x25], (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        break;
    case 10:
        func_8003E188(0x2E, 0);
        if ((*(u32 *)(p + 0x14) & 0x20000000) != 0)
        {
            func_800ACB98((u8 *)p, *(u8 **)(p - 0x18), *(u8 **)(p - 0x14), (u8 *)p);
            return;
        }
        func_800D8590(p);
        if (*(u8 *)(p + 0x13) == 0)
        {
            func_80096088((UnkDungeonActor *)p, (u8 *)p);
            g = D_80082E80;
            t = D_800DD274;
            *(void ***)(g + 0x2C) = t;
            func_8003DB94(g, t[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
            for (i = 3; i >= 0; i--)
            {
                D_80083110[i] = 0;
            }
        }
        break;
    }
    func_80041E70((u8 *)p);
}

/* Pick a random free tile: 256 tries at a random D_800E2970 room, then a
   full 1..0x3E scan, then a range check on whatever the caller already had.
   Returns the room index on the random hit and 0 otherwise. 136 words,
   0 differing.

   BOTH HITS ARE `goto`s TO BLOCKS WRITTEN AFTER THE `return 0`. The two
   bodies are `*x = cx; *y = cy; return <n>;` and cross-jumping merges their
   tails at .L800A5000, which lets reorg steal `addu $v0, $s3, $zero` into
   the first loop's own `beqz` delay slot. Written as inline bodies inside
   the loops the merge still happens but the branch cannot be inverted over
   the `v0 = room` insn, and it is 135 words / 102 differing.

   THE TWO DRAWS COME FIRST AND THE TWO BASES ARE ADDED AFTER:
   `cx = rand % e->unk_04; cy = rand % e->unk_06; cx += e->unk_00;
   cy += e->unk_02;`. Folded into one expression each, the `+ unk_00` fills
   the second `jal`'s delay slot where the target has a `nop` -- 135 words,
   one short. Two extra `rx`/`ry` locals instead are 134 words, because the
   `%`'s HImode result then costs an `andi 0xFFFF` at the assignment.
   `(u32)func_800A6D30() % ...` is what makes those two divides `divu`; the
   room's `% *(s16 *)(g + 0x6)` is signed and stays `div`.

   `i = 0x100;` IS ITS OWN STATEMENT AHEAD OF `g = (u8 *)D_80081468;`. As the
   `for`'s init the preheader emits D_80081468's `lui`/`addiu` first and it
   is 5 differing; `for (i = 0x100, g = ...; ...)` measures identically to
   this spelling. */
s32 func_800A4E2C(u8 *x, u8 *y)
{
    u8 *g;
    s16 room;
    s16 cx;
    s16 cy;
    s32 i;
    u16 w;

    i = 0x100;
    g = (u8 *)D_80081468;
    for (; i > 0; i--)
    {
        room = func_800A6D30() % *(s16 *)(g + 0x6);
        if (D_800E2970_s[room].unk_0A != 0)
        {
            cx = (u32)func_800A6D30() % D_800E2970_s[room].unk_04;
            cy = (u32)func_800A6D30() % D_800E2970_s[room].unk_06;
            cx += D_800E2970_s[room].unk_00;
            cy += D_800E2970_s[room].unk_02;
            if ((s16)func_8009A350(cx - 1, cy, 0, &w) != 0 && (w & 0xF720) == 0)
            {
                goto found;
            }
        }
    }
    for (cy = 1; cy < 0x3F; cy++)
    {
        for (cx = 1; cx < 0x3F; cx++)
        {
            if ((s16)func_8009A350(cx - 1, cy, 0, &w) != 0 && (w & 0xF720) == 0)
            {
                goto found_scan;
            }
        }
    }
    if ((u32)(*x - 1) >= 0x3E || (u32)(*y - 1) >= 0x3E)
    {
        goto bad;
    }
    return 0;
found_scan:
    *x = cx;
    *y = cy;
    return 0;
found:
    *x = cx;
    *y = cy;
    return room;
bad:
    *x = 1;
    *y = 1;
    return 0;
}

/* 401 words, 0 differing.  Park closed by j63; the park header's own
   diagnosis of the stuck 6-row group was right and is what pointed here.

   TWO LEVERS, BOTH FROM docs/matching-levers.md.

   1. `d = 0;` AT THE END OF THE ELSE-BRANCH CASE 2 IS A DEAD STORE FLOW
   DELETES, AND IT BUYS cse CANONICALITY FOR THE COPY WITHOUT BUYING A
   REFERENCE ("CANONICALITY IS PAID AT cse1, THE REFERENCE COUNT AT flow").
   The target wants the call result in the 71-reference pseudo ($s0) and the
   COPY in the ten-reference one ($s2), with the block-move destination and
   the two pre-loop 0x5C/0x58 reads ON THE COPY.  cse1 only leaves those
   reads on `d` if `d` is qty_first_reg at the `d = q` insn, which
   make_regs_eqv (cse.c:865) grants only when regno_last_uid[d] >
   regno_last_uid[q] -- and `q` is reused by the other switch cases, so it
   always dies later.  The dead `d = 0;` is the latest reference to `d` in
   the function, survives cse1 and cse2, and is deleted at flow
   (flow.c:1539), so `d` is canonical with exactly the target's ten
   references and still scores 0.409, between `w` (0.551) and `p` (0.3105):
   q $s0, w $s1, d $s2, p $s3, g $s4.  Without it, the same spelling is
   48 aligned rows -- cse puts the pre-loop reads back on `q`, `d` falls to
   seven references (0.222), and `p` and the gate pseudo swap $s2/$s3.
   Inverting the copy instead (`d = <call>; q = d;`) is 10 rows.

   2. THE FIVE `goto out;` ARE WHAT KEEPS A `v0 = 0` OUT OF THE SHARED
   `D_800DCF5E++` BLOCK.  Written as `return 0;`, each of those four tails
   is `[lhu][set v0 0][addu][sh][jump]` already at sched1 -- sched1 fills
   the load-delay slot with the return value (visible in the .sched dump),
   so local-alloc gives the increment $v1 and jump2's cross-jump can no
   longer peel the `set` off the end: 4 rows.  Jumping to a shared
   `out: return 0;` leaves the set in its own block, the slot a nop, the
   increment in $v0 and the `j` pointing at .L800A5668.  The fifth goto,
   on the else-branch case 2's `q == NULL` exit, is what makes reorg fill
   that branch's delay slot from the fall-through (`addu $a0, $s3, $zero`)
   instead of stealing `addu $v0, $zero, $zero` from .L800A5668: 3 rows
   without it.  Making EVERY `return 0` a `goto out` is 400 words.

   Other spellings the park measured and rejected: a separate local for
   q-0x20 (404w), a separate `s` local for the 0x8C-field source (+6),
   a `s16 n` gate local with `D_800DCF5C = n` (402w / 397), `g = p; d = q;`
   moved down beside the field copies (399w, cse kills both copies),
   `d = q - 0x20` as the tail (27), `d = q; if (d == NULL)` (64). */
s32 func_800A504C(u8 *a, u8 *p)
{
    u8 *g;
    u8 *q;
    u8 *w;
    u8 *d;
    u8 *e;
    u8 *z;
    s16 k;
    s16 i;
    u16 m;
    s32 r;
    u8 id;
    s32 x;
    s32 y;
    s32 f;
    u32 ab;
    g = (u8 *)(&D_80083460);
    if ((*((s16 *)(g + 0xA))) != 1)
    {
        return 0;
    }
    if (func_8003FA44(8) == 0)
    {
        return 0;
    }
    if (((*((u32 *)(p + 0x14))) & 0x4000) != 0)
    {
        switch (D_800DCF5E)
        {
        case 0:
            if (func_80053EF0(4) != 0)
            {
                return 0;
            }
            q = func_800A1618(p[0x13], 2);
            if (q == 0)
            {
                return 0;
            }
            D_80081488 = (u32)func_80047DB8(p[0x13]);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)(&D_800E3E40));
            D_800DCF5E++;
            goto out;

        case 1:
            if (D_800E3E40 == 0)
            {
                return 0;
            }
            func_80047E30();
            D_800DCF5E++;
            goto out;

        case 2:
            q = func_800A1618(p[0x13], 2);
            if (q == 0)
            {
                return 0;
            }
            w = *((u8 **)(p - 0x14));
            q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(p[0x13], q, 1))(6, w[0x24], w[0x25], (s16)((*((u16 *)(p + 0x88))) - 0x20));
            if (q == 0)
            {
                return 0;
            }
            g = p;
            d = q;
            x = *((s32 *)(d + 0x5C));
            y = *((s32 *)(d + 0x58));
            *((Unk42710 *)d) = *((Unk42710 *)p);
            *((s32 *)(q + 0x4C)) = 0;
            *((s32 *)(q + 0x50)) = 0;
            *((s32 *)(q + 0x5C)) = x;
            *((s32 *)(q + 0x58)) = y;
            func_80042640((Unk42710 *)q, q[0x13]);
            w = *((u8 **)(q - 0x14));
            *((s16 *)(w + 0x1E)) = 0x1000;
            *((s16 *)(w + 0x1C)) = 0x1000;
            *((u32 *)(q + 0x14)) &= ~7;
            *((u32 *)(q + 0x1C)) &= ~7;
            *((u32 *)(q + 0x14)) |= (*((u32 *)(p + 0x14))) & 7;
            *((u32 *)(q + 0x1C)) |= (*((u32 *)(p + 0x1C))) & 7;
            func_800424E0(q, q[0x13], 0);
            func_80042710((Unk42710 *)q, (Unk42710 *)p);
            *((u32 *)(q + 0x14)) &= ~0x4000;
            *((u32 *)(d + 0x8C)) = *((u32 *)(g + 0x8C));
            *((u32 *)(d + 0x90)) = *((u32 *)(g + 0x90));
            *((u16 *)(d + 0x94)) = *((u16 *)(g + 0x94));
            *((u16 *)(d + 0x98)) = *((u16 *)(g + 0x98));
            d[0x9A] = g[0x9A];
            d[0x9B] = g[0x9B];
            k = (s16)func_800A1BD0(p);
            ab = D_800814A8;
            *((s32 *)((ab + (k * 4)) + 0xAC)) = (s32)q;
            func_8009A3D0(w[0x24], w[0x25], 0x300);
            func_8009A028(q);
            q -= 0x20;
            func_80044A50((UnkNode *)q);
            *((u32 *)(q + 0x10)) |= 0x80000000;
            *((u16 *)(p - 0x2)) |= 0x8000;
            D_800DCF5E = 0;
            D_800814A0 |= 0x8000;
            return 0;
        }

        return 0;
    }
    switch (D_800DCF5E)
    {
    case 0:
        q = func_800A1618(p[0x13] - 1, 3);
        if (q != 0)
        {
            p[0x13]--;
            func_8003E188(p[0x13], 0);
            D_80080A90 = D_80023000;
            D_80081488 = func_80048118(p[0x13], D_8008149C);
            func_800A0B94(p[0x13], q, 0);
            D_800E3E40 = 0;
            func_8003E4FC(0xFF, D_8003E140, (s32)(&D_800E3E40));
        }
        D_800DCF5E++;
        goto out;

    case 1:
        if (D_800E3E40 == 0)
        {
            return 0;
        }
        if (D_80081488 != 0)
        {
            func_80047FF4(p[0x13], D_80081488);
        }
        D_800DCF5E++;
        goto out;

    case 2:
        q = func_800A1618(p[0x13], 3);
        if (q == 0)
        {
            return 0;
        }
        w = *((u8 **)(p - 0x14));
        q = ((u8 * (*)(s32, s32, s32, s32)) func_800A0B94(p[0x13], q, 1))(1, w[0x24], w[0x25], (s16)((*((u16 *)(p + 0x88))) - 0x20));
        if (q == 0)
        {
            goto out;
        }
        *((u32 *)(q + 0x14)) |= 0x4000;
        k = (s16)func_800A1BD0(p);
        ab = D_800814A8;
        e = *((u8 **)((ab + (k * 4)) + 0xD0));
        *((s32 *)((ab + (k * 4)) + 0xAC)) = (s32)q;
        i = e[3] & 0x1F;
        e[0] = p[0x13];
        D_800E3DF0[i] = (s32)q;
        w = *((u8 **)(q - 0x14));
        *((s16 *)(w + 0x1E)) = 0x1000;
        *((s16 *)(w + 0x1C)) = 0x1000;
        func_80042640((Unk42710 *)q, q[0x13]);
        *((u32 *)(q + 0x14)) &= ~7;
        *((u32 *)(q + 0x1C)) &= ~7;
        *((u32 *)(q + 0x14)) |= (*((u32 *)(p + 0x14))) & 7;
        *((u32 *)(q + 0x1C)) |= (*((u32 *)(p + 0x1C))) & 7;
        func_800424E0(q, q[0x13], 0);
        func_80042710((Unk42710 *)q, (Unk42710 *)p);
        if (q[0x43] < 0x40)
        {
            z = (u8 *)0x80010000;
            r = q[0x43];
            id = q[0x13];
            *((u8 *)((z + (r * 0x54)) + 0xA93)) = id;
            *((u8 *)((z + (r * 4)) + 0x980)) = id;
        }
        func_800A152C(q[0x13], 3);
        func_80042560((s8 *)q);
        m = *((u16 *)(p + 0x2A));
        *((u16 *)(q + 0x2A)) = m;
        func_80047738(w, (*((u8 **)(w + 0x2C)))[(((D_80083228 + ((s16)m)) + 0x100) >> 9) & 7], *((s8 *)(w + 0x4)));
        ((u16 *)w)[0xA] &= 0xFFFE;
        D_80080A90 = D_8014A000;
        ((u16 *)g)[5] -= 1;
        D_800DCF5C = 1;
        *((u16 *)(p - 0x2)) |= 0x8000;
        f = D_800814A0;
        *((s32 *)(p + 0x90)) = 0;
        D_800814A0 = f | 0x8000;
        d = 0;
        break;
    }

out:
    return 0;
}

/* The bound test reads the counter SIGNED (`lh`) and the increment reads it
   UNSIGNED (`lhu`) off the same `lui`; that pair is what `s16` plus a bare
   `++` gives. 12 words, 0 differing. */
s32 func_800A5690(void)
{
    if (D_800E3CCE < 0x20)
    {
        D_800E3CCE++;
        return 1;
    }
    return 0;
}

/* Saturating countdown. 8 words, 0 differing. */
void func_800A56C0(void)
{
    if (D_800E3CCE != 0)
    {
        D_800E3CCE -= 1;
    }
}

/* A COMMON RETURN VARIABLE, as in func_800A9E38: with two `return`s jump.c
   inverts the test and moves the `return 0` past the call block. The result
   is the sound call's, which is why the prototype is s32 and not void --
   every call site in this unit drops it. 16 words, 0 differing. */
s32 func_800A56E0(s32 id)
{
    s32 r;

    if (D_80013714 & 2)
    {
        r = 0;
    }
    else
    {
        r = func_80053DA8(id);
    }

    return r;
}

/* Walk the message to its terminator, fold a shift-JIS second byte in
   0x81..0x9A down by 0x21 after an 0x82 lead byte, and hand the message to
   func_800353F4.

   THE EARLY EXIT IS WRITTEN LAST. As `if (D_80013714 & 2) return 0;` at the
   top the `return 0` is laid out inline and the body jumps round it: 36
   words against 37, 30 differing. Wrapping the body in the inverted test and
   leaving `return 0` as the final statement puts the zero at the epilogue
   where the target has it -- so the rule is about where the EPILOGUE ends
   up, not about which arm is trivial. The `(u8)` on `c - 0x81` is where
   `andi 0xFF` comes from, and combine rewrites the constant to +0x7F under
   that mask. 37 words, 0 differing. */
s32 func_800A5720(void *msg)
{
    u8 *p;
    u8 c;

    if ((D_80013714 & 2) == 0)
    {
        p = (u8 *)msg;
        while (*(s8 *)p > 0)
        {
            p++;
        }
        if (p[0] == 0x82)
        {
            c = p[1];
            if ((u8)(c - 0x81) < 0x1A)
            {
                p[1] = c - 0x21;
            }
        }
        return func_800353F4((u32)msg);
    }
    return 0;
}

/* Which of the three stride-3 slots holds a +8 byte in the same group of
   three as the given one. BOTH DIVISIONS ARE WRITTEN INSIDE THE LOOP AND THE
   INVARIANT ONE IS THE LEFT OPERAND. Computing `(n - 1) / 3` into a local
   ahead of the loop gives the magic constant TWICE -- once for it and once
   hoisted for the body's own division, 31 words at 27 differing; written
   inline in the comparison, cse unifies them in the body and loop.c hoists
   one. With the operands the other way round it is 29 words at 1 differing.
   29 words, 0 differing. */
s16 func_800A57B4(u8 *p, s16 n)
{
    s32 i;
    s32 c;

    for (i = 0; i < 3; i++, p += 3)
    {
        c = p[8];
        if (c != 0 && (n - 1) / 3 == (c - 1) / 3)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* The UnkNode +0x10 handler func_800A5A18 installs: a two-phase colour ramp
   on the renderable at +0x0C and a sine wobble on its scale. State +0x64 is
   0 for the ramp up and nonzero for the ramp down; D_80083160's +0x4 flag
   bypasses the ease in each phase, and the down phase ends by whiting out
   the partner node's renderable and raising both 0x8000 flags.

   THE -0x40 IS WRITTEN 0xFFC0 THROUGH A `u16` STORE. With `*(s16 *)` the
   ternary's constant folds to a signed -64 and gcc emits `addiu $v1, $zero,
   -0x40` where the target has `ori $v1, $zero, 0xFFC0`; `-0x40`, `0xFFC0`
   and `0xFFC0u` through the s16 store are all 1 differing, and only the u16
   destination keeps the constant unsigned. 124 words, 0 differing. */
void func_800A5828(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    u8 *q;
    s32 v;

    g = (u8 *)&D_80083160;
    if (*(s16 *)(p + 0x64) == 0)
    {
        if ((*(u16 *)(g + 0x4) & 1) != 0)
        {
            v = *(s16 *)(p + 0x6A) >> 1;
        }
        else
        {
            *(s16 *)(p + 0x6A) += (0xC0 - *(s16 *)(p + 0x6A)) / *(s16 *)(p + 0x66);
            v = *(u8 *)(p + 0x6A);
        }
        c[0xE] = v;
        c[0xD] = v;
        c[0xC] = v;
        if (--*(s16 *)(p + 0x66) <= 0)
        {
            *(s16 *)(p + 0x66) = 0x10;
            *(s16 *)(p + 0x64) += 1;
        }
    }
    else
    {
        if ((*(u16 *)(g + 0x4) & 1) != 0)
        {
            *(s32 *)(c + 0xC) = 0;
        }
        else
        {
            *(s16 *)(p + 0x6A) -= *(s16 *)(p + 0x6A) / *(s16 *)(p + 0x66);
            v = *(u8 *)(p + 0x6A);
            c[0xE] = v;
            c[0xD] = v;
            c[0xC] = v;
        }
        if (--*(s16 *)(p + 0x66) <= 0)
        {
            q = *(u8 **)(*(u8 **)(p + 0x4) - 0x14);
            q[0xC] = 0x80;
            q[0xD] = 0x80;
            q[0xE] = 0x80;
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
    *(s16 *)(c + 0x1C) = (func_800644B8((++*(s16 *)(p + 0x68)) << 9) >> 4) + 0x1000;
    *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C);
    *(u16 *)(c + 0x1A) = (*(s16 *)(p + 0x68) & 1) ? 0x40 : 0xFFC0;
    q = *(u8 **)(*(u8 **)(p + 0x4) - 0x14);
    v = ((*(s16 *)(p + 0x68) & 3) << 4) + 0x40;
    q[0xC] = v;
    q[0xD] = v;
    q[0xE] = v;
}

/* Spawn the kind-0x212 node under D_80083498. TWO POINTER LOCALS, NOT ONE
   REUSED: with a single `q` for both the +0x0C renderable and the payload
   base the two stack constants land in $a2/$v1 instead of $a3/$a2 and the
   payload pointer takes $a3 -- 54 words, 14 differing. 54 words,
   0 differing. */
UnkNode *func_800A5A18(UnkVec *a, u8 *b)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *q;
    s32 v[2];

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A5828;
        func_8004491C(n, (u32)D_80045340);
        n->unk_08 = a;
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DD854;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0x4) = b;
        *(s16 *)(q + 0x66) = 4;
        v[0] = 0x1800340;
        v[1] = 0x400040;
        func_800B835C(D_800DD860, v, 1, 0);
        func_800A56E0(0x60F);
    }
    return n;
}

/* Three-state fade: state 0 eases +0x2 towards p->unk_08 and the renderer's
   +0x1C towards 0x1000 over the +0x6 countdown, state 1 waits (restarting the
   countdown while D_80083170[0] is set), and the last state decays the three
   colour bytes and raises the 0x8000 flags. 96 words, 0 differing, first
   compile. Each arm's countdown is `if (--*(s16 *)(p + 0x6) <= 0)`, the
   func_800999D4 idiom (:3385); the two `+=` in state 0 keep the whole
   expression inline, which is what gives the `lh` for the subtraction and a
   second `lhu` for the accumulate. */
void func_800A5AF0(u8 *p, u8 *b, u8 *c)
{
    if (*(s16 *)(p + 0x4) == 0)
    {
        *(s16 *)(b + 0x2) += (*(s16 *)(p + 0x8) - *(s16 *)(b + 0x2)) / *(s16 *)(p + 0x6);
        *(s16 *)(c + 0x1C) += (0x1000 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x30;
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(p + 0x4) += 1;
        }
    }
    else if (*(s16 *)(p + 0x4) == 1)
    {
        if (D_80083170[0] != 0)
        {
            *(s16 *)(p + 0x6) = 0;
        }
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(s16 *)(p + 0x6) = 0x20;
            *(s16 *)(p + 0x4) += 1;
        }
    }
    else
    {
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x6);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x6);
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x6);
        if (--*(s16 *)(p + 0x6) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Is any visible ring member standing within four tiles of D_80082E80's
   (0x24, 0x25)? THE RING HEAD IS READ FIRST and the flag word second: the
   other order puts the flags in $a0 and the head in $a1's place, 4
   differing, although the target emits the two `lui`s head-first and the
   two loads flags-first. Both abs() pairs are the `bgez`/nop/`negu` trio.
   53 words, 0 differing. */
s32 func_800A5C70(void)
{
    u8 *p;
    u8 *q;
    u8 *end;
    u8 *g;
    u16 f;

    p = D_800E3D7C;
    f = D_80013714;
    if (f & 2)
    {
        return 1;
    }
    if (f & 9)
    {
        return 0;
    }
    if (D_800E296C & 0x200000)
    {
        return 0;
    }
    g = D_80082E80;
    end = p;
    do
    {
        if (!(*(s32 *)(p + 0x1C) & 0x2208))
        {
            q = *(u8 **)(p - 0x14);
            if (abs(g[0x24] - q[0x24]) < 5 && abs(g[0x25] - q[0x25]) < 5)
            {
                return 0;
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != end);
    return 1;
}

/* 6 words, 0 differing. */
void func_800A5D44(void)
{
    *(s32 *)0x8001362C = D_8008347E;
}

/* func_800999D4's close, counted up instead of down: bump the record's own
   halfword and at 0x10 retire it. 24 words, 0 differing. */
void func_800A5D5C(u8 *p)
{
    u8 *g;

    if (++*(s16 *)p >= 0x10)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* THE REUSED TEMPORARY MUST BE THE NODE'S OWN VARIABLE. Two separate
   locals, in either order, are 15 words / 10-12 differing: sched1
   interleaves the two split address pairs and fills the lhu's load-delay
   slot. A separate reused `u8 *g` alongside `UnkNode *n` is 16 / 2 -- right
   shape, wrong registers, because the second (high)/(lo_sum) pair fails to
   tie. Reusing `n` itself ties them. 16 words, 0 differing. */
void func_800A5DBC(void)
{
    u8 *n;

    n = (u8 *)func_8003FC64(0);
    *(u32 *)(n + 0x10) = (u32)func_800A5D5C;
    n = (u8 *)&D_80083460;
    *(u16 *)(n + 0xA) += 1;
}

/* Spend the charge at +0x29 on the gauge at +0x28/+0x64. The `bgez`/+0x1E
   pair at the top is `(p[0x29] + 15) / 16` -- gcc's signed divide-by-16
   bias, not source. The total is RE-READ for the clamp rather than kept from
   the first test, which is the second `lbu 0x28`/`lh 0x64` pair.
   79 words, 0 differing on the first compile. */
void func_800A5DFC(u8 *p, u8 *v)
{
    s32 n;
    s32 tot;
    u8 *q;

    if ((*(s32 *)(p + 0x1C) & 0x40) == 0)
    {
        return;
    }
    if (p[0x28] + *(s16 *)(p + 0x64) < 2)
    {
        return;
    }
    *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x2A) + 0x800;
    n = (p[0x29] + 15) / 16;
    tot = p[0x28] + *(s16 *)(p + 0x64);
    if (tot - n <= 0)
    {
        n = tot - 1;
    }
    if (n == 0)
    {
        return;
    }
    *(s32 *)(p + 0x1C) &= ~0x20000000;
    q = *(u8 **)(p - 0x14);
    if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
    {
        func_800B4C7C(3, p, (s16)n, 0);
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) - n;
        func_800C5BBC(*(s16 *)(v + 0x2), *(s16 *)(v + 0x6), *(u16 *)(v + 0xA) - 8, 0x802080, 0x20,
                      1);
    }
    else
    {
        p[0x28] = p[0x28] - n;
    }
}

/* The kill handlers' shared report: turn the record's name pair into a
   message id, announce it at the record's tile, play 0x50C, release the
   message and drop the D_80083460 + 0xA live count. The name pair goes in as
   `(q[1], q[0])`, the same order func_800A7A38 records. The record the tile
   comes from is the one at p - 0x18. 34 words, 0 differing. */
void func_800A5F38(u8 *p, u8 *q)
{
    u8 *r;
    u8 *g;
    u32 t;

    r = *(u8 **)(p - 0x18);
    t = func_8004A658(q[1], q[0]);
    func_800A7A7C(*(u16 *)(r + 0x2) >> 6, *(u16 *)(r + 0x6) >> 6, *(s16 *)(r + 0xA), t, q);
    func_800A56E0(0x50C);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

/* The bar-wipe primitive pair, func_800A624C's allocator shape with
   different fill.

   (1) The base must be a POINTER LOCAL: through the bare COMPONENT_REF the
   two post-call reads re-materialise a `lui` into the dead $s0 instead of
   keeping the symbol's address in $s3 across the calls, 80 words against 81.
   (2) The +0xA store belongs in BOTH arms of the counter test, not after the
   join: with a `v` local holding the value the whole chain lands one register
   across, 12 differing, and nine spellings of the division move none of it.
   (3) The read-back of +0xA goes in a local: written out twice the store to
   the renderable's +0x14 invalidates cse's entry and the second read is a
   second `lhu`, 83 words. 81 words, 0 differing. */
s32 func_800A5FC0(u8 *p)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    s16 n;
    s32 w;

    g = &D_80083160;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, *(s32 *)(p + 0x4), 0);
    *(s32 *)(r + 0x4) = *(s32 *)(p + 0x0);
    SetPolyF4(r);
    SetSemiTrans(r, 1);
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x8) = 0;
    *(s16 *)(r + 0xE) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    n = *(u16 *)(p + 0x8) - 1;
    *(s16 *)(p + 0x8) = n;
    if (n > 0)
    {
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + (0x140 - *(s16 *)(p + 0xA)) / n;
    }
    else
    {
        *(s16 *)(p + 0x8) = 0;
        *(s16 *)(p + 0xA) = 0x140;
    }
    w = *(u16 *)(p + 0xA);
    *(s16 *)(r + 0x14) = w;
    *(s16 *)(r + 0xC) = w;
    AddPrim(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), r);
    AddPrim(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), q);
    return 0;
}

/* The base local is advanced halfway through, which is where the target's
   `addiu $v1, $v1, 0x20` is. 14 words, 0 differing. */
void func_800A6104(void)
{
    u8 *p;

    p = D_800DCF64;
    if (p != NULL)
    {
        *(s32 *)(p + 0x20) = 0xFFFFFF;
        p += 0x20;
        *(s32 *)(p + 0x4) = 0x40;
        *(s16 *)(p + 0xC) = 1;
    }
}

/* 22 words, 0 differing. */
UnkNode *func_800A613C(void)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        func_8004491C(n, (u32)func_800A5FC0);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 6;
        *(s16 *)(p + 0xC) = 0x10;
    }
    return n;
}

/* A node's +0x10 step: fade the level at +0x0A up to 0xFF over the frames
   left at +0x08.

   THE `|=` ON D_800E296C HAS TO BE SPLIT AROUND THE +0x08 STORE. gcc cannot
   prove the store through `p` misses the global, so written as one compound
   assignment the `sh` is pinned either ahead of the `lw` or behind the `sw`,
   and the target has it BETWEEN them. All six orderings of the arm's three
   statements were measured: 3, 3, 4, 8 differing and two at 44 words.
   46 words, 0 differing. */
void func_800A6194(u8 *p)
{
    s32 t;
    s32 f;

    t = --*(s16 *)(p + 0x8);
    if (t > 0)
    {
        *(s16 *)(p + 0xA) += (0xFF - *(s16 *)(p + 0xA)) / t;
    }
    else
    {
        *(s16 *)(p + 0xA) = 0xFF;
        f = D_800E296C;
        *(s16 *)(p + 0x8) = 0;
        D_800E296C = f | 0x2000000;
    }
    if ((D_800E296C & 0x1000000) == 0)
    {
        D_800E296C &= ~0x2000000;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A node handler: take a 0xC-byte and a 0x18-byte primitive off D_80083160's
   +0x8D0 cursor and hang both on an OT row.

   THE CURSOR MUST BE READ THROUGH THE STRUCT FIELD. With
   `b = *(u8 **)&D_80083160;` and a `u8 *` store -- func_8009DA70's spelling,
   which is right for ONE allocation -- cse forwards the first bump into the
   second read, deletes the dead store and folds both allocations into one
   `addiu +0x24`: 61 words against 64, 62 differing. Reading the COMPONENT_REF
   and storing through `*(u32 *)` makes each store invalidate the next read,
   which is the two reloads the target has. The discriminator is the READ's
   spelling, not the store's. 64 words, 0 differing. */
s32 func_800A624C(u8 *p)
{
    u8 *q;
    u8 *r;
    s32 c;

    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, 0x40, 0);
    c = *(s16 *)(p + 0xA);
    *(s32 *)(r + 0x4) = c | (c << 8) | (c << 16);
    SetPolyF4(r);
    SetSemiTrans(r, 1);
    *(s32 *)(r + 0x8) = 0;
    *(s32 *)(r + 0xC) = 0x140;
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x14) = 0xE00140;
    AddPrim(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), r);
    AddPrim(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), q);
    return 0;
}

/* The width of arg0 is NOT witnessed: the only caller passes a constant 0
   and the callee only stores it with `sh`, so s16 and s32 are the same
   bytes. s16 matches the landed func_800BC228, one constant over.
   27 words, 0 differing. */
UnkNode *func_800A634C(s16 arg0)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A6194;
        func_8004491C(n, (u32)func_800A624C);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 8;
        *(s16 *)(p + 0xC) = arg0;
    }
    return n;
}

/* func_800A6480's message with the noun chosen by `k`. The whole chain is
   duplicated into both arms and cross-jumping merges it back from the
   `jal func_80099194` onwards: one shared func_80099368 call ahead of the
   `if` is 46 words of 50 and 43 differing.

   THE `__asm__` MARKER IS A DEVICE and it is the whole difference between 0
   and 9. Without it `q` (3 references over 16 insns, 1 * 3 / 16) loses $s1 to
   the handle pseudo (4 over 34, 2 * 4 / 34) in global.c's allocno_compare.
   The marker's fourth reference on `q` crosses floor_log2's step (2 * 4 / 16
   = 5000 against 2352) and emits nothing. Seven structural spellings were
   ruled out at 50 words / 9 differing, so no source spelling reaches it.
   50 words, 0 differing. */
void func_800A63B8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    /* DEVICE: a zero-instruction reference on q, worth the whole 9-differing
       residue above and no words. */
    __asm__("" ::"r"(q));
    if (k == 0x15)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A03, h);
    }
    else
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A12, h);
    }
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* The smallest of the nineteen: the reused-temporary idiom and nothing
   else. See func_800BD45C for why `h` is the name that is written again.
   34 words, 0 differing. */
void func_800A6480(u8 *p, u8 *q)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099368(q, h);
    h = func_80099194(D_800E0A1B, h);
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* 9 words, 0 differing. */
void func_800A6508(void)
{
    func_800997FC(D_800E092B);
}

/* D_800DCF3C/44/48 are D_800DCF08[13], [15] and [16] by address, but the
   target gives each its own `lui`/`lw %lo(SYM)` pair and all four names are
   in splat/dungeon.bin_undefined_syms_auto.txt, so they are written as
   separate symbols; only the VARIABLE index goes through the array face.
   `s2 = 7` in the loop preheader is loop.c hoisting the `i == 7` constant.
   61 words, 0 differing. */
void *func_800A652C(u8 *p)
{
    s32 i;
    s16 r;

    if (p[0x13] != 0)
    {
        if (p[0x25] == 0)
        {
            return D_800DCF44;
        }
        if ((*(u32 *)(p + 0x1C) & 0x20000) == 0)
        {
            return D_800DCF3C;
        }
    }
    for (i = 0xE; i >= 0; i--)
    {
        r = func_80042900((s8 *)p, i);
        if (r > 0 && i == 7)
        {
            return D_800DCF48;
        }
        if (r != 0)
        {
            return D_800DCF08[i];
        }
    }
    return D_800DCF08[0];
}

/* Release the D_800E3DF0 slot the actor's +0x3 byte names, close its record
   down and report the ring index. THE MODE ARGUMENT IS A TERNARY, not the
   `if/else` into a local that func_800AA258 uses next to the same call:
   with the local, the two `lbu` argument loads stay below the branch and
   the function is 88 words / 5 differing, with exactly those two loads and
   the branch rotated; as a ternary both loads rise above it.
   ONE LOCAL CARRIES BOTH THE SLOT INDEX AND THE RESULT -- they share $s1,
   which is what two disjoint ranges in one declared local do.
   0x80010A80 is a constant address (`lui`/`ori`, not `lui`/`addiu`).
   88 words, 0 differing. */
s32 func_800A6620(u8 *p, s32 f)
{
    u8 *q;
    u8 *base;
    u8 *e;
    s32 r;

    r = p[3] & 0x1F;
    q = (u8 *)D_800E3DF0[r];
    D_800E3DF0[r] = 0;
    r = q[0x43];
    if (r < 0x40)
    {
        q[0x44] = 0;
        func_800422DC((u8 *)(r * 0x54 + 0x80010A80), q);
    }
    if ((p[3] & 0x20) != 0)
    {
        r = (s16)func_800A1BD0(q);
        if (r >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(r * 4 + base + 0xAC) = 0;
            *(s32 *)(r * 4 + base + 0xD0) = 0;
        }
        if ((s16)func_80042900((s8 *)q, 0x1B) == 0)
        {
            e = *(u8 **)(q - 0x14);
            func_8009A3D0(e[0x24], e[0x25], (*(s32 *)(q + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        if (f != 0)
        {
            func_800A32A4(q);
        }
        func_8009A028(q);
        *(u16 *)(q - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_80098B38(p);
    return r;
}

/* The "is the shop open" probe. ONE 0x80010000 BASE, shared by the two
   globals.

   AND THE FIRST READ MUST BE AN ARRAY REF, which is the whole fight. Spelled
   `*(s32 *)(b + 0x2090)` the MEM is not MEM_IN_STRUCT_P, so true_dependence
   keeps a dependence against the `sb` to D_80080A88 -- its second exemption
   needs the READ in-struct and the WRITE's address non-varying, and a
   LO_SUM's address does not vary -- the store stays pinned ahead of the load
   and the `beq` keeps a `nop`: 30 words, 28 differing. As `((s32 *)b)[0x824]`
   expr.c stamps the ARRAY_REF in-struct, the exemption fires, and reorg takes
   the store into the delay slot. A pointer local frees the slot but
   rematerialises the base, 30 / 25. 29 words, 0 differing. */
s32 func_800A6780(void)
{
    u8 *b;
    s32 v;
    s32 t;

    b = (u8 *)0x80010000;
    D_80080A88 = 0;
    v = ((s32 *)b)[0x824];
    if (v == 1)
    {
        D_80080A88 = 1;
        return 1;
    }
    if (v != 0)
    {
        return 0;
    }
    t = ((s32 *)b)[0x8D];
    if (t / 5 != (t + 1) / 5)
    {
        return 0;
    }
    D_80080A88 = 1;
    return 1;
}

/* D_800DCF4F IS READ WITH `lb`, so its declaration above was corrected from
   u8 to s8; as u8 the test is `lbu` and the word is wrong. 31 words,
   0 differing. */
void func_800A67F4(void)
{
    u8 *g;

    if ((D_800E296C & 0x200000) == 0)
    {
        if ((D_80013714 & 8) == 0)
        {
            if (D_800DCF4F != 0)
            {
                D_800DCF4F = 0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) -= 1;
            }
            func_80035208(D_800E045C);
            func_80035348();
        }
    }
}

/* 20 words, 0 differing. */
s32 func_800A6870(s16 x)
{
    if (x == 0xFF)
    {
        x = 0x1E;
    }
    return (x >> 2) + (func_800A6D30() & 3);
}

/* Save bit 4 of the engine record's +0x1C flag word, then drop it. The
   field is re-read for the mask because the intervening byte store
   invalidates cse's entry. 13 words, 0 differing. */
void func_800A68C0(void)
{
    u8 *p;

    p = D_800E3D7C;
    D_800DD878 = (*(u32 *)(p + 0x1C) >> 4) & 1;
    *(u32 *)(p + 0x1C) &= ~0x10;
}

/* func_800A68C0's restore. 13 words, 0 differing. */
void func_800A68F4(void)
{
    u8 *p;

    if (D_800DD878 != 0)
    {
        p = D_800E3D7C;
        *(u32 *)(p + 0x1C) |= 0x10;
    }
}

/* func_80098FF8's base rule again; here splat could not pattern-match the
   `lui 0x8001`/`ori 0x4C` at all, which is the proof there is no
   relocation. 21 words, 0 differing. */
s16 func_800A6928(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 n;
    s32 i;

    t = (u8 *)0x80010000;
    key = (a << 8) | b;
    n = 0;
    for (i = 0x13; i >= 0; i--)
    {
        if (**(u16 **)(t + i * 4 + 0x29C) == key)
        {
            n++;
        }
    }
    return n;
}

/* func_800A6928's backward scan over a second 64-entry stride-4 table.

   THE BASE CONSTANT CARRIES THE WHOLE ADDRESS: `t = (u8 *)0x80011F80` and
   `t + i * 4`. Written func_800A6928's way -- 0x80010000 with the table
   offset in the index, as `+ 0x1F80`, as `(i + 0x7E0) * 4`, or as
   `&((u32 *)t)[i + 0x7E0]` -- fold distributes the constant back out and
   loop.c leaves it in the load's DISPLACEMENT, so the giv's initial value is
   0x800100FC: 26 words, 2 differing. 26 words, 0 differing. */
s16 func_800A697C(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 i;

    t = (u8 *)0x80011F80;
    key = (a << 8) | b;
    for (i = 0x3F; i >= 0; i--)
    {
        if (*(u16 *)(t + i * 4) == key)
        {
            return 1;
        }
    }
    return func_800A6928(a, b);
}

/* Sixteen calls with a rising s16 argument from 2, then one per unit of
   func_800A6DA4's halfword result.

   THE RISING ARGUMENT IS `(s16)(i + 2)`, NOT AN `s16` COUNTER. With a second
   variable `s16 k = 2; ... k++` the sign extension stays at the use and the
   loop costs two extra words. Cast inline, loop.c strength-reduces
   `(i + 2) << 16` into the induction variable the target walks by 0x10000.
   The second bound is masked at the ASSIGNMENT: declared `u16` the mask
   lands at the guard instead and costs a word. 34 words, 0 differing. */
void func_800A69E4(void)
{
    s32 i;
    s32 n;

    n = func_800A6DA4(4, 8) & 0xFFFF;
    for (i = 0; i < 0x10; i++)
    {
        func_800A08A0((s16)(i + 2));
    }
    for (i = 0; i < n; i++)
    {
        func_800A08A0(1);
    }
}

/* A two-byte stack record handed to a ONE-argument callee -- the `jal`'s
   delay slot is the second `sb`, not an argument. 12 words, 0 differing. */
void func_800A6A6C(void)
{
    u8 buf[2];

    buf[0] = 1;
    buf[1] = 0x12;
    func_800A90E8(buf);
}

/* Clear bit 0x400 on every entry of every descriptor's 0x14-stride table,
   then re-set it from the 19x8-byte bitmap at 0x80015720.

   THREE SPELLINGS. (1) The bitmap is a CONSTANT BASE, not the symbol: a
   register index never lets cse fold a symbol's `%lo` into the MEM, so the
   declared array spends an `addiu %lo` the target does not have (54 words).
   (2) The mask has to be a named `s32`: `*(u16 *)q &= ~0x400` is narrowed by
   the front end to `andi 0xFBFF` (3 differing), where the target keeps -0x401
   in a register. (3) Both invariants are assigned INSIDE the outer loop so
   loop.c hoists them; written above the loop they are preheader STATEMENTS
   and sit ahead of `j = 0`, 3 differing. 53 words, 0 differing. */
void func_800A6A9C(void)
{
    s32 j;
    s32 i;
    s32 v;
    s32 m;
    u8 *t;

    for (j = 0; j < 0x13; j++)
    {
        m = ~0x400;
        t = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            v = *(u16 *)(D_80073414[j].unk_0C + i * 0x14);
            v &= m;
            *(u16 *)(D_80073414[j].unk_0C + i * 0x14) = v;
            if ((t[j * 8 + i / 8 + 0x5720] >> (i & 7)) & 1)
            {
                *(u16 *)(D_80073414[j].unk_0C + i * 0x14) |= 0x400;
            }
        }
    }
    func_800A6A6C();
}

/* 19 words, 0 differing. */
void func_800A6B70(u8 *p, s32 b, UnkDungeonSprite *s)
{
    s32 v;

    v = *(s32 *)(p + 0xC) + 1;
    *(s32 *)(p + 0xC) = v;
    v = (func_800644B8(v * 64) >> 7) + 0x60;
    s->unk_0E = v;
    s->unk_0D = v;
    s->unk_0C = v;
}

/* `s = step;` FIRST, `q = p + 2;` SECOND. With the s16 parameter used
   directly the sign-extension pair is scheduled to the end of the preheader
   (4 differing in every control-flow spelling); copied out first the
   preheader comes out sll / sra / addiu q / acc = 0. A `goto` back-edge is
   12-17 differing -- loop.c never sees the loop and the two cursors are not
   givs. 17 words, 0 differing. */
void func_800A6BBC(u8 *p, s16 step)
{
    s32 s;
    u8 *q;
    s32 acc;

    s = step;
    q = p + 2;
    acc = 0;
    for (;;)
    {
        *q += acc;
        acc += s;
        if (*(s8 *)p >= 0)
        {
            q += 0xC;
            p += 0xC;
            continue;
        }
        break;
    }
}

/* The 0x12-kind node for the dungeon's own HUD panel. `-r[3] + 8`, NOT
   `8 - r[3]`: the subtraction form materialises the 8 with an `li` and emits
   `subu $a2, $a2, $v0`, where the target negates the byte in place and adds
   -- 7 differing, all in those three words. 76 words, 0 differing. */
UnkNode *func_800A6C00(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkDungeonSprite *s;
    u8 *q;
    u8 *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        m = n->unk_08;
        s = (UnkDungeonSprite *)n->unk_0C;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = 0x3C;
        *(s16 *)((u8 *)m + 0x6) = 0x40;
        *(s16 *)(q + 0x8) = 1;
        s->unk_1E = 0x1800;
        s->unk_1C = 0x1800;
        n->unk_10 = (u32)func_800A6B70;
        func_8004491C(n, (u32)func_800B06F0);
        r = (u8 *)n + 0x30;
        *(u32 *)q = (u32)((u8 *)n + 0xF0);
        *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0xC0);
        *(s32 *)((u8 *)n + 0xF0) = func_8004DA74(r, D_80089074, 0) & 0x7FFFFFFF;
        func_800B1320(r, 0xA8, (s16)(-*(s8 *)(r + 0x3) + 8));
        func_800A6BBC(r, 4);
        func_800B135C(r, 8);
        func_800B13CC(r, 0x20);
        func_800B1400(r, 2);
        *(u32 *)((u8 *)n + 0xC0) = 0x808080;
        *(s32 *)((u8 *)n + 0xF4) = 0;
    }
    return n;
}

/* The LCG. D_80083160 + 0x1FC is the seed, and with two accesses it arrives
   as a base pointer with folded displacements rather than as D_8008335C.
   12 words, 0 differing. */
u16 func_800A6D30(void)
{
    u8 *g;
    s32 v;

    g = (u8 *)&D_80083160;
    v = *(s32 *)(g + 0x1FC) * 0x41C64E6D + 0x3039;
    *(s32 *)(g + 0x1FC) = v;
    return ((u32)v >> 16) & 0x7FFF;
}

/* ONE BASE LOCAL FOR BOTH ARMS: the `addiu %lo(D_80083160)` sits in the
   `beqz` delay slot and both sides use it. The halfword parameter is only
   tested, so its `sra` is combined away and the bare `sll` is what is left.
   14 words, 0 differing. */
void func_800A6D60(s16 flag)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (flag != 0)
    {
        D_800DD87C = *(s32 *)(g + 0x1FC);
    }
    else
    {
        *(s32 *)(g + 0x1FC) = D_800DD87C;
    }
}

void func_800A6D98(s32 v)
{
    D_8008335C = v;
}

/* A random number in the inclusive range the two arguments bound, in either
   order.

   ONE LOCAL DOES THE SWAP AND THE DRAW. With a separate `s32 r` for the call
   result -- or with the call written inline, or `lo + ...` operand order, or
   signed parameters with casts, or `lo > hi` for `hi < lo` -- the
   `andi $v0, 0xFFFF` lands in $v1 and the divisor in $v0: 27 words, 4-5
   differing. Reusing the swap temporary for the draw gives that pseudo the
   second set that keeps it in $v0. The divisor needs its own local either
   way. 27 words, 0 differing. */
s32 func_800A6DA4(u32 lo, u32 hi)
{
    u32 t;
    u32 n;

    if (hi < lo)
    {
        t = hi;
        hi = lo;
        lo = t;
    }
    t = func_800A6D30();
    n = hi - lo + 1;
    return (u16)(t % n + lo);
}

/* COUNT the live slots standing on a tile -- func_800A7030's sweep with a
   counter instead of a clear. Both tables are indexed off the loop counter
   for the reason recorded there, and the count is initialised before the
   counter, which is the `addu $a3, $t0, $zero` the listing opens with.
   31 words, 0 differing. */
s16 func_800A6E10(s16 x, s16 y)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if (D_800E36C8[i].unk_00 == x)
            {
                if (D_800E36C8[i].unk_01 == y)
                {
                    n++;
                }
            }
        }
    }
    return n;
}

/* The nearest matching spot on D_800E3548/D_800E36C8, searched from slot 0x3F
   down: the slot's +0x01 kind must equal the key's low byte, its +0x00 must
   equal the key's second byte when that byte is nonzero, and the spot must be
   on the floor the record's +0x26 names. The winner's coordinates go to the
   two out-parameters and the return is whether anything matched.

   Four spellings, each worth what the next line says:
   * `i = 0x3F;` IS ITS OWN STATEMENT AHEAD OF `lo`/`hi`, with the `for`
     reduced to `for (; i >= 0; i--)`. With the init in the `for` the counter's
     `li $s4, 0x3F` lands AFTER the `andi`/`sw` pair in the preheader and that
     is 3 differing -- the preheader runs in source order and the `for`'s init
     is the last source statement before it.
   * The two absolute values are TERNARIES, `dx = (dx < 0) ? -dx : dx;`. As
     `if (dx < 0) { dx = -dx; }` the sign-extension of dx for the comparison
     moves out of the +0x25 block into the compare block, a `nop` takes its
     place, and it is 106 words / 51 differing.
   * The test is `best > d`, not `d < best`: the other order evaluates d's
     `sll` before best's and swaps $a2/$a3 for the flag and the maximum
     (105 / 10).
   * The maximum is written TWICE, once into `d` and once into `best`. With
     `best = d;` the three-word re-select collapses to one `move` and the
     function is 101 words / 61 differing.
   `lo` is an `s32` holding `key & 0xFF`, not a `u8`: as a `u8` local it is
   spilled with `sb`/`lbu` instead of the target's `andi`/`sw`/`lw`
   (112 words / 91 differing). 105 words, 0 differing. */
s32 func_800A6E8C(u8 *p, s32 key, s16 *ox, s16 *oy)
{
    s32 i;
    s32 lo;
    s8 hi;
    s16 best;
    s16 found;
    s16 dx;
    s16 dy;
    s16 d;
    s32 x;
    s32 y;

    if (*(s8 *)(p + 0x26) < 0)
    {
        return 0;
    }
    best = 0x100;
    found = -1;
    i = 0x3F;
    lo = key & 0xFF;
    hi = key >> 8;
    for (; i >= 0; i--)
    {
        if (D_800E3548_s[i].unk_01 != lo)
        {
            continue;
        }
        if (hi != 0 && D_800E3548_s[i].unk_00 != hi)
        {
            continue;
        }
        x = D_800E36C8[i].unk_00;
        y = D_800E36C8[i].unk_01;
        if (func_8009FB34(x, y) != *(s8 *)(p + 0x26))
        {
            continue;
        }
        dx = x - p[0x24];
        dx = (dx < 0) ? -dx : dx;
        dy = y - p[0x25];
        dy = (dy < 0) ? -dy : dy;
        d = (dx < dy) ? dy : dx;
        if (best > d)
        {
            best = (dx < dy) ? dy : dx;
            *ox = x;
            *oy = y;
            found = i;
        }
    }
    return found >= 0;
}

/* Clear any of the 0x40 live slots standing on the given tile and report it.
   D_800E3548's +0x01 is the slot's live flag and D_800E36C8 holds the tile.

   BOTH TABLES ARE INDEXED OFF THE COUNTER, not walked as pointers, and the
   tile bytes are held in LOCALS. Three measurements behind that: a `u8 *`
   walk over the wide table collapses its two field reads into one giv
   biased by the later offset and loses the base (`addiu $s0, $v0, 7`, reads
   at -1 and 0) where the target keeps the base and uses +6/+7; a
   hand-initialised pointer for the narrow table is a biv rather than a giv,
   which reverses the order loop.c emits the two initialisations in; and
   passing `D_800E36C8[i].unk_00` to the call a second time costs two
   reloads, because the word store to the slot between them invalidates
   cse's memory. 45 words, 0 differing. */
void func_800A7030(s16 x, s16 y)
{
    s32 i;
    u8 cx;
    u8 cy;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            cx = D_800E36C8[i].unk_00;
            if (cx == x)
            {
                cy = D_800E36C8[i].unk_01;
                if (cy == y)
                {
                    *(s32 *)&D_800E3548_s[i] = 0;
                    func_8009A3D0(cx, cy, 0x800);
                }
            }
        }
    }
}

/* func_800A7030's sweep as a SEARCH: the first live slot on (x, y) whose
   +0x04 facing is within 0x40 of `ang`. The +0x04 halfword is read with `lh`
   through an explicit `(s16)` cast rather than through the D_800E36C8_u
   alias: a second `extern` under the same `__asm__` name would defeat cse's
   unification of the bases, and the target has one `addiu` for all three
   reads. 68 words, 0 differing. */
s16 func_800A70E4(s16 x, s16 y, s16 ang)
{
    s32 i;
    s32 d;
    u16 m;

    func_8009A350(x - 1, y, 0, &m);
    if ((m & 0x800) == 0)
    {
        return -1;
    }
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            d = ang - (s16)D_800E36C8[i].unk_04;
            if (d < 0)
            {
                d = -d;
            }
            if (d < 0x40)
            {
                return i;
            }
        }
    }
    return -1;
}

/* Find the first free slot of a stride-4 table by its +0x01 byte, returning
   the index as a halfword and -1 when the table is full. The index is cast
   on the way out -- that is the `sll`/`sra` pair in the delay slots -- while
   the -1 needs no widening, which is why only one arm has it. */
s16 func_800A71F4(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3548; i < 0x40; i++, p += 4)
    {
        if (p[1] == 0)
        {
            return i;
        }
    }
    return -1;
}

/* Find a free tile for a new spot near (x, y): the tile west of it, then the
   eight neighbours starting at D_8008347E's facing, then the sixteen offsets
   in D_800DCE6C/D_800DCE8C starting at D_8008347E's low nibble. A candidate
   passes when func_8009A350 reports the tile exists, none of the 0x8820 flags
   is set, and the floor height func_800BCB04 gives for its centre is below
   0x200; the tile and that height go out through the three pointers.
   197 words, 0 differing.

   FOUR SPELLINGS, each measured against the landing:
   (1) `r` IS `s32` WITH THE `(s16)` AT THE COMPARE. As `s16` the first
   probe's result lands in $v1 while both loops' land in $a0, so jump.c cannot
   cross-jump the three `*oz = r; return 1` tails into one: 200 words, 150
   differing.
   (2) EACH OF THE THREE PROBES NEEDS ITS OWN PAIR of func_800BCB04 argument
   locals. One pair shared by all three is a global allocno and the `<< 6`
   chain comes out of $v1/$v0 with the final `andi` copying into $a0/$a1
   instead of running in $a0/$a1 from the load: 27 differing.
   (3) THE TABLES ARE INDEXED, NOT WALKED THROUGH `s16 *` LOCALS. Pointer
   locals are two more allocnos and they outrank the direction counter in
   global-alloc, so the three callee-saved registers come out permuted
   ($s0/$s1/$s2 against the target's $s2/$s0/$s1) and the scheduler can no
   longer hoist the `z - 0x20` and `&m` setups past them: 32 differing.
   (4) THE EIGHT-WAY LOOP'S INDEX IS AN `s16` COPY of the `s32` counter --
   that is the `addu $s0, $s1, $zero` / `addu $a2, $s0, $zero` pair. An `s32`
   copy is propagated away by cse and the function is a word short: 196 words,
   120 differing. The `s16` copy survives cse because the modes differ, and
   combine still drops the sign-extension at the use because the counter is
   masked to 0..7.

   The counter has to be stepped BEFORE the index, which is why both loops are
   `while` and not `for (i = 0; i < 8; i++)` with the mask last: the other
   order fills both early-exit delay slots from the wrong block, 59
   differing. */
s32 func_800A7234(s16 x, s16 y, s16 z, s16 *ox, s16 *oy, s16 *oz)
{
    u16 m;
    s32 r;
    s16 gx;
    s16 gy;
    s32 i;
    s16 j;
    s32 k;
    s32 ax;
    s32 ay;
    s32 bx;
    s32 by;
    s32 cx;
    s32 cy;

    if ((s16)func_8009A350(x - 1, y, 0, &m) != 0)
    {
        if ((m & 0x8820) == 0)
        {
            ax = x << 6;
            ax = (ax + 0x20) & 0xFFE0;
            ay = y << 6;
            ay = (ay + 0x20) & 0xFFE0;
            r = func_800BCB04(ax, ay, z - 0x20);
            if ((s16)r < 0x200)
            {
                *ox = x;
                *oy = y;
                *oz = r;
                return 1;
            }
        }
    }
    gx = x;
    gy = y;
    k = D_8008347E & 7;
    i = 0;
    while (i < 8)
    {
        j = k;
        if ((s16)func_8009A350(gx, gy, j, &m) != 0)
        {
            if ((m & 0x8820) == 0)
            {
                bx = (gx + D_8006CCD8[j]) << 6;
                bx = (bx + 0x20) & 0xFFE0;
                by = (gy + D_8006CCE8[j]) << 6;
                by = (by + 0x20) & 0xFFE0;
                r = func_800BCB04(bx, by, z - 0x20);
                if ((s16)r < 0x200)
                {
                    *ox = x + (u16)D_8006CCD8[j];
                    *oy = y + (u16)D_8006CCE8[j];
                    *oz = r;
                    return 1;
                }
            }
        }
        i++;
        k = (k + 1) & 7;
    }
    gx = x;
    gy = y;
    k = D_8008347E & 0xF;
    i = 0;
    while (i < 0x10)
    {
        if ((s16)func_8009A350(x + (u16)D_800DCE6C[k] - 1, y + (u16)D_800DCE8C[k], 0, &m) != 0)
        {
            if ((m & 0x8820) == 0)
            {
                cx = (gx + D_800DCE6C[k]) << 6;
                cx = (cx + 0x20) & 0xFFE0;
                cy = (gy + D_800DCE8C[k]) << 6;
                cy = (cy + 0x20) & 0xFFE0;
                r = func_800BCB04(cx, cy, z - 0x20);
                if ((s16)r < 0x200)
                {
                    *ox = x + (u16)D_800DCE6C[k];
                    *oy = y + (u16)D_800DCE8C[k];
                    *oz = r;
                    return 1;
                }
            }
        }
        i++;
        k = (k + 1) & 0xF;
    }
    return 0;
}

/* func_800A7700's entity, stepped: case 0 arms the 4-frame countdown, case 1
   swings the renderable's +0x1C/+0x1E by rsin/rcos of a rising angle, case 2
   unwinds it, bleeds the mover's +0x14 and fades the three colour bytes, and
   the mover's +0x08 takes the accumulated +0x14 on every path.
   110 words, 0 differing, first compile.

   The switch's `case 0` FALLS THROUGH into `case 1`, and `*(s16 *)(a + 0xA4)
   = 1;` in case 2 reuses the constant 1 the switch head already materialised
   for its `beq`. */
void func_800A7548(u8 *a, UnkVec *b, u8 *c)
{
    switch (*(s16 *)(a + 0xA0))
    {
    case 0:
        *(s16 *)(a + 0xA6) = 4;
        *(s16 *)(a + 0xA4) = 0;
        *(s16 *)(a + 0xA0) = *(u16 *)(a + 0xA0) + 1;
    case 1:
        *(s16 *)(a + 0xA4) = *(u16 *)(a + 0xA4) + 1;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + func_800644B8(*(s16 *)(a + 0xA4) * 32);
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) - func_800644B8(*(s16 *)(a + 0xA4) * 32);
        *(s16 *)(a + 0xA6) = *(u16 *)(a + 0xA6) - 1;
        if (*(s16 *)(a + 0xA6) <= 0)
        {
            *(s16 *)(a + 0xA0) = *(u16 *)(a + 0xA0) + 1;
        }
        break;
    case 2:
        *(s16 *)(a + 0xA4) = *(u16 *)(a + 0xA4) - 1;
        if (*(s16 *)(a + 0xA4) <= 0)
        {
            *(s16 *)(a + 0xA4) = 1;
        }
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) - func_800644B8(*(s16 *)(a + 0xA4) * 32) * 2;
        *(s16 *)(c + 0x1E) = *(u16 *)(c + 0x1E) + func_800644B8(*(s16 *)(a + 0xA4) * 32) * 4;
        b->unk_14 -= 0x20000;
        c[0xC] = c[0xD] = c[0xE] -= 4;
        if (*(s16 *)(c + 0x1C) <= 0)
        {
            *(u16 *)(a - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    b->unk_08 += b->unk_14;
}

/* The node spawner for func_800A7548's entity. THE THIRD PARAMETER IS `s32`
   although it is only ever `sh`-stored: as s16 (or u16) its HImode pseudo
   outranks the other three in global-alloc and the four parameter copies come
   out $s0/$s2/$s3/$s1 against the target's $s1/$s3/$s0/$s2 -- 43 words, 13
   differing, every one a register. Caching n->unk_08 in a local is required:
   reaching it twice is 46 words / 31. 43 words, 0 differing. */
UnkNode *func_800A7700(s32 x, s32 y, s32 ang, s32 *q)
{
    UnkNode *n;
    UnkVec *m;

    n = (UnkNode *)func_800A8608(D_80083498, q, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    n->unk_10 = (u32)func_800A7548;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = (x << 6) + 0x20;
    *(s16 *)((u8 *)m + 0x6) = (y << 6) + 0x20;
    *(s16 *)((u8 *)m + 0xA) = ang;
    *(s32 *)((u8 *)n + 0xB8) = *q;
    return n;
}

/* Ease the mover onto the tile at +0xAA/+0xAC over the +0xA0 countdown, and
   on the last frame claim a D_800E3548 slot for it: the four-byte word at
   +0x98 goes into the slot, the two tile bytes, the +0xAE height twice and
   the renderer's +0x08 go into the matching D_800E36C8 record, and the view
   is re-aimed. 104 words, 0 differing.

   THE +0x02 AND +0x04 STORES SHARE ONE READ -- written as two statements
   each reading `*(u16 *)(p + 0xAE)` the second `lhu` and its load-delay nop
   survive, 106 words. A chained assignment or one `u16` local is 104.
   The third numerator has no `<< 6` and no `+ 0x20`: it is a raw height,
   where the first two are tile coordinates. */
void func_800A77AC(u8 *p, UnkVec *v, u8 *c)
{
    s16 i;
    s16 t;
    u8 *g;

    v->unk_00 += ((((*(s16 *)(p + 0xAA) << 6) + 0x20) << 16) - v->unk_00) / *(s16 *)(p + 0xA0);
    v->unk_04 += ((((*(s16 *)(p + 0xAC) << 6) + 0x20) << 16) - v->unk_04) / *(s16 *)(p + 0xA0);
    v->unk_08 += ((*(s16 *)(p + 0xAE) << 16) - v->unk_08) / *(s16 *)(p + 0xA0);
    t = *(u16 *)(p + 0xA0) - 1;
    *(s16 *)(p + 0xA0) = t;
    if (t <= 0)
    {
        i = func_800A71F4();
        if (i >= 0)
        {
            *(s32 *)&D_800E3548_t[i] = *(s32 *)(p + 0x98);
            D_800E36C8_u[i].unk_00 = p[0xAA];
            D_800E36C8_u[i].unk_01 = p[0xAC];
            D_800E36C8_u[i].unk_04 = D_800E36C8_u[i].unk_02 = *(u16 *)(p + 0xAE);
            D_800E36C8_u[i].unk_08 = *(s32 *)(c + 0x8);
            func_8009A21C(*(s16 *)(p + 0xAA), *(s16 *)(p + 0xAC), 0x800);
            func_800A4300(D_80082E80, D_800E3D7C);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn the 0x212-style node through func_800A8608.

   THE THREE STACK PARAMETERS ARE `s32`. Declared s16 they are hoisted into
   $s0/$s1/$s2 at entry instead of being reloaded at their stores, and the
   function is 63 words against 59 with 60 differing -- the stack-parameter
   memory discount only applies while the parameter's mode is the slot's. AND
   THE RECORD READ NEEDS ITS OWN LOCAL: written inline at the +0x98 store the
   `lw` is scheduled eight words later, 59 / 9. 59 words, 0 differing. */
u8 *func_800A794C(s32 x, s32 z, s32 ang, u8 *p, s32 e, s32 f, s32 h)
{
    u8 *n;
    u8 *q;
    u8 *r;
    u8 *g;
    u8 *v;

    n = (u8 *)func_800A8608(D_80083498, p, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    *(u32 *)(n + 0x10) = (u32)func_800A77AC;
    q = *(u8 **)(n + 0x8);
    *(s16 *)(q + 0x2) = x * 64 + 0x20;
    *(s16 *)(q + 0x6) = z * 64 + 0x20;
    *(s16 *)(q + 0xA) = ang;
    r = n + 0x20;
    v = *(u8 **)p;
    *(s16 *)(r + 0xA0) = 4;
    *(s16 *)(r + 0xAA) = e;
    *(s16 *)(r + 0xAC) = f;
    *(u8 **)(r + 0x98) = v;
    *(s16 *)(r + 0xAE) = h;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    return n;
}

/* ONE RESULT VARIABLE, NOT TWO RETURNS: the call block is a branch target
   rather than a fall-through, so cse's table does not reach it and p[1] is
   reloaded for the call. Two returns let jump.c swap the arms (7
   differing). 17 words, 0 differing. */
u32 func_800A7A38(u8 *p)
{
    u32 r;

    if (p[1] == 0x12)
    {
        r = (u32)D_8006E240;
    }
    else
    {
        r = func_8004A658(p[1], p[0]);
    }
    return r;
}

/* Claim a stride-4 slot for a new spot, or report that it could not be done.
   The record's fields go in through UnkDungeonSpotD rather than a `u8 *`
   cast: see that type's comment for the four words it costs otherwise.
   122 words, 0 differing. */
s32 func_800A7A7C(s16 x, s16 y, s16 z, s32 u, u8 *w)
{
    void *m;
    void *h;
    s32 i;
    UnkDungeonSpotD *r;
    s16 a;
    s16 b;
    s16 c;

    if ((s16)func_800A7234(x, y, z, &a, &b, &c) != 0)
    {
        i = func_800A71F4();
        if (i >= 0)
        {
            if (func_800A794C(x, y, z, w, a, b, c) == 0)
            {
                *(s32 *)&D_800E3548_s[i] = *(s32 *)w;
                r = &D_800E36C8_u[i];
                r->unk_00 = a;
                r->unk_01 = b;
                r->unk_04 = c;
                r->unk_02 = c;
                r->unk_08 = func_800A7A38(w);
                func_8009A21C(a, b, 0x800);
                func_800A4300(D_80082E80, D_800E3D7C);
            }
            return 1;
        }
    }
    func_800A7700(x, y, z, w);
    h = func_800990FC();
    m = h;
    h = func_80099368(w, h);
    h = func_80099194(D_800E0B54, h);
    func_80099290(h);
    func_800A5720(m);
    return 0;
}

/* Allocate a kind-2 node, park a handler at its +0x10 and set the
   renderable it carries to white at unit scale. The `lw 0xC` and the
   `sw $zero, 0x10` both happen BEFORE the install call, which is where the
   reads of the node have to be: func_8004491C clobbers $v0. */
void func_800A7C64(void)
{
    UnkNode *n;
    UnkDungeonSprite *s;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        n->unk_10 = 0;
        func_8004491C(n, (u32)func_800A7CC4);
        s->unk_0E = 0x80;
        s->unk_0D = 0x80;
        s->unk_0C = 0x80;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
    }
}

/* The 0x40 ground-item slots: for each live D_800E3548 slot, project
   D_800E36C8's tile into the scratchpad, then emit TWO POLY_FT4s per element
   of its 0xC-byte list -- the sprite itself and, under a second matrix that
   has been flattened by ScaleMatrix and offset by six, a semi-transparent
   shadow whose colour word is the sprite's plus 0x7FC00000.  func_800D3738's
   renderer with the slot table's outer loop around it; the u/v + CLUT and
   u/v + TPAGE halves arrive as ONE `sw` each here, which is what
   UnkDungeonFT4W above exists for.

   SIX SPELLING FACTS, each measured on this function.

   (1) `s32 i = 0;` IS THE FIRST DECLARATION.  Left as `for (i = 0; ...)`
   after the four pointer initialisers, the counter's zeroing and its own
   `sw $s6` come out seventh and eighth instead of first and second: 4 aligned
   differences and nothing else wrong.

   (2) THE BYTE-OFFSET LOCAL IS TWO VARIABLES, ONE PER GROUP.  One `s16 a`
   across both the +0x02 and the +0x03 group keeps its quantity live over the
   whole stretch, so local-alloc never ties the sum to it, hands it $t0, and
   EVERY compiler temporary in the function shifts from $t0 to $t1 -- 87
   aligned differences, all of them register numbers.  Two locals die at their
   own sums, both get $v0, and the shift goes away.

   (3) `u16 cam[3]`, NOT `s16`.  Only the `(cam[2] + 0x100)` read shows it:
   `lhu` against `lh`, 1 word.  The array has to be an array (three `sh` into
   consecutive stack halfwords at 0x28); three scalars would be registers.

   (4) `Unk80083160 *g = &D_80083160;`.  All nine callee-saved registers are
   taken ($s0..$s7 and $fp), so `g` gets none and reload rematerialises its
   `lui`/`addiu` at each use -- which is exactly the target's three
   re-materialisations and its closing `lw 0x0($t0)`.  Reaching the struct by
   name instead gives the symbolic `lw %lo(D_80083160)($v0)` at the tail, one
   word short of the target's three.

   (5) EACH ADDEND OF `p->unk_102` IS IN ITS OWN LOCAL.  Inline, the
   `s->unk_18 - 0x100` half is computed in HImode as `ori $v1,0xFF00` plus
   `addu` rather than one `addiu`: one word long and 15 more aligned
   differences.

   (6) THE TILE'S +0x02 IS SEEDED FROM THE SCRATCHPAD, NOT FROM +0x04 AGAIN.
   `D_800E36C8_u[i].unk_02 = sp->unk_04;` lets cse forward the halfword it has
   just stored; `= D_800E36C8_u[i].unk_04;` reloads it, because the scratchpad
   store between the two invalidates cse's memory.  1 word.  The same
   mechanism is load-bearing in the shadow's scale: `sc.vz` is assigned twice
   with the second reading the first back, and the store between the two
   `(s16)sp->unk_04` reads is why they are not commoned -- which is where the
   target's dead `-4` store and its subtract-itself pair come from.

   593 words, 0 differing. */
s32 func_800A7CC4(u8 *p, UnkVec *v, UnkSlot *s)
{
    s32 i = 0;
    Unk80083160 *g = &D_80083160;
    MATRIX *m = (MATRIX *)0x1F8000D0;
    MATRIX *mm = (MATRIX *)0x1F800050;
    Unk453E0SP *sp = (Unk453E0SP *)0x1F800000;
    UnkDungeonFT4W *q;
    Unk44D24E *e;
    u16 cam[3];
    VECTOR sc;
    s32 k;
    s32 n;

    q = *(UnkDungeonFT4W **)(g->unk_00 + 0x8D0);
    sp->unk_20 = g->unk_00 + 0xB0;
    sp->unk_E4 = 0;
    sp->unk_E8 = 0;
    sp->unk_EC = 0;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    cam[0] = g->unk_C4;
    cam[1] = g->unk_C6;
    cam[2] = g->unk_C8;
    for (; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            sp->unk_00 = (D_800E36C8_u[i].unk_00 << 6) + 0x20;
            sp->unk_02 = (D_800E36C8_u[i].unk_01 << 6) + 0x20;
            sp->unk_04 = D_800E36C8_u[i].unk_04;
            D_800E36C8_u[i].unk_02 = sp->unk_04;
            e = (Unk44D24E *)D_800E36C8_u[i].unk_08;
            sp->unk_C0 = RotTransPers((void *)sp, &sp->unk_B8, &sp->unk_90, &sp->unk_94);
            D_8006CD30.unk_1C = sp->unk_C0 * 4;
            sp->unk_C0 = sp->unk_C0 - 2;
            if ((u32)sp->unk_C0 < 0x1E0)
            {
                PushMatrix();
                sp->unk_B8 = sp->unk_B8 - 0xA0;
                sp->unk_BA = sp->unk_BA - 0x78;
                sp->unk_100 = s->unk_16;
                sp->unk_104 = g->unk_B8 + (s->unk_1A - cam[1]);
                k = (cam[2] + 0x100) & 0x1FF;
                n = s->unk_18 - 0x100;
                sp->unk_102 = k + n;
                RotMatrix(&sp->unk_100, m);
                CompMatrix(&D_8006CD30, m, mm);
                SetTransMatrix(mm);
                SetRotMatrix(mm);
                for (;;)
                {
                    if ((e->unk_00 & 0x20) == 0)
                    {
                        s32 x;
                        s32 y;
                        s16 a;
                        s16 b;
                        u8 t;

                        x = e->unk_08;
                        sp->unk_08.w = x;
                        y = e->unk_0A;
                        sp->unk_10.w = y;
                        if (x + y >= 0x100)
                        {
                            sp->unk_10.w = y - 1;
                        }
                        x = e->unk_09;
                        sp->unk_0C.w = x;
                        y = e->unk_0B;
                        sp->unk_14.w = y;
                        if (x + y >= 0x100)
                        {
                            sp->unk_14.w = y - 1;
                        }
                        a = (s8)e->unk_02;
                        sp->unk_80 = a;
                        sp->unk_70 = a;
                        a = a + sp->unk_10.h;
                        sp->unk_88 = a;
                        sp->unk_78 = a;
                        b = (s8)e->unk_03;
                        sp->unk_7A = b;
                        sp->unk_72 = b;
                        b = b + sp->unk_14.h;
                        sp->unk_8A = b;
                        sp->unk_82 = b;
                        RotTransPers4(&sp->unk_70, &sp->unk_78, &sp->unk_80, &sp->unk_88,
                                      &sp->unk_F0, &sp->unk_F4, &sp->unk_F8, &sp->unk_FC,
                                      &sp->unk_90, &sp->unk_94);
                        q->unk_08 = sp->unk_F0 + sp->unk_B8;
                        q->unk_0A = sp->unk_F2 + sp->unk_BA;
                        q->unk_10 = sp->unk_F4 + sp->unk_B8;
                        q->unk_12 = sp->unk_F6 + sp->unk_BA;
                        q->unk_18 = sp->unk_F8 + sp->unk_B8;
                        q->unk_1A = sp->unk_FA + sp->unk_BA;
                        q->unk_20 = sp->unk_FC + sp->unk_B8;
                        q->unk_22 = sp->unk_FE + sp->unk_BA;
                        if ((((u16)(q->unk_08 + 0x20) < 0x181 && (u16)(q->unk_0A + 0x20) < 0x121) |
                             ((u16)(q->unk_10 + 0x20) < 0x181 && (u16)(q->unk_12 + 0x20) < 0x121) |
                             ((u16)(q->unk_18 + 0x20) < 0x181 && (u16)(q->unk_1A + 0x20) < 0x121) |
                             ((u16)(q->unk_20 + 0x20) < 0x181 && (u16)(q->unk_22 + 0x20) < 0x121)) !=
                            0)
                        {
                            sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
                            sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
                            sp->unk_0C.w = sp->unk_0C.w << 8;
                            q->unk_0C.w = sp->unk_0C.w + sp->unk_08.w + (e->unk_06 << 16);
                            q->unk_14.w = sp->unk_0C.w + sp->unk_10.w + ((s16)e->unk_04 << 16);
                            q->unk_1C.h = sp->unk_14.h + sp->unk_08.h;
                            q->unk_24.h = sp->unk_14.h + sp->unk_10.h;
                            if ((s16)sp->unk_50 >= 0x1800)
                            {
                                t = q->unk_24.b[0];
                                q->unk_24.b[0] = t + 0xFF;
                                q->unk_14.b[0] = t;
                            }
                            if ((s16)sp->unk_58 >= 0x1800)
                            {
                                t = q->unk_24.b[1];
                                q->unk_24.b[1] = t + 0xFF;
                                q->unk_1C.b[1] = t;
                            }
                            if ((s16)q->unk_08 > (s16)q->unk_20)
                            {
                                q->unk_14.b[0] = q->unk_14.b[0] - 1;
                                q->unk_24.b[0] = q->unk_24.b[0] - 1;
                            }
                            if ((s16)q->unk_0A > (s16)q->unk_22)
                            {
                                q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                                q->unk_24.b[1] = q->unk_24.b[1] - 1;
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            SetPolyFT4(q);
                            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
                            q++;
                            PushMatrix();
                            sc.vz = (s16)sp->unk_04 - 4;
                            sc.vz = (sc.vz - (s16)sp->unk_04) * 32 + 0x1000;
                            if (sc.vz < 0)
                            {
                                sc.vz = 0;
                            }
                            sc.vx = sc.vz;
                            sc.vy = sc.vz / 2;
                            sp->unk_102 = 0;
                            sp->unk_100 = -cam[0];
                            sp->unk_104 = g->unk_B8 - cam[1];
                            RotMatrix(&sp->unk_100, m);
                            CompMatrix(&D_8006CD30, m, mm);
                            ScaleMatrix(mm, &sc);
                            SetTransMatrix(mm);
                            SetRotMatrix(mm);
                            sp->unk_70 = sp->unk_70 + 6;
                            sp->unk_78 = sp->unk_78 + 6;
                            RotTransPers4(&sp->unk_70, &sp->unk_78, &sp->unk_80, &sp->unk_88,
                                          &sp->unk_F0, &sp->unk_F4, &sp->unk_F8, &sp->unk_FC,
                                          &sp->unk_90, &sp->unk_94);
                            q->unk_08 = sp->unk_F0 + sp->unk_B8;
                            q->unk_0A = sp->unk_F2 + sp->unk_BA;
                            q->unk_10 = sp->unk_F4 + sp->unk_B8;
                            q->unk_12 = sp->unk_F6 + sp->unk_BA;
                            q->unk_18 = sp->unk_F8 + sp->unk_B8;
                            q->unk_1A = sp->unk_FA + sp->unk_BA;
                            q->unk_20 = sp->unk_FC + sp->unk_B8;
                            q->unk_22 = sp->unk_FE + sp->unk_BA;
                            q->unk_0C.w = sp->unk_0C.w + sp->unk_08.w + 0x7FC00000;
                            q->unk_14.w = sp->unk_0C.w + sp->unk_10.w + ((s16)e->unk_04 << 16);
                            q->unk_1C.h = sp->unk_14.h + sp->unk_08.h;
                            q->unk_24.h = sp->unk_14.h + sp->unk_10.h;
                            if ((s16)q->unk_08 > (s16)q->unk_20)
                            {
                                q->unk_14.b[0] = q->unk_14.b[0] - 1;
                                q->unk_24.b[0] = q->unk_24.b[0] - 1;
                            }
                            if ((s16)q->unk_0A > (s16)q->unk_22)
                            {
                                q->unk_1C.b[1] = q->unk_1C.b[1] - 1;
                                q->unk_24.b[1] = q->unk_24.b[1] - 1;
                            }
                            q->unk_04 = *(u32 *)s->unk_0C;
                            SetPolyFT4(q);
                            setSemiTrans(q, 1);
                            AddPrim(sp->unk_20 + sp->unk_C0 * 4, q);
                            q++;
                            PopMatrix();
                        }
                    }
                    if ((s8)e->unk_00 < 0)
                    {
                        break;
                    }
                    e++;
                }
                PopMatrix();
            }
        }
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

/* The three halfword stores into the mover are the 16.16 integer halves at
   +0x2/+0x6/+0xA, which is why c, d and e are s16; no caller witnesses the
   width, so the `sh` stores are the whole evidence. The second parameter is
   `void *` and not `u8 *`: func_800A7700 passes its own `s32 *q`. This is
   also the witness for func_800BC26C's fourth parameter -- it sets all four
   argument registers before the `jal`. 53 words, 0 differing. */
UnkNode *func_800A8608(void *a, void *b, s16 c, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        *(s16 *)((u8 *)m + 0x2) = c;
        *(s16 *)((u8 *)m + 0x6) = d;
        *(s16 *)((u8 *)m + 0xA) = e;
        func_8004491C(n, (u32)D_80045340);
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_08 = func_800A7A38(b);
        func_800BC26C(n, 0, 0, 0);
    }
    return n;
}

/* THE BASE IS REMATERIALISED: one `b = ...` at the top is 15 words / 7
   differing because it stays live in $a1 across the loop. Assigning it
   again before the tail store is 0. The constant written inline at every
   site is 23 words. 16 words, 0 differing. */
void func_800A86DC(s32 i)
{
    u8 *b;

    b = (u8 *)0x80010000;
    for (; i < 0x13; i++)
    {
        *(s32 *)(b + i * 4 + 0x29C) = *(s32 *)(b + i * 4 + 0x2A0);
    }
    b = (u8 *)0x80010000;
    *(s32 *)(b + i * 4 + 0x29C) = 0;
}

/* func_800A8E74's node steps here: the OTHER 0x110-node +0x10 handler (see
   func_8008D470, which is this one's sibling), a two-state machine on the
   view's +0xA0. State 0 either walks the carrier's own tile one step along
   the stored facing (+0x1C bit 0x200000) or projects
   the carrier, seeds the mover from the view's +0x8C record and arms the
   +0xA2 flag; state 1 eases the mover onto c[0x24]/c[0x25] over the +0xA4
   countdown and, when it expires, either lands on a target func_8009B25C
   finds or steps one more tile. Every path that does not return falls into
   the three 16.16 integrations at the end. 470 words, 0 differing;
   seqcheck seq=OK with raw == relocdump == 71 and no relocation on a
   function symbol.

   THE DIRECTION INDEX `i` IS `s16`, AND THAT IS THE WHOLE LAST WORD. As
   `s32` the `andi $v1, $v0, 0x7` and the variable are one pseudo and the
   `addu $s4, $v1, $zero` at 0x800A8878 never exists: 470 words, 96
   differing. An `s16` local gets an HImode pseudo here, so the assignment
   is a mode-changing copy that jump2's find_equiv_reg cannot delete, while
   the value 0..7 needs no re-extension at either `sll i, 1`. `u16` and `u8`
   are 469 / 80. func_8008D470's own comment records the same mechanism on
   its `s16` latch, so this is the second measurement of it in this unit.
   One `s16 i` serves all three index sites; three separate `s32` locals are
   also 0 but a single `s32` one is 470 / 98.

   TWO OF THE NINE `b + 0xA` ACCESSES MUST BE THE INDEX FORM `*((s16 *)b + 5)`
   -- the `+ 8` store in the 0x80000 arm and the third argument of the SECOND
   func_800BCB04 in the big arm. Only the index form leaves a top-level
   PLUS_EXPR, which is what sets MEM_IN_STRUCT_P, so sched1 lifts
   `lw %lo(D_800814A8)` and `lh 0xA($s3)` over the store beside them and the
   load-delay slot stops being a `nop`: the flat `*(s16 *)((u8 *)b + 0xA)` is
   471 / 9 and 471 / 5 respectively. The other seven are ties, so they keep
   the unit's usual spelling.

   func_8009B164 IS CALLED THROUGH A PROTOTYPED CAST. Its landed definition
   takes `u16`, which makes the argument `lhu`; the target has `lh`, i.e. the
   original translation unit had no prototype in scope and the argument
   promoted to `int`. The cast emits the same `jal` and the same relocation.
   Calling it directly is 470 / 1.

   Also measured, each against the landed body: `h` as `s16` with the `(s16)`
   casts dropped 470 / 2 (the compares read the call's return register
   instead of h's own); one `(u8 *)&D_80083460` local instead of two 470 / 4;
   `x` and `y` merged, or hoisted out of the subtraction, 470 / 3 each
   (func_800AB1C0's idiom); `e` inlined twice instead of held in a local
   472 / 9, because the `sb` to c[0x24] wipes cse's memory table; dropping
   `q = p` and using one pointer 464 / 88. */
void func_800A871C(u8 *p, UnkVec *b, u8 *c)
{
    s16 out[4];
    u8 *q;
    u8 *d;
    u8 *e;
    u8 *g;
    u8 *g2;
    s16 i;
    s32 h;
    s32 x;
    s32 y;

    q = p;
    if (*(u8 **)(q + 0x9C) == D_80081484 && (*(u8 **)(q + 0x9C))[1] == 0)
    {
        *(u8 **)(q + 0x9C) = (u8 *)&D_800E3540;
    }
    switch (*(s16 *)(p + 0xA0))
    {
    case 0:
        if ((*(s32 *)(q + 0x1C) & 0x200000) != 0)
        {
            *(s16 *)(q + 0x2A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            if (func_8003DE58(*(u32 *)(*(u8 **)(*(u8 **)*(u8 **)*(u8 **)(*(u8 **)(p + 0x90) + 0x28) +
                                                (*(u8 **)(*(u8 **)(p + 0x90) + 0x2C))[2] * 4) +
                                       4),
                              *(u8 **)(p + 0x90), out, 0) == 0)
            {
                out[2] = -0x30;
            }
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(p + 0x8C) + 0xA) + (u16)out[2];
            c[0x24] = (*(u8 **)(p + 0x90))[0x24];
            c[0x25] = (*(u8 **)(p + 0x90))[0x25];
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(p + 0xA0) = *(u16 *)(p + 0xA0) + 1;
            *(s16 *)(q + 0x6A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            i = (*(u16 *)(q + 0x6A) >> 9) & 7;
            if (*(s16 *)(q + 0x8A) != 0)
            {
                h = func_800BCB04((((c[0x24] + D_8006CCD8[i] * *(s16 *)(q + 0x8A)) << 6) + 0x20) & 0xFFE0,
                                  (((c[0x25] + D_8006CCE8[i] * *(s16 *)(q + 0x8A)) << 6) + 0x20) & 0xFFE0,
                                  *(s16 *)((u8 *)b + 0xA));
                if ((s16)h < 0x200)
                {
                    b->unk_14 = (((s16)h - *(s16 *)((u8 *)b + 0xA)) << 15) / *(s16 *)(q + 0x8A);
                }
                else
                {
                    b->unk_14 = ((*(s16 *)(*(u8 **)(q + 0x60) + 0x88) - *(s16 *)((u8 *)b + 0xA)) << 15) /
                                *(s16 *)(q + 0x8A);
                }
            }
            c[0x24] += *(u8 *)&D_8006CCD8[i];
            c[0x25] += *(u8 *)&D_8006CCE8[i];
            *(s16 *)(p + 0xA4) = 2;
            return;
        }
        if (func_8003DE58(*(u8 **)(*(u8 **)(p + 0x90) + 0x8), *(u8 **)(p + 0x90), out, 0) != 0 &&
            (*(s32 *)(q + 0x1C) & 0x80000) != 0)
        {
            *(s16 *)(p + 0xA2) = 1;
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(*(u8 **)(p + 0x8C) + 0x2) + (u16)out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(*(u8 **)(p + 0x8C) + 0x6) + (u16)out[1];
            *((s16 *)b + 5) = *(u16 *)(*(u8 **)(p + 0x8C) + 0xA) + (u16)out[2] + 8;
            *(s16 *)(q + 0x88) = *(u16 *)((u8 *)D_800814A8 + 0x88);
        }
        else if (*(s16 *)(p + 0xA2) == 0)
        {
            return;
        }
        else if ((*(s32 *)(q + 0x1C) & 0x80000) == 0)
        {
            e = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
            *(s16 *)(q + 0x6A) = *(u16 *)(*(u8 **)(q + 0x60) + 0x2A);
            i = (*(u16 *)(q + 0x6A) >> 9) & 7;
            c[0x24] = e[0x24] + *(u8 *)&D_8006CCD8[i];
            c[0x25] = e[0x25] + *(u8 *)&D_8006CCE8[i];
            func_800A2B04(b, c[0x24], c[0x25]);
            h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(s16 *)((u8 *)b + 0xA));
            if ((s16)h > 0x200)
            {
                h = *(u16 *)((u8 *)b + 0xA);
            }
            func_800A7A7C(c[0x24], c[0x25], h, *(s32 *)(c + 0x8), *(u8 **)(p + 0x9C));
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            *(s16 *)((u8 *)b + 0xA) =
                func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *((s16 *)b + 5));
            func_800B66C8((u8 *)b);
            func_8009A028(q);
            return;
        }
        if (*(s32 *)(D_800E3D7C + 0x124) != 0)
        {
            break;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_8009A028(q);
        return;
    case 1:
        if (*(s16 *)(p + 0xA4) != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(p + 0xA4);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(p + 0xA4);
        }
        *(s16 *)(p + 0xA4) = *(u16 *)(p + 0xA4) - 1;
        if (*(s16 *)(p + 0xA4) > 0)
        {
            break;
        }
        d = func_8009B25C(q, c[0x24], c[0x25], *(s16 *)(q + 0x88));
        if (d != NULL)
        {
            if (d[0x13] == 0x1F && (*(s32 *)(d + 0x1C) & 0x228) == 0 && D_800DD7DC != 0)
            {
                *(s32 *)(d + 0x14) |= 0x800000;
                *(s32 *)(d + 0x48) = **(s32 **)(p + 0x9C);
            }
            else if (func_80098920(d, *(u8 **)(p + 0x9C), 0xD, (s32)q) == 0)
            {
                goto cont;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            func_8009A028(q);
            g = (u8 *)&D_80083460;
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) - 1;
            return;
        }
    cont:
        if ((s16)((s32 (*)(s32, UnkVec *, u8 *))func_8009B164)(*(s16 *)(q + 0x6A), b, c) != 0)
        {
            if (--*(s16 *)(q + 0x8A) > 0)
            {
                func_800A2B04(b, c[0x24], c[0x25]);
                i = (*(u16 *)(q + 0x6A) >> 9) & 7;
                c[0x24] += *(u8 *)&D_8006CCD8[i];
                c[0x25] += *(u8 *)&D_8006CCE8[i];
                *(s16 *)(p + 0xA4) = 2;
                break;
            }
        }
        h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(s16 *)((u8 *)b + 0xA));
        if ((s16)h > 0x200)
        {
            h = *(u16 *)((u8 *)b + 0xA);
        }
        func_800A7A7C(c[0x24], c[0x25], h, *(s32 *)(c + 0x8), *(u8 **)(p + 0x9C));
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_8009A028(q);
        g2 = (u8 *)&D_80083460;
        *(s16 *)(g2 + 0xA) = *(u16 *)(g2 + 0xA) - 1;
        break;
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
}

/* Spawn the 0x110 node func_800A871C steps, seeding the mover's three 16.16
   integer halves and the view's +0x88 either from the D_800E36C8 spot record
   `idx` names or, when `idx` is negative, from the caller's own record and
   source view. The tail is func_800A8608's word for word: the engine list,
   the three 0x80 colour bytes, the two 0x1000 scales, and func_800A7A38's
   handle into the renderable's +0x08.

   THE TABLE ARM IS WRITTEN FIRST. Both arms fall into the shared tail, so
   they are emitted in source order; writing the `idx < 0` arm first is
   108 words, 38 differing, with the `bltz` coming out as a `bgez`.

   THE +0x98 STORE GOES BEFORE THE +0x9C AND +0x13 STORES. Written last, the
   two `lw`s off `d` get different registers, the second one floats above its
   own store and the whole five-store block reschedules: 108 words,
   9 differing. Caching the two loads in one reused `s32` local instead is 11
   differing, and with the +0x98 load cached as well 4. Moving only the +0x98
   store between +0x9C and +0x13 is 107 words.

   The sixth parameter is `s16`: it arrives as a stack word (`lw` at
   0x44($sp)) and is re-extended with `sll 16`/`sra 16` at its one use, and
   both ASM callers pass a sign-extended halfword. The third is never read.
   108 words, 0 differing. */
UnkNode *func_800A8E74(u8 *a, u8 *b, s32 c, u8 *d, u8 *e, s16 idx)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;

    n = func_8003FD64(0x110, a - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A871C;
        p = (u8 *)n + 0x20;
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        *(u8 **)(p + 0x60) = d;
        if (idx >= 0)
        {
            *(s16 *)((u8 *)m + 0x2) = (D_800E36C8_u[idx].unk_00 << 6) + 0x20;
            *(s16 *)((u8 *)m + 0x6) = (D_800E36C8_u[idx].unk_01 << 6) + 0x20;
            *(s16 *)((u8 *)m + 0xA) = D_800E36C8_u[idx].unk_02;
            *(s16 *)(p + 0x88) = D_800E36C8_u[idx].unk_04;
        }
        else
        {
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(b + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(b + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(b + 0xA);
            *(s16 *)(p + 0x88) = *(u16 *)(d + 0x88);
        }
        func_8004491C(n, (u32)D_80045340);
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_06 = 2;
        r->unk_08 = func_800A7A38(e);
        *(s32 *)(p + 0x8C) = *(s32 *)(d - 0x18);
        *(s32 *)(p + 0x90) = *(s32 *)(d - 0x14);
        *(s32 *)(p + 0x98) = *(s32 *)e;
        *(u8 **)(p + 0x9C) = p + 0x98;
        *(p + 0x13) = 0xFF;
        func_80099FDC(n);
        func_800BC26C(n, 0, 0, 0);
    }
    return n;
}

/* Set bit (i & 7) of a bitmap at 0x80015720 for every live entry of every
   D_80073414 descriptor whose +0x01 has the caller's bit.

   0x80015720 IS A CONSTANT ADDRESS, NOT A SYMBOL: a declared array (or a
   sized, scalar or 2-D alias of it) materialises the whole `lui`/`addiu` as a
   loop invariant and the `%lo` leaves the `lbu`/`sb` -- 50 words against 49.
   THE CONSTANT BASE MUST BE A POINTER LOCAL RE-ASSIGNED AT THE TOP OF THE
   OUTER LOOP: at function scope loop.c turns `b + j * 8` into a giv, 21
   differing; inside the inner loop 17; inside the `if` 30.

   Both `addu`s are INDEX-FIRST, so both operands must reach them as integers:
   `i * 0x14 + e->unk_0C` is pointer arithmetic and comes out base-first, 1
   differing, and so does `(j * 8 + i / 8) + b`. Note the scaled operand is
   the base's PARTNER, not the base, so the "unscaled index only" caveat on
   that lever does not hold here. 49 words, 0 differing. */
void func_800A9024(s32 bit)
{
    s32 i;
    s32 j;
    Unk80073414 *e;
    u8 *q;
    u8 *b;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        e = &D_80073414[j];
        if (((*((u8 *)e + 1) >> bit) & 1) == 0)
        {
            continue;
        }
        for (i = 0; i < e->unk_02; i++)
        {
            q = (u8 *)(i * 0x14 + (s32)e->unk_0C);
            if ((*(u16 *)q & 0x200) == 0)
            {
                *(u16 *)q |= 0x400;
                b[(j * 8 + i / 8) + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* Clear bit 0x400 of the halfword at the head of entry p[0] of the
   0x14-stride table D_80073414[p[1]].unk_0C points at. 30 words,
   0 differing. */
void func_800A90E8(u8 *p)
{
    u8 *q;

    func_8004A8D8(p[1], p[0]);
    q = D_80073414[p[1]].unk_0C;
    *(u16 *)(q + p[0] * 0x14) &= 0xFBFF;
}

/* Rebuild the 0x13 x 8-byte occupancy bitmap in low RAM.

   THE BITMAP IS A CONSTANT BASE, NOT A SYMBOL: as `extern u8 D_80015720[]`
   the whole address is materialised and hoisted, the stores lose their
   displacement and the function is 53 words. A 2-D `[][8]` declaration
   reaches 52 but keeps the row base in a register, 38 differing. `b` IS
   ASSIGNED INSIDE THE OUTER LOOP so loop.c hoists it after `j = 0`; at
   function scope the `lui` is emitted first, 2 differing. AND THERE IS NO
   OFFSET VARIABLE: `o += 0x14` as a source local puts `o = 0` ahead of the
   loop guard, 9 differing. 52 words, 0 differing. */
void func_800A9160(void)
{
    u8 *b;
    s32 j;
    s32 i;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            if ((i & 7) == 0)
            {
                b[(j << 3) + i / 8 + 0x5720] = 0;
            }
            if (*(u16 *)(D_80073414[j].unk_0C + i * 0x14) & 0x400)
            {
                b[(j << 3) + i / 8 + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* The one-shot 0x20-kind prompt, and then a random D_800E3548 slot claim.
   THE RESULT IS A FOUR-BYTE STACK OBJECT, not a register value: it is
   zeroed as a word, filled two bytes at a time in one arm and replaced
   whole in the other, and taking its address is what keeps every access in
   memory.
   THE THREE SLOT TESTS MUST BE SEPARATE `if`s. Written
   `k != 0 && k < 0xB && k == 1` the front end's range folding collapses the
   first two into the third and the function is 84 words / 25 differing --
   exactly the `beqz` / `sltiu 0xB` / `beqz` triple and its load-delay nop
   missing. Nested, they survive.
   AND D_80012090 IS THE CONSTANT ADDRESS, as it is everywhere else in this
   unit: as the declared symbol the pair is `lui $v0` / `lw $v1, %lo($v0)`
   against the target's `lui $v1` / `lw $v1, %lo($v1)` -- 2 differing, and
   NOT hidden by the relocation mask. 88 words, 0 differing;
   `seqcheck` reports the two constant-address words with raw == relocdump
   at 20 == 20. */
s32 func_800A9230(u8 *p)
{
    UnkDungeonSlot4 s;
    s32 i;

    *(s32 *)&s = 0;
    if (p[0x13] == 0x20 && D_800E3CCC == 0)
    {
        D_800E3CCC = 1;
        if (*(s32 *)0x80012090 == 0)
        {
            if (D_8008146C == 0x19 && func_80033BC0(0x11F9) != 0 &&
                func_80033BC0(0x11FC) == 0 && (s16)func_800A697C(0xD, 2) == 0)
            {
                s.unk_00 = 2;
                s.unk_01 = 0xD;
                *(s16 *)(*(u8 **)(p - 0x14) + 0x12) = 0xC;
            }
        }
    }
    if (D_800DCF5A != 0 && p[0x49] == 0 && (func_800A6D30() & 3) == 0)
    {
        i = (u16)func_800A6DA4(1, 0x1F);
        if (D_800E3548_t[i].unk_01 != 0)
        {
            if (D_800E3548_t[i].unk_01 < 0xB)
            {
                if (D_800E3548_t[i].unk_01 == 1 && D_800E3548_t[i].unk_00 == 8)
                {
                    *(s32 *)&s = *(s32 *)&D_800E3548_t[i];
                    *(s32 *)&D_800E3548_t[i] = 0;
                }
            }
        }
    }
    return *(s32 *)&s;
}

/* THE LOCAL IS `s16`. The target copies the `lbu` result into a second
   register before the three compares; `u8 v` makes that copy an
   `andi $v1, $a0, 0xFF` (1 differing) and `s32 v` deletes it altogether,
   loading straight into the compare register (27 of 28 words, 19 differing).
   Only the widening copy of a NARROW local is a bare `addu`. This is the
   function the c6 batch left at 1 differing after twenty-one declarations;
   `s16` was not among them. 28 words, 0 differing. */
s16 func_800A9390(s16 idx)
{
    s16 v;

    v = D_800DD8E7[idx];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (v == 0x32)
        {
            return 0x33;
        }
        if (v == 0x39)
        {
            return 0x3A;
        }
        if (v == 0x40)
        {
            return 0x42;
        }
    }
    return v;
}

/* `s32 c`, NOT `u8 c`: an lbu already zero-extends, but a QImode local makes
   each compare carry its own `andi 0xFF` and the function is 41 words.
   40 words, 0 differing. */
s16 func_800A9400(s16 x)
{
    s32 c;

    c = D_800DD8E7[x];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (c == 0x32)
        {
            return 7;
        }
        if (c == 0x39)
        {
            return 8;
        }
        if (c == 0x40)
        {
            return 9;
        }
    }
    else
    {
        if (c == 0x32)
        {
            return 1;
        }
        if (c == 0x39)
        {
            return 2;
        }
        if (c == 0x40)
        {
            return 3;
        }
    }
    return x;
}

/* 227/227 words, 0 differing.  Came down from 76 aligned differences over two
   days of parking; the five levers below are what closed it, and every device
   in the body is load-bearing -- a leave-one-out sweep costs 8, 16, 21, 30 and
   45 rows respectively.

   1. `y' AND `z' ARE TWO SEPARATE SINGLE-SET LOCALS, not one variable and not
      the parameter `q'.  The target's `addu $a0, $s4, $zero' comes BEFORE
      `addu $s3, $v0, $zero' at words 169/170, and that order is unreachable
      while the call result lands in a variable with more than one set:
      expand_call pins the return copy's LUID immediately after the call, and
      sched.c's rank_for_schedule falls through to INSN_LUID, so the copy comes
      out first.  What flips it is `adjust_priority': the return copy becomes a
      BIRTHING insn (LAUNCH_PRIORITY 7f000001 in the ready list) only when its
      destination pseudo has reg_n_sets == 1, and then it wins the tie and is
      emitted second.  `z' therefore has to be assigned exactly once -- the
      `if (q) { q = func_800990FC(); } else { ... }' device this park carried
      for two days gave it two sets and made those two words unreachable.
      Same for `y': func_80069EF8's result needs its own single-set local, and
      all three of q, y and z end up in $s3 because their ranges are disjoint.

   2. `if (y) { func_800C77D0(...); } else { func_800C77D0(...); }' -- a
      duplicated-arm device whose only purpose is a free reference on `y'.
      Without it the dead `addu $s3, $v0, $zero' at word 70 (func_80069EF8's
      result, never read on any path) is deleted and the whole file rotates:
      45 rows.  The condition must be `y' and the device must sit inside the
      `(s16)c == 0' arm, where `y' is defined.

   3. `if (cc) { cc = c; } else { cc = c; }' READS `cc' BEFORE IT IS SET, and
      that is the one device here that is not ordinary C.  It is worth 21 rows
      and nothing defined reproduces it: every other condition (c, a, q, n, s,
      e, y, z, t, q0, q1, k, cc2, h, m, p, g, src, o), initialising `cc' to 0
      or to `c' first, moving the device onto `cc2', and a cancelled
      `cc++; cc--;' in its place ALL measure 21.  Both arms store the same
      value and `cc' is never read again, so nothing the function computes
      depends on the branch, and gcc 2.7.2 does not exploit the UB.
      `cc2 = c;' must stay AHEAD of it -- after it costs 10 rows.

   4. TWO CANCELLED PAIRS, `n++; n--;' (30 rows) and `q0++; q0--;' (16 rows).
      Each compiles to nothing -- combine folds the two addiu -- but flow
      counts four references per pair, and global.c's allocno_compare orders
      the queue by floor_log2(n_refs) * n_refs / live_length * 10000, with the
      numbers printed by `dump_flow_info' at the top of the `.lreg' dump.  The
      `q0' pair takes q0 from 10 refs/148 insns to 12/152, i.e. 2027 -> 2368,
      which lifts it past `y' (2 refs/9 insns = 2222) so q0 takes $s2 and
      y/q/z take $s3.  `q0 += 1; q0 -= 1;' and `n += 1; n -= 1;' are the same
      bytes; about 30 statement positions for the q0 pair all land.

   5. EACH ARM OF THE LAST `if' GETS ITS OWN BASE AND RESULT POINTER, and the
      base is materialised BEFORE the index: `b1 = (u8 *)D_8006DE24_n;
      n1 = b1 + q0 * 0x14;'.  The target materialises %hi/%lo(D_8006DE24)
      first in each arm; `(u8 *)&D_8006DE24_n[q0]' makes expand emit the index
      chain first and the base last, and nothing moves it back.  One shared
      base is a global quantity by flow.c:2561 (two sets, two blocks) and
      global_alloc gives it a register of its own after local_alloc has spent
      $v1/$v0 on the block-local HIGH temps; per-arm locals keep it block-local
      so the HIGH and the LO_SUM share $a0 as the target has them.  Worth 15
      rows on the park's 26-row body in two halves (base-before-index 7,
      per-arm locals 8); writing both arms back as `(u8 *)&D_8006DE24_n[q0]'
      on THIS body is 16 rows.

   And two spellings that must not be touched, both already in the park:
   `q0 = func_800A9400(q0);' self-assigns so q0 keeps a second set and combine
   cannot fold the three sll/sra pairs away (`k = func_800A9400(q0);' is 16
   rows here and was 222 words on the park's earlier body), and the two
   D_800DCEAC/D_800DCEBC halves are `>> 1', not `/ 2' -- `/ 2' is 233 words. */
s32 func_800A94A0(u8 *a, u8 *q, s32 c, u8 *e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *p;
    u8 *s;
    u8 *g;
    u8 *src;
    u8 *o;
    void *h;
    s16 q0;
    s32 cc;
    s32 cc2;
    u8 q1;
    s16 k;
    s32 t;
    u8 *y;
    u8 *z;

    s = *(u8 **)(a - 0x14);
    n = func_8003FD64(0x12, D_80083498);
    n++;
    n--;
    if (n != NULL)
    {
        cc2 = c;
        if (cc)
        {
            cc = c;
        }
        else
        {
            cc = c;
        }
        q0 = q[0];
        D_800E3D68 = q[1];
        t = func_800A982C(q0, (s16)c, a[0x13]);
        q0++;
        q0--;
        if (t == 0)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return 0;
        }
        n->unk_10 = t;
        p = (u8 *)n + 0x20;
        *(u8 **)p = a;
        *(u8 **)(p + 0x4) = e;
        p[0x8] = q0;
        D_800E3CC8[0] = q0;
        q1 = q[1];
        p[0x9] = q1;
        D_800E3CC8[1] = q1;
        if ((s16)c == 0)
        {
            m = n->unk_08;
            src = *(u8 **)(a - 0x18);
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)(src + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)(src + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)(src + 0xA);
            y = (u8 *)func_80069EF8();
            if ((D_80013714 & 1) == 0)
            {
                o = *(u8 **)(a + 0x60);
                if (o != NULL)
                {
                    func_8009FD40(*(u8 **)(a - 0x14), *(u8 **)(o - 0x14));
                }
            }
            if (y)
            {
                func_800C77D0(n, (u8 *)m, 8, 0x300);
            }
            else
            {
                func_800C77D0(n, (u8 *)m, 8, 0x300);
            }
        }
        else
        {
            g = D_80082E80;
            if ((*(u16 *)(s + 0x14) & 0x8000) == 0 && (*(u16 *)(g + 0x14) & 0x8000) == 0)
            {
                func_800C7A3C(g, s, *(s16 *)(D_800E3D7C + 0x88), *(s16 *)(a + 0x88), 8, 0x300);
            }
            else
            {
                k = (*(u16 *)(D_800E3D7C + 0x2A) >> 9) & 7;
                func_800C78A0(a - 0x20, (g[0x24] << 6) + (*(s16 *)&D_800DCEAC[k] >> 1) + 0x20,
                              (g[0x25] << 6) + (*(s16 *)&D_800DCEBC[k] >> 1) + 0x20,
                              *(s16 *)(D_800E3D7C + 0x88), 8, 0x300);
            }
        }
        if ((s16)cc2 != 0)
        {
            func_800AD594(a, D_800DD8B4[q0] << 6);
        }
        else
        {
            func_800AD594(a, D_800DD880[q0] << 8);
        }
        if (n != NULL && *(s8 *)(a + 0x13) > 0)
        {
            z = (u8 *)func_800990FC();
            h = func_80099734(a, z);
            h = func_80099194(D_800E1C58, h);
            if ((s16)cc2 != 0)
            {
                u8 *b1;
                u8 *n1;

                q0 = func_800A9400(q0);
                b1 = (u8 *)D_8006DE24_n;
                n1 = b1 + q0 * 0x14;
                h = func_80099194(*(u8 **)(n1 + 8), h);
            }
            else
            {
                u8 *b2;
                u8 *n2;

                b2 = (u8 *)D_8006DE24_n;
                n2 = b2 + q0 * 0x14;
                h = func_80099194(*(u8 **)n2, h);
            }
            func_80099290(func_80099194(D_80089080, h));
            func_800A5720(z);
        }
    }
    return (s32)n;
}

/* Re-upload the dungeon HUD's 0xC x 0x18 tile when the (kind, variant) pair
   at D_800DCF4D changes, then hand func_8003F540 the pair of words the
   variant's table holds, and leave D_800DCF59 as a two-frame handshake.
   118 words, 0 differing.

   THE "CHANGED" FLAG MUST BE NARROW. As `s32 f` the function is 116 words:
   both `if (f != 0)` tests become a bare `beqz $s3` where the target copies
   the flag into $v0 first (`addu $v0, $s3, $zero` at words 26/28 and 97).
   With `s16 f` the read of the flag is a mode conversion whose extension
   combine drops -- nonzero_bits is 1 -- leaving a plain register copy cse
   cannot propagate away, which is those two words.
   docs/matching-levers.md :16019. `s8` measures identically; `u8` and `u16`
   are 118 words at 3 differing.

   0x80024000 IS A CONSTANT ADDRESS, not a symbol: words 2/5 are a
   `lui`/`ori` pair, which is the assembler's `li` macro and not a
   `%hi`/`%lo` relocation. It is held in a local because it is used twice --
   as func_8003F6D4's second argument and as the function's return value.

   ARITY: no landed caller. The stub func_800A94A0 sets $a0 (delay slot),
   $a1 and $a2 for this `jal` -- `lbu 0x13($s4)` two insns ahead -- so the
   third parameter is real but unread here, and it is left `s32`. */
s32 func_800A982C(s16 a, s16 b, s32 c)
{
    s32 *q;
    u8 *tbl;
    s16 f;
    s32 t;
    s16 k;

    q = (s32 *)0x80024000;
    if (D_800DCF59[0] == 0)
    {
        f = 0;
        if (a != (D_800DCF4D & 0x3F) || ((D_800DCF4D >> 6) & 1) != b)
        {
            f = 1;
        }
        if (f != 0)
        {
            if (b != 0)
            {
                t = func_800A9390(a) * 12 + 0x607F;
            }
            else
            {
                t = (a - 1) * 12 + 0x607F;
            }
            func_8003F6D4(0xC, q, D_800E58B0, t);
            func_8003E4FC(6, D_800E58B0, 0);
            func_8003E4FC(0xFF, D_8003E140, (s32)D_800DCF59);
            D_800DCF4D = a | (b << 6);
        }
        if (b != 0)
        {
            tbl = D_800DDAB8;
            k = func_800A9400(a) * 2;
        }
        else
        {
            tbl = D_800DD918;
            k = a * 2;
        }
        func_8003F540(0, D_8006CD58, *(u32 *)(tbl + k * 4), *(u32 *)(tbl + k * 4 + 4));
        func_8003E4FC(0x15, (u8 *)func_800445E0(), 0);
        if (f != 0)
        {
            D_800DCF59[0] = 2;
            return 0;
        }
    }
    else
    {
        if (D_800DCF59[0] != 1)
        {
            return 0;
        }
        D_800DCF59[0] = 0;
    }
    return *q;
}

void func_800A9A04(u8 *p)
{
}

/* The per-frame tick for an actor's 0x6D countdown. TWO SPELLINGS COST TWO
   WORDS EACH. 0x80013714 has to be the CONSTANT ADDRESS: as structs.h's
   symbol the `high` is a separate insn, and because the use is inside the
   loop, loop.c hoists it into a callee-saved register -- 62 words against 60,
   the two being $s2's save and restore. And the 0x2F bound is UNSIGNED: an
   s32 local gives `slti` where the target has `sltiu`. 60 words,
   0 differing. */
void func_800A9A0C(u8 *p)
{
    u32 v;

    p[0x71] &= 0x7F;
    if ((D_80083462 & 0x2800) != 0)
    {
        return;
    }
    if (*(s8 *)(p + 0x6D) != 0)
    {
        do
        {
            if ((*(u16 *)0x80013714 & 0x8) == 0)
            {
                if ((*(u32 *)(p + 0x1C) & 0x8) == 0)
                {
                    v = p[0x13];
                    if (v < 0x2F || v == 0x39)
                    {
                        func_800AD594(p, 4);
                    }
                }
                func_800A4ACC(p);
            }
            *(s8 *)(p + 0x6D) -= 1;
        } while (*(s8 *)(p + 0x6D) != 0);
    }
    *(u16 *)(p + 0x46) &= 0x7FFF;
    func_800AA53C(p);
}

/* Push the actor's grid square onto the record's +0x73/+0x7B queue.

   THREE THINGS THE LISTING FORCES. `(s16)func_80042900(...)` assigned to an
   `s32` puts the `sll`/`sra` pair at the ASSIGNMENT, which is what makes the
   two `bnez` test the sign-extended $s4; an s16 local narrows at the use
   instead (70 words / 26) and dropping the cast is 68 / 54. The whole FLAG
   WORD is hoisted into a local before the two byte arguments -- hoisting the
   already-MASKED boolean instead measures identically to not hoisting at all
   (4 differing), because what decides sched1's order is which load's
   statement comes first, not where the mask is. And the queue stores are
   written base, index, constant last: the subscript form folds the constant
   in first and leaves the index as the `addu`'s left operand, 2 differing.
   69 words, 0 differing. */
void func_800A9AFC(u8 *a, u8 *b)
{
    s32 t;
    s32 u;
    s32 x;
    s32 y;
    s32 x2;
    s32 y2;
    s32 f;

    if ((b[0x71] & 0x7F) != 0)
    {
        t = (s16)func_80042900((s8 *)b, 0x1B);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A3D0(x2, y2, u);
        }
        x = b[0x74];
        y = b[0x7C];
        func_80069F28(b + 0x74, b + 0x75, 0xF);
        *(b + b[0x71] + 0x73) = a[0x24];
        *(b + b[0x71] + 0x7B) = a[0x25];
        a[0x24] = x;
        a[0x25] = y;
        *(s16 *)(b + 0x88) = *(u16 *)(b + 0x8A);
        *(s16 *)(b + 0x2A) = *(u16 *)(b + 0x6A);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A21C(x2, y2, u);
        }
        *(s16 *)(b + 0x8A) = 0;
    }
}

void func_800A9C10(void)
{
}

/* The arrival tail for a kind-(d & 3) == 2 entity: pick the cast or
   D_80081488 for the renderable's +0x28, place the mover, measure the leash
   into +0x88, dress the slot, face it off D_80083160's +0xC8 stamp, light
   the node and hang the 0x110 marker, then set or clear the slot's bit 0
   from D_8006CCF8. 136 words, 0 differing on the first compile.

   func_800B22F0 (csrc/ovl_dungeon.c:12451) is the line-for-line template for
   everything from the func_8009FB34 call to func_800BC26C; only the head,
   the D_80083160 facing lookup and the D_8006CCF8 tail are new.

   THE HEAD'S `(d & ~3)` IS A SEPARATE s16 LOCAL: the `sll 16`/`bnez` pair is
   the HImode comparison, where the `(d & 3) == 2` test next to it needs no
   extension because `andi 3` has already proved the range. */
void func_800A9C18(UnkNode *n, UnkVec *m, UnkSlot *r, s16 d)
{
    u8 *p;
    u8 *g;
    s16 id;

    p = (u8 *)n + 0x20;
    id = d & ~3;
    if ((d & 3) == 2)
    {
        if (id == 0)
        {
            r->unk_28 = func_80047D44(p[0x13]);
        }
        else
        {
            r->unk_28 = D_80081488;
        }
        D_800DCF4D = -1;
    }
    else
    {
        if (p[0x13] != 0x38)
        {
            func_80048088(p[0x13]);
        }
        func_8004827C(n, p[0x13]);
    }
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s16 *)(p + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                       *(u16 *)((u8 *)m + 0xA) - 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    r->unk_14 |= 0x8000;
    g = (u8 *)&D_80083160;
    func_80047784((u8 *)r,
                  (*(u8 **)((u8 *)r + 0x2C))[((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7],
                  0);
    func_80099FDC(n);
    if ((*(u32 *)(p + 0x14) & 0x4000) == 0)
    {
        func_800A34BC(p);
    }
    func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                  (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0x71] = 0;
    *(u32 *)(p + 0x1C) |= 0x40000000;
    func_800BC26C(n, 0, (s32)((u8 *)r + 0x2C), (s32)(p + 0x2A));
    if (D_8006CCF8[((*(s16 *)(g + 0xC8) + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7] != 0)
    {
        r->unk_14 |= 1;
    }
    else
    {
        r->unk_14 &= 0xFFFE;
    }
}

/* A COMMON RETURN VARIABLE, not two returns. With `return 1;` as its own
   statement reorg lifts the `li 1` into the `beqz` delay slot and the
   function is 12 words; one local keeps the `nop` and the `j`.
   14 words, 0 differing. */
s32 func_800A9E38(u8 *p)
{
    u8 *r;
    s32 v;

    r = *(u8 **)(p - 0x14);
    if (*(s32 *)r != 0)
    {
        v = (u32)(p[0x13] - 0x2E) < 2;
    }
    else
    {
        v = 1;
    }

    return v;
}

/* The actor's per-frame gate for the +0x100000 and +0x400/+0x8000 modes: on
   the record's 0x100000 flag it re-points the slot's sprite for the current
   eight-way facing through the +0x2C table, sets or clears the slot's bit 0
   from D_8006CCF8 and reports 1; otherwise it ages the +0x46 mode word, keeps
   the +0x98 0x400 bit in step with the slot's 0x8000 flag, and then runs
   func_800B253C's claim-and-dispatch tail verbatim, always reporting 0.

   THE +0x46 GATE IS TWO NESTED `if`s, NOT AN `&&`. Written
   `(w & 0x8000) != 0 && (w & 0x4000) == 0` fold_truthop merges the pair into
   one `andi 0xC000` / `bne 0x8000` -- 247 words, 163 differing. This is the
   existing fold_truthop lever read in the direction that COSTS words.

   THE SLOT'S 0x8000 TEST IS SPELT `== 0`, with the `|= 0x400` in the `else`.
   Written `!= 0` with the redraw in the `else if` the two arms come out in
   the other order: 249 words, 162 differing from that word on.

   THE +0x9A VALUE MUST REUSE THE MASK'S OWN PSEUDO -- func_800B253C's lever
   again, but here it buys a REGISTER and not the hoist. The target KEEPS
   jump.c's store-flag hoist (`addiu $v0, $zero, 1` in the `bnez`'s delay
   slot), so both

       if ((t & 0x4000) == 0) { v = 2; } else { v = 1; }
       v = t & 0x4000; if (v == 0) { v = 2; } else { v = 1; }

   emit the same 250 words here. Only the second shares $v0 between the
   `andi`, the branch and the two `li`s; the first gives `v` $a0 and is
   3 differing on exactly the two `li`s and the `sb`.

   250 words, 0 differing. */
s32 func_800A9E70(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *cur;
    u32 f;
    u32 t;
    s32 k;
    s32 m;
    s32 v;

    if ((*(u32 *)(d + 0x14) & 0x100000) != 0)
    {
        k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
        if (*(s16 *)((u8 *)p + 0x94) != k)
        {
            if (func_800A9E38(d) != 0)
            {
                func_8003DB94(c, *(void **)(*(u8 **)(c + 0x2C) + k * 4), *(s8 *)(c + 0x4));
            }
            else
            {
                func_80047738(c, (*(u8 **)(c + 0x2C))[k], *(s8 *)(c + 0x4));
            }
            *(s16 *)((u8 *)p + 0x94) = k;
        }
        if (D_8006CCF8[k] != 0)
        {
            *(u16 *)(c + 0x14) |= 1;
        }
        else
        {
            *(u16 *)(c + 0x14) &= 0xFFFE;
        }
        func_800478B8(c);
        return 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x400) != 0)
    {
        t = *(u16 *)(d + 0x46);
        if ((t & 0x8000) != 0)
        {
            if ((t & 0x4000) == 0)
            {
                m = t & 0x3FFF;
                if ((u32)(m - 0x5) < 3 || m == 0xC)
                {
                    *(u16 *)(d + 0x46) = t & 0x7FFF;
                }
            }
        }
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        if ((*(u16 *)((u8 *)p + 0x98) & 0x400) != 0)
        {
            k = ((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7;
            if (func_800A9E38(d) != 0)
            {
                func_8003DB94(c, *(void **)(*(u8 **)(c + 0x2C) + k * 4), *(s8 *)(c + 0x4));
            }
            else
            {
                func_80047738(c, (*(u8 **)(c + 0x2C))[k], *(s8 *)(c + 0x4));
            }
            *(u16 *)((u8 *)p + 0x98) &= 0xFBFF;
        }
    }
    else
    {
        *(u16 *)((u8 *)p + 0x98) |= 0x400;
    }
    if ((*(u32 *)(d + 0x1C) & 0x2000) == 0 && (*(u32 *)(d + 0x14) & 0x4000) == 0)
    {
        return 0;
    }
    if (p->unk_9A == 0x13 || p->unk_9A == 0x14)
    {
        return 0;
    }
    if (*(s8 *)(d + 0x6D) != 0)
    {
        return 0;
    }
    if (D_800DCE68 == 0)
    {
        if ((s16)func_800A2BDC(d) != 0)
        {
            return 0;
        }
    }
    else
    {
        g = (u8 *)&D_80083460;
        cur = *(u8 **)(g + 0xC);
        if (cur != d)
        {
            if (*(u32 *)(g + 0x10) != 0)
            {
                return 0;
            }
            if (cur != NULL)
            {
                return 0;
            }
            if (*(s16 *)(g + 0x8) != 0)
            {
                return 0;
            }
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return 0;
            }
            if (d[0x43] != 0xFD)
            {
                return 0;
            }
            *(u8 **)(g + 0xC) = d;
        }
        if (d[0x43] != 0xFD)
        {
            return 0;
        }
    }
    f = *(u32 *)(d + 0x1C);
    if ((f & 0x400000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x400000;
        t = *(u32 *)(d + 0x14);
        if ((t & 0x20000000) != 0)
        {
            *(u32 *)(d + 0x14) = t | 0x400000;
            func_800ACB98((u8 *)p, b, c, d);
            return 0;
        }
        *(u32 *)(d + 0x14) = t & ~0x400000;
        v = t & 0x4000;
        if (v == 0)
        {
            v = 2;
        }
        else
        {
            v = 1;
        }
        p->unk_9A = v;
    }
    else if ((f & 0x2000000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x2000000;
        d[0x71] = 0;
        if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
        {
            func_800ACD74((u8 *)p, b, c, d);
            return 0;
        }
        p->unk_9A = 0;
    }
    else
    {
        return 0;
    }
    p->unk_9B = 0;
    return 0;
}

/* State 0xA, func_800AA888's installer one state over. THE MODE IS A `u16`
   LOCAL, not the s32 func_800AA888 uses: it is live across both reports, so
   it needs a callee-saved register, and the `andi $s0, 0xFFFF` at the join is
   what the narrow type buys. The sine/cosine tables are read a BYTE at a time
   at a doubled index, which is what gives `lbu` rather than the `lh` an s16
   subscript would. 69 words, 0 differing. */
void func_800AA258(u8 *p, void *b, u8 *c, u8 *d)
{
    u16 t;
    s32 i;
    u8 *g;

    p[0x9A] = 0xA;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(c[0x24], c[0x25], t);
    i = (*(u16 *)(d + 0x6A) >> 9) & 7;
    c[0x24] += ((u8 *)D_8006CCD8)[i * 2];
    c[0x25] += ((u8 *)D_8006CCE8)[i * 2];
    func_8009A21C(c[0x24], c[0x25], t);
    *(s32 *)(d + 0x1C) &= ~0x100;
    *(s16 *)(p + 0x96) = 0x21;
    *(u16 *)(p + 0x98) |= 0x8;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* The seven-gate "can this actor act" entry: the engine must be idle
   (func_800A2BDC), the node must not be dying, neither the view's +0x1C
   0x80000 nor its +0x14 0x100000 request bit may be up, the player record
   must not be in state 0x18 or 0x11, func_800A41F0 must agree, and
   func_8009B7E4 must return a non-zero mode -- then the +0x40000000 request
   goes up, the move is reported twice and mode 1 or anything else picks
   func_800AA508 or func_800AA5E4.

   `x` AND `y` MUST BE `s16`. They hold c+0x24/c+0x25 across the
   func_8009B7E4 call, and the width decides nothing about the copies
   themselves -- `addu $a0, $s2, $zero` either way -- but it does decide
   sched1's priority for them: as `s32` both copies sink below the mode
   ternary's branch, the second one lands in the first `jal`'s delay slot
   where the target has a `nop`, and the function is 102 words.
   `u8` and `u16` are 103 words and 2 differing.

   THE GATES ARE NESTED, with ONE trailing `return 0`. Written as seven
   `if (...) return 0;` early returns the shared zero block is cross-jumped
   to a spot early in the function instead of sitting between the two
   `return 1` tails and the epilogue: 103 words, 71 differing (102 and 75
   before the `s16` fix).

   Both reports take the 0x300/0x3000 choice as an INLINE TERNARY. Lifted
   into an `s32 t` local, each one's argument setup moves below the test:
   one local for both is 103 words / 20 differing, one for the first call
   only is 102 / 57.

   TWO LANDED CALLEES HAD TO BE WIDENED FOR THIS CALL SITE -- see the report:
   func_800A2BDC was `(void)` and takes one unread pointer (all five callers
   set $a0), func_800AA508 was `(u8 *)` and takes the handler four. Both
   still measure 0 differing at 22 and 13 words.

   103 words, 0 differing. */
s32 func_800AA36C(u8 *p, void *b, u8 *c, u8 *d)
{
    s16 k;
    s16 x;
    s16 y;

    if ((s16)func_800A2BDC(d) == 0)
    {
        if ((*(u16 *)(p - 0x2) & 0x8000) == 0)
        {
            if ((*(u32 *)(d + 0x1C) & 0x80000) == 0)
            {
                if ((*(u32 *)(d + 0x14) & 0x100000) == 0)
                {
                    if (*(u8 *)(D_800814A8 + 0x9A) != 0x18 && *(u8 *)(D_800814A8 + 0x9A) != 0x11)
                    {
                        if ((s16)func_800A41F0(d) != 0)
                        {
                            x = c[0x24];
                            y = c[0x25];
                            k = func_8009B7E4(c, d);
                            if (k != 0)
                            {
                                *(u32 *)(d + 0x1C) |= 0x40000000;
                                func_8009A3D0(x, y, (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                                func_8009A21C(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                                if (k == 1)
                                {
                                    func_800AA508(p, b, c, d);
                                }
                                else
                                {
                                    func_800AA5E4(p, b, c, d);
                                }
                                return 1;
                            }
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* Installer: state 3, handler slot cleared, a 4-step life, and the
   D_80083460 counter at +0xA bumped. 13 words, 0 differing. */
/* FOUR PARAMETERS, as its twin func_800AA5E4 has: func_800AA36C calls both
   the same way. 13 words, 0 differing either way. */
void func_800AA508(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 3;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 4;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* Re-aim one record's facing and re-run its walk.

   D_80082E80'S SUBSCRIPTS ARE WRITTEN AT THE SITE. Hoisted into the same
   `u8 *r` the else arm uses, the symbol's `high` gets its own register
   (`lui $v0` / `addiu $s0, $v0`) where the target ties both to $s0: 2
   differing. `&D_80082E80[0]` is the same 2; a block-scoped `r` per arm
   measures 0, like the subscript. 42 words, 0 differing. */
void func_800AA53C(u8 *a)
{
    u8 *r;

    if (a == NULL || a[0x13] == 0)
    {
        D_80082E80[0x26] = func_8009FB34(D_80082E80[0x24], D_80082E80[0x25]);
        func_800A1B44(3, 6);
    }
    else
    {
        r = *(u8 **)(a - 0x14);
        r[0x26] = func_8009FB34(r[0x24], r[0x25]);
        func_800A19E4((s32)r, a, 3, 6, a + 0x9C);
    }
}

/* State 4. The `sll 16` / `bltz` with no `sra` is `(s16)result < 0`.
   52 words, 0 differing. */
void func_800AA5E4(u8 *p, void *b, u8 *r, u8 *d)
{
    u8 *g;

    p[0x9A] = 4;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 0x10;
    *(u16 *)(p + 0x98) |= 8;
    *(u32 *)(d + 0x1C) |= 0x10000;
    func_8009A3D0(r[0x24], r[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    do
    {
    } while ((s16)func_800A4E2C(r + 0x24, r + 0x25) < 0);
    r[0x26] = func_8009FB34(r[0x24], r[0x25]);
    func_800AA53C(d);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* THE NULL TEST NESTS, IT DOES NOT RETURN. `if (t == NULL) return 1;` as its
   own early exit materialises a third `addiu $v0, 1` and the function is 41
   words; nesting the rest inside `if (t != NULL)` lets the NULL path fall
   into the tail's `return 1` while the 0x20 path keeps its own, which is the
   two the target has. The prototype gains a return type and a pointer fourth
   parameter; the four landed callers pass a literal 0 and discard the
   result, so neither changes a byte. 40 words, 0 differing. */
s32 func_800AA6B4(u8 *p, UnkVec *b, u8 *c, u8 *t)
{
    s32 v;

    if (*(s16 *)(p + 0x64) > 0)
    {
        func_800AA754(p, b, c, p);
        return 0;
    }
    D_80083470 = p - 0x20;
    p[0x9A] = 0x7;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        if (*(s32 *)(p + 0x1C) & 0x20)
        {
            return 1;
        }
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    return 1;
}

/* 18 words, 0 differing. */
void func_800AA754(u8 *a, UnkVec *b, u8 *c, u8 *p)
{
    func_800AD4D0(p);
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

/* The actor's "give up and go back to state 5" transition. The call to
   func_800ACB98 forwards all four of this function's own parameters, which is
   why the `jal` has a bare `nop` delay slot and no argument setup.
   59 words, 0 differing. */
s32 func_800AA79C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        p->unk_9A = 0xE;
        return 0;
    }
    if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
    {
        func_800ACB98((u8 *)p, b, c, d);
        return 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x8) == 0)
    {
        *(u16 *)(g + 0xA) -= 1;
    }
    p->unk_9A = 5;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(u16 *)((u8 *)p + 0x98) &= ~0x8;
    *(s16 *)(d + 0x24) = 0;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) = (*(u32 *)(d + 0x1C) | 0x8) & ~0x40000;
    *(u16 *)(c + 0x14) |= 0x6000;
    return 1;
}

/* THE TWO BYTE ARGUMENTS ARE READ BEFORE THE `if`, NOT AT THE CALL. Written
   as `func_8009A3D0(c[0x24], c[0x25], t)` the loads land after the branch,
   $a2 has to be copied to a callee-live register to survive it, and the
   function is 41 words. Hoisted into two locals they are issued from $a2 in
   the prologue and nothing is saved: 39 words, 0 differing. */
void func_800AA888(u8 *p, void *b, u8 *c, u8 *d)
{
    s32 t;
    s32 x;
    s32 y;

    p[0x9A] = 0xB;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    x = c[0x24];
    y = c[0x25];
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(x, y, t);
    *(u16 *)(p + 0x98) |= 0x8;
    *(s32 *)(d + 0x1C) &= ~0x40000;
    *(s16 *)(p + 0x96) = 0;
    *(u16 *)(d + 0x6A) = *(u16 *)(*(u8 **)(d + 0x60) + 0x2A);
}

/* Installer: two refusals into state 0xE, else state 0xD with the facing
   pushed through func_80047784. 65 words, 0 differing. */
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t)
{
    u8 *g;
    s32 v;

    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if (*(u16 *)(g + 0x2) & 0x2008)
    {
        p[0x9A] = 0xE;
        return 1;
    }
    if (*(s32 *)(p + 0x1C) & 0x20)
    {
        p[0x9A] = 0xE;
        *(s32 *)(p + 0x1C) &= ~0x200;
        return 0;
    }
    p[0x9A] = 0xD;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    *(u16 *)(g + 0xA) += 1;
    *(s32 *)(p + 0x1C) &= ~0x40000;
    return 1;
}

/* func_80095DD0 on the SECOND argument and without the parked pointer.
   11 words, 0 differing. */
/* TWO parameters, not four. func_800AD9B4 is this function's only reference
   anywhere in the overlay -- no other `jal`, no address-take -- and it passes
   two. The four-parameter signature was the handler convention applied to a
   function nothing dispatches; narrowed, this still measures 11 words,
   0 differing. */
void func_800AAA28(void *a, u8 *p)
{
    u8 *g;

    p[0x9A] = 0x15;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x14) += 1;
}

/* The shared "actor dies" tail the four +0x8C state machines call with a NULL
   fourth argument. The fourth parameter keeps the `s32` the unit already
   declares even though the body indexes it: all eight landed call sites pass
   a literal 0, so the width is not witnessed. 47 words, 0 differing. */
void func_800AAA54(UnkDungeonActor *a, UnkVec *v, u8 *c, s32 d)
{
    a->unk_9A = 8;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    *(s32 *)(c + 0x2C) = d;
    if (d != 0)
    {
        func_80047784(c, ((u8 *)d)[((D_80083228 + *(s16 *)((u8 *)a + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u32 *)((u8 *)a + 0x1C) |= 0x4000000;
    func_80042B68((s8 *)a, 4);
    if ((s16)func_80042900((s8 *)a, 0xA) != 0)
    {
        *((u8 *)a + 0x13) = *((u8 *)a + 0xA8);
        func_80042518((s8 *)a, 0xA);
    }
    a->unk_96 = 0x40;
}

/* The 0x201 hand-over: refuse unless D_800E296C's 0x100000 bit is up and the
   D_80083460 record names THIS view with its counter clear, then re-dress
   through func_800C77D0, announce with func_800A56E0(0x201), and report 1
   only if the actor passes a seven-term screen. func_8008D1D0 is the same
   head, call for call. 101 words, 0 differing.

   TWO THINGS WERE MEASURED HERE, both worth a word each.

   (1) `ok` MUST BE NARROW. As `s32 ok` the `w != d` arm's `return 0` comes
   out as `addu $v0, $s1, $zero` -- cse substitutes the pseudo it knows holds
   zero for the constant -- 101 words, 1 differing. An HImode (or QImode)
   pseudo cannot stand in for an SImode `const_int 0`, so the arm keeps its
   own `addu $v0, $zero, $zero`. `s16` and `u8` are byte-identical; the return
   needs no widening either way because the value is only ever 0 or 1.

   (2) THE `w == NULL` ARM AND THE func_800A1C58 ARM ARE ONE `goto`. The
   target's `return 0` at 0x800AAC7C is a block with two predecessors -- the
   `beqz` at 0x800AAB4C and the fall-through of the tail's `beqz` -- which is
   what stops jump.c folding it into either branch. Written as two separate
   `return 0` statements the tail's collapses into its own branch and the
   function is 100 words, 17 differing.

   The D_8007359C entry is read as a HALFWORD: `*(u16 *)&...unk_00` is the
   target's `lhu`, where the declared `u32 unk_00` gives `lw`. The index is
   `[d[0x13] - 1]`, whose -1 folds into the -0x14 displacement. */
s32 func_800AAB10(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *w;
    s16 ok;

    ok = 0;
    if ((D_800E296C & 0x100000) != 0)
    {
        g = (u8 *)&D_80083460;
        w = *(u8 **)(g + 0xC);
        if (w == NULL)
        {
            goto fail;
        }
        if (w != d)
        {
            return 0;
        }
        if (*(s16 *)(g + 0xA) != 0)
        {
            return 0;
        }
        if ((*(u16 *)(g + 0x2) & 8) != 0)
        {
            return 0;
        }
        func_800C77D0(p - 0x20, b, 8, 0x300);
        func_800A1D4C(d, 1);
        func_800A56E0(0x201);
        if ((D_80013714 & 2) == 0)
        {
            D_80080AA0 = 1;
            D_80080A88 = 0;
        }
        if ((*(s32 *)(d + 0x54) & 0x800000) == 0 && (*(s32 *)(d + 0x14) & 0x4000) != 0 &&
            (*(u16 *)&D_8007359C[0][d[0x13] - 1].unk_00 & 0x80) != 0 && d[0x13] >= 2 &&
            d[0x11] >= 0x14 && (*(s32 *)(d + 0x1C) & 0x228) == 0 &&
            (s16)func_80042900((s8 *)d, 0x18) == 0)
        {
            ok = 1;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
        fail:
            return 0;
        }
        D_8008346C = 0;
    }
    return ok;
}

/* The state-0xC installer: park the handler slot, copy the tile the record at
   d's +0x60 - 0x14 is standing on into c, project it with func_8003DE58,
   place the mover on it and -- when d still has a +0x8A sweep left -- give
   the mover a +0x14 lift towards either the probed floor or d's own +0x88,
   then step c one tile along the stored facing. 151 words, 0 differing.

   THE +0x6A STORE COMES BEFORE THE +0x1C MASK. The other way round the
   `lw 0x60($s1)` that feeds the `lhu 0x2A` is issued after the mask's own
   load and its load-delay slot comes out a `nop` -- 149 words, 136
   differing, with the whole function shifted by one.
   THE DIVISION IS WRITTEN IN BOTH ARMS, not once after the join. The
   divisor is `(s16)t` where `t` is the just-stored +0x8A + 2, and the target
   sign-extends it inside EACH arm; computing the numerator per arm and
   dividing once after the join leaves one of the two `sll 16`/`sra 16` pairs
   out and is 150 words against 151. Cross-jumping puts the `div`, the `mflo`
   and the store back together. */
void func_800AACA4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s16 out[4];
    u8 *q;
    s16 h;
    s32 n;
    s32 t;

    p[0x9A] = 0xC;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(d + 0x6A) = *(u16 *)(*(u8 **)(d + 0x60) + 0x2A);
    *(u32 *)(d + 0x1C) &= 0xFFF7FFFF;
    *(s16 *)(p + 0x96) = 2;
    q = *(u8 **)(*(u8 **)(d + 0x60) - 0x14);
    if (func_8003DE58(*(u32 *)(*(u8 **)(*(u8 **)*(u8 **)*(u8 **)(q + 0x28) +
                                        (*(u8 **)(q + 0x2C))[2] * 4) +
                               4),
                      q, out, 0) == 0)
    {
        out[2] = -0x60;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(*(u8 **)(*(u8 **)(d + 0x60) - 0x18) + 0xA) + (u16)out[2];
    c[0x24] = q[0x24];
    c[0x25] = q[0x25];
    func_800A2B04(b, c[0x24], c[0x25]);
    if (*(s16 *)(d + 0x8A) != 0)
    {
        h = func_800BCB04(
            (((c[0x24] + D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] * *(s16 *)(d + 0x8A)) << 6) +
             0x20) &
                0xFFE0,
            (((c[0x25] + D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] * *(s16 *)(d + 0x8A)) << 6) +
             0x20) &
                0xFFE0,
            *(s16 *)((u8 *)b + 0xA));
        t = *(u16 *)(d + 0x8A) + 2;
        *(s16 *)(d + 0x8A) = t;
        if (h < 0x200)
        {
            b->unk_14 = -((*(s16 *)((u8 *)b + 0xA) - h) << 15) / (s16)t;
        }
        else
        {
            b->unk_14 =
                -((*(s16 *)((u8 *)b + 0xA) - *(s16 *)(*(u8 **)(d + 0x60) + 0x88)) << 15) / (s16)t;
        }
        *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) - 2;
    }
    c[0x24] += D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7];
    c[0x25] += D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7];
}

/* 176 words, 0 differing.  Parked at 4 for a day as "the proved-unreachable
   `|= 0x80000000` gate" and closed by the cure in docs/matching-levers.md,
   "THE `|= 0x80000000` GATE IS A LOCALITY PROBLEM, NOT UNREACHABLE": assign
   the constant to `cc2` ONE BLOCK EARLIER -- in the block holding the
   `& 0x400` test -- and give it a SECOND reference so a single-use REG_EQUIV
   constant is not rematerialised back into the body block.  Wrapping the
   store in `do { } while (0);` is that reference in ordinary C: flow.c:2128
   weights each reference by `loop_depth` and the loop NOTEs emit nothing.
   (`__asm__("" ::"r"(cc2));` after the store does the same thing and is how
   this first landed; the do-while is preferred because it is not a scaffold.
   Both are 176 words / 0 differing; without either, 4 rows.  Of the plain-C
   placements, `do { }` around the store and around `cc2 = 0x80000000;` are
   both 0, a duplicated-arm `if (p)` around the store is 0, and everything
   else measured runs 2 or worse.)  Multi-block, the constant leaves local_alloc,
   `block_alloc` can no longer tie the `ior`'s destination to it, and
   global_alloc hands the loaded value $v0 and the constant $v1 -- the
   target's `lw $v0` / `lui $v1` / `or $v0, $v0, $v1`.  Nothing else in the
   parked body changed; the four rows went to 0 on the first compile.

   The other four levers, all still load-bearing (see the park record in
   git history, docs/dungeon-parks/func_800AAF00.c before this landing):
   `*(volatile u16 *)` on the FIRST read of (p + 0x46) -- the 0x4000 test,
   not the re-read, because that is where the target has an unfilled delay
   slot; the table as a pointer local `tbl = (UnkDungeonName10 *)D_8006DE24`
   with `e = &tbl[i]`, which splits `lui %hi` / `addiu %lo` around the
   stride-0x14 multiply; `i` REUSED for the table index it loads, so a
   second set takes it out of local-alloc and global_alloc hands it $a0
   (worth ten of the twelve table rows); and `gp = p + i * 3` as its own
   pointer local, which is the only one of seven spellings that emits
   `addu $v0, $s0, $v0` in the source operand order.  The first gate is
   `(*(s16 *)(p + 0x46) & 0x3FFF) >= 5` through s16, not u16: through u16
   combine proves the masked value non-negative and the compare becomes
   `sltiu` where the target has `slti`. */
void func_800AAF00(UnkDungeonActor *a, UnkVec *b, u8 *c, s32 t, void (*h)())
{
    u8 *p;
    u8 *g;
    u8 *g2;
    u8 *q;
    u8 *gp;
    UnkDungeonName10 *e;
    UnkDungeonName10 *tbl;
    u16 f;
    s32 k;
    s32 i;
    s32 cc2;

    p = (u8 *)a;
    k = 0;
    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    f = *(u16 *)(g + 0x2);
    if ((f & 0x2008) != 0)
    {
        return;
    }
    if ((*(u32 *)(p + 0x1C) & 0x2000) != 0 && (*(s16 *)(p + 0x46) & 0x3FFF) >= 5)
    {
        q = D_800E3D7C;
        k = 1;
        if ((*(u32 *)(q + 0x98) & 0x3000) == 0x2000)
        {
            if (*(u16 *)(q + 0xA6) == 0)
            {
                *(u16 *)(q + 0x98) |= 0x1000;
                *(u16 *)(q + 0xA6) += 1;
                *(u16 *)(g + 0xA) += 1;
                goto join;
            }
        }
        func_800A4ACC(p);
        p[0x6D] -= 1;
        a->unk_8C = h;
        return;
    }
    if ((f & 8) != 0)
    {
        return;
    }
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
    func_800C77D0(p - 0x20, b, 8, 0x300);
    if ((s16)func_800A2B5C(p) != 0)
    {
        return;
    }
join:
    if (k == 0)
    {
        if ((*(u32 *)(p + 0x1C) & 0x400) != 0)
        {
            cc2 = 0x80000000;
            if (*(s32 *)(p + 0x14) >= 0)
            {
                do
                {
                    *(u32 *)(p + 0x14) |= cc2;
                } while (0);
                *(u16 *)(p + 0x2A) += (func_800A6D30() & 7) << 9;
            }
        }
        if ((*(volatile u16 *)(p + 0x46) & 0x4000) == 0)
        {
            i = (*(u16 *)(p + 0x46) & 0x3FFF) - 1;
            gp = p + i * 3;
            i = gp[8];
            tbl = (UnkDungeonName10 *)D_8006DE24;
            e = &tbl[i];
            if (e->unk_12 == 2 && *(s32 *)(p + 0x60) == 0)
            {
                *(s32 *)(p + 0x60) =
                    func_800A05A4(p, c[0x24], c[0x25], *(s16 *)(p + 0x2A), e->unk_13);
            }
        }
    }
    else
    {
        *(u8 **)(D_800E3D7C + 0x11C) = p;
    }
    a->unk_9A = 0x12;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    if (t != 0)
    {
        *(s32 *)(c + 0x2C) = t;
        func_80047784(c, ((u8 *)t)[((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u16 *)(c + 0x14) |= 0x800;
    g2 = (u8 *)&D_80083460;
    *(u16 *)(g2 + 0xA) += 1;
}

/* Ease the mover onto the tile at c[0x24]/c[0x25] over the +0x96 countdown,
   and on the last frame re-place it, bleed the engine's +0x08 counter, clear
   d's +0x46 bit and re-probe the floor height. Returns 0 while the countdown
   is still running and 1 once it has expired. 110 words, 0 differing.

   EACH AXIS' `- 0x20` NEEDS ITS OWN LOCAL, ASSIGNED INSIDE THE EXPRESSION.
   Written `((c[0x24] << 6) - (*(s16 *)((u8 *)b + 0x2) - 0x20)) << 16` fold's
   `associate:` moves the constant to the other operand -- `addiu $v0, 0x20`
   on the shifted tile where the target has `addiu $v1, -0x20` on the loaded
   position -- 111 words / 81 differing. Hoisting the subtraction into a
   statement of its own fixes the arithmetic but leaves the two chains in the
   wrong order (110 / 4), and ONE local shared by both axes keeps that 4;
   two distinct locals assigned INSIDE the subtraction, `(... - (x = ...))`,
   is 0. */
s32 func_800AB1C0(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s16 h;
    s32 x;
    s32 y;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        *(s16 *)(a + 0x96) = 0;
    }
    if (*(s16 *)(a + 0x96) != 0)
    {
        b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(a + 0x96);
        b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(a + 0x96);
    }
    *(s16 *)(a + 0x96) = *(u16 *)(a + 0x96) - 1;
    if (*(s16 *)(a + 0x96) > 0)
    {
        return 0;
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    func_800A2B04(b, c[0x24], c[0x25]);
    if (*(s16 *)(g + 0x8) != 0)
    {
        *(s16 *)(g + 0x8) = *(u16 *)(g + 0x8) - 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x2000) != 0)
    {
        *(u16 *)(d + 0x46) &= 0x7FFF;
    }
    if ((*(u16 *)(g + 0x2) & 0x80) != 0)
    {
        *(u32 *)(d + 0x1C) &= 0xBFFFFFFF;
        h = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, *(u16 *)(d + 0x88) - 0x20);
        *(s16 *)(a + 0x92) = *(u16 *)(a + 0x92) + (*(u16 *)(d + 0x88) - h);
        *(s16 *)(d + 0x88) = h;
        if ((*(u32 *)(d + 0x1C) & 0x40000) == 0)
        {
            *(s32 *)(a + 0x90) = 0;
        }
    }
    return 1;
}

/* WRITTEN K&R ON PURPOSE. The body reads all four argument registers, but the
   three landed installers that call it (func_800B2C68, func_800D45D0,
   func_800D9C18) are PURE FORWARDS written with no arguments at all, so a
   prototyped four-parameter definition is `too few arguments to function`
   three times over. clang-format wants the first parameter declaration at
   COLUMN ZERO here -- an `s32` return with a pointer first parameter -- not
   indented as the K&R note in docs/town-sweep.md says. 59 words,
   0 differing. */
s32 func_800AB378(p, b, c, d)
u8 *p;
UnkVec *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *q;
    s16 t;

    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(s16 *)(p + 0x96) = 0;
    }
    if (*(s16 *)(p + 0x96) != 0)
    {
        q = *(u8 **)(*(u8 **)(d + 0x60) - 0x18);
        b->unk_0C = *(s32 *)(q + 0xC);
        b->unk_10 = *(s32 *)(q + 0x10);
    }
    t = *(u16 *)(p + 0x96) - 1;
    *(s16 *)(p + 0x96) = t;
    if (t <= 0)
    {
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        return 1;
    }
    return 0;
}

/* The `blez` installer gate. MUST STAY K&R, like func_800AC7EC above:
   func_800D491C and func_800DA7D0 call this with NO arguments.

   THE RESULT LOCAL IS `s32` WITH AN EXPLICIT `(s16)` AT BOTH USES. As `s16 r`
   the test's `sll` takes the RAW call result instead of the copy in $v1 -- 53
   words, 3 differing -- and `return r` without the cast is 52 / 14. The big
   arm must also be written FIRST: with the `r = 1` arm first the `bnez`
   inverts and the layout costs 32 differing.

   func_800ADAE4 is deliberately NOT declared at the top of this unit: it is
   landed below as s16, and with that prototype in scope gcc drops both casts
   and the function is 52 words / 16 differing. Widening the landed definition
   to s32 breaks func_800ADAE4 itself (42 words / 12). The implicit `int`
   declaration this call site rides is the only resolution, and it costs three
   warnings of the class this unit already carries. 53 words, 0 differing. */
s32 func_800AB464(a, b, c, p)
UnkDungeonActor *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 r;
    u8 *g;

    if (a->unk_9B == 0)
    {
        p[0x71] &= 0x7F;
        g = (u8 *)&D_80083460;
        if (*(u16 *)(g + 0x2) & 0x2000)
        {
            return 0;
        }
        if ((s16)func_800A2B5C(p) != 0)
        {
            return 0;
        }
        r = func_800ADAE4(c, p);
        if ((s16)r != 0)
        {
            *(s32 *)(g + 0xC) = 0;
            *(u16 *)(g + 0x14) -= 1;
            a->unk_9B = 1;
        }
    }
    else
    {
        r = 1;
    }
    return (s16)r;
}

/* The three-state cutscene pan: state 0 arms the +0x96 countdown and either
   jumps straight to state 2 or seeds the view's +0x8A step, state 1 runs the
   per-frame glide towards D_80083780 and hands the leash back at the end,
   state 2 stops the pan and raises the view's 0x800000.

   WRITTEN K&R, AND THE DECLARATION IS WIDENED TO `s32`. Every exit sets
   `$v0` to zero, which is a `return 0` and not a `void` tail, so the
   declaration at the top of the file changed from `void func_800AB538();` to
   `s32 func_800AB538();` -- both landed callers (func_800B306C :12519 and
   func_800D9F9C :24048) discard it and both still measure 8 words,
   0 differing. They sit BELOW this definition and pass no arguments, so an
   ANSI four-parameter definition is `too few arguments` twice over; K&R is
   the only form that compiles.

   THE MOVER IS `u8 *`, NOT `UnkVec *`, and that is worth two words. A
   COMPONENT_REF store is MEM_IN_STRUCT_P with a varying address, which
   `true_dependence` (sched.c:854) excuses against a non-in-struct load at a
   non-varying address -- so with `b->unk_0C = 0` the two D_80083780 reads and
   the D_8008378A read are free to float into the stores' load-delay slots and
   two `nop`s the target keeps disappear. Through `*(s32 *)(b + 0xC)` casts
   the dependence is recorded and the order is the target's: 144 words against
   142 with 76 differing.

   Case 2's two `sh $zero` stores are in one basic block with nothing to
   split it, so their order is a TIE in the bytes; they are written +0x1E
   then +0x1C to match. 144 words, 0 differing. */
s32 func_800AB538(p, b, c, d)
u8 *p;
u8 *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    s16 t;

    switch (p[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x96) = 0x10;
        *(s32 *)(d + 0x1C) &= 0xFFFBFFFF;
        *(u16 *)(p + 0x98) |= 0x8;
        if ((D_80013714 & 2) != 0)
        {
            p[0x9B] = 2;
        }
        else
        {
            *(s16 *)(d + 0x8A) = 0x800 / *(s16 *)(p + 0x96);
            p[0x9B]++;
        }
        break;
    case 1:
        if (*(s16 *)(p + 0x96) - 2 > 0)
        {
            g = D_80083780;
            *(s32 *)(b + 0xC) = ((*(s16 *)(g + 0x2) - *(s16 *)(b + 0x2)) << 16) / (*(s16 *)(p + 0x96) - 2);
            *(s32 *)(b + 0x10) = ((*(s16 *)(g + 0x6) - *(s16 *)(b + 0x6)) << 16) / (*(s16 *)(p + 0x96) - 2);
        }
        else
        {
            g2 = D_80083780;
            *(s32 *)(b + 0x10) = 0;
            *(s32 *)(b + 0xC) = 0;
            *(s16 *)(b + 0x2) = *(u16 *)(g2 + 0x2);
            *(s16 *)(b + 0x6) = *(u16 *)(g2 + 0x6);
        }
        if (*(s16 *)(p + 0x96) != 0)
        {
            *(s32 *)(b + 0x14) = ((D_8008378A - (func_800644B8(*(s16 *)(p + 0x96) << 6) >> 4) - *(s16 *)(b + 0xA)) << 16) / *(s16 *)(p + 0x96);
        }
        t = *(u16 *)(c + 0x1C) - *(u16 *)(d + 0x8A);
        *(s16 *)(c + 0x1C) = t;
        *(s16 *)(c + 0x1E) = t;
        t = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = t;
        if (t < 0)
        {
            *(s32 *)(b + 0x14) = 0;
            *(s16 *)(d + 0x88) = D_8008378A;
            *(s16 *)(b + 0xA) = D_8008378A;
            *(s16 *)(p + 0x92) = 0;
            p[0x9B]++;
        }
        break;
    case 2:
        *(s16 *)(c + 0x1E) = 0;
        *(s16 *)(c + 0x1C) = 0;
        *(s32 *)(d + 0x1C) |= 0x800000;
        break;
    }
    return 0;
}

/* The engine's four-state teardown for a retiring actor, switched on the
   actor's +0x9B: state 0 bumps D_80083460's +0x0A census and advances to 3
   (clearing the resident's +0x110 slot when the record's +0x43 is 0xFD and
   otherwise refusing if func_800A2C78 says the floor is busy); state 3
   either hands a sub-0x40 record to func_800C542C and jumps straight to
   0x10, or arms the +0x96 fade and falls into state 4; state 4 runs the
   five-way fade down -- three colour bytes and two scales, each divided by
   what is left of the counter -- and on the last frame announces the record,
   releases its D_800E3DF0 slot and its D_800E3E48 shadow and tears it down;
   state 0x10 re-raises the 0x4000 flag, announces D_800E0C34 and runs the
   same teardown without the census arithmetic. 401 words, 0 differing.

   ONE `u8 *` LOCAL PER `&D_80083460` MATERIALISATION -- g, g2, g3, g4, four
   sites in three cases. With ONE local reassigned the `%hi` lands in a
   different register from its `%lo` at three of the four sites and the +0x0A
   read-modify-writes lose their scheduling: 402 words, 127 differing, where
   four locals plus the in-place `f` mask below is 401 / 8. This is
   docs/matching-levers.md's "THE NUMBER OF SOURCE LOCALS HOLDING ONE
   GLOBAL'S ADDRESS DECIDES WHETHER ITS %hi AND %lo SHARE A REGISTER", and
   it is the single biggest lever in the function.

   TWO `Unk800E3E48 *` LOCALS, ONE PER SITE, AND THEY ARE WHAT KEEPS +0x13 A
   DISPLACEMENT. Written `((u8 *)&D_800E3E48[j])[0x13] = 0` the address is
   `plus(plus(mult, symbol), 19)` and plus_constant folds the 19 into the
   symbol, giving `%lo(D_800E3E48)+0x13` and `sb $zero, 0x0($v0)` where the
   target has `addiu %lo(D_800E3E48)` and `sb $zero, 0x13($v0)` -- 2
   differing. A pointer LOCAL makes the base a register, so the 19 has
   nowhere to fold: `sh = D_800E3E48; *((u8 *)&sh[j] + 0x13) = 0;`. One local
   SHARED between the two sites is 403 words / 176 differing; two locals are
   401 / 4. `*(u8 *)((s32)&D_800E3E48[j] + 0x13)` folds the same way as the
   array form and buys nothing.

   THE SLOT INDEX IS TWO NARROW LOCALS: `s16 i` for D_800E3DF0 and `s16 j`
   for D_800E3E48, per docs/matching-levers.md's "THE SCRATCHPAD INDEX TYPE
   CAN BE WORTH 45 DIFFERING WORDS BY ITSELF", which was measured on this
   very function: `s32`/`u32` is 403 words / 53 differing, one narrow index
   is 401 / 18, two are 401 / 8.

   `k = f & 0x4000;` IS A SEPARATE LOCAL *BEFORE* THE CALL, AND THAT IS THE
   `jal func_80099844` DELAY SLOT. Written `if ((f & 0x4000) == 0)` after the
   call the mask gets its own pseudo in $v0 and dbr fills the slot with
   `addiu $a1, $a1, %lo(D_800E0C34)` instead -- 4 differing. `f &= 0x4000;`
   in place before the call ties the result to $s0 but sched1 then hoists the
   `andi` ahead of both argument setups -- 6 differing. Only a separate local
   leaves the `andi` adjacent to the `jal`, which is where dbr takes it from.
   401 / 2 -> this is worth 2 words.

   STATE 0'S PROBE EXIT IS A `break`, AND THE `!= 0xFD` ARM ORDER GOES WITH
   IT. As `return 0` the exit compiles to an inline `move $v0,0` + `j`, which
   is two insns, so jump.c's conditional-jump-around-an-unconditional-jump
   rule cannot fire and the branch comes out `beqz` into the join with a
   `nop` in the following `j`'s delay slot -- 2 differing. As `break` the
   exit is a bare `j` to the switch's shared `return 0`, jump.c inverts it to
   `bnez` and reorg steals the zero into its delay slot and redirects to the
   epilogue, which is the target. But `break` ALONE also lets gcc reorder the
   two arms of the +0x43 test and merge both `j`s: 399 words, 349 differing.
   Writing the test `if (d[0x43] != 0xFD) { probe } else { clear }` puts the
   arms back in the target's order. Either change alone is no better than the
   base; together they are the last 2 words.

   `k` carries three disjoint values -- the `f` mask, the +0x43 record index
   and func_800A1BD0's slot index -- and that is what the register assignment
   wants. 0x80010A80 is a CONSTANT ADDRESS (`lui`/`ori`, not `lui`/`addiu`);
   `ovlverify --raw` has both words byte-identical to the target, and the
   other 90 raw differences are all `jal`/`j`/`%hi`/`%lo` relocation fields. */
s32 func_800AB778(p, b, c, d)
u8 *p;
void *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *g4;
    u8 *e;
    u8 *base;
    Unk800E3E48 *sh;
    Unk800E3E48 *sh2;
    void *h;
    void *m;
    s32 f;
    s16 k;
    s16 n;
    s16 i;
    s16 j;

    f = *(s32 *)(d + 0x14);
    if ((f & 0x20000000) != 0)
    {
        *(s32 *)(d + 0x14) = f | 0x400000;
        func_800ACB98(p, b, c, d);
        return 0;
    }
    switch (p[0x9B])
    {
    case 0:
        if (d[0x43] != 0xFD)
        {
            if ((s16)func_800A2C78(d) != 0)
            {
                break;
            }
        }
        else
        {
            *(s32 *)(D_800E3D7C + 0x110) = 0;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        p[0x9B] = 3;
        break;
    case 3:
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_80094E34();
        }
        if (d[0x43] < 0x40)
        {
            func_800C542C(d, D_800DCED4[func_800429E4(d)], d == *(u8 **)(D_800E3D7C + 0xAC), 2);
            *(s16 *)(p + 0x96) = 0;
            p[0x9B] = 0x10;
            break;
        }
        if (*(u16 *)(c + 0x12) == 0)
        {
            *(u16 *)(c + 0x12) = 0xFF80;
        }
        *(s16 *)(c + 0x10) = 0x20;
        *(u16 *)(c + 0x14) |= 0xC;
        *(s16 *)(p + 0x96) = 0x10;
        *(s32 *)(d + 0x1C) |= 0x10000000;
        p[0x9B]++;
        break;
    case 4:
        c[0xC] -= c[0xC] / *(s16 *)(p + 0x96);
        c[0xD] -= c[0xD] / *(s16 *)(p + 0x96);
        c[0xE] -= c[0xE] / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1C) += (0x400 - *(u16 *)(c + 0x1C)) / *(s16 *)(p + 0x96);
        *(u16 *)(c + 0x1E) += (0x400 - *(u16 *)(c + 0x1E)) / *(s16 *)(p + 0x96);
        n = *(u16 *)(p + 0x96) - 1;
        *(s16 *)(p + 0x96) = n;
        if (n > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return 0;
        }
        h = func_800990FC();
        m = func_8009929C(8, h);
        if (d[0x43] == 0xFF)
        {
            m = func_80099194(D_800E0C58, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else if (d[0x43] == 0xFE)
        {
            m = func_80099194(D_800E0C67, func_80099734(d, m));
            func_80099290(m);
            func_800A5720(h);
        }
        else
        {
            D_800DCE68 -= 1;
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
        }
        func_800A31D0(d);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        if (*(s32 *)(D_800E3D7C + 0x110) != 0)
        {
            func_800A18E8(d[0x13], 3);
            e = *(u8 **)(D_800E3D7C + 0x110);
            i = e[3] & 0x1F;
            j = i;
            ((u8 *)D_800E3DF0[i])[0x13] = 0;
            sh = D_800E3E48;
            *((u8 *)&sh[j] + 0x13) = 0;
            D_800E3DF0[i] = 0;
            func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        }
        else
        {
            func_800A18E8(d[0x13], 2);
        }
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) -= 1;
        if (*(s32 *)(g3 + 0xC) == (s32)d)
        {
            *(s32 *)(g3 + 0xC) = 0;
        }
        break;
    case 0x10:
        *(s32 *)(d + 0x14) = f | 0x4000;
        k = f & 0x4000;
        func_80099844(d, D_800E0C34);
        if (k == 0)
        {
            *(s32 *)(d + 0x14) &= ~0x4000;
        }
        func_800A31D0(d);
        k = d[0x43];
        d[0x44] = 0;
        if ((s16)func_80042900((s8 *)d, 0xA) != 0)
        {
            d[0x13] = d[0xA8];
        }
        func_800422DC((u8 *)(k * 0x54 + 0x80010A80), (s32)d);
        func_800A18E8(d[0x13], 3);
        k = (s16)func_800A1BD0(d);
        if (k >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(k * 4 + base + 0xAC) = 0;
            *(s32 *)(k * 4 + base + 0xD0) = 0;
        }
        e = *(u8 **)(D_800E3D7C + 0x110);
        i = e[3] & 0x1F;
        j = i;
        ((u8 *)D_800E3DF0[i])[0x13] = 0;
        sh2 = D_800E3E48;
        *((u8 *)&sh2[j] + 0x13) = 0;
        D_800E3DF0[i] = 0;
        func_80098B38(*(u8 **)(D_800E3D7C + 0x110));
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g4 = (u8 *)&D_80083460;
        *(u16 *)(g4 + 0xA) -= 1;
        break;
    }
    return 0;
}

/* The three-state climb/descend glide: case 0 seeds the 0x10-frame countdown
   and either re-places the mover and jumps to state 2 or parks the per-frame
   lift in d's +0x8A, case 1 eases the mover onto the tile over the remaining
   frames and, on the frame the countdown goes negative, stops it dead and
   advances the state, and case 2 reports done. 149 words, 0 differing.

   MUST STAY K&R: the landed func_800B30AC and func_800D9FDC call this with
   NO arguments, so a prototyped four-parameter definition is "too few
   arguments to function" twice over -- the same rule
   func_800AB378 and func_800AC7EC above record.

   EACH `- 0x20` AND THE `+ 0x20` NEEDS ITS OWN LOCAL ASSIGNED INSIDE THE
   EXPRESSION, the func_800AB1C0 lever one function up. Written plainly,
   fold's `associate:` moves the constant onto the other operand -- `addiu
   $v0, 0x20` on the shifted tile byte where the target has `addiu $v1,
   -0x20` on the loaded position, and `addiu $v0, -0x20` on the negated
   lift where the target has `addiu $v1, 0x20` on the +0x92 read.
   AND CASE 1'S FIRST TEST IS WRITTEN THE WAY THE TARGET BRANCHES: as
   `if (unk_96 + 2 >= 0x10) { zero the two deltas } else if ...` the `slti`'s
   `bnez` goes to the else and the two `sw $zero` are inline; the natural
   `< 0x10` spelling inverts the whole arm.
   THE QUOTIENT OF case 0's ELSE ARM IS A LOCAL. As
   `*(s16 *)(d + 0x8A) = 0x800 / *(s16 *)(a + 0x96);` the `mflo` lands in $v0,
   the store cannot be deferred past the +0x9B read-modify-write and the arm
   is one word long with the operands of the `div` swapped (150 words,
   104 differing with the other two levers already applied). */
s32 func_800ABDBC(a, b, c, d)
u8 *a;
UnkVec *b;
u8 *c;
u8 *d;
{
    s16 q;
    s32 x;
    s32 y;
    s32 z;

    switch (a[0x9B])
    {
    case 0:
        *(s16 *)(a + 0x96) = 0x10;
        *(u32 *)(d + 0x1C) &= 0xFFFBFFFF;
        if (D_80013714 & 2)
        {
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(c + 0x1C) = 0x1000;
            *(s16 *)(c + 0x1E) = 0x1000;
            a[0x9B] = 2;
        }
        else
        {
            q = 0x800 / *(s16 *)(a + 0x96);
            a[0x9B] += 1;
            *(s16 *)(d + 0x8A) = q;
        }
        break;
    case 1:
        if (*(s16 *)(a + 0x96) + 2 >= 0x10)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
        }
        else if (*(s16 *)(a + 0x96) - 2 > 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) /
                        (*(s16 *)(a + 0x96) - 2);
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) /
                        (*(s16 *)(a + 0x96) - 2);
        }
        else
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
        }
        if (*(s16 *)(a + 0x96) != 0)
        {
            b->unk_14 = ((-(func_800644B8(*(s16 *)(a + 0x96) << 6) >> 4) -
                          (z = *(s16 *)(a + 0x92) + 0x20))
                         << 16) /
                        *(s16 *)(a + 0x96);
        }
        *(s16 *)(c + 0x1E) = *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1C) + *(u16 *)(d + 0x8A);
        if ((*(s16 *)(a + 0x96) -= 1) < 0)
        {
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800A2B04(b, c[0x24], c[0x25]);
            *(s16 *)(a + 0x92) = -0x20;
            a[0x9B] += 1;
        }
        break;
    case 2:
        return 1;
    }
    return 0;
}

/* func_800AC010 -- 284 words, 0 differing.  State 4's per-frame step for the
   four-argument installer family.

   THE LAST FOUR WORDS WERE A sched1 PRIORITY BOOST, AND THE CURE IS FOUR
   SINGLE-SET LOCALS.  The residue was one transposition of two independent
   two-insn chains at 0x800AC04C -- target `lbu`/`lh`/`sll`/`addiu`, candidate
   `lh`/`lbu`/`addiu`/`sll` -- and it survived sixteen spellings of the
   subtraction.  Cause, read out of `tools/decomp/sched.py --sched1`:
   `adjust_priority` (sched.c:2551) hands LAUNCH_PRIORITY (0x7f000001) to any
   insn that becomes ready while `birthing_insn_p` holds, i.e. that SETs a
   live pseudo with `reg_n_sets == 1`.  With ONE `s32 x` shared by the two
   subtractions, `x` has two sets, the `addiu` is not birthing and not
   boosted, the `sll` into its compiler temp IS, and the boosted insn wins the
   cycle in which both become ready -- which in a backward scheduler means it
   is EMITTED LAST, putting the `lh` chain first.  Give every operand of both
   divisions its own write-once local (`u1`/`x1`, `u2`/`x2`) and both chain
   tails are birthing, the tie falls through to INSN_LUID, and the chain
   written SECOND is emitted first, which is the target's order.
   The subtrahend still MUST go through a local: written inline, `fold`'s
   associate path rewrites `A - (B - 0x20)` into `(A + 0x20) - B` and the
   `addiu` comes out `+32` on the shifted byte.  `s32 x; s32 y; s32 u;` with
   `u` shared is 285 words / 12 -- a shared local goes to global_alloc
   (local-alloc.c:472 wants `reg_n_deaths == 1`) and costs the extra word.

   THE THREE DEVICES THAT GOT IT TO 284 WORDS BEFORE THAT, each measured:
   1. `goto fail;` IN THE SECOND func_8009B88C ARM.  Both arms end in the same
      `func_800A2B04` / `func_800AA5E4` / `return 0` tail.  Written out twice,
      jump.c's forward scan always deletes the FIRST copy, so the shared tail
      lands at the END of the function where the target has it at 0x800AC1C0.
      Both arms `== 0`: 282 / 38.  Arm 1 `== 0`, arm 2 `!= 0`: 282 / 33.
      Label in arm 1 and `goto fail` in arm 2: 282 / 12.
   2. A TRAILING `out: return 0;`.  The `*(s16 *)(a + 0x96) = 2` arm's
      `return 0` must cross-jump onto a one-insn `v0 = 0` block that falls
      into the epilogue; that is what makes the final `return 1` need its own
      `j .L800AC458`.  Worth the last two words: 282 -> 284.
   3. `s32 func_8009B164s(s16, UnkVec *, u8 *) __asm__("func_8009B164");`
      The landed definition takes `u16` (its own `(a >> 9) & 7` needs the
      logical shift), and through it the argument load is `lhu` where the
      target has `lh`.

   MUST STAY K&R: `s32 func_800AC010();` at the top of the unit and two landed
   call sites pass NO arguments, so an ANSI four-parameter definition makes
   both "too few arguments" -- the same constraint func_800AC82C records. */

s32 func_800AC010(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *d;
    u8 *g;
    u8 *h;
    s32 u1;
    s32 x1;
    s32 u2;
    s32 x2;
    s16 o0;
    s16 o1;
    s16 t;

    if (*(s16 *)(a + 0x96) != 0)
    {
        u1 = c[0x24] << 6;
        x1 = *(s16 *)((u8 *)b + 0x2) - 0x20;
        b->unk_0C = ((u1 - x1) << 16) / *(s16 *)(a + 0x96);
        u2 = c[0x25] << 6;
        x2 = *(s16 *)((u8 *)b + 0x6) - 0x20;
        b->unk_10 = ((u2 - x2) << 16) / *(s16 *)(a + 0x96);
    }
    *(s16 *)(a + 0x96) = *(u16 *)(a + 0x96) - 1;
    if (*(s16 *)(a + 0x96) > 0)
    {
        return 0;
    }
    d = (u8 *)func_8009C93C(p, c, *(s16 *)(p + 0x6A), 0, 0);
    if (d != NULL)
    {
        func_8009CE1C(p, 4, 1, 8, (s16)(*(u16 *)(p + 0x6A) + 0x800), 0, 1);
        func_8009CE1C(d, 8, 1, 8, *(s16 *)(p + 0x6A), 0, 1);
        *(u16 *)(a + 0x98) &= 0xFFF7;
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        *(s32 *)(p + 0x1C) |= 0x40000000;
        a[0x9C] = c[0x26];
        c[0x26] = func_8009FB34(c[0x24], c[0x25]);
        func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
        if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
        {
        fail:
            func_800A2B04(b, c[0x24], c[0x25]);
            func_800AA5E4(a, b, c, p);
            return 0;
        }
        c[0x24] = o0;
        c[0x25] = o1;
        func_800A2B04(b, c[0x24], c[0x25]);
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        *(s32 *)(p + 0x1C) &= 0xFFDFFFFF;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(s16 *)(a + 0x96) = 0;
        p[0x71] = 0;
        return 1;
    }
    if ((s16)func_8009B164s(*(s16 *)(p + 0x6A), b, c) != 0)
    {
        *(s16 *)(p + 0x8A) = *(u16 *)(p + 0x8A) - 1;
        if (*(s16 *)(p + 0x8A) > 0)
        {
            c[0x24] += *(u8 *)&D_8006CCD8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            c[0x25] += *(u8 *)&D_8006CCE8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            *(s16 *)(a + 0x96) = 2;
            goto out;
        }
    }
    b->unk_14 = 0;
    b->unk_10 = 0;
    b->unk_0C = 0;
    *(u16 *)(a + 0x98) &= 0xFFF7;
    a[0x9C] = c[0x26];
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    func_800A19E4((s32)c, p, 3, 6, a + 0x9C);
    t = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                      (s16)(*(u16 *)((u8 *)b + 0xA) - 0x20));
    if ((s16)t < 0x200)
    {
        *(s16 *)(a + 0x92) = 0;
        *(s16 *)(p + 0x88) = t;
    }
    h = (u8 *)&D_80083460;
    *(u16 *)(h + 0xA) -= 1;
    *(s16 *)(a + 0x96) = 0;
    *(s32 *)(p + 0x1C) = (*(s32 *)(p + 0x1C) | 0x40000000) & 0xFFDFFFFF;
    if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) == 0)
    {
        goto fail;
    }
    c[0x24] = o0;
    c[0x25] = o1;
    func_800A2B04(b, c[0x24], c[0x25]);
    func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0x71] = 0;
    return 1;
out:
    return 0;
}

/* Glide the mover towards the tile record's centre over the +0x96 countdown,
   then -- on the frame it reaches zero -- stop it dead, re-place it on the
   tile, and hand the leash at +0x88 back to the view. 82 words, 0 differing.

   THE 0x20 IN THE FIRST NUMERATOR IS A VARIABLE, and that is the whole
   function. Both halves want `(byte << 6) - (halfword - 0x20)`, which fold
   reassociates into `(byte << 6) - halfword + 0x20` unless the subtrahend is
   its own statement -- and with the statement the two equal-depth chains of
   the FIRST numerator schedule the other way round: `lh 0x2($a0)` before
   `lbu 0x24($s0)`, four words differing, with every other word in the
   function exact. rank_for_schedule has nothing left to separate them but
   INSN_LUID, and no source order reaches it: thirty-three spellings were
   measured, including the byte hoisted into a local of either type, two
   numerator locals, either store as a cast, `* 0x10000` for `<< 16`, and an
   empty `__asm__ volatile("")` between the chains (84 words) or before them
   (no change). Holding the 0x20 in a local instead blocks the reassociation
   without a statement boundary, and the chains come out in the target's
   order. The second numerator keeps the ordinary spelling; it was never
   wrong. decomp-permuter found this in two minutes after the hand sweep had
   run out. */
s32 func_800AC480(u8 *p, UnkVec *v, u8 *r, u8 *b)
{
    s16 n;
    s16 d;
    s32 w;
    u8 *g;

    n = *(s16 *)(p + 0x96);
    if (n != 0)
    {
        w = 0x20;
        v->unk_0C = (((r[0x24] << 6) - (*(s16 *)((u8 *)v + 0x2) - w)) << 16) / n;
        w = *(s16 *)((u8 *)v + 0x6) - 0x20;
        v->unk_10 = (((r[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
    }
    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return 0;
    }
    v->unk_14 = 0;
    v->unk_10 = 0;
    v->unk_0C = 0;
    func_800A2B04(v, r[0x24], r[0x25]);
    d = func_800BCB04((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(u16 *)(b + 0x88) - 0x20);
    if (d < 0x200)
    {
        *(s16 *)(p + 0x92) += *(u16 *)(b + 0x88) - d;
        *(s16 *)(b + 0x88) = d;
    }
    g = (u8 *)&D_80083460;
    *(s16 *)(g + 0xA) -= 1;
    return 1;
}

/* The three-state landing glide: state 0 stamps the facing into +0x6A, state
   1 pans +0x2A, bleeds the mover's +0x14 and on the last frame either tears
   the actor down or re-measures its leash, state 2 pans and closes with
   func_8009A21C and the +0x6A restore. 137 words, 0 differing.

   MUST STAY K&R: the landed func_800B321C and func_800DA760 both call this
   with NO arguments, and an ANSI four-parameter definition makes those calls
   "too few arguments to function" -- the same resolution func_800AB464 and
   func_800AC7EC next door already carry.

   THE TWO D_80083460 BASES ARE TWO SEPARATE LOCALS. One shared `u8 *g` is a
   single global-alloc quantity, so both sites get the same hard register and
   the `high` keeps its own -- 137 words, 6 differing, every differing word a
   `lui`/`addiu` register. Two locals are local-alloc quantities, the `high`
   is combined into each one's own register ($v1 and $v0) and it is 0.
   The spelling of the local is inert: `s16 *`, `u16 *` and `*(s16 *)(g+0xA)`
   all give the identical program. */
s32 func_800AC5C8(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *g;
    u8 *h;

    switch (a[0x9B])
    {
    case 0:
        *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x2A) & 0xFFF;
        a[0x9B] += 1;
    case 1:
        *(s16 *)(p + 0x2A) += 0x200;
        b->unk_14 += 0xFFFF0000;
        if ((*(s16 *)(a + 0x96) -= 1) > 0)
        {
            return 0;
        }
        b->unk_14 = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(s16 *)(a + 0x96) = 0x10;
        if ((*(s32 *)(p + 0x14) & 0x4000) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            func_800A32A4(p);
            func_8009A028(p);
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        else
        {
            *(s16 *)(p + 0x88) = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20, -0x400);
            *(u16 *)(a + 0x98) &= 0xFFF7;
            *(u32 *)(p + 0x1C) &= 0xFFFEFFFF;
            *(s16 *)(a + 0x96) = 0x10;
            a[0x9B] += 1;
        }
        break;
    case 2:
        *(s16 *)(p + 0x2A) += 0x200;
        if ((*(s16 *)(a + 0x96) -= 1) > 0)
        {
            return 0;
        }
        *(s32 *)(a + 0x90) = 0;
        func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0xA) -= 1;
        *(s16 *)(p + 0x2A) = *(u16 *)(p + 0x6A);
        return 1;
    }
    return 0;
}

/* MUST STAY K&R: the landed func_800DA808 below calls this with NO
   arguments, and an ANSI four-parameter definition makes that call "too few
   arguments to function". 16 words, 0 differing. */
s32 func_800AC7EC(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 f;

    f = *(s32 *)(p + 0x1C);
    if (f & 0x8000000)
    {
        if (*(s32 *)(p + 0x14) & 0x40000)
        {
            *(s32 *)(p + 0x1C) = f | 0x40000;
        }
        return 1;
    }
    return 0;
}

/* The 0x9B state pair that re-seats an actor on its carrier: state 0 waits
   for func_8004CAE8 on the carrier's source record, state 1 copies the
   carrier's tile pair into the renderable, then either walks the eight-way
   D_8006CCD8/E8 offset onto it and asks func_8009B88C for a free cell, or --
   when the view carries 0x80000 -- places the mover from func_8003DE58's
   projection and pans +0x2A by the facing delta.

   MUST STAY K&R: the declaration at the top of the file is
   `s32 func_800AC82C();` and two landed call sites pass NO arguments, so an
   ANSI four-parameter definition makes both "too few arguments".

   THREE SPELLINGS IN THE 0x80000 ARM ARE EACH WORTH WORDS, measured:
   (1) the arm must be the `else` -- the 0x80000 test is written `== 0` with
       the ordinary block inline, since the target's `bnez` sends the 0x80000
       arm out of line (as the `!= 0` arm it is 221 words / 153 differing);
   (2) the arm must END BY FALLING INTO the function's shared `return 0`
       rather than returning itself: written `if (... == 0) { return 0; }
       <stores> return 0;` it is 221 words, the extra two being a `j` and a
       second `addu $v0, $zero, $zero`;
   (3) the facing delta is an `s32` LOCAL. The target reads the carrier's
       +0x2A twice, once `lhu` for the +0x6A store and once `lh` for the
       subtraction; with the subtraction inline, force_to_mode masks the
       whole chain to 16 bits, both loads come out `lhu` and cse then folds
       them into one (219 words, 24 differing). An `s16` temp does the same;
       only the `s32` keeps the sign extension and therefore both loads.
   The carrier pointer must also be its own local in that tail -- re-read as
   `*(u8 **)(p + 0x60)` it is reloaded after the +0x2A store (223 words).
   219 words, 0 differing. */
s32 func_800AC82C(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    u8 *e;
    u8 *f;
    s16 d;
    s16 o0;
    s16 o1;
    u8 *x;
    s32 t;
    s16 out[4];

    *(s16 *)(c + 0x6) = 0;
    switch (a[0x9B])
    {
    case 0:
        e = *(u8 **)(*(u8 **)(p + 0x60) - 0x14);
        if (func_8004CAE8(*(u8 **)(e + 0x8), 0) == 0)
        {
            return 0;
        }
        a[0x9B] += 1;
    case 1:
        if (*(s16 *)(p + 0x64) < 0)
        {
            func_80094E34();
        }
        if (*(s8 *)(p + 0x6D) != 0)
        {
            func_800A9A0C(p);
        }
        if ((*(s32 *)(p + 0x1C) & 0x200000) != 0)
        {
            func_800AACA4(a, b, c, p);
            return 0;
        }
        e = *(u8 **)(*(u8 **)(p + 0x60) - 0x14);
        f = *(u8 **)(*(u8 **)(p + 0x60) - 0x18);
        c[0x24] = e[0x24];
        c[0x25] = e[0x25];
        if ((*(s32 *)(p + 0x1C) & 0x80000) == 0)
        {
            b->unk_10 = 0;
            b->unk_0C = 0;
            *(u16 *)(a + 0x98) &= 0xFFF7;
            *(s16 *)(p + 0x6A) = *(u16 *)(*(u8 **)(p + 0x60) + 0x2A);
            c[0x24] = e[0x24] + (u16)D_8006CCD8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            c[0x25] = e[0x25] + (u16)D_8006CCE8[(*(u16 *)(p + 0x6A) >> 9) & 7];
            func_800A2B04(b, c[0x24], c[0x25]);
            d = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6), *(u16 *)(p + 0x88) - 0x20);
            if (d < 0x200)
            {
                *(s16 *)(a + 0x92) = 0;
                *(s16 *)(p + 0x88) = d;
            }
            if (func_8009B88C(p, c[0x24], c[0x25], &o0, &o1) != 0)
            {
                c[0x24] = o0;
                c[0x25] = o1;
                func_800A2B04(b, c[0x24], c[0x25]);
                *(s32 *)(p + 0x1C) |= 0x40000000;
                func_8009A21C(c[0x24], c[0x25], (*(s32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
                p[0x71] = 0;
                return 1;
            }
            func_800A2B04(b, c[0x24], c[0x25]);
            func_800AA36C(a, b, c, p);
            return 0;
        }
        if (func_8003DE58(*(u8 **)(e + 0x8), e, out, 0) != 0)
        {
            *(s16 *)((u8 *)b + 0x2) = (u16)D_800E3DA8[0] + (*(u16 *)(f + 0x2) + out[0]);
            *(s16 *)((u8 *)b + 0x6) = (u16)D_800E3DA8[1] + (*(u16 *)(f + 0x6) + out[1]);
            *(s16 *)(p + 0x88) = *(u16 *)(*(u8 **)(p + 0x60) + 0x88);
            *(s16 *)(a + 0x92) = (*(u16 *)(f + 0xA) + out[2]) - *(u16 *)(p + 0x88) + 8;
            *(s16 *)(c + 0x6) = 0x10;
            *(s32 *)(p + 0x1C) |= 0x40000000;
            x = *(u8 **)(p + 0x60);
            t = *(s16 *)(p + 0x6A) - *(s16 *)(x + 0x2A);
            *(s16 *)(p + 0x6A) = *(u16 *)(x + 0x2A);
            *(s16 *)(p + 0x2A) -= t;
        }
        break;
    }
    return 0;
}

/* 19 words, 0 differing. */
void func_800ACB98(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 0x14;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(c + 0x14) |= 0x800;
    *(s16 *)(p + 0x96) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) |= 0x10000000;
}

/* The two-state fade-out behind the func_800C6AEC binder: case 0 re-tags the
   node and clears the colour cursor, case 1 eases the three +0xC..+0xE bytes
   towards zero by `-v / ++c[0xF]` and, once the divisor reaches 8, runs
   func_800B30E4's teardown with one extra flag move. 100 words, 0 differing,
   first compile.

   THE EASE IS A NEGATE-THEN-DIVIDE, not a subtract: `c[0xE] += -c[0xE] /
   c[0xF]` is the `negu`/`div` pair the target has, where `c[0xE] -= c[0xE] /
   c[0xF]` would be `div`/`subu`. The three stores are ONE CHAINED
   assignment, which is why they come out 0xE, 0xD, 0xC -- the same rule
   func_800ACD74 records for its four.

   The teardown is func_800B30E4 verbatim plus the +0x14 / +0x1C 0x400000
   hand-over, whose source value is read into a local because the target's
   `and` at 0x800ACD24 reuses the register the test loaded. */
void func_800ACBE4(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;
    s32 f;

    switch (p[0x9B])
    {
    case 0:
        n = (UnkNode *)(p - 0x20);
        func_80044A50(n);
        *(u16 *)(c + 0x12) -= 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        func_8004491C(n, (u32)func_800C6AEC);
        c[0xF] = 0;
        p[0x9B] += 1;
        break;
    case 1:
        if (c[0xF] < 8)
        {
            c[0xF] += 1;
            c[0xC] = c[0xD] = c[0xE] += -c[0xE] / c[0xF];
        }
        else
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            func_800A32A4(d);
            func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            f = *(s32 *)(d + 0x14);
            if (f & 0x400000)
            {
                *(s32 *)(d + 0x14) = f & ~0x400000;
                *(s32 *)(d + 0x1C) |= 0x400000;
            }
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* State 0x13. The four byte stores come out 0xF, 0xE, 0xD, 0xC because they
   are ONE CHAINED ASSIGNMENT, which emits right to left. 48 words,
   0 differing. */
void func_800ACD74(u8 *p, void *b, u8 *r, u8 *d)
{
    UnkNode *n;
    u8 *g;

    p[0x9A] = 0x13;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(r + 0x14) &= ~0x800;
    *(s16 *)(p + 0x96) = 0;
    n = (UnkNode *)(p - 0x20);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    func_80044A50(n);
    func_8004491C(n, (u32)func_800C6AEC);
    r[0xC] = r[0xD] = r[0xE] = r[0xF] = 0;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x12) -= 0x80;
    *(s32 *)(d + 0x1C) |= 0x10000000;
}

/* The fade handler behind func_800B3254 and func_800DA798. Case 0 eases the
   three +0xC..+0xE bytes towards 0x80 by (0x80 - v) / ++r[0xF]; the divisor is
   re-read FIVE times because each `sb` through the `u8 *` wipes cse's memory
   table and the next `lbu` is a zero-extend the SImode store cannot be
   forwarded into. Case 1 unlinks the node and returns 1.

   FOUR PARAMETERS, FORWARDED: func_800B3254 and func_800DA798 called this
   with NO arguments while it was a stub, exactly as their func_800AC480
   siblings did before that function was read. Both are widened in the same
   change and both stay 14 words / 0 differing. 85 words, 0 differing on the
   first compile. */
s32 func_800ACE34(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;

    switch (p[0x9B])
    {
    case 0:
        c[0xF] += 1;
        c[0xC] += (0x80 - c[0xC]) / c[0xF];
        c[0xD] += (0x80 - c[0xD]) / c[0xF];
        c[0xE] += (0x80 - c[0xE]) / c[0xF];
        if (c[0xF] >= 8)
        {
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            p[0x9B] += 1;
        }
        break;
    case 1:
        if (--c[0xF] == 0)
        {
            n = (UnkNode *)(p - 0x20);
            func_80044A50(n);
            *(u16 *)(c + 0x12) += 0x80;
            *(u16 *)(c + 0x14) &= 0xFFF3;
            func_8004491C(n, (u32)D_80045340);
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            return 1;
        }
        break;
    }
    return 0;
}

/* 0x80013714 IS A CONSTANT ADDRESS HELD IN A POINTER LOCAL, and here the
   local is what produces the shared `lui $s1` both tests read through. As
   structs.h's SYMBOL the prologue schedules differently -- `sw $ra` lands in
   the first branch's delay slot and the parameter copy fills the `lhu`'s load
   delay, so the target's `nop` is gone and the function is 51 words against
   52. Spelled `*(u16 *)0x80013714` at each site it is 53.
   52 words, 0 differing. */
void func_800ACF88(u8 *p)
{
    u8 *f;
    u8 *g;
    void *m;
    void *h;

    f = (u8 *)0x80010000;
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        D_800DCF4F = 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    m = func_800990FC();
    h = func_80099734(p, m);
    h = func_80099194(D_800E0C78, h);
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        h = (void *)func_8009929C(0x11, h);
        h = (void *)func_8009929C(0x4C, h);
        h = (void *)func_80099254(D_800E0458, h);
    }
    func_80099290(h);
    func_800A5720(m);
}

/* The four-state fade-and-retire machine the actor's +0x9B drives, and the
   widest member of the func_800D1A80 / func_800D34CC / func_800AB538 family:
   state 0 arms it unless D_8008346A says a transition is already running,
   state 1 sets the renderable's +0x10/+0x12/+0x14, raises the record's
   0x10000000, rings 0x806 for kinds 5 and 0x1E and 0x805 otherwise and arms
   the sixteen-step ramp, state 2 walks the three colour bytes towards 0x20
   and -- on the last step or when the renderable raises 0x8000 -- releases
   the engine's +0x10 slot and retires the record down one of two tails, and
   state 3 glides the mover towards D_80083780, shrinks the renderable's two
   scales and retires it the same way. 286 words, 0 differing.

   THREE SPELLINGS ARE LOAD-BEARING, each measured on this function:

   (1) THE RETIRE TAIL IS WRITTEN OUT IN BOTH ARMS, not reached by `break`
   from a shared one, and that is 52 of the 67 differing words. Cross-jumping
   runs in jump2, AFTER global-alloc, so the duplicated tail is still two
   copies when `allocno_compare` prices the four parameters -- it is what puts
   `d`'s allocno ahead of `p`'s, and the two swap $s1 and $s2 without it.
   With ONE copy in the source the function is still 286 words (the arm
   simply branches to it), which is why the error reads as pure register
   noise.

   (2) THE TWO D_80083460 POINTERS ARE TWO LOCALS, one per case -- the g/g2
   split func_800AB538 records for D_80083780. Shared, the pointer lives in
   two basic blocks, global-alloc owns it, and both blocks come out with the
   pointer and the value in each other's registers: 9 differing.

   (3) THE FIRST D_80083780 READ GOES THROUGH THE SYMBOL AND THE POINTER
   LOCAL IS ASSIGNED AFTER IT. cse unifies the two `lui`s either way, so the
   bytes are the same instructions; but with `h = D_80083780;` ahead of the
   first group the `addiu $s0, $s0, %lo` starts above it and sched2 leaves it
   one slot BELOW the second `jal` instead of one slot above -- 6 differing
   words, every one of them a shift.

   The three `/ *(s16 *)(p + 0x96)` divisions are written out, not cached in
   a local: the target reloads +0x96 for each one.

   K&R AND `s32`, both forced: the landed func_800D9F7C below calls this with
   NO arguments, so a prototyped four-parameter definition is "too few
   arguments"; and every exit sets $v0, which is a `return 0`/`return 1` and
   not a `void` tail. func_800D9F7C still measures 8 words, 0 differing. */
s32 func_800AD058(p, b, c, d)
u8 *p;
u8 *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *g2;
    u8 *h;

    switch (p[0x9B])
    {
    case 0:
        if (D_8008346A != 0)
        {
            return 0;
        }
        p[0x9B] = 1;
        /* fall through */
    case 1:
        *(s16 *)(c + 0x10) = 0x20;
        *(s16 *)(c + 0x12) -= 0x80;
        *(u16 *)(c + 0x14) |= 0xC;
        *(u32 *)(d + 0x1C) |= 0x10000000;
        func_800A56E0((d[0x13] == 5 || d[0x13] == 0x1E) ? 0x806 : 0x805);
        *(u32 *)(c + 0xC) = 0x808080;
        *(s16 *)(p + 0x96) = 0x10;
        p[0x9B]++;
        /* fall through */
    case 2:
        c[0xC] += (0x20 - c[0xC]) / *(s16 *)(p + 0x96);
        c[0xD] += (0x20 - c[0xD]) / *(s16 *)(p + 0x96);
        c[0xE] += (0x20 - c[0xE]) / *(s16 *)(p + 0x96);
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            return 0;
        }
        if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
        {
            g = (u8 *)&D_80083460;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
        }
        if ((*(s32 *)(d + 0x14) & 0x4000) == 0)
        {
            func_800A2FE0(d);
            func_800A32A4(d);
            if (d[0x49] != 0 && (d[0x4B] & 0x20) == 0)
            {
                func_800B8228(*(s16 *)(b + 0x2), *(s16 *)(b + 0x6), *(s16 *)(d + 0x88), d + 0x48);
            }
            if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
            {
                func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
            }
            func_8009A028(d);
            *(u16 *)(d - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return 1;
        }
        if ((*(s32 *)(d + 0x14) & 0x20000000) == 0)
        {
            func_800ACF88(d);
        }
        func_800A2FE0(d);
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_800A56E0(0x609);
        return 1;
    case 3:
        *(s32 *)(b + 0x0) +=
            ((*(s32 *)D_80083780 + ((*(s16 *)(p + 0x96) * func_80064584(c[0x27] << 7)) << 5)) - *(s32 *)(b + 0x0)) >> 2;
        h = D_80083780;
        *(s32 *)(b + 0x4) +=
            ((*(s32 *)(h + 0x4) + ((*(s16 *)(p + 0x96) * func_800644B8(c[0x27] << 7)) << 5)) - *(s32 *)(b + 0x4)) >> 2;
        *(s16 *)(d + 0x88) +=
            ((*(s16 *)(h + 0xA) - (func_800644B8(*(s16 *)(p + 0x96) << 3) >> 6)) - *(s16 *)(d + 0x88)) >> 4;
        *(s16 *)(c + 0x1C) -= *(u16 *)(c + 0x1C) / *(s16 *)(p + 0x96);
        *(s16 *)(c + 0x1E) -= *(u16 *)(c + 0x1E) / *(s16 *)(p + 0x96);
        c[0x27]++;
        *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        if (*(s16 *)(p + 0x96) > 0)
        {
            return 0;
        }
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0xA) -= 1;
        func_800A2FE0(d);
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(c[0x24], c[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return 1;
    }
    return 0;
}

/* 38 words, 0 differing. */
void func_800AD4D0(u8 *p)
{
    s32 v;

    if (*(s16 *)(p + 0x64) < 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x1D) != 0)
        {
            *(s16 *)(p + 0x64) = 0;
        }
    }
    v = p[0x28] + *(s16 *)(p + 0x64);
    if (v < 0)
    {
        v = 0;
    }
    else if (v >= 0x100)
    {
        v = 0xFF;
    }
    p[0x28] = v;
    *(s16 *)(p + 0x64) = 0;
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

/* Clamp the +0x64 cursor so cursor + step never passes the limit at +0x29.

   THE THREE LOADS ARE LOCALS. As one condition the two bytes are re-read
   inside the arm -- the `slt` kills the register holding +0x29 before cse
   can reuse it -- and the function comes out 16 words against 11.
   11 words, 0 differing. */
/* NOT `s16`: the return is already a `lh`, so the width costs this function
   nothing -- but an s16 return makes every caller sign-extend $v0 before
   testing it, which is a word func_800BD1C4 does not have (167 against 166).
   That also says the two were in different translation units: the caller saw
   no prototype at all. */
s32 func_800AD568(u8 *p)
{
    s32 v;
    s32 a;
    s32 b;

    v = *(s16 *)(p + 0x64);
    a = p[0x28];
    b = p[0x29];
    if (v + a > b)
    {
        *(s16 *)(p + 0x64) = b - a;
    }
    return *(s16 *)(p + 0x64);
}

/* Apply `dmg` to the actor's +0x24 HP word: halve it behind the +0x54 flag,
   double it behind probe 0xF, clamp, then fold the high byte against +0x66
   and run the death path.

   D_80012090 IS THE CONSTANT ADDRESS READ INTO A LOCAL, and that single
   spelling is the whole landing. As the declared symbol it is a two-insn
   `high` + `lo_sum`, so the `lui` goes to its own register, reorg steals it
   into the preceding `bnez`'s delay slot and the comparison's
   `addiu $v0, $zero, 1` covers the load delay the target leaves as part of
   its macro: 90 words / 13 differing with `p[0x25] = D_80012090` (which also
   re-reads the global as an `lbu`), 89 / 38 once a local kills that re-read.
   `*(s32 *)0x80012090` used at BOTH sites is 91 / 54 -- two uses build the
   address in a register with `lui`/`ori`. Read once into `s32 k` it is the
   one tied `lui $a0` / `lw $a0, %lo($a0)` macro the target has, and `sb $a0`
   is that same register. 90 words, 0 differing. The candidate therefore
   emits no relocation for those two words, as every other D_80012090 site in
   this unit does. */
void func_800AD594(u8 *p, s32 dmg)
{
    u8 *g;
    s32 hp;
    s32 hi;
    s32 k;

    if (*(s8 *)(p + 0x13) > 0)
    {
        hp = *(u16 *)(p + 0x24);
        hi = p[0x25];
        if ((s16)func_80042900((s8 *)p, 0x20) != 0)
        {
            dmg = 0;
        }
        else if (*(s32 *)(p + 0x54) & 0x100)
        {
            dmg = (dmg + 1) >> 1;
        }
        if ((s16)func_80042900((s8 *)p, 0xF) != 0)
        {
            dmg = dmg << 1;
        }
        hp -= dmg;
        if (hp < 0)
        {
            hp = 0;
        }
        else if (hp > 0x10000)
        {
            hp = 0xFFFF;
        }
        *(s16 *)(p + 0x24) = hp;
        if (p[0x66] < p[0x25])
        {
            p[0x25] = p[0x66];
            p[0x24] = 0xFF;
        }
        if (p[0x25] == 0)
        {
            k = *(s32 *)0x80012090;
            if (k == 1 && (p[0x13] == 2 || p[0x13] == 0x39))
            {
                p[0x25] = k;
            }
            else
            {
                if (*(s32 *)(p + 0x1C) & 0x80000)
                {
                    func_80094E34();
                }
                if (hi != 0)
                {
                    g = (u8 *)&D_80083460;
                    *(u16 *)(g + 0xA) += 1;
                }
            }
        }
    }
}

/* 174/174 words, 0 differing.  Builds one of four message strings for a
   record, adds a score to its +0x24 counter and clamps it.
   Parked at 31 aligned differences; the whole residue was one $s2/$s4
   exchange between `mode' and the third argument, plus six rows of delay
   slots around the double condition, and TWO SPELLING FACTS closed both.

   (1) THE THIRD ARGUMENT IS USED THROUGH A CAST AT EVERY USE AND IS NEVER
   COPIED TO A LOCAL.  A `p = (u8 *)arg;' at the top -- which reads far
   better -- costs 25 of the 31 rows, and the mechanism is a REGISTER NOTE,
   not the copy instruction (combine deletes that).  assign_parms
   (function.c:4014) puts a REG_EQUIV on each incoming parameter's copy
   insn, and update_equiv_regs (local-alloc.c:1094) DOUBLES the
   REG_LIVE_LENGTH of any pseudo that has one and is set exactly once.
   When a local copy exists, cse rewrites the parameter pseudo away and the
   surviving insn's note is a REG_EQUAL, which update_equiv_regs ignores:
   the argument prices at 10 refs / 89 insns = 3370 instead of 10 / 178 =
   1685, and sorts ABOVE `c' (16 / 240 = 2666) in allocno_compare instead of
   below it.  Measured: with the copy, `arg' got $s2 and `mode' $s4; without
   it, the order is c > mode > arg and the two had merely swapped (39 rows).

   (2) THE DOUBLE CONDITION IS TWO `else if' ARMS WITH THEIR OWN `mode = 2;'
   -- not `if (a && b || c && d) mode = 2;'.  That is worth both of the
   remaining levers at once, which is why neither was findable alone:
     * the second `mode = 2' is an 11th reference to `mode', taking it from
       10 / 116 = 2586 to 11 / 118 = 2796, just past `c' at 16 / 244 = 2622.
       Nothing else cheap reaches that: `mode++; mode--;' (14 refs) and a
       matched-operand asm (12 refs) both overshoot but still measure 10.
     * each arm materialises `addiu $v0, 0xC' for itself, so jump2's
       cross-jump merges the arms only from the `bne' onward and dbr has a
       constant to steal into BOTH delay slots (0x800AD7B0 and 0x800AD7C0),
       leaving the shared block's head as the `lbu's load-delay nop.
   The single-condition form needs a device to stop -fcse-skip-blocks
   commoning the two `p[1]' reads, and the best one found --
   `*(volatile u8 *)(arg + 1) == 0xC' on the second test -- puts the re-read
   at the TOP of the join block, where nothing can move across it: 174 words
   but 10 rows, and 31 before fact (1).  Two arms need no volatile at all.

   Measured and rejected on the final body: the condition as one `if' with
   the volatile re-read (10), with `mode++; mode--;' instead of the second
   assignment (10), the arms duplicated down to two full `p[1] == 0xC &&
   p[0] == 6' tests (171-182 words, 8-28 rows over all 16 volatile
   placements), the third parameter redeclared `u8 *' (0 differing as well,
   but it needs 53 call sites in two other units changed, so the cast is
   preferred).  `((u8 *)arg)[1]' and `*(u8 *)(arg + 1)' are both 0.  */
s32 func_800AD6FC(u8 *c, s32 mode, s32 arg)
{
    u8 *m;
    u8 *h;
    s32 t;
    s32 t0;

    m = func_800990FC();
    h = m;
    if ((s16)func_80042900((s8 *)c, 0x19) != 0)
    {
        h = func_80099194(D_800E0CAE, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        func_80099290(h);
        func_800A5720(m);
        return 0;
    }
    if (mode == 3 && arg != 0)
    {
        if (((u8 *)arg)[1] == 2 && ((u8 *)arg)[0] == 1)
        {
            mode = 2;
        }
        else if (((u8 *)arg)[1] == 0xC && ((u8 *)arg)[0] == 6)
        {
            mode = 2;
        }
    }
    t = *(u16 *)(c + 0x24);
    t0 = t;
    switch (mode)
    {
    case 0:
        h = func_80099194(D_800E0CDC, h);
        h = func_80099734(c, h);
        h = func_80099194(D_80089084, h);
        break;
    case 1:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0CF3, h);
        t += 0x300;
        break;
    case 2:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D1B, h);
        if (arg != 0 && ((u8 *)arg)[1] == 0xC && ((u8 *)arg)[0] == 6)
        {
            t += 0xA00;
        }
        else
        {
            t += 0x3200;
        }
        break;
    case 3:
        h = func_80099734(c, h);
        h = func_80099194(D_800E0D36, h);
        t += 0x6400;
        break;
    }
    if (arg != 0 || mode == 0)
    {
        func_80099290(h);
        func_800A5720(m);
    }
    if (t0 != t)
    {
        func_800A56E0(0x51E);
    }
    if (t > 0x10000)
    {
        t = 0xFFFF;
    }
    *(u16 *)(c + 0x24) = t;
    if (c[0x25] != 0)
    {
        *(s32 *)(c + 0x1C) &= ~0x8;
    }
    if (c[0x66] < c[0x25])
    {
        c[0x25] = c[0x66];
        c[0x24] = 0xFF;
    }
    return mode;
}

/* Can the actor step onto the tile the view names? THE func_800B500C RESULT
   IS `s16`: through an s32 local the `sll 16`/`sra 16` pair in front of the
   `bltz` disappears, 75 words / 41 differing. func_800AAA28's signature was
   narrowed to two parameters on this function's evidence -- see its comment.
   76 words, 0 differing. */
s32 func_800AD9B4(u8 *p, u8 *q)
{
    u8 *g;
    u8 *h;
    s16 i;
    s32 d;

    if (*(u16 *)(q - 0x2) & 0x8000)
    {
        return 1;
    }
    g = D_80082E80;
    if (*(s8 *)(g + 0x26) != *(s8 *)(p + 0x26) && func_8009FD40(g, p) >= 7)
    {
        return 1;
    }
    if (q[0x25] == 0)
    {
        return 1;
    }
    i = func_800B500C(p[0x24], p[0x25], *(s16 *)(q + 0x88));
    if (i < 0)
    {
        return 1;
    }
    if ((D_800E3648_s[i].unk_03 & 0x80) == 0)
    {
        return 1;
    }
    h = (u8 *)&D_80083460;
    if (*(u16 *)(h + 0x2) & 0x1000)
    {
        d = *(s8 *)(q + 0x71);
        if (d > 0)
        {
            *(u16 *)(h + 0x8) -= d - *(u16 *)(q + 0x8A);
            q[0x71] = 0;
        }
    }
    func_800A9A0C(q);
    func_800AAA28(p, q);
    return 0;
}

/* func_80095464's probe again, carried past the two `return 1` gates into the
   +0x03 clear of func_800954E0 and a fourth call. The return is s16: the two
   constants need no extension, and the `sll`/`sra` pair on the tail is where
   func_800B627C's s32 result is narrowed. 42 words, 0 differing. */
s16 func_800ADAE4(u8 *a, u8 *b)
{
    s16 i;
    u8 f;
    s32 r;

    i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
    if (i < 0)
    {
        return 1;
    }
    f = D_800E3648_s[i].unk_03;
    if ((f & 0x80) == 0)
    {
        return 1;
    }
    if ((f & 0x40) == 0)
    {
        D_800E3648_s[i].unk_03 = f & 0x7F;
    }
    r = func_800B627C(*(u8 *)&D_800E3648_s[i], b, i, 0);
    if (r == 0)
    {
        return 0;
    }
    return r;
}

/* 16 words, 0 differing. */
void func_800ADB8C(u8 *p)
{
    if (**(u8 **)p != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node walk again (see func_800BC054), with the record's +0x8 parked in
   the scratchpad each turn and the visitor skipped when the slot's +0x14
   carries 0x80.

   TWO SPELLINGS ARE LOAD-BEARING. The scratchpad address needs a POINTER
   LOCAL: written as `*(s32 *)0x1F80013C` it is the assembler's absolute
   macro, a `lui $at`/`sw` pair that cannot be hoisted out of the loop, and
   the function is 28 words. And the slot must be a `u8 *`, not an
   `UnkSlot *`: through the struct pointer the +0x14 read is MEM_IN_STRUCT_P,
   sched1 hoists it over the scratchpad store and the three load-delay `nop`s
   the target has disappear -- 29 words at 26 differing.
   32 words, 0 differing. */
s32 func_800ADBCC(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = *(s16 *)(p + 0x8);
        if (!(*(u16 *)(s + 0x14) & 0x80))
        {
            func_800453E0(p, v, s, *(s16 *)(s + 0x6));
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (u8 *)n->unk_0C;
    }
}

/* The 0x12 spawner. No caller anywhere in the project, so the arity is read
   off the four argument registers alone. 53 words, 0 differing. */
UnkNode *func_800ADC4C(u8 *a, s32 b, s16 c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800ADB8C;
        func_8004491C(n, (u32)func_800ADBCC);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(a + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(a + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(a + 0xA);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x8) = b;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)((u8 *)n + 0x28) = c;
        *(u8 **)((u8 *)n + 0x20) = d;
        *d = 0;
    }
    return n;
}

/* Set bit 0x8000 of +0x14 once, then park bit 0x100 of +0x98 on whether a
   fresh random number divides by n. The draw goes into an `s32` and
   func_800A6D30 is declared u16, so the `andi $v0, 0xFFFF` is the widening at
   the assignment. 32 words, 0 differing. */
void func_800ADD20(u8 *p, s32 n)
{
    s32 r;

    if ((*(s32 *)(p + 0x14) & 0x8000) == 0)
    {
        *(s32 *)(p + 0x14) |= 0x8000;
        r = func_800A6D30();
        if (r % n != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFEFF;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0x100;
        }
    }
}

/* Two narrow-type facts, each worth one word. (1) The result local is `s32`
   with `(s16)` casts at its uses, not an `s16` local: the target's first
   `sll $v0, $s1, 16` reads the CALLEE-SAVED copy, and an s16 local lets cse
   substitute the return register back into the extension. (2) func_8009A180's
   result is tested as a HALFWORD -- without the cast the test is a bare
   `beqz` and the function is 52 words / 21 differing. 53 words,
   0 differing. */
s16 func_800ADDA0(void *a, void *b, u8 *c, s16 d, s16 e, s32 f)
{
    u8 *g;
    s32 r;

    r = func_800ADE74(a, b, c, d, e, f);
    if ((s16)r != 0 && (s16)r != 2 && (s16)r != 3)
    {
        g = (u8 *)&D_80083460;
        if (*(s16 *)(g + 0x8) != 0 || (*(u16 *)(g + 0x2) & 0x2808) != 0)
        {
            if ((s16)func_8009A180(c, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                *(u16 *)(c + 0x46) &= 0x7FFF;
                return -1;
            }
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089074);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089080);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADE74);

/* The four-state step of the 0x800AFAD8 object. The `% 7168` tail is written
   in BOTH arms and jump.c cross-jumps it, which is the single `mfhi`/`subu`
   run the two cases share. 73 words, 0 differing. */
void func_800AFAD8(u8 *p)
{
    s16 v;

    switch (*(s16 *)(p + 0x2))
    {
    case 0:
        *(u16 *)(p + 0x6) += 2;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 1:
        *(u16 *)(p + 0x6) += 4;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 2:
        v = *(u16 *)(p + 0x8) + 0x40;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    case 3:
        *(u16 *)(p + 0x6) += 8;
        v = (func_800644B8(*(s16 *)(p + 0x0) << 5) >> 1) + 0xC00;
        *(u16 *)(p + 0x0) += 1;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFBFC);

/* The vertical strip of gouraud-textured quads one ring segment is drawn as:
   fourteen RotAverageNclip4 passes up one 0xE00-per-step column of the
   scratchpad quad, each visible pass emitting TWO POLY_GT4s split across the
   mid-edge, and returning the advanced primitive cursor (or NULL when nothing
   was emitted). func_800AFBFC is the only caller and passes 0 or 1 as the
   fifth argument.

   THIS IS csrc/ovl_town.c's func_800A41D8 WITH POLY_GT4 FOR POLY_FT4. Every
   load-bearing spelling that function's comment records transfers verbatim --
   the bitwise `|` of four `&&` pairs with the `(u16)` casts, the `>> 1`
   midpoints, the chained colour assignment running b/g/r with p[0] before
   p[1], the two divides by twelve, `sp->unk_0C` first in the +0x14 sum, the
   `u0`/`u1` word pair with `u2`/`u3` as halves and the four decrements after,
   and `s16 flag` arriving in a stack slot. The differences are that this one
   has no `p->code |= c->unk_0F & 2;`, no DR_MODE pass and so no second
   AddPrim loop, and that the two colour values come out of a depth the
   scratchpad carries instead of the caller's +0x16 field. 356 words,
   0 differing; seqcheck `seq=OK`, `arith=OK`, 4 relocated words.

   THE SECOND COLOUR VALUE IS THE SAME VARIABLE, `d = d - 0x10;`, NOT A
   SECOND LOCAL. With a separate `s16 w = d - 0x10;` the function is still
   356 words but 8 differing, and every one of the eight is a register
   number: a three-way rotation in which the SImode temp holding
   `0xE0 - (...)`, its sign-extension and `d` itself take $v1/$a1/$a2 where
   the target takes $a1/$a2/$v1. The cause is global.c's `allocno_compare`,
   read out of the .greg/.lreg dumps rather than swept for: with a separate
   `w`, `d` is pseudo 91 with 4 refs across 9 insns, priority
   floor_log2(4)*4/9*10000 = 8888, and it sorts SEVENTEENTH in
   `allocno_order` -- behind the temp (8 refs / 7 insns, 34285), which
   therefore takes $v1 first. Reusing `d` deletes the third pseudo and the
   order comes out right. Declaration order of the locals is NOT the lever
   here: d/w/v, v/w/d and `w = d; w -= 0x10;` all measure the same 8.

   `lo` IS `s16`, NOT `u16`. As `u16` the function is 355 words / 181
   differing: the one missing word is `addu $v0, $t0, $zero`, the copy that
   stands between the spilled flag's `lhu` and its `beqz`, and the shift it
   causes drags the whole `d` block with it. `s32` is 355 / 184, `u8` 355 /
   182, `volatile u16` 354 / 200.

   THE CALLEES ARE SPELLED `RotNclip4` / `SetPolyGT4` / `AddPrim`.  They are
   declared by psyq/include and resolve through splat/common_syms.txt, which
   build/dungeon.bin did NOT link until 2026-11-15 -- the Makefile passed that
   script for town.bin and main.bin only, so the library names compiled,
   measured 356/356 at 0 differing, and then failed the link.  The dungeon
   rule passes it now and all four containers stay byte-identical. */
POLY_GT4 *func_800AFFB4(u8 *a, void *c, UnkDungeonSP5 *sp, POLY_GT4 *p, s16 flag)
{
    s32 i;
    s32 j;
    s32 n;
    u16 z;
    s16 lo;
    s16 d;
    s16 v;
    UnkDungeonAFFB4T *t;

    n = 0;
    lo = *(s16 *)(a + 0x8) >= 0xE00;
    z = *(u16 *)(a + 0x8);
    sp->unk_70[0].vz = sp->unk_70[1].vz = z - 0x6200;
    sp->unk_70[2].vz = sp->unk_70[3].vz = z - 0x5400;
    for (i = 0; i < 0xE; i++)
    {
        sp->unk_70[0].vz += 0xE00;
        sp->unk_70[1].vz += 0xE00;
        sp->unk_70[2].vz += 0xE00;
        sp->unk_70[3].vz += 0xE00;
        if (RotNclip4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                      (s32 *)&sp->unk_F0[0], (s32 *)&sp->unk_F0[1], (s32 *)&sp->unk_F0[2],
                      (s32 *)&sp->unk_F0[3], (s32 *)&sp->unk_90, (s32 *)&sp->unk_C0,
                      (s32 *)&sp->unk_94) > 0)
        {
            if (((u16)(sp->unk_F0[0].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[0].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[1].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[1].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[2].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[2].vy + 0x20) < 0x121) |
                ((u16)(sp->unk_F0[3].vx + 0x20) < 0x181 &&
                 (u16)(sp->unk_F0[3].vy + 0x20) < 0x121))
            {
                *(u32 *)&p[0].x0 = *(u32 *)&sp->unk_F0[0];
                *(u32 *)&p[0].x2 = *(u32 *)&sp->unk_F0[2];
                *(u32 *)&p[1].x1 = *(u32 *)&sp->unk_F0[1];
                *(u32 *)&p[1].x3 = *(u32 *)&sp->unk_F0[3];
                p[0].x1 = p[1].x0 = (sp->unk_F0[0].vx + sp->unk_F0[1].vx) >> 1;
                p[0].x3 = p[1].x2 = (sp->unk_F0[2].vx + sp->unk_F0[3].vx) >> 1;
                p[0].y1 = p[1].y0 = (sp->unk_F0[0].vy + sp->unk_F0[1].vy) >> 1;
                p[0].y3 = p[1].y2 = (sp->unk_F0[2].vy + sp->unk_F0[3].vy) >> 1;
                sp->unk_90 = sp->unk_70[0].vz % 0xE00;
                sp->unk_90 = sp->unk_90 / 0x100;
                if (lo)
                {
                    sp->unk_90 += 0x10;
                }
                d = 0xE0 - ((u16)sp->unk_90 + i * 16);
                v = 0x80;
                if (d <= 0x80)
                {
                    v = d;
                    if (d < 0)
                    {
                        v = 0;
                    }
                }
                p[1].r0 = p[1].r1 = p[0].r1 = p[0].r0 = p[1].g0 = p[1].g1 = p[0].g1 = p[0].g0 =
                    p[1].b0 = p[1].b1 = p[0].b1 = p[0].b0 = v;
                d = d - 0x10;
                if (d > 0x80)
                {
                    d = 0x80;
                }
                else if (d < 0)
                {
                    d = 0;
                }
                p[1].r2 = p[1].r3 = p[0].r3 = p[0].r2 = p[1].g2 = p[1].g3 = p[0].g3 = p[0].g2 =
                    p[1].b2 = p[1].b3 = p[0].b3 = p[0].b2 = d;
                t = &((UnkDungeonAFFB4T *)sp->unk_118)[(i & 1) * 2 + flag * 4];
                for (j = 0; j < 2; j++)
                {
                    SetPolyGT4(p);
                    sp->unk_08 = t->unk_08;
                    sp->unk_0C = t->unk_09;
                    sp->unk_10 = t->unk_0A;
                    sp->unk_14 = t->unk_0B;
                    sp->unk_10 = sp->unk_10 + sp->unk_08;
                    sp->unk_14 = (sp->unk_0C + sp->unk_14) * 0x100;
                    sp->unk_0C = sp->unk_0C * 0x100;
                    *(u32 *)&p->u0 = (sp->unk_0C + sp->unk_08) | (t->unk_06 << 16);
                    *(u32 *)&p->u1 = (sp->unk_0C + sp->unk_10) | (t->unk_04 << 16);
                    *(u16 *)&p->u2 = sp->unk_14 + sp->unk_08;
                    *(u16 *)&p->u3 = sp->unk_14 + sp->unk_10;
                    p->u1--;
                    p->u3--;
                    p->v2--;
                    p->v3--;
                    AddPrim((u32 *)sp->unk_20 + (t - (UnkDungeonAFFB4T *)sp->unk_118) /
                                                    sizeof(UnkDungeonAFFB4T),
                            p);
                    p++;
                    t++;
                }
                n++;
            }
        }
    }
    if (n == 0)
    {
        return NULL;
    }
    return p;
}

/* Open the kind-0x12 panel for corner `k`. THE RENDERABLE POINTER IS RE-READ
   PER GROUP, and the source says so by assigning the local three times: the
   +0x8 store, the three colour bytes and the two scales are each one group
   off their own `lw 0xC($s0)`. Written inline per field every store reloads
   and the function is 74 words / 46 differing; with ONE assignment it would
   be one load for all six. The three colour bytes go 0xE, 0xD, 0xC and the
   two scales 0x1E, 0x1C -- descending in both groups. The mover at +0x08 IS
   written inline, which is where its three reloads come from. 68 words,
   0 differing. */
void func_800B0544(s16 k)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *w;
    s32 v;

    if (D_800DCF4C != k)
    {
        func_8003E1C4(k);
        D_800DCF4C = k;
    }
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800AFAD8;
        func_8004491C(n, (u32)func_800AFBFC);
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DEF90;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_0E = 0xE0;
        s->unk_0D = 0xE0;
        s->unk_0C = 0xE0;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = 0x4000;
        v = *(s32 *)0x80012090;
        w = (u8 *)n + 0x20;
        if (v == 0)
        {
            if (D_8008146C == 0x28)
            {
                *(s16 *)(w + 0x2) = 1;
            }
        }
        else if (v == 2)
        {
            *(s16 *)(w + 0x2) = v;
        }
    }
}

/* Project the record's (x, y, z) through RotTransPers out of the third
   argument's own scratch at +0x28, and bias the returned depth by the second
   argument's signed +0x4 byte. 27 words, 0 differing. */
void func_800B0654(u8 *a, u8 *b, u8 *c)
{
    *(s16 *)(c + 0x28) = *(u16 *)(a + 0x2);
    *(s16 *)(c + 0x2A) = *(u16 *)(a + 0x6);
    *(s16 *)(c + 0x2C) = *(u16 *)(a + 0xA);
    *(s32 *)(c + 0xC0) = RotTransPers((SVECTOR *)(c + 0x28), (s32 *)(c + 0xB8), (s32 *)(c + 0x90),
                                      (s32 *)(c + 0x94)) +
                         *(s8 *)(b + 0x4);
}

/* 12 words, 0 differing. */
void func_800B06C0(u8 *v, u8 *q, u8 *p)
{
    *(s16 *)(p + 0xB8) = *(u16 *)(v + 0x2);
    *(s16 *)(p + 0xBA) = *(u16 *)(v + 0x6);
    *(s32 *)(p + 0xC0) = *(s16 *)(v + 0xA) + *(s8 *)(q + 0x4);
}

s32 func_800B06F0(u8 *p, UnkVec *v, UnkSlot *s)
{
    MATRIX *m = (MATRIX *)0x1F8000D0;
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPC *sp = (UnkDungeonSPC *)0x1F800000;
    UnkNode *n;
    u8 ***e;
    u32 *col;
    POLY_FT4 *q;
    volatile u8 *r;
    s32 ta;
    s32 tb;
    s32 tc;
    s32 td;
    s32 u;

    sp->unk_EC = *(s32 *)((u8 *)g + 0xA0);
    q = *(POLY_FT4 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_20 = (u32)(D_80083160.unk_00 + 0xB0);
    for (;;)
    {
        D_800DEFF0[*(u16 *)(p + 0x8) & 1](v, s, sp);
        e = *(u8 ***)p;
        col = *(u32 **)(p + 0x4);
        if ((u32)sp->unk_C0 < 0x1E0)
        {
            sp->unk_C0 = sp->unk_C0 * 4 + sp->unk_20;
            sp->unk_B8 -= 0xA0;
            sp->unk_BA -= 0x78;
            PushMatrix();
            sp->unk_100.vx = s->unk_16;
            sp->unk_100.vy = s->unk_18;
            sp->unk_100.vz = s->unk_1A;
            sp->unk_E4 = s->unk_20;
            sp->unk_E8 = s->unk_22;
            sp->unk_30 = s->unk_1C;
            sp->unk_34 = s->unk_1E;
            sp->unk_38 = 0x1000;
            RotMatrix(&sp->unk_100, m);
            ScaleMatrix(m, (VECTOR *)&sp->unk_30);
            SetTransMatrix(m);
            SetRotMatrix(m);
            r = (volatile u8 *)*e;
            if (r != NULL)
            {
                do
                {
                again:
                {
                    sp->unk_70[3].vz = 0;
                    sp->unk_70[2].vz = 0;
                    sp->unk_70[1].vz = 0;
                    sp->unk_70[0].vz = 0;
                    r = (volatile u8 *)((u32)r | 0x80000000);
                    if ((r[0x0] ^ s->unk_14) & 1)
                    {
                        ta = -(s8)r[0x2] - s->unk_20;
                        sp->unk_70[2].vx = ta;
                        sp->unk_70[0].vx = ta;
                        ta = ta - (*(u8 *)(r + 0xA));
                        sp->unk_70[3].vx = ta;
                        sp->unk_70[1].vx = ta;
                    }
                    else
                    {
                        tb = (s8)r[0x2] - s->unk_20;
                        sp->unk_70[2].vx = tb;
                        sp->unk_70[0].vx = tb;
                        tb += (*(u8 *)(r + 0xA));
                        sp->unk_70[3].vx = tb;
                        sp->unk_70[1].vx = tb;
                    }
                    if ((r[0x0] ^ s->unk_14) & 2)
                    {
                        tc = -(s8)r[0x3] - s->unk_22;
                        sp->unk_70[1].vy = tc;
                        sp->unk_70[0].vy = tc;
                        tc = tc - (*(u8 *)(r + 0xB));
                        sp->unk_70[3].vy = tc;
                        sp->unk_70[2].vy = tc;
                    }
                    else
                    {
                        td = (s8)r[0x3] - s->unk_22;
                        sp->unk_70[1].vy = td;
                        sp->unk_70[0].vy = td;
                        td += (*(u8 *)(r + 0xB));
                        sp->unk_70[3].vy = td;
                        sp->unk_70[2].vy = td;
                    }
                    RotTransPers4(&sp->unk_70[0], &sp->unk_70[1], &sp->unk_70[2], &sp->unk_70[3],
                                  &sp->unk_F0[0].w, &sp->unk_F0[1].w, &sp->unk_F0[2].w,
                                  &sp->unk_F0[3].w, &sp->unk_90, &sp->unk_94);
                    if (*(s8 *)r >= 0 && (*(u8 *)r & 8))
                    {
                        u = *(u8 *)r & 4;
                        if (NormalClip(sp->unk_F0[0].w, sp->unk_F0[1].w, sp->unk_F0[2].w) <= 0)
                        {
                            if (u == 0)
                            {
                                goto skip;
                            }
                        }
                        else if (u != 0)
                        {
                            goto skip;
                        }
                    }
                    goto draw;
                skip:
                    if (r[0x1] != 0x2C)
                    {
                        goto nextelem;
                    }
                    goto bump;
                draw:
                    if (*(u8 *)r & 4)
                    {
                        func_8003E12C(&sp->unk_F0[0].w, &sp->unk_F0[1].w);
                        func_8003E12C(&sp->unk_F0[2].w, &sp->unk_F0[3].w);
                    }
                    q->x0 = sp->unk_F0[0].h[0] + sp->unk_B8;
                    q->y0 = sp->unk_F0[0].h[1] + sp->unk_BA;
                    q->x1 = sp->unk_F0[1].h[0] + sp->unk_B8;
                    q->y1 = sp->unk_F0[1].h[1] + sp->unk_BA;
                    q->x2 = sp->unk_F0[2].h[0] + sp->unk_B8;
                    q->y2 = sp->unk_F0[2].h[1] + sp->unk_BA;
                    q->x3 = sp->unk_F0[3].h[0] + sp->unk_B8;
                    q->y3 = sp->unk_F0[3].h[1] + sp->unk_BA;
                    if ((r[0x1] & 0xFC) == 0x2C)
                    {
                        q = (POLY_FT4 *)func_800B0BE0((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        func_8004C010((u8 *)q - 0x24, col);
                        if (r != NULL)
                        {
                            goto again;
                        }
                    bump:
                        col++;
                    }
                    else
                    {
                        if (sp != NULL || s->unk_14 != 0)
                        {
                            q = (POLY_FT4 *)func_8004CD28((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        }
                        else
                        {
                            q = (POLY_FT4 *)func_8004CD28((u8 *)sp->unk_C0, (u8 *)s, (u8 **)&r, (u8 *)q);
                        }
                        if (r != NULL)
                        {
                            goto again;
                        }
                    }
                }
                nextelem:
                    e++;
                    r = (volatile u8 *)*e;
                } while (*(u8 *volatile *)&r != NULL);
            }
            PopMatrix();
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
    return 0;
}

/* Build one POLY_FT4 (0x28 bytes) for the 0xC-byte sprite descriptor at *c
   and hand it to AddPrim, then step the cursor or clear it when the
   descriptor's first byte is negative.

   THE CURSOR IS ADVANCED BEFORE THE TAIL `if`, not in the `return`:
   `return q + 0x28;` is 84 words / 12 differing, because the target has
   `addiu $s1, $s1, 0x28` in the `bgez`'s delay slot and then a bare
   `addu $v0, $s1, $zero`. The four UV pairs are CHAINED assignments
   (`q[0xC] = q[0x1C] = ...`), which is what gives the target's
   high-offset-then-low store order off one computed value; `*c` is re-read
   once per group because the previous group's stores invalidate it.
   85 words, 0 differing. */
u8 *func_800B0BE0(u8 *a, u8 *b, u8 **c, u8 *q)
{
    s32 t;

    *(u32 *)(q + 0x4) = *(u32 *)(b + 0xC);
    SetPolyFT4(q);
    *(s16 *)(q + 0xE) = *(u16 *)(*c + 0x6);
    t = *(u16 *)(b + 0x10);
    if (t != 0)
    {
        *(s16 *)(q + 0x16) = t + (*(u16 *)(*c + 0x4) & 0xFF9F);
    }
    else
    {
        *(s16 *)(q + 0x16) = *(u16 *)(*c + 0x4);
    }
    q[0xC] = q[0x1C] = (*c)[0x8];
    q[0x14] = q[0x24] = (*c)[0x8] + (*c)[0xA] - 1;
    q[0xD] = q[0x15] = (*c)[0x9];
    q[0x1D] = q[0x25] = (*c)[0x9] + (*c)[0xB] - 1;
    AddPrim(a, q);
    q += 0x28;
    if (*(s8 *)*c < 0)
    {
        *c = NULL;
    }
    else
    {
        *c = *c + 0xC;
    }
    return q;
}

/* The mirror/tail step on a node's +0x10: pick the reflected direction from
   the +0x12 phase, stamp two 3-byte colour cells when the actor's gauges say
   so, then run the two-state fade on the slot's +0x18 -- state 1 tears the
   node and its four children down below -0x3FF -- and finally track the
   actor's position into the mover with D_800DDC40's per-kind height.
   135 words, 0 differing.

   `m` MUST BE DECLARED `s16`. The direction is `m = k;` and then, inside the
   phase test, `m = -k & 0x1F;` and `m = 0x10;` -- three copies into one
   variable. As `s32` cse's `make_regs_eqv` merges m with k (m's last mention
   is later, so m becomes the canonical register), both copies vanish and the
   function is 133 words / 126 differing; an `if`/`else` instead is 135/100
   but lays the `else` arm out after a `j`. As `s16` the copy is
   `(set (reg:HI m) (subreg:HI k))`, `make_regs_eqv` never runs, and both
   `addu $a3` copies survive -- and `m << 3` still costs no extension because
   gcc knows the value is 0..0x1F.

   CASE 1'S EARLY ARM IS THE *NORMAL* ONE. With the teardown written first as
   `if (c18 < -0x3FF) { ...; return; }` it is laid out inline and the branch
   sense inverts -- 135 words, 7 differing.

   THE FOUR-SLOT SWEEP IS AN INDEX, NOT A CURSOR. `*(u8 **)(p + 0x28 + i * 4)`
   lets loop.c make the giv, whose initialiser does not sit ahead of the
   D_800814A0 hoist in the preheader; a `r = p; ... r += 4;` cursor is a
   source statement that does, and it is 2 differing. */
void func_800B0D34(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    u8 *t;
    u8 *q;
    s32 k;
    s16 m;
    s32 i;

    a = *(u8 **)(p + 0x4);
    t = *(u8 **)(p + 0x14);
    k = *(u16 *)(p + 0x12) & 0x1F;
    m = k;
    if ((*(u16 *)(p + 0x12) & 0x10) != 0)
    {
        m = -k & 0x1F;
        if (m == 0)
        {
            m = 0x10;
        }
    }
    a += 4;
    if (t[0x29] / (t[0x11] / 2 + 2) >= t[0x28])
    {
        a[0] = -0x80 - (m << 3);
        a[2] = 0x10;
        a[1] = 0x10;
    }
    a += 4;
    if (t[0x25] < 0xA)
    {
        a[0] = -0x80 - (m << 3);
        a[2] = 0x10;
        a[1] = 0x10;
    }
    *(s16 *)(p + 0x12) += 1;
    switch (*(s16 *)(p + 0x10))
    {
    case 0:
        if (*(u16 *)(c + 0x18) != 0)
        {
            *(s16 *)(c + 0x18) += 0x80;
        }
        if (*(u8 **)(D_800E3D7C + 0x104) != *(u8 **)(p + 0x14))
        {
            *(s16 *)(p + 0x10) += 1;
        }
        break;
    case 1:
        if (*(s16 *)(c + 0x18) >= -0x3FF)
        {
            *(s16 *)(c + 0x18) = *(u16 *)(c + 0x18) - 0x100;
            break;
        }
        func_8004E994(*(u8 **)(p + 0x5C));
        for (i = 0; i < 4; i++)
        {
            if (*(u8 **)(p + 0x28 + i * 4) != NULL)
            {
                *(u16 *)(*(u8 **)(p + 0x28 + i * 4) - 0x2) |= 0x8000;
                D_800814A0 |= 0x8000;
            }
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        return;
    }
    c[0x4] = 0;
    q = *(u8 **)(t - 0x18);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(q + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(q + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0xA) - (D_800DDC40[t[0x13]] + 0x50);
}

/* The dungeon HUD panel's 0x12-kind node: two number plates from the actor's
   +0x28 and +0x25, a name string picked out of D_800DEFF8, and two bare
   elements parked in the +0x70 slot array. 233 words, 0 differing.

   THE LENGTH CALL IS DUPLICATED INTO BOTH ARMS. Written once after the join
   over a shared `name` local it is 235 words / 226 differing: `i = 4` then
   lives inside the arms, sched1 lifts its `li` above the D_800DEFF8 base's
   last use, `i` loses $s0 to that base and `c` takes an EIGHTH callee-saved
   register -- the two extra words are its save and restore. Duplicated, the
   arms end at the call, cross-jumping merges the tail, and $s0 carries the
   base and then `i` exactly as the target does.

   `c = (u8 *)n + 0x64;` BOUND BEFORE THE `if` is the 233rd word. Spelled
   `*(u32 *)((u8 *)n + 0x64) = 0x808080;` at its use the address folds into
   the store and the function is 232 words; the target computes it ahead of
   the branch and reorg takes it into the delay slot.

   `r[1] |= 2;` GOES AFTER ALL FOUR `q` COLOUR COPIES. Its lbu/ori/sb chain
   outranks the +0x10 copy and is hoisted past it wherever it sits earlier:
   before +0x4 is 236 / 67, before +0xC 234 / 64, before +0x10 233 / 11,
   between +0x10 and +0x14 233 / 8.

   Three narrower spellings, each measured: `*(s8 *)(r + 0x2) = -2` (through
   the `u8 *` the -2 is narrowed to `li 254`), `*(u16 *)` for the 0xFC00 at
   the sprite's +0x18 (`ori`, where `s16` gives `li -1024`), and the paired
   byte and `buf` stores written high offset first. */
UnkNode *func_800B0F50(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    UnkDungeonSprite *s;
    u8 *q;
    u8 *r;
    u8 *t;
    u8 *c;
    s32 w;
    s32 i;
    u8 buf[0x10];

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        m = n->unk_08;
        s->unk_1C = 0x1000;
        s->unk_1E = 0x1000;
        *(u32 *)((u8 *)s + 0xC) = 0x808080;
        *(u16 *)((u8 *)s + 0x18) = 0xFC00;
        t = *(u8 **)(p - 0x18);
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(t + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(t + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(t + 0xA) - (D_800DDC40[p[0x13]] + 0x50);
        n->unk_10 = (u32)func_800B0D34;
        func_8004491C(n, (u32)func_800B06F0);
        q = (u8 *)n + 0x20;
        *(u32 *)(q + 0x14) = (u32)p;
        *(u32 *)((u8 *)n + 0x20) = (u32)((u8 *)n + 0x70);
        *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
        *(u32 *)((u8 *)n + 0x70) = (u32)D_80077FEC;
        *(u32 *)((u8 *)n + 0x58) = 0x808080;
        r = (u8 *)func_800B12F4();
        *(u32 *)(q + 0x28) = (u32)r;
        if (r != NULL)
        {
            buf[2] = 9;
            buf[0] = 9;
            buf[3] = 0x38;
            buf[1] = 0x38;
            func_8004E5A0(p[0x28], 3, &buf[4]);
            *(u32 *)((u8 *)n + 0x74) = (u32)func_8004E298(r, buf, 0);
            func_800B1320(r, 8, (s16)(-*(s8 *)(r + 0x3)));
            func_800B135C(r, 8);
            r += 0x24;
            *(u32 *)((u8 *)n + 0x5C) = 0x808080;
            func_8004E5A0(p[0x25], 3, &buf[4]);
            *(u32 *)((u8 *)n + 0x78) = (u32)func_8004E298(r, buf, 0);
            func_800B1320(r, 8, (s16)(-*(s8 *)(r + 0x3) + 8));
            func_800B135C(r, 8);
            r += 0x24;
            *(u32 *)((u8 *)n + 0x60) = 0x808080;
            c = (u8 *)n + 0x64;
            if ((*(u16 *)(p + 0x46) & 0x8000) != 0)
            {
                *(u32 *)((u8 *)n + 0x7C) =
                    (u32)func_8004DA74(r, D_800DEFF8[*(u16 *)(p + 0x46) & 0x3FFF] + 1, 0);
                i = 4;
                w = strlen(D_800DEFF8[*(u16 *)(p + 0x46) & 0x3FFF] + 1) * 4;
            }
            else
            {
                *(u32 *)((u8 *)n + 0x7C) = (u32)func_8004DA74(r, D_800DEFF8[4] + 1, 0);
                i = 4;
                w = strlen(D_800DEFF8[4] + 1) * 4;
            }
            func_800B1320(r, (s16)(((0x20 - w) >> 1) + 0x80), (s16)(-*(s8 *)(r + 0x3) + 0x11));
            func_800B135C(r, 8);
            r += 0x30;
            *(u32 *)c = 0x808080;
            *(u32 *)(q + i * 4 + 0x50) = (u32)r;
            r[0x1] = 0x38;
            *(s8 *)(r + 0x2) = -2;
            *(s8 *)(r + 0x3) = -2;
            r[0x9] = 0;
            r[0x8] = 0;
            r[0xB] = 0x24;
            r[0xA] = 0x24;
            r[0x0] = 0xC0;
            *(u32 *)(q + 0x18) = 0x202020;
            *(u32 *)(q + 0x1C) = 0x402020;
            *(u32 *)(q + 0x20) = 0x204020;
            *(u32 *)(q + 0x24) = 0x202040;
            *(u32 *)(r + 0x4) = *(u32 *)(q + 0x18);
            *(u32 *)(r + 0xC) = *(u32 *)(q + 0x1C);
            *(u32 *)(r + 0x10) = *(u32 *)(q + 0x20);
            *(u32 *)(r + 0x14) = *(u32 *)(q + 0x24);
            r[0x1] |= 2;
            i++;
            r += 0x18;
            *(u32 *)(q + i * 4 + 0x50) = (u32)r;
            r[0x1] = 0x81;
            *(s16 *)(r + 0x4) = 0x40;
            *(s16 *)(r + 0x6) = 0;
            r[0x0] = 0xC0;
        }
    }
    return n;
}

/* 11 words, 0 differing. */
u8 *func_800B12F4(void)
{
    UnkNode *n;
    u8 *r;

    n = func_8003FC64(0);
    r = NULL;
    if (n != NULL)
    {
        r = (u8 *)n + 0x20;
    }
    return r;
}

/* A POINTER WALK IS WHAT THIS TARGET WANTS, the reverse of func_800A7030's
   note: loop.c turns p[2]/p[3] into one giv biased by +3 and read at -1 and
   0, and the terminator keeps the base. 15 words, 0 differing. */
void func_800B1320(u8 *p, s16 a, s16 b)
{
    while (1)
    {
        p[0x2] += a;
        p[0x3] += b;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* `g = f; f &= 0x4;` -- the hoisted test must land in the PARAMETER's own
   register with the original copied out, which `m = f & 4;` does not do
   (19 words against 20). 20 words, 0 differing. */
void func_800B135C(u8 *p, s32 f)
{
    s32 g;

    g = f;
    f &= 0x4;
    while (1)
    {
        p[0] |= g;
        if (f)
        {
            p[2] = -p[2];
        }
        if ((s8)p[0] < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* Walk a 0xC-stride table terminated by a negative first byte, writing `v`
   into each entry's +0x06.

   THE BACKWARD `goto` IS LOAD-BEARING. A `for (;;)` with a `break` is 9
   words and 8 differing -- loop.c rotates it and emits a preheader jump --
   and tail recursion is 14, because this compiler does not eliminate it
   here. 8 words, 0 differing. */
void func_800B13AC(s8 *p, s16 v)
{
top:
    *(s16 *)(p + 0x6) = v;
    if (*p < 0)
    {
        return;
    }
    p += 0xC;
    goto top;
}

/* OR a mask into every record's +0x4 halfword until the lead byte goes
   negative. The +0x4 cursor and the record base are two induction
   variables, which is the two `addiu 0xC`s. 13 words, 0 differing. */
void func_800B13CC(u8 *p, u16 mask)
{
    while (1)
    {
        *(u16 *)(p + 4) |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* func_800B13CC on the +0x1 byte. 13 words, 0 differing. */
void func_800B1400(u8 *p, u8 mask)
{
    while (1)
    {
        p[1] |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* 20 words, 0 differing. */
u8 *func_800B1434(u8 *d, u8 *s)
{
    while (1)
    {
        *(s32 *)(d + 0x0) = *(s32 *)(s + 0x0);
        *(s32 *)(d + 0x4) = *(s32 *)(s + 0x4);
        *(s32 *)(d + 0x8) = *(s32 *)(s + 0x8);
        d += 0xC;
        if ((s8)s[0] < 0)
        {
            return d;
        }
        s += 0xC;
    }
}

/* 12 words, 0 differing. */
u8 *func_800B1484(u8 *p)
{
    while (*(s8 *)p >= 0)
    {
        p += 0xC;
    }
    return p;
}

/* 6 words, 0 differing. */
s32 func_800B14B4(void)
{
    return *(s32 *)(D_800E3D7C + 0x104) == 0;
}

/* 4 words, 0 differing. */
s32 func_800B14CC(void)
{
    return D_80081485[0] == 0;
}

/* 8 words, 0 differing. */
s32 func_800B14DC(void)
{
    return D_800E3D7C[0x9A] != 0x29;
}

/* The fade driver at a node's +0x10: run the +0x24 probe out of D_800DF030,
   arm state 2 for 4 frames when it fires, then state 0 fades the renderable's
   +0x1E and three colour bytes UP toward 0x1000/0x80 over the +0x22 counter
   and hands on to state 1, while state 2 fades them DOWN and, at zero,
   releases the +0x50 handle list, flags the four +0x28 children and itself.
   155 words, 0 differing.

   TWO LOOP SPELLINGS ARE LOAD-BEARING:
   (1) THE HANDLE LIST TESTS `> 0` FIRST. `for (q = p;; q += 4) { v = ...;
   if (v > 0) call; else if (v == 0) break; }` gives the target's
   `blez`/`jal`/`j` + `bnez` pair; a `while ((v = ...) != 0)` with the `> 0`
   test inside puts the `beqz` first and is three words long (158).
   (2) THE FOUR-SLOT SWEEP IS AN INDEX, `*(u8 **)(p + 0x28 + i * 4)`, not a
   cursor. A `w = p; ... w += 4;` cursor is a source statement and takes the
   callee-saved register the target spends on nothing -- 7 differing, and 2
   with the counter initialised first. Same rule func_800B0D34 records. */
void func_800B14FC(u8 *p, UnkVec *b, UnkSlot *c)
{
    u8 *q;
    u8 *n;
    s32 v;
    s32 i;

    if (D_800DF030[*(s16 *)(p + 0x24)](p, b, c) != 0)
    {
        if (*(s16 *)(p + 0x20) < 2)
        {
            *(s16 *)(p + 0x22) = 4;
            *(s16 *)(p + 0x20) = 2;
        }
    }
    switch (*(s16 *)(p + 0x20))
    {
    case 0:
        if (*(s16 *)(p + 0x26) != 0)
        {
            *(u16 *)(p + 0x26) -= 1;
            break;
        }
        if (*(s16 *)(p + 0x22) == 0)
        {
            break;
        }
        c->unk_1E += (0x1000 - c->unk_1E) / *(s16 *)(p + 0x22);
        c->unk_0C[0] = c->unk_0C[1] = c->unk_0C[2] += (0x80 - c->unk_0C[2]) / *(s16 *)(p + 0x22);
        if (--*(s16 *)(p + 0x22) == 0)
        {
            c->unk_1E = 0x1000;
            *(u32 *)c->unk_0C = 0x808080;
            *(s16 *)(p + 0x20) += 1;
        }
        break;
    case 1:
        break;
    case 2:
        if (*(s16 *)(p + 0x22) != 0)
        {
            c->unk_1E += (-c->unk_1E) / *(s16 *)(p + 0x22);
            c->unk_0C[0] = c->unk_0C[1] = c->unk_0C[2] += (-c->unk_0C[2]) / *(s16 *)(p + 0x22);
        }
        if (--*(s16 *)(p + 0x22) <= 0)
        {
            for (q = p;; q += 4)
            {
                v = *(s32 *)(q + 0x50);
                if (v > 0)
                {
                    func_8004E994(v);
                }
                else if (v == 0)
                {
                    break;
                }
            }
            for (i = 0; i < 4; i++)
            {
                n = *(u8 **)(p + 0x28 + i * 4);
                if (n != NULL)
                {
                    *(u16 *)(n - 0x2) |= 0x8000;
                    D_800814A0 |= 0x8000;
                }
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

/* The dungeon panel's three-chain cursor: a kind-0x12 node whose +0x20 view
   takes the x/y pair, the func_800B14FC step and the func_800B06F0 tag, and
   whose +0x50 array collects the func_800B12F4 chains -- `k' picks a
   D_80073618 prologue chain, `d''s low byte the D_800DF03C label, `e' the
   narrow layout and `f' the +0x26 tag.  234 words, 0 differing.

   NO DEVICES.  The park this closes carried three -- two `do { } while (0);'
   wrappers and a duplicated-arm `if (tmp)' -- and none of them survives: the
   duplicated arm COSTS 7 rows on this body (235/7) and the two wrappers are
   inert (0 rows either way).  Four spellings carry it instead.

   (1) THE SECOND CHAIN'S OLD CURSOR GOES IN THE TEMP, NOT ITS RESULT:
           tmp = u;
           u = func_800B1434(u, u - 0x60);
           func_800B1320(tmp, 1, 1);
       The park had it the other way round -- `tmp = func_800B1434(u, u -
       0x60); func_800B1320(u, 1, 1); u = tmp;' -- and that is 235 words /
       25 rows here and was the whole of its residue for two sessions.  It is
       unreachable, not merely worse.  expand_call emits the return copy
       IMMEDIATELY after the call, so with the RESULT in the temp the order is
       `(set tmp v0)' then `(set a0 u)', both priority 1; `rank_for_schedule'
       breaks that tie on INSN_LUID and schedules the HIGHER LUID first, which
       (sched runs backwards) puts the LOWER-LUID copy FIRST in the program.
       `u' is then still live when `tmp' is born, the two conflict, `tmp'
       takes a callee-saved register and `u = tmp' is the extra word the park
       could never lose.  Putting the OLD cursor in the temp makes that insn
       `(set tmp u)', ahead of the call, which sched1 sinks BELOW the call to
       the target's `addu $a0, $s1, $zero' at word 148, and the result copy is
       then a plain `(set u v0)' -- word 149 -- sharing $s1 with `u'.
       `u = func_800B1434(tmp = u, u - 0x60);' is the same insns and also 0.
   (2) THE TERNARY'S ELSE ARM IS A LOCAL ASSIGNED BEFORE THE TEST:
           arg = base + 0x80;
           func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : arg, ...);
       The target computes `base + 0x80' unconditionally and reorg fills the
       branch's delay slot from the insn PRECEDING it (`addiu $a1, $v1, 0x80',
       word 107), so the then arm falls through into the join with no `j' and
       the load delay after `lhu 0x10($sp)' costs the `nop' at 104.  Inline in
       the ternary (`: (s16)(base + 0x80)') is 236 words / 6 rows.  A full
       `if (e != 0) arg = ...;' STATEMENT is 234 / 19 -- it narrows through
       func_800B1320's `s16' prototype after the join, and its own test keeps
       the `sra' that cse then shares with the three later `e != 0' tests,
       deleting the target's second `lhu'/`nop'/`sll' reload group at 121-123.
       Free: `arg' as `s16' or `u32' (both 0), `: (s16)arg' (0), and
       `e == 0 ? arg : (s16)(...)' (0).  Not free: dropping the THEN arm's
       `(s16)' (2 rows) and `base - w + 0x76' for `base + 0x76 - w' (3).
   (3) FIVE SEPARATE TAIL LOCALS -- base, v, v2, z, z2 -- WITH NO SHARING.
       The park's "one local, two roles" rule is refuted on this body:
       `base' reused as `z' is 233 / 14 and as `v' 234 / 7.  Each
       `x2 = x; if (e != 0) x2 = x - 2;' pair is the target's copy-then-add
       (`addu $v1, $a0, $zero' / `addiu $v1, $v1, -0x2', words 166/167);
       folding it to an in-place `v -= 2' is 233 / 9, and `z -= 2' 233 / 6.
   (4) `u32 w' -- `s32' 234/2, `u16' 235/15, `s16' 236/16, `u8' 233/17 -- and
       `s16 j', not `s32' (230 / 50): `s16 j' is what puts the
       `andi $s7, $s6, 0xFF' at word 52.

   Also measured and inert: `has = i' for `has = 1' (cse folds it back),
   `u32 has', `u32 base' (`s16 base' is 4 rows), and both do-while wrappers. */
void func_800B1768(s16 k, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *q;
    u8 *r;
    u8 *u;
    u8 *tmp;
    u32 *col;
    s32 i;
    s16 j;
    s32 has;
    u32 w;
    s32 base;
    s32 v2;
    s32 z2;
    s32 z;
    s32 v;
    s32 arg;

    i = 0;
    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    if (e != 0)
    {
        *(u16 *)(r + 0x14) |= 1;
    }
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0xC) = y;
    *(s16 *)((u8 *)m + 0xA) = 0;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    j = d & 0xFF;
    *(s16 *)(q + 0x24) = d >> 8;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    col = (u32 *)((u8 *)n + 0x58);
    u = func_800B12F4();
    *(u8 **)(q + 0x28) = u;
    if (u != NULL)
    {
        if (k != 0)
        {
            i = 1;
            col = (u32 *)((u8 *)n + 0x5C);
            *(u32 *)((u8 *)n + 0x70) = (u32)u;
            u = func_800B1434(u, D_80073618[k]);
            has = 1;
            *(u32 *)((u8 *)n + 0x58) = 0x808080;
        }
        else
        {
            has = 0;
        }
        *(s32 *)(q + i * 4 + 0x50) = (s32)func_8004E298(u, D_800DF03C[j], 0);
        i++;
        w = func_80069E98(D_800DF03C[j]) * 8;
        base = has * 10;
        arg = base + 0x80;
        func_800B1320(u, e != 0 ? (s16)(base + 0x76 - w) : arg, (s16)(-*(s8 *)(u + 0x3) - 0x9));
        func_800B135C(u, e != 0 ? 4 : 8);
        func_800B13CC(u, 0x20);
        func_800B1400(u, 2);
        *col++ = 0x808080;
        u += 0x60;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        tmp = u;
        u = func_800B1434(u, u - 0x60);
        func_800B1320(tmp, 1, 1);
        *col = 0x80808;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        i++;
        u[0x1] = 0x38;
        v = has == 0 ? -0x2 : -0xA;
        v2 = v;
        if (e != 0)
        {
            v2 = v - 2;
        }
        *(s8 *)(u + 0x2) = v2;
        *(s8 *)(u + 0x3) = -0xE;
        u[0x9] = 0;
        u[0x8] = 0;
        z = w + has * 24;
        if (has == 0)
        {
            z += 4;
        }
        z2 = z;
        if (e != 0)
        {
            z2 = z - 2;
        }
        u[0xA] = z2;
        u[0xB] = 0x10;
        u[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(u + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(u + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(u + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(u + 0x14) = *(s32 *)(q + 0x1C);
        u[0x1] |= 2;
        u += 0x18;
        *(s32 *)(q + i * 4 + 0x50) = (s32)u;
        u[0x1] = 0x81;
        *(s16 *)(u + 0x4) = 0x20;
        *(s16 *)(u + 0x6) = 0;
        u[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}

/* The dungeon's message panel: a 0x12-kind node whose +0x10 step is
   func_800B14FC, three func_800B12F4 sprite chains (the frame, the
   func_8004DA74 text laid out from func_800990FC's handle, and a two-piece
   cursor), and the five chain heads parked in the +0x50 array off the node.

   THE PARAMETERS ARE ALL `s16`. The fourth arrives as `d >> 8`
   (`sll 16`/`sra 24`), the fifth is a stack word tested twice as a halfword
   -- the two tests are why it is COPIED into $fp at entry -- and the sixth
   is spilled to 0x10($sp) because every callee-saved register is already
   taken.

   FOUR SPELLINGS THAT EACH COST WORDS, all measured:
   (1) func_800B1320's SECOND AND THIRD PARAMETERS HAD TO BE WIDENED from
       `u8` to `s16` (its definition below). This call site needs
       `sra 16`/`sra 24` where the `u8` prototype narrows to `andi 0xFF`,
       and it folds `(s16)(0x89 - len)` into `li -119`. func_800B1320 itself
       and its other caller func_800A6C00 both re-measure at 0 differing
       after the widening.
   (2) The five slots at +0x50 off the NODE are written off the node
       (`(u8 *)n + 0x70`, `+0x74`, `+0x58`), not off the +0x20 view, exactly
       as the landed func_800A6C00 writes its own.
   (3) ONE cursor local for the second and third func_800B12F4 chains. As a
       separate local the second chain lands in $a1 and the whole
       n/view/cursor trio rotates through $s1/$s2/$s3: 80 differing with
       every other word already exact.
   (4) `t[1] |= 2` goes AFTER all four +0x10..+0x1C copies. Written between
       the third and fourth copy it is 8 differing, before the first 223
       words; the `|=` reads back a byte of the record the copies write, so
       its position is the block's only scheduling freedom.
   The two negative bytes must be stored through `s8 *`: as `t[2] = -0xA` on
   a `u8 *` the constant folds to `li 246`.
   220 words, 0 differing. */
void func_800B1B10(u8 *p, s16 x, s16 y, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    u8 *u;
    u8 *t;
    u8 *s;
    u32 *col;
    s32 k;
    s32 len;
    void *h;

    n = func_8003FD64(0x12, D_80083498);
    if (n == NULL)
    {
        return;
    }
    q = (u8 *)n + 0x20;
    r = (u8 *)n->unk_0C;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = x;
    *(s16 *)((u8 *)m + 0x6) = y;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(r + 0x1C) = 0x1000;
    *(s32 *)(r + 0xC) = 0;
    n->unk_10 = (u32)func_800B14FC;
    func_8004491C(n, (u32)func_800B06F0);
    *(s16 *)(q + 0x24) = d >> 8;
    *(s16 *)(q + 0xA) = x;
    *(s16 *)(q + 0xC) = y;
    *(u32 *)q = (u32)((u8 *)n + 0x70);
    *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0x58);
    if (func_8003FA44(3) != 0)
    {
        col = (u32 *)((u8 *)n + 0x5C);
        k = 2;
        u = func_800B12F4();
        *(u8 **)(q + 0x28) = u;
        t = u;
        *(u32 *)((u8 *)n + 0x70) = (u32)t;
        t = func_800B1434(t, (u8 *)func_8004A658(p[1], p[0]));
        func_800B13CC(u, 0x20);
        func_800B1400(u, k);
        *(u32 *)((u8 *)n + 0x58) = 0x808080;
        h = func_800990FC();
        *(u8 *)func_80099368(p, h) = 0;
        s = (u8 *)func_8004DA74(t, h, 0);
        *(s32 *)((u8 *)n + 0x74) = (s32)s & 0x7FFFFFFF;
        len = strlen(h) * 4;
        func_800B1320(t, e != 0 ? (s16)(0x89 - len) : 0x8A, (s16)(-*(s8 *)(t + 0x3) - 0xE));
        func_800B135C(t, e != 0 ? 4 : 8);
        func_800B13CC(t, 0x20);
        func_800B1400(t, 2);
        *col++ = 0x808080;
        strlen(h);
        t = func_800B12F4();
        *(u8 **)(q + 0x2C) = t;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t & 0x7FFFFFFF;
        k++;
        func_800B1434(t, s);
        func_800B1320(s, 1, 1);
        t = func_800B12F4();
        *(u8 **)(q + 0x30) = t;
        *col = 0x80808;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t;
        t[0x1] = 0x38;
        *(s8 *)(t + 0x2) = -0xA;
        *(s8 *)(t + 0x3) = -0x10;
        t[0xA] = len + 0x18;
        t[0x9] = 0;
        t[0x8] = 0;
        t[0xB] = 0x14;
        t[0x0] = 0xC0;
        *(s32 *)(q + 0x10) = 0;
        *(s32 *)(q + 0x14) = 0;
        *(s32 *)(q + 0x18) = 0x84008;
        *(s32 *)(q + 0x1C) = 0x84008;
        *(s32 *)(t + 0x4) = *(s32 *)(q + 0x10);
        *(s32 *)(t + 0xC) = *(s32 *)(q + 0x14);
        *(s32 *)(t + 0x10) = *(s32 *)(q + 0x18);
        *(s32 *)(t + 0x14) = *(s32 *)(q + 0x1C);
        t[0x1] |= 2;
        k++;
        t += 0x18;
        *(s32 *)(q + k * 4 + 0x50) = (s32)t;
        t[0x1] = 0x81;
        *(s16 *)(t + 0x4) = 0x20;
        *(s16 *)(t + 0x6) = 0;
        t[0x0] = 0xC0;
    }
    *(s16 *)(q + 0x22) = 4;
    *(s16 *)(q + 0x26) = f;
}

/* A node +0x10 step: every fourth frame advance the +0x2 cursor by 8, wrap it
   at 0x60, and play the row it now points at. The field is read BACK for the
   call because the wrap store sits between. 32 words, 0 differing. */
void func_800B1E80(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s16 u;

    t = *(u16 *)p - 1;
    *(s16 *)p = t;
    if (t <= 0)
    {
        *(s16 *)p = 4;
        u = *(u16 *)(p + 0x2) + 8;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x60)
        {
            *(s16 *)(p + 0x2) = 0;
        }
        func_8003F80C((u32)&D_800DF068[*(s16 *)(p + 0x2)], 0x7340, 1, 2);
    }
}

/* 13 words, 0 differing. */
void func_800B1F00(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B1E80;
    }
}

/* The node's +0x10 step for func_800B2074's marker: a four-state machine on
   the view's +0x04. TWO FALL-THROUGHS ARE REAL -- state 0 falls into state 1,
   and state 2 falls into state 3. Four cases plus a default is under
   CASE_VALUES_THRESHOLD, so this is a comparison tree and not a jump table.
   80 words, 0 differing. */
void func_800B1F34(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(p + 0x6) = 0x10;
        *(s16 *)(p + 0x4) += 1;
    case 1:
        b->unk_14 -= 0x4000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x40;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 2:
        b->unk_14 += 0x6000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
    case 3:
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            b->unk_14 = 0;
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    }
    b->unk_08 -= b->unk_14;
    *(u32 *)(D_800814A8 + 0x1C) |= 0x40000000;
    **(s16 **)p = 0x20 - *(u16 *)((u8 *)b + 0xA);
}

/* Spawn the marker node.

   THE TWO COORDINATES ARE `s32`, not s16. The cell index is written
   `(x << 10) >> 16`; with s16 parameters gcc folds the entry sign-extension
   into the pair and emits `sll 16`/`sra 22` instead of the target's
   `sll 10`/`sra 16` -- 7 differing, with no other difference.

   THE VIEW POINTER MUST BE BOUND BEFORE ITS USE, not at it. Assigned on the
   statement that uses it, `(u8 *)n + 0x20` folds into the store's
   displacement (3 differing); bound at the top of the `if` it survives as the
   target's own `addiu $v0, $s1, 0x20`, even though update_equiv_regs still
   moves the set down to the use. 81 words, 0 differing. */
void func_800B2074(s32 x, s32 y)
{
    UnkNode *n;
    u8 *g;
    u8 *r;
    u8 *v;
    UnkVec *m;
    UnkSlot *s;

    g = (u8 *)&D_80083160;
    r = *(u8 **)(g + 0x1DC);
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B1F34;
        func_8004491C(n, (u32)D_80046398);
        s = (UnkSlot *)n->unk_0C;
        s->unk_20 = 0x1000;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        s->unk_08 = 2;
        *(s32 *)((u8 *)s + 0xC) = 0x808080;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x16) = 6;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        r += (((x << 10) >> 16) + (((y << 10) >> 16) << *(s16 *)(g + 0x1F0))) * 6;
        *(u8 **)((u8 *)n + 0x20) = r + 2;
        *(s16 *)r = 3;
        *(s16 *)((u8 *)m + 0xA) = -*(u16 *)(*(u8 **)((u8 *)n + 0x20));
        *(u16 *)(r + 0x2) += 0x20;
        *(u16 *)(r + 0x4) |= 1;
        func_800A56E0(0x603);
        *(s16 *)(v + 0xE) = *(u16 *)(D_800814A8 + 0x2A);
    }
}

/* A node +0x10 step: lift the mover 0x8000 a frame, re-register the
   renderable, and mark the node dead when the +0x6 countdown goes negative.
   30 words, 0 differing. */
void func_800B21B8(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    b->unk_08 += 0x8000;
    func_800478B8(c);
    t = *(u16 *)(p + 0x6) - 1;
    *(s16 *)(p + 0x6) = t;
    if (t < 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 48 words, 0 differing. The whole-UnkVec assignment is the 24-byte block
   move: four `lw`/`sw` pairs then two. */
void func_800B2230(UnkVec *src)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B21B8;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *m = *src;
        r = (UnkSlot *)n->unk_0C;
        *(u32 *)r->unk_0C = 0x808080;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_8003DB94(r, D_800DEC70, 0);
        *(s16 *)((u8 *)n + 0x26) = 0x20;
    }
}

/* The spawner tail func_800D1C70 shares: stamp the record's room id, place
   the mover, measure the leash into the view's +0x88, dress the renderer,
   light the node up and hang a 0x110 marker node off it. FOUR PARAMETERS --
   the fourth is never read here, but func_800B23F8 sign-extends $a3 into
   the `jal`'s delay slot. 66 words, 0 differing, first compile. */
void func_800B22F0(UnkNode *n, UnkVec *m, UnkSlot *r, s16 d)
{
    u8 *p;

    p = (u8 *)n + 0x20;
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s16 *)(p + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                       *(u16 *)((u8 *)m + 0xA) - 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    r->unk_14 |= 0x8000;
    func_80099FDC(n);
    func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                  (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0xAD] = 1;
    p[0x71] = 0;
    *(u32 *)(p + 0x1C) |= 0x40000000;
    func_800BC26C(n, 0, (s32)((u8 *)r + 0x2C), (s32)((u8 *)n + 0x4A));
}

/* The 0x112 spawner: allocate a node, install func_800B274C as its handler
   and func_800B2A60 as the payload's +0x8C step, return the payload or NULL.
   81 words, 0 differing.

   THE PARAMETER WIDTHS ARE THE WHOLE FUNCTION.  `b' and `c' are stored with
   `sb' but must be DECLARED ONE STEP WIDER, as u16.  A sub-word declared type
   makes assign_parms emit a raw SImode copy plus a separate truncation (and
   function.c:4093 flushes every truncation after every parameter has left its
   hard register); when the only use is NARROWER than the declared type, cse
   cannot fold the truncation away, so it survives, combine folds the raw copy
   into it, and the raw copy at the parameter's declaration position becomes a
   NOTE_INSN_DELETED.  sched1's parameter-pinning prefix loop (sched.c:3247)
   requires GET_CODE (head) == INSN, so it BREAKS AT THAT NOTE and pins only
   the first parameter -- `b', `c' and `d' are then schedulable and sched1's
   birthing-insn rule sinks them into the entry block's call setup, which is
   the listing's interleaved `move $sN,$aN' order.  Measured on this body:
   `u8 b, u8 c' 34 rows, `u16 b, u8 c' 5, `u8 b, u16 c' 25, `u32 b' 29,
   `u32 c' 11.  Signedness is free (all four s16/u16 combinations are 0) and
   the width is exact: same-width keeps the raw copy pinned, and u32/s32 emits
   no truncation at all and also keeps it pinned.  See
   docs/matching-levers.md, "A SUB-WORD parameter type is how a parameter copy
   becomes schedulable".

   DO NOT PUT THIS PROTOTYPE IN csrc/ovl_dungeon_decls.h.  Nothing calls this
   function from C today, and a narrow prototype in scope rewrites a future
   caller's `sll'/`sra' into a truncation.  The definition's own parameter list
   is all the landing needs.

   `w' is a SECOND payload pointer and it is load-bearing: the late stores go
   through $s3, the register `c' dies in.  Writing them through `p' is 80 words
   and 31 rows.  `w = p;' and `w = (u8 *)n + 0x20;' are byte-identical.

   `*(s8 *)(w + 0x9C) = -1;' and NOT `w[0x9C] = 0xFF;' -- through a u8 lvalue
   gcc folds -1 to 0xFF, cse shares it with the neighbouring 0x9A store and the
   `addiu $v0, $zero, -0x1' disappears: 80 words, 22 rows.

   NO DEVICES.  The park carried two -- an `__asm__("" ::"r"(n));' and a
   `c = (s32)p;' merge that made the parameter double as the second payload
   pointer -- and both are unnecessary once the widths are right. */
/* func_800B274C is defined below this use and its address is taken, so it
   needs a declaration here; across the parameter list it is not implicit. */
void func_800B274C(u8 *, UnkVec *, u8 *);
u8 *func_800B23F8(s16 a, u16 b, u16 c, s16 d, s32 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *p;
    u8 *w;
    s32 k;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        w = p;
        n->unk_10 = (u32)func_800B274C;
        p[0x13] = 0x1E;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = d;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x25) = c;
        *((u8 *)r + 0x24) = b;
        k = a & 3;
        if (k == 1)
        {
            *(u32 *)(p + 0x14) |= 0x6000;
            *(u32 *)(p + 0x1C) |= 0x6000;
        }
        else if (k >= 2)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_800B22F0(n, m, r, a);
        w[0x9A] = 0xFF;
        *(s8 *)(w + 0x9C) = -1;
        *(u32 *)(w + 0x8C) = (u32)func_800B2A60;
        *(s32 *)((u8 *)r + 0x8) = e;
    }
    return p;
}

/* The +0x8C handler's wake-up gate: if the record is flagged busy or
   queued, and the actor is not in state 0x13/0x14, hand it to one of two
   continuations or drop it back to a plain state. The D_80083460 block is the
   engine's single "current record" slot: when D_800DCE68 is set this function
   claims that slot for `d` (only while it is empty, its +0x10 partner is
   clear, +0x08 is zero, +0x02 has neither of 0x2008 set and d[0x43] is 0xFD);
   when it is clear, func_800A2BDC decides instead.

   THE WHOLE RESIDUE WAS ONE REGISTER, AND THE CURE IS TO REUSE THE TEST'S OWN
   TEMPORARY AS THE VALUE. Written as

       if ((t & 0x4000) == 0) { v = 2; } else { v = 1; }

   the function is 132 words and 3 differing: `jump.c:789` rewrites it to
   `v = 1; if (...) v = 2;` and hoists the `v = 1` above the compare, so `v` is
   live across the `andi` whose result holds $v0 and global-alloc gives `v`
   $a0 instead. Writing

       v = t & 0x4000;
       if (v == 0) { v = 2; } else { v = 1; }

   makes the mask, the branch operand and the stored value ONE pseudo, which
   both blocks the hoist (`reg_referenced_between_p` sees `v` in the test) and
   forces the shared $v0. Also measured at 3 differing, all on the same word
   triple: `v` as u8/s16/u32/s32, declared first or last or block-scoped, `v`
   reused from `t`, the arms written in the other order, a do/while(0) wrapper,
   and a dead second assignment in each arm. `v` reused from `f` is 31, the
   0x14 clear moved below the if/else is 12, and storing the field in both
   arms instead of a shared `v` is 134 words / 44.

   A second spelling fact, worth 2 words: the two arms must share the `sb`.
   `p->unk_9A = (t & 0x4000) ? 1 : 2;` and a store per arm both give 134 words.

   132 words, 0 differing. */
void func_800B253C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *cur;
    u32 f;
    u32 t;
    s32 v;

    if ((*(u32 *)(d + 0x1C) & 0x2000) == 0 && (*(u32 *)(d + 0x14) & 0x4000) == 0)
    {
        return;
    }
    if (p->unk_9A == 0x13 || p->unk_9A == 0x14)
    {
        return;
    }
    if (*(s8 *)(d + 0x6D) != 0)
    {
        return;
    }
    if (D_800DCE68 == 0)
    {
        if ((s16)func_800A2BDC(d) != 0)
        {
            return;
        }
    }
    else
    {
        g = (u8 *)&D_80083460;
        cur = *(u8 **)(g + 0xC);
        if (cur != d)
        {
            if (*(u32 *)(g + 0x10) != 0)
            {
                return;
            }
            if (cur != NULL)
            {
                return;
            }
            if (*(s16 *)(g + 0x8) != 0)
            {
                return;
            }
            if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
            {
                return;
            }
            if (d[0x43] != 0xFD)
            {
                return;
            }
            *(u8 **)(g + 0xC) = d;
        }
        if (d[0x43] != 0xFD)
        {
            return;
        }
    }
    f = *(u32 *)(d + 0x1C);
    if ((f & 0x400000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x400000;
        t = *(u32 *)(d + 0x14);
        if ((t & 0x20000000) != 0)
        {
            *(u32 *)(d + 0x14) = t | 0x400000;
            *((u8 *)p + 0xAD) = 0;
            func_800ACB98((u8 *)p, b, c, d);
            return;
        }
        *(u32 *)(d + 0x14) = t & ~0x400000;
        v = t & 0x4000;
        if (v == 0)
        {
            v = 2;
        }
        else
        {
            v = 1;
        }
        p->unk_9A = v;
    }
    else if ((f & 0x2000000) != 0)
    {
        p->unk_8C = NULL;
        *(u32 *)(d + 0x1C) &= ~0x2000000;
        d[0x71] = 0;
        if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
        {
            func_800ACD74((u8 *)p, b, c, d);
            return;
        }
        p->unk_9A = 0;
    }
    else
    {
        return;
    }
    p->unk_9B = 0;
}

/* The dungeon actor's per-frame driver: past the engine's 0x2000 gate it runs
   func_800B253C, then the record's own +0x8C continuation, then the +0x9A
   entry of the D_800DF1E8 table, re-aims the tile when the +0x6D facing moved
   under it, steps the mover by its velocity, runs the +0x9D bob counter and
   finally leashes +0x92/+0x88 to the ground height func_800BCB04 reports.

   THE RECORD IS ITS OWN ACTOR: all four four-argument calls pass $a0 as both
   the first and the fourth argument. AND IT IS READ THROUGH TWO ALIASES --
   `q` carries +0x88 and +0x1C, `p` carries everything else INCLUDING the
   +0x1C that the func_800A020C argument reads, which is the same
   one-field-two-spellings split func_800B2A60 records for D_80083462.
   Dropping `q` and spelling every access through `p` is 194 words / 192
   differing: the `move $s1, $s0` and its two save/restore words go, and the
   body re-registers wholesale.

   `*(u32 *)(q + 0x1C) &= 0xF7FFFFFF;` is written out in all three arms that
   need it; cross-jumping merges the two that fall into the tail and leaves
   the third sharing only the `sw` at word 146.

   197 words, 0 differing, first compile. */
void func_800B274C(u8 *p, UnkVec *b, u8 *c)
{
    void (*h)(u8 *, UnkVec *, u8 *, u8 *);
    u8 *q;
    s16 dir;
    s32 r;

    q = p;
    if ((D_80083462 & 0x2000) != 0)
    {
        p[0x71] &= 0x7F;
        return;
    }
    dir = (s8)p[0x6D];
    *(u16 *)(p + 0x98) &= 0xFBFF;
    if ((*(u32 *)(p + 0x14) & 0x100000) != 0)
    {
        return;
    }
    func_800B253C((UnkDungeonActor *)p, b, c, p);
    h = *(void (**)(u8 *, UnkVec *, u8 *, u8 *))(p + 0x8C);
    if (h != NULL)
    {
        h(p, b, c, p);
    }
    D_800DF1E8[p[0x9A]](p, b, c, p);
    if (dir != *(s8 *)(p + 0x6D))
    {
        func_800AA36C(p, b, c, p);
    }
    if ((*(u16 *)(c + 0x14) & 0x8000) == 0)
    {
        func_800A020C(*(s32 *)(p + 0x1C), c + 0xC);
    }
    *(u16 *)(c + 0x14) |= 0x7000;
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if ((*(u16 *)(p + 0x98) & 0x8) != 0)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if ((*(u16 *)(p + 0x98) & 0x4) == 0)
    {
        r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                          *(u16 *)(q + 0x88) - 0x20);
        if ((s16)r < 0x200)
        {
            if (*(s16 *)(p + 0x92) + *(s16 *)(q + 0x88) < (s16)r)
            {
                *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
            }
            else
            {
                if ((s16)r >= *(s16 *)(q + 0x88))
                {
                    *(s32 *)(p + 0x90) = 0;
                }
                else
                {
                    *(s16 *)(p + 0x92) = r - *(u16 *)(q + 0x88);
                }
                p[0x9D] = 0;
                b->unk_14 = 0;
                *(u32 *)(q + 0x1C) |= 0x8000000;
            }
            if ((*(u32 *)(q + 0x1C) & 0x40000000) != 0)
            {
                *(u32 *)(q + 0x1C) &= 0xBFFFFFFF;
                r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                                  *(u16 *)(q + 0x88) - 0x20);
                *(u16 *)(p + 0x92) += *(u16 *)(q + 0x88) - r;
                *(u16 *)(q + 0x88) = r;
            }
        }
        else
        {
            *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
        }
    }
    else
    {
        *(u32 *)(q + 0x1C) &= 0xF7FFFFFF;
    }
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(q + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
}

/* State 0xE's gate chain, the func_800C9F34 (:18679) shape one record over:
   the D_80083460 +0x2 flags, the view's +0x25 and func_80042900, then --
   only when the +0x2000 draw flag is clear -- the +0x1C bits, the state
   byte, the +0x98 mask, the +0x64 probe and the +0x80000 hand-off; the tail
   re-aims the tile and, past the +0x6D/D_80083462/func_800A2C34 gates, asks
   func_800A03C4 whether to tear down or clear +0x6D.

   D_80083460 + 0x2 IS READ TWICE THROUGH THE POINTER LOCAL AND ONCE AS THE
   DECLARED `D_80083462`, which is the three-spelling split func_800C9F34
   records -- the declared scalar is the `lui`/`lhu` pair with its own `%hi`
   at word 95. `p[0xAD] = 0; func_800ACB98(p, b, c, d);` is written out at
   all four exits: cross-jumping merges three of them onto the copy at word
   112 and leaves the `d[0x25] == 0` one its own two words, because there the
   incoming $a0..$a3 are still the call's own arguments and it needs no setup.
   130 words, 0 differing, first compile. */
void func_800B2A60(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x1000) != 0)
    {
        return;
    }
    if (d[0x25] == 0)
    {
        p[0xAD] = 0;
        func_800ACB98(p, b, c, d);
        return;
    }
    if ((s16)func_80042900((s8 *)d, 1) != 0)
    {
        p[0xAD] = 0;
        func_800ACB98(p, b, c, d);
        return;
    }
    if ((*(u16 *)(g + 0x2) & 0x2000) == 0)
    {
        if ((*(s32 *)(d + 0x1C) & 0x100) != 0)
        {
            func_800AA258(p, b, c, d);
            return;
        }
        if (p[0x9A] != 0xE)
        {
            p[0x9A] = 0xE;
        }
        *(u16 *)(p + 0x98) &= 0xFFF3;
        if (*(s16 *)(d + 0x64) != 0)
        {
            if (func_800AA6B4(p, b, c, NULL) != 0)
            {
                return;
            }
        }
        if ((*(s32 *)(d + 0x1C) & 0x80000) != 0)
        {
            func_800AA888(p, b, c, d);
            func_800B318C(p, b, c, d);
            return;
        }
        if ((s16)func_800A1C58(d) != 0)
        {
            p[0xAD] = 0;
            func_800ACB98(p, b, c, d);
            return;
        }
    }
    c[0x26] = func_8009FB34(c[0x24], c[0x25]);
    if (*(s8 *)(d + 0x6D) > 0)
    {
        if ((D_80083462 & 0x1000) == 0)
        {
            if ((s16)func_800A2C34(NULL) == 0)
            {
                if (func_800A03C4(d, c[0x24], c[0x25]) != NULL)
                {
                    p[0xAD] = 0;
                    func_800ACB98(p, b, c, d);
                }
                else
                {
                    d[0x6D] = 0;
                }
            }
        }
    }
}

/* Ten one-gate installers: run the predicate, and on a pass park the next
   handler in the record's +0x8C slot. csrc/ovl_town.c's func_800A4F4C is the
   same source with a different record offset and predicate. 14 words each,
   0 differing. */
void func_800B2C68(UnkDungeonActor *p)
{
    if (func_800AB378() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* State 1's three-step lunge, switched on the actor's +0x9B. Step 0 faces the
   record with func_800AD4D0 and seeds the mover's velocity from the facing
   tables, then either hands straight to step 2, tears the actor down, or arms
   the +0x96 countdown; step 1 bleeds a quarter off each axis per frame and
   runs the countdown out; step 2 eases the mover onto c[0x24]/c[0x25] over
   what is left of the countdown and parks func_800B2A60 in the +0x8C slot.

   CASE 0 FALLS THROUGH INTO CASE 1 -- the `sh $v1, 0x96($s1)` at word 43 is
   the last insn before .L800B2DB8, which is also case 1's switch arm. The
   gcc switch tree for cases 0/1/2 is the balanced one: `beq 1` first, then
   `slti 2` splitting 0 from 2.

   THE D_80083460 TEARDOWN IS WRITTEN OUT TWICE AND CROSS-JUMPING MERGES IT.
   Case 0's `d[0x28] == 0` exit and case 1's countdown-expired exit are the
   same seventeen words ending in the same `j`, so they fold onto one copy at
   word 100; writing it once behind a label is not expressible and writing it
   as a helper would cost the call. Each copy gets ITS OWN `u8 *` local, the
   usual rule for this base in this unit.

   The second `if` is inverted against the first. Case 0 writes
   `if (d[0x28] != 0) { +0x9B = 2; return; }` and gets `beqz` past the then-
   block; case 1 writes `if (d[0x28] == 0) { <teardown> return; }` so the
   `+0x96 = 8` tail sits AFTER the if and the `bnez` reaches it with the 8
   already in its delay slot.

   `p->unk_96 = -1; if (d+0x1C & 0x228) p->unk_96 = 8;` written as a shared
   local assigned -1 FIRST is the jump.c:789 shape the target has: the -1
   rides the `beqz` delay slot and both arms share the one `sh`.

   The two facing-table reads are spelled out in full rather than through a
   shared index: the `b->unk_0C` store between them is a QImode-free MEM
   store that invalidates cse's memory table, so the target reloads
   `*(u16 *)(d + 0x6A)` and recomputes the index, which is exactly what two
   written-out copies give. 195 words, 0 differing, first compile. */
void func_800B2CA0(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    u8 *h;
    u8 *k;
    s32 t;
    s32 x;
    s32 y;

    switch (p->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = -D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = -D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        p->unk_9B++;
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            if (d[0x28] != 0)
            {
                p->unk_9B = 2;
                return;
            }
            g = (u8 *)&D_80083460;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
            }
            *(s16 *)(g + 0xA) = *(u16 *)(g + 0xA) + 1;
            func_8009A028(d);
            *(u16 *)((u8 *)p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        t = -1;
        if ((*(s32 *)(d + 0x1C) & 0x228) != 0)
        {
            t = 8;
        }
        p->unk_96 = t;
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (p->unk_96 > 0)
        {
            p->unk_96--;
        }
        else if ((*(u16 *)(c + 0x14) & 0x6000) != 0)
        {
            p->unk_96 = 0;
        }
        if (p->unk_96 != 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
            h = (u8 *)&D_80083460;
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            if (*(s32 *)(h + 0x10) == (s32)(d - 0x20))
            {
                *(s32 *)(h + 0x10) &= 0x7FFFFFFF;
            }
            *(s16 *)(h + 0xA) = *(u16 *)(h + 0xA) + 1;
            func_8009A028(d);
            *(u16 *)((u8 *)p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
        p->unk_96 = 8;
        p->unk_9B++;
        break;
    case 2:
        if (p->unk_96 != 0)
        {
            b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 15) / p->unk_96;
            b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 15) / p->unk_96;
        }
        p->unk_96--;
        if (p->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        k = (u8 *)&D_80083460;
        if (*(s32 *)(k + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(k + 0x10) &= 0x7FFFFFFF;
        }
        p->unk_8C = func_800B2A60;
        break;
    }
}

/* func_800B30E4 behind the renderable's 0xE000 gate and with func_800A2DB8
   ahead of the teardown. 48 words, 0 differing. */
void func_800B2FAC(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    if (*(u16 *)(c + 0x14) & 0xE000)
    {
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A2DB8(d);
        func_800A32A4(d);
        func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Six forwarders, one call each and nothing else. 8 words each,
   0 differing. */
void func_800B306C(void)
{
    func_800AB538();
}

void func_800B308C(void)
{
    func_800AB778();
}

void func_800B30AC(UnkDungeonActor *p)
{
    if (func_800ABDBC() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* The +0x8C step that tears an actor down. THE 0x300 / 0x3000 CHOICE IS A
   TERNARY IN THE CALL, not an if/else around two calls: the target sets
   $a0/$a1 above the branch and only $a2 inside it. 42 words, 0 differing. */
void func_800B30E4(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
    {
        *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
    }
    func_800A32A4(d);
    func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    func_8009A028(d);
    *(u16 *)(d - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_800B318C(void)
{
    func_800AC82C();
}

void func_800B31AC(UnkDungeonActor *p)
{
    if (func_800AC010() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* FOUR PARAMETERS, forwarded. func_800AC480 reads all four argument
   registers; this caller only passes them through, so the wrong arity was
   invisible until that function was read. 14 words, 0 differing either way. */
void func_800B31E4(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AC480((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

void func_800B321C(UnkDungeonActor *p)
{
    if (func_800AC5C8() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* FOUR PARAMETERS, forwarded, exactly as the func_800AC480 pair next door:
   func_800ACE34 reads $a0 and $a2 and this caller only passes them through.
   14 words, 0 differing either way. */
void func_800B3254(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800ACE34((u8 *)p, b, c, d) != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

/* The kind-0x210 node's +0x10 step func_800B341C parks: project the parent
   slot into `out`, move the mover onto the parent mover's position plus that
   offset, and -- once the +0x8C latch is set -- spawn a func_800B8EA8 trail
   from the OLD position to the new one. 100 words, 0 differing, first
   compile.

   THE OLD POSITION IS THREE `s16` LOCALS, read before the three updates
   overwrite the same halfwords. Each one costs an `lhu` plus its own
   `sll 16`/`sra 16` at the int use, which is what an `s16` local costs
   here, and the loads sit above the stores because the source reads them
   first.

   The two stores per axis are a CHAINED assignment: the mover's field is
   the inner store and the `u16 trail[]` slot the outer, which is the order
   the listing emits them in.

   The dead-node tail is reached from TWO places -- the `r == NULL` arm with
   the latch set, and the +0x8E countdown going negative -- and the `bnez`
   at 0x800B33C0 branches straight into it, so it is one `goto` label and
   not a duplicated arm. */
void func_800B328C(u8 *q, UnkVec *b, u32 c)
{
    UnkSlot *s;
    u8 *r;
    u8 *v;
    s16 x;
    s16 y;
    s16 z;
    s16 t;
    s16 out[4];
    u16 trail[4];

    s = (UnkSlot *)(*(UnkNode **)q)->unk_0C;
    r = (u8 *)func_8003DE58((u8 *)s->unk_08, s, out, 0);
    if (r != NULL)
    {
        v = (u8 *)(*(UnkNode **)q)->unk_08;
        if (*(s16 *)(q + 0x8C) != 0)
        {
            x = *(s16 *)((u8 *)b + 0x2);
            y = *(s16 *)((u8 *)b + 0x6);
            z = *(s16 *)((u8 *)b + 0xA);
            trail[0] = *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2) + out[0];
            trail[1] = *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6) + out[1];
            trail[2] = *(s16 *)((u8 *)b + 0xA) = *(u16 *)(v + 0xA) + out[2];
            func_800B8EA8(*(void **)q, x, y, z, trail, *(void **)(q + 0x4), 2, 0x800, 1);
        }
        else
        {
            *(s16 *)((u8 *)b + 0x2) = *(u16 *)(v + 0x2) + out[0];
            *(s16 *)((u8 *)b + 0x6) = *(u16 *)(v + 0x6) + out[1];
            *(s16 *)((u8 *)b + 0xA) = *(u16 *)(v + 0xA) + out[2];
            *(s16 *)(q + 0x8C) = 1;
        }
    }
    else if (*(s16 *)(q + 0x8C) != 0)
    {
        goto dead;
    }
    t = *(u16 *)(q + 0x8E) - 1;
    *(s16 *)(q + 0x8E) = t;
    if (t < 0)
    {
    dead:
        *(u16 *)(q - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Allocate a kind-0x210 node under the caller's record, park func_800B328C at
   its +0x10 and write the two pointers and a halfword into the payload.
   `*(void **)q = a` comes back as `sw $s1, 0x20($v1)` off the NODE while
   `*(void **)(q + 4)` keeps the `$v0` base -- cse rewrites `*q` into
   `*(p + C)` and leaves `*(q + 4)` alone. 28 words, 0 differing. */
void func_800B341C(u8 *a, void *b, s16 c)
{
    UnkNode *n;
    u8 *q;

    if (b != NULL)
    {
        n = func_8003FD64(0x210, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800B328C;
            q = (u8 *)n + 0x20;
            *(void **)q = a;
            *(void **)(q + 0x4) = b;
            *(s16 *)(q + 0x8E) = c;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089118);

/* The torch/lantern cast's state machine over the actor's +0x9B, sixteen
   entries in jtbl_800891C0 with 11..14 falling to the default. States 0..2
   open the effect (D_800DD030/D_800DD068 skins, func_800C77D0 on the
   record's -0x20), 3..6 run the knock-back -- the eight-way D_8006CCD8/E8
   step into the mover, the saved +0x14 fall rate parked in D_800DF244 and
   func_800B341C/func_800B3D10 on the way out -- 7..9 close on
   func_8009C12C's message node, and 10 hands the actor back to
   func_8008ACDC. The table is at rodata offset 0xA60, 0 mod 8, so cc1psx's
   `.align 3` is a no-op: NOT one of the seven at 4 mod 8.
   545 words, 0 differing.

   THREE LEVERS, and the function went 106 -> 39 -> 18 -> 4 -> 0 aligned
   differences on them, with no register work at all.

   (1) ONE POINTER LOCAL PER ARM, func_800974DC's rule, and it applies to
   the SCRATCH INTEGERS too. One shared `t` over the ten skin-table arms is
   a global allocno and comes out in $a3 where the target has $a1 in every
   arm: 106 differences. Ten per-arm locals: 39. The same rule on the ONE
   eight-way index shared by case 3 and case 6 -- `k` and `k2`, each
   written once -- is worth another FIVE (23 -> 18), because a local
   written in two arms is never deferred by sched1 (:26241) and its `lhu`
   outranks the table's `high` in the ready list. With the index split, the
   `lui %hi(D_8006CCD8)` lands ahead of the `lhu 0x2A(d)` and its `addiu`
   fills the load-delay slot, exactly as func_800974DC's landed case 0 does.

   (2) THE THREE MOVER STORES ARE IN SOURCE ORDER +0x0C, +0x10, +0x14 IN
   BOTH ARMS, and the order is worth 14 of the last 18. All six
   permutations were measured at 545 words with the index split: case 3
   gives 18 (C,A,B... i.e. 0x0C/0x14/0x10), 18, 20, 10, 20 and **8** for
   0x0C/0x10/0x14; case 6 gives 14 for the same order and 28/35 at 544
   words -- one SHORT -- for the two that put +0x14 first or last-but-one.
   Both arms at 0x0C/0x10/0x14 together: 4. The +0x14 store is the only
   insn in the block with no consumer, so wherever it sits in the source it
   eats whichever load-delay hole is open there.

   (3) THE `-8` MASK IN CASE 10 MUST BE ITS OWN STATEMENT, worth the last
   four. `*(s32 *)(d + 0x14) &= -8;` emits `li $a0,-8` after the
   `lhu 0x2(&D_80083460)`, where the target has it before; `m = -8;` ahead
   of the D_800DF248 read swaps the two and the function is byte-exact.
   `&= ~7`, `&= 0xFFFFFFF8`, `(u32 *)`, a `d + 0x14` pointer local and the
   expanded `x = x & -8` form are all still 4.

   `v2 = D_800DF248;` as its own statement before the mask is the lever
   this file already records for this function at :27577 -- without it the
   `lui`/`lh` pair falls after the `and` and three nops appear: 547 words.

   All 24 orderings of the case-10 tail's four statements were measured;
   none beats the one here, and six of them cost words (546 or 548).

   func_800B3D10, func_800B4194 and func_800C77D0 are called IMPLICITLY and
   must stay that way: all three are defined BELOW this function, and a
   declaration at the top of the unit would reach every other call site. */
void func_800B348C(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t1;
    u8 *t2;
    u8 *t3;
    u8 *t4;
    u8 *t5;
    u8 *t6;
    u8 *t7;
    u8 *t8;
    u8 *t9;
    u8 *t10;
    u8 *e1;
    u8 *e2;
    u8 *e;
    u8 *g;
    u8 *f;
    u8 *h;
    s32 x;
    s32 y;
    s32 fall;
    s16 n;
    s32 k;
    s32 k2;
    s32 v2;
    s32 m;

    switch (p[0x9B])
    {
    case 0:
        *(s32 *)(d + 0x14) &= -8;
        p[0x9B]++;
        D_800DF248 = 0;
        break;
    case 1:
        if (*(u16 *)(p + 0xA6) != 0)
        {
            t1 = D_800DD030;
            *(u8 **)(c + 0x2C) = t1;
            func_80048A44(c, t1[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(s32 *)(p + 0xCC) = 0;
            if (*(u16 *)(p + 0xA6) >= 2)
            {
                break;
            }
        }
        else
        {
            *(s32 *)(p + 0xF4) = 0;
        }
        *(u16 *)(p + 0x98) &= 0xDFFF;
        p[0x9B]++;
        /* fall through */
    case 2:
        if (*(u16 *)(p + 0xA6) != 0)
        {
            break;
        }
        func_800C77D0(d - 0x20, b, 8, 0x300);
        t2 = D_800DD068;
        *(u8 **)(c + 0x2C) = t2;
        func_80048A44(c, t2[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s32 *)(p + 0xCC) = 0;
        p[0x9B]++;
        break;
    case 3:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        k = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_0C = D_8006CCD8[k] << 17;
        b->unk_10 = D_8006CCE8[k] << 17;
        b->unk_14 = 0xFFEBC000;
        t3 = D_800DD070;
        *(u8 **)(c + 0x2C) = t3;
        func_80048A44(c, t3[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        if (*(s32 *)(d + 0x4C) != 0)
        {
            e1 = D_800DD078;
        }
        else
        {
            e1 = D_800DD0A8;
        }
        *(u8 **)(p + 0xCC) = e1;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
        break;
    case 4:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        t4 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t4;
        func_80048A44(c, t4[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        if (*(s32 *)(d + 0x4C) != 0)
        {
            e2 = D_800DD080;
        }
        else
        {
            e2 = D_800DD0B0;
        }
        *(u8 **)(p + 0xCC) = e2;
        p[0x9B]++;
        break;
    case 5:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        if (p[0xA8] != 0)
        {
            D_800DF248 = 1;
            p[0x9B]++;
            break;
        }
        *(s16 *)(p + 0x96) = 4;
        p[0x9B] += 0xA;
        break;
    case 15:
        if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        func_800A56E0(*(s32 *)(d + 0x4C) != 0 ? 0x605 : 0x606);
        t5 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t5;
        func_80048A44(c, t5[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD088;
        *(s16 *)(p + 0x96) = 2;
        p[0x9B] = 7;
        break;
    case 6:
        *(u16 *)(c + 0x14) |= 0x800;
        *(u16 *)(p + 0x98) |= 0x8;
        fall = b->unk_14;
        b->unk_0C = 0;
        b->unk_10 = 0;
        b->unk_14 = 0;
        n = --*(s16 *)(p + 0x96);
        D_800DF244 = fall;
        if (n >= 0)
        {
            break;
        }
        k2 = (*(u16 *)(d + 0x2A) >> 9) & 7;
        b->unk_0C = D_8006CCD8[k2] << 17;
        b->unk_10 = D_8006CCE8[k2] << 17;
        b->unk_14 = fall;
        *(u16 *)(c + 0x14) &= 0xF7FF;
        *(s16 *)(p + 0x96) = 4;
        *(u16 *)(p + 0x98) &= 0xFFF7;
        p[0x9B]++;
        func_800B341C(p - 0x20, *(void **)(p + 0xF4), 0x40);
        t6 = *(u8 **)(p + 0xCC);
        *(u8 **)(c + 0x2C) = t6;
        func_80048A44(c, t6[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD088;
        func_800B3D10(p[0xA8], *(s32 *)(d + 0x60), d);
        break;
    case 7:
        if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0x96) = 0;
        }
        else
        {
            *(s16 *)(p + 0x96) = *(u16 *)(p + 0x96) - 1;
        }
        if (*(s16 *)(p + 0x96) == 0)
        {
            if (p[0x102] == 0)
            {
                e = func_8009C12C(d, c, *(s16 *)(d + 0x2A), 1);
                if (e != NULL)
                {
                    func_800B4194(p[0xA8], e, d);
                    p[0xA8] = 0;
                }
                else
                {
                    e = *(u8 **)(d + 0x60);
                    if (e != NULL)
                    {
                        *(s32 *)(d + 0x60) = 0;
                        *(s32 *)(e + 0x1C) &= 0xEFFFFFFF;
                        g = *(u8 **)(e - 0x14);
                        *(s32 *)(g + 0xC) = 0x808080;
                    }
                }
            }
            else
            {
                f = *(u8 **)(d + 0x60);
                if (f != NULL)
                {
                    *(s32 *)(f + 0x14) |= 0x1000000;
                }
            }
        }
        if (*(s16 *)(p + 0x96) <= 0 && (*(u16 *)(c + 0x14) & 0xE000) != 0)
        {
            t7 = D_800DD088;
            *(u8 **)(c + 0x2C) = t7;
            func_80048A44(c, t7[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
            *(u8 **)(p + 0xCC) = D_800DD090;
            p[0x9B]++;
        }
        /* fall through */
    case 8:
        if ((*(u16 *)(p + 0xA2) & 0x10) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        if (p[0xA8] != 0)
        {
            p[0xA8] = 0;
        }
        if (*(s16 *)(p + 0x96) > 0)
        {
            break;
        }
        t8 = D_800DD090;
        *(u8 **)(c + 0x2C) = t8;
        func_80048A44(c, t8[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(u8 **)(p + 0xCC) = D_800DD098;
        p[0x9B] = 9;
        break;
    case 9:
        if ((*(u16 *)(c + 0x14) & 0xE000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        t9 = D_800DD098;
        *(u8 **)(c + 0x2C) = t9;
        func_80048A44(c, t9[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        *(s16 *)(p + 0x96) = 2;
        b->unk_0C = (((c[0x24] << 6) - (x = *(s16 *)((u8 *)b + 0x2) - 0x20)) << 16) / *(s16 *)(p + 0x96);
        b->unk_10 = (((c[0x25] << 6) - (y = *(s16 *)((u8 *)b + 0x6) - 0x20)) << 16) / *(s16 *)(p + 0x96);
        p[0x9B]++;
        break;
    case 10:
        if (--*(s16 *)(p + 0x96) > 0 && (*(u16 *)(c + 0x14) & 0x8000) == 0)
        {
            break;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        t10 = D_800DD0A0;
        *(u8 **)(c + 0x2C) = t10;
        func_80048A44(c, t10[((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 9) & 7], 0, 1);
        func_80099F04(*(u8 **)(d + 0x5C));
        h = (u8 *)&D_80083460;
        *(u16 *)(h + 0x2) |= 0x412;
        *(void **)(p + 0x8C) = func_8008ACDC;
        m = -8;
        v2 = D_800DF248;
        *(s32 *)(d + 0x14) &= m;
        if (v2 == 0)
        {
            *(s32 *)(h + 0xC) = 0;
        }
        *(s16 *)(p + 0x96) = 0;
        break;
    }
}

/* The weapon-element speed table: look the slot's kind up with
   func_800A9400 and, per element, raise one of the renderable's three
   +0x14 bits and scale its +0x20 speed by D_800E3D68 / (D_800E3D68 + 1).
   Kinds 1..3 scale unconditionally, kinds 7..9 scale by 2x when the
   wielder's +0x4C record is kind 0x10 and matches the element, by 1.5x
   when it is 0x10 and does not, and otherwise work off the wielder's own
   func_800981F8 reading; kinds 13..45 only set the bit. 289 words,
   0 differing, and the 49-entry jtbl_80089200 comes out entry for entry.

   IT OWNS A JUMP TABLE, AND THAT TABLE IS AT 0 MOD 8, so the `.align 3`
   cc1psx writes before it is a no-op -- `jtbl_80089200` is NOT one of the
   seven 4-mod-8 tables docs/dungeon-sweep.md lists, and this is the second
   landed table in the unit after func_80095538's.

   THE TABLE'S LENGTH IS THE ONLY WITNESS FOR THE TOP CASE LABEL. gcc sizes
   a switch table max-min+1, the table is 49 entries and the dispatch's
   `addiu $v0, -0x1` / `sltiu 0x31` puts min at 1, so a case 49 exists even
   though its body is empty; 46..48 are written with it because they share
   the triple rhythm of 13..45. Stopping at `case 45:` is a 45-entry table
   and four words short.

   FOUR SPELLINGS ARE LOAD-BEARING, each measured on this function:

   (1) IN KINDS 1..3 THE `+=` COMES FIRST AND THE `|=` SECOND. The other way
   round the mult/div chain loses sched1's first slot to the `|=`'s
   load/or/store, the `sw +0x14` no longer sits next to the `addu`/`sh +0x20`
   pair, and cross-jumping then merges only the `sh` instead of all four
   words: 286 words against 289, 250 differing.

   (2) IN KINDS 7..9 THE `|=` COMES FIRST. The two orders are not the same
   question: here the store is followed by a `lw +0x4C` and two byte tests,
   not by a divide.

   (3) THE func_800981F8 PATH NEEDS AN EXPLICIT `u` FOR THE OLD +0x20 AND
   AN EXPLICIT STORE. Written `*(s16 *)(c + 0x20) += ...` the `+=`'s own
   `lhu` is commoned with the `lh` the subtraction already did and the
   target's second load disappears -- the two live in different arms of the
   same statement, so nothing later re-separates them. Read into `u` right
   after the call, it lands in the `lh`'s load-delay slot exactly as the
   target has it. Worth 3 words.

   (4) EACH ARM OF THE 1.5x/1x CHOICE CARRIES ITS OWN DIVISION. With the
   `/ (D_800E3D68 + 1)` factored out of the ternary, gcc materialises
   D_800E3D68 and the `+ 1` ABOVE the arms and the target's second
   `lbu %lo(D_800E3D68)` and its in-arm `mult` both vanish: 284 words
   against 289. With the division inside both arms, cross-jumping puts the
   shared `mflo`/`addiu 1`/`div` tail back at .L800B40EC by itself.

   `D_800E3D68 / 2` is a bare `srl` and not the signed bias sequence because
   c-typeck's shorten path does a `u8 / small-constant` division in the
   UNSIGNED type; `D_800E3D68 + 1` is a plain int add, which is why the
   divide beside it is a signed `div`.

   The second parameter is never read here; the only caller, func_800B348C,
   sets $a0/$a1/$a2, so the arity is three. */
void func_800B3D10(s16 a, s32 b, u8 *c)
{
    s32 t;
    s32 u;

    switch (func_800A9400(a))
    {
    case 1:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 2:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 3:
        *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68) / (D_800E3D68 + 1);
        *(s32 *)(c + 0x14) |= 4;
        break;
    case 7:
        *(s32 *)(c + 0x14) |= 1;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 6)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 5
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 8:
        *(s32 *)(c + 0x14) |= 2;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 7)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 6
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 9:
        *(s32 *)(c + 0x14) |= 4;
        if ((*(u8 **)(c + 0x4C))[1] == 0x10)
        {
            if ((*(u8 **)(c + 0x4C))[0] == 8)
            {
                *(s16 *)(c + 0x20) += (*(s16 *)(c + 0x20) * D_800E3D68 * 2) / (D_800E3D68 + 1);
            }
            else
            {
                *(s16 *)(c + 0x20) +=
                    (*(s16 *)(c + 0x20) * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1);
            }
        }
        else
        {
            t = *(s16 *)(c + 0x20) - func_800981F8(c) * 2;
            u = *(u16 *)(c + 0x20);
            if (t < 0)
            {
                t = 0;
            }
            *(s16 *)(c + 0x20) = u + ((*(u8 **)(c + 0x4C))[0] == 7
                                          ? (t * (D_800E3D68 + D_800E3D68 / 2)) / (D_800E3D68 + 1)
                                          : (t * D_800E3D68) / (D_800E3D68 + 1));
        }
        break;
    case 13:
    case 40:
    case 43:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 14:
    case 41:
    case 44:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 16:
    case 19:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 17:
    case 20:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 22:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 23:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 31:
    case 34:
    case 37:
        *(s32 *)(c + 0x14) |= 1;
        break;
    case 32:
    case 35:
    case 38:
        *(s32 *)(c + 0x14) |= 2;
        break;
    case 15:
    case 18:
    case 21:
    case 24:
    case 33:
    case 36:
    case 39:
    case 42:
    case 45:
        *(s32 *)(c + 0x14) |= 4;
        break;
    case 46:
    case 47:
    case 48:
    case 49:
        break;
    }
}

/* func_800B4194's two rodata strings (defined by the INCLUDE_RODATA just after
   it) and the five engine strings its message chains use. */
extern u8 D_800892C4[];
extern u8 D_800892C8[];
extern u8 D_800E0BC7[];
extern u8 D_800E0BDC[];
extern u8 D_800E0BF3[];
extern u8 D_800E0C09[];
extern u8 D_800E0C1D[];

s32 func_800B4194(s16 a, u8 *p, u8 *c)
{
    void *m;
    void *h;
    u8 *d;
    u8 *q;
    s16 flag;
    s16 k;
    s16 t;
    s32 x;
    s32 y;

    k = func_800A9400(a);
    flag = p[0x28] + *(s16 *)(p + 0x64) <= 0;
    if (k != 0)
    {
        switch (k)
        {
        case 0x10:
        case 0x13:
            if (!flag)
            {
                if (func_800C8C1C(p, D_800E3D7C[0xA9] + 0x80,
                                  (D_800E3D7C[0xA9] >> 2) + 0x10 + (func_800A6D30() & 3)) != 0)
                {
                    m = func_800990FC();
                    h = func_80099194(D_800E0BC7, m);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800892C4, h);
                    func_80099290(h);
                    func_800A5720(m);
                }
            }
            break;
        case 0x12:
        case 0x15:
            if (!flag && (*(s32 *)(p + 0x1C) & 0x20) == 0)
            {
                if (func_800C8980(p, D_800E3D7C[0xA9] + 0x80,
                                  (D_800E3D7C[0xA9] >> 2) + 0x2 + (func_800A6D30() & 3)) != 0)
                {
                    m = func_800990FC();
                    h = func_80099194(D_800E0BDC, m);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800892C8, h);
                    func_80099290(h);
                    func_800A5720(m);
                }
            }
            break;
        }
    }
    d = *(u8 **)(c + 0x4C);
    if (d == NULL)
    {
        return 0;
    }
    if (d[1] != 0x10)
    {
        return 0;
    }
    switch (d[0])
    {
    case 3:
        t = -(*(s16 *)(p + 0x64) >> 3);
        if (t != 0)
        {
            *(s16 *)(c + 0x64) = *(u16 *)(c + 0x64) + t;
            func_800B4C7C(0x10, c, t, 1);
        }
        break;
    case 4:
        if (!flag && (*(s32 *)(p + 0x1C) & 0x20) == 0 && func_800C8980(p, 8, 8) != 0)
        {
            func_800A56E0(0x700);
            m = func_800990FC();
            h = func_80099194(D_800E0BF3, m);
            h = func_80099734(p, h);
            h = func_80099194(D_800892C4, h);
            func_80099290(h);
            func_800A5720(m);
        }
        break;
    case 5:
        if (flag)
        {
            q = p + 0x48;
            if (q[1] == 0 && (func_800A6D30() & 3) == 0)
            {
                q[1] = 0xE;
                if (D_80012090 != 2)
                {
                    if (D_8008146C < 0xC)
                    {
                        x = 0x10;
                        y = 0x18;
                        q[0] = 1;
                    }
                    else if (D_8008146C < 0x16)
                    {
                        x = 0x50;
                        y = 0x78;
                        q[0] = 2;
                    }
                    else
                    {
                        goto three;
                    }
                    q[2] = func_800A6DA4(x, y);
                    q[3] = 0;
                    break;
                }
            three:
                p[0x48] = 3;
                q[2] = func_800A6DA4(0x10, 0x18);
                q[3] = 1;
            }
        }
        break;
    case 9:
        if (!flag && (s16)func_80042900((s8 *)p, 6) == 0 && (func_800A6D30() & 7) == 0 &&
            func_800A48F0((s8 *)p, 6, 0x10) >= 0)
        {
            func_800A56E0(0x700);
            m = func_800990FC();
            h = func_80099194(D_800E0C09, m);
            h = func_80099734(p, h);
            h = func_80099194(D_800E0C1D, h);
            func_80099290(h);
            func_800A5720(m);
        }
        break;
    }
    return 0;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892C4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892C8);
