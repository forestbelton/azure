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
s32 func_8002219C(s32 idx);
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

s32 func_800491F4(s32 a, u8 *fmt, s32 n);
void func_800269B4(s32 *p);
void func_80026B18(u8 *p, s32 *b, s32 *c);
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

/* Four bytes, alignment 1: func_80026128 copies this out of D_80028064 with
   lwl/lwr + swl/swr, which is what gcc emits for a struct it cannot assume is
   word-aligned. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;

/* 0x80 bytes, alignment 1: func_80021DF4 copies a whole D_80083E98 row with
   gcc's unknown-alignment block move -- the `(src | dst) & 3` test and both a
   lwl/lwr and a lw loop. Through the s32[][32] type gcc would know both sides
   are word-aligned and emit only the second. */
typedef struct
{
    u8 b[0x80];
} UnkRow;

typedef struct
{
    s32 unk_00;
    UnkCardPrim *unk_04;
    UnkCardPrim12 *unk_08;
    s32 unk_0C;
} UnkCardPair;

/* 24-byte records: five string pointers then four bytes. The size comes
   from func_80022C48's `b * 24` index and the layout from func_80022BE8,
   which walks the five pointers and writes the bytes at 0x16 and 0x17. The
   struct spelling is load-bearing in func_80022CD8: as a plain u8 array
   indexed `[i * 24 + 0x15]` gcc folds the 0x15 into the symbol's %lo and the
   listing's `lbu $v0, 0x15($v0)` comes out with displacement 0. */
typedef struct
{
    u8 *strs[5];
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 unk_17;
} UnkStrSet;

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
void func_800239B0(s32 **pp);
void func_800240EC(s32 *p);
void func_800251A4(s32 *p);
void func_80024E44(s32 *p);
void func_8002742C(s32 *p);
void func_80025030(s32 *p);
void func_80025DC8(s32 *p);
void func_8004F52C(s32 a);
u8 *func_8004DA74(u8 *out, u8 *s, s32 a);
void func_8004CB2C(s32 *p);
void func_80026920(s32 *p);
void func_80027BF4(void);
void func_80040AA0(s32 a);
void func_8005863C(void);
void func_8006A084(void);
void func_8006A094(void);
void func_8006A248(u32 ev);
s32 *func_80027AFC(s32 a, s32 b);
u8 *func_80022A30(u8 *p, s32 *q);
void func_80021538(void);
void func_80025D34(s32 *p);
void func_80023E14(s32 *p);
void func_80025F0C(s32 *p, s32 v);
void func_80025FFC(s32 *p, s32 v);
void func_80026D78(s32 *p);
void func_80022BE8(UnkStrSet *p);
s32 func_80069BF8(s32 a);
void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n);
void func_80026C94(s32 *p);
void func_80026CF0(s32 *p);
void func_80064F20(s32 a);
void func_80064F00(s32 a, s32 b);
s32 func_8006A024(s32 ev);
s32 func_8002191C(void);
void func_80027C90(s32 *p);
s32 *func_80025C7C(s32 *p);
s32 *func_80026868(s32 *p, s32 a);
s32 func_8004F418(s32 *p, s32 *q);
void func_80020F18(void);
void func_800585A0(void);
s32 func_80021FF0(s32 a, u8 *b, s32 c);
s32 func_80033B2C(s16 a);
void func_800A6104(void);
void func_80022774(s32 *a, s32 *b);
void func_80022934(s32 *p);
void func_80022488(s32 *p);
void func_80022524(void);
void func_8004CBFC(s32 *p, u8 *q);
void func_800439F8(void);
void func_8006A0F4(u8 *p);
extern const u8 D_80020084[];

s32 func_8006A104(u8 *name, u8 *e);
void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z);
void func_8006A134(u8 *name);
s32 func_80049E6C(s32 a);
void func_80023C44(s32 *p);
s32 func_8006A0A4(u8 *name, s32 mode);
s32 *func_8003FE78(s32 a, u8 *b, s32 c);
s32 *func_8003FC64(s32 a);
s32 func_8004B4A8(s32 *p);
s32 func_80021F18(s32 a, u8 *b);
void func_800237EC(s32 a, s32 n);
void func_80023258(s32 *p);
s32 func_80069C38(s32 a);
s32 func_80069C18(s32 a);
s32 func_8002168C(void);
void func_80026DF0(s32 a, s32 n);
s32 func_8006A0D4(s32 fd, u8 *buf, s32 len);
u8 *func_800242D8(u8 *p, s32 w, s32 h);
extern u8 D_8002B850[];
extern u8 D_80079054[];
extern u8 D_800790B4[];
extern u8 D_80079114[];
extern s32 D_8002809C[];
extern u8 D_800291D0[];
extern u8 *D_800282A4[];
extern u8 D_800791A4[];
void func_8002727C(s32 a, s32 n);
s16 func_80027DD0(void);
extern u8 D_8002BB08[];
extern u8 D_8002B968[];
s32 *func_80027364(s32 *p);
s32 *func_800238D4(s32 *p);
s32 func_80024D58(s32 *p, s32 a, s32 b);
extern u8 D_80027FD0[];
extern u8 D_8002B7E0[];
s32 func_8006A0B4(s32 fd, s32 off, s32 whence);
s32 func_8006A0C4(s32 fd, u8 *buf, s32 len);
s32 func_8006A0E4(s32 fd);
extern u8 D_800200A0[];
void func_80023B18(s32 *p);
extern u8 D_80027F7C[];
extern u8 D_800282B0[];
extern u8 D_80077E84[];
void func_800223B4(s32 *p, s32 x, s32 y, s32 n, s32 z);
s32 func_80021B98(u8 *name, u8 *buf, s32 n, s32 off);
extern u8 D_80028064[];
extern u8 *D_800283EC[];
u8 *func_8006A114(u8 *e);
void func_8005FE18(s32 a);
extern u8 D_80027E68[];
void func_80069E78(u8 *dst, u8 *src);
void func_80069E38(u8 *dst, u8 *src);
void func_80025788(s32 *p);
extern u8 *D_80027F18[];
extern u8 D_80027F20[];
extern u8 *D_80027F30[];
s32 func_80069C08(s32 a);
void func_80024390(s32 *p);
void func_80024818(s32 *p);
void func_800249B4(s32 *p);
void func_800250E8(s32 *p);
void func_80025584(void);
extern s16 D_80028010[];
void func_80020A80(void);
void func_80069EC8(void *p, s32 n);
void func_8004E884(u8 *p, s32 a);
void func_8004E264(u8 *p, s32 a);
extern s32 D_80084118;
extern s32 D_800287D0;
extern s32 D_800287D8;
extern s32 D_80028808;
extern u8 *D_800282BC[];
s32 *func_80026ED0(s32 *p, s32 a);
void func_800252CC(s32 *p);
void func_8002570C(s32 *p);
void func_80025848(s32 *p);
extern u8 D_80028418[];
extern u8 D_80028430[];
extern u8 D_8002807C[];
extern u8 D_80028084[];
extern s32 D_80083200;
extern s32 D_80028550;
extern s32 D_80028554;
extern s32 D_80028558;
extern s32 D_8002855C;
extern s32 D_80028560;
extern s32 D_80028564;
extern s32 D_80028568;
extern s32 D_8002856C;
extern s32 D_80084128[];
extern u8 D_80028020[];
extern UnkStrSet D_800280B4[];
extern s32 D_80028570[][10];
extern s32 D_800287E0;
void func_80024A50(s32 *p);
/* In town's/dungeon's overlay region above the engine, reached as a plain
   absolute call; the address comes from main.bin_undefined_funcs_auto.txt. */
