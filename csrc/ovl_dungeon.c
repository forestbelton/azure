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
s32 func_800AC480(u8 *, UnkVec *, u8 *, u8 *);
s32 func_800AC5C8();
s32 func_800ACE34(u8 *, UnkVec *, u8 *, u8 *);
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
/* FOUR parameters: it reads $a0, $a2 and $a3. Its one call site, in the
   landed func_800D4494, is a pure FORWARD of that function's own arguments,
   so no argument setup is emitted and the wrong arity was invisible. */
void func_800AA258(u8 *, void *, u8 *, u8 *);

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
    /* func_800C2F2C clears this whole word through the subscript. */
    s32 unk_08;
    UnkNode *unk_0C;
    u8 pad_10[0x2];
    /* s16, not u16: func_800B500C reads it with `lh` into a subtraction.
       The two `+=` sites truncate on store, so the width is invisible
       there -- both re-measure at 0 differing. */
    s16 unk_12;
    u16 unk_14; /* flags; 0x40 gates func_800B627C's re-dress */
    u8 pad_16[0x2];
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
extern s32 D_800DCF78;
void func_800C4944();
void *func_800990FC(void);
void *func_80099194(u8 *, u8 *);
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
/* RETURNS the advanced cursor: the `addu $v0, $a0, $zero` in its `jr`'s
   delay slot is a real `return p`, and as `void` the function is one word
   short. Both call sites drop the value. */
u8 *func_800998C0(u8 *, u8 *);
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
/* TWO parameters: it writes $a1[0..2] and never sets $a1. func_800A020C,
   its only jal caller, is a pure forwarder and was widened to match -- all
   five of ITS callers set $a1 before the jal. */
void func_800A0150(u8 *, u8 *);
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
void *func_80099734(u8 *, u8 *);
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

/* Not void: the body ends in `addu $v0, $zero, $zero`, the same correction
   func_800A624C's declaration below carries. Only its address is taken, so
   nothing had falsified the old spelling. */
s32 func_800A5FC0();
void func_800A6194();
/* Not void: the body ends in `addu $v0, $zero, $zero`. Only its address is
   taken, so nothing had falsified the old spelling. */
s32 func_800A624C();
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
s32 func_800A19E4(s32, u8 *, s16, s16, u8 *);
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
/* Returns 0 -- the listing zeroes $v0 on both exits -- and reads $a0 as a
   node-payload pointer. Only its address is taken, so nothing had falsified
   the old `void`. */
s32 func_800BC3E4(u8 *);
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
/* The engine's buffer allocator and the handler table func_800DCD38
   installs. */
u32 func_8004B404(s32);
extern u8 D_8004CAA0[];
/* func_800C1A44's handler and its renderable's descriptor. */
void func_800C1718();
extern u8 D_800DF3C0[];
/* func_8008DB0C's direction table and the engine pointer its tail tests. */
extern u8 D_800DD130[];
void func_800B63A8();
extern u8 D_800DEA68[];
extern u8 D_800E188B[];
extern u8 D_800E1729[];
extern u8 D_800E101C[];
extern u8 D_800E1580[];
extern u8 D_800E15A2[];
extern u8 *D_80082EB4;
s32 func_80069EF8(void);
extern u8 *D_80082EB0;
s32 func_80098C80(u8 *);
void func_800C9F34();
void func_80093A94();
void func_800ACB98(u8 *, void *, u8 *, u8 *);
void func_800AA53C(u8 *);
/* func_8009955C's two string tables. */
extern u8 *D_800DD72C[];
extern u8 *D_800DD784[];
s32 func_80064584(s32);
void func_800A5828();
extern u8 D_800DD854[];
extern u8 D_800DD860[];
extern u8 D_800DD120[];
extern u8 D_800E1C25[];
s32 func_800CD994(u8 *, s16);
extern u8 D_80081471[];
extern u8 D_800E04BA[];
extern u8 D_800E04E6[];
void func_800CEA44();
void func_800A77AC();
void func_800C0404();
extern u8 D_800DF4A4[];
void func_80048190(s16);
extern u8 D_800DCFE0[];
extern u8 D_800DCFE8[];
extern u8 D_800E3E41;
void func_800666E0(u8 *);
void func_80066640(u8 *, s32);
void func_800DAEF4();
extern u8 D_800DF358[];
extern u8 D_800E045F[];
extern u8 *D_800DD728;
void func_800B8830();
extern u8 D_800DF2F8[];
extern u16 D_800DF37C;
extern u16 D_80083164[];
/* The node view func_800B8C20 fills at +0x20. IT HAS TO BE A STRUCT, and the
   witness is the scheduler: through a `u8 *` the stores are not
   MEM_IN_STRUCT_P, so sched1 cannot lift the `lw` of D_800814A8 over them and
   the function comes out 2 to 6 differing whichever way the statements are
   ordered (24 store permutations measured, best 2). */
typedef struct UnkDungeonTag
{
    s32 unk_00;
    u8 *unk_04;
    s32 unk_08;
    u8 *unk_0C;
    u8 pad_10[0x22];
    s16 unk_32;
} UnkDungeonTag;
void func_800C75D0();
void func_800DA93C();
void func_800DAB50();
extern u8 D_80088CA8[];
extern u8 D_800DF870[];
extern u8 D_800E18C0[];
extern u8 D_800E1A55[];
extern u8 D_800E3DD0[];
extern u8 D_800E3DE0[];
extern void *D_800DCF08[];
extern void *D_800DCF10;
extern void *D_800DCF3C;
extern void *D_800DCF44;
extern void *D_800DCF48;
void func_800401FC(void);
void func_8004027C(void);
void func_8004D70C(void);
void func_80044B48(void);
s32 func_80042A80(u8 *);
s32 func_80045310(u32);
s32 func_8006649C(s32, s32);
UnkNode *func_800A8608(void *, void *, s16, s16, s16);
UnkNode *func_800BC26C(void *, s16, s32, s32);
/* The 8-byte, alignment-1 object func_800DA840 copies into each spawned
   node's +0x40: the target moves it with the `lwl`/`lwr` + `swl`/`swr` pairs
   an alignment of 1 forces, exactly as UnkDungeonName12 above. */
typedef struct UnkDungeonName8
{
    u8 b[0x8];
} UnkDungeonName8;
void func_8004D0C8(u32);
void func_8004D294(u16 *, u8 *, s32);
void func_8004D7A8(s32);
extern s32 D_800832B4;
extern u8 D_800DCE60[];
/* func_800D4AB0 parks this step in the node it spawns. */
void func_800D4954();
extern u8 D_800E233C[];
extern u8 D_80046398[];
/* func_80093C70's option word: the listing reads it with
   `lhu %lo(D_80082E94)($v0)` -- a %lo inside the load, so a declared
   scalar. */
extern u16 D_80082E94;
void func_800BBBE0();
void func_800CB374();
extern u8 D_800E02CC[];
/* func_8008C7B4's two eight-way facing tables. */
extern u8 D_800DD0B8[];
extern u8 D_800DCFB0[];
/* func_800DCAC4's 16-byte record. The stride is the loops' `sll 4`; the
   record is spelled as a TYPE rather than as pointer arithmetic because the
   destination's `addu` is INDEX-FIRST, which C pointer arithmetic cannot
   emit. */
typedef struct UnkDungeonDCAC4
{
    u32 unk_00;
    u32 unk_04;
    u32 unk_08;
    u32 unk_0C;
} UnkDungeonDCAC4;
void func_80069F28(u8 *, u8 *, s32);
/* func_80097934's eight-way descriptor table; its entries go to
   func_8003DB94, so a pointer array and not the byte tables func_80048A44's
   installers index. */
extern void *D_800DD274[];
u32 func_8004383C(u8 *, s16);
void func_80067E2C(u8 *, void *);
/* func_800C7930's two halfword tables, indexed by the renderable's facing. */
extern u16 D_800DCEAC[];
extern u16 D_800DCEBC[];
/* func_80091A38's table of per-kind handler-word tables. */
extern u32 *D_800DD6B8[];
/* The 4-byte, alignment-1 object func_800B8228 copies into the spawned
   node's +0xB8. */
typedef struct UnkDungeonName4
{
    u8 b[0x4];
} UnkDungeonName4;
void func_800B80D8();
void func_800DB420();
extern u8 D_8004F5F4[];
extern s32 D_800E4940;
void func_800CB9DC();
/* Its own object, not D_800DF64C + 4: D_800DF640 is three pointers and
   D_800DF64C the three bytes func_800C7D54 indexes beside them, so the byte
   array ends at 0x800DF64F. */
extern u8 D_800DF650[];
extern u8 D_80089074[];
extern u8 D_800E1AE6[];
extern u8 D_800E1BDA[];
void func_800B06F0();
extern u8 D_800E1A88[];
extern u8 D_800E1A9D[];
extern u8 D_800E1AC2[];
extern u8 D_800E1AD0[];
s32 func_800422A8(u8 *, void *, s32, s32);
void func_800BABA8();
extern u8 D_800E03B0[];
extern u8 D_800E03D4[];
void func_800CC744();
UnkNode *func_800CC5F0(s16, s16, s16, s16, s16, s16);
s32 func_800CEEFC(u8 *, UnkVec *, UnkSlot *);
s32 func_8003F794(s16, s16);
void func_8003E1C4(s16);
void func_800AFAD8();
void func_800AFBFC();
extern s8 D_800DCF4C;
extern u8 D_800DEF90[];
extern u16 D_800E2924[][2];
extern u8 D_800E1BBC[];
extern void *D_800DF258[];
/* The per-kind hatch func_800B627C dispatches through. */
typedef s32 (*UnkDungeonHatch)(u8 *, s16);
extern UnkDungeonHatch D_800DF2A8[];
extern u8 D_80079444[];
extern s16 D_800DCE66;
void func_800B96C4();
extern s16 D_800E2468[][2];
extern u8 D_800E2178[];
extern u8 D_800E221C[];
/* The nine kill handlers of the g2 batch: their report strings, the retry
   counter func_800C0E88 runs down, and the engine helpers none of them had
   a prototype for. */
extern u8 D_800E0E14[];
extern u8 D_800E0E34[];
extern u8 D_800E0E50[];
extern u8 D_800E11F5[];
extern u8 D_800E131C[];
extern u8 D_800E1440[];
extern u8 D_800E144C[];
extern u8 D_800E1456[];
extern u8 D_800E146F[];
extern u8 D_800E14D8[];
extern u8 D_800E14EB[];
extern u8 D_800E1514[];
extern u8 D_800E1567[];
extern u8 D_800E15BB[];
extern u8 D_800E15E5[];
extern u8 D_800E1684[];
extern u8 D_800E169A[];
extern u8 D_800E17C6[];
extern u8 D_800E17EF[];
extern u8 D_800E180E[];
extern u8 D_80089354[];
extern u8 D_80089374[];
extern u8 D_800893D4[];
extern s16 D_800DF4B0;
/* The same 0x14-byte D_8006DE24 record as include/structs.h's Unk8006DE24,
   under the one more face func_800C0E88 needs: the pointer at +0x00 it hands
   to func_80099194. */
typedef struct UnkDungeonName0
{
    u8 *unk_00;
    u8 pad_04[0x10];
} UnkDungeonName0;
s32 func_800A05A4(u8 *, s32, s32, s16, s32);
s32 func_800A94A0(u8 *, u8 *, s32, u8 *);
extern u8 D_800DD0F8[];
/* func_800A19E4 reads this byte with its OWN `lui`, in a block the
   D_80082E80 address materialisation does not reach, so it is its own
   symbol rather than `D_80082E80[0x26]`. */
extern s8 D_80082EA6;
/* func_800A9230 reads and writes these with `lbu` and `lh`/`sh`. */
extern u8 D_800DCF5A;
extern s16 D_800E3CCC;
/* The g1 batch of kill handlers. func_800C7380 and func_800B60B8 are in this
   unit and sit below their caller; func_800C8CD8 is defined below too. */
u8 *func_800C7380(s32, s32, s16, s32, s32);
s32 func_800B60B8(s32, s32, s16, s32, u8 *);
s32 func_800C8CD8(u8 *, s16, s16);
void func_800C1F28(u8 *, s16);
void func_8009D6F4(void);
/* `void func_800403BC(s32)` where it is defined, in csrc/unk_12DEC.c. */
void func_800403BC(s32);
/* Parked through func_800403BC by func_800C3D3C; an address-take needs a
   declaration above it. */
void func_800CE028();
extern u8 D_80089378[];
extern u8 D_8008937C[];
extern u8 D_800893D8[];
extern u8 D_800DF45C[];
extern u8 D_800E1375[];
extern u8 D_800E13B7[];
extern u8 D_800E0F2D[];
extern u8 D_800E0F44[];
extern u8 D_800E0F65[];
extern u8 D_800E0F85[];
extern u8 D_800E13DA[];
extern u8 D_800E13E9[];
extern u8 D_800E1532[];
extern u8 D_800E1549[];
extern u8 D_800E16E3[];
extern u8 D_800E1713[];
extern u8 D_800E1756[];
extern u8 D_800E176F[];
extern u8 D_800E1788[];
extern u8 D_800E1863[];

/* The overlay's per-frame entry point; `void func_800894A0(void)` is declared
   in include/structs.h because the resident engine calls it. D_80083160 is a
   POINTER LOCAL assigned before the first call -- that is where the
   `lui`/`addiu` pair in the prologue comes from. 58 words, 0 differing. */
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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C8BC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D470);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D7D0);

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
void func_8008DB0C(UnkDungeonActor *p, void *b, u8 *c, s16 d, u16 e)
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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096C24);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800971A8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098920);

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
        *(s32 *)(sp + 0xC0) = func_80065420((SVECTOR *)sp, (s32 *)(sp + 0xB8),
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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009AF18);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009CE1C);

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
    func_80067E2C(q, D_80083160.unk_00);
    func_8006658C(r, q);
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
    func_8006658C(r, u);
    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    func_80067E2C(q, w);
    func_8006658C(r, q);
}

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

/* Step the text cursor forward. The record is the fixed RAM address
   0x80013710, a constant address rather than a `D_` symbol.

   (1) THE BACK EDGE IS A `goto`, which loop.c cannot see, so the `lui`/`ori`
   pair is re-materialised at the top of every iteration where a `while (1)`
   would have let it be hoisted into a preheader.
   (2) THE LAST `return q` NEEDS ITS OWN BLOCK. Written as a plain tail the
   `move $v0, $a0` is scheduled into the `lhu 0xA`'s load-delay slot and the
   counter is pushed to $v1 -- 72 words, 5 differing. Sharing one exit through
   `goto out` puts the copy in a JOIN BLOCK where sched1 cannot reach it,
   which also lets the counter have $v0. An if/else with the tail duplicated
   is 75 words: cross-jumping does not merge them, because the two copies
   schedule differently. 72 words, 0 differing. */
u8 *func_8009F868(void)
{
    UnkDungeon13710 *p;
    u8 *q;

loop:
    p = (UnkDungeon13710 *)0x80013710;
    q = p->unk_0C + p->unk_0A * 2;
    if (q[1] == 0)
    {
        if (D_80013714 & 8)
        {
            *(u16 *)((u8 *)p + 0x4) |= 0x10;
            return NULL;
        }
        *(u16 *)((u8 *)p + 0x4) &= 0xC;
        p->unk_0A = 0;
        func_8004D0C8((u32)D_80083780);
        D_800832B4 = 0;
        func_8004D7A8(1);
        func_8004D294(NULL, D_800DCE60, 8);
        return NULL;
    }
    if (q[1] == 0xD0)
    {
        p->unk_08 = 0;
        p->unk_0A += *(s8 *)q;
        goto loop;
    }
    if (*(s8 *)q >= 0)
    {
        p->unk_08++;
        if (p->unk_08 < q[0])
        {
            goto out;
        }
    }
    p->unk_0A++;
    p->unk_08 = 0;
out:
    return q;
}

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

/* Are the two tiles adjacent, non-identical and connected?

   ONE SHARED `return 0`, NOT FIVE EARLY ONES. Written as five early returns
   the first two tests still branch to the shared tail but the third gets its
   own `j`/`move v0,zero` pair: 70 words, 21 differing. Nesting the tests so
   every failure falls out of the bottom gives the target's three
   `beqz`. `w` is both func_800A0818's out-parameter and the variable the
   shifted result is parked in -- the address-take is why the store survives.
   70 words, 0 differing. */
