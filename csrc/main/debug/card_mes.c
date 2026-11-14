#include "common.h"
#include "main_twin.h"

/* Defined in sibling units of this block; main_twin.h does not
   declare them because they were file-local to the single unit
   these were carved out of. */
extern void func_804008A0(s32 use_second_slot);

/* THE 0x80400000 DEBUG TWIN -- card_mes.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x80400548..0x804005A0   .text from 0x80402508

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main/debug/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- card_mes.c's assert pair. --- */
/* 0x80400548 */
const char D_80400548[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x80400570 */
const char D_80400570[12] __attribute__((section(".rodata"))) = "card_mes.c";

/* 0x8040057C "リプレイ" -- the four menu rows, D_80408C8C[3]..[0] */
const u8 D_8040057C[12] __attribute__((section(".rodata"))) = "\x83\x8A\x83\x76\x83\x8C\x83\x43";

/* 0x80400588 "消す" */
const u8 D_80400588[8] __attribute__((section(".rodata"))) = "\x8F\xC1\x82\xB7";

/* 0x80400590 "コピー" */
const u8 D_80400590[8] __attribute__((section(".rodata"))) = "\x83\x52\x83\x73\x81\x5B";

/* 0x80400598 "ロード" */
const u8 D_80400598[8] __attribute__((section(".rodata"))) = "\x83\x8D\x81\x5B\x83\x68";

void func_80402508(u8 *s, s32 a, s32 *x, s32 *y, s32 *z)
{
    u8 *t;
    s32 total = 0;
    s32 n = 0;
    s32 i = 0;

    *z = 0;
    if (a < 0)
    {
        return;
    }
    do
    {
        *z = n + *z;
        while (*s == ' ')
        {
            s++;
            total++;
        }
        *x = total;
        n = 0;
        t = s;
        while (*t != ' ' && *t != 0)
        {
            t += 2;
            n++;
        }
        s = t;
        total += n * 2;
        *y = n;
    } while (++i <= a);
}

/* Live func_80022300, with three constants changed: n * 12 where it has
 * n << 3, 0x15 where it has 0x13, and 0x800000 where it has 0xA04010. */
u8 *func_804025B8(s32 *a, u8 *p, s32 n)
{
    p[1] = 0x28;
    p[0xA] = (n * 12) + 4;
    p[0xB] = 0x15;
    *(s32 *)(p + 4) = 0x800000;
    p[0] |= 0x80;
    return p + 0xC;
}

void func_804025F8(u8 *p, s32 idx, s32 n)
{
    s32 i;

    /* The frame is 0x28 here against the live build's 0x20, with the same
       four saves 8 bytes higher and every instruction otherwise identical.
       That used to need a DEVICE -- `s32 unused[2]`, eight bytes of locals
       the function never reads -- on the theory that the debug source had an
       unused local. It did not: FSF 2.7.2 simply reserves those 8 bytes and
       the Cygnus snapshot does not, so the device came out when this unit
       moved to tools/psx272gcc. See the header. */
    func_80051DE4(p, 1);
    p += idx * 12;
    for (i = 0; i < n; i++)
    {
        func_800514A8(p, 0);
        p += 12;
    }
}

/* Live func_800223B4, in raw word indices because every offset moved: ptrs
 * at c[0x1A3] against the live group's 563, the pair at c[0x1A8] (ptrs[5],
 * not ptrs[6]), the scratch at c[0x4B] against p[57], and x * 6 where the
 * live one has x * 4. `sx` still has to be the FIRST local: the target
 * computes it at word 3, ahead of the $s4 save. */
void func_80402670(s32 *c, s32 x, s32 y, s32 n, s32 z)
{
    s32 sx = x * 6;
    UnkCardPair *e = (UnkCardPair *)c[0x1A8];
    u8 *q = (u8 *)&c[0x4B];

    e->unk_04->unk_08 = sx - (c[6] / 2) + 0xA2;
    e->unk_04->unk_0A = (n * 17) - (c[7] / 2) + 0x7A;
    func_804025B8(c, q, y);
    e->unk_00 = (s32)q;
    func_804025F8((u8 *)((UnkCardPair *)c[n + 0x1A3])->unk_00, z, y);
}

/* Live func_80022488. The strings are a SEPARATE table here, D_80408B2C,
 * four per record (the index is `sll 4`), where the live build has them in
 * the same 24-byte UnkStrSet as the four bytes. */
void func_8040274C(s32 *p)
{
    s32 i = p[2];
    s32 n = D_80408ADC[i].unk_01 - 1;
    s32 x;
    s32 y;
    s32 z;

    if (D_80408ADC[i].unk_02 != 0)
    {
        func_80402508(D_80408B2C[i][n], p[0], &x, &y, &z);
        func_80402670(p, x, y, n, z);
    }
}

/* The third of the input-word handlers, after func_80405FC4 and
 * func_80406DA0. Its masks are 0xA000/0x8000/0x2000 where theirs are
 * 0x5000/0x1000/0x4000, its 0x40 arm does nothing at all, its counter is
 * p[4] with a threshold of 3 and NO decrement in the over-threshold arm,
 * and the step comes out of the message table. */
void func_804027F8(s32 *p)
{
    s32 dir = 0;

    if (D_801379A8.unk_00 != 0)
    {
        if ((D_801379B0.unk_00 & 0x40) == 0)
        {
            if (D_801379B0.unk_00 & 0x20)
            {
                func_80063FF8(0x514);
                if (p[8] != 0)
                {
                    *(s32 *)p[8] = func_8040343C(1);
                }
                else
                {
                    func_8040343C(1);
                }
            }
            else if (D_801379A8.unk_00 & 0xA000)
            {
                if (D_801379B0.unk_00 & 0xA000)
                {
                    p[4] = 0;
                    if (D_801379B0.unk_00 & 0x8000)
                    {
                        dir = -1;
                    }
                    else if (D_801379B0.unk_00 & 0x2000)
                    {
                        dir = 1;
                    }
                }
                else if (p[4] >= 3)
                {
                    if (D_801379A8.unk_00 & 0x8000)
                    {
                        dir = -1;
                    }
                    else if (D_801379A8.unk_00 & 0x2000)
                    {
                        dir = 1;
                    }
                }
                else
                {
                    p[4]++;
                }
            }
        }
        if (dir != 0)
        {
            func_80063FF8(0x502);
            p[0] = func_80056450(p[0], dir, D_80408ADC[p[2]].unk_02);
            func_8040274C(p);
        }
    }
}

void func_8040293C(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_8040343C(0);
    }
}

