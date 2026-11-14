#include "common.h"
#include "main_twin.h"

/* THE 0x80400000 DEBUG TWIN -- card_opt.c, one of the nine translation units the twin's
   nine __FILE__ strings name.

   .rodata 0x804005A0..0x804005F8   .text from 0x804034D8

   Carved out of csrc/ovl_main_80400000.c on 2026-11-13. The unit
   boundaries and which of them are INFERRED are in
   docs/overlays.md, "Carving the twin".  Build flags come from the
   build/csrc/main/debug/%.o rules in the Makefile, NOT from the
   ovl_% pattern -- this block is -G8 and uses its own compiler. */


/* --- card_opt.c's assert pair. --- */
/* 0x804005A0 */
const char D_804005A0[40] __attribute__((section(".rodata"))) = "Assertion failed: file \"%s\", line %d\n";

/* 0x804005C8 */
const char D_804005C8[12] __attribute__((section(".rodata"))) = "card_opt.c";

/* 0x804005D4  D_80408CB0[1] */
const char D_804005D4[4] __attribute__((section(".rodata"))) = "";

/* 0x804005D8 "使用できません。" -- D_80408CB0[0] */
const u8 D_804005D8[20] __attribute__((section(".rodata"))) =
    "\x8E\x67\x97\x70\x82\xC5\x82\xAB\x82\xDC\x82\xB9"
    "\x82\xF1\x81\x42";

/* 0x804005EC "回" */
const u8 D_804005EC[4] __attribute__((section(".rodata"))) = "\x89\xF1";

/* 0x804005F0 "冒険中" */
const u8 D_804005F0[8] __attribute__((section(".rodata"))) = "\x96\x60\x8C\xAF\x92\x86";


/* Live func_80023258, with the ptrs array 0x18 further along and FOUR slots
 * in the loop, not three. This function is what found --expand-div: see the
 * Makefile.
 *
 * THE MARKER TAKES $v0 AWAY FROM THE TIED QUOTIENT, and it is worth SIX
 * words -- delete it, keep the `c` hoist, and this is 112 words with 6
 * differing. local-alloc ties the quotient to `e + 0xA` (an arithmetic
 * source is a REG, so `combine_regs` fires) and then prices that pair,
 * `ptrs[1]` and `->unk_04` at EXACTLY 5000 each under `qty_compare_1`:
 * 4 refs over 16, 2 over 4, 2 over 4, all of them floor_log2(n)*n/span = 1/2.
 * A three-way exact tie falls through to the quantity NUMBER, which is birth
 * order, and the quotient is born at the `div` five insns ahead of the
 * `lw 0x1BC` -- so it takes $v0 and the pointers fall to $v1.
 *
 * The divisor is a FOURTH quantity that already outranks all three: hoisted
 * into `c` it is 3 refs over a span of 2, priced 15000, allocated first into
 * $v0 and released at the `div`. Naming it once more AFTER the division
 * stretches it over the quotient's range but not over the pointers', so the
 * tied pair is pushed to $v1 -- `mflo $v1` / `addiu $v1,$v1,0xA`, tied, which
 * is what the target has -- and both pointers get $v0 back.
 *
 * The earlier reading of this function was backwards and is worth recording
 * so it is not re-derived: it tried to UNTIE the quotient with a reference
 * from a later block, which did buy the pointers their $v0 but made the
 * quotient a global allocno stranded in $t3 (6 -> 2, never 0). The target's
 * `addiu $v1,$v1,0xA` reads and writes one register, so the target is TIED;
 * the tie had to be kept and merely lose the contest. */