s16 func_8009FD7C(s16 x1, s16 y1, s16 x2, s16 y2)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                w = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((w & 1) == 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* func_8009FD7C one argument wider: on a connected pair it asks
   func_8009A540 whether the step is legal rather than testing the wall bit.
   THE SHIFTED RESULT IS ITS OWN LOCAL HERE, which is the one place the two
   differ: reusing `w` for it, as func_8009FD7C does, costs the `sh`/`lhu`
   pair that spills it to the out-parameter's slot and reloads it for the call
   -- 81 words against 80, 34 differing. 80 words, 0 differing. */
s32 func_8009FE94(s16 x1, s16 y1, s16 c, s16 x2, s16 y2)
{
    s32 dx;
    s32 dy;
    s32 adx;
    s32 ady;
    s16 r1;
    s16 r2;
    u16 w;
    s32 t;

    dx = x2 - x1;
    adx = dx >= 0 ? dx : -dx;
    if (adx < 2)
    {
        dy = y2 - y1;
        ady = dy >= 0 ? dy : -dy;
        if (ady < 2)
        {
            if (adx + ady != 0)
            {
                r1 = func_8009FB34(x1, y1);
                r2 = func_8009FB34(x2, y2);
                if (r1 == r2)
                {
                    return 1;
                }
                t = (s16)func_800A0818(x1, y1, x2, y2, &w) >> 9;
                if ((s16)func_8009A540((u16)t, x1, y1, (s16)(c - 0x20)) != 0)
                {
                    return 1;
                }
            }
        }
    }
    return 0;
}

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

/* Tint three colour bytes by the engine's low-five-bit frame counter.

   TWO DECLARATIONS CARRY THE WHOLE FUNCTION, both about width.
   - THE THREE QUOTIENTS ARE `s16`. As s32 the signed divide-by-16 ends in
     `sra` where the target has `srl`: combine converts the ASHIFTRT to an
     LSHIFTRT only when it can see the result narrowed, and an s16
     destination is what narrows it. `u8` quotients, a `(u8)` cast at the
     store and an `s8 *` destination all leave the three `sra`.
   - `t` IS `s16` AND IS FIRST ASSIGNED THE UNMASKED HALFWORD, which is what
     keeps the `sll 16`/`sra 16` pair: with `t` only ever assigned masked
     values, combine's reg_nonzero_bits proves the top bits clear and elides
     the extension, one word short. The masked value lives in its own `s32 u`,
     which is also where the `addu $a2, $v1, $zero` copy comes from.
   47 words, 0 differing. */
void func_800A0150(u8 *p, u8 *d)
{
    s16 t;
    s32 u;
    s16 r;
    s16 g;
    s16 b;

    t = D_80083164[0];
    u = t & 0x1F;
    t = u;
    if (u & 0x10)
    {
        t = -u & 0x1F;
        if (t == 0)
        {
            t = 0x10;
        }
    }
    r = (0x80 - p[0]) * t / 16;
    g = (0x80 - p[1]) * t / 16;
    b = (0x80 - p[2]) * t / 16;
    d[0] = -0x80 - r;
    d[1] = -0x80 - g;
    d[2] = -0x80 - b;
}

/* Six bit tests in priority order, each arm a string address, and the NULL
   test after the chain is real: cc1psx does not fold `&array != 0`.
   40 words, 0 differing. */
void func_800A020C(s32 f, u8 *d)
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
        func_800A0150(s, d);
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

/* CARRIES A DEVICE, and the device-free floor is 5 differing.

   The whole residue is an ALLOCATION one: every opcode, immediate and
   displacement already matches and only three registers are wrong. The target
   puts the loop counter in $a2, the sign-extended `k` in $a1 and the raw `k`
   copy in $a3; the plain source puts the counter in $a1 and both `k`s in $a2.
   Read off `.lreg`/`.greg`: global.c's `floor_log2(n_refs) * n_refs /
   live_length` scores the counter 9 refs over 30 insns (0.90) against the
   extended `k`'s 7 over 18 (0.78), so the counter is allocated first and
   takes the lower register. The counter has to LOSE that race, and the only
   thing that moves it is making it live at FUNCTION ENTRY: that stretches its
   live length to 48, scores it 0.69, and every register falls into place.

   Measured, all 64 words: plain 13 differing; `i = 4` before the chain 5;
   every arm writing `i` before `p` 9 (all-four-or-nothing -- the other
   fifteen subsets are 15..19); `i = 0` before the chain 25; a third `s16 i`
   parameter 13 (its incoming copy is dead on every path, so flow deletes it);
   a two-case switch 24; a `do`/`while` 13; `s32 i` 63 words / 22. Two
   permuter runs, 352 and 82,594 candidates, agree that no legal spelling
   reaches 0.

   THE DEVICE READS AN UNINITIALISED LOCAL. It emits no instruction and exists
   only to make `i` live-in at entry. The function has no callers and no
   address-takers anywhere in asm/, so nothing downstream depends on it. If
   this is not wanted, delete the two device lines and write `i = 4;` before
   the `if` chain with the k==3 arm reduced to `p = D_800E3DD0;` -- that is 64
   words, 5 differing, all inside that one arm. 64 words, 0 differing. */
u8 *func_800A17E8(s16 x, s16 k)
{
    u8 *g;
    u8 *p;
    s16 i;

    /* DEVICE: a zero-instruction reference that makes `i` live at entry. */
    __asm__("" ::"r"(i));
    if (k == 3)
    {
        p = D_800E3DD0;
        i = 4;
    }
    else if (k == 2)
    {
        return D_800E3DE0;
    }
    else
    {
        g = D_800E3DB0;
        if (g[0x11] == 0x38)
        {
            p = g + 0x18;
            i = 3;
        }
        else if (g[0x9] == 0x31)
        {
            p = g + 0x10;
            i = 2;
        }
        else
        {
            p = g;
            i = 0;
        }
    }
    while (i < 6)
    {
        if (k == 1 || k == 3)
        {
            if (p[0] != k)
            {
                return NULL;
            }
        }
        if (p[1] == x)
        {
            return p;
        }
        i++;
        p += 8;
    }
    return NULL;
}

/* Run down one 8-byte D_800E3DB0 entry whose kind and owner match.

   THE OWNER LOCAL IS `s16`. As `s32 m` the whole register assignment rotates
   -- the value is loaded into $a0 and copied into $s0 at the call where the
   target loads it into $s0 and copies it into $a0: 63 words, 16 differing,
   every one a register name, with no extra instruction anywhere. As `u8 m`
   it also costs an `andi` (64 words). 63 words, 0 differing. */
void func_800A18E8(s16 a, s16 b)
{
    s16 i;
    u8 *p;
    s16 m;

    p = D_800E3DB0;
    if (a == 0x39)
    {
        a = 2;
    }
    if (b == 1 || b == 3)
    {
        for (i = 0; i < 6; i++, p += 8)
        {
            if (p[0] == b)
            {
                m = p[1];
                if (m == a)
                {
                    if (*(s8 *)(p + 2) <= 0)
                    {
                        return;
                    }
                    if (p[0] == 1)
                    {
                        return;
                    }
                    p[2]--;
                    if ((s8)p[2] > 0)
                    {
                        return;
                    }
                    p[1] = 0;
                    p[2] = 0;
                    if ((s16)func_800A15B0(m) == 0)
                    {
                        func_80048190(m);
                    }
                    return;
                }
            }
        }
    }
}

/* Raise or clear the view's 0x20000 "this one is interesting" bit from the
   distance to D_80082E80, and hand the caller back 0 when the bit ends up
   clear. THREE THINGS THE LISTING FORCES.
   THE RETURN TYPE IS NOT `void`: the declaration at the top of the unit said
   so, but the tail reads +0x1C again and shifts it into $v0, which a void
   function would have deleted -- so it is now `s32`. Both landed callers
   (func_800A1B44, func_800AA53C) discard the result and both still measure
   0 differing.
   THE SHIFT IS UNSIGNED: through `*(s32 *)` it is `sra 17` (1 differing);
   the field has to be read as `u32` for the `srl`.
   AND THE OUT-PARAMETER'S -1 IS SIGNED: `*out = -1` through the declared
   `u8 *` is `li $v0, 255` where the target has `addiu $v0, $zero, -0x1`
   (2 differing, one per arm), so the store casts to `s8 *`.
   D_80082EA6 is its own symbol and not `D_80082E80[0x26]`: the `lb` carries
   its own `lui`, in a block the call's `addiu $a0, %lo(D_80082E80)` does not
   reach. 88 words, 0 differing. */
s32 func_800A19E4(s32 a, u8 *b, s16 c, s16 d, u8 *out)
{
    s32 fl;

    fl = *(s32 *)(b + 0x1C);
    if ((fl & 0x20000) != 0)
    {
        if ((fl & 0x10) != 0)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
        else if ((*(s8 *)(a + 0x26) < 0 || *(s8 *)(a + 0x26) != D_80082EA6) &&
                 func_8009FD40(D_80082E80, (u8 *)a) >= d)
        {
            *(s32 *)(b + 0x1C) &= ~0x20000;
            b[0x73] = 0;
            b[0x72] = 0;
            *(s8 *)out = -1;
        }
    }
    else if ((fl & 0x10) == 0)
    {
        if ((*(s8 *)(a + 0x26) >= 0 && *(s8 *)(a + 0x26) == D_80082EA6) ||
            func_8009FD40(D_80082E80, (u8 *)a) < c)
        {
            *(s32 *)(b + 0x1C) |= 0x20000;
        }
    }
    return (*(u32 *)(b + 0x1C) >> 17) & 1;
}

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

/* A five-gate predicate returning 0 or 1 as an s16 -- the shared exit is
   `sra $v0, $v0, 16`, and each path narrows with its own `sll 16`. The last
   comparison is written TABLE-FIRST: the other order emits the two `lw`s
   reversed and swaps $v0/$v1 across six words. 61 words, 0 differing. */
s16 func_800A1C58(u8 *p)
{
    s16 r;

    r = 0;
    if (*(s8 *)(p + 0x13) < 0)
    {
        return 0;
    }
    if (p[0x11] >= 0x63)
    {
        return 0;
    }
    if ((s16)func_80042900((s8 *)p, 0xA) != 0)
    {
        return 0;
    }
    if ((*(u32 *)(p + 0x1C) & 0x80000) != 0)
    {
        return 0;
    }
    if (p[0x13] == 0)
    {
        if (D_800E3D7C[0x9A] == 0x22)
        {
            return 0;
        }
    }
    if ((*(u32 *)(p + 0x1C) & 0x228) == 0 && D_800835E8[p[0x11]] <= *(u32 *)(p + 0x18))
    {
        r++;
    }
    return r;
}

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

/* Walk the ring from q looking for a neighbour that is either in the same
   room as (x, y) or standing within one tile of it; func_8009B25C is the
   same walk against a facing instead of a room. 73 words, 0 differing.

   THREE SPELLINGS WERE MEASURED, and two of them are not guessable.
   (1) The comparison is `r == e[0x26]`, NOT `e[0x26] == r`: with the operands
   the other way round every callee-saved register rotates by one -- 24
   differing with the whole structure already exact -- because the HImode
   extension of `r` is born in the other order.
   (2) `r` IS `s16`, so the call result's `sll`/`sra` sits at the first INT
   use, inside the `if`, where reorg can take the `sll` into the `beq`'s
   delay slot. As `s32` the extension is at the assignment and lands ahead of
   the `lw` -- 5 differing, no word change.
   (3) THE 1 IS A VARIABLE. `abs(d) > 1` folds to `d >= 2` and comes out as
   `slti $v0, $v0, 2`; the target's `slt $v0, $s2, $v0` needs the constant in
   a register, which only a local gives, and the local must be `s16`: as
   `s32` it takes a seventh callee-saved register and the function is 75
   words. */
u8 *func_800A3F28(u16 x, u16 y, u8 *h, u8 *q)
{
    u8 *p;
    u8 *e;
    s16 r;
    s16 lim;
    s16 xx;
    s16 yy;
    s32 d;

    r = func_8009FB34(x, y);
    p = *(u8 **)(q + 0x5C) + 0x20;
    if (p != h)
    {
        xx = x;
        lim = 1;
        yy = y;
        do
        {
            if (func_800A41F0(p) != 0)
            {
                e = *(u8 **)(p - 0x14);
                if (r == *(s8 *)(e + 0x26) && r >= 0)
                {
                    return p;
                }
                d = xx - e[0x24];
                d = abs(d);
                if (d <= lim)
                {
                    d = yy - e[0x25];
                    d = abs(d);
                    if (d <= lim)
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

/* Pick the best of the record's three 3-byte slots, starting from
   D_8008347E's low two bits and wrapping at 3; the winner is the one with
   the highest +0x09, ties broken by func_800A35D8's score, and the answer
   is the slot index plus 5. 81 words, 0 differing.

   `q[9]` IS READ BEFORE THE CALL. It is live across it in $s0, and gcc will
   not move a load across a call, so reading it after is 11 differing. The
   two score locals are `s16`, which is what gives the tie-break its
   `sll`/`sll`/`slt` with no `sra`. */
s16 func_800A40AC(u8 *p, u16 arg)
{
    s32 n;
    s16 i;
    s16 best;
    s16 bestt;
    s16 ans;
    s16 t;
    s16 q9;
    u8 *q;

    ans = -1;
    bestt = -4;
    best = 0;
    i = D_8008347E & 3;
    for (n = 0; n < 3; n++, i++)
    {
        if (i == 3)
        {
            i = 0;
        }
        q = p + i * 3;
        if (q[8] != 0)
        {
            q9 = q[9];
            t = func_800A35D8(D_8006DE24[q[8]].unk_10, arg);
            if (best < q9)
            {
                best = q9;
                ans = i + 5;
                bestt = t;
            }
            else if (best == q9 && bestt < t)
            {
                bestt = t;
                ans = i + 5;
            }
        }
    }
    return ans;
}

/* Can this actor act? THE LAST TEST IS NARROW: `sll $v0, 16` / `sltiu $v0, 1`
   is the low halfword of func_80042900's result compared against zero, so the
   result takes an `(s16)` cast at the point of use rather than a changed
   prototype. 42 words, 0 differing. */
s16 func_800A41F0(u8 *p)
{
    u8 *q;
    u16 r;

    if (p[0x28] + *(s16 *)(p + 0x64) <= 0)
    {
        return 0;
    }
    if (*(s32 *)(p + 0x1C) & 0x2000)
    {
        q = *(u8 **)(p - 0x14);
        func_8009A350(q[0x24] - 1, q[0x25], 0, &r);
        if (r & 2)
        {
            return 0;
        }
    }
    if (*(s32 *)(p + 0x1C) & 0x290000)
    {
        return 0;
    }
    return (s16)func_80042900((s8 *)p, 0x1B) == 0;
}

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

/* Pick what the cursor is standing on. The `sw $zero, 0xF0($v1)` tail is
   shared by the first and last arms, which is cross-jumping putting the
   duplicated statement back. 75 words, 0 differing. */
void func_800A4300(u8 *a, u8 *b)
{
    s16 r;
    s16 i;
    s16 j;

    r = func_800A4474(a[0x24], a[0x25]);
    if (r != 0)
    {
        *(u16 *)D_80081470 = r | 0x1400;
        *(u32 *)(D_800814A8 + 0xF0) = 0;
    }
    else
    {
        i = func_800B500C(a[0x24], a[0x25], *(s16 *)(b + 0x88));
        if (i >= 0)
        {
            if ((D_800E3648_s[i].unk_03 & 0x40) == 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3648_s[i];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3648_s[i];
            }
            else
            {
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
        else
        {
            j = func_800A70E4(a[0x24], a[0x25], *(s16 *)(b + 0x88));
            if (j >= 0)
            {
                *(u32 *)D_80081470 = *(u32 *)&D_800E3548_s[j];
                *(u32 *)(D_800814A8 + 0xF0) = (u32)&D_800E3548_s[j];
            }
            else
            {
                *(u32 *)D_80081470 = 0;
                *(u32 *)(D_800814A8 + 0xF0) = 0;
            }
        }
    }
}

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

/* TWO ALLOCATION FACTS, and nothing else in the function is free. The
   residue of the obvious spelling is 11 to 23 differing with every opcode,
   immediate and displacement already right and only $s1/$s2/$s3 permuted.

   (1) THE TWO SUMS GO THROUGH `u16` LOCALS, so the truncation happens at the
   ASSIGNMENT and not inside the argument expression. Written inline, `u`
   scores 3 refs over 24 insns against `v`'s 3 over 30 and takes the lower
   register, which is the wrong way round; through the two `u16` temporaries
   the lengths become 28 and 26 and `v` wins. Hoisting the same sums into
   `s32` temporaries changes NOTHING -- cse folds them back -- so it is the
   TYPE of the temporary, not the statement split.

   (2) THE THIRD PARAMETER IS `s16`, and that is what puts it in $s1: as s32
   it is live over 34 insns and scores 0.059, below both coordinates; as s16
   the pseudo is live over 17 and scores 0.118. Two callers confirm it --
   func_800A36B4 and func_800A05A4 both sign-extend $a2 immediately before the
   `jal`. `dir` is the other way: func_800A05A4 passes `$t0 << 9` with no
   truncation, so it is u32. 62 words, 0 differing. */
s32 func_800A44E0(u16 x, u16 y, s16 c, u32 dir)
{
    s16 t;
    s32 u;
    s32 v;
    s32 i;
    u16 aa;
    u16 bb;

    u = x >> 6;
    v = y >> 6;
    i = (dir >> 9) & 7;
    if (func_8009A350(u, v, i, (u16 *)&t) == 0)
    {
        return 0;
    }
    if ((t & 0x8000) != 0)
    {
        return 1;
    }
    aa = ((u16 *)D_8006CCD8)[i] + u;
    bb = ((u16 *)D_8006CCE8)[i] + v;
    t = func_800BCB04((((s32)aa << 6) + 0x20) & 0xFFE0, (((s32)bb << 6) + 0x20) & 0xFFE0,
                      c - 0x20);
    return t > 0x200;
}

/* Is the tile ahead blocked? THE TWO TILE INDICES ARE `u16` LOCALS. As s32
   the shift lands straight in the callee-saved register and the function is
   43 words against 44 with 41 differing; an HImode pseudo cannot be the
   `srl`'s destination, so the shift goes to a temp and the copy into the
   callee-saved register is the missing word. 44 words, 0 differing. */
s16 func_800A45D8(u16 x, u16 y, s32 c)
{
    u16 tx;
    u16 ty;
    s16 r;

    tx = x >> 6;
    ty = y >> 6;
    if (func_8009A350(tx - 1, ty, 0, (u16 *)&r) == 0)
    {
        return 0;
    }
    if (r & 0x8000)
    {
        return 1;
    }
    r = func_800BCB04((tx << 6) | 0x20, (ty << 6) | 0x20, (s16)(c - 0x20));
    return r > 0x200;
}

/* Is the tile one step along `ang` from (px, py) blocked? func_8009B164 is
   the same probe one record out and its three spellings transfer.

   THREE THINGS ARE SPECIFIC. (1) `face` is a NARROW parameter: as s32 with
   the `(s16)` cast at the call it is 60 words and 11 differing, every
   difference a callee-saved register -- the sign-extension of a 16-bit
   parameter is one more reference on its allocno, which lifts it past
   `gx`/`gy` in global-alloc's priority. Nothing calls this function anywhere
   in the project, so the width is unwitnessed. (2) `k` must be s16 as well:
   as s32 the index is computed straight into $s0 and copied to $a2 at the
   call, where the target computes it into $a2 and copies it into $s0, 17
   differing. (3) The `+ 0x20` must be its own set of an ALREADY-SET variable:
   in one expression combine proves the low bits clear and turns the add into
   `ori`, 21-32 differing. 60 words, 0 differing. */
s32 func_800A4688(u16 px, u16 py, u16 face, u16 ang, s32 flag)
{
    u16 m;
    s32 gx;
    s32 gy;
    s16 k;
    u16 u0;
    u16 u1;
    s32 x;
    s32 y;
    s16 r;

    gx = px >> 6;
    gy = py >> 6;
    k = (ang >> 9) & 7;
    if (flag == 0)
    {
        if ((s16)func_8009A350(gx, gy, k, &m) != 0)
        {
            u0 = (u16)D_8006CCD8[k] + gx;
            u1 = (u16)D_8006CCE8[k] + gy;
            x = u0 * 64;
            x = (x + 0x20) & 0xFFE0;
            y = u1 * 64;
            y = (y + 0x20) & 0xFFE0;
            r = func_800BCB04(x, y, (s16)(face - 0x20));
            m = r;
            return r > 0x200;
        }
    }
    return 0;
}

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

/* Spawn the kind-0x212 node under D_80083498. TWO POINTER LOCALS, NOT ONE
   REUSED: with a single `q` for both the +0x0C renderable and the payload
   base the two stack constants land in $a2/$v1 instead of $a3/$a2 and the
   payload pointer takes $a3 -- 54 words, 14 differing. 54 words,
   0 differing. */
UnkNode *func_800A5A18(UnkVec *a, u8 *b)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *q;
    s32 v[2];

    n = func_8003FD64(0x212, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800A5828;
        func_8004491C(n, (u32)D_80045340);
        n->unk_08 = a;
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DD854;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        q = (u8 *)n + 0x20;
        *(u8 **)(q + 0x4) = b;
        *(s16 *)(q + 0x66) = 4;
        v[0] = 0x1800340;
        v[1] = 0x400040;
        func_800B835C(D_800DD860, v, 1, 0);
        func_800A56E0(0x60F);
    }
    return n;
}

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

/* Spend the charge at +0x29 on the gauge at +0x28/+0x64. The `bgez`/+0x1E
   pair at the top is `(p[0x29] + 15) / 16` -- gcc's signed divide-by-16
   bias, not source. The total is RE-READ for the clamp rather than kept from
   the first test, which is the second `lbu 0x28`/`lh 0x64` pair.
   79 words, 0 differing on the first compile. */
void func_800A5DFC(u8 *p, u8 *v)
{
    s32 n;
    s32 tot;
    u8 *q;

    if ((*(s32 *)(p + 0x1C) & 0x40) == 0)
    {
        return;
    }
    if (p[0x28] + *(s16 *)(p + 0x64) < 2)
    {
        return;
    }
    *(s16 *)(p + 0x6A) = *(u16 *)(p + 0x2A) + 0x800;
    n = (p[0x29] + 15) / 16;
    tot = p[0x28] + *(s16 *)(p + 0x64);
    if (tot - n <= 0)
    {
        n = tot - 1;
    }
    if (n == 0)
    {
        return;
    }
    *(s32 *)(p + 0x1C) &= ~0x20000000;
    q = *(u8 **)(p - 0x14);
    if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
    {
        func_800B4C7C(3, p, (s16)n, 0);
        *(s16 *)(p + 0x64) = *(u16 *)(p + 0x64) - n;
        func_800C5BBC(*(s16 *)(v + 0x2), *(s16 *)(v + 0x6), *(u16 *)(v + 0xA) - 8, 0x802080, 0x20,
                      1);
    }
    else
    {
        p[0x28] = p[0x28] - n;
    }
}

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

/* The bar-wipe primitive pair, func_800A624C's allocator shape with
   different fill.

   (1) The base must be a POINTER LOCAL: through the bare COMPONENT_REF the
   two post-call reads re-materialise a `lui` into the dead $s0 instead of
   keeping the symbol's address in $s3 across the calls, 80 words against 81.
   (2) The +0xA store belongs in BOTH arms of the counter test, not after the
   join: with a `v` local holding the value the whole chain lands one register
   across, 12 differing, and nine spellings of the division move none of it.
   (3) The read-back of +0xA goes in a local: written out twice the store to
   the renderable's +0x14 invalidates cse's entry and the second read is a
   second `lhu`, 83 words. 81 words, 0 differing. */
s32 func_800A5FC0(u8 *p)
{
    Unk80083160 *g;
    u8 *q;
    u8 *r;
    s16 n;
    s32 w;

    g = &D_80083160;
    q = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(g->unk_00 + 0x8D0);
    *(u32 *)(g->unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, *(s32 *)(p + 0x4), 0);
    *(s32 *)(r + 0x4) = *(s32 *)(p + 0x0);
    func_800666E0(r);
    func_80066640(r, 1);
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x8) = 0;
    *(s16 *)(r + 0xE) = 0;
    *(s16 *)(r + 0x16) = 0xE0;
    n = *(u16 *)(p + 0x8) - 1;
    *(s16 *)(p + 0x8) = n;
    if (n > 0)
    {
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + (0x140 - *(s16 *)(p + 0xA)) / n;
    }
    else
    {
        *(s16 *)(p + 0x8) = 0;
        *(s16 *)(p + 0xA) = 0x140;
    }
    w = *(u16 *)(p + 0xA);
    *(s16 *)(r + 0x14) = w;
    *(s16 *)(r + 0xC) = w;
    func_8006658C(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), r);
    func_8006658C(g->unk_00 + (*(s16 *)(p + 0xC) * 4 + 0x70), q);
    return 0;
}

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

/* A node handler: take a 0xC-byte and a 0x18-byte primitive off D_80083160's
   +0x8D0 cursor and hang both on an OT row.

   THE CURSOR MUST BE READ THROUGH THE STRUCT FIELD. With
   `b = *(u8 **)&D_80083160;` and a `u8 *` store -- func_8009DA70's spelling,
   which is right for ONE allocation -- cse forwards the first bump into the
   second read, deletes the dead store and folds both allocations into one
   `addiu +0x24`: 61 words against 64, 62 differing. Reading the COMPONENT_REF
   and storing through `*(u32 *)` makes each store invalidate the next read,
   which is the two reloads the target has. The discriminator is the READ's
   spelling, not the store's. 64 words, 0 differing. */
s32 func_800A624C(u8 *p)
{
    u8 *q;
    u8 *r;
    s32 c;

    q = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(q + 0xC);
    r = *(u8 **)(D_80083160.unk_00 + 0x8D0);
    *(u32 *)(D_80083160.unk_00 + 0x8D0) = (u32)(r + 0x18);
    func_80067F20((u32 *)q, 0, 0, 0x40, 0);
    c = *(s16 *)(p + 0xA);
    *(s32 *)(r + 0x4) = c | (c << 8) | (c << 16);
    func_800666E0(r);
    func_80066640(r, 1);
    *(s32 *)(r + 0x8) = 0;
    *(s32 *)(r + 0xC) = 0x140;
    *(s32 *)(r + 0x10) = 0xE00000;
    *(s32 *)(r + 0x14) = 0xE00140;
    func_8006658C(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), r);
    func_8006658C(D_80083160.unk_00 + (*(s16 *)(p + 0xC) * 4 + 0xB0), q);
    return 0;
}

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

/* D_800DCF3C/44/48 are D_800DCF08[13], [15] and [16] by address, but the
   target gives each its own `lui`/`lw %lo(SYM)` pair and all four names are
   in splat/dungeon.bin_undefined_syms_auto.txt, so they are written as
   separate symbols; only the VARIABLE index goes through the array face.
   `s2 = 7` in the loop preheader is loop.c hoisting the `i == 7` constant.
   61 words, 0 differing. */
void *func_800A652C(u8 *p)
{
    s32 i;
    s16 r;

    if (p[0x13] != 0)
    {
        if (p[0x25] == 0)
        {
            return D_800DCF44;
        }
        if ((*(u32 *)(p + 0x1C) & 0x20000) == 0)
        {
            return D_800DCF3C;
        }
    }
    for (i = 0xE; i >= 0; i--)
    {
        r = func_80042900((s8 *)p, i);
        if (r > 0 && i == 7)
        {
            return D_800DCF48;
        }
        if (r != 0)
        {
            return D_800DCF08[i];
        }
    }
    return D_800DCF08[0];
}

/* Release the D_800E3DF0 slot the actor's +0x3 byte names, close its record
   down and report the ring index. THE MODE ARGUMENT IS A TERNARY, not the
   `if/else` into a local that func_800AA258 uses next to the same call:
   with the local, the two `lbu` argument loads stay below the branch and
   the function is 88 words / 5 differing, with exactly those two loads and
   the branch rotated; as a ternary both loads rise above it.
   ONE LOCAL CARRIES BOTH THE SLOT INDEX AND THE RESULT -- they share $s1,
   which is what two disjoint ranges in one declared local do.
   0x80010A80 is a constant address (`lui`/`ori`, not `lui`/`addiu`).
   88 words, 0 differing. */
s32 func_800A6620(u8 *p, s32 f)
{
    u8 *q;
    u8 *base;
    u8 *e;
    s32 r;

    r = p[3] & 0x1F;
    q = (u8 *)D_800E3DF0[r];
    D_800E3DF0[r] = 0;
    r = q[0x43];
    if (r < 0x40)
    {
        q[0x44] = 0;
        func_800422DC((u8 *)(r * 0x54 + 0x80010A80), q);
    }
    if ((p[3] & 0x20) != 0)
    {
        r = (s16)func_800A1BD0(q);
        if (r >= 0)
        {
            base = D_800E3D7C;
            *(s32 *)(r * 4 + base + 0xAC) = 0;
            *(s32 *)(r * 4 + base + 0xD0) = 0;
        }
        if ((s16)func_80042900((s8 *)q, 0x1B) == 0)
        {
            e = *(u8 **)(q - 0x14);
            func_8009A3D0(e[0x24], e[0x25], (*(s32 *)(q + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        }
        if (f != 0)
        {
            func_800A32A4(q);
        }
        func_8009A028(q);
        *(u16 *)(q - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    func_80098B38(p);
    return r;
}

/* The "is the shop open" probe. ONE 0x80010000 BASE, shared by the two
   globals.

   AND THE FIRST READ MUST BE AN ARRAY REF, which is the whole fight. Spelled
   `*(s32 *)(b + 0x2090)` the MEM is not MEM_IN_STRUCT_P, so true_dependence
   keeps a dependence against the `sb` to D_80080A88 -- its second exemption
   needs the READ in-struct and the WRITE's address non-varying, and a
   LO_SUM's address does not vary -- the store stays pinned ahead of the load
   and the `beq` keeps a `nop`: 30 words, 28 differing. As `((s32 *)b)[0x824]`
   expr.c stamps the ARRAY_REF in-struct, the exemption fires, and reorg takes
   the store into the delay slot. A pointer local frees the slot but
   rematerialises the base, 30 / 25. 29 words, 0 differing. */
s32 func_800A6780(void)
{
    u8 *b;
    s32 v;
    s32 t;

    b = (u8 *)0x80010000;
    D_80080A88 = 0;
    v = ((s32 *)b)[0x824];
    if (v == 1)
    {
        D_80080A88 = 1;
        return 1;
    }
    if (v != 0)
    {
        return 0;
    }
    t = ((s32 *)b)[0x8D];
    if (t / 5 != (t + 1) / 5)
    {
        return 0;
    }
    D_80080A88 = 1;
    return 1;
}

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

/* Clear bit 0x400 on every entry of every descriptor's 0x14-stride table,
   then re-set it from the 19x8-byte bitmap at 0x80015720.

   THREE SPELLINGS. (1) The bitmap is a CONSTANT BASE, not the symbol: a
   register index never lets cse fold a symbol's `%lo` into the MEM, so the
   declared array spends an `addiu %lo` the target does not have (54 words).
   (2) The mask has to be a named `s32`: `*(u16 *)q &= ~0x400` is narrowed by
   the front end to `andi 0xFBFF` (3 differing), where the target keeps -0x401
   in a register. (3) Both invariants are assigned INSIDE the outer loop so
   loop.c hoists them; written above the loop they are preheader STATEMENTS
   and sit ahead of `j = 0`, 3 differing. 53 words, 0 differing. */
void func_800A6A9C(void)
{
    s32 j;
    s32 i;
    s32 v;
    s32 m;
    u8 *t;

    for (j = 0; j < 0x13; j++)
    {
        m = ~0x400;
        t = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            v = *(u16 *)(D_80073414[j].unk_0C + i * 0x14);
            v &= m;
            *(u16 *)(D_80073414[j].unk_0C + i * 0x14) = v;
            if ((t[j * 8 + i / 8 + 0x5720] >> (i & 7)) & 1)
            {
                *(u16 *)(D_80073414[j].unk_0C + i * 0x14) |= 0x400;
            }
        }
    }
    func_800A6A6C();
}

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

/* The 0x12-kind node for the dungeon's own HUD panel. `-r[3] + 8`, NOT
   `8 - r[3]`: the subtraction form materialises the 8 with an `li` and emits
   `subu $a2, $a2, $v0`, where the target negates the byte in place and adds
   -- 7 differing, all in those three words. 76 words, 0 differing. */
UnkNode *func_800A6C00(void)
{
    UnkNode *n;
    UnkVec *m;
    UnkDungeonSprite *s;
    u8 *q;
    u8 *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        m = n->unk_08;
        s = (UnkDungeonSprite *)n->unk_0C;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = 0x3C;
        *(s16 *)((u8 *)m + 0x6) = 0x40;
        *(s16 *)(q + 0x8) = 1;
        s->unk_1E = 0x1800;
        s->unk_1C = 0x1800;
        n->unk_10 = (u32)func_800A6B70;
        func_8004491C(n, (u32)func_800B06F0);
        r = (u8 *)n + 0x30;
        *(u32 *)q = (u32)((u8 *)n + 0xF0);
        *(u32 *)(q + 0x4) = (u32)((u8 *)n + 0xC0);
        *(s32 *)((u8 *)n + 0xF0) = func_8004DA74(r, D_80089074, 0) & 0x7FFFFFFF;
        func_800B1320(r, 0xA8, (s16)(-*(s8 *)(r + 0x3) + 8));
        func_800A6BBC(r, 4);
        func_800B135C(r, 8);
        func_800B13CC(r, 0x20);
        func_800B1400(r, 2);
        *(u32 *)((u8 *)n + 0xC0) = 0x808080;
        *(s32 *)((u8 *)n + 0xF4) = 0;
    }
    return n;
}

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

/* func_800A7030's sweep as a SEARCH: the first live slot on (x, y) whose
   +0x04 facing is within 0x40 of `ang`. The +0x04 halfword is read with `lh`
   through an explicit `(s16)` cast rather than through the D_800E36C8_u
   alias: a second `extern` under the same `__asm__` name would defeat cse's
   unification of the bases, and the target has one `addiu` for all three
   reads. 68 words, 0 differing. */
s16 func_800A70E4(s16 x, s16 y, s16 ang)
{
    s32 i;
    s32 d;
    u16 m;

    func_8009A350(x - 1, y, 0, &m);
    if ((m & 0x800) == 0)
    {
        return -1;
    }
    for (i = 0; i < 0x40; i++)
    {
        if (D_800E3548_s[i].unk_01 != 0 && D_800E36C8[i].unk_00 == x && D_800E36C8[i].unk_01 == y)
        {
            d = ang - (s16)D_800E36C8[i].unk_04;
            if (d < 0)
            {
                d = -d;
            }
            if (d < 0x40)
            {
                return i;
            }
        }
    }
    return -1;
}

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

/* Spawn the 0x212-style node through func_800A8608.

   THE THREE STACK PARAMETERS ARE `s32`. Declared s16 they are hoisted into
   $s0/$s1/$s2 at entry instead of being reloaded at their stores, and the
   function is 63 words against 59 with 60 differing -- the stack-parameter
   memory discount only applies while the parameter's mode is the slot's. AND
   THE RECORD READ NEEDS ITS OWN LOCAL: written inline at the +0x98 store the
   `lw` is scheduled eight words later, 59 / 9. 59 words, 0 differing. */
u8 *func_800A794C(s32 x, s32 z, s32 ang, u8 *p, s32 e, s32 f, s32 h)
{
    u8 *n;
    u8 *q;
    u8 *r;
    u8 *g;
    u8 *v;

    n = (u8 *)func_800A8608(D_80083498, p, 0, 0, 0);
    if (n == NULL)
    {
        return NULL;
    }
    *(u32 *)(n + 0x10) = (u32)func_800A77AC;
    q = *(u8 **)(n + 0x8);
    *(s16 *)(q + 0x2) = x * 64 + 0x20;
    *(s16 *)(q + 0x6) = z * 64 + 0x20;
    *(s16 *)(q + 0xA) = ang;
    r = n + 0x20;
    v = *(u8 **)p;
    *(s16 *)(r + 0xA0) = 4;
    *(s16 *)(r + 0xAA) = e;
    *(s16 *)(r + 0xAC) = f;
    *(u8 **)(r + 0x98) = v;
    *(s16 *)(r + 0xAE) = h;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
    return n;
}

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

/* The three halfword stores into the mover are the 16.16 integer halves at
   +0x2/+0x6/+0xA, which is why c, d and e are s16; no caller witnesses the
   width, so the `sh` stores are the whole evidence. The second parameter is
   `void *` and not `u8 *`: func_800A7700 passes its own `s32 *q`. This is
   also the witness for func_800BC26C's fourth parameter -- it sets all four
   argument registers before the `jal`. 53 words, 0 differing. */
UnkNode *func_800A8608(void *a, void *b, s16 c, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        *(s16 *)((u8 *)m + 0x2) = c;
        *(s16 *)((u8 *)m + 0x6) = d;
        *(s16 *)((u8 *)m + 0xA) = e;
        func_8004491C(n, (u32)D_80045340);
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0x1000;
        r->unk_1C = 0x1000;
        r->unk_08 = func_800A7A38(b);
        func_800BC26C(n, 0, 0, 0);
    }
    return n;
}

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

/* Rebuild the 0x13 x 8-byte occupancy bitmap in low RAM.

   THE BITMAP IS A CONSTANT BASE, NOT A SYMBOL: as `extern u8 D_80015720[]`
   the whole address is materialised and hoisted, the stores lose their
   displacement and the function is 53 words. A 2-D `[][8]` declaration
   reaches 52 but keeps the row base in a register, 38 differing. `b` IS
   ASSIGNED INSIDE THE OUTER LOOP so loop.c hoists it after `j = 0`; at
   function scope the `lui` is emitted first, 2 differing. AND THERE IS NO
   OFFSET VARIABLE: `o += 0x14` as a source local puts `o = 0` ahead of the
   loop guard, 9 differing. 52 words, 0 differing. */
void func_800A9160(void)
{
    u8 *b;
    s32 j;
    s32 i;

    for (j = 0; j < 0x13; j++)
    {
        b = (u8 *)0x80010000;
        for (i = 0; i < D_80073414[j].unk_02; i++)
        {
            if ((i & 7) == 0)
            {
                b[(j << 3) + i / 8 + 0x5720] = 0;
            }
            if (*(u16 *)(D_80073414[j].unk_0C + i * 0x14) & 0x400)
            {
                b[(j << 3) + i / 8 + 0x5720] |= 1 << (i & 7);
            }
        }
    }
}

/* The one-shot 0x20-kind prompt, and then a random D_800E3548 slot claim.
   THE RESULT IS A FOUR-BYTE STACK OBJECT, not a register value: it is
   zeroed as a word, filled two bytes at a time in one arm and replaced
   whole in the other, and taking its address is what keeps every access in
   memory.
   THE THREE SLOT TESTS MUST BE SEPARATE `if`s. Written
   `k != 0 && k < 0xB && k == 1` the front end's range folding collapses the
   first two into the third and the function is 84 words / 25 differing --
   exactly the `beqz` / `sltiu 0xB` / `beqz` triple and its load-delay nop
   missing. Nested, they survive.
   AND D_80012090 IS THE CONSTANT ADDRESS, as it is everywhere else in this
   unit: as the declared symbol the pair is `lui $v0` / `lw $v1, %lo($v0)`
   against the target's `lui $v1` / `lw $v1, %lo($v1)` -- 2 differing, and
   NOT hidden by the relocation mask. 88 words, 0 differing;
   `seqcheck` reports the two constant-address words with raw == relocdump
   at 20 == 20. */
s32 func_800A9230(u8 *p)
{
    UnkDungeonSlot4 s;
    s32 i;

    *(s32 *)&s = 0;
    if (p[0x13] == 0x20 && D_800E3CCC == 0)
    {
        D_800E3CCC = 1;
        if (*(s32 *)0x80012090 == 0)
        {
            if (D_8008146C == 0x19 && func_80033BC0(0x11F9) != 0 &&
                func_80033BC0(0x11FC) == 0 && (s16)func_800A697C(0xD, 2) == 0)
            {
                s.unk_00 = 2;
                s.unk_01 = 0xD;
                *(s16 *)(*(u8 **)(p - 0x14) + 0x12) = 0xC;
            }
        }
    }
    if (D_800DCF5A != 0 && p[0x49] == 0 && (func_800A6D30() & 3) == 0)
    {
        i = (u16)func_800A6DA4(1, 0x1F);
        if (D_800E3548_t[i].unk_01 != 0)
        {
            if (D_800E3548_t[i].unk_01 < 0xB)
            {
                if (D_800E3548_t[i].unk_01 == 1 && D_800E3548_t[i].unk_00 == 8)
                {
                    *(s32 *)&s = *(s32 *)&D_800E3548_t[i];
                    *(s32 *)&D_800E3548_t[i] = 0;
                }
            }
        }
    }
    return *(s32 *)&s;
}

/* THE LOCAL IS `s16`. The target copies the `lbu` result into a second
   register before the three compares; `u8 v` makes that copy an
   `andi $v1, $a0, 0xFF` (1 differing) and `s32 v` deletes it altogether,
   loading straight into the compare register (27 of 28 words, 19 differing).
   Only the widening copy of a NARROW local is a bare `addu`. This is the
   function the c6 batch left at 1 differing after twenty-one declarations;
   `s16` was not among them. 28 words, 0 differing. */
s16 func_800A9390(s16 idx)
{
    s16 v;

    v = D_800DD8E7[idx];
    if ((*(u8 **)(D_800E3D7C + 0x4C))[1] != 0)
    {
        if (v == 0x32)
        {
            return 0x33;
        }
        if (v == 0x39)
        {
            return 0x3A;
        }
        if (v == 0x40)
        {
            return 0x42;
        }
    }
    return v;
}

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

/* The per-frame tick for an actor's 0x6D countdown. TWO SPELLINGS COST TWO
   WORDS EACH. 0x80013714 has to be the CONSTANT ADDRESS: as structs.h's
   symbol the `high` is a separate insn, and because the use is inside the
   loop, loop.c hoists it into a callee-saved register -- 62 words against 60,
   the two being $s2's save and restore. And the 0x2F bound is UNSIGNED: an
   s32 local gives `slti` where the target has `sltiu`. 60 words,
   0 differing. */
void func_800A9A0C(u8 *p)
{
    u32 v;

    p[0x71] &= 0x7F;
    if ((D_80083462 & 0x2800) != 0)
    {
        return;
    }
    if (*(s8 *)(p + 0x6D) != 0)
    {
        do
        {
            if ((*(u16 *)0x80013714 & 0x8) == 0)
            {
                if ((*(u32 *)(p + 0x1C) & 0x8) == 0)
                {
                    v = p[0x13];
                    if (v < 0x2F || v == 0x39)
                    {
                        func_800AD594(p, 4);
                    }
                }
                func_800A4ACC(p);
            }
            *(s8 *)(p + 0x6D) -= 1;
        } while (*(s8 *)(p + 0x6D) != 0);
    }
    *(u16 *)(p + 0x46) &= 0x7FFF;
    func_800AA53C(p);
}

/* Push the actor's grid square onto the record's +0x73/+0x7B queue.

   THREE THINGS THE LISTING FORCES. `(s16)func_80042900(...)` assigned to an
   `s32` puts the `sll`/`sra` pair at the ASSIGNMENT, which is what makes the
   two `bnez` test the sign-extended $s4; an s16 local narrows at the use
   instead (70 words / 26) and dropping the cast is 68 / 54. The whole FLAG
   WORD is hoisted into a local before the two byte arguments -- hoisting the
   already-MASKED boolean instead measures identically to not hoisting at all
   (4 differing), because what decides sched1's order is which load's
   statement comes first, not where the mask is. And the queue stores are
   written base, index, constant last: the subscript form folds the constant
   in first and leaves the index as the `addu`'s left operand, 2 differing.
   69 words, 0 differing. */
void func_800A9AFC(u8 *a, u8 *b)
{
    s32 t;
    s32 u;
    s32 x;
    s32 y;
    s32 x2;
    s32 y2;
    s32 f;

    if ((b[0x71] & 0x7F) != 0)
    {
        t = (s16)func_80042900((s8 *)b, 0x1B);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A3D0(x2, y2, u);
        }
        x = b[0x74];
        y = b[0x7C];
        func_80069F28(b + 0x74, b + 0x75, 0xF);
        *(b + b[0x71] + 0x73) = a[0x24];
        *(b + b[0x71] + 0x7B) = a[0x25];
        a[0x24] = x;
        a[0x25] = y;
        *(s16 *)(b + 0x88) = *(u16 *)(b + 0x8A);
        *(s16 *)(b + 0x2A) = *(u16 *)(b + 0x6A);
        if (t == 0)
        {
            f = *(s32 *)(b + 0x1C);
            x2 = a[0x24];
            y2 = a[0x25];
            if (f & 0x2000)
            {
                u = 0x300;
            }
            else
            {
                u = 0x3000;
            }
            func_8009A21C(x2, y2, u);
        }
        *(s16 *)(b + 0x8A) = 0;
    }
}

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

/* State 0xA, func_800AA888's installer one state over. THE MODE IS A `u16`
   LOCAL, not the s32 func_800AA888 uses: it is live across both reports, so
   it needs a callee-saved register, and the `andi $s0, 0xFFFF` at the join is
   what the narrow type buys. The sine/cosine tables are read a BYTE at a time
   at a doubled index, which is what gives `lbu` rather than the `lh` an s16
   subscript would. 69 words, 0 differing. */
void func_800AA258(u8 *p, void *b, u8 *c, u8 *d)
{
    u16 t;
    s32 i;
    u8 *g;

    p[0x9A] = 0xA;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (*(s32 *)(d + 0x1C) & 0x2000)
    {
        t = 0x300;
    }
    else
    {
        t = 0x3000;
    }
    func_8009A3D0(c[0x24], c[0x25], t);
    i = (*(u16 *)(d + 0x6A) >> 9) & 7;
    c[0x24] += ((u8 *)D_8006CCD8)[i * 2];
    c[0x25] += ((u8 *)D_8006CCE8)[i * 2];
    func_8009A21C(c[0x24], c[0x25], t);
    *(s32 *)(d + 0x1C) &= ~0x100;
    *(s16 *)(p + 0x96) = 0x21;
    *(u16 *)(p + 0x98) |= 0x8;
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) += 1;
}

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

/* The actor's "give up and go back to state 5" transition. The call to
   func_800ACB98 forwards all four of this function's own parameters, which is
   why the `jal` has a bare `nop` delay slot and no argument setup.
   59 words, 0 differing. */
s32 func_800AA79C(UnkDungeonActor *p, void *b, u8 *c, u8 *d)
{
    u8 *g;

    d[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if ((*(u16 *)(g + 0x2) & 0x2008) != 0)
    {
        p->unk_9A = 0xE;
        return 0;
    }
    if ((*(u32 *)(d + 0x14) & 0x20000000) != 0)
    {
        func_800ACB98((u8 *)p, b, c, d);
        return 1;
    }
    if ((*(u32 *)(d + 0x1C) & 0x8) == 0)
    {
        *(u16 *)(g + 0xA) -= 1;
    }
    p->unk_9A = 5;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(u16 *)((u8 *)p + 0x98) &= ~0x8;
    *(s16 *)(d + 0x24) = 0;
    *(u16 *)(g + 0xA) += 1;
    *(u32 *)(d + 0x1C) = (*(u32 *)(d + 0x1C) | 0x8) & ~0x40000;
    *(u16 *)(c + 0x14) |= 0x6000;
    return 1;
}

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

/* Installer: two refusals into state 0xE, else state 0xD with the facing
   pushed through func_80047784. 65 words, 0 differing. */
s32 func_800AA924(u8 *p, void *b, u8 *c, u8 *t)
{
    u8 *g;
    s32 v;

    p[0x71] &= 0x7F;
    g = (u8 *)&D_80083460;
    if (*(u16 *)(g + 0x2) & 0x2008)
    {
        p[0x9A] = 0xE;
        return 1;
    }
    if (*(s32 *)(p + 0x1C) & 0x20)
    {
        p[0x9A] = 0xE;
        *(s32 *)(p + 0x1C) &= ~0x200;
        return 0;
    }
    p[0x9A] = 0xD;
    p[0x9B] = 0;
    *(s32 *)(p + 0x8C) = 0;
    if (t != NULL)
    {
        *(u8 **)(c + 0x2C) = t;
        v = ((D_80083228 + *(s16 *)(p + 0x2A) + 0x100) >> 9) & 7;
        func_80047784(c, t[v], 0);
    }
    *(u16 *)(g + 0xA) += 1;
    *(s32 *)(p + 0x1C) &= ~0x40000;
    return 1;
}

/* func_80095DD0 on the SECOND argument and without the parked pointer.
   11 words, 0 differing. */
/* TWO parameters, not four. func_800AD9B4 is this function's only reference
   anywhere in the overlay -- no other `jal`, no address-take -- and it passes
   two. The four-parameter signature was the handler convention applied to a
   function nothing dispatches; narrowed, this still measures 11 words,
   0 differing. */
void func_800AAA28(void *a, u8 *p)
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

/* WRITTEN K&R ON PURPOSE. The body reads all four argument registers, but the
   three landed installers that call it (func_800B2C68, func_800D45D0,
   func_800D9C18) are PURE FORWARDS written with no arguments at all, so a
   prototyped four-parameter definition is `too few arguments to function`
   three times over. clang-format wants the first parameter declaration at
   COLUMN ZERO here -- an `s32` return with a pointer first parameter -- not
   indented as the K&R note in docs/town-sweep.md says. 59 words,
   0 differing. */
s32 func_800AB378(p, b, c, d)
u8 *p;
UnkVec *b;
u8 *c;
u8 *d;
{
    u8 *g;
    u8 *q;
    s16 t;

    if ((*(u16 *)(c + 0x14) & 0x8000) != 0)
    {
        *(s16 *)(p + 0x96) = 0;
    }
    if (*(s16 *)(p + 0x96) != 0)
    {
        q = *(u8 **)(*(u8 **)(d + 0x60) - 0x18);
        b->unk_0C = *(s32 *)(q + 0xC);
        b->unk_10 = *(s32 *)(q + 0x10);
    }
    t = *(u16 *)(p + 0x96) - 1;
    *(s16 *)(p + 0x96) = t;
    if (t <= 0)
    {
        b->unk_14 = 0;
        b->unk_10 = 0;
        b->unk_0C = 0;
        func_800A2B04(b, c[0x24], c[0x25]);
        *(u16 *)(p + 0x98) &= 0xFFF7;
        *(u32 *)(d + 0x1C) |= 0x40000000;
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        return 1;
    }
    return 0;
}

/* The `blez` installer gate. MUST STAY K&R, like func_800AC7EC above:
   func_800D491C and func_800DA7D0 call this with NO arguments.

   THE RESULT LOCAL IS `s32` WITH AN EXPLICIT `(s16)` AT BOTH USES. As `s16 r`
   the test's `sll` takes the RAW call result instead of the copy in $v1 -- 53
   words, 3 differing -- and `return r` without the cast is 52 / 14. The big
   arm must also be written FIRST: with the `r = 1` arm first the `bnez`
   inverts and the layout costs 32 differing.

   func_800ADAE4 is deliberately NOT declared at the top of this unit: it is
   landed below as s16, and with that prototype in scope gcc drops both casts
   and the function is 52 words / 16 differing. Widening the landed definition
   to s32 breaks func_800ADAE4 itself (42 words / 12). The implicit `int`
   declaration this call site rides is the only resolution, and it costs three
   warnings of the class this unit already carries. 53 words, 0 differing. */
s32 func_800AB464(a, b, c, p)
UnkDungeonActor *a;
UnkVec *b;
u8 *c;
u8 *p;
{
    s32 r;
    u8 *g;

    if (a->unk_9B == 0)
    {
        p[0x71] &= 0x7F;
        g = (u8 *)&D_80083460;
        if (*(u16 *)(g + 0x2) & 0x2000)
        {
            return 0;
        }
        if ((s16)func_800A2B5C(p) != 0)
        {
            return 0;
        }
        r = func_800ADAE4(c, p);
        if ((s16)r != 0)
        {
            *(s32 *)(g + 0xC) = 0;
            *(u16 *)(g + 0x14) -= 1;
            a->unk_9B = 1;
        }
    }
    else
    {
        r = 1;
    }
    return (s16)r;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB538);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AB778);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ABDBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC010);

/* Glide the mover towards the tile record's centre over the +0x96 countdown,
   then -- on the frame it reaches zero -- stop it dead, re-place it on the
   tile, and hand the leash at +0x88 back to the view. 82 words, 0 differing.

   THE 0x20 IN THE FIRST NUMERATOR IS A VARIABLE, and that is the whole
   function. Both halves want `(byte << 6) - (halfword - 0x20)`, which fold
   reassociates into `(byte << 6) - halfword + 0x20` unless the subtrahend is
   its own statement -- and with the statement the two equal-depth chains of
   the FIRST numerator schedule the other way round: `lh 0x2($a0)` before
   `lbu 0x24($s0)`, four words differing, with every other word in the
   function exact. rank_for_schedule has nothing left to separate them but
   INSN_LUID, and no source order reaches it: thirty-three spellings were
   measured, including the byte hoisted into a local of either type, two
   numerator locals, either store as a cast, `* 0x10000` for `<< 16`, and an
   empty `__asm__ volatile("")` between the chains (84 words) or before them
   (no change). Holding the 0x20 in a local instead blocks the reassociation
   without a statement boundary, and the chains come out in the target's
   order. The second numerator keeps the ordinary spelling; it was never
   wrong. decomp-permuter found this in two minutes after the hand sweep had
   run out. */
s32 func_800AC480(u8 *p, UnkVec *v, u8 *r, u8 *b)
{
    s16 n;
    s16 d;
    s32 w;
    u8 *g;

    n = *(s16 *)(p + 0x96);
    if (n != 0)
    {
        w = 0x20;
        v->unk_0C = (((r[0x24] << 6) - (*(s16 *)((u8 *)v + 0x2) - w)) << 16) / n;
        w = *(s16 *)((u8 *)v + 0x6) - 0x20;
        v->unk_10 = (((r[0x25] << 6) - w) << 16) / *(s16 *)(p + 0x96);
    }
    if ((*(s16 *)(p + 0x96) -= 1) > 0)
    {
        return 0;
    }
    v->unk_14 = 0;
    v->unk_10 = 0;
    v->unk_0C = 0;
    func_800A2B04(v, r[0x24], r[0x25]);
    d = func_800BCB04((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(u16 *)(b + 0x88) - 0x20);
    if (d < 0x200)
    {
        *(s16 *)(p + 0x92) += *(u16 *)(b + 0x88) - d;
        *(s16 *)(b + 0x88) = d;
    }
    g = (u8 *)&D_80083460;
    *(s16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The fade handler behind func_800B3254 and func_800DA798. Case 0 eases the
   three +0xC..+0xE bytes towards 0x80 by (0x80 - v) / ++r[0xF]; the divisor is
   re-read FIVE times because each `sb` through the `u8 *` wipes cse's memory
   table and the next `lbu` is a zero-extend the SImode store cannot be
   forwarded into. Case 1 unlinks the node and returns 1.

   FOUR PARAMETERS, FORWARDED: func_800B3254 and func_800DA798 called this
   with NO arguments while it was a stub, exactly as their func_800AC480
   siblings did before that function was read. Both are widened in the same
   change and both stay 14 words / 0 differing. 85 words, 0 differing on the
   first compile. */
s32 func_800ACE34(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    UnkNode *n;
    u8 *g;

    switch (p[0x9B])
    {
    case 0:
        c[0xF] += 1;
        c[0xC] += (0x80 - c[0xC]) / c[0xF];
        c[0xD] += (0x80 - c[0xD]) / c[0xF];
        c[0xE] += (0x80 - c[0xE]) / c[0xF];
        if (c[0xF] >= 8)
        {
            c[0xE] = 0x80;
            c[0xD] = 0x80;
            c[0xC] = 0x80;
            p[0x9B] += 1;
        }
        break;
    case 1:
        if (--c[0xF] == 0)
        {
            n = (UnkNode *)(p - 0x20);
            func_80044A50(n);
            *(u16 *)(c + 0x12) += 0x80;
            *(u16 *)(c + 0x14) &= 0xFFF3;
            func_8004491C(n, (u32)D_80045340);
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
            return 1;
        }
        break;
    }
    return 0;
}

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
/* NOT `s16`: the return is already a `lh`, so the width costs this function
   nothing -- but an s16 return makes every caller sign-extend $v0 before
   testing it, which is a word func_800BD1C4 does not have (167 against 166).
   That also says the two were in different translation units: the caller saw
   no prototype at all. */
s32 func_800AD568(u8 *p)
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

/* Can the actor step onto the tile the view names? THE func_800B500C RESULT
   IS `s16`: through an s32 local the `sll 16`/`sra 16` pair in front of the
   `bltz` disappears, 75 words / 41 differing. func_800AAA28's signature was
   narrowed to two parameters on this function's evidence -- see its comment.
   76 words, 0 differing. */
s32 func_800AD9B4(u8 *p, u8 *q)
{
    u8 *g;
    u8 *h;
    s16 i;
    s32 d;

    if (*(u16 *)(q - 0x2) & 0x8000)
    {
        return 1;
    }
    g = D_80082E80;
    if (*(s8 *)(g + 0x26) != *(s8 *)(p + 0x26) && func_8009FD40(g, p) >= 7)
    {
        return 1;
    }
    if (q[0x25] == 0)
    {
        return 1;
    }
    i = func_800B500C(p[0x24], p[0x25], *(s16 *)(q + 0x88));
    if (i < 0)
    {
        return 1;
    }
    if ((D_800E3648_s[i].unk_03 & 0x80) == 0)
    {
        return 1;
    }
    h = (u8 *)&D_80083460;
    if (*(u16 *)(h + 0x2) & 0x1000)
    {
        d = *(s8 *)(q + 0x71);
        if (d > 0)
        {
            *(u16 *)(h + 0x8) -= d - *(u16 *)(q + 0x8A);
            q[0x71] = 0;
        }
    }
    func_800A9A0C(q);
    func_800AAA28(p, q);
    return 0;
}

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

/* The 0x12 spawner. No caller anywhere in the project, so the arity is read
   off the four argument registers alone. 53 words, 0 differing. */
UnkNode *func_800ADC4C(u8 *a, s32 b, s16 c, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800ADB8C;
        func_8004491C(n, (u32)func_800ADBCC);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(a + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(a + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(a + 0xA);
        r = (u8 *)n->unk_0C;
        *(s32 *)(r + 0x8) = b;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)((u8 *)n + 0x28) = c;
        *(u8 **)((u8 *)n + 0x20) = d;
        *d = 0;
    }
    return n;
}

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

/* Two narrow-type facts, each worth one word. (1) The result local is `s32`
   with `(s16)` casts at its uses, not an `s16` local: the target's first
   `sll $v0, $s1, 16` reads the CALLEE-SAVED copy, and an s16 local lets cse
   substitute the return register back into the extension. (2) func_8009A180's
   result is tested as a HALFWORD -- without the cast the test is a bare
   `beqz` and the function is 52 words / 21 differing. 53 words,
   0 differing. */
s16 func_800ADDA0(void *a, void *b, u8 *c, s16 d, s16 e, s32 f)
{
    u8 *g;
    s32 r;

    r = func_800ADE74(a, b, c, d, e, f);
    if ((s16)r != 0 && (s16)r != 2 && (s16)r != 3)
    {
        g = (u8 *)&D_80083460;
        if (*(s16 *)(g + 0x8) != 0 || (*(u16 *)(g + 0x2) & 0x2808) != 0)
        {
            if ((s16)func_8009A180(c, *(u8 **)(D_800814A8 + 0x58) + 0x20) != 0)
            {
                *(u16 *)(c + 0x46) &= 0x7FFF;
                return -1;
            }
        }
    }
    return r;
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089074);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089080);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADE74);

/* The four-state step of the 0x800AFAD8 object. The `% 7168` tail is written
   in BOTH arms and jump.c cross-jumps it, which is the single `mfhi`/`subu`
   run the two cases share. 73 words, 0 differing. */
void func_800AFAD8(u8 *p)
{
    s16 v;

    switch (*(s16 *)(p + 0x2))
    {
    case 0:
        *(u16 *)(p + 0x6) += 2;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 1:
        *(u16 *)(p + 0x6) += 4;
        *(u16 *)(p + 0x0) += 1;
        break;
    case 2:
        v = *(u16 *)(p + 0x8) + 0x40;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    case 3:
        *(u16 *)(p + 0x6) += 8;
        v = (func_800644B8(*(s16 *)(p + 0x0) << 5) >> 1) + 0xC00;
        *(u16 *)(p + 0x0) += 1;
        *(s16 *)(p + 0x8) = v % 7168;
        break;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFBFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFFB4);

/* Open the kind-0x12 panel for corner `k`. THE RENDERABLE POINTER IS RE-READ
   PER GROUP, and the source says so by assigning the local three times: the
   +0x8 store, the three colour bytes and the two scales are each one group
   off their own `lw 0xC($s0)`. Written inline per field every store reloads
   and the function is 74 words / 46 differing; with ONE assignment it would
   be one load for all six. The three colour bytes go 0xE, 0xD, 0xC and the
   two scales 0x1E, 0x1C -- descending in both groups. The mover at +0x08 IS
   written inline, which is where its three reloads come from. 68 words,
   0 differing. */
void func_800B0544(s16 k)
{
    UnkNode *n;
    UnkDungeonSprite *s;
    u8 *w;
    s32 v;

    if (D_800DCF4C != k)
    {
        func_8003E1C4(k);
        D_800DCF4C = k;
    }
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800AFAD8;
        func_8004491C(n, (u32)func_800AFBFC);
        s = (UnkDungeonSprite *)n->unk_0C;
        *(u8 **)((u8 *)s + 0x8) = D_800DEF90;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_0E = 0xE0;
        s->unk_0D = 0xE0;
        s->unk_0C = 0xE0;
        s = (UnkDungeonSprite *)n->unk_0C;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0x6) = 0;
        *(s16 *)((u8 *)n->unk_08 + 0xA) = 0x4000;
        v = *(s32 *)0x80012090;
        w = (u8 *)n + 0x20;
        if (v == 0)
        {
            if (D_8008146C == 0x28)
            {
                *(s16 *)(w + 0x2) = 1;
            }
        }
        else if (v == 2)
        {
            *(s16 *)(w + 0x2) = v;
        }
    }
}

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

/* Build one POLY_FT4 (0x28 bytes) for the 0xC-byte sprite descriptor at *c
   and hand it to func_8006658C, then step the cursor or clear it when the
   descriptor's first byte is negative.

   THE CURSOR IS ADVANCED BEFORE THE TAIL `if`, not in the `return`:
   `return q + 0x28;` is 84 words / 12 differing, because the target has
   `addiu $s1, $s1, 0x28` in the `bgez`'s delay slot and then a bare
   `addu $v0, $s1, $zero`. The four UV pairs are CHAINED assignments
   (`q[0xC] = q[0x1C] = ...`), which is what gives the target's
   high-offset-then-low store order off one computed value; `*c` is re-read
   once per group because the previous group's stores invalidate it.
   85 words, 0 differing. */
u8 *func_800B0BE0(u8 *a, u8 *b, u8 **c, u8 *q)
{
    s32 t;

    *(u32 *)(q + 0x4) = *(u32 *)(b + 0xC);
    func_800666F4(q);
    *(s16 *)(q + 0xE) = *(u16 *)(*c + 0x6);
    t = *(u16 *)(b + 0x10);
    if (t != 0)
    {
        *(s16 *)(q + 0x16) = t + (*(u16 *)(*c + 0x4) & 0xFF9F);
    }
    else
    {
        *(s16 *)(q + 0x16) = *(u16 *)(*c + 0x4);
    }
    q[0xC] = q[0x1C] = (*c)[0x8];
    q[0x14] = q[0x24] = (*c)[0x8] + (*c)[0xA] - 1;
    q[0xD] = q[0x15] = (*c)[0x9];
    q[0x1D] = q[0x25] = (*c)[0x9] + (*c)[0xB] - 1;
    func_8006658C(a, q);
    q += 0x28;
    if (*(s8 *)*c < 0)
    {
        *c = NULL;
    }
    else
    {
        *c = *c + 0xC;
    }
    return q;
}

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

/* The node's +0x10 step for func_800B2074's marker: a four-state machine on
   the view's +0x04. TWO FALL-THROUGHS ARE REAL -- state 0 falls into state 1,
   and state 2 falls into state 3. Four cases plus a default is under
   CASE_VALUES_THRESHOLD, so this is a comparison tree and not a jump table.
   80 words, 0 differing. */
void func_800B1F34(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    switch (*(s16 *)(p + 0x4))
    {
    case 0:
        *(s16 *)(p + 0x6) = 0x10;
        *(s16 *)(p + 0x4) += 1;
    case 1:
        b->unk_14 -= 0x4000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x40;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    case 2:
        b->unk_14 += 0x6000;
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
    case 3:
        t = *(u16 *)(p + 0x6) - 1;
        *(s16 *)(p + 0x6) = t;
        if (t < 0)
        {
            b->unk_14 = 0;
            *(s16 *)(p + 0x6) = 0x3C;
            *(s16 *)(p + 0x4) += 1;
        }
        break;
    }
    b->unk_08 -= b->unk_14;
    *(u32 *)(D_800814A8 + 0x1C) |= 0x40000000;
    **(s16 **)p = 0x20 - *(u16 *)((u8 *)b + 0xA);
}

/* Spawn the marker node.

   THE TWO COORDINATES ARE `s32`, not s16. The cell index is written
   `(x << 10) >> 16`; with s16 parameters gcc folds the entry sign-extension
   into the pair and emits `sll 16`/`sra 22` instead of the target's
   `sll 10`/`sra 16` -- 7 differing, with no other difference.

   THE VIEW POINTER MUST BE BOUND BEFORE ITS USE, not at it. Assigned on the
   statement that uses it, `(u8 *)n + 0x20` folds into the store's
   displacement (3 differing); bound at the top of the `if` it survives as the
   target's own `addiu $v0, $s1, 0x20`, even though update_equiv_regs still
   moves the set down to the use. 81 words, 0 differing. */
void func_800B2074(s32 x, s32 y)
{
    UnkNode *n;
    u8 *g;
    u8 *r;
    u8 *v;
    UnkVec *m;
    UnkSlot *s;

    g = (u8 *)&D_80083160;
    r = *(u8 **)(g + 0x1DC);
    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B1F34;
        func_8004491C(n, (u32)D_80046398);
        s = (UnkSlot *)n->unk_0C;
        s->unk_20 = 0x1000;
        s->unk_1E = 0x1000;
        s->unk_1C = 0x1000;
        s->unk_08 = 2;
        *(s32 *)((u8 *)s + 0xC) = 0x808080;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x16) = 6;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        r += (((x << 10) >> 16) + (((y << 10) >> 16) << *(s16 *)(g + 0x1F0))) * 6;
        *(u8 **)((u8 *)n + 0x20) = r + 2;
        *(s16 *)r = 3;
        *(s16 *)((u8 *)m + 0xA) = -*(u16 *)(*(u8 **)((u8 *)n + 0x20));
        *(u16 *)(r + 0x2) += 0x20;
        *(u16 *)(r + 0x4) |= 1;
        func_800A56E0(0x603);
        *(s16 *)(v + 0xE) = *(u16 *)(D_800814A8 + 0x2A);
    }
}

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

/* The spawner tail func_800D1C70 shares: stamp the record's room id, place
   the mover, measure the leash into the view's +0x88, dress the renderer,
   light the node up and hang a 0x110 marker node off it. FOUR PARAMETERS --
   the fourth is never read here, but func_800B23F8 sign-extends $a3 into
   the `jal`'s delay slot. 66 words, 0 differing, first compile. */
void func_800B22F0(UnkNode *n, UnkVec *m, UnkSlot *r, s16 d)
{
    u8 *p;

    p = (u8 *)n + 0x20;
    *((u8 *)r + 0x26) = func_8009FB34(*((u8 *)r + 0x24), *((u8 *)r + 0x25));
    func_800A2B04(m, *((u8 *)r + 0x24), *((u8 *)r + 0x25));
    *(s16 *)(p + 0x88) = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                                       *(u16 *)((u8 *)m + 0xA) - 0x20);
    r->unk_1E = 0x1000;
    r->unk_1C = 0x1000;
    *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
    r->unk_14 |= 0x8000;
    func_80099FDC(n);
    func_8009A21C(*((u8 *)r + 0x24), *((u8 *)r + 0x25),
                  (*(u32 *)(p + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    p[0xAD] = 1;
    p[0x71] = 0;
    *(u32 *)(p + 0x1C) |= 0x40000000;
    func_800BC26C(n, 0, (s32)((u8 *)r + 0x2C), (s32)((u8 *)n + 0x4A));
}

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

/* func_800B30E4 behind the renderable's 0xE000 gate and with func_800A2DB8
   ahead of the teardown. 48 words, 0 differing. */
void func_800B2FAC(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    if (*(u16 *)(c + 0x14) & 0xE000)
    {
        g = (u8 *)&D_80083460;
        if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
        {
            *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
        }
        func_800A2DB8(d);
        func_800A32A4(d);
        func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
        func_8009A028(d);
        *(u16 *)(d - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* The +0x8C step that tears an actor down. THE 0x300 / 0x3000 CHOICE IS A
   TERNARY IN THE CALL, not an if/else around two calls: the target sets
   $a0/$a1 above the branch and only $a2 inside it. 42 words, 0 differing. */
void func_800B30E4(u8 *a, void *b, u8 *c, u8 *d)
{
    u8 *g;

    g = (u8 *)&D_80083460;
    if (*(s32 *)(g + 0x10) == (s32)(d - 0x20))
    {
        *(s32 *)(g + 0x10) &= 0x7FFFFFFF;
    }
    func_800A32A4(d);
    func_8009A3D0(c[0x24], c[0x25], (*(s32 *)(d + 0x1C) & 0x2000) ? 0x300 : 0x3000);
    func_8009A028(d);
    *(u16 *)(d - 0x2) |= 0x8000;
    D_800814A0 |= 0x8000;
}

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

/* FOUR PARAMETERS, forwarded. func_800AC480 reads all four argument
   registers; this caller only passes them through, so the wrong arity was
   invisible until that function was read. 14 words, 0 differing either way. */
void func_800B31E4(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AC480((u8 *)p, b, c, d) != 0)
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

/* FOUR PARAMETERS, forwarded, exactly as the func_800AC480 pair next door:
   func_800ACE34 reads $a0 and $a2 and this caller only passes them through.
   14 words, 0 differing either way. */
void func_800B3254(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800ACE34((u8 *)p, b, c, d) != 0)
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

/* Find the spot whose tile is (x, y) and whose +0x12 facing is within 0x40
   of c, over the same 0x20-entry pair of tables func_800B6008 sweeps.
   71 words, 0 differing.

   TWO THINGS WERE MEASURED HERE. (1) The +0x12 field is READ WITH `lh`, so
   UnkDungeonSpotB.unk_12 is `s16`; spelled `*(s16 *)&...unk_12` off the u16
   field it is a SECOND induction variable biased by 0x12 -- 74 words against
   71, the extra `addiu $a3, $v0, 0x12` / `addiu $a3, $a3, 0x18` pair plus a
   load-delay nop. Both existing users of the field only STORE through it and
   both still measure 0 differing. (2) The 0x20 mask test is a NESTED `if`
   around the loop, not a third early `return -1`: as an early return it gets
   its own `j`/`addiu -1` pair instead of sharing the tail's, and the first
   return then collapses into a direct `beqz` -- 11 differing, all layout. */
s16 func_800B500C(s16 x, s16 y, s16 c)
{
    s32 i;
    s32 d;
    u16 m;

    if (func_8009A350(x - 1, y, 0, &m) == 0)
    {
        return -1;
    }
    if (m & 0x20)
    {
        for (i = 0; i < 0x20; i++)
        {
            if (D_800E3648_s[i].unk_01 != 0 && D_800E39C8[i].unk_06 == x &&
                D_800E39C8[i].unk_07 == y)
            {
                d = c - D_800E39C8[i].unk_12;
                d = abs(d);
                if (d < 0x40)
                {
                    return i;
                }
            }
        }
    }
    return -1;
}

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

/* Dispatch to the per-kind hatch in D_800DF2A8, and on a non-zero answer
   tear the old view down (only while D_800E3D40 survived the hatch) and
   re-dress the spot's renderer from D_800DF258 when its +0x14 carries 0x40.
   The +0x14 flags word is a field of UnkDungeonSpotB added for this; the
   +0x24-stride byte beside it is the flat `D_800E3648[idx * 4]` spelling,
   which is what gives the `sll 2` / `addu` / `lbu 0` the target has.
   75 words, 0 differing, first compile. */
s32 func_800B627C(s16 kind, u8 *b, s16 idx, u8 flag)
{
    s32 r;
    u8 *g;

    D_800E3D40 = flag;
    r = D_800DF2A8[kind](b, idx);
    if (r != 0)
    {
        if (D_800E3D40 != 0)
        {
            func_80099F70(*(u8 **)(b + 0x5C));
            func_80099F04(*(u8 **)(b + 0x5C));
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0x2) |= 0x812;
        }
        if (D_800E39C8[idx].unk_14 & 0x40)
        {
            func_8003DB94(&D_800E39C8[idx], D_800DF258[D_800E3648[idx * 4]], 0);
            D_800E39C8[idx].unk_14 &= 0xF7FF;
        }
    }
    return r;
}

/* A particle's per-frame step: accelerate the three deltas from the record's
   own three constants (the third scaled by the frame counter), advance the
   position by them, then bleed the renderable's three colour bytes by the
   record's three rates, clamping each at 0x80 when it wraps. 101 words,
   0 differing on the first compile.

   The clamp is `if (c[N] > 0x80) c[N] = 0x80;` on a `u8` field, which is the
   `andi 0xFF` / `sltiu 0x81` / `bnez` triple -- the mask is on the register
   cse forwarded from the `sb`, not a reload. */
void func_800B63A8(u8 *p, UnkVec *v, u8 *c)
{
    s16 t;

    v->unk_0C += *(s32 *)p;
    v->unk_10 += *(s32 *)(p + 0x4);
    v->unk_14 += *(s32 *)(p + 0x8) * *(s16 *)(p + 0x10);
    *(s16 *)(p + 0x10) = *(u16 *)(p + 0x10) + 1;
    v->unk_00 += v->unk_0C;
    v->unk_04 += v->unk_10;
    v->unk_08 += v->unk_14;
    func_800478B8(c);
    if (c[0xC] != 0)
    {
        c[0xC] -= p[0x14];
        if (c[0xC] > 0x80)
        {
            c[0xC] = 0x80;
        }
    }
    if (c[0xD] != 0)
    {
        c[0xD] -= p[0x16];
        if (c[0xD] > 0x80)
        {
            c[0xD] = 0x80;
        }
    }
    if (c[0xE] != 0)
    {
        c[0xE] -= p[0x18];
        if (c[0xE] > 0x80)
        {
            c[0xE] = 0x80;
        }
    }
    t = *(u16 *)(p + 0xE) - 1;
    *(s16 *)(p + 0xE) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

void func_800B653C(u8 *t, u16 ang)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *v;
    s32 i;

    i = (ang >> 9) & 7;
    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        m = n->unk_08;
        r = (u8 *)n->unk_0C;
        v = (u8 *)n + 0x20;
        n->unk_10 = (u32)func_800B63A8;
        func_8004491C(n, (u32)D_80045340);
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(t + 0x2) - D_8006CCD8[i] * 16;
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(t + 0x6) - D_8006CCE8[i] * 16;
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(t + 0xA);
        m->unk_0C = -(*(s32 *)(t + 0xC) * ((func_80069EF8() & 1) + 2));
        m->unk_10 = -(*(s32 *)(t + 0x10) * ((func_80069EF8() & 1) + 2));
        m->unk_14 = (~func_80069EF8() & 1) << 15;
        func_8003DB94(r, D_800DEA68, 0);
        r[0xE] = 0x80;
        r[0xD] = 0x80;
        r[0xC] = 0x80;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        *(s16 *)(r + 0x10) = 0x60;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(v + 0x18) = 0x10;
        *(s16 *)(v + 0x16) = 0x10;
        *(s16 *)(v + 0x14) = 0x10;
        *(s16 *)(v + 0xE) = (func_80069EF8() & 3) | 4;
        *(s32 *)(v + 0x8) = 0x800;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B66C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6814);

/* The +0x64 halfword is read ONCE into an s32 local and tested twice (`bltz`
   then `blez`), and the `&= 0xF7FF` is written out in both arms --
   cross-jumping is what keeps that one copy. 58 words, 0 differing. */
void func_800B69DC(u8 *p, UnkVec *b, u8 *c, u8 *d)
{
    s32 t;

    t = *(s16 *)(d + 0x64);
    if (t < 0 || (*(u16 *)(p + 0x10C) & 1) != 0)
    {
        *(u16 *)(c + 0x14) &= 0xF7FF;
        func_8008CAA0(p, b, c, d);
    }
    else
    {
        if (t > 0)
        {
            func_8008CBA0(p, b, c, d);
        }
        if (p[0x9A] != 0xD && (*(u32 *)(d + 0x1C) & 0x200) != 0)
        {
            *(u16 *)(c + 0x14) &= 0xF7FF;
            func_80090200(p, b, c, d);
        }
    }
}

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

/* An UnkNode +0x10 step (func_800B7D74 installs it), so the three
   parameters are func_80040190's cast and all three are read incoming.
   The three byte stores of ONE shared local come out 0xC / 0xE / 0xD and
   the source has to be written that way: `0xC, 0xD, 0xE` is the same 88
   words at 2 differing, with only those two `sb` displacements swapped.
   (That is the opposite of the three-read-modify-write lever, where the
   listing's last two offsets are the reverse of the source -- here only the
   FIRST store is a read-modify-write and the other two are plain stores of
   the local, so sched1 has nothing to hoist past.) 88 words, 0 differing. */
void func_800B7C14(u8 *p, UnkVec *b, u8 *c)
{
    s32 v;
    s16 t;

    if (*(s32 *)c != 0)
    {
        func_800478B8(c);
        if (*(u16 *)(c + 0x14) & 0x4000)
        {
            func_8003DB94(c, *(void **)(p + 0x8), 0);
        }
    }
    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= b->unk_0C >> 4;
    b->unk_10 -= b->unk_10 >> 4;
    b->unk_14 += 0x40000;
    if (*(s16 *)(p + 0x12) != 0)
    {
        if (c[0xC] != 0)
        {
            v = c[0xC] - 8;
            c[0xC] = v;
            c[0xE] = v;
            c[0xD] = v;
        }
    }
    *(s16 *)(c + 0x16) += *(u16 *)(p + 0x14);
    *(s16 *)(c + 0x18) += *(u16 *)(p + 0x16);
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* The 0x12 node's +0x10 step for func_800B8228's world marker: a three-case
   switch on the actor view's +0xA2, case 0 falling through into case 1. The
   +0xA4 countdown is spent with `if (--*(s16 *)(p + 0xA4) == 0)` -- the
   decrement's own value is what the target tests (`sll 16` with no `sra`),
   so a separate `-= 1` then a re-read is the wrong shape. The last case's
   fourth argument is `(s32)D_8006E240`, copied from func_800B8228's own call
   two functions down. 84 words, 0 differing on the first compile. */
void func_800B80D8(u8 *p, UnkVec *m, u8 *r)
{
    switch (*(s16 *)(p + 0xA2))
    {
    case 0:
        if ((*(u16 *)(r + 0x14) & 0x8000) != 0)
        {
            *(s16 *)(p + 0xA2) = 2;
            return;
        }
        *(s16 *)(p + 0xA4) = 0xC;
        *(s16 *)(p + 0xA2) += 1;
        /* fallthrough */
    case 1:
        m->unk_14 += 0x30000;
        m->unk_08 += m->unk_14;
        if (*(u16 *)(r + 0x1C) < 0x1000)
        {
            *(u16 *)(r + 0x1C) += 0x50;
            *(u16 *)(r + 0x1E) += 0x50;
        }
        if (--*(s16 *)(p + 0xA4) == 0)
        {
            m->unk_14 = 0;
            *(s16 *)(p + 0xA2) += 1;
        }
        break;
    case 2:
        func_800A7A7C(*(s16 *)(p + 0xB0), *(s16 *)(p + 0xB2), *(u16 *)((u8 *)m + 0xA) - 0x20, (s32)D_8006E240,
                      p + 0x98);
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
        break;
    }
}

/* Spawn the 0x12 node for a world-space marker, or hand the same three
   coordinates to func_800A7A7C when the allocator is empty.

   THE THREE COORDINATES ARE `u32`, NOT `u16`. The `>> 6` pair is a LOGICAL
   shift, so they are unsigned; declared u16, cc1psx masks each with
   `andi 0xFFFF` before the shift, which forces two more callee-saved
   registers and their saves and restores -- 85 words against 77 with 84
   differing. The +0xB8 copy is a 4-byte object of ALIGNMENT 1, hence the
   `lwl`/`lwr` + `swl`/`swr` pairs. 77 words, 0 differing. */
UnkNode *func_800B8228(u32 x, u32 y, u32 z, u8 *d)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        func_8004491C(n, (u32)D_80045340);
        n->unk_10 = (u32)func_800B80D8;
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = z;
        *(s16 *)((u8 *)m + 0x16) = -0x12;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x1E) = 0xC90;
        *(s16 *)(r + 0x1C) = 0xC90;
        *(u32 *)(r + 0x8) = func_800A7A38(d);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0xB0) = x >> 6;
        *(s16 *)(q + 0xB2) = y >> 6;
        *(UnkDungeonName4 *)((u8 *)n + 0xB8) = *(UnkDungeonName4 *)d;
        func_800BC26C(n, 0, 0, 0);
        func_800A56E0(0x51C);
    }
    else
    {
        func_800A7A7C((s16)(x >> 6), (s16)(y >> 6), (s16)z, (s32)D_8006E240, d);
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B835C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B84E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8830);

/* Allocate the 0x12 node for a floating tag. THE +0x20 VIEW IS A STRUCT, and
   that is worth the whole function: see UnkDungeonTag. Through a `u8 *` the
   four stores are not MEM_IN_STRUCT_P, sched1 cannot lift the D_800814A8 load
   over them, and the best of 24 store permutations x four pointer spellings
   is 2 differing. 52 words, 0 differing. */
void func_800B8C20(s32 a, s32 e, u8 *c, u8 *d)
{
    UnkDungeonTag *n;
    u8 *r;

    n = (UnkDungeonTag *)func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        r = *(u8 **)((u8 *)n + 0xC);
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800B8830;
        if (c != NULL)
        {
            *(u8 **)(r + 0x8) = c;
        }
        else
        {
            *(u8 **)(r + 0x8) = D_800DF2F8;
        }
        *(s16 *)(r + 0x10) = 0x60;
        n = (UnkDungeonTag *)((u8 *)n + 0x20);
        if (d != NULL)
        {
            n->unk_0C = d;
        }
        else
        {
            n->unk_0C = D_800DF2F8;
        }
        n->unk_00 = a;
        n->unk_04 = D_80083498;
        n->unk_32 = *(u16 *)(D_800814A8 + 0x88);
        n->unk_08 = e;
    }
}

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

/* Spawn the 0x212 node. THE MOVER POINTER IS CACHED IN A LOCAL: written as
   three `n->unk_08->...` stores the pointer is RELOADED before each one -- a
   store through the cast can alias it -- and the function is 49 words against
   45 with 27 differing. 45 words, 0 differing. */
void func_800B8D64(s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    u8 *m;
    u8 *r;

    n = func_8003FC64(0x212);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B8CF0;
        func_8004491C(n, (u32)D_80045340);
        m = (u8 *)n->unk_08;
        *(s16 *)(m + 0x2) = x;
        *(s16 *)(m + 0x6) = y;
        *(s16 *)(m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800DF358;
        *(s16 *)(r + 0x1E) = 0x800;
        *(s16 *)(r + 0x1C) = 0x800;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)((u8 *)n + 0x22) = 6;
    }
}

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

/* Spawn the kind-0x12 node under `a`.

   TWO PARAMETER WIDTHS ARE FORCED, both on STACK arguments, and they pull in
   opposite directions. The NINTH must be `s16`: as s32 the entry `lw` dies
   and gcc reloads it after the calls instead of keeping it in a callee-saved
   register, 70 words against 72. The SEVENTH is `s16` but its value has to be
   COPIED INTO AN s32 LOCAL before use: read directly, gcc tests the
   parameter's HImode pseudo with a bare `sll 16` and takes the raw register
   for `g - 1`, so the `sra` never appears -- 71 words, 29 differing.
   Declaring it s32 and narrowing with `(s16)g` into that same local is
   71 / 63. 72 words, 0 differing. */
void func_800B8EA8(void *a, s32 b, s32 c, s32 d, u16 *e, void *f, s16 g, s32 h, s16 i)
{
    UnkNode *n;
    u8 *v;
    u8 *s;
    s32 t;

    n = func_8003FD64(0x12, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800B8E18;
        func_8004491C(n, (u32)func_800CEEFC);
        v = (u8 *)n->unk_08;
        *(s16 *)(v + 0x2) = e[0];
        *(s16 *)(v + 0x6) = e[1];
        *(s16 *)(v + 0xA) = e[2];
        *(s16 *)(v + 0xE) = b;
        *(s16 *)(v + 0x12) = c;
        *(s16 *)(v + 0x16) = d;
        s = (u8 *)n->unk_0C;
        *(void **)(s + 0x8) = f;
        *(s16 *)(s + 0x1E) = h;
        *(s16 *)(s + 0x1C) = h;
        *(s32 *)(s + 0xC) = 0x808080;
        t = g;
        if (t != 0)
        {
            *(s16 *)(s + 0x10) = (t - 1) << 5;
            *(u16 *)(s + 0x14) |= 0xC;
        }
        *(s16 *)((u8 *)n + 0x22) = i;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9144);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B96C4);

/* A spawner: allocate a 0x12 node, install func_80045C34 as the engine hook
   and func_800B96C4 as the node's step, seed the mover from D_80083780 and
   the owner's +0x88 facing, dress the renderer, point the view's +0x00 back
   at the owner, publish the node in D_8008346C and open the message box.
   69 words, 0 differing.

   TWO SPELLINGS ARE LOAD-BEARING, both worth one word and both measured.
   (1) `q` MUST BE ASSIGNED ABOVE THE func_8003DB94 CALL. cse rewrites the
   bare `*q` into `0x20($s2)` but leaves `*(q + 0xE)` alone, so `q` has one
   use left and combine folds the `addiu` away -- 68 words -- unless a CALL
   sits between the assignment and the use, which combine will not cross.
   (2) THE +0x10 INSTALL IS A CAST, NOT `n->unk_10`. As a COMPONENT_REF the
   store carries MEM_IN_STRUCT_P, sched1 proves it independent of the two
   node loads and sinks it four insns, and the two address materialisations
   interleave: 10 differing with every opcode already right. */
UnkNode *func_800B9964(u8 *p)
{
    UnkNode *n;
    UnkVec *m;
    UnkSlot *r;
    u8 *q;
    u8 *g;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        func_8004491C(n, (u32)func_80045C34);
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800B96C4;
        g = D_80083780;
        m = n->unk_08;
        r = (UnkSlot *)n->unk_0C;
        q = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = *(s16 *)(g + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(s16 *)(g + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(*(u8 **)p + 0x88);
        func_8003DB94(r, D_80079444, 0);
        r->unk_1E = 0x100;
        r->unk_1C = 0x100;
        r->unk_10 = 0x20;
        *(u32 *)((u8 *)r + 0xC) = 0x2C808080;
        r->unk_06 = 4;
        r->unk_14 |= 0xC;
        *(s16 *)(q + 0xE) = 4;
        *(u8 **)q = p;
        D_8008346C = (s32)n;
        func_800C77D0(n, (u8 *)m, 8, D_800DCE66);
    }
    return n;
}

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

/* A node +0x10 handler, the same shape as func_800A6B70 next door.
   `extern u16 D_800DF37C;` as a SCALAR: the `%lo` sits inside the `lhu`, not
   in a separate `addiu`. 43 words, 0 differing. */
void func_800BA6B8(u8 *p, s32 b, UnkDungeonSprite *s)
{
    s32 v;

    if (D_800DF37C != 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    v = (func_800644B8(D_80083164[0] << 8) >> 7) - 0x80;
    if (p[0x30] != 0)
    {
        s->unk_0C = v;
    }
    if (p[0x31] != 0)
    {
        s->unk_0D = v;
    }
    if (p[0x32] != 0)
    {
        s->unk_0E = v;
    }
}

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

/* State 0 seeds the 0x40-byte noise table at +0x12 and falls into state 1
   WITHOUT breaking. THE INDEX MUST BE WRITTEN `(p + i)[0x12]`: as
   `p[i + 0x12]` the `addu` comes out index-first, which is exactly one
   differing word. `i` is s16, so the index and the bound test each pay their
   own `sll 16`/`sra 16`. 80 words, 0 differing. */
void func_800BAF04(u8 *p)
{
    s16 i;

    switch (*(s16 *)(p + 0xA))
    {
    case 0:
        for (i = 0; i < 0x40; i++)
        {
            (p + i)[0x12] = func_80069EF8() & 0x1F;
        }
        *(s16 *)(p + 0xE) = 0;
        p[0x10] = 0;
        p[0x32] = 0;
        p[0x11] = 1;
        func_8004491C((UnkNode *)(p - 0x20), (u32)func_800BABA8);
        *(s16 *)(p + 0xA) = *(u16 *)(p + 0xA) + 1;
    case 1:
        *(s16 *)(p + 0xE) = *(u16 *)(p + 0xE) + 1;
        if (*(s16 *)(p + 0xE) >= 0x11)
        {
            *(s16 *)(p + 0xE) = 0;
        }
        p[0x32] = p[0x32] + 1;
        if ((s8)p[0x32] >= 0x21)
        {
            p[0x32] = 0x20;
            p[0x10] = 1;
        }
        if (p[0x11] == 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
}

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

/* An eight-argument 0x202 spawner on the engine's own record. A POINTER
   LOCAL for `n + 0x20` is what gives the target's `addiu $a1, $s0, 0x20`;
   cse rewrites `*p` back to `0x20($s0)` and leaves the rest on the
   materialised base. 67 words, 0 differing. */
UnkNode *func_800BC0A8(s16 x, s16 y, s16 ang, s16 d, s16 e, s16 f, u32 g, s16 h)
{
    UnkNode *n;
    UnkVec *m;
    u8 *p;
    u8 *r;

    n = func_8003FD64(0x202, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800BBBE0;
        func_8004491C(n, (u32)func_800BC054);
        m = n->unk_08;
        p = (u8 *)n + 0x20;
        *(s16 *)((u8 *)m + 0x2) = x;
        *(s16 *)(p + 0x4) = x;
        *(s16 *)((u8 *)m + 0x6) = y;
        *(s16 *)(p + 0x6) = y;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s16 *)(p + 0x8) = ang;
        r = (u8 *)n->unk_0C;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1A) = d + 0x400;
        *(s16 *)(p + 0x4E) = d;
        *(s16 *)(p + 0x4C) = 4;
        *(s16 *)(p + 0x50) = e;
        *(u32 *)p = g;
        *(s16 *)(p + 0x58) = f;
        *(s16 *)(p + 0x48) = h;
    }
    return n;
}

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
UnkNode *func_800BC26C(void *list, s16 flag, s32 val, s32 d)
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

/* ONE POINTER LOCAL FOR THE SCRATCHPAD, not three constant addresses:
   `*(s16 *)0x1F800100 = ...` three times gives three `lui $at, 0x1f80` and
   the function is 62 words / 57 differing, where 0x1F800000 -- a constant
   with a ZERO LOW HALF -- is a one-word base the three stores share as
   displacements. splat renders these as raw immediates rather than %lo
   symbols, so there is no relocation here. 60 words, 0 differing. */
s32 func_800BC3E4(u8 *p)
{
    Unk80083160 *g = &D_80083160;
    u8 *s = (u8 *)0x1F800000;
    UnkNode *n;
    UnkNode *q;
    UnkVec *m;
    u8 *r;
    s32 t;

    *(s16 *)(s + 0x100) = -g->unk_C4;
    *(s16 *)(s + 0x102) = -g->unk_C6;
    *(s16 *)(s + 0x104) = g->unk_B8;
    while (1)
    {
        q = *(UnkNode **)p;
        r = (u8 *)q->unk_0C;
        if ((*(u16 *)(r + 0x14) & 0x8000) == 0)
        {
            m = q->unk_08;
            t = func_800BCB04(*(u16 *)((u8 *)m + 0x2), *(u16 *)((u8 *)m + 0x6),
                              *(s16 *)((u8 *)m + 0xA));
            func_800BC4D4(m, r, (s16)t, 0);
            if (func_80045310(*(u32 *)(g->unk_00 + 0x8D0)) != 0)
            {
                break;
            }
        }
        n = *(UnkNode **)(p - 0x8);
        if (n == NULL)
        {
            break;
        }
        p = (u8 *)n + 0x20;
    }
    return 0;
}

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

/* Widened from `(void)`: func_80091A38 calls it with the high byte of its
   handler word minus one. An unread parameter costs it nothing. */
void func_800BCFBC(s32 a)
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

/* 166 words, 0 differing. */
s32 func_800BD1C4(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s32 n;
    s32 t;
    s16 d;
    u8 *g;

    d = 0;
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
    func_800C4AFC(*(u8 **)(p - 0x18), 0x20E020, p);
    n = (func_800A6D30() & 7) + 0x20;
    if (n >= 0x100)
    {
        n = 0xFF;
    }
    *(s16 *)(p + 0x64) = n;
    if (p[0x28] == p[0x29])
    {
        if (q[0] == 0xF)
        {
            if (p[5] < 0xFF)
            {
                p[5] = p[5] + 1;
                func_80041E70(p);
            }
            *(s16 *)(p + 0x64) = 1;
            d = 1;
        }
        else
        {
            d = -1;
        }
    }
    else
    {
        if (p[0x29] - p[0x28] < n)
        {
            n = p[0x29] - p[0x28];
        }
        *(s16 *)(p + 0x64) = n;
    }
    if (func_800AD568(p) != 0)
    {
        if (d > 0)
        {
            t = 0x8010;
        }
        else
        {
            t = 0x10;
        }
        func_800B4C7C(t, p, *(s16 *)(p + 0x64), 1);
    }
    if (*(u32 *)(p + 0x14) & 0x4000)
    {
        m = func_800990FC();
        if (d > 0)
        {
            h = func_80099734(p, m);
            h = func_80099194(D_800E0E14, h);
            h = func_8003AD08(*(s16 *)(p + 0x64), h);
            h = func_80099194(D_80089354, h);
        }
        else if (d == 0)
        {
            h = func_80099734(p, m);
            h = func_80099194(D_800E0E34, h);
            h = func_8003AD08(*(s16 *)(p + 0x64), h);
            h = func_80099194(D_80089354, h);
        }
        else
        {
            h = func_80099194(D_800E0E50, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The first kill skeleton with a THREE-WAY flag and the D_800DDE84 field
   taken as bits 8-9 (`srl 8` / `andi 3`), the only member that reads that
   half; the func_800AD6FC gate is tested as a HALFWORD here (`sll 16` with
   no `sra`), which is the (s16) cast func_800C4490 also needs.

   `s16 f` CARRIES -1, 0 AND 1, and the three message arms are spelled
   `if (f < 0) ... else if (f != 0) ... else ...`. Written as the reading
   order `else if (f == 0) {0F65} else {0F44}` the second test comes out
   `bnez` where the target has `beqz` -- 1 differing at the right length,
   and invisible everywhere else because both strings are relocations.
   136 words, 0 differing. */
s32 func_800BDC98(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    s16 f;
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
        if (p[0x25] == p[0x66])
        {
            f = 1;
        }
        if ((s16)func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 8) & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        if (f != 0)
        {
            if (p[0x4] < 0xFF)
            {
                p[0x4]++;
                func_80041E70(p);
            }
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            if (f == 0)
            {
                if (p[0x25] != p[0x66])
                {
                    f = -1;
                }
            }
            h = func_800990FC();
            m = h;
            if (f < 0)
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F2D, h);
            }
            else if (f != 0)
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F44, h);
            }
            else
            {
                h = func_80099734(p, h);
                h = func_80099194(D_800E0F65, h);
            }
            func_80099290(h);
            func_800A5720(m);
        }
        func_800D5460((UnkNode *)(p - 0x20), 0x20A0A0, 0x702);
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E0F85);
        func_800A56E0(0x506);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

s32 func_800BE360(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    s32 f;

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
        if ((*(s16 *)(D_80073414[q[1]].unk_0C + q[0] * 0x14) & 0x8000) == 0)
        {
            f = D_800DDE84[p[0x13]] & 3;
        }
        else
        {
            f = (D_800DDE84[p[0x13]] >> 4) & 3;
        }
        if (func_800AD6FC(p, f, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
    }
    else
    {
        func_800997FC(D_800E101C);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* 103 words, 0 differing. */
s32 func_800BF074(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *g2;
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
        h = func_800990FC();
        m = h;
        g2 = (u8 *)&D_80082EB0;
        h = func_80099368(*(u8 **)(g2 + 4), h);
        h = func_80099194(D_800E11F5, h);
        (*(u8 **)(g2 + 4))[3] &= 0x7F;
        func_800A90E8(*(u8 **)(g2 + 4));
        h = func_80099368(*(u8 **)(g2 + 4), h);
        h = func_80099194(D_80089374, h);
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

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

/* 133 words, 0 differing. */
s32 func_800BF9EC(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *h;
    u8 *e;
    s32 t;
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
        func_800C4D78((u8 *)0x2020C0, 1);
        func_800997FC(D_800E131C);
        h = (u8 *)D_800814A8;
        for (e = *(u8 **)(h + 0x5C) + 0x20; e != h; e = *(u8 **)(e + 0x5C) + 0x20)
        {
            if ((*(u32 *)(e + 0x1C) & 0x2400) != 0x2000 ||
                (s16)func_80042900((s8 *)e, 0xC) != 0)
            {
                if (p[0x11] < 0x63)
                {
                    *(u32 *)(e + 0x18) = D_800835E8[e[0x11]];
                    func_800A1D4C(e, 0);
                    if (!(*(u16 *)(*(u8 **)(e - 0x14) + 0x14) & 0x8000))
                    {
                        if (*(u32 *)(e + 0x14) & 0x2000)
                        {
                            t = 0x8002;
                        }
                        else
                        {
                            t = 0x8003;
                        }
                        func_800B4C7C(t, e, -2, 1);
                    }
                }
            }
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The func_800BD72C kill skeleton with this entity's middle: below a
   population of 0x63 the record takes a fresh func_8004383C index, drops its
   two +0x41/+0x42 bytes and raises D_800E296C's 0x100000. First compile.
   79 words, 0 differing. */
s32 func_800BFF00(u8 *p, u8 *q, s16 k)
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
    if (p[0x11] < 0x63)
    {
        *(u32 *)(p + 0x18) = func_8004383C(p, 0);
        p[0x42] = 0;
        p[0x41] = 0;
        D_800E296C |= 0x100000;
    }
    func_800D4FC8((UnkNode *)(p - 0x20), 0x20F0F0, 0x616);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The kill skeleton whose out-of-range arm refuses while the engine's two
   state gates are up, otherwise runs func_800BBA40's effect and BUMPS the
   counter rather than dropping it -- the only member that does.

   THE PARAMETER IS COPIED THROUGH A DUPLICATED `if` ARM, and that is a
   device: the whole residue is one delay slot. Written plainly, reorg fills
   the cached-RAM gate's `bnez` with `lui %hi(D_80012090)` stolen from the
   head of the out-of-range block and retargets the branch past it; the
   target fills it from the fall-through with func_800A6480's first argument
   and leaves the `lui` where it is. 26 differing, every one of them that
   one-instruction shift, with no register anywhere wrong. Twenty-odd hand
   spellings sat flat on it; decomp-permuter went from base 60 to 0 in four
   minutes, and this is its idea minimised. A plain `pp = p;` does NOT work
   (26 differing) -- the two arms are what put a CODE_LABEL in reorg's way,
   and the guard is irrelevant (`q != NULL` and `k != 0` both reach 0).
   jump2 cross-jumps the arms back together, so the emitted code carries no
   trace of the device. Same mechanism as func_8009704C's duplicated arm.

   And the lever that got it to the right LENGTH, which is not a device:
   with three counter sites and one shared tail, exactly ONE arm
   early-returns. All three sharing the tail is 114 words of 117; all three
   carrying their own copy is 115. 117 words, 0 differing. */
s32 func_800C0230(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *g2;
    u8 *g3;
    u8 *e;
    u8 *pp;

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
    if (q != NULL)
    {
        pp = p;
    }
    else
    {
        pp = p;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(pp, q);
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
        if (D_80012090 == 0 && D_8008146C == 0x28 && func_80033BC0(0xA2) == 0)
        {
            func_800997FC(D_800E1375);
            g2 = (u8 *)&D_80083460;
            *(u16 *)(g2 + 0xA) -= 1;
            func_80098B38(q);
            return 1;
        }
        e = *(u8 **)(p - 0x14);
        if (func_800BBA40(e[0x24], e[0x25], *(s16 *)(p + 0x88), D_800DF45C, 0x2800, 0x208020,
                          func_800C0180) == 0)
        {
            return 0;
        }
        func_800997FC(D_800E13B7);
        g3 = (u8 *)&D_80083460;
        *(u16 *)(g3 + 0xA) += 1;
    }
    func_80098B38(q);
    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0404);

/* Re-home the engine node that carries `p`. THE COLOUR WORD IS STORED BEFORE
   THE TWO 0x1000 SCALES: that one statement swap is the whole residue -- with
   the colour last the 0x1000 constant and the +0x14 read-modify-write
   exchange $v0 and $a0 and the payload base moves, 62 words / 10 differing.
   All 120 orderings of the six +0x0C-record statements were measured; only
   this one reaches 0. 62 words, 0 differing. */
void *func_800C07AC(void *p)
{
    UnkNode *n;
    u8 *a;
    u8 *b;
    u8 *c;
    u8 *q;
    u8 *r;

    n = func_8003FD64(2, p);
    if (n != NULL)
    {
        func_80044A50(p);
        func_800BC318(p);
        n->unk_10 = (u32)func_800C0404;
        func_8004491C(n, (u32)D_80045340);
        b = *(u8 **)((u8 *)p + 0x8);
        a = *(u8 **)((u8 *)n + 0x8);
        *(s16 *)(a + 0x2) = *(u16 *)(b + 0x2);
        *(s16 *)(a + 0x6) = *(u16 *)(b + 0x6);
        *(s16 *)(a + 0xA) = *(u16 *)(b + 0xA);
        c = (u8 *)n->unk_0C;
        *(s16 *)(c + 0x10) = 0x20;
        *(u8 **)(c + 0x8) = D_800DF4A4;
        *(s32 *)(c + 0xC) = 0x808080;
        *(s16 *)(c + 0x1E) = 0x1000;
        *(s16 *)(c + 0x1C) = 0x1000;
        *(u16 *)(c + 0x14) |= 0xC;
        q = (u8 *)p + 0x20;
        r = (u8 *)n + 0x20;
        *(s16 *)(r + 0x4C) = 0x10;
        *(u8 **)(r + 0x68) = q;
        *(s32 *)(q + 0x14) |= 0x100000;
    }
    return n;
}

/* The second kill skeleton with a 0x16 ENTITY ARM that runs the same
   func_80099978 / func_800998C0 sentence the out-of-range arm does, and a
   0xD short-circuit that is itself conditional:
   `if (k == 0xD) { if (p[0x13] != 0x16 || (func_800A6D30() & 1)) return
   func_80098864(q, r); }`.

   The chain is func_800BEB30's, including its two measured facts -- `h` is
   REUSED for func_80099368's result with the old handle parked in its own
   variable first, and the biased pointer holds `h - 2` rather than taking
   the `- 2` at the call site. A second variable for func_80099368's result
   (`b = func_80099368(q, h); func_800998C0(a, h);`) is 177 words.

   THE TWO CHAINS SHARE `m` AND `h` BUT NOT THE BIASED POINTER AND THE PARKED
   HANDLE. With `a`/`b` shared as well neither `b` coalesces, the function
   takes a fifth callee-saved register and comes out 179 words; with all four
   split per arm it is the right length but `m` and `h` swap $s1 and $s2,
   24 differing. Only this split reaches 0 -- the other half of
   func_800C0B60's "two pairs of handle variables" lesson.
   175 words, 0 differing. */
s32 func_800C08A4(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    void *a;
    void *b;
    void *a2;
    void *b2;
    u8 *n;
    u8 *g;

    if (k == 0xD)
    {
        if (p[0x13] != 0x16 || (func_800A6D30() & 1))
        {
            return func_80098864(q, r);
        }
    }
    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        if (p[0x13] == 0x16)
        {
            n = *(u8 **)(p - 0x18);
            p[0x4B] &= ~0x20;
            func_800A7A7C(*(u16 *)(n + 0x2) >> 6, *(u16 *)(n + 0x6) >> 6, *(s16 *)(n + 0xA),
                          func_8004A658(p[0x49], p[0x48]), p + 0x48);
            p[0x48] = q[0];
            p[0x4B] |= 0x20;
            m = func_800990FC();
            h = func_80099978(func_80099194(D_800E13DA, func_80099734(p, m)));
            a = (u8 *)h - 2;
            h = func_80099194(D_80089378, h);
            b = h;
            h = func_80099368(q, h);
            func_800998C0(a, b);
            h = func_80099194(D_8008937C, h);
            func_80099290(h);
            func_800A5720(m);
        }
        else
        {
            func_800A6480(p, q);
            if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 4) & 3, 0) == 0)
            {
                func_800A5F38(p, q);
                return 1;
            }
        }
        func_80098B38(q);
    }
    else
    {
        m = func_800990FC();
        h = func_80099978(func_80099194(D_800E13E9, func_80099734(p, m)));
        a2 = (u8 *)h - 2;
        h = func_80099194(D_80089378, h);
        b2 = h;
        h = func_80099368(q, h);
        func_800998C0(a2, b2);
        h = func_80099194(D_8008937C, h);
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* 309 words, 0 differing. */
s32 func_800C0E88(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    UnkDungeonName0 *t;
    s32 i;
    s32 d;
    u8 *e;
    s32 f;
    s32 n;
    s32 w;
    s32 a;
    s32 b;
    u16 v;
    u8 *g;
    u8 *g2;
    u8 *g3;

    if (k == 0xD)
    {
        return func_80098864(q, r);
    }
    if (p == D_800E3D7C)
    {
        q[3] |= 0x20;
        *(u8 **)(D_800E3D7C + 0x110) = q;
        func_8008D344(D_800E3D7C, D_80083780, D_80082E80, D_800E3D7C);
        return 0;
    }
    if ((u32)p <= 0x9FFFFFFF)
    {
        func_800A6480(p, q);
        if ((s8)q[2] >= 0xA && q[0] < 0x10)
        {
            if (func_800A57B4(p, (q[0] - 1) * 3 + 1) < 0)
            {
                t = (UnkDungeonName0 *)D_8006DE24;
                for (i = 0; i < 3; i++)
                {
                    if (p[i * 3 + 8] == 0)
                    {
                        f = *(u32 *)(p + 0x14);
                        if (f & 1)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 1;
                        }
                        else if (f & 2)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 2;
                        }
                        else if (f & 4)
                        {
                            p[i * 3 + 8] = (q[0] - 1) * 3 + 3;
                        }
                        d = (s8)q[2];
                        d = d / 10;
                        p[i * 3 + 0xA] = d;
                        p[i * 3 + 9] = d;
                        if (*(u32 *)(p + 0x14) & 0x4000)
                        {
                            h = func_800990FC();
                            m = h;
                            h = func_80099734(p, h);
                            h = func_80099194(D_800E1440, h);
                            h = func_80099194(t[p[i * 3 + 8]].unk_00, h);
                            h = func_80099194(D_800E144C, h);
                            func_80099290(h);
                            func_800A5720(m);
                        }
                        break;
                    }
                }
            }
        }
        else if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        func_80098B38(q);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
    }
    else if (*(s8 *)(q + 2) == 0)
    {
        func_800997FC(D_800E1456);
        q[3] &= 0xDF;
        g2 = (u8 *)&D_80083460;
        *(u16 *)(g2 + 0xA) -= 1;
    }
    else
    {
        *(u16 *)(D_800E3D7C + 0x98) &= 0xFF7F;
        p[9] = 0xFF;
        n = (q[0] - 1) * 3;
        p[8] = n + 1;
        v = *(u16 *)(D_80073470 + q[0] * 0x14);
        if (v & 2)
        {
            p[8] = n + 2;
        }
        else if (v & 4)
        {
            p[8] = n + 3;
        }
        e = *(u8 **)(p - 0x14);
        p = (u8 *)((u32)p & 0xDFFFFFFF);
        w = 0x10;
        *(s32 *)(p + 0x60) = func_800A05A4(p, e[0x24], e[0x25], *(s16 *)(p + 0x2A), w);
        a = *(s8 *)(p + 0x72);
        b = *(s8 *)(p + 0x73);
        p[0x72] = __builtin_abs(a);
        p[0x73] = __builtin_abs(b);
        if (q[3] & 0x20)
        {
            if (func_800A94A0(p, p + 8, 0, D_800E3D7C + 0x98) == 0)
            {
                return 0;
            }
            D_800DF4B0 = w;
            q[3] &= 0xDF;
        }
        D_800DF4B0 -= 1;
        if (D_800DF4B0 >= 0)
        {
            return 0;
        }
        D_800DF4B0 = 0;
        if (func_8003F270() != 0)
        {
            return 0;
        }
        g3 = (u8 *)&D_80083460;
        *(u8 **)(g3 + 0xC) = p;
        *(u16 *)(D_800814A8 + 0x98) |= 0x80;
        q[2] -= 1;
        h = func_800990FC();
        m = h;
        h = func_800992E8(q, h);
        h = func_80099194(D_800E146F, h);
        func_80099290(h);
        func_800A5720(m);
        *(u16 *)(g3 + 0xA) -= 1;
    }
    return 1;
}

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

/* The 0x12 spawner for the two engine-slot markers func_800C152C places. The
   two position arguments are s16: the target sign-extends each with
   `sll 16`/`sra 10`, which is the narrow parameter's extension folded into
   the `* 64`. No prototype is added -- its only caller sits ABOVE it and
   rides the implicit declaration, and a prototype would narrow that caller's
   three promoted arguments. 54 words, 0 differing. */
UnkNode *func_800C1A44(s16 x, s16 y, s16 ang)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FD64(0x12, D_80083498);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800C1718;
        func_8004491C(n, (u32)func_80045C34);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800DF3C0;
        *(u32 *)(r + 0xC) = 0x808080;
        *(s16 *)(r + 0x6) = 8;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4C) = 8;
        *(s16 *)(p + 0x48) = 7;
    }
    return n;
}

/* 113 words, 0 differing. */
s32 func_800C1B1C(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    u8 *g2;
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
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        m = func_800990FC();
        g2 = (u8 *)&D_80082EB0;
        if ((*(u8 **)(g2 + 4))[1] == 4)
        {
            h = func_80099194(D_800E14D8, m);
            h = func_800992E8(*(u8 **)(g2 + 4), h);
            h = func_80099194(D_800E14EB, h);
            func_800A56E0(0x80F);
            e = *(u8 **)(g2 + 4);
            if (*(s8 *)(e + 2) < 0x63)
            {
                e[2] = e[2] + 1;
            }
        }
        else
        {
            h = func_80099194(D_800E1514, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The second kill skeleton whose out-of-range arm takes the message handle
   FIRST and only then decides what to say: with either +0xAC or +0xB0 of
   D_800E3D7C occupied it runs func_800C1F28 over both slots behind
   func_8003FA44's gate, otherwise it reports D_800E1549. The slot walk is
   func_800C0180's inline `i * 4 + D_800E3D7C + 0xAC` spelling, upward from 0.
   121 words, 0 differing. */
s32 func_800C1CE0(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
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
        h = func_800990FC();
        m = h;
        if (*(u8 **)(D_800E3D7C + 0xAC) != NULL || *(u8 **)(D_800E3D7C + 0xB0) != NULL)
        {
            if (func_8003FA44(4) == 0)
            {
                return 0;
            }
            for (i = 0; i < 2; i++)
            {
                e = *(u8 **)(i * 4 + D_800E3D7C + 0xAC);
                if (e != NULL)
                {
                    func_800C1F28(e, (s16)i);
                }
            }
            h = func_80099194(D_800E1532, h);
            func_800A56E0(0x501);
        }
        else
        {
            h = func_80099194(D_800E1549, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

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

/* 111 words, 0 differing. */
s32 func_800C2130(u8 *p, u8 *q, s16 k, s32 r)
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
        func_800A6480(p, q);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, (s32)q) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        func_800C4D78((u8 *)0xC02020, 1);
        h = func_800990FC();
        m = h;
        if (D_80082EA6 >= 0)
        {
            for (i = 0; i < 0x20; i++)
            {
                if (D_800E3648_s[i].unk_01 != 0 && *(u8 *)&D_800E3648_s[i] != 0 &&
                    (D_800E3648_s[i].unk_03 & 0x40) == 0)
                {
                    D_800E3648_s[i].unk_03 |= 0x80;
                }
            }
        }
        else
        {
            h = func_80099194(D_800E1567, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

/* The kill skeleton's out-of-range arm reaches the record D_80082EB4 names,
   and `p` ITSELF is reused for it -- the target keeps everything in $s0.
   THE FLAG IS SET LAST IN THE ARM: written first its `li $s2, 1` lands
   before the mask's `ori` and the function is 3 differing; one statement
   later, 2; last, 0. 111 words, 0 differing. */
s32 func_800C22EC(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *g;
    u8 *e;
    s32 f;
    s32 i;

    f = 0;
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
        func_800A63B8(p, q, k);
        if (func_800AD6FC(p, D_800DDE84[p[0x13]] & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
    }
    else
    {
        e = D_80082EB4;
        if (e[1] == 0x13)
        {
            i = e[3] & 0x1F;
            if (i < 0x14)
            {
                p = (u8 *)D_800E3DF0[i];
                if ((s16)func_80042900((s8 *)p, 0xE) != 0)
                {
                    p[0x28] = (p[0x29] >> 1) + 1;
                    *(s32 *)(p + 0x1C) &= ~0x4000000;
                    func_80041E70(p);
                    func_80099844(p, D_800E1580);
                    f = 1;
                }
            }
        }
        if (f == 0)
        {
            func_800997FC(D_800E15A2);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* 93 words, 0 differing. */
s32 func_800C24A8(u8 *p, u8 *q, s16 k)
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
    if ((s16)func_800A48F0((s8 *)p, 0x9, 0x60) != 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            h = func_800990FC();
            m = h;
            h = func_80099734(p, h);
            h = func_80099194(D_800E15BB, h);
        }
        else
        {
            m = func_800990FC();
            h = func_80099194(D_800E15E5, m);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* THE PAYLOAD POINTER IS ITS OWN STATEMENT, AND IT HAS TO BE ON THE OTHER
   SIDE OF A CALL. `d = n + 0x20;` written next to its use is folded into the
   store (`sh $v0, 0x40($s0)`, 147 words) -- combine turns
   (plus (plus n 32) 32) into one displacement whether the intermediate is a
   `u8 *`, an `s16 *` indexed [0x10], or written inline. Put the `d = n +
   0x20` BEFORE func_800A56E0 and the `addiu $v1, $s0, 0x20` survives as its
   own word: 148 and 0 differing.
   The D_800E39C8 clear needs the STRUCT field too -- `*(s32 *)((u8
   *)&D_800E39C8[i] + 8)` folds the +8 into the table address
   (`addiu $a0, $a0, 8`) and loses the store's displacement.
   148 words, 0 differing. */
s32 func_800C2CDC(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *e;
    u8 *n;
    u8 *d;
    s16 i;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    e = *(u8 **)(p - 0x14);
    n = func_800C7380(e[0x24], e[0x25], *(s16 *)(p + 0x88), 7, q[0]);
    if (n == NULL)
    {
        return 0;
    }
    d = n + 0x20;
    func_800A56E0(0x816);
    i = func_800B60B8(e[0x24], e[0x25], *(s16 *)(p + 0x88), 6, n);
    *(s16 *)(d + 0x20) = i;
    D_800E39C8[i].unk_08 = 0;
    h = func_800990FC();
    m = h;
    h = func_8009929C(8, h);
    if (k == 0xD)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E1684, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893D4, h);
        h = func_800999B0(h);
        if (!(*(u32 *)(p + 0x1C) & 0x2000))
        {
            if (p[0x13] < 0x31)
            {
                func_8009A3D0(e[0x24], e[0x25], 0x3000);
                *(u32 *)(p + 0x1C) |= 0x2000;
                func_8009A21C(e[0x24], e[0x25], 0x300);
            }
        }
        func_80099290(h);
        func_800A5720(m);
    }
    h = func_800990FC();
    m = h;
    h = func_80099194(D_800E169A, h);
    func_80099290(h);
    func_800A5720(m);
    if ((s16)func_80042900((s8 *)p, 2) != 0)
    {
        func_80042B68((s8 *)p, 2);
    }
    func_8009A21C(e[0x24], e[0x25], 2);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

/* The kill skeleton WITHOUT the cached-RAM gate: past the D_800E3D7C
   identity test it spawns through func_800C7380 and returns 0 if that
   fails, then dresses the spawn, clears the spot record's +0x08 word and
   speaks only when k == 0xD -- the handle is taken unconditionally, ahead
   of that test.

   THE SPOT INDEX IS NOT A LOCAL: `D_800E39C8[*(s16 *)(c + 0x20)].unk_08 = 0`
   with the subscript written at the site. Read into an `s32 i` first the
   `lui`/`addiu` for D_800E39C8 lands AFTER the `sh`/`sll`/`sra` instead of
   before it -- 5 differing at the right length. `c` is the spawn's record
   at `n + 0x20`, which is where the `addiu $v1, $s0, 0x20` comes from.
   106 words, 0 differing. */
s32 func_800C2F2C(u8 *p, u8 *q, s16 k)
{
    void *m;
    void *h;
    u8 *e;
    u8 *n;
    u8 *c;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D344(p, D_80083780, D_80082E80, p);
        return 0;
    }
    e = *(u8 **)(p - 0x14);
    n = func_800C7380(e[0x24], e[0x25], *(s16 *)(p + 0x88), -1, q[0]);
    if (n == NULL)
    {
        return 0;
    }
    func_800A56E0(0x60D);
    c = n + 0x20;
    *(s16 *)(c + 0x20) = func_800B60B8(e[0x24], e[0x25], *(s16 *)(p + 0x88), 5, n);
    D_800E39C8[*(s16 *)(c + 0x20)].unk_08 = 0;
    h = func_800990FC();
    m = h;
    if (k == 0xD)
    {
        h = func_80099368(q, h);
        h = func_80099194(D_800E1713, h);
        h = func_80099734(p, h);
        h = func_80099194(D_800893D8, h);
        h = func_800999B0(h);
        func_80099290(h);
        func_800A5720(m);
    }
    func_800997FC(D_800E16E3);
    func_800C8CD8(p, 0x100, 0x10);
    func_8009A21C(e[0x24], e[0x25], 0x10);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

s32 func_800C30D4(u8 *p, u8 *q, s16 k)
{
    u8 *e;
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
    e = *(u8 **)(p - 0x14);
    func_800C5BBC((e[0x24] << 6) | 0x20, (e[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802080, 0x20,
                  1);
    if ((s16)func_800A48F0((s8 *)p, 0x5, 0x8) != 0)
    {
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E1729);
        }
    }
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The second kill skeleton with an out-of-range arm that sweeps the two
   engine slots at D_800E3D7C's +0x4C and +0x50, clearing each one's 0x40
   bit and announcing it, and falls back to D_800E1788 when neither fired.
   The flag is SET LAST IN EACH ARM, as in func_800C22EC. 148 words,
   0 differing. */
s32 func_800C3440(u8 *p, u8 *q, s16 k, s32 r)
{
    void *m;
    void *h;
    u8 *e;
    s32 f;
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
        func_800C4D78((u8 *)0xC0C0C0, 0);
        func_800A56E0(0x702);
        func_80042B68((s8 *)p, 0x8);
        f = 0;
        e = *(u8 **)(D_800E3D7C + 0x4C);
        if (e[1] != 0 && (e[3] & 0x40))
        {
            e[3] &= ~0x40;
            h = func_800990FC();
            m = h;
            h = func_80099368(e, h);
            h = func_80099194(D_800E1756, h);
            func_80099290(h);
            func_800A5720(m);
            f = 1;
        }
        e = *(u8 **)(D_800E3D7C + 0x50);
        if (e[1] != 0 && (e[3] & 0x40))
        {
            e[3] &= ~0x40;
            h = func_800990FC();
            m = h;
            h = func_80099368(e, h);
            h = func_80099194(D_800E176F, h);
            func_80099290(h);
            func_800A5720(m);
            f = 1;
        }
        if (f == 0)
        {
            h = func_800990FC();
            m = h;
            h = func_80099194(D_800E1788, h);
            func_80099290(h);
            func_800A5720(m);
        }
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

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

/* 120 words, 0 differing. */
s32 func_800C3900(u8 *p, u8 *q, s16 k)
{
    s32 f;
    u8 t;
    u8 *gi;
    u8 *g;

    if (p == D_800E3D7C)
    {
        *(u8 **)(p + 0x110) = q;
        func_8008D330((UnkDungeonActor *)p, D_80083780, D_80082E80, p);
        return 0;
    }
    if (p[0x13] >= 1 && p[0x13] < 0x2F && (u32)p <= 0x9FFFFFFF)
    {
        func_800A63B8(p, q, k);
        f = (*(u32 *)(p + 0x1C) >> 3) & 1;
        if (func_800AD6FC(p, (D_800DDE84[p[0x13]] >> 6) & 3, 0) == 0)
        {
            func_800A5F38(p, q);
            return 1;
        }
        t = f;
        if (t != 0)
        {
            *(u32 *)(p + 0x1C) |= 8;
        }
        *(s16 *)(p + 0x24) = 0;
        if ((*(u32 *)(p + 0x1C) & 8) == 0)
        {
            if (*(u32 *)(p + 0x1C) & 0x80000)
            {
                func_80094E34();
            }
            gi = (u8 *)&D_80083460;
            *(u16 *)(gi + 0xA) += 1;
        }
        func_800A56E0(0x51E);
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            func_80099844(p, D_800E17C6);
        }
        func_80098B38(q);
    }
    else if (p[0x13] == 0)
    {
        func_800997FC(D_800E17EF);
        func_800A56E0(0x506);
    }
    else
    {
        func_80098B38(q);
        func_800997FC(D_800E180E);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* The floor-wide reveal: behind D_800E296C's 0x20000000 it raises
   0x40000000 on every record of the engine's +0x5C ring, clears the two spot
   tables' live halfwords and walks the 0x3E x 0x3E cell grid, flooring every
   cell whose first halfword is 0xB or more and clearing its +0x02.

   THE D_80083160 BASE IS A LOCAL OF ITS OWN. Written `w = (u8 *)&D_80083160
   + 0x1DC` in one go gcc folds the displacement into the symbol
   (`%hi(D_80083160+476)`) and the pair that materialises the base disappears
   -- 188 words of 189. Assigning `v = (u8 *)&D_80083160;` first and taking
   both `v + 0x1DC` and `*(s32 *)(v + 0x1DC)` off it keeps the
   `lui`/`addiu`/`addiu` the target has.

   THE SWEEP COUNTER IS NOT THE NESTED LOOPS' COUNTER: with one `i` for all
   three loops the pseudo gets one register and the two table sweeps come out
   with the counter and the narrow table's giv in each other's registers,
   13 differing. The ring walk also loads into the CURSOR and copies to the
   sentinel (`n = D_800E3D7C; b = n;`), not the other way round -- reversed
   it is 2 differing.

   THE INNER COUNTER IS BUMPED BETWEEN THE INDEX AND THE ADDRESS. The target
   puts `addiu $a0, $a0, 1` immediately after `addu $v0, $a0, $v0` and before
   the multiply by 6; a plain `for (l = 1; l < 0x3F; l++)` emits it after the
   store and the last five words shift. Splitting the third subscript into
   `x = l + (j << shift); l++;` is the only spelling measured that places it
   there. 189 words, 0 differing. */
s32 func_800C3D3C(u8 *p, u8 *q, s16 k, s32 r)
{
    u8 *v;
    u8 *w;
    s32 t;
    u8 *b;
    u8 *n;
    u8 *c;
    s32 i;
    s32 j;
    s32 l;
    s32 x;
    u8 *g;

    v = (u8 *)&D_80083160;
    w = v + 0x1DC;
    t = *(s32 *)(v + 0x1DC);
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
    else if (D_800E296C & 0x20000000)
    {
        func_8009BF7C(1, 8);
        func_800A56E0(0x80F);
        n = D_800E3D7C;
        b = n;
        do
        {
            *(u32 *)(n + 0x1C) |= 0x40000000;
            *(s16 *)(n + 0x88) = 0;
            n = *(u8 **)(n + 0x5C) + 0x20;
        } while (n != b);
        for (i = 0; i < 0x40; i++)
        {
            if (D_800E3548_s[i].unk_01 != 0)
            {
                D_800E36C8[i].unk_04 = 0;
            }
        }
        for (i = 0; i < 0x20; i++)
        {
            if (D_800E3648_s[i].unk_01 != 0)
            {
                D_800E39C8[i].unk_12 = 0;
            }
        }
        for (j = 1; j < 0x3F; j++)
        {
            for (l = 1; l < 0x3F;)
            {
                c = (u8 *)((l + (j << *(s16 *)(w + 0x14))) * 6 + t);
                if (*(u16 *)c >= 0xB)
                {
                    *(s16 *)c = 0x68;
                    c = (u8 *)((l + (j << *(s16 *)(w + 0x14))) * 6 + t);
                    *(u16 *)(c + 0x4) &= 0x7B32;
                }
                x = l + (j << *(s16 *)(w + 0x14));
                l++;
                c = (u8 *)(x * 6 + t);
                *(s16 *)(c + 0x2) = 0;
            }
        }
        func_8009D6F4();
        D_800E296C &= 0x3FF7FFFF;
        func_800403BC((s32)func_800CE028);
    }
    else
    {
        func_800997FC(D_800E1863);
    }
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    func_80098B38(q);
    return 1;
}

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

s32 func_800C4324(u8 *p, u8 *q, s16 k)
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
        h = func_800990FC();
        m = h;
        if (!(*(u32 *)(p + 0x1C) & 0x10))
        {
            h = func_80099194(D_800E188B, h);
        }
        func_80099290(h);
        func_800A5720(m);
    }
    func_80042B68((s8 *)p, 0x3);
    func_80098B38(q);
    g = (u8 *)&D_80083460;
    *(u16 *)(g + 0xA) -= 1;
    return 1;
}

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

/* Spawn the 0x202 tint node the kill handlers close on: stash the colour in
   D_800DCF78, run func_800B835C over the pair {0x1000340, 0x200020}, copy the
   dying mover's three coordinates into both the new node's mover and its own
   +0x40 record, and bump the engine's +0x0A counter. 65 words, 0 differing.

   THE FIRST ARGUMENT IS `(u8 *)&D_800DCF78 - 0x10`, and that spelling is what
   produces the target's THREE-instruction address: one `lui %hi`, one
   `addiu %lo` and a separate `addiu -0x10`, with the colour's own store
   reusing the same `%hi` register. A symbol of its own at 0x800DCF68 would
   fold into two.

   ONE TEMPORARY PER COORDINATE, NOT ONE REUSED. With a single `u16 t` for all
   three the three live quantities rotate -- the copy takes $a1 where the
   target has $v0, the record pointer $v1 where the target has $a1 and the
   mover $v0 where the target has $v1 -- 18 differing with every opcode right.
   Three locals is 0. The screen that proved it was the mode: `u8 t` alone
   reproduces the target's allocation exactly and leaves only the three `lbu`
   against `lhu`, 5 differing, which says the residue was never about the
   stores.

   AND THE COLOUR'S STORE COMES FIRST, above both stack-array stores: written
   last it swaps with the `sw 0x14($sp)` that fills the call's delay slot
   (2 differing), and written between them it also swaps the two constants'
   registers (7). */
void func_800C4AFC(u8 *m, s32 col, u8 *act)
{
    UnkNode *n;
    u8 *q;
    UnkVec *v;
    s32 arr[2];
    u16 t1;
    u16 t2;
    u16 t3;
    u8 *g;

    n = func_8003FD64(0x202, D_80083498);
    if (n != NULL)
    {
        D_800DCF78 = col;
        arr[0] = 0x1000340;
        arr[1] = 0x200020;
        func_800B835C((u8 *)&D_800DCF78 - 0x10, arr, 1, 0);
        q = (u8 *)n + 0x20;
        *(s16 *)(q + 0x2) = 0x40;
        v = n->unk_08;
        *(u32 *)((u8 *)n + 0x10) = (u32)func_800C4944;
        t1 = *(u16 *)(m + 0x2);
        *(s16 *)((u8 *)v + 0x2) = t1;
        *(s16 *)(q + 0x20) = t1;
        t2 = *(u16 *)(m + 0x6);
        *(s16 *)((u8 *)v + 0x6) = t2;
        *(s16 *)(q + 0x22) = t2;
        t3 = *(u16 *)(m + 0xA);
        *(s16 *)((u8 *)v + 0xA) = t3;
        *(s16 *)(q + 0x24) = t3;
        *(u8 **)(q + 0x1C) = m;
        *(u8 **)(q + 0x28) = act;
        func_800A56E0(0x702);
        g = (u8 *)&D_80083460;
        *(s16 *)(g + 0xA) += 1;
    }
}

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

/* Per-frame step: advance the mover by its own three deltas, damp the first
   two by a 1/8th subtract, fade the renderable's three colour bytes towards 0
   by 1/(remaining frames), and on the last frame drop the engine's +0xA
   counter and raise the node's 0x8000 flags. `*(s16 *)(p + 0x10)` is re-read
   for every division because each `sb` invalidates it, while `b->unk_10`
   survives from the `b->unk_04 +=` above it -- both fall out of the plain
   spelling. 86 words, 0 differing on the first compile. */
void func_800C5A64(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    s16 t;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C -= b->unk_0C >> 3;
    b->unk_10 -= b->unk_10 >> 3;
    func_800478B8(c);
    c[0xC] -= c[0xC] / *(s16 *)(p + 0x10);
    c[0xD] -= c[0xD] / *(s16 *)(p + 0x10);
    c[0xE] -= c[0xE] / *(s16 *)(p + 0x10);
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        if (*(s16 *)(p + 0xC) == 0 && *(s16 *)(p + 0x1C) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C5BBC);

/* The +0x10 countdown is the usual `lhu` / `addiu -1` / `sh` / `sll 16`+
   `bgtz` pair of statements through one s16 local; the colour fade is one s32
   local, `v -= v / n`. 55 words, 0 differing. */
void func_800C5D80(u8 *p, UnkVec *b, u8 *c)
{
    u8 *g;
    s16 t;
    s32 v;

    b->unk_08 += b->unk_14;
    b->unk_14 += 0x2000;
    func_800478B8(c);
    v = c[0xC];
    v -= v / *(s16 *)(p + 0x10);
    c[0xC] = v;
    c[0xD] = v;
    c[0xE] = v;
    t = *(u16 *)(p + 0x10) - 1;
    *(s16 *)(p + 0x10) = t;
    if (t <= 0)
    {
        if (*(s16 *)(p + 0xC) == 0)
        {
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) -= 1;
        }
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* THE TWO RANGE TESTS ARE abs TERNARIES, not `if (d < 0) d = -d;`: only
   `d >= 0 ? d : -d` reaches fold's ABS_EXPR, and only an ABS_EXPR's
   `bgez`/nop/`negu` carries its own delay slot. Written as an `if` the three
   words are a real branch, reorg steals the $a2 copy into its slot, and the
   function is 9 differing. D_80083780 needs a POINTER LOCAL -- read as
   `*(s16 *)(D_80083780 + 2)` the `%lo` folds into the load displacement and
   the second read walks the pointer, 53 words.

   THE THIRD PARAMETER IS s16 AND THE FOURTH IS s32, and only the CALLER says
   so: func_800C78A0 sign-extends $a2 across the `jal` and passes $a3 straight
   out of a `lw` with no extension. Both are stored with `sh` here, so the
   callee is 0 differing either way; `s16 d` costs func_800C78A0 two words.
   52 words, 0 differing. */
s32 func_800C77D0(void *a, u8 *b, s16 c, s32 d)
{
    u8 *q;
    u8 *s;
    u8 *p;
    s32 r;
    s32 dx;
    s32 dz;

    q = D_80083780;
    dx = *(s16 *)(q + 2) - *(s16 *)(b + 2);
    dx = dx >= 0 ? dx : -dx;
    if (dx >= 0xC1)
    {
        return 0;
    }
    dz = *(s16 *)(q + 6) - *(s16 *)(b + 6);
    dz = dz >= 0 ? dz : -dz;
    if (dz >= 0xC1)
    {
        return 0;
    }
    s = (u8 *)&D_80083178;
    p = s + 0xB8;
    r = (*(u32 *)(p + 0xC) != (u32)b);
    *(u32 *)(p + 0x34) = (u32)a;
    *(u32 *)(p + 0xC) = (u32)b;
    *(u32 *)(p + 0x28) = 0;
    *(u32 *)(s + 0xB8) = (u32)(s + 0xBC);
    if (*(u32 *)(p + 0x14) != 9)
    {
        r |= 2;
    }
    *(u32 *)(p + 0x14) = 9;
    *(u32 *)(p + 0x18) = 0;
    *(s16 *)(p + 0x26) = d;
    *(u32 *)(s + 0xB4) = (u32)func_800C75D0;
    if (r & 1)
    {
        D_800E3D20 = c;
    }
    *(s16 *)(p + 0x24) = c;
    return r;
}

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

/* Nothing happens unless D_80083780's position is within 0xC0 of the
   caller's on both axes. Both bounds are the `bgez`/nop/`negu` trio, so
   `abs()` with the same destination. The two tables are indexed
   `[(f >> 9) & 7]` on a u16 array -- cc1psx folds the element scale into the
   mask and emits `srl 8` / `andi 0xE`. The third parameter is s16, its
   `sll`/`sra` pair sitting at the forwarding call; the fourth is forwarded in
   $a3 with no setup. 67 words, 0 differing. */
s32 func_800C7930(u8 *a, u8 *b, s16 c, s32 d)
{
    u8 *q;
    u8 *v;
    u8 *e;

    q = D_80083780;
    if (abs(*(s16 *)(q + 0x2) - *(s16 *)(b + 0x2)) < 0xC1 &&
        abs(*(s16 *)(q + 0x6) - *(s16 *)(b + 0x6)) < 0xC1)
    {
        v = a + 0x20;
        e = D_800E58F8;
        *(s16 *)(e + 0x2) = *(u16 *)(b + 0x2) + (s16)D_800DCEAC[(*(u16 *)(v + 0x2A) >> 9) & 7] / 2;
        *(s16 *)(e + 0x6) = *(u16 *)(b + 0x6) + (s16)D_800DCEBC[(*(u16 *)(v + 0x2A) >> 9) & 7] / 2;
        *(s16 *)(e + 0xA) = *(u16 *)(b + 0xA);
        func_800C77D0(a, e, c, d);
    }
    return 0;
}

/* func_800C78A0's two-actor form: fire func_800C77D0 at the midpoint of the
   two actors' cell centres. THREE measured spellings carry it.

   (1) The two cell centres must be SEPARATE LOCALS. As one expression fold
   cancels the two +0x20s against each other and each axis loses its second
   `addiu`: 59 of 63 words.
   (2) THE SECOND AXIS'S ROUNDING HAS TO BE WRITTEN OUT. `q /= 2` compiles to
   a bare `sra`, not the target's `srl 31`/`addu`/`sra` triple, because
   nonzero_bits' MINUS case gives `result_low = MIN(low0, low1)` and
   `(byte << 6) + 0x20` has five low zero bits, so the rounding correction is
   provably redundant. Six spellings measure 61 words; only the two-statement
   form keeps it. The corollary: the FIRST axis's single `sra` is NOT evidence
   of a `>> 1` -- the two spellings are byte-identical there.
   (3) The frame is 0x20 where its one four-argument call needs 0x18, so there
   are 5..8 bytes of stack local nothing in the body reads -- a leftover
   declaration in the original. No local is 0x18 (4 differing), a 4-byte local
   is still 0x18 (6), an 8-byte local is 0x20 and 0. Only its SIZE is
   witnessed, never its type. 63 words, 0 differing. */
void func_800C7A3C(u8 *a, u8 *b, s16 c, s16 d, s16 e, s32 f)
{
    u8 *g;
    u8 *r;
    s32 p;
    s32 q;
    s32 x;
    s32 z;
    s32 t;
    s32 unused[2];

    p = (a[0x24] << 6) + 0x20;
    q = (b[0x24] << 6) + 0x20;
    q -= p;
    q >>= 1;
    x = p + q;
    g = D_80083780;
    t = *(s16 *)(g + 0x2) - x;
    if (t < 0)
    {
        t = -t;
    }
    if (t < 0xC1)
    {
        r = D_800E58F8;
        *(s16 *)(r + 0x2) = x;
        p = (a[0x25] << 6) + 0x20;
        q = (b[0x25] << 6) + 0x20;
        q -= p;
        q += (u32)q >> 31;
        q >>= 1;
        z = p + q;
        t = *(s16 *)(g + 0x6) - z;
        if (t < 0)
        {
            t = -t;
        }
        if (t < 0xC1)
        {
            *(s16 *)(r + 0x6) = z;
            *(s16 *)(r + 0xA) = c + (d - c) / 2;
            func_800C77D0(NULL, r, e, f);
        }
    }
}

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

/* The func_800C8844 probe's two locals (s32 draw, s16 remainder) with
   func_800C8900's report tail folded in and no leading gate call. 54 words,
   0 differing. */
s32 func_800C8CD8(u8 *p, s16 chance, s16 arg)
{
    s32 r;
    s16 roll;

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
        if (func_800A48F0((s8 *)p, 0x2, (s8)arg) >= 0)
        {
            func_80099844(p, D_800E1A55);
            if (p[0x13] == 0)
            {
                func_800DC1B8(D_800DCF10);
            }
            return 1;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8DB0);

/* The hunger/charge drain effect. The LAST func_80099194 is written in both
   arms so jump.c cross-jumps it into the one call the target has. The roll is
   func_800C8844's two forced locals, an s32 draw and an s16 remainder.
   78 words, 0 differing. */
s32 func_800C8EFC(u8 *p, s16 chance)
{
    s32 r;
    s16 roll;
    void *m;
    void *h;

    if (func_800C8078(p) == 0)
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
            if (p[0x26] >= 2)
            {
                p[0x26] = p[0x26] - 1;
                func_80041E70(p);
                h = func_800990FC();
                m = h;
                h = func_80099194(D_800E1A88, h);
                if (*(s32 *)(p + 0x14) & 0x4000)
                {
                    h = func_80099734(p, h);
                    h = func_80099194(D_800E1A9D, h);
                }
                else
                {
                    h = func_80099194(D_800E1AC2, h);
                    h = func_80099734(p, h);
                    h = func_80099194(D_800E1AD0, h);
                }
                func_80099290(h);
                func_800A5720(m);
                func_800A56E0(0x615);
                return 1;
            }
        }
    }
    return 0;
}

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

/* Pick the slot under the cursor with the +0x2000 draw bits raised around
   each engine call, then either re-enter func_800CA788 on the same slot or
   re-aim through func_800A0818 before closing on func_800CA444.

   THE FIRST PARAMETER IS HELD IN TWO LOCALS, and that is three words:
   `q = p;` with `q` used for every field access and for both calls' trailing
   argument, and the parameter itself used only as func_800CA444's first.
   With one variable the function is 83 words / 86 differing -- it saves five
   callee-saved registers instead of six and the frame is 0x30 rather than
   0x38. The two pseudos are simultaneously live to the last call, so
   local-alloc cannot coalesce them. 86 words, 0 differing. */
s32 func_800CA93C(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    u8 *e;
    u8 *t;

    q = p;
    *(u32 *)(q + 0x14) |= 0x2000;
    *(u32 *)(q + 0x1C) |= 0x2000;
    *(u32 *)(q + 0x60) = func_800A3D18(c, q, 4);
    *(u32 *)(q + 0x14) &= ~0x2000;
    *(u32 *)(q + 0x1C) &= ~0x2000;
    if (*(u32 *)(q + 0x60) != 0)
    {
        e = *(u8 **)(*(u8 **)(q + 0x60) - 0x14);
        *(u32 *)(q + 0x14) |= 0x2000;
        *(u32 *)(q + 0x1C) |= 0x2000;
        t = func_8009B4B0(q, c[0x24], c[0x25]);
        *(u32 *)(q + 0x14) &= ~0x2000;
        *(u32 *)(q + 0x1C) &= ~0x2000;
        if (t == *(u8 **)(q + 0x60))
        {
            func_800CA788(q, b, c, q);
            return 0;
        }
        *(s16 *)(q + 0x2A) = func_800A0818(c[0x24], c[0x25], e[0x24], e[0x25], (u16 *)(q + 0x98));
        *(u16 *)(q + 0x46) |= 0x8000;
    }
    func_800CA444(p, b, c, q);
    return 0;
}

/* 13 words, 0 differing. */
void func_800CAA94(UnkDungeonActor *p, void *b, void *c)
{
    p->unk_9A = 0x8;
    p->unk_96 = 0x40;
    p->unk_9B = 0;
    p->unk_8C = NULL;
    *(s32 *)((u8 *)p + 0x1C) = (*(s32 *)((u8 *)p + 0x1C) | 0x4000000) & ~0x1638;
}

/* A +0x8C handler. TWO THINGS COST SOMETHING. 0x80013714 is a CONSTANT
   ADDRESS here: the symbol's `lui` is a separate `high` insn that reorg puts
   in the `blez`'s delay slot, where the target has a `nop` and the two-word
   macro after it -- 21 differing. And the RETURNING arm has to be written
   FIRST: with the install arm first the tests come out `beq` and the call
   block goes out of line, 27 differing. 56 words, 0 differing. */
void func_800CAAC8(UnkDungeonActor *p, void *b, void *c, u8 *d)
{
    if (func_800AB1C0((u8 *)p, b, c, d) != 0)
    {
        func_800A4ACC(d);
        *(s16 *)((u8 *)p + 0xB6) -= 1;
        if ((*(s16 *)((u8 *)p + 0xB6) <= 0 || (*(u16 *)0x80013714 & 8) != 0) &&
            d[0x71] == *(s16 *)(d + 0x8A))
        {
            func_800CAA94(p, b, c);
            return;
        }
        p->unk_8C = func_800C9F34;
    }
    if ((D_80083462 & 0x80) != 0)
    {
        *(s16 *)((u8 *)p + 0x92) = -0x20;
    }
}

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_8008942C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089430);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CABA8);

/* Turn the mover towards func_800BCB04's bearing, or -- if it is already
   past it -- glide one frame and decay the three deltas by a quarter, with
   a 0x8000 climb on the third; the +0x96 countdown falls by 8 a frame and
   raises the node's redraw bit at zero. func_800D5CA0's glide with the
   deltas bled rather than halved. 67 words, 0 differing, first compile. */
void func_800CAED0(u8 *p, UnkVec *v)
{
    s16 r;

    r = func_800BCB04(*(u16 *)((u8 *)v + 0x2), *(u16 *)((u8 *)v + 0x6),
                      *(u16 *)((u8 *)v + 0xA) + 2);
    if (*(s16 *)((u8 *)v + 0xA) < r)
    {
        v->unk_00 += v->unk_0C;
        v->unk_04 += v->unk_10;
        v->unk_08 += v->unk_14;
        v->unk_0C -= v->unk_0C >> 2;
        v->unk_10 -= v->unk_10 >> 2;
        v->unk_14 += 0x8000;
    }
    else
    {
        *(s16 *)((u8 *)v + 0xA) = r;
    }
    if ((*(s16 *)(p + 0x96) -= 8) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Spawn TWO 0x212 sparks off the parent node, each jittered +/-0x20 from
   the parent's mover on all three axes, given a velocity from one angle
   through func_80064584/func_800644B8 and a lifetime of 0x80..0xFF frames.
   The loop is `for (i = 2; i > 0; i--)`, which is the `addiu $s3, -1` /
   `bgtz` pair at the bottom with no entry test. 101 words, 0 differing on
   the first compile. */
void func_800CAFDC(UnkNode *a, s16 b, s32 col)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *q;
    s32 i;
    s16 t;

    for (i = 2; i > 0; i--)
    {
        n = func_8003FD64(0x212, a);
        if (n != NULL)
        {
            n->unk_10 = (u32)func_800CAED0;
            func_8004491C(n, (u32)D_80045340);
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) =
                *(u16 *)((u8 *)a->unk_08 + 0x2) + (func_80069EF8() & 0x3F) - 0x20;
            *(s16 *)((u8 *)m + 0x6) =
                *(u16 *)((u8 *)a->unk_08 + 0x6) + (func_80069EF8() & 0x3F) - 0x20;
            *(s16 *)((u8 *)m + 0xA) =
                *(u16 *)((u8 *)a->unk_08 + 0xA) + (func_80069EF8() & 0x3F) - 0x20;
            t = func_80069EF8();
            *(s32 *)((u8 *)m + 0xC) = func_80064584(t) << 5;
            *(s32 *)((u8 *)m + 0x10) = func_800644B8(t) << 5;
            *(s16 *)((u8 *)m + 0x16) = -(func_80069EF8() & 7) - 8;
            r = (u8 *)n->unk_0C;
            *(s16 *)(r + 0x1E) = 0x1000;
            *(s16 *)(r + 0x1C) = 0x1000;
            *(u8 **)(r + 0x8) = D_800E03B0;
            *(s32 *)(r + 0xC) = col;
            *(s16 *)(r + 0x6) = 8;
            q = (u8 *)n + 0x20;
            *(s16 *)(q + 0xB0) = b;
            *(s16 *)(q + 0x96) = (func_80069EF8() & 0xFF) | 0x80;
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB170);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB374);

/* A 0x100 spawner whose mover is either SHARED with the parent node or
   copied from it a halfword at a time. THE COPY ARM RE-READS `a->unk_08` FOR
   EVERY FIELD and caches the destination once: a halfword store through a
   pointer invalidates every varying-address entry in cse's table. The last
   pair's order is load-bearing -- with `r->unk_10 = 0x60` written before the
   +0xC copy the two loads get no delay-slot filler and the function is 81
   against 80, 25 differing. 80 words, 0 differing. */
UnkNode *func_800CB4C0(UnkNode *a, s16 b)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FD64(0x100, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CB374;
        func_8004491C(n, (u32)D_80045340);
        r = (u8 *)n->unk_0C;
        func_8003DB94(r, D_800E02CC, 0);
        *(s16 *)(r + 0x1E) = 0x1400;
        *(s16 *)(r + 0x1C) = 0x1400;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x10) = 0x20;
        if (b == 0)
        {
            n->unk_08 = a->unk_08;
        }
        else
        {
            m = n->unk_08;
            *(s16 *)((u8 *)m + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
            *(s16 *)((u8 *)m + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
            *(s16 *)((u8 *)m + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
            *(u32 *)(r + 0xC) = *(u32 *)((u8 *)a->unk_0C + 0xC);
            *(s16 *)(r + 0x10) = 0x60;
        }
        *(s16 *)(r + 0x6) = -1;
        p = (u8 *)n + 0x20;
        *(UnkNode **)(p + 0xA8) = a;
        *(s16 *)(p + 0xB4) = b;
        *(s16 *)(p + 0x96) = 8;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB600);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB82C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CB9DC);

/* Spawn the kind-2 node the overlay's aimed-shot effect rides on.

   (1) The white has to be a LOCAL assigned before the mover is read: written
   at its store the `lui`/`ori` pair is emitted after the first tile centre's
   `andi`/`sll`/`addiu` instead of before it, 5 differing; one statement
   earlier is 68 words / 66, so the position is exact.
   (2) `v = x * 64; v = v + 0x20;` as two sets of ONE variable is what keeps
   the `addiu`: in one expression combine proves the shifted value's low bits
   clear and rewrites the add as `ori`. 66 words, 0 differing. */
UnkNode *func_800CBB98(u16 x, u16 y, s16 z, u8 *owner)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *g;
    s32 v;
    s32 col;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CB9DC;
        func_8004491C(n, (u32)D_80045340);
        col = 0x808080;
        m = n->unk_08;
        v = x * 64;
        v = v + 0x20;
        *(s16 *)((u8 *)m + 0x2) = v;
        v = y * 64;
        v = v + 0x20;
        *(s16 *)((u8 *)m + 0x6) = v;
        *(s16 *)((u8 *)m + 0xA) = z;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = col;
        *(s16 *)(r + 0x1E) = 0x1000;
        *(s16 *)(r + 0x1C) = 0x1000;
        func_8003DB94(r, D_800DF650, 0);
        *(u8 **)((u8 *)n + 0x20) = owner;
        *(s32 *)(owner + 0x14) |= 0x100000;
        func_800A56E0(0x614);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) += 1;
    }
    return n;
}

/* The aimed-shot attempt. THE ROLL IS TWO LOCALS WITH FORCED TYPES, exactly
   as func_800C8844 records: an s32 for the draw and an s16 for the remainder.

   DEVICE: the empty `__asm__ volatile("")` before the early `return 1` is
   worth ONE WORD and emits no instruction. Without it jump.c cross-jumps that
   `return 1`'s `addiu $v0, $zero, 1` into the identical one before the
   epilogue -- find_cross_jump's minimum is 1 for an unconditional jump
   (jump.c:2188) -- which leaves the early block a bare `j`; the conditional
   branch above it is then "a conditional jump jumping over an unconditional
   jump" and jump.c inverts it, so the function is 68 words against 69 with 58
   differing. The barrier breaks the insn match and both the `j` and its
   `addiu` survive. Ruled out at 68/58 each: `&&` and nested ifs for the guard
   (identical output), the guard as `||` round the whole body, the tail's
   `return 1` duplicated into both arms, an explicit `else` on the roll test,
   and returning a local set to 1. The barrier at the TAIL's return instead is
   also 68/58, so it is this block's match that has to be broken.
   69 words, 0 differing. */
s32 func_800CBCA0(u8 *p)
{
    s32 r;
    s16 roll;
    UnkNode *n;
    u8 *q;

    if (p[0x13] != 0 && (*(s32 *)(p + 0x14) & 0x4000) != 0)
    {
        __asm__ volatile("");
        return 1;
    }
    if (D_800E3D40 != 0)
    {
        roll = 0;
    }
    else
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
    }
    if (roll < 0x30)
    {
        q = *(u8 **)(p - 0x14);
        n = func_800CBB98(q[0x24], q[0x25], *(s16 *)(p + 0x88), p);
        if (n != NULL && p[0x13] == 0)
        {
            func_80094E34();
            func_80099844(p, D_800E1AE6);
        }
        return n != NULL;
    }
    if (p[0x13] == 0)
    {
        func_800A6508();
    }
    return 1;
}

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

/* The 0x615 shock effect: refuse while D_800E3D40 is up, otherwise draw a
   random number modulo the record's +0x03 and run the effect when it comes
   out under 0x30 and func_800A48F0 accepts. 77 words, 0 differing.

   `ret` IS NARROW. As `s32` the `sltiu` writes straight into its register
   and the test reads it there -- 75 words against 77. A narrow local makes
   the store a truncation and the read a widening, and because `sltiu` is
   provably 0 or 1 combine turns the widening into the target's plain
   `addu $v0, $s1, $zero`, which reorg then duplicates into the `j`'s delay
   slot. `s8` and `s16` both reach 0; `u8` and `u16` are 2 differing.
   The two-locals split for the draw is func_800C8844's, verbatim. */
s32 func_800CC058(u8 *p)
{
    u8 *q;
    s32 r;
    s16 roll;
    s16 ret;

    ret = 0;
    if (D_800E3D40 == 0)
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
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30 && func_800A48F0((s8 *)p, 7, -0x10) >= 0)
    {
        q = *(u8 **)(p - 0x14);
        if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
        {
            func_800C5BBC((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88),
                          0x202020, 0x40, 0);
            func_800A56E0(0x615);
        }
        if ((*(s32 *)(p + 0x14) & 0x4000) != 0)
        {
            func_80099844(p, D_800E1BBC);
        }
    }
    else
    {
        ret = p[0x13] == 0;
    }
    if (ret != 0)
    {
        func_800A6508();
    }
    return 1;
}

/* The same roll one effect further on, with a 0x40 threshold.
   (1) `k`, the 0/1/2 selector, has to be an `s16`: as s32 the two arms write
   it in place and the join's `addu $v1, $v0, $zero` disappears, 70 words
   against 71. `s8` also reaches 0; `u8` and `u16` are 1 differing.
   (2) The tail is a TERNARY, not an if/else pair -- `?:` keeps the copy that
   `if` deletes; as three flat returns it is 67 words / 30 differing.
   (3) The ternary's arms are `!= 0 ? 1 : -1`, which puts the -1 in the `beqz`
   delay slot; the `== 0 ? -1 : 1` spelling branches the other way,
   1 differing. 71 words, 0 differing. */
s32 func_800CC18C(u8 *p)
{
    s32 r;
    s16 roll;
    s16 flag;
    s16 k;

    flag = 0;
    if (D_800E3D40 != 0)
    {
        roll = 0;
    }
    else
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
    }
    if (roll < 0x40)
    {
        func_800AA5E4(p, *(void **)(p - 0x18), *(u8 **)(p - 0x14), p);
        func_800A56E0(0x705);
        if (D_800E3D40 != 0)
        {
            flag = -1;
        }
        k = p[0x13] == 0;
    }
    else
    {
        flag = 1;
        k = (p[0x13] == 0) * 2;
    }
    if (k != 0)
    {
        if (k == 1)
        {
            func_80099844(p, D_800E1BDA);
        }
        else
        {
            func_800A6508();
        }
    }
    if (flag < 0)
    {
        return 0;
    }
    return (flag != 0) ? 1 : -1;
}

/* func_800CD1A4's third sibling: the same chance roll through func_800C8CD8
   -- doubled while D_800E3D40 is up -- with no announcement. 50 words,
   0 differing. */
s32 func_800CC2A8(u8 *p)
{
    u8 *r;

    if (func_800C8CD8(p, D_800E3D40 != 0 ? 0x400 : 0x40, 0x10) == 0)
    {
        if (p[0x13] == 0)
        {
            func_800A6508();
        }
        return 1;
    }
    r = *(u8 **)(p - 0x14);
    if (*(u16 *)(r + 0x14) & 0x8000)
    {
        return 1;
    }
    func_800C5BBC((r[0x24] << 6) | 0x20, (r[0x25] << 6) | 0x20, *(s16 *)(p + 0x88), 0x802020, 0x20,
                  0);
    func_800A56E0(0x611);

    return 1;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC370);

/* A node's +0x10 per-frame step. The arity is the +0x10 convention
   func_80040190 casts to, since the only reference to this function anywhere
   is func_800CC5F0 TAKING its address. 64 words, 0 differing. */
void func_800CC4F0(u8 *p, UnkVec *m, u32 c)
{
    *(s16 *)((u8 *)m + 0xE) += func_80064584(*(s16 *)(p + 0x4)) >> 6;
    *(s16 *)((u8 *)m + 0x12) += func_800644B8(*(s16 *)(p + 0x4)) >> 6;
    if (*(s16 *)(p + 0x6) == 0)
    {
        func_800CC5F0(*(u16 *)((u8 *)m + 0x2) >> 6, *(u16 *)((u8 *)m + 0x6) >> 6,
                      *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x4), 0, 1);
        func_800CC5F0(*(u16 *)((u8 *)m + 0x2) >> 6, *(u16 *)((u8 *)m + 0x6) >> 6,
                      *(s16 *)((u8 *)m + 0xA), *(s16 *)(p + 0x4), 1, 1);
    }
    *(s16 *)(p + 0x2) -= 1;
    if (*(s16 *)(p + 0x2) <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* The 0x202 spawner for func_800CC4F0's pair of markers -- func_800CC88C
   three functions down is the line-for-line template, including the
   `*(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);` read-back and the two
   `(e != 0) ? :` ternaries off the same fifth argument.

   `*(u32 *)(r + 0xC) = 0x808080;` MUST COME BEFORE `*(u8 **)(r + 0x8)`:
   the other order is 85 words / 4 differing, because the `li` for 0x808080 is
   split into `lui`/`ori` after reload and sched2 then places the `ori` one
   slot early. The six-argument signature is read off the landed caller
   func_800CC4F0, which passes `(x >> 6, y >> 6, ang, d, 0|1, 1)`; adding the
   prototype leaves that caller at 64 words / 0 differing. 85 words,
   0 differing. */
UnkNode *func_800CC5F0(s16 x, s16 y, s16 ang, s16 d, s16 e, s16 f)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FC64(0x202);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CC4F0;
        func_8004491C(n, (u32)func_800CEEFC);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xE) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x12) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        *(s16 *)((u8 *)m + 0x16) = ang;
        r = (u8 *)n->unk_0C;
        *(u32 *)(r + 0xC) = 0x808080;
        *(u8 **)(r + 0x8) = D_800E03D4;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1E) = (e != 0) ? 0x1000 : 0x400;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
        *(s16 *)(r + 0x10) = (e != 0) ? 0x60 : 0x20;
        *(s16 *)(r + 0x6) = 0x20;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4) = d;
        *(s16 *)(p + 0x2) = 8;
        *(s16 *)(p + 0x6) = f;
    }
    return n;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC744);

/* The kind-2 marker spawner. The 0x1C scale is a READ-BACK of the 0x1E one
   (`sh` then `lhu` with the load-delay `nop`), not a second copy of the
   ternary. The positions are s16 with `x * 64 + 0x20`, func_800C1A44's form
   -- the `sll 16`/`sra 10` pair is the narrow parameter's sign-extension
   folded into the multiply. 76 words, 0 differing on the first compile. */
UnkNode *func_800CC88C(s16 x, s16 y, s16 ang, s16 d, s16 e)
{
    UnkNode *n;
    UnkVec *m;
    u8 *r;
    u8 *p;

    n = func_8003FC64(2);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800CC744;
        func_8004491C(n, (u32)func_80045C34);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = x * 64 + 0x20;
        *(s16 *)((u8 *)m + 0x6) = y * 64 + 0x20;
        *(s16 *)((u8 *)m + 0xA) = ang;
        r = (u8 *)n->unk_0C;
        *(u8 **)(r + 0x8) = D_800E03D4;
        *(u16 *)(r + 0x14) |= 0xC;
        *(s16 *)(r + 0x1E) = (e != 0) ? 0x6000 : 0x3000;
        *(s16 *)(r + 0x1C) = *(u16 *)(r + 0x1E);
        *(s16 *)(r + 0x10) = (e != 0) ? 0x60 : 0x20;
        *(s16 *)(r + 0x6) = 0x20;
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x4) = d;
        *(s16 *)(p + 0x2) = 4;
        *(s16 *)(p + 0x6) = e;
    }
    return n;
}

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
        func_800CD994(*(u8 **)p, 0x10);
        g = (u8 *)&D_80083460;
        *(u16 *)(g + 0xA) -= 1;
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* Apply effect 0xA to the record unless it is kind 0x2E. The restore pair is
   the point: +0x13 is read before func_800A48F0, read again after it, the OLD
   value is written back for the announcement and the NEW one put back
   afterwards. 55 words, 0 differing. */
s32 func_800CD994(u8 *p, s16 e)
{
    s32 t;
    s32 u;

    if (p[0x13] != 0x2E)
    {
        if (p[0x13] == 0x1E)
        {
            if (*(s32 *)(p + 0xA4) != 0)
            {
                return 0;
            }
            if (*(s16 *)(p + 0xAA) != 0)
            {
                return 0;
            }
        }
        if ((s16)func_80042900((s8 *)p, 0xA) != 0)
        {
            return 0;
        }
        t = p[0x13];
        if (func_800A48F0((s8 *)p, 0xA, (s8)e) < 0)
        {
            return 0;
        }
        if (*(u32 *)(p + 0x14) & 0x4000)
        {
            u = p[0x13];
            p[0x13] = t;
            func_80099844(p, D_800E1C25);
            p[0x13] = u;
        }
    }
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDA70);

/* The random charge roll on the player record. THE REMAINDER IS AN `s16`
   LOCAL, the same split func_800C8844 records: as `s32 roll` the `mfhi` lands
   straight in $s0 and the function is 59 words against 61 with 40 differing.
   D_800E3D7C is dereferenced INLINE while D_80083460 takes a local: a local
   for both is 61 / 2. 61 words, 0 differing. */
s32 func_800CDC18(u8 *p)
{
    s32 r;
    s16 roll;
    u8 *g;

    if (p[0x13] == 0)
    {
        roll = 0;
        if (D_800E3D40 == 0)
        {
            r = func_800A6D30();
            if (p[3] != 0)
            {
                roll = r % p[3];
            }
        }
        if ((s16)func_80042900((s8 *)p, 0xA) != 0)
        {
            roll = 0xFF;
        }
        if (roll < 0x30)
        {
            *(u16 *)(D_800E3D7C + 0xA2) |= 0x100;
            g = (u8 *)&D_80083460;
            *(u16 *)(g + 0xA) += 1;
        }
        else if (p[0x13] == 0)
        {
            func_800A6508();
        }
    }
    return 1;
}

/* The func_800C8844 probe's s32 draw and s16 remainder again, here gated on
   D_800E3D40 as well as on +0x3, so the two `roll = 0` arms cross-jump into
   one block. 64 words, 0 differing. */
s32 func_800CDD0C(u8 *p)
{
    u8 *q;
    s32 r;
    s16 roll;

    if (D_800E3D40 == 0)
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
    }
    else
    {
        roll = 0;
    }
    if (roll < 0x30)
    {
        func_800C8DB0(p, D_800E3D40 != 0 ? 0x400 : 0x20, 0x10);
        q = *(u8 **)(p - 0x14);
        if ((*(u16 *)(q + 0x14) & 0x8000) == 0)
        {
            func_800C5E5C((q[0x24] << 6) | 0x20, (q[0x25] << 6) | 0x20, *(s16 *)(p + 0x88),
                          D_800DF870, 0);
            func_800A56E0(0x70A);
        }
    }
    else if (p[0x13] == 0)
    {
        func_800A6508();
    }
    return 1;
}

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

/* Charge one of the 4-byte D_800E3648 slots and spawn the node that spends
   it. THE ROW POINTER IS `base + index`, NOT AN ARRAY REF: `&D_800E3648[i*4]`
   expands the symbol AFTER the index shift and commutes the `addu`, 5
   differing. The node must also be a `u8 *` with cast stores: as `UnkNode *n`
   the COMPONENT_REF lets sched1 hoist the unrelated D_80083460 load above it,
   13 differing. 58 words, 0 differing. */
s32 func_800CEE0C(u8 *p, s16 i)
{
    u8 *n;
    u8 *g;
    u8 *b;
    u8 *e;

    if (p[0x13] != 0)
    {
        return 1;
    }
    g = (u8 *)&D_80083460;
    b = D_800E3648;
    e = b + i * 4;
    if ((s16)(*(u16 *)(g + 0x1C) + *(s8 *)(e + 2)) >= 0x21)
    {
        *(s8 *)(e + 2) = 0x20 - *(u8 *)(g + 0x1C);
    }
    if (func_8003FA44(*(s8 *)(e + 2) + 1) == 0)
    {
        return 0;
    }
    n = (u8 *)func_8003FC64(2);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800CEA44;
        *(s16 *)(n + 0x2C) = i;
        *(u16 *)(g + 0xA) += 1;
        func_80053DA8(0xB1);
    }
    return n != NULL;
}

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
            func_800AA258(a, b, c, d);
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

