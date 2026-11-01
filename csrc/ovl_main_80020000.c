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
s32 func_80022D60(s32 a, s32 id, s32 kind, s32 p);

extern s32 D_800287C8;
extern s32 D_800287CC;
extern s32 D_800287E4;
/* 128 bytes per entry; func_80021D38's `sll $a0, $a0, 7` is the stride and
   the 2D array spelling is what produces the lui/addiu pair plus the shift,
   where a scalar would fold differently. The element type is a guess that
   only has to make offset 0 a word. */
extern s32 D_80083E98[][32];
extern s32 D_80083EA4[][32];
extern u8 D_80078FD0[];
void func_8004E5A0(s32 v, s32 n, u8 *out);
void func_8004E69C(u8 *q);
s8 *func_8004E298(u8 *a, u8 *b, s32 c);
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

/* Eight and 0x20 bytes, alignment 1, for the same reason as UnkFour:
   func_80025FFC copies D_800200B8 onto its stack and a D_80027E78 row into
   its argument with lwl/lwr + swl/swr. */
typedef struct
{
    u8 b[8];
} UnkEight;

typedef struct
{
    u8 b[0x20];
} UnkBlock20;

/* 24 bytes, alignment 1: func_80022774 copies a D_80027F48 template into its
   third primitive with lwl/lwr + swl/swr. */
typedef struct
{
    u8 b[24];
} UnkBlock24;

/* Two words, alignment 4: func_8002789C copies the pair func_800217D4
   returns into p[6]/p[7] with gcc's aligned 8-byte block move (lw/lw/sw/sw);
   as two field assignments it is one word longer and interleaved. */
typedef struct
{
    s32 a;
    s32 b;
} UnkTwoWords;

/* The u16 pair at the front of func_80025FFC's stack copy: the loads are
   `lhu`, not `lh`, so the fields are unsigned. */
typedef struct
{
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} UnkURect;

/* 0x80 bytes, alignment 1: func_80021DF4 copies a whole D_80083E98 row with
   gcc's unknown-alignment block move -- the `(src | dst) & 3` test and both a
   lwl/lwr and a lw loop. Through the s32[][32] type gcc would know both sides
   are word-aligned and emit only the second. */
typedef struct
{
    u8 b[0x80];
} UnkRow;

/* Five function pointers, 20 bytes: func_80021594 block-copies both
   D_80020034 and D_80020048 onto its stack and dispatches through the copy.
   It has to be a struct COPY from the extern, not a local array
   INITIALISER: gcc emits a function's own constants into .rodata right after
   that function, and func_80021594 sits BEFORE this unit's first
   INCLUDE_RODATA line, so an initialiser lands at 0x80020000 and pushes
   every other rodata block down. (func_800258C8's initialiser works only
   because that function sits where its constant belongs.) */
typedef struct
{
    s32 (*f[5])(void);
} UnkFnTab5;

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
UnkNode *func_80044144(s16 a, s16 b, u32 c, u32 d);
void func_800A68F4(void);
void func_8005863C(void);
void func_8006A084(void);
extern u8 D_80020000[];
extern u8 D_80020028[];
s32 func_8006A004(s32 ev);
s32 func_80069FF4(u32 cls, u32 spec, s32 mode, void *fn);
s32 func_8006A034(s32 ev);
void func_80069F18(u8 *fmt, u8 *file, s32 line);
void func_80069DF8(s32 code) __attribute__((noreturn));
void func_8006A094(void);
void func_8006A248(u32 ev);
s32 *func_80027AFC(s32 a, s32 b);
u8 *func_80022A30(u8 *p, s32 *q);
void func_80021538(void);
void func_80025D34(s32 *p);
void func_80023E14(s32 *p);
void func_80025F0C(u8 *p, s32 n);
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
u8 *func_80022774(u8 *p, s32 *q);
void func_80022934(s32 *p);
void func_80022488(s32 *p);
void func_80022524(s32 *p);
void func_8004CBFC(u32 *p, u8 *q, u32 *r);
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
void func_80024C70(s32 a, s32 n);
void func_8003AD08(s32 n, u8 *buf);
s32 func_800210F0(void);
extern UnkFnTab5 D_80020034;
extern UnkFnTab5 D_80020048;
s32 func_80021298(void);
extern u8 *D_80028294[];
extern u8 *D_800282A0;
/* func_80024D58 calls func_80024C48 with ONLY $a0 set, where the function
   itself takes five arguments (func_80022C90 passes all five). The one-argument
   call cannot be written through the real prototype, so it goes through an
   `__asm__`-aliased declaration, the same device csrc/gprel_72A8.c uses for
   func_80033AE8w. */