void func_80402974(UnkCardPair *p, u32 *a, u32 *b)
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
    p->unk_04->unk_0A = -72;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

u8 *func_80402A1C(u8 *p, s32 *q)
{
    u8 buf[4];
    u32 w;
    u32 h;
    u32 hh;
    u32 bh;
    s32 g;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_80400544;
    i = 0;
    w = D_80408ADC[q[2]].unk_00 * 12;
    hh = D_80408ADC[q[2]].unk_01;
    h = hh * 17;
    q[6] = w + 8;
    q[7] = h + 8;
    for (; i < 4; i++)
    {
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x101010;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = (s32)(w + 0xA) >> 1;
        bh = h + 0xA;
        p[0xB] = bh >> 1;
        p[1] = 0x48;
        *(s32 *)(p + 4) = 0x101010;
        p[0] = buf[i];
        p += 0xC;
        *(UnkBlock24 *)p = D_80408C6C;
        g = 0x30BFC0;
        /* DEVICE: one marker, worth 68 words (68 -> 0). `"0"(g)` is a second
           set of g inside the loop, and `"r"(p)` is a VARYING input, so the
           asm's set is not invariant and `consec_sets_invariant_p` (loop.c)
           refuses the pair: the lui/ori stays in the loop, where the target
           has it. This is the twin of the same device in func_80402A1C's live
           counterpart func_80022774, with one difference that matters -- there
           the varying input is `i`, here it is `p`. `i` cannot do the job in
           this build: it has no non-giv use, so loop.c's strength reduction
           eliminates it and rewrites `i < 4` as the buf pointer against
           `buf + 4` (`slt $t1, $t8`), which is what the target does. Naming
           `i` in the marker is the one spelling that keeps it alive, and that
           costs the elimination -- 104 words, 74 differing.
           `"r"(bh)` is the live device's (b)/(c) jobs and does both here too:
           it is the last use of `h + 0xA`, so force_movables does not link the
           `srl` to the `addiu` and the shift stays in the loop while the addiu
           hoists; and it is bh's third reference, which fixes its register.
           Legal spellings ruled out: no marker 68; `"0"(g), "r"(bh)` 67 (bh is
           loop-invariant, so the asm is invariant and both insns hoist);
           `"0"(g), "r"(p)` 24, with or without `volatile`, and the same 24 with
           the shift written inline as `p[0xB] = (h + 0xA) >> 1;`.
           Operand order does not matter. `h = hh * 17;` is free here -- the
           live source needs `(hh << 4) + hh` only because expand_mult's copy
           would lose its hard-register pin, and this build has no pin.
           Warns under -Wall? No -- g is initialised. */
        __asm__("" : "=r"(g) : "0"(g), "r"(p), "r"(bh));
        *(s32 *)(p + 4) = g;
        *(s32 *)(p + 0xC) = g;
        *(s32 *)(p + 0x10) = g;
        *(s32 *)(p + 0x14) = g;
        p[0xA] = (w + 8) >> 1;
        p[0xB] = (h + 8) >> 1;
        p[0] = buf[i];
        p += 0x18;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80402BB8(s32 *p)
{
    s32 **q = (s32 **)p[0x1A7];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

/* Live func_80022934. FOUR entries here, not five; the buffer stride is
 * 0x108 from a base of 0x234 where the live one is 0x180 from 0x114; the
 * strings come from the separate D_80408B2C table. */
void func_80402BE0(s32 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[i + 0x1A3];

        e->unk_04->unk_08 = 0x124 - (p[6] / 2);
        e->unk_04->unk_0A = (i * 17) - (p[7] / 2) + 0xFC;
        e->unk_00 = (s32)func_80051B50((u8 *)p + 0x234 + i * 0x108,
                                       D_80408B2C[p[2]][i], 1);
    }
}

/* Live func_80022A30, whose seven-statement preheader device carries over --
 * see that function's note in csrc/main/card_mes.c for why the `h1 = w1` copy
 * survives. Two differences here: the shared word is 0x101010 where the live
 * one has 0x104058, and the SECOND height is not a copy but a shift the
 * target recomputes every iteration. The marker in the loop is what keeps it
 * there. */
u8 *func_80402CD4(u8 *p, s32 *q)
{
    u8 buf[4];
    u8 w1;
    u8 h1;
    u8 c;
    s32 d;
    u8 w2;
    u32 e;
    s32 i;

    *(UnkFour *)buf = *(UnkFour *)D_80400544;
    i = 0;
    w1 = 5;
    h1 = w1;
    c = 0x48;
    d = 0x101010;
    w2 = 6;
    e = 0xC;
    for (; i < 4; i++)
    {
        p[0xA] = w1;
        p[0xB] = h1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
        p[0xA] = w2;
        /* DEVICE: one marker, worth 24 words (24 -> 0) and the last word of
           length. `"0"(e)` is a SECOND set of e inside the loop and `"r"(p)`
           is a varying input, so the asm is not invariant, e is not invariant,
           and loop.c leaves `e >> 1` in the loop -- where the target computes
           `srl $v0,$a3,1` every iteration. Written without the marker the
           shift is a movable, loop.c hoists it into the preheader, e is a
           known 0xC in that block and the shift FOLDS to `li 6`: one word
           short and every register after it renamed. Keeping the constant out
           of the loop is the point, so this is the mirror of func_80402A1C's
           marker, which keeps one in.
           `"r"(d)` TWICE is the second job. One set and one use leave e with
           three references against d's two, so e takes $a1 and d takes $a3;
           the target has them the other way round. Two more references for d
           restore the order. Three also match.
           Legal spellings ruled out: no marker 46 words, 24 differing;
           `"0"(e)` alone 48/29; `"r"(e)` alone 48/36; `volatile` with the
           same operands 47/2; dropping `"r"(p)` 48/25 (the asm goes back to
           being invariant); dropping one `"r"(d)`, or replacing it with
           `"r"(c)`, 47/6; and declaring d after e, or e before d, 47/6.
           Warns under -Wall? No. */
        __asm__("" : "=r"(e) : "0"(e), "r"(p), "r"(d), "r"(d));
        p[0xB] = e >> 1;
        p[1] = c;
        *(s32 *)(p + 4) = d;
        p[0] = buf[i];
        p += 0xC;
    }
    p[-0x18] |= 0x80;
    return p;
}

void func_80402D90(s32 *p)
{
    func_80402CD4((u8 *)(p + 9), p);
    func_80402BB8(p);
}

s32 func_80402DC8(s32 a)
{
    return func_80047358(a, D_80408C84, 6);
}

/* Live func_80022B48, the UnkTwinGrp670 instance of the five-way template builder. The
 * group is 0x670 bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80402DF0(UnkTwinGrp670 *c, s32 n)
{
    s32 i;

    for (i = 0; i < n; i++)
    {
        c->ptrs[i] = &c->ents[i];
        c->prims[i][0] = D_8002D690_a[0];
        c->prims[i][1] = D_8002D690_a[1];
        c->prims[i][2] = D_8002D690_a[2];
        c->prims[i][3] = D_8002D690_a[3];
        c->ents[i].unk_04 = (UnkCardPrim *)c->prims[i];
        c->ents[i].unk_08 = (UnkCardPrim12 *)c->shared;
    }
    c->shared[0] = D_8002D6A0.unk_00;
    c->shared[1] = D_8002D6A4.unk_00;
    c->shared[2] = D_8002D6A8.unk_00;
}

void func_80402EA4(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    p[5] = a;
    p[2] = b;
    p[3] = c;
    p[8] = d;
}

void func_80402EBC(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    func_80402EA4(p, a, b, c, d);
    func_80402974((UnkCardPair *)(p + 0x19F), (u32 *)(p + 0x195), (u32 *)(p + 0x199));
    func_80402D90(p);
}

/* The two handlers are the other way round from the live func_80022CD8, and
 * the pointer goes through a LOCAL: the target materialises func_804027F8
 * before the test and overwrites it, where two stores would cost a second
 * %hi pair. Same in-place device as func_804008A0's. */
void func_80402F04(s32 *p)
{
    void (*f)(s32 *);

    func_80402A1C((u8 *)(p + 9), p);
    func_80402BE0(p);
    p[0] = D_80408ADC[p[2]].unk_03;
    func_8040274C(p);
    f = func_804027F8;
    if (p[3] == 2)
    {
        f = func_8040293C;
    }
    p[-4] = (s32)f;
}

/* Live open_card_message, the third of the group-allocating family after
 * func_804057F0 and func_804076C8 -- same assert this build carries and the
 * live one does not. func_80402DF0 takes 6 here, not 7; raw word indices for
 * the group: root at r[0x19F], its unk_0C at r[0x1A2], ptrs at r[0x1A3]. */
s32 func_80402F84(s32 a, s32 id, s32 kind, s32 p)
{
    s32 *q = (s32 *)D_80409508;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_80409508, 0x1E1);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_80400548, D_80400570, 0x299);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_8040343C(r[3] != 2);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x764);
    }
    func_80402DF0((UnkTwinGrp670 *)r, 6);
    q[3] = (s32)(r + 0x19F);
    r[0x1A2] = func_80402DC8((s32)(r + 0x1A3));
    func_80402EBC(r, a, id, kind, p);
    q[4] = (s32)func_80402F04;
    return (s32)q;
}