void func_800A68C0(void);
extern u8 D_80077EF0[];
extern u8 D_80077EFC[];
void func_80020EA0(void);
extern u8 D_800282CC[];
s32 func_80021D54(s32 n);
s32 func_80023FF0(s32 a, s32 n);
void func_80025B9C(s32 *p);
void func_80027684(s32 *p);
void func_80020984(void);
void func_80026410(s32 *p);
void func_80049D80(s32 p, s16 *r);
void func_8004E994(s32 a);
extern u8 D_800786DC[];
extern u8 D_800786E8[];
s32 func_8002168C(void);
void func_80026DF0(s32 a, s32 n);
s32 func_8006A0D4(s32 fd, u8 *buf, s32 len);
u8 *func_800242D8(u8 *p, s32 w, s32 h);
extern u8 D_8002B850[];
extern u8 D_80079054[];
extern u8 D_800790B4[];
extern u8 D_80079114[];
s32 func_800231E4(s32 flag);
void func_80024ACC(s32 *p);
void func_800270E0(s32 *p);

void func_80025230(s32 *p);
void func_800265B8(void);
void func_80027584(s32 *p);
void func_8002789C(void);
void func_8002593C(void);
void func_80025398(s32 *p);
void func_800262BC(s32 *p);
void func_80027584(s32 *p);

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

void func_80020984(void)
{
    func_800214A4();
    func_80069C28(0);
    func_80069C28(1);
    func_800213C4(D_800287CC);
}

void func_800209C4(void)
{
    s32 v;

    func_800214A4();
    func_80069C28(0);
    func_80069C28(1);
    v = 0x10 - D_800287CC;
    D_800287CC = v;
    func_800213C4(v);
}

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

void func_80020EA0(void)
{
    if (D_80084130[0] == 0)
    {
        D_80084130[0] = 1;
        D_80084118 = 0;
        (&D_80084118)[1] = 0;
        func_80069EC8(D_80083E98, 0x280);
    }
    func_80020A80();
    D_800287D0 = 0;
    (&D_800287D0)[1] = 0;
    D_800287D8 = 0;
    (&D_800287D8)[1] = 0;
    D_800287C8 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80020F18);

s32 func_800210E0(void)
{
    return D_800287C8;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800210F0);

s32 func_8002121C(void)
{
    s32 r = 0;

    if (D_800287C8 == 2)
    {
        func_800214A4();
        if (func_80069C08(D_800287CC) == 0)
        {
            D_800287C8 = 0;
            r = 5;
        }
        else
        {
            D_800287E0 = 0;
            D_800287C8 = D_800287C8 + 1;
        }
    }
    return r;
}

/* NEAR MISS: 58 words against 57, 23 DIFF. The dispatch is the problem: the target tests st==1 and st==3 with BEQ to blocks placed AFTER the chain, sets r=5 in the second beq's delay slot and jumps to the exit. Four structures measured -- plain if/else-if chain (56w/44, one word short, gcc merges the r=5 path into the exit), st!=1 first (56w/44, the FIRST test then matches), fully nested != (56w/44), a switch (58w/23), and this goto chain (58w/23). The bodies are right: state 1 re-registers the events then checks func_80069C38 and func_80069C18's bit 4 before advancing D_800287C8, state 3 clears one of the two D_80084118 words chosen by D_800287CC and returns 3, anything else returns 5.
   Source in docs/near-misses/func_80021298.c. */
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

/* NEAR MISS: 18 words against 19: the target carries an extra 'addu $v1, $v0, $zero' copy before storing 1 into D_800287C8, with the constant materialised back in the loop's branch delay slot. Three spellings measured (a local for the 1, 'while' against 'do/while', the two stores swapped), all 18 words and 10-11 DIFF. Reload artifact; permuter.
   Source in docs/near-misses/func_800213C4.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800213C4);

s32 func_80021410(void)
{
    s32 r = 0;

    if (func_8006A024(D_80028550) != 0)
    {
        r = 1;
    }
    else if (func_8006A024(D_80028554) != 0)
    {
        r = 2;
    }
    else if (func_8006A024(D_80028558) != 0)
    {
        r = 3;
    }
    else if (func_8006A024(D_8002855C) != 0)
    {
        r = 4;
    }
    return r;
}

void func_800214A4(void)
{
    func_8006A024(D_80028550);
    func_8006A024(D_80028554);
    func_8006A024(D_80028558);
    func_8006A024(D_8002855C);
}

s32 func_800214FC(void)
{
    s32 v = D_800287E4;
    s32 r = 0;

    if (v != 0)
    {
        r = v;
        func_80021538();
    }
    return r;
}

void func_80021538(void)
{
    D_800287E4 = 0;
    func_8006A024(D_80028560);
    func_8006A024(D_80028564);
    func_8006A024(D_80028568);
    func_8006A024(D_8002856C);
}

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

s32 func_8002191C(void)
{
    u8 name[0x20];
    s32 n = 0;
    u8 *e = (u8 *)D_80028570;
    u8 **t = D_80027F18;

    if (D_800287CC != 0)
    {
        t++;
    }
    func_80069E78(name, *t);
    func_80069E38(name, (u8 *)D_80020084);
    if (func_8006A104(name, e) != 0)
    {
        do
        {
            n++;
            e += 0x28;
        } while (func_8006A114(e) == e);
    }
    return n;
}

/* splat MIGRATED this block into func_8002191C's nonmatching listing, so
   landing that function in C left the symbol with no definition and the link
   failed with "undefined reference to D_80020084". It cannot be replaced by a
   `"*"` literal in the function either: the label covers 28 bytes holding
   SEVEN strings, lumped under one name because only the first is referenced
   symbolically, and "TMP" and "04".."00" belong to functions still in asm.
   So it is defined here, immediately after the function whose listing used to
   carry it, which is its place in the assembly stream. As those other
   functions land, their literals come from gcc and this shrinks.

   (Turning splat's `migrate_rodata_to_functions` off is NOT the fix, though
   it looks like one: with migration off splat routes the whole `.rodata`
   subsegment to a standalone asm/data file instead of into the sibling C
   object, and then gcc's own constants -- func_800258C8's local
   function-pointer array -- have nowhere correct to land.) */
const u8 D_80020084[28] = "*\0\0\0TMP\0"
                          "04\0\0"
                          "03\0\0"
                          "02\0\0"
                          "01\0\0"
                          "00\0\0";

