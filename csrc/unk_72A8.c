#include "common.h"

/* Bare file-scope size records: each symbol lands in maspsx's sdata_entries
   while nothing is DEFINED, so maspsx rewrites every load and store of it in
   this unit to %gp_rel while leaving address materialisation absolute. See the
   $gp section of docs/carved-sweep.md; adding a symbol changes every access to
   it in this whole unit, so grep this file first.
   D_8008099C is deliberately NOT here, and cannot be: func_80034EF8 and
   func_8003C0C8 in this unit reach it ABSOLUTELY and match that way, while
   func_80033D44 and func_80033D54 need it $gp-relatively. One unit cannot
   give one symbol both forms, so those two were carved off into
   csrc/gprel_72A8.c, which is why this unit now starts at file 0x7db8
   (func_800345B8) while keeping the name it had when it started at 0x72a8.

   THE __asm__-ALIAS TRICK WAS TRIED FIRST AND DOES NOT LIFT IT, 2026-10-27,
   which is why the carve was needed. The idea was the one that gives one
   symbol two spellings elsewhere in this project: add the size record, leave
   the two blocked functions on the scalar declaration (bare symbol -> maspsx
   rewrites to %gp_rel) and give the two landed ones
   `extern u32 D_8008099Ca[] __asm__("D_8008099C")` with `[0]`
   (compiler-split -> maspsx leaves it alone). It builds and it is NOT
   byte-identical: the landed form is `lui $at, 0x8008` +
   `sw $zero, 0x99c($at)`, i.e. GAS expanding a bare symbol through the
   assembler temp, and the array spelling makes cc1psx emit the pair itself
   through a real register ($v0). The absolute form those two match is
   specifically "bare symbol, not in sdata_entries, expanded by gas", which no
   C spelling reproduces once the symbol IS in sdata_entries. */
__asm__(".sdata\n\t.size D_80080A6C,4\n\t.size D_80081440,4\n"
        "\t.size D_80081448,4\n\t.size D_80081460,4\n"
        "\t.size D_800809B8,4\n\t.size D_800809BC,4\n"
        "\t.text");

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800345B8);

void func_80034E40(void) {
}

void func_8003C920(void);
void func_80034EC4(u8 *);

void func_80034E48(void) {
    u8 *p;

    func_8003C920();
    p = (u8 *) D_80082A38;
    func_80034EB4(p);
    func_80034EC4(p);
    ClearImage(D_800809A8, 0, 0, 0);
    ClearImage(D_800809B0, 0, 0, 0);
}

void func_80034EB4(u8 *p) {
    p[0] = 0;
    p[1] = 0;
    *(u32 *) (p + 0x34) = 0;
}

void func_80034EC4(u8 *p) {
    u8 *q = p + 8;
    /* A local for the table base, so the symbol is materialised once and both
       words stay displacements off it. */
    u8 *b = D_8006ADBC;

    p[2] = 2;
    *(u8 **) (p + 0x10) = q;
    *(u8 **) (p + 0xC) = q;
    *(u32 *) (p + 0x44) = *(u32 *) (b + 0x20);
    *(u32 *) (p + 0x40) = *(u32 *) (b + 0x1C);
}

void func_80037D50(u8 *, u8);

void func_80034EF8(u8 *p, u8 v) {
    /* The table byte is read into a local BEFORE the last two stores, which is
       what fills the load's delay slot; as the last statement's RHS the load
       lands next to its own use and costs a nop. */
    u8 c;

    D_8008099C = 0;
    func_80037D50(p, v);
    p[2] = v;
    p[0] = 0;
    p[1] = 0;
    c = D_8007216C[0];
    p[0x3D] = 0;
    *(u32 *) (p + 0x34) = 0;
    p[0x3C] = c;
}

void func_80035484(u8 *, u8 *, s32);

void func_80034F58(s32 a) {
    func_80035484(D_800820A8, D_80081EB0, a);
}

void func_80034F88(u32 *p, u32 v) {
    p[13] = v;
    p[14] = v;
    p[66] = (u32) &p[50];
}

void func_80034F9C(u8 *p) {
    if (func_80034FD0((u32 *) p) != 0) {
        p[0] = 1;
    }
}

s32 func_80034FD0(u32 *p) {
    return p[4] != (u32) &p[2];
}

s32 func_80034FE4(u32 *p) {
    return p[39] != (u32) func_80038A00;
}

/* The split spelling: the scalar D_80082E6A's load is one insn before maspsx
   and comes out in one register, where the target has the pair across two. */
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s32 func_80034FFC(u32 *p) {
    s32 r;

    /* The `!= 2` arm is written FIRST so the bare `return func_80034FE4();`
       ends up last in memory, which is where the target has it. */
    if (D_80082E6Aa[0] != 2) {
        r = 0;
        if (func_80034FE4(p) != 0 || D_8006ADE8[0] != 0) {
            r = 1;
        }
        return r;
    }
    return func_80034FE4(p);
}

void func_80035060(u8 *p, u32 v) {
    p[3] = v;
}

void func_80035068(u8 *p, u8 *n) {
    *(u8 **) (n + 0x4) = p + 0x8;
    *(u8 **) (n + 0x8) = *(u8 **) (p + 0x10);
    *(u8 **) (*(u8 **) (p + 0x10) + 0x4) = n;
    *(u8 **) (p + 0x10) = n;
}

void func_80035090(u32 *p) {
    ((u32 *) p[1])[2] = p[2];
    ((u32 *) p[2])[1] = p[1];
}

void func_800350B0(u8 *a, u8 *p) {
    *(u8 **) (p + 0x54) = p;
    func_80035068(a, p + 0x54);
}

void func_80033C84(u32 *);
void func_80035090(u32 *);
void func_8003FFF0(u8 *);

/* Drains the list at p+0x10 (the head is p+8, the forward link +8 off it):
   each link stores the node that owns it, so `e` is the node and `e - 0x20` is
   its allocation header. The head is re-read by the loop condition, which is
   what the target does -- the three calls can move it. */
void func_800350D4(u8 *p) {
    u8 *h = p + 8;
    u8 *n;
    u8 *e;
    u8 *d;

    while ((n = *(u8 **) (h + 8)) != h) {
        e = *(u8 **) n;
        if (*(u32 **) e != 0) {
            func_80033C84(*(u32 **) e);
        }
        func_80035090((u32 *) n);
        d = e - 0x20;
        func_80044A50((UnkNode *) d);
        func_8003FFF0(d);
    }
}

void func_80037D9C(u8 *, u8);

/* Both calls written out in all three arms: an `&&` or a nested `if` inverts
   both branch senses (28 DIFF either way), and a shared `u8 *q = (u8 *) p;`
   local costs a fourth callee-saved register. gcc's cross-jumping merges the
   tails back to the target's single func_80037D9C and func_80034F88 sites. */
void func_80035164(u8 *p, s32 b, u8 c) {
    if (func_80034FD0((u32 *) p) != 0) {
        if (p[0] != 0) {
            func_800350D4(p);
            func_80037D9C(p, c);
        } else if (p[2] != c) {
            func_800350D4(p);
            func_80037D9C(p, c);
        } else {
            func_80034F88((u32 *) p, b);
            return;
        }
    } else {
        func_80037D9C(p, c);
    }
    func_80034F88((u32 *) p, b);
}

void func_80035208(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 2);
}

void func_80035234(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 1);
}

void func_80035260(s32 a) {
    func_80035164((u8 *) D_80082A38, a, 0);
}

void func_8003528C(u32 v) {
    func_80034F88((u32 *) D_80082A38, v);
}

void func_800352B4(void) {
    func_80034F9C((u8 *) D_80082A38);
}

void func_800352D8(void) {
    func_80034FD0((u32 *) D_80082A38);
}

s32 func_80034FFC(u32 *);

/* Returns func_80034FFC's answer: func_800353F4 tests it. The tail call puts
   it in $v0 either way, so the code is the same as the `void` spelling's. */
s32 func_800352FC(void) {
    return func_80034FFC((u32 *) D_80082A38);
}

void func_80035320(u32 v) {
    func_80035060((u8 *) D_80082A38, v);
}

void func_80035348(void) {
    u32 *b = (u32 *) D_80082A38;
    s32 i;

    b[0xE] = 0;
    for (i = 0xA; i < 0xF; i++) {
        b[0x12 + i] = 0;
    }
}

/* The five-slot queue at D_80082A38+0x48, indexed 10..14. The 0x48 is added to
   `&D_80082A38w[i]` rather than written inside the index: as `[i + 0x12]`
   loop.c folds 0x48 into the giv's initial value and the function is four
   words longer. D_80082AB8 is the last slot reached as its own symbol, through
   the incomplete-array spelling so the store shares the `lui` instead of going
   through the assembler's $at. */
void func_80035378(u32 v) {
    s32 i;

    for (i = 10; i < 15; i++) {
        u8 *q = (u8 *) &D_80082A38w[i];

        if (*(u32 *) (q + 0x48) == 0) {
            *(u32 *) (q + 0x48) = v;
            if (i != 14) {
                *(u32 *) (q + 0x4C) = 0;
            }
            return;
        }
    }
    for (i = 11; i < 14; i++) {
        u8 *q = (u8 *) &D_80082A38w[i];

        *(u32 *) (q + 0x48) = *(u32 *) (q + 0x4C);
    }
    D_80082AB8[0] = v;
}

extern u32 D_80082A70[];
extern u8 D_8006B230[];

s32 func_800352FC(void);
void func_80035208(s32);

/* The retry gate: if the engine is idle, or its current script is not
   D_8006B230, it clears the five-slot queue, re-queues `v` and re-arms
   D_8006B230; otherwise it only re-queues. Either way it answers 1.

   The `||` is what gives the first test a branch INTO the body and the second
   one a branch past it (as nested ifs, 22 DIFF), and the clear is
   func_80035378's own `&D_80082A38w[i]` plus a 0x48 displacement -- written
   `[i + 0x12]` instead, loop.c folds the 0x48 into the giv's initial value.

   D_80082A70 IS AN INCOMPLETE ARRAY read through `[0]`, and that is the last
   two words. The scalar spelling puts the `lui` and the `lw` in ONE register
   (`lui $v1` / `lw $v1, 0($v1)`), so the pair cannot straddle the `beqz`;
   declared `extern u32 D_80082A70[];` the high half gets its own register,
   reorg puts `lui $v0, %hi(D_80082A70)` in the branch's delay slot and the
   load reads `%lo(D_80082A70)($v0)` after it, which is the listing. Both
   operand orders of the comparison are 2 DIFF with the scalar form, and a
   scalar D_8006B230 with `&` is 37 words. See matching-levers section 6. */
s32 func_800353F4(u32 v) {
    s32 i;

    if (func_800352FC() == 0 || D_80082A70[0] != (u32) D_8006B230) {
        for (i = 0xA; i < 0xF; i++) {
            u8 *q = (u8 *) &D_80082A38w[i];

            *(u32 *) (q + 0x48) = 0;
        }
        func_80035378(v);
        func_80035208((s32) D_8006B230);
        return 1;
    }
    func_80035378(v);
    return 1;
}

extern UnkNode *D_80082BC0[];
extern u8 D_80073B54[];
extern s16 D_8006A93C[];
extern u32 D_8006A8F0[];
extern s32 D_80081440;

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_800355EC(u8 *);
void func_80035614(u8 *);
void func_80033D54(void);

/* The 0x11 spawn: allocates the node, installs func_800355EC at its +0x10 and
   func_80033D54 through func_8004491C, records it in D_80081440, fills the
   +0x20 sub-object and the +0x08/+0x0C records, and sets the last two fields
   from the D_8006A93C and D_8006A8F0 tables indexed by the third argument's
   +0x02 byte.

   MATCHED on the first draft, which is worth a note on why: every shape it
   needs was already settled by its three siblings. `D_80082BC0` is an
   incomplete ARRAY passed by name (func_80036B10's lever -- it puts the
   lui/addiu between the two parameter save/load pairs); `e = (u8 *) n + 0x20`
   is its own local while the +0x20 STORE and the three reads that follow are
   spelled `(u8 *) n + 0x20` through the node, which is what names $s0 and
   $s1 the way the listing does (func_80035E58's second finding); and the two
   table lookups re-read their bases (`+0x20` for the first, `+0x74` for the
   second) rather than reusing `e` or `c`, because the stores between them
   flush cse.

   The signature is the unit's own `void func_80035484(u8 *, u8 *, s32);` from
   above, with the casts inside; the third argument is used as a POINTER here
   (`lbu 0x2($s4)`), but the caller at func_800354xx passes an s32, and the
   declared form matches too. */
void func_80035484(u8 *a, u8 *b, s32 c) {
    UnkNode *n;
    u8 *e;
    u8 *g;
    u8 *h;

    n = func_8003FF2C(0x11, (UnkNode *) a, 0x49, D_80082BC0);
    D_80081440 = (s32) n;
    n->unk_10 = (u32) func_800355EC;
    func_8004491C(n, (u32) func_80033D54);
    g = (u8 *) n->unk_08;
    h = (u8 *) n->unk_0C;
    func_80033C1C(b, 0);
    *(u32 *) ((u8 *) n + 0x20) = (u32) b;
    *(u32 *) (b + 0x60) = 6;
    e = (u8 *) n + 0x20;
    *(u32 *) (*(u32 *) ((u8 *) n + 0x20) + 0x10) = 0;
    *(s16 *) (*(u32 *) ((u8 *) n + 0x20) + 8) = 0x400;
    *(u32 *) (e + 4) = 0;
    *(u32 *) (e + 8) = 0;
    func_800350B0((u8 *) c, e);
    *(u32 *) (e + 0x68) = (u32) func_80035614;
    *(s16 *) (g + 2) = -0x86;
    *(s16 *) (g + 6) = 0x78;
    *(s16 *) (g + 0xA) = 0x200;
    *(u32 *) (h + 0xC) = 0x808080;
    *(u32 *) (h + 8) = (u32) D_80073B54;
    *(u32 *) h = 0;
    h[4] = 0;
    h[5] = 0;
    *(u32 *) (e + 0x74) = c;
    *(u32 *) (*(u32 *) ((u8 *) n + 0x20) + 0x14) = D_8006A93C[((u8 *) c)[2]];
    *(u32 *) (h + 8) = D_8006A8F0[(*(u8 **) (e + 0x74))[2]];
}
void func_800355EC(u8 *p) {
    (*(void (**)(u8 *)) (p + 0x68))(p);
}

void func_80035888(u8 *, u8 *, u8 *, s32, u32);
void func_8003566C(u8 *);

void func_80035614(u8 *p) {
    func_80035888(D_800821CC, D_80081F14, p, 0, *(u32 *) (p + 0x74));
    p[0x4C] = 0;
    *(u32 *) (p + 0x68) = (u32) func_8003566C;
}

extern u8 D_800822F0[];
extern u8 D_80081F78[];
extern u8 D_8006A86C[];
void func_80035E58(u8 *, u8 *, u8 *, u8 *, s32, u32);
void func_8003571C(u8 *);

/* THREE separate locals for the three reads of `*(u8 **) p`, not one `q`
   reassigned: as one variable it is one allocno and the pointer and the
   halfword swap registers in the first statement (8 DIFF). */
void func_8003566C(u8 *p) {
    u8 *q;
    u8 *r;
    u8 *s;

    q = *(u8 **) p;
    *(u16 *) (q + 8) = *(u16 *) (q + 8) - 0xA0;
    r = *(u8 **) p;
    if (*(s16 *) (r + 8) < 0) {
        *(s16 *) (r + 8) = 0;
    }
    s = *(u8 **) p;
    if (*(s16 *) (s + 8) < 0x100) {
        func_80035E58(D_800822F0, D_80081F78, D_8006A86C, p, 1,
                      *(u32 *) (p + 0x74));
        p[0x4D] = 0;
        *(u32 *) (p + 0x68) = (u32) func_8003571C;
    }
}

void func_80035760(u8 *);

void func_8003571C(u8 *p) {
    *(u16 *) (*(u8 **) p + 8) -= 0xA0;
    if (*(s16 *) (*(u8 **) p + 8) < 0) {
        *(u16 *) (*(u8 **) p + 8) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80035760;
    }
}

void func_800357B0(u8 *);

void func_80035760(u8 *p) {
    u8 *q = *(u8 **) (p + 0x74);

    if (*q == 1 || p[0x51] == 0xFF) {
        *q = 1;
        p[0x4C] = 0xFE;
        p[0x4D] = 0xFE;
        p[0x51] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800357B0;
    }
}

void func_800357D0(u8 *);

void func_800357B0(u8 *p) {
    if (*(u16 *) (p + 0x50) == 0x101) {
        *(u32 *) (p + 0x68) = (u32) func_800357D0;
    }
}

void func_80035820(u8 *);

void func_800357D0(u8 *p) {
    *(u16 *) (*(u8 **) p + 8) += 0x40;
    if (*(s16 *) (*(u8 **) p + 8) > 0x400) {
        p[0x4C] = 0xFF;
        p[0x4D] = 0xFF;
        *(u32 *) (p + 0x68) = (u32) func_80035820;
    }
}

void func_8003FFF0(u8 *);

void func_80035820(u8 *p) {
    u8 *q;

    if (*(u16 *) (p + 0x50) == 0x101) {
        func_80034EB4(*(u8 **) (p + 0x74));
        func_80033C84(*(u32 **) p);
        func_80035090((u32 *) (p + 0x54));
        q = p - 0x20;
        func_80044A50((UnkNode *) q);
        func_8003FFF0(q);
    }
}

extern UnkNode *D_80082BC0[];
extern u8 D_8006A9E8[];
extern u8 D_8007386C[];
extern s16 D_8006A90C[];
extern u32 D_800809B8;
extern u32 D_800809BC;

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
s32 func_8004491C(UnkNode *, u32);
void func_800375C0(u8 *, u8 *);
void func_800359FC(u8 *, u8 *, s32);
void func_80035AAC(u32 *);
void func_80033D54(void);
void func_80037714(u8 *);

/* The five-argument spawn, func_80035484's and func_80035E58's sibling:
   allocates the node, installs func_800359FC at its +0x10 and func_80033D54
   through func_8004491C, fills the +0x20 sub-object from the argument record
   and the +0x08/+0x0C records, and sets the last field from D_8006A90C
   indexed by the fifth argument's +0x02 byte.

   Everything structural came from the siblings on the first compile, eight
   words out. The eight were ONE SWAP: `*(u32 *) (e + 0x14) = D_800809B8;`
   comes BEFORE `*(u32 *) (e + 0x68) = (u32) func_80035AAC;`. With the
   function pointer first, the 0x808080 constant that the +0x0C store wants
   lands in $v1 instead of the listing's $a0, and its `lui`/`ori` pair sits
   where sched2 put it rather than split across the group as the listing has
   it -- the $gp-relative load coming first is what lengthens the other
   quantities' live ranges enough to leave $a0 to the constant.

   perm.py over all nine stores of that group (250 of 362880 sampled) found a
   6-DIFF ordering, better than the 8 this started at but NOT this: it scatters
   the three zero stores through the group, which fixes the register by
   accident and breaks the order of six stores. The listing's own store order
   is the one below, and only the first two statements move.

   The signature is the unit's own `(u8 *, u8 *, u8 *, s32, u32);` from above,
   with the casts inside: the third argument is a `u32 *` here and the fifth a
   pointer, but the call site passes them the declared way and both spellings
   match. */
