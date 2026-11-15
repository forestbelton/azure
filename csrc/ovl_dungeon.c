#include "common.h"

/* dungeon.bin's overlay at 0x80088760, loaded per floor when the player is in
   a dungeon. 863 functions, 85,616 words -- the largest single block in the
   project, and the same RAM slot town.bin's overlay occupies, so the two are
   mutually exclusive at runtime and an address in this range means different
   code depending on which is resident (docs/overlays.md).

   Built at cc1psx -G0, not the -G8 the resident executable uses; the
   Makefile's `ovl_%` pattern rule has the override and says why.

   THIS IS NOT TOWN'S OVERLAY WITH DIFFERENT DATA, and that was measured
   before any of it was written. Comparing all 863 functions against every
   landed function in the project by opcode stream finds 19 near-siblings at
   ratio >= 0.88, 550 words -- 0.6% of the block. The overlays share a handful
   of library-shaped routines and nothing else. See docs/dungeon-sweep.md.

   Types for this overlay's own data are declared HERE rather than in
   include/structs.h while they are still guesses, the same rule ovl_town.c
   follows: 0x800DCE60 and up is dungeon-private, and every edit to
   include/structs.h rebuilds every engine unit. */

/* The dungeon's cast scratchpad. func_800BCE7C is func_8008CE08 of
   csrc/ovl_town.c word for word with every field displacement 0xE8 higher --
   27 of the 80 words differ and all 27 by exactly +0xE8 -- so this is town's
   UnkTownCastSP behind a 0xE8-longer prefix, and the names are carried over
   from there. Only the fields that function witnesses are named; the prefix
   is unexplored space, not known filler. */
typedef struct UnkDungeonCastSP
{
    u8 pad_00[0x154];
    /* The four (x, z) delta pairs of the angular-span test, then the
       reference angle at +0x164 with the other three made relative to it,
       then the three consecutive gaps that get range-checked. */
    s16 unk_154;
    s16 unk_156;
    s16 unk_158;
    s16 unk_15A;
    s16 unk_15C;
    s16 unk_15E;
    s16 unk_160;
    s16 unk_162;
    s32 unk_164;
    s32 unk_168;
    s32 unk_16C;
    s32 unk_170;
    u8 pad_174[0x4];
    s32 unk_178;
    s32 unk_17C;
    s32 unk_180;
} UnkDungeonCastSP;

/* The state-machine installers below. Every one of them reaches a record's
   +0x8C and writes the handler the dispatcher runs next; twenty functions in
   three shapes witness that field and nothing else, so nothing else is named.
   The spelling follows csrc/ovl_town.c's func_800A4F4C, which is this
   overlay's family one record and one predicate over. */
typedef struct UnkDungeonActor
{
    u8 pad_00[0x8C];
    void (*unk_8C)();
    u8 pad_90[0x6];
    /* The step counter the 0x800D1C70..0x800D4634 state machines run down:
       written as a halfword, decremented in place and compared signed
       (`lhu` / `addiu -1` / `sh` / `sll 16` / `bgtz`), which is what an s16
       field gives. */
    s16 unk_96;
    u8 pad_98[0x2];
    /* func_8008D330 and func_80091920 park a state byte here and clear the
       one after it in the same breath as clearing the handler slot, which is
       the witness that +0x8C and +0x9A are fields of one record. */
    u8 unk_9A;
    /* The state index the +0x8C handlers switch on. */
    u8 unk_9B;
} UnkDungeonActor;

/* The 4-halfword rectangle func_800673A0 takes. The engine function has no
   declaration in include/structs.h; the `s16 *` signature is the one
   csrc/ovl_town_800BEBA4.c read off its own listing. Alignment 2 is the
   whole shape of the copy -- an `s16[4]` cannot be moved with aligned `lw`,
   so gcc emits the `lwl`/`lwr` + `swl`/`swr` pairs the listing has. */
typedef struct UnkDungeonRect
{
    s16 v[4];
} UnkDungeonRect;

/* The renderable func_800A7C64's node hands back at +0x0C: three colour
   bytes set to 0x80 and two halfword scales set to 0x1000, which is all
   that has a witness. */
typedef struct UnkDungeonSprite
{
    u8 pad_00[0xC];
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 pad_0F[0xD];
    s16 unk_1C;
    s16 unk_1E;
} UnkDungeonSprite;

/* Declarations precede all definitions in this file. */

/* The predicates. Each is `jal` then a test of $v0 against zero, so each
   returns a value; the parameter lists are not witnessed and are left open. */
s32 func_800AB378();
s32 func_800ABDBC();
s32 func_800AC010();
s32 func_800AC480();
s32 func_800AC5C8();
s32 func_800ACE34();
/* Called for effect by func_800B318C and as a predicate by func_800DA660. */
s32 func_800AC82C();
/* Called for effect only. */
void func_800AB538();
void func_800AB778();
void func_800AD058();
/* The second gate of the four-argument installers, whose result is tested as
   a HALFWORD -- `sll $v0, 16` then `blez`, with no `sra`. Two arguments. */
s32 func_800AD9B4();
/* The handlers these installers park in the +0x8C slot. An address-take needs
   a real declaration above it. */
void func_800B2A60();
void func_800D8C64();
void func_800D4494(u8 *, UnkVec *, u8 *, u8 *);
void func_800AA258(void);

/* The six flag probes and their three callers. func_800A6D30's result is
   masked to 16 bits at every use, so it is declared u16 rather than cast at
   three sites; func_800A48F0 is declared in include/structs.h, and the note
   there says why its return type was corrected from void. */
u16 func_800A6D30();
/* The two probe callers that are reached from above their own definitions;
   without these the call sites take an implicit `int f()` and cc1psx warns. */
s32 func_800C8844(u8 *, s16, s16);
s32 func_800C8980(u8 *, s16, s16);
s32 func_800C8900(u8 *, s16, s16);
void func_80099844(u8 *, u8 *);
void func_800DC1B8(void *);
extern u8 D_800E19C2[];
extern u8 D_800E19D3[];
extern u8 D_800E19E8[];
extern u8 D_800E1A01[];
extern u8 D_800E1A1D[];
extern u8 D_800E1A2D[];
extern void *D_800DCF0C;
extern void *D_800DCF18;
extern void *D_800DCF1C;

/* The `blez` gates of the three installers that test for a POSITIVE result
   rather than a non-zero one. */
s32 func_800AB464();
s32 func_800AC7EC();
/* The engine's rectangle setter; see UnkDungeonRect. */
void func_800673A0(s16 *r, s32 x, s32 y);
extern UnkDungeonRect D_80089490;
extern UnkDungeonRect D_80089498;
/* The engine's node allocator and its install call. func_8004491C is
   declared `s32 func_8004491C(UnkNode *, u32)` in include/structs.h, so the
   handler goes in as a cast. */
UnkNode *func_8003FC64(s32);
/* Parked at the node's +0x10 by the pair below; an address-take needs a
   declaration. */
void func_800A7CC4();
void func_800B51C8();

/* Engine and overlay globals this unit reaches. D_800E3D7C is the resident
   engine's, and its `u8 *` spelling is copied from csrc/unk_154BC.c, which
   walks the same +0xAC field at the same stride. */
extern u8 *D_800E3D7C;
extern u8 D_800E3548[];
extern u8 D_800E3648[];
/* The two parallel tables the four sweeps below walk, beside D_800E3548's
   and D_800E3648's 4-byte flag slots. They are STRUCT ARRAYS rather than
   byte pointers: walked as `u8 *` the two field reads become one giv biased
   by the later offset and the base is lost (`addiu $s0, $v0, 7`, reads at
   -1 and 0), where the target keeps the base and uses +6/+7 displacements. */
typedef struct UnkDungeonSpotA
{
    u8 unk_00;
    u8 unk_01;
    u8 pad_02[0x2];
    u16 unk_04;
    u8 pad_06[0x6];
} UnkDungeonSpotA;

typedef struct UnkDungeonSpotB
{
    u8 pad_00[0x6];
    u8 unk_06;
    u8 unk_07;
    u8 pad_08[0x4];
    UnkNode *unk_0C;
    u8 pad_10[0x2];
    u16 unk_12;
    u8 pad_14[0x4];
} UnkDungeonSpotB;

typedef struct UnkDungeonSlot
{
    u8 pad_00[0x1];
    u8 unk_01;
    u8 pad_02[0x1];
    u8 unk_03;
} UnkDungeonSlot;

/* The same two flag tables as D_800E3548/D_800E3648 above, under their
   struct face: the sweeps below index BOTH tables off the loop counter, and
   a hand-walked pointer for one of them is not a giv, which reverses the
   order loop.c emits the two initialisations in. */
extern UnkDungeonSlot D_800E3548_s[] __asm__("D_800E3548");
extern UnkDungeonSlot D_800E3648_s[] __asm__("D_800E3648");
/* The same slots as whole words, for func_800A7030's clear. */
extern UnkDungeonSpotA D_800E36C8[];
extern UnkDungeonSpotB D_800E39C8[];
void func_800B5F80(s32);
extern s32 D_800DCF60;
extern s32 D_8008335C;

/* The two gates of the 17-word pair below. */
s32 func_800D1A80();
void func_800A56C0();

/* The four-argument node walk's callees -- one per instance; see the walk
   family below. */
void func_800BBDEC(u8 *, UnkVec *, UnkSlot *, s16);
void func_800C55F4(u8 *, UnkVec *, UnkSlot *, s16);
void func_800C6B40(u8 *, UnkVec *, UnkSlot *, s16);
void func_800C9088(u8 *, UnkVec *, UnkSlot *, s16);
void func_800D07C8(u8 *, UnkVec *, UnkSlot *, s16);
void func_800D6170(u8 *, UnkVec *, UnkSlot *, s16);
/* The five-argument one. func_800CEEFC stores a fifth zero at 0x10($sp). */
void func_800CEFB8(u8 *, UnkVec *, UnkSlot *, s16, s32);

/* The 0x800D1C70..0x800D4634 state machines. D_8006CCD8/E8 are the engine's
   eight-entry sine and cosine tables, indexed by the facing field's top
   three bits exactly as csrc/ovl_town.c indexes them. */
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
void func_800AD4D0(u8 *);
void func_800AAA54(UnkDungeonActor *, UnkVec *, u8 *, s32);
void func_800A2B04(UnkVec *, s32, s32);
/* The handler each of the four copies parks; an address-take needs a
   declaration. func_800D4494 is declared with the installers above. */
void func_800D20D8();
void func_800D28E4();
void func_800D3140();

/* The per-frame step driver the cluster's entities share. The +0x8C slot and
   the D_800E2xxx tables both hold this shape -- the four-argument step the
   installers above park -- and the first and last arguments are the same
   record reached through two differently-typed pointers, which is why
   func_800D21CC takes it twice. */
typedef void (*UnkDungeonStep)(u8 *, UnkVec *, u8 *, u8 *);
extern UnkDungeonStep D_800E21C0[];
extern UnkDungeonStep D_800E2228[];
extern UnkDungeonStep D_800E2284[];
extern UnkDungeonStep D_800E22E0[];
/* The engine's global flag halfword at D_80083460 + 2. csrc/unk_154BC.c
   reaches the same object as `*(u16 *)((u8 *)&D_80083460 + 2)`; here it is a
   `%lo` displacement off its own `lui`, which is what a declared u16 gives. */
extern u16 D_80083462;
/* Two arguments at its definition in csrc/unk_1AF38.c, one at every call in
   this unit -- csrc/unk_25944.c already declares it short the same way. */
void func_800478B8(u8 *);
/* Returns the distance between two points as seen from a facing; the third
   argument is sign-extended at every call site and the result is used both
   raw and as a halfword, so the return is s32 and the casts are explicit. */
s32 func_800BCB04(s32, s32, s16);
/* The handler trio's callees. func_800A9A0C is declared `void func_800A9A0C(u8 *)`
   in csrc/unk_154BC.c and its result is never read here either. */
void func_800A9A0C(u8 *);
s32 func_800AA6B4(u8 *, UnkVec *, u8 *, u8 *);
extern u8 *D_80083470;

/* The cluster's spawners and what they reach. D_80083498 is declared
   UnkTownCam in include/structs.h; only its address is wanted here. */
UnkNode *func_8003FD64(s32, void *);
/* csrc/ovl_town.c declares the same two as `UnkTownCam D_80083498` and
   `u8 D_80045340[]`; only their addresses are wanted here, and the array
   spelling gives the same split lui/addiu the listing has. */
extern u8 D_80083498[];
extern u8 D_80045340[];
/* The step drivers and handlers the spawners park; the spawners sit ahead of
   them in address order, so each needs a prototype here. */
void func_800D1E34(u8 *, UnkVec *, u8 *);
void func_800D2664(u8 *, UnkVec *, u8 *);
void func_800D2E9C(u8 *, UnkVec *, u8 *);
void func_800D4158(u8 *, UnkVec *, u8 *);
extern u8 D_800DEEC0[];
extern u8 D_800DEF08[];
extern u32 D_800814A0;
extern s16 D_8008346A;
/* The kill handlers' callees and tables. D_80082E80 is csrc/ovl_town.c's
   UnkTownScene; only its address is wanted. */
extern u8 D_80082E80[];
extern u16 D_800DDE84[];
extern u8 D_800E0E69[];
extern u8 D_800E0E82[];
void func_800A63B8(u8 *, u8 *, s16);
s32 func_800AD6FC(u8 *, s32, s32);
void func_800A5F38(u8 *, u8 *);
void func_800C4AFC(u8 *, s32, u8 *);
void *func_800990FC(void);
void *func_80099194(u8 *, void *);
/* RETURNS the advanced cursor: it writes the terminating NUL and hands back
   p + 1, which is why the string chain can be nested. */
u8 *func_80099290(u8 *);
/* Not void: it leaves 0 in $v0 on the gated path and func_800353F4's answer
   on the other. Every caller here and in csrc/unk_154BC.c discards it, so the
   correction moves no byte. */
s32 func_800A5720(void *);
void func_80042B68(s8 *, s8);
void func_80098B38(u8 *);
void func_80041E70(u8 *);
extern u8 D_800E17A1[];
extern u8 D_800E2082[];
extern u8 D_800E0E9B[];
extern u8 D_800E0EE1[];
extern u8 D_800E0F07[];
void func_800997FC(u8 *);
s32 func_800A2424(u8 *, s32);
extern u8 D_800E173A[];
extern u8 D_800E1827[];
s32 func_800C8A3C(u8 *, s16, s16);
void func_800D4FC8(UnkNode *, s32, u16);
void func_800D5460(UnkNode *, s32, u16);
void func_800D4DE8();
void func_800D5294();
extern u8 D_800DECF8[];
extern u8 D_800E104E[];
extern u8 D_800E107C[];
extern u8 D_800E10C0[];
extern u8 D_800E10F2[];
/* The second kill skeleton's callees and strings. func_8008D344 is in this
   unit and is reached with four arguments, exactly like func_8008D330. */
void func_8008D344(u8 *, void *, void *, void *);
s32 func_80098864(u8 *, s32);
void func_800A6480(u8 *, u8 *);
void func_8009D8A4(void);
extern u32 D_800E296C;
extern u8 D_800E11FD[];
extern u8 D_800E1224[];
extern u8 D_800E1258[];
extern u8 D_800E1279[];
extern u8 D_800E12A9[];
/* The two kill handlers whose out-of-range arm spawns through func_800BBA40
   (seven arguments, three of them on the stack) instead of reporting. */
s32 func_800BBA40(s32, s32, s16, void *, s32, s32, void *);
/* Not void: it answers 1 or 0. Only its ADDRESS is taken -- it is
   func_800C13C8's seventh argument to func_800BBA40 -- which is why nothing
   had falsified it. */
s32 func_800C135C();
/* Not void: the loop's back branch carries `addiu $v0, $zero, 1` in its delay
   slot. Only its ADDRESS is taken, which is why nothing had falsified it. */
s32 func_800C152C();
extern u8 D_800DEAE0[];
extern u8 D_800DF514[];
extern u8 D_800E1482[];
extern u8 D_800E14A6[];
extern u8 D_800E1843[];
extern u8 D_800E206A[];
extern u8 D_800E1350[];
extern u8 D_800E2056[];
/* The two kill handlers that build a sentence. D_80089364 and D_80089368 are
   in this overlay's own rodata, two of the three the unit still carries as
   INCLUDE_RODATA. */
void func_8008D368(u8 *, void *, void *, void *, s32);
void *func_8009955C(u8 *, void *);
/* `u8 *`, not `void *`: a void * parameter forces a copy pseudo that lets
   combine collapse the whole *p++ chain into displacements, where the target
   keeps three `addiu`. */
u8 *func_80099978(u8 *);
void *func_80099368(u8 *, void *);
void func_800998C0(void *, void *);
extern u8 D_800DD148[];
extern u8 D_80089364[];
extern u8 D_80089368[];
extern u8 D_800E110B[];
extern u8 D_800E112A[];
extern u8 D_800E1156[];
extern u8 D_800E116E[];
extern u8 D_800DD158[];
extern u8 D_8008936C[];
extern u8 D_80089370[];
extern u8 D_800E118C[];
extern u8 D_800E11AB[];
extern u8 D_800E11D7[];
extern u8 D_80081485[];
extern s16 D_800E3CCE;
/* s8, not u8: func_800A67F4 reads it with `lb`. Its only other mention in
   this unit is a store, which is why the wrong signedness survived. */
extern s8 D_800DCF4F;
void func_80045C34();
extern u8 D_800E092B[];
extern u8 D_800E0A1B[];
extern u8 D_800E3D20;
extern u8 D_800DF3DC[];
extern u8 D_800DD000[];
extern u8 D_800E1B5D[];
void func_800CBDB4();
void func_800419EC(s32, s32);
s16 func_8009B88C(u8 *, s32, s32, s16 *, s16 *);
extern u8 D_800DCFF0[];
void func_80047784(u8 *, s32, s32);
void func_80094E34();
extern u8 D_800DD8E7[];
extern u8 D_800DD83C[];
extern u8 D_800DD840[];
extern u8 D_800DD844[];
extern u8 D_800DD848[];
extern u8 D_800DD84C[];
extern u8 D_800DD850[];
s16 func_80095464(u8 *, void *, u8 *, u8 *);
void func_800A0150(u8 *);
extern u8 D_800DD294[];
extern s16 D_80083228;
void func_80048A44(u8 *, s32, s32, s32);
void func_8009F644(u8 *, s32, s32, s32);
s32 func_800A5C70();
void func_800A5D5C();
void func_800BC1B4();
extern u8 D_800E2804[];
extern u8 D_800E2828[];
extern u8 D_800E28B8[];
extern u8 D_800E2970[];
extern u8 D_800DF3EC[];
extern u8 D_80081470[];
extern u8 D_8006E240[];
u32 func_8004A658(s32, s32);
extern u8 *D_800E3DE8;
extern u8 D_800DD80C[];
extern s16 D_80080B04;
extern u8 D_800E0DF5[];
extern u8 D_80081484[];
void func_8004B1A4(s32);
void func_800B835C(u8 *, s32 *, s32, s32);
s32 func_800CE4E8(s32, s32, s32, u8 *, s32);
void func_800D1824();
extern u8 *D_800DF55C;
extern void *D_800DF560;
extern u8 D_800E045D[];
extern u8 *D_800DCF64;
extern s32 D_800DD87C;
extern u8 D_800E58B8[];
extern u8 *D_800DD71C;
void *func_800C07AC(void *);
s16 func_8009FCF0(u8, u8, u8, u8);
UnkNode *func_8003FF2C(s32, UnkNode *, s16, UnkNode **);
void func_800C5FA8();
s16 func_800C8DB0(u8 *, s32, s32);
extern u8 D_800DD878;
extern u8 D_800DF3E4[];
extern u8 D_800E50A8[];
extern u8 D_800DD704[];
void func_800A90E8(u8 *);
void func_8009D3B0(void);
/* Parked at a node's +0x10 by func_800B1F00; an address-take needs one. */
void func_800B1E80();
extern u8 D_80089000[];
void *func_80099734(u8 *, void *);
extern s16 D_80083466;
extern u8 *D_800E5910;

/* D_8006DE24 is `Unk8006DE24 D_8006DE24[]` in include/structs.h, whose
   0x14-byte record names only +0x10; +0x13 is inside its trailing pad, so
   func_800A3820 reaches the same array under a second face rather than
   widening a shared declaration. The 0x13 has to arrive as a FIELD -- as
   `(u8 *)&D_8006DE24[i] + 0x13` it folds into the symbol's `%lo` and the
   load loses its displacement. */
typedef struct UnkDungeonName
{
    u8 pad_00[0x11];
    u8 unk_11;
    u8 pad_12[0x1];
    u8 unk_13;
} UnkDungeonName;
/* The two eight-direction probes. func_8009B25C is in this unit and sits
   above these, but a stub gives no prototype. */
u8 *func_8009B25C(u8 *, u16, u16, s16);
s16 func_8009A540(s32, s32, s32, s16);
s16 func_800A2CB8(u8 *, u8 *);
extern u16 D_8008347E;
s16 func_8009A350(s16, s16, s32, u16 *);
s16 func_800A41F0(u8 *);
/* The three 60-word effect handlers below. */
s32 func_800C8ABC(u8 *, s16, s16);
void func_800A6508(void);
void func_800C5BBC(s32, s32, s16, s32, s32, s32);
extern u8 D_800E3D40;
extern u8 D_800E1BF3[];
extern u8 D_800E1C03[];
extern u8 D_800E1C14[];
extern void *D_800DCF14;
/* The two engine-state gates below. func_80033BC0 is declared `void` in
   csrc/gprel_72A8.c and RETURNS a value anyway; csrc/ovl_town_800BF3F0.c
   records the same thing and says not to take that prototype, so this is
   the old-style form, which gives the value without the wrong type. */
s32 func_80033BC0();
void func_80040AA0(s32);
void func_8003F320(void);
extern u8 D_80082E6B;
extern u8 D_800DCF4E;
extern u8 D_800DF3CC[];
extern u8 D_800DF3D4[];
/* The two digit strips below. func_8004E57C is `void func_8004E57C(s32, u16,
   s32)` where it is defined; D_800808ED is the engine's per-character
   attribute table. */
void func_8004E57C(s32, u16, s32);
extern u8 D_800808ED[];
/* Returns the sound call's result; every call site in this unit drops it. */
s32 func_800A56E0(s32);
void func_800A32A4(u8 *);
void func_8009A028(u8 *);
void func_8009A3D0(s32, s32, s32);
s32 func_80042900(s8 *, s8);
s32 func_800D36D0(u8 *, UnkVec *, UnkSlot *);
void func_800D3738(u8 *, UnkVec *, UnkSlot *, s16);
void func_8003DB94(void *, void *, s32);
void func_80099FDC(UnkNode *);
void func_8009A21C(s32, s32, s32);
s16 func_8009FB34(u16, u16);
/* Neither is in include/structs.h; both spellings are
   csrc/ovl_town_800BF3F0.c's. func_80066460 returns u16 -- the call site
   masks with `andi $a3, $v0, 0xFFFF`. */
s32 func_80065420(SVECTOR *v0, s32 *sxy, s32 *p, s32 *flag); /* RotTransPers */
u16 func_80066460(s32 tp, s32 abr, s32 x, s32 y);            /* GetTPage */

/* The dungeon's 0x1F800000 scratchpad as the two tag callbacks below see it:
   the projection input vector at +0x04, the primitive cursor at +0x1C, the
   OT base at +0x24, RotTransPers' two out-parameters at +0xD0/+0xD4 and the
   resulting OT index at +0x100. Only those are named. */
typedef struct UnkDungeonSP
{
    u8 pad_00[0x4];
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    u8 pad_0A[0x12];
    u8 *unk_1C;
    u8 pad_20[0x4];
    u8 *unk_24;
    u8 pad_28[0xA8];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDungeonSP;

void func_800A5FC0();
void func_800A6194();
void func_800A624C();
void func_80096384();
void func_800D7FB8();
/* The 12-byte, alignment-1 object func_800A34BC copies out of the
   D_8007359C record into the actor's +0x34: an s16[4] or any 2-aligned type
   would move it with aligned `lw`, and the target's copy is the `lwl`/`lwr`
   + `swl`/`swr` pair an alignment of 1 forces. */
typedef struct UnkDungeonName12
{
    u8 b[0xC];
} UnkDungeonName12;
/* The 0x80083350 shift count, read into an s32 local by func_8009EE4C. */
extern s16 D_80083350;
extern s16 D_8008146E;
/* The eight-entry direction table func_80090200 hands to func_80048A44, the
   same shape as D_800DD000 next door. */
extern u8 D_800DD0E8[];
/* func_800B1E80's 0x7340 strip; indexed by a halfword, four bytes a row. */
extern u32 D_800DF068[];
void func_800A08A0(s32);
/* Five arguments, the fifth on the stack at 0x10($sp). */
void func_800A19E4(s32, u8 *, s16, s16, u8 *);
s32 func_800A7A7C(s16, s16, s16, s32, u8 *);
/* Parked at a node's +0x10 by func_800C4EB4 and func_800B341C; an
   address-take needs a declaration above it. */
void func_800C4E00();
void func_800B328C();
void func_800DC724(u8 *);
void func_800DCAC4(u8 *, s32);
u8 *func_800DCA1C(u8 *, s32);
/* The eight per-frame tag builders func_800DC8F8 runs; entry 0 is never
   called, the loop starts at 1. */
typedef void (*UnkDungeonTagFn)(u8 *, u8 *);
extern UnkDungeonTagFn D_800E2934[];
extern u8 D_800E3DB0[];
extern u8 D_800E2954[];
extern u8 D_800E2960[];
void func_8008EAC8();
void func_8008ACDC();
/* An 8-byte-stride table with three halfword fields: as a byte walk loop.c
   builds a SECOND address giv biased +4 and hoists the compared constant
   into a callee-saved register, which costs seven words. */
typedef struct UnkDungeonMark
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
} UnkDungeonMark;
extern UnkDungeonMark D_800E3CD8[];
void func_800BA414(u8 *, UnkVec *, UnkSlot *, s16);
typedef struct UnkDungeonCue
{
    void (*unk_00)(s16, s16, s16, s16);
    s16 unk_04;
    s16 unk_06;
    s16 unk_08;
    s16 unk_0A;
} UnkDungeonCue;
extern u8 D_800DF64C[];
extern UnkDungeonCue *D_800DF640[];
/* Handlers parked in a node's +0x10 by the two spawners below; an
   address-take needs a declaration above it. */
void func_800D7A78();
void func_800C4C00();
void func_8009BE2C();
/* The engine's twenty-entry word table func_800A4298 searches, beside the
   0x80010248 record array func_8009FADC hands out. */
extern s32 D_800E3DF0[];
/* The slot func_80094E34 parks D_80081484's word in. */
extern void *D_800E3540;
extern u8 D_800E1A44[];
extern u8 D_800E191B[];
/* The record func_800C78A0 fills before handing it to func_800C77D0. */
extern u8 D_800E58F8[];
/* func_80096A54 above calls this before its definition, so without a
   prototype the `void` return contradicts the implicit `int`. */
void func_80099F04(u8 *);
void func_800453E0(u8 *, UnkVec *, u8 *, s16);
extern u8 D_800E0B54[];
extern u8 D_800E0D51[];
extern u8 D_800E0D6D[];
extern u8 D_800E0EB4[];
extern u8 D_800E0EC9[];
extern u8 D_800E0FA4[];
extern u8 D_800E0FB9[];
extern u8 D_800E0FD7[];
extern u8 D_800E0FF4[];
extern u8 D_800E100F[];
extern u8 D_800E12D6[];
extern u8 D_800E1303[];
extern u8 D_800E13FB[];
extern u8 D_800E1402[];
extern u8 D_800E140A[];
extern u8 D_800E1411[];
extern u8 D_800E1426[];
extern u8 D_800E15FE[];
extern u8 D_800E1677[];
extern u8 D_800E187C[];
extern u8 D_800E18A4[];
extern u8 D_800E196F[];
extern u8 D_800E1987[];
extern u8 D_800E1996[];
extern u8 D_800E19AA[];
extern u8 D_80088CAC[];
extern u8 D_80089358[];
extern u8 D_8008935C[];
extern u8 D_80089380[];
extern u8 D_80089384[];
extern u8 D_800893DC[];
extern u8 D_800893E0[];
extern u8 D_8008942C[];
u8 *func_8003AD08(u32, u8 *);
void func_80042984(u8 *);
/* D_800E36C8's record as func_800A7A7C writes it. The fields have to arrive
   as COMPONENT_REFs, not casts off a `u8 *`: with MEM_IN_STRUCT_P clear the
   +0x08 store cannot be scheduled past the stack locals' `lh`s and the store
   comes out four words early (122 words, 4 differing). */
typedef struct UnkDungeonSpotD
{
    u8 unk_00;
    u8 unk_01;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
    s32 unk_08;
} UnkDungeonSpotD;
extern UnkDungeonSpotD D_800E36C8_u[] __asm__("D_800E36C8");
/* D_800E3548's four bytes and D_800E36C8's +0x08 word under struct faces:
   func_800C4030 walks both tables off ONE loop counter, and hand-walked
   `u8 *` cursors there make loop.c reduce the four byte references into a
   SECOND giv biased to +3 and hoist the stored constants into callee-saved
   registers -- 132 words against the target's 124. */
typedef struct UnkDungeonSlot4
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} UnkDungeonSlot4;
typedef struct UnkDungeonSpotC
{
    u8 pad_00[0x8];
    s32 unk_08;
} UnkDungeonSpotC;
extern UnkDungeonSlot4 D_800E3548_t[] __asm__("D_800E3548");
extern UnkDungeonSpotC D_800E36C8_t[] __asm__("D_800E36C8");
/* The record at the fixed RAM address 0x80013710 that func_8009F988 walks: a
   counter at +0x08, a second at +0x0A and the cursor base at +0x0C. The
   address arrives as a `lui`/`ori` pair, which is the constant-address
   spelling rather than a declared symbol, so there is no `D_` name for it. */
typedef struct UnkDungeon13710
{
    u8 pad_00[0x8];
    u16 unk_08;
    u16 unk_0A;
    u8 *unk_0C;
} UnkDungeon13710;
/* The stride-6 map-cell table func_800D1338 clears a bit in. The same object
   D_8008333C[0] points at; this is the only site that reaches it by its own
   symbol. */
typedef struct UnkDungeonTile
{
    u8 pad_00[0x4];
    u16 unk_04;
} UnkDungeonTile;
extern UnkDungeonTile D_800EA000[];
void func_800BAF04();
void func_800BC388(u8 *);
extern u8 D_800E0600[];
extern s32 D_8008346C;
extern u8 *D_800DD724;
u8 *func_8004AC3C(u8 *, s32 *);
s32 func_80069E98(u8 *);
void func_80069ED8(u8 *, u8 *, s32);
void func_800C2824();
u8 *func_800C9850(s32, s32, s32);
void func_8004A8D8(s32, s32);
void func_80035208(void *);
void func_80035348(void);
u8 *func_800A1618(s16, s16);
void func_800DC82C(u8 *);
s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
void func_800A4ACC(u8 *);
extern u8 D_800E260C[];
extern u8 D_800E045C[];
extern s32 D_800DD718;
extern u8 D_800E4958[];
/* The handler func_800A7700 parks at its node's +0x10; an address-take needs
   a declaration above it. */
void func_800A7548();
/* func_800BC26C's second install, handed to func_8004491C. */
void func_800BC3E4();
/* The halfword table func_800C0180 indexes with func_800429E4's result. */
extern s16 D_800DCED4[];
extern u8 *D_800E4938;
extern s32 D_80082EB8;
extern u8 D_800DEC70[];
extern u8 D_800E0C78[];
extern u8 D_800E0458[];
void func_800422DC(u8 *, s32);
s32 func_800448BC(u16, u16);
void func_80042518(s8 *, s32);
/* The two eight-way facing tables func_8008C468 picks between, beside
   D_800DD000 and D_800DD0E8 above, and the one func_8008C514 installs. */