/* NEAR MISS: 18 words against 19: the target copies the materialised base into the induction variable ('addu $v1, $v0, $zero') and seeds the sum from the counter's zero ('addu $a1, $a2, $zero'); every spelling folds one of the two away. Four measured (indexed against pointer-walk, counter declared first or not, guarded do/while), all 18 words.
   Source in docs/near-misses/func_800219B8.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800219B8);

s32 func_80021A04(s32 n, u8 *s)
{
    s32 i = 0;
    s32 ret = 0;
    u8 *tbl = (u8 *)D_80028570;
    u8 *row;

    if (n > 0)
    {
        row = tbl;
        do
        {
            s32 bad = 0;
            s32 j = 0;

            do
            {
                u8 c = s[j];

                if (c == 0)
                {
                    break;
                }
                if (c != row[j])
                {
                    bad++;
                    break;
                }
                j++;
            } while (j < 0x15);
            if (bad == 0)
            {
                ret = 1;
                break;
            }
            row += 0x28;
        } while (++i < n);
    }
    return ret;
}

s32 func_80021A84(s32 flag)
{
    u8 **t;
    s32 r;

    func_80069C28(0);
    func_80069C28(1);
    func_80021538();
    t = D_80027F18;
    if (flag != 0)
    {
        t++;
    }
    func_8006A0F4(*t);
    while ((r = func_800214FC()) == 0)
    {
        func_8005FE18(0);
    }
    if (r != 1)
    {
        if (r == 3)
        {
            r = 4;
        }
        else
        {
            r = 5;
        }
    }
    return r;
}

void func_80021B18(u8 *s, s32 idx)
{
    u8 **t = D_80027F18;

    if (D_800287CC != 0)
    {
        t++;
    }
    func_80069E78(s, *t);
    func_80069E38(s, D_80027F20);
    func_80069E38(s, D_80027F30[idx]);
}

/* NEAR MISS: 45 words, 28 DIFF, all register NAMES: the open/lseek/read/close sequence is right (func_8006A0A4/B4/C4/E4 = open/lseek/read/close on "bu00:"), but local-alloc assigns the five callee-saved locals differently -- the target has buf=$s5, n=$s2, off=$s4, r=$s1, -1=$s3, fd=$s0 and keeps the -1 in a register across both comparisons. Three spellings measured (an explicit -1 local, fd declared first with a len local, the plain form); the plain form is this one. Allocation only.
   Source in docs/near-misses/func_80021B98.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021B98);

s32 func_80021C4C(u8 *name, u8 *buf, s32 n, s32 off, s32 skip)
{
    s32 ok = 0;
    s32 fd;

    if (skip == 0)
    {
        fd = func_8006A0A4(name, 0x30200);
        if (fd == -1)
        {
            return ok;
        }
        func_8006A0E4(fd);
    }
    fd = func_8006A0A4(name, 2);
    if (fd == -1)
    {
        return ok;
    }
    if (off == 0 || func_8006A0B4(fd, off << 7, 0) != -1)
    {
        if (func_8006A0D4(fd, buf, n << 7) == (n << 7))
        {
            ok = 1;
        }
    }
    func_8006A0E4(fd);
    return ok;
}

s32 func_80021D38(s32 i)
{
    return D_80083E98[i][0];
}

s32 func_80021D54(s32 idx)
{
    u8 name[0x20];
    s32 r = 1;
    s32 **t;
    s32 *e;
    s32(*tbl)[32];
    s32 v;

    func_80021B18(name, idx);
    t = (s32 **)D_80084128;
    if (D_800287CC != 0)
    {
        t++;
    }
    v = func_80021A04((s32)*t, name + 5);
    tbl = D_80083E98;
    e = tbl[idx];
    e[0] = v;
    if (v != 0)
    {
        r = func_80021B98(name, (u8 *)e, 1, 4);
        e[0] = 1;
    }
    return r;
}

/* NEAR MISS: 49 words, 7 DIFF: the function is an 0x80-byte block copy of one D_80083E98 row from src+0x200, through a byte-aligned struct so gcc emits its unknown-alignment expansion (the (src|dst)&3 test plus both a lwl/lwr and a lw loop), then sets row[0] = 1. What differs is only the ORDER of the SECOND row-address computation: the target emits 'sll' then 'lui'/'addiu' then 'addu' (and reorg pulls the sll into the preceding j's delay slot), while every spelling here emits the base first. Three measured: the plain 2D index, a byte offset for the second access, and byte offsets for both. The FIRST computation's order (base first) already matches, so the two differ in the target and no single spelling gives both.
   Source in docs/near-misses/func_80021DF4.c. */
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

/* NEAR MISS, 14 words and 6 DIFF, all of them register NAMES -- the target
   keeps the cursor in $a1 and the counter in $v1, this gets them the other way
   round. Source in docs/near-misses/func_80021EE0.c; it is the verifier half
   of func_80021EB8 above.

   The INDEXED loop is not a stylistic choice and must be kept: written as
   `*q++` with an upcount, loop.c's check_dbra_loop reverses the loop into the
   downcount form func_80021EB8 actually has (`li 0x177D` / `addiu -1` /
   `bgez`), 13 words. Indexing by the counter makes it the addressing biv and
   blocks the reversal. Seven spellings all give 14 words and the same 6 DIFF:
   declaration order either way, `for` against `do`/`while`, `q[i]` against
   `gsw[0x82 + i]` against `(gsw + 0x82)[i]`, `== sum` against
   `(... ^ sum) == 0`, and u32 against s32. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021EE0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021F18);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80021FF0);

void func_800220DC(void)
{
    s32 i = D_800287CC != 0;
    s32 n;

    n = func_8002191C();
    D_80084128[i] = n;
    D_80084120[i] = func_800219B8(n);
}

s32 func_80022138(void)
{
    s32 *p = D_80084120;

    if (D_800287CC != 0)
    {
        p++;
    }
    return *p;
}

s32 func_80022160(void)
{
    s32 count = 0;
    s32 i = 0;

    do
    {
        if (D_80083E98[i][0] != 0)
        {
            count++;
        }
    } while (++i < 5);
    return count;
}

s32 func_8002219C(s32 idx)
{
    u8 name[0x20];
    s32(*tbl)[32];
    s32 v;

    func_80021B18(name, idx);
    func_80069C28(0);
    func_80069C28(1);
    func_80021538();
    func_8006A134(name);
    while ((v = func_800214FC()) == 0)
    {
        func_8005FE18(0);
    }
    if (idx < 5)
    {
        tbl = D_80083E98;
        tbl[idx][0] = 0;
        func_800220DC();
    }
    return v == 1;
}

void func_8002223C(void)
{
    func_8002219C(5);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002225C);

/* NEAR MISS, 15 words -- the right length and the right instruction set, with
   11 of 15 in the wrong order or register. Source in
   docs/near-misses/func_80022300.c. The target reads p[0] at word 4, between
   the two halves of `(n << 3) + 4`, keeps it in $v1 across the whole body and
   writes it back from the `jr` delay slot, and computes the return value
   `p + 0xC` in the middle at word 9; every spelling tried reads p[0] at word 7
   and computes the return value in the delay slot instead. Seven measured: an
   explicit temp for p[0] or the OR folded into it, the return value in a local
   declared first, the word store moved before or after the `p[0xA]` byte
   store, and `u32 *` against `s32 *` for it.

   The first parameter really is unused -- $a0 is overwritten by the 0xA04010
   constant at word 0 while $a1 and $a2 are read, so there are three
   parameters and the first is dead. This is very likely a PSY-Q primitive
   macro (the byte at 0x1 is 0x28, the byte at 0xB is 0x13, the word at 0x4 is
   0xA04010), in which case the shape comes from the macro rather than from
   anything spellable here. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022300);

void func_8002233C(u8 *p, s32 idx, s32 n)
{
    s32 i;

    func_8004E884(p, 1);
    p += idx * 12;
    for (i = 0; i < n; i++)
    {
        func_8004E264(p, 0);
        p += 12;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800223B4);

void func_80022488(s32 *p)
{
    UnkStrSet *t = D_800280B4;
    UnkStrSet *r = &t[p[2]];
    s32 n = r->unk_17 - 1;
    s32 x;
    s32 y;
    s32 z;

    if (r->unk_14 != 0)
    {
        func_8002225C(r->strs[n], p[0], &x, &y, &z);
        func_800223B4(p, x, y, n, z);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022524);

void func_800226A8(s32 *p)
{
    if (*(s32 *)p[8] == 0)
    {
        func_800231E4(0);
    }
}

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

/* The second parameter is unused by the body but func_80022AE8 sets $a1
   before the call and sets nothing else. */
