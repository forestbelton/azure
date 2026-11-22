#include "common.h"
#include "ovl_dungeon_decls.h"

typedef struct UnkDungeonSPB
{
    s16 unk_00;
    u16 unk_02;
    u8 pad_04[0x4];
    UnkDungeonSPW unk_08;
    UnkDungeonSPW unk_0C;
    UnkDungeonSPW unk_10;
    UnkDungeonSPW unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    s16 unk_24;
    u8 pad_26[0x2];
    s16 unk_28;
    s16 unk_2A;
    s16 unk_2C;
    u8 pad_2E[0x2];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0xC];
    s32 unk_48;
    u8 pad_4C[0x24];
    s16 unk_70;
    s16 unk_72;
    s16 unk_74;
    u8 pad_76[0x2];
    s16 unk_78;
    s16 unk_7A;
    s16 unk_7C;
    u8 pad_7E[0x2];
    s16 unk_80;
    s16 unk_82;
    s16 unk_84;
    u8 pad_86[0x2];
    s16 unk_88;
    s16 unk_8A;
    s16 unk_8C;
    u8 pad_8E[0xA];
    s16 unk_98;
    s16 unk_9A;
    s16 unk_9C;
    u8 pad_9E[0x46];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u8 pad_F0[0x10];
    u16 unk_100;
    u16 unk_102;
} UnkDungeonSPB;
s32 func_8009E798(u8 *p, UnkVec *b, UnkSlot *s);

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

/* 151 words, 0 differing.  THREE levers, each measured on the body before it,
   and the park file this replaced records every rejected spelling with its
   score (9 -> 13 -> 1 -> 0 rows):

   * BLOCK 2's STORE INSIDE BOTH ARMS (no `y').  cse2's extended block walks
     AROUND a one-armed `if' (cse.c:8198, skip_blocks), so canon_reg commons
     the two `q[0x3] = 5;' into ONE callee-saved pseudo and the function comes
     out a word short of the target's two `addiu $v1, $zero, 0x5'.  Two-armed
     ends cse's block at the join (cse.c:8166) and both 5s appear; jump2
     cross-jumps the identical `sh' tails so the branch is unchanged.  Worth
     -4 rows alone and +12 with the next one.

   * `h' DECLARED INSIDE EACH TAIL BLOCK.  At function scope it is assigned in
     two basic blocks, so combine_regs refuses it (local-alloc.c:1834, a local
     assigned in more than one block has reg_qty == -1) and `h + 0xE' cannot be
     tied to `h'.  The target's `addiu $v0, $v0, 0xE' IS that tie.  Two
     inner-scope declarations give two block-local pseudos, each tied to its own
     sum: h in $v0 and the 5 in $v1 in both tails.  Undoing it costs 12 rows.

   * `g' MATERIALISED LATE.  `Unk80083160 *g;' with no initialiser, all seven
     block-0 accesses spelled `D_80083160.unk_00', and `g = &D_80083160;' inside
     block 1's five-store header.  That moves the `addiu $s1, $s1, %lo' lo_sum
     three insns later, to 80089634, and lowers the %hi quantity's QTY_CMP_PRI
     span enough that `s32 lean' wins $s0 without the `andi' that `u8 lean'
     costs.  The direct spellings are free alone and the move is worth nothing
     alone: one change.  Outside the header it is 22-23 rows.

   The do-while pair is a reference count, not a guess.  `lean' must be
   allocated before the `%hi(D_80083160)' pseudo (getting it the wrong way round
   is 21 rows) and local-alloc ties the xor temp into lean's quantity
   (local-alloc.c:1939), which gives it 5 refs where it needs 7:
   floor_log2(n)*n is 10, 12, 14 at n = 5, 6, 7 and 6 refs still loses.
   flow.c:2122 is `reg_n_refs[regno] += loop_depth;', so depth 3 on the `tmp'
   read supplies them for no words.  The SPLIT through `tmp' is required -- the
   same wrapper on `lean2 = (s16)lean;' gives `lean2' four refs, which outranks
   `g' in allocno_compare and swaps $s1/$s3 (23 rows).  `tmp = lean;' without
   the cast is 21 rows: a plain copy dies in cse before flow counts it.
   `u8 lean' reaches 7 refs instead but costs the `andi' -- 152 words / 1 row.
   refsweep (386 points), widthsweep and stmtsweep are all clean on the
   device-free body, so there is no cancelled pair that replaces it.

   `lean2 = (s16)lean;' before the first `if' and `lean3 = (s16)lean2;' before
   the second are the target's two reg-reg copies at 80089638 and 800896A8:
   combine folds a redundant extension of a value whose nonzero_bits is 1 into
   a plain movsi (combine.c:8303).  `lean2 = lean;' coalesces and emits
   nothing.  Do not simplify the chain. */