/* func_800D4AB0's step: re-seed the spawned node's mover from the owner's
   name record, tint the renderable and fade it, then raise the kill flag on
   two conditions. THE OWNER'S FLAG WORD IS REACHED THROUGH AN `UnkNode *` AT
   `q - 0x20`, NOT AS `*(u16 *)(q - 0x2)`: the listing keeps `q - 0x20` in
   $s1 across the call and reads `0x1E($s1)`, and the folded spelling is
   86 words / 60 differing -- a word short, with everything after the call
   shifted. Writing the 0x1E/0x1C pair as two statements and as one chained
   assignment are both 0 differing, so that order is a TIE. 87 words,
   0 differing. */
void func_800D4954(u8 *p, UnkVec *b, u8 *c)
{
    u8 *q;
    UnkNode *n;
    u8 *s;

    q = *(u8 **)(p + 0x88);
    n = (UnkNode *)(q - 0x20);
    s = *(u8 **)(q - 0x18);
    *(s16 *)((u8 *)b + 0x2) = *(u16 *)(s + 0x2);
    *(s16 *)((u8 *)b + 0x6) = *(u16 *)(s + 0x6);
    *(s16 *)((u8 *)b + 0xA) = *(u16 *)(s + 0xA) - 0x32;
    c[0xE] = 0x28;
    c[0xD] = 0x28;
    c[0xC] = 0x28;
    *(s16 *)(c + 0x1A) += 0x190;
    if (*(u16 *)(p + 0x5A) & 1)
    {
        c[0xC] = 0x64;
    }
    if (*(u16 *)(p + 0x5A) & 2)
    {
        c[0xD] = 0x64;
    }
    if (*(u16 *)(p + 0x5A) & 4)
    {
        c[0xE] = 0x64;
    }
    if (*(u16 *)(c + 0x1C) < 0x1800)
    {
        *(s16 *)(c + 0x1E) += 0x190;
        *(s16 *)(c + 0x1C) = *(u16 *)(c + 0x1E);
    }
    if ((s16)func_80042900((s8 *)q, 0x1D) == 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
    if (n->unk_1E & 0x8000)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

/* func_800D4FC8's spawner over a SECOND actor. The +0x88 field is read TWICE
   because func_8004491C sits between the two reads; the 12-byte name copy is
   UnkDungeonName12, whose alignment of 1 produces the `lwl`/`lwr` +
   `swl`/`swr` pairs. 73 words, 0 differing. */
void func_800D4AB0(u8 *a)
{
    UnkNode *n;
    u8 *p;
    u8 *s;
    UnkVec *m;
    UnkSlot *r;

    n = func_8003FC64(0x12);
    if (n != NULL)
    {
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x5A) = 0x78;
        *(s16 *)(p + 0x5C) = 0x78;
        *(s32 *)(p + 0x88) = *(s32 *)(a + 0x88);
        n->unk_10 = (u32)func_800D4954;
        func_8004491C(n, (u32)D_80045340);
        r = (UnkSlot *)n->unk_0C;
        r->unk_10 = 0x60;
        r->unk_06 = 0;
        r->unk_14 |= 0xC;
        s = *(u8 **)(*(u8 **)(a + 0x88) - 0x18);
        m = n->unk_08;
        *(s16 *)((u8 *)m + 0x2) = *(u16 *)(s + 0x2);
        *(s16 *)((u8 *)m + 0x6) = *(u16 *)(s + 0x6);
        *(s16 *)((u8 *)m + 0xA) = *(u16 *)(s + 0xA) - 0x32;
        r = (UnkSlot *)n->unk_0C;
        *((u8 *)r + 0xE) = 0x80;
        *((u8 *)r + 0xD) = 0x80;
        *((u8 *)r + 0xC) = 0x80;
        r->unk_1E = 0;
        r->unk_1C = 0;
        *(UnkDungeonName12 *)((u8 *)n + 0x96) = *(UnkDungeonName12 *)D_800E233C;
        r->unk_08 = (u32)((u8 *)n + 0x96);
    }
}

/* Wire a newly spawned node to its owner. TWO THINGS THE LISTING FORCES.
   `n->unk_0C` is READ TWICE -- the halfword stores through the vector pointer
   wipe cse's memory table -- and so is `*(u32 *)(b + 0xC)`, for the same
   reason across the store to the view. The last four stores were swept
   exhaustively: of the 24 orders only this one and its 1-3-4-2 sibling reach
   0, the reader's order is 7 differing and three orders are a word short
   because the two `b + 0xC` reads merge. 55 words, 0 differing. */
void func_800D4BD4(UnkNode *n, u8 *b, u8 *c)
{
    u8 *p;
    u8 *r;
    UnkVec *m;

    p = (u8 *)n + 0x20;
    *(u8 **)(p + 0x24) = b;
    func_8004491C(n, (u32)D_80045340);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x10) = 0x20;
    *(u16 *)(r + 0x14) |= 0xC;
    m = n->unk_08;
    *(s16 *)((u8 *)m + 0x2) = *(u16 *)(c + 0x2);
    *(s16 *)((u8 *)m + 0x6) = *(u16 *)(c + 0x6);
    *(s16 *)((u8 *)m + 0xA) = *(u16 *)(c + 0xA);
    r = (u8 *)n->unk_0C;
    *(s16 *)(r + 0x1E) = 0x800;
    *(s16 *)(r + 0x1C) = 0x800;
    *(u32 *)(p + 0xC) = *(u32 *)(b + 0xC);
    *(u32 *)(r + 0xC) = *(u32 *)(b + 0xC);
    *(s16 *)(r + 0x12) = 0x7DCF;
    *(u16 *)(r + 0x14) |= 0x100;
    func_8003DB94(r, D_800DECF8, 0);
}

