#include "common.h"

/* main.bin's overlay at 0x80020000: the memory card save/load subsystem.
   Built at cc1psx -G0, not the -G8 the resident executable uses; the Makefile
   has the per-unit override and says why.

   This file is still one splat unit, but the original source was NINE:
   c_server.c, card_mes.c, card_opt.c, card_win.c, cardbk.c, cardinfo.c,
   cardslot.c, cardsme.c and memory.c, in that link order. The names and the
   boundaries come from the debug twin at 0x80400000, which is the same source
   built at -G8 with more of its asserts compiled in; `tools/assertmap.py`
   recovers the map and docs/overlays.md records it. Carve this file along
   those boundaries before matching anything whose shape depends on which unit
   it is in. */

/* 0x80069C28 is libcard's `_card_wait`, resident in the engine. splat names
   it positionally here because main.bin has no symbol_addrs file yet. */
void func_80069C28(s32 chan);

void func_800213C4(s32 arg0);
/* No arguments: func_80020A0C calls it with no $a0 set up at all, and its own
   listing never reads $a0 (it works off D_80028550 and up). func_80020924's
   call looks like it passes arg0 only because $a0 still holds the incoming
   parameter there; both spellings compile identically in that caller, and
   this one is what func_80020A0C requires. See the CALLEE ARITY caution in
   docs/matching-levers.md. */
void func_800214A4(void);
void func_8002219C(s32 arg0);
/* card_mes.c's message entry point. The 20 wrappers at func_80022E74..
   func_8002316C are one per message id 0..19, each passing a fixed id and
   kind and either a caller-supplied parameter or 0. Decoded mechanically
   from which argument register each wrapper sets; the register copies
   (`addu $a2, $a1, $zero` where the kind equals the id, `addu $a3, $a2,
   $zero` where both are 0) are gcc reusing an equal value, not source. */
void func_80022D60(s32 a, s32 id, s32 kind, s32 p);

extern s32 D_800287C8;
extern s32 D_800287CC;
extern s32 D_800287E4;
/* 128 bytes per entry; func_80021D38's `sll $a0, $a0, 7` is the stride and
   the 2D array spelling is what produces the lui/addiu pair plus the shift,
   where a scalar would fold differently. The element type is a guess that
   only has to make offset 0 a word. */
extern s32 D_80083E98[][32];
extern s32 D_80084120[];
/* The overlay's own data, past its text end at 0x80027E20 and so in the
   following `bin` segment; the linker resolves them from
   splat/main.bin_undefined_syms_auto.txt. Declared as arrays because the
   listings materialise them with the lui/addiu pair rather than folding a
   %lo into the access. */
extern u8 D_80027F60[];
extern u8 D_80027F68[];

extern u8 D_80027FC4[];
extern u8 D_80027FF8[];
extern u8 D_8002808C[];
extern u8 D_80028094[];
/* D_800814A0 is already declared `extern u32` in include/structs.h, which
   common.h pulls in; declaring it again here is a conflicting-types error.

   This unit needs BOTH spellings of it, which is what the `__asm__` alias is
   for. func_80026FB4 updates it in place beside a u16 store and wants the
   SCALAR form: there the target interleaves the two updates, which is what a
   non-varying-address store permits. func_80023A00 does the same two updates
   and the target does NOT interleave them -- 20 words with a load-delay nop
   against 19 interleaved -- so there it needs the ARRAY form, where both
   sides are in-struct, cse's `invalidate` drops the cached load and sched1
   cannot hoist. Same mechanism and the same lever as `extern u32
   D_800814B0[1]` in include/structs.h; see docs/matching-levers.md section 5.
   Neither a cast-to-pointer index nor a pointer local reaches it: both still
   give 19 words. The lever is PER SITE. */
extern u32 D_800814A0_a[1] __asm__("D_800814A0");

void func_800491F4(s32 a, u8 *fmt, s32 n);
void func_800269B4(s32 *p);
void func_80026B18(s32 *p);
s32 func_8004CAA0(void);

/* State handlers whose ADDRESS is stored into a field at -0x10 by the
   countdown family (func_800252A4, func_8002546C, func_80026398,
   func_8002765C). Only the address is taken, so the signature here does not
   affect the generated code; it is written `(void)` rather than guessed. */
/* The two GPU-primitive templates the card UI copies out of the engine's
   data: D_8002E5D8 is 4 words and D_8002E5E8 is 3, both declared `extern
   u32 []` in include/structs.h (which calls D_8002E5D8 "a 4-word template"),
   so they are copied through a cast rather than retyped -- retyping them
   would change every engine unit that already matches. The destination
   layouts below are what the stores after each copy require: word-sized at
   0x00/0x04, then s16 at 0x08/0x0A/0x0C and a byte at 0x0F. */
typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 unk_0E;
    u8 unk_0F;
} UnkCardPrim;

typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s32 unk_08;
} UnkCardPrim12;

typedef struct
{
    s32 unk_00;
    UnkCardPrim *unk_04;
    UnkCardPrim12 *unk_08;
    s32 unk_0C;
} UnkCardPair;

/* The arrayed form of the same initialiser, at six different base offsets.
   Each of func_80022B48, func_800237EC, func_80023EF4, func_80024C70,
   func_80026DF0 and func_8002727C builds N of these in one loop, and the
   layout is IDENTICAL in all six -- a 12-byte shared UnkCardPrim12 at the
   base, 0x10 bytes of padding, then N back-pointers, N UnkCardPairs and N
   4-word primitive templates -- only the base offset and N differ
   (0x8B0/7, 0x184/18, 0x240/10, 0xB20/21, 0x44/4, 0xD4/4). It has to be
   spelled as a struct rather than as separately computed pointers: the
   target's loop keeps ONE base register and strength-reduces into
   base-relative induction variables carrying the large displacements
   (0x8CC, 0x958, 0x8EC, 0x8F0), which is what indexing through a struct
   produces. Precomputing `base + 0x8CC` and friends into locals gives 38
   words against 40 and every word differing. */
#define UNK_CARD_GROUP(name, pad, n) \
    typedef struct                   \
    {                                \
        u8 pad_000[pad];             \
        u32 shared[3];               \
        u8 pad_00C[0x10];            \
        UnkCardPair *ptrs[n];        \
        UnkCardPair ents[n];         \
        u32 prims[n][4];             \
    } name

void func_80023600(s32 *p);
void func_80023730(UnkCardPair *p, u32 *a, u32 *b);
void func_80024BB4(UnkCardPair *p, u32 *a, u32 *b);
void func_800271C0(UnkCardPair *p, u32 *a, u32 *b);
void func_800239B0(s32 *p);
s32 func_8002168C(void);
void func_800231E4(s32 a);
void func_80024ACC(s32 *p);
void func_800270E0(s32 *p);

void func_80025230(void);
void func_800261F0(void);
void func_800265B8(void);
void func_80027584(void);
void func_8002789C(void);
void func_8002593C(void);
void func_800274D8(void);
void func_80025398(void);
void func_800262BC(void);
void func_80027584(void);

/* c_server.c, somewhere in lines 1-180: the first function in the overlay and
   the first one landed. Its twin is func_804008A0.

   The pointer local is load-bearing and is the lever at
   docs/matching-levers.md:3173, spent at the two stores and NOT at the read.
   Storing straight to the global emits each `%hi` beside its own store, so the
   else branch gets a second `lui` and the function runs one word long (25).
   Through a local assigned after the three calls, the `%hi` is emitted at the
   assignment -- before the branch, so both stores share $v1, and reorg pulls
   it into the beqz delay slot. The trailing read is left as the bare symbol so
   it keeps its own re-materialised `lui $v0`, which is what the listing has.
   The local costs no instruction: combine folds the `%lo` back into the
   stores. */
void func_80020924(s32 arg0)
{
    s32 *p;

    func_800214A4();
    func_80069C28(0);
    func_80069C28(1);
    p = &D_800287CC;
    if (arg0)
    {
        *p = 0x10;
    }
    else
    {
        *p = 0;
    }
    func_800213C4(D_800287CC);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020984);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800209C4);

void func_80020A0C(void)
{
    s32 *p = &D_800287C8;

    *p = 0;
    func_800214A4();
}

s32 func_80020A30(void)
{
    D_800287E4 = 1;
    return 0;
}

s32 func_80020A44(void)
{
    D_800287E4 = 2;
    return 0;
}

s32 func_80020A58(void)
{
    D_800287E4 = 3;
    return 0;
}

s32 func_80020A6C(void)
{
    D_800287E4 = 4;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020A80);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020EA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020F18);

s32 func_800210E0(void)
{
    return D_800287C8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800210F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002121C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021298);

s32 func_8002137C(void)
{
    s32 *p = &D_800287C8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 4;
        *p = 0;
    }
    return r;
}

s32 func_800213A0(void)
{
    s32 *p = &D_800287C8;
    s32 r = 0;

    if (*p != 0)
    {
        r = 5;
        *p = 0;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800213C4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021410);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800214A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800214FC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021538);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021594);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020000);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020028);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020034);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020048);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002168C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800217D4);

void func_80021904(void)
{
}

s32 func_8002190C(void)
{
    return D_800287CC != 0;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020074);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002191C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800219B8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021A04);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021A84);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021B18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021B98);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021C4C);