extern u8 D_800DD030[];
extern u8 D_800DD050[];
extern u8 D_800DD0E0[];
extern u8 D_800E18C8[];
extern u8 D_800E1035[];
extern u8 D_800E0A03[];
extern u8 D_800E0A12[];
/* The pair func_800BA764 parks in its node; an address-take needs a
   declaration above it. */
void func_800BA6B8();
void func_800B7774();
void func_80042BDC(s8 *, s16);
void func_80044A50(UnkNode *);
/* D_800E2970's record, 0x14 bytes: an (x, y) origin at +0x00/+0x02 with the
   matching extents at +0x04/+0x06, a live halfword at +0x0A and a flag word
   at +0x0C. Indexed as an array of these it is ONE address giv, where a
   walked `u8 *` becomes two. The landed func_8009FCAC reaches +0x0A and
   +0x10 through a `u8 *` and is left alone. */
typedef struct UnkDungeonRoom
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u8 pad_08[0x2];
    s16 unk_0A;
    u16 unk_0C;
    u8 pad_0E[0x6];
} UnkDungeonRoom;
extern UnkDungeonRoom D_800E2970_s[] __asm__("D_800E2970");
/* The scratchpad frame func_800C95C0's tag walk uses. A different frame from
   UnkDungeonSP above, which is func_800D5AC0's. */
typedef struct UnkDungeonSP2
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x8A];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    s32 unk_B8;
    u8 pad_BC[0x4];
    s32 unk_C0;
} UnkDungeonSP2;
/* The dungeon's height map at D_800EA000 under a 6-byte record face, so that
   the +0x02 stays a MEM displacement rather than folding into the symbol's
   own `addiu`. */
typedef struct UnkDungeonCell
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
} UnkDungeonCell;
extern UnkDungeonCell D_800EA000_s[] __asm__("D_800EA000");
/* Defined below in this unit, and its address is taken above it. */
s32 func_800C6AEC(u8 *, UnkVec *, UnkSlot *);
/* THE 0x80013720 TABLE'S LIVE COUNT IS DECLARED TWICE UNDER ONE ASM NAME ON
   PURPOSE: func_8009F9E8 reads it in its loop bound and again in one arm, and
   with a single declaration cse folds the two `%hi` together. */
extern u16 D_80013716;
extern u16 D_80013716_b __asm__("D_80013716");
extern u8 D_800E2178[];
extern u8 D_800E221C[];

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800894A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80089588);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800897E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80089AA0);

void func_8008ACD4(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008ACDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008B870);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008B9FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C13C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C5C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C7B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C8BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CAA0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CBD4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CD4C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D024);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D1D0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D388);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D470);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D7D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D94C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D9F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008DB0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008DBE8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008DDE8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008E0C4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088760);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008E264);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008E504);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008E700);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008EAC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F428);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F6EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F878);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FA7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FD50);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FF18);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80090298);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80090548);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009074C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800909A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80090C10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091430);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091958);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091A38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091B5C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088904);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088934);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088970);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091C64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092018);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009255C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800927E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092BB8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092E90);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800930F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009345C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093A94);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093C70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093D8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093E74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009402C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094270);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800945E8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094988);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094F74);

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
s32 func_800954E0(u8 *p)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095538);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800956B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095854);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095A10);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095DFC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096100);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096384);

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
void func_80096AC0(u8 *p)
{
    p[0x9A] = 0x34;
    p[0x9B] = 0;
    *(void **)(p + 0x8C) = NULL;
    *(u16 *)(p + 0xA2) &= ~1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096AE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096C24);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009704C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800971A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800973F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800974DC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097934);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097AF8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097C78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097DB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097F84);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800982A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098614);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098864);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098920);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098B38);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098CF8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099194);

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

   THEY ARE SPELLED `func_80069E98` / `func_80069ED8`, NOT `strlen` /
   `memcpy`. Both names link and both measure the same bytes, but the `func_`
   spellings are the ones in splat/dungeon.bin_undefined_funcs_auto.txt, which
   is what the listing's relocations name and what keeps seqcheck's ordered
   symbol comparison clean. 32 words, 0 differing. */
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
    len = func_80069E98(src);
    func_80069ED8(dst, src, len);
    return dst + len;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099368);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009955C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009965C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099734);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800998C0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099A1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099B18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099C58);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A3D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A540);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A66C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A8C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ABA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ADB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009AF18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B164);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B4B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B5AC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B88C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009BE2C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009C12C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009C93C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009CD58);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009CE1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D218);

/* 12 words, 0 differing. */
void func_8009D380(void)
{
    func_8003DB4C((int *)D_800E50A8, 0x200);
    func_8009D3B0();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D3B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D6F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D8A4);

/* Append a 0xC-byte primitive to the list at D_80083160's +0x8D0 and hand it
   to func_8006658C.

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
    func_8006658C(tag, q);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009DAD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009DF0C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CA8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CAC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009E038);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009E798);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ED30);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F13C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F21C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F3D4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F644);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F868);

/* Step the text cursor back one entry and hand back where it now points.
   The record lives at the fixed RAM address 0x80013710, which arrives as a
   `lui`/`ori` pair -- the constant-address spelling, not a `D_` symbol.
   24 words, 0 differing. */
u8 *func_8009F988(void)
{
    UnkDungeon13710 *p;
    u8 *q;
    s32 c;

    p = (UnkDungeon13710 *)0x80013710;
    c = p->unk_0A;
    q = p->unk_0C + c * 2;
    if (p->unk_08 == 0)
    {
        q -= 2;
        p->unk_0A = c - 1;
        if (*(s8 *)q >= 0)
        {
            p->unk_08 = *q - 1;
        }
        else
        {
            p->unk_08 = 0;
        }
    }
    else
    {
        p->unk_08 = p->unk_08 - 1;
    }
    return q;
}

/* Search the 0x80013720 table downward for the entry whose type nibble is
   0x70, 0x78, 0x80 or 0x90 and whose +0x01 byte and 0x20 bit match (a, b).

   TWO SEPARATE DEVICES KEEP THE ARM'S READ WHERE IT IS, both ablated from
   this body. (1) The dead `c = 0;` ahead of the loop: without it loop.c
   hoists the arm's whole `base + n * 2` out and cse2 folds it onto the
   preheader load -- 45 words of 49, 36 differing. loop.c ~695 clause (3),
   `reg_in_basic_block_p`, opens on `regno_first_uid`, and the dead set moves
   that off the in-loop set. (2) The second `extern` under the same `__asm__`
   name: with the plain name in the arm the two `%hi(D_80013716)` are one
   pseudo and the arm gets `move $t0, $v1` where the target has its own `lui`
   -- 49 words, 3 differing.

   The type test must also be the `||` chain with the early return in the
   `else`: an `&&` chain of `!=` makes the return arm the fall-through, 47
   words of 49, 31 differing. 49 words, 0 differing. */
u8 *func_8009F9E8(u8 a, u8 b)
{
    s32 i;
    u8 *base;
    u8 *q;
    s32 t;
    u16 c;

    base = (u8 *)0x80013720;
    /* DEVICE: a dead set, so that `c`'s first mention is not the one inside
       the loop. flow deletes it and it costs no word. */
    c = 0;
    for (i = D_80013716 - 1; i >= 0; i--)
    {
        q = base + i * 2;
        t = q[1] & 0xF8;
        if (t == 0x70 || t == 0x78 || t == 0x80 || t == 0x90)
        {
            if (q[1] == a && (q[0] & 0x20) == b)
            {
                return q;
            }
        }
        else
        {
            c = D_80013716_b;
            return base + c * 2;
        }
    }
    return base;
}

/* TWO out-of-image accesses, so a BASE LOCAL. One access written as a plain
   constant address stays absolute and the assembler expands it with
   `lui $at`; two want the constant-base idiom, which gives one bare `lui`
   in an allocatable register and two displacements. `$at` in a listing is
   the tell for the first shape. 6 words, 0 differing. */
void func_8009FAAC(void)
{
    u8 *base = (u8 *)0x80010000;

    *(u16 *)(base + 0x3718) = 0;
    *(u16 *)(base + 0x371A) += 1;
}

/* ONE out-of-image access, so a plain constant address -- the assembler's
   `lui $at` expansion; see func_8009FAAC. 6 words, 0 differing. */
void func_8009FAC4(void)
{
    *(void **)0x8001371C = (void *)0x80013720;
}

/* 22 words, 0 differing. */
void *func_8009FADC(s16 x)
{
    s16 i;

    i = x - 1;
    if (i < 0x14)
    {
        return (u8 *)0x80010248 + i * 4;
    }
    if (i == 0x14)
    {
        return D_80081484;
    }
    return *(void **)(D_800E3D7C + 0xF0);
}

/* Find the D_800E2970 region containing tile (x, y). The lower-bound tests
   are `sltu` because both operands are u16 and shorten_compare keeps the
   comparison narrow; the upper-bound tests are `slt` because the sum is an
   int.

   THE BOUND IS READ IN THE `for` CONDITION, NOT INTO A LOCAL AHEAD OF IT. A
   local loads straight into the bound's register and the function is 46 words
   of 47 -- the target loads into $v0 for the guard and COPIES it to $t0,
   which is what the inline read gives. AND THE RECORD MUST BE INDEXED AS AN
   ARRAY OF STRUCTS: a hand-walked `u8 *` makes loop.c build two address givs,
   one biased by +6, and that is 48 words / 24 differing.
   47 words, 0 differing. */
s16 func_8009FB34(u16 x, u16 y)
{
    s32 i;

    for (i = 0; i < D_8008146E; i++)
    {
        if (D_800E2970_s[i].unk_0A != 0 && x >= D_800E2970_s[i].unk_00 &&
            x < D_800E2970_s[i].unk_00 + D_800E2970_s[i].unk_04 && y >= D_800E2970_s[i].unk_02 &&
            y < D_800E2970_s[i].unk_02 + D_800E2970_s[i].unk_06)
        {
            return i;
        }
    }
    return -1;
}

/* The room-rectangle lookup over D_800E2970's 0x14-byte records.

   TWO THINGS ARE FORCED. The record address is `D_800E2970 + i * 0x14` INSIDE
   the body, so loop.c makes it one giv; walking a `u8 *r` by hand gives TWO
   induction variables -- a second biased by 6, with the later fields read at
   -4/-2/0 -- 48 words against 47 and 22 differing. And the bound must be read
   in the loop CONDITION: a local is a second quantity and costs the whole
   prologue, 46 words / 42 differing. 47 words, 0 differing. */
s16 func_8009FBF0(u16 x, u16 y)
{
    s32 i;
    u8 *r;

    for (i = 0; i < D_8008146E; i++)
    {
        r = D_800E2970 + i * 0x14;
        if (*(s16 *)(r + 0xA) != 0 && x >= *(u16 *)r - 1 && *(u16 *)r + *(u16 *)(r + 4) >= x &&
            y >= *(u16 *)(r + 2) - 1 && *(u16 *)(r + 2) + *(u16 *)(r + 6) >= y)
        {
            return i;
        }
    }
    return -1;
}

/* `b = D_800E2970;` AS ITS OWN STATEMENT: both `&D_800E2970[i * 0x14]` and
   `D_800E2970 + i * 0x14` put the symbol's lui/addiu AFTER the index chain
   and sit at 6 differing. 17 words, 0 differing. */
s32 func_8009FCAC(s16 i)
{
    u8 *b;
    u8 *p;

    if (i < 0)
    {
        return 0;
    }
    b = D_800E2970;
    p = b + i * 0x14;
    if (*(s16 *)(p + 0xA) == 0)
    {
        return 0;
    }
    return *(s32 *)(p + 0x10);
}

/* Chebyshev distance between two tiles, and `abs()` IS THE WHOLE FUNCTION.
   The `bgez`/nop/`negu` trio plus the surviving `sll 16`/`sra 16` on the
   return is the abs() tell at docs/matching-levers.md:4615. A conditional
   negate loses BOTH -- reorg fills the trio's nop and combine's
   reg_sign_bit_copies then proves the s16 return extension redundant and
   deletes it: 17 words against 20. Nineteen other spellings were swept at
   15 differing; one near-miss, `s32` parameters with the operands masked in
   place, reproduces the target's register allocation and its cross-jumped
   tail exactly and still sits at 2 -- a right diagnosis of the allocation
   and a wrong one of the negate. 20 words, 0 differing. */
s16 func_8009FCF0(u8 x0, u8 y0, u8 x1, u8 y1)
{
    s32 dx;
    s32 dy;

    dx = abs(x1 - x0);
    dy = abs(y1 - y0);
    if (dx < dy)
    {
        return dy;
    }
    return dx;
}

/* 15 words, 0 differing. */
s16 func_8009FD40(u8 *a, u8 *b)
{
    return func_8009FCF0(a[0x24], a[0x25], b[0x24], b[0x25]);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FD7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FE94);

/* Walk the eight neighbouring tiles and report the first that passes:
   D_8006CCD8 and D_8006CCE8 are the engine's sine and cosine tables, read
   here as u16 and added to the record's tile, and the index is handed back
   through the caller's halfword as `i << 9` -- the same facing encoding
   csrc/ovl_town.c decodes with `(ang >> 9) & 7`.

   func_8009FB34 RETURNS s16 AND TAKES u16: the `andi $a0, $a0, 0xFFFF`
   before the call is a u16 parameter truncating, and the `sll $v0, 16`
   after it is a halfword result being tested signed. The spawners above
   store its result with `sb` and so witness neither.
   44 words, 0 differing. */
s32 func_8009FFD4(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) < 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* func_8009FFD4 with the test inverted -- `bltz` for `bgez`, the first
   neighbour that FAILS. 44 words, 0 differing. */
s32 func_800A0084(u8 *p, s16 *out)
{
    s32 i;

    for (i = 0; i < 8; i++)
    {
        if (func_8009FB34(p[0x24] + ((u16 *)D_8006CCD8)[i], p[0x25] + ((u16 *)D_8006CCE8)[i]) >= 0)
        {
            *out = i << 9;
            return 1;
        }
    }
    return 0;
}

/* 7 words, 0 differing. */
s16 func_800A0134(u8 *a, u8 *b)
{
    return *(u16 *)(a + 0x88) - *(u16 *)(b + 0x88);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0150);

/* Six bit tests in priority order, each arm a string address, and the NULL
   test after the chain is real: cc1psx does not fold `&array != 0`.
   40 words, 0 differing. */
void func_800A020C(s32 f)
{
    u8 *s;

    if (f & 0x10000000)
    {
        return;
    }
    if (f & 0x8)
    {
        s = D_800DD844;
    }
    else if (f & 0x20)
    {
        s = D_800DD850;
    }
    else if (f & 0x40)
    {
        s = D_800DD848;
    }
    else if (f & 0x400)
    {
        s = D_800DD840;
    }
    else if (f & 0x10)
    {
        s = D_800DD84C;
    }
    else
    {
        s = D_800DD83C;
    }
    if (s != NULL)
    {
        func_800A0150(s);
    }
}

/* Try the eight neighbouring tiles starting from the engine's current
   facing (D_8008347E & 7) and hand back the first that both func_8009B25C
   accepts and func_8009A540 confirms. The direction wraps with `& 7` while
   a separate counter runs 0..7, which is why there are two induction
   variables. 70 words, 0 differing. */
u8 *func_800A02AC(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
            {
                return r;
            }
        }
    }
    return NULL;
}

/* func_800A02AC with a func_800A2CB8 gate between the two.
   75 words, 0 differing. */
u8 *func_800A03C4(u8 *p, u16 x, u16 y)
{
    s32 i;
    s32 d;
    u8 *e;
    u8 *r;

    e = *(u8 **)(p - 0x14);
    d = D_8008347E & 7;
    for (i = 0; i < 8; i++, d = (d + 1) & 7)
    {
        r = func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                          *(s16 *)(p + 0x88));
        if (r != NULL)
        {
            if (func_800A2CB8(p, r) != 0)
            {
                if (func_8009A540(d, e[0x24], e[0x25], *(u16 *)(p + 0x88) - 0x20) != 0)
                {
                    return r;
                }
            }
        }
    }
    return NULL;
}

/* `dir` is UNSIGNED -- as s32 the shift is `sra` where the target has
   `srl`. 22 words, 0 differing. */
void func_800A04F0(u8 *p, s32 x, s32 y, u16 dir)
{
    s32 d;

    d = (dir >> 9) & 7;
    func_8009B25C(p, x + ((u16 *)D_8006CCD8)[d], y + ((u16 *)D_8006CCE8)[d],
                  *(s16 *)(p + 0x88));
}

/* Is (x, y) outside the map? 1 when it is. TWO EARLY RETURNS, NOT AN `&&`
   CHAIN: the positive form is the same 23 words and 8 differing, with the
   four branches' delay slots and the two constant materialisations
   redistributed. 23 words, 0 differing. */
s32 func_800A0548(s16 x, s16 y)
{
    s16 *sh = (s16 *)D_8008333C;

    if (x < 0 || x >= (1 << sh[10]))
    {
        return 1;
    }
    if (y < 0 || y >= (1 << sh[11]))
    {
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A05A4);

/* 18 words, 0 differing. */
s16 func_800A07D0(s16 a, s16 b, s16 c, s16 d)
{
    return func_80065F90(d - b, c - a);
}

/* func_800A07D0's angle with a quadrant flag written back through the fifth
   (stack) argument.

   THE RESULT IS KEPT IN A `u16` COPY, AND THE RETURN EXPRESSION IS NARROWED
   TO MATCH. The target masks the CALL'S OWN $v0 while a copy of it lives in
   $v1 for the tail. A single `s32 ang` cannot do that: cse's make_regs_eqv
   short-circuits on `firstr < FIRST_PSEUDO_REGISTER`, so the pseudo always
   becomes canonical and every later use is rewritten to it -- nine spellings
   all sit at 1 differing. A NARROWER copy is invisible to cse, because
   `(set (reg:HI r) (subreg:HI ...))` is not a same-mode REG, and MIPS emits
   it as a plain `addu`. The widening back has to be free too:
   `(r + 0x100) & 0xE00` on a u16 is 35 words, one `andi 0xFFFF` too many.
   34 words, 0 differing. */
s32 func_800A0818(s16 a, s16 b, s16 c, s16 d, u16 *f)
{
    s32 ang;
    u16 r;

    ang = func_80065F90(d - b, c - a);
    r = ang;
    if ((ang & 0x1FF) >= 0x100)
    {
        *f |= 2;
    }
    else
    {
        *f &= 0xFFFD;
    }
    r += 0x100;
    return r & 0xE00;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A08A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0B94);

/* Run the D_80083460 + 0x6 countdown down and, when it goes negative, reseed
   it from the LCG with 0x20 forced on. 27 words, 0 differing. */
void func_800A0DD8(void)
{
    u8 *g;
    s16 t;

    if (D_8008146E != 0)
    {
        g = (u8 *)&D_80083460;
        t = *(u16 *)(g + 0x6) - 1;
        *(s16 *)(g + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(g + 0x6) = (func_800A6D30() & 0x1F) | 0x20;
            func_800A08A0(0);
        }
    }
}

/* Park a random nibble in the engine's D_80083466.

   THE CALL IS ITS OWN STATEMENT AND THE MASK IS NOT. Spelled
   `D_80083466 = func_800A6D30() & 0xF;` the assignment expands its LHS
   address FIRST, so the `%hi` lands in the jal's delay slot in a
   callee-saved register and the function is two words longer; with the
   mask folded into the call's own statement the `lui` comes out after the
   `andi`. 10 words, 0 differing. */
void func_800A0E44(void)
{
    s32 v;

    v = func_800A6D30();
    D_80083466 = v & 0xF;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0E6C);

/* The `q == NULL` arm is laid out INLINE here, so it is the second arm of the
   `if`, not an early return: `if (q == NULL) { return 0; }` puts the
   `return 0` at the END and measures 33 words / 12 differing. Both spellings
   are 33 words, so only the differing count separates them.
   33 words, 0 differing. */
s32 func_800A152C(s16 a, s16 b)
{
    u8 *q;
    u8 *g;

    if (a == 0x39)
    {
        a = 2;
    }
    q = func_800A1618(a, b);
    if (q != NULL)
    {
        q[1] = a;
        q[2] += 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0x1C) += 1;
        return 1;
    }
    return 0;
}

/* Is the given kind (0x39 read as 2) in the six-entry table at
   D_800E3DB0 + 1, stride 8?

   THE TABLE POINTER IS ASSIGNED BEFORE THE `if`. Left in the `for`
   initialiser it is 26 words with 16 differing: the `lui`/`addiu` pair lands
   after the 0x39 test instead of at the top, and every register in the
   function moves with it. `i = 0` stays in the `for` -- hoisting that too is
   6 differing. 26 words, 0 differing. */
s32 func_800A15B0(s16 k)
{
    s16 i;
    u8 *p;

    p = D_800E3DB0;
    if (k == 0x39)
    {
        k = 2;
    }
    for (i = 0; i < 6; i++, p += 8)
    {
        if (p[1] == k)
        {
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A1618);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A17E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A18E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A19E4);

/* Walk the engine's own ring and run func_800A19E4 on every member. The head
   is RE-READ out of D_800E3D7C at the bottom of each turn rather than held in
   a local, which is the second `lw %lo(D_800E3D7C)($s3)` inside the loop; the
   two s16 parameters have their `sll` hoisted into the preheader and their
   `sra` left in the body. 35 words, 0 differing. */
void func_800A1B44(s16 x, s16 y)
{
    u8 *p;

    p = *(u8 **)(D_800E3D7C + 0x5C) + 0x20;
    while (p != D_800E3D7C)
    {
        func_800A19E4(*(s32 *)(p - 0x14), p, x, y, p + 0x9C);
        p = *(u8 **)(p + 0x5C) + 0x20;
    }
}

/* The same search over the engine's four-entry table, keyed on a POINTER
   field rather than a byte: +0xAC in one and +0xE4 in the other. The base is
   read from D_800E3D7C ONCE and walked, which is what keeps the `lw` out of
   the loop.

   func_800A1BD0 IS DECLARED IN include/structs.h, as `s32 func_800A1BD0(u8
   *)`, because a resident engine unit calls this address -- and the engine
   reaches whichever overlay is loaded, so that declaration is about this
   function. It is adopted here rather than guessed at, and it is what fixes
   the key as a `u8 *`: csrc/unk_154BC.c writes the same +0xAC field with
   `*(u8 **)(((s16)k) * 4 + D_800E3D7C + 0xAC) = nw;`. The s32 return then
   makes the index's `sll`/`sra` an explicit cast rather than a consequence
   of the return type. */
s32 func_800A1BD0(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xAC) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

s32 func_800A1C14(u8 *key)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3D7C; i < 4; i++, p += 4)
    {
        if (*(u8 **)(p + 0xE4) == key)
        {
            return (s16)i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A1C58);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A1D4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2424);

/* 8 words, 0 differing. */
void func_800A2AE4(void)
{
    u8 *g;

    D_800DCF4F = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

/* Tile (x, z) to the halfword world coordinates at +0x2 and +0x6. The
   `(s16)` casts are what give the `sll 16`/`sra 10` pairs, and neither
   field is an UnkVec member, so both go in as offsets.
   9 words, 0 differing. */
void func_800A2B04(UnkVec *p, s32 x, s32 z)
{
    *(s16 *)((u8 *)p + 0x2) = ((s16)x) * 64 + 0x20;
    *(s16 *)((u8 *)p + 0x6) = ((s16)z) * 64 + 0x20;
}

/* func_80094EA4's shape over two different globals, nested for the same
   reason. 13 words, 0 differing. */
s32 func_800A2B28(void)
{
    if ((D_80013714 & 2) == 0)
    {
        if (D_800E3D20 != 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Claim D_80083460 + 0xC for the caller's object unless anything else is
   pending, then report func_800A2B28's answer. ONE `||` CHAIN, NOT FOUR
   `if`s, for the same reason func_800A2C34 above records: each test's
   `addiu $v0, $zero, 1` is reorg duplicating ONE `return 1` into four delay
   slots. 32 words, 0 differing. */
s32 func_800A2B5C(void *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(void **)(g + 0xC) != d)
    {
        if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0 ||
            (*(u16 *)(g + 0x2) & 8))
        {
            return 1;
        }
        *(void **)(g + 0xC) = d;
    }
    return (s16)func_800A2B28();
}

/* Four engine flags, any of which blocks whatever the caller is about to do.

   A COMMON RESULT VARIABLE, NOT FOUR EARLY `return 1`s. Written as four
   `if (...) return 1;` and a trailing `return 0`, jump.c store-flags the LAST
   pair into `j` + `sltu $v0, $zero, $v0` and the fourth `bnez` is gone: 22
   words, 5 differing. An `&&` chain whose fall-through returns 0 is the same
   5, and so is a `goto`-shared `return 1`. Only the explicit if/else over one
   variable keeps all four branches -- which CORRECTS the escape the
   store-flag lever records. 22 words, 0 differing. */
s32 func_800A2BDC(void)
{
    u8 *g;
    s32 r;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s32 *)(g + 0x8) != 0 ||
        (*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        r = 1;
    }
    else
    {
        r = 0;
    }
    return r;
}

/* ONE `||` CHAIN, NOT THREE `if`s: separate returns are 17 words / 8
   differing because jump.c's store-flag transform turns the last pair into
   `sltu`. Sharing one `return 1` lets reorg duplicate the `addiu $v0, 1`
   into all three delay slots. 17 words, 0 differing. */
s32 func_800A2C34(void)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0xC) != 0 || *(s32 *)(g + 0x10) != 0 || *(s16 *)(g + 0xA) != 0)
    {
        return 1;
    }
    return 0;
}

/* Two accesses off D_80083460, so a base pointer. The result accumulating
   in $v1 and moving to $v0 at the `jr` is the `||`. 13 words, 0 differing. */
s32 func_800A2C78(void)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    return *(s32 *)(g + 0xC) != 0 || *(s16 *)(g + 0xA) != 0;
}

/* 3 words, 0 differing. */
s32 func_800A2CAC(u8 *p)
{
    return p[0x25] < 0x40;
}

/* THE TAIL'S XOR MUST LAND IN AN `s32` LOCAL FIRST. Written as one
   expression, the s16 return's truncation is folded back into both loads and
   they come out `lhu` -- 44 words, 2 differing. The local keeps the two `lw`
   and leaves the narrowing as the trailing `sll`/`srl` pair. 44 words,
   0 differing. */
s16 func_800A2CB8(u8 *a, u8 *b)
{
    s32 t;

    if (b == NULL)
    {
        return 0;
    }
    if ((*(u32 *)(a + 0x1C) & 0x410) != 0)
    {
        return 1;
    }
    if (a[0x13] == 0x2B && (*(u16 *)(a + 0x98) & 0x8000) != 0)
    {
        return 1;
    }
    if ((*(u32 *)(b + 0x14) & 0x4000) != 0 && (*(u32 *)(a + 0x14) & 0x4000) != 0)
    {
        return (s16)func_80042900((s8 *)a, 0xC) != 0;
    }
    t = *(u32 *)(a + 0x1C) ^ *(u32 *)(b + 0x1C);
    return t & 0x2000;
}

/* The read and the add are SEPARATE STATEMENTS: folded into one expression
   the `or`'s result takes $v0 instead of reusing the low byte's $v1.
   20 words, 0 differing. */
void func_800A2D68(u8 *p, u16 n)
{
    u32 v;

    v = (p[0x41] << 8) | p[0x42];
    v += n;
    if (v > 0xFFFF)
    {
        v = 0xFFFF;
    }
    p[0x41] = v >> 8;
    p[0x42] = v;
    D_800E296C |= 0x100000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2DB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2FE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A31D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A32A4);

/* Copy the twelve-byte name the D_8007359C record at +0x04 points at into
   the actor's +0x34, then drop a NUL on its last word's first byte. The
   copy's type has to have ALIGNMENT 1 -- see UnkDungeonName12 -- which is
   what gives the `lwl`/`lwr` + `swl`/`swr` pairs rather than aligned `lw`.
   23 words, 0 differing. */
void func_800A34BC(u8 *p)
{
    *(UnkDungeonName12 *)(p + 0x34) = *(UnkDungeonName12 *)D_8007359C[0][p[0x13]].unk_04;
    p[0x3C] = 0;
}

/* A capacity predicate: true once +0x29 over half of +0x11 plus two has
   caught up with +0x28.

   THE DIVISOR IS ITS OWN STATEMENT -- the target loads +0x11 before +0x29,
   which is the divisor's expression first; inline, the dividend goes first.
   And `>> 1`, not `/ 2`: the value comes out of an `lbu`, so combine knows
   the sign bit is clear and the shift stays the `srl` the target has, where
   a signed divide by two costs three words. 11 words, 0 differing. */
s32 func_800A3518(u8 *p)
{
    s32 d;

    d = (p[0x11] >> 1) + 2;
    return p[0x29] / d >= p[0x28];
}

/* func_800A35A4's record lookup as a scan over three stride-3 slots.

   TWO PREHEADER FACTS, each worth measuring: the subscript `p[i * 3 + 8]` is
   what loop.c reduces to the target's `addiu $a0, $a0, 0x3` biv, while
   walking `p += 3` myself leaves the giv's own `addiu $a0, $a0, 8` in the
   preheader and is 25 words / 24 differing; and `t` has to be written INSIDE
   the loop, because the target's preheader sets `$a2` (i) before the
   `%hi(D_8006DE24)`, which the hoist reproduces and an assignment ahead of
   the `for` does not (24 words / 3 differing, the three being just those two
   insns and the `j`). Hoisting `p[8]` into a local is 24 / 10.
   24 words, 0 differing. */
s16 func_800A3544(u8 *p, u8 k)
{
    UnkDungeonName *t;
    s32 i;

    for (i = 0; i < 3; i++)
    {
        t = (UnkDungeonName *)D_8006DE24;
        if (p[i * 3 + 8] != 0 && t[p[i * 3 + 8]].unk_11 == k)
        {
            return (s16)i;
        }
    }
    return -1;
}

/* func_800A3820's +0x13 read with the index taken out of a record. THE
   TABLE ARRIVES AS A POINTER LOCAL, not as a symbol added to the index:
   added, it expands SECOND and the `lui` lands after the multiply, which is
   what func_800A3820 wants and this one does not (9 differing). `p += i *
   3;` as its own statement is what keeps the update in $a0.
   13 words, 0 differing. */
u8 func_800A35A4(u8 *p, s32 i)
{
    UnkDungeonName *t;

    t = (UnkDungeonName *)D_8006DE24;
    p += i * 3;
    return t[p[8]].unk_13;
}

/* A three-bit cross-product: for each bit of `a`, the bit one place round the
   3-cycle of `b` adds and the bit one place the other way subtracts.

   THE PARAMETER WIDTH IS WORTH TWO WORDS. Declared `s32`, gcc keeps both
   parameters in $a0/$a1, the first block's `sltu` writes straight into the
   counter, and the function is 31 words. Declared sub-word, the two incoming
   copies `addu $a3, $a0, $zero` / `addu $a2, $a1, $zero` survive (a sub-word
   copy of a word register is not the no-op jump2 deletes) and all 33 words
   match. u8, s8, u16 and s16 all measure 33 / 0 here, so the callee cannot
   pick between them; u16 is what the CALLERS witness -- func_800A40AC passes
   an `lhu` result with no mask, which rules out u8, and with no `sll`/`sra`,
   which rules out s16. 33 words, 0 differing. */