void func_804030CC(s32 a, s32 p)
{
    func_80402F84(a, 0, 1, p);
}

/* THE ELEVEN ZERO-KIND MESSAGE WRAPPERS, and the twelfth below them, were
 * the reload_cse_regs cluster: one word each, `addu $a3, $a2, $zero` in the
 * live build against `addu $a3, $zero, $zero` here, because 4.1's
 * reload_cse_simplify_set rewrites the second zero argument into a copy of
 * the first. tools/psx272gcc has no such pass and they land untouched.
 *
 * THE MESSAGE ID IS THE ONLY THING THAT PAIRS THEM, and twinalign cannot:
 * every wrapper masks to the same ten words, so its order-preserving DP had
 * the whole run shifted by one. Each id below is read off that function's
 * own listing and agrees with csrc/main_twin.h's map. */
/* Live msg_no_card. */
void func_804030F4(s32 a)
{
    func_80402F84(a, 1, 0, 0);
}

/* Live msg_please_wait, NOT msg_loading -- twinalign paired it with
 * msg_loading, which is the arbitrary-pairing trap this block's wrapper run
 * is famous for, and func_804031E4 already holds message 14. Message 2 with
 * kind 2 is msg_please_wait, and it is the only live wrapper with that pair. */
void func_8040311C(s32 a, s32 p)
{
    func_80402F84(a, 2, 2, p);
}