s32 func_80021D38(s32 i)
{
    return D_80083E98[i][0];
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021D54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021DF4);

void func_80021EB8(s32 *gsw)
{
    s32 sum = 0;
    s32 *q = gsw + 0x82;
    s32 n = 0x177D;

    do
    {
        sum ^= *q++;
        n--;
    } while (n >= 0);
    gsw[0x81] = sum;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021EE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021F18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800220DC);

s32 func_80022138(void)
{
    s32 *p = D_80084120;

    if (D_800287CC != 0)
    {
        p++;
    }
    return *p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022160);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002219C);

void func_8002223C(void)
{
    func_8002219C(5);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002225C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022300);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002233C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800223B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022488);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022524);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800226A8);

void func_800226E0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022774);

void func_8002290C(s32 *p)
{
    s32 **q = (s32 **)p[0x238];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022934);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022A30);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022AE8);

void func_80022B20(s32 a)
{
    func_800491F4(a, D_80027F60, 7);
}

/* NEAR MISS, left as asm deliberately. The shape is worked out: see the
   UNK_CARD_GROUP comment above for the layout and the six base offsets and
   counts. Two spellings measured, neither reached it:

     separate base pointers (base + 0x8CC etc. into locals)   38w, 40 DIFF
     struct-indexed, `u32 *s1 = D_8002E5D8` local before loop 39w, 37 DIFF
     struct-indexed, D_8002E5D8[k] inline in the loop         39w, 40 DIFF

   Target is 40 words. What the remaining difference is about: the target's
   first word is `addu $t3, $a0, $zero`, a copy of the base parameter into
   another register AHEAD of the `blez $a1` guard, and only `$a3 = 0` joins it
   there -- every other invariant sits in the preheader after the guard. The
   struct-indexed forms put too much above the guard. Worth trying next: a
   local pointer assigned from the parameter (`c = base;`) so the copy is a
   real insn, and checking which of the four induction variables the target
   actually derives (it keeps base in $t3 and walks byte offsets in $t1/$t2
   plus base-relative pointers in $a2/$t0). */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022B48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022BE8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022C48);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022C90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022CD8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022D60);

void func_80022E74(s32 a, s32 p)
{
    func_80022D60(a, 0, 1, p);
}

void func_80022E9C(s32 a)
{
    func_80022D60(a, 1, 0, 0);
}

void func_80022EC4(s32 a, s32 p)
{
    func_80022D60(a, 2, 2, p);
}

void func_80022EEC(s32 a)
{
    func_80022D60(a, 8, 0, 0);
}

void func_80022F14(s32 a, s32 p)
{
    func_80022D60(a, 4, 2, p);
}

void func_80022F3C(s32 a, s32 p)
{
    func_80022D60(a, 5, 1, p);
}

void func_80022F64(s32 a, s32 p)
{
    func_80022D60(a, 6, 2, p);
}

void func_80022F8C(s32 a, s32 p)
{
    func_80022D60(a, 14, 2, p);
}

void func_80022FB4(s32 a)
{
    func_80022D60(a, 9, 0, 0);
}

void func_80022FDC(s32 a)
{
    func_80022D60(a, 10, 0, 0);
}

void func_80023004(s32 a)
{
    func_80022D60(a, 11, 0, 0);
}

void func_8002302C(s32 a)
{
    func_80022D60(a, 12, 0, 0);
}

void func_80023054(s32 a)
{
    func_80022D60(a, 13, 0, 0);
}

void func_8002307C(s32 a)
{
    func_80022D60(a, 3, 0, 0);
}

void func_800230A4(s32 a)
{
    func_80022D60(a, 7, 0, 0);
}

void func_800230CC(s32 a, s32 p)
{
    func_80022D60(a, 15, 2, p);
}

void func_800230F4(s32 a)
{
    func_80022D60(a, 16, 0, 0);
}

void func_8002311C(s32 a)
{
    func_80022D60(a, 17, 0, 0);
}

void func_80023144(s32 a, s32 p)
{
    func_80022D60(a, 18, 1, p);
}

void func_8002316C(s32 a)
{
    func_80022D60(a, 19, 0, 0);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023194);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800231E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023258);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800233D0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023470);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002352C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800235A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023600);