s16 func_800A35D8(u16 a, u16 b)
{
    s16 r;

    r = 0;
    if (a & 1)
    {
        if (b & 4)
        {
            r++;
        }
        if (b & 2)
        {
            r--;
        }
    }
    if (a & 2)
    {
        if (b & 1)
        {
            r++;
        }
        if (b & 4)
        {
            r--;
        }
    }
    if (a & 4)
    {
        if (b & 2)
        {
            r++;
        }
        if (b & 1)
        {
            r--;
        }
    }
    return r;
}

/* Two tile positions are "in line" when their x and y distances are equal or
   either one is zero.

   BOTH abs() ARE THE `bgez`/nop/`negu` TRIO, and the spelling matters: the
   TERNARY form `dx = dx >= 0 ? dx : -dx;` writing back into its own variable
   is 22 words / 14 differing, every difference a register. Written into a
   SEPARATE variable the ternary is 0, and `abs()` inline is 0 and the same
   bytes. Computing both differences before either abs is one word SHORT: the
   second pair of `lbu` fills the first subtraction's load-delay slot, which
   the target leaves as a `nop`. 22 words, 0 differing. */
s32 func_800A365C(u8 *a, u8 *b)
{
    s32 dx;
    s32 dy;
    s32 r;

    dx = abs(a[0x24] - b[0x24]);
    dy = abs(a[0x25] - b[0x25]);
    r = 0;
    if (dx == dy || dx == 0 || dy == 0)
    {
        r = 1;
    }
    return r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A36B4);

/* Read +0x13 out of the D_8006DE24 record; see UnkDungeonName above. The
   index is a separate statement, or fold hands the symbol to expand first
   and the `lui` comes out ahead of the multiply. 11 words, 0 differing. */
u8 func_800A3820(s16 i)
{
    s32 off;

    off = (s16)i * 0x14;
    return ((UnkDungeonName *)(off + (u8 *)D_8006DE24))->unk_13;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A384C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3D18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3F28);

/* Is either of the engine's two slots holding a record whose +0x46 is flagged
   and whose low 14 bits sit in 5..7?

   THE RANGE TEST MUST BE PARENTHESISED. Written
   `(t & 0x8000) && (t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7` the `&&` chain
   associates left, fold never sees the two bounds as one range, and it is 25
   words against 24 with 15 differing -- two `sltiu`s instead of the target's
   `addiu -5`/`sltiu 3`. 24 words, 0 differing. */
s32 func_800A404C(void)
{
    s32 i;
    u8 *p;
    u8 *q;
    u16 t;

    for (i = 0, p = (u8 *)D_800814A8; i < 2; i++, p += 4)
    {
        q = *(u8 **)(p + 0xAC);
        if (q != NULL)
        {
            t = *(u16 *)(q + 0x46);
            if ((t & 0x8000) && ((t & 0x3FFF) >= 5 && (t & 0x3FFF) <= 7))
            {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A40AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A41F0);

/* Find the entry of the 0x80010248 record array whose +1 byte is 0x13 and
   whose +3 byte indexes a D_800E3DF0 word equal to the key; return that
   index. THE CURSOR MUST BE A GIV, not a pointer walked from a statement
   before the loop: as `q = (u8 *)0x8001024B;` ahead of the `for` it is a
   SOURCE statement and sits at the head of the preheader, where the target
   emits it LAST, after the counter, the 0x13 and the table base -- 26 words
   at 5 differing, all of them preheader order. 26 words, 0 differing. */
s32 func_800A4298(s32 key)
{
    s32 i;
    u8 *q;
    s32 k;

    for (i = 0; i < 0x14; i++)
    {
        q = (u8 *)0x8001024B + i * 4;
        if (q[-2] == 0x13)
        {
            k = q[0] & 0x1F;
            if (D_800E3DF0[k] == key)
            {
                return k;
            }
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4300);

/* 18 words, 0 differing. */
u8 *func_800A442C(void)
{
    u8 *r;

    if (D_80081470[1] < 0x14)
    {
        r = *(u8 **)(D_800814A8 + 0xF0);
        if (r != NULL)
        {
            return r;
        }
        *(s32 *)D_80081470 = 0;
    }
    return D_80081470;
}

/* Find (x, y) among the four D_800E3CD8 marks. SUBSCRIPT THE STRUCT ARRAY;
   DO NOT WALK A `s16 *`: the walk makes TWO address givs -- a base walked by
   8 and a second biased by 4, read at -2 and 0 -- and the counter moves to
   $t0, 29 words / 29 differing. 27 words, 0 differing. */
s32 func_800A4474(s16 x, s16 y)
{
    s32 i;
    s16 n;

    for (i = 0; i < 4; i++)
    {
        n = D_800E3CD8[i].unk_00;
        if (n < 2)
        {
            return 0;
        }
        if (x == D_800E3CD8[i].unk_02 && y == D_800E3CD8[i].unk_04)
        {
            return n;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A44E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A45D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4688);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4778);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A48F0);

/* Four 2-byte records scanned DOWNWARD from +0x32; the counter and the -2
   cursor are the target's `bgez`/`addiu -2` pair. 12 words, 0 differing. */
s32 func_800A4A9C(u8 *p)
{
    s32 i;

    for (i = 3; i >= 0; i--)
    {
        if (*(s8 *)(p + i * 2 + 0x2C) == 0)
        {
            return 1;
        }
    }
    return 0;
}

/* Run the four two-byte slots at +0x2C/+0x2D down from the top.

   THE SUBSCRIPT IS WRITTEN AT EVERY SITE, NOT WALKED. `s = p + 6;` with
   `s -= 2` in the loop's third clause folds the +0x2C into the biv's
   initialiser, so the cursor starts at p + 0x32 and the two accesses come out
   at displacements 0 and 1 where the target keeps p + 6 and +0x2C/+0x2D: 47
   words, 8 differing, every one a displacement. The degenerate `bgez` after
   the `blez` is the `else if (d < 0)` arm's own test, not a redundancy.
   47 words, 0 differing. */
void func_800A4ACC(u8 *p)
{
    s32 i;
    s32 c;
    s32 d;
    u8 v;

    for (i = 3; i >= 0; i--)
    {
        c = *(s8 *)(p + i * 2 + 0x2C);
        if (c != 0)
        {
            if (c == 5)
            {
                func_800A5DFC(p, *(s32 *)(p - 0x18));
            }
            d = *(s8 *)(p + i * 2 + 0x2D);
            if (d != 0)
            {
                if (d > 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))--;
                }
                else if (d < 0)
                {
                    (*(u8 *)(p + i * 2 + 0x2D))++;
                }
                if (*(s8 *)(p + i * 2 + 0x2D) == 0)
                {
                    v = *(u8 *)(p + i * 2 + 0x2C);
                    *(u8 *)(p + i * 2 + 0x2C) = 0;
                    func_80042BDC((s8 *)p, (s8)v);
                }
            }
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089000);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4B88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A504C);

/* The bound test reads the counter SIGNED (`lh`) and the increment reads it
   UNSIGNED (`lhu`) off the same `lui`; that pair is what `s16` plus a bare
   `++` gives. 12 words, 0 differing. */
s32 func_800A5690(void)
{
    if (D_800E3CCE < 0x20)
    {
        D_800E3CCE++;
        return 1;
    }
    return 0;
}

/* Saturating countdown. 8 words, 0 differing. */
void func_800A56C0(void)
{
    if (D_800E3CCE != 0)
    {
        D_800E3CCE -= 1;
    }
}

/* A COMMON RETURN VARIABLE, as in func_800A9E38: with two `return`s jump.c
   inverts the test and moves the `return 0` past the call block. The result
   is the sound call's, which is why the prototype is s32 and not void --
   every call site in this unit drops it. 16 words, 0 differing. */
s32 func_800A56E0(s32 id)
{
    s32 r;

    if (D_80013714 & 2)
    {
        r = 0;
    }
    else
    {
        r = func_80053DA8(id);
    }

    return r;
}

/* Walk the message to its terminator, fold a shift-JIS second byte in
   0x81..0x9A down by 0x21 after an 0x82 lead byte, and hand the message to
   func_800353F4.

   THE EARLY EXIT IS WRITTEN LAST. As `if (D_80013714 & 2) return 0;` at the
   top the `return 0` is laid out inline and the body jumps round it: 36
   words against 37, 30 differing. Wrapping the body in the inverted test and
   leaving `return 0` as the final statement puts the zero at the epilogue
   where the target has it -- so the rule is about where the EPILOGUE ends
   up, not about which arm is trivial. The `(u8)` on `c - 0x81` is where
   `andi 0xFF` comes from, and combine rewrites the constant to +0x7F under
   that mask. 37 words, 0 differing. */
s32 func_800A5720(void *msg)
{
    u8 *p;
    u8 c;

    if ((D_80013714 & 2) == 0)
    {
        p = (u8 *)msg;
        while (*(s8 *)p > 0)
        {
            p++;
        }
        if (p[0] == 0x82)
        {
            c = p[1];
            if ((u8)(c - 0x81) < 0x1A)
            {
                p[1] = c - 0x21;
            }
        }
        return func_800353F4((u32)msg);
    }
    return 0;
}

/* Which of the three stride-3 slots holds a +8 byte in the same group of
   three as the given one. BOTH DIVISIONS ARE WRITTEN INSIDE THE LOOP AND THE
   INVARIANT ONE IS THE LEFT OPERAND. Computing `(n - 1) / 3` into a local
   ahead of the loop gives the magic constant TWICE -- once for it and once
   hoisted for the body's own division, 31 words at 27 differing; written
   inline in the comparison, cse unifies them in the body and loop.c hoists
   one. With the operands the other way round it is 29 words at 1 differing.
   29 words, 0 differing. */
s16 func_800A57B4(u8 *p, s16 n)
{
    s32 i;
    s32 c;

    for (i = 0; i < 3; i++, p += 3)
    {
        c = p[8];
        if (c != 0 && (n - 1) / 3 == (c - 1) / 3)
        {
            return (s16)i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5828);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5A18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5AF0);

/* Is any visible ring member standing within four tiles of D_80082E80's
   (0x24, 0x25)? THE RING HEAD IS READ FIRST and the flag word second: the
   other order puts the flags in $a0 and the head in $a1's place, 4
   differing, although the target emits the two `lui`s head-first and the
   two loads flags-first. Both abs() pairs are the `bgez`/nop/`negu` trio.
   53 words, 0 differing. */
s32 func_800A5C70(void)
{
    u8 *p;
    u8 *q;
    u8 *end;
    u8 *g;
    u16 f;

    p = D_800E3D7C;
    f = D_80013714;
    if (f & 2)
    {
        return 1;
    }
    if (f & 9)
    {
        return 0;
    }
    if (D_800E296C & 0x200000)
    {
        return 0;
    }
    g = D_80082E80;
    end = p;
    do
    {
        if (!(*(s32 *)(p + 0x1C) & 0x2208))
        {
            q = *(u8 **)(p - 0x14);
            if (abs(g[0x24] - q[0x24]) < 5 && abs(g[0x25] - q[0x25]) < 5)
            {
                return 0;
            }
        }
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != end);
    return 1;
}

/* 6 words, 0 differing. */
void func_800A5D44(void)
{
    *(s32 *)0x8001362C = D_8008347E;
}

/* func_800999D4's close, counted up instead of down: bump the record's own
   halfword and at 0x10 retire it. 24 words, 0 differing. */
void func_800A5D5C(u8 *p)
{
    u8 *g;

    if (++*(s16 *)p >= 0x10)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* THE REUSED TEMPORARY MUST BE THE NODE'S OWN VARIABLE. Two separate
   locals, in either order, are 15 words / 10-12 differing: sched1
   interleaves the two split address pairs and fills the lhu's load-delay
   slot. A separate reused `u8 *g` alongside `UnkNode *n` is 16 / 2 -- right
   shape, wrong registers, because the second (high)/(lo_sum) pair fails to
   tie. Reusing `n` itself ties them. 16 words, 0 differing. */
void func_800A5DBC(void)
{
    u8 *n;

    n = (u8 *)func_8003FC64(0);
    *(u32 *)(n + 0x10) = (u32)func_800A5D5C;
    n = (u8 *)&D_80083460;
    *(u16 *)(n + 0xA) += 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5DFC);

/* The kill handlers' shared report: turn the record's name pair into a
   message id, announce it at the record's tile, play 0x50C, release the
   message and drop the D_80083460 + 0xA live count. The name pair goes in as
   `(q[1], q[0])`, the same order func_800A7A38 records. The record the tile
   comes from is the one at p - 0x18. 34 words, 0 differing. */
void func_800A5F38(u8 *p, u8 *q)
{
    u8 *r;
    u8 *g;
    u32 t;

    r = *(u8 **)(p - 0x18);
    t = func_8004A658(q[1], q[0]);
    func_800A7A7C(*(u16 *)(r + 0x2) >> 6, *(u16 *)(r + 0x6) >> 6, *(s16 *)(r + 0xA), t, q);
    func_800A56E0(0x50C);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5FC0);

/* The base local is advanced halfway through, which is where the target's
   `addiu $v1, $v1, 0x20` is. 14 words, 0 differing. */
void func_800A6104(void)
{
    u8 *p;

    p = D_800DCF64;
    if (p != NULL)
    {
        *(s32 *)(p + 0x20) = 0xFFFFFF;
        p += 0x20;
        *(s32 *)(p + 0x4) = 0x40;
        *(s16 *)(p + 0xC) = 1;
    }
}

/* 22 words, 0 differing. */
UnkNode *func_800A613C(void)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        func_8004491C(n, (u32)func_800A5FC0);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 6;
        *(s16 *)(p + 0xC) = 0x10;
    }
    return n;
}

/* A node's +0x10 step: fade the level at +0x0A up to 0xFF over the frames
   left at +0x08.

   THE `|=` ON D_800E296C HAS TO BE SPLIT AROUND THE +0x08 STORE. gcc cannot
   prove the store through `p` misses the global, so written as one compound
   assignment the `sh` is pinned either ahead of the `lw` or behind the `sw`,
   and the target has it BETWEEN them. All six orderings of the arm's three
   statements were measured: 3, 3, 4, 8 differing and two at 44 words.
   46 words, 0 differing. */
void func_800A6194(u8 *p)
{
    s32 t;
    s32 f;

    t = --*(s16 *)(p + 0x8);
    if (t > 0)
    {
        *(s16 *)(p + 0xA) += (0xFF - *(s16 *)(p + 0xA)) / t;
    }
    else
    {
        *(s16 *)(p + 0xA) = 0xFF;
        f = D_800E296C;
        *(s16 *)(p + 0x8) = 0;
        D_800E296C = f | 0x2000000;
    }
    if ((D_800E296C & 0x1000000) == 0)
    {
        D_800E296C &= ~0x2000000;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A624C);

/* The width of arg0 is NOT witnessed: the only caller passes a constant 0
   and the callee only stores it with `sh`, so s16 and s32 are the same
   bytes. s16 matches the landed func_800BC228, one constant over.
   27 words, 0 differing. */
UnkNode *func_800A634C(s16 arg0)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A6194;
        func_8004491C(n, (u32)func_800A624C);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x8) = 8;
        *(s16 *)(p + 0xC) = arg0;
    }
    return n;
}

/* func_800A6480's message with the noun chosen by `k`. The whole chain is
   duplicated into both arms and cross-jumping merges it back from the
   `jal func_80099194` onwards: one shared func_80099368 call ahead of the
   `if` is 46 words of 50 and 43 differing.

   THE `__asm__` MARKER IS A DEVICE and it is the whole difference between 0
   and 9. Without it `q` (3 references over 16 insns, 1 * 3 / 16) loses $s1 to
   the handle pseudo (4 over 34, 2 * 4 / 34) in global.c's allocno_compare.
   The marker's fourth reference on `q` crosses floor_log2's step (2 * 4 / 16
   = 5000 against 2352) and emits nothing. Seven structural spellings were
   ruled out at 50 words / 9 differing, so no source spelling reaches it.
   50 words, 0 differing. */
void func_800A63B8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    /* DEVICE: a zero-instruction reference on q, worth the whole 9-differing
       residue above and no words. */
    __asm__("" ::"r"(q));
    if (k == 0x15)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A03, h);
    }
    else
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E0A12, h);
    }
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* The smallest of the nineteen: the reused-temporary idiom and nothing
   else. See func_800BD45C for why `h` is the name that is written again.
   34 words, 0 differing. */
void func_800A6480(u8 *p, u8 *q)
{
    void *m;
    void *h;

    h = func_800990FC();
    m = h;
    h = func_80099368(q, h);
    h = func_80099194(D_800E0A1B, h);
    h = func_80099734(p, h);
    h = func_80099194(D_80089000, h);
    func_80099290(func_800999B0(h));
    func_800A5720(m);
}

/* 9 words, 0 differing. */
void func_800A6508(void)
{
    func_800997FC(D_800E092B);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A652C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6620);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6780);

/* D_800DCF4F IS READ WITH `lb`, so its declaration above was corrected from
   u8 to s8; as u8 the test is `lbu` and the word is wrong. 31 words,
   0 differing. */
void func_800A67F4(void)
{
    u8 *g;

    if ((D_800E296C & 0x200000) == 0)
    {
        if ((D_80013714 & 8) == 0)
        {
            if (D_800DCF4F != 0)
            {
                D_800DCF4F = 0;
                g = (u8 *)&D_80083460;
                *(u16 *)(g + 0xA) -= 1;
            }
            func_80035208(D_800E045C);
            func_80035348();
        }
    }
}

/* 20 words, 0 differing. */
s32 func_800A6870(s16 x)
{
    if (x == 0xFF)
    {
        x = 0x1E;
    }
    return (x >> 2) + (func_800A6D30() & 3);
}

/* Save bit 4 of the engine record's +0x1C flag word, then drop it. The
   field is re-read for the mask because the intervening byte store
   invalidates cse's entry. 13 words, 0 differing. */
void func_800A68C0(void)
{
    u8 *p;

    p = D_800E3D7C;
    D_800DD878 = (*(u32 *)(p + 0x1C) >> 4) & 1;
    *(u32 *)(p + 0x1C) &= ~0x10;
}

/* func_800A68C0's restore. 13 words, 0 differing. */
void func_800A68F4(void)
{
    u8 *p;

    if (D_800DD878 != 0)
    {
        p = D_800E3D7C;
        *(u32 *)(p + 0x1C) |= 0x10;
    }
}

/* func_80098FF8's base rule again; here splat could not pattern-match the
   `lui 0x8001`/`ori 0x4C` at all, which is the proof there is no
   relocation. 21 words, 0 differing. */
s16 func_800A6928(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 n;
    s32 i;

    t = (u8 *)0x80010000;
    key = (a << 8) | b;
    n = 0;
    for (i = 0x13; i >= 0; i--)
    {
        if (**(u16 **)(t + i * 4 + 0x29C) == key)
        {
            n++;
        }
    }
    return n;
}

/* func_800A6928's backward scan over a second 64-entry stride-4 table.

   THE BASE CONSTANT CARRIES THE WHOLE ADDRESS: `t = (u8 *)0x80011F80` and
   `t + i * 4`. Written func_800A6928's way -- 0x80010000 with the table
   offset in the index, as `+ 0x1F80`, as `(i + 0x7E0) * 4`, or as
   `&((u32 *)t)[i + 0x7E0]` -- fold distributes the constant back out and
   loop.c leaves it in the load's DISPLACEMENT, so the giv's initial value is
   0x800100FC: 26 words, 2 differing. 26 words, 0 differing. */
s16 func_800A697C(u8 a, u8 b)
{
    u8 *t;
    u16 key;
    s32 i;

    t = (u8 *)0x80011F80;
    key = (a << 8) | b;
    for (i = 0x3F; i >= 0; i--)
    {
        if (*(u16 *)(t + i * 4) == key)
        {
            return 1;
        }
    }
    return func_800A6928(a, b);
}

/* Sixteen calls with a rising s16 argument from 2, then one per unit of
   func_800A6DA4's halfword result.

   THE RISING ARGUMENT IS `(s16)(i + 2)`, NOT AN `s16` COUNTER. With a second
   variable `s16 k = 2; ... k++` the sign extension stays at the use and the
   loop costs two extra words. Cast inline, loop.c strength-reduces
   `(i + 2) << 16` into the induction variable the target walks by 0x10000.
   The second bound is masked at the ASSIGNMENT: declared `u16` the mask
   lands at the guard instead and costs a word. 34 words, 0 differing. */
void func_800A69E4(void)
{
    s32 i;
    s32 n;

    n = func_800A6DA4(4, 8) & 0xFFFF;
    for (i = 0; i < 0x10; i++)
    {
        func_800A08A0((s16)(i + 2));
    }
    for (i = 0; i < n; i++)
    {
        func_800A08A0(1);
    }
}

/* A two-byte stack record handed to a ONE-argument callee -- the `jal`'s
   delay slot is the second `sb`, not an argument. 12 words, 0 differing. */
void func_800A6A6C(void)
{
    u8 buf[2];

    buf[0] = 1;
    buf[1] = 0x12;
    func_800A90E8(buf);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6A9C);

/* 19 words, 0 differing. */
void func_800A6B70(u8 *p, s32 b, UnkDungeonSprite *s)
{
    s32 v;

    v = *(s32 *)(p + 0xC) + 1;
    *(s32 *)(p + 0xC) = v;
    v = (func_800644B8(v * 64) >> 7) + 0x60;
    s->unk_0E = v;
    s->unk_0D = v;
    s->unk_0C = v;
}

/* `s = step;` FIRST, `q = p + 2;` SECOND. With the s16 parameter used
   directly the sign-extension pair is scheduled to the end of the preheader
   (4 differing in every control-flow spelling); copied out first the
   preheader comes out sll / sra / addiu q / acc = 0. A `goto` back-edge is
   12-17 differing -- loop.c never sees the loop and the two cursors are not
   givs. 17 words, 0 differing. */
void func_800A6BBC(u8 *p, s16 step)
{
    s32 s;
    u8 *q;
    s32 acc;

    s = step;
    q = p + 2;
    acc = 0;
    for (;;)
    {
        *q += acc;
        acc += s;
        if (*(s8 *)p >= 0)
        {
            q += 0xC;
            p += 0xC;
            continue;
        }
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6C00);

/* The LCG. D_80083160 + 0x1FC is the seed, and with two accesses it arrives
   as a base pointer with folded displacements rather than as D_8008335C.
   12 words, 0 differing. */
u16 func_800A6D30(void)
{
    u8 *g;
    s32 v;

    g = (u8 *)&D_80083160;
    v = *(s32 *)(g + 0x1FC) * 0x41C64E6D + 0x3039;
    *(s32 *)(g + 0x1FC) = v;
    return ((u32)v >> 16) & 0x7FFF;
}

/* ONE BASE LOCAL FOR BOTH ARMS: the `addiu %lo(D_80083160)` sits in the
   `beqz` delay slot and both sides use it. The halfword parameter is only
   tested, so its `sra` is combined away and the bare `sll` is what is left.
   14 words, 0 differing. */
void func_800A6D60(s16 flag)
{
    u8 *g;

    g = (u8 *)&D_80083160;
    if (flag != 0)
    {
        D_800DD87C = *(s32 *)(g + 0x1FC);
    }
    else
    {
        *(s32 *)(g + 0x1FC) = D_800DD87C;
    }
}

void func_800A6D98(s32 v)
{
    D_8008335C = v;
}

/* A random number in the inclusive range the two arguments bound, in either
   order.

   ONE LOCAL DOES THE SWAP AND THE DRAW. With a separate `s32 r` for the call
   result -- or with the call written inline, or `lo + ...` operand order, or
   signed parameters with casts, or `lo > hi` for `hi < lo` -- the
   `andi $v0, 0xFFFF` lands in $v1 and the divisor in $v0: 27 words, 4-5
   differing. Reusing the swap temporary for the draw gives that pseudo the
   second set that keeps it in $v0. The divisor needs its own local either
   way. 27 words, 0 differing. */
s32 func_800A6DA4(u32 lo, u32 hi)
{
    u32 t;
    u32 n;

    if (hi < lo)
    {
        t = hi;
        hi = lo;
        lo = t;
    }
    t = func_800A6D30();
    n = hi - lo + 1;
    return (u16)(t % n + lo);
}

/* COUNT the live slots standing on a tile -- func_800A7030's sweep with a
   counter instead of a clear. Both tables are indexed off the loop counter
   for the reason recorded there, and the count is initialised before the
   counter, which is the `addu $a3, $t0, $zero` the listing opens with.
   31 words, 0 differing. */
s16 func_800A6E10(s16 x, s16 y)
{
    s32 n;
    s32 i;

    n = 0;
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            if (D_800E36C8[i].unk_00 == x)
            {
                if (D_800E36C8[i].unk_01 == y)
                {
                    n++;
                }
            }
        }
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6E8C);

/* Clear any of the 0x40 live slots standing on the given tile and report it.
   D_800E3548's +0x01 is the slot's live flag and D_800E36C8 holds the tile.

   BOTH TABLES ARE INDEXED OFF THE COUNTER, not walked as pointers, and the
   tile bytes are held in LOCALS. Three measurements behind that: a `u8 *`
   walk over the wide table collapses its two field reads into one giv
   biased by the later offset and loses the base (`addiu $s0, $v0, 7`, reads
   at -1 and 0) where the target keeps the base and uses +6/+7; a
   hand-initialised pointer for the narrow table is a biv rather than a giv,
   which reverses the order loop.c emits the two initialisations in; and
   passing `D_800E36C8[i].unk_00` to the call a second time costs two
   reloads, because the word store to the slot between them invalidates
   cse's memory. 45 words, 0 differing. */
void func_800A7030(s16 x, s16 y)
{
    s32 i;
    u8 cx;
    u8 cy;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0)
        {
            cx = D_800E36C8[i].unk_00;
            if (cx == x)
            {
                cy = D_800E36C8[i].unk_01;
                if (cy == y)
                {
                    *(s32 *)&D_800E3548_s[i] = 0;
                    func_8009A3D0(cx, cy, 0x800);
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A70E4);

/* Find the first free slot of a stride-4 table by its +0x01 byte, returning
   the index as a halfword and -1 when the table is full. The index is cast
   on the way out -- that is the `sll`/`sra` pair in the delay slots -- while
   the -1 needs no widening, which is why only one arm has it. */
s16 func_800A71F4(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3548; i < 0x40; i++, p += 4)
    {
        if (p[1] == 0)
        {
            return i;
        }
    }
    return -1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7234);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7548);

/* The node spawner for func_800A7548's entity. THE THIRD PARAMETER IS `s32`
   although it is only ever `sh`-stored: as s16 (or u16) its HImode pseudo
   outranks the other three in global-alloc and the four parameter copies come
   out $s0/$s2/$s3/$s1 against the target's $s1/$s3/$s0/$s2 -- 43 words, 13
   differing, every one a register. Caching n->unk_08 in a local is required:
   reaching it twice is 46 words / 31. 43 words, 0 differing. */