s32 func_80024C48w(s32 *p) __asm__("func_80024C48");
extern u8 D_8002A990[];
u8 *func_800242D8(u8 *p, s32 w, s32 h);
extern u8 D_8002B850[];
extern u8 D_80079054[];
extern u8 D_800790B4[];
extern u8 D_80079114[];
extern s32 D_8002809C[];
extern u8 D_80028068[][2];
extern u8 D_80027E78[][0x20];
extern u8 D_800200B8[];
s32 func_80049DE8(s32 a, s32 b, s32 c);
s32 func_80049E1C(s32 a, s32 d, s32 m);
void func_8002651C(s32 *p);
void func_8006733C(UnkURect *r, u8 *src);
void func_80067014(s32 n);
extern s32 D_8002806C[];
extern u8 *D_80028074[];
extern u8 D_800287E8[];
extern u8 D_800291D0[];
extern u8 D_80029618[][0x3E4];
extern u8 *D_800282A4[];
extern u8 D_800791A4[];
void func_8002727C(s32 a, s32 n);
s16 func_80027DD0(void);
extern u8 D_8002BB08[];
extern u8 D_8002B968[];
s32 *func_80027364(s32 *p);
s32 *func_800238D4(s32 *p);
s32 *func_80024D58(s32 *p, s32 a, s32 b);
extern u8 D_80027FD0[];
extern u8 D_8002B7E0[];
s32 func_8006A0B4(s32 fd, s32 off, s32 whence);
s32 func_8006A0C4(s32 fd, u8 *buf, s32 len);
s32 func_8006A0E4(s32 fd);
extern u8 D_800200A0[];
extern u8 D_80027F48[];
void func_80023B18(s32 *p);
extern u8 D_80027F7C[];
extern u8 D_800282B0[];
extern u8 D_80077E84[];
void func_800223B4(s32 *p, s32 x, s32 y, s32 n, s32 z);
s32 func_80021B98(u8 *name, u8 *buf, s32 n, s32 off);
extern u8 D_80028064[];
extern u8 *D_800283B8[];
extern u8 D_800283C4[];
extern u8 *D_800283EC[];
extern u8 *D_800283F4[];
extern u8 D_800283FC[];
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
void func_80025584(s32 *p);
extern s16 D_80028010[];
void func_80020A80(void);
void func_80069EC8(void *p, s32 n);
void func_8004E884(u8 *p, s32 a);
void func_8004E264(u8 *p, s32 a);
extern s32 D_80084118;
extern s32 D_80028530;
extern s32 D_80028538;
extern s32 D_800287D0;
extern s32 D_80028540;
extern s32 D_80028548;
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
void func_80024C70(s32 a, s32 n);
void func_8003AD08(s32 n, u8 *buf);
s32 func_800210F0(void);
extern UnkFnTab5 D_80020034;
extern UnkFnTab5 D_80020048;
s32 func_80021298(void);
extern u8 *D_80028294[];
extern u8 *D_800282A0;
/* func_80024D58 calls func_80024C48 with ONLY $a0 set, where the function
   itself takes five arguments (func_80022C90 passes all five). The one-argument
   call cannot be written through the real prototype, so it goes through an
   `__asm__`-aliased declaration, the same device csrc/gprel_72A8.c uses for
   func_80033AE8w. */
s32 func_80024C48w(s32 *p) __asm__("func_80024C48");
extern u8 D_8002A990[];
u8 *func_800242D8(u8 *p, s32 w, s32 h);
extern u8 D_8002B850[];
extern u8 D_80079054[];
extern u8 D_800790B4[];
extern u8 D_80079114[];
s32 func_800231E4(s32 flag);
void func_80024ACC(s32 *p);
void func_800270E0(s32 *p);

void func_80025230(s32 *p);
void func_800265B8(s32 *p);
void func_80027584(s32 *p);
void func_8002789C(s32 *p);
void func_8002593C(s32 *p);
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

/* c_server.c's event setup, and the overlay's second largest function: eight
   func_80069FF4 (OpenEvent) calls inside the func_8006A084 / func_8006A094
   critical section, each asserting the handle is not -1, then eight
   func_8006A034 (EnableEvent) calls each asserting a non-zero result. Source
   lines 185..200 and 212..226.

   It falls out whole once func_80069DF8 is NORETURN (see func_80020F18): each
   of the sixteen %hi values then stays live in its own callee-saved register
   across the calls, which is why the function saves $s0..$s7 and $fp, and
   $s0 holds the -1 the first eight comparisons share. */
void func_80020A80(void)
{
    func_8006A084();
    D_80028550 = func_80069FF4(0xF4000001, 4, 0x2000, 0);
    if (D_80028550 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 185);
        func_80069DF8(1);
    }
    D_80028554 = func_80069FF4(0xF4000001, 0x8000, 0x2000, 0);
    if (D_80028554 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 187);
        func_80069DF8(1);
    }
    D_80028558 = func_80069FF4(0xF4000001, 0x100, 0x2000, 0);
    if (D_80028558 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 189);
        func_80069DF8(1);
    }
    D_8002855C = func_80069FF4(0xF4000001, 0x2000, 0x2000, 0);
    if (D_8002855C == -1)
    {
        func_80069F18(D_80020000, D_80020028, 191);
        func_80069DF8(1);
    }
    D_80028560 = func_80069FF4(0xF0000011, 4, 0x1000, (void *)func_80020A30);
    if (D_80028560 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 194);
        func_80069DF8(1);
    }
    D_80028564 = func_80069FF4(0xF0000011, 0x8000, 0x1000, (void *)func_80020A44);
    if (D_80028564 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 196);
        func_80069DF8(1);
    }
    D_80028568 = func_80069FF4(0xF0000011, 0x100, 0x1000, (void *)func_80020A58);
    if (D_80028568 == -1)
    {
        func_80069F18(D_80020000, D_80020028, 198);
        func_80069DF8(1);
    }
    D_8002856C = func_80069FF4(0xF0000011, 0x2000, 0x1000, (void *)func_80020A6C);
    if (D_8002856C == -1)
    {
        func_80069F18(D_80020000, D_80020028, 200);
        func_80069DF8(1);
    }
    func_8006A094();
    if (func_8006A034(D_80028550) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 212);
        func_80069DF8(1);
    }
    if (func_8006A034(D_80028554) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 214);
        func_80069DF8(1);
    }
    if (func_8006A034(D_80028558) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 216);
        func_80069DF8(1);
    }
    if (func_8006A034(D_8002855C) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 218);
        func_80069DF8(1);
    }
    if (func_8006A034(D_80028560) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 220);
        func_80069DF8(1);
    }
    if (func_8006A034(D_80028564) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 222);
        func_80069DF8(1);
    }
    if (func_8006A034(D_80028568) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 224);
        func_80069DF8(1);
    }
    if (func_8006A034(D_8002856C) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 226);
        func_80069DF8(1);
    }
}

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

/* c_server.c's teardown: eight asserts bracketed by func_8006A084 /
   func_8006A094 (EnterCriticalSection / ExitCriticalSection), one per event
   handle, at source lines 263..277 in steps of two.

   func_80069DF8 has to be declared NORETURN. Without it gcc assumes each
   assert body falls through into the next test, so after the two calls it
   re-materialises the next handle's %hi -- one extra word per assert, 121
   against 114. With it the `lui` that reorg puts in each `bnez`'s delay slot
   is the one the branch target's `lw` uses. This is the lever that makes the
   whole assert family reachable; see func_80020A80, which has 24 of them. */