void func_80023730(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

void func_800237C4(s32 a)
{
    func_800491F4(a, D_80027F68, 0x12);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800237EC);

void func_8002388C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80023894(s32 *p, s32 v)
{
    func_8002388C(p, v);
    func_80023730((UnkCardPair *)(p + 0x64), (u32 *)(p + 0x5A), (u32 *)(p + 0x5E));
    func_80023600(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800238D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800239B0);

void func_80023A00(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800239B0(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80023A50(s32 *p, s32 v)
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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023A78);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023B18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023BE4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023C44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023D64);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023E14);

void func_80023EC4(s32 *p)
{
    p[-4] = 0;
}

void func_80023ECC(s32 a)
{
    func_800491F4(a, D_80027FC4, 0xA);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023EF4);

void func_80023F94(s32 *p, s32 a, s32 b)
{
    p[0] = b;
    p[1] = a;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023FA0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023FF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800240EC);

void func_80024160(s32 **p)
{
    s32 i = 0;

    do
    {
        *p[0x97] = 0;
        p++;
    } while (++i < 10);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024184);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800241D4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024224);

void func_80024274(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x89] = 0x606868;
        *(s32 *)q[0xA0] = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024298);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800242D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024390);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024818);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800249B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024A50);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024ACC);

void func_80024BB4(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

void func_80024C48(s32 a)
{
    func_800491F4(a, D_80027FF8, 0x15);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024C70);

void func_80024D10(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80024D18(s32 *p, s32 v)
{
    func_80024D10(p, v);
    func_80024BB4((UnkCardPair *)(p + 0x2CB), (u32 *)(p + 0x2C1), (u32 *)(p + 0x2C5));
    func_80024ACC(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024D58);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024E44);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024EEC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024F3C);

void func_80024F7C(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

void func_80024FAC(s32 *p)
{
    func_80024F7C(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002593C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025030);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800250E8);

void func_8002519C(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800251A4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025230);

void func_800252A4(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025230;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800252CC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025358);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025398);

void func_8002546C(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025398;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025494);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800254E4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025584);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002570C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025788);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025848);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800258C8);

void func_8002592C(s32 *p)
{
    p[18] = 1;
    p[19] = 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002593C);

void func_80025B38(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80025B40(s32 *p, s32 v)
{
    func_80025B38(p, v);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025B9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025BD4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025D34);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025DC8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025E54);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025ECC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025FFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026128);

void func_800261C0(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[13];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800261F0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026240);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026270);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800262BC);

void func_80026398(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_800262BC;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800263C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026410);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002644C);

void func_80026494(s32 *p)
{
    p[13] = (s32)func_800265B8;
    func_80022FB4((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

void func_800264D8(s32 *p)
{
    p[13] = (s32)func_800265B8;
    func_80022FDC((s32)(p - 8));
    p[-4] = (s32)func_800261F0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002651C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800265B8);

void func_80026720(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80026728(s32 *p, s32 v)
{
    func_80026720(p, v);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026748);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026784);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800267C0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026868);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026920);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800269B4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026B18);

void func_80026C00(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026C94);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026D78);

void func_80026DC8(s32 a)
{
    func_800491F4(a, D_8002808C, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026DF0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026E90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026ED0);

void func_80026FB4(u16 *p)
{
    p[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80026FD4(s32 *p, s32 v)
{
    s32 *q = p + 8;

    q[9] = v;
    func_800269B4(q);
}

void func_80026FF8(s32 *p)
{
    func_80026B18(p + 8);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027018);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027054);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027080);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800270E0);

void func_800271C0(UnkCardPair *p, u32 *a, u32 *b)
{
    u32 *s1 = D_8002E5D8;
    u32 *s2 = D_8002E5E8;

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

void func_80027254(s32 a)
{
    func_800491F4(a, D_80028094, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002727C);

void func_8002731C(s32 *p, s32 v)
{
    p[0] = v;
}

void func_80027324(s32 *p, s32 v)
{
    func_8002731C(p, v);
    func_800271C0((UnkCardPair *)(p + 0x38), (u32 *)(p + 0x2E), (u32 *)(p + 0x32));
    func_800270E0(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027364);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002742C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027458);

void func_800274A8(s32 *p)
{
    u16 *w = (u16 *)(p - 8);
    u16 f = w[15];

    if ((f & 0x2000) != 0)
    {
        w[15] = f & ~0x2000;
        p[-4] = p[11];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800274D8);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027528);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027584);

void func_8002765C(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80027584;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027684);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800276EC);

void func_80027734(s32 *p)
{
    p[11] = (s32)func_8002789C;
    func_80022E9C((s32)(p - 8));
    p[-4] = (s32)func_800274A8;
}

void func_80027778(s32 *p)
{
    p[11] = (s32)func_8002789C;
    func_8002307C((s32)(p - 8));
    p[-4] = (s32)func_800274D8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800277BC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002789C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027A68);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027AFC);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027BC0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027BF4);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027C60);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027C90);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D40);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027D7C);

s16 func_80027DD0(void)
{
    return func_8004CAA0();
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027DF4);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200B8);
