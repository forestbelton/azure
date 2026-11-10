#ifndef OVERLAY_MAIN_H_
#define OVERLAY_MAIN_H_

#define MEM_CARD_FILE_ID_TMP 5

/* `gsw->check_sum`, the word index of it in the `s32 *` the save-block
   checksum pair takes. The name is the 0x80400000 debug twin's: func_80401F98
   and func_80401FEC -- set_gsw_checksum and check_gsw_checksum here -- print
   it as `printf("%s = %x\n", "(int)gsw->check_sum", ...)` against the word at
   gsw+0x204, which is this index. The struct the twin's `->` implies is not
   written yet (and the `(int)` cast says the member is not plainly `int`), so
   until there is a type for it the member can only be named as its index into
   the pointer the live source already calls `gsw`. */
#define GSW_CHECK_SUM 0x81

/* Types */
typedef enum CARD_STATE
{
    CARD_STATE_OK = 0,
    CARD_STATE_EOF = 1,
    CARD_STATE_ERROR = 2,
    CARD_STATE_TIMEOUT = 3,
    CARD_STATE_NEW = 4,
} CARD_STATE;

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

/* Four bytes, alignment 1: init_save_file_header copies this out of
   SAVE_FILE_MAGIC with lwl/lwr + swl/swr, which is what gcc emits for a
   struct it cannot assume is word-aligned. */
typedef struct
{
    u8 b0;
    u8 b1;
    u8 b2;
    u8 b3;
} UnkFour;

typedef struct
{
    u8 b[0x20];
} UnkBlock20;

/* One step of an animation script, 8 bytes, witnessed by the two resident
   functions that walk one: func_8004CBFC seeds a primitive from the first
   step (`p[0] = *(u16 *)q / D_80080A84`, `r[0] = *(u32 *)(q + 4)`) and
   func_8004CB2C advances it, switching on the halfword at +2 -- 2 means
   `arg` is this step's cel record and the walk moves on 8 bytes, 0 and 1
   mean `arg` is the next script to run, which is how a script loops. */
typedef struct
{
    u16 delay;
    s16 op;
    void *arg;
} UnkAnimStep;

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

/* The u16 pair at the front of set_save_icon's stack copy: the loads are
   `lhu`, not `lh`, so the fields are unsigned. */
typedef struct
{
    u16 x;
    u16 y;
    u16 w;
    u16 h;
} UnkURect;

/* 0x80 bytes, alignment 1: store_save_slot_info copies a whole D_80083E98
   row with gcc's unknown-alignment block move -- the `(src | dst) & 3` test
   and both a lwl/lwr and a lw loop. Through the s32[][32] type gcc would
   know both sides are word-aligned and emit only the second. */
typedef struct
{
    u8 b[0x80];
} UnkRow;

/* Five function pointers, 20 bytes: poll_card_events block-copies both
   gSwCardEventHandlers and gHwCardEventHandlers onto its stack and dispatches
   through the copy. It has to be a struct COPY from the extern, not a local
   array INITIALISER: gcc emits a function's own constants into .rodata right
   after that function, and poll_card_events sits BEFORE this unit's first
   INCLUDE_RODATA line, so an initialiser lands at 0x80020000 and pushes every
   other rodata block down. (func_800258C8's initialiser works only because
   that function sits where its constant belongs.) */
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
/* The 0x2C bytes this used to spell as two pads are the WINDOW'S OWN root
   primitive, and the arithmetic says so in all six windows at once. Each
   setup function passes its group three addresses -- pad+0xC, pad-0x1C and
   pad-0xC -- to one of func_800226E0/func_80023730/func_80023A78/
   func_80024BB4/func_80026C00/func_800271C0, which copies the same two
   engine templates (D_8002E5D8, 4 words; D_8002E5E8, 3 words) that
   `prims`/`shared` get and then hangs them off that first address:

     func_80022C90  0x8B0 -> 0x8BC, 0x894, 0x8A4
     func_80023894  0x184 -> 0x190, 0x168, 0x178
     func_80023FA0  0x240 -> 0x24C, 0x224, 0x234
     func_80024D18  0xB20 -> 0xB2C, 0xB04, 0xB14
     func_80026E90  0x044 -> 0x050, 0x028, 0x038
     func_80027324  0x0D4 -> 0x0E0, 0x0B8, 0x0C8

   Six for six on pad+0xC, pad-0x1C and pad-0xC, so `pad_00C[0x10]` is an
   UnkCardPair (16 bytes, exactly), and the last 0x1C of `pad_000` is that
   pair's own 4-word primitive and 3-word UnkCardPrim12. Its `unk_0C` is
   written too: each ctor stores the func_800491F4 handle for `ptrs` into
   pad+0x18.

   Naming them changes no offset and no size -- verified member by member and
   for all six instantiations against the previous spelling. */