/* Live msg_overwrite_failed. */
void func_80403144(s32 a)
{
    func_80402F84(a, 8, 0, 0);
}

void func_8040316C(s32 a, s32 p)
{
    func_80402F84(a, 4, 2, p);
}

void func_80403194(s32 a, s32 p)
{
    func_80402F84(a, 5, 1, p);
}

void func_804031BC(s32 a, s32 p)
{
    func_80402F84(a, 6, 2, p);
}

void func_804031E4(s32 a, s32 p)
{
    func_80402F84(a, 14, 2, p);
}

/* Live msg_not_enough_blocks_overwrite. */
void func_8040320C(s32 a)
{
    func_80402F84(a, 9, 0, 0);
}

/* Live msg_not_enough_blocks. */
void func_80403234(s32 a)
{
    func_80402F84(a, 10, 0, 0);
}

/* Live msg_load_failed. */
void func_8040325C(s32 a)
{
    func_80402F84(a, 11, 0, 0);
}

/* Live msg_format_failed. */
void func_80403284(s32 a)
{
    func_80402F84(a, 12, 0, 0);
}

/* Live msg_format_done. */
void func_804032AC(s32 a)
{
    func_80402F84(a, 13, 0, 0);
}

/* Live msg_card_damaged. */
void func_804032D4(s32 a)
{
    func_80402F84(a, 3, 0, 0);
}