UnkNode *func_800A7700(s32 x, s32 y, s32 ang, s32 *q)
{
    UnkNode *n;
    UnkVec *m;

    n = (UnkNode *)func_800A8608(D_80083498, q, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    n->unk_10 = (u32)func_800A7548;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = (x << 6) + 0x20;
    *(s16 *)((u8 *)m + 0x6) = (y << 6) + 0x20;
    *(s16 *)((u8 *)m + 0xA) = ang;
    *(s32 *)((u8 *)n + 0xB8) = *q;
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A77AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A794C);

/* ONE RESULT VARIABLE, NOT TWO RETURNS: the call block is a branch target
   rather than a fall-through, so cse's table does not reach it and p[1] is
   reloaded for the call. Two returns let jump.c swap the arms (7
   differing). 17 words, 0 differing. */
u32 func_800A7A38(u8 *p)
{
    u32 r;

    if (p[1] == 0x12)
    {
        r = (u32)D_8006E240;
    }
    else
    {
        r = func_8004A658(p[1], p[0]);
    }
    return r;
}

/* Claim a stride-4 slot for a new spot, or report that it could not be done.
   The record's fields go in through UnkDungeonSpotD rather than a `u8 *`
   cast: see that type's comment for the four words it costs otherwise.
   122 words, 0 differing. */
s32 func_800A7A7C(s16 x, s16 y, s16 z, s32 u, u8 *w)
{
    void *m;
    void *h;
    s32 i;
    UnkDungeonSpotD *r;
    s16 a;
    s16 b;
    s16 c;

    if ((s16)func_800A7234(x, y, z, &a, &b, &c) != 0)
    {
        i = func_800A71F4();
        if (i >= 0)
        {
            if (func_800A794C(x, y, z, w, a, b, c) == 0)
            {
                *(s32 *)&D_800E3548_s[i] = *(s32 *)w;
                r = &D_800E36C8_u[i];
                r->unk_00 = a;
                r->unk_01 = b;
                r->unk_04 = c;
                r->unk_02 = c;
                r->unk_08 = func_800A7A38(w);
                func_8009A21C(a, b, 0x800);
                func_800A4300(D_80082E80, D_800E3D7C);
            }
            return 1;
        }
    }
    func_800A7700(x, y, z, w);
    h = func_800990FC();
    m = h;
    h = func_80099368(w, h);
    h = func_80099194(D_800E0B54, h);
    func_80099290(h);
    func_800A5720(m);
    return 0;
}

/* Allocate a kind-2 node, park a handler at its +0x10 and set the
   renderable it carries to white at unit scale. The `lw 0xC` and the
   `sw $zero, 0x10` both happen BEFORE the install call, which is where the
   reads of the node have to be: func_8004491C clobbers $v0. */
void func_800A7C64(void)
{
    UnkNode *n;
    UnkDungeonSprite *s;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        n->unk_10 = 0;
        func_8004491C(n, (u32)func_800A7CC4);
        s->unk_0E = 0x80;
        s->unk_0D = 0x80;
        s->unk_0C = 0x80;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7CC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A8608);

/* THE BASE IS REMATERIALISED: one `b = ...` at the top is 15 words / 7
   differing because it stays live in $a1 across the loop. Assigning it
   again before the tail store is 0. The constant written inline at every
   site is 23 words. 16 words, 0 differing. */
void func_800A86DC(s32 i)
{
    u8 *b;

    b = (u8 *)0x80010000;
    for (; i < 0x13; i++)
    {
        *(s32 *)(b + i * 4 + 0x29C) = *(s32 *)(b + i * 4 + 0x2A0);
    }
    b = (u8 *)0x80010000;
    *(s32 *)(b + i * 4 + 0x29C) = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A871C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A8E74);

/* Set bit (i & 7) of a bitmap at 0x80015720 for every live entry of every
   D_80073414 descriptor whose +0x01 has the caller's bit.

   0x80015720 IS A CONSTANT ADDRESS, NOT A SYMBOL: a declared array (or a
   sized, scalar or 2-D alias of it) materialises the whole `lui`/`addiu` as a
   loop invariant and the `%lo` leaves the `lbu`/`sb` -- 50 words against 49.
   THE CONSTANT BASE MUST BE A POINTER LOCAL RE-ASSIGNED AT THE TOP OF THE
   OUTER LOOP: at function scope loop.c turns `b + j * 8` into a giv, 21
   differing; inside the inner loop 17; inside the `if` 30.

   Both `addu`s are INDEX-FIRST, so both operands must reach them as integers:
   `i * 0x14 + e->unk_0C` is pointer arithmetic and comes out base-first, 1
   differing, and so does `(j * 8 + i / 8) + b`. Note the scaled operand is
   the base's PARTNER, not the base, so the "unscaled index only" caveat on
   that lever does not hold here. 49 words, 0 differing. */
void func_800A9024(s32 bit)
{
    s32 i;
    s32 j;
    Unk80073414 *e;
    u8 *q;
    u8 *b;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        e = &D_80073414[j];
        if (((*((u8 *)e + 1) >> bit) & 1) == 0)
        {
            continue;
        }
        for (i = 0; i < e->unk_02; i++)
        {
            q = (u8 *)(i * 0x14 + (s32)e->unk_0C);
            if ((*(u16 *)q & 0x200) == 0)
            {
                *(u16 *)q |= 0x400;
                b[(j * 8 + i / 8) + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* Clear bit 0x400 of the halfword at the head of entry p[0] of the
   0x14-stride table D_80073414[p[1]].unk_0C points at. 30 words,
   0 differing. */
void func_800A90E8(u8 *p)
{
    u8 *q;

    func_8004A8D8(p[1], p[0]);
    q = D_80073414[p[1]].unk_0C;
    *(u16 *)(q + p[0] * 0x14) &= 0xFBFF;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9160);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9230);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9390);

/* `s32 c`, NOT `u8 c`: an lbu already zero-extends, but a QImode local makes
   each compare carry its own `andi 0xFF` and the function is 41 words.
   40 words, 0 differing. */
s16 func_800A9400(s16 x)
{
    s32 c;

    c = D_800DD8E7[x];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (c == 0x32)
        {
            return 7;
        }
        if (c == 0x39)
        {
            return 8;
        }
        if (c == 0x40)
        {
            return 9;
        }
    }
    else
    {
        if (c == 0x32)
        {
            return 1;
        }
        if (c == 0x39)
        {
            return 2;
        }
        if (c == 0x40)
        {
            return 3;
        }
    }
    return x;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A94A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A982C);

void func_800A9A04(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9A0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9AFC);

void func_800A9C10(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9C18);

/* A COMMON RETURN VARIABLE, not two returns. With `return 1;` as its own
   statement reorg lifts the `li 1` into the `beqz` delay slot and the
   function is 12 words; one local keeps the `nop` and the `j`.
   14 words, 0 differing. */
s32 func_800A9E38(u8 *p)
{
    u8 *r;
    s32 v;

    r = *(u8 **)(p - 0x14);
    if (*(s32 *)r != 0)
    {
        v = (u32)(p[0x13] - 0x2E) < 2;
    }
    else
    {
        v = 1;
    }

    return v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9E70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA258);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA36C);

/* Installer: state 3, handler slot cleared, a 4-step life, and the
   D_80083460 counter at +0xA bumped. 13 words, 0 differing. */
void func_800AA508(u8 *p)
{
    u8 *g;

    p[0x9A] = 3;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 4;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* Re-aim one record's facing and re-run its walk.

   D_80082E80'S SUBSCRIPTS ARE WRITTEN AT THE SITE. Hoisted into the same
   `u8 *r` the else arm uses, the symbol's `high` gets its own register
   (`lui $v0` / `addiu $s0, $v0`) where the target ties both to $s0: 2
   differing. `&D_80082E80[0]` is the same 2; a block-scoped `r` per arm
   measures 0, like the subscript. 42 words, 0 differing. */
void func_800AA53C(u8 *a)
{
    u8 *r;

    if (a == NULL || a[0x13] == 0)
    {
        D_80082E80[0x26] = func_8009FB34(D_80082E80[0x24], D_80082E80[0x25]);
        func_800A1B44(3, 6);
    }
    else
    {
        r = *(u8 **)(a - 0x14);
        r[0x26] = func_8009FB34(r[0x24], r[0x25]);
        func_800A19E4((s32)r, a, 3, 6, a + 0x9C);
    }
}

/* State 4. The `sll 16` / `bltz` with no `sra` is `(s16)result < 0`.
   52 words, 0 differing. */
void func_800AA5E4(u8 *p, void *b, u8 *r, u8 *d)
{
    u8 *g;

    p[0x9A] = 4;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(s16 *)(p + 0x96) = 0x10;
    *(u16 *)(p + 0x98) |= 8;
    *(u32 *)(d + 0x1C) |= 0x10000;
    func_8009A3D0(r[0x24], r[0x25], (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    do
    {
    } while ((s16)func_800A4E2C(r + 0x24, r + 0x25) < 0);
    r[0x26] = func_8009FB34(r[0x24], r[0x25]);
    func_800AA53C(d);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

/* THE NULL TEST NESTS, IT DOES NOT RETURN. `if (t == NULL) return 1;` as its
   own early exit materialises a third `addiu $v0, 1` and the function is 41
   words; nesting the rest inside `if (t != NULL)` lets the NULL path fall
   into the tail's `return 1` while the 0x20 path keeps its own, which is the
   two the target has. The prototype gains a return type and a pointer fourth
   parameter; the four landed callers pass a literal 0 and discard the
   result, so neither changes a byte. 40 words, 0 differing. */
s32 func_800AA6B4(u8 *p, UnkVec *b, u8 *c, u8 *t)
{
    s32 v;

    if (*(s16 *)(p + 0x64) > 0)
    {
        func_800AA754(p, b, c, p);
        return 0;
    }
    D_80083470 = p - 0x20;
    p[0x9A] = 0x7;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        if (*(s32 *)(p + 0x1C) & 0x20)
        {
            return 1;
        }
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    return 1;
}

/* 18 words, 0 differing. */
void func_800AA754(u8 *a, UnkVec *b, u8 *c, u8 *p)
{
    func_800AD4D0(p);
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA79C);

/* THE TWO BYTE ARGUMENTS ARE READ BEFORE THE `if`, NOT AT THE CALL. Written
   as `func_8009A3D0(c[0x24], c[0x25], t)` the loads land after the branch,
   $a2 has to be copied to a callee-live register to survive it, and the
   function is 41 words. Hoisted into two locals they are issued from $a2 in
   the prologue and nothing is saved: 39 words, 0 differing. */
void func_800AA888(u8 *p, void *b, u8 *c, u8 *d)
{
    s32 t;
    s32 x;
    s32 y;

    p[0x9A] = 0xB;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    x = c[0x24];
    y = c[0x25];
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(x, y, t);
    *(u16 *)(p + 0x98) |= 0x8;
    *(s32 *)(d + 0x1C) &= ~0x40000;
    *(s16 *)(p + 0x96) = 0;
    *(u16 *)(d + 0x6A) = *(u16 *)(*(u8 **)(d + 0x60) + 0x2A);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA924);

/* func_80095DD0 on the SECOND argument and without the parked pointer.
   11 words, 0 differing. */
void func_800AAA28(void *a, u8 *p, void *c, void *d)
{
    u8 *g;

    p[0x9A] = 0x15;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0x14) += 1;
}

/* The shared "actor dies" tail the four +0x8C state machines call with a NULL
   fourth argument. The fourth parameter keeps the `s32` the unit already
   declares even though the body indexes it: all eight landed call sites pass
   a literal 0, so the width is not witnessed. 47 words, 0 differing. */
void func_800AAA54(UnkDungeonActor *a, UnkVec *v, u8 *c, s32 d)
{
    a->unk_9A = 8;
    a->unk_9B = 0;
    a->unk_8C = NULL;
    *(s32 *)(c + 0x2C) = d;
    if (d != 0)
    {
        func_80047784(c, ((u8 *)d)[((D_80083228 + *(s16 *)((u8 *)a + 0x2A) + 0x100) >> 9) & 7], 0);
    }
    *(u32 *)((u8 *)a + 0x1C) |= 0x4000000;
    func_80042B68((s8 *)a, 4);
    if ((s16)func_80042900((s8 *)a, 0xA) != 0)
    {
        *((u8 *)a + 0x13) = *((u8 *)a + 0xA8);
        func_80042518((s8 *)a, 0xA);
    }
    a->unk_96 = 0x40;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AAB10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AACA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AAF00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB1C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB378);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB464);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB538);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB778);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ABDBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC010);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC480);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC5C8);

/* MUST STAY K&R: the landed func_800DA808 below calls this with NO
   arguments, and an ANSI four-parameter definition makes that call "too few
   arguments to function". 16 words, 0 differing. */
s32 func_800AC7EC(a, b, c, p)
u8 *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 f;

    f = *(s32 *)(p + 0x1C);
    if (f & 0x8000000)
    {
        if (*(s32 *)(p + 0x14) & 0x40000)
        {
            *(s32 *)(p + 0x1C) = f | 0x40000;
        }
        return 1;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC82C);

/* 19 words, 0 differing. */
void func_800ACB98(u8 *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    p[0x9A] = 0x14;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(c + 0x14) |= 0x800;
    *(s16 *)(p + 0x96) = 0;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) |= 0x10000000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACBE4);

/* State 0x13. The four byte stores come out 0xF, 0xE, 0xD, 0xC because they
   are ONE CHAINED ASSIGNMENT, which emits right to left. 48 words,
   0 differing. */
void func_800ACD74(u8 *p, void *b, u8 *r, u8 *d)
{
    UnkNode *n;
    u8 *g;

    p[0x9A] = 0x13;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    *(u16 *)(r + 0x14) &= ~0x800;
    *(s16 *)(p + 0x96) = 0;
    n = (UnkNode *)(p - 0x20);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    func_80044A50(n);
    func_8004491C(n, (u32)func_800C6AEC);
    r[0xC] = r[0xD] = r[0xE] = r[0xF] = 0;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x12) -= 0x80;
    *(s32 *)(d + 0x1C) |= 0x10000000;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACE34);

/* 0x80013714 IS A CONSTANT ADDRESS HELD IN A POINTER LOCAL, and here the
   local is what produces the shared `lui $s1` both tests read through. As
   structs.h's SYMBOL the prologue schedules differently -- `sw $ra` lands in
   the first branch's delay slot and the parameter copy fills the `lhu`'s load
   delay, so the target's `nop` is gone and the function is 51 words against
   52. Spelled `*(u16 *)0x80013714` at each site it is 53.
   52 words, 0 differing. */
void func_800ACF88(u8 *p)
{
    u8 *f;
    u8 *g;
    void *m;
    void *h;

    f = (u8 *)0x80010000;
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        D_800DCF4F = 1;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    m = func_800990FC();
    h = func_80099734(p, m);
    h = func_80099194(D_800E0C78, h);
    if ((*(u16 *)(f + 0x3714) & 1) == 0)
    {
        h = (void *)func_8009929C(0x11, h);
        h = (void *)func_8009929C(0x4C, h);
        h = (void *)func_80099254(D_800E0458, h);
    }
    func_80099290(h);
    func_800A5720(m);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD058);

/* 38 words, 0 differing. */
void func_800AD4D0(u8 *p)
{
    s32 v;

    if (*(s16 *)(p + 0x64) < 0)
    {
        if ((s16)func_80042900((s8 *)p, 0x1D) != 0)
        {
            *(s16 *)(p + 0x64) = 0;
        }
    }
    v = p[0x28] + *(s16 *)(p + 0x64);
    if (v < 0)
    {
        v = 0;
    }
    else if (v >= 0x100)
    {
        v = 0xFF;
    }
    p[0x28] = v;
    *(s16 *)(p + 0x64) = 0;
    if (p[0x29] < p[0x28])
    {
        p[0x28] = p[0x29];
    }
}

/* Clamp the +0x64 cursor so cursor + step never passes the limit at +0x29.

   THE THREE LOADS ARE LOCALS. As one condition the two bytes are re-read
   inside the arm -- the `slt` kills the register holding +0x29 before cse
   can reuse it -- and the function comes out 16 words against 11.
   11 words, 0 differing. */
s16 func_800AD568(u8 *p)
{
    s32 v;
    s32 a;
    s32 b;

    v = *(s16 *)(p + 0x64);
    a = p[0x28];
    b = p[0x29];
    if (v + a > b)
    {
        *(s16 *)(p + 0x64) = b - a;
    }
    return *(s16 *)(p + 0x64);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD594);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD6FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD9B4);

/* func_80095464's probe again, carried past the two `return 1` gates into the
   +0x03 clear of func_800954E0 and a fourth call. The return is s16: the two
   constants need no extension, and the `sll`/`sra` pair on the tail is where
   func_800B627C's s32 result is narrowed. 42 words, 0 differing. */
s16 func_800ADAE4(u8 *a, u8 *b)
{
    s16 i;
    u8 f;
    s32 r;

    i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
    if (i < 0)
    {
        return 1;
    }
    f = D_800E3648_s[i].unk_03;
    if ((f & 0x80) == 0)
    {
        return 1;
    }
    if ((f & 0x40) == 0)
    {
        D_800E3648_s[i].unk_03 = f & 0x7F;
    }
    r = func_800B627C(*(u8 *)&D_800E3648_s[i], b, i, 0);
    if (r == 0)
    {
        return 0;
    }
    return r;
}