void func_80020F18(void)
{
    func_8006A084();
    if (func_8006A004(D_80028550) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 263);
        func_80069DF8(1);
    }
    if (func_8006A004(D_80028554) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 265);
        func_80069DF8(1);
    }
    if (func_8006A004(D_80028558) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 267);
        func_80069DF8(1);
    }
    if (func_8006A004(D_8002855C) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 269);
        func_80069DF8(1);
    }
    if (func_8006A004(D_80028560) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 271);
        func_80069DF8(1);
    }
    if (func_8006A004(D_80028564) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 273);
        func_80069DF8(1);
    }
    if (func_8006A004(D_80028568) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 275);
        func_80069DF8(1);
    }
    if (func_8006A004(D_8002856C) == 0)
    {
        func_80069F18(D_80020000, D_80020028, 277);
        func_80069DF8(1);
    }
    func_8006A094();
}

s32 func_800210E0(void)
{
    return D_800287C8;
}

/* NEAR MISS: 75 words, 2 DIFF -- everything matches but the ORDER of the two stores that end the st==3 block. The target has 'u[0] = 1' then 'D_800287C8 = 0'; in that order gcc cross-jumps the trailing 'D_800287C8 = 0' with the identical store that ends the st==1 block, which drops the function to 74 words, and the only orders that keep 75 put one of the two pairs the wrong way round. Four things ARE load-bearing and already right: the dispatch has to be a GOTO CHAIN with the bodies placed after it (a switch on any case grouping roots the tree at 1 instead of testing 0 and 2 by equality first, and the inner split must be spelled '(u32)st >= 3' to get the sltiu); the D_80084118 selection is a pointer advanced by the condition, not '(&D_80084118)[cond]'; the two blocks need SEPARATE pointer locals, because with one shared local the st==1 block's lo_sum lands in a different register from its 'lui' (3 DIFF) where the target ties them; and 'st' must be read into a local before 'r' so the %hi(D_800287C8) stays live in $a1 for both stores. Measured: switch with five case groupings (75w/63 each), two-stage early-goto plus switch (70w/66), explicit unsigned split with inline bodies (73w/62), goto chain with shared local (74w/50), the three reorderings of each block's tail, and 'r = 2' moved to the end of the st==3 block (74w/47, gcc hoists it back above the stores).
   Source in docs/near-misses/func_800210F0.c. */
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

s32 func_80021594(void)
{
    UnkFnTab5 a = D_80020034;
    UnkFnTab5 b = D_80020048;
    s32 r = 0;
    s32 st = D_800287E0;

    if (st == 0)
    {
        r = func_80021410();
        if (r != 0)
        {
            r = a.f[r]();
        }
    }
    else if (st == 1)
    {
        r = func_800214FC();
        if (r != 0)
        {
            r = b.f[r]();
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020000);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020028);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020034);

INCLUDE_RODATA("asm/nonmatchings/ovl_main_80020000", D_80020048);

/* NEAR MISS: 82 words, 11 DIFF, and all eleven sit in one region -- the two switch arms that zero a word and the shared store they cross-jump into. It is the overlay's only function with a real JUMP TABLE: jtbl_80020060 has five entries for st 1..5, grouped 1+2, 3, 4+5, which a plain switch reproduces exactly. The target gives each arm its own 'sll $v1, $a0, 2' and cross-jumps only the two-word 'addu'/'sw' tail, so the shift stays live into the shared 'D_80028530[i] >= 0xB' check; every spelling here cross-jumps the shift as well, leaving the arms to pass only the symbol base and swapping $v0 and $v1 through the whole region. Four arm spellings measured (two direct stores, a pointer local in either arm, separate pointer locals in both) all give 82w/11, and so does declaring D_80028530/D_80028538 as arrays rather than scalars. Two things ARE load-bearing and right: the st==0 exit must be an enclosing 'if (st != 0)' with a single 'return st' at the end, because the target's early branch jumps to the shared 'addu $v0, $s0, $zero' instead of materialising a zero (as 'return st' early it is 82 words but 46 DIFF), and the two zeroing arms must reach the check through a 'goto', not each carry their own copy of it -- duplicated, cse forwards the stored 0 into arm 3's comparison and folds it away (83w/36).
   Source in docs/near-misses/func_8002168C.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_8002168C);

/* A button-repeat counter over the two-entry D_80028540/D_80028548 pair. It
   RETURNS &D_800287D8, which is why the target computes that address on both
   exits and then never reads it -- as a void function those two words
   disappear and nothing else changes. Both arms repeat the pair of clearing
   stores so jump.c cross-jumps them into the shared tail at $a0/$a1. */
s32 *func_800217D4(void)
{
    s32 v = func_80021410();
    s32 i;

    if (v != 0)
    {
        i = func_8002190C();
        if ((&D_80028548)[i] == v)
        {
            (&D_80028540)[i]++;
        }
        else
        {
            (&D_80028540)[i] = 0;
        }
        (&D_80028548)[i] = v;
        if (v == 1 || v == 4)
        {
            if ((&D_80028540)[i] >= 4)
            {
                (&D_800287D8)[i] = 1;
                (&D_80028540)[i] = 0;
                (&D_80028548)[i] = 0;
            }
        }
        else
        {
            (&D_80084118)[i] = 0;
            if ((&D_80028540)[i] >= 4)
            {
                (&D_800287D8)[i] = 0;
                (&D_80028540)[i] = 0;
                (&D_80028548)[i] = 0;
            }
        }
        func_800209C4();
    }
    return &D_800287D8;
}

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

s32 func_8006A124(u8 *a, u8 *b);
void func_80021DF4(s32 idx, u8 *src);