/* Live msg_no_saved_game. */
void func_804032FC(s32 a)
{
    func_80402F84(a, 7, 0, 0);
}

void func_80403324(s32 a, s32 p)
{
    func_80402F84(a, 15, 2, p);
}

/* Live msg_copy_failed. */
void func_8040334C(s32 a)
{
    func_80402F84(a, 16, 0, 0);
}

/* Live msg_cannot_overwrite. */
void func_80403374(s32 a)
{
    func_80402F84(a, 17, 0, 0);
}

void func_8040339C(s32 a, s32 p)
{
    func_80402F84(a, 18, 1, p);
}

/* Live msg_delete_failed. */
void func_804033C4(s32 a)
{
    func_80402F84(a, 19, 0, 0);
}

void func_804033EC(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80051FBC(*p[0x1A3]);
        p++;
    } while (++i < 4);
}

s32 func_8040343C(s32 flag)
{
    s32 **q = (s32 **)D_80409528;

    if (func_80047FD8((s32 *)(q - 8)) != 0)
    {
        if (flag != 0)
        {
            D_8040953C[15] |= 0x2000;
        }
        func_804033EC(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4 |= 0x8000;
    }
    return (s32)q[0];
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S `.data`, 0x80408ADC..0x80408C8C.

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

/* 0x80408ADC */
/* The 20 message records, four bytes each. splat cuts a label at the
   first three bytes (D_80408ADD..D_80408ADF) because the unit reads them
   with lb; nothing references those names. */
UnkTwinMsg D_80408ADC[20] = {
    {0x15, 0x03, 0x02, 0x01},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0C, 0x01, 0x00, 0x00},
    {0x14, 0x02, 0x00, 0x00},
    {0x12, 0x02, 0x00, 0x00},
    {0x0D, 0x02, 0x02, 0x00},
    {0x0E, 0x02, 0x00, 0x00},
    {0x0D, 0x01, 0x00, 0x00},
    {0x10, 0x02, 0x00, 0x00},
    {0x15, 0x04, 0x00, 0x00},
    {0x13, 0x04, 0x00, 0x00},
    {0x10, 0x01, 0x00, 0x00},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0E, 0x01, 0x00, 0x00},
    {0x0E, 0x02, 0x00, 0x00},
    {0x0C, 0x02, 0x00, 0x00},
    {0x0B, 0x01, 0x00, 0x00},
    {0x14, 0x02, 0x00, 0x00},
    {0x12, 0x02, 0x02, 0x01},
    {0x0E, 0x01, 0x00, 0x00},
};

