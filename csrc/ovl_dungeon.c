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
    u8 pad_90[0xA];
    /* func_8008D330 and func_80091920 park a state byte here and clear the
       one after it in the same breath as clearing the handler slot, which is
       the witness that +0x8C and +0x9A are fields of one record. */
    u8 unk_9A;
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
void func_800D4494();

/* The six flag probes and their three callers. func_800A6D30's result is
   masked to 16 bits at every use, so it is declared u16 rather than cast at
   three sites; func_800A48F0 is declared in include/structs.h, and the note
   there says why its return type was corrected from void. */
u16 func_800A6D30();
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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C468);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C514);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C5C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C7B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008C8BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CAA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CBA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CBD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CCB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CD4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008CF6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D024);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D1D0);

/* Reset an actor: a state byte, the byte after it, and the handler slot.
   The store order is the source order -- three independent stores, nothing
   for reorg to reassociate. */
void func_8008D330(UnkDungeonActor *p)
{
    p->unk_9A = 0x21;
    p->unk_9B = 0;
    p->unk_8C = NULL;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D344);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008D368);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008EA40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008EAC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F428);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F6EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008F878);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FA14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FA7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FD50);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8008FF18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80090200);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091934);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091958);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091A38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091B5C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088904);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088934);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088970);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80091C64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092018);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800924F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009255C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800927E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092BB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092DFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80092E90);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800930F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009345C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093A94);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093C70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093D8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80093E74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009402C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094208);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094270);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094548);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009456C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009458C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800945C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800945E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800948BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094988);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094E34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094ED4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80094F74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095464);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800954E0);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088A80);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088A84);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095538);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800956B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095854);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095A10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095DD0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80095DFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096088);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096100);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096384);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800969B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096A54);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096AC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096AE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80096C24);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009704C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800971A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800973F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800974DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097898);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097934);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097A68);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097AF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80097C50);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098C48);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098C80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098CEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098CF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098FB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80098FF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009904C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099090);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800990C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800990FC);

/* A one-word publish, twice: the overlay's own slot and an engine one. */
void func_80099188(s32 v)
{
    D_800DCF60 = v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099194);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099254);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099290);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009929C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800992A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800992E8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088B64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099368);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009955C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009965C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099734);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800997FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099844);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800998A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800998C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099978);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800999B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800999D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099A1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099B18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099C58);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099EA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099F04);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099F70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_80099FDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A028);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A078);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A0FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A180);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A21C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A2B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A350);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A3D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A540);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A66C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009A8C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ABA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ADB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009AF18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B164);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B25C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B390);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B4B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B5AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B7E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009B88C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009BE2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009BF7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009BFF8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009C12C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009C93C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009CD58);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009CE1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D218);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D380);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D3B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D6F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009D8A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009DA70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009DAD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009DF0C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CA8);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CAC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80088CB0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009E038);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009E798);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009ED30);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009EE4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009EEAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009EF04);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009EF78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F020);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F0B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F13C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F21C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F3D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F5CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F644);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F868);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F988);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009F9E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FAAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FAC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FADC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FB34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FBF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FCAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FCF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FD40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FD7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FE94);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_8009FFD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0134);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0150);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A020C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A02AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A03C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A04F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0548);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A05A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A07D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0818);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A08A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0B94);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0DD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0E44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A0E6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A152C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A15B0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A1618);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A17E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A18E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A19E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A1B44);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2AE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2B04);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2B28);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2B5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2BDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2C34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2C78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2CAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2CB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2D68);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2DB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A2FE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A31D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A32A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A34BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3518);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3544);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A35A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A35D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A365C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A36B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3820);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A384C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3D18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A3F28);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A404C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A40AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A41F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4298);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4300);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A442C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4474);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A44E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A45D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4688);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4778);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A48F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4ACC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089000);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4B88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A4E2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A504C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5690);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A56C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A56E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5720);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A57B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5828);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5A18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5AF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5C70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5D44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5D5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5DBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5DFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5F38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A5FC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6104);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A613C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6194);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A624C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A634C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A63B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6480);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6508);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A652C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6620);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6780);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A67F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6870);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A68C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A68F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6928);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A697C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A69E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6A6C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6A9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6B70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6BBC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6C00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6D30);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6D60);