/* One frame of the two-record glide. The countdown is func_800C5D80's idiom,
   and the life pair at +0x1E/+0x20 is RE-READ for each of the three channels
   rather than cached, which is what the three `lh`/`lh` pairs are.
   78 words, 0 differing. */
void func_800D4CB0(u8 *p, UnkVec *b, u8 *c)
{
    s16 t;

    b->unk_00 += b->unk_0C;
    b->unk_04 += b->unk_10;
    b->unk_08 += b->unk_14;
    b->unk_0C /= 2;
    b->unk_10 /= 2;
    b->unk_14 /= 2;
    c[0xC] = p[0xC] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xD] = p[0xD] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    c[0xE] = p[0xE] * *(s16 *)(p + 0x1E) / *(s16 *)(p + 0x20);
    t = *(u16 *)(p + 0x1E) - 1;
    *(s16 *)(p + 0x1E) = t;
    if (t <= 0)
    {
        *(u16 *)(p - 0x2) |= 0x8000;
        D_800814A0 |= 0x8000;
    }
}

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

/* Is the tile one step along the record's facing blocked? The direction
   decodes out of the mover's +0x2A exactly as func_8009B164 decodes it, and
   D_800E2468 is the interleaved (dx, dz) table for the eight directions.
   58 words, 0 differing.

   THREE SPELLINGS, ALL MEASURED, AND EACH ONE IS WORTH REGISTERS RATHER
   THAN WORDS -- the function is 58 words under every spelling below.
   (1) THE TWO SUMS ARE `s32` LOCALS AND THE TRUNCATION IS AN EXPLICIT
   `& 0xFFFF` INTO AN `s32`. Through `u16 x`/`u16 y` the HImode pseudo
   outranks nothing: the row pointer wins `$v1`, the x sum is pushed to
   `$a0`, and 17 words differ with every opcode already right. The mask form
   gives the row pointer `$a1` -- the incoming `b` register -- and the sums
   `$v1`, which is the target. Writing the sums straight into the u16 locals
   is the same 17.
   (2) `t[0] * 64`, NOT `t[0] << 6`. The shift lets the final mask propagate
   back through force_to_mode and the table reads come out `lhu` where the
   target has `lh`; `* 64` is a MULT, which force_to_mode does not narrow.
   Four loads change, no word count.
   (3) THE TABLE BASE IS ITS OWN STATEMENT, ahead of the index. Inline --
   `D_800E2468[(... >> 9) & 7]` -- fold puts the ADDR_EXPR second, the
   `srl`/`andi` are emitted before the `lui`/`addiu`, and that is 4 words.
   And the tail is TWO `if`s with a `||`, not three nested tests: nested,
   `r` takes `$a0` and the reloaded +0xA takes `$v1` instead of the other
   way round, 6 differing. */