void func_80089588(void)
{
    Unk80083160 *g;
    u32 w[2];
    u8 *q;
    s32 lean;
    s32 tmp;
    s32 lean2;
    s32 lean3;

    lean = D_80083160.unk_00 != D_801C9E40;
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(D_80083160.unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, D_80083160.unk_00);
    AddPrim(D_80083160.unk_00 + 0x830, q);
    q += 0xC;
    func_80067EF4(q, 0, 0);
    AddPrim(D_80083160.unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    do
    {
        do
        {
            tmp = (s16)lean;
        } while (0);
    } while (0);
    *(s16 *)(q + 0x14) = 0x100;
    g = &D_80083160;
    *(s16 *)(q + 0xC) = 0x100;
    lean2 = (s16)tmp;
    if (lean)
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    q[0x3] = 5;
    {
        s32 h;
        h = *(u16 *)(q + 0xE);
        *(s16 *)(q + 0xA) = h;
        *(s16 *)(q + 0x16) = h + 0xE;
        *(s16 *)(q + 0x12) = h + 0xE;
    }
    AddPrim(g->unk_00 + 0x830, q);
    q += 0x18;
    *(u32 *)(q + 0x4) = 0x2A000000;
    *(s16 *)(q + 0x10) = 1;
    *(s16 *)(q + 0x8) = 1;
    *(s16 *)(q + 0x14) = 0x100;
    *(s16 *)(q + 0xC) = 0x100;
    lean3 = (s16)lean2;
    if (lean3 == 0)
    {
        *(s16 *)(q + 0xE) = 0x1C0;
    }
    else
    {
        *(s16 *)(q + 0xE) = 0xE0;
    }
    q[0x3] = 5;
    {
        s32 h;
        h = *(u16 *)(q + 0xE);
        *(s16 *)(q + 0xA) = h;
        *(s16 *)(q + 0x16) = h + 0xE;
        *(s16 *)(q + 0x12) = h + 0xE;
    }
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0x18;
    func_80067EF4(q, 0, 1);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    func_80067F20((u32 *)q, 0, 0, 0x20, 0);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    w[0] = 0x01C00000;
    w[1] = 0x00100100;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x7B0, q);
    q += 0xC;
    SetDrawArea(q, w);
    AddPrim(g->unk_00 + 0x830, q);
    q += 0xC;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)q;
}

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