/* NEAR MISS: 44 words against 46, and the two missing words are COPIES of hoisted constants: the target has 'addiu $t4, $zero, 5' then 'addu $t3, $t4, $zero', and the same pair for 6, where every spelling here CSEs the two 5s into one pseudo before loop.c sees them. Per docs/matching-levers.md:567, combine_movables matches two movables of equal value and mode and hoists one while the other becomes a copy -- so the target's two 5s survived cse1 as separate pseudos and this one's did not. The rest is right: an unaligned 4-byte copy of D_800200A0 to a stack buffer, then four iterations writing two 0xC-byte records each (byte 0 from the buffer, byte 1 = 0x48, word at 4 = 0x104058, bytes 0xA/0xB = 5 then 6), then 'p[-0x18] |= 0x80' and return p.
   Source in docs/near-misses/func_80022A30.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022A30);

void func_80022AE8(s32 *p)
{
    func_80022A30((u8 *)(p + 9), p);
    func_8002290C(p);
}

void func_80022B20(s32 a)
{
    func_800491F4(a, D_80027F60, 7);
}

/* NEAR MISS, parked at 40 words / 13 DIFF; source and the full measurement
   table are in docs/near-misses/func_80022B48.c, which is also the permuter's
   base (`tools/decomp/permimport.py func_80022B48`). Register allocation
   already matches; what is left is the schedule of the `ptrs[i] =
   &c->ents[i]` pair and, consequently, two register names. The five siblings
   are the same shape at the other five base offsets. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022B48);

/* NEAR MISS: 27 words against 24, and the register allocation ALREADY matches ($a3 maxlen, $t1 last, $a2 index, $t0 cursor). The three extra words are a duplicated inner-loop test: the target enters the inner loop with 'j' straight to the bottom test, the plain gcc 2.7.2 'while' shape, while every spelling here gets a guard copy of the test at the top AND the bottom one. Related, and probably the cause: 'last = i' is loop-invariant in the inner loop, and loop.c SINKS it out past the loop here (it lands in the bottom branch's delay slot, shared with the guard's target) where the target keeps it inside, in the if's delay slot. Three spellings measured: 'last = i' before the if, after it, and the if-body order swapped.
   Source in docs/near-misses/func_80022BE8.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022BE8);

void func_80022C48(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    p[5] = a;
    p[2] = b;
    p[3] = c;
    p[8] = d;
    func_80022BE8(&D_800280B4[b]);
}

void func_80022C90(s32 *p, s32 a, s32 b, s32 c, s32 d)
{
    func_80022C48(p, a, b, c, d);
    func_800226E0((UnkCardPair *)(p + 0x22F), (u32 *)(p + 0x225), (u32 *)(p + 0x229));
    func_80022AE8(p);
}

void func_80022CD8(s32 *p)
{
    func_80022774(p + 9, p);
    func_80022934(p);
    p[0] = D_800280B4[p[2]].unk_15;
    func_80022488(p);
    if (p[3] == 2)
    {
        p[-4] = (s32)func_800226A8;
    }
    else
    {
        p[-4] = (s32)func_80022524;
    }
}

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

/* The local copy of the parameter is load-bearing, and so is declaring the
   counter first: the target saves $s1 (the counter) before $s0 (the cursor)
   and fills the load-delay slot with the cursor's own increment. Walking the
   parameter directly puts $s0 first and leaves a nop there -- 6 differing. */
void func_80023194(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_8004E994(*p[0x233]);
        p++;
    } while (++i < 5);
}

s32 func_800231E4(s32 flag)
{
    s32 *p = &D_80028808;

    if (flag != 0)
    {
        ((u16 *)p[5])[15] |= 0x2000;
    }
    func_80023194((s32 **)p);
    ((u16 *)p)[-1] |= 0x8000;
    D_800814A0_a[0] |= 0x8000;
    return D_80028808;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023258);

void func_800233D0(s32 *p)
{
    s32 i;

    *(s32 *)p[0x68] = func_80049E6C(3);
    *(s32 *)p[0x69] = (s32)D_80077E84;
    for (i = 0; i < 3; i++)
    {
        *(s32 *)p[0x6A + i] = (s32)D_80077EF0;
        *(s32 *)p[0x6E + i] = (s32)D_80077EFC;
        *(s32 *)p[0x72 + i] = (s32)(p + 1);
        *(s32 *)p[0x76 + i] = (s32)((u8 *)p + 0x90 + i * 0x48);
    }
}

void func_80023470(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        ((UnkCardPair *)p[0x72 + i])->unk_04->unk_08 = 0x24;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[0] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[1] = 0x68;
        ((u8 *)((UnkCardPair *)p[0x72 + i])->unk_04)[2] = 0x60;
        ((UnkCardPair *)p[0x6A + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[0x6E + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[0x76 + i])->unk_04->unk_08 = 0x8F;
    }
}

void func_8002352C(u8 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004DA74(p + 0x90 + i * 0x48, D_800282BC[i], 1);
    }
}

s32 func_800235A0(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

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

s32 func_800237C4(s32 a)
{
    return func_800491F4(a, D_80027F68, 0x12);
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

s32 *func_800238D4(s32 *p)
{
    s32 *q = (s32 *)D_800291D0;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800291D0, 0x112);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0x428);
    }
    func_800237EC((s32)r, 0x12);
    q[3] = (s32)(r + 0x64);
    r[0x67] = func_800237C4((s32)(r + 0x68));
    func_80023894(r, (s32)p);
    q[4] = (s32)func_80023258;
    func_80023258(r);
    return q;
}

void func_800239B0(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_8004E994(*p[0x76]);
        p++;
    } while (++i < 3);
}

void func_80023A00(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800239B0((s32 **)q);
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

void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n)
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
    p->unk_04->unk_08 = -0xA0;
    p->unk_04->unk_0A = n * 24 - 0x78;
    p->unk_04->unk_0C = 0x200;
    p->unk_04->unk_0F = 4;
}

void func_80023B18(s32 *p)
{
    UnkCardPair *q = (UnkCardPair *)p[0x98];
    s32 k;

    k = func_80022138() >= 3;
    q->unk_00 = (s32)func_8004DA74((u8 *)p + 8, D_800282A4[k], 1);
    q->unk_04->unk_08 = 0xF0;
    q->unk_04->unk_0A = 0x91;
    q = (UnkCardPair *)p[0x9C];
    q->unk_04->unk_08 = 0x70;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_800791A4;
    q = (UnkCardPair *)p[0x9E];
    q->unk_00 = (s32)((u8 *)p + 0x134);
    q->unk_04->unk_08 = 0xAE;
    q->unk_04->unk_0A = 0x18;
}

s32 func_80023BE4(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023C44);