/* func_8006A124 is almost certainly libapi `rename`: it takes two of the
   0x20-byte name buffers func_80021B18 builds, and this function uses it to
   move the real save to the scratch name (slot 5), read it back, and move it
   home again. The two buffers must be declared nameA-then-nameB to land at
   sp+0x18 and sp+0x38 in that order; reversed gives 19 DIFF. */
s32 func_80021F18(s32 idx, u8 *buf)
{
    u8 nameA[0x20];
    u8 nameB[0x20];
    s32(*tbl)[32];
    s32 *row;
    s32 r;

    func_80021B18(nameA, idx);
    func_80021B18(nameB, 5);
    tbl = D_80083E98;
    row = tbl[idx];
    if (row[0] != 0)
    {
        func_8006A124(nameA, nameB);
    }
    *(s32 *)(buf + 0x204) = 0;
    func_80021EB8((s32 *)buf);
    r = func_80021C4C(nameB, buf, 0xC0, 0, row[0]);
    if (r != 0)
    {
        func_80021DF4(idx, buf);
        func_800220DC();
    }
    func_8006A124(nameB, nameA);
    return r;
}

s32 func_80021FF0(s32 idx, u8 *buf, s32 flag)
{
    u8 name[0x20];
    s32 saved;
    s32 *row;
    s32(*tbl)[32];

    func_80021B18(name, idx);
    if (func_80021B98(name, buf, 0xC0, 0) != 0 && func_80021EE0((s32 *)buf) != 0)
    {
        if (flag != 0)
        {
            saved = *(s16 *)(buf + 0x208);
            *(s16 *)(buf + 0x208) = 0;
            *(s32 *)(buf + 0x204) = 0;
            func_80021EB8((s32 *)buf);
            tbl = D_80083E98;
            row = tbl[idx];
            *(s16 *)((u8 *)row + 8) = 0;
            row[1] = *(s32 *)(buf + 0x204);
            func_80021C4C(name, (u8 *)row, 1, 4, row[0]);
            func_80021C4C(name, buf, 1, 0, row[0]);
            *(s16 *)(buf + 0x208) = saved;
        }
        else
        {
            *(s16 *)(buf + 0x208) = 0;
        }
    }
}

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

/* The space-skipping width measurer: for each of the a+1 segments it skips a
   run of spaces (one unit each), then walks the glyphs two bytes at a time
   (two units each), reporting the run start in *x, the glyph count in *y and
   the accumulated glyph total in *z.

   The space skip has to be a plain `while`, not a guarded `do`/`while`. Both
   compile to the same guard-plus-rotated-test shape, but written as
   `if (...) do ... while (...)` gcc shares ONE register for the 0x20 it
   compares against, where the target has three: one hoisted out of the outer
   loop, one hoisted into the skip loop by loop rotation, and one for the
   glyph walk. Writing the skip as a loop in its own right is what makes
   loop.c hoist that second copy. */
void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z)
{
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
        *z += n;
        while (*s == ' ')
        {
            s++;
            total++;
        }
        *x = total;
        n = 0;
        while (*s != ' ' && *s != 0)
        {
            s += 2;
            n++;
        }
        total += n * 2;
        *y = n;
    } while (++i <= a);
}

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

u8 *func_80022300(s32 *a, u8 *p, s32 n);

/* p[569] is ptrs[6] of the 0x8B0 group (ptrs starts at 0x8CC) and p[563 + n]
   is ptrs[n]; the two s16 stores are the sprite's x and y, each a position
   minus half the corresponding extent. `x * 4` has to be a local declared
   BEFORE the other two: the target computes it at word 3, ahead of the $s4
   save and the `p + 0xE4` address, and inline it is computed after both. */
void func_800223B4(s32 *p, s32 x, s32 y, s32 n, s32 z)
{
    s32 sx = x * 4;
    UnkCardPair *e = (UnkCardPair *)p[569];
    u8 *q = (u8 *)&p[57];

    e->unk_04->unk_08 = sx - (p[6] / 2) + 0xA2;
    e->unk_04->unk_0A = (n * 17) - (p[7] / 2) + 0x7A;
    func_80022300(p, q, y);
    e->unk_00 = (s32)q;
    func_8002233C((u8 *)((UnkCardPair *)p[563 + n])->unk_00, z, y);
}

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

/* The same input-handler shape as func_800265B8, and the same two levers:
   the `p[4] >= 3` arm comes first with the increment as the `else`, and the
   0x8000/0x2000 pair is written out in BOTH arms so jump.c cross-jumps the
   two `dir` assignments into one tail. func_800231E4(1) is likewise written
   in both arms of the p[8] test -- the target calls it either way but only
   stores the result when p[8] is non-zero, and the store needs p[8] reloaded
   after the call, so the test has to precede it. */
void func_80022524(s32 *p)
{
    s32 dir = 0;
    s32 n;

    if (D_80083160.unk_08 == 0)
    {
        return;
    }
    if (D_80083160.unk_10 & 0x20)
    {
        func_80053DA8(0x515);
        func_800231E4(1);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = 1;
        }
    }
    else if (D_80083160.unk_10 & 0x40)
    {
        func_80053DA8(0x514);
        if (p[8] != 0)
        {
            *(s32 *)p[8] = func_800231E4(1);
        }
        else
        {
            func_800231E4(1);
        }
    }
    else if (D_80083160.unk_08 & 0xA000)
    {
        if (D_80083160.unk_10 & 0xA000)
        {
            p[4] = 0;
            if (D_80083160.unk_10 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_10 & 0x2000)
            {
                dir = 1;
            }
        }
        else if (p[4] >= 3)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                dir = -1;
            }
            else if (D_80083160.unk_08 & 0x2000)
            {
                dir = 1;
            }
        }
        else
        {
            p[4]++;
        }
    }
    if (dir != 0)
    {
        n = func_80049E1C(p[0], dir, D_800280B4[p[2]].unk_14);
        if (n != p[0])
        {
            p[0] = n;
            func_80053DA8(0x502);
        }
        func_80022488(p);
    }
}

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

