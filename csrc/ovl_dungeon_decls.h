#ifndef OVL_DUNGEON_DECLS_H
#define OVL_DUNGEON_DECLS_H

/* The dungeon overlay's shared declaration block, lifted out of
   csrc/ovl_dungeon.c on 2026-11-20 so that the unit can be CARVED without
   duplicating 2600 lines of it.
 *
 * WHY THE CARVE: seven of this block's 28 jump tables sit at a 4-mod-8 rodata
 * offset, and cc1psx writes `.align 3' before every table it compiles -- so
 * landing one of their functions inside a single translation unit pads the
 * table and the container comes out eight bytes long. The fix is the one
 * town.bin took: give those functions a unit whose rodata STARTS at a 4-mod-8
 * offset, where the directive is a no-op. See splat/dungeon.bin.yaml and
 * docs/dungeon-sweep.md, "SEVEN of the 28 jump tables are at 4 mod 8".
 *
 * This file is TEXT MOVED VERBATIM. Nothing here was edited in the move, so
 * every one of the unit's landed functions compiles from the same
 * declarations it did before -- which is the only reason the move is safe to
 * make in one step.
 */

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

/* The map header at `&D_80083160 + 0x1DC': the cell array's base pointer and,
   at +0x14, the row shift.  WHY IT IS A STRUCT AND NOT A `u8 *': cse.c's
   invalidate_memory drops an entry only on `all || (nonscalar &&
   p->in_struct) || cse_rtx_addr_varies_p (p->exp)'.  Read through a `u8 *'
   the row shift's cse entry does not survive the halfword store next to it
   and gets reloaded; read through this struct it does.  Worth 13 words on
   func_8009A3D0 and the same lever landed func_8008DBE8. */
typedef struct UnkDungeonMapHdr
{
    u8 *unk_00;
    u8 pad_04[0x10];
    s16 unk_14;
} UnkDungeonMapHdr;

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
    u8 pad_9C[0x2C];
    /* func_80090C10's case 2 clears this through the struct face, and that is
       load-bearing: through `*(s32 *)(p + 0xC8)` the D_800E296C
       read-modify-write beside it schedules two words differently. */
    void *unk_C8;
} UnkDungeonActor;

/* The 4-halfword rectangle MoveImage takes. The engine function has no
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
/* Called for effect only; both forwarders discard the 0 it returns. */
s32 func_800AB538();
s32 func_800AB778();
/* Widened to `s32`: every exit sets $v0, which is a `return 0`/`return 1`
   and not a `void` tail. Its one landed caller, func_800D9F7C, discards the
   result and is unchanged at 8 words, 0 differing -- the same correction
   func_800AB538 one line up records. The definition stays K&R for the same
   reason: func_800D9F7C sits BELOW it and passes no arguments. */
s32 func_800AD058();
/* The second gate of the four-argument installers, whose result is tested as
   a HALFWORD -- `sll $v0, 16` then `blez`, with no `sra`. Two arguments. */
s32 func_800AD9B4();
/* The handlers these installers park in the +0x8C slot. An address-take needs
   a real declaration above it. */
void func_800B2A60();
void func_800D8C64();
/* Defined below; func_800D8590 installs its address at the node's +0x10. */
void func_800D8728();
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
s32 func_800C8C1C(u8 *, s16, s16);
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
extern UnkDungeonRect D_80089490;
extern UnkDungeonRect D_80089498;
/* The engine's node allocator and its install call. func_8004491C is
   declared `s32 func_8004491C(UnkNode *, u32)` in include/structs.h, so the
   handler goes in as a cast. */
UnkNode *func_8003FC64(s32);
/* Parked at the node's +0x10 by the pair below; an address-take needs a
   declaration. */
s32 func_800A7CC4(u8 *, UnkVec *, UnkSlot *);
s32 func_800B51C8();

/* Engine and overlay globals this unit reaches. D_800E3D7C is the resident
   engine's, and its `u8 *` spelling is copied from csrc/unk_154BC.c, which
   walks the same +0xAC field at the same stride. */
extern u8 *D_800E3D7C;
/* func_800BA810's cached-route state and the name record it indexes. */
extern u8 *D_800DF374;
extern u16 D_800DF378;
extern u16 D_800DF37A;
extern u8 D_800DF380[];
typedef struct UnkDungeonName1
{
    u8 pad_00[0x12];
    u8 unk_12;
    u8 pad_13[0x1];
} UnkDungeonName1;
extern UnkDungeonName1 D_8006DE24_n[] __asm__("D_8006DE24");
extern u8 D_800E3548[];
extern u8 D_800E3648[];
/* func_80095DFC's three message strings. */
extern u8 D_800E0B39[];
extern u8 D_80088B14[];
extern u8 D_800E0B4C[];
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
    s16 unk_10;
    /* s16, not u16: func_800B500C reads it with `lh` into a subtraction.
       The two `+=` sites truncate on store, so the width is invisible
       there -- both re-measure at 0 differing. */
    s16 unk_12;
    u16 unk_14; /* flags; 0x40 gates func_800B627C's re-dress */
    u8 pad_16[0x2];
} UnkDungeonSpotB;

typedef struct UnkDungeonSlot
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
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
void func_800CEFB8(u8 *, UnkVec *, UnkSlot *, s16, s16);

/* The 0x800D1C70..0x800D4634 state machines. D_8006CCD8/E8 are the engine's
   eight-entry sine and cosine tables, indexed by the facing field's top
   three bits exactly as csrc/ovl_town.c indexes them. */
extern s16 D_8006CCD8[];
extern s16 D_8006CCE8[];
extern s16 D_8006CD00[];
extern s16 D_8006CD02;
/* func_800A7234's two eight-way offset tables, and its own forward
   declaration: the definition is below func_8008F6EC and func_800A7A7C,
   which were calling it IMPLICITLY. */
extern s16 D_800DCE6C[];
extern s16 D_800DCE8C[];
s32 func_800A7234(s16, s16, s16, s16 *, s16 *, s16 *);
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
/* func_800B14FC's per-frame probe table, indexed by the record's +0x24.
   The result is tested, so it is not void. */
typedef s32 (*UnkDungeonProbe)(u8 *, UnkVec *, UnkSlot *);
extern UnkDungeonProbe D_800DF030[];
/* The engine's global flag halfword at D_80083460 + 2. csrc/unk_154BC.c
   reaches the same object as `*(u16 *)((u8 *)&D_80083460 + 2)`; here it is a
   `%lo` displacement off its own `lui`, which is what a declared u16 gives. */
extern u16 D_80083462;
/* Two arguments at its definition in csrc/unk_1AF38.c, one at every call in
   this unit -- csrc/unk_25944.c already declares it short the same way. */
void func_800478B8(u8 *);
/* func_800BB55C's five-argument renderer: the first two arguments are the two
   endpoint records (NULL on the whole-frame calls), the third the `s32 v[2]`
   VRAM pair, and the fifth the actor (NULL except on the opening call). */
void func_800BB2E4(void *, void *, s32 *, s32, void *);
/* Returns the distance between two points as seen from a facing; the third
   argument is sign-extended at every call site and the result is used both
   raw and as a halfword, so the return is s32 and the casts are explicit. */
s32 func_800BCB04(s32, s32, s16);
/* func_800BCB04 under a HALFWORD return type, for the one call site whose
   result goes into an `s16` local.  With the `s32` prototype the call
   result's mode does not match the variable's, so expand_call takes
   calls.c:2141 and copies $v0 into a fresh SImode pseudo; cse then
   forwards that pseudo into the compare and the store and the `s16`
   local dies, and the pseudo -- crossing no call -- is handed $v1.  With
   the halfword return the modes agree, calls.c:2057 moves $v0 straight
   into the local, and the local is callee-saved because it crosses the
   loop's calls.  That is the three words in func_800CA444. */
s16 func_800BCB04h(s32, s32, s16) __asm__("func_800BCB04");
s32 func_8009B164s(s16, UnkVec *, u8 *) __asm__("func_8009B164");
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
u8 *func_8009C93C(u8 *a, u8 *b, s32 ang, u16 dist, u8 *e);
extern u8 D_800245A8[];
extern void (*D_800DD830[])(void);
extern u8 D_80050CAC[];
void func_8008B9FC(u8 *, void *, u8 *, u8 *);
void func_8008C468(u8 *, void *, u8 *, u8 *);
void func_8008C514(u8 *, void *, u8 *, u8 *);
s32 func_8008C5C4(u8 *, UnkVec *, u8 *, u8 *);
void func_8008C7B4(u8 *, void *, u8 *, u8 *);
s16 func_8008C8BC(u8 *, UnkVec *, u8 *, u8 *);
void func_8008CAA0(u8 *, void *, u8 *, u8 *);
void func_8008CBA0(void *, void *, void *, u8 *);
void func_8008CCB0(u8 *, void *, u8 *, u8 *);
void func_8008CF6C(UnkDungeonActor *, void *, u8 *, u8 *);
s32 func_8008D024(u8 *, void *, u8 *, s16, s16);
s32 func_8008D1D0(u8 *, void *, u8 *, u8 *);
void func_8008D7D0(u8 *, void *, u8 *, u8 *);
void func_8008D94C(u8 *, void *, u8 *, u8 *);
void func_80090200(u8 *, void *, u8 *, u8 *);
s32 func_8009074C(s16, u16 *, u16 *);
void func_80091B5C(u8 *, void *, u8 *, u8 *);
s32 func_80094270(u8 *, void *, u8 *, u8 *, s16);
void func_80094548(s16, s32);
void func_8009456C(s16, u8);
void func_8009458C(s16, u8);
void func_800945C4(s16, s32);
s32 func_80094EA4(void);
s16 func_80095538(u8 *, s16, s16);
void func_800956B8(u8 *, void *, void *, u8 *);
s32 func_80095854(UnkDungeonActor *, void *, u8 *, u8 *);
UnkNode *func_80098CF8(u8 *, void *, u8 *, u8 *);
u8 *func_8009F868(void);
u8 *func_8009F988(void);
void func_8009FAAC(void);
void *func_8009FADC(s16);
s16 func_800A1C58(u8 *);
s32 func_800A2C34(u8 *);
void func_800A4300(u8 *, u8 *);
s32 func_800A4474(s16, s16);
/* cfc2 $12, $31 + nop + sw: the GTE flag register read func_800CF8E4 uses to
   reject a triangle whose RTPT overflowed.  Spelled like gte_stszotz above it
   in include/gte_inline.h -- hard $12, `: "memory"` -- because the listing's
   scratchpad store must not sink past it. */