void func_80035888(u8 *a, u8 *b, u8 *d, s32 i, u32 f)
{
    UnkNode *n;
    u8 *e;
    u8 *g;
    u8 *h;

    n = func_8003FF2C(0x11, (UnkNode *) a, 0x49, D_80082BC0);
    n->unk_10 = (u32) func_800359FC;
    func_8004491C(n, (u32) func_80033D54);
    e = (u8 *) n + 0x20;
    g = (u8 *) n->unk_08;
    h = (u8 *) n->unk_0C;
    *(u32 *) (e + 0x44) = (u32) d;
    e[0x48] = 1;
    *(s16 *) (e + 0x4A) = i;
    func_80033C1C(b, *(u32 *) d);
    *(u32 *) ((u8 *) n + 0x20) = (u32) b;
    *(u32 *) (e + 4) = 0;
    *(u32 *) (e + 8) = (u32) func_80037714;
    func_800375C0(e, D_8006A9E8);
    func_800350B0((u8 *) f, e);
    *(u32 *) (e + 0x14) = D_800809B8;
    *(u32 *) (e + 0x68) = (u32) func_80035AAC;
    *(u32 *) (e + 0x18) = D_800809BC;
    *(s16 *) (g + 2) = 0;
    *(u32 *) (h + 0xC) = 0x808080;
    *(u32 *) (h + 8) = (u32) D_8007386C;
    *(u32 *) h = 0;
    h[4] = 0;
    h[5] = 0;
    *(u32 *) (e + 0x74) = f;
    e[0x78] = ((u8 *) f)[1];
    *(u32 *) (*(u32 *) ((u8 *) n + 0x20) + 0x34) = D_8006A90C[(*(u8 **) (e + 0x74))[2]];
}
void func_80035CE4(u8 *, u8 *, s32);
void func_80037C7C(u8 *, s16);

/* The +0x44 pointer and the +0x4A index in SEPARATE statements, pointer
   first: that is the func_800368D0 family's load-order lever (sched1's
   tie-break is RTL order), and here the registers come out right as well, so
   it lands. Both calls take their arguments straight from the parameter
   registers -- nothing sets $a0 before either `jal`. */
void func_800359FC(u8 *p, u8 *q, s32 r) {
    u8 *b;
    s16 i;
    u8 *e;

    if (func_80037534(p, (s32) q, r) != 0) {
        return;
    }
    if (p[0x48] != 0) {
        b = *(u8 **) (p + 0x44);
        i = *(s16 *) (p + 0x4A);
        e = b + i;
        if (e[0x4C] == 0xFE) {
            func_8003C24C();
            *(u32 *) (p + 0x68) = (u32) func_80035CE4;
        }
    }
    (*(void (**)(u8 *, u8 *, s32)) (p + 0x68))(p, q, r);
    func_80037C7C(p, *(s16 *) (q + 2));
}

void func_80035AAC(u32 *p) {
    p[0x1A] = (u32) func_80035AF8;
}

void func_80035ABC(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v -= 0x10;
    if ((s16) *v < 2) {
        *v = 2;
        *(u32 *) (p + 0x68) = (u32) func_80035AF8;
    }
}

void func_8003AE60(s16, s16, s16, s16, u8, u8, u8);
u32 func_80035DBC(s32);
void func_8003AC1C(s16, s16, u8, u8 *);
u32 *func_8003C8E4(s32);
s32 func_8003C890(s32, s32);
void func_8003C2B8(void);
void func_80035BE0(u8 *);
void func_80035C44(u8 *, u8 *);

void func_80035AF8(u8 *p)
{
    u8 *e;
    u8 n;

    n = *(u8 *)(*(u8 **)(p + 0x74) + 1);
    if (n == 0)
    {
        return;
    }
    p[0x78] = n;
    func_8003AE60(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 0x30, 0x10, 0, 0, 0);
    func_8003AC1C(*(s16 *)(p + 0x14), *(s16 *)(p + 0x16), 1, (u8 *)func_80035DBC(p[0x78]));
    e = (u8 *)func_8003C8E4(p[0x78]);
    if (e != 0)
    {
        u8 c = e[2];

        p[0x79] = c;
        if (func_8003C890(p[0x78], (s8)c) != 0)
        {
            e[2] = e[3];
            func_8003C2B8();
            *(s16 *)(p + 0x64) = 1;
            *(u32 *)(p + 0x68) = (u32)func_80035BE0;
        }
        else
        {
            *(u32 *)(p + 0x68) = (u32)func_80035C44;
        }
    }
    else
    {
        *(u32 *)(p + 0x68) = (u32)func_80035C44;
    }
}

s32 func_8003C7D8(s32, s32);
void func_8003C160(void);
void func_80035C44(u8 *, u8 *);

void func_80035BE0(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c - 1;
    if ((s16) *c < 0) {
        func_8003C7D8(p[0x78], *(s8 *) (p + 0x79));
        func_8003C160();
        *(u32 *) (p + 0x68) = (u32) func_80035C44;
    }
}

void func_80035C80(u8 *);

void func_80035C44(u8 *p, u8 *q) {
    u16 *v = (u16 *) (q + 2);

    *v += 0x10;
    if ((s16) *v > 0x40) {
        *v = 0x40;
        *(u32 *) (p + 0x68) = (u32) func_80035C80;
    }
}

void func_80035D38(void);

/* The func_800368D0 family: four functions that all end by storing through
   `*(u8 **)(p + 0x44) + *(s16 *)(p + 0x4A)`, and all four were two registers
   wrong at the exact word count until func_80036F24 showed the spelling. The
   +0x44 pointer takes a local -- that is what puts its `lw` ahead of the `lh`,
   sched1's tie-break being RTL order -- and the index stays INSIDE the
   subscript. Given its own local the index is a second quantity with the
   shorter live range, so local-alloc hands it $v0 and the pointer $v1, the
   reverse of every one of these listings, and the `addu`'s operands swap with
   them. func_800359FC above is the one member that wanted both as locals. */
void func_80035C80(u8 *p)
{
    u8 *t;

    if (p[0x78] != (*(u8 **)(p + 0x74))[1])
    {
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
        func_8003C24C();
        *(u32 *)(p + 0x68) = (u32)func_80035ABC;
    }
}

void func_80035CE4(u8 *p, u8 *q, s32 r)
{
    u8 *t;

    *(u16 *)(q + 2) = *(u16 *)(q + 2) - 0x20;
    if (*(s16 *)(q + 2) < 2)
    {
        *(u16 *)(q + 2) = 2;
        *(u32 *)(p + 0x68) = (u32)func_80035D38;
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
    }
}

void func_80035D38(void) {
}

void func_80035D40(u32 v) {
    D_8006ADB8[0] = v;
}

/* `k == p[0]`, not `p[0] == k`: the operand order survives into the `bne`. */
void func_80035D4C(s16 k) {
    s16 *p = D_8006A94C;

    while (p[0] != 0) {
        if (k == p[0]) {
            func_80033AA8(p[1]);
        }
        p += 2;
    }
}

void func_80035D4C(s16);

/* The parameter is s32 -- only the call takes an s16, which is where the
   sll/sra comes from; as an s16 parameter the three comparisons each want the
   sign-extended copy and the function is five words longer. */
u32 func_80035DBC(s32 k) {
    func_80035D4C((s16) k);
    if (k == 1) {
        return D_8006A944;
    }
    if (k == 2) {
        return D_8006A948;
    }
    if (k == 0xA && func_80033B2C(0x1473) == 0) {
        return D_80080A50;
    }
    return D_8006AC5C[k];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80035E58);

void func_80036900(u8 *, u8 *, s32);

/* A (p, q, r) HANDLER, like func_800359FC: the last two parameters are unused
   except as the pass-through to the indirect call, and their allocnos are what
   keep the +0x4C byte off $a1 -- declared `(u8 *p)` alone it lands in $a1
   where the target has $a3, 3 DIFF that no cast, cache or branch shape moves.
   And the +0x44 pointer and the +0x4A index in separate statements, pointer
   first, as that family always wants. */
void func_8003605C(u8 *p, u8 *q, s32 r) {
    u8 *b;
    s16 i;
    u8 *e;
    u8 c;
    u8 *d;

    if (p[0x48] != 0) {
        b = *(u8 **) (p + 0x44);
        i = *(s16 *) (p + 0x4A);
        e = b + i;
        c = e[0x4C];
        if (c == 0xFF) {
            e[0x50] = 1;
            func_80033C84(*(u32 **) p);
            func_80035090((u32 *) (p + 0x54));
            d = p - 0x20;
            func_80044A50((UnkNode *) d);
            func_8003FFF0(d);
            return;
        }
        if (c == 0xFE) {
            p[0x4C] = 0xFF;
            p[0x4D] = 0xFF;
            *(u32 *) (p + 0x68) = (u32) func_80036900;
        }
    }
    (*(void (**)(u8 *, u8 *, s32)) (p + 0x68))(p, q, r);
}

void func_80036154(u8 *);