/* NEAR MISS: 102 words -- the right count -- but 84 differing, nearly all of it register names cascading from one hoisting decision. This is func_80022A30's bigger sibling (same unaligned D_800200A0 copy, same four iterations, same 'p[-0x18] |= 0x80' and return) with THREE primitives per iteration instead of two: 0xC, 0xC and then 0x18 bytes, the last one a D_80027F48 template copied through a 24-byte alignment-1 struct and then patched with four 0x90D0E0 words. The width and height come from D_800280B4[q[2]].unk_16 * 4 and .unk_17 * 17, stored to q[6]/q[7] with +8 and halved for the sprite bytes. Declaring w and h as u32 is right -- it turns three of the four halvings from 'sra' into the 'srl' the target has, and the fourth, (w + 0xA) >> 1, really is 'sra' there. What is left: loop.c hoists all FOUR halvings out of the loop here, where the target hoists only three and leaves 'h + 0xA' unshifted in $t4, doing the 'srl' inside the loop each iteration -- a move_movables threshold decision, so the register pressure differs by one quantity. Seven spellings measured: >>1 and /2 on u32 and on s32 with casts (all 102w/84), and three shapes that hoist a, b and c into explicit locals and shift d inline (102w/88, and 111w/102 when the UnkStrSet row is re-indexed per value instead of cached in w/h).
   Source in docs/near-misses/func_80022774.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80022774);

void func_8002290C(s32 *p)
{
    s32 **q = (s32 **)p[0x238];

    ((u16 *)q[1])[4] = 0xA0;
    ((u16 *)q[1])[5] = 0x78;
    q[0] = p + 9;
}

/* Five sprites in a row, 0x180 bytes of buffer apart, each label rendered by
   func_8004DA74 into `(u8 *)p + 0x114 + i * 0x180`. D_800280B4 must be
   indexed directly: hoisting it into a `UnkStrSet *t` local costs 5 DIFF
   even though gcc hoists the base address either way, and giving the row its
   own local inside the loop costs 21. */
void func_80022934(s32 *p)
{
    s32 i;

    for (i = 0; i < 5; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[563 + i];

        e->unk_04->unk_08 = 0x124 - (p[6] / 2);
        e->unk_04->unk_0A = (i * 17) - (p[7] / 2) + 0xFC;
        e->unk_00 = (s32)func_8004DA74((u8 *)p + 0x114 + i * 0x180,
                                       D_800280B4[p[2]].strs[i], 1);
    }
}

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

s32 func_80022B20(s32 a)
{
    return func_800491F4(a, D_80027F60, 7);
}

/* NEAR MISS, parked at 40 words / 9 DIFF; source and the full measurement
   table are in docs/near-misses/func_80022B48.c, which is also the permuter's
   base (`tools/decomp/permimport.py func_80022B48`). Two permuter runs and a
   by-hand sweep took it from 13 to 9: the lever is a SECOND base pointer,
   `UnkCardGrp8B0 *b = c;`, used only for the three trailing `shared` stores.
   That is what forces the `addu $t3, $a0, $zero` copy at word 0 -- without it
   gcc keeps the base in $a0 and the function is one word short -- and once
   the `e` local is dropped and both UnkCardPair fields go through
   `c->ents[i]`, the loop body is the target instruction for instruction.
   What is left is that the loop still addresses off $a0 where the target uses
   $t3, so the prims address takes $v1 instead of $a0. The five siblings are
   the same shape at the other five base offsets. */
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
    func_80022774((u8 *)(p + 9), p);
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

void func_80022B48(s32 a, s32 n);

/* The same constructor idiom as func_80023FF0 and func_800238D4, at the
   singleton D_800287E8. It RETURNS q even though every caller ignores the
   value: the target copies $s0 into $v0 and stores the func_80022CD8 pointer
   through $v0, which as a void function is three words instead of four.
   func_80022B20 likewise had to become `s32` with a `return`, since its
   value lands in r[0x232]; its own ten words are unchanged. */
s32 func_80022D60(s32 a, s32 id, s32 kind, s32 p)
{
    s32 *q = (s32 *)D_800287E8;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_800287E8, 0x27A);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_800231E4(r[3] != 2);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0x9C8);
    }
    func_80022B48((s32)r, 7);
    q[3] = (s32)(r + 0x22F);
    r[0x232] = func_80022B20((s32)(r + 0x233));
    func_80022C90(r, a, id, kind, p);
    q[4] = (s32)func_80022CD8;
    return (s32)q;
}

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

/* NEAR MISS: 94 words, 70 DIFF, and nearly all of it is one register swap -- the target holds p in $t1 and the loop counter in $t2, every spelling here has them the other way round, and that shifts the register names through the whole body. Words 1-11 and 15-20 are otherwise instruction-for-instruction right, which took three levers: p[33] * 16 must be spelled '<< 4' (as '* 16' gcc reassociates the constant onto the (p[34] - p[35]) difference instead, 93w/85); p[35] needs a LOCAL, or gcc reloads it narrowed to 'lhu' for the 16-bit store instead of reusing the word it already loaded; and the division plus 0xA needs its own local, or fold moves the 0xA onto the (p[35] << 4) term. The remaining word is the store address: the target computes 'addiu $t0, $a3, 0x7C' off the stride-1 induction variable and stores through it, where the plain '((u8 *)p)[0x7C + i] = v' gives a one-word 'sb $v0, 124($a2)' (93w/49, the best DIFF measured) and '&((u8 *)p)[0x7C + i]' gives two words but computes 'i + 124' before adding p. Also measured: four groupings of the head expression, three store-pointer spellings, three declaration orders, and a copy of the parameter into a local (68 DIFF, the only thing that moved the swap at all).
   Source in docs/near-misses/func_80023258.c. */
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

/* Four sprite slots per row, three rows, each x and y written through a
   freshly reloaded `p[k + i]` -- gcc cannot CSE the second store's base
   because the first store may alias it, so spelling each store as its own
   full expression is what produces the doubled `lw` pairs. A `do`/`while`
   here is 78 words against 76. */