#define gte_stflg(r0) __asm__ volatile("cfc2 $12, $31\n\tnop\n\tsw $12, 0(%0)" ::"r"(r0) : "$12", "memory")
/* func_800CF8E4's map-cell render pass.  Same shape as csrc/ovl_town.c's
   func_800AC4C4 (0.602 by opcode stream) but around a 6-byte map cell rather
   than a bare u16, so the types are its own rather than UnkTownAC*. */
typedef struct UnkDungeonCF8E4Cell
{
    u16 unk_00;
    s16 unk_02;
    u16 unk_04;
} UnkDungeonCF8E4Cell;
typedef struct UnkDungeonCF8E4Vtx
{
    s32 unk_00;
    u16 unk_04;
    u16 unk_06;
} UnkDungeonCF8E4Vtx;
typedef struct UnkDungeonCF8E4Nrm
{
    s32 unk_00;
    s16 unk_04;
    s16 unk_06;
} UnkDungeonCF8E4Nrm;
typedef struct UnkDungeonCF8E4Obj
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
    u32 unk_0C;
    s32 unk_10;
    s32 unk_14;
} UnkDungeonCF8E4Obj;
typedef struct UnkDungeonCF8E4Map
{
    UnkDungeonCF8E4Cell *unk_00;
    UnkDungeonCF8E4Obj **unk_04;
    UnkDungeonCF8E4Vtx *unk_08;
    UnkDungeonCF8E4Nrm *unk_0C;
    u8 pad_10[0x4];
    s16 unk_14;
    s16 unk_16;
    s16 unk_18;
    s16 unk_1A;
} UnkDungeonCF8E4Map;
/* D_80083160 + 0x18 as func_800CF8E4 reads it: the camera record, with the
   eye/target halfword triples at +0x08..+0x2C that the D_800DCF58 arm snaps
   and the other arm halves towards. */
typedef struct UnkDungeonCF8E4View
{
    u8 pad_00[0x8];
    s16 unk_08;
    s16 unk_0A;
    s16 unk_0C;
    u8 pad_0E[0x2];
    s16 unk_10;
    s16 unk_12;
    s16 unk_14;
    u8 pad_16[0x2];
    s16 unk_18;
    s16 unk_1A;
    s16 unk_1C;
    u8 pad_1E[0x2];
    s16 unk_20;
    s16 unk_22;
    s16 unk_24;
    u8 pad_26[0x2];
    s16 unk_28;
    s16 unk_2A;
    s16 unk_2C;
    u8 pad_2E[0x56];
    s32 unk_84;
    s32 unk_88;
    u8 pad_8C[0x4];
    u32 unk_90;
    u8 pad_94[0x10];
    u16 unk_A4;
    u16 unk_A6;
    u16 unk_A8;
} UnkDungeonCF8E4View;
/* The 0x1F800000 scratchpad as func_800CF8E4 lays it out: a word at
   0xE0/0xE8/0xF0/0xF8 and a halfword at +4 of each, which SVECTOR cannot
   spell.  0x10, 0x164, 0x16C and 0x170 are each stored as ONE WORD and read
   back as halfwords or bytes, so they stay s32 here and the half reads are
   casts off `sp` -- csrc/ovl_town.c's func_800AC4C4 spelling.  Declaring them
   SPLIT instead (a u16 pair with the word store through an (s32 *) cast) is
   measurably worse: 0x164 split is 51 aligned rows against 4, and 0x16C with
   0x170 split together is 125. */
typedef struct UnkDungeonCF8E4Vert
{
    u32 unk_00;
    u16 unk_04;
    u16 unk_06;
} UnkDungeonCF8E4Vert;
typedef struct UnkDungeonCF8E4SP
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    s32 unk_0C;
    s32 unk_10;
    s32 unk_14;
    s32 unk_18;
    Unk46C20B unk_1C[4];
    u32 *unk_BC;
    u8 pad_C0[0x8];
    s32 unk_C8;
    u8 pad_CC[0x8];
    s32 unk_D4;
    s32 unk_D8;
    s32 unk_DC;
    UnkDungeonCF8E4Vert unk_E0;
    UnkDungeonCF8E4Vert unk_E8;
    UnkDungeonCF8E4Vert unk_F0;
    UnkDungeonCF8E4Vert unk_F8;
    u8 pad_100[0x10];
    u32 unk_110;
    s32 unk_114;
    s32 unk_118;
    s32 unk_11C;
    s32 unk_120;
    s32 unk_124;
    s32 unk_128;
    s32 unk_12C;
    u8 pad_130[0x4];
    s32 unk_134;
    s32 unk_138;
    s32 unk_13C;
    u8 pad_140[0x4];
    s32 unk_144;
    s32 unk_148;
    s32 unk_14C;
    u8 pad_150[0x8];
    s32 unk_158;
    u8 pad_15C[0x8];
    s32 unk_164;
    u8 pad_168[0x4];
    s32 unk_16C;
    s32 unk_170;
    u16 unk_174;
    u16 unk_176;
    u16 unk_178;
    u8 pad_17A[0x2];
    u32 unk_17C;
} UnkDungeonCF8E4SP;
/* Resident libgte, unnamed in splat/common_syms.txt but in the matrix block
   beside SetColorMatrix/SetLightMatrix; copied from csrc/ovl_town.c. */