/* 16 words, 0 differing. */
void func_800ADB8C(u8 *p)
{
    if (**(u8 **)p != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The node walk again (see func_800BC054), with the record's +0x8 parked in
   the scratchpad each turn and the visitor skipped when the slot's +0x14
   carries 0x80.

   TWO SPELLINGS ARE LOAD-BEARING. The scratchpad address needs a POINTER
   LOCAL: written as `*(s32 *)0x1F80013C` it is the assembler's absolute
   macro, a `lui $at`/`sw` pair that cannot be hoisted out of the loop, and
   the function is 28 words. And the slot must be a `u8 *`, not an
   `UnkSlot *`: through the struct pointer the +0x14 read is MEM_IN_STRUCT_P,
   sched1 hoists it over the scratchpad store and the three load-delay `nop`s
   the target has disappear -- 29 words at 26 differing.
   32 words, 0 differing. */
s32 func_800ADBCC(u8 *p, UnkVec *v, u8 *s)
{
    UnkNode *n;
    u8 *sp;

    sp = (u8 *)0x1F800000;
    for (;;)
    {
        *(s32 *)(sp + 0x13C) = *(s16 *)(p + 0x8);
        if (!(*(u16 *)(s + 0x14) & 0x80))
        {
            func_800453E0(p, v, s, *(s16 *)(s + 0x6));
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (u8 *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADC4C);

/* Set bit 0x8000 of +0x14 once, then park bit 0x100 of +0x98 on whether a
   fresh random number divides by n. The draw goes into an `s32` and
   func_800A6D30 is declared u16, so the `andi $v0, 0xFFFF` is the widening at
   the assignment. 32 words, 0 differing. */
void func_800ADD20(u8 *p, s32 n)
{
    s32 r;

    if ((*(s32 *)(p + 0x14) & 0x8000) == 0)
    {
        *(s32 *)(p + 0x14) |= 0x8000;
        r = func_800A6D30();
        if (r % n != 0)
        {
            *(u16 *)(p + 0x98) &= 0xFEFF;
        }
        else
        {
            *(u16 *)(p + 0x98) |= 0x100;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADDA0);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089074);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089080);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADE74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFAD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFBFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFFB4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0544);

/* Project the record's (x, y, z) through RotTransPers out of the third
   argument's own scratch at +0x28, and bias the returned depth by the second
   argument's signed +0x4 byte. 27 words, 0 differing. */
void func_800B0654(u8 *a, u8 *b, u8 *c)
{
    *(s16 *)(c + 0x28) = *(u16 *)(a + 0x2);
    *(s16 *)(c + 0x2A) = *(u16 *)(a + 0x6);
    *(s16 *)(c + 0x2C) = *(u16 *)(a + 0xA);
    *(s32 *)(c + 0xC0) = func_80065420((SVECTOR *)(c + 0x28), (s32 *)(c + 0xB8), (s32 *)(c + 0x90),
                                       (s32 *)(c + 0x94)) +
                         *(s8 *)(b + 0x4);
}

/* 12 words, 0 differing. */
void func_800B06C0(u8 *v, u8 *q, u8 *p)
{
    *(s16 *)(p + 0xB8) = *(u16 *)(v + 0x2);
    *(s16 *)(p + 0xBA) = *(u16 *)(v + 0x6);
    *(s32 *)(p + 0xC0) = *(s16 *)(v + 0xA) + *(s8 *)(q + 0x4);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B06F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0D34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0F50);

/* 11 words, 0 differing. */
u8 *func_800B12F4(void)
{
    UnkNode *n;
    u8 *r;

    n = func_8003FC64(0);
    r = NULL;
    if (n != NULL)
    {
        r = (u8 *)n + 0x20;
    }
    return r;
}

/* A POINTER WALK IS WHAT THIS TARGET WANTS, the reverse of func_800A7030's
   note: loop.c turns p[2]/p[3] into one giv biased by +3 and read at -1 and
   0, and the terminator keeps the base. 15 words, 0 differing. */
void func_800B1320(u8 *p, u8 a, u8 b)
{
    while (1)
    {
        p[0x2] += a;
        p[0x3] += b;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* `g = f; f &= 0x4;` -- the hoisted test must land in the PARAMETER's own
   register with the original copied out, which `m = f & 4;` does not do
   (19 words against 20). 20 words, 0 differing. */
void func_800B135C(u8 *p, s32 f)
{
    s32 g;

    g = f;
    f &= 0x4;
    while (1)
    {
        p[0] |= g;
        if (f)
        {
            p[2] = -p[2];
        }
        if ((s8)p[0] < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* Walk a 0xC-stride table terminated by a negative first byte, writing `v`
   into each entry's +0x06.

   THE BACKWARD `goto` IS LOAD-BEARING. A `for (;;)` with a `break` is 9
   words and 8 differing -- loop.c rotates it and emits a preheader jump --
   and tail recursion is 14, because this compiler does not eliminate it
   here. 8 words, 0 differing. */
void func_800B13AC(s8 *p, s16 v)
{
top:
    *(s16 *)(p + 0x6) = v;
    if (*p < 0)
    {
        return;
    }
    p += 0xC;
    goto top;
}

/* OR a mask into every record's +0x4 halfword until the lead byte goes
   negative. The +0x4 cursor and the record base are two induction
   variables, which is the two `addiu 0xC`s. 13 words, 0 differing. */
void func_800B13CC(u8 *p, u16 mask)
{
    while (1)
    {
        *(u16 *)(p + 4) |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* func_800B13CC on the +0x1 byte. 13 words, 0 differing. */
void func_800B1400(u8 *p, u8 mask)
{
    while (1)
    {
        p[1] |= mask;
        if (*(s8 *)p < 0)
        {
            break;
        }
        p += 0xC;
    }
}

/* 20 words, 0 differing. */
u8 *func_800B1434(u8 *d, u8 *s)
{
    while (1)
    {
        *(s32 *)(d + 0x0) = *(s32 *)(s + 0x0);
        *(s32 *)(d + 0x4) = *(s32 *)(s + 0x4);
        *(s32 *)(d + 0x8) = *(s32 *)(s + 0x8);
        d += 0xC;
        if ((s8)s[0] < 0)
        {
            return d;
        }
        s += 0xC;
    }
}

/* 12 words, 0 differing. */
u8 *func_800B1484(u8 *p)
{
    while (*(s8 *)p >= 0)
    {
        p += 0xC;
    }
    return p;
}

/* 6 words, 0 differing. */
s32 func_800B14B4(void)
{
    return *(s32 *)(D_800E3D7C + 0x104) == 0;
}

/* 4 words, 0 differing. */
s32 func_800B14CC(void)
{
    return D_80081485[0] == 0;
}

/* 8 words, 0 differing. */
s32 func_800B14DC(void)
{
    return D_800E3D7C[0x9A] != 0x29;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B14FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1768);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1B10);

/* A node +0x10 step: every fourth frame advance the +0x2 cursor by 8, wrap it
   at 0x60, and play the row it now points at. The field is read BACK for the
   call because the wrap store sits between. 32 words, 0 differing. */
void func_800B1E80(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s16 u;

    t = *(u16 *)p - 1;
    *(s16 *)p = t;
    if (t <= 0)
    {
        *(s16 *)p = 4;
        u = *(u16 *)(p + 0x2) + 8;
        *(s16 *)(p + 0x2) = u;
        if (u >= 0x60)
        {
            *(s16 *)(p + 0x2) = 0;
        }
        func_8003F80C((u32)&D_800DF068[*(s16 *)(p + 0x2)], 0x7340, 1, 2);
    }
}

/* 13 words, 0 differing. */
void func_800B1F00(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B1E80;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1F34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2074);

/* A node +0x10 step: lift the mover 0x8000 a frame, re-register the
   renderable, and mark the node dead when the +0x6 countdown goes negative.
   30 words, 0 differing. */
void func_800B21B8(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    b->unk_08 += 0x8000;
    func_800478B8(c);
    t = *(u16 *)(p + 0x6) - 1;
    *(s16 *)(p + 0x6) = t;
    if (t < 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* 48 words, 0 differing. The whole-UnkVec assignment is the 24-byte block
   move: four `lw`/`sw` pairs then two. */
void func_800B2230(UnkVec *src)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B21B8;
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *m = *src;
        r = (UnkSlot *)n->unk_0C;
        *(u32 *)r->unk_0C = 0x808080;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        func_8003DB94(r, D_800DEC70, 0);
        *(s16 *)((u8 *)n + 0x26) = 0x20;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B22F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B23F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B253C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B274C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2A60);

/* Ten one-gate installers: run the predicate, and on a pass park the next
   handler in the record's +0x8C slot. csrc/ovl_town.c's func_800A4F4C is the
   same source with a different record offset and predicate. 14 words each,
   0 differing. */
void func_800B2C68(UnkDungeonActor *p)
{
    if (func_800AB378() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2CA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2FAC);

/* Six forwarders, one call each and nothing else. 8 words each,
   0 differing. */
void func_800B306C(void)
{
    func_800AB538();
}

void func_800B308C(void)
{
    func_800AB778();
}

void func_800B30AC(UnkDungeonActor *p)
{
    if (func_800ABDBC() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B30E4);

void func_800B318C(void)
{
    func_800AC82C();
}

void func_800B31AC(UnkDungeonActor *p)
{
    if (func_800AC010() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

void func_800B31E4(UnkDungeonActor *p)
{
    if (func_800AC480() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

void func_800B321C(UnkDungeonActor *p)
{
    if (func_800AC5C8() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

void func_800B3254(UnkDungeonActor *p)
{
    if (func_800ACE34() != 0)
    {
        p->unk_8C = func_800B2A60;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B328C);

/* Allocate a kind-0x210 node under the caller's record, park func_800B328C at
   its +0x10 and write the two pointers and a halfword into the payload.
   `*(void **)q = a` comes back as `sw $s1, 0x20($v1)` off the NODE while
   `*(void **)(q + 4)` keeps the `$v0` base -- cse rewrites `*q` into
   `*(p + C)` and leaves `*(q + 4)` alone. 28 words, 0 differing. */
void func_800B341C(u8 *a, void *b, s16 c)
{
    UnkNode *n;
    u8 *q;

    if (b != NULL)
    {
        n = func_8003FD64(0x210, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800B328C;
            q = (u8 *)n + 0x20;
            *(void **)q = a;
            *(void **)(q + 0x4) = b;
            *(s16 *)(q + 0x8E) = c;
        }
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089118);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B348C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B3D10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B4194);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892C4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B45E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B490C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B4C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B500C);

s16 func_800B5128(void)
{
    s32 i;
    u8 *p;

    for (i = 0, p = D_800E3648; i < 0x20; i++, p += 4)
    {
        if (p[1] == 0)
        {
            return i;
        }
    }
    return -1;
}

void func_800B5168(void)
{
    UnkNode *n;
    UnkDungeonSprite *s;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        s = (UnkDungeonSprite *)n->unk_0C;
        n->unk_10 = 0;
        func_8004491C(n, (u32)func_800B51C8);
        s->unk_0E = 0x80;
        s->unk_0D = 0x80;
        s->unk_0C = 0x80;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B51C8);

/* Step one tile along the eight-way direction in `dir` and claim the spot
   there. TWO OF THE THREE `return -1`s SHARE THE EXIT: written as three early
   returns the first takes its own `li $v0, -1` in the `bltz`'s delay slot and
   the function is 45 words of 44, 22 differing; wrapping the body in
   `if (k >= 0) { ... }` with one trailing `return -1` leaves exactly the two
   copies the target has. 44 words, 0 differing. */
s32 func_800B5ED0(s16 x, s16 y, u16 dir, s16 w)
{
    s32 i;
    s16 k;
    u8 f;

    i = (dir >> 9) & 7;
    k = func_800B500C((s16)(x + ((u16 *)D_8006CCD8)[i]), (s16)(y + ((u16 *)D_8006CCE8)[i]), w);
    if (k >= 0)
    {
        f = D_800E3648_s[k].unk_03;
        if (f & 0x40)
        {
            return -1;
        }
        if (f & 0x80)
        {
            D_800E3648_s[k].unk_03 = f & 0x7F;
            return k;
        }
    }
    return -1;
}

/* Free slot `i` of the second spot table: clear its flag word, play the
   effect at its tile and drop the renderable it holds at +0x0C.

   THE SUBSCRIPT IS WRITTEN AT EVERY SITE, not hoisted into a pointer local.
   `q = &D_800E39C8[i]` is 34 words / 9 differing -- `&arr[i]` gives a
   BASE-FIRST `addu` and materialises the symbol after the index, where the
   target has the `lui`/`addiu` first and an index-first
   `addu $s0, $v0, $v1`. 34 words, 0 differing. */
void func_800B5F80(s32 i)
{
    UnkNode *n;

    *(s32 *)&D_800E3648_s[i] = 0;
    func_8009A3D0(D_800E39C8[i].unk_06, D_800E39C8[i].unk_07, 0x20);
    n = D_800E39C8[i].unk_0C;
    if (n != NULL)
    {
        n->unk_1E |= 0x8000;
        D_800814A0 |= 0x8000;
        D_800E39C8[i].unk_0C = NULL;
    }
}

/* func_800A7030 over the OTHER pair of tables -- 0x20 entries at stride 0x18,
   tile at +0x06/+0x07 -- handing the index to func_800B5F80 rather than
   clearing the slot itself. 44 words, 0 differing. */
void func_800B6008(s16 x, s16 y)
{
    s32 i;

    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            func_800B5F80(i);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B60B8);

/* 9 words, 0 differing. */
s32 func_800B6204(s8 *p)
{
    func_800A48F0(p, 7, 0x10);
    return 1;
}

/* 21 words, 0 differing. */
s32 func_800B6228(u8 *p)
{
    if ((s16)func_80042900((s8 *)p, 2))
    {
        func_80042B68((s8 *)p, 2);
        func_80099844(p, D_800E0DF5);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B627C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B63A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B653C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B66C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6814);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B69DC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6AC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7774);

/* Spawner: a kind-0x12 node carrying func_800B7774. The first two arguments
   are read back out of the new mover (`lh`) while the third is the halfword
   just stored, sign-extended -- which is what writing all three stores and
   all three reads as `*(s16 *)` gives, with cse forwarding only the last.
   45 words, 0 differing. */
UnkNode *func_800B7B60(u8 *p, u32 *q)
{
    UnkNode *n;
    UnkVec *m;
    UnkVec *s;
    u8 *g;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B7774;
        *(u32 *)((u8 *)n + 0x20) = *q;
        s = *(UnkVec **)(p - 0x18);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)s + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)s + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)s + 0xA);
        func_800B8024(*(s16 *)((u8 *)m + 0x2), *(s16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7C14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7D74);

/* 45 words, 0 differing. The first call needs no argument setup at all,
   because $a0/$a1/$a2 are still the incoming parameters. */
void func_800B8024(u8 *a, u8 *b, u8 *c)
{
    s32 i;

    func_800B7D74(a, b, c, 1);
    func_800B7D74(a, b, c, 2);
    func_800B7D74(a, b, c, 3);
    func_800B7D74(a, b, c, 4);
    for (i = 10; i > 0; i--)
    {
        func_800B7D74(a, b, c, 0);
    }
    func_800A56E0(0x513);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B80D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8228);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B835C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B84E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8830);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8C20);

/* A node +0x10 step: flash the renderable's three colour bytes while the +0x2
   countdown lasts, then mark the node dead.

   TWO VARIABLES FOR ONE COUNTDOWN -- the s16 the field holds and an s32 copy
   of it. With a single s16 the comparison folds into `sll 16`/`bgtz`, the
   `<< 3` then reads the RAW halfword and gcc spends a pointless
   `move $a1, $v1` where the target has the `sra`: 29 words, 2 differing.
   Widening once into `u` makes the sign-extension real and both uses take it;
   `(s32)t * 8 + 0x20` with one variable is still 1 differing, so the cast
   alone is not enough -- it has to be a separate binding. The colour bytes
   are written 0xC, 0xE, 0xD: reorg puts the last of the three in the `j`'s
   delay slot, and C, D, E is a further 2 differing. 29 words, 0 differing. */
void func_800B8CF0(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;
    s32 u;

    t = *(u16 *)(p + 0x2) - 1;
    *(s16 *)(p + 0x2) = t;
    u = t;
    if (u <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    else if (t & 1)
    {
        c[0xC] = (u << 3) | 0x20;
        c[0xE] = (u << 3) | 0x20;
        c[0xD] = (u << 3) | 0x20;
    }
    else
    {
        *(u32 *)(c + 0xC) = 0x101010;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8D64);

/* 36 words, 0 differing. */
void func_800B8E18(u8 *p, void *b, u8 *c)
{
    *(s16 *)(c + 0x1C) = *(s16 *)(c + 0x1E) -=
        *(u16 *)(c + 0x1E) * *(s16 *)(p + 0x2) / 24;
    c[0xC] = c[0xD] = c[0xE] -= *(s16 *)(p + 0x2) * 8;
    if (c[0xC] == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8EA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9144);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B96C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9964);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9A78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA074);

/* THE `!= 0` MUST BE SPLIT OFF THE AND. `return (x & 0x20000) != 0;` folds
   to srl 17 / andi 1 (3 differing); cc1psx expands statement by statement,
   so assigning the AND to a local first leaves a plain register compare and
   gives lui / and / sltu. 18 words, 0 differing. */
s32 func_800BA33C(u8 *p)
{
    s32 v;

    if (p == NULL || (p[0x13] == 0x18 && p[0xB5] == 1))
    {
        return 0;
    }
    v = *(s32 *)(p + 0x1C) & 0x20000;
    return v != 0;
}

/* The node walk of func_800BC054, with a 62x62 quad parked in the
   scratchpad's four RotAverage4 input vectors first.

   THE TWELVE STORES ARE FIVE CHAINED ASSIGNMENTS. The stores keep source
   order here, so the target's order (8C, 84, 7C, 74, then 80, 70, then
   88, 78, then 7A, 72, then 8A, 82) is the source's, and each DESCENDING run
   is one chained assignment, because a chained assignment emits its stores
   right to left. All 24 regular orders -- record-major and field-major, both
   record directions, every field permutation -- were measured: the floor is
   10 differing and plain ascending is 14. 36 words, 0 differing. */
s32 func_800BA384(u8 *p, UnkVec *v, UnkSlot *s)
{
    Unk463ECSP *sp = (Unk463ECSP *)0x1F800000;
    UnkNode *n;

    sp->unk_70[0].vz = sp->unk_70[1].vz = sp->unk_70[2].vz = sp->unk_70[3].vz = 0;
    sp->unk_70[0].vx = sp->unk_70[2].vx = -0x1F;
    sp->unk_70[1].vx = sp->unk_70[3].vx = 0x1F;
    sp->unk_70[0].vy = sp->unk_70[1].vy = -0x1F;
    sp->unk_70[2].vy = sp->unk_70[3].vy = 0x1F;
    for (;;)
    {
        func_800BA414(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA414);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA6B8);

/* Spawner: a kind-0x202 node carrying func_800BA6B8 at +0x10 and
   func_800BA384 as its draw callback.

   `x * 0x40 + 0x20`, NOT `(x << 6) + 0x20`. The shift form lets gcc drop the
   u16 parameter's zero-extension, which is the `andi $v0, $s1, 0xFFFF` the
   target has at each use: 41 words of 43, 30 differing, and the four
   parameter copies then come out in declaration order where the target copies
   $a2/$a3 first. 43 words, 0 differing. */
UnkNode *func_800BA764(u16 x, u16 y, s16 ang, s32 w)
{
    UnkNode *n;
    UnkVec *m;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BA6B8;
        func_8004491C(n, (u32)func_800BA384);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 0x40 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s32 *)((u8 *)n + 0x50) = w;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA810);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BABA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BAF04);

/* Spawn the func_800BAF04 node for the payload `p`, parented on p's own node.
   NO LOCAL FOR THE PARENT NODE: `parent = (UnkNode *)(p - 0x20);` used twice
   costs a whole callee-saved register -- 28 words against 25, with $s1 saved
   and restored -- because the pointer stays live across the call.
   25 words, 0 differing. */
void func_800BB044(u8 *p)
{
    UnkNode *n;

    n = func_8003FD64(0x202, p - 0x20);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BAF04;
        *(u32 *)((u8 *)n + 0x20) = (u32)p;
        n->unk_08 = *(UnkVec **)(p - 0x18);
        *(s16 *)((u8 *)n + 0x2C) = *(u16 *)(p + 0x2A);
        func_800A56E0(0x703);
    }
}

/* The two arms duplicate the call rather than share a third-argument
   variable: both load +0x10 and +0x12 for themselves and only the `jal` is
   cross-jumped. 41 words, 0 differing. */
void func_800BB0A8(u8 *p, void *b, u8 *c)
{
    if (D_80083462 & 0x10)
    {
        if (--*(s16 *)(p + 0x1E) <= 0)
        {
            if (*(s16 *)(p + 0x1C) == 1)
            {
                func_8009A3D0(*(s16 *)(p + 0x10), *(s16 *)(p + 0x12), 2);
            }
            else
            {
                func_8009A3D0(*(s16 *)(p + 0x10), *(s16 *)(p + 0x12), 0x10);
            }
            func_800B5F80(*(s16 *)(p + 0x20));
            return;
        }
    }
    *(u16 *)(c + 0x1A) += 0x40;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB14C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB2E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB55C);

/* 8 words, 0 differing. */
s32 func_800BBA20(void)
{
    func_80045C34();
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BBA40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BBBE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BBDEC);

/* The node walk, seven instances, in three variants. This is func_80046398
   of csrc/unk_19434.c -- the engine's own walk -- recompiled once per
   visitor: `p - 8` is the node's own back-pointer read out of its payload,
   so the loop steps along a list whose links sit 8 bytes below each payload
   base.

   Variant A passes the slot's unk_06 as the visitor's fourth argument,
   exactly as the engine's does: func_800BC054, func_800C55A0, func_800C9034
   and func_800D0774. 21 words each, 0 differing. */
s32 func_800BC054(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800BBDEC(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC0A8);

/* Wrap the record's +0x02 counter into 0..0x1F by its +0x0C span and blit the
   two 0x10 x 0x20 cells. The rectangle is the 4-halfword UnkDungeonRect
   func_800673A0 takes; its first halfword reuses the 0x270 already in $a1
   for the call. 29 words, 0 differing. */
void func_800BC1B4(u8 *p)
{
    UnkDungeonRect r;

    *(u16 *)(p + 0x2) = (*(u16 *)(p + 0x2) - *(u16 *)(p + 0xC)) & 0x1F;
    r.v[0] = 0x270;
    r.v[1] = *(u16 *)(p + 0x2) + 0x140;
    r.v[2] = 0x10;
    r.v[3] = 0x20;
    func_800673A0(r.v, 0x270, 0x100);
    func_800673A0(r.v, 0x270, 0x120);
}

/* 17 words, 0 differing. */
void func_800BC228(s16 arg0)
{
    UnkNode *n;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BC1B4;
        *(s16 *)((u8 *)n + 0x2C) = arg0;
    }
}

/* The complement of func_800A7700 above: here the `s16` second parameter IS
   required. It is both the ternary's test and the +0x0E store, and that pair
   is where the `addu $s3, $s1, $zero` in the first branch's delay slot comes
   from -- the HImode pseudo is a second quantity, not a redundant copy.
   43 words, 0 differing. */
UnkNode *func_800BC26C(void *list, s16 flag, s32 val)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x110, list);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BC388;
        func_8004491C(n, (u32)func_800BC3E4);
        p = (u8 *)n + 0x20;
        *(void **)p = list;
        *(s32 *)(p + 4) = val;
        *(s16 *)(p + 0xC) = flag ? 0x7F80 : 0x7FC0;
        *(s16 *)(p + 0xE) = flag;
    }
    return n;
}

/* Walk the engine's node list for the func_800BC388 node whose payload word
   is `key` and mark it for redraw. 28 words, 0 differing. */
void func_800BC318(u8 *key)
{
    UnkNode *n;

    for (n = D_80081498; n != NULL; n = n->unk_00)
    {
        if (n->unk_10 == (u32)func_800BC388 && *(u8 **)((u8 *)n + 0x20) == key)
        {
            n->unk_1E |= 0x8000;
            D_800814A0 |= 0x8000;
            return;
        }
    }
}

/* Raise 0x8000 on the node header's +0x1E and on D_800814A0 when either the
   referenced record's +0x1E already carries it or the second pointer's first
   word is zero. The two tests are one `||`: the first `bnez` jumps TO the
   body and the second jumps PAST it. Note this target WANTS the folded
   `-0x2($a0)` that func_8009A078 above had to avoid. 23 words,
   0 differing. */
void func_800BC388(u8 *p)
{
    if ((*(u16 *)(*(u8 **)p + 0x1E) & 0x8000) || *(s32 *)*(u8 **)(p + 0x4) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC3E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC4D4);

/* Look a tile up in the 6-byte map rows at D_80083160 + 0x1DC and return the
   negated height at +0x2, or 0x400 where the row's first halfword is 0.

   THREE THINGS, ALL MEASURED. (1) The row address is INTEGER arithmetic: read
   the base as `*(u32 *)` and write the index first and the `addu` comes out
   `addu $v1, $v0, $v1`; through `*(u8 **)` it is base-first and wrong by a
   word. (2) The ZERO arm has to be written first -- with the negate arm first
   the 0x400 block is laid out out of line and the function is 28 words.
   (3) The narrowing belongs in the negate arm on an s32 result, not on an s16
   result variable: an s16 `r` shares one `sll`/`sra` between both arms, which
   is 7 differing, and the target narrows only the negation while 0x400 goes
   straight to the epilogue in the `beqz`'s delay slot.
   26 words, 0 differing. */
s16 func_800BCA68(u16 x, u16 y)
{
    u8 *g;
    u16 i;
    u16 *e;
    s32 r;

    g = (u8 *)&D_80083160;
    i = (y >> 6) << *(s16 *)(g + 0x1F0);
    e = (u16 *)((i + (x >> 6)) * 6 + *(u32 *)(g + 0x1DC));
    if (*e == 0)
    {
        r = 0x400;
    }
    else
    {
        r = (s16)(0 - e[1]);
    }
    return r;
}

/* The record's (x, z) and facing into func_800BCB04; the result is
   re-extended, so the return is s16. 13 words, 0 differing. */
s16 func_800BCAD0(u8 *p)
{
    return (s16)func_800BCB04(*(u16 *)(p + 0x2), *(u16 *)(p + 0x6), *(s16 *)(p + 0xA));
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BCB04);

/* The angular-span test on the cast scratchpad: four corner (x, z) deltas
   through ratan2, three gaps between consecutive angles, and a pass only if
   every gap is under 0x801. This is csrc/ovl_town.c's func_8008CE08 with the
   record 0xE8 longer -- see UnkDungeonCastSP above -- and the shape notes
   there apply unchanged: a corner's two halfwords are tested as ONE WORD,
   unk_170 and unk_16C are stored unmasked while unk_168 is masked, and the
   result is the last comparison rather than a branch. 80 words,
   0 differing. */
s32 func_800BCE7C(UnkDungeonCastSP *p)
{
    if (*(s32 *)&p->unk_154 == 0)
    {
        return 1;
    }
    if (*(s32 *)&p->unk_160 == 0)
    {
        return 1;
    }
    p->unk_164 = func_80065F90(p->unk_156, p->unk_154);
    p->unk_170 = func_80065F90(p->unk_162, p->unk_160);
    p->unk_170 = p->unk_170 - p->unk_164;
    p->unk_180 = (0x1000 - p->unk_170) & 0xFFF;
    if (p->unk_180 >= 0x801)
    {
        return 0;
    }
    if (*(s32 *)&p->unk_15C == 0)
    {
        return 1;
    }
    p->unk_16C = func_80065F90(p->unk_15E, p->unk_15C);
    p->unk_16C = p->unk_16C - p->unk_164;
    p->unk_17C = (p->unk_170 - p->unk_16C) & 0xFFF;
    if (p->unk_17C >= 0x801)
    {
        return 0;
    }
    if (*(s32 *)&p->unk_158 == 0)
    {
        return 1;
    }
    p->unk_168 = func_80065F90(p->unk_15A, p->unk_158);
    p->unk_168 = (p->unk_168 - p->unk_164) & 0xFFF;
    if (p->unk_168 >= 0x801)
    {
        return 0;
    }
    p->unk_178 = (p->unk_16C - p->unk_168) & 0xFFF;
    return p->unk_178 < 0x801;
}

void func_800BCFBC(void)
{
}

/* THE TWO WORDS FOUR BYTES APART ARE NOT A SPLIT ADDRESS. $s0 holds the
   CONSTANT 0x80010000, which mips_move_1word materialises with `lui` alone
   because its low half is zero, and 0x2090/0x208C ride in the two loads as
   ordinary displacements -- splat renders those as `%lo(SYM)`, which is what
   made this look like a `%hi` kept in a register. See
   docs/matching-levers.md, "A CONSTANT ADDRESS WITH A ZERO LOW HALF IS A
   ONE-WORD BASE".

   So the question this was parked on -- can this cc1psx emit
   `(lo_sum (high X) X+k)`? -- is answered: it does in `.rtl`, and it never
   survives. cse folds `(lo_sum (high X) X)` back to `X` and then prefers
   reg+displacement over a constant address on `mips_address_cost`, which is
   the `addiu $s0, $v0, %lo` word that every symbol spelling produced.

   `b` is ASSIGNED INSIDE the first arm. As an initialiser on the
   declaration it is 44 words and 7 differing. 44 words, 0 differing. */
void func_800BCFC4(void)
{
    u8 *b;

    if (D_80082E6B == 0x11)
    {
        b = (u8 *)0x80010000;
        if (*(s32 *)(b + 0x2090) == 0)
        {
            if (D_8008146C == 0x28)
            {
                if (func_80033BC0(0xA2) == 0)
                {
                    if (*(s32 *)(b + 0x208C) == 0)
                    {
                        *(s32 *)(b + 0x208C) = 1;
                        func_80040AA0(0x21);
                        D_800DCF4E = 0;
                        return;
                    }
                }
            }
        }
    }
    func_8003E4FC(0x6, D_800DF3CC, 0);
    func_8003F320();
}

/* func_800BCFC4 with D_800DF3D4. 44 words, 0 differing. */
void func_800BD074(void)
{
    u8 *b;

    if (D_80082E6B == 0x11)
    {
        b = (u8 *)0x80010000;
        if (*(s32 *)(b + 0x2090) == 0)
        {
            if (D_8008146C == 0x28)
            {
                if (func_80033BC0(0xA2) == 0)
                {
                    if (*(s32 *)(b + 0x208C) == 0)
                    {
                        *(s32 *)(b + 0x208C) = 1;
                        func_80040AA0(0x21);
                        D_800DCF4E = 0;
                        return;
                    }
                }
            }
        }
    }
    func_8003E4FC(0x6, D_800DF3D4, 0);
    func_8003F320();
}

/* 13 words, 0 differing. */
void func_800BD124(void)
{
    func_8003E4FC(0x6, D_800DF3DC, 0);
    D_800DCF4D = -1;
}

/* 11 words, 0 differing. */
void func_800BD158(void)
{
    func_8003E4FC(6, D_800DF3E4, 0);
}

/* 16 words, 0 differing. */
void func_800BD184(s16 i)
{
    func_8003E4FC(6, &D_800DF3EC[i * 8], 0);
    func_8003F320();
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD1C4);

/* The kill skeleton plus a spoken report, and the one that was parked for a
   day over two words.

   THE CALL VALUE GOES INTO THE NAME THAT IS WRITTEN AGAIN, and the
   long-lived handle is a copy taken off it:

       h = func_800990FC();
       m = h;
       h = func_80099194(MSG, h);
       ...
       func_800A5720(m);

   `make_regs_eqv` (cse.c:836-890) puts the two in one quantity with `h`
   canonical -- which is what the target wants, because the argument insn
   then keeps `h`, the SHORT pseudo, and local-alloc can give it $a1, making
   the argument move a no-op. The only damage cse does is rewriting `m`'s
   later use back to `h`, and reassigning `h` immediately after the copy
   calls `delete_reg_equiv` on it, so `m` becomes the class head and
   `func_800A5720(m)` is left alone. A third variable for func_80099194's
   result, or the copy written the other way round (`m = f(); h = m;`), is
   90 words and 2 differing -- which is what twelve spellings in the park
   file measured.

   This is a 19-INSTANCE IDIOM in this overlay. `addu $aN, $vM, $zero` …
   `jal` … `addu $sK, $aN, $zero` matches nineteen functions here and
   nowhere else in the project. 90 words, 0 differing. */
s32 func_800BD45C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x802080, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x40))
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E0E69, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    func_80042B68((s8 *)p, 0x5);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800BD45C with a different tint, mask, string and sound.
   90 words, 0 differing. */
s32 func_800BD5C4(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0xC02020, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x400))
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E0E82, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    func_80042B68((s8 *)p, 0x2);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE KILL HANDLERS: one skeleton, five landed copies and two parked. Every
   member opens the same way -- if the record IS the engine's current one
   (D_800E3D7C), park the caller's node at +0x110, reset the actor through
   func_8008D330 and report 0; otherwise, for a record below 0xA0000000, run
   func_800A63B8 and consult func_800AD6FC with a two-bit field pulled out of
   D_800DDE84[record[0x13]], and hand the whole thing to func_800A5F38 if
   that says no. What follows the gate is the per-entity part, and it is all
   that differs between the seven. They close alike: func_80098B38 on the
   node and the engine's +0x0A population counter down by one.

   The pointer gate is `(u32)p <= 0x9FFFFFFF`, which is the `sltu` against
   the constant read the way the listing has it -- the comparison is
   unsigned and the constant is the last KSEG0 address, so this is "is the
   record in cached RAM".

   func_8008D330 IS CALLED WITH FOUR ARGUMENTS HERE, which is the witness
   for the signature it now carries; see its own comment above.

   func_800BD45C and func_800BD5C4 are the two that did not land: they are
   this skeleton plus the message chain, and they sit at 90 of 90 words with
   2 differing. docs/near-misses/func_800BD45C.c has the analysis and the
   twelve spellings that were measured.

   80 words, 0 differing. */
s32 func_800BD72C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0xC0C0C0, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (!(*(u32 *)(p + 0x1C) & 0x200))
        {
            func_800997FC(D_800E0E9B);
        }
    }
    func_80042B68((s8 *)p, 0x1);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with the message chain unconditional inside the
   0x4000 gate -- no inner +0x1C test as func_800BD45C has -- and a long tail.
   118 words, 0 differing. */
s32 func_800BD86C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 i;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20C0C0, p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        h = func_80099194(D_800E0EB4, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800E0EC9, h);
        func_80099290(h);
        func_800A5720(m);
    }
    if (*(u32 *)(p + 0x1C) & 0x4000000)
    {
        p[0x28] = (p[0x29] >> 1) + 1;
    }
    p[0x26] = p[0x68];
    p[0x27] = p[0x69];
    for (i = 1; i < 0x21; i++)
    {
        func_80042B68((s8 *)p, i);
    }
    *(u32 *)(p + 0x1C) &= 0xFBFFE10F;
    func_80041E70(p);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton with a 0x2020C0 tint, the record's +0x68 copied into
   +0x26 and one message. 76 words, 0 differing. */
s32 func_800BDA44(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x2020C0, p);
    p[0x26] = p[0x68];
    func_80041E70(p);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E0EE1);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same skeleton with a 0x40C040 tint, the record's +0x69 copied into
   +0x27 and no func_80041E70. 73 words, 0 differing. */
s32 func_800BDB74(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x40C040, p);
    p[0x27] = p[0x69];
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E0F07);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BDC98);

/* THE KILL HANDLER WITH A FLAG, and four spellings had to be right at once.

   - `t = f` IS A NARROW COPY (u8 off a u16). Both tests read `t`, not `f`,
     and cse only leaves the copy alone because the modes differ; same-width
     and the two fuse into one pseudo and the three `addu $v0, $s3, $zero` the
     listing has disappear. `u16 f` rather than `s32` is what keeps the first
     gate's `return 0` as `addu $v0, $zero, $zero`.
   - THE TWO `goto`s ARE THE BRANCH TARGETS. Written as the plain
     `if (t == 0)` / `if (t != 0)` pair the first branch's target is the
     second test's own copy, reorg steals it into the delay slot and the
     function is a word long; jump threading cannot save it because
     `thread_jumps` refuses a target whose compared register is written
     between the label and the branch (jump.c's `modified_regs`).
   - `f = 1` IS WRITTEN TWICE in the 0x1E arm. The `||` spelling is one
     assignment and leaves `f` at five references against `old`'s two, which
     hands `old` the lower register. The if/else-if pair is a sixth reference
     that costs nothing: flow counts both insns and jump2's cross-jumping,
     which runs after it, puts them back into one `addiu $s3, $zero, 0x1`.

   154 words, 0 differing. */
s32 func_800BDEB8(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    void *m2;
    void *h2;
    u16 f;
    u8 t;
    u8 old;
    u8 cur;
    u8 *g;

    f = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x13] < 0x2E)
    {
        if (p[0x13] == 0x1E)
        {
            if (*(s32 *)(p + 0xA4) != 0)
            {
                f = 1;
            }
            else if (*(s16 *)(p + 0xAA) != 0)
            {
                f = 1;
            }
        }
        t = f;
        if (t != 0)
        {
            goto msg;
        }
        old = p[0x13];
        if (*(u32 *)(p + 0x1C) & 0x80000)
        {
            *(s16 *)(*(u8 **)(p - 0x14) + 6) = 0;
        }
        if ((s16)func_800A48F0((s8 *)p, 0xA, 0x20) >= 0)
        {
            cur = p[0x13];
            m = func_800990FC();
            p[0x13] = old;
            h = func_80099734(p, m);
            p[0x13] = cur;
            h = func_80099194(D_800E0FA4, h);
        }
        else
        {
            m = func_800990FC();
            h = func_80099194(D_800E0FB9, m);
        }
        func_80099290(h);
        func_800A5720(m);
        func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x613);
    }
    else
    {
        f = 1;
    }
    t = f;
    if (t == 0)
    {
        goto tail;
    }
msg:
    h2 = func_800990FC();
    m2 = h2;
    h2 = func_80099194(D_800E0FD7, h2);
    h2 = func_80099734(p, h2);
    h2 = func_80099194(D_80089358, h2);
    func_80099290(h2);
    func_800A5720(m2);
tail:
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The same `u8 t = f` flag as func_800BDEB8, but with ONE test, so the
   `goto`s are not needed: the copy at the head of the test block is
   duplicated into the four gates' delay slots by reorg. 144 words,
   0 differing. */
s32 func_800BE120(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u16 f;
    u8 t;
    u8 *g;

    f = 0;
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (p[0x13] >= 2 && !(*(u32 *)(p + 0x54) & 0x800000) && p[0x49] == 0)
        {
            if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
            {
                func_800A5F38(p, q);
                return 1;
            }
            *(s32 *)(p + 0x48) = *(s32 *)q;
            p[0x48] = q[0];
            p[0x49] = q[1];
            p[0x4A] = q[2];
            p[0x4B] = q[3];
            *(u32 *)(p + 0x54) |= 0x800000;
            func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x702);
            f = 1;
            if (*(u32 *)(p + 0x14) & 0x4000)
            {
                func_80099844(p, D_800E0FF4);
            }
        }
    }
    t = f;
    if (t == 0)
    {
        if (p[0x13] < 2 || (*(u32 *)(p + 0x54) & 0x800000) || p[0x49] != 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E100F, h);
            h = func_80099368(q, h);
            h = func_80099194(D_8008935C, h);
            func_80099290(h);
            func_800A5720(m);
            func_800A5F38(p, q);
            return 1;
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE360);

/* The kill skeleton's short member: no D_800E3D7C arm, func_800A6480 for the
   report and the 0x2-bit field of D_800DDE84's entry. THE THIRD PARAMETER IS
   NOT READ AND HAS NO WITNESS -- there is no `jal` to this address and no
   address-take anywhere under asm/nonmatchings/ovl_dungeon, so whatever
   dispatches it lives in the overlay's unsplit data. `s16 k` is carried over
   from the eleven landed members of the same family; the bytes are identical
   without it. 48 words, 0 differing. */
s32 func_800BE4E8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 2) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E1035);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* A kill handler with an OUT-OF-RANGE arm: where the other six fall through
   the pointer gate into shared code, these two branch to func_800997FC with
   a string of their own, and the `lui $a0, %hi(...)` for it sits in the
   gate's delay slot. The in-range arm applies an effect through
   func_800A48F0, announces it, and tints the node through func_800D5460.
   82 words, 0 differing. */
s32 func_800BE5A8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        if ((s16)func_800A48F0((s8 *)p, 0x20, 0x10) != 0)
        {
            if (*(u32 *)(p + 0x14) & 0x4000)
            {
                func_80099844(p, D_800E104E);
            }
        }
        func_800D5460((UnkNode *)(p - 0x20), 0xE0E0E0, 0x702);
    }
    else
    {
        func_800997FC(D_800E107C);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE6F0);

/* func_800BE5A8 with a RANDOM effect value -- `func_800A6D30() & 0x1F` with
   bit 5 forced on -- whose result is not tested, a different tint and a
   different pair of strings. 83 words, 0 differing. */
s32 func_800BE9E4(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_800A48F0((s8 *)p, 0xF, (func_800A6D30() & 0x1F) | 0x20);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E10C0);
        }
        func_800D5460((UnkNode *)(p - 0x20), 0x804040, 0x612);
    }
    else
    {
        func_800997FC(D_800E10F2);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE KILL HANDLER THAT BUILDS A SENTENCE. The second skeleton's gate, then
   an out-of-range arm that reports what happened in words: take a message
   buffer from func_800990FC and run it through the overlay's
   func_80099194 / func_80099368 / func_8009955C / func_80099978 /
   func_800998C0 chain, each returning the handle the next takes, and hand
   the result to func_80099290. D_80089364 and D_80089368 are two of the
   three fragments this overlay keeps in its own rodata.

   Two spellings cost words here:

   - `h` IS REUSED for func_80099368's result; a second variable for it puts
     that result in the temporary's register instead and moves four words.
     The OLD handle still has to reach func_800998C0, so it goes into its own
     variable first: `e2 = h; h = func_80099368(e, h); func_800998C0(t, e2);`
   - `t` HOLDS THE BIASED POINTER, `h - 2`, not `h`. Written as `t = h` with
     the `- 2` at the call site the subtraction moves to the call and the
     function is 1 word differing.

   The arms are in the target's physical order, which is the reverse of the
   natural reading twice over: the `e == NULL` report is the LAST arm and the
   `e[3] & 0x40` short form is the SECOND, so both are written as the `else`.
   175 words, 0 differing. */
s32 func_800BEB30(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *t;
    void *e2;
    u8 *e;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        q[3] &= 0xDF;
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(D_800E3D7C + 0x4C);
        if (e != NULL)
        {
            if (!(q[3] & 0x20))
            {
                *(s32 *)(D_800E3D7C + 0x114) = 0x202080;
                func_8008D368(D_800E3D7C, D_80083780, D_80082E80, D_800DD148, 0x2);
                q[3] |= 0x20;
                return 0;
            }
            m = func_800990FC();
            h = m;
            if (e[1] == 0xF || (e[1] == 0x10 && e[0] == 2))
            {
                if (!(e[3] & 0x40))
                {
                    h = func_8009955C(e, m);
                    if (*(s8 *)(e + 2) < 0x63)
                    {
                        e[2]++;
                    }
                    h = func_80099978(func_80099194(D_800E110B, h));
                    t = (u8 *)h - 2;
                    h = func_80099194(D_80089364, h);
                    e2 = h;
                    h = func_80099368(e, h);
                    func_800998C0(t, e2);
                    h = func_80099194(D_80089368, h);
                }
                else
                {
                    h = func_80099194(D_800E112A, func_80099368(e, m));
                }
            }
            else
            {
                h = func_80099194(D_800E1156, func_80099368(e, h));
            }
            func_80099290(h);
            func_800A5720(m);
        }
        else
        {
            func_800997FC(D_800E116E);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BEB30's shorter sibling: the record comes from +0x50 rather than
   +0x4C, there is no +0x01/+0x00 dispatch at all, and func_800990FC is
   called INSIDE each arm rather than once above them -- which is also why
   the final func_80099194 result goes straight into func_80099290 here
   instead of through a variable. 162 words, 0 differing. */
s32 func_800BEDEC(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *t;
    void *e2;
    u8 *e;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        q[3] &= 0xDF;
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(D_800E3D7C + 0x50);
        if (e != NULL)
        {
            if (!(q[3] & 0x20))
            {
                *(s32 *)(D_800E3D7C + 0x114) = 0x802020;
                func_8008D368(D_800E3D7C, D_80083780, D_80082E80, D_800DD158, 0);
                q[3] |= 0x20;
                return 0;
            }
            if (!(e[3] & 0x40))
            {
                m = func_800990FC();
                h = func_8009955C(e, m);
                if (*(s8 *)(e + 2) < 0x63)
                {
                    e[2]++;
                }
                h = func_80099978(func_80099194(D_800E118C, h));
                t = (u8 *)h - 2;
                h = func_80099194(D_8008936C, h);
                e2 = h;
                h = func_80099368(e, h);
                func_800998C0(t, e2);
                func_80099290(func_80099194(D_80089370, h));
            }
            else
            {
                m = func_800990FC();
                func_80099290(func_80099194(D_800E11AB, func_80099368(e, m)));
            }
            func_800A5720(m);
        }
        else
        {
            func_800997FC(D_800E11D7);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF074);

/* func_800BF34C with D_800E11FD and bit 0x800000, plus func_8009D8A4 and
   sound 0x80F on the out-of-range arm. 79 words, 0 differing. */
s32 func_800BF210(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E11FD);
        func_8009D8A4();
        D_800E296C |= 0x800000;
        func_800A56E0(0x80F);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* THE SECOND KILL SKELETON, in five copies. It is not the func_800BD72C one:
   four parameters rather than three, a 0xD short-circuit at the top that
   hands the node straight to func_80098864, func_800A6480 in place of
   func_800A63B8, func_8008D344 in place of func_8008D330, the D_800DDE84
   field taken as the LOW two bits rather than bits 6-7, the node passed to
   func_800AD6FC as its third argument instead of zero, and an out-of-range
   arm that reports a string and raises a bit in D_800E296C. Only that
   string and that bit differ between the five -- D_800E1224/0x100,
   D_800E1258/0x800, D_800E1279/0x400, D_800E12A9/0x200 -- except
   func_800BF210, which also rings func_8009D8A4 and sound 0x80F.

   The 0xD arm RETURNS func_80098864's value: the branch leaves $v0 alone
   and jumps to the epilogue, where the other arms set it explicitly.

   75 words, 0 differing. */
s32 func_800BF34C(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1224);
        D_800E296C |= 0x100;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E1258 and bit 0x800. 75 words, 0 differing. */
s32 func_800BF478(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1258);
        D_800E296C |= 0x800;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E1279 and bit 0x400. 75 words, 0 differing. */
s32 func_800BF5A4(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E1279);
        D_800E296C |= 0x400;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800BF34C with D_800E12A9 and bit 0x200. 75 words, 0 differing. */
s32 func_800BF6D0(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800997FC(D_800E12A9);
        D_800E296C |= 0x200;
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The second kill skeleton with an out-of-range arm that reaches the
   D_800E2970 room table through the record's -0x14 node: the 0x14-byte
   stride and the +0x0C flag halfword, with `b = D_800E2970;` as its own
   statement exactly as func_8009FCAC needs it. The +0x26 index is RE-READ
   after func_800C4EB4, which is the second `lb`. 124 words, 0 differing. */
s32 func_800BF7FC(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    u8 *b;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        i = *(s8 *)(e + 0x26);
        if (i >= 0)
        {
            b = D_800E2970;
            if (!(*(u16 *)(b + i * 0x14 + 0xC) & 2))
            {
                if (func_800C4EB4(e[0x24], e[0x25], *(s16 *)(p + 0x88), q[0], i) == 0)
                {
                    return 0;
                }
                i = *(s8 *)(e + 0x26);
                *(u16 *)(b + i * 0x14 + 0xC) |= 2;
                func_800C4D78((u8 *)0x20C0C0, 1);
                h = func_800990FC();
                m = h;
                h = func_80099194(D_800E12D6, h);
                func_80099290(h);
                func_800A5720(m);
            }
            else
            {
                func_800997FC(D_800E1303);
            }
        }
        else
        {
            func_800997FC(D_800E1303);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF9EC);

/* The first kill skeleton with a two-stage counter: when the record's +0x26
   has caught up with its +0x68 cap, bump +0x00 and notify func_80041E70;
   then raise +0x26 itself while it is below both 0xFF and the cap.
   96 words, 0 differing. */
s32 func_800BFC00(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x26] == p[0x68])
    {
        if (p[0x0] < 0xFF)
        {
            p[0x0]++;
            func_80041E70(p);
        }
    }
    if (p[0x26] < 0xFF && p[0x26] < p[0x68])
    {
        p[0x26]++;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E1350);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x2020F0, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800BFC00 one byte over -- +0x27/+0x69/+0x01 -- with its own string
   and tint. 96 words, 0 differing. */
s32 func_800BFD80(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x27] == p[0x69])
    {
        if (p[0x1] < 0xFF)
        {
            p[0x1]++;
            func_80041E70(p);
        }
    }
    if (p[0x27] < 0xFF && p[0x27] < p[0x69])
    {
        p[0x27]++;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E2056);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x20F020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BFF00);

/* func_800C3C08 on the +0x02 counter, with the effect applied
   unconditionally through func_800A48F0 and a different tint.
   81 words, 0 differing. */
s32 func_800C003C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x2] < 0xFF)
    {
        p[0x2]++;
    }
    func_800A48F0((s8 *)p, 0x7, 0x8);
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        func_80099844(p, D_800E206A);
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0xF02020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 44 words, 0 differing. The +0xAC slot table walked DOWNWARD from 1, with
   D_800E3D7C re-read every turn -- the inline `i * 4 + D_800E3D7C + 0xAC`
   spelling, not the pointer-local hoist. */
s32 func_800C0180(void)
{
    s32 i;
    u8 *p;

    if (func_800C07AC(D_80083498) == NULL)
    {
        return 0;
    }
    for (i = 1; i >= 0; i--)
    {
        p = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
        if (p != NULL)
        {
            func_800C542C(p, D_800DCED4[func_800429E4(p)], (s16)i, 0);
        }
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0230);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0404);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C07AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C08A4);

/* The elemental-brand handler. TWO PAIRS OF HANDLE VARIABLES, not one. The
   in-range chain and the out-of-range chain share `m`/`h` at 2 differing:
   `m` becomes a global allocno, the in-range chain loses the
   reused-temporary shape and the record takes $s0 away from the handle.
   `c = p + 8` BEFORE `i = 0` in the for-init is the last word.
   202 words, 0 differing. */