void func_80023600(s32 *p)
{
    s32 i;

    p[32] = 1;
    p[33] = 1;
    func_8002352C((u8 *)p);
    func_800235A0((s32)(p + 1), 0x36, 0x10);
    func_800233D0(p);
    ((UnkCardPair *)p[104])->unk_04->unk_0A = 0x1A;
    for (i = 0; i < 3; i++)
    {
        ((UnkCardPair *)p[118 + i])->unk_04->unk_08 = 0x8E;
        ((UnkCardPair *)p[118 + i])->unk_04->unk_0A = (i * 16) + 0x82;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_08 = 9;
        ((UnkCardPair *)p[106 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_08 = 0x42;
        ((UnkCardPair *)p[110 + i])->unk_04->unk_0A = (i * 16) + 0xA;
        ((UnkCardPair *)p[114 + i])->unk_04->unk_08 = 0x24;
        ((UnkCardPair *)p[114 + i])->unk_04->unk_0A = (i * 16) + 0xA;
    }
    func_80023470(p);
}

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

/* Four UnkCardPair slots set up in a row, each `e->unk_00` plus the x and y
   in its UnkCardPrim. Two things are load-bearing:

   `e = (UnkCardPair *)p[155]` must be assigned BEFORE the func_8004E5A0 /
   func_8004E69C pair, not after: the target loads $s3 at word 39, ahead of
   both calls, and with the load after them the function is 71 words instead
   of 72 (gcc fills the p[0] load-delay slot with the `li $a1, 4` that the
   target has already issued back at word 27).

   The second table access has to be spelled as a BYTE offset. The target
   advances the D_80083EA4 base register in place at word 37
   (`addu $s0, $s0, $v0`), which gives that pseudo a fourth reference and so
   wins it $s0 over `p`; `D_80083EA4[p[0]][8]` computes the sum into a
   scratch instead and leaves $s0/$s1 swapped through the whole function (18
   DIFF). A named `s32 (*tbl)[32]` local with `tbl += p[0]` gets the in-place
   update but hoists the `lui`/`addiu` into the prologue, which is worse
   (23). */
void func_80023C44(s32 *p)
{
    u8 buf[8];
    UnkCardPair *e;

    e = (UnkCardPair *)p[156];
    e->unk_04->unk_08 = 0x70;
    e->unk_04->unk_0A = 0x10;
    e->unk_00 = (s32)D_80078FD0;

    e = (UnkCardPair *)p[152];
    e->unk_00 = (s32)func_8004DA74((u8 *)(p + 2), (u8 *)D_80083EA4[p[0]], 1);
    e->unk_04->unk_08 = 0xF0;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[155];
    func_8004E5A0(*(s32 *)((u8 *)D_80083EA4[p[0]] + 0x20), 4, buf);
    func_8004E69C(buf);
    e->unk_00 = (s32)func_8004E298((u8 *)(p + 0x41), buf, 1);
    e->unk_04->unk_08 = 0x147;
    e->unk_04->unk_0A = 0x91;

    e = (UnkCardPair *)p[158];
    e->unk_00 = (s32)(p + 77);
    e->unk_04->unk_08 = 0xAE;
    e->unk_04->unk_0A = 0x18;
}

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

s32 func_80023ECC(s32 a)
{
    return func_800491F4(a, D_80027FC4, 0xA);
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

void func_80023EF4(s32 *p, s32 n);

/* The func_800238D4 constructor idiom at an INDEXED row: D_80029618 is an
   array of 0x3E4-byte rows, and gcc synthesises the 996 multiply as
   `(((b << 5) - b) << 3) + b) << 2`. func_80023ECC had to become `s32` with
   a `return` -- its value lands in r[150] here -- which leaves its own ten
   words unchanged. `q[3] = ...` must come BEFORE the func_80023ECC call:
   reversed, the function grows to 64 words. */
s32 func_80023FF0(s32 a, s32 b)
{
    s32 *q = (s32 *)D_80029618[b];
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, (u8 *)q, 0xF9);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024184(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0x3C4);
    }
    func_80023EF4(r, 0xA);
    q[3] = (s32)(r + 147);
    r[150] = func_80023ECC((s32)(r + 151));
    func_80023FA0(r, a, b);
    q[4] = (s32)func_80023EC4;
    return (s32)q;
}

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

/* Three label layouts for the save-slot list, then a loop writing the text
   sprites. `k` is `p[32] == 1` -- gcc's `xori` plus `sltu 1` -- and it is used
   BOTH as the first condition and as an index into D_800283B8 and
   D_800283F4, even though it is provably 0 on the two arms that index with
   it. n/3 is the signed magic multiply by 0x55555556. */
void func_80024818(s32 *p)
{
    u8 buf[0x40];
    u8 num[0x10];
    s32 k = p[32] == 1;
    s32 n;
    s32 cnt;
    s32 i;

    n = func_80022138();
    if (k != 0 && func_80022160() == 0)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283EC[0], 1);
        func_8004DA74((u8 *)p + 0x204, D_800283EC[1], 1);
        cnt = 2;
    }
    else if (n < 3)
    {
        func_8004DA74((u8 *)p + 0x84, D_800283C4, 1);
        func_8004DA74((u8 *)p + 0x204, D_800283B8[k], 1);
        func_8004DA74((u8 *)p + 0x384, D_800283B8[2], 1);
        cnt = 3;
    }
    else
    {
        func_8004DA74((u8 *)p + 0x84, D_800283F4[k], 1);
        func_8003AD08(n / 3, num);
        func_80069E78(buf, num);
        func_80069E38(buf, D_800283FC);
        func_8004DA74((u8 *)p + 0x204, buf, 1);
        cnt = 2;
    }
    for (i = 0; i < cnt; i++)
    {
        UnkCardPair *e = (UnkCardPair *)p[723 + i];

        e->unk_00 = (s32)((u8 *)p + 0x84 + 0x180 * i);
        e->unk_04->unk_08 = 0xA9;
        e->unk_04->unk_0A = 0x110 + 0x10 * i;
    }
}

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