s32 func_800D5F80(u8 *p, u8 *b)
{
    s16 *t;
    s16(*tb)[2];
    s32 k;
    s32 u;
    s32 u0;
    s32 u1;
    s16 c;
    s32 x;
    s32 y;
    s16 r;

    u = *(s16 *)(p + 0xA) - 0x20;
    c = u;
    tb = D_800E2468;
    k = (*(u16 *)(b + 0x2A) >> 9) & 7;
    t = tb[k];
    u0 = *(s16 *)(p + 0x2) + t[0] * 64;
    x = u0 & 0xFFFF;
    u1 = *(s16 *)(p + 0x6) + t[1] * 64;
    y = u1 & 0xFFFF;
    if (func_800A45D8(x, y, c) != 0)
    {
        return 1;
    }
    r = (s16)func_800BCB04(x, y, c);
    if (r >= 0x200 || *(s16 *)(p + 0xA) - 0x40 >= r)
    {
        return 1;
    }
    return 0;
}

/* State 0x18. The two `(s16)` casts are the whole shape of the tests:
   func_800A2B5C is declared s32 and func_800A48F0 s16, and every test here is
   on the low halfword. 66 words, 0 differing. */
void func_800D6068(u8 *p, void *b, u8 *c, u8 *d)
{
    d[0x71] &= 0x7F;
    if ((D_80083462 & 0x2000) == 0)
    {
        if ((s16)func_800A2B5C(d) == 0)
        {
            if ((s16)func_800A2B5C(d) == 0)
            {
                if ((s16)func_800A48F0((s8 *)d, 0x1B, 0) < 0)
                {
                    *(u16 *)(d + 0x46) &= 0x7FFF;
                }
                else
                {
                    *(s32 *)(p + 0x8C) = 0;
                    p[0x9A] = 0x18;
                    p[0x9B] = 0;
                    *(s16 *)(p + 0x96) = 0;
                    p[0xB3] = 0;
                    p[0xB1] = 0;
                    p[0xB2] = 0;
                    *(s32 *)(d + 0x1C) &= ~0x40000;
                    *(u16 *)(p + 0x98) |= 8;
                    func_80042B68((s8 *)d, 5);
                    func_800A4ACC(d);
                    d[0x6D]--;
                }
            }
        }
    }
}

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