void func_80036110(u8 *p) {
    *(u16 *) (*(u8 **) p + 0xA) -= 0x140;
    if (*(s16 *) (*(u8 **) p + 0xA) < 0) {
        *(u16 *) (*(u8 **) p + 0xA) = 0;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80036210(u8 *);
void func_800362D0(u8 *);
void func_800364EC(u8 *);
void func_8003666C(u8 *);
void func_800368D0(u8 *);
void func_80038478(u8 *);
void func_80038588(u8 *);
void func_800385E8(u8 *);
void func_80038648(u8 *);
void func_80038A00(void);
void func_80036988(u8 *, u8 *);

void func_80036154(u8 *p) {
    u32 h = *(u32 *) (*(u8 **) (p + 0x74) + 0x10);

    if (h == (u32) func_80038478) {
        *(u32 *) (p + 0x68) = (u32) func_80036210;
    } else if (h == (u32) func_80038588) {
        *(u32 *) (p + 0x68) = (u32) func_800362D0;
    } else if (h == (u32) func_800385E8) {
        *(u32 *) (p + 0x68) = (u32) func_800364EC;
    } else if (h == (u32) func_80038648) {
        *(u32 *) (p + 0x68) = (u32) func_8003666C;
    } else if (h == (u32) func_80038A00) {
        *(u32 *) (p + 0x68) = (u32) func_800368D0;
    }
    func_80036988(p + 0xC, *(u8 **) (p + 0x74));
    (*(u8 **) (p + 0x74))[0x85] = 1;
}

void func_80036B10(u8 *, u8 *, u8 *, s32, u32);
void func_8003626C(u8 *);

void func_80036210(u8 *p) {
    func_80036B10(D_80082414, D_80081FDC, p, 0,
                  *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    p[0x4C] = 0;
    *(u32 *) (p + 0x68) = (u32) func_8003626C;
}

void func_800362A0(u8 *);

void func_8003626C(u8 *p) {
    if ((D_80083170[0] & 0x60) != 0) {
        p[0x4C] = 0xFF;
        p[0x50] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800362A0;
    }
}

void func_80036154(u8 *);

void func_800362A0(u8 *p) {
    if (p[0x50] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80036ED4(u8 *, s32, s32, s16, s16, u32);
void func_80036350(u8 *, u8 *, s32);

void func_800362D0(u8 *p) {
    u8 *s = *(u8 **) (p + 0x74);
    s16 t = *(u16 *) (s + 0x22) - (s[0x26] - 1);

    p[0x4D] = s[0x27];
    func_80036ED4(D_80082538, (s32) D_80082040, (s32) p, 1, t, *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    *(u32 *) (p + 0x68) = (u32) func_80036350;
}

void func_80036434(u8 *, u8 *, s32);

void func_80036350(u8 *p, u8 *q, s32 r)
{
    u8 *b = (u8 *)&D_80083160;
    u32 f = *(u32 *)(b + 0x10);

    if (f & 0x4000)
    {
        func_80053DA8(0x502);
        p[0x4D] = p[0x4D] + 1;
        p[0x4D] = p[0x4D] % *(u8 *)(*(u8 **)(p + 0x74) + 0x26);
    }
    else if (f & 0x1000)
    {
        func_80053DA8(0x502);
        p[0x4D] = p[0x4D] + *(u8 *)(*(u8 **)(p + 0x74) + 0x26) - 1;
        p[0x4D] = p[0x4D] % *(u8 *)(*(u8 **)(p + 0x74) + 0x26);
    }
    if (*(u32 *)(b + 0x10) & 0x40)
    {
        func_80036434(p, q, r);
    }
}

void func_80036484(u8 *);

void func_80036434(u8 *p, u8 *q, s32 r) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

void func_800364BC(u8 *);

void func_80036484(u8 *p) {
    u16 *c = (u16 *) (p + 0x64);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        p[0x4D] = 0xFF;
        p[0x51] = 0;
        *(u32 *) (p + 0x68) = (u32) func_800364BC;
    }
}

void func_80036154(u8 *);

void func_800364BC(u8 *p) {
    if (p[0x51] != 0) {
        (*(u8 **) (p + 0x74))[0x84] = 1;
        *(u32 *) (p + 0x68) = (u32) func_80036154;
    }
}

void func_80037030(u8 *, s32, s32, s16, s16, u32);
void func_80036574(u8 *);

void func_800364EC(u8 *p) {
    u8 *s = *(u8 **) (p + 0x74);
    s16 t = *(u16 *) (s + 0x22) - (s[0x26] - 1);

    p[0x4D] = s[0x27];
    func_80037030(D_80082538, (s32) D_80082040, (s32) p, 1, t,
                  *(u32 *) (*(u8 **) (p + 0x74) + 0x80));
    *(u16 *) (p + 0x64) = 0x5A;
    *(u32 *) (p + 0x68) = (u32) func_80036574;
}

void func_8003661C(u8 *);
void func_800364BC(u8 *);

/* The expired arm is written FIRST, so the countdown test branches INTO the
   still-running block as the target's `bgtz` does (41 DIFF the other way
   round), and D_80083160 goes through a one-set POINTER LOCAL -- the struct
   field read directly is the two-insn symbolic form where the target
   materialises the base and loads `0x10($v1)`. */
void func_80036574(u8 *p) {
    Unk80083160 *g = &D_80083160;
    s16 n;
    u32 f;
    u8 *e;

    n = *(u16 *) (p + 0x64) - 1;
    *(s16 *) (p + 0x64) = n;
    if (n <= 0) {
        e = *(u8 **) (p + 0x74);
        p[0x4D] = 0xFF;
        p[0x51] = 0;
        e[0x27] = e[0x26];
        *(u32 *) (p + 0x68) = (u32) func_800364BC;
    } else {
        f = g->unk_10;
        if ((f & 0x20) != 0) {
            p[0x4D] = *(u8 *) (*(u8 **) (p + 0x74) + 0x26) - 1;
        } else if ((f & 0x40) != 0) {
            p[0x4D] = 0;
        } else {
            return;
        }
        func_8003661C(p);
    }
}

void func_80036484(u8 *);

void func_8003661C(u8 *p) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003666C);

void func_80036880(u8 *, s32, s32);

/* The first test's two bits are written through TWO DIFFERENT SPELLINGS of the
   same lvalue: `*(u32 *) (g + 0x10)` for 0x2000 and `D_80083160.unk_10` for
   0x8000. With both written the same way `fold` merges
   `(x & 0x2000) || (x & 0x8000)` into one `andi 0xA000` and the function is
   three words short; with two spellings fold cannot see one expression, so the
   two `andi`s and the two branches survive -- and cse still unifies the loads,
   which is why the listing has only one `lw 0x10($s2)` feeding both.

   The flag word is re-read before the 0x4000 test and again before the 0x40
   test, and that comes for free: both labels have more than one predecessor,
   so cse's extended basic block ends there. Reading it into a local would give
   one load and lose the two re-reads. */
void func_800366F4(u8 *p, s32 b, s32 c)
{
    u8 *g = (u8 *)&D_80083160;
    s32 s;
    s32 n;

    s = p[0x4D];
    if ((*(u32 *)(g + 0x10) & 0x2000) != 0 || (D_80083160.unk_10 & 0x8000) != 0)
    {
        if ((s & 1) != 0)
        {
            s = s - 1;
        }
        else if (s + 1 < (*(u8 **)(p + 0x74))[0x26])
        {
            s = s + 1;
        }
    }
    if ((*(u32 *)(g + 0x10) & 0x4000) != 0)
    {
        if (s + 2 < (*(u8 **)(p + 0x74))[0x26])
        {
            s = s + 2;
        }
        else
        {
            s = s % 2;
        }
    }
    else if ((*(u32 *)(g + 0x10) & 0x1000) != 0)
    {
        if (s - 2 >= 0)
        {
            s = s - 2;
        }
        else
        {
            n = (*(u8 **)(p + 0x74))[0x26];
            if ((n & 1) == 0)
            {
                s = s % 2 + n - 2;
            }
            else if ((s & 1) != 0)
            {
                s = n - 2;
            }
            else
            {
                s = n - 1;
            }
        }
    }
    if (p[0x4D] != s)
    {
        func_80053DA8(0x502);
        p[0x4D] = s;
    }
    if ((*(u32 *)(g + 0x10) & 0x40) != 0)
    {
        func_80036880(p, b, c);
    }
}

void func_80036484(u8 *);

void func_80036880(u8 *p, s32 b, s32 c) {
    u8 *q = *(u8 **) (p + 0x74);

    q[0x27] = p[0x4D];
    *(u16 *) (p + 0x64) = 0x11;
    *(u32 *) (p + 0x68) = (u32) func_80036484;
    p[0x4D] |= 0x80;
    func_80053DA8(0x503);
}

void func_800368F8(void);

void func_800368D0(u8 *p)
{
    u8 *t;

    t = *(u8 **)(p + 0x44);
    (t + *(s16 *)(p + 0x4A))[0x50] = 0xFF;
    *(u32 *)(p + 0x68) = (u32)func_800368F8;
}

void func_800368F8(void) {
}

void func_80036980(void);

void func_80036900(u8 *p, u8 *q, s32 r)
{
    u8 *t;

    *(u16 *)(*(u8 **)p + 0xA) = *(u16 *)(*(u8 **)p + 0xA) + 0x100;
    if (*(s16 *)(*(u8 **)p + 0xA) >= 0x401)
    {
        func_80044A50((UnkNode *)(p - 0x20));
        *(u32 *)(p + 0x68) = (u32)func_80036980;
        t = *(u8 **)(p + 0x44);
        (t + *(s16 *)(p + 0x4A))[0x50] = 1;
    }
}

void func_80036980(void) {
}

void func_80036988(u8 *d, u8 *s) {
    *(u16 *) (d + 0x8) = *(u16 *) (s + 0x0) + *(u16 *) (s + 0x8);
    *(u16 *) (d + 0xA) = *(u16 *) (s + 0x2) + *(u16 *) (s + 0xA);
    *(u16 *) (d + 0xC) = *(u16 *) (s + 0xC);
    *(u16 *) (d + 0xE) = *(u16 *) (s + 0xE);
}

void func_80067F20(u32 *, s32, s32, s32, s32);

void func_800369C8(s32 n) {
    u8 *b = D_80083160.unk_00;
    u32 *p = *(u32 **) (b + 0x8D0);
    s16 t[4];
    u16 tp;
    s32 o;

    *(u32 **) (b + 0x8D0) = p + 3;
    t[0] = 0;
    t[1] = 0;
    t[2] = 0x40;
    t[3] = 0x20;
    tp = GetTPage(0, 0, 0x1C0, 0x100);
    func_80067F20(p, 0, 0, tp, (s32) t);
    o = n * 4 + 0x70;
    AddPrim(D_80083160.unk_00 + o, p);
}

void func_80067F20(u32 *, s32, s32, s32, s32);

/* The 0x70 binds to the term the source groups it with: as
   `D_80083160.unk_00 + n * 4 + 0x70` it folds onto the loaded base and costs
   4 words, so the whole index lives in its own local. */
void func_80036A70(s32 n) {
    u8 *b = D_80083160.unk_00;
    u32 *p = *(u32 **) (b + 0x8D0);
    s16 t[4];
    u16 tp;
    s32 o;

    *(u32 **) (b + 0x8D0) = p + 3;
    t[0] = 0;
    t[1] = 0;
    t[2] = 0;
    t[3] = 0;
    tp = GetTPage(0, 0, 0x1C0, 0x100);
    func_80067F20(p, 0, 0, tp, (s32) t);
    o = n * 4 + 0x70;
    AddPrim(D_80083160.unk_00 + o, p);
}

extern UnkNode *D_80082BC0[];
extern s16 D_8006A934[];
extern u8 D_80073DC4[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_8003DB94(u8 *, u32, s32);
void func_80033D54(void);
void func_80036C44(s32, s32, s32);

/* THE STACK-PARAMETER PROLOGUE INTERLEAVE, and it was the SPELLING of
   D_80082BC0. func_8003FF2C's fourth argument is the list head's address; as
   `&D_80082BC0` off a scalar `extern UnkNode *` the `lui`/`addiu` pair lands
   AFTER both parameter save/load pairs, and as an incomplete array passed by
   name it lands between them, which is what the listing has. The whole
   function is one basic block, so every entry insn has priority 1 and
   schedule_select is choosing on potential_hazard: the array form gives the
   pair a different DEFINE_FUNCTION_UNIT shape and the memory operations stop
   outranking the argument setup. Three and six words in the two siblings, six
   here, and it also moved `addiu $a2, 0x49` ahead of the first save. */
void func_80036B10(u8 *a, u8 *b, u8 *c, s32 d, u32 e)
{
    u8 *n;
    u8 *m;
    u8 *q;

    n = (u8 *)func_8003FF2C(0x11, (UnkNode *)a, 0x49, D_80082BC0);
    *(u32 *)(n + 0x10) = (u32)func_80036C44;
    func_8004491C((UnkNode *)n, (u32)func_80033D54);
    m = n + 0x20;
    q = *(u8 **)(n + 0xC);
    *(u32 *)(m + 0x44) = (u32)c;
    m[0x48] = 1;
    *(s16 *)(m + 0x4A) = d;
    func_80033C1C(b, *(u32 *)c);
    *(u32 *)(n + 0x20) = (u32)b;
    *(u32 *)(b + 0x60) = 6;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x10) = 0x10C;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x14) =
        D_8006A934[*(u8 *)(*(u8 **)(*(u8 **)(c + 0x74) + 0x80) + 2)];
    *(u32 *)(m + 0x4) = 0;
    *(u32 *)(m + 0x8) = 0;
    func_800350B0((u8 *)e, m);
    *(u32 *)(q + 0xC) = 0x808080;
    func_8003DB94(q, (u32)D_80073DC4, 0);
}

s32 func_80037534(u8 *, s32, s32);
void func_800478B8(s32);

void func_80036C44(s32 a, s32 b, s32 c) {
    if (func_80037534((u8 *) a, b, c) == 0) {
        func_800478B8(c);
    }
}

void func_80036C7C(u8 *p, u32 *s, u8 *d) {
    *(u32 *) (d + 0) = s[0];
    *(u32 *) (d + 4) = s[1];
    *(u32 *) (d + 8) = s[2];
    *(s16 *) (d + 4) = GetTPage(0, 1, *(s16 *) (p + 8), *(s16 *) (p + 0xA));
    d[8] = d[8] + (p[0x18] + (s16) (*(s16 *) (p + 8) % 64) * 4);
    d[9] = d[9] + (*(s16 *) (p + 0xA) % 256 + p[0x1A]);
}

extern UnkNode *D_80082BC0[];
extern u32 D_8006A958[];
extern u32 D_8006A964[];
extern u32 D_8006A970[];
extern u8 D_80082B50[];
extern u8 D_80082B60[];
extern u8 D_80082B70[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_80036C7C(u8 *, u32 *, u8 *);
void func_80033D54(void);
void func_80036F24(u8 *, u32, u32);

/* D_80082BC0 as an incomplete array, not `&` on a scalar -- see
   func_80036B10. Three of the six words here. */
void func_80036D4C(u8 *p, s32 b, s32 c, s32 d, s32 e, u32 f)
{
    u8 *n;
    u8 *m;
    u8 *q;

    n = (u8 *)func_8003FF2C(0x11, (UnkNode *)p, 0x49, D_80082BC0);
    *(u32 *)(n + 0x10) = (u32)func_80036F24;
    func_8004491C((UnkNode *)n, (u32)func_80033D54);
    m = n + 0x20;
    q = *(u8 **)(n + 0xC);
    *(u32 *)(m + 0x44) = c;
    m[0x48] = 1;
    *(s16 *)(m + 0x4A) = d;
    func_80033C1C((u8 *)b, *(u32 *)c);
    *(u32 *)(n + 0x20) = b;
    *(u32 *)(b + 0x60) = 6;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x10) = 0x47;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x14) = 2;
    *(u32 *)(m + 0x4) = 0;
    *(u32 *)(m + 0x8) = 0;
    func_800350B0((u8 *)f, m);
    *(u32 *)(q + 0xC) = 0xC0C0C0;
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A970, D_80082B70);
    *(u32 *)(q + 0x8) = (u32)D_80082B60;
    *(u32 *)(q + 0x0) = 0;
    q[4] = 0;
    q[5] = 0;
    *(s16 *)(q + 0x14) = *(u16 *)(q + 0x14) | 0x1C;
    *(s16 *)(m + 0x66) = e;
}

void func_80036D4C(u8 *, s32, s32, s32, s32, u32);
void func_80036F24(u8 *, u32, u32);

void func_80036ED4(u8 *p, s32 b, s32 c, s16 d, s16 e, u32 f) {
    func_80036D4C(p, b, c, d, e, f);
    func_80036F24(p + 0x20, *(u32 *) (p + 0x8), *(u32 *) (p + 0xC));
}

extern u32 D_8006A958[];
extern u32 D_8006A964[];
extern u32 D_8006A970[];
extern u32 D_8006A97C[];
extern u8 D_80082B50[];
extern u8 D_80082B60[];
extern u8 D_80082B70[];

/* The counter at +0x64 is VOLATILE. The target increments it and then LOADS
   IT AGAIN to test bit 0 -- `lhu`, `nop`, `addiu`, `sh`, `lhu`, `nop`,
   `andi 1` -- and nothing about how the increment or the test is spelled stops
   cse forwarding the stored value instead: a `u16 *` local, `(*p)++`, reading
   it back as `s16`, and the increment through a `u16` temporary were all two
   words short, and MEM_IN_STRUCT_P is refuted as the cause (matching-levers
   section 10). A volatile load is never forwarded and never deleted, which is
   exactly the behaviour the listing has, so the field was volatile in the
   original struct. Only the RE-READ needs the qualifier; marking both is
   equivalent.

   The base at +0x44 goes into its own local so its `lw` is emitted before the
   `lh` of +0x4A -- sched's tie-break for equal-priority insns is INSN_LUID,
   i.e. RTL order. The index must NOT also be a local: as a local it is a
   second quantity whose live range is shorter, so local-alloc gives it $v0 and
   the pointer $v1, where the target has them the other way round, and the
   `addu`'s operands come out swapped with them. Left inside the subscript it
   is one operand of the pointer's own address expression. */
void func_80036F24(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A970, D_80082B70);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) == 0)
        {
            *(u32 *)(r + 0xC) = 0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0xC0C0C0;
    }
    *(u32 *)(r + 0x8) = D_8006A97C[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

void func_80036D4C(u8 *, s32, s32, s32, s32, u32);
void func_80037090(u8 *, u32, u32);

/* func_80036ED4's shape plus the two stores, and both argument loads read into
   locals BEFORE the store through `q`. Left in the call, they are evaluated
   after it, and `sw $zero, 0xC($a2)` has a base sched cannot disambiguate from
   `$s0`, so the `lw $a1, 0x8($s0)` can never move back above it: 7 DIFF of
   pure order. The pointer itself has to be a local too, or the argument load
   and the store's load are two separate `lw`s and the function is a word long
   (14 DIFF). */
void func_80037030(u8 *p, s32 b, s32 c, s16 d, s16 e, u32 f) {
    u32 *q;
    u32 v;

    func_80036D4C(p, b, c, d, e, f);
    q = *(u32 **) (p + 0xC);
    v = *(u32 *) (p + 0x8);
    *(u32 *) (p + 0x10) = (u32) func_80037090;
    q[3] = 0;
    func_80037090(p + 0x20, v, (u32) q);
}

/* func_80036F24 with the two arms of the inner test the other way round: the
   zero store is the shared block both branches reach, so it is the `else` of
   the inner test AND the `else` of the outer one. See func_80036F24 above for
   the volatile counter and the pointer local. */
void func_80037090(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A958, D_80082B50);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A964, D_80082B60);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A970, D_80082B70);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) != 0)
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0;
    }
    *(u32 *)(r + 0x8) = D_8006A97C[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

extern UnkNode *D_80082BC0[];
extern u32 D_8006A988[];
extern u32 D_8006A994[];
extern u32 D_8006A9A0[];
extern u32 D_8006A9AC[];
extern u32 D_8006A9B8[];
extern u32 D_8006A9C4[];
extern u8 D_80082B80[];
extern u8 D_80082B90[];
extern u8 D_80082BA0[];
extern u8 D_80082BB0[];
extern u8 D_80081E90[];
extern u8 D_80081EA0[];

UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
void func_80033C1C(u8 *, u32);
void func_800350B0(u8 *, u8 *);
void func_80033D54(void);
void func_80036C7C(u8 *, u32 *, u8 *);
void func_80037394(u8 *, u32, u32);

/* `t = D_80082B90;` goes AFTER the first func_80036C7C call: it is the second
   call's third argument and the value stored to `q + 8` at the end, so it
   lives in $s0 across all six, and a local above the first call puts its
   lui/addiu one call too early -- nine words. D_80082BC0 is an incomplete
   array, not `&` on a scalar; that is the last six -- see func_80036B10. */
void func_8003719C(u8 *a, u8 *b, u8 *c, s16 d, s32 e, u32 f)
{
    u8 *n;
    u8 *m;
    u8 *q;
    u8 *v;
    u8 *t;

    n = (u8 *)func_8003FF2C(0x11, (UnkNode *)a, 0x49, D_80082BC0);
    *(u32 *)(n + 0x10) = (u32)func_80037394;
    func_8004491C((UnkNode *)n, (u32)func_80033D54);
    m = n + 0x20;
    v = *(u8 **)(n + 0x8);
    q = *(u8 **)(n + 0xC);
    *(u32 *)(m + 0x44) = (u32)c;
    m[0x48] = 1;
    *(s16 *)(m + 0x4A) = d;
    func_80033C1C(b, *(u32 *)c);
    *(u32 *)(n + 0x20) = (u32)b;
    *(u32 *)(b + 0x60) = 6;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x10) = 0x47;
    *(u32 *)(*(u8 **)(n + 0x20) + 0x14) = 2;
    *(u32 *)(m + 0x4) = 0;
    *(u32 *)(m + 0x8) = 0;
    func_800350B0((u8 *)f, m);
    *(u32 *)(q + 0xC) = 0xC0C0C0;
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A988, D_80082B80);
    t = D_80082B90;
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A994, t);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9A0, D_80082BA0);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9AC, D_80082BB0);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9B8, D_80081E90);
    func_80036C7C(*(u8 **)(m + 0x44) + 0xC, D_8006A9C4, D_80081EA0);
    *(u32 *)(q + 0x8) = (u32)t;
    *(u32 *)(q + 0x0) = 0;
    q[0x4] = 0;
    q[0x5] = 0;
    *(u16 *)(q + 0x14) |= 0x1C;
    *(s16 *)(m + 0x66) = e;
    func_80037394(m, (u32)v, (u32)q);
}

extern u32 D_8006A988[];
extern u32 D_8006A994[];
extern u32 D_8006A9A0[];
extern u32 D_8006A9AC[];
extern u32 D_8006A9B8[];
extern u32 D_8006A9C4[];
extern u32 D_8006A9D0[];
extern u8 D_80082B80[];
extern u8 D_80082B90[];
extern u8 D_80082BA0[];
extern u8 D_80082BB0[];
extern u8 D_80081E90[];
extern u8 D_80081EA0[];

/* func_80036F24's body with six of the copies instead of three. */
void func_80037394(u8 *p, u32 b, u32 r)
{
    u8 n;
    u8 *t;

    if (func_80037534(p, b, r) != 0)
    {
        return;
    }
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A988, D_80082B80);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A994, D_80082B90);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9A0, D_80082BA0);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9AC, D_80082BB0);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9B8, D_80081E90);
    func_80036C7C(*(u8 **)(p + 0x44) + 0xC, D_8006A9C4, D_80081EA0);
    t = *(u8 **)(p + 0x44);
    n = (t + *(s16 *)(p + 0x4A))[0x4C];
    if ((n & 0x80) != 0)
    {
        *(volatile u16 *)(p + 0x64) = *(u16 *)(p + 0x64) + 1;
        if ((*(volatile u16 *)(p + 0x64) & 1) == 0)
        {
            *(u32 *)(r + 0xC) = 0;
        }
        else
        {
            *(u32 *)(r + 0xC) = 0xC0C0C0;
        }
    }
    else
    {
        *(u32 *)(r + 0xC) = 0xC0C0C0;
    }
    *(u32 *)(r + 0x8) = D_8006A9D0[(n & 0x7F) + *(s16 *)(p + 0x66)];
}

u32 func_800374F4(u16 n) {
    /* The product must be unsigned -- `(u32)` -- or the high half comes out
       with `sra` where the target has `srl`. */
    return (u32) (n * (u16) (rand() << 1)) >> 16;
}

void func_80033C84(u32 *);
void func_80035090(u32 *);
void func_8003FFF0(u8 *);

/* Two of the three parameters are unused -- func_80036C44 passes all three and
   they cost nothing either way -- and the base goes into its own local: as one expression the `lh 0x4A` is
   emitted before the `lw 0x44` whichever way round the operands are (3 DIFF). */
s32 func_80037534(u8 *p, s32 b2, s32 c2) {
    u8 *b;
    u8 *q;
    u8 *d;

    if (p[0x48] == 0) {
        return 0;
    }
    b = *(u8 **) (p + 0x44);
    q = b + *(s16 *) (p + 0x4A);
    if (q[0x4C] != 0xFF) {
        return 0;
    }
    q[0x50] = 1;
    func_80033C84(*(u32 **) p);
    func_80035090((u32 *) (p + 0x54));
    d = p - 0x20;
    func_80044A50((UnkNode *) d);
    func_8003FFF0(d);
    return 1;
}

void func_800375C0(u8 *p, u8 *q)
{
    *(s32 *)(*(u8 **)p + 0X60) = *(s16 *)(q + 0X0);
    *(s32 *)(*(u8 **)p + 0X10) = *(s16 *)(q + 0X4);
    *(s32 *)(*(u8 **)p + 0X14) = *(s16 *)(q + 0X6);
    *(s32 *)(*(u8 **)p + 0X18) = *(s16 *)(q + 0X8);
    *(s32 *)(*(u8 **)p + 0X30) = *(s16 *)(q + 0XC);
    *(s32 *)(*(u8 **)p + 0X34) = *(s16 *)(q + 0XE);
    *(s32 *)(*(u8 **)p + 0X38) = *(s16 *)(q + 0X10);
    *(u16 *)(*(u8 **)p + 0X8) = *(u16 *)(q + 0X14);
    *(u16 *)(*(u8 **)p + 0XA) = *(u16 *)(q + 0X16);
    *(u16 *)(*(u8 **)p + 0XC) = *(u16 *)(q + 0X18);
    *(u32 *)(p + 0XC) = *(u32 *)(q + 0X1C);
    *(u32 *)(p + 0X10) = *(u32 *)(q + 0X20);
    *(u32 *)(p + 0X14) = *(u32 *)(q + 0X24);
    *(u32 *)(p + 0X18) = *(u32 *)(q + 0X28);
    *(u32 *)(p + 0X1C) = *(u32 *)(q + 0X2C);
    *(u16 *)(p + 0X20) = *(u16 *)(q + 0X30);
    *(u16 *)(p + 0X22) = *(u16 *)(q + 0X32);
    *(u16 *)(p + 0x24) = *(u16 *)(p + 0x26) = 0;
    *(u32 *)(p + 0X28) = *(u32 *)(q + 0X38);
    *(u32 *)(p + 0X2C) = *(u32 *)(q + 0X3C);
    *(u32 *)(p + 0X30) = *(u32 *)(q + 0X40);
    *(u32 *)(p + 0X34) = *(u32 *)(q + 0X44);
    *(u32 *)(p + 0X38) = *(u32 *)(q + 0X48);
    *(u16 *)(p + 0X3C) = *(u16 *)(q + 0X4C);
    *(u16 *)(p + 0X3E) = *(u16 *)(q + 0X4E);
    *(u16 *)(p + 0x40) = *(u16 *)(p + 0x42) = 0;
}

void func_80067F20(u32 *, s32, s32, s32, s32);