/* Defined below; func_800894A0 above calls it. */
/* The dungeon's map-cell render pass: walk the four view-span edges across the
   cell grid, and for every object in a visible cell transform its quad, light
   it from the cell's normal and link it into the OT.  932 words, 0 differing.
   IT IS csrc/ovl_town.c's func_800AC4C4 REBUILT AROUND A 6-BYTE MAP CELL, and
   that is the whole reason this landed: the object loop, the four edge loops,
   the vertex block, the LINE_F4 block and the backface tail are town's shape
   statement for statement, and town's own scheduler levers transfer verbatim.
   `xsibs.py` scores the pair at only 0.602 and badly undersells it.  What
   differs from town: the cell is a 6-byte record so every cell reference is
   `*6` instead of `*2`; the camera record at +0x18 is snapped or halved
   towards its target by the D_800DCF58 arm at the head, which town has no
   counterpart for; func_800BCB04 supplies a floor height into sp->unk_158 that
   the LINE_F4 gate tests, and sp->unk_148 is a per-cell light threshold; town's
   sp->unk_168/0x16A are this function's 0x164/0x166 and town's sp->unk_164
   (the prim limit) is this function's 0x17C; and the quad floor prim town draws
   for a 0xC000 cell is absent.
   THE FOUR SPELLINGS THAT MATTER, in the order they were worth the most.  Each
   number is aligned rows from `aligndiff.py` at the point it was measured.
   (1) `h1 = (u16)sp->unk_14;` READ INTO A LOCAL BEFORE THE SECOND VERTEX'S +4
   STATEMENT -- town's own last lever, and here it is the LAST FOUR ROWS: 4 to
   0.  Both orders are legal; SCHED_SORT picks within the priority group by
   INSN_LUID, and hoisting that one read swaps two LUIDs and nothing else.
   EVERY OTHER read of `(u16)sp->unk_14` must stay its own expression.
   (2) THE THREE VERTEX INDEX LOCALS AND WHERE THEY SIT, worth 44 rows to 6.
   `vi0`/`vi1`/`vi2` each feed only the `sp->unk_164 = vt[vi].unk_00` store,
   while the `.unk_04` read beside it goes through `o->unk_0X` again -- the
   target really does load each index twice, because the `sp->unk_164` store in
   between is at a constant address and `o`'s is not, so cse cannot fold them.
   The PLACEMENT is the lever, not the locals: `vi1` must sit between the first
   vertex's +4 and +0 statements and `vi2` between the second's, and the head
   must be `unk_170, vi0, unk_16C, unk_164` in that order.  Measured: all six
   permutations of the head (17/24/16/6/20/23 rows), all four placements of
   `vi1` (33/6/28/28) and all twelve orderings of the second vertex's group
   (6 through 59).  `vi0` before `unk_170` is 17; all three indices hoisted to
   the top of the loop is 119.
   (3) THE unk_148 STORE WRITTEN AT EVERY EARLY-OUT, NOT BEHIND ONE LABEL --
   13 rows, and it is an ALLOCNO PRIORITY, not a scheduling fact.  Read off
   `tools/decomp/dump.sh`: pseudo 96 is `h`, 804 is addPrim's 0xFF000000, and
   `allocno_compare` ranks by floor_log2(refs)*refs/live_length while `flow`
   adds loop_depth per reference.  With one `sp->unk_148 = h;` behind a
   `setBig:` label, 96 has 8 weighted refs against 804's 17, loses the sort,
   and the two swap registers -- which is exactly the "register swap of two
   hoisted constants" this function was parked on for a day.  Writing the store
   at FIVE of the six early-outs as `sp->unk_148 = h; goto doLight;` takes 96
   to 28 weighted refs and puts it ahead, and jump2's cross-jumping -- which
   runs AFTER greg -- merges them back into the single `sw $t3, 0x148($s0)` the
   target has, for no words.  The SIXTH, the `cf & 0x40` test, must stay
   `goto setBig`, because there the target branches FORWARD to the store and
   falls through to the negate; writing all six is 73 rows against 62.
   (4) THE EARLY-OUTS GO TO `cellDone`, NOT TO `allDone` -- one word and 17
   rows.  Every `bltz`/`beqz`/`beq` in the object loop that reads as "give up"
   targets the per-cell `sp->unk_17C < (u32)p` test; only the `while` headers
   reach the function tail.  Sixteen of those 17 rows were branch
   DISPLACEMENTS, which read as a length artefact rather than as a wrong
   destination, so this one is worth checking first in any sibling.
   And five smaller ones, each measured here:
   * `one = 1;` then `h = 0x7FFF;` as the FIRST TWO statements of the outer
     loop body.  scan_loop hoists a user variable only through arm 1
     (`! maybe_never && ! loop_reg_used_before_p`), so the same statement four
     lines down is not hoisted at all, and the preheader emits hoisted movables
     in the order their set insns appear -- which is what gives the target's
     `1, 0x7FFF` rather than `0x7FFF, 1`.
   * `addPrim(...); p++;` DUPLICATED into both arms of the 0x1BE gate, so
     jump.c cannot merge the two `p->code |= 2` tails into the join, and the
     gate written `if ((u32)x < 0x1BE) {...} else {...}` and never as `>=`.
   * `v = (UnkDungeonCF8E4View *)((u8 *)g + 0x18);` must be assigned BEFORE the
     `D_80013714 & 2` gate, and the six pointer locals in the order
     `g, v, m, cell, vt, nm`.  After the gate, `lui $s0, 0x1F80` falls into the
     second basic block; in any other order the prologue's `sw $s1, 0x44($sp)`
     comes out two slots late (6 rows against 4).
   * the D_800DCF58 arm's NINE SELF-ASSIGNMENTS (`v->unk_10 = v->unk_10;` and
     eight more) are really in the target -- `lhu`/`sh`, same base, same
     displacement -- and its first three stores must be CHAINED
     (`v->unk_28 = v->unk_08 = buf[0].vx;`).  Two statements cost two
     `addu rd, rs, $zero` copies and pushed the twelve halfword loads up one
     register each.  The other arm's tail is 0x28, 0x2A, 0x2C in that order:
     the LAST of the three is the one sched1 hoists.
   * the inner `while (unk_10 >= 0)` body is `unk_00 +=; unk_10 -=; unk_20 -=;`
     and then tests unk_20, so the `blez` reads unk_20's new value out of a
     register; with `unk_20 -=` before `unk_10 -=` the test reloads 0x3C.
   Spellings that are byte-identical, so pick on taste: `(u16)sp->unk_12C` as
   `*(u16 *)((u8 *)sp + 0x12C)` at all four sites, or sp->unk_12C declared as a
   `u16`/`u16` pair with the word store cast.  Spellings that are NOT: the two
   `|` operands swapped (89 rows), 0x164 split into a `u16` pair (51), 0x16C or
   0x170 split (125 together), and a `: "memory"` clobber on gte_ldv3 (no
   change at 4 rows, 51 rows earlier -- it is not a lever here).
   SVECTOR buf[6] is 0x30 bytes of locals, which is what puts the register
   saves at 0x40 and makes the frame 0x60; buf[5] gives a 0x58 frame. */
void func_80064624(s32, s32);
void func_800CF8E4(void);
extern u8 D_800DCF5C;
extern s16 D_800DCF5E;
extern u8 D_800E0C34[];
extern u8 D_800DD260;
extern s16 D_800DD262;
extern u8 D_8008149C[];
extern u8 D_800E06FA[];
extern u8 D_800E0714[];
extern s32 D_800E3D74[];
/* The same word as D_80082EB0, read as a STRUCT MEMBER. */
typedef struct UnkDungeon82EB0
{
    u8 *unk_00;
} UnkDungeon82EB0;
extern UnkDungeon82EB0 D_80082EB0_m __asm__("D_80082EB0");
u8 *func_80048118(s16, u8 *);
void func_80047FF4(s16, u32);
/* func_800A31D0's view of a list record: the +0x1C request word, the +0x6D
   byte and the +0x6E halfword it clears. */
typedef struct UnkDungeonA31D0
{
    u8 pad_00[0x1C];
    s32 unk_1C;
    u8 pad_20[0x4D];
    u8 unk_6D;
    s16 unk_6E;
} UnkDungeonA31D0;
void func_800A31D0(u8 *);
void func_800B8FC8(u8 *, s32 *, s32 *, s32, s32);
extern u8 D_800E0C58[];
extern u8 D_800E0C67[];
/* The +0x9A state table func_800C9AAC dispatches through, same element type
   as D_800E22E0's. */
extern UnkDungeonStep D_800E0354[];
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
s32 func_800A2424(u8 *, s16);
/* Defined below func_8009DAD8, which calls it; without this prototype the
   call rides an implicit `int f()' and loses the `sll 16'/`sra 16' pair in
   front of its `slti' -- two words. */
s16 func_8009FD40(u8 *, u8 *);
/* func_8008E700's three animation tables. */
extern u8 D_800DD048[];
extern u8 D_800DD060[];
extern u8 D_800DCFD0[];
s32 func_800647A0(s32);
extern u8 D_800E0953[];
extern u8 D_800E095F[];
extern u8 D_800E0970[];
extern u8 D_800E0979[];
extern u8 D_800E098B[];
extern u8 D_800E099A[];
extern u8 D_800E09B2[];
extern u8 D_800E09BB[];
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
extern u8 D_800E1095[];
extern u8 D_800E10B3[];
extern u8 D_80089360[];
/* The second kill skeleton's callees and strings. func_8008D344 is in this
   unit and is reached with four arguments, exactly like func_8008D330. */
void func_8008D344(u8 *, void *, void *, void *);
s32 func_80098864(u8 *, s32);
void func_800A6480(u8 *, u8 *);
void func_8009D8A4(void);
extern u32 D_800E296C;
extern u32 D_800DCF80[];
extern u32 D_800DCFA4[];
extern u8 D_800E11FD[];
extern u8 D_800E1224[];
extern u8 D_800E1258[];
extern u8 D_800E1279[];
extern u8 D_800E12A9[];
/* The two kill handlers whose out-of-range arm spawns through func_800BBA40
   (seven arguments, three of them on the stack) instead of reporting. */
s32 func_800BBA40(s16, s16, s16, void *, s16, s32, void *);
/* Not void: it answers 1 or 0. Only its ADDRESS is taken -- it is
   func_800C13C8's seventh argument to func_800BBA40 -- which is why nothing
   had falsified it. */
s32 func_800C135C();
/* Not void: the loop's back branch carries `addiu $v0, $zero, 1` in its delay
   slot. Only its ADDRESS is taken, which is why nothing had falsified it. */
s32 func_800C152C();
extern u8 D_800DEAE0[];
extern u8 D_800DEC00[];
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
/* `u8 *` on both the return and the cursor, not `void *`: the body advances
   the output with `*out++ = *q++;`, and a `void *` parameter forces a copy
   pseudo that lets combine collapse the whole chain into displacements. Both
   existing users re-measure at 0 differing. */
u8 *func_80099368(u8 *, u8 *);
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
/* func_800B7774's three message strings. */
extern u8 D_800E0A42[];
extern u8 D_800E0A56[];
extern u8 D_800E0A6A[];
extern u8 D_800E3D20;
extern u8 D_800DF3DC[];
extern u8 D_800DD000[];
extern u8 D_800E1B5D[];
void func_800CBDB4();
void func_800419EC(s32, s32);
s16 func_8009B88C(u8 *, s32, s32, s16 *, s16 *);
extern u8 D_800DCFF0[];
/* func_80092E90's state-0 skin table and its two outcome messages. */
extern u8 D_800DCFF8[];
extern u8 D_800E0597[];
extern u8 D_800E05C3[];
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
void func_800B84E4(s16 *, u8 *, u8 *, s32);
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
/* The same 0x14-byte D_8006DE24 record again, under the face func_800A384C
   needs: it reads +0x10, +0x11, +0x12 and +0x13 off one record pointer. */