void func_80023D64(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x9D];
    q->unk_04->unk_08 = 0xFA;
    q->unk_04->unk_0A = 0x10;
    q->unk_00 = (s32)D_80027F7C;
    q = (UnkCardPair *)p[0x9A];
    q->unk_00 = (s32)func_8004DA74((u8 *)p + 0xA4, D_800282B0, 1);
    q->unk_04->unk_08 = 0x178;
    q->unk_04->unk_0A = 0x90;
    q = (UnkCardPair *)p[0x9F];
    q->unk_00 = (s32)((u8 *)p + 0x1AC);
    q->unk_04->unk_08 = 0x10C;
    q->unk_04->unk_0A = 0x18;
}

void func_80023E14(s32 *p)
{
    s32(*tbl)[32];

    func_80023BE4((s32)(p + 0x4D), 0x80, 0x18);
    func_80023BE4((s32)(p + 0x6B), 0x2A, 0x14);
    tbl = D_80083E98;
    if (tbl[p[0]][0] != 0)
    {
        func_80023C44(p);
        if (((s16 *)tbl[p[0]])[4] != 0)
        {
            func_80023D64(p);
        }
    }
    else
    {
        func_80023B18(p);
    }
}

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

void func_80023FA0(s32 *p, s32 a, s32 b)
{
    func_80023F94(p, a, b);
    func_80023A78((UnkCardPair *)(p + 0x93), (u32 *)(p + 0x89), (u32 *)(p + 0x8D), b);
    func_80023E14(p);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80023FF0);

void func_800240EC(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        func_8004E994(*(s32 *)p[off + 0x98]);
        *(s32 *)p[off + 0x98] = 0;
        off++;
    } while (++i < 3);
    *(s32 *)p[0x9B] = 0;
}

void func_80024160(s32 **p)
{
    s32 i = 0;

    do
    {
        *p[0x97] = 0;
        p++;
    } while (++i < 10);
}

void func_80024184(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_800241D4(s32 *p, s32 flag)
{
    s32 *q;
    s32 *r;

    if (p != 0)
    {
        q = p + 8;
        r = (s32 *)q[0xA0];
        q[0x89] = 0x808080;
        if (flag != 0)
        {
            r[0] = (s32)D_800786E8;
        }
        else
        {
            r[0] = 0;
        }
        ((u16 *)r[1])[4] = 0x60;
        ((u16 *)r[1])[5] = 0x18;
    }
}

void func_80024224(s32 *p, s32 flag)
{
    s32 *q;
    s32 *r;

    if (p != 0)
    {
        q = p + 8;
        r = (s32 *)q[0xA0];
        q[0x89] = 0x808080;
        if (flag != 0)
        {
            r[0] = (s32)D_800786DC;
        }
        else
        {
            r[0] = 0;
        }
        ((u16 *)r[1])[4] = 0x66;
        ((u16 *)r[1])[5] = 0x18;
    }
}

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

void func_80024298(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_800240EC(q);
        func_80024160((s32 **)q);
        func_80023E14(q);
    }
}

/* NEAR MISS: 46 words, 10 DIFF: the halved width and height must be computed INSIDE the loop (they are loop-invariant, so loop.c hoists them, and hoisting puts them AFTER the three constant movables where the target has them) -- that took it from 24 DIFF to 10. What is left is register naming around those two divisions. The shape is right: a 0xC-byte header, four 0x18-byte records (byte 1 = 0x38, three words of 0x9CBCBC then one of 0x8C9C9C, bytes 0xA/0xB the halved extents, byte 0 from D_80027FD0[i]), then a 0xC-byte trailer, returning p + 0xC.
   Source in docs/near-misses/func_800242D8.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800242D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200A0);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024390);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80024818);

void func_800249B4(s32 *p)
{
    s32 i;

    func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
    func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
    for (i = 0; i < 2; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[0x2D3 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + i * 0x180);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + i * 0x10;
    }
}

void func_80024A50(s32 *p)
{
    s32 n = p[0x1F];

    if (D_80083E98[n][0] != 0)
    {
        func_80024390(p);
    }
    else
    {
        s32 m = p[0x20];

        if (m != 0 && m != 2)
        {
            func_80024818(p);
        }
        else
        {
            func_800249B4(p);
        }
    }
}

void func_80024ACC(s32 *p)
{
    UnkCardPair *q;

    func_800242D8((u8 *)(p + 1), 0xF8, 0x48);
    q = (UnkCardPair *)p[0x2D2];
    q->unk_04->unk_08 = 0xA0;
    q->unk_04->unk_0A = 0xB0;
    q->unk_00 = (s32)(p + 1);
    q = (UnkCardPair *)p[0x2CF];
    q->unk_04->unk_08 = 0x60;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079054;
    q = (UnkCardPair *)p[0x2D0];
    q->unk_04->unk_08 = 0xD0;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_800790B4;
    q = (UnkCardPair *)p[0x2D1];
    q->unk_04->unk_08 = 0x20;
    q->unk_04->unk_0A = 0x88;
    q->unk_00 = (s32)D_80079114;
}

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

void func_80024E44(s32 *p)
{
    s32 i;

    for (i = 0; i < 3; i++)
    {
        func_8004E994(*(s32 *)p[0x2D3 + i]);
        *(s32 *)p[0x2D3 + i] = 0;
    }
    for (i = 0; i < 4; i++)
    {
        *(s32 *)p[0x2D6 + i] = 0;
    }
    for (i = 0; i < 10; i++)
    {
        *(s32 *)p[0x2DA + i] = 0;
    }
}

void func_80024EEC(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_80024E44(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

void func_80024F3C(s32 *p, s32 a, s32 b)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[0x1F] = a;
        q[0x20] = b;
        func_80024E44(q);
        func_80024A50(q);
    }
}

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

void func_80024FFC(s32 *p)
{
    func_80025030(p);
    p[-4] = (s32)func_8002593C;
}

/* NEAR MISS: 46 words, 4 DIFF, register naming only: the target materialises the comparison's 1 into $v0 and COPIES it into $s3 for the flag ('addu $s3, $v0, $zero', which reorg then puts in the beq's delay slot), while every spelling here puts the 1 straight into $s3 and compares against that. Four measured: flag initialised at its declaration or assigned before the if, the test as == with the 1-assignment in the then arm, as != with it in the else, and a shared 'one' local. Initialising the loop counter and cursor AFTER the if is what took it from 13 DIFF to 4 and must be kept.
   Source in docs/near-misses/func_80025030.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025030);

void func_800250E8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_80024224((s32 *)p[off + 1], 1);
        }
        else if (i == p[10])
        {
            func_800241D4((s32 *)p[off + 1], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 1]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[7], p[10], p[0x11]);
}

/* Empty, but it takes an argument: func_80025230 sets $a0 to the bare
   0x80010000 base before calling it and sets nothing else. A callee that
   never reads its parameter compiles identically either way, so this is the
   caller's requirement, not the callee's. */
void func_8002519C(u8 *p)
{
}

void func_800251A4(s32 *p)
{
    u16 *b = (u16 *)0x80010000;
    s32 v;

    func_80027BF4();
    v = 1;
    if (p[0x11] == 0)
    {
        v = 3;
    }
    b[0x1B8A] = v;
    D_80082E60.unk_16 |= 2;
    func_80025D34(p - 8);
    func_800439F8();
    func_80040AA0(b[0x104] != 0 ? 5 : 6);
}