/* 0x80408B2C */
/* The 20 records' strings, four per record. They live in this block's
   rodata pool, which is not owned yet, so they are addresses. */
u8 *D_80408B2C[20][4] = {
    {(u8 *)0x80400518, (u8 *)0x804004F4, (u8 *)0x804004D4, (u8 *)0x804004D0},
    {(u8 *)0x804004B0, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400474, (u8 *)0x80400448, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400420, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400404, (u8 *)0x804003E8, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804003C8, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804003AC, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400388, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x8040035C, (u8 *)0x80400330, (u8 *)0x80400308, (u8 *)0x804002F4},
    {(u8 *)0x804002D8, (u8 *)0x804002B4, (u8 *)0x80400308, (u8 *)0x804002F4},
    {(u8 *)0x80400290, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400270, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400250, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400230, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400218, (u8 *)0x80400494, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400200, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x804001E4, (u8 *)0x804001B8, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400190, (u8 *)0x80400168, (u8 *)0x804004D0, (u8 *)0x804004D0},
    {(u8 *)0x80400148, (u8 *)0x804004D0, (u8 *)0x804004D0, (u8 *)0x804004D0},
};

/* 0x80408C6C */
/* The 24-byte primitive template, D_80027F48's twin. */
UnkBlock24 D_80408C6C = {{0x00, 0x38, 0x00, 0x00, 0xBC, 0xBC, 0x9C, 0x00, 0x00, 0x00, 0xA0, 0x70,
                          0xBC, 0xBC, 0x9C, 0x00, 0xBC, 0xBC, 0x9C, 0x00, 0xBC, 0xBC, 0x9C, 0x00}};

/* 0x80408C84 */
/* D_80027F60's twin. */
u8 D_80408C84[0x8] __attribute__((section(".data"))) = {
    0x04, 0x05, 0x00, 0x01, 0x02, 0x03, 0x06, 0x35};