void func_804034D8(s32 *p)
{
    s32 w = p[35];
    s32 c = p[32];
    s32 e = ((p[34] - w) * (p[33] << 4)) / c;
    s32 d = e + 0xA;
    s32 i;

    /* DEVICE: one extra reference to the divisor, after the division. It
       emits no instruction -- the template is empty and the operand is
       already in a register -- so it costs zero words. What it buys is the
       lifetime of `c`: local-alloc.c prices the divisor's quantity at 15000
       and hands it $v0 first, and this reference keeps that quantity alive
       past the `div`, across the range of the tied {quotient, quotient+0xA}
       quantity, which therefore cannot have $v0 and takes $v1 -- the target's
       `mflo $v1` / `addiu $v1,$v1,0xA`. `c` is dead again before `ptrs[1]` is
       loaded, so the pointer pair gets $v0 back. Worth SIX words: without it
       the function is correct C, still 112 words, and 6 differ. `c` itself
       exists only so the divisor has a name to reference; `p[32]` written
       inline here would emit a second load. The matching constraint pins the
       marker to `w` without a copy; `w` is used after it, so nothing is dead
       and there is no uninitialised read, hence no -Wall warning (the default
       CFLAGS has no -Wall in any case). Spellings ruled out, and the legal
       asm-free attempts at the same effect, are measured in the header. */
    __asm__("" : "=r"(w) : "0"(w), "r"(c));
    ((UnkTwinGrp19C *)p)->ptrs[1]->unk_04->unk_0A = (w << 4) + d;
    for (i = 0; i < 4; i++)
    {
        s32 v = ((u8 *)p)[0x7C + i];
        s32 u;
        s32 n;
        u8 *q = (u8 *)p + i + 0x7C;

        if (i == p[34])
        {
            n = v + (8 - v) / (p[32] - p[33] + 1);
        }
        else
        {
            n = v;
            if (n != 0)
            {
                n -= 1;
            }
        }
        *q = n;
        u = ((u8 *)p)[0x7C + i];
        ((UnkTwinGrp19C *)p)->ptrs[10 + i]->unk_04->unk_08 = u + 0x24;
        ((UnkTwinGrp19C *)p)->ptrs[14 + i]->unk_04->unk_08 = u + 0x8F;
        ((UnkTwinGrp19C *)p)->ptrs[2 + i]->unk_04->unk_08 = u + 9;
        ((UnkTwinGrp19C *)p)->ptrs[6 + i]->unk_04->unk_08 = u + 0x42;
        ((u8 *)((UnkTwinGrp19C *)p)->ptrs[10 + i]->unk_04)[0] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkTwinGrp19C *)p)->ptrs[10 + i]->unk_04)[1] = ((u8 *)p)[0x7C + i] * 3 + 0x68;
        ((u8 *)((UnkTwinGrp19C *)p)->ptrs[10 + i]->unk_04)[2] = ((u8 *)p)[0x7C + i] * 4 + 0x60;
    }
    if (p[33] < p[32])
    {
        p[33]++;
    }
}

/* Live func_800233D0, with FOUR revision differences and nothing else -- the
 * families in this file's header, all four at once. UnkCardGrp184's `ptrs`
 * array is 0x18 LATER here (ptrs[0] at 0x1B8 against the live 0x1A0), so the
 * indices are written raw off p; the loop runs to 4 where the live source
 * runs to 3, which makes the four groups of four tile exactly where the live
 * ones leave gaps; func_80056F14 is called with 4 rather than 3; and the
 * embedded stride is 0x3C against 0x48, the same 12-bytes-smaller record that
 * func_804037F4 sees. */
void func_80403698(s32 *p)
{
    s32 i;

    ((UnkCardPair *)p[0x6E])->unk_00 = func_80056F14(4);
    ((UnkCardPair *)p[0x6F])->unk_00 = (s32)D_80082FF4;
    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[0x70 + i])->unk_00 = (s32)D_80083060;
        ((UnkCardPair *)p[0x74 + i])->unk_00 = (s32)D_8008306C;
        ((UnkCardPair *)p[0x78 + i])->unk_00 = (s32)(p + 1);
        ((UnkCardPair *)p[0x7C + i])->unk_00 = (s32)((u8 *)p + 0x90 + i * 0x3C);
    }
}

void func_80403738(s32 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[0x78 + i])->unk_04->unk_08 = 0x24;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[0] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[1] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x78 + i])->unk_04)[2] = 0x60;
        ((UnkCardPair *)p[0x70 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[0x74 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[0x7C + i])->unk_04->unk_08 = 0x8F;
    }
}

/* Live func_8002352C, with TWO revision differences and one addressing lever.
 * The stride is 0x3C where the live source steps 0x48, and the bound is 4
 * where it is 3 -- both measured off the listing, and both of the families
 * the header describes: a structure 12 bytes smaller, and a table walked to
 * one more entry. It also needed a sized `D_80408C8C_a[1]` alias under 4.1's
 * compiler, to make the table's address one insn; tools/psx272gcc makes it
 * one insn anyway, so the plain `D_80408C8C` is back. */
void func_804037F4(u8 *p)
{
    s32 i;

    for (i = 0; i < 4; i++)
    {
        func_80051B50(p + 0x90 + i * 0x3C, D_80408C8C[i], 1);
    }
}