#define UNK_CARD_GROUP(name, pad, n) \
    typedef struct                   \
    {                                \
        u8 pad_000[(pad) - 0x1C];    \
        u32 root_prim[4];            \
        u32 root_shared[3];          \
        u32 shared[3];               \
        UnkCardPair root;            \
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

/* Functions */
/* State handlers whose ADDRESS is stored into a field at -0x10 by the
   countdown family (func_800252A4, func_8002546C, func_80026398,
   func_8002765C). Only the address is taken, so the signature here does not
   affect the generated code; it is written `(void)` rather than guessed. */
void recheck_card_slot(void);
void open_card_events(void);
void start_card_server(void);
void close_card_events(void);
s32 on_sw_card_eof(void);
s32 on_card_new(void);
s32 func_800213C4(s32 arg0);
void reset_sw_events(void);

/* No arguments: reset_card_server calls it with no $a0 set up at all, and
   its own listing never reads $a0 (it works off D_80028550 and up).
   set_card_slot's
   call looks like it passes arg0 only because $a0 still holds the incoming
   parameter there; both spellings compile identically in that caller, and
   this one is what reset_card_server requires. See the CALLEE ARITY caution in
   docs/matching-levers.md. */
void func_800214A4(void);

void reset_hw_events(void);
void func_80021538(void);
s32 func_8002168C(void);
s32 count_card_files(void);
s32 read_card_file(u8 *name, u8 *buf, s32 n, s32 off);
s32 load_save_slot_info(s32 n);
s32 write_save_file(s32 a, u8 *b);
s32 read_save_file(s32 a, u8 *b, s32 c);
void refresh_card_info(void);
s32 erase_save_file(s32 idx);
void func_8002225C(u8 *s, s32 a, s32 *x, s32 *y, s32 *z);
void func_800223B4(s32 *p, s32 x, s32 y, s32 n, s32 z);
void func_80022488(s32 *p);
void func_80022524(s32 *p);
u8 *func_80022774(u8 *p, s32 *q);
void func_80022934(s32 *p);
u8 *func_80022A30(u8 *p, s32 *q);
void func_80022BE8(UnkStrSet *p);

/* card_mes.c's message entry point. The 20 wrappers at msg_format_confirm..
   msg_delete_failed are one per message id 0..19, each passing a fixed id and
   kind and either a caller-supplied parameter or 0. Decoded mechanically
   from which argument register each wrapper sets; the register copies
   (`addu $a2, $a1, $zero` where the kind equals the id, `addu $a3, $a2,
   $zero` where both are 0) are gcc reusing an equal value, not source. */
s32 open_card_message(s32 a, s32 id, s32 kind, s32 p);

s32 func_800231E4(s32 flag);
void func_80023258(s32 *p);
void func_80023600(s32 *p);
void func_80023730(UnkCardPair *p, u32 *a, u32 *b);
void func_800237EC(UnkCardGrp184 *c, s32 n);
s32 *func_800238D4(s32 *p);
void func_800239B0(s32 **pp);
void func_80023A00(s32 *p);
void func_80023A78(UnkCardPair *p, u32 *a, u32 *b, s32 n);
void func_80023B18(UnkCardGrp240 *c);
void func_80023C44(UnkCardGrp240 *c);
void func_80023E14(UnkCardGrp240 *c);
s32 func_80023FF0(s32 a, s32 n);
void func_800240EC(UnkCardGrp240 *c);
void func_80024184(s32 *p);
u8 *func_800242D8(u8 *p, s32 w, s32 h);
void func_80024390(s32 *p);
void func_80024818(s32 *p);
void func_800249B4(s32 *p);
void func_80024A50(s32 *p);
void func_80024ACC(s32 *p);
void func_80024BB4(UnkCardPair *p, u32 *a, u32 *b);

/* func_80024D58 calls func_80024C48 with ONLY $a0 set, where the function
   itself takes five arguments (func_80022C90 passes all five). The one-argument
   call cannot be written through the real prototype, so it goes through an
   `__asm__`-aliased declaration, the same device csrc/gprel_72A8.c uses for
   func_80033AE8w. */
s32 func_80024C48w(s32 *p) __asm__("func_80024C48");

void func_80024C70(UnkCardGrpB20 *c, s32 n);
s32 *func_80024D58(s32 *p, s32 a, s32 b);
void func_80024E44(s32 *p);
void func_80024EEC(s32 *p);
void func_80025030(s32 *p);
void func_800250E8(s32 *p);
void func_800251A4(s32 *p);
void func_80025230(s32 *p);
void func_800252CC(s32 *p);
void func_80025398(s32 *p);
void func_80025584(s32 *p);
void func_8002570C(s32 *p);
void func_80025788(s32 *p);
void func_80025848(s32 *p);
void func_8002593C(s32 *p);
void func_80025B9C(s32 *p);
s32 *func_80025C7C(s32 *p, s32 a);
void func_80025D34(s32 *p);
void func_80025DC8(s32 *p);
void set_save_title(u8 *p, s32 n);
void set_save_icon(s32 *p, s32 v);
void func_800262BC(s32 *p);
void func_80026410(s32 *p);
void func_8002651C(s32 *p);
void func_800265B8(s32 *p);
s32 *func_80026868(s32 *p, s32 a);
void func_80026920(s32 *p);
void func_800269B4(s32 *p);
void func_80026B18(u8 *p, s32 *b, s32 *c);
void func_80026C94(UnkCardGrp044 *c);
void func_80026CF0(UnkCardGrp044 *c);
void func_80026D78(UnkCardGrp044 *c);
void func_80026DF0(UnkCardGrp044 *c, s32 n);
s32 *func_80026ED0(s32 *p, s32 a);
void func_80026FB4(u16 *p);
void func_800270E0(UnkCardGrp0D4 *c);
void func_800271C0(UnkCardPair *p, u32 *a, u32 *b);
void func_8002727C(UnkCardGrp0D4 *c, s32 n);
s32 *func_80027364(s32 *p);
void func_8002742C(UnkCardGrp0D4 *c);
void func_80027458(s32 *p);
void func_80027584(s32 *p);
void func_80027684(s32 *p);
void func_8002789C(s32 *p);
s32 *func_80027AFC(s32 a, s32 b);
void func_80027BF4(void);
void func_80027C90(s32 *p);
s16 func_80027DD0(void);
s32 func_80033B2C(s16 a);
void func_8003AD08(s32 n, u8 *buf);
s32 *func_8003FC64(s32 a);
s32 *func_8003FE78(s32 a, u8 *b, s32 c);
void func_80040AA0(s32 a);
void func_800439F8(void);
UnkNode *func_80044144(s16 a, s16 b, u32 c, u32 d);
s32 func_800491F4(s32 a, u8 *fmt, s32 n);
void func_80049D80(s32 p, s16 *r);
s32 func_80049DE8(s32 a, s32 b, s32 c);
s32 func_80049E1C(s32 a, s32 d, s32 m);
s32 func_80049E6C(s32 a);
s32 func_8004B4A8(s32 *p);
s32 func_8004CAA0(void);
void func_8004CB2C(s32 *p);
void func_8004CBFC(u32 *p, u8 *q, u32 *r);
u8 *func_8004DA74(u8 *out, u8 *s, s32 a);
void func_8004E264(u8 *p, s32 a);
s8 *func_8004E298(u8 *a, u8 *b, s32 c);
void func_8004E5A0(s32 v, s32 n, u8 *out);
u8 *func_8004E5E8(s32 v, u16 n, u8 *out);
void func_8004E69C(u8 *q);
void func_8004E884(u8 *p, s32 a);
void func_8004E994(s32 a);
s32 func_8004F418(s32 *p, s32 *q);
void func_8004F52C(s32 a);
void func_800585A0(void);
void func_8005863C(void);
void func_800A6104(void);

/* In town's/dungeon's overlay region above the engine, reached as a plain
   absolute call; the address comes from main.bin_undefined_funcs_auto.txt. */
void func_800A68C0(void);

void func_800A68F4(void);

/* Variables */
extern const char ASSERT_FMT[40];
extern const char FILENAME_C_SERVER[];
extern const UnkFnTab5 gSwCardEventHandlers;
extern const UnkFnTab5 gHwCardEventHandlers;
extern const char MEM_CARD1[8];
extern const char MEM_CARD0[8];
extern const char D_80020084[];
extern const u8 D_800200A0[4];
extern const u8 D_800200A4[];
extern const RECT SAVE_ICON_VRAM_RECT;

/* The overlay's own data, 0x80027E20..0x8002BB6C, past its text end. These
   are NOT externs into a splat blob any more -- csrc/ovl_main_80020000.c
   defines every one of them, at the foot of the file in address order, and
   they are declared here because almost every use site is above the
   definition. Declared as arrays because the listings materialise them with
   the lui/addiu pair rather than folding a %lo into the access. A sized
   declaration against a differently sized definition is a hard
   `conflicting types` error, which is one more gate for free -- it is what
   caught card_dir being 15 rows and not 10. */

/* The 1-frame animation script, spelled as what it is now that this unit
   defines it; func_8004CBFC's parameter is a `u8 *`, hence the casts at the
   three call sites. */
extern UnkAnimStep D_80027E68[2];
extern u8 SAVE_ICON_CLUTS[][0x20];
extern const char *MEM_CARD_NAMES[2];
extern char MEM_CARD_PREFIX[16];
extern const char *MEM_CARD_FILENAMES[6];
extern UnkBlock24 D_80027F48;
extern u8 D_80027F60[];
extern u8 D_80027F68[];
extern u8 D_80027F7C[];
extern u8 D_80027FC4[];
extern u8 D_80027FD0[];
extern u8 *D_80027FD4[];
extern u8 D_80027FF8[];
extern s16 D_80028010[];
extern u8 D_80028020[];
extern u8 SAVE_FILE_MAGIC[];
extern u8 D_80028068[][2];
extern s32 D_8002806C[];
extern u8 *D_80028074[];
extern u8 D_8002807C[];
extern u8 D_80028084[];
extern u8 D_8002808C[];
extern u8 D_80028094[];
extern void (*D_8002809C[6])(s32 *);
extern UnkStrSet gCardMessages[];
extern u8 *D_80028294[4];
extern u8 *D_800282A4[];
extern u8 D_800282B0[];
extern u8 *D_800282BC[];
extern u8 D_800282CC[];
extern u8 D_800282D8[];
extern u8 D_800282E8[];
extern u8 *D_800283A8[];
extern u8 *D_800283B8[];
extern u8 D_800283C4[];
extern u8 *D_800283EC[];
extern u8 *D_800283F4[];
extern u8 D_800283FC[];
extern u8 D_80028418[];
extern u8 D_80028430[];
extern s32 D_80028530[2];
extern s32 D_80028538[2];
extern s32 D_80028540[2];
extern s32 D_80028548[2];

extern long gSwCardEOFEvent;

extern long gSwCardErrorEvent;

extern long gSwCardTimeoutEvent;

extern long gSwCardNewEvent;

extern long gHwCardEOFEvent;

extern long gHwCardErrorEvent;

extern long gHwCardTimeoutEvent;

extern long gHwCardNewEvent;

/* 15 rows, not 10: the object runs 0x258 bytes to card_st and
   count_card_files/card_file_exists walk it 0x28 bytes at a time, which is
   sizeof(struct DIRENTRY) exactly. */
extern struct DIRENTRY card_dir[15];

extern s32 card_st;

extern s32 card_slot;

extern s32 D_800287D0[2];
extern s32 D_800287D8[2];
extern s32 card_evflg;

extern CARD_STATE gCardState;

extern u8 D_800287E8[];
extern s32 D_80028808;
extern u8 D_800291D0[];
extern u8 D_80029618[][0x3E4];
extern u8 D_8002A990[];
extern u8 D_8002B7E0[];
extern u8 D_8002B850[];
extern u8 D_8002B968[];
extern u8 D_8002BB08[];

/* The 12-byte cel records at the top of the overlay's data, declared here
   because the tables that point at them (D_80027E20, D_80027E68 and
   D_80027FD4) are defined 0x5D0 bytes lower and so come first in the C
   file. */
extern u8 D_80028448[];
extern u8 D_80028454[];
extern u8 D_80028460[];
extern u8 D_8002846C[];
extern u8 D_80028478[];
extern u8 D_80028484[];
extern u8 D_80028490[];
extern u8 D_8002849C[];
extern u8 D_800284A8[];
extern u8 D_800284B4[];
extern u8 D_800284C0[];
extern u8 D_800284CC[];
extern u8 D_800284D8[];
extern u8 D_800284E4[];
extern u8 D_800284F0[];
extern u8 D_800284FC[];
extern u8 D_80028520[];
extern u8 D_80077E84[];
extern u8 D_80077EF0[];
extern u8 D_80077EFC[];
extern u8 D_800786DC[];
extern u8 D_800786E8[];
extern u8 D_80078FD0[];
extern u8 D_80079054[];
extern u8 D_800790B4[];
extern u8 D_80079114[];
extern u8 D_800791A4[];

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

extern s32 D_80083200;

/* 128 bytes per entry; save_slot_used's `sll $a0, $a0, 7` is the stride and
   the 2D array spelling is what produces the lui/addiu pair plus the shift,
   where a scalar would fold differently. The element type is a guess that
   only has to make offset 0 a word. */
extern s32 D_80083E98[][32];
extern UnkSaveRow D_80083E98_s[] __asm__("D_80083E98");
extern s32 D_80083EA4[][32];
extern s32 D_80084118;
/* These two are the resident bss at 0x80084120/0x80084128, which this unit
   does not own -- the names come from splat/main.bin_undefined_syms_auto.txt
   and the linker script, so they are renamed through an `__asm__` alias
   rather than at the definition, as with D_80083E98_s above. */
extern s32 card_free_blocks[] __asm__("D_80084120");
extern s32 card_file_count[] __asm__("D_80084128");

#endif