s32 func_800C0B60(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    void *m2;
    void *h2;
    u8 *s;
    u8 *c;
    s32 i;
    s32 n;
    u32 f;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        *(u32 *)(p + 0x14) &= ~7;
        *(u32 *)(p + 0x1C) &= ~7;
        if (q[0] == 5)
        {
            s = D_800E13FB;
            *(u32 *)(p + 0x14) |= 1;
            *(u32 *)(p + 0x1C) |= 1;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF0, 0x81C);
        }
        else if (q[0] == 6)
        {
            s = D_800E1402;
            *(u32 *)(p + 0x14) |= 2;
            *(u32 *)(p + 0x1C) |= 2;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF00000, 0x81C);
        }
        else
        {
            s = D_800E140A;
            *(u32 *)(p + 0x14) |= 4;
            *(u32 *)(p + 0x1C) |= 4;
            func_800D4FC8((UnkNode *)(p - 0x20), 0xF000, 0x81C);
        }
        func_80042984(p);
        for (c = p + 8, i = 0; i < 3; i++, c += 3)
        {
            if (c[0] != 0)
            {
                n = c[0];
                f = *(u32 *)(p + 0x14);
                if (((u8)D_8006DE24[n].unk_10 & 7) & f)
                {
                    continue;
                }
                if (f & 1)
                {
                    c[0] = (n - 1) / 3 * 3 + 1;
                }
                else if (f & 2)
                {
                    c[0] = (n - 1) / 3 * 3 + 2;
                }
                else if (f & 4)
                {
                    c[0] = (n - 1) / 3 * 3 + 3;
                }
            }
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E1411, h);
            h = func_80099194(s, h);
            h = func_80099194(D_80089380, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    else
    {
        h2 = func_800990FC();
        m2 = h2;
        h2 = func_80099194(D_800E1426, h2);
        h2 = func_80099734(p, h2);
        h2 = func_80099194(D_80089380, h2);
        func_80099290(h2);
        func_800A5720(m2);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0E88);

/* THE `p == NULL` ARM IS AN EARLY RETURN and is laid out at the END. Spelled
   `if (p != NULL) { ...; return 1; } return 0;` the `return 0` collapses into
   the `beqz`'s delay slot and the function is 25 words against 27.
   27 words, 0 differing. */
s32 func_800C135C(void)
{
    u8 *p;

    p = func_800C9850(D_80082E80[0x24], D_80082E80[0x25], *(s16 *)(D_800E3D7C + 0x88));
    if (p == NULL)
    {
        return 0;
    }
    *(s32 *)(*(u8 **)(p - 0x14) + 0xC) = 0;
    func_80042640((Unk42710 *)p, p[0x13]);
    return 1;
}

/* WIP C13C8 */
s32 func_800C13C8(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, NULL);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DEAE0, 0x5000, 0x202080, func_800C135C) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E1482);
    }
    func_80098B38(q);
    return 1;
}

/* func_8003FA44's gate, then one func_800C1A44 per occupied engine slot.
   34 words, 0 differing. */
s32 func_800C152C(void)
{
    s32 i;
    u8 *p;
    u8 *e;

    if (func_8003FA44(2) == 0)
    {
        return 0;
    }
    for (i = 0; i < 2; i++)
    {
        p = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
        if (p != NULL)
        {
            e = *(u8 **)(p - 0x14);
            func_800C1A44(e[0x24], e[0x25], *(s16 *)(p + 0x88));
        }
    }
    return 1;
}

/* WIP C15B4 */
s32 func_800C15B4(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, NULL);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else
    {
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DF514, 0x2800, 0x802020, func_800C152C) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E14A6);
    }
    func_80098B38(q);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1718);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1A44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1B1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1CE0);

/* 25 words, 0 differing. */
void func_800C1EC4(u8 *p)
{
    u8 *g;

    if (--*(s16 *)(p + 0x2) <= 0)
    {
        *(s16 *)(*(s32 *)(p + 0xC) + 0x88) = *(u16 *)(p + 0x8);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1F28);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2130);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C22EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C24A8);

/* The first kill skeleton WITHOUT the pointer gate. TWO SCRATCH LOCALS FOR
   D_80083460, one per site. With one `g` the symbol becomes a global
   allocno, both pairs come out as `lui $v0` / `addiu $a1` split across two
   registers, and the function is 10 differing at the right length.
   117 words, 0 differing. */
s32 func_800C261C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *g;
    u8 *g2;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        if (p[0x13] >= 3 && p[0x13] < 0x2E)
        {
            if (D_80012090 == 0 && D_8008146C == 0x28 && func_80033BC0(0xA2) == 0)
            {
                func_800997FC(D_800E15FE);
                g2 = (u8 *)&D_80083460;
                *(u16 *)(g2 + 0xA) -= 1;
                func_80098B38(q);
                return 1;
            }
            if (func_800C2C7C(p) == 0)
            {
                return 0;
            }
            func_80098B38(q);
        }
        else
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E1677, h);
            h = func_80099368(q, h);
            h = func_80099194(D_80089384, h);
            func_80099290(h);
            func_800A5720(m);
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        if (p[0x13] >= 3 && p[0x13] < 0x2E)
        {
            if (func_800C2C7C(p) == 0)
            {
                return 0;
            }
        }
        else
        {
            func_800A63B8(p, q, k);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* THE CALL GETS ITS OWN STATEMENT, which is func_800A0E44's lever: written
   as `D_800DF560 = func_800C07AC(...)` the global's `lui` expands ahead of
   the call. 13 words, 0 differing. */
s32 func_800C27F0(void)
{
    void *p;

    p = func_800C07AC(D_800DF55C - 0x20);
    D_800DF560 = p;

    return p != NULL;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089354);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089358);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_8008935C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089360);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089364);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089368);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_8008936C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089370);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089374);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089378);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_8008937C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089380);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089384);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2824);

/* THE NODE IS A `u8 *`, NOT AN `UnkNode *`. With `UnkNode *n` and
   `n->unk_10 = (u32)func_800C2824;` the node pseudo loses $a0 to the
   handler's `(high)`, the two address pairs interleave, the `addiu` of the
   second fills the `lhu`'s load-delay slot and the function is 23 words
   against 24 with 18 differing -- in either store order. 24 words,
   0 differing. */
u8 *func_800C2C7C(u8 *p)
{
    u8 *n;
    u8 *g;

    n = (u8 *)func_8003FC64(2);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800C2824;
        *(u8 **)(n + 0x20) = p;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2CDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2F2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C30D4);

/* The same skeleton again, with a charge roll through func_800C8900 and
   sound 0x612 on a hit. 65 words, 0 differing. */
s32 func_800C3238(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800C8900(p, 0x400, 0x8) != 0)
    {
        func_800A56E0(0x612);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800C3238 with a func_800A2424 gate and a message instead of a
   sound. 65 words, 0 differing. */
s32 func_800C333C(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800A2424(p, 1) == 0)
    {
        func_80099844(p, D_800E173A);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3440);

/* The same skeleton with NO func_800C4AFC tint at all: this one spends a
   +0x26 charge instead, with sound 0x615, and only when at least two are
   left. func_800C37C8 is it again on +0x27 with D_800E2082.
   78 words, 0 differing. */
s32 func_800C3690(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x26] >= 2)
    {
        func_800A56E0(0x615);
        p[0x26]--;
        func_80041E70(p);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E17A1);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* func_800C3690 on the record's +0x27 charge with D_800E2082.
   78 words, 0 differing. */
s32 func_800C37C8(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x27] >= 2)
    {
        func_800A56E0(0x615);
        p[0x27]--;
        func_80041E70(p);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E2082);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3900);

/* The same skeleton with an effect applied through func_800A48F0 -- the
   engine entry whose return type include/structs.h had as void until this
   block was opened; the `sll $v0, 16` before the `beqz` is the halfword
   test that corrected it. 74 words, 0 differing. */
s32 func_800C3AE0(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if ((s16)func_800A48F0((s8 *)p, 0x7, -0xA) != 0)
    {
        func_800A56E0(0x615);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1827);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The first kill skeleton with a +0x03 counter bumped and the node handed
   to func_800D4FC8 for its burst. 77 words, 0 differing. */
s32 func_800C3C08(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (p[0x3] < 0xFF)
    {
        p[0x3]++;
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1843);
        }
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x202020, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3D3C);

/* The second kill skeleton with the chain in the IN-RANGE arm and a 64-slot
   sweep out of range. The sweep indexes both tables off the counter through
   UnkDungeonSlot4 / UnkDungeonSpotC: see those types for the eight words the
   `u8 *` cursors cost. 124 words, 0 differing. */
s32 func_800C4030(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    s32 i;
    u8 *g;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        h = func_800990FC();
        m = h;
        h = func_80099368(q, h);
        h = func_80099194(D_800E187C, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893DC, h);
        func_80099290(func_800999B0(h));
        func_800A5720(m);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_8009BF7C(1, 8);
        func_800A56E0(0x80F);
        for (i = 0; i < 0x40; i++)
        {
            if (D_800E3548_t[i].unk_01 != 0)
            {
                D_800E3548_t[i].unk_01 = 0xE;
                D_800E3548_t[i].unk_00 = 3;
                D_800E36C8_t[i].unk_08 = func_8004A658(0xE, 3);
                D_800E3548_t[i].unk_02 = func_800A6DA4(0x10, 0x18);
                D_800E3548_t[i].unk_03 = 1;
            }
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* func_800C3238 with func_800C8A3C and sound 0x520.
   65 words, 0 differing. */
s32 func_800C4220(u8 *p, u8 *q, s16 k)
{
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    if (func_800C8A3C(p, 0x400, 0x8) != 0)
    {
        func_800A56E0(0x520);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4324);

/* The heal: 7 plus two random bits, clamped to 0xFF and then to the gap
   between +0x28 and +0x29, parked in +0x64 and read back out of the field
   for func_8003AD08 to spell. The func_800AD6FC gate is tested as a HALFWORD
   here (`sll 16` with no `sra`), which is the (s16) cast. 108 words,
   0 differing. */
s32 func_800C4490(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 n;
    s32 d;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        if ((s16)func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    n = (func_800A6D30() & 3) + 7;
    if (n > 0xFF)
    {
        n = 0xFF;
    }
    d = p[0x29] - p[0x28];
    if (d < n)
    {
        n = d;
    }
    *(s16 *)(p + 0x64) = n;
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        h = func_800990FC();
        m = h;
        h = func_80099734(p, h);
        h = func_80099194(D_800E18A4, h);
        h = func_8003AD08(*(s16 *)(p + 0x64), h);
        h = func_80099194(D_800893E0, h);
        func_80099290(h);
        func_800A5720(m);
    }
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20E020, p);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4640);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4944);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4AFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4C00);

/* Spawn a kind-0x200 node carrying the caller's record, park func_800C4C00
   at its +0x10, and make a noise when the flag is set. 34 words,
   0 differing. */
void func_800C4D78(u8 *p, s16 noise)
{
    UnkNode *n;
    u8 *g;

    n = func_8003FD64(0x200, D_80083498);
    if (n != NULL)
    {
        *(s16 *)((u8 *)n + 0x26) = 0x20;
        n->unk_10 = (u32)func_800C4C00;
        *(u8 **)((u8 *)n + 0x20) = p;
        if (noise != 0)
        {
            func_800A56E0(0x501);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}

/* A node's +0x10 step, gated on the engine's 0x10 flag. The record is
   reached as an ARRAY OF 0x14-BYTE STRUCTS, which is what puts the
   `lui`/`addiu` ahead of the index chain and makes the final `addu`
   index-first. 45 words, 0 differing. */
void func_800C4E00(u8 *p)
{
    if ((D_80083462 & 0x10) != 0)
    {
        if (--*(s16 *)(p + 0x8) <= 0)
        {
            if (*(s16 *)(p + 0xA) == 1)
            {
                func_800997FC(D_800E18C8);
                D_800E2970_s[*(s16 *)(p + 0xC)].unk_0C &= ~2;
            }
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
    }
}

/* Allocate a kind-0 node under D_80083498, park func_800C4E00 at its +0x10
   and fill three halfwords of its payload. The first three parameters are
   unread here; the fourth and the stack fifth are the two halfwords, and
   func_800BF7FC's call site passes them with `lbu`/`sw` so neither width is
   witnessed -- s16 is this unit's convention for a parameter whose only use
   is a halfword store. 27 words, 0 differing. */
UnkNode *func_800C4EB4(s32 a, s32 b, s32 c, s16 d, s16 e)
{
    UnkNode *n;
    u8 *q;

    n = func_8003FD64(0, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800C4E00;
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x8) = 0x40;
        *(s16 *)(q + 0xA) = d;
        *(s16 *)(q + 0xC) = e;
    }
    return n;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800893D4);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800893D8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800893DC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800893E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4F20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C542C);

s32 func_800C55A0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C55F4(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C55F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5A64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5BBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5D80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5E5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5FA8);

/* 15 words, 0 differing. */
void func_800C6AB0(void)
{
    UnkNode *n;

    n = func_8003FF2C(0, (UnkNode *)D_800E58B8, 0x10, (UnkNode **)D_80083498);
    n->unk_10 = (u32)func_800C5FA8;
}

/* Variant B of the node walk, func_800C6AEC and func_800D67B0: the visitor's
   fourth argument is a constant 0 rather than the slot's unk_06, which is
   what moves the `addu $a0, $s0, $zero` above the loop head -- with no `lh`
   to schedule, reorg has a free slot at the bottom of the loop and hoists the
   copy into it. 21 words each, 0 differing. */
s32 func_800C6AEC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C6B40(p, v, s, 0);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C6B40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7380);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C75D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C77D0);

/* Fire func_800C77D0 at (x, z) only while both axes are within 0xC0 of
   D_80083780's own position. THE FIRST THREE COORDINATE PARAMETERS ARE `s32`,
   not `s16`: the target subtracts the incoming registers raw and only
   truncates them at the `sh` stores, so an `s16` declaration adds a
   `sll 16`/`sra 16` pair per axis -- 37 words against 36 with 35 differing.
   The FIFTH argument, on the stack, IS narrow: it is `lw`-ed at entry and
   sign-extended at the call, which is what a promoted `s16` stack parameter
   gives. 36 words, 0 differing. */
void func_800C78A0(u8 *a, s32 x, s32 z, s32 ang, s16 e, s32 f)
{
    u8 *g;
    u8 *r;
    s32 d;

    g = D_80083780;
    d = *(s16 *)(g + 0x2) - x;
    if (d < 0)
    {
        d = -d;
    }
    if (d < 0xC1)
    {
        d = *(s16 *)(g + 0x6) - z;
        if (d < 0)
        {
            d = -d;
        }
        if (d < 0xC1)
        {
            r = D_800E58F8;
            *(s16 *)(r + 0x2) = x;
            *(s16 *)(r + 0x6) = z;
            *(s16 *)(r + 0xA) = ang;
            func_800C77D0(a, r, e, f);
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7930);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7A3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7B38);

/* Run one cue list: func_800BD184 on the byte D_800DF64C[k] names, then every
   entry of D_800DF640[k] until its function pointer is NULL. The second
   walked register is loop.c combining the four halfword reads into one
   address giv biased to the last of them (+0xA), which is why the listing
   reads at -6/-4/-2/0. 38 words, 0 differing. */
void func_800C7D54(s16 k)
{
    UnkDungeonCue *q;

    func_800BD184(D_800DF64C[k]);
    for (q = D_800DF640[k]; q->unk_00 != NULL; q++)
    {
        q->unk_00(q->unk_04, q->unk_06, q->unk_08, q->unk_0A);
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7DEC);

/* True while anything is still blocking. THE LAST TEST HAS TO SHARE THE
   FUNCTION'S EXIT with the loop's, as an `else if` rather than a third
   top-level `if`: written flat, jump.c store-flags the final
   `return 1`/`return 0` pair into one `sltu $v0, $zero, $v0` and the
   function is 35 words against 37 with 10 differing. 37 words,
   0 differing. */
s32 func_800C7F68(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *e;

    if (D_800E296C & 0x10000000)
    {
        return 1;
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        q = D_800E3D7C + 4;
        for (i = 1; i >= 0; i--, q -= 4)
        {
            e = *(u8 **)(q + 0xAC);
            if (e != NULL && (*(u32 *)(e + 0x54) & 0x20000))
            {
                return 1;
            }
        }
    }
    else if (*(u32 *)(p + 0x54) & 0x20000)
    {
        return 1;
    }
    return 0;
}

/* Six copies of one flag probe, one per bit. With the record's +0x14 bit
   0x4000 set the test runs over the engine's two-entry party table instead
   -- D_800E3D7C's +0xAC pointers, walked DOWNWARDS from entry 1, which is
   what the `addiu $v1, $v1, -0x4` says -- and otherwise it is the record's
   own +0x54.

   THE SINGLE TRAILING `return 0` IS THE SHAPE, and it is worth two words per
   member. Give each arm its own `return 0` and jump.c store-flags the last
   pair into one `sltu $v0, $zero, $v0`; with one shared exit the branch
   survives, and the `return` in each arm is also what keeps $v0 live into
   the epilogue so the else arm's mask chain lands in $v1/$a0 rather than
   $v0/$v1 (docs/matching-levers.md, "Duplicate the tail per arm"). Measured:
   six tail spellings -- bool return, explicit else, inverted test, goto,
   local, do-while loop -- all give 29 words and 9 differing; this gives 31
   and 0. 31 words each, 0 differing. */
s32 func_800C7FFC(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x20000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x20000000)
    {
        return 1;
    }
    return 0;
}

/* Does anything hold the 0x4000 flag? THE LAST TEST MUST SHARE THE
   FUNCTION'S ONE `return 0`: written flat, jump.c store-flags the final 1/0
   pair into a single `sltu $v0, $zero, $v0` and the function is 28 words / 8
   differing. 30 words, 0 differing. */
s32 func_800C8078(u8 *p)
{
    u8 *base;
    u8 *q;
    s32 i;

    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        base = D_800E3D7C;
        for (i = 1; i >= 0; i--)
        {
            q = *(u8 **)(base + i * 4 + 0xAC);
            if (q != NULL && (*(u32 *)(q + 0x54) & 0x4000))
            {
                return 1;
            }
        }
    }
    else if (*(u32 *)(p + 0x54) & 0x4000)
    {
        return 1;
    }
    return 0;
}

/* Does either of the engine's two +0xAC slots hold a record with 0x10000 set?
   The 0x4000 gate and the exhausted loop share the one `return 0`.
   24 words, 0 differing. */
s32 func_800C80F0(u8 *p)
{
    u8 *base;
    u8 *q;
    s32 i;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        base = D_800E3D7C;
        for (i = 1; i >= 0; i--)
        {
            q = *(u8 **)(base + i * 4 + 0xAC);
            if (q != NULL)
            {
                if (*(s32 *)(q + 0x54) & 0x10000)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8150);

/* Count the engine's two occupied slots, but only for the player's own record
   and only while its +0x54 carries 8. The loop counts DOWN from 1 with the
   pointer biased +4 and decremented. 22 words, 0 differing. */
s32 func_800C82B8(u8 *p)
{
    s32 i;
    u8 *q;
    s32 n;

    n = 0;
    if (p == D_800E3D7C)
    {
        for (i = 1, q = p + 4; i >= 0; i--, q -= 4)
        {
            if (*(s32 *)(q + 0xAC) != 0)
            {
                if ((*(s32 *)(p + 0x54) & 8) != 0)
                {
                    n++;
                }
            }
        }
    }
    return n;
}

s32 func_800C8310(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x00080000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x00080000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C838C(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x08000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x08000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8408(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x02000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x02000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8484(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x10000000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x10000000)
    {
        return 1;
    }
    return 0;
}

s32 func_800C8500(u8 *p)
{
    s32 i;
    u8 *q;
    u8 *r;

    if (*(s32 *)(p + 0x14) & 0x4000)
    {
        for (i = 1, q = D_800E3D7C + 4; i >= 0; i--, q -= 4)
        {
            r = *(u8 **)(q + 0xAC);
            if (r != NULL && (*(s32 *)(r + 0x54) & 0x00100000))
            {
                return 1;
            }
        }
    }
    else if (*(s32 *)(p + 0x54) & 0x00100000)
    {
        return 1;
    }
    return 0;
}

/* The two-state swap between 0xB and 0xC with a percentage roll in front of
   it. THREE SPELLINGS carry it: `s32 v = func_800A6D30();` with the u16
   returner, so the `andi 0xFFFF` lands at the assignment; `d` HOLDS THE
   SHIFT, so the two arms share one `srl` (written out twice it is 103
   differing); and `s16 res`, not `s32` -- every test of it is `sll 16` then
   `bltz`/`bgez` and the return is the `nor`/`srl 31` of `res >= 0`.
   131 words, 0 differing. */
s32 func_800C857C(u8 *p, u8 *q)
{
    void *m;
    void *h;
    s32 v;
    s32 n;
    s32 d;
    s16 res;

    res = -1;
    if (func_800C8500(q) != 0)
    {
        if (*(u32 *)(q + 0x14) & 0x4000)
        {
            func_80099844(q, D_800E196F);
        }
        return 0;
    }
    v = func_800A6D30();
    d = p[0x11] >> 2;
    n = (p[3] != 0) ? (v % p[3] + d) : d;
    if (n >= 0x30)
    {
        if (*(u32 *)(p + 0x1C) & 0x2000)
        {
            if (!(*(u32 *)(q + 0x1C) & 0x2000))
            {
                if ((s16)func_80042900((s8 *)q, 0xC) != 0)
                {
                    func_80042B68((s8 *)q, 0xC);
                    res = 0;
                }
                else
                {
                    res = func_800A48F0((s8 *)q, 0xB, 0x10);
                    if (res >= 0)
                    {
                        h = func_800990FC();
                        m = h;
                        h = func_80099194(D_800E1987, h);
                        h = func_80099734(q, h);
                        h = func_80099194(D_8008942C, h);
                        func_80099290(h);
                        func_800A5720(m);
                    }
                }
            }
        }
        else if (*(u32 *)(q + 0x1C) & 0x2000)
        {
            if ((s16)func_80042900((s8 *)q, 0xB) != 0)
            {
                func_80042B68((s8 *)q, 0xB);
                res = 0;
            }
            else
            {
                res = func_800A48F0((s8 *)q, 0xC, 0x10);
                if (res >= 0)
                {
                    func_80099844(q, D_800E1996);
                }
            }
        }
    }
    if (res < 0)
    {
        if (*(u32 *)(q + 0x14) & 0x4000)
        {
            func_80099844(q, D_800E19AA);
        }
    }
    return res >= 0;
}

/* The `nor`/`srl 31` pair is `>= 0` on the sign-extended halfword.
   15 words, 0 differing. */
s32 func_800C8788(u8 *p, u8 *q)
{
    u8 v;

    v = p[0x11];

    return func_800C8DB0(q, v << 2, (v >> 2) + 0x10) >= 0;
}

/* And the three callers of those: report the outcome in a message, with a
   second message through the overlay's own slot when the record's +0x13
   byte is clear. 32 words each, 0 differing. */
s32 func_800C87C4(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8844(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E19C2);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF1C);
        }
        return 1;
    }
    func_80099844(p, D_800E19D3);
    return 0;
}

/* The probe's three callers: refuse outright if the flag is set, then draw
   a random number modulo the record's +0x03 and compare it with the chance
   the caller passed -- 0xFF being the unconditional case -- and on a pass
   apply the effect, which succeeds on a NON-NEGATIVE halfword result.

   TWO LOCALS, NOT ONE, and the types are both forced. The draw is an s32,
   which is where the `andi $a0, $v0, 0xFFFF` comes from -- func_800A6D30 is
   declared u16 and the widening is at the assignment -- and the remainder is
   an s16. Written as one variable (`roll = func_800A6D30(); roll = roll %%
   p[3];`) the pseudo lives from the call to the compare, which is longer
   than the promoted `chance`, so local-alloc hands $v1 to chance and $a0 to
   the remainder, the `addu $a0, $s0, $zero` for the next call fills the
   `bnez` delay slot, and the function comes out 46 words and 23 differing.
   Measured across eleven spellings; only the split pair with these two types
   reaches 0. Same single trailing `return 0` as the probes above.
   47 words each, 0 differing. */
s32 func_800C8844(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C7FFC(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x5, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C8900(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8980(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E19E8);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF18);
        }
        return 1;
    }
    func_80099844(p, D_800E1A01);
    return 0;
}

s32 func_800C8980(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C8484(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x4, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_800C8A3C(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8ABC(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E1A1D);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF0C);
        }
        return 1;
    }
    func_80099844(p, D_800E1A2D);
    return 0;
}

/* The fourth member of the func_800C8844 probe family: a different gate,
   effect 1 instead of 5, and the chance compared against HALF the caller's
   value -- the `srl 31` / `addu` / `sra 1` trio is the signed `/ 2`, and it
   is the whole 50-against-47-word difference. 50 words, 0 differing. */
s32 func_800C8ABC(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C8408(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance / 2 || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x1, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

/* func_800CD294's gate on its own. The HIT arm is written first -- the
   target's `beqz` puts the miss arm out of line, and inverting the test is
   38 words at 21 differing. 38 words, 0 differing. */
s32 func_800C8B84(u8 *p, s16 chance, s16 arg)
{
    if (func_800C8C1C(p, chance, arg) != 0)
    {
        func_80099844(p, D_800E1A44);
        if (p[0x13] == 0)
        {
            func_800DC1B8(D_800DCF14);
        }
        return 1;
    }
    if (*(u32 *)(p + 0x1C) & 0x10)
    {
        return 0;
    }
    func_80099844(p, D_800E191B);
    return 0;
}

s32 func_800C8C1C(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

    if (func_800C838C(p) == 0)
    {
        r = func_800A6D30();
        if (p[3] != 0)
        {
            roll = r % p[3];
        }
        else
        {
            roll = 0;
        }
        if (roll < chance || chance == 0xFF)
        {
            if (func_800A48F0((s8 *)p, 0x3, (s8)arg) >= 0)
            {
                return 1;
            }
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8CD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8DB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8EFC);

s32 func_800C9034(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800C9088(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9088);

/* The node walk behind func_800C96E8. Same `while (1)` shape as the landed
   func_800D5AC0, including `p = (u8 *)n + 0x20;` ahead of `v = n->unk_08;`.
   41 words, 0 differing. */
s32 func_800C95C0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkDungeonSP2 *sp = (UnkDungeonSP2 *)0x1F800000;
    UnkNode *n;

    while (1)
    {
        sp->unk_00 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_02 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_04 = *(u16 *)((u8 *)v + 0xA);
        sp->unk_C0 =
            func_80065420((SVECTOR *)&sp->unk_00, &sp->unk_B8, &sp->unk_90, &sp->unk_94) -
            s->unk_06;
        func_800C96E8(p, sp);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
    return 0;
}

/* Walk a node and its +0x18 chain, projecting each one through the
   0x1F800000 scratchpad. +0xB8/+0xBA are inside Unk463ECSP's pad_98 and
   reached by cast; +0xC0 is its own field. 33 words, 0 differing. */
s32 func_800C9664(u8 *p, UnkVec *m)
{
    Unk463ECSP *sp;
    UnkNode *n;

    sp = (Unk463ECSP *)0x1F800000;
    for (;;)
    {
        *(s16 *)((u8 *)sp + 0xB8) = *(u16 *)((u8 *)m + 0x2) - (*(s16 *)p >> 1);
        *(s16 *)((u8 *)sp + 0xBA) = *(u16 *)((u8 *)m + 0x6);
        sp->unk_C0 = *(s16 *)((u8 *)m + 0xA);
        func_800C96E8(p, sp);
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        m = (UnkVec *)n->unk_08;
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C96E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9850);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9AAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9F34);

/* Step the record's waypoint cursor one entry on, move the tile with it, and
   re-aim. TWO WRITTEN ORDERS, 4 differing between them. The guard reads the
   LIMIT first (`d[0x71] > *(s16 *)(d + 0x8A)`) -- the other way round the
   `lh` and the `lbu` swap places. And the row address is BASE-FIRST:
   `*(u8 *)(d + idx + 0x74)` gives `addu $v0, $s0, $v1`, where the subscript
   `d[idx + 0x74]` gives the index-first `addu $v0, $v1, $s0`. The two
   coordinate reads before the stores are the OLD values the call still
   wants; gcc keeps them above the stores by itself. 65 words, 0 differing. */
void func_800CA0DC(u8 *p, void *b, u8 *c, u8 *d)
{
    s32 n;
    u8 x;
    u8 y;

    if (*(s8 *)(d + 0x71) > 0)
    {
        if (d[0x71] > *(s16 *)(d + 0x8A))
        {
            x = c[0x24];
            c[0x24] = *(u8 *)(d + *(s16 *)(d + 0x8A) + 0x74);
            y = c[0x25];
            c[0x25] = *(u8 *)(d + *(s16 *)(d + 0x8A) + 0x7C);
            *(s16 *)(d + 0x8A) = *(u16 *)(d + 0x8A) + 1;
            *(s16 *)(d + 0x2A) = func_800A0818(x, y, c[0x24], c[0x25], (s32 *)(p + 0x98));
            *(s32 *)(d + 0x1C) |= 0x40000000;
            p[0x9A] = 0xF;
            *(s32 *)(p + 0x8C) = 0;
            if (D_80083462 & 0x80)
            {
                *(s16 *)(p + 0x96) = 0;
            }
            else
            {
                *(s16 *)(p + 0x96) = 8;
                n = d[0x71];
                if (n > 0)
                {
                    *(s16 *)(p + 0x96) = 8 / n;
                }
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA1E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA444);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA788);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA93C);

/* 13 words, 0 differing. */
void func_800CAA94(UnkDungeonActor *p)
{
    p->unk_9A = 0x8;
    p->unk_96 = 0x40;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(s32 *)((u8 *)p + 0x1C) = (*(s32 *)((u8 *)p + 0x1C) | 0x4000000) & ~0x1638;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CAAC8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_8008942C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089430);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CABA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CAED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CAFDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB170);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB374);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB4C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB600);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB82C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB9DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CBB98);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CBCA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CBDB4);

/* Spawn the kind-2 node behind the 0x1C probe, or replay the refusal line.

   THE NODE'S TWO STORES MUST BE PLAIN CASTS, NOT `n->field`. As COMPONENT_REFs
   they carry MEM_IN_STRUCT_P, and true_dependence's exemption then lets the
   `lhu %lo(D_80083460 + 0xA)` hoist above them: the three address
   materialisations overlap, reload spreads them over $a2/$a3/$v1 instead of
   reusing $v1 three times, and it is 10-11 differing. Through a `u8 *` the
   stores are ordinary MEMs, nothing is exempt, and every value dies before
   the next is made.

   THE REFUSAL CALL IS WRITTEN LAST, inside the outer `if`, so it falls into
   the shared `return 1`; written as an early arm it is 28 differing.
   `t = 0x10;` stored and then passed is the reused-temporary idiom: the
   `addu $v1, $a0, $zero` is the copy that keeps $a0 for the call.
   52 words, 0 differing. */
s32 func_800CBF88(u8 *p)
{
    u8 *q;
    u8 *g;
    s32 t;

    if (p == D_800E3D7C)
    {
        if (func_8003FA44(1) == 0)
        {
            return 0;
        }
        if (!(s16)func_80042900((s8 *)p, 0x1C))
        {
            if ((s16)func_800A48F0(p, 0x1C, 0x10) < 0)
            {
                return 1;
            }
            q = (u8 *)func_8003FC64(2);
            *(u32 *)(q + 0x10) = (u32)func_800CBDB4;
            t = 0x10;
            *(s16 *)(q + 0x24) = t;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
            func_800419EC(t, 8);
            func_800A56E0(0x818);
            return 1;
        }
        func_800997FC(D_800E1B5D);
    }
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC058);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC18C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC2A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC370);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC4F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC5F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC744);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC88C);

