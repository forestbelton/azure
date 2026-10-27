#include "common.h"

/* D_800814A0 is declared `extern u32` in structs.h and csrc/unk_12DEC.c passes
   its address to a function taking `u32 *`, so that declaration cannot become
   an incomplete array -- and the scalar spelling gets the assembler-expanded
   symbolic form, one `lui` per access. func_8004EE90 and func_8004F52C need
   the compiler-split form (one `lui` shared by the load and the store), which
   only the incomplete array gives, while func_8004B500 in this same unit needs
   the symbolic one. One carved unit standing in for several original
   translation units is why both appear against one symbol: an `__asm__` alias
   gives those two functions the array spelling under a different C name,
   without touching the declaration unk_12DEC depends on. */
extern u32 D_800814A0a[] __asm__("D_800814A0");

/* Bare file-scope size records: they put each symbol in maspsx's sdata_entries
   while DEFINING nothing, so .sdata stays 0 bytes, the symbols stay undefined
   and the linker script's own assignments resolve them -- and maspsx rewrites
   every load and store of them in this unit to %gp_rel, which is what these
   functions' listings have. Address materialisation stays absolute, so
   D_80080B18 is deliberately NOT here: func_80048DA0 indexes it through a
   materialised base. Adding a symbol here changes every access to it in this
   whole unit, so check `grep` before adding one. */
__asm__(".sdata\n\t.size D_80081550,4\n\t.size D_80081554,4\n"
        "\t.size D_80081558,4\n\t.size D_80080B94,4\n"
        "\t.size D_80080B98,4\n\t.size D_80081540,4\n"
        "\t.size D_80081568,4\n\t.size D_8008156C,4\n"
        "\t.size D_80080B58,4\n\t.size D_80080B5C,4\n"
        "\t.size D_80080B60,4\n\t.size D_80080B64,4\n"
        "\t.size D_80080B80,4\n\t.size D_80080B84,4\n"
        "\t.size D_80080B1B,1\n\t.size D_80080B1C,1\n"
        "\t.size D_80080B30,1\n\t.text");

void func_80048C3C(s32);

void func_80048D40(void) {
    func_80048C3C(2);
}

void func_8004B568(u8 *);

void func_80048D60(u8 *p) {
    u16 f = *(u16 *) (p - 2);

    if ((f & 0x2000) != 0) {
        *(u16 *) (p - 2) = f & 0xDFFF;
        *(u32 *) (p - 0x10) = (u32) D_800212DC;
        func_8004B568(p);
    }
}

u8 func_80048DA0(s32 i, s32 n) {
    D_80080B1B = n + 5;
    D_80080B1C = n * 2 + 5;
    return D_80080B18[i];
}

u32 func_80048DCC(s32 i) {
    return D_80071250[i];
}

u8 func_80048DE8(s32 i) {
    return D_80071298[i];
}

u8 *func_80048E00(u8 *p, u8 *q)
{
    s32 h;

    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x10;
    *(s16 *)(p + 6) = 0;
    p += 0xC;

    p[1] = 0x3A;
    *(u32 *)(p + 4) = 0x201808;
    *(u32 *)(p + 0xC) = 0x602820;
    *(u32 *)(p + 0x10) = 0x201808;
    *(u32 *)(p + 0x14) = 0x602820;
    h = *(s16 *)(q + 4) / 2;
    p[0xA] = h;
    p[0xB] = q[6];
    p[2] = q[0];
    p[3] = q[2];
    p += 0x18;

    p[1] = 0x3A;
    *(u32 *)(p + 4) = 0x602820;
    *(u32 *)(p + 0xC) = 0x201808;
    *(u32 *)(p + 0x10) = 0x602820;
    *(u32 *)(p + 0x14) = 0x201808;
    p[0xA] = h;
    p[0xB] = q[6];
    p[2] = q[0] + h;
    p[3] = q[2];
    p += 0x18;

    *(s16 *)(p + 6) = 1;
    p[1] = 0x81;
    *(s16 *)(p + 4) = 0x10;
    return p - 0x3C;
}

/* `*pp = f(...)` and then `b = *pp`: cse forwards the load to the value just
   stored, which is the target's `addu $s0, $v0, $zero` reusing the parameter's
   register. Assigning the call result to `b` first and storing that keeps both
   live and costs a third callee-saved register (41 DIFF). */
u8 *func_80048EE4(u8 **pp, s32 n) {
    u8 **b;
    u8 *e;
    s32 i;

    *pp = (u8 *) func_8004B404(0x104);
    b = (u8 **) *pp;
    if (b != 0) {
        e = (u8 *) (b + n);
        for (i = 0; i < n; i++) {
            b[i] = e;
            e += 0x10;
        }
        for (i = 0; i < n; i++) {
            *(u8 **) (b[i] + 4) = e;
            e += 0x10;
        }
        func_8004CC38(b, n);
    }
    return (u8 *) b;
}

s32 func_80048F8C(u32 *p, s32 n) {
    s32 i = 0;

    while (i < n) {
        if (*p == 0) {
            break;
        }
        i++;
        p++;
    }
    return i;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80048FBC);

/* Builds a 0x104-byte row table: clears the old one, allocates, grows the two
   NUL-terminated pointer lists that follow p[0], copies each list into one
   half of a 32-word stack buffer, then fills the table with
   `buf[i / 16] + (i % 16) * 16` and chains the second half at +4 of each row.

   ONE buffer again, with the second list at `d[0x10]` so the store keeps 0x40
   as its displacement off `$sp + 0x10`, and loop C reads `buf[k]` and
   `buf[k + 0x10]` off one shared address for the same reason.

   Both copy loops are an explicit `*p != 0` guard round a `do`/`while`,
   because the cursor `d` has to be initialised in the preheader -- here in
   the guard branch's own delay slot (see matching-levers section 8). Loop C
   is a plain `for`: its buffer base is loop-invariant and used only inside the
   body, so loop-invariant motion puts it in the preheader by itself, and the
   row cursor is loop.c's giv for `r[i]`.

   `p++` goes BEFORE the `r != 0` test, not inside it. reorg fills a delay slot
   from the insn immediately preceding the branch first, so with `p++` inside
   the arm the branch takes `sw $s1, 0x0($s0)` into its slot and the store and
   the test come out swapped. Advancing the cursor ahead of the test puts
   `addiu $s0, $s0, 0x4` in that position instead, which is what the listing
   has.

   The two shifts are UNSIGNED -- `(u32) n / 16` and `(u32) i / 16` -- because
   both come out as a bare `srl`, while `i` and `n` are signed for the `blez`
   guard and the `slt` bound. */
u8 *func_80049004(u8 **p, s32 n, s32 m)
{
    u32 buf[0x20];
    u8 *r;
    s32 i;

    bzero(p[0], m * 4);
    r = (u8 *) func_8004B404(0x104);
    p[0] = r;
    p++;
    if (r != 0)
    {
        if (func_8004B42C((u32 *) p, (u32) n / 16 + 1) == 0)
        {
            r = 0;
            goto out;
        }
        if (*p != 0)
        {
            u32 *d = buf;

            do
            {
                *d = (u32) *p;
                p++;
                d++;
            } while (*p != 0);
        }
        if (func_8004B42C((u32 *) p, (u32) n / 16 + 1) == 0)
        {
            r = 0;
            goto out;
        }
        if (*p != 0)
        {
            u32 *d = buf;

            do
            {
                d[0x10] = (u32) *p;
                p++;
                d++;
            } while (*p != 0);
        }
        for (i = 0; i < n; i++)
        {
            u8 *x = (u8 *) (buf[(u32) i / 16] + (i & 0xF) * 16);

            ((u8 **) r)[i] = x;
            *(u32 *) (x + 4) = buf[(u32) i / 16 + 0x10] + (i & 0xF) * 16;
        }
        func_8004CC38((u8 **) r, n);
    }
out:
    return r;
}

s32 func_80048F8C(u32 *, s32);
u32 *func_8004B42C(u32 *, s32);

u8 *func_80049150(u8 *p, u32 n, s32 c) {
    s32 i = func_80048F8C((u32 *) p, c);
    u8 *q = p + i * 4;

    if (func_8004B42C((u32 *) q, n / 21 + 1) == 0) {
        q = 0;
    }
    return q;
}

void func_800491CC(u32 **p, u32 v, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        (*p++)[2] = v;
    }
}

/* An explicit guard plus a do-while, not a `while`: with the `while`, cse
   shares the bottom test's `lbu` into the top of the body across the back edge
   and the body has two reads where the target has three (21 words of 23). */
u32 func_800491F4(s32 t, u8 *p, s32 n) {
    u32 *tt = (u32 *) t;
    u32 prev = 0;

    if (*p < n) {
        do {
            *(u32 *) (tt[*p] + 0xC) = prev;
            prev = tt[*p];
            p++;
        } while (*p < n);
    }
    return prev;
}

void func_80049250(u8 *p, u8 *n) {
    while (*(u8 **) (p + 0xC) != 0) {
        p = *(u8 **) (p + 0xC);
    }
    *(u8 **) (p + 0xC) = n;
}

s32 func_80049280(s32 a) {
    return a == 0x14;
}

s32 func_80049280(s32);

s32 func_8004928C(u8 *p) {
    return func_80049280(p[3] & 0x1F);
}

/* csrc/unk_154BC.c declares this symbol `u8 *D_800E3DF0[]`; here it is read as
   a word table, under an alias so that declaration can stay. */
extern s32 D_800E3DF0w[] __asm__("D_800E3DF0");
s32 func_80049280(s32);
u32 func_80021300(u32);
u32 func_8002137C(s32);

u32 func_800492B0(u32 i) {
    u32 h;
    s32 r;

    if (func_80049280(i) != 0) {
        h = *(s32 *) (D_800814A8 + 0x124);
    } else {
        h = D_800E3DF0w[i];
    }
    r = func_80021300(h);
    if (r != 2) {
        h = func_8002137C(r);
    }
    return h;
}

u32 func_800492B0(u32);

u32 func_80049330(u8 *p) {
    if (p[0] != 0) {
        return func_800492B0(p[3] & 0x1F);
    } else {
        return D_800814A8;
    }
}

s32 func_800494FC(u8 *);
void func_8004DD2C(u8 *);

/* The index is spelled out in each arm instead of via a shared `n`: with
   `n = p[0] * 5` / `* 3` and one `n * 4` after the join, the two quantities
   land in each other's registers (11 DIFF). do_cross_jump merges the common
   tail either way, so the per-arm spelling costs nothing.

   `s = D_80083D38` goes AFTER the second strcpy: that puts its `addu` in the
   call's delay slot. Assigning before costs a word. */
void func_80049374(u8 *p, u8 *q)
{
    u8 *s;

    if (p == 0 || p[1] == 0)
    {
        s = *(u8 **)(q + 4);
    }
    else if (func_800494FC(p) != 0)
    {
        s = *(u8 **)q;
    }
    else
    {
        Unk80073414 *b = D_80073414;
        Unk80073414 *e;

        e = &b[p[1]];

        if (e->unk_00 == 0)
        {
            s = *(u8 **)(e->unk_0C + p[0] * 20 + 8);
        }
        else
        {
            s = *(u8 **)(e->unk_0C + p[0] * 12 + 8);
        }
        if (p[1] == 0x13 && func_8004928C(p) != 0)
        {
            strcpy((char *)D_80083D38, (char *)s);
            strcpy((char *)rindex((char *)D_80083D38, 0) - 0x12,
                   (char *)D_800713F8[0]);
            s = D_80083D38;
        }
    }
    func_8004DD2C(s);
}

void func_80049490(u8 *a)
{
    func_80049374(a, D_800713FC);
}

s32 func_800B8C90(void);

void func_800494B4(u8 *p)
{
    u8 *q;

    /* One call after the join, not a call in each arm: in each arm the
       argument setup for p is duplicated and the jal's delay slot goes empty
       (14 DIFF). */
    if (func_800B8C90() != 0) {
        q = D_80080B34;
    } else {
        q = D_80080B40;
    }
    func_80049374(p, q);
}

s32 func_800494FC(u8 *p) {
    return func_80042A80(p) & 0x400;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004951C);

s32 func_8004951C(u8 *, s32);

u8 *func_80049790(s32 a) {
    u8 *p = (u8 *) func_8004B404(0x104);

    if (p != 0) {
        if (func_8004951C(p, a) == 0) {
            func_8004B1A4((s32) p);
            p = 0;
        }
    }
    return p;
}

/* The `!=` arm first, so the test branches forward INTO the `k` arm as the
   target's `beq` does -- the other way round the `*p = 0` block needs a `j`
   to reach the epilogue and the function is a word long. And the last read of
   `p[1]` goes into its OWN local: reusing `e` makes one allocno for all four
   reads and it keeps $v1, where the target coalesces the last one onto $a0,
   the dead parameter's register. */
void func_800497F4(u32 *p, s32 k) {
    u8 *e;
    u8 *f;

    if (p[0] == 0) {
        return;
    }
    e = (u8 *) p[1];
    if ((*(u32 *) e << 8) != 0x80808000) {
        e[0] -= 0x10;
        ((u8 *) p[1])[1] -= 0x10;
        ((u8 *) p[1])[2] -= 0x10;
        f = (u8 *) p[1];
        if ((*(u32 *) f << 8) == 0x80808000) {
            *(u32 *) f = *(u32 *) D_80080B54;
        }
    } else if (k != 0) {
        p[0] = 0;
    }
}

typedef struct Unk498A0 {
    u8 unk_00[8];
} Unk498A0;

u8 func_800498A0(u8 *p) {
    u8 t[8];

    /* A byte-aligned 8-byte struct assignment: two lwl/lwr and two swl/swr
       pairs, not four word moves. */
    *(Unk498A0 *) t = *(Unk498A0 *) D_80080B20;
    return t[*(u32 *) (p + 0x14) & 7];
}

u32 func_800498EC(s32 i) {
    return D_8007361C[D_800712AC[i]];
}

u32 func_80049918(s32 i) {
    return D_8007361C[D_800712B0[i]];
}

s32 func_80049944(u32 a) {
    s32 r = 3;

    if (a & 1) {
        r = 0;
    } else if (a & 2) {
        r = 1;
    } else if (a & 4) {
        r = 2;
    }
    return r;
}

s32 func_80049944(u32);

s32 func_80049984(u8 *p) {
    s32 r = 3;

    if (p[0x13] != 0) {
        r = func_80049944(*(u32 *) (p + 0x14));
    }
    return r;
}

void func_800499BC(void) {
    D_80083D78[0] = -1;
    D_80083D78[2] = -1;
    D_80083D78[4] = -1;
    D_80083D78[6] = -1;
}

void func_800499DC(s32 *p) {
    *p = 1;
}

/* Builds the four-record gradient: two 0x18-byte records with four colour
   words each, a 0xC-byte one in between, and a 0x81 terminator whose +0x00
   gets the end bit. Returns the start of the run, which the listing computes
   as `p - 0x3C` because the walked pointer is the only copy left.

   Four things paid for, 123 DIFF down to a match:

   * **The two templates are INCOMPLETE ARRAYS read `[0]`, not scalars.** 13
     words, all of them register names. `extern CVECTOR D_80080B28;` lets the
     `lui`/`addiu` pair share one register (combine_regs ties the lo_sum to
     the dying high) and the copied word then takes $v0; as an array the
     address is a separate quantity, the high keeps $v0, the word goes to $v1
     and the address to $a2, which is the listing. Same declared-size lever as
     the -G8 one in docs/open-matching-problems.md, seen from the register
     side.

   * **Both colour words go through LOCALS read before the stores.** The
     second `lw` cannot be hoisted over a `sw` through `p` -- sched cannot
     prove $sp+0x18 and $s0+4 distinct -- so written inline the second load
     stalls. Read into `g`/`h` first and both loads come out together.

   * **The third record needs ITS OWN pair of locals.** One `g`/`h` pair for
     both records is one pseudo each, live across the records' blocks, so
     global-alloc pins them to $v0/$v1 for the whole function and the third
     record gets them in the first record's order. Fresh locals are local
     quantities and local-alloc hands them out in load order, which is the
     listing's swap.

   * The second colour test is NESTED inside the first, not sequential: the
     outer branch skips both scalings. The scalings' own `sb`s to the frame
     flush cse, which is why the second test's `lw` of the colour is a real
     re-load and the compiler cannot fold the test away.

   The `/4` and `/2` of `*(s16 *) (s + 6)` are both HImode divides (the front
   end narrows `/ C` -- see matching-levers section 8), which is why the third
   record, where one load feeds both, sign-extends through `sll 16`/`sra 16`
   and the first record, where only `/4` is wanted, uses the `lh` directly. */
extern CVECTOR D_80080B28[];
extern CVECTOR D_80080B2C[];

u8 *func_800499E8(u8 *p, u8 *s, u8 *c)
{
    CVECTOR x;
    CVECTOR y;
    s32 g;
    s32 h;
    s32 i;
    s32 j;

    x = D_80080B28[0];
    y = D_80080B2C[0];
    bzero(p, 0x14);
    if ((*(u32 *) c & 0xFFFFFF) != 0x808080)
    {
        x.r = x.r * c[0] >> 7;
        x.g = x.g * c[1] >> 7;
        x.b = x.b * c[2] >> 7;
        if ((*(u32 *) c & 0xFFFFFF) != 0x808080)
        {
            y.r = y.r * c[0] >> 7;
            y.g = y.g * c[1] >> 7;
            y.b = y.b * c[2] >> 7;
        }
    }
    p[1] = 0x38;
    g = *(s32 *) &x;
    h = *(s32 *) &y;
    *(s32 *) (p + 0x4) = g;
    *(s32 *) (p + 0xC) = g;
    *(s32 *) (p + 0x10) = h;
    *(s32 *) (p + 0x14) = h;
    p[2] = s[0];
    p[3] = s[2];
    p[0xA] = s[4];
    p[0xB] = *(s16 *) (s + 6) / 4;
    p += 0x18;
    p[1] = 0x28;
    *(s32 *) (p + 0x4) = *(s32 *) &y;
    p[2] = s[0];
    p[3] = s[2] + *(s16 *) (s + 6) / 4;
    p[0xA] = s[4];
    p[0xB] = *(s16 *) (s + 6) / 2;
    p += 0xC;
    p[1] = 0x38;
    i = *(s32 *) &y;
    j = *(s32 *) &x;
    *(s32 *) (p + 0x4) = i;
    *(s32 *) (p + 0xC) = i;
    *(s32 *) (p + 0x10) = j;
    *(s32 *) (p + 0x14) = j;
    p[2] = s[0];
    p[3] = s[2] + *(s16 *) (s + 6) / 4 + *(s16 *) (s + 6) / 2;
    p[0xA] = s[4];
    p[0xB] = s[6] - *(s16 *) (s + 6) / 4 - *(s16 *) (s + 6) / 2;
    p += 0x18;
    p[1] = 0x81;
    *(s16 *) (p + 0x4) = 0x10;
    *(s16 *) (p + 0x6) = 1;
    p[0] |= 0x80;
    return p - 0x3C;
}

/* func_800508F0 with the two words as literals instead of globals. Both
   parameters really are pointers: as `s32` with the casts inside, 22 DIFF --
   which is why func_80049D80 and func_80049DC8 now cast at the call. */
u8 *func_80049CF4(u8 *q, u8 *s) {
    *(u32 *) (q + 4) = 0x586868;
    q[1] = 0x48;
    q[2] = s[0];
    q[3] = s[2];
    q[0xA] = s[4];
    q[0xB] = s[6];
    q += 0xC;
    *(u32 *) (q + 4) = 0x98B8B8;
    q[1] = 0x48;
    q[2] = s[0];
    q[3] = s[2];
    q[0xA] = s[4];
    q[0xB] = s[6];
    q[0] = 3;
    return q + 0xC;
}

u8 *func_80049CF4(u8 *, u8 *);

void func_80049D80(u8 *p, s32 b) {
    func_80049CF4(p, (u8 *) b);
    func_800499E8(p + 0x18, b, D_80080B54);
}

u8 *func_80049CF4(u8 *, u8 *);

/* Two arguments that pass through untouched, which is invisible here and shows
   up at func_80049D80, which passes both. */
void func_80049DC8(s32 a, s32 b) {
    func_80049CF4((u8 *) a, (u8 *) b);
}

/* The negative arm is a CONDITIONAL EXPRESSION, not an `if`: the target spends
   a copy into $v0 on each of its two paths and a copy back into $a0 at the
   join, which is what `?:` produces and what a plain `if (a < 0) a = m + a;`
   optimises away (12 words against 13, 6 DIFF). The positive arm is a plain
   `if`, in place. */
s32 func_80049DE8(s32 a, s32 d, s32 m) {
    a += d;
    if (d > 0) {
        if (a >= m) {
            a -= m;
        }
    } else {
        a = (a < 0) ? m + a : a;
    }
    return a;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80049E1C);

u32 func_80049E6C(s32 i) {
    return D_800712B4[i];
}

extern s8 D_800712E4[];
extern u8 D_800712F0[];
s16 func_80042900(s8 *, s8);

/* func_80042900 is declared s16 HERE (it is `s32` in csrc/unk_154BC.c, where
   it is defined): both tests of its result are on the low halfword,
   `sll 16` then `bnez`/`blez`.

   The trailing byte is read into a `u8` and cast back to `s8` for the call.
   That is what the target's `lbu` plus `sll 24`/`sra 24` is -- the loop above
   reads the same array with a plain `lb`, so the declaration is signed and the
   unsigned read is the cast. A `u8` __asm__ alias gives the base a second
   materialisation and costs 14 words. */
u8 func_80049E88(s8 *p) {
    s32 i = 0xB;
    u8 c;

    while (func_80042900(p, D_800712E4[i]) == 0) {
        i--;
        if (i <= 0) {
            break;
        }
    }
    c = D_800712E4[i];
    if (c == 7 && func_80042900(p, (s8) c) > 0) {
        i--;
    }
    return D_800712F0[i];
}

void func_80049F2C(u8 *p) {
    *(u16 *) (p + 0x28) += (*(s16 *) (p + 0x4) - *(s16 *) (p + 0x28)) >> 1;
    *(u16 *) (p + 0x2A) += (*(s16 *) (p + 0x6) - *(s16 *) (p + 0x2A)) >> 1;
}

extern u8 D_80080B30;

/* $gp group: D_80080B30 joined this unit's .sdata size records for this
   function, so it was verified in a scratch file carrying the block. Two
   levers. `u32 *s = D_8002E5D8;` is assigned INSIDE the loop body, so
   loop-invariant motion materialises it in the preheader rather than leaving
   a copy of the %hi pseudo behind (one word, func_8004CCBC's lever). And the
   chain base is assigned inside the loop too and INDEXED rather than walked:
   as a walked `c++` pointer the materialisation is expanded before the `4`'s
   hoisted copy and the two come out in the wrong order (3 DIFF), while
   indexing an incomplete-array alias splits the address into two registers
   (2 DIFF). Assigning `&D_80080B30` inside the loop gives both the one-insn
   `la` and the hoist order. */
void func_80049F68(u8 *p) {
    u32 *t = D_8002E5E8;
    s32 i;
    u8 *c;
    s32 j;
    u8 *d;

    *(u32 *) (p + 0xA0) = D_8002E5E8[0];
    *(u32 *) (p + 0xA4) = t[1];
    *(u32 *) (p + 0xA8) = t[2];
    for (i = 0; i < 4; i++) {
        u8 *r = p + i * 0x20;
        u32 *s = D_8002E5D8;

        *(u32 *) (r + 0x20) = D_8002E5D8[0];
        *(u32 *) (r + 0x24) = s[1];
        *(u32 *) (r + 0x28) = s[2];
        *(u32 *) (r + 0x2C) = s[3];
        *(u32 *) (r + 0x34) = (u32) (p + 0x20 + i * 0x20);
        *(u32 *) (r + 0x38) = (u32) (p + 0xA0);
    }
    d = p + 0x30;
    if (D_80080B30 != 4) {
        j = 0;
        do {
            u8 *x;

            c = &D_80080B30;
            x = (u8 *) ((c[j] << 5) + (u32) p + 0x30);
            j++;
            *(u32 *) (d + 0xC) = (u32) x;
            d = x;
        } while (c[j] != 4);
    }
    *(u32 *) (d + 0xC) = 0;
}

void func_8004A030(u8 *p, s16 x, s16 y, s16 h) {
    u8 *q = p + 0x20;

    *(u32 *) (p + 0x30) = 0;
    *(u16 *) (q + 8) = x - 0xA0;
    *(u16 *) (q + 0xA) = y - 0x80;
    *(u16 *) (q + 0xC) = 0x200;
    q[0xF] = 4;
    q = p + 0x40;
    *(u32 *) (p + 0x50) = *(u32 *) (p + 0x1C);
    *(u16 *) (q + 8) = 0xC;
    *(u16 *) (q + 0xA) = 0xE;
    q = p + 0x60;
    *(u32 *) (p + 0x70) = *(u32 *) (p + 0x14);
    *(u16 *) (q + 8) = h / 2;
    *(u16 *) (q + 0xA) = 8;
    q = p + 0x80;
    *(u32 *) (p + 0x90) = *(u32 *) (p + 0x18);
    *(u16 *) (q + 8) = 0x94;
    *(u16 *) (q + 0xA) = 0x81;
}

u32 func_8004A0C0(u32 m) {
    /* The result, the counter and the pointer initialise in source order, and
       the counter's `addu $a1, $a2, $zero` is cse sharing the result's zero. */
    u32 r = 0;
    s32 i = 0;
    u8 *p = D_800712FC;

    for (; i < 0xD; i++) {
        if ((*(u32 *) (p + 4) & m) != 0) {
            r = *(u32 *) p;
            break;
        }
        p += 8;
    }
    return r;
}

s16 func_8004A10C(u8 *p) {
    s32 n;

    if (p == 0) {
        return 0;
    }
    n = func_8004B4DC(p);
    return *(s8 *) (p + n * 0xC - 0xA) + 0x80;
}

extern u8 D_8007136C[];

void func_8004A170(s32 a, u8 *p, u16 n, s32 f)
{
    s32 h;

    memcpy(p, D_8007136C, 0x60);
    h = n >> 1;
    p[0x0A] = n;
    p[0x16] = n;
    p[0x22] = n;
    p[0x3A] = n;
    p[0x46] = n;
    n = -h;
    p[0x02] = n;
    p[0x0E] = n;
    p[0x1A] = n;
    p[0x32] = n;
    p[0x3E] = n;
}

void func_80049F68(u8 *);
u32 func_8004A0C0(u32);
u8 *func_8004DC14(u8 *, s32);
s16 func_8004A10C(u8 *);
void func_8004A030(u8 *, s16, s16, s16);
void func_8004A170(s32, u8 *, u16, s32);

void func_8004A24C(u8 *p, u32 b, u8 *c, s16 d, s16 e, s16 f)
{
    u8 *r;
    s16 g;

    func_80049F68(p);
    *(u32 *)(p + 0x1C) = func_8004A0C0(b);
    r = func_8004DC14(c, 1);
    *(u8 **)(p + 0x18) = r;
    g = f;
    if (f == -1)
    {
        g = func_8004A10C(r) + 0x20;
    }
    func_8004A170((s32)p, *(u8 **)(p + 8), g & 0xFFFF, *(u16 *)p & 1);
    *(u32 *)(p + 0x14) = *(u32 *)(p + 8);
    func_8004A030(p, d, e, g);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A330);

/* Both load-delay slots are filled in the target, and cc1psx's fill pass only
   ever swaps a load with the insn IMMEDIATELY after it: so the `addiu` for the
   +0x20 base has to stand right behind the `lhu`, and the first argument's
   load right behind the global's load. That is this statement order -- the
   flag update, then the base, then the argument read into a local, then the
   global. Any other order leaves cc1psx emitting its own `#nop` after the
   `lhu` and the function is a word long (14 DIFF over six orderings). */
void func_8004A464(u8 *p) {
    u8 *q;
    u8 *v;

    if (p != 0) {
        *(u16 *) (p + 0x1E) |= 0x8000;
        q = p + 0x20;
        v = *(u8 **) (q + 8);
        D_800814A0 |= 0x8000;
        func_8004B530(v);
        func_8004B530(*(u8 **) (q + 0x18));
    }
}

s32 func_8004A4C4(s32 arg0, u8 *p) {
    return func_800438E4(p);
}

s32 func_8004A4E4(s32 a, u8 *p) {
    a += ((u32) a / 10) * *(s8 *) (p + 2);
    if ((p[3] & 0x40) != 0) {
        a += a * 4;
    }
    if (a <= 0) {
        a = 1;
    }
    return a;
}

typedef struct Unk800102F0 {
    u8 pad_00[0x11];
    u8 unk_11;
    u8 pad_12[0x42];
} Unk800102F0;

u32 func_8004A534(u32 a, u8 *p) {
    /* A struct at the constant base, so the +0x11 stays in the load's
       displacement instead of folding onto the constant. */
    return a + (a >> 2) * ((Unk800102F0 *) 0x800102F0)[p[3] & 0x1F].unk_11;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004A574);

void func_8004A574(s32, s32);

void func_8004A618(s32 a) {
    func_8004A574(a, 1);
}

void func_8004A574(s32, s32);

void func_8004A638(s32 a) {
    func_8004A574(a, 0);
}

/* The tail is written TWICE, once per arm, and jump2's cross-jumping merges
   it back into the single tail the listing shows -- so the duplication is
   invisible in the output but decides the registers. Written with a join
   block, the index is a global allocno that conflicts with the $v0 that
   local-alloc gives the `lw` in the join; written per arm, the whole tail is
   block-local, the index chain keeps $v0 and the load falls to $v1 -- which is
   also where `p` ends up, dying into its own load. 29 DIFF with every opcode
   already right.

   The base pointers are read into locals first (`b`, `t`) so that each symbol
   is materialised before the index that uses it: `&D_80073414[i]` alone folds
   to index+ADDR_EXPR, and the ADDR_EXPR is TREE_CONSTANT, so fold puts it
   second and the lui/addiu come out after the shifts. */
u32 func_8004A658(s32 i, s32 j) {
    u32 *t = D_8007361C;
    Unk80073414 *b = D_80073414;
    Unk80073414 *p = &b[i];

    if (p->unk_00 != 0) {
        return t[*(s16 *) (p->unk_0C + (j * 2 + j) * 4 + 2)];
    }
    return t[*(s16 *) (p->unk_0C + (j * 4 + j) * 4 + 2)];
}

/* The scan is INDEXED by the counter off a loop-invariant constant base, not a
   pointer walked with `p += 4`. Both spellings give the same 16 words, but a
   walked pointer whose only uses are the two loads is eliminated by loop.c,
   which biases the replacement giv's initial value to the second field
   (`ori 0x3566`, `lh -2`, `lh 0`) -- one word long and the whole reason this
   family sat parked. Indexed, the giv is `base + i * 4` and the two field
   offsets stay as the loads' displacements, which is what the listing shows:
   a bare `lui 0x8001` (0x80010000 has no low half) with 0x3564/0x3566 in the
   loads and `addiu 4` in the branch delay slot.

   s32 parameters, not s16: the target compares the `lh` results against $a0
   and $a1 as they arrive, with no sign-extension of its own. */
s32 func_8004A6C0(s32 x, s32 y) {
    u8 *b = (u8 *) 0x80010000;
    s32 i = 0;

    for (; i < 0x14; i++) {
        if (*(s16 *) (b + i * 4 + 0x3564) == x) {
            if (*(s16 *) (b + i * 4 + 0x3566) == y) {
                break;
            }
        }
    }
    return i;
}

s32 func_8004A6C0(s32, s32);

/* The zero that initialises the returned pointer is also both arguments of the
   scan -- one `addu $s0, $zero, $zero` serves all three. The halfwords go to
   the constant base 0x80010000 with 0x3564/0x3566 as displacements, and the
   base is declared INSIDE the if so its `lui` lands there. */
u8 *func_8004A700(s32 x, s32 y) {
    u8 *r = 0;
    s32 i = func_8004A6C0(0, 0);

    if (i < 0x14) {
        u8 *b = (u8 *) 0x80010000;

        r = (u8 *) 0x800133E8 + i * 19;
        *(s16 *) (b + i * 4 + 0x3564) = x;
        *(s16 *) (b + i * 4 + 0x3566) = y;
    }
    return r;
}

s32 func_8004A6C0(s32, s32);

/* 20 entries of 19 bytes from 0x800133E8, the other half of the table
   func_8004A6C0 scans. */
u8 *func_8004A784(s32 x, s32 y) {
    u8 *r = 0;
    s32 i = func_8004A6C0(x, y);

    if (i < 0x14) {
        r = (u8 *) 0x800133E8 + i * 19;
    }
    return r;
}

s32 func_8004A6C0(s32, s32);

/* The two halfwords are at the CONSTANT base 0x80010000 with their offsets
   written inside the index expression -- 0x80010000's low half is zero, so the
   base is a bare `lui`, and 0x3564/0x3566 stay as the stores' displacements
   (splat prints them as %lo(D_80013564) and %lo(D_80013566) because both
   addresses are in its symbol list). Declaring the table as a symbol instead,
   in any of five spellings including a struct array and a one-set pointer
   local, always materialises the whole address with an `addiu %lo` and is a
   word long: cse folds a %lo into a MEM only for a CONSTANT offset off the
   `(high)` pseudo, never past a register index. 13 DIFF. */
void func_8004A7D8(s32 x, s32 y) {
    s32 i = func_8004A6C0(x, y);
    u8 *b = (u8 *) 0x80010000;

    if (i < 0x14) {
        bzero((u8 *) 0x800133E8 + i * 19, 0x13);
        *(s16 *) (b + i * 4 + 0x3564) = 0;
        *(s16 *) (b + i * 4 + 0x3566) = 0;
    }
}

u8 *func_8004A784(s32, s32);
u8 *func_8004A700(s32, s32);

/* Nested, not an early `return r;`: with the early return gcc merges the two
   `$v0 = $s0` copies and the function is a word short. Both parameters are
   s32 -- as s16 each one costs its own sll/sra pair, which is also why
   func_8004A700's own parameters are now s32 (it stores them with `sh`, so
   either spelling matches there). */