void func_80094988(u8 *p, u8 *q, s16 x, s16 y)
{
    s16 sx;
    s16 sy;
    u8 bx;
    u8 by;
    u8 *b;
    u8 *e;
    u8 *n;
    s16 sv;
    s16 idx;
    s32 i;
    s32 k;
    u8 *slot;
    u8 *c3;
    u8 *c4;
    u8 *c;
    u8 *c2;

    b = (u8 *)0x80010000;
    sv = *(u16 *)(b + 0x3714) & 2;
    *(u16 *)(b + 0x3714) |= 2;
    func_800A6A9C();
    func_80041E28(q, (u8 *)0x80012194);
    *(u16 *)(p + 0xF8) = *(u16 *)(b + 0x2D50);
    *(u32 *)(p + 0xFC) = *(u32 *)(b + 0x2D58);
    for (i = 0; i < 0x14; i++)
    {
        *(UnkDungeonName4 *)(b + 0x248 + i * 4) = *(UnkDungeonName4 *)(b + 0x21E8 + i * 4);
        if (*(u8 *)(b + 0x2238 + i) != 0xFF)
        {
            *(u32 *)(b + 0x29C + i * 4) = (u32)((u8 *)0x80010248 + *(u8 *)(b + 0x2238 + i) * 4);
        }
        else
        {
            *(u32 *)(b + 0x29C + i * 4) = 0;
        }
        D_800E3E48[i] = *(Unk800E3E48 *)(b + 0x2260 + i * 0x8C);
        if (*(u8 *)(b + 0x224C + i) != 0xFF)
        {
            D_800E3DF0[i] = (s32)&D_800E3E48[*(u8 *)(b + 0x224C + i)];
        }
        else
        {
            D_800E3DF0[i] = 0;
        }
    }
    n = (u8 *)0x80010000;
    *(u32 *)(n + 0x29C + i * 4) = 0;
    for (i = 0; i < 2; i++)
    {
        if (*((u8 *)0x80012D52 + i) != 0xFF)
        {
            *(u32 *)(p + i * 4 + 0xD0) = (u32)((u8 *)0x80010248 + *((u8 *)0x80012D52 + i) * 4);
        }
        else
        {
            *(u32 *)(p + i * 4 + 0xD0) = 0;
        }
    }
    c = (u8 *)0x80010000;
    c2 = (u8 *)0x80010000;
    slot = (*(u8 *)(c + 0x21E0) == 0xFF) ? NULL : (u8 *)0x80010248 + *(u8 *)(c2 + 0x21E0) * 4;
    func_800982A8(q, slot);
    c3 = (u8 *)0x80010000;
    c4 = (u8 *)0x80010000;
    slot = (*(u8 *)(c3 + 0x21E1) == 0xFF) ? NULL : (u8 *)0x80010248 + *(u8 *)(c4 + 0x21E1) * 4;
    func_80098614(q, slot);
    slot = (u8 *)0x80010248;
    for (k = 0; k < 0x14; k++, slot += 4)
    {
        if (slot[1] == 0x13 && (slot[3] & 0x20) != 0)
        {
            for (i = 0, idx = 0; i < 2; i++)
            {
                if (*(u32 *)(p + i * 4 + 0xD0) == (u32)slot)
                {
                    idx = i + 1;
                }
            }
            if (idx != 0)
            {
                idx--;
                e = func_800A1618(slot[0], 3);
                if (e != NULL)
                {
                    if (func_8009B88C(NULL, x, y, &sx, &sy) == 0)
                    {
                        while ((s16)func_800A4E2C(&bx, &by) < 0)
                        {
                        }
                        sx = bx;
                        sy = by;
                    }
                    e = ((u8 * (*)(s32, s16, s16, s16)) func_800A0B94(slot[0], e,
                                                                      1))(1, sx, sy,
                                                                          *(s16 *)(q + 0x88));
                    *(u8 **)(p + idx * 4 + 0xAC) = e;
                    if (e != NULL)
                    {
                        func_80042640((Unk42710 *)e, e[0x13]);
                        func_800424E0(e, e[0x13], slot);
                        *(u32 *)(p + idx * 4 + 0xD0) = (u32)slot;
                        slot[3] |= 0x20;
                        D_800E3D74[idx] = 0;
                        if (e[0x25] == 0)
                        {
                            *(u32 *)(e + 0x1C) |= 8;
                        }
                        D_800E3DF0[slot[3] & 0x1F] = (s32)e;
                        func_80042984(e);
                        func_800A152C(slot[0], 3);
                        func_80042560((s8 *)e);
                        func_800C542C(e, D_800DCED4[func_800429E4(e)], idx, 1);
                    }
                }
            }
        }
    }
    if (sv == 0)
    {
        D_80013714 &= 0xFFFD;
    }
    func_80035208(D_800E045C);
}

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