s32 *func_80024D58(s32 *p, s32 a, s32 b)
{
    s32 *q = (s32 *)D_8002A990;
    s32 *r = q + 8;

    if (func_8004B4A8(q) == 0)
    {
        q = func_8003FE78(0, D_8002A990, 0x394);
        func_8004491C((UnkNode *)q, (u32)func_80027DD0);
    }
    else
    {
        func_80024EEC(q);
        ((u16 *)q)[15] &= 0x7FFF;
        func_80069EC8(r, 0xE30);
    }
    r[0x1F] = a;
    r[0x20] = b;
    func_80024C70((s32)r, 0x15);
    q[3] = (s32)(r + 0x2CB);
    r[0x2CE] = (s32)func_80024C48w(r + 0x2CF);
    func_80024D18(r, (s32)p);
    q[4] = 0;
    return q;
}

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
    func_80025F0C((u8 *)p, v);
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

/* The third of the input handlers, and the closest to func_800265B8 -- same
   0x40 / 0x5000 chain, same `p[12] >= 0xD` arm first, same pair of separate
   exit tests -- with three differences: the 0x20 case FALLS THROUGH into the
   0x40 test instead of leaving through a goto, the held-key step is -2 rather
   than -4, and func_80027AFC takes a literal 0 as its second argument. The
   unk_08 test has to be an enclosing `if`, not an early return: on the
   zero path the target still runs the func_8002168C/func_80021904 tail. */
void func_80025584(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            p[-4] = (s32)func_80024FFC;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_800254E4(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 2;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
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
            func_80053DA8(0x502);
            p[10] = func_80049DE8(p[10], dir, 5);
            func_800250E8(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
    func_80025D34(p - 8);
    func_80027AFC(p[8], 0);
}

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

/* The fifth input handler, and the widest: two flags rather than one. `dir`
   drives the func_80049DE8 step (through p[10] with a limit of 5 or p[17]
   with 3, depending on p[18]) and `upd` records that the layout changed, so
   func_80025030 can run once at the very end. Its 0x20 case RETURNS, which
   is why it skips that final call. Same two levers as its siblings -- the
   `p[12] >= 0xD` arm first, and the two exit tests nested rather than
   `r == 0 || r == 1`. */
void func_8002593C(s32 *p)
{
    s32 dir = 0;
    s32 upd = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
            return;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            if (p[18] == 0)
            {
                func_8002592C(p);
                upd = 1;
            }
            else
            {
                func_800258C8(p);
            }
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
                {
                    dir = 1;
                }
            }
            else
            {
                p[12]++;
            }
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x2000)
            {
                p[18] = 1;
            }
            else
            {
                p[18] = 0;
            }
            upd = 1;
        }
        if (dir != 0)
        {
            func_80053DA8(0x502);
            if (p[18] == 1)
            {
                p[10] = func_80049DE8(p[10], dir, 5);
            }
            else
            {
                p[17] = func_80049DE8(p[17], dir, 3);
            }
            func_80025030(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r != 0)
    {
        if (r != 1)
        {
            func_80025D34(p - 8);
            func_80027AFC(p[8], 0);
        }
    }
    if (upd != 0)
    {
        func_80025030(p);
    }
}

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
    r[7] = (s32)func_80024D58(q, r[10], 0);
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

/* NEAR MISS: 57 words against 60: the target keeps the parameter p ITSELF in a fourth callee-saved register ($s3) as well as p+4 in $s0, and RECOMPUTES p+4 once after the if/else join -- which is where the three words go. Five spellings measured: p+4 in one local (57w/58), p+4 written inline at all eleven call sites (61w/31), the local reassigned after the join (61w/32), and the D_80028294 base hoisted into a local for indices 1..3 or for all four (both 57w/58). The body is right: strcpy from D_80028294[0], then func_8003AD08(n+1) into a stack buffer and a chain of strcats with D_80028294[3] twice, the 0x8001020C literal, D_80028294[1] or [2] on the 0x80010208 halfword, D_800282A0, and finally func_8003AD08 of the word at 0x8001022C.
   Source in docs/near-misses/func_80025F0C.c. */
INCLUDE_ASM("asm/nonmatchings/ovl_main_80020000", func_80025F0C);

/* A three-pass VRAM upload. 0x80010080 is a constant address, not a symbol --
   the `lui` + `ori` pair is the tell (see docs/addressing-and-translation-units.md).
   Both block copies have to go through the alignment-1 structs: the loads are
   lwl/lwr, so gcc is not allowed to assume either side is word-aligned.
   The `r.y` update must come AFTER the row copy (43 DIFF before it), and the
   source pointer must be `(u8 *)0x80010080 + i * 0x80` rather than a walking
   local advanced at the bottom of the loop (56 DIFF). */
void func_80025FFC(s32 *p, s32 n)
{
    UnkURect r;
    s32 i;

    *(UnkEight *)&r = *(UnkEight *)D_800200B8;
    *(UnkBlock20 *)((u8 *)p + 0x60) = *(UnkBlock20 *)D_80027E78[n];
    r.y += n * 16;
    for (i = 0; i < 3; i++)
    {
        func_8006733C(&r, (u8 *)0x80010080 + i * 0x80);
        r.x += 4;
    }
    func_80067014(0);
}

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
    func_80025F0C((u8 *)p, a);
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

/* The card-list input handler. Three things are load-bearing:

   The `p[12] >= 0xD` arm has to come FIRST, with the plain increment as the
   `else`: the target's `slti`/`bnez` jumps forward to the increment, so the
   increment is the block placed after the chain, and writing it the natural
   way round costs a `j` over it.

   The two exit tests must be two separate `if`s. As `r == 0 || r == 1` gcc
   folds them into `sltiu $s0, $s0, 2`, where the target keeps `beqz` plus a
   `beq` against a materialised 1.

   The 0x20 case leaves through a `goto` into the tail -- it does NOT fall
   into the func_8002168C/func_80021904 pair the way every other path does.

   D_80083160 is the Unk80083160 struct from include/structs.h, read as
   .unk_08 and .unk_10; the same pair appears in func_800277BC. */