typedef struct UnkDungeonName10
{
    u8 pad_00[0x10];
    u8 unk_10;
    u8 unk_11;
    u8 unk_12;
    u8 unk_13;
} UnkDungeonName10;
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
s32 func_800C5BBC(s16, s16, s16, s32, u16, s16);
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
/* Returns the slot record it found -- the two `addu $v0, $s2, $zero` at its
   tail. All seven landed call sites drop the result and all seven stay at 0
   differing. */
u8 *func_800A32A4(u8 *);
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
   csrc/ovl_town_800BF3F0.c's. GetTPage returns u16 -- the call site
   masks with `andi $a3, $v0, 0xFFFF`. */
/* The nine GTE/matrix-stack routines func_800DBA90 drives. splat names them
   PushMatrix, PopMatrix, SetRotMatrix, SetTransMatrix, RotTrans, RotMatrix,
   RotMatrixX, RotMatrixY and RotMatrixZ.  Both spellings link since
   build/dungeon.bin started passing --script=splat/common_syms.txt on
   2026-11-15; the library names are the readable ones and are what this unit
   now uses. */

/* Prototype copied from its definition in csrc/unk_1C540.c:2936. The `s16`
   second parameter is load-bearing HERE: the CALLER is what carries the
   `sll`/`sra 16` pair the target has at each of func_800C55F4's three call
   sites. */
u8 *func_8004CECC(u32, s16, s32, u8 *);

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
/* The spark step func_800D7FB8 parks at each node's +0x10. It owns a 4-mod-8
   jump table, so it stays INCLUDE_ASM until the unit is carved; only its
   address is taken here. */
void func_800D81D4();
extern u32 D_80081488;
extern u8 D_8006CCF8[];
extern s16 D_800DD264[];
extern s16 D_800120A2;
extern u8 D_800DCFB8[];
extern u8 D_800DD018[];
/* func_80092018's other two eight-way facing tables. */
extern u8 D_800DD020[];
extern u8 D_800DD028[];
/* Defined below; func_80092018 calls it above its definition. */
void func_800B653C(u8 *, s16);
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
/* func_800A08A0's spawn table and its counter: the table is a POINTER
   global read into a local, the counter a halfword. */
extern s16 D_8008347C;
extern u8 *D_80083478;
extern u32 D_800835E4[];
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
void func_8008D470();
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
/* The one-byte counter func_800CCC20 bumps per re-armed slot and clears when
   it spawns the tint node. Read and written with `lbu`/`sb` through a `%lo`
   in the displacement, which is a declared SCALAR. */
extern u8 D_800E5908;
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
extern u8 *D_800DD720;
extern u8 D_800DD824[];
u8 *func_8004AC3C(u8 *, s32 *);
void func_800C2824();
/* THREE s16 PARAMETERS: declared s32 the three prologue parameter copies
   come out in parameter order where the target emits the third's pair
   first -- 20 differing, all in the first six words. */
u8 *func_800C9850(s16, s16, s16);
void func_8004A8D8(s32, s32);
void func_80035208(void *);
void func_80035348(void);
u8 *func_800A1618(s16, s16);
void func_800DC82C(u8 *);
s32 func_800AB1C0(u8 *, UnkVec *, u8 *, u8 *);
void func_800AD594(u8 *, s32);
/* The h7 batch's new declarations. */
/* The record func_8008F878 parks while the cut-scene runs; its +0x6D byte is
   cleared when the scene closes. */
extern u8 *D_800DD25C;
/* func_8008E0C4's own symbols. */
extern u8 D_800E0672[];
extern u8 D_800E3CD0;
extern u16 D_80082E76;
void func_800945E8(u8 *);
void func_800948BC(void);
void func_80041094(u16, s16, s16, s16, u16);
/* func_800956B8's strings and its second key lookup, which is landed below
   it. */
extern u8 D_800E0B18[];
extern u8 D_800E05F0[];
extern u8 D_800E05E1[];
extern u8 D_800E0B2B[];
extern u8 D_800E202D[];
s32 func_800A1C14(u8 *);
void func_800C96E8(u8 *, void *);
/* Defined below; the landed func_800C0180 calls it above its definition. */
UnkNode *func_800C542C(u8 *, s16, s16, s16);
/* The two messages func_800C8150 sends. */
extern u8 D_800E18F5[];
extern u8 D_800E195C[];
/* The renderable descriptor func_800C542C parks at its node's +0x0C record,
   and the handler/tag pair that node carries. */
extern u8 D_800DF630[];
void func_800C4F20();
/* The tag func_800C542C hands to func_8004491C; defined below, and an
   address-take needs a declaration above it. Copied from that definition. */
s32 func_800C55A0(u8 *, UnkVec *, UnkSlot *);
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
extern u8 D_800DD058[];
/* func_80091C64's other eight-way facing table, beside D_800DD030/D_800DD050
   above. */
extern u8 D_800DD038[];
/* func_800909A0's two skin tables and the +0x9A step table it dispatches
   through; the table has UnkDungeonStep's four-argument shape. */
extern u8 D_800DD008[];
extern u8 D_800DD010[];
extern UnkDungeonStep D_800DD168[];
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
    s16 unk_0E;
    u8 pad_10[0x4];
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
extern u8 *D_80082EB0;
s32 func_80098C80(u8 *);
void func_800C9F34();
/* The UnkNode +0x10 handler func_800C9850 installs; defined below. */
void func_800C9AAC();
/* The eight-entry facing table func_800C9850 parks at the view's +0xA4 and
   the slot's +0x2C, and the 11-byte record it copies into the node's +0x54.
   D_80089430 is this unit's own rodata. */
extern u32 D_800E0334[];
extern u32 D_80089430[];
void func_80093A94();
void func_800ACB98(u8 *, void *, u8 *, u8 *);
void func_800AA53C(u8 *);
/* func_8009955C's two string tables. */
extern u8 *D_800DD72C[];
extern u8 *D_800DD784[];
s32 func_80064584(s32);
/* func_800B84E4's per-primitive hand-off, beside AddPrim's. */
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
void func_800DAEF4();
extern u8 D_800DF358[];
extern u8 D_800E045F[];
extern u8 *D_800DD728;
void func_800B8830();
/* Defined below func_8009CE1C in this unit, which calls it. */
s32 func_8009D218(u8 *, s32, u8 *);
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
extern s16 D_800DCE68;
s32 func_80098920(u8 *, u8 *, s16, s32);
void func_800C75D0();
extern u8 D_800DF63C;
/* Returns 0 (`addu $v0, $zero, $zero` before the epilogue); only its address
   is taken, so the unprototyped form is the minimal change. */
s32 func_800DA93C();
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
extern u8 D_800E23D8[];
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
extern u8 D_800DCFD8[];
/* The 0x34-byte Gouraud-textured primitive func_800D3738 and func_800C6B40
   emit -- libgpu's POLY_GT4, with the three u/v pairs that are read back a
   byte at a time written through Unk453E0UV the way the resident
   func_800453E0's Unk453E0P does. */
typedef struct UnkDungeonGT4
{
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    u16 unk_0C;
    u16 unk_0E;
    u32 unk_10;
    u16 unk_14;
    u16 unk_16;
    Unk453E0UV unk_18;
    u16 unk_1A;
    u32 unk_1C;
    u16 unk_20;
    u16 unk_22;
    Unk453E0UV unk_24;
    u16 unk_26;
    u32 unk_28;
    u16 unk_2C;
    u16 unk_2E;
    Unk453E0UV unk_30;
    u16 unk_32;
} UnkDungeonGT4;
extern u8 D_800DCFB0[];
/* func_80090C10's own callees and rodata.  func_8004B568 takes NO argument at
   its one call site here ($a0 is never set before the `jal`), so it is
   declared K&R; other units declare it `void func_8004B568(u8 *)`. */
void func_8004B568();
void func_8002534C(u8 *, void *, u8 *, u8 *);
extern s8 D_800DCF4D;
extern u8 D_8001EF2C[];
extern u8 D_800DDCBC[][8];
extern u8 D_800E0462[];
extern u8 D_800E0472[];
extern u8 D_800E0542[];
extern u8 D_800E0571[];
extern u8 D_800E069D[];
extern u8 D_800E06BD[];
extern u8 D_800E06C0[];
extern u8 D_800E06E0[];
/* func_8009C12C's seven message strings. */
extern u8 D_800E0D7B[];
extern u8 D_800E0D92[];
extern u8 D_800E0D9E[];
extern u8 D_800E0DB8[];
extern u8 D_800E0DD0[];
extern u8 D_800E0DDD[];
extern u8 D_800E0DEA[];
/* func_8008B9FC's eight-way facing table, one more of the same class. */
extern u8 D_800DCFC0[];
/* The byte func_8008B9FC switches on at 0xF0/0xF8/0xE8/0xE0 and tests for
   8 or 0; an independent scalar rather than a field of D_800E3540, which
   this unit declares as a `void *`. */
extern u8 D_800E3544;
/* One more eight-entry direction table of the same class, func_8008F6EC's.
   An independent symbol rather than D_800DD030 + 0xA0. */
extern u8 D_800DD0D0[];
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
/* func_800B06F0's two projection callbacks, picked by the view's +0x8:
   func_800B0654 at index 0 and func_800B06C0 at index 1. */