/* 358 words, 0 differing.  LANDED 2026-11-22, after five days parked and a
   final run at 1 row.

   THE ONE SPELLING FACT: the SECOND of the two adjacent AddPrim calls passes
   `r - 0x28', not `q'.  They are the same pointer -- `r == q + 0x28' there --
   but naming `r' lets combine fold the subtraction straight into the argument
   move, `(set (reg a1) (plus (reg/v r) (const_int -40)))', so the target's
   `addiu $a1, $s2, -0x28' comes out where `q' gives `addu $a1, $s0, $zero'.

   Identical instruction count, identical allocation -- `.greg' is byte-identical
   between the two bodies and there is no $s8 in either, which refutes the
   park's own long-standing diagnosis.  The change is worth exactly ONE ROW and
   ZERO WORDS, which is why no sweep and no permuter could ever see it: both
   spellings are one instruction at 358 words.

   Spelling it `c - 0x50' -- the same address off the cursor -- is 1 row,
   because the target reaches the argument from `r' in $s2 and not from the
   cursor in $s1.  That is a fact about which local the original author wrote.

   AND THE PARK HAD THIS LINE RECORDED AS REFUTED, at 360 words / 128 rows,
   in a list that claimed to be scoped to the 1-row body.  That measurement was
   taken on an ANCESTOR body, before one or more of the park's own four
   changes.  A two-word regression reads as a 128-row catastrophe, so it looked
   like a refutation of the mechanism rather than of the body it was tried on.
   The reliable tell is the WORD COUNT.

   The three devices are all still load-bearing at 0 rows: without the asm on
   `r' 80 rows, without the asm on `q' 359 words / 128 rows, without the
   cancelled `r++'/`r--' pair 9 rows.  `base' is free either way (0 rows
   without it) and is kept for legibility. */