void func_800265B8(s32 *p)
{
    s32 dir = 0;
    s32 r;

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            goto close;
        }
        if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_8002651C(p);
        }
        else if (D_80083160.unk_08 & 0x5000)
        {
            if (D_80083160.unk_10 & 0x5000)
            {
                p[12] = 0;
                if (D_80083160.unk_10 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_10 & 0x4000)
                {
                    dir = 1;
                }
            }
            else if (p[12] >= 0xD)
            {
                p[12] -= 4;
                if (D_80083160.unk_08 & 0x1000)
                {
                    dir = -1;
                }
                else if (D_80083160.unk_08 & 0x4000)
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
            func_80053DA8(0x502);
            p[11] = func_80049DE8(p[11], dir, 5);
            func_80025DC8(p);
        }
    }
    r = func_8002168C();
    func_80021904();
    if (r == 0)
    {
        return;
    }
    if (r == 1)
    {
        return;
    }
close:
    func_80026920(p - 8);
    func_80027AFC(p[8], p[9]);
}

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
        r[1] = (s32)func_80024D58(q, r[11], 3);
        r[2] = (s32)func_80027364(q);
        r[10] = 0;
        q[4] = (s32)func_800267C0;
    }
    return q;
}

/* Same destructor family as func_80025D34, which is still a near miss; this
   one lands because only the last of its four calls is inside the loop, so
   there is no delay-slot nop to account for. gcc folds the 0xC into the
   memory displacement and strength-reduces the index to the q cursor, so
   `q[3]` with `q++` and `q[3 + i]` in a `for` both give the same 37 words. */
void func_80026920(s32 *p)
{
    s32 *q = p + 8;
    s32 i;

    if (p != 0)
    {
        func_8004F52C(q[0]);
        func_80024EEC((s32 *)q[1]);
        func_80027458((s32 *)q[2]);
        i = 0;
        do
        {
            func_80024184((s32 *)q[3]);
            q++;
        } while (++i < 5);
        ((u16 *)p)[15] |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Two slots, each selected through the D_80028068 pick table. Four spellings
   were load-bearing here, and three of them are about the ORDER gcc emits a
   two-register `addu` in:

   - D_80028068 has to be declared `[][2]` and indexed `[p[9]][i]`. As a flat
     `u8 []` with `[p[9] * 2 + i]` gcc adds `i` before the array base, which
     is six words wrong across the three evaluations.
   - The func_8004CBFC pointer needs its offset parenthesised,
     `(u8 *)p + (k * 12 + 4)`, so the `+ 4` lands in the scaled value rather
     than after the base is added.
   - The tested pointer has to come off a BYTE cursor whose scale is already
     applied (`(u8 *)p + k * 4`, then `0x68` as the displacement). Written
     `p[26 + k]` the offset is still a shift when the address is formed, and
     gcc canonicalises `(plus (mult ...) (reg))` with the shift first, giving
     `addu $v0, $v0, $s2` where the target has `addu $v0, $s2, $v0`. Four
     base-first spellings of the same index all leave that one word wrong.

   func_8004CBFC takes THREE arguments (see csrc/unk_1C540.c:2791); the
   declaration here had two, and the third is this same pointer, which is why
   the target loads it into $a2 before the test. */
void func_800269B4(s32 *p)
{
    s32 i;

    for (i = 0; i < 2; i++)
    {
        s32 k = D_80028068[p[9]][i];
        u8 *q = (u8 *)p + k * 4;
        u32 *e = *(u32 **)(q + 0x68);

        if (e[0] != 0)
        {
            func_8004CBFC((u32 *)((u8 *)p + (k * 12 + 4)), D_80028074[i], e);
        }
        ((UnkCardPair *)p[24 + i])->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
        ((UnkCardPair *)p[26 + i])->unk_04->unk_00 = D_8002806C[D_80028068[p[9]][i]];
    }
}

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

/* The fourth input handler. Its tail is the interesting part: it calls
   func_800217D4 for the &D_800287D8 pair that function returns, records
   whether each word changed, then copies the pair into p[6]/p[7]. That copy
   has to be an 8-byte STRUCT assignment -- gcc's aligned movstrsi, lw/lw
   then sw/sw -- because as two field assignments it comes out interleaved
   and one word longer. The 0x40 case returns outright rather than falling
   into the shared tail. */
void func_8002789C(s32 *p)
{
    s32 flag = 0;
    s32 *t;
    s32 diff[2];

    if (D_80083160.unk_08 != 0)
    {
        if (D_80083160.unk_10 & 0x20)
        {
            func_80053DA8(0x515);
            func_80027C90(p - 8);
            func_80027BF4();
            if (D_80082E60.unk_0B == 3)
            {
                func_80040AA0(3);
            }
            else
            {
                func_80044144(0, 0, 0, 0);
                ((u16 *)p[5])[15] |= 0x2000;
                if (p[0] == 2)
                {
                    func_800A68F4();
                }
            }
        }
        else if (D_80083160.unk_10 & 0x40)
        {
            func_80053DA8(0x503);
            func_80020924(p[3]);
            p[13] = 0;
            p[-4] = (s32)func_800277BC;
            return;
        }
        else if (D_80083160.unk_08 & 0xA000)
        {
            if (D_80083160.unk_08 & 0x8000)
            {
                if (p[3] != 0)
                {
                    flag = 1;
                }
                p[3] = 0;
            }
            else
            {
                if (p[3] == 0)
                {
                    flag = 1;
                }
                p[3] = 1;
            }
        }
        if (flag != 0)
        {
            func_80053DA8(0x502);
            func_80026FD4((s32 *)p[9], p[3]);
        }
    }
    t = func_800217D4();
    diff[0] = t[0] != p[6];
    diff[1] = t[1] != p[7];
    *(UnkTwoWords *)&p[6] = *(UnkTwoWords *)t;
    func_80026FF8((s32 *)p[9], t, diff);
    func_80021904();
}

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