/* One frame of the two-record fade. THE STEP IS RE-READ, NOT CACHED: a local
   for it is 58 words against 60, 59 differing -- the parameter never gets its
   own $a3 copy and the divide loses the register copy gcc keeps only when the
   quotient is written back into the dividend. 60 words, 0 differing. */
void func_800D6804(u8 *p, u8 *q)
{
    *(s32 *)(q + 0x8) += *(s32 *)(q + 0x14);
    *(s32 *)(q + 0x14) = *(s32 *)(q + 0x14) * 3 / 4;
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

/* A 0x211 spawner that seeds its mover from the parent's and then offsets
   each axis.

   THE FIFTH ARGUMENT IS `s16`, AND THAT IS TWO WORDS. Declared s32 the stack
   parameter is rematerialised at its single use instead of being held in
   $s4, the function saves one callee-saved register fewer, and it comes out
   79 against 81 with 77 differing. The caller only ever passes `x >> 16`,
   whose sign extension gcc proves redundant, so no shift pair appears at the
   call site to witness the type -- the callee's word count is the only
   witness. The +0xC/+0x10 clear is a CHAINED assignment: as two statements
   the second reloads the mover, 83 words / 30 differing. 81 words,
   0 differing. */
void func_800D6AD4(UnkNode *a, u32 b, s16 dx, s16 dy, s16 dz, s16 f)
{
    UnkNode *n;
    u8 *p;

    n = func_8003FD64(0x211, a);
    if (n != NULL)
    {
        n->unk_10 = (u32)func_800D6804;
        *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)((u8 *)a->unk_08 + 0x2);
        *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)((u8 *)a->unk_08 + 0x6);
        *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)((u8 *)a->unk_08 + 0xA);
        *(s16 *)((u8 *)n->unk_08 + 0x2) += dx;
        *(s16 *)((u8 *)n->unk_08 + 0x6) += dy;
        *(s16 *)((u8 *)n->unk_08 + 0xA) += dz;
        *(s32 *)((u8 *)n->unk_08 + 0xC) = *(s32 *)((u8 *)n->unk_08 + 0x10) = 0;
        *(s32 *)((u8 *)n->unk_08 + 0x14) = 0xFFFA0000 - (f << 18);
        p = (u8 *)n + 0x20;
        *(s16 *)(p + 0x32) = 0x14;
        *(s16 *)(p + 0x34) = 0x14;
        func_8004491C(n, (u32)func_800D68F4);
        *(u32 *)p = b;
        *(u32 *)(p + 0x8) = b;
    }
}

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