/* func_800999D4's close over a byte countdown at +0x7. 22 words,
   0 differing. */
void func_800CC9BC(u8 *p)
{
    u8 *g;

    if (--p[0x7] == 0)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCA14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCC20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCDA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD004);

/* THREE 60-WORD EFFECT HANDLERS, one source. Roll a chance through one of the
   probe callers -- the chance itself doubles from 0x40 to 0x400 while the
   engine's D_800E3D40 flag is up -- and on a miss poke func_800A6508 and
   leave. On a hit, announce it, show the overlay's own message when the
   record's +0x13 is clear, and unless the renderer already carries 0x8000,
   fire func_800C5BBC at the tile centre with a tint.

   func_800A6508 TAKES NO ARGUMENTS, and the witness is a `nop`. The target
   leaves its `jal` delay slot empty while `$a0` already holds the record --
   put there by the `addu $a0, $s0, $zero` in the BRANCH delay slot above,
   which is the OTHER path's func_80099844 argument that reorg hoisted
   because it is harmless here. Given a `u8 *` parameter the call emits its
   own copy, that copy fills the delay slot, and the function is one word
   over at the same length. An empty `jal` slot next to a live argument
   register is a nullary callee.

   60 words each, 0 differing. */
s32 func_800CD1A4(u8 *p)
{
    u8 *r;

    if (func_800C8ABC(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1BF3);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF0C);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x808080, 0x20,
                  0);
    func_800A56E0(0x520);

    return 1;
}

/* func_800CD1A4 with func_800C8C1C, D_800E1C03/D_800DCF14, a white tint, a
   0x40 fifth argument and sound 0x612. 60 words, 0 differing. */
s32 func_800CD294(u8 *p)
{
    u8 *r;

    if (func_800C8C1C(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1C03);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF14);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0xFFFFFF, 0x40,
                  0);
    func_800A56E0(0x612);

    return 1;
}

/* func_800CD1A4 with func_800C8844, the miss gate on the record's +0x14
   0x4000 flag instead of its +0x13 byte, a 1 in the sixth argument and no
   sound. 60 words, 0 differing. */
s32 func_800CD384(u8 *p)
{
    u8 *r;

    if (func_800C8844(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x8) == 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_800A6508();
        }
        return 1;
    }
    func_80099844(p, D_800E1C14);
    if (p[0x13] == 0)
    {
        func_800DC1B8(D_800DCF1C);
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802080, 0x20,
                  1);

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD474);

/* func_800CD1A4's charge roll on its own, result dropped. The record never
   enters a register of its own -- it is the incoming $a0 passed straight
   through. 13 words, 0 differing. */
void func_800CD6AC(u8 *p)
{
    func_800C8900(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10);
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD6E0);

/* 33 words, 0 differing. */
void func_800CD910(u8 *p)
{
    u8 *g;

    if (--*(s16 *)(p + 0x6) <= 0)
    {
        func_800CD994(*(s32 *)p, 0x10);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD994);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDA70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDC18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDD0C);

/* 13 words, 0 differing. */
void func_800CDE0C(void)
{
    u8 *p;
    u8 *h;

    p = D_800E3D7C;
    h = p;
    do
    {
        *(s32 *)(p + 0x1C) |= 0x40000000;
        p = *(u8 **)(p + 0x5C) + 0x20;
    } while (p != h);
}

/* Both tables in one pass: add to the counter of every live slot standing on
   the given tile. A leaf -- no frame, no $ra -- so the two parameters are
   re-sign-extended for the second loop out of $t1/$t2, which is what the
   second `sll`/`sra` pair is. func_800CDF40 is this source with `=` for
   `+=`. 64 words, 0 differing. */
void func_800CDE40(s16 x, s16 y, u16 v)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            D_800E36C8[i].unk_04 += v;
        }
    }
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            D_800E39C8[i].unk_12 += v;
        }
    }
}

/* func_800CDE40 with the counter SET rather than added to.
   58 words, 0 differing. */
void func_800CDF40(s16 x, s16 y, u16 v)
{
    s32 i;

    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            D_800E36C8[i].unk_04 = v;
        }
    }
    for (i = 0; i < 0x20; i++)
    {
        if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x && D_800E39C8[i].unk_07 == y)
        {
            D_800E39C8[i].unk_12 = v;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE028);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE4E8);

/* 21 words, 0 differing. */
s32 func_800CE69C(u8 *p)
{
    u8 *e;

    if (D_800E296C & 0xC0000000)
    {
        return 1;
    }
    e = *(u8 **)(p - 0x14);
    return func_800CE4E8(e[0x24], e[0x25], *(s16 *)(p + 0x88), p, 0) != 0;
}

/* THE `return 1` GUARD IS WRITTEN FIRST AND IS LAID OUT LAST. The other way
   round gcc emits the `return 1` as the branch's fall-through and pushes the
   body behind it: 22 words, 11 differing, every body instruction correct but
   two words late. 22 words, 0 differing. */
s32 func_800CE6F0(u8 *p)
{
    u8 *e;

    if (D_800E296C & 0xC0000000)
    {
        return 1;
    }
    e = *(u8 **)(p - 0x14);
    return func_800CE4E8(e[0x24], e[0x25], *(s16 *)(p + 0x88), p, 1) != 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE748);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CEA44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CEE0C);

/* Three stubs that return zero and nothing else. */
s32 func_800CEEF4(void)
{
    return 0;
}

/* Variant C: a five-argument visitor, the fifth a constant 0 passed on the
   stack, which is the whole reason this one's frame is 0x20 rather than
   0x18. 22 words, 0 differing. */
s32 func_800CEEFC(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800CEFB8(p, v, s, s->unk_06, 0);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

/* Variant C of the node walk again -- see func_800CEEFC above -- with the
   fifth argument 1 rather than 0. The constant has to live across the call,
   so this one saves $s1 and its frame is 0x28: 25 words against that one's
   22. 25 words, 0 differing. */
s32 func_800CEF54(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800CEFB8(p, v, s, s->unk_06, 1);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CEFB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CF8E4);

s32 func_800D0774(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800D07C8(p, v, s, s->unk_06);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D07C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D0DE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D112C);

/* Clear bit 0x80 of each listed cell's +0x04 halfword and empty the list.
   The cell address is the same `x + (y << D_8008333C[10])` index into the
   stride-6 map that func_8009A350 computes. The `while` with its test at the
   top is what duplicates the `lbu 0x0($a1)` into the entry path. 32 words,
   0 differing. */
void func_800D1338(void)
{
    s16 *sh;
    u8 *p;

    p = (u8 *)D_80081508;
    sh = (s16 *)D_8008333C;
    while (p[0] != 0)
    {
        D_800EA000[p[0] + (p[1] << sh[10])].unk_04 &= 0xFF7F;
        p += 4;
    }
    D_80080B04 = 0;
    *(u8 *)D_80081508 = 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D13B8);

/* 21 words, 0 differing. */
void func_800D1648(void)
{
    UnkNode *n;

    n = func_8003FC64(0);
    func_8004491C(n, (u32)func_800D1824);
    D_80081508 = (u32)((u8 *)n + 0x20);
    *((u8 *)n + 0x20) = 0;
    D_80080B04 = 0;
}

/* 48 words, 0 differing. D_80080B04 is read with `lh` for the bound and the
   index and with `lhu` for the `++`, off TWO separate `lui`s -- cse restarts
   at the if/else join, so the second materialisation is free rather than a
   second local. */
void func_800D169C(u16 x, u16 y, s16 v, s16 flag)
{
    u8 *e;

    if (D_80080B04 >= 0x40)
    {
        return;
    }
    if ((s16)func_800448BC(x, y) != 0)
    {
        return;
    }
    e = (u8 *)D_80081508 + D_80080B04 * 4;
    e[0] = x;
    e[1] = y;
    if (flag != 0)
    {
        *(s16 *)(e + 2) = v;
    }
    else
    {
        *(s16 *)(e + 2) = -0x8000;
    }
    D_80080B04++;
    e[4] = 0;
}

/* The vertical offset of the tile at (x, y).

   ONE BASE FOR BOTH ENGINE GLOBALS. D_80083178 is reached as
   `(u8 *)D_8008333C - 0x1C4`, which is the single `lui`/`addiu` plus one
   `addiu` the target has, and the map-width shift at D_80083350 is then the
   +0x14 displacement off the same register. Naming D_80083350 and
   &D_80083178 separately is the same 50 words with two `lui`s: 17 differing.

   AND THE VALUE THE 0x3F IS ADDED TO MUST BE ONE VARIABLE SPANNING BOTH ARMS.
   `d += 0x3F` in place is 4 differing; a separate sum variable with the else
   arm's load written inline is 2, and a named local for the load does not
   help because gcc ties the arithmetic result to its dying source. Assigning
   the load back into `d` -- the pseudo that already crosses the branch --
   unties it. 50 words, 0 differing. */
s16 func_800D175C(s16 x, s16 y)
{
    s32 s;
    u16 w;
    s32 d;
    s32 e;
    u8 *g;
    u8 *q;

    g = (u8 *)D_8008333C;
    q = g - 0x1C4;
    s = *(s16 *)(g + 0x14);
    w = D_800EA000_s[(y << s) + x].unk_02;
    d = func_800BCB04(*(u16 *)(q + 0xA4), *(u16 *)(q + 0xA6), (s16)(*(u16 *)(q + 0xA8) - 0x20));
    if ((s16)d < 0x201)
    {
        e = d + 0x3F;
    }
    else
    {
        d = *(u16 *)(D_800814A8 + 0x88);
        e = d + 0x3F;
    }
    return ((w + e) & ~0x3F) - w;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1824);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1A80);

/* THE CLUSTER'S SPAWNER, and the third shape in the set: func_800D24A8 and
   func_800D3F74 below are this source with the template, the scale and the
   two hooks substituted. Allocate a 0x112 node out of D_80083498, tag it
   0x34, take the engine's D_80045340 list, seed the mover's facing and the
   renderer's colour, scale and tile, park func_800D1E34 as the node's step
   and func_800D20D8 in the record's +0x8C slot, then measure the leash with
   func_800BCB04 and light the record up. Returns the node's payload, or
   NULL when the allocation fails -- one exit, with `p` preset to NULL, which
   is what the early `addu $s2, $zero, $zero` is.

   ALL FOUR PARAMETERS ARE s16 AND THAT IS WORTH ELEVEN WORDS. Only `kind`
   is witnessed directly -- `sll $v0, $s0, 16` / `sra` / `bne $v0, 1` is a
   narrow parameter re-extended, which an `s32` would not need. The other
   three are invisible in their own instructions: `sb $s6, 0x24($s0)` and
   `sh $s4, 0xA($s1)` truncate whatever they are given. They show up in the
   PROLOGUE instead. As `s32`, the tile pair's two allocnos come out at
   2 refs / 72 and 74 insns, the x pseudo wins `allocno_compare` by 3%, the
   two take each other's callee-saved registers, and the whole save-and-copy
   sequence reschedules around them: 13 differing words, none of them in the
   body. As `s16` they land the right way round and the function is 0. A
   parameter whose only use is a narrowing store still has a declared type,
   and the prologue is where you read it.

   113 words, 0 differing. */