s32 func_8009E798(u8 *p, UnkVec *b, UnkSlot *s)
{
    Unk80083160 *g = &D_80083160;
    UnkDungeonSPB *sp = (UnkDungeonSPB *)0x1F800000;
    u8 *base = D_80083160.unk_00;
    u8 *q;
    u8 *r;
    u8 *e;
    s16 flag;
    s32 ry;
    s16 a;
    s16 bb;
    RECT rc;
    u8 *c;

    q = *(u8 **)(base + 0x8D0);
    sp->unk_20 = base + 0xB0;
    sp->unk_38 = 0x1000;
    flag = base != D_801C9E40;
    sp->unk_48 = 0;
    func_80067E2C((DR_AREA *)q, (RECT *)base);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, q);
    sp->unk_30 = s->unk_1C;
    sp->unk_34 = s->unk_1E;
    sp->unk_00 = *(u16 *)((u8 *)b + 0x2);
    sp->unk_02 = *(u16 *)((u8 *)b + 0x6);
    e = (u8 *)s->unk_08;
    sp->unk_24 = s->unk_14;
    PushMatrix();
    sp->unk_98 = 0;
    sp->unk_9A = 0;
    sp->unk_9C = g->unk_C8;
    RotMatrix((SVECTOR *)0x1F800098, (MATRIX *)0x1F8000D0);
    ScaleMatrix((MATRIX *)0x1F8000D0, (VECTOR *)0x1F800030);
    sp->unk_E4 = sp->unk_00;
    sp->unk_E8 = (s16)sp->unk_02;
    sp->unk_EC = 0;
    func_80064CF0((MATRIX *)0x1F8000D0);
    func_80064D80((MATRIX *)0x1F8000D0);
    sp->unk_28 = 0x20 - s->unk_20;
    sp->unk_2A = 0x20 - s->unk_22;
    sp->unk_2C = 0;
    func_80065320((SVECTOR *)0x1F800028, (s32 *)0x1F800100, (s32 *)0x1F800094);
    q += 0xC;
    sp->unk_100 = sp->unk_100 - sp->unk_00;
    sp->unk_102 = sp->unk_102 - sp->unk_02;
    sp->unk_08.w = e[0x8];
    sp->unk_0C.w = e[0x9];
    sp->unk_10.w = e[0xA];
    sp->unk_14.w = e[0xB];
    a = (s8)e[0x2];
    sp->unk_80 = a;
    sp->unk_70 = a;
    a = a + sp->unk_10.h;
    sp->unk_88 = a;
    sp->unk_78 = a;
    bb = (s8)e[0x3];
    sp->unk_7A = bb;
    sp->unk_72 = bb;
    bb = bb + sp->unk_14.h;
    sp->unk_8A = bb;
    sp->unk_82 = bb;
    sp->unk_8C = 0;
    sp->unk_84 = 0;
    sp->unk_7C = 0;
    sp->unk_74 = 0;
    func_80065320((SVECTOR *)0x1F800070, (s32 *)(q + 0x8), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800078, (s32 *)(q + 0x10), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800080, (s32 *)(q + 0x18), (s32 *)0x1F800094);
    func_80065320((SVECTOR *)0x1F800088, (s32 *)(q + 0x20), (s32 *)0x1F800094);
    *(s16 *)(q + 0x8) = *(u16 *)(q + 0x8) + sp->unk_100;
    *(s16 *)(q + 0x10) = *(u16 *)(q + 0x10) + sp->unk_100;
    *(s16 *)(q + 0x18) = *(u16 *)(q + 0x18) + sp->unk_100;
    *(s16 *)(q + 0x20) = *(u16 *)(q + 0x20) + sp->unk_100;
    *(s16 *)(q + 0xA) = *(u16 *)(q + 0xA) + sp->unk_102;
    *(s16 *)(q + 0x12) = *(u16 *)(q + 0x12) + sp->unk_102;
    *(s16 *)(q + 0x1A) = *(u16 *)(q + 0x1A) + sp->unk_102;
    *(s16 *)(q + 0x22) = *(u16 *)(q + 0x22) + sp->unk_102;
    sp->unk_10.w = sp->unk_10.w + sp->unk_08.w;
    sp->unk_14.w = (sp->unk_14.w + sp->unk_0C.w) << 8;
    sp->unk_0C.w = sp->unk_0C.w << 8;
    *(s16 *)(q + 0xE) = s->unk_12 + *(u16 *)(e + 0x6);
    *(s16 *)(q + 0xC) = sp->unk_0C.h + sp->unk_08.h;
    *(u32 *)(q + 0x14) =
        sp->unk_0C.w + sp->unk_10.w + ((s->unk_10 | *(s16 *)(e + 0x4)) << 16);
    *(s16 *)(q + 0x1C) = sp->unk_14.h + sp->unk_08.h;
    *(s16 *)(q + 0x24) = sp->unk_14.h + sp->unk_10.h;
    *(u32 *)(q + 0x4) = *(u32 *)s->unk_0C;
    func_800666F4((POLY_FT4 *)q);
    __asm__("" : "=r"(q) : "0"(q));
    r = q + 0x28;
    __asm__("" : "=r"(r) : "0"(r));
    func_8003DB6C((int *)r, (int *)q, 0xA);
    c = r + 0x28;
    q[0x7] |= 2;
    r[0xD] = r[0x15] += 0x40;
    *(u16 *)(r + 0x16) |= 0x100;
    r++;
    r--;
    r[0x1D] = r[0x25] += 0x40;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, r - 0x28);
    *(u32 *)(c + 0x4) = 0x606060;
    *(s32 *)(c + 0x8) = *(s32 *)(c - 0x20);
    *(s32 *)(c + 0xC) = *(s32 *)(c - 0x18);
    *(s32 *)(c + 0x10) = *(s32 *)(c - 0x10);
    *(s32 *)(c + 0x14) = *(s32 *)(c - 0x8);
    c[0x4] >>= 1;
    c[0x5] >>= 1;
    c[0x6] >>= 1;
    func_800666E0((POLY_F4 *)c);
    c[0x7] |= 2;
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    c = r + 0x40;
    func_80067F20((u32 *)c, 0, 0, 0x40, 0);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    rc.x = *(u16 *)(p + 0x4);
    ry = *(u16 *)(p + 0x6);
    c = r + 0x4C;
    rc.y = ry;
    rc.w = *(u16 *)(p + 0x8);
    rc.h = *(u16 *)(p + 0xA);
    if (flag != 0)
    {
        rc.y = ry + 0xE0;
    }
    func_80067E2C((DR_AREA *)c, &rc);
    AddPrim(sp->unk_20 + *(s16 *)((u8 *)b + 0xA) * 4, c);
    PopMatrix();
    c = r + 0x58;
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)c;
    return 0;
}

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