/* FOUR PARAMETERS, forwarded. func_800AC480 reads all four argument
   registers; this caller only passes them through, so the wrong arity was
   invisible until that function was read. 14 words, 0 differing either way. */
void func_800DA728(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800AC480((u8 *)p, b, c, d) != 0)
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

/* FOUR PARAMETERS, forwarded, exactly as the func_800AC480 pair next door:
   func_800ACE34 reads $a0 and $a2 and this caller only passes them through.
   14 words, 0 differing either way. */
void func_800DA798(UnkDungeonActor *p, UnkVec *b, u8 *c, u8 *d)
{
    if (func_800ACE34((u8 *)p, b, c, d) != 0)
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

/* A 2x8 nested countdown; the inner `i = 7` reinit sits in the OUTER back
   branch's delay slot, which is what the two nested `for`s give. The source
   object is an alignment-1 8-byte struct, so the copy is the `lwl`/`lwr` +
   `swl`/`swr` pair. 63 words, 0 differing. */
void func_800DA840(UnkDungeonName8 *src, s16 a)
{
    UnkNode *n;
    u8 *q;
    s32 i;
    s32 j;
    s32 t;

    for (j = 1; j >= 0; j--)
    {
        for (i = 7; i >= 0; i--)
        {
            n = func_8003FC64(2);
            if (n != NULL)
            {
                q = (u8 *)n + 0x20;
                *(s16 *)q = 0;
                *(s16 *)(q + 0x2) = 0x20;
                *(s16 *)(q + 0x8) = j;
                t = i << 9;
                if (j == 0)
                {
                    t += 0x100;
                }
                *(s16 *)(q + 0xA) = t;
                *(UnkDungeonName8 *)(q + 0x20) = *src;
                *(s16 *)(q + 0x6) = func_8006649C(a << 4, 0x1F8);
                n->unk_10 = (u32)func_800DAB50;
                func_8004491C(n, (u32)func_800DA93C);
            }
        }
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DA93C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DAB50);

/* Spawn the 0x312 effect at the mover's position. THE NODE POINTER IS BUMPED
   IN PLACE -- `n += 0x20` -- rather than taken into a second local: the
   target's `addiu $s0, $s0, 0x20` reuses the one register, which two locals
   do not share. The three `n->unk_08` reads are written out rather than
   cached. 44 words, 0 differing. */
void func_800DAE44(u8 *p, s16 v)
{
    u8 *n;

    n = (u8 *)func_8003FD64(0x312, D_80083498);
    if (n != NULL)
    {
        *(u32 *)(n + 0x10) = (u32)func_800DAEF4;
        *(s16 *)(*(u8 **)(n + 0x8) + 0x2) = *(u16 *)(p + 0x2);
        *(s16 *)(*(u8 **)(n + 0x8) + 0x6) = *(u16 *)(p + 0x6);
        *(s16 *)(*(u8 **)(n + 0x8) + 0xA) = *(u16 *)(p + 0xA);
        n += 0x20;
        *(s16 *)(n + 0x10) =
            func_800BCB04(*(u16 *)(p + 0x2), *(u16 *)(p + 0x6), (s16)(*(u16 *)(p + 0xA) - 0x80));
        *(s16 *)(n + 0x48) = v;
    }
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DAEF4);

/* The spawned marker's +0x10 step: a three-case state machine on +0x4C,
   driven by the +0x48 countdown the function spends on entry. Case 0 jitters
   the renderable's two scales, case 1 bleeds the colour word and the mover's
   +0x14 until the view's height passes +0x10, case 2 kills the node. The
   tail adds the remaining delta once the state has left 0.

   EACH RANDOM DELTA IS ITS OWN STATEMENT. Written inline,
   `field - ((func_80069EF8() & 0xFF) + 0x300)`, fold reassociates it into
   `(field - 0x300) - draw` and the constant is folded into the field's own
   `addiu` -- 6 differing, three words per arm in the wrong order. With the
   delta computed first the target's `andi` / `lhu` / `addiu 0x300` / `subu`
   comes out directly. (Holding the 0x300 in a local works too, and so does
   an s16 delta; all four spellings measured 0.)

   BOTH `*(s16 *)(p + 0x48) <= 0` TESTS ARE THE SAME SOURCE. In case 2 no
   call intervenes, so cse forwards the stored value and the test is a bare
   `sll 16` on the register; in case 0 the two func_80069EF8 calls kill the
   equivalence and the field is reloaded with `lh`. 94 words, 0 differing. */
void func_800DB164(u8 *p, UnkVec *v, UnkSlot *r)
{
    s32 d;

    *(s16 *)(p + 0x48) = *(u16 *)(p + 0x48) - 1;
    switch (*(s16 *)(p + 0x4C))
    {
    case 0:
        d = (func_80069EF8() & 0xFF) + 0x300;
        *(s16 *)((u8 *)r + 0x1C) = *(u16 *)((u8 *)r + 0x1C) - d;
        d = (func_80069EF8() & 0xFF) + 0x200;
        *(s16 *)((u8 *)r + 0x1E) = *(u16 *)((u8 *)r + 0x1E) + d;
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 1:
        *(s32 *)((u8 *)r + 0xC) += 0xFFF3F3F4;
        v->unk_14 -= (func_80069EF8() & 0xFF) << 10;
        if (*(s16 *)((u8 *)v + 0xA) < *(s16 *)(p + 0x10))
        {
            *(s16 *)(p + 0x48) = 4;
            *(s16 *)(p + 0x4C) = *(u16 *)(p + 0x4C) + 1;
        }
        break;
    case 2:
        if (*(s16 *)(p + 0x48) <= 0)
        {
            *(u16 *)(p - 0x2) |= 0x8000;
            D_800814A0 |= 0x8000;
        }
        break;
    }
    if (*(s16 *)(p + 0x4C) != 0)
    {
        v->unk_08 += v->unk_14;
        func_800478B8((u8 *)r);
    }
}

/* Two passes of four 0x312 nodes each, the outer counter stepping by 3 so
   that it is 3 then 0, which is what the `sltu $zero, $s1` stored at +0x24 of
   each payload reports.

   THE FOURTH PARAMETER IS NARROW. Declared s32 the function is still 81 words
   but 13 differing, with `d` and the two loop invariants rotated through
   $s4..$s7 the other way. Nothing calls this function anywhere in the
   overlay, so the width is not witnessed and s16 is the guess. THE THREE
   READS OFF THE SECOND ARGUMENT ARE IN SOURCE ORDER +0x28, +0x14, +0x12, and
   the +0x28 copy must come FIRST: a load off `b` cannot be lifted over a
   store through `r`, so with the 0x1E/0x1C scale written ahead of it cc1psx
   emits a load-delay `nop` and the function is 83 words. 81 words,
   0 differing. */
void func_800DB2DC(u8 *a, u8 *b, u8 *c, s16 d)
{
    UnkNode *n;
    u8 *q;
    u8 *r;
    s32 i;
    s32 j;

    for (j = 3; j >= 0; j -= 3)
    {
        for (i = 3; i >= 0; i--)
        {
            n = func_8003FD64(0x312, D_80083498);
            if (n != NULL)
            {
                n->unk_10 = (u32)func_800DB420;
                *(s16 *)((u8 *)n + 0x4A) = *(u16 *)(c + 0x2A);
                r = (u8 *)n->unk_0C;
                *(u32 *)(r + 0x28) = *(u32 *)(b + 0x28);
                *(s16 *)(r + 0x1E) = 0x1000;
                *(s16 *)(r + 0x1C) = 0x1000;
                *(s16 *)(r + 0x14) = *(u16 *)(b + 0x14);
                *(s16 *)(r + 0x12) = *(u16 *)(b + 0x12) - 0x80;
                *(u32 *)(r + 0xC) = 0x808080;
                *(u16 *)(r + 0x14) |= 0xC;
                *(u8 **)((u8 *)n + 0x20) = a;
                *(s16 *)((u8 *)n->unk_08 + 0x2) = *(u16 *)(a + 0x2);
                *(s16 *)((u8 *)n->unk_08 + 0x6) = *(u16 *)(a + 0x6);
                *(s16 *)((u8 *)n->unk_08 + 0xA) = *(u16 *)(a + 0xA);
                q = (u8 *)n + 0x20;
                *(s16 *)(q + 0x6) = d;
                *(s16 *)(q + 0x4) = (j != 0);
                *(u32 *)(r + 0x2C) = *(u32 *)(b + 0x2C);
            }
        }
    }
}

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

/* Republish the two glyph indices whenever the engine's record has moved on.
   THE COMPARISON IS WRITTEN GLOBAL-FIRST: the other order is 2 differing,
   both `beq` operands swapped, and the two loads come out in the same order
   either way -- so the register order of the branch is the only witness.
   43 words, 0 differing. */
void func_800DC0BC(u8 *p, u8 **q)
{
    if (*(u8 *)(D_800814A8 + 0x28) != *(s32 *)(p + 0x10))
    {
        func_800DC078(q[0], *(u8 *)(D_800814A8 + 0x28));
        *(s32 *)(p + 0x10) = *(u8 *)(D_800814A8 + 0x28);
    }
    if (*(u8 *)(D_800814A8 + 0x29) != *(s32 *)(p + 0x14))
    {
        func_800DC078(q[0] + 0xC, *(u8 *)(D_800814A8 + 0x29));
        *(s32 *)(p + 0x14) = *(u8 *)(D_800814A8 + 0x29);
    }
}

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
void func_800DC628(u8 *p, s32 v)
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

/* Step the tag builder's facing toward the target angle in units of 0x80.

   THE STORE THROUGH D_800E5910 IS WRITTEN IN BOTH ARMS. Hoisted into one
   assignment after the if/else it is 53 words / 9 differing: the call result
   needs a copy into $v1 and the pointer load's delay slot gets filled.
   Duplicated into the two arms, cross-jumping puts the tail back. The `>> 1`
   is UNSIGNED, so the flag word is read as u32. func_800DC628's second
   parameter had to be WIDENED from s16 to s32: the target passes $s0 raw with
   no `sll 16`/`sra 16`. 53 words, 0 differing. */
void func_800DC650(u8 *p)
{
    u8 *s;
    s32 cur;
    s32 t;
    s32 d;

    s = p + 0x3C;
    cur = *(s16 *)(*(u8 **)(s + 0x8) + 0xE);
    t = *(s16 *)((*(s32 *)(p + 0x50) & 2) + (u8 *)D_800E2934);
    d = t - cur;
    if (d < 0)
    {
        if (d < -0x80)
        {
            cur -= 0x80;
        }
        else
        {
            cur = t;
        }
    }
    else
    {
        if (d < 0x81)
        {
            cur = t;
        }
        else
        {
            cur += 0x80;
        }
    }
    if (cur != t)
    {
        *(s32 *)(p + 0x58) = 1;
        *(s32 *)(D_800E5910 + 0xC) = (s32)func_800DCA1C(*(u8 **)(p + 0x3C), 0);
    }
    else
    {
        *(s32 *)(D_800E5910 + 0xC) =
            (s32)func_800DCA1C(*(u8 **)(p + 0x3C), ((*(u32 *)(p + 0x50) >> 1) ^ 1) & 1);
    }
    func_800DC628(s, cur);
}

/* Ease the tag window's two screen coordinates toward the pair D_800E2924
   holds for the current corner.

   ONE LOCAL PER READ-BACK, NOT ONE REUSED. With a single `s32 v` for both
   halves the two disjoint live ranges become ONE quantity whose priority then
   beats the record pointer's: `q` lands in $a1 and the read-back in $a0,
   exactly swapped, 9 differing. THE STEP IS A TERNARY WITH A SHARED TAIL:
   written as two complete store arms the `>> 2`, the add and the store are
   not cross-jumped and the function is 70 words. And the two table halfwords
   are read into u16 LOCALS up front -- read inline, combine folds the `lhu`
   and the `sll 16`/`sra 16` into one `lh`. 66 words, 0 differing. */
void func_800DC724(u8 *p)
{
    u8 *q;
    s32 i;
    u16 tx;
    u16 tz;
    s32 d;
    s32 k;
    s32 cx;
    s32 cz;

    i = *(s32 *)(p + 0x50);
    tx = D_800E2924[i][0];
    tz = D_800E2924[i][1];
    q = *(u8 **)(*(u8 **)(p + 0x3C) + 0x4);
    *(s32 *)(p + 0x58) = 0;
    d = (s16)tx - *(s16 *)(q + 0x8);
    if (d != 0)
    {
        *(s32 *)(p + 0x58) = 1;
        cx = *(s16 *)(q + 0x8);
        k = (d > 0) ? (d + 3) : (d - 3);
        *(s16 *)(q + 0x8) = cx + (k >> 2);
    }
    d = (s16)tz - *(s16 *)(q + 0xA);
    if (d != 0)
    {
        *(s32 *)(p + 0x58) = 1;
        cz = *(s16 *)(q + 0xA);
        k = (d > 0) ? (d + 3) : (d - 3);
        *(s16 *)(q + 0xA) = cz + (k >> 2);
    }
    func_800DC650(p);
    if (*(s32 *)(p + 0x58) == 0 && *(s32 *)(p + 0x50) != *(s32 *)(p + 0x54))
    {
        if (((*(s32 *)(p + 0x54) ^ *(s32 *)(p + 0x50)) & 2) != 0)
        {
            func_80053DA8(0x507);
        }
        *(s32 *)(p + 0x50) = *(s32 *)(p + 0x54);
    }
}

/* The three flag states of func_800DBF38's object. ONE FUNCTION-SCOPE LOCAL
   FOR THE VALUE IS WHAT COSTS IT: written `v = p[0x54]; ...; p[0x54] = v;`
   with one `v` reused in two arms, the value chain lands in $v1 and the two
   mask constants in $v0 -- which makes the second constant anti-dependent on
   the arm's own `ori`, so sched2 cannot lift it above the `ori`, both arms
   end `[li -3][and]` and jump2 cross-jumps two insns instead of one: 22 words
   against 23, 13 differing. Updating the field in place keeps each arm's
   chain local to its block. Separate per-arm locals also reach 0.
   23 words, 0 differing. */
void func_800DC82C(u8 *p)
{
    s32 t;

    *(s32 *)(p + 0x5C) = 0;
    t = **(s32 **)(p + 0x38);
    if (t == 0)
    {
        *(s32 *)(p + 0x54) |= 3;
    }
    else if (t == 1)
    {
        *(s32 *)(p + 0x54) |= 1;
        *(s32 *)(p + 0x54) &= ~2;
    }
    else
    {
        *(s32 *)(p + 0x5C) = 1;
        *(s32 *)(p + 0x54) &= ~1;
        *(s32 *)(p + 0x54) &= ~2;
    }
}

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

/* Wire the nine 16-byte records at p[0] to the arrays at p[1], p[2] and
   p[3]. THE DESTINATION IS A STRUCT ARRAY, NOT POINTER ARITHMETIC: its `addu`
   is INDEX-FIRST, and C pointer arithmetic can only emit base-first. The
   source operands stay pointer arithmetic and come out base-first, which is
   what the target has. The second loop initialises `i` BEFORE `j`, in the
   for-init; with `j = 0;` on its own line ahead of the loop the two inits
   swap, 2 differing. 72 words, 0 differing. */
void func_800DCAC4(u8 *p, s32 b)
{
    s32 i;
    s32 j;

    for (i = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_04 = (u32)(*(u8 **)(p + 0x4) + i * 0x10);
    }
    for (i = 0, j = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_08 = (u32)(*(u8 **)(p + 0x8) + j);
        j += 0xC;
    }
    for (i = 0; i < 9; i++)
    {
        ((UnkDungeonDCAC4 *)*(u8 **)p)[i].unk_00 = ((u32 *)*(u8 **)(p + 0xC))[i];
    }
    *(s16 *)(*(u8 **)(p + 0x8) + 0x12) = 0x5F;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x1E) = 0x61;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x44) = -0xF;
    *(s16 *)(*(u8 **)(p + 0x4) + 0x88) = 0x9A;
    *(s16 *)(*(u8 **)(p + 0x4) + 0x8A) = 0x69;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x68) = -0xF;
    *(s16 *)(*(u8 **)(p + 0x8) + 0x60) = 0x800;
    if (b != 0)
    {
        func_800DC628(p, 0x400);
    }
    else
    {
        func_800DC628(p, 0);
    }
}

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