void func_800A6D98(s32 v)
{
    D_8008335C = v;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6DA4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6E10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A6E8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7030);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7700);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A77AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A794C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7A38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A7A7C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A86DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A871C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A8E74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9024);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A90E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9160);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9230);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9390);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9400);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9E38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800A9E70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA258);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA36C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA508);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA53C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA5E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA6B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA754);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA79C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA888);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AA924);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AAA28);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AAA54);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC7EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AC82C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACB98);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACBE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACD74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACE34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ACF88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD058);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD4D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD568);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD594);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD6FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AD9B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADAE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADB8C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADBCC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADC4C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADD20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADDA0);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089074);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089080);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089084);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800ADE74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFAD8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFBFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800AFFB4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0544);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0654);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B06C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B06F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0BE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0D34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B0F50);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B12F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1320);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B135C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B13AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B13CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1400);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1434);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1484);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B14B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B14CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B14DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B14FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1768);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1B10);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1E80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1F00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B1F34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2074);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B21B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B2230);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B341C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B5ED0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B5F80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6008);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B60B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6204);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6228);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B627C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B63A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B653C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B66C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6814);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B69DC);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_800892F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B6AC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7774);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7B60);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7C14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B7D74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8024);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B80D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8228);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B835C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B84E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8830);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8C20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8CF0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8D64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8E18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8EA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B8FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9144);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B96C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9964);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800B9A78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA074);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA33C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA384);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA414);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA6B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA764);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BA810);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BABA8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BAF04);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB044);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB0A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB14C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB2E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BB55C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BBA20);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC1B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC228);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC26C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC318);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC388);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC3E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BC4D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BCA68);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BCAD0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BCFC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD074);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD124);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD158);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD184);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD1C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD45C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD5C4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD72C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BD86C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BDA44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BDB74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BDC98);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BDEB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE120);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE360);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE4E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE5A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE6F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BE9E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BEB30);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BEDEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF074);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF210);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF34C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF478);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF5A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF6D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF7FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BF9EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BFC00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BFD80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800BFF00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C003C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0180);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0230);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0404);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C07AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C08A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0B60);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C0E88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C135C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C13C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C152C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C15B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1718);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1A44);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1B1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1CE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1EC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C1F28);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2130);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C22EC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C24A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C261C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C27F0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2C7C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2CDC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C2F2C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C30D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3238);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C333C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3440);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3690);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C37C8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3900);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3AE0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3C08);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C3D3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4030);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4220);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4324);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4490);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4640);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4944);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4AFC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4C00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4D78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4E00);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C4EB4);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C6AB0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C78A0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7930);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7A3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7B38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7D54);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7DEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C7F68);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8078);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C80F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8150);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C82B8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C857C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8788);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8ABC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C8B84);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C95C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9664);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C96E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9850);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9AAC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800C9F34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA0DC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA1E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA444);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA788);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CA93C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CAA94);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CBF88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC058);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC18C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC2A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC370);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC4F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC5F0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC744);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC88C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CC9BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCA14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCC20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CCDA0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD004);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD1A4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD294);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD384);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD474);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD6AC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD6E0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD910);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CD994);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDA70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDC18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDD0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDE0C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDE40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CDF40);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE028);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE4E8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE69C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CE6F0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800CEF54);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1338);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D13B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1648);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D169C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D175C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1824);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1A80);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1C70);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D1E34);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D20D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D21CC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D24A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D2664);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D28E4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D29D8);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D2CB4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D2E9C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D3140);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D3234);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D34CC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D36D0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D3738);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D3F74);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4158);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4494);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4634);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D48CC);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D4FC8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D50FC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5294);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5460);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5594);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D586C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D58D8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5AC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D5CA0);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D68F4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6AD4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6C18);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6DC0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D6FEC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D707C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D71A8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D78C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7960);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D79B0);

s32 func_800D7A04(void)
{
    return 0;
}

s32 func_800D7A0C(void)
{
    return 0;
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7A14);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7A78);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7D30);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7F38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D7FB8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D81D4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8590);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8728);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D8C64);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D904C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D92C0);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D9820);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800D9884);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB618);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DB660);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBA90);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBC20);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBD5C);

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

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBF38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBF5C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DBF94);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC000);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC078);

void func_800DC0B4(void)
{
}

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC0BC);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC168);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC1B8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC1F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC5B4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC628);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC650);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC724);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC82C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC888);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC8F8);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DC988);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCA1C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCA88);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCAC4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCBE4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCC3C);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCCF4);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCD38);

INCLUDE_ASM("asm/nonmatchings/ovl_dungeon", func_800DCE0C);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089490);

INCLUDE_RODATA("asm/nonmatchings/ovl_dungeon", D_80089498);