u8 *func_8004A83C(s32 a, s32 b) {
    u8 *r = func_8004A784(a, b);

    if (r == 0) {
        r = func_8004A700(a, b);
        if (r != 0) {
            bzero(r, 0x13);
            *(u32 *) (D_80073414[a].unk_0C + b * 20 + 0xC) = (u32) r;
        }
    }
    return r;
}

s32 func_8004A6C0(s32, s32);

/* The constant base is declared INSIDE the if, so its `lui` is the body's
   first insn and reorg lifts it into the `beqz`'s delay slot. At function
   scope the index comes out first instead (fold puts the constant second) and
   the two insns are in the other order -- 4 DIFF, with the call's result in
   $v1 rather than $a0 as a knock-on. */
void func_8004A8D8(s32 x, s32 y) {
    s32 i = func_8004A6C0(x, y);

    if (i < 0x14) {
        u8 *b = (u8 *) 0x80010000;

        *(s16 *) (b + i * 4 + 0x3564) = 0;
        *(s16 *) (b + i * 4 + 0x3566) = 0;
    }
}

void func_8004A918(void) {
    bzero((void *) 0x80013564, 0x50);
}

void func_8004A940(void) {
    s16 *k = (s16 *) 0x80013564;
    u8 *e = (u8 *) 0x800133E8;
    s32 i = 0;

    for (; i < 0x14; i++) {
        if (k[0] != 0) {
            *(u32 *) (D_80073414[k[0]].unk_0C + k[1] * 20 + 0xC) = (u32) e;
        }
        k += 2;
        e += 0x13;
    }
}

s32 func_800A6D30(void);

void func_8004A9B0(u8 *p, s32 n) {
    s32 i = n - 1;

    while (i > 0) {
        s32 j = (func_800A6D30() & 0xFFFF) / (0x7FFF / (i + 1) + 1);
        u8 t = p[i];

        p[i] = p[j];
        p[j] = t;
        i--;
    }
}

s32 func_8004AA34(s32 *a, s32 *b) {
    return *a - *b;
}

s32 func_8004AA34(s32 *, s32 *);
void func_8004AFC8(s32, s32, s32, void *);

void func_8004AA44(s32 a, s32 b) {
    func_8004AFC8(b, a, 4, func_8004AA34);
}

/* Shuffles the nonzero +0x0C pointers of D_80073414[a]'s 0x14-stride table
   among themselves: collects them into the stack buffer's word half, fills the
   byte half with 0..k-1, shuffles those bytes with func_8004A9B0, then writes
   them back through the permutation.

   ONE buffer, reached at +0x40, not two arrays. `u8 b[0x140]` with the words
   at `b + 0x40` keeps 0x40 as the store's displacement off a base of
   `$sp + 0x10`; two locals (`u8 idx[0x40]; u32 vals[0x40];`) would put the
   second at `$sp + 0x50` with displacement 0. Same reason as the giv rule in
   matching-levers section 9: the constant only survives on the access when
   the base it is added to is a register.

   The first loop is an EXPLICIT `n != 0` guard around a `do`/`while`, not a
   `for`. The cursor `d` has to be initialised in the PREHEADER -- the listing
   has `addiu $v1, $sp, 0x10` after the guard branch, next to loop.c's own giv
   init for `q` -- and a for-init is evaluated before the guard, where sched2
   then floats it to the top of the entry block (4 words). `if (n != 0)`
   wrapped round a `for` gets the placement right but pays for a second test.
   `i = 0` stays OUTSIDE the guard so that `k = 0` keeps the function's first
   slot and reorg fills the guard's delay slot with `i = 0` instead.

   The third loop reuses `k`, reset after the call, rather than a fresh
   counter. That is what decides its registers: one allocno across both loops
   outranks the hoisted buffer base, keeps $a1, and leaves $a0 for the base;
   a separate `c` is allocated first and takes $a0, which is 6 wrong words.
   The two operand orders in the body are deliberate -- `b[k]` for the byte
   read (base first, `addu $v0, $a0, $a1`) and `b[k] * 4 + (u32) b` for the
   word (index first, `addu $v0, $v0, $a0`). */
void func_8004AA78(s32 a)
{
    u8 b[0x140];
    Unk80073414 *t;
    Unk80073414 *e;
    u8 *p;
    s32 n;
    s32 i;
    s32 k;

    k = 0;
    t = D_80073414;
    e = &t[a];
    n = e->unk_02;
    p = e->unk_0C;
    i = 0;
    if (n != 0)
    {
        u8 *q = p;
        u8 *d = b;

        do
        {
            if (*(u32 *) (q + 0xC) != 0)
            {
                *(u32 *) (d + 0x40) = *(u32 *) (q + 0xC);
                d += 4;
                k++;
            }
            q += 20;
            i++;
        } while (i < n);
    }
    for (i = 0; i < k; i++)
    {
        b[i] = i;
    }
    func_8004A9B0(b, k);
    k = 0;
    for (i = 0; i < n; i++)
    {
        if (*(u32 *) (p + i * 20 + 0xC) != 0)
        {
            *(u32 *) (p + i * 20 + 0xC) = *(u32 *) (b[k] * 4 + (u32) b + 0x40);
            k++;
        }
    }
}

void func_8003E1FC(void);
void func_8003F320(void);
void func_800A6D60(s32);
void func_800A6D98(u32);
void func_8004AA78(s32);

/* D_80012090 is read inside the loop, not hoisted: the call in the body
   invalidates it, and the load sits ahead of the call. */
void func_8004AB7C(void) {
    u8 *e;
    s32 i = 0;

    func_8003E1FC();
    func_8003F320();
    func_800A6D60(1);
    func_800A6D98(D_80013628);
    e = (u8 *) D_80073414;
    for (; i < 0x14; i++) {
        if (((e[1] >> D_80012090) & 1) != 0) {
            func_8004AA78(i);
        }
        e += 0x14;
    }
    func_8004A940();
    func_800A6D60(0);
}

u32 func_8004AC18(s32 i) {
    return D_8007359C[0][i].unk_04;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004AC3C);

s32 func_8004AE3C(u32 *p) {
    s32 n = 0;

    while (*p != 0) {
        p++;
        n++;
    }
    return n;
}

s32 func_8004AE68(u8 **a, u8 **b) {
    return D_800713E4[(*a)[1]] - D_800713E4[(*b)[1]];
}

s32 func_8004AE98(u8 **a, u8 **b) {
    return D_80081540[(*a)[0]] - D_80081540[(*b)[0]];
}

/* Selection sort over `n` records of `size` bytes with a caller-supplied
   comparison, swapping through a 0x100-byte stack buffer. The three memcpys
   run even when the minimum is already in place.

   Two spellings are load-bearing, and both are allocation, not shape -- the
   straightforward version is 65 words with the right structure and sixteen
   wrong registers.

   `p = base` BEFORE the `n == 0` test. combine folds the copy into the
   parameter's own hard-register copy by substituting into the LATER insn and
   deleting the earlier one, so a0's copy ends up at the assignment's position
   -- last of the four instead of first -- and sched2 interleaves the prologue
   saves in that order: $s6, $s5, $fp, $s3. Walking the parameter itself puts
   a0's copy first (8 wrong words). Putting the assignment AFTER the test
   overshoots: the copy leaves the entry block altogether and comes out behind
   the saves and the guard branch.

   `m = p;` then `j = i + 1;` then `q = p + size;`, with the inner loop a
   `while` rather than a `for` carrying j's initialiser. j and q tie at
   global-alloc to within 0.6% -- floor_log2(13) * 13 / 12 against
   floor_log2(14) * 14 / 13 -- so one insn of live range decides which takes
   $s0, and the init order is what supplies it. j initialised first and the
   listing's registers appear but `m = p` lands a slot late (10 DIFF); j's
   initialiser left in the `for` gives j $s0 and q $s1, the wrong way round
   (16 DIFF). */
void func_8004AEC4(u8 *base, s32 n, s32 size, s32 (*cmp)(u8 *, u8 *))
{
    u8 tmp[0x100];
    s32 i;
    s32 j;
    u8 *p;
    u8 *m;
    u8 *q;

    p = base;
    if (n == 0)
    {
        return;
    }
    for (i = 0; i < n - 1; i++)
    {
        m = p;
        j = i + 1;
        q = p + size;
        while (j < n)
        {
            if (cmp(m, q) > 0)
            {
                m = q;
            }
            q += size;
            j++;
        }
        memcpy(tmp, p, size);
        memcpy(p, m, size);
        memcpy(m, tmp, size);
        p += size;
    }
}

/* A forwarding wrapper: its four arguments pass through untouched, which is
   invisible in the listing (no register setup before the jal) and only shows
   up at a caller -- func_8004AA44 passes four. */
void func_8004AFC8(s32 a, s32 b, s32 c, void *d) {
    func_8004AEC4((u8 *) a, b, c, (s32 (*)(u8 *, u8 *)) d);
}

/* `k` is a u32, not a u8: declared u8 the byte gets an `andi 0xFF` it does not
   need, since the `lbu` already zero-extended it (17 words of 16). */
s32 func_8004AFE8(u8 **p) {
    u32 k = (*p)[1];
    s32 n = 0;

    while (p != NULL && (*p)[1] == k) {
        p++;
        n++;
    }
    return n;
}

void func_8004B028(u8 *m, u8 **list, s32 k) {
    s32 i = 0;
    u8 *e;
    u8 *q;

    while (*list != 0) {
        e = *list;
        if (e[1] == k) {
            q = m + e[0];
            if (*q == 0) {
                *q = i + 1;
            }
        }
        list++;
        i++;
    }
}

/* `copy` is an array of 0x40 pointers and `m` a 0x80-byte table, in that
   declaration order: reversing them swaps the two stack offsets. The NULL
   terminator is written as `copy[n]`, which shares memcpy's `n * 4` through
   cse -- one `sll` feeds both the size argument and the store's base.

   The `list == NULL` test is INSIDE the body, after the `*list != 0` loop
   condition, which is why only the latter appears twice (jump.c duplicates
   the loop test to the back edge, the body's test stays single). */
void func_8004B08C(u8 **list)
{
    u8 *copy[0x40];
    u8 m[0x80];
    s32 n;
    s32 last;

    D_80081540 = m;
    n = func_8004AE3C((u32 *)list);
    memcpy(copy, list, n * 4);
    copy[n] = NULL;
    func_8004AFC8((s32)list, n, 4, (void *)func_8004AE68);
    last = 0;
    while (*list != 0)
    {
        if (list == NULL)
        {
            break;
        }
        if ((*list)[1] != last)
        {
            last = (*list)[1];
            bzero(m, 0x80);
            n = func_8004AFE8(list);
            func_8004B028(m, copy, last);
            func_8004AFC8((s32)list, n, 4, (void *)func_8004AE98);
            list += n;
        }
    }
}

/* s32 rather than u8 *, because the two callers 280 lines below already
   declare it that way and pass their own s32 parameter straight through. */