s32 func_80403868(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80058368(p, r);
    return p;
}

void func_804038C8(s32 *p)
{
    s32 i;

    p[32] = 1;
    p[33] = 1;
    func_804037F4((u8 *)p);
    func_80403868((s32)(p + 1), 0x36, 0x10);
    func_80403698(p);
    ((UnkCardPair *)p[110])->unk_04->unk_0A = 0x22;
    for (i = 0; i < 4; i++)
    {
        ((UnkCardPair *)p[124 + i])->unk_04->unk_08 = 0x8E;
        ((UnkCardPair *)p[124 + i])->unk_04->unk_0A = (i * 16) + 0x82;
        ((UnkCardPair *)p[112 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[112 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[116 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[116 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[120 + i])->unk_04->unk_08 = 0x24;
        ((UnkCardPair *)p[120 + i])->unk_04->unk_0A = (i * 16) + 0xA;
    }
    func_80403738(p);
}

void func_804039F8(UnkCardPair *p, u32 *a, u32 *b)
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
    p->unk_04->unk_08 = -152;
    p->unk_04->unk_0A = -98;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

s32 func_80403AA0(s32 a)
{
    return func_80047358(a, D_80408C9C, 0x12);
}

/* Live func_800237EC, the UnkTwinGrp19C instance of the five-way template builder. The
 * group is 0x19C bytes of padding where the live one is a different size, and the
 * two templates are this block's own D_8002D690/D_8002D6A0 pair -- the first
 * through a BASE POINTER (one lui, four offsets) and the trailing three as
 * separate absolutes. */
void func_80403AC8(UnkTwinGrp19C *c, s32 n)
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

void func_80403B7C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80403B84(s32 *p, s32 v)
{
    func_80403B7C(p, v);
    func_804039F8((UnkCardPair *)(p + 0x6A), (u32 *)(p + 0x60), (u32 *)(p + 0x64));
    func_804038C8(p);
}

/* Live func_800238D4, the same allocator over the UnkTwinGrp19C: root at
 * r[0x6A], unk_0C at r[0x6D], ptrs at r[0x6E]. */
s32 *func_80403BC4(s32 *p)
{
    s32 *q = (s32 *)D_80409C90;
    s32 *r = q + 8;

    if (func_80047FD8(q) == 0)
    {
        q = func_8003C714(0, D_80409C90, 0x118);
        if (q == 0)
        {
            __asm__ volatile("");
            func_8007C040(D_804005A0, D_804005C8, 0x151);
            func_8007BEF0(1);
        }
        func_80040560(q, (u32)func_8040861C);
    }
    else
    {
        func_80403D24(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_8007BFE0((u8 *)r, 0x440);
    }
    func_80403AC8((UnkTwinGrp19C *)r, 0x12);
    q[3] = (s32)(r + 0x6A);
    r[0x6D] = func_80403AA0((s32)(r + 0x6E));
    func_80403B84(r, (s32)p);
    q[4] = (s32)func_804034D8;
    func_804034D8(r);
    return q;
}

void func_80403CD4(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80051FBC(*p[0x7C]);
        p++;
    } while (++i < 4);
}

void func_80403D24(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80403CD4((UnkCardGrp184 *)q);
        ((u16 *)q)[-1] |= 0x8000;
        D_8008DAB4_a[0] |= 0x8000;
    }
}

void func_80403D7C(s32 *p, s32 v)
{
    s32 *q;
    s32 t;

    if (p != 0)
    {
        q = p + 8;
        t = q[34];
        q[33] = 0;
        q[32] = 3;
        q[34] = v;
        q[35] = t;
    }
}

/* ---------------------------------------------------------------------------
   THIS UNIT'S `.data`, 0x80408C8C..0x80408CB0.

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

/* 0x80408C8C */
/* D_800282BC's twin. */
u8 *D_80408C8C[4] = {
    (u8 *)0x80400598,
    (u8 *)0x80400590,
    (u8 *)0x80400588,
    (u8 *)0x8040057C,
};

/* 0x80408C9C */
/* D_80027F68's twin: a func_80047358 index list, terminated by the
   first byte that is not below the count the call passes. */
u8 D_80408C9C[0x14] = {
    0x00, 0x01, 0x0A, 0x0B, 0x0C, 0x0D, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
    0x08, 0x09, 0x0E, 0x0F, 0x10, 0x11, 0x12, 0x31};