/* The two-quad actor draw: projects the +0x0C rect's four corners with
   RotTransPers4 and, if any corner lands on screen, builds a textured
   POLY_FT4 for it; then does the same for the +0x28 rect, which gets an
   untextured POLY_G4 plus a tpage primitive instead.

   Nine shapes, 355 DIFF down to a match. Five are matching-levers entries now
   (section 8); the four specific to this function:

   * The four screen-coord slots RotTransPers4 writes are 8 BYTES apart
     (sp+0x48, 0x50, 0x58, 0x60), so whatever the original declared there was
     not `long sxy[4]`. `u8 s[0x20]` with the four slots cast reproduces the
     frame, the `lw` for the vertex copies and the `lhu` pairs for the bound
     tests exactly.

   * `a = p + 0xC` is ONE pointer. The listing reads the first field through
     the parameter (`0xC($t0)`) and the rest through `$s1` because cse
     substitutes a MEM address that is a bare register with its equivalence
     and cannot do the same for `(plus a 4)`.

   * The ot index is INLINE in each `AddPrim`, not a local -- the opposite of
     func_800369C8 in this unit, because here the add has a hard argument
     register to tie to. 15 words.

   * `g = &D_80083160` as a struct pointer, used in the SECOND half only.
     That is the listing's `addiu $s5, $s3, %lo(D_80083160)`: `$s3` keeps the
     `high` for the first half's two `%lo(...)($s3)` loads and `$s5` the whole
     address for the second half's four `0($s5)` loads. Without it the
     function is five words short.

   And the last one, which was this function's park for a while: the u and v
   running values are TWO locals, `t` and `w`. One shared `t` is 7 words out,
   because the two chains are in different basic blocks and the listing gives
   them different registers ($v1 and $v0) -- which one pseudo cannot have.
   advise.py called it ("assign back to an existing local instead of
   introducing a fresh one" is the same lever read from the other side). */
void func_80037714(u8 *p)
{
    Unk80083160 *g;
    u8 *a;
    u8 *b;
    u8 *q;
    u8 *e;
    SVECTOR v[4];
    u8 s[0x20];
    s32 pz;
    s32 flag;
    s32 t;
    s32 u;
    s32 w;
    s32 y;

    g = &D_80083160;
    q = *(u8 **) p;
    a = p + 0xC;
    b = p + 0x28;

    v[0].vx = v[2].vx = *(u16 *) a;
    v[1].vx = v[3].vx = *(u16 *) a + *(u16 *) (a + 4);
    v[0].vy = v[1].vy = *(u16 *) (a + 2);
    v[2].vy = v[3].vy = *(u16 *) (a + 2) + *(u16 *) (a + 6);
    v[0].vz = v[1].vz = v[2].vz = v[3].vz = 0;
    RotTransPers4(&v[0], &v[1], &v[2], &v[3], (s32 *) (s + 0x00), (s32 *) (s + 0x08),
                  (s32 *) (s + 0x10), (s32 *) (s + 0x18), &pz, &flag);
    if (((u16) (*(u16 *) (s + 0x00) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x02) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x08) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x0A) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x10) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x12) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x18) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x1A) + 0x20) < 0x121))
    {
        e = *(u8 **) (D_80083160.unk_00 + 0x8D0);
        *(u8 **) (D_80083160.unk_00 + 0x8D0) = e + 0x28;
        SetPolyFT4(e);
        *(s16 *) (e + 0xE) = GetClut(*(s16 *) (a + 0x14), *(s16 *) (a + 0x16));
        *(s16 *) (e + 0x16) = GetTPage(0, 0, *(s16 *) (a + 0x8), *(s16 *) (a + 0xA));
        e[4] = e[5] = e[6] = a[0x10];
        u = *(s16 *) (a + 0x8) % 64 * 4;
        t = a[0x18] + u;
        e[0x1C] = t;
        e[0xC] = t;
        t = t + a[0xC];
        e[0x24] = t;
        e[0x14] = t;
        y = *(s16 *) (a + 0xA);
        w = y % 256 + a[0x1A];
        e[0x15] = w;
        e[0xD] = w;
        w = w + a[0xE];
        e[0x25] = w;
        e[0x1D] = w;
        *(s32 *) (e + 0x08) = *(s32 *) (s + 0x00);
        *(s32 *) (e + 0x10) = *(s32 *) (s + 0x08);
        *(s32 *) (e + 0x18) = *(s32 *) (s + 0x10);
        *(s32 *) (e + 0x20) = *(s32 *) (s + 0x18);
        AddPrim(D_80083160.unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);
    }

    v[0].vx = v[2].vx = *(u16 *) b;
    v[1].vx = v[3].vx = *(u16 *) b + *(u16 *) (b + 4);
    v[0].vy = v[1].vy = *(u16 *) (b + 2);
    v[2].vy = v[3].vy = *(u16 *) (b + 2) + *(u16 *) (b + 6);
    v[0].vz = v[1].vz = v[2].vz = v[3].vz = 0;
    RotTransPers4(&v[0], &v[1], &v[2], &v[3], (s32 *) (s + 0x00), (s32 *) (s + 0x08),
                  (s32 *) (s + 0x10), (s32 *) (s + 0x18), &pz, &flag);
    if (((u16) (*(u16 *) (s + 0x00) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x02) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x08) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x0A) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x10) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x12) + 0x20) < 0x121)
        | ((u16) (*(u16 *) (s + 0x18) + 0x20) < 0x181 && (u16) (*(u16 *) (s + 0x1A) + 0x20) < 0x121))
    {
        e = *(u8 **) (g->unk_00 + 0x8D0);
        *(u8 **) (g->unk_00 + 0x8D0) = e + 0x24;
        SetPolyG4(e);
        SetSemiTrans(e, 1);
        e[4] = e[5] = e[6] = e[0xC] = e[0xD] = e[0x14] = e[0x15] = e[0x16] = e[0x1C] = e[0x1D] = 0;
        e[0xE] = e[0x1E] = 0x40;
        *(s32 *) (e + 0x08) = *(s32 *) (s + 0x00);
        *(s32 *) (e + 0x10) = *(s32 *) (s + 0x08);
        *(s32 *) (e + 0x18) = *(s32 *) (s + 0x10);
        *(s32 *) (e + 0x20) = *(s32 *) (s + 0x18);
        AddPrim(g->unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);

        e = *(u8 **) (g->unk_00 + 0x8D0);
        *(u8 **) (g->unk_00 + 0x8D0) = e + 0xC;
        func_80067F20((u32 *) e, 0, 0, GetTPage(0, 0, 0x140, 0), 0);
        AddPrim(g->unk_00 + (*(s32 *) (q + 0x60) * 4 + 0x70), e);
    }
}

void func_80037C7C(u8 *p, s16 n)
{
    s16 m;
    s16 t;

    *(s32 *)(*(u8 **)p + 0x10) = n;
    if (n >= 0x37)
    {
        *(s16 *)(p + 0xC) = -0x38;
        *(s16 *)(p + 0x10) = 0x30;
    }
    else
    {
        *(s16 *)(p + 0xC) = -n - 2;
        *(s16 *)(p + 0x10) = n - 6;
        if (*(s16 *)(p + 0x10) <= 0)
        {
            *(s16 *)(p + 0x10) = 1;
        }
    }
    if (n < 0x36)
    {
        s16 k = -n;

        *(s16 *)(p + 0x24) = k + 0x36;
    }
    else
    {
        *(s16 *)(p + 0x24) = 0;
    }
    m = -n;
    *(s16 *)(p + 0x28) = m - 3;
    t = n + 6;
    *(s16 *)(p + 0x2C) = t;
    *(s16 *)(p + 0x18) = *(u16 *)(p + 0x10);
    if (t <= 0)
    {
        *(s16 *)(p + 0x2C) = 1;
    }
    if (n < 0x40)
    {
        *(s16 *)(p + 0x40) = m + 0x40;
    }
    else
    {
        *(s16 *)(p + 0x40) = 0;
    }
    *(s16 *)(p + 0x34) = *(u16 *)(p + 0x2C);
}

/* The second parameter is unused here; func_80034EF8 masks a byte into it, so
   it is real. An unused parameter costs no instruction. */
void func_80037D50(u8 *p, u8 b) {
    u8 *q = p + 0x8C;

    q[0x25] = D_8006A908[p[2]];
    *(u16 *) (q + 0xE) = D_8006A8B4[q[0x2A]] * q[0x25];
}

extern u8 D_800809C0[];
extern u8 D_8006A86C[];
void func_8003AB14(u16 *, u16 *);
void func_80037D50(u8 *, u8);
void func_80038A10(u8 *, u8 *, s32);

void func_80037D9C(u8 *p, u8 c) {
    u8 *q = p + 0x8C;
    u8 v;

    *(u32 *) (q + 0x80) = (u32) p;
    func_8003AB14((u16 *) q, (u16 *) D_8006A86C);
    q[0x2A] = 1;
    q[0x24] = 0x20;
    *(s16 *) (q + 0xC) = 0xFC;
    *(s16 *) (q + 8) = 0;
    *(s16 *) (q + 0xA) = 0;
    *(u32 *) (q + 0x1C) = (u32) D_800809C0;
    *(s16 *) (q + 0x22) = 0;
    *(s16 *) (q + 0x20) = 0;
    q[0x2E] = 0;
    v = p[3];
    p[4] = v;
    q[0x28] = v;
    *(u32 *) (q + 0x7C) = (u32) (p + 0xC8);
    *(u32 *) (q + 0x10) = (u32) func_80038A10;
    p[2] = c;
    func_80037D50(p, c);
}

/* A scalar ALIAS for each of the two symbols, and `&` on the use: the
   DECLARED SIZE is what decides whether the address is a one-insn `la` (which
   gas expands to a lui/addiu pair in ONE register) or a two-insn
   compiler-split pair, and here both want the one-insn form -- it is the last
   eight words of this function. The unit's own `extern u8 D_800809C0[];`
   above is left as it is, because func_80037D9C is matched against the array
   spelling. */
extern u8 D_800809C0s __asm__("D_800809C0");
extern u8 D_80012D6E;

UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_80037F70(u8 **);

/* Spawns the node with func_8003FE78, hangs func_80037F70 on it, points its
   +0x20/+0x24 pair at D_80082A38 and D_80082A38+0x8C, zeroes 0x6B eight-byte
   D_80082660 entries, clears sixteen words of D_80082A38 from the top down,
   plants D_800809C0 and a zero, and copies a byte out of D_80012D6E.

   Four shapes, and the two DECLARATIONS above are half of them:

   * The DESCENDING loop keeps 0x48 as the store's DISPLACEMENT, with the giv
     based at D_80082A38 + 0x3C. Reaching the words by name
     (`D_80082A38w[i + 0x12] = 0`) lets loop.c fold the 0x48 into the giv's
     add_val, because symbol plus constant IS a constant, and the store comes
     out `sw $zero, 0x0($v0)` off a base of +0x84. Going through a
     loop-invariant REGISTER (`r = D_80082A38w;` then `r[i + 0x12]`) leaves
     the add_val a pseudo, the 0x48 cannot fold into it, and the displacement
     survives.

   * `q = (u8 **) (n + 0x20)` as its own local, with the +0x24 store and every
     later read through it but the +0x20 store spelled `n + 0x20`. That
     asymmetry is what the listing has ($s2 for 0x4, $v0 for 0x20).

   * The three tail reads of q[0] each reload it, because the preceding `sb`
     invalidates cse's memory table -- but the last pair,
     `q[0][0x86] = q[0][0x85]`, shares ONE load, which is only true when both
     subscripts are in the same statement.

   The two declarations were worth six words and then the last two. With both
   as incomplete arrays the park sat at 8 DIFF for a session with the right
   diagnosis and the wrong cure: local-alloc priced lo_sum(D_800809C0) at 2
   refs over 2 insns against lo_sum(D_80082A38)'s 3 over 4, took the shorter
   one first and gave it $v0, and the park asked for "D_800809C0's address one
   insn rather than two" without naming the declaration that does it. Found by
   `search.py --only=global_decl`. */
void func_80037E58(void)
{
    u8 *n;
    u8 **q;
    s32 i;
    u32 *r;

    n = (u8 *) func_8003FE78(0, D_80082BC0, 0x49);
    q = (u8 **) (n + 0x20);
    *(u32 *) (n + 0x10) = (u32) func_80037F70;
    q[1] = (u8 *) D_80082A38w;
    *(u32 *) (n + 0x20) = (u32) ((u8 *) D_80082A38w + 0x8C);
    for (i = 0; i < 0x6B; i++)
    {
        memset(&D_80082660[i], 0, 8);
    }
    r = D_80082A38w;
    for (i = 0xF; i >= 0; i--)
    {
        r[i + 0x12] = 0;
    }
    D_80082A38w[0x22] = (u32) &D_800809C0s;
    D_80082A38w[0xE] = 0;
    q[1][3] = D_80012D6E;
    func_80037D9C(q[1], 3);
    q[0][0x87] = 0;
    q[0][0x85] = 0;
    q[0][0x86] = q[0][0x85];
}

s32 func_80043FB8(s32 arg0, ...);
void func_8003800C(u8 *);

void func_80037F70(u8 **p) {
    u8 *b = (u8 *) D_80082A38;

    func_80043FB8((s32) D_8002D000, (s32) b);
    func_80043FB8((s32) D_8002D010, (s32) (b + 0xA8));
    func_80043FB8((s32) D_8002D020, (s32) (b + 0x9C));
    func_8003800C(*p);
    (*p)[0x86] = (*p)[0x85];
    (*p)[0x85] = 0;
}

void func_80038000(u8 v) {
    D_80082B4B[0] = v;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003800C);

void func_80038A10(u8 *, u8 *, s32);

/* The struct through a pointer local declared at function scope, so its
   lui/addiu pair lands in the entry block (the `addiu` ends up in the first
   guard's delay slot) and the field load keeps a plain +8 displacement.
   Written as `D_80083160.unk_08` the pair is emitted inside the second block
   and cse folds the %lo into the load, which is a word short. */