void func_80025230(s32 *p)
{
    if (func_80021FF0(p[10], (u8 *)0x80010000, 1) == 0)
    {
        p[13] = (s32)func_8002593C;
        func_80023004((s32)(p - 8));
        p[-4] = (s32)func_80024F7C;
    }
    else
    {
        func_8002519C((u8 *)0x80010000);
        p[-4] = (s32)func_800251A4;
    }
    func_80020984();
    p[16] = 0;
}

void func_800252A4(s32 *p)
{
    s32 n = p[15] - 1;

    p[15] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80025230;
    }
}

void func_800252CC(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[15] = 1;
        func_80022F8C((s32)(p - 8), (s32)(p + 16));
        p[16] = 1;
        p[-4] = (s32)func_800252A4;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025358(s32 *p, s32 v)
{
    func_80025F0C(p, v);
    func_80025FFC(p, v);
}

/* NEAR MISS: 53 words and exactly ONE differing word: the target fills func_80025358's jal delay slot with a DEAD 'lui $a0, 0x8001' (overwritten by the next instruction's 'lw $a0, 0x28($s2)'), where every spelling here fills it with a second copy of 'addiu $a0, $s2, -0x20'. Both are reorg picking the next instruction in the stream; the target's stream had the 0x80010000 materialisation there and this one's had the fail-path argument. Four measured: the constant in a local against inline at all three uses, and 'p - 8' hoisted into a local (54w/50, much worse). Everything else matches, including the goto structure -- the two failure exits really do converge on one block and then on the shared tail.
   Source in docs/near-misses/func_80025398.c. */
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

void func_80025494(s32 *p)
{
    p[15] = 1;
    func_800230CC((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_8002546C;
}

void func_800254E4(s32 *p)
{
    s32(*tbl)[32] = D_80083E98;

    if (tbl[p[10]][0] == 0)
    {
        if (func_80022138() < 3)
        {
            p[13] = (s32)func_80024FFC;
            func_80022FDC((s32)(p - 8));
            p[-4] = (s32)func_80024FAC;
        }
        else
        {
            func_80025494(p);
        }
    }
    else
    {
        p[13] = (s32)func_80024FFC;
        func_8002311C((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025584);

void func_8002570C(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[11] = n;
        func_800250E8(p);
        p[-4] = (s32)func_80025584;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_80025788(s32 *p)
{
    s32 i;
    s32 off;

    if (p[14] != 0)
    {
        p[-4] = (s32)func_8002593C;
    }
    else
    {
        if (func_8002219C(p[10]) != 0)
        {
            p[-4] = (s32)func_8002593C;
            off = 0;
            for (i = 0; i < 5; i++)
            {
                func_80024298((s32 *)p[off + 1]);
                off++;
            }
        }
        else
        {
            p[13] = (s32)func_8002593C;
            func_8002316C((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
        func_80020984();
        p[16] = 0;
    }
}

void func_80025848(s32 *p)
{
    s32 n = p[10];

    if (D_80083E98[n][0] != 0)
    {
        p[13] = (s32)func_80025788;
        func_80023144((s32)(p - 8), (s32)(p + 14));
        p[-4] = (s32)func_80024FAC;
    }
    else
    {
        p[13] = (s32)func_8002593C;
        func_800230A4((s32)(p - 8));
        p[-4] = (s32)func_80024FAC;
    }
}

void func_800258C8(s32 *p)
{
    void (*fns[4])(void) = {func_800252CC, func_8002570C, func_80025848, func_800252CC};

    fns[p[0x11]]();
}

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

void func_80025B60(s32 *p)
{
    func_80025030(p);
    func_80020984();
    p[-4] = (s32)func_8002593C;
}

void func_80025B9C(s32 *p)
{
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

void func_80025BD4(s32 *p)
{
    s32 n = p[9];

    if (n < 5)
    {
        if (func_80021D54(n) != 0)
        {
            p[n + 1] = func_80023FF0((s32)(p - 8), n);
            p[9] = p[9] + 1;
        }
        else
        {
            p[13] = (s32)func_80025B9C;
            func_80023004((s32)(p - 8));
            p[-4] = (s32)func_80024F7C;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80025B60;
    }
}

s32 *func_80025C7C(s32 *p)
{
    s32 *q = func_8003FE78(0, D_8002B7E0, 0x1C);
    s32 *r = q + 8;

    func_80025B40(r, (s32)p);
    func_8002223C();
    func_800220DC();
    r[0x12] = 1;
    q[8] = func_8004F418(q, q + 0x1A);
    r[6] = (s32)func_800238D4(q);
    r[7] = func_80024D58(q, r[10], 0);
    r[9] = 0;
    q[4] = (s32)func_80025BD4;
    return q;
}

/* NEAR MISS, 36 words against 37, parked. Source in
   docs/near-misses/func_80025D34.c. The missing word is a delay-slot nop: the
   target leaves BOTH of the first two loop calls' slots empty and fills only
   the third, with the counter increment, while every spelling tried lets
   reorg fill the first one too. Seven measured, all 36 words: q[0] or p[8]
   for the leading call, counter declared before or after the cursor, `i = 0`
   at the declaration or in the body, `++i` in the condition against `i++` at
   the top or bottom of the body, and a `for` loop. The cursor's base also
   differs -- the target walks from p+0x20 with displacements 0x18/0x1C/0x04,
   this walks from p+0x24. func_800267C0 is its sibling and will follow it. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025D34);

void func_80025DC8(s32 *p)
{
    s32 off = 0;
    s32 i = 0;

    do
    {
        if (i == p[11])
        {
            func_800241D4((s32 *)p[off + 3], 1);
        }
        else
        {
            func_80024274((s32 *)p[off + 3]);
        }
        off++;
    } while (++i < 5);
    func_80024F3C((s32 *)p[1], p[11], 3);
}

s32 func_80025E54(void)
{
    s32 mask = 0;
    s32 i = 0;

    do
    {
        if (func_80033B2C(D_80028010[i]) != 0)
        {
            mask += 1 << i;
        }
    } while (++i < 8);
    return mask;
}

/* The byte table is NOT the symbol splat prints. The listing reads
   `lui $v1, %hi(D_800133A4)` / `lbu $v0, %lo(D_800133A4)($v1)` with `$v1`
   incremented by 2 each iteration -- a %lo in the displacement with a
   REGISTER index, which per docs/matching-levers.md:1870 cannot be a symbol
   reference at all: cse folds a %lo into a MEM only when what is left is a
   constant offset off the `(high)` pseudo. It is the constant base
   0x80010000, whose low half is zero so it needs a bare `lui`, with 0x33A4
   written inside the index expression. splat prints the displacement
   symbolically because 0x800133A4 happens to be in its symbol list.

   Four symbol spellings cost the extra `addiu` the target does not have, all
   17 words against 16: `extern u8 D_800133A4[]` indexed, the same as a
   scalar through `(&D_800133A4)[k]`, a pointer walked by 2, and
   `*((u8 *)0x800133A4 + i * 2)` -- the last one being a constant too, but
   with a NONZERO low half, so it still has to be materialised. The zero low
   half of 0x80010000 is the whole point. */
s32 func_80025ECC(void)
{
    u8 *b = (u8 *)0x80010000;
    s32 sum = 0;
    s32 i;

    for (i = 0; i < 0x22; i++)
    {
        sum += D_80028020[b[i * 2 + 0x33A4]];
    }
    return sum;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025F0C);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025FFC);

void func_80026128(u8 *p, s32 a, s32 mode)
{
    u8 *b = (u8 *)0x80010000;
    s32 f;

    *(UnkFour *)p = *(UnkFour *)D_80028064;
    f = (mode ^ 2) == 0;
    *(s32 *)(b + 0x224) = *(s32 *)(b + 0x2D5C);
    *(s16 *)(b + 0x208) = f;
    *(s16 *)(b + 0x20A) = f;
    *(s32 *)(b + 0x230) = func_80025E54();
    *(s32 *)(b + 0x228) = func_80025ECC();
    func_80025F0C((s32 *)p, a);
    func_80025FFC((s32 *)p, a);
}

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

void func_800261F0(s32 *p)
{
    func_800261C0(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_800265B8;
    }
}

void func_80026240(s32 *p)
{
    func_80026920(p - 8);
    func_80027BF4();
    func_80040AA0(3);
}

void func_80026270(s32 **pp)
{
    s32 i = 0;
    s32 **p = pp;

    do
    {
        func_80024298((s32 *)p[3]);
        p++;
    } while (++i < 5);
}

/* NEAR MISS: 55 words, 3 DIFF: in the failure block the target computes the call's argument 'addiu $a0, $s0, -0x20' BEFORE finishing the func_800265B8 address and schedules the 'sw $v0, 0x34($s0)' into the call's delay slot; every spelling here emits the store before the argument. Four measured: the two branch senses (== 0 first is right -- it took this from 28 DIFF to 3), the argument hoisted into a local inside the block, and the store moved after the call (56w/45). Everything else matches, including the 0x80010000 constant base for both the D_80010208 halfword read and the two pointer arguments.
   Source in docs/near-misses/func_800262BC.c. */
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

void func_800263C0(s32 *p)
{
    p[15] = 1;
    func_80022F64((s32)(p - 8), (s32)(p + 16));
    p[16] = 1;
    p[-4] = (s32)func_80026398;
}

void func_80026410(s32 *p)
{
    if (p[14] != 0)
    {
        p[-4] = (s32)func_800265B8;
    }
    else
    {
        func_800263C0(p);
    }
}

void func_8002644C(s32 *p)
{
    p[13] = (s32)func_80026410;
    func_80022F3C((s32)(p - 8), (s32)(p + 14));
    p[-4] = (s32)func_800261F0;
}

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

void func_8002651C(s32 *p)
{
    if (D_80083E98[p[11]][0] != 0)
    {
        p[-4] = (s32)func_8002644C;
    }
    else if (func_80022138() >= 3)
    {
        p[-4] = (s32)func_800263C0;
    }
    else if (func_80022160() != 0)
    {
        func_80026494(p);
    }
    else
    {
        func_800264D8(p);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800265B8);

void func_80026720(s32 *p, s32 v)
{
    p[8] = v;
}

void func_80026728(s32 *p, s32 v)
{
    func_80026720(p, v);
}

void func_80026748(s32 *p)
{
    func_80025DC8(p);
    func_80020984();
    p[-4] = (s32)func_800265B8;
}

void func_80026784(s32 *p)
{
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

void func_800267C0(s32 *p)
{
    s32 n = p[10];

    if (n < 5)
    {
        if (func_80021D54(n) != 0)
        {
            p[n + 3] = func_80023FF0((s32)(p - 8), n);
            p[10] = p[10] + 1;
        }
        else
        {
            p[13] = (s32)func_80026784;
            func_80023004((s32)(p - 8));
            p[-4] = (s32)func_800261C0;
        }
    }
    if (n == 5)
    {
        p[-4] = (s32)func_80026748;
    }
}

s32 *func_80026868(s32 *p, s32 a)
{
    s32 *q = func_8003FC64(0);
    s32 *r;

    if (q != 0)
    {
        r = q + 8;
        r[9] = a;
        func_80026728(r, (s32)p);
        func_8002223C();
        func_800220DC();
        q[8] = func_8004F418(q, 0);
        r[1] = func_80024D58(q, r[11], 3);
        r[2] = (s32)func_80027364(q);
        r[10] = 0;
        q[4] = (s32)func_800267C0;
    }
    return q;
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026920);

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_800269B4);

void func_80026AE4(s32 *p)
{
    func_8004CB2C(p + 1);
    func_8004CB2C(p + 4);
}

/* NEAR MISS: 56 words against 58: the target carries TWO extra induction variables -- a byte cursor advancing 0xC from p and a separate 0xC-stride offset starting at 4 -- where the 'i * 0xC + 4' and 'i * 0xC + 8' forms here get folded into one. Two measured: the folded form (56w/51) and explicit cursors advanced at the bottom of the loop (62w/53, worse). The shape is right: two iterations, each testing c[i] then b[i], calling func_8004CBFC(p + 0xC*i + 4, D_80027E68) on the true side and clearing both the UnkCardPair at p[0x1A + i] and the word at p + 0xC*i + 8 on the false one, with 'any = 1' set unconditionally once c[i] is non-zero and func_800269B4 called at the end if it was.
   Source in docs/near-misses/func_80026B18.c. */
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

void func_80026C94(s32 *p)
{
    UnkCardPair *q;

    q = (UnkCardPair *)p[0x18];
    q->unk_04->unk_08 = 0x40;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028418;
    q = (UnkCardPair *)p[0x19];
    q->unk_04->unk_08 = 0xC0;
    q->unk_04->unk_0A = 0x20;
    q->unk_00 = (s32)D_80028430;
}

/* NEAR MISS: 34 words, 11 DIFF: $s0 and $s1 are swapped -- the target keeps the parameter in $s1 and the D_80027E68 base in $s0, every spelling here does the reverse. Three measured (the table inline at both call sites, hoisted into a local declared before or after the UnkCardPair temp, and the parameter copied to a local). Allocation only; the instruction sequence is identical.
   Source in docs/near-misses/func_80026CF0.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026CF0);

void func_80026D78(s32 *p)
{
    func_80026C94(p);
    func_80026CF0(p);
    func_80026B18((u8 *)p, (s32 *)D_8002807C, (s32 *)D_80028084);
    func_800269B4(p);
}

s32 func_80026DC8(s32 a)
{
    return func_800491F4(a, D_8002808C, 4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80026DF0);

void func_80026E90(s32 *p, s32 v)
{
    p[0] = v;
    func_80026C00((UnkCardPair *)(p + 0x14), (u32 *)(p + 0xA), (u32 *)(p + 0xE));
    func_80026D78(p);
}

s32 *func_80026ED0(s32 *p, s32 a)
{
    s32 *q = (s32 *)D_8002B850;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B850, 0x45);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80026FB4((u16 *)q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0xF0);
    }
    func_80026DF0((s32)r, 4);
    q[3] = (s32)(r + 0x14);
    r[0x17] = func_80026DC8((s32)(r + 0x18));
    r[9] = a;
    func_80026E90(r, (s32)p);
    q[4] = (s32)func_80026AE4;
    return q;
}

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

void func_80026FF8(s32 *p, s32 *b, s32 *c)
{
    func_80026B18((u8 *)(p + 8), b, c);
}

void func_80027018(s32 *p)
{
    *(s32 *)p[0x3C] = (s32)D_80077EF0;
    *(s32 *)p[0x3D] = (s32)D_80077EFC;
    *(s32 *)p[0x3E] = (s32)(p + 1);
    *(s32 *)p[0x3F] = (s32)(p + 0x1F);
}

void func_80027054(u8 *p)
{
    func_8004DA74(p + 0x7C, D_800282CC, 1);
}

s32 func_80027080(s32 p, s32 w, s32 h)
{
    s16 r[4];

    r[0] = -w / 2;
    r[1] = -h / 2;
    r[2] = w;
    r[3] = h;
    func_80049D80(p, r);
    return p;
}

void func_800270E0(s32 *p)
{
    func_80027054((u8 *)p);
    func_80027080((s32)(p + 1), 0x36, 0x10);
    func_80027018(p);
    ((UnkCardPair *)p[0x3F])->unk_04->unk_08 = 0x8E;
    ((UnkCardPair *)p[0x3F])->unk_04->unk_0A = 0x82;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_08 = 9;
    ((UnkCardPair *)p[0x3C])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_08 = 0x42;
    ((UnkCardPair *)p[0x3D])->unk_04->unk_0A = 0xA;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_08 = 0x24;
    ((UnkCardPair *)p[0x3E])->unk_04->unk_0A = 0xA;
}

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

s32 func_80027254(s32 a)
{
    return func_800491F4(a, D_80028094, 4);
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

s32 *func_80027364(s32 *p)
{
    s32 *q = (s32 *)D_8002B968;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002B968, 0x68);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80023A00(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0x180);
    }
    func_8002727C((s32)r, 4);
    q[3] = (s32)(r + 0x38);
    r[0x3B] = func_80027254((s32)(r + 0x3C));
    func_80027324(r, (s32)p);
    return q;
}

void func_8002742C(s32 *p)
{
    func_8004E994(*(s32 *)p[0x3F]);
}

void func_80027458(s32 *p)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        func_8002742C(q);
        ((u16 *)q)[-1] |= 0x8000;
        D_800814A0_a[0] |= 0x8000;
    }
}

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

void func_800274D8(s32 *p)
{
    func_800274A8(p);
    if (func_8002168C() >= 4)
    {
        func_800231E4(0);
        p[-4] = (s32)func_8002789C;
    }
}

/* NEAR MISS: 23 words, 2 DIFF, register names only: the target holds the tested p[0] in $a1 and reuses it for the call's second argument, this holds it in $v0 and moves zero into $a1. Two spellings measured (the if reversed, a local temp), 2 and 4 DIFF.
   Source in docs/near-misses/func_80027528.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80027528);

void func_80027584(s32 *p)
{
    s32 i = 0;
    s32 r;

    do
    {
        r = func_80021A84(p[3]);
        if (r != 5)
        {
            break;
        }
    } while (i++ < 6);
    p[14] = 0;
    func_80020984();
    if (r == 1)
    {
        p[11] = (s32)func_8002789C;
        func_80023054((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
    else if (r == 4)
    {
        p[11] = (s32)func_8002789C;
        func_80022E9C((s32)(p - 8));
        p[-4] = (s32)func_800274A8;
    }
    else
    {
        p[11] = (s32)func_8002789C;
        func_8002302C((s32)(p - 8));
        p[-4] = (s32)func_800274D8;
    }
}

void func_8002765C(s32 *p)
{
    s32 n = p[16] - 1;

    p[16] = n;
    if (n < 0)
    {
        p[-4] = (s32)func_80027584;
    }
}

void func_80027684(s32 *p)
{
    s32 h;

    if (p[15] != 0)
    {
        h = (s32)func_8002789C;
    }
    else
    {
        p[16] = 1;
        func_80022F14((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
        h = (s32)func_8002765C;
    }
    p[-4] = h;
}

void func_800276EC(s32 *p)
{
    p[11] = (s32)func_80027684;
    func_80022E74((s32)(p - 8), (s32)(p + 15));
    p[-4] = (s32)func_800274D8;
}

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

void func_800277BC(s32 *p)
{
    s32 n;
    s32 h;

    if (D_80083160.unk_08 != 0 && (D_80083160.unk_10 & 0x20) != 0)
    {
        func_800209C4();
        p[14] = 0;
        p[-4] = (s32)func_8002789C;
        return;
    }
    n = func_8002168C();
    func_80021904();
    if (n != 0)
    {
        p[14] = 0;
        p[12] = n;
        h = D_8002809C[n];
        p[13] = n;
        p[-4] = h;
        return;
    }
    if (func_800210E0() == 3 && p[14] == 0)
    {
        func_80022EC4((s32)(p - 8), (s32)(p + 14));
        p[14] = 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002789C);

void func_80027A68(s32 *p, s32 a, s32 b)
{
    p[5] = a;
    p[0] = b;
    p[3] = func_8002190C();
}

void func_80027A9C(s32 *p)
{
    s32 *q = p - 8;

    p[8] = func_8004F418(q, p + 3);
    p[9] = (s32)func_80026ED0(q, p[3]);
    func_800209C4();
    p[-4] = (s32)func_8002789C;
}

s32 *func_80027AFC(s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002BB08;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002BB08, 0x19);
    }
    else
    {
        func_80027C90(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0x44);
    }
    D_80082E60.unk_0F |= 0x80;
    func_80027A68(r, a, b);
    q[4] = (s32)func_80027A9C;
    return q;
}

void func_80027BC0(void)
{
    func_8006A084();
    func_8006A248(0xF2000002);
    func_8006A094();
}

void func_80027BF4(void)
{
    D_80080A84 = 2;
    func_80020F18();
    func_800585A0();
    if (D_80082E6A == 1)
    {
        func_80053DA8(0x200);
    }
    else
    {
        D_80080A88 = 0;
    }
    D_80082E60.unk_0F &= 0x7F;
}

void func_80027C60(void)
{
    func_8005863C();
    D_80080A84 = 1;
    func_80020EA0();
}

void func_80027C90(s32 *p)
{
    s32 *q = p + 8;

    func_8004F52C(q[8]);
    func_80026FB4((u16 *)q[9]);
    if (p[8] == 2)
    {
        func_800A6104();
    }
    ((u16 *)p)[15] |= 0x8000;
    D_800814A0 |= 0x8000;
}

void func_80027D0C(s32 *p)
{
    func_80027C60();
    func_80027AFC((s32)p, 1);
}

void func_80027D40(s32 *p)
{
    func_800A68C0();
    func_80027C60();
    func_80027AFC((s32)p, 2);
}

void func_80027D7C(s32 *p)
{
    func_80027C60();
    D_80083200 = 0x200;
    func_80064F20(0x200);
    func_80064F00(0xA0, 0x78);
    func_80027AFC((s32)p, 0);
}

s16 func_80027DD0(void)
{
    return func_8004CAA0();
}

void func_80027DF4(s32 *p, s32 v)
{
    s32 *q;

    if (p != 0)
    {
        q = p + 8;
        q[3] = v;
        func_80026FD4((s32 *)q[9], v);
    }
}

/* D_800200A8 is func_800258C8's own local array initialiser, which gcc now
   emits into this unit's .rodata, so it must NOT also be pulled in by
   INCLUDE_RODATA -- doing both put 0x10 extra bytes of rodata ahead of the
   text and moved every function from func_800210F0 on by 0x10, which the
   function-pointer tables at D_80020034 and D_80020048 then disagreed with.
   Each per-function ovlverify check still passed; only `make` caught it. */

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_800200B8);