u8 *func_800D1C70(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1800;
        r->unk_1C = 0x1800;
        func_8003DB94(r, D_800E2178, 0);
        n->unk_10 = (u32)func_800D1E34;
        *(u32 *)(p + 0x8C) = (u32)func_800D20D8;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* THE CLUSTER'S PER-FRAME STEP DRIVER, and the entry point the four machines
   above are reached through. Three copies: func_800D2E9C is this source with
   D_800E2284 for D_800E21C0 and nothing else changed, and func_800D2664 is
   it again with D_800E2228 and WITHOUT the func_800478B8 guard -- 160 words
   against 169.

   A frame for one entity: bail out with the record's +0x71 top bit cleared
   while the engine's global 0x2000 flag is up; run the entity's own +0x8C
   hook if it has one and then its state handler out of the table, both with
   the same four arguments the installed machines take; integrate the mover
   (+0x00 += +0x0C, +0x04 += +0x10) and its +0x14 accumulator; then decide
   whether the entity is still inside its leash, which is what the two
   func_800BCB04 calls measure -- the first from the mover's own position,
   the second from the tile centre once the +0x40000000 request bit is set.
   Out of range, or with +0x98 bit 2 up, the +0x8000000 bit is dropped.

   THE RECORD IS REACHED THROUGH TWO POINTERS and this is not a cosmetic
   choice: `addu $s0, $s1, $zero` makes a second pseudo for the same address,
   and the fields are split between them -- +0x1C, +0x71, +0x88, +0x8C and
   +0x9A on the copy, +0x90, +0x92, +0x98 and +0x9D on the parameter. Both
   survive to allocation and both take a callee-saved register. The copy is
   also what the handler calls pass as their FIRST and FOURTH arguments, which
   is why func_800D21CC above takes the same record twice.

   The two tests before func_800478B8 are NESTED `if`s, not `&&`. Written with
   `&&`, fold merges `(x & 0x8000) == 0 && (x & 0x40) == 0` into a single
   `andi $v0, $v0, 0x8040` and the function comes out two words short; the
   target keeps one `lhu` and two `andi`/`bnez` pairs, which is what two
   statements give.

   169 words, 0 differing. */
void func_800D1E34(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E21C0[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* THREE BYTE-IDENTICAL COPIES -- func_800D28E4 and func_800D3140 differ from
   this one in nothing but their label -- and the handler the four machines
   above park in the actor's +0x8C slot once their glide finishes.

   It runs the record's +0x6D countdown down: while it is positive, clear
   +0x60, let func_800A9A0C act on the record, and if the counter started
   non-zero and has since been zeroed, decrement the +0x28 population byte.
   When that reaches zero the record is published at D_80083470 (biased by
   -0x20 to the node, the same bias the walk family uses) and handed to
   func_800AAA54; otherwise, if +0x64 is non-zero, to func_800AA6B4.

   THE STORE COMES FIRST AND THE COUNTER IS READ SECOND, and the order is
   worth four words. Written the other way round -- the counter read, then
   the store -- cse still holds the `lb $v0, 0x6D($s1)` the guard above
   loaded, combine folds the `lbu`/`sll 24`/`sra 24` triple back into that
   one `lb`, and the function comes out 57 words. With the store between
   them the load is reissued as the target has it. `s16 n = (s8)d[0x6D]` is
   both casts: the `(s8)` is the `sll 24`/`sra 24` pair and the `s16`
   declaration is the `sll 16` before the `beqz`, which is how an HImode
   compare against zero is spelled. Five statement orders measured; only
   this one reaches 0, and the nearest two sit at 4 differing.

   61 words each, 0 differing. */
void func_800D20D8(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* FOUR IDENTICAL COPIES, one per entity, differing in exactly one symbol:
   the handler parked in the actor's +0x8C slot at the end -- func_800D20D8,
   func_800D28E4, func_800D3140 and func_800D4494 respectively. Every other
   word of all four listings is the same.

   The machine is a step hook, switched on the actor's +0x9B state. State 0
   seeds the mover's two velocity words from the engine's sine/cosine tables
   at the entity's facing (+0x6A, top three bits -- the same index expression
   csrc/ovl_town.c uses) and either arms an eight-step glide or, when the
   record's +0x14 bit 15 is set, jumps straight to state 2 with a zero count.
   State 1 bleeds a quarter off each velocity per step. State 2 steers the
   mover towards the record's +0x24/+0x25 tile centre, dividing the remaining
   distance by the steps left, and on the last step snaps it there, clears
   the engine's +0x10 slot tag if it points at this node, and hands the
   actor to its next handler.

   Three spellings to note:

   - `case 0` FALLS THROUGH into `case 1`, and its `d[0x28] == 0` arm reaches
     case 1's tail with a `goto`. Both are in the listing: the fallthrough is
     the absent `j` after `sh $v0, 0x96($s0)`, and the `goto` is
     `beqz $v0, .L800D2334` branching from case 0 into the middle of case 1.
   - `d` is the node PAYLOAD pointer, not the node: the engine's slot holds
     `d - 0x20`, which is the same +0x20 bias the walk family uses.
   - D_80083460 is reached through a `u8 *` local, which is what gives the
     split `lui`/`addiu` and a `lw`/`sw` pair off one register rather than a
     `%lo` displacement; csrc/unk_154BC.c spells the same object the same way.

   TWO THINGS COST THE WHOLE SESSION'S MEASUREMENT, and both are now levers in
   docs/matching-levers.md.

   The `do { } while (0)` around the last store is NOT decoration. `a` and `b`
   are both live across calls, so both are global-alloc allocnos, and
   `allocno_compare`'s priority put them 2% apart the wrong way:
   `a` = 19 refs / 226 insns = 0.3363 against `b` = 16 / 186 = 0.3441, so `b`
   sorted first and took $s0. That is 36 of the 40 differing words -- every
   `$s0`/`$s1` in the function swapped -- at the exact word count. The
   degenerate loop weights the reference inside it by two, `a` reaches 20
   refs and 0.3540, and the swap goes away. Solve for the count before
   spelling anything: `floor_log2(R) * R / 226 > 0.3441` first holds at
   R = 20, so exactly ONE extra weighted reference is wanted and two would be
   no better. Eleven structural rewrites -- nested tails, split decrements,
   `u8 *` parameters in place of the typed ones, local aliases, reordered
   stores -- all left both numbers at 19/226 and 16/186 exactly, which is
   what sent this to the compiler's own dumps.

   The first division is in THREE statements and the second in two, and the
   asymmetry is measured, not an accident. Written as one expression,
   `X - (b02 - 0x20)`, fold's `associate` (fold-const.c:4183, reached through
   `split_tree` on the second operand) moves the constant to the other side
   and emits `addiu $v0, $v0, 32` where the target has `addiu $v1, $v1,
   -0x20`: one word. Written with the difference in its own local, fold is
   defeated but sched1 then emits the b-side load first and the pair at
   0x800D23A4 comes out swapped: two words. Only `goal -= now;` as its own
   statement gets both -- it keeps the constant on the b-term AND puts the
   `subu` in a statement of its own, which is what orders the two loads.
   Giving the second division the same three-statement shape costs a word
   (167 and 54 differing), and ten other spellings of the first were measured
   in between.

   166 words each, 0 differing. */
void func_800D21CC(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        a->unk_8C = func_800D20D8;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* One gate, two arms, twice over: the same source compiled at two
   addresses -- the only word that differs between the pair is the `j` to its
   own epilogue. */
void func_800D2464(void *a, void *b, u8 *c)
{
    if (func_800D1A80() != 0)
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0;
    }
}

/* func_800D1C70 with D_800E221C stored straight into the renderer's +0x08
   instead of handed to func_8003DB94, a 0x1400 scale, and func_800D2664 /
   func_800D28E4 as the two hooks. 111 words, 0 differing. */
u8 *func_800D24A8(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        r->unk_08 = (u32)D_800E221C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1400;
        r->unk_1C = 0x1400;
        n->unk_10 = (u32)func_800D2664;
        *(u32 *)(p + 0x8C) = (u32)func_800D28E4;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E2228 and no func_800478B8 guard -- the entity
   this one drives is never handed to it. Same source otherwise, and the
   nine words it loses are why the two parameters land in the other order:
   fewer references to the slot record moves it past the mover in
   `allocno_compare`. 160 words, 0 differing. */
void func_800D2664(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E2228[a[0x9A]](a, b, c, a);
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 again, byte for byte. */
void func_800D28E4(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D29D8(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        a->unk_8C = func_800D28E4;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

void func_800D2C70(void *a, void *b, u8 *c)
{
    if (func_800D1A80() != 0)
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0;
    }
}

/* The cluster's FOURTH spawner, and the one that shows the list tag is a
   FUNCTION: func_8004491C takes func_800D36D0 where the other three pass
   D_80045340, so this entity's nodes are walked by the guarded walk below
   rather than by the engine's own list. Otherwise func_800D1C70 with the
   D_800DEF08 template, no colour bytes, a different scale pair, three more
   renderer flags, func_800D2E9C / func_800D3140 as hooks, a plain 0x808080
   colour word and a zeroed +0x9E. 122 words, 0 differing. */
u8 *func_800D2CB4(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)func_800D36D0);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        func_8003DB94(r, D_800DEF08, 0);
        r->unk_12 = 0x7E02;
        r->unk_1C = 0x1800;
        r->unk_1E = 0x2000;
        r->unk_14 |= 0xC;
        r->unk_10 |= 0x20;
        r->unk_14 |= 0x100;
        n->unk_10 = (u32)func_800D2E9C;
        *(u32 *)(p + 0x8C) = (u32)func_800D3140;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        *(s16 *)(p + 0x88) =
            func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(u32 *)((u8 *)r + 0xC) = 0x808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(s16 *)(p + 0x9E) = 0;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E2284 for D_800E21C0; see it for the whole
   mechanism. 169 words, 0 differing. */
void func_800D2E9C(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E2284[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 again, byte for byte. */
void func_800D3140(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D3234(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        a->unk_8C = func_800D3140;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* A two-state fade-out: state 0 raises the record's 0x10000000 bit, asks
   func_800A56E0 for sound 0x805 and arms a sixteen-step ramp, and state 1
   walks the renderer's three colour bytes towards 0x20 and its two scales
   towards zero, a step's worth of the remaining distance each frame. On
   the last step -- or at once if the renderer's 0x8000 flag is up -- it
   releases the engine's +0x10 slot, retires the record and rings
   func_800A56C0.

   The 0x20 the three colour terms are measured against is materialised
   ONCE, in the dispatch's `beq` delay slot, and again at the end of state
   0 -- one constant reaching the fallthrough and the direct entry. State 0
   falls through into state 1, as in func_800D21CC. 129 words, 0
   differing. */
void func_800D34CC(UnkDungeonActor *a, UnkVec *b, UnkSlot *r, u8 *d)
{
    u8 *g;

    switch (a->unk_9B)
    {
    case 0:
        if (D_8008346A != 0)
        {
            return;
        }
        *(u32 *)(d + 0x1C) |= 0x10000000;
        func_800A56E0(0x805);
        *(u32 *)((u8 *)r + 0xC) = 0x808080;
        a->unk_96 = 0x10;
        a->unk_9B++;
        /* fallthrough */
    case 1:
        *((u8 *)r + 0xC) += (0x20 - *((u8 *)r + 0xC)) / a->unk_96;
        *((u8 *)r + 0xD) += (0x20 - *((u8 *)r + 0xD)) / a->unk_96;
        *((u8 *)r + 0xE) += (0x20 - *((u8 *)r + 0xE)) / a->unk_96;
        r->unk_1C -= r->unk_1C / a->unk_96;
        r->unk_1E -= r->unk_1E / a->unk_96;
        if (--a->unk_96 > 0 && !(r->unk_14 & 0x8000))
        {
            return;
        }
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A32A4(d);
        if ((s16)func_80042900((s8 *)d, 0x1B) == 0)
        {
            func_8009A3D0(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                          (*(u32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        func_8009A028(d);
        *(u16 *)(d - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
        func_800A56C0();
        return;
    }
}

/* The node walk func_800D2CB4 hands to func_8004491C as its list tag: the
   same four-member walk as func_800BC054 above, with the visitor skipped
   for any node whose renderer carries the 0x80 flag. 26 words, 0
   differing. */
s32 func_800D36D0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        if (!(s->unk_14 & 0x80))
        {
            func_800D3738(p, v, s, s->unk_06);
        }
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D3738);

/* func_800D1C70 with the D_800DEEC0 template, a 0x1000 scale,
   func_800D4158 / func_800D4494 as the hooks, two more renderer fields set
   (+0x12 and the 0x100 flag), and a tail the other two do not have: the
   leash measurement is kept in a local so the record's +0x92 can be seeded
   with `ang - v` after +0x88 takes `v` itself. 121 words, 0 differing. */
u8 *func_800D3F74(s16 kind, s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    s32 v;

    p = NULL;
    n = func_8003FD64(0x112, D_80083498);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        p[0x13] = 0x34;
        if (kind == 1)
        {
            *(u32 *)(p + 0x14) |= 0x2000;
            *(u32 *)(p + 0x1C) |= 0x2000;
        }
        func_8004491C(n, (u32)D_80045340);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        *((u8 *)r + 0x24) = x;
        *((u8 *)r + 0x25) = y;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_12 = 0x7E40;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DEEC0, 0);
        n->unk_10 = (u32)func_800D4158;
        *(u32 *)(p + 0x8C) = (u32)func_800D4494;
        *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
        func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
        v = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6), *(s16 *)((u8 *)m + 0xA));
        *(s16 *)(p + 0x88) = v;
        *(s32 *)(p + 0x90) = 0;
        *(s16 *)(p + 0x92) = ang - v;
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_14 |= 0x8000;
        func_80099FDC(n);
        func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                      (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        p[0x9A] = 0xE;
        *(s8 *)(p + 0x9C) = -1;
        *(u32 *)(p + 0x1C) |= 0x40000200;
    }
    return p;
}

/* func_800D1E34 with D_800E22E0 and a 38-word tail of its own: a +0x9E
   counter stepped modulo 96, and a brightness ramp over its first 25 steps
   written into the renderer's three colour bytes.

   The modulo is cc1psx's signed-by-constant sequence and comes out of a
   plain `% 96` -- `mult` by 0x2AAAAAAB, `sra 4`, minus the sign, then
   `q*3 << 5` subtracted back off. `t` is s16: the `sll 16`/`sra 16` ahead
   of the `mult` is the dividend being narrowed, and the second pair after
   the `sh` is the same value re-extended for `t < 0x19`. The argument is
   `t * 85`, which synth_mult splits into `t*5` and `(t*5) << 4` added --
   two shifts and two adds, not a `mult`.
   207 words, 0 differing. */
void func_800D4158(u8 *p, UnkVec *b, u8 *c)
{
    u8 *a;
    s32 r;
    s16 t;

    a = p;
    if (D_80083462 & 0x2000)
    {
        a[0x71] &= 0x7F;
        return;
    }
    if (*(UnkDungeonStep *)(a + 0x8C) != NULL)
    {
        (*(UnkDungeonStep *)(a + 0x8C))(a, b, c, a);
    }
    D_800E22E0[a[0x9A]](a, b, c, a);
    if (!(*(u16 *)(c + 0x14) & 0x8000))
    {
        if (!(*(u16 *)(c + 0x14) & 0x40))
        {
            func_800478B8(c);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    if (*(u16 *)(p + 0x98) & 0x8)
    {
        p[0x9D] = 0;
    }
    else
    {
        b->unk_14 += (*(s8 *)(p + 0x9D) * 5) << 14;
        p[0x9D]++;
    }
    *(s32 *)(p + 0x90) += b->unk_14;
    if (*(u16 *)(p + 0x98) & 0x4)
    {
        goto drop;
    }
    r = func_800BCB04(*(u16 *)((u8 *)b + 0x2), *(u16 *)((u8 *)b + 0x6),
                      (s16)(*(u16 *)(a + 0x88) - 0x20));
    if ((s16)r >= 0x200)
    {
        goto drop;
    }
    if (*(s16 *)(p + 0x92) + *(s16 *)(a + 0x88) < (s16)r)
    {
        /* A BARE VOLATILE READ, and the listing leaves no other reading: this
           arm is four words -- `lhu $v0, 0x98($s1)`, `j`, its empty delay
           slot, and the `beqz` that branches over it -- and the load's
           destination is dead at the jump. An ordinary read here is deleted
           by flow (`insn_dead_p` keeps only a set whose source has side
           effects), an empty arm collapses the if/else into one branch, and
           both come out three words short. */
        *(volatile u16 *)(p + 0x98);
    }
    else
    {
        if ((s16)r >= *(s16 *)(a + 0x88))
        {
            *(s32 *)(p + 0x90) = 0;
        }
        else
        {
            *(s16 *)(p + 0x92) = r - *(u16 *)(a + 0x88);
        }
        b->unk_14 = 0;
        *(u32 *)(a + 0x1C) |= 0x8000000;
        p[0x9D] = 0;
    }
    if (*(u32 *)(a + 0x1C) & 0x40000000)
    {
        *(u32 *)(a + 0x1C) &= ~0x40000000;
        r = func_800BCB04((c[0x24] << 6) | 0x20, (c[0x25] << 6) | 0x20,
                          (s16)(*(u16 *)(a + 0x88) - 0x20));
        *(s16 *)(p + 0x92) += *(u16 *)(a + 0x88) - r;
        *(u16 *)(a + 0x88) = r;
    }
    goto tail;
drop:
    *(u32 *)(a + 0x1C) &= ~0x8000000;
tail:
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(a + 0x88) + *(u16 *)(p + 0x92);
    t = (s16)(*(u16 *)(p + 0x9E) + 1) % 96;
    *(s16 *)(p + 0x9E) = t;
    if (t < 0x19)
    {
        s32 w = ((func_800644B8(t * 85) >> 4) * 96 >> 8) + 0x80;

        c[0xE] = w;
        c[0xD] = w;
        c[0xC] = w;
    }
    else
    {
        c[0xE] = 0x80;
        c[0xD] = 0x80;
        c[0xC] = 0x80;
    }
    *(u16 *)(c + 0x14) |= 0x40;
    *(u32 *)(a + 0x1C) |= 0x200;
}

/* func_800D20D8 with a guard ahead of it: while the engine's 0x2000 flag is
   down, a record carrying the 0x100 bit is handed to func_800AA258 and the
   frame ends there, and otherwise the actor's state byte is forced to 0xE.
   The 0xE is materialised once and used for both the compare and the store,
   which is the `addiu $v1, $zero, 0xE` in the `beqz` delay slot.
   79 words, 0 differing. */
void func_800D4494(u8 *a, UnkVec *b, u8 *c, u8 *d)
{
    s16 n;

    if (!(D_80083462 & 0x2000))
    {
        if (*(u32 *)(d + 0x1C) & 0x100)
        {
            func_800AA258();
            return;
        }
        if (a[0x9A] != 0xE)
        {
            a[0x9A] = 0xE;
        }
    }
    if (*(s8 *)(d + 0x6D) > 0)
    {
        *(s32 *)(d + 0x60) = 0;
        n = (s8)d[0x6D];
        func_800A9A0C(d);
        if (n != 0)
        {
            if (*(s8 *)(d + 0x6D) == 0)
            {
                d[0x28]--;
            }
        }
        if (d[0x28] == 0)
        {
            D_80083470 = d - 0x20;
            func_800AAA54((UnkDungeonActor *)a, b, c, 0);
            return;
        }
    }
    if (*(s16 *)(d + 0x64) != 0)
    {
        func_800AA6B4(a, b, c, 0);
    }
}

/* Four two-gate installers: the same park as the ten above behind a second
   test, whose result is read as a halfword. The second parameter is unused --
   no instruction in any of the four mentions $a1 -- and the third and fourth
   are what the inner gate is handed. 25 words each, 0 differing. */
void func_800D45D0(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D4494;
        }
    }
}

/* Four identical copies of one four-state walk-and-stop machine, differing
   only in the handler they park at the end. See func_800D21CC. */
void func_800D4634(UnkDungeonActor *a, UnkVec *b, u8 *c, u8 *d)
{
    u8 *g;
    s32 goal;
    s32 now;

    switch (a->unk_9B)
    {
    case 0:
        func_800AD4D0(d);
        b->unk_0C = D_8006CCD8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        b->unk_10 = D_8006CCE8[(*(u16 *)(d + 0x6A) >> 9) & 7] << 15;
        a->unk_9B++;
        if (d[0x28] != 0)
        {
            if (*(u16 *)(c + 0x14) & 0x8000)
            {
                a->unk_96 = 0;
                a->unk_9B = 2;
                return;
            }
            a->unk_96 = 8;
        }
        else
        {
            goto stop;
        }
        /* fallthrough */
    case 1:
        b->unk_0C -= b->unk_0C / 4;
        b->unk_10 -= b->unk_10 / 4;
        if (--a->unk_96 > 0)
        {
            return;
        }
        if (d[0x28] == 0)
        {
        stop:
            b->unk_14 = 0;
            b->unk_10 = 0;
            b->unk_0C = 0;
            func_800AAA54(a, b, c, 0);
        }
        else
        {
            a->unk_96 = 8;
            a->unk_9B++;
        }
        return;
    case 2:
        if (a->unk_96 != 0)
        {
            goal = c[0x24] << 6;
            now = *(s16 *)((u8 *)b + 0x2) - 0x20;
            goal -= now;
            b->unk_0C = (goal << 15) / a->unk_96;
            now = *(s16 *)((u8 *)b + 0x6) - 0x20;
            b->unk_10 = (((c[0x25] << 6) - now) << 15) / a->unk_96;
        }
        if (--a->unk_96 > 0)
        {
            return;
        }
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        a->unk_8C = func_800D4494;
        do
        {
            a->unk_9A = 0xE;
        } while (0);
        return;
    }
}

/* 20 words, 0 differing. */
void func_800D48CC(u8 *a, void *b, u8 *c)
{
    if (func_800D1A80())
    {
        func_800A56C0();
    }
    else
    {
        *(s16 *)(c + 0x12) = 0x7E40;
        *(u16 *)(c + 0x14) |= 0x100;
    }
}

/* Three more installers, with a `blez` gate instead of the `beqz` one: the
   predicate's result is tested as positive, not merely non-zero. */
void func_800D491C(UnkDungeonActor *p)
{
    if (func_800AB464() > 0)
    {
        p->unk_8C = func_800D4494;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4954);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4AB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4BD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4CB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4DE8);

/* The effect spawner the kill handlers call: allocate a 0x12 node, park a
   step in it, seed its mover from the caller's, tint it and ring a sound.
   func_800D5460 below is this source with func_800D5294 as the step and a
   0x32 life. See its `&colour` note. 77 words, 0 differing. */
void func_800D4FC8(UnkNode *src, s32 colour, u16 snd)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    u8 *g;

    /* `&colour` IS LOAD-BEARING and costs no code. The target keeps this
       parameter in its incoming home slot at 0x24($sp) -- stored once in the
       jal's delay slot, reloaded once -- and assign_parms (function.c:3775)
       gives a register parameter a MEM for its DECL_RTL at -O2 in exactly
       three cases: TREE_ADDRESSABLE, TREE_SIDE_EFFECTS, or a BLKmode type.
       A struct is ruled out by the call sites, which pass integer constants
       with `lui`/`ori`, so the original took its address. Without this the
       parameter takes a third callee-saved register and pays the
       save/restore pair: 78 words. `volatile` reaches 77 with 3 differing --
       the prologue permuted -- and the address-take reaches 0. */
    (void)&colour;
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0x24) = src;
        n->unk_10 = (u32)func_800D4DE8;
        *(s16 *)(p + 0x1E) = 0x1E;
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src->unk_08 + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src->unk_08 + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src->unk_08 + 0xA);
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *(s32 *)(p + 0xC) = *(s32 *)((u8 *)r + 0xC) = colour;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
        if (snd != 0)
        {
            func_800A56E0(snd);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D50FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5294);

/* func_800D4FC8 with func_800D5294 and a 0x32 life. 77 words, 0 differing. */
void func_800D5460(UnkNode *src, s32 colour, u16 snd)
{
    UnkNode *n;
    u8 *p;
    UnkVec *m;
    UnkSlot *r;
    u8 *g;

    /* `&colour` IS LOAD-BEARING and costs no code. The target keeps this
       parameter in its incoming home slot at 0x24($sp) -- stored once in the
       jal's delay slot, reloaded once -- and assign_parms (function.c:3775)
       gives a register parameter a MEM for its DECL_RTL at -O2 in exactly
       three cases: TREE_ADDRESSABLE, TREE_SIDE_EFFECTS, or a BLKmode type.
       A struct is ruled out by the call sites, which pass integer constants
       with `lui`/`ori`, so the original took its address. Without this the
       parameter takes a third callee-saved register and pays the
       save/restore pair: 78 words. `volatile` reaches 77 with 3 differing --
       the prologue permuted -- and the address-take reaches 0. */
    (void)&colour;
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0x24) = src;
        n->unk_10 = (u32)func_800D5294;
        *(s16 *)(p + 0x1E) = 0x32;
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x20;
        r->unk_14 |= 0xC;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)src->unk_08 + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)src->unk_08 + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)src->unk_08 + 0xA);
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        *(s32 *)(p + 0xC) = *(s32 *)((u8 *)r + 0xC) = colour;
        r->unk_12 = 0x7DCE;
        r->unk_14 |= 0x100;
        func_8003DB94(r, D_800DECF8, 0);
        if (snd != 0)
        {
            func_800A56E0(snd);
        }
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5594);

/* A node's +0x10 handler, so three parameters off func_80040190's cast. The
   masked value is stored unconditionally -- the `sh` sits in the `beqz`'s
   delay slot -- which is the `&=` followed by a test of the same field.
   27 words, 0 differing. */
void func_800D586C(u8 *p, UnkVec *m, u8 *r)
{
    func_800478B8(r);
    *(u16 *)(r + 0x14) &= 0x6000;
    if (*(u16 *)(r + 0x14) != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D58D8);

/* One semi-transparent 1x1 tile and one DR_MODE per node of the tag chain,
   projected through the 0x1F800000 scratchpad. func_800D68F4 is this
   function again, byte for byte. The family is ovl_town's func_800BB218 --
   this walk with a 0x180-iteration inner loop and a constant colour -- and
   csrc/ovl_town_800BF3F0.c's func_800BFFF4 is the LINE_G2 version, which
   carries the long note on the shape.

   THE OT LINKS ARE `addPrim`, i.e. bitfield stores through P_TAG's
   `addr:24`; a hand-written `|` cannot produce the
   `lw`/`and 0xFF000000`/`and 0xFFFFFF`/`or`/`sw` quintets, and
   csrc/unk_19434.c:92-99 says why. The two masks are loop-hoisted into
   $s2/$s4.

   THE THREE DEAD `lbu`s AT +0x4/+0x5/+0x6 ARE `setRGB0(t, t->r0, t->g0,
   t->b0)` -- an echo of the colour word just copied in. cse deletes the
   three STORES as redundant, because it knows the word that was written,
   and the three loads survive tightly packed where the target has them.
   They are NOT bare `volatile` reads: that spelling does keep the loads,
   but gcc 2.7.2's `read_dependence` pairs volatile only with volatile
   (scratch/gcc-src/cdk-gcc-b18/sched.c:822), so they take priority 0, drift
   to wherever the ready list puts them, and let the first addPrim's
   `lw 0x0($s0)` hoist to the top of the block -- 120 words and 36
   differing, the right length and the wrong function. Omitting them is 117.

   `setTile1` plus `setSemiTrans(t, 1)` is ONE `sb 0x6A`: cse folds the
   `getcode` back to the 0x68 just stored and drops the dead first store.
   `setlen(t, 2)` plus `setcode(t, 0x6A)` is identical.

   Three orderings are load-bearing. The colour word is copied BEFORE the tag
   setup (6 differing the other way: with `setlen` first, $v0 carries both
   the constant and the loaded word and the `lw 0x8($s3)` cannot issue
   first). `sp->unk_1C` is written BEFORE `sp->unk_24` -- 20 differing
   swapped, and the whole $s3/$s4 split between `p` and the 0xFF000000 mask
   turns over -- even though the target STORES them in the other order. And
   `p = (u8 *)n + 0x20;` comes before `v = n->unk_08;`: 4 differing swapped.

   120 words, 0 differing. */
s32 func_800D5AC0(u8 *p, UnkVec *v)
{
    UnkDungeonSP *sp = (UnkDungeonSP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = func_80065420((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, func_80066460(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

/* One frame of a fading burst, and a leaf: integrate the mover, halve each
   of its three velocities, scale the three colour bytes by the remaining
   life over the original life, copy the colour word forward, and when the
   life runs out raise the node's and the engine's 0x8000 flags. The three
   `srl 31`/`addu`/`sra 1` triples are a signed divide by two, which is
   what `/= 2` gives; `>>= 1` would drop the correction.
   75 words, 0 differing. */
void func_800D5CA0(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5DCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5F80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6068);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6170);

s32 func_800D67B0(u8 *p, UnkVec *v, UnkSlot *s)
{
    UnkNode *n;

    for (;;)
    {
        func_800D6170(p, v, s, 0);
        n = *(UnkNode **)(p - 8);
        if (n == NULL)
        {
            return 0;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
        s = (UnkSlot *)n->unk_0C;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6804);

/* func_800D5AC0 again, byte for byte. 120 words, 0 differing. */
s32 func_800D68F4(u8 *p, UnkVec *v)
{
    UnkDungeonSP *sp = (UnkDungeonSP *)0x1F800000;
    Unk80083160 *g = &D_80083160;
    UnkNode *n;
    TILE_1 *t;
    DR_MODE *dm;

    sp->unk_1C = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    sp->unk_24 = D_80083160.unk_00 + 0xB0;
    while (1)
    {
        sp->unk_04 = *(u16 *)((u8 *)v + 0x2);
        sp->unk_06 = *(u16 *)((u8 *)v + 0x6);
        sp->unk_08 = *(u16 *)((u8 *)v + 0xA);
        t = (TILE_1 *)sp->unk_1C;
        sp->unk_1C = (u8 *)(t + 1);
        sp->unk_100 = func_80065420((SVECTOR *)&sp->unk_04, (s32 *)&t->x0, &sp->unk_D0, &sp->unk_D4);
        if ((u32)sp->unk_100 < 0x1E0)
        {
            *(u32 *)&t->r0 = *(u32 *)(p + 0x8);
            setTile1(t);
            setSemiTrans(t, 1);
            setRGB0(t, t->r0, t->g0, t->b0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, t);
            dm = (DR_MODE *)sp->unk_1C;
            sp->unk_1C = (u8 *)(dm + 1);
            func_80067F20((u32 *)dm, 0, 0, func_80066460(0, 1, 0, 0), 0);
            addPrim(sp->unk_24 + sp->unk_100 * 4, dm);
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
        v = n->unk_08;
    }
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)sp->unk_1C;
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6AD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6C18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6DC0);

/* Copy the three integer halves of the mover at +0xB0 into b, run the node
   call, and close on func_800999D4's countdown tail. 36 words, 0 differing. */
void func_800D6FEC(u8 *p, u8 *b, u8 *c)
{
    u8 *m;

    m = *(u8 **)(p + 0xB0);
    *(s16 *)(b + 0x2) = *(s16 *)(m + 0x2);
    *(s16 *)(b + 0x6) = *(s16 *)(m + 0x6);
    *(s16 *)(b + 0xA) = *(s16 *)(m + 0xA);
    func_800478B8(c);
    if (--*(s16 *)(p + 0x96) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D5CA0 again, byte for byte. 75 words, 0 differing. */
void func_800D707C(u8 *p, UnkVec *v)
{
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    v->unk_0C /= 2;
    v->unk_10 /= 2;
    v->unk_14 /= 2;
    p[4] = p[0] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[5] = p[1] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    p[6] = p[2] * *(s16 *)(p + 0x32) / *(s16 *)(p + 0x34);
    *(s32 *)(p + 0x8) = *(s32 *)(p + 0x4);
    if (--*(s16 *)(p + 0x32) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D71A8);

/* `&p[i * 0x30 + 0x8]` AND NOT A WALKED OFFSET. With `off = 8;` before the
   loop and `off += 0x30` in the body, the `addiu $s1, $zero, 0x8` is emitted
   ahead of the loop guard (5 differing); as a subscript the 8 is loop.c's
   giv initial value and lands in the preheader, after the guard, where the
   target has it. `p + i * 0x30 + 8` rather than `&p[...]` is 2 differing.
   40 words, 0 differing. */
void func_800D78C0(u8 *p)
{
    s32 i;

    if (**(u16 **)(p + 0x98) & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    for (i = 0; i < *(s16 *)(p + 0x2); i++)
    {
        func_800478B8(&p[i * 0x30 + 0x8]);
    }
}

/* 20 words, 0 differing. */
void func_800D7960(u8 *p, void *b, u8 *c)
{
    if (p[0x9B] == 0xFF)
    {
        *(u16 *)(p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_800478B8(c);
}

/* func_800D7960 with the renderer's 0x6000 bits as the gate.
   21 words, 0 differing. */
void func_800D79B0(u8 *p, void *b, u8 *c)
{
    if (*(u16 *)(c + 0x14) & 0x6000)
    {
        *(u16 *)(p - 2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_800478B8(c);
}

s32 func_800D7A04(void)
{
    return 0;
}

s32 func_800D7A0C(void)
{
    return 0;
}

/* Spawn a kind-2 node carrying the caller's record and park func_800D7A78 at
   its +0x10. THE NODE HAS TO BE HELD AS `u8 *`, not `UnkNode *`: through the
   struct pointer the install's `n->unk_10` is MEM_IN_STRUCT_P, sched1 lifts
   the handler's `lui` into the counter load's delay slot, the D_80083460
   base and the handler address stop sharing $v0, and the function comes out
   24 words against 25 with 14 differing. 25 words, 0 differing. */
void func_800D7A14(u8 *p)
{
    u8 *n;
    u8 *g;

    n = (u8 *)func_8003FD64(2, D_80083498);
    if (n != NULL)
    {
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
        *(u32 *)(n + 0x10) = (u32)func_800D7A78;
        *(u8 **)(n + 0x2C) = p;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7A78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7D30);

/* Bump the D_80083460 counter at +0xA, allocate a kind-2 node and seed its
   view from the three halfwords of D_80083780.

   D_80083780 NEEDS A POINTER LOCAL, assigned immediately before its first
   use: read as three raw `*(u16 *)(D_80083780 + N)` the first offset folds
   into the `%lo` addend and cse derives the other two with an extra
   `addiu $v1, $v1, 2`, which is 5 differing. 32 words, 0 differing. */
void func_800D7F38(void)
{
    UnkNode *n;
    u8 *g;
    u8 *p;
    u8 *b;

    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    n = func_8003FD64(2, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D7FB8;
        p = (u8 *)n + 0x20;
        b = D_80083780;
        *(s16 *)(p + 0x4) = *(u16 *)(b + 0x2);
        *(s16 *)(p + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)(p + 0x8) = *(u16 *)(b + 0xA) - 0x30;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D81D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8590);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8728);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8C64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D904C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D92C0);

/* State 0x10, and hand c the D_800E260C entry the facing at d+0x2A selects.
   The table base is a POINTER LOCAL because it is used twice -- stored at
   c+0x2C and indexed -- and the index is a byte offset (`andi 0x1C` after the
   shift), so it is added to the `u8 *` rather than written as a subscript.
   25 words, 0 differing. */
void func_800D9820(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    u8 *t;

    p[0x9A] = 0x10;
    p[0x9B] = 0;
    t = D_800E260C;
    *(u8 **)(c + 0x2C) = t;
    func_8003DB94(c, *(void **)(t + (((D_80083228 + *(s16 *)(d + 0x2A) + 0x100) >> 7) & 0x1C)), 0);
}

/* A two-gate installer of func_800D45D0's shape, except that the inner gate's
   failure RETURNS rather than falling into the D_80083462 test: the target's
   `blez` goes to the epilogue while the `beqz` of the first gate goes to the
   flag test. 38 words, 0 differing. */
void func_800D9884(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AB1C0(p, b, c, d) != 0)
    {
        func_800AD594(d, 4);
        func_800A4ACC(d);
        if ((s16)func_800AD9B4(c, d) <= 0)
        {
            return;
        }
        *(void **)(p + 0x8C) = func_800D8C64;
        *(s32 *)(p + 0x90) = 0;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)(p + 0x92) = 0;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D991C);

void func_800D9C18(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AB378() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D9C7C);

void func_800D9F7C(void)
{
    func_800AD058();
}

void func_800D9F9C(void)
{
    func_800AB538();
}

void func_800D9FBC(void)
{
    func_800AB778();
}

void func_800D9FDC(UnkDungeonActor *p)
{
    if (func_800ABDBC() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DA014);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DA398);

void func_800DA660(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AC82C() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

void func_800DA6C4(UnkDungeonActor *p, void *b, void *c, void *d)
{
    if (func_800AC010() != 0)
    {
        if ((s16)func_800AD9B4(c, d) > 0)
        {
            p->unk_8C = func_800D8C64;
        }
    }
}

void func_800DA728(UnkDungeonActor *p)
{
    if (func_800AC480() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA760(UnkDungeonActor *p)
{
    if (func_800AC5C8() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA798(UnkDungeonActor *p)
{
    if (func_800ACE34() != 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA7D0(UnkDungeonActor *p)
{
    if (func_800AB464() > 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

void func_800DA808(UnkDungeonActor *p)
{
    if (func_800AC7EC() > 0)
    {
        p->unk_8C = func_800D8C64;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DA840);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DA93C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DAB50);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DAE44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DAEF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB164);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB2DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB420);

/* 18 words, 0 differing. */
void func_800DB618(u8 *p)
{
    if (*(s32 *)(p + 0x18) >= *(s32 *)(p + 0x1C))
    {
        *(s32 *)(p + 0x18) -= *(s32 *)(p + 0x1C);
    }
    else
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB660);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBA90);

/* Draw one digit strip: format two numbers into stack buffers through
   func_8004E57C, then walk the first string and blit each character with
   func_800673A0, the glyph's x chosen from the engine's per-character
   attribute table and stepping left by 4 each time.

   SIX PARAMETERS, AND THE LAST TWO ARE BOTH s16. The sixth arrives on the
   stack and is read with `lw` and then `sll 16`/`sra 16` -- a PROMOTED s16
   stack parameter, where a plain s32 gets rematerialised at its use and
   costs the function a word. The fifth is also s16 and the witness is the
   `sll`/`sra` where it is passed on; as u16 it is an `andi $a2, $s5,
   0xFFFF` instead, which is the same length and one differing word.

   THE WALKER IS WHAT IS PASSED TO THE FIRST func_8004E57C, not the array.
   Passing `t` and walking a separate `s` makes two pseudos and an
   `addu $s0, $s2, $zero` between them. 79 words, 0 differing. */
void func_800DBC20(s32 a, s32 b, u16 c, s16 x, s16 y, s16 n)
{
    UnkDungeonRect r;
    u8 t[0x10];
    u8 u[0x10];
    u8 *s;

    s = t;
    func_8004E57C(a, c, (s32)s);
    func_8004E57C(b, c, (s32)u);
    if (t[0] != 0)
    {
        do
        {
            if (n != 0)
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1C0;
            }
            else
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1E8;
            }
            r.v[1] = 0x1C0;
            r.v[2] = 0x4;
            r.v[3] = 0x8;
            func_800673A0(r.v, x, y);
            s++;
            x -= 4;
        } while (*s != 0);
    }
}

/* func_800DBC20 with the other glyph row: 0x1EC for the fallback, 0x1B0
   for the row and a height of 0x10. 79 words, 0 differing. */
void func_800DBD5C(s32 a, s32 b, u16 c, s16 x, s16 y, s16 n)
{
    UnkDungeonRect r;
    u8 t[0x10];
    u8 u[0x10];
    u8 *s;

    s = t;
    func_8004E57C(a, c, (s32)s);
    func_8004E57C(b, c, (s32)u);
    if (t[0] != 0)
    {
        do
        {
            if (n != 0)
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1C0;
            }
            else
            {
                r.v[0] = (D_800808ED[*s] & 4) ? (*s * 4 + 0x100) : 0x1EC;
            }
            r.v[1] = 0x1B0;
            r.v[2] = 0x4;
            r.v[3] = 0x10;
            func_800673A0(r.v, x, y);
            s++;
            x -= 4;
        } while (*s != 0);
    }
}

/* Hand one of the overlay's two stored rectangles to the engine. The copy
   to a local is in the source, not an artefact: func_800673A0 takes a
   writable `s16 *`. */
void func_800DBE98(void)
{
    UnkDungeonRect r = D_80089490;

    func_800673A0(r.v, 0x1C2, 0x19C);
}

void func_800DBEE8(void)
{
    UnkDungeonRect r = D_80089498;

    func_800673A0(r.v, 0x1C2, 0x19C);
}

/* Drop bit 0 of a flag word in the object D_800E5910 points at.

   THE BASE LOCAL IS LOAD-BEARING AND SO IS ITS SECOND USE. As one
   expression the two constants fold and the access becomes `lw 0x74`; split
   across a local, combine would still propagate the `addiu` into the memory
   address -- except that the local feeds BOTH the load and the store, and
   combine does not substitute a def with two uses. 9 words, 0 differing. */
void func_800DBF38(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    *(s32 *)(p + 0x54) &= ~1;
}

/* func_800DBF38's object, gated. 14 words, 0 differing. */
void func_800DBF5C(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    if (*(s32 *)(p + 0x5C) == 0)
    {
        *(s32 *)(p + 0x54) |= 1;
    }
}

/* 27 words, 0 differing. */
void func_800DBF94(u8 *p)
{
    u8 *g;

    g = (u8 *)D_80081468;
    if (*(s16 *)(g + 0x4) != *(s32 *)p)
    {
        func_800DBD5C(*(s16 *)(g + 0x4), *(s32 *)p, 2, 0x18C, 0x1B8, 0);
        *(s32 *)p = *(s16 *)(g + 0x4);
    }
}

/* 30 words, 0 differing. */
void func_800DC000(u8 *p)
{
    if (*(s32 *)(D_800814A8 + 0xFC) != *(s32 *)(p + 0x4))
    {
        func_800DBD5C(*(s32 *)(D_800814A8 + 0xFC), *(s32 *)(p + 0x4), 8, 0x1A8, 0x1E0, 0);
        *(s32 *)(p + 0x4) = *(s32 *)(D_800814A8 + 0xFC);
    }
}

/* The `srl 31`/`addu`/`sra 1` trio is the signed `/ 2`.
   15 words, 0 differing. */
void func_800DC078(u8 *p, s16 v)
{
    s32 n;

    if (v < 0x80)
    {
        n = v / 2 + 1;
    }
    else
    {
        n = 0x40;
    }
    p[0x2] = 0x57 - n;
    p[0xA] = n;
}

void func_800DC0B4(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC0BC);

/* `u32 t` for the `sltiu`, and `-(s32)t / 2 - 1` for the signed halving.
   20 words, 0 differing. */
void func_800DC168(u8 *p, u8 *q)
{
    u32 t;
    u8 *r;
    s16 v;

    t = *(u8 *)(D_800814A8 + 0x29);
    if (t != *(s32 *)(p + 0x14))
    {
        r = *(u8 **)(q + 0x4);
        if (t < 0x80)
        {
            v = -(s32)t / 2 - 1;
        }
        else
        {
            v = -0x40;
        }
        *(s16 *)(r + 0x8) = v;
    }
}

/* 16 words, 0 differing. */
void func_800DC1B8(void *arg0)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    *(s32 *)(p + 0x18) = *(s32 *)(p + 0x18) % 32 + 0x20;
    *(void **)(p + 0x2C) = arg0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC1F8);

/* 29 words, 0 differing. */
void func_800DC5B4(u8 *p)
{
    if (*(u8 *)(D_800814A8 + 0x11) != *(s32 *)(p + 0x30))
    {
        func_800DBD5C(*(u8 *)(D_800814A8 + 0x11), *(s32 *)(p + 0x30), 2, 0x18A, 0x190, 0);
        *(s32 *)(p + 0x30) = *(u8 *)(D_800814A8 + 0x11);
    }
}

/* One half-extent into four fields of the record at +0x8 and its negation
   into a fifth.

   THE NODE IS RE-READ FOR THE LAST STORE. Dereferenced inline at all five
   the unit gets five `lw`s -- a halfword store through an unrelated pointer
   kills the equivalence every time -- and cached in one local it gets one;
   the target has exactly two. 10 words, 0 differing. */
void func_800DC628(u8 *p, s16 v)
{
    u8 *q;

    q = *(u8 **)(p + 0x8);
    *(s16 *)(q + 0x58) = v;
    *(s16 *)(q + 0x34) = v;
    *(s16 *)(q + 0x28) = v;
    *(s16 *)(q + 0xE) = v;
    q = *(u8 **)(p + 0x8);
    *(s16 *)(q + 0x1A) = -v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC650);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC724);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC82C);

/* func_800DBF38's object again, with the counter its +0x38 points at
   advanced modulo 3.

   THE TAIL CALL TAKES `p`, and that is the whole residue: func_800DC82C's
   own listing reads 0x38, 0x54 and 0x5C off $a0, and passing nothing is 28
   words / 13 differing with every difference a register name -- p lands in
   $v1 instead of the $a0 the argument ties it to, and the three values below
   it all shift down one. Six spellings of the two updates are all 13
   differing, so the allocation is not reachable from the statement form.
   28 words, 0 differing. */
void func_800DC888(void)
{
    u8 *p;

    p = D_800E5910 + 0x20;
    **(s32 **)(p + 0x38) += 1;
    **(s32 **)(p + 0x38) %= 3;
    func_800DC82C(p);
}

/* The per-frame tag pass: run D_800E2934[1..7] over the primitive buffer at
   +0x3C, 0x10 bytes a slot, then the fixed tail and whatever is parked at
   +0x4C. The first table entry is never called -- the loop starts at 1 and
   gcc drops the entry guard, which is why there is no test above the loop.
   36 words, 0 differing. */
void func_800DC8F8(u8 *p, UnkVec *b, u8 *c)
{
    s32 i;

    for (i = 1; i < 8; i++)
    {
        D_800E2934[i](p, *(u8 **)(p + 0x3C) + (i << 4));
    }
    func_800DC724(p);
    if (*(u32 *)(p + 0x4C) != 0)
    {
        (*(void (**)(u8 *))(p + 0x4C))(p);
    }
}

/* BINDING THE FIRST VALUE TO A LOCAL IS WHAT ORDERS THE TWO `lui`s. The
   target materialises %hi(D_8008146C) before %hi(D_800814A8); written
   `*(s32 *)(p + 0x20) = ~D_8008146C;` as a statement the two come out the
   other way round and the function is 37 words / 2 differing -- and no order
   of the statements or of the declarations moves it (four spellings,
   byte-identical output each time). Moving the store itself ahead of
   `r = (u8 *)D_800814A8;` is worse, 38 words: a store through `p` may alias
   the global, so the load has to be re-issued after it. 37 words,
   0 differing. */
void func_800DC988(u8 *p)
{
    u8 *q;
    u8 *r;
    s32 v;

    v = ~D_8008146C;
    r = (u8 *)D_800814A8;
    q = p + 0x20;
    *(s32 *)(p + 0x20) = v;
    *(s32 *)(q + 0x4) = ~*(s32 *)(r + 0xFC);
    *(s32 *)(q + 0x10) = ~r[0x28];
    *(s32 *)(q + 0x14) = ~r[0x29];
    *(s32 *)(q + 0x30) = ~r[0x11];
    func_800DC82C(q);
    do
    {
        func_800DC724(q);
    } while (*(s32 *)(q + 0x58) != 0);
}

/* Thread one of the two index lists into a backwards chain through each
   0x10-byte record's +0xC, and hand back the last record.

   THE CURSOR IS A SECOND VARIABLE. Walking the selected table pointer itself
   merges the guard's read with the loop's and loses the target's
   `addu $a1, $v1, $zero`: 26 words against 27, 20 differing. `for (q = p; ...)`
   over the same body is 27 / 12 -- the copy has to sit AFTER the guard test.
   `*q` is read twice per iteration because the `sw` between them flushes
   cse. 27 words, 0 differing. */
u8 *func_800DCA1C(u8 *base, s32 which)
{
    u8 *p;
    u8 *q;
    u8 *r;

    r = NULL;
    if (which != 0)
    {
        p = D_800E2960;
    }
    else
    {
        p = D_800E2954;
    }
    if (*p < 9)
    {
        q = p;
        do
        {
            *(u8 **)(base + *q * 0x10 + 0xC) = r;
            r = base + *q * 0x10;
            q++;
        } while (*q < 9);
    }
    return r;
}

/* The record at +0x4 is RE-READ for every store, which is the four
   `lw 0x4($a0)`. 15 words, 0 differing. */
void func_800DCA88(u8 *p, s32 flag)
{
    if (flag != 0)
    {
        *(s16 *)(*(u8 **)(p + 0x4) + 0x8) = -0x8C;
    }
    else
    {
        *(s16 *)(*(u8 **)(p + 0x4) + 0x8) = -0x22;
    }
    *(s16 *)(*(u8 **)(p + 0x4) + 0xA) = -0x52;
    *(s16 *)(*(u8 **)(p + 0x4) + 0xC) = 0x200;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCAC4);

/* Rebuild the three tag chains and hand back the cursor func_800DCA1C stopped
   at; func_800DBE98's own result is dropped. 22 words, 0 differing. */
s32 func_800DCBE4(u8 *p, s32 flag)
{
    s32 r;

    func_800DCA88(p, flag);
    func_800DCAC4(p, flag);
    r = (s32)func_800DCA1C(*(u8 **)p, flag);
    func_800DBE98();
    return r;
}

/* The engine's two GPU-primitive templates stamped into nine 0x10-byte and
   nine 0xC-byte slots off the record's +0x04 and +0x08 pointers.

   BOTH TEMPLATES ARE REACHED BY SUBSCRIPT, not through a `u32 *` pointer
   local: the subscript keeps the symbol's own `high` for element 0 --
   `lw %lo(D_8002E5D8)($t3)` -- while cse materialises the full address for
   elements 1..3. The pointer local eats that `%lo` and gives `lw 0x0($t0)` in
   both copies: 46 words, 9 differing. Mixing the two is 48 / 47. The
   destination base is re-read out of the record for every store.
   46 words, 0 differing. */
void func_800DCC3C(u8 *p)
{
    s32 i;

    for (i = 0; i < 9; i++)
    {
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4)) = D_8002E5D8[0];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 4) = D_8002E5D8[1];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 8) = D_8002E5D8[2];
        *(u32 *)(i * 0x10 + *(u32 *)(p + 4) + 0xC) = D_8002E5D8[3];
        *(u8 *)(i * 0x10 + *(u32 *)(p + 4) + 0xF) = 4;
        *(u32 *)(i * 0xC + *(u32 *)(p + 8)) = D_8002E5E8[0];
        *(u32 *)(i * 0xC + *(u32 *)(p + 8) + 4) = D_8002E5E8[1];
        *(u32 *)(i * 0xC + *(u32 *)(p + 8) + 8) = D_8002E5E8[2];
    }
}

/* 17 words, 0 differing. */
s32 func_800DCCF4(u8 *p, void *a)
{
    *(u8 **)(p + 0x4) = D_800E2828;
    *(u8 **)(p + 0x8) = D_800E28B8;
    *(void **)(p + 0x0) = a;
    *(u8 **)(p + 0xC) = D_800E2804;
    func_800DCC3C(p);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCD38);

/* `UnkNode *` IS WHAT LETS THE SCHEDULER INTERLEAVE. With `u8 *n` and
   `*(u16 *)(p + 0x1E) |= 0x8000`, the load of D_800814A0 cannot move above
   the `sh` and the function is 22 words with a load-delay nop; as a
   COMPONENT_REF, MEM_IN_STRUCT_P separates it from the scalar global and
   both `|=` interleave. 21 words, 0 differing. */
void func_800DCE0C(UnkNode *n)
{
    func_8004B1A4(*(s32 *)((u8 *)n + 0x5C));
    D_800E5910 = NULL;
    n->unk_1E |= 0x8000;
    D_800814A0 |= 0x8000;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089490);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089498);