void func_80038128(u8 *p) {
    Unk80083160 *s = &D_80083160;

    if (*(s8 *) (p + 0x87) == 0 && (s->unk_08 & 0x20) != 0) {
        if (p[0x2E] == 0) {
            p[0x2E] = 1;
        }
        *(u16 *) (p + 0x18) = 0;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    } else {
        u16 *c = (u16 *) (p + 0x18);

        *c = *c - 1;
        if ((s16) *c <= 0) {
            *(u32 *) (p + 0x10) = (u32) func_80038A10;
        }
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_800381A4(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

s32 func_80038240(s32);
void func_80038288(u8 *);
void func_800382F0(u8 *);
void func_8003832C(u8 *);

u32 func_800381D0(u8 *p) {
    /* The func_80038288 arm is written LAST because the target places it last,
       reached by `beq`; as an early `if (... == 0x100) return ...;` it comes
       out first and everything after it shifts (18 DIFF). */
    if (func_80053EF0(4) != 0x100) {
        if (func_80038240((*(u8 **) (p + 0x1C))[0]) != 0) {
            return (u32) func_800382F0;
        }
        return (u32) func_8003832C;
    }
    return (u32) func_80038288;
}

s32 func_80038240(s32 a) {
    if (a == 0) {
        return 0;
    }
    if (a == 1) {
        func_80053DA8(0xB4);
        return 0;
    }
    func_80053DA8(0x74);
    return 1;
}

s32 func_80038240(s32);
void func_800382F0(u8 *);
void func_8003832C(u8 *);

void func_80038288(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        if (func_80038240((*(u8 **) (p + 0x1C))[0]) != 0) {
            *(u32 *) (p + 0x10) = (u32) func_800382F0;
        } else {
            *(u32 *) (p + 0x10) = (u32) func_8003832C;
        }
    }
}

void func_8003832C(u8 *);

void func_800382F0(u8 *p) {
    if (func_80053EF0(4) == 0) {
        func_8003832C(p);
    }
}

void func_80039AF0(u8 *);
void func_80038368(u8 *);

void func_8003832C(u8 *p) {
    func_80039AF0(p);
    *(u16 *) (p + 0x18) = 5;
    *(u32 *) (p + 0x10) = (u32) func_80038368;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038368(u8 *p) {
    u16 *c = (u16 *) (p + 0x18);

    *c = *c - 1;
    if ((s16) *c <= 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038394(u8 *p) {
    if (func_80033B2C(*(s16 *) (p + 0x30)) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_800383D4(u8 *p) {
    if (D_80082660[p[0x2F]].unk_01 == 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

u8 *func_800392A4(u8);
void func_80038A10(u8 *, u8 *, s32);

void func_80038408(u8 *p) {
    if (func_800392A4(p[0x2F]) != 0) {
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    } else {
        p[0x89] -= 1;
        if ((s8) p[0x89] < 0) {
            *(u32 *) (p + 0x10) = (u32) func_80038A10;
        }
    }
}

void func_80038464(u8 *p) {
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038478;
}

void func_800384A8(u8 *);

void func_80038478(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_800384A8(p);
    }
}

void func_800384F8(u8 *);
void func_80038538(u8 *);

void func_800384A8(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80038538(p);
    } else {
        *(u32 *) (p + 0x10) = (u32) func_800384F8;
    }
}

void func_80038538(u8 *);

void func_800384F8(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80038538(p);
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038538(u8 *p) {
    func_80053DA8(0xB4);
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

void func_80038570(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038588;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038588(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_800385D0(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_800385E8;
}

void func_80038A10(u8 *, u8 *, s32);

void func_800385E8(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038630(u8 *p) {
    p[0x27] = 0;
    p[0x84] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038648;
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038648(u8 *p) {
    if (*(s8 *) (p + 0x84) != 0) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038690(u8 *p) {
    u8 *s = *(u8 **) (p + 0x80);
    u32 r = (*(u32 (**)(u32, u32, u32, u32)) (*(u8 **) (s + 0x40) + *(s8 *) (p + 0x32) * 4))(*(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));

    *(u32 *) (s + 0x84) = r;
    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A10(u8 *, u8 *, s32);

void func_80038708(u8 *p) {
    ((void (*)(void)) D_800DCF54[*(s8 *) (p + 0x32)])();
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

/* D_80110E98 is an ARRAY of pointers, not a pointer to one: the target takes
   its address (`addiu %lo`) where `extern u32 **` loads through it. */
extern u32 *D_80110E98[];
void func_80038A10(u8 *, u8 *, s32);

void func_8003875C(u8 *p) {
    u32 r = (*(u32 (**)(u32)) (D_80110E98[*(s8 *) (p + 0x32)] + *(s8 *) (p + 0x33)))(*(u32 *) (p + 0x34));

    if (r != 0) {
        *(u32 *) (p + 0x34) = r;
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_8003AE28(s16, s16, s16, s16, s32, s32);
void func_8003AE60(s16, s16, s16, s16, u8, u8, u8);

/* The wipe animation's per-frame step: advances the +0x0A counter by 4 and,
   once it has reached D_8006A8B4[+0x2A], draws p[0x25] - 1 bars through
   func_8003AE28 and then one black one through func_8003AE60, and hands
   control back to func_80038A10.

   THREE spellings of the same two fields, and the listing wants all three:
   `*(u16 *) (p + 0)` passed to an `s16` parameter gives `lhu` with the
   narrowing `sll`/`sra` AFTER the sum, `(s16) *(u16 *) (p + 0)` gives the
   `sll`/`sra` on each term BEFORE it, and `*(s16 *) (p + 2)` gives a plain
   `lh`. Which one each argument takes is read straight off the listing.

   The guard is `i < p[0x25] - 1`, not `p[0x25] - 1 > 0`. Both fold to the
   same `blez`, but the dead reference to `i` is the tenth one its allocno
   needs: `floor_log2(10) * 10 / 42` beats the +0x08 temporary's
   `floor_log2(6) * 6 / 17` where nine references do not, and that decides
   whether `i` or the temporary gets $t0. 10 wrong words otherwise, which is
   matching-levers section 3's free-reference trick in its purest form.

   `w` holds the clamped counter and is computed BEFORE `t` is assigned, which
   keeps $v0 and $v1 busy across the table address's `lui` so its high half
   shares $a0 with the address itself -- the listing's
   `lui $a0, %hi` / `addiu $a0, $a0, %lo`. With `t` first the high lands in
   $v0 and the pair is two registers. */
void func_800387D0(u8 *p)
{
    u32 *t;
    s32 i;
    s32 w;

    *(u16 *) (p + 0xA) = *(u16 *) (p + 0xA) + 4;
    w = (s16) *(u16 *) (p + 0xA);
    t = D_8006A8B4;
    if (w < (s32) t[p[0x2A]])
    {
        return;
    }
    i = 0;
    *(u16 *) (p + 0xA) = 0;
    if (i < p[0x25] - 1)
    {
        u32 *u = t;

        do
        {
            func_8003AE28(*(u16 *) (p + 0) + *(u16 *) (p + 8),
                          *(u16 *) (p + 2) + *(u16 *) (p + 0xA) + u[p[0x2A]] * (i + 1) + 2,
                          *(s16 *) (p + 0xC) / 4, (s16) u[p[0x2A]],
                          (s16) *(u16 *) (p + 0) + (s16) *(u16 *) (p + 8),
                          *(s16 *) (p + 2) + *(s16 *) (p + 0xA) + u[p[0x2A]] * i + 2);
            i = i + 1;
        } while (i < p[0x25] - 1);
    }
    func_8003AE60(*(u16 *) (p + 0) + *(u16 *) (p + 8),
                  *(u16 *) (p + 2) + *(u16 *) (p + 0xA) + D_8006A8B4[p[0x2A]] * (p[0x25] - 1) + 2,
                  *(s16 *) (p + 0xC) / 4, (s16) D_8006A8B4[p[0x2A]], 0, 0, 0);
    *(u32 *) (p + 0x10) = (u32) func_80038A10;
}

void func_80038A10(u8 *, u8 *, s32);

void func_800389B4(u8 *p) {
    if (func_80053EF0(4) != 0x100) {
        func_80053DA8(0xB4);
        *(u32 *) (p + 0x10) = (u32) func_80038A10;
    }
}

void func_80038A00(void) {
}

void func_80038A08(void) {
}

extern u32 D_80081448;
s32 func_80038AB8(u8 *, u8 *);

/* $gp group (D_80081448), so this was verified in a scratch file carrying the
   .sdata block at the top of this file. The three guards are `&&`-joined, so
   each branches past the body as the target does, and reorg fills all three
   delay slots with the `lui %hi(func_80038A10)` the loop then compares
   against. */
void func_80038A10(u8 *p, u8 *q, s32 r) {
    Unk80083160 *g = &D_80083160;
    u8 *s;

    D_80081448 = (u32) p;
    s = *(u8 **) (p + 0x80);
    if (*(s8 *) (p + 0x87) == 0 && (g->unk_08 & 0x20) != 0 && p[0x2E] == 0) {
        p[0x2E] = 1;
    }
    do {
        if (func_80038AB8(p, s) == 0) {
            break;
        }
    } while (*(u32 *) (p + 0x10) == (u32) func_80038A10);
}

extern u32 D_8006AA90[];

void func_80038CB8(u8 *);
s32 func_80038C40(s8);
s16 func_8003ADB4(u16);
void func_80039AE8(void);

/* One step of the script VM: when the opcode at +0x1C is positive it runs
   opcodes out of the D_8006AA90 table until one stops being func_80038A10 or
   the stream turns negative, then re-arms through func_800381D0; when it is
   not positive, func_80038CB8 handles it. Either way a +0x85 of zero means
   give up: stop the sound, kick func_8003ADB4(0xB4) and reset. Returns 1 only
   when +0x2E is 2.

   TWO gotos, and both are needed for the word count. `goto bail` from the
   non-positive arm into the body of the positive arm's `f == 0` block, and
   `goto done` to the shared `return 0`. Written out twice instead,
   cross-jumping does NOT merge them and the function is 111 words against 98;
   and with the tail written as its own `if (...) return 1; return 0;` with
   only one predecessor, jump.c rewrites it branchlessly as
   `xori 0x2` / `sltiu 1`, where the listing branches. The `goto done` gives
   the `return 0` a second predecessor and the branches come back.

   The +0x85 read in the positive arm is the dereference itself, NOT an `s8`
   local: assigned to a local whose only use is `== 0`, gcc drops the sign and
   emits `lbu` where the listing has `lb`. One word, and the only one left
   after the gotos.

   `t` and `k` are locals at the top of the positive arm so their lui/addiu
   pairs land in the loop's preheader, which is where the listing has them;
   the table call is `(c * 4 + (u32) t)` index-first so 0x0 stays the load's
   displacement. */
s32 func_80038AB8(u8 *p, u8 *q)
{
    if (*(s8 *) *(u8 **) (p + 0x1C) <= 0)
    {
        if (*(s8 *) (p + 0x85) == 0)
        {
            goto bail;
        }
        func_80038CB8(p);
        if (*(u8 *) (p + 0x2E) == 2)
        {
            if (func_80038C40(*(s8 *) *(u8 **) (p + 0x1C)) != 0)
            {
                return 1;
            }
        }
        *(u8 *) (p + 0x2E) = 0;
        goto done;
    }
    else
    {
        u32 *t = D_8006AA90;
        u32 k = (u32) func_80038A10;

        do
        {
            u8 *pc = *(u8 **) (p + 0x1C);
            u8 c = *pc;

            *(u8 **) (p + 0x1C) = pc + 1;
            (*(void (**)(u8 *)) (c * 4 + (u32) t))(p);
        } while (*(s8 *) *(u8 **) (p + 0x1C) > 0 && *(u32 *) (p + 0x10) == k);
        if (*(u32 *) (p + 0x10) == (u32) func_80039AE8)
        {
            *(u32 *) (p + 0x10) = 0;
            if (*(s8 *) (p + 0x85) == 0)
            {
            bail:
                if (func_80034FD0((u32 *) q) == 0)
                {
                    func_8003ADB4(0xB4);
                    func_80034EF8(q, q[2]);
                    func_80034F58((s32) q);
                }
                return 0;
            }
            *(u8 **) (p + 0x1C) = *(u8 **) (p + 0x1C) + 1;
            *(u32 *) (p + 0x10) = func_800381D0(p);
        }
    }
    if (*(u8 *) (p + 0x2E) == 2)
    {
        return 1;
    }
done:
    return 0;
}

typedef struct Unk8002D03C {
    s8 unk_00[9];
} Unk8002D03C;

/* The 9-byte table is COPIED to the stack -- a byte-aligned struct, so the
   copy is lwl/lwr pairs plus an `lb` -- and the copy comes before the
   counter's initialisation (as a `for`, not `i = 0` ahead of the copy, which
   puts `move $t0, $zero` first and spends a register: 22 DIFF). The `for`
   guard folds away because 0 < 9. */
s32 func_80038C40(s8 k) {
    Unk8002D03C t;
    s32 i;

    t = *(Unk8002D03C *) D_8002D03C;
    for (i = 0; i < 9; i++) {
        if (k == t.unk_00[i]) {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_80038CB8);

extern u32 D_8006A944w[] __asm__("D_8006A944");

u8 *func_8003AD08(u32, u8 *);
void func_80039640(u8 *);

/* The script VM's control-flow dispatcher: while the opcode at +0x1C is 0xFD,
   0xFE, 0xFF or 0x00, it pushes a return address onto the +0x7C stack and
   jumps -- 0xFD through the +0x80 table's +0x48 handler via func_8003AD08,
   0xFE through D_8006A944 indexed by the next byte, 0xFF through a 32-bit
   address built from the next four bytes -- and 0x00 goes to func_80039640.

   The loop condition is `(u8) (op + 3) < 3 || op == 0` with `op` a `u8`
   local (`addiu 3` / `andi 0xFF` / `sltiu 3` / `bnez`, then `beqz`). The
   three arms are an if/else-if CHAIN over -3/-2/-1, not a switch. `t` is the
   +0x80 pointer read BEFORE the loop so 0x48 stays the load's displacement;
   the field is re-read inside the arm and after the call because the call
   invalidates cse. D_8006A944 takes an incomplete-array `__asm__` alias so
   its address is a `lui`/`addiu` pair. `q` is re-read from +0x1C in each arm
   because the store through the +0x7C stack pointer invalidates cse.

   In the 0xFF arm the four-byte sum goes through a `u32` LOCAL before it is
   dereferenced. Written inline as the address, `*(u8 **) (q[1] + ... )`,
   expand_expr sees the sum with EXPAND_SUM and builds one PLUS tree, so all
   four `lbu`s and three `sll`s are emitted first and the three `addu`s last,
   and four bytes are live at once. Through the local the sum is a plain
   binop: load, shift, add, load, shift, add, and the listing's interleave
   with $v1 reused for r[1] and r[3] follows (12 DIFF inline). */
void func_80038F48(u8 *p)
{
    u8 *t;
    u8 op;

    op = **(u8 **)(p + 0x1C);
    t = *(u8 **)(p + 0x80);
    while ((u8)(op + 3) < 3 || op == 0)
    {
        u8 *pc = *(u8 **)(p + 0x1C);

        if (*(s8 *)pc == -3)
        {
            u8 *q;

            **(u8 ***)(p + 0x7C) = pc + 2;
            *(u8 **)(p + 0x7C) = *(u8 **)(p + 0x7C) + 4;
            q = *(u8 **)(p + 0x1C);
            *(u8 **)(p + 0x1C) = q + 1;
            func_8003AD08(*(u32 *)(q[1] * 4 + (u32)t + 0x48), *(u8 **)(p + 0x80) + 0x14);
            *(u8 **)(p + 0x1C) = *(u8 **)(p + 0x80) + 0x14;
        }
        else if (*(s8 *)pc == -2)
        {
            u8 *q;

            **(u8 ***)(p + 0x7C) = pc + 2;
            *(u8 **)(p + 0x7C) = *(u8 **)(p + 0x7C) + 4;
            q = *(u8 **)(p + 0x1C);
            *(u8 **)(p + 0x1C) = q + 1;
            *(u8 **)(p + 0x1C) = *(u8 **)(q[1] * 4 + (u32)D_8006A944w);
        }
        else if (*(s8 *)pc == -1)
        {
            u8 *q;
            u8 *r;
            u32 a;

            **(u8 ***)(p + 0x7C) = pc + 5;
            *(u8 **)(p + 0x7C) = *(u8 **)(p + 0x7C) + 4;
            q = *(u8 **)(p + 0x1C);
            r = q + 1;
            *(u8 **)(p + 0x1C) = r;
            a = q[1] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24);
            *(u8 **)(p + 0x1C) = *(u8 **)a;
        }
        else
        {
            func_80039640(p);
        }
        op = **(u8 **)(p + 0x1C);
    }
}

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");
void func_80038A00(void);
s16 func_80053DA8(u16);

/* `!= 0x100` with the early return in the ELSE: the branch is TAKEN to the
   returning arm, which is therefore placed after the other one. Written
   `== 0x100` with the return in the then-arm, the two blocks swap and the
   whole tail follows them (19 DIFF). */
void func_800390D0(u8 *p) {
    if (D_80082E6Aa[0] != 2) {
        if (func_80053EF0(4) != 0x100) {
            func_80053DA8(0xC4);
        } else {
            *(u32 *) (p + 0x10) = 0;
            *(u32 *) (p + 0x1C) -= 1;
            return;
        }
    }
    *(u32 *) (p + 0x10) = (u32) func_80038A00;
}

void func_800381A4(u8 *);

void func_80039148(u8 *p) {
    *(u16 *) (p + 0x20) = 0;
    if (*(s16 *) (p + 0x22) < p[0x25] - 1) {
        *(u16 *) (p + 0x22) += 1;
        if (p[0x2E] == 1) {
            *(u16 *) (p + 0x18) = 0;
            *(u32 *) (p + 0x10) = (u32) func_800381A4;
        }
    } else {
        *(u32 *) (p + 0x10) = (u32) func_800387D0;
    }
}

void func_8003919C(void) {
}

void func_800391A4(void) {
}

void func_800391AC(void) {
}

void func_800391B4(void) {
}

void func_800391BC(u8 *p) {
    *(u16 *) (p + 0x20) = 0;
    *(u16 *) (p + 0x22) = 0;
    ClearImage((RECT *)p, 0, 0, 0);
}

void func_800391EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x28] = *(*q)++;
}

void func_80039208(u8 *p) {
    p[0x2E] = 2;
}

void func_80039214(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AA8((s16) v);
}

void func_80039258(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);

    *q = c + 2;
    func_80033AE8((s16) v);
}

void func_8003929C(void) {
}

u8 *func_800392A4(u8 i) {
    u8 *q = (u8 *) D_80082660[i].unk_04;
    u8 *r;

    if (q != 0) {
        r = q + 0x20;
        if (i == 1) {
            return r;
        }
        if (*(u32 *) (q + 0x10) == (u32) D_800C3174 || *(u32 *) (q + 0x10) == (u32) D_800C321C) {
            if (*(u32 *) (r + 0x50) == (u32) D_800C3960) {
                return r;
            }
        }
    }
    return 0;
}

/* A script opcode handler, and the same shape as func_80039470 below: it
   reads a byte from the cursor at p+0x1C, indexes D_80082660 by it, and either
   rewinds two bytes and installs func_800383D4 / func_80038408, or fills the
   slot and writes two halfwords into the object func_800392A4 returns -- at
   +0x36/+0x38 when the slot is 1 and the +0x10 field is neither D_800C3174 nor
   D_800C321C, and at +0x88/+0x8A otherwise.

   Three shapes buy the word count: the cursor reads are ONE statement,
   `n = *(*(u8 **)(p + 0x1C))++;` (split, a register copy); both error blocks
   are `else` arms, not guard clauses (as guards the first `bne` spans 7 words
   instead of 51); and the table base goes through a pointer local so its
   `lui`/`addiu` precedes the index.

   The last ten words were the byte's LIVE LENGTH. The opcode byte and the
   +0x80 pointer both cross the call and compete for $s3/$s4 on global-alloc's
   `floor_log2(refs) * refs / live_length`: 4 refs over 23 insns (0.348) lost
   to the pointer's 6 over 32 (0.375). In the `else` arm, computing the
   rewound cursor into `c` BEFORE the `sb` of the byte and storing it after
   lets sched1 emit the `sb` first in that block (the `addiu`/`sw` chain is
   launched and sinks to its store), so the byte is live for 1 insn there
   instead of 3: 4/21 = 0.381 beats 0.375 and the two swap. sched2 then puts
   the `sb` back between the `addiu %lo` and the `sw 0x10`, because a ready
   store beats an ALU op on potential_hazard, so the listing's order is kept.
   Written `*(p+0x1C) -= 2` AFTER the `sb` the QImode store flushes cse and
   the rewind re-loads the cursor (87 words, 80 DIFF); the byte store moved
   first in the OTHER error block takes the live length to 20, which TIES the
   index's 5/25 = 0.400 and loses on allocno number (8 DIFF). */
void func_8003931C(u8 *p)
{
    u8 n;
    s32 k;
    u8 *e;
    u8 *r;
    Unk80082660 *t;

    n = *(*(u8 **)(p + 0x1C))++;
    k = n;
    t = D_80082660;
    e = (u8 *)&t[k];
    r = *(u8 **)(p + 0x80);
    if (*(s8 *)(e + 1) == 0)
    {
        if (k != 0)
        {
            u8 *q = func_800392A4(k);

            if (q != 0)
            {
                u8 *f = q - 0x20;

                e[0] = *(*(u8 **)(p + 0x1C))++;
                e[1] = 1;
                e[2] = r[0x48];
                if (k == 1 && *(u32 **)(f + 0x10) != D_800C3174 && *(u32 **)(f + 0x10) != D_800C321C)
                {
                    *(s16 *)(q + 0x36) = *(u16 *)(r + 0x4C);
                    *(s16 *)(q + 0x38) = *(u16 *)(r + 0x50);
                }
                else
                {
                    *(s16 *)(q + 0x88) = *(u16 *)(r + 0x4C);
                    *(s16 *)(q + 0x8A) = *(u16 *)(r + 0x50);
                }
            }
            else
            {
                *(u32 *)(p + 0x10) = (u32)func_80038408;
                p[0x2F] = n;
                p[0x89] = 0x10;
                *(u8 **)(p + 0x1C) = *(u8 **)(p + 0x1C) - 2;
            }
        }
    }
    else
    {
        u8 *c = *(u8 **)(p + 0x1C) - 2;

        p[0x2F] = n;
        *(u8 **)(p + 0x1C) = c;
        *(u32 *)(p + 0x10) = (u32)func_800383D4;
    }
}

s32 func_80039470(s32 a, s32 b, s32 c, s32 d)
{
    Unk80082660 *t = D_80082660;
    s32 k = d & 0xFF;
    u8 *e = (u8 *)&t[k];

    if (*(s8 *)(e + 1) == 0)
    {
        u8 *r;
        u8 *f;

        if (k == 0)
        {
            return 1;
        }
        r = func_800392A4(k);
        if (r == 0)
        {
            return 1;
        }
        e[0] = d >> 8;
        e[1] = 1;
        e[2] = a;
        f = r - 0x20;
        if (k == 1 && *(u32 **)(f + 0x10) != D_800C3174 && *(u32 **)(f + 0x10) != D_800C321C)
        {
            *(s16 *)(r + 0x36) = b;
            *(s16 *)(r + 0x38) = c;
        }
        else
        {
            *(s16 *)(r + 0x88) = b;
            *(s16 *)(r + 0x8A) = c;
        }
        return 1;
    }
    return 0;
}

void func_80039560(void) {
}

void func_80039568(u32 *p) {
    p[4] = (u32) func_80038464;
}

void func_80038128(u8 *);

void func_80039578(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    *(u16 *) (p + 0x18) = (*q)[0] + ((*q)[1] << 8);
    *q += 2;
    *(u32 *) (p + 0x10) = (u32) func_80038128;
}

void func_80038394(u8 *);

void func_800395B0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    *(u16 *) (p + 0x30) = (*q)[0] + ((*q)[1] << 8);
    *q += 2;
    *(u32 *) (p + 0x10) = (u32) func_80038394;
}

void func_800395E8(void) {
}

void func_800395F0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 **s = (u32 **) (p + 0x7C);
    u8 *c;

    **s = (u32) (*q + 4);
    *s = *s + 1;
    c = *q;
    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}

void func_80039640(u8 *p) {
    u32 **q = (u32 **) (p + 0x7C);

    *(u32 *) (p + 0x1C) = *--(*q);
}

void func_8003965C(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);

    *(u32 *) (p + 0x1C) = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);
}

/* The 32-bit jump target is composed from the four stream bytes; both arms
   end in the same store to the cursor, which cross-jumping merges. */
s32 func_80033B2C(s32);

void func_80039694(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);
    u8 *r;

    *q = c + 2;
    if (func_80033B2C((s16) v) != 0) {
        r = *q;
        *q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    } else {
        *q = *q + 4;
    }
}

void func_80038570(u8 *);

void func_80039730(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038570;
}

void func_80039760(u8 *p) {
    u32 i = p[0x27];
    u8 *c = *(u8 **) (p + 0x1C);

    /* The byte offsets are written INSIDE each index, not folded into a
       `c = base + i * 4` pointer: through a derived pointer the four loads
       interleave with the shifts and the registers shift with them (12 DIFF).
       Same rule as the field offsets in docs/matching-levers.md section 6. */
    *(u32 *) (p + 0x1C) = c[i * 4] + (c[i * 4 + 1] << 8) +
                          (c[i * 4 + 2] << 16) + (c[i * 4 + 3] << 24);
}

void func_800397A0(void) {
}

u32 *func_8003C8E4(s32);

void func_800397A8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *r = (u8 *) func_8003C8E4(*(*q)++);

    if (r != 0) {
        r[2] = **q;
    }
    *q = *q + 1;
}

void func_80039814(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *r = (u8 *) func_8003C8E4(*(*q)++);
    u8 b;

    if (r != 0) {
        b = **q;
        r[2] = b;
        r[3] = b;
    }
    *q = *q + 1;
}

s32 func_80039884(u8 *p) {
    /* Through a pointer local, so the struct's address is materialised
       (lui+addiu) rather than split with %lo in the load's displacement --
       the target spends the addiu and fills the branch's delay slot with it. */
    Unk80083160 *s = &D_80083160;
    s32 r = 0;

    if (*(s8 *) (p + 0x87) != 0) {
        r = 1;
    } else if ((s->unk_08 & 0x60) == 0) {
        r = 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_800398BC);

void func_800399EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    t[0x3D] = *(*q)++;
}

s32 func_80039884(u8 *);
void func_800389B4(u8 *);
s16 func_80053DA8(u16);

void func_80039A08(u8 *p) {
    s32 r = func_80053EF0(4);

    if (func_80039884(p) != 0) {
        if (r != 0) {
            *(u32 *) (p + 0x10) = 0;
            *(u32 *) (p + 0x1C) -= 1;
        }
    } else if (r == 0x100) {
        *(u32 *) (p + 0x10) = (u32) func_800389B4;
    } else {
        func_80053DA8(0xB4);
    }
}

void func_80039A88(u8 *p) {
    if (func_80053EF0(4) != 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}

void func_80039ACC(u8 *p) {
    *(u32 *) (p + 0x10) = (u32) func_80039AE8;
    *(s32 *) (p + 0x1C) -= 1;
}

void func_80039AE8(void) {
}

void func_80039AF0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 **r = (u8 **) (p + 0x80);
    u8 *t;

    (*r)[1] = *(*q)++;
    t = *r;
    t[0x3C] = D_8007216C[t[1]];
}

void func_80039B28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] | (c[1] << 8);

    *q = c + 2;
    func_80053DA8(v);
}

void func_80039B64(void) {
}

void func_80039B6C(void) {
}

void func_80039B74(void) {
}

void func_80039B7C(void) {
}

void func_80039B84(u32 *p) {
    p[4] = (u32) func_80038A08;
}

void func_800383D4(u8 *);

void func_80039B94(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_800383D4;
    p[0x2F] = b;
}

void func_80038690(u8 *);

void func_80039BBC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038690;
    p[0x32] = b;
}

void func_80038708(u8 *);

void func_80039BE4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 b = *(*q)++;

    *(u32 *) (p + 0x10) = (u32) func_80038708;
    p[0x32] = b;
}

void func_8003875C(u8 *);

void func_80039C0C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x32] = *(*q)++;
    p[0x33] = *(*q)++;
    *(u32 *) (p + 0x10) = (u32) func_8003875C;
}

void func_800385D0(u8 *);

void func_80039C44(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_800385D0;
}

s32 func_80033B2C(s32);

void func_80039C74(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *c = *q;
    u16 v = c[0] + (c[1] << 8);
    u8 *r;

    *q = c + 2;
    if (func_80033B2C((s16) v) == 0) {
        r = *q;
        *q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    } else {
        *q = *q + 4;
    }
}

void func_80038630(u8 *);

void func_80039D10(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x26] = **q;
    *q += 1;
    p[0x27] = 0;
    *(u32 *) (p + 0x10) = (u32) func_80038630;
}

void func_80039D40(void) {
}

/* A bytecode opcode: the four bytes at the cursor are a little-endian function
   address, called with four words out of the block at +0x80 and its result
   stored back there. */
void func_80039D48(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 f = c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24);

    *(u8 **) (p + 0x1C) = c + 4;
    *(u32 *) (s + 0x84) = ((u32 (*)(u32, u32, u32, u32)) f)(*(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));
}

void func_80039DBC(u8 *p) {
    u8 *c = *(u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 n = *c;
    u32 *t = *(u32 **) (s + 0x40);

    *(u8 **) (p + 0x1C) = c + 1;
    *(u32 *) (s + 0x84) = ((u32 (*)(u32, u32, u32, u32)) t[n])(
        *(u32 *) (s + 0x48), *(u32 *) (s + 0x4C), *(u32 *) (s + 0x50),
        *(u32 *) (s + 0x54));
}

/* `*cp += 4` comes AFTER the second byte chain. The store is to `p + 0x1C`
   and the chain loads through an arbitrary pointer, so gcc cannot disambiguate
   them: written before the chain the store pins all four loads behind it and
   the function is a word long (42 DIFF). */
void func_80039E1C(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u8 *s = *(u8 **) (p + 0x80);
    u32 a;
    u32 w;
    u8 *r;
    u8 *q;
    u8 *t;

    a = *(*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24));
    w = q[0] + (q[1] << 8) + (q[2] << 16) + (q[3] << 24);
    *cp += 4;
    t = (u8 *) (a * 4 + w);
    *(u32 *) (s + 0x84) = (*(u32 (**)(u32, u32, u32, u32)) t)(
        *(u32 *) (s + 0x48), *(u32 *) (s + 0x4C),
        *(u32 *) (s + 0x50), *(u32 *) (s + 0x54));
}

void func_80039ED0(u32 *p) {
    p[4] = 0;
}

void func_80039ED8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);

    p[0x88] = *(*q)++;
}