typedef void (*UnkDungeonProj)(UnkVec *, UnkSlot *, void *);
extern UnkDungeonProj D_800DEFF0[];
/* Swaps the two packed screen pairs it is handed. */
void func_8003E12C(s32 *, s32 *);
/* func_800B06F0's two primitive emitters and the colour-row hand-off. */
void func_8004C010(u8 *, u32 *);
u8 *func_8004CD28(u8 *, u8 *, u8 **, u8 *);
u8 *func_800B0BE0(u8 *, u8 *, u8 **, u8 *);
/* A packed screen-XY word in the scratchpad, read back as two halfwords --
   the same word/halfword union Unk3DBD0W is for func_8003DBD0. */
typedef union UnkDungeonSPXY
{
    s32 w;
    u16 h[2];
} UnkDungeonSPXY;

/* A scratchpad word written whole and read back as its low BYTE and as the
   signed word -- the byte-wide face of the slot UnkDungeonSPW gives a
   halfword face to. */
typedef union UnkDungeonSPUV
{
    s32 w;
    u8 b;
} UnkDungeonSPUV;

/* The 0x1F800000 scratchpad as func_800B84E4 lays it out: the four UV
   scalars at +0x08..+0x17 and two packed screen pairs at +0x70 and +0x78,
   each the XY half of an 8-byte slot whose Z halfword is zeroed. */
typedef struct UnkDungeonSPF
{
    u8 pad_00[0x8];
    UnkDungeonSPUV unk_08;
    UnkDungeonSPUV unk_0C;
    UnkDungeonSPUV unk_10;
    UnkDungeonSPUV unk_14;
    u8 pad_18[0x58];
    UnkDungeonSPXY unk_70;
    s16 unk_74;
    u8 pad_76[0x2];
    UnkDungeonSPXY unk_78;
    s16 unk_7C;
} UnkDungeonSPF;

/* The same 0x34-byte POLY_GT4 as UnkDungeonGT4 above with its four screen
   pairs under their WORD face and ALL FOUR u/v pairs byte-addressed:
   func_800B84E4 copies each screen pair in with one `sw' and reads its own
   u0/v0 back a byte at a time. */
typedef struct UnkDungeonGT4W
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    Unk453E0UV unk_0C;
    u16 unk_0E;
    u32 unk_10;
    s32 unk_14;
    Unk453E0UV unk_18;
    u16 unk_1A;
    u32 unk_1C;
    s32 unk_20;
    Unk453E0UV unk_24;
    u16 unk_26;
    u32 unk_28;
    s32 unk_2C;
    Unk453E0UV unk_30;
    u16 unk_32;
} UnkDungeonGT4W;
/* The 0x1F800000 scratchpad as func_800B06F0 lays it out: the OT base at
   +0x20, ScaleMatrix's vector at +0x30, the quad's four corners at +0x70,
   RotTransPers4's two out-parameters at +0x90/+0x94, the projection's screen
   offset at +0xB8, the OT index (then row pointer) at +0xC0, the matrix's
   translation at +0xE4, the four projected corners at +0xF0 and RotMatrix's
   input at +0x100. */
typedef struct UnkDungeonSPC
{
    u8 pad_00[0x20];
    u32 unk_20;
    u8 pad_24[0xC];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x34];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    UnkDungeonSPXY unk_F0[4];
    SVECTOR unk_100;
} UnkDungeonSPC;

/* A named view of D_80083460 for func_8008DBE8: its +0x02 flag halfword,
   the +0x04 heading it divides twice, and the +0x0A counter.  The struct
   face is what sets MEM_IN_STRUCT_P, which lets the `*(s32 *)(b + 0xC)'
   store invalidate cse's entry for +0x04 so the second divide reloads it
   -- worth 4 differing against reading the bytes through a `u8 *'. */
typedef struct UnkDungeonGlobal
{
    u8 pad_00[0x2];
    u16 unk_02;
    s16 unk_04;
    u8 pad_06[0x4];
    u16 unk_0A;
} UnkDungeonGlobal;

/* The 0x1F800000 scratchpad as func_800BABA8 lays it out: the mover's three
   position halfwords at +0x00/+0x02/+0x04, the OT base at +0x20, the pair of
   RotTransPers input vectors at +0x70 and the OT index at +0xC0.  Its two
   out-parameters at +0x90/+0x94 are CONSTANT ADDRESSES in pointer locals,
   not members, which is what keeps them in callee-saved registers. */
typedef struct UnkDungeonSPA
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u8 pad_06[0x1A];
    u8 *unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[2];
    u8 pad_80[0x40];
    s32 unk_C0;
} UnkDungeonSPA;
/* The 0x24-byte Gouraud quad func_800BABA8 emits -- libgpu's POLY_G4, with
   the two screen pairs RotTransPers writes as words and read back as the
   halfword UnkDungeonSPXY gives a face to. */
typedef struct UnkDungeonG4
{
    u32 unk_00;
    u32 unk_04;
    s32 unk_08;
    u32 unk_0C;
    UnkDungeonSPXY unk_10;
    u8 unk_14;
    u8 unk_15;
    u8 unk_16;
    u8 pad_17[0x1];
    s32 unk_18;
    u8 unk_1C;
    u8 unk_1D;
    u8 unk_1E;
    u8 pad_1F[0x1];
    UnkDungeonSPXY unk_20;
} UnkDungeonG4;

/* RotTransPers-family: one scratchpad SVECTOR in, a packed screen pair and
   the flag out. Only the `func_' spelling is in
   splat/dungeon.bin_undefined_funcs_auto.txt, so that is what links. */
/* The 0xE6 mask primitive func_800B9144 brackets its run with, the spelling
   csrc/ovl_town.c:2511 uses. */
void func_80067EF4(void *p, s32 a, s32 b);
/* A scratchpad word that is written whole and read back as its low halfword;
   the mode difference is what makes the read a real `lhu', exactly as
   Unk3DBD0W does for func_8003DBD0. */
typedef union UnkDungeonSPW
{
    s32 w;
    u16 h;
} UnkDungeonSPW;
/* The 0x1F800000 scratchpad as func_800B9144 lays it out: the view's two
   origin halfwords at +0x00/+0x02, the four per-element UV scalars at
   +0x08..+0x17, the OT base at +0x20 and a copy of the slot's +0x14 flag
   word at +0x24, ScaleMatrix's vector at +0x30, TransMatrix's at +0x40,
   the composed matrix at +0x50, the quad's four corners at +0x70,
   RotTransPers' flag out-parameter at +0x94 and its four packed screen
   pairs at +0x98/+0xA0/+0xA8/+0xB0, the mask-primitive cursor at +0xC0 and
   the slot's own origin at +0x108/+0x10A. */
typedef struct UnkDungeonSPD
{
    s16 unk_00;
    s16 unk_02;
    u8 pad_04[0x4];
    UnkDungeonSPW unk_08;
    UnkDungeonSPW unk_0C;
    UnkDungeonSPW unk_10;
    UnkDungeonSPW unk_14;
    u8 pad_18[0x8];
    u32 unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x4];
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    u8 pad_4C[0x24];
    SVECTOR unk_70[4];
    u8 pad_90[0x4];
    s32 unk_94;
    s32 unk_98;
    u8 pad_9C[0x4];
    s32 unk_A0;
    u8 pad_A4[0x4];
    s32 unk_A8;
    u8 pad_AC[0x4];
    s32 unk_B0;
    u8 pad_B4[0xC];
    s32 unk_C0;
    u8 pad_C4[0x44];
    u16 unk_108;
    u16 unk_10A;
} UnkDungeonSPD;

/* The 0x1F800000 scratchpad as func_800BC4D4 lays it out: the mover's two
   origin halfwords at +0x00/+0x02 and the caller's depth at +0x04, the four
   per-element UV scalars at +0x08..+0x17, the OT base at +0x20 and a copy of
   the slot's +0x14 flag word at +0x24, ScaleMatrix's vector at +0x30, the
   quad's four corners at +0x70, RotTransPers4's two out-parameters at
   +0x90/+0x94, the projection's screen offset at +0xB8, the OT index at
   +0xC0, the matrix translation at +0xE4 and the four projected corners at
   +0xF0.  Its +0x04 is u16 BECAUSE the function reads it back as `(s16)`:
   the `lhu`/`sll 16`/`sra 16` triple is only reachable from a u16 field with
   an explicit cast -- an s16 field folds to one `lh`. */
typedef struct UnkDungeonSP9
{
    s16 unk_00;
    s16 unk_02;
    u16 unk_04;
    u8 pad_06[0x2];
    UnkDungeonSPW unk_08;
    UnkDungeonSPW unk_0C;
    UnkDungeonSPW unk_10;
    UnkDungeonSPW unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    UnkDungeonSPW unk_30;
    UnkDungeonSPW unk_34;
    UnkDungeonSPW unk_38;
    u8 pad_3C[0x34];
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
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x20];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
} UnkDungeonSP9;
/* func_800BC3E4 calls this one; without the forward declaration the call
   rides an implicit one and the definition then contradicts it. */
void func_800BC4D4(UnkVec *, UnkSlot *, s16, s32);
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
/* The 0x30-byte renderable template func_800D6DC0 copies. WORD members, not
   `u8 b[0x30]`: a whole-struct assignment picks its copy expansion off
   TYPE_ALIGN, and at alignment 1 cc1psx emits the runtime `or`/`andi 3` test
   plus an lwl/lwr path -- 163 words against 139. */