/* Allocate the tag-builder node and wire its handler.

   THREE PLACEMENT FACTS. (1) `p = (u8 *)n + 0x20` must sit ABOVE the
   `if (b != 0)`: inside the arm it is a once-set pseudo with one use,
   update_equiv_regs sinks its `addiu` to the store and $s2 never enters the
   frame -- 50 words against 53. (2) 0x800133A0 is a CONSTANT ADDRESS, stored
   as a value at +0x58 and dereferenced for the call's second argument; the
   `lui`/`ori` pair rather than `lui`/`addiu` is what says so. (3) The load
   has to be read into a local BEFORE the store, or the store is emitted ahead
   of the `lw` instead of into the `jal`'s delay slot (3 differing).
   53 words, 0 differing. */
UnkNode *func_800DCD38(void)
{
    UnkNode *n;
    u8 *p;
    u8 *q;
    u32 b;
    s32 c;

    n = func_8003FC64(0);
    if (n != NULL)
    {
        D_800E5910 = (u8 *)n;
        p = (u8 *)n + 0x20;
        b = func_8004B404(0x100);
        if (b != 0)
        {
            q = (u8 *)n + 0x5C;
            func_800DCCF4(q, (void *)b);
            c = *(s32 *)0x800133A0;
            *(u8 **)(p + 0x38) = (u8 *)0x800133A0;
            n->unk_0C = func_800DCBE4(q, c);
            func_800DC988(n);
            n->unk_10 = (u32)func_800DC8F8;
            func_8004491C(n, (u32)D_8004CAA0);
            return n;
        }
        n->unk_1E |= 0x8000;
        D_800814A0 |= 0x8000;
        n = NULL;
    }
    return n;
}

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