void func_80039EF4(u8 *p) {
    p[0x88] -= 1;
    if ((s8) p[0x88] >= 0) {
        *(u32 *) (p + 0x10) = 0;
        *(s32 *) (p + 0x1C) -= 1;
    }
}

void func_80039F28(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + *(*q)++ * 4 + 0x48) = 0;
}

void func_80039F4C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 v = *(*q)++;
    s32 i;

    for (i = 0xF; i >= 0; i--) {
        *(u32 *) (t + i * 4 + 0x48) = v;
    }
}

void func_80039F80(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}

void func_80039FC0(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    *(u32 *) (t + i * 4 + 0x48) = c[0] + (c[1] << 8) + (c[2] << 16) +
                                  (c[3] << 24);
    *q += 4;
}

void func_8003A01C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *(u32 *) (t + a * 4 + 0x48) = *tab[b];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A060);

/* The same opcode in the other direction: composes the address the same way,
   reads FOUR BYTES at it into a little-endian word, dereferences THAT as a
   pointer, and stores the word it finds into `state[a].unk_48`.

   Both byte compositions are one expression each, as in the landed
   func_8003A1A4, and the cursor pair is the family's. Matched on the first
   compile once func_8003A2E0's re-read finding was in hand; the sweep row had
   both at 7-23 DIFF. */
void func_8003A10C(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u8 *q;
    u8 *s;
    u8 *t;
    u8 *d;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    d = (u8 *) (q[0] + (q[1] << 8) + (q[2] << 16) + (q[3] << 24));
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = *(u32 *) d;
}

/* The func_8003A2E0 family. The first chain is one expression with the
   `* 4` of the table value at the END; the SECOND chain has to be written as
   an explicit accumulation (`w = q[0]; w += q[1] << 8; ...`) or the two
   low bytes swap registers and the accumulator takes $v0 where the target has
   $v1 (9 DIFF as one expression, in any term order). */
void func_8003A1A4(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u32 w;
    u8 *s;
    u8 *r;
    u8 *q;
    u8 *d;
    u8 *t;

    a = *(*cp)++;
    s = *(u8 **) (p + 0x80);
    m = *(u32 *) (s + **cp * 4 + 0x48) * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    w = q[0];
    w += q[1] << 8;
    w += q[2] << 16;
    w += q[3] << 24;
    d = (u8 *) w;
    t = (u8 *) (a * 4 + (u32) s);
    *(u32 *) (t + 0x48) = *(u32 *) d;
}

void func_8003A248(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + b * 4 + 0x48) = *(u32 *) (t + a * 4 + 0x48);
}

void func_8003A288(u32 *p) {
    p[7]++;
}

void func_8003A29C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u32 **tab = *(u32 ***) (t + 0x44);

    *tab[b] = *(u32 *) (t + a * 4 + 0x48);
}

/* The palette-word WRITE opcode: composes a target address out of four
   bytecode bytes plus the second index times four, and copies the four bytes
   of `state[a].unk_48` into it.

   THE FOUR STORES EACH RE-READ `*(s32 *) (t + 0x48)`, and that is the whole
   function. A local for the field, or four declared byte fields, give one
   load and three shifts; the listing has FOUR loads of three different widths
   -- `lbu 0x48`, `lw 0x48` with `sra 8`, `lh 0x4A`, `lb 0x4B` -- because each
   store through `q` invalidates cse's memory table (section 4's
   invalidate_memory rule: a store through any pointer drops every
   pointer-based load, with no offset check), so the next read is real, and
   gcc then narrows each one to the width the shift actually needs. The sweep
   row had this as "ONE s32 read four ways" without the re-read, and sat at
   7-23 DIFF chasing a misplaced `sll` through all 120 permutations of the
   five statements; written this way it matches on the first compile.

   The cursor pair is the family's: `a = *(*cp)++;` then `m = **cp * 4;` with
   `(*cp)++` after it, `r = *cp` for the four address bytes, and `*cp += 4`. */
void func_8003A2E0(u8 *p) {
    u8 **cp = (u8 **) (p + 0x1C);
    u32 a;
    u32 m;
    u8 *r;
    u8 *q;
    u8 *s;
    u8 *t;

    a = *(*cp)++;
    m = **cp * 4;
    (*cp)++;
    r = *cp;
    q = (u8 *) (r[0] + (r[1] << 8) + (r[2] << 16) + (r[3] << 24) + m);
    *cp += 4;
    s = *(u8 **) (p + 0x80);
    t = (u8 *) (a * 4 + (u32) s);
    q[0] = *(s32 *) (t + 0x48);
    q[1] = *(s32 *) (t + 0x48) >> 8;
    q[2] = *(s32 *) (t + 0x48) >> 16;
    q[3] = *(s32 *) (t + 0x48) >> 24;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A374);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A3E8);

void func_8003A468(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) += *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A4B4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) -= *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A500(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + 0x84) = *(u32 *) (t + a * 4 + 0x48) -
                          *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A54C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(u32 *) (t + i * 4 + 0x48) == 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A5B4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(u32 *) (t + i * 4 + 0x48) != 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A61C(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    /* s32, not u32: the table entry is compared `<= 0`, which is `bgtz`. */
    if (*(s32 *) (t + i * 4 + 0x48) <= 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A684(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) < 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A6EC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) >= 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A754(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    if (*(s32 *) (t + i * 4 + 0x48) > 0) {
        *q = (u8 *) (c[0] + (c[1] << 8) + (c[2] << 16) + (c[3] << 24));
    } else {
        *q = c + 4;
    }
}

void func_8003A7BC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) = *(u32 *) (t + b * 4 + 0x48);
}

void func_8003A7FC(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;
    u8 *c = *q;

    *(u32 *) (t + i * 4 + 0x48) = c[0] + (c[1] << 8) + (c[2] << 16) +
                                  (c[3] << 24);
    *q += 4;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A858);

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003A904);

void func_8003A9B8(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 i = *(*q)++;

    *q = *(u8 **) (t + i * 4 + 0x48);
}

void func_8003A9E4(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(u32 *) (t + a * 4 + 0x48) *= *(u32 *) (t + b * 4 + 0x48);
}

void func_8003AA34(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u32 a = *(*q)++;
    u32 b = *(*q)++;
    u8 *t = *(u8 **) (p + 0x80);

    *(s32 *) (t + a * 4 + 0x48) /= *(s32 *) (t + b * 4 + 0x48);
}

void func_8003AA84(u8 *p) {
    u8 **q = (u8 **) (p + 0x1C);
    u8 *t = *(u8 **) (p + 0x80);
    u32 **s = (u32 **) (p + 0x7C);
    u32 i = *(*q)++;

    **s = (u32) *q;
    *s = *s + 1;
    *q = *(u8 **) (t + i * 4 + 0x48);
}

u32 func_8003AAC4(void) {
    return D_80083168[0];
}

u32 func_8003AAD4(void) {
    return D_80083170[0];
}

void func_8003AAE4(u16 a, u16 b) {
    D_80082A38[86] = a;
    D_80082A38[87] = b;
}

void func_8003AAF8(void) {
    /* A local pointer, not `D_80082A38[36]` directly: with constant indices on
       the array name the base comes out biased to the first one
       (`addiu %lo(D_80082A38)+0x48`) and a second addiu undoes it, 8 words
       against 7. Through a pointer the symbol is materialised once and all
       four accesses are displacements, as the target has them. */
    s16 *b = D_80082A38;

    *(s32 *) &b[36] = b[86];
    *(s32 *) &b[38] = b[87];
}

void func_8003AB14(u16 *d, u16 *s) {
    d[0] = s[2];
    d[1] = s[3];
    d[2] = s[4];
    d[3] = s[5];
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003AB44);

void func_8003AB44(u8 *, s16, s32, u8, s16, s32, s32);

void func_8003AC1C(s16 x, s16 y, u8 c, u8 *p) {
    s16 i = 0;

    while (*p != 0) {
        func_8003AB44(p, i, 0, c, x, y, 1);
        p += 2;
        i++;
    }
}