typedef struct UnkDungeonWord48
{
    u32 w[0xC];
} UnkDungeonWord48;
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
/* Not void: the body ends in `addu $v0, $zero, $zero`, the same node-walk
   `return 0` csrc/unk_19434.c's func_80046398 carries.  Its only three uses
   are address-takes, so nothing else moves. */
s32 func_800B06F0();
extern u8 D_800E1A88[];
extern u8 D_800E1A9D[];
extern u8 D_800E1AC2[];
extern u8 D_800E1AD0[];
s32 func_800422A8(u8 *, void *, s32, s32);
s32 func_800BABA8();
extern u8 D_800E03B0[];
extern u8 D_800E03D4[];
extern u8 D_800E03E0[];
void func_800CC744();
UnkNode *func_800CC5F0(s16, s16, s16, s16, s16, s16);
s32 func_800CEEFC(u8 *, UnkVec *, UnkSlot *);
s32 func_8003F794(s16, s16);
void func_8003E1C4(s16);
void func_800AFAD8();
s32 func_800AFBFC();
extern s8 D_800DCF4C;
extern u8 D_800DEF90[];
extern u16 D_800E2924[][2];
extern u8 D_800E1BBC[];
extern void *D_800DF258[];
/* The per-kind hatch func_800B627C dispatches through. */
typedef s32 (*UnkDungeonHatch)(u8 *, s16);
extern UnkDungeonHatch D_800DF2A8[];
/* The +0x9A state dispatch table func_800B274C runs. */
extern void (*D_800DF1E8[])(u8 *, UnkVec *, u8 *, u8 *);
/* func_800D8728's +0x9A handler table, the eight-entry D_800E262C facing
   table's neighbour: the image at 0x800E264C holds code addresses, with
   entry 6 NULL. */
extern void (*D_800E264C[])(u8 *, UnkVec *, u8 *, u8 *);
extern u8 D_80079444[];
extern s16 D_800DCE66;
void func_800B96C4();
extern s16 D_800E2468[][2];
extern u8 D_800E2178[];
extern u8 D_800E221C[];
extern u8 D_800DD7E0[];
extern u8 D_800DD7DC;
void func_8009E038();
s32 func_8009E798(u8 *, UnkVec *, UnkSlot *);
extern u8 D_800E1937[];
extern u8 D_800E1A6D[];
extern void *D_800DCF20;
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
/* ALL FIVE PARAMETERS ARE s16. A narrow parameter is converted in
   assign_parms, so function.c's REG_EQUIV test fails and the pseudo has no
   memory equivalence: reload gives each spill a fresh frame slot instead of
   reusing its incoming argument home slot. Declared with s32s the frame is
   0x40 rather than 0x48 and the function is 144 words of 148. Both landed
   callers stay at 0 differing under the narrowed form. */
u8 *func_800C7380(s16, s16, s16, s16, s16);
s32 func_800B60B8(s16, s16, s16, s16, u8 *);
/* Installed by func_800C7380 into each of its eleven children; defined
   below. */
s32 func_800D0774(u8 *, UnkVec *, UnkSlot *);
/* The two tables func_800C7380 dresses its slots from. */
extern u32 D_800777E8[3];
extern u32 D_800DF3B4[3];
s32 func_800C8CD8(u8 *, s16, s16);
/* Returns the node it allocated -- `addu $v0, $s3, $zero` at two exits. Its
   one caller discards the result, which is why `void` was never falsified. */
UnkNode *func_800C1F28(u8 *, s16);
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
/* Parked at the spawned node's +0x10 by func_800D5294; an address-take needs
   a declaration. One of the seven 4-mod-8 jump-table functions, so it must
   stay INCLUDE_ASM until the unit is carved. */
void func_800D50FC();
/* func_800A982C's and func_80093A94's globals. */
extern u8 D_800DD140[];
extern u8 D_800DCF59[];
extern u8 D_800DD918[];
extern u8 D_800DDAB8[];
extern u8 D_800E58B0[];
extern u8 D_8003E140[];
extern s32 D_8006CD58;
/* The six-byte per-slot flag array func_800A1618 clears and searches, and the
   byte after it that its b == 2 arm clears; the listing takes a separate %hi
   for each, so they are two symbols rather than one array of seven. `s8`
   because the loop reads it with `lb` -- `u8` is 17 differing. */
extern s8 D_800E3DA0[];
extern u8 D_800E3DA6;
/* func_800CBDB4's two message strings. */
extern u8 D_800E1B08[];
extern u8 D_800E1B2E[];
/* The packed-map rectangle func_8009D6F4 hands to the engine's LoadImage;
   splat gives it its own rodata block of two words, which decode as the
   4-halfword UnkDungeonRect {0x300, 0x80, 0x10, 0x40}. */
extern UnkDungeonRect D_80088CB0;
/* func_800A8E74's step, installed into the node's +0x10. */
void func_800A871C();
/* func_800CB82C is defined below in this unit and func_8009D218 above it calls
   it four times, so the prototype has to be here; its renderable template, its
   two-byte tile pair and the step it installs follow. */
UnkNode *func_800CB82C(UnkVec *, u8 *, u8 *);
extern u8 D_800E03BC[];
extern u8 D_800E3CC8[];
void func_800CB600();
/* Both defined below in this unit, and func_800D5DCC takes their addresses. */
s32 func_800D5AC0(u8 *p, UnkVec *v);
void func_800D5CA0(u8 *p, UnkVec *v);
extern u8 D_800DE870[];
extern u8 D_800DCFC8[];
extern u8 D_800E0495[];
extern u8 D_800E09CD[];
extern u8 D_800E09D9[];
extern u8 D_800E09E6[];
extern u8 D_800E09EE[];
extern u8 D_800E09FB[];
void func_80047738(u8 *, s16, s16); /* definition: csrc/unk_1AF38.c:24 */
/* The func_8004491C tag func_8009F21C installs; csrc/ovl_town.c declares it
   the same way. */
extern u8 D_80044BB0[];
/* func_800C4944 parks func_800C4640 at its spawns' +0x10 and hands
   D_800DF564 to each one's renderable; both sit below it. */
void func_800C4640();
extern u8 D_800DF564[];
void func_800481E0(void);
/* The h8 batch. */
void func_800CCDA0();
extern u8 D_800DFD0C[];
extern u8 D_800DFD30[];
extern u8 D_800DDC40[];
/* func_800B0F50's sprite-chain template table and the engine's glyph page. */
extern u8 *D_800DEFF8[];
extern u8 D_80077FEC[];
void func_800CD910(u8 *);
extern u8 D_800DF820[];
/* func_800AB538's pan limit, a halfword of its own: the listing reads it
   with `lh` off its own %hi in one place and `lhu` in the other, which is
   what a declared `s16` gives (`u8 D_80083780[] + 0xA` would have to share
   D_80083780's base register and the listing does not). */
extern s16 D_8008378A;
/* func_800A0B94's three image buffers and the 0xC-byte descriptor it fills.
   The listing takes a %hi/%lo pair for each, not a lui/ori, so they are
   relocated symbols and not constant addresses. */
extern u8 D_800E58A8[];
extern u8 D_80164800[];
extern u8 D_8016A800[];
extern u8 D_80170800[];
/* func_80099C58's two alternate element tables, beside the already-declared
   D_800DE870 it falls back on. */
extern u8 D_800DEDB0[];
extern u8 D_800DEE38[];
/* func_80098614's five message strings and the per-kind byte it copies into
   the record's +0x86. */
extern u8 D_800E07C0[];
extern u8 D_800E07D3[];
extern u8 D_800E0874[];
extern u8 D_800E089C[];
extern u8 D_800E08AB[];
extern u8 D_800E08BA[];
/* func_80098CF8's three failure messages and the name of the key it wants. */
extern u8 D_800E08CC[];
extern u8 D_800E08E5[];
extern u8 D_800E08F1[];
extern u8 D_800E0919[];
extern u8 D_800DD2EC[];
/* func_80095538 calls this one ABOVE its definition; without the forward
   declaration the call rides an implicit one and the definition then
   contradicts it. */
void func_80098614(u8 *, u8 *);
extern u8 D_800DCF58;
extern u8 D_8014A000[];
UnkNode *func_8003FE78(s32, UnkNode *, s16);
/* csrc/ovl_town.c's declaration.  func_8004E5A0 is left on the implicit
   declaration this unit already gives it at func_800B1B10's call sites.
   2026-11-21: the warning that used to be here -- that prototyping
   func_8004E298 "would retype those landed calls" -- is NOT TRUE. Landing
   func_800B1768 put `s8 *func_8004E298(u8 *, u8 *, s32);' at the top of
   csrc/ovl_dungeon.c, above every call site in the unit, and func_800B1B10
   re-measures 220/220 and all four containers come out byte-identical. The
   prototype lives in that unit's own block rather than here because only it
   needs the type. */
u8 *func_8004E634(u32, u8 *);
/* The two +0x10 steps func_800B4C7C parks; an address-take needs a
   declaration above them, and both are still INCLUDE_ASM below. */
void func_800B45E0();
void func_800B490C();
/* The eight glyph strings func_800B4C7C hands func_8004E298 for a negative
   value; indexed by ~v. */
