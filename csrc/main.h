#ifndef OVERLAY_MAIN_H_
#define OVERLAY_MAIN_H_

s32 func_800213C4(s32 arg0);
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

#define gCardChannel D_800287CC
extern s32 gCardChannel;

typedef enum CARD_STATE
{
    CARD_STATE_OK = 0,
    CARD_STATE_EOF = 1,
    CARD_STATE_ERROR = 2,
    CARD_STATE_TIMEOUT = 3,
    CARD_STATE_NEW = 4,
} CARD_STATE;

#define gCardState D_800287E4
extern CARD_STATE gCardState;

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

/* The 0x80-byte save-slot row, declared as a STRUCT alias of D_80083E98 (the
   unit already has it as `s32 [][32]`, which the other functions want).
   func_80024390 needs the struct spelling: its first row access is the name
   at +0xC, so gcc relocates against D_80083E98+0xC -- which splat names
   D_80083EA4 -- keeps that address in a callee-saved register, and derives
   the row base as `that - 0xC` in $fp for the other seven fields. With two
   separate symbols each access materialises its own %hi. */
typedef struct
{
    s32 unk_00;
    s32 unk_04;
    s16 unk_08;
    s16 unk_0A;
    u8 name[0x10];
    u32 unk_1C;
    s32 unk_20;
    s32 unk_24;
    s32 unk_28;
    u32 unk_2C;
    s32 unk_30;
    s32 unk_34;
    u8 pad_38[0x48];
} UnkSaveRow;

extern UnkSaveRow D_80083E98_s[] __asm__("D_80083E98");

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

UNK_CARD_GROUP(UnkCardGrp8B0, 0x8B0, 7);
UNK_CARD_GROUP(UnkCardGrp184, 0x184, 18);
UNK_CARD_GROUP(UnkCardGrp240, 0x240, 10);
UNK_CARD_GROUP(UnkCardGrpB20, 0xB20, 21);
UNK_CARD_GROUP(UnkCardGrp044, 0x44, 4);
UNK_CARD_GROUP(UnkCardGrp0D4, 0xD4, 4);

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
extern u8 D_80020000[];
extern u8 D_80020028[];
s32 *func_80027AFC(s32 a, s32 b);
u8 *func_80022A30(u8 *p, s32 *q);
void func_80021538(void);
void func_80025D34(s32 *p);
void func_80023E14(s32 *p);
void func_80025F0C(u8 *p, s32 n);
void func_80025FFC(s32 *p, s32 v);
void func_80026D78(s32 *p);
void func_80022BE8(UnkStrSet *p);
void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n);
void func_80026C94(s32 *p);
void func_80026CF0(s32 *p);
s32 func_8002191C(void);
void func_80027C90(s32 *p);
s32 *func_80025C7C(s32 *p, s32 a);
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
extern const u8 D_80020084[];

void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z);
s32 func_80049E6C(s32 a);
void func_80023C44(s32 *p);
s32 *func_8003FE78(s32 a, u8 *b, s32 c);
s32 *func_8003FC64(s32 a);
s32 func_8004B4A8(s32 *p);
s32 func_80021F18(s32 a, u8 *b);
void func_800237EC(UnkCardGrp184 *c, s32 n);
void func_80023258(s32 *p);
s32 func_8002168C(void);
void func_80026DF0(UnkCardGrp044 *c, s32 n);
void func_80024C70(UnkCardGrpB20 *c, s32 n);
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
extern s32 D_8002806C[];
extern u8 *D_80028074[];
extern u8 D_800287E8[];
extern u8 D_800291D0[];
extern u8 D_80029618[][0x3E4];
extern u8 *D_800282A4[];
extern u8 D_800791A4[];
void func_8002727C(UnkCardGrp0D4 *c, s32 n);
s16 func_80027DD0(void);
extern u8 D_8002BB08[];
extern u8 D_8002B968[];
s32 *func_80027364(s32 *p);
s32 *func_800238D4(s32 *p);
s32 *func_80024D58(s32 *p, s32 a, s32 b);
extern u8 D_80027FD0[];
extern u8 D_8002B7E0[];
extern u8 D_800200A0[];
extern u8 D_80027F48[];
extern u8 D_800282D8[];
extern u8 D_800282E8[];
extern u8 *D_800283A8[];
extern const u8 D_800200A4[];
extern u8 *D_80027FD4[];
u8 *func_8004E5E8(s32 v, u16 n, u8 *out);
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
extern u8 D_80027E68[];
void func_80025788(s32 *p);

/* DEFINED by this unit, in the .data block at the foot of
   csrc/ovl_main_80020000.c. {"bu00:", "bu10:"} -- index 0 is the SECOND of the
   two strings in the rodata listing. */
#define MEM_CARD_NAMES D_80027F18
extern char *MEM_CARD_NAMES[2];

/* "bu10:", immediately followed in the same rodata listing by "bu00:" at +8.
   Declared here so D_80027F18's initialiser can take its address. */
#define MEM_CARD1 D_80020074
extern char MEM_CARD1[8];
extern char MEM_CARD0[8];

/* DEFINED by this unit, in the .data block at the foot of
   csrc/ovl_main_80020000.c. The declaration stays here because the first use
   (func_80021F0C) is above the definition, which has to sit at its address
   position. */
extern char D_80027F20[16];

extern char *D_80027F30[];

void func_80024390(s32 *p);
void func_80024818(s32 *p);
void func_800249B4(s32 *p);
void func_800250E8(s32 *p);
void func_80025584(s32 *p);
extern s16 D_80028010[];
void func_80020A80(void);
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

#define gSwCardEOFEvent D_80028550
extern long gSwCardEOFEvent;

#define gSwCardErrorEvent D_80028554
extern long gSwCardErrorEvent;

#define gSwCardTimeoutEvent D_80028558
extern long gSwCardTimeoutEvent;

#define gSwCardNewEvent D_8002855C
extern long gSwCardNewEvent;

extern s32 D_80028560;
extern s32 D_80028564;
extern s32 D_80028568;
extern s32 D_8002856C;
extern s32 D_80084128[];
extern u8 D_80028020[];
extern UnkStrSet D_800280B4[];

extern struct DIRENTRY D_80028570[10];
// extern s32 D_80028570[][10];

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
void func_80026DF0(UnkCardGrp044 *c, s32 n);
void func_80024C70(UnkCardGrpB20 *c, s32 n);
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

void func_800220DC(void);
void func_80023A00(s32 *p);
void func_80024184(s32 *p);
void func_80024EEC(s32 *p);
void func_80026FB4(u16 *p);
void func_80027458(s32 *p);

#endif