u8 *func_8003ACC0(u8 *p, u16 n) {
    /* The PARAMETER is the moving pointer and the saved copy is what gets
       returned, which is the `addu $a2, $a0, $zero` at the top; the other way
       round swaps those two registers through the whole function. */
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

u8 *func_8003ACC0(u8 *, u16);

/* `d` is REUSED for func_8003ACC0's result rather than a second pointer
   local: as its own variable it takes $v1 and the hoisted 0x82 takes $a0,
   where the target has them the other way round (5 DIFF, and no declaration
   order, loop shape or explicit constant local moves it). The prototype has to
   be in scope too -- without it the `u16` conversion of the digit count is
   not emitted. */
u8 *func_8003AD08(u32 n, u8 *out) {
    u8 buf[0x10];
    u8 *d = buf;
    s32 c = 0;

    do {
        c++;
        *d = n % 10;
        n = n / 10;
        d++;
    } while (n != 0);
    d = func_8003ACC0(buf, c);
    do {
        *out++ = 0x82;
        *out++ = *d++ + 0x4F;
        c--;
    } while (c > 0);
    *out = 0;
    return out;
}

/* The array spelling, not the scalar `D_80082E6A`: a 1-byte scalar is small
   data at -G8 and maspsx expands its load into one register, where the target
   has the compiler-split pair across two. */
extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

s16 func_8003ADB4(u16 a) {
    if (D_80082E6Aa[0] != 2) {
        return func_80053DA8(a);
    } else {
        return 0;
    }
}

void func_8003ADF4(s16 x, s16 y, s16 w, s16 h, u32 *data) {
    RECT r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    LoadImage(&r, data);
}

void func_8003AE28(s16 x, s16 y, s16 w, s16 h, s32 a, s32 b) {
    RECT r;

    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;
    MoveImage(&r, a, b);
}

void func_8003AE60(s16 x, s16 y, s16 w, s16 h, u8 r, u8 g, u8 b) {
    RECT rect;

    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;
    ClearImage(&rect, r, g, b);
}

void func_8003AEA8(u32 *p, s32 n) {
    for (; n > 0; n--) {
        *p++ = 0;
    }
}

void func_800A4928(void);

void func_8003AEC8(void) {
    func_800A4928();
}

void func_8003AEE8(void) {
}

void func_8003AEF0(void) {
}

void func_8003AEF8(u8 *p) {
    u8 *next;
    u32 f;

    while (p != 0) {
        next = *(u8 **) (p + 0x14);
        f = *(u32 *) p;
        *(u32 *) (p + 0x14) = 0;
        if (f != 0) {
            ((void (*)(u32, u32, u32, u32)) f)(*(u32 *) (p + 0x4),
                                               *(u32 *) (p + 0x8),
                                               *(u32 *) (p + 0xC),
                                               *(u32 *) (p + 0x10));
        }
        p = next;
    }
}

void func_8003AF58(u8 *p, u8 *q) {
    while (*(u8 **) (p + 0x14) != 0) {
        p = *(u8 **) (p + 0x14);
    }
    *(u8 **) (p + 0x14) = q;
    *(u8 **) (q + 0x14) = 0;
}

void func_8003AF8C(u32 *p) {
    p[5] = 0;
}

void func_8003AF94(s32 i, u32 v) {
    D_80189394 = v;
    D_801B8EB8 = D_8006CD50[i];
    func_801768AC();
    func_800411FC(0);
}

void func_8003AFE0(s32 i, u32 v) {
    /* The table read is the FIRST statement: with the `| 0x40000000` first
       its `lui` comes out ahead of the index chain instead of in the load's
       shadow, 8 DIFF. */
    u32 t = D_8006CD50[i];

    D_80189394 = v | 0x40000000;
    D_801B8EB8 = t;
    func_801768AC();
    func_800411FC(0);
}

u32 func_80048D20(void);
void func_800483AC(s32);

void func_8003B034(void) {
    D_80080A80 = func_80048D20();
    func_800483AC(1);
    func_800483AC(0);
}

s32 func_8003C758(u8 *);

void func_8003B06C(void) {
    func_8003C758(D_80080F40);
    func_8003C758(D_80081010);
    func_8003C758(D_80080F60);
}

extern Unk41284 D_80080EA0;
extern Unk41284 D_80080E28;
extern Unk41284 D_800D1D54;
extern Unk41284 D_800D1E7C;
extern u8 D_80081018;
void func_8008B408(s32);
void func_8008B550(s32);
void func_800903A8(void);

/* D_80081018 is declared as a SCALAR and passed as `&D_80081018`: as an
   incomplete array the decayed pointer comes out as a split lui/addiu pair
   and reorg puts the addiu in func_8003C758's delay slot, a word short. As an
   address-of it is one two-instruction `la`, which reorg cannot use for a
   delay slot -- which is why all five of these calls have an empty one. */
void func_8003B0B4(void) {
    VSync(0);
    VSync(0);
    DrawSync(0);
    func_80053DA8(0x7F);
    func_800542BC();
    VSync(0);
    func_8003AF94(0x16, 0x5DB);
    SetDispMask(1);
    func_80041284(&D_80080EA0);
    func_80041284(&D_80080E28);
    func_80041284(&D_800D1D54);
    func_80041284(&D_800D1E7C);
    func_8003B034();
    func_8003B06C();
    func_8003C758(&D_80081018);
    func_8008B408(1);
    func_8008B550(1);
    func_800903A8();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B188(void) {
    func_8003AF58(D_800717D0, D_8006AE1C);
}

void func_8003B034(void);
void func_8003B06C(void);
void func_800903A8(void);

void func_8003B1B4(void) {
    func_8003B034();
    func_8003B06C();
    func_800903A8();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B1E4(void) {
    func_8003AF58(D_800717D0, D_8006AE34);
}

void func_8003B210(void) {
    func_8003B034();
    func_8003B06C();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B238(void) {
    func_8003AF58(D_800717D0, D_8006AE4C);
}

void func_800C17A0(s32, s32);

void func_8003B264(void) {
    func_8003B034();
    func_8003B06C();
    func_800C17A0(0x2B, 0x200);
}

void func_8003AF58(u8 *, u8 *);

void func_8003B298(void) {
    func_8003AF58(D_800717D0, D_8006AE64);
}

void func_800C178C(s32, s32);

void func_8003B2C4(void) {
    func_800C178C(0xD, -1);
    func_8003B034();
    func_8003B06C();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B2F8(void) {
    func_8003AF58(D_800717D0, D_8006AE7C);
}

void func_8003AE28(s16, s16, s16, s16, s32, s32);

void func_8003B324(void) {
    func_8003AE28(0x90, 0x1F2, 0x10, 1, 0, 0x1F2);
}

extern u8 D_8006AE94[];
extern Unk41284 D_80080ED0;
extern Unk41284 D_80080ED8;
void func_8003AF58(u8 *, u8 *);
void func_8003AE28(s16, s16, s16, s16, s32, s32);
void func_8004B834(void);
void func_800418B4(void);

void func_8003B35C(void) {
    func_8003AF58(D_800717D0, D_8006AE94);
    func_8003AE28(0, 0x1F2, 0x10, 1, 0x90, 0x1F2);
    func_8003AE28(0x80, 0x1F2, 0x10, 1, 0, 0x1F2);
    func_80041284(&D_80080ED0);
    func_80041284(&D_80080ED8);
    func_8004B834();
    func_800418B4();
}

void func_8003AF58(u8 *, u8 *);

void func_8003B400(void) {
    func_8003AF58(D_800717D0, D_8006AEAC);
}

void func_8003B42C(s32 a) {
    if (a != 0) {
        func_80033AA8(0x3E8);
        func_80033AE8(0xA);
    } else {
        func_80033AE8(0x3E8);
        func_80033AA8(0xA);
    }
}

/* The frame reset: five fixed calls, then a dispatch on the mode flags at
   D_80082E76, then a common tail. Written with LABELS AND GOTOS, because the
   five arms rejoin at three different places and the listing's physical block
   order is dispatch, arms, `join1`, `ba24`, `tail`, `other`, `join2` -- the
   `other` block, which falls through into `join2`, comes LAST even though its
   branch is the earliest. Writing the arms as nested if/else puts `other`
   ahead of `join1` and the whole tail moves: 48 DIFF at 119 words against this
   one's MATCH at 117.

   `D_80082E76` is declared as an INCOMPLETE ARRAY and read as `D_80082E76[0]`.
   As a bare `u16` scalar it gets the assembler-expanded form -- one `lui` into
   the destination register -- where the listing has the compiler-split pair
   with a separate address temp. Same lever as D_8007359C.

   The flag word is also D_80082E60's +0x16 field, and the function reaches it
   BOTH ways: through its own symbol at the top, and through a struct pointer
   local at the end, which is what gives the final mask a materialised base
   with `0x16` displacements rather than one shared `%lo`. */
extern u16 D_80082E76[];
extern u8 D_8006AEC4[];
extern Unk41284 D_80080EA0;

void func_8003AF8C(u32 *);
void func_8008EF58(void);
void func_8008FCE0(void);
void func_800A03DC(void);
void func_80033D44(s32);
void func_8003B42C(s32);
void func_8003B988(void);
void func_8003B92C(void);
s32 func_80033B2C(s32);
void func_8003BA60(void);
void func_8003BA9C(void);
void func_8003B9E8(void);
void func_8003B9B8(void);
void func_8003BA24(void);
void func_800434E4(void);
void func_800B9830(void);
void func_8003BAF8(u32);
void func_8008B408(s32);
void func_8008B550(s32);
void func_80041284(Unk41284 *);
void func_8009FF28(void);
void func_800B9890(void);
void func_800B98B4(void);
void func_800C0FA4(void);

void func_8003B470(void)
{
    Unk80082E60 *q;
    u8 *p;
    u16 f;

    func_8003AF8C((u32 *)D_800717D0);
    func_8003AF8C((u32 *)D_800717E8);
    func_8008EF58();
    func_8008FCE0();
    func_800A03DC();
    *(u32 *)D_8006ADBC = 0;
    func_80033D44(0);
    f = D_80082E76[0];
    if ((f & 0x4000) != 0)
    {
        func_8003B42C(0);
        func_8003B988();
        goto join1;
    }
    if ((f & 0x8000) != 0)
    {
        func_8003B42C(1);
        func_8003B92C();
        if (func_80033B2C(0x1391) == 0 || func_80033B2C(0xA2) != 0)
        {
            func_8003BA60();
            goto tail;
        }
        if (func_80033B2C(0xA3) != 0)
        {
            goto join1;
        }
        func_8003BA9C();
        goto tail;
    }
    if ((f & 4) != 0)
    {
        goto join1;
    }
    p = (u8 *)0x80010000;
    if ((f & 2) == 0)
    {
        goto other;
    }
    *(u16 *)(p + 0x3714) &= 0xFFFD;
    if (*(u16 *)(p + 0x20A) == 0)
    {
        goto ba24;
    }
    func_8003B42C(1);
    *(u16 *)(p + 0x20A) = 0;
    func_8003B9B8();

join1:
    func_8003B9E8();
    goto tail;
ba24:
    func_8003BA24();
tail:
    func_8008B408(1);
    func_8008B550(1);
    func_80041284(&D_80080EA0);
    goto join2;
other:
    func_800434E4();
    p[0x2D6C] = 1;
    func_800B9830();
    *(u32 *)(p + 0x2D5C) = 0;
    func_8003BAF8((u32) D_8006AEC4);
    func_8008B408(0);
    func_8008B550(0);
join2:
    q = &D_80082E60;
    q->unk_16 &= 0x3FF9;
    func_8009FF28();
    func_800B9890();
    func_800B98B4();
    func_800C0FA4();
}

void func_8003B644(void) {
}

void func_800A0150(s32, u8 *, u8 *, s32);
void func_800B2244(u32);

/* func_8003B820's constant-address recipe: the 0x80010000 half in its own
   pointer local and the list pointer built from it with `| 0x29C`. Unlike
   func_8003B714 -- same walk, one call fewer -- a single `s32 v` local for
   the table byte is right here; the extra func_800B2244 call keeps the loop
   long enough that loop.c leaves the comparison constant inside it. */
void func_8003B64C(void) {
    u32 *e;
    u8 *base;

    if (*(u32 *) 0x8001029C == 0) {
        return;
    }
    base = (u8 *) 0x80010000;
    e = (u32 *) ((u32) base | 0x29C);
    do {
        u8 *a = (u8 *) *e;

        if (a[1] == 0x13) {
            s32 k = (a[3] & 0x1F) * 84;
            s32 v = *(s8 *) (base + 0x333 + k);

            if (v >= 0) {
                func_800A0150(v & 0x3F, a, (u8 *) (0x800102F0 + k), 0);
                func_800B2244(*e);
                continue;
            }
        }
        e++;
    } while (*e != 0);
}

/* func_8003B820's twin with one call more, and THE LOOP IS A BACKWARD `goto`.
   That is the whole difference between them. The sweep row here had the right
   diagnosis and the wrong cure: the comparison constant 0x13 must NOT be
   hoisted (the target re-materialises it inside the loop, where
   func_8003B820 keeps it in $s2), and the only body that stopped loop.c
   hoisting it was one that read the table byte TWICE -- once for the sign
   test and once for the `& 0x3F` -- which is three words over, because the
   target loads it once into $a0. As a `goto` loop there is no loop for
   loop.c to hoist out of at all, so a single `s32 v` local for the byte is
   right here as well as in the twin, and the function MATCHES. See
   matching-levers section 2: this is the third function in a day that the
   goto form settled.

   The rest is func_8003B820's constant-address recipe: the 0x80010000 half in
   its own pointer local, the list pointer built from it with `| 0x29C`, the
   record index as `(a[3] & 0x1F) * 84`, and the table byte reached as
   `base + 0x333 + k` so the 0x333 stays the `lb`'s displacement. */
void func_8003B714(void) {
    u32 *e;
    u8 *base;

    if (*(u32 *) 0x8001029C == 0) {
        return;
    }
    base = (u8 *) 0x80010000;
    e = (u32 *) ((u32) base | 0x29C);
loop:
    {
        u8 *a = (u8 *) *e;

        if (a[1] == 0x13) {
            s32 k = (a[3] & 0x1F) * 84;
            s32 v = *(s8 *) (base + 0x333 + k);

            if (v >= 0) {
                func_800A0150(v & 0x3F, a, (u8 *) (0x800102F0 + k), 1);
            }
        }
        e++;
    }
    if (*e != 0) {
        goto loop;
    }
}

s32 func_800B28A0(void);

/* Indexed by the counter off the constant base, which is what puts the giv's
   initialisation in the loop's PREHEADER -- after the `blez` -- and leaves
   0xAC4 as the store's displacement and 0x54 as the increment. A pointer
   walked with `p += 0x54` hoists its `lui` above the table lookup instead
   (9 DIFF); see the unbiased-giv note in docs/matching-levers.md. */
void func_8003B7C8(void) {
    u8 *b = (u8 *) 0x80010000;
    s32 n = D_800D0728[func_800B28A0()];
    s32 i;

    for (i = 0; i < n; i++) {
        *(b + i * 0x54 + 0xAC4) = 0;
    }
}

void func_800B2244(u32);

/* Two constant-address levers here. The 0x80010000 half goes into its OWN
   pointer local and the list pointer is built from it with `| 0x29C`: that is
   what gives the target's `lui $s1` / `addu $s0, $s1, $zero` / `ori` -- one
   hoisted %hi pseudo serving both constants -- where `(u32 *) 0x8001029C`
   written whole is a separate `li` and the table base a second one (41 DIFF).
   And the comparison constant goes into a local assigned BEFORE the base, so
   loop.c hoists it first and it takes $s2 while the base takes $s1; the
   literal 0x13 is hoisted second and the two registers swap (6 DIFF). */
void func_8003B820(void) {
    u32 *e;
    u8 *base;
    s32 key;

    if (*(u32 *) 0x8001029C == 0) {
        return;
    }
    key = 0x13;
    base = (u8 *) 0x80010000;
    e = (u32 *) ((u32) base | 0x29C);
    do {
        u8 *a = (u8 *) *e;

        if (a[1] == key &&
            *(s8 *) (base + 0x333 + (a[3] & 0x1F) * 84) < 0) {
            func_800B2244((u32) a);
        } else {
            e++;
        }
    } while (*e != 0);
}

void func_800B2244(u32);

void func_8003B8DC(void) {
    while (*(u32 *) 0x8001029C != 0) {
        func_800B2244(*(u32 *) 0x8001029C);
    }
}

void func_80043458(void);
void func_8003B820(void);
void func_8003B714(void);

void func_8003B92C(void) {
    u8 *b;

    func_80043458();
    func_8003B820();
    func_8003B714();
    /* The constant base is assigned AFTER the three calls, which is where the
       target materialises it; as an initialiser its `lui` is hoisted above
       them. */
    b = (u8 *) 0x80010000;
    *(u32 *) (b + 0x2D5C) += *(u32 *) (b + 0x2D58);
    if (*(u32 *) (b + 0x2D5C) > 0x3B9ACA00) {
        *(u32 *) (b + 0x2D5C) = 0x3B9ACA00;
    }
}

void func_8003B64C(void);
void func_8003B8DC(void);

void func_8003B988(void) {
    func_80043458();
    func_8003B64C();
    func_8003B8DC();
}

void func_8003B8DC(void);
void func_8003B7C8(void);

void func_8003B9B8(void) {
    func_80043458();
    func_8003B8DC();
    func_8003B7C8();
}

s32 func_800B2834(void);
void func_8003BAF8(u32);

void func_8003B9E8(void) {
    func_8003BAF8(D_8006AFDC[func_800B2834()]);
}

void func_8003BA24(void) {
    func_8003BAF8(D_8006AFE8[func_800B2834()]);
}

void func_8003BA60(void) {
    func_8003BAF8(D_8006AFF4[func_800B2834()]);
}

void func_8003BA9C(void) {
    func_8003BAF8(D_8006B000[func_800B2834()]);
}

void func_8003B9E8(void);

void func_8003BAD8(void) {
    func_8003B9E8();
}

/* The frame-start handshake. Five things decide it:

   * the two 0x30-byte records take POINTER LOCALS, and they are assigned
     AFTER func_800A0560, not as initialisers: an initialiser is evaluated at
     block entry, so its lui/addiu lands ahead of the call and the pointer
     then lives across it in a callee-saved register (2 extra words). Written
     as `D_8006ADEC + 0x28` inline, fold migrates each offset onto the symbol
     and every store loses its displacement.
   * THREE separate locals for D_8006ADEC, one per region. cse is an
     extended-basic-block pass and restarts at a join, so the three
     assignments stay three pseudos with short live ranges -- which is the
     three lui/addiu pairs the listing has. One local reused would be one
     pseudo spanning the calls, hence $s2.
   * the two table arms each spell out `BASE + *(s16 *)(p + 2) * 0x20` in
     full. jump.c's cross-jumping merges their identical trailing `sll`/`addu`
     back into the join, which is exactly where the listing has them, while a
     `cond ? A : B` base with the arithmetic after the join reads the index
     once instead of twice.
   * `*(u32 *)(p + 0xC)` goes into a local BEFORE the +0x10 store. After it,
     the load's base (the record pointer) and the store's (p) are different
     registers, memrefs_conflict_p cannot disambiguate them, and the load
     cannot be hoisted into the lui/addiu gap -- a load-delay nop instead.
   * D_800812F8 is a SCALAR whose address is taken, so its materialisation is
     the assembler-expanded `la` -- ONE insn of length 2 before maspsx, which
     `eligible_for_delay` rejects, so the `beq`'s delay slot stays a nop. As an
     incomplete array the compiler-split `addiu` drops straight into it.

   func_80041094's fifth parameter is declared u16 HERE: the listing stores
   `D_80082E76[0] ^ 1` to the outgoing slot with no sign extension, and the s16
   spelling adds the `sll`/`sra` pair (2 words). Its definition is in another
   carved unit, so the two declarations can differ. */
extern u8 D_800D381A;
extern u8 D_800D2650[];
extern u8 D_800D2FC0[];
extern s16 D_800D426C;
extern s16 D_800D426E;
extern u16 D_800D5070[];
extern u8 D_800812F8;
extern void (*D_8006AE10)(void);
extern s16 D_8006AE18[];
extern u16 D_80013714a[] __asm__("D_80013714");

void func_800A0560(void);
u8 *func_800C1034(s16, s16);
void func_80041094(u16, s16, s16, s16, u16);
s32 func_80034FE4(u32 *);

void func_8003BAF8(u32 p)
{
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *d;
    u8 *t;
    u8 *e;
    u32 g;

    func_800A0560();
    a = D_8006ADBC;
    b = D_8006ADEC;
    *(s16 *)(b + 0x28) = *(s16 *)(a + 0x28) = *(u16 *)(a + 0x18);
    *(s16 *)(b + 0x2A) = *(s16 *)(a + 0x2A) = *(u16 *)(a + 0x1A);
    *(s16 *)(b + 0x18) = *(u16 *)p;
    D_800D381A = *(s16 *)(b + 0x1A) = *(u16 *)(p + 2);
    if (*(s16 *)p == 0xB)
    {
        t = D_800D2650 + *(s16 *)(p + 2) * 0x20;
    }
    else
    {
        t = D_800D2FC0 + *(s16 *)(p + 2) * 0x20;
    }
    c = D_8006ADEC;
    e = func_800C1034(*(s16 *)(c + 0x18), *(s16 *)(c + 0x1A));
    if (*(s16 *)(e + 4) == D_800D426C && *(s16 *)(e + 6) == D_800D426E)
    {
        D_80080A88 = 1;
    }
    g = *(u32 *)(p + 0xC);
    d = D_8006ADEC;
    *(u32 *)(d + 0x10) = (u32)t;
    *(u32 *)(d + 0x24) = g;
    *(s16 *)(d + 0x14) = *(u16 *)(p + 4);
    *(s16 *)(d + 0x16) = *(u16 *)(p + 6);
    *(s16 *)(d + 0xC) = D_800D5070[*(u16 *)(p + 0x10) & 3];
    *(s16 *)(d + 0xE) = *(u16 *)(p + 0x12);
    if (*(u32 *)(p + 8) != 0)
    {
        if (*(s16 *)(d + 0x18) != 0xC)
        {
            *(u32 *)(d + 4) = (u32)&D_800812F8;
        }
        else
        {
            *(u32 *)(d + 4) = *(u32 *)(p + 8);
        }
    }
    if (D_8006AE10 != NULL)
    {
        D_8006AE10();
    }
    func_80041094(*(u16 *)p, *(u16 *)(t + 4) + (*(u16 *)(p + 4) + *(u16 *)t),
                  *(u16 *)(t + 6) + (*(u16 *)(p + 6) + *(u16 *)(t + 2)), 0,
                  D_80082E76[0] ^ 1);
    D_80013714a[0] = D_80013714a[0] & 0xFFFD;
    if (func_80034FE4((u32 *)D_80082A38) != 0)
    {
        D_8006ADE8[0] = 1;
    }
    else
    {
        D_8006ADE8[0] = 0;
    }
    D_8006AE18[0] = 0;
}

typedef struct Unk8006ADBC {
    u32 unk_00[12];
} Unk8006ADBC;

/* A 0x30-byte struct assignment: cc1psx's block move does four words per
   iteration, which is the loop in the listing -- nothing in the source is
   unrolled. */
void func_8003BD34(void) {
    *(Unk8006ADBC *) D_8006ADBC = *(Unk8006ADBC *) D_8006ADEC;
}

void func_8003BD7C(void) {
}

s32 func_80065F90(s32, s32);
s32 func_800644B8(s32);
s32 func_80064584(s32);

/* atan2-style quadrant divide: takes |x| and |y|, and divides whichever of the
   two is larger by a scale derived from both, shifting the quotient up 12.

   THE ABSOLUTE VALUES ARE `abs()`, and that is the whole function. Written out
   as a conditional negate -- `ax = x; if (ax < 0) { ax = -ax; }` -- the copy
   and the negation end up in one cse equivalence class, canon_reg rewrites the
   negation's operand to the OLDER member (the parameter), and the result is
   `negu $v0, $s1` reading the parameter directly with the copy hoisted above
   the branch. That is a word short of the listing (37 of 38) and 24 DIFF.
   `abs()` expands to the listing's own shape instead: the branch first, the
   copy in its DELAY SLOT, and the negation IN PLACE on the copy --
   `bgez $s1` / `move $v0, $s1` / `negu $v0, $v0`.

   The ternary `ax = (x < 0) ? -x : x` measures identically to the conditional
   negate, as does testing on `x` rather than `ax`, and computing the negation
   first (`ax = -x; if (x >= 0) { ax = x; }`): all three are 37 words and 24
   DIFF, because cse folds the operand back either way. Only one of the two
   needs to be `abs()` for the other to stay wrong, so both are. */

s32 func_8003BD84(s32 x, s32 y)
{
    s32 ax;
    s32 ay;
    s32 r;

    ax = abs(x);
    ay = abs(y);
    if (ax < ay)
    {
        r = (y / func_800644B8(func_80065F90(y, x))) << 12;
    }
    else
    {
        r = (x / func_80064584(func_80065F90(y, x))) << 12;
    }
    return r;
}

/* Pulls four 3-component vectors at a 4-halfword stride toward their common
   average by n/d, clamping each component at 0x7FFF. The average is summed
   into a zeroed stack array -- which is why every accumulation goes through
   memory, the memset having taken its address -- and divided by 4 with the
   `bgez` / `addiu 3` / `sra 2` sequence of a signed divide.

   Both loops INDEX by the counter: `p[i * 4]`, `p[i * 4 + 1]`,
   `p[i * 4 + 2]`. Walking a cursor with `c += 4` and reading `c[0]`, `c[1]`,
   `c[2]` does NOT give the listing's single `addiu $a0, $a0, 0x8` -- gcc adds
   a second pointer biased to +4 and reads two of the three through it at -2
   and 0, which is three words. Indexed, the giv is one register with the
   three field offsets as displacements (matching-levers section 9), and
   neither a `goto` loop nor a `do { } while (0)` wrapper reaches that; the
   subscript is the lever here. */
void func_8003BE1C(s16 *p, s32 n, s32 d)
{
    s32 sum[4];
    s32 i;
    s32 v;

    memset(sum, 0, 0x10);
    for (i = 0; i < 4; i++)
    {
        sum[0] += p[i * 4];
        sum[1] += p[i * 4 + 1];
        sum[2] += p[i * 4 + 2];
    }
    sum[0] = sum[0] / 4;
    sum[1] = sum[1] / 4;
    sum[2] = sum[2] / 4;
    for (i = 0; i < 4; i++)
    {
        v = (p[i * 4] - sum[0]) * n / d + sum[0];
        if (v > 0x7FFF)
        {
            v = 0x7FFF;
        }
        p[i * 4] = v;
        v = (p[i * 4 + 1] - sum[1]) * n / d + sum[1];
        if (v > 0x7FFF)
        {
            v = 0x7FFF;
        }
        p[i * 4 + 1] = v;
        v = (p[i * 4 + 2] - sum[2]) * n / d + sum[2];
        if (v > 0x7FFF)
        {
            v = 0x7FFF;
        }
        p[i * 4 + 2] = v;
    }
}


/* Three halfwords per entry, six entries, through two walked pointers that
   loop.c splits into four induction variables (+0 and +4 on each side). The
   source base is `base + 0x20` off its OWN local, not `&D_80083160 + 0x20`:
   written as one address expression the 0x20 folds into the `addiu %lo` and
   the function is a word short. `d += 8` before `s += 8` is what gives the two
   secondary givs the target's $a0/$v1 rather than the other way round. */
void func_8003BFE4(void) {
    u8 *d = D_80082D08;
    u8 *base = (u8 *) &D_80083160;
    u8 *s = base + 0x20;
    s32 i;

    for (i = 0; i < 6; i++) {
        *(u16 *) d = *(u16 *) s;
        *(u16 *) (d + 2) = *(u16 *) (s + 2);
        *(u16 *) (d + 4) = *(u16 *) (s + 4);
        d += 8;
        s += 8;
    }
    func_8003BE1C((s16 *) D_80082D08, 6, 4);
}

void func_8003C064(void) {
}

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

/* A return in each arm, not one `t` assigned in both: with a join the table
   pointer is a global allocno in $v1 and each arm's `(high)` is a block-local
   that takes $v0, where the target has the `lui` and the `addiu` on the same
   register. Per arm, cross-jumping merges the index and the load back into the
   one tail the listing shows and the pair can share a register. 4 DIFF.

   `== 2` with the fall-through arm second: written `!= 2` gcc branches TAKEN to
   the then-block and lays the arms out the other way round (1 DIFF). */
u32 func_8003C06C(s32 i) {
    if (D_80082E6Aa[0] == 2) {
        return D_8006B008[i];
    }
    return D_800D0858[i];
}

void func_8003C0A4(u8 *p, u32 *q) {
    if (q[2] != 0) {
        p[4] = 1;
    }
}

/* The second parameter is unused here -- func_8003C2B8 loads a word and passes
   it, so it is real. An unused parameter costs no instruction, so the body
   stays byte-identical. */
void func_8003C0C0(u8 *p, u32 a) {
    p[4] = 0;
}

void func_80044BB0(void);

void func_8003C0C8(u8 *p, s32 q) {
    if (D_8008099C == 0 && p[4] != 0) {
        func_80033CD8(p, (u32) func_80044BB0);
        func_800478B8(q);
    } else {
        func_80033D08(p);
    }
}

UnkNode *func_8003FE78(s32, UnkNode *, s16);
void func_8003C3B4(u8 *, u8 *, s32);

void func_8003C128(void) {
    /* u8 * rather than UnkNode *: the node's +0x20 is past the end of the
       struct, so the offsets stay explicit until something says what is
       there. */
    u8 *n = (u8 *) func_8003FE78(0x15, &D_80082D38, 0x49);

    *(u32 *) (n + 0x10) = (u32) func_8003C3B4;
    *(u32 *) (n + 0x20) = 0;
}

extern u8 D_800F26F8[];
extern u8 D_80077784[];
extern u8 D_800776FC[];

void func_8003DB94(u8 *, u32, s32);
void func_8003C0A4(u8 *, u32 *);
void func_8003C450(u32 *, s32, u8 *);

void func_8003C160(void)
{
    u32 *b = D_80082D58;
    u8 *e = (u8 *)b - 0x20;
    u8 *p = *(u8 **)(e + 0xC);
    u8 *q = *(u8 **)(e + 0x8);
    u8 *t;

    *(s16 *)(p + 0x1E) = 0x200;
    *(s16 *)(p + 0x1C) = 0x200;
    *(u32 *)(p + 0xC) = 0x808080;
    *(u32 *)(p + 0x8) = (u32)D_800F26F8;
    *(u32 *)(p + 0x0) = 0;
    p[4] = 0;
    p[5] = 0;
    func_8003DB94(p, b[2], 0);
    *(s16 *)(p + 0x14) = 0;
    *(s16 *)(p + 0x10) = 0;
    t = (u8 *)b[2];
    if (t == D_80077784 || t == D_800776FC)
    {
        *(s16 *)(p + 0x14) |= 0x400;
    }
    *(s16 *)(q + 0x2) = 0x54;
    *(s16 *)(q + 0x6) = 0x8D;
    *(s16 *)(q + 0xA) = 0x10;
    b[0] = (u32)func_8003C450;
    func_8003C0A4((u8 *)b, (u32 *)p);
}

void func_8003C4C8(u8 *, s32, u8 *);

void func_8003C24C(void) {
    u32 *b = D_80082D58;
    u8 *e = *(u8 **) ((u8 *) b - 0x14);

    if (e[0xC] == 0x80) {
        e[0xE] = 0x60;
        e[0xD] = 0x60;
        e[0xC] = 0x60;
        *(u16 *) (e + 0x10) = 0x20;
        *(u16 *) (e + 0x14) |= 0x1C;
    }
    b[0] = (u32) func_8003C4C8;
    func_8003C0A4((u8 *) b, (u32 *) e);
}

void func_8003C2B8(void) {
    u8 *p = (u8 *) D_80082D58;
    u32 a = *(u32 *) (p - 0x14);

    D_80082D58[0] = 0;
    func_8003C0C0(p, a);
    func_80033D08(p);
}

void func_8003C520(u8 *, s32, u8 *);

void func_8003C2F8(void) {
    u32 *b = D_80082D58;
    u8 *e = *(u8 **) ((u8 *) b - 0x14);

    if (e[0xC] == 0x80) {
        e[0xE] = 0x60;
        e[0xD] = 0x60;
        e[0xC] = 0x60;
        *(u16 *) (e + 0x10) = 0x20;
        *(u16 *) (e + 0x14) |= 0x1C;
    }
    b[0] = (u32) func_8003C520;
    func_8003C0A4((u8 *) b, (u32 *) e);
}

void func_8003C364(void) {
    u8 *p = (u8 *) D_80082D58;
    u32 g;

    func_80033D08(p);
    g = D_800814A0;
    *(u16 *) (p - 2) |= 0x8000;
    D_800814A0 = g | 0x8000;
}

/* The last store as an if/else, not a `?:`: as a conditional expression the
   value lands in $v1 instead of reusing the flag's $v0 (3 DIFF). The indirect
   call's argument is the pass-through $a0. */
void func_8003C3B4(u8 *p, u8 *q, s32 c) {
    u8 *b = (u8 *) &D_80083160;
    u32 f = *(u32 *) p;

    if (f != 0) {
        ((void (*)(u8 *)) f)(p);
        if ((*(u16 *) (p - 2) & 0x8000) != 0) {
            return;
        }
    }
    func_8003C0C8(p, c);
    if ((*(u32 *) (b + 8) & 0x10) != 0) {
        *(s16 *) (q + 0xA) = 5;
    } else {
        *(s16 *) (q + 0xA) = 0x10;
    }
}

void func_8003C488(u32 *, s32, u8 *);

void func_8003C450(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1E);

    *v += 0x400;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C488;
    }
}

void func_8003C4C0(void);

void func_8003C488(u32 *cb, s32 b, u8 *p) {
    u16 *v = (u16 *) (p + 0x1C);

    *v += 0x200;
    if (*v >= 0x1000) {
        *v = 0x1000;
        *cb = (u32) func_8003C4C0;
    }
}

void func_8003C4C0(void) {
}

void func_8003C4C8(u8 *p, s32 b, u8 *q) {
    s32 v = q[0xE] - 0x10;

    *(u16 *) (q + 0x1C) -= 0x200;
    if (v < 0) {
        v = 0;
    }
    q[0xE] = v;
    q[0xD] = v;
    q[0xC] = v;
    if (*(s16 *) (q + 0x1C) <= 0) {
        *(u16 *) (q + 0x1E) = 0;
        *(u32 *) p = 0;
        func_8003C0C0(p, (u32) q);
    }
}

void func_8003C520(u8 *p, s32 b, u8 *q) {
    s32 n = q[0xE] - 0x10;

    *(u16 *) (q + 0x1C) -= 0x200;
    if (n < 0) {
        n = 0;
    }
    q[0xE] = n;
    q[0xD] = n;
    q[0xC] = n;
    if (*(s16 *) (q + 0x1C) <= 0) {
        func_80033D08(p);
        *(u16 *) (p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* A `while` with the test on the INDEXED table, not a walked pointer: loop
   duplication gives the guard's `lb %lo(D_8006B1EC)` with the index folded
   away, and strength reduction then walks the pointer in the body. One
   `return -1` serves both the guard and the loop exit, which is why the target
   can copy the guard's comparison constant into $a3 and return it in $v0 --
   written as two returns the -1 is rematerialised and the index return needs
   its own block (11 DIFF, one word long). The pointer spelling instead swaps
   the pointer and the counter registers (13 DIFF). */
s32 func_8003C5A4(s32 key) {
    s32 i = 0;

    while (D_8006B1EC[i] != -1) {
        if (D_8006B1EC[i] == key) {
            return i;
        }
        i++;
    }
    return -1;
}

s32 func_8003C5A4(s32);

/* The key passes straight through from this function's own parameter, so no
   argument move is emitted -- see the pass-through note in
   docs/matching-levers.md section 3. */
u32 func_8003C5F0(s32 key) {
    s32 i = func_8003C5A4(key);

    if (i < 0) {
        return 0;
    } else {
        return D_8006B1A8[i];
    }
}

/* $gp group: D_80080A6C is in this unit's sdata_entries block, so this was
   verified in a scratch file carrying that block. The comparison is written
   with the parameter on the LEFT: the other way round the `beq` operands swap.
   */
s32 func_8003C634(u8 *a) {
    s32 i;

    for (i = 0; i < D_80080A6C; i++) {
        if ((u32) a == D_8006B200s[i].unk_00) {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/unk_72A8", func_8003C67C);

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");

void func_8003C6F8(u32 a, s32 i) {
    /* Through the struct array, which brings the base materialisation out
       ahead of the index shift as the target has it (3 DIFF the other way). */
    u32 v = D_8006B200s[i].unk_04;

    D_8006B200s[i].unk_00 = a;
    D_80081460 = a;
    D_80080A8C = v;
    func_80041284((Unk41284 *) a);
    if (D_80082E6Aa[0] == 2) {
        D_800DCF4D = -2;
    }
}

extern u8 D_80082E6Aa[] __asm__("D_80082E6A");
s32 func_8003C634(u8 *);

/* $gp group: D_80080A6C is in this unit's sdata_entries block, so it was
   verified in a scratch file carrying that block. It is re-read after the call
   because the call invalidates it. */
s32 func_8003C758(u8 *p) {
    s32 r;

    if (D_80082E6Aa[0] == 2 && D_800DCF4D != -2) {
        r = -1;
    } else {
        r = func_8003C634(p);
    }
    if (r == -1) {
        func_8003C6F8((u32) p, D_80080A6C - 1);
        r = D_80080A6C - 1;
    }
    return r;
}

extern RECT D_8006B220[];
void func_8003C67C(s32);

/* NESTED guards with one `return -1` at the end, not three early returns:
   with three branches to a shared `-1` block reorg fills the second one's
   delay slot from that block and the `li -1` is duplicated, a word over
   (36 DIFF). Both parameters are `s32`, not the `(u8, s8)` the callers'
   prototype said: the listing has no sign extension of the index and no
   `andi` of the key before func_8003C5F0's pass-through. And the element
   address is written index-first, `(u8 *) (n * 8 + (u32) t)`. */
s32 func_8003C7D8(s32 k, s32 n) {
    u32 *d = D_80082D58;
    u8 *t;
    u8 *e;
    u8 *f;
    s32 r;
    u8 *b;

    if (n >= 0) {
        t = (u8 *) func_8003C5F0(k);
        if (t != 0) {
            e = (u8 *) (n * 8 + (u32) t);
            f = *(u8 **) e;
            if (f != 0) {
                d[2] = *(u32 *) (e + 4);
                r = func_8003C758(f);
                func_8003C67C(r);
                b = D_8006B200;
                LoadImage(D_8006B220, *(u32 **) (b + 4));
                LoadImage(&D_8006B220[1],
                          (u32 *) (*(u32 *) (b + 4) + 0x2000));
                return r;
            }
        }
    }
    return -1;
}

u32 func_8003C5F0(s32);

/* `||` is the shape, not two nested ifs. The short-circuit layout puts the if
   BODY between the two tests and the continuation last -- `bltz` to the body,
   `bnez` past it -- which is what the listing has. Every nesting of two tests
   with a `return 0` in each arm instead ends with gcc keeping the TRAILING
   copy of `return 0` and inverting the second branch, because cross-jumping
   merges the two identical return blocks and redirects the earlier one. 13 DIFF
   over four spellings. */
s32 func_8003C890(s32 a, s32 b) {
    u32 t;

    if (b < 0 || (t = func_8003C5F0(a)) == 0) {
        return 0;
    }
    return *(u32 *) (t + b * 8) != 0;
}

s32 func_8003C5A4(s32);

/* The argument passes straight through to func_8003C5A4, which is invisible
   here and shows up at func_800397A8, which passes a stream byte. */
u32 *func_8003C8E4(s32 k) {
    s32 i = func_8003C5A4(k);

    /* The arm written FIRST ends up SECOND in memory, so the `>= 0` arm first
       is what gives the target's `bgez`; written as `if (i < 0) return 0;`
       the branch comes out `bltz` and the two arms swap (6 DIFF). */
    if (i >= 0) {
        return &((u32 *) 0x800135C4)[i];
    } else {
        return 0;
    }
}

extern Blk20 D_8002D594b __asm__("D_8002D594");
extern u32 D_8006B200w[] __asm__("D_8006B200");

/* Copies the eight-word D_8002D594 template into a DEAD local -- gcc 2.7.2's
   flow does not delete dead stores to memory, so the copy survives and is the
   whole 0x20 frame -- and then, unless D_80082E6A says mode 2, into
   D_8006B200 as well; mode 2 instead plants a zero and the D_80016000
   pointer. D_80080A6C records which.

   MATCHED on the third draft. Two things carry it:

   * The `!= 2` arm is written FIRST, so the mode-2 arm ends up last in
     memory, where the listing has it (the other way round is 32 DIFF).

   * The mode-2 arm's FIRST store is symbolic and its second goes through a
     base LOCAL assigned between them. That is section 8's "N materialisations
     of ONE base mean N LOCALS" read forwards: the listing has
     `sw $zero, %lo(D_8006B200)($v0)` then `addiu $v0, $v0, %lo(D_8006B200)`
     then `sw $v1, 0x4($v0)`, i.e. ONE `lui` -- which reorg lifts into the
     `beq`'s delay slot -- serving the symbolic store, the rebuild, and the
     other arm's `addiu $a1, $v0, %lo`. Both stores through one pointer local
     declared before the `if` instead is 31 DIFF: that gives a lui/addiu pair
     in the common block and two zero-displacement stores. */
void func_8003C920(void) {
    Blk20 t = D_8002D594b;

    if (D_80082E6Aa[0] != 2) {
        *(Blk20 *) D_8006B200w = D_8002D594b;
        D_80080A6C = 4;
    } else {
        u32 *b;

        D_8006B200w[0] = 0;
        b = D_8006B200w;
        b[1] = (u32) D_80016000;
        D_80080A6C = 1;
    }
}