extern u8 *D_800DF24C[];
/* func_80094270's eight message strings. */
extern u8 D_800E0633[];
extern u8 D_800E0726[];
extern u8 D_800E0739[];
extern u8 D_800E0747[];
extern u8 D_800E0766[];
extern u8 D_800E0769[];
extern u8 D_800E077C[];
extern u8 D_800E078A[];
/* func_800927E4's two message strings. */
extern u8 D_800E06EE[];
extern u8 D_800E06F7[];
/* func_800D5594's renderable tables. */
extern u8 D_800E23F8[];
extern u8 D_800E2400[];
extern u8 D_800E2408[];
extern u8 D_800E2420[];
extern u8 D_800E2428[];
extern u8 D_800E2438[];
extern u8 D_800E2440[];
extern u8 D_800E2450[];
extern u8 D_800E2458[];
extern u8 D_800E2460[];
extern s8 D_800DCECC[];
/* func_8008FF18's alternate skin table. */
extern u8 D_800DD0C8[];
/* Defined below; func_8008FF18 calls it above its definition. */
void func_80099F70(u8 *);
/* func_8008F428's shake offsets and its two message strings.  D_800E3DA8 is
   declared as an ARRAY deliberately: as a scalar reached through
   `*(s16 *)((u8 *)&D_800E3DA8 + 2)` the second store's lo_sum lands after the
   intervening call instead of in its delay slot. */
extern s16 D_800E3DA8[];
extern u8 D_800E0523[];
extern u8 D_800E0683[];
extern u8 D_800DD100[];
extern u8 D_800E06EB[];
extern u8 D_800DD150[];
/* Defined below; func_8008F428 calls it above its definition. */
s32 func_800A6DA4(u32, u32);
/* The 0x80010248 record array's 4-byte entry.  func_800945E8 copies one whole
   and the copy comes out as lwl/lwr + swl/swr, so its alignment is 1;
   func_800A4298 reads the same record's +1 and +3 bytes. */
typedef struct Unk80010248
{
    u8 unk_00;
    u8 unk_01;
    u8 unk_02;
    u8 unk_03;
} Unk80010248;
/* Defined below; func_800945E8 calls it above its definition. */
void func_800A9160(void);
/* func_800DAEF4 installs this at each of its thirteen spawns' +0x10 and it is
   defined below, so the address-take needs the declaration here. */
void func_800DB164(u8 *p, UnkVec *v, UnkSlot *r);
/* func_80096100's second palette cursor and its own four-entry table.
   D_800DD26C is NOT D_800DD264 + 8: the function reaches both in one block
   through independent lui/addiu pairs, each with its own `& 3` index. */
extern s16 D_800120A0;
extern s16 D_800DD26C[];
extern u8 D_800E07EF[];
/* func_800D7A78 parks this at each spawn's +0x10 and it is defined below the
   installer, so the address-take needs the declaration here. */
void func_800D7D30(u8 *, UnkVec *, u8 *);
/* The eight-entry facing table func_800DA398 hands to func_8003DB94, and
   whose own address it parks at the renderable's +0x2C. */
extern u8 *D_800E262C[];
/* func_800B7D74's five func_8003DB94 templates. csrc/game_data.c defines the
   last three as `u32 X[]`; only their addresses are wanted here. */
extern u8 D_8006E24C[];
extern u8 D_8006E258[];
extern u8 D_8006E51C[];
extern u8 D_8006E57C[];
extern u8 D_8006E5D4[];
/* The 0x1F800000 scratchpad as func_800BA414 lays it out: the OT base at
   +0x20, the composed matrix at +0x50, RotAverage4's four input vectors at
   +0x70 and its two out-parameters at +0x90/+0x94, the OT index at +0xC0,
   the node's own matrix at +0xD0 (whose `t` is the mover's position at
   +0xE4..+0xEC), the zero rotation handed to RotMatrix at +0x100 and
   ReadRotMatrix's output at +0x11C. */
typedef struct UnkDungeonSP3
{
    u8 pad_00[0x20];
    u8 *unk_20;
    u8 pad_24[0x4];
    SVECTOR unk_28;
    VECTOR unk_30;
    u8 pad_40[0x10];
    MATRIX unk_50;
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
    s32 unk_C4;
    s32 unk_C8;
    s32 unk_CC;
    MATRIX unk_D0;
    s32 unk_F0;
    u8 pad_F4[0xC];
    s32 unk_100;
    s16 unk_104;
    u8 pad_106[0xE];
    s32 unk_114;
    u8 pad_118[0x4];
    MATRIX unk_11C;
} UnkDungeonSP3;
/* The 0x1F800000 scratchpad as func_800C9088 lays it out: Unk463ECSP's model
   view (the scale at +0x30, the translation at +0x40, the four projected
   corners at +0x70, the OT row base at +0x20, the depth at +0xC0 and the
   volatile NormalClip result at +0x114) plus the working MATRIX at +0x50 that
   this one builds by hand out of three RotMatrix axes. */
typedef struct UnkDungeonSP4
{
    u8 pad_00[0x20];
    u32 unk_20;
    u8 pad_24[0xC];
    s32 unk_30;
    s32 unk_34;
    s32 unk_38;
    u8 pad_3C[0x4];
    s32 unk_40;
    s32 unk_44;
    s32 unk_48;
    u8 pad_4C[0x4];
    MATRIX unk_50;
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
    u8 pad_C4[0x50];
    volatile s32 unk_114;
} UnkDungeonSP4;

/* The 0x1F800000 scratchpad as func_800AFFB4 lays it out -- town's
   UnkTownA3B80SP field for field: the four UV scalars at +0x08..+0x17,
   RotAverageNclip4's p/flag/otz out-parameters at +0x90/+0x94/+0xC0, the
   quad's four SVECTORs at +0x70, the four projected screen points at +0xF0,
   the OT base at +0x20 and the texture table base at +0x118. */
typedef struct UnkDungeonSP5
{
    u8 pad_00[0x8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u8 pad_18[0x8];
    u32 unk_20;
    u8 pad_24[0x4C];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x28];
    s32 unk_C0;
    u8 pad_C4[0x2C];
    DVECTOR unk_F0[4];
    u8 pad_100[0x18];
    u32 unk_118;
} UnkDungeonSP5;

/* The 0x1F800000 scratchpad as func_800D6170 lays it out: the OT base at
   +0x24, ScaleMatrix's vector at +0x34, the composed matrix at +0x54, the
   node's own matrix at +0x74 (whose `t` is the mover's position at
   +0x88..+0x90), RotMatrix's input at +0xA4, RotAverage4's four input vectors
   at +0xB0, its p/flag out-parameters at +0xD0/+0xD4 and the OT index at
   +0x100. */
typedef struct UnkDungeonSP6
{
    u8 pad_00[0x24];
    u8 *unk_24;
    s16 unk_28;
    u8 pad_2A[0xA];
    VECTOR unk_34;
    u8 pad_44[0x10];
    MATRIX unk_54;
    MATRIX unk_74;
    u8 pad_94[0x10];
    SVECTOR unk_A4;
    u8 pad_AC[0x4];
    SVECTOR unk_B0[4];
    s32 unk_D0;
    s32 unk_D4;
    u8 pad_D8[0x28];
    s32 unk_100;
} UnkDungeonSP6;
/* The 0x1F800000 scratchpad as func_800D71A8 lays it out: the four UV scalars
   at +0x08..+0x17, the OT base at +0x20 and the pair's mirror flags at +0x24,
   the quad's four SVECTORs at +0x70, RotTransPers4's two out-parameters at
   +0x90/+0x94, the screen offset at +0xB8, the OT index at +0xC0, the four
   projected screen points at +0xF0 and the sprite's half-extents at +0x108. */
typedef struct UnkDungeonSP7
{
    u8 pad_00[0x8];
    u32 unk_08;
    u32 unk_0C;
    u32 unk_10;
    u32 unk_14;
    u8 pad_18[0x8];
    u32 unk_20;
    u16 unk_24;
    u8 pad_26[0x4A];
    SVECTOR unk_70[4];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x2C];
    DVECTOR unk_F0[4];
    u8 pad_100[0x8];
    u16 unk_108;
    u16 unk_10A;
} UnkDungeonSP7;

/* The 0xC-byte texture record the scratchpad's +0x118 points at, town's
   UnkTownA41D8T field for field.  Indexed [flag * 4 + (i & 1) * 2 + half]. */
typedef struct UnkDungeonAFFB4T
{
    u8 pad_00[0x4];
    s16 unk_04;
    u16 unk_06;
    u8 unk_08;
    u8 unk_09;
    u8 unk_0A;
    u8 unk_0B;
} UnkDungeonAFFB4T;
/* A 0x20-byte face record, the gouraud-textured cousin of the resident
   Unk463ECF (0x18 bytes): four vertex indices, two packed UV/CLUT words, FOUR
   colour-table indices instead of one, the +0x1A/+0x1C UV pair and the
   +0x1E/+0x1F end-of-list pair. */
typedef struct UnkDungeonFace
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u32 unk_08;
    u32 unk_0C;
    u16 unk_10;
    u16 unk_12;
    u16 unk_14;
    u16 unk_16;
    u8 pad_18[0x2];
    u16 unk_1A;
    u16 unk_1C;
    u8 unk_1E;
    s8 unk_1F;
} UnkDungeonFace;
/* RotAverage4. Kept on ONE line: a wrapped declaration's first line carries
   no `;`, so the whole-unit sweep's awk recipe takes it for a definition and
   ovlverify then hard-exits on it. */