void func_8004B1A4(s32 a) {
    if (a != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Indexed, so the walked pointer is a giv loop.c creates in the PREHEADER --
   after the `blez` -- where `p++` on the parameter puts the copy in the
   prologue instead (8 DIFF, the register saves in the other order). */
void func_8004B1DC(s32 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        if (p[i] != 0) {
            func_8004B1A4(p[i]);
            p[i] = 0;
        }
    }
}

void func_8004B248(u32 *p) {
    while (*p != 0) {
        func_8004B1A4(*p);
        p++;
    }
}

void func_8004B298(s32 a, s32 b) {
    u32 *q = *(u32 **) (b + 4);

    if (q[0] == 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_8004B2E0(s32 a, s32 b) {
    s32 *n = (s32 *) b;

    /* `!= 0`: the flag goes in while the count is still nonzero. */
    *n -= 1;
    if (*n != 0) {
        *(u16 *) (a - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_8004B298(s32, s32);
void func_8004B2E0(s32, s32);

void func_8004B324(s32 a, s32 b) {
    func_8004B298(a, b);
    func_8004B2E0(a, b);
}

void func_8004B324(s32, s32);

/* Three parameters, as both callers declare it, though the third only passes
   through to func_8003FC64 (whose own $a0 this zeroes). One return of the node
   pointer, advanced in place -- `return (u8 *)(n + 1)` from two places
   computes `$v1 + 0x20` twice, once into each guard's delay slot, and is a
   word short. */
u32 func_8004B364(s32 a, s32 b, s32 c) {
    UnkNode *n = func_8003FC64(0);

    if (n != NULL) {
        if (a != 0) {
            if (b != 0) {
                u8 *e = (u8 *) n->unk_08;

                *(s32 *) e = b;
                *(s32 *) (e + 4) = a;
                n->unk_10 = (u32) func_8004B324;
            }
        }
        n++;
    }
    return (u32) n;
}

u32 func_8004B364(s32, s32, s32);

void func_8004B3DC(s32 a) {
    func_8004B364(0, 1, a);
}

u32 func_8004B364(s32, s32, s32);

/* Returns what func_8004B364 returns: func_80048FBC stores the result. */
u32 func_8004B404(s32 a) {
    return func_8004B364(0, 0, a);
}

u32 *func_8004B42C(u32 *p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        u32 v = func_8004B404(0x82);

        p[i] = v;
        if (v == 0) {
            func_8004B248(p);
            p = 0;
            break;
        }
    }
    return p;
}

UnkNode *func_8004B4A8(UnkNode *target) {
    UnkNode *p = D_80081498;

    while (p != 0) {
        if (p == target) {
            break;
        }
        p = *(UnkNode **) p;
    }
    return p;
}

s32 func_8004B4DC(u8 *p) {
    s32 n = 0;
    u8 v;

    /* The advance is inside the loop body and BEFORE the test, which is how it
       ends up in the backward branch's delay slot and so runs on the exiting
       iteration too. Latching the byte in `v` is what lets it. */
    do {
        n++;
        v = *p;
        p += 0xC;
    } while ((v & 0x80) == 0);
    return n;
}

void func_8004B500(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0;
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0 = g | 0x8000;
    }
}

void func_8004B530(u8 *p) {
    if (p != 0) {
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The rect's y is `0x100 + i * 0x80`, not a running counter: loop.c reduces
   it to the walked value the target initialises in the preheader AFTER the
   three hoisted constants, where a `y` variable of its own is expanded before
   them and the two registers swap with the loop counter (12 DIFF). */
void func_8004B568(u8 *unused) {
    RECT r;
    s32 i;
    u8 *q;

    q = (u8 *) 0x80016000;
    DrawSync(0);
    D_80081480 = D_8008148C;
    for (i = 0; i < 2; i++) {
        r.y = 0x100 + i * 0x80;
        r.x = 0x140;
        r.w = 0x40;
        r.h = 0x80;
        StoreImage(&r, (u32 *) D_8008148C);
        DrawSync(0);
        q = func_8004068C((u8 *) D_80081480, q);
    }
}

/* The sized alias, not the incomplete `D_80080B48[]`: at -G8 an object of 8
   bytes or fewer is small data, so its address is the ONE-insn `la` macro that
   maspsx splits afterwards -- and a pair maspsx made cannot be scheduled
   apart. Through the incomplete array the compiler splits it itself and sched1
   puts the `addiu $a0, 6` between the `lui` and the `addiu`, which is the 2
   DIFF this sat at. (The symbol is not in this unit's sdata_entries, so the
   expansion is absolute, not $gp-relative.) */
extern u8 D_80080B48s[8] __asm__("D_80080B48");

void func_8004B634(void) {
    func_8003E4FC(6, D_80080B48s, 0);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B660(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 1, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B6BC(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 2, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B718(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 0, c);
}

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B774(s32 a, s32 b, s32 c) {
    func_8004B634();
    func_8003F320();
    func_800253A0(a, b, 3, c);
}

typedef struct Unk4B7D0 {
    u8 unk_00[4];
} Unk4B7D0;

void func_8004B634(void);
void func_8003F320(void);
void func_800253A0(s32, s32, s32, s32);

void func_8004B7D0(s32 a) {
    u8 t[4];

    *(Unk4B7D0 *) t = *(Unk4B7D0 *) D_80080B50;
    func_8004B634();
    func_8003F320();
    func_800253A0(a, 0, 0, (s32) t);
}

void func_8004B634(void);

void func_8004B834(void) {
    func_8004B634();
}

void func_8004B834(void);
void func_8003F320(void);
void func_8002537C(s32, s32, s32, s32, s32);

/* func_8004B834 takes no arguments. Passing a0/a1/a2 through to it costs no
   instructions either way -- the moves are deleted as no-ops -- but while they
   exist they are REFERENCES that lengthen each parameter's live range, and
   global-alloc's priority is refs*log(refs)/live_length. With the three extra
   refs `a` outranked `b` and took $s1; with the real (void) call the two
   parameters' ranges end in source order and they come out $s2/$s1. 6 DIFF. */
void func_8004B854(s32 a, s32 b, s32 c) {
    u8 t[2];

    func_8004B834();
    t[0] = c;
    t[1] = 0x13;
    func_8003F320();
    func_8002537C(a, 0, 0, (s32) t, b);
}

void func_8004B854(s32, s32, s32);

void func_8004B8BC(s32 a, s32 b) {
    func_8004B854(a, b, 0);
}

void func_8004B834(void);
void func_8003F320(void);
void func_800B6A0C(s32, s32, s32, s32, s32);

void func_8004B8DC(s32 a, s32 b, s32 c, s32 d, s32 e) {
    func_8004B834();
    func_8003F320();
    func_800B6A0C(a, b, c, d, e);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004B954);

u8 *func_8004C010(u8 *, u8 *);

/* Two shapes carry this one:

   * `p[0x18] = p[0x30] = r[8] + r[0xA];` -- ONE chained assignment, not two
     statements. The sum is computed once and the stores come out 0x30 first,
     which is the order `a = b = e` gives. As two statements the second one
     re-reads both bytes and re-adds them: the first `sb` is a QImode store,
     and `note_mem_written` sets cse's `all` flag for every QImode store, so
     nothing survives it. Same at the tail for 0x31/0x25.
   * each decrement guard is `if (A < B || B != C)`, which puts the body
     BETWEEN the two tests -- `bnez` to it, then `beq` past it. `B` is read
     once because both tests are in the same expression, ahead of the four
     byte stores that would otherwise flush it. */
u8 *func_8004BDDC(u8 *a, u8 *b, u8 *p, u8 *q, u8 *r)
{
    *(u32 *)(p + 0x2C) = *(u32 *)(p + 0x20);
    *(u32 *)(p + 0x20) = *(u32 *)(p + 0x18);
    *(u32 *)(p + 0x14) = *(u32 *)(p + 0x10);
    if (r[8] + r[0xA] >= 0x100)
    {
        r[0xA] = r[0xA] - 1;
    }
    if (r[9] + r[0xB] >= 0x100)
    {
        r[0xB] = r[0xB] - 1;
    }
    q += 0xC;
    *(u32 *)(p + 0x10) = *(u32 *)q;
    p[4] = p[0x13];
    func_8004C010(p + 0x10, a);
    func_8004C010(p + 0x10, b);
    *(u32 *)(p + 0x1C) = *(u32 *)(q + 4);
    p[5] = p[0x1F];
    func_8004C010(p + 0x1C, a);
    func_8004C010(p + 0x1C, b);
    *(u32 *)(p + 0x28) = *(u32 *)(q + 8);
    p[6] = p[0x2B];
    func_8004C010(p + 0x28, a);
    func_8004C010(p + 0x28, b);
    func_8004C010(p + 4, a);
    func_8004C010(p + 4, b);
    p[3] = 0xC;
    p[7] = r[1];
    p[0xC] = r[8];
    *(s16 *)(p + 0xE) = *(u16 *)(r + 6);
    p[0xD] = r[9];
    if (*(s16 *)(p + 0x14) < *(s16 *)(p + 8) || *(s16 *)(p + 8) != *(s16 *)(p + 0x20))
    {
        r[0xA] = r[0xA] - 1;
    }
    p[0x18] = p[0x30] = r[8] + r[0xA];
    p[0x19] = r[9];
    *(s16 *)(p + 0x1A) = *(u16 *)(r + 4);
    p[0x24] = r[8];
    if (*(s16 *)(p + 0x22) < *(s16 *)(p + 0xA) || *(s16 *)(p + 0xA) != *(s16 *)(p + 0x16))
    {
        r[0xB] = r[0xB] - 1;
    }
    p[0x25] = p[0x31] = r[9] + r[0xB];
    return q;
}

u8 *func_8004C010(u8 *d, u8 *s) {
    u8 *p;
    s32 i;
    s32 v;

    if ((*(u32 *) s << 8) != 0x80808000) {
        p = d;
        /* All three increments in the `for`, in the target's order: with
           `p++`/`s++` in the body the counter is bumped after the pointer. */
        for (i = 0; i < 3; i++, p++, s++) {
            v = (*p * *s) >> 7;
            if (v < 0x100) {
                *p = v;
            } else {
                *p = 0xFF;
            }
        }
    }
    return d;
}

/* The primitive builder: dispatch on `e[1] & 0xFC`, modulate the colour words
   with func_8004C010 for each of the primitive's vertices, and return the
   advanced cursor. Three things decide it:

   * ONE `return q;` at the end, not an early return per arm. Each `return q`
     is another REFERENCE to `q` in global-alloc's priority formula, and with
     five of them `q` (17 refs, live 152) outranks `e` (17 refs, live 188) and
     takes $s3 where the listing has `e` there -- 32 register-only differing
     words from that alone, with the word count already exact. Written as a
     plain if/else-if chain with the single exit, the two swap.
   * `p[7] = k;` followed by `p[7] = e[1];` -- two stores to the same byte with
     the first dead. flow does NOT delete it, because the `lbu` of `e[1]`
     between them is a load through a different pointer and that drops flow's
     `mem_set_list` entry for the first store.
   * the dispatch is an if/else-if chain, not a switch: the listing is a plain
     `bne` chain with no range test anywhere. */
u8 *func_8004C010(u8 *, u8 *);
u8 *func_8004BDDC(u8 *, u8 *, u8 *, u8 *, u8 *);
u8 *func_8004B954(u8 *, u8 *, u8 *, u8 *, u8 *, s32, s32);
void func_80067DF0(u8 *, RECT *);

u8 *func_8004C080(u8 *a, u8 *b, u8 *p, u8 *q, u8 *e, s32 n)
{
    RECT r;
    s32 k = e[1] & 0xFC;

    if (k == 0x28)
    {
        *(u32 *)(p + 4) = *(u32 *)(e + 4);
        func_8004C010(p + 4, a);
        func_8004C010(p + 4, b);
        p[3] = 5;
        *(u32 *)(p + 0xC) = *(u32 *)(p + 0x10);
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x18);
        *(u32 *)(p + 0x14) = *(u32 *)(p + 0x20);
        p[7] = e[1];
    }
    else if (k == 0x48)
    {
        *(u32 *)(p + 4) = *(u32 *)(e + 4);
        func_8004C010(p + 4, a);
        func_8004C010(p + 4, b);
        p[3] = 5;
        *(u32 *)(p + 0x14) = 0x55555555;
        p[7] = e[1];
        *(u32 *)(p + 8) = *(u32 *)(p + 0x10);
        *(u32 *)(p + 0xC) = *(u32 *)(p + 0x20);
        *(u32 *)(p + 0x10) = *(u32 *)(p + 0x18);
    }
    else if (k == 0x38)
    {
        *(u32 *)(p + 4) = *(u32 *)(e + 4);
        func_8004C010(p + 4, a);
        func_8004C010(p + 4, b);
        q += 0xC;
        p[3] = 8;
        p[7] = k;
        p[7] = e[1];
        *(u32 *)(p + 0xC) = *(u32 *)q;
        func_8004C010(p + 0xC, a);
        func_8004C010(p + 0xC, b);
        *(u32 *)(p + 0x14) = *(u32 *)(q + 4);
        func_8004C010(p + 0x14, a);
        func_8004C010(p + 0x14, b);
        *(u32 *)(p + 0x1C) = *(u32 *)(q + 8);
        func_8004C010(p + 0x1C, a);
        func_8004C010(p + 0x1C, b);
    }
    else if (k == 0x3C)
    {
        q = func_8004BDDC(a, b, p, q, e);
    }
    else if ((e[1] & 0x80) != 0)
    {
        s32 m = e[1] & 0x7F;

        if (m == 0)
        {
            r.x = e[8];
            r.y = e[9];
            r.w = e[0xA] + 1;
            r.h = e[0xB] + 1;
            func_80067DF0(p, &r);
        }
        else if (m == 1)
        {
            func_80067F20((u32 *)p, 1, *(u16 *)(e + 6), *(s16 *)(e + 4), 0);
        }
        else if ((m & 0x7C) == 0x30)
        {
            q = func_8004B954(a, b, p, q, e, n, 1);
        }
        else if ((m & 0x7C) == 0x20)
        {
            q = func_8004B954(a, b, p, q, e, n, 0);
        }
    }
    return q;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004C36C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CAA0);

u8 *func_8004CAE8(u8 *p, s32 k) {
    /* A do-while with the byte latched and the advance BEFORE the test, so
       the `addiu` lands in the back edge's delay slot and runs on the exiting
       iteration too -- the same shape as func_8004B4DC. The second read of
       p[0] is a second read in the source; cse does not share it across the
       0x20 mask. */
    u8 v;

    do {
        if ((p[0] & 0x20) != 0) {
            if (*(u16 *) (p + 6) == k) {
                return p;
            }
        }
        v = p[0];
        p += 0xC;
    } while ((v & 0x80) == 0);
    return 0;
}

/* The counter is decremented IN PLACE with a separate copy for the test
   (`m = n--;`), which is the target's `addu $v1, $v0, $zero` followed by
   `addiu $v0, $v0, -0x1`. Written `*(s32 *) p = n - 1;` the decrement goes to
   a fresh register, there is no copy and the function is a word short. */
s32 func_8004CB2C(u8 *p) {
    s32 r = -1;
    u8 *e;
    s32 n;
    s32 m;
    s32 t;

    if (*(u32 *) (p + 4) != 0) {
        n = *(s32 *) p;
        m = n--;
        *(s32 *) p = n;
        if (m <= 0) {
            e = *(u8 **) (p + 8);
            t = *(s16 *) (e + 2);
            if (t == 2) {
                *(u8 **) (p + 8) = e + 8;
                r = 2;
            } else if (t == 1) {
                *(u8 **) (p + 8) = *(u8 **) (e + 4);
                r = 1;
            } else if (t == 0) {
                *(u8 **) (p + 8) = *(u8 **) (e + 4);
                r = 0;
            }
            e = *(u8 **) (p + 8);
            if (*(s16 *) (e + 2) == 2) {
                **(u32 **) (p + 4) = *(u32 *) (e + 4);
            } else {
                r = -2;
            }
            *(s32 *) p = *(u16 *) *(u8 **) (p + 8) / D_80080A84;
        }
    }
    return r;
}

void func_8004CBFC(u32 *p, u8 *q, u32 *r) {
    p[2] = (u32) q;
    p[0] = *(u16 *) q / D_80080A84;
    p[1] = (u32) r;
    if (r != 0) {
        r[0] = *(u32 *) (q + 4);
    }
}

/* The whole chain is re-read for each of the four words: a store through the
   inner pointer invalidates cse's entries for both dereferences. */
void func_8004CC38(u8 **p, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        *(u32 *) *(u8 **) (*p + 4) = D_8002E5D8[0];
        *(u32 *) (*(u8 **) (*p + 4) + 4) = D_8002E5D8[1];
        *(u32 *) (*(u8 **) (*p + 4) + 8) = D_8002E5D8[2];
        *(u32 *) (*(u8 **) (*p + 4) + 0xC) = D_8002E5D8[3];
        p++;
    }
}

void func_8004CCBC(u8 **p, s32 n) {
    s32 i = 0;

    for (; i < n; i++) {
        /* `s` is assigned INSIDE the body so loop-invariant motion puts its
           materialisation in the preheader, after the `blez`; as an
           initialiser before the loop it lands ahead of the guard. And the
           first element is read through the symbol while the other two go
           through `s`, which is what the target's `%lo($t0)` against
           `0x4($a3)` is showing. */
        u32 *s = D_8002E5E8;

        (*(u32 **) (*p + 8))[0] = D_8002E5E8[0];
        (*(u32 **) (*p + 8))[1] = s[1];
        (*(u32 **) (*p + 8))[2] = s[2];
        p++;
    }
}

/* Builds the per-entry primitive chain through func_8004C080 and links each
   one into the OT. Three things decide it:

   * the descriptor is copied with a WHOLE-STRUCT assignment through an
     all-`u8` struct, which has alignment 1 -- that is the three `lwl`/`lwr`
     pairs and three `swl`/`swr` pairs, not six word moves. Same lever as
     func_800498A0 above.
   * the counter's ADDRESS is passed to func_8004C080, so it is addressable
     and lives on the stack: the listing reads and writes `0x28($sp)` once per
     iteration rather than keeping it in a register.
   * `e` is initialised to NULL and overridden, and the `*pp = e + 0xC` store
     that the override makes dead is kept because the overwrite is in the
     block AFTER the join -- flow's `mem_set_list` is per basic block. */
typedef struct Unk4CD28
{
    u8 unk_00[12];
} Unk4CD28;

u8 *func_8004C080(u8 *, u8 *, u8 *, u8 *, u8 *, s32);

u8 *func_8004CD28(u8 **ot, u8 *b, u8 **pp, u8 *prim)
{
    u8 t[12];
    s32 n;
    u8 *e;

    n = 1;
    *(Unk4CD28 *)t = *(Unk4CD28 *)*pp;
    *pp = func_8004C080(b + 0xC, D_80080B54, prim, *pp, t, (s32)&n);
    n--;
    while (n != -1)
    {
        addPrim(ot, prim);
        prim = prim + prim[3] * 4 + 4;
        n--;
    }
    e = NULL;
    if ((t[0] & 0x80) == 0)
    {
        e = *pp;
        *pp = e + 0xC;
    }
    *pp = e;
    return prim;
}

void func_8004CE68(u8 *p, s32 f) {
    /* A do-while with the `p = q` at the TOP of the body and an explicit
       guard, not `while (p != 0)`: the loop condition is tested once, so the
       listing has no `p != 0` at the bottom, and the copy lands in the first
       branch's delay slot. */
    u8 *q = p;

    if (p != 0) {
        do {
            p = q;
            if (f != 0) {
                p[1] |= 2;
            } else {
                p[1] &= 0xFD;
            }
            if ((p[1] & 0xFC) == 0x38) {
                q = p + 0x18;
            } else {
                q = p + 0xC;
            }
        } while ((p[0] & 0x80) == 0);
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004CECC);

u8 *func_8004D064(u8 *p) {
    /* The parameter is advanced in place and returned, which is the `addu
       $v0, $a0, $zero` at the end; computing `p + 0xC` into the return value
       instead fills both delay slots and loses a word. */
    switch (p[1] & 0xF0) {
    case 0x20:
        p += 0xC;
        break;
    case 0x30:
        p += 0x18;
        break;
    default:
        p += 0xC;
        break;
    }
    return p;
}

void func_8004D09C(u8 *p) {
    u16 *q = *(u16 **) (p + 0x20);

    D_80083160.unk_BC = q[1];
    D_80083160.unk_BE = q[3];
    D_80083160.unk_C0 = q[5];
}

void func_8004D0C8(u32 a) {
    D_80083178.unk_D8 = a;
    D_80083178.unk_B4 = (u32) func_8004D09C;
}

void func_8004D0E4(u8 *p) {
    u16 *q = *(u16 **) p;

    D_80083160.unk_BC = q[0];
    D_80083160.unk_BE = q[1];
    D_80083160.unk_C0 = q[2];
}

void func_8004D110(u32 a) {
    D_80083178.unk_B8 = a;
    D_80083178.unk_B4 = (u32) func_8004D0E4;
}

/* THREE separate halfword locals, not one reused: as one quantity it spans
   the whole tail and local-alloc gives it $a1, leaving the parameter in $a0,
   where the target has the halfword in $a0 and the parameter copied to $a1 --
   that copy IS the parameter move, and no explicit `u8 *p = r;` produces it
   (10 DIFF in every distribution of p against r). */
void func_8004D12C(u8 *p) {
    u16 *d;
    s32 t;
    u16 a;
    u16 b;
    u16 c;

    t = *(s32 *) (p + 0x14) + *(s32 *) (p + 0x18);
    d = *(u16 **) p;
    *(s32 *) (p + 0x14) = t;
    if (t >= 0x1000) {
        *(s32 *) (p + 0x14) = 0x1000;
        *(u32 *) (p - 4) = 0;
    }
    a = *(u16 *) (p + 4);
    d[0] = *(u16 *) (p + 0xC) - a + ((s16) a * *(s32 *) (p + 0x14) >> 12);
    b = *(u16 *) (p + 6);
    d[1] = *(u16 *) (p + 0xE) - b + ((s16) b * *(s32 *) (p + 0x14) >> 12);
    c = *(u16 *) (p + 8);
    d[2] = *(u16 *) (p + 0x10) - c + ((s16) c * *(s32 *) (p + 0x14) >> 12);
}

void func_8004D12C(u8 *);

/* The two pointer stores LAST, after all three subtractions: perm.py says
   that is the only one of the 120 orderings of those five statements that
   matches. */
void func_8004D1EC(u8 *p, u16 *a, s32 d, u16 *b) {
    *(u32 *) (p + 0x14) = 0;
    *(u32 *) (p + 0x1C) = 0;
    if (d != 0) {
        *(u32 *) (p + 0x18) = 0x400 / d;
    } else {
        *(u32 *) (p + 0x18) = 0x1000;
    }
    if (a != 0) {
        *(u16 *) (p + 0xC) = a[0];
        *(u16 *) (p + 0xE) = a[1];
        *(u16 *) (p + 0x10) = a[2];
        *(u16 *) (p + 4) = a[0] - b[0];
        *(u16 *) (p + 6) = a[1] - b[1];
        *(u16 *) (p + 8) = a[2] - b[2];
        *(u32 *) (p + 0) = (u32) b;
        *(u32 *) (p - 4) = (u32) func_8004D12C;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D294);

void func_8004D294(s16 *, s32, u16);

void func_8004D45C(u8 *p, s32 b, u16 c) {
    s16 t[3];
    s16 *q = 0;

    if (p != 0) {
        t[0] = *(u16 *) (p + 0x2);
        t[1] = *(u16 *) (p + 0x6);
        t[2] = *(u16 *) (p + 0xA);
        q = t;
    }
    func_8004D294(q, b, c);
}

void func_80041900(u8 *);

void func_8004D4AC(void)
{
    VECTOR v;
    MATRIX m2;
    MATRIX m3;
    MATRIX m1;
    SVECTOR sv;
    s32 flag;
    Unk80083178 *b = &D_80083178;

    m1.t[0] = m1.t[1] = m1.t[2] = 0;
    m2.t[0] = m2.t[1] = m2.t[2] = 0;
    RotMatrix(&b->unk_94[3], &m1);
    SetRotMatrix(&m1);
    SetTransMatrix(&m1);
    sv.vx = -b->unk_94[2].vx;
    sv.vy = -b->unk_94[2].vy;
    sv.vz = -b->unk_94[2].vz;
    RotTrans(&sv, &v, &flag);
    TransMatrix(&m1, &v);
    RotMatrix(&b->unk_94[1], &m2);
    SetRotMatrix(&m2);
    SetTransMatrix(&m2);
    RotTrans(&b->unk_94[0], &v, &flag);
    TransMatrix(&m2, &v);
    func_80041900((u8 *)&m2);
    CompMatrix(&m2, &m1, &m3);
    SetTransMatrix(&m3);
    SetRotMatrix(&m3);
}

void func_8004D4AC(void);

void func_8004D5D0(void) {
    u8 *d = (u8 *) &D_80083178;
    u8 *s = D_80083780;

    *(u16 *) (d + 0xA4) = *(u16 *) (s + 0x2);
    *(u16 *) (d + 0xA6) = *(u16 *) (s + 0x6);
    *(u16 *) (d + 0xA8) = *(u16 *) (s + 0xA);
    func_8004D4AC();
}

/* $gp group: D_80080B58..D_80080B64 are in this unit's sdata_entries block
   (added for this function -- nothing else in the unit touches them), so their
   four loads are %gp_rel. Verified in a scratch file carrying the block, since
   verify.py compiles standalone. */
void func_8004D614(void) {
    u8 *p = (u8 *) &D_80083178;

    bzero(p + 0x9C, 8);
    *(u32 *) (p + 0x94) = D_80080B60;
    *(u32 *) (p + 0x98) = D_80080B64;
    *(u32 *) (p + 0xAC) = D_80080B58;
    *(u32 *) (p + 0xB0) = D_80080B5C;
    bzero(p + 0xA4, 8);
    *(u32 *) (p + 0x13C) = 0;
    *(u32 *) (p + 0x180) = 0;
    *(u32 *) (p + 0xB4) = 0;
    *(u32 *) (p + 0xF8) = 0;
    func_8004D0C8((u32) D_80083780);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D690);

void func_8004D690(u8 *, u8 *, s32);
void func_8004D4AC(void);

void func_8004D70C(void) {
    u8 *p = (u8 *) &D_80083160;

    func_8004D690(p + 0xCC, p + 0x110, 0);
    func_8004D690(p + 0x154, p + 0x198, 1);
    func_8004D4AC();
}

/* The head word is copied by hand and the 0x40-byte tail as a struct: as one
   0x44-byte struct assignment gcc puts the leftover word AFTER the loop, which
   is where the target has it before (19 words either way, all 19 differing). */
void func_8004D75C(u8 *d, u8 *s) {
    *(u32 *) d = *(u32 *) s;
    *(Blk40 *) (d + 4) = *(Blk40 *) (s + 4);
    *(u32 *) s = 0;
}

void func_8004D75C(u8 *, u8 *);

void func_8004D7A8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xF8, p + 0xB4);
    } else {
        func_8004D75C(p + 0x180, p + 0x13C);
    }
}

void func_8004D7E8(s32 a) {
    u8 *p = (u8 *) &D_80083178;

    if (a == 0) {
        func_8004D75C(p + 0xB4, p + 0xF8);
    } else {
        func_8004D75C(p + 0x13C, p + 0x180);
    }
}

u8 func_8004D828(u16 k) {
    /* Indexed, not a walked pointer: through `u16 *p` and `p++` the base is
       materialised before the guard branch instead of in the loop's
       preheader (7 DIFF). */
    s32 i = 0;

    while (D_8007142C[i] != 0) {
        if (D_8007142C[i] == k) {
            break;
        }
        i++;
    }
    return D_8007147C[i];
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004D880);

void func_8004D8F0(u32 a, u8 *p) {
    p[0] = (a & 0xF) << 3;
    p[1] = (a & 0xF0) - 0x20;
    p[2] = 8;
    p[3] = 0x10;
}

/* The `andi $a0, 0xFF` is the u8 parameter widening to func_8004D8F0's u32,
   not a mask in the source. */
void func_8004D91C(u8 a, s16 *out) {
    u8 t[4];

    func_8004D8F0(a, t);
    out[0] = (t[0] >> 2) + 0x3C0;
    out[1] = t[1] + 0x100;
    out[2] = t[2] >> 2;
    out[3] = t[3];
}

void func_8004D98C(u8 a, s32 b, u8 *p) {
    s32 t = b & 0xF;

    p[1] = 0x2C;
    *(u16 *) (p + 4) = 0x1F;
    p[0] = 0;
    *(u16 *) (p + 6) = t + 0x7C80;
    if ((b & 0x10) != 0) {
        *(u16 *) (p + 6) = t + 0x7C88;
    }
    func_8004D8F0(a, p + 8);
}

void func_8004D9E0(u8 *p, s32 b, s32 c, s32 d, s32 e) {
    /* `b * 4` is the LEFT operand of the outer add: the target's
       `addu $a1, $a1, $a3` puts the sum in the register holding b * 4. */
    p[2] = b * 4 + (b * d / 2 - 0x80);
    p[3] = c * (e + 0x10) - 0x80;
}

void func_8004DA24(u8 *p, u8 *q) {
    if (q != 0) {
        p[0x8] += q[0];
        p[0x9] += q[1];
        p[0xA] = q[2];
        p[0xB] = q[3];
    }
}

u32 func_8004D880(u32);
void func_8004D98C(u8, s32, u8 *);
void func_8004E264(u16 *, u32);
void func_8004D9E0(u8 *, s32, s32, s32, s32);

/* The name renderer's escape parser: walks a Shift-JIS string and for each
   pair emits a 0xC-byte glyph record through func_8004D98C/func_8004E264/
   func_8004D9E0, tracking a column, a row and a size that the escapes 0x20,
   0x08, 0x0A and 0x09 move about. Returns the first record, or 0 when nothing
   was emitted; the last record gets 0x80 ORed into its first byte.

   `c` is a `u8` local, which is why every `lbu` is followed by the
   `andi 0xFF` the listing has -- as a `u32` the mask would be dropped, and
   this is the one place in the file where the redundant mask is wanted.

   The byte after a 0x09 goes into its own local `d`, read ONCE: the listing
   reuses the register it tested against 0x73 for the arithmetic, where a
   second `*s` reloads it (four words).

   `s32 t = x - 0x30; x = t + d;` as two statements. Written
   `x = x - 0x30 + d` -- or with parentheses, or as `d + (x - 0x30)` -- fold
   moves the constant onto `d` and emits `addiu $v0, $v1, -0x30` /
   `addu $s1, $s1, $v0`; the listing keeps it on `x`:
   `addiu $v0, $s1, -0x30` / `addu $s1, $v0, $v1`. A temporary is the only way
   to stop the reassociation (see matching-levers section 3).

   `s[1] | (*s << 8)` in that operand order, and the trailing `out != base`
   arm written FIRST so the `base = 0` arm goes out of line. */
u8 *func_8004DA74(u8 *out, u8 *s, s32 a)
{
    u8 *base;
    s32 x;
    s32 y;
    s32 z;
    u8 c;

    x = 0;
    y = x;
    z = x;
    base = out;
    if (s == 0)
    {
        return 0;
    }
    c = *s;
    while (c != 0)
    {
        if (c == 0x20)
        {
            s++;
            x++;
        }
        else if (c == 8)
        {
            s++;
            a = *s - 0x30;
            s++;
        }
        else if (c == 0xA)
        {
            s++;
            x = 0;
            y++;
        }
        else if (c == 9)
        {
            u8 d;

            s++;
            d = *s;
            if (d == 0x73)
            {
                s++;
                z = *s - 0x30;
            }
            else
            {
                s32 t = x - 0x30;

                x = t + d;
            }
            s++;
        }
        else
        {
            u8 d = (u8) func_8004D880(s[1] | (*s << 8));

            s += 2;
            if (d != 0x20)
            {
                func_8004D98C(d, a, out);
                func_8004E264((u16 *) out, a);
                func_8004D9E0(out, x, y, z, 0);
                out += 0xC;
            }
            x += 2;
        }
        c = *s;
    }
    if (out != base)
    {
        out[-0xC] |= 0x80;
    }
    else
    {
        base = 0;
    }
    return base;
}

u32 func_8004B404(s32);
s32 func_8004B4DC(u8 *);
s32 puts(u8 *);

u8 *func_8004DC14(u8 *name, s32 b) {
    u8 *h = (u8 *) func_8004B404(0x100);

    if (h != 0) {
        if (func_8004DA74(h, name, b) != 0) {
            if (func_8004B4DC(h) >= 0x16) {
                puts(name);
            }
            return h;
        }
        func_8004B1A4((s32) h);
        h = 0;
    }
    return h;
}

void func_8004DCA8(u8 *p) {
    if (p == 0) {
        p = D_80080B68;
    }
    /* The post-increment, not a separate read-and-store pair: it is what puts
       the `addiu` before the index's `addu` (2 DIFF the other way, 13 with the
       increment written as its own statement first). */
    D_80083D98[D_80081554++] = (u32) p;
}

void func_8004DCE0(u32 a) {
    D_80081550 = a;
}

void func_8004DCEC(void) {
    ((void (*)(u8 *)) D_800714A4[D_80081550])(D_80080B6C);
}

void func_8003528C(u32);

void func_8004DD2C(u8 *p) {
    D_80081554 &= 0xF;
    D_80081558 = D_80081554;
    D_80083D98[D_80081554++] = 0x15080707;
    func_8004DCA8(p);
    D_80083D98[D_80081554++] = 0x23;
    func_8003528C((u32) &D_80083D98[D_80081558]);
}

void func_8004DDB4(void) {
    D_80081554 = 0;
    D_80081558 = 0;
}

void func_80033AE8(s32);

void func_8004DDC4(void) {
    func_80033AE8(2);
}

void func_8004DCA8(u8 *);
void func_8003528C(u32);

/* func_8004DD2C's shape with a third word. `p` is the pass-through: nothing
   sets $a0 before the `jal func_8004DCA8`. */
void func_8004DDE4(u8 *p) {
    D_80081554 &= 0xF;
    D_80081558 = D_80081554;
    D_80083D98[D_80081554++] = 0x15080707;
    func_8004DCA8(p);
    D_80083D98[D_80081554++] = 0x20C11;
    D_80083D98[D_80081554++] = 1;
    func_8003528C((u32) &D_80083D98[D_80081558]);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DE88);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004DF8C);

s32 func_80033B2C(s32);
void func_80033AE8(s32);

s32 func_8004E0AC(void) {
    s32 r = 0;

    if (func_80033B2C(3) != 0) {
        func_80033AE8(3);
        r = 1;
    } else if (func_80033B2C(4) != 0) {
        func_80033AE8(4);
        r = 2;
    } else if (func_80033B2C(5) != 0) {
        func_80033AE8(5);
        r = 3;
    }
    return r;
}

void func_8003528C(u32);
void func_800352B4(void);

/* $gp group (D_80081554/58), so this was verified in a scratch file carrying
   the unit's .sdata block. The call argument is written as a SECOND reference
   to the array -- after `D_80081558 = D_80081554` cse knows the load of
   D_80081558 is that same pseudo, so the two addresses are one expression and
   there is only one `addu`, with the base materialised first (22 words; a
   local for the element address gets the base materialised last, 21 words). */
void func_8004E130(void) {
    D_80081554 &= 0xF;
    D_80081558 = D_80081554;
    D_80083D98[D_80081554++] = 1;
    func_8003528C((u32) &D_80083D98[D_80081558]);
    func_800352B4();
}

extern u32 D_80071410[];

/* Both of the first two arms write the whole four-byte group; cross-jumping
   merges the shared tail, which is what puts the second arm's `addiu -0x60`
   just before the common `sb 0x9`. */
u8 *func_8004E188(u8 *p, u32 k) {
    u32 i = k & 0xFF;
    u32 f = D_800714B0[i];

    if (f & 0x30) {
        p[8] = (k & 0xF) << 3;
        p[9] = (k & 0x70) >> 1;
        p[0xA] = 8;
        p[0xB] = 8;
    } else if (f & 0x40) {
        p[8] = (k & 0xF) << 3;
        p[9] = (k & 0xF0) - 0x60;
        p[0xA] = 8;
        p[0xB] = 8;
    } else if (f & 0x80) {
        *(u32 *) (p + 8) = D_80071410[i];
    }
    return p;
}

typedef struct Unk4E21C {
    u8 unk_00[4];
} Unk4E21C;

u8 *func_8004E188(u8 *, u32);

u8 *func_8004E21C(u8 *p, u8 b) {
    u8 t[16];

    func_8004E188(t, b);
    /* A byte-aligned 4-byte struct assignment: that is the lwl/lwr and
       swl/swr pair rather than a word move. */
    *(Unk4E21C *) p = *(Unk4E21C *) (t + 8);
    return p;
}

void func_8004E264(u16 *p, u32 v) {
    u32 t = (v & 0xF) + 0x7C80;

    p[3] = t + ((v & 0x10) << 3);
}

u8 *func_8004E280(u8 *p, s32 *out) {
    *out = *p - 0x30;
    return p + 1;
}

/* The text layout pass: walks a NUL-terminated string and turns every
   printable byte into a 0xC-byte glyph record through func_8004E264 and
   func_8004E188, advancing a running `x` and `y`. Space, newline, tab, 0x08
   and 0x0C are control codes; tab with an `s` after it sets the tab stop,
   otherwise it advances `x` by (digit * (stop + 8)). Returns the start of the
   run with the end bit set on the last record, or 0 if nothing was emitted.

   `o + 0xA` is a GIV, not a second source pointer. `o` is set once in the
   loop (`o += 0xC`, on one path only, which still makes it a biv for
   loop.c's purposes) so strength_reduce adds a second register biased to the
   +0x0A field -- the one the glyph arm reads back -- and rewrites +1, +2, +3
   and +4 as NEGATIVE displacements off it. Write the pointer naturally and
   loop.c produces the pair.

   Three shapes paid for, 104 DIFF down to a match:

   * The byte is read ONCE per iteration into an `s32` and masked into a
     SECOND variable: `d = *src;` carries the loop's zero test, `c = d & 0xFF`
     every comparison. A `u8 c = d` copy is a plain `addu` (that is
     func_8005947C's shape), not the listing's `andi $v1, $v0, 0xFF`, because
     the truncation of an `lbu` result is free; the explicit mask is what
     survives.

   * The space arm needs a TEMPORARY for the constant: `x = x + 8 + w;`
     reassociates to `x + (w + 8)` and emits `addiu $v0, $s4, 8`, where the
     listing has `addiu $v0, $s1, 8` -- the sum pinned as `(x + 8) + w`. Same
     lever as matching-levers section 8's reassociation note.

   * The four initialisers go `x`, `y`, `start`, `w`. That order decides which
     of the five callee-saved register save/init pairs sched2 interleaves
     first; every other permutation of the four is 4 to 9 words out, with
     `start` first (the natural reading) at 6. */
u8 *func_8004E298(u8 *o, u8 *src, s32 flags)
{
    u8 *start;
    s32 x;
    s32 y;
    s32 w;
    s32 t;
    s32 u;
    s32 d;
    s32 c;

    x = 0;
    y = 0;
    start = o;
    w = 0;
    d = *src;
    while (d != 0)
    {
        c = d & 0xFF;
        if (c == 0x20)
        {
            src++;
            u = x + 8;
            x = u + w;
        }
        else if (c == 0xA)
        {
            src++;
            x = 0;
            y += 8;
        }
        else if (c == 9)
        {
            src++;
            if (*src == 0x73)
            {
                src++;
                w = *src - 0x30;
            }
            else
            {
                x += (*src - 0x30) * (w + 8);
            }
            src++;
        }
        else if (c == 8)
        {
            src = func_8004E280(src + 1, &flags);
        }
        else if (c == 0xC)
        {
            src = func_8004E280(src + 1, &t);
            x += t;
        }
        else
        {
            o[0] = 0;
            o[1] = (flags & 0x80) ? 0x2E : 0x2C;
            o[2] = x - 0x80;
            o[3] = y - 0x80;
            *(s16 *) (o + 4) = ((flags & 0x300) >> 3) | 0xF;
            func_8004E264((u16 *) o, flags);
            func_8004E188(o, *src);
            src++;
            x += o[0xA] + w;
            o += 0xC;
        }
        d = *src;
    }
    if (o != start)
    {
        o[-0xC] |= 0x80;
    }
    else
    {
        start = 0;
    }
    return start;
}

u8 *func_8004E478(u8 *p, u16 n) {
    /* The PARAMETER is the moving pointer and the saved copy is returned. */
    u8 *r = p;
    u8 *b = r + (n - 1);
    u8 x;
    u8 y;

    while (p < b) {
        x = *p;
        y = *b;
        *b-- = x;
        *p++ = y;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E4C0);

void func_8004E4C0(s32, u16, s32, s32);

void func_8004E57C(s32 a, u16 b, s32 c) {
    func_8004E4C0(a, b, c, 0x20);
}

u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5A0(s32 a, u16 n, u8 *dst) {
    func_8004E57C(a, n, (s32) dst);
    func_8004E478(dst, n);
    return dst;
}

void func_8004E4C0(s32, u16, s32, s32);
u8 *func_8004E478(u8 *, u16);

u8 *func_8004E5E8(s32 a, u16 n, u8 *dst) {
    func_8004E4C0(a, n, (s32) dst, 0x30);
    func_8004E478(dst, n);
    return dst;
}

u8 *func_8004E478(u8 *, u16);

void func_8004E634(u32 n, u8 *s) {
    s32 len = 0;
    u32 q;

    do {
        q = n / 10;
        *s = n - q * 10 + 0x30;
        len++;
        s++;
        n = q;
    } while (n != 0);
    *s = 0;
    func_8004E478(s - len, len);
}

u8 *func_8004E69C(u8 *s) {
    u8 *p = s;
    u8 c;

    while (*p != 0) {
        c = *p;
        if ((D_800714B0[c] & 0x10) != 0) {
            *p = c + 0x31;
        }
        p++;
    }
    return s;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E6F4);

void func_8004E884(u8 *p, u32 b) {
    u8 v;

    if (p != 0) {
        do {
            func_8004E264((u16 *) p, b);
            v = p[0];
            p += 0xC;
        } while ((v & 0x80) == 0);
    }
}

s32 func_8004E8D8(u8 *p, s32 b) {
    s32 r = 0;
    u8 c = *p;

    while (c != 0) {
        if (c != 0x20 && c != 0xA) {
            r = 1;
            break;
        }
        p++;
        c = *p;
    }
    return r;
}


u8 *func_8004E928(s32 a, s32 b) {
    u8 *r = 0;

    if (func_8004E8D8((u8 *) a, b) != 0) {
        r = (u8 *) func_8004B404(0x100);
        if (r != 0) {
            func_8004E298(r, a, b);
        }
    }
    return r;
}

void func_8004E994(void) {
}

void func_8004B1A4(s32);

void func_8004E99C(s32 a) {
    if (a != 0) {
        func_8004B1A4(a);
    }
}

void func_8004B1A4(s32);

void func_8004E9C0(s32 a) {
    if (a != 0) {
        func_8004B1A4(a);
    }
}

void func_8004E9E4(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004E9EC);

s32 func_8004E9EC(u8 *, s32);

u8 *func_8004EA70(u8 *p, s32 i) {
    func_8004E9EC(p, i);
    p += i * 2;
    strncpy(p, D_80080B70, 2);
    return p;
}

s32 func_8004E9EC(u8 *, s32);

/* The offset pointer is its OWN variable, not `dst += i * 2`. In-place, the
   parameter and the walked pointer are one quantity that spans both calls and
   the branch, so it is the function's only global allocno and takes the last
   callee-saved register, dragging the other two parameters round with it. As a
   separate local, `dst` dies at the `addu` and `q` is what lives, which is the
   $s1/$s2/$s0 assignment the target has. 13 DIFF of pure register naming. */
u8 *func_8004EAC4(u8 *dst, u8 *src, s32 i) {
    s32 r = func_8004E9EC(dst, i);
    u8 *q = dst + i * 2;

    strncpy(q, src, 2);
    if (r != 0) {
        q[2] = 0;
    }
    return q;
}

void func_8004EB30(void) {
    D_80084130[0] = 0;
}

void func_8004EB3C(u8 *p) {
    p[0] = 0;
    func_8003E4FC(6, D_8007164C, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
}

void func_8004EB8C(u8 *p) {
    p[0] = 0;
    func_8003E4FC(6, D_80071644, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
}

s32 func_8004EBDC(void) {
    s32 r = 0;

    if (func_80053EF0(1) == 0) {
        r = func_80053EF0(4) == 0;
    }
    return r;
}

s32 func_8004EBDC(void);

/* One `r` and one `return` at the end, with the two guards as `&&`: written as
   early `return r;`s gcc materialises 0 into $v0 twice and the function is two
   words short -- the target spends `addu $v1, $v0, $zero` after the call and
   `addu $v0, $v1, $zero` at the join. */
s32 func_8004EC20(u8 *p) {
    s32 r = 0;

    if (p[8] != 0 && p[9] != 0) {
        while (func_8004EBDC() == 0) {
            func_80044618(2);
        }
        r = ((s32 (*)(u8 *)) D_80071654[*(u32 *) (p + 4)])(p - 0x20);
    }
    return r;
}

void func_8004EE90(u8 *);

void func_8004ECAC(u8 *q) {
    u8 *p = q - 0x20;
    u16 f = *(u16 *) (p + 0x1E);
    u8 *r;

    if ((f & 0x2000) != 0) {
        *(u16 *) (p + 0x1E) = f & 0xDFFF;
        r = *(u8 **) q;
        *(u16 *) (r + 0x1E) |= 0x2000;
        func_8004EE90(p);
    }
}

void func_8004EB8C(u8 *);
void func_8004ECAC(u8 *);
/* The split spelling of D_80081480, whose own declaration is a scalar with
   users elsewhere: the symbolic form is one insn before maspsx, so its `lui`
   cannot go in the `beqz`'s delay slot where the target has it. */
extern u32 D_80081480a[] __asm__("D_80081480");

void func_8004ED00(u8 *p) {
    if (p[8] != 0) {
        func_80041344(0x80020000, D_80081480a[0]);
        DrawSync(0);
        func_8004EB8C(p + 9);
        *(u32 *) (p - 0x10) = (u32) func_8004ECAC;
    }
}

s32 func_8004EC20(u8 *);
void func_8004ED00(u8 *);

void func_8004ED5C(u8 *p) {
    if (func_80044144(1, 0x1E, (u32) func_8004EC20, (u32) p) != 0) {
        *(u32 *) (p - 0x10) = (u32) func_8004ED00;
    }
}

void func_8004EB3C(u8 *);
s16 func_80053DA8(u16);

UnkNode *func_8004EDA8(s32 a, s32 b) {
    UnkNode *n;
    u8 *e;

    *(u16 *) (a + 0x1E) &= 0xDFFF;
    n = func_8003FC64(0);
    if (n != NULL) {
        e = (u8 *) (n + 1);
        *(s32 *) (e + 4) = b;
        *(s32 *) e = a;
        func_80053DA8(0xD1);
        *(u32 *) (e - 0x10) = (u32) func_8004ED5C;
        func_8004EB3C((u8 *) n + 0x28);
    }
    return n;
}

UnkNode *func_8004EDA8(s32, s32);

void func_8004EE30(s32 a) {
    func_8004EDA8(a, 0);
}

UnkNode *func_8004EDA8(s32, s32);

void func_8004EE50(s32 a) {
    func_8004EDA8(a, 1);
}

UnkNode *func_8004EDA8(s32, s32);

void func_8004EE70(s32 a) {
    func_8004EDA8(a, 2);
}

void func_8004EE90(u8 *p) {
    /* The global is read BEFORE the halfword store and written after, and that
       is load-bearing: in the split addressing form its MEM is register-based,
       so sched will not hoist the load above a store through a pointer it
       cannot disambiguate. Written as a plain `D_800814A0a[0] |= 0x8000;`
       after the halfword, the two chains stay serialised and the function is
       12 words against 11 (12 DIFF). The target interleaves them, which also
       costs the copy of the argument into $a1: with all four quantities live
       at once the global's value takes $a0. */
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}

void func_8004EEBC(u8 *p) {
    u32 *q = *(u32 **) (p + 4);

    if (q != 0) {
        if (q[0] != 0) {
            *(u32 *) (p + 0x28C) = 0;
            *(u32 *) (p + 0x29C) = (u32) (p + 0xF8);
        } else {
            *(u32 *) (p + 0x28C) = (u32) (p + 0x80);
            *(u32 *) (p + 0x29C) = 0;
        }
    }
}

/* Both template bases go into locals: inline, the second one's lui/lw/addiu
   does not get scheduled into the first block's load-delay nops and the
   function is two words longer. The four `*(u8 **)(p + 4)` reads are written
   out, not cached -- the first is cse'd to a copy of `a` and the rest are real
   reloads, because each halfword store invalidates the entry. */
void func_8004EEFC(u8 *p, u32 *a, u32 *b) {
    u32 *s = D_8002E5D8;
    u32 *t = D_8002E5E8;

    a[0] = s[0];
    a[1] = s[1];
    a[2] = s[2];
    a[3] = s[3];
    b[0] = t[0];
    b[1] = t[1];
    b[2] = t[2];
    *(u32 **) (p + 4) = a;
    *(u32 **) (p + 8) = b;
    *(u32 *) p = 0;
    *(s16 *) (*(u8 **) (p + 4) + 8) = -0xA0;
    *(s16 *) (*(u8 **) (p + 4) + 0xA) = -0x78;
    *(s16 *) (*(u8 **) (p + 4) + 0xC) = 0x200;
    (*(u8 **) (p + 4))[0xF] = 4;
}

/* func_8004EFF4's shape: the two template words read into locals before the
   byte stores, through the incomplete-array spelling so their lui/lw pairs are
   the compiler's and can be scheduled apart. The `|= 2` then re-reads p[1]
   because the intervening store through p invalidated cse's entry for it --
   the parked note had this backwards, blaming the store ORDER. 24 DIFF. */
u8 *func_8004EF90(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80071660;
    a = D_80071668[0];
    b = D_80071664[0];
    p[1] |= 2;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] |= 0x80;
    return p + 0xC;
}

/* The two template words are read into locals BEFORE the byte stores, because
   sched cannot lift a load above a store through a pointer it cannot
   disambiguate -- and they are read through the incomplete-array spelling, so
   the lui/lw pair is the compiler's and can be split apart, where the scalar
   spelling's pair is maspsx's and stays glued (10 DIFF with the loads in
   place, 26 with them left in the stores). The first word stays a scalar. */
u8 *func_8004EFF4(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_8007166C;
    a = D_80071674[0];
    b = D_80071670[0];
    p[1] |= 2;
    p[0] |= 0x60;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 1;
    p[0] |= 0x80;
    return p + 0xC;
}

/* func_8004EFF4's twin; see the note there.
   The two template words are read into locals BEFORE the byte stores, because
   sched cannot lift a load above a store through a pointer it cannot
   disambiguate -- and they are read through the incomplete-array spelling, so
   the lui/lw pair is the compiler's and can be split apart, where the scalar
   spelling's pair is maspsx's and stays glued (10 DIFF with the loads in
   place, 26 with them left in the stores). The first word stays a scalar. */
u8 *func_8004F068(u8 *p) {
    u32 a;
    u32 b;

    p[1] = 0xB0;
    *(u32 *) (p + 8) = D_80071678;
    a = D_80071680[0];
    b = D_8007167C[0];
    p[1] |= 2;
    p[0] |= 0x61;
    *(u32 *) (p + 4) = a;
    *(u32 *) (p + 0xC) = b;
    p += 0x18;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 1;
    p[0] |= 0x80;
    return p + 0xC;
}

/* Seven entries of the same shape. The inner pointer at +4 is re-read for the
   second store because the first one goes through it, which is what every
   repeated `lw 0x4($a3)` in the listing is. */
void func_8004F0DC(u8 *p) {
    u8 *q;

    q = *(u8 **) (p + 0x220);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 6;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x10;
    *(u8 **) q = p + 8;
    q = *(u8 **) (p + 0x234);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x80;
    q = *(u8 **) (p + 0x238);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0x68;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0xF8;
    q = *(u8 **) (p + 0x224);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x228);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0x70;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x22C);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0xA0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
    q = *(u8 **) (p + 0x230);
    *(s16 *) (*(u8 **) (q + 4) + 8) = 0;
    *(s16 *) (*(u8 **) (q + 4) + 0xA) = 0;
    *(u8 **) q = p + 0x170;
}

u8 *func_8004F1E8(u8 *p) {
    p[1] = 0x80;
    p[8] = 0;
    p[9] = 0;
    p[0xA] = 0xFF;
    p[0xB] = 0xFF;
    p += 0xC;
    p[1] = 0x2C;
    *(s16 *) (p + 4) = GetTPage(1, 0, 0x380, 0x100);
    *(s16 *) (p + 6) = GetClut(0, 0x1F5);
    p[0xA] = 0xA0;
    p[8] = 0;
    p[9] = 0;
    p[0xB] = 0x70;
    p += 0xC;
    p[8] = 0x70;
    p[9] = 0xE0;
    p[1] = 0x80;
    p[0xA] = 8;
    p[0xB] = 8;
    p += 0xC;
    p[1] = 0x81;
    *(u16 *) (p + 4) = 0x20;
    *(u16 *) (p + 6) = 0;
    p[0] = 0x80;
    return p + 0xC;
}

u8 *func_8004EF90(u8 *);
u8 *func_8004EFF4(u8 *);
u8 *func_8004F068(u8 *);
u8 *func_8004F1E8(u8 *);
void func_8004F0DC(u8 *);

void func_8004F2BC(u8 *p) {
    func_8004EF90(p + 0x8);
    func_8004EFF4(p + 0x80);
    func_8004F068(p + 0xF8);
    func_8004F1E8(p + 0x170);
    func_8004F0DC(p);
}

u32 func_800491F4(s32, u8 *, s32);

void func_8004F308(s32 a) {
    func_800491F4(a, D_80071684, 7);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8004F330);

void func_8004F3D0(u32 *p, u32 v) {
    *p = v;
}

void func_8004EEFC(u8 *, u32 *, u32 *);
void func_8004F2BC(u8 *);

void func_8004F3D8(u8 *p, u32 v) {
    func_8004F3D0((u32 *) p, v);
    func_8004EEFC(p + 0x210, (u32 *) (p + 0x1E8), (u32 *) (p + 0x1F8));
    func_8004F2BC(p);
}

extern UnkNode D_80084138;
extern u8 D_80027DD0[];

UnkNode *func_8004B4A8(UnkNode *);
UnkNode *func_8003FE78(s32, UnkNode *, s16);
s32 func_8004491C(UnkNode *, u32);
void func_8004F52C(u8 *);
void func_8004F330(u8 *, s32);
void func_8004F3D8(u8 *, u32);
void func_8004EEBC(u8 *);

/* func_8004F308 is landed as `void` but its result is stored here. */
s32 func_8004F308r(s32) __asm__("func_8004F308");

u8 *func_8004F418(u32 a, u32 b)
{
    u8 *n = (u8 *)&D_80084138;
    u8 *m = n + 0x20;

    if (func_8004B4A8((UnkNode *)n) == 0)
    {
        n = (u8 *)func_8003FE78(0, (UnkNode *)n, 0xCF);
        func_8004491C((UnkNode *)n, (u32)D_80027DD0);
    }
    else
    {
        func_8004F52C(n);
        *(s16 *)(n + 0x1E) = *(u16 *)(n + 0x1E) & 0x7FFF;
        bzero(m, 0x31C);
    }
    D_801C9E40[0x19] = 0;
    D_801C9E40[0x1A] = 0;
    D_801C9E40[0x1B] = 0;
    D_801C9E40[0x108ED] = 0;
    D_801C9E40[0x108EE] = 0;
    D_801C9E40[0x108EF] = 0;
    func_8004F330(m, 7);
    *(u32 *)(n + 0xC) = (u32)(m + 0x210);
    *(u32 *)(m + 0x21C) = func_8004F308r((s32)(m + 0x220));
    *(u32 *)(m + 0x4) = b;
    func_8004F3D8(m, a);
    *(u32 *)(n + 0x10) = (u32)func_8004EEBC;
    func_8004EEBC(m);
    return n;
}

void func_8004F52C(u8 *p) {
    u32 g;

    if (p != 0) {
        g = D_800814A0a[0];
        *(u16 *) (p + 0x1E) |= 0x8000;
        D_800814A0a[0] = g | 0x8000;
    }
}

void func_8004FE78(u32);

void func_8004F558(u8 *p) {
    func_8004FE78(*(u32 *) (p + 4));
    *(u16 *) (p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_8004F558(u8 *);

void func_8004F5B0(u8 *p) {
    if ((D_80083170[0] & 0x20) != 0) {
        func_80053DA8(0x515);
        func_8004F558(p);
    }
}

u32 func_8004FDE0(u32);

UnkNode *func_8004F5F4(s32 a, s32 b) {
    UnkNode *n = func_8003FC64(0);

    if (n != NULL) {
        u8 *e = (u8 *) (n + 1);
        u32 r;

        n->unk_10 = (u32) func_8004F5B0;
        r = func_8004FDE0(b);
        *(u32 *) (e + 4) = r;
        if (r == 0) {
            *(u16 *) ((u8 *) n + 0x1E) |= 0x8000;
            D_800814A0 |= 0x8000;
            n = NULL;
        }
    }
    return n;
}

void func_8004F67C(void) {
}

extern u8 D_80071784[];

/* func_8004F780's sibling, writing the +0xA/+0xC halfwords of each entry's
   primitive instead of its colour bytes. Four spellings:

   * `e` is the FIRST statement of the body, ahead of the D_80071784 pointer.
     loop.c's `record_giv` PREPENDS, so the giv found LAST in the body gets its
     initialisation emitted FIRST in the preheader -- and the listing's
     preheader is `lui`/`addiu` of D_80071784 and then `addu $t3, $t0`, so the
     `p + i * 4` reference has to come first in the source. Three words, and
     the last thing wrong.
   * `h = -lo + t[...]`, not `t[...] - lo`: the subtraction's operand order
     decides which of the two byte addresses gets $v0 and which $v1, and that
     permutes six words downstream.
   * `c = *(s32 *)(p + 0x2C) * 30;` goes in a local. Written inline as
     `h * (*(s32 *)(p + 0x2C) * 30)` fold reassociates the `* 30` onto `h`,
     and the listing multiplies the +0x2C field by 30 first.
   * the three-term sums are `lo * 30 + 0x200 + h * c / d` and
     `lo * 12 - 0x40 + h * c / d`, with the constant SECOND. fold always
     pushes a constant to the end of an associative chain, so the four
     orderings of these three terms are not interchangeable: measured at 3, 9,
     50 and 52 differing words. */
void func_8004F684(u8 *p)
{
    s32 i;

    for (i = 0; i < *(s32 *)(p + 0x24); i++)
    {
        u8 *e = *(u8 **)(*(u8 **)(p + i * 4 + 4) + 0xC);
        u8 *t = &D_80071784[i * 3];
        u8 lo = t[*(s32 *)(p + 0x34)];
        s32 h = -lo + t[*(s32 *)(p + 0x30)];
        s32 c;

        c = *(s32 *)(p + 0x2C) * 30;
        *(s16 *)(*(u8 **)(e + 4) + 0xC) = lo * 30 + 0x200 + h * c / *(s32 *)(p + 0x28);
        c = *(s32 *)(p + 0x2C) * 12;
        *(s16 *)(*(u8 **)(e + 4) + 0xA) = lo * 12 - 0x40 + h * c / *(s32 *)(p + 0x28);
    }
}

extern u8 D_80071784[];
extern u8 D_80071790[];

void func_8004F780(u8 *p)
{
    s32 i;

    for (i = 0; i < *(s32 *)(p + 0x24); i++)
    {
        u8 *e = *(u8 **)(*(u8 **)(p + i * 4 + 4) + 0xC);
        u8 *t = &D_80071784[i * 3];
        u8 lo = D_80071790[t[*(s32 *)(p + 0x34)] * 4];
        u8 hi = D_80071790[t[*(s32 *)(p + 0x30)] * 4];

        **(u8 **)(e + 4) = lo + (hi - lo) * *(s32 *)(p + 0x2C) / *(s32 *)(p + 0x28);
        (*(u8 **)(e + 4))[1] = **(u8 **)(e + 4);
        (*(u8 **)(e + 4))[2] = **(u8 **)(e + 4);
    }
}

s32 func_8004F85C(u32 *p, u32 *q) {
    u32 *a = (u32 *) ((u32 *) ((u32 *) p[0])[3])[1];
    u32 *b = (u32 *) ((u32 *) ((u32 *) q[0])[3])[1];

    return *(s16 *) ((u8 *) a + 0xC) - *(s16 *) ((u8 *) b + 0xC);
}

s32 func_8004F85C(u32 *, u32 *);
void func_80044AAC(UnkNode *, UnkNode *);
void func_8004F684(u8 *);
void func_8004F780(u8 *);

void func_8004F884(u8 *p) {
    u32 t[4];
    s32 i;

    for (i = 0; i < *(s32 *) (p + 0x24); i++) {
        t[i] = *(u32 *) (p + i * 4 + 4);
    }
    func_8004AFC8((s32) t, *(s32 *) (p + 0x24), 4, (void *) func_8004F85C);
    for (i = 0; i < *(s32 *) (p + 0x24) - 1; i++) {
        func_80044AAC((UnkNode *) t[i], (UnkNode *) t[i + 1]);
    }
    func_8004F684(p);
    func_8004F780(p);
}

void func_8004F884(u8 *);

void func_8004F95C(u8 *p) {
    func_8004F884(p);
    /* The post-increment INSIDE the condition: the old value is compared and
       the incremented one stored, which is the `addu $v1, $v0, $zero` copy.
       Read, compare, then store as three statements is a word shorter. */
    if ((*(s32 *) (p + 0x2C))++ >= *(s32 *) (p + 0x28)) {
        *(u32 *) p = (u32) func_8004F67C;
    }
}

void func_8004F67C(void);

/* Both loop variables are written as expressions OF THE COUNTER, so loop.c
   makes them givs and initialises them in the preheader -- after the `blez` --
   which is where the listing has `$a2 = 0x18` and `$a1 = $a0`. Declared and
   walked outside the loop their initialisations come out ahead of the guard
   (17 DIFF), and an explicit `if (n > 0) { ... do ... }` guard reads the bound
   a third time (36 words against 32).

   `>> 3`, not `/ 8`: the division's sign correction is four extra insns. */
void func_8004F9AC(u8 *p) {
    s32 n = *(s32 *) (p + 0x20);
    s32 i;

    if (n > 0x20) {
        n = 0x20;
        *(u32 *) p = (u32) func_8004F67C;
    }
    for (i = 0; i < *(s32 *) (p + 0x24); i++) {
        u8 *q = p + i * 4;
        s32 v = 0x18 + i * n;

        *(s16 *) (*(u8 **) (*(u8 **) (*(u8 **) (q + 4) + 0xC) + 4) + 8) = v;
    }
    *(s32 *) (p + 0x20) = n + ((0x28 - n) >> 3);
}

void func_8004F9AC(u8 *);
void func_8004F95C(u8 *);
s32 func_80049DE8(s32, s32, s32);

void func_8004FA2C(u8 *p, s32 b) {
    *(u32 *) (p + 0x2C) = 0;
    if (*(u32 *) p == (u32) func_8004F9AC) {
        *(u32 *) (p + 0x20) = 0x20;
        func_8004F9AC(p);
    }
    *(u32 *) p = (u32) func_8004F95C;
    *(u32 *) (p + 0x34) = *(u32 *) (p + 0x30);
    *(u32 *) (p + 0x30) = func_80049DE8(*(u32 *) (p + 0x30), b, *(u32 *) (p + 0x24));
}

s32 func_8004FD78(u8 *, s32);
void func_8004FA2C(u8 *, s32);

/* D_80083160 through a STRUCT POINTER local, as in func_80050308: that is what
   makes the flag word at +0x08 RE-READ before the inner 0x2000/0x8000 tests.
   The store to `p + 0x38` just above invalidates it only if the two MEMs'
   MEM_IN_STRUCT_P flags differ, and `q->unk_08` is a COMPONENT_REF where
   `*(u32 *) (g + 8)` off a `u8 *` base is a bare dereference like the store
   itself -- with both bare, cse forwards the value from the outer `& 0xA000`
   test and the reload plus its delay nop go missing. */
void func_8004FAA4(u8 *p)
{
    Unk80083160 *q = &D_80083160;
    s32 s;
    s32 t;

    s = 0;
    if (q->unk_08 != 0 && *(s32 *)(p + 0x24) >= 2)
    {
        if ((q->unk_08 & 0xA000) != 0)
        {
            if ((q->unk_10 & 0x2000) != 0)
            {
                s = 1;
                *(s32 *)(p + 0x38) = 0;
                *(s32 *)(p + 0x28) = 4;
            }
            else if ((q->unk_10 & 0x8000) != 0)
            {
                s = -1;
                *(s32 *)(p + 0x38) = 0;
                *(s32 *)(p + 0x28) = 4;
            }
            else
            {
                t = *(s32 *)(p + 0x38);
                if (t >= 9)
                {
                    *(s32 *)(p + 0x38) = t - 2;
                    if ((q->unk_08 & 0x2000) != 0)
                    {
                        s = 1;
                        *(s32 *)(p + 0x28) = 3;
                    }
                    else if ((q->unk_08 & 0x8000) != 0)
                    {
                        s = -1;
                        *(s32 *)(p + 0x28) = 3;
                    }
                }
                else
                {
                    *(s32 *)(p + 0x38) = t + 1;
                }
            }
        }
        if (s == 0)
        {
            if ((q->unk_10 & 1) != 0)
            {
                if (*(s32 *)(D_800814A8 + 0xB0) != 0)
                {
                    t = func_8004FD78(p, 1);
                    *(s32 *)(p + 0x28) = 4;
                    s = t - *(s32 *)(p + 0x30);
                }
            }
            else if ((q->unk_10 & 2) != 0 && *(s32 *)(D_800814A8 + 0xAC) != 0)
            {
                t = func_8004FD78(p, 0);
                *(s32 *)(p + 0x28) = 4;
                s = t - *(s32 *)(p + 0x30);
            }
        }
        if (s != 0)
        {
            func_80053DA8(0x504);
            func_8004FA2C(p, s);
        }
    }
}

void func_8004FAA4(u8 *);

void func_8004FC2C(u32 *p) {
    ((void (*)(u32 *)) p[0])(p);
    func_8004FAA4((u8 *) p);
}

u32 func_80021448(u8 *);

void func_8004FC68(u8 *p) {
    *(u32 *) (p + 0x24) = func_80021448(p + 0x10);
}

extern u8 D_80071784[];

s32 func_80018174(s32, s32, s32, s32, s32);
void func_8004F884(u8 *);

s32 func_8004FC98(u8 *p)
{
    s32 i;

    for (i = 0; i < *(s32 *)(p + 0x24); i++)
    {
        u8 *t = D_80071784 + *(s32 *)(p + 0x30) * 3;
        u8 n = t[i];
        s32 r = func_80018174(*(s32 *)(p + i * 4 + 0x10), i, 0x18, n * 12 - 0x40, n * 30 + 0x200);

        *(s32 *)(p + i * 4 + 4) = r;
        if (r == 0)
        {
            return 0;
        }
    }
    *(s32 *)(p + 0x2C) = 1;
    *(s32 *)(p + 0x28) = 1;
    func_8004F884(p);
    return 1;
}

u32 func_8002137C(s32);

/* The call is the FIRST declaration, so the counter is initialised after it
   and never crosses it: it stays in $v1 and the function spends one
   callee-saved register. Declared before the call, `i` crosses it, takes $s0
   and pushes the pointer to $s1 -- 28 DIFF and two words longer. */
s32 func_8004FD78(u8 *p, s32 b) {
    u32 v = func_8002137C(b);
    s32 i = 0;
    s32 n = *(s32 *) (p + 0x24);

    while (i < n) {
        if (*(u32 *) (p + i * 4 + 0x10) == v) {
            break;
        }
        i++;
    }
    return i;
}

s32 func_8004FD78(u8 *, s32);
s32 func_8004FC98(u8 *);

/* The FAILURE arm is the fall-through (`== 0`), so the `bnez` goes to the
   function-pointer pair; the other way round costs the `j` placement. */
u32 func_8004FDE0(u32 a) {
    UnkNode *n = func_8003FC64(0);
    u8 *e;

    if (n != NULL) {
        e = (u8 *) (n + 1);
        func_8004FC68(e);
        *(u32 *) (e + 0x30) = func_8004FD78(e, a);
        if (func_8004FC98(e) == 0) {
            func_8004FE78((u32) n);
            n = NULL;
        } else {
            n->unk_10 = (u32) func_8004FC2C;
            *(u32 *) ((u8 *) n + 0x20) = (u32) func_8004F9AC;
        }
    }
    return (u32) n;
}

void func_8001832C(u32);

/* The two read-modify-writes at the end are split into both reads then both
   writes, which is what fills the `lhu`'s delay slot with the global's `lui`. */
void func_8004FE78(u32 a) {
    u8 *p = (u8 *) a;
    s32 i;
    u8 *q;
    u16 f;
    u32 g;

    if (p != 0) {
        i = 0;
        q = p + 0x20;
        do {
            if (*(u32 *) (q + 4) != 0) {
                func_8001832C(*(u32 *) (q + 4));
            }
            i++;
            q += 4;
        } while (i < 3);
        f = *(u16 *) (p + 0x1E);
        g = D_800814A0;
        *(u16 *) (p + 0x1E) = f | 0x8000;
        D_800814A0 = g | 0x8000;
    }
}

u32 func_8004FF00(u8 *p) {
    u32 *q = (u32 *) (p + 0x20);

    return q[q[12] + 4];
}

s32 func_800213AC(void);

s32 func_8004FF20(u8 *p) {
    s32 x = 0;

    if (D_800717B4[*(u32 *) (p + 0x1C)] != 0) {
        x = *(s32 *) (p + 0x20);
        if (x == 2) {
            x = func_800213AC() == 2;
        }
    }
    return ((s32 (*)(u8 *, s32)) D_8007179C[*(u32 *) (p + 0x1C)])(p - 0x20, x);
}

s32 func_8004FF20(u8 *);
void func_80050EA8(u8 *);

s32 func_8004FFA8(u8 *p) {
    s32 r = func_8004FF20(p);

    if (r != 0) {
        *(u32 *) (p - 0x10) = *(u32 *) (p + 4);
        func_80050EA8(p - 0x20);
    }
    return r;
}

/* The row pointer is INDEXED off the parameter inside the body: walking
   `p += 4` makes loop.c bias the giv and the eight chains come out off
   offsets 0 and 0x24 instead of 4 and 0x28, a word over. */
void func_8004FFF4(u8 *b) {
    s32 i;

    for (i = 0; i < 6; i++) {
        u8 *p = b + i * 4;

        *(s16 *) (*(u8 **) (*(u8 **) (p + 4) + 4) + 8) = 0;
        *(s16 *) (*(u8 **) (*(u8 **) (p + 0x28) + 4) + 8) = 0x1E;
        *(*(u8 **) (*(u8 **) (p + 0x28) + 4) + 0) = 0x58;
        *(*(u8 **) (*(u8 **) (p + 0x28) + 4) + 1) = 0x58;
        *(*(u8 **) (*(u8 **) (p + 0x28) + 4) + 2) = 0x58;
        *(*(u8 **) (*(u8 **) (p + 4) + 4) + 0) = 0x58;
        *(*(u8 **) (*(u8 **) (p + 4) + 4) + 1) = 0x58;
        *(*(u8 **) (*(u8 **) (p + 4) + 4) + 2) = 0x58;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800500B4);

u32 func_80021300(u32);

/* Returns the tail call's value -- func_80050308 stores it to +0x20 -- so the
   `return` is explicit even though the value is already in $v0 either way. */
u32 func_800502E0(u8 *p) {
    return func_80021300(func_8004FF00(p));
}

s16 func_80053DA8(u16);
void func_80050DA8(u8 *);
u32 func_800502E0(u8 *);
s32 func_8004FF20(u8 *);
s32 func_8004FFA8(u8 *);
void func_80050EA8(u8 *);
s32 func_80049DE8(s32, s32, s32);
void func_800500B4(u32, u8 *);

/* Two shape facts, both about cse.

   D_80083160 is reached through a STRUCT POINTER local, not `(u8 *)` plus byte
   offsets. The pointer gets the address materialised into a register, which is
   the `addiu $a1, $v0, %lo(D_80083160)` the listing shares between all three
   reads -- and, because the reads are then COMPONENT_REFs, MEM_IN_STRUCT_P is
   set on them, so the store to `p + 0xC` invalidates the +8 word and it is
   RE-READ for the second pair of flag tests. Through a `u8 *` base the reads
   are bare dereferences like the store, `true_dependence` lets cse forward the
   value it already has in $a0, and the re-load plus its delay nop go missing.

   The +0x1C word is read TWICE rather than held in a local: once for the
   +0x18 store and once as func_80049DE8's argument. cse forwards the second
   read -- the +0x18 store cannot alias +0x1C, the offsets being constant off
   one base -- but it still needs the value in two registers, which is the
   `addu $v0, $a0, $zero` before the call. A single local is a word short. */
s16 func_80053DA8(u16);
void func_80050DA8(u8 *);
u32 func_800502E0(u8 *);
s32 func_8004FF20(u8 *);
s32 func_8004FFA8(u8 *);
void func_80050EA8(u8 *);
s32 func_80049DE8(s32, s32, s32);
void func_800500B4(u32, u8 *);

void func_80050308(u8 *p)
{
    Unk80083160 *q = &D_80083160;
    u32 f;
    u32 g;
    s32 s;
    s32 c;
    u32 v;

    f = q->unk_08;
    if (f != 0)
    {
        g = q->unk_10;
        if ((g & 0x20) != 0)
        {
            func_80053DA8(0x515);
            func_80050DA8(p - 0x20);
        }
        else if ((g & 0x40) != 0)
        {
            func_80053DA8(0x514);
            *(u32 *)(p + 0x20) = func_800502E0(*(u8 **)(p + 0x28));
            if (func_8004FF20(p) == 0)
            {
                v = *(u32 *)(p - 0x10);
                *(u32 *)(p - 0x10) = (u32)func_8004FFA8;
                *(u32 *)(p + 4) = v;
            }
            else
            {
                func_80050EA8(p - 0x20);
            }
        }
        else if ((f & 0x5000) != 0)
        {
            s = 0;
            if ((g & 0x4000) != 0)
            {
                s = 1;
                *(s32 *)(p + 0xC) = 0;
                *(s32 *)(p + 0x10) = 3;
            }
            else if ((g & 0x1000) != 0)
            {
                s = -1;
                *(s32 *)(p + 0xC) = 0;
                *(s32 *)(p + 0x10) = 3;
            }
            else
            {
                c = *(s32 *)(p + 0xC);
                if (c >= 3)
                {
                    *(s32 *)(p + 0xC) = c - 1;
                    if ((q->unk_08 & 0x4000) != 0)
                    {
                        s = 1;
                        *(s32 *)(p + 0x10) = 2;
                    }
                    else if ((q->unk_08 & 0x1000) != 0)
                    {
                        s = -1;
                        *(s32 *)(p + 0x10) = 2;
                    }
                }
                else
                {
                    *(s32 *)(p + 0xC) = c + 1;
                }
            }
            if (s != 0)
            {
                func_80053DA8(0x502);
                *(s32 *)(p + 0x14) = 0;
                *(s32 *)(p + 0x18) = *(s32 *)(p + 0x1C);
                *(s32 *)(p + 0x1C) = func_80049DE8(*(s32 *)(p + 0x1C), s, 6);
            }
        }
    }
    func_800500B4(*(u32 *)(p + 0x68), p);
}

void func_80050308(u8 *);

/* The argument passes through untouched, which is invisible here and shows up
   at func_80050640. */
void func_80050494(u8 *p) {
    func_80050308(p);
}

void func_80050308(u8 *);

/* Three parameters, two unused -- the caller passes all three. */
void func_800504B4(u8 *p, s32 b, s32 c) {
    s32 i;

    for (i = 1; i < 7; i++) {
        u8 *e = (u8 *) (*(u32 **) (p + 0x68))[i];
        u8 *f = *(u8 **) (e + 8);

        if (*(s16 *) (f + 2) > 0) {
            *(s16 *) (f + 2) = *(u16 *) (f + 2) - 0x100;
        } else {
            *(s16 *) (f + 2) = 0;
        }
    }
    if (*(s16 *) (*(u8 **) ((u8 *) (*(u32 **) (p + 0x68))[6] + 8) + 2) == 0) {
        *(u32 *) p = (u32) func_80050494;
    }
    func_80050308(p);
}

/* Three quotients first, then the stores. The three bytes go through one local
   copy of the +0x4 pointer and the halfword at +0xC re-reads it, which is what
   the two `lw 0x4($a0)` in the listing are: a byte store through a loaded
   pointer invalidates cse's entry for the pointer itself.

   s32 for the second parameter, not the u32 the caller's prototype said: the
   three divisions are signed. */
void func_80050550(u8 *p, s32 b, s32 d) {
    s32 x = b * 80 / d;
    s32 y = -(b << 7) / d;
    s32 z = (b << 10) / d;
    u8 *q = *(u8 **) (p + 4);

    q[2] = x + 0x30;
    q[1] = x + 0x30;
    q[0] = x + 0x30;
    *(s16 *) (*(u8 **) (p + 4) + 0xC) = 0x180 - y;
    *(s16 *) (*(u8 **) (p + 8) + 2) = 0x400 - z;
}

void func_80050550(u8 *, s32, s32);
void func_800504B4(u8 *, s32, s32);

/* The pre-increment is the ARGUMENT: the new value goes in $a1 and is stored
   in the call's delay slot, and the test after the call re-reads the field. */
void func_800505BC(u8 *p, s32 b, s32 c) {
    func_80050550(p + 0x58, ++*(s32 *) (p + 8), 3);
    if (*(s32 *) (p + 8) >= 3) {
        *(s32 *) (p + 8) = 0;
        *(u32 *) p = (u32) func_800504B4;
    }
    func_800504B4(p, b, c);
}

void func_80050550(u8 *, s32, s32);
void func_800505BC(u8 *, s32, s32);

void func_80050640(u8 *p) {
    /* The post-increment is the ARGUMENT: the old value goes in $a1 and the
       incremented one is stored, which is the copy in the listing. */
    func_80050550(p + 0x58, (*(u32 *) (p + 8))++, 3);
    *(u32 *) p = (u32) func_800505BC;
    func_80050308(p);
}

void func_80050694(u8 *p) {
    (*(void (**)(u8 *)) p)(p);
}

extern u8 D_80077E3C[];
extern u8 D_80077E48[];
extern u8 D_80077E54[];
extern u8 D_80077E60[];
extern u8 D_80077E6C[];
extern u8 D_80077E78[];
extern u8 D_80077E84[];
extern u8 D_80077F80[];
extern u8 D_80077F8C[];
extern u8 D_80081485[];

u32 func_80049E6C(s32);
void func_8004FFF4(u8 *);

/* Wires the ten-odd sub-objects of the +0x68 table to their handlers and
   seeds their coordinates: six fixed D_80077Exx tables, one of two chosen by
   D_80081485, then two more; then the 10..15 entries get their +0x00 from
   `q`'s +0x2C with +0x0A stepping by 0x10 from -0x28 and +0x02 by 0xC4, and
   the 1..5 entries get +0x02 stepping by 0xC4 again.

   `s = (u32 *) p[6]` is loaded BEFORE the `if`: the listing has that `lw` in
   the `lbu`'s load-delay slot, which only happens when it is ahead of the
   branch. Left in the store after the join it comes out behind the branch
   with a nop in the slot, and the value and the pointer swap registers -- one
   word over.

   The two loops have SEPARATE cursor and accumulator locals. This is the
   inverse of func_8004AA78, where reusing one counter across two loops was
   what the listing wanted: here it is wrong, because reusing them makes one
   allocno per role and both loops come out on the same registers, where the
   listing walks $a0 then $v1 and accumulates in $a3 then $a0. 18 wrong words.

   Each loop's initialisers are hoisted out of the `for` and ordered the way
   the listing's preheader has them -- the counter, then the accumulator, then
   the rest -- and the `== 0` arm of the D_80081485 test is written FIRST so
   the `!= 0` arm is the one that goes out of line. */
void func_800506BC(u8 **p, u8 *q, s32 c)
{
    u8 *t;
    u32 *s;
    u8 **r;
    u8 **r2;
    s32 z2;
    s32 i;
    s32 y;
    s32 z;

    *(u32 *) p[0] = func_80049E6C(6);
    *(u32 *) p[1] = (u32) D_80077E3C;
    *(u32 *) p[2] = (u32) D_80077E48;
    *(u32 *) p[3] = (u32) D_80077E54;
    *(u32 *) p[4] = (u32) D_80077E60;
    *(u32 *) p[5] = (u32) D_80077E6C;
    s = (u32 *) p[6];
    if (D_80081485[0] == 0)
    {
        t = D_80077F80;
    }
    else
    {
        t = D_80077F8C;
    }
    *s = (u32) t;
    *(u32 *) p[8] = (u32) D_80077E78;
    *(u32 *) p[9] = (u32) D_80077E84;
    *(u32 *) (q + 0x10) = 1;
    *(u32 *) (q + 0x14) = 1;
    func_8004FFF4((u8 *) p);
    i = 10;
    z = 0;
    *(s16 *) (*(u8 **) (p[0] + 4) + 0xA) = 0;
    r = &p[10];
    y = -0x28;
    for (; i < 0x10; i++)
    {
        *(u32 *) r[0] = *(u32 *) (q + 0x2C);
        *(s16 *) (*(u8 **) (r[0] + 4) + 0xA) = y;
        *(s16 *) (*(u8 **) (r[0] + 8) + 2) = z;
        r++;
        y += 0x10;
        z += 0xC4;
    }
    i = 1;
    z2 = 0;
    r2 = &p[1];
    for (; i < 6; i++)
    {
        *(s16 *) (*(u8 **) (r2[0] + 8) + 2) = z2;
        r2++;
        z2 += 0xC4;
    }
    *(s16 *) (*(u8 **) (p[8] + 8) + 6) = -4;
}

void func_80050550(u8 *, s32, s32);

/* Two things, both measured. The two templates are read ENTIRELY through
   pointer locals (`d[0] = D_8002E5D8[0]` with the first element through the
   symbol costs 30 DIFF here, unlike func_8004CCBC where that is the shape --
   the difference is that these stores go to a parameter, not through a double
   dereference). And `d`/`e` must be `u32 *` so the copies are ARRAY_REFs: as
   `*(u32 *) (d + 4)` on a `u8 *` it is 32 DIFF.

   The three tail stores all re-read `*(u8 **) (p + 4)`. cse forwards the
   first one to the register just stored -- which is the target's
   `addu $v1, $a2, $zero` -- and the `sh` through that pointer then
   invalidates the entry, so the next two come out as real loads. Writing them
   through `d` loses the copy and the function is a word short. */
void func_8005084C(u8 *a, u8 *p, u32 *d, u32 *e) {
    u32 *s = D_8002E5D8;
    u32 *t = D_8002E5E8;

    d[0] = s[0];
    d[1] = s[1];
    d[2] = s[2];
    d[3] = s[3];
    e[0] = t[0];
    e[1] = t[1];
    e[2] = t[2];
    *(u32 **) (p + 4) = d;
    *(u32 **) (p + 8) = e;
    *(u32 *) p = 0;
    *(s16 *) (*(u8 **) (p + 4) + 8) = -0x6C;
    *(s16 *) (*(u8 **) (p + 4) + 0xA) = -0x10;
    *(*(u8 **) (p + 4) + 0xF) = 4;
    func_80050550(p, 0, 1);
}

/* Two 0xC-byte primitives; D_80080B84/80 joined this unit's .sdata block (no
   other function in the unit reaches them), so this was verified in a scratch
   file carrying the block. The global store comes BEFORE `q[1] = 0x48` so the
   load's delay slot gets the constant, and `q[0] = 3` is the LAST statement. */
u8 *func_800508F0(u8 *q, u8 *s) {
    *(u32 *) (q + 4) = D_80080B84;
    q[1] = 0x48;
    q[2] = s[0];
    q[3] = s[2];
    q[0xA] = s[4];
    q[0xB] = s[6];
    q += 0xC;
    *(u32 *) (q + 4) = D_80080B80;
    q[1] = 0x48;
    q[2] = s[0];
    q[3] = s[2];
    q[0xA] = s[4];
    q[0xB] = s[6];
    q[0] = 3;
    return q + 0xC;
}

u8 *func_800508F0(u8 *, u8 *);

void func_8005097C(u8 *p) {
    func_800508F0(p, D_80080B88);
    func_800499E8(p + 0x18, (s32) D_80080B88, D_80080B90);
}

void func_8005097C(u8 *);
void func_80050640(u8 *);

void func_800509C4(u8 *p, s32 a) {
    *(s32 *) (p + 0x1C) = a;
    func_8005097C(*(u8 **) (p + 0x2C));
    *(u32 *) p = (u32) func_80050640;
}

void func_800509C4(u8 *, s32);
void func_8005084C(u8 *, u8 *, u32 *, u32 *);

void func_80050A00(u8 *p, s32 b) {
    func_800509C4(p, b);
    func_8005084C(p, p + 0x58, (u32 *) (p + 0x30), (u32 *) (p + 0x40));
    func_800506BC(*(u8 ***) (p + 0x68), p, b);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80050A58);

u8 *func_80049150(u8 *, u32, s32);
void func_80050A58(u32, u8 *);
s32 func_80048FBC(u8 *, s32);

s32 func_80050AF0(u8 *p, s32 b) {
    u8 *q = p + 0x6C;
    s32 ok = 0;
    s32 r;
    u8 *e;

    r = (s32) func_80049004((u8 **) q, b, 0xC);
    *(u32 *) (p + 0x68) = r;
    if (r != 0) {
        e = func_80049150(q, 9, 0xC);
        if (e != 0) {
            func_80050A58(*(u32 *) (p + 0x68), e);
            r = func_80048FBC(q, 0xC);
            *(u32 *) (p + 0x2C) = r;
            ok = r != 0;
        }
    }
    return ok;
}

u32 func_800491F4(s32, u8 *, s32);

void func_80050B7C(s32 a) {
    func_800491F4(a, D_800717BC, 0x11);
}

s32 func_80050AF0(u8 *, s32);
void func_80050A00(u8 *, s32);
u32 func_8004FDE0(u32);
void func_8004E130(void);
void func_8004B248(u32 *);
void func_80050694(u8 *);
void func_8004CAA0(void);
s32 func_8004491C(UnkNode *, u32);

/* func_80050B7C is landed as `void` but its result is stored here, so this
   function needs its own declaration of the same assembler symbol. */
s32 func_80050B7Cr(s32) __asm__("func_80050B7C");

s32 func_80050BA4(s32 a)
{
    u8 *x;
    u8 *y;
    u8 *m;
    s32 r;
    u16 f;
    u32 g;

    x = (u8 *) func_8003FC64(0);
    if (x == 0)
    {
        return 0;
    }
    y = (u8 *) func_8003FC64(0);
    if (y != 0)
    {
        m = y + 0x20;
        *(u32 *)(m + 0xA0) = (u32)x;
        if (func_80050AF0(m, 0x11) != 0)
        {
            *(u32 *)(y + 0xC) = (u32)(y + 0x78);
            *(u32 *)(m + 0x64) = func_80050B7Cr(*(s32 *)(m + 0x68));
            func_80050A00(m, a);
            *(u32 *)(y + 0x10) = (u32)func_80050694;
            r = func_8004FDE0(2);
            *(u32 *)(m + 0x28) = r;
            if (r != 0)
            {
                func_8004E130();
                func_8004491C((UnkNode *)y, (u32)func_8004CAA0);
                return (s32)x;
            }
        }
        func_8004B248((u32 *)(m + 0x6C));
        f = *(u16 *)(y + 0x1E);
        g = D_800814A0;
        *(s16 *)(y + 0x1E) = f | 0x8000;
        D_800814A0 = g | 0x8000;
    }
    return (s32)x;
}

s32 func_80050BA4(s32);

void func_80050CAC(void) {
    s32 a = 0;

    if (D_80080B94 != 0) {
        a = D_80080B98;
    }
    func_80050BA4(a);
}

void func_8004B248(u32 *);

void func_80050CDC(u8 *p, s32 b, u8 **q) {
    u8 *e;
    u8 v;
    s32 n;

    e = q[2];
    *(u16 *) (e + 2) = *(u16 *) (e + 2) - 0x40;
    e = q[1];
    *(u16 *) (e + 0xC) = *(u16 *) (e + 0xC) + 0x20;
    e = q[1];
    *(u16 *) (e + 8) = *(u16 *) (e + 8) - 0x40;
    e = q[1];
    v = e[2] - 0x20;
    e[2] = v;
    e[1] = v;
    e[0] = v;
    n = *(s32 *) (p + 8) - 1;
    *(s32 *) (p + 8) = n;
    if (n == -1) {
        func_8004B248((u32 *) (p + 0x6C));
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_80050CDC(u8 *, s32, u8 **);
void func_8004FE78(u32);

/* $gp group: the store to D_80080B98 is %gp_rel because the symbol is in this
   unit's sdata_entries block at the top of the file -- so this one has to be
   verified in a scratch file carrying that block, not by sweep.py (which
   compiles standalone and would report it wrong).

   Both loads are read into locals before the two stores through `q`: sched
   cannot lift a load above a store it cannot disambiguate, so left inside the
   call and the assignment they come out after both stores (4 DIFF). */
void func_80050DA8(u8 *p) {
    u8 *q = p + 0x20;

    if (p != 0) {
        u32 v;
        u32 arg;
        u8 *e;
        u32 g;
        u16 f;

        *(u32 *) (q + 8) = 2;
        v = *(u32 *) (q + 0x1C);
        arg = *(u32 *) (q + 0x28);
        *(u32 *) (q - 0x10) = (u32) func_80050CDC;
        D_80080B98 = v;
        func_8004FE78(arg);
        e = *(u8 **) (q + 0xA0);
        g = D_800814A0;
        f = *(u16 *) (e + 0x1E);
        D_800814A0 = g | 0x8000;
        *(u16 *) (e + 0x1E) = f | 0x8000;
        *(u32 *) (q + 0x28) = 0;
    }
}

void func_8004FE78(u32);
void func_8004B248(u32 *);

/* $gp group (D_80080B98), so verified in a scratch file with the unit's .sdata
   block. The +0x28 clear goes BEFORE the global store, which is what leaves
   the call's argument for the `jal`'s delay slot, and the last two
   read-modify-writes are split into read/read/write/write: whole, they come
   out as two serialised groups (15 DIFF). */
void func_80050E20(u8 *p) {
    u8 *q = p + 0x20;
    u32 g;
    u16 f;

    func_8004FE78(*(u32 *) (q + 0x28));
    *(u32 *) (q + 0x28) = 0;
    D_80080B98 = *(s32 *) (q + 0x1C);
    func_8004B248((u32 *) (p + 0x8C));
    *(u16 *) (*(u8 **) (q + 0xA0) + 0x1E) |= 0x8000;
    g = D_800814A0;
    f = *(u16 *) (p + 0x1E);
    D_800814A0 = g | 0x8000;
    *(u16 *) (p + 0x1E) = f | 0x8000;
}

void func_80050EA8(u8 *p) {
    u8 *q = p + 0x20;
    u32 v = *(u32 *) (p + 0x10);

    if (v != 0) {
        *(u32 *) (q + 0x24) = v;
        *(u32 *) (p + 0x10) = 0;
    }
    func_80044A50((UnkNode *) p);
    func_8004FE78(*(u32 *) (q + 0x28));
    *(u32 *) (q + 0x28) = 0;
}

u32 func_8004FDE0(u32);
void func_8004CAA0(void);

u32 func_80050EF8(u8 *p) {
    u8 *q = p + 0x20;

    if (*(u32 *) (q + 0x28) != 0 ||
        (*(u32 *) (q + 0x28) = func_8004FDE0(*(u32 *) (q + 0x20))) != 0) {
        if (*(u32 *) (q + 0x24) != 0) {
            *(u32 *) (p + 0x10) = *(u32 *) (q + 0x24);
            *(u32 *) (q + 0x24) = 0;
        }
        func_8004491C((UnkNode *) p, (u32) func_8004CAA0);
    }
    return *(u32 *) (q + 0x28);
}

void func_80050F74(u8 *p, u32 v) {
    u32 *q = (u32 *) (p + 0x20);

    q[6] = q[7] = v;
}

void func_8003AEF8(u8 *);

void func_80050F84(void) {
    func_8003AEF8(D_800717D0);
}

void func_8003AEF8(u8 *);

void func_80050FA8(void) {
    func_8003AEF8(D_800717E8);
}

void func_80050FCC(void) {
    func_80041284(&D_80080B9C);
}

extern Unk41284 D_80080BAC;
void func_8003E140(s8 *);
void func_80041344(u32, u32);

void func_80050FF4(u8 *p) {
    *p = 0;
    func_8003E4FC(6, (u8 *) &D_80080BAC, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    *p = 0;
    func_8003E4FC(6, (u8 *) &D_80080B9C, 0);
    func_8003E4FC(0xFF, (u8 *) func_8003E140, (s32) p);
    func_8003F320();
}

void func_80050FF4(u8 *);
u32 func_80126890(u32);

void func_800510A0(u8 *p) {
    func_80050FF4(p + 0xC);
    *(u32 *) (p + 0x8) = func_80126890(*(u32 *) (p + 0x4));
}

void func_80051228(u8 *);

void func_800510DC(u8 *p) {
    if ((*(u16 *) (*(u8 **) (p + 4) + 0x1E) & 0x2000) != 0) {
        func_80051228(p);
        *(u16 *) (*(u8 **) p + 0x1E) |= 0x2000;
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_800510DC(u8 *);
void func_800510A0(u8 *);

void func_80051164(u8 *p) {
    if (func_80044144(1, 0x1E, (u32) func_800510A0, (u32) p) != 0) {
        *(u32 *) (*(u8 **) (p + 4) + 0x10) = (u32) func_800510DC;
    }
}

void func_80051164(u8 *);

UnkNode *func_800511B4(UnkNode *p) {
    UnkNode *n = func_8003FC64(0);
    u8 *e;

    if (n != NULL) {
        *(UnkNode **) (n + 1) = p;
        n->unk_1E &= 0xDFFF;
        p->unk_1E &= 0xDFFF;
        e = (u8 *) (n + 1);
        *(UnkNode **) (e + 4) = n;
        e[0xC] = 0;
        n->unk_10 = (u32) func_80051164;
    }
    return n;
}

/* The five-stage load screen. The repeated block is func_80050FF4's, spelled
   the same way, five times over; the fifth takes its name from a nested
   dispatch. Three things decide it:

   * `func_8003E4FC(6, <addr>, 0)` is called IN EACH ARM, not once after the
     switch on a `name` local. Cross-jumping merges the longest common suffix,
     which runs from the `addu $a2, $zero, $zero` in the shared `jal`'s delay
     slot backwards and stops at the differing address -- so `addiu $a0, 6`
     and the `lui`/`addiu` of the name stay in every arm, which is what the
     listing has (four words).
   * the outer switch needs a case label BELOW 2 sharing the default's body.
     With only `case 2`/`case 3`, `emit_case_nodes` takes its own shortcut --
     "omit the conditional branch to default if we avoid only one right
     child" -- and emits two plain equality tests. With a third node the root
     is 2 with a left child and a right child, neither bounded, so it emits
     `slt x, 3` to split them; jump.c then deletes the left child's test
     (its label is the default's) and inverts the branch, leaving exactly the
     listing's `slti`/`bnez`. Two words. The compiler only pins the value to
     be less than 2: `case 0:` and `case 1:` are byte-identical here.
   * the default arm is written FIRST, because the listing lays its inner
     dispatch and the four name blocks out before `case 2`'s one-liner and
     `case 3`'s inner dispatch -- gcc emits bodies in source order and
     reorders the decision tree to the front.

   func_8003E140's address is hoisted into $s0 for the first four blocks and
   re-materialised for the fifth: cse restarts at the switch's join, which is
   the per-join rule in matching-levers section 6. */
extern Unk41284 D_80080BB4;
extern Unk41284 D_80080BBC;
extern Unk41284 D_80080BC4;
extern Unk41284 D_80080BCC;
extern Unk41284 D_80080BD4;
extern Unk41284 D_80080BDC;
extern Unk41284 D_80080BE4;
extern Unk41284 D_80080BEC;
extern Unk41284 D_80080BF4;
extern Unk41284 D_80080BFC;
extern Unk41284 D_80080C04;
extern Unk41284 D_80080C0C;
extern Unk41284 D_80080C14;
extern u8 D_800D381A;
extern u8 D_800136B8;
extern u8 D_800D2FB4[];
extern u8 D_8012F004[];

void func_8003E140(s8 *);
void func_80041344(u32, u32);
void func_80046E38(s16, u8 *);

void func_80051228(u8 *unused)
{
    u8 buf;

    buf = 0;
    func_8003E4FC(6, (u8 *)&D_80080BB4, 0);
    func_8003E4FC(0xFF, (u8 *)func_8003E140, (s32)&buf);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    buf = 0;
    func_8003E4FC(6, (u8 *)&D_80080BBC, 0);
    func_8003E4FC(0xFF, (u8 *)func_8003E140, (s32)&buf);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    buf = 0;
    func_8003E4FC(6, (u8 *)&D_80080BC4, 0);
    func_8003E4FC(0xFF, (u8 *)func_8003E140, (s32)&buf);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    buf = 0;
    func_8003E4FC(6, (u8 *)&D_80080BCC, 0);
    func_8003E4FC(0xFF, (u8 *)func_8003E140, (s32)&buf);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    buf = 0;
    switch (D_800D381A)
    {
    case 0:
    default:
        switch (D_800136B8)
        {
        default:
            func_8003E4FC(6, (u8 *)&D_80080BD4, 0);
            break;
        case 0xB:
            func_8003E4FC(6, (u8 *)&D_80080BDC, 0);
            break;
        case 0xC:
            func_8003E4FC(6, (u8 *)&D_80080BE4, 0);
            break;
        case 0xD:
            func_8003E4FC(6, (u8 *)&D_80080BEC, 0);
            break;
        }
        break;
    case 2:
        func_8003E4FC(6, (u8 *)&D_80080BF4, 0);
        break;
    case 3:
        switch (D_800136B8)
        {
        default:
            func_8003E4FC(6, (u8 *)&D_80080BFC, 0);
            break;
        case 0xB:
            func_8003E4FC(6, (u8 *)&D_80080C04, 0);
            break;
        case 0xC:
            func_8003E4FC(6, (u8 *)&D_80080C0C, 0);
            break;
        case 0xD:
            func_8003E4FC(6, (u8 *)&D_80080C14, 0);
            break;
        }
        break;
    }
    func_8003E4FC(0xFF, (u8 *)func_8003E140, (s32)&buf);
    func_8003F320();
    func_80041344(0x80020000, D_80081480);
    DrawSync(0);
    func_80046E38(D_800D2FB4[D_800D381A * 0x20], D_8012F004);
}

void func_80051500(void) {
    func_80041284(&D_80080C1C);
}

void func_80051548(u8 *, u8 *, u8 *);

void func_80051528(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051548) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

/* func_80051F58's sibling, and it wants the same two things: the switch
   subject as `s32`, not `s16`, and the default's three byte stores written
   out. Two differences of its own.

   `func_800478B8` is called with ONE argument here. Its definition in
   unk_1AF38 takes two, but the listing sets only $a0 and csrc/unk_72A8.c
   already calls it with one, so the one-argument spelling is what this unit's
   original had.

   Cases 0 and 2 share a body -- both reach the +0xC check -- and case 1's tail
   shares the +4 increment with them, which jump2 cross-joins. */
extern u32 D_800814A0;

void func_800478B8(s32);
s32 func_800644B8(s32);
s32 func_80064584(s32);

void func_80051548(u8 *p, u8 *q, u8 *r)
{
    s32 s;

    *(u16 *)(p + 6) = *(u16 *)(p + 6) + 1;
    func_800478B8((s32)r);
    s = *(s16 *)(p + 4);
    switch (s)
    {
    case 1:
        *(s32 *)(q + 0) = 0x1C00000 - (func_800644B8(*(s16 *)(p + 6) * 0x10) >> 4) * 0xC000;
        *(s32 *)(q + 4) = ((func_80064584(*(s16 *)(p + 6) * 0x10) >> 4) << 13) + 0x900000;
        if (*(s16 *)(p + 6) >= 0x40)
        {
            *(s32 *)(q + 0) = 0x1000000;
            *(s32 *)(q + 4) = 0x900000;
            *(s32 *)(q + 8) = 0x700000;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 0:
    case 2:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 3:
        *(s32 *)(q + 0) = *(s32 *)(q + 0) + 0xFFF40000;
        *(s32 *)(q + 4) = *(s32 *)(q + 4) + 0x20000;
        if (*(s16 *)(p + 6) < 0x40)
        {
            return;
        }
        break;
    default:
        r[0xE] = 0;
        r[0xD] = 0;
        r[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0xC) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

extern UnkElem D_80072044[];
void func_80051548(u8 *, u8 *, u8 *);
void func_80044BB0(void);

/* func_800520B4's recipe, with a SEPARATE local for the +8 block at the end:
   reusing `e` gives that block $a0 where the target has $v1 (4 DIFF). */
void func_80051708(u8 *p, s32 a) {
    u8 *q;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_80051548;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0x808080;
    func_8003DB94((UnkSlot *) e, D_80072044, 0);
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0x1C00000;
    *(u32 *) (f + 4) = 0xC00000;
    *(u32 *) (f + 8) = 0x800000;
}

void func_800517CC(void);

void func_800517AC(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_800517CC) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800517CC);

extern UnkElem D_800720AC[];
void func_800517CC(void);
void func_80044BB0(void);

void func_80051BFC(u8 *p, s32 a) {
    u8 *q;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_800517CC;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 8) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0x808080;
    func_8003DB94((UnkSlot *) e, D_800720AC, 0);
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0x1A00000;
    *(u32 *) (f + 4) = 0x600000;
    *(u32 *) (f + 8) = 0x700000;
}

void func_80051CC4(u8 *, s32, u8 *);

void func_80051CA4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051CC4) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

extern u32 D_80071A68[];

/* func_80051F58's family again, and the same two declaration facts carry it:
   the switch subject as `s32`, and the UNSIGNED read of +4 as a second local
   that only case 2 uses. The extras here:

   `a` is `u16` and the `!= 7` test and the `% 7` are on `(s16) a`. The listing
   stores the unextended value to +0xA (`sh $v0`) and sign-extends a COPY for
   the comparison and the division, so the cast goes on the uses and not on the
   variable.

   The table index is `s16`, which is what folds `sll 16` / `sra 16` / `sll 2`
   into the listing's `sll 16` / `sra 14`. Same spelling as func_80051D18's
   `D_80071A68[*(s16 *) (q + 0xA)]` below. */
void func_80051CC4(u8 *p, s32 b, u8 *q)
{
    s32 s;
    u16 w;
    u16 n;
    u8 v;
    u16 a;
    s16 m;

    n = *(u16 *)(p + 6) + 1;
    s = *(s16 *)(p + 4);
    w = *(u16 *)(p + 4);
    *(u16 *)(p + 6) = n;
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 1:
        v = q[0xE] + 4;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) >= 0x20)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 2:
        if ((s16)n >= 0x3F)
        {
            *(u16 *)(p + 4) = w + 1;
            *(u16 *)(p + 6) = 0;
        }
        return;
    case 3:
        v = q[0xE] - 4;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) < 0x20)
        {
            return;
        }
        a = *(u16 *)(p + 0xA) + 1;
        *(u16 *)(p + 0xA) = a;
        if ((s16)a != 7)
        {
            m = (s16)a % 7;
            *(u16 *)(p + 0xA) = m;
            *(u32 *)(q + 8) = D_80071A68[m];
            *(s16 *)(p + 4) = 0;
            *(u16 *)(p + 6) = 0;
            return;
        }
        break;
    default:
        q[0xE] = 0;
        q[0xD] = 0;
        q[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0x14) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

extern u32 D_80071A68[];
void func_80051CC4(u8 *, s32, u8 *);
void func_80044BB0(void);

void func_80051E94(u8 *p, s32 a) {
    u8 *q;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_80051CC4;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u16 *) (q + 0xC) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0;
    *(u32 *) (e + 8) = D_80071A68[*(s16 *) (q + 0xA)];
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0x380000;
    *(u32 *) (f + 4) = 0x280000;
    *(u32 *) (f + 8) = 0x900000;
}

void func_80051F58(u8 *, s32, u8 *);

void func_80051F38(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_80051F58) {
        *(u16 *) (p + 0x2C) = 1;
    }
}

/* Three declaration facts, each worth a word or two:

   * The switch subject is `s32`, not `s16`. As an `s16` the pseudo is HImode,
     so the read is a `lhu` and the switch sign-extends it with `sll`/`sra`
     where the target has one `lh`.

   * `w`, the UNSIGNED read of the same halfword, is a second load in the
     listing -- `lh $v1, 0x4($a0)` and `lhu $a3, 0x4($a0)` back to back --
     because cse relates a sign-extend and a zero-extend of one `(mem:HI)`
     only one way: with both reads before the +6 store, whichever it folds
     costs the `sll`/`sra` pair. Reading `w` before the store works only with
     the `s32` spelling above; reading it after the store also works (the
     store invalidates all memory, both addresses being p-relative bare
     dereferences), and is a word worse.

   * The three byte stores take ONE value in a local. `q[0xD] = q[0xE]` after
     the store to +0xE re-reads it: two words in each of the two arms.

   The cases are written 0, 1, 2, 3, default, which is the listing's physical
   order -- case 0's tail is cross-jumped into case 1's, so case 0 has to come
   first. */
void func_80051F58(u8 *p, s32 b, u8 *q)
{
    s32 s;
    u16 w;
    u16 n;
    u8 v;

    n = *(u16 *)(p + 6) + 1;
    s = *(s16 *)(p + 4);
    w = *(u16 *)(p + 4);
    *(u16 *)(p + 6) = n;
    switch (s)
    {
    case 0:
        if (*(s16 *)(p + 0xC) != 0)
        {
            *(s16 *)(p + 0xC) = 0;
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 1:
        v = q[0xE] + 2;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) >= 0x40)
        {
            *(u16 *)(p + 6) = 0;
            *(u16 *)(p + 4) = *(u16 *)(p + 4) + 1;
        }
        return;
    case 2:
        if ((s16)n >= 0x100)
        {
            *(u16 *)(p + 4) = w + 1;
            *(u16 *)(p + 6) = 0;
        }
        return;
    case 3:
        v = q[0xE] - 2;
        q[0xE] = v;
        q[0xD] = v;
        q[0xC] = v;
        if (*(s16 *)(p + 6) < 0x40)
        {
            return;
        }
        break;
    default:
        q[0xE] = 0;
        q[0xD] = 0;
        q[0xC] = 0;
        break;
    }
    *(u32 *)(*(u8 **)p + 0x18) = 0;
    *(u16 *)(*(u8 **)p + 6) = *(u16 *)(*(u8 **)p + 6) + 1;
    *(u16 *)(p - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80051F58(u8 *, s32, u8 *);
void func_80044BB0(void);

/* func_8004491C's first argument is the pass-through: nothing sets $a0 before
   that `jal`, so `p` is still there. */
void func_800520B4(u8 *p, s32 a) {
    u8 *q;
    u8 *e;

    *(u32 *) (p + 0x10) = (u32) func_80051F58;
    *(u32 *) (p + 0x20) = a;
    q = p + 0x20;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u16 *) (q + 0xC) = 0;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 8) = (u32) D_80071FF0;
    *(u32 *) (e + 0xC) = 0;
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    e = *(u8 **) (p + 8);
    *(u32 *) (e + 0) = 0x980000;
    *(u32 *) (e + 4) = 0x800000;
    *(u32 *) (e + 8) = 0xA00000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052144);

extern u8 D_80071E44[];

UnkNode *func_8003FC64(s32);
u8 *func_8003FD64(s32, u8 *);
void func_8003F540(u32, u32, u32, u32);
s16 func_80053DA8(u16);
s32 func_80033B2C(s32);
void func_80052144(void);
void func_80051708(u8 *, s32);
void func_80051BFC(u8 *, s32);
void func_80051E94(u8 *, s32);
void func_800520B4(u8 *, s32);
void func_80053374(u8 *, s32);

/* The handle takes a BLOCK-SCOPED local in each of the five slots, not one
   shared `h` across all of them. As one variable it is a single allocno with
   twenty references over a short total live length, which outranks `s` -- the
   sub-struct base, thirty references but live across the whole body -- in
   global-alloc's priority, so it takes $s0 and `s` takes $s1, the reverse of
   the listing. Five separate allocnos each lose to `s`. 45 DIFF either way at
   the exact word count, every word of it those two registers. */
void func_800525D4(void)
{
    UnkNode *n = func_8003FC64(0);
    u8 *s;

    if (n != NULL)
    {
        s = (u8 *)(n + 1);
        n->unk_10 = (u32)func_80052144;
        *(u16 *)(n + 1) = 0;
        *(u16 *)(s + 2) = 0;
        *(u16 *)(s + 4) = 0;
        *(u16 *)(s + 6) = 0;
        *(u32 *)(s + 8) = (u32)D_80071E44;
        *(u32 *)(s + 0xC) = 0;
        *(u32 *)(s + 0x10) = 0;
        *(u32 *)(s + 0x14) = 0;
        *(u32 *)(s + 0x18) = 0;
        *(u32 *)(s + 0x1C) = 0;
        func_8003F540(0, 0x15B7C, 0, 0x10093C8);
        func_80053DA8(0x300);
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80051708(h, (s32)s);
                *(u32 *)(s + 0xC) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        if (func_80033B2C(0x1392) == 0)
        {
            {
                u8 *h = func_8003FD64(0x212, (u8 *)n);

                if (h != NULL)
                {
                    func_80051BFC(h, (s32)s);
                    *(u32 *)(s + 0x10) = (u32)h;
                    *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
                }
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80051E94(h, (s32)s);
                *(u32 *)(s + 0x14) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_800520B4(h, (s32)s);
                *(u32 *)(s + 0x18) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
        {
            u8 *h = func_8003FD64(0x212, (u8 *)n);

            if (h != NULL)
            {
                func_80053374(h, (s32)s);
                *(u32 *)(s + 0x1C) = (u32)h;
                *(u16 *)(s + 4) = *(u16 *)(s + 4) + 1;
            }
        }
    }
}

/* Three stacked LINE_F2 primitives -- a bright one at y and two dimmed ones at
   y-1 and y+1 -- with the brightness taken from the fade step `n`. Six
   spellings, and every one of them was worth words:

   * `n` is used DIRECTLY, with no `s32` copy. An `s16` parameter's
     sign-extension is emitted at its first integer USE, not by assign_parms,
     so it lands after the `&D_80083160` materialisation, which is the
     listing's order. `s32 k = n;` as the first statement puts the `sll`/`sra`
     at the top and the `lui`/`addiu` behind it -- four words, and the only
     thing left after everything else was right.
   * TWO primitive-pointer variables: `p` for the first line and `q` for the
     second and third. One variable is one pseudo and one register; the
     listing has $t0 for the first and $a2 shared by the other two.
   * the dimmed colours are assigned back into `c` and `d`. With separate
     locals there are nine long-lived quantities and the allocation spills
     into $a0/$a2/$a3, where the listing keeps seven in $t0..$t6 by letting
     each colour die into its dimmed value (34 differing words).
   * the dimming is `((u32)(c / 3) << 16) >> 15`, which is the listing's
     `sll 16` / `srl 15`. `(c / 3) * 2` makes the second shift arithmetic
     (`sra`), and `(u16)(c / 3) * 2` comes out as `andi 0xFFFF` / `sll 1`.
   * `d` is computed AFTER the `c == 0x100` test, so reorg has it to put in
     the `bne`'s delay slot; before the test, the slot takes a nop.
   * the four coordinate stores are written out in BOTH arms of each
     `if (x > 0)`. Cross-jumping merges the three identical trailing `sh`s
     into the join and leaves the differing `sh 0x8` and the duplicated
     `x + w - 1` in each arm, which is exactly what the listing has. */
void func_80052774(s16 x, s16 y, s16 w, s16 n)
{
    Unk80083160 *s = &D_80083160;
    u8 *p;
    u8 *q;
    s16 c;
    s16 d;

    if (n < 0x11)
    {
        c = n * 8 + 0x80;
        if (c == 0x100)
        {
            c = 0xFF;
        }
        d = n * 8 + 0x40;
    }
    else
    {
        c = 0x200 - n * 16;
        d = 0x100 - n * 8;
    }
    p = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = p + 0x10;
    p[3] = 3;
    p[7] = 0x40;
    p[4] = d;
    p[5] = d;
    p[6] = c;
    if (x > 0)
    {
        *(s16 *)(p + 8) = 0;
        *(s16 *)(p + 0xA) = y;
        *(s16 *)(p + 0xC) = x + w - 1;
        *(s16 *)(p + 0xE) = y;
    }
    else
    {
        *(s16 *)(p + 8) = x;
        *(s16 *)(p + 0xA) = y;
        *(s16 *)(p + 0xC) = x + w - 1;
        *(s16 *)(p + 0xE) = y;
    }
    addPrim(s->unk_00 + 0x230, p);
    c = ((u32)(c / 3) << 16) >> 15;
    d = ((u32)(d / 3) << 16) >> 15;
    q = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = q + 0x10;
    q[3] = 3;
    q[7] = 0x42;
    q[6] = c;
    q[4] = d;
    q[5] = d;
    if (x > 0)
    {
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 0xA) = y - 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y - 1;
    }
    else
    {
        *(s16 *)(q + 8) = x;
        *(s16 *)(q + 0xA) = y - 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y - 1;
    }
    addPrim(s->unk_00 + 0x230, q);
    q = *(u8 **)(s->unk_00 + 0x8D0);
    *(u8 **)(s->unk_00 + 0x8D0) = q + 0x10;
    q[3] = 3;
    q[7] = 0x42;
    q[4] = d;
    q[5] = d;
    q[6] = c;
    if (x > 0)
    {
        *(s16 *)(q + 8) = 0;
        *(s16 *)(q + 0xA) = y + 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y + 1;
    }
    else
    {
        *(s16 *)(q + 8) = x;
        *(s16 *)(q + 0xA) = y + 1;
        *(s16 *)(q + 0xC) = x + w - 1;
        *(s16 *)(q + 0xE) = y + 1;
    }
    addPrim(s->unk_00 + 0x230, q);
}

/* The counter is consumed and incremented in one expression at the TOP of the
   body (`i++ << 4`), which is where the target's `addiu $a0, $a0, 1` sits --
   as a `for`'s increment it comes out last (12 DIFF). The ten stores go
   through a walked pointer, which loop.c splits into the two induction
   variables the listing has: one at +0 and one biased to +0xB. */
void func_80052A20(void) {
    u8 *p = D_80084478;
    s32 i = 0;

    do {
        u8 v = i++ << 4;

        p[0] = 0xC0;
        p[1] = 0x2C;
        p[2] = 0;
        p[3] = 0;
        *(s16 *) (p + 4) = 0xE;
        *(s16 *) (p + 6) = 0x7080;
        p[8] = 0;
        p[9] = v;
        p[0xA] = 0xFC;
        p[0xB] = 0x10;
        p += 0xC;
    } while (i < 0x10);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052A90);

void func_80052A90(u32, s32, s16);

void func_80052C10(s16 k, u8 *p) {
    RECT r1;
    RECT r2;

    r1.x = 0x380;
    r1.y = k * 16;
    r1.w = 0x40;
    r1.h = 0x10;
    ClearImage(&r1, 0, 0, 0);
    r2.x = 0x3C0;
    r2.y = k * 16;
    r2.w = 0x40;
    r2.h = 0x10;
    ClearImage(&r2, 0, 0, 0);
    func_80052A90(*(u32 *) (p + 8), 0x380, k * 16);
    func_80052A90(*(u32 *) (p + 0xC), 0x3C0, k * 16);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80052CE0);

void func_80052CE0(void);
void func_80044BB0(void);

/* The 0x808080 store comes FIRST of the five through `e`: anywhere else and
   the `ori` completing that constant is scheduled before the handler
   pointer's lui/addiu instead of after the `addiu $v0, $s0, 0x20` (6 DIFF one
   slot later, 22 two). */
void func_80052FE8(u8 *p, s16 k) {
    u8 *q = p + 0x20;
    u8 *e;
    u8 *f;

    *(u32 *) (p + 0x10) = (u32) func_80052CE0;
    *(u16 *) (p + 0x20) = 0;
    *(u16 *) (q + 2) = 0;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 0xC) = 0;
    *(u16 *) (q + 0xE) = 0;
    *(u16 *) (q + 8) = k;
    e = *(u8 **) (p + 0xC);
    *(u32 *) (e + 0xC) = 0x808080;
    *(u16 *) (e + 0x1E) = 0x1000;
    *(u16 *) (e + 0x1C) = 0x1000;
    *(u32 *) (e + 8) = (u32) (D_80084478 + k * 12);
    *(u16 *) (e + 0x14) = *(u16 *) (e + 0x14) | 0x80;
    func_8004491C((UnkNode *) p, (u32) func_80044BB0);
    f = *(u8 **) (p + 8);
    *(u32 *) (f + 0) = 0;
    *(u32 *) (f + 4) = 0;
    *(u32 *) (f + 8) = 0x600000;
}

void func_8005313C(void);

void func_800530A4(u8 *p) {
    if (*(u32 *) (p + 0x10) == (u32) func_8005313C) {
        *(u16 *) (p + 0x2A) = 1;
    }
}

/* Both read-modify-writes in the loop are split into a read, a read, then the
   two writes: cc1psx fills a load's delay slot with the insn that follows it
   in the source and nothing else, so the global's `lw` needs the entry's `lhu`
   next (17 DIFF with the two `|=` written whole, one nop each).

   The last `D_800814A0 |= 0x8000` re-uses the value the loop left in the
   register -- section 5's exception, since the global's address does not
   vary. */
void func_800530C4(u8 *p) {
    u8 **b = (u8 **) (p + 0x20);
    s32 i;

    *(u32 *) (*b + 0x1C) = 0;
    *(u16 *) (*b + 6) += 1;
    for (i = 0; i < 0x10; i++) {
        u8 *e = b[i + 4];
        u32 g = D_800814A0;
        u16 f = *(u16 *) (e + 0x1E);

        D_800814A0 = g | 0x8000;
        *(u16 *) (e + 0x1E) = f | 0x8000;
    }
    *(u16 *) ((u8 *) b - 2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005313C);

extern u8 D_80071A84[];
void func_8005313C(void);
u8 *func_8003FD64(s32, u8 *);
void func_80052FE8(u8 *, s16);
void func_80052A20(void);

void func_80053374(u8 *p, s32 a) {
    u8 *q = p + 0x20;
    s32 i;

    *(u32 *) (p + 0x10) = (u32) func_8005313C;
    *(u32 *) (p + 0x20) = a;
    *(u16 *) (q + 4) = 0;
    *(u16 *) (q + 6) = 0;
    *(u16 *) (q + 8) = 0;
    *(u16 *) (q + 0xA) = 0;
    *(u32 *) (q + 0xC) = (u32) D_80071A84;
    for (i = 0; i < 0x10; i++) {
        u8 *e = func_8003FD64(0x212, p);

        if (e != 0) {
            func_80052FE8(e, i);
            *(u32 *) (q + i * 4 + 0x10) = (u32) e;
        }
    }
    func_80052A20();
}

/* func_80053604's twin, the fade-IN: the grey level is `0x100 - (p->unk_02 <<
   8) / p->unk_04` instead of the quotient itself, the guard returns 1 when
   +0x02 has caught up with +0x04, and +0x02 is advanced at the end rather
   than conditionally. Every spelling in func_80053604's comment applies here
   unchanged -- `col` reused for the quotient, the inner grouping of the grey
   ramp explicit, the eight vertex halfwords in value order, `m = 1` then the
   override, and `addPrim` for the two OT links. */
s32 func_80053428(Unk80083120 *p)
{
    Unk80083160 *s;
    u8 *q;
    u8 *r;
    s32 m;
    u32 col;

    s = &D_80083160;
    if (p->unk_02 >= p->unk_04)
    {
        return 1;
    }
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = q + 0xC;
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = r + 0x18;
    m = 1;
    if (p->unk_06 == 0)
    {
        m = 2;
    }
    func_80067F20((u32 *)q, 0, 0, GetTPage(0, m, 0, 0), 0);
    if (p->unk_02 == 0)
    {
        col = 0xFFFFFF;
    }
    else
    {
        col = 0x100 - ((p->unk_02 << 8) / p->unk_04);
        col = col + ((col << 8) + (col << 16));
    }
    *(u32 *)(r + 4) = col;
    SetPolyF4((POLY_F4 *)r);
    SetSemiTrans(r, 1);
    *(s16 *)(r + 0x14) = 0x140;
    *(s16 *)(r + 0x10) = 0x140;
    *(s16 *)(r + 0xC) = 0;
    *(s16 *)(r + 8) = 0;
    *(s16 *)(r + 0x12) = 0;
    *(s16 *)(r + 0xA) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    *(s16 *)(r + 0xE) = 0xE0;
    addPrim(s->unk_00 + 0x70, r);
    addPrim(s->unk_00 + 0x70, q);
    p->unk_02 = p->unk_02 + 1;
    return 0;
}

/* The screen fade quad: allocate a 0xC-byte tpage primitive and a 0x18-byte
   POLY_F4 out of the +0x8D0 bump allocator, build a full-screen semi-transparent
   quad whose grey level tracks +0x02 against +0x04, and link both into the OT
   at +0x70. Five spellings:

   * `col` is REUSED for the quotient. With a separate `k` local the two are
     different quantities and land in $v0 and $v1, where the listing has the
     `mflo` and the final `addu` both writing $a1.
   * the grey ramp is `col + ((col << 8) + (col << 16))`, with the inner
     grouping explicit. `col * 0x10101` makes synth_mult pick the three-op
     factorisation `((k << 8) + k) << 8) + k`, and the left-grouped
     `col + (col << 8) + (col << 16)` adds `k` first -- both are the same four
     instructions in the wrong order.
   * the eight vertex halfwords go in the LISTING's order, grouped by value
     (0x14, 0x10, then the four zeros, then 0x16, 0xE). In x0,y0,x1,y1 order
     both constants are live at once, so they take two registers instead of
     one and the whole tail shifts.
   * `m = 1; if (p->unk_06 == 0) { m = 2; }` -- an init and an override, not an
     if/else: the listing sets $a1 to 1 before the `lh` and only the 2 is in a
     branch. And the 0xFFFFFF arm is written FIRST, since the `bnez` into the
     divide makes it the fall-through.
   * the two OT links are `addPrim` from libgpu.h, i.e. P_TAG bitfield stores.
     A hand-written `|` cannot match -- see the landed func_800453E0 in
     csrc/gprel_18B40.c for the expand_binop reason. */
s32 func_80053604(Unk80083120 *p)
{
    Unk80083160 *s;
    u8 *q;
    u8 *r;
    s32 c = 0;
    s32 m;
    u32 col;

    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = q + 0xC;
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u8 **)(D_80083160.unk_00 + 0x8D0) = r + 0x18;
    s = &D_80083160;
    m = 1;
    if (p->unk_06 == 0)
    {
        m = 2;
    }
    func_80067F20((u32 *)q, 0, 0, GetTPage(0, m, 0, 0), 0);
    if (p->unk_02 > p->unk_04)
    {
        c = 1;
    }
    if (p->unk_02 >= p->unk_04)
    {
        col = 0xFFFFFF;
    }
    else
    {
        col = (p->unk_02 << 8) / p->unk_04;
        col = col + ((col << 8) + (col << 16));
    }
    if (c == 0)
    {
        p->unk_02 = p->unk_02 + 1;
    }
    *(u32 *)(r + 4) = col;
    SetPolyF4((POLY_F4 *)r);
    SetSemiTrans(r, 1);
    *(s16 *)(r + 0x14) = 0x140;
    *(s16 *)(r + 0x10) = 0x140;
    *(s16 *)(r + 0xC) = 0;
    *(s16 *)(r + 8) = 0;
    *(s16 *)(r + 0x12) = 0;
    *(s16 *)(r + 0xA) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    *(s16 *)(r + 0xE) = 0xE0;
    addPrim(s->unk_00 + 0x70, r);
    addPrim(s->unk_00 + 0x70, q);
    return c;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800537D0);

u8 *func_8004E21C(u8 *, u8);

/* func_80053A88's sibling, one level further in: for every node in the -0x8
   chain it walks the NUL-terminated byte string at +0x04, turns each byte into
   a sprite through func_8004E21C and links it, advancing a running x by each
   glyph's width, then links one tpage primitive per node.

   `r = p` is a SECOND pointer for the body. The listing copies it in the
   guard branch's delay slot (`addu $s1, $s5, $zero`) and reads every field
   through it, keeping `p` for the string guard, the running x and the -0x8
   link. One variable gives one register and the function is three words
   short, because the extra callee-saved register is what the frame is sized
   for.

   `x` is an `s32` holding `*(s16 *) (p + 0x14)`, not an `s16` local: as an
   s16 the load comes out `lhu` and the running sum is done narrow.

   `f` is a SEPARATE variable for the tpage primitive, not `e` reused. With
   `e` reused the final `addPrim`'s mask lands in $v1; with its own variable
   the listing's in-place `and $s0, $s0, $s3` comes back, because the
   primitive pointer dies into its own mask. 5 words. */
s32 func_80053858(u8 *p)
{
    Unk80083160 *s = &D_80083160;
    u8 buf[4];
    u8 *e;
    u8 *f;
    u8 *q;
    u8 *r;
    s32 i;
    s32 x;

    while (1)
    {
        i = 0;
        x = *(s16 *) (p + 0x14);
        r = p;
        if ((*(u8 **) (p + 4))[0] != 0)
        {
            do
            {
                func_8004E21C(buf, (*(u8 **) (r + 4))[i]);
                e = *(u8 **) (s->unk_00 + 0x8D0);
                *(u8 **) (s->unk_00 + 0x8D0) = e + 0x14;
                *(s16 *) (e + 8) = x;
                *(s16 *) (e + 0xA) = *(u16 *) (r + 0x16);
                *(s16 *) (e + 0xE) = *(u16 *) (r + 0x1A);
                *(u32 *) (e + 4) = *(u32 *) (r + 0x10);
                e[0xC] = buf[0];
                e[0xD] = buf[1];
                *(s16 *) (e + 0x10) = buf[2];
                *(s16 *) (e + 0x12) = buf[3];
                x = x + buf[2];
                SetSprt((SPRT *) e);
                if ((*(u16 *) (r + 0x1C) & 1) != 0)
                {
                    SetSemiTrans(e, 1);
                }
                if ((*(u16 *) (r + 0x1C) & 2) == 0)
                {
                    addPrim(s->unk_00 + 0x70, e);
                }
                i++;
            } while ((*(u8 **) (r + 4))[i] != 0);
        }
        f = *(u8 **) (s->unk_00 + 0x8D0);
        *(u8 **) (s->unk_00 + 0x8D0) = f + 0xC;
        func_80067F20((u32 *) f, 0, 0, GetTPage(0, *(s16 *) (r + 0x18), 0x3C0, 0), 0);
        addPrim(s->unk_00 + 0x70, f);
        q = *(u8 **) (p - 8);
        if (q == 0)
        {
            break;
        }
        p = q + 0x20;
    }
    return 0;
}

/* Walks a linked list of tile records, emitting a TILE and a tpage primitive
   for each and linking both into the OT at +0x74. The loop is a `while (1)`
   with an explicit `break` on the null next-pointer, NOT a `do`/`while (q !=
   0)`: the two differ only in which register `q` gets -- $a0 for the break
   form, $v0 for the tested-condition form -- and that is the last three
   words. */
s32 func_80053A88(u8 *p)
{
    Unk80083160 *s = &D_80083160;
    u8 *e;
    u8 *q;

    while (1)
    {
        e = *(u8 **)(s->unk_00 + 0x8D0);
        *(u8 **)(s->unk_00 + 0x8D0) = e + 0x10;
        *(s16 *)(e + 8) = *(u16 *)(p + 0xC);
        *(s16 *)(e + 0xA) = *(u16 *)(p + 0xE);
        *(s16 *)(e + 0xC) = *(u16 *)(p + 0x10);
        *(s16 *)(e + 0xE) = *(u16 *)(p + 0x12);
        *(u32 *)(e + 4) = *(u32 *)(p + 8);
        SetTile((TILE *)e);
        if ((*(u16 *)(p + 0x16) & 1) != 0)
        {
            SetSemiTrans(e, 1);
        }
        if ((*(u16 *)(p + 0x16) & 2) == 0)
        {
            addPrim(s->unk_00 + 0x74, e);
        }
        e = *(u8 **)(s->unk_00 + 0x8D0);
        *(u8 **)(s->unk_00 + 0x8D0) = e + 0xC;
        func_80067F20((u32 *)e, 0, 0, GetTPage(0, *(s16 *)(p + 0x14), 0, 0), 0);
        if ((*(u16 *)(p + 0x16) & 2) == 0)
        {
            addPrim(s->unk_00 + 0x74, e);
        }
        q = *(u8 **)(p - 8);
        if (q == 0)
        {
            break;
        }
        p = q + 0x20;
    }
    return 0;
}

void func_80053C3C(u8 *p) {
    func_8003E4FC(6, p, 0);
    func_8003F320();
}

void func_80053C6C(void) {
    u32 *b = (u32 *) D_8006ADBC;
    u32 v = b[1];

    *(u32 *) D_8006ADBC = v;
    if (v != 0) {
        func_80053C3C((u8 *) v);
    }
}

void func_80053C6C(void);

void func_80053CA4(void) {
    D_8006ADC0[0] = (u32) D_800812C8;
    func_80053C6C();
}

void func_80053C6C(void);

void func_80053CD0(void) {
    D_8006ADC0[0] = (u32) D_800812D0;
    func_80053C6C();
}

/* $gp group (D_80081568 and D_8008156C added to the unit's sdata_entries
   block; nothing else in the unit touches them). The pair is filled with an
   8-BYTE STRUCT COPY, which is what makes the following `D_80081568` read a
   real `lw` again: as two word stores cse keeps the value it just stored and
   the function is a word short. Verified in a scratch file carrying the block,
   since verify.py compiles standalone. */
void func_80053CFC(u32 *p, u32 b) {
    *(Unk80081568 *) &D_80081568 = *(Unk80081568 *) p;
    D_80081568 = (((D_80081568 + 0x7FF) >> 11) << 23) | (b & 0x7FFFFF);
    func_8003E4FC(6, (u8 *) &D_80081568, b);
    func_8003F320();
}

s32 func_80053D64(void) {
    u8 loc[8];

    if (CdSync(1, 0) == 2) {
        CdControl(0x10, 0, loc);
    }
    return CdPosToInt((CdlLOC *)loc);
}

s32 func_80055778(s32);

s16 func_80053DA8(u16 a) {
    return func_80055778(a);
}

void func_80054D64(s32);

void func_80053DCC(s32 a) {
    D_8008480C[0] = a;
    func_80054D64(a);
}

void func_800552C8(s32);

void func_80053DF0(s32 a) {
    D_8008480A[0] = a;
    func_800552C8(a);
}

void func_80053E14(s16 v) {
    D_80084808[0] = v;
}

void func_800552C8(s32);
void func_80054D64(s32);

/* The case bodies are emitted in SOURCE order, which the compare tree (built
   from the sorted values) does not show: case 4 first, then 1, then 2, which
   is why the `bne` for 4 falls straight into its own body. Written 2, 1, 4 the
   three blocks come out in the other order -- 18 DIFF with every instruction
   right. Both callees take the first parameter, which arrives in $a0 and is
   never moved. */
void func_80053E20(s32 a, s32 k) {
    switch (k) {
    case 4:
        D_8008480C[0] = a;
        func_80054D64(a);
        break;
    case 1:
        D_8008480A[0] = a;
        func_800552C8(a);
        break;
    case 2:
        D_80084808[0] = a;
        break;
    }
}

/* func_80053E20's getter, same case order. */
s16 func_80053E90(s32 k) {
    switch (k) {
    case 4:
        return D_8008480C[0];
    case 1:
        return D_8008480A[0];
    case 2:
        return D_80084808[0];
    }
    return 0;
}

extern u32 D_800848FC[];
extern u32 D_8008485C[];

/* A `switch`, not an if/else chain: both tests branch INTO their arm and the
   default is the fall-through `j`. And each masked flag goes into its OWN
   local before the `!= 0` -- written `return (f & 0x100) != 0;` combine turns
   the pair into `srl 8` / `andi 1` where the target has `andi 0x100` / `sltu`
   (4 DIFF; `> 0` and `!!` do the same). */
s32 func_80053EF0(s32 k) {
    u32 f;
    u32 g;

    switch (k) {
    case 1:
        f = D_800847D0[0];
        if ((f & 0x1000) != 0) {
            return 2;
        }
        if (D_800848FC[0] == 2) {
            return 3;
        }
        g = f & 0x100;
        return g != 0;
    case 4: {
        u8 *b = (u8 *) D_800847D0;

        if ((*(u32 *) (b + 4) & 0x200) != 0) {
            return 0x100;
        }
        f = D_800847D0[0];
        if ((f & 0x4000) != 0) {
            return 2;
        }
        if (D_8008485C[0] == 2) {
            return 3;
        }
        g = f & 0x400;
        return g != 0;
    }
    }
    return -1;
}

extern u8 D_80084548[];
void func_8005A2FC(void);
void func_8005A39C(u32);
s32 func_8005A3F0(u32);
/* func_8005A420 is an empty `(void)` function in this unit but is called here
   with three arguments; an __asm__ alias gives this one call site the real
   signature without touching that definition. */
void func_8005A420c(u8 *, s32, s32) __asm__("func_8005A420");
void func_8005ACD0(u16);
s16 func_8005B378(s16);
void func_8005B320(void);
void func_8005B3D8(s32, s32);
void func_8005A5C8(s16, s16);
void func_800559B4(void);
void func_80055864(void);
void func_800557BC(void);
void func_8005A37C(void);

void func_80053F98(void) {
    func_8005A2FC();
    func_8005A39C(1);
    func_8005A3F0(0x18);
    func_8005A420c(D_80084548, 1, 1);
    func_8005ACD0(0);
    func_8005B378(4);
    func_8005E7E0(4);
    func_8005B320();
    func_8005B3D8(0x3C, 0x3C);
    func_8005A5C8(0x7F, 0x7F);
    func_8005A4E8(0, 0, 0);
    func_8005A56C(0, 0, 0);
    func_80055B44(2);
    func_800559B4();
    func_80055864();
    func_800557BC();
    func_8005A37C();
}

/* Nested ifs with ONE return 0 at the end, and the 1 returned from the inner
   arm. Every shape that assigns a variable in the arms puts the result in a
   callee-chosen register with a `move $v0` at the end: a variable live to the
   single return is a global allocno, local-alloc has already given the second
   mask chain $v0, and the result is pushed to $a1. Returning directly makes
   hard reg $v0 live from each arm, so local-alloc routes the second chain
   around it ($v1/$a0) exactly as the target does. 13 DIFF.

   `r = 1` before the second test and `r = 0` in the arm is also what jump.c
   store-flags into one `sltu`, which three earlier spellings all hit. */
s16 func_8005405C(s16 n) {
    if ((D_800847D0[1] & (0x1000000 << n)) == 0) {
        if ((D_800847D0[0] & (0x10000 << n)) != 0) {
            return 1;
        }
    }
    return 0;
}

void func_800540A8(void) {
    u8 *p;

    func_8005A4E8(0, 0, 0);
    func_80055990(D_80084858);
    /* Assigned AFTER the calls: as an initialiser it lives across them in
       $s0, which costs the save, the restore and the frame slot (3 words). */
    p = (u8 *) D_800847D0;
    *(u32 *) (p + 0x8) = 0;
    *(u32 *) (p + 0xC) = 0;
    p[0x30] = 0;
    p[0x32] = 0;
    *(u32 *) (p + 0x10) = 0;
    *(u32 *) (p + 0x14) = 0;
    p[0x31] = 0;
    p[0x33] = 0;
    *(u32 *) (p + 0x18) = 0;
}

extern u8 D_80032E0C[];

/* UNPARKED. Copies the 8 bytes at D_80032E0C into a local `s16 t[4]` and then,
   for each of 0x30 outer steps and the first three entries of t, calls
   func_80055730 when func_8005405C accepts the entry.

   The inner loop is written with `goto`, which is what keeps it out of
   loop.c's hands: the C front end emits NOTE_INSN_LOOP_BEG only for a loop
   STATEMENT, so a goto back edge gets no strength reduction and no invariant
   motion. As a `for`, loop.c reduced `t[j]` to a walking cursor and hoisted
   `$sp + 0x10` into a register of its own -- two callee-saved registers and
   four words the target does not spend, where the listing recomputes
   `sll $v0, $s1, 1` / `addu $v0, $sp, $v0` / `lh 0x10($v0)` every iteration.
   Five subscript spellings had been measured against that and none of them
   reached it; the loop's own form is the lever.

   `j = 0` is the FIRST statement of the outer body, ahead of `hi` and `base`.
   Loop rotation leaves one copy in the preheader and one in the outer back
   edge's delay slot, which is where the listing has them; written after the
   two expressions, the delay slot takes `andi $v0, $s2, 0xFF` instead.

   `((u32) i & 0xFF) >> 4` unsigned, or the shift is `sra` where the listing
   has `srl`. */
void func_80054104(void)
{
    s16 t[4];
    s32 i;
    s32 j;

    memcpy(t, D_80032E0C, 8);
    for (i = 0; i < 0x30; i++)
    {
        s32 hi;
        s32 base;

        j = 0;
        hi = ((u32) i & 0xFF) >> 4;
        base = ((i & 0xF) << 10) + 0x1C00;
    inner:
        {
            s16 v = t[j];
            s32 k = ((v << 8) & 0xFF00) | hi;

            if (v == 0)
            {
                k += 0x10;
            }
            if (func_8005405C(v))
            {
                func_80055730(k, base);
            }
        }
        j++;
        if (j < 3)
        {
            goto inner;
        }
    }
}

void func_8005E7E0(s32);

void func_800541E8(void) {
    func_8005B300();
    func_8005B418(0);
    func_8005B348();
    func_8005E7E0(4);
    func_8005B320();
}

void func_800553D4(s32);
void func_80055C50(s16);
void func_8005B418(s32);
void func_8005A428(void);
void func_8005A4B8(void);

void func_80054228(void) {
    s16 i;

    if ((D_800847D0[0] & 0x100) != 0) {
        func_800553D4(0x71);
    }
    func_8005B418(0);
    for (i = 0; i < 4; i++) {
        func_80055C50(i);
    }
    func_8005A428();
    func_8005A4B8();
}

s32 func_800542A4(void) {
    return 0;
}

void func_800542AC(void) {
    D_800847F6[0] = -1;
}

extern u32 D_80084850[];

u16 func_800557C8(void);
void func_80054788(u16);
void func_80054B08(u16);
void func_8005500C(u16);
void func_8005552C(u16);
void func_800543C8(void);

/* The dispatch is a real `switch`, so gcc builds the balanced comparison tree
   the listing has (0x500 at the root, then 0x200, then 0x300/0 and
   0x700/0x600/0x800) rather than a chain. The four handler bodies are laid out
   in SOURCE order -- 0, 0x300, 0x200, then the group -- which is the order the
   labels come in.

   `func_8005552C`'s delay slot re-computes `v & 0xFFFF` where the three
   single-case arms inherit the mask from the loop guard: its block has four
   predecessors, so cse's extended basic block ends at that label and the
   value is not available there. Nothing to spell differently, but it is why
   one arm looks a word more expensive than its siblings. */
void func_800542BC(void)
{
    u16 v;

    D_80084850[0]++;
    for (;;)
    {
        v = func_800557C8();
        if (v == 0)
        {
            break;
        }
        switch (v & 0xF00)
        {
        case 0x000:
            func_80054788(v);
            break;
        case 0x300:
            func_80054B08(v);
            break;
        case 0x200:
            func_8005500C(v);
            break;
        case 0x500:
        case 0x600:
        case 0x700:
        case 0x800:
            func_8005552C(v);
            break;
        }
    }
    func_800543C8();
    func_800557BC();
}

void func_80055CF4(void);
void func_800544A4(void);
void func_8005440C(void);

void func_800543C8(void) {
    if ((D_800847D4 & 0xFF000000) != 0) {
        func_80055CF4();
    }
    func_800544A4();
    func_8005440C();
}

void func_800550E8(void);
void func_80054538(u8 *);
s32 func_800545F4(u8 *);
void func_800553D4(s32);
s32 func_80059D60(void);

void func_8005440C(void) {
    u8 *b = (u8 *) D_800847D0;

    if ((*(u32 *) (b + 4) & 2) != 0) {
        func_800550E8();
    }
    if ((D_800847D0[0] & 0x100) != 0) {
        if (func_80059D60() == 3 ||
            func_800545F4((u8 *) D_800848F8) == -1) {
            func_800553D4(0x71);
        }
    }
    func_80054538((u8 *) D_800848F8);
}

void func_80054538(u8 *);
void func_800546B0(void);

/* +0x04 through a `u8 *` base and +0x00 as `D_800847D0[0]`: that is the pair
   of forms the target has, one `lui` shared between the materialised base and
   the symbolic load. */
void func_800544A4(void) {
    u8 *b = (u8 *) D_800847D0;
    u32 f;
    u8 *c;

    if (*(u32 *) (b + 4) & 0x200) {
        func_80054704();
    }
    f = D_800847D0[0];
    if (f & 0x400) {
        if (f & 0x4000) {
            c = D_80084858;
            func_80054538(c);
            if (*(u32 *) (c + 0xC) == 3) {
                func_80054E00(0x74);
            }
        } else {
            func_800546B0();
        }
    }
}

void func_80054538(u8 *p) {
    switch (*(s32 *) (p + 0xC)) {
    case 1:
        *(u16 *) (p + 0x10) = *(u16 *) (p + 0x10) + *(u16 *) (p + 0x14);
        if (*(s16 *) (p + 0x10) >= 0x80) {
            *(u16 *) (p + 0x10) = 0x7F;
            *(s32 *) (p + 0xC) = 3;
        }
        break;
    case 2:
        *(u16 *) (p + 0x10) = *(u16 *) (p + 0x10) - *(u16 *) (p + 0x14);
        if (*(s16 *) (p + 0x10) < *(s16 *) (p + 0x12)) {
            *(u16 *) (p + 0x10) = *(u16 *) (p + 0x12);
            *(s32 *) (p + 0xC) = 3;
        }
        break;
    default:
        return;
    }
    if (*(u32 *) p != 0) {
        (*(void (**)(void)) p)();
    }
}

s32 func_800545F4(u8 *p) {
    switch (*(s32 *) (p + 4)) {
    case 1:
        *(u16 *) (p + 8) = *(u16 *) (p + 8) + *(u16 *) (p + 0x16);
        if (*(s16 *) (p + 0xA) < *(s16 *) (p + 8)) {
            *(u16 *) (p + 8) = *(u16 *) (p + 0xA);
            *(s32 *) (p + 4) = 3;
        }
        break;
    case 2:
        *(u16 *) (p + 8) = *(u16 *) (p + 8) - *(u16 *) (p + 0x18);
        if (*(s16 *) (p + 8) <= 0) {
            *(u16 *) (p + 8) = 0;
            return -1;
        }
        break;
    default:
        return 0;
    }
    if (*(u32 *) p != 0) {
        (*(void (**)(void)) p)();
    }
    return 0;
}

s32 func_800545F4(u8 *);
void func_80054E00(s32);

void func_800546B0(void) {
    if ((u32) func_80053D64() >= D_800847DC ||
        func_800545F4(D_80084858) == -1) {
        func_80054E00(0x74);
    }
}

s32 func_8003F5AC(void);
void func_80054E00(s32);
/* func_80054D64 is called here with NO argument -- $a0 still holds the ~0x200
   mask from the `&=` above and nothing sets it. An `__asm__` alias gives this
   one call site a (void) declaration while the rest of the unit keeps the
   `void(s32)` one; passing `~0x200` explicitly costs a word, because cse does
   not delete the second `li` of a constant into $a0. */
void func_80054D64v(void) __asm__("func_80054D64");
void func_8005A4E8(u8, u8, u8);

void func_80054704(void) {
    if ((func_8003F5AC() & 4) != 0) {
        D_800847D0[1] &= ~0x200;
        if (D_800847D0[4] != 0) {
            func_80054E00(0x74);
        } else if ((D_800847D0[0] & 0x4000) == 0) {
            D_800847D0[0] |= 0x400;
            func_80054D64v();
            func_8005A4E8(0, 0, 1);
        }
    }
}

/* The 0xF0-group dispatch. Three things decide it:

   * the case BODIES are laid out in the listing's order -- 0x10, 0x20, 0x70,
     0xE0, 0xF0, then the shared 0xB0/0xC0/0xD0 body, then the default -- which
     is not numeric order; gcc emits bodies in source order and builds the
     comparison tree separately (root 0xB0, the median of the eight labels).
   * the two D_800847D0 tests in the shared body are NESTED IFS, not one
     `&&` chain. `fold_truthop` merges two comparisons against one operand
     into a single masked compare, so `(x & 0x100) != 0 && (x & 0x1000) == 0`
     comes out as `andi 0x1100` against 0x100 -- two words per group, four in
     all. Nested `if`s are separate statements, which fold cannot reach, and
     cse still shares the one `lw`.
   * func_8005497C and func_800549FC are called with NO PROTOTYPE in scope --
     both are defined below this function and nothing declares them above it
     -- so the `u16` argument takes the default promotion to `int` and the
     listing's `andi $a0, 0xFFFF` is that zero-extension. Their own
     definitions take `u8`, which would mask with 0xFF instead. */
void func_800553D4(s32);
void func_80054104(void);
void func_80054E00(s32);

void func_80054788(u16 n)
{
    switch (n & 0xF0)
    {
    case 0x10:
        func_8005497C(n);
        break;
    case 0x20:
        func_800549FC(n);
        break;
    case 0x70:
        if ((n & 1) != 0)
        {
            func_800553D4(0x71);
        }
        if ((n & 2) != 0)
        {
            func_80054104();
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0x74);
        }
        break;
    case 0xE0:
        if ((n & 1) != 0)
        {
            func_800553D4(0xE1);
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0xE4);
        }
        break;
    case 0xF0:
        if ((n & 1) != 0)
        {
            func_800553D4(0xF1);
        }
        if ((n & 4) != 0)
        {
            func_80054E00(0xF4);
        }
        break;
    case 0xB0:
    case 0xC0:
    case 0xD0:
        if ((n & 1) != 0)
        {
            if ((D_800847D0[0] & 0x100) != 0)
            {
                if ((D_800847D0[0] & 0x1000) == 0)
                {
                    func_80054F9C(n & 0xFFF1, D_800848F8);
                }
            }
        }
        if ((n & 4) != 0)
        {
            if ((D_800847D0[0] & 0x400) != 0)
            {
                if ((D_800847D0[0] & 0x4000) == 0)
                {
                    func_80054F9C(n & 0xFFF4, D_80084858);
                }
            }
        }
        break;
    default:
        func_80054A7C(n & 0xFF);
        break;
    }
}

void func_800558FC(u16);
void func_80055BD8(s16);

/* Four cases that all call the same function with different constants;
   cross-jumping merges the four calls into the one the listing shows and reorg
   puts two of the four argument loads into delay slots. */
void func_8005497C(u8 k) {
    func_800558FC(k);
    switch (k) {
    case 0x11:
        func_80055BD8(0);
        break;
    case 0x12:
        func_80055BD8(1);
        break;
    case 0x14:
        func_80055BD8(2);
        break;
    case 0x18:
        func_80055BD8(3);
        break;
    }
}

void func_800553D4(s32);
void func_80055ADC(u8 *, u32);

/* A switch, not an if/else chain: two `beq`s that branch TO their bodies and a
   `j` past them, where the chain tests and falls into each body in turn (23
   DIFF). The shared `lui %hi(D_800847F0)` in the second test's delay slot is
   the other half of that reading. */
void func_800549FC(u8 k) {
    func_800553D4(0x71);
    switch (k) {
    case 0x21:
        D_800847F0[0] = 0;
        func_80055ADC(D_801BEE40, 0);
        break;
    case 0x22:
        D_800847F0[0] = 1;
        func_80055ADC(D_801C4640, 1);
        break;
    }
}

void func_80055B44(u16);
void func_800540A8(void);
void func_800541E8(void);

/* Cases 1 and 2 share a body and there is no default: {1,2} is a RANGE node,
   and after `!= 3`, `< 4` and `!= 0` nothing is left to test, so
   node_is_bounded holds for it and emit_case_nodes branches on the LEFT with
   `blt` instead of the usual `bgt` on the right -- which is the branch sense
   the listing shows. Written as a default, the test comes out the other way
   round and the trailing `j` goes to the default body instead of the
   epilogue. */
void func_80054A7C(u16 k) {
    switch (k & 0xFF) {
    case 1:
    case 2:
        func_80055B44(k);
        break;
    case 3:
        func_800540A8();
        break;
    case 4:
        func_800541E8();
        break;
    }
}

s32 func_80054AF0(s32 a) {
    if (a == 1) {
        return 0x5F;
    }
    return 0x7F;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054B08);

void func_80054C58(void) {
    u32 *p = D_800847D0;
    u8 *q = D_80084858;
    u32 a = p[4];
    u32 b = p[5];
    u8 c = *((u8 *) p + 0x31);
    u8 d = *((u8 *) p + 0x33);

    p[4] = 0;
    p[5] = 0;
    *((u8 *) p + 0x31) = 0;
    *((u8 *) p + 0x33) = 0;
    *(u32 *) (q + 0xC) = 3;
    *(s16 *) (q + 0x10) = 0x80;
    p[2] = a;
    p[3] = b;
    *((u8 *) p + 0x30) = c;
    *((u8 *) p + 0x32) = d;
    D_800847D0[0] &= ~0x400;
    D_800847D0[0] &= ~0x4000;
    p[1] &= ~0x200;
}

void func_8005A4E8(u8, u8, u8);
void func_8005A56C(s16, s16, s16);

/* D_80084904 through the incomplete-array alias, so the compiler materialises
   its `%hi` into a register instead of letting the assembler spend $at -- and
   the store goes BEFORE the base assignment, which is what gets that `lui`
   emitted first. As `D_80084904 = 2;` on a scalar: `lui $at` + `sw 0($at)`,
   10 DIFF; with the base in its declaration, 9. */
void func_80054CD4(void) {
    u8 *b;

    D_80084904a[0] = 2;
    b = (u8 *) D_800847D0;
    b[0x2C] = b[0x32];
    b[0x2D] = b[0x30];
    *(u32 *) (b + 4) |= 0x200;
    func_8005A4E8(0, 0, 0);
    func_8005A56C(0, 0, 0);
    func_8003E4FC(0xE, 0, (s32) (b + 0x28));
    func_8003E4FC(0xD, *(u8 **) (b + 8), (s32) (b + 0x2C));
}

void func_8005A56C(s16, s16, s16);

/* `/ 32767`, not the 10922.56 that 2^(32+14)/0x180010003 estimates: the magic
   and the shift are chosen together, so invert the magic with
   tools/decomp/magic.py instead of dividing. The base is declared INSIDE the
   if so its lui/addiu land there rather than ahead of the guard. */
void func_80054D64(s32 a) {
    if ((D_800847D0[0] & 0x400) != 0) {
        u8 *b = D_80084858;
        s16 t = *(s16 *) (b + 8) * D_8008480C[0] / 32767;
        s16 v = t * *(s16 *) (b + 0x10) / 128;

        func_8005A56C(0, v, v);
    }
}

/* Each case materialises D_800847D0's base for ITSELF -- a `u8 *` local inside
   the case block, not one at function scope. At function scope the base is
   live across the 0x74 arm's two calls and takes a callee-saved register,
   where the listing creates it in $a2 after them.

   D_80084858 takes a pointer local too. Written as `*(u32 *)(D_80084858 + 4)`
   the first store goes out symbolically and gcc then materialises
   `D_80084858 + 4` as the base for the two halfword stores, so their offsets
   come out 4 and 6 where the listing has 8 and 0xA off the unbiased base --
   the same bias func_80054B08 is parked on. */
extern u32 D_80084864[];

void func_8005A4E8(u8, u8, u8);
s32 func_80054AF0(s32);
s32 func_80053D64(void);
void func_80054C58(void);
void func_80054CD4(void);

void func_80054E00(s32 n)
{
    switch (n & 0xFF)
    {
    case 0x74:
    {
        u8 *b;
        u8 *s;
        s32 v;

        func_8005A4E8(0, 0, 0);
        func_8003E4FC(9, NULL, 0);
        b = (u8 *)D_800847D0;
        D_800847D0[0] &= ~0x400;
        if (*(u32 *)(b + 0x18) == 0)
        {
            s = D_80084858;
            D_80084904a[0] = 1;
            *(u32 *)(s + 4) = 0;
            if (*(u32 *)(b + 0x10) != 0)
            {
                v = func_80054AF0(*(s16 *)(b + 0x1C));
                *(s16 *)(s + 8) = v;
                *(s16 *)(s + 0xA) = v;
                func_80054C58();
                func_80054CD4();
            }
        }
        break;
    }
    case 0xE4:
    {
        u8 *b = (u8 *)D_800847D0;

        if ((*(u32 *)(b + 4) & 0x200) != 0)
        {
            func_8003E4FC(9, NULL, 0);
        }
        else if ((D_800847D0[0] & 0x400) != 0)
        {
            u32 v = func_80053D64();

            *(u32 *)(b + 0x18) = v;
            if (*(u32 *)(b + 8) >= v)
            {
                *(u32 *)(b + 0x18) = *(u32 *)(b + 8);
            }
            D_80084864[0] = 2;
            *(u32 *)(b + 0x10) = *(u32 *)(b + 0x18);
            *(u32 *)(b + 0x14) = *(u32 *)(b + 0xC);
            b[0x31] = b[0x30];
            b[0x33] = b[0x32];
            D_800847D0[0] |= 0x4000;
        }
        break;
    }
    case 0xF4:
        if ((D_800847D0[0] & 0x4000) != 0)
        {
            *(u32 *)((u8 *)D_800847D0 + 0x18) = 0;
            func_80054C58();
            func_80054CD4();
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80054F9C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005500C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800550E8);

/* The magic 0x80010003 with post_shift 14 is a DIVISION BY 32767 (magic.py;
   do not estimate it from the arithmetic). D_800848F8 is read through a
   pointer local so the two constant indices come out as displacements off a
   materialised base. func_8005B27C is reached through an __asm__ alias with
   `(s16, s16, s16)`: the target passes the s16 result twice unmasked, where
   the unit's own `(s16, u8, u8)` definition would make the caller `andi`. */
s32 func_80055750(s16);
void func_8005B27Cw(s16, s16, s16) __asm__("func_8005B27C");

void func_800552C8(s32 unused) {
    u8 *b = (u8 *) D_800847D0;
    s16 *q;
    s32 t;
    s16 e;
    s32 f;
    s16 v;

    if ((D_800847D0[0] & 0x100) != 0) {
        q = D_800848F8;
        t = q[4] * D_8008480A[0];
        e = t / 32767;
        f = e * q[8];
        v = func_80055750(f / 128);
        func_8005B27Cw(*(s16 *) (b + 0x22), v, v);
    }
}

void func_8005537C(s16 a) {
    s16 v;

    D_800848F8[5] = a;
    if ((D_800847D0[0] & 0x20) != 0) {
        D_800848F8[5] = a - 0x18;
    }
    v = func_80055750(D_800848F8[5]);
    D_800848F8[5] = v;
    D_800848F8[4] = v;
}

extern u32 D_800848FC[];
extern s16 D_80084900[];

void func_800552C8(s32);
void func_8005AF74(s16);
void func_8005AFF4(s16);
void func_8005B070(s16);
void func_8005B16C(s16);

/* This call site passes NO argument. The listing has no argument move before
   `jal func_800552C8` -- $a0 still holds the masked event code from the
   dispatch, and the call's delay slot is spent on the D_80084900 store -- and
   every spelling that passes the event code costs a word: cse follows the
   switch's own `beq` into the arm (the label has one reference), records
   `n == 0x71` there, and attaches it as a REG_EQUAL note that combine
   rematerialises as `addiu $a0, $zero, 0x71`. Passing the literal 0x71 emits
   the same word, so there is no source that passes a value and matches. The
   `__asm__` alias says "prototype-less at this site" without touching the
   declaration the two other call sites in this unit need; it works in the
   original because $a0 happens to hold exactly what the callee reads. */
extern void func_800552C8a(void) __asm__("func_800552C8");

/* `D_800847D0[0] &= ~0x100;` and `&= ~0x1000;` as TWO statements. As one
   expression fold merges the masks into a single `addiu $v1, $zero, -0x1101`
   and the function is a word short; as two, gcc's flow pass drops the first
   store as dead (nothing reads the location in between) and what is left is
   the listing's one `lw`, two `and`s and one `sw`. */
void func_800553D4(s32 n)
{
    u8 *b;

    n &= 0xFF;
    switch (n)
    {
    case 0x71:
        b = (u8 *)D_800847D0;
        if ((D_800847D0[0] & 0x1100) != 0)
        {
            D_80084900[0] = 0;
            func_800552C8a();
            func_8005AF74(*(s16 *)(b + 0x22));
            func_8005AFF4(*(s16 *)(b + 0x22));
        }
        D_800848FC[0] = 0;
        D_800847D0[0] &= ~0x100;
        D_800847D0[0] &= ~0x1000;
        *(u32 *)(b + 4) &= ~2;
        if (*(s16 *)(b + 0x26) != -1)
        {
            func_800550E8();
        }
        break;
    case 0xE1:
        if ((D_800847D0[0] & 0x100) != 0)
        {
            func_8005B070(*(s16 *)((u8 *)D_800847D0 + 0x22));
            D_800847D0[0] = (D_800847D0[0] & ~0x100) | 0x1000;
        }
        break;
    case 0xF1:
        if ((D_800847D0[0] & 0x1000) != 0)
        {
            func_8005B16C(*(s16 *)((u8 *)D_800847D0 + 0x22));
            D_800847D0[0] = (D_800847D0[0] & ~0x1000) | 0x100;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005552C);

void func_800555FC(void) {
}

void func_80055604(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005560C);

void func_8005BAB0(s32, s32);

/* Both arguments pass straight through, so this compiles to the same bytes
   either way; the widths are the ones func_8005BAB0's own entry and
   func_80054104's call site prove. */
void func_80055730(s32 a, s32 b) {
    func_8005BAB0(a, b);
}

s32 func_80055750(s16 a) {
    if (a < 0) {
        return 0;
    }
    if (a >= 0x80) {
        return 0x7F;
    }
    return a;
}

/* Push onto the D_80084778 ring. The guard is an EARLY RETURN and the success
   path returns its own 0: one variable assigned in both arms gave the target's
   instructions with $v1 and a copy, because a variable live through the join is
   a global allocno. With a return in each arm the whole tail is block-local,
   the 0 goes straight into $v0, and jump2's cross-jumping merges the two `jr`s
   back into the one the listing shows. 7 DIFF.

   The count is re-read after `++` because the store invalidated cse's entry for
   it -- matching-levers section 4 -- so the element written is [count + 1]. */
s32 func_80055778(s32 v) {
    if (D_80084778[0] >= 0x20) {
        return -1;
    }
    D_80084778[0]++;
    D_80084778[D_80084778[0]] = v;
    return 0;
}

void func_800557BC(void) {
    D_80084778[0] = 0;
}

/* Pops the head off the halfword queue at D_80084778 (slot 0 is the count).
   `r = 0;` is a STATEMENT after the clamp, not the declaration's initialiser:
   as an initialiser the zero is materialised at the top of the function and
   the guard's delay slot gets a nop; an early `return 0;` instead costs a `j`
   and a second zero (40 words either way). */
u16 func_800557C8(void) {
    u16 r;
    u16 i;

    if (D_80084778[0] > 0x20) {
        D_80084778[0] = 0x20;
    }
    r = 0;
    if (D_80084778[0] != 0) {
        r = D_80084778[1];
        for (i = 1; i < D_80084778[0]; i++) {
            D_80084778[i] = D_80084778[i + 1];
        }
        D_80084778[0] = D_80084778[0] - 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055864);

s32 func_8003F5D4(void);

/* The four targets are incomplete arrays, so the `lui` lands in a register the
   `beq`'s delay slot can take; as scalars each store spends `lui $at` instead
   and the function is five words longer. */
void func_800558FC(u16 k) {
    s32 v = func_8003F5D4();

    switch (k) {
    case 0x11:
        D_80084758[0] = v;
        break;
    case 0x12:
        D_8008475C[0] = v;
        break;
    case 0x14:
        D_80084760[0] = v;
        break;
    case 0x18:
        D_80084764[0] = v;
        break;
    }
}

void func_80055990(u8 *p) {
    *(u16 *) (p + 0x10) = 0x80;
    *(u32 *) (p + 0xC) = 3;
    *(u32 *) (p + 0x4) = 0;
    *(u16 *) (p + 0x16) = 0xA;
    *(u16 *) (p + 0x18) = 0xA;
}

extern u32 D_80084850[];

void func_80055990(u8 *);
void func_800552C8(s32);
void func_80054D64(s32);

void func_800559B4(void)
{
    u8 *p = (u8 *)D_800847D0;
    u8 *q = (u8 *)D_800848F8;
    u8 *r = D_80084858;
    u8 *c;

    D_800847D0[0] = 0;
    *(u32 *)(p + 0x4) = 0;
    D_800847D0[0] = 0x10;
    *(s16 *)(p + 0x1E) = 0x5A;
    *(s16 *)(p + 0x26) = -1;
    *(u32 *)(p + 0x8) = 0;
    *(u32 *)(p + 0xC) = 0;
    p[0x30] = 0;
    p[0x32] = 0;
    *(u32 *)(p + 0x10) = 0;
    *(u32 *)(p + 0x14) = 0;
    p[0x31] = 0;
    p[0x33] = 0;
    *(u32 *)(p + 0x18) = 0;
    *(s16 *)(p + 0x22) = 0;
    *(s16 *)(p + 0x20) = 0;
    D_80084808[0] = 0x7FFF;
    c = (u8 *)D_80084808;
    *(s16 *)(c + 0x2) = 0x7FFF;
    *(s16 *)(c + 0x4) = 0x7FFF;
    D_80084850[0] = 0;
    func_80055990(q);
    func_80055990(r);
    *(u32 *)D_800848F8 = (u32)func_800552C8;
    *(s16 *)(q + 0x8) = 0x64;
    *(s16 *)(q + 0xA) = 0x64;
    *(s16 *)(q + 0x12) = 0x50;
    *(s16 *)(q + 0x14) = 2;
    *(u32 *)D_80084858 = (u32)func_80054D64;
    *(s16 *)(r + 0x8) = 0x7F;
    *(s16 *)(r + 0xA) = 0x7F;
    *(s16 *)(r + 0x14) = 0x14;
    *(s16 *)(r + 0x12) = 0;
    p[0x28] = 0xC8;
}

/* Three separate statements -- the mask, the base, then the index -- which is
   the order the five address insns come out in (`andi`, `lui`, `addiu`, `sll`,
   `addu`). Any spelling that leaves the index inside the address expression
   emits the shift before the symbol, because fold puts the ADDR_EXPR second:
   3 DIFF through a base local, a sized alias, a struct array and an explicit
   index local. The declarations' order is the rest of it: the offset, the
   count and the counter are initialised before the address. */
void func_80055ADC(u8 *p, u32 b) {
    s32 off = 0x14;
    s32 n = *(s32 *) (p + 0x10);
    s32 i = 0;
    s32 k = b & 0xFFFF;
    u8 *base = D_80084878;
    u32 *q = (u32 *) (base + k * 0x40);
    u32 *d = q;

    for (; i < 0x10; i++) {
        if (i >= n) {
            *d = *q;
        } else {
            u8 *e = p + off;

            *d = (u32) (e + 0x10);
            off += 0x10 + *(s32 *) (e + 4);
        }
        d++;
    }
}

void func_8005A3E0(void);
void func_8005A3D4(void);

/* The CdMix levels are written 2, 0, 3, 1 in both arms, which is the order the
   target emits them in (0, 1, 2, 3 is 8 DIFF at the same word count). */
void func_80055B44(u16 kk) {
    u8 k = kk;
    CdlATV t;

    if (k == 1) {
        func_8005A3E0();
        t.val2 = 0x4F;
        t.val0 = 0x4F;
        t.val3 = 0x4F;
        t.val1 = 0x4F;
        D_800847D0[0] = (D_800847D0[0] & ~0x10) | 0x20;
    } else {
        func_8005A3D4();
        t.val2 = 0x7F;
        t.val0 = 0x7F;
        t.val3 = 0;
        t.val1 = 0;
        D_800847D0[0] = (D_800847D0[0] & ~0x20) | 0x10;
    }
    CdMix(&t);
}

void func_800553D4(s32);
void func_80055C50(s16);

void func_80055BD8(s16 k) {
    if (k == 0 && (D_800847D0[0] & 0x100) != 0) {
        func_800553D4(0x71);
    }
    func_80055C50(k);
    D_800847D0[1] |= 0x1000000 << k;
}

void func_8005AC68(s16);

void func_80055C50(s16 k) {
    u8 *b = (u8 *) D_800847D0;

    if ((D_800847D0[0] & (0x10000 << k)) != 0) {
        if (k == 0 && (D_800847D0[0] & 0x100) != 0) {
            func_800553D4(0x71);
            *(s16 *) (b + 0x26) = -1;
        }
        func_8005AC68(k);
        D_800847D0[0] &= ~(0x10000 << k);
    }
}

void func_80055CEC(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055CF4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80055D84);

void func_80055E6C(void) {
}

void func_80055E74(void) {
}

void func_80055E7C(s32 a, s32 b, s32 c) {
}

/* func_80056098's body one channel over -- every offset is +0x1C from that
   function's, the final divide is by 256 rather than 1024, and the whole thing
   is wrapped in an `else` so the early `p[0x42]++` path falls into the tail
   instead of returning. All four of func_80056098's spellings carry over
   unchanged: `t` declared `s8`, the wave test on the undoubled value, the
   fall-through arm of each if/else written first, and the explicit `v != 0`.

   The tail's own two facts: D_80084918 takes TWO pointer locals, one for the
   `+4` store before the clamps and one for the `+0xA`/`+0xC`/`+0xE` stores
   after them, because cse restarts at the clamps' joins and the listing
   materialises the base twice (one local would live across them in a
   callee-saved register, and this function saves only $ra); and `+8` is
   reached through its OWN symbol D_80084920 as an incomplete array, which is
   the compiler-split `lui`/`sh %lo` the listing has there, while `+0xA` and
   up go through the second local. */
extern s16 D_80084920[];
void func_8005F134(void *);

void func_80055E84(u8 *p)
{
    s32 g;
    s32 n;
    s8 t;
    s32 v;
    s32 u;
    u8 *b;
    u8 *c;
    s32 x;

    g = *(u32 *)(p + 0x4C);
    if (g == 0)
    {
        return;
    }
    if (p[0x42] != p[0x50])
    {
        p[0x42] = p[0x42] + 1;
    }
    else
    {
        if (p[0x44] >= p[0x51])
        {
            *(u32 *)(p + 0x48) = g;
        }
        else
        {
            if (p[0x44] != 0)
            {
                *(u32 *)(p + 0x48) = *(u32 *)(p + 0x48) + *(u32 *)(p + 0x54);
            }
            else
            {
                *(u32 *)(p + 0x48) = *(u32 *)(p + 0x54);
            }
            p[0x44] = p[0x44] + 1;
        }
        p[0x43] = p[0x43] + p[0x52];
        p[0x41] = 0;
        n = *(s8 *)(p + 0x43);
        if (n < 0)
        {
            n = -n;
            u = n * 2;
            t = (n << 25) < 0 ? -u : u;
            v = *(s32 *)(p + 0x48) * t;
            if (v > 0)
            {
                v = -v;
            }
        }
        else
        {
            u = n * 2;
            t = (n << 25) < 0 ? -u : u;
            v = *(s32 *)(p + 0x48) * t;
            if (v < 0)
            {
                v = -v;
            }
        }
        if (v != 0)
        {
            *(u32 *)(p + 0x58) = v / 256;
        }
        else
        {
            *(u32 *)(p + 0x58) = 0;
        }
    }
    if (*(s16 *)(p + 0x6C) != *(s32 *)(p + 0x58))
    {
        *(s16 *)(p + 0x6C) = *(u16 *)(p + 0x58);
        b = D_80084918;
        *(u32 *)(b + 4) = 0xF;
        *(u32 *)D_80084918 = D_80073740[*(u16 *)p];
        x = *(u16 *)(p + 0x10) + *(u32 *)(p + 0x58);
        if (x < 0)
        {
            x = 0;
        }
        else if (x >= 0x4000)
        {
            x = 0x3FFF;
        }
        D_80084920[0] = x;
        x = *(u16 *)(p + 0x12) + *(u32 *)(p + 0x58);
        if (x < 0)
        {
            x = 0;
        }
        else if (x >= 0x4000)
        {
            x = 0x3FFF;
        }
        c = D_80084918;
        *(s16 *)(c + 0xA) = x;
        *(s16 *)(c + 0xC) = 0;
        *(s16 *)(c + 0xE) = 0;
        func_8005F134(c);
    }
}

/* The +0x3C ramp: advance the +0x26 delay, then the +0x28 step, then fold the
   +0x27 phase through a triangle wave and scale by +0x2C. Four spellings:

   * `t` is declared `s8`, not `s32` with an `(s8)` cast at the multiply.
     As an `s32`, `t` and `u` are one quantity and the negation happens in
     place; the listing has `addu $v1, $a1, $zero` in the `bgez` delay slot
     and `negu $v1, $a1` after it, which is two DISTINCT quantities. The
     narrow declaration is what keeps them apart.
   * the wave's test is written on `n`, not on `u`. `u = n * 2;` and then
     `(u & 0x80) != 0` leaves the `andi`, because combine cannot fold the mask
     back through a pseudo with several uses; `(n << 25) < 0` is the fold it
     would have done, and two words.
   * both if/else pairs have the arm the listing FALLS THROUGH written first:
     `p[0x28] >= p[0x35]` (the `sltu` is the taken branch) and the inner
     `p[0x28] != 0`.
   * the final store keeps its explicit `v != 0` test; `v / 1024` alone is the
     three-word `bgez`/`addiu 0x3FF`/`sra` and no zero branch. */
void func_80056098(u8 *p)
{
    s32 g;
    s32 n;
    s8 t;
    s32 v;
    s32 u;

    g = *(u32 *)(p + 0x30);
    if (g == 0)
    {
        return;
    }
    if (p[0x26] != p[0x34])
    {
        p[0x26] = p[0x26] + 1;
        return;
    }
    if (p[0x28] >= p[0x35])
    {
        *(u32 *)(p + 0x2C) = g;
    }
    else
    {
        if (p[0x28] != 0)
        {
            *(u32 *)(p + 0x2C) = *(u32 *)(p + 0x2C) + *(u32 *)(p + 0x38);
        }
        else
        {
            *(u32 *)(p + 0x2C) = *(u32 *)(p + 0x38);
        }
        p[0x28] = p[0x28] + 1;
    }
    p[0x27] = p[0x27] + p[0x36];
    p[0x25] = 0;
    n = *(s8 *)(p + 0x27);
    if (n < 0)
    {
        n = -n;
        u = n * 2;
        t = (n << 25) < 0 ? -u : u;
        v = *(s32 *)(p + 0x2C) * t;
        if (v > 0)
        {
            v = -v;
        }
    }
    else
    {
        u = n * 2;
        t = (n << 25) < 0 ? -u : u;
        v = *(s32 *)(p + 0x2C) * t;
        if (v < 0)
        {
            v = -v;
        }
    }
    if (v != 0)
    {
        *(u32 *)(p + 0x3C) = v / 1024;
    }
    else
    {
        *(u32 *)(p + 0x3C) = 0;
    }
}

extern u16 D_80073738[];
/* An incomplete-array alias: the listing's reads of the index are gcc's own
   `(high)`/`(lo_sum)` pair with the high in a SEPARATE register, which the
   committed 2-byte scalar cannot give -- that form is one insn and maspsx
   splits it using the destination as the temp. */
extern s16 D_80086D50a[] __asm__("D_80086D50");

/* Derives an entity's two scale factors and its +0x18 brightness from the
   active D_80086A40/D_80086C00 pair: the brightness is the slot's +0x1B plus
   the entity's +0x16/+0x17 plus the caller's +0x04, biased by -0xC0 and
   clamped to 0..0x7F (or pinned at 0x40 while D_8007382A is set); the scale is
   a five-term fixed-point product split between the two axes by whether the
   brightness is past 0x40, then scaled by the +0x08/+0x0A pair and the
   entity's own +0x1C, and squared once more when the caller's +0x2C is 0x40 or
   more.

   The two table bases go through LOCALS (`b`, `g`) so that each symbol is
   materialised before the index that uses it -- func_8004A658's idiom -- which
   here also fills the `lh`'s load-delay slot with the second base's `addiu`
   instead of a nop.

   ONE accumulator for the five-term product, not an intermediate `m` and then
   `s`. With two variables the chain's `mflo`s land in $t1, the final `sra`
   reads a register the following `slti` does not write, and reorg is then free
   to pull that `sra` into the branch's delay slot -- which the listing does
   not do, because there the `sra` reads the very register the `slti` clobbers.

   The two axis products are assigned BACK to `x` and `y` rather than into
   fresh locals. The listing reuses x's and y's registers for them, which is
   what gcc does when the old value dies in the same insn that computes the
   new one; fresh locals land in $v1 twice over and serialise the two chains,
   where the listing interleaves them (9 wrong words).

   The `>= 0x40` arm is written FIRST so the other one goes out of line, and
   the `t = sum - 0xC0` assignment comes before the D_8007382A test so it ends
   up in that branch's delay slot. */
void func_800561D8(u8 *p, Unk80084960 *q)
{
    Unk80086A40 *b;
    Unk80086A40 *e;
    Unk80086C00 *g;
    Unk80086C00 *f;
    s32 t;
    s32 s;
    s32 x;
    s32 y;

    b = D_80086A40;
    e = &b[D_80086D50a[0]];
    t = e->unk_1B + p[0x16] + p[0x17] + q->unk_04 - 0xC0;
    if (D_8007382A[0] != 0)
    {
        t = 0x40;
    }
    if (t < 0)
    {
        t = 0;
    }
    if (t >= 0x80)
    {
        t = 0x7F;
    }
    *(s16 *) (p + 0x18) = t;
    s = e->unk_18 * D_80073738[0] * q->unk_14 * q->unk_0C;
    s = (u32) s >> 14;
    s = (s * p[0x14] * p[0x15]) >> 14;
    if (t >= 0x40)
    {
        y = s;
        x = ((0x40 - (t & 0x3F)) * (y << 1)) >> 7;
    }
    else
    {
        x = s;
        y = (t * (x << 1)) >> 7;
    }
    g = D_80086C00;
    f = &g[D_80086D50a[0]];
    x = (x * *(s16 *) &f->unk_08) >> 7;
    y = (y * *(s16 *) &f->unk_0A) >> 7;
    *(s16 *) (p + 0x10) = (x * (p[0x1C] & 0x7F)) >> 7;
    *(s16 *) (p + 0x12) = (y * (p[0x1C] & 0x7F)) >> 7;
    if (q->unk_2C >= 0x40)
    {
        *(s16 *) (p + 0x10) = (*(u16 *) (p + 0x10) * *(u16 *) (p + 0x10)) >> 14;
        *(s16 *) (p + 0x12) = (*(u16 *) (p + 0x12) * *(u16 *) (p + 0x12)) >> 14;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800563B0);

void func_800563B0(s32, s32, s32);

/* UNPARKED. The entry is indexed off the symbol INSIDE the loop body; as a
   walked `e += 0x78` loop.c adds a second giv biased to `e + 0x12` and uses
   negative displacements, which is what this was parked on. The D_80073734
   bound is read signed (`blez`/`slt`) and re-read by the loop condition, which
   is what keeps it out of a callee-saved register. */
void func_80056408(void) {
    s32 i;

    for (i = 0; i < (s32) D_80073734[0]; i++) {
        u8 *e = D_80085458 + i * 0x78;

        func_800561D8(e, &D_80084960[*(u16 *) (e + 6)]);
        func_800563B0(i, *(u16 *) (e + 0x10), *(u16 *) (e + 0x12));
    }
}


/* The entry pointer is INDEXED off the symbol inside the body, not walked:
   `u8 *e = D_80085458; ... e += 0x78;` makes loop.c add a SECOND giv biased to
   `e + 0x12` and use negative displacements, which is func_80056408's parked
   problem (48 DIFF). And the D_80073734 bound is read SIGNED -- the listing
   has `blez` and `slt`, where the u32 declaration gives `beqz` and `sltu`.
   The parameter is unused; the caller's prototype already says it is there. */
void func_800564A8(s32 unused) {
    s32 i;

    for (i = 0; i < (s32) D_80073734[0]; i++) {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *) (e + 6) < 0x10) {
            func_800561D8(e, &D_80084960[*(u16 *) (e + 6)]);
            func_800563B0(i, *(u16 *) (e + 0x10), *(u16 *) (e + 0x12));
        }
    }
}

extern s32 D_80073734s __asm__("D_80073734");
void func_800563B0(s32, s32, s32);

/* The bound through the SCALAR spelling, so its load is one macro insn that
   maspsx splits afterwards -- as the incomplete array the compiler splits it
   itself and loop.c hoists the `lui` half into a callee-saved register, two
   words longer. Signed, so the guard is `blez` and not `beqz`.
   The entry pointer is computed INSIDE the body, which is what puts the giv's
   initialisation in the preheader unbiased; `p += 0x78` on a pointer declared
   outside biases it to +0x12 (the first field used) instead.
   Both loads are left inside the comparison: in locals, the `lw` is emitted
   first and the fill pass cannot put it in the `lh`'s delay slot (2 DIFF). */
void func_80056558(void) {
    s32 i = 0;

    while (i < D_80073734s) {
        u8 *p = D_80085458 + i * 0x78;

        if (*(s16 *) (p + 0x6C) != *(s32 *) (p + 0x58)) {
            func_800563B0(i, *(u16 *) (p + 0x10) + *(s32 *) (p + 0x58), *(u16 *) (p + 0x12) + *(s32 *) (p + 0x58));
        }
        i++;
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800565D8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056654);

/* No local copy of the parameter: `u8 *p = q;` spends a word the target
   leaves as a `nop`. And the negative arm's test is written with the
   DIFFERENCE first (`a - b > -limit`), which is the order the target evaluates
   its three `lh`s in; `-limit < a - b` negates first, 12 DIFF. */
void func_80056800(u8 *p) {
    if (*(u32 *) (p + 8) != 0) {
        if (*(u32 *) (p + 0x34) != 0) {
            if (*(s16 *) (p + 0x3A) + *(s16 *) (p + 0x38) <
                *(s16 *) (p + 0x40)) {
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x3A) +
                                      *(u16 *) (p + 0x38);
            } else {
                *(u32 *) (p + 0x34) = 0;
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x40);
            }
        } else {
            if (*(s16 *) (p + 0x3A) - *(s16 *) (p + 0x38) >
                -*(s16 *) (p + 0x40)) {
                *(u16 *) (p + 0x3A) = *(u16 *) (p + 0x3A) -
                                      *(u16 *) (p + 0x38);
            } else {
                *(u32 *) (p + 0x34) = 1;
                *(u16 *) (p + 0x3A) = -*(u16 *) (p + 0x40);
            }
        }
    } else {
        *(s16 *) (p + 0x3A) = 0;
    }
}

/* Steps +0x52 towards +0x56 by +0x54, clamping; +0x58 picks the direction.
   Each arm ends in its own `return` -- that is what gives all four of them
   their own `j` to the epilogue (two words short with the else arms falling
   through) -- and the second test is written `x - y > -c`, not `-c < x - y`,
   so the subtraction is emitted before the negation. */
void func_800568B4(u8 *p) {
    if (*(s16 *) (p + 0x50) == 0) {
        return;
    }
    if (*(u32 *) (p + 0x58) != 0) {
        if (*(s16 *) (p + 0x52) + *(s16 *) (p + 0x54) < *(u16 *) (p + 0x56)) {
            *(s16 *) (p + 0x52) = *(s16 *) (p + 0x52) + *(s16 *) (p + 0x54);
            return;
        }
        *(s16 *) (p + 0x52) = *(u16 *) (p + 0x56);
        return;
    }
    if (*(s16 *) (p + 0x52) - *(s16 *) (p + 0x54) > -*(u16 *) (p + 0x56)) {
        *(s16 *) (p + 0x52) = *(s16 *) (p + 0x52) - *(s16 *) (p + 0x54);
        return;
    }
    *(s16 *) (p + 0x52) = -*(u16 *) (p + 0x56);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056948);

/* The three-table reset: clears all 0x20 D_80085FA8 entries, then every live
   D_80085458 entry whose +0x06 index is in range (keying its voice off and
   collecting the handles into one mask), then all 0x10 D_80084960 entries,
   and finally keys the collected mask off in one call.

   Right at 138 words on the first compile; three shapes, two of them this
   unit's standing recipe:

   * Each entry pointer is INDEXED off the symbol inside the loop body
     (`e = D_80085FA8 + i * 0x50`), which is what leaves loop.c one unbiased
     giv per loop -- the listing's single `addiu $v1, $v1, 0x50` /
     `addiu $s1, $s1, 0x78` / `addiu $v1, $v1, 0x9C`. Walking a pointer
     declared outside biases the giv to the first field used.

   * D_80084960 is reached through `(u8 *) &D_80084960[i]` -- the struct for
     the stride (see the note on Unk80084960 in structs.h: a byte array lets
     fold migrate the field offset onto the symbol) and a `u8 *` for the
     fields, because this loop writes 28 of them and all but four are still
     inside pads.

   * `m = 0` comes BEFORE the D_80073738 store, which is what puts $s5's save
     and its zeroing ahead of the `lui`; the natural order (the store first)
     is 4 words out, and `i = 0` has to stay last so cse spells it
     `addu $s2, $s5, $zero`. */
void func_80056A08(void)
{
    s32 i;
    u32 m;
    u32 h;

    m = 0;
    D_80073738[0] = 0x7F;
    i = 0;
    do
    {
        u8 *e = D_80085FA8 + i * 0x50;

        *(u32 *) (e + 0x00) = 0;
        *(u32 *) (e + 0x04) = 0;
        *(u32 *) (e + 0x08) = 0;
        *(u32 *) (e + 0x0C) = 0;
        *(u32 *) (e + 0x10) = 0;
        *(u32 *) (e + 0x18) = 0;
        *(u32 *) (e + 0x1C) = 0;
        *(u32 *) (e + 0x20) = 0;
        *(u32 *) (e + 0x24) = 0;
        *(u32 *) (e + 0x28) = 0;
        *(u32 *) (e + 0x2C) = 1;
        *(u32 *) (e + 0x34) = 0;
        *(u32 *) (e + 0x38) = 0;
        *(u32 *) (e + 0x3C) = 0x78;
        *(u32 *) (e + 0x40) = 0;
        e[0x48] = 0;
        e[0x49] = 0xFF;
        e[0x4A] = 0;
        e[0x4C] = 0;
        i++;
    } while (i < 0x20);

    for (i = 0; i < (s32) D_80073734[0]; i++)
    {
        u8 *p = D_80085458 + i * 0x78;

        if (*(u16 *) (p + 6) < 0x10)
        {
            *(s16 *) (p + 0x00) = i;
            *(u16 *) (p + 0x1A) = 0;
            *(u16 *) (p + 0x18) = 0x40;
            p[0x1D] = 0;
            p[0x27] = 0;
            p[0x26] = 0;
            p[0x28] = 0;
            *(u32 *) (p + 0x2C) = 0;
            *(u32 *) (p + 0x30) = 0;
            *(u32 *) (p + 0x3C) = 0;
            p[0x43] = 0;
            p[0x42] = 0;
            p[0x44] = 0;
            *(u32 *) (p + 0x48) = 0;
            *(u32 *) (p + 0x4C) = 0;
            *(u32 *) (p + 0x58) = 0;
            *(u32 *) (p + 0x70) = 0x40;
            *(u32 *) (p + 0x74) = 0x40;
            h = D_80073740[i];
            func_80056DB4(i);
            m |= h;
            func_8005E97C(0, h);
        }
    }

    for (i = 0; i < 0x10; i++)
    {
        u8 *q = (u8 *) &D_80084960[i];

        *(u32 *) (q + 0x0C) = 0x7F;
        *(u32 *) (q + 0x04) = 0x40;
        *(u32 *) (q + 0x18) = 0;
        *(u32 *) (q + 0x1C) = 0x40;
        *(u32 *) (q + 0x14) = 0x7F;
        *(u32 *) (q + 0x20) = 0x7F;
        *(u32 *) (q + 0x24) = 0x7F;
        *(u32 *) (q + 0x08) = 0;
        *(u16 *) (q + 0x3A) = 0;
        *(u32 *) (q + 0x44) = 0;
        *(u16 *) (q + 0x40) = 0;
        *(u32 *) (q + 0x48) = 0;
        *(u32 *) (q + 0x94) = 0;
        q[0x30] = 0;
        q[0x60] = 0;
        q[0x5F] = 0;
        q[0x61] = 0;
        *(u32 *) (q + 0x64) = 0;
        *(u32 *) (q + 0x68) = 0;
        q[0x7B] = 0;
        q[0x7A] = 0;
        q[0x7C] = 0;
        *(u32 *) (q + 0x80) = 0;
        *(u32 *) (q + 0x84) = 0;
        *(u32 *) (q + 0x28) = 0;
        *(u32 *) (q + 0x2C) = 0;
        *(u16 *) (q + 0x50) = 0;
        q[0x98] = 0;
    }
    func_8005E97C(0, m);
}

/* The D_80085458 reset. Four loop facts, three of them the recipe this unit's
   other sweeps use:

   * the counter is `s32`, not `s16`. As `s16` gcc sign-extends it at every use
     and keeps an extra copy for the increment: six words and two more
     callee-saved registers.
   * the bound takes the `__asm__` SCALAR alias, so its load is one macro insn
     that maspsx splits inside the loop. Through the incomplete array loop.c
     hoists the `lui` half into a callee-saved register -- the opposite of what
     func_8005B070 needs for the same symbol, which is why the rule is to read
     the `lui`'s destination per site.
   * the entry pointer is INDEXED inside the body. Walked from a pointer
     declared outside, loop.c keeps the walked pointer AND adds a second giv
     biased to +0x74.

   * `i++` goes AFTER func_8005E97C, not between the two calls. The listing
     has `addu $a0, $zero, $zero` -- func_8005E97C's first argument --
     immediately after func_80056DB4's delay slot, with the three giv updates
     on the far side of it. An argument setup cannot be scheduled earlier than
     its place in the chain, but the giv updates CAN be hoisted across it and
     across the call itself, because they write callee-saved registers the
     call does not clobber. So put the increment after the call and let sched
     pull the updates forward; with it between the calls the four insns come
     out in the other order (4 DIFF at the exact word count). */
extern u8 D_80085014[];
extern s32 D_80073734s __asm__("D_80073734");

void func_80056A08(void);
void func_80056DB4(s32);
void func_8005E97C(s32, s32);

void func_80056C30(void)
{
    u8 *b;
    s32 i;
    u32 m;
    u32 v;

    func_80056A08();
    i = 0;
    m = 0;
    if (D_80073734s > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            *(s16 *)e = i;
            *(s16 *)(e + 0x1A) = 0;
            *(s16 *)(e + 0x18) = 0x40;
            *(u32 *)(e + 0x70) = 0x40;
            *(u32 *)(e + 0x74) = 0x40;
            v = D_80073740[i];
            func_80056DB4(i);
            func_8005E97C(0, v);
            i++;
            m |= v;
        } while (i < D_80073734s);
    }
    b = D_80085014;
    *(u32 *)(b + 0xC) = 0x7F;
    *(u32 *)(b + 4) = 0x40;
    *(u32 *)(b + 0x18) = 0;
    *(u32 *)(b + 0x1C) = 0x40;
    *(u32 *)(b + 0x14) = 0x7F;
    *(u32 *)(b + 0x20) = 0x7F;
    *(u32 *)(b + 0x24) = 0x7F;
    *(u32 *)(b + 8) = 0;
    *(u32 *)(b + 0x94) = 0;
    *(s16 *)(b + 0x50) = 0;
    func_8005E97C(0, m);
}

void func_80056D3C(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056D44);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056DB4);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80056E10);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057948);

void func_80057A48(void) {
}

extern s16 D_80086D50a[] __asm__("D_80086D50");

/* The two address materialisations interleave -- `lui`, `lui`, `addiu`, `lh`
   -- which they can only do if BOTH are compiler-split pairs. As the scalar
   `D_80086D50`, cc1psx emits `lh $2,D_80086D50` as one macro insn and maspsx
   expands it into an adjacent lui/lh pair that nothing can schedule apart, so
   the pair lands after the other symbol's `addiu`: 2 DIFF that six statement
   orderings could not move. The array alias gives the split form.

   The 0x820 binds to the index term and not to the sum, so the index is its
   own local: written as one expression the constant folds onto the pointer
   instead and the final `addu`/`addiu` come out in the other order. */
u8 *func_80057A50(s32 *p, u16 j) {
    u8 *q = D_80086A40[D_80086D50a[0]].unk_04;
    s32 k = (((*p << 4) + j) << 5) + 0x820;

    return q + k;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057A94);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80057D20);

void func_8005845C(u8 i, u8 v) {
    D_80084960[i].unk_00 = v;
}

void func_8005848C(void) {
}

void func_80058494(u8 i, s32 b, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
    func_80055E7C(1, i, v);
}

s32 func_80059BC4(void);

void func_800584E4(void) {
    if (D_800737A4[0] != 0) {
        func_80059BC4();
    }
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058510);

void func_80058510(void);

/* Arms the root counter 2 event with func_80058510 as the handler. */
void func_800585A0(void) {
    if (D_800737C0[0] == 0) {
        D_800737C0[0] = 1;
        EnterCriticalSection();
        D_80085F9C[0] = OpenEvent(0xF2000002, 2, 0x1000, (void *) func_80058510);
        EnableEvent(D_80085F9C[0]);
        SetRCnt(0xF2000002, 0x1C00, 0x1000);
        StartRCnt(0xF2000002);
        ExitCriticalSection();
        D_80085F98[0] = 0;
        D_800737C0[0] = 0;
    }
}

void func_8006A27C(u32);

void func_8005863C(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    DisableEvent(D_80085F9C[0]);
    CloseEvent(D_80085F9C[0]);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

void func_8006A27C(u32);

void func_800586B0(void) {
    D_800737C0[0] = 1;
    EnterCriticalSection();
    func_8006A27C(0xF2000002);
    ExitCriticalSection();
    D_80085F98[0] = 0;
    D_800737C0[0] = 0;
}

extern u32 D_800869A8[];
void func_800584E4(void);
void func_80056948(void);
void func_8005ACDC(void);

void func_80058700(void) {
    if (D_80085F98[0] == 0) {
        D_80085F98[0] = 1;
        if (D_800737A4[0] != 0) {
            if (D_800869A8[0] == 0x1E0) {
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
                func_800584E4();
            }
            func_800584E4();
            func_800584E4();
            func_800584E4();
            func_800584E4();
        }
        func_80056948();
        func_8005ACDC();
        D_80085F98[0] = 0;
    }
}

void func_800599B0(void);

void func_800587B4(void) {
    D_80085FA4[0] = 0x80080000;
    D_800869B0[0] = 0xC000;
    func_80056C30();
    func_800599B0();
    D_800737A4[0] = 1;
}

void func_800587F8(void) {
}

s32 func_80058800(u8 *a, u8 *b, s32 n) {
    if (n == 0) {
        return -1;
    }
    while (--n != 0) {
        if (*a != *b) {
            return *a - *b;
        }
        a++;
        b++;
    }
    return *a - *b;
}

s32 func_80058850(u32 i) {
    do {
        if (func_80058800(D_800330FC, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_800588C8(u32 i) {
    do {
        if (func_80058800(D_80033104, (u8 *) (D_80085FA4[0] + i), 4) == 0) {
            return i + 4;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

s32 func_80058940(u32 i) {
    do {
        if (func_80058800(D_800737D0, (u8 *) (D_80085FA4[0] + i), 3) == 0) {
            return i + 3;
        }
        i++;
    } while (D_800869B0[0] >= i);
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800589B8);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058A04);

u32 func_80058A7C(u32 a, u32 b, u32 c, u32 d) {
    return ((((((a & 0xFF) << 8) + (b & 0xFF)) << 8) + (c & 0xFF)) << 8) | (d & 0xFF);
}

u32 func_80058AA8(u32 a, u32 b) {
    return ((a & 0xFF) << 8) | (b & 0xFF);
}

u32 func_800589B8(s32);
u32 func_80058A7C(u32, u32, u32, u32);

/* Four u8 locals, so each result is masked again at the call. The first call
   needs no argument move -- $a0 still holds the parameter. */
s32 func_80058ABC(u8 *p) {
    u8 a = func_800589B8((s32) p);
    u8 b = func_800589B8((s32) p);
    u8 c = func_800589B8((s32) p);
    u8 d = func_800589B8((s32) p);

    return func_80058A7C(a, b, c, d);
}

u32 func_800589B8(s32);

u16 func_80058B2C(s32 a) {
    u8 x = func_800589B8(a);
    u8 y = func_800589B8(a);

    return func_80058AA8(x, y);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058B70);

void func_80058E50(s32 *p, s32 v) {
    *p += v;
}

void func_80058E64(void) {
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80058E6C);

u32 func_80058A04(s32);
u32 func_800589B8(s32);

/* `func_80058A04(p)` emits nothing -- $a0 already holds p and cse deletes the
   no-op move -- but it is the FOURTH reference to p, which crosses the
   floor_log2 boundary in global-alloc's priority (2*4/11 against 1*3/9) and
   wins p the $s1 the target has. With three references n outranks it and the
   two swap. The increment is written FIRST in the body, which is what puts a
   copy of it in the preheader and the other in the bottom branch's delay
   slot. */
void func_800590E0(s32 p) {
    u32 i = 0;
    u32 n = func_80058A04(p);

    do {
        i++;
        if ((u8) func_800589B8(p) == 0xF7) {
            break;
        }
    } while (i < n);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005914C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005947C);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800595C0);

s32 func_800588C8(u32);
s32 func_80058ABC(u8 *);

/* No local for the second result: the target stores it straight out of $v0 and
   only then copies it to $a1, because the `return 0` needs $v0. Reading
   `*(u32 *) (p + 0x18)` back for the sum is free -- cse disambiguates it from
   the +0x38 store -- while `*(u32 *) p` is re-read for real, the call having
   invalidated it. */
s32 func_800597A8(u8 *p) {
    s32 v = func_800588C8(*(u32 *) p);

    *(u32 *) p = v;
    if (v == -1) {
        return 1;
    }
    *(u32 *) (p + 0x18) = func_80058ABC(p);
    *(u32 *) (p + 0x38) = *(u32 *) p;
    *(u32 *) (p + 0x1C) = *(u32 *) p + *(u32 *) (p + 0x18);
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059814);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_800599B0);

extern u32 D_800737C4[];
extern u32 D_800737C8[];
/* An incomplete-array alias: this function's reads of the count have their
   `lui` scheduled apart from the `lw` -- one lands in a branch delay slot --
   which the scalar `extern u32 D_800869B4` cannot do, since that form is a
   single insn until maspsx splits it. */
extern u32 D_800869B4n[] __asm__("D_800869B4");

void func_8005947C(Unk80085FA8 *);
u8 func_800595C0(Unk80085FA8 *);
void func_80059814(Unk80085FA8 *);

/* Per-entry tick: advances each live D_80085FA8 entry's +0x3C accumulator by
   its +0x24 step and, on each wrap past 0x100, either counts +0x10 down or --
   when it is already zero -- runs the command pump until something sets it.

   The inner pump is written with `goto`, and that is the whole point: the C
   front end only emits NOTE_INSN_LOOP_BEG for a real loop statement, so
   loop.c never sees a goto loop and never hoists its invariants. Written as
   `for (;;)` with `continue`, loop-invariant motion lifts
   `(high D_800737C8)` into the preheader, which puts the loop's LABEL after
   the `lui` instead of before it -- and then reorg cannot move the
   fall-through's `lui %hi(D_800737C4)` into the first back edge's delay slot,
   because the opposite thread now starts with the `lw` that READS that
   register. It copies the loop's own `lui` in instead, and the function is a
   word long. With `goto` the label precedes the `lui`, the opposite thread
   starts with an insn that only WRITES the register, and the move goes
   through.

   D_800737C8 has to be the incomplete-array form for the same reason: a
   4-byte scalar is small data at -G8, one insn until maspsx splits it, so its
   `lui` could not be stolen into the second back edge's slot at all.

   `e = &D_80085FA8s[i]` indexed, not a walked `e++`: the pointer is passed to
   every call, so a source-level increment would be a biv that cannot be
   eliminated AND loop.c would reduce the field addresses into a second
   register. Indexed, the giv is the one `addiu $s0, $s0, 0x50` the listing
   has. */
s32 func_80059BC4(void)
{
    Unk80085FA8 *e;
    u32 i;
    u32 v;

    i = 0;
    if (D_800869B4n[0] != 0)
    {
        do
        {
            e = &D_80085FA8s[i];
            if (e->unk_2C == 0)
            {
                v = e->unk_3C + *(u16 *) &e->unk_24;
                e->unk_3C = v;
                if (v >= 0x100)
                {
                    e->unk_3C = v & 0xFF;
                    if (e->unk_10 == 0)
                    {
                    top:
                        if (D_800737C8[0] != 2)
                        {
                            func_8005947C(e);
                            if (e->unk_2C != 0)
                            {
                                goto out;
                            }
                            e->unk_10 = func_80058A04((s32) e);
                        }
                        else if ((u8) func_800595C0(e) == 0)
                        {
                            if (e->unk_2C != 0)
                            {
                                goto out;
                            }
                            e->unk_10 = func_80058A04((s32) e);
                        }
                        else if (e->unk_2C != 0)
                        {
                            goto out;
                        }
                        if (e->unk_10 == 0)
                        {
                            goto top;
                        }
                        if (D_800737C4[0] != 0)
                        {
                            func_80059814(e);
                        }
                        if (e->unk_10 == 0)
                        {
                            goto top;
                        }
                    out:
                        if (e->unk_10 != 0)
                        {
                            e->unk_10 = e->unk_10 - 1;
                        }
                    }
                    else
                    {
                        e->unk_10 = e->unk_10 - 1;
                    }
                }
            }
            i++;
        } while (i < D_800869B4n[0]);
    }
    return 0;
}

void func_80059D1C(void) {
    u32 i = 0;

    if (D_800869B4 != 0) {
        do {
            D_80085FA8s[i].unk_2C = 1;
            D_80085FA8s[i].unk_00 = 0;
            i++;
        } while (i < D_800869B4);
    }
}

s32 func_80059D60(void) {
    u32 i;

    for (i = 0; i < D_800869B4; i++) {
        if (D_80085FA8s[i].unk_2C == 0) {
            return 1;
        }
    }
    return 3;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059DAC);

void func_80059DAC(void);

/* Claims a D_800869C0 slot -- a {offset, length} pair per slot, sixteen of
   them -- for a run of `size` bytes and returns its offset, or -1 when the
   run would reach past 0x80000 - D_80073830[D_8007382B[0]]. The slot table
   empty is its own arm, allocating at 0x1010; otherwise the first slot with a
   zero length is taken. Running off the end of the table is not an error: the
   0x1010 that `base` still holds is what comes back.

   `p` IS SET ONCE and the rows are reached as `p[i * 2]`, not by walking
   `p += 2`. Both halves of the listing's preheader want that:

     * strength reduction turns the two row references into ONE giv -- the
       `addiu $a0, $a0, 0x8` at the bottom -- where `p += 2` plus `p[0]`/`p[1]`
       leaves the user's own pointer and `D_800869C0[i * 2]` spelt out gives
       two givs that combine_givs does not merge;
     * a pointer set once is a pseudo with `reg_n_sets == 1`, so sched1's
       birthing-insn rule raises `p = D_800869C0` above the hoisted `lui
       $a3, 0x8` and emits the constant FIRST. With `p += 2` the pointer is
       set twice, the two tie on priority, RTL order wins, and the constant
       comes out one slot late -- which also costs the registers, because
       `$a3` is then free to carry both values in turn ($a3 for the table
       address, reused for 0x80000) instead of the listing's $t0 and $a3. */
s32 func_80059E94(u32 size)
{
    u32 base;
    u32 *p;
    s32 i;

    base = 0x1010;
    if (D_800869C0[0] == 0)
    {
        if (base + size < 0x80000 - D_80073830[D_8007382B[0]])
        {
            D_800869C0[0] = base;
            D_800869C0[1] = size;
        }
        else
        {
            return -1;
        }
    }
    else
    {
        i = 0;
        p = D_800869C0;
        for (; i < 0x10; i++)
        {
            if (p[i * 2 + 1] == 0)
            {
                base = p[i * 2];
                if (base + size < 0x80000 - D_80073830[D_8007382B[0]])
                {
                    p[i * 2] = base + size;
                    p[i * 2 + 1] = size;
                    break;
                }
                return -1;
            }
        }
    }
    func_80059DAC();
    return base;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_80059F8C);

/* UNPARKED. Both loops index off a base LOCAL with the index written first --
   `(u8 *) (i * 8 + (u32) b)`, not `b + i * 8` -- which is what gives the
   target's `addu $v0, $v0, $a2` operand order and stops loop.c walking the
   inner row pointer. `s32 i = 0;` is declared BEFORE `b` so its `addu $v1,
   $zero, $zero` comes out ahead of the base's lui/addiu. */
void func_8005A1D0(u32 k) {
    s32 i = 0;
    u8 *b = (u8 *) D_800869C0;

    for (; i < 0x10; i++) {
        if (*(u32 *) (i * 8 + (u32) b) == k) {
            while (i < 0x10) {
                u8 *d = (u8 *) (i * 8 + (u32) b);
                u8 *s = (u8 *) ((i + 1) * 8 + (u32) b);
                u32 v;

                *(u32 *) (d + 4) = *(u32 *) (s + 4);
                v = *(u32 *) s;
                *(u32 *) d = v;
                if (v == 0) {
                    break;
                }
                i++;
            }
            func_80059DAC();
            return;
        }
    }
}

/* Resets both tables. D_800869C0 is a pair of words per slot, cleared +4
   before +0. */
void func_8005A26C(void) {
    Unk80086A40 *p;
    Unk80086C00 *q;
    u32 *r;
    s32 i;

    p = D_80086A40;
    for (i = 0; i < 0x10; i++) {
        p[i].unk_00 = -1;
        p[i].unk_19 = 0x7F;
        p[i].unk_1A = 0x7F;
        p[i].unk_1B = 0x40;
    }
    q = D_80086C00;
    r = D_800869C0;
    for (i = 0; i < 0x10; i++) {
        q[i].unk_00 = -1;
        q[i].unk_02 = 0;
        q[i].unk_0A = 0x7F;
        q[i].unk_08 = 0x7F;
        r[i * 2 + 1] = 0;
        r[i * 2] = 0;
    }
    D_80086D4C[0] = 0x1000;
}

void func_8005D838(s32, u8 *);

void func_8005A2FC(void) {
    func_8005CA70();
    func_80056C30();
    func_8005A26C();
    func_8005D838(0x10, D_80086CC0);
}

void func_800585A0(void);

void func_8005A33C(void) {
    /* Nested ifs, not `> 0 && < 4`: written with && the two bounds combine
       into one `addiu -1` / `sltiu 3` range check (13 DIFF). */
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_800585A0();
        }
    }
    D_80073824[0] = 1;
}

void func_8005A33C(void);

void func_8005A37C(void) {
    func_8005A33C();
}

void func_8005A39C(u32 v) {
    D_80086D4C[0] = v;
}

void func_80058700(void);

void func_8005A3A8(void) {
    if (D_80073824[0] != 0) {
        func_80058700();
    }
}

void func_8005A3D4(void) {
    D_8007382A[0] = 0;
}

void func_8005A3E0(void) {
    D_8007382A[0] = 1;
}

s32 func_8005A3F0(u32 a) {
    a &= 0xFF;
    if (a >= 0x19) {
        return 0xFF;
    }
    if (a == 0) {
        return 0xFF;
    }
    D_80073734[0] = a;
    return a;
}

void func_8005A420(void) {
}

void func_8005863C(void);
void func_8005A1D0(u32);
void func_8005A26C(void);

/* Nested ifs for the 0 < x < 4 test, as in func_8005A33C: with `&&` the two
   bounds combine into one range check. */
void func_8005A428(void) {
    Unk80086A40 *p;
    s32 i;

    func_80056C30();
    if ((s32) D_80086D4C[0] > 0) {
        if ((s32) D_80086D4C[0] < 4) {
            func_8005863C();
        }
    }
    p = D_80086A40;
    for (i = 0; i < 8; i++) {
        if (p[i].unk_00 >= 0) {
            func_8005A1D0(p[i].unk_10);
        }
    }
    func_8005A26C();
}

void func_80056C30(void);
void func_8005A26C(void);
void func_8005D7BC(void);

void func_8005A4B8(void) {
    func_80056C30();
    func_8005A26C();
    func_8005D7BC();
}

void func_8005EDA0(s16 *);

void func_8005A4E8(u8 a, u8 b, u8 c) {
    u32 t[10];

    if (a == 0) {
        if (b == 0) {
            t[0] = 0x200;
            t[6] = c;
        } else {
            t[0] = 0x100;
            t[5] = c;
        }
    } else {
        if (b == 0) {
            t[0] = 0x2000;
            t[9] = c;
        } else {
            t[0] = 0x1000;
            t[8] = c;
        }
    }
    func_8005EDA0((s16 *) t);
}

void func_8005EDA0(s16 *);

/* The two shifts are hoisted out of the arms, and as `s32` locals: that is
   what sign-extends them (`sll 16` + `sra 8`). Written inside the arms, each
   store needs only the low half and gcc emits a bare `sll 8` twice over. */
void func_8005A56C(s16 f, s16 x, s16 y) {
    s16 t[20];
    s32 u = x << 8;
    s32 v = y << 8;

    if (f == 0) {
        *(s32 *) &t[0] = 0xC0;
        t[8] = u;
        t[9] = v;
    } else {
        *(s32 *) &t[0] = 0xC00;
        t[14] = u;
        t[15] = v;
    }
    func_8005EDA0(t);
}

void func_8005EDA0(s16 *);

void func_8005A5C8(s16 x, s16 y) {
    s16 t[20];

    *(s32 *) &t[0] = 0xF;
    t[2] = x << 7;
    t[3] = y << 7;
    t[4] = 0;
    t[5] = 0;
    func_8005EDA0(t);
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A608);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A778);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005A90C);

u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

/* The table base through a pointer local initialised in its declaration, so
   its lui/addiu comes out BEFORE the index's sign extension and the final
   `addu` has the operands the target's way round (8 DIFF indexing the symbol
   directly). */
s32 func_8005AAA8(u8 *p, s16 k) {
    Unk80086A40 *t = D_80086A40;
    u8 *e = (u8 *) &t[k];
    s16 n = *(s16 *) e;
    u32 r;

    if (n != k) {
        return -1;
    }
    r = func_8005ECA0(*(u32 *) (e + 0x10));
    if (func_8005EC40(p, *(u32 *) (e + 0x14)) != *(u32 *) (e + 0x14)) {
        return -1;
    }
    *(u32 *) (e + 0xC) = (u32) p;
    *(u32 *) (e + 0x10) = r;
    return n;
}

extern s32 D_8007382C[];

u32 func_8005ECA0(u32);
u32 func_8005EC40(u8 *, u32);

s32 func_8005AB50(u8 *p, u32 n, s16 k)
{
    Unk80086A40 *t = D_80086A40;
    u8 *e = (u8 *)&t[k];
    s16 id = *(s16 *)e;
    u32 avail;
    s32 cur;
    s32 lim;

    if (id != k)
    {
        return -1;
    }
    func_8005ECA0(*(u32 *)(e + 0x10) + D_8007382C[0]);
    avail = *(u32 *)(e + 0x14) - D_8007382C[0];
    if (avail < n)
    {
        n = avail;
    }
    if (func_8005EC40(p, n) != n)
    {
        return -1;
    }
    cur = D_8007382C[0];
    lim = *(s32 *)(e + 0x14);
    cur += n;
    D_8007382C[0] = cur;
    if (cur >= lim)
    {
        return id;
    }
    return -2;
}

s32 func_8005ECF8(s32);

s16 func_8005AC30(s16 a) {
    /* Two returns of the same call, not `func_8005ECF8(a != 0)`, which
       store-flags into one `sltu`. And the arm written FIRST ends up SECOND in
       memory: crossjumping merges the two tails and leaves the second arm as
       the fall-through, so `a == 0` first is what gives the target's `beqz`. */
    if (a == 0) {
        return func_8005ECF8(0);
    } else {
        return func_8005ECF8(1);
    }
}

void func_8005A1D0(u32);

/* The base read into a local first, so its lui/addiu come out ahead of the
   28-stride index chain (8 DIFF the other way round). The -1 is one constant
   shared by the compare and the store, which is why it sits in a callee-saved
   register across the call. */
void func_8005AC68(s16 i) {
    Unk80086A40 *b = D_80086A40;
    Unk80086A40 *p = &b[i];

    if (p->unk_00 != -1) {
        func_8005A1D0(p->unk_10);
        p->unk_00 = -1;
    }
}

void func_8005ACD0(u16 v) {
    D_80073828[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005ACDC);

/* The three magics are 'pQSE', 'MTdh' and 'KDT ' little-endian: the sequence
   header check, then the first free slot in D_80086C00 takes the pointer. */
s16 func_8005AE08(u32 *p, s16 n) {
    s32 i;

    if (*p != 0x53455170 && *p != 0x6468544D && *p != 0x2054444B) {
        return -1;
    }
    for (i = 0; i < 8; i++) {
        if (D_80086C00[i].unk_00 == -1) {
            D_80086C00[i].unk_04 = (u32) p;
            D_80086C00[i].unk_00 = n;
            D_80086C00[i].unk_02 = 2;
            return i;
        }
    }
    return -1;
}

extern s16 D_80086D48[];

void func_80056A08(void);
void func_800599B0(void);

void func_8005AE90(s16 k, u8 b, s16 c)
{
    Unk80086C00 *e;
    Unk80086C00 *t;

    if (k == -1)
    {
        return;
    }
    func_80056A08();
    t = D_80086C00;
    e = (Unk80086C00 *)(k * 12 + (u32)t);
    if (e->unk_00 == -1)
    {
        return;
    }
    D_80086D50a[0] = *(u16 *)e;
    D_80086D48[0] = c;
    D_800869B0[0] = 0x10000;
    D_80085FA4[0] = e->unk_04;
    func_800599B0();
    if (b == 0)
    {
        D_800737A4[0] = 0;
        D_80085F98[0] = 1;
        e->unk_02 = 4;
    }
    else
    {
        D_800737A4[0] = 1;
        D_80085F98[0] = 0;
        e->unk_02 = 1;
    }
}

void func_80059D1C(void);
void func_80056A08(void);

void func_8005AF74(s16 i) {
    Unk80086C00 *b;
    Unk80086C00 *e;

    D_80085F98[0] = 1;
    D_800737A4[0] = 0;
    func_80059D1C();
    func_80056A08();
    b = D_80086C00;
    e = &b[i];
    e->unk_0A = 0x7F;
    e->unk_08 = 0x7F;
    e->unk_02 = 2;
    D_80085F98[0] = 0;
}

void func_80059D1C(void);
void func_80056A08(void);

/* The entry base read into a local after the calls, so its lui/addiu come out
   ahead of the 0xC-stride index chain (13 DIFF the other way round). */
void func_8005AFF4(s16 i) {
    Unk80086C00 *b;
    Unk80086C00 *e;

    D_80085F98[0] = 1;
    func_80059D1C();
    func_80056A08();
    b = D_80086C00;
    e = &b[i];
    e->unk_00 = -1;
    e->unk_02 = 0;
    e->unk_0A = 0x7F;
    e->unk_08 = 0x7F;
    D_80085F98[0] = 0;
}

void func_8005F134(void *);

/* The D_80085458 sweep. Four facts, each worth two words because a hoisted
   invariant drags a callee-saved save and restore with it:

   * the entry pointer is computed INSIDE the body. Walked from a pointer
     declared outside, loop.c biases the giv to +0x1A -- the first field used
     -- and the two loads come out with negative displacements. Same recipe as
     func_80056558 above.
   * the bound is read in the loop CONDITION, not into a local.
   * the bound takes the INCOMPLETE ARRAY spelling. That is the OPPOSITE of
     func_80056558, which needs the scalar alias for the same symbol: here the
     listing has `lui $v0` / `lw $v1, %lo(...)($v0)`, the compiler-split form
     with a separate address temp, where the scalar gives the assembler
     macro's one-register form. Read the `lui`'s destination per site.
   * the stack primitive is an `Unk5F134SP`, not `u8 buf[0x40]` with casts.
     Struct accesses carry MEM_IN_STRUCT_P, which makes every varying-address
     load depend on them, and the body then comes out in strict source order.
     Through a byte array sched1 interleaves the loads with the stores.

   The store order matters with it: +0x04, then +0x00 from the table, then the
   two halfwords. With +0x00 written last the table load moves two slots
   earlier and takes a second register (4 DIFF). */
void func_8005B070(s16 n)
{
    Unk5F134SP buf;
    s32 i = 0;

    D_80085F98[0] = 1;
    while (i < (s32)D_80073734[0])
    {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *)(e + 6) < 0x10 && *(u16 *)(e + 0x1A) != 0)
        {
            buf.unk_04 = 3;
            buf.unk_00 = D_80073740[i];
            buf.unk_08 = 0;
            buf.unk_0A = 0;
            func_8005F134(&buf);
        }
        i++;
    }
    D_80086C00[n].unk_02 = 4;
    D_80085F98[0] = 0;
    D_800737A4[0] = 0;
}

/* func_8005B070's sweep with the two halfwords taken from the entry at +0x10
   and +0x12 instead of zeroed, and a different tail: +0x02 gets 1 and
   D_800737A4 gets 1, in the other order. The same four facts carry it; see
   above. */
void func_8005B16C(s16 n)
{
    Unk5F134SP buf;
    s32 i = 0;

    D_80085F98[0] = 1;
    while (i < (s32)D_80073734[0])
    {
        u8 *e = D_80085458 + i * 0x78;

        if (*(u16 *)(e + 6) < 0x10 && *(u16 *)(e + 0x1A) != 0)
        {
            buf.unk_04 = 3;
            buf.unk_00 = D_80073740[i];
            buf.unk_08 = *(u16 *)(e + 0x10);
            buf.unk_0A = *(u16 *)(e + 0x12);
            func_8005F134(&buf);
        }
        i++;
    }
    D_80086C00[n].unk_02 = 1;
    D_800737A4[0] = 1;
    D_80085F98[0] = 0;
}

void func_800564A8(s32);

void func_8005B27C(s16 i, u8 a, u8 b) {
    D_80086C00[i].unk_08 = a & 0x7F;
    D_80086C00[i].unk_0A = b & 0x7F;
    func_800564A8(i);
}

void func_8005B2C8(s16 i, s32 b, u16 *x, u16 *y) {
    *x = D_80086C00[i].unk_08;
    *y = D_80086C00[i].unk_0A;
}

void func_8005ACDC(void);

void func_8005B300(void) {
    func_8005ACDC();
}

void func_8005E450(s32);
void func_8005D88C(s32);

void func_8005B320(void) {
    func_8005E450(1);
    func_8005D88C(1);
}

void func_8005D88C(s32);
void func_8005E450(s32);

void func_8005B348(void) {
    D_8007382B[0] = 0;
    func_8005D88C(0);
    func_8005E450(0);
}

s32 func_8005DA88(void *);

s16 func_8005B378(s16 a) {
    s32 t[6];

    t[0] = 1;
    t[1] = a;
    if (func_8005DA88(t) != 0) {
        return -1;
    }
    D_8007382B[0] = a;
    return a;
}

s32 func_8005DA88(void *);

void func_8005B3D8(s32 x, s32 y) {
    s16 t[12];

    *(s32 *) &t[0] = 6;
    /* `(x << 16) >> 8`, not `(s16) x << 8`: the two shifts cannot be combined
       (the arithmetic right shift fills from bit 15, so the result is not
       x << 8), which is why the target spends both. Any spelling that means
       "sign-extend then shift" collapses to one `sll` because the value is
       stored as a halfword. */
    t[4] = (x << 16) >> 8;
    t[5] = (y << 16) >> 8;
    func_8005DA88(t);
}

void func_8005E97C(s32, s32);

void func_8005B418(s32 a) {
    s32 i = 0;
    s32 r = 0;

    /* The bound is read from the global IN the loop condition, not hoisted
       into a local: loop-invariant motion then lifts that read into the
       preheader as a COPY of the guard's read, which is the target's
       `addu $a2, $v0, $zero`. With a local it is one read and one register,
       a word short. The `(s32)` is what makes the tests `blez`/`slt`. */
    for (; i < (s32) D_80073734[0]; i++) {
        r |= D_80073740[i];
    }
    func_8005E97C(0, r);
}

s32 func_8005B470(s16 i, u8 *dst) {
    u8 *src;
    u32 j;

    if (D_80086A40[i].unk_00 == -1) {
        return -1;
    }
    src = D_80086A40[i].unk_04;
    for (j = 0; j < 0x20; j++) {
        *dst = *src;
        src++;
        dst++;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005B4D0);

void func_80056D44(s32, u8 *);
void func_8005E97C(s32, s32);

/* func_80057948's sibling: stops every D_80085458 entry whose +0x06 is at
   least 0x10 and whose +0x0A, +0x5C and +0x04 match the two arguments' high
   byte, high byte and low seven bits, OR-ing their D_80073740 handles
   together and stopping the accumulation at the end. D_80085F98 is held at 1
   across it.

   Same two idioms as func_80057948: the entry pointer is INDEXED inside the
   body (`D_80085458 + i * 0x78`) so loop.c keeps one giv with the field
   offsets as displacements, and `D_80073740[i]` is indexed rather than
   walked. The five tests are one `&&` chain, which is what gives the five
   branches to the shared skip label; the fields differ so nothing combines
   into a range check.

   The two identical func_80056D44/func_8005E97C pairs and the two
   `m |= D_80073740[i]` are in the listing exactly as written here. */
void func_8005BAB0(s32 a, s32 b)
{
    s32 i = 0;
    s32 m = 0;

    D_80085F98[0] = 1;
    if ((s32) D_80073734[0] > 0)
    {
        do
        {
            u8 *e = D_80085458 + i * 0x78;

            if (*(u16 *) (e + 0x1A) != 0 && *(u16 *) (e + 6) >= 0x10 &&
                *(u16 *) (e + 0xA) == (b >> 8) && *(u16 *) (e + 0x5C) == (a >> 8) &&
                *(u16 *) (e + 4) == (a & 0x7F))
            {
                m |= D_80073740[i];
                *(u16 *) (e + 0x1A) = 0;
                *(u16 *) (e + 0xA) = 0;
                func_80056D44(i, e);
                func_8005E97C(0, D_80073740[i]);
                func_80056D44(i, e);
                func_8005E97C(0, D_80073740[i]);
                m |= D_80073740[i];
            }
            i++;
        } while (i < (s32) D_80073734[0]);
    }
    func_8005E97C(0, m);
    D_80085F98[0] = 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005BBFC);

s32 func_8005EB78(u32);
s16 func_8005BBFC(s16, s16, s16, s16, s16, s16, s16, s16);

/* Finds a free slot -- first a D_80073740 handle func_8005EB78 reports idle,
   then a D_80085458 entry with a zero +0x1A -- and hands it plus all seven
   arguments to func_8005BBFC, returning -1 if neither sweep found one.
   D_80085F98 is held at 1 across the whole thing.

   The first sweep is a `goto` loop inside a `do { } while (0)`, and that pair
   is the lever. The listing wants BOTH halves of loop.c's behaviour split
   apart: `lui $s1, %hi(D_80073734)` hoisted out of the sweep (so only the
   `lw %lo(...)($s1)` is inside it), but the element address NOT
   strength-reduced -- `sll $v0, $s0, 2` / `addu $v0, $v0, $s2` / `lw` every
   iteration. A real `do`/`while` gives both: the giv's benefit is 4, one
   `add_cost` over the cut, so loop.c reduces it to a walking pointer and the
   function is two words short. A bare `goto` loop gives neither: with no
   NOTE_INSN_LOOP_BEG there is nothing to hoist from, the `lui` stays inside
   the sweep, the register pressure drops and the `c` parameter no longer
   spills to 0x20($sp) -- two words short again, differently.

   `do { } while (0)` round the goto loop gives exactly one of them. The
   wrapper is a loop STATEMENT, so loop.c scans the region and hoists the
   `(high D_80073734)` into the entry block; the back edge inside it is a
   `goto`, so there is no biv and nothing to reduce. The extra callee-saved
   register that the hoist occupies is what pushes `c` to its stack home.

   The SECOND sweep is a plain `do`/`while`, because there the listing does
   want the giv: `addiu $a0, $a0, 0x78` with +0x1A as the displacement, which
   needs the 0x78-stride base in a register (matching-levers section 9).

   `i = 0` is the function's first statement so that its `addu $s0, $zero,
   $zero` pairs with `sw $s0` ahead of the stack parameters' loads. */
s16 func_8005C130(s16 a, s16 b, s16 c, s16 d, s16 e, s16 f, s16 g)
{
    u32 *t;
    u8 *q;
    s32 i;

    i = 0;
    D_80085F98[0] = 1;
    t = D_80073740;
    do
    {
    scan:
        if (func_8005EB78(*(u32 *) (i * 4 + (u32) t)) == 0)
        {
            goto call;
        }
        i++;
        if (i <= (s32) D_80073734[0] - 1)
        {
            goto scan;
        }
        i = -1;
    } while (0);
call:
    if (i == -1)
    {
        q = D_80085458;
        i = 0;
        do
        {
            if (*(u16 *) (q + i * 0x78 + 0x1A) == 0)
            {
                goto call2;
            }
            i++;
        } while (i <= (s32) D_80073734[0] - 1);
        i = -1;
    call2:
        if (i == -1)
        {
            goto out;
        }
    }
    i = func_8005BBFC(i, a, b, c, d, e, f, g);
out:
    D_80085F98[0] = 0;
    return i;
}

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C2C0);

INCLUDE_ASM("asm/nonmatchings/unk_1C540", func_8005C4D0);

s32 func_80059D60(void);

/* The base into a local first so its lui/addiu precede the 0xC-stride index
   chain, and the return RE-INDEXES rather than reusing `e`. Unk80086C00.unk_02
   is signed: the target reads it with `lh`. */
s16 func_8005C5D0(s16 i) {
    Unk80086C00 *b = D_80086C00;
    Unk80086C00 *e = &b[i];
    s32 r;

    if (e->unk_02 == 1) {
        r = func_80059D60();
        if (r == 3) {
            e->unk_02 = r;
        }
    }
    return D_80086C00[i].unk_02;
}

void func_8005F134(void *);

s32 func_8005C668(s16 i, s16 a, s16 b) {
    u32 t[0x10];

    t[1] = 3;
    t[0] = D_80073740[i];
    *(u16 *) ((u8 *) t + 8) = a;
    *(u16 *) ((u8 *) t + 0xA) = b;
    func_8005F134(t);
    return 0;
}

void func_8005F134(void *);

/* An `s16` block here, where func_8005C668's is `u32` with casts for the two
   halfwords: the masked stores have to be ARRAY_REFs on the DECL so that
   MEM_IN_STRUCT_P is set on them. Through a cast they are plain INDIRECT_REFs,
   sched1's alias calls answer differently and the `andi`/`sll` pair for the
   first halfword floats to the top of the function (11 DIFF, same words). */
s32 func_8005C6B4(s16 i, s16 a, s16 b) {
    s16 t[0x20];

    *(u32 *) &t[2] = 3;
    *(u32 *) &t[0] = D_80073740[i];
    t[4] = (a & 0x7F) << 7;
    t[5] = (b & 0x7F) << 7;
    func_8005F134(t);
    return 0;
}

void func_8005FA34(u32 *);

/* A u32 block, not a u8 one: as `u8 t[0x40]` with `*(u32 *)` casts the two
   stores and the table load come out in the other order (3 DIFF). */
s32 func_8005C710(s16 i, s16 *a, s16 *b) {
    u32 t[0x10];

    t[1] = 0;
    t[0] = D_80073740[i];
    func_8005FA34(t);
    *a = *(u16 *) ((u8 *) t + 8);
    *b = *(u16 *) ((u8 *) t + 0xA);
    return 0;
}

void func_8005FA34(u32 *);

s32 func_8005C784(s16 i, s16 *a, s16 *b) {
    u32 t[0x10];

    t[1] = 0;
    t[0] = D_80073740[i];
    func_8005FA34(t);
    *a = (s16) *(u16 *) ((u8 *) t + 8) >> 7;
    *b = (s16) *(u16 *) ((u8 *) t + 0xA) >> 7;
    return 0;
}

u32 func_8005C808(void) {
    return D_80085FCC[0];
}

void func_8005C818(s32 a, u32 b) {
    u32 i = 0;

    if (D_800869B4 != 0) {
        do {
            D_80085FA8s[i].unk_24 = b;
            D_80085FA8s[i].unk_20 = b;
            i++;
        } while (i < D_800869B4);
    }
}

s32 func_8005C858(s32 a, s16 i) {
    return D_80084960[i].unk_0C;
}

void func_80057D20(s16, s32, u32);

void func_8005C88C(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 7, c & 0x7F);
}

s32 func_8005C8B8(s32 a, s16 i) {
    return D_80084960[i].unk_04;
}

void func_80057D20(s16, s32, u32);

void func_8005C8EC(s32 arg0, s16 b, u32 c) {
    func_80057D20(b, 0xA, c & 0x7F);
}

s32 func_8005C918(s32 a, s16 i) {
    return D_80084960[i].unk_1C;
}

void func_8005C94C(s32 a, s16 i, u8 v) {
    D_80084960[i].unk_1C = v & 0x7F;
}

void func_8005C980(void) {
}

void func_8005C988(void) {
}

void func_8005C990(void) {
}

void func_8005C998(void) {
}

void func_8005C9A0(void) {
}