/* func_800A48F0 (above) calls this one; without the forward declaration the
   call rides an implicit one and the definition then contradicts it. */
void func_800A4B88(s8 *, s16);
/* func_800B96C4's cursor into the party record: the `%lo` sits inside the
   `lw`, so it is a declared SCALAR holding a pointer. */
extern u8 *D_800DF364;
/* func_8008E264's state-0/8/10 skin table, read one byte per facing. */
extern u8 D_800DD040[];
/* func_80090298's state-0 skin table, one byte per facing. */
extern u8 D_800DD0F0[];

/* func_800982A8's four message strings, the +0x4C swap's own string in this
   overlay's rodata, and the two records it parks at D_800E3D18. */
/* This unit's own INCLUDE_RODATA, below the use. */
extern u8 D_80088B64[];
extern u8 D_800E080A[];
extern u8 D_800E081C[];
extern u8 D_800E0844[];
extern u8 D_800E0853[];
extern u8 D_800E0862[];
extern u8 *D_800E3D18;
extern u8 D_800E3CF8[];
extern u8 D_800E3D80[];
/* func_800982A8's four per-kind byte tables: +0x84 and +0x85 come from the
   D_800DD2E0/D_800DD2D4 pair when the entry's kind is 0x10 and from the
   D_800DD2C4/D_800DD2B4 pair otherwise. */
extern u8 D_800DD2C4[];
extern u8 D_800DD2B4[];
extern u8 D_800DD2E0[];
extern u8 D_800DD2D4[];
/* The 12 bytes at 0x80013710 that func_800930F0 copies down to 0x80012080.
   Alignment 2: the copy comes out as lwl/lwr + swl/swr pairs, so this cannot
   be the 0x10-byte UnkDungeon13710 above. */
typedef struct UnkDungeon12080
{
    u16 unk_00[6];
} UnkDungeon12080;
/* func_800930F0's case-3 slot: the already-declared `u8 *D_800E4938` is the
   function pointer and the two words after it are its arguments, reached off
   one base. */
extern u32 D_800E4938_a[] __asm__("D_800E4938");
/* The handler func_800930F0 parks at D_800E4938.  It is a function, but
   dungeon.bin_undefined_syms_auto.txt only carries the `D_` spelling. */
extern u8 D_80021268[];

/* func_800CE748's four slot-tick message strings.  Four independent symbols,
   not offsets off D_800E1B5D: that is this run's practice in the unit and the
   only spelling the listing's own %hi/%lo names support. */
extern u8 D_800E1B76[];
extern u8 D_800E1B87[];
extern u8 D_800E1B99[];
extern u8 D_800E1BAA[];
/* func_80095A10's state-0 skin table, read one byte per facing, and its
   case-1/2 handshake byte -- a declared SCALAR, since the `%lo` sits inside
   the `lbu`. */
extern u8 D_800DD138[];
/* func_800B3D10's speed divisor, a declared SCALAR -- the `%lo` sits inside
   the `lbu`, the same reading D_800E3E40 below carries. */
extern u8 D_800E3D68;
/* func_800B348C's nine per-state skin tables, read one byte per facing the
   same way D_800DD030 above is, plus the two the +0xCC slot is loaded with
   when the record's +0x4C is set and when it is not. */
extern u8 D_800DD068[];
extern u8 D_800DD070[];
extern u8 D_800DD078[];
extern u8 D_800DD080[];
extern u8 D_800DD088[];
extern u8 D_800DD090[];
extern u8 D_800DD098[];
extern u8 D_800DD0A0[];
extern u8 D_800DD0A8[];
extern u8 D_800DD0B0[];
/* func_800B348C's two engine globals: the saved +0x14 fall rate it parks
   across the knock-back, and the state-5 flag it sets and the state-10 tail
   tests.  Both are declared SCALARS -- the `%lo` sits inside the `sw`/`lh`. */
extern s32 D_800DF244;
extern s16 D_800DF248;
extern u8 D_800E3E40;
/* Resident helpers func_80095A10 needs; prototypes copied from their
   definitions in csrc/unk_1AF38.c and csrc/unk_154BC.c. */
u8 *func_80047DB8(s32);
void func_80047DF0(void);
void func_80043914(u8 *);
void func_800424E0(u8 *, s16, u8 *);
void func_80042560(s8 *);

/* The 0x1F800000 scratchpad as func_800CEFB8 lays it out: the resident
   func_8003DBD0's Unk453E0SP with one more halfword witnessed at +0x10E --
   the ratan2 bearing between the renderable's two projected points, read
   back when the caller's fifth argument is zero. */
typedef struct UnkDungeonSP8
{
    s16 unk_00;
    s16 unk_02;
    s16 unk_04;
    u8 pad_06[0x2];
    Unk3DBD0W unk_08;
    Unk3DBD0W unk_0C;
    Unk3DBD0W unk_10;
    Unk3DBD0W unk_14;
    u8 pad_18[0x8];
    u8 *unk_20;
    u16 unk_24;
    u8 pad_26[0xA];
    Unk3DBD0W unk_30;
    Unk3DBD0W unk_34;
    Unk3DBD0W unk_38;
    u8 pad_3C[0x14];
    u16 unk_50;
    u8 pad_52[0x6];
    u16 unk_58;
    u8 pad_5A[0x16];
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
    u8 pad_8E[0x2];
    s32 unk_90;
    s32 unk_94;
    u8 pad_98[0x20];
    u16 unk_B8;
    u16 unk_BA;
    u8 pad_BC[0x4];
    s32 unk_C0;
    u8 pad_C4[0x8];
    s32 unk_CC;
    u8 pad_D0[0x14];
    s32 unk_E4;
    s32 unk_E8;
    s32 unk_EC;
    u16 unk_F0;
    u16 unk_F2;
    u16 unk_F4;
    u16 unk_F6;
    u16 unk_F8;
    u16 unk_FA;
    u16 unk_FC;
    u16 unk_FE;
    s16 unk_100;
    s16 unk_102;
    s16 unk_104;
    u8 pad_106[0x2];
    u16 unk_108;
    u16 unk_10A;
    u8 pad_10C[0x2];
    u16 unk_10E;
    u8 pad_110[0x2C];
} UnkDungeonSP8;
/* The 0x28-byte flat-textured primitive func_800CEFB8 emits -- libgpu's
   POLY_FT4, with the three u/v pairs that are read back a byte at a time
   written through Unk453E0UV the way UnkDungeonGT4 above does. */
typedef struct UnkDungeonFT4
{
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    Unk453E0UV unk_0C;
    u16 unk_0E;
    u16 unk_10;
    u16 unk_12;
    Unk453E0UV unk_14;
    u16 unk_16;
    u16 unk_18;
    u16 unk_1A;
    Unk453E0UV unk_1C;
    u16 unk_1E;
    u16 unk_20;
    u16 unk_22;
    Unk453E0UV unk_24;
    u16 unk_26;
} UnkDungeonFT4;
/* A 4-byte slot of the primitive below that is written as a WORD and read
   back as a halfword or a byte -- the u/v + CLUT and u/v + TPAGE pairs, which
   func_800A7CC4 fills with one `sw` each and then decrements a byte at a
   time. UnkDungeonGT4's Unk453E0UV is the 2-byte version of the same idea. */
typedef union UnkDungeonPUV
{
    u32 w;
    u16 h;
    u8 b[2];
} UnkDungeonPUV;
/* The same 0x28-byte POLY_FT4 as UnkDungeonFT4 above with its three u/v
   slots under their WORD face: func_800A7CC4 writes each u/v pair and the
   CLUT or TPAGE beside it in ONE `sw` (the CLUT arrives as `<< 16`), where
   func_800CEFB8 writes the two halves separately. */
typedef struct UnkDungeonFT4W
{
    u32 unk_00;
    u32 unk_04;
    u16 unk_08;
    u16 unk_0A;
    UnkDungeonPUV unk_0C;
    u16 unk_10;
    u16 unk_12;
    UnkDungeonPUV unk_14;
    u16 unk_18;
    u16 unk_1A;
    UnkDungeonPUV unk_1C;
    u16 unk_20;
    u16 unk_22;
    UnkDungeonPUV unk_24;
} UnkDungeonFT4W;

/* Rodata this unit owns; func_80091430 hands one of the three to
   func_8004DD2C. */
extern u8 D_80088904[];
extern u8 D_80088934[];
extern u8 D_80088970[];
extern u8 D_80012D6D;
extern u8 D_80023C58[];
extern u8 D_800E2004[];
/* Resident helpers func_80091430 needs; prototypes copied from their
   definitions in csrc/unk_1C540.c. */
void func_8004DCE0(u32);
void func_8004DCEC(void);
void func_8004DD2C(u8 *);
void func_8004E130(void);
/* Defined below; the s16 return is what the `sll'/`beqz' on the result in
   func_80091430 tests. */
s16 func_8009402C(u8 *, s32, u8 *, s16 *, s16 *, u8 *);

/* The overlay's per-frame entry point; `void func_800894A0(void)` is declared
   in include/structs.h because the resident engine calls it. D_80083160 is a
   POINTER LOCAL assigned before the first call -- that is where the
   `lui`/`addiu` pair in the prologue comes from. 58 words, 0 differing. */

#endif
